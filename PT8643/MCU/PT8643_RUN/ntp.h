//-----------------------------------------------------------------------------
// User Configurable Settings
//-----------------------------------------------------------------------------

#define NTP_PORT				123				// Defines the UDP port assigned
                                                // to the Netfinder protocol

#define FRACTION				21474837 		// 34359738 // 2^32 / fractioncounter
												// 21474837 = Timer Interval (5ms) / NTP Interval pr. bit (32Bit) (232,..... pS)
								
#define NTP_BUFF_SIZE		   	128

#define GPS_LOCK_TIMEOUT		60				// Timeout value for GPS SYNC.

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
extern int ntp_socketno;
extern bit ntp_need_reset_socket;
extern unsigned int ntp_socket_timeout;
extern unsigned char ntp_reset_socket_delay;
extern unsigned long sec_counter;
extern unsigned long sec_preload_counter;
extern unsigned long sec_frac;
extern unsigned char gps_lock_timeout;
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
extern int ntp_start(void);
extern int ntp_process_packet(void);