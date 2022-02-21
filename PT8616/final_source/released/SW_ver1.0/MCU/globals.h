extern unsigned char FPGA_system_control;
extern unsigned char master_status_byte;
extern unsigned char KU_str[11];
extern unsigned char current_MCU_sw_version;

extern int OCXO_phase_diff;

void set_bit(unsigned char *byte, unsigned char bit_number);
void clear_bit(unsigned char *byte, unsigned char bit_number);