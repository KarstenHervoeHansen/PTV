#include <userint.h>
#include "xx.h"

static int panelHandle;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)	/* Needed if linking in external compiler; harmless otherwise */
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "xx.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	return 0;
}

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
