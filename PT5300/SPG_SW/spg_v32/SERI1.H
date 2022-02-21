/* SERI1.H */
#define UI unsigned int
#define UC unsigned char
#define UL unsigned long

extern void serial1_init(void);
extern interrupt void serial1_int(void);
extern void sndchar(UC adr, char A, char B, UC x) ;
extern void sndint(UC adr, char A, char B, UI x);
extern void sndlong(UC adr, char A, char B, UL x);
extern void return_long_int(UL x, UI y);
extern void return_long(UL x);
extern void return_int(UI y);
extern void return_char(UC x);

extern void sndvoid(UC adr, char A, char B);
extern void sndasc(UC adr, char A, char B, const char *st);
extern void put_char1(UC c);
extern char get_char1(void);

extern void putdata0(char A,char B,unsigned char x);
extern void putdata0_16(char A,char B,unsigned int x);
extern void putcmd(char A,char B);
extern void return_string(UC *s);
