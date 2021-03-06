#include <string.h>
#include <c8051f320.h>
#include "globals.h"
#include "serial.h"
#include "ublox_prot.h"
#include "LTC_handler.h"
#include "SW_uart.h"

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

unsigned int xdata GPS_week;
unsigned char xdata GPS_time_valid;
char xdata leap_seconds;

//UTC tids info
struct time xdata UTC_time_buffer;
unsigned char xdata UTC_time_valid;

// Monitor HW
unsigned int noise;
unsigned int maxnoise;

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

// Konfiguration af timing
// 0x06, 0x1A = Config, Nav2
// 0x28, 0x00 = payload length
// 0x01 = stationary
// 0x00, 0x00,0x00 = reserved
// 0x01 = Initial Min SVs to fix
// 0x03, 0x10 = Min/Max SVs 
// 0x02 = Position fix mode = 2 (auto 2d/3d)
// 0x50,0xC3, 0x00, 0x00 = Fixed altitude (500 m)
// 0x0f = MinCN0Initial (15 dBHz)
// 0x0A = Min C/N0 (10 dBHz)
// 0x05 = Min elevation for satellites to be used
// 0x3c = Timeout for differential correction data
// 0x00 = Dead reconing timeout
// 0x01 = Navopt (1 = All RAIM checks enabled)
// 0x00, 0x00 = reserved
// 0xFA, 0x00 = Position DOP Mask
// 0xFA, 0x00 = Time DOP Mask
// 0x64, 0x00 = Position accuracy mask
// 0x2C, 0x01 = Time accuracy mask
// 0x00 = Static threshold (0 = disabled)
// 11 x 0x00 = reserved

PACKET code nav2 = 
{0x06, 0x1A, 0x28, 0x00, 0x01, 0x00, 0x00, 0x00, 
0x01, 0x03, 0x10, 0x02, 0x50, 0xC3, 0x00, 0x00, 0x0F, 0x0A, 
0x05, 0x3C, 0x00, 0x01, 0x00, 0x00, 0xFA, 0x00, 0xFA, 0x00, 
0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//states for inl?sning af UBX protokol
enum states {idle, read_id, read_msg, read_ckc} state;

//header for ub_protokol
unsigned char code ub_header[2]= {0xb5, 0x62};

//indl?snings buffer
unsigned char xdata msg_buf[buffer_length];
unsigned char ck_a, ck_b;
unsigned char buf_index, str_ptr;
unsigned int length;

bit msg_update_flag;
bit acknowledge;
bit gps_utc_init_ok = 0;
//Rutine der processesere UBLOX beskeder, byte for byte
void packet_process()
{		
	// Hvis l?st for meget data, undg? overflows
	if (buf_index>buffer_length)
	{
		state=idle;
		buf_index=0;
		str_ptr=0;
	}

	if(getch(&msg_buf[buf_index]))
	{
		//der ventes p? ny besked (ub-sync karaktere)
		if (state==idle)
		{
			if(msg_buf[buf_index] == ub_header[str_ptr])
				str_ptr++;
			else
				str_ptr=0;

			buf_index++;

			//hvis begge synk karaktere er l?st i rigtig r?kkef?lge, g? til n?ste state
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
		
		//der l?ses besked ID
		else if (state==read_id)
		{
			str_ptr++;

			//beregn checksum
			ck_a+=msg_buf[buf_index];
			ck_b+=ck_a;

			//n?r fire karaktere l?st (ID-MSG-Length (16 bit)), g? til n?ste state
			if (str_ptr==4)
			{
				length = msg_buf[2]+(msg_buf[3]<<8);
				state=read_msg;
				str_ptr=0;
			}

			buf_index++;
		}

		//indl?s payload
		else if (state==read_msg)
		{
			str_ptr++;

			//beregn checksum
			ck_a+=msg_buf[buf_index];
			ck_b+=ck_a;

			//n?r alle er l?st, g? til n?ste state
			if(str_ptr==length)
				state=read_ckc;
		
			buf_index++;

			// Hvis l?st for meget til buffer, g? idle
			/*if(str_ptr==buffer_length)
			{
				state=idle;
				buf_index=0;
			}*/
		}

		//indl?s checksum
		else if (state==read_ckc)
		{
			str_ptr++;

			//n?r to ckc er l?st
			if(str_ptr==length+2)
			{
				str_ptr=0;
				buf_index=0;
				state=idle;

				//hvis checksummen er korrekt, s?t beskedopdateringsflag h?jt
				if((ck_a==msg_buf[length+4]) && (ck_b==msg_buf[length+5]))
					msg_update_flag=1;
			}

			buf_index++;
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
		case 0x0A:
			switch(in_packet.id_num)
			{
				// MON-HW
				case 0x09:
					noise = in_packet.payload[16] + (in_packet.payload[17]<<8);
					
					if (noise>maxnoise)
						maxnoise = noise;
				break;
			}
		break;

		//navigations klasse
		case 0x01:
			//case med mulige id's for nav
			switch(in_packet.id_num)
			{
				//GPS tidsh?ndtering
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
					GPS_week+=(unsigned int)in_packet.payload[9]<<8;

					//hent GPS validitet
					GPS_time_valid=in_packet.payload[11];
				break;

				//UTC tidsh?ndtering
				case 0x21:
					//hent ?r, 	m?ned, dag, minut, sekund og validitet 
					UTC_time_buffer.year=in_packet.payload[12]+(in_packet.payload[13]<<8);
					UTC_time_buffer.month=in_packet.payload[14];
					UTC_time_buffer.day=in_packet.payload[15];
					UTC_time_buffer.hour=in_packet.payload[16];
					UTC_time_buffer.min=in_packet.payload[17];
					UTC_time_buffer.sec=in_packet.payload[18];
					UTC_time_valid=in_packet.payload[19];

					// 20-01-12 - Rev. 2.6 BS: gps_utc_init_ok angiver for LTC_Handler at GPS tid er modtaget. (Hack)
					gps_utc_init_ok=1;
				break;

				case 0x30:
					index = 12; //F?rste index til signal quality
					for(count=0; count < 16; count++)
					{
						signal_quality[count]=in_packet.payload[index];
						index=index+12;
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

		default:
			index = 0;
			break;
	}
}

void poll_packet(unsigned char class_num, unsigned char id_num)
{
	PACKET xdata poll_packet;

	poll_packet.class_num=class_num;
	poll_packet.id_num=id_num;
	poll_packet.l_lo=0;
	poll_packet.l_hi=0;

	send_packet(&poll_packet);
}

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

		//send ops?tning for ?nsket protokol og baudrate
		send_packet(&cfg_prt);
	
		//vent p? at der er modtaget en hel pakke, eller p? time out
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

	//send ops?tning for timemode
	send_packet(&timemode);
	send_packet(&nav2);

	//send kvantiseringsfejl 1 gang pr. sekund
	set_message_rate(0x0D, 0x01, 1);

	//set GPS tid ud 1 gang pr. sekund
	set_message_rate(0x01, 0x20, 1);

	//set UTC tid ud 1 gang pr. sekund
	set_message_rate(0x01, 0x21, 1);

	//set signal quality ud 1 gang pr. 10 sekunder
	set_message_rate(0x01, 0x30, 11);
	
	//set survey in progress ud 1 gang pr. 10 sekunder
	set_message_rate(0x0D, 0x04, 10);

	return 1;
}

//s?tter beskedmodtagelsesrate i sekunder
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