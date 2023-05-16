 /*
 ; FILE NAME        flashUSB.c
 ; PURPOSE          To load hex file into 8051F320 flash using USB adapter.
 ;
 ; SPEC             Janusz Kuzminski
 ; DESIGN           Janusz Kuzminski
 ; CODING           Janusz Kuzminski
 ; CODE INSP.
 ; TEST             
 ;
 ; LAST UPDT:        4.08.2005
 ;                  20.04.2006: 2Wire option added for F320 processor (ver 2.0)
 ;
 ; NOTES:           ***************************************************************
 ;                  This program does not work with the newer version of siutil.dll
 ;                  It works with previous version of that dll, but, although
 ;                  programmed, the processor does not run.
 ;      :           ***************************************************************
 ;                  Invocation:
 ;                    flash interface hexfile
 ;                  
 ;                  This program uses DLLs suplied by Silabs (siutil.dll)
 ;                  which contain all necessary functions.
 ;                  Apnote AN117 from Silicon Laboratories describes all
 ;                  the functions used here.
 ;                  To compile use:
 ;                                 -P   (C++ compile)
 ;                                 -VF  (MFC compatible code)
 ;
 ;                  To link under BC5 you must use IMPLIB to convert 
 ;                  the DLL to Borland format.  The command is:
 ;                    IMPLIB -f jk.lib siutil.dll
 ;                  To run the program you need only siutil.dll in your
 ;                  path.
 ; TBD:             
*/
#include <iostream.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flash.h"

#define  UL unsigned long
#define  UI unsigned short
#define  UC unsigned char

#define  VERSION (float) 3.0

#define MOVING_PIN_SPEED 1

typedef struct tagxx
  {
  int    errcode;
  char * txt;
  } ERRCODES;

char *rotating_pin[] =  /* rotating pin: one step per 20 lines sent */
  {
  "|",
  "/",
  "-",
  "\\",
  NULL
  };


ERRCODES errarray[12] =
  {
   -3, "Flash Write Error",
   -2, "Target State Failure",
   -1, "Target State Failure",
    1, "File Not Found",
    2, "COM Port Error",
    3, "Download Error",
    4, "Reset Error",
    5, "Erase Error",
    7, "COM Port Close Error",
    8, "Invalid Parameter",
    9, "USB DLL missing",
   10, "USB Adapter Error"
  };

/*        Prototypes                */
void give_info(void);
void interpret(int);

short main(short argc, char ** argv)
  {
  int    second;
  int    result;
  string tmp;
  DWORD  cnt;
  string *pt;

  pt = &tmp;


  if (argc != 3)
    {
    give_info();
    exit(0);
    }

  second = atoi(argv[1]); /* interface */

  //cout << "Flash Version" << VERSION << "\n";

  printf("FlashUSB Version %.2f.\n", VERSION);


  printf("Counting USB DebugDevices....");
  result = USBDebugDevices(&cnt);
  if (result != 0)
    {
    printf("There is no USB device\n");
    interpret(result);
    return (1);
    }
  //cout << cnt << endl;
  if (cnt == 1)
    printf("There is %ld USB device\n", (long)cnt);
  else
    printf("There are %ld USB devices\n", (long)cnt);



  printf("Obtaining SN....");
  result = GetUSBDeviceSN(0, pt);
  if (result != 0)
    {
    interpret(result);
    return (1);
    }
  cout << tmp << endl;
  printf("SN %s\n", tmp.c_str());



  printf("Connecting USB....");
  result = ConnectUSB("",       // serial number
                       0,       // protocol: 0 - JTAG   1 - 2Wire
                       0,       // power:    0 - do not supply   1 - supply
                       1);      // disable dialog boxes: 0 - enable  1 - disable
  if (result != 0)
    {
    interpret(result);
    return (1);
    }

#if 0
  printf("Erasing....");
  result = FLASHEraseUSB("", 1, second);
  if (result != 0)
    {
    interpret(result);
    return (1);
    }
#endif

  printf("Downloading....");
  /* 
  Differences in prototypes:

  C++ Prototype: from flash.h:
                 Download(char * sDownloadFile, int nDeviceErase=0, 
                          int nDisableDialogBoxes=0, int nDownloadScratchPadSFLE=0, 
                          int nBankSelect=-1, int nLockFlash=0);

  C++ Prototype: from AN117:
                 Download(char* sDownloadFile, int nDeviceErase=0, 
                          int nDisableDialogBoxes=0, int nDownloadScratchPadSFLE = 0, 
                          int nBankSelect = -1);
  */
  result = Download(argv[2], //char * sDownloadFile
                          1, //int nDeviceErase=0
                          1, //int nDisableDialogBoxes=0
                          0, //int nDownloadScratchPadSFLE=0
                         -1, //int nBankSelect=-1
                          0);//int nLockFlash=0 ?????????????????????????
  if (result != 0)
    {
    interpret(result);
    return (1);
    }

  /* 23.06.2006: Set target GO added (version 3.0) */
  printf("Running...");
  result = SetTargetGo();
  if (result != 0)
    {
    interpret(result);
    return (1);
    }

  printf("Disconnecting.");
  result = DisconnectUSB();
  if (result != 0)
    {
    interpret(result);
    return (1);
    }

  return (0);
  }

void interpret(int err)
  {
  int i;

  for (i = 0; i < 12; i++)
    {
    if (errarray[i].errcode == err)
      {
      printf("%s\n", errarray[i].txt);
      return;
      }
    }
  printf("Unknown Error.\n");
  }

void give_info(void)
  {
  printf("FlashUSB: loads hex file into F320 processor via EC2 USB adapter.\n");
  printf("Version %.2f.\n", VERSION);
  printf("Invocation: flashUSB interface hexfile\n");
  printf(" where interface is 0 - JTAG, 1 - 2Wire.\n");
  }

