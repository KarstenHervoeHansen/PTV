#include <C8051F320.h>
#include "inouts.h"
#include "i2c_bus.h"

void tmr1_isr() interrupt 3
{
}

//short delay
void sdelay()
{
	TCON &= 0xCF;	//stop timer, clear OF flag
	TL0 = 0x00;		//0xE8
	TH0 = 0xFF;
	TCON |= 0x10;	//start timer
	while(! (TCON&0x20))
	;
}

void i2c_start()
{
	scl_int_io = 1;
	sdelay();
	sda_int_io = 0;
	sdelay();
	scl_int_io = 0;
	sdelay();
	sda_int_io = 1;
	sdelay();
}

void i2c_stop()
{
	sda_int_io = 0;
	scl_int_io = 1;
	sdelay();
	sda_int_io = 1;
	sdelay();
}

void i2c_sendbyte(char byte)
{
	char count=0;

	for(count; count<8; count++)
	{
		sda_int_io=byte&128;
		sdelay();
		scl_int_io=1;
		sdelay();
		scl_int_io=0;
		byte= byte << 1;
		sdelay();
	}

	//end transmission
	sda_int_io=1;
	//scl=1;

	sdelay();
}

unsigned char i2c_readbyte()
{
	unsigned char inbyte=0;
	char count=0;

	for(count; count<8; count++)
	{
		inbyte=inbyte<<1;
		scl_int_io=1;
		sdelay();
		inbyte|=sda_int_io;
		scl_int_io=0;
		sdelay();
	}

	sda_int_io=1;
	//scl=1;

	sdelay();

	return inbyte;
}

char i2c_getack()
{
	//get acknowledge
	unsigned char count=0;
	sda_int_io=1;
	//scl=1;
	while(sda_int_io)	//while sda != low
	{
		count++;
		if(count==255)	//if timeout
			return 0;
	}

	scl_int_io=1;
	sdelay();
	scl_int_io=0;
	sdelay();

	return 1;
}

void i2c_giveack()
{
	scl_int_io=0;
	sda_int_io=0;
	sdelay();
	scl_int_io=1;
	sdelay();
	scl_int_io=0;
	sdelay();
}
