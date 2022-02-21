//-----------------------------------------------------------------------------
//	Filename	: UART_0.h
//	Date		: 2010-05-31
//	Name		: BS
//	Processor	: C8051F132
//	Copyright	: DK-Technologies A/S
//-----------------------------------------------------------------------------

// Size of TX and RX ring buffer.
#define TX0BUFSIZE		0x80	//0x100
#define RX0BUFSIZE		0x40	//0x100

// Memmory space allocation for TX and RX buffer.
//#define TX0BUF_IDATA
//#define TX0BUF_PDATA
#define TX0BUF_XDATA

//#define RX0BUF_IDATA
//#define RX0BUF_PDATA
#define RX0BUF_XDATA

#if defined TX0BUF_IDATA
	typedef unsigned char idata Ttx0buffer;
#elif defined TX0BUF_PDATA
	typedef unsigned char pdata Ttx0buffer;
#elif defined TX0BUF_XDATA
	typedef unsigned char xdata Ttx0buffer;
#endif // TX0BUF_IDATA

#if defined RX0BUF_IDATA
	typedef unsigned char idata Trx0buffer;
#elif defined RX0BUF_PDATA
	typedef unsigned char pdata Trx0buffer;
#elif defined RX0BUF_XDATA
	typedef unsigned char xdata Trx0buffer;
#endif // RX0BUF_IDATA

//-----------------------------------------------------------------------------
// Externs
//-----------------------------------------------------------------------------
extern unsigned int TX0count;				// Number of bytes to transmit to PC.
extern unsigned int RX0count;				// Number of data recieved from PC.
extern unsigned int TX0Ready;						// TX buffer has data to transmit.
extern unsigned int RX0Ready;						// RX buffer has room to receive.

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


extern Ttx0buffer TX0Buffer[ TX0BUFSIZE ];		// Ring buffer for TX. MCU --> PC
extern Trx0buffer RX0Buffer[ RX0BUFSIZE ];		// Ring buffer for RX. PC  --> MCU

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
