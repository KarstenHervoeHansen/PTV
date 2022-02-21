/*******************************************************************************
Filename:    c1191.h
Description: Header file for c1191_08.c. Consult the C file for details

Copyright (c) 1999 STMicroelectronics.
THIS PROGRAM IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,EITHER
EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTY
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE RISK
AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU. SHOULD THE
PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
REPAIR OR CORRECTION.
*******************************************************************************/
/*******************************************************************************
Commands for the various functions
*******************************************************************************/
#define FLASH_READ_MANUFACTURER (-2)
#define FLASH_READ_DEVICE_CODE  (-1)
/*******************************************************************************
Error Conditions and return values.
See end of C file for explanations and help
*******************************************************************************/
#define FLASH_BLOCK_PROTECTED     (0x01)
#define FLASH_BLOCK_UNPROTECTED   (0x00)
#define FLASH_BLOCK_NOT_ERASED    (0xFF)
#define FLASH_SUCCESS             (-1)
#define FLASH_POLL_FAIL           (-2)
#define FLASH_TOO_MANY_BLOCKS     (-3)
#define FLASH_MPU_TOO_SLOW        (-4)
#define FLASH_BLOCK_INVALID       (-5)
#define FLASH_PROGRAM_FAIL        (-6)
#define FLASH_OFFSET_OUT_OF_RANGE (-7)
#define FLASH_WRONG_TYPE          (-8)
#define FLASH_ERASE_FAIL          (-14)
#define FLASH_TOGGLE_FAIL         (-15)
/*******************************************************************************
Function Prototypes
*******************************************************************************/
extern unsigned char FlashRead(unsigned long ulOff);
extern void          FlashReadReset(void);
extern int           FlashAutoSelect(int iFunc);
extern int           FlashBlockErase(unsigned char ucNumBlocks, unsigned char ucBlock[]);
extern int           FlashChipErase(void);
extern int           FlashProgram(unsigned long ulOff, size_t NumBytes, void *Array);
extern char          *FlashErrorStr(int iErrNum);
