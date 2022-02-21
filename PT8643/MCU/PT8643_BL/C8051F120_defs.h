//-----------------------------------------------------------------------------
// C8051F120_defs.h
//-----------------------------------------------------------------------------
// Copyright 2008, Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Register/bit definitions for the C8051F12x family.
// **Important Note**: The compiler_defs.h header file should be included
// before including this header file.
//
// Target:         C8051F120, 'F121, 'F122, 'F123, 'F124, 'F125, 'F126, 'F127,
//                 'F130, 'F131, 'F132, 'F133
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.2 - 21 July 2008 (ES)
//    -Added P6 and MAC0STA to bit-addressable registers
// Release 1.1 - 07 AUG 2007 (PKC)
//    -Removed #include <compiler_defs.h>. The C source file should include it.
//    -Corrected preprocessor directive to C8051F120_DEFS_H
// Release 1.0 - 08 DEC 2006 (BW)
//    -Ported from 'F360 DEFS rev 1.1

//-----------------------------------------------------------------------------
// Header File Preprocessor Directive
//-----------------------------------------------------------------------------

#ifndef C8051F120_DEFS_H
#define C8051F120_DEFS_H

//-----------------------------------------------------------------------------
// Byte Registers
//-----------------------------------------------------------------------------

SFR (P0, 0x80);                        // Port 0 Latch
SFR (SP, 0x81);                        // Stack Pointer
SFR (DPL, 0x82);                       // Data Pointer Low
SFR (DPH, 0x83);                       // Data Pointer High
SFR (SFRPAGE, 0x84);                   // SFR Page Select
SFR (SFRNEXT, 0x85);                   // SFR stack next page
SFR (SFRLAST, 0x86);                   // SFR stack last page
SFR (PCON, 0x87);                      // Power Control
SFR (TCON, 0x88);                      // Timer/Counter Control
SFR (CPT0CN, 0x88);                    // Comparator1 Control
SFR (CPT1CN, 0x88);                    // Comparator0 Control
SFR (FLSTAT, 0x88);                    // Flash Status
SFR (TMOD, 0x89);                      // Timer/Counter Mode
SFR (CPT0MD, 0x89);                    // Comparator0 Mode
SFR (CPT1MD, 0x89);                    // Comparator1 Mode
SFR (PLL0CN, 0x89);                    // PLL0 Control
SFR (TL0, 0x8A);                       // Timer/Counter 0 Low
SFR (OSCICN, 0x8A);                    // Internal Oscillator Control
SFR (TL1, 0x8B);                       // Timer/Counter 1 Low
SFR (OSCICL, 0x8B);                    // Internal Oscillator Calibration
SFR (TH0, 0x8C);                       // Timer/Counter 0 High
SFR (OSCXCN, 0x8C);                    // External Oscillator Control
SFR (TH1, 0x8D);                       // Timer/Counter 1 High
SFR (PLL0DIV, 0x8D);                   // PLL0 Divider
SFR (CKCON, 0x8E);                     // Clock Control
SFR (PLL0MUL, 0x8E);                   // PLL0 Multiplier
SFR (PSCTL, 0x8F);                     // Program Store R/W Control
SFR (PLL0FLT, 0x8F);                   // PLL0 Filter
SFR (P1, 0x90);                        // Port 1 Latch
SFR (SSTA0, 0x91);                     // UART0 Status
SFR (MAC0BL, 0x91);                    // MAC0 B Low Byte
SFR (MAC0BH, 0x92);                    // MAC0 B High Byte
SFR (MAC0ACC0, 0x93);                  // MAC0 Accumulator Byte 0
SFR (MAC0ACC1, 0x94);                  // MAC0 Accumulator Byte 1
SFR (MAC0ACC2, 0x95);                  // MAC0 Accumulator Byte 2
SFR (MAC0ACC3, 0x96);                  // MAC0 Accumulator Byte 3
SFR (SFRPGCN, 0x96);                   // SFR Page Control
SFR (CLKSEL, 0x97);                    // System clock select
SFR (MAC0OVR, 0x97);                   // MAC0 Accumulator Overflow
SFR (SCON0, 0x98);                     // UART0 Control
SFR (SCON, 0x98);                      // UART0 Control
SFR (SCON1, 0x98);                     // UART1 Control
SFR (SBUF0, 0x99);                     // UART0 Data Buffer
SFR (SBUF, 0x99);                      // UART0 Data Buffer
SFR (SBUF1, 0x99);                     // UART1 Data Buffer
SFR (SPI0CFG, 0x9A);                   // SPI0 Configuration
SFR (CCH0MA, 0x9A);                    // Cache Miss Accumulator
SFR (SPI0DAT, 0x9B);                   // SPI0 Data
SFR (P4MDOUT, 0x9C);                   // Port 4 Output Mode
SFR (SPI0CKR, 0x9D);                   // SPI0 Clock rate control
SFR (P5MDOUT, 0x9D);                   // Port 5 Output Mode
SFR (P6MDOUT, 0x9E);                   // Port 6 Output Mode
SFR (P7MDOUT, 0x9F);                   // Port 7 Output Mode
SFR (P2, 0xA0);                        // Port 2 Latch
SFR (EMI0TC, 0xA1);                    // EMIF Timing control
SFR (CCH0CN, 0xA1);                    // Cache control
SFR (EMI0CN, 0xA2);                    // EMIF control
SFR (CCH0TN, 0xA2);                    // Cache tuning
SFR (EMI0CF, 0xA3);                    // EMIF configuration
SFR (CCH0LC, 0xA3);                    // Cache lock
SFR (P0MDOUT, 0xA4);                   // Port 0 Output Mode
SFR (P1MDOUT, 0xA5);                   // Port 1 Output Mode
SFR (P2MDOUT, 0xA6);                   // Port 2 Output Mode
SFR (P3MDOUT, 0xA7);                   // Port 3 Output Mode
SFR (IE, 0xA8);                        // Interrupt Enable
SFR (SADDR0, 0xA9);                    // UART0 Slave address
SFR (P1MDIN, 0xAD);                    // Port 1 Analog Input Mode
SFR (P3, 0xB0);                        // Port 3 Latch
SFR (PSBANK, 0xB1);                    // Flash bank select
SFR (FLSCL, 0xB7);                     // Flash scale
SFR (FLACL, 0xB7);                     // Flash access limit
SFR (IP, 0xB8);                        // Interrupt Priority
SFR (SADEN0, 0xB9);                    // UART0 Slave address mask
SFR (AMX0CF, 0xBA);                    // AMUX0 Channel configuration
SFR (AMX2CF, 0xBA);                    // AMUX2 Channel configuration
SFR (AMX0SL, 0xBB);                    // AMUX0 Channel select
SFR (AMX2SL, 0xBB);                    // AMUX2 Channel select
SFR (ADC0CF, 0xBC);                    // ADC0 Configuration
SFR (ADC2CF, 0xBC);                    // ADC2 Configuration
SFR (ADC0L, 0xBE);                     // ADC0 Data Low
SFR (ADC2, 0xBE);                      // ADC2 Data
SFR (ADC0H, 0xBF);                     // ADC0 Data High
SFR (SMB0CN, 0xC0);                    // SMBus0 Control
SFR (MAC0STA, 0xC0);                   // MAC0 Status
SFR (SMB0STA, 0xC1);                   // SMBus0 Status
SFR (MAC0AL, 0xC1);                    // MAC0 A Low Byte
SFR (SMB0DAT, 0xC2);                   // SMBus0 Data
SFR (MAC0AH, 0xC2);                    // MAC0 A High Byte
SFR (SMB0ADR, 0xC3);                   // SMBus0 Slave address
SFR (MAC0CF, 0xC3);                    // MAC0 Configuration
SFR (ADC0GTL, 0xC4);                   // ADC0 Greater-Than Compare Low
SFR (ADC2GT, 0xC4);                    // ADC2 Greater-Than Compare
SFR (ADC0GTH, 0xC5);                   // ADC0 Greater-Than Compare High
SFR (ADC0LTL, 0xC6);                   // ADC0 Less-Than Compare Word Low
SFR (ADC2LT, 0xC6);                    // ADC2 Less-Than Compare Word
SFR (ADC0LTH, 0xC7);                   // ADC0 Less-Than Compare Word High
SFR (TMR2CN, 0xC8);                    // Timer/Counter 2 Control
SFR (TMR3CN, 0xC8);                    // Timer/Counter 3 Control
SFR (TMR4CN, 0xC8);                    // Timer/Counter 4 Control
SFR (P4, 0xC8);                        // Port 4 Latch
SFR (TMR2CF, 0xC9);                    // Timer/Counter 2 Configuration
SFR (TMR3CF, 0xC9);                    // Timer/Counter 3 Configuration
SFR (TMR4CF, 0xC9);                    // Timer/Counter 4 Configuration
SFR (RCAP2L, 0xCA);                    // Timer/Counter 2 Reload Low
SFR (RCAP3L, 0xCA);                    // Timer/Counter 3 Reload Low
SFR (RCAP4L, 0xCA);                    // Timer/Counter 4 Reload Low
SFR (RCAP2H, 0xCB);                    // Timer/Counter 2 Reload High
SFR (RCAP3H, 0xCB);                    // Timer/Counter 3 Reload High
SFR (RCAP4H, 0xCB);                    // Timer/Counter 4 Reload High
SFR (TMR2L, 0xCC);                     // Timer/Counter 2 Low
SFR (TMR3L, 0xCC);                     // Timer/Counter 3 Low
SFR (TMR4L, 0xCC);                     // Timer/Counter 4 Low
SFR (TMR2H, 0xCD);                     // Timer/Counter 2 High
SFR (TMR3H, 0xCD);                     // Timer/Counter 3 High
SFR (TMR4H, 0xCD);                     // Timer/Counter 4 High
SFR (MAC0RNDL, 0xCE);                  // MAC0 Rounding Register Low Byte
SFR (SMB0CR, 0xCF);                    // SMBus0 Clock Rate
SFR (MAC0RNDH, 0xCF);                  // MAC0 Rounding Register High Byte
SFR (PSW, 0xD0);                       // Program Status Word
SFR (REF0CN, 0xD1);                    // Voltage Reference Control
SFR (DAC0L, 0xD2);                     // DAC0 Data Low
SFR (DAC1L, 0xD2);                     // DAC1 Data Low
SFR (DAC0H, 0xD3);                     // DAC0 Data High
SFR (DAC1H, 0xD3);                     // DAC1 Data High
SFR (DAC0CN, 0xD4);                    // DAC0 Control
SFR (DAC1CN, 0xD4);                    // DAC2 Control
SFR (PCA0CN, 0xD8);                    // PCA0 Control
SFR (P5, 0xD8);                        // Port 5 Latch
SFR (PCA0MD, 0xD9);                    // PCA0 Mode
SFR (PCA0CPM0, 0xDA);                  // PCA0 Module 0 Mode Register
SFR (PCA0CPM1, 0xDB);                  // PCA0 Module 1 Mode Register
SFR (PCA0CPM2, 0xDC);                  // PCA0 Module 2 Mode Register
SFR (PCA0CPM3, 0xDD);                  // PCA0 Module 3 Mode Register
SFR (PCA0CPM4, 0xDE);                  // PCA0 Module 4 Mode Register
SFR (PCA0CPM5, 0xDF);                  // PCA0 Module 5 Mode Register
SFR (ACC, 0xE0);                       // Accumulator
SFR (PCA0CPL5, 0xE1);                  // PCA0 Module 5 Capture/Compare Low
SFR (XBR0, 0xE1);                      // Port I/O Crossbar Control 0
SFR (PCA0CPH5, 0xE2);                  // PCA0 Module 5 Capture/Compare High
SFR (XBR1, 0xE2);                      // Port I/O Crossbar Control 1
SFR (XBR2, 0xE3);                      // Port I/O Crossbar Control 2
SFR (EIE1, 0xE6);                      // Extended Interrupt Enable 1
SFR (EIE2, 0xE7);                      // Extended Interrupt Enable 2
SFR (ADC0CN, 0xE8);                    // ADC0 Control
SFR (ADC2CN, 0xE8);                    // ADC2 Control
SFR (P6, 0xE8);                        // Port 6 Latch
SFR (PCA0CPL2, 0xE9);                  // PCA0 Capture 2 Low
SFR (PCA0CPH2, 0xEA);                  // PCA0 Capture 2 High
SFR (PCA0CPL3, 0xEB);                  // PCA0 Capture 3 Low
SFR (PCA0CPH3, 0xEC);                  // PCA0 Capture 3 High
SFR (PCA0CPL4, 0xED);                  // PCA0 Capture 4 Low
SFR (PCA0CPH4, 0xEE);                  // PCA0 Capture 4 High
SFR (RSTSRC, 0xEF);                    // Reset Source Configuration/Status
SFR (B, 0xF0);                         // B Register
SFR (EIP1, 0xF6);                      // External Interrupt Priority 1
SFR (EIP2, 0xF7);                      // External Interrupt Priority 2
SFR (SPI0CN, 0xF8);                    // SPI0 Control
SFR (P7, 0xF8);                        // Port 7 Latch
SFR (PCA0L, 0xF9);                     // PCA0 Counter Low
SFR (PCA0H, 0xFA);                     // PCA0 Counter High
SFR (PCA0CPL0, 0xFB);                  // PCA0 Capture 0 Low
SFR (PCA0CPH0, 0xFC);                  // PCA0 Capture 0 High
SFR (PCA0CPL1, 0xFD);                  // PCA0 Capture 1 Low
SFR (PCA0CPH1, 0xFE);                  // PCA0 Capture 1 High
SFR (WDTCN, 0xFF);                     // Watchdog Timer Control

//-----------------------------------------------------------------------------
// 16-bit Register Definitions (might not be supported by all compilers)
//-----------------------------------------------------------------------------

SFR16 (DP, 0x82);                      // Data Pointer
SFR16 (MAC0B, 0x91);                   // MAC0B data register
SFR16 (MAC0ACCL, 0x93);                // MAC0ACC low registers
SFR16 (MAC0ACCH, 0x95);                // MAC0ACC high registers
SFR16 (ADC0, 0xbe);                    // ADC0 data
SFR16 (MAC0A, 0xc1);                   // MAC0A data register
SFR16 (ADC0GT, 0xc4);                  // ADC0 greater than window
SFR16 (ADC0LT, 0xc6);                  // ADC0 less than window
SFR16 (RCAP2, 0xca);                   // Timer2 capture/reload
SFR16 (RCAP3, 0xca);                   // Timer3 capture/reload
SFR16 (RCAP4, 0xca);                   // Timer4 capture/reload
SFR16 (TMR2, 0xcc);                    // Timer2
SFR16 (TMR3, 0xcc);                    // Timer3
SFR16 (TMR4, 0xcc);                    // Timer4
SFR16 (MAC0RND, 0xce);                 // MAC0RND registers
SFR16 (DAC0, 0xd2);                    // DAC0 data
SFR16 (DAC1, 0xd2);                    // DAC1 data
SFR16 (PCA0CP5, 0xe1);                 // PCA0 Module 5 capture
SFR16 (PCA0CP2, 0xe9);                 // PCA0 Module 2 capture
SFR16 (PCA0CP3, 0xeb);                 // PCA0 Module 3 capture
SFR16 (PCA0CP4, 0xed);                 // PCA0 Module 4 capture
SFR16 (PCA0, 0xf9);                    // PCA0 counter
SFR16 (PCA0CP0, 0xfb);                 // PCA0 Module 0 capture
SFR16 (PCA0CP1, 0xfd);                 // PCA0 Module 1 capture

//-----------------------------------------------------------------------------
// Address Definitions for Bit-addressable Registers
//-----------------------------------------------------------------------------

#define SFR_P0       0x80
#define SFR_TCON     0x88
#define SFR_CPT0CN   0x88
#define SFR_CPT1CN   0x88
#define SFR_FLSTAT   0x88
#define SFR_P1       0x90
#define SFR_SCON0    0x98
#define SFR_SCON     0x98
#define SFR_SCON1    0x98
#define SFR_P2       0xA0
#define SFR_IE       0xA8
#define SFR_P3       0xB0
#define SFR_IP       0xB8
#define SFR_SMB0CN   0xC0
#define SFR_MAC0STA  0xC0
#define SFR_TMR2CN   0xC8
#define SFR_TMR3CN   0xC8
#define SFR_TMR4CN   0xC8
#define SFR_P4       0xC8
#define SFR_PSW      0xD0
#define SFR_PCA0CN   0xD8
#define SFR_P5       0xD8
#define SFR_ACC      0xE0
#define SFR_ADC0CN   0xE8
#define SFR_ADC2CN   0xE8
#define SFR_P6	     0xE8
#define SFR_B        0xF0
#define SFR_SPI0CN   0xF8
#define SFR_P7       0xF8

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

// TCON 0x88
SBIT (TF1, SFR_TCON, 7);               // Timer 1 Overflow Flag
SBIT (TR1, SFR_TCON, 6);               // Timer 1 On/Off Control
SBIT (TF0, SFR_TCON, 5);               // Timer 0 Overflow Flag
SBIT (TR0, SFR_TCON, 4);               // Timer 0 On/Off Control
SBIT (IE1, SFR_TCON, 3);               // Ext. Interrupt 1 Edge Flag
SBIT (IT1, SFR_TCON, 2);               // Ext. Interrupt 1 Type
SBIT (IE0, SFR_TCON, 1);               // Ext. Interrupt 0 Edge Flag
SBIT (IT0, SFR_TCON, 0);               // Ext. Interrupt 0 Type

// CPT0CN  0x88
SBIT (CP0EN, SFR_CPT0CN, 7);           // Comparator 0 Enable
SBIT (CP0OUT, SFR_CPT0CN, 6);          // Comparator 0 Output
SBIT (CP0RIF, SFR_CPT0CN, 5);          // Comparator 0 Rising Edge Interrupt
SBIT (CP0FIF, SFR_CPT0CN, 4);          // Comparator 0 Falling Edge Interrupt
SBIT (CP0HYP1, SFR_CPT0CN, 3);         // Comparator 0 Positive Hysteresis 1
SBIT (CP0HYP0, SFR_CPT0CN, 2);         // Comparator 0 Positive Hysteresis 0
SBIT (CP0HYN1, SFR_CPT0CN, 1);         // Comparator 0 Negative Hysteresis 1
SBIT (CP0HYN0, SFR_CPT0CN, 0);         // Comparator 0 Negative Hysteresis 0

// CPT1CN  0x88
SBIT (CP1EN, SFR_CPT1CN, 7);           // Comparator 1 Enable
SBIT (CP1OUT, SFR_CPT1CN, 6);          // Comparator 1 Output
SBIT (CP1RIF, SFR_CPT1CN, 5);          // Comparator 1 Rising Edge Interrupt
SBIT (CP1FIF, SFR_CPT1CN, 4);          // Comparator 1 Falling Edge Interrupt
SBIT (CP1HYP1, SFR_CPT1CN, 3);         // Comparator 1 Positive Hysteresis 1
SBIT (CP1HYP0, SFR_CPT1CN, 2);         // Comparator 1 Positive Hysteresis 0
SBIT (CP1HYN1, SFR_CPT1CN, 1);         // Comparator 1 Negative Hysteresis 1
SBIT (CP1HYN0, SFR_CPT1CN, 0);         // Comparator 1 Negative Hysteresis 0

// FLSTAT  0x88
SBIT (FLBUSY, SFR_FLSTAT, 0);          // FLASH Busy

// SCON0 0x98
SBIT (SM00, SFR_SCON0, 7);             // UART0 Mode 0
SBIT (SM10, SFR_SCON0, 6);             // UART0 Mode 1
SBIT (SM20, SFR_SCON0, 5);             // UART0 Multiprocessor enable
SBIT (REN0, SFR_SCON0, 4);             // UART0 RX Enable
SBIT (REN, SFR_SCON0, 4);              // UART0 RX Enable
SBIT (TB80, SFR_SCON0, 3);             // UART0 TX Bit 8
SBIT (TB8, SFR_SCON0, 3);              // UART0 TX Bit 8
SBIT (RB80, SFR_SCON0, 2);             // UART0 RX Bit 8
SBIT (RB8, SFR_SCON0, 2);              // UART0 RX Bit 8
SBIT (TI0, SFR_SCON0, 1);              // UART0 TX Interrupt Flag
SBIT (TI, SFR_SCON0, 1);               // UART0 TX Interrupt Flag
SBIT (RI0, SFR_SCON0, 0);              // UART0 RX Interrupt Flag
SBIT (RI, SFR_SCON0, 0);               // UART0 RX Interrupt Flag

// SCON1 0x98
SBIT (S1MODE, SFR_SCON1, 7);           // UART1 Mode
                                       // Bit6 UNUSED
SBIT (MCE1, SFR_SCON1, 5);             // UART1 MCE
SBIT (REN1, SFR_SCON1, 4);             // UART1 RX Enable
SBIT (TB81, SFR_SCON1, 3);             // UART1 TX Bit 8
SBIT (RB81, SFR_SCON1, 2);             // UART1 RX Bit 8
SBIT (TI1, SFR_SCON1, 1);              // UART1 TX Interrupt Flag
SBIT (RI1, SFR_SCON1, 0);              // UART1 RX Interrupt Flag


// IE 0xA8
SBIT (EA, SFR_IE, 7);                  // Global Interrupt Enable
                                       // Bit 6 unused
SBIT (ET2, SFR_IE, 5);                 // Timer 2 Interrupt Enable
SBIT (ES0, SFR_IE, 4);                 // UART0 Interrupt Enable
SBIT (ET1, SFR_IE, 3);                 // Timer 1 Interrupt Enable
SBIT (EX1, SFR_IE, 2);                 // External Interrupt 1 Enable
SBIT (ET0, SFR_IE, 1);                 // Timer 0 Interrupt Enable
SBIT (EX0, SFR_IE, 0);                 // External Interrupt 0 Enable

// IP 0xB8
                                       // Bit 7 unused
                                       // Bit 6 unused
SBIT (PT2, SFR_IP, 5);                 // Timer 2 Priority
SBIT (PS0, SFR_IP, 4);                 // UART0 Priority
SBIT (PS, SFR_IP, 4);                  // UART0 Priority
SBIT (PT1, SFR_IP, 3);                 // Timer 1 Priority
SBIT (PX1, SFR_IP, 2);                 // External Interrupt 1 Priority
SBIT (PT0, SFR_IP, 1);                 // Timer 0 Priority
SBIT (PX0, SFR_IP, 0);                 // External Interrupt 0 Priority

// SMB0CN 0xC0
SBIT (BUSY, SFR_SMB0CN, 7);            // SMBus0 Busy
SBIT (ENSMB, SFR_SMB0CN, 6);           // SMBus0 Enable
SBIT (STA, SFR_SMB0CN, 5);             // SMBus0 Start Flag
SBIT (STO, SFR_SMB0CN, 4);             // SMBus0 Stop Flag
SBIT (SI, SFR_SMB0CN, 3);              // SMBus0 Interrupt pending
SBIT (AA, SFR_SMB0CN, 2);              // SMBus0 Assert/Ack Flag
SBIT (SMBFTE, SFR_SMB0CN, 1);          // SMBus0 Bus free timeout enable
SBIT (SMBTOE, SFR_SMB0CN, 0);          // SMBus0 SCL low timeout enable

// TMR2CN 0xC8
SBIT (TF2, SFR_TMR2CN, 7);             // Timer2 Overflow
SBIT (EXF2, SFR_TMR2CN, 6);            // Timer2 External
                                       // Bit 5 unused
                                       // Bit 4 unused
SBIT (EXEN2, SFR_TMR2CN, 3);           // Timer2 External Enable
SBIT (TR2, SFR_TMR2CN, 2);             // Timer2 Run Enable
SBIT (CT2, SFR_TMR2CN, 1);             // Timer2 Counter select
SBIT (CPRL2, SFR_TMR2CN, 0);           // Timer2 Capture select

// TMR3CN 0xC8
SBIT (TF3, SFR_TMR3CN, 7);             // Timer3 Overflow
SBIT (EXF3, SFR_TMR3CN, 6);            // Timer3 External
                                       // Bit 5 unused
                                       // Bit 4 unused
SBIT (EXEN3, SFR_TMR3CN, 3);           // Timer3 External Enable
SBIT (TR3, SFR_TMR3CN, 2);             // Timer3 Run Enable
SBIT (CT3, SFR_TMR3CN, 1);             // Timer3 Counter select
SBIT (CPRL3, SFR_TMR3CN, 0);           // Timer3 Capture select

// TMR4CN 0xC8
SBIT (TF4, SFR_TMR4CN, 7);             // Timer4 Overflow
SBIT (EXF4, SFR_TMR4CN, 6);            // Timer4 External
                                       // Bit 5 unused
                                       // Bit 4 unused
SBIT (EXEN4, SFR_TMR4CN, 3);           // Timer4 External Enable
SBIT (TR4, SFR_TMR4CN, 2);             // Timer4 Run Enable
SBIT (CT4, SFR_TMR4CN, 1);             // Timer4 Counter select
SBIT (CPRL4, SFR_TMR4CN, 0);           // Timer4 Capture select

// PSW 0xD0
SBIT (CY, SFR_PSW, 7);                 // Carry Flag
SBIT (AC, SFR_PSW, 6);                 // Auxiliary Carry Flag
SBIT (F0, SFR_PSW, 5);                 // User Flag 0
SBIT (RS1, SFR_PSW, 4);                // Register Bank Select 1
SBIT (RS0, SFR_PSW, 3);                // Register Bank Select 0
SBIT (OV, SFR_PSW, 2);                 // Overflow Flag
SBIT (F1, SFR_PSW, 1);                 // User Flag 1
SBIT (P, SFR_PSW, 0);                  // Accumulator Parity Flag

// PCA0CN 0xD8
SBIT (CF, SFR_PCA0CN, 7);              // PCA0 Counter Overflow Flag
SBIT (CR, SFR_PCA0CN, 6);              // PCA0 Counter Run Control Bit
SBIT (CCF5, SFR_PCA0CN, 5);            // PCA0 Module 5 Interrupt Flag
SBIT (CCF4, SFR_PCA0CN, 4);            // PCA0 Module 4 Interrupt Flag
SBIT (CCF3, SFR_PCA0CN, 3);            // PCA0 Module 3 Interrupt Flag
SBIT (CCF2, SFR_PCA0CN, 2);            // PCA0 Module 2 Interrupt Flag
SBIT (CCF1, SFR_PCA0CN, 1);            // PCA0 Module 1 Interrupt Flag
SBIT (CCF0, SFR_PCA0CN, 0);            // PCA0 Module 0 Interrupt Flag

// ADC0CN 0xE8
SBIT (AD0EN, SFR_ADC0CN, 7);           // ADC0 Enable
SBIT (AD0TM, SFR_ADC0CN, 6);           // ADC0 Track Mode
SBIT (AD0INT, SFR_ADC0CN, 5);          // ADC0 EOC Interrupt Flag
SBIT (AD0BUSY, SFR_ADC0CN, 4);         // ADC0 Busy Flag
SBIT (AD0CM1, SFR_ADC0CN, 3);          // ADC0 Convert Start Mode Bit 1
SBIT (AD0CM0, SFR_ADC0CN, 2);          // ADC0 Convert Start Mode Bit 0
SBIT (AD0WINT, SFR_ADC0CN, 1);         // ADC0 Window Interrupt Flag
SBIT (AD0LJST, SFR_ADC0CN, 0);         // ADC0 Left Justify

// ADC2CN 0xE8
SBIT (AD2EN, SFR_ADC2CN, 7);           // ADC2 Enable
SBIT (AD2TM, SFR_ADC2CN, 6);           // ADC2 Track Mode
SBIT (AD2INT, SFR_ADC2CN, 5);          // ADC2 EOC Interrupt Flag
SBIT (AD2BUSY, SFR_ADC2CN, 4);         // ADC2 Busy Flag
SBIT (AD2CM2, SFR_ADC2CN, 3);          // ADC2 Convert Start Mode Bit 2
SBIT (AD2CM1, SFR_ADC2CN, 2);          // ADC2 Convert Start Mode Bit 1
SBIT (AD2CM0, SFR_ADC2CN, 1);          // ADC2 Convert Start Mode Bit 0
SBIT (AD2WINT, SFR_ADC2CN, 0);         // ADC2 Window Interrupt Flag

// SPI0CN 0xF8
SBIT (SPIF, SFR_SPI0CN, 7);            // SPI0 Interrupt Flag
SBIT (WCOL, SFR_SPI0CN, 6);            // SPI0 Write Collision Flag
SBIT (MODF, SFR_SPI0CN, 5);            // SPI0 Mode Fault Flag
SBIT (RXOVRN, SFR_SPI0CN, 4);          // SPI0 RX Overrun Flag
SBIT (NSSMD1, SFR_SPI0CN, 3);          // SPI0 Slave Select Mode 1
SBIT (NSSMD0, SFR_SPI0CN, 2);          // SPI0 Slave Select Mode 0
SBIT (TXBMT, SFR_SPI0CN, 1);           // SPI0 TX Buffer Empty Flag
SBIT (SPIEN, SFR_SPI0CN, 0);           // SPI0 Enable

//-----------------------------------------------------------------------------
// Interrupt Priorities
//-----------------------------------------------------------------------------

#define INTERRUPT_INT0             0   // External Interrupt 0
#define INTERRUPT_TIMER0           1   // Timer0 Overflow
#define INTERRUPT_INT1             2   // External Interrupt 1
#define INTERRUPT_TIMER1           3   // Timer1 Overflow
#define INTERRUPT_UART0            4   // UART0
#define INTERRUPT_TIMER2           5   // Timer2 Overflow
#define INTERRUPT_SPI0             6   // SPI0
#define INTERRUPT_SMBUS0           7   // SMBus0 Interface
#define INTERRUPT_ADC0_WINDOW      8   // ADC0 Window Comparison
#define INTERRUPT_PCA0             9   // PCA0 Peripheral
#define INTERRUPT_COMPARATOR0F     10  // Comparator0 Falling
#define INTERRUPT_COMPARATOR0R     11  // Comparator0 Rising
#define INTERRUPT_COMPARATOR1F     12  // Comparator1 Falling
#define INTERRUPT_COMPARATOR1R     13  // Comparator1 Rising
#define INTERRUPT_TIMER3           14  // Timer3 Overflow
#define INTERRUPT_ADC0_EOC         15  // ADC0 End Of Conversion
#define INTERRUPT_TIMER4           16  // Timer4 Overflow
#define INTERRUPT_ADC2_WINDOW      17  // ADC2 Window Comparison
#define INTERRUPT_ADC2_EOC         18  // ADC2 End Of Conversion
                                       // 19 - RESERVED
#define INTERRUPT_UART1            20  // UART1

//-----------------------------------------------------------------------------
// SFR Page Definitions
//-----------------------------------------------------------------------------

#define  CONFIG_PAGE       0x0F        // SYSTEM AND PORT CONFIGURATION PAGE
#define  LEGACY_PAGE       0x00        // LEGACY SFR PAGE
#define  TIMER01_PAGE      0x00        // TIMER0 AND TIMER1
#define  CPT0_PAGE         0x01        // COMPARATOR0
#define  CPT1_PAGE         0x02        // COMPARATOR1
#define  UART0_PAGE        0x00        // UART0
#define  UART1_PAGE        0x01        // UART1
#define  SPI0_PAGE         0x00        // SPI0
#define  EMI0_PAGE         0x00        // EMIF
#define  ADC0_PAGE         0x00        // ADC0
#define  ADC2_PAGE         0x02        // ADC2
#define  MAC0_PAGE         0x03        // MAC
#define  SMB0_PAGE         0x00        // SMBUS0
#define  TMR2_PAGE         0x00        // TIMER2
#define  TMR3_PAGE         0x01        // TIMER3
#define  TMR4_PAGE         0x02        // TIMER4
#define  DAC0_PAGE         0x00        // DAC0
#define  DAC1_PAGE         0x01        // DAC1
#define  PCA0_PAGE         0x00        // PCA0
#define  REF0_PAGE         0x00        // VREF0
#define  PLL0_PAGE         0x0F        // PLL0

//-----------------------------------------------------------------------------
// Header File PreProcessor Directive
//-----------------------------------------------------------------------------

#endif                                 // #define C8051F120_DEFS_H

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------