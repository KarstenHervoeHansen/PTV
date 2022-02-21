//#define DEBUG

extern unsigned char FPGA_system_control;
extern unsigned char master_status_byte;
extern unsigned char xdata KU_str[11];
extern unsigned char code current_MCU_sw_version;

extern int xdata OCXO_phase_diff;
//extern unsigned char xdata debug_buffer[64];

extern unsigned char signal_quality[16];
extern unsigned char xdata PT5300_str[11];
extern unsigned char xdata phase_str[11];
extern unsigned char xdata counter_str[11];
extern unsigned char xdata quality_str[11];  
extern unsigned char xdata debug_str[11];

extern unsigned long xdata int_GPS_tow;
extern unsigned int xdata int_GPS_week;
extern unsigned int xdata GPS_week;

void set_bit(unsigned char *byte, unsigned char bit_number);
void clear_bit(unsigned char *byte, unsigned char bit_number);
//void print_debug_page();