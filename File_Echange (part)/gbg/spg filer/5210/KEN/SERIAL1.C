/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		SERIAL1.C																	*/
/*	Author:		Preben Christiansen, DEV												*/
/*	Org. date:	960903																		*/
/*	Rev. date:	980915, KEn DEV															*/
/*	Status:		Version 2.1																	*/
/* Purpose:		RS232 drivers for RX1/TX1 (port P1.4/P1.5)						*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		void Serial1_init( void)															*/
/*		interrupt void Serial1_int( void)												*/
/*		UC AnswerCheck( void)																*/
/*		void pause1( UI tid)																	*/
/*		UC waitanswer( void)																	*/
/*		void put_addr1( UC adr)																*/
/*		void put_char1( UC c) 																*/
/*		void put_value( char delimiter, UL value) 									*/
/*		char get_char1( void)																*/
/*		UC SndChar( UC adr, char *A, UC x)												*/
/*		UC SndInt( UC adr, char *A, UI x)												*/
/*		UC SndInt3( UC adr, char *A, UI x, UI y, UI z)								*/
/*		UC SndInt2( UC adr, char *A, UI x, UI y)										*/
/*		UC SndLong( UC adr, char *A, UL x)												*/
/*		UC SndVoid( UC adr, char *A)														*/
/*		UC SndAsc( UC adr, char *A, UC n, const char *st)							*/
/*		UC SndRequest( UC adr, char *A, UC n)											*/
/*		UC SndSpecial( UC adr, char *A, char C)										*/
/*		UI RecInt( UC address, char* cmd, UC val, UC* error)						*/
/*		UL RecLong( UC address, char* cmd, UC val, UC* error)						*/
/*		char* RecAsc( UC address, char* cmd, UC val, UC* error)					*/
/*		char* RecStr( UC address, char* cmd, UC val, UC* error)					*/
/*																									*/
/* HISTORY:																						*/
/* 980915: In waitanswer, put_char1() & put_addr(): due to the change of	*/
/*			  specification in the XA-G3 processor introducing double 			*/
/*			  buffering in the UART transmitter, this routine now FORCES the	*/
/*			  single buffering method. NOTE! Since the ninth bit in the			*/
/*			  transmitter, T1B8 (TB8_1), is used for multiprocessor coding, 	*/
/*			  double buffering should NOT be used, because this bit is NOT		*/
/*			  double buffered.																*/
/* 970428 Increased delay-parameter in put_char1 from 100->135 to prevent	*/
/*			  the OSC from generating a checksum error								*/
/* 971203 Several errors corrected														*/
/* 971126 Added parameter val to RecInt() & RecLong								*/
/* 971124 Added put_value() & rewrote ALL Snd?? routines							*/
/* 971031 Added RecStr()																	*/
/* 971030 Added SndInt3()																	*/
/* 971027 Modified RecAsc. Removed SndRequest as public							*/
/* 971024 Removed SndAsc_n & SndRequest_n												*/
/* 971022 Increased CmdToSend buffer to 28, (KEn). In SndAsc & SndAsc_n		*/
/*			  the max. number of characters increased to 16.						*/
/* 970306 Added a 100us delay in routine put_char1 and waitanswer when		*/
/*			  retransmitting, (KEn)															*/
/* 970218 Added SndInt2 function (KEn)													*/
/* 970115 Removed UC myaddress, (KEn)													*/
/* 961209 AnswerCheck now returns an UC. Transmission to units terminates	*/
/* 		  with an ';' instead of \0A													*/
/* 961129 Termination (0) added to AnswerBuf. No save of delimiter.			*/
/* 961111 Added SndRequest_n(), SndSpecial(), SndAsc_n()							*/
/* 961101 Added SndRequest() routine.													*/
/* 961031 Added request answering handling and more delimiters.				*/
/* 961014 Added Re-send. A checksum is sent after each command. If answer	*/
/* 		  is not 0x80 within 2 ms, the command is repeated (3 times).		*/
/* 960703 Routine names changed to Snd.. (Char, Int, Long, Asc, Void)		*/
/***************************************************************************/

#include <stdio.h>
#include <xa.h>						// Define 80C51XA registers
#include <intrpt.h>
#include <stdlib.h>
#include <string.h>

#include "serial1.h"


#define FALSE			0
#define TRUE			1

#define FifoLength	32				// Number of bytes in receiver buffer

#define MaxTransmit	3				// Maximum number of re-transmissions
#define TIME_OUT		10000			// Timeout: each 1000 is appr. 2 ms

static bit SM2_1 @ 0x325;
static bit TB8_1 @ 0X323;
static bit request;

near struct {
	char Buffer[FifoLength];		// Define receive-buffer
	UC Input;
	UC Output;
} RxBuf1;

static near UC CmdToSend[28];		// Transmitter command buffer
static near UC CmdIndex;			// Index to buffer
static near UC rstate;				// State of receive-machine
static near UC rep_no;				// No. of resend
static near UC cksum;				// Checksum of sent command
static near UC v24stat;				// Status for transmission

static near char AnswerBuf[30];	// Buffer to save answer and checksum

static bit AnswerError;				// Flag for checksum error in answer

static UI answertout;				// Time-out counter

/***************************************************************************/
/*	Serial1_init																  SERIAL1.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960903										*/
/* Revised:		971022				     													*/
/*																									*/
/*	Function:	 																				*/
/*	Remarks:																						*/
/*	Returns:		--			 																	*/
/*	Updates:																						*/
/***************************************************************************/
void Serial1_init(void) {

	RxBuf1.Input = 0;
	RxBuf1.Output = FifoLength - 1;
	RxBuf1.Buffer[0] = '\0';

	SWE |= 2;						// Enable 'wait_for_answer' interrupt
	ERI1 = 1; 						// Enable serial interrupt
	SM2_1 = 0;
}

/***************************************************************************/
/*	Serial1_int																	  SERIAL1.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960903										*/
/* Revised:		971022				     													*/
/*																									*/
/*	Function:	Interrupt function!!!! DO NOT CALL DIRECTLY!!!					*/
/*	Remarks:																						*/
/*	Returns:		--			 																	*/
/*	Updates:																						*/
/***************************************************************************/
interrupt void Serial1_int( void) {

	UC c;
	static near UC bufindex;

	while ( RI1) {
		c = S1BUF;
		RI1 = 0;										// save received byte

		if ( rstate == 1) {						// awaits answer
			if ( c == 0x80) {
				if ( request)
					rstate = 4;
            else
					rstate = 0;
				rep_no = 0;							// reset repetition counter
				cksum = 0;							// prepare check of answer (if any)
				bufindex = 0;						// prepare answer buffer
				v24stat = 0;						// OK-flag for transmit
			}
			else
				if ( c == 0x81) {					// Error in checksum
					v24stat = 1;					// Set checksum error flag
					rstate = 2;						// Enter re-send state
				}
		}
      else
			if ( rstate == 5) {					// Receive checksum
				AnswerBuf[bufindex] = '\0';	// Terminate

				if ( c == ( cksum & 0x7f))
					AnswerError = FALSE;			// Checksum OK
				else
					AnswerError = TRUE;			// Error in checksum

				rstate = 0;							// Back to initial state
				request = 0;						// Request flag off
			}
      else
			if ( rstate == 4) {					// Receive data
				answertout = TIME_OUT;			// Refresh time-out counter
				cksum += c;

				if (( c == 0x0A) || ( c == 0x0D) || ( c == ';') ||
					 ( c == '!') || ( c == '?') || ( c == '$'))
					rstate = 5;						// wait for checksum
				else
					AnswerBuf[bufindex++] = c;	// save byte
			}
	}

   if ( TI1)
		TI1 = 0;
}

/***************************************************************************/
/*	AnswerCheck																	  SERIAL1.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960903										*/
/* Revised:		971022				     													*/
/*																									*/
/*	Function:	Having sent a question, this routine waits for an answer		*/
/*	Remarks:		--																				*/
/*	Returns:		0: OK																			*/
/*					1: checksum error in answer from unit								*/
/*					2: No answer from unit													*/
/*	Updates:		answertout																	*/
/***************************************************************************/
UC AnswerCheck( void) {

	answertout = TIME_OUT;					// set time-out delay (is refreshed
													//  for each byte received)

	while (( answertout > 0) && ( rstate > 0))
		answertout--;

	if ( answertout == 0)
		return( 2);								// Time-out gives error

	return( (UC) AnswerError);
}

/***************************************************************************/
/*	pause1																		  SERIAL1.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960903										*/
/* Revised:		971022				     													*/
/*																									*/
/*	Function:	Delay function, appr. 110 us for each 100							*/
/*	Remarks:		--																				*/
/*	Returns:		-- 																			*/
/*	Updates:		--																				*/
/***************************************************************************/
void pause1( UI tid) {

	while( tid > 0)
		tid--;							// appr. 110 us for each 100
}

/***************************************************************************/
/*	waitanswer																	  SERIAL1.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960903										*/
/* Revised:		980915, KEn, DEV	     													*/
/*																									*/
/*	Function:	 																				*/
/*	Remarks:																						*/
/*	Returns:					 																	*/
/*	Updates:																						*/
/***************************************************************************/
UC waitanswer( void) {

// (former Low priority (SW)interrupt,) called from Serial1_int.
// Purpose is to wait for command-confirm code from units and re-send
// command in case of checksum error or time-out.

	UI tel;

	while (( rstate > 0) && ( rstate < 3)) {

		if ( rstate == 1) {
			tel = TIME_OUT;						// Wait up to appr. 2 ms

														// Wait until answer or timeout
			while (( rstate == 1) && ( --tel > 0))
				;

			if ( tel == 0) {						// If timeout...
				rstate = 2;							//  set state re-transmitting
				v24stat = 2;						//  timeout error
			}
		}

      if ( rstate == 2) {						// If state is re-transmission..
			CmdIndex = 0;							// resend command
			rep_no += 1;							// count re-sends

			if ( rep_no >= MaxTransmit) {		// If re-transmitted max. times..
				rstate = 0;							//  Sorry! receiver would not respond
				rep_no = 0;							//  this is a communication error!
			}
			else {									//	..otherwise re-transmit command
				TB8_1 = 1;							// Set bit for address transmit

				di();									// Disable interrupt while loading
														//  S1BUF to ensure that TI1 = 0
														//  clears a possible UART empty flag
														//  and NOT a transmission done flag.

				S1BUF = CmdToSend[CmdIndex++];// Send address
				TI1 = 0;								// Reset transmitter interrupt

				ei();									// Enable interrupt again.

				ERI1 = 0;       					// Disable serial interrupt
				while ( !TI1) 						//  while waiting for transmitter ready
					;
				ERI1 = 1;							// Enable serial interrupt
				TB8_1 = 0;							// Clear bit for data transmit

				pause1( 100);						// Wait for slave to evaluate address

				while ( CmdToSend[CmdIndex] != 0) {
					di();								// Disable interrupt while loading
														//  S1BUF to ensure that TI1 = 0
														//  clears a possible UART empty flag
														//  and NOT a transmission done flag.

														// Send next byte
					S1BUF = CmdToSend[CmdIndex++];
					TI1 = 0;							// Reset transmitter interrupt

					ei();								// Enable interrupt again.

					ERI1 = 0; 					  	// Disable serial interrupt 
					while ( !TI1)					//  while waiting for transmitter ready
						;
					ERI1 = 1;						// Enable serial interrupt

					pause1( 100);					// Wait for slave to evaluate data
            }
				rstate = 1;							// Wait for status answer
			}
		}
	}

	return( v24stat);
}

/***************************************************************************/
/*	put_addr1																	  SERIAL1.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 961023												*/
/* Revised:		980915, KEn DEV	     													*/
/*																									*/
/*	Function:	Transmit an adress and prepare for a new command				*/
/*	Remarks:		Sets checksum, cksum, to ZERO											*/
/*	Returns:		--			 																	*/
/*	Updates:		cksum, CmdIndex, CmdToSend												*/
/***************************************************************************/
void put_addr1( UC adr)  {

	pause1( 100);					// Wait (app.180us) for slaves to evaluate data

	TB8_1 = 1;						// Set bit for address transmit

	CmdToSend[0] = adr;			// Save address for re-transmission

	di();								// Disable interrupt while loading S1BUF to
										//  ensure that TI1 = 0 clears a possible UART
										//  empty flag and NOT a transmission done flag.

	S1BUF = adr;					// Transmit address
	TI1 = 0;							// Reset transmitter interrupt

	ei();								// Enable interrupt again.

	ERI1 = 0;						// Disable serial interrupt
	while ( !TI1)					//  while waiting for transmitter ready
		;
	ERI1 = 1;						// Enable serial interrupt

	TB8_1 = 0;						// Clear bit for data transmit

	cksum = 0;						// Clear checksum
	CmdIndex = 1;					// New command in progress
}

/***************************************************************************/
/*	put_char1																	  SERIAL1.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960903										*/
/* Revised:		980915				     													*/
/*																									*/
/*	Function:	Transmit one byte and update command buffer						*/
/*	Remarks:		--																				*/
/*	Returns:		--			 																	*/
/*	Updates:		cksum, CmdIndex, CmdToSend												*/
/***************************************************************************/
void put_char1( UC c)  {

	pause1( 135);					// Wait (app.??us) for slaves to evaluate data

	CmdToSend[CmdIndex++] = c;	// Save character for re-transmission
	cksum += c;

	di();								// Disable interrupt while loading S1BUF to
										//  ensure that TI1 = 0 clears a possible UART
										//  empty flag and NOT a transmission done flag.

	S1BUF = c;						// Transmit character
	TI1 = 0;							// Reset transmitter interrupt

	ei();								// Enable interrupt again.

	ERI1 = 0;						// Disable serial interrupt
	while ( !TI1)					//  while waiting for transmitter ready
		;
	ERI1 = 1;						// Enable serial interrupt
}

/***************************************************************************/
/*	put_value																	  SERIAL1.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 971124												*/
/* Revised:		971124				     													*/
/*																									*/
/*	Function:	Convert and transmit a value											*/
/*	Remarks:		--																				*/
/*	Returns:		--			 																	*/
/*	Updates:		cksum																			*/
/***************************************************************************/
void put_value( char delimiter, UL value) {

   if ( value >= 1000000000)
		put_char1( value/1000000000 + '0');

   if ( value >= 100000000)
	   put_char1(( value/100000000)%10 + '0');

   if ( value >= 10000000)
	   put_char1((  value/10000000)%10 + '0');

   if ( value >= 1000000) 
	   put_char1((  value/1000000)%10 + '0');

   if ( value >= 100000)
   	put_char1(( value/100000)%10 + '0');

   if ( value >= 10000)
	   put_char1(( value/10000)%10 + '0');

   if ( value >= 1000) 
   	put_char1(( value/1000)%10 + '0');

   if ( value >= 100)
	   put_char1(( value/100)%10 + '0');

   if ( value >= 10)
	   put_char1(( value/10)%10 + '0');

   put_char1( value%10 + '0');
	put_char1( delimiter);
}

/***************************************************************************/
/*	get_char1																	  SERIAL1.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960903										*/
/* Revised:		971022				     													*/
/*																									*/
/*	Function:	 																				*/
/*	Remarks:																						*/
/*	Returns:					 																	*/
/*	Updates:																						*/
/***************************************************************************/
char get_char1( void) {

	if ( RxBuf1.Input != RxBuf1.Output)
		return( RxBuf1.Buffer[RxBuf1.Output = ++RxBuf1.Output % FifoLength]);
	else
		return( 0);
}

/***************************************************************************/
/*	SndChar																		  SERIAL1.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960903										*/
/* Revised:		971124, KEn, DEV     													*/
/*																									*/
/*	Function:	Transmit command: <ccx;>												*/
/*	Remarks:		x is an unsigned char													*/
/*	Returns:		0: OK																			*/
/*					1: Unit reports a checksum error in command						*/
/*					2: No acknowledge from unit											*/
/*	Updates:		--																				*/
/***************************************************************************/
UC SndChar( UC adr, char *A, UC x) {

	put_addr1( adr);						// Send address, clear checksum
	put_char1( *A++);						// Send command
	put_char1( *A);

	put_value( ';', ( UL) x);			// Send data
	put_char1( cksum & 0x7f);

	CmdToSend[CmdIndex] = 0;			// End the command string
	rstate = 1;								// State for 'wait for answer'
	request = 0;							// Request flag off

	return( waitanswer());				// Wait for answer (with re-send)
}

/***************************************************************************/
/*	SndInt																		  SERIAL1.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960903										*/
/* Revised:		971124, KEn, DEV     													*/
/*																									*/
/*	Function:	Transmit command: <ccx;>												*/
/*	Remarks:		x is an unsigned integer												*/
/*	Returns:		0: OK																			*/
/*					1: Unit reports a checksum error in command						*/
/*					2: No acknowledge from unit											*/
/*	Updates:		--																				*/
/***************************************************************************/
UC SndInt( UC adr, char *A, UI x) {

	put_addr1( adr);						// Send address, clear checksum
	put_char1( *A++);						// Send command
	put_char1( *A);

	put_value( ';', ( UL) x);			// Send data
	put_char1( cksum & 0x7f);

	CmdToSend[CmdIndex] = 0;			// End the command string
	rstate = 1;								// State for 'wait for answer'
	request = 0;							// Request flag off

	return( waitanswer());				// Wait for answer (with re-send)
}

/***************************************************************************/
/*	SndInt2																		  SERIAL1.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960903										*/
/* Revised:		971124, KEn, DEV     													*/
/*																									*/
/*	Function:	Transmit command: <ccx,y;>												*/
/*	Remarks:		x,y are unsigned integers												*/
/*	Returns:		0: OK																			*/
/*					1: Unit reports a checksum error in command						*/
/*					2: No acknowledge from unit											*/
/*	Updates:		--																				*/
/***************************************************************************/
UC SndInt2( UC adr, char *A, UI x, UI y) {

	put_addr1( adr);						// Send address, clear checksum
	put_char1( *A++);						// Send command
	put_char1( *A);

	put_value( ',', ( UL) x);			// Send data
	put_value( ';', ( UL) y);
	put_char1( cksum & 0x7f);

	CmdToSend[CmdIndex] = 0;			// End the command string
	rstate = 1;								// State for 'wait for answer'
	request = 0;							// Request flag off

	return( waitanswer());				// Wait for answer (with re-send)
}

/***************************************************************************/
/*	SndInt3																		  SERIAL1.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 971030										*/
/* Revised:		971124, KEn, DEV     													*/
/*																									*/
/*	Function:	Transmit command: <ccx,y,z;>											*/
/*	Remarks:		x,y,z are unsigned integers											*/
/*	Returns:		0: OK																			*/
/*					1: Unit reports a checksum error in command						*/
/*					2: No acknowledge from unit											*/
/*	Updates:		--																				*/
/***************************************************************************/
UC SndInt3( UC adr, char *A, UI x, UI y, UI z) {

	put_addr1( adr);						// Send address, clear checksum
	put_char1( *A++);						// Send command
	put_char1( *A);

	put_value( ',', ( UL) x);			// Send data
	put_value( ',', ( UL) y);
	put_value( ';', ( UL) z);
	put_char1( cksum & 0x7f);

	CmdToSend[CmdIndex] = 0;			// End the command string
	rstate = 1;								// State for 'wait for answer'
	request = 0;							// Request flag off

	return( waitanswer());				// Wait for answer (with re-send)
}

/***************************************************************************/
/*	SndLong																		  SERIAL1.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960903										*/
/* Revised:		971124, KEn, DEV	     													*/
/*																									*/
/*	Function:	Transmit command: <ccl;>												*/
/*	Remarks:		l is an unsigned long, ie. 0-4294967295							*/
/*	Returns:		0: OK																			*/
/*					1: Unit reports a checksum error in command						*/
/*					2: No acknowledge from unit											*/
/*	Updates:																						*/
/***************************************************************************/
UC SndLong( UC adr, char *A, UL x) {

	put_addr1( adr);						// Send address, clear checksum
	put_char1( *A++);						// Send command
	put_char1( *A);

	put_value( ';', x);					// Send data
	put_char1( cksum & 0x7f);

	CmdToSend[CmdIndex] = 0;			// End the command string
	rstate = 1;								// State for 'wait for answer'
	request = 0;							// Request flag off

	return( waitanswer());				// Wait for answer (with re-send)
}

/***************************************************************************/
/*	SndVoid																		  SERIAL1.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960903										*/
/* Revised:		971024, KEn, DEV     													*/
/*																									*/
/*	Function:	Transmit command: <cc;>													*/
/*	Remarks:		--																				*/
/*	Returns:		0: OK																			*/
/*					1: Unit reports a checksum error in command						*/
/*					2: No acknowledge from unit											*/
/*	Updates:		--																				*/
/***************************************************************************/
UC SndVoid( UC adr, char *A) {

	put_addr1( adr);						// Send address, clear checksum
	put_char1( *A++);						// Send command
	put_char1( *A);

	put_char1( ';');
	put_char1( cksum & 0x7f);

	CmdToSend[CmdIndex] = 0;			// End the command string
	rstate = 1;								// State for 'wait for answer'
	request = 0;							// Request flag off

	return( waitanswer());				// Wait for answer (with re-send)
}

/***************************************************************************/
/*	SndAsc																		  SERIAL1.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960903										*/
/* Revised:		971024, KEn, DEV     													*/
/*																									*/
/*	Function:	Transmit command: <cc'aa..a'> or <ccn,'aa..a'>					*/
/*	Remarks:		if (n > 9): <cc'aa..'> otherwise <ccn,'aa..a'>					*/
/*					strlen( aa..a) <= 16														*/
/*	Returns:		0: OK																			*/
/*					1: Unit reports a checksum error in command						*/
/*					2: No acknowledge from unit											*/
/*	Updates:		cksum																			*/
/***************************************************************************/
UC SndAsc( UC adr, char *A, UC n, const char *st) {

	UC i;

	put_addr1( adr);						// Send address, clear checksum
	put_char1( *A++);						// Send command
	put_char1( *A);

	if ( n <= 9) {
		put_char1( n + '0');
		put_char1( ',');
	}

	put_char1( 34);				// Quote

	i = 0;
	while (( st[i] != '\0') && ( i < 16))
		put_char1( st[i++]);

	put_char1( 34);				// Quote
	put_char1( ';');
	put_char1( cksum & 0x7f);

	CmdToSend[CmdIndex] = 0;	// End the command string
	rstate = 1;						// State for 'wait for answer'
	request = 0;					// Request flag off

	return( waitanswer());		// Wait for answer (with re-send)
}

/***************************************************************************/
/*	SndRequest																	  SERIAL1.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960903										*/
/* Revised:		971203, KEn, DEV     													*/
/*																									*/
/*	Function:	Transmit command: <cc?> or <ccn?>									*/
/*	Remarks:		if ( n > 9): <cc?> otherwise <ccn?>									*/
/*	Returns:		0: OK																			*/
/*					1: Unit reports a checksum error in command						*/
/*					2: No acknowledge from unit											*/
/*	Updates:																						*/
/***************************************************************************/
UC SndRequest( UC adr, char *A, UC n) {

	put_addr1( adr);						// Send address, clear checksum
	put_char1( *A++);						// Send command
	put_char1( *A);

	if ( n <= 9)
		put_char1( n + '0');

	put_char1( '?');
	put_char1( cksum & 0x7f);

	CmdToSend[CmdIndex] = 0;	// End the command string
	rstate = 1;						// State for 'wait for answer'
	request = 1;					// Request flag on

	return( waitanswer());		// Wait for answer (with re-send)
}

/***************************************************************************/
/*	SndSpecial																	  SERIAL1.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960903										*/
/* Revised:		971024, KEn, DEV     													*/
/*																									*/
/*	Function:	Transmit command: <cca>													*/
/*	Remarks:		a is user definable														*/
/*	Returns:		0: OK																			*/
/*					1: Unit reports a checksum error in command						*/
/*					2: No acknowledge from unit											*/
/*	Updates:		--																				*/
/***************************************************************************/
UC SndSpecial( UC adr, char *A, char C) {

	put_addr1( adr);						// Send address, clear checksum
	put_char1( *A++);						// Send command
	put_char1( *A);

	put_char1( C);
	put_char1( cksum & 0x7f);

	CmdToSend[CmdIndex] = 0;			// End the command string
	rstate = 1;								// State for 'wait for answer'
	request = 0;							// Request flag off

	return( waitanswer());				// Wait for answer (with re-send)
}

/***************************************************************************/
/*	RecInt																		  SERIAL1.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 961023												*/
/* Revised:		971126				     													*/
/*																									*/
/*	Function:	--																				*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		error 0: OK																	*/
/*					      1: Unit reports a checksum error in command				*/
/*					      2: No acknowledge from unit									*/
/*					      3: checksum error in answer from unit						*/
/*					      4: No answer from unit											*/
/***************************************************************************/
UI RecInt( UC address, char* cmd, UC val, UC* error) {

	if (( *error = SndRequest( address, cmd, val)) == 0) {
		if (( *error = AnswerCheck()) == 0)
			return((UI) atoi( AnswerBuf));
		else
			*error += 2;
	}

	return( 0xFFFF);
}

/***************************************************************************/
/*	RecLong																		  SERIAL1.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 961023												*/
/* Revised:		971126, KEn, DEV	     													*/
/*																									*/
/*	Function:	--																				*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		error 0: OK																	*/
/*					      1: Unit reports a checksum error in command				*/
/*					      2: No acknowledge from unit									*/
/*					      3: checksum error in answer from unit						*/
/*					      4: No answer from unit											*/
/***************************************************************************/
UL RecLong( UC address, char* cmd, UC val, UC* error) {

	if (( *error = SndRequest( address, cmd, val)) == 0) {
		if (( *error = AnswerCheck()) == 0)
			return( (UL) atol( AnswerBuf));
		else
			*error += 2;
	}

	return( 0xFFFFFFFF);
}

/***************************************************************************/
/*	RecAsc																		  SERIAL1.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 961023												*/
/* Revised:		971029, KEn, DEV	     													*/
/*																									*/
/*	Function:	--																				*/
/*	Remarks:		Returns answer as received by unit									*/
/*	Returns:		NULL, if any error, otherwise a pointer to AnswerBuf			*/
/*	Updates:		error 0: OK																	*/
/*					      1: Unit reports a checksum error in command				*/
/*					      2: No acknowledge from unit									*/
/*					      3: checksum error in answer from unit						*/
/*					      4: No answer from unit											*/
/***************************************************************************/
char* RecAsc( UC address, char* cmd, UC val, UC* error) {

	if (( *error = SndRequest( address, cmd, val)) == 0) {
		if (( *error = AnswerCheck()) == 0)
			return( AnswerBuf);
		else
			*error += 2;
	}

	return( NULL);
}

/***************************************************************************/
/*	RecStr																		  SERIAL1.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 961031												*/
/* Revised:		971031, KEn, DEV	     													*/
/*																									*/
/*	Function:	--																				*/
/*	Remarks:		Removes quotes if contained in answer								*/
/*	Returns:		NULL, if any error, otherwise a pointer to AnswerBuf			*/
/*	Updates:		error 0: OK																	*/
/*					      1: Unit reports a checksum error in command				*/
/*					      2: No acknowledge from unit									*/
/*					      3: checksum error in answer from unit						*/
/*					      4: No answer from unit											*/
/***************************************************************************/
char* RecStr( UC address, char* cmd, UC val, UC* error) {

	UC tmp;

	if (( *error = SndRequest( address, cmd, val)) == 0) {
		if (( *error = AnswerCheck()) == 0) {

			tmp = strlen( AnswerBuf) - 1;						// Point to last entry

			if (( AnswerBuf[tmp] == '"') && ( AnswerBuf[0] == '"')) {

				AnswerBuf[tmp] = '\0';							// Remove quotes
				return( &AnswerBuf[1]);
			}
			return( AnswerBuf);
		}	
		else
			*error += 2;
	}

	return( NULL);
}
