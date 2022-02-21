Program FileTime;

{ Peter Frederiksen, P-DPE, 921127
  (c) copyright Philips Danmark A/S, 1992
  Philips TV Test Equipment A/S
  Kornmarksvej 21
  DK 2605 Br›ndby.

  Param(1):  Filnavn hvori tiden skal ‘ndres som angivet i Param(2).
  Param(2):  Tiden som filnavn i Param(1) skal have.

  Error codes:    1    ingen filnavn og tid angivet som parameter
  Error codes:    2    ingen tid angivet som parameter
  Error codes:    3    filnavn ikke fundet
  Error codes:    4    tid ikke korrekt

  Error codes kan testes med MS-DOS'ens 'errorlevel' fra f.eks. en bat-fil.
}

USES Dos;

VAR
 EXEFile      : TEXT;
 H,M          : WORD;
 Ftime        : LONGINT;
 Dt           : DateTime;
 p            : BYTE;
 Kode         : INTEGER;
 TimeErr      : BOOLEAN;

 BEGIN
  IF ParamCount < 1 THEN
  BEGIN
   Write('Error 1: No file and time found   --->    ');
   Writeln('FILETIME <file> <time>');
   Halt(1);
  END
  ELSE
  IF ParamCount < 2 THEN
  BEGIN
   Write('Error 2: No time found   --->    ');
   Writeln('FILETIME <file> <time>');
   Halt(2);
  END;

  p := Pos('.',ParamStr(2));
  Val(Copy(ParamStr(2),1,p-1),H,Kode);
  TimeErr := Kode <> 0;
  Val(Copy(ParamStr(2),p+1,Length(ParamStr(2))-p),M,Kode);
  TimeErr := TimeErr OR (Kode <> 0);
  IF TimeErr OR (H > 23) OR (M > 59) THEN
  BEGIN
   Writeln('Error 4: Time not correct');
   Halt(4);
  END;

  Assign(EXEFile, ParamStr(1));
 {$I-}
  Reset(EXEFile);                            { Open file }
 {$I+}
 IF IOResult <> 0 THEN
  BEGIN
   Writeln('Error 3: File "',ParamStr(1),'" not found');
   Halt(3);
  END;

  GetFTime(EXEFile,Ftime);
  UnpackTime(Ftime,Dt);
  Dt.Hour := H;
  Dt.Min := M;
  Dt.Sec := 0;
  PackTime(Dt,Ftime);
  Reset(EXEFile);                 { Gen†ben fil for l‘sning }
                                  { Ellers vil Close opdatere klokkeslet}
  SetFTime(EXEFile,Ftime);
  Close(EXEFile);                 { Luk fil }
 END.
