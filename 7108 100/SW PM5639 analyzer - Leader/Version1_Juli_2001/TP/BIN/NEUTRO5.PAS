PROGRAM Neutronsim;

USES
  CRT;
TYPE
  TAtom = (H,C,O,Si,Ca);
  TGrundstof = RECORD
                Navn            : String[8];
                Atomvegt, Sigma : Real;
              END;
  TGrundstofTabel = ARRAY[TAtom] OF TGrundstof;

  TAtomData = RECORD
                Navn      : String[8];
                Atomvegt,
                Alfa,
                Sigma,
                StSigma,
                VgtBr,
                PAkk      : Real;
              END;
  TAtomTabel = ARRAY[TAtom] OF TAtomData;
  TMaalingsnavn=(Stoed,Vej,Tid);
  TMaalingsverdi = ARRAY[TMaalingsnavn] OF Real;
  THyppighed=ARRAY[0..99] OF Integer;
  TMaale=RECORD
               Maalingsnavn: STRING[8];
               Enhedsnavn  : STRING[8];
               Middel      : Real;
               Spredning   : Real;
               Min         : Real;
               Max         : Real;
               Delta       : Real;
               MaxBrugt    : Byte;
               Hyppighed   : THyppighed;
  END;
  TResultattabel = ARRAY[TMaalingsnavn] OF TMaale;

CONST
  GrundstofTabel : TGrundstofTabel=                    (*Tabel over grund-*)
    ((Navn:'Hydrogen'; Atomvegt:1.01 ; Sigma:20.0),    (*stoffernes natur-*)
     (Navn:'Kulstof' ; Atomvegt:12.01; Sigma:4.8 ),    (*givne data.*)
     (Navn:'Oxygen'  ; Atomvegt:16.00; Sigma:4.2 ),
     (Navn:'Silicium'; Atomvegt:28.10; Sigma:1.7 ),
     (Navn:'Calcium' ; Atomvegt:14.10; Sigma:3.0));

VAR
  AtomTabel : TAtomTabel;
  Massefylde, EStart, ESlut : Real;
  NAntal : Integer;
  l,StAtom:Tatom;
  ResultatTabel:TResultatTabel;
  k,i:TMaalingsnavn;
  j:byte;
  StoedAntal:Integer;
  VejAkk,
  TidAkk: Real;
  ETemp: Real;
  Flyvetid,
  Vejlengde: Real;
  Maalingsverdi: TMaalingsverdi;


PROCEDURE Indlaes;
TYPE
  TStreng = String[30];
VAR
  i          : TAtom;
  Streng     : TStreng;
  Test       : Integer;
  SamletBrok : Real;

BEGIN
  Clrscr;
REPEAT
  SamletBrok:=0;
  FOR i := Low(TAtom) TO High(TAtom) DO
  BEGIN
      REPEAT
        Write('Indtast massebr›k af ',GrundstofTabel[i].Navn,'-> ');
        Readln(Streng);
        Val(Streng, AtomTabel[i].VgtBr, Test);
      UNTIL (Test = 0) AND (AtomTabel[i].VgtBr >= 0) AND (AtomTabel[i].VgtBr <= 1);
    SamletBrok:= SamletBrok + AtomTabel[i].VgtBr;
  END;
  Clrscr;
  IF (SamletBrok < 0.99) OR (SamletBrok>1.01) THEN Writeln('Den samlede v‘gtbr›k skal v‘re 1. Pr›v igen!');
UNTIL (SamletBrok > 0.99) AND (SamletBrok < 1.01);
  REPEAT
    Write('Indtast mediets massefylde -> ');
    Readln(Streng);
    Val(Streng, Massefylde, Test);
  UNTIL Test = 0;
  REPEAT
    Write('Indtast neutronernes gennemsnitlige startenergi i MeV -> ');
    Readln(Streng);
    Val(Streng, EStart, Test);
  UNTIL Test = 0;
  REPEAT
    Write('Indtast den ›nskede slutenergi (Termisk energi) i MeV -> ');
    Readln(ESlut);
    Val(Streng, ESlut, Test)
  UNTIL Test = 0;
  REPEAT
    Write('Hvor mange neutronnedbremsninger ›nskes simuleret? (Max. 10000) -> ');
    Readln(NAntal);
    Val(Streng, NAntal, Test)
  UNTIL Test = 0;
END;  {Indlaes}


PROCEDURE BeregnData;

VAR
  l:TAtom;
  A,StSigmaAkk,TempPAkk:real;

BEGIN
  StSigmaAkk:=0;
  For l:=low(TAtom) TO high(TAtom) DO
  BEGIN
    A                    :=GrundstofTabel[l].Atomvegt;
    ATomTabel[l].Atomvegt:=GrundstofTabel[l].Atomvegt;
    Atomtabel[l].alfa    :=((a-1)/(a+1))*((a-1)/(a+1));
    ATomtabel[l].sigma   :=GrundstofTabel[l].sigma;
    Atomtabel[l].StSigma :=GrundstofTabel[l].sigma*(0.6023*exp(24*ln(10))*
    Massefylde*AtomTabel[l].VgtBr/AtomTabel[l].Atomvegt);
    StSigmaAkk           :=AtomTabel[l].StSigma+StSigmaAkk;
  END;
  TempPAkk:=0;
  For i:=low(TAtom) TO high(TAtom) DO
  BEGIN
    AtomTabel[i].PAkk:=AtomTabel[i].StSigma/StSigmaAkk+TempPAkk;
    TempPAkk:=TempPAkk+AtomTabel[i].PAkk;
  END;
END; {BeregnData}


PROCEDURE StoedAtomtype;

VAR
  Tt:real;
  i : TAtom;

BEGIN
  Tt:=Random;
  IF ((0<Tt) AND (Tt<Atomtabel[Low(TAtom)].PAkk))  THEN StAtom:=H;
  FOR i:= Succ(Low(TAtom)) TO High(TAtom) DO
  IF ((Atomtabel[Pred(i)].PAkk<=Tt)  AND (Tt<Atomtabel[i].PAkk))  THEN StAtom:=i;
END;

PROCEDURE Initialisering;

VAR
  i: TMaalingsnavn;
  j: Byte;

BEGIN
    VejAkk    :=0     ;
    TidAkk    :=0     ;
    ETemp     :=EStart;
    Stoedantal:=0     ;
  FOR i:=low(TMaalingsnavn) TO high(TMaalingsnavn) DO
  BEGIN
    ResultatTabel[i].Min     := 1000;
    ResultatTabel[i].Max     := 0;
    ResultatTabel[i].Delta   := 0.25;
    ResultatTabel[i].MaxBrugt:= 0;
    FOR j:=0 TO 99 DO
    ResultatTabel[i].Hyppighed[j]:=0;
  END;
END;




PROCEDURE TabelFit(k:TMaalingsnavn);

BEGIN
  FOR j := 0 TO 49 DO
  BEGIN
    ResultatTabel[k].Hyppighed[j] :=
    ResultatTabel[k].Hyppighed[j*2] + ResultatTabel[k].Hyppighed[j*2+1];
  END;
  FOR j := 50 TO 99 DO
    ResultatTabel[k].Hyppighed[j] := 0;
    ResultatTabel[k].delta := ResultatTabel[k].delta*2;
  ResultatTabel[k].Max:=ResultatTabel[k].Hyppighed[j];
END;


PROCEDURE PropiTabel(Verdi:Real;k:TMaalingsnavn);

BEGIN
  REPEAT
    IF ((Verdi/Resultattabel[k].delta)>=100) THEN TabelFit(k);
  UNTIL (Verdi/Resultattabel[k].delta)<100;
    Resultattabel[k].Hyppighed[TRUNC(Verdi/Resultattabel[k].delta)]:=
    Resultattabel[k].Hyppighed[TRUNC(Verdi/Resultattabel[k].delta)]+1;
    IF Verdi<Resultattabel[k].Min THEN Resultattabel[k].:=Verdi;
END;


PROCEDURE EtStoed;

BEGIN
    StoedAtomType;
    Vejlengde:=(ln(1/(1-Random)))/Atomtabel[StAtom].StSigma;
    ETemp:=ETemp*(Atomtabel[StAtom].alfa+Random*(1-Atomtabel[StAtom].alfa));
    Flyvetid:=Vejlengde/(1383100*Sqrt(ETemp));
    MaalingsVerdi[Vej]:=MaalingsVerdi[Vej]+Vejlengde;
    MaalingsVerdi[Tid]:=MaalingsVerdi[Tid]+Flyvetid;
    MaalingsVerdi[Stoed]:=MaalingsVerdi[Stoed]+1;
END;



PROCEDURE EnNedbremsning;

BEGIN
  MaalingsVerdi[Vej]:=0;
  MaalingsVerdi[Tid]:=0;
  MaalingsVerdi[Stoed]:=0;
  REPEAT
  EtStoed;
  UNTIL ETemp<=ESlut;
END;


PROCEDURE DataBehandling;

VAR

BEGIN
  FOR k:= Low(TMaalingsnavn) TO High(TMaalingsnavn) DO
    FOR j:=0 TO 99 DO
    SamletStoed:=Resultattabel[k].Hyppighed[j]*(j+Resultattabel[k].delta/2)
END;


BEGIN
  Randomize;
  Indlaes;
  BeregnData;
  Initialisering;
  FOR j:=1 TO NAntal DO EnNedbremsning;
  FOR k:= Low(TMaalingsnavn) TO High(TMaalingsnavn) DO
    PropiTabel(Maalingsverdi[k],k);
  DataBehandling;
END.
