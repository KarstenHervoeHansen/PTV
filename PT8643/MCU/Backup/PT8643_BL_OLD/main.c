//-----------------------------------------------------------------------------
//	Filename	: main.c
//	Date		: 2010-06-01
//	Name		: BS
//	Processor	: C8051F132
//	Copyright	: DK-Technologies A/S
//-----------------------------------------------------------------------------

#include <c8051F120.h>                      	// Device-specific SFR Definitions
#include <stdio.h>								// PrintF
#include <string.h>
#include "main.h"
#include "i2c_bus.h"
#include "i2c_slave.h"
#include "UART_0.h"
#include "textstr.h"
#include "cmd_io.h"
#include "mcu_flash.h"

sbit	TP1		=	P0^2;
sbit	TP2		=	P0^3;
sbit	TP3		=	P0^4;
sbit	TP4		=	P0^5;

sbit	TP6		=	P2^0;
sbit	TP7		=	P2^1;
sbit	TP8		=	P2^2;
sbit	TP9		=	P2^3;

sbit	LED1	=	P2^4;
sbit	LED2	=	P2^5;
sbit	LED3	=	P2^6;
sbit	LED4	=	P2^7;

sbit    INT0	=   P0^4;
sbit    GPS_LOCK=   P1^0;
sbit    GPS_REQ =   P1^1;						// Request leap seconds data.

sbit	ETH_RES	=	P4^5;
unsigned char bdata BOOT _at_ 0x2F;				// Used for Interrupt redirection.

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F12x
//-----------------------------------------------------------------------------
sfr16 ADC0     = 0xbe;                         // ADC0 data register
 
//------------------------------------------------------------------------------
// Global Constants
//------------------------------------------------------------------------------
#define SYSCLK                  98000000L      // System Clock Frequency in Hz
#define CHECK_BIT(var,pos) !((var) & (1<<(pos)))

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
void LED_Update(char LED_No);
void StartRUN(void);
void ReadMAC(void);
void GenerateNetfinderStr(void); 					// This is to avoid linker warning.
void ResetCPFlash(void);
//-----------------------------------------------------------------------------
// Global Variables...
//-----------------------------------------------------------------------------
unsigned char idata CmdArray[32];				// Array to hold incomming Commands.
												// Must be in IDATA otherwise the
												// function will be too slow.
unsigned char CmdIndex;							// Index in the CmdArray.
unsigned char CmdState;							// Defines how incomming commands should be handled.

unsigned int boot_timeout 				= 1;

unsigned int  xdata LED1_STATE_COUNTER 	= 0x0000;// Led Control Counter
unsigned char xdata LED1_STATE 			= BLINK_SLOW;
unsigned int  xdata LED2_STATE_COUNTER 	= 0x0000;// Led Control Counter
unsigned char xdata LED2_STATE 			= SOLID_OFF;
unsigned int  xdata LED3_STATE_COUNTER 	= 0x0000;// Led Control Counter
unsigned char xdata LED3_STATE 			= SOLID_OFF;
unsigned int  xdata LED4_STATE_COUNTER 	= 0x0000;// Led Control Counter
unsigned char xdata LED4_STATE 			= SOLID_OFF;


#define UART_BUFFERSIZE 64
unsigned char UART_Buffer[UART_BUFFERSIZE];
unsigned char UART_Buffer_Size = 0; 
unsigned char UART_Input_First = 0;
unsigned char UART_Output_First = 0;
unsigned char TX_Ready =1;
static char Byte;

unsigned char idata flash_secure_1 = 0x00;		// Should be set to 0x44 to enable Flash Functions.
unsigned char idata flash_secure_2 = 0x00;		// Should be set to 0x85 to enable Flash Writes.
unsigned char idata flash_secure_3 = 0x00; 		// Should be set to 0x02 to enable Flash Erase.
unsigned char idata flash_secure_4 = 0x00;	 	// Should be set to 0x55 to enable MOVX Redirect.
	
bit code_erased = 0;	  			            // Flag to indicate that the FLASH erase operation is complete.

unsigned int xdata BL_CRC 		   = 0x0000;	// Flash CRC.
unsigned int xdata RUN_CRC         = 0x0000;	// Flash CRC.

//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main(void)
{
	unsigned char dt;

	// Disable watchdog timer
	WDTCN = 0xDE;
	WDTCN = 0xAD;

	RSTSRC = 0x02;								// Select VDD Monitor as reset source... (Bit PORSF)

	BOOT = 0;									// Tell interrupt rutines in bootloader that the bootloader is running.

	Init_Device();								// Initialize the MCU

	setup_I2C();

	CmdState = CmdState_Normal;

	i2c_set_rs232dest(RS232_DSUB_MCU); 			// RS232_DISCONNECTED RS232_DSUB_MCU RS232_DSUB_MF;RS232_MCU_MF

//	i2c_set_rs232dest(RS232_DISCONNECTED); 			// RS232_DISCONNECTED RS232_DSUB_MCU RS232_DSUB_MF;RS232_MCU_MF

	SFRPAGE = CONFIG_PAGE;
	while(ETH_RES == 0)							// Wait for CP2200 startup completes.
	{
		if (boot_timeout > 100) break;
	}
	ReadMAC();
	GenerateNetfinderStr();

	//ResetCPFlash();

	BL_CRC = CRConFlash(0); 					// Bootloader CRC.
	RUN_CRC = CRConFlash(1); 					// Run code CRC.

	Flush_COM0buffers();
	SFRPAGE = LEGACY_PAGE;
	if ( RSTSRC == 0x10) boot_timeout = 5000;	// Software reset... Force boot timeout...


	while(1)
	{

		if (boot_timeout > 3000) // 3000 = 15 Seconds. If BOOT = 0x55 the run code signals a fast restart is needed.
		{
			boot_timeout = 0;
			i2c_set_rs232dest(RS232_DSUB_MF);
			StartRUN();
		}
	
	    Oscillator_Init();
		if (RXcount > 0)
		{
			dt = COM0GetByte();
			if (CmdState == CmdState_CMD){decode_cmd(dt);} 					// Decode Incomming Commands.
			else
			{

			//----< When IO module is in transparent mode scan data >----
			//----< for the command @AT+++ [CR]                     >----
			//----< In memorymodel LARGE this is fast enough.       >----              
			if (dt == 0x40) {CmdIndex = 0;} // @ will reset the array. (Start of CMD)
			CmdArray[CmdIndex] = dt;
			CmdIndex++;
	
			if (CmdIndex > 8) {CmdIndex = 0;}
			if ((dt == 0x0D) && (CmdIndex == 7)) 		// CR = End of CMD.
			{
				CmdArray[CmdIndex - 1] = 0x00; 			// Make sure the CMD is ended with a 0x00 (CR replaced with 0x00)
				if (strcmp(CmdArray,cmd_enable) == 0) 	
				{										//Enable Command Mode.
					boot_timeout = 0; 					// Disable RS232 Switch and normal boot procedure. (Stay in bootloader.)
					LED1_STATE	= BLINK_FAST;
					CmdIndex = 0;
					str2COM0(&str_ok, 0x00);
					str2COM0(&str_lfcr, 0x00);
					Transmit_COM0();
					CmdState = CmdState_CMD;
				}
			}
			//----<                     END scan data               >----
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Timer2_ISR (T2_OVERFLOW_RATE Hz)
//-----------------------------------------------------------------------------
void Timer2_ISR (void) interrupt 5
{
	if (BOOT == 0)							// Check if bootloader is running.
	{

		char SFRPAGE_SAVE = SFRPAGE;     	// Save Current SFR page

		TF2 = 0;							// Clear Timer 2 Overflow Flag
		
		LED1_STATE_COUNTER++;
		LED2_STATE_COUNTER++;
		LED3_STATE_COUNTER++;
		LED4_STATE_COUNTER++;
		LED_Update(1);
		LED_Update(2);
		LED_Update(3);
		LED_Update(4);
	
		if (boot_timeout > 0){boot_timeout++;}
		
	
		SFRPAGE = SFRPAGE_SAVE;       		// Restore SFR page

	} else {((void (code *) (void)) 0x242B)();} //Jump to interrupt in run code.
}

void LED_Update(char LED_No)
{
	unsigned char	LED_STATE;
	unsigned int	LED_STATE_COUNTER;
	bit				LED;

	SFRPAGE = CONFIG_PAGE;

	switch (LED_No)
	{
		case 1:
			LED_STATE 			= LED1_STATE;
			LED_STATE_COUNTER 	= LED1_STATE_COUNTER;
			LED					= LED1;
		break;
	
		case 2:
			LED_STATE 			= LED2_STATE;
			LED_STATE_COUNTER 	= LED2_STATE_COUNTER;
			LED					= LED2;
		break;
		
		case 3:
			LED_STATE 			= LED3_STATE;
			LED_STATE_COUNTER 	= LED3_STATE_COUNTER;
			LED					= LED3;
		break;
		
		case 4:
			LED_STATE 			= LED4_STATE;
			LED_STATE_COUNTER 	= LED4_STATE_COUNTER;
			LED					= LED4;
		break;
	}
	
   
   switch (LED_STATE){
       
      case DISABLED:                   // Do nothing
		 break;

      case SOLID_OFF:                  // Always OFF
         LED = 1;
         LED_STATE_COUNTER = 0;
         break;
      
      case SOLID_ON:                   // Always ON
         LED = 0;
         LED_STATE_COUNTER = 0;
         break;

      case BLINK_SLOW:                 // Toggle every second
         if(LED_STATE_COUNTER >= T2_OVERFLOW_RATE){
            LED = !LED;
            LED_STATE_COUNTER = 0;
         }
         break;

      case BLINK_FAST:                 // Toggle 10 times per second
         if(LED_STATE_COUNTER >= T2_OVERFLOW_RATE/5){
            LED = !LED;
            LED_STATE_COUNTER = 0;
         }
         break;

      case BLINK_TWICE:                // Toggle 4 times, then pause.
         if(LED_STATE_COUNTER == 0){
            LED = 0;
         } else
         if(LED_STATE_COUNTER == 1*T2_OVERFLOW_RATE/10){
            LED = 1;
         }
         if(LED_STATE_COUNTER == 3*T2_OVERFLOW_RATE/10){
            LED = 0;
         }
         if(LED_STATE_COUNTER == 4*T2_OVERFLOW_RATE/10){
            LED = 1;
         }
         if(LED_STATE_COUNTER == 20*T2_OVERFLOW_RATE/10){
            LED_STATE_COUNTER = -1;
         }
         break;
	  
   }

	switch (LED_No)
	{
		case 1:
			LED1_STATE_COUNTER	= LED_STATE_COUNTER;
			LED1				= LED;
		break;
	
		case 2:
			LED2_STATE_COUNTER	= LED_STATE_COUNTER;
			LED2				= LED;
		break;
		
		case 3:
			LED3_STATE_COUNTER	= LED_STATE_COUNTER;
			LED3				= LED;
		break;
		
		case 4:
			LED4_STATE_COUNTER	= LED_STATE_COUNTER;
			LED4				= LED;
		break;
	}
}

//-----------------------------------------------------------------------------
// UART0_Interrupt
//-----------------------------------------------------------------------------
void UART0_Interrupt (void) interrupt 4
{
	if (BOOT == 0)								// Check if bootloader is running.
	{
		char SFRPAGE_SAVE = SFRPAGE;     		// Save Current SFR page
		SFRPAGE = UART0_PAGE;

		if (RI0 == 1)
		{
			RI0 = 0;							// Clear interrupt flag
			COM0GetByte_ISR(SBUF0);
		}

		if (TI0 == 1)							// Should something be transmitted...
		{
			TI0 = 0;							// Clear interrupt flag
			if(TXcount > 0) 
				SBUF0 = COM0PutByte_ISR();
		}

		SFRPAGE = SFRPAGE_SAVE;       			// Restore SFR page

	} else {((void (code *) (void)) 0x2423)();} //Jump to interrupt in run code.
}

unsigned char BitOn(unsigned char TheChar, unsigned char TheBit)
{
	return (TheChar | (1 << TheBit));
}

unsigned char BitOff(unsigned char TheChar, unsigned char TheBit)
{
	return (TheChar & ((1 << TheBit) ^ 0xFF)); // ^ xor
}

void StartRUN(void)
{
    int i = 0;

	unsigned char CRCStatus		= 0;
	unsigned int CRCFromFlash	= 0x0000;   
	unsigned int CRCCalcFlash	= 0x0000;

	CRCStatus =  read_flash_addr(FLASH_CRCOWR_ADDR);

	CRCFromFlash = read_flash_addr(FLASH_RUNCRC_ADDR);
	CRCFromFlash <<= 8;
	CRCFromFlash += read_flash_addr(FLASH_RUNCRC_ADDR + 1);

	CRCCalcFlash = CRConFlash(1); 
	if (((CRCStatus == 0x01) && (CRCCalcFlash == CRCFromFlash)) || (CRCStatus == 0xA2)) // If CRCStatus = A2 skip CRC check. Just run the code.
	{
		i2c_set_rs232dest(RS232_DSUB_MF);

	    SFRPAGE   = CONFIG_PAGE;

	    OSCICN    = 0xC0;	// Internal OSC enabled running at programmed Freq.
	    CLKSEL    = 0x00;	// SYS Clock derived from internal OSC without PRE-divide.
	    CCH0CN    = 0xE6;
	    SFRPAGE   = LEGACY_PAGE;
	    FLSCL     = 0x80;
	    SFRPAGE   = CONFIG_PAGE;
	    PLL0CN    = 0x00;
	    PLL0DIV   = 0x01;
	    PLL0FLT   = 0x31;
	    PLL0MUL   = 0x01;

		BOOT = 1;
		((void (code *) (void)) FLASH_START_ADDR) ();
	}
	else
	{
		LED1_STATE	= BLINK_SLOW;
		LED2_STATE	= BLINK_SLOW;
		LED1_STATE_COUNTER = 0;
		LED2_STATE_COUNTER = 100;
	}
}

void ReadMAC(void)
{
	unsigned char i, j;
	char xdata *p;

    bit	EA_state = EA;								// Holds interrupt state

	EA = 0;
	ETH_RES = 1;

    for( i = 0; i < 255; i++)
	{
		i2c_delay(); // Wait for CP2200 to start up.
	}

	p = 0x2068;	// Set Address pointer Low.
	*p = 0xFA;
	p = 0x2069; // Set Address pointer High.
	*p = 0x1F;
	p = 0x2005; // Auto Read Flash...
	
	i2c_delay(); // A small delay is necessary before data can be read from flash...
    for( i = 0; i < 6; i++)
	{
		j = *p;
		MAC_addr[i] = j;
	}

	ETH_RES = 0;
    EA = EA_state;                  				// restore interrupt state
}

//-----------------------------------------------------------------------------
// GenerateNetfinderStr : This is to avoid calling "sprintf" from an ISR.
//-----------------------------------------------------------------------------
void GenerateNetfinderStr()
{
	unsigned int v1, v2;
	unsigned char c, d;

	v1 = read_flash_addr(FLASH_BLInfo_ADDR);
	v2 = read_flash_addr(FLASH_BLInfo_ADDR + 1);
	c = sprintf(netfinder_name, "PT8643 Bootloader v.%d.%d",v1, v2);
	
	for(d = c; d < 32; d++) netfinder_name[d] = 0x20;
}

//-----------------------------------------------------------------------------
// ResetCPFlash : Write a 0 to the flash init byte. Run code will write defaults.
//-----------------------------------------------------------------------------
void ResetCPFlash(void)
{
	unsigned char i, j;
	char xdata *p;

    bit	EA_state = EA;								// Holds interrupt state

	EA = 0;
	ETH_RES = 1;

    for( i = 0; i < 255; i++)
	{
		i2c_delay(); // Wait for CP2200 to start up.
	}

	p = 0x2067;	// Set Flash Lock and Key.
	*p = 0xA5;	// Set write key part 1.
	*p = 0xF1;	// Set write key part 2.

	p = 0x2068;	// Set Address pointer Low.
	*p = 0x00;	// 0x1C00 is the flash init byte in the RUN code.
	p = 0x2069; // Set Address pointer High.
	*p = 0x1C;

	p = 0x206A; // Flash Erase Register.
	*p = 0x01;	// Trigger flash erase.

	p = 0x207B; // Flash status register.
	

    for( i = 0; i < 255; i++)
	{
		j = *p;
		if (CHECK_BIT(j, 3) == 1) break;
	}

	ETH_RES = 0;
    EA = EA_state;                  				// restore interrupt state
}
