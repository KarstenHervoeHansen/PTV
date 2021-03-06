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


/*	Pin io chart
	P0.0 = SDA
	P0.1 = SCL
	P0.4 = TX
	P0.5 = RX
	P0.7 = EXT_INT
	P1.0 = TP_STATUS
	P1.1 = SDA_int
	P1.2 = SCL_int 
	P1.3 = GPIO_0
	P1.4 = GPIO_1
	P1.5 = GPIO_2
	P1.6 = GPIO_3
	P2.0 = MCU_LED1
	P2.1 = MCU_LED2
	P2.2 = FPGA_RESET
	P2.5 = NTP_LOCK_STATUS
	P2.6 = NTP_REQUEST
	P2.7 = MCU_TEST_POINT
*/

//den nuv?rende softwareversion i FPGA (!! SKAL stemme overens med MCU)
unsigned char code current_FPGA_sw_version=0x14;

//den nuv?rende MCU software version (bliver vist i PT5300) 10'ere = version, 1'ere = revision
unsigned char code current_MCU_sw_version=29;

//l?st software version i FPGA
unsigned char FPGA_software_version=0;
bit ext1_rise_flag=0, ext1_fall_flag=0;

typedef enum {start_up, setup_gps, ublox_error, fixed_osc, unlocked_valid_TP, locked, locked_invalid_TP, locked_out_of_phase, unlocked_invalid_TP} states;
states main_state;
states next_state;

//FPGA status inputs
sbit timepulse_status = P1^0;

//LED out
sbit LED1 = P2^0;
sbit LED2 = P2^1;

// NTP conns
sbit NTP_LOCK_STATUS = P2^5;
sbit NTP_REQUEST = P2^6;

// test point
sbit MCU_tp = P2^7;

//reset out til FPGA
sbit FPGA_reset = P2^2;

//t?ller til diverse form?l
unsigned int counter=0;
unsigned char message_counter=0;
unsigned char NTP_request_edge;

// t?llere til blinkere
unsigned int blink_count=0;
unsigned char blinkseq;
unsigned char blinkpat;


/********* Master status byte *************
Bit 0: Timepulse status
Bit 1: GPS PLL l?st status
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
Bit 1: 1 = 4 sekunder PAL sync, 0 = ingen sync (h?j for hver 4. sekund, tilpasset GPS tid)
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

// L?st v?rdi for OCXO styresp?ning, gemmes til n?ste reboot
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

//tolerance for tidsrum, der skal v?re l?st i/ude af l?s, f?r skift af loophastighed
#define OCXO_time			30

unsigned char xdata status_str[11];
unsigned char xdata PT5300_str[11];
unsigned char xdata phase_str[11];
unsigned char xdata counter_str[11];
unsigned char xdata quality_str[11];  // "qq.qq.qq.qq"
unsigned char xdata debug_str[15]; //    "ss,p,l     " // 11;

//signal qualities
unsigned char signal_quality[16];

//sorted signal qualities
unsigned char sorted_quality[4];

unsigned char GPS_Recovery_Count;// = 0;

//rising edge p? syntesiseret 1 PPS
void int_rise() interrupt 0
{
	ext1_rise_flag=1;
}

//falling edge p? syntesiseret 1 PPS (500 ms efter GPS PPS)
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
			// Check efter h?jere v?rdi
			if(signal_quality[idx1]>sorted_quality[idx2])
				// Men ikke h?jere end forrige maks
				if(signal_quality[idx1]<max_quality)
					sorted_quality[idx2]=signal_quality[idx1];

		// S?t n?ste niveau maks
		max_quality=sorted_quality[idx2];
	}
}



/************************************************************************************************
*																		                		*
*						GPS Genlock main rutine							                		*
*																		                		*
*	Hovedprogram for GPS					Udgivet 05/02/08		                			*     
*	Udvikler: Jens K Hansen											                  			*
*																	                			*
*	Revision 0: Oprettet												              			*
*	Revision 1: 05/02/08: Udgivet som version 1.0						              			*
*	Revision 2: 28/04/08:Udgivet som version 1.1.						              			*
*				1.Rettet opstartssekvens til hurtigere bootup  			              			*
*				2.Tilf?jet SW UART p? P2.6 = jumperpin til debug		              			*
*				S?t DEBUG = 1 i globals.h, SW_UART.c tilf?jet til		              			*
*				projektet												              			*
*				3.Resetter alle variable								              			*
*				4. ?ndret styringslogik til bedre h?ndtering af			              			*
*					udfald og OCXO og hurtigere opstart					             			*
*	Revision 3: 17/11/08: Udgivet som version 1.2						             			*
*				1. Rettet synkronisme tilpasset Frankfurt tid			             			*
*	Revision 4: 18/02/10: Udgivet som version 2.0						             			*
*				1. HW setup rettet										             			*
*				2. Main state machine rettet							              			*
*				3. Signal quality sorteres								             			*
*	Revision 5: 12/03/10: Udgivet som version 2.1						              			*
*				1. Rettet udl?sning af FPGA version, f?r start			              			*
*	Revision 6:	xx/xx/10: Udgivet som version 2.1						              			*
*				1. Viser besked hvert 4 sekund for sig quality			              			*
*				2. Rettet EEPROM delay, KU virker ikke p? 2.1			              			*
*	Revision 7: 15/12/10: Udgivet som version 2.2						              			*
*				1. Tilf?jet NTP support									              			*
*	Revision 8: 26/04/11: Udgivet som version 2.3						              			*
*				1. Rettet delays i EEPROM read/write (igen!)			              			*	
*					Gav genlock fejl med 67 mins interval				              			*
*	Revision 9: 06/06/11: Rettet PLL logik i FPGA						                    	*
*	Revision 10: 08/12/11: Rettet PLL stabilitet i FPGA					                    	*
*	Revision 11: 20/01/12: LTC viser nu GPS tid uden GPS signal. v.2.6	                    	*
*	Revision 12: 21/06/12: Rettet LTC low freq jitter v.2.7				                   		*
*	Revision 13: 03/09/12: BS,JKH: v.28. Rettet LTC_manager, BB timing ved GPS uge skift. v.2.8 *
*	Revision 14: 15/10/12: BS: v.29. Corrected uBlox protocol handler, GPS config is stored in  *
*                          BB-Ram and uBlox protocol handler can recover when GPS chip sends	*
*						   NMEA data.															*
*				                															    *
*************************************************************************************************/

void main()
{
	Init_Device();

	GPS_Recovery_Count = 0;
	ublox_setup_done = 0;

	// init statemachine
	next_state=start_up;
	main_state=!start_up;

	//delay, mens FPGA skal n? at loade PROM kode (SW version kan l?ses)
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

		//process?r indkommende uBlox data
		packet_process();

		//Check for bekr?ftelse af skift til sommer/vintertid
		confirm_daylight_switch();

		// Output GPS lock status
		NTP_LOCK_STATUS = timepulse_status;

		// Edge detector for NTP request
		if(NTP_REQUEST)
			NTP_request_edge=(NTP_request_edge|0x01);
		
		NTP_request_edge=NTP_request_edge<<1;

		// Hvis nedadg?ende flanke
		if (NTP_request_edge==0xf0)
			poll_packet(0x0b, 0x02); // GPS Health, UTC and ionosphere parameters 

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
			//poll_packet(0x0a, 0x09);

			////////////////////// OCXO styring ///////////////////////////
			//l?s OCXO faseforskel
			OCXO_phase_diff = (FPGA_read_byte(36)*256) + FPGA_read_byte(35);

			// Skriv til OCXO styring
			FPGA_write_byte(20, FPGA_system_control);

			if(main_state==locked)	//beregn kun gennemsnits DAC v?rdi ved l?st
			{
				OCXO_dac_value.OCXO_dac_value_bytes[1]=FPGA_read_byte(33);
				OCXO_dac_value.OCXO_dac_value_bytes[0]=FPGA_read_byte(34);

				//beregn gennemsnits DAC v?rdi over de sidste 16 v?rdier
				OCXO_avg_dac_value.OCXO_avg_dac_value_int-=OCXO_avg_dac_value.OCXO_avg_dac_value_int/16;
				OCXO_avg_dac_value.OCXO_avg_dac_value_int+=OCXO_dac_value.OCXO_dac_value_int/16;
			}

			update_LTC_strings();
			check_daylight_switch();

			counter++;
			message_counter++;

			
			//////////////////////////////////////////////////////////////////////////
			//   PT 5300 Status beskeder											//
			//////////////////////////////////////////////////////////////////////////
			
			memcpy(PT5300_str, status_str, 11);

/*			sprintf(phase_str, "%3i,%6li", (unsigned int)int_GPS_week, (unsigned long)int_GPS_tow);
			sprintf(counter_str, "%010i", (unsigned int)counter);
			sprintf(quality_str, "%02i,%02i,%02i", (unsigned int)sorted_quality[0],(unsigned int)sorted_quality[1],(unsigned int)sorted_quality[2]);
			sprintf(debug_str, "%02i,%1i,%1i",	(unsigned int)main_state, (unsigned int)timepulse_status, (unsigned int)OCXO_locked_status); 
*/			
			sprintf(phase_str, "%8li ", (unsigned long)int_GPS_tow);
			sprintf(counter_str, "%8li ", (unsigned long)GPS_tow.GPS_tow_long);
			sprintf(quality_str, "%03i,%03i ", (unsigned int)int_GPS_week, (unsigned int)GPS_week);
			sprintf(debug_str, "%02i,%1i,%1i,%1i",	(unsigned int)main_state, (unsigned int)timepulse_status, (unsigned int)OCXO_locked_status, (unsigned int)GPS_Recovery_Count); 
			//sprintf(debug_str, "%02i",	(unsigned int)main_state); 

			LED2=0;
			ext1_rise_flag=0;
		}

		//hvis faldende puls (500 ms efter stignede puls) 
		if(ext1_fall_flag==1)
		{
			//h?ndter tid
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
					sprintf(&status_str, "Boot...   ");

					OCXO_dac_value.OCXO_dac_value_int=0;
					OCXO_avg_dac_value.OCXO_avg_dac_value_int=0;

					//reset FPGA
					FPGA_reset=1;
				
					//Indl?s KU nummer
					for(counter=0;counter<11;counter++)
						KU_str[counter]=EEPROM_read_byte(0, 200+counter);
		
					init_LTC();

					//indl?s genlock system setup  fra EEPROM, og skriv denne til FPGA (masker OCXO kontrol v?k)
					FPGA_system_control=EEPROM_read_byte(0, 0)&0x01;
					
					//send startop styresp?nding til FPGA
					FPGA_write_byte(21, OCXO_dac_value.OCXO_dac_value_bytes[1]);
					FPGA_write_byte(22, OCXO_dac_value.OCXO_dac_value_bytes[0]);

					/////////////////////// SETUP OCXO /////////////////////////////
					// 29/04/11 Default v?rdier l?ses aldrig mere fra EEPROM, da 
					// skrivning under run-time gave problemer med jitter

					//l?s gemt OCXO styresp?nding
					//OCXO_dac_value.OCXO_dac_value_bytes[0] = EEPROM_read_byte(0, 1);
					//OCXO_dac_value.OCXO_dac_value_bytes[1] = EEPROM_read_byte(0, 2);

					//hvis f?rste gangs boot, s?t default v?rdi
					//if (OCXO_dac_value.OCXO_dac_value_int == 0xffff)
					OCXO_dac_value.OCXO_dac_value_int = 18260;

					//set gennemsnits styresp?nding til at starte p? startop styresp?nding
					OCXO_avg_dac_value.OCXO_avg_dac_value_int=OCXO_dac_value.OCXO_dac_value_int;

					//OCXO = reset + hurtigt loop filter
					set_bit(&FPGA_system_control, 2);
					set_bit(&FPGA_system_control, 3);

				break;

				case setup_gps:
					blinkpat = 0x00;
					
					sprintf(&status_str, "Setup...  ");

					FPGA_reset=0;	//f?rdig med initialisering

				break;

				case ublox_error:
					blinkpat=0xFF;
					sprintf(&status_str, "GPS fail  ");
				break;

				case fixed_osc:
					blinkpat = 0x33;
					
					sprintf(&status_str, "Unlocked  ");

					OCXO_locked_status=0;				//angiv som ikke l?st
	
					//send gennemsnits styresp?nding til DAC
					FPGA_write_byte(21, OCXO_avg_dac_value.OCXO_avg_dac_bytes[1]);
					FPGA_write_byte(22, OCXO_avg_dac_value.OCXO_avg_dac_bytes[0]);
					set_bit(&FPGA_system_control, 4);	//s?t OCXO til fixed mode

				break;

				case unlocked_valid_TP:
					blinkpat = 0x01;

					sprintf(&status_str, "Locking...");
					
					OCXO_locked_status=0;				//angiv som ikke l?st
					clear_bit(&FPGA_system_control, 2); //slip reset (dette er kun for hardsync ved f?rste reset)
					set_bit(&FPGA_system_control, 3);	//s?t til hurtigt loop-filter
					clear_bit(&FPGA_system_control, 4); //s?t OCXO til tracking mode

				break;

				case locked:
					blinkpat=0x00;

					sprintf(&status_str, "Locked    ");

					OCXO_locked_status=1;				//angiv som l?st!
					clear_bit(&FPGA_system_control, 3);	//s?t til langsomt loop-filter + sl? fixed mode fra
					clear_bit(&FPGA_system_control, 4); //hvis man kommer fra locked_invalid_tp, hvor den er fixed

				break;

				case locked_invalid_TP:
					blinkpat=0x05;

					sprintf(&status_str, "GPS lost  ");
					
					//send gennemsnits styresp?nding til DAC
					FPGA_write_byte(21, OCXO_avg_dac_value.OCXO_avg_dac_bytes[1]);
					FPGA_write_byte(22, OCXO_avg_dac_value.OCXO_avg_dac_bytes[0]);
					
					set_bit(&FPGA_system_control, 4);	//s?t OCXO til fixed mode
				break;

				case locked_out_of_phase:
					blinkpat=0x15;

					sprintf(&status_str, "Locked    ");

				break;

				case unlocked_invalid_TP:
					blinkpat=0xff;

					sprintf(&status_str, "Fixed     ");

					OCXO_locked_status=0;				//angiv som ikke l?st
					
					set_bit(&FPGA_system_control, 4);	//s?t OCXO til fixed mode

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
				if(counter>5)	// Vent 5 sekunder, s? FPGA prom er l?st
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
					if(counter>3)	//hvis tidspuls har v?ret valid i 3 sekunder
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
						//hvis l?st i lang tid nok
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
				// Hvis tidspuls tilg?ngelig igen
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


//S?tter en bit h?j i *byte
void set_bit(unsigned char *byte, unsigned char bit_number)
{
	*byte=*byte|(1<<bit_number);
}

//S?tter en bit lav i *byte
void clear_bit(unsigned char *byte, unsigned char bit_number)
{
	*byte=*byte&~(1<<bit_number);
}

void restart_mainstate(void)
{
	next_state=start_up;
	main_state=!start_up;
}