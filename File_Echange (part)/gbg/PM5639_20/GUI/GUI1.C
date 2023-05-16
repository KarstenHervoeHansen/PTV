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

#include "gui1.h"

char* photxt[] = {
	"EBU       (fty)",
	"SMPTE ""C""   (fty)"
};

static char* whrtxt[] = {
	"D6500     (fty)",
	"3200 Cor. (fty)",
	"9300 Cor. (fty)"
};

const float D6500[] = {
	0.3127, 0.3291, 0.1978, 0.4683, 0.1978, ( 2./3.*0.4683)
};

const float cor3200[] = {
	0.423, 0.399, 0.2437, 0.5173, 0.2437, ( 2./3.*0.5173)
};

const float cor9300[] = {
	0.285, 0.293, 0.1917, 0.4435, 0.1917, ( 2./3.*0.4435)
};

const int EBUphosphor[] = {
	int_scale, -3761, -1300, -4231, int_scale, 212, 575, -1709, int_scale
};

const int SMPTECphosphor[] = {
	int_scale, -4017, -1282, -4433, int_scale, 174, 611, -1702, int_scale
};

int WhrYmin( float x, float y) {

	if ( x < 0.0082)
		return( -1);
	else
		if (( x >= 0.0082) && ( x < 0.0913)) {
			if ( y >= ( -4.8821*x + 0.5784))
				return( 0);
			return( -1);
		}
	else
		if (( x >= 0.0913) && ( x < 0.1741)) {
			if ( y >= ( -1.5422*x + 0.2735))
				return( 0);
			return( -1);
		}
	else
		if (( x >= 0.1741) && ( x <= 0.7347)) {
			if ( y >= ( 0.4643*x - 0.0758))
				return( 0);
			return( -1);
		}
	return( -1);
}


int WhrYmax( float x, float y) {

	if ( x < 0.0082)
		return( -1);
	else
		if (( x >= 0.0082) && ( x < 0.0139)) {
			if( y <= ( 37.1579*x + 0.2337))
				return( 0);
			return( -1);
		}
	else
		if (( x >= 0.0139) && ( x < 0.0743)) {
			if ( y <= ( 1.3841*x + 0.7310))
				return( 0);
			return( -1);
		}
	else
		if (( x >= 0.0743) && ( x < 0.2296)) {
			if ( y <= ( -0.5441*x + 0.8792))
				return( 0);
			return( -1);
		}
	else
		if (( x >= 0.2296) && ( x < 0.6658)) {
			if ( y <= ( -0.9779*x + 0.9851))
				return( 0);
			return( -1);
		}
	else
		if (( x >= 0.6658) && ( x <= 0.7347)) {
			if ( y <= ( -1.068*x + 1.050))
				return( 0);
			return( -1);
		}
	return( -1);
}


int openWhrWindow( int WhrMode, int helptxt) {

/*	0=Name, 1=List, 5=Restore, 6=Cancel, 7=Help
		0=Name, 1=List, 5=Save, 6=Cancel, 7=Help
		0=Name, 1=Meas, 2=xy, 3=u'v', 4=uv, 5=Ok, 6=Cancel, 7=Help */

	WININFO* oldP = WinPtr[NoWin++];
	WININFO* newP = WinPtr[NoWin];

	EDITFIELD ef0;					/* White reference name */
	EDITFIELD ef1[6];				/*	x, y, u', v', u, v */

	BUTTON bu[3];					/* ( Ok, Restore, Save), Cancel, Help */
	BUTTON tm;						/* Take Measurement */

	LIST wl;							/* List of white references */

	FILENODE* whrroot;			/* Pointer to start of list */
	FILENODE* whrtop;				/* Pointer to 1 element displayed in list */
	FILENODE* whrptr;				/* Pointer to actual element in list */
	FILENODE* whraux;				/* Temporary pointer */

	FILE* stream;

	char namebuf[16];
	char valbuf[6][8];
	char* headptr;

	float auxvar;
	int ch, i, j, group, a_inner, alive, WhrErrorFlag = 0;
	int pl[6] = { 24, 40, 86, 102, 148, 164 };

	for ( i=0; i<6; i++)
		pl[i] += WhrTop;

	newP->mode = WhrMode;
	SetRect( &newP->rect, WhrLeft, WhrTop-WINDOWBARDEEP, WhrRight, WhrBottom);

	whrptr = whrroot = ( FILENODE *) malloc( sizeof( FILENODE));
	whrroot->next = NULL;

	if ( WhrMode != LearnMode) {
		if (( stream = openFile( NAME_WHR_FILE, "rb", -2)) == NULL) {

			if (( stream = openFile( NAME_WHR_FILE, "wb", -1)) == NULL) {
				free( whrptr);
				closeWindow();
				return( 0);
			}

			for ( j=0; j<6; j++)
				whrptr->val[j] = D6500[j];

			strcpy( whrptr->name, whrtxt[0]);
			fwrite( whrptr, sizeof( *whrroot), 1, stream);

			whrptr = whrptr->next = ( FILENODE *) malloc( sizeof( FILENODE));

			for ( j=0; j<6; j++)
				whrptr->val[j] = cor3200[j];

			strcpy( whrptr->name, whrtxt[1]);
			fwrite( whrptr, sizeof( *whrroot), 1, stream);

			whrptr = whrptr->next = ( FILENODE *) malloc( sizeof( FILENODE));

			for ( j=0; j<6; j++)
				whrptr->val[j] = cor9300[j];

			strcpy( whrptr->name, whrtxt[2]);
			fwrite( whrptr, sizeof( *whrroot), 1, stream);

			wl.count = 3;
			whrptr->next = NULL;
		}
		else {
			wl.count = 0;

			fread( whrroot, sizeof( *whrroot), 1, stream);

			while ( !feof( stream)) {
				wl.count++;
				whraux = whrptr;
				whrptr = whrptr->next = ( FILENODE *) malloc( sizeof( FILENODE));
				fread( whrptr, sizeof( *whrroot), 1, stream);
			}
			whraux->next = NULL;
			free( whrptr);
		}
		fclose( stream);
		whrtop = whrptr = whrroot;
	}

	alive = -1;

	wl.top = wl.ptr = group = 0;
	whrptr = whrtop = whrroot;

	switch ( WhrMode) {
		case LearnMode:
			headptr = "Learn White Reference";

			addButtons( &bu[0], &bu[1], &bu[2], &newP->rect, Ok);

			addEditfield( &ef0, WhrleftCol, WhrTop+24, 16, namebuf, NULL);
			strcpy( namebuf, "");

			addButton( &tm, WhrleftCol+39, WhrTop+89, WhrleftCol+122, WhrTop+104,
																		"MEASURE", ButColor);
			for ( i=0; i<6; i++) {
				sprintf( valbuf[i], "%5.4f", whrptr->val[i] = oldP->meas.whr[i]);

				if ( oldP->meas.whr[i] < 0.0001) {
					for ( i=0; i<6; i++)
						strcpy( valbuf[i], "");
					break;
				}
			}
			break;

		case RestoreMode:
			group++;
			headptr = "Restore White Reference";

			addButtons( &bu[0], &bu[1], &bu[2], &newP->rect, Restore);

			addList( &wl, WhrleftCol, WhrTop+60, 14, 10, wl.count, 0, NULL);

			addEditfield( &ef0, WhrleftCol, WhrTop+24, 16, namebuf, NULL);

			for ( i=0; i<6; i++)
				sprintf( valbuf[i], "%5.4f", whrroot->val[i]);
			break;

		case SaveMode:
			headptr = "Save White Reference";

			addButtons( &bu[0], &bu[1], &bu[2], &newP->rect, Save);

			addList( &wl, WhrleftCol, WhrTop+60, 14, 10, wl.count, 0, NULL);

			addEditfield( &ef0, WhrleftCol, WhrTop+24, 16, namebuf, NULL);

			for( i=0; i<6; i++)
				sprintf( valbuf[i], "%5.4f", oldP->whrinfo.whr[i]);

			strcpy( namebuf, oldP->whrinfo.name);
			break;
	}

	for ( i=0; i<6; i++)
		addEditfield( &ef1[i], WhrrightCol+128, pl[i], 7,
																		valbuf[i], NumberTrap);
	i = 0;

	do {
		a_inner = -1;

		openWindow( newP, headptr);

		for ( j=0; j<6; j++) {
			if ( j < 3) {
				drawButton( &bu[j], INACTIVE);
				drawSetupRect( WhrrightCol-8, pl[j<<1]-12, 2,
																	HeadText[j], WhrRect);
			}

			outtextxy( WhrrightCol, pl[j], WhrText[j]);
			drawEditfield( &ef1[j], INACTIVE);
		}

		if ( group > 4)
			drawButton( &bu[group-5], ACTIVE);

		setcolor( EGA_BLACK);

		switch ( WhrMode) {
			case SaveMode:
				outtextxy( WhrleftCol, WhrTop+48, "References");
				if ( group == 1)
					drawFileList( &wl, whrtop, ACTIVE);
				else
					drawFileList( &wl, whrtop, INACTIVE);

				setcolor( EGA_BLACK);
				outtextxy( WhrleftCol, WhrTop+12, "Save ref.");

				if ( group == 0)
					drawEditfield( &ef0, ACTIVE);
				else
					drawEditfield( &ef0, INACTIVE);
				break;

			case RestoreMode:
				drawButton( &bu[0], ACTIVE);

				outtextxy( WhrleftCol, WhrTop+48, "References");

				if ( group == 1)
					drawFileList( &wl, whrtop, ACTIVE);
				else
					drawFileList( &wl, whrtop, INACTIVE);

				setcolor( EGA_BLACK);
				outtextxy( WhrleftCol, WhrTop+12, "Restore ref.");

				strcpy( namebuf, whrptr->name);
				drawEditfield( &ef0, INACTIVE);
				break;

			case LearnMode:
				outtextxy( WhrleftCol, WhrTop+12, "Name ref.");

				drawSetupRect( WhrleftCol-2, WhrTop+74, 7, "Measurement", 21*8+4);

				if ( group == 0)
					drawEditfield( &ef0, ACTIVE);
				else
					drawEditfield( &ef0, INACTIVE);

				if ( group == 1)
					drawButton( &tm, ACTIVE);
				else
					drawButton( &tm, INACTIVE);

				if (( group > 1) && ( group <5))
					drawEditfield( &ef1[i], ACTIVE);

				outtextxy( WhrleftCol+5, WhrTop+112, "     To take a");
				outtextxy( WhrleftCol+5, WhrTop+124, "      single");
				outtextxy( WhrleftCol+5, WhrTop+136, "    measurement");
				outtextxy( WhrleftCol+5, WhrTop+148, "       press: ");
				outtextxy( WhrleftCol+5, WhrTop+160, " <SPACE> or <ENTER> ");
				break;
		}

		while ( a_inner) {
			switch ( ch = GetKey()) {

				case kbNoKey:
					break;

				case kbEsc:
					a_inner = alive = 0;
					break;

				case kbTab:
				case kbShiftTab:
					switch ( group) {
						case 0:
							drawEditfield( &ef0, INACTIVE);
							break;
						case 1:
							if ( WhrMode == LearnMode)
								drawButton( &tm, INACTIVE);
							else
								drawFileList( &wl, whrtop, INACTIVE);
							break;
						case 2:
						case 3:
						case 4:
							if (( i % 2) == 0 )
								j = i;
							else
								j = i-1;

							if (( valbuf[j][0] == '\x0') &&
								 ( valbuf[j+1][0] == '\x0')) {
								WhrErrorFlag = 0;
								drawEditfield( &ef1[i], INACTIVE);
								break;
							}

							/* NB!!   ( u = u') & ( v = (2/3)*v') */

							switch ( i) {
								case 0:
									/* For new x: update ( u',v') & ( u,v) */
									/*		u' = u = 4x / ( 12y + 3 - 2x)		*/
									/*		v' = 9y / ( 12y + 3 - 2x)			*/
									/*		v  = (2/3)*v'							*/

									auxvar = 12.*whrptr->val[1]+3.-2.*whrptr->val[0];

									whrptr->val[2] = ( 4.*whrptr->val[0])/auxvar;
									whrptr->val[3] = ( 9.*whrptr->val[1])/auxvar;
									whrptr->val[4] = whrptr->val[2];
									whrptr->val[5] = ( 2./3.)* whrptr->val[3];
									break;

								case 1:
									/* For new y: update ( u',v') & ( u,v) */
									/*		u' = u = 4x / ( 12y + 3 - 2x)		*/
									/*		v' = 9y / ( 12y + 3 - 2x)			*/
									/*		v = ( 2/3)*v'							*/

									auxvar = 12.*whrptr->val[1]+3.-2.*whrptr->val[0];

									whrptr->val[2] = ( 4.*whrptr->val[0])/auxvar;
									whrptr->val[3] = ( 9.*whrptr->val[1])/auxvar;
									whrptr->val[4] = whrptr->val[2];
									whrptr->val[5]  = ( 2./3.)*whrptr->val[3];
									break;

								case 2:
								case 4:
									/* For new u': update ( x,y) & ( u,v)  */
									/*		x  = -9u' / ( 16v' - 12 - 6u')	*/
									/*		y  = -4v' / ( 16v' - 12 - 6u')	*/
									/*		u  = u'									*/

									auxvar= 16.*whrptr->val[3]-12.-6.*whrptr->val[2];

									whrptr->val[0] = ( -9.*whrptr->val[2])/auxvar;
									whrptr->val[1] = ( -4.*whrptr->val[3])/auxvar;

									if ( i == 2)
										whrptr->val[4] = whrptr->val[2];
									else
										whrptr->val[2] = whrptr->val[4];
									break;

								case 3:
									/* For new v': update ( x,y) & ( u,v)  */
									/*		x = -9u' / ( 16v' - 12 - 6u')		*/
									/*		y = -4v' / ( 16v' - 12 - 6u')		*/
									/*		v = ( 2/3)*v'							*/

									auxvar = 16.*whrptr->val[3]-12.-6.*whrptr->val[2];

									whrptr->val[0] = ( -9.*whrptr->val[2])/auxvar;
									whrptr->val[1] = ( -4.*whrptr->val[3])/auxvar;
									whrptr->val[5] = ( 2./3.)*whrptr->val[3];
									break;

								case 5:
									/* For new v: update ( x,y) & ( u',v') */
									/*		x  = -9u / ( 24v - 12 - 6u)		*/
									/*		y  = -4v / ( 24v - 12 - 6u)		*/
									/*		v' = ( 3/2)*v							*/

									auxvar = 24.*whrptr->val[5]-12.-6.*whrptr->val[4];

									whrptr->val[0] = (-9.*whrptr->val[4])/auxvar;
									whrptr->val[1] = (-6.*whrptr->val[5])/auxvar;
									whrptr->val[3] = (3./2.)*whrptr->val[5];
									break;
							}

							if ( WhrYmax( whrptr->val[0], whrptr->val[1]) ||
								  WhrYmin( whrptr->val[0], whrptr->val[1])) {

								displayError( 12);

								WhrErrorFlag = 0xFF;
								break;
							}

							for ( j = 0; j < 6; j++) {
								sprintf( valbuf[j], "%5.4f", whrptr->val[j]);
								drawEditfield( &ef1[j], INACTIVE);
							}

							WhrErrorFlag = 0;
							break;
						default:
							drawButton( &bu[group-5], INACTIVE);
							break;
					}

					if ( WhrErrorFlag) {
						break;
					}

					if ( ch == kbTab)
						group++;
					else
						group--;

					if ( group > 7) {
						group = 0;

						if ( WhrMode == RestoreMode)
							group++;
					}

					if ((( WhrMode == RestoreMode) && group == 0) || ( group < 0))
						group = 7;

					if (( WhrMode != LearnMode) || ( oldP->mode != CIEMode)) {

						if ( group == 2)
							group = 5;

						if ( group == 4)
							group = 1;

						if ( WhrMode == RestoreMode)
							drawButton( &bu[0], ACTIVE);
					}

					switch ( group) {
						case 0:
							drawEditfield( &ef0, ACTIVE);
							break;
						case 1:
							if ( WhrMode == LearnMode)
								drawButton( &tm, ACTIVE);
							else
								drawFileList( &wl, whrtop, ACTIVE);
							break;
						case 2:
						case 3:
						case 4:
							i = ( group - 2) << 1;
							drawEditfield( &ef1[i], ACTIVE);
							break;
						default:
							drawButton( &bu[0], INACTIVE);
							drawButton( &bu[group-5], ACTIVE);
							break;
					}
					break;

				case kbUp:
				case kbDown:
					if (( group > 1) && ( group < 5)) {
						drawEditfield( &ef1[i], INACTIVE);

						j = ( group - 2) << 1;
						if ( ch == kbUp) {
							if ( --i < j)
								i = j + 1;
						}
						else
							if ( ++i > j + 1)
								i = j;

						drawEditfield( &ef1[i], ACTIVE);
						break;
					}										/* break missing on purpose */
				case kbHome:
				case kbEnd:
				case kbPgUp:
				case kbPgDn:
					switch ( group) {
						case 0:
							editEditfield( &ef0, ch);
							break;

						case 1:
							if ( WhrMode != LearnMode) {
								if ( updateList( &wl, ch)) {

									whrptr = whrtop = whrroot;

									for ( i=0; i < wl.ptr+wl.top; i++) {

										if (( i < wl.top) &&  whrtop->next)
											whrtop = whrtop->next;
	
										if ( whrptr->next)
											whrptr = whrptr->next;
									}

									drawFileList( &wl, whrtop, ACTIVE);

									if ( WhrMode == RestoreMode) {

										strcpy( namebuf, whrptr->name);
										drawEditfield( &ef0, INACTIVE);

										for ( j=0; j<6; j++) {
											sprintf( valbuf[j], "%5.4f", whrptr->val[j]);
											drawEditfield( &ef1[j], INACTIVE);
										}
									}
								}
							}
							break;

						case 2:
						case 3:
						case 4:
							editEditfield( &ef1[i], ACTIVE);
							break;
					}
					break;

				case kbF1:
					openHelp( helptxt);
					a_inner = 0;
					break;

				case kbSpace:
					if ( group == 0) {
						editEditfield( &ef0, ch);
						break;
					}

					if ( group == 1) {
						if ( WhrMode == LearnMode)
							;
						if ( WhrMode == SaveMode) {
							strcpy( namebuf, whrptr->name);
							drawEditfield( &ef0, INACTIVE);
							break;
						}
					}
					else
						break;

				case kbEnter:
					if ( group == 6) {						/* Cancel */
						a_inner = alive = 0;
						break;
					}

					if ( group == 7) {						/* Help */
						openHelp( helptxt);
						a_inner = 0;
						break;
					}

					if ( WhrMode == SaveMode) {
						if (( group == 5) && ( namebuf[0] != 0)) {

							if ( !strcmpi( namebuf, whrtxt[0]) ||
								  !strcmpi( namebuf, whrtxt[1]) ||
									!strcmpi( namebuf, whrtxt[2])) {
									displayError( 15);
									break;
								}

							whraux = ( FILENODE *) malloc( sizeof( FILENODE));
							strcpy( whraux->name, namebuf);

							for ( j=0; j<6; j++)
								whraux->val[j] = oldP->whrinfo.whr[j];

							whrptr = whrroot->next->next;

							do {
								whrtop = whrptr;
								whrptr = whrptr->next;
							} while ((( j = strcmpi( whrptr->name, namebuf)) < 0)
																			&& whrptr);
							if ( !j) {
								free( whraux);

								if ( displayError( 3) == YES)
									for ( j=0; j<6; j++)
										whrptr->val[j] = oldP->whrinfo.whr[j];
								else {
									whrptr = whrtop = whrroot;
									a_inner = 0;
									break;
								}
							}
							else
								if ( j > 0) {
									whraux->next = whrtop->next;
									whrtop->next = whraux;
								}
							else {
								whrtop->next = whraux;
								whraux->next = NULL;
							}

							whrptr = whrroot;

							if (( stream =
										openFile( NAME_WHR_FILE, "wb", -1)) == NULL) {
								alive = 0;
								break;
							}

							while ( whrptr) {
								fwrite( whrptr, sizeof( *whrptr), 1, stream);
								whrptr = whrptr->next;
							}
							fclose( stream);

							alive = a_inner = 0;
						}
						break;
					}

					if ( WhrMode == RestoreMode) {
						if (( group == 1) || ( group == 5)) {

							strcpy( oldP->whrinfo.name, whrptr->name);

							for ( j = 0; j<6; j++)
								oldP->whrinfo.whr[j] = whrptr->val[j];

							alive = a_inner = 0;
						}
						break;
					}

					if ( WhrMode == LearnMode) {
						if ( group == 1) {

							if ( !COMopened[oldP->sensor])
								openCOMPort( oldP->sensor, &newP);

							if ( COMopened[oldP->sensor]) {
								Get_Measurement( oldP->sensor, &oldP->meas);

								for ( j=0; j<6; j++) {
									sprintf( valbuf[j], "%5.4f",
													whrptr->val[j] = oldP->meas.whr[j]);
									drawEditfield( &ef1[j], INACTIVE);
								}

								if ( oldP->running == INACTIVE)
									shutdownCOMPorts();
							}
						}

						if ( group == 5) {
							if ( namebuf[0] != 0) {

								if ( WhrYmax( whrptr->val[0], whrptr->val[1]) ||
										WhrYmin( whrptr->val[0], whrptr->val[1])) {

									displayError( 12);
									break;
								}

								strcpy( oldP->whrinfo.name, namebuf);

								for ( j = 0; j<6; j++)
									oldP->whrinfo.whr[j] = whrptr->val[j];

								alive = a_inner = 0;
							}
							else
								displayError( 13);
						}
						break;
					}
					break;

				default:
					if ( group == 0) {
						editEditfield( &ef0, ch);
						break;
					}

					if (( group > 1) && ( group < 5)) {
						editEditfield( &ef1[i], ch);
						whrptr->val[i]= atof( valbuf[i]);
						break;
					}

					if (( WhrMode == SaveMode) && ( ch == kbSpace)) {
						strcpy( namebuf, whrptr->name);
						drawEditfield( &ef0, INACTIVE);
						break;
					}

					if (( WhrMode == SaveMode) && ( ch == kbDel) && ( group==1)) {
						if ((( wl.ptr >= 3) && ( wl.count >= 3)) ||  wl.top) {

							errTxt[7].txt2 = whrptr->name;
							if ( displayError( 7) == YES) {

								whrptr = whrroot;

								for ( j=1; j < wl.ptr+wl.top; j++)
									whrptr = whrptr->next;

								whraux = whrptr->next;

								if ( whrptr->next->next)
									whrptr->next = whrptr->next->next;
								else
									whrptr->next = NULL;

								free( whraux);

								wl.count--;

								if (( wl.ptr + wl.top) >= wl.count)
									if ( --wl.top < 0) {
										wl.top = 0;
										wl.ptr--;
									}

								whrptr = whrroot;

								if (( stream =
										openFile( NAME_WHR_FILE, "wb", -1)) == NULL) {
									alive = 0;
									break;
								}

								while ( whrptr) {
									fwrite( whrptr, sizeof( *whrptr), 1, stream);
									whrptr = whrptr->next;
								}
								fclose( stream);

								whrtop = whrptr = whrroot;

								for ( i=0; i < wl.ptr+wl.top; i++) {

									if (( i < wl.top) && whrtop->next)
										whrtop = whrtop->next;

									if ( whrptr->next)
										whrptr = whrptr->next;
								}
								drawFileList( &wl, whrtop, ACTIVE);
							}
							break;
						}
					}
					break;
			}
		}
	} while( alive);

	closeWindow();

	whrptr = whrroot;

	while ( whrptr != NULL) {
		whrptr = whrroot->next;
		free( whrroot);
		whrroot = whrptr;
	}

	return( 0);
}


int doRestoreWhr( void) {

	int mode = WinPtr[NoWin]->mode;

	if ( NoWin) {
		if (( mode == CIEMode) || ( mode == RGBMode))
			return( openWhrWindow( RestoreMode, HlpRestoreWhrRef));

		return( displayError( 21));
	}
	return( displayError( 0));
}


int doSaveWhr( void) {

	if ( NoWin)
		return( openWhrWindow( SaveMode, HlpSaveWhrRef));

	return( displayError( 0));
}


int doLearnWhr( void) {

	int mode = WinPtr[NoWin]->mode;

	if ( NoWin) {
		if (( mode == CIEMode) || ( mode == RGBMode))
			return( openWhrWindow( LearnMode, HlpLearnWhrRef));

		return( displayError( 22));
	}
	return( displayError( 0));
}


int LearnRedPhosphor( WININFO* wptr, FILENODE* F) {

	MEASINFO* meas = &wptr->meas;

	if ( !COMopened[wptr->sensor])
		openCOMPort( wptr->sensor, wptr);

	if ( COMopened[wptr->sensor]) {
		Get_Measurement( wptr->sensor, meas);

		if ( meas->X > 0.01) {

			F->val[1] = meas->Y / meas->X;
			F->val[2] = meas->Z / meas->X;

			if ( wptr->running == INACTIVE)
					shutdownCOMPorts();

			return( 0xFF);
		}
		if ( wptr->running == INACTIVE);
			shutdownCOMPorts();

		displayError( 23);
	}
	return( 0);
}


int LearnGreenPhosphor( WININFO* wptr, FILENODE* F) {

	MEASINFO* meas = &wptr->meas;

	if ( !COMopened[wptr->sensor])
		openCOMPort( wptr->sensor, wptr);

	if ( COMopened[wptr->sensor]) {
		Get_Measurement( wptr->sensor, meas);

		if ( meas->Y > 0.01) {

			F->val[3] = meas->X / meas->Y;
			F->val[5] = meas->Z / meas->Y;

			if ( wptr->running == INACTIVE)
					shutdownCOMPorts();

			return( 0xFF);
		}
		if ( wptr->running == INACTIVE);
			shutdownCOMPorts();

		displayError( 23);
	}
	return( 0);
}


int LearnBluePhosphor( WININFO* wptr, FILENODE* F) {

	MEASINFO* meas = &wptr->meas;

	if ( !COMopened[wptr->sensor])
		openCOMPort( wptr->sensor, wptr);

	if ( COMopened[wptr->sensor]) {
		Get_Measurement( wptr->sensor, meas);

		if ( meas->Z > 0.01) {

			F->val[6] = meas->X / meas->Z;
			F->val[7] = meas->Y / meas->Z;

			if ( wptr->running == INACTIVE)
					shutdownCOMPorts();

			return( 0xFF);
		}
		if ( wptr->running == INACTIVE);
			shutdownCOMPorts();

		displayError( 23);
	}
	return( 0);
}


void calculatePhosphor( PHOSINFO* new, FILENODE* tmp) {

	int i;
	float aa, rr1;

	/* Calculate for RED phosphor */

	aa = tmp->val[6];

	for ( i=0; i<10; i++) {
		rr1 = tmp->val[3] - aa * tmp->val[5];
		aa = tmp->val[6] - rr1 * tmp->val[7];
	}
	new->pho[2] = - ( int)( aa  * int_scale);
	new->pho[1] = - ( int)( rr1 * int_scale);

	/* Calculate for GREEN phosphor */

	aa = tmp->val[7];

	for ( i=0; i<10; i++) {
		rr1 = tmp->val[1] - aa * tmp->val[2];
		aa = tmp->val[7] - rr1 * tmp->val[6];
	}
	new->pho[5] = - ( int)( aa  * int_scale);
	new->pho[3] = - ( int)( rr1 * int_scale);

	/* Calculate for BLUE phosphor */

	aa = tmp->val[5];

	for ( i=0; i<10; i++) {
		rr1 = tmp->val[2] - aa * tmp->val[1];
		aa = tmp->val[5] - rr1 * tmp->val[3];
	}
	new->pho[7] = - ( int)( aa  * int_scale);
	new->pho[6] = - ( int)( rr1 * int_scale);

	new->pho[0] = new->pho[4] = new->pho[8] = int_scale;
}


void drawLearnOk( int left, int top, int color) {
	setfillstyle( SOLID_FILL, color);
	bar( left+87, top, left+113, top+11);

	setcolor( EGA_WHITE);
	outtextxy( left+92, top+2, "OK");
}


int openPhosWindow( int PhosMode, int helptxt) {

	/*	0=Name, 1=List, 2=Sensor, 6=Restore, 7=Cancel, 8=Help
		0=Name, 1=List, 2=Sensor, 6=Save, 7=Cancel, 8=Help
		0=Name, 2=Sensor, 3=Red, 4=Green, 5=Blue, 6=Ok, 7=Cancel, 8=Help */

	WININFO* oldP = WinPtr[NoWin++];
	WININFO* newP = WinPtr[NoWin];

	EDITFIELD ef[2];			   /* Phosphor name  and sensor KU no */
	BUTTON bu[3];				   /* ( Ok, Save, Restore), Cancel, Help */
	BUTTON tm[3];				   /* Take Measurement */

	LIST wl;

	FILENODE* phoroot;			/* Pointer to start of list */
	FILENODE* photop;				/* Pointer to 1 element displayed in list */
	FILENODE* phoptr;				/* Pointer to actual element in list */
	FILENODE* phoaux;				/* Temporary pointer */

	FILENODE pho;

	FILE* stream;
	void* Image;
	char* headptr;

	int ch, i, j, group, a_inner, alive, xpos, ypos;
	int LearnDone[3] = { 0, 0, 0 };

	pho.next = NULL;
	pho.name[0] = pho.kuno[0] = 0;

	clearSensorId( newP);

	newP->running = oldP->running;
	newP->sensor = oldP->sensor;
	strcpy( newP->sensorid.KuNo, oldP->sensorid.KuNo);
	newP->mode = PhosMode;

	SetRect( &newP->rect, PhoLeft, PhoTop-WINDOWBARDEEP, PhoRight, PhoBottom);

	addEditfield( &ef[0], PholeftCol, PhoTop+30, 16, pho.name, NULL);
	addEditfield( &ef[1], PhorightCol, PhoTop+30, 10, pho.kuno, NULL);

	phoptr = phoroot = ( FILENODE *) malloc( sizeof( FILENODE));
	phoroot->next = NULL;

	wl.count = 0;

	if ( PhosMode != LearnMode) {
		if (( stream = openFile( NAME_PHO_FILE, "rb", -2)) == NULL) {

			if (( stream = openFile( NAME_PHO_FILE, "wb", -1)) == NULL) {
				free( phoptr);
				closeWindow();
				return( 0);
			}

			strcpy( phoptr->name, photxt[0]);
			strcpy( phoptr->kuno, "KU_fty.");

			for ( i=0; i<9; i++)
				phoptr->val[i] = EBUphosphor[i];

			fwrite( phoptr, sizeof( *phoptr), 1, stream);

			phoptr = phoptr->next = ( FILENODE *) malloc( sizeof( FILENODE));

			strcpy( phoptr->name, photxt[1]);
			strcpy( phoptr->kuno, "KU_fty.");

			for ( i=0; i<9; i++)
				phoptr->val[i] = SMPTECphosphor[i];

			fwrite( phoptr, sizeof( *phoptr), 1, stream);

			fclose( stream);

			phoptr->next = NULL;

			wl.count = 2;
		}
		else {
			fread( phoroot, sizeof( *phoroot), 1, stream);

			while ( !feof( stream)) {
				wl.count++;
				phoaux = phoptr;
				phoptr = phoptr->next = ( FILENODE *) malloc( sizeof( FILENODE));
				fread( phoptr, sizeof( *phoroot), 1, stream);
			}
			fclose( stream);
			free( phoptr);

			phoaux->next = NULL;
		}
	}

	photop = phoptr = phoroot;
	wl.top = wl.ptr = group = i = 0;

	alive = -1;

	switch ( PhosMode) {
		case LearnMode:
			headptr = "Learn Phosphor";

			addButtons( &bu[0], &bu[1], &bu[2], &newP->rect, Ok);
			addButton( &tm[0], PholeftCol, PhoTop+70,
												PholeftCol+75, PhoTop+85, " RED ", RED);
			addButton( &tm[1], PholeftCol, PhoTop+100,
												PholeftCol+75, PhoTop+115, "GREEN", GREEN);
			addButton( &tm[2], PholeftCol, PhoTop+130,
												PholeftCol+75, PhoTop+145, " BLUE", BLUE);
			strcpy( pho.name, "");
			strcpy( phoptr->kuno, "");
			break;

		case RestoreMode:
			group++;
			headptr = "Restore Phosphor";

			addButtons( &bu[0], &bu[1], &bu[2], &newP->rect, Restore);

			addList( &wl, PholeftCol, PhoTop+60, 14, 10, wl.count, 0, NULL);
			break;

		case SaveMode:
			headptr = "Save Phosphor";

			addButtons( &bu[0], &bu[1], &bu[2], &newP->rect, Save);

			addList( &wl, PholeftCol, PhoTop+60, 14, 10, wl.count, 0, NULL);
			strcpy( pho.name, oldP->P.phoName);
			break;
	}

	do {
		a_inner = -1;

		openWindow( newP, headptr);

		for ( j=0; j < 3; j++) {
			drawButton( &bu[j], INACTIVE);
		}

		if ( group > 5)
			drawButton( &bu[group-6], ACTIVE);

		setcolor( EGA_BLACK);
		outtextxy( PhorightCol, PhoTop+18, "Sensor ID:");

		switch ( PhosMode) {
			case SaveMode:
				outtextxy( PholeftCol, PhoTop+48, "Phosphors");

				if  ( group == 1)
					drawFileList( &wl, photop, ACTIVE);
				else
					drawFileList( &wl, photop, INACTIVE);

				setcolor( EGA_BLACK);
				outtextxy( PholeftCol, PhoTop+18, "Save phosphor");

				if ( group == 0)
					drawEditfield( &ef[0], ACTIVE);
				else
					drawEditfield( &ef[0], INACTIVE);

				strcpy( pho.kuno, oldP->P.KuNo);
				drawEditfield( &ef[1], INACTIVE);
				break;

			case RestoreMode:

				outtextxy( PholeftCol, PhoTop+48, "Phosphors");

				if ( group == 1)
					drawFileList( &wl, photop, ACTIVE);
				else
					drawFileList( &wl, photop, INACTIVE);

				setcolor( EGA_BLACK);
				outtextxy( PholeftCol, PhoTop+18, "Restore phosphor");

				strcpy( pho.name, phoptr->name);
				drawEditfield( &ef[0], INACTIVE);

				strcpy( pho.kuno, phoptr->kuno);
				drawEditfield( &ef[1], INACTIVE);

				if ( group < 6)
					drawButton( &bu[0], ACTIVE);
				break;

			case LearnMode:
				outtextxy( PholeftCol, PhoTop+18, "Name phosphor");

				if ( group == 0)
					drawEditfield( &ef[0], ACTIVE);
				else
					drawEditfield( &ef[0], INACTIVE);

				for ( j = 0; j< 3; j++) {
					drawButton( &tm[j], INACTIVE);
					rectangle( PholeftCol+85, PhoTop+70+j*30,
															PholeftCol+115, PhoTop+85+j*30);
				}

				if (( group > 2) && ( group < 6))
					drawButton( &tm[group-3], ACTIVE);

				rectangle( PhorightCol-59, PhoTop+70, PhorightCol+113, PhoTop+137);

				outtextxy( PhorightCol-55, PhoTop+74, "To learn a phosphor:");
				outtextxy( PhorightCol-55, PhoTop+84, "  Select one of the");
				outtextxy( PhorightCol-55, PhoTop+94, "  three buttons and");
				outtextxy( PhorightCol-55, PhoTop+104, "       press:");
				outtextxy( PhorightCol-55, PhoTop+114, " <SPACE> or <ENTER>");

				strcpy( pho.kuno, phoptr->kuno);
				drawEditfield( &ef[1], INACTIVE);
				break;
		}

		while ( a_inner) {
			switch ( ch = GetKey()) {
				case kbNoKey:
					break;

				case kbEsc:
					alive = a_inner = 0;
					break;

				case kbTab:
				case kbShiftTab:
					switch ( group) {
						case 0:
							drawEditfield( &ef[0], INACTIVE);
							break;
						case 1:
							drawFileList( &wl, photop, INACTIVE);
							break;
						case 2:
							break;
						case 3:
						case 4:
						case 5:
							drawButton( &tm[group-3], INACTIVE);
							break;
						default:
							drawButton( &bu[group-6], INACTIVE);
							break;
					}

					if ( ch == kbTab) {
						if ( ++group == 2)
							group++;
						if ( group > 8)
							group = 0;
						if (( PhosMode == LearnMode) && ( group == 1))
							group = 3;
						if (( PhosMode == RestoreMode) && ( group == 0))
							group = 1;
						if (( PhosMode == SaveMode) && ( group == 1) && !wl.count)
							group = 6;
					}
					else {
						if ( --group == 2)
							group--;
						if ( group < 0)
							group = 8;
						if (( PhosMode == LearnMode) && ( group == 1))
							group = 0;
						if (( PhosMode == RestoreMode) && ( group == 0))
							group = 8;
						if (( PhosMode == SaveMode) && ( group == 5) && !wl.count)
							group = 0;
					}

					if ( PhosMode != LearnMode) {
						if ( group == 3)
							group = 6;
						if ( group == 5)
							group = 1;

						if ( PhosMode == RestoreMode)
							drawButton( &bu[0], ACTIVE);
					}

					switch ( group) {
						case 0:
							drawEditfield( &ef[0], ACTIVE);
							break;
						case 1:
							drawFileList( &wl, photop, ACTIVE);
							break;
						case 2:
							break;
						case 3:
						case 4:
						case 5:
							drawButton( &tm[group-3], ACTIVE);
							break;
						default:
							drawButton( &bu[0], INACTIVE);
							drawButton( &bu[group-6], ACTIVE);
							break;
					}
					break;

				case kbUp:
				case kbDown:
				case kbHome:
				case kbEnd:
				case kbPgUp:
				case kbPgDn:
					switch ( group) {
						case 0:
							editEditfield( &ef[0], ch);
							break;

						case 1:
							if ( updateList( &wl, ch)) {

								phoptr = photop = phoroot;

								for ( i=0; i < wl.ptr+wl.top; i++) {

									if (( i < wl.top) &&  photop->next)
										photop = photop->next;

									if ( phoptr->next)
										phoptr = phoptr->next;
								}
								drawFileList( &wl, photop, ACTIVE);

								if ( PhosMode == RestoreMode) {

									strcpy( pho.name, phoptr->name);
									drawEditfield( &ef[0], INACTIVE);

									strcpy( pho.kuno, phoptr->kuno);
									drawEditfield( &ef[1], INACTIVE);
								}
							}
							break;
					}
					break;

				case kbF1:
					openHelp( helptxt);
					a_inner = 0;
					break;

				case kbSpace:

					if ( group == 0) {
						editEditfield( &ef[0], ch);
						break;
					}

					if ( group == 1) {
						if ( PhosMode == LearnMode)
							;
						if ( PhosMode == SaveMode) {
							strcpy( pho.name, phoptr->name);
							drawEditfield( &ef[0], INACTIVE);
							break;
						}
					}
					else
						if (( group > 2) && ( group < 6))
							;
					else
						break;

				case kbEnter:
					if (( group > 2) && ( group < 6)) {
						switch ( group) {
							case 3:
								errTxt[25].txt2 = "RED";
								break;
							case 4:
								errTxt[25].txt2 = "GREEN";
								break;
							case 5:
								errTxt[25].txt2 = "BLUE";
								break;
						}

						if ( displayError( 25) == kbEsc)
							break;

						xpos = tm[group-3].frame.right+50;
						ypos = tm[group-3].frame.top;

						Image = displayMessage( xpos, ypos,
																EGA_GREEN, "Please Wait!!");
						if ( group == 3)
							if ( LearnRedPhosphor( newP, &pho)) {
								LearnDone[0] = 0xFF;
								drawLearnOk( PholeftCol, PhoTop+72, EGA_RED);
							}

						if ( group == 4)
							if ( LearnGreenPhosphor( newP, &pho)) {
								LearnDone[1] = 0xFF;
								drawLearnOk( PholeftCol, PhoTop+102, EGA_GREEN);
							}

						if ( group == 5)
							if ( LearnBluePhosphor( newP, &pho)) {
								LearnDone[2] = 0xFF;
								drawLearnOk( PholeftCol, PhoTop+132, EGA_BLUE);
							}

						for ( j=0; j<3; j++)
							if ( LearnDone[j])
								strcpy( pho.kuno, newP->sensorid.KuNo);

						drawEditfield( &ef[1], INACTIVE);

						putimage( xpos, ypos, Image, COPY_PUT);
						free( Image);
						break;
					}

					if ( group == 7) {						/* Cancel */
						a_inner = alive = 0;
						break;
					}

					if ( group == 8) {						/* Help */
						openHelp( helptxt);
						a_inner = 0;
						break;
					}

					if ( PhosMode == SaveMode) {
						if ( group == 6) {
							if ( pho.name[0] != 0) {

								if ( !strcmpi( pho.name, photxt[0]) ||
									  !strcmpi( pho.name, photxt[1])) {
										displayError( 39);
										break;
									}

								phoaux = ( FILENODE *) malloc( sizeof( FILENODE));

								strcpy( phoaux->name, pho.name);
								strcpy( phoaux->kuno, pho.kuno);

								for ( j=0; j<6; j++)
									phoaux->val[j] = oldP->P.pho[j];

								phoptr = phoroot->next;

								do {
									photop = phoptr;
									phoptr = phoptr->next;
								} while ((( j = strcmpi( phoptr->name, pho.name)) < 0)
																			&& phoptr);
								if ( !j) {
									free( phoaux);

									if ( displayError( 24) == YES)
										for ( j=0; j<6; j++)
											phoptr->val[j] = oldP->P.pho[j];
									else {
										phoptr = photop = phoroot;
										a_inner = 0;
										break;
									}
								}
								else
									if ( j > 0) {
										phoaux->next = photop->next;
										photop->next = phoaux;
									}
							else {
									photop->next = phoaux;
									phoaux->next = NULL;
								}

								if (( stream =
										openFile( NAME_PHO_FILE, "wb", -1)) == NULL) {
									alive = 0;
									break;
								}

								phoptr = phoroot;

								while ( phoptr) {
									fwrite( phoptr, sizeof( *phoptr), 1, stream);
									phoptr = phoptr->next;
								}
								fclose( stream);

								alive = a_inner = 0;
							}
							else
								displayError( 14);
						}
						break;
					}

					if ( PhosMode == RestoreMode) {
						if (( group == 1) || ( group == 6)) {

							strcpy( oldP->P.phoName, phoptr->name);
							strcpy( oldP->P.KuNo, phoptr->kuno);

							for ( j = 0; j < 6; j++)
								oldP->P.pho[j] = phoptr->val[j];

							alive = a_inner = 0;
						}
						break;
					}

					if ( PhosMode == LearnMode) {
						if ( group == 6) {
							if ( LearnDone[0] && LearnDone[1] && LearnDone[2]) {
								if ( pho.name[0] != 0) {

									calculatePhosphor( &newP->P, &pho);

									for ( i = 0; i < 9; i++)
										oldP->P.pho[i] = newP->P.pho[i];

									strcpy( oldP->P.KuNo, newP->sensorid.KuNo);
									strcpy( oldP->P.phoName, pho.name);

									alive = a_inner = 0;
								}
								else
									displayError( 14);
								break;
							}
							else
								displayError( 16);
							break;
						}
						if (( group < 3) || ( group > 5))
							break;
					}
					else
						break;

				default:
					if ( group == 0) {
						editEditfield( &ef[0], ch);
						break;
					}

					if (( PhosMode == SaveMode) && ( ch == kbSpace)) {
						strcpy( pho.name, phoptr->name);
						drawEditfield( &ef[0], INACTIVE);
						break;
					}

					if (( PhosMode == SaveMode) && ( ch == kbDel) && ( group==1)) {
						if ((( wl.ptr >= 2) && ( wl.count >= 2)) || wl.top) {

							errTxt[8].txt2 = phoptr->name;
							if ( displayError( 8) == YES) {

								phoptr = phoroot;

								for ( j=1; j < wl.ptr+wl.top; j++)
									phoptr = phoptr->next;

								phoaux = phoptr->next;

								if ( phoptr->next->next)
									phoptr->next = phoptr->next->next;
								else
									phoptr->next = NULL;

								free( phoaux);

								wl.count--;

								if (( wl.ptr + wl.top) >= wl.count)
									if ( --wl.top < 0) {
										wl.top = 0;
										wl.ptr--;
									}
								phoptr = phoroot;

								if (( stream =
									openFile( NAME_PHO_FILE, "wb", -1)) == NULL) {
									alive = 0;
									break;
								}

								while ( phoptr) {
									fwrite( phoptr, sizeof( *phoptr), 1, stream);
									phoptr = phoptr->next;
								}
								fclose( stream);

								photop = phoptr = phoroot;

								for ( i=0; i < wl.ptr+wl.top; i++) {

									if (( i < wl.top) && photop->next)
										photop = photop->next;

									if ( phoptr->next)
										phoptr = phoptr->next;
								}
								drawFileList( &wl, photop, ACTIVE);
							}
						}
						break;
					}
					break;
			}
		}
	} while( alive);

	closeWindow();

	phoptr = phoroot;

	while ( phoptr != NULL) {
		phoptr = phoroot->next;
		free( phoroot);
		phoroot = phoptr;
	}

	return( 0);
}


int doRestorePhosphor( void) {

	WININFO* wptr = WinPtr[NoWin];

	if ( NoWin) {
		if (( wptr->mode == RGBMode) &&
			 (( wptr->RGBmode == RGBHor) || ( wptr->RGBmode == RGBVer)))
			return( openPhosWindow( RestoreMode, HlpRestorePhosphor));

		return( displayError( 10));
	}
	return( displayError( 0));
}


int doSavePhosphor( void) {

	WININFO* wptr = WinPtr[NoWin];

	if ( NoWin) {
		if (( wptr->mode == RGBMode) &&
			 (( wptr->RGBmode == RGBHor) || ( wptr->RGBmode == RGBVer)))
			return( openPhosWindow( SaveMode, HlpSavePhosphor));

		return( displayError( 11));
	}
	return( displayError( 0));
}


int doLearnPhosphor( void) {

	WININFO* wptr = WinPtr[NoWin];

	if ( NoWin) {
		if (( wptr->mode == RGBMode) &&
			 (( wptr->RGBmode == RGBHor) || ( wptr->RGBmode == RGBVer)))
			return( openPhosWindow( LearnMode, HlpLearnPhosphor));

		return( displayError( 9));
	}
	return( displayError( 0));
}
