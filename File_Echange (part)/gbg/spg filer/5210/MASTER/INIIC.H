
// INIIC
#define UC unsigned char
#define UI unsigned int
extern void Wr_iic2(UC Address,UC Data,UC Data2);
extern void Wr_iic(UC Address, UC Data);
extern UC Rd_iic(UC Address);
extern UI Read_Keyb_IIC(UC Address);
extern bit Write_X_Disp_IIC(UC Address, UC Data, UC Data2);
extern bit Write_Disp_IIC(UC Address, UC Data);
extern UI Read_Disp_IIC(UC Address);
extern bit Write_Keyb_IIC(UC Address,UC Data);
extern UI Init_Disp_IIC();
extern bit Init_Keyb_IIC();

