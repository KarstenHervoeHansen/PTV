/* FILE NAME    sditest.c
 * PURPOSE      
 *             
 * SPEC       
 * DESIGN
 * CODING    
 * CODE INSP.
 * TEST     
 *            
 * MODIFIED:
 *
 * NOTES:     EDH Gennum chip ikke monteret - se 'CheckSDIEmbAudio_EDH'
 *            PT5201:  SDI Output
 *            PF 000626
 *          
*/

#include <analysis.h>
#include <rs232.h>
#include <ansi_c.h>
#include "sdi.h"
#include <utility.h>
#include <formatio.h>
#include <userint.h>
#include "cviutil.h"
#include "vcautil.h"
#include "pt5201.h"
#include "bbcal.h"
#include "def.h"




// constants
const float xd = 0.2; // omgå fejl i remote system pga overflow i input buffer
                      // ellers kræves handshake


const float JT_peakpeakMaxFrame4 = 0.2; //max 0.12UI målt på SDI625 75% color bar
                      // 4 frames   lp=off
const float JT_peakpeakMaxLine  = 0.07; //max 0.03UI målt på SDI625 75% color bar
                      // 1 line     lp=1kHz

const float CheckFreqAmsp[2] = {10,350};  // Frekevns hvor ampl. spec. checkes
const float AMSP_Max[2] = {+0.65,-12};    // Max level ved amsp check frekvens
const float AMSP_Min[2] = {-0.65,-40};    // Min level ved amsp check frekvens

const float CheckRLoss[3] = {20 , 150, 270};// Frekevns hvor return loss. checkes
const float RLoss_Max[3]  = {-30, -23, -20};// Max level ved checkfrekvensen

const double VCOAdjTol = 0.02;        // ± volt tol ved SDI genlock justering (TP1/R6)


char  SDIKUStr[20];

float jitterMeanFrame4,
      jitterMeanLine;
float LevelmV;


// variables
int jitPNL,
    sdiPNL,
    pattPNL,
    amspPNL,
    rlossPNL,
    sdigenlPNL;
  
int SDIJitterOk,
    SDITimingOk,
    SDIAmplSpecOk,
    SDIRLossOk,
    SDIGenlOk,
    SDIPattOk,
    SDIEmbAudioOk,
    VCAOk;

int XX1_Stik;

unsigned int SDICalphase[2];


// prototyping ----------------------------------------------------------------
int   InitSDI(void);
int   WriteSDICalDate(void);
char  *ReadSDIKUNumber (void);
char  *ReadSDIGenlKUNumber (void);
char  *ReadSDIGenlProdDate (void);
char  *ReadSDIGenlCalDate (void);
int   SDIReturnLossMeasurement(void);
char  *ReadSDITPGHwVer (void);
char  *ReadSDITPGSwVer (void);
int   ResetSDI(int sdino);
int   SetupPT5211(void);








//---------------------------------------------------------------------------------
int CheckSDIEmbAudio_EDH(void)
  {
  // 970819  PT5210 remote komando syntax for embedded audio level ændret. 
  // Gammel syntax gælder til og med PT5210-master SW rev 2.1. 
  // 980325  PT5230 anvender kun ny syntax.


  double AudioLevelShiftDelay;
  char TxtStr[60];
  int handle_returned, controlID_returned;
 
  SDIEmbAudioOk = TRUE;

  DisplayPanel (pattPNL);
  SetCtrlVal(pattPNL, PATT_PNL_SCOPMSG,"");

  // init panel

  Fmt(TxtStr,"%s< Embedded Audio in SDI Color Bar ");
  SetPanelAttribute (pattPNL, ATTR_TITLE, TxtStr);
 
  ResetTextBox(pattPNL, PATT_PNL_TXTBOX,"");
 
 
  SDIEmbAudioOk = WriteProtect(OFF,FALSE);
  if (SDIEmbAudioOk == FALSE)
    {
    HidePanel (pattPNL);
    return SDIEmbAudioOk;
    }
 
  // Embedded Audio = Silence
  // Audio Group = 1
  WriteCOMPort(DUTCOM,":OUTP:TSG:EMB:SIGN SIL;");
 
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Forbind SDI generator til DEX3072 D1 IN\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Forbind DEX3072 AES75 OUT A til TEK764 AES75 IN\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Indstil DEX3072: CH SELECT A = 0 og derefter = 1\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"        Aflæs:  A = grøn LED\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Indstil TEK764:  AUDIO\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"        Aflæs:  CH-1 og CH-2 = MUTE\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Indstil TEK764:  VIEW = CH STATUS      FORMAT = Text\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"        Aflæs:  Block CRC = is valid\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"        Aflæs:  Channel use = professional\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Tast F4: OK\n      F9: STOP");

  WaitForContinue();
 
 if (SDIEmbAudioOk == FALSE)  // <<<<<<< Why this one more time? :JK
   {
   //MessagePopup (" SDI Embedded Audio", "FEJLET");
   HidePanel(pattPNL);
   return SDIEmbAudioOk;
   }

 //  Embedded Audio = OFF   

  WriteCOMPort(DUTCOM,":OUTP:TSG:EMB:SIGN OFF;");
  WriteCOMPort(DUTCOM,":OUTP:TSG:PATT BLACK;");

  ResetTextBox(pattPNL, PATT_PNL_TXTBOX,"");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"\n Indstil TEK764:  AUDIO\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"        Aflæs:  CH-1 og CH-2 = VBIT SET\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Tast F4: OK\n      F9: STOP\n\n");
 
  WaitForContinue();
 
  if (SDIEmbAudioOk == FALSE)
    {
    //MessagePopup (" SDI Embedded Audio", "FEJLET");
    HidePanel(pattPNL);
    return SDIEmbAudioOk;
    }
/* Removed by JK, redundant
 
 if (SDIEmbAudioOk == FALSE)
   {
   MessagePopup (" SDI TSG Embedded Audio", "FEJLET");
   HidePanel(pattPNL);
   return SDIEmbAudioOk;
   } 
*/  

  HidePanel(pattPNL);
  return SDIEmbAudioOk;

#if 0
  // EDH Gennum chip ikke monteret
  //  EDH = ON Test ---------------------------------------
  SDIEmbAudioOk = TRUE;
  vca_selectsystem(L625);
  vca_selectinput(SERIAL_B);
 
  WriteCOMPort(DUTCOM,":OUTP:SB34:SYST SDI625;PATT BLACK;EMB OFF;EDH ON;");

  SetPanelAttribute (pattPNL, ATTR_TITLE, " SDI EDH Test ");
  ResetTextBox(pattPNL, PATT_PNL_TXTBOX,"");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Forbind SDI generator til VCA B-IN\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Indstil R&S VCA:  SCOPE  -  ANC DATA STATUS  -  INFO\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Aflæs VCA:\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"                          FIELD 1   FIELD 2\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"    ACTIVE PICTURE CRC:   7AC2      7AC2\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"    FULL-FIELD CRC:       3765      6455\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Tast F4: OK\n      F9: STOP\n\n\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," SDI-generator er indstillet til:\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"   SYSTEM=625/50   PATTERN=BLACK   AUDIO=OFF   EDH=ON   TEXT=OFF");
 
  WaitForContinue();
 
  if (SDIEmbAudioOk == FALSE)
    {
    MessagePopup (" SDI  EDH", "FEJLET");
    HidePanel(pattPNL);
    return SDIEmbAudioOk;
    }
 
  // EDH = OFF Test ----------------------------------------
  WriteCOMPort(DUTCOM,":OUTP:SB34:SYST SDI625;EDH OFF;");

  ResetTextBox(pattPNL, PATT_PNL_TXTBOX,"");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Aflæs VCA:\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"                          FIELD 1   FIELD 2\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"    ACTIVE PICTURE CRC:   0000      0000\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"    FULL-FIELD CRC:       0000      0000\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Tast F4: OK\n      F9: STOP\n\n\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," SDI-generator er indstillet til:\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"   SYSTEM=625/50   PATTERN=BLACK   AUDIO=OFF   EDH=OFF   TEXT=OFF");
 
  WaitForContinue();
 
 
  if (SDIEmbAudioOk == TRUE)
    MessagePopup (" SDI Embedded Audio & EDH", "OK");
  else
    MessagePopup (" SDI Embedded Audio & EDH", "FEJLET");
   

  HidePanel(pattPNL);

  return SDIEmbAudioOk;
#endif
  } // End of CheckSDIEmbAudio_EDH

 
//---------------------------------------------------------------------------
int SDIJitterTest(void)
  {
  float jitterPPmeas[20];
  float  jitterMean;
  char TxtStr[60],
       *KUread;
  int handle_returned, 
      controlID_returned, 
      n, 
      k,
      xx_stik;

  // init panel
  Fmt(TxtStr,"%s< Jittermålinger:  SDI Test Signal Generator ");

  SetPanelAttribute (jitPNL, ATTR_TITLE, TxtStr);
  SetCtrlAttribute (jitPNL, SDI_PNL_SDIOK_BTN, ATTR_VISIBLE, TRUE);

  ResetTextBox(jitPNL, JIT_PNL_PPMEAS,"");
  SetCtrlVal(jitPNL, JIT_PNL_PPTOL1 ,"");
  SetCtrlVal(jitPNL, JIT_PNL_PPMEAN1 ,"");
  SetCtrlVal(jitPNL, JIT_PNL_PPTOL2 ,"");
  SetCtrlVal(jitPNL, JIT_PNL_PPMEAN2 ,"");
 
  ResetTextBox(jitPNL, JIT_PNL_TXTBOX,"");
 
  Fmt(TxtStr,"%s< %f[b4] UI", JT_peakpeakMaxFrame4);
  SetCtrlVal(jitPNL, JIT_PNL_PPTOL1,TxtStr);
 
  Fmt(TxtStr,"%s< %f[b4] UI", JT_peakpeakMaxLine);
  SetCtrlVal(jitPNL, JIT_PNL_PPTOL2,TxtStr);
  ProcessDrawEvents();

  DisplayPanel (jitPNL);
 
  SDIJitterOk = WriteProtect(OFF,FALSE);
  if (SDIJitterOk == FALSE)
    {
    HidePanel (jitPNL);
    return SDIJitterOk;
    }
 
  SetCtrlVal(jitPNL, JIT_PNL_TXTBOX," Forbind SDI generator til R&S VCA B-in\n\n");
  SetCtrlVal(jitPNL, JIT_PNL_TXTBOX," F4: OK \n F9: STOP\n");

  SDIJitterOk = TRUE; 

  WaitForContinue();
 
  if (SDIJitterOk == FALSE) 
    {
    HidePanel(jitPNL);
    return FALSE;
    }

  k = 1;   // k=1:  1.jittermåling   4 frames lpf=off
           // k=2:  2.jittermåling   1 line   lpf=100kHz


  while ((k < 3) && (SDIJitterOk == TRUE))
    {
    ResetTextBox(jitPNL, JIT_PNL_PPMEAS,"");

    if (k == 1)
      {
      SetCtrlVal (jitPNL, JIT_PNL_VCASETUP,"Jitter målinger: 4 FRAMES,  LPF = OFF");
      } 
    if (k == 2)
      {
      SetCtrlVal (jitPNL, JIT_PNL_VCASETUP, "Jitter målinger: 1 LINE,  LPF = 100 kHz");
      } 
 
    ProcessDrawEvents();
  
    vca_selectsystem(L625);
    vca_selectinput(SERIAL_B);
    vca_jittertime();
    if (k == 1)
      {
      vca_jittertime_lpfilter(OFF);
      vca_jittertime_meastime(FRAME4);
      }
    if (k == 2)
      {
      vca_jittertime_lpfilter(F100K);
      vca_jittertime_meastime(LINE);
      }
    vca_jittertime_Xgain(X1);
    vca_jittertime_Ygain(Y1);
    vca_jittertime_average(A10);

    //WriteCOMPort(DUTCOM,":OUTP:SB34:SYST SDI625; PATT CBEBU;");//<<<<<<< error
    WriteCOMPort(DUTCOM,":OUTP:TSG:SYST PAL;");
    Delay(2.0);
    WriteCOMPort(DUTCOM,":OUTP:TSG:PATT CBEBU;");
    Delay(2.0);
    vca_jittertime_readpeakpeak(&jitterPPmeas[0]);
 
    // get 15 measurements
    for (n = 0; n < 15; n++) 
      {
      Delay(1.0);
      vca_jittertime_readpeakpeak(&jitterPPmeas[n]);

      Fmt(TxtStr,"%s< %f[b4p3]\n", JT_peakpeak_ui);
      SetCtrlVal(jitPNL, JIT_PNL_PPMEAS ,TxtStr);
      } //for 

    // calculate mean from meas #10..#15
    jitterMean = 0;
    for (n = 10; n < 15; n++) 
      jitterMean = jitterMean + jitterPPmeas[n];

    if (k == 1)
      jitterMeanFrame4 = jitterMean / 5;
    if (k == 2)
      jitterMeanLine = jitterMean / 5;

    if (k == 1)
      if (jitterMeanFrame4 >= JT_peakpeakMaxFrame4)
        SDIJitterOk = FALSE; 
    if (k == 2)
      if (jitterMeanLine >= JT_peakpeakMaxLine)
        SDIJitterOk = FALSE; 

    if (k == 1)
      {
      Fmt(TxtStr,"%s< %f[b4p3] UI", jitterMeanFrame4);
      SetCtrlVal(jitPNL, JIT_PNL_PPMEAN1 ,TxtStr);
      } 
    if (k == 2)
      {
      Fmt(TxtStr,"%s< %f[b4p3] UI", jitterMeanLine);
      SetCtrlVal(jitPNL, JIT_PNL_PPMEAN2 ,TxtStr);
      } 
    k++;
    } // while (k...

  if (SDIJitterOk == TRUE)
    {
    MessagePopup (" SDI Jitter Time", "Måling   OK");
    }
  else
    {
    MessagePopup (" SDI Jitter Time", "Måling   FEJLET");
    } 

  HidePanel(jitPNL);
  return SDIJitterOk;
  } // SDIJitterTest



//---------------------------------------------------------------------------------
int SDIAmplSpecTest(void)
  {
  char  *strread, 
        *KUread, 
        TxtStr[100];
  int   handle_returned, 
        controlID_returned, 
        n, 
        amsp_handle,
        LevelmV,
        xx_stik;
  float LevelMeas[2];

  // init panel
  Fmt(TxtStr,"%s< Amplitudespectrum:  SDI Output ");

  SetPanelAttribute (amspPNL, ATTR_TITLE, TxtStr);
  SetCtrlAttribute (amspPNL, SDI_PNL_SDIOK_BTN, ATTR_VISIBLE, TRUE);
 
  Fmt(TxtStr,"%s< Level ved %f[b4p0] MHz: %f[b4p2] til %f[b4p2] dB",CheckFreqAmsp[0],AMSP_Min[0],AMSP_Max[0]);
  SetCtrlAttribute (amspPNL, AMSP_PNL_LEV_0, ATTR_LABEL_TEXT, TxtStr);
  Fmt(TxtStr,"%s< Level ved %f[b4p0] MHz:  max: %f[b4p2] dB",CheckFreqAmsp[1],AMSP_Max[1]);
  SetCtrlAttribute (amspPNL, AMSP_PNL_LEV_1, ATTR_LABEL_TEXT, TxtStr);

  DisplayPanel (amspPNL);
  DeleteGraphPlot (amspPNL, AMSP_PNL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
               
  SDIAmplSpecOk = WriteProtect(OFF,FALSE);
 if (SDIAmplSpecOk == FALSE)
   {
   HidePanel (amspPNL);
   return SDIAmplSpecOk;
   }

  SetCtrlVal(amspPNL, AMSP_PNL_LEV_0 ,"");
  SetCtrlVal(amspPNL, AMSP_PNL_LEV_1 ,"");

  ResetTextBox(amspPNL, AMSP_PNL_TXTBOX,"");
  SetCtrlVal(amspPNL, AMSP_PNL_TXTBOX," Forbind SDI generator til R&S VCA B-in\n\n");
  SetCtrlVal(amspPNL, AMSP_PNL_TXTBOX," F4: OK \n F9: STOP\n\n");
  ProcessDrawEvents();

  SDIAmplSpecOk = TRUE; 
 
  WaitForContinue();
 
  if (SDIAmplSpecOk == FALSE) 
    {
    HidePanel(amspPNL);
    return FALSE;
    }
 
  SetCtrlVal(amspPNL, AMSP_PNL_TXTBOX," Vent *");

  vca_selectsystem(L625);
  vca_selectinput(SERIAL_B);
  vca_amplspec();
  vca_amplspec_rescale();
  vca_amplspec_sweep(NORM);
  vca_amplspec_norm(OFF);
  vca_amplspec_Xgain(X50Div);    //   MHz/div
  vca_amplspec_Ygain(Y10Div);    //   dB/div
 

   //WriteCOMPort(DUTCOM,":OUTP:SB34:SYST SDI625; PATT CBEBU;");//<<<<<<<<< command must be for 5201
  WriteCOMPort(DUTCOM,":OUTP:TSG:SYST PAL;");
  Delay(2.0);
  WriteCOMPort(DUTCOM,":OUTP:TSG:PATT CBEBU;");
  Delay(2.0);
  for (n = 0; n < 4; n++)
    {
    SetCtrlVal(amspPNL, AMSP_PNL_TXTBOX," *");
    Delay(2.0);
    }

  SetCtrlVal(amspPNL, AMSP_PNL_TXTBOX,"\n");
  vca_amplspec_readdata();
#if 0  
  if (vca_amplspec_readdata())
    {
    // popup error message
    MessagePopup (" SDI Amplitude Spectrum", "Fejl paa COMPort");
    SDIAmplSpecOk = FALSE;
    HidePanel(amspPNL);    
    return SDIAmplSpecOk;
    }
#endif 
  LevelMeas[0] = AS_level[(int)(CheckFreqAmsp[0]/AS_freq_delta) - 1];
  LevelMeas[1] = AS_level[(int)(CheckFreqAmsp[1]/AS_freq_delta) - 1];
 
  Fmt(TxtStr,"%s< %f[b4p2] dB:",LevelMeas[0]);
  SetCtrlVal (amspPNL, AMSP_PNL_LEV_0, TxtStr);
  Fmt(TxtStr,"%s< %f[b4p2] dB:",LevelMeas[1]);
  SetCtrlVal (amspPNL, AMSP_PNL_LEV_1, TxtStr);
 
  amsp_handle = PlotXY (amspPNL, AMSP_PNL_GRAPH, AS_freq,
                        AS_level,AS_data_length, VAL_FLOAT, VAL_FLOAT,VAL_THIN_LINE,
                        VAL_EMPTY_SQUARE, VAL_SOLID, 1,VAL_RED);

  if ((LevelMeas[0] > AMSP_Max[0]) || (LevelMeas[0] < AMSP_Min[0])  || 
      (LevelMeas[1] > AMSP_Max[1]))
    SDIAmplSpecOk = FALSE;
 
 
  if (SDIAmplSpecOk == TRUE) 
    {
    LevelmV = (int)800.0 * pow (10.0, (LevelMeas[0] / 20.0)); // fra dB til mV (800mV=0dB)
    // get system date 
    dato = GetDate(1);
 
    // get system time
    tt = TimeStr();
  
    // check om logfil findes, ellers opret en ny logfil
    fhandle = OpenFile (SDIMeasLogFile[LogDest], VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);
   
    // SDI jitter & amplitude spectrum målinger gemmes i logfil
    Fmt(wrtbuf,"%s<Jitter,%s,%s,KUxxxxxx,%f[b4p3],%f[b4p3],   Amsp,%i\n",dato,tt,jitterMeanFrame4,jitterMeanLine,LevelmV); 
    bytes_written = WriteFile (fhandle, wrtbuf, strlen(wrtbuf));
    CloseFile(fhandle);
    MessagePopup (" SDI Amplitude Spectrum", "Måling   OK");
    Fmt(TxtStr,"%s<%i[w3]",LevelmV);   //  Amsp = xxx
    }
  else
    {
    MessagePopup (" SDI Amplitude Spectrum", "Måling   FEJLET");
    } 

  HidePanel(amspPNL);
  return SDIAmplSpecOk;
  } //SDIAmplSpecTest


//---------------------------------------------------------------------------------
int SDIReturnLossTest(void)
  {
  // kræver at clock til SDI-Gennum chip GS9032 kan afbrydes
  char  *strread,
        *KUread,
        TxtStr[100];
  int   handle_returned, 
        controlID_returned, 
        n,
        xx_stik,
        rloss_handle;
  float LevelMeas[3], RLossLevel;

  // init panel
  Fmt(TxtStr,"%s< Return Loss:  SDI Test Signal Generator ");

  SetPanelAttribute (rlossPNL, ATTR_TITLE, TxtStr);
  SetCtrlAttribute (rlossPNL, SDI_PNL_SDIOK_BTN, ATTR_VISIBLE, TRUE);
 
  ResetTextBox(rlossPNL, RLOSS_PNL_MAXLEVEL,"");

  DisplayPanel (rlossPNL);
               
  SDIRLossOk = WriteProtect(OFF,FALSE);
  if (SDIRLossOk == FALSE)
    {
    HidePanel (rlossPNL);
    return SDIRLossOk;
    }
 
  ResetTextBox(rlossPNL, RLOSS_PNL_TXTBOX,"");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 input forbindes til R&S VCA NOISE\n");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 output forbindes til R&S VCA B-in\n\n");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 TEST forbindes IKKE!!!\n\n");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Placer jumper PP1 på SDI generator\n\n");
 
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," F4: OK \n F9: STOP\n\n");

  WaitForContinue();

  if (SDIRLossOk == FALSE) 
    {
    HidePanel(rlossPNL);
    return SDIRLossOk;
    }
 
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Vent *");
  vca_selectsystem(L625);
  vca_selectinput(SERIAL_B);
  vca_returnloss();
  vca_returnloss_rescale();
  vca_returnloss_Ygain(Y10Div);
  vca_returnloss_Xgain(X50Div);
  vca_returnloss_sweep(NORM);
  vca_returnloss_cursorfreq(270);
  vca_returnloss_norm(OFF);
  Delay(2.0);
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," *");
  Delay(2.0);
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," *");
 
  vca_returnloss_norm(ON);
 
  Delay(2.0);
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," *");
  Delay(2.0);

  // Den ene SDI udgang måles
  ResetTextBox(rlossPNL, RLOSS_PNL_TXTBOX,"");
  ResetTextBox(rlossPNL, RLOSS_PNL_MAXLEVEL,"");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 TEST forbindes til SDI output\n");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebroen SKAL forbindes direkte til BNC-stik\n\n");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," HUSK: Jumper i PP1 inden tryk på F4\n\n");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," F4: OK \n F9: STOP\n\n");
  DeleteGraphPlot (rlossPNL, RLOSS_PNL_GRAPH, -1, VAL_IMMEDIATE_DRAW);

  WaitForContinue();

 if (SDIRLossOk == FALSE) 
   {
   HidePanel(rlossPNL);
   return SDIRLossOk;
   }
 
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Vent *");
  Delay(2.0);
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," *");
  Delay(2.0);
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," *");
 
  vca_returnloss_readdata();
 
  LevelMeas[0] = RL_level[(int)(CheckRLoss[0]/RL_freq_delta) - 1];
  LevelMeas[1] = RL_level[(int)(CheckRLoss[1]/RL_freq_delta) - 1];
  LevelMeas[2] = RL_level[(int)(CheckRLoss[2]/RL_freq_delta) - 1];
 
  Fmt(TxtStr,"%s< %f[w3b4p0]MHz: %f[w5b4p1]dB  (max: %f[w5b4p1])\n",CheckRLoss[0],LevelMeas[0],RLoss_Max[0]);
  SetCtrlVal(rlossPNL, RLOSS_PNL_MAXLEVEL,TxtStr);
  Fmt(TxtStr,"%s< %f[w3b4p0]MHz: %f[w5b4p1]dB  (max: %f[w5b4p1])\n",CheckRLoss[1],LevelMeas[1],RLoss_Max[1]);
  SetCtrlVal(rlossPNL, RLOSS_PNL_MAXLEVEL,TxtStr);
  Fmt(TxtStr,"%s< %f[w3b4p0]MHz: %f[w5b4p1]dB  (max: %f[w5b4p1])\n",CheckRLoss[2],LevelMeas[2],RLoss_Max[2]);
  SetCtrlVal(rlossPNL, RLOSS_PNL_MAXLEVEL,TxtStr);

  // plot data
  rloss_handle = PlotXY (rlossPNL, RLOSS_PNL_GRAPH, RL_freq,
                         RL_level,RL_data_length, VAL_FLOAT, VAL_FLOAT,VAL_THIN_LINE,
                         VAL_EMPTY_SQUARE, VAL_SOLID, 1,VAL_RED);


  if ((LevelMeas[0] > RLoss_Max[0]) || 
      (LevelMeas[1] > RLoss_Max[1]) ||
      (LevelMeas[2] > RLoss_Max[2]))
    SDIRLossOk = FALSE;

  if (SDIRLossOk == FALSE) 
    {
    MessagePopup (" SDI Return Loss", "Måling   FEJLET");
    HidePanel(rlossPNL);
    return SDIRLossOk;
    }  
  RLossLevel = LevelMeas[2];
 
  // Den anden SDI udgang måles
  ResetTextBox(rlossPNL, RLOSS_PNL_TXTBOX,"");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 TEST forbindes til det andet SDI output\n\n\n");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," HUSK:  Fjern jumper PP1 efter måling!!\n\n\n");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," F4: OK \n F9: STOP\n\n");

  WaitForContinue();
 
  if (SDIRLossOk == FALSE) 
    {
    HidePanel(rlossPNL);
    return SDIRLossOk;
    }

  ResetTextBox(rlossPNL, RLOSS_PNL_MAXLEVEL,"");
  DeleteGraphPlot (rlossPNL, RLOSS_PNL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
  ProcessDrawEvents();
 
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Vent *");
  Delay(2.0);
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," *");
  Delay(2.0);
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," *");
 
  vca_returnloss_readdata();
 
  LevelMeas[0] = RL_level[(int)(CheckRLoss[0]/RL_freq_delta) - 1];
  LevelMeas[1] = RL_level[(int)(CheckRLoss[1]/RL_freq_delta) - 1];
  LevelMeas[2] = RL_level[(int)(CheckRLoss[2]/RL_freq_delta) - 1];
 
  Fmt(TxtStr,"%s< %f[w3b4p0]MHz: %f[w5b4p1]dB  (max: %f[w5b4p1])\n",CheckRLoss[0],LevelMeas[0],RLoss_Max[0]);
  SetCtrlVal(rlossPNL, RLOSS_PNL_MAXLEVEL,TxtStr);
  Fmt(TxtStr,"%s< %f[w3b4p0]MHz: %f[w5b4p1]dB  (max: %f[w5b4p1])\n",CheckRLoss[1],LevelMeas[1],RLoss_Max[1]);
  SetCtrlVal(rlossPNL, RLOSS_PNL_MAXLEVEL,TxtStr);
  Fmt(TxtStr,"%s< %f[w3b4p0]MHz: %f[w5b4p1]dB  (max: %f[w5b4p1])\n",CheckRLoss[2],LevelMeas[2],RLoss_Max[2]);
  SetCtrlVal(rlossPNL, RLOSS_PNL_MAXLEVEL,TxtStr);

 
  // plot data
  rloss_handle = PlotXY (rlossPNL, RLOSS_PNL_GRAPH, RL_freq,
                         RL_level,RL_data_length, VAL_FLOAT, VAL_FLOAT,VAL_THIN_LINE,
                         VAL_EMPTY_SQUARE, VAL_SOLID, 1,VAL_RED);

  if ((LevelMeas[0] > RLoss_Max[0]) || 
      (LevelMeas[1] > RLoss_Max[1]) ||
      (LevelMeas[2] > RLoss_Max[2]))
    SDIRLossOk = FALSE;
 

  if (LevelMeas[2] > RLossLevel)   // gem den største værdi ( = dårligste return loss)
    RLossLevel = LevelMeas[2];
 
  MessagePopup (" SDI Return Loss", " Fjern jumper PP1 ");
  Delay(1.0);  // vent på opstart af SDI-generator
 
  if (SDIRLossOk == TRUE) 
    {
    // get system date 
    dato = GetDate(1);
 
    // get system time
    tt = TimeStr();
  
    //   strread = ReadSDIKUNumber(); <<<<<<<<< apparatets ku-nummer skal bruges
    //   CopyString(SDIKUStr,0,strread,0,-1);
  
    // check om logfil findes, ellers opret en ny logfil
    fhandle = OpenFile (SDIMeasLogFile[LogDest], VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);
   
    // SDI return loss måling gemmes i logfil
    Fmt(wrtbuf,"%s<R Loss,%s,%s,KU%s,%f[b4p1],%i\n",dato,tt,SDIKUStr,RLossLevel,AppType);  
    bytes_written = WriteFile (fhandle, wrtbuf, strlen(wrtbuf));
    CloseFile(fhandle);

    Fmt(TxtStr,"%s<%f[b4p1]",RLossLevel); // return loss = -xx.x
    MessagePopup (" SDI Return Loss", "Måling   OK");
    }
  else
    {
    MessagePopup (" SDI Return Loss", "Måling   FEJLET");
    } 


  HidePanel(rlossPNL);
  return SDIRLossOk;
  } //SDIReturnLossTest


//-------------------------------------------------------------------------
int SDITotalTest(int SDINo,int TestNo)
  {
  SDIAmplSpecOk = FALSE;
  SDIJitterOk = FALSE;
  SDIRLossOk = FALSE;
  SDIEmbAudioOk = FALSE;


  if (TestNo == 1)
    {
    if (SDIJitterTest() == TRUE)
      SDIAmplSpecTest();
    } 

  if ( (TestNo == 2) || ((SDIAmplSpecOk == TRUE) && (SDIJitterOk == TRUE)) )
    SDIReturnLossTest();
              
  if ((TestNo == 3) || (SDIRLossOk == TRUE))
    CheckSDIEmbAudio_EDH();
          
  return TRUE;
  }

int SDIReturnLossMeasurement(void)
  {
  // generel måleroutine - ikke udviklet færdig
  // kræver at clock til SDI-Gennum chip GS9032 kan afbrydes
  char  TxtStr[100];
  int   handle_returned, 
        controlID_returned, 
        n, 
        rloss_handle,btn,
        PrintStatus;
float   LevelMeas[3], 
        RLossLevel;

  // init panel
  SetPanelAttribute (rlossPNL, ATTR_TITLE," Return Loss:  SDI Signal Generator");
  SetCtrlAttribute (rlossPNL, RLOSS_PNL_RLOSSOK_BTN, ATTR_VISIBLE, TRUE);
  // SetCtrlAttribute (rlossPNL, RLOSS_PNL_SAVERLOSS_BTN, ATTR_VISIBLE, FALSE);
  DeleteGraphPlot (rlossPNL, RLOSS_PNL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
  ProcessDrawEvents();
 
  ResetTextBox(rlossPNL, RLOSS_PNL_MAXLEVEL,"");

  DisplayPanel (rlossPNL);
  SDIRLossOk = TRUE;
 
  SDIRLossOk = WriteProtect(OFF,FALSE);
  if (SDIRLossOk == FALSE)
    {
    HidePanel (rlossPNL);
    return SDIRLossOk;
    }

  ResetTextBox(rlossPNL, RLOSS_PNL_TXTBOX,"");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 input forbindes til R&S VCA NOISE\n");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 output forbindes til R&S VCA B-in\n\n");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 TEST forbindes IKKE!!!\n\n");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Placer jumper PP1 på SDI generator\n\n");
 
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," F4: OK \n F9: FEJL\n\n");

  WaitForContinue();

  if (SDIRLossOk == FALSE) 
    {
    HidePanel(rlossPNL);
    return SDIRLossOk;
    }
 
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Vent *");
  vca_selectsystem(L625);
  vca_selectinput(SERIAL_B);
  vca_returnloss();
  vca_returnloss_rescale();
  vca_returnloss_Ygain(Y10Div);
  vca_returnloss_Xgain(X50Div);
  vca_returnloss_sweep(NORM);
  vca_returnloss_cursorfreq(270);
  vca_returnloss_norm(OFF);
  Delay(4);
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," *");
  vca_returnloss_norm(ON);
  Delay(4);

  // SDI udgang måles
  ResetTextBox(rlossPNL, RLOSS_PNL_TXTBOX,"");
  ResetTextBox(rlossPNL, RLOSS_PNL_MAXLEVEL,"");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 TEST forbindes til SDI output\n");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebroen SKAL forbindes direkte til BNC-stik\n\n");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," HUSK: Jumper i PP1 inden tryk på F4\n\n");
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," F4: OK \n F9: FEJL\n\n");

  WaitForContinue();

 if (SDIRLossOk == FALSE) 
   {
   HidePanel(rlossPNL);
   return SDIRLossOk;
   }
 
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Vent *");
  Delay(4);
  SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," *");
 
  vca_returnloss_readdata();
 
  LevelMeas[0] = RL_level[(int)(CheckRLoss[0]/RL_freq_delta) - 1];
  LevelMeas[1] = RL_level[(int)(CheckRLoss[1]/RL_freq_delta) - 1];
  LevelMeas[2] = RL_level[(int)(CheckRLoss[2]/RL_freq_delta) - 1];
 
  Fmt(TxtStr,"%s< %f[w3b4p0]MHz: %f[w5b4p1]dB  (max: %f[w5b4p1])\n",CheckRLoss[0],LevelMeas[0],RLoss_Max[0]);
  SetCtrlVal(rlossPNL, RLOSS_PNL_MAXLEVEL,TxtStr);
  Fmt(TxtStr,"%s< %f[w3b4p0]MHz: %f[w5b4p1]dB  (max: %f[w5b4p1])\n",CheckRLoss[1],LevelMeas[1],RLoss_Max[1]);
  SetCtrlVal(rlossPNL, RLOSS_PNL_MAXLEVEL,TxtStr);
  Fmt(TxtStr,"%s< %f[w3b4p0]MHz: %f[w5b4p1]dB  (max: %f[w5b4p1])\n",CheckRLoss[2],LevelMeas[2],RLoss_Max[2]);
  SetCtrlVal(rlossPNL, RLOSS_PNL_MAXLEVEL,TxtStr);

 
  // plot data
  rloss_handle = PlotXY (rlossPNL, RLOSS_PNL_GRAPH, RL_freq,
                         RL_level,RL_data_length, VAL_FLOAT, VAL_FLOAT,VAL_THIN_LINE,
                         VAL_EMPTY_SQUARE, VAL_SOLID, 1,VAL_RED);
        
  //SetCtrlAttribute (rlossPNL, RLOSS_PNL_SAVERLOSS_BTN, ATTR_VISIBLE, FALSE);

  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  GetUserEvent (WAIT, &handle_returned, &controlID_returned);
 
  SetPrintAttribute (ATTR_PAPER_HEIGHT, VAL_USE_PRINTER_DEFAULT);
  SetPrintAttribute (ATTR_PAPER_WIDTH, VAL_USE_PRINTER_DEFAULT);
  SetPrintAttribute (ATTR_COLOR_MODE, VAL_BW);
 
  /* if (controlID_returned == RLOSS_PNL_SAVERLOSS_BTN){
    PrintStatus = PrintCtrl (rlossPNL, RLOSS_PNL_GRAPH, "sdirloss.prn",VAL_INTEGRAL_SCALE, 1);
    MessagePopup(" SDI Return Loss"," Måledata skrevet til fil: \"sdirloss.prn\"");
   } 
 
    */

  if ((LevelMeas[0] > RLoss_Max[0]) || 
      (LevelMeas[1] > RLoss_Max[1])  ||
      (LevelMeas[2] > RLoss_Max[2]))
    SDIRLossOk = FALSE;
  //SetCtrlAttribute (rlossPNL, RLOSS_PNL_SAVERLOSS_BTN, ATTR_VISIBLE, FALSE);

  HidePanel(rlossPNL);
  return SDIRLossOk;
  } // SDIReturnLossMeasurement

//---------------------------------------------------------------------------------
int CVICALLBACK SDIExitCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) 
  {
  if (event == EVENT_COMMIT) 
    {
    SDIJitterOk = FALSE;;
    SDITimingOk = FALSE;;
    SDIAmplSpecOk = FALSE;  
    SDIRLossOk = FALSE;  
    SDIGenlOk = FALSE;
    SDIPattOk = FALSE;
    TPGPattOk = FALSE;
    SDIEmbAudioOk = FALSE;
    done = TRUE; Cont = TRUE;
    }
  return 0;
  }



int CVICALLBACK SDIOkCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) 
  {
  if (event == EVENT_COMMIT) 
    {
    done = TRUE; Cont = TRUE;;
    }    
  return 0;
  }
