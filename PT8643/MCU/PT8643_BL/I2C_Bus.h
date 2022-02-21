void i2c_delay();
void sdelay();
void i2c_start();
void i2c_stop();
void i2c_sendbyte(char byte);
unsigned char i2c_readbyte();
char i2c_getack();
//void i2c_giveack();
//void i2c_notack();
//void i2c_test();
char i2c_writeEEbyte(unsigned int addr, unsigned char c);
char i2c_readEEbyte(unsigned int addr);
char i2c_set_rs232dest(unsigned char dest);


#define RS232_DSUB_MCU  	0x92 //0x12 //0x92 //0x12
#define RS232_MCU_MF		0x28 //0x28
#define RS232_DSUB_MF		0x05
#define RS232_DISCONNECTED	0xC0 //0x00