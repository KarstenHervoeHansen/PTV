//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <registry.hpp>
#pragma hdrstop

#include "status.h"
#include "util.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TSystemStatusForm *SystemStatusForm;
//---------------------------------------------------------------------------
__fastcall TSystemStatusForm::TSystemStatusForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystemStatusForm::FormClose(TObject *Sender,
	TCloseAction &Action)
{
    SaveWindowPosition( SystemStatusForm, "SystemStatusWindow");
}
//---------------------------------------------------------------------------
void __fastcall TSystemStatusForm::FormShow(TObject *Sender)
{
	TMemoryStatus MS;
	OSVERSIONINFO info;
  long tmp = 0xFFFFFFFF;

  RestoreWindowPosition( SystemStatusForm, "SystemStatusWindow");

  info.dwOSVersionInfoSize = sizeof( OSVERSIONINFO);
  GetVersionEx( &info);

  Memo1->Lines->Clear();
 	Memo1->Lines->Add( "System:");

	if ( info.dwMajorVersion < 4)
  	Memo1->Lines->Add( "    Microsoft NT 3.X");
	else
		switch ( info.dwPlatformId)
    {
			case VER_PLATFORM_WIN32s:
		  	Memo1->Lines->Add( "    Microsoft Win32s");
        break;

			case VER_PLATFORM_WIN32_WINDOWS:
				if (( info.dwMajorVersion > 4) || (( info.dwMajorVersion == 4) && (info.dwMinorVersion > 0)))
			  	Memo1->Lines->Add( "    Microsoft Windows 98");
				else
			  	Memo1->Lines->Add( "    Microsoft Windows 95");

        tmp = 0x0000FFFF;
        break;

			case VER_PLATFORM_WIN32_NT:
		  	Memo1->Lines->Add( "    Microsoft Windows NT");
        break;

    }

  Memo1->Lines->Add( "    " + IntToStr( info.dwMajorVersion) + "." +\
  													  IntToStr( info.dwMinorVersion) + "." +\
														  IntToStr( info.dwBuildNumber & tmp));
  Memo1->Lines->Add("");


	MS.dwLength = sizeof( MS);
	GlobalMemoryStatus( &MS);

	LPSTR lpMemLoad = new char[5];
	sprintf( lpMemLoad, "%d %%", MS.dwMemoryLoad);

 	Memo1->Lines->Add( "Memory:");
 	Memo1->Lines->Add( "    Physical memory :  " + FormatFloat(( AnsiString)"#,###' KB'", MS.dwTotalPhys/1024));
 	Memo1->Lines->Add( "    Memory in use    :  " + ( AnsiString) lpMemLoad);

	delete [] lpMemLoad;
}
//---------------------------------------------------------------------------
void __fastcall TSystemStatusForm::PrintButtonClick(TObject *Sender)
{
	TPrinter *APrinter;

	if ( PrintDialog->Execute())
  {
		APrinter = Printer();

		int PageHeight = APrinter->PageHeight;
    int TextHeight = APrinter->Canvas->TextHeight( "M");

		int LinesPerPage = PageHeight/TextHeight;

		int Lines = 0;
    int TotalLines = 0;
    int YPos = 20;

    APrinter->BeginDoc();

		while ( Memo1->Lines->Count > TotalLines)
    {
	    APrinter->Canvas->TextOut( 100, YPos, Memo1->Lines->Strings[TotalLines]);

			Lines++;

      TotalLines++;
      YPos += TextHeight;
    }

    APrinter->EndDoc();
  }
}
//---------------------------------------------------------------------------
