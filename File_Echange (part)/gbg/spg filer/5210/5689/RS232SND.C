/************************************************************************/
/* File :       Rs232snd.c                                              */
/* Create :                                                             */
/* Last Edit :  951029 Sten A.Andersen                                  */
/* Status :     Accepted                                                */
/* Version :    1.00                                                    */
/************************************************************************/
/*951029 Added test 5 & 6 : AD 0/1 ,    dB * 2 */
#include "Standard.def"   /* General definitions */
#include "Nicam89.def"   /* Instrument specific definitions */
#include "Version.def"   /* Defining SW release */

#include <stdio.h>
#include <reg52.h>

/* Instrument specific data type declarations */
#include <main_mem.h>
#include <rs232snd.h>
/* Remote interface required for IEEE address programming */
#include <rem_hwi.h>

extern xdata UC slicer;
extern UC TestMaster;

bit TrxFree;      /* Bit which indicates a idle transmission channel */

bit AlarmsChanged;  /* Bit is set whenever a Alarm bit is changed */

xdata   UC      RemLocStatus;


xdata   char  CmdToSend[30];
xdata   UC    CmdIndex;
xdata   UC    TrxState;
xdata   UC    TrxTimer;
xdata   UC    TrxBEt;
xdata   UC    Front_slicer;

#define TimeOut 10
xdata   UC    Trx_I_request; /* request from receiver part for a transmission
                                '00' none , other : code */

enum T_States { TrxIdle = 0, Trx_error, Trx_i, Trx_c,
                             TrxRepeat, TrxAlmostIdle1, TrxAlmostIdle2 };

/*********************************************************************/
/* Interrupt interface                                               */
/* Called from rx interrupt to request tx                            */
UC Request(UC rCode){
    if(Trx_I_request == 0) {
        Trx_I_request = rCode;
        Front_slicer = 0;
        return(0);
    }
    else {
        /* In case of not ready : overwrite and return '1' as errorind. */
        Trx_I_request = rCode;
        Front_slicer = 0;
        return(1);
    }
}


/* Called from  interrupt if Checksum recived and OK */
void CheckOK(void){
     if((TrxState == Trx_i)|| (TrxState == Trx_c)){
         TrxState = TrxAlmostIdle1;
     }
     else {
         if (TrxState == TrxRepeat){
             TrxState = TrxAlmostIdle1;
         }
         else {
             TrxState = Trx_error;
         }
     }
}

/* Called from  interrupt if Checksum error received */
void CheckError(void){
     if(TrxState == Trx_i){
         /* Force timeout */
         TrxTimer = TimeOut;
     }
     else {
         if(TrxState == Trx_c){
             TrxState = TrxAlmostIdle1;
         }
         else {
             if (TrxState == TrxRepeat){
                 TrxState = TrxAlmostIdle1;
             }
             else {
                 TrxState = Trx_error;
             }
         }
     }
}

/* Called from  interrupt if Command error received */
void CommandError(void){
     if(( TrxState == Trx_i)|| (TrxState == Trx_c)){
         TrxState = TrxAlmostIdle1;
     }
     else {
         if (TrxState == TrxRepeat){
             TrxState = TrxAlmostIdle1;
         }
         else {
             TrxState = Trx_error;
         }
     }
}
/*********************************************************************/

UC ReSend(void){
    CmdIndex = 0xFE;
}

void TrxSTMinit(void){
    TrxState = TrxIdle;
    CmdIndex = 0xff;
    TrxFree  = 1;
    Trx_I_request =  WaitForFront ;
    AlarmsChanged = On;
    Measured.TestMode = 0;
}

void GoTrx_i(void){
              Trx_I_request = 0;
              TrxState = Trx_i;
              TrxTimer = 0;
              CmdIndex = 0;
}

void GoTrx_i2(void){
              TrxState = Trx_i;
              TrxTimer = 0;
              CmdIndex = 0;
}

void GoTrx_c(void){
              TrxState = Trx_c;
              TrxTimer = 0;
              CmdIndex = 0;
}


UC TrxSTM(void){

    if ((TrxFree == 1) && (CmdIndex != 0xff )) {
       TrxFree = 0;
       TI = 1;
    }

    switch (TrxState)  {

    default:
    case TrxIdle:
         if(Trx_I_request != 0 ) {
              switch (Trx_I_request) {
              case WaitForFront:
                  /* hang here until front requests GO or FACT */
                  /* or any other request from front */
              break;

              case FACT_work:
              case PRST_work:
              case GO_work:
                  Front_slicer++;

                  if (Front_slicer == 10) {
                      sprintf(CmdToSend,"IFIN%bu;",Actual.setup.InputSelect);
                      GoTrx_i2();
                  }
                  if (Front_slicer == 20) {
                      sprintf(CmdToSend,"IFCA%bu;",Actual.setup.IF_Mode);
                      GoTrx_i2();
                  }
                  if (Front_slicer == 30) {
                      sprintf(CmdToSend,"AULA%bu;",Actual.setup.Audio_select);
                      GoTrx_i2();
                  }
                  if (Front_slicer == 40) {
                      sprintf(CmdToSend,"EYE%bu,%bu;",Measured.I_Eye,Measured.Q_Eye);
                      GoTrx_c();
                  }
                  if (Front_slicer == 50) {
                      sprintf(CmdToSend,"BERnE-n;");
                      CmdToSend[3] = ( Measured.BER >> 4 ) + '0';
                      CmdToSend[6] = ( Measured.BER & 0x0f ) + '0';
                      GoTrx_c();
                  }
                  if (Front_slicer == 60) {
                      if (Measured.No_Error_Time < 15*50*60 ) {
                          sprintf(CmdToSend,"BRST%u,%u,0,%u;",
                                  Measured.Error_Bursts,
                                  Measured.Burst_Length*20,
                                  Measured.No_Error_Time/50);
                      }
                      else {
                          sprintf(CmdToSend,"BRST%u,%u,%u;",
                                  Measured.Error_Bursts,
                                  Measured.Burst_Length*20,
                                  Measured.No_Error_Time/(50*60));
                      }
                      GoTrx_c();
                  }
                  if (Front_slicer == 70) {
                      sprintf(CmdToSend,"SNDM%bu,%bu;",
                        ( (Measured.SndStatus & 0x04) == 0x04 ? NOT_DEFINEDMode :
                             (Measured.SndStatus & 0x03)
                        )
                        , ((Measured.SndStatus & 0x08) == 0x08) ?(UC)TRUE : (UC)FALSE
                      );
                      GoTrx_c();
                  }
                  if (Front_slicer == 80) {
                      sprintf(CmdToSend,"ALST%bu;",Measured.Alarms);
                      GoTrx_c();
                  }

                  if (Front_slicer > 100) {
                      if (Trx_I_request == FACT_work){
                          sprintf(CmdToSend,"FACT%bu;",TestMaster);
                          GoTrx_i();
                      }
                      if (Trx_I_request == GO_work){
                          sprintf(CmdToSend,"GO%bu;",TestMaster);
                          GoTrx_i();
                      }
                      if (Trx_I_request == PRST_work){
                          sprintf(CmdToSend,"PRST;");
                          GoTrx_i();
                      }
                  }

              break;

              case FACT_tx:
                   Trx_I_request = FACT_work;
                   Actual.setup = Default;
              break;

              case PRST_tx:
                   Trx_I_request = PRST_work;
                   Actual.setup = Default;
              break;

              case GO_tx:
                   Trx_I_request = GO_work;
              break;

              case HCON_tx:
                  sprintf(CmdToSend,"HCON%bu,%bu,%bu;",Actual.setup.InputVersion,
                                                       Actual.setup.TV_Version,
                                                       Actual.setup.Options);
                  GoTrx_i();
              break;

              case EHS_tx:
                  sprintf(CmdToSend,
                          "EHS%bu,%bu,%bu;",
                          Actual.setup.Eye_limit,Min.Eye_limit,Max.Eye_limit);
                  GoTrx_i();
              break;
              case IEAD_tx:
                  sprintf(CmdToSend,
                      "IEAD%bu,%bu,%bu,%bu;",    /* 3 no HW , 2 : SW , 0:HW ok
                                                      , 1:HW not ready*/
                      ((Actual.setup.Options & OptionsMask_IEEE) == 0 ?
                       3 :
                       ((busadr_get() & 0x80 ) == 0 ?
                           2  :
                           ((busadr_get() & 0x40 ) == 0 ? 0 : 1 )  ) ),

                       busadr_get() & 0x1f,Min.busadr,Max.busadr);

                  GoTrx_i();
              break;
              case MADI_tx:
                  sprintf(CmdToSend,"MADI%bu;",TestMaster);
                  GoTrx_i();
              break;
              case ALST_tx:
                  sprintf(CmdToSend,"ALST%bu;",Measured.Alarms);
                  GoTrx_i();
              break;
              case QPAL_tx:
                  sprintf(CmdToSend,"QPAL%bu;",Actual.setup.Input_alarm_enable);
                  GoTrx_i();
              break;
              case ATT_tx:
                  sprintf(CmdToSend,"ATT%bu;",(Actual.setup.Atten<10) ? 0 : 0xff );
                  GoTrx_i();
              break;
              case AULA_tx:
                  sprintf(CmdToSend,"AULA%bu;",Actual.setup.Audio_select);
                  GoTrx_i();
              break;
              case AULE_tx:
                  sprintf(CmdToSend,"AULE%bu,%bu,%bu;",Actual.setup.Audio_level+64,
                  Min.Audio_level+64,Max.Audio_level+64);
                  GoTrx_i();
              break;
              case BERS_tx:

               /* Tager ikke h›jde for 'cifre' > '9' !!!!!!!!!!!!! */
              /*    sprintf(CmdToSend,"BERS1E-5,1E-9,9E-1;"); */
                  sprintf(CmdToSend,"BERSnE-n,nE-n,nE-n;");
                  CmdToSend[4] = ( Actual.setup.BER_limit >> 4 ) + '0';
                  CmdToSend[7] = ( Actual.setup.BER_limit & 0x0f ) + '0';
                  CmdToSend[9] = ( Min.BER_limit >> 4 ) + '0';
                  CmdToSend[12] = ( Min.BER_limit  & 0x0f ) + '0';
                  CmdToSend[14] = ( Max.BER_limit >> 4 ) + '0';
                  CmdToSend[17] = ( Max.BER_limit  & 0x0f ) + '0';
                  GoTrx_i();

              break;
              case IFCA_tx:
                  sprintf(CmdToSend,"IFCA%bu;",Actual.setup.IF_Mode);
                  GoTrx_i();
              break;
              case IFIN_tx:
                  sprintf(CmdToSend,"IFIN%bu;",Actual.setup.InputSelect);
                  GoTrx_i();
              break;
              case QPRE_tx:
                  sprintf(CmdToSend,"QPRE%bu;",(UC)TRUE);     /* ALLWAYS Accept . TO BE CHANGED !!*/
                  GoTrx_i();
              break;
              case SVER_tx:
                  sprintf(CmdToSend,"SVER%bu;",ReleaseAsHex);
                  GoTrx_i();
              break;
              case GTRE_tx:
                  sprintf(CmdToSend,"GTRE;");
                  GoTrx_i();
              break;
              case GTLO_tx:
                  sprintf(CmdToSend,"GTLO;");
                  GoTrx_i();
              break;

              case TEST_tx:
                  sprintf(CmdToSend,"TEST%bu;",(UC)NumOfTests);
                  GoTrx_i();
              break;
              }
         }
         else {
             if (AlarmsChanged == On || (RemLocStatus != RemoteStatus() )) {
                  if (AlarmsChanged == On)
                  {   sprintf(CmdToSend,"ALST%bu;",Measured.Alarms);
                      TrxState = Trx_i;
                      TrxTimer = 0;
                      CmdIndex = 0;
                      AlarmsChanged = Off;
                  }
                  if (RemLocStatus != RemoteStatus() )
                  {
                      if ((RemoteStatus() & 0x01) == 0)
                      {
                          sprintf(CmdToSend,"GTLO;");
                      }
                      else
                      {
                          sprintf(CmdToSend,"GTRE;");
                      }
                      RemLocStatus = RemoteStatus() ;
                      GoTrx_i2();
                  }
             }         /*...... Temporary diabling auto-transmit : */
             else {
                 if (slicer == 6){

                  /*    sprintf(CmdToSend,"EYE67,77;");*/

                      sprintf(CmdToSend,"EYE%bu,%bu;",Measured.I_Eye,Measured.Q_Eye);
                      TrxState = Trx_c;
                      TrxTimer = 0;
                      CmdIndex = 0;
                 }

                 if ( slicer == 16 ) {
                     switch (Measured.TestMode) {
                     default:

                         if(TrxBEt++ < 2){
                             sprintf(CmdToSend,"BERnE-n;");
                             CmdToSend[3] = ( Measured.BER >> 4 ) + '0';
                             CmdToSend[6] = ( Measured.BER & 0x0f ) + '0';
                         }
                         else {
                             if (Measured.No_Error_Time < 15*50*60 ) {
                                 sprintf(CmdToSend,"BRST%u,%u,0,%u;",
                                         Measured.Error_Bursts,
                                         Measured.Burst_Length*20,
                                         (UI)(Measured.No_Error_Time/50));
                             }
                             else {
                                 sprintf(CmdToSend,"BRST%u,%u,%u;",
                                         Measured.Error_Bursts,
                                         Measured.Burst_Length*20,
                                         (UI)(Measured.No_Error_Time/(50*60)));
                             }
                             TrxBEt = 0;
                         }
                         GoTrx_c();

                         break;
                         case Test1:
                              sprintf(CmdToSend,"TRES%u,%u;",
                                  (UI)(10000+10.0*Measured.InputLevel),
                                  (UI)(10000+10.0*Actual.setup.InputLev_ref));
                              GoTrx_c();
                         break;
                         case Test2:
                              sprintf(CmdToSend,"TRES%u,%u;",
                                  (UI)TestData.I_Byte,
                                  (UI)TestData.Q_Byte);
                              GoTrx_c();
                         break;
                         case Test3:
                              sprintf(CmdToSend,"TRES%u,%u;",
                                  TestData.IC_lev,
                                  TestData.AD_Channel[0]);
                              GoTrx_c();
                         break;

                         case Test4:
                              sprintf(CmdToSend,"TRES%u,%u;",
                                  TestData.BadCounter,
                                  TestData.AD_Channel[0]);
                              GoTrx_c();
                         break;

                         case Test5:
                              sprintf(CmdToSend,"TRES%u,%u;",
                                  TestData.AD_Channel[0],
                                  TestData.AD_Channel[1]);
                              GoTrx_c();
                         break;

                         case Test6:
                              sprintf(CmdToSend,"TRES%u,%u;",
                                  TestData.IC_lev,
                                  (UI)(1100+10.0*Measured.InputLevel));
                              GoTrx_c();
                         break;

                         case Test10:
                              sprintf(CmdToSend,"TRES1000,00;");
                              GoTrx_c();
                         break;
                         case Test11:
                              sprintf(CmdToSend,"TRES4000,00;");
                              GoTrx_c();
                         break;

                         case Test12:
                              sprintf(CmdToSend,"TRES14000,00;");
                              GoTrx_c();
                         break;

                         case Test13:
                              sprintf(CmdToSend,"TRES20,00;");
                              GoTrx_c();
                         break;

                         case Test14:
                              sprintf(CmdToSend,"TRES0,1000;");
                              GoTrx_c();
                         break;
                         case Test15:
                              sprintf(CmdToSend,"TRES0,4000;");
                              GoTrx_c();
                         break;

                         case Test16:
                              sprintf(CmdToSend,"TRES0,14000;");
                              GoTrx_c();
                         break;

                         case Test17:
                              sprintf(CmdToSend,"TRES0,20;");
                              GoTrx_c();
                         break;

                         case Test18:
                              sprintf(CmdToSend,"TRES4960,5040;");
                              GoTrx_c();
                         break;

                         case Test19:
                              sprintf(CmdToSend,"TRES2,2;");
                              GoTrx_c();
                         break;

                         case Test20:
                              sprintf(CmdToSend,"TRES9960,10040;");
                              GoTrx_c();
                         break;

                         case Test21:
                              sprintf(CmdToSend,"TRES10101,10101;");
                              GoTrx_c();
                         break;
                     }
                 }

                 if (slicer == 26){
                     if (  ( Measured.SndStatus & 0x10) == 0  )   {
                          sprintf(CmdToSend,"SNDM%bu,%bu;",
                            ( (Measured.SndStatus & 0x04) == 0x04 ? NOT_DEFINEDMode :
                                 (Measured.SndStatus & 0x03)
                            )
                            , ((Measured.SndStatus & 0x08) == 0x08) ? (UC)TRUE : (UC)FALSE
                          );
                      }
                      else {
                          sprintf(CmdToSend,"SNDM8,0;");
                      }
                      TrxState = Trx_c;
                      TrxTimer = 0;
                      CmdIndex = 0;
                 }
             }
         }
    break;
    case Trx_i:
         /**************************************/
         /*Interrupt controlled change of State*/
         /**************************************/
         TrxTimer++;
         if (TrxTimer > TimeOut) {
            /*TIMEOUT*/
             ReSend();
             TrxTimer = 0;
             TrxState = TrxRepeat;
         }
    break;
    case Trx_c:
         /**************************************/
         /*Interrupt controlled change of State*/
         /**************************************/
         TrxTimer++;
         if (TrxTimer > TimeOut) {
            /*TIMEOUT*/
             TrxState = TrxIdle;
         }

    break;
    case TrxRepeat:
         /**************************************/
         /*Interrupt controlled change of State*/
         /**************************************/
         TrxTimer++;
         if (TrxTimer > TimeOut) {
            /*TIMEOUT*/
             TrxState = TrxIdle;
         }

    break;
    case Trx_error:
         /* Comm error handling */
         TrxState = TrxAlmostIdle2;
    break;

    case TrxAlmostIdle1:
         TrxState = TrxAlmostIdle2;
    break;

    case TrxAlmostIdle2:
         TrxState = TrxIdle;
    break;

    }
}
