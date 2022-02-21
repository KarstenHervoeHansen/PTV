(*
CAL2.INC   Utility for PM8550.PAS file
See also CAL1.INC

Procedure CalculateMean;
Procedure GetWhiteMX;
Procedure Calibration;
Procedure DrawBox1
*)



Procedure CalculateMean(MS : MeasStrType; NoMeas : BYTE; var R,G,B : REAL; var St : BOOLEAN);
var   n,p       : BYTE;
      Kode      : INTEGER;
      FT,MVal   : REAL;
begin
 R := 0; G := 0; B := 0;

 for n := 1 to NoMeas do
   begin
     p := Pos(',',MS[n]);                    {Red}
      if p > 0 then
        begin
          Val(Copy(MS[n],1,p-1),MVal,Kode);
          if Kode <> 0 then
            St := FALSE;
          R := R + Mval;
          Delete(MS[n],1,p);
        end;
      p := Pos(',',MS[n]);                    {Green}
      if p > 0 then
        begin
          Val(Copy(MS[n],1,p-1),MVal,Kode);
          if Kode <> 0 then
            St := FALSE;
          G := G + Mval;
          Delete(MS[n],1,p);
        end;

      if OutputFormat = RGBI_OutputFormat then
        begin
          p := Pos(',',MS[n]);                            {Blue}
          if p > 0 then
            begin
              Val(Copy(MS[n],1,p-1),MVal,Kode);
              if Kode <> 0 then
                St := FALSE;
              B := B + Mval;
              Delete(MS[n],1,p);
            end;
        end;

      if OutputFormat = RGB_OutputFormat then
        begin
          Val(Copy(MS[n],1,Length(MS[n])),MVal,Kode);    {Blue}
          if Kode <> 0 then
            St := FALSE;
          B := B + Mval;
        end;
   end; {for}



  if OutputFormat = RGBI_OutputFormat then
   begin
     Val(MS[NoMeas],FT,Kode);                  {Field time}
     if (Kode = 0) AND (FT <> 0) then
      begin
       R := R / NoMeas / Abs(FT) * 20;            {norm to field time = 20 mS}
       G := G / NoMeas / Abs(FT) * 20;
       B := B / NoMeas / Abs(FT) * 20;
       St := TRUE;
      end
     else
      St := FALSE;
   end; { if OutputFormat }

  if OutputFormat = RGB_OutputFormat then
    begin
     FT := 20;
     R := R / NoMeas ;
     G := G / NoMeas ;
     B := B / NoMeas ;
     St := true;
    end;

end;  { CalculateMean }



Procedure GetWhiteMX(NoMeas : BYTE; var Status : BOOLEAN);
var n       : BYTE;
    MeasStr : MeasStrType;
    St,
    Timeout : BOOLEAN;
begin
 St := FALSE;
 Sound(1000); Delay(200); NoSound;
 UserMessage(' Place the color sensor on the monitor',
             '',1);

 if OutputFormat = RGB_OutputFormat then
  begin
   COMStatus := WriteCOMPort(PortNo,'MS,');
   Delay(10);
   COMStatus := WriteCOMPort(PortNo,'SI200,');  { integration time = 20mS }
   Delay(10);
  end;

 COMStatus := WriteCOMPort(PortNo,'MX,');
 COMStatus := WriteCOMPort(PortNo,'MC,');
 Check := __DispWin(MeasWin);
 Writeln('               Sensor Calibrating');
 Writeln;
 Write  ('               °°°°°°°°°°°°°°°°°°');
 __GotoWin(16,WhereY);

 COMStatus := ClearInputQue(PortNo);
 n := 1;

 repeat
   COMStatus  := jk_ReadlnMX(PortNo, Svar, 1);

   if (COMStatus <> 0) then
    begin
     UserMessage('Timeout', 'on Receive', 2);
     Finished := TRUE;
     Exit;
    end;

   Write('²');

   MeasStr[n] := Svar;
   Inc(n);
 until (n = NoMeas + 1) or (COMStatus <> 0);

 Timeout := COMStatus <> 0;

 if NOT Timeout then
   CalculateMean(MeasStr, NoMeas, X65, Y65, Z65, St);

 Status := (St AND (NOT Timeout));

 Zp65 := 1 - Xp65 - Yp65;

 COMStatus := WriteCOMPort(PortNo,'MS,');
 Check := __RemWin;
end;


Procedure Calibration;
var
 n,
 Error     : BYTE;
 GetWOk    : BOOLEAN;
 NewTxt    : STR_10;
 Mess1,
 Mess2     : STR_60;
 xx,yy,
 X,Y,Z     : REAL;     {/* Store X,Y og Z for beregning af lille x og y */}
 LimitErr,
 OnlyLumCal,
 OnlyUsTxt  : BOOLEAN;


FUNCTION ValidSolution : BOOLEAN;
{/*
   Kontrollerer om en af de 9 K-matrixelementer er absolut st›rre
   end 32000/KorrMulFactor, idet inden matrixelementerne skrives i EýPROM,
   multipliceres de med KorrMulFactor. Hver element skal kunne indeholdes
   i en INTEGER;
*/}
 var Valid : BOOLEAN;
     n     : BYTE;
     Lim   : REAL;
 begin
   Valid := TRUE;
   Lim   := 32000 / KorrMulFactor;

   for n := 1 to 9 do
     if Abs(KElement[n]) > Lim then
       Valid := FALSE;

   ValidSolution := Valid;

  end; {/* ValidSolution */}




begin

 OnlyUsTxt  := (Xp65 = 0) AND (Yp65 = 0) AND (LumValue = 0);
 OnlyLumCal := (Xp65 = 0) AND (Yp65 = 0) AND (LumValue > 0);

 NewTxt := SensorUserText;         {/* newtxt = kalibrerings tekst */}
 repeat
  GetStatus(SensorConnected,
            SensorPhilips,
            SensorValid,
            FactoryCal,
            SensorWP,
            KUStr,
            SensorSWStr,
            SensorUserText,
            OutputFormat);
  Finished := RetryYesOrNo(SensorConnected,UserTxt[6],UserTxt[7],2);
 until SensorConnected or Finished;
 if Finished then
   Exit;

 if (NOT SensorPhilips) then
  begin
   UserMessage(UserTxt[10],UserTxt[2],2);
   Finished := TRUE;
   Exit;
  end;

 if (NOT SensorValid) then
  begin
   UserMessage(UserTxt[3],UserTxt[2],2);
   Finished := TRUE;
   Exit;
  end;

 if SensorWP then
  begin
   UserMessage(UserTxt[15]+UserTxt[16], UserTxt[17],2);
   Exit;
  end;


 {/*---- gem K-matrix i adresse 158-175 ved xy/Y eller Y kalibrering ----*/}
 if NOT OnlyUsTxt then
   begin
     WaitMessage(' Please wait . . .');
     repeat
       ReadKMatrix (103,ReadKMOk,KElement);
       Finished := RetryYesOrNo(ReadKMOk,UserTxt[6],UserTxt[7],2);
     until ReadKMOk or Finished;
     if Finished then
       Exit;

     if FactoryCal then                           {/* gem factory kalibrering */}
       WriteKMatrix(158,SensorConnected)
     else
       if NOT YesOrNo(UserTxt[8],UserTxt[9]) then {/* cal status = USER CAL */}
         Exit;
   end; {/*NOT OnlyUsTxt*/}

 CloseWaitMessage;

 if Finished then
   Exit;

 {/*------------  write user text to sensor ---------------*/}
 WriteUserText(NewTxt);

 {/*------------  Kun ‘ndring af user text ----------------*/}
 if OnlyUsTxt then
   begin
     repeat
       GetStatus(SensorConnected,
              SensorPhilips,
              SensorValid,
              FactoryCal,
              SensorWP,
              KUStr,
              SensorSWStr,
              SensorUserText,
              OutputFormat);
       Finished := RetryYesOrNo(SensorConnected,UserTxt[6],UserTxt[7],2);
     until SensorConnected or Finished;


     if NOT Finished then
       if NewTxt = '' then
         UserMessage(' "User Text" in color sensor is unchanged.',' Color and luminance calibration is unchanged.',1)
       else
         UserMessage(' "User Text" written to color sensor.',' Color and luminance calibration is unchanged.',1);

     Exit;
   end; {/*if onlyustxt*/}


{/*---------- check cal limits before calibration ----------------*/}
 Mess1 := '';
 Mess2 := '';
 LimitErr := FALSE;

  if NOT OnlyLumCal then               {/* xy + Y kalibrering */}
    if ((Xp65 = 0) or (Yp65 = 0)) then
      begin
       LimitErr := TRUE;
        if CIEMode = 1 then
         Mess1 := ' Valid range for xy:   ' + RealToStr(MinCoor,5,3)+
                  ' - '+ RealToStr(MaxCoor,5,3)
        else
        if CIEMode = 2 then
         Mess1 := ' Valid range for u''v'': ' + RealToStr(MinCoor,5,3)+
                  ' - '+ RealToStr(MaxCoor,5,3)
        else
        if CIEMode = 3 then
         Mess1 := ' Valid range for uv: ' + RealToStr(MinCoor,5,3)+
                  ' - '+ RealToStr(MaxCoor,5,3);
      end;

    if ((LumValue < MinLum) or (LumValue > MaxLum)) then
      begin
       LimitErr := TRUE;
        if LumMode = 1 then
         Mess2 := ' Valid range for Y:    '+ RealToStr(MinLum,5,1)+
                  ' - '+ RealToStr(MaxLum,5,1) + ' cd/mý'
        else
        if LumMode = 2 then
         Mess2 := ' Valid range for Y:    '+ RealToStr(MinLum,5,1)+
                  ' - '+ RealToStr(MaxLum,5,1) + ' NIT'
        else
        if LumMode = 3 then
         Mess2 := ' Valid range for Y:    '+
                         RealToStr(MinLum*Cd_ftL_Factor+0.1,5,1)+
                  ' - '+ RealToStr(MaxLum*Cd_ftL_Factor-0.1,5,1) + ' ftL';
      end;

 if LimitErr then
  begin
   UserMessage(Mess1,Mess2,2);
   Exit;
  end;



   {/*
    KElement[x] er aktiv K-matrix i sensoren, som bruges for at beregne
    MX-v‘rdierne for r›d, gr›n, bl† og hvid sk‘rm.

       OnlyUsTxt  := (Xp65 = 0) AND (Yp65 = 0) AND (LumValue = 0);
       OnlyLumCal := (Xp65 = 0) AND (Yp65 = 0) AND (LumValue > 0);

   */}

    for n := 1 to 9 do
     KElement[n] := KElement[n] / KorrMulFactor;

    XpR := 0.64;  YpR := 0.33;  ZpR := 1 - XpR - YpR;   {/* koordinater kun for */}
    XpG := 0.29;  YpG := 0.60;  ZpG := 1 - XpG - YpG;   {/* beregning af MX */}
    XpB := 0.15;  YpB := 0.06;  ZpB := 1 - XpB - YpB;

    {/* Xp65 og Yp65 hentes fra user input eller beregnes ved CalMode = 2 */}
{/*
  On exit, FindMX does:

     XR := Solution[1];  YR := Solution[2]; ZR := Solution[3];
     XG := Solution[4];  YG := Solution[5]; ZG := Solution[6];
     XB := Solution[7];  YB := Solution[8]; ZB := Solution[9];

*/}

    FindMX(KElement,XpR,YpR, XpG,YpG, XpB,YpB, 0.31,0.33,
           XR,YR,ZR, XG,YG,ZG, XB,YB,ZB, Error);

    if Error <> 0 then
     begin
      UserMessage(UserTxt[4],UserTxt[5],2);
      Exit;
     end;


 {/*--- Hent MX-m†lev‘rdier X65,Y65 og Z65 fra user kalibrerings monitor --*/}
   GetWhiteMX(AntalMeas, GetWOk);

    if NOT GetWOk then
     begin
      UserMessage(UserTxt[4],UserTxt[5],2);
      Exit;
     end;


 {/*-------- beregning af xy og Y p† user monitor -------------------------*/}
     X := KElement[1] * X65 + KElement[2] * Y65 + KElement[3] * Z65;
     Y := KElement[4] * X65 + KElement[5] * Y65 + KElement[6] * Z65;
     Z := KElement[7] * X65 + KElement[8] * Y65 + KElement[9] * Z65;

     xx := X / (X + Y + Z);   {/* xx og yy er kun for debugning */}
     yy := Y / (X + Y + Z);


 if OnlyLumCal then            {/* Kun Y kalibrering: Xp65,Yp65 fra monitor */}
  begin                        {/* LumValue f†s altid fra user input        */}
   Xp65 := X / (X + Y + Z);
   Yp65 := Y / (X + Y + Z);
  end;

{/* LumMode    1 = cd/mý  2 = NIT   3 = ftL */}
{/* CIEMode    1 = xy     2 = u'v'  3 = uv  */}

  FindK(CIEMode,
        LumMode,
        Cd_ftL_Factor,
        XpR, YpR, XpG,  { coordinates defined above }
        YpG, XpB, YpB,  { coordinates defined above }
        Xp65,Yp65,      { x and y values ENTERED BY USER }
        XR,YR,ZR,       { returned by FindMX }
        XG,YG,ZG,       { returned by FindMX }
        XB,YB,ZB,       { returned by FindMX }
        X65,Y65,Z65,    { returned by GetWhiteMX, values for users white screen }
        LumValue,       { luminance value ENTERED BY USER }
        KElement,
        Error);

{/*  KM[n] := Solution[n] on exit from FindK */}

 if OnlyUsTxt or OnlyLumCal then   {/* initialiser til n‘ste kalibrering */}
  begin
   Xp65 := 0;
   Yp65 := 0;
  end;

  if (Error = 0) AND ValidSolution then
    begin
      for n := 1 to 9 do
        KElement[n] := KElement[n] * KorrMulFactor;
      WriteKMatrix(103,SensorConnected)
    end {if}
  else
   begin
    UserMessage(UserTxt[4],UserTxt[5],2);
    Exit;
   end;

   if Finished then
    Exit;

 {/*------- calibration ok, set cal status to USER CAL ------*/}
   repeat
    SetUserCal(SensorConnected);
    Finished := RetryYesOrNo(SensorConnected,UserTxt[6],UserTxt[7],2);
   until SensorConnected or Finished;

   if Finished then
    Exit;


 {/*--------------- opdater status field ------------*/}
   repeat
    GetStatus(SensorConnected,
              SensorPhilips,
              SensorValid,
              FactoryCal,
              SensorWP,
              KUStr,
              SensorSWStr,
              SensorUserText,
              OutputFormat);
    Finished := RetryYesOrNo(SensorConnected,UserTxt[6],UserTxt[7],2);
   until SensorConnected or Finished;

   __QuikWin(Col4,Row13,MainFore,MainBack,'          ');
   __QuikWin(Col4,Row13,MainFore,MainBack,SensorUserText);
   __QuikWin(Col8,Row13,MainFore,MainBack,'          ');
   __QuikWin(Col8,Row13,MainFore,MainBack,SensorUserText);

  if Finished then
   Exit;

 {/*------ prompt user om WP skal s‘ttes ON eller OFF ----------*/}
  if YesOrNo(UserTxt[13],UserTxt[14]) then
    WriteProtectOn;


end;  {/*calibration*/}





Procedure DrawBox1(x1,y1,x2,y2 : BYTE; Title : STR_30);
Var n : BYTE;
begin
  {218:Ú 217:Ù 192:À  191:¿ 196:Ä 179:³}
  __GotoWin(X1,Y1); Write(#218);
  for n := (x1+1) to (x2-1) do Write(#196);
  Write(#191);
  for n := 1 to (y2-y1-1) do
   begin
    __GotoWin(x2,y1+n);Write(#179);
   end;
  __GotoWin(x2,y2); Write(#217);
  __GotoWin(x1,y1+1);
  for n := 1 to (y2-y1-1) do
   begin
    __GotoWin(x1,y1+n);Write(#179);
   end;
  __GotoWin(x1,y2); Write(#192);
  for n := (x1+1) to (x2-1) do Write(#196);
  __QuikWin(x1+((x2-x1-Length(Title)) DIV 2)+1,y1,MainFore,MainBack,Title);
end;
