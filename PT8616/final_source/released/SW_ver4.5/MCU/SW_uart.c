#include <C8051F320.h>
#include <string.h>

//transmit pin
sbit TX = P2^6;
sbit LED2 = P2^1;

//char for outbuffer
bit CLK;
unsigned char SWBUF;
unsigned char BITCOUNT;

unsigned char xdata outbuffer[128];
unsigned char outptr, outindex;

//timer1 interrupt (38400x2 Hz)
void TX_gen() interrupt 3
{
	//clock devider
	if(CLK)
	{
		//hvis flere bytes i outbuffer
		if(outptr!=outindex)
		{
			//idle mode
			if(BITCOUNT==0)
			{
				LED2=0;
				//send start bit, load buffer
				TX=0;
				SWBUF=outbuffer[outptr];
				BITCOUNT++;
			}
		
			//send stop bit
			else if(BITCOUNT==9)
			{
				TX=1;
				BITCOUNT++;
			}
			//hel byte sendt, gå til idle mode
			else if(BITCOUNT==10)
			{
				LED2 = 1;
				//send stop bit
				TX=1;
				outptr=(++outptr)&127;
				BITCOUNT=0;
			}
			//ellers transmiter enkelte bits
			else
			{
				if(SWBUF&1)
					TX=1;
				else
					TX=0;
		
				SWBUF=SWBUF>>1;
				BITCOUNT++;
			}
		}
	}
	CLK=~CLK;
}

//Setup af SW UART
void setup_SW_uart()
{
	outptr=0;
	outindex=0;
	memset(outbuffer, 0, sizeof(outbuffer));

	SWBUF=0;

	//enabler timer 0 interrupt, til generering af RS232 data
	ET1=1;
}

//send enkelt karakter
void SW_putch(unsigned char out)
{
	//vent på at der ikke sende længere
	while(outptr!=outindex)
		;

	outbuffer[outindex++]=out;
	outindex&=127;
}

//send en streng
void SW_sendstr(unsigned char *str, unsigned char length)
{
	unsigned char bytecount=0;

	for(bytecount; bytecount<length; bytecount++)
		SW_putch(str[bytecount]);
}
