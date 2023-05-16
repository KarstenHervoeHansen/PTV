/************************************************************************/
/* File :       master.c                                                */
/* Create :     950501 Sten A.Andersen                                  */
/* Last Edit :  951029 Sten A.Andersen                                  */
/* Status :     Accepted                                                */
/* Version :    1.00                                                    */
/************************************************************************/
/* main module */
/* Start of file is correction log Since 950608 Montreux-model version 0.02 */
/* Log */
/* 950621 Adding setting of bits in 'TestMaster' byte for transmission
   of diagnostic data to frontpanel. */
/* 950621 Adding state machine construction to handle start and
          diagnostic sequencing **not tested**  */

/* 950626 Set unused alarm bit to not active (to front)*/
/* 950630 Testing different RESET methods for frontpanel */
/* 950703 Single LOW to activate PULL-up transient current in 80C31 port */
/* 950731 Ref. Change request 89014 . Change in Vis Carr Alarm detect.*/
/* 950816 Ref. Changerequest 89026 . Added Loop BW control. Pos: case 16 */
/* 950817 Changing slicer timing to more distruted AND to support
          much faster BER updating . Decoder access / BerCalc is done
          in all ODD time-slices . All other functions are placed in EVEN
          time slices. */
/* 950821 Adding Control of decoder in in special test-mode. Control is
          based on Measured.TestMode */
/* 950828 Ref.CR89020 Changing conditions for NO EYEHEIGHT AVAIL from QPSK
          lock to locking info from Eyeheight circuit */
/* 950829 Implement test of dataoption */

/* 950921 Changing use of GetIClevel AND GetIFgain according to
          PM5689 sh.160 KREDSLùBSBESKRIVELSE side 3 date : 950921
          and the new function GetVISsnd() from frontend-module.
          DK is now same level as BG & I (was like L). */
/* 951029 Removing VisCarr Alarm in system L ( Change request 89050 )
          Adding C4 as output to TTL connector ( C.R. 89055 )
          ver 1.00 */

/* Implement alarms to TTL connector */

/* End of log */

#include "Standard.def"   /* General definitions */
#include "Nicam89.def"   /* Instrument specific definitions */

/* Standard C & special 80c31 includes */
#include <absacc.h>
#include <reg51.h>                      /* define 8051 registers */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* Dummy declaration of putchar required for ?????? purpose */
putchar(char c) {
}

/***********************************************************/
/* Hardware definitions                                    */
/***********************************************************/
/* Include basic IO  */
#include <rs232ini.h>
#include <rs232snd.h>
#include <rem_hwi.h>
#include <rem_pars.h>
#include <rem_cmds.h>
#include <iic_int.h>
#include <iic_unit.h>
#include <ttl_port.h>
#include <audio.h>
#include <eyeheigh.h>
#include <frontend.h>
#include <decoder.h>
#include <dataopt.h>
#include <ber_calc.h>

/* Version definitions */
#include <version.def>

/* Instrument specific data type declarations */
#include <main_mem.h>

/* Defining hardware (Should be moved to a module) */
sbit    Unit_RESET     =   P1^5;
sbit    Front_RESET    =   P3^2;
sbit    Remote_RESET   =   P1^2;
sbit    Watch_dog_pin  =   P3^5;
sbit    Mute_NICAM     =   P3^4;

/* End page 1  Page 2 Master.c */
/***********************************************************/
/* Data define. Define as static to avoid external access  */
/***********************************************************/
/* Try using some memory : */
   UC Dummyuser[24];
/*  Define types and data */

UC  BERTimer;
xdata   UC      MODE;
xdata   UC      Code,x,y,peak,slicer,HighByte,LowByte;
bit             vision,Timer_flag,ChangeInput;
UC              TestMaster;
xdata   UC      Temp;

xdata   UI      AD_Channel[2];

/*End of data definitions*/

void TrigDog(){
    Watch_dog_pin = ~Watch_dog_pin;
}

Timer_int_Init()
{
char dummy;
    TMOD = 0x01 ;
    TCON = 0x10 ;
    PT0  = 0 ;
    EA   = 1 ;
    ET0  = 1 ;
}

Timer_int() interrupt 1 using 1
{
    TH0 = -1000000/256/100;  /* Load timer to apr. 100 Hz    */
    Timer_flag = 1;          /* Actual freq is :  100.16 Hz  */
    BERTimer++;
}

void IniSetup() {
   /* Check Actual setting */
   if ( Check(&Actual) != 0 ) {
      /* Error in actual : try Backup #0 */
      if( GetMem(&Backup[0]) != 0) {
          TestMaster |= TestMask_Memory;    /* Set Memory backup Error bi in testbyte */
          /* Error in Backup #0 : Use ROM -init */
          /* Direct writing, last write with 'Update' (for checksum */
          Actual.setup.Atten = 0;               /* Valid  0 , 20 (entry -1 21 rounded)*/
          Actual.setup.InputVersion = IF;       /*  */
          Actual.setup.InputSelect = IF;        /*  */
          Actual.setup.IF_Mode = Splitcarrier;  /* Interc. , Splitc. , */
          Actual.setup.Input_alarm_enable = FALSE; /* On , Off */
          Actual.setup.Audio_select = AA;       /* A , B , A/B */
          Actual.setup.Audio_level = -10;       /* -64 .. +6 */
          Actual.setup.FrontPort = 0xff;       /* */
          Actual.setup.BER_limit = 0x15;        /* 1e-2 - 1e-9 */
          Actual.setup.Eye_limit = 70;          /* 20-99 */
          Actual.setup.InputLev_ref = 27.0;
          Update(InputLev_ref,27.0);            /**/
      }
   }
   /* Test limits for all values */

}


void SetupTest() {
   /* Check Actual settings for allowed values */
   /* Some values depends on version etc. */
   /* Test limits for all values */

    if ( (Actual.setup.InputVersion != IF) &&
         (Actual.setup.InputVersion != IC) &&
         (Actual.setup.InputVersion != Dig)    ) {

          Actual.setup.InputVersion = IF;       /*  */
    }

    if (Actual.setup.InputVersion == Dig) {
          Actual.setup.InputSelect = Dig;
    }

    if (Actual.setup.InputVersion == IC) {
          Actual.setup.InputSelect = IC;
    }

    if ( (Actual.setup.InputSelect != IF) &&
         (Actual.setup.InputSelect != IC) &&
         (Actual.setup.InputSelect != Dig)    ) {

          Actual.setup.InputSelect = IF;       /*  */
    }

    Actual.setup.Options &= 0x07;   /* pt only 3 defined options */

    if ( Actual.setup.Input_alarm_enable != FALSE ) {
        Actual.setup.Input_alarm_enable = TRUE;
    }

}

/* End page 2  Page 3 Master.c */
main() {

/* Disable interupts */
    EA   = 0 ;
/* Reset all units */

    Timer_int_Init();

  /* Wait for POWER line to all units (100 msec) */

    x = 0;
    while(x++ < 10) {
        Timer_flag = 0;
        TrigDog();
        while (Timer_flag == 0 ) {
            Unit_RESET = 1;
            Front_RESET = 1;
            Remote_RESET = 1;
        }
    }

  /* Keep reset LOW to all units (100 msec)  (NOT FRONT !!!!)*/

    x = 0;
    while(x++ < 10) {
        Timer_flag = 0;
        TrigDog();
        while (Timer_flag == 0 ) {
            Unit_RESET = 0;
            Remote_RESET = 0;
        }
    }

    /* Single LOW to activate PULL-up transient current in 80C31 port */
    Front_RESET = 0;


    /* RESET  pulse 200 msec */
    x = 0;
    while(x++ < 20) {
        Timer_flag = 0;
        TrigDog();
        while (Timer_flag == 0 ) {
            Unit_RESET = 1;
            Front_RESET = 1;
            Remote_RESET = 1;
        }
    }



  /* Wait for units to init 100  msec*/
    x = 0;
    while(x++ < 10) {
        Timer_flag = 0;
        TrigDog();
        while (Timer_flag == 0 ) {
            Unit_RESET = 0;
            Remote_RESET = 0;
        }
    }


    TrxSTMinit();   /* Init transmission-part of front comm. */

    Serial_Init();      /*  Init hardware and receiver-part of front-comm. */

    Front_RESET = 0;


                       /* Bit 7 (128)  : Special test mode              */
                       /* Bit 6  (64)  : Self-test in progress          */
                       /* Bit 0   (1)  : external RAM NOT found / error */
                       /* Bit 1   (2)  : Checksum / data external RAM   */
                       /* Bit 2   (4)  : EEPROM not found / error       */
                       /* Bit 3   (8)  : Data / checksum error EEPROM   */
                       /* Bit 4  (16)  : Watchdog error                 */
                       /* Bit 5  (32)  : Error on one or more serial lines */

    TestMaster = TestMask_Testing;

    /* Test for TEST-JIG-Operation */

    /* HARDWARE SELF-TEST */

    /* Watchdog circuit test */

    /* RAM TEST  NOT destructive */

/* Memory manager */
    IniSetup();

    /* ROM TEST */

    /* I˝C TEST : Internal bus */
       if ( 0 != TestBus_Int() ) {
           /* Hardware problems with IIC bus (internal) */
          TestMaster |= TestMask_Ser_HW;    /* Set Serial HardWare
                                             Error bit in testbyte */
       }

    /* I˝C TEST : Unit bus */

       if ( 0 != TestBus_Unit() ) {
           /* Hardware problems with IIC bus (internal) */
          TestMaster |= TestMask_Ser_HW;    /* Set Serial HardWare
                                             Error bit in testbyte */
       }

    /* I˝C TEST : Remote bus */

    /* Serial bus test  */

    /* Test for hardware functions */

    /* Test for access to version-defining circuits */
       x = GetTVstandard();

       if( 0xff == x) {
          /* Error accessing the standard-defining circuit */
          TestMaster |= TestMask_Ser_Com;    /* Set Serial Communication
                                             Error bit in testbyte */
          /* Action on error is to use memory backup
              : do nothing ( later checksum is tested, fail => default )*/

       }
       else {
           Update(TV_Version ,x);
       }

       x = GetInputVersion();
       if( 0xff == x) {
          /* Error accessing the standard-defining circuit */
          TestMaster |= TestMask_Ser_Com;    /* Set Serial Communication
                                             Error bit in testbyte */
          /* Action on error is to use memory backup
              : do nothing ( later checksum is tested, fail => default )*/

       }
       else {
           Update(InputVersion ,x);
       }

    /* Audio circuit */
       if (0 !=  InitAudioControl()) {
           /* Access problems with audio circuit */
          TestMaster |= TestMask_Ser_Com;    /* Set Serial Communication
                                             Error bit in testbyte */
       }

    /* Decoder circuit */
       if (0 !=  InitDecoder()) {
           /* Access problems with decoder circuit */
          TestMaster |= TestMask_Ser_Com;    /* Set Serial Communication
                                             Error bit in testbyte */
       }

      /* Do not test for analog circuits in digital only version :*/

      if ( Actual.setup.InputVersion != VerDig ) {

        /* Eyeheight IIC circuit */
           if (0 !=  InitEyeheigh()) {
               /* Access problems with eyeheight IIC circuit */
           }

        /* Frontend IIC circuit */
           if (0 !=  InitFrontend()) {
               /* Access problems with eyeheight IIC circuit */
           }
       }

    /* Front communication test */

    RemoteInit();

    ParserInit();

    BerInit();

    slicer = 0;

    Mute_NICAM = 0;     /* Enable digital output */

/* End page 3  Page 4 Master.c */

    Measured.TestMode = 0;

    TestMaster &= ~TestMask_Testing;   /* Clear testing-bit from test-byte */

    while(1) {

        /* Remote control (IEEE488, RS232, TTL ,... ) */

        while (Timer_flag == 0 ) {
            if (2 == RemotePoll() ) {
                Code = PM91Maskine(RemoteGet());
                if (Code != 0) {
                    if(CmdParameters == 0 && CmdParam[0].Type == 0 ) {
                    }
                    else {
                        CmdParameters++;
                    }
                    Code = IEEExecute(Code);
                    /* Returns Error-code */
                    if ( Code != 0 ) {
                        StatReturn(Code);
                    }
                }
            }
        }

        Timer_flag = 0;

        TrigDog();

        /* Check for required comm to front, only transmit if ready
           for new command. Response to requests has higher priority
           than continous commands. */

        /* Module rs232snd.c */


        slicer++;
        if (slicer >= 30)
        {
            slicer = 0;
        }

        TrxSTM();

        switch (slicer) {

        case 1:
        case 3:
        case 5:
        case 7:
        case 9:
        case 11:
        case 13:
        case 15:
        case 17:
        case 19:
        case 21:
        case 23:
        case 25:
        case 27:
        case 29:

            if ( 0 == ReadDecoder() )
            {   Measured.SndStatus = GetDecoderStatus();
                BerCalc(   GetErrors()  );
            }
            else
            {   BerCalc( 0 );
            }


             /* Test for Frames Available ALARM  */

            if (    ( Measured.SndStatus & 0x10) != 0  )   {
                    /* Alarm */
                    if ((Measured.Alarms & AlarmMask_FRAME) == 0 ) {
                    /* Changed from no alarm */
                        AlarmsChanged = On;
                    }

                    Measured.Alarms |= AlarmMask_FRAME;

                    BerInit();

            }
            else {  /* Not alarm */
                     if ((Measured.Alarms & AlarmMask_FRAME) != 0 ) {
                    /* Changed from alarm */
                        AlarmsChanged = On;
                    }
                    Measured.Alarms &= ~AlarmMask_FRAME;
            }

             /* Test for BER ALARM  */

            if (    ( Measured.BER & 0x0f) < (Actual.setup.BER_limit & 0x0f)
                || ( ( Measured.BER & 0x0f) == (Actual.setup.BER_limit & 0x0f))
                   && (( Measured.BER & 0xf0) > (Actual.setup.BER_limit & 0xf0) )
                    )   {

                    /* Alarm */
                    if ((Measured.Alarms & AlarmMask_BER) == 0 ) {
                    /* Changed from no alarm */
                        AlarmsChanged = On;
                    }
                    Measured.Alarms |= AlarmMask_BER;
            }
            else {  /* Not alarm */
                     if ((Measured.Alarms & AlarmMask_BER) != 0 ) {
                    /* Changed from alarm */
                        AlarmsChanged = On;
                    }
                    Measured.Alarms &= ~AlarmMask_BER;
            }


            /* NICAM Decoder test-mode handling */
            if (( Measured.SndStatus & 0x80) != 0 )
            {
                switch (Measured.TestMode)
                {
                case 10:
                    DecoderTest(0x80,1000 * 2.048 );
                    DecoderTest(0x82, 0x8000 );
                    DecoderTest(0x83, 0x0000 );
                break;
                case 11:
                    DecoderTest(0x80,4000 * 2.048 );
                    DecoderTest(0x82, 0x8000 );
                    DecoderTest(0x83, 0x0000 );
                break;
                case 12:
                    DecoderTest(0x80,14000 * 2.048 );
                    DecoderTest(0x82, 0x8000 );
                    DecoderTest(0x83, 0x0000 );
                break;
                case 13:
                    DecoderTest(0x80,20 * 2.048 );
                    DecoderTest(0x82, 0x8000 );
                    DecoderTest(0x83, 0x0000 );
                break;

                case 14:
                    DecoderTest(0x81,1000 * 2.048 );
                    DecoderTest(0x83, 0x8000 );
                    DecoderTest(0x82, 0x0000 );
                break;
                case 15:
                    DecoderTest(0x81,4000 * 2.048 );
                    DecoderTest(0x83, 0x8000 );
                    DecoderTest(0x82, 0x0000 );
                break;
                case 16:
                    DecoderTest(0x81,14000 * 2.048 );
                    DecoderTest(0x83, 0x8000 );
                    DecoderTest(0x82, 0x0000 );
                break;
                case 17:
                    DecoderTest(0x81,20 * 2.048 );
                    DecoderTest(0x83, 0x8000 );
                    DecoderTest(0x82, 0x0000 );
                break;

                case 18:   /* Dual tone Setup 5kHz +/- 40 Hz */
                    DecoderTest(0x80,4960 * 2.048 );
                    DecoderTest(0x81,5040 * 2.048 );
                    DecoderTest(0x82, 0x4000 );
                    DecoderTest(0x83, 0x4000 );
                break;

                case 19:   /* Dual tone Activate */
                    DecoderTest(0x87, 0x8000 );
                break;

                case 20:  /* Dual tone Setup 10kHz +/- 40 Hz*/
                    DecoderTest(0x80,9960 * 2.048 );
                    DecoderTest(0x81,10040 * 2.048 );
                    DecoderTest(0x82, 0x4000 );
                    DecoderTest(0x83, 0x4000 );
                break;

                case 21:   /* Multi tone test */
                    DecoderTest(0x86, 0x8000 );
                break;

                }
            }

        break;

        case 0:
            /* IF-vision carr alarm ONLY in IF / Intercarrier mixing
               alarm if more than 3 dB outside specs */

            TestData.IC_lev = -10.0 * GetIClevel();

            if ( (Actual.setup.InputSelect == IF) &&
                 (Actual.setup.IF_Mode == InterCarrier)  ) {

                if ( (Actual.setup.TV_Version == VerBG) ||
                     (Actual.setup.TV_Version == VerDK) ||
                     (Actual.setup.TV_Version == VerI )   ) {


                    if (   (GetVISsnd() < 14.0 )  || ( GetVISsnd() > 26.0 )  ) {

                        /* Alarm */
                        if ((Measured.Alarms & AlarmMask_VIS) == 0 ) {
                        /* Changed from no alarm */
                            AlarmsChanged = On;
                        }

                        Measured.Alarms |= AlarmMask_VIS;
                    }
                    else {  /* Not alarm */
                         if ((Measured.Alarms & AlarmMask_VIS) != 0 ) {
                        /* Changed from alarm */
                            AlarmsChanged = On;
                        }
                        Measured.Alarms &= ~AlarmMask_VIS;
                    }
                }
                else {  /*  L version : 951029 : NO Alarm (C.R.89050)*/
                    if ((Measured.Alarms & AlarmMask_VIS) != 0 ) {
                    /* Changed from alarm */
                        AlarmsChanged = On;
                    }
                    Measured.Alarms &= ~AlarmMask_VIS;
                }
            }
            else {
                if ((Measured.Alarms & AlarmMask_VIS) != 0 ) {
                /* Changed from alarm */
                    AlarmsChanged = On;
                }
                Measured.Alarms &= ~AlarmMask_VIS;
            }

            if ( Actual.setup.InputSelect == IC ) {
                Measured.InputLevel = GetIClevel();
            }
            else {
                Measured.InputLevel = GetIClevel() - GetIFgain();
            }

           /* Test Input level alarms */
           if ( (Actual.setup.Input_alarm_enable == TRUE ) &&
                (Actual.setup.InputSelect != Dig )              ) {
               if (Measured.InputLevel > Actual.setup.InputLev_ref + 2.0) {
                       /* Alarm */
                       if ((Measured.Alarms & AlarmMask_HIGH) == 0 ) {
                       /* Changed from no alarm */
                           AlarmsChanged = On;
                       }
                       Measured.Alarms |= AlarmMask_HIGH;
               }
               else {  /* Not alarm */
                        if ((Measured.Alarms & AlarmMask_HIGH) != 0 ) {
                       /* Changed from alarm */
                           AlarmsChanged = On;
                       }
                       Measured.Alarms &= ~AlarmMask_HIGH;
               }

               if (Measured.InputLevel < Actual.setup.InputLev_ref - 2.0) {
                       /* Alarm */
                       if ((Measured.Alarms & AlarmMask_LOW) == 0 ) {
                       /* Changed from no alarm */
                           AlarmsChanged = On;
                       }
                       Measured.Alarms |= AlarmMask_LOW;
               }
               else {  /* Not alarm */
                        if ((Measured.Alarms & AlarmMask_LOW) != 0 ) {
                       /* Changed from alarm */
                           AlarmsChanged = On;
                       }
                       Measured.Alarms &= ~AlarmMask_LOW;
               }
           }
           else {

               if ((Measured.Alarms & AlarmMask_LOW) != 0 ) {
               /* Changed from alarm */
                   AlarmsChanged = On;
               }
               Measured.Alarms &= ~AlarmMask_LOW;

               if ((Measured.Alarms & AlarmMask_HIGH) != 0 ) {
               /* Changed from alarm */
                   AlarmsChanged = On;
               }
               Measured.Alarms &= ~AlarmMask_HIGH;
           }

        break;


        case 2:
            CheckUpdate();

            SetupTest();

            PutMem(&Backup[0]);

            /* Set unused alarm to not active */

            Measured.Alarms &= ~AlarmMask_x3;

        break;


        case 4:

            /* If Eyeheight avail then calc eyes else set to zero */

            if (((TestData.EyeStatus & EyeMask_LOCK) == 0) ||
                 ( Actual.setup.InputSelect == Dig ) ) {
                    Measured.I_Eye = 0;
                    Measured.Q_Eye = 0;
            }
            else {
                if ( 0==ReadEyes(10) ) {
                    Measured.I_Eye = 0.5 + 100.0 / 256.0 * GetData1();
                    Measured.Q_Eye = 0.5 + 100.0 / 256.0 * GetData2();

                    TestData.I_Byte = GetData1();
                    TestData.Q_Byte = GetData2();
                }
            }

            /* Test for Eyeheight alarm */

            if( Actual.setup.InputSelect != Dig ) {

                if (    ( Measured.I_Eye < Actual.setup.Eye_limit )
                    ||  ( Measured.Q_Eye < Actual.setup.Eye_limit ) ) {
                        /* Alarm */
                        if ((Measured.Alarms & AlarmMask_EYE) == 0 ) {
                        /* Changed from no alarm */
                            AlarmsChanged = On;
                        }
                        Measured.Alarms |= AlarmMask_EYE;
                }
                else {  /* Not alarm */
                         if ((Measured.Alarms & AlarmMask_EYE) != 0 ) {
                        /* Changed from alarm */
                            AlarmsChanged = On;
                        }
                        Measured.Alarms &= ~AlarmMask_EYE;
                }
            }
            else {
                 if ((Measured.Alarms & AlarmMask_EYE) != 0 ) {
                /* Changed from alarm */
                    AlarmsChanged = On;
                }
                Measured.Alarms &= ~AlarmMask_EYE;
            }

        break;


        case 8:
            SetAtten(Actual.setup.Atten);
        break;

        case 10:
            ResetAttenRelay();
        break;

        case 12:
            /* Split / interc select */
            if (Actual.setup.IF_Mode == Splitcarrier) {

                Update (FrontPort, Actual.setup.FrontPort | 0x40 );
            }
            else {
                Update (FrontPort, Actual.setup.FrontPort & 0xBF );

            }

                /* IC - IF Input select */
            if (Actual.setup.InputSelect == IF) {
                Update (FrontPort, Actual.setup.FrontPort & 0x7f );
                DecoderUse1();
            }
            else {
                if (Actual.setup.InputSelect == IC) {
                /*IC input*/
                    DecoderUse1();
                    Update (FrontPort, Actual.setup.FrontPort | 0x80 );
                }
                else {
                    DecoderUse2();
                }
            }

            /* LOOP BW pt. allways high ( pin = low ) */
            /* Change request 89026 : LOOP BW controlled directly from
               QPSK locking. */

            if ( GetQlock() != On )
            {
                Update (FrontPort, Actual.setup.FrontPort & ~0x20 );
            }
            else
            {
                Update (FrontPort, Actual.setup.FrontPort | 0x20 );
            }


            SetPort( Actual.setup.FrontPort | 0x1F );

            TestData.FrontPort = GetPort();

            if (Actual.setup.InputSelect != Dig)
            {   if ( GetQlock() != On )
                {   /* Alarm */
                    if ((Measured.Alarms & AlarmMask_QPSK) == 0 )
                    {   /* Changed from no alarm */
                        AlarmsChanged = On;
                    }
                    Measured.Alarms |= AlarmMask_QPSK;
                }
                else
                {   /* Not alarm */
                    if ((Measured.Alarms & AlarmMask_QPSK) != 0 )
                    {   /* Changed from alarm */
                        AlarmsChanged = On;
                    }
                    Measured.Alarms &= ~AlarmMask_QPSK;
                }
            }
            else
            {   if ((Measured.Alarms & AlarmMask_QPSK) != 0 )
                {   /* Changed from alarm */
                    AlarmsChanged = On;
                }
                Measured.Alarms &= ~AlarmMask_QPSK;
            }
        break;

        case 14:
            if ( 0==ReadEyes(11) ) {
                TestData.EyeStatus = GetData1();
            }
        break;

        case 18:
            if (0==ReadEyes(2)) {
                TestData.I_Sample = GetData1();
                TestData.Q_Sample = GetData2();
            }
        break;

        case 20:
            if(0==ReadEyes(1)) {
                TestData.I_MeanLow = GetData1();
                TestData.Q_MeanLow = GetData2();
            }
        break;

        case 22:
            if (0==ReadEyes(0)){
                TestData.I_MeanHigh = GetData1();
                TestData.Q_MeanHigh = GetData2();
            }
        break;

        case 24:

            if (Actual.setup.InputSelect != Dig)
            {   if ( GetQlock() != On )
                {   /* Alarm */
                    DecoderMute();
                }
                else
                {   /* Not alarm */
                    DecoderEnable();
                }
            }
            else
            {
                if ((Measured.Alarms & AlarmMask_FRAME) != 0 )
                {
                    DecoderMute();
                }
                else
                {
                    DecoderEnable();
                }
            }

            /* Access to otherwise unused functions */

            GetDataoptStat(1);

            GetAudioStat(1);

            GetEyeStat(1);

            TestData.BadCounter = GetDecoderStat(1);

                 /* Write ALARM & STATUS Info to TTL */

           Temp = 0;

           if ((Measured.Alarms & AlarmMask_BER) != 0 ) {
               Temp |= TTLMask_BER;
           }
           else {
               Temp &= ~TTLMask_BER;
           }

           if ((Measured.Alarms & AlarmMask_EYE) != 0 ) {
               Temp |= TTLMask_EYE;
           }
           else {
               Temp &= ~TTLMask_EYE;
           }

           if ((Measured.Alarms & AlarmMask_VIS) != 0 ) {
               Temp |= TTLMask_VIS;
           }
           else {
               Temp &= ~TTLMask_VIS;
           }

           if ((Measured.Alarms & AlarmMask_QPSK) != 0 ) {
               Temp |= TTLMask_QPSK;
           }
           else {
               Temp &= ~TTLMask_QPSK;
           }

           if ((Measured.Alarms & AlarmMask_FRAME) != 0 ) {
               Temp |= TTLMask_FRAME;
           }
           else {
               Temp &= ~TTLMask_FRAME;
           }

           if ((Measured.Alarms & AlarmMask_LOW) != 0 ) {
               Temp |= TTLMask_LOW;
           }
           else {
               Temp &= ~TTLMask_LOW;
           }

           if ((Measured.Alarms & AlarmMask_HIGH) != 0 ) {
               Temp |= TTLMask_HIGH;
           }
           else {
               Temp &= ~TTLMask_HIGH;
           }

            SetPort1(Temp);

            Temp = 0;
           if ((Measured.Alarms & AlarmMask_FRAME) == 0 ) {

                if ((Measured.SndStatus & 0x08) == 0x08) {
                    Temp |= TTLMask_C4;
                }
                switch ((Measured.SndStatus & 0x07 )) {

                case 7:
                case 6:
                case 5:
                case 4:
                    Temp |= TTLMask_NotDef;
                break;
                case 0:
                    Temp |= TTLMask_STEREO;
                break;
                case 1:
                    Temp |= TTLMask_MONO;
                break;
                case 2:
                    Temp |= TTLMask_DUAL;
                break;
                case 3:
                    Temp |= TTLMask_DATA;
                break;
                }
           }

            SetPort2(Temp);

        break;

        case 28:
            SetAudioSource(Actual.setup.Audio_select);

            SetAudioVolume(Actual.setup.Audio_level);

             /* Test for options */

             /* Data option check */

             if ( InitDataopt() == 0 )
             {
                 Actual.setup.Options |= OptionsMask_Data;
             }
             else
             {
                 Actual.setup.Options &= ~OptionsMask_Data;
             }

             /* Remote Option Check */

             if (InterfaceType() == 3 ){
                 Actual.setup.Options |= OptionsMask_RS232;
             }
             else {
                 Actual.setup.Options &= ~OptionsMask_RS232;
             }

             if (InterfaceType() == 0 ){
                 Actual.setup.Options |= OptionsMask_IEEE;
             }
             else {
                 Actual.setup.Options &= ~OptionsMask_IEEE;
             }

        break;

        default:
        break;
        }
    }
}

