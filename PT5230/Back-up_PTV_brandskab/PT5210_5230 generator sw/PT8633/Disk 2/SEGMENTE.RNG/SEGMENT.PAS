Program Seg_ment;

Uses Crt,DOS,Unit_Sup,Unit_Mem,Unit_Str;

Label    Jumpout;

Const    Commax=3;
         Comcon:array[1..Commax,1..2] of string=(
         ('FirstLine','FL'),
         ('LastLine','LL'),
         ('SegmentLength','SL'));

Type     LineType=Array [1..4096] of word;
         AdressType=Array [1..512] of word;
         SegmentType=Array [1..512] of longint;
         CheckSumType=Array [1..65000] of byte;

Var      Dmem,Emem                                         :Word;
         Dptr                                              :Pointer;
         Eadr,Dadr                                         :_XAds;
         IFS,ILS,LLS,BS,LO,SO1,SO2,SO3,DacO1,DacO2,DacO3,
         DR1,DR2,DR3,F,NAL,NAS,AR,RN1,RN2,RN3,C4SR         :Double;
         F0                                                :File of double;
         Buf1,Buf2,Buf3                                    :^LineType;
         BufA                                              :^AdressType;
         BufS,BufD                                         :^SegmentType;
         BufC                                              :^CheckSumType;
         F1,F2,F3,F4,F5                                    :File;
         FL,LL,SL,Line,Segment                             :Word;
         SegCount,SegLen,SegMax,SegLast,SegSize            :Longint;
         Found                                             :Boolean;
         Y,CheckSumS                                       :Byte;
         C                                                 :Char;
         TimeH,TimeM,TimeS,TimeHS                          :Word;
         Time                                              :Longint;


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

Procedure Memory;
Begin    __AvailMem(Dmem,Emem,Dptr,Eadr);
         Writeln('Available memory is ',longint(Emem)/1024:6:3,' Mbytes.');
         If longint(Emem)=0 then
         Begin     Beep;
                   Writeln('Error - No extended memory found !!!');
                   Halt;
         End;
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
         Writeln('Command example: SEGMENT FL=1 LL=8 SL=32');
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
                   SL:=0;
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
                                       3: Begin  Val(ParamVal,SL,Error);
                                                 If Error<>0
                                                 then GetCommandError;
                                       End;
                             End;
                   End;
         End;
End;

{---------------------------------------------------------------------------}

Procedure CheckSumConvert(SegNum:Longint);
Var      S:Word;
         A:Word;
Begin    CheckSumS:=0;
         A:=(SegNum-1)*SegLen;
         For S:=1 to SegLen do
         CheckSumS:=CheckSumS +
              (Buf3^[A+S]) +
              (Buf2^[A+S] SHR 3) +
              (Buf1^[A+S] SHL 2);
End;

{Procedure CheckSumConvert(SegNum:Longint);
Var      S,B:Word;
         A:Longint;
Begin    CheckSumS:=0;
         A:=(SegNum-1)*SegLen;
         For S:=1 to Trunc(SegLen/8) do
         Begin     B:=A+S*4;
                   CheckSumS:=CheckSumS + Buf3^[B] +
                   (Buf2^[B] SHR 1) + (Buf1^[B] SHL 1);
         End;
End;}

{Procedure CheckSumConvert(SegNum:Longint);
Var      S,B:Word;
         A:Longint;
Begin    CheckSumS:=0;
         A:=(SegNum-1)*SegLen;
         For S:=1 to Trunc(SegLen/4) do
         Begin     B:=A+S*4;
                   CheckSumS:=CheckSumS + Buf3^[B] +
                   (Buf2^[B] SHR 1) + (Buf1^[B] SHL 1);
         End;
End;}

{Procedure CheckSumConvert(SegNum:Longint);
Var      S,B:Word;
         A:Longint;
Begin    A:=(SegNum-1)*SegLen+1;
         CheckSumS:=Buf3^[A] + (Buf2^[A] SHR 1) + (Buf1^[A] SHL 1);
End;}

Procedure CheckSumSave(SegNum:Longint);
Begin    BufC^[SegNum]:=CheckSumS;
End;

Function CheckSumLoadCompare(SegNum:Longint):Boolean;
Begin    CheckSumLoadCompare:=(CheckSumS=BufC^[SegNum]);
End;

{---------------------------------------------------------------------------}

Procedure SegLenError;
Begin    Beep;
         Writeln('Error - Segment length not compatible with block size !!!');
         Halt;
End;
Procedure SegLoad(SegNum:Longint);
Var      D:Longint;
         C:word;
Begin    D:=__2LongSup(Eadr._HiByte,Eadr._LoWord);
         D:=D+(((SegNum-1)*SegLen) shl 2);
         Dadr._HiByte:=__HiWrdSup(D);
         Dadr._LoWord:=__LoWrdSup(D);
         {Dadr points at next segm in ext mem}
         __XtMovMem(BufD,Dadr,SegLen*2,False,C);
         {Reading segment from extended memory}
End;

Procedure SegSave(SegNum:Longint);
Var      D:Longint;
         C:word;
Begin    D:=__2LongSup(Eadr._HiByte,Eadr._LoWord);
         D:=D+(((SegNum-1)*SegLen) shl 2);
         Dadr._HiByte:=__HiWrdSup(D);
         Dadr._LoWord:=__LoWrdSup(D);
         {Dadr points at first new segment}
         Move(BufS^,BufD^,SizeOf(BufD^));
         __XtMovMem(BufD,Dadr,SegLen*2,True,C);
         {Storing segment in extended memory}
End;

Procedure SegConvert(SegNum:Longint);
Var      S:Word;
Begin    For S:=1 to SegLen do
         BufS^[S]:=__2LongSup(
         (Buf3^[(SegNum-1)*SegLen+S] shl 4)+(Buf2^[(SegNum-1)*SegLen+S] shr 6),
         (Buf2^[(SegNum-1)*SegLen+S] shl 10)+Buf1^[(SegNum-1)*SegLen+S]);
End;

Function SegCompare:Boolean;
Var      S:word;
Begin    S:=1;
         While (BufS^[S]=BufD^[S]) and (S<=SegLen) do Inc(S);
         If S<=SegLen then SegCompare:=False else SegCompare:=True;
End;

Procedure SegFind;
Begin    Found:=CheckSumLoadCompare(SegCount);
         If Found then Found:=SegCompare;
         { Comparing with SegCount. }

         If (not Found) and (SegCount<SegLast) then
         Begin     SegCount:=BufA^[Segment]+1;
                   Found:=CheckSumLoadCompare(SegCount);
                   If Found then
                   Begin     SegLoad(SegCount);
                             Found:=SegCompare;
                   End;
         End;
         { Comparing with same as above line. }

         If not Found then
         Begin     SegCount:=0;
                   Repeat    Inc(SegCount);
                             Found:=CheckSumLoadCompare(SegCount);
                             If Found then
                             Begin     SegLoad(SegCount);
                                       Found:=SegCompare;
                             End;
                   Until Found or (SegCount=SegLast);
         End;
         { Comparing with the rest. }
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
         If (IOResult<>0) then FileError;

         Assign(F2,'CHANNEL2.DAT');
         Reset(F2,2);
         If (IOResult<>0) then FileError;

         Assign(F3,'CHANNEL3.DAT');
         Reset(F3,2);
         If (IOResult<>0) then FileError;

         Assign(F4,'SEGADR.DAT');
         Rewrite(F4,2);
         If (IOResult<>0) then FileError;

         Assign(F5,'SEGDAT.DAT');
         Rewrite(F5,4);
         If (IOResult<>0) then FileError;
         {$I+}
End;

Procedure FileLoad(Line:Longint);
Var      Num:Word;
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

         GotoXY(1,Y);
         Write('Processing line :',Line+1:5);
End;

Procedure FileSaveAdr(Line:Longint);
Var      Num:Word;
Begin    Seek(F4,Line*SegMax);
         BlockWrite(F4,BufA^,SegMax,Num);
         If SegMax<>Num then FileError;
End;

Procedure FileSaveSeg;
Var      Num:Word;
Begin    For SegCount:=1 to SegLast do
         Begin     GotoXY(1,Y);
                   Write('Storing segment :',SegCount:5,'=',SegCount/SegLast*100:4:1,'%');
                   SegLoad(SegCount);
                   Seek(F5,(SegCount-1)*SegLen);
                   BlockWrite(F5,BufD^,SegLen,Num);
                   If SegLen<>Num then FileError;
         End;
End;

{---------------------------------------------------------------------------}

Begin    GetTime(TimeH,TimeM,TimeS,TimeHS);
         Time:=(((longint(TimeH)*60)+TimeM)*60+TimeS)*100+TimeHS;

         GetChannel0;
         GetCommand;

         New(Buf1);
         New(Buf2);
         New(Buf3);
         New(BufA);
         New(BufS);
         New(BufD);
         New(BufC);

         FileOpen;
         Memory;

         SegLen:=Trunc(SL);
         SegMax:=Trunc(BS/SL);
         If Trunc(BS)<>SegLen*SegMax then SegLenError;

         SegSize:=longint(Emem)*256 div SegLen;
         Eadr._HiByte:=19+16;
         Y:=WhereY;

         Segment:=1;
         Line:=1;
         FileLoad(Trunc(LO)-2+1);
         SegConvert(1);
         CheckSumConvert(1);
         BufA^[1]:=0;
         SegSave(1);
         CheckSumSave(1);
         SegCount:=1;
         SegLast:=1;
         { Gemt f›rste segment. }

         For Line:=Trunc(FL+0.9999) to Trunc(LL) do
         Begin     FileLoad(Trunc(LO)-2+Line);
                   For Segment:=1 to SegMax do
                   Begin     SegConvert(Segment);
                             CheckSumConvert(Segment);
                             SegFind;
                             If not Found then
                             Begin     SegCount:=SegLast+1;
                                       SegSave(SegCount);
                                       CheckSumSave(SegCount);
                                       Inc(SegLast);
                             End;
                             BufA^[Segment]:=SegCount-1;
                   End;
                   FileSaveAdr(Trunc(LO)-2+Line);
                   If keypressed then
                   Begin     C:=readkey;
                             GotoXY(1,Y+2);
                             Write('Do you want to exit ? ');
                             repeat
                             until keypressed;
                             C:=readkey;
                             GotoXY(1,Y+2);
                             Write('                       ');
                             If (C='y') or (C='Y') then goto Jumpout;
                   End;
         End;

         Jumpout: FileSaveSeg;
         Writeln;

         Close(F1);
         Close(F2);
         Close(F3);
         Close(F4);
         Close(F5);

         Dispose(Buf1);
         Dispose(Buf2);
         Dispose(Buf3);
         Dispose(BufS);
         Dispose(BufD);
         Dispose(BufA);
         Dispose(BufC);

         GetTime(TimeH,TimeM,TimeS,TimeHS);
         Time:=(((longint(TimeH)*60)+TimeM)*60+TimeS)*100+TimeHS-Time;
         TimeHS:=Trunc(Frac(Time/100)*100);
         Time:=Trunc((Time-TimeHS)/100);
         TimeS:=Trunc(Frac(Time/60)*60);
         Time:=Trunc((Time-TimeS)/60);
         TimeM:=Trunc(Frac(Time/60)*60);
         TimeH:=Trunc((Time-TimeM)/60);
         Writeln;
         Writeln('Expired time:',TimeH,':',TimeM,':',TimeS,':',TimeHS);
End.
