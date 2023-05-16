/* SERIAL1.H */
#define UI unsigned int
#define UC unsigned char
#define UL unsigned long

extern void Serial0_Init(void);
extern void Serial1_Init(void);
extern interrupt void Serial0_int(void);
extern interrupt void Serial1_int(void);
extern void PutData0(char A,char B,unsigned char x) ;
extern void SndChar(UC adr, char A, char B, UC x) ;
extern void SndInt(UC adr, char A, char B, UI x);
extern void SndLong(UC adr, char A, char B, UL x);
extern void SndVoid(UC adr, char A, char B);
extern void SndAsc(UC adr, char A, char B, const char *st);
extern void PutData0_16(char A,char B,unsigned int x) ;
extern void PutCmd(char A,char B) ;
extern void put_char1(UC c);
