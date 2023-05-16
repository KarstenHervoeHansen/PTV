/* uart.h */
void   serinit(void);
void   serint0(void);
void   serinit(void);
char   serout(char);
char  serinp(void);
bit    serhit(void);
bit    collect_sesor_data(char xdata*);
void   serflush(void);
char putchar(char c);

