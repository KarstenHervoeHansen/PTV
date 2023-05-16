/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PM5639/20 Industrial Color Analyzer									*/
/*	Module:		FILES.C																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	------																		*/
/*	Rev. date:	970826, KEn	DEV															*/
/*	Status:		Version -.-																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*	Changes:																						*/
/* 970826:	Added new text for PM5639/25 Miniature CRT sensor					*/
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include <dos.h>
#include <string.h>
#include <alloc.h>
#include <bios.h>
#include <dir.h>
#include <ctype.h>
#include <math.h>
#include "pm5639.h"

#pragma hdrstop

#include "files.h"

extern const char* KUNUMBER;
extern float SoftWareNO;
extern char* SoftwareType;

static char PrtTxt[3][65];

static int PrtNoWin;

static BUTTON Prtbu[4];
static EDITFIELD Prtef[3];
static FILE* stream;

static char errbuf[129];
static int diskErr;

static char SR_drives[28];
static char SR_filepath[129];

static char SR_buf[129];
static char SR_name[16];
static char SR_filename[16];
static char SR_drive[MAXDRIVE];
static char SR_dir[MAXDIR];
static char SR_txt[25];

static void interrupt ( *oldHarderr)();
static FILE* fp;
static float readSoftWareNO;
static int Checksum;
static int readChecksum;

static char* LPTText[] = {
	"LPT1", "LPT2"
};

static char* month[] = {
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December"
};

int SoftWareVersionOK( FILE* stream) {

	fread( &readSoftWareNO, sizeof( SoftWareNO), 1, stream);

	if ( readSoftWareNO >= 110)
		return( readSoftWareNO);

	return( 0);
}

void initFilesSystem( char* filNavn) {

	int i, j, save_disk;
	union REGS regs;

	for ( j=i=0; i<2; i++) {

		regs.h.ah = 0x15;
		regs.h.dl = i;

		int86( 0x13, &regs, &regs);

		if ( regs.h.ah != 0)
			SR_drives[j++] = 'A' + i;
	}

	save_disk = getdisk();

	for ( i= 2; i<26; i++) {
		setdisk( i);
		if ( i == getdisk())
			SR_drives[j++] = 'A' + i;
	}

	setdisk( save_disk);
	SR_drives[j] = 0;

	getcwd( SR_filepath, 128);

	if ( SR_filepath[strlen( SR_filepath) - 1] != '\\')
		strcat( SR_filepath, "\\");

	if ( filNavn[0] != '\x0')
		stream = openFile( filNavn, "rb", 19);
	else
		stream = openFile( CONFIG_FILE, "rb", -1);

	if ( stream != NULL) {

		if ( i = SoftWareVersionOK( stream)) {

			if (( strstr( filNavn, ".PGM") != NULL) && ( i < 200))
				;
			else
				fread( &FASTmode, sizeof( FASTmode), 1, stream);

			while ( !feof( stream))
				fread( &Win[++NoWin], sizeof( Win[0]), 1, stream);

			NoWin--;
		}
		else
			displayError( 41);

		fclose( stream);
	}


	for ( i=0; i < MAXWINDOW; i++) {
		WinPtr[i] = &Win[i];

		WinPtr[i]->WinNo = i;
		WinPtr[i]->running = INACTIVE;

		if ( i<= MAXDISPLAY) {
			CIECrossSize[i] = -1;
			CIECrossImage[i] = NULL;
		}
	}
}


int SaveRestoreWindow( int type, int mode, int helptxt) {

	WININFO WinTmp;
	int i, j, WinNoTmp, alive = -1;
	char* txtptr;

	if ( mode == RestoreMode)
		strcpy( SR_txt, "Restore");
	else
		strcpy( SR_txt, "Save");

	strcpy( SR_buf, SR_filepath);

	switch ( type) {
		case DisplayType:
			strcat( SR_buf, "*.DIS");
			strcat( SR_txt, " Display Setup");
			break;

		case ProgramType:
			strcat( SR_buf, "*.PGM");
			strcat( SR_txt, " Program Setup");
			break;

		case MeasurementType:
			strcat( SR_buf, "*.MSM");
			strcat( SR_txt, " Measurement");
			break;

		case FilePrintType:
			strcat( SR_buf, "*.PRN");
			strcpy( SR_txt, "File Report");
			break;
	}

	while ( alive) {
		if ( chooseFile( SR_buf, SR_name, SR_drives, SR_txt, helptxt, mode)) {
			fnsplit( SR_buf, SR_drive, SR_dir, NULL, NULL);
			fnsplit( SR_name, NULL, NULL, SR_filename, NULL);
			fnmerge( SR_filepath, SR_drive, SR_dir, NULL, NULL);

			strcpy( SR_buf, SR_filepath);
			strcat( SR_buf, SR_name);

			if ( mode == SaveMode) {

				if ( type == MeasurementType) {
					i = NoWin - 1;

					if (( WinPtr[i]->meas.X < 0.01) ||
						 ( WinPtr[i]->meas.Y < 0.01) ||
						 ( WinPtr[i]->meas.Z < 0.01) ) {
						displayError( 43);
						closeWindow();
						return( 0);
					}
				}

				if (( stream = openFile( SR_buf, "wb", 19)) != NULL){

					Checksum = 0;

					switch ( type) {
						case DisplayType:
							i = NoWin-1;

							strcpy( WinPtr[i]->windowname, SR_filename);
							strcat( WinPtr[i]->windowname, ".DIS");

							fwrite( &SoftWareNO, sizeof( SoftWareNO), 1, stream);
							fwrite( WinPtr[i], sizeof( Win[0]), 1, stream);

							txtptr = WinPtr[i];
							for ( j=0; j< sizeof( Win[0]); j++)
								Checksum += *txtptr++;
							break;

						case ProgramType:

							fwrite( &SoftWareNO, sizeof( SoftWareNO), 1, stream);
							fwrite( &FASTmode, sizeof( FASTmode), 1, stream);

							for ( i=1; i < NoWin; i++) {
								fwrite( WinPtr[i], sizeof( Win[0]), 1, stream);
								txtptr = WinPtr[i];

								for ( j=0; j< sizeof( Win[0]); j++)
									Checksum += *txtptr++;
							}
							break;

						case MeasurementType:
							i = NoWin - 1;

							CalcRGBReference( i);

							memcpy( &WinTmp, WinPtr[i], sizeof( WinTmp));

							switch ( WinPtr[i]->mode) {
								case CIEMode:
									WinTmp.mode = CIEMeasMode;
									break;
								case RGBMode:
									WinTmp.mode = RGBMeasMode;
									break;
							}

							strcpy( &WinTmp.windowname, SR_filename);
							strcat( &WinTmp.windowname, ".MSM");

							fwrite( &SoftWareNO, sizeof( SoftWareNO), 1, stream);
							fwrite( &WinTmp, sizeof( WinTmp), 1, stream);

							for ( j=0; j< sizeof( Win[0]); j++)
								Checksum += *txtptr++;
							break;

						case FilePrintType:
							printToMedia( stream);
							break;
					}
					fwrite( &Checksum, sizeof( Checksum), 1, stream);
					fclose( stream);
					alive = 0;
				}
			}
			else {
				if (( stream = openFile( SR_buf, "rb", 19)) != NULL) {

					if ( !(i = SoftWareVersionOK( stream)))
						displayError( 41);
					else
						switch ( type) {

							case DisplayType:
								WinNoTmp = WinPtr[NoWin]->WinNo;

								fread( WinPtr[NoWin], sizeof( Win[0]), 1, stream);

								WinPtr[NoWin]->running = INACTIVE;
								WinPtr[NoWin]->WinNo = WinNoTmp;

								clearSensorId( WinPtr[NoWin]);

								NoWin++;
								break;

							case ProgramType:
								if ( doCloseAll() == 0) {

									if ( i >= 200)
										fread( &FASTmode, sizeof( FASTmode), 1, stream);

									while ( !feof( stream)) {
										fread( WinPtr[++NoWin], sizeof( Win[0]), 1, stream);

										WinPtr[NoWin]->running = INACTIVE;
										WinPtr[NoWin]->WinNo = NoWin;

										clearSensorId( WinPtr[NoWin]);
									}
								}
								break;

							case MeasurementType:
								WinNoTmp = WinPtr[NoWin]->WinNo;

								fread( WinPtr[NoWin], sizeof( *WinPtr[0]), 1, stream);

								WinPtr[NoWin]->running = INACTIVE;
								WinPtr[NoWin]->WinNo = WinNoTmp;

								NoWin++;
								break;
						}
					fclose( stream);
					alive = 0;
				}
			}
		}
		else
			alive = 0;

		closeWindow();
	};
	return( 0);
}


int doSaveDisplay( void) {

	if ( NoWin)
		return( SaveRestoreWindow( DisplayType, SaveMode, HlpSaveDisplay));

	return( displayError( 0));
}


int doRestoreDisplay( void) {

	if ( NoWin < MAXDISPLAY)
		return( SaveRestoreWindow( DisplayType,
														RestoreMode, HlpRestoreDisplay));

	return( displayError( 1));
}


int doSaveProgram( void) {

	return( SaveRestoreWindow( ProgramType, SaveMode, HlpSaveProgram));
}


int doRestoreProgram( void) {

	return( SaveRestoreWindow( ProgramType, RestoreMode, HlpRestoreProgram));
}


int doSaveMeasurement( void) {

	if ( NoWin)
		return( SaveRestoreWindow( MeasurementType,
												SaveMode, HlpSaveMeasurement));
	return( displayError( 0));
}


int doRestoreMeasurement( void) {

	return( SaveRestoreWindow( MeasurementType,
												RestoreMode, HlpRestoreMeasurement));
}


void printToMedia( FILE* stream) {

	WININFO* oldP = WinPtr[PrtNoWin-1];

	int j, i;
	float d1, d2, deviation;
	struct date da;
	struct time ti;

	char txt[80];

	oldHarderr = getvect( 0x24);
	harderr( diskErrorHandler);

	diskErr = 0;

	/* Headline */

	fprintf( stream, "\r\n\r\n      Measuring report for the PM5639 ");

	if ( !strcmp( SoftwareType, "INDU"))
		fprintf( stream, "Industrial CRT Color Analyzer.\r\n\r\n");
	else {
		if ( !strcmp( SoftwareType, "SMLL"))
			fprintf( stream, "Miniature CRT Color Analyzer.\r\n\r\n");
		else
			fprintf( stream, "CRT Color Analyzer, PC.\r\n\r\n");
	}

	/* Date & Time */

	getdate( &da);
	gettime( &ti);

	strcpy( txt, "      Date:                               Time:   ");

	sprintf( &txt[12], "%02d. %s %4d", da.da_day, month[da.da_mon-1], da.da_year);
	sprintf( &txt[48], "%2d:%02d:%02d", ti.ti_hour, ti.ti_min, ti.ti_sec);
	txt[16+strlen( month[da.da_mon-1])] = ' ';
	txt[21+strlen( month[da.da_mon-1])] = ' ';
	fprintf( stream, "%s\r\n\r\n", txt);


	/* Attached CRT-sensor */

	fprintf( stream, "      Measuring configuration:\r\n");
	fprintf( stream, "      컴컴컴컴컴컴컴컴컴컴컴컴\r\n");

	fprintf( stream, "         PC Software Serial No.   : %s\r\n",
																KUNUMBER);
	fprintf( stream, "         Sensor Serial No.        : %s\r\n",
															oldP->sensorid.KuNo);
	fprintf( stream, "         Sensor Software revision : %s\r\n\r\n",
															oldP->sensorid.SoftWareRev);


	/* Measured X, Y & Z values */

	fprintf( stream, "      Measured XYZ-values:\r\n");
	fprintf( stream, "      컴컴컴컴컴컴컴컴컴컴\r\n");
	strcpy( txt, "         X =             Y =             Z =     ");
	sprintf( &txt[14], "%5.1f", oldP->meas.X);
	sprintf( &txt[30], "%5.1f", oldP->meas.Y);
	sprintf( &txt[47], "%5.1f", oldP->meas.Z);
	txt[14+5] = ' ';
	txt[30+5] = ' ';
	fprintf( stream, "%s\r\n\r\n", txt);


	/* Measured Luminance */

	fprintf( stream, "      Luminance:\r\n");
	fprintf( stream, "      컴컴컴컴컴\r\n");
	fprintf( stream, "         Y = ");
	fprintf( stream, "%5.1f", oldP->meas.Y);
	fprintf( stream, "  %s\r\n\r\n", LumText[oldP->lumunit]);


	/* Measured CIE values */

	fprintf( stream, "      Calculated CIE chromaticity coordinates:\r\n");
	fprintf( stream, "      컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴\r\n");
	fprintf( stream, "         CIE 1931        CIE 1976        CIE 1960\r\n");
	strcpy( txt, "         x  =            u' =            u  =     ");

	for ( j=14, i=0; i< 6; i += 2, j+=16)
		sprintf( &txt[j], "%5.3f", oldP->meas.whr[i]);

	txt[14+5] = ' ';
	txt[30+5] = ' ';
	fprintf( stream, "%s\r\n", txt);

	strcpy( txt, "         y  =            v' =            v  =     ");

	for ( j=14, i=1; i < 7; i += 2, j+=16)
		sprintf( &txt[j], "%5.3f", oldP->meas.whr[i]);

	txt[14+5] = ' ';
	txt[30+5] = ' ';
	fprintf( stream, "%s\r\n\r\n", txt);


	/* Difference between white-reference and measured CIE-values */

	fprintf( stream, "      Calculated differential CIE chromaticity coordinates:\r\n");
	fprintf( stream, "      컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�\r\n");
	fprintf( stream, "         CIE 1931        CIE 1976        CIE 1960\r\n");
	strcpy( txt, "         dx  =           du' =           du  =     ");

	for ( j=15, i=0; i< 6; i += 2, j+=16)
		sprintf( &txt[j], "%+5.3f", oldP->whrinfo.whr[i] - oldP->meas.whr[i]);

	txt[14+7] = ' ';
	txt[30+7] = ' ';
	fprintf( stream, "%s\r\n", txt);

	strcpy( txt, "         dy  =           dv' =           dv  =     ");

	for ( j=15, i=1; i < 7; i += 2, j+=16)
		sprintf( &txt[j], "%+5.3f", oldP->whrinfo.whr[i] - oldP->meas.whr[i]);

	txt[14+7] = ' ';
	txt[30+7] = ' ';
	fprintf( stream, "%s\r\n\r\n", txt);


	/* Color Error */

	fprintf( stream, "      Color Error:\r\n");
	fprintf( stream, "      컴컴컴컴컴컴컴컴컴컴컴컴\r\n");
	fprintf( stream, "         dE = ");

	if ( oldP->colorerror == CIELUV) {
		d1 = oldP->meas.whr[2] - oldP->whrinfo.whr[2];
		d2 = oldP->meas.whr[3] - oldP->whrinfo.whr[3];

		deviation = 1300. * sqrt( d1*d1 + d2*d2);
	}
	else {
		d1 = oldP->meas.whr[4] - oldP->whrinfo.whr[4];
		d2 = oldP->meas.whr[5] - oldP->whrinfo.whr[5];

		deviation = sqrt( d1*d1 + d2*d2) / 0.00384;
	}

	fprintf( stream, "%5.1f", deviation);
	fprintf( stream, "  %s\r\n\r\n", ColorErrText[oldP->colorerror]);


	/* Color Temperature */

	fprintf( stream, "      Correlated color temperature:\r\n");
	fprintf( stream, "      컴컴컴컴컴컴컴컴컴컴컴컴컴컴�\r\n");
	fprintf( stream, "         ccT = ");
	fprintf( stream, "%5.1f", oldP->ccT);
	fprintf( stream, "  Kelvin\r\n\r\n");


	/* Luminance Reference */

	fprintf( stream, "      Active Luminance reference:\r\n");
	fprintf( stream, "      컴컴컴컴컴컴컴컴컴컴컴컴컴�\r\n");

	if (( oldP->mode == RGBMode) && ( oldP->DispMode == refA)) {
		fprintf( stream, "         Y (ref) = ");
		fprintf( stream, "%5.1f", oldP->ABSref);
		fprintf( stream, "  %s\r\n\r\n", LumText[oldP->lumunit]);
	}
	else
		fprintf( stream, "         NA\r\n\r\n");


	/* Luminance Error */

	fprintf( stream, "      Luminance difference:\r\n");
	fprintf( stream, "      컴컴컴컴컴컴컴컴컴컴�\r\n");

	if (( oldP->mode == RGBMode) && ( oldP->DispMode == refA)) {
		fprintf( stream, "         dL = ");
		fprintf( stream, "%5.1f", oldP->ABSref - oldP->meas.Y);
		fprintf( stream, "  %s\r\n\r\n", LumText[oldP->lumunit]);
	}
	else
		fprintf( stream, "         NA\r\n\r\n");


	/* Active White reference */

	fprintf( stream, "      Active white reference:\r\n");
	fprintf( stream, "      컴컴컴컴컴컴컴컴컴컴컴�\r\n");
	fprintf( stream, "         CIE 1931        CIE 1976        CIE 1960\r\n");
	strcpy( txt, "         x  =            u' =            u  =     ");

	for ( j=14, i=0; i < 6; i += 2, j+=16)
		sprintf( &txt[j], "%5.3f", oldP->whrinfo.whr[i]);

	txt[14+5] = ' ';
	txt[30+5] = ' ';
	fprintf( stream, "%s\r\n", txt);

	strcpy( txt, "         y  =            v' =            v  =     ");

	for ( j=14, i=1; i < 7; i += 2, j+=16)
		sprintf( &txt[j], "%5.3f", oldP->whrinfo.whr[i]);

	txt[14+5] = ' ';
	txt[30+5] = ' ';
	fprintf( stream, "%s\r\n\r\n", txt);


	/* Three lines of user text */

	fprintf( stream, "      User text:\r\n");
	fprintf( stream, "      컴컴컴컴컴\r\n");
	for ( i=0; i<3; i++)
		fprintf( stream, "         %s\r\n", PrtTxt[i]);

	if ( WinPtr[PrtNoWin+1]->mode == PrintPrintMode)
		fprintf( stream, "\f");

	setvect( 0x24, oldHarderr);
}


void drawFilePrintWindow( void) {

	WININFO* newP = WinPtr[PrtNoWin];
	int i;

	newP->mode = HelpMode;
	openWindow( newP, "Generate Report");

	setcolor( EGA_BLACK);

	outtextxy( PrtLeft+7, PrtTop+20, "Generate a measure report");

	outtextxy( PrtLeft+7, PrtTop+70, "User Text Line 1: ");
	outtextxy( PrtLeft+7, PrtTop+97, "User Text Line 2: ");
	outtextxy( PrtLeft+7, PrtTop+124, "User Text Line 3: ");

	for ( i=1; i<4; i++) {
		drawEditfield( &Prtef[i-1], INACTIVE);
		drawButton( &Prtbu[i], INACTIVE);
	}

	drawButton( &Prtbu[0], INACTIVE);
}


int printFilePrint( void) {

	WININFO* newP = WinPtr[++NoWin];

	BUTTON bu[3];
	CHECKBOX cb[2];
	void* Image;

	int ch, i, alive = -1, a_inner, group, top;
	int portnumber, status;

	newP->mode = PrintPrintMode;
	SetRect( &newP->rect, PrtPrtLeft, PrtPrtTop, PrtPrtRight, PrtPrtBottom);

	addButtons( &bu[0], &bu[1], &bu[2], &newP->rect, Ok);

	addCheckbox( &cb[0], PrtPrtLeft+32,
										PrtPrtTop+WINDOWBARDEEP+22, "Output to LPT1");
	addCheckbox( &cb[1], PrtPrtLeft+32,
										PrtPrtTop+WINDOWBARDEEP+36, "Output to LPT2");
	cb[1].select = 0;
	cb[0].select = 0xFF;

	group = 2;
	drawCheckbox( &cb[0], INACTIVE);
	drawCheckbox( &cb[1], INACTIVE);

	while( alive) {
		openWindow( newP, "Print Report");

		drawSetupRect( PrtPrtLeft+24, PrtPrtTop+WINDOWBARDEEP+12,
															2, "Printer Output", 20*8);
		for ( i=0; i < 3; i++)
			drawButton( &bu[i], INACTIVE);

		drawCheckbox( &cb[0], INACTIVE);
		drawCheckbox( &cb[1], INACTIVE);

		if ( group < 2)
			drawCheckbox( &cb[group], ACTIVE);
		else
			drawButton( &bu[group-2], ACTIVE);

		a_inner = -1;

		while ( a_inner) {
			switch ( ch = GetKey()) {

				case kbNoKey:
					break;

				case kbTab:
				case kbShiftTab:
					if ( group > 1)
						drawButton( &bu[group-2], INACTIVE);
					else
						drawCheckbox( &cb[group], INACTIVE);

					if ( ch == kbTab) {
						if ( ++group > 4)
							group = 0;
					}
					else {
						if ( --group < 0)
							group = 4;
					}

					if ( group > 1)
						drawButton( &bu[group-2], ACTIVE);
					else
						drawCheckbox( &cb[group], ACTIVE);
					break;

				case kbEsc:
					a_inner = alive = 0;
					break;

				case kbF1:
					openHelp( HlpEditReportPrint);
					drawFilePrintWindow();
					a_inner = 0;
					break;

				case kbEnter:
					switch ( group) {
						case 0:
						case 1:
							cb[group].select = 0xFF;
							cb[abs( group-1)].select = 0;
							drawCheckbox( &cb[group], ACTIVE);
							drawCheckbox( &cb[abs( group-1)], INACTIVE);
						case 2:										/* break IS missing */
							if ( cb[0].select)
								portnumber = 0;
							else
								portnumber = 1;

							status = biosprint( 2, group, portnumber);

							if ( !( status & 0x10)) {
								errTxt[36].txt2 = "Printer NOT selected";
								displayError( 36);
							}
							else
								if ( status & 0x20) {
									errTxt[36].txt2 = "Printer out of paper";
									displayError( 36);
								}
							else
								if ( status & 0x01) {
									errTxt[36].txt2 = "Printer I/O-error";
									displayError( 36);
								}
							else
								if ( status & 0x08) {
									errTxt[36].txt2 = "Printer time out";
									displayError( 36);
								}
							else
								if (( stream =
									fopen( LPTText[portnumber], "w")) != NULL) {

									Image = displayMessage(
														( PrtPrtRight+PrtPrtLeft)/2,
														( PrtPrtTop+PrtPrtBottom)/2,
														EGA_GREEN, "!!! Printing !!!");

									printToMedia( stream);
									free( Image);
									fclose( stream);
									a_inner = alive = 0;
									break;
								}
							a_inner = 0;
							break;

						case 3:
							a_inner = alive = 0;
							break;

						case 4:
							openHelp( HlpEditReportPrint);
							drawFilePrintWindow();
							a_inner = 0;
							break;
					}
					break;

				case kbUp:
				case kbDown:
				case kbSpace:
					if (( group == 0) || ( group == 1)) {
						if ( ch == kbSpace) {
							cb[group].select = 0xFF;
							cb[abs( group-1)].select = 0;
						}
						else {
							if ( --group)
								group = 1;
						}
						drawCheckbox( &cb[group], ACTIVE);
						drawCheckbox( &cb[abs( group-1)], INACTIVE);
					}
					break;
			}
		}
	}
	closeWindow();

	return( 1);
}


int doPrintReport( void) {

	int alive = -1, a_inner, group;
	int start, len, ch, i, j;

	if ( NoWin) {

		PrtNoWin = ++NoWin;

		WinPtr[NoWin]->mode = PrintMode;
		SetRect( &WinPtr[NoWin]->rect, PrtLeft, PrtTop, PrtRight, PrtBottom);

		len = ( PrtRight - PrtLeft) / 4.0;
		start = PrtLeft + ( len - 60) / 3.0;

		addButton( &Prtbu[0], start, PrtBottom-25,
					start+60, PrtBottom-10, "Print", ButColor);

		addButton( &Prtbu[1], len+start, PrtBottom-25,
					len+start+60, PrtBottom-10, "File", ButColor);

		addButton( &Prtbu[2], 2*len+start, PrtBottom-25,
					2*len+start+60, PrtBottom-10, "Cancel", ButColor);

		addButton( &Prtbu[3], 3*len+start, PrtBottom-25,
					3*len+start+60, PrtBottom-10, "Help", ButColor);

		for ( j = PrtTop+82, i=0; i<3; i++, j += 27)
			addEditfield( &Prtef[i], PrtLeft+7, j, 50, PrtTxt[i], NULL);

		PrtTxt[0][0] = PrtTxt[1][0] = PrtTxt[2][0] = '\x0';

		group = 3;

		while( alive) {

			drawFilePrintWindow();

			if ( group < 3)
				drawEditfield( &Prtef[group], ACTIVE);
			else
				drawButton( &Prtbu[group-3], ACTIVE);

			a_inner = -1;

			while ( a_inner) {
				switch ( ch = GetKey()) {
					case kbNoKey:
						break;

					case kbEsc:
						a_inner = alive = 0;
						break;

					case kbTab:
					case kbShiftTab:
						if ( group > 2)
							drawButton( &Prtbu[group-3], INACTIVE);
						else
							drawEditfield( &Prtef[group], INACTIVE);

						if ( ch == kbTab) {
							if ( ++group > 6)
								group = 0;
						}
						else {
							if ( --group < 0)
								group = 6;
						}
						if ( group > 2)
							drawButton( &Prtbu[group-3], ACTIVE);
						else
							drawEditfield( &Prtef[group], ACTIVE);
						break;

					case kbF1:
						openHelp( HlpEditReport);
						a_inner = 0;
						break;

					case kbEnter:
						switch ( group) {
							case 3:
								printFilePrint();
								a_inner = 0;
								break;
							case 4:
								SaveRestoreWindow( FilePrintType,
													SaveMode, HlpEditReportFile);
								a_inner = 0;
								break;
							case 5:
								a_inner = alive = 0;
								break;
							case 6:
								openHelp( HlpEditReport);
								a_inner = 0;
								break;
						}
						break;

					default:
						if (( group >= 0) && ( group < 3))
							editEditfield( &Prtef[group], ch);
						break;
				}
			}
		}
		closeWindow();

		return( 1);
	}
	return( displayError( 0));
}


/* return character for file name of false if not ok - used by choosefile */

int isname( int c) {

	c = toupper( c);
	if ( isalpha( c) || isdigit( c) ||
		( c == '.') || ( c == '-') || ( c== '_'))
		return( c);

	return( 0);
}


/* return a buffer with file names in it - used by choosefile */

char* getfiles( char* spec, int* count, char* drivemap) {

	struct ffblk f;
	char* p, drive[MAXDRIVE], dir[MAXDIR], b[128];
	int j, i=0, k;

	*count = 0;

	/* start by counting the files */

	k = strlen( drivemap);

	if ( !findfirst( spec, &f, 0)) {
		do {
			++*count;
		} while ( !findnext( &f));
	}

	fnsplit( spec, drive, dir, NULL, NULL);
	fnmerge( b, drive, dir, "*", ".");

	if ( !findfirst( b, &f, 16)) {
		do {
			if ((( f.ff_attrib & 16) == 16) && strcmp( f.ff_name, "."))
				++*count;
		} while ( !findnext( &f));
	}

	if (( *count += k) == 0)
		return( NULL);

	if (( p = malloc( *count*( FILELINESIZE+1))) != NULL) {

		if ( !findfirst( spec, &f, 0)) {
			do {
				sprintf( p+(( FILELINESIZE+1)*i++), "%s", f.ff_name);
			} while ( !findnext( &f));
		}

		if ( !findfirst( b, &f, 16)) {
			do {
				if ((( f.ff_attrib & 16) == 16) && strcmp( f.ff_name, "."))
					sprintf( p+(( FILELINESIZE+1)*i++), "\\%s", f.ff_name);
			} while ( !findnext( &f));
		}

		for ( j=0; j<k; ++j)
			sprintf( p+(( FILELINESIZE+1)*i++), "[ %c ]", drivemap[j]);

		qsort( p, *count, FILELINESIZE+1, strcmp);
	}
	return( p);
}


int displayDiskError( int diskErr) {

	char* txtptr;
	int ch;

	switch ( diskErr) {
		case 0:
			txtptr = "Disk is write-protected";
			break;
		case 1:
			txtptr = "Unknown disk unit ID";
			break;
		case 2:
			txtptr = "Disk drive not ready";
			break;
		case 3:
			txtptr = "Unknown command";
			break;
		case 4:
			txtptr = "Disk data error ( CRC)";
			break;
		case 5:
			txtptr = "Bad request";
			break;
		case 6:
			txtptr = "Disk seek error";
			break;
		case 7:
			txtptr = "Non-DOS disk";
			break;
		case 8:
			txtptr = "Disk sector not found";
			break;
		case 10:
			txtptr = "Write error";
			break;
		case 11:
			txtptr = "Read error";
			break;
		case 12:
			txtptr = "General failure";
			break;
		case 13:
			txtptr = "Reserved";
			break;
		case 14:
			txtptr = "Reserved";
			break;
		case 15:
			txtptr = "Invalid disk change";
			break;
	}
	errTxt[4].txt2 = txtptr;

	if (( ch = displayError( 4)) == kbEsc)
		ch = 0;

	return( ch);
}



FILE* openFile( char* filename, char* filemode, int errCode) {

	int retry = 1;
	char openTest[20];

	getcwd( errbuf, 128);
	diskErr = -1;

	oldHarderr = getvect( 0x24);
	harderr( diskErrorHandler);

	fnsplit( filename, openTest, NULL, NULL, NULL);

	if ( openTest[0] == '\x0') {
		openTest[0] = errbuf[0];
		openTest[1] = ':';
		openTest[2] = '\x0';
		strcat( openTest, filename);
	}
	else
		openTest[0] = 0x00;

	while ( retry) {

		if ( openTest[0] == 0) {
			if (( fp = fopen( filename, filemode)) != NULL)
				fclose( fp);
		}
		else {
			if (( fp = fopen( openTest, filemode)) != NULL)
				fclose( fp);
		}

		if ( diskErr == -1) {
			if (( fp = fopen( filename, filemode)) == NULL) {
				if (( diskErr == -1) || (( diskErr != -1) && ( errCode != -1))) {
					if ( errCode > -1)
						displayError( errCode);

					setvect( 0x24, oldHarderr);
					return( NULL);
				}
			}
			else {
				setvect( 0x24, oldHarderr);
				return( fp);
			}
		}
		else {
			if ( errCode != -1) {
				if ( errCode > -1)
					displayError( errCode);

				setvect( 0x24, oldHarderr);
				return( NULL);
			}
		}

		retry = displayDiskError( diskErr);
	}

	restoreDir( errbuf);

	setvect( 0x24, oldHarderr);
	return( NULL);
}


int testDisk( int n) {

	char b[32];
	int retry = 1;

	oldHarderr = getvect( 0x24);
	harderr( diskErrorHandler);

	getcwd( errbuf, 80);
	sprintf( b, "%c:\\TEMP.dat", n+'A');

	do {
		diskErr = -1;

		if (( fp = fopen( b, "r")) != NULL)
			fclose( fp);

		if ( diskErr != -1)
			retry = displayDiskError( diskErr);

	} while (( diskErr != -1) && retry);

	setvect( 0x24, oldHarderr);

	if ( diskErr == -1)
		return( 1);

	restoreDir( errbuf);
	return( 0);
}


/* handle a disk hardware error - used by openFile */

#pragma warn -par
int diskErrorHandler( int errval, int ax, int bp, int si) {

	unsigned int di;

	di = _DI;

	if ( ax >= 0)
		diskErr = di & 0x00FF;

	hardretn( 0);
}
#pragma warn +par


/* restore a saved directory path - used by choosefile */

int restoreDir( char* s) {

	strupr( s);

	if ( isalpha( s[0]) && ( s[1] == ':'))
		setdisk( s[0] - 'A');

	chdir( s);

	return( diskErr);
}

int checkFilename( char* name) {

	if ( !strcmpi( name, "LPT1") ||
		  !strcmpi( name, "LPT2") ||
		  !strcmpi( name, "COM1") ||
		  !strcmpi( name, "COM2") ||
		  !strcmpi( name, "NUL") ||
		  !strcmpi( name, "PRN") ||
		  !strcmpi( name, "CON") ||
		  !strcmpi( name, "AUX")) {
		displayError( 42);
		return( 0);
	}
	return( 1);
}



int chooseFile( char* spec, char* name, char* drivemap,
											char* headline, int helptxt, int mode) {

	/*	Restore/Save: 0=Directory, 1=List, 2=Name, 3=extension,
						  4=Restore/Save, 5=Cancel, 6=Help */

	WININFO* newP = WinPtr[++NoWin];

	LIST ls;							/* List of files in directory */
	EDITFIELD fspec;				/* Directory */
	EDITFIELD fname;				/* Filename in directory to edit */
	BUTTON bu[3];					/* Restore/Save, Cancel, Help */

	struct ffblk f;

	char* thespec;
	char dir[MAXDIR], file[MAXFILE], ext[MAXEXT], drive[MAXDRIVE];
	char nm[24], tmpbuf[129];

	int a_inner, alive = -1;
	int ch, i, j, group;

	if ( strlen( spec) <= 35)
		thespec = spec;
	else
		thespec = spec + strlen( spec - 35);

	newP->mode = mode;
	SetRect( &newP->rect, FileLeft, FileTop - WINDOWBARDEEP,
															FileRight, FileBottom);
	addButtons( &bu[0], &bu[1], &bu[2], &newP->rect, Ok);

	addEditfield( &fspec, FileLeft+28, FileTop+20, 35, thespec, NULL);
	addEditfield( &fname, FileLeft+204, FileTop+50, 13, name, isname);

	addList( &ls, FileLeft+27, FileTop+50, FILELINESIZE, 12, 0, 0, NULL);

	group = 0;

	do {
		name[0] = fname.curpos = ls.count = 0;

		if ( !testDisk( spec[0] - 'A'))
			return( 0);

		ls.base = getfiles( spec, &ls.count, drivemap);

		ls.top = ls.ptr = 0;

		openWindow( newP, headline);

		for ( i=0; i<3; i++)
			drawButton( &bu[i], INACTIVE);

		setcolor( EGA_BLACK);
		outtextxy( FileLeft+27, FileTop+8, "Directory");
		drawEditfield( &fspec, INACTIVE);

		outtextxy( FileLeft+27, FileTop+38, "Files");

		if ( group == 0)
			drawList( &ls, ACTIVE);
		else
			drawList( &ls, INACTIVE);

		outtextxy( FileLeft+204, FileTop+38, "Name");

		if ( group == 1)
			drawEditfield( &fname, ACTIVE);
		else
			drawEditfield( &fname, INACTIVE);

		if ( group < 4)
			drawButton( &bu[0], ACTIVE);

		a_inner = -1;

		while ( a_inner) {
			switch ( ch = GetKey()) {
				case kbNoKey:
					break;

				case kbEsc:
					a_inner = alive = name[0] = 0;
					break;

				case kbTab:
				case kbShiftTab:
					switch ( group) {
						case 0:
							drawList( &ls, INACTIVE);
							break;
						case 1:
							drawEditfield( &fname, INACTIVE);
							break;
						default:
							drawButton( &bu[group - 2], INACTIVE);
							break;
					}

					if ( ch == kbTab) {
						if ( ++group > 4)
							group = 0;
					}
					else {
						if ( --group < 0)
							group = 4;
					}

					drawButton( &bu[0], ACTIVE);

					switch ( group) {
						case 0:
							drawList( &ls, ACTIVE);
							break;
						case 1:
							drawEditfield( &fname, ACTIVE);
							break;
						case 2:
							break;
						default:
							drawButton( &bu[0], INACTIVE);
							drawButton( &bu[group - 2], ACTIVE);
							break;
					}
					break;

				case kbHome:
				case kbEnd:
					if ( group == 1)
						editEditfield( &fname, ch); 		  /* Break IS missing */
				case kbUp:
				case kbDown:
				case kbPgUp:
				case kbPgDn:
					if ( group == 0)
						if ( updateList( &ls, ch))
							drawList( &ls, ACTIVE);
					break;

				case kbF1:
					openHelp( helptxt);
					if ( helptxt == HlpEditReportFile)
						drawFilePrintWindow();
					a_inner = 0;
					break;

				case kbEnter:
					switch ( group) {
						case 0:
							strcpy( nm, name);
							strcpy( name, ls.base +
									(( ls.ptr + ls.top)*( ls.length + 1)));
							drawEditfield( &fname, ACTIVE);
							drawList( &ls, INACTIVE);
							group++;
							break;

						case 1:
						case 2:
							if ( fname.text[0] == 0) {
								displayError( 37);
								break;
							}

							drawEditfield( &fname, INACTIVE);
							a_inner = group = 0;
							break;

						case 3:
							a_inner = alive = 0;
							break;

						case 4:
							openHelp( helptxt);

							if ( helptxt == HlpEditReportFile)
								drawFilePrintWindow();

							a_inner = 0;
							break;
					}
					break;

				default:
					if ( group == 1)
						editEditfield( &fname, ch);
					break;
			}
		}

		if ( ls.base != NULL)
			free( ls.base);

		if ( !alive && ( name[0] == 0))
			break;

		if ( ch == kbF1)
			;
		else
			if ( name[0] == '\\') {
				/* change directory */

				fnsplit( spec, drive, dir, file, ext);

				if ( strcmp( name+1, "..")) {
					/* add new directory */

					strcat( dir, name+1);
					strcat( dir, "\\");
				}
				else {
					/* back up one directory */

					i = strlen( dir) - 2;
					while (( i > 0) && ( dir[i] != '\\'))
					i--;
					dir[i+1] = 0;
				}
				name[0] = 0;
				fnmerge( spec, drive, dir, file, ext);
				drawEditfield( &fspec, INACTIVE);
				drawEditfield( &fname, INACTIVE);
			}
		else
			if ( name[0] == '[') {
				/* change drives */

				if ( testDisk( name[2]-'A')) {
					fclose( stream);

					fnsplit( spec, drive, dir, file, ext);
					drive[0] = name[2];
					strcpy( dir, "\\");
					fnmerge( spec, drive, dir, file, ext);
					name[0] = 0;
					drawEditfield( &fname, INACTIVE);
					drawEditfield( &fspec, INACTIVE);
				}
				else
					return ( 0);
			}
		else
			if (( mode == RestoreMode) && (name[0] != 0)) {

				if ( checkFilename( name)) {

					strcpy( tmpbuf, spec);
					fnsplit( tmpbuf, drive, dir, NULL, NULL);
					fnmerge( tmpbuf, drive, dir, NULL, NULL);

					strcat( tmpbuf, name);

					if (( stream = openFile( tmpbuf, "rb", 5)) == NULL)
						;
					else {
						fclose( stream);
						alive = 0;
					}
				}
			}
		else
			if (( mode == SaveMode) && ( name[0] != 0)) {

				if ( checkFilename( name)) {

					strcpy( tmpbuf, spec);
					fnsplit( tmpbuf, drive, dir, file, ext);

					if ( !strstr( name, "."))
						strcat( name, ext);

					if ( !strstr( name, ext)) {
						errTxt[6].txt2 = ext;
						displayError( 6);
					}
					else {
							fnmerge( tmpbuf, drive, dir, NULL, NULL);
							strcat( tmpbuf, name);

							if (( stream = openFile( tmpbuf, "rb", -1)) != NULL) {
								fclose( stream);
								if ( displayError( 2) == YES)
									alive = 0;
							}
							else
								alive = 0;
					}
				}
			}
	} while ( alive);

	return( name[0]);
}
