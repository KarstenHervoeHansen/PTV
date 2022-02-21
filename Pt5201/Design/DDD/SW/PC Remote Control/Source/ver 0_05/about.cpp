//---------------------------------------------------------------------
#include <vcl\vcl.h>
#include <printers.hpp>
#include <shellapi.h>
#pragma hdrstop

#include "about.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TAboutBox *AboutBox;
//---------------------------------------------------------------------
__fastcall TAboutBox::TAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TAboutBox::WebLabelClick(TObject *Sender)
{
	ShellExecute( NULL, "open", "http://www.ptv.dk", NULL, NULL, SW_RESTORE);

	WebLabel->Font->Color = clRed;
}
//---------------------------------------------------------------------------
void __fastcall TAboutBox::emailLabelClick(TObject *Sender)
{
	ShellExecute( NULL, "open", "mailto:helpdesk@ptv.dk", NULL, NULL, SW_RESTORE);

	emailLabel->Font->Color = clRed;
}
//---------------------------------------------------------------------------
void __fastcall TAboutBox::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TAboutBox::FormShow(TObject *Sender)
{
	TMemoryStatus MS;
	OSVERSIONINFO info;
  long tmp = 0xFFFFFFFF;

  info.dwOSVersionInfoSize = sizeof( OSVERSIONINFO);
  GetVersionEx( &info);

  AboutMemo->Lines->Clear();
 	AboutMemo->Lines->Add( "System:");

	if ( info.dwMajorVersion < 4)
  	AboutMemo->Lines->Add( "    Microsoft NT 3.X");
	else
		switch ( info.dwPlatformId)
    {
			case VER_PLATFORM_WIN32s:
		  	AboutMemo->Lines->Add( "    Microsoft Win32s");
        break;

			case VER_PLATFORM_WIN32_WINDOWS:
				if (( info.dwMajorVersion > 4) || (( info.dwMajorVersion == 4) && (info.dwMinorVersion > 0)))
			  	AboutMemo->Lines->Add( "    Microsoft Windows 98");
				else
			  	AboutMemo->Lines->Add( "    Microsoft Windows 95");

        tmp = 0x0000FFFF;
        break;

			case VER_PLATFORM_WIN32_NT:
		  	AboutMemo->Lines->Add( "    Microsoft Windows NT");
        break;

    }

  AboutMemo->Lines->Add( "    " + IntToStr( info.dwMajorVersion) + "." +\
  													  IntToStr( info.dwMinorVersion) + "." +\
														  IntToStr( info.dwBuildNumber & tmp));
  AboutMemo->Lines->Add("");

	MS.dwLength = sizeof( MS);
	GlobalMemoryStatus( &MS);

	LPSTR lpMemLoad = new char[5];
	sprintf( lpMemLoad, "%d %%", MS.dwMemoryLoad);

 	AboutMemo->Lines->Add( "Memory:");
 	AboutMemo->Lines->Add( "    Physical memory :  " + FormatFloat(( AnsiString)"#,###' KB'", MS.dwTotalPhys/1024));
 	AboutMemo->Lines->Add( "    Memory in use    :  " + ( AnsiString) lpMemLoad);

	delete [] lpMemLoad;
}
//---------------------------------------------------------------------------
void __fastcall TAboutBox::PrintBitBtnClick(TObject *Sender)
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

		while ( AboutMemo->Lines->Count > TotalLines)
    {
	    APrinter->Canvas->TextOut( 100, YPos, AboutMemo->Lines->Strings[TotalLines]);

			Lines++;

      TotalLines++;
      YPos += TextHeight;
    }

    APrinter->EndDoc();
  }
}
//---------------------------------------------------------------------------
