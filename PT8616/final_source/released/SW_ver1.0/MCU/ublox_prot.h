typedef struct packet_tag{
	unsigned char class_num;
	unsigned char id_num;
	unsigned char l_lo;
	unsigned char l_hi;
	unsigned char payload[30];
} PACKET;

//GPS tids info
extern union {
	unsigned long GPS_tow_long;
	unsigned char GPS_tow_bytes[4];
}  xdata GPS_tow;

extern bit msg_update_flag;

extern signed long xdata quant_error;
extern unsigned char xdata UTC_time_valid;

void packet_process();
void interpret_message();
void send_packet(PACKET *out_packet);
void get_packet(void *in_packet);
void setup_ublox();
void set_message_rate(char class_num, char id_num, char rate);
void get_UTC_time(struct time *out_time);