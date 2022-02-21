#include <C8051F320.h>
#include <string.h>
#include <stdio.h>
#include "ini.h"
#include "globals.h"
#include "i2c_bus.h"
#include "i2c_slave.h"
#include "serial.h"
#include "ublox_prot.h"
#include "LTC_handler.h"
#include "pt5300_handler.h"
#include "EEPROM_drv.h"
#include "SW_uart.h"

//den nuværende softwareversion i FPGA (!! SKAL stemme overens med MCU)
unsigned char code current_FPGA_sw_version=0x10;
//den nuværende MCU software version (bliver vist i PT5300) 10'ere = version, 1'ere = revision
unsigned char code current_MCU_sw_version=11;
//læst software version i FPGA
unsigned char FPGA_software_version=0;
bit ext1_rise_flag, ext1_fall_flag;

//FPGA status inputs
sbit timepulse_status = P1^0;

//LED out
sbit LED1 = P2^0;
sbit LED2 = P2^1;

//reset out til FPGA
sbit FPGA_reset = P2^2;

//tæller til diverse formål
unsigned char counter;

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

// !! OBS !! Alting på  TP3 er forlænget til 1 ms ekstra
char tp_select[3] = {int_pps, int_pps_tick_100mhz, pps_148_tick};
unsigned char xdata debug_buffer[64];

/********* Master status byte *************
Bit 0: Timepulse status
Bit 1: GPS PLL låst status
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
Bit 1: 1 = 4 sekunder PAL sync, 0 = ingen sync (høj for hver 4. sekund, tilpasset GPS tid)
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


int OCXO_phase_diff;

// Læst værdi for OCXO styrespæning, gemmes til næste reboot
union {
	unsigned int OCXO_dac_value_int;
	unsigned char OCXO_dac_value_bytes[2];
} OCXO_dac_value;

union {
	unsigned int OCXO_avg_dac_value_int;
	unsigned char OCXO_avg_dac_bytes[2];
} OCXO_avg_dac_value;

bit OCXO_locked_status;
unsigned char OCXO_counter;

//tolerance for OCXO faseforskel, for switch mellem hurtigt/langsomt loop
#define OCXO_tolerance		32

//tolerance for tidsrum, der skal være låst i/ude af lås, før skift af loophastighed
#define OCXO_time			100

//rising edge på syntesiseret 1 PPS
void int_rise() interrupt 0
{
	ext1_rise_flag=1;
}

//falling edge på syntesiseret 1 PPS (500 ms efter GPS PPS)
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
*		Revision 2: 28/4/2008: 												*
*					1.Rettet opstartssekvens til hurtigere bootup  			*
*					2.Tilføjet SW UART på P2.6 = jumperpin til debug		*
*					Sæt DEBUG = 1 i globals.h, SW_UART.c tilføjet til		*
*					projektet												*
*					3.Resetter alle variable								*
*					4. Ændret styringslogik til bedre håndtering af			*
*						udfald og OCXO og hurtigere opstart					*
*																			*
*																			*
****************************************************************************/

void main()
{
	config();

	//29/4/08 - rev 2: Reset alle variable
	ext1_rise_flag=0;
	ext1_fall_flag=0;
	OCXO_locked_status=0;
	OCXO_counter=0;
	OCXO_phase_diff=0;
	OCXO_dac_value.OCXO_dac_value_int=0;
	OCXO_avg_dac_value.OCXO_avg_dac_value_int=0;
	FPGA_system_control=0;
	master_status_byte=0;
	counter=0;
	memset(debug_buffer, 0, sizeof(debug_buffer));

	//Setup sekvens
	setup_I2C();
				
	//enable interrupts. Må ikke flyttes, da SMBUS skal være enabled, for at kunne registrere modul i mainframe
	EA=1;

	//reset FPGA så længe FPGA ikke er boot'et og der ingen tidspuls er
	FPGA_reset=1;
	LED1 = 0;
	LED2 = 0;

	//Indlæs KU nummer
	for(counter=0;counter<11;counter++)
		KU_str[counter]=EEPROM_read_byte(0, 200+counter);

	setup_UART();
	set_UART38400();
	init_LTC();

	//29/4/08: Nyt! Tilføjet software UART.
	//setup_SW_uart();
	
	//indlæs genlock system setup  fra EEPROM, og skriv denne til FPGA (masker OCXO kontrol væk)
	FPGA_system_control=EEPROM_read_byte(0, 0)&0x01;
	
	//delay, mens FPGA skal nå at loade PROM kode (SW version kan læses)
	while(FPGA_software_version != current_FPGA_sw_version)
		FPGA_software_version=FPGA_read_byte(32);

	/////////////////////// SETUP TESTPOINTS ///////////////////////
	FPGA_write_array(23, &tp_select, 3);

	/////////////////////// SETUP OCXO /////////////////////////////
	//læs gemt OCXO styrespænding
	OCXO_dac_value.OCXO_dac_value_bytes[0] = EEPROM_read_byte(0, 1);
	OCXO_dac_value.OCXO_dac_value_bytes[1] = EEPROM_read_byte(0, 2);

	//hvis første gangs boot, sæt default værdi
	if (OCXO_dac_value.OCXO_dac_value_int == 0)
		OCXO_dac_value.OCXO_dac_value_int = 23000;

	//OCXO = reset + hurtigt loop filter
	set_bit(&FPGA_system_control, 2);
	set_bit(&FPGA_system_control, 3);
	FPGA_write_byte(20, FPGA_system_control);
	
	//send startop styrespænding til FPGA
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

	//færdig med initialisering
	FPGA_reset=0;
	
	counter=0;
	LED1= 1;

	//main løkke
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

		//processér indkommende uBlox data
		packet_process();

		//Check for bekræftelse af skift til sommer/vintertid
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
			//læs OCXO faseforskel
			OCXO_phase_diff = (FPGA_read_byte(36)*256) + FPGA_read_byte(35);
	
			//29/4/08: Rev 2: Ny OCXO styring for tolerancer

			//hvis ikke låst, men tidspuls tilgængelig
			if(!OCXO_locked_status && timepulse_status)
			{
				//hvis indenfor tolerancerne
				if((OCXO_phase_diff<OCXO_tolerance) && (OCXO_phase_diff>(-OCXO_tolerance)))
				{
					//hvis låst i lang tid nok
					if(OCXO_counter>OCXO_time)
					{
						OCXO_counter=0;
						OCXO_locked_status=1;				//angiv som låst!
						clear_bit(&FPGA_system_control, 3);	//sæt til langsomt loop-filter
					}
					//ellers tæl på låsetiden
					else			
						OCXO_counter++;
				}
				else
					OCXO_counter=0;
			}
			//ellers hvis låst
			else if(OCXO_locked_status)
			{
				//hvis uden for tolerance
				if((OCXO_phase_diff>OCXO_tolerance) || (OCXO_phase_diff<(-OCXO_tolerance)))
				{
					//hvis ude af lås i for lang tid
					if(OCXO_counter>OCXO_time)
					{
						OCXO_counter=0;
						OCXO_locked_status=0;				//angiv som ikke låst!
						set_bit(&FPGA_system_control, 3);	//sæt til hurtigt loop-filter
					}
					//ellers tæl på låsetiden
					else			
						OCXO_counter++;
				}
				else
					OCXO_counter=0;
			}
			//hvis GPS mistes, skal der startes forfra
			else if(!timepulse_status)
				OCXO_counter=0;

			update_LTC_strings();
			check_daylight_switch();

			//29/4/08: Rev 2: Ny styring for OCXO for GPS tilgængelighed

			//hvis ingen tidspuls, men OCXO aktiv (ikke fixed)
			if(!timepulse_status && !(FPGA_system_control&16))
			{
				//send gennemsnits styrespænding til DAC
				FPGA_write_byte(21, OCXO_dac_value.OCXO_dac_value_bytes[1]);
				FPGA_write_byte(22, OCXO_dac_value.OCXO_dac_value_bytes[0]);

				//sæt OCXO til fixed mode
				set_bit(&FPGA_system_control, 4);
				OCXO_locked_status=0;
			}
			//ellers hvis tidspuls, men OCXO ikke aktiv
			else if(timepulse_status && (FPGA_system_control&16))
			{
				//hvis GPS signal tilgængeligt, slå fixed mode fra
				clear_bit(&FPGA_system_control, 4);
			}

			//hvis PLL'en er låst, log styrespænding i EEPROM + udregn gennemsnits styrespænding
			if(OCXO_locked_status && timepulse_status)
			{
				OCXO_dac_value.OCXO_dac_value_bytes[1]=FPGA_read_byte(33);
				OCXO_dac_value.OCXO_dac_value_bytes[0]=FPGA_read_byte(34);

				//beregn gennemsnits DAC værdi over de sidste 16 værdier
				OCXO_avg_dac_value.OCXO_avg_dac_value_int-=OCXO_avg_dac_value.OCXO_avg_dac_value_int/16;
				OCXO_avg_dac_value.OCXO_avg_dac_value_int+=OCXO_dac_value.OCXO_dac_value_int/16;

				//gem OCXO styrespænding over to(!!) sekunder, for ikke at forsinke MCU
				if(counter==1)
					EEPROM_write_byte(0, 2, OCXO_dac_value.OCXO_dac_value_bytes[1]);

				if(counter==2)
					EEPROM_write_byte(0, 1, OCXO_dac_value.OCXO_dac_value_bytes[0]);
			}

			//28/4/08:  counter kunne tælle, selvom UTC time valid !=0x07
			//			resulterer i, at der kan gå mere end 15 sek for aktivering af
			//			OCXO, efter tid var valid 
			//			tidskrav er sat ned til 0x03 = valid TOW og WEEK, men ikke UTC
			//			= hurtigere startop!
			
			//OCXO PLL har nu modtaget start op værdi, så aktiver (der ventes 15 sek)
			if ((UTC_time_valid>0x01) && timepulse_status)
			{
				counter++;

				if(counter==15 && (FPGA_system_control&4))
				{
					clear_bit(&FPGA_system_control, 2);
					LED2=1;
				}
			}
			else
				counter=0;
			
			ext1_rise_flag=0;
		}


		
		//hvis faldende puls (500 ms efter stignede puls) (!! Ikke flere kommandoer her, tidsmangel ??)
		if(ext1_fall_flag==1)
		{
			LED1=0;

			//håndter tid
			LTC_manager();
			transmit_LTC_data();
			ext1_fall_flag=0;
		}

		//hvis ingen aktivitet, sluk LED1
		LED1=1;
	}
}


//Sætter en bit høj i *byte
void set_bit(unsigned char *byte, unsigned char bit_number)
{
	*byte=*byte|(1<<bit_number);
}

//Sætter en bit lav i *byte
void clear_bit(unsigned char *byte, unsigned char bit_number)
{
	*byte=*byte&~(1<<bit_number);
}


//29/4/08: Rev 2: Udskriver diverse statusinfo via SW_uart
void print_debug_page()
{
	//new page
	SW_sendstr("\f\n\n\rPAGE\n\r", 10);
	
	//print GPS status
	if(timepulse_status)
		SW_sendstr("GPS OK", 6);
	else 
		SW_sendstr("GPS NO", 6);


	//print OCXO status
	if(OCXO_locked_status)
		SW_sendstr(" OCXO LOCK", 10);
	else 
		SW_sendstr(" OCXO UNLK", 10);

	//PLL filter status
	if(FPGA_system_control&8)
		SW_sendstr("-FST", 4);
	else 
		SW_sendstr("-SLW", 4);

	//PLL status
	if(FPGA_system_control&16)
		SW_sendstr("-FIX", 4);
	else 
		SW_sendstr("-RUN", 4);

	//OCXO info
	sprintf(debug_buffer, "\n\rXO PD %d Avg: %d LC %d\n\r", (int)OCXO_phase_diff, (int)OCXO_avg_dac_value.OCXO_avg_dac_value_int, (int)OCXO_counter);
	SW_sendstr(debug_buffer, strlen(debug_buffer));

}