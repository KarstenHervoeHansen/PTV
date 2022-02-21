extern unsigned char out_buffer_select;

void timer3_ISR();
void SMBus_ISR();
void setup_I2C();
char i2c_putch();

extern bit i2c_msgupdate;
extern unsigned char xdata in_buffer[16];