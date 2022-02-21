/* FILE NAME   bbcal.c
 * PURPOSE      
 *             
 * NOTES:      001004  PF/GBG    TSG Line Phase = 0 for PAL rettet 1 linie.
 *             001010  PF      1k potm (V116) indført.
 *             Rettelser ved luminance-kalibrering af TSG
 *
 *             14.06.2002: function MakeBBPhaseCal() changed.
 *             We store now two sets of BB dac values using two new
 *             FACT funcions FACT:BBx:DACG for PAL and
 *                           FACT:BBx:DACM for NTSC
*/

#include "hp34970a.h"
#include <gpib.h>
#include <rs232.h>
#include <ansi_c.h>
#include "phase.h"
#include <utility.h>
#include <formatio.h>
#include <userint.h>
#include "cviutil.h"
#include "bb_sync.h"
#include "pt5201.h"
#include "cfg.h"
#include "sdi.h"
#include "spgcal.h"
#include "aescal.h"
#include "sditest.h"
#include "vcc.h"
#include "filter.h"
#include "def.h"




// Power supply tolerancer
const double PowerSupplyTol50 = 4,  // ± % tol for +5.0V power supply
         PowerSupplyTol33 = 4,      // ± % tol for +3.3V power supply
         PowerSupplyTol50M = 4,     // ± % tol for -5.0V power supply
       
         PowerSupplyNom50 = 5.0,    // Nominal voltage for +5.0V
         PowerSupplyNom33 = 3.3,    // Nominal voltage for +3.3V
         PowerSupplyNom50M = -5.0;  // Nominal voltage for -5.0V


// ------ BB ---------------------
const int   
    MaxBBPhasePAL = 1105919460 + 300,
    MinBBPhasePAL = 1105919460 - 300,
    MaxBBPhaseNTSC = 230631200 + 300,
    MinBBPhaseNTSC = 230631200 - 300,
    
    MaxBBScHPAL = 1320 + 50,
    MinBBScHPAL = 1320 - 50,
    MaxBBScHNTSC = 1700 + 50,
    MinBBScHNTSC = 1700 - 50,

    DefPALPhaseOffsetBB = 1105919460,
    DefPALScHOffsetBB = 1300,
    DefNTSCPhaseOffsetBB = 230631200; // half picture=230630400
    DefNTSCScHOffsetBB = 1700,

    PALModulusBB  = 1105920000, // BB generator   625*4*1728*256
    NTSCModulusBB = 461260800,    // 525*2*1716*256     resolution = 0.145ns
    ScHModulusBB = 2048,        // resolution = 0.17 deg
    
    DefGainValueBB = 130, // 0..255
    MaxGainValueBB = 255, // Color Bar generator
    MinGainValueBB = 0,

    MinChromaValueBB = 0,
    MaxChromaValueBB = 15;
    
// Sync level/offset cal data
const double 
    NomSyncLevel = -300,  // mV
    NomWindowLevelPAL = 700,  // mV
    NomWindowLevelNTSC = 714, // mV
    SyncLevelTol = 2,   // mV
    NomDCOffset = 0,      // mV
    DCOffsetTol = 3,      // mV
    MinmVStepSyncBB = 0.1,    // mV    (typisk = 0.18 mV pr step)
    MaxmVStepSyncBB = 0.4,
    MinmVStepDCOffsetBB = 0.2,   // mV     (typisk = 0.4 mV pr step)
    MaxmVStepDCOffsetBB = 1.0;


const int MaxNoOfBBMeas = 25;   // max antal målinger for TSG og BB-kalibrering




// ------ Analog Test Signal Generator (TSG) -----------------------------------
const int   
    DefPALPhaseOffsetTSG = 4318509,
    DefPALScHOffsetTSG = 118,
    
    DefNTSCPhaseOffsetTSG = 247,
    DefNTSCScHOffsetTSG = 96,

    DefChromaValuePAL = 8,  
    DefChromaValueNTSC = 8, // 0..15

    PALModulusTSG  = 4320000, // Color bar generator   625*4*1728
    NTSCModulusTSG = 1801800, // 525*2*1716     resolution = 37ns
    ScHModulusTSG = 256,      // resolution = 1.41 deg

    MaxTSGPhasePAL = 100,   // typisk = 0
    MinTSGPhasePAL =  4320000 -100,
    MaxTSGPhaseNTSC = 100,    // typisk = 0
    MinTSGPhaseNTSC =  1801800-100,
    
    MaxTSGScHPhasePAL = 128,   
    MinTSGScHPhasePAL = 108,
    MaxTSGScHPhaseNTSC = 105,  
    MinTSGScHPhaseNTSC = 85,

    MinTSGChromaValuePAL = 0,
    MaxTSGChromaValuePAL = 6;
    MinTSGChromaValueNTSC = 10,
    MaxTSGChromaValueNTSC = 15;

const double 
    MinmVStepLevelTSG = 0.15,   // mV 
    MaxmVStepLevelTSG = 5.0,

    MinAudioVCO = -2.0, // VDC  (check level for audio VCO)
    MaxAudioVCO =  1.0,

    PatternShiftDelay = 3.0;

unsigned char DACValue;       // level + offset kalibrering

// Phase & Sch cal
int       PhaseOffsetPAL,
        PhaseOffsetNTSC,
          ScHOffsetPAL,
          ScHOffsetNTSC,

        PhaseValuePAL,    // aktuel værdi ved kalibrering af color bar ganerator
        ScHValuePAL,
        PhaseValueNTSC,
        ScHValueNTSC,
        GainValue,
        ChromaValuePAL,
        ChromaValueNTSC,
        V116ValuePAL,
        V116ValueNTSC,
        PALModulus,
        NTSCModulus,
        ScHModulus;
        
        
int CheckingTCXO,
    CheckingPS,
    CheckingAudioVCO;

int TSGPhaseSelected;     // vælger mellem BB og TPG(=TSG) ved phase & ScH kalibrering

int SaveToFile;

int err,
    n,
    slideno,      // 1=phase,  2=sch,   3=gain  (Phase, ScH & Gain cal of BB and Test Signal Generator)
    ConfirmReply;

int CalPhase[2],    // phase for PAL,NTSC
    CalScHPhase[2], // ScHphase for PAL,NTSC
    CalBBDac[3];    // gain,offset,level


// *** prototyping **********************************************************
int InitPT5201(void);
int SaveCalData(int Unit);
int ShowCalFiles(void);
int ReadBBDac(int BB, int DACType, unsigned int *dac_data_read);

int CheckBBCaldata(int BBNo);
int CheckTPGPattern(void);




//*******************************************************************************
int ReadDCVolt(double *result){
ViChar datastr[20];
int st;
// The function 'hp34970a_voltageMeasure' must be executed once before using this function
st = hp34970a_cmdString_Q (hp34970, "READ?", 20, datastr);
Scan(datastr,"%s>%f",result);
return (st);
}




//*******************************************************************************
// read chroma gain value from NVRAM
void ReadChromaValue(void){
int dd[2];
 WriteCOMPort(DUTCOM,":FACT:TSG:CGAIN?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Scan(strread,"%s>%2i[x]",dd);
 ChromaValuePAL = dd[0];
 ChromaValueNTSC = dd[1];
} 


//*******************************************************************************
// read TSG gain value from NVRAM (digital potmeter V116)
void ReadV116PotmValue(void){
int dd[2];
 WriteCOMPort(DUTCOM,":FACT:TSG:AGAIN?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Scan(strread,"%s>%2i[x]",dd);
 V116ValuePAL = dd[0];
 V116ValueNTSC = dd[1];
} 





//*******************************************************************************
void SetBBDac(unsigned char dac, int BB, int DACType)
  {
  if (DACType == SYNC_DAC)
    {
    switch (BB)
      {
      case 1: Fmt(DUTStr,"%s<:FACT:V24C:COMM 52,'HD%i';",dac); // set DAC level
        break;  //BB
      case 2: Fmt(DUTStr,"%s<:FACT:V24C:COMM 52,'JD%i';",dac); 
        break;
      case 3: Fmt(DUTStr,"%s<:FACT:V24C:COMM 52,'KD%i';",dac); 
        break;
      case 4: 
        if (TV == PAL)  // Possible error. JK   !!No, it's OK!!
        //if (TV == NTSC)
          Fmt(DUTStr,":FACT:TSG:AGAIN %i,%i;",dac,V116ValueNTSC);
        else
          Fmt(DUTStr,":FACT:TSG:AGAIN %i,%i;",V116ValuePAL,dac);
        break; // Color Bar
      }  
    }  

 if (DACType == OFFSET_DAC){
  switch (BB){
    case 1: Fmt(DUTStr,"%s<:FACT:V24C:COMM 52,'HC%i';",dac); break;
    case 2: Fmt(DUTStr,"%s<:FACT:V24C:COMM 52,'JC%i';",dac); break;
    case 3: Fmt(DUTStr,"%s<:FACT:V24C:COMM 52,'KC%i';",dac); break;
  }  
 }  
 
 WriteCOMPort(DUTCOM,DUTStr);
 SetCtrlVal(syncPNL, SYNC_PNL_COMMTXT,DUTStr);
 Delay(0.8);

} // SetBBDac

//*******************************************************************************
#if 0  // the following is 5201 code taken from rs232cmd.c
void FactBBGenDAC( void)
  {
  char buffer[30];
  UI DACGain, DACOffset, DACLevel;

  if ( CmdExecute && !SCPIError)
    {
    if ( CmdRequest)
      {
      if ( ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        sprintf( buffer, "%u,%u,%u\n", Calibration.BBCalibration[Suffix[0]].DACGain, 
        Calibration.BBCalibration[Suffix[0]].DACOffset,
        Calibration.BBCalibration[Suffix[0]].DACLevel);
        rs232puts( buffer);
        }
      }
    else
      {
      if ( !PasswordProtected)
        {
        if (( ParCnt != 3) || (( sscanf( Cmd, "%u,%u,%u", &DACGain, &DACOffset, &DACLevel)) != 3))
          SCPIError = SyntaxErr;
        else
          {
          Calibration.BBCalibration[Suffix[0]].DACGain   = DACGain;
          Calibration.BBCalibration[Suffix[0]].DACOffset = DACOffset;
          Calibration.BBCalibration[Suffix[0]].DACLevel  = DACLevel;       
          NV_Store( &Calibration, CalibrationPtr, sizeof( Calibration));
          }
        }
      }
    }
  }
#endif
/* void StoreBBDac(int BB): Performs FACTORY command ":FACT:BBx:DACG(M) X,Y,Z;" */
/* where X - DACGain   (E)                                                      */
/*       Y - DACOffset (C)                                                      */
/*       Z - DACLevel  (D)                                                      */
/* which stores X,Y,Z in Calibration.BBCalibration[x].X, etc and stores         */
/* the Calibration object in NOVRAM                                             */
/* Note:                                                                        */
/* DACGain is first read and then saved, so it is unchanged here                */
/* 17.04.2002: new DAC command implemented                                      */

void StoreBBDac(int BB)
  {
  int gaindac;

  ReadBBDac(BB, GAIN_DAC, &gaindac);   // read gain dac 
 
  Fmt(DUTStr, "%s<:FACT:BB%i:DACG %i,%i,%i;", BB, gaindac, CalBBDac[1], CalBBDac[2]);
  WriteCOMPort(DUTCOM,DUTStr);

// Writes the same valuees for PAL and NTSC
  Fmt(DUTStr, "%s<:FACT:BB%i:DACM %i,%i,%i;", BB, gaindac, CalBBDac[1], CalBBDac[2]);
  WriteCOMPort(DUTCOM,DUTStr);

  SetCtrlVal(syncPNL, SYNC_PNL_COMMTXT,DUTStr);
  Delay(0.1);
  } // StoreBBDac

//************************************************************
// data read from NVRAM
int ReadBBDac(int BB, int DACType, unsigned int *dac_data_read)
  {
  unsigned int dac_values[3];

  FlushInQ(DUTCOM);
  Fmt(DUTStr,"%s<:FACT:BB%i:DACG?;",BB);
  WriteCOMPort(DUTCOM,DUTStr);
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%3i[x]",dac_values);

  if (strlen(strread) < 5)
    {
    *dac_data_read = 0;
    return FALSE;
    } 
  switch (DACType)
    {
    case GAIN_DAC:
      *dac_data_read = dac_values[0];
      break;
    case OFFSET_DAC:
      *dac_data_read = dac_values[1];
      break;
    case SYNC_DAC:
      *dac_data_read = dac_values[2];
      break; // level
    }
  return TRUE;
  } //ReadDAC




//*******************************************************************************
void SetBBGainDac(unsigned char dac)
  {
  switch (BBNo)
    {
    case 1: 
      Fmt(DUTStr,"%s<:FACT:V24C:COMM 52,'HE%i';",dac);
      break;
    case 2:
      Fmt(DUTStr,"%s<:FACT:V24C:COMM 52,'JE%i';",dac);
      break;
    case 3:
      Fmt(DUTStr,"%s<:FACT:V24C:COMM 52,'KE%i';",dac);
      break;
    }
  WriteCOMPort(DUTCOM,DUTStr);
#if DEBUG
  SetCtrlVal (phasePNL, PHASEPNL_COMM, DUTStr);  // kun ved debug  
#endif 
  }



//*******************************************************************************
int WriteBBCalDate(void){
 GetDate(2);
 Fmt(DUTStr,"%s<:FACT:BB%i:DATE %s,%s,%s;",BBNo,YearStr,MonthStr,DayStr);
 WriteCOMPort(DUTCOM,DUTStr);

 // Save last update = last calibration date
 Delay(0.03);
 Fmt(DUTStr,"%s<:FACT:MAIN:FMSD:UPD %s,%s,%s;",YearStr,MonthStr,DayStr);
 WriteCOMPort(DUTCOM,DUTStr);

return TRUE;
}



char datestr[20];

//*******************************************************************************
char *ReadBBCalDate (int BB) {
int dd[3];
  FlushInQ(DUTCOM);
  Fmt(DUTStr,"%s<:FACT:BB%i:DATE?;",BB);
  WriteCOMPort(DUTCOM,DUTStr);
  Delay(0.05);
  strread = ReadlnCOMPort(DUTCOM,0.5);
  if (strlen(strread) > 1){
   Scan(strread,"%s>%3i[x]",dd);
   Fmt(datestr,"%s<%i[w2p0]%i[w2p0]%i[w2p0]",dd[0],dd[1],dd[2]);  //yymmdd
  } 
  else
   Fmt(datestr,"%s<??????");  //yymmdd

return datestr;
}


//*******************************************************************************
int WriteTSGCalDate(void){
 GetDate(2);
 Fmt(DUTStr,"%s<:FACT:TSG:DATE %s,%s,%s;",YearStr,MonthStr,DayStr);
 WriteCOMPort(DUTCOM,DUTStr);

 // Save last update = last calibration date
 Delay(0.03);
 Fmt(DUTStr,"%s<:FACT:MAIN:FMSD:UPD %s,%s,%s;",YearStr,MonthStr,DayStr);
 WriteCOMPort(DUTCOM,DUTStr);
return TRUE;
}



char dcstr[10];
//*******************************************************************************
char *ReadTSGCalDate (void) {
int dd[3];
char *datestr;
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:TSG:DATE?;");
  Delay(0.05);
  strread = ReadlnCOMPort(DUTCOM,0.5);
  if (strlen(strread) > 1){
   Scan(strread,"%s>%3i[x]",dd);
   Fmt(dcstr,"%s<%i[w2p0]%i[w2p0]%i[w2p0]",dd[0],dd[1],dd[2]);  //yymmdd
  } 
  else
   Fmt(dcstr,"%s<??????");  //yymmdd

return dcstr;
}







//*******************************************************************************
char *ReadBBSWVer (void) {
  // read BB SW-version from BB-µP

 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:BB1:VERS?;");
 Delay(0.05);
 strread = ReadlnCOMPort(DUTCOM,0.5);
 if (strlen(strread) > 1)
  CopyString(strread,0,strread,strlen(strread)-3,3);
  else
   Fmt(strread,"%s<???");

return strread;
}



//*******************************************************************************
char *ReadMasterSWVer (void) {
// read Master uP SW ver
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:MAIN:VERS?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
return strread;
}



//*******************************************************************************
char *ReadMasterIDStr (void){
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,"*IDN?;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  if (strlen(strread) < 5)
   Fmt(strread,"%s<??????");
return strread;


}


//*******************************************************************************
char *ReadKUStr (void) {
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:MAIN:IDN:KUN?;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  if (strlen(strread) < 5)
   Fmt(strread,"%s<??????");
return strread;
}


//*******************************************************************************
int WriteFMSUpdate(void){
 GetDate(2);
 Fmt(DUTStr,"%s<:FACT:MAIN:FMSD:UPD %s,%s,%s;",YearStr,MonthStr,DayStr);
 WriteCOMPort(DUTCOM,DUTStr);
return TRUE;  
}  



//*******************************************************************************
char *ReadProdDate (void) {
// read FMS produktion date
int dd[3];
char *datestr;
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:MAIN:FMSD:PROD?;");
  Delay(0.05);
  strread = ReadlnCOMPort(DUTCOM,0.5);
  if (strlen(strread) > 1){
   Scan(strread,"%s>%3i[x]",dd);
   Fmt(datestr,"%s<%s[w2p0]%s[w2p0]%s[w2p0];",dd[0],dd[1],dd[2]); //yymmdd
  } 
  else
   Fmt(datestr,"%s<??????");  //yymmdd

 return datestr;
}



//*******************************************************************************
int SetStartupSystem5201(void){
int button_pressed;
char MsgStr[40];

 if (WriteProtect(OFF,FALSE) == FALSE){
  return FALSE;
 } 

// Read present system
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:MAIN:SYST?;");
 strread = ReadlnCOMPort(DUTCOM,0.5); 
 Fmt(MsgStr,"%s< Nuværende system: %s\n\n Vælg:",strread);    

 button_pressed = GenericMessagePopup (" System startup for PT5201",
                          MsgStr,
                          "Ingen ændring", "PAL", "NTSC",
                          0, -1, 0,
                          VAL_GENERIC_POPUP_INPUT_STRING,
                          VAL_GENERIC_POPUP_BTN1,
                          VAL_GENERIC_POPUP_BTN3);

 switch (button_pressed) 
   {
   case 1: break;
   case 2: WriteCOMPort(DUTCOM,":FACT:MAIN:SYST PAL;"); break;
   case 3: WriteCOMPort(DUTCOM,":FACT:MAIN:SYST NTSC;");break;
   } 

 WriteCOMPort(DUTCOM,":FACT:RESET;");
 Delay(0.5);
// Read present system after change
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:MAIN:SYST?;");
 strread = ReadlnCOMPort(DUTCOM,0.5); 
 Fmt(MsgStr,"%s< %s",strread);    
 MessagePopup (" System startup ", MsgStr);

return TRUE;
}




//*******************************************************************************
int InitPT5201(void){
/* Set  Company       PTV
      Type          PT5201
      KU-number     from user
      PT5201 revision from a text file, see function 'ReadTestSetup')
      Production date,  from PC-clock
*/
char KUread[10], response[10];
int p, dd[3], //yy,mm,dd
    validdate;

 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM, ":FACT:MAIN:IDN:KUN?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 p = FindPattern (strread, 0, -1, "KU", 1, 0);

 if (p == 0)
   CopyString(KUread,0,strread,p+2,6);
  else
   CopyString(KUread,0,"",0,-1);

  EnterKUNo("PT5201: ", KUread, response);
  
  Fmt(KUStr,"%s<KU%s",response);


// KU
 Fmt (DUTStr, ":FACT:MAIN:IDN:KUN '%s';",KUStr);
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.03);

// Company
 WriteCOMPort(DUTCOM, ":FACT:MAIN:IDN:COMP 'PTV';");
 Delay(0.03);

// Type
 WriteCOMPort(DUTCOM, ":FACT:MAIN:IDN:TYPE 'PT5201';");
 Delay(0.03);

// Apparatets samlede revision
// PT5201RevStr is read from file (see function 'ReadTestSetup' in cviutil.c)
 Fmt (DUTStr, ":FACT:MAIN:IDN:SWR '%s';",PT5201RevStr);
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.03);



// System start-up
 WriteCOMPort(DUTCOM,":FACT:MAIN:SYST PAL;");
 Delay(0.03);
 

// produktion date  (only first time in FMS)
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:MAIN:FMSD:PROD?;");
 Delay(0.05);
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Scan(strread,"%s>%3i[x]",dd);
 validdate = ((dd[0] < 100) && 
         (dd[1] >= 1) && (dd[1] <= 12) && 
         (dd[2] >= 1) && (dd[2] <=31)); // check year<100 and month=1-12 and day=1-31

 if (validdate == FALSE){
  GetDate(2);
  Fmt(DUTStr,"%s<:FACT:MAIN:FMSD:PROD %s,%s,%s;",YearStr,MonthStr,DayStr);
  WriteCOMPort(DUTCOM,DUTStr);
  Delay(0.03);
  WriteCOMPort(DUTCOM,":FACT:MAIN:SYST PAL;");
  Delay(0.05);
  WriteCOMPort(DUTCOM,":FACT:RESET;");
  Delay(0.5);
 } 

return TRUE;
} // InitPT5201






//-----------------------------------------------------------------
int CheckTPGCaldata(int TVsys){


return TRUE;
}




int SetTSGChroma(void){   // Color bar generator: analog switch V118
// ChromaValue kalibreres ved Phase-kalibrering (analog switch V118)
 Fmt (DUTStr, ":FACT:TSG:CGAIN %i,%i;",ChromaValuePAL,ChromaValueNTSC);
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.03);

#if DEBUG
 SetCtrlVal (phasePNL, PHASEPNL_COMM, DUTStr);  // kun ved debug  
#endif 
return TRUE;
}




//----------------------------------------------------------------
void SetTSGPhase(int ph){
// Set phase in PLD-register & NVRAM
if (TV == PAL)
//  Fmt (DUTStr, ":OUTP:TSG:DEL 0,0,%i;",ph);
  Fmt (DUTStr, ":FACT:TSG:PHAS %i,%i;",ph,PhaseValueNTSC + PhaseOffsetNTSC);
 else
  Fmt (DUTStr, ":FACT:TSG:PHAS %i,%i;",PhaseValuePAL + PhaseOffsetPAL,ph);
 
WriteCOMPort(DUTCOM, DUTStr);

#if DEBUG
 SetCtrlVal (phasePNL, PHASEPNL_COMM, DUTStr);  // kun ved debug  
#endif 
}




//***************************************************************************
void SetTSGScH(int sch){
// Set sch-phase in PLD-register & NVRAM
if (TV == PAL)
  Fmt (DUTStr, ":FACT:TSG:SCHP %i,%i;",sch,ScHValueNTSC + ScHOffsetNTSC);
 else
  Fmt (DUTStr, ":FACT:TSG:SCHP %i,%i;",ScHValuePAL + ScHOffsetPAL,sch);
 
WriteCOMPort(DUTCOM, DUTStr);

#if DEBUG
 SetCtrlVal (phasePNL, PHASEPNL_COMM, DUTStr);  // kun ved debug  
#endif 
}





//******************************************************************************
int MakeSyncOffsetCal(int CalType)
  {
  // BBNo 1-3 = Black Burst   
  // BBNo 4 = TSG

  unsigned char DACValueSend, DACdataRead;

  int     NoOfMeas, NoOfOkMeas, StartKal, DACStep,
          LevelInsideTol, LevelOk;
  double  MinmVStep, MaxmVStep, mVprStep, NomLevel,
          LevelTol, meas, m1;
  char    TitleStr[30], MsgStr[100], LevelStr[20], ErrStr[200],
          *dataread;
    
  ViChar meas34970str[31];

  int handle_returned, controlID_returned;

  if (CalType == SYNC_CAL)
    {
    if (BBNo <= 3)
      BBCalOk = TRUE;
    else
      TSGCalOk = TRUE;
    }   

  // set password = off & check RS-232 forbindelse
  if (CalType == SYNC_CAL)
    if (WriteProtect(OFF,FALSE) == FALSE)
      {
      if (BBNo <= 3)
        BBCalOk = FALSE;
      else
        TSGCalOk = FALSE;
      return FALSE;
      } 

// init panel
  if (BBNo <= 3)
    {
    Fmt(TitleStr,"%s< Black Burst #%i ",BBNo);
    DACValue = 130;          // BB startværdi for DAC
    } 
  else
    {
    Fmt(TitleStr,"%s< Test Signal Generator ");
    DACValue = 210;          // TSG startværdi for DAC
    } 

  if (BBNo <= 3)
    {
    TV = 0;
    if (CalType == SYNC_CAL)
      {
      SetVLM(50,200,20,30);      // measurements in sync bottom
      VLMdata(0,4);            // AC coupled meter
      MinmVStep = MinmVStepSyncBB;   
      MaxmVStep = MaxmVStepSyncBB;
      NomLevel = NomSyncLevel;     // NomSyncLevel is -300
      LevelTol = SyncLevelTol;
      SetCtrlAttribute (syncPNL, SYNC_PNL_LEVELBOX, ATTR_LABEL_TEXT,"Sync Level:");
      }
    else
      { // offset kalibrering
      SetVLM(50,200,400,500);      // measurements in active video line
      VLMdata(1,4);            // DC coupled meter
      MinmVStep = MinmVStepDCOffsetBB;   
      MaxmVStep = MaxmVStepDCOffsetBB;
      NomLevel = NomDCOffset;
      LevelTol = DCOffsetTol;
      SetCtrlAttribute (syncPNL, SYNC_PNL_LEVELBOX, ATTR_LABEL_TEXT,"DC Offset:");
      SetCtrlAttribute (syncPNL, SYNC_PNL_VIDEOMETER, ATTR_MAX_VALUE, 200.0);
      SetCtrlAttribute (syncPNL, SYNC_PNL_VIDEOMETER, ATTR_MIN_VALUE, -400.0);
      } 
    } 

  // TSG kalibrering
  if (BBNo == 4)
    {        
    ReadChromaValue();
    DACValue = 160;         // startværdi for DAC
    VLMdata(0,4);         // AC coupled meter
    MinmVStep = MinmVStepLevelTSG;   
    MaxmVStep = MaxmVStepLevelTSG;
    if (TV == PAL)
      {
      // Insert message box: Forbind TSG til Video_in paa Video Level Meter: JK change
      Fmt(ErrStr, "%s< TSG forbindes til Video_in paa VLM");
      MessagePopup (" Konfirmer: ", ErrStr);
      NomLevel = NomWindowLevelPAL;
      SetVLM(130,200,320,400);  // StartLine,StopLine,StartPos,StopPos
      } 
    else
      {
      NomLevel = NomWindowLevelNTSC;
      SetVLM(100,170,320,400);
      }  
    LevelTol = SyncLevelTol;
    SetCtrlAttribute (syncPNL, SYNC_PNL_LEVELBOX, ATTR_LABEL_TEXT,"Level:");
 
    SetCtrlAttribute (syncPNL, SYNC_PNL_VIDEOMETER, ATTR_MAX_VALUE, 800.0);
    SetCtrlAttribute (syncPNL, SYNC_PNL_VIDEOMETER, ATTR_MIN_VALUE, -400.0);
    } 
 
  Fmt(LevelStr,"%s<%f[p0w5] ± %f[p0] mV",NomLevel,LevelTol);
  InsertTextBoxLine(syncPNL, SYNC_PNL_LEVELBOX,0,LevelStr);
  SetPanelAttribute (syncPNL, ATTR_TITLE, TitleStr);
  DisplayPanel (syncPNL);
  SetCtrlAttribute (syncPNL, SYNC_PNL_STATUSLED, ATTR_ON_COLOR,VAL_GREEN);
#if IEEEBUS 
  hp34970a_voltageMeasure (hp34970, "101", HP34970A_VM_VTYPE_DC,
                           HP34970A_VM_VRANGE_10V,
                           HP34970A_VM_RES_6_5DIG, meas34970str);

  hp34970a_voltageSetup (hp34970, "101", HP34970A_VS_VTYPE_DC,
                         HP34970A_VS_VRANGE_10V, HP34970A_VS_RES_6_5DIG);
#endif 
  StartKal = YES;
  if (CalType == SYNC_CAL)
    {
    if (BBNo <= 3)
      StartKal = ConfirmPopup (""," Start kalibrering?");
    else
      if ((BBNo == 4) && (TV == NTSC))  // JK change
//      if ((BBNo == 4) && (TV == PAL))
        StartKal = ConfirmPopup (""," Start kalibrering?");
     }   
  if (StartKal == NO) 
    {
    HidePanel (syncPNL);
    if (BBNo <= 3)
      BBCalOk = FALSE;
    else
      TSGCalOk = FALSE;
    return FALSE;
    }

  if (CalType == SYNC_CAL)   // <<<<<<<<<error ?
    switch (BBNo) 
      {
      case 1 : WriteCOMPort(DUTCOM,":OUTP:BB1:SYST PAL;"); break;
      case 2 : WriteCOMPort(DUTCOM,":OUTP:BB2:SYST PAL;"); break;
      case 3 : WriteCOMPort(DUTCOM,":OUTP:BB3:SYST PAL;"); break;
      case 4 : 
        if (TV == PAL)
          WriteCOMPort(DUTCOM,":OUTP:TSG:SYST PAL;"); 
        else 
          WriteCOMPort(DUTCOM,":OUTP:TSG:SYST NTSC;"); 
        Delay(1.0);
        WriteCOMPort(DUTCOM,":OUTP:TSG:PATT WIN100;");
        break;
      }
  Delay(0.1);
  done = FALSE;
  LevelInsideTol = FALSE;
  NoOfOkMeas = 0;  // antal målinger indenfor tol
  NoOfMeas = 0;    // antal målinger

#if 0
  // Send internal command for BB
  if (BBNo <= 3)
    WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'X';");// Set calib. Phase G -JK
  Delay(0.05);
#endif

  // 1. måling
  SetBBDac(DACValue, BBNo, CalType); // set DAC level (130) 
  DisableBreakOnLibraryErrors();
  // read VLM voltage
  ErrIEEE = hp34970a_cmdString_Q (hp34970, "READ?", 20, meas34970str);
  Scan(meas34970str,"%s>%f",&m1);
  EnableBreakOnLibraryErrors();
  m1 = m1 * 1000 / VLMGain;    // set videometer to m1
  SetCtrlVal (syncPNL, SYNC_PNL_VIDEOMETER, m1);
 
  // 2. måling
  if (BBNo <= 3)
    DACValue -= 30;
  else 
    DACValue += 20;
   
  SetBBDac(DACValue, BBNo, CalType); // set DAC level (100)
  DisableBreakOnLibraryErrors();
  // read VLM voltage
  ErrIEEE = hp34970a_cmdString_Q (hp34970, "READ?", 20, meas34970str);
  Scan(meas34970str,"%s>%f",&meas);
  EnableBreakOnLibraryErrors();
  meas = meas * 1000 / VLMGain;   // set videometer to meas
  SetCtrlVal (syncPNL, SYNC_PNL_VIDEOMETER, meas);
 
  // beregn DAC værdi 
  if (BBNo <= 3)
    mVprStep = (meas-m1) / 30;  // calc. mV per step
  else 
    mVprStep = (meas-m1) / 20;
  if (BBNo <= 3)
    DACValue = DACValue - ((NomLevel - meas) / mVprStep);
  else
    DACValue = DACValue - ((meas - NomLevel) / mVprStep);
  //  DACValue = 170;
  Fmt(MsgStr,"%s< mV pr step: %f[p2w5] mV",mVprStep);
  SetCtrlVal (syncPNL, SYNC_PNL_STEPTXT, MsgStr);
 
  // ************* check mV pr step *********************************
  if ( (fabs(mVprStep) < MinmVStep) || (fabs(mVprStep) > MaxmVStep))
    {
    SyncLevelOk = FALSE;
    Fmt(ErrStr,"%s< mV pr DAC-Sync step udenfor tol (%f[p2w5] - %f[p2w5]):  %f[p2w5]",fabs(MinmVStep),fabs(MaxmVStep),fabs(mVprStep));
    if (BBNo <= 3)
      if (CalType == SYNC_CAL)
        Fmt(ErrStr,"%s[a]<\n Typisk værdi: 0.2mV/step\n Check modstande og lodninger");
      else
        Fmt(ErrStr,"%s[a]<\n Typisk værdi: 1.25mV/step\n Check modstande og lodninger");
  
    if (CalType == SYNC_CAL)
      MessagePopup (" Sync Level Kalibreringsfejl ",ErrStr);
    else
      MessagePopup (" Offset Kalibreringsfejl ",ErrStr);
    HidePanel (syncPNL);
    if (BBNo <= 3)
      BBCalOk = FALSE;
    else
      TSGCalOk = FALSE;
    return FALSE;
    }
 //*********** loop ******************* 
  do 
    {
    if (BBNo == 4)
      if (TV == PAL)
        V116ValuePAL = DACValue;
       else 
        V116ValueNTSC = DACValue;
        
    SetBBDac(DACValue,BBNo,CalType);
    DACValueSend = DACValue;   // ******* this value is then sent ??? JK
    GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);

    DisableBreakOnLibraryErrors();
    ErrIEEE = hp34970a_cmdString_Q (hp34970, "READ?", 20, meas34970str);
    Scan(meas34970str,"%s>%f",&meas);
    EnableBreakOnLibraryErrors();

    meas = meas * 1000 / VLMGain;
    SetCtrlVal (syncPNL, SYNC_PNL_VIDEOMETER, meas);

    if (BBNo == 4)
      {
      if (fabs(meas - NomLevel) < 2)
        DACStep = 1;
      else
        DACStep = 5;
      
      if (meas > NomLevel)
        DACValue -= DACStep;
       else
        DACValue += DACStep;
      }  
     if (BBNo <= 3)
       {
       if (CalType == SYNC_CAL)
         {
         if (meas > NomLevel)
           DACValue++;
         else
           DACValue--;
         }
       else
         {
         if (meas > NomLevel)
           DACValue--;
         else
           DACValue++;
         }
       }
     LevelInsideTol = ((meas > (NomLevel - LevelTol)) && (meas < (NomLevel + LevelTol)));
     if (LevelInsideTol)
       {
       NoOfOkMeas++;
       SetCtrlAttribute (syncPNL, SYNC_PNL_STATUSLED, ATTR_ON_COLOR,VAL_GREEN);
       }
     else 
       {
       NoOfOkMeas = 0;
       SetCtrlAttribute (syncPNL, SYNC_PNL_STATUSLED, ATTR_ON_COLOR,VAL_RED);
       }
     if ((NoOfOkMeas > 3) && (done == FALSE))   // mindst 3 OK-målinger i træk før godkendelse
       LevelOk = TRUE;
     else 
       LevelOk = FALSE;
     NoOfMeas++; 
     } while ((LevelOk == FALSE) && (done == FALSE) && (NoOfMeas <= MaxNoOfBBMeas));
//***************** loop end *****************************
     
   if (NoOfMeas > MaxNoOfBBMeas)
     {
     Fmt(ErrStr,"%s< Fejlet efter mere end %i målinger\n DAC = %i",MaxNoOfBBMeas,DACValueSend);
     if (CalType == SYNC_CAL)
       {
       MessagePopup (" Level Kalibrering",ErrStr);
       SyncLevelOk = FALSE;
       } 
     else
       {
       MessagePopup (" Offset Kalibrering",ErrStr);
       DCOffsetOk = FALSE;
       }
     HidePanel (syncPNL);
     return FALSE;
     }
   if (BBNo <= 3)
     {
     switch (CalType)
       {
       case SYNC_CAL:
         SyncLevelOk = LevelOk; 
         CalBBDac[2] = DACValueSend; // ?? JK (DACValueSend contains 130)  
         break;
      case OFFSET_CAL: 
         DCOffsetOk = LevelOk; 
         CalBBDac[1] = DACValueSend;
         break;
       } 
     if (DCOffsetOk == TRUE) //************  BREAK ****************
       StoreBBDac(BBNo);     // stores DAC values in PT5201 NOVRAM !!!!!!
     }                       //*******************************************
   if (BBNo == 4)
     {
     SyncLevelOk = LevelOk; 
     if (LevelOk == TRUE)
       WriteTSGCalDate();
     } 
   if ((SyncLevelOk == FALSE) && (CalType == SYNC_CAL))
     {
     if (BBNo <= 3)
       BBCalOk = FALSE;
     else
       TSGCalOk = FALSE;
     SetCtrlAttribute (syncPNL, SYNC_PNL_STATUSLED, ATTR_ON_COLOR,VAL_RED);
     MessagePopup ("Level Kalibrering", "Kalibrering   AFVIST ");
     }
   if ((DCOffsetOk == FALSE)  && (CalType == OFFSET_CAL))
     {
     if (BBNo <= 3)
       BBCalOk = FALSE;
     else
       TSGCalOk = FALSE;
     SetCtrlAttribute (syncPNL, SYNC_PNL_STATUSLED, ATTR_ON_COLOR,VAL_RED);
     MessagePopup ("DC Offset Kalibrering", "Kalibrering   AFVIST ");
     }
   HidePanel (syncPNL);
   if (CalType == SYNC_CAL)
     return SyncLevelOk;
   else  
     return DCOffsetOk;
   } // MakeSyncOffsetCal




//-------- BB -----------------------------------------------------------------------
void SetPhase(int ph)
  {
  if (TV == PAL)
    {
    switch (BBNo)
      {
      case 1: Fmt (DUTStr, ":FACT:V24C:COMM 52,'HK%i';",ph); break;
      case 2: Fmt (DUTStr, ":FACT:V24C:COMM 52,'JK%i';",ph); break;
      case 3: Fmt (DUTStr, ":FACT:V24C:COMM 52,'KK%i';",ph); break;
      } 
    } 
  else
    {
    switch (BBNo)
      {
      case 1: Fmt (DUTStr, ":FACT:V24C:COMM 52,'HM%i';",ph); break;
      case 2: Fmt (DUTStr, ":FACT:V24C:COMM 52,'JM%i';",ph); break;
      case 3: Fmt (DUTStr, ":FACT:V24C:COMM 52,'KM%i';",ph); break;
      } 
    } 
  WriteCOMPort(DUTCOM,DUTStr);
  Delay(0.1);
#if DEBUG
  SetCtrlVal (phasePNL, PHASEPNL_COMM, DUTStr);  // kun ved debug  
#endif 
//  Fmt (DUTStr, ":OUTP:BB%i:DEL 0,0,%i;",BBNo,ph);
  }



//------- BB ------------------------------------------------------------------------
void SetScH(int sch)
  {
  switch (BBNo)
    {
    case 1: Fmt (DUTStr, ":FACT:V24C:COMM 52,'HH%i';",sch); break;
    case 2: Fmt (DUTStr, ":FACT:V24C:COMM 52,'JH%i';",sch); break;
    case 3: Fmt (DUTStr, ":FACT:V24C:COMM 52,'KH%i';",sch); break;
    } 
  WriteCOMPort(DUTCOM,DUTStr);
  Delay(0.1);
#if DEBUG 
  SetCtrlVal (phasePNL, PHASEPNL_COMM, DUTStr);  // kun ved debug  
#endif 
}

#if 0
// -------  read PAL/NTSC cal data and check values  -------------------------------
int CheckBBCaldata(int BB){ // function er ikke færdig for PT5201
char BBKUStr[20],
   MasterSW[20],
   BBSW[20],
   MsgStr[200],
   wrtbuf[150],
   *tt;         // HH:MM:SS
int fhandle,
   bytes_written,
   phaseflag,
   schflag,
   dacflag;


 phaseflag = TRUE;
 schflag = TRUE;
 dacflag = TRUE;
 
 // phase PAL,NTSC
 Fmt(DUTStr,"%s<:FACT:BB%i:PHAS?;",BBNo);
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,DUTStr);
 strread = ReadlnCOMPort(DUTCOM,0.5);
 if (strlen(strread) > 3)
   Scan(strread,"%s>%2i[x]",CalPhase);
  else 
   phaseflag = FALSE;
 
 if (phaseflag == FALSE)
  if ((CalPhase[0] < MinBBPhasePAL) || (CalPhase[0] > MaxBBPhasePAL) || 
      (CalPhase[1] < MinBBPhaseNTSC) || (CalPhase[1] > MaxBBPhaseNTSC)){
    Fmt(MsgStr," Kalibreringsværdi:\n PhasePAL = %i   (%i - %i)  \n PhaseNTSC = %i     (%i - %i)\n Fejlen kan opstå, hvis der tastes OK når kalibreringen IKKE er ok\n Gennemfør en ny kalibrering",
    CalPhase[0],MinBBPhasePAL,MaxBBPhasePAL,CalPhase[1],MinBBPhaseNTSC,MaxBBPhaseNTSC);
    MessagePopup(" Kalibrering af Phase afvist",MsgStr);
    phaseflag = FALSE;
  }
      
      
 // sch-phase PAL,NTSC
 Fmt(DUTStr,"%s<:FACT:BB%i:SCHP?;",BBNo);
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,DUTStr);
 strread = ReadlnCOMPort(DUTCOM,0.5);
 if (strlen(strread) > 3)
   Scan(strread,"%s>%2i[x]",CalScHPhase);
  else 
   schflag = FALSE;
 
 if (schflag == FALSE)
  if ((CalScHPhase[0] < MinBBScHPAL) || (CalScHPhase[0] > MaxBBScHPAL) || 
      (CalScHPhase[1] < MinBBScHNTSC) || (CalScHPhase[1] > MaxBBScHNTSC))
    {
    Fmt(MsgStr," Kalibreringsværdi:\n ScH-PAL = %i   (%i - %i)  \n Sch-NTSC = %i     (%i - %i)\n Fejlen kan opstå, hvis der tastes OK når kalibreringen IKKE er ok\n Gennemfør en ny kalibrering",
    CalScHPhase[0],MinBBScHPAL,MaxBBScHPAL,CalScHPhase[1],MinBBScHNTSC,MaxBBScHNTSC);
    MessagePopup(" Kalibrering af ScH afvist",MsgStr);
    schflag = FALSE;
    }
      
      
 // read BB gain,offset,sync DAC
 FlushInQ(DUTCOM);
 Fmt(DUTStr,":FACT:BB%i:DAC?;",BB);
 WriteCOMPort(DUTCOM,DUTStr);
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Scan(strread,"%s>%3i[x]",CalBBDac);
 // get system date 
 dato = GetDate(1);
 // get system time
 tt = TimeStr();
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:MAIN:IDN:KUN?;");
 strread = ReadlnCOMPort(DUTCOM,0.5); 
 CopyString(BBKUStr,0,strread,0,-1);
    

       
     // check om logfil findes, ellers opret en ny logfil
     fhandle = OpenFile (BBLogFile[LogDest], VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);
   
     strread = ReadBBSWVer();
     CopyString(BBSW,0,strread,0,-1);
      
     strread = ReadMasterSWVer();
     CopyString(MasterSW,0,strread,0,-1);



  // BB cal data + BB-SW + master SW gemmes i logfil
      Fmt(wrtbuf,"%s<%s,%s,KU%s,%i,%i,%i,%i,%i,%i,%i,%i,BB-SW=%s,Master-SW=%s\n",
      dato,tt,BBKUStr,BB,CalBBDac[2],CalBBDac[1],CalBBDac[0],CalPhase[0],CalPhase[1],CalScHPhase[0],CalScHPhase[1],BBSW,MasterSW);  
      bytes_written = WriteFile (fhandle, wrtbuf, strlen(wrtbuf));
      CloseFile(fhandle);

         
    MessagePopup(" Kalibrering af Phase og ScH afvist"," Ingen kalibreringsværdier læst fra NVRAM");
    return FALSE;
   
} //CheckBBCaldata
#endif



//-------------------------------------------------------------------------------
int MakeBBPhaseCal(int TVSys) 
  {
  /* 
   Ved start af kalibrering, resettes registre i BB generator for både PAL og NTSC.
   Det betyder, at både PAL og NTSC skal kalibreres; også selv om kun det ene af 
   systemerne skal kalibreres.
   */
  char TitleStr[50];
  int handle_returned, controlID_returned;

  TSGPhaseSelected = FALSE;
 
  PALModulus = PALModulusBB;
  NTSCModulus = NTSCModulusBB;
  ScHModulus = ScHModulusBB;
  // set password = off & check RS-232 forbindelse
  if (WriteProtect(OFF,FALSE) == FALSE){
   return FALSE;
  } 
 GainValue = DefGainValueBB;
 slideno = 2;     // selects sch-slide at start of calibration

 TV = TVSys;    // TV is a global variable

 done = FALSE;
 PhaseScHGainOk == FALSE;
 
 DisplayPanel (phasePNL);

 SetCtrlAttribute (phasePNL, PHASEPNL_PHASEPILOP_BTN, ATTR_DIMMED,TRUE);
 SetCtrlAttribute (phasePNL, PHASEPNL_PHASEPILNED_BTN, ATTR_DIMMED,TRUE);
 SetCtrlAttribute (phasePNL, PHASEPNL_SCHPILOP_BTN, ATTR_DIMMED,FALSE);
 SetCtrlAttribute (phasePNL, PHASEPNL_SCHPILNED_BTN, ATTR_DIMMED,FALSE);
 SetCtrlAttribute (phasePNL, PHASEPNL_GAINPILOP_BTN, ATTR_DIMMED,TRUE);
 SetCtrlAttribute (phasePNL, PHASEPNL_GAINPILNED_BTN, ATTR_DIMMED,TRUE);

 SetCtrlAttribute (phasePNL, PHASEPNL_SCH_DEC, ATTR_FRAME_COLOR,VAL_BLUE);
 SetCtrlAttribute (phasePNL, PHASEPNL_PHASE_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);
 SetCtrlAttribute (phasePNL, PHASEPNL_GAIN_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);

 SetCtrlAttribute (phasePNL, PHASEPNL_GAINSLIDE, ATTR_TICK_STYLE,VAL_NO_MINOR_TICKS);
 
 if (TVSys == PAL) {
  PhaseValuePAL = 0;
  SetCtrlAttribute (phasePNL, PHASEPNL_PHASESLIDE, ATTR_MAX_VALUE, PhaseValuePAL + 2000);
  SetCtrlAttribute (phasePNL, PHASEPNL_PHASESLIDE, ATTR_MIN_VALUE, PhaseValuePAL - 2000);
  SetCtrlVal (phasePNL, PHASEPNL_PHASESLIDE, PhaseValuePAL);
  ScHValuePAL = 0;
  SetCtrlAttribute (phasePNL, PHASEPNL_SCHSLIDE, ATTR_MAX_VALUE, ScHValuePAL + 100);
  SetCtrlAttribute (phasePNL, PHASEPNL_SCHSLIDE, ATTR_MIN_VALUE, ScHValuePAL - 100);
  SetCtrlVal (phasePNL, PHASEPNL_SCHSLIDE, ScHValuePAL);
  ScHOffsetPAL = DefPALScHOffsetBB;
  PhaseOffsetPAL = DefPALPhaseOffsetBB;
  Fmt(TitleStr,"%s< PAL:  Fase, ScH og burst amplitude for BB%i ",BBNo);
 } 
 else {
  PhaseValueNTSC = 0;
  SetCtrlAttribute (phasePNL, PHASEPNL_PHASESLIDE, ATTR_MAX_VALUE, PhaseValueNTSC + 2000);
  SetCtrlAttribute (phasePNL, PHASEPNL_PHASESLIDE, ATTR_MIN_VALUE, PhaseValueNTSC - 2000);
  SetCtrlVal (phasePNL, PHASEPNL_PHASESLIDE, PhaseValueNTSC);
  ScHValueNTSC = 0;
  SetCtrlAttribute (phasePNL, PHASEPNL_SCHSLIDE, ATTR_MAX_VALUE, ScHValueNTSC + 100);
  SetCtrlAttribute (phasePNL, PHASEPNL_SCHSLIDE, ATTR_MIN_VALUE, ScHValueNTSC - 100);
  SetCtrlVal (phasePNL, PHASEPNL_SCHSLIDE, ScHValueNTSC);
  ScHOffsetNTSC = DefNTSCScHOffsetBB;
  PhaseOffsetNTSC = DefNTSCPhaseOffsetBB;
  Fmt(TitleStr,"%s< NTSC:  Fase, ScH og burst amplitude for BB%i ",BBNo);
 }
 
 SetCtrlAttribute (phasePNL, PHASEPNL_GAINSLIDE, ATTR_MIN_VALUE, MinGainValueBB);
 SetCtrlAttribute (phasePNL, PHASEPNL_GAINSLIDE, ATTR_MAX_VALUE, MaxGainValueBB);
 SetCtrlVal (phasePNL, PHASEPNL_GAINSLIDE, GainValue);
 SetPanelAttribute (phasePNL, ATTR_TITLE, TitleStr);
 

 ResetTextBox(phasePNL, PHASEPNL_PHASESCHTXTBOX,"");
 if (TV == PAL) {
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Forbind PT5210 BB til DUT GENLOCK A\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Forbind DUT GENLOCK B   til 5662 CH-A\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Forbind DUT BB          til 5662 CH-B\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," NB: Kabler fra DUT til 5662 SKAL være lige lange!!\n\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Indstil 5662G: VECT B        INT REF \n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Indstil 5662M: WFM  A-B  2H  INT REF  X-MAG\n\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," ScH aflæses på 5662G      Fase aflæses på 5662M\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX,"\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Ved fejl: Tryk F9           ");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Ved OK:   Tryk F4");
  SetPM5662(PAL);
 }
 else {
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Forbind PT5210 BB til DUT GENLOCK A\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Forbind DUT GENLOCK B   til 5662 CH-A\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Forbind DUT BB          til 5662 CH-B\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," NB: Kabler fra DUT til 5662 SKAL være lige lange!!\n\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Indstil 5662G: WFM  A-B  2H  INT REF  X-MAG\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Indstil 5662M: VECT B        INT REF \n\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," ScH aflæses på 5662M      Fase aflæses på 5662G\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX,"\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Ved fejl: Tryk F9           ");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Ved OK:   Tryk F4");
  SetPM5662(NTSC);
 }

  if (TV == PAL)
    {
    WriteCOMPort(PT5210COM,":OUTP:BB1:SYST PAL_ID;SCHP 0;");
    WriteCOMPort(DUTCOM,":INP:GENL:SYST PALB;");
    Delay(0.01);
    switch (BBNo)
      {
      case 1:
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HK0';");Delay(0.01);// h-phase
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HX0';");Delay(0.01);
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HT0';");Delay(0.01);// sch-phase
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HH0';");
        break;
      case 2:
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'JK0';");Delay(0.01);// h-phase
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'JX0';");Delay(0.01);
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'JT0';");Delay(0.01);// sch-phase
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'JH0';");
        break;
      case 3:
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'KK0';");Delay(0.01);// h-phase
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'KX0';");Delay(0.01);
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'KT0';");Delay(0.01);// sch-phase
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'KH0';");
        break;
      }
     }
   else
     { // NTSC
     WriteCOMPort(DUTCOM,":INP:GENL:SYST NTSC;");
     WriteCOMPort(PT5210COM,":OUTP:BB1:SYST NTSC;DEL 0,0,0;SCHP 0;");
     Delay(0.01);
     switch (BBNo)
       {
       case 1:  
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HY0';");Delay(0.01);// h-phase
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HM0';");Delay(0.01);
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HU0';");Delay(0.01);// sch-phase
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HH0';");
        break;
      case 2:  
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'JY0';");Delay(0.01);// h-phase
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'JM0';");Delay(0.01);
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'JU0';");Delay(0.01);// sch-phase
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'JH0';");
        break;
      case 3:
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'KY0';");Delay(0.01);// h-phase
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'KM0';");Delay(0.01);
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'KU0';");Delay(0.01);// sch-phase
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'KH0';");
        break;
      }
    } 
  
 Delay(0.01); 
  
#if 0
 // Setup for internal commands for BB
 // X Command requires parameter (ul), it means Set Calib Phase G ?????????? JK
 WriteCOMPort(DUTCOM, ":FACT:V24C:COMM 52,'X';");
#endif

 if (TV == PAL) 
   {
   SetScH(ScHValuePAL + ScHOffsetPAL); //"COMM 52,'HH%i';",sch)
   //  SetPhase(PhaseOffsetPAL = 1105919550); //ok
   SetPhase(PhaseValuePAL + PhaseOffsetPAL);
   }
 else
   {
   SetScH(ScHValueNTSC + ScHOffsetNTSC);
   //  SetPhase(PhaseOffsetNTSC = 230631240);  //ok
   SetPhase(PhaseValueNTSC + PhaseOffsetNTSC);
   } 
 //************* BREAK **********************
 SetBBGainDac(GainValue); // "%s<COMM 52,'HE%i';"  ** Not stored in NOVRAM

 // her ventes indtil brugeren har tastet OK=F4 eller FEJL=F9
 while (done == FALSE) 
   {
   GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   }

 if (PhaseScHGainOk == FALSE)
   {
   switch (TV)
     {
     case PAL  :
       MessagePopup (" Phase, ScH og burst amplitude kalibrering", " FEJLET"); 
       break;
     case NTSC : MessagePopup (" Phase & ScH kalibrering", " FEJLET"); 
       break;
     }
   BBCalOk = FALSE; 
   }  
 // save phase and schphase cal data for PAL & NTSC - save cal date
 if ((PhaseScHGainOk == TRUE) && (TV == PAL)) 
   {                          // JK
   Fmt(DUTStr,"%s<:FACT:BB%i:DACG %i,%i,%i;",BBNo, GainValue, CalBBDac[1], CalBBDac[2]);
   WriteCOMPort(DUTCOM,DUTStr);
   }
 if ((PhaseScHGainOk == TRUE) && (TV == NTSC))
   {
   Fmt(DUTStr,"%s<:FACT:BB%i:PHAS %i,%i;",BBNo,PhaseValuePAL + PhaseOffsetPAL,PhaseValueNTSC + PhaseOffsetNTSC);
   WriteCOMPort(DUTCOM,DUTStr);
   Delay(0.05);
   Fmt(DUTStr,"%s<:FACT:BB%i:SCHP %i,%i;",BBNo,ScHValuePAL + ScHOffsetPAL,ScHValueNTSC + ScHOffsetNTSC);
   WriteCOMPort(DUTCOM,DUTStr);
   Delay(0.05);
   // 
   // 
   Fmt(DUTStr,"%s<:FACT:BB%i:DACM %i,%i,%i;",BBNo, GainValue, CalBBDac[1], CalBBDac[2]);
   WriteCOMPort(DUTCOM,DUTStr);
   WriteBBCalDate();
   }

  //PhaseScHGainOk = CheckBBCaldata(BBNo,TV); // check and save caldata & SW ver <<<< new for PT5201
 
 Fmt(DUTStr,"%s<:OUTP:BB%i:SYST NTSC;",BBNo);
 WriteCOMPort(DUTCOM,DUTStr);

 HidePanel (phasePNL);
 
 return PhaseScHGainOk;
 } // MakeBBPhaseCal





//************************************************************************************
int MakeTSGPhaseCal(int TVSys) {

char TitleStr[60];
int handle_returned, controlID_returned;
int ga[2];


#if DEBUG 
 SetCtrlAttribute (phasePNL, PHASEPNL_COMM, ATTR_VISIBLE, TRUE);  // kun ved debug
#else 
 SetCtrlAttribute (phasePNL, PHASEPNL_COMM, ATTR_VISIBLE, FALSE);
#endif 

 BBNo = 4;    // BB=1,2,3   Color bar gen = 4
 TV = TVSys;
 
 TSGPhaseSelected = TRUE;
 
 if (TVSys == PAL)
   ChromaValuePAL = DefChromaValuePAL;
  else
   ChromaValueNTSC = DefChromaValueNTSC;
 
 PALModulus  = PALModulusTSG;
 NTSCModulus = NTSCModulusTSG;
 ScHModulus  = ScHModulusTSG;


 if (TVSys == PAL)
  Fmt(TitleStr,"%s< PAL:  Fase, ScH for Test Signal Generator ");
 else
  Fmt(TitleStr,"%s< NTSC:  Fase, ScH for Test Signal Generator ");

 SetPanelAttribute (phasePNL, ATTR_TITLE, TitleStr);


 slideno = 2;
 done = FALSE;
 PhaseScHGainOk == FALSE;
 
 
 // set password = off & check RS-232 forbindelse
 if (WriteProtect(OFF,FALSE) == FALSE){
   PhaseScHGainOk = FALSE;
   return FALSE;
 } 

 DisplayPanel (phasePNL);

 
 SetCtrlAttribute (phasePNL, PHASEPNL_SCHPILOP_BTN, ATTR_DIMMED,FALSE);
 SetCtrlAttribute (phasePNL, PHASEPNL_SCHPILNED_BTN, ATTR_DIMMED,FALSE);
 SetCtrlAttribute (phasePNL, PHASEPNL_PHASEPILOP_BTN, ATTR_DIMMED,TRUE);
 SetCtrlAttribute (phasePNL, PHASEPNL_PHASEPILNED_BTN, ATTR_DIMMED,TRUE);
 SetCtrlAttribute (phasePNL, PHASEPNL_GAINPILOP_BTN, ATTR_DIMMED,TRUE);
 SetCtrlAttribute (phasePNL, PHASEPNL_GAINPILNED_BTN, ATTR_DIMMED,TRUE);
 SetCtrlAttribute (phasePNL, PHASEPNL_PHASE_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);
 SetCtrlAttribute (phasePNL, PHASEPNL_SCH_DEC, ATTR_FRAME_COLOR,VAL_BLUE);
 SetCtrlAttribute (phasePNL, PHASEPNL_GAIN_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);
 

 if (TVSys == PAL) {
  PhaseValuePAL = 0;
  SetCtrlAttribute (phasePNL, PHASEPNL_PHASESLIDE, ATTR_MAX_VALUE, PhaseValuePAL + 100);
  SetCtrlAttribute (phasePNL, PHASEPNL_PHASESLIDE, ATTR_MIN_VALUE, PhaseValuePAL - 100);
  SetCtrlVal (phasePNL, PHASEPNL_PHASESLIDE, PhaseValuePAL);
  SetCtrlVal (phasePNL, PHASEPNL_GAINSLIDE, DefChromaValuePAL);
  ScHValuePAL = 0;
  SetCtrlAttribute (phasePNL, PHASEPNL_SCHSLIDE, ATTR_MAX_VALUE, ScHValuePAL + 40);
  SetCtrlAttribute (phasePNL, PHASEPNL_SCHSLIDE, ATTR_MIN_VALUE, ScHValuePAL - 40);
  SetCtrlVal (phasePNL, PHASEPNL_SCHSLIDE, ScHValuePAL);

  ScHOffsetPAL = DefPALScHOffsetTSG;
  PhaseOffsetPAL = DefPALPhaseOffsetTSG;
  ChromaValuePAL = DefChromaValuePAL;
 } 
 else {
  PhaseValueNTSC = 0;
  SetCtrlAttribute (phasePNL, PHASEPNL_PHASESLIDE, ATTR_MAX_VALUE, PhaseValueNTSC + 100);
  SetCtrlAttribute (phasePNL, PHASEPNL_PHASESLIDE, ATTR_MIN_VALUE, PhaseValueNTSC - 100);
  SetCtrlVal (phasePNL, PHASEPNL_PHASESLIDE, PhaseValueNTSC);
  SetCtrlVal (phasePNL, PHASEPNL_GAINSLIDE, DefChromaValueNTSC);
  ScHValueNTSC = 0;
  SetCtrlAttribute (phasePNL, PHASEPNL_SCHSLIDE, ATTR_MAX_VALUE, ScHValueNTSC + 40);
  SetCtrlAttribute (phasePNL, PHASEPNL_SCHSLIDE, ATTR_MIN_VALUE, ScHValueNTSC - 40);
  SetCtrlVal (phasePNL, PHASEPNL_SCHSLIDE, ScHValueNTSC);
  
  ScHOffsetNTSC = DefNTSCScHOffsetTSG;
  PhaseOffsetNTSC = DefNTSCPhaseOffsetTSG;
  ChromaValueNTSC = DefChromaValueNTSC;
 }
 
  SetCtrlAttribute (phasePNL, PHASEPNL_GAINSLIDE, ATTR_MIN_VALUE, 0);
  SetCtrlAttribute (phasePNL, PHASEPNL_GAINSLIDE, ATTR_MAX_VALUE, 15);
  SetCtrlAttribute (phasePNL, PHASEPNL_GAINSLIDE, ATTR_TICK_STYLE,VAL_NO_MINOR_TICKS);
 
 

 ResetTextBox(phasePNL, PHASEPNL_PHASESCHTXTBOX,"");
 if (TV == PAL) {
  WriteCOMPort(PT5210COM,":OUTP:BB1:SYST PAL_ID;SCHP 0;");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Forbind PT5210 BB til DUT GENLOCK A\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Forbind DUT GENLOCK B   til 5662 CH-A\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Forbind DUT COLOR BAR   til 5662 CH-B\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," NB: Kabler fra DUT til 5662 SKAL være lige lange!!\n\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Indstil 5662G: VECT B        INT REF \n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Indstil 5662M: WFM  A-B  2H  INT REF  X-MAG\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," ScH aflæses på 5662G      Linie Fase aflæses på 5662M\n\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Chrominance gain justres ved hjælp af vectorscope!!\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Set vectorscope gain til CAL!!\n\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Ved fejl: Tryk F9           Ved OK:   Tryk F4");
  SetPM5662(PAL);
 }
 else if (TV == NTSC) {
  WriteCOMPort(PT5210COM,":OUTP:BB1:SYST NTSC;SCHP 0;");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Forbind PT5210 BB til DUT GENLOCK A\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Forbind DUT GENLOCK B   til 5662 CH-A\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Forbind DUT COLOR BAR   til 5662 CH-B\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," NB: Kabler fra DUT til 5662 SKAL være lige lange!!\n\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Indstil 5662G: WFM  A-B  2H  INT REF  X-MAG\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Indstil 5662M: VECT B        INT REF \n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," ScH aflæses på 5662M      Linie Fase aflæses på 5662G\n\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Chrominance gain justres ved hjælp af vectorscope!!\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Set vectorscope gain til CAL!!\n\n");
  SetCtrlVal(phasePNL, PHASEPNL_PHASESCHTXTBOX," Ved fejl: Tryk F9           Ved OK:   Tryk F4");
  SetPM5662(NTSC);
 }

 
 
 if (TV == PAL){
   WriteCOMPort(DUTCOM,":INP:GENL:SYST PALB;");
   Delay(0.8);
   WriteCOMPort(DUTCOM,":INP:GENL:DEL 0,0,0;");
   Delay(0.8);
   WriteCOMPort(DUTCOM,":OUTP:TSG:SYST PAL;");
   Delay(0.8);
   WriteCOMPort(DUTCOM,":OUTP:TSG:PATT CBEBU;");
   //Fmt (DUTStr, ":FACT:MMCH %i,%i;",0x5F,0x3D);WriteCOMPort(DUTCOM, DUTStr);
   Delay(0.8);
   SetTSGPhase(PhaseValuePAL + PhaseOffsetPAL);
   Delay(0.8);
   SetTSGScH(ScHValuePAL + ScHOffsetPAL);
  }  
  else{
   WriteCOMPort(DUTCOM,":INP:GENL:SYST NTSC;"); 
   Delay(0.8);
   WriteCOMPort(DUTCOM,":INP:GENL:DEL 0,0,0;");
   Delay(0.8);
   WriteCOMPort(DUTCOM,":OUTP:TSG:SYST NTSC;");
   Delay(0.8);
   WriteCOMPort(DUTCOM,":OUTP:TSG:PATT CBSMPTE;");
   Delay(0.8);
   SetTSGPhase(PhaseValueNTSC + PhaseOffsetNTSC);
   Delay(0.8);
   SetTSGScH(ScHValueNTSC + ScHOffsetNTSC);
 }
   

   Delay(0.8);
   SetTSGChroma();

 // her ventes indtil brugeren har tastet OK=F4 eller FEJL=F9
 while (done == FALSE) {
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 }


 
 if (PhaseScHGainOk == TRUE) {
  PhaseScHGainOk = CheckTPGCaldata(TV);     // check data in NVRAM
  Delay(0.8);
  WriteTSGCalDate();

 } //if



 HidePanel (phasePNL);
 

 return PhaseScHGainOk;
} // MakeTPGPhaseCal



void ChkPatt(void)
  { 
  int handle_returned, controlID_returned;
  done = FALSE;
  // ----- Check pattern -----------------------------------
  while (done == FALSE)
    {
    GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);

    WriteCOMPort(DUTCOM,":OUTP:TSG:SYST PAL;");
    Delay(PatternShiftDelay);

    WriteCOMPort(DUTCOM,":OUTP:TSG:PATT CBEB;");
    Delay(PatternShiftDelay);
    GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
    if (done == TRUE) break;
    WriteCOMPort(DUTCOM,":OUTP:TSG:PATT CBGREY75;");
    Delay(PatternShiftDelay);
    GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
    if (done == TRUE) break;
    WriteCOMPort(DUTCOM,":OUTP:TSG:PATT WIN100;");
    Delay(PatternShiftDelay);
    GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
    if (done == TRUE) break;

    WriteCOMPort(DUTCOM,":OUTP:TSG:SYST NTSC;");
    Delay(PatternShiftDelay);


    WriteCOMPort(DUTCOM,":OUTP:TSG:PATT CBSM;");
    Delay(PatternShiftDelay);
    GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
    if (done == TRUE) break;
    WriteCOMPort(DUTCOM,":OUTP:TSG:PATT MULT;");
    Delay(PatternShiftDelay);
    GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
    if (done == TRUE) break;
    WriteCOMPort(DUTCOM,":OUTP:TSG:PATT PLUG;");
    Delay(PatternShiftDelay);
    } // while
  }


// ------ Check TPG Pattern ---------------------------------------------------
int CheckTPGPattern(void)
  {
  char *usertxt;

  TPGPattOk = TRUE;
  ResetTextBox(pattPNL, PATT_PNL_TXTBOX,"");

  TPGPattOk = WriteProtect(OFF,FALSE);
  if (TPGPattOk == FALSE)
    {
    HidePanel (pattPNL);
    return TPGPattOk;
    }
  Delay(0.1);
 

  WriteCOMPort(DUTCOM,":OUTP:TSG:SYST PAL;");
  Delay(PatternShiftDelay);
  WriteCOMPort(DUTCOM,":OUTP:TSG:PATT CBEB;");  // FAIL HERE
  Delay(PatternShiftDelay);
  
  SetPM5662(PAL);
  SetPanelAttribute (pattPNL, ATTR_TITLE, " PT5201 Pattern Test ");
  
  // check analog Color Bar output  
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Forbind DUT's TSG til PM5662 CH-B\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Fjern kabel til D1DAC-IN\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Indstil PM5662G INPUT = CH-B\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Barco monitor indstilles til composite: Tast 'INP1'\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Tast F4: OK\n F9: STOP\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," På monitor vises derefter følgende pattern i rækkefølge:\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," PAL:   EBU Colorbar     Colorbar+Grey    Window 100%\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," NTSC:  SMPTE Colorbar   Multi Burst      Pluge\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Kontroller at alle pattern vises korrekt på monitoren\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Tast F4: OK\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Tast F9: FEJL\n");
  DisplayPanel (pattPNL);
  
  WaitForContinue();
  if (TPGPattOk == FALSE)
    {
    //MessagePopup (" Pattern Test", " AFBRUDT");
    HidePanel(pattPNL);
    return FALSE;
    }
 
  ChkPatt();

  if (TPGPattOk == FALSE)
    {
    //MessagePopup (" Pattern Test", " AFBRUDT");
    HidePanel(pattPNL);
    return FALSE;
    }

  // check SDI output  
  ResetTextBox(pattPNL, PATT_PNL_TXTBOX,"");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Forbind SDI til D1DAC-IN\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Fjern kabel til PM5662 CH-B\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Barco monitor indstilles til component: Tast 'COMP'\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Tast F4: OK\n F9: STOP\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," På monitor vises derefter følgende pattern i rækkefølge:\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," PAL:   EBU Colorbar     Colorbar+Grey    Window 100%\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," NTSC:  SMPTE Colorbar   Multi Burst      Pluge\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Kontroller at alle pattern vises korrekt på monitoren\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Tast F4: OK\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Tast F9: FEJL\n");
  
  WaitForContinue ();
  if (TPGPattOk == FALSE)
    {
    //MessagePopup (" Pattern Test", " AFBRUDT");
    HidePanel(pattPNL);
    return FALSE;
    }
  
  ChkPatt();
  
  WriteCOMPort(DUTCOM,":OUTP:TSG:SYST PAL;");
  Delay(PatternShiftDelay);
  WriteCOMPort(DUTCOM,":OUTP:TSG:PATT CBEB;");  // FAIL HERE
  Delay(PatternShiftDelay);
  
  if (TPGPattOk == FALSE)
    {
    //MessagePopup (" Pattern Test", " FEJLET");
    HidePanel(pattPNL);
    return FALSE;
    }
    
  //MessagePopup (" Pattern Test", " OK");
  HidePanel(pattPNL);
  
  return TRUE;
  } //CheckTPGPattern





//----------------------------------------------------------------------------
int CheckPowerSupply(int NomVolt){
/* 
Check of    NomVolt
+3.3V      33
+5V      50
-5V     -50
Results not saved in logfile
*/ 
ViChar datastr[20];
float voltmeas;
double  VMax,
      VMin;
int handle_returned,
    controlID_returned;

char MsgTxt[50];
 
 CheckingPS = TRUE;
 CheckingAudioVCO = FALSE;
 CheckingTCXO = FALSE;

 ResetTextBox (vccPNL, VCC_PNL_TXTBOX, "");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, "\n");

 switch (NomVolt){
  case 50  : SetPanelAttribute (vccPNL, ATTR_TITLE, " +5V Power Supply Check "); 
         VMax = PowerSupplyNom50 * (1 + PowerSupplyTol50/100);
         VMin = PowerSupplyNom50 * (1 - PowerSupplyTol50/100);
         SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Der måles med multimeterprobe i TP35 eller stik X13 pin 3\n");
         Volt50Ok = TRUE;
           break;
  case 33  : SetPanelAttribute (vccPNL, ATTR_TITLE, " +3.3V Power Supply Check ");
         VMax = PowerSupplyNom33 * (1 + PowerSupplyTol33/100);
         VMin = PowerSupplyNom33 * (1 - PowerSupplyTol33/100);
         SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Der måles med multimeterprobe i TP2 eller stik X13 pin 5\n");
         Volt33Ok = TRUE;
           break;
  case -50 : SetPanelAttribute (vccPNL, ATTR_TITLE, " -5V Power Supply Check ");
         VMax = PowerSupplyNom50M * (1 - PowerSupplyTol50M/100);
         VMin = PowerSupplyNom50M * (1 + PowerSupplyTol50M/100);
         SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Der måles med multimeterprobe i TP3 eller stik X13 pin 7\n");
         VoltM50Ok = TRUE;
           break;
 }

 
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " GND = TP36\n\n\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Ved fejl: Tryk F9\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Ved OK:   Tryk F4\n");

 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_MAX_VALUE, VMax + 1.0);
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_MIN_VALUE, VMin - 1.0);

 Fmt(MsgTxt,"%s<%f[p2] - %f[p2] VDC",VMin,VMax);
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_LABEL_TEXT,MsgTxt);
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_TEXT_JUSTIFY,VAL_CENTER_JUSTIFIED);
 SetCtrlAttribute (vccPNL, VCC_PNL_LED, ATTR_ON_COLOR, VAL_RED);

 DisplayPanel(vccPNL);

 // set password = off & check RS-232 forbindelse
 if (WriteProtect(OFF,FALSE) == FALSE){
  HidePanel (vccPNL);
  VoltOk = FALSE;
  return VoltOk;
  }

#if IEEEBUS 
hp34970a_voltageMeasure (hp34970, "102", HP34970A_VM_VTYPE_DC,
                 HP34970A_VM_VRANGE_10V,
                 HP34970A_VM_RES_5_5DIG, datastr);
// The function 'hp34970a_voltageMeasure' must be executed once before using this function
hp34970a_voltageSetup (hp34970, "102", HP34970A_VS_VTYPE_DC,
                HP34970A_VS_VRANGE_10V, HP34970A_VS_RES_5_5DIG);
#endif
 done = FALSE;

 // wait for user to press F4=OK or F9=FAIL
 while (done == FALSE) {
  GetUserEvent(NOWAIT,&handle_returned, &controlID_returned);
  
  DisableBreakOnLibraryErrors();
#if IEEEBUS 
  ErrIEEE = hp34970a_cmdString_Q (hp34970, "READ?", 20, datastr);
  Scan(datastr,"%s>%f[b4]",&voltmeas);
#else
  voltmeas = 5.0;
#endif
  EnableBreakOnLibraryErrors();
  SetCtrlVal (vccPNL, VCC_PNL_VCCSLIDE, voltmeas);
  if ((voltmeas > VMax) || (voltmeas < VMin)) {
   SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_FILL_COLOR, VAL_RED);
   SetCtrlAttribute (vccPNL, VCC_PNL_LED, ATTR_ON_COLOR, VAL_RED);
    }
   else {
   SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_FILL_COLOR, VAL_GREEN);
   SetCtrlAttribute (vccPNL, VCC_PNL_LED, ATTR_ON_COLOR, VAL_GREEN);
    }

 } // while
 
 switch (NomVolt){
  case  50 : Volt50Ok = VoltOk; break;
  case  33 : Volt33Ok = VoltOk; break;
  case -50 : VoltM50Ok = VoltOk; break;
 } 

 
 HidePanel(vccPNL);
 return VoltOk;
} // CalPowerSupply()





//----------------------------------------------------------------------------
int CheckAudioGenVCO(int VCONo) {
// Spændingen måles i TP18
// VCONo vælger mellem VCO6144 og VCO56448
double  AudioVCOmeas;
int handle_returned, 
    controlID_returned;
char  MsgTxt[30];
ViChar datastr[20];

 CheckingAudioVCO = TRUE;
 CheckingTCXO = FALSE;
 
 ResetTextBox (vccPNL, VCC_PNL_TXTBOX, "");
 if (VCONo == VCO6144)
   SetPanelAttribute (vccPNL, ATTR_TITLE, " Audio Generator: Check af VCO spænding ved 6.144MHz");
  else
   SetPanelAttribute (vccPNL, ATTR_TITLE, " Audio Generator: Check af VCO spænding ved 5.6448MHz");
 
 DisplayPanel(vccPNL);

 WriteCOMPort(DUTCOM,":INP:GENL:SYST INT;");

 WriteCOMPort(DUTCOM,":OUTP:AUD:OUTP AES;");
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:SYST PAL;");

#if IEEEBUS
 hp34970a_voltageMeasure (hp34970, "102", HP34970A_VM_VTYPE_DC,
                  HP34970A_VM_VRANGE_10V,
                  HP34970A_VM_RES_6_5DIG, datastr);

 hp34970a_voltageSetup (hp34970, "102", HP34970A_VS_VTYPE_DC,
                HP34970A_VS_VRANGE_10V, HP34970A_VS_RES_6_5DIG);
#endif 

 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_MAX_VALUE, MaxAudioVCO + 1.0);
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_MIN_VALUE, MinAudioVCO - 1.0);
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Der måles med mulitmeterprobe i TP18, når apparatet har været tændt i > 5 min.\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Der kan også måles stik X13 pin 10.\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " HUSK: Intern ref (TCXO) skal være justeret\n\n");
 
 if (VCONo == VCO6144){
  SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " \n");
  SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Hvis spændingen er for høj formindskes C155\n");
  SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, "   -      -       -  -  lav øges C155\n\n");
  WriteCOMPort(DUTCOM,":OUTP:AUD:AES:WORD F48KHZ;");
 }
 else{
  SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " \n");
  SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Hvis spændingen er for høj formindskes C140\n");
  SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, "   -      -       -  -  lav øges C140\n\n");
  WriteCOMPort(DUTCOM,":OUTP:AUD:AES:WORD F441KHZ;");
 } 

 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " BEMÆRK: Proben SKAL holdes på målepunktet for at måle korrekt\n\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Ved OK:   Tryk F4\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Ved fejl: Tryk F9\n");

 Fmt(MsgTxt,"%s<%f[p2] - %f[p2] VDC",MinAudioVCO,MaxAudioVCO);
 
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_LABEL_TEXT,MsgTxt);
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_TEXT_JUSTIFY,VAL_CENTER_JUSTIFIED);

 done = FALSE;

 while (done == FALSE) {
  GetUserEvent(NOWAIT,&handle_returned, &controlID_returned);
  
  DisableBreakOnLibraryErrors();
#if IEEEBUS 
  ReadDCVolt(&AudioVCOmeas);
#else
  AudioVCOmeas = 1.0;
#endif
  EnableBreakOnLibraryErrors();
  SetCtrlVal (vccPNL, VCC_PNL_VCCSLIDE, AudioVCOmeas);
  if ((AudioVCOmeas > MaxAudioVCO) || (AudioVCOmeas < MinAudioVCO)) {
    SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_FILL_COLOR, VAL_RED);
   SetCtrlAttribute (vccPNL, VCC_PNL_LED, ATTR_ON_COLOR, VAL_RED);
    }
   else {
    SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_FILL_COLOR, VAL_GREEN);
   SetCtrlAttribute (vccPNL, VCC_PNL_LED, ATTR_ON_COLOR, VAL_GREEN);
    }

 } // while


 HidePanel(vccPNL);
 CheckingAudioVCO = FALSE;
 
return AudioVCOOk;
} //CheckAudioGenVCO





//-----------------------------------------------------------------------
int TPGTotalTest(int TestNo){

  DCOffsetOk = FALSE;
  PhaseScHGainOk = FALSE;
  TPGPattOk = FALSE;
  BBNo = 4;     // Used in 'MakeSyncOffsetCal' to select Test Signal Generator
  
  
  if (TestNo == 1){
      if (MakeSyncOffsetCal(SYNC_CAL) == TRUE)
     MakeSyncOffsetCal(OFFSET_CAL);
    }
     
  if ((TestNo == 2) || (DCOffsetOk == TRUE)){
    if (MakeTSGPhaseCal(PAL) == TRUE)
     MakeTSGPhaseCal(NTSC);
  }        

  if ((TestNo == 3) || (PhaseScHGainOk == TRUE))
   CheckTPGPattern();

  
return TPGPattOk;
}








//------ BB & TSG --------------------------------------------------------------
int CVICALLBACK ScHPilopCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
int psch;   
  switch (event) {
    case EVENT_COMMIT:
       if (TV == PAL){
        GetCtrlVal(phasePNL,PHASEPNL_SCHSLIDE,&ScHValuePAL);
        if (BBNo == 4)
          ScHValuePAL += 1;
         else
          ScHValuePAL += 3;
        SetCtrlVal(phasePNL,PHASEPNL_SCHSLIDE,ScHValuePAL);
        }
        else{
        GetCtrlVal(phasePNL,PHASEPNL_SCHSLIDE,&ScHValueNTSC);
        if (BBNo == 4)
          ScHValueNTSC += 1;
         else
          ScHValueNTSC += 3;
        SetCtrlVal(phasePNL,PHASEPNL_SCHSLIDE,ScHValueNTSC);
        }
       
       if (TV == PAL)
        psch = ScHValuePAL + ScHOffsetPAL;
       else
        psch = ScHValueNTSC + ScHOffsetNTSC;
       
       if (psch < 0)
        psch = ScHModulus + psch;
    
      if (TSGPhaseSelected == TRUE)
        SetTSGScH(psch);    // color bar generator
       else
        SetScH(psch);       // bb generator
      break;
  }
  return 0;
}


//------ BB & TSG -------------------------------------------------------------
int CVICALLBACK ScHPilnedCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
int psch;
  switch (event) {
    case EVENT_COMMIT:
       if (TV == PAL){
        GetCtrlVal(phasePNL,PHASEPNL_SCHSLIDE,&ScHValuePAL);
        if (BBNo == 4)
          ScHValuePAL -= 1;
         else
          ScHValuePAL -= 3;
        SetCtrlVal(phasePNL,PHASEPNL_SCHSLIDE,ScHValuePAL);
        }
        else{
        GetCtrlVal(phasePNL,PHASEPNL_SCHSLIDE,&ScHValueNTSC);
        if (BBNo == 4)
          ScHValueNTSC -= 1;
         else
          ScHValueNTSC -= 3;
        SetCtrlVal(phasePNL,PHASEPNL_SCHSLIDE,ScHValueNTSC);
        }

       if (TV == PAL)
        psch = ScHValuePAL + ScHOffsetPAL;
       else 
        psch = ScHValueNTSC + ScHOffsetNTSC;
        
       if (psch < 0)
        psch = ScHModulus + psch;
    
      if (TSGPhaseSelected == TRUE)
        SetTSGScH(psch);    // color bar generator
       else
        SetScH(psch);       // bb generator
      break;
  }
  return 0;
}

int CVICALLBACK SchSlideCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
int psch;
  switch (event) {
    case EVENT_VAL_CHANGED:
       if (TV == PAL)
        GetCtrlVal(phasePNL,PHASEPNL_SCHSLIDE,&ScHValuePAL);
       else
        GetCtrlVal(phasePNL,PHASEPNL_SCHSLIDE,&ScHValueNTSC);
       
       if (TV == PAL)
        psch = ScHValuePAL + ScHOffsetPAL;
       else
        psch = ScHValueNTSC + ScHOffsetNTSC;
       
       if (psch < 0)
        psch = ScHModulus + psch;
    
      if (TSGPhaseSelected == TRUE)
        SetTSGScH(psch);
       else
        SetScH(psch);
      break;
  }
  return 0;
}



int CVICALLBACK PhasePilopCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
int pv;   
  switch (event) {
    case EVENT_COMMIT:
       if (TV == PAL){
        GetCtrlVal(phasePNL,PHASEPNL_PHASESLIDE,&PhaseValuePAL);
        if (BBNo <= 3) PhaseValuePAL += 5; else PhaseValuePAL += 1;
        SetCtrlVal(phasePNL,PHASEPNL_PHASESLIDE,PhaseValuePAL);
        }
        else{
        GetCtrlVal(phasePNL,PHASEPNL_PHASESLIDE,&PhaseValueNTSC);
        if (BBNo <= 3) PhaseValueNTSC += 5; else PhaseValueNTSC += 1;
        SetCtrlVal(phasePNL,PHASEPNL_PHASESLIDE,PhaseValueNTSC);
       }

       if (TV == PAL){
         pv = PhaseValuePAL + PhaseOffsetPAL;
         if (pv < 0)
          pv = PALModulus + pv;
         } 
        else {
         pv = PhaseValueNTSC + PhaseOffsetNTSC;
         if (pv < 0)
          pv = NTSCModulus + pv;
       } 

      if (TSGPhaseSelected == TRUE)
        SetTSGPhase(pv);
       else
        SetPhase(pv);//"COMM 52,'HH%i';",sch)
      break;
  }
  return 0;
}


int CVICALLBACK PhasePilnedCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
int pv;   
  switch (event) {
    case EVENT_COMMIT:
       if (TV == PAL) {
        GetCtrlVal(phasePNL,PHASEPNL_PHASESLIDE,&PhaseValuePAL);
        if (BBNo <= 3) PhaseValuePAL -= 5;else PhaseValuePAL -= 1;
        SetCtrlVal(phasePNL,PHASEPNL_PHASESLIDE,PhaseValuePAL);
        }
        else{
        GetCtrlVal(phasePNL,PHASEPNL_PHASESLIDE,&PhaseValueNTSC);
        if (BBNo <= 3) PhaseValueNTSC -= 5;else PhaseValueNTSC -= 1;
        SetCtrlVal(phasePNL,PHASEPNL_PHASESLIDE,PhaseValueNTSC);
        }

       if (TV == PAL){
        pv = PhaseValuePAL + PhaseOffsetPAL;
        if (pv < 0)
         pv = PALModulus + pv;
       } 
       else{
        pv = PhaseValueNTSC + PhaseOffsetNTSC;
        if (pv < 0)
         pv = NTSCModulus + pv;
       } 

    
      if (TSGPhaseSelected == TRUE)
        SetTSGPhase(pv);
       else
        SetPhase(pv);
      break;
  }
  return 0;
}



int CVICALLBACK PhaseSlideCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
int pv;
  switch (event) {
    case EVENT_VAL_CHANGED:
       if (TV == PAL)
        GetCtrlVal(phasePNL,PHASEPNL_PHASESLIDE,&PhaseValuePAL);
       else
        GetCtrlVal(phasePNL,PHASEPNL_PHASESLIDE,&PhaseValueNTSC);

       if (TV == PAL)
        pv = PhaseValuePAL + PhaseOffsetPAL;
       else 
        pv = PhaseValueNTSC + PhaseOffsetNTSC;
        
       if (TV == PAL) {
        if (pv < 0)
         pv = PALModulus + pv;
       }  

       if (TV == NTSC) {
        if (pv < 0)
         pv = NTSCModulus + pv;
       }

       if (TSGPhaseSelected == TRUE)
        SetTSGPhase(pv);
       else
        SetPhase(pv);
      break;
  }
  return 0;
}



//-------------------------------------------------------------------------------
int CVICALLBACK GainSlideCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
{
  switch (event) {
    case EVENT_COMMIT:
      if (BBNo <= 3){   // BB 1,2,3
      GetCtrlVal(phasePNL,PHASEPNL_GAINSLIDE,&GainValue);
      SetBBGainDac(GainValue);
      }
      else{   // 4-bit chroma gain regulator V118 for TSG
      if (TV == PAL)
        GetCtrlVal(phasePNL,PHASEPNL_GAINSLIDE,&ChromaValuePAL);
       else
        GetCtrlVal(phasePNL,PHASEPNL_GAINSLIDE,&ChromaValueNTSC);
      SetTSGChroma();
      }
      
      break;
      
  }
  return 0;
}


//-------------------------------------------------------------------------------
int CVICALLBACK GainPilopCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
{
  switch (event) {
    case EVENT_COMMIT:
      if (BBNo <= 3){   // BB 1,2,3
       if (GainValue <= 250)
        GainValue += 10;
       SetCtrlVal(phasePNL,PHASEPNL_GAINSLIDE,GainValue);
       SetBBGainDac(GainValue);
      }
      else{   // 4-bit chroma gain regulator V118 for TSG
      if (TV == PAL){
         if (ChromaValuePAL <= 14)  // range 0..15
          ChromaValuePAL += 1;
         SetCtrlVal(phasePNL,PHASEPNL_GAINSLIDE,ChromaValuePAL);
        } 
        else{
         if (ChromaValueNTSC <= 14) // range 0..15
          ChromaValueNTSC += 1;
         SetCtrlVal(phasePNL,PHASEPNL_GAINSLIDE,ChromaValueNTSC);
       } 
       SetTSGChroma();
      
      
      }
      break;
  }
  return 0;
}



//-------------------------------------------------------------------------------
int CVICALLBACK GainPilnedCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
  {
  switch (event) 
    {
    case EVENT_COMMIT:
      if (BBNo <= 3)
        {   // BB 1,2,3
        if (GainValue >= 5)
          GainValue -= 10;
        SetCtrlVal(phasePNL,PHASEPNL_GAINSLIDE,GainValue);
        SetBBGainDac(GainValue);
        } 
      else
        {   // 4-bit chroma gain regulator for TSG
        if (TV == PAL)
          {
          if (ChromaValuePAL >= 1) // range 0..15
            ChromaValuePAL -= 1;
          SetCtrlVal(phasePNL,PHASEPNL_GAINSLIDE,ChromaValuePAL);
          }
        else
          {
          if (ChromaValueNTSC >= 1)  // range 0..15
            ChromaValueNTSC -= 1;
          SetCtrlVal(phasePNL,PHASEPNL_GAINSLIDE,ChromaValueNTSC);
          }
        SetTSGChroma();
        }
      break;
    }
  return 0;
  }





//-------------------------------------------------------------------------------
int CVICALLBACK Phase_LeftCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
  switch (event) {
    case EVENT_COMMIT:
     if (slideno == 1) 
      slideno = 3;
     else
      slideno--;
     
     if (slideno == 1){
      SetCtrlAttribute (phasePNL, PHASEPNL_SCHPILOP_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_SCHPILNED_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASEPILOP_BTN, ATTR_DIMMED,FALSE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASEPILNED_BTN, ATTR_DIMMED,FALSE);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAINPILOP_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAINPILNED_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASE_DEC, ATTR_FRAME_COLOR,VAL_BLUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_SCH_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAIN_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);
     }
     else if (slideno == 2){
      SetCtrlAttribute (phasePNL, PHASEPNL_SCHPILOP_BTN, ATTR_DIMMED,FALSE);
      SetCtrlAttribute (phasePNL, PHASEPNL_SCHPILNED_BTN, ATTR_DIMMED,FALSE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASEPILOP_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASEPILNED_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAINPILOP_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAINPILNED_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASE_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);
      SetCtrlAttribute (phasePNL, PHASEPNL_SCH_DEC, ATTR_FRAME_COLOR,VAL_BLUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAIN_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);
     }
     else if (slideno == 3){
      SetCtrlAttribute (phasePNL, PHASEPNL_SCHPILOP_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_SCHPILNED_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASEPILOP_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASEPILNED_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAINPILOP_BTN, ATTR_DIMMED,FALSE);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAINPILNED_BTN, ATTR_DIMMED,FALSE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASE_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);
      SetCtrlAttribute (phasePNL, PHASEPNL_SCH_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAIN_DEC, ATTR_FRAME_COLOR,VAL_BLUE);
     }
      break;
  }
  return 0;
}



//-------------------------------------------------------------------------------
int CVICALLBACK Phase_RightCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
  switch (event) {
    case EVENT_COMMIT:
     if (slideno == 3) 
      slideno = 1;
     else
      slideno++;
     
     if (slideno == 1){
      SetCtrlAttribute (phasePNL, PHASEPNL_SCHPILOP_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_SCHPILNED_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASEPILOP_BTN, ATTR_DIMMED,FALSE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASEPILNED_BTN, ATTR_DIMMED,FALSE);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAINPILOP_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAINPILNED_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASE_DEC, ATTR_FRAME_COLOR,VAL_BLUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_SCH_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAIN_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);
     }
     else if (slideno == 2){
      SetCtrlAttribute (phasePNL, PHASEPNL_SCHPILOP_BTN, ATTR_DIMMED,FALSE);
      SetCtrlAttribute (phasePNL, PHASEPNL_SCHPILNED_BTN, ATTR_DIMMED,FALSE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASEPILOP_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASEPILNED_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAINPILOP_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAINPILNED_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASE_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);
      SetCtrlAttribute (phasePNL, PHASEPNL_SCH_DEC, ATTR_FRAME_COLOR,VAL_BLUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAIN_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);
     }
     else if (slideno == 3){
      SetCtrlAttribute (phasePNL, PHASEPNL_SCHPILOP_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_SCHPILNED_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASEPILOP_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASEPILNED_BTN, ATTR_DIMMED,TRUE);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAINPILOP_BTN, ATTR_DIMMED,FALSE);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAINPILNED_BTN, ATTR_DIMMED,FALSE);
      SetCtrlAttribute (phasePNL, PHASEPNL_PHASE_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);
      SetCtrlAttribute (phasePNL, PHASEPNL_SCH_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);
      SetCtrlAttribute (phasePNL, PHASEPNL_GAIN_DEC, ATTR_FRAME_COLOR,VAL_BLUE);
     }

      break;
  }
  return 0;
}



//-------------------------------------------------------------------------------
int CVICALLBACK PhaseScHOkCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
  switch (event) {
    case EVENT_COMMIT:
      done = TRUE;
      PhaseScHGainOk = TRUE;
      break;
  }
  return 0;
}



//-------------------------------------------------------------------------------
int CVICALLBACK ExitCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
  switch (event) {
    case EVENT_COMMIT:
      done = TRUE;
      PhaseScHGainOk = FALSE;

      break;
  }
  return 0;
}




//-------------------------------------------------------------------------------
int CVICALLBACK VCCOkCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
  switch (event) {
    case EVENT_COMMIT:  done = TRUE;
              if (CheckingPS == TRUE)
               VoltOk = TRUE;
              if (CheckingTCXO == TRUE)
               TcxoOk = TRUE;
              if (CheckingAudioVCO == TRUE)
               AudioVCOOk = TRUE;
              break;
  }
  return 0;
}


//-------------------------------------------------------------------------------
int CVICALLBACK VCCExitCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
  switch (event) {
    case EVENT_COMMIT:  done = TRUE;
              if (CheckingPS == TRUE)
               VoltOk = FALSE;
              if (CheckingTCXO == TRUE)
               TcxoOk = FALSE;
              if (CheckingAudioVCO == TRUE)
               AudioVCOOk = FALSE;
              break;
  }
  return 0;
}




//-------------------------------------------------------------------------------
int CVICALLBACK DataOkCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
  switch (event) {
    case EVENT_COMMIT: Cont = TRUE;
               break;
  }
  return 0;
}





//-------------------------------------------------------------------------------
int CVICALLBACK FilterOkCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
  switch (event) {
    case EVENT_COMMIT:
        Cont = TRUE;

      break;
  }
  return 0;
}


#if 1
//-------------------------------------------------------------------------------
int CVICALLBACK SpecialPatternCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
  switch (event) {
    case EVENT_COMMIT:
         WriteCOMPort(DUTCOM,":OUTP:ASIG:SYST PAL;");
          WriteCOMPort(DUTCOM,":FACT:V24C:ADDR ANLS;");
        break;
  }
  return 0;
}
#endif


