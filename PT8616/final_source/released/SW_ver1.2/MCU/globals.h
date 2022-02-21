//#define DEBUG

extern unsigned char FPGA_system_control;
extern unsigned char master_status_byte;
extern unsigned char xdata KU_str[11];
extern unsigned char code current_MCU_sw_version;

extern int xdata OCXO_phase_diff;
extern unsigned char xdata debug_buffer[64];

void set_bit(unsigned char *byte, unsigned char bit_number);
void clear_bit(unsigned char *byte, unsigned char bit_number);
void print_debug_page();