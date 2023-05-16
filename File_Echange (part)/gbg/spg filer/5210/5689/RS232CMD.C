/************************************************************************/
/* File :       Rs232cmd.c                                              */
/* Create :     950xxx Kim Engedahl                                     */
/* Last Edit :  950905 Sten A.Andersen                                  */
/* Status :     Accepted                                                */
/* Version :    0.04                                                    */
/************************************************************************/
/* Log of changes since Montreux */
/* 950704 Changing commands to IEEE address programming */
/* 950621 importing new command-describing tables from KEn */
/* 950621 Adding shell of commands TEST,TRES,GO,FACT,PRST  */
/* Add functionality HCON */

/* 95062x Add body to above functions */
/* 950621 Adding limit value to Min-Max interval to AULE */

/* End of log */
#include "Standard.def"   /* General definitions */
#include "Nicam89.def"   /* Instrument specific definitions */
#pragma RB(3)

/* Low level receiver / interpreter driver include*/
#include "rs232drv.h"

/* Low level transmitter include */
#include "rs232snd.h"

/* Instrument specific data type declarations */
#include <main_mem.h>
#include <rem_hwi.h>

/* At the end of the module 3 tables and some constants has to be carefully
   updated  when there is any changes in number or spelling of commands !!! */

/*************/
/* COMMANDS :*/
/***************************************************************************/
/* Common information to all functions in this module                      */
/* Module:      RS232CMD.C                                                 */
/*   Syntax:     void nnnn_Command();                                      */
/*   Returns:    nothing                                                   */
/* Input       CmdReady       bit   i/o     if '0' : still parsing
                                            if '1' then command is
   & Output :                               terminated and OK.
                                            If execute error then return '0'.

               ParamNo        uchar i       Index to the currently parsed
                                            parameter.
               Param[]        uchar i/(o)   Array of parameters. Initial = 0

               CharType       uchar i       Type of actual char
               ReceiveChar    uchar i       The actual received char
               */
/**/
/***************************************************************************/
/*   EHS_Command                                                           */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*   Function:   Receive Eyeheight alarm setting and limits.               */
/***************************************************************************/
void EHS_Command()  {
   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(EHS_tx);
      }
      /* Command */
      if (ReceiveDel == '$') {
         /* Test for number of parameters */
          if ( ParamNo == 1) {
             /* Do whatever required with Param[0] */
             iUpdate(Eye_limit,Param[0]);
          }
          else {
            /* Number of parameters not accepted , */
             CmdReady = 0;
          }
      }
   }
   else {
      /* Parse */
      /* Test for Char of Type Numeric */
      if ( CharType == N)
         Param[0] = 10*Param[0] + ( ReceiveChar - '0');
   }

}

/***************************************************************************/
/*   ALST_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*                                                                         */
/*   Function:   Receive alarm status information from RS232               */
/*   Remarks:    RS232-command: ALST<n>;                                   */
/*                  n(0) =   IF vision carrier    ( IF)                    */
/*                    n(1) =   QPSK level carrier   ( IF & IC)             */
/*                    n(2) =   Eye height                                  */
/*                    n(3) =   Bit error rate                              */
/*                    n(4) =   NICAM Frames                                */
/*                    n(5-7) = free                                        */
/***************************************************************************/
void ALST_Command()  {
   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(ALST_tx);
      }
      else { /* Only requests expected */
           CmdReady = 0;
      }
   }
   else {
      /* Parse , nothing to do */
   }
}

/***************************************************************************/
/*   ATT_Command                                                           */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*   Function:   Receive attenuator setting, (On/Off ie. 0/20dB)           */
/*   Syntax:     void ATT_Command();                                       */
/*   Remarks:    RS232-command: ATT<l>;                                    */
/*   Returns:    nothing                                                   */
/***************************************************************************/
void ATT_Command()  {         /* Input attenuator, IF & IC version */
   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(ATT_tx);
      }

      /* Command */
      if (ReceiveDel == '$') {
         /* Test for number of parameters */
          if ( ParamNo == 1) {
             iUpdate(Atten,(Param[0]==0) ? 0 : 20 );
          }
          else {
            /* Number of parameters not accepted , */
             CmdReady = 0;
          }
      }
   }
   else {
      /* Test for Char of Type Numeric */
      if ( CharType == N)
         Param[0] = 10*Param[0] + ( ReceiveChar - '0');
   }


}


/***************************************************************************/
/*   AULA_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*   Function:   Receive audio language setting, (ie. AA/BB/AB)            */
/*   Syntax:     void AULA_Command();                                      */
/*   Remarks:    RS232-command: AULA<e>;                                   */
/*   Returns:    nothing                                                   */
/***************************************************************************/
void AULA_Command()  {      /* Audio language setting */

   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(AULA_tx);
      }

      /* Command */
      if (ReceiveDel == '$') {
         /* Test for number of parameters */
          if ( ParamNo == 1) {
             iUpdate(Audio_select,Param[0]);
          }
          else {
            /* Number of parameters not accepted , */
             CmdReady = 0;
          }
      }
   }
   else {
      /* Test for Char of Type Numeric */
      if ( CharType == N)
         Param[0] = 10*Param[0] + ( ReceiveChar - '0');
   }
}


/***************************************************************************/
/*   AULE_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*   Function:   Receive audio level setting and limits                    */
/*   Syntax:     void AULE_Command();                                      */
/*   Remarks:    RS232-command: AULA<n>,<n>,<n>;                           */
/*   Returns:    nothing                                                   */
/***************************************************************************/
void AULE_Command()  {
   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(AULE_tx);
      }

      /* Command */
      if (ReceiveDel == '$') {
         /* Test for number of parameters */
          if ( ParamNo == 1) {
             /* Do whatever required with Param[0] */
             if(Param[0] < Max.Audio_level+64) {
                 if (Param[0] > Min.Audio_level+64){
                     iUpdate(Audio_level,Param[0]-64);
                 }
                 else {
                     iUpdate(Audio_level,Min.Audio_level);
                 }
             }
             else {
                 iUpdate(Audio_level,Max.Audio_level);
             }
          }
          else {
            /* Number of parameters not accepted , */
             CmdReady = 0;
          }
      }
   }
   else {
      /* Parse */
      /* Test for Char of Type Numeric */
      if ( CharType == N)
         Param[0] = 10*Param[0] + ( ReceiveChar - '0');
   }
}

/***************************************************************************/
/*   BERS_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*   Function:   Receive Bit Error Rate alarm setting and limits           */
/*   Remarks:    RS232-command: BERS<f1>,<f1>,<f1>;                        */
/***************************************************************************/
void BERS_Command()  {
   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(BERS_tx);
      }

      /* Command */
      if (ReceiveDel == '$') {
         /* Test for number of parameters */
          if ( ParamNo == 1) {
             /* Do whatever required with Param[0] */
             iUpdate(BER_limit,Param[0]);
          }
          else {
            /* Number of parameters not accepted , */
             CmdReady = 0;
          }
      }
   }
   else {
      /* Parse */
      /* Test for Char of Type Numeric OBS Format expected: 2e-4 */
      if ( CharType == N)     /* by ignoring '-' and 'e'
                                 and multiply by 16 */

         if ( (ReceiveChar != '-') && (ReceiveChar != 'E') ) {
            Param[0] = 16*Param[0] + ( ReceiveChar - '0');
         }
   }
}

/***************************************************************************/
/*   BER_Command                                                           */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*   Function:   Receive actual Bit Error Rate.                            */
/*   Syntax:     void BER_Command();                                       */
/*   Remarks:    Not a master receiver                                     */
/*   Returns:    nothing                                                   */
/***************************************************************************/
void BER_Command()  {
    CmdReady = 0;
}
/***************************************************************************/
/*   BRST_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*   Function:   Receive actual Error Burst information.                   */
/*   Syntax:     void BRST_Command();                                      */
/*   Remarks:    Not a master receiver                                     */
/***************************************************************************/
void BRST_Command()  {
    CmdReady = 0;
}
/***************************************************************************/
/*   EYE_Command                                                           */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*   Function:   Receive actual Eyeheights, (ie. I & Q).                   */
/*   Syntax:     void EYE_Command();                                       */
/*   Remarks:    Not a master receiver                                     */
/***************************************************************************/
void EYE_Command()  {
    CmdReady = 0;
}
/***************************************************************************/
/*   FRDI_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*   Function:   Front plate diagnostics request.                          */
/*   Syntax:     void FRDI_Command();                                      */
/*   Remarks:    Not a master receiver                                     */
/***************************************************************************/
void FRDI_Command()  {
    CmdReady = 0;
}
/***************************************************************************/
/*   GTLO_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950621                                                     */
/*   Function:   Request from front: Goto local                            */
/*   Syntax:     void GTLO_Command();                                      */
/*   Remarks:    RS232-command: GTLO;                                      */
/*               Requires either 'GTLO' or 'GTRE' as reponse to front      */
/*               (GTRE means instument is in 'Local Lockout')              */
/***************************************************************************/
void GTLO_Command()  {
   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          if (RequestLocal() == 0 ) {
              Request(GTLO_tx);
          }
          else {
              Request(GTRE_tx);
          }
      }
      else {
          CmdReady = 0;
      }
   }
}

/***************************************************************************/
/*   GTRE_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950621                                                     */
/*   Function:   Goto remote, (ie. remote/local operation)                 */
/*   Remarks:    NOT master rx cmd;                                        */
/***************************************************************************/
void GTRE_Command()  {
     CmdReady = 0;
}

/***************************************************************************/
/*   HCON_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950621                                                     */
/*   Function:   Request hardware configuration.                           */
/***************************************************************************/
void HCON_Command()  {
   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(HCON_tx);
      }
      else {
          CmdReady = 0;
      }
   }
}

/***************************************************************************/
/*   IEAD_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*   Function:   Receive IEEE-address setting and options/limits.          */
/*   Syntax:     void IEAD_Command();                                      */
/*   Remarks:    RS232-command: IEAD<e>,<n>,<n>,<n>;                       */
/*   Returns:    nothing                                                   */
/***************************************************************************/
void IEAD_Command()  {

   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(IEAD_tx);
      }
      /* Command */
      if (ReceiveDel == '$') {
         /* Test for number of parameters */
          if ( ParamNo == 1) {
             /* Do whatever required with Param[0] */
             if ( Param[0] == Max.busadr+1) {   /* Use HW switches */
                 busadr_use_switches();
             }
             else {                            /* Soft address */
                 busadr_set(Param[0]);
             }
          }
          else {
            /* Number of parameters not accepted , */
             CmdReady = 0;
          }
      }
   }
   else {
      /* Parse */
      /* Test for Char of Type Numeric */
      if ( CharType == N)
         Param[0] = 10*Param[0] + ( ReceiveChar - '0');
   }



}

/***************************************************************************/
/*   IFCA_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*   Function:   Receive input carrier mixing setting.                     */
/*   Syntax:     void IFCA_Command();                                      */
/*   Remarks:    RS232-command: IFCA<e>;                                   */
/*   Returns:    nothing                                                   */
/***************************************************************************/
void IFCA_Command()  {
   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(IFCA_tx);
      }

      /* Command */
      if (ReceiveDel == '$') {
         /* Test for number of parameters */
          if ( ParamNo == 1) {
             iUpdate(IF_Mode,Param[0]);
          }
          else {
            /* Number of parameters not accepted , */
             CmdReady = 0;
          }
      }
   }
   else {
      /* Test for Char of Type Numeric */
      if ( CharType == N)
         Param[0] = 10*Param[0] + ( ReceiveChar - '0');
   }
}


/***************************************************************************/
/*   IFIN_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*   Function:   Receive input selection setting.                          */
/*   Syntax:     void IFIN_Command();                                      */
/*   Remarks:    RS232-command: IFIN<e>;                                   */
/*   Returns:    nothing                                                   */
/***************************************************************************/
void IFIN_Command()  {
   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(IFIN_tx);
      }

      /* Command */
      if (ReceiveDel == '$') {
         /* Test for number of parameters */
          if ( ParamNo == 1) {
             iUpdate(InputSelect,Param[0]);
          }
          else {
            /* Number of parameters not accepted , */
             CmdReady = 0;
          }
      }
   }
   else {
      /* Test for Char of Type Numeric */
      if ( CharType == N)
         Param[0] = 10*Param[0] + ( ReceiveChar - '0');
   }
}


/***************************************************************************/
/*   MADI_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*   Function:   Receive general master diagnostics, (non destructive).    */
/*   Syntax:     void MADI_Command();                                      */
/*   Remarks:    RS232-command: MADI  T.B.D;                               */
/*   Returns:    nothing                                                   */
/***************************************************************************/
void MADI_Command()  {
   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(MADI_tx);
      }
      else {
          CmdReady = 0;
      }
   }
}

/***************************************************************************/
/*   QPAL_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*   Function:   Receive QPSK level alarm setting.                         */
/*   Syntax:     void QPAL_Command();                                      */
/*   Remarks:    RS232-command: QPAL<l>;                                   */
/*   Returns:    nothing                                                   */
/***************************************************************************/
void QPAL_Command()  {

   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(QPAL_tx);
      }

      /* Command */
      if (ReceiveDel == '$') {
         /* Test for number of parameters */
          if ( ParamNo == 1) {
             iUpdate(Input_alarm_enable,Param[0]);
          }
          else {
            /* Number of parameters not accepted , */
             CmdReady = 0;
          }
      }
   }
   else {
      /* Test for Char of Type Numeric */
      if ( CharType == N)
         Param[0] = 10*Param[0] + ( ReceiveChar - '0');
   }
}


/***************************************************************************/
/*   QPRE_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*   Function:   Receive QPSK level preset acceptance.                     */
/*   Syntax:     void QPRE_Command();                                      */
/*   Remarks:    RS232-command: QPRE<l>;                                   */
/*   Returns:    nothing                                                   */
/***************************************************************************/
void QPRE_Command()  {

   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(QPRE_tx);
      }

      /* Command */
      if (ReceiveDel == '$') {
         /* Test for number of parameters */
          if ( ParamNo == 0) {
             iUpdate(InputLev_ref,Measured.InputLevel);
          }
          else {
            /* Number of parameters not accepted , */
             CmdReady = 0;
          }
      }
   }
   else {
      /* Parse , nothing to do */
   }
}

/***************************************************************************/
/*   SNDM_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*   Function:   Receive actual sound mode.                                */
/*   Syntax:     void SNDM_Command();                                      */
/*   Remarks:    Not a master receiver                                     */
/***************************************************************************/
void SNDM_Command()  {
    CmdReady = 0;
}

/***************************************************************************/
/*   SVER_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950525                                                     */
/*   Revised:   950525                                                     */
/*   Function:   Receive master software versions or request from master   */
/*             to transmit front plate software version.                   */
/*   Syntax:     void SVER_Command();                                      */
/*   Remarks:    RS232-command: SVER T.B.D;                                */
/*                              SVER?                                      */
/*   Returns:    nothing                                                   */
/***************************************************************************/
void SVER_Command()  {
   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(SVER_tx);
      }
      else {
          CmdReady = 0;
      }
   }
}

/***************************************************************************/
/*   TEST_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950621                                                     */
/*   Revised:   950621                                                     */
/*   Function:   Front requests number of tests available and              */
/*               selects test-number ( 0 means end of test mode )          */
/***************************************************************************/
void TEST_Command()  {
   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(TEST_tx);
      }
      /* Command */
      if (ReceiveDel == '$' || ReceiveDel == ';') {
         /* Test for number of parameters */
          if ( ParamNo == 1) {
             /* Do whatever required with Param[0] */
             if (Param[0] <= NumOfTests ) {
                 Measured.TestMode = Param[0];

             }
             else {
                 Measured.TestMode = NoTests;
             }
          }
          else {
            /* Number of parameters not accepted , */
             CmdReady = 0;
          }
      }
   }
   else {
      /* Parse */
      /* Test for Char of Type Numeric */
      if ( CharType == N)
         Param[0] = 10*Param[0] + ( ReceiveChar - '0');
   }


}

/***************************************************************************/
/*   TRES_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950621                                                     */
/*   Revised:   950621                                                     */
/*   Function:   Transmit test results to front panel                      */
/***************************************************************************/
void TRES_Command()  {
}

/***************************************************************************/
/*   FACT_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950621                                                     */
/*   Revised:   950621                                                     */
/*   Function:   Request factory setting.                                  */
/*               Response to front after factory restore.                  */
/***************************************************************************/
void FACT_Command()  {
   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(FACT_tx);
      }
      else {
          CmdReady = 0;
      }
   }
}

/***************************************************************************/
/*   PRST_Command                                                          */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950621                                                     */
/*   Revised:   950621                                                     */
/*   Function:   Request preset setting.                                   */
/*               Response to front after preset restore.                   */
/***************************************************************************/
void PRST_Command()  {
   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(PRST_tx);
      }
      else {
          CmdReady = 0;
      }
   }
}

/***************************************************************************/
/*   GO_Command                                                            */
/*                                                                         */
/* Written by:      SAA                                                    */
/*   Date:      950621                                                     */
/*   Revised:   950621                                                     */
/*   Function:   Front requests parameters to start (after power on)       */
/*               Response to front after parameter transfer                */
/***************************************************************************/
void GO_Command()  {
   /* test for parse or execute (bit CmdReady = '1' for execute ) */
   if ( CmdReady) {
      /* Execute */
      /* Request */
      if (ReceiveDel == '?') {
          Request(GO_tx);
      }
      else {
          CmdReady = 0;
      }
   }
}
/**/
/***************************************************************************/
/***************************************************************************/
/* Defines which defines tables etc. in this module */
#define No_Of_Commands  26
#define No_In_Alphabet  26

/* Tables required from low-level :*/

/* Table with addresses of all command functions : */

code const struct FUNCTION_CALL Command[No_Of_Commands] = {
   /*  0 */      ALST_Command,
   /*  1 */      ATT_Command,
   /*  2 */      AULA_Command,
   /*  3 */      AULE_Command,
   /*  4 */      BER_Command,
   /*  5 */      BERS_Command,
   /*  6 */      BRST_Command,
   /*  7 */      EHS_Command,
   /*  8 */      EYE_Command,
   /*  9 */      FACT_Command,
   /* 10 */      FRDI_Command,
   /* 11 */      GO_Command,
   /* 12 */      GTLO_Command,
   /* 13 */      GTRE_Command,
   /* 14 */      HCON_Command,
   /* 15 */      IEAD_Command,
   /* 16 */      IFCA_Command,
   /* 17 */      IFIN_Command,
   /* 18 */      MADI_Command,
   /* 19 */      PRST_Command,
   /* 20 */      QPAL_Command,
   /* 21 */      QPRE_Command,
   /* 22 */      SNDM_Command,
   /* 23 */      SVER_Command,
   /* 24 */      TEST_Command,
   /* 25 */      TRES_Command,
};

/* Table with the spelling of each command.
   All entries are UPPERCASE but the command-interpreter is NOT case sensitive
   The table is not a table of strings but a table of 4 char for each command.
    */

code const uchar CommandTxt[ 4*No_Of_Commands] = {
   /*  0   */      'A', 'L', 'S', 'T',
   /*  1 */      'A', 'T', 'T', ' ',
   /*  2 */      'A', 'U', 'L', 'A',
   /*  3 */      'A', 'U', 'L', 'E',
   /*  4 */      'B', 'E', 'R', ' ',
   /*  5 */      'B', 'E', 'R', 'S',
   /*  6 */      'B', 'R', 'S', 'T',
   /*  7 */      'E', 'H', 'S', ' ',
   /*  8 */      'E', 'Y', 'E', ' ',
   /*  9 */      'F', 'A', 'C', 'T',
   /* 10 */      'F', 'R', 'D', 'I',
   /* 11 */      'G', 'O', ' ', ' ',
   /* 12 */      'G', 'T', 'L', 'O',
   /* 13 */      'G', 'T', 'R', 'E',
   /* 14 */      'H', 'C', 'O', 'N',
   /* 15 */      'I', 'E', 'A', 'D',
   /* 16 */      'I', 'F', 'C', 'A',
   /* 17 */      'I', 'F', 'I', 'N',
   /* 18 */      'M', 'A', 'D', 'I',
   /* 19 */      'P', 'R', 'S', 'T',
   /* 20 */      'Q', 'P', 'A', 'L',
   /* 21 */      'Q', 'P', 'R', 'E',
   /* 22 */      'S', 'N', 'D', 'M',
   /* 23 */      'S', 'V', 'E', 'R',
   /* 24 */      'T', 'E', 'S', 'T',
   /* 25 */      'T', 'R', 'E', 'S',
};

/**/
/* Fast lookup table with 2 parameters related to the above table
   for each letter in the alphabet. The first parameter is the number
   (index) in the table for the first command starting with the letter.
   The next parameter is 4 times the number of commands starting with
   the letter.
   */

code const uchar CommandNdx[2*No_In_Alphabet] = {
   /*  A */      0, 16,
   /*  B */      4, 12,
   /*  C */      0, 0,
   /*  D */      0, 0,
   /*  E */      7, 8,
   /*  F */      9,   8,
   /*  G */      11, 12,
   /*  H */      14, 4,
   /*  I */      15, 12,
   /*  J */      0,   0,
   /*  K */      0,   0,
   /*  L */      0, 0,
   /*  M */      18, 4,
   /*  N */      0,   0,
   /*  O */      0,   0,
   /*  P */      19, 4,
   /*  Q */      20, 8,
   /*  R */      0,   0,
   /*  S */      22, 8,
   /*  T */      24, 8,
   /*  U */      0,   0,
   /*  V */      0,   0,
   /*  W */      0,   0,
   /*  X */      0, 0,
   /*  Y */      0,   0,
   /*  Z */      0, 0,
};

