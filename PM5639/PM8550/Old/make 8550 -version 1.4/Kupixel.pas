Program KUPixel;


USES PhLogo,Graph,Crt;

CONST KUPixFileName = 'KU_PIXEL.DAT';
      SerialNoFileName = 'SERIAL.DAT';


VAR  P            : POINTER;
     NumWr,
     X1,Y1,X2,Y2,
     Size         : WORD;
     IOErr        : INTEGER;
     SerialNo     : STRING[8];
     PixData      : FILE;
     SerialNoFile : TEXT;

BEGIN
 Writeln;
 Writeln;
 REPEAT
  GotoXY(2,WhereY-1); ClrEol;
  Write(' Indtast KU nummer  ( 6 tal ):  KU');
  Readln(SerialNo);
  SerialNo := Copy(SerialNo,1,6);
 UNTIL Length(SerialNo) = 6;
 GotoXY(34,WhereY-1); ClrEol;
 SerialNo := 'KU' + SerialNo;
 Writeln(SerialNo);


 InitLogoGraph;
 WriteKU('Serial number: '+SerialNo);
 X1 := (MaxX - KUWidth) DIV 2;
 Y1 := MaxY * 45 DIV 100;
 X2 := X1 + KUWidth;
 Y2 := Y1 + KUHeight;

 Size := ImageSize(X1,Y1,X2,Y2);
 GetMem(P,Size);
 GetImage(X1,Y1,X2,Y2,P^);

 {$I-}
 Assign(PixData,KUPixFileName);
 Assign(SerialNoFile,SerialNoFileName);
 Rewrite(PixData,1);
 Rewrite(SerialNoFile);
 {$I+}
 IOErr := IOResult;
 IF IOErr <> 0 THEN
  BEGIN
   CloseGraph;
   Writeln('KU pixel file IKKE oprettet   -   Error: ',IOErr);
   Halt(1);
  END;

 {$I-}
 Assign(SerialNoFile,SerialNoFileName);
 Rewrite(SerialNoFile);
 {$I+}
 IOErr := IOResult;
 IF IOErr <> 0 THEN
  BEGIN
   CloseGraph;
   Writeln('Serial number file IKKE oprettet   -   Error: ',IOErr);
   Halt(1);
  END;
 BlockWrite(PixData,P^,Size,NumWr);
 Close(PixData);
 Writeln(SerialNoFile,SerialNo);
 Close(SerialNoFile);
 CloseGraph;
 Writeln('Serial number: ',SerialNo,' skrevet til ''',SerialNoFileName,'''');
 Writeln('KU pixel file ''', KUPixFileName,''' oprettet  (',Size,' bytes)');
 Writeln;
 Halt(0);
END.
