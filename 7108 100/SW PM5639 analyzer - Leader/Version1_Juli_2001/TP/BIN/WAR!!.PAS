Program Krig;

Uses
  crt;

Type
  GrenType=(Inf,pan,luf);
  TAlleEnhed= ARRAY[0..40,1..3] OF Integer;

VAR
  AlleEnhed:TAlleEnhed;
  x,z:char;
  y,i,e:byte;
  a,f:byte;
  ai,ap,al,fi,fp,fl,tai,tap,tal,tfi,tfp,tfl:Longint;
  l:real;


FUNCTION FemTilfTal(l:real):real;
BEGIN
  FemTilfTal:=(Random+Random+Random+Random+Random)*l/100;
END;


PROCEDURE Indskriv ;

BEGIN
  Clrscr;
  write('Hvilken enhed vil du gerne starte? (1-20(sort) 21-40(roed)');
  Readln(e);
  Write('Hvor store er infanteristyrkerne?');
  Readln(AlleEnhed[e,1]);
  Write('Hvor store er panserstyrkerne?');
  Readln(AlleEnhed[e,2]);
  Write('Hvor store er luftstyrkerne?');
  Readln(AlleEnhed[e,3]);
END;


PROCEDURE KampStats;

BEGIN
  clrscr;
  ai:=0;ap:=0;al:=0;fi:=0;fp:=0;fl:=0;
  write('Er det en by der angribes? (j/n)');
  readln(x);
  write('Hvor mange enheder angriber?');
  Readln(a);
  FOR i:=1 TO a DO
    BEGIN
      write('Den ',i,'. enheds nr.?');
      Readln(e);
      ai:=ai+Alleenhed[e,1];
      ap:=ap+Alleenhed[e,2];
      al:=al+Alleenhed[e,3];
    END;
  IF x='j' THEN
  BEGIN
    fi:=3000;  {Alleenhed[0,1]}
    fp:=80;    {Alleenhed[0,2]}
    fl:=20;    {Alleenhed[0,3]}
  END
  ELSE
  BEGIN
    write('Hvor mange enheder forsvarer?');
    Readln(f);
    FOR i:=1 TO f DO
    BEGIN
      write('Den ',i,'. enheds nr.?');
      Readln(e);
      fi:=fi+Alleenhed[e,1];
      fp:=fp+Alleenhed[e,2];
      fl:=fl+Alleenhed[e,3];
    END;
  END;
  write('Vil angriberen foretage flyangreb? (j/n):');
  readln(z);
  IF z='j' THEN
  BEGIN
    tal:=TRUNC(FemTilfTal(4)*fl+FemTilfTal(2)*fp+FemTilfTal(0.1)*fi);
    tfl:=TRUNC(FemTilfTal(4)*al);
    tfp:=TRUNC(FemTilfTal(10)*al);
    tfi:=TRUNC(FemTilfTal(60)*al);
    writeln('Tabene efter flyangreb fordeler sig s†ledes:');
    writeln('Angribers fly:',tal);
    writeln('Forsvarer: Infanteri:',tfi,'   Panser:',tfp,'   Fly:',tfl);
    al=al-tal; fi:=fi-tfi; fp:=fp-tfp; fl:=fl-tfl;
    IF al<0 THEN al=0;
    IF fi<0 THEN fi=0;
    IF fp<0 THEN fp=0;
    IF fl<0 THEN fl=0;
  END;
  tai:=
END;


BEGIN
  clrscr;
  Randomize;
  REPEAT
  Write('Vil du gerne starte en ny enhed? (j/n)');
  Readln(x);
  IF x='j' THEN Indskriv;
  UNTIL x<>'j';
  write('Skal et kampresultat beregnes? (j/n)');
  Readln(x);
  IF x='j' THEN Kampstats;

END.



