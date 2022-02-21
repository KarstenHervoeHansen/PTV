#include <C8051F320.h>
#include <string.h>
#include "ini.h"
#include "globals.h"
#include "i2c_bus.h"
#include "i2c_slave.h"
#include "serial.h"
#include "ublox_prot.h"
#include "LTC_handler.h"
#include "pt5300_handler.h"
#include "EEPROM_drv.h"

//den nuv�rende softwareversion i FPGA (!! SKAL stemme overens med MCU)
unsigned char current_FPGA_sw_version=0x10;
//den nuv�rende MCU software version (bliver vist i PT5300) 10'ere = version, 1'ere = revision
unsigned char current_MCU_sw_version=10;
//software version i FPGA
unsigned char FPGA_software_version=0;

bit ext1_rise_flag, ext1_fall_flag;

//FPGA status inputs
sbit timepulse_status = P1^0;

//LED out
sbit LED1 = P2^0;
sbit LED2 = P2^1;

//Jumper in
sbit MCU_jumper = P2^6;

//reset out til FPGA
sbit FPGA_reset = P2^2;

//t�ller til diverse form�l
unsigned char counter=0;

#define pps_i					0
#define int_pps					1
#define int_clk					2
#define kvant_pps				3
#define pps_148_tick			4
#define ms_tick					5
#define SPG_sync_signal			6
#define PAL_sync				7
#define NTSC_sync				8
#define int_pps_tick_100mhz		9
#define longer_tick				10
#define I2C_start				11
#define I2C_update				12
#define I2C_stop				13
#define I2C_debug				14
#define LTC_a_debug				15
#define LTC_b_debug				16
#define ref_clk					17
#define none					18

// !! OBS !! Alting p�  TP3 er forl�nget til 1 ms ekstra
char tp_select[3] = {int_pps, int_pps_tick_100mhz, pps_148_tick};

/********* Master status byte *************
Bit 0: Timepulse status
Bit 1: GPS PLL l�st status
Bit 2: 
Bit 3: 
Bit 4: 
Bit 5: 
Bit 6: 
Bit 7: 

*******************************************/
unsigned char master_status_byte;


/********* FPGA system control *************
Bit 0: 0 = PAL, 1 = NTSC
Bit 1: 1 = 4 sekunder PAL sync, 0 = ingen sync (h�j for hver 4. sekund, tilpasset GPS tid)
Bit 2: 1 = reset OCXO, 0 = aktiv OCXO
Bit 3: 1 = hurtigt OCXO filter, 0 = langsomt OCXO filter
Bit 4: 1 = OCXO fixed, 0 = OCXO tracking
Bit 5: 
Bit 6: 
Bit 7: 

*******************************************/
unsigned char FPGA_system_control;

// KU streng, indeholder KU nummer for GPS board. Er gemt i EEPROM bank 0, byte 200-210
unsigned char KU_str[11];

// L�st v�rdi for OCXO styresp�ning, gemmes til n�ste reboot
unsigned char OCXO_value_hi, OCXO_value_lo;
int OCXO_phase_diff;

union {
	unsigned int OCXO_dac_value_int;
	unsigned char OCXO_dac_value_bytes[2];
} OCXO_dac_value;

union {
	unsigned int OCXO_avg_dac_value_int;
	unsigned char OCXO_avg_dac_bytes[2];
} OCXO_avg_dac_value;

bit OCXO_locked_status;
unsigned char OCXO_counter=0;

//tolerance for OCXO faseforskel, for switch mellem hurtigt/langsomt loop
#define OCXO_tolerance		32

//tolerance for tidsrum, der skal v�re l�st i/ude af l�s, f�r skift af loophastighed
#define OCXO_time			100

//rising edge p� syntesiseret 1 PPS
void int_rise() interrupt 0
{
	ext1_rise_flag=1;
}

//falling edge p� syntesiseret 1 PPS (500 ms efter GPS PPS)
void int_fall() interrupt 2
{
	ext1_fall_flag=1;
}



/****************************************************************************
*																			*
*						GPS Genlock main rutine								*
*																			*
*		Hovedprogram for GPS					Udgivet 05/02/08			*
*		Udvikler: Jens K Hansen												*
*																			*
*		Revision 0: Oprettet												*
*		Revision 1: 5/2/2008: Udgivet som version 1.0						*
*																			*
****************************************************************************/

void main()
{
	config();

	//Setup sekvens
	setup_I2C();

	//enable interrupts. M� ikke flyttes, da SMBUS skal v�re enabled, for at kunne registrere modul i mainframe
	EA=1;

	//reset FPGA s� l�nge FPGA ikke er boot'et og der ingen tidspuls er
	FPGA_reset=1;
	LED1 = 0;
	LED2 = 0;

	//Indl�s KU nummer
	for(counter=0;counter<11;counter++)
		KU_str[counter]=EEPROM_read_byte(0, 200+counter);

	setup_UART();
	set_UART38400();
	init_LTC();

	//indl�s genlock system setup  fra EEPROM, og skriv denne til FPGA (masker OCXO kontrol v�k)
	FPGA_system_control=EEPROM_read_byte(0, 0)&0x01;
	
	//delay, mens FPGA skal n� at loade PROM kode (SW version kan l�ses)
	while(FPGA_software_version != current_FPGA_sw_version)
		FPGA_software_version=FPGA_read_byte(32);

	/////////////////////// SETUP TESTPOINTS ///////////////////////
	FPGA_write_array(23, &tp_select, 3);

	/////////////////////// SETUP OCXO /////////////////////////////
	//l�s gemt OCXO styresp�nding
	OCXO_dac_value.OCXO_dac_value_bytes[0] = EEPROM_read_byte(0, 1);
	OCXO_dac_value.OCXO_dac_value_bytes[1] = EEPROM_read_byte(0, 2);

	//hvis f�rste gangs boot, s�t default v�rdi
	if (OCXO_dac_value.OCXO_dac_value_int == 0)
		OCXO_dac_value.OCXO_dac_value_int = 23000;

	//OCXO = reset + hurtigt loop filter
	set_bit(&FPGA_system_control, 2);
	set_bit(&FPGA_system_control, 3);
	FPGA_write_byte(20, FPGA_system_control);
	
	//send startop styresp�nding til FPGA
	FPGA_write_byte(21, OCXO_dac_value.OCXO_dac_value_bytes[1]);
	FPGA_write_byte(22, OCXO_dac_value.OCXO_dac_value_bytes[0]);

	//////////////////// SETUP UBLOX OUTPUT ////////////////////////
	//setup UBlox LEA 4T modul
	setup_ublox();

	//send kvantiseringsfejl 1 gang pr. sekund
	set_message_rate(0x0D, 0x01, 1);

	//set GPS tid ud 1 gang pr. sekund
	set_message_rate(0x01, 0x20, 1);

	//set UTC tid ud 1 gang pr. sekund
	set_message_rate(0x01, 0x21, 1);
	
	//set survey in progress ud 1 gang pr. 10 sekunder
	set_message_rate(0x0D, 0x04, 10);

	//f�rdig med initialisering
	FPGA_reset=0;
	
	counter=0;
	LED1= 1;

	//main l�kke
	while(1)
	{	
		//pak master status streng
		if(timepulse_status)
			set_bit(&master_status_byte, 0);
		else
			clear_bit(&master_status_byte, 0);

		if(OCXO_locked_status)
			set_bit(&master_status_byte, 1);
		else
			clear_bit(&master_status_byte, 1);



//////////////////////////////////////////////////////
//			kontinuerlige processer					//
//////////////////////////////////////////////////////

		//process�r indkommende uBlox data
		packet_process();

		//Check for bekr�ftelse af skift til sommer/vintertid
		confirm_daylight_switch();

		//hvis modtaget data fra PT5300
		if(i2c_msgupdate==1)
		{	
			LED1=0;
			pt5300_in_handle();
			i2c_msgupdate=0;
		}

		//hvis hel besked modtaget
		if(msg_update_flag==1)
		{	
			LED1=0;
			interpret_message();
			msg_update_flag=0;
		}


//////////////////////////////////////////////////////
//	    		synkrone processer					//
//////////////////////////////////////////////////////

		//hvis stigende puls (synkron med GPS PPS)
		if(ext1_rise_flag==1)
		{
			LED1=0;

			////////////////////// OCXO styring ///////////////////////////
			//l�s OCXO faseforskel
			OCXO_phase_diff = (FPGA_read_byte(36)*256) + FPGA_read_byte(35);

			//hvis indenfor l�setolerance, men ikke l�st endnu
			if((!OCXO_locked_status) && timepulse_status && (OCXO_phase_diff<OCXO_tolerance) && (OCXO_phase_diff>(-OCXO_tolerance)))
			{
				
				//hvis l�st i lang tid nok
				if(OCXO_counter>OCXO_time)
				{
					OCXO_counter=0;
					OCXO_locked_status=1;				//angiv som l�st!
					clear_bit(&FPGA_system_control, 3);	//s�t til langsomt loop-filter
				}
				//ellers t�l p� l�setiden
				else			
					OCXO_counter++;
			}
			//ellers hvis l�st, men ikke indenfor tolerance eller ingen GPS kontakt
			else if (OCXO_locked_status && ((!timepulse_status) || (OCXO_phase_diff>OCXO_tolerance) || (OCXO_phase_diff<(-OCXO_tolerance))))
			{
				//hvis ude af l�s i for lang tid
				if(OCXO_counter>OCXO_time)
				{
					OCXO_counter=0;
					OCXO_locked_status=0;				//angiv som ikke l�st!
					set_bit(&FPGA_system_control, 3);	//s�t til hurtigt loop-filter
				}
				//ellers t�l p� l�setiden
				else			
					OCXO_counter++;
			}

			update_LTC_strings();
			check_daylight_switch();

			if(timepulse_status)
				counter++;
			else
				counter=0;

			//hvis ingen tidspuls
			if(!timepulse_status)
			{
				//send gennemsnits styresp�nding til DAC
				FPGA_write_byte(21, OCXO_dac_value.OCXO_dac_value_bytes[1]);
				FPGA_write_byte(22, OCXO_dac_value.OCXO_dac_value_bytes[0]);

				//s�t OCXO til fixed mode
				set_bit(&FPGA_system_control, 4);
			}
			else
			{
				//hvis GPS signal tilg�ngeligt, sl� fixed mode fra
				clear_bit(&FPGA_system_control, 4);
			}

			//hvis PLL'en er l�st, log styresp�nding i EEPROM + udregn gennemsnits styresp�nding
			if(OCXO_locked_status && timepulse_status)
			{
				OCXO_dac_value.OCXO_dac_value_bytes[1]=FPGA_read_byte(33);
				OCXO_dac_value.OCXO_dac_value_bytes[0]=FPGA_read_byte(34);

				//beregn gennemsnits DAC v�rdi
				OCXO_avg_dac_value.OCXO_avg_dac_value_int-=OCXO_avg_dac_value.OCXO_avg_dac_value_int/16;
				OCXO_avg_dac_value.OCXO_avg_dac_value_int+=OCXO_dac_value.OCXO_dac_value_int/16;

				//gem OCXO styresp�nding over to(!!) sekunder, for ikke at forsinke MCU
				if(counter==1)
					EEPROM_write_byte(0, 2, OCXO_dac_value.OCXO_dac_value_bytes[1]);

				if(counter==2)
					EEPROM_write_byte(0, 1, OCXO_dac_value.OCXO_dac_value_bytes[0]);
			}

			//OCXO PLL har nu modtaget start op v�rdi, s� aktiver (der ventes 15 sek)
			if ((counter == 15) && (UTC_time_valid==0x07) && timepulse_status)
			{
				clear_bit(&FPGA_system_control, 2);
				LED2=1;
			}

			ext1_rise_flag=0;
		}


		
		//hvis faldende puls (500 ms efter stignede puls) (!! Ikke flere kommandoer her, tidsmangel ??)
		if(ext1_fall_flag==1)
		{
			LED1=0;

			//h�ndter tid
			LTC_manager();
			transmit_LTC_data();
			ext1_fall_flag=0;
		}

		//hvis ingen aktivitet, sluk LED1
		LED1=1;
	}
}


//S�tter en bit h�j i *byte
void set_bit(unsigned char *byte, unsigned char bit_number)
{
	*byte=*byte|(1<<bit_number);
}

//S�tter en bit lav i *byte
void clear_bit(unsigned char *byte, unsigned char bit_number)
{
	*byte=*byte&~(1<<bit_number);
}