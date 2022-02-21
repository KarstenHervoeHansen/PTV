//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "TimingTrackBar.h"
//---------------------------------------------------------------------------
static inline TimingTrackBar *ValidCtrCheck()
{
	return new TimingTrackBar(NULL);
}
//---------------------------------------------------------------------------
__fastcall TimingTrackBar::TimingTrackBar(TComponent* Owner)
	: TTrackBar(Owner)
{
}
//---------------------------------------------------------------------------
namespace Timingtrackbar
{
	void __fastcall Register()
	{
		TComponentClass classes[1] = {__classid(TimingTrackBar)};
		RegisterComponents("Win95", classes, 0);
	}
}
//---------------------------------------------------------------------------
