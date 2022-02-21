/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		UTIL.H																		*/
/*	Author:		Kim Engedahl, VDE															*/
/*	Org. date:	968628																		*/
/*	Rev. date:	971215, KEn VDE															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	Changes:																						*/
/* 971215: Altered parameters in FindNextASCII() & CheckNextASCII()			*/
/* 971014: Added RS232CheckOK()															*/
/* 971003: Added Cstrpy()																	*/
/* 971001: Added parameter list for ROMCheckOK()									*/
/* 970919: Added BitPos()																	*/
/* 970918: Added RAMCheckOK()																*/
/* 970724: Added ROMCheckOK()																*/
/* 970625: Added CheckTextASCII()														*/
/* 970127: Added VoltageADCErrorType													*/
/* 970106: Added ErrorHandling															*/
/* 961222: _ultoa() now returns a void, instead of UC								*/
/***************************************************************************/

#define SystemErrorType 	  	  0
#define AnlBlkErrorType		 	 10
#define SDIBlkErrorType		 	 20
#define AES_EBUErrorType	 	 30
#define AnlSignalErrorType	 	 40
#define SDISignalErrorType	 	 50
#define SDIGenlockErrorType 	 60
#define TimeCodeErrorType	 	 70
#define SPGErrorType			 	 80
#define ROMMemoryErrorType	 	 90
#define RAMMemoryErrorType		100
#define KeyboardErrorType		110
#define LCD_ADCErrorType		120
#define TempADCErrorType 		130
#define VoltageADCErrorType 	140

extern void NONE();
extern UC BitPos( UC value);
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
extern char FindNextASCII( UC Direction, UC Type, char ASCIIChar);
extern bit CheckTextASCII( UC Type, char ASCIIChar);
extern bit ROMCheckOK( UI* Checksum);
extern bit RAMCheckOK();
extern bit RS232CheckOK( UC Baudrate);
