//---------------------------------------------------------------------------
#include <vcl\condefs.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dir.h>

#pragma hdrstop

#include <vcl\Classes.hpp>

//---------------------------------------------------------------------------
USERES("PT5201loader.res");
//---------------------------------------------------------------------------
#define UPLOADCOM "*"
#define DELIMITER '\n'
#define IDCOM "IDN?\r\n"
#define ERASECOM "FBER 2\n"

typedef struct
{
	HANDLE Port;
	char *File;
	unsigned long BWTN;
	bool Finished;
} ParamType;

TStringList *InitialConfig;
AnsiString CurPath;

bool CheckCommAnswer( HANDLE Port, char *TX, char *RX);
bool EraseDevice( HANDLE Port);
bool UploadFile( bool erase, AnsiString &FileName, AnsiString &Port);
AnsiString SendCommand( HANDLE Port, AnsiString &Command);
void ASendFile( PVOID pvoid);
void ARecFile( PVOID pvoid);

int main( int argc, char **argv)
{
	AnsiString tmp;
  bool UploadResult, validfile = false;
	InitialConfig = new TStringList();

	switch ( argc)
  {
  	case 1:
			cout << "Filename is missing!\n";
      break;

    case 2:
			try
		  {
				InitialConfig->LoadFromFile( argv[1]);
		  }
		  catch( EFOpenError &E)
		  {
				cout << ( AnsiString( argv[1]) + " not found!!\n");
				break;
			}

      validfile = true;
    	break;

    default:
			cout << "Too many arguments!\n";
			break;
  }

	if ( !validfile)
  {
		delete InitialConfig;

		cout << "Press any key to end\n";

	  while ( !kbhit())
	   	;

		return( -1);
  }

 	cout << ( "Processing file: " + InitialConfig->Values["File"] + " ");

	UploadResult = UploadFile( true, InitialConfig->Values["File"], InitialConfig->Values["Port"]+":");

	int i = 1;
	while ( UploadResult && InitialConfig->IndexOfName( tmp = ( "File" + AnsiString(i++))) != -1 )
  {
	 	cout << ( "Processing file: " + InitialConfig->Values[tmp] + " ");

		UploadResult = UploadFile( false, InitialConfig->Values[tmp], InitialConfig->Values["Port"]+":");
	}

	delete InitialConfig;

	cout << "The End!!!\n";
	cout << "Press any key to end\n";

	while ( !kbhit())
		;

	return( 0);
}
//---------------------------------------------------------------------------
bool UploadFile( bool erase, AnsiString &FileName, AnsiString &PortName)
{
	bool Status = false;
	TStringList *TextFil;
	DWORD ThreadID,Thread2ID;
	HANDLE ThreadHandle, Thread2Handle;
	ParamType Params, Params2;
	DCB NewDCB, OldDCB;
	COMMTIMEOUTS OldTMO, NewTMO;
	unsigned long N;
	AnsiString Reply;
	AnsiString DCBstr = "baud=115200 parity=N data=8 stop=2";
	HANDLE Port;

	if ( FileName == "None")
		return( false);

	TextFil = new TStringList();

	try
	{
		TextFil->LoadFromFile(FileName);
	}
	catch(EFOpenError &E)
  {
		cout << ( "File: " + FileName + " missing,\r\nunable to upload.\n");
  	goto Finish2;
	}

	Port = CreateFile( PortName.c_str(), GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	if ( Port == INVALID_HANDLE_VALUE)
	{
  	cout << ( "Port: " + PortName + " is unconnectable.\r\nMaybe in use by other application\n");
		goto Finish2;
	}

	GetCommState( Port, &OldDCB);
	BuildCommDCB( DCBstr.c_str(), &NewDCB);
	SetCommState( Port, &NewDCB);

	GetCommTimeouts( Port,& OldTMO);
	NewTMO.ReadIntervalTimeout = 15;
	NewTMO.ReadTotalTimeoutMultiplier = 1;
	NewTMO.ReadTotalTimeoutConstant = 250;
	NewTMO.WriteTotalTimeoutMultiplier = 1;
	NewTMO.WriteTotalTimeoutConstant = 250;
  SetCommTimeouts( Port, &NewTMO);

	Reply = SendCommand( Port, IDCOM);
	Reply.SetLength( 8);

	if ( Reply != "PTV,PT52")
	{
		cout << "Device does not respond.\n";
		goto Finish;
	}

	if ( erase)
  {
		if ( !EraseDevice( Port))
		{
			cout << "Erase of device failed.\n";
			goto Finish;
		}
  }

	N = TextFil->Text.Length();

	if ( SendCommand( Port, UPLOADCOM)!= "ENTERING LOADER STATE\r\n")
  {
	  cout << "Device does not respond.\n";
		goto Finish;
	}

  Params.BWTN = TextFil->Text.Length();
	Params.Port = Port;
  Params.File = new char[N+1];
  strcpy( Params.File, TextFil->Text.c_str());

  Params.Finished = false;
  Params2.BWTN = 0;
	Params2.Port = Port;
	Params2.Finished = false;

	ThreadHandle = CreateThread( 0, 1024, (LPTHREAD_START_ROUTINE)ASendFile, (void *)&Params, 0, &ThreadID);
	Thread2Handle = CreateThread( 0, 1024, (LPTHREAD_START_ROUTINE)ARecFile, (void *)&Params2, 0, &Thread2ID);

	if(( ThreadHandle == 0) ||( Thread2Handle == 0))
	{
		cout << "No thread handles available.\n";

		delete[] Params.File;
    goto Finish;
	}

	while( !Params.Finished)
  {
		sleep(3);
		cout << ".";
  }

	delete[] Params.File;

	Reply = SendCommand( Port, UPLOADCOM);
	Reply.SetLength( 7);

	sleep( 5);

  if ( Reply != "LEAVING")
	{
  	cout << "Upload failed.\n";
		goto Finish;
	}

	cout << " uploaded.\n";

	Status = true;

Finish:
	SetCommState( Port, &OldDCB);
	SetCommTimeouts( Port, &OldTMO);
	CloseHandle( Port);

Finish2:
	delete TextFil;

	return( Status);
}
//---------------------------------------------------------------------------
bool EraseDevice( HANDLE Port)
{
	char ch;
	unsigned long BWTN;
	AnsiString Command( ERASECOM);

	WriteFile( Port, Command.c_str(), Command.Length(), &BWTN, NULL);

	ReadFile( Port, &ch, 1, &BWTN, NULL);

	while( ch != '\n')
	{
		ReadFile( Port, &ch, 1, &BWTN, NULL);

		if ( ch == -1)
	   	return( false);				// Erase error
	}

	return( true);
}
//---------------------------------------------------------------------------
AnsiString SendCommand( HANDLE Port, AnsiString &Command)
{
	unsigned long BWTN;
  static char Buff[128];
	unsigned long RxResult;

  WriteFile( Port, Command.c_str(), Command.Length(), &BWTN,NULL);

	RxResult = ReadFile( Port, &Buff[0], sizeof(Buff), &BWTN, NULL);

	Buff[BWTN] = 0;

	if ( !RxResult || !BWTN)
		return( AnsiString(""));

  return( AnsiString( Buff));
}
//---------------------------------------------------------------------------
void ASendFile( PVOID pvoid)
{
	ParamType *ParamPtr;
	ParamPtr = ( ParamType *) pvoid;

	unsigned long N = ParamPtr->BWTN;

	WriteFile( ParamPtr->Port, ParamPtr->File, N, &( ParamPtr->BWTN), NULL);

	ParamPtr->Finished = true;

	ExitThread( 0);
}
//---------------------------------------------------------------------------
void ARecFile( PVOID pvoid)
{
	char ch[8];
	unsigned long N = 1;
	ParamType *ParamPtr;

	ParamPtr = (ParamType *) pvoid;
	ParamPtr->BWTN = 0;

	while ( N > 0)
	{
		ReadFile( ParamPtr->Port, &ch, sizeof( ch), &N, NULL);

		ParamPtr->BWTN += N;
	}

	ParamPtr->Finished = true;

	ExitThread( 0);
}
