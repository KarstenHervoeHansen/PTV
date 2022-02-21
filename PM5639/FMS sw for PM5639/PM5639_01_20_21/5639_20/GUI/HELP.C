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

#include <io.h>
#include "help.h"

static char helpbuffer[HELPBUFSIZE][HELPLINESIZE];

static fpos_t menuinfo[NDXSIZE];
static fpos_t indexinfo[NDXSIZE];

static FILE* stream;
static NDXTYPE getndx;
static int NDXread;
static char* txtptr;
static int HelpAvailable = INACTIVE;

int initHelpSystem( void) {

	struct ftime ft[2];

	int indexcount = 0, count = 0, ndx = 0, i;
	long checksum = 0, NDXchecksum;

	if (( stream = openFile( HELP_FILE, "rt", -1)) == NULL)
		return( 31);										/* Help file not found */

	getftime( fileno( stream), &ft[0]);
	fclose( stream);

	if (( stream = openFile( INDEX_FILE, "rb", -1)) == NULL)
		return( 32);									  /* Index file not found */

	fread( &NDXread, sizeof( NDXread), 1, stream);
	fread( &ft[1], sizeof( ft[1]), 1, stream);

	if ( memcmp( &ft[0], &ft[1], sizeof( ft[0]))) {
		fclose( stream);  	/* Date/time - stamp in NDX file does not */
		return( 33);    		/* match the date/time-stamp in the HLP-file */
	}

	while ( !feof( stream) && ( count < NDXread)) {

		fread( &getndx.filepos, sizeof( fpos_t), 1, stream);
		checksum += getndx.filepos;

		if ( !feof( stream)) {
			txtptr = &getndx.headline[0];
			i = 0;
			while (( *txtptr++ = fgetc( stream)) && ( i++ < HEADLINESIZE))
				;
		}

		if (( getndx.headline[0] != '_') || ( getndx.headline[1] != '_'))
			menuinfo[ndx++] = getndx.filepos;

		if (( getndx.headline[0] != '_') && ( getndx.headline[1] != '_'))
			indexcount++;

		if ( strstr( getndx.headline, "_EMPTY") == NULL)
			indexinfo[indexcount++] = getndx.filepos;

		count++;
	}

	fread( &NDXchecksum, sizeof( NDXchecksum), 1, stream);
	fclose( stream);

	if ( count != NDXread) {
										/* Number of menus in NDX file does not */
		return( 33);				/* match the number of menus in the HLP-file */
	}

	if ( checksum != NDXchecksum) {
										/* Checksum in NDX file does not match */
		return( 34);				/* the calculated checksum from the NDX-file */
	}

	HelpAvailable = ACTIVE;
	return( 0);
}


int GetHelpFile( fpos_t filepos) {

	int txtread=0, found=0, j, level;

	if ( HelpAvailable) {

		if (( stream = openFile( HELP_FILE, "rt", -1)) == NULL)
			return( txtread);

		fsetpos( stream, &filepos);
		fgets( &helpbuffer[0][0], HELPLINESIZE, stream);

		if ( helpbuffer[0][3] != '_') {
			if ( helpbuffer[0][2] == '_')
				level++;
		}
		else
			level = 2;

		while ( !feof( stream) && !found) {
			txtptr = fgets( &helpbuffer[txtread][0], HELPLINESIZE, stream);

			j = 0;
			while (( *txtptr != '\x0A') && ( *txtptr != '\x0D') &&
					 ( *txtptr != '\x1A') && ( ++j < HELPLINESIZE))
				txtptr++;

			*txtptr = '\x0';

			if (( helpbuffer[txtread][0] == '@') &&
				 ( helpbuffer[txtread][1] == '@')) {

				if ( helpbuffer[txtread][2] == '_') {
					if ( helpbuffer[txtread][3] != '_') {

						if ( level != 0) {
							txtread--;
							found = -1;
						}
					}
					else
						if ( level == 2) {
							txtread--;
							found = -1;
						}

					txtread--;
				}
				else
					if ( helpbuffer[txtread][3] != '_') {
						txtread--;
						found = -1;
					}
			}
			txtread++;
		}
		fclose( stream);
	}
	return( txtread);
}


int GetHelpIndex( void) {

	struct ftime ft;
	int i, j=0, hlpndx=0, indent=0;

	if ( HelpAvailable) {
		if (( stream = openFile( INDEX_FILE, "rb", -1)) == NULL)
			return( hlpndx);

		fread( &i, sizeof( i), 1, stream);
		fread( &ft, sizeof( ft), 1, stream);

		while ( !feof( stream) && ( j++ < NDXread)) {
			fread( &getndx.filepos, sizeof( fpos_t), 1, stream);

			if ( !feof( stream)) {
				txtptr = &getndx.headline[0];
				i = 0;
				while (( *txtptr++ = fgetc( stream)) && ( i++ < HEADLINESIZE))
					;
			}

			if ( strstr( getndx.headline, "_EMPTY") == NULL) {

				if ( getndx.headline[0] == '_') {
					if ( getndx.headline[1] != '_') {
						strcpy( &helpbuffer[hlpndx][0], " ");
						strcat( &helpbuffer[hlpndx][0], &getndx.headline[0]);

						helpbuffer[hlpndx++][1] = ' ';

						indent = 1;
					}
					else {
						if ( indent == 1) {
							strcpy( &helpbuffer[hlpndx][0], "   ");
							strcat( &helpbuffer[hlpndx][0], &getndx.headline[1]);
							helpbuffer[hlpndx++][3] = ' ';
						}
						else {
							strcpy( &helpbuffer[hlpndx][0], "  ");
							strcat( &helpbuffer[hlpndx++][0], &getndx.headline[2]);
						}
					}
				}
				else {
					helpbuffer[hlpndx++][0] = '\x0';
					strcpy( &helpbuffer[hlpndx++][0], &getndx.headline[0]);
					indent = 2;
				}
			}
		}
		fclose( stream);
	}
	return( hlpndx);
}


int openHelpIndex( void) {

	openHelp( HlpHelpIndex);
	return(0 );
}


int openHelpHelp( void) {

	openHelp( HlpHelpHelpOnHelp);
	return(0 );
}


int openHelp( int helptxt) {

	const depth = 17;

	BUTTON bu[3];
	LIST hl;

	int i, j, ch, nextline, menuhelp, indexmode = 0;
	int group, a_inner = -1, alive = -1;

	if ( HelpAvailable) {

		setcolor( EGA_BLACK);
		setlinestyle( SOLID_LINE, 0, THICK_WIDTH);
		setfillstyle( SOLID_FILL, WindowBkColor);

		bar( HlpLeft, HlpTop, HlpRight, HlpBottom);

		line( HlpRight+2, HlpTop+DROPSHADOW, HlpRight+2, HlpBottom+3);
		line( HlpLeft+DROPSHADOW, HlpBottom+2, HlpRight, HlpBottom+2);

		setcolor( EGA_BLUE);
		setlinestyle( SOLID_LINE, 0, NORM_WIDTH);

		for ( i=0; i < 6; i += 2)
			rectangle( HlpLeft+i, HlpTop+i, HlpRight-i, HlpBottom-i);

		line( HlpLeft+10, HlpTop+25, HlpRight-10, HlpTop+25);

		settextstyle( DEFAULT_FONT, HORIZ_DIR, 2);

		i = HlpLeft + HlpWidth/2 - ( strlen( "Help") << 3);
		outtextxy( i, HlpTop+8, "Help.");

		settextstyle( DEFAULT_FONT, HORIZ_DIR, 1);

		i = HlpLeft + ( HlpWidth/3. - 60)/2;

		addButton( &bu[0], i, HlpBottom-25,
										i+60, HlpBottom-10, "Ok", ButColor);
		i += HlpWidth/3.;
		addButton( &bu[1], i, HlpBottom-25,
										i+60, HlpBottom-10, "Cancel", ButColor);
		i += HlpWidth/3.;
		addButton( &bu[2], i, HlpBottom-25,
										i+60, HlpBottom-10, "Index", ButColor);

		addList( &hl, 0, 0, 0, depth, 0, 0, NULL);

		if ( MenuMode) {
			for ( menuhelp=1, i=0; i < ActiveMenu; i++)
				menuhelp += Menu[i]->count+1;

			if ( MenuMode == 2)
				menuhelp += Menu[ActiveMenu]->lastitem+1;

			hl.count = GetHelpFile( menuinfo[menuhelp]);
		}
		else {
			if ( helptxt == HlpHelpIndex) {
				indexmode = ACTIVE;
				hl.count = GetHelpIndex();
			}
			else
				hl.count = GetHelpFile( indexinfo[menuhelp=helptxt]);
		}

		while ( alive) {

			for ( i=0; i<3; i++)
				drawButton( &bu[i], INACTIVE);

			group = hl.top = hl.ptr = 0;

			drawButton( &bu[group], ACTIVE);

			setcolor( EGA_BLACK);
			setfillstyle( SOLID_FILL, WindowBkColor);
			bar( HlpLeft+5, HlpTop+30-1, HlpRight-5, HlpBottom-30);

			nextline = HlpTop+30;

			for ( i=0; i < hl.depth; i++, nextline += 10)
				if ( i < hl.count) {
					if ( indexmode && ( i == 1)) {

						setcolor( EGA_WHITE);
						setfillstyle( SOLID_FILL, EGA_GREEN);

						bar( HlpLeft+15, nextline-1, HlpRight-15, nextline+7);
						outtextxy( HlpLeft+20, nextline, &helpbuffer[i][0]);

						setcolor( EGA_BLACK);
						setfillstyle( SOLID_FILL, WindowBkColor);

						hl.ptr = i;
					}
					else
						outtextxy( HlpLeft+20, nextline, &helpbuffer[i][0]);
				}

			a_inner = -1;

			while ( a_inner) {
				switch ( ch = GetKey()) {
					case kbNoKey:
						break;

					case kbEsc:
						alive = a_inner = 0;
						break;

					case kbEnter:
						switch ( group) {
							case 0:
								if ( indexmode) {
									menuhelp = hl.top+hl.ptr;
									hl.count = GetHelpFile( indexinfo[menuhelp]);
									indexmode = INACTIVE;
									a_inner = 0;
									break;
								}										/* break IS missing */
							case 1:
								a_inner = alive = 0;
								break;
							case 2:
								hl.count = GetHelpIndex();
								indexmode = ACTIVE;
								a_inner = 0;
								break;
						}
						break;

					case kbShiftTab:
					case kbTab:
						drawButton( &bu[group], INACTIVE);

						if ( ch == kbTab) {
							if ( ++group > 2)
								group = 0;
						}
						else {
							if ( --group < 0)
								group = 2;
						}
						drawButton( &bu[group], ACTIVE);
						break;

					case kbEnd:
					case kbHome:
					case kbPgUp:
					case kbPgDn:
					case kbUp:
					case kbDown:
						if ( indexmode) {
							updateList( &hl, ch);

							if ( helpbuffer[hl.ptr+hl.top][0] == '\x0')
								updateList( &hl, ch);

							if (( hl.top == 0) && ( hl.ptr == 0))
								hl.ptr = 1;
						}
						else {
							if ( ch == kbDown)
								hl.top = max( 0, min( hl.top++, hl.count-hl.depth));
							if ( ch == kbUp)
								hl.top = max( hl.top--, 0);

							updateList( &hl, ch);
						}

						nextline = HlpTop+30;

						for ( i=0; i < hl.depth; i++, nextline += 10) {
							if ( i < hl.count) {

								if ( i == hl.ptr)
									if ( indexmode) {
										setcolor( EGA_WHITE);
										setfillstyle( SOLID_FILL, EGA_GREEN);
									}
								bar( HlpLeft+15, nextline-1,
																HlpRight-15, nextline+7);
								outtextxy( HlpLeft+20, nextline,
																&helpbuffer[i+hl.top][0]);
								if ( i == hl.ptr)
									if ( indexmode) {
										setcolor( EGA_BLACK);
										setfillstyle( SOLID_FILL, WindowBkColor);
									}
							}
						}
						break;
				}
			}
		}
		drawWindows();
		return( 0);
	}

	return( displayError( 35));
}


