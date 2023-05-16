/************************************************************************/
/* File :       Rs232drv.c                                              */
/* Create :     950xxx Kim Engedahl                                     */
/* Last Edit :  950905 Sten A.Andersen                                  */
/* Status :     Accepted                                                */
/* Version :    0.02                                                    */
/************************************************************************/
/* Log of changes since Montreux */
/* 950621 importing new state-machine from KEn */
/* 950621 Reset statemachine in Serial_Init() */

/* End of log */
#include "Standard.def"   /* General definitions */
#include "Nicam89.def"   /* Instrument specific definitions */

/* include tables and command-routines required for operation */
#include "rs232cmd.h"

#include "rs232snd.h"

/* Define limits for different command-related parameters */
#define max_alpha      4
#define max_number    9
#define max_string    9
#define NoCommand      No_Of_Commands
#define No_In_ASCII    128

enum char_type  { A=0, N, D, S, Q, R };
enum state_type { STD_0=0, STD_1, STD_2, STD_3, STD_4, STD_5,
                           STD_6, STD_7, STD_8 };

#include <reg52.h>

code const uchar TypeTable[No_In_ASCII] = {
        /*              0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15    */
        /*   0 */       R, R, R, R, R, R, R, R, R, R, D, R, R, D, R, R,
        /*  16 */       R, R, R, R, R, R, R, R, R, R, R, R, R, R, R, R,
        /*  32 */       R, D, Q, R, D, R, R, R, R, R, R, N, S, N, N, R,
        /*  48 */       N, N, N, N, N, N, N, N, N, N, R, D, R, R, R, D,
        /*  64 */       R, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A,
        /*  80 */       A, A, A, A, A, A, A, A, A, A, R, R, R, R, R, R,
        /*  96 */       R, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A,
        /* 112 */       A, A, A, A, A, A, A, A, A, A, A, R, R, R, R, R
};

uchar CharType;

uchar CmdState;
uchar CharCount;

bit CmdReady;
uchar CmdNo;

uint CmdNdx;

uchar NoOfCmds;
uchar CheckSum;

uchar ParamNo;
uchar Param[4];
uchar ReceiveDel;

uchar ReceiveChar;
bit TransmitDone;

/***************************************************************************/
/*    Serial_Init                                                         */
/*                                                                        */
/* Written by:        Sten A. Andersen                                    */
/*    Date:        950523                                                 */
/*    Revised:    950523                                                  */
/*                                                                        */
/* Module:        RS232drv.C51                                            */
/*    Function:                                                           */
/*    Syntax:        void Serial_Init( void)                              */
/*    Returns:                                                            */
/***************************************************************************/
void Serial_Init( void)  {
      T2CON = 0x34;       /* 00110100 */
      RCAP2H = 65497 / 256;                /* Reload value = 65497 */
      RCAP2L = 65497 ;
      SCON = 0x50;        /* 10010000   Serial port mode 1 for 9615 kbit/sec */
      PCON = 0x00 ;       /* SMOD = 0; Not Bit-addressable */
      ES = 1;             /* Enables serial interrupt */
      PS = 1;             /* High prio. on serial interrupt  */
      CmdIndex = 0xff;
      TrxFree = 1;
      CmdState = STD_0;
}

/***************************************************************************/
/*    Serial_Interrupt                                                    */
/*                                                                        */
/* Written by:        Kim Engedahl, VDE                                   */
/*    Date:        950509                                                 */
/*    Revised:    950509                                                  */
/*                                                                        */
/* Module:        RS232.C51                                               */
/*    Function:                                                           */
/*    Syntax:        void Serial_Interrupt( void) interrupt 4 using 3     */
/*    Remarks:        DO NOT CALL THIS FUNCTION!!! IT'S AN INTERRUPT ONLY */
/*    Returns:                                                            */
/***************************************************************************/
/* Prototype command interpreter routine */
void Command_Interpreter(void);

void Serial_Interrupt( void) interrupt 4 using 3 {
static  uchar CkSum;

    if ( RI) {                            /* If receiver interrupt */
        ReceiveChar = SBUF;            /*  get the character and signal that */
                                            /*  a new character has been received. */
        if (ReceiveChar < 0x80 ) {
            Command_Interpreter();
        }
        else {
            if (ReceiveChar == 0x80){
                CheckOK();
            }
            if (ReceiveChar == 0x81){
                CheckError();
            }
            if (ReceiveChar == 0x82){
                CommandError();
            }
        }
        RI = 0;                            /* Reset the interrupt. */
    }
    if (TI) {
        /* Test for caracters to send */
        if(CmdIndex < 0x80) {

           if (CmdToSend[CmdIndex] != 0x00 ) {
              	SBUF = CmdToSend[CmdIndex];
               if (CmdIndex == 0) {
          	       CkSum = CmdToSend[CmdIndex++];
          	   }
          	   else {
          	       CkSum += CmdToSend[CmdIndex++];
          	   }
           }	
           else {
                   	SBUF = CkSum & 0x7f;
                   	CmdIndex = 0xff;
           }

        }
        else {
            if(CmdIndex == 0xff) {
                TrxFree = 1;
            }
            if(CmdIndex == 0xfd) {
               SBUF = ';';
               CmdIndex = 0;
            }
            if(CmdIndex == 0xfe) {
               SBUF = ';';
               CmdIndex = 0xfd;
            }
        }
        TI = 0;                        /* Reset the interrupt. */
    }
}

/***************************************************************************/
/*	TrxChar																						*/
/*																									*/
/* Author:		SAA          																*/
/*	Date:			950509																		*/
/*	Revised:		950522																		*/
/*																									*/
/* Module:		RS232.C51																	*/
/*	Function:	                                             					*/
/*	Syntax:		void TrxChar( char c);													*/
/*	Remarks:																						*/
/*	Returns:		Nothing;																		*/
/***************************************************************************/
void TrxChar( char c) using 3 {
   while ((!TI) && (!TrxFree)) {
   ;
   }
	SBUF = c;
	TI = 0;
   TrxFree = 0;
}

/***************************************************************************/
/*   Command_Interpreter                                                   */
/*                                                                         */
/* Author:      KEn, VDE                                                   */
/*   Date:         950425                                                  */
/*   Revised:      950525                                                  */
/*                                                                         */
/* Module:      RS232INT.C                                                 */
/*   Function:                                                             */
/*   Syntax:     void Command_Interpreter()                                */
/*   Remarks:                                                              */
/*   Returns:    Nothing                                                   */
/***************************************************************************/
void Command_Interpreter() using 3{
   uchar i, j;

   CharType = TypeTable[ReceiveChar];

   switch ( CmdState) {
      case STD_0:
         ParamNo = 0;
         Param[3] = Param[2] = Param[1] = Param[0] = 0;

         CmdReady = 0;
         CharCount = 0;
         CheckSum = ReceiveChar;

         CmdNo = NoCommand;

         if ( CharType == A) {
            CmdState++;

            ReceiveChar &= 0xDF;         /* Equ. toupper( ReceiveChar) */;

            i = 2 * ( ReceiveChar - 'A');

            CmdNdx = 4 * CommandNdx[i];
            NoOfCmds = CommandNdx[++i];

            if ( NoOfCmds == 0)
               CmdState = STD_7;
            return;
         }

         if ( CharType != D)
            CmdState = STD_7;
         return;

      case STD_1:
         CheckSum += ReceiveChar;

         if ( CharType == A) {

            CmdState = STD_7;

            if ( ++CharCount >= max_alpha)
               break;

            ReceiveChar &= 0xDF;         /* Equ. toupper( ReceiveChar */;

            CmdNdx += CharCount;

            for ( j = 0; j < NoOfCmds; j += 4) {
               if ( ReceiveChar == CommandTxt[ CmdNdx + j]) {

                  for ( i = j + 4; i < NoOfCmds; i += 4)
                     if ( ReceiveChar != CommandTxt[ CmdNdx + i])
                        break;

                  NoOfCmds = i - j;
                  CmdNdx = j + CmdNdx - CharCount;

                  CmdState = STD_1;
                  break;
               }
            }

            if ( CmdState == STD_7)
               return;
         }
         else {
            for ( i = CharCount+1; i < max_alpha; i++)
               if ( CommandTxt[ CmdNdx + i] != ' ') {
                  CmdState = STD_7;
                  return;
               }

            CmdNo = CmdNdx/4;

            switch ( CharType) {
               case N:
                  CmdState = STD_2;
                  ParamNo++;
                  CharCount = 1;
                  break;

               case D:
                  CmdState = STD_6;
                  break;

               case Q:
                  CmdState = STD_3;
                  ParamNo++;
                  CharCount = 1;
                  break;

               default:
                  CmdState = STD_7;
                  return;
            }
         }
         break;

      case STD_2:
         CheckSum += ReceiveChar;

         switch ( CharType) {
            case D:
               CmdState = STD_6;
               break;

            case S:
               CmdState = STD_5;
               ParamNo++;
               break;

            case A:
               ReceiveChar &= 0xDF;

               if ( ReceiveChar != 'E') {
                  CmdState = STD_7;
                  return;
               }                                 /* Break IS missing */

               CharType = N;

            case N:
               if ( ++CharCount > max_number) {
                  CmdState = STD_7;
                  return;
               }
               break;

            default:
               CmdState = STD_7;
               return;
         }
         break;

      case STD_3:
         CheckSum += ReceiveChar;

         if ( CharType == Q)
            CmdState++;
         else
            if ( ++CharCount > max_string) {
               CmdState = STD_7;
               return;
            }
         break;

      case STD_4:
         CheckSum += ReceiveChar;

         switch ( CharType) {
            case D:
               CmdState = STD_6;
               break;

            case S:
               CmdState++;
               ParamNo++;
               break;

            case Q:
               if ( ++CharCount > max_string) {
                  CmdState = STD_7;
                  return;
               }
               else
                  CmdState--;
               break;

            default:
               CmdState = STD_7;
               return;
         }
         break;

      case STD_5:
         CheckSum += ReceiveChar;

         switch ( CharType) {
            case N:
               CmdState = STD_2;
               CharCount = 1;
               break;

            case Q:
               CmdState = STD_3;
               CharCount = 0;
               break;

            default:
               CmdState = STD_7;
               return;
         }
         break;

      case STD_6:
         if ( ReceiveChar == ( CheckSum & 0x7F)) {

            if ( CmdNo != NoCommand) {

               CmdReady = TRUE;
               Command[CmdNo].Exec();

               if ( CmdReady)
                  TrxChar( 0x80);   /* Acknowledge */
               else
                  TrxChar( 0x82);   /* Acknowledge command NOT interpreted */
            }
         }
         else
            TrxChar( 0x81);         /* Acknowledge checksum error */

         CmdState = 0;
         return;

      case STD_7:
         if ( CharType == D)
            CmdState++;
         return;         

      case STD_8:
         CmdState = 0;
         return;         
   }

   if ( CmdState == STD_6)
      ReceiveDel = ReceiveChar;

   if ( CmdNo != NoCommand)
      Command[CmdNo].Exec();
}
