#include <ansi_c.h>
#include <formatio.h>
#include <utility.h>
#include <rs232.h>
#include <cvirte.h>		/* Needed if linking in external compiler; harmless otherwise */
#include <userint.h>
#include "vca.h"
#include "vcautil.h"
#include "cviutil.h"

static int mainPNL;
char *dataread;
int bytes_send;
float RLfreq[200];
float RLlevel[200];

int main (int argc, char *argv[]) {
	if (InitCVIRTE (0, argv, 0) == 0)	/* Needed if linking in external compiler; harmless otherwise */
		return -1;	/* out of memory */
	if ((mainPNL = LoadPanel (0, "vca.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (mainPNL);
	
	Err232 = OpenComConfig (DUTCOM, COMname[DUTCOM-1], 19200, 0, 8, 1, 3000, 100);
	
	
	RunUserInterface ();
	return 0;
}

int CVICALLBACK ExitCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK F1CallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
		vca_readid();
//		FlushInQ(DUTCOM);
//		WriteCOMPort(DUTCOM, "*IDN?\r\n");
// 		dataread = ReadlnCOMPort(DUTCOM,2.0);
		SetCtrlVal (mainPNL, PANEL_VCA_TEXTBOX, VCA_ID);
		SetCtrlVal (mainPNL, PANEL_VCA_TEXTBOX, "\n");

			break;
	}
	return 0;
}

int CVICALLBACK F2Callback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			vca_returnloss_variation(CURSOR);
			vca_returnloss_cursorfreq(250);  
			vca_returnloss_Xgain(50);
			vca_returnloss_Ygain(10);
			break;
	}
	return 0;
}

//extern void vca_set_returnloss(void);

int CVICALLBACK F3CallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
char ms[20];		
	switch (event) {
		case EVENT_COMMIT:
		vca_returnloss();
		vca_returnloss_readscale();

		break;
	}
	return 0;
}

int CVICALLBACK F4CallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
//int inQL;
char QL[5];
unsigned char *v;
int n;

	switch (event) {
		case EVENT_COMMIT:

//		vca_returnloss();
//		Delay(4);
//		vca_returnloss_readdata();
		
		//vca_returnloss();
		//Delay(4);
		vca_returnloss_readdata();
		
		Cls();
	/*	printf("mode %i\n",RL_data_mode);
		printf("length %i\n",RL_data_length);
		printf("start %f\n",RL_freq_start);
		printf("stop %f\n",RL_freq_stop);
		printf("delta %f\n",RL_freq_delta);
		for (n = 0; n < RL_data_length; n++)
			printf("%3i  freq= %5.1f   data= %f\n",n,RL_freq[n],RL_level[n]);
	*/
	//	XYGraphPopup ("D1 Return loss data", RL_freq, RL_level,
    //					  RL_data_length, VAL_FLOAT, VAL_FLOAT);
		PlotXY (mainPNL, PANEL_RL_GRAPH, RL_freq, RL_level, RL_data_length, VAL_FLOAT,
				VAL_FLOAT, VAL_FAT_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 2,
				VAL_RED);
	
		break;
	}
	return 0;
}

int CVICALLBACK RL_GraphCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
char Ydata[20];		
	switch (event) {
		case EVENT_VAL_CHANGED:
			GetAxisItem (mainPNL, PANEL_RL_GRAPH, VAL_LEFT_YAXIS, 20, Ydata, 0);
			SetCtrlVal (mainPNL, PANEL_RL_LEVEL, Ydata);
			break;
		case EVENT_COMMIT:
		    break;
	}
	return 0;
}
