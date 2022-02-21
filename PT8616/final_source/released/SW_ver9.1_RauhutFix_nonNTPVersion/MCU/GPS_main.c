#include <C8051F320.h>
#include <string.h>
#include <stdio.h>
#include "config.h"
#include "globals.h"
#include "i2c_bus.h"
#include "i2c_slave.h"
#include "serial.h"
#include "ublox_prot.h"
#include "LTC_handler.h"
#include "pt5300_handler.h"
#include "EEPROM_drv.h"

//den nuv�rende softwareversion i FPGA (!! SKAL stemme overens med MCU)
unsigned char code current_FPGA_sw_version=0x10;
//den nuv�rende MCU software version (bliver vist i PT5300) 10'ere = version, 1'ere = revision
unsigned char code current_MCU_sw_version=91; // Special version for Carsten Rauhut, fixes jitter every 67 min
//l�st software version i FPGA
unsigned char FPGA_software_version=0;
bit ext1_rise_flag=0, ext1_fall_flag=0;

//typedef enum {start_up, setup_gps, ublox_error, fixed_osc, unlocked_valid_TP, locked, locked_invalid_TP, locked_out_of_phase, unlocked_invalid_TP, save_OCXO_val} states;
typedef enum {start_up, setup_gps, ublox_error, fixed_osc, unlocked_valid_TP, locked, locked_invalid_TP, locked_out_of_phase, unlocked_invalid_TP} states;
states main_state;
states next_state;

//FPGA status inputs
sbit timepulse_status = P1^0;

//LED out
sbit LED1 = P2^0;
sbit LED2 = P2^1;

unsigned int blink_count=0;
unsigned char blinkseq;
unsigned char blinkpat;

sbit MCU_tp = P2^7;

//reset out til FPGA
sbit FPGA_reset = P2^2;

//t�ller til diverse form�l
unsigned int counter=0;
unsigned char message_counter=0;

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
char tp_select[3] = {int_pps, pps_i, int_pps};

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
unsigned char master_status_byte=0;


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
unsigned char FPGA_system_control=0;

// KU streng, indeholder KU nummer for GPS board. Er gemt i EEPROM bank 0, byte 200-210
unsigned char xdata KU_str[11]={0,0,0,0,0,0,0,0,0,0,0};


int xdata OCXO_phase_diff=0;

// L�st v�rdi for OCXO styresp�ning, gemmes til n�ste reboot
union {
	unsigned int OCXO_dac_value_int;
	unsigned char OCXO_dac_value_bytes[2];
} OCXO_dac_value;

union {
	unsigned int OCXO_avg_dac_value_int;
	unsigned char OCXO_avg_dac_bytes[2];
} OCXO_avg_dac_value;

bit OCXO_locked_status=0;

//tolerance for OCXO faseforskel, for switch mellem hurtigt/langsomt loop
#define OCXO_tolerance		40

//tolerance for tidsrum, der skal v�re l�st i/ude af l�s, f�r skift af loophastighed
#define OCXO_time			30

unsigned char xdata status_str[11];
unsigned char xdata PT5300_str[11];

//signal qualities
unsigned char signal_quality[16];

//sorted signal qualities
unsigned char sorted_quality[4];

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


void sort_qualities()
{
	unsigned char idx1, idx2;
	unsigned char max_quality=255;

	// Sorted
	memset(&sorted_quality, 0, sizeof(sorted_quality));

	// For alle 4 sorterede
	for(idx2=0; idx2<4; idx2++)								
	{
		for(idx1=0; idx1<16; idx1++)
			// Check efter h�jere v�rdi
			if(signal_quality[idx1]>sorted_quality[idx2])
				// Men ikke h�jere end forrige maks
				if(signal_quality[idx1]<max_quality)
					sorted_quality[idx2]=signal_quality[idx1];

		// S�t n�ste niveau maks
		max_quality=sorted_quality[idx2];
	}
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
*		Revision 2: 28/4/2008:Udgivet som version 1.1.						*
*					1.Rettet opstartssekvens til hurtigere bootup  			*
*					2.Tilf�jet SW UART p� P2.6 = jumperpin til debug		*
*					S�t DEBUG = 1 i globals.h, SW_UART.c tilf�jet til		*
*					projektet												*
*					3.Resetter alle variable								*
*					4. �ndret styringslogik til bedre h�ndtering af			*
*						udfald og OCXO og hurtigere opstart					*
*		Revision 3: 17/11/08: Udgivet som version 1.2						*
*					1. Rettet synkronisme tilpasset Frankfurt tid			*
*		Revision 4: 18/02/10: Udgivet som version 2.0						*
*					1. HW setup rettet										*
*					2. Main state machine rettet							*
*					3. Signal quality sorteres								*
*		Revision 5: 12/03/10: Udgivet som version 2.1						*
*					1. Rettet udl�sning af FPGA version, f�r start			*
*		Revision 6:	xx/xx/10: Udgivet som version 2.1						*
*					Viser besked hvert 4 sekund for sig quality				*
*																			*
*																			*
****************************************************************************/

void main()
{
	Init_Device();

	// init statemachine
	next_state=start_up;
	main_state=!start_up;

	//delay, mens FPGA skal n� at loade PROM kode (SW version kan l�ses)
	while(FPGA_software_version != current_FPGA_sw_version)
		FPGA_software_version=FPGA_read_byte(32);

	// Main loop
	while(1)
	{
		//////////////////////////////////////////////////////
		//			kontinuerlige processer					//
		//////////////////////////////////////////////////////

		//pak master status streng
		set_bit(&master_status_byte, 0);

		if(OCXO_locked_status)
			set_bit(&master_status_byte, 1);
		else
			clear_bit(&master_status_byte, 1);

		//process�r indkommende uBlox data
		packet_process();

		//Check for bekr�ftelse af skift til sommer/vintertid
		confirm_daylight_switch();

		//hvis modtaget data fra PT5300
		if(i2c_msgupdate==1)
		{	
			pt5300_in_handle();
			i2c_msgupdate=0;
		}

		//hvis hel besked modtaget fra uBlox
		if(msg_update_flag==1)
		{	
			interpret_message();
			msg_update_flag=0;
		}	

		//blinker til visuel check af statemachine
		if(blink_count++==0x03ff)
		{
			blink_count=0;
			blinkseq++;
			blinkseq&=0x07;

			if((blinkpat<<blinkseq)&0x80)
				LED1=0;
			else
				LED1=1;
		}

		//////////////////////////////////////////////////////
		//	    		synkrone processer					//
		//////////////////////////////////////////////////////

		//hvis stigende puls (synkron med GPS PPS)
		if(ext1_rise_flag==1)
		{
			////////////////////// OCXO styring ///////////////////////////
			//l�s OCXO faseforskel
			OCXO_phase_diff = (FPGA_read_byte(36)*256) + FPGA_read_byte(35);

			// Skriv til OCXO styring
			FPGA_write_byte(20, FPGA_system_control);

			if(main_state==locked)	//beregn kun gennemsnits DAC v�rdi ved l�st
			{
				OCXO_dac_value.OCXO_dac_value_bytes[1]=FPGA_read_byte(33);
				OCXO_dac_value.OCXO_dac_value_bytes[0]=FPGA_read_byte(34);

				//beregn gennemsnits DAC v�rdi over de sidste 16 v�rdier
				OCXO_avg_dac_value.OCXO_avg_dac_value_int-=OCXO_avg_dac_value.OCXO_avg_dac_value_int/16;
				OCXO_avg_dac_value.OCXO_avg_dac_value_int+=OCXO_dac_value.OCXO_dac_value_int/16;
			}

			update_LTC_strings();
			check_daylight_switch();

			counter++;
			message_counter++;

			//if(message_counter==4)
				memcpy(PT5300_str, status_str, 11);

			/*else if(message_counter==8)
			{
				//sprintf(PT5300_str, "Sig %u    ", (unsigned int)sorted_quality[0]);

				if(sorted_quality[0]==0)
					sprintf(PT5300_str, "No sig.    ");
				else if(sorted_quality[0]>43)
					sprintf(PT5300_str, "Excel. sig.");
				else if(sorted_quality[0]>39)
					sprintf(PT5300_str, "Good sig.  ");
				else if(sorted_quality[0]>23)
					sprintf(PT5300_str, "Medium sig.");
				else
					sprintf(PT5300_str, "Poor sig.  ");

				message_counter=0;
			}*/
			//else if(message_counter==12)
			//{
			//	sprintf(PT5300_str, "Phs %04i ns", (int)OCXO_phase_diff);
			//	message_counter=0;
			//}

			LED2=0;
			ext1_rise_flag=0;
		}

		//hvis faldende puls (500 ms efter stignede puls) (!! Ikke flere kommandoer her, tidsmangel ??)
		if(ext1_fall_flag==1)
		{
			//h�ndter tid
			LTC_manager();
			transmit_LTC_data();
			sort_qualities();

			LED2=1;
			ext1_fall_flag=0;
		}

		//////////////////////////////////////////////////////
		//			main state machine						//
		//////////////////////////////////////////////////////
		
		//next states
		if(main_state!=next_state)
		{
			switch(next_state)
			{
				case start_up:
					blinkpat = 0xff;

					// Status string for PT5300 display
					sprintf(&status_str, "boot...   ");

					OCXO_dac_value.OCXO_dac_value_int=0;
					OCXO_avg_dac_value.OCXO_avg_dac_value_int=0;

					//reset FPGA
					FPGA_reset=1;
				
					//Indl�s KU nummer
					for(counter=0;counter<11;counter++)
						KU_str[counter]=EEPROM_read_byte(0, 200+counter);
		
					init_LTC();

					//indl�s genlock system setup  fra EEPROM, og skriv denne til FPGA (masker OCXO kontrol v�k)
					FPGA_system_control=EEPROM_read_byte(0, 0)&0x01;

						/////////////////////// SETUP TESTPOINTS ///////////////////////
					FPGA_write_array(23, &tp_select, 3);
					
					//send startop styresp�nding til FPGA
					FPGA_write_byte(21, OCXO_dac_value.OCXO_dac_value_bytes[1]);
					FPGA_write_byte(22, OCXO_dac_value.OCXO_dac_value_bytes[0]);

					/////////////////////// SETUP OCXO /////////////////////////////
					//l�s gemt OCXO styresp�nding
					OCXO_dac_value.OCXO_dac_value_bytes[0] = EEPROM_read_byte(0, 1);
					OCXO_dac_value.OCXO_dac_value_bytes[1] = EEPROM_read_byte(0, 2);

					//hvis f�rste gangs boot, s�t default v�rdi
					if (OCXO_dac_value.OCXO_dac_value_int == 0xffff)
						OCXO_dac_value.OCXO_dac_value_int = 18260;

					//set gennemsnits styresp�nding til at starte p� startop styresp�nding
					OCXO_avg_dac_value.OCXO_avg_dac_value_int=OCXO_dac_value.OCXO_dac_value_int;

					//OCXO = reset + hurtigt loop filter
					set_bit(&FPGA_system_control, 2);
					set_bit(&FPGA_system_control, 3);

				break;

				case setup_gps:
					blinkpat = 0x00;
					
					sprintf(&status_str, "setup...  ");

					FPGA_reset=0;	//f�rdig med initialisering

				break;

				case ublox_error:
					blinkpat=0xFF;
					sprintf(&status_str, "GPS fail  ");
				break;

				case fixed_osc:
					blinkpat = 0x33;
					
					sprintf(&status_str, "unlocked  ");

					OCXO_locked_status=0;				//angiv som ikke l�st
	
					//send gennemsnits styresp�nding til DAC
					FPGA_write_byte(21, OCXO_avg_dac_value.OCXO_avg_dac_bytes[1]);
					FPGA_write_byte(22, OCXO_avg_dac_value.OCXO_avg_dac_bytes[0]);
					set_bit(&FPGA_system_control, 4);	//s�t OCXO til fixed mode

				break;

				case unlocked_valid_TP:
					blinkpat = 0x01;

					sprintf(&status_str, "locking...");
					
					OCXO_locked_status=0;				//angiv som ikke l�st
					clear_bit(&FPGA_system_control, 2); //slip reset (dette er kun for hardsync ved f�rste reset)
					set_bit(&FPGA_system_control, 3);	//s�t til hurtigt loop-filter
					clear_bit(&FPGA_system_control, 4); //s�t OCXO til tracking mode

				break;

				case locked:
					blinkpat=0x00;

					sprintf(&status_str, "locked    ");

					OCXO_locked_status=1;				//angiv som l�st!
					clear_bit(&FPGA_system_control, 3);	//s�t til langsomt loop-filter + sl� fixed mode fra
					clear_bit(&FPGA_system_control, 4); //hvis man kommer fra locked_invalid_tp, hvor den er fixed

				break;

				case locked_invalid_TP:
					blinkpat=0x05;

					sprintf(&status_str, "GPS lost  ");
					
					//send gennemsnits styresp�nding til DAC
					FPGA_write_byte(21, OCXO_avg_dac_value.OCXO_avg_dac_bytes[1]);
					FPGA_write_byte(22, OCXO_avg_dac_value.OCXO_avg_dac_bytes[0]);
					
					set_bit(&FPGA_system_control, 4);	//s�t OCXO til fixed mode
				break;

				case locked_out_of_phase:
					blinkpat=0x15;

					sprintf(&status_str, "locked    ");

				break;

				case unlocked_invalid_TP:
					blinkpat=0xff;

					sprintf(&status_str, "fixed     ");

					OCXO_locked_status=0;				//angiv som ikke l�st
					
					set_bit(&FPGA_system_control, 4);	//s�t OCXO til fixed mode

				break;

				//case save_OCXO_val:
				//	blinkpat=0xff;					
						
				break;
			}

			counter=0;
			main_state=next_state;
		}

		switch(main_state)
		{
			case start_up:
				next_state=setup_gps;
			break;

			case setup_gps:
				if(counter>5)	// Vent 5 sekunder, s� FPGA prom er l�st
				{
					if(!setup_ublox())	//setup UBlox LEA 4T modul
						next_state=ublox_error;
					else
						next_state=fixed_osc;
				}
			break;

			case fixed_osc:
				if(timepulse_status==1)
				{
					if(counter>3)	//hvis tidspuls har v�ret valid i 3 sekunder
						next_state=unlocked_valid_TP;
				}
				else
					counter=0;

			break;

			case unlocked_valid_TP:
				//hvis tispuls mistes igen
				if(timepulse_status==0)
					next_state=fixed_osc;
				else
					//hvis indenfor tolerancerne
					if((OCXO_phase_diff<OCXO_tolerance) && (OCXO_phase_diff>(-OCXO_tolerance)))
					{
						//hvis l�st i lang tid nok
						if(counter>OCXO_time)
							next_state=locked;
					}
					else
						counter=0;

			break;

			case locked:
				// Hvis mistet tidspuls
				if(timepulse_status==0)
					next_state=locked_invalid_TP;

				// Hvis udenfor tolerance
				else if((OCXO_phase_diff>OCXO_tolerance) || (OCXO_phase_diff<(-OCXO_tolerance)))
					next_state=locked_out_of_phase;

				//if(counter==10)
				//	next_state=save_OCXO_val;

			break;

			case locked_invalid_TP:
				// Hvis tidspuls tilg�ngelig igen
				if(timepulse_status==1)
					next_state=locked;
				// Hvis uden tidspuls i for lang tid
				else if(counter>(OCXO_time*10))
					next_state=unlocked_invalid_TP;

			break;

			case locked_out_of_phase:
				//hvis indenfor tolerancerne
				if((OCXO_phase_diff<OCXO_tolerance) && (OCXO_phase_diff>(-OCXO_tolerance)))
					next_state=locked;

				//hvis mistet tidspuls
				else if(timepulse_status==0)
					next_state=locked_invalid_TP;

				else if(counter>(OCXO_time*10))
					next_state=unlocked_valid_TP;

			break;

			case unlocked_invalid_TP:
				if(timepulse_status==1)
					next_state=unlocked_valid_TP;

			break;

			case ublox_error:
				next_state = ublox_error;
			break;

			/*case save_OCXO_val:
				if (counter == 1)
					EEPROM_write_byte(0, 2, OCXO_dac_value.OCXO_dac_value_bytes[1]);
				else if (counter == 2)
					EEPROM_write_byte(0, 1, OCXO_dac_value.OCXO_dac_value_bytes[0]);
				else if (counter >= 3)
					next_state = locked;
			break;*/

			default:
				next_state = ublox_error;
			break;
		}
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