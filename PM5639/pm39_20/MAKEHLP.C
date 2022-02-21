#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include <dos.h>
#include <io.h>
#include <string.h>
#include <bios.h>
#include <dir.h>

#define NDXSIZE   100
#define HEADLINESIZE  40

#define HELPBUFSIZE 100
#define HELPLINESIZE  50


typedef struct {
  char headline[HEADLINESIZE];
  fpos_t filepos;
} NDXTYPE;


FILE* stream;
char txtbuffer[HELPBUFSIZE][HELPLINESIZE];
NDXTYPE ndxinfo[NDXSIZE];


int MakeHelpIndexFile( void) {

  struct ftime ft;
  int j, i = 0;
  long checksum = 0;
  char* txtptr;

  if (( stream = fopen( "PM5639.HLP", "rt")) != NULL) {
    getftime( fileno( stream), &ft);

    while ( !feof( stream) && ( i < NDXSIZE)) {
      fgetpos( stream, &ndxinfo[i].filepos);
      txtptr = fgets( txtbuffer, HEADLINESIZE+1, stream);

      j = 0;
      while (( *txtptr != '\x0A') && ( *txtptr != '\x0D') &&
           ( *txtptr != '\n') && ( j < HEADLINESIZE)) {
        txtptr++;
        j++;
      }
      *txtptr = '\x0';

      if (( txtbuffer[0][0] == '@') && ( txtbuffer[0][1] == '@'))
        strcpy( &ndxinfo[i++].headline, &txtbuffer[0][2]);
    }
    fclose( stream);
  }
  else
    return( -1);

  if (( stream = fopen( "PM5639.NDX", "wb")) != NULL) {

    fwrite( &i, sizeof( i), 1, stream);
    fwrite( &ft, sizeof( ft), 1, stream);

    for ( j=0; j < i; j++) {
      fwrite( &ndxinfo[j].filepos, sizeof( fpos_t), 1, stream);
      fwrite( &ndxinfo[j].headline[0],
            strlen( ndxinfo[j].headline)+1, 1, stream);

      checksum += ndxinfo[j].filepos;
    }
    fwrite( &checksum, sizeof( checksum), 1, stream);

    fclose( stream);
  }
  else
    return( -2);

  return( 0);
}


int GetHelpIndexFile( void) {

  struct ftime ft[2];
  int i = 0, j, NDXread;
  long checksum = 0, NDXchecksum;
  char* txtptr;

  if (( stream = fopen( "PM5639.HLP", "rt")) != NULL) {
    getftime( fileno( stream), &ft[0]);
    fclose( stream);
  }
  else
    return( -1);

  if (( stream = fopen( "PM5639.NDX", "rb")) != NULL) {
    fread( &NDXread, sizeof( NDXread), 1, stream);
    fread( &ft[1], sizeof( ft[1]), 1, stream);

    if ( memcmp( &ft[0], &ft[1], sizeof( ft[0]))) {
      /* Date/time - stamp in NDX file does not match the date/time-stamp
        in the HLP-file */

      fclose( stream);
      return( -3);
    }

    while ( !feof( stream) && ( i < NDXread) && ( i < NDXSIZE)) {
      fread( &ndxinfo[i].filepos, sizeof( fpos_t), 1, stream);
      checksum += ndxinfo[i].filepos;

      if ( !feof( stream)) {
        txtptr = &ndxinfo[i++].headline;
        j = 0;
        while (( *txtptr++ = fgetc( stream)) && ( j++ < HEADLINESIZE))
          ;
      }
    }
    fread( &NDXchecksum, sizeof( NDXchecksum), 1, stream);

    fclose( stream);
  }
  else
    return( -2);

  if ( i != NDXread) {
    /* Number of menus in NDX file does not match the number of menus
      in the HLP-file */
    return( -3);
  }

  if ( checksum != NDXchecksum) {
    /* Checksum in NDX file does not match the calculated checksum from
      the NDX-file */
    return( -3);
  }
  return( NDXread);
}

main() {
  int i, j;

  if ( MakeHelpIndexFile() == -1)
    printf( "\nError opening PM5639.HLP file\n");
  else
    if ( MakeHelpIndexFile() == -2)
      printf( "\nError opening PM5639.NDX file\n");
  else {
    i = GetHelpIndexFile();
    if ( i == -1)
      printf( "\nError opening PM5639.HLP file\n");
    else
      if ( i == -2)
        printf( "\nError opening PM5639.NDX file\n");
    else {
      printf( "\nNumber of helpmenus read:  %d\n", i);
      printf( "\nWait 1 sec..\n\n\n");

      delay( 1000);

      for ( j=0; j<i; j++)
        printf( "\n%s\t%d", ndxinfo[j].headline, ndxinfo[j].filepos);
    }
  }
  return( 0);
}