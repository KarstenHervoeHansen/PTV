#define buffer_length 300  // Need to be 200 to read signal quality

typedef struct packet_tag{
	unsigned char class_num;
	unsigned char id_num;
	unsigned char l_lo;
	unsigned char l_hi;
	unsigned char payload[buffer_length];
} PACKET;

//GPS tids info
extern union {
	unsigned long GPS_tow_long;
	unsigned char GPS_tow_bytes[4];
}  xdata GPS_tow;


//UTC Time Of Week
extern union {
	unsigned long UTC_tow_long;
	unsigned char UTC_tow_bytes[4];
}  xdata UTC_tow;

//structur for den interne tid
typedef struct time {
	int year;
	char month;
	char day;
	char hour;
	char min;
	char sec;
};


// Structure for leap second information.
typedef struct leap_info {
	unsigned int utc_week_number;	// 28 I2 - utcWNF - UTC - week number when next leap second event occurs
	unsigned int utc_diff_before;	// 26 I2 - utcLS - UTC - time difference due to leap seconds before event
	unsigned int utc_leap_week;		// 28 I2 - utcWNF - UTC - week number when next leap second event occurs
	unsigned int utc_day_of_week;	// 30 I2 - utcDN - UTC - day of week when next leap second event occurs
	unsigned int utc_diff_after;	// 32 I2 - utcLSF - UTC - time difference due to leap seconds after event
	unsigned char utc_dow;			// Calculated day of week.
	unsigned char utc_leap_second;	// Calculated Leap Second Addition or Removal.
};

extern bit msg_update_flag;

extern signed long xdata quant_error;
extern unsigned char xdata UTC_time_valid;
extern struct time xdata UTC_time_buffer;
extern struct leap_info xdata UTC_leap_buffer;
extern struct time xdata UTC_Boot_Time;
extern struct time xdata UTC_ServerStart_Time;

extern char signal_quality[16];
extern unsigned char signal_strength[16];

extern unsigned char ublox_timeout; // Workaround 20120224 for system lockup.

void packet_process();
void interpret_message();
void send_packet(PACKET *out_packet);
void get_packet(void *in_packet);
char setup_ublox();
void set_message_rate(char class_num, char id_num, char rate);
void get_UTC_time(struct time *out_time);