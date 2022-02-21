Program Chanpic3;

Uses     Crt,Dos,Graph;

{$i svga32ka.inc}   {Fra grafik demo diskette}
{$i svga_put.inc}

Const    Commax=21;
         Comcon:array[1..Commax,1..2] of string=(
         ('NumberofActiveSamples','NAS'),
         ('NumberofActiveLines','NAL'),
         ('XZero0','XZ0'),
         ('YZero0','YZ0'),
         ('XZero1','XZ1'),
         ('YZero1','YZ1'),
         ('XZero2','XZ2'),
         ('YZero2','YZ2'),
         ('XOffs0','XO0'),
         ('YOffs0','YO0'),
         ('YOffs1','YO1'),
         ('YOffs2','YO2'),
         ('YOffs3','YO3'),
         ('YOffs4','YO4'),
         ('YOffs5','YO5'),
         ('YOffs6','YO6'),
         ('YOffs7','YO7'),
         ('YOffs8','YO8'),
         ('YOffs9','YO9'),
         ('Mode','M'),
         ('Channel','C'));
         BufLength=4095;

Type     LineType=Array [0..BufLength] of word;

Var      IFS,ILS,LLS,BS,LO,SO1,SO2,SO3,DacO1,DacO2,DacO3,
         DR1,DR2,DR3,F,NAL,NAS,AR,RN1,RN2,RN3,C4SR         :Double;
         F0                                                :File of double;
         F1,F2,F3                                          :File;
         Buf1,Buf2,Buf3                                    :^LineType;
         Linex,LineOld,Sample,BufSize                       :Integer;
         Xzero,Yzero,Xstep,Ystep,Xoffs,Yoffs               :Double;
         Def_Xzero0,Def_Xzero1,Def_Xzero2,
         Def_Yzero0,Def_Yzero1,Def_Yzero2,
         Def_Xstep,Def_Ystep,Def_Xoffs0,
         Def_Yoffs0,Def_Yoffs1,Def_Yoffs2,Def_Yoffs3,
         Def_Yoffs4,Def_Yoffs5,Def_Yoffs6,Def_Yoffs7,
         Def_Yoffs8,Def_Yoffs9,Def_Mode,Def_C              :Double;
         Val_Xzero0,Val_Xzero1,Val_Xzero2,
         Val_Yzero0,Val_Yzero1,Val_Yzero2,
         Val_Xoffs0,Val_Yoffs0,Val_Yoffs1,Val_Yoffs2,
         Val_Yoffs3,Val_Yoffs4,Val_Yoffs5,Val_Yoffs6,
         Val_Yoffs7,Val_Yoffs8,Val_Yoffs9                  :Boolean;
         C                                                 :Char;
         I                                                 :Longint;
         Number,Code                                       :Integer;
         VideoMode,Red,Green,Blue                          :Byte;
         X,Y                                               :Word;

	 GraphMode, GraphDriver   : integer; {Fra grafik demo diskette}
         Ky                       : Char;
         Drv                      : Integer;

{----------------------------------------------------------------------------}
{ ************************************************************************** }
{$F+}

function DetectVGA32k : Integer;
var Vid : Integer;

begin
  Writeln('Which video mode would you like to use?');
  Writeln('  0) 320x200x32768');
  Writeln('  1) 640x350x32768');
  Writeln('  2) 640x400x32768');
  Writeln('  3) 640x480x32768');
  Writeln('  4) 800x600x32768');
  Write('> ');
  Vid:=3;
  DetectVGA32k := Vid;
end;

{$F-}
{ ************************************************************************** }

Procedure Beep;
Begin    Sound(2000);
         Delay(200);
         NoSound;
End;

Procedure ColourRGB(Sample:Word);
Var      Temp1,Temp2,Temp3:Integer;
         TempColour:Longint;
Begin    Temp1:=Round(DR1*(Buf1^[Sample+Round(SO1)]-DacO1));
         Temp1:=Round(Temp1*256/700);
         If Temp1<0 then Temp1:=0;
         If 255<Temp1 then Temp1:=255;

         Temp2:=Round(DR2*(Buf2^[Sample+Round(SO2)]-DacO2));
         Temp2:=Round(Temp2*256/700);
         If Temp2<0 then Temp2:=0;
         If 255<Temp2 then Temp2:=255;

         Temp3:=Round(DR3*((Buf3^[Sample+Round(SO3)] and $3FF)-DacO3));
         Temp3:=Round(Temp3*256/700);
         If Temp3<0 then Temp3:=0;
         If 255<Temp3 then Temp3:=255;

         Green:=Round(Temp1);
         Blue:=Round(Temp2);
         Red:=Round(Temp3);
         Case Round(Def_C) of
         1:        Begin     Blue:=Green;
                             Red:=Green;
                   End;
         2:        Begin     Green:=Blue;
                             Red:=Blue;
                   End;
         3:        Begin     Green:=Red;
                             Blue:=Red;
                   End;
         End;
End;

Procedure ColourYUV(Sample:Word);
Var      TempY,TempU,TempV,Temp1,Temp2,Temp3:Double;
Begin    TempY:=DR1*(Buf1^[Sample+Round(SO1)]-DacO1);
         TempU:=DR2*(Buf2^[Sample+Round(SO2)]-DacO2);
         TempV:=DR3*((Buf3^[Sample+Round(SO3)] and $3FF)-DacO3);

         Temp1:=TempY-2*(0.299*(1-0.299)*TempV+0.114*(1-0.114)*TempU)/0.587;
         Temp1:=Round(Temp1*256/700);
         If Temp1<0 then Temp1:=0;
         If 255<Temp1 then Temp1:=255;

         Temp2:=2*(1-0.299)*TempU+TempY;
         Temp2:=Round(Temp2*256/700);
         If Temp2<0 then Temp2:=0;
         If 255<Temp2 then Temp2:=255;

         Temp3:=2*(1-0.114)*TempV+TempY;
         Temp3:=Round(Temp3*256/700);
         If Temp3<0 then Temp3:=0;
         If 255<Temp3 then Temp3:=255;

         Green:=Round(Temp1);
         Blue:=Round(Temp2);
         Red:=Round(Temp3);
         Case Round(Def_C) of
         1:        Begin     Blue:=Green;
                             Red:=Green;
                   End;
         2:        Begin     Green:=Blue;
                             Red:=Blue;
                   End;
         3:        Begin     Green:=Red;
                             Blue:=Red;
                   End;
         End;
End;

Procedure ColourYUV_Pal(Sample:Word);
Var      TempY,TempU,TempV,Temp1,Temp2,Temp3:Double;
Begin    TempY:=DR1*(Buf1^[Sample+Round(SO1)]-DacO1);
         TempU:=DR2*(Buf2^[Sample+Round(SO2)]-DacO2);
         TempV:=DR3*((Buf3^[Sample+Round(SO3)] and $3FF)-DacO3);

         Temp1:=TempY-(0.299*TempV/0.877+0.114*TempU/0.493)/0.587;
         Temp1:=Round(Temp1*256/700);
         If Temp1<0 then Temp1:=0;
         If 255<Temp1 then Temp1:=255;

         Temp2:=TempU/0.493+TempY;
         Temp2:=Round(Temp2*256/700);
         If Temp2<0 then Temp2:=0;
         If 255<Temp2 then Temp2:=255;

         Temp3:=TempV/0.877+TempY;
         Temp3:=Round(Temp3*256/700);
         If Temp3<0 then Temp3:=0;
         If 255<Temp3 then Temp3:=255;

         Green:=Round(Temp1);
         Blue:=Round(Temp2);
         Red:=Round(Temp3);
         Case Round(Def_C) of
         1:        Begin     Blue:=Green;
                             Red:=Green;
                   End;
         2:        Begin     Green:=Blue;
                             Red:=Blue;
                   End;
         3:        Begin     Green:=Red;
                             Blue:=Red;
                   End;
         End;
End;

Procedure ColourYUV_Ntsc(Sample:Word);
Var      TempY,TempU,TempV,Temp1,Temp2,Temp3:Double;
Begin    TempY:=DR1*(Buf1^[Sample+Round(SO1)]-DacO1)-53.55;
         TempU:=DR2*(Buf2^[Sample+Round(SO2)]-DacO2);
         TempV:=DR3*((Buf3^[Sample+Round(SO3)] and $3FF)-DacO3);

         Temp1:=TempY-(0.299*TempV/0.877+0.114*TempU/0.493)/0.587;
         Temp1:=Round(Temp1*256/(714-53.55));
         If Temp1<0 then Temp1:=0;
         If 255<Temp1 then Temp1:=255;

         Temp2:=TempU/0.493+TempY;
         Temp2:=Round(Temp2*256/(714-53.55));
         If Temp2<0 then Temp2:=0;
         If 255<Temp2 then Temp2:=255;

         Temp3:=TempV/0.877+TempY;
         Temp3:=Round(Temp3*256/(714-53.55));
         If Temp3<0 then Temp3:=0;
         If 255<Temp3 then Temp3:=255;

         Green:=Round(Temp1);
         Blue:=Round(Temp2);
         Red:=Round(Temp3);
         Case Round(Def_C) of
         1:        Begin     Blue:=Green;
                             Red:=Green;
                   End;
         2:        Begin     Green:=Blue;
                             Red:=Blue;
                   End;
         3:        Begin     Green:=Red;
                             Blue:=Red;
                   End;
         End;
End;

Procedure ColourYDbDr_Secam(Sample:Word);
Var      TempY,TempU,TempV,Temp1,Temp2,Temp3:Double;
Begin    TempY:=DR1*(Buf1^[Sample+Round(SO1)]-DacO1);
         TempU:=DR2*(Buf2^[Sample+Round(SO2)]-DacO2);
         TempV:=DR3*((Buf3^[Sample+Round(SO3)] and $3FF)-DacO3);
         TempU:=TempU/3.053;
         TempV:=TempV/-2.169;

         Temp1:=TempY-(0.299*TempV/0.877+0.114*TempU/0.493)/0.587;
         Temp1:=Round(Temp1*256/700);
         If Temp1<0 then Temp1:=0;
         If 255<Temp1 then Temp1:=255;

         Temp2:=TempU/0.493+TempY;
         Temp2:=Round(Temp2*256/700);
         If Temp2<0 then Temp2:=0;
         If 255<Temp2 then Temp2:=255;

         Temp3:=TempV/0.877+TempY;
         Temp3:=Round(Temp3*256/700);
         If Temp3<0 then Temp3:=0;
         If 255<Temp3 then Temp3:=255;

         Green:=Round(Temp1);
         Blue:=Round(Temp2);
         Red:=Round(Temp3);
         Case Round(Def_C) of
         1:        Begin     Blue:=Green;
                             Red:=Green;
                   End;
         2:        Begin     Green:=Blue;
                             Red:=Blue;
                   End;
         3:        Begin     Green:=Red;
                             Blue:=Red;
                   End;
         End;
End;

Function UpString(T:String):String;
Var      Temp:String;
         I:Byte;
Begin    Temp:=T;
         For I:=1 to Length(Temp) do Temp[I]:=UpCase(Temp[I]);
         UpString:=Temp;
End;

{----------------------------------------------------------------------------}

Procedure GetChannel0;
Begin    Assign(F0,'CHANNEL0.DAT');
         Reset(F0);
         If IOResult<>0 then
         Begin     Writeln('Error during opening of CHANNEL0 - program aborted !');
                   Halt;
         End;
         Seek(F0,0);
         Read(F0,IFS);
         Seek(F0,1);
         Read(F0,ILS);
         Seek(F0,2);
         Read(F0,LLS);
         Seek(F0,3);
         Read(F0,BS);
         Seek(F0,4);
         Read(F0,LO);
         Seek(F0,5);
         Read(F0,SO1);
         Seek(F0,6);
         Read(F0,SO2);
         Seek(F0,7);
         Read(F0,SO3);
         Seek(F0,8);
         Read(F0,DacO1);
         Seek(F0,9);
         Read(F0,DacO2);
         Seek(F0,10);
         Read(F0,DacO3);
         Seek(F0,11);
         Read(F0,DR1);
         Seek(F0,12);
         Read(F0,DR2);
         Seek(F0,13);
         Read(F0,DR3);
         Seek(F0,14);
         Read(F0,F);
         Seek(F0,15);
         Read(F0,NAL);
         Seek(F0,16);
         Read(F0,NAS);
         Seek(F0,17);
         Read(F0,AR);
         Seek(F0,18);
         Read(F0,RN1);
         Seek(F0,19);
         Read(F0,RN2);
         Seek(F0,20);
         Read(F0,RN3);
         Seek(F0,21);
         Read(F0,C4SR);
         Close(F0);
End;

{----------------------------------------------------------------------------}

Procedure GetCommandError;
Begin    Beep;
         Writeln('Command error - No processing done !!!');
         Halt;
End;

Procedure GetCommandExample;
Var      I:Integer;
Begin    Write('Valid commands are:');
         For I:=1 to Commax do Write(' ',Comcon[I,1]);
         Writeln;
         Writeln('Command example: CHANPICT NAS=1040 NAL=576 XZ1=200 XZ2=400 YZ1=48 YZ2=96 XO0=218 YO0=44');
         Writeln('Mode 0=RGB component, 1=YUV component, 2=YUV Pal, 3=YUV Ntsc, 4=YDbDr Secam.');
         Halt;
End;

Procedure GetCommand;
Var      I,J,EqPos,Error:Integer;
         ParamVal,ParamCode:String;
Begin    If ParamCount=0 then GetCommandError
         Else
         Begin     If (UpString(ParamStr(1))='H')
                   or (UpString(ParamStr(1))='/H')
                   or (UpString(ParamStr(1))='HELP') then GetCommandExample;
                   NAS:=0;
                   NAL:=0;
                   Def_Xzero0:=0;
                   Def_Yzero0:=0;
                   Def_Xzero1:=0;
                   Def_Yzero1:=0;
                   Def_Xzero2:=0;
                   Def_Yzero2:=0;
                   Def_Xoffs0:=0;
                   Def_Yoffs0:=0;
                   Def_Yoffs1:=0;
                   Def_Yoffs2:=0;
                   Def_Yoffs3:=0;
                   Def_Yoffs4:=0;
                   Def_Yoffs5:=0;
                   Def_Yoffs6:=0;
                   Def_Yoffs7:=0;
                   Def_Yoffs8:=0;
                   Def_Yoffs9:=0;
                   Def_Mode:=0;
                   Def_C:=0;
                   Val_Xzero0:=False;
                   Val_Yzero0:=False;
                   Val_Xzero1:=False;
                   Val_Yzero1:=False;
                   Val_Xzero2:=False;
                   Val_Yzero2:=False;
                   Val_Xoffs0:=False;
                   Val_Yoffs0:=False;
                   Val_Yoffs1:=False;
                   Val_Yoffs2:=False;
                   Val_Yoffs3:=False;
                   Val_Yoffs4:=False;
                   Val_Yoffs5:=False;
                   Val_Yoffs6:=False;
                   Val_Yoffs7:=False;
                   Val_Yoffs8:=False;
                   Val_Yoffs9:=False;
                   For I:=1 to ParamCount do
                   Begin     J:=1;
                             EqPos:=Pos('=',ParamStr(I))+Pos(':',ParamStr(I));
                             ParamVal:=Copy(ParamStr(I),EqPos+1,
                                            Length(ParamStr(I))-EqPos);
                             ParamCode:=Copy(UpString(ParamStr(I)),1,EqPos-1);
                             While (J<=Commax)
                             and (ParamCode<>UpString(ComCon[J,1]))
                             and (ParamCode<>UpString(ComCon[J,2]))
                             do Inc(J);
                             If Commax<J then GetCommandError;
                             Case J of
                                       1: Begin  Val(ParamVal,NAS,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                       End;
                                       2: Begin  Val(ParamVal,NAL,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                       End;
                                       3: Begin  Val(ParamVal,Def_Xzero0,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                                 Val_Xzero0:=True;
                                       End;
                                       4: Begin  Val(ParamVal,Def_Yzero0,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                                 Val_Yzero0:=True;
                                       End;
                                       5: Begin  Val(ParamVal,Def_Xzero1,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                                 Val_Xzero1:=True;
                                       End;
                                       6: Begin  Val(ParamVal,Def_Yzero1,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                                 Val_Yzero1:=True;
                                       End;
                                       7: Begin  Val(ParamVal,Def_Xzero2,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                                 Val_Xzero2:=True;
                                       End;
                                       8: Begin  Val(ParamVal,Def_Yzero2,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                                 Val_Yzero2:=True;
                                       End;
                                       9: Begin  Val(ParamVal,Def_Xoffs0,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                                 Val_Xoffs0:=True;
                                       End;
                                       10: Begin  Val(ParamVal,Def_Yoffs0,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                                 Val_Yoffs0:=True;
                                       End;
                                       11: Begin  Val(ParamVal,Def_Yoffs1,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                                 Val_Yoffs1:=True;
                                       End;
                                       12: Begin  Val(ParamVal,Def_Yoffs2,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                                 Val_Yoffs2:=True;
                                       End;
                                       13: Begin  Val(ParamVal,Def_Yoffs3,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                                 Val_Yoffs3:=True;
                                       End;
                                       14: Begin  Val(ParamVal,Def_Yoffs4,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                                 Val_Yoffs4:=True;
                                       End;
                                       15: Begin  Val(ParamVal,Def_Yoffs5,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                                 Val_Yoffs5:=True;
                                       End;
                                       16: Begin  Val(ParamVal,Def_Yoffs6,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                                 Val_Yoffs6:=True;
                                       End;
                                       17: Begin  Val(ParamVal,Def_Yoffs7,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                                 Val_Yoffs7:=True;
                                       End;
                                       18: Begin  Val(ParamVal,Def_Yoffs8,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                                 Val_Yoffs8:=True;
                                       End;
                                       19: Begin  Val(ParamVal,Def_Yoffs9,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                                 Val_Yoffs9:=True;
                                       End;
                                       20: Begin  Val(ParamVal,Def_Mode,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                       End;
                                       21: Begin  Val(ParamVal,Def_C,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                       End;
                             End;
                   End;
         End;
End;

{----------------------------------------------------------------------------}

Procedure FileError;
Begin    Beep;
         Write('File I/O error - Program aborted !!!');
         Halt;
End;

Procedure FileOpen;
Begin    Assign(F1,'CHANNEL1.DAT');
         Reset(F1,2);
         If (IOResult<>0) then halt;

         Assign(F2,'CHANNEL2.DAT');
         Reset(F2,2);
         If (IOResult<>0) then halt;

         Assign(F3,'CHANNEL3.DAT');
         Reset(F3,2);
         If (IOResult<>0) then halt;
End;

Procedure FileLoad(Linex:Word);
Var      C:Integer;
Begin    Seek(F1,Linex*BufSize);
         BlockRead(F1,Buf1^,BufSize,C);
         If C<>BufSize then FileError;

         Seek(F2,Linex*BufSize);
         BlockRead(F2,Buf2^,BufSize,C);
         If C<>BufSize then FileError;

         Seek(F3,Linex*BufSize);
         BlockRead(F3,Buf3^,BufSize,C);
         If C<>BufSize then FileError;


         FOR C:= Bufsize-64 DOWNTO 1 DO
           BEGIN
             Buf1^[C+32]:=Buf1^[C];
             Buf2^[C+32]:=Buf2^[C];
             Buf3^[C+32]:=Buf3^[C];
           END;
         FOR C:=1 TO 32 DO
           BEGIN
             Buf1^[C]:=Buf1^[C+832];
             Buf2^[C]:=Buf2^[C+832];
             Buf3^[C]:=Buf3^[C+832];
           END;
End;

{----------------------------------------------------------------------------}

Procedure PlotPixel;
Var     WhitePixel: Word;
Begin
        Red:= (Red SHR 3) AND 31;
        Green:=(Green SHR 3) AND 31;
        Blue:=(Blue SHR 3) AND 31 ;
        WhitePixel:=Red SHL 10 + Green SHL 5 + Blue;
        Setcolor(RealDrawColor(WhitePixel));
        line (X,Y,X,Y);
End;

Procedure PlotGraph;
Begin    
         For Y:=0 to 479 do
         Begin     Linex:=Trunc(Yoffs+Yzero+Y*Ystep);
                   If Linex<>LineOld then
                   Begin     LineOld:=Linex;
                             FileLoad(Linex+Round(LO)-1);
                   End;
                   IF KEYPRESSED THEN EXIT;
                   For X:=0 to 639 do
                   Begin     Sample:=Round(Xoffs+Xzero+X*Xstep);
                             Case Round(Def_Mode) of
                             0: ColourRGB(Sample);
                             1: ColourYUV(Sample);
                             2: ColourYUV_Pal(Sample);
                             3: ColourYUV_Ntsc(Sample);
                             4: ColourYDbDr_Secam(Sample);
                             End;
                             PlotPixel
                   End;
         End;
End;

{----------------------------------------------------------------------------}

Begin    Getchannel0;
         GetCommand;
         Def_Xstep:=NAS/640;
         Def_Ystep:=NAL/480;

         Writeln('Which driver would you like to use?');
         Writeln('  0) Svga256');
         Writeln('  1) Svga16');
         Writeln('  2) Tweak256');
         Writeln('  3) Tweak16');
         Writeln('  4) Svga32k');{Denne er valgt}
         Writeln('  5) SvgaS3');
         Write('>');
         GraphDriver := InstallUserDriver('Svga32k',@DetectVGA32k);
         GraphDriver := Detect;
         InitGraph(GraphDriver,GraphMode,'');


         FileOpen;

         Xstep:=Def_Xstep;
         Ystep:=Def_Ystep;
         Xzero:=Def_Xzero0;
         Yzero:=Def_Yzero0;
         Xoffs:=Def_Xoffs0;
         Yoffs:=Def_Yoffs0;
         BufSize:=Trunc(BS);
         LineOld:=0;

         New(Buf1);
         New(Buf2);
         New(Buf3);
         For Sample:=0 to BufLength do Buf1^[Sample]:=Round(DacO1);
         For Sample:=0 to BufLength do Buf2^[Sample]:=Round(DacO2);
         For Sample:=0 to BufLength do Buf3^[Sample]:=Round(DacO3);
         PlotGraph;

         Repeat    Repeat
                   Until KeyPressed;

                   C:=UpCase(ReadKey);

                   If (C='0') and (Xstep<>Def_Xstep) then
                   Begin     Xstep:=Def_Xstep;
                             Ystep:=Def_Ystep;
                             Xzero:=Def_Xzero0;
                             Yzero:=Def_Yzero0;
                             PlotGraph;
                   End;

                   If (C='7') and not((Xstep=1) and (Xzero=Def_Xzero0) and
                      (Yzero=Def_Yzero0)) and Val_Xzero0 and Val_Yzero0 then
                   Begin     Xstep:=1;
                             Ystep:=1;
                             Xzero:=Def_Xzero0;
                             Yzero:=Def_Yzero0;
                             PlotGraph;
                   End;

                   If (C='8') and not((Xstep=1) and (Xzero=Def_Xzero1) and
                      (Yzero=Def_Yzero0)) and Val_Xzero1 and Val_Yzero0 then
                   Begin     Xstep:=1;
                             Ystep:=1;
                             Xzero:=Def_Xzero1;
                             Yzero:=Def_Yzero0;
                             PlotGraph;
                   End;

                   If (C='9') and not((Xstep=1) and (Xzero=Def_Xzero2) and
                      (Yzero=Def_Yzero0)) and Val_Xzero2 and Val_Yzero0 then
                   Begin     Xstep:=1;
                             Ystep:=1;
                             Xzero:=Def_Xzero2;
                             Yzero:=Def_Yzero0;
                             PlotGraph;
                   End;

                   If (C='4') and not((Xstep=1) and (Xzero=Def_Xzero0) and
                      (Yzero=Def_Yzero1)) and Val_Xzero0 and Val_Yzero1 then
                   Begin     Xstep:=1;
                             Ystep:=1;
                             Xzero:=Def_Xzero0;
                             Yzero:=Def_Yzero1;
                             PlotGraph;
                   End;

                   If (C='5') and not((Xstep=1) and (Xzero=Def_Xzero1) and
                      (Yzero=Def_Yzero1)) and Val_Xzero1 and Val_Yzero1 then
                   Begin     Xstep:=1;
                             Ystep:=1;
                             Xzero:=Def_Xzero1;
                             Yzero:=Def_Yzero1;
                             PlotGraph;
                   End;

                   If (C='6') and not((Xstep=1) and (Xzero=Def_Xzero2) and
                      (Yzero=Def_Yzero1)) and Val_Xzero2 and Val_Yzero1 then
                   Begin     Xstep:=1;
                             Ystep:=1;
                             Xzero:=Def_Xzero2;
                             Yzero:=Def_Yzero1;
                             PlotGraph;
                   End;

                   If (C='1') and not((Xstep=1) and (Xzero=Def_Xzero0) and
                      (Yzero=Def_Yzero2)) and Val_Xzero0 and Val_Yzero2 then
                   Begin     Xstep:=1;
                             Ystep:=1;
                             Xzero:=Def_Xzero0;
                             Yzero:=Def_Yzero2;
                             PlotGraph;
                   End;

                   If (C='2') and not((Xstep=1) and (Xzero=Def_Xzero1) and
                      (Yzero=Def_Yzero2)) and Val_Xzero1 and Val_Yzero2 then
                   Begin     Xstep:=1;
                             Ystep:=1;
                             Xzero:=Def_Xzero1;
                             Yzero:=Def_Yzero2;
                             PlotGraph;
                   End;

                   If (C='3') and not((Xstep=1) and (Xzero=Def_Xzero2) and
                      (Yzero=Def_Yzero2)) and Val_Xzero2 and Val_Yzero2 then
                   Begin     Xstep:=1;
                             Ystep:=1;
                             Xzero:=Def_Xzero2;
                             Yzero:=Def_Yzero2;
                             PlotGraph;
                   End;

                   If (C='A') and (Yoffs<>Def_Yoffs0) and Val_Yoffs0 then
                   Begin     Yoffs:=Def_Yoffs0;
                             PlotGraph;
                   End;

                   If (C='B') and (Yoffs<>Def_Yoffs1) and Val_Yoffs1 then
                   Begin     Yoffs:=Def_Yoffs1;
                             PlotGraph;
                   End;

                   If (C='C') and (Yoffs<>Def_Yoffs2) and Val_Yoffs2 then
                   Begin     Yoffs:=Def_Yoffs2;
                             PlotGraph;
                   End;

                   If (C='D') and (Yoffs<>Def_Yoffs3) and Val_Yoffs3 then
                   Begin     Yoffs:=Def_Yoffs3;
                             PlotGraph;
                   End;

                   If (C='E') and (Yoffs<>Def_Yoffs4) and Val_Yoffs4 then
                   Begin     Yoffs:=Def_Yoffs4;
                             PlotGraph;
                   End;

                   If (C='F') and (Yoffs<>Def_Yoffs5) and Val_Yoffs5 then
                   Begin     Yoffs:=Def_Yoffs5;
                             PlotGraph;
                   End;

                   If (C='G') and (Yoffs<>Def_Yoffs6) and Val_Yoffs6 then
                   Begin     Yoffs:=Def_Yoffs6;
                             PlotGraph;
                   End;

                   If (C='H') and (Yoffs<>Def_Yoffs7) and Val_Yoffs7 then
                   Begin     Yoffs:=Def_Yoffs7;
                             PlotGraph;
                   End;

                   If (C='I') and (Yoffs<>Def_Yoffs8) and Val_Yoffs8 then
                   Begin     Yoffs:=Def_Yoffs8;
                             PlotGraph;
                   End;

                   If (C='J') and (Yoffs<>Def_Yoffs9) and Val_Yoffs9 then
                   Begin     Yoffs:=Def_Yoffs9;
                             PlotGraph;
                   End;


                   Until C='Q';
              CloseGraph;
End.
