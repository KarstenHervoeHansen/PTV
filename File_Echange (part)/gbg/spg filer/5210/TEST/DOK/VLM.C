
// Program VLM
// Controlling the Video Level Meter 7108 100 74411 via PC-DIO24 interface
// I/O address is defined in IOAddr
// 961021 PF


#include <hp34401a.h>
#include <cvirte.h>		/* Needed if linking in external compiler; harmless otherwise */
#include "vlm_uir.h"
#include <utility.h>
#include <ansi_c.h>
#include <userint.h>
#include <analysis.h>

static ViReal64 	meas;
static ViSession 	tmp1;
static ViStatus 	err;
static int 			panelVlm;

int 	IOAddr = 0x210;
int 	i=0;
char 	bytewritten;

void VLMdata(unsigned short xdata, unsigned short sel)
{
// bit 0: SCL    bit 1: SDA    bit 2..4: SEL 0..2   
unsigned char n,p;

	xdata = xdata << 6;
	p = 0xe0 + (sel * 4);	// control data bit 2,3,4  (bit 5,6,7=1)
	for (n = 0; n < 10; ++n)
	 {
	  if ((xdata & 0x8000) == 0)
	    p = p & 0xfd;		// set SDA
	  if ((xdata & 0x8000) == 0x8000)
	    p = p | 0x02;			
	  outp(IOAddr,p); ++p;
	  outp(IOAddr,p); --p;
	  outp(IOAddr,p);
	  
	  xdata = xdata << 1;
	  
	} // for
} // vlmdata


void SetVLM (unsigned short StartLine, 
			 unsigned short StopLine, 
			 unsigned short StartPos, 
			 unsigned short StopPos)
{
	VLMdata(StartLine,0);
	VLMdata(StopLine,1);
	VLMdata(StartPos,2);
	VLMdata(StopPos,3);
} // SetVLM





int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)	/* Needed if linking in external compiler; harmless otherwise */
		return -1;	/* out of memory */
	outp(IOAddr+3,128);


	if ((panelVlm = LoadPanel (0, "vlm_uir.uir", PANEL_VLM)) < 0)
		return -1;
		
	err = hp34401a_init ("GPIB::22", VI_ON, VI_ON, &tmp1);
	err = hp34401a_confTrig (tmp1, 0, VI_ON, 0.000001, 1, 1);
	err = hp34401a_conf (tmp1, 1, VI_OFF, 10, 1);

	
	DisplayPanel (panelVlm);
	RunUserInterface ();
	

	return 0;
}


int CVICALLBACK Shutdown (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			QuitUserInterface (0);
			break;
	}
																			 
	return 0;
}

int CVICALLBACK DC_Control (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
		
			
		
			err = hp34401a_singleMeas (tmp1, &meas);			 
			err = SetCtrlVal (panelVlm, PANEL_VLM_SHOWLEVEL, meas * 1000); 
		
		
			break;
	}
	return 0;
}


int CVICALLBACK SHOW (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:

			break;
	}
	return 0;
}


int CVICALLBACK VideoMeas_1 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			SetVLM (100,150,200,300);
			err = hp34401a_singleMeas (tmp1, &meas);			 
			err = SetCtrlVal (panelVlm, PANEL_VLM_SHOWLEVEL, meas * 1000); 

			break;
	}
	return 0;
}

int CVICALLBACK VideoMeas_2 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			SetVLM (100,150,200,300);
			err = hp34401a_singleMeas (tmp1, &meas);			 
			err = SetCtrlVal (panelVlm, PANEL_VLM_SHOWLEVEL, meas * 1000); 
			break;
	}
	return 0;
}


int CVICALLBACK VideoMeas_3 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			SetVLM (100,150,200,300);
			err = hp34401a_singleMeas (tmp1, &meas);			 
			err = SetCtrlVal (panelVlm, PANEL_VLM_SHOWLEVEL, meas * 1000); 
			break;
	}
	return 0;
}

int CVICALLBACK VideoMeas_4 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			SetVLM (100,150,200,300);
			err = hp34401a_singleMeas (tmp1, &meas);			 
			err = SetCtrlVal (panelVlm, PANEL_VLM_SHOWLEVEL, meas * 1000); 
			break;
	}
	return 0;
}

int CVICALLBACK VideoMeas_5 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			SetVLM (100,150,200,300);
			err = hp34401a_singleMeas (tmp1, &meas);			 
			err = SetCtrlVal (panelVlm, PANEL_VLM_SHOWLEVEL, meas * 1000); 
			break;
	}
	return 0;
}

int CVICALLBACK VideoMeas_6 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			SetVLM (100,150,200,300);
			err = hp34401a_singleMeas (tmp1, &meas);			 
			err = SetCtrlVal (panelVlm, PANEL_VLM_SHOWLEVEL, meas * 1000); 
			break;
	}
	return 0;
}

int CVICALLBACK SyncMeas (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event) {
		case EVENT_COMMIT:
			SetVLM (100,150,200,300);
			err = hp34401a_singleMeas (tmp1, &meas);			 
			err = SetCtrlVal (panelVlm, PANEL_VLM_SHOWLEVEL, meas * 1000); 
			break;
	}
	return 0;
}
