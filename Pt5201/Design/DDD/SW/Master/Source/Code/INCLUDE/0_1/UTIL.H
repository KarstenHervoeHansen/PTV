/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997			*/
/*	Project:		PT5210 VariTime digital sync generator, master	*/
/*	Module:		UTIL.H							*/
/*	Author:		Kim Engedahl, VDE					*/
/*	Org. date:	968628							*/
/*	Rev. date:	980609, KEn VDE						*/
/*	Status:		Version 2.1						*/
/*										*/
/*	Changes:								*/
/* 980609: Replaced RAMCheckOK() with PowerUpRAMCheckOK and 			*/
/*				DiagnoseRAMChecklOK()				*/
/* 971014: Added RS232CheckOK()							*/
/* 971003: Added Cstrpy()							*/
/* 971001: Added parameter list for ROMCheckOK()				*/
/* 970919: Added BitPos()							*/
/* 970918: Added RAMCheckOK()							*/
/* 970724: Added ROMCheckOK()							*/
/* 970625: Added CheckTextASCII()						*/
/* 970127: Added VoltageADCErrorType						*/
/* 970106: Added ErrorHandling							*/
/* 961222: _ultoa() now returns a void, instead of UC				*/
/***************************************************************************/

extern void None(void);
extern UC BitPos( UC value);
extern UC htol( char* s, UL* result);
extern UC otol( char* s, UL* result);
extern UC btol( char* s, UL* result);
extern void _ultoa( UL val, char* result);
extern size_t Cstrlen( register char code* s);
extern char* Cstrcat( register char* to, register char code* from);
extern void* Cmemcpy( void* d1, void code* s1, register size_t n);
extern char* Cstrcpy( char* to, register char code* from);
extern int Cstrcmp( register const char * s1, register char code* s2);
extern int Cstrncmp( register const char * s1, register char code* s2, size_t len);
extern char FindNextASCII( UC Direction, UC LimitedType, char ASCIIChar);
extern bit CheckTextASCII( UC LimitedType, char ASCIIChar);
