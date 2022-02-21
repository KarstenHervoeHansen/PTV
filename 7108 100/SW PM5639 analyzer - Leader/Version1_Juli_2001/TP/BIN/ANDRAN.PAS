Program Test;

Uses Dos,Crt;

Var X,Y       : Real;
    O,U,I,P,T : LongInt;
    H,M,S,S60 : Word;

BEGIN
 O:=0;U:=0;I:=0;T:=0;
 ClrScr;
 GetTime(H,M,S,S60);
 X:=S60/100;
 Repeat
  X:=X*X-2;
  Y:=(X+2)/4;
  GotoXY(1,1);
  If (Y>0.75) And (Y<=1) Then Inc(o);
  If (Y>0.5 ) And (Y<=0.75) Then Inc(u);
  If (Y>0.25) And (Y<=0.5) Then Inc(p);
  If (Y>0 ) And (Y<=0.25) Then Inc(t);

  Inc(I);
  WriteLn(o:8,' ',(o/i):5:5);
  WriteLn(u:8,' ',(u/i):5:5);
  WriteLn(p:8,' ',(p/i):5:5);
  WriteLn(t:8,' ',(t/i):5:5);
 Until Keypressed;
END.