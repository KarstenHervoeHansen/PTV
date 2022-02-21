//-----------------------------------------------------------------------------
//	Filename	: UART_0.h
//	Date		: 2010-05-31
//	Name		: BS
//	Processor	: C8051F132
//	Copyright	: DK-Technologies A/S
//-----------------------------------------------------------------------------

// Size of TX and RX ring buffer.
#define TXBUFSIZE		0x100	//0x100
#define RXBUFSIZE		0x100	//0x100

// Memmory space allocation for TX and RX buffer.
//#define TXBUF_IDATA
//#define TXBUF_PDATA
#define TXBUF_XDATA

//#define RXBUF_IDATA
//#define RXBUF_PDATA
#define RXBUF_XDATA

#if defined TXBUF_IDATA
	typedef unsigned char idata Ttxbuffer;
#elif defined TXBUF_PDATA
	typedef unsigned char pdata Ttxbuffer;
#elif defined TXBUF_XDATA
	typedef unsigned char xdata Ttxbuffer;
#endif // TXBUF_IDATA

#if defined RXBUF_IDATA
	typedef unsigned char idata Trxbuffer;
#elif defined RXBUF_PDATA
	typedef unsigned char pdata Trxbuffer;
#elif defined RXBUF_XDATA
	typedef unsigned char xdata Trxbuffer;
#endif // RXBUF_IDATA

//-----------------------------------------------------------------------------
// Externs
//-----------------------------------------------------------------------------
extern unsigned int TXcount;			// Number of bytes to transmit to PC.
extern unsigned int RXcount;			// Number of data recieved from PC.
extern bit TXReady;						// TX buffer has data to transmit.
extern bit RXReady;						// RX buffer has room to receive.

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
void Flush_COM0buffers(void);
unsigned char COM0GetByte(void);
void COM0PutByte(unsigned char dt);
void COM0GetByte_ISR(unsigned char dt);
unsigned char COM0PutByte_ISR(void);
void Transmit_COM0(void);
void str2COM0(unsigned char *dat_ptr, unsigned char length);

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
