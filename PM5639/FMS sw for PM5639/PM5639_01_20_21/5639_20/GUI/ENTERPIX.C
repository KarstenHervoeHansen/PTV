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

main() {
	FILE* stream1;
	FILE* stream2;
	fpos_t pos;
	volatile int found;
	int ch;

	stream1 = fopen( "PM39_20.EXE", "rb");

	found = 0;
	ch = 'w';

	while ( !found && !feof( stream1)) {

		while ((( ch = fgetc( stream1)) != 'Q') && !feof( stream1))
			;

		fgetpos( stream1, &pos);

		if ( ch == 'Q')
			if (( ch = fgetc( stream1)) == 'Q')
				if (( ch = fgetc( stream1)) == 'Q') {
					fclose( stream1);
					pos--;
					found = 1;
				}
	}

	stream1 = fopen( "PM39_20.EXE", "r+");
	stream2 = fopen( "KU_PIXEL.DAT", "rb");

	fsetpos( stream1, &pos);

	while ( !feof( stream2))
		fputc( ch = fgetc( stream2), stream1);

	fclose( stream1);
	fclose( stream2);

	ch = getch();
}
