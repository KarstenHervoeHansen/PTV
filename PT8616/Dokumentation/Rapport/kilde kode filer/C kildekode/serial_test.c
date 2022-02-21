#include <C8051F320.h>
#include <string.h>

sbit sda = P1^3;
sbit scl = P1^2;
sbit LED = P2^0;

unsigned int index=0;
unsigned char count=0;
unsigned char xdata buffer[1024];

void tmr1_isr() interrupt 3
{
}

void delay()
{
	TCON &= 0xCF;	//stop timer, clear OF flag
	TL0 = 0xFF;
	TH0 = 0x4B;
	TCON |= 0x10;	//start timer
	while(! (TCON&0x20))
	;
}

//short delay
void sdelay()
{
	TCON &= 0xCF;	//stop timer, clear OF flag
	TL0 = 0xF0;
	TH0 = 0xFF;
	TCON |= 0x10;	//start timer
	while(! (TCON&0x20))
	;
}

void i2c_start()
{
	scl = 1;
	sdelay();
	sda = 0;
	sdelay();
	scl = 0;
	sdelay();
	sda = 1;
	sdelay();
}

void i2c_stop()
{
	sda = 0;
	scl = 1;
	sdelay();
	sda = 1;
	sdelay();
}

void i2c_sendbyte(char byte)
{
	char count=0;

	for(count; count<8; count++)
	{
		sda=byte&128;
		sdelay();
		scl=1;
		sdelay();
		scl=0;
		byte= byte << 1;
	}

	//end transmission
	sda=1;
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
		scl=1;
		sdelay();
		inbyte|=sda;
		scl=0;
		sdelay();
	}

	sda=1;
	//scl=1;

	sdelay();

	return inbyte;
}

char i2c_getack()
{
	//get acknowledge
	unsigned char count=0;
	sda=1;
	//scl=1;
	while(sda)	//while sda != low
	{
		count++;
		if(count==255)	//if timeout
			return 0;
	}

	scl=1;
	sdelay();
	scl=0;
	sdelay();

	return 1;
}

void i2c_giveack()
{
	scl=0;
	sda=0;
	sdelay();
	scl=1;
	sdelay();
	scl=0;
	sdelay();
}


void write_prom(unsigned char address, char byte)
{
	i2c_start();
	i2c_sendbyte(0xA0);
	i2c_getack();
	i2c_sendbyte(address);
	i2c_getack();
	i2c_sendbyte(byte);
	i2c_getack();
	i2c_stop();
}

char read_prom(unsigned char address)
{
	char inbyte=0;

	i2c_start();
	i2c_sendbyte(0xA0);
	i2c_getack();
	i2c_sendbyte(address);
	i2c_getack();
	i2c_start();
	i2c_sendbyte(0xA1);
	i2c_getack();
	inbyte=i2c_readbyte();
	i2c_stop();

	return inbyte;
}

char read_address()
{
	char inbyte=0;

	i2c_start();
	i2c_sendbyte(0xA1);
	i2c_getack();
	inbyte=i2c_readbyte();
	i2c_stop();

	return inbyte;
}
		
void main(void)
{
	
	PCA0MD &= ~0x40;     //disable watchdog timer

	//sæt ubrugte inputs til analog = formindsk strømforbrug+støj)
	P0MDIN = 0x03;  // Input configuration for P0 (ext xtal)
    P1MDIN = 0x0F;  // Input configuration for P1 (0-3 = digitale (I2C))
  	P2MDIN = 0x7F;  // Input configuration for P2 (0-6 = digitale (gpio, LEDS, dip))
    P3MDIN = 0x00;  // Input configuration for P3 (alle analoge)

	//sæt I2C=open-drain, resten push-pull
    P0MDOUT = 0x00; // Output configuration for P0 
    P1MDOUT = 0x00; // Output configuration for P1 (0-3 I2C = opendrain)
	P2MDOUT = 0xFF; // Output configuration for P2 
    P3MDOUT = 0xFF; // Output configuration for P3

	//brug ekstern oscillator   
    P0SKIP = 0x0C;  // Port 0 Crossbar Skip Register (skip ext xtal)
    P1SKIP = 0x00;  // Port 1 Crossbar Skip Register
 	P2SKIP = 0x00;  // Port 2 Crossbar Skip Register

	XBR0 = 0x01;  	// Crossbar Register 1 (enable UART)
  	XBR1 = 0x40;  	// Crossbar Register 2 (weak pull-ups globalt disabled)

  	OSCXCN = 0x67;  // EXTERNAL Oscillator Control Register                     // wait for osc to start
  	while ( (OSCXCN & 0x80) == 0 )
    ;                              // wait for xtal to stabilize

	//setup timer1
	TL1   = 0x00;     // Timer 1 Low Byte
    TH1   = 0x70;     // BaudRate = 9600
    TMOD  = 0x21;     // Timer Mode Register
    TCON  = 0x40;     // Timer Control Register

	CKCON=0x00;

  	CLKSEL = 0x01;   // = External Oscillator

	//enable interrupts
	ET1=1;
	EA=1;

	memset(&buffer, 0x00, 1024);

	for (index=0; index<1024; index++)
	{
		i2c_start();
		i2c_sendbyte(0xA1);
		i2c_getack();
		buffer[index]=i2c_readbyte();
		i2c_getack();
		i2c_stop();
		
		for (count = 0; count < 20; count++)
			delay();

		LED=~LED;
	}

	while(1)
		LED=0;

}