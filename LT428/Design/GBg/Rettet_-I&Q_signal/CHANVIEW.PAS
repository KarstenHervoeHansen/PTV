Program lineview;

Uses     Crt,Graph;

Const    InitCol=2;

Type     LineType=Array [0..4095] of word;

Var      IFS,ILS,LLS,BS,LO,SO1,SO2,SO3,DacO1,DacO2,DacO3,
         DR1,DR2,DR3,F,NAL,NAS,AR,RN1,RN2,RN3,C4SR         :Double;
         Channum,Linnum,Xstart,Xmark,Zoom,Dot,Dold,
         Overwrite,Zero,Trin,Max,C0A,C0B,C90A,C90B,C180A,
         C180B,C270A,C270B,X,TextStart                     :Integer;
         Res                                               :Double;
         F0                                                :File of double;
         F1,F2,F3                                          :File;
         Buf                                               :^LineType;
         C,BufSize                                         :Word;
         Key                                               :Char;
         KeyString                                         :String[3];

Procedure Getchannel0;
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

Procedure Setupgraph;
Var      GraphDriver,GraphMode,I:Integer;
         T:String;
Begin    GraphDriver:=Detect;
         InitGraph(GraphDriver,GraphMode,'C:\TP\BGI');
         If GraphResult<>grOk then Halt;
         SetColor(15);
         Line(0,0,639,0);
         For I:=0 to 63 do PutPixel(I*10,1,15);
         Line(0,479,639,479);
         For I:=0 to 63 do PutPixel(I*10,478,15);
         Line(0,0,0,479);
         For I:=0 to 47 do PutPixel(1,I*10,15);
         Line(639,0,639,479);
         For I:=0 to 47 do PutPixel(638,I*10,15);
         Line(19,69,620,69);
         Line(19,460,620,460);
         Line(19,69,19,460);
         Line(620,69,620,460);
         SetViewPort(0,20,639,40,ClipOn);
         T:='(    :        )  Clear Zoom Dot ESC  Xs       Linie      Ow  ';
         TextStart:=Trunc((640-TextWidth(T))/2);
         OutTextXY(TextStart,10,T);
         SetViewPort(20,70,619,459,ClipOn);
End;

Procedure Setupvar;
Var      I:Integer;
Begin    Channum:=1;
         Linnum:=1;
         Xstart:=0;
         Xmark:=0;
         SetColor(InitCol);
         Zoom:=Trunc(BS);
         Trin:=1;
         For I:=1 to (300 Div Zoom) do Trin:=Trin*2;
         Max:=Trunc(599/Trin)*Trin;
         Dot:=2;
         Overwrite:=1;
         New(Buf);
         BufSize:=Trunc(BS);
End;

Procedure Setupfiles;
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

Function BufVal(X:Integer):Integer;
var t:Double;
Begin    Case Channum of
         1: BufVal:=Trunc(Zero-Res*DR1*(Buf^[Trunc(Zoom/600*X+Xstart)]-DacO1));
         2: BufVal:=Trunc(Zero-Res*DR2*(Buf^[Trunc(Zoom/600*X+Xstart)]-DacO2));
         3: BufVal:=Trunc(Zero-Res*DR3*((Buf^[Trunc(Zoom/600*X+Xstart)] and $3FF)-DacO3));
         End;
End;

Procedure Textow;
Var      T,Tow:String;
         Col:Integer;
Begin    Str(Overwrite:1,Tow);
         T:=Concat('(    :        )  Clear Zoom Dot ESC  Xs       Linie      Ow ',Tow);
         SetViewPort(TextStart+480,20,TextStart+488,40,ClipOn);
         ClearViewPort;
         SetViewPort(0,20,639,40,ClipOn);
         Col:=GetColor;
         SetColor(15);
         OutTextXY(TextStart,10,T);
         SetColor(Col);
         SetViewPort(20,70,619,459,ClipOn);
End;

Procedure Textlin;
Var      T,Tl:String;
         Col:Integer;
Begin    Str(Linnum:4,Tl);
         T:=Concat('(    :        )  Clear Zoom Dot ESC  Xs       Linie ',Tl);
         SetViewPort(TextStart+416,20,TextStart+448,40,ClipOn);
         ClearViewPort;
         SetViewPort(0,20,639,40,ClipOn);
         Col:=GetColor;
         SetColor(15);
         OutTextXY(TextStart,10,T);
         SetColor(Col);
         SetViewPort(20,70,619,459,ClipOn);
End;

Procedure Textxval;
Var      T,Txs:String;
         Col:Integer;
Begin    Str(Xstart:4,Txs);
         T:=Concat('(    :        )  Clear Zoom Dot ESC  Xs ',Txs);
         SetViewPort(TextStart+320,20,TextStart+352,40,ClipOn);
         ClearViewPort;
         SetViewPort(0,20,639,40,ClipOn);
         Col:=GetColor;
         SetColor(15);
         OutTextXY(TextStart,10,T);
         SetColor(Col);
         SetViewPort(20,70,619,459,ClipOn);
End;

Procedure Textmark;
Var      T,Tx,Td:String;
         Col:Integer;
         Y:Double;
Begin    X:=Trunc(Zoom/600*Xmark+Xstart);
         Case Channum of
         1: Y:=DR1*(Buf^[X]-DacO1);
         2: Y:=DR2*(Buf^[X]-DacO2);
         3: Y:=DR3*(Buf^[X]-DacO3);
         End;
         Str(X+1:4,Tx);
         Str(Y:8:3,Td);
         T:=Concat('(',Tx,':',Td,')');
         SetViewPort(TextStart+8,20,TextStart+40,40,ClipOn);
         ClearViewPort;
         SetViewPort(TextStart+48,20,TextStart+112,40,ClipOn);
         ClearViewPort;
         SetViewPort(0,20,639,40,ClipOn);
         Col:=GetColor;
         SetColor(15);
         OutTextXY(TextStart,10,T);
         SetColor(Col);
         SetViewPort(20,70,619,459,ClipOn);
End;

Procedure Chanload;
Begin    Case Channum of
                   1: Begin  Seek(F1,(Linnum-1)*BufSize);
                             BlockRead(F1,Buf^,BufSize,C);
                             If 0<DR1
                             then Zero:=390-Trunc(390*DacO1/1024)
                             else Zero:=390-Trunc(390*(1024-DacO1)/1024);
                             Res:=390/Abs(DR1)/1024;
                      End;
                   2: Begin  Seek(F2,(Linnum-1)*BufSize);
                             BlockRead(F2,Buf^,BufSize,C);
                             If 0<DR2
                             then Zero:=390-Trunc(390*DacO2/1024)
                             else Zero:=390-Trunc(390*(1024-DacO2)/1024);
                             Res:=390/Abs(DR2)/1024;
                      End;
                   3: Begin  Seek(F3,(Linnum-1)*BufSize);
                             BlockRead(F3,Buf^,BufSize,C);
                             If 0<DR3
                             then Zero:=390-Trunc(390*DacO3/1024)
                             else Zero:=390-Trunc(390*(1024-DacO3)/1024);
                             Res:=390/Abs(DR3)/1024;
                      End;
         End;
End;

Procedure Grid;
Var      I:Integer;
         Col:Integer;
Begin    Col:=GetColor;
         SetColor(15);
         Line(0,Zero,599,Zero);
         For I:=0 to 59 do PutPixel(I*10,Zero-1,15);
         For I:=0 to 59 do PutPixel(I*10,Zero+1,15);
         SetColor(Col);
End;

Procedure Markwrite;
Var      D,Col:Integer;
Begin    D:=BufVal(Xmark);
         IF GetColor=GetBkColor then
         Begin     PutPixel(Xmark,D-2,C90B);
                   PutPixel(Xmark,D-1,C90A);
                   PutPixel(Xmark,D+1,C270A);
                   PutPixel(Xmark,D+2,C270B);
                   If 1<Xmark then PutPixel(Xmark-2,D,C180B);
                   If 0<Xmark then PutPixel(Xmark-1,D,C180A);
                   If Xmark<599 then PutPixel(Xmark+1,D,C0A);
                   If Xmark<598 then PutPixel(Xmark+2,D,C0B);
         End else
         Begin     Col:=GetColor;
                   SetColor(15);
                   C90B:=GetPixel(Xmark,D-2);
                   C90A:=GetPixel(Xmark,D-1);
                   C270A:=GetPixel(Xmark,D+1);
                   C270B:=GetPixel(Xmark,D+2);
                   If 1<Xmark then C180B:=GetPixel(Xmark-2,D);
                   If 0<Xmark then C180A:=GetPixel(Xmark-1,D);
                   If Xmark<599 then C0A:=GetPixel(Xmark+1,D);
                   If Xmark<598 then C0B:=GetPixel(Xmark+2,D);
                   PutPixel(Xmark,D-2,GetColor);
                   PutPixel(Xmark,D-1,GetColor);
                   PutPixel(Xmark,D+1,GetColor);
                   PutPixel(Xmark,D+2,GetColor);
                   If 1<Xmark then PutPixel(Xmark-2,D,GetColor);
                   If 0<Xmark then PutPixel(Xmark-1,D,GetColor);
                   If Xmark<599 then PutPixel(Xmark+1,D,GetColor);
                   If Xmark<598 then PutPixel(Xmark+2,D,GetColor);
                   SetColor(Col);
         End;
End;

Procedure Sigwrite;
Var      I,X,D,Dotold:Integer;
Begin    If GetColor=GetBkColor then Markwrite;
         D:=0;
         For I:=0 to Trunc(599/Trin) do
         Begin     X:=I*Trin;
                   Dold:=D;
                   D:=BufVal(X);
                   If (Trunc(Zero)<>D) or (Trunc(Zero)<>Dold) or
                   (GetColor<>GetBkColor) then
                   Begin     Dotold:=Dot;
                             If Xstart+X=0 then Dot:=1;
                             Case Dot of
                                       1: PutPixel(X,D,GetColor);
                                       2: Line(X-1,Dold,X,D);
                                       3: Line(X,Zero,X,D);
                             End;
                             Dot:=Dotold;
                   End;
         End;
         If GetColor=GetBkColor then Grid else Markwrite;
End;

Function GetKey:Char;
Var      Key:Char;
Begin    Repeat    Repeat
                   Until KeyPressed;
                   Key:=ReadKey;
                   If Key=#0 then Key:=ReadKey;
         Until (('0'<=Key) and (Key<='9')) or (Key=#13) or (Key=#75);
         GetKey:=Key;
End;

Procedure KeyL;
Var      KeyString:String[4];
         KeyCount,Col,ErrorCode,KeyX:Integer;
         Key:Char;
Begin    KeyCount:=1;
         KeyString:='';
         SetViewPort(TextStart+416,20,TextStart+448,40,ClipOn);
         ClearViewPort;
         SetViewPort(0,20,639,40,ClipOn);
         Col:=GetColor;
         SetColor(15);
         Repeat    Key:=GetKey;
                   KeyX:=TextStart+408+8*KeyCount;
                   If (Key=#75) and (1<KeyCount) then
                   Begin     KeyString:=Copy(KeyString,1,KeyCount-2);
                             Dec(KeyCount);
                             SetViewPort(KeyX-8,20,KeyX,40,ClipOn);
                             ClearViewPort;
                             SetViewPort(0,20,639,40,ClipOn);
                   End Else If (Key<>#13) and (Key<>#75) then
                   Begin     KeyString:=Concat(KeyString,Key);
                             Inc(KeyCount);
                             OutTextXY(KeyX,10,Key);
                   End;
         Until (Key=#13) or (4<KeyCount);
         Val(KeyString,KeyCount,ErrorCode);
         SetColor(Col);
         SetViewPort(20,70,619,459,ClipOn);
         If ErrorCode=0 then
         Begin     Col:=GetColor;
                   SetColor(GetBkColor);
                   If Overwrite=1 then
                   Begin     Sigwrite;
                             SetColor(Col);
                   End Else
                   Begin     Markwrite;
                             SetColor((Col mod 15)+1);
                   End;
                   Linnum:=KeyCount;
                   Chanload;
                   Sigwrite;
         End;
         Textlin;
         Textmark;
End;

Procedure KeyUp;
Var      Col:Integer;
Begin    Col:=GetColor;
         SetColor(GetBkColor);
         If Overwrite=1 then
         Begin     Sigwrite;
                   SetColor(Col);
         End Else
         Begin     Markwrite;
                   SetColor((Col mod 15)+1);
         End;
         Inc(Linnum);
         Chanload;
         Sigwrite;
         Textlin;
         Textmark;
End;

Procedure KeyDown;
Var      Col:Integer;
Begin    If 1<Linnum then
         Begin     Col:=GetColor;
                   SetColor(GetBkColor);
                   If Overwrite=1 then
                   Begin     Sigwrite;
                             SetColor(Col);
                   End Else
                   Begin     Markwrite;
                             SetColor((Col mod 15)+1);
                   End;
                   Dec(Linnum);
                   Chanload;
                   Sigwrite;
                   Textlin;
                   Textmark;
         End;
End;

Procedure KeyO;
Begin    Overwrite:=(Overwrite+1) mod 2;
         Textow;
End;

Procedure KeyX;
Var      KeyString:String[4];
         KeyCount,Col,ErrorCode,KeyX:Integer;
         Key:Char;
Begin    KeyCount:=1;
         KeyString:='';
         SetViewPort(TextStart+320,20,TextStart+352,40,ClipOn);
         ClearViewPort;
         SetViewPort(0,20,639,40,ClipOn);
         Col:=GetColor;
         SetColor(15);
         Repeat    Key:=GetKey;
                   KeyX:=TextStart+312+8*KeyCount;
                   If (Key=#75) and (1<KeyCount) then
                   Begin     KeyString:=Copy(KeyString,1,KeyCount-2);
                             Dec(KeyCount);
                             SetViewPort(KeyX-8,20,KeyX,40,ClipOn);
                             ClearViewPort;
                             SetViewPort(0,20,639,40,ClipOn);
                   End Else If (Key<>#13) and (Key<>#75) then
                   Begin     KeyString:=Concat(KeyString,Key);
                             Inc(KeyCount);
                             OutTextXY(KeyX,10,Key);
                   End;
         Until (Key=#13) or (4<KeyCount);
         Val(KeyString,KeyCount,ErrorCode);
         SetColor(Col);
         SetViewPort(20,70,619,459,ClipOn);
         If ErrorCode=0 then
         Begin     If Trunc(BS)-Zoom<KeyCount then KeyCount:=Trunc(BS)-Zoom;
                   If KeyCount<0 then KeyCount:=0;
                   If KeyCount<>Xstart then
                   Begin     Col:=GetColor;
                             SetColor(GetBkColor);
                             Sigwrite;
                             SetColor(Col);
                             Xstart:=KeyCount;
                             Sigwrite;
                   End;
         End;
         Textxval;
         Textmark;
End;

Procedure KeyZ;
Var      Col,I:Integer;
Begin    Col:=GetColor;
         SetColor(GetBkColor);
         Sigwrite;
         X:=Trunc(Zoom/600*Xmark+Xstart);
         Case Zoom of
                   600:       Zoom:=300;
                   300:       Zoom:=150;
                   150:       Zoom:=75;
                   75:        Zoom:=Trunc(BS);
         Else
                   Zoom:=600;
         End;
         Xstart:=Trunc(X-Zoom/2);
         IF Trunc(BS)-Zoom<Xstart then Xstart:=Trunc(BS-Zoom);
         IF Xstart<0 then Xstart:=0;
         Trin:=1;
         For I:=1 to (300 Div Zoom) do Trin:=Trin*2;
         Max:=Trunc(599/Trin)*Trin;
         Xmark:=Trunc((X-Xstart)/Zoom*600/Trin)*Trin;
         SetColor(Col);
         Sigwrite;
         Textmark;
         Textxval;
End;

Procedure KeyAltZ;
Var      Col,I:Integer;
Begin    Col:=GetColor;
         SetColor(GetBkColor);
         Sigwrite;
         X:=Trunc(Zoom/600*Xmark+Xstart);
         Case Zoom of
                   600:       Zoom:=Trunc(BS);
                   300:       Zoom:=600;
                   150:       Zoom:=300;
                   75:        Zoom:=150;
         Else
                   Zoom:=75;
         End;
         Xstart:=Trunc(X-Zoom/2);
         IF Trunc(BS)-Zoom<Xstart then Xstart:=Trunc(BS-Zoom);
         IF Xstart<0 then Xstart:=0;
         Trin:=1;
         For I:=1 to (300 Div Zoom) do Trin:=Trin*2;
         Max:=Trunc(599/Trin)*Trin;
         Xmark:=Trunc((X-Xstart)/Zoom*600/Trin)*Trin;
         SetColor(Col);
         Sigwrite;
         Textmark;
         Textxval;
End;

Procedure KeyD;
Var      Col:Integer;
Begin    Col:=GetColor;
         SetColor(GetBkColor);
         Sigwrite;
         Dot:=(Dot mod 3)+1;
         SetColor(Col);
         Sigwrite;
End;

Procedure KeyC;
Begin    ClearViewPort;
         Grid;
         Sigwrite;
End;

Procedure Key1;
Var      Col:Integer;
Begin    If Channum<>1 then
         Begin     If Overwrite=1 then
                   Begin     Col:=GetColor;
                             SetColor(GetBkColor);
                             Sigwrite;
                             SetColor(Col);
                   End else  SetColor((GetColor mod 15)+1);
                   Channum:=1;
                   Chanload;
                   Sigwrite;
         End;
End;

Procedure Key2;
Var      Col:Integer;
Begin    If Channum<>2 then
         Begin     If Overwrite=1 then
                   Begin     Col:=GetColor;
                             SetColor(GetBkColor);
                             Sigwrite;
                             SetColor(Col);
                   End else  SetColor((GetColor mod 15)+1);
                   Channum:=2;
                   Chanload;
                   Sigwrite;
         End;
End;

Procedure Key3;
Var      Col:Integer;
Begin    If Channum<>3 then
         Begin     If Overwrite=1 then
                   Begin     Col:=GetColor;
                             SetColor(GetBkColor);
                             Sigwrite;
                             SetColor(Col);
                   End else  SetColor((GetColor mod 15)+1);
                   Channum:=3;
                   Chanload;
                   Sigwrite;
         End;
End;

Procedure KeyRight;
Var      Col:Integer;
Begin    If (Xmark=Max) and (Xstart<Trunc(BS)-Zoom) then
         Begin     Col:=GetColor;
                   SetColor(GetBkColor);
                   Sigwrite;
                   Xstart:=Xstart+Trunc(Zoom/4);
                   If Trunc(BS)-Zoom<Xstart then Xstart:=Trunc(BS)-Zoom;
                   SetColor(Col);
                   Sigwrite;
                   Textmark;
                   Textxval;
         End Else If Xmark<Max then
         Begin     Col:=GetColor;
                   SetColor(GetBkColor);
                   Markwrite;
                   Xmark:=Xmark+Trin;
                   If Max<Xmark then Xmark:=Max;
                   SetColor(Col);
                   Markwrite;
                   Textmark;
         End;
End;

Procedure KeyLeft;
Var      Col:Integer;
Begin    If (Xmark=0) and (0<Xstart) then
         Begin     Col:=GetColor;
                   SetColor(GetBkColor);
                   Sigwrite;
                   Xstart:=Xstart-Trunc(Zoom/4);
                   If Xstart<0 then Xstart:=0;
                   SetColor(Col);
                   Sigwrite;
                   Textmark;
                   Textxval;
         End Else If 0<Xmark then
         Begin     Col:=GetColor;
                   SetColor(GetBkColor);
                   Markwrite;
                   Xmark:=Xmark-Trin;
                   If Xmark<0 then Xmark:=0;
                   SetColor(Col);
                   Markwrite;
                   Textmark;
         End;
End;

Procedure KeyPgUp;
Var      Col:Integer;
Begin    If (Xmark=Max) and (Xstart<Trunc(BS)-Zoom) then
         Begin     Col:=GetColor;
                   SetColor(GetBkColor);
                   Sigwrite;
                   Xstart:=Xstart+Trunc(Zoom/2);
                   If Trunc(BS)-Zoom<Xstart then Xstart:=Trunc(BS)-Zoom;
                   SetColor(Col);
                   Sigwrite;
                   Textmark;
                   Textxval;
         End Else If Xmark<Max then
         Begin     Col:=GetColor;
                   SetColor(GetBkColor);
                   Markwrite;
                   Xmark:=Xmark+Trunc(75/Trin)*Trin;
                   If Max<Xmark then Xmark:=Max;
                   SetColor(Col);
                   Markwrite;
                   Textmark;
         End;
End;

Procedure KeyPgDown;
Var      Col:Integer;
Begin    If (Xmark=0) and (0<Xstart) then
         Begin     Col:=GetColor;
                   SetColor(GetBkColor);
                   Sigwrite;
                   Xstart:=Xstart-Trunc(Zoom/2);
                   If Xstart<0 then Xstart:=0;
                   SetColor(Col);
                   Sigwrite;
                   Textmark;
                   Textxval;
         End Else If 0<Xmark then
         Begin     Col:=GetColor;
                   SetColor(GetBkColor);
                   Markwrite;
                   Xmark:=Xmark-Trunc(75/Trin)*Trin;
                   If Xmark<0 then Xmark:=0;
                   SetColor(Col);
                   Markwrite;
                   Textmark;
         End;
End;

Procedure KeyHome;
Var      Col:Integer;
Begin    If (Xstart<>0) or (Xmark<>0) then
         Begin     Col:=GetColor;
                   SetColor(GetBkColor);
                   If Xmark=0 then
                   Begin     Sigwrite;
                             Xstart:=0;
                             SetColor(Col);
                             Sigwrite;
                             Textxval;
                   End Else Begin
                             Markwrite;
                             Xmark:=0;
                             SetColor(Col);
                             Markwrite;
                   End;
                   Textmark;
         End;
End;

Procedure KeyEnd;
Var      Col:Integer;
Begin    If (Trunc(BS)-Zoom<>Xstart) or (Xmark<>Trunc(599/Trin)*Trin) then
         Begin     Col:=GetColor;
                   SetColor(GetBkColor);
                   If Xmark=Trunc(599/Trin)*Trin then
                   Begin     Sigwrite;
                             Xstart:=Trunc(BS)-Zoom;
                             SetColor(Col);
                             Sigwrite;
                             Textxval;
                   End Else Begin
                             Markwrite;
                             Xmark:=Trunc(599/Trin)*Trin;
                             SetColor(Col);
                             Markwrite;
                   End;
                   Textmark;
         End;
End;

Procedure KeySelect;
Begin    Case KeyString[1] of
                   'L','l': KeyL;
                   'O','o': KeyO;
                   'X','x': KeyX;
                   'Z','z': KeyZ;
                   'D','d': KeyD;
                   'C','c': KeyC;
                   '1'    : Key1;
                   '2'    : Key2;
                   '3'    : Key3;
         Else
                   Case KeyString[2] of
                             #72: KeyUp;
                             #80: KeyDown;
                             #44: KeyAltZ;
                             #77: KeyRight;
                             #75: KeyLeft;
                             #73: KeyPgUp;
                             #81: KeyPgDown;
                             #71: KeyHome;
                             #79: KeyEnd;
                   End;
         End;
End;

Begin    Getchannel0;
         Setupgraph;
         Setupvar;
         Setupfiles;

         Chanload;
         Grid;
         Sigwrite;
         Textmark;
         Textxval;
         Textlin;
         Textow;

         Repeat    Repeat
                   Until KeyPressed;
                   KeyString:='';
                   Repeat    Key:=ReadKey;
                             KeyString:=Concat(KeyString,Key);
                   Until Key<>#0;

                   KeySelect;
         Until KeyString[1]=#27;
         CloseGraph;
End.