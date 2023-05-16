/* EQU.C
date:		960425
version:	0.0  961030
author:	P.Chr.
*/

#define UC unsigned char
#define UI unsigned int
#define UL unsigned long
#define sbit static bit
#define NUC near unsigned char
#define NUI near unsigned int
#define NUL near unsigned long


#define gmod 2500
#define mmod 1050

// V24 ADDRESS (set to 0x00 if not option-BBU, 0xf0 otherwise):
// #define defaddr 0xf0
#define defaddr 0x00

#define hcaldefg	1105919210L
#define hcaldefm	230626860L
#define schcaldefg	1855
#define schcaldefm	1736
#define daclevcal	75
#define dacofscal	109

#define ram_addr	0xA0
#define dac1_addr 	0x58
#define dac2_addr 	0x5A

// DO NOT CHANGE THE FOLLOWING ADDRESSES! (are used with auto-increment)
#define daclev1_addr 	0
#define dacofs1_addr 	1
#define daclev2_addr 	2
#define dacofs2_addr 	3
#define trimphag1_addr 	4
#define trimphag2_addr 	10
#define trimpham1_addr 	16
#define trimpham2_addr 	22

// Text address should lie on '16 cell boundary' (for burst write)
#define text_addr	32

// SUBC PHASE INCREMENTS, G and M:
#define inceven  0x000346dc
#define incunev 0x040346dc
#define incm 0x04000000

#define eeprom_addr 0xA0
