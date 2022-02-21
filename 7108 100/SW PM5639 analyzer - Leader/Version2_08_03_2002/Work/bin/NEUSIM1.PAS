PROGRAM Neutronsim;
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

PROCEDURE Indlaes;
TYPE
  TStreng = String[30];
VAR
  i          : TAtom;
  Streng     : TStreng;
  Test       : Integer;
  SamletBrok : Real;
BEGIN
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
  IF SamletBrok<>1 THEN Writeln('Den samlede v‘gtbr›k skal v‘re 1. Pr›v igen!');
UNTIL SamletBrok = 1;
  REPEAT
    Write('Indtast stoffets massefylde -> ');
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
END;
Begin
Indlaes;
END.