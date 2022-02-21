 /*
 ; FILE NAME        gains.c
 ; PURPOSE          To perform calibration of 5639 '90 series 5 sensor.
 ;
 ; SPEC             Peter Frederiksen
 ; DESIGN           Janusz Kuzminski
 ; CODING           Janusz Kuzminski
 ; CODE INSP.
 ; TEST             
 ;
 ; LAST UPDT:       01.09.2004 Xcom_TxData
 ;                  15.11.2005 Set for '93 sensor.
 ;
 ;                  Invocation:
 ;                  gains x
 ;                  where x is dummy parameter.
 ;
 ;                  All output of this program goes to stdout and to
 ;                  file log.dat
 ;
 ; NOTES:    Regarding phosphor matrix calculation:
 ;           The calibration program (original crt39.pas) calculates
 ;           phosphor matrix based on measurements taken in MX mode.
 ;           The algorithm used is the same as one used in PM39_20, 
 ;           where measurements are taken in XY mode.  However, scaling
 ;           before actual calculation is different in these two programs.
 Below is the sequence for total calibration of the sensor
 taken from original Pascal program CRT39.pas:
-----------------------------------------------------------
 SupplyCurrentTest;
 AnalogProbeTest;
 RS232Test;             {(in unit_s39.pas)}

 EnterKU_Nummer;        {(in unit_s39.pas)}

 InitPtvEEPROM;         {(in util92.inc) }

 writeCRTNamesToEEPROM; {(in util92.inc) }
 writeIdString;         {(in unit_s39.pas)}

/************************ Some of the variables used **************************************
  94 :  for n := 1 TO Length(PM5639IdStr[5]) DO
         writeEEPROM(n+61,Ord(PM5639IdStr[5,n]));  'PTV    ,400810990100,KU');    {5639/94}

 DefaultGainFactor  : array[1..12] of byte =
                      ($FF,$FF,$00,$40,$00,$40,$00,$40,$00,$40,$00,$40);

 DefaultPreGain     : array[1..3] of real =
                      (1.0,1.0,1.0);
 PreGainMulFactor   = 1024;         Multiplikationsfaktor for
                                    XFact, YFact og ZFact
 PreGainAddr        = 97;           Start-adresse i EýPROM for
                                    for pre gain
/******************************************************************************************

  writeDefaultGainFactor;  { Default Gain Factors       (in unit_s39.pas)}
    {
    writeEEPROM(k*12 + n + 1,DefaultGainFactor[n]), etc, starts at addr = 2;
    {
  WriteDefaultPreGain;     { Default Pre-gain Constants (in util92.inc) }
    {
    WriteEEPROM(PreGainAddr ,Hi(Trunc((DefaultPreGain[1]*PreGainMulFactor), etc;
    }
  LearnOffset;              { (in unit_s39.pas)  Er offsetv‘rdierne OK ?   J/N}
  DarkCurrentTest;          { (in unit_s39.pas) }
  BeregnGainFactor;         { (in unit_s39.pas) }
  
  LearnKMatrix(NotSaveCalData);
  LearnSMPTE_CFosfor;
  
  ReadGainFactor;
  ReadIdString(true);
  ReadPreGainFactor(notSilent);
  ReadNoCRTFosfor(NotSilent); PM5639id_str

  ReadNoEEPROMPages;
  ReadInternID;
  ReadCRTNames(Silent);
  ReadCRTMatrix(Silent);
  ReadKMatrix(NotSilent);
if (OK)
  {
  writeOkToEEPROM;
  ReadWPAddr(NotSilent);
  WPon;                               { write protect = on }
  ReadKalDataFromEEPROM;
  WriteCertifFileToLaser(false);
  }
else
  writenot_OkToEEPROM;
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <dos.h>
#include <conio.h>
#include <ctype.h>
#include <setjmp.h>
#include <math.h>
#include <limits.h>
#include "xcom.h"
#include "iic_drv.h"

#define  UL unsigned long
#define  UI unsigned int
#define  UC unsigned char

#define  VERSION (float) 2.0

#define IIC_ADDRESS      (int) 0x300  /* address of IIC card in PC             */
#define LIGHT_LEVEL_DAC (UC)     144  /* address of PCF8591 DAC  in light box  */
#define LAMP_PORT                 64  /* address of PCF8574 port in light box  */
#define GUN_SWITCH_PORT           66  /* address of PCF8574 port RGB generator */
#define LAMP_ON                    0
#define LAMP_OFF                   1

/* EEPROM addresses for constants, see memmap39.doc */
#define PreGainAddr               97
#define GainAddr                   2
#define IdSringAddr               62
#define OffsetAddr               121
#define KmatAddr                 103

#define PreGainMulFactor (float) 1024.0

#define MAX_DARK_RED     (float) 0.03
#define MAX_DARK_GREEN   (float) 0.03
#define MAX_DARK_BLUE    (float) 0.03

#define CORFACTTOL       (float)     7.0    /* tolerance in % */ 
#define GANom            (float) 16384.0

#define RR    0
#define GG    1
#define BB    2
#define WW    3
#define BLACK 4

#define MX_MODE  1
#define XY_MODE  0

/* structure to hold meas results in XY or MX mode */
typedef struct xx
  {
  float X;
  float Y;
  float Z;
  } XY_MX_MEAS;

typedef struct jj
  {
  UI gain[6];
  }  GAINFACTORS;

typedef struct kk
  {
  GAINFACTORS rgb_gains[3];
  }  RGB_GAINFACTORS;

typedef struct ll
  {
  UI address;
  UC value;
  } A_V_PAIR;


/* structures used to hold data under K-matrix calculation */
/* structure to hold measurements results */
typedef struct yy
  {
  float RonR; /* red channel on red   screen */
  float GonR;
  float BonR;
  float RonG; /* red channel on green screen */
  float GonG;
  float BonG;
  float RonB; /* green channel on blue screen */
  float GonB;
  float BonB;
  float RonW;
  float GonW; /* green channel on white screen */
  float BonW;
  } RGB;


/* structure to hold XYZ data for for phosphor matrix calculation */
typedef struct zz
  {
  float X_of_RED;
  float Y_of_RED;
  float Z_of_RED;

  float X_of_GRE;
  float Y_of_GRE;
  float Z_of_GRE;

  float X_of_BLU;
  float Y_of_BLU;
  float Z_of_BLU;

  float X_of_WHI;
  float Y_of_WHI;
  float Z_of_WHI;
  } LEARN_MEAS;

/* union to combine our data types into one data type */
typedef union ZZ
  {
  RGB        mes_vals;
  XY_MX_MEAS meas[4];
  LEARN_MEAS learn;
  float      entire[12]; 
  } CAL_DATA;



/* cie coordinates for EBU monitor, normally they reside */
/* in file c:\test\ebu_xy.dat. Used for K-matrix calculations.   2005 file */

CAL_DATA ebu_monitor_data =
  {
  0.643,                   /* X_of_RED  */
  0.331,                   /* Y_of_RED  */
  (1.0 - 0.643 - 0.331),   /* Z_of_RED  */
  0.291,                   /* X_of_GRE  */
  0.602,                   /* Y_of_GRE  */
  (1.0 - 0.291 - 0.602),   /* Z_of_GRE  */
  0.150,                   /* X_of_BLU  */
  0.058,                   /* Y_of_BLU  */
  (1.0 - 0.150 - 0.058),   /* Z_of_BLU  */
  0.312,                   /* X_of_WHI */
  0.329,                   /* Y_of_WHI */
  (1.0 - 0.312 - 0.329)    /* Z_of_WHI */
  };

/* Luminance value for EBU monitor, normally it resides  */
/* in file ebu_xy.dat. Used for K-matrix calculations.   */
float ebu_luminance = 81.3;

/* Default gain factors.  Must be written into EEPROM    */
/* prior to gain factors calculations                    */
RGB_GAINFACTORS DefaultGainFactors = /* contains 18 integers */
  {
  0xFFFF, 
  0x4000,
  0x4000,
  0x4000,
  0x4000,
  0x4000,
  0xFFFF, 
  0x4000,
  0x4000,
  0x4000,
  0x4000,
  0x4000,
  0xFFFF, 
  0x4000,
  0x4000,
  0x4000,
  0x4000,
  0x4000
  };
  
/* Default pregain factors.  Must be written into EEPROM */
/* prior to gain factors calculations                    */
/* They are multiplied by PreGainMulFactor = 1024.0      */
float DefaultPreGains[3] =
  {
  1.0,
  1.0,
  1.0
  };

/* PM5639id_str  is a serial number prefix.  Currently only: */
/*  1 - /90: big round sensor                                */
/*  2 - /93: small, cyl inder sensor                         */
/*  3 - /94: LCD sensor                                      */
/* are produced.                                             */
/* It must match IntID[] number for particular sensor.       */
/* See: initial_eeprom_values below.                         */
/* Detais of this are unknown.                               */
char * PM5639id_str[] =
  {
  "PTV    ,400810979300,KU",    /* 5639/90 serie III,IV,V   */
  "PTV    ,400810978930,KU",    /* 5639/92 serie I,II,III   */
  "PTV    ,400810978930,KU",    /* 5639/916                 */
  "PTV    ,400810983120,KU",    /* 5639/93                  */
  "PTV    ,400810990100,KU"     /* 5639/94                  */
  };

/* Below are address/value pairs of default values */
/* written to 5639 '94 EEPROM                      */
A_V_PAIR initial_eeprom_values[] =
  {
    {52,    4},   /* 52: Nr of EEPROM pages      */
    {53,   30},   /* 53: Nr of CRT phosphors     */
    {54,  186},   /* 54: Highiest wp address, lo */
    {55,    0},   /* 55: Highiest wp address, hi */
    {56,  'O'},   /* 56: Cal. status 1           */
    {57,  'K'},   /* 57: Cal. status 2           */
    {58,    1},   /* 58: IntID[0]                */ /* 1, 0, 0, 2:  90/Series 5 */
    {59,    0},   /* 59: IntID[1]                */ /* 1, 0, 0, 57: 94 */
    {60,    0},   /* 60: IntID[2]                */ /* 1, 0, 0, 56: 93 */
    {61,   56},   /* 61: IntID[3], 57 for '94    */
    {95,  255},   /* 95: Output format           */
    {96,    1},   /* 96: Trace flag              */
    {157, 255}    /*157: FACTORY / USER cal st.  */
  };
/*
 DefInternID        : array[1..14,1..4] of byte =    {addr 58 - 61}
                     ((1,0,0,1),            {5639/90 serie III   }
                      (1,0,0,2),            {5639/90 serie IV,V  }
                      (0,0,0,0),            {                    }
                      (0,0,0,0),            {                    }
                      (1,0,0,32),           {5639/92 serie I     }
                      (1,0,0,33),           {5639/92 serie II,III}
                      (0,0,0,0),            {                    }
                      (0,0,0,0),            {                    }
                      (1,0,0,56),           {5639/93 serie II,III}
                      (1,0,0,0),            {                    }
                      (1,0,0,0),            {                    }
                      (1,0,0,0),            {                    }
                      (1,0,0,40),           {5639/916            }
                      (1,0,0,0));           {                    }
*/
char *fast_gain_commands[] =
  {
  "FG0",
  "FG1",
  "FG2",
  "FG3",
  "FG4",
  "FG5"
  };

float KorrNom[5] =
  {
  1.08, 1.00, 1.03, 0.88, 0.96
  };

 /* Minimum values for readings during gain factor calc */
 /* /90 serie 5 + /92/93 serie 3   fra 23.6.1999 */
float MinValues[] =
  {
  200.0, 
  200.0, 
  100.0, 
   20.0, 
    2.0
  };

#if 0
/* Values of LightLevel[] are taken from crt39 program  */
/* and are probably used for new sensors other than '94 */
/* They are sure used for '90 series 5                  */
/* and for                '93 series 3                  */
/* Original: (255,255,200,150,120));   { /90 serie 5 + /92/93 serie 3   fra 23.6.1999}
UC LightLevel[] = /* The values are adjusted for problematic '93 sensor */
  {
  204, 
  204, 
  160, 
  120, 
  96
  };
#else
/* Values below are used for  /90 serie 5 + /92/93 serie 3 */
UC LightLevel[] =
  {
  255,
  255,
  200,
  150,
  120
  };
#endif

/* Values of LightLevel[] are taken from '94 program */
UC LightLevel94[] =
  {
  255,
  200,
  160,
  130,
  95
  };

/* In crt39 program, for all except '94 sensors, LightLevel[] */
/* is defined as follows:                                     */
/* CalLightLevel      : array[1..2] of LLType =  {lysstyrke ved gain-kal}
   ((255,200,160,140,100),    { /90 <=serie 4 + /92/93 <=serie 2 fra 10.3.1995 }
    (255,255,200,150,120));   { /90 serie 5 + /92/93 serie 3   fra 23.6.1999}
*/


UC port_stat_64; /* status of PCF8574 port in light box  */
UC port_stat_66; /* status of PCF8574 port RGB generator */

FILE * log_file; /* global pointer to log file */

/* Default CRT Names */
A_V_PAIR def_phosphor_names[] =
  {
  1016, 'E',     /* CRT name 1 */
  1017, 'B',
  1018, 'U',
  1019, '\0',
  996,  'S',     /* CRT name 2 */
  997,  'M',
  998,  'P',
  999,  'T',
  1000, 'E',
  1001, ' ',
  1002, 'C',
  1003, '\0'
  };

/* test data: */
CAL_DATA inp_dat3 =
  {
  /* Red   */
   782.23,
   447.21,
    31.07,
  /* Green */
   543.51,
  1205.03,
   217.83,
  /* Blue */
   314.89,
   122.62,
  1435.34,
  /* White */
  1640.55,
  1775.79,
  1687.01 
  };
float K3[] =    /* result obtained from crt39 program on test data */
  {
  4296.75,    265.52,    -29.31,
  -449.34,   4756.03,     54.20,
    16.01,    -82.59,   4945.62
  };

float CRT3[] =
  {
   8192,        -3705.03,  -1547.38,
  -4338.89,      8192,       314.00,
    683.00,     -1892.23,   8192,
  };

int test_vals[] =
  {
  32767,32768,32769,
  32770,32771,32772,
  32773,32774,32775,
  32776,32777,32778,
  32779,32780,32781,
  0,   /* zero gives -32768 */
  1,2
  };


void give_info(void);
void do_exit(int code, int comport, char *message);
int  set_light_level(UC);
int  set_lamp(UC on_off);
int  wp_off(int where);
int  wp_on(int where);
int  get_measurement(XY_MX_MEAS *, int);
int  extract_3_values(char *ascii, XY_MX_MEAS *res);
void send_string(char *, int);
int  get_string(char *str, int);
int  learn_offset(int port);
int  show_offset_values(int, int);
int  read_gain_factors(int addr, RGB_GAINFACTORS * ptr, int port);
int  write_gain_factors(int addr, RGB_GAINFACTORS * ptr, int where);
int  write_pregain_factors(int addr, float * ptr, int prt);
int  val2eeprom(A_V_PAIR *ptr, int hmany, int prt);
int  write_id_string(int addr, char *str, int prt);
int  dark_current_test(int);
int  do_gain_factors(RGB_GAINFACTORS * gfp, int cport);
int  get_7_measurements(XY_MX_MEAS *, int, int);
void get_any_key(int, char *);
void calculate_mean(XY_MX_MEAS *, XY_MX_MEAS *, int, int);
int  do_k_matrix(CAL_DATA *, CAL_DATA *, float, float *, int);
int  gaussian_elimination(int, float a[][13]);
int  find_k(int size, float coefs[][13], CAL_DATA *, CAL_DATA *);
int  read_kmatrix(int, float *, int);
int  write_kmatrix(int addr, float *matbuff, int prt);
int  user_or_factory(int *, int);
int  get_monitor_measurements(CAL_DATA *, int, int);
void ebu(UC what);
int  do_phosphors(float *p, CAL_DATA *, int);
int  MatrixInversion(float* Atemp, int n, float* AInverse);
int  calc_phosphor_matrix_roughly(float * crt, CAL_DATA * ptr, int);
int  calc_phosphor_matrix_exactly(float * crt, CAL_DATA * ptr, int);
int  calculate_phosphor_matrix(float * crt, CAL_DATA * ptr, int);
int  write_phosphor_matrix(int nr, float *matbuff, int prt);
int  read_phosphor_matrix(int nr, float *matbuff, int where);
int  print(char *msg, ...);
int  input(CAL_DATA *mes, int mode, int comport);
int  calc_value_my(long);

int main(int argc, char ** argv)
  {
  int             i;
  int             port;
  RGB_GAINFACTORS gains;
  CAL_DATA        readout;
  float           kmat[9];
  long      k;
  int      z;

  port_stat_64 = 0xFF; /* default state of PCF8574 port in light box, all */
                       /* outputs hi                                      */

  port_stat_66 = 0xFF; /* default state of PCF8574 port in RGB gener, all */
                       /* outputs hi                                      */
  if (argc != 2)
    {
    give_info();
    exit (0);
    }

  print("gains: Version %.2f.\n", VERSION);

  if (!init_iic(IIC_ADDRESS))
    {
    print("Cannot initialize IIC bus.\n");
/*     exit (0); */
    }

  /* Open log file */
  log_file = fopen("log.dat", "w");
  if (log_file == NULL)
    print("Cannot open log file.\n");

  port  = 0; /* com1 */

  if (!Xcom_Init(port, 4800L, 8, 0, 2, 2000L))
    do_exit(1, port, "Cannot open comport\n.");

  send_string("MS", port);  /* measurement stop                   */
  send_string("MS", port);  /* measurement stop                   */
  send_string("MX", port);  /* MX mode                            */

  wp_off(port);
  
#if 1
  /* Init PTV EEPROM */
  val2eeprom(initial_eeprom_values, 13, port);
  print("Initial values written to EEPROM:\n");

  /* Write ID string to EEPROM */
  if (!write_id_string(IdSringAddr, PM5639id_str[3], port))
    do_exit(1, port, "Cannot write ID string.\n");
  /* print("ID string written to EEPROM: %s\n", PM5639id_str[3]); */


  /* Write default CRT names to EEPROM */
  val2eeprom(def_phosphor_names, 12, port);
  print("Default CRT names written to EEPROM:\n");

  /* Write default gain factors to EEPROM */
  if (!write_gain_factors(GainAddr, &DefaultGainFactors, port))
    do_exit(1, port, "Cannot write gains.\n");
  print("Default Gain Factors written to EEPROM:\n");

  /* Write default pregain factors to EEPROM */
  if (!write_pregain_factors(PreGainAddr, DefaultPreGains, port))
    do_exit(1, port, "Cannot write pregains.\n");
  print("Default Pregain Factors written to EEPROM:\n");

  /* Do Learn Offset */
  if (!learn_offset(port))
    do_exit(1, port, "Cannot learn offset.\n");

  /* Do dark current test */
  if (!dark_current_test(port))
    do_exit(1, port, "Dark current test failed!.\n");

  if (!do_gain_factors(&gains, port))
    do_exit(1, port, "Cannot calculate gain factors!\n");

  ebu(WW); /* Display white rectangle on EBU monitor */

  if (!do_k_matrix(&readout, &ebu_monitor_data, ebu_luminance, kmat, port))
    do_exit(1, port, "Cannot calculate K-Matrix!\n");

  print("Found K-Matrix:\n");
  for (i = 0; i < 9; i++)
    print("K[%d] = %+9.3f\n", i, kmat[i]); 

  /* check if elements are smaller than 32767 */
  for (i = 0; i < 9; i++)
    {
    if (kmat[i] > (float) 32767)
      {
      print("Element kmat[%d] is too big!\n", i);
      do_exit(2, port, "\n");
      }
    }

  if (!write_kmatrix(KmatAddr, kmat, port))
    {
    do_exit(2, port, "Copy of K-Matrix written incorrectly!\n");
    }

  get_any_key(port, "Calculating EBU CRT matrix in MX mode:");



#if 0 /* Below is some test data */
  readout.learn.X_of_RED = 311.1;
  readout.learn.Y_of_RED = 187.1;
  readout.learn.Z_of_RED =  12.6;
  
  readout.learn.X_of_GRE = 236.6;
  readout.learn.Y_of_GRE = 642.5;
  readout.learn.Z_of_GRE =  90.8;
  
  readout.learn.X_of_BLU = 130.3;
  readout.learn.Y_of_BLU =  64.9;
  readout.learn.Z_of_BLU = 529.2;

  readout.learn.X_of_WHI = 680.1;
  readout.learn.Y_of_WHI = 897.0;
  readout.learn.Z_of_WHI = 636.5;
#endif

  print("Input data:\n");
  print("readout.learn.X_of_RED = %+4.1f\n", readout.learn.X_of_RED);
  print("readout.learn.Y_of_RED = %+4.1f\n", readout.learn.Y_of_RED);
  print("readout.learn.Z_of_RED = %+4.1f\n", readout.learn.Z_of_RED);
  
  print("readout.learn.X_of_GRE = %+4.1f\n", readout.learn.X_of_GRE);
  print("readout.learn.Y_of_GRE = %+4.1f\n", readout.learn.Y_of_GRE);
  print("readout.learn.Z_of_GRE = %+4.1f\n", readout.learn.Z_of_GRE);
  
  print("readout.learn.X_of_BLU = %+4.1f\n", readout.learn.X_of_BLU);
  print("readout.learn.Y_of_BLU = %+4.1f\n", readout.learn.Y_of_BLU);
  print("readout.learn.Z_of_BLU = %+4.1f\n", readout.learn.Z_of_BLU);

  print("readout.learn.X_of_WHI = %+4.1f\n", readout.learn.X_of_WHI);
  print("readout.learn.Y_of_WHI = %+4.1f\n", readout.learn.Y_of_WHI);
  print("readout.learn.Z_of_WHI = %+4.1f\n", readout.learn.Z_of_WHI);


  if (!do_phosphors(kmat, &readout, (int)MX_MODE))
    {
    do_exit(2, port, "Cannot Learn EBU phosphor!\n");
    }

  print("Found CRT-Matrix in MX mode:\n");
  for (i = 0; i < 9; i++)
    print("CRT[%d] = %+9.3f\n", i, kmat[i]);
#endif

#if 1
  if (!write_phosphor_matrix(0, kmat, port))
    do_exit(1, port, "Cannot write phosphor matrix!\n");
  print("EBU CRT matrix saved:\n");
#endif

#if 0
  /* we will get 4 sets of XYZ measurements from monitor with  */
  /* different red gun output                                  */

  send_string("MS", port);  /* measurement stop         */
  send_string("MS", port);  /* measurement stop         */
  send_string("XY", port);  /* XY mode                  */

  if (!input(&readout, (int)XY_MODE, port))
    do_exit(1, port, "Cannot get measurements!\n");
#endif

  do_exit(0, port, "");
  return (1);
  }

/********************************************************************/
/*                     K-MATRIX ROUTINES                            */
/********************************************************************/


/**********************************************************************
 * NAME       int do_k_matrix()
 * PURPOSE    To find k-matrix
 *            
 * ARGUMENTS  CAL_DATA *mes     - pointer to structure holding 
 *                                measurement data
 *            CAL_DATA *rmd     - pointer to structure holding 
 *                                reference monitor data
 *            float     rlum    - luminance of ref. monitor
 *            float    *mat     - pointer to k-matrix
 *            int       comport - comport where sensor is 
 *                                connected
 * RETURNS    0 on errors
 *            1 otherwise
 *
 * EXTERNS    None.
 *
 * TEST:      When using inp_dat3[] instead of data from sensor, the 
 *            resulting k-matrix must be equal to K3[], which was 
 *            obtained by original Pascal program crt39.
 *
 * NOTES:     If successful, mat will contain calculated k-matrix on 
 *            exit.
 *            This function does:
 *            1 - Collects sensor measurement data taken on reference 
 *                monitor on Red, Green, Blue and White screens.
 *
 *            2 - Calculates Pregain Factors and wrtites them into 
 *                sensors EEPROM @ address 97 (3 integers)
 *
 *            3 - scales  X_of_WHI,  Y_of_WHI and  Z_of_WHI of ref.
 *                monitor data using luminance of ref. monitor
 *
 *            4 - solves matrix equations
 *
 *            5 - multiplies k-matrix values by KorrMulFactor = 1E5
 **********************************************************************/
int do_k_matrix(CAL_DATA *mes, CAL_DATA *rmd, float rlum, float *mat, int comport)
  {
  float  a[12][13];    /* a = [A, B] in AX = B */
  float  KorrMulFactor;
  float  XYZfact[3];   /* Pregain Factors */
  int    k;

  print("Calculating K-Matrix. EBU Monitor data:\n");
    for (k = 0; k < 12; k += 3)
    print("%3.3f  %3.3f  %3.3f\n", rmd->entire[k], rmd->entire[k + 1], rmd->entire[k + 2]);
  print("Luminance = %3.1f\n", rlum);

  get_any_key(comport, "K-matrix: Place sensor on EBU monitor");

  send_string("MS", comport);
  send_string("MS", comport);
  send_string("MX", comport); /* unmodified sensor output */

  /***************************************************************************/
  /*                                1                                        */
  /***************************************************************************/
  /* Below we can use either some input data, which will give known results  */
  /* (for testing this algorithm), or use data from sensor placed on monitor */
#if 0
  mes = &inp_dat3;  /* use sample test data */
#else
  /* Get measurement data from monitor into mes */
  if (!get_monitor_measurements(mes, (int)MX_MODE, comport))
    return (0);
#endif

  send_string("MS", comport);

  KorrMulFactor = 1E5;   /* if it is not 94 */
  
  if ((mes->entire[9] == 0.0) || (mes->entire[10] == 0.0) || (mes->entire[11] == 0.0))
    {
    print("Divide by zero error! Wrong measurement data!");
    return (0);
    }

  /***************************************************************************/
  /*                                2                                        */
  /***************************************************************************/
  XYZfact[0] = mes->learn.Y_of_WHI / mes->learn.X_of_WHI;  /* Y65 / X65 */
  XYZfact[1] = mes->learn.Y_of_WHI / mes->learn.Y_of_WHI;  /* Y65 / Y65   = 1 */
  XYZfact[2] = mes->learn.Y_of_WHI / mes->learn.Z_of_WHI;  /* Y65 / Z65 */

#if 1
  if (!write_pregain_factors(PreGainAddr, XYZfact, comport))
    return (0);
#endif
  
  /***************************************************************************/
  /*                                3                                        */
  /***************************************************************************/
  rmd->learn.X_of_WHI = rlum / rmd->learn.Y_of_WHI * rmd->learn.X_of_WHI;
  rmd->learn.Y_of_WHI = rlum; 
  rmd->learn.Z_of_WHI = rlum / rmd->learn.Y_of_WHI * rmd->learn.Z_of_WHI; 

  print("Xp65  = %f, Yp65  = %f, Zp65  = %f\n", rmd->learn.X_of_WHI, 
                                                rmd->learn.Y_of_WHI, 
                                                rmd->learn.Z_of_WHI);


  /***************************************************************************/
  /*                                4                                        */
  /***************************************************************************/
  if (!find_k(12, a, mes, rmd))
    {
    print("Cannot find K_Matrix.\n");
    return (0);
    }
  else    /* on exit from find_k(), a[][size] contains results */
    {
    for (k = 0; k < 9; k++)
      mat[k] = a[k][12];

  /***************************************************************************/
  /*                                5                                        */
  /***************************************************************************/
    for (k = 0; k < 9; k++)
      mat[k] *= KorrMulFactor;
    }
  print("XFact = %lf, YFact = %lf, ZFact = %lf\n", XYZfact[0], XYZfact[1], XYZfact[2]);
  return (1);
  }

int get_monitor_measurements(CAL_DATA *mes, int mode, int comport)
  {
  ebu(RR);
  Xcom_Flush(comport);
  send_string("MC", comport);  /* measure continously      */
  if (!get_7_measurements(&mes->meas[0], mode, comport))
    return (0);
  print("Red:   X=%6.1f  Y=%6.1f  Z=%6.1f\n", mes->meas[0].X, mes->meas[0].Y, mes->meas[0].Z);
  send_string("MS", comport);  /* measure stop      */

  ebu(GG);
  Xcom_Flush(comport);
  send_string("MC", comport);  /* measure continously      */
  if (!get_7_measurements(&mes->meas[1], mode, comport))
    return (0);
  print("Green: X=%6.1f  Y=%6.1f  Z=%6.1f\n", mes->meas[1].X, mes->meas[1].Y, mes->meas[1].Z);
  send_string("MS", comport);  /* measure stop      */

  Xcom_Flush(comport);
  send_string("MC", comport);  /* measure continously      */
  ebu(BB);
  if (!get_7_measurements(&mes->meas[2], mode, comport))
    return (0);
  print("Blue:  X=%6.1f  Y=%6.1f  Z=%6.1f\n", mes->meas[2].X, mes->meas[2].Y, mes->meas[2].Z);
  send_string("MS", comport);  /* measure stop      */

  ebu(WW);
  Xcom_Flush(comport);
  send_string("MC", comport);  /* measure continously      */
  if (!get_7_measurements(&mes->meas[3], mode, comport))
    return (0);
  print("White: X=%6.1f  Y=%6.1f  Z=%6.1f\n", mes->meas[3].X, mes->meas[3].Y, mes->meas[3].Z);
  send_string("MS", comport);  /* measure stop      */

  return (1);
  }

/**********************************************************************
 * NAME       int find_k()
 * PURPOSE    To find k-matrix
 *            
 * ARGUMENTS  int size                 - nr of equations
 *            float coefs[][NMAX + 1]) - array with coeficients and results
 *            CAL_DATA * coef_val  - pointer to CIE values of measurement data
 *            CAL_DATA * constants - pointer to CIE values of reference monitor
 *                                   data
 *
 * RETURNS    0 if equations cannot be solved
 *            1 otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:     on exit, coefs[][size] contains results.
 *            This function receives 12 equations with 12 unknowns
 *            (a, b, c, ...... k, l, m)
 *
 *          |a b c|   |C1|   |kA|
 *          |d e f| * |C2| = |kB|
 *          |g h j|   |C3|   |kC|
 *          
 *          |a b c|   |C4|   |lD|
 *          |d e f| * |C5| = |lE|
 *          |g h j|   |C6|   |lF|
 *      
 *          |a b c|   |C7|   |mG|
 *          |d e f| * |C8| = |mH|
 *          |g h j|   |C9|   |mI|
 *      
 *          |a b c|   |W1|   |M|
 *          |d e f| * |W2| = |N|
 *          |g h j|   |W3|   |O|
 *
 *          It rearanges them into form:
 *
 *          |C1 +C2 +C3 + 0 + 0 + 0 + 0 + 0 + 0 - A + 0 + 0| |a| = 0
 *          | 0 + 0 + 0 +C1 +C2 +C3 + 0 + 0 + 0 - B + 0 + 0| |b| = 0
 *          | 0 + 0 + 0 + 0 + 0 + 0 +C1+ C2 +C3 - C + 0 + 0| |c| = 0
 *          |C4 +C5 +C6 + 0 + 0 + 0 + 0 + 0 + 0 + 0 - D + 0| |d| = 0
 *          | 0 + 0 + 0 +C4 +C5 +C6 + 0 + 0 + 0 + 0 - E + 0| |e| = 0
 *          | 0 + 0 + 0 + 0 + 0 + 0 +C4 +C5 +C6 + 0 - F + 0| |f| = 0
 *          |C7 +C8 +C9 + 0 + 0 + 0 + 0 + 0 + 0 + 0 + 0 - G| |g| = 0
 *          | 0 + 0 + 0 +C7 +C8 +C9 + 0 + 0 + 0 + 0 + 0 - H| |h| = 0
 *          | 0 + 0 + 0 + 0 + 0 + 0 +C7 +C8 +C9 + 0 + 0 - I| |j| = 0
 *          |W1 +W2 +W3 + 0 + 0 + 0 + 0 + 0 + 0 + 0 + 0 + 0| |k| = M
 *          | 0 + 0 + 0 +W1 +W2 +W3 + 0 + 0 + 0 + 0 + 0 + 0| |l| = N
 *          | 0 + 0 + 0 + 0 + 0 + 0 +W1 +W2 +W3 + 0 + 0 + 0| |m| = O
 *
 *          and solves them using gaussian_elimination function.
 *            
 **********************************************************************/
int find_k(int size, float coefs[][13], CAL_DATA * coef_val, CAL_DATA * constants)
  {
  int row;
  int col;
  
  /* zero the coefs array */
  for (row = 0; row < size; row++)
    for (col = 0; col < size; col++)
      coefs[row][col] = 0.0;

  /* zero the constants column in coefs[][] array */
  for (col = 0; col < size; col++)
     coefs[col][size] = 0.0;

  /* fill coefficients: 1-st 3 rows (XonR) */
    row = 0;
    for (col = 0; col < 3; col++)
      coefs[row][col] = coef_val->entire[col];      /* 0, 1, 2 */
    row = 1;
    for (col = 3; col < 6; col++)
      coefs[row][col] = coef_val->entire[col - 3];  /* 0, 1, 2 */
    row = 2;
    for (col = 6; col < 9; col++)
      coefs[row][col] = coef_val->entire[col - 6];  /* 0, 1, 2 */

  /* fill coefficients: 2-nd 3 rows (XonG) */
    row = 3;
    for (col = 0; col < 3; col++)
      coefs[row][col] = coef_val->entire[col + 3];      /* 3, 4, 5 */
    row = 4;
    for (col = 3; col < 6; col++)
      coefs[row][col] = coef_val->entire[col - 3 + 3];  /* 3, 4, 5 */
    row = 5;
    for (col = 6; col < 9; col++)
      coefs[row][col] = coef_val->entire[col - 6 + 3];  /* 3, 4, 5 */

  /* fill coefficients: 3-rd 3 rows (XonB) */
    row = 6;
    for (col = 0; col < 3; col++)
      coefs[row][col] = coef_val->entire[col + 6];      /* 6, 7, 8 */
    row = 7;
    for (col = 3; col < 6; col++)
      coefs[row][col] = coef_val->entire[col - 3 + 6];  /* 6, 7, 8 */
    row = 8;
    for (col = 6; col < 9; col++)
      coefs[row][col] = coef_val->entire[col - 6 + 6];  /* 6, 7, 8 */

  /* fill coefficients: 4-th 3 rows (XonW) */
    row = 9;
    for (col = 0; col < 3; col++)
      coefs[row][col] = coef_val->entire[col + 9];      /* 9, 10, 11 */
    row = 10;
    for (col = 3; col < 6; col++)
      coefs[row][col] = coef_val->entire[col - 3 + 9];  /* 9, 10, 11 */
    row = 11;
    for (col = 6; col < 9; col++)
      coefs[row][col] = coef_val->entire[col - 6 + 9];  /* 9, 10, 11 */

  /* fill coefficients: columns 9 - 11 from constants (with -) (XonR, XonG and XonB) */
    col = 9;
    for (row = 0; row < 3; row++)
      coefs[row][col] = -constants->entire[row];
    col = 10;
    for (row = 3; row < 6; row++)
      coefs[row][col] = -constants->entire[row];
    col = 11;
    for (row = 6; row < 9; row++)
      coefs[row][col] = -constants->entire[row];


  /*load the constans vector (all are zeros except last 3, user's white entry) */
  for (col = 9; col < 12; col++)
    coefs[col][size]  = constants->entire[col];

#if 0
  for (row = 0; row < 12; row++)
    {
    for (col = 0; col < 12; col++)
      print("a[%d][%d] = %lf\n", row, col, coefs[row][col]);
    }
#endif

  /* solve equations */
  if (!gaussian_elimination(size, coefs))
    return 0;
  else
    return 1;
  }


/**********************************************************************
 * NAME       int gaussian_elimination(int n, float a[][NMAX + 1])
 * PURPOSE    To solve a set of n equations with n unknowns
 *            by Gaussian Elimination method.
 *            
 * ARGUMENTS  int n                - nr of equations
 *            float a[][NMAX + 1]) - array with coeficients and results
 *
 * RETURNS    0 if equations cannot be solved
 *            1 otherwise, with results residing in last column at a[][n]
 *
 * EXTERNS    None.
 *
 * NOTES:     This function is taken from
 *              "Introductory Numerical Analysis"
 *               by Mircea Andrecut
 *            which also contains description of the method.
 *            
 **********************************************************************/
int gaussian_elimination(int n, float a[][13])
  {
  int    i, k, p, q, t;
  int    row[13];           /* vector with row numbers */
  float x[13], sum, m;     /* solution vector */

  /* initialize the vector with row numbers */
  for (i = 1; i <= n; i++)
    row[i - 1] = i - 1;

  /* start upper triangularization routine  */
  for (p = 1; p <= n - 1; p++)
    {
    for (k = p + 1; k <= n; k++)
      {
      if (fabs(a[row[k - 1]][p - 1]) > fabs(a[row[p - 1]][p - 1]))
        {
        t          = row[p - 1];
        row[p - 1] = row[k - 1];
        row[k - 1] = t;
        }
      }
    if (a[row[p - 1]][p - 1] == 0.0)
      {
      print("ERROR: the matrix is singular!\n");
      return 0;
      }
    for (k = p + 1; k <= n; k++)
      {
      m = a[row[k - 1]][p - 1] / a[row[p - 1]][p - 1];
      for (q = p + 1; q <= n + 1; q++)
        {
        a[row[k - 1]][q - 1] -= m * a[row[p - 1]][q - 1];
        }
      }
    } 
  /* end of upper triangularization routine  */
  /* start the backward substitution routine */
  if (a[row[n - 1]][n - 1] == 0.0)
    {
    print("ERROR: the matrix is singular!\n");
    return 0;
    }

  x[n - 1] = a[row[n - 1]][n] / a[row[n - 1]][n - 1];

  for (k = n - 1; k >= 1; k--)
    {
    sum = 0;
    for (q = k + 1; q <= n; q++)
      sum += a[row[k - 1]][q - 1] * x[q - 1];
    x[k - 1] = (a[row[k - 1]][n] - sum) / a[row[k - 1]][k - 1];
    }
  /* end the backward substitution routine */
  /* solution vector */
  for (k = 1; k <= n; k++)
    a[k - 1][n] = x[k - 1];
  return 1;
  }


/**********************************************************************
 * NAME       int read_kmatrix(int addr, float *matbuff, int where)
 * PURPOSE    To read kmatrix stored in sensor's EEPROM
 *            
 * ARGUMENTS  int addr        - EEPROM address where matrix is stored
 *            float *matbuff - array to put matrix into
 *            int where       - comport
 *
 * RETURNS    0 if matrix cannot be read,
 *            1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     The correction matrix float[9] is stored as int (2 bytes)
 *            so 18 bytes are read.  The command 'MA' sets memory
 *            address of the first read, address is then automatically
 *            incremented on each subsequent read. 
 *            See: memmap39.doc for details on how data is placed in 
 *            EEPROM.
 **********************************************************************/
int read_kmatrix(int addr, float *matbuff, int where)
  {
  char tmpb[40];
  char buff[40];
  char ch;
  char cl;
  int  c;
  int  i;

  send_string("MS", where);
  strcpy(buff, "MA");
  itoa(addr, tmpb, 10);
  strcat(buff, tmpb);
  send_string(buff, where);

  for (i = 0; i < 9; i++)
    {
    send_string("RM", where);
    if (!get_string(buff, where))
      return (0);
    ch = atoi(buff);  /* get HI value */
    send_string("RM", where);
    if (!get_string(buff, where))
      return (0);
    cl = atoi(buff);  /* get LO value */
    c = 256 * ch + cl;
    matbuff[i] = (float) c;
    }
  return (1);
  }

/**********************************************************************
 * NAME       int write_kmatrix(int addr, float *matbuff, int prt)
 * PURPOSE    To write kmatrix into sensor's EEPROM
 *            
 * ARGUMENTS  int addr        - EEPROM address where matrix is stored
 *            float *matbuff - array holding matrix values
 *            int prt         - comport
 *
 * RETURNS    0 if matrix cannot be written correctly
 *            1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     See: memmap39.doc for details on how data is placed in 
 *            EEPROM.
 *            This function reads the matrix again after write and 
 *            compares results with original values.
 **********************************************************************/
int write_kmatrix(int addr, float *matbuff, int prt)
  {
  float  check[9];
  char   tmpb[40];
  char   buff[40];
  char   ch;
  char   cl;
  unsigned int    c;
  int    i;

  send_string("MS", prt);
  strcpy(buff, "MA");
  itoa(addr, tmpb, 10);
  strcat(buff, tmpb);
  send_string(buff, prt);

  for (i = 0; i < 9; i++)
    {
    strcpy(buff, "SM");
    c  = (unsigned int)matbuff[i];
    ch = c / 256;
    sprintf(tmpb, "%d", ch);
    strcat(buff, tmpb);
    send_string(buff, prt);

    strcpy(buff, "SM");
    c  = (unsigned int)matbuff[i];
    cl = c % 256;
    sprintf(tmpb, "%d", cl);
    strcat(buff, tmpb);
    send_string(buff, prt);
    }
  /* now check if matrix is written correctly */
  if (!read_kmatrix(addr, check, prt))
    do_exit(1, prt, "Cannot read K-matrix!\n");

  for (i = 0; i < 9; i++)
    if ((int) check[i] != (int) matbuff[i])
      {
      return (0);
      }
  return 1;
  }

/********************************************************************/
/*                        OFFSET ROUTINES                           */
/********************************************************************/


/**********************************************************************
 * NAME       int learn_offset(int port)
 * PURPOSE    To perform 'measure DC offset' command (MO)
 *            
 * ARGUMENTS  int where - comport
 *
 * RETURNS    1 on success
 *            0 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     This function performs the MO command.  The sensor, which
 *            must not have any light coming into it, measures DC 
 *            offsets of its R,G and B channels
 *            for each of 6 gains.  These values are transmitted
 *            by the sensor and printed by this function.
 *            The sensor stores these offset values (6 * 3 * 2 bytes)
 *            in EEPROM at address 121 in the manner
 *            described inn MEMMAP39.doc.
 *            This function reads these values, recalculates them to mV
 *            and prints them at the end.
 *            The MO command takes quite long time, hence extra long
 *            timeout value when waiting for answer from sensor.
 *
 * TEST:  Data from MO command output were obtained from CRT39.
 *        mV values obtained from this data were identical to those
 *        obtained from CRT39. (22.11.2005).
 *
        Sample data from sensor: (copied from Pascal program output)
        tmp[0].X = 32769.0;  tmp[0].Y = 32783.0;  tmp[0].Z = 32777.0;
        tmp[1].X =     0.0;  tmp[1].Y = 32782.0;  tmp[1].Z = 32776.0;
        tmp[2].X =     2.0;  tmp[2].Y = 32782.0;  tmp[2].Z = 32775.0;
        tmp[3].X =     9.0;  tmp[3].Y = 32779.0;  tmp[3].Z =     6.0;
        tmp[4].X =    27.0;  tmp[4].Y =     2.0;  tmp[4].Z =    31.0;
        tmp[5].X =    93.0;  tmp[5].Y =    30.0;  tmp[5].Z =   122.0;
        
        Gives this output:
                    R    G    B
        GAIN0:     -1  -15   -9
        GAIN1:   -32768  -14   -8
        GAIN2:      2  -14   -7
        GAIN3:      9  -11    6
        GAIN4:     27    2   31
        GAIN5:     93   30  122
        
        and this voltages: (copied from Pascal program output)
        Gain 0:   -0.05   -0.72   -0.43 mV           1 128      15 128       9 128
        Gain 1:    0.00   -0.67   -0.38 mV           0   0      14 128       8 128
        Gain 2:    0.10   -0.67   -0.34 mV           2   0      14 128       7 128
        Gain 3:    0.43   -0.53    0.29 mV           9   0      11 128       6   0
        Gain 4:    1.30    0.10    1.49 mV          27   0       2   0      31   0
        Gain 5:    4.46    1.44    5.86 mV          93   0      30   0     122   0

 * It is still unclear why these mV values are calculated and displayed.
 **********************************************************************/
int learn_offset(int port)
  {
  XY_MX_MEAS tmp[6];
  char       str[60];
  int        i;
  int        j;
  uchar      c;
  long        X;
  long        Y;
  long        Z;
  int    x;
  int    y;
  int    z;

  print("Learn offset procedure ('MO' command):\n");
  get_any_key(port, "Cut off any light to sensor");
  
  send_string("MS", port);  /* measurement stop                   */
  send_string("MX", port);  /* MX mode: unmodified sensor outoput */
  send_string("MO", port);  /* measure offset                     */

  print(" String received:\n");

#if 1
  for (j = 0; j < 6; j++)
    {
    i = 0;
    do
      {
      if (!Xcom_Getchar_wtimeout(&c, port, 30000L))
        {
        print("learn_offset: Timeout on receive.\n");
        return (0);
        }
      str[i++] = c;
      } while (c != '\r');
    str[i] = '\0';
    print("GAIN%d: %s\n", j, str);
    extract_3_values(str, &tmp[j]);
    }
#else
  /* Test data */
  tmp[0].X = 32769.0;  tmp[0].Y = 32783.0;  tmp[0].Z = 32777.0;
  tmp[1].X =     0.0;  tmp[1].Y = 32782.0;  tmp[1].Z = 32776.0;
  tmp[2].X =     2.0;  tmp[2].Y = 32782.0;  tmp[2].Z = 32775.0;
  tmp[3].X =     9.0;  tmp[3].Y = 32779.0;  tmp[3].Z =     6.0;
  tmp[4].X =    27.0;  tmp[4].Y =     2.0;  tmp[4].Z =    31.0;
  tmp[5].X =    93.0;  tmp[5].Y =    30.0;  tmp[5].Z =   122.0;
#endif

  print("Unmodified A/D  output:\n");
  print("            R    G    B\n");
  for (j = 0; j < 6; j++)
    {
    /* The procedure below is correct: first convert to int and then to long */
    /* Converting float directly to long is not correct.                     */
    x = (int)tmp[j].X;
    y = (int)tmp[j].Y;
    z = (int)tmp[j].Z;

    X = (long) x;
    Y = (long) y;
    Z = (long) z;
  
    print("GAIN%d:   %4d %4d %4d  \n", j, 
           calc_value_my(X), 
           calc_value_my(Y), 
           calc_value_my(Z));
    }

#if 1
  if (!show_offset_values(OffsetAddr, port))
    {
    print("Cannot read offsets\n");
    return (0);
    }
#endif
  return (1);
  }

/**********************************************************************
 * NAME       int show_offset_values(int addr, int where)
 * PURPOSE    To read, recalculate and display offset constants in mV
 *            stored in sensor's EEPROM
 *            
 * ARGUMENTS  int addr        - EEPROM address where constants are
 *                              stored
 *            int where       - comport
 *
 * RETURNS    0 if constants cannot be read,
 *            1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     This function uses algorithm from '94 sensor calibration
 *            program (in Pascal).  It reads offset constants, calculates
 *            the mV values and prints them.
 *            Offset constants are stored @ 121 in EEPROM in the manner
 *            described inn MEMMAP39.doc

      if Os < 0 then
       Os := -1 * (32768 + Os);
      {$R+}
      OffSet[k,n] := Os * 1E3 * ADRef / ADCounts;   {til mV}
                                          {OffSet array bruges ved ST-m†linger}

 **********************************************************************/
int show_offset_values(int addr, int where)
  {
  char tmpb[40];
  char buff[40];
  char ch;
  char cl;
  int  c;
  int  i;
  int  j;
  float  flt;
  float  R[6];
  float  G[6];
  float  B[6];
  float *p[3]; /* array of pointers to float */
  float *sel;
  long   clong;

  send_string("MS", where);
  strcpy(buff, "MA");
  itoa(addr, tmpb, 10);
  strcat(buff, tmpb);
  send_string(buff, where);

  p[0] = R;
  p[1] = G;
  p[2] = B;
  for (j = 0; j < 3; j++)
    {
    sel = p[j];
    for (i = 0; i < 6; i++)
      {
      send_string("RM", where);
      if (!get_string(buff, where))
        return (0);
      cl = atoi(buff);  /* get LO value */
      send_string("RM", where);
      if (!get_string(buff, where))
        return (0);
      ch = atoi(buff);  /* get HI value */
      c = 256 * ch + cl;
#if 0
      clong = (long) c;
      print("Offset[%d] = %4d\n", i, calc_value_my(clong));
#endif
      /* check if the algor below is really OK!  21.11.2005 */
      /* Procedure ShowOffsetValues in unit_s39.pas         */
      /******************************************************/
      if (c < 0)
        c = (-1) * (32768 + c);
      flt = (float)c * 1E3 * 2.4 / 50000.0; 
      sel[i] = flt;
      }
    }

  for (i = 0; i < 6; i++)
    print("Gain%d   R = %+6.3f mV  G = %+6.3f mV  B = %+6.3f mV\n", i, R[i], G[i], B[i]);
  return (1);
  }

int calc_value_my(long what)
  {
  if (what > 0L)
    return ((int)what);
  else
    return ((int)((-1) * (what - (long)32768))); /* 32768 */
  }

/********************************************************************/
/*                     GAIN FACTORS ROUTINES                        */
/********************************************************************/

/**********************************************************************
 * NAME       int do_gain_factors(RGB_GAINFACTORS * gfp, int cport)
 * PURPOSE    To perform calculation of gain factors
 * ARGUMENTS  cport - comport
 *            RGB_GAINFACTORS * gfp - pointer to holder of gain factors
 *                                    [3][6]
 * RETURNS     0 if comm errors or values out of tolerance.
 *             1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     Sensor runs in MX mode, i.e. unmodified sensor output.
 **********************************************************************/
int do_gain_factors(RGB_GAINFACTORS * gfp, int cport)
  {
  float      K[3][5];
  XY_MX_MEAS mean[2][5];
  int        i;
  int        j;

  print("Calculating gain factors.\n");
  get_any_key(cport, "Place sensor on light box");

  /* Write default gain factors to EEPROM */
  if (!write_gain_factors(GainAddr, &DefaultGainFactors, cport))
    {
    print("Error writing default gain factors!\n");
    return (0);
    }
  
  set_lamp(LAMP_ON);
  send_string("MS", cport);
  send_string("MX", cport); /* unmodified sensor output */
  send_string("MC", cport);

  /* Get measurement data */
#if 0
  /* sample input data for do_gain_factors() function */
  mean[0][0].X = 349.363434; mean[1][0].X = 320.714294;
  mean[0][0].Y = 341.820557; mean[1][0].Y = 315.571442;
  mean[0][0].Z = 551.984009; mean[1][0].Z = 511.600006;
  mean[0][1].X = 320.571442; mean[1][1].X = 317.434296;
  mean[0][1].Y = 315.399994; mean[1][1].Y = 312.190857;
  mean[0][1].Z = 511.285706; mean[1][1].Z = 510.161194;
  mean[0][2].X = 151.573715; mean[1][2].X = 147.944000;
  mean[0][2].Y = 142.811432; mean[1][2].Y = 139.070831;
  mean[0][2].Z = 197.442291; mean[1][2].Z = 192.348541;
  mean[0][3].X =  38.987427; mean[1][3].X =  44.113140;
  mean[0][3].Y =  34.308571; mean[1][3].Y =  39.027431;
  mean[0][3].Z =  39.518856; mean[1][3].Z =  44.938286;
  mean[0][4].X =  15.438858; mean[1][4].X =  16.022856;
  mean[0][4].Y =  13.308573; mean[1][4].Y =  13.814857;
  mean[0][4].Z =  14.803429; mean[1][4].Z =  15.394285;
#else

  for (i = 0; i < 5; i++)
    {
    set_light_level(LightLevel[i]);
    send_string(fast_gain_commands[i], cport);     /* FG[i] */
    delay(2000);
    if (!get_7_measurements(&mean[0][i], MX_MODE, cport))
      return (0);
    print("Meas%d @ %s at LLevel = %d: X=%6.1f  Y=%6.1f  Z=%6.1f  Min = %6.1f\n", i, fast_gain_commands[i], LightLevel[i], mean[0][i].X, mean[0][i].Y, mean[0][i].Z, MinValues[i]);

    send_string(fast_gain_commands[i + 1], cport); /* FG[i+1]*/
    delay(2000);
    if (!get_7_measurements(&mean[1][i], MX_MODE, cport))
      return (0);
    print("Meas%d @ %s at LLevel = %d: X=%6.1f  Y=%6.1f  Z=%6.1f  Min = %6.1f\n", i, fast_gain_commands[i + 1], LightLevel[i], mean[1][i].X, mean[1][i].Y, mean[1][i].Z, MinValues[i]);
    }
#endif
  send_string("NR", cport);
  send_string("MS", cport);
  send_string("MX", cport);

  /* check if measurement results are within tolerances */
  /* mean values mean[0][i] must be bigger than those   */
  /* given by MinValues[] array                         */

  if (mean[0][1].X > 1800)
    print("Sesor is Hypersensitive!");

  for (i = 0; i < 5; i++)
    {
    if ((mean[0][i].X < MinValues[i]) || 
        (mean[0][i].Y < MinValues[i]) || 
        (mean[0][i].Z < MinValues[i]))
      {
      print("Result too little at i=%d\n", i);
#if 0
      set_lamp(LAMP_OFF);
      return (0);
#endif
      }
    }

  /* Print data and calculate Correction Coefficients */
  print("        R        G        B      Knom     Rmean    Gmean    Bmean\n");
  for (i = 0; i < 5; i++)
    {
    if ((mean[1][i].X == 0.0) || (mean[1][i].Y == 0.0) || (mean[1][i].Z == 0.0))
      {
      print("Divide by 0 error at i=%d\n", i);
      set_lamp(LAMP_OFF);
      return (0);
      }

    K[0][i] = mean[0][i].X / mean[1][i].X;
    K[1][i] = mean[0][i].Y / mean[1][i].Y;
    K[2][i] = mean[0][i].Z / mean[1][i].Z;
  
    print("K%d:  %+02.3f   %+02.3f   %+02.3f   %+02.2f    %+6.1f   %+6.1f   %+6.1f\n",
            i, K[0][i], K[1][i], K[2][i], KorrNom[i], mean[0][i].X, mean[0][i].Y, mean[0][i].Z);
    }

  set_lamp(LAMP_OFF);

  /* then check if all correction factors are within 7%
     tolerance of nominal correction factors            */
  for (i = 0; i < 3; i++)
    for (j = 0; j < 5; j++)
      if ((KorrNom[j] * 1.07) < K[i][j] || (KorrNom[j] * 0.93) > K[i][j])
        {
        print("Korrection factors out of tolerance at i=%d j=%d\n", i, j);
        return (0);
        }

  /* Calculate gain factors and write them into EEPROM */
  for (i = 0; i < 3; i++)
    {
    gfp->rgb_gains[i].gain[0] = 0xFFFF;
    gfp->rgb_gains[i].gain[1] = (UI)(K[i][0] * GANom);
    gfp->rgb_gains[i].gain[2] = (UI)(K[i][0] * K[i][1] * GANom);
    gfp->rgb_gains[i].gain[3] = (UI)(K[i][0] * K[i][1] * K[i][2] * GANom);
    gfp->rgb_gains[i].gain[4] = (UI)(K[i][0] * K[i][1] * K[i][2] * K[i][3] * GANom);
    gfp->rgb_gains[i].gain[5] = (UI)(K[i][0] * K[i][1] * K[i][2] * K[i][3] * K[i][4] * GANom);
    }

#if 1
  if (!write_gain_factors(GainAddr, gfp, cport))
    {
    print("Error writing gain factors!\n");
    return (0);
    }
#endif
  return (1);
  }


int read_gain_factors(int addr, RGB_GAINFACTORS * ptr, int where)
  {
  char tmpb[40];
  char buff[40];
  UC   ch;
  UC   cl;
  UI   c;
  int  i;
  int  j;

  send_string("MS", where);
  strcpy(buff, "MA");
  itoa(addr, tmpb, 10);
  strcat(buff, tmpb);
  send_string(buff, where);

  for (j = 0; j < 3; j++)
    for (i = 0; i < 6; i++)
      {
      send_string("RM", where);
      if (!get_string(buff, where))
        return (0);
      cl = atoi(buff);  /* get LO value */
      send_string("RM", where);
      if (!get_string(buff, where))
        return (0);
      ch = atoi(buff);  /* get HI value */
      c = 256 * ch + cl;
      ptr->rgb_gains[j].gain[i] = c;
      }
  print("Gain Factors written to EEPROM:\n");
    print("          R       G       B\n");
  for (i = 0; i < 6; i++)
    print("Gain%d = %05u   %05u   %05u\n", i, ptr->rgb_gains[0].gain[i],
            ptr->rgb_gains[1].gain[i], ptr->rgb_gains[2].gain[i]);
  return (1);
  }

/**********************************************************************
 * NAME       int write_gain_factors(int addr, RGB_GAINFACTORS * ptr,
 *                                   int prt)
 * PURPOSE    To write gain factors into EEPROM, starting at addr.
 *            
 * ARGUMENTS  int addr        - EEPROM address where gain factors are
 *                              stored
 *            RGB_GAINFACTORS * ptr - structure containing valuses
 *            int prt         - comport
 *
 * RETURNS    0 if constants cannot be writen,
 *            1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     
 *            gain factor constants are stored @ GainAddr in EEPROM in 
 *            the manner described inn MEMMAP39.doc
 **********************************************************************/
int write_gain_factors(int addr, RGB_GAINFACTORS * ptr, int prt)
  {
  RGB_GAINFACTORS check;
  char   tmpb[40];
  char   buff[40];
  UC     ch;
  UC     cl;
  UI     c;
  int    i;
  int    j;

  send_string("MS", prt);
  strcpy(buff, "MA");
  itoa(addr, tmpb, 10);
  strcat(buff, tmpb);
  send_string(buff, prt);

  for (j = 0; j < 3; j++)
    for (i = 0; i < 6; i++)
      {
      strcpy(buff, "SM");
      c  = (unsigned int)ptr->rgb_gains[j].gain[i];
      cl = c % 256;
      sprintf(tmpb, "%d", cl);
      strcat(buff, tmpb);
      send_string(buff, prt);
  
      strcpy(buff, "SM");
      ch = c / 256;
      sprintf(tmpb, "%d", ch);
      strcat(buff, tmpb);
      send_string(buff, prt);
      }
  /* now check if gains are written correctly */
  if (!read_gain_factors(addr, &check, prt))
    return (0);

  /* by comparing read values to originals */
  for (j = 0; j < 3; j++)
    for (i = 0; i < 6; i++)
      if ((check.rgb_gains[j].gain[i] != ptr->rgb_gains[j].gain[i]))
        {
        return (0);
        }
  return 1;
  }


/**********************************************************************
 * NAME       int write_pregain_factors(int addr, float * ptr,
 *                                      int prt)
 * PURPOSE    To write pregain factors into EEPROM, starting at addr.
 *            
 * ARGUMENTS  int addr        - EEPROM address where pregain factors
 *                              are stored
 *            float     * ptr - structure containing values
 *            int prt         - comport
 *
 * RETURNS    1
 *
 * EXTERNS    None.
 *
 * NOTES:     
 *            pregain factor constants are stored @ PreGainAddr = 97
 *            in EEPROM in the manner described inn MEMMAP39.doc
 *
 *            Pregains *ptr are multiplied by PreGainMulFactor, 
 *            wchich is equal to (float) 1024.0, 
 *            converted to UI and written into EEPROM.
 *
 *            It is not known what are these pregain factors nor why
 *            they are stored in EEPROM.  They are not used in sensor
 *            software.  They are used in Display software, which 
 *            operates in MX mode, to calculate RGB bars.
 **********************************************************************/
int write_pregain_factors(int addr, float ptr[], int prt)
  {
  char   tmpb[40];
  char   buff[40];
  UC     ch;
  UC     cl;
  UI     c;
  int    i;

  send_string("MS", prt);
  strcpy(buff, "MA");
  itoa(addr, tmpb, 10);
  strcat(buff, tmpb);
  send_string(buff, prt);

  for (i = 0; i < 3; i++)
    {
    strcpy(buff, "SM");
    c  = (UI)(ptr[i] * PreGainMulFactor);
    cl = c % 256;
    sprintf(tmpb, "%d", cl);
    strcat(buff, tmpb);
    send_string(buff, prt);

    strcpy(buff, "SM");
    ch = c / 256;
    sprintf(tmpb, "%d", ch);
    strcat(buff, tmpb);
    send_string(buff, prt);
    }
  /* now check if gains are written correctly */
  send_string("MS", prt);
  strcpy(buff, "MA");
  itoa(addr, tmpb, 10);
  strcat(buff, tmpb);
  send_string(buff, prt);

  for (i = 0; i < 3; i++)
    {
    send_string("RM", prt);
    if (!get_string(buff, prt))
      return (0);
    cl = atoi(buff);  /* get LO value */
    send_string("RM", prt);
    if (!get_string(buff, prt))
      return (0);
    ch = atoi(buff);  /* get HI value */
    c = 256 * ch + cl;
#if 0
    print("c = %d, ptr[%d] = %d\n", c, i, (int)(ptr[i] * PreGainMulFactor));
#endif
    if (c != (UI)(ptr[i] * PreGainMulFactor))
      return (0);
    }
  return (1);
  }

/********************************************************************/
/*                     MISCELLANEOUS  ROUTINES                      */
/********************************************************************/


int val2eeprom(A_V_PAIR *ptr, int hmany, int prt)
  {
  char   tmpb[40];
  char   buff[40];
  int    i;
  int    c;

  send_string("MS", prt);

  for (i = 0; i < hmany; i++)
    {
    strcpy(buff, "MA");
    itoa(ptr[i].address, tmpb, 10);
    strcat(buff, tmpb);
    send_string(buff, prt);
    strcpy(buff, "SM");
    c  = (unsigned int)ptr[i].value;
    sprintf(tmpb, "%d", c);
    strcat(buff, tmpb);
    send_string(buff, prt);
    }
  return (1);
  }

int write_id_string(int addr, char *str, int prt)
  {
  char   tmpb[40];
  char   buff[40];
  UC     ch;
  UC     cl;
  UI     c;
  int    i;

  send_string("MS", prt);
  strcpy(buff, "MA");
  itoa(addr, tmpb, 10);
  strcat(buff, tmpb);
  send_string(buff, prt);

  for (i = 0; i < strlen(str); i++)
    {
    sprintf(tmpb, "SM%d", str[i]);
    send_string(tmpb, prt);
    }
#if 1
  /* check if it was written correctly */
  send_string("MS", prt);
  strcpy(buff, "MA");
  itoa(addr, tmpb, 10);
  strcat(buff, tmpb);
  send_string(buff, prt);

  for (i = 0; i < strlen(str); i++)
    {
    send_string("RM", prt);
    if (!get_string(buff, prt))
      return (0);
    c = atoi(buff);
    tmpb[i] = c;
    }
  tmpb[i] = '\0';
  print("Sring sent    : %s\n", str);
  print("Sring received: %s\n", tmpb);
  if (strcmp(str, tmpb))
    return (0);
#endif
  return (1);
  }


/**********************************************************************
 * NAME       int dark_current_test(cport)
 * PURPOSE    To test measurement values taken in the dark
 *            are not bigger than allowed.
 * ARGUMENTS  cport - comport
 * 
 * RETURNS     0 if comm errors or values out of tolerance.
 *             1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     Sensor runs in MX mode, i.e. unmodified sensor output.
 *            No light enters sensor.
 *            
 **********************************************************************/
int dark_current_test(cport)
  {
  XY_MX_MEAS tmp;

  print("Dark current test.\n");
  get_any_key(cport, "Cut off any light to sensor");

  send_string("MS", cport);  /* measurement stop                   */
  send_string("MX", cport);  /* MX mode: unmodified sensor outoput */
  send_string("MC", cport);  /* measure continously                */

  if (!get_measurement(&tmp, cport))
    return (0);

  send_string("MS", cport);  /* measurement stop                   */
  print("dark_current_test(): x = %+02.2f y = %+02.2f z = %+02.2f\n", tmp.X, tmp.Y, tmp.Z);

  /* check if values are inside tolerances */
  if (tmp.X / 25.0 > MAX_DARK_RED)
    return (0);
  if (tmp.X / 25.0 > MAX_DARK_GREEN)
    return (0);
  if (tmp.X / 25.0 > MAX_DARK_BLUE)
    return (0);
  return (1);
  }


int get_7_measurements(XY_MX_MEAS * buff, int mode, int cport)
  {
  XY_MX_MEAS tmp[7];
  int        i;

  printf(".");
  if (!get_measurement(&tmp[0], cport)) /* get one extra */
    return (0);
  printf(".");
  for (i = 0; i < 7; i++)
    {
    if (!get_measurement(&tmp[i], cport))
      return (0);
    printf(".");
    }
  calculate_mean(buff, tmp, mode, 7);
  printf("\r");
  return (1);
  }

/**********************************************************************
 * NAME       int get_measurement(XY_MX_MEAS * ptr, int from)
 * PURPOSE    To fetch measurement result from sensor operating
 *            in XY mode.
 * ARGUMENTS  XY_MX_MEAS * ptr - pointer to structure holding result
 *            int from         - comport
 *
 * RETURNS     0 if a character does not arrive during RXTIMEOUT, or
 *               if too many characters are received.
 *             1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     In XY mode, the sensor outputs XYZ values
 *            as follows:
 *                        nX,nY,nZ<CR>
 *            As this function may be entered in the middle of a 
 *            transmission, it first waits until a CR is received and
 *            then collects the result.
 *            It then converts result from ASCII to int.
 **********************************************************************/
int get_measurement(XY_MX_MEAS * ptr, int from)
  {
  int   i;
  uchar c;
  char  str[80];
  char  buff[80];

#if 0
  send_string("MS", from);  /* measurements stop */
  Xcom_Flush(from);
  send_string("TM", from);  /* take single measurement */
#endif
  /* first wait until present transmission is over */
  do
    {
    if (!Xcom_Getchar(&c, from))
      {
      print("get_measurement(): Timeout on receive.\n");
      return (0);
      }
    } while (c != ('\r'));
  /* then collect sent string */
  i = 0;
  do
    {
    if (!Xcom_Getchar(&c, from))
      {
      print("get_measurement(): Timeout on receive.\n");
      return (0);
      }
    str[i++] = c;
    if (i > 79)    /* if buffer overflow, return and do not cause */
      {            /* eventual exception error                    */
      print("get_measurement(): Too many characters received.\n");
      return (0);
      }
    } while (c != ('\r'));
  str[i] = '\0';
#if 0
  print("get_measurement():                 %s\n", str);
#endif
  /* now extract nX,nY and nZ from the string */
  i = 0;
  if (extract_3_values(str, ptr))
    {
#if 0
    print("get_measurement(): x = %+02.2f y = %+02.2f z = %+02.2f\n", ptr->X, ptr->Y, ptr->Z);
#endif
    return (1);
    }
  else
    return (0);
  }
/**********************************************************************
 * NAME       void calculate_mean(XY_MX_MEAS *dest, XY_MX_MEAS *src,
 *                                int mode, int hmany)
 *
 * PURPOSE    To mean value of measurement results in MX and XY mode.
 *
 * ARGUMENTS  XY_MX_MEAS *dest
 *            XY_MX_MEAS *src
 *            int mode        - indicates mode, i.e. MX or XY.
 *            int hmany       - nr of elements 
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
 * NOTES:     Below is algor from original program
 *
 *  R := R / NoMeas / Abs(FT) * 20; {norm to field time = 20 mS}
 *  G := G / NoMeas / Abs(FT) * 20;
 *  B := B / NoMeas / Abs(FT) * 20;
 *            
 * which is used in MX mode.  It is unclear if the same algor is 
 * used in XY mode.  It is not!!!!!!!!!!!!!!
**********************************************************************/
void calculate_mean(XY_MX_MEAS *dest, XY_MX_MEAS *src, int mode, int hmany)
  {
  XY_MX_MEAS sum;
  int        i;
  float      scanrate;

  if (hmany == 0)
    {
    print("Calculate mean: Div by zero!\n");
    return;
    }
  sum.X = 0.0;
  sum.Y = 0.0;
  sum.Z = 0.0;

  for (i = 0; i < hmany; i++)
    {
    sum.X += src[i].X;
    sum.Y += src[i].Y;
    sum.Z += src[i].Z;
    }
  dest->X = sum.X / (float)hmany;
  dest->Y = sum.Y / (float)hmany;
  dest->Z = sum.Z / (float)hmany;
  
  if (mode == MX_MODE)
    {
    scanrate = 25.0 / 20.0;
  
    dest->X /= scanrate;
    dest->Y /= scanrate;
    dest->Z /= scanrate;
#if 0
    print("Calc_mean: using scanrate.\n");
#endif
    }
  }


/**********************************************************************
 * NAME       int extract_3_values(char *ascii, XY_MX_MEAS *res)
 *
 * PURPOSE    To extract 3 float values from string received from
 *            sensor operating in XY mode.
 *
 * ARGUMENTS  char * ascii     - string containing sensor response
 *            XY_MX_MEAS * res - pointer to structure holding result
 *
 * RETURNS    0 - if data in *ascii is not meaningful
 *            1 - on success
 *
 * EXTERNS    None.
 *
 * NOTES:     In XY mode, the sensor outputs XYZ values
 *            as follows:
 *                        nX,nY,nZ,<CR>, like:
 *                        811.10,1068.66,795.52
 *
 *            In MX mode, the sensor outputs unmodified AD output
 *            (as floats) as follows:
 *                        nX,nY,nZ,INTEGR_TIME<CR>, like:
 *                        811.10,1068.6,795.52,25.0
 *            This function converts 3 coma separated ascii values into
 *            3 float values.
 **********************************************************************/
int extract_3_values(char *ascii, XY_MX_MEAS *res)
  {
  char tmp[80];
  int  i;
  int  j;
  float  values[3];

  for (i = 0; i < 3; i++)
    {
    j = 0;
    while ((*ascii != ',') && (*ascii != '\r'))
      {
      tmp[j++] = *(ascii++);
      if (j == 79)    /* if buffer overflow, return and do not cause */
        {             /* eventual exception error                    */
        print("extract_3_values(): Too many characters in line.\n");
        return (0);
        }
      }
    ascii++;
    tmp[j] = '\0';
    values[i] = atof(tmp);
    }
  res->X = values[0];
  res->Y = values[1];
  res->Z = values[2];
  return (1);
  }

/**********************************************************************
 * NAME       void send_string(char *str, int prt)
 * PURPOSE    To send a string to sensor
 *            
 * ARGUMENTS  char *str - string to send
 *            int prt   - comport
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
 * NOTES:     This function adds terminator to the string, as required
 *            by sensor command protocol.
 **********************************************************************/
void send_string(char *str, int prt)
  {
  while (*str)
    {
    Xcom_Putchar(*(str++), prt);
    }
  Xcom_Putchar(';', prt);
  }

/**********************************************************************
 * NAME       int get_string(char *str, int prt)
 * PURPOSE    To get a string terminated by \r sent by sensor
 *            
 * ARGUMENTS  char *str - string
 *            int prt   - comport
 *
 * RETURNS    0 on communication errors
 *            1 otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:     
 *            
 **********************************************************************/
int get_string(char *str, int prt)
  {
  int    i;
  uchar  c;
  
  i = 0;
  do
    {
    if (!Xcom_Getchar(&c, prt))
      {
      print("get_string: Timeout on receive.\n");
      return (0);
      }
    str[i++] = c;
    } while (c != '\r');
  str[i] = '\0';
  return (1);
  }

/**********************************************************************
 * NAME       int wp_on(int where)
 *
 * PURPOSE    To set sensor write protection on
 *
 * ARGUMENTS  int where - comport
 *
 * RETURNS    0
 *
 * EXTERNS    None.
 *
 * NOTES:     To set sensor write protection off the following command
 *            sequence must be sent to sensor:
 *            "MS;WR79;WR102;WR102;"
 *            
 *            Two bytes in the E2PROM must both contain 0xFF
 *            to allow writing in the write protected memory.
 *            If only ONE BIT is cleared in any of these two 
 *            bytes, the E2PROM is write protected.
 *            Both these bytes are located in the write
 *            protected memory.
 *            The E2PROM conatins 0xFF in these bytes when leaving
 *            the factory, thereby setting the write-protection OFF
 *            during first time calibration.
 *            To initialize write protection, ie. reset the 
 *            two bytes in the E2PROM, use the command:
 *            
 *            WR0;
 *            
 *            To suspend write protection, ie. set the two 
 *            bytes to 0xFF, isssue the following
 *            command sequence exactly as shown below:
 *            
 *            MS;WR79;WR102;WR102;
 **********************************************************************/
int wp_on(int where)
  {
  send_string("MS", where);
  send_string("WR0", where);
  return (0);
  }

/**********************************************************************
 * NAME       int wp_off(int where)
 *
 * PURPOSE    To set sensor write protection off
 *
 * ARGUMENTS  int where - comport
 *
 * RETURNS    0
 *
 * EXTERNS    None.
 *
 * NOTES:     To set sensor write protection off the following command
 *            sequence must be sent to sensor:
 *            "MS;WR79;WR102;WR102;"
 **********************************************************************/
int wp_off(int where)
  {
  char temp[20];
  char buff[20];
#if 0
  send_string("MS", where);
  strcpy(buff, "MA");
  itoa(38, temp, 10);
  strcat(buff, temp);
  send_string(buff, where);

  send_string("SM255", where);
  send_string("SM255", where);
  return (0);
#else

  send_string("MS,WR79,WR102,WR102", where);
  return (0);
#endif
  }

/**********************************************************************
 * NAME       int set_light_level(UC level)
 *
 * PURPOSE    To set the light level in the Light Box
 * ARGUMENTS  UC level - light level
 * RETURNS    0 - if there was timeout on IIC transmission
 *            1 - if OK
 * EXTERNS    None.
 *
 * NOTES:     This function transmits appropriate value via IIC to
 *            the light box's DA converter which controls light level.
 **********************************************************************/
int set_light_level(UC level)
  {
  int stat;

  stat = trmDA8591(IIC_ADDRESS, LIGHT_LEVEL_DAC, level);
  return (stat);
  }

int set_lamp(UC on_off)
  {
  if (on_off)
    port_stat_64 |= 0x01;
  else
    port_stat_64 &= 0xFE;
  
  return (trm8574(IIC_ADDRESS, LAMP_PORT, port_stat_64));
  }

/**********************************************************************
 * NAME       void ebu(UC what)
 *
 * PURPOSE    To control electron guns in the EBU monitor
 * ARGUMENTS  UC what - gun to control
 * RETURNS    Nothing
 * EXTERNS    None.
 *
 * NOTES:     This function transmits appropriate command via IIC to
 *            RGB generator which contains EBU monitor gun control
 *            unit.

 **********************************************************************/
void ebu(UC what)
  {
  switch (what)
    {
    case RR:
      port_stat_66 &= 0xF8;
      port_stat_66 |= 0x01; /* Red gun on */
      break;
    case GG:
      port_stat_66 &= 0xF8;
      port_stat_66 |= 0x02; /* Green gun on */
      break;
    case BB:
      port_stat_66 &= 0xF8;
      port_stat_66 |= 0x04; /* Blue gun on */
      break;
    case WW:
      port_stat_66 |= 0x07; /* All guns on */
      break;
    case BLACK:
      port_stat_66 &= 0xF8; /* All guns off */
      break;
    default:
      port_stat_66 &= 0xF8; /* All guns off */
      break;
    }
  trm8574(IIC_ADDRESS, GUN_SWITCH_PORT, port_stat_66);
  }

void get_any_key(int com, char * str)
  {
  print("%s and press a key...", str);

  while (!kbhit())
    if (CCFlag)
      do_exit(1, com, "Ctrl-Brk pressed.\n");
  getch();
  print("%s\n", "Thank you.");
  }

void do_exit(int code, int comport, char *message)
  {
  wp_on(comport);
  ebu(BLACK);
  set_lamp(LAMP_OFF);
  Xcom_Close(comport);
  print("%s", message);
  fclose(log_file);
  print("Terminated normally.\n");
  exit(code);
  }


void give_info(void)
  {
  print("gains.exe: performs gain calibration of a 5639.\n");
  print("Version %.2f.\n", VERSION);
  print("Invocation: gains x\n");
  print("where x is a dummy parameter.\n");
  }



#if 0
/* sample input data for do_gain_factors() function */
  mean[0][0].X = 349.363434; mean[1][0].X = 320.714294;
  mean[0][0].Y = 341.820557; mean[1][0].Y = 315.571442;
  mean[0][0].Z = 551.984009; mean[1][0].Z = 511.600006;
  mean[0][1].X = 320.571442; mean[1][1].X = 317.434296;
  mean[0][1].Y = 315.399994; mean[1][1].Y = 312.190857;
  mean[0][1].Z = 511.285706; mean[1][1].Z = 510.161194;
  mean[0][2].X = 151.573715; mean[1][2].X = 147.944000;
  mean[0][2].Y = 142.811432; mean[1][2].Y = 139.070831;
  mean[0][2].Z = 197.442291; mean[1][2].Z = 192.348541;
  mean[0][3].X =  38.987427; mean[1][3].X =  44.113140;
  mean[0][3].Y =  34.308571; mean[1][3].Y =  39.027431;
  mean[0][3].Z =  39.518856; mean[1][3].Z =  44.938286;
  mean[0][4].X =  15.438858; mean[1][4].X =  16.022856;
  mean[0][4].Y =  13.308573; mean[1][4].Y =  13.814857;
  mean[0][4].Z =  14.803429; mean[1][4].Z =  15.394285;
  
/* sample input data for pascal version */
        MR[1,1] := 349.363434; MR[2,1] := 320.714294;
        MG[1,1] := 341.820557; MG[2,1] := 315.571442;
        MB[1,1] := 551.984009; MB[2,1] := 511.600006;
        MR[1,2] := 320.571442; MR[2,2] := 317.434296;
        MG[1,2] := 315.399994; MG[2,2] := 312.190857;
        MB[1,2] := 511.285706; MB[2,2] := 510.161194;
        MR[1,3] := 151.573715; MR[2,3] := 147.944000;
        MG[1,3] := 142.811432; MG[2,3] := 139.070831;
        MB[1,3] := 197.442291; MB[2,3] := 192.348541;
        MR[1,4] :=  38.987427; MR[2,4] :=  44.113140;
        MG[1,4] :=  34.308571; MG[2,4] :=  39.027431;
        MB[1,4] :=  39.518856; MB[2,4] :=  44.938286;
        MR[1,5] :=  15.438858; MR[2,5] :=  16.022856;
        MG[1,5] :=  13.308573; MG[2,5] :=  13.814857;
        MB[1,5] :=  14.803429; MB[2,5] :=  15.394285;
#endif
#if 0
  for (i = 0; i < 5; i++)
    {
    print("MR[1,%d] := %lf; MR[2,%d] := %lf;\n", i + 1, mean[0][i].X, i + 1, mean[1][i].X);
    print("MG[1,%d] := %lf; MG[2,%d] := %lf;\n", i + 1, mean[0][i].Y, i + 1, mean[1][i].Y);
    print("MB[1,%d] := %lf; MB[2,%d] := %lf;\n", i + 1, mean[0][i].Z, i + 1, mean[1][i].Z);
    }
#endif


/********************************************************************/
/*                     LEARN PHOSPHOR  ROUTINES                     */
/********************************************************************/


/**********************************************************************
 * NAME       int do_phosphors(float *pho, CAL_DATA *dat, int where)
 * PURPOSE    To calculate new phosphor matrixes.
 *
 * ARGUMENTS  float *pho    - CRT matrix to fill with results
 *            CAL_DATA *dat - pointer to data used in calculations
 *            int mode      - MX or XY mode
 *
 * RETURNS    0 if not successful
 *            1 otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:     
 **********************************************************************/
int do_phosphors(float *pho, CAL_DATA *dat, int mode)
  {
  int i;


  if (!calculate_phosphor_matrix(pho, dat, mode))
    return (0);
  return (1);
  }
/**********************************************************************
 * NAME       int calculate_phosphor_matrix(float * crt, CAL_DATA * ptr)
 * PURPOSE    To calculate phosphor matrix
 *            
 * ARGUMENTS  float *crt      - pointer to result
 *            CAL_DATA * ptr  - pointer to measurements taken on red,
 *                               green and blue rectangles in XY mode.
 *
 * RETURNS    1 - if success
 *            0 - otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:     
 *            This function uses two algorithms, one original, taken
 *            from PM39_20 program or one using matrix inversion 
 *            algorithm. The #if 0 -- #if 1 condition below allows to 
 *            switch between these two algorithms.  Calculations in 
 *            both algorithms are followed by a test run, where we put
 *            input data into equations and check that they are the 
 *            right solution.

  The matrix inversion algorithm works as follows:
  We take (in XY mode) X, Y and Z measurements  on red, green and blue
  color on our CRT.
  We write the following equations, which are (XYZ-->RGB) transformations
  using a crt matrix, giving desired values of RGB.  For red screen, we
  want R to be equal to X_of_RED, G and B to be equal 0, and so on.

  For red screen:

    RR = X_of_RED = CRT[0]*X_of_RED + CRT[1]*Y_of_RED + CRT[2]*Z_of_RED
    GG =    0     = CRT[3]*X_of_RED + CRT[4]*Y_of_RED + CRT[5]*Z_of_RED
    BB =    0     = CRT[6]*X_of_RED + CRT[7]*Y_of_RED + CRT[8]*Z_of_RED

  For green screen:

    RR =    0     = CRT[0]*X_of_GRE + CRT[1]*Y_of_GRE + CRT[2]*Z_of_GRE
    GG = Y_of_GRE = CRT[3]*X_of_GRE + CRT[4]*Y_of_GRE + CRT[5]*Z_of_GRE
    BB =    0     = CRT[6]*X_of_GRE + CRT[7]*Y_of_GRE + CRT[8]*Z_of_GRE

  For blue screen:

    RR =    0     = CRT[0]*X_of_BLU + CRT[1]*Y_of_BLU + CRT[2]*Z_of_BLU
    GG =    0     = CRT[3]*X_of_BLU + CRT[4]*Y_of_BLU + CRT[5]*Z_of_BLU
    BB = Z_of_BLU = CRT[6]*X_of_BLU + CRT[7]*Y_of_BLU + CRT[8]*Z_of_BLU

  Munching on.....

    |X_of_RED|           |X_of_RED Y_of_RED Z_of_RED|
    |   0    | = |CRT| * |X_of_RED Y_of_RED Z_of_RED|
    |   0    |           |X_of_RED Y_of_RED Z_of_RED|

    |   0    |           |X_of_GRE Y_of_GRE Z_of_GRE|
    |Y_of_GRE| = |CRT| * |X_of_GRE Y_of_GRE Z_of_GRE|
    |   0    |           |X_of_GRE Y_of_GRE Z_of_GRE|

    |   0    |           |X_of_BLU Y_of_BLU Z_of_BLU|
    |   0    | = |CRT| * |X_of_BLU Y_of_BLU Z_of_BLU|
    |Z_of_BLU|           |X_of_BLU Y_of_BLU Z_of_BLU|

   After normalizing......

     |1 0 0|   |   |   |B[0] B[3] B[6]|
     |0 1 0| = |CRT| * |B[1] B[4] B[7]|
     |0 0 1|   |   |   |B[2] B[5] B[8]|

        C    =  CRT  *        B
   and as can be seen from the obove, the crt matrix is inverted
   matrix B.

  Note that the matrix obtained by this method has all different elements
  and it takes 18 bytes to store the matrix as integers.

  The original PM39_method basically solves the same set of equations, however, 
  elements B[0], B[4] and B[8] are assumed to be equal to 1 (or some
  scale factor).  Resulting crt matrix is not exact solution, but it
  requires only 12 bytes to store it (the diagonal elements are known)
  and in practice the accuracy obtained is adequate.  Note that input
  data for this method is placed differently inside matrix B, as 
  compared to matrix inversion method.

  Note that in both methods, all elements of crt matrix are multiplied
  by 8192.0, which is a scale factor.

  Below are results of test run on the same input data for both methods.

         Input data:       Exact method:      Approx. method:
      ---------------------------------------------------------
      X_of_RED = 31.06   R = +31.06000000    R = +22.83389648
      Y_of_RED = 17.28   G =  -0.00000000    G =  +0.00190918
      Z_of_RED =  1.84   B =  +0.00000000    B =  -0.00229736
      
      X_of_GRE = 26.96   R =  +0.00000000    R =  +0.00035278
      Y_of_GRE = 55.53   G = +55.53000000    G = +40.67575195
      Z_of_GRE =  9.36   B =  +0.00000000    B =  -0.00194458
      
      X_of_BLU = 13.24   R =  +0.00000000    R =  +0.00167725
      Y_of_BLU =  6.11   G =  -0.00000000    G =  -0.00680054
      Z_of_BLU = 68.51   B = +68.51000000    B = +67.96523560
      
                        CRT-matrix:          CRT-matrix:
                        m[0] = +11143.278    m[0] =  +8192.000
                        m[1] =  -5124.137    m[1] =  -3767.000
                        m[2] =  -1696.519    m[2] =  -1247.000
                        m[3] =  -6234.325    m[3] =  -4566.000
                        m[4] = +11183.826    m[4] =  +8192.000
                        m[5] =   +207.405    m[5] =   +151.000
                        m[6] =   +390.713    m[6] =   +387.000
                        m[7] =  -1581.563    m[7] =  -1569.000
                        m[8] =  +8257.542    m[8] =  +8192.000

  The first method is obviously more accurate, however, in practice
  the second method is probably more that adequate.
 **********************************************************************/
int calculate_phosphor_matrix(float * crt, CAL_DATA * ptr, int mode)
  {
  int i;
  float aa, rr1;
  float R, G, B;

  /* Below is some sample input data used to test both algorithms */

#if 1  /************ with MatrixInversion algorithm */

  if (!calc_phosphor_matrix_exactly(crt, ptr, mode))
    return (0);

#else  /************ using PTV algorithm */

  if (!calc_phosphor_matrix_roughly(crt, ptr, mode))
    {
    print("Cannot find solution.\n");
    return (0);
    }
#endif


  for (i = 0; i < 9; i++)
    crt[i] *= 8192.0;

  return (1);
  }

/**********************************************************************
 * NAME       int calc_phosphor_matrix_exactly(float * crt, CAL_DATA * ptr)
 * PURPOSE    To calculate CRT matrix based on measurement results 
 *            taken in MX mode.
 *            
 * ARGUMENTS  float *crt     - pointer to result
 *            CAL_DATA * ptr - pointer to measurements taken on red,
 *                             green and blue rectangles.
 *            int            - mode
 *
 * RETURNS    1 - if success
 *            0 - otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:     
 *            This function solves the following matrix equation:
 *
 *                  |1 0 0|   |   |   |B[0] B[3] B[6]|
 *                  |0 1 0| = |CRT| * |B[1] B[4] B[7]|
 *                  |0 0 1|   |   |   |B[2] B[5] B[8]|
 *
  Below are data preparation lines taken from crt39.pas, which uses 
  measurements taken in MX mode.  They are different from those in
  PM39_20 algorithm, which uses data taken in XY mode.
 **********************************************************************/
int calc_phosphor_matrix_exactly(float * crt, CAL_DATA * ptr, int mode)
  {
  float B[9];
  float XFact;
  float ZFact;

  XFact = ptr->entire[10] / ptr->entire[9];
  ZFact = ptr->entire[10] / ptr->entire[11];

  if (mode == MX_MODE)
    {
    /* Prepare input data (normalize matrix equations) */
    B[0] = 1;
    B[1] =  ptr->learn.Y_of_RED          / (ptr->learn.X_of_RED * XFact);
    B[2] = (ptr->learn.Z_of_RED * ZFact) / (ptr->learn.X_of_RED * XFact);
    B[3] = (ptr->learn.X_of_GRE * XFact) /  ptr->learn.Y_of_GRE;
    B[4] = 1;
    B[5] = (ptr->learn.Z_of_GRE * ZFact) /  ptr->learn.Y_of_GRE;
    B[6] = (ptr->learn.X_of_BLU * XFact) / (ptr->learn.Z_of_BLU * ZFact);
    B[7] =  ptr->learn.Y_of_BLU          / (ptr->learn.Z_of_BLU * ZFact);
    B[8] = 1;
    }
  else /* XY mode */
    {
    /* Prepare input data (normalize matrix equations) */
    /* red */
    B[0] = ptr->learn.X_of_RED / ptr->learn.X_of_RED;  /* 1 */
    B[1] = ptr->learn.Y_of_RED / ptr->learn.X_of_RED;
    B[2] = ptr->learn.Z_of_RED / ptr->learn.X_of_RED;
    /* green */
    B[3] = ptr->learn.X_of_GRE / ptr->learn.Y_of_GRE;
    B[4] = ptr->learn.Y_of_GRE / ptr->learn.Y_of_GRE;  /* 1 */
    B[5] = ptr->learn.Z_of_GRE / ptr->learn.Y_of_GRE;
    /* blue */
    B[6] = ptr->learn.X_of_BLU / ptr->learn.Z_of_BLU;
    B[7] = ptr->learn.Y_of_BLU / ptr->learn.Z_of_BLU;
    B[8] = ptr->learn.Z_of_BLU / ptr->learn.Z_of_BLU;  /* 1 */
    }

  /* Prepare input data (normalize matrix equations) */
  /* red */
  B[0] = ptr->learn.X_of_RED / ptr->learn.X_of_RED;  /* 1 */
  B[1] = ptr->learn.X_of_GRE / ptr->learn.Y_of_GRE;
  B[2] = ptr->learn.X_of_BLU / ptr->learn.Z_of_BLU;
  /* green */
  B[3] = ptr->learn.Y_of_RED / ptr->learn.X_of_RED;
  B[4] = ptr->learn.Y_of_GRE / ptr->learn.Y_of_GRE;  /* 1 */
  B[5] = ptr->learn.Y_of_BLU / ptr->learn.Z_of_BLU;
  /* blue */
  B[6] = ptr->learn.Z_of_RED / ptr->learn.X_of_RED;
  B[7] = ptr->learn.Z_of_GRE / ptr->learn.Y_of_GRE;
  B[8] = ptr->learn.Z_of_BLU / ptr->learn.Z_of_BLU;  /* 1 */

  if (!MatrixInversion(B, 3, crt))
    {
    print("calc_phosphor_matrix_exactly(): Cannot invert matrix.\n");
    return (0);
    }
  return (1);
  }

/**********************************************************************
 * NAME       int calc_phosphor_matrix_roughly(float * crt, CAL_DATA * ptr)
 * PURPOSE    To calculate CRT matrix using measurements made in MX or
 *            XY mode.
 *            
 * ARGUMENTS  float *crt     - pointer to result
 *            CAL_DATA * ptr - pointer to measurements taken on red,
 *                             green and blue rectangles in XY mode.
 *            int mode       - 5639 measuring mode, MX or XY
 * RETURNS    1
 *
 * EXTERNS    None.
 *
 * NOTES:     This function is based on PM39_20 crt39 programs.
 *            Note that if this function is used in MX mode, the
 *            input data must contain measurements taken on white
 *            screen (elements [9], [10] and [11]) which are used
 *            for scaling.
 *
 **********************************************************************
  Below are data preparation lines taken from crt39.pas, which uses 
  measurements taken in MX mode.  They are different from those in
  PM39_20 algorithm, which uses data taken in XY mode.
 **********************************************************************

   XFact := (Y65 / X65)
   YFact := 1;
   ZFact := (Y65 / Z65)

  XYZfact[0] = mes->entire[10] / mes->entire[9];   Y65 / X65 
  XYZfact[1] = 1.0;
  XYZfact[2] = mes->entire[10] / mes->entire[11];  Y65 / Z65 


 B[0] := 1;
 B[1] := YR / (XR * XFact);
 B[2] := (ZR * ZFact) / (XR * XFact);
 B[3] := (XG * XFact) / YG;
 B[4] := 1;
 B[5] := (ZG * ZFact) / YG;
 B[6] := (XB * XFact) / (ZB * ZFact);
 B[7] := YB / (ZB * ZFact);
 B[8] := 1;

 **********************************************************************/
int calc_phosphor_matrix_roughly(float * crt, CAL_DATA * ptr, int mode)
  {
  int i;
  float aa, rr1;
  float B[9];
  float XFact;
  float ZFact;

  XFact = ptr->entire[10] / ptr->entire[9];
  ZFact = ptr->entire[10] / ptr->entire[11];

  if (mode == MX_MODE)
    {
    /* Prepare input data (normalize matrix equations) */
    B[0] = 1;
    B[1] =  ptr->learn.Y_of_RED          / (ptr->learn.X_of_RED * XFact);
    B[2] = (ptr->learn.Z_of_RED * ZFact) / (ptr->learn.X_of_RED * XFact);
    B[3] = (ptr->learn.X_of_GRE * XFact) /  ptr->learn.Y_of_GRE;
    B[4] = 1;
    B[5] = (ptr->learn.Z_of_GRE * ZFact) /  ptr->learn.Y_of_GRE;
    B[6] = (ptr->learn.X_of_BLU * XFact) / (ptr->learn.Z_of_BLU * ZFact);
    B[7] =  ptr->learn.Y_of_BLU          / (ptr->learn.Z_of_BLU * ZFact);
    B[8] = 1;
    }
  else /* XY mode */
    {
    /* Prepare input data (normalize matrix equations) */
    /* red */
    B[0] = ptr->learn.X_of_RED / ptr->learn.X_of_RED;  /* 1 */
    B[1] = ptr->learn.Y_of_RED / ptr->learn.X_of_RED;
    B[2] = ptr->learn.Z_of_RED / ptr->learn.X_of_RED;
    /* green */
    B[3] = ptr->learn.X_of_GRE / ptr->learn.Y_of_GRE;
    B[4] = ptr->learn.Y_of_GRE / ptr->learn.Y_of_GRE;  /* 1 */
    B[5] = ptr->learn.Z_of_GRE / ptr->learn.Y_of_GRE;
    /* blue */
    B[6] = ptr->learn.X_of_BLU / ptr->learn.Z_of_BLU;
    B[7] = ptr->learn.Y_of_BLU / ptr->learn.Z_of_BLU;
    B[8] = ptr->learn.Z_of_BLU / ptr->learn.Z_of_BLU;  /* 1 */
    }

  /* Calculate for RED phosphor */
  aa = B[6];
  for (i = 0; i < 10; i++)
    {
    rr1 = B[3] - aa  * B[5];
    aa  = B[6] - rr1 * B[7];
    }
  crt[2] = - (aa  * 8192.0);
  crt[1] = - (rr1 * 8192.0);
  /* Calculate for GREEN phosphor */
  aa = B[7];
  for (i = 0; i < 10; i++)
    {
    rr1 = B[1] - aa  * B[2];
    aa  = B[7] - rr1 * B[6];
    }
  crt[5] = - (aa  * 8192.0);
  crt[3] = - (rr1 * 8192.0);
  /* Calculate for BLUE phosphor */
  aa = B[5];
  for (i = 0; i < 10; i++)
    {
    rr1 = B[2] - aa  * B[1];
    aa  = B[5] - rr1 * B[3];
    }
  crt[7] = - (aa  * 8192.0);
  crt[6] = - (rr1 * 8192.0);

  crt[0] = crt[4] = crt[8] = 8192.0; /* int_scale = 8192.0 */

  for (i = 0; i < 9; i++)
    crt[i] /= 8192.0;
  return (1);
  }
/**********************************************************************
 * NAME       int read_phosphor_matrix(int addr, float *matbuff, int where)
 * PURPOSE    To read crt matrix stored in sensor's EEPROM
 *            
 * ARGUMENTS  int nr          - number of CRT matrix
 *            float *matbuff - array to put matrix into
 *            int where       - comport
 *
 * RETURNS    0 if matrix cannot be read,
 *            1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     5639 has place in EEPROM to store 30 phosphor matrixes.
 *            When leaving factory, two of matrixes are written: one
 *            for EBU phosphor (matrix 0) and one for SMPTE phosphor
 *            User can save additional phosphor matrixes using appropriate
 *            software or display unit (learn phosphor procedure).
 *            Phosphor matrix has always its diagonal elements set to
 *            8192, so only 6 out of 9 elements are stored, requiring
 *            12 bytes per matrix, plus 8 bytes for matrix name.
 *            Matrix #0 is stored at address of 1004, all others are 
 *            stored at Addr := 1004 - (nr * 20).
 **********************************************************************/
int read_phosphor_matrix(int nr, float *matbuff, int where)
  {
  char   tmpb[40];
  char   buff[40];
  char   ch;
  char   cl;
  int    c;
  int    i;
  int    address;
  float tmpbuff[6];

  /* calculate address of CRT matrix */

  address = 1004 - (nr * 20);

  send_string("MS", where);
  strcpy(buff, "MA");
  itoa(address, tmpb, 10);
  strcat(buff, tmpb);
  send_string(buff, where);

  for (i = 0; i < 6; i++)
    {
    send_string("RM", where);
    if (!get_string(buff, where))
      return (0);
    ch = atoi(buff);  /* get HI value */
    send_string("RM", where);
    if (!get_string(buff, where))
      return (0);
    cl = atoi(buff);  /* get LO value */
    c = 256 * ch + cl;
    tmpbuff[i] = (float) c;
    }

  /* rearrange the elements */

  matbuff[0] = 8192.0;
  matbuff[1] = tmpbuff[0];
  matbuff[2] = tmpbuff[1];
  matbuff[3] = tmpbuff[2];
  matbuff[4] = 8192.0;
  matbuff[5] = tmpbuff[3];
  matbuff[6] = tmpbuff[4];
  matbuff[7] = tmpbuff[5];
  matbuff[8] = 8192.0;

  return (1);
  }

/**********************************************************************
 * NAME       int write_phosphor_matrix(int nr, float *matbuff, int prt)
 * PURPOSE    To read crt matrix stored in sensor's EEPROM
 *            
 * ARGUMENTS  int nr          - number of CRT matrix
 *            float *matbuff - array containing matrix
 *            int prt         - comport
 *
 * RETURNS    0 on errors,
 *            1 otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     5639 has place in EEPROM to store 30 phosphor matrixes.
 *            When leaving factory, two of matrixes are written: one
 *            for EBU phosphor (matrix 0) and one for SMPTE phosphor
 *            User can save additional phosphor matrixes using appropriate
 *            software or display unit (learn phosphor procedure).
 *            Phosphor matrix has always its diagonal elements set to
 *            8192, so only 6 out of 9 elements are stored, requiring
 *            12 bytes per matrix, plus 8 bytes for matrix name.
 *            Matrix #0 is stored at address of 1004, all others are 
 *            stored at Addr := 1004 - (nr * 20).
 **********************************************************************/
int write_phosphor_matrix(int nr, float *matbuff, int prt)
  {
  float  check[9];
  float  tmp[9];
  char   tmpb[40];
  char   buff[40];
  char   ch;
  char   cl;
  UI     c;
  int    i;
  int    address;

  /* calculate address of CRT matrix */

  address = 1004 - (nr * 20);

  check[0] = matbuff[1];
  check[1] = matbuff[2];
  check[2] = matbuff[3];
  check[3] = matbuff[5];
  check[4] = matbuff[6];
  check[5] = matbuff[7];

  send_string("MS", prt);
  strcpy(buff, "MA");
  itoa(address, tmpb, 10);
  strcat(buff, tmpb);
  send_string(buff, prt);

  for (i = 0; i < 6; i++)
    {
    strcpy(buff, "SM");
    c  = (UI)check[i];
    ch = c / 256;
    sprintf(tmpb, "%d", ch);
    strcat(buff, tmpb);
    send_string(buff, prt);

    strcpy(buff, "SM");
    c  = (UI)check[i];
    cl = c % 256;
    sprintf(tmpb, "%d", cl);
    strcat(buff, tmpb);
    send_string(buff, prt);
    }
  /* now check if matrix is written correctly */
  if (!read_phosphor_matrix(nr, tmp, prt))
    return (0);

  for (i = 0; i < 6; i++)
    if ((int) matbuff[i] != (int) tmp[i])
      return (0);
  return 1;
  }


/********************************************************************/
/*               MATRIX MANIPULATION ROUTINES                       */
/********************************************************************/

/* Matrix Inversion Routine                                         */
/* A = input matrix (n x n)                                         */
/* n = dimension of A                                               */
/* AInverse = inverted matrix (n x n)                               */
/* This function inverts a matrix based on the Gauss Jordan method. */
/* The function returns 1 on success, 0 on failure.                 */

int MatrixInversion(float* Atemp, int n, float* AInverse)
  {
  int i, j, iPass, imx, icol, irow;
  float det, temp, pivot, factor;
  float* ac;
  float* A;


  ac = (float*)calloc(n * n, sizeof(float));
  
  if (ac == NULL)
    {
    print("Cannot allocate memory!\n");
    return 0;
    }

  /* This function destroys contents of first argument.  So we copy
     first atgument to another array                               */
  A = (float*)calloc(n * n, sizeof(float));
  
  if (A == NULL)
    {
    free(A);
    print("Cannot allocate memory!\n");
    return 0;
    }

  for (i = 0; i < n * n; i++)
    A[i] = Atemp[i];


  det = 1.0;
  for (i = 0; i < n; i++)
    {
    for (j = 0; j < n; j++)
      {
      AInverse[n*i+j] = 0.0;
      ac[n*i+j]       = A[n*i+j];
      }
    AInverse[n*i+i] = 1.0;
    }
  /* The current pivot row is iPass.                                    */
  /* For each pass, first find the maximum element in the pivot column. */
  for (iPass = 0; iPass < n; iPass++)
    {
    imx = iPass;
    for (irow = iPass; irow < n; irow++)
      {
      if (fabs(A[n*irow+iPass]) > fabs(A[n*imx+iPass]))
        imx = irow;
      }
    /* Interchange the elements of row iPass and row imx in both A and AInverse. */
    if (imx != iPass)
      {
      for (icol = 0; icol < n; icol++)
        {
        temp                   = AInverse[n*iPass+icol];
        AInverse[n*iPass+icol] = AInverse[n*imx+icol];
        AInverse[n*imx+icol]   = temp;
        if (icol >= iPass)
          {
          temp            = A[n*iPass+icol];
          A[n*iPass+icol] = A[n*imx+icol];
          A[n*imx+icol]   = temp;
          }
        }
      }
    /* The current pivot is now A[iPass][iPass].             */
    /* The determinant is the product of the pivot elements. */
    pivot = A[n*iPass+iPass];
    det   = det * pivot;
    if (det == 0.0) 
      {
      free(A);
      free(ac);
      print("Matrix determinant = 0\n");
      return 0;
      }
    for (icol = 0; icol < n; icol++)
      {
      /* Normalize the pivot row by dividing by the pivot element. */
      AInverse[n*iPass+icol] = AInverse[n*iPass+icol] / pivot;
      if (icol >= iPass) A[n*iPass+icol] = A[n*iPass+icol] / pivot;
      }
    for (irow = 0; irow < n; irow++)
      /* Add a multiple of the pivot row to each row.  The multiple factor  */
      /* is chosen so that the element of A on the pivot column is 0.       */
      {
      if (irow != iPass)
        factor = A[n*irow+iPass];
      for (icol = 0; icol < n; icol++)
        {
        if (irow != iPass)
          {
          AInverse[n*irow+icol] -= factor * AInverse[n*iPass+icol];
          A[n*irow+icol] -= factor * A[n*iPass+icol];
          }
        }
      }
    }
  free(A);
  free(ac);
  return 1;
  }

/**********************************************************************
 * NAME       int print(char *msg, ...)
 * PURPOSE    To perform formated writing to console and to file
 *            
 * ARGUMENTS  char *msg, ... - format and arguments
 *
 * RETURNS    1
 *
 * EXTERNS    None.
 *
 * NOTES:     It seems that when writing to screen, sign is not printed,
 *            also, results are not exactly the same as when using
 *            ordinary printf().  Compiler error !?
 *            Writing to file is OK.
 *            It is that first (file or screen) writing is OK, second
 *            (file or sceen) is not OK.
 *  It is compiler error!!! Using BC3 gives both results OK
 **********************************************************************/
int print(char *msg, ...)
  {
  va_list argptr;

  va_start(argptr, msg);
  vprintf(msg, argptr);              /* write to screen */
  if (log_file != NULL)
    vfprintf(log_file, msg, argptr); /* write to file */
  va_end(argptr);

  return (1);
  }


/*
Data from '94 calibration run (k matrix routine);
Obtained from '94 calibration program.

 RÝd:       15.18     10.42      0.57
 GrÝn:       9.50     32.82      4.52
 Bl :        6.33      2.67     25.88
 Hvid:      31.10     46.09     31.20
 XFact: 1.482  YFact: 1.000  ZFact : 1.477
K-matrix beregnet:
  21654.60   1819.64   -328.76
    311.19  16572.14    207.96
    426.28   -881.63  27234.89
040903
K-matrix og PreGain skrevet til EÿPROM,  Kal status = FACTORY
      8192  -3148.22  -1791.72
  -3807.77      8192    362.51
    578.64  -1915.76      8192
MX type CRT-matrix EBU skrevet til EÿPROM
Pre gain:    1.482   1.000   1.478


Another set of data:


  mes->meas[0].X = 16.89;
  mes->meas[0].Y = 11.75;
  mes->meas[0].Z =  0.73;

  mes->meas[1].X = 10.84;
  mes->meas[1].Y = 34.77;
  mes->meas[1].Z =  5.04;

  mes->meas[2].X =  7.42;
  mes->meas[2].Y =  3.35;
  mes->meas[2].Z = 28.02;

  mes->meas[3].X = 32.61;
  mes->meas[3].Y = 48.72;
  mes->meas[3].Z = 32.51;
 RÝd:       16.89     11.75      0.73
 GrÝn:      10.84     34.77      5.04
 Bl :        7.42      3.35     28.02
 Hvid:      32.61     48.72     32.51
 XFact: 1.494  YFact: 1.000  ZFact : 1.499
K-matrix beregnet:
  23883.01    231.23  -1319.90
   1819.81  15015.24   -333.12
    619.14  -1253.64  26553.13
040906
K-matrix og PreGain skrevet til EÿPROM,  Kal status = FACTORY
      8192  -3403.87  -1890.70
  -3830.68      8192    357.07
    605.71  -2060.94      8192


A set of data from crt39:

  mes->meas[0].X =  782.23;
  mes->meas[0].Y =  447.21;
  mes->meas[0].Z =   31.07;

  mes->meas[1].X =  543.51;
  mes->meas[1].Y = 1205.03;
  mes->meas[1].Z =  217.83;

  mes->meas[2].X =  314.89;
  mes->meas[2].Y =  122.62;
  mes->meas[2].Z = 1435.34;

  mes->meas[3].X = 1640.55;
  mes->meas[3].Y = 1775.79;
  mes->meas[3].Z = 1687.01;

 XFact: 1.082  YFact: 1.000  ZFact : 1.053
K-matrix beregnet:
   4296.75    265.52    -29.31
   -449.34   4756.03     54.20
     16.01    -82.59   4945.62
040906
K-matrix og PreGain skrevet til EÿPROM,  Kal status = FACTORY
      8192  -3705.03  -1547.38
  -4338.89      8192    314.00
    683.00  -1892.23      8192
*/

/* This function will collect 4 sets of XYZ measurements */
/* made on white with decreasing level of red gun output */
/* Used only for some tests.                             */
int input(CAL_DATA *mes, int mode, int port)
  {
  get_any_key(port, "Place sensor on balanced white");
  Xcom_Flush(port);
  send_string("MC", port);  /* measure continously      */
  if (!get_7_measurements(&mes->meas[0], mode, port))
    return (0);
  send_string("MS", port);  /* measurement stop         */
  send_string("MS", port);  /* measurement stop         */

  print("mes->meas[0].X =%6.1f\n", mes->meas[0].X);
  print("mes->meas[0].Y =%6.1f\n", mes->meas[0].Y);
  print("mes->meas[0].Z =%6.1f\n", mes->meas[0].Z);

  get_any_key(port, "Decrease red gun output 70 pct");
  Xcom_Flush(port);
  send_string("MC", port);  /* measure continously      */
  if (!get_7_measurements(&mes->meas[1], mode, port))
    return (0);
  send_string("MS", port);  /* measurement stop         */
  send_string("MS", port);  /* measurement stop         */

  print("mes->meas[1].X =%6.1f\n", mes->meas[1].X);
  print("mes->meas[1].Y =%6.1f\n", mes->meas[1].Y);
  print("mes->meas[1].Z =%6.1f\n", mes->meas[1].Z);

  get_any_key(port, "Decrease red gun output by 65 pct");
  Xcom_Flush(port);
  send_string("MC", port);  /* measure continously      */
  if (!get_7_measurements(&mes->meas[2], mode, port))
    return (0);
  send_string("MS", port);  /* measurement stop         */
  send_string("MS", port);  /* measurement stop         */

  print("mes->meas[2].X =%6.1f\n", mes->meas[2].X);
  print("mes->meas[2].Y =%6.1f\n", mes->meas[2].Y);
  print("mes->meas[2].Z =%6.1f\n", mes->meas[2].Z);

  get_any_key(port, "Decrease red gun output by 60 pct");
  Xcom_Flush(port);
  send_string("MC", port);  /* measure continously      */
  if (!get_7_measurements(&mes->meas[3], mode, port))
    return (0);
  send_string("MS", port);  /* measurement stop         */
  send_string("MS", port);  /* measurement stop         */

  print("mes->meas[3].X =%6.1f\n", mes->meas[3].X);
  print("mes->meas[3].Y =%6.1f\n", mes->meas[3].Y);
  print("mes->meas[3].Z =%6.1f\n", mes->meas[3].Z);

  return (1);
  }
#if 0
  /* printf("INT_MAX = %d\n", INT_MAX); */
  strcpy(temp, "32767,32782,32768,25.0");
  extract_3_values(temp, &tmp); 
  X = (long)tmp.X;
  Y = (long)tmp.Y;
  Z = (long)tmp.Z;

  print("%s\n", temp);
  print("GAINf:   %f  %f  %f\n", tmp.X, tmp.Y, tmp.Z);

  x = (X >= (long)INT_MAX) ? -(int)(X - (long)INT_MAX) : (int)X;
  y = (Y >= (long)INT_MAX) ? -(int)(Y - (long)INT_MAX) : (int)Y;
  z = (Z >= (long)INT_MAX) ? -(int)(Z - (long)INT_MAX) : (int)Z;


  printf("GAINxd:   %d\n", x);
  printf("GAINyd:   %d\n", y);
  printf("GAINzd:   %d\n", z);
  do_exit(1, port, "That's it.\n");
#endif

#if 0
  send_string("NR", port);  /* normal run (?)                     */
  send_string("MS", port);  /* measurement stop                   */
  send_string("MX", port);  /* MX mode: unmodified sensor outoput */
  send_string("MO", port);  /* measure offset                     */

  /* wait for 2 answers (like in original .pas file) */
  for (j = 0; j < 2; j++)
    {
    i = 0;
    do
      {
      if (!Xcom_Getchar_wtimeout(&c, port, 30000L))
        {
        print("learn_offset: Timeout on receive.\n");
        return (0);
        }
      str[i++] = c;
      } while (c != '\r');
    str[i] = '\0';
    print ("~");
    }
  print ("\n");
#endif
