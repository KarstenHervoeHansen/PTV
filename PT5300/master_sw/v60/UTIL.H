/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998            */
/*  Project:    PT5230 Digital Video Generator                    */
/*  Module:   UTIL.H                                    */
/*  Author:   Kim Engedahl, DEV                             */
/*  Org. date:  980212                                    */
/*  Rev. date:  980724, KEn, DEV                              */
/*  Status:   Version 1.1                                 */
/*                                                  */
/*  Changes:                                            */
/* 980724: Added routines for time offset.                      */
/* 980721: Update TimeFormat() and DateFormat()                   */
/* 980616: Released as 4008 002 06812                         */
/* 980514: Released as 4008 002 06811                         */
/***************************************************************************/

extern void NONE();
extern UC htol( char* s, UL* result);
extern UC otol( char* s, UL* result);
extern UC btol( char* s, UL* result);
extern void _ultoa( UL val, char* result);

extern size_t Cstrlen( register code char* s);
extern char* Cstrcat( register char* to, register code char* from);
extern void* Cmemcpy( void* d1, code void* s1, register size_t n);
extern char* Cstrcpy( char* to, register code char * from);
extern int Cstrcmp( register const char * s1, register code char * s2);
extern int Cstrncmp( register const char * s1, register code char * s2, size_t len);

extern char GetNextTextASCII( UC HWtype, UC direction, UC ASCIIChar);
extern char GetNext(UC direction, UC ASCIIChar);
extern char GetNextPresetASCII( UC direction, UC ASCIIChar);

extern void TimeFormat( UC format, UL TimeInf, UC TCIFormat, char* TimeTxt);
extern UC ValidTime( UL TimeInf);
extern UL AdjustTime( UC ndx, UC dir, UC format, UL TimeInf);

extern void DateFormat( UC format, UL DateInf, UC TCIFormat, char* DateTxt);
extern UC ValidDate( UL DateInf);
extern UL AdjustDate( UC ndx, UC dir, UC format, UL DateInf);

extern void TimeOffsetFormat( UL time, char* TimeTxt);
extern UL AdjustTimeOffset( UC ndx, UC dir, UL TimeInf);

extern int ROMCheckOK( UI* Checksum);
extern bit PowerUpRAMCheckOK();
extern bit DiagnoseRAMCheckOK();

extern bit RS232CheckOK( UC Baudrate);
extern char FindNextASCII(UC, UC, char);
