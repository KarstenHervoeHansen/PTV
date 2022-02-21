/***************************************************************************/
/*  Copyright DK-Audio A/S, 2005                                           */
/*  Project:  PT8641 HDTPG (PT8612), using 8051F320 Cygnal                 */
/*            processor.                                                   */
/*  Module:   ini.C                                                        */
/*  Author:   Jnausz Kuzminski                                             */
/*  Date:     01.03.2006                                                   */
/*  Status:   Version 1.0                                                  */
/*  Purpose:  This is initialization routine obtained from Cygnal config   */
/*            wizard version 2.04 for F320 processor.                      */
/*  Change:   19.04.2006: P0.7 used as LEVEL OK SIGNAL TO MASTER           */
/***************************************************************************/
#include <C8051F320.h>  // Register definition file.

void config (void)
  {
  int n = 0;

  PCA0MD &= ~0x40;     //disable watchdog timer

//----------------------------------------------------------------
// CROSSBAR REGISTER CONFIGURATION
//
// NOTE: The crossbar register should be configured before any  
// of the digital peripherals are enabled. The pinout of the 
// device is dependent on the crossbar configuration so caution 
// must be exercised when modifying the contents of the XBR0, 
// XBR1 registers. For detailed information on 
// Crossbar Decoder Configuration, refer to Application Note 
// AN001, "Configuring the Port I/O Crossbar Decoder". 
//----------------------------------------------------------------

// Configure the XBRn Registers

  XBR0 = 0x01;    // Crossbar Register 1
  //XBR1 = 0xC0;  // Weak puillup disabled
  XBR1 = 0x40;    // Weak puillup enabled: needed for JTAG programmer
// Select Pin I/0

// NOTE: Some peripheral I/O pins can function as either inputs or 
// outputs, depending on the configuration of the peripheral. By default,
// the configuration utility will configure these I/O pins as push-pull 
// outputs.
                      // Port configuration (1 = Push Pull Output)
    P0MDOUT = 0xC3; // Output configuration for P0 (bit 7 as LEVOK)
    P1MDOUT = 0x87; // Output configuration for P1 
    P3MDOUT = 0x00; // Output configuration for P3 

    P0MDIN = 0xF3;  // Input configuration for P0
    P1MDIN = 0xFF;  // Input configuration for P1
    P3MDIN = 0xFF;  // Input configuration for P3

    P0SKIP = 0x0C;  //  Port 0 Crossbar Skip Register
    P1SKIP = 0x00;  //  Port 1 Crossbar Skip Register

    P2MDOUT = 0x00; // Output configuration for P2 
    P2MDIN  = 0xFF;  // Input configuration for P2
    P2SKIP  = 0x00;  //  Port 2 Crossbar Skip Register
//----------------------------------------------------------------
// Oscillator Configuration
//----------------------------------------------------------------

  OSCXCN = 0x67;  // EXTERNAL Oscillator Control Register 
#if 1
  for (n = 0; n < 255; n++)
    ;
#endif                              // wait for osc to start
  while ( (OSCXCN & 0x80) == 0 )
    ;                              // wait for xtal to stabilize

  CLKSEL = 0x01;                   // Oscillator Clock Select Register
  OSCICN = 0x03;                   // Internal Oscillator Control Register

//----------------------------------------------------------------
// UART0 Configuration
//----------------------------------------------------------------
    SCON0 = 0xF0;        // Serial Port Control Register

    PCON   = 0x00;       // Power Control Register

//----------------------------------------------------------------
// Timers Configuration
//----------------------------------------------------------------

    CKCON = 0x0C;     // Clock Control Register    
    TL0   = 0x00;     // Timer 0 Low Byte
    TH0   = 0x00;     // Timer 0 High Byte
    TL1   = 0xB8;     // Timer 1 Low Byte
    TH1   = 0xB8;     // Timer 1 High Byte    
    TMOD  = 0x20;     // Timer Mode Register
    TCON  = 0x40;     // Timer Control Register 
    
    TMR2RLL = 0x00;   // Timer 2 Reload Register Low Byte
    TMR2RLH = 0x00;   // Timer 2 Reload Register High Byte
    TMR2L   = 0x00;   // Timer 2 Low Byte
    TMR2H   = 0x00;   // Timer 2 High Byte    
    TMR2CN  = 0x00;   // Timer 2 Control Register

    TMR3RLL = 0x00;   // Timer 3 Reload Register Low Byte
    TMR3RLH = 0x00;   // Timer 3 Reload Register High Byte
    TMR3L   = 0x00;   // Timer 3 Low Byte
    TMR3H   = 0x40;   // Timer 3 High Byte    
    TMR3CN  = 0x00;   // Timer 3 Control Register


/* Enable VDD Monitor */
    VDM0CN  |= 0x80;

//----------------------------------------------------------------
// Reset Source Configuration
// NOTE! : Comparator 0 must be enabled before it is enabled as a 
// reset source.
//------------------------------------------------------------------
  RSTSRC = 0x00;  // Reset Source Register

  TR1 = 1;       //start timer1
  RI0 = 0;       //clear receive interrupt bit
  TI0 = 0;       //clear transmit interrupt bit

  ES0 = 1;       /* Enable serial interrupt   */
  EA  = 1;
  }              //End of config
