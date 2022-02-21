Program Trans;

USES Crt,PepUtil,ComUtil,Win;

CONST    InQueSize  = 500;
         OutQueSize = 50;
         MeasDelay  = 5000;
         WinDes    : ARRAY[1..5,1..6] OF BYTE =
                      ((1,2,80,1,White,Red),
                      (1,2,80,19,Yellow,Blue),
                      (1,20,80,24,Black,White),
                      (1,25,80,25,Red,White),
                      (1,1,80,25,White,Black));

VAR COM1Status   : INTEGER;
    MeasFile     : TEXT;
    MeasFileName : STR_30;
    IOErr        : INTEGER;
    ProgramExit,
    QuitMeas,
    MeasFileOpen : BOOLEAN;
    WaveLength,
    KorrFactor   : REAL;
    Svar         : STRING;
    MeasStr      : STR_30;
    Win2Y        : BYTE;



PROCEDURE SelectWindow(WNo : BYTE);
BEGIN
 Window(WinDes[WNo,1],WinDes[WNo,2],WinDes[WNo,3],WinDes[WNo,4]);
 Color(WinDes[WNo,5],WinDes[WNo,6]);
END;

PROCEDURE InitPC_COM1Port;
{Initialiserer seriel-COM1-port.}
BEGIN
 IF NOT COM1Open THEN
  COM1Status := OpenCOMPort(_COM1,InQueSize,OutQueSize);
 COM1Status := InitCOMPort(_COM1,_Baud4800,_Databit8,_Stopbit2,_None,_CTSNotRequired);
END;

PROCEDURE CreateMeasFile;
BEGIN
 ProgramExit := FALSE;
 CreateWindow(10,10,5,60,' Opret fil til m†ledata ',Black+16*Cyan,Black+16*Cyan,Frame);
 Color(Black,Cyan);
 ClrScr;
 REPEAT
 Write('Navn p† fil til m†ledata > ');
 Readln(MeasFileName);
 {$I-}
 Assign(Measfile,MeasfileName);
 Reset(Measfile);
 {$I+}
 IOErr := IOResult;
 IF IOErr = 0 THEN
  BEGIN
   Write('Fil findes - Overskriv/Tilf›j  O/T  ');
   EmptyKeyboardBuffer;
   CASE Scancode OF
    TastO :BEGIN
            Writeln;
            Write('Fil: "',MeasfileName,'" overskrevet');
            {$I-}
            Rewrite(Measfile);
            {$I+}
            MeasFileOpen := TRUE;
          END;
    TastT :BEGIN
            Writeln;
            Write('Fil: "',MeasfileName,'" tilf›jes');
            {$I-}
            Append(Measfile);
            {$I+}
            MeasFileOpen := TRUE;
          END;
        END; {case}
  END
  ELSE
  BEGIN
   CASE IOErr OF
    2 : BEGIN
         Writeln('Fil: "',MeasfileName,'" oprettet');
         {$I-}
         Rewrite(Measfile);
         {$I+}
         MeasFileOpen := TRUE;
         ProgramExit := FALSE;
        END;
  150 : BEGIN
         Writeln(Bell,'Disk er write protected   (I/O Error: ',IOErr,')');
         MeasFileOpen := FALSE;
         ProgramExit := TRUE;
        END;
  152 : BEGIN
         Writeln(Bell,'Fil: "',MeasfileName,'" kan ikke oprettes   (I/O Error: ',IOErr,')');
         MeasFileOpen := FALSE;
         ProgramExit := TRUE;
        END;
    ELSE
        BEGIN
         Writeln(Bell,'I/O Error:  ',IOErr);
         MeasFileOpen := FALSE;
         ProgramExit := TRUE;
        END;
    END; {case}
         Delay(1500);
  END; {if}
 Delay(2000);
 IOErr := IOResult;

 UNTIL (IOErr = 0);
 CloseWindow;

 SelectWindow(4);
 GotoXY(60,1); Write('Fil: "',MeasfileName,'"');
 SelectWindow(3);
END;


PROCEDURE InitVar;
BEGIN
 ProgramExit := FALSE;
 QuitMeas := FALSE;
 WaveLength := 0;
 KorrFactor := 0;
 Win2Y:= 1;
END;


PROCEDURE InitScreen;
BEGIN
SelectWindow(1);
ClrScr;
Write('     PM5639 color sensor measurement   -   (c) 1992 Philips TV Test' );
SelectWindow(2);
ClrScr;
SelectWindow(4);
ClrScr;
SelectWindow(3);
ClrScr;
END;

FUNCTION InputKeyboard(Message : STR_50; LineNo : BYTE) : REAL;
VAR InStr : STR_30;
    k     : INTEGER;
    v     : REAL;
BEGIN
IF LineNo = 1 THEN
 REPEAT
  GotoXY(1,LineNo); ClrEol;
  Write(Message); Readln(InStr);
  Val(InStr,v,k);
 UNTIL ((k = 0) AND (v > 300) AND (v < 900));
IF LineNo = 2 THEN
 REPEAT
  GotoXY(1,LineNo); ClrEol;
  Write(Message); Readln(InStr);
  Val(InStr,v,k);
 UNTIL ((k = 0) AND (v > 0.1));
 InputKeyboard := v;
END;

PROCEDURE ReadColorSensor;
VAR n : BYTE;
BEGIN
 ClrEol; Write(' ');
 FOR n := 1 TO (MeasDelay DIV 1000) DO
  BEGIN
   Write('.');
   Delay(1000);
  END;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,2);
 IF COM1Status = 0 THEN
   MeasStr := Svar
  ELSE
   MeasStr := 'Timeout';
END;

PROCEDURE NewMeasYesOrNo( LineNo : BYTE);
BEGIN
 SelectWindow(3);
 GotoXY(1,LineNo);
 Write(' Ny m†ling ?    Ja/Nej');
 IF (ScanCode = TastN) THEN
   QuitMeas  := TRUE
  ELSE
   QuitMeas  := FALSE;
END;


PROCEDURE WriteDataToFile;
BEGIN
 Writeln(MeasFile,WaveLength:6:1,',',KorrFactor:6:3,',',MeasStr);
 SelectWindow(2);
 GotoXY(1,Win2Y);
 Writeln(WaveLength:6:1,',',KorrFactor:6:3,',',MeasStr);
 Win2Y := WhereY;
END;

BEGIN
 InitWindow;
 InitVar;
 InitScreen;
 InitPC_COM1Port;
 CreateMeasFile;
 COM1Status := WriteCOMPort(_COM1,'MX,');
 COM1Status := WriteCOMPort(_COM1,'SF150,');
 COM1Status := WriteCOMPort(_COM1,'MC,');
 IF NOT ProgramExit THEN
  REPEAT
   WaveLength := InputKeyboard(' Wavelength  > ',1);
   KorrFactor := InputKeyboard(' Korr factor > ',2);
   ReadColorSensor;
   WriteDataToFile;
   NewMeasYesOrNo(3);
   ClrScr;
  UNTIL QuitMeas;




IF MeasFileOpen THEN
 Close(Measfile);

SelectWindow(5); ClrScr;
GotoXY(1,2); Writeln('M†ledata i fil: "',MeasfileName,'"');

END.