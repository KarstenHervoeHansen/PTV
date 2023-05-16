/*
 * Physical Layer Module -- IBM-PC Serial Interface
 * Employs Hardware Interrupts and direct UART access.
 *
 * Programmer: Brian R. Anderson
 * Date: Sept. 29, 1990
 *
 * Modified: December 17, 1990 -- (Support for COM3 & COM4)
 * Modified: December 10, 1993 -- (Monitor handshake)
 */


#define COM1 0
#define COM2 1
#define COM3 2
#define COM4 3

#define TRUE 1
#define FALSE 0

enum { NONE, ODD, filler, EVEN };

/* initialize baud rate, data bits, parity, and stop bits */
void initcom (int port, long baud, int data, int parity, int stop);

/* start receiving -- install interrupt service routine */
void start (int port);

/* stop receiving -- remove ISR, disable interrupts */
void stop (int port);

/* determine if UART is able to accept another character */
int txstat (int port);

/* send one character to the UART */
void tx (int port, char c);

/* determine if there is a character waiting in the receive buffer */
int rxstat (int port);

/* get the next character from the receive buffer */
char rx (int port);

/* determine status of Clear To Send */
int ctsstat (int port);

/* determine status of Ring Indicator */
int ristat (int port);

/* determine status of Data Carrier Detect */
int dcdstat (int port);

/* determine status of Data Set Ready */
int dsrstat (int port);