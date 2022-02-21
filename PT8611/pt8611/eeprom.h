

#define CONROL_BYTE (UC) 0xA0  /* device address  = A0 */


UC ByteWriteE2prom(UC, UC, UC *);
UC PageWriteE2prom(UC, UC, UC idata *, UC);
UC Ack_pol_r(UC);
UC Ack_pol_w(UC);
UC RandomReadE2prom(UC, UC, UC *);
UC CurrentReadE2prom(UC, UC *);
UC SeqReadE2prom(UC, UC, UC idata *, UC);

UC   I2cInit(void);
void I2cStart(void);
UC   I2cMasterWrite(UC);
UC   I2cMasterRead(UC);
UC   I2cStop(void);
