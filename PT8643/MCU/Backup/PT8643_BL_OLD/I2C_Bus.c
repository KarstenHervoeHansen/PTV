#include <C8051F340.h>
#include "I2C_Bus.h"
#include <intrins.h> // Using nop


sbit sda_int_io = P3^0; //Digital OpenDrain.
sbit scl_int_io = P3^1;	//Digital OpenDrain.
//sbit dir_int_io = P2^2; //Digital PushPull. 	dir_int_io = 0 = MCU -----SDA---> FPGA
						//					 	dir_int_io = 1 = MCU <---SDA----- FPGA
//variable delay
//void delay(char hi, char lo)
void i2c_delay() //Delay 9.20 mS
{
   unsigned int i, j;
   for(i=0; i<40; i++)
	{
  		for(j=0; j<2200; j++){_nop_ (); } //Delay 500 uS.

   	}
}

//short delay
void sdelay() // Delay 30uS.
{
   unsigned int i;
   for(i=0; i<290; i++){_nop_ ();}
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

/*
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
*/

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

/*
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
*/
/*
void i2c_notack()
{
	scl_int_io=0;
	sda_int_io=1;
	sdelay();
	scl_int_io=1;
	sdelay();
	scl_int_io=0;
	sdelay();
}
*/
/*
void i2c_test()
{
	while(1)
	{
		sda_int_io=1;
		sdelay();
		sda_int_io=0;
		sdelay();
	}	
}
*/

char i2c_set_rs232dest(unsigned char dest)
{
	unsigned char LED = 0x00;
	unsigned char ack = 0x00;		//Acknowledge byte;

	switch(dest)
	{
		case RS232_DSUB_MCU:
			LED = 0x07; //3;
		break;
					
		case RS232_DSUB_MF:
			LED = 2;
		break;

		case RS232_MCU_MF:
			LED = 1;
		break;
		case RS232_DISCONNECTED:
			LED = 0x04; //0;
		break;
	}

	i2c_start();

	i2c_sendbyte(0x68);				// Write control byte to IC. (Write Mode)
	ack = i2c_getack();
	if (ack == 0) {return 0;}

	i2c_sendbyte(0xC0);				// Write last 8 bits of address to IC.
	ack = i2c_getack();
	if (ack == 0) {return 0;}

	i2c_sendbyte(LED);				// Write data.
	ack = i2c_getack();
	if (ack == 0) {return 0;}

	i2c_sendbyte(dest);				// Write data.
	ack = i2c_getack();

	i2c_stop();
	i2c_delay();
	return ack;
}
