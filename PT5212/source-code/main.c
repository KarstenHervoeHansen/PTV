#include <C8051F320.h>	// Register definition file.

//leds
sbit fault1_LED = P2^0;
sbit fault2_LED = P2^1;
sbit sync_1_2_LED_sel = P2^2;
sbit man_auto_LED_sel = P2^3;
sbit onair_1_2_LED_sel = P2^4;
sbit ext_remote_LED = P1^2;

                             
//relay + remote en sel
sbit remote_en_sel = P2^5;
sbit auto_sel	= P2^6;
sbit manual_sel = P2^7;
sbit sync1_sel = P1^6;
sbit sync2_sel = P1^7;

sbit rel_sense1 = P1^0;
sbit rel_sense2 = P1^1;

//remote in
sbit fault1 = P0^0;
sbit fault2 = P0^1;
sbit on_air = P0^2;
unsigned char fault1_=0;
unsigned char fault2_line=0;
unsigned char on_air_line=0;
unsigned char VDD_Check=0;

//switch in

sbit auto_sel_sw = P0^3;
sbit manual_sel_sw = P0^4;
sbit sync1_sw = P0^5;
sbit sync2_sw = P0^6;
sbit remote_en_sw = P0^7;
unsigned char auto_sel_line=0;
unsigned char manual_sel_line=0;
unsigned char sync1_line=0;
unsigned char sync2_line=0;
unsigned char remote_en_line=0;
void vardelay(int time);
void sw_ver(int version);

char gen_flag = 0;
char man_auto_flag = 0;
char remote_en_flag = 0;

//debouncing in line
void timer_0() interrupt 3
{

	auto_sel_line = auto_sel_line << 1;
	auto_sel_line |= auto_sel_sw;

	manual_sel_line = manual_sel_line << 1;
	manual_sel_line |= manual_sel_sw;

	sync1_line = sync1_line << 1;
	sync1_line |= sync1_sw;

	sync2_line = sync2_line << 1;
	sync2_line |= sync2_sw;

	remote_en_line = remote_en_line << 1;
	remote_en_line |= remote_en_sw;

	on_air_line = on_air_line << 1;
	on_air_line |= on_air;
}

void vardelay(int time)
{
   int i, j;

   for(i=0; i<time; i++)
	{
    	for(j=0; j<100; j++)
		{
		    i=i+0;
    	}
   	}
}

void sw_ver(int version)
{
   int i;

   for(i=0; i<version; i++)
	{
	 	ext_remote_LED = 1;
		vardelay(250);
		ext_remote_LED = 0;
		vardelay(250);
   	}
		vardelay(750);
}

void main()
{
	remote_en_sel = 0;

	VDM0CN = 0x80; //Enable VDD Monitor.

	PCA0MD &= ~0x40;   
	OSCXCN = 0x00;	// EXTERNAL Oscillator Control Register	
    CLKSEL = 0x00;  // Oscillator Clock Select Register
	OSCICN = 0x80;	// Internal Oscillator Control Register

	CKCON = 0x08;     // Timer1 bruger SYSCLK
	TL1   = 0x00;     // Timer 1 Low Byte
    TH1   = 0x00;     // BaudRate = 38400
    TMOD  = 0x20;     // Timer Mode Register
    TCON  = 0x45;     // Timer Control Register, timer1 enabled
	TR1 = 1;       	  // Start timer1

	XBR0 = 0x00;	// Crossbar Register 1
	XBR1 = 0x40; //40;	// Crossbar Register 2

	P0SKIP    = 0xFF;
	P1SKIP    = 0xFF;
	P2SKIP    = 0xDF;
	
	P1MDIN = 0xFF; // Input configuration for P1

	P0MDOUT   = 0x00;
	P1MDOUT   = 0x04;
	P2MDOUT = 0xFF; // Output configuration for P

	sw_ver(2); //Blink Remote Led to show software revision.

	RSTSRC = 0x02;//Enable VDD Monitor as reset source.
	IE=0x08;
	EA=1;

	vardelay(100);
	PCA0MD = 0x00; //Disable WD Timer.
	PCA0CPL4 = 0xFF; //Set WD Timer timeout interval.

//	AMX0P = 0xFF;
//	AMX0N = 0xFF;
//	ADC0CN = 0x80;
//	REF0CN = 0x08;
//	AD0BUSY = 1;

	ext_remote_LED = 1;

	//sæt primær gen = on air gen
	if(on_air_line==0)
	{
		sync1_sel=0;
		vardelay(25);
		sync1_sel=1;
	}
	else
	{
		sync2_sel=0;
		vardelay(25);
		sync2_sel=1;
	}

	PCA0MD = 0x40;	//Enable the WD Timer (System Clock Divide by 12)

	while(1)
	{
	PCA0CPH4 = 0xFF; //Update WD Timer...

	/*if((fault1 == 0) && (fault2 == 0)) {fault1_LED = 0; fault2_LED = 1;}// Gen1 Fail
	if((fault1 != 0) && (fault2 != 0)) {fault1_LED = 1; fault2_LED = 1;}// No Fail
	if((fault1 != 0) && (fault2 == 0)) {fault1_LED = 0; fault2_LED = 0;}// Both Fail
	if((fault1 == 0) && (fault2 != 0)) {fault1_LED = 1; fault2_LED = 0;}// Gen2 Fail*/

	if((fault1 == 0) && (fault2 == 0)) {fault1_LED = 0; fault2_LED = 0;}// Both Fail
	if((fault1 != 0) && (fault2 != 0)) {fault1_LED = 1; fault2_LED = 1;}// No Fail
	if((fault1 == 0) && (fault2 != 0)) {fault1_LED = 0; fault2_LED = 1;}// Gen1 Fail
	if((fault1 != 0) && (fault2 == 0)) {fault1_LED = 1; fault2_LED = 0;}// Gen2 Fail

	
	onair_1_2_LED_sel = on_air_line;

	man_auto_flag = rel_sense2;
	gen_flag = rel_sense1;

	//hvis der trykkes på remote enable
	if (remote_en_line == 1) 
	{
		vardelay(100);
		remote_en_sel = !remote_en_sel;
		remote_en_flag = remote_en_sel;
	}

	//hvis remote er disabled, følg on_air
	if (remote_en_flag == 0)
	{
		if(on_air_line==1)
		{
			sync2_sel=0;
			vardelay(25);
		}

		else if (on_air_line == 254)
		{
			sync1_sel=0;
			vardelay(25);
		}
	}	
	
	//hvis remote enabled og i manual-mode
	if((remote_en_flag == 1) && (man_auto_flag == 0))
	{
		if(auto_sel_line == 1)
		{
			auto_sel=0;
			vardelay(25);
		}

		if(sync1_line==1)
		{
			sync1_sel=0;
			vardelay(25);
		}

		if(sync2_line==1)
		{
			sync2_sel=0;
			vardelay(25);
		}
	}

	//hvis remote enabled og i auto-mode
	if((remote_en_flag == 1) && (man_auto_flag == 1))
	{
		//hvis der slås tilbage til manual mode
		if(manual_sel_line == 1)
		{
			//sæt prim sync = on air gen
			if(on_air_line==0)
			{
				sync1_sel=0;
				vardelay(25);
				sync1_sel=1;
			}
			else
			{
				sync2_sel=0;
				vardelay(25);
				sync2_sel=1;
			}

			manual_sel = 0;
			vardelay(25);
		}
		
		//hvis der skiftes generator
		if((sync1_line==1) || (sync2_line==1)) 
		{
			//sæt prim sync = on air gen
			if(on_air_line==0)
			{
				sync1_sel=0;
				vardelay(25);
				sync1_sel=1;
			}
			else
			{
				sync2_sel=0;
				vardelay(25);
				sync2_sel=1;
			}

			//sæt til manual mode og tilbage for evt. reset
			manual_sel = 0;
			vardelay(25);
			manual_sel = 1;
			auto_sel = 0;
			vardelay(25);
			auto_sel = 1;

			//set primær gen til den på knapperne valgte generator
			if(sync2_line==0)
			{
				sync1_sel=0;
				vardelay(25);
			}
			else
			{
				sync2_sel=0;
				vardelay(25);
			}
		}
	}

	man_auto_LED_sel = !rel_sense2;
	sync_1_2_LED_sel = rel_sense1;

	auto_sel = 1;
	manual_sel = 1;
	sync2_sel = 1;
	sync1_sel = 1;
	
	/*
	VDD_Check = checkbit(VDM0CN, 0x40); 
	if (VDD_Check == 0)
	{
		remote_en_sel = 0;
	} 
	*/

	}

}