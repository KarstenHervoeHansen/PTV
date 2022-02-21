Program Type3920;

USES CRT, Dos;

(*
Errorlevel  =  0        OK
Errorlevel  =  1        QQQ konstant i EXE file ikke fundet.
Errorlevel  =  2        EXE file ikke fundet / ikke l‘st korrekt.
Errorlevel  =  3        Pix Data file ikke fundet / ikke l‘st korrekt.
Errorlevel  =  4        Ny EXE file med pix data og KU-nummer ikke skrevet
                        korrekt til disk.
Errorlevel  =  5        WWW konstant i EXE file ikke fundet.
*)
CONST  ZZZStartPos = $26000;

VAR EXEFile      : FILE OF CHAR;
    SoftWareType : STRING[4];
    EXEData      : ARRAY[0..3] OF CHAR;
    Ftime,
    ZZZPos,
    EXEFileSize,
    n            : LONGINT;
    NumRd        : WORD;
    ZZZFound,
    Finished     : BOOLEAN;
    Dt           : DateTime;

PROCEDURE GetEXEFiletime;
VAR FName : STRING[20];
BEGIN
 FName := ParamStr(1);
 FName := Copy(FName,1,Length(FName));
 Assign(EXEFile, FName);
{$I-}
 Reset(EXEFile);
{$I+}
 IF IOResult <> 0 THEN
  BEGIN
   Writeln(' > Fejl: File "',ParamStr(1),'" ikke fundet');
   Writeln('   Program afbrudt');
   Halt(2);
  END;
 GetFTime(EXEFile,Ftime);

 {UnpackTime(Ftime,Dt);
 Dt.Hour
 Dt.Min
 Dt.Sec}

{$I-}
 Close(EXEFile);
{$I+}
 IF IOResult <> 0 THEN
  BEGIN
   Writeln(' > Fejl: File "',ParamStr(1),'" ikke lukket');
   Writeln('   Program afbrudt');
   Halt(2);
  END;
END;

PROCEDURE SetEXEFiletime;
BEGIN
 Assign(EXEFile,ParamStr(1));
{$I-}
 Reset(EXEFile);                 { Gen†ben fil for l‘sning }
                                 { Ellers vil Close opdatere klokkeslet}
{$I+}
 IF IOResult <> 0 THEN
  BEGIN
   Writeln(' > Fejl: File "',ParamStr(1),'" ikke fundet');
   Writeln('   Program afbrudt');
   Halt(2);
  END;

 SetFTime(EXEFile,Ftime);
{$I-}
 Close(EXEFile);
{$I+}
 IF IOResult <> 0 THEN
  BEGIN
   Writeln(' > Fejl: File "',ParamStr(1),'" ikke lukket');
   Writeln('   Program afbrudt');
   Halt(2);
  END;

END;

PROCEDURE ReadEXEFile;
BEGIN
 Assign(EXEFile,ParamStr(1));
 {$I-}
 Reset(EXEFile);
 {$I+}
 EXEFileSize := FileSize(EXEFile);
 IF IOResult <> 0 THEN
  BEGIN
   Writeln(' > Fejl: Ingen EXE file fundet: "',ParamStr(1),'"');
   Writeln(' Program afbrudt');
   Finished := TRUE;
   Exit;
  END;

  n := 0;
  Write(' S›ger efter ZZZ .');
  Seek(EXEFile,ZZZStartPos);
  WHILE (n < 20000) AND (NOT ZZZFound)  DO
   BEGIN
    Read(EXEFile,EXEData[0]);
    IF EXEData[0] = 'Z' THEN
     BEGIN
      Read(EXEFile,EXEData[1]);
      Read(EXEFile,EXEData[2]);
      Read(EXEFile,EXEData[3]);
      ZZZFound := (EXEData[1] = 'Z') AND
                  (EXEData[2] = 'Z') AND
                  (EXEData[3] = 'Z');
      Inc(n,3);
     END;
    Inc(n);
    IF ((n MOD 2000) = 0) THEN Write('.');
   END; {while}

  IF ZZZFound THEN
   BEGIN
    ZZZPos := ZZZStartPos + n - 4;
    Writeln('  fundet i position: ',ZZZPos);
   END
   ELSE
   BEGIN
    Writeln('  IKKE fundet');
    ZZZPos := 0;
   END;
  Seek(EXEFile,ZZZPos);

  FOR n := 1 TO 4 DO
   Write(EXEFile, SoftWareType[n]);

  IF ZZZFound THEN
  Writeln(' Data flyttet')
   ELSE
  Writeln(' Data IKKE flyttet');



{$I-}
 Close(EXEFile);
{$I+}
 IF IOresult <> 0 THEN
  BEGIN
   Writeln(' > Fejl: "',ParamStr(1),'" ikke lukket korrekt');
   Writeln('   Program afbrudt');
   Finished := TRUE;
   Exit;
  END;
END;

BEGIN
 Finished := FALSE;

 IF ParamCount = 0 THEN
  BEGIN
   Writeln(' Ingen EXE file defineret >>>     ENT3920  <EXE file>');
   Writeln(' Program afbrudt');
   Exit;
  END;

 SoftWareType := ParamStr(2);

 GetEXEFileTime;

 ReadEXEFile;
 IF Finished THEN Halt(2);

 IF NOT Finished THEN
  SetEXEFiletime;

 Halt(0);
END.
