/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998                 */
/*  Project:    PT5230 Digital Video Generator                             */
/*  Module:    KEYB_DRV.H                                                  */
/*  Author:    Kim Engedahl, DEV                                           */
/*  Org. date:  980203                                                     */
/*  Rev. date:  980616, KEn, DEV                                           */
/*  Status:    Version 1.0                                                 */
/*                                                                         */
/*  Changes:                                                               */
/* 980616: Released as 4008 002 06812                                      */
/* 980514: Released as 4008 002 06811                                      */
/***************************************************************************/

// Below are defins for 5202 buttons  for DK-audio hardware
// with new LCD display.
/* Refer to PANEL.DRW for diagram over buttons hardware */
#define LEFTButton      0xDE    // S1
#define UPButton        0xDD    // S3
#define DOWNButton      0xDB    // S5
#define RIGHTButton     0xD7    // S7


#if 1
#define EXECButton      0xEF // S10    // if we run DK-Audio LCD/Key panel
#else          
#define EXECButton      0xCF // S5 207 // if we run PTV LCD/Key panel modified by JK
#endif

#define GENLOCKButton   0xBF    // S11
#define BBButton        0xFE    // S2
#define SDIButton       0xFD    // S4
#define AUDIOButton     0xFB    // S6
#define PRESETButton    0xF7    // S8
//******************************* ******
#define NOButton         0xFF
#define NOCommand        0xFF

#define MASTERButton      LEFTButton & RIGHTButton & EXECButton
#define FACTORYButton     LEFTButton & RIGHTButton

#define NormalRepeatFreq  15
#define FastRepeatFreq    4
#define KEYB_ADDR         0x4E  // changed from 0x40 to avoid conflict with MMC

bit ButtonReady(UC *);
void keyb_init(void);

