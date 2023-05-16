#include <stdlib.h>
#include <dos.h>
#include <alloc.h>
#include <conio.h>

#include "pm39io.h"

#define _int1C 0x1C			/* Hardware interrupt for TIMETICK */

#define _ENABLE_INTR_8259	asm { mov al, 0x20; out 0x20, al }

/* ************************************************************************ */
/*                  VARIABLES FOR SERIAL PORT COM 1(3)                      */

void interrupt ( *_int01vect)();			/* Storage for default intr. COM1(3) */

static volatile char Data_Ready_COM1;	/* Data ready from sensor */
static volatile char Send_To_COM1;		/* PC sending to Sensor */

static char	 COM1_rsno;						/* Pointer counter */
static char* COM1_Ptr[4];					/* Pointer to actual COM1(3) string */

static char* COM1_Tmp_Str[4];				/* Temporary data from sensor */
static char* COM1_Str[4];					/* Available data from sensor */

static int _COM1_RBR;	/* Receiver Buffer Register */
static int _COM1_THR;	/* Transmitter Holding Register */
static int _COM1_IER;	/* Interrupt Enable Register */
static int _COM1_IIR;	/* Interrupt Identity Register */
static int _COM1_LCR;	/* Line Control Register */
static int _COM1_MCR;	/* Modem Control Register */
static int _COM1_LSR;	/* Line Status Register */
static int _COM1_MSR;	/* Modem Status Register */

static int _COM1_intr;
static int _COM1_mask;

/*              END OFF VARIABLES FOR SERIAL PORT COM 1(3)                  */
/* ************************************************************************ */


/* ************************************************************************ */
/*                  VARIABLES FOR SERIAL PORT COM 2(4)                      */

void interrupt ( *_int02vect)();			/* Storage for default intr. COM2(4) */

static volatile char Data_Ready_COM2;	/* Data ready from sensor */
static volatile char Send_To_COM2;		/* PC sending to Sensor */

static char	 COM2_rsno;						/* Pointer counter */
static char* COM2_Ptr[4];					/* Pointer to actual COM2(4) string */

static char* COM2_Tmp_Str[4];				/* Temporary data from sensor */
static char* COM2_Str[4];					/* Available data from sensor */

static int _COM2_RBR;	/* Receiver Buffer Register */
static int _COM2_THR;	/* Transmitter Holding Register */
static int _COM2_IER;	/* Interrupt Enable Register */
static int _COM2_IIR;	/* Interrupt Identity Register */
static int _COM2_LCR;	/* Line Control Register */
static int _COM2_MCR;	/* Modem Control Register */
static int _COM2_LSR;	/* Line Status Register */
static int _COM2_MSR;	/* Modem Status Register */

static int _COM2_intr;
static int _COM2_mask;

/*              END OFF VARIABLES FOR SERIAL PORT COM 2(4)                  */
/* ************************************************************************ */


/* ************************************************************************ */
/*          VARIABLES COMMON TO BOTH SERIAL PORT COM1(3) AND COM2(4)        */

void interrupt ( *_int1Cvect)();		/* Storage for default intr. timetick */

static char* dest;						/* Aux. pointer */
static char* source;						/* Aux. pointer */

static volatile char ch;				/* Intr. aux. variable: character */
static volatile int i;					/* Intr. aux. variable: counter */
static volatile int ix;					/* Intr. aux. variable: counter */

static float sumXYZ;						/* Variables to calculate CIE values */
static float divisor;

volatile timeout_ok;						/* timeout_ok factor = 53,4 ms */

/* ************************************************************************ */

char* PM39IOErrorTxt[15] = {
	/* 00 */	"No error",
	/* 01 */	"Not enough memory to allocate variables",
	/* 02 */	"Serial COM port not supported",
	/* 03 */	"Serial COM port was not installed",
	/* 04 */	"Serial COM port already installed",
	/* 05 */	"Timeout transmitting data to serial COM port",
	/* 06 */	"Timeout receiving data from serial COM port",
	/* 07 */ "_TIME_TICK was not installed",
	/* 08 */	"_TIME_TICK already installed",
	/* 09 */	"Integration time out of range",
	/* 10 */ "White reference values NOT calculated",
	/* 11 */ "Overload",
	/* 12 */ "Lowlight",
	/* 13 */ "Wrong sensor",
	/* 14 */ "Wrong baudrate",
};

enum PM39IOerrcodes { noerr = 0, err1, err2, err3, err4, err5, err6,
	err7, err8, err9, err10, err11, err12, err13, err14 };

/* ************************************************************************ */


void interrupt _TIME_TICK() {
							/* _TIME_TICK decreases the GLOBAL variabel timeout_ok */
							/*	18,2 times per second, ( app. every 53 ms) */
	if ( timeout_ok)
		timeout_ok--;

	_ENABLE_INTR_8259;
}


void interrupt _COM1_Interrupt() {

	if ( !Send_To_COM1) {

		if (( ch = inportb( _COM1_RBR)) != ',') {		/* Read char from COM1 */

			*COM1_Ptr[COM1_rsno] = ch;

			if ( COM1_Ptr[COM1_rsno] < &COM1_Tmp_Str[COM1_rsno][19])
				COM1_Ptr[COM1_rsno]++;

			if ( ch == '\x0D') { 							/* Check end of message */

				*(--COM1_Ptr[COM1_rsno]) = '\0';			/* Delete 'return' */

				for ( COM1_rsno = ix = 0; ix<4; ix++) {
					*COM1_Ptr[ix] = '\x0';                 /* Add end of string */

					source = COM1_Ptr[ix] = COM1_Tmp_Str[ix];
					dest = COM1_Str[ix];

					while ( *dest++ = *source++)				/* Correct to use '=' */
						;
				}

				Data_Ready_COM1 = 0xFF;						/* Signal data are ready */
			}
		}
		else
			if ( ++COM1_rsno > 3)
				COM1_rsno = 0; 		  	  /* Comma detected, target next string */

		ch = inportb( _COM1_LSR);
	}
	_ENABLE_INTR_8259;
}


void interrupt _COM2_Interrupt() {

	if ( !Send_To_COM2) {

		if (( ch = inportb( _COM2_RBR)) != ',') {	  /* Read char from COM2 */

			*COM2_Ptr[COM2_rsno] = ch;

			if ( COM2_Ptr[COM2_rsno] < &COM2_Tmp_Str[COM2_rsno][19])
				COM2_Ptr[COM2_rsno]++;

			if ( ch == '\x0D') { 							/* Check end of message */

				*(--COM2_Ptr[COM2_rsno]) = '\0';		  	/* Delete 'return' */

				for ( COM2_rsno = ix = 0; ix<4; ix++) {
					*COM2_Ptr[ix] = '\x0';					  	/* Add end of string */

					source = COM2_Ptr[ix] = COM2_Tmp_Str[ix];
					dest = COM2_Str[ix];

					while ( *dest++ = *source++)				/* Correct to use '=' */
						;
				}

				Data_Ready_COM2 = 0xFF;						/* Signal data are ready */
			}
		}
		else
			if ( ++COM2_rsno > 3)
				COM2_rsno = 0; 		  	  /* Comma detected, target next string */

		ch = inportb( _COM2_LSR);
	}
	_ENABLE_INTR_8259;
}


int Command_To_COM( int COM_Port, char* str) {

	int _COM1_IER_tmp, _COM2_IER_tmp, tmp_error = noerr;

	_COM1_IER_tmp = inportb( _COM1_IER);
	_COM2_IER_tmp = inportb( _COM2_IER);

	outportb( _COM1_IER, 0x00);				/* Disable interrupt from COM1 */
	outportb( _COM2_IER, 0x00);				/* Disable interrupt from COM2 */

	timeout_ok = 8;

	switch ( COM_Port) {
		case COM1:
		case COM3:

			Send_To_COM1 = 0xFF;

			inportb( _COM1_IIR);

			while (( ch = *str++) && !tmp_error) {

				while ( !( inportb( _COM1_LSR) & 0x20) && timeout_ok)
					;

				outportb( _COM1_THR, ch);

				if ( !timeout_ok)
					tmp_error = err5;
			}

			for ( Send_To_COM1 = Data_Ready_COM1 = COM1_rsno = i = 0; i<4; i++)
				COM1_Ptr[i] = COM1_Tmp_Str[i];				/* Initiate pointers */

			inportb( _COM1_LSR);
			inportb( _COM1_RBR);
			inportb( _COM1_IIR);
			inportb( _COM1_MSR);

			outportb( _COM2_IER, _COM2_IER_tmp);
			outportb( _COM1_IER, 0x05);

			return( tmp_error);

		case COM2:
		case COM4:

			Send_To_COM2 = 0xFF;

			inportb( _COM2_IIR);

			while (( ch = *str++) && !tmp_error) {

				while ( !( inportb( _COM2_LSR) & 0x20) && timeout_ok)
					;

				outportb( _COM2_THR, ch);

				if ( !timeout_ok)
					tmp_error = err5;
			}

			for ( Send_To_COM2 = Data_Ready_COM2 = COM2_rsno = i = 0; i<4; i++)
				COM2_Ptr[i] = COM2_Tmp_Str[i];				/* Initiate pointers */

			inportb( _COM2_LSR);
			inportb( _COM2_RBR);
			inportb( _COM2_IIR);
			inportb( _COM2_MSR);

			outportb( _COM1_IER, _COM1_IER_tmp);
			outportb( _COM2_IER, 0x05);

			return( tmp_error);
	}

	outportb( _COM1_IER, _COM1_IER_tmp);
	outportb( _COM2_IER, _COM2_IER_tmp);

	return( err2);
}


int Open_COM( int COM_Port, int Baud_Rate) {

	int PortBase;
	struct REGPACK reg;

	switch ( Baud_Rate) {
		case Baud4800:
		case Baud9600:
			break;

		default:
			return( err14);
	}

	PortBase = *(( int far *)MK_FP( 0x40, 0) + COM_Port);

	switch ( COM_Port) {
		case COM1:
			if ( PortBase == 0)
				PortBase = 0x3F8;

			_COM1_intr = 0x0C;
			_COM1_mask = '\xEF';
			break;

		case COM2:
			if ( PortBase == 0)
				PortBase = 0x2F8;

			_COM2_intr = 0x0B;
			_COM2_mask = '\xF7';
			break;

		case COM3:
			if ( PortBase == 0x3220) {				/* If PC is a PS/2 */
				_COM1_intr = 0x0B;
				_COM1_mask = '\xF7';
			}
			else {
				if ( PortBase == 0)
					PortBase = 0x3E8;

				_COM1_intr = 0x0C;
				_COM1_mask = '\xEF';
			}
			break;

		case COM4:
			if ( PortBase == 0x3228) {				/* If PC is a PS/2 */
				_COM2_intr = 0x0C;
				_COM2_mask = '\xEF';
			}
			else {
				if ( PortBase == 0)
					PortBase = 0x2E8;

				_COM2_intr = 0x0B;
				_COM2_mask = '\xF7';
			}
			break;

		default:
			return( err2);
	}

	switch ( COM_Port) {
		case COM1:
		case COM3:
			if ( _COM1_Interrupt != getvect( _COM1_intr)) {
				_COM1_RBR = _COM1_THR = PortBase;

				_COM1_IER = PortBase + 1;
				_COM1_IIR = PortBase + 2;
				_COM1_LCR = PortBase + 3;
				_COM1_MCR = PortBase + 4;
				_COM1_LSR = PortBase + 5;
				_COM1_MSR = PortBase + 6;

				for ( i=0; i<4; i++) {
					if ( !( COM1_Tmp_Str[i] = ( char*) malloc(( char) 20)) ||
						  !( COM1_Str[i] = ( char*) malloc(( char) 20))) {

										/* Not enough memory to initialize variables */
						for ( i=0; i<4; i++) {
							free( COM1_Str[i]);
							free( COM1_Tmp_Str[i]);
						}
						return( err1);
					}
					COM1_Ptr[i] = COM1_Tmp_Str[i];
				}

				Send_To_COM1 = Data_Ready_COM1 = COM1_rsno = 0;

				/* Disable COM1(3) interrupt */
				outportb( 0x21, inportb( 0x21) | ~_COM1_mask);

				/* Test if specified port exists */
				outportb( _COM1_IER, 0x02);

				if ( inportb( _COM1_IER) != 0x02)
					return( 2);

				if ( Baud_Rate == Baud4800) {
					if ( PortBase == 0x3220) {		/* If PC is a PS/2 */
						reg.r_ax = 0x0400;			/* Subservice 4, no break */
						reg.r_bx = 0x0001;			/* NO parity, 2 stop */
						reg.r_cx = 0x0306;      	/* 8 data, 4800 baud */
						reg.r_dx = COM_Port;			/* Serial port COM1(3) */
					}
					else {
						reg.r_ax = 0xC7;		/* 4800 baud, 8 data, 2 stop, NO parity */
						reg.r_dx = 0x00;		/* Serial port COM1(3) */
					}
				}
				else {
					if ( PortBase == 0x3220) {		/* If PC is a PS/2 */
						reg.r_ax = 0x0400;			/* Subservice 4, no break */
						reg.r_bx = 0x0001;			/* NO parity, 2 stop */
						reg.r_cx = 0x0307;      	/* 8 data, 9600 baud */
						reg.r_dx = COM_Port;			/* Serial port COM1(3) */
					}
					else {
						reg.r_ax = 0xE7;		/* 9600 baud, 8 data, 2 stop, NO parity */
						reg.r_dx = 0x00;		/* Serial port COM1(3) */
					}
				}
				intr( 0x14, &reg);

				outportb( _COM1_MCR, 0x0B);		 	/* Clear DTR, RTS & OUT2 */

				inportb( _COM1_RBR);						/* Clear input register */
				inportb( _COM1_LSR);						/* Clear status register */
				inportb( _COM1_IIR);
				inportb( _COM1_MSR);
														/* Save the old interrupt vector */
				_int01vect = getvect( _COM1_intr);
														/* Install new interrupt handler */
				setvect( _COM1_intr, _COM1_Interrupt);

				/* Enable COM1(3) intr. in UART, i.e.
				  enable Received Data Available &  Receiver Line Status */
				outportb( _COM1_IER, 0x05);

				/* Enable COM1(3) interrupt */
				outportb( 0x21, inportb( 0x21) & _COM1_mask);

				_ENABLE_INTR_8259;

				return( noerr);
			}
			return( err4);

		case COM2:
		case COM4:
			if ( _COM2_Interrupt != getvect( _COM2_intr)) {

				_COM2_RBR = _COM2_THR = PortBase;

				_COM2_IER = PortBase + 1;
				_COM2_IIR = PortBase + 2;
				_COM2_LCR = PortBase + 3;
				_COM2_MCR = PortBase + 4;
				_COM2_LSR = PortBase + 5;
				_COM2_MSR = PortBase + 6;

				for ( i=0; i<4; i++) {
					if ( !( COM2_Tmp_Str[i] = ( char*) malloc(( char) 20)) ||
						  !( COM2_Str[i] = ( char*) malloc(( char) 20))) {

										/* Not enough memory to initialize variables */
						for ( i=0; i<4; i++) {
							free( COM2_Str[i]);
							free( COM2_Tmp_Str[i]);
						}
						return( err1);
					}
					COM2_Ptr[i] = COM2_Tmp_Str[i];
				}

				Send_To_COM2 = Data_Ready_COM2 = COM2_rsno = 0;

				/* Disable COM2(4) interrupt */
				outportb( 0x21, inportb( 0x21) | ~_COM2_mask);

				/* Test if specified port exists */
				outportb( _COM2_IER, 0x02);

				if ( inportb( _COM2_IER) != 0x02)
					return( 2);

				if ( Baud_Rate == Baud4800) {
					if ( PortBase == 0x3228) {		/* If PC is a PS/2 */
						reg.r_ax = 0x0400;			/* Subservice 4, no break */
						reg.r_bx = 0x0001;			/* NO parity, 2 stop */
						reg.r_cx = 0x0306;      	/* 8 data, 4800 baud */
						reg.r_dx = COM_Port;			/* Serial port COM2(4) */
					}
					else {
						reg.r_ax = 0xC7;		/* 4800 baud, 8 data, 2 stop, NO parity */
						reg.r_dx = 0x01;		/* Serial port COM2(4) */
					}
				}
				else {
					if ( PortBase == 0x3228) {		/* If PC is a PS/2 */
						reg.r_ax = 0x0400;			/* Subservice 4, no break */
						reg.r_bx = 0x0001;			/* NO parity, 2 stop */
						reg.r_cx = 0x0307;      	/* 8 data, 9600 baud */
						reg.r_dx = COM_Port;			/* Serial port COM2(4) */
					}
					else {
						reg.r_ax = 0xE7;		/* 9600 baud, 8 data, 2 stop, NO parity */
						reg.r_dx = 0x01;		/* Serial port COM2(4) */
					}
				}	
				intr( 0x14, &reg);

				outportb( _COM2_MCR, 0x0B);		/* Clear DTR, RTS & OUT2 */

				inportb( _COM2_RBR);					/* Clear input register */
				inportb( _COM2_LSR);					/* Clear status register */
				inportb( _COM2_IIR);
				inportb( _COM2_MSR);

				_int02vect = getvect( _COM2_intr);	/* Save the old interrupt vector */
				setvect( _COM2_intr, _COM2_Interrupt);	/* Install new interrupt handler */

				/* Enable COM2 intr. in UART, i.e.
					enable Received Data Available &  Receiver Line Status */
				outportb( _COM2_IER, 0x05);

				/* Enable COM2(4) interrupt */
				outportb( 0x21, inportb( 0x21) & _COM2_mask);

				_ENABLE_INTR_8259;

				return( noerr);
			}
			return( err4);
	}
}


int Close_COM( int COM_Port) {

	switch ( COM_Port) {
		case COM1:
		case COM3:
			if ( _COM1_Interrupt == getvect( _COM1_intr)) {
											/* Disable ALL serial intr. in UART COM1 */
				Stop_Measuring( COM1);
				outportb( _COM1_IER, 0x00);

				/* Disable interrupt */
				outportb( 0x21, inportb( 0x21) | ~_COM1_mask);

				inportb( _COM1_IIR);					/* Reset serial COM1 IIR */

				setvect( _COM1_intr, _int01vect);		/* Reset old interrupt handler */

				for ( i=0; i<4; i++) {				/* Free ALL allocated memory */
					free( COM1_Str[i]);
					free( COM1_Tmp_Str[i]);
				}
				return( noerr);
			}
			return( err3);

		case COM2:
		case COM4:
			if ( _COM2_Interrupt == getvect( _COM2_intr)) {

				Stop_Measuring( COM2);			/* Disable ALL serial intr. in UART COM2 */
				outportb( _COM2_IER, 0x00);

				/* Disable interrupt */
				outportb( 0x21, inportb( 0x21) | ~_COM2_mask);

				inportb( _COM2_IIR);					/* Reset serial COM2 IIR */

				setvect( _COM2_intr, _int02vect);		/* Reset old interrupt handler */

				for ( i=0; i<4; i++) {				/* Free ALL allocated memory */
					free( COM2_Str[i]);
					free( COM2_Tmp_Str[i]);
				}
				return( noerr);
			}
			return( err3);
	}
	return( err2);
}


int Open_TIME_TICK( void) {

	if ( _TIME_TICK != getvect( _int1C)) {

		_int1Cvect = getvect( _int1C);		/* Save the old interrupt vector */
		setvect( _int1C, _TIME_TICK);			/* Install new interrupt handler */

		return( noerr);
	}
	return( err8);
}


int Close_TIME_TICK( void) {

	if ( _TIME_TICK == getvect( _int1C)) {

		setvect( _int1C, _int1Cvect);				/* Reset old interrupt handler */
		return( noerr);
	}
	return( err7);
}


int Start_Measuring( int COM_Port) {

	return( Command_To_COM( COM_Port, "MC;"));
}


int Stop_Measuring( int COM_Port) {

	return( Command_To_COM( COM_Port, "MS;"));
}


int Set_Integration_Time( int COM_Port, int time) {

	char str[8], str1[4];

	if (( time < 25) || ( time > 250))
		return( err9);

	strcpy( str, "SI");

	itoa( time, str1, 10);

	strcat( str, str1);
	strcat( str, ";");

	return( Command_To_COM( COM_Port, str));
}


float Get_Integration_Time( int COM_Port) {

	int tmp_error;

	if ( tmp_error = Command_To_COM( COM_Port, "F?;"))
		return( -tmp_error);

	switch ( COM_Port) {
		case COM1:
		case COM3:
			timeout_ok = 8;

			while ( !Data_Ready_COM1 && timeout_ok)
				;

			return( timeout_ok ? atof( COM1_Str[0]) : -err6);

		case COM2:
		case COM4:
			timeout_ok = 8;

			while ( !Data_Ready_COM2 && timeout_ok)
				;

			return( timeout_ok ? atof( COM2_Str[0]) : -err6);
	}

	return( -err2);
}


int Get_ID( int COM_Port, IDINFO* SensorID) {

	int tmp_error;

	if ( tmp_error = Command_To_COM( COM_Port, "I?;"))
		return( tmp_error);

	switch ( COM_Port) {
		case COM1:
		case COM3:
			timeout_ok = 8;

			while ( !Data_Ready_COM1 && timeout_ok)
				;

			if ( !timeout_ok)
				return( err6);

			strcpy( SensorID->Company, COM1_Str[0]);
			strcpy( SensorID->nc, COM1_Str[1]);
			strcpy( SensorID->KuNo, COM1_Str[2]);
			strcpy( SensorID->SoftWareRev, COM1_Str[3]);

			if ( tmp_error = Command_To_COM( COM_Port, "MA61;"))
				return( tmp_error);

			delay(1);
			if ( tmp_error = Command_To_COM( COM_Port, "RM;"))
				return( tmp_error);

			timeout_ok = 8;

			while ( !Data_Ready_COM1 && timeout_ok)
				;

			if ( !timeout_ok)
				return( err6);

			SensorID->SensorType = tmp_error = atoi( COM1_Str[0]);

			if ((tmp_error == 0) || ( tmp_error == 16))
				return( err13);
			else
				return( noerr);

		case COM2:
		case COM4:
			timeout_ok = 8;

			while ( !Data_Ready_COM2 && timeout_ok)
				;

			if ( !timeout_ok)
				return( err6);

			strcpy( SensorID->Company, COM2_Str[0]);
			strcpy( SensorID->nc, COM2_Str[1]);
			strcpy( SensorID->KuNo, COM2_Str[2]);
			strcpy( SensorID->SoftWareRev, COM2_Str[3]);

			if ( tmp_error = Command_To_COM( COM_Port, "MA61;"))
				return( tmp_error);

			if ( tmp_error = Command_To_COM( COM_Port, "RM;"))
				return( tmp_error);

			timeout_ok = 8;

			while ( !Data_Ready_COM2 && timeout_ok)
				;

			if ( !timeout_ok)
				return( err6);

			SensorID->SensorType = tmp_error = atoi( COM2_Str[0]);

			if ((tmp_error == 0) || ( tmp_error == 16))
				return( err13);
			else
				return( noerr);
	}

	return( err2);
}


int Get_Measurement( int COM_Port, MEASINFO* Meas) {

	timeout_ok = 50;

	switch ( COM_Port) {
		case COM1:
		case COM3:

			while ( !Data_Ready_COM1 && timeout_ok)
				;

			if ( !timeout_ok)
				return( Meas->IOerror = err6);

			Meas->X = atof( COM1_Str[0]);
			Meas->Y = atof( COM1_Str[1]);
			Meas->Z = atof( COM1_Str[2]);

			Data_Ready_COM1 = 0;
			break;

		case COM2:
		case COM4:

			while ( !Data_Ready_COM2 && timeout_ok)
					;

			if ( !timeout_ok)
				return( Meas->IOerror = err6);

			Meas->X = atof( COM2_Str[0]);
			Meas->Y = atof( COM2_Str[1]);
			Meas->Z = atof( COM2_Str[2]);

			Data_Ready_COM2 = 0;
			break;

		default:
			return( Meas->IOerror = err2);
	}

	if ( Meas->X < -0.5) {
		Meas->X = Meas->Y = Meas->Z = 0;
		return( Meas->IOerror = err11);
	}

	if (( Meas->X < 0.01) || ( Meas->Y < 0.01) || ( Meas->Z < 0.01)) {
		Meas->X = Meas->Y = Meas->Z = 0;
		return( Meas->IOerror = err12);
	}

	sumXYZ = Meas->X + Meas->Y + Meas->Z;
	divisor = Meas->X + 15*Meas->Y + 3*Meas->Z;

	if (( sumXYZ > 0.01) && ( divisor > 0.01)) {
		Meas->whr[0] = Meas->X/sumXYZ;
		Meas->whr[1] = Meas->Y/sumXYZ;

		Meas->whr[2] = 4.*Meas->X/divisor;
		Meas->whr[3] = 9.*Meas->Y/divisor;

		Meas->whr[4] = Meas->whr[2];
		Meas->whr[5] = 6*Meas->Y/divisor;

		return( Meas->IOerror = noerr);
	}
	else
		return( Meas->IOerror = err10);
}
