{***********************************************************************}
{  SONY39.PAS                                                           }
{  Philips TV Test Equipment A/S   920914                               }
{  Peter Frederiksen, P-DPE                                             }

{  The program communicates with the color probe via the COM1-port.     }
{  The port is initialised to 4800 baud, 8 bit, 2 stopbit and no parity }

{  The function key is used to send kommands to the color probe         }

{  The program is terminated by pressing the ESC-key twice              }

Program Sony39;

{$D+}
{$R+}
{$M 8192 0 8192}

USES
 Dos,Crt,Unit_A0,Unit_A1;


  TYPE
    HexStr	 = STRING[5];
    Windows	 = (Status,Receive,Transmit,Messge,Screen,MeasHead);
    WindowCorner = (UpperX,UpperY,LowerX,LowerY);

  CONST
    LogFileName = '39LOG.DAT';
    NULL = #0;
    F1 = 59; F2 = 60; F3 = 61; F4 = 62; F5 = 63;     {scancode for}
    F6 = 64; F7 = 65; F8 = 66; F9 = 67; F10 = 68;    {keyboard}
    AltL  = $26;
    AltF1 = 104;   {select red}
    AltF2 = 105;   {select green}
    AltF3 = 106;   {select blue}
    AltF4 = 107;   {select white}

    FKeyCommands : ARRAY[F1..F10] OF STRING[2] =
                   ('I?','F?','S?','RM','MR','MX','TM','LC','MC','MS');

    WindowMarks  : ARRAY[Windows,WindowCorner] OF WORD
		   = ((1,1,80,1),       { Status Window                 }
		      (1,2,37,16),	{ Receive Window		}
		      (1,17,34,23),	{ Transmit Window		}
		      (1,24,80,25),	{ Message Window		}
		      (1,1,80,25),	{ The entire physical screen	}
		      (38,2,80,23));	{ Color probe window	        }

    ESCAPE	   = $1B;		{ Escape Character		}
    BACKSPACE	   = $08;		{ Backspace			}
    EOT 	   = $04;		{ End of Transmission		}
    ETX 	   = $03;		{ End of Text			}
    ENTER	   = $0D;		{ Enter key			}
    LINEFEED	   = $0A;		{ Linefeed			}
    BLANK	   = ' ';
    InQSize        = 1000;
    OutQSize       = 200;

 VAR
  LogFile  : TEXT;
  OutPort,                              { Transmission port number }
  InPort   : INTEGER; 		        { Receive port number	     }
  InQueue  : ARRAY[1..InQSize] OF CHAR; { Receive buffer           }
  OutQueue : ARRAY[1..OutQSize] OF CHAR;{ Transmission buffer      }

  OrigMode,
  OrigMax,
  WinAttr,
  BordAttr,
  MsgAttr,
  I,
  ErrorCode,
  Parameter,
  RcvXCoor,
  RcvYCoor    : WORD;
  Ch,LastCh   : CHAR;
  LogFlag,
  Done,
  EndofTrans,
  EndofText   : BOOLEAN;


  {*********************************************************************}
  { SelectWindow	Activate the appropriate window 		}
  {_____________________________________________________________________}

 PROCEDURE SelectWindow(Win : Windows);
    BEGIN
      TextAttr := WinAttr;
      WindMin  := ((WindowMarks[Win,UpperY] - 1) SHL 8) or
                   (WindowMarks[Win,UpperX] - 1);
      WindMax  := ((WindowMarks[Win,LowerY] - 1) SHL 8) or
					   (WindowMarks[Win,LowerX] - 1);
 END;  {SelectWindow}


  {*********************************************************************}
  { ClrWindow		Clear all text from a window			}
  {_____________________________________________________________________}

 PROCEDURE ClrWindow(Win : Windows);
 VAR  I : WORD;
 BEGIN
  SelectWindow(Win);
  GotoXY(1,1);
  FOR I := 1 TO WindowMarks[Win,LowerY] - WindowMarks[Win,UpperY] + 1 DO
    DelLine;
 END;  {ClrWindow}


 PROCEDURE InitFKeyMessage;
 BEGIN
  SelectWindow(Messge);
  GotoXY(1,1);
  TextBackground(White);
  ClrScr;
  TextColor(Red);Write(' F1-');TextColor(Black);Write(FKeyCommands[F1]);
  TextColor(Red);Write('   F2-');TextColor(Black);Write(FKeyCommands[F2]);
  TextColor(Red);Write('   F3-');TextColor(Black);Write(FKeyCommands[F3]);
  TextColor(Red);Write('   F4-');TextColor(Black);Write(FKeyCommands[F4]);
  TextColor(Red);Write('   F5-');TextColor(Black);Write(FKeyCommands[F5]);
  TextColor(Red);Write('   F6-');TextColor(Black);Write(FKeyCommands[F6]);
  TextColor(Red);Write('   F7-');TextColor(Black);Write(FKeyCommands[F7]);
  TextColor(Red);Write('   F8-');TextColor(Black);Write(FKeyCommands[F8]);
  TextColor(Red);Write('   F9-');TextColor(Black);Write(FKeyCommands[F9]);
  TextColor(Red);Write('   F10-');TextColor(Black);Write(FKeyCommands[F10]);
  GotoXY(1,2);
  TextColor(Red);Write(' AltF1-');TextColor(Black);Write('Red');
  TextColor(Red);Write(' AltF2-');TextColor(Black);Write('Green');
  TextColor(Red);Write(' AltF3-');TextColor(Black);Write('Blue');
  TextColor(Red);Write(' AltF4-');TextColor(Black);Write('White');
  TextColor(Red);Write(' AltL-');TextColor(Black);Write('Log to 39LOG.DAT');
  GotoXY(69,2);
  TextColor(Red);Write(' 2xESC-');TextColor(Black);Write('Exit');
 END;



PROCEDURE SelectCRTColor( Color : BYTE);
VAR OldX,OldY : BYTE;
BEGIN
 OldX := WhereX; OldY := WhereY;
 SelectWindow(MeasHead);
 TextBackground(Color); ClrScr;
 SelectWindow(Screen);
 GotoXY(OldX,OldY);
END;


  {*********************************************************************}
  { InitWindows 	Initialize the windows.  The windows are marked }
  {			with horizontal lines, and the status line is	}
  {			written to the Status window.			}
  {_____________________________________________________________________}

PROCEDURE InitWindows;
VAR  I : WORD;
BEGIN {InitWindows}
 TextAttr := WinAttr;
 ClrScr;
 SelectWindow(Status);
 TextAttr := MsgAttr;
 GotoXY(1,1);
 ClrScr;
 Write(' Philips TV Test Equipment');
 Write('SONY color probe':53);
 InitFKeyMessage;
 SelectCRTColor(LightGray);
END;  {InitWindows}


  {*********************************************************************}
  { Pause		Wait for keystroke	                        }
  {_____________________________________________________________________}

 PROCEDURE Pause;
 VAR      Ch : CHAR;
 BEGIN
  WHILE (NOT KeyPressed) DO;
   Ch := ReadKey;
 END;  {Pause}


  {*********************************************************************}
  { ToHex	Convert a word to a hexadecimal string. The string type }
  {		type HexStr is globally defined as string[5].		}
  {_____________________________________________________________________}

 FUNCTION ToHex(J : WORD) : HexStr;
 CONST
   HexCheck : array[0..15] of CHAR = '0123456789ABCDEF';
 VAR
   I : WORD;
 BEGIN
  ToHex[0] := Chr(4);
  FOR I := 0 TO 3 DO
   BEGIN
    ToHex[4 - I] := HexCheck[J AND $000F];
     J := J SHR 4
   END;
 END;  {ToHex}


  {*********************************************************************}
  { ErrorMessage	Display an Error Message and Error Code 	}
  {_____________________________________________________________________}

 PROCEDURE ErrorMessage(ErrorMsg : STRING;
			 ErrorCode,PortStatus : WORD; Wait : BOOLEAN);
 CONST
  Level1Error  : array[1..18] of STRING[40] =  { Standard error message}
               ('Reserved for future use',
		'Invalid COM port number',
		'COM port not open',
		'Invalid parameter or function value',
		'Reserved for future use',
		'No seriel port found',
		'Output queue is full',
		'Reserved for future use',
		'COM port is already opne',
		'Input queue is empty',
		'Reserved for future use',
		'Timeout ',
		'Wait cancelled',
		'No default port address avaible',
		'No default IRQ-line avaible',
		'Bad buffer size or buffer pointer',
		'Unknown baud rate divisor',
		'Invalid Level Zero option parameter');

 MessageOn : BOOLEAN = TRUE;

 BEGIN {ErrorMessage}
  IF (ErrorMsg = 'ERASE') THEN
    IF (NOT MessageOn) THEN
      Exit
     ELSE
      BEGIN
       ClrWindow(Messge);
       MessageOn := FALSE;
       Exit
      END;

  ClrWindow(Messge);
  TextAttr := MsgAttr;
  GotoXY(1,1);
  MessageOn := TRUE;
  IF Length(ErrorMsg) > 0 THEN
   Writeln(ErrorMsg);
  IF (ErrorCode <> 0) THEN
   Write(' Error Code = ',ErrorCode:2,'    ',
         Level1Error[ErrorCode]);
  IF (PortStatus <> 0) THEN
       Write(' Port Status = ',ToHex(PortStatus));
  IF Wait THEN
   BEGIN
    Write(' Press any key...');
    Pause;
    InitFKeyMessage;
   END;
 END;  {ErrorMessage}

  {*********************************************************************}
  { InitPort		Initialize the COM Port. 			}
  {			The COM port are opened and baud rate, parity,  }
  {			stop bits and data bits parameters are set.	}
  {_____________________________________________________________________}

 PROCEDURE InitPort;
 VAR
  ErrorCode       : WORD;
  Valid	          : BOOLEAN;
  XCoor,YCoor,
  PortAds,
  IntLevel,
  BaudRate,
  Parity,
  DataBits,
  StopBits	  : WORD;

 BEGIN
  SelectWindow(Receive);
  GotoXY(1,1);

  InPort    := _COM1;           {Receiving Port  (1...4)}
  OutPort   := _COM1;           {Transmission Port}
  IntLevel  := 0;               {Interrupt Level (0 = default)}
  PortAds   := 0;               {Port Address    (0 = default)}
  ErrorCode := __OpenA1(InPort,InQSize,OutQSize,IntLevel,PortAds,@InQueue);

    { Set the Baud Rate		}
    {Baud Rate: 110 150 300 600 1200 2400 4800 9600 19200}
    {Code     :   0   1   2   3    4    5    6    7     8}
       BaudRate  := 6;
       ErrorCode := __SetOpA1(InPort,1,BaudRate);
       ErrorCode := __SetOpA1(OutPort,1,BaudRate);

    { Set the Parity 		}
    {Parity      None  Odd  Even  Mark  Space}
    {Code          0    1     2     3     4}
       Parity    := 0;
       ErrorCode := __SetOpA1(InPort,2,Parity);
       ErrorCode := __SetOpA1(OutPort,2,Parity);

    { Set the number of data bits	}
    {Data Bits     5    6    7    8}
    {Code          0    1    2    3}
       DataBits  := 3;
       ErrorCode := __SetOpA1(InPort,3,DataBits);
       ErrorCode := __SetOpA1(OutPort,3,DataBits);

    { Set the number of stop bits}
    {Stop Bits     1    2}
    {Code          0    1}
       StopBits  := 1;
       ErrorCode := __SetOpA1(InPort,4,StopBits);
       ErrorCode := __SetOpA1(OutPort,4,StopBits);

       ClrWindow(Receive);

    {Do not require CTS for the output port.}
       ErrorCode := __SetOpA1(OutPort,9,0);
 END;  {InitPort}


 {*********************************************************************}
 { OutCh		Send a character to the COM output queue.	}
 {_____________________________________________________________________}

 PROCEDURE OutCh(Ch : CHAR);

 VAR
  ErrorCode   : WORD;
  XCoor,YCoor : WORD;

 BEGIN
  ErrorCode := __WrtChA1(OutPort,Ch);
   IF (ErrorCode <> 0) THEN
    BEGIN
     XCoor := WhereX;
     YCoor := WhereY;
     ErrorMessage('Write error',ErrorCode,0,TRUE);
     ErrorCode := __OFlshA1(OutPort);   { Drain the output queue. }
     SelectWindow(Transmit);
     GotoXY(XCoor,YCoor);
    END;
 END;  {OutCh}


  {*********************************************************************}
  { ReadCh		Return a character from the COM port and dis-	}
  {			play it in the Receive window.                  }
  {_____________________________________________________________________}

 PROCEDURE ReadCh(var RcvXCoor,RcvYCoor : WORD;
		      VAR EndofText,EndofTransmission : BOOLEAN);

 VAR
  ErrorCode,
  InQSize,
  PortStatus,
  XCoor,YCoor : WORD;
  Ch	       : CHAR;
  Echo        : BOOLEAN;

 BEGIN

 XCoor	:= WhereX;	       { Cursor position in the trans-	}
 YCoor	:= WhereY;	       { mit window.			}
 Echo	:= FALSE;

 ErrorCode := __RdChA1(InPort,Ch,InQSize,PortStatus);
 IF ((PortStatus AND $00F3) <> 0) THEN   { Check Port Status error }
	 BEGIN
           ErrorMessage('',ErrorCode,PortStatus,TRUE);
           ErrorCode := __IFlshA1(InPort)
	 END
      ELSE IF (ErrorCode = 0) THEN     { Character successfully read	}
       BEGIN
         SelectWindow(Receive);
         GotoXY(RcvXCoor,RcvYCoor);
         TextColor(Yellow);
         IF (Ch <> Chr(LINEFEED)) THEN
             Write(Ch);	               { Ignore line feeds	}
         IF LogFlag THEN
           BEGIN
             Write(LogFile,Ch);
             IF (Ch = Chr(ENTER)) THEN
               Write(LogFile,Chr(LINEFEED));
             END;
         IF (Ch = Chr(BACKSPACE)) THEN	{ Delete the character	}
           BEGIN
             Write(BLANK);
             GotoXY(RcvXCoor - 1,RcvYCoor)
           END
          ELSE
             IF (Ch = Chr(ENTER)) THEN
               Write(Chr(LINEFEED));

         RcvXCoor := WhereX;
         RcvYCoor := WhereY
       END
      ELSE IF (ErrorCode <> 10) THEN
	 BEGIN
	   ErrorMessage(' Read error',ErrorCode,PortStatus,TRUE);
	   ErrorCode := __IFlshA1(InPort)
	 END;

      SelectWindow(Transmit);
      GotoXY(XCoor,YCoor)

 END;  {ReadCh}


 {*********************************************************************}
 { LogData                                                             }
 {           Toggle the log function. If the logfile does not exist,   }
 {           then it is opened - otherwise the data is appended to the }
 {           logfile                                                   }
 {_____________________________________________________________________}

 PROCEDURE LogData;
 BEGIN
 {$I-}
   Reset(LogFile);
  IF IOResult <> 0 THEN
    Rewrite(LogFile)
   ELSE
    Append(Logfile);
 {$I+}
  LogFlag := NOT LogFlag;
  IF LogFlag THEN
    Writeln('Log: ON')
   ELSE
    Writeln('Log: OFF');
 END;



 {*********************************************************************}
 { TransmitFunctionCommand                                             }
 {                Transmits a string from the array FKeyCommands       }
 {_____________________________________________________________________}

PROCEDURE TransmitFunctionCommand(KeyNo : BYTE);
VAR  n : BYTE;
     s : STRING[1];
BEGIN
 CASE KeyNo OF
  F1..F10 :  BEGIN
              FOR n := 1 TO Length(FKeyCommands[KeyNo]) DO
               BEGIN
                s := Copy(FKeyCommands[KeyNo],n,1);
                OutCh(s[1]);
               END;
              OutCh(Chr(ENTER));
              Writeln(FKeyCommands[KeyNo]);
             END;
    AltF1 :  SelectCRTColor(Red);
    AltF2 :  SelectCRTColor(Green);
    AltF3 :  SelectCRTColor(Blue);
    AltF4 :  SelectCRTColor(LightGray);
    AltL  :  LogData;

   ELSE
     Writeln(#7,'Not a valid key !');
   END; {case}
END;




  {********************************************************************}
  { Main program                                                       }
  {********************************************************************}

  BEGIN

    LogFlag := FALSE;
    Assign(LogFile,LogFileName);
    OrigMode   := LastMode;             { Restore on exit         }
    OrigMax    := WindMax;
    TextMode(Lo(LastMode));             { Ensure 25 lines         }
    IF (Lo(LastMode)) = Mono THEN
       BEGIN
	 WinAttr  := LightGray;
	 BordAttr := White;
	 MsgAttr  := Black + LightGray * 16
       END
    ELSE
       BEGIN
	 WinAttr  := White + Blue * 16;
	 BordAttr := BordAttr;
	 MsgAttr  := White + Red * 16
       END;
    InitWindows;
    InitPort;
    RcvXCoor   := 1;
    RcvYCoor   := 1;
    EndofTrans := FALSE;
    EndofText  := FALSE;
    SelectWindow(Transmit);
    GotoXY(1,1);

    Done   := FALSE;
    LastCh := BLANK;
    WHILE (NOT Done) DO
      BEGIN
	WHILE (KeyPressed) DO
	  BEGIN
            TextColor(White);
	    Ch := ReadKey;
            IF Ch = NULL THEN
             BEGIN
              Ch := ReadKey;
              TransmitFunctionCommand(Ord(Ch));
             END
             ELSE
              IF (Ch = Chr(ESCAPE)) THEN
	       IF (LastCh = Chr(ESCAPE)) THEN
                  Done := TRUE
                 ELSE
		  OutCh(Chr(ETX))
	    ELSE IF (Ch = Chr(Backspace)) THEN
	       BEGIN
		 Write(Ch);
		 Write(BLANK);
		 Write(Ch);
		 OutCh(Ch)
	       END
	    ELSE
	       BEGIN
		 Write(Ch);
		 OutCh(Ch)
	       END;
	    IF (Ch = Chr(ENTER)) THEN
	       Writeln;
	    LastCh := Ch
	  END;
	ReadCh(RcvXCoor,RcvYCoor,EndofText,EndofTrans)
      END;

    {$I-}
    Close(LogFile);
    {$I+}
    ErrorCode := __CloseA1(InPort);
    ErrorCode := __CloseA1(OutPort);
    TextMode(OrigMode);
    GotoXY(1,Hi(OrigMax))

  END.


