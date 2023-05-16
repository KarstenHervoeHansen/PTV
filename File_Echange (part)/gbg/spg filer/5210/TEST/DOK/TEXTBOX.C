
#include <cvirte.h>    /* Needed if linking in external compiler; harmless otherwise */
#include <userint.h>
#include <formatio.h>
#include "textbox.h"

int count = 0;

int main (int argc, char *argv[])
{
    int panel;
    
    if (InitCVIRTE (0, argv, 0) == 0)    /* Needed if linking in external compiler; harmless otherwise */
        return -1;    /* out of memory */
    
    panel = LoadPanel(0, "textbox.uir", PANEL);
    DisplayPanel(panel);
    RunUserInterface();

    return 0;
}

int CVICALLBACK AddCallback(int panel, int control, int event,
                void *callbackData, int eventData1,
                int eventData2)
{
    char string[100];

    if (event == EVENT_COMMIT)
    {
        count++;
        if (count > 1)
            Fmt (string, "%s<%s%d%s", "This is line at index ", count-1, " added to the end of the existing text\n");
        else
            Fmt (string, "%s<%s", "This is the first line added to the box\n");
        SetCtrlVal (panel, PANEL_ACTIONBOX, string);
    }
    return 0;
}

int CVICALLBACK ClearCallback(int panel, int control, int event,
                  void *callbackData, int eventData1,
                  int eventData2)
{
    if (event == EVENT_COMMIT)
    {
        count = 0;
        ResetTextBox (panel, PANEL_ACTIONBOX, "");
    }
    return 0;
}

int CVICALLBACK DeleteCallback(int panel, int control, int event,
                    void *callbackData, int eventData1,
                    int eventData2)
{
    if (event == EVENT_COMMIT && count > 0)
    {
        count--;
        DeleteTextBoxLine (panel, PANEL_ACTIONBOX, 0);
    }
    return 0;

}

int CVICALLBACK InsertCallback(int panel, int control, int event,
                    void *callbackData, int eventData1,
                    int eventData2)
{
    if (event == EVENT_COMMIT)
    {
        count++;
        InsertTextBoxLine (panel, PANEL_ACTIONBOX, 0,
                       "This line is inserted at index 0 (at the top)");
    }
    return 0;
}

int CVICALLBACK QuitCallback(int panel, int control, int event,
                    void *callbackData, int eventData1,
                    int eventData2)
{
    if (event == EVENT_COMMIT)
        QuitUserInterface(0);
    return 0;
}
