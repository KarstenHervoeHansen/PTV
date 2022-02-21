#include <c8051F120.h>                      	// Device-specific SFR Definitions
#include "ini.h"
#include "mn_userconst.h"                   	// TCP/IP Library Constants
#include "mn_stackconst.h"                  	// TCP/IP Library Constants
#include "mn_errs.h"                        	// Library Error Codes
#include "mn_defs.h"                        	// Library Type definitions
#include "mn_funcs.h"                       	// Library Function Prototypes

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F12x
//-----------------------------------------------------------------------------
sfr16 RCAP2    = 0xca;                         // Timer2 reload value
sfr16 TMR2     = 0xcc;                         // Timer2 counter

//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Configure the Interrupts, Crossbar and GPIO ports
//
void PORT_Init (void)
{
    // P0.0  -  TX0 (UART0), Push-Pull,  Digital
    // P0.1  -  RX0 (UART0), Open-Drain, Digital
    // P0.2  -  SCK  (SPI0), Open-Drain, Digital
    // P0.3  -  MISO (SPI0), Open-Drain, Digital
    // P0.4  -  MOSI (SPI0), Open-Drain, Digital
    // P0.5  -  NSS  (SPI0), Open-Drain, Digital
    // P0.6  -  SDA (SMBus), Open-Drain, Digital
    // P0.7  -  SCL (SMBus), Open-Drain, Digital

    // P1.0  -  TX1 (UART1), Push-Pull,  Digital
    // P1.1  -  RX1 (UART1), Open-Drain, Digital
    // P1.2  -  CEX0 (PCA),  Open-Drain, Digital
    // P1.3  -  CEX1 (PCA),  Open-Drain, Digital
    // P1.4  -  CEX2 (PCA),  Open-Drain, Digital
    // P1.5  -  CEX3 (PCA),  Open-Drain, Digital
    // P1.6  -  INT0 (Tmr0), Push-Pull,  Digital
    // P1.7  -  INT1 (Tmr1), Open-Drain, Digital

    // P2.0  -  Unassigned,  Push-Pull,  Digital
    // P2.1  -  Unassigned,  Push-Pull,  Digital
    // P2.2  -  Unassigned,  Push-Pull,  Digital
    // P2.3  -  Unassigned,  Push-Pull,  Digital
    // P2.4  -  Unassigned,  Push-Pull,  Digital
    // P2.5  -  Unassigned,  Push-Pull,  Digital
    // P2.6  -  Unassigned,  Push-Pull,  Digital
    // P2.7  -  Unassigned,  Push-Pull,  Digital

    // P3.0  -  Unassigned,  Push-Pull,  Digital
    // P3.1  -  Unassigned,  Push-Pull,  Digital
    // P3.2  -  Unassigned,  Push-Pull,  Digital
    // P3.3  -  Unassigned,  Push-Pull,  Digital
    // P3.4  -  Unassigned,  Open-Drain, Digital
    // P3.5  -  Unassigned,  Open-Drain, Digital
    // P3.6  -  Unassigned,  Open-Drain, Digital
    // P3.7  -  Unassigned,  Open-Drain, Digital

	char SFRPAGE_SAVE = SFRPAGE;     // Save Current SFR page

    SFRPAGE   = CONFIG_PAGE;
    P0MDOUT   = 0x01;
    P1MDOUT   = 0x41;
    P2MDOUT   = 0xFF;
    P3MDOUT   = 0x0F;
    P4MDOUT   = 0xDF;
    P5MDOUT   = 0xFF;
    P6MDOUT   = 0xFF;
    P7MDOUT   = 0xFF;
    XBR0      = 0x27;
    XBR1      = 0x14;
    XBR2      = 0x44;

	SFRPAGE = SFRPAGE_SAVE;       	// Restore SFR page
}

//-----------------------------------------------------------------------------
// EMIF_Init
//-----------------------------------------------------------------------------
//
// Configure the External Memory Interface for both on and off-chip access.
//
void EMIF_Init (void)
{
   char SFRPAGE_SAVE = SFRPAGE;  // Save Current SFR page

   SFRPAGE = LEGACY_PAGE;

   EMI0CF = 0xFB;             // Split-mode, non-multiplexed on P0 - P3

   EMI0TC = EMIF_TIMING;      // This constant may be modified
                              // according to SYSCLK to meet the
                              // timing requirements for the CP2200

   EMI0CN = BASE_ADDRESS;     // Page of XRAM accessed by EMIF
   SFRPAGE = SFRPAGE_SAVE;    // Restore SFR page

}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// This routine initializes the system clock.
//
void SYSCLK_Init (void)
{
    int i = 0;
    char SFRPAGE_SAFE = SFRPAGE;

    SFRPAGE   = CONFIG_PAGE;
    OSCICN    = 0x83;
    CCH0CN    &= ~0x20;
    SFRPAGE   = LEGACY_PAGE;
    FLSCL     = 0xB0;
    SFRPAGE   = CONFIG_PAGE;
    CCH0CN    |= 0x20;
    PLL0CN    |= 0x01;
    PLL0DIV   = 0x01;
    PLL0FLT   = 0x01;
    PLL0MUL   = 0x04;
    for (i = 0; i < 15; i++);  // Wait 5us for initialization
    PLL0CN    |= 0x02;
    while ((PLL0CN & 0x10) == 0);
    CLKSEL    = 0x02;

    SFRPAGE = SFRPAGE_SAFE;
}


//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// This routine initializes Timer 2 to <T2_OVERFLOW_RATE> Hz.
//
void Timer_Init(void)
{  
	char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page
/*
    SFRPAGE   = TIMER01_PAGE;
    TCON      = 0x44;
    TMOD      = 0x21;
    CKCON     = 0x02;
    TH1       = 0x96;
    SFRPAGE   = TMR2_PAGE;
    TMR2CN    = 0x04;
    TMR2CF    = 0x02;
    RCAP2L    = 0x8F;
    RCAP2H    = 0x5E;
    SFRPAGE   = TMR4_PAGE;
    TMR4CN    = 0x04;
    TMR4CF    = 0x08;
    RCAP4L    = 0x82;
    RCAP4H    = 0xFD;

 
    SFRPAGE   = TIMER01_PAGE;
    TCON      = 0x44;
    TMOD      = 0x21;
    CKCON     = 0x02;
    TH1       = 0x96;
    SFRPAGE   = TMR2_PAGE;
    TMR2CN    = 0x04;
    TMR2CF    = 0x00;
    RCAP2L    = 0x8F;
    RCAP2H    = 0x5E;
    SFRPAGE   = TMR4_PAGE;
    TMR4CN    = 0x04;
    TMR4CF    = 0x08;
    RCAP4L    = 0x82;
    RCAP4H    = 0xFD;
*/

	SFRPAGE   = TIMER01_PAGE;
    TCON      = 0x44;
    TMOD      = 0x21;
    CKCON     = 0x02;
    TH1       = 0x96;
    SFRPAGE   = TMR2_PAGE;
    TMR2CN    = 0x04;
    TMR2CF    = 0x08;
    RCAP2L    = 0x82;
    RCAP2H    = 0xFD;
    SFRPAGE   = TMR4_PAGE;
    TMR4CN    = 0x04;
    TMR4CF    = 0x02;
    RCAP4L    = 0x40;
    RCAP4H    = 0x5E;

	SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page
}

void UART_Init()
{
	unsigned char SFRPAGE_Save;

	SFRPAGE_Save = SFRPAGE;		// Save Current SFR page

    SFRPAGE   = UART0_PAGE;
    SCON0     = 0x50;
    SSTA0     = 0x05;
   
   
    SFRPAGE   = UART1_PAGE;
    SCON1     = 0x50;

	SFRPAGE = SFRPAGE_Save;		// Restore SFR page
}

void Interrupts_Init()
{
/*
    IE        = 0xB0; //0xB4;
    EIE1      = 0x02;
//    EIE2      = 0x40;
//	IP        = 0x04; // Int 1 has priority...
	IP        = 0x20; //0x24; // UTimer 2 and Int 1 has priority...
    //EIP1      = 0x02; // SMB0 Has priority.
    //EIP2      = 0x40; // UART1 Has priority.
*/
/*
    IE        = 0xB0;
    IP        = 0x20;
    EIE1      = 0x02;
*/
    IE        = 0x90;
    IP        = 0x00; //0x20;
    EIE1      = 0x02;
    EIE2      = 0x04;
	EIP2	  = 0x40; // Uart1 has priority.
}

void SMBus_Init()
{
	unsigned char SFRPAGE_Save;

    SFRPAGE   = SMB0_PAGE;
    SMB0CN    = 0x40;
    SMB0CR    = 0xB4;

	SFRPAGE = SFRPAGE_Save;		// Restore SFR page
}

void MCUInit(void)
{
	PORT_Init();
	SYSCLK_Init();
	EMIF_Init();
	Timer_Init();
	UART_Init();
	SMBus_Init();
	Interrupts_Init();
}