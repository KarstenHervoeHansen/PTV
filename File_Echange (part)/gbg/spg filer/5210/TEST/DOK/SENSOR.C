#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <rs232.h>
#include <cvirte.h>		/* Needed if linking in external compiler; harmless otherwise */
#include <userint.h>
#include "sensor.h"
#include "cviutil.h"

static int sensPanel;
//static int Err232;
//int DUTCOM = 1;				// Device Under Test   COM Port#
//static char COMname[7][10] = {"com1","com2","","","com5","com6","com7"};

static char read_data[100];

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)	/* Needed if linking in external compiler; harmless otherwise */
		return -1;	/* out of memory */
	if ((sensPanel = LoadPanel (0, "sensor.uir", SENS_PANEL)) < 0)
		return -1;
	Err232 = OpenComConfig (DUTCOM, COMname[DUTCOM-1], 4800, 0, 8, 2, 512, 512);

	DisplayPanel (sensPanel);
	RunUserInterface ();
	
	
	
	return 0;
}

void DisplayRS232Errors(int PortNo, int Err)
{
   Beep();
   if (PortNo == 1)
     MessagePopup ("     RS232 Fejl     ", "  Timeout COM1");
     else
   if (PortNo == 2)
     MessagePopup ("     RS232 Fejl     ", "  Timeout COM2");
     else
   if (PortNo == 5)
     MessagePopup ("     RS232 Fejl     ", "  Timeout COM5");
     else
   if (PortNo == 6)
     MessagePopup ("     RS232 Fejl     ", "  Timeout COM6");
     else
   if (PortNo == 7)
     MessagePopup ("     RS232 Fejl     ", "  Timeout COM7");
}



/*
char *ReadCOMPort(int PortNo){

int bytes_read;
  SetComTime (PortNo, 1.0);   // set timeout
  read_data[0] = '\0';
  bytes_read = ComRdTerm (PortNo, read_data, 100, 13);   //terminate on 13=CR
  Err232 = ReturnRS232Err();
  if (Err232) 
    DisplayRS232Errors(PortNo,Err232);
   
 return (&read_data[0]);
} */





int CVICALLBACK QuitCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK IDCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
int bytes_send;
char *data[40];
char *pdata;

	switch (event) {
		case EVENT_COMMIT:
		
		FlushOutQ(DUTCOM);
		FlushInQ(DUTCOM);
		bytes_send = ComWrt (DUTCOM, "I?,", 3);
		while (GetOutQLen(DUTCOM)> 0);
		
		pdata = ReadCOMPort(DUTCOM,2.0);
		SetCtrlVal (sensPanel, SENS_PANEL_FROMSENSOR, pdata);
		SetCtrlVal (sensPanel, SENS_PANEL_FROMSENSOR, "\n");
		break;
	}
	return 0;
}

int CVICALLBACK STOPCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
		ComWrt (DUTCOM, "MS,", 3);
		while (GetOutQLen(DUTCOM)> 0);
		
		break;
	}
	return 0;
}

int CVICALLBACK MCCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
		ComWrt (1, "MX,MC,", 6);
		while (GetOutQLen(1)> 0);
		
		ReadCOMPort(DUTCOM,2.0);
		break;
	}
	return 0;
}
