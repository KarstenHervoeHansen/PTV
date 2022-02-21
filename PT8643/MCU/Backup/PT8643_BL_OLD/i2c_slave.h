void setup_I2C();

void SMBus_ISR();

void interpret_msg();
void set_out_buffer(char str_num);

extern unsigned char MAC_addr[6];
extern char netfinder_name[32];
