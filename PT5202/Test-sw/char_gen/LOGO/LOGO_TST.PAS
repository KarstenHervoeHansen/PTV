


PROGRAM MAKE_KBS;

USES     CRT;

VAR
      Filter_logo                       : ARRAY[1..32] OF REAL;
      Nfolde2                           :INTEGER;

{ *********************************************************** }

PROCEDURE Make_Logo_Filter;

VAR   I,C                   : INTEGER;
      Y,Yr,Ys,Ampl,Glampl   : REAL;

BEGIN
     Yr:=150E-9;
     Ys:=13.5E6;
     Glampl :=0;
     C:=TRUNC(1.03734*Yr*Ys);
     FOR I:=1 TO 20 DO
       Filter_logo[I]:=0;

     FOR I:=-C TO C DO
       BEGIN
         Y:=(I)/Yr/Ys/2.07468+0.5;
         Ampl:=(Y-SIN(2*PI*Y)/(2*PI));
         Filter_logo[1+C+I]:=Ampl-Glampl;
         Writeln('[', 1+C+I, '] ', Filter_logo[1+C+I]);
         Glampl:=Ampl;
       END;
     Filter_logo[1+C+I+1]:=1-Glampl;
     Writeln('[', 1+C+I+1, '] ', Filter_logo[1+C+I+1]);
     { WRITELN( 1+C+I+1,Filter_logo[1+C+I+1] ); }
     Nfolde2:=2*(I+1);    { Antal, der skal foldes med}
     Writeln('Nfolde2  ',Nfolde2,'  ',I)
END;



    BEGIN
         Make_Logo_Filter;
         writeln ('Slut ');
    END.
