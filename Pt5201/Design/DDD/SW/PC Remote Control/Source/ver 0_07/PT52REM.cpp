//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USERES("PT52REM.res");
USEFORM("report.cpp", SystemReportForm);
USEFORM("System.cpp", SystemForm);
USEFORM("about.cpp", AboutBox);
USEFORM("audio.cpp", AudioForm);
USEFORM("Black1.cpp", Black1Form);
USEFORM("black2.cpp", Black2Form);
USEFORM("black3.cpp", Black3Form);
USEFORM("Colorbar.cpp", CBarForm);
USERES("handpnt.res");
USEFORM("mainform.cpp", PT5201Form);
USEFORM("preset.cpp", PresetForm);
USEFORM("proper.cpp", PropertiesForm);
USEUNIT("pt52class.cpp");
USEUNIT("util.cpp");
USEFORM("genlock.cpp", GenlockForm);
USEUNIT("comm.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->Title = "PT5210 PC Remote Control";
		Application->HelpFile = "U:\\Pt5201\\Work\\Ken\\PT5201 PC Remote Control\\help\\example1.hlp";
		Application->CreateForm(__classid(TPT5201Form), &PT5201Form);
		Application->CreateForm(__classid(TSystemReportForm), &SystemReportForm);
		Application->CreateForm(__classid(TSystemForm), &SystemForm);
		Application->CreateForm(__classid(TAboutBox), &AboutBox);
		Application->CreateForm(__classid(TAudioForm), &AudioForm);
		Application->CreateForm(__classid(TBlack1Form), &Black1Form);
		Application->CreateForm(__classid(TBlack2Form), &Black2Form);
		Application->CreateForm(__classid(TBlack3Form), &Black3Form);
		Application->CreateForm(__classid(TCBarForm), &CBarForm);
		Application->CreateForm(__classid(TPresetForm), &PresetForm);
		Application->CreateForm(__classid(TPropertiesForm), &PropertiesForm);
		Application->CreateForm(__classid(TGenlockForm), &GenlockForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------
