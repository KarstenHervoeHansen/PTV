Program Tiffdump;

Uses     Crt;

Const    Commax=6;
         Comcon:array[1..Commax,1..2] of string=(
         ('FirstLine','FL'),
         ('LastLine','LL'),
         ('FirstSample','FS'),
         ('LastSample','LS'),
         ('LogoOFfset','LOF'),
         ('Mode','M'));

Type     LineType=Array [1..4096] of Word;
         TiffType=Array [1..12288] of Byte;

Var      IFS,ILS,LLS,BS,LO,SO1,SO2,SO3,DacO1,DacO2,DacO3,
         DR1,DR2,DR3,F,NAL,NAS,AR,RN1,RN2,RN3,C4SR         :Double;
         F0                                                :File of double;
         Buf1,Buf2,Buf3                                    :^LineType;
         Buf4                                              :^TiffType;
         F1,F2,F3,F4                                       :File;
         Path                                              :String;
         FL,LL,FS,LS,LOF,M,PBFL,PBLL,FLine,LLine           :Double;
         Line                                              :Longint;


{---------------------------------------------------------------------------}

Procedure Beep;
Begin    Sound(2000);
         Delay(200);
         NoSound;
End;

Function UpString(T:String):String;
Var      I:Byte;
         S:String;
Begin    S:='';
         For I:=1 to Length(T) do S:=S+UpCase(T[I]);
         UpString:=S;
End;

{---------------------------------------------------------------------------}

Procedure GetChannel0Error;
Begin    Beep;
         Writeln('Error reading from CHANNEL0 - program aborted !');
         Halt;
End;

Procedure GetChannel0;
Begin    {$I-}
         Assign(F0,'CHANNEL0.DAT');
         Reset(F0);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,0);
         Read(F0,IFS);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,1);
         Read(F0,ILS);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,2);
         Read(F0,LLS);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,3);
         Read(F0,BS);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,4);
         Read(F0,LO);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,5);
         Read(F0,SO1);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,6);
         Read(F0,SO2);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,7);
         Read(F0,SO3);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,8);
         Read(F0,DacO1);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,9);
         Read(F0,DacO2);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,10);
         Read(F0,DacO3);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,11);
         Read(F0,DR1);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,12);
         Read(F0,DR2);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,13);
         Read(F0,DR3);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,14);
         Read(F0,F);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,15);
         Read(F0,NAL);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,16);
         Read(F0,NAS);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,17);
         Read(F0,AR);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,18);
         Read(F0,RN1);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,19);
         Read(F0,RN2);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,20);
         Read(F0,RN3);
         If IOResult<>0 then GetChannel0Error;
         Seek(F0,21);
         Read(F0,C4SR);
         If IOResult<>0 then GetChannel0Error;
         Close(F0);
         If IOResult<>0 then GetChannel0Error;
         {$I+}
End;

{---------------------------------------------------------------------------}

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
         Writeln('Command example: TIFFDUMP FL=1 LL=576 FS=100 LS=801 LOF=200 M=1');
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
                   FL:=1;
                   LL:=1;
                   FS:=1;
                   LS:=1;
                   LOF:=0;
                   M:=0;
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
                                       1: Begin  Val(ParamVal,FL,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                       End;
                                       2: Begin  Val(ParamVal,LL,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                       End;
                                       3: Begin  Val(ParamVal,FS,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                       End;
                                       4: Begin  Val(ParamVal,LS,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                       End;
                                       5: Begin  Val(ParamVal,LOF,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                       End;
                                       6: Begin  Val(ParamVal,M,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                       End;
                             End;
                   End;
         End;
End;

{---------------------------------------------------------------------------}

Procedure FileError;
Begin    Beep;
         Write('File I/O error - Program aborted !!!');
         Halt;
End;

Procedure FileOpen;
Begin    {$I-}
         Assign(F1,'CHANNEL1.DAT');
         Reset(F1,2);
         If (IOResult<>0) then halt;

         Assign(F2,'CHANNEL2.DAT');
         Reset(F2,2);
         If (IOResult<>0) then halt;

         Assign(F3,'CHANNEL3.DAT');
         Reset(F3,2);
         If (IOResult<>0) then halt;

         Assign(F4,'TIFFDUMP.BMP');
         Reset(F4,1);
         If (IOResult<>0) then FileError;
         {$I+}
End;

Procedure FileLoad(Line:Longint);
Var      Num,I:Word;
Begin    {$I-}
         Seek(F1,Line*Trunc(BS));
         BlockRead(F1,Buf1^,Trunc(BS),Num);
         If Trunc(BS)<>Num then FileError;

         Seek(F2,Line*Trunc(BS));
         BlockRead(F2,Buf2^,Trunc(BS),Num);
         If Trunc(BS)<>Num then FileError;

         Seek(F3,Line*Trunc(BS));
         BlockRead(F3,Buf3^,Trunc(BS),Num);
         If Trunc(BS)<>Num then FileError;
         {$I+}
End;

Procedure FileSave(Line:Longint);
Var      Num:Word;
Begin    {$I-}
         Seek(F4,Line*3*Trunc(LS-FS+1)+Trunc(LOF));
         BlockWrite(F4,Buf4^,3*720,Num); {Trunc(LS-FS+1)}
         If 3*Trunc(LS-FS+1)<>Num then FileError;
         {$I+}
End;

{---------------------------------------------------------------------------}
Procedure ColourRGB(Sample:Word);
Var      Green,Red,Blue,Y,U,V:Double;
Begin    Green:=Round(DR1*(Buf1^[Sample+Round(SO1)]-DacO1));
         Blue:=Round(DR2*(Buf2^[Sample+Round(SO2)]-DacO2));
         Red:=Round(DR3*((Buf3^[Sample+Round(SO3)] and $3FF)-DacO3));
         If Green<0 then Green:=0;
         If 700<Green then Green:=700;
         If Blue<0 then Blue:=0;
         If 700<Blue then Blue:=700;
         If Red<0 then Red:=0;
         If 700<Red then Red:=700;
         Buf4^[(Sample-Trunc(FS+0.9999))*3+2]:=(Trunc(Green/700*255) and $FF);
         Buf4^[(Sample-Trunc(FS+0.9999))*3+3]:=(Trunc(Blue/700*255) and $FF);
         Buf4^[(Sample-Trunc(FS+0.9999))*3+1]:=(Trunc(Red/700*255) and $FF);
End;

Procedure ColourYUV(Sample:Word);
Var      Green,Red,Blue,Y,U,V:Double;
Begin    Y:=DR1*(Buf1^[Sample+Round(SO1)]-DacO1);
         U:=DR2*(Buf2^[Sample+Round(SO2)]-DacO2);
         V:=DR3*((Buf3^[Sample+Round(SO3)] and $3FF)-DacO3);
         Blue:=U/0.564+Y;
         Red:=V/0.713+Y;
         Green:=(Y-0.299*Red-0.114*Blue)/0.587;
         If Green<0 then Green:=0;
         If 700<Green then Green:=700;
         If Blue<0 then Blue:=0;
         If 700<Blue then Blue:=700;
         If Red<0 then Red:=0;
         If 700<Red then Red:=700;
         Buf4^[(Sample-Trunc(FS+0.9999))*3+2]:=(Trunc(Green/700*255) and $FF);
         Buf4^[(Sample-Trunc(FS+0.9999))*3+3]:=(Trunc(Blue/700*255) and $FF);
         Buf4^[(Sample-Trunc(FS+0.9999))*3+1]:=(Trunc(Red/700*255) and $FF);
End;

Procedure ColourYUV_Pal(Sample:Word);
Var      Green,Red,Blue,Y,U,V:Double;
Begin    Y:=DR1*(Buf1^[Sample+Round(SO1)]-DacO1);
         U:=DR2*(Buf2^[Sample+Round(SO2)]-DacO2);
         V:=DR3*((Buf3^[Sample+Round(SO3)] and $3FF)-DacO3);
         Blue:=U/0.493+Y;
         Red:=V/0.877+Y;
         Green:=(Y-0.299*Red-0.114*Blue)/0.587;
         If Green<0 then Green:=0;
         If 700<Green then Green:=700;
         If Blue<0 then Blue:=0;
         If 700<Blue then Blue:=700;
         If Red<0 then Red:=0;
         If 700<Red then Red:=700;
         Buf4^[(Sample-Trunc(FS+0.9999))*3+3]:=(Trunc(Green/700*255) and $FF);
         Buf4^[(Sample-Trunc(FS+0.9999))*3+2]:=(Trunc(Blue/700*255) and $FF);
         Buf4^[(Sample-Trunc(FS+0.9999))*3+1]:=(Trunc(Red/700*255) and $FF);
End;

Procedure ColourYUV_Ntsc(Sample:Word);
Var      Green,Red,Blue,Y,U,V:Double;
Begin    Y:=DR1*(Buf1^[Sample+Round(SO1)]-DacO1)-53.55;
         U:=DR2*(Buf2^[Sample+Round(SO2)]-DacO2);
         V:=DR3*((Buf3^[Sample+Round(SO3)] and $3FF)-DacO3);
         Blue:=U/0.493+Y;
         Red:=V/0.877+Y;
         Green:=(Y-0.299*Red-0.114*Blue)/0.587;
         If Green<0 then Green:=0;
         If 700<Green then Green:=700;
         If Blue<0 then Blue:=0;
         If 700<Blue then Blue:=700;
         If Red<0 then Red:=0;
         If 700<Red then Red:=700;
         Buf4^[(Sample-Trunc(FS+0.9999))*3+2]:=(Trunc(Green/700*255) and $FF);
         Buf4^[(Sample-Trunc(FS+0.9999))*3+3]:=(Trunc(Blue/700*255) and $FF);
         Buf4^[(Sample-Trunc(FS+0.9999))*3+1]:=(Trunc(Red/700*255) and $FF);
End;

Procedure ColourYDbDr_Secam(Sample:Word);
Var      Green,Red,Blue,Y,U,V:Double;
Begin    Y:=DR1*(Buf1^[Sample+Round(SO1)]-DacO1);
         U:=DR2*(Buf2^[Sample+Round(SO2)]-DacO2);
         V:=DR3*((Buf3^[Sample+Round(SO3)] and $3FF)-DacO3);
         U:=U/3.053;
         V:=V/-2.169;
         Blue:=U/1.505+Y;
         Red:=V/-1.902+Y;
         Green:=(Y-0.299*Red-0.114*Blue)/0.587;
         If Green<0 then Green:=0;
         If 700<Green then Green:=700;
         If Blue<0 then Blue:=0;
         If 700<Blue then Blue:=700;
         If Red<0 then Red:=0;
         If 700<Red then Red:=700;
         Buf4^[(Sample-Trunc(FS+0.9999))*3+2]:=(Trunc(Green/700*255) and $FF);
         Buf4^[(Sample-Trunc(FS+0.9999))*3+3]:=(Trunc(Blue/700*255) and $FF);
         Buf4^[(Sample-Trunc(FS+0.9999))*3+1]:=(Trunc(Red/700*255) and $FF);
End;

Procedure Process;
Var      Sample:word;
Begin    For Sample:=Trunc(FS+0.9999) to Trunc(LS) do
         Case Trunc(M) of
         0: ColourRGB(Sample);
         1: ColourYUV(Sample);
         2: ColourYUV_Pal(Sample);
         3: ColourYUV_Ntsc(Sample);
         4: ColourYDbDr_Secam(Sample);
         End;
End;

{---------------------------------------------------------------------------}

Begin    GetChannel0;
         GetCommand;

         FileOpen;
         New(Buf1);
         New(Buf2);
         New(Buf3);
         New(Buf4);

         For Line:=Trunc(FL+0.9999) to Trunc(LL) do
         Begin     FileLoad(Trunc(LO)-2+Line);
                   Process;
                   FileSave(Trunc(LL)-(Line));
                   writeln('Line',Line,'  ',(Trunc(LL)));
         End;

End.

