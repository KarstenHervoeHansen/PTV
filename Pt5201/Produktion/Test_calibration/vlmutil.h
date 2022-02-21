
extern int vlm_StartLine,vlm_NoOfLines;
extern float vlm_StartPos, vlm_Delay;
extern int vlmPNL;

extern void RunVLMMeasurement(void);

extern int CVICALLBACK StartLineCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2); 

extern int CVICALLBACK NoOfLinesCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2);

extern int CVICALLBACK StartPosCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2);

extern int CVICALLBACK DelayCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2); 

