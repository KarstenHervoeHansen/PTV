//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("mainform.cpp", PT5201Form);
USERES("PT52REM1.res");
USEFORM("about.cpp", AboutBox);
USEFORM("genlock.cpp", GenlockForm);
USEFORM("proper.cpp", PropertiesForm);
USEFORM("audio.cpp", AudioForm);
USEFORM("Colorbar.cpp", CBarForm);
USEFORM("status.cpp", SystemStatusForm);
USEFORM("apparatu.cpp", ApparatusSettingsForm);
USERES("handpnt.res");
USEUNIT("util.cpp");
USEUNIT("pt52class.cpp");
USEFORM("Black1.cpp", Black1Form);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->Title = "PT5210 PC Remote Control";
		Application->HelpFile = "U:\\Pt5201\\Work\\Ken\\PT5201 PC Remote Control\\help\\example1.hlp";
		Application->CreateForm(__classid(TPT5201Form), &PT5201Form);
		Application->CreateForm(__classid(TAboutBox), &AboutBox);
		Application->CreateForm(__classid(TGenlockForm), &GenlockForm);
		Application->CreateForm(__classid(TPropertiesForm), &PropertiesForm);
		Application->CreateForm(__classid(TAudioForm), &AudioForm);
		Application->CreateForm(__classid(TCBarForm), &CBarForm);
		Application->CreateForm(__classid(TSystemStatusForm), &SystemStatusForm);
		Application->CreateForm(__classid(TApparatusSettingsForm), &ApparatusSettingsForm);
		Application->CreateForm(__classid(TBlack1Form), &Black1Form);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------
