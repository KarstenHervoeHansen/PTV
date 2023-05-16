#include <formatio.h>
#include <gpib.h>
#include <utility.h>
#include <hp34401a.h>
#include "cviutil.h"
// Utility file for the Video Level Meter 7108 100 7441

// All CallBack functions from the 'vlm.uir' are implemented here
#include <userint.h>
#include "vlm.h"
#include "cviutil.h"

#define NOWAIT 0


int VlmDone;
int handle_returned, controlID_returned;
int vlmPNL;
int vlm_StartLine,vlm_NoOfLines;
float vlm_StartPos, vlm_Delay;
double mVmeas;
char LevelTxt[30];

void RunVLMMeasurements(void){

double meas;

 DisplayPanel (vlmPNL);
 SetCtrlVal (vlmPNL, VLM_PNL_NOOFLINES,vlm_NoOfLines);
 SetCtrlVal (vlmPNL, VLM_PNL_STARTLINE,vlm_StartLine);
 SetCtrlVal (vlmPNL, VLM_PNL_DELAY,vlm_Delay);
 SetCtrlVal (vlmPNL, VLM_PNL_STARTPOS,vlm_StartPos);
 

 if (CheckFrontRearTerminal(REARTERM34401) == FALSE)
  HidePanel(vlmPNL);
 

 
 VLMdata(0,4);					// AC coupled meter

 SetVLM (vlm_StartLine,vlm_StartLine + vlm_NoOfLines,
 		(int)vlm_StartPos * 10,((int)vlm_StartPos * 10) + ((int)vlm_Delay * 10));
 
 VlmDone = FALSE;
#if IEEEBUS 
 hp34401a_conf (hp34401, 1, VI_OFF, 10.0, 1);
#endif

    
 while  (VlmDone == FALSE) {
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  DisableBreakOnLibraryErrors();
  ErrIEEE = hp34401a_singleMeas (hp34401, &meas);			 
  EnableBreakOnLibraryErrors();
  mVmeas = meas * 1000 / VLMGain;
  SetCtrlVal (vlmPNL, VLM_PNL_METER, mVmeas);
  Fmt(LevelTxt,"%s<%f[p1]",mVmeas);
  SetCtrlVal (vlmPNL, VLM_PNL_LEVELTXT, LevelTxt); 
 } //while

	HidePanel (vlmPNL);
	
}





int CVICALLBACK StartLineCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {

	switch (event) {
		case EVENT_VAL_CHANGED:
		 GetCtrlVal (vlmPNL, VLM_PNL_STARTLINE,&vlm_StartLine);
		 GetCtrlVal (vlmPNL, VLM_PNL_NOOFLINES,&vlm_NoOfLines);
		 GetCtrlVal (vlmPNL, VLM_PNL_STARTPOS,&vlm_StartPos);
		 GetCtrlVal (vlmPNL, VLM_PNL_DELAY,&vlm_Delay);
		 
		 if((vlm_StartLine + vlm_NoOfLines) > 313) {
		  vlm_NoOfLines = 313 - vlm_StartLine - 0;
		  SetCtrlVal (vlmPNL, VLM_PNL_NOOFLINES,vlm_NoOfLines);
		 }
		 
		 //vlm_StartLine,vlm_StartLine+vlm_NoOfLines
		 SetVLM (vlm_StartLine,vlm_StartLine + vlm_NoOfLines,
		 		(int)(vlm_StartPos * 10),((int)(vlm_StartPos * 10)) + ((int)(vlm_Delay * 10)));

		 
		 
			break;
	}
	return 0;
}


int CVICALLBACK NoOfLinesCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_VAL_CHANGED:
		 GetCtrlVal (vlmPNL, VLM_PNL_STARTLINE,&vlm_StartLine);
		 GetCtrlVal (vlmPNL, VLM_PNL_NOOFLINES,&vlm_NoOfLines);
		 GetCtrlVal (vlmPNL, VLM_PNL_STARTPOS,&vlm_StartPos);
		 GetCtrlVal (vlmPNL, VLM_PNL_DELAY,&vlm_Delay);

		 if((vlm_StartLine + vlm_NoOfLines) > 313) {
		  vlm_StartLine = 313 - vlm_NoOfLines - 0;
		  SetCtrlVal (vlmPNL, VLM_PNL_STARTLINE,vlm_StartLine);
		 }

		 SetVLM (vlm_StartLine,vlm_StartLine + vlm_NoOfLines,
		 		(int)(vlm_StartPos * 10),((int)(vlm_StartPos * 10)) + ((int)(vlm_Delay * 10)));
		  
			break;
	}
	return 0;
}

int CVICALLBACK StartPosCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_VAL_CHANGED:
		 GetCtrlVal (vlmPNL, VLM_PNL_STARTLINE,&vlm_StartLine);
		 GetCtrlVal (vlmPNL, VLM_PNL_NOOFLINES,&vlm_NoOfLines);
		 GetCtrlVal (vlmPNL, VLM_PNL_STARTPOS,&vlm_StartPos);
		 GetCtrlVal (vlmPNL, VLM_PNL_DELAY,&vlm_Delay);

		 if((vlm_StartPos + vlm_Delay) > 64) {
		  vlm_Delay = 64 - vlm_StartPos - 0.1;
		  SetCtrlVal (vlmPNL, VLM_PNL_DELAY,vlm_Delay);
		 }
		 
		 SetVLM (vlm_StartLine,vlm_StartLine + vlm_NoOfLines,
		 		(int)(vlm_StartPos * 10),((int)(vlm_StartPos * 10)) + ((int)(vlm_Delay * 10)));

			break;
	}
	return 0;
}

int CVICALLBACK DelayCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_VAL_CHANGED:
		 GetCtrlVal (vlmPNL, VLM_PNL_STARTLINE,&vlm_StartLine);
		 GetCtrlVal (vlmPNL, VLM_PNL_NOOFLINES,&vlm_NoOfLines);
		 GetCtrlVal (vlmPNL, VLM_PNL_STARTPOS,&vlm_StartPos);
		 GetCtrlVal (vlmPNL, VLM_PNL_DELAY,&vlm_Delay);

		 if((vlm_StartPos + vlm_Delay) > 64) {
		  vlm_StartPos = 64 - vlm_Delay - 0.1;
		  SetCtrlVal (vlmPNL, VLM_PNL_STARTPOS,vlm_StartPos);
		 }
		 
		 SetVLM (vlm_StartLine,vlm_StartLine + vlm_NoOfLines,
		 		(int)(vlm_StartPos * 10),((int)(vlm_StartPos * 10)) + ((int)(vlm_Delay * 10)));
		  
			break;
	}
	return 0;
}



int CVICALLBACK DC_Control (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
int DC_state;		
	switch (event) {
		case EVENT_COMMIT:
			GetCtrlVal (vlmPNL, VLM_PNL_DC_ON_OFF,&DC_state);
			if (DC_state == 1) {
			 VLMdata(1, 4);		// DC coupled
			 SetCtrlAttribute (vlmPNL, VLM_PNL_METER, ATTR_LABEL_TEXT,"DC offset (mV)");
			 }
			else {
			 VLMdata(0, 4);		// AC coupled
			 SetCtrlAttribute (vlmPNL, VLM_PNL_METER, ATTR_LABEL_TEXT,"Video level (mV)");
			 }
			 
			break;
	}
	return 0;
}

int CVICALLBACK VideoMeas_6 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
		 SetVLM(vlm_StartLine,vlm_StartLine+vlm_NoOfLines,560,580);
		 vlm_StartPos = 56;
		 vlm_Delay = 58-56;
		 SetCtrlVal (vlmPNL, VLM_PNL_STARTPOS,vlm_StartPos);
		 SetCtrlVal (vlmPNL, VLM_PNL_DELAY,vlm_Delay);
			break;
	}
	return 0;
}

int CVICALLBACK VideoMeas_5 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
		 SetVLM(vlm_StartLine,vlm_StartLine+vlm_NoOfLines,480,500);
		 vlm_StartPos = 48;
		 vlm_Delay = 50-48;
		 SetCtrlVal (vlmPNL, VLM_PNL_STARTPOS,vlm_StartPos);
		 SetCtrlVal (vlmPNL, VLM_PNL_DELAY,vlm_Delay);
			break;
	}
	return 0;
}

int CVICALLBACK VideoMeas_4 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
		 SetVLM(vlm_StartLine,vlm_StartLine+vlm_NoOfLines,400,420);
		 vlm_StartPos = 40;
		 vlm_Delay = 42-40;
		 SetCtrlVal (vlmPNL, VLM_PNL_STARTPOS,vlm_StartPos);
		 SetCtrlVal (vlmPNL, VLM_PNL_DELAY,vlm_Delay);
			break;
	}
	return 0;
}

int CVICALLBACK VideoMeas_3 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
		 SetVLM(vlm_StartLine,vlm_StartLine+vlm_NoOfLines,310,330);
		 vlm_StartPos = 31;
		 vlm_Delay = 33-31;
		 SetCtrlVal (vlmPNL, VLM_PNL_STARTPOS,vlm_StartPos);
		 SetCtrlVal (vlmPNL, VLM_PNL_DELAY,vlm_Delay);
			break;
	}
	return 0;
}

int CVICALLBACK VideoMeas_2 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
		 SetVLM(vlm_StartLine,vlm_StartLine+vlm_NoOfLines,220,240);
		 vlm_StartPos = 20;
		 vlm_Delay = 24-22;
		 SetCtrlVal (vlmPNL, VLM_PNL_STARTPOS,vlm_StartPos);
		 SetCtrlVal (vlmPNL, VLM_PNL_DELAY,vlm_Delay);
			break;
	}
	return 0;
}

int CVICALLBACK VideoMeas_1 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
		 SetVLM(vlm_StartLine,vlm_StartLine+vlm_NoOfLines,100,120);
		 vlm_StartPos = 10;
		 vlm_Delay = 12-10;
		 SetCtrlVal (vlmPNL, VLM_PNL_STARTPOS,vlm_StartPos);
		 SetCtrlVal (vlmPNL, VLM_PNL_DELAY,vlm_Delay);
			break;
	}
	return 0;
}

int CVICALLBACK SyncMeas (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
		 SetVLM(vlm_StartLine,vlm_StartLine+vlm_NoOfLines,20,30);
		 vlm_StartPos = 2;
		 vlm_Delay = 3-2;
		 SetCtrlVal (vlmPNL, VLM_PNL_STARTPOS,vlm_StartPos);
		 SetCtrlVal (vlmPNL, VLM_PNL_DELAY,vlm_Delay);
			break;
	}
	return 0;
}

int CVICALLBACK QuitVlmCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
		VlmDone = TRUE;
			break;
	}
	return 0;
}
