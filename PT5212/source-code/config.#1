//---------------------------------------------------------------
// CYGNAL Integrated Products 
//
// C Code Configuration Tool: F320 INITIALIZATION/CONFIGURATION CODE
//----------------------------------------------------------------
// This file is read only. To insert the code into your  
// application, simply cut and paste or use the "Save As" 
// command in the file menu to save the file in your project 
// directory. 
//----------------------------------------------------------------

//----------------------------------------------------------------
// INCLUDES
//----------------------------------------------------------------

#include <C8051F320.h>	// Register definition file.

//------------------------------------------------------------------------------------
// Global CONSTANTS
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Function PROTOTYPES
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Config Routine
//------------------------------------------------------------------------------------
void config (void) {

//Local Variable Definitions

	

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

	XBR0 = 0x00;	// Crossbar Register 1
	XBR1 = 0x00;	// Crossbar Register 2
// Select Pin I/0

// NOTE: Some peripheral I/O pins can function as either inputs or 
// outputs, depending on the configuration of the peripheral. By default,
// the configuration utility will configure these I/O pins as push-pull 
// outputs.
                      // Port configuration (1 = Push Pull Output)
    P0MDOUT = 0x00; // Output configuration for P0 
    P1MDOUT = 0x00; // Output configuration for P1 
    P2MDOUT = 0x00; // Output configuration for P2 
    P3MDOUT = 0x00; // Output configuration for P3 

    P0MDIN = 0xFF;  // Input configuration for P0
    P1MDIN = 0xFF;  // Input configuration for P1
    P2MDIN = 0xFF;  // Input configuration for P2
    P3MDIN = 0xFF;  // Input configuration for P3

    P0SKIP = 0x00;  //  Port 0 Crossbar Skip Register
    P1SKIP = 0x00;  //  Port 1 Crossbar Skip Register
    P2SKIP = 0x00;  //  Port 2 Crossbar Skip Register

// View port pinout

		// The current Crossbar configuration results in the 
		// following port pinout assignment:
		// Port 0
		// P0.0 = unassigned      (Open-Drain Output/Input)(Digital)
		// P0.1 = unassigned      (Open-Drain Output/Input)(Digital)
		// P0.2 = unassigned      (Open-Drain Output/Input)(Digital)
		// P0.3 = unassigned      (Open-Drain Output/Input)(Digital)
		// P0.4 = unassigned      (Open-Drain Output/Input)(Digital)
		// P0.5 = unassigned      (Open-Drain Output/Input)(Digital)
		// P0.6 = unassigned      (Open-Drain Output/Input)(Digital)
		// P0.7 = unassigned      (Open-Drain Output/Input)(Digital)

        // Port 1
		// P1.0 = unassigned      (Open-Drain Output/Input)(Digital)
		// P1.1 = unassigned      (Open-Drain Output/Input)(Digital)
		// P1.2 = unassigned      (Open-Drain Output/Input)(Digital)
		// P1.3 = unassigned      (Open-Drain Output/Input)(Digital)
		// P1.4 = unassigned      (Open-Drain Output/Input)(Digital)
		// P1.5 = unassigned      (Open-Drain Output/Input)(Digital)
		// P1.6 = unassigned      (Open-Drain Output/Input)(Digital)
		// P1.7 = unassigned      (Open-Drain Output/Input)(Digital)

        // Port 2
		// P2.0 = unassigned      (Open-Drain Output/Input)(Digital)
		// P2.1 = unassigned      (Open-Drain Output/Input)(Digital)
		// P2.2 = unassigned      (Open-Drain Output/Input)(Digital)
		// P2.3 = unassigned      (Open-Drain Output/Input)(Digital)
		// P2.4 = unassigned      (Open-Drain Output/Input)(Digital)
		// P2.5 = unassigned      (Open-Drain Output/Input)(Digital)
		// P2.6 = unassigned      (Open-Drain Output/Input)(Digital)
		// P2.7 = unassigned      (Open-Drain Output/Input)(Digital)

        // Port 3
		// P3.0 = unassigned      (Open-Drain Output/Input)(Digital)

//----------------------------------------------------------------
// Comparator Register Configuration
//----------------------------------------------------------------

    CPT0MX = 0x00;   // Comparator 0 MUX Selection Register
    CPT0MD = 0x02;   // Comparator 0 Mode Selection Register
    CPT0CN = 0x00;   // Comparator 0 Control Register

    CPT1MX = 0x00;   // Comparator 1 MUX Selection Register
    CPT1MD = 0x02;   // Comparator 1 Mode Selection Register
    CPT1CN = 0x00;   // Comparator 1 Control Register
	
//----------------------------------------------------------------
// Oscillator Configuration
//----------------------------------------------------------------

	OSCXCN = 0x00;	// EXTERNAL Oscillator Control Register	
    CLKSEL = 0x00;  // Oscillator Clock Select Register
	OSCICN = 0x80;	// Internal Oscillator Control Register

//----------------------------------------------------------------
// SPI Configuration
//----------------------------------------------------------------

	SPI0CFG = 0x00;	// SPI Configuration Register
	SPI0CKR = 0x00;	// SPI Clock Rate Register
    SPI0CN = 0x00;	// SPI Control Register


//----------------------------------------------------------------
// Reference Control Register Configuration
//----------------------------------------------------------------

	REF0CN = 0x00;	// Reference Control Register

//----------------------------------------------------------------
// ADC Configuration
//----------------------------------------------------------------

    AMX0P = 0x00;	// AMX0 Positive Select Register
	AMX0N = 0x00;	// AMX0 Negative Select Register
	ADC0CF = 0xF8;	// ADC Configuration Register
	ADC0CN = 0x00;	// ADC Control Register
	
    ADC0H = 0x00;   // ADC Data MSB
    ADC0L = 0x00;   // ADC Data LSB
	ADC0LTH = 0x00;	// ADC Less-Than High Byte Register
	ADC0LTL = 0x00;	// ADC Less-Than Low Byte Register
	ADC0GTH = 0xFF;	// ADC Greater-Than High Byte Register
	ADC0GTL = 0xFF;	// ADC Greater-Than Low Byte Register

//----------------------------------------------------------------
// UART0 Configuration
//----------------------------------------------------------------
    
    SCON0 = 0x00;       // Serial Port Control Register

    PCON = 0x00;        // Power Control Register

//----------------------------------------------------------------
// SMBus Configuration
//----------------------------------------------------------------
	
    SMB0CF = 0x00;  // SMBus Configuration Register
    SMB0DAT = 0x00; // SMBus Data Register
    SMB0CN = 0x00;  // SMBus Control Register

//----------------------------------------------------------------
// PCA Configuration
//----------------------------------------------------------------

    PCA0MD = 0x00;      // PCA Mode Register
    PCA0L = 0x00;       // PCA Counter/Timer Low Byte
    PCA0H = 0x00;       // PCA Counter/Timer High Byte	    
    PCA0CN = 0x00;      // PCA Control Register
	

    //Module 0
    PCA0CPM0 = 0x00;    // PCA Capture/Compare Register 0
    PCA0CPL0 = 0x00;    // PCA Counter/Timer Low Byte
    PCA0CPH0 = 0x00;    // PCA Counter/Timer High Byte

    //Module 1
    PCA0CPM1 = 0x00;    // PCA Capture/Compare Register 1
    PCA0CPL1 = 0x00;    // PCA Counter/Timer Low Byte
    PCA0CPH1 = 0x00;    // PCA Counter/Timer High Byte

    //Module 2
    PCA0CPM2 = 0x00;    // PCA Capture/Compare Register 2
    PCA0CPL2 = 0x00;    // PCA Counter/Timer Low Byte
    PCA0CPH2 = 0x00;    // PCA Counter/Timer High Byte

    //Module 3
    PCA0CPM3 = 0x00;    // PCA Capture/Compare Register 3
    PCA0CPL3 = 0x00;    // PCA Counter/Timer Low Byte
    PCA0CPH3 = 0x00;    // PCA Counter/Timer High Byte

    //Module 4
    PCA0CPM4 = 0x00;    // PCA Capture/Compare Register 4
    PCA0CPL4 = 0x00;    // PCA Counter/Timer Low Byte
    PCA0CPH4 = 0x00;    // PCA Counter/Timer High Byte

//----------------------------------------------------------------
// Timers Configuration
//----------------------------------------------------------------

    CKCON = 0x02;   // Clock Control Register    
    TL0 = 0x00;     // Timer 0 Low Byte
    TL1 = 0x00;     // Timer 1 Low Byte
    TH0 = 0x00;     // Timer 0 High Byte
    TH1 = 0x00;     // Timer 1 High Byte    
    TMOD = 0x02;    // Timer Mode Register
    TCON = 0x00;    // Timer Control Register 
		
    TMR2RLL = 0x00; // Timer 2 Reload Register Low Byte
    TMR2RLH = 0x00; // Timer 2 Reload Register High Byte
    TMR2L = 0x00;   // Timer 2 Low Byte
    TMR2H = 0x00;   // Timer 2 High Byte    
    TMR2CN = 0x00;  // Timer 2 Control Register

    TMR3RLL = 0x00; // Timer 3 Reload Register Low Byte
    TMR3RLH = 0x00; // Timer 3 Reload Register High Byte
    TMR3L = 0x00;   // Timer 3 Low Byte
    TMR3H = 0x00;   // Timer 3 High Byte    
    TMR3CN = 0x00;  // Timer 3 Control Register

//----------------------------------------------------------------
// Reset Source Configuration
// NOTE! : Comparator 0 must be enabled before it is enabled as a 
// reset source.
//------------------------------------------------------------------

	RSTSRC = 0x00;	// Reset Source Register


//----------------------------------------------------------------
// Interrupt Configuration
//----------------------------------------------------------------

    IE = 0x02;         //Interrupt Enable
    IP = 0x00;         //Interrupt Priority
    EIE1 = 0x00;       //EXtended Interrupt Enable
    EIP1 = 0x00;       //EXtended Interrupt Priority
    IT01CF = 0x01;     //INT0/INT1 Configuration Register

// other initialization code here...



}   //End of config
