void setup_I2C();

void SMBus_ISR();

void interpret_msg();
void set_out_buffer(char str_num);

extern unsigned char bdata BOOT;				// Used for Interrupt redirection.

extern bit b_write_network_settings;
