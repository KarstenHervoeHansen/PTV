/*
********************************************************************
* Project	: PT5210  MASTER
* Filename	: SERIAL1.H
* Version	: 0.0	961209
* Purpose	: HEADER FILE FOR SERIAL1.C
* Org.date	: 960903
* Author	: PRC
********************************************************************
HISTORY:
961209 Changed AnswerCheck from bit to UC
961111 Added SndRequest_n(), SndSpecial(), SndAsc_n()
961101 Added UC AnswerBuf[]; bit AnswerCheck(void); near UC rstate;
*/
#define UI unsigned int
#define UC unsigned char
#define UL unsigned long

extern void Serial0_init(void);
extern void Serial1_init(void);
extern interrupt void Serial0_int(void);
extern interrupt void Serial1_int(void);
//extern void PutData0(char A,char B,unsigned char x) ;
extern UC SndChar(UC adr, char* A, UC x) ;
extern UC SndInt(UC adr, char* A, UI x);
extern UC SndLong(UC adr, char* A,UL x);
extern UC SndVoid(UC adr, char* A);
extern UC SndRequest(UC adr, char* A);
extern UC SndRequest_n(UC adr, char* A, UC n);
extern UC SndSpecial(UC adr, char* A, char C);
extern UC SndAsc(UC adr, char* A, const char *st);
extern UC SndAsc_n(UC adr, char* A, char C, const char *st);
//extern void PutData0_16(char A,char B,unsigned int x) ;
//extern void PutCmd(char A, char B) ;
extern void put_char1(char c);
extern near char AnswerBuf[];
extern UC AnswerCheck(void);
extern near UC rstate;
