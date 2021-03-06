#include <C8051F320.h>	// Register definition file.

//leds
sbit fault1_LED = P2^0;
sbit fault2_LED = P2^1;
sbit sync_1_2_LED_sel = P2^2;
sbit man_auto_LED_sel = P2^3;
sbit onair_1_2_LED_sel = P2^4;

//relay + remote en sel
sbit remote_en_sel = P2^5;
sbit auto_sel	= P2^6;
sbit manual_sel = P2^7;
sbit sync1_sel = P1^6;
sbit sync2_sel = P1^7;

//remote in
sbit fault1 = P0^2;
sbit fault2 = P0^1;
sbit on_air = P0^0;
unsigned char fault1_line=0;
unsigned char fault2_line=0;
unsigned char on_air_line=0;

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
		
	fault1_line = fault1_line << 1;
	fault1_line |= fault1;

	fault2_line = fault2_line << 1;
	fault2_line |= fault2;

	on_air_line = on_air_line << 1;
	on_air_line |= on_air;
}

void main()
{
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
	XBR1 = 0x40;	// Crossbar Register 2

	P2MDOUT = 0xFF; // Output configuration for P

	IE=0x08;
	EA=1;

	remote_en_sel = 1;

	while(1)
	{
		//remote_en_sel = remote_en_sw;		
	}
}