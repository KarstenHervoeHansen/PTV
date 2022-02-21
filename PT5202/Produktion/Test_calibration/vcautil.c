/* FILE NAME     vcautil.c
 * PURPOSE       This file contains low-level functions for controlling the R&S VCA
 *             
 * SPEC       
 * DESIGN
 * CODING    
 * CODE INSP.
 * TEST     
 *            
 * MODIFIED:
 *
 * NOTES:    
 *          
*/

#include <rs232.h>
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include "cviutil.h"
#include "vcautil.h"
#include "def.h"


// general variables

char *VCA_ID;

// variables for returnloss measurements

int   RL_data_length,
      RL_data_mode;
      
float RL_freq_start,
      RL_freq_stop,
      RL_freq_delta;

float RL_freq[300];
float RL_level[300];

// variables for amplitude spectrum measurements

int   AS_data_length,
      AS_data_mode;
      
float AS_freq_start,
      AS_freq_stop,
      AS_freq_delta;

float AS_freq[300];
float AS_level[300];





// variables for jitter time measurements

int   JT_data_length,
      JT_data_mode;
      
float JT_time_start,
      JT_time_stop,
      JT_time_delta;

float JT_freq[300];
float JT_level[300];

float JT_peakpeak_ui;

// variables for signal delay measurements

float SD_total;    // delay in seconds
int   SD_frames,
      SD_lines,
      SD_samples;

// -------------------- VCA selection ----------------------------------

void vca_reset(void)
  {
  WriteCOMPort(VCACOM, "*RST\r\n");    // see page 2.9 - 2.16
  Delay(0.5);
  }


void vca_readid(void)
  {
  char *vcadata;

  FlushInQ(VCACOM);
  WriteCOMPort(VCACOM, "*IDN?\r\n");  // Identification querry
  VCA_ID = ReadlnCOMPort(VCACOM,1.0);
  }   



void vca_selectinput(int input)       // Select input signal channels
  {
  switch (input) 
    {
    case SERIAL_A : WriteCOMPort(VCACOM, "ROUT:OPEN (@SER)\r\n"); break;
    case SERIAL_B : WriteCOMPort(VCACOM, "ROUT:OPEN (@BSER)\r\n"); break;
    default :     WriteCOMPort(VCACOM, "ROUT:OPEN (@BSER)\r\n"); break;
    }
   Delay(0.1);
  }

void vca_selectsystem(int system)     // Select standard
  {
  switch (system) 
    {
    case L625 : WriteCOMPort(VCACOM, "SYST:STAN L625\r\n"); break;
    case L525 : WriteCOMPort(VCACOM, "SYST:STAN L525\r\n"); break;
    }
  Delay(0.1); 
  }





// -------------------- return loss -----------------------------------

void vca_returnloss(void)
  {
  WriteCOMPort(VCACOM, "SENS:FUNC \"RLOS\"\r\n");  // Select operating mode
  Delay(0.5);
  }


void vca_returnloss_rescale(void)
  {
  WriteCOMPort(VCACOM, "DISP:RLOS:VAR RESC\r\n");  // Display, return loss, rescale
  Delay(0.5);
  }



void vca_returnloss_norm(int on_off)  // RLOS normalization ON/OFF
  {
  if (on_off == ON)
    WriteCOMPort(VCACOM, "CONF:RLOS:NORM ON\r\n");
  else
    WriteCOMPort(VCACOM, "CONF:RLOS:NORM OFF\r\n");
  Delay(0.05);
  }



void vca_returnloss_Ygain(int Ygain)
  {
   switch (Ygain) 
     {
     case Y2Div  : WriteCOMPort(VCACOM, "DISP:WIND:RLOS:GAIN:SCAL Y2DIV\r\n"); break;
     case Y5Div  : WriteCOMPort(VCACOM, "DISP:WIND:RLOS:GAIN:SCAL Y5DIV\r\n"); break;
     case Y10Div : WriteCOMPort(VCACOM, "DISP:WIND:RLOS:GAIN:SCAL Y10DIV\r\n"); break;
     default     : WriteCOMPort(VCACOM, "DISP:WIND:RLOS:GAIN:SCAL Y10DIV\r\n"); break;
     }
  Delay(0.05);
  }

void vca_returnloss_Xgain(int Xgain)
  {
  switch (Xgain) 
    {
    case X5Div   : WriteCOMPort(VCACOM, "DISP:WIND:RLOS:FREQ:SCAL X5DIV\r\n"); break;
    case X10Div  : WriteCOMPort(VCACOM, "DISP:WIND:RLOS:FREQ:SCAL X10DIV\r\n"); break;
    case X30Div  : WriteCOMPort(VCACOM, "DISP:WIND:RLOS:FREQ:SCAL X30DIV\r\n"); break;
    case X50Div  : WriteCOMPort(VCACOM, "DISP:WIND:RLOS:FREQ:SCAL X50DIV\r\n"); break;
    case X100Div : WriteCOMPort(VCACOM, "DISP:WIND:RLOS:FREQ:SCAL X100DIV\r\n"); break;
    default      : WriteCOMPort(VCACOM, "DISP:WIND:RLOS:FREQ:SCAL X50DIV\r\n"); break;
    }
  Delay(0.05);
  }


void vca_returnloss_sweep(int sweep)
  {
  switch (sweep) 
    {
    case FAST : WriteCOMPort(VCACOM, "CONF:RLOS:SWE FAST\r\n"); break;
    case NORM : WriteCOMPort(VCACOM, "CONF:RLOS:SWE NORM\r\n"); break;
    case SLOW : WriteCOMPort(VCACOM, "CONF:RLOS:SWE SLOW\r\n"); break;
    default   : WriteCOMPort(VCACOM, "CONF:RLOS:SWE NORM\r\n"); break;
    }
  Delay(0.05);
  }

void vca_returnloss_cursorfreq(int cursorfreq)
  {
  char freq[6];
  char cmd[50];

  if ((cursorfreq > 880) | (cursorfreq < 5))
    cursorfreq = 270;
  Fmt(freq,"%s<%i",cursorfreq);
  CopyString(cmd,0,"CONF:RLOS:CURS ",0,-1);
  CopyString(cmd,StringLength(cmd),freq,0,-1);
  CopyString(cmd,StringLength(cmd),"\r\n",0,-1);
  WriteCOMPort(VCACOM, cmd);
  Delay(0.05);
  }

void vca_returnloss_variation(int variation)
  {
  if (variation == WAVEF)
    WriteCOMPort(VCACOM, "DISP:WIND:RLOS:VAR WAV\r\n");
  else
    WriteCOMPort(VCACOM, "DISP:WIND:RLOS:VAR CURS\r\n");
  Delay(0.05);
  }



int vca_returnloss_readdata(void)
  {
  // data format is defined in the VCA manual on page 2.28 - 2.29
  int InQL;
  unsigned char *vcadata;
  int n;

  FlushInQ(VCACOM);
  WriteCOMPort(VCACOM, "READ:ARRAY:RLOS?\r\n");
  Delay(3.0);
  InQL = GetInQLen(VCACOM);
  vcadata = ReadBinCOMPort(VCACOM,1.0,InQL);
    
  memcpy(&RL_data_mode,&vcadata[5],2);
  memcpy(&RL_data_length,&vcadata[7],2);
  memcpy(&RL_freq_start,&vcadata[9],4);
  memcpy(&RL_freq_start,&vcadata[9],4);
  memcpy(&RL_freq_stop,&vcadata[13],4);
  memcpy(&RL_freq_delta,&vcadata[17],4);
  for (n = 0; n < RL_data_length; n++)
    {
    memcpy(&RL_level[n],&vcadata[(n*4)+21],4);
    RL_freq[n] = RL_freq_start + (n*RL_freq_delta);
    } 
  return(InQL);
  }


void vca_returnloss_readscale(void)
  {
  char *dataread;
  float aa[3];

  FlushInQ(VCACOM);
  WriteCOMPort(VCACOM, "READ:SCAL:RLOS:SCAL?\r\n");
  dataread = ReadlnCOMPort(VCACOM,2.0);
  Scan(dataread,"%s>%3f[b4x]",aa);
  RL_freq_start = aa[0];
  RL_freq_stop = aa[1];
  RL_freq_delta = aa[2];
  }




// -------------------- amplitude spectrum -----------------------------------
void vca_amplspec(void)
  {
  WriteCOMPort(VCACOM, "SENS:FUNC \"AMSP\"\r\n");
  Delay(1.5);
  }


void vca_amplspec_rescale(void)
  {
  WriteCOMPort(VCACOM, "DISP:AMSP:VAR RESC\r\n");
  Delay(0.5);
  }




void vca_amplspec_sweep(int sweep)
  {
  switch (sweep) 
    {
    case FAST : WriteCOMPort(VCACOM, "CONF:AMSP:SWE FAST\r\n"); break;
    case NORM : WriteCOMPort(VCACOM, "CONF:AMSP:SWE NORM\r\n"); break;
    case SLOW : WriteCOMPort(VCACOM, "CONF:AMSP:SWE SLOW\r\n"); break;
    default   : WriteCOMPort(VCACOM, "CONF:AMSP:SWE NORM\r\n"); break;
    }
  Delay(0.05);
  }

void vca_amplspec_norm(int on_off)
  {
  if (on_off == ON)
    WriteCOMPort(VCACOM, "CONF:AMSP:NORM ON\r\n");
  else
     WriteCOMPort(VCACOM, "CONF:AMSP:NORM OFF\r\n");
  Delay(0.05);
  }

void vca_amplspec_Ygain(int Ygain)
  {
  switch (Ygain) 
    {
    case Y2Div  : WriteCOMPort(VCACOM, "DISP:WIND:AMSP:GAIN:SCAL Y2DIV\r\n"); break;
    case Y5Div  : WriteCOMPort(VCACOM, "DISP:WIND:AMSP:GAIN:SCAL Y5DIV\r\n"); break;
    case Y10Div : WriteCOMPort(VCACOM, "DISP:WIND:AMSP:GAIN:SCAL Y10DIV\r\n"); break;
    default     : WriteCOMPort(VCACOM, "DISP:WIND:AMSP:GAIN:SCAL Y10DIV\r\n"); break;
    }
  Delay(0.05);
  }

void vca_amplspec_Xgain(int Xgain)
  {
  switch (Xgain) 
    {
    case X5Div   : WriteCOMPort(VCACOM, "DISP:WIND:AMSP:FREQ:SCAL X5DIV\r\n"); break;
    case X10Div  : WriteCOMPort(VCACOM, "DISP:WIND:AMSP:FREQ:SCAL X10DIV\r\n"); break;
    case X30Div  : WriteCOMPort(VCACOM, "DISP:WIND:AMSP:FREQ:SCAL X30DIV\r\n"); break;
    case X50Div  : WriteCOMPort(VCACOM, "DISP:WIND:AMSP:FREQ:SCAL X50DIV\r\n"); break;
    case X100Div : WriteCOMPort(VCACOM, "DISP:WIND:AMSP:FREQ:SCAL X100DIV\r\n"); break;
    default      : WriteCOMPort(VCACOM, "DISP:WIND:AMSP:FREQ:SCAL X50DIV\r\n"); break;
    }
  }

void vca_amplspec_cursorfreq(int cursorfreq)
  {
  char freq[6];
  char cmd[50];

  if ((cursorfreq > 880) | (cursorfreq < 5))
    cursorfreq = 270;
  Fmt(freq,"%s<%i",cursorfreq);
  CopyString(cmd,0,"CONF:AMSP:CURS ",0,-1);
  CopyString(cmd,StringLength(cmd),freq,0,-1);
  CopyString(cmd,StringLength(cmd),"\r\n",0,-1);
  WriteCOMPort(VCACOM, cmd);
  Delay(0.05);
  }

int vca_amplspec_readdata(void)
  {
  // data format is defined in the VCA manual on page 2.27
  int InQL;
  unsigned char *vcadata;
  int n;

  FlushInQ(VCACOM);
  WriteCOMPort(VCACOM, "READ:ARRAY:AMSP?\r\n");
  Delay(3.0);
  InQL = GetInQLen(VCACOM);
  if (InQL < 0) // if error, JK change
    {
    return (InQL);
    }
  vcadata = ReadBinCOMPort(VCACOM,1.0,InQL);
    
  memcpy(&AS_data_mode,&vcadata[5],2);
  memcpy(&AS_data_length,&vcadata[7],2);
  memcpy(&AS_freq_start,&vcadata[9],4);
  memcpy(&AS_freq_stop,&vcadata[13],4);
  memcpy(&AS_freq_delta,&vcadata[17],4);
  for (n = 0; n < AS_data_length; n++)
    {
    memcpy(&AS_level[n],&vcadata[(n*4)+21],4);
    AS_freq[n] = AS_freq_start + (n*AS_freq_delta);
    } 
  return(InQL);
  }

void vca_amplspec_readscale(void) 
  {
  char *dataread;
  float aa[3];

  FlushInQ(VCACOM);
  WriteCOMPort(VCACOM, "READ:SCAL:AMSP:SCAL?\r\n");
  dataread = ReadlnCOMPort(VCACOM,2.0);
  Scan(dataread,"%s>%3f[b4x]",aa);
  AS_freq_start = aa[0];
  AS_freq_stop  = aa[1];
  AS_freq_delta = aa[2];
}

// -------------------- jitter time -----------------------------------
void vca_jittertime(void)
  {
  WriteCOMPort(VCACOM, "SENS:FUNC \"JTIM\"\r\n");
  Delay(0.5);
  }

void vca_jittertime_average(int aver)
  {
  switch (aver) 
    {
    case A1   : WriteCOMPort(VCACOM, "CALC:AVER:JTIM:COUN A1\r\n"); break;
    case A10  : WriteCOMPort(VCACOM, "CALC:AVER:JTIM:COUN A10\r\n"); break;
    case A50  : WriteCOMPort(VCACOM, "CALC:AVER:JTIM:COUN A50\r\n"); break;
    case A100 : WriteCOMPort(VCACOM, "CALC:AVER:JTIM:COUN A100\r\n"); break;
    case A200 : WriteCOMPort(VCACOM, "CALC:AVER:JTIM:COUN A200\r\n"); break;
    default   : WriteCOMPort(VCACOM, "CALC:AVER:JTIM:COUN A100\r\n"); break;
    }
  Delay(0.05);
  }

void vca_jittertime_lpfilter(int lp)
  {
  switch (lp) 
    {
    case OFF   : WriteCOMPort(VCACOM, "CONF:JTIM:LOWP OFF\r\n"); break;
    case F1K   : WriteCOMPort(VCACOM, "CONF:JTIM:LOWP F1K\r\n"); break;
    case F100K : WriteCOMPort(VCACOM, "CONF:JTIM:LOWP F100K\r\n"); break;
    default    : WriteCOMPort(VCACOM, "CONF:JTIM:LOWP F1K\r\n"); break;
    }
  Delay(0.05);
  }


void vca_jittertime_meastime(int meastime)
  {
  switch (meastime) 
    {
    case  FRAME4 : WriteCOMPort(VCACOM, "CONF:JTIM:SPAN F4R\r\n"); break;
    case  FRAME1 : WriteCOMPort(VCACOM, "CONF:JTIM:SPAN FRAM\r\n"); break;
    case  LINE   : WriteCOMPort(VCACOM, "CONF:JTIM:SPAN LIN\r\n"); break;
    default      : WriteCOMPort(VCACOM, "CONF:JTIM:SPAN FRAM\r\n"); break;
    }
  Delay(0.05);
  }  


void vca_jittertime_Xgain(int Xgain)
  {
  switch (Xgain) 
    {
    case  X1  : WriteCOMPort(VCACOM, "DISP:WIND:JTIM:MAGN X1\r\n"); break;
    case  X10 : WriteCOMPort(VCACOM, "DISP:WIND:JTIM:MAGN X10\r\n"); break;
    default   : WriteCOMPort(VCACOM, "DISP:WIND:JTIM:MAGN X1\r\n"); break;
    }
  Delay(0.05);
  }

void vca_jittertime_Ygain(int Ygain)
  {
  switch (Ygain) 
    {
    case  Y1  : WriteCOMPort(VCACOM, "DISP:WIND:JTIM:MAGN Y1\r\n"); break;
    case  Y5  : WriteCOMPort(VCACOM, "DISP:WIND:JTIM:MAGN Y5\r\n"); break;
    case  Y10 : WriteCOMPort(VCACOM, "DISP:WIND:JTIM:MAGN Y10\r\n"); break;
    default   : WriteCOMPort(VCACOM, "DISP:WIND:JTIM:MAGN Y1\r\n"); break;
    }
  Delay(0.05);
  }

int vca_jittertime_readscale(void)
  {
  char *dataread;
  float aa[3];

  FlushInQ(VCACOM);
  WriteCOMPort(VCACOM, "READ:SCAL:JTIM:SCAL?\r\n");
  dataread = ReadlnCOMPort(VCACOM,2.0);
  Scan(dataread,"%s>%3f[b4x]",aa);
  JT_time_start = aa[0];
  JT_time_stop = aa[1];
  JT_time_delta = aa[2];
  return(strlen(dataread));   
  }

int vca_jittertime_readpeakpeak(float *ppUI)
  {
  char *dataread;

  FlushInQ(VCACOM);
  WriteCOMPort(VCACOM, "READ:SCAL:JTIM:P2P?\r\n");
  dataread = ReadlnCOMPort(VCACOM,2.0);
  Scan(dataread,"%s>%f[b4]",&JT_peakpeak_ui);
  *ppUI = JT_peakpeak_ui;
  return(strlen(dataread));   
  }


// -------------------- signal delay -----------------------------------
void vca_signaldelay(void)
  {
  WriteCOMPort(VCACOM, "SENS:FUNC \"SIGD\"\r\n");
  Delay(0.5);
  }
 
int vca_signaldelay_readdelay(float *delaytotal)
  {
  char *dataread;
  float aa[4];

  FlushInQ(VCACOM);
  WriteCOMPort(VCACOM, "READ:SCAL:SIGD?\r\n");
  dataread = ReadlnCOMPort(VCACOM,2.0);
  Scan(dataread,"%s>%4f[b4x]",aa);
  SD_total = aa[0];
  *delaytotal = SD_total;
  Fmt(&SD_frames,"%i<%f",aa[1]);
  Fmt(&SD_lines,"%i<%f",aa[2]);
  Fmt(&SD_samples,"%i<%f",aa[3]);
  return(strlen(dataread));   
  }

