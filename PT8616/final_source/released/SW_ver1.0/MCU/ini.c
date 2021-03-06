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
/*            UART uses Timer0 for Baud Rate.                              */
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

  XBR0 = 0x05;  // Crossbar Register 1
  XBR1 = 0x40;  // Crossbar Register 2 (weak pull-ups globally disabled)
                // (weak pull-ups globally enabled = 0x40)
/* We must have weak pull-ups enabled, otherwise if there is no */
/* load on input port, reading from it may give changing level. */

// Select Pin I/0

// NOTE: Some peripheral I/O pins can function as either inputs or 
// outputs, depending on the configuration of the peripheral. By default,
// the configuration utility will configure these I/O pins as push-pull 
// outputs.
                      // Port configuration (1 = Push Pull Output)
    P0MDOUT = 0x00; // Output configuration for P0 (bit 7 as LEVOK)
    P1MDOUT = 0xF9; // Output configuration for P1 
	P2MDOUT = 0xFF;
    P3MDOUT = 0x00; // Output configuration for P3 

    P0MDIN = 0xF3;  // Input configuration for P0 //F8
    P1MDIN = 0xFF;  // Input configuration for P1
    P3MDIN = 0xFF;  // Input configuration for P3

    P0SKIP = 0x0C;  //  Port 0 Crossbar Skip Register
    P1SKIP = 0x00;  //  Port 1 Crossbar Skip Register


/* New values for JTAG signals from P2 */
  P2MDOUT = 0xFF;   /* Output configuration for P2 (0x0C)           */
  P2MDIN  = 0xFF;   /* Input configuration for P2                   */
  P2SKIP  = 0x00;   /* Port 2 Crossbar Skip Register                */


//----------------------------------------------------------------
// Comparator Register Configuration
//----------------------------------------------------------------
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

  CLKSEL = 0x01;   // = External Oscillator
  //OSCICN = 0x03;   // Internal Oscillator Control Register

//----------------------------------------------------------------
// SPI Configuration
//----------------------------------------------------------------
//----------------------------------------------------------------
// Reference Control Register Configuration
//----------------------------------------------------------------
//----------------------------------------------------------------
// ADC Configuration
//----------------------------------------------------------------
//----------------------------------------------------------------
// UART0 Configuration
//----------------------------------------------------------------
    SCON0 = 0x30; //0x98; //0x30;        // Serial Port Control Register

    PCON  = 0x00;       // Power Control Register

//----------------------------------------------------------------
// SMBus Configuration
//----------------------------------------------------------------

	
    SMB0CF = 0x89;  // SMBus Configuration Register //98
    SMB0DAT = 0x00; // SMBus Data Register
    SMB0CN = 0x00;  // SMBus Control Register


	TMR3RLH=0xA6; TMR3RLL=0x00;	//Timer3 reload => 25 ms overflow
	TMR3H=TMR3RLH; TMR3L=TMR3RLL;
	TMR3CN=0x04;				//16 bit autoreload, sysclk/12, timer enabled
	
//----------------------------------------------------------------
// PCA Configuration
//----------------------------------------------------------------

//----------------------------------------------------------------
// Reset Source Configuration
// NOTE! : Comparator 0 must be enabled before it is enabled as a 
// reset source.
//------------------------------------------------------------------
  RSTSRC = 0x00;  // Reset Source Register

  TR1 = 1;       //start timer1
  RI0 = 0;       //clear receive interrupt bit
  TI0 = 0;       //clear transmit interrupt bit

//----------------------------------------------------------------
// Interrupt Configuration
//----------------------------------------------------------------

    IE = 0x05;         //Interrupt Enable
    IP = 0x00;         //Interrupt Priority
    EIE1 = 0x80;       //EXtended Interrupt Enable
//    EIP1 = 0x00;       //EXtended Interrupt Priority

    IT01CF = 0x7F;     //INT0/INT1 Configuration Register
	TCON|=5;	       //Set Ext0+1 int = edge trigger

  }              //End of config
