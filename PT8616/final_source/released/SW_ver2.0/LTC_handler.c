#include <C8051F320.h>
#include <string.h>
#include <stdio.h>
#include "globals.h"
#include "ublox_prot.h"
#include "LTC_handler.h"
#include "i2c_bus.h"
#include "EEPROM_drv.h"
//#include "SW_uart.h"

sbit LED2 = P2^1;
sbit timepulse_status = P1^0;

bit first_time_run = 1;

//antal dage i m?neden
const unsigned char code month_days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

//kvantiseringsfejl i 100 MHz clocksteps
char xdata quant_error_100MHz=0;

//offset for NTSC forskydning
union  {
	unsigned int NTSC_offset_int;
	unsigned char NTSC_offset_bytes[2];
} NTSC_offset;

//tidsstruct for intern tid
//intern tid
struct time xdata int_time;

//intern GPS tow og week
unsigned long xdata int_GPS_tow;
unsigned int xdata int_GPS_week;


//korrigeret LTC tid med tidzoner og NTSC offset
struct time xdata LTC_a_time;			
struct time xdata LTC_b_time;


//offsets for ur - ltc a og ltc b (tidszoner)
char xdata LTC_a_watchoffset[3];
char xdata LTC_b_watchoffset[3];

//frame offsets
unsigned char xdata LTC_a_frames=0;
unsigned char xdata LTC_b_frames=0;

// LTC setup byte
// 0-1 = LTC A format ("00" = 24 fps, "01" = 25 fps, "10" = 30/1,001 fps, "11" = 30 FPS)
// 2 = dropframe, hvis NTSC 0 = non-drop, 1 = drop
// 4-5 = LTC B format ("00" = 24 fps, "01" = 25 fps, "10" = 30/1,001 fps, "11" = 30 FPS)
// 6 = dropframe, hvis NTSC 0 = non-drop, 1 = drop
unsigned char xdata LTC_setup=0;


//Daylight saving switch mode
//0-1 = Dropframe mode ("00" = NONE, "01" = CONFIRM, "10" = AUTO)
//4-5 = Dropframe mode ("00" = NONE, "01" = CONFIRM, "10" = AUTO)
unsigned char xdata daylight_switch_setup=0;

struct time xdata daylight_on_a_time;	//sommertid for LTC A
struct time xdata daylight_off_a_time;	//vintertid for LTC A
struct time xdata daylight_on_b_time;	//sommertid for LTC B
struct time xdata daylight_off_b_time;	//vintertid for LTC B

//sommer/vinter tids variable
bit LTC_a_daylight_flag=0;
bit LTC_b_daylight_flag=0;

//Request byte (request for sommer/vintertid)
//sendes til PT5300, hvor en menu skal bekr?fte/annullere at s?tte indstilling
//Bekr?ftelse sendes tilbage til GPS modul
unsigned char xdata LTC_request=0;

#define	request_LTC_a_daylight_on_bit		0	//0: H?j = request for skift til sommertid	(LTC A)
#define	request_LTC_a_daylight_off_bit		1	//1: H?j = request for skift til vintertid	   --
#define request_LTC_a_daylight_mode_bit		2	//2: H?j = Confirm mode / Lav = Auto mode      --
#define	request_LTC_b_daylight_on_bit		4	//4: H?j = request for skift til sommertid	(LTC B)
#define	request_LTC_b_daylight_off_bit		5	//5: H?j = request for skift til vintertid	   --
#define request_LTC_b_daylight_mode_bit		6	//6: H?j = Confirm mode / Lav = Auto mode      --


//LTC Dropframe sync setup
//0-1 = Dropframe mode ("00" = NONE, "01" = CONFIRM, "10" = AUTO)
//2-3 = reserveret
//4-5 = Dropframe mode ("00" = NONE, "01" = CONFIRM, "10" = AUTO)
//6-7 = reserveret
unsigned char xdata dropframe_setup=0;


//sync tider til LTC a og b (auto eller confirm sync)
unsigned char xdata LTC_a_sync_hour;
unsigned char xdata LTC_a_sync_min;
unsigned char xdata LTC_b_sync_hour;
unsigned char xdata LTC_b_sync_min;
unsigned long xdata secs_since_sync_a=0;
unsigned long xdata secs_since_sync_b=0;


//Request byte for LTC sync
unsigned char xdata LTC_sync_request=0;

#define request_LTC_a_resync_bit			0	//0: H?j = LTC a sync request
#define request_LTC_a_resync_mode_bit		1	//1: H?j = LTC a confirm mode, lav = auto mode
#define request_LTC_b_resync_bit			4	//4: H?j = LTC b sync request
#define request_LTC_b_resync_mode_bit		5	//5: H?j = LTC b confirm mode, lav = auto mode


//offsets for ltc a og ltc b
union { 
	char LTC_offset_bytes[4];
	long LTC_offset_long;
} xdata LTC_a_offset;

union { 
	char LTC_offset_bytes[4];
	long LTC_offset_long;
} xdata LTC_b_offset;




//omregnet tid til BCD kodede bytes
unsigned char xdata BCD_coded_time_a[4];
unsigned char xdata BCD_coded_time_b[4];

//strenge, der indeholder LTC tidspunkter i format "TT:MM:SS   "
char xdata LTC_a_str[11];
char xdata LTC_b_str[11];


//Confirm byte (confirm for sommer/vintertid + NTSC resync)
//0: H?j = bekr?ft LTC A sommer/vinter skift
//1: H?j = annuler LTC A sommer/vinter skift
//2: H?j = bekr?ft LTC A resync
//3: H?j = annuler LTC A resync
//4: H?j = bekr?ft LTC B sommer/vinter skift
//5: H?j = annuler LTC B sommer/vinter skift
//6: H?j = bekr?ft LTC B resync
//7: H?j = annuler LTC B resync
unsigned char xdata LTC_confirm=0;


//long til udregning af diverse
union  {
	unsigned long temp_long;
	unsigned char bytes[4];
} temp_long;


//I2C variable
unsigned char byte_index;
unsigned char i2c_byte;

//buffer til uddata til FPGA
char xdata send_buffer[21];




//konvertere et bin?rt tal (8 bit) til 2x4 bit 10'ere og 1'ere (TTTT EEEE)
unsigned char bin_to_BCD(unsigned char binary)
{
	unsigned char tens, ones, BCD;

	ones = binary%10;
	tens = binary/10;
	BCD = (tens<<4)|ones;

	return BCD;
}




//inkrementer tiden med ?t sekund
void sec_inc()
{
	int_time.sec++;

	//incrementer minutter
	if (int_time.sec==60)
	{
		int_time.sec=0;
		int_time.min++;
	
		//incrementer timer
		if (int_time.min==60)
		{
			int_time.min=0;
			int_time.hour++;
	
			//incrementer dage
			if (int_time.hour==24)
			{
				int_time.hour=0;
				int_time.day++;

				//incrementer m?neder (med h?jde for dag 29 i skud?r)
				if ((int_time.year%4)==0 && int_time.month==2)
				{
					if(int_time.day==30)
					{
						int_time.day=1;
						int_time.month++;
					}
				}

				else if (int_time.day>month_days[int_time.month-1])
				{
					int_time.day=1;
					int_time.month++;
				}

				//inkrementer ?r
				if (int_time.month==13)
				{
					int_time.month=1;
					int_time.year++;
				}
			}
		}
	}
}




//forskyd tiden frem/tilbage
//m? kun forskyde i +/- 60 sekunder og minutter og +/- 24 timer
void offset_time(struct time in_time, struct time *out_time, char hours, char mins, char secs)
{
	struct time temp_time;

	//besk?ring
	memcpy(&temp_time, &in_time, 7);

	//h?ndter sekunder
	temp_time.sec+=secs;

	//hvis overskredet minut nedaf
	if(temp_time.sec<0)
	{
		temp_time.min--;
		temp_time.sec+=60; //wrap rundt
	}
	//overskredet minut opad
	else if(temp_time.sec>59)
	{
		temp_time.min++;
 		temp_time.sec=temp_time.sec%60;
	}


	//h?ndter minutter
	temp_time.min+=mins;
	
	//hvis negativ
	if(temp_time.min<0)
	{
		temp_time.hour--;
		temp_time.min+=60;
	}
	//hvis positiv
	else if(temp_time.min>59)
	{
		temp_time.hour++;
		temp_time.min=temp_time.min%60;
	}


	//h?ndter timer
	temp_time.hour+=hours;

	//hvis negativ
	if(temp_time.hour<0)
	{
		temp_time.day--;
		temp_time.hour+=24;
	}
	//hvis positiv
	else if(temp_time.hour>23)
	{
		temp_time.day++;
		temp_time.hour=temp_time.hour%24;
	}

	//h?ndter dag/m?ned/?r
	//overskredet m?ned negativt
	if(temp_time.day<1)
	{
		temp_time.month--;

		if((int_time.year%4)==0 && temp_time.month==2)
			temp_time.day=29;
		else
			temp_time.day=month_days[temp_time.month-1];
		
		if(temp_time.month<1)
		{
			temp_time.year--;
			temp_time.month=12;
		}
	}

	//overskredet m?ned positivt (skud?r og februar)
	else if((int_time.year%4)==0 && temp_time.month==2)
	{
		if(temp_time.day>29)
		{
			temp_time.month++;
			temp_time.day=1;
		}
	}

	else if(temp_time.day>month_days[temp_time.month-1])
	{
		temp_time.month++;
		temp_time.day=1;
	}

	if(temp_time.month>12)
	{
		temp_time.year++;
		temp_time.month=1;
	}

	memcpy(out_time, &temp_time, 7);
}








//k?res hvert sekund (500 ms efter GPS sekund start)
void LTC_manager()
{
	//NTSC
	unsigned long NTSC_offset_long;
	int skipped_frames = 0;

	char temp_offset[3];
	unsigned long temp_sec_offset = 0;
	unsigned long temp_secs_since_sync;
	
	//////////////////////////////////////////////////////////////////
	//			    	intern tidsh?ndtering						//
	//////////////////////////////////////////////////////////////////

	// 17/11/08 - Rev 3: ?ndret p? synkronisering mellem intern og GPS tid

	//hent tider n?r GPS er tilg?ngelig
	if(timepulse_status==1)
	{
		get_UTC_time(&int_time);
		int_GPS_tow=GPS_tow.GPS_tow_long;
	}

	//inkrementer intern clocks
	sec_inc();
	int_GPS_tow=(int_GPS_tow+1)%604800;
	
	//inkrementer sekunder siden NTSC sync
	secs_since_sync_a++;
	secs_since_sync_b++;

	//hvis ny uge, inkrementer intern ugenummer
	if(int_GPS_tow==0)
		int_GPS_week++;

	
	//hent kvantiseringsfejl (i ps, omregnet til antal 100 MHz clocks) symmetrisk "rund op"
	if(quant_error<0)
		quant_error_100MHz = (quant_error-5000)/10000;
	else
		quant_error_100MHz = (quant_error+5000)/10000;

	//////////////////////////////////////////////////////////////////
	//			    	PAL 1/4 sync h?ndtering						//
	//////////////////////////////////////////////////////////////////
	
	//lav signal hvert 4 sekund, GPS tilpasset
	NTSC_offset_long = (int_GPS_week%143)*604800;		//uge*sekunder siden start
	NTSC_offset_long = NTSC_offset_long + int_GPS_tow;	//sekunder i nuv?rende uge
	NTSC_offset_long = NTSC_offset_long%4;
	
	if(NTSC_offset_long==0)
		set_bit(&FPGA_system_control, 1);
	else
		clear_bit(&FPGA_system_control, 1);


	//////////////////////////////////////////////////////////////////
	//			    	NTSC forskydningsh?ndtering					//
	//////////////////////////////////////////////////////////////////

	//udregn framestart forkskydning
	NTSC_offset_long = (int_GPS_week%143)*604800;		//uge*sekunder siden start
	NTSC_offset_long = NTSC_offset_long + int_GPS_tow;	//sekunder i nuv?rende uge
	NTSC_offset_long = NTSC_offset_long%1001;

	NTSC_offset.NTSC_offset_int=(unsigned int)NTSC_offset_long;



	//////////////////////////////////////////////////////////////////
	//						LTC A h?ndtering						//
	//////////////////////////////////////////////////////////////////

	//kopier offset til midlertidig offset, hvor sommer/vinter tidsforskel s?ttes
	memcpy(temp_offset, LTC_a_watchoffset, 3);

	//s?t til korrekt tidszone og s?t sommer/vintertid
	if(LTC_a_daylight_flag==1)
		temp_offset[0]=temp_offset[0]+1;

	//////////////////////////////////////////////////////////////////
	//					LTC NTSC tidsforsinkelse					//
	//////////////////////////////////////////////////////////////////

	//hvis format = NTSC, udregn LTC tids forsinkelse
	if((LTC_setup&0x03)==0x02)
	{			
		LTC_a_frames=0;

		//udregn forsinkelse, udfra sekunder siden sidste sync
		temp_offset[1]-=(secs_since_sync_a/60060);
		temp_offset[2]-=((secs_since_sync_a/1001)%60);
		
	//////////////////////////////////////////////////////////////////
	//					LTC DROPFRAME kompensering					//
	//////////////////////////////////////////////////////////////////

		//hvis drop-frame, kompenser for dette
		if((LTC_setup&0x04)==0x04)
		{	
			skipped_frames=0;

			//gem i temp_sss, hvor mange NTSC(!) sekunder det er siden sync
			temp_secs_since_sync=secs_since_sync_a+temp_offset[1]*60+temp_offset[2];

			//skipped frames uger
			skipped_frames = (temp_secs_since_sync / 86400) * 2592;
			temp_secs_since_sync -= (temp_secs_since_sync / 86400)*86400;
			
			//skipped frames timer
			skipped_frames += (temp_secs_since_sync / 3600) * 108;
			temp_secs_since_sync -= (temp_secs_since_sync / 3600)*3600;
	
			//skipped frames minutter
			skipped_frames += (temp_secs_since_sync / 60) * 2;
			skipped_frames -= (temp_secs_since_sync / 600) * 2; //spring 10+20+30+40+50 over
			temp_secs_since_sync -= (temp_secs_since_sync / 60)*60; //hele minutter

			//spring 00 over
			skipped_frames += 2;
			
			temp_offset[0] += skipped_frames/108000; //skip 30*3600 frames (timer)
			skipped_frames -= (skipped_frames/108000)*108000;

			temp_offset[1] += skipped_frames/1800; //skip 30*60 frames (minutter)
			skipped_frames -= (skipped_frames/1800)*1800;
			
			temp_offset[2] += skipped_frames/30;	//skip 30 frames (sekunder)
			skipped_frames -= (skipped_frames/30)*30;

			if(skipped_frames<0)
				skipped_frames = 0;

			LTC_a_frames = skipped_frames;	//rest = hele frames
		}
	}
	//Hvis ikke 30/1,001 frekvens
	else
	{
		temp_offset[2] = 0;
		LTC_a_frames = 0;
	}

	
	//////////////////////////////////////////////////////////////////
	//					LTC manual offsetting						//
	//////////////////////////////////////////////////////////////////
	
	temp_long.temp_long=LTC_a_offset.LTC_offset_long;

	//hvis negativ forskydning, t?l et sekund forud
	if(LTC_a_offset.LTC_offset_long<0)
	{
		temp_offset[2]=temp_offset[2]+1; //var +1 
		temp_long.temp_long+=148500000;
	}

	//h?ndter at (delay + NTSC forskydning) ikke overskrider gr?nser
	if ((LTC_setup&3) == 0x02)
	{
		temp_long.temp_long+=(NTSC_offset.NTSC_offset_int*148500);

		//lav ikke puls, hvis 1001. frame
		if ((temp_long.temp_long>148500000) && (temp_long.temp_long<(148500000+148500)))
		{
			temp_long.temp_long = -1;
		}

		else if (temp_long.temp_long>=(148500000+148500))
		{
			temp_long.temp_long-=(148500000+148500);
			temp_offset[2]=temp_offset[2]-1;
		}
	}

	LTC_a_frames+=1; 	//forskyd frames, da denne hele tiden er ?n bagud i FPGA
	
	//hvis overskredet 29 frames, t?l ?t sekund mere
	if(LTC_a_frames<30)
		offset_time(int_time, &LTC_a_time, temp_offset[0], temp_offset[1], temp_offset[2]);
	else
	{
		LTC_a_frames=0;
		offset_time(int_time, &LTC_a_time, temp_offset[0], temp_offset[1], temp_offset[2]+1);
	}

	send_buffer[7] = temp_long.bytes[3];
	send_buffer[8] = temp_long.bytes[2];
	send_buffer[9] = temp_long.bytes[1];
	send_buffer[10] = temp_long.bytes[0];

	//////////////////////////////////////////////////////////////////
	//					LTC resync checking							//
	//////////////////////////////////////////////////////////////////

	//Hvis i en 30/1.001 mode, check for resync
	if((LTC_setup&0x03)==0x02)
	{
		//hvis i CONFIRM mode
		if((dropframe_setup&0x0F) == 0x01)
		{
			if(LTC_a_time.sec==0)
			{
				if ((LTC_a_time.hour==LTC_a_sync_hour) && (LTC_a_time.min == LTC_a_sync_min))
				{
					//request for resync
					set_bit(&LTC_sync_request, request_LTC_a_resync_bit);
					set_bit(&LTC_sync_request, request_LTC_a_resync_mode_bit);
				}
			}
		}

		//hvis i AUTO mode
		else if((dropframe_setup&0x0F) == 0x02)
		{
			if(LTC_a_time.sec==0)
			{
				if ((LTC_a_time.hour==LTC_a_sync_hour) && (LTC_a_time.min == LTC_a_sync_min))
				{
					//auto resync
					secs_since_sync_a = 0;
					set_bit(&LTC_sync_request, request_LTC_a_resync_bit);
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////
	//					LTC BCD konvertering						//
	//////////////////////////////////////////////////////////////////

	BCD_coded_time_a[0]=bin_to_BCD(LTC_a_time.hour);
	BCD_coded_time_a[1]=bin_to_BCD(LTC_a_time.min);
	BCD_coded_time_a[2]=bin_to_BCD(LTC_a_time.sec);
	BCD_coded_time_a[3]=bin_to_BCD(LTC_a_frames);






	//////////////////////////////////////////////////////////////////
	//						LTC B h?ndtering						//
	//////////////////////////////////////////////////////////////////

	//kopier offset til midlertidig offset, sommer/vinter tidsforskel s?ttes
	memcpy(temp_offset, LTC_b_watchoffset, 3);

	//s?t til korrekt tidszone og s?t sommer/vintertid
	if(LTC_b_daylight_flag==1)
		temp_offset[0]=temp_offset[0]+1;

	//////////////////////////////////////////////////////////////////
	//					LTC NTSC tidsforsinkelse					//
	//////////////////////////////////////////////////////////////////

	//hvis format = NTSC, udregn LTC tids forsinkelse
	if((LTC_setup&0x30)==0x20)
	{	
		LTC_b_frames=0;
		
		//udregn forsinkelse, udfra sekunder siden sidste sync
		temp_offset[1]-=(secs_since_sync_b/60060);
		temp_offset[2]-=((secs_since_sync_b/1001)%60);
		
	//////////////////////////////////////////////////////////////////
	//					LTC DROPFRAME kompensering					//
	//////////////////////////////////////////////////////////////////

		//hvis drop-frame, kompenser for dette
		if((LTC_setup&0x40)==0x40)
		{	
			skipped_frames=0;
			//gem i temp_sss, hvor mange NTSC(!) sekunder det er siden sync
			temp_secs_since_sync=secs_since_sync_b+temp_offset[1]*60+temp_offset[2];

			//skipped frames uger
			skipped_frames = (temp_secs_since_sync / 86400) * 2592;
			temp_secs_since_sync -= (temp_secs_since_sync / 86400)*86400;
			
			//skipped frames timer
			skipped_frames += (temp_secs_since_sync / 3600) * 108;
			temp_secs_since_sync -= (temp_secs_since_sync / 3600)*3600;
	
			//skipped frames minutter
			skipped_frames += (temp_secs_since_sync / 60) * 2;
			skipped_frames -= (temp_secs_since_sync / 600) * 2; //spring 10+20+30+40+50 over
			temp_secs_since_sync -= (temp_secs_since_sync / 60)*60;

			//spring 00 over
			skipped_frames += 2;
			
			temp_offset[0] += skipped_frames/108000; //skip 30*3600 frames (timer)
			skipped_frames -= (skipped_frames/108000)*108000;

			temp_offset[1] += skipped_frames/1800; //skip 30*60 frames (minutter)
			skipped_frames -= (skipped_frames/1800)*1800;

			temp_offset[2] += skipped_frames/30;	//skip 30 frames (sekunder)
			skipped_frames -= (skipped_frames/30)*30;

			if(skipped_frames<0)
				skipped_frames = 0;

			LTC_b_frames = skipped_frames;	//rest = hele frames
		}
	}
	//Hvis ikke 30/1,001 frekvens
	else
	{
		temp_offset[2] = 0;
		LTC_b_frames = 0;
	}

	
	//////////////////////////////////////////////////////////////////
	//					LTC manual offsetting						//
	//////////////////////////////////////////////////////////////////
	
	temp_long.temp_long=LTC_b_offset.LTC_offset_long;

	//hvis negativ forskydning, t?l et sekund forud
	if(LTC_b_offset.LTC_offset_long<0)
	{
		temp_offset[2]=temp_offset[2]+1; //var +1
		temp_long.temp_long+=148500000;
	}

	//h?ndter at (delay + NTSC forskydning) ikke overskrider gr?nser
	if ((LTC_setup&0x30) == 0x20)
	{
		temp_long.temp_long+=(NTSC_offset.NTSC_offset_int*148500);

		//lav ikke puls, hvis 1001. frame
		if ((temp_long.temp_long>148500000) && (temp_long.temp_long<(148500000+148500)))
		{
			temp_long.temp_long = -1;
		}

		else if (temp_long.temp_long>=(148500000+148500))
		{
			temp_long.temp_long-=(148500000+148500);
			temp_offset[2]=temp_offset[2]-1;
		}
	}

 	LTC_b_frames+=1; 	//forskyd frames, da denne hele tiden er ?n bagud i FPGA


	//hvis overskredet 29 frames, t?l ?t sekund mere
	if(LTC_b_frames<30)
		offset_time(int_time, &LTC_b_time, temp_offset[0], temp_offset[1], temp_offset[2]);
	else
	{
		LTC_b_frames=0;
		offset_time(int_time, &LTC_b_time, temp_offset[0], temp_offset[1], temp_offset[2]+1);
	}

	send_buffer[15] = temp_long.bytes[3];
	send_buffer[16] = temp_long.bytes[2];
	send_buffer[17] = temp_long.bytes[1];
	send_buffer[18] = temp_long.bytes[0];

	//////////////////////////////////////////////////////////////////
	//					LTC resync checking							//
	//////////////////////////////////////////////////////////////////

	//Hvis i en 30/1.001 mode, check for resync
	if((LTC_setup&0x30)==0x20)
	{
		//hvis i CONFIRM mode
		if((dropframe_setup&0xF0) == 0x10)
		{
			if(LTC_b_time.sec==0)
			{
				if ((LTC_b_time.hour==LTC_b_sync_hour) && (LTC_b_time.min == LTC_b_sync_min))
				{
					//request for resync
					set_bit(&LTC_sync_request, request_LTC_b_resync_bit);
					set_bit(&LTC_sync_request, request_LTC_b_resync_mode_bit);
				}
			}
		}

		//hvis i AUTO mode
		else if((dropframe_setup&0xF0) == 0x20)
		{
			if(LTC_b_time.sec==0)
			{
				if ((LTC_b_time.hour==LTC_b_sync_hour) && (LTC_b_time.min == LTC_b_sync_min))
				{
					//auto resync
					secs_since_sync_b = 0;
					set_bit(&LTC_sync_request, request_LTC_b_resync_bit);
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////
	//					LTC BCD konvertering						//
	//////////////////////////////////////////////////////////////////

	BCD_coded_time_b[0]=bin_to_BCD(LTC_b_time.hour);
	BCD_coded_time_b[1]=bin_to_BCD(LTC_b_time.min);
	BCD_coded_time_b[2]=bin_to_BCD(LTC_b_time.sec);
	BCD_coded_time_b[3]=bin_to_BCD(LTC_b_frames);



	//////////////////////////////////////////////////////////////////
	//					Dropframe sync h?ndtering					//
	//////////////////////////////////////////////////////////////////

	//s?rg for f?rste resync
	if((UTC_time_valid==7) && first_time_run)
	{
		secs_since_sync_a=(int_GPS_tow%600);
		secs_since_sync_b=(int_GPS_tow%600);
		first_time_run=0;
	}
}


void check_daylight_switch()
{
	/////////////////// LTC A ////////////////////

	//check for sommer vintertid (kun p? hele timer) (skal g?res EFTER NTSC offsetting)
	if( (LTC_a_time.min==0) && (LTC_a_time.sec==0) )
	{
		//check for sommertid
		if( (LTC_a_time.month==daylight_on_a_time.month) && (LTC_a_time.day==daylight_on_a_time.day) && (LTC_a_time.hour==daylight_on_a_time.hour))
		{
			switch (daylight_switch_setup&0x03)
			{
				//Hvis none
				case 0x00:			
					break;
				//Hvis confirm
				case 0x01:			
					set_bit(&LTC_request, request_LTC_a_daylight_on_bit);	//s?t request for sommertid + confirm mode
					set_bit(&LTC_request, request_LTC_a_daylight_mode_bit);
					break;
				//Hvis auto
				case 0x02:
					LTC_a_daylight_flag=1;	//s?t til sommertid
					set_bit(&LTC_request, request_LTC_a_daylight_on_bit);		//s?t request til sommertid + auto mode 
					break;	
			}
		}

		//check for vintertid
		else if( (LTC_a_time.month==daylight_off_a_time.month) && (LTC_a_time.day==daylight_off_a_time.day) && (LTC_a_time.hour==daylight_off_a_time.hour))
		{	
			switch (daylight_switch_setup&0x03)
			{
				//Hvis none
				case 0x00:
					break;
				//Hvis confirm
				case 0x01:
					set_bit(&LTC_request, request_LTC_a_daylight_off_bit);	//s?t request for vintertid + confirm mode
					set_bit(&LTC_request, request_LTC_a_daylight_mode_bit);
					break;
				//Hvis auto
				case 0x02:
					LTC_a_daylight_flag=0;	//s?t til vintertid
					set_bit(&LTC_request, request_LTC_a_daylight_off_bit);	//s?t request for vintertid + auto mode
					break;	
			}
		}
	}

	/////////////////// LTC B ////////////////////
	
	//check for sommer vintertid (kun p? hele timer) (skal g?res EFTER NTSC offsetting)
	if( (LTC_b_time.min==0) && (LTC_b_time.sec==0) )
	{
		//check for sommertid
		if( (LTC_b_time.month==daylight_on_b_time.month) && (LTC_b_time.day==daylight_on_b_time.day) && (LTC_b_time.hour==daylight_on_b_time.hour))
		{
			switch (daylight_switch_setup&0x30)
			{
				//Hvis none
				case 0x00:			
					break;
				//Hvis confirm
				case 0x10:			
					set_bit(&LTC_request, request_LTC_b_daylight_on_bit);	//s?t request for sommertid + confirm mode
					set_bit(&LTC_request, request_LTC_b_daylight_mode_bit);
					break;
				//Hvis auto
				case 0x20:
					LTC_b_daylight_flag=1;	//s?t til sommertid
					set_bit(&LTC_request, request_LTC_b_daylight_on_bit);		//s?t request til sommertid + auto mode 
					break;	
			}
		}

		//check for vintertid
		else if( (LTC_b_time.month==daylight_off_b_time.month) && (LTC_b_time.day==daylight_off_b_time.day) && (LTC_b_time.hour==daylight_off_b_time.hour))
		{	
			switch (daylight_switch_setup&0x30)
			{
				//Hvis none
				case 0x00:
					break;
				//Hvis confirm
				case 0x10:
					set_bit(&LTC_request, request_LTC_b_daylight_off_bit);	//s?t request for vintertid + confirm mode
					set_bit(&LTC_request, request_LTC_b_daylight_mode_bit);
					break;
				//Hvis auto
				case 0x20:
					LTC_b_daylight_flag=0;	//s?t til vintertid
					set_bit(&LTC_request, request_LTC_b_daylight_off_bit);	//s?t request for vintertid + auto mode
					break;	
			}
		}
	}
}


void confirm_daylight_switch()
{
	//hvis LTC A skift bekr?ftet
	if(LTC_confirm&1)
	{
		//hvis sommertid var requestet
		if(LTC_request&1)
			LTC_a_daylight_flag=1;
		//hvis vintertid var requestet
		else if(LTC_request&2)
			LTC_a_daylight_flag=0;

		LTC_request&=0xF8;	//slet LTC a sommer/vintertids request
		LTC_confirm&=0xFC;	//slet LTC a sommer/vintertids confirms
	}
	//hvis LTC A skift annuleret
	else if(LTC_confirm&2)
	{
		LTC_request&=0xF8;	//slet LTC a sommer/vintertids request
		LTC_confirm&=0xFC;	//slet LTC a sommer/vintertids confirms
	}

	//hvis LTC B skift bekr?ftet
	if(LTC_confirm&16)
	{
		//hvis sommertid var requestet
		if(LTC_request&16)
			LTC_b_daylight_flag=1;
		//hvis vintertid var requestet
		else if(LTC_request&32)
			LTC_b_daylight_flag=0;

		LTC_request&=0x8F;	//slet LTC b sommer/vintertids request
		LTC_confirm&=0xCF;	//slet LTC b sommer/vintertids confirms
	}
	//hvis LTC B skift annuleret
	else if(LTC_confirm&32)
	{
		LTC_request&=0x8F;	//slet LTC b sommer/vintertids request
		LTC_confirm&=0xCF;	//slet LTC b sommer/vintertids confirms
	}
}


void update_LTC_strings()
{
	sprintf(LTC_a_str, "%02d:%02d:%02d", (int)LTC_a_time.hour, (int)LTC_a_time.min, (int)LTC_a_time.sec);
	sprintf(LTC_b_str, "%02d:%02d:%02d", (int)LTC_b_time.hour, (int)LTC_b_time.min, (int)LTC_b_time.sec);

	//sprintf(LTC_a_str, "%08d", (int)OCXO_phase_diff);
}

	////////////////////// TRANSMISSION ////////////////////////////////////


void transmit_LTC_data()
{
	//pak data i udbuffer
	send_buffer[0] = quant_error_100MHz;
	send_buffer[1] = NTSC_offset.NTSC_offset_bytes[1];
	send_buffer[2] = NTSC_offset.NTSC_offset_bytes[0];
	send_buffer[3] = BCD_coded_time_a[0];
	send_buffer[4] = BCD_coded_time_a[1];
	send_buffer[5] = BCD_coded_time_a[2];
	send_buffer[6] = BCD_coded_time_a[3];
	//7-10 = LTC a offset long
	send_buffer[11] = BCD_coded_time_b[0];
	send_buffer[12] = BCD_coded_time_b[1];
	send_buffer[13] = BCD_coded_time_b[2];
	send_buffer[14] = BCD_coded_time_b[3];
	//15-18 = LTC b offset long
	send_buffer[19] = LTC_setup;
	send_buffer[20] = FPGA_system_control;

	//foretag sending
	FPGA_write_array(0, &send_buffer, 21);
}


//reset alle parametre til 0 eller EEPROM-state
void init_LTC()
{
	quant_error_100MHz=0;
	secs_since_sync_a=0;
	secs_since_sync_b=0;
	LTC_request=0;
	LTC_sync_request=0;
	LTC_confirm=0;
	LTC_setup=0x00;

	//tidsstructs for intern og ublox tid, samt 2x LTC
	memset(&int_time, 0, sizeof (int_time));
	memset(&LTC_a_time, 0, sizeof (LTC_a_time));
	memset(&LTC_b_time, 0, sizeof (LTC_b_time));

	memset(BCD_coded_time_a, 0, sizeof (BCD_coded_time_a));
	memset(BCD_coded_time_b, 0, sizeof (BCD_coded_time_b));

	memset(LTC_a_str, 0, 11);
	memset(LTC_b_str, 0, 11);

	LTC_a_daylight_flag=0;
	LTC_b_daylight_flag=0;

	LTC_a_frames=0;
	LTC_b_frames=0;

	int_GPS_tow=0;
	int_GPS_week=0;

	memset(&daylight_on_a_time, 0, sizeof (daylight_on_a_time));
	memset(&daylight_off_a_time, 0, sizeof (daylight_off_a_time));

	memset(&daylight_on_b_time, 0, sizeof (daylight_on_b_time));
	memset(&daylight_off_b_time, 0, sizeof (daylight_off_b_time));

	memset(&LTC_a_watchoffset, 0, sizeof (LTC_a_watchoffset));
	memset(&LTC_b_watchoffset, 0, sizeof (LTC_b_watchoffset));

	LTC_a_offset.LTC_offset_long=0;
	LTC_b_offset.LTC_offset_long=0;
}