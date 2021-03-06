#include <C8051F320.h>
#include "i2c_bus.h"

sbit sda_int_io = P1^1;
sbit scl_int_io = P1^2;


//variable delay
void delay(char hi, char lo)
{
	TCON &= 0xCF;	//stop timer, clear OF flag
	TL0 = lo;		//0xE8
	TH0 = hi;
	TCON |= 0x10;	//start timer
	while(! (TCON&0x20))
	;
}

//short delay
void sdelay()
{
	TCON &= 0xCF;	//stop timer, clear OF flag
	TL0 = 0xE8;		//0xE8
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
	sdelay();

	return inbyte;
}

char i2c_getack()
{
	//get acknowledge
	unsigned char count=0;
	char sda_temp=1;
	sda_int_io=1;
	while(sda_temp!=0)	//while sda != low
	{
		sda_temp=sda_int_io;

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

unsigned char FPGA_read_byte(unsigned char subaddress)
{
	unsigned char in_byte=0;

	i2c_start();
	i2c_sendbyte(0xF0);
	i2c_getack();
	i2c_sendbyte(subaddress);
	i2c_getack();
	i2c_start();
	i2c_sendbyte(0xF1);
	i2c_getack();
	in_byte=i2c_readbyte();
	i2c_getack();
	i2c_stop();

	return in_byte;
}

void FPGA_write_byte(unsigned char subaddress, unsigned char byte)
{
	i2c_start();
	i2c_sendbyte(0xF0);
	i2c_getack();
	i2c_sendbyte(subaddress);
	i2c_getack();
	i2c_sendbyte(byte);
	i2c_getack();
	i2c_stop();
}

void FPGA_write_array(unsigned char subaddress, unsigned char *array, unsigned char num_of_bytes)
{
	unsigned char byte_count=0;

	i2c_start();
	i2c_sendbyte(0xF0);
	i2c_getack();
	i2c_sendbyte(subaddress);
	i2c_getack();

	for(byte_count=0; byte_count<num_of_bytes; byte_count++)
	{
		i2c_sendbyte(array[byte_count]);
		i2c_getack();
	}

	i2c_stop();
}