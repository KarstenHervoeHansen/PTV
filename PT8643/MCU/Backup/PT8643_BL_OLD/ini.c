/////////////////////////////////////
//  Generated Initialization File  //
/////////////////////////////////////

#include "C8051F120.h"

// Peripheral specific initialization functions,
// Called from the Init_Device() function
void Timer_Init()
{
    SFRPAGE   = TIMER01_PAGE;
    TCON      = 0x40;
    TMOD      = 0x20;
    CKCON     = 0x02;
    TH1       = 0x96;
    SFRPAGE   = TMR2_PAGE;
    TMR2CN    = 0x04;
    TMR2CF    = 0x02;
    RCAP2L    = 0x7F;
    RCAP2H    = 0x60;
    SFRPAGE   = TMR4_PAGE;
    TMR4CN    = 0x04;
    TMR4CF    = 0x08;
    RCAP4L    = 0xCB;
    RCAP4H    = 0xFF;
}

void UART_Init()
{
    SFRPAGE   = UART0_PAGE;
    SCON0     = 0x50;
    SSTA0     = 0x0F;
}

void SMBus_Init()
{
    SFRPAGE   = SMB0_PAGE;
    SMB0CN    = 0x40;
    SMB0CR    = 0xB4;
}

void Port_IO_Init()
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
    // P1.2  -  Unassigned,  Open-Drain, Digital
    // P1.3  -  Unassigned,  Open-Drain, Digital
    // P1.4  -  Unassigned,  Open-Drain, Digital
    // P1.5  -  Unassigned,  Open-Drain, Digital
    // P1.6  -  Unassigned,  Open-Drain, Digital
    // P1.7  -  Unassigned,  Open-Drain, Digital

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
    // P3.2  -  Unassigned,  Open-Drain, Digital
    // P3.3  -  Unassigned,  Open-Drain, Digital
    // P3.4  -  Unassigned,  Open-Drain, Digital
    // P3.5  -  Unassigned,  Open-Drain, Digital
    // P3.6  -  Unassigned,  Open-Drain, Digital
    // P3.7  -  Unassigned,  Open-Drain, Digital
/*
    SFRPAGE   = CONFIG_PAGE;
    P0MDOUT   = 0x01;
    P1MDOUT   = 0x01;
    P2MDOUT   = 0xFF;
    P3MDOUT   = 0x03;
    XBR0      = 0x07;
    XBR2      = 0x44;
*/
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

void Oscillator_Init()
{
    int i = 0;
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
}

void Interrupts_Init()
{
    IE        = 0xB0;
    EIE1      = 0x02;
    EIP1      = 0x02;
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

   EMI0TC = 0xFF;		      // This constant may be modified
                              // according to SYSCLK to meet the
                              // timing requirements for the CP2200

   EMI0CN = 0x20;		      // Page of XRAM accessed by EMIF
   SFRPAGE = SFRPAGE_SAVE;    // Restore SFR page

}

// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
	EMIF_Init();
    Timer_Init();
    UART_Init();
    SMBus_Init();
    Port_IO_Init();
    Oscillator_Init();
    Interrupts_Init();
}
