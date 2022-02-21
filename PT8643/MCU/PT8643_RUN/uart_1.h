// Memmory space allocation for RX buffer

typedef unsigned char xdata Tserbuffer;

#define RX1BUFSIZE  200

extern unsigned char RXTimeout;					// When timeout a whole u-Blox packet has been received.
extern unsigned int RXcount;
extern void UART1(void);

void putch(unsigned char c);
//void Flush_COMbuffers(void);
//unsigned char COMGetByte(void);
unsigned char getch(unsigned char * c);
