#include <string.h>
#include <c8051F120.h>                         // Device-specific SFR Definitions
//#include "globals.h"
#include "uart_1.h"
#include "ublox_prot.h"
#include "ntp.h"
#include "time_utils.h"

extern void set_led(unsigned char led, state);

sbit    GPS_REQ = P3^2;						// Request leap seconds data.
sbit	LED1	= P2^4;

//#include "LTC_handler.h"
//#include "SW_uart.h"

//GPS data variable
//kvantiseringsfejl
signed long xdata quant_error;

//time mode
unsigned char xdata GPS_timemode;

//GPS tids info
union {
	unsigned long GPS_tow_long;
	unsigned char GPS_tow_bytes[4];
}  xdata GPS_tow;

//UTC Time Of Week
union {
	unsigned long UTC_tow_long;
	unsigned char UTC_tow_bytes[4];
}  xdata UTC_tow;

unsigned int xdata GPS_week;
unsigned char xdata GPS_time_valid;
char xdata leap_seconds;

//UTC tids info
struct time xdata UTC_time_buffer;
unsigned char xdata UTC_time_valid;

struct time xdata UTC_Boot_Time;
struct time xdata UTC_ServerStart_Time;

struct leap_info xdata UTC_leap_buffer;

//Konfiguration af timemode
//0x06, 0x1D => Config, tmode
//0x1C, 0x00 => Payload length
//unsigned long => timemode (1= survey in)
//long Fixed Pos X
//long Fixed Pos Y
//long Fixed Pos Z
//unsigned long => standard deviation
//unsigned long => minimum survey in duration (3600*2 sekunder = 2 timer)
//unsigned long => minumum survey in standard deviation

PACKET code timemode =
{0x06, 0x1D, 0x1C, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
 0x10, 0x27, 0x00, 0x00, 0x20, 0x1C, 0x00, 0x00, 0x90, 0xD0,
 0x03, 0x00, 0x00, 0x00};  

 //Konfiguration af port
//0x06, 0x00 => Config, port
//0x14, 0x00 => Payload length
//0x01 => port 1, UART
//0x00, 0x00, 0x00 => Reserverede bytes
//0xD0, 0x08, 0x08, 0x00 => UART mode (8 bit, no parity, 1 stopbit)
//0x80, 0x25, 0x00, 0x00 => BAUD rate = 9600	// 0x00 0x96 0x00 0x00 = 38400
//0x07, 0x00 => Inputprotokoller til modul  (alle)
//0x01, 0x00 => Outputprotokoller fra modul (UBX kun)
//0x00, 0x00 => Autobauding fra
//0x00, 0x00 => Reserverede bytes
PACKET code cfg_prt=
{0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08,
 0x08, 0x00, 0x80, 0x25, 0x00, 0x00, 0x07, 0x00, 0x01, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00};

//states for inlæsning af UBX protokol
enum states {idle, read_id, read_msg, read_ckc} state = idle;

//header for ub_protokol
unsigned char code ub_header[2]= {0xb5, 0x62};

//indlæsnings buffer
unsigned char xdata msg_buf[buffer_length] _at_ 0x1C00; // Workaround 20120627. Placed manually otherwise NTP does not work...
unsigned char ck_a, ck_b;
unsigned char buf_index, str_ptr;
unsigned int length;

unsigned char xdata tmp_buf[16];
unsigned char tmp_buf_ptr;

bit msg_update_flag;
bit acknowledge;

unsigned char ublox_timeout; // Workaround 20120224 for system lockup.
	
void packet_process()
{		
	// Hvis læst for meget data, undgå overflows
	if (buf_index>buffer_length)
	{
		state=idle;
		buf_index=0;
		str_ptr=0;
	}

	if(getch(&msg_buf[buf_index]))
	{
		//der ventes på ny besked (ub-sync karaktere)
		if (state==idle)
		{
			if(msg_buf[buf_index] == ub_header[str_ptr])
				str_ptr++;
			else
				str_ptr=0;

			buf_index++;

			//hvis begge synk karaktere er læst i rigtig rækkefølge, gå til næste state
			if (str_ptr==2)
			{
				str_ptr=0;
				buf_index=0;
				state=read_id;
				ck_a=0;
				ck_b=0;
				length=0;
			}
		}
		
		//der læses besked ID
		else if (state==read_id)
		{
			str_ptr++;

			//beregn checksum
			ck_a+=msg_buf[buf_index];
			ck_b+=ck_a;

			//når fire karaktere læst (ID-MSG-Length (16 bit)), gå til næste state
			if (str_ptr==4)
			{
				length = msg_buf[2]+(msg_buf[3]<<8);
				state=read_msg;
				str_ptr=0;
				if (length>=buffer_length) state=idle; //BS20120629
			}

			buf_index++;
		}

		//indlæs payload
		else if (state==read_msg)
		{
			str_ptr++;

			//beregn checksum
			ck_a+=msg_buf[buf_index];
			ck_b+=ck_a;

			//når alle er læst, gå til næste state
			if(str_ptr==length)
				state=read_ckc;
		
			buf_index++;

			// Hvis læst for meget til buffer, gå idle
			/*if(str_ptr==buffer_length)
			{
				state=idle;
				buf_index=0;
			}*/
		}

		//indlæs checksum
		else if (state==read_ckc)
		{
			str_ptr++;

			//når to ckc er læst
			if(str_ptr==length+2)
			{
				str_ptr=0;
				buf_index=0;
				state=idle;

				//hvis checksummen er korrekt, sæt beskedopdateringsflag højt
				if((ck_a==msg_buf[length+4]) && (ck_b==msg_buf[length+5]))
					msg_update_flag=1;
			}

			buf_index++;
			//hvis hel besked modtaget fra uBlox
	
			if(msg_update_flag==1)
			{	
				interpret_message();
				msg_update_flag=0;
			}

		}
	}
}

void interpret_message()
{
	PACKET xdata in_packet;
	unsigned char count, index;

	get_packet(&in_packet);

	//fortolk klasse
	switch(in_packet.class_num)
	{
		//navigations klasse
		case 0x01:
			//case med mulige id's for nav
			switch(in_packet.id_num)
			{
				//GPS tidshåndtering
				case 0x20:
					//hent skudsekunder
					leap_seconds = in_packet.payload[10];

					//hent TOW
					GPS_tow.GPS_tow_bytes[3]=in_packet.payload[0];
					GPS_tow.GPS_tow_bytes[2]=in_packet.payload[1];
					GPS_tow.GPS_tow_bytes[1]=in_packet.payload[2];
					GPS_tow.GPS_tow_bytes[0]=in_packet.payload[3];
					GPS_tow.GPS_tow_long=(GPS_tow.GPS_tow_long+500)/1000;

					//juster GPS til UTC tid
					GPS_tow.GPS_tow_long-=leap_seconds;

					//hent week
					GPS_week=in_packet.payload[8];
					GPS_week+=(unsigned int)in_packet.payload[9]<<8; //unsigned

					//hent GPS validitet
					GPS_time_valid=in_packet.payload[11];
				break;

				//UTC tidshåndtering
				case 0x21:
					//hent år, 	måned, dag, minut, sekund og validitet 
					UTC_time_buffer.year= in_packet.payload[12]+(in_packet.payload[13]<<8); //2011
					UTC_time_buffer.month= in_packet.payload[14]; //12
					UTC_time_buffer.day= in_packet.payload[15]; //31
					UTC_time_buffer.hour= in_packet.payload[16]; //23
					UTC_time_buffer.min= in_packet.payload[17]; //59
					UTC_time_buffer.sec= in_packet.payload[18]; //47
					UTC_time_valid=in_packet.payload[19];


//					 Leap second test.
/*
					UTC_time_buffer.year = 2015;
					UTC_time_buffer.month= 12;
					UTC_time_buffer.day= 31;
					UTC_time_buffer.hour= 23;
					UTC_time_buffer.min= 59;
					UTC_time_buffer.sec= 47;
					UTC_time_valid=in_packet.payload[19];
*/
					sec_preload_counter = SecsFrom1900( UTC_time_buffer.year,
														UTC_time_buffer.month, 
														UTC_time_buffer.day, 
														UTC_time_buffer.hour, 
														UTC_time_buffer.min, 
														UTC_time_buffer.sec, 0);


				break;

				case 0x30:
					index = 11; //Første index til signal quality
					for(count=0; count < 16; count++)
					{
						signal_quality[count]=in_packet.payload[index];
						index += 1;
						signal_strength[count]=in_packet.payload[index];
						index += 11;

					}

				break;



				default:
					break;
			}
		break;

		//acknowledge
		case 0x05:
			switch(in_packet.id_num)
			{
				//acknowledged
				case 0x01:
					acknowledge=1;
				break;
				//NOT acknowdledged
				case 0x00:
					acknowledge=0;
				break;
				default:
					acknowledge=0;
			}
		break;

		//timing info
		case 0x0D:
			switch(in_packet.id_num)
			{
				//tidspuls info
				case 0x01:
					quant_error=in_packet.payload[8];
					quant_error+=in_packet.payload[9]<<8;
					quant_error+=in_packet.payload[10]<<16;
					quant_error+=in_packet.payload[11]<<24;
				break;

				//survey in status
				case 0x04:
					GPS_timemode = in_packet.payload[24];
				break;
			}
		break;

		//timing info
		case 0x0B:
			GPS_REQ	= 1;
			LED1 = 1;
			switch(in_packet.id_num)
			{
				//Leap second information.
				case 0x02:
					//hent TOW
					UTC_tow.UTC_tow_bytes[3]=in_packet.payload[20];
					UTC_tow.UTC_tow_bytes[2]=in_packet.payload[21];
					UTC_tow.UTC_tow_bytes[1]=in_packet.payload[22];
					UTC_tow.UTC_tow_bytes[0]=in_packet.payload[23];

					UTC_leap_buffer.utc_week_number = in_packet.payload[24]+(in_packet.payload[25]<<8);	// 24 I2 - utcWNT 	- UTC - reference week number (Current week.)			
					UTC_leap_buffer.utc_diff_before = in_packet.payload[26]+(in_packet.payload[27]<<8);	// 26 I2 - utcLS 	- UTC - time difference due to leap seconds before event
					UTC_leap_buffer.utc_leap_week	= in_packet.payload[28]+(in_packet.payload[29]<<8);	// 28 I2 - utcWNF 	- UTC - week number when next leap second event occurs
					UTC_leap_buffer.utc_day_of_week = in_packet.payload[30]+(in_packet.payload[31]<<8);	// 30 I2 - utcDN 	- UTC - day of week when next leap second event occurs
					UTC_leap_buffer.utc_diff_after  = in_packet.payload[32]+(in_packet.payload[33]<<8);	// 32 I2 - utcLSF 	- UTC - time difference due to leap seconds after event

						 if (UTC_tow.UTC_tow_long >= 518400) UTC_leap_buffer.utc_dow = 7;	// Saturday.
					else if (UTC_tow.UTC_tow_long >= 432000) UTC_leap_buffer.utc_dow = 6;	// Friday.
					else if (UTC_tow.UTC_tow_long >= 345600) UTC_leap_buffer.utc_dow = 5;	// Thursday.
					else if (UTC_tow.UTC_tow_long >= 259000) UTC_leap_buffer.utc_dow = 4;	// Wednesday.
					else if (UTC_tow.UTC_tow_long >= 172800) UTC_leap_buffer.utc_dow = 3;	// Tuesday.
					else if (UTC_tow.UTC_tow_long >= 86400)  UTC_leap_buffer.utc_dow = 2;	// Monday.
					else UTC_leap_buffer.utc_dow = 1;										// Sunday.

//					 Leap second test.
/*						UTC_leap_buffer.utc_week_number = 1668;
						UTC_leap_buffer.utc_leap_week = UTC_leap_buffer.utc_week_number;
						UTC_leap_buffer.utc_dow	= 7;
						UTC_leap_buffer.utc_day_of_week = UTC_leap_buffer.utc_dow;
						UTC_leap_buffer.utc_diff_after = UTC_leap_buffer.utc_diff_before + 1;
*/					

					if ( (UTC_leap_buffer.utc_leap_week == UTC_leap_buffer.utc_week_number) && (UTC_leap_buffer.utc_day_of_week == UTC_leap_buffer.utc_dow) )
						UTC_leap_buffer.utc_leap_second = UTC_leap_buffer.utc_diff_after - UTC_leap_buffer.utc_diff_before;
					else UTC_leap_buffer.utc_leap_second = 0;
				
				break;
			}
		break;
	}
}

/*void poll_packet(unsigned char class_num, unsigned char id_num)
{
	PACKET poll_packet;

	poll_packet.class_num=class_num;
	poll_packet.id_num=id_num;
	poll_packet.l_lo=0;
	poll_packet.l_hi=0;

	send_packet(&poll_packet);
}*/

void send_packet(PACKET *out_packet)
{
	ck_a=0; ck_b=0;

	length=out_packet->l_lo;

	//transmit sync chars
	putch(0xB5);
	putch(0x62);

	//transmit class and id
	putch(out_packet->class_num);
	ck_a=out_packet->class_num; ck_b=ck_a;

	putch(out_packet->id_num);
	ck_a=ck_a+out_packet->id_num; ck_b=ck_b+ck_a;

	//transmit length
	putch(out_packet->l_lo);
	ck_a=ck_a+out_packet->l_lo; ck_b=ck_b+ck_a;

	putch(out_packet->l_hi);
	ck_a=ck_a+out_packet->l_hi; ck_b=ck_b+ck_a;

	//transmit payload
	for(buf_index=0; buf_index<length; buf_index++)
	{
			putch(out_packet->payload[buf_index]);
			ck_a=ck_a+out_packet->payload[buf_index]; ck_b=ck_b+ck_a;
	}

	//transmit checksum
	putch(ck_a);
	putch(ck_b);
}

void get_packet(void *in_packet)
{
	//kopier hele beskeden + 4 bytes (ID+Class+2 bytes length)
	memcpy(in_packet, msg_buf, length+4);
}

char setup_ublox()
{
	unsigned int timer;
	unsigned char retry=0;

	//29/04/08 Rev 2: Reset alle variable
	quant_error=0;
	GPS_timemode=0;
	GPS_tow.GPS_tow_long=0;
	GPS_week=0;
	GPS_time_valid=0;
	leap_seconds=0;
	UTC_time_valid=0;
	ck_a=0; ck_b=0;
	buf_index=0; 
	str_ptr=0;
	length=0;
	msg_update_flag=0;
	acknowledge=0;
	memset(msg_buf, 0, sizeof(msg_buf));

	while(!acknowledge && retry<5)
	{
		timer=0;

		//send opsætning for ønsket protokol og baudrate
		send_packet(&cfg_prt);
	
		//vent på at der er modtaget en hel pakke, eller på time out
		while(!msg_update_flag && timer!=1000)
		{
			timer++;
			packet_process();
		}

		retry++;
		
		interpret_message();
		msg_update_flag=0;
	}

	if(retry==5)
		return 0;

	//nulstil ack
	acknowledge=0;

	//send opsætning for timemode
	send_packet(&timemode);


	//send kvantiseringsfejl 1 gang pr. sekund
	set_message_rate(0x0D, 0x01, 1);

	//set GPS tid ud 1 gang pr. sekund
	set_message_rate(0x01, 0x20, 1);

	//set UTC tid ud 1 gang pr. sekund
	set_message_rate(0x01, 0x21, 1);

	//set signal quality ud 1 gang pr. 10 sekunder		message buffer != 200
//	set_message_rate(0x01, 0x30, 11);
	
	//set survey in progress ud 1 gang pr. 10 sekunder
	set_message_rate(0x0D, 0x04, 10);

	return 1;
}

//sætter beskedmodtagelsesrate i sekunder
void set_message_rate(char class_num, char id_num, char rate)
{
	PACKET xdata temp_packet;

	temp_packet.class_num=0x06;
	temp_packet.id_num=0x01;
	temp_packet.l_lo=0x06;
	temp_packet.l_hi=0x00;

	temp_packet.payload[0]=class_num;
	temp_packet.payload[1]=id_num;
	temp_packet.payload[2]=0;
	temp_packet.payload[3]=rate;
	temp_packet.payload[4]=0;
	temp_packet.payload[5]=0;

	send_packet(&temp_packet);
}


void get_UTC_time(struct time *out_time)
{
	memcpy(out_time, &UTC_time_buffer, 7);
}