#include <C8051F320.h>
//#include <string.h>

void setup_ports()
{
	IT01CF=0x91;
	EX1=1;
	EX0=1;

	XBR0 = 0x05;  	// Crossbar Register 1 (enable UART + SMBus)
  	XBR1 = 0x40;  	// Crossbar Register 2 (weak pull-ups globalt disabled)

	//sæt ubrugte inputs til analog = formindsk strømforbrug+støj
	P0MDIN = 0x33;  // Input configuration for P0 (ext xtal)
    P1MDIN = 0x0F;  // Input configuration for P1 (0-3 = digitale (I2C))
  	P2MDIN = 0x7F;  // Input configuration for P2 (0-6 = digitale (gpio, LEDS, dip))
    P3MDIN = 0x00;  // Input configuration for P3 (alle analoge)

	//sæt I2C=open-drain, resten push-pull
    P0MDOUT = 0xC3; // Output configuration for P0 
    P1MDOUT = 0xF0; // Output configuration for P1 (0-3 I2C = opendrain)
	P2MDOUT = 0xFF; // Output configuration for P2 
    P3MDOUT = 0xFF; // Output configuration for P3

	//brug ekstern oscillator, SDA og SCL på P1.0 og P1.1
    P0SKIP = 0xCF;  // Port 0 Crossbar Skip Register (skip ext xtal)
    P1SKIP = 0x00;  // Port 1 Crossbar Skip Register
 	P2SKIP = 0x00;  // Port 2 Crossbar Skip Register
}

void setup_timer1()
{
	CKCON = 0x08;     // Timer1 bruger SYSCLK
	TL1   = 0x00;     // Timer 1 Low Byte
    TH1   = 0x70;     // BaudRate = 38400
    TMOD  = 0x21;     // Timer Mode Register
    TCON  = 0x45;     // Timer Control Register, timer1 enabled
	TR1 = 1;       	  // Start timer1
}

void setup_timer3()
{
	TMR3RLH=0xA6; TMR3RLL=0x00;	//Timer3 reload => 25 ms overflow
	TMR3H=TMR3RLH; TMR3L=TMR3RLL;
	TMR3CN=0x04;				//16 bit autoreload, sysclk/12, timer enabled
	EIE1|=0x80;					//enable timer3 interrupt
}

void setup_osc()
{
	int n=0;
	//Oscillator setup
	OSCXCN = 0x67;  		// EXTERNAL Oscillator Control Register >10 MHz

	// vent til krystal er stabiliseret
	while ( (OSCXCN & 0x80) == 0 )  ;              			
	
	CLKSEL = 0x01;   		// = Ekstern Oscillator
}
