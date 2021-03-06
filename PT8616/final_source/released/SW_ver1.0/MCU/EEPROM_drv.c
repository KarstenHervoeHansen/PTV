#include "i2c_bus.h"

void EEPROM_write_byte(char bank, char adresse, char byte)
{
	bank=(bank&0x07)<<1;

	i2c_start();
	i2c_sendbyte(0xA0|bank);
	i2c_getack();
	i2c_sendbyte(adresse);
	i2c_getack();
	i2c_sendbyte(byte);
	i2c_getack();
	i2c_stop();
	delay(0x27, 0xFF);
}

char EEPROM_read_byte(char bank, char adresse)
{
	char in_byte=0;
	bank=(bank&0x07)<<1;

	i2c_start();
	i2c_sendbyte(0xA0|bank);
	i2c_getack();
	i2c_sendbyte(adresse);
	i2c_getack();
	i2c_start();
	i2c_sendbyte(0xA1);
	i2c_getack();
	in_byte=i2c_readbyte();
	i2c_stop();
	delay(0x27, 0xFF);

	return in_byte;
}