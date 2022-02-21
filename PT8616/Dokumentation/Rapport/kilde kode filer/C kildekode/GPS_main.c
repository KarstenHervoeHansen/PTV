#include <C8051F320.h>
#include <string.h>
#include "inouts.h"
#include "UART.h"
#include "GPS_init.h"
#include "NavSync_prot.h"
#include "i2c_bus.h"
#include "i2c_slave.h"

char reset_flag=1;
char rise_flag=0;
char fall_flag=0;

unsigned char hours; 
unsigned char mins; 
unsigned char secs;

unsigned long GPS_week;
unsigned long GPS_tow;
unsigned long clock_offset;
unsigned long temp;
unsigned char send_byte;

sbit PLL_lock_flag_i = P2^3;
sbit GPS_lock_flag_o = P2^4;
sbit FPGA_reset_o = P2^5;

void int_fall() interrupt 0
{
	fall_flag=1;
}

void int_rise() interrupt 2
{
	rise_flag=1;
}

char in_byte=0;
unsigned long xdata buffer[20];
unsigned char index=0;

void main()
{
	//disable watchdog timer
	PCA0MD &= ~0x40;     
	
	//Setup sekvens
	setup_ports();
	setup_osc();
    setup_timer1();
    setup_timer3();
	setup_I2C();
	setup_UART();
    
	FPGA_reset_o=1;

 	EA=1;
	ES0=0;
	//send kun polyt-beskeder
	setup_msg("$PRTHS,U1OP,NMEA,ALL=0,PLT=1,GLL=1");
	//dynamic = stationary, but unknown position
	setup_msg("$PRTHS,DYNA,1");
	//output 250 KHz
	setup_msg("$PRTHS,FRQD,10");
	ES0=1;

	FPGA_reset_o=0;
	memset(buffer, 0, 256);
	
	//main løkke
	while(1)
	{
		//output GPS lås
		LED1_o = !gps_valid;

		//hvis hel NMEA besked indlæst, opdater tid/GPS status
		if(update_flag)
		{
			get_info();
			update_flag=0;
		}

		if(reset_flag==1 && PLL_lock_flag_i==1 && gps_valid==1)
		{
			GPS_week = timing_info[5];
			GPS_tow = timing_info[6];

			clock_offset = GPS_week%143;
			clock_offset = clock_offset*604800+GPS_tow+1;
			clock_offset = clock_offset%1001;
			clock_offset = 148500000 - ((clock_offset*4450549)%148500000);

			//send til FPGA (0xBB = modtage offset)
			i2c_start();
			i2c_sendbyte(0xA0);
			i2c_giveack();
			i2c_sendbyte(0xBB);
			i2c_giveack();
			i2c_stop();

			send_byte = (unsigned char) clock_offset&0xff;
			clock_offset = clock_offset << 8;

			//send MSB byte
			i2c_start();
			i2c_sendbyte(0xA0);
			i2c_giveack();
			i2c_sendbyte(send_byte);
			i2c_giveack();
			i2c_stop();

			send_byte = (unsigned char) clock_offset&0xff;
			clock_offset = clock_offset << 8;
				
			//2. byte
			i2c_start();
			i2c_sendbyte(send_byte);
			i2c_giveack();
			i2c_sendbyte(0xAA);
			i2c_giveack();
			i2c_stop();

			send_byte = (unsigned char) clock_offset&0xff;
			clock_offset = clock_offset << 8;

			//3. byte
			i2c_start();
			i2c_sendbyte(0xA0);
			i2c_giveack();
			i2c_sendbyte(send_byte);
			i2c_giveack();
			i2c_stop();

			send_byte = (unsigned char) clock_offset&0xff;
			clock_offset = clock_offset << 8;

			//LSB byte
			i2c_start();
			i2c_sendbyte(0xA0);
			i2c_giveack();
			i2c_sendbyte(send_byte);
			i2c_giveack();
			i2c_stop();

			reset_flag = 0;
		}
		
		//opdater tid til LTC i FPGA
		if(rise_flag==1 && reset_flag!=1)
		{			
			GPS_lock_flag_o=gps_valid;
			LED0_o=1;
			temp=timing_info[0];

			secs=temp%100; temp=temp/100;
			mins=temp%100; temp=temp/100;
			hours=temp%100;

			i2c_start();
			i2c_sendbyte(0xA0);
			i2c_giveack();
			i2c_sendbyte(0xAA);
			i2c_giveack();
			i2c_stop();
		
			i2c_start();
			i2c_sendbyte(0xA0);
			i2c_giveack();
			i2c_sendbyte(hours);
			i2c_giveack();
			i2c_stop();
		
			i2c_start();
			i2c_sendbyte(0xA0);
			i2c_giveack();
			i2c_sendbyte(mins);
			i2c_giveack();
			i2c_stop();
		
			i2c_start();
			i2c_sendbyte(0xA0);
			i2c_giveack();
			i2c_sendbyte(secs);
			i2c_giveack();
			i2c_stop();
			
			rise_flag=0;
			LED0_o=0;
		}

		//læs output fra FPGA
		/*	i2c_start();
			i2c_sendbyte(0xA1);
			i2c_giveack();
			in_byte=i2c_readbyte();
			buffer[index]=in_byte;
			index++;
			i2c_giveack();
			i2c_stop();*/
	}
}