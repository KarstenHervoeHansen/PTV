Program EnterEXE;
{ Flytter data fra KU_PIXEL.DAT og SERIAL.INC til PM8550.EXE:
  Inden programmet k›res, skal PM8550.ORG kopieres til PM8550.EXE.
  PM8550.ORG er den EXE-file, som pascal compileren laver.
  QQQStartPos = pixel data
  WWWStartPos = serial number
}


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
CONST QQQStartPos = $28000;
      WWWStartPos = $02000;
      KUPixelFileName = 'KU_PIXEL.DAT';
      SerialFileName = 'SERIAL.DAT';

VAR EXEFile      : FILE OF CHAR;
    PixDataFile  : FILE;
    SerialFile   : TEXT;
    SerialNo     : STRING[8];
    EXEData      : ARRAY[0..3] OF CHAR;
    PixData      : ARRAY[0..3000] OF CHAR;
    Ftime,
    WWWPos,
    QQQPos,
    EXEFileSize,
    PixFileSize,
    n            : LONGINT;
    NumRd        : WORD;
    QQQFound,
    WWWFound,
    Finished     : BOOLEAN;
    Dt           : DateTime;

PROCEDURE GetEXEFiletime;
VAR FName : STRING[20];
BEGIN
 FName := ParamStr(1);
 FName := Copy(FName,1,Length(FName)-3) + 'ORG';
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

  QQQFound := FALSE;
  n := 0;
  Write(' S›ger efter QQQ .');
  Seek(EXEFile,QQQStartPos);
  WHILE (NOT Eof(EXEFile)) AND (NOT QQQFound)  DO
   BEGIN
    Read(EXEFile,EXEData[0]);
    IF EXEData[0] = 'Q' THEN
     BEGIN
      Read(EXEFile,EXEData[1]);
      Read(EXEFile,EXEData[2]);
      Read(EXEFile,EXEData[3]);
      QQQFound := (EXEData[1] = 'Q') AND
                  (EXEData[2] = 'Q') AND
                  (EXEData[3] = 'Q');
      Inc(n,3);
     END;
    Inc(n);
    IF ((n MOD 2000) = 0) THEN Write('.');
   END; {while}

  IF QQQFound THEN
   BEGIN
    QQQPos := QQQStartPos + n - 4;
    Writeln('  fundet i position: ',QQQPos);
   END
   ELSE
   BEGIN
    Writeln('  IKKE fundet');
    QQQPos := 0;
   END;
  Seek(EXEFile,QQQPos);
  FOR n := 0 TO PixFileSize - 1 DO
   Write(EXEFile,PixData[n]);


  n := 0;
  Write(' S›ger efter WWW .');
  Seek(EXEFile,WWWStartPos);
  WHILE (n < 90000) AND (NOT WWWFound)  DO
   BEGIN
    Read(EXEFile,EXEData[0]);
    IF EXEData[0] = 'W' THEN
     BEGIN
      Read(EXEFile,EXEData[1]);
      Read(EXEFile,EXEData[2]);
      Read(EXEFile,EXEData[3]);
      WWWFound := (EXEData[1] = 'W') AND
                  (EXEData[2] = 'W') AND
                  (EXEData[3] = 'W');
      Inc(n,3);
     END;
    Inc(n);
    IF ((n MOD 2000) = 0) THEN Write('.');
   END; {while}

  IF WWWFound THEN
   BEGIN
    WWWPos := WWWStartPos + n - 4;
    Writeln('  fundet i position: ',WWWPos);
   END
   ELSE
   BEGIN
    Writeln('  IKKE fundet');
    WWWPos := 0;
   END;
  Seek(EXEFile,WWWPos);

  FOR n := 1 TO 6 DO
   Write(EXEFile,SerialNo[n+2]);
  IF QQQFound AND WWWFound THEN
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

PROCEDURE ReadSerialFile;
BEGIN
 Assign(SerialFile,SerialFileName);
 {$I-}
  Reset(SerialFile);
 {$I+}
 Readln(SerialFile,SerialNo);
 Close(SerialFile);
END;


PROCEDURE ReadPixFile;
BEGIN
 Assign(PixDataFile,KUPixelFileName);
 {$I-}
  Reset(PixDataFile,1);
  PixFileSize := FileSize(PixDataFile);
 {$I+}
 IF IOResult <> 0 THEN
  BEGIN
   Writeln(' > Fejl: Ingen Pix Data file fundet: "',KUPixelFileName,'"');
   Writeln('   Program afbrudt');
   Finished := TRUE;
   Exit;
  END;

 BlockRead(PixDataFile,PixData,PixFileSize,NumRd);

 Writeln(' Data l‘st fra Pix Data file "',KUPixelFileName,'"  (',NumRd,' bytes)');

 IF PixFileSize <> NumRd THEN
  BEGIN
   Writeln(' > Fejl ved l‘sning af data fra: "',KUPixelFileName,'"');
   Writeln('   Antal l‘ste bytes: ',NumRd,'       forventet: ',PixFileSize);
   Writeln('   Program afbrudt');
   Finished := TRUE;
   Exit;
  END;

{$I-}
 Close(PixDataFile);
{$I+}
 IF IOResult <> 0 THEN
  BEGIN
   Writeln(' > Fejl: "',KUPixelFileName,'" ikke lukket korrekt');
   Writeln('   Program afbrudt');
   Finished := TRUE;
   Exit;
  END;
END;


BEGIN
 Finished := FALSE;

 IF ParamCount <> 1 THEN
  BEGIN
   Writeln(' Ingen EXE file defineret >>>     ENT3920  <EXE file>');
   Writeln(' Program afbrudt');
   Exit;
  END;

 Writeln;
 Writeln;
 TextColor(Black);TextBackground(LightGray);
 Write('               ');
 Writeln('Pix data og KU nummer kopiering til "',ParamStr(1),'"               ');
 TextColor(LightGray);TextBackground(Black);
 Writeln;

 GetEXEFileTime;

 ReadSerialFile;
 ReadPixFile;
 IF Finished THEN Halt(3);

 ReadEXEFile;
 IF Finished THEN Halt(2);



 IF NOT Finished THEN
  SetEXEFiletime;

 Halt(0);
END.
