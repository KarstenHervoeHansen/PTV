//-----------------------------------------------------------------------------
//	Filename	: textstr.c
//	Date		: 2010-05-31
//	Name		: BS
//	Processor	: None
//	Copyright	: DK-Technologies A/S
//-----------------------------------------------------------------------------

char code str_lfcr[] 		= {0x0A, 0x0D, 0x00};
char code str_cr[] 			= {0x0D, 0x00};
char code str_ver[] 		= "PT8643 Bootloader V. ";
char code str_ok[] 			= "OK";
char code str_nok[] 		= "Not OK";
char code str_errhex[] 		= "E500 Can not decode HEX file.";
char code str_errNoErase[] 	= "E501 I/O Flash not erased.";
char code str_errcrc[] 		= "E502 I/O CRC Error.";
char code str_erradr[] 		= "E503 I/O Address Error.";
char code str_asterisk[] 	= "*";

char code cmd_test[] 		= "@ATTEST";
char code cmd_ver[] 		= "@ATVER";
char code cmd_enable[] 		= "@AT+++";
char code cmd_esc[] 		= "@AT---";
char code cmd_erase[] 		= "@ATErase";
char code cmd_prgflash[] 	= "@ATPrg";
char code cmd_runflash[] 	= "@ATRUN";
char code cmd_getstat[] 	= "@ATSTAT";
char code cmd_armflash[] 	= "@ATArmFlash";
char code cmd_restart[] 	= "@ATRestart";

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
