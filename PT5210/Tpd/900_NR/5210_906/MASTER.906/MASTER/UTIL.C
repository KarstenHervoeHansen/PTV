/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		UTIL.C																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960628																		*/
/*	Rev. date:	971215, KEn	DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		void NONE()																				*/
/*		UC BitPos( UC value)																	*/
/*		UC HexToByte( char s)																*/
/*		UC htol( char* s, UL* result)														*/
/*		UC otol( char* s, UL* result)														*/
/*		UC btol( char* s, UL* result)														*/
/*		void _ultoa( UL val, char* result)												*/
/*		int Cstrlen( code char * s)                                     		*/
/*		char* Cstrcat( register char* to, register code char* from)				*/
/*		void* Cmemcpy( void* d1, code void* s1, register size_t n)				*/
/*		char* Cstrcpy( char* to, register code char * from)						*/
/*		int Cstrcmp( register const char * s1, register code char * s2)		*/
/*		int Cstrncmp( register const char * s1, register code char * s2)		*/
/*		char FindNextASCII( UC Direction, UC Type, char ASCIIChar)				*/
/*		bit CheckTextASCII( UC Type, char ASCIIChar)									*/
/*		bit ROMCheckOK()																		*/
/*		bit RAMCheckOK()																		*/
/*		bit RS232CheckOK( UC Baudrate)													*/
/*                                                                         */
/* Changes:                                                                */
/* 971215: /906 version. Added the character + in FindNextASCII &				*/
/*				CheckTextASCII. LimitedType now extended								*/
/*	971210: In CheckTextASCII: Space was NOT recognized as a valid character*/
/* 971015: RAMCheckOK() implemented. Added RS232CheckOK							*/
/* 971010: SizeOfPROMCode is now 0x30000												*/
/* 971009: Cleanup. ROMCheckOK() optimized											*/
/* 971003: Optimized ROMCheckOK(). Added Cstrpy()									*/
/* 971001: Added parameter list for ROMCheckOK()									*/
/* 970919: Added routine: BitPos()														*/
/* 970918: Added routine: RAMCheckOK(), (still empty though)					*/
/* 970724: Added routine: ROMCheckOK()													*/
/* 970625: Added routine: CheckTextASCII()											*/
/* 970119: Optimized _ultoa																*/
/* 970202: Removed ErrorHandling to its own module: MESSAGE.C51				*/
/* 970127: Added VoltageErrorType														*/
/* 970106: Added function ErrorHandling()												*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <stdio.h>
#include <string.h>
#include <xa.h>

#include <ctype.h>

#include "mas.h"
#include "52sync.h"
#include "util.h"
#include "piic.h"
#include "tables.h"
#include "text.h"
#include "xaexprt.h"

#define SizeOfPROMCode	0x30000	// No of bytest used in PROM
code char CSumAddr @ 0x2FFFE;		// Start of 16-bit PROM checksum

UC StartOfRAM @ 0x200;				// First available external RAM location

code char Page0 @ 0x00000;			// Code space page 0
code char Page1 @ 0x10000;			// Code space page 1
code char Page2 @ 0x20000;			// Code space page 2
code char Page3 @ 0x30000;			// Code space page 3
code char Page4 @ 0x40000;			// Code space page 4
code char Page5 @ 0x50000;			// Code space page 5
code char Page6 @ 0x60000;			// Code space page 6
code char Page7 @ 0x70000;			// Code space page 7

/***************************************************************************/
/*	NONE																		 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	DOES ABSOLUTELY NOTHING!!!												*/
/*	Remarks:		This one is primarily used in menu tables							*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void NONE() {
}

/***************************************************************************/
/*	BitPos																	 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970919	    										*/
/* Revised:		970919				     													*/
/*																									*/
/*	Function:	Return the bit position of a variable								*/
/*	Remarks:		If parameter contains more than one bit the number of the	*/
/*					first bit is returned													*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
UC BitPos( UC value) {

	UC cnt = 1;

	while ( !( value & 0x01) && ( cnt < 9)) {
		cnt++;
		value >>= 1;
	}

	return( cnt);
}

/***************************************************************************/
/*	HexToByte																 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Converts a char defined in hex to binary							*/
/*	Remarks:		--																				*/
/*	Returns:		A byte from 0-15															*/
/*	Updates:		--																				*/
/***************************************************************************/
UC HexToByte( char s) {

	toupper( s);

	if (( s >= 'A') && ( s <= 'F'))
		return( s - 'A' + 10);

	return( s - '0');
}

/***************************************************************************/
/*	htol																		 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Converts a string containg a hex number to an unsigned long	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
UC htol( char* s, UL* result) {

	*result = 0;

	while ( *s)
		*result = 16*(*result) + HexToByte( *s++);

	return( 0);
}

/***************************************************************************/
/*	otol																		 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Converts a string containg an octal number to unsinged long	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
UC otol( char* s, UL* result) {

	*result = 0;

	while ( *s)
		*result = 8*(*result) + ( *s++ - '0');

	return( 0);
}

/***************************************************************************/
/*	btol																		 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Converts a string containg a binary number to unsigned long	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
UC btol( char* s, UL* result) {

	*result = 0;

	while ( *s)
		*result = 2*(*result) + ( *s++ - '0');

	return( 0);
}

/***************************************************************************/
/*	_ultoa																	 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		970219				     													*/
/*																									*/
/*	Function:	Converts an unsigned long to a string								*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void _ultoa( UL val, char* result) {

	UL tmp = val;
	UC i=0, j=0, TmpChar;

	if ( val == 0)
		strcpy( result, "0");
	else {
		while ( val) {
			tmp = val / 10;
			result[i++] = (val % 10) + '0';
			val = tmp;
		}
		result[i--] = '\0';

		while ( i >= j) {							// Reverse ASCII string
			TmpChar = result[i];
			result[i] = result[j];
			result[j] = TmpChar;
			i--;
			j++;
		}
	}
}

/***************************************************************************/
/*	Cstrlen																	 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Calculates the length of a string from code memory				*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
size_t Cstrlen( register code char* s) {

	register code char *	cp;

	cp = s;
	while( *cp++)
		continue;

	return( cp-s-1);
}

/***************************************************************************/
/*	Cstrcat																	 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Concatenate a string from code memory to data memory			*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
char* Cstrcat( register char* to, register code char* from) {

	register char* cp;

	cp = to;
	while( *cp)
		cp++;

	while( *cp++ = *from++)
		continue;

	return( to);
}

/***************************************************************************/
/*	Cmemcpy																	 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Copy a memory-area from code-memory to RAM-memory				*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void* Cmemcpy( void* d1, code void* s1, register size_t n) {

	register char* d;
	register code char* s;

	s = s1;
	d = d1;

	while( n--)
		*d++ = *s++;

	return( d1);
}

/***************************************************************************/
/*	Cstrcpy																	 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 971003	    										*/
/* Revised:		971003				     													*/
/*																									*/
/*	Function:	Copy a string in code-memory to a string in RAM-memory		*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
char* Cstrcpy( char* to, register code char * from) {

	register char*	cp;

	cp = to;
	while( *cp++ = *from++)
		continue;

	return( to);
}

/***************************************************************************/
/*	Cstrcmp																	 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Compare a string in code-memory to a string in RAM-memory	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
int Cstrcmp( register const char * s1, register code char * s2) {

	register signed char	r;

	while( !(r = *s1 - *s2++) && *s1++)
		continue;

	return( r);
}

/***************************************************************************/
/*	Cstrncmp																	 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Compare a string in code-memory to a string in RAM-memory	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
int Cstrncmp( register const char * s1, register code char * s2, size_t len) {

	while( len--) {
		if ( *s1 == 0 || *s1 != *s2)
			return( *s1 - *s2);

		s1++;
		s2++;
	}

	return( 0);
}

/***************************************************************************/
/*	FindNextASCII															 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		971215				     													*/
/*																									*/
/*	Function:	Handle the display containing the									*/
/*	Remarks:		Type: 0 = A-Z, a-z, 		0-9, -,    _, spc							*/
/*							1 = A-Z,      		0-9, -,    _, spc							*/
/*							2 = A-Z, a,h,o,x,	0-9, -, +, _, spc							*/
/*					Type 2 is ONLY used with PT8603/905									*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
char FindNextASCII( UC Direction, UC Type, char ASCIIChar) {

	if ( Direction ) {						// Down: 0 otherwise UP

		switch ( Type) {
			case 0:
				switch ( ASCIIChar) {
					case ' ':
						return( '-');
					case '-':
						return( '0');
					case '9':
						return( 'A');
					case 'Z':
						return( 'a');
					case 'z':
						return( '_');
					case '_':
						return( ' ');
				}
				break;

			case 1:
				switch ( ASCIIChar) {
					case ' ':
						return( '-');
					case '-':
						return( '0');
					case '9':
						return( 'A');
					case 'Z':
						return( '_');
					case '_':
						return( ' ');
				}
				break;

			case 2:
				switch ( ASCIIChar) {
					case ' ':
						return( '-');
					case '-':
						return( '+');
					case '+':
						return( '0');
					case '9':
						return( 'A');
					case 'Z':
						return( 'a');
					case 'a':
						return( 'h');
					case 'h':
						return( 'o');
					case 'o':
						return( 'x');
					case 'x':
						return( '_');
					case '_':
						return( ' ');
				}
				break;
		}
		return( ++ASCIIChar);
	}
	else {
		switch ( Type) {
			case 0:
				switch ( ASCIIChar) {
					case ' ':
						return( '_');
					case '-':
						return( ' ');
					case '+':
						return( '-');
					case '0':
						return( '-');
					case 'A':
						return( '9');
					case 'a':
						return( 'Z');
					case '_':
						return( 'z');
				}
				break;

			case 1:
				switch ( ASCIIChar) {
					case ' ':
						return( '_');
					case '-':
						return( ' ');
					case '+':
						return( '-');
					case '0':
						return( '-');
					case 'A':
						return( '9');
					case '_':
						return( 'Z');
					default:
						return( --ASCIIChar);
				}
				break;

			case 2:
				switch ( ASCIIChar) {
					case ' ':
						return( '_');
					case '-':
						return( ' ');
					case '+':
						return( '-');
					case '0':
						return( '+');
					case 'A':
						return( '9');
					case 'a':
						return( 'Z');
					case 'h':
						return( 'a');
					case 'o':
						return( 'h');
					case 'x':
						return( 'o');
					case '_':
						return( 'x');
				}
				break;
		}
		return( --ASCIIChar);
	}
}

/***************************************************************************/
/*	CheckTextASCII															 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970625	    										*/
/* Revised:		971215				     													*/
/*																									*/
/*	Function:	Check if ASCII value is valid in text								*/
/*	Remarks:																						*/
/*	Returns:		--																				*/
/*	Updates:		-------																		*/
/***************************************************************************/
bit CheckTextASCII( UC Type, char ASCIIChar) {

	if ((( ASCIIChar >= '0') && ( ASCIIChar <= '9')) ||
		 (( ASCIIChar >= 'A') && ( ASCIIChar <= 'Z')) ||
		 ( ASCIIChar == '-') || ( ASCIIChar == '_') ||
		 ( ASCIIChar == '_') || ( ASCIIChar == ' '))
		 return( TRUE);

	 if ( Type == 2) {

		 if (( ASCIIChar == '+') || ( ASCIIChar == 'a') ||
		     ( ASCIIChar == 'h') || ( ASCIIChar == 'o') ||
		 	  ( ASCIIChar == 'x'))
			return( TRUE);
	}

	if ( Type == 0) {
		if (( ASCIIChar >= 'a') && ( ASCIIChar <= 'z'))
			 return( TRUE);
	}

	return( FALSE);
}

/***************************************************************************/
/*	ROMCheckOK																 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970724	    										*/
/* Revised:		971009				     													*/
/*																									*/
/*	Function:	Test if checksum stored in EEPROM is correct						*/
/*	Remarks:		Checksum is a simple 16 bit checksum								*/
/*					This routine requires a LOT of attention.							*/
/*	Returns:		1: ROM check was OK, 0: The calculated checksum does not		*/
/*					match the stored checksum												*/
/*	Updates:		--																				*/
/***************************************************************************/
bit ROMCheckOK( UI* Checksum) {

	register code char* s;
	register UI i;
	UC j;
	UI CalcChecksum = 0;

	for ( j = 0; j < ( SizeOfPROMCode / 0x10000); j++) {
		feed_dog;

		switch ( j) {
			case 0:
				s = &Page0;
				break;
			case 1:
				s = &Page1;
				break;
			case 2:
				s = &Page2;
				break;
			case 3:
				s = &Page3;
				break;
			case 4:
				s = &Page4;
				break;
			case 5:
				s = &Page5;
				break;
			case 6:
				s = &Page6;
				break;
			case 7:
				s = &Page7;
				break;
		}

		i = 0;
		do {
			CalcChecksum += ( UC) *s++;
		} while ( ++i);
	}
	s = &CSumAddr;

	*Checksum = ( 256 * ( UC) *s++);
	*Checksum += (UC) *s;

	return( CalcChecksum == *Checksum);
}

/***************************************************************************/
/*	RAMCheckOK																 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970918	    										*/
/* Revised:		971013				     													*/
/*																									*/
/*	Function:	Test if checksum stored in EEPROM is correct						*/
/*	Remarks:		This routine requires a LOT of attention.							*/
/*	Returns:		1: RAM check was OK, 0: The RAM failed at some point			*/
/*	Updates:		--																				*/
/***************************************************************************/
bit RAMCheckOK() {

	UC tmp, OKFlag = TRUE;
	UC* RAMPage;
	UI i;

	IIC2_Write( mainram1_addr, RAMCheckOK_addr);
	if ( IIC2_Read( mainram1_addr) != TRUE)
		return( FALSE);

	IIC2_Write2( mainram1_addr, RAMCheckOK_addr, FALSE);

	RAMPage = &StartOfRAM;

	for ( i = 0; i < ( 8*1024 - (int) &StartOfRAM); i++) {
		tmp = *RAMPage;

		*RAMPage = 0x55;

		if ( *RAMPage != 0x55) {
			*RAMPage = tmp;
			OKFlag = FALSE;
			break;
		}
		else {
			*RAMPage = 0xAA;

			if ( *RAMPage != 0xAA) {
				*RAMPage = tmp;
				OKFlag = FALSE;
				break;
			}
		}

		*RAMPage++ = tmp;
	}

	IIC2_Write2( mainram1_addr, RAMCheckOK_addr, TRUE);

	return( OKFlag);
}

/***************************************************************************/
/*	RS232CheckOK															 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 971014	    										*/
/* Revised:		971015				     													*/
/*																									*/
/*	Function:	Test if serial communication at a given baudrate is OK		*/
/*	Remarks:		Requires that a loopback connector is mounted					*/
/*	Returns:		1: RS232 check was OK, 0: An error occurred						*/
/*	Updates:		--																				*/
/***************************************************************************/
bit RS232CheckOK( UC Baudrate) {

	UC i, j, idx;
	char ch;

	feed_dog;

	for ( i = UDR_DATA7; i <= UDR_DATA8; i++) {
		for ( j = UDR_NONE; j <= UDR_EVEN; j++) {
			waitms( 2);
														// Re-initialize UART
			Udr_InitializeUart( Baudrate, i, j, UDR_XON_XOFF);

			waitms( 2);
	
			idx = 0;
			while ( ch = RS232TestTxt[idx]) {
				Udr_PutChar( ch);

				feed_dog;

				UserTimer = 40;
				while ((( ch = Udr_InChar()) == UDR_EOF) && UserTimer)
					feed_dog;

				if (( ch != RS232TestTxt[idx++]) || ( UserTimer == 0))
					return( FALSE);
			}	
		}
	}
	return( TRUE);
}
