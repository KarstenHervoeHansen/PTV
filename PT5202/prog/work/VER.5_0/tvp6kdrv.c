/****************************************************************************/
/* MODULE:                                                                  */
/*  tvp6kdrv.c - TI TVP600+ Multimedie chip driver                          */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*  int pld_Init(void);                                                     */
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/* NOTES: Coefficients for PAL_Gain_x and NTSC_Gain_x                       */
/*        changed on 29.05.2002 (Gunner)                                    */
/*        Individual IIC bus driver included here only for TVP600 chip      */
/*        made of iic_drv.c.  All IIC functions, except TransmitIIC() and   */
/*        ReceiveIIC(),  are static and for this                            */
/*        module only.  The chip did not work with IIC functions used for   */
/*        keyboard/LCD hardware. 27.01.2003.                                *
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201    
 *   Copyright (c) 
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

#include "define.h"
#include "tables.h"
#include "tvp6kdrv.h"
/*********************/

/* Prototypes of local procedures */
//static void Delay(int usec);
static int SDA_get(void);
static int SCL_get(void);
static int SCL_set(UC level);
static int SCL_set_wait();
static int SDA_set(UC level);
static int Start_IIC(void);
static int Stop_IIC(void);
static UC  RxByte(int Last);
static int TxByte(UC X);
static int InitIICBus(void);


#define Delay(x)

/* Local defines */

#define High 1
#define Low  0
#define Bit7 0x80

#define IICtimeout 10    // Timeout in usec

/* Define direct pins used for local (PTV) IIC */
_sfrbit    SCLpin  _atbit(P1,7);   // P1^7
_sfrbit    SDApin  _atbit(P1,6);   // P1^6


/**************************************************************************/
/* tvp6k_init                                                   TVP6KDRV.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000401                                    */
/* Revised:   000401, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:                                                                 */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int tvp6k_init(void)
  {
  InitIICBus();
  return(tvp6k_system(PAL));
  }

/**************************************************************************/
/* tvp6k_system                                               TVP6KDRV.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000401                                    */
/* Revised:  000705, KEn, DEV                                             */
/*                                                                        */
/* Function: To program the MM chip for given system                      */
/* Remarks:                                                               */
/* Returns:  OK                                                           */
/* Updates:  29.05.2002 (Gunner)  PAL_Gain_x and NTSC_Gain_x changed      */
/**************************************************************************/
int tvp6k_system(int system)
{

enum 
  {  
  PAL_Gain_U = 289,      /* (default datasheep 0x115 (227 dec)) */
  PAL_Gain_V = 407,      /* (default datasheep 0x18C (396 dec)) */
  PAL_Black_level = 51,  /* (default datasheep 0x3C (60 dec)) */
  PAL_Blank_level = 62,  /* (default datasheep 0x3C (60 dec)) */
  PAL_Gain_Y =     319,  /* (default datasheep 0x145 (325 dec)) */
  PAL_COLOR_BURST_AMP = 62, /* (default datasheep 0x41 (65 dec) */

  NTSC_Gain_U = 279,     /* (default datasheep 0x101 (257 dec)) */
  NTSC_Gain_V = 394,      /* (default datasheep 0x16B (363 dec)) */
  NTSC_Black_level = 65,  /* (default datasheep 0x4C (76 dec)) */
  NTSC_Blank_level = 56,  /* (default datasheep 0x38 (56 dec)) */
  NTSC_Gain_Y =     304,  /* (default datasheep 0x1E2 (482 dec)) */
  NTSC_COLOR_BURST_AMP = 60, /* (default datasheep 0x38 (56 dec) */

  JNTSC_Gain_U = 300,     /* (default datasheep 0x101 (257 dec)) */
  JNTSC_Gain_V = 426,      /* (default datasheep 0x16B (363 dec)) */
  JNTSC_Black_level = 45,  /* (default datasheep 0x4C (76 dec)) */
  JNTSC_Blank_level = 56,  /* (default datasheep 0x38 (56 dec)) */
  JNTSC_Gain_Y =     326,  /* (default datasheep 0x1E2 (482 dec)) */
  JNTSC_COLOR_BURST_AMP = 60  /* (default datasheep 0x38 (56 dec) */
  };

  UC buffer[15];

  if (system < NTSC_US)
    {
    /*------------------------------------------------------
    |    P A L   S e t u p
    -----+----+----------------------------------------------
    | 3A | 8F | CBAR=1, FTM=0x03, Y2C=1, UV2C=1, (SCN=0)
    |----+----+----------------------------------------------
    | 5A | 00 | CPHS=0x00                 (User adj.)
    |----+----+----------------------------------------------
    | 5B | 30 | GU=0x130      (default datasheep 0x115)
    |    |    |   Gain_U
    |----+----+----------------------------------------------
    | 5C | 96 | GV=0x196      (default datasheep 0x18C)
    |    |    |   Gain_V
    |----+----+----------------------------------------------
    | 5D | B3 | BLACK=0x33
    |    |    |   Black_Level
    |----+----+----------------------------------------------
    | 5E | BE | BLANK=0x3E
    |    |    |   Blank_Level
    |----+----+----------------------------------------------
    | 5F | 3F | GY=0x13F
    |    |    |   Gain_Y
    |----+----+----------------------------------------------
    | 60 | 20 | XC=0, LCD=0x0
    |----+----+----------------------------------------------
    | 61 | 42 | SDOWN=0, CDOWN=1, PALPHS=0, GLCE=0, CBW=0, PAL=1, FREQ=0
    |----+----+----------------------------------------------
    | 62 | 3E | SQP=0, BSTAP=0x3E
    |    |    |   Color_burst_amplitude
    |----+----+----------------------------------------------
    | 63 | CB | FSC=0x2A098ACB
    |----+----+----------------------------------------------
    | 64 | 8A |
    |----+----+----------------------------------------------
    | 65 | 09 |
    |----+----+----------------------------------------------
    | 66 | 2A |
    |----+----+----------------------------------------------
    | 67 | 00 | L21O=0x0000
    |----+----+----------------------------------------------
    | 68 | 00 |
    |----+----+----------------------------------------------
    | 69 | 00 | L21E=0x0000
    |----+----+----------------------------------------------
    | 6A | 00 |
    |----+----+----------------------------------------------
    | 6B | 11 | SLINE=0x11
    |----+----+----------------------------------------------
    | 6C | A0 | RCV1A=1, RCV1B=0, HSINSEL=1, ORCV1=0, PRCV1=0,
    |    |    | CBLFV2=0, ORCV2=0, PRCV2=0
    |----+----+----------------------------------------------
    | 6D | 00 | CRCM2=0, CBLFM2=0, ORCM2=0, PRCM2=0, RCM1A=0,
    |    |    | RCM1B=0, L21ENA=0, L21ENB=0
    |----+----+----------------------------------------------
    | 6E | 0A |  HTRIG=0x10A
    |----+----+----------------------------------------------
    | 6F | 01 |
    |----+----+----------------------------------------------
    | 70 | 80 | PRESA=1, PRESB=0, SBLANK=0, VTRIG=0x00
    |----+----+----------------------------------------------
    | 71 | 8C | BMRQ=0x08C
    |----+----+----------------------------------------------
    | 72 | 4C | EMRQ=0x34C
    |----+----+----------------------------------------------
    | 73 | 30 |
    |----+----+----------------------------------------------
    | 77 | 8C | BRCV=0x08C
    |----+----+----------------------------------------------
    | 78 | 4C | ERCV=0x34C
    |----+----+----------------------------------------------
    | 79 | 30 |
    |----+----+----------------------------------------------
    | 7A | 70 | FLEN=0x270
    |----+----+----------------------------------------------
    | 7B | 16 | FAL=0x016
    |----+----+----------------------------------------------
    | 7C | 35 | LAL=0x135
    |----+----+----------------------------------------------
    | 7D | 22 |
    |----+----+----------------------------------------------
    | 7E | 0A | ESAV=0, IGNP=0, FREE=0, BLINKS=1, AVM=0x01, FID=0
    |----+----+----------------------------------------------
    | 90 | 00 | SWPF=0, HSC=0x0, VSC=0x0, SCN=0
    |----+----+----------------------------------------------
    | 91 | 00 | SLPF=0x000
    |----+----+----------------------------------------------
    | 92 | 00 | SPPL=0x000
    |----+----+----------------------------------------------
    | 93 | 00 |
    |----+----+----------------------------------------------
    | 94 | 00 | DLPF=0x000
    |----+----+----------------------------------------------
    | 95 | 00 | DPPL=0x000
    |----+----+----------------------------------------------
    | 96 | 00 |
    |----+----+----------------------------------------------
    | 97 | 00 | VDTA=0x0000
    |----+----+----------------------------------------------
    | 98 | 00 |
    |----+----+----------------------------------------------
    | 99 | 00 | HDTA=0x0000
    |----+----+----------------------------------------------
    | 9A | 00 |
    |----+----+----------------------------------------------
    | 9B | 00 | VOFS=0x00
    |----+----+----------------------------------------------
    | 9C | 00 | HOFS=0x00
    |----+----+----------------------------------------------
    | 9D | 00 | NLR=0x00
    |--------------------------------------------------------
    */

    // PAL setup
    buffer[0] = 0x3A;
    buffer[1] = 0x8F;
    TransmitIIC(TVP6K_IIC_address, 2, buffer);

    buffer[0] = 0x5A;
    buffer[1] = 0x00;               // [5A]
    buffer[2] = 0xFF & PAL_Gain_U;  // [5B]      //was 0x10, 0x32
    buffer[3] = 0xFF & PAL_Gain_V;  // [5C]     //was 0x80, 0xAC
    buffer[4] = PAL_Black_level | ((PAL_Gain_U & 0x100) >> 1);   // [5D]
    buffer[5] = PAL_Blank_level | ((PAL_Gain_V & 0x100) >> 1);   // [5E]
    buffer[6] = 0xFF & PAL_Gain_Y;  // [5F]
    buffer[7] = 0x00 | ((PAL_Gain_Y & 0x100) >> 3);  // [60]
    buffer[8] = 0x42;         // [61]
    buffer[9] = PAL_COLOR_BURST_AMP;  // [62]               // was 0x41
    buffer[10] = 0xCB;        // [62]
    buffer[11] = 0x8A;        // [63]
    buffer[12] = 0x09;
    buffer[13] = 0x2A;
    TransmitIIC(TVP6K_IIC_address, 14, buffer);

    buffer[0] = 0x67;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x11;
    buffer[6] = 0xA0;
    buffer[7] = 0x00;
    buffer[8] = 0x0A;
    buffer[9] = 0x01;
    buffer[10] = 0x80;
    buffer[11] = 0x8C;
    buffer[12] = 0x4C;
    buffer[13] = 0x30;

    TransmitIIC(TVP6K_IIC_address, 14, buffer);

    buffer[0] = 0x77;
     buffer[1] = 0x8C;
    buffer[2] = 0x4C;
    buffer[3] = 0x30;
    buffer[4] = 0x70;
    buffer[5] = 0x16;
    buffer[6] = 0x35;
    buffer[7] = 0x22;
    buffer[8] = 0x0A;
    TransmitIIC(TVP6K_IIC_address, 9, buffer);

    buffer[0] = 0x90;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;

    TransmitIIC(TVP6K_IIC_address, 15, buffer);

    buffer[0] = 0x3A;
    buffer[1] = 0x0F;
    TransmitIIC(TVP6K_IIC_address, 2, buffer);
  }
  else
    {
    if (system < NTSC_J)
      {
      /*------------------------------------------------------
      |    N T S C   S e t u p
      -----+----+----------------------------------------------
      | 3A | 8F | CBAR=1, FTM=0x03, Y2C=1, UV2C=1, (SCN=0)
      |----+----+----------------------------------------------
      | 5A | 00 | CPHS=0x00                 (User adj.)
      |----+----+----------------------------------------------
      | 5B | 12 | GU=0x112      (default datasheep 0x101)
      |    |    |   Gain_U
      |----+----+----------------------------------------------
      | 5C | 85 | GV=0x185      (default datasheep 0x)
      |    |    |   Gain_V
      |----+----+----------------------------------------------
      | 5D | c1 | BLACK=0x41
      |    |    |   Black_Level
      |----+----+----------------------------------------------
      | 5E | B8 | BLANK=0x38
      |    |    |   Blank_Level
      |----+----+----------------------------------------------
      | 5F | 30 | GY=0x130
      |    |    |   Gain_Y
      |----+----+----------------------------------------------
      | 60 | 20 | XC=0, LCD=0x0
      |----+----+----------------------------------------------
      | 61 | 41 | SDOWN=0, CDOWN=1, PALPHS=0, GLCE=0, CBW=0, PAL=0, FREQ=1
      |----+----+----------------------------------------------
      | 62 | 3C | SQP=0, BSTAP=0x3C     // was 0x38
      |    |    |   Color_burst_amplitude
      |----+----+----------------------------------------------
      | 63 | CB | FSC=0x21F07C1F
      |----+----+----------------------------------------------
      | 64 | 8A |
      |----+----+----------------------------------------------
      | 65 | 09 |
      |----+----+----------------------------------------------
      | 66 | 2A |
      |----+----+----------------------------------------------
      | 67 | 00 | L21O=0x0000
      |----+----+----------------------------------------------
      | 68 | 00 |
      |----+----+----------------------------------------------
      | 69 | 00 | L21E=0x0000
      |----+----+----------------------------------------------
      | 6A | 00 |
      |----+----+----------------------------------------------
      | 6B | 11 | SLINE=0x11
      |----+----+----------------------------------------------
      | 6C | A0 | RCV1A=1, RCV1B=0, HSINSEL=1, ORCV1=0, PRCV1=0,
      |    |    | CBLFV2=0, ORCV2=0, PRCV2=0
      |----+----+----------------------------------------------
      | 6D | 00 | CRCM2=0, CBLFM2=0, ORCM2=0, PRCM2=0, RCM1A=0,
      |    |    | RCM1B=0, L21ENA=0, L21ENB=0
      |----+----+----------------------------------------------
      | 6E | 0A |  HTRIG=0x10A
      |----+----+----------------------------------------------
      | 6F | 01 |
      |----+----+----------------------------------------------
      | 70 | C0 | PRESA=1, PRESB=1, SBLANK=0, VTRIG=0x00
      |----+----+----------------------------------------------
      | 71 | 82 | BMRQ=0x082
      |----+----+----------------------------------------------
      | 72 | 46 | EMRQ=0x346
      |----+----+----------------------------------------------
      | 73 | 30 |
      |----+----+----------------------------------------------
      | 77 | 82 | BRCV=0x082
      |----+----+----------------------------------------------
      | 78 | 46 | ERCV=0x346
      |----+----+----------------------------------------------
      | 79 | 30 |
      |----+----+----------------------------------------------
      | 7A | 0C | FLEN=0x20C
      |----+----+----------------------------------------------
      | 7B | 11 | FAL=0x011
      |----+----+----------------------------------------------
      | 7C | 03 | LAL=0x103
      |----+----+----------------------------------------------
      | 7D | 22 |
      |----+----+----------------------------------------------
      | 7E | 0A | ESAV=0, IGNP=0, FREE=0, BLINKS=1, AVM=0x01, FID=0
      |----+----+----------------------------------------------
      | 90 | 00 | SWPF=0, HSC=0x0, VSC=0x0, SCN=0
      |----+----+----------------------------------------------
      | 91 | 00 | SLPF=0x000
      |----+----+----------------------------------------------
      | 92 | 00 | SPPL=0x000
      |----+----+----------------------------------------------
      | 93 | 00 |
      |----+----+----------------------------------------------
      | 94 | 00 | DLPF=0x000
      |----+----+----------------------------------------------
      | 95 | 00 | DPPL=0x000
      |----+----+----------------------------------------------
      | 96 | 00 |
      |----+----+----------------------------------------------
      | 97 | 00 | VDTA=0x0000
      |----+----+----------------------------------------------
      | 98 | 00 |
      |----+----+----------------------------------------------
      | 99 | 00 | HDTA=0x0000
      |----+----+----------------------------------------------
      | 9A | 00 |
      |----+----+----------------------------------------------
      | 9B | 00 | VOFS=0x00
      |----+----+----------------------------------------------
      | 9C | 00 | HOFS=0x00
      |----+----+----------------------------------------------
      | 9D | 00 | NLR=0x00
      |--------------------------------------------------------
      */

      // NTSC US

      buffer[0] = 0x3A;
      buffer[1] = 0x8F;
      TransmitIIC(TVP6K_IIC_address, 2, buffer);

      buffer[0] = 0x5A;
      buffer[1] = 0x00;
      buffer[2] = 0xFF & NTSC_Gain_U;    // [5B]        // was 0x01, 0x03
      buffer[3] = 0xFF & NTSC_Gain_V;    // [5C]        // was 0x6B, 0x6E
      buffer[4] = NTSC_Black_level | ((NTSC_Gain_U & 0x100) >> 1);   // [5D]
      buffer[5] = NTSC_Blank_level | ((NTSC_Gain_V & 0x100) >> 1);   // [5E]
      buffer[6] = 0xFF & NTSC_Gain_Y;  // [5F]       // was 0x2E
      buffer[7] = 0x00 | ((NTSC_Gain_Y & 0x100) >> 3);  // [60]
      buffer[8] = 0x41;    // [61]
      buffer[9] = NTSC_COLOR_BURST_AMP;  // [62]               // was 0x38
      buffer[10] = 0x1F;    // [63]
      buffer[11] = 0x7C;    // [64]
      buffer[12] = 0xF0;    // [65]
      buffer[13] = 0x21;    // [66]
      TransmitIIC(TVP6K_IIC_address, 14, buffer);

      buffer[0] = 0x67;
      buffer[1] = 0x00;
      buffer[2] = 0x00;
      buffer[3] = 0x00;
      buffer[4] = 0x00;
      buffer[5] = 0x11;
      buffer[6] = 0xA0;
      buffer[7] = 0x00;
      buffer[8] = 0x0A;
      buffer[9] = 0x01;
      buffer[10] = 0xC0;
      buffer[11] = 0x82;
      buffer[12] = 0x46;
      buffer[13] = 0x30;

      TransmitIIC(TVP6K_IIC_address, 14, buffer);

      buffer[0] = 0x77;
      buffer[1] = 0x82;
      buffer[2] = 0x46;
      buffer[3] = 0x30;
      buffer[4] = 0x0C;
      buffer[5] = 0x11;
      buffer[6] = 0x03;
      buffer[7] = 0x22;
      buffer[8] = 0x0A;
      TransmitIIC(TVP6K_IIC_address, 9, buffer);

      buffer[0] = 0x90;
      buffer[1] = 0x00;
      buffer[2] = 0x00;
      buffer[3] = 0x00;
      buffer[4] = 0x00;
      buffer[5] = 0x00;
      buffer[6] = 0x00;
      buffer[7] = 0x00;
      buffer[8] = 0x00;
      buffer[9] = 0x00;
      buffer[10] = 0x00;
      buffer[11] = 0x00;
      buffer[12] = 0x00;
      buffer[13] = 0x00;
      buffer[14] = 0x00;

      TransmitIIC(TVP6K_IIC_address, 15, buffer);

      buffer[0] = 0x3A;
      buffer[1] = 0x0F;
      TransmitIIC(TVP6K_IIC_address, 2, buffer);
    }
    else
    {
      // NTSC J

    buffer[0] = 0x3A;
    buffer[1] = 0x8F;
    TransmitIIC(TVP6K_IIC_address, 2, buffer);

    buffer[0] = 0x5A;
    buffer[1] = 0x00;
    buffer[2] = 0xFF & JNTSC_Gain_U;    // [5B]        // was 0x01, 0x03
    buffer[3] = 0xFF & JNTSC_Gain_V;    // [5C]        // was 0x6B, 0x6E
    buffer[4] = JNTSC_Black_level | ((JNTSC_Gain_U & 0x100) >> 1);   // [5D]
    buffer[5] = JNTSC_Blank_level | ((JNTSC_Gain_V & 0x100) >> 1);   // [5E]
    buffer[6] = 0xFF & JNTSC_Gain_Y;  // [5F]       // was 0x2E
    buffer[7] = 0x00 | ((JNTSC_Gain_Y & 0x100) >> 3);  // [60]
    buffer[8] = 0x41;    // [61]
    buffer[9] = JNTSC_COLOR_BURST_AMP;  // [62]               // was 0x38
    buffer[10] = 0x1F;    // [63]
    buffer[11] = 0x7C;    // [64]
    buffer[12] = 0xF0;    // [65]
    buffer[13] = 0x21;    // [66]
    TransmitIIC(TVP6K_IIC_address, 14, buffer);

    buffer[0] = 0x67;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x11;
    buffer[6] = 0xA0;
    buffer[7] = 0x00;
    buffer[8] = 0x0A;
    buffer[9] = 0x01;
    buffer[10] = 0xC0;
    buffer[11] = 0x82;
    buffer[12] = 0x46;
    buffer[13] = 0x30;

    TransmitIIC(TVP6K_IIC_address, 14, buffer);

    buffer[0] = 0x77;
    buffer[1] = 0x82;
    buffer[2] = 0x46;
    buffer[3] = 0x30;
    buffer[4] = 0x0C;
    buffer[5] = 0x11;
    buffer[6] = 0x03;
    buffer[7] = 0x22;
    buffer[8] = 0x0A;
    TransmitIIC(TVP6K_IIC_address, 9, buffer);

    buffer[0] = 0x90;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;

    TransmitIIC(TVP6K_IIC_address, 15, buffer);

    buffer[0] = 0x3A;
    buffer[1] = 0x0F;
    TransmitIIC(TVP6K_IIC_address, 2, buffer);
    }
  }
  return(OK);
  }




static int SDA_get(void)
  {
  UC locvar;

  locvar = (SDApin == 0 ? Low : High);    /*pin reading on XA*/
  return(locvar);
  }

static int SCL_get(void)
  {
  UC locvar;

  locvar = (SCLpin == 0 ? Low : High);    /*pin reading on XA*/
  return(locvar);
  }

static int SCL_set(UC level)
  {
  if (level == High)
    SCLpin = 1;
  else
    SCLpin = 0;
  return(OK);  /* No errors possible */
  }

static int SCL_set_wait(void)
  {
  UI i;
  SCL_set(High);      /* Release clockline and */
  i = 0;              /* wait for physical line to go high */

  while ((SCL_get() == Low) && (i++ < IICtimeout))
    ;
  return(OK);
  }

static int SDA_set(UC level)
  {
  if (level == High)
    SDApin = 1;
  else
    SDApin = 0;
  return(OK);  /* No errors possible */
  }


static int Start_IIC(void)
  {
  /* Test for bus ready (both lines high ) */
  if ((SCL_get() == High) && (SDA_get() == High))
    {
    SDA_set(Low);
    Delay(5);
    SCL_set(Low);
    }
  return(OK);
  }

static int Stop_IIC(void)
  {
	Delay(2);                      /* prepare by setting dataline low */
	SDA_set(Low);
	Delay(3);
	
	SCL_set_wait();
	
	Delay(5);                     /* Setup time to stopcondition */
	
	SDA_set(High);
	return(OK);
  }

static int TxByte(UC X)
  {
  int locvar;

  for (locvar = 8;locvar != 0; locvar-- )
    {
		SCL_set(Low);            /* Clock line low */
		Delay(2);                /* Small delay before changing data */
		
		if ((X & Bit7) == 0 )    /* Output databit */
		  SDA_set(Low);
		else
		  SDA_set(High);
		
		X += X;             /* Get next databit in position   */
		                    /* Rotate left by adding to itself*/
		Delay(3);           /* Data setup time to clock high */
		SCL_set_wait();
		Delay(5);           /* 5 usec clock high time */
    }
                        /* 8 bits are now out , get ready for acknowledge */
	SCL_set(Low);         /* Clock line low */
	Delay(2);             /* Small delay before changing data */
	SDA_set(High);        /* Data line high : ready for input */
	Delay(3);             /* Data setup time to clock high */
	
	SCL_set_wait();
	
	if (SDA_get() == Low)
	  locvar = OK;      /* Acknowledge OK */
	else
	  locvar = FAIL;    /* Acknowledge FAIL */
	
	Delay(5);           /* 5 usec clock high time */
	SCL_set(Low);       /* Clock line low */
	return(locvar);
  }

static UC RxByte(int Last)
  {
  int locvar;
  UC  datarx = 0;   /* Initialise datashifter */

  for (locvar = 8;locvar != 0; locvar-- )
    {
		Delay(5);
		
		SCL_set_wait();
		
		datarx += datarx + SDA_get();    /* Shift and input databit  */
		Delay(5);                        /* Clock high >= 5 microseconds */
		
		SCL_set(Low);                    /* Clock line low */
    }
   Delay(2);
   /* Acknowledge bit has to be set low if NOT last byte */
   /* and high if last byte */
   if (Last == TRUE)
     SDA_set(High);
   else
     SDA_set(Low);

   Delay(3);

   SCL_set_wait();

   Delay(5);                /* 5 usec clock high time */
   SCL_set(Low);            /* Clock line low */

   Delay(2);                /* Allways leave dataline high */
   SDA_set(High);

   return(datarx);          /* Return the received byte */
  }

static int InitIICBus(void)
  {
  UC i;

	if (1)
	  {
		/* Release bus by setting SDA high and toggling SCL */
		SDA_set(High);
		Delay(5);
		
		for(i = 9 ; i != 0 ; i--)
		 {
		 SCL_set(Low);
		 Delay(5);
		 SCL_set(High);
		 Delay(5);
		 }
		
		/* Test for actual high level on SCL then SDA */
		if (SCL_get() == Low )
		  {
		  /* SCL Short to gnd */
		  i |= 0x01;
		  }
		if (SDA_get() == Low  )
		  {
		  /* SDA Short to gnd */
		  i |= 0x02;
		  }
		
		/* Set SCL low and test for actual low level on SCL */
		SCL_set(Low);
		if (SCL_get() == High )
		  {
		  /* SCL Short to vcc */
		  i |= 0x04;
		  }
		/* Test for SDA low after setting SCL low (Short-circ. SDA-SCL) */
		if (SDA_get() == Low  )
		  {
		  /* SDA Short to SCL */
		  i |= 0x10;
		  }
		/* Set SDA low and test for actual low level on SDA */
		SDA_set(Low);
		SCL_set(High);
		if (SDA_get() == High )
		  {
		  /* SDA Short to vcc */
		  i |= 0x08;
		  }
		
		/* Release bus by setting SDA high and toggling SCL */
		SDA_set(High);
		Delay(5);
		
		SCL_set(Low);
		Delay(5);
		SCL_set(High);
		Delay(5);
		
		SCL_set(Low);
		Delay(5);
		SCL_set(High);
		Delay(5);
		
		Stop_IIC();
		}
	return(OK);
  }

int TransmitIIC(UC Address ,UC num ,UC *data)
  {
  UC i;
  
  Start_IIC();
  TxByte(Address & ~0x01);
  for (i = 0; i < num; i++)
    {
    TxByte(data[i]);
    }
  Stop_IIC();
  return(OK);
  }


int ReceiveIIC(UC Address ,UC num ,UC *data)
  {
  UC i;
  
  Start_IIC();
  TxByte(Address | 0x01);
  for (i = 0; i < num - 1; i++)
    data[i] = RxByte(FALSE);
  data[i] = RxByte(TRUE);
  Stop_IIC();
  return(OK);
  }

