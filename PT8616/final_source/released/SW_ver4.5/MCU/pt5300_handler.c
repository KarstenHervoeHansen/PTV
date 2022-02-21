#include <c8051f320.h>
#include <string.h>
#include "globals.h"
#include "i2c_slave.h"
#include "i2c_bus.h"
#include "LTC_handler.h"
#include "EEPROM_drv.h"

void WriteEEpromData(void);

sbit LED2 = P2^1;

char EpromWritePending;

//håndtere indkomne data fra PT5300 mainframe
void pt5300_in_handle()
{

	//tæller til diverse formål
	unsigned char counter=0;
	
	if ((in_buffer[0] >= 7) && (in_buffer[0] <= 62)) EpromWritePending = 30;

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
			//sæt korrekt format
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

		//register 18-20: sommertid (måned, dag, år)
		case 18:
			daylight_on_a_time.month=in_buffer[1]+1;
			break;
		case 19:
			daylight_on_a_time.day=in_buffer[1];
			break;
		case 20:
			daylight_on_a_time.hour=in_buffer[1];
			break;

		//register 21-23: vintertid (måned, dag, år)
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
			//sæt korrekt format
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

		//register 38-40: sommertid (måned, dag, år)
		case 38:
			daylight_on_b_time.month=in_buffer[1]+1;
			break;
		case 39:
			daylight_on_b_time.day=in_buffer[1];
			break;
		case 40:
			daylight_on_b_time.hour=in_buffer[1];
			break;

		//register 41-43: vintertid (måned, dag, år)
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

		//register 52+53 = sæt sync tidspunkt for LTC
		case 52:
			secs_since_sync_a=int_GPS_tow%600; //set synctime til nærmeste 10'er minut
			break;
		case 53:
			secs_since_sync_b=int_GPS_tow%600; //set synctime til nærmeste 10'er minut
			break;

		//register 54+55 = requested sync for LTC a og b
		//LTC a
		case 54:
			if(in_buffer[1]==1)
				secs_since_sync_a=int_GPS_tow%600; //set synctime til nærmeste 10'er minut

			//reset request
			clear_bit(&LTC_sync_request, 0);
			clear_bit(&LTC_sync_request, 1);
			break;

		//LTC b
		case 55:
			if(in_buffer[1]==1)
				secs_since_sync_b=int_GPS_tow%600; //set synctime til nærmeste 10'er minut

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
//	WriteEEpromData(in_buffer[0]);
}

//håndtere udgående data fra PT5300 mainframe
void pt5300_out_handle()
{
	switch(in_buffer[0])
	{
		////////////////////////////////////////////////////////////////////////////
		// Første læste I2C char over 100 = subadresse til udstreng, herefter læses
		////////////////////////////////////////////////////////////////////////////

		// 100 = LTC A ud
		case 100:
			out_buffer_select=0;
			break;
		// 101 = LTC B ud
		case 101:
			out_buffer_select=1;
			break;
		case 102:
			out_buffer_select=2;
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
		// 109 = phase string 
		case 109:
			out_buffer_select=9;
			break;
		// 110 = counter string 
		case 110:
			out_buffer_select=10;
			break;
		// 111 = quality string 
		case 111:
			out_buffer_select=11;
			break;
		// 112 = debug string 
		case 112:
			out_buffer_select=12;
			break;


		// 201 = SW version ud
		case 201:
			out_buffer_select=101;
			break;
		// 202 = KU nummer ud
		case 202:
			out_buffer_select=102;
			break;

	
	}
}

void WriteEEpromData(void)
{
/*	switch(caseptr)
	{
		////////// LTC GENERATOR A IND DATA ////////////////
		//register 7-10: LTC a offset
		case 7:
			EEPROM_write_byte(0, 64, LTC_a_offset.LTC_offset_bytes[3]);		
			break;
		case 8:
			EEPROM_write_byte(0, 65, LTC_a_offset.LTC_offset_bytes[2]);
			break;
		case 9:
			EEPROM_write_byte(0, 66, LTC_a_offset.LTC_offset_bytes[1]);
			break;
		case 10:
			EEPROM_write_byte(0, 67, LTC_a_offset.LTC_offset_bytes[0]);
			break;

		//register 11: LTC a format
		case 11:
			EEPROM_write_byte(0, 68, LTC_setup);
			break;

		//register 12-14: Dropframe setup + timer/min
		case 12:
			EEPROM_write_byte(0, 69, dropframe_setup);
			break;

		case 13:
			EEPROM_write_byte(0, 70, LTC_a_sync_hour);
			break;
		
		case 14:
			EEPROM_write_byte(0, 71, LTC_a_sync_min);
			break;

		//register 15: Tidszone, timer
		case 15:	
			break;

		//register 16: Tidszone, minutter
		case 16:
			EEPROM_write_byte(0, 72, LTC_a_watchoffset[0]);
			EEPROM_write_byte(0, 73, LTC_a_watchoffset[1]);
			break;

		//register 17: Sommer/vinter tid skift mode
		case 17:
			EEPROM_write_byte(0, 74, daylight_switch_setup);
			break;

		//register 18-20: sommertid (måned, dag, år)
		case 18:
			break;
		case 19:
			break;
		case 20:
			EEPROM_write_byte(0, 75, daylight_on_a_time.month);
			EEPROM_write_byte(0, 76, daylight_on_a_time.day);
			EEPROM_write_byte(0, 77, daylight_on_a_time.hour);
			break;

		//register 21-23: vintertid (måned, dag, år)
		case 21:
			EEPROM_write_byte(0, 78, daylight_off_a_time.month);
			break;
		case 22:
			EEPROM_write_byte(0, 79, daylight_off_a_time.day);
			break;
		case 23:
			EEPROM_write_byte(0, 80, daylight_off_a_time.hour);
			break;

		////////// LTC GENERATOR B IND DATA ////////////////
		//register 27-30: LTC b offset
		case 27:
			EEPROM_write_byte(0, 84, LTC_b_offset.LTC_offset_bytes[3]);
			break;
		case 28:
			EEPROM_write_byte(0, 85, LTC_b_offset.LTC_offset_bytes[2]);
			break;
		case 29:
			EEPROM_write_byte(0, 86, LTC_b_offset.LTC_offset_bytes[1]);
			break;
		case 30:
			EEPROM_write_byte(0, 87, LTC_b_offset.LTC_offset_bytes[0]);
			break;

		//register 31: LTC b format
		case 31:
			EEPROM_write_byte(0, 88, LTC_setup); 	//Reset LTC b format bits
			break;

		//register 33-34: Dropframe setup + timer/min
		case 32:
			EEPROM_write_byte(0, 89, dropframe_setup);					//Reset dropframe mode
			break;

		case 33:
			EEPROM_write_byte(0, 90, LTC_b_sync_hour);			
			break;
		
		case 34:
			EEPROM_write_byte(0, 91, LTC_b_sync_min);
			break;

		//register 35: Tidszone, timer
		case 35:	
			break;

		//register 36: Tidszone, minutter
		case 36:	
			EEPROM_write_byte(0, 92, LTC_b_watchoffset[0]);
			EEPROM_write_byte(0, 93, LTC_b_watchoffset[1]);
			break;
		//register 37: Sommer/vinter tid skift mode
		case 37:
			EEPROM_write_byte(0, 94, daylight_switch_setup); //reset switch mode
			break;

		//register 38-40: sommertid (måned, dag, år)
		case 38:
			EEPROM_write_byte(0, 95, daylight_on_b_time.month);
			break;
		case 39:
			EEPROM_write_byte(0, 96, daylight_on_b_time.day);
			break;
		case 40:
			EEPROM_write_byte(0, 97, daylight_on_b_time.hour);
			break;

		//register 41-43: vintertid (måned, dag, år)
		case 41:
			EEPROM_write_byte(0, 98, daylight_off_b_time.month);
			break;
		case 42:
			EEPROM_write_byte(0, 99, daylight_off_b_time.day);
			break;
		case 43:
			EEPROM_write_byte(0, 100, daylight_off_b_time.hour);
			break;
	
		//register 50+51 = confirm/annuler skift til sommer/vintertid for hhv. LTC A/B
		case 50:
			EEPROM_write_byte(0, 101, LTC_confirm);
			break;

		case 51:
			EEPROM_write_byte(0, 101, LTC_confirm);
			break;
		//daylight setup LTC A+B bit:0 DL on, bit 1: DL off, bit 4 DL on, bit 5 DL off
		case 56:
			EEPROM_write_byte(0, 102, LTC_a_daylight_flag);
			EEPROM_write_byte(0, 103, LTC_b_daylight_flag);
			break;


	}*/

			EEPROM_write_byte(0, 64, LTC_a_offset.LTC_offset_bytes[3]);		
			EEPROM_write_byte(0, 65, LTC_a_offset.LTC_offset_bytes[2]);
			EEPROM_write_byte(0, 66, LTC_a_offset.LTC_offset_bytes[1]);
			EEPROM_write_byte(0, 67, LTC_a_offset.LTC_offset_bytes[0]);
			EEPROM_write_byte(0, 68, LTC_setup);
			EEPROM_write_byte(0, 69, dropframe_setup);
			EEPROM_write_byte(0, 70, LTC_a_sync_hour);
			EEPROM_write_byte(0, 71, LTC_a_sync_min);
			EEPROM_write_byte(0, 72, LTC_a_watchoffset[0]);
			EEPROM_write_byte(0, 73, LTC_a_watchoffset[1]);
			EEPROM_write_byte(0, 74, daylight_switch_setup);
			EEPROM_write_byte(0, 75, daylight_on_a_time.month);
			EEPROM_write_byte(0, 76, daylight_on_a_time.day);
			EEPROM_write_byte(0, 77, daylight_on_a_time.hour);
			EEPROM_write_byte(0, 78, daylight_off_a_time.month);
			EEPROM_write_byte(0, 79, daylight_off_a_time.day);
			EEPROM_write_byte(0, 80, daylight_off_a_time.hour);
			EEPROM_write_byte(0, 84, LTC_b_offset.LTC_offset_bytes[3]);
			EEPROM_write_byte(0, 85, LTC_b_offset.LTC_offset_bytes[2]);
			EEPROM_write_byte(0, 86, LTC_b_offset.LTC_offset_bytes[1]);
			EEPROM_write_byte(0, 87, LTC_b_offset.LTC_offset_bytes[0]);
			EEPROM_write_byte(0, 88, LTC_setup); 	//Reset LTC b format bits
			EEPROM_write_byte(0, 89, dropframe_setup);					//Reset dropframe mode
			EEPROM_write_byte(0, 90, LTC_b_sync_hour);			
			EEPROM_write_byte(0, 91, LTC_b_sync_min);
			EEPROM_write_byte(0, 92, LTC_b_watchoffset[0]);
			EEPROM_write_byte(0, 93, LTC_b_watchoffset[1]);
			EEPROM_write_byte(0, 94, daylight_switch_setup); //reset switch mode
			EEPROM_write_byte(0, 95, daylight_on_b_time.month);
			EEPROM_write_byte(0, 96, daylight_on_b_time.day);
			EEPROM_write_byte(0, 97, daylight_on_b_time.hour);
			EEPROM_write_byte(0, 98, daylight_off_b_time.month);
			EEPROM_write_byte(0, 99, daylight_off_b_time.day);
			EEPROM_write_byte(0, 100, daylight_off_b_time.hour);
			EEPROM_write_byte(0, 101, LTC_confirm);
			EEPROM_write_byte(0, 101, LTC_confirm);
			EEPROM_write_byte(0, 102, LTC_a_daylight_flag);
			EEPROM_write_byte(0, 103, LTC_b_daylight_flag);

}

void ReadEEpromData(void)
{

		////////// LTC GENERATOR A IND DATA ////////////////
		//register 7-10: LTC a offset
			LTC_a_offset.LTC_offset_bytes[3] = EEPROM_read_byte(0, 64);		
			LTC_a_offset.LTC_offset_bytes[2] = EEPROM_read_byte(0, 65);
			LTC_a_offset.LTC_offset_bytes[1] = EEPROM_read_byte(0, 66);
			LTC_a_offset.LTC_offset_bytes[0] = EEPROM_read_byte(0, 67);

		//register 11: LTC a format
			LTC_setup = EEPROM_read_byte(0, 68);

		//register 12-14: Dropframe setup + timer/min
			dropframe_setup = EEPROM_read_byte(0, 69);
			LTC_a_sync_hour = EEPROM_read_byte(0, 70);	
			LTC_a_sync_min = EEPROM_read_byte(0, 71);

		//register 15: Tidszone, timer
			LTC_a_watchoffset[0] = EEPROM_read_byte(0, 72);

		//register 16: Tidszone, minutter
			 LTC_a_watchoffset[1] = EEPROM_read_byte(0, 73);

		//register 17: Sommer/vinter tid skift mode
			daylight_switch_setup = EEPROM_read_byte(0, 74);

		//register 18-20: sommertid (måned, dag, år)
			daylight_on_a_time.month = EEPROM_read_byte(0, 75);
			daylight_on_a_time.day = EEPROM_read_byte(0, 76);
			daylight_on_a_time.hour = EEPROM_read_byte(0, 77);

		//register 21-23: vintertid (måned, dag, år)
			daylight_off_a_time.month = EEPROM_read_byte(0, 78);
			daylight_off_a_time.day = EEPROM_read_byte(0, 79);
			daylight_off_a_time.hour = EEPROM_read_byte(0, 80);

		////////// LTC GENERATOR B IND DATA ////////////////
		//register 27-30: LTC b offset
			LTC_b_offset.LTC_offset_bytes[3] = EEPROM_read_byte(0, 84);
			LTC_b_offset.LTC_offset_bytes[2] = EEPROM_read_byte(0, 85);
			LTC_b_offset.LTC_offset_bytes[1] = EEPROM_read_byte(0, 86);
			LTC_b_offset.LTC_offset_bytes[0] = EEPROM_read_byte(0, 87);

		//register 31: LTC b format
			LTC_setup = EEPROM_read_byte(0, 88); 	//Reset LTC b format bits

		//register 33-34: Dropframe setup + timer/min
			dropframe_setup = EEPROM_read_byte(0, 89);					//Reset dropframe mode
			LTC_b_sync_hour = EEPROM_read_byte(0, 90);			
			LTC_b_sync_min = EEPROM_read_byte(0, 91);

		//register 35: Tidszone, timer
			LTC_b_watchoffset[0] = EEPROM_read_byte(0, 92);

		//register 36: Tidszone, minutter
			LTC_b_watchoffset[1] = EEPROM_read_byte(0, 93);
		//register 37: Sommer/vinter tid skift mode
			daylight_switch_setup = EEPROM_read_byte(0, 94); //reset switch mode

		//register 38-40: sommertid (måned, dag, år)
			daylight_on_b_time.month = EEPROM_read_byte(0, 95);
			daylight_on_b_time.day = EEPROM_read_byte(0, 96);
			daylight_on_b_time.hour = EEPROM_read_byte(0, 97);

		//register 41-43: vintertid (måned, dag, år)
			daylight_off_b_time.month = EEPROM_read_byte(0, 98);
			daylight_off_b_time.day = EEPROM_read_byte(0, 99);
			daylight_off_b_time.hour = EEPROM_read_byte(0, 100);
	
		//register 50+51 = confirm/annuler skift til sommer/vintertid for hhv. LTC A/B
			LTC_confirm = EEPROM_read_byte(0, 101);

			LTC_confirm = EEPROM_read_byte(0, 101);
		//daylight setup LTC A+B bit:0 DL on, bit 1: DL off, bit 4 DL on, bit 5 DL off
			LTC_a_daylight_flag = EEPROM_read_byte(0, 102);
			LTC_b_daylight_flag = EEPROM_read_byte(0, 103);
}