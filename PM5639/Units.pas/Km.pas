UNIT KM;
{
 Beregning af MX-v‘rdier og K-matrix i PM8550 software

 Peter Frederiksen, P-DPE, 921117
 Rettelser:



 Philips TV Test Equipment A/S
 (c) Philips Danmark A/S, 1992
}

INTERFACE

TYPE KMType     = ARRAY[1..9] OF REAL;

{--------------------------------------------------------------------------}
PROCEDURE FindMx(KM                                 : KMType;
                 XpR,YpR,XpG,YpG,XpB,YpB,Xp65,Yp65  : REAL;
             VAR XR,YR,ZR,XG,YG,ZG,XB,YB,ZB         : REAL;
             VAR Err                                : BYTE);

{
 XpR,YpR        : Chromaticity koordinater for r›d   angivet i xy-koordinater
 XpG,YpG        : Chromaticity koordinater for gr›n  angivet i xy-koordinater
 XpB,YpB        : Chromaticity koordinater for bl†   angivet i xy-koordinater
 Xp65,Yp65      : Chromaticity koordinater for hvid  angivet i xy-koordinater

 Beregnede MX-v‘rdier:
 XR,YR,ZR       : MX-m†lev‘rdier fra sensor for kanalerne R,G og B p† r›d sk‘rm
 XG,YG,ZG       : MX-m†lev‘rdier fra sensor for kanalerne R,G og B p† gr›n sk‘rm
 XB,YB,ZB       : MX-m†lev‘rdier fra sensor for kanalerne R,G og B p† bl† sk‘rm
 xW,yW,zW       : MX-m†lev‘rdier fra sensor for kanalerne R,G og B p† hvid sk‘rm

 Err            : Returneret fejlkode   0: no error
                                        1: ikke relevant i denne application
                                        2: no solution
}



PROCEDURE FindK (CIEMode, LumMode                       : INTEGER;
                 Cd_ftL_Factor,
                 XpR,YpR, XpG,YpG, XpB,YpB, Xp65,Yp65,
                 XR,YR,ZR, XG,YG,ZG, XB,YB,ZB, X65,Y65,Z65,
                 LumValue                               : REAL;
             VAR KM                                     : KMType;
             VAR Err                                    : BYTE);

{
 CIEMode       Koordinater angivet i en af f›lgende enheder:
               1: Chormaticity koordinater angivet som xy    (CIE 1931)
               2: Chormaticity koordinater angivet som u'v'  (CIE 1976)
               3: Chormaticity koordinater angivet som uv    (CIE 1960)

 LumMode       Luminance angivet i en af f›lgende enheder:
               1: Luminance angivet som cd/mý.
               2: Luminance angivet som NIT.
               3: Luminance angivet som ftL.

 Cd_ftL_Factor  : Omregningsfaktor mellem ftL og cd/mý(NIT)

 XpR,YpR        : Chromaticity koordinater for r›d  i angivet i xy-koordinater
 XpG,YpG        : Chromaticity koordinater for gr›n i angivet i xy-koordinater
 XpB,YpB        : Chromaticity koordinater for bl†  i angivet i xy-koordinater
 Xp65,Yp65      : Chromaticity koordinater for hvid i angivet i xy-koordinater

 XR,YR,ZR       : MX-m†lev‘rdier fra sensor for kanalerne R,G og B p† r›d sk‘rm
 XG,YG,ZG       : MX-m†lev‘rdier fra sensor for kanalerne R,G og B p† gr›n sk‘rm
 XB,YB,ZB       : MX-m†lev‘rdier fra sensor for kanalerne R,G og B p† bl† sk‘rm
 X65,Y65,Z65    : MX-m†lev‘rdier fra sensor for kanalerne R,G og B p† hvid sk‘rm

 LumValue       : Luminance niveau ved kalibrering angivet i cd/mý

 KM             : Beregnet K-matrix

 Err            : Returneret fejlkode   0: no error
                                        1: ikke relevant i denne application
                                        2: no solution
}
{--------------------------------------------------------------------------}



IMPLEMENTATION
USES Matrix;
VAR Dimen     : INTEGER;
    Coef      : TNmatrix;         { The matrix }
    Constants,                    { Constant terms in the equations }
    Solution  : TNvector;         { Solution to the set of equations }
    Error     : BYTE;             { Error flags }

PROCEDURE FindMx(KM                                 : KMType;
                 XpR,YpR,XpG,YpG,XpB,YpB,Xp65,Yp65  : REAL;
             VAR XR,YR,ZR,XG,YG,ZG,XB,YB,ZB         : REAL;
             VAR Err                                : BYTE);

  (*
  XpR,YpR,ZpR,                 {red       xyz-v‘rdier for monitor}
  XpG,YpG,ZpG,                 {green     fra felterne x= og y=}
  XpB,YpB,ZpB,                 {blue}
  Xp65,Yp65,Zp65               {white ~ D6500}
  *)

BEGIN
     Dimen := 12;                                {define 12 x 12 matrix}
     FillChar(Coef, SizeOf(Coef), 0);            {init 12 x 12 matrix}
     FillChar(Constants, SizeOf(Constants), 0);  {init 12 constants}
     {R›d sk‘rm}
     Coef[1,1] := KM[1]; Coef[1,2] := KM[2]; Coef[1,3] := KM[3];
     Coef[2,1] := KM[4]; Coef[2,2] := KM[5]; Coef[2,3] := KM[6];
     Coef[3,1] := KM[7]; Coef[3,2] := KM[8]; Coef[3,3] := KM[9];
     {gr›n sk‘rm}
     Coef[4,4] := KM[1]; Coef[4,5] := KM[2]; Coef[4,6] := KM[3];
     Coef[5,4] := KM[4]; Coef[5,5] := KM[5]; Coef[5,6] := KM[6];
     Coef[6,4] := KM[7]; Coef[6,5] := KM[8]; Coef[6,6] := KM[9];
     {bl† sk‘rm}
     Coef[7,7] := KM[1]; Coef[7,8] := KM[2]; Coef[7,9] := KM[3];
     Coef[8,7] := KM[4]; Coef[8,8] := KM[5]; Coef[8,9] := KM[6];
     Coef[9,7] := KM[7]; Coef[9,8] := KM[8]; Coef[9,9] := KM[9];
     {hvid sk‘rm}
     Coef[10,10] := KM[1]; Coef[10,11] := KM[2]; Coef[10,12] := KM[3];
     Coef[11,10] := KM[4]; Coef[11,11] := KM[5]; Coef[11,12] := KM[6];
     Coef[12,10] := KM[7]; Coef[12,11] := KM[8]; Coef[12,12] := KM[9];


     Constants[1] :=  XpR * 50;
     Constants[2] :=  YpR * 50;
     Constants[3] :=  (1- XpR - YpR) * 50;
     Constants[4] :=  XpG * 50;
     Constants[5] :=  YpG * 50;
     Constants[6] :=  (1 - XpG - YpG) * 50;
     Constants[7] :=  XpB * 50;
     Constants[8] :=  YpB * 50;
     Constants[9] :=  (1 - XpB - YpB) * 50;
     Constants[10] := Xp65 * 200;
     Constants[11] := Yp65 * 200;
     Constants[12] := (1 - Xp65 - Yp65) * 200;

     Gaussian_Elimination(Dimen, Coef, Constants, Solution, Error);

     Err := Error;
     XR := Solution[1];  YR := Solution[2]; ZR := Solution[3];
     XG := Solution[4];  YG := Solution[5]; ZG := Solution[6];
     XB := Solution[7];  YB := Solution[8]; ZB := Solution[9];
     {MX v‘rdier for hvid sk‘rm findes i Solution[10,11,12] - bruges ikke her}
END;  {FindMX}



PROCEDURE FindK (CIEMode, LumMode                       : INTEGER;
                 Cd_ftL_Factor,
                 XpR,YpR, XpG,YpG, XpB,YpB, Xp65,Yp65,
                 XR,YR,ZR, XG,YG,ZG, XB,YB,ZB, X65,Y65,Z65,
                 LumValue                               : REAL;
             VAR KM                                     : KMType;
             VAR Err                                    : BYTE);


VAR u,v,x,y : REAL;
    n       : BYTE;
BEGIN
 IF LumMode = 3 THEN                    {hvis lum unit = ftL}
  LumValue := LumValue / Cd_ftL_Factor;

   IF (CIEMode = 2) THEN           { CIE 1976:  omregning fra u`v` til xy koordinater}
    BEGIN
     u := Xp65; v := Yp65;                  {white}
     Xp65 := -9*u /(16*v - 6*u - 12);
     Yp65 := -4*v /(16*v - 6*u - 12);
     u := XpR; v := YpR;                    {red}
     XpR := -9*u /(16*v - 6*u - 12);
     YpR := -4*v /(16*v - 6*u - 12);
     u := XpG; v := YpG;                    {green}
     XpG := -9*u /(16*v - 6*u - 12);
     YpG := -4*v /(16*v - 6*u - 12);
     u := XpB; v := YpB;                    {blue}
     XpB := -9*u /(16*v - 6*u - 12);
     YpB := -4*v /(16*v - 6*u - 12);
    END; {if}

   IF CIEMode = 3 THEN        { CIE 1960: omregning fra uv til xy koordinater}
    BEGIN
     u := Xp65; v := Yp65;                  {white}
     Xp65 := -9*u /(24*v - 6*u - 12);
     Yp65 := -6*v /(24*v - 6*u - 12);
     u := XpR; v := YpR;                    {red}
     XpR := -9*u /(24*v - 6*u - 12);
     YpR := -6*v /(24*v - 6*u - 12);
     u := XpG; v := YpG;                    {green}
     XpG := -9*u /(24*v - 6*u - 12);
     YpG := -6*v /(24*v - 6*u - 12);
     u := XpB; v := YpB;                    {blue}
     XpB := -9*u /(24*v - 6*u - 12);
     YpB := -6*v /(24*v - 6*u - 12);
    END; {if}

 Dimen := 12;                                {define 12 x 12 matrix}
 FillChar(Coef, SizeOf(Coef), 0);            {init 12 x 12 matrix}
 FillChar(Constants, SizeOf(Constants), 0);  {init 12 constants}
 {R›d sk‘rm}
 Coef[1,1] := XR; Coef[1,2] := YR; Coef[1,3] := ZR; Coef[1,10] := -XpR;
 Coef[2,4] := XR; Coef[2,5] := YR; Coef[2,6] := ZR; Coef[2,10] := -YpR;
 Coef[3,7] := XR; Coef[3,8] := YR; Coef[3,9] := ZR; Coef[3,10] := -1+XpR+YpR;
 {gr›n sk‘rm}
 Coef[4,1] := XG; Coef[4,2] := YG; Coef[4,3] := ZG; Coef[4,11] := -XpG;
 Coef[5,4] := XG; Coef[5,5] := YG; Coef[5,6] := ZG; Coef[5,11] := -YpG;
 Coef[6,7] := XG; Coef[6,8] := YG; Coef[6,9] := ZG; Coef[6,11] := -1+XpG+YpG;
 {bl† sk‘rm}
 Coef[7,1] := XB; Coef[7,2] := YB; Coef[7,3] := ZB; Coef[7,12] := -XpB;
 Coef[8,4] := XB; Coef[8,5] := YB; Coef[8,6] := ZB; Coef[8,12] := -YpB;
 Coef[9,7] := XB; Coef[9,8] := YB; Coef[9,9] := ZB; Coef[9,12] := -1+XpB+YpB;
 {hvid sk‘rm}
 Coef[10,1] := X65; Coef[10,2] := Y65; Coef[10,3] := Z65;
 Coef[11,4] := X65; Coef[11,5] := Y65; Coef[11,6] := Z65;
 Coef[12,7] := X65; Coef[12,8] := Y65; Coef[12,9] := Z65;

 Constants[10] := LumValue / Yp65 * Xp65;
 Constants[11] := LumValue;
 Constants[12] := LumValue / Yp65 * (1-Xp65-Yp65);

 Gaussian_Elimination(Dimen, Coef, Constants, Solution, Error);

 Err := Error;
 FOR n:= 1 TO 9 DO
  KM[n] := Solution[n];
END; {FindK}



BEGIN
END.
