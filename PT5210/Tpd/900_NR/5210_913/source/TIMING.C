/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		TIMING.C																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	970211																		*/
/*	Rev. date:	971211, KEn DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*																									*/
/*	Changes:																						*/
/* 970701 Added ONLY text explanations													*/
/* 970206 Added password protection														*/
/* 970129 Added errorcode: HardwareNotpresentErr									*/
/* 970122 Added Audio:Read, ADC:Read & levreldetector:read						*/
/* 970109 Problems with suffix in BB & SB corrected								*/
/*			 Added usertext in main ram													*/
/* 970106 Remove FactoryReset																*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mas.h"

#include "52sync.h"
#include "timing.h"
#include "menu.h"
#include "menuhlp.h"
#include "menutree.h"
#include "xaexprt.h"
#include "xadrivr.h"
#include "tables.h"
#include "text.h"
#include "disp_drv.h"

#define labs(c)   ((c) >= 0 ? (c) : (-c))

static char TextBuffer[10];

/***************************************************************************/
/* AnalogFLTToSamples														  	TIMING.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970211	    										*/
/* Revised:		970211				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:	FLT in samples, or -1 if illegal											*/
/*	Updates:																						*/
/***************************************************************************/
long AnalogFLTToSamples( UC system, char F, int L, float T) {

	long samples;
	char minus;

	if (( F == 0) && ( L == 0) && ( T == 0))
		return( 0);

	if (( F <= 0) && ( L <= 0) && ( T <= 0)) {
		minus = 1;
	}
	else {
		if (( F >= 0) && ( L >= 0) && ( T >= 0))
			minus = 0;
		else
			return( -1);
	}

														// 1 sample = (1/27MHz)/256
	samples = ( long)(( T*6912)/1000 + 0.5);	

	if ( system < NTSC) {						// If selected system is PAL..

		if ( labs( samples) > 442367)			// Test for illegal time var
			return( -1);

		switch ( F) {								// Test for illegal field & line
			case 0:
				if (( minus && ( L < -311)) || ( !minus && ( L > 312)))
					return( -1);
				break;

			case -1:
			case -3:
			case 2:
				if ( abs( L) > 312)
					return( -1);
				break;

			case -2:
			case 1:
			case 3:
				if ( abs( L) > 311)
					return( -1);
				break;

			case 4:
				if (( L > 0) || ( T > 0.0))
					return( -1);
				break;

			default:
				return( -1);
		}

		if ( !minus) {								// if posistive delay..
			if ( F % 2) {
				L += 313;
				F--;
			}
		}
		else {										// ..else delay is negative
			F += 7;

			samples += 442367;

			if ( F % 2) {
				L += 624;
				F--;
			}
			else
				L += 312;

			if ( T == 0)
				samples++;
		}

		samples += F*138240000 + L*442368;
	}
	else {											// ..else system is NTSC

		if ( labs( samples) > 439295)			// Test for illegal time var
			return( -1);

		switch ( F) {								// Test for illegal field & line
			case 0:
				if (( minus && ( L < -261)) || ( !minus && ( L > 262)))
					return( -1);
				break;

			case -1:
				if ( abs( L) > 262)
					return( -1);
				break;

			case 1:
				if ( abs( L) > 261)
					return( -1);
				break;

			case 2:
				if (( L > 0) || ( T > 0.0))
					return( -1);
				break;

			default:
				return( -1);
		}

		if ( !minus) {								// if posistive delay..

			if ( F % 2) {
				L += 263;
				F--;
			}
		}
		else {										// ..else delay is negative
			F += 3;

			samples += 439295;

			if ( F % 2) {
				L += 524;
				F--;
			}
			else
				L += 262;

			if ( T == 0)
				samples++;
		}

		samples += F*115315200 + L*439296;
	}

	return( samples);
}

/***************************************************************************/
/* AnalogSamplesToFLT														  	TIMING.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970211	    										*/
/* Revised:		970211				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void AnalogSamplesToFLT( UC system, long samples,\
												 char* sign, char* F, int* L, float* T) {

	long k, l=samples;
	char frame, field;
	int line;

	*sign = '+';

	if ( system < NTSC) {								// If system selected is PAL..

		if ( samples >=  ((long) 2*625*1728*256)) {

			samples--;

			frame = samples / ((long) 625*1728*256);
			line = ( samples % ((long) 625*1728*256)) / ((long) 1728*256);
			samples = ( samples % ((long) 625*1728*256)) % ((long) 1728*256);

			field = 2*frame;

			if ( line > 312) {
				field++;
				line = 624-line;
			}
			else
				line = 312-line;

			samples = ((long) 1728*256) - 1 - samples;
			field = 7-field;

			if ( field != 4)
				*sign = '-';
		}
		else {
			frame = samples / ((long) 625*1728*256);
			line = ( samples % ((long) 625*1728*256)) / ((long) 1728*256);
			samples = ( samples % ((long) 625*1728*256)) % ((long) 1728*256);
			field = 2*frame;

			if ( line > 312) {
				field++;
				line -= 313;
			}
		}
	}
	else {                  							// ..system selected is NTSC
		if ( samples >=  ((long) 525*1716*256)) {

			samples--;

			frame = samples / ((long) 525*1716*256);
			line = ( samples % ((long) 525*1716*256)) / ((long) 1716*256);
			samples = ( samples % ((long) 525*1716*256)) % ((long) 1716*256);

			field = 2*frame;

			if ( line > 262) {
				field++;
				line = 524-line;
			}
			else
				line = 262-line;

			samples = ((long) 1716*256) - 1 - samples;
			field = 3 - field;

			if ( field != 2)
				*sign = '-';
		}
		else {
			frame = samples / ((long) 525*1716*256);
			line = ( samples % ((long) 525*1716*256)) / ((long) 1716*256);
			samples = ( samples % ((long) 525*1716*256)) % ((long) 1716*256);
			field = 2*frame;

			if ( line > 262) {
				field++;
				line -= 263;
			}
		}
	}

	*L = abs( line);
	*F = abs( field);
	*T = fabs( (float) ( samples*1000)/6912);
}

/***************************************************************************/
/* SDIFLTToSamples															  	TIMING.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970211	    										*/
/* Revised:		970211				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
long SDIFLTToSamples( UC system, char F, int L, float T) {

	long samples, line_samples;
	char minus;

	if (( F == 0) && ( L == 0) && ( T == 0))
		return( 0);

	if (( F <= 0) && ( L <= 0) && ( T <= 0))
		minus = 1;
	else {
		if (( F >= 0) && ( L >= 0) && ( T >= 0))
			minus = 0;
		else
			return( -1);
	}

	samples = ( long)(( T*6912)/1000 + 0.5);

	if ( system == SDI625) {					// If selected system is PAL..

		if ( labs( samples) > 442367)			// Test for illegal time var
			return( -1);

		switch ( F) {								// Test for illegal field & line
			case 0:
				if (( minus && ( L < -313)) || ( !minus && ( L > 312)))
					return( -1);
				break;

			case 1:
				if (( L > 0) || ( T > 0.0))
					return( -1);
				break;

			default:
				return( -1);
		}

		if ( !minus) {								// if posistive delay..
			if ( F % 2) {
				L += 312;
				F--;
			}
		}
		else {										// ..else delay is negative
			F += 1;

			samples += 442367;

			if ( F % 2) {
				L += 624;
				F--;
			}
			else
				L += 313;

			if ( T == 0)
				samples++;
		}

		samples += F*138240000 + L*442368;
	}
	else {											// ..else system is NTSC

		if ( labs( samples) > 439295)			// Test for illegal time var
			return( -1);

		switch ( F) {								// Test for illegal field & line
			case 0:
				if (( minus && ( L < -263)) || ( !minus && ( L > 262)))
					return( -1);
				break;

			case 1:
				if (( L > 0) || ( T > 0.0))
					return( -1);
				break;

			default:
				return( -1);
		}

		if ( !minus) {								// if positive delay..
			if ( F % 2) {
				L += 262;
				F--;
			}
		}
		else {										// ..else delay is negative
			F += 1;

			samples += 439295;

			if ( F % 2) {
				L += 524;
				F--;
			}
			else
				L += 263;

			if ( T == 0)
				samples++;
		}

		samples += F*115315200 + L*439296;
	}

	return( samples);
}

/***************************************************************************/
/* SDISamplesToFLT															  	TIMING.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970211	    										*/
/* Revised:		970211				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SDISamplesToFLT( UC system, long samples,\
												 char* sign, char* F, int* L, float* T) {

	long l = samples;   // TO BE REMOVED
	char field;
	int line;

	*sign = '+';

	if ( system == SDI625) {						// If system selected is SDI625..

		if ( samples >=  ((long) 312*1728*256)) {

			samples--;

			line = ( samples % ((long) 625*1728*256)) / ((long) 1728*256);
			samples = ( samples % ((long) 625*1728*256)) % ((long) 1728*256);

			field = 0;

			if ( line > 311) {
				field++;
				line = 624-line;
			}
			else
				line = 311-line;

			samples = ((long) 1728*256) - 1 - samples;
			field = 1-field;

			if ( field != 1)
				*sign = '-';
		}
		else {
			line = ( samples % ((long) 625*1728*256)) / ((long) 1728*256);
			samples = ( samples % ((long) 625*1728*256)) % ((long) 1728*256);
			field = 0;

			if ( line > 311) {
				field++;
				line -= 312;
			}
		}
	}
	else {                  							// ..system selected is SI525
		if ( samples >=  ((long) 262*1716*256)) {

			samples--;

			line = ( samples % ((long) 525*1716*256)) / ((long) 1716*256);
			samples = ( samples % ((long) 525*1716*256)) % ((long) 1716*256);

			field = 0;

			if ( line > 261) {
				field++;
				line = 524-line;
			}
			else
				line = 261-line;

			samples = ((long) 1716*256) - 1 - samples;
			field = 1 - field;

			if ( field != 1)
				*sign = '-';
		}
		else {
			line = ( samples % ((long) 525*1716*256)) / ((long) 1716*256);
			samples = ( samples % ((long) 525*1716*256)) % ((long) 1716*256);
			field = 0;

			if ( line > 261) {
				field++;
				line -= 262;
			}
		}
	}

	*L = abs( line);
	*F = abs( field);
	*T = fabs( (float) ( samples*1000)/6912);
}

/***************************************************************************/
/* WriteTimingFLT																  	TIMING.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970211	    										*/
/* Revised:		970211				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void WriteTimingFLT( char sign, char field, int line, float time) {

	WriteCodeLN2( 0, TimeVHdrTxt);			// Write V: (ie. field) header
	sprintf( TextBuffer, "%2d", field);		// Copy field number to buffer
	TextBuffer[0] = sign;						// Add sign to field number
	WriteTxt2( TextBuffer);						// Write field number

	WriteCode2( TimeHHdrTxt);					// Write H: (ie. line) header
	sprintf( TextBuffer, "%04d", line);		// Copy line number to buffer
	TextBuffer[0] = sign;						// Add sign to line no
	WriteTxt2( TextBuffer);						// Write line number

	WriteCode2( TimeTHdrTxt);					// Write T: (ie. ns) header
														// Copy samples (in ns) to buffer
	sprintf( TextBuffer, "%08.1f", time);
	TextBuffer[0] = sign;						// Add sign to time value
	WriteTxt2( TextBuffer);						// Write time value

	ClearRestLN2();								// Clear rest of line 2
}

/***************************************************************************/
/* PrintTimingFLT																   TIMING.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970211	    										*/
/* Revised:		970211				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void PrintTimingFLT( char sign, char field, int line, float time) {

														// Copy field number to buffer
	sprintf( TextBuffer, "%2d", field);
	TextBuffer[0] = sign;						// Add sign to field number
	Udr_PutNChar( TextBuffer);
	Udr_PutChar( ',');

														// Copy line number to buffer
	sprintf( TextBuffer, "%04d", line);
	TextBuffer[0] = sign;						// Add sign to line no
	Udr_PutNChar( TextBuffer);
	Udr_PutChar( ',');

														// Copy samples (in ns) to buffer
	sprintf( TextBuffer, "%08.1f", time);
	TextBuffer[0] = sign;						// Add sign to time value
	Udr_PutNChar( TextBuffer);
}
