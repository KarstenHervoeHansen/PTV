/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998            */
/*  Project:    PT5230 Digital Video Generator                    */
/*  Module:   XADRIVR.H                                 */
/*  Author:   P.H.Seerden, Application note: AN95040                */
/*  Org. date:  950105, P.H.Seerden, Philips Semiconductors B.V.        */
/*  Rev. date:  980616, KEn DEV                             */
/*  Status:   Version 0.5                                 */
/*                                                  */
/*  This module contains exported declarations of the XA, Uart Driver     */
/*  package. Include this module in your source file if you want to make    */
/*  use of one of the interface functions of the package.             */
/*                                                  */
/*  History:                                            */
/* 06-16-98   KEn, Rx_Count() & Tx_Count() now returns UC instead of char */
/* 06-15-98   KEn, increased UDR_RXBUFSIZE from 64 to 128           */
/* 10-15-97   KEn, added 19200 baud & handshake NONE                */
/* 10-10-97   KEn, added parameters to Udr_InitializeUart()         */
/* 03-03-97   KEn, added UDR_PARITY & UDR_FRAMING                 */
/*  04-10-96    KEn, modified version for the XA processor            */
/*  05-01-95    P.H. Seerden, Initial version                     */
/***************************************************************************/

#define UC unsigned char
#define UI unsigned int

#define FALSE 0
#define TRUE  1

#define UDR_INT_LEVEL 0     // For both UART- and CTS-interrupt

#define UDR_FLOWSPACE 2
#define UDR_RXBUFSIZE 128
#define UDR_TXBUFSIZE 32

#define XON         0x11    // character defined as XON
#define XOFF        0x13    // character defined as XOFF
#define LOCK        0x0C    // character defined as local lockout switch

                      // User programmable baudrates
enum UDR_Baudrate   { UDR_BAUD_300 = 0, UDR_BAUD_600, UDR_BAUD_1200,\
                UDR_BAUD_2400, UDR_BAUD_4800, UDR_BAUD_9600,\
                UDR_BAUD_19200 };

                      // User programmable parity 
enum UDR_Parity     { UDR_NONE = 0, UDR_ODD, UDR_EVEN };

                      // User programmable number of databits
enum UDR_Databit    { UDR_DATA7 = 7, UDR_DATA8 };

                      // User programmable handshake
enum UDR_Handshake  { UDR_NO_HANDSHAKE = 0, UDR_XON_XOFF, UDR_RTS_CTS };

// Status Messages/Errors

#define UDR_OK      ( char)  0
#define UDR_EOF   ( char) -1
#define UDR_OVL   ( char) -2

#define UDR_OVERRUN ( char) -3
#define UDR_PARITY  ( char) -4
#define UDR_FRAMING ( char) -5
#define UDR_BREAK   ( char) -6


//**************************************************************************
//      I N T E R F A C E   F U N C T I O N   P R O T O T Y P E S     *
//**************************************************************************

extern char Udr_InChar( void);
extern void Udr_PutChar( char ch);
extern void Udr_PutNChar( char *s);
extern void Udr_CPutNChar( code char *s);
extern void Udr_PutNewline( void);

extern void Udr_InitializeUart( UC Baudrate, UC Databit, UC Parity, UC Handshake);
extern UC Udr_RxCount( void);
extern char Udr_RxFlush( char num);
extern UC Udr_TxCount( void);
extern char Udr_TxFlush( char num);
