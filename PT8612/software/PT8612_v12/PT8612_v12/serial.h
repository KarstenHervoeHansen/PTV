/* serial.h */

UC   getch(UC *);
UC   get(UC *);
bit  serhit(void);
void serint0(void);
void putch(UC);
void flush(void);
void serinit(void);
void Timer2_Init (int);
void Timer2_ISR (void);
void delay(void);
UC   crc8(UC*, UC);


