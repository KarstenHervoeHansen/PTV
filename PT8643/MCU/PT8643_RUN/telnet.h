//-----------------------------------------------------------------------------
// telnet.h
//-----------------------------------------------------------------------------
//
// Description:
// 	This file contains constants and function prototypes for the Telnet Server.
//


//-----------------------------------------------------------------------------
// User Configurable Settings
//-----------------------------------------------------------------------------

#define telnet_TX_BUFF_SIZE		256				// Size of the Transmit Buffer

#define telnet_RX_BUFF_SIZE		128				// Size of the Recieve Buffer

#define telnet_CMD_BUFF_SIZE	32				// Size of the buffer used for command interpretation.

// Possible Socket States
#define DISCONNECTED    0
#define IDLE			1
#define CONNECTED       2
#define LOGIN_Name		3
#define LOGIN		    4
#define COMMAND			5
#define DATA			6
#define DISABLED	    7						// Telnet socket will NOT restart. Used when changing IP settings.

#define TELNETTIMEOUT   24000					// Telnet inactivity timeout = 2 minutes. (Each tick is 5mS.)

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
extern int telnet_start(void);             
extern void telnet_process_packet(void);
extern SCHAR telnet_server_idle(void);
extern void telnet_socket_closed(void);
extern char telnet_SendTXBUFF(unsigned int length);
extern void telnet_CMD_BUFF_PUTBYTE(unsigned char c);
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
extern int telnet_socketno;
extern unsigned char telnet_socketstate;
extern unsigned char telnet_TX_BUFF[telnet_TX_BUFF_SIZE];
extern unsigned char telnet_RX_BUFF[telnet_RX_BUFF_SIZE];

extern unsigned int telnet_send_length; 
extern bit telnet_send_now;
extern unsigned int TELNET_PORT;				// Telnet Port Number. 

extern unsigned long telnet_InactivityTimer;