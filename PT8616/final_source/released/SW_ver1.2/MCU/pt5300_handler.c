#include <c8051f320.h>
#include <string.h>
#include "globals.h"
#include "i2c_slave.h"
#include "i2c_bus.h"
#include "LTC_handler.h"
#include "EEPROM_drv.h"

sbit LED2 = P2^1;

//h�ndtere indkomne data fra PT5300 mainframe
void pt5300_in_handle()
{
	//t�ller til diverse form�l
	unsigned char counter=0;
	
	switch(in_buffer[0])
	{
		////////// SYSTEM IND DATA ////////////////
		//register 0: Genlock format
		case 0:	
			//4 = SDI625@50, 5=SDI525@30/1,001
			if(in_buffer[1]==5)
				set_bit(&FPGA_system_control, 0);
			else 
				clear_bit(&FPGA_system_control, 0);
			
			//Afsend FPGA system setup
			FPGA_write_byte(20, FPGA_system_control);
			EEPROM_write_byte(0, 0, FPGA_system_control);

			break;
	
		////////// LTC GENERATOR A IND DATA ////////////////
		//register 7-10: LTC a offset
		case 7:
			LTC_a_offset.LTC_offset_bytes[3]=in_buffer[1];
			break;
		case 8:
			LTC_a_offset.LTC_offset_bytes[2]=in_buffer[1];
			break;
		case 9:
			LTC_a_offset.LTC_offset_bytes[1]=in_buffer[1];
			break;
		case 10:
			LTC_a_offset.LTC_offset_bytes[0]=in_buffer[1];
			break;

		//register 11: LTC a format
		case 11:
			LTC_setup&=0xF0; 	//Reset LTC a format bits
			//s�t korrekt format
			switch(in_buffer[1])
			{
				case 0x00:				
					LTC_setup|=0x00;	//hvis 24 FPS
					break;
				case 0x01:				
					LTC_setup|=0x01;	//hvis 25 FPS
					break;
				case 0x02:
					LTC_setup|=0x06;	//hvis 29.97 Drop frame
					break;
				case 0x03:
					LTC_setup|=0x02;	//hvis 29.97 Nondrop frame
					break;
				case 0x04:
					LTC_setup|=0x03;	//hvis 30 FPS
					break;
			}
			break;

		//register 12-14: Dropframe setup + timer/min
		case 12:
			dropframe_setup&=0xF0;					//Reset dropframe mode
			dropframe_setup|=in_buffer[1]; 			//Set bits for LTC a
			break;

		case 13:
			LTC_a_sync_hour=in_buffer[1];
			break;
		
		case 14:
			LTC_a_sync_min=in_buffer[1];
			break;

		//register 15: Tidszone, timer
		case 15:	
			LTC_a_watchoffset[0]=in_buffer[1];
			break;

		//register 16: Tidszone, minutter
		case 16:
			LTC_a_watchoffset[1]=in_buffer[1];
			break;

		//register 17: Sommer/vinter tid skift mode
		case 17:
			daylight_switch_setup&=0xF0; //reset switch mode
			daylight_switch_setup|=in_buffer[1];
			break;

		//register 18-20: sommertid (m�ned, dag, �r)
		case 18:
			daylight_on_a_time.month=in_buffer[1]+1;
			break;
		case 19:
			daylight_on_a_time.day=in_buffer[1];
			break;
		case 20:
			daylight_on_a_time.hour=in_buffer[1];
			break;

		//register 21-23: vintertid (m�ned, dag, �r)
		case 21:
			daylight_off_a_time.month=in_buffer[1]+1;
			break;
		case 22:
			daylight_off_a_time.day=in_buffer[1];
			break;
		case 23:
			daylight_off_a_time.hour=in_buffer[1];
			break;


		////////// LTC GENERATOR B IND DATA ////////////////
		//register 27-30: LTC b offset
		case 27:
			LTC_b_offset.LTC_offset_bytes[3]=in_buffer[1];
			break;
		case 28:
			LTC_b_offset.LTC_offset_bytes[2]=in_buffer[1];
			break;
		case 29:
			LTC_b_offset.LTC_offset_bytes[1]=in_buffer[1];
			break;
		case 30:
			LTC_b_offset.LTC_offset_bytes[0]=in_buffer[1];
			break;

		//register 31: LTC b format
		case 31:
			LTC_setup&=0x0F; 	//Reset LTC b format bits
			//s�t korrekt format
			switch(in_buffer[1])
			{
				case 0x00:
					LTC_setup|=0x00;	//hvis 24 FPS
					break;
				case 0x01:
					LTC_setup|=0x10;	//hvis 25 FPS
					break;
				case 0x02:
					LTC_setup|=0x60;	//hvis 29.97 Drop frame
					break;
				case 0x03:
					LTC_setup|=0x20;	//hvis 29.97 Nondrop frame
					break;
				case 0x04:
					LTC_setup|=0x30;	//hvis 30 FPS
					break;
			}
			break;

		//register 33-34: Dropframe setup + timer/min
		case 32:
			dropframe_setup&=0x0F;					//Reset dropframe mode
			dropframe_setup|=in_buffer[1]<<4; 		//Set bits for LTC a
			break;

		case 33:
			LTC_b_sync_hour=in_buffer[1];			
			break;
		
		case 34:
			LTC_b_sync_min=in_buffer[1];
			break;

		//register 35: Tidszone, timer
		case 35:	
			LTC_b_watchoffset[0]=in_buffer[1];
			break;

		//register 36: Tidszone, minutter
		case 36:	
			LTC_b_watchoffset[1]=in_buffer[1];
			break;
		//register 37: Sommer/vinter tid skift mode
		case 37:
			daylight_switch_setup&=0x0F; //reset switch mode
			daylight_switch_setup|=in_buffer[1]<<4;
			break;

		//register 38-40: sommertid (m�ned, dag, �r)
		case 38:
			daylight_on_b_time.month=in_buffer[1]+1;
			break;
		case 39:
			daylight_on_b_time.day=in_buffer[1];
			break;
		case 40:
			daylight_on_b_time.hour=in_buffer[1];
			break;

		//register 41-43: vintertid (m�ned, dag, �r)
		case 41:
			daylight_off_b_time.month=in_buffer[1]+1;
			break;
		case 42:
			daylight_off_b_time.day=in_buffer[1];
			break;
		case 43:
			daylight_off_b_time.hour=in_buffer[1];
			break;
	
		//register 50+51 = confirm/annuler skift til sommer/vintertid for hhv. LTC A/B
		case 50:
			if(in_buffer[1])
				set_bit(&LTC_confirm, 0);
			else
				set_bit(&LTC_confirm, 1);
			break;

		case 51:
			if(in_buffer[1])
				set_bit(&LTC_confirm, 4);
			else
				set_bit(&LTC_confirm, 5);
			break;

		//register 52+53 = s�t sync tidspunkt for LTC
		case 52:
			secs_since_sync_a=int_GPS_tow%600; //set synctime til n�rmeste 10'er minut
			break;
		case 53:
			secs_since_sync_b=int_GPS_tow%600; //set synctime til n�rmeste 10'er minut
			break;

		//register 54+55 = requested sync for LTC a og b
		//LTC a
		case 54:
			if(in_buffer[1]==1)
				secs_since_sync_a=int_GPS_tow%600; //set synctime til n�rmeste 10'er minut

			//reset request
			clear_bit(&LTC_sync_request, 0);
			clear_bit(&LTC_sync_request, 1);
			break;

		//LTC b
		case 55:
			if(in_buffer[1]==1)
				secs_since_sync_b=int_GPS_tow%600; //set synctime til n�rmeste 10'er minut

				//reset request
			clear_bit(&LTC_sync_request, 4);
			clear_bit(&LTC_sync_request, 5);
			break;

		//daylight setup LTC A+B bit:0 DL on, bit 1: DL off, bit 4 DL on, bit 5 DL off
		case 56:
			if (in_buffer[1]&1)
				LTC_a_daylight_flag=1;
			else if (in_buffer[1]&2)
				LTC_a_daylight_flag=0;

			if (in_buffer[1]&16)
				LTC_b_daylight_flag=1;
			else if (in_buffer[1]&32)
				LTC_b_daylight_flag=0;
			break;

		//hvis der skrives en KU streng
		case 62:
			memcpy(KU_str, &in_buffer[1], sizeof(KU_str));

			for(counter=0;counter<11;counter++)
				EEPROM_write_byte(0, 200+counter, KU_str[counter]);
			break;
			
	}
}

//h�ndtere udg�ende data fra PT5300 mainframe
void pt5300_out_handle()
{
	switch(in_buffer[0])
	{
		////////////////////////////////////////////////////////////////////////////
		// F�rste l�ste I2C char over 100 = subadresse til udstreng, herefter l�ses
		////////////////////////////////////////////////////////////////////////////

		// 100 = LTC A ud
		case 100:
			out_buffer_select=0;
			break;
		// 101 = LTC B ud
		case 101:
			out_buffer_select=1;
			break;
		// 105 = master status byte ud
		case 105:
			out_buffer_select=5;
			break;
		// 106 = LTC request byte ud
		case 106:
			out_buffer_select=6;
			break;
		// 107 = LTC sync request byte ud
		case 107:
			out_buffer_select=7;
			break;
		// 108 = sommer/vintertid status
		case 108: 
			out_buffer_select=8;
			break;
		// 201 = SW version ud
		case 201:
			out_buffer_select=101;
			break;
		// 202 = KU nummer ud
		case 202:
			out_buffer_select=102;
	}
}