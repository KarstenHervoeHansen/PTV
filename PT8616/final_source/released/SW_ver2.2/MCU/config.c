/////////////////////////////////////
//  Generated Initialization File  //
/////////////////////////////////////

#include "C8051F320.h"

// Peripheral specific initialization functions,
// Called from the Init_Device() function
void PCA_Init()
{
    PCA0MD    &= ~0x40;
    PCA0MD    = 0x00;
}

void Timer_Init()
{
    TCON      = 0x45;
    TMOD      = 0x20;
    TH1       = 0x98;
    TMR3CN    = 0x04;
    TMR3RLL   = 0xAF;
    TMR3RLH   = 0x3C;
}

void UART_Init()
{
    SCON0     = 0x10;
}

void SMBus_Init()
{
    SMB0CF    = 0x89;
}

void Port_IO_Init()
{
    // P0.0  -  SDA (SMBus), Open-Drain, Digital
    // P0.1  -  SCL (SMBus), Open-Drain, Digital
    // P0.2  -  Skipped,     Open-Drain, Analog
    // P0.3  -  Skipped,     Open-Drain, Analog
    // P0.4  -  TX0 (UART0), Open-Drain, Digital
    // P0.5  -  RX0 (UART0), Open-Drain, Digital
    // P0.6  -  Unassigned,  Open-Drain, Digital
    // P0.7  -  Unassigned,  Open-Drain, Digital

    // P1.0  -  Unassigned,  Push-Pull,  Digital
    // P1.1  -  Unassigned,  Open-Drain, Digital
    // P1.2  -  Unassigned,  Open-Drain, Digital
    // P1.3  -  Unassigned,  Push-Pull,  Digital
    // P1.4  -  Unassigned,  Push-Pull,  Digital
    // P1.5  -  Unassigned,  Push-Pull,  Digital
    // P1.6  -  Unassigned,  Push-Pull,  Digital
    // P1.7  -  Unassigned,  Push-Pull,  Digital
    // P2.0  -  Unassigned,  Push-Pull,  Digital
    // P2.1  -  Unassigned,  Push-Pull,  Digital
    // P2.2  -  Unassigned,  Push-Pull,  Digital
    // P2.3  -  Unassigned,  Push-Pull,  Digital

    P0MDIN    = 0xF3;
    P1MDOUT   = 0xF9;
    P2MDOUT   = 0xBF;
    P0SKIP    = 0x0C;
    XBR0      = 0x05;
    XBR1      = 0x40;
}

void Oscillator_Init()
{
    int i = 0;
    CLKMUL    = 0x80;
    for (i = 0; i < 20; i++);    // Wait 5us for initialization
    CLKMUL    |= 0xC0;
    while ((CLKMUL & 0x20) == 0);
    CLKSEL    = 0x02;
    OSCICN    = 0x83;
}

void Interrupts_Init()
{
    EIE1      = 0x01;
    IT01CF    = 0x7F;
    IE        = 0x95;
}

// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
    PCA_Init();
    Timer_Init();
    UART_Init();
    SMBus_Init();
    Port_IO_Init();
    Oscillator_Init();
    Interrupts_Init();
}
