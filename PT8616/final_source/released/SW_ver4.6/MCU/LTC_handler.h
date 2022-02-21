extern struct time xdata LTC_a_time;
extern struct time xdata LTC_b_time;
extern struct time xdata LTC_a_sync_time;
extern struct time xdata LTC_b_sync_time;
extern struct time xdata daylight_on_a_time;
extern struct time xdata daylight_off_a_time;
extern struct time xdata daylight_on_b_time;
extern struct time xdata daylight_off_b_time;
extern unsigned char xdata LTC_a_sync_hour;
extern unsigned char xdata LTC_a_sync_min;
extern unsigned char xdata LTC_b_sync_hour;
extern unsigned char xdata LTC_b_sync_min;
extern bit LTC_a_daylight_flag;
extern bit LTC_b_daylight_flag;
extern unsigned long xdata int_GPS_tow;
extern char xdata LTC_a_watchoffset[3];
extern char xdata LTC_b_watchoffset[3];

extern union { 
	char LTC_offset_bytes[4];
	long LTC_offset_long;
} xdata LTC_a_offset;

extern union { 
	char LTC_offset_bytes[4];
	long LTC_offset_long;
} xdata LTC_b_offset;

extern char xdata LTC_a_str[13];
extern char xdata LTC_b_str[13];
extern unsigned char xdata LTC_setup;
extern unsigned char xdata dropframe_setup;
extern unsigned char xdata daylight_switch_setup;
extern unsigned char xdata LTC_request;
extern unsigned char xdata LTC_sync_request;
extern unsigned char xdata LTC_confirm;
extern unsigned long xdata secs_since_sync_a;
extern unsigned long xdata secs_since_sync_b;
extern bit LTC_a_daylight_flag;
extern bit LTC_b_daylight_flag;

//structur for den interne tid
typedef struct time {
	int year;
	char month;
	char day;
	char hour;
	char min;
	char sec;
};

unsigned char bin_to_BCD(unsigned char binary);
void sec_inc();
void offset_time(struct time in_time, struct time *out_time, char hours, char mins, char secs);
void LTC_manager();
void check_daylight_switch();
void confirm_daylight_switch();
void update_LTC_strings();
void transmit_LTC_data();
void init_LTC();
void print_LTC_info();