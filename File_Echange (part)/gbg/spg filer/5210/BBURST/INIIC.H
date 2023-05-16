/*
********************************************************************
* Project	: PT5210  Black Burst
* Filename	: INIIC.H
* Version	: 1.1
* Purpose	: Header file for INIIC.C
* Org.date	: 961104
* Author	: PRC
********************************************************************
HISTORY:
970114 RELEASED 1.1
*/

extern UC Rd_iic(UC Address);
extern void Wr_iic(UC Address,UC Data);
extern void Wr_iic2(UC Address,UC Data,UC Data2);
extern void store_phases(UC Address, UC subadr, NUL x, NUI y);
extern void store_string(UC Address, UC subadr, UC s[]);
extern void store_byte(UC Address, UC subadr, UC Data);
extern UL Rd_adr_n(UC Address, UC subadr, UC n);
extern void waitms(UI time);

