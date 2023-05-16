/*
  Program til overvagning af TrueTime GPS-modtager XL-DC i FMS.
  
  Programmet kommunikerer med XL-DC via PC COM-port #8 (9600 baud,8 bit,2 stop ,none par)
  Der leses XL-DC status for: antenne, PLL-status og GPS-status.
  Status leses med Serial Function F72 (manual see section 3-272, page 3-67)
  
  Status skrives til fil paa LAN (se GPSStatusFile) hver 2. minut (hele minuttal)
  
  
  PF, 980812

06-29-2004
10:24
XLDC: ANT 1 PLL 1 GPS 1

  
  JK, 08.03.01: Modified for new paths for DK-Audio net
  JK, 29.06.04: Speeded up.
*/

#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <rs232.h>
#include <userint.h>

#define AT232   1

#define CTRL_C  3
#define LF     10
#define CR     13

const int   XLDCCOM = 8;    // TrueTime GPS receiver XL-DC i test rack   COM Port#

char GPSStatusFile[50] = {"l:\\produk\\Masterdata\\5230\\gpsstat.dat"};


/* Protos */
char *ReadlnCOMPort(int, char *, double);
void WriteCOMPort(int, char);
void WriteCOMPortTerm(int, char *,int);
int  WriteXLDCStatusToFile(int, int, int);
void ReadXLDCFaultStatus(int *, int *, int *, char *);

int main (void)
  {
  int   antst;       // antenna status
  int   pllst;       // pll status
  int   gpsst;       // gps status
  int   minute;
  char  *tt;
  char  minutestr[5];
  char  buff[100];    


#if AT232       
  if (OpenComConfig (XLDCCOM, "com8", 9600, 0, 8, 2, 500, 500) != 0)
    {
    MessagePopup (" Kan ikke abne COM8 port", " Afslut program - sluk  PC - start igen");
    return 1;
    }

  WriteCOMPortTerm(XLDCCOM, "",    CTRL_C);   // stop sending time information (see nmanual 3-199)
  WriteCOMPortTerm(XLDCCOM, "F72", CR);       // dummy write/read to avoid ERROR 05 message
  ReadlnCOMPort(XLDCCOM, buff, 2.0);
#endif

  while (1)
    {                                    // loop forever until PC is switched off
    tt = TimeStr();                      // get system time
    CopyString(minutestr, 0, tt, 3, 2);  // get minutes string
    Scan(minutestr,"%s>%i",&minute);     // scan minutes string into integer

    if ((minute % 2) == 0)               // check for even minutes
      {
      ReadXLDCFaultStatus(&antst, &pllst, &gpsst, buff);  
      WriteXLDCStatusToFile (antst, pllst, gpsst);
      }
    ProcessSystemEvents();
    } // while
  CloseCom(XLDCCOM);
  return 0;
  }


int WriteXLDCStatusToFile(int ant, int pll, int gps)
  {
  int  bytes_written,
       fhandle,
       fileerr;
  char *tt,
       *dd,
       timestr[10],
       datestr[11],
       wrtbuf[40];

  tt = TimeStr();                       // get system time
  CopyString(timestr, 0, tt, 0,  5);    // get hour+minutes string
  dd = DateStr();                       // get system date
  CopyString(datestr, 0, dd, 0, -1);    // get date string
  
  fhandle = OpenFile (GPSStatusFile, VAL_WRITE_ONLY, VAL_TRUNCATE, VAL_ASCII);
  
  Fmt(wrtbuf,"%s<%s\n%s\n",datestr, timestr);    
  bytes_written = WriteFile (fhandle, wrtbuf, strlen(wrtbuf));
  
  Fmt(wrtbuf,"%s<XLDC: ANT %i PLL %i GPS %i\n", ant, pll, gps);    
  bytes_written = WriteFile (fhandle, wrtbuf, strlen(wrtbuf));
  
  fileerr = CloseFile (fhandle);
  return (fileerr != -1);   
  }


void ReadXLDCFaultStatus(int *ant, int *pll, int *gps, char *buffer)
  {
  char *strread;
  char StatusStr[50];
  
  WriteCOMPortTerm(XLDCCOM, "F72", CR);          // see 3-272
  
  strread = ReadlnCOMPort(XLDCCOM, buffer, 2.0);
  CopyString(StatusStr, 0, strread, 0, strlen(strread)); 
   
  *ant = (FindPattern (StatusStr, 0, -1, "Antenna: OK", 1, 1) > 0);
  *pll = (FindPattern (StatusStr, 0, -1, "PLL: OK",     1, 1) > 0);
  *gps = (FindPattern (StatusStr, 0, -1, "GPS: Locked", 1, 1) > 0);
  }


void WriteCOMPortTerm(int PortNo, char *TStr, int Term)
  {
  // send TransmitStr terminated with Term (CR or LF)
  int NoOfBytes;
  char TransmitStr[50];
  
   Fmt(TransmitStr, "%s<%s%c", TStr, Term); 
  
   NoOfBytes = StringLength (TransmitStr);
   ComWrt (PortNo, TransmitStr, NoOfBytes);
  }

char *ReadlnCOMPort(int PortNo, char * ASCIIdata, double Timeout)
  {
  int bytes_read;

  SetComTime (PortNo, Timeout);   // set timeout
  ASCIIdata[0] = 0x0;
  bytes_read = ComRdTerm (PortNo, ASCIIdata, 100, LF);   //terminates on LF
  if (bytes_read > 0)
    {
    if (ASCIIdata[bytes_read - 1] == CR)
      ASCIIdata[bytes_read - 1] = 0x00;
    }  
  else
    {
    ASCIIdata[0] = 0x30;
    ASCIIdata[1] = 0x00;    // '0' + NULL
    }
  return (&ASCIIdata[0]);
  }
