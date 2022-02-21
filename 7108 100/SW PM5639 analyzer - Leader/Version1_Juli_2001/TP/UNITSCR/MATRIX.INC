{----------------------------------------------------------------------------}
{-                                                                          -}
{-     Turbo Pascal Numerical Methods Toolbox                               -}
{-     Copyright (c) 1986, 87 by Borland International, Inc.                -}
{-                                                                          -}
{----------------------------------------------------------------------------}

procedure Determinant{(Dimen : integer;
                      Data   : TNmatrix;
                  var Det    : Float;
                  var Error  : byte)};

procedure Initial(Dimen : integer;
              var Data  : TNmatrix;
              var Det   : Float;
              var Error : byte);

{---------------------------------------------------------}
{- This procedure tests for errors in the value of Dimen -}
{---------------------------------------------------------}

begin
  Error := 0;
  if Dimen < 1 then
    Error := 1
  else
    if Dimen = 1 then
      Det := Data[1, 1];
end; { procedure Initial }

procedure EROswitch(var Row1 : TNvector;
                    var Row2 : TNvector);

{-------------------------------------------------}
{- Elementary row operation - switching two rows -}
{-------------------------------------------------}

var
  DummyRow : TNvector;

begin
  DummyRow := Row1;
  Row1 := Row2;
  Row2 := DummyRow;
end; { procedure EROswitch }

procedure EROmultAdd(Multiplier   : Float;
                     Dimen        : integer;
                 var ReferenceRow : TNvector;
                 var ChangingRow  : TNvector);

{-----------------------------------------------------------}
{- Row operation - adding a multiple of one row to another -}
{-----------------------------------------------------------}

var
  Term : integer;

begin
  for Term := 1 to Dimen do
    ChangingRow[Term] := ChangingRow[Term] + Multiplier * ReferenceRow[Term];
end; { procedure EROmultAdd }

function Deter(Dimen : integer;
           var Data  : TNmatrix) : Float;

{-------------------------------------------------------}
{- Input: Dimen, Data                                  -}
{- Output: Deter                                       -}
{-                                                     -}
{- Function returns the determinant of the Data matrix -}
{-------------------------------------------------------}

var
  PartialDeter, Multiplier : Float;
  Row, ReferenceRow : integer;
  DetEqualsZero : boolean;

procedure Pivot(Dimen         : integer;
                ReferenceRow  : integer;
            var Data          : TNmatrix;
            var PartialDeter  : Float;
            var DetEqualsZero : boolean);

{------------------------------------------------------------}
{- Input: Dimen, ReferenceRow, Data, PartialDeter           -}
{- Output: Data, PartialDeter, DetEqualsZero                -}
{-                                                          -}
{- This procedure searches the ReferenceRow column of the   -}
{- matrix Data for the first non-zero element below the     -}
{- diagonal. If it finds one, then the procedure switches   -}
{- rows so that the non-zero element is on the diagonal.    -}
{- Switching rows changes the determinant by a factor of    -}
{- -1; this change is returned in PartialDeter.             -}
{- If it doesn't find one, the matrix is singular and the   -}
{- Determinant is zero (DetEqualsZero = true is returned).  -}
{------------------------------------------------------------}

var
  NewRow : integer;

begin
  DetEqualsZero := true;
  NewRow := ReferenceRow;
  while DetEqualsZero and (NewRow < Dimen) do  { Try to find a row  }
                                               { with a non-zero    }
                                               { element in this    }
                                               { column             }
  begin
    NewRow := Succ(NewRow);
    if ABS(Data[NewRow, ReferenceRow]) > TNNearlyZero then
    begin
      EROswitch(Data[NewRow], Data[ReferenceRow]);
      { Switch these two rows }
      DetEqualsZero := false;
      PartialDeter := -PartialDeter;  { Switching rows changes }
                                      { the determinant by a   }
                                      { factor of -1           }
    end;
  end;
end; { procedure Pivot }

begin  { function Deter }
  DetEqualsZero := false;
  PartialDeter := 1;
  ReferenceRow := 0;
  { Make the matrix upper triangular }
  while not(DetEqualsZero) and (ReferenceRow < Dimen - 1) do
  begin
    ReferenceRow := Succ(ReferenceRow);
    { If diagonal element is zero then switch rows }
    if ABS(Data[ReferenceRow, ReferenceRow]) < TNNearlyZero then
      Pivot(Dimen, ReferenceRow, Data, PartialDeter, DetEqualsZero);
    if not(DetEqualsZero) then
      for Row := ReferenceRow + 1 to Dimen do
        { Make the ReferenceRow element of this row zero }
        if ABS(Data[Row, ReferenceRow]) > TNNearlyZero then
        begin
          Multiplier := -Data[Row, ReferenceRow] /
                        Data[ReferenceRow, ReferenceRow];
          EROmultAdd(Multiplier, Dimen, Data[ReferenceRow], Data[Row]);
        end;
    { Multiply the diagonal Term into PartialDeter }
    PartialDeter := PartialDeter * Data[ReferenceRow, ReferenceRow];
  end;
  if DetEqualsZero then
    Deter := 0
  else
    Deter := PartialDeter * Data[Dimen, Dimen];
end; { function Deter }

begin { procedure Determinant }
  Initial(Dimen, Data, Det, Error);
  if Dimen > 1 then
    Det := Deter(Dimen, Data);
end; { procedure Determinant }

procedure Inverse{(Dimen : integer;
                  Data   : TNmatrix;
              var Inv    : TNmatrix;
              var Error  : byte)};


procedure Initial(Dimen : integer;
              var Data  : TNmatrix;
              var Inv   : TNmatrix;
              var Error : byte);

{--------------------------------------------------------}
{- Input: Dimen, Data                                   -}
{- Output: Inv, Error                                   -}
{-                                                      -}
{- This procedure test for errors in the value of Dimen -}
{--------------------------------------------------------}

var
  Row : integer;

begin
  Error := 0;
  if Dimen < 1 then
    Error := 1
  else
    begin
      { First make the inverse-to-be the identity matrix }
      FillChar(Inv, SizeOf(Inv), 0);
      for Row := 1 to Dimen do
        Inv[Row, Row] := 1;
      if Dimen = 1 then
        if ABS(Data[1, 1]) < TNNearlyZero then
          Error := 2   { Singular matrix }
        else
          Inv[1, 1] := 1 / Data[1, 1];
    end;
end; { procedure Initial }

procedure EROdiv(Divisor : Float;
                 Dimen   : integer;
             var Row     : TNvector);

{-----------------------------------------------------}
{- Input: Divisor, Dimen, Row                        -}
{-                                                   -}
{- elementary row operation - dividing by a constant -}
{-----------------------------------------------------}

var
  Term : integer;

begin
  for Term := 1 to Dimen do
    Row[Term] := Row[Term] / Divisor;
end; { procedure EROdiv }

procedure EROswitch(var Row1 : TNvector;
                    var Row2 : TNvector);

{-------------------------------------------------}
{- Input: Row1, Row2                             -}
{- Output: Row1, Row2                            -}
{-                                               -}
{- Elementary row operation - switching two rows -}
{-------------------------------------------------}

var
  DummyRow : TNvector;

begin
  DummyRow := Row1;
  Row1 := Row2;
  Row2 := DummyRow;
end; { procedure EROswitch }

procedure EROmultAdd(Multiplier   : Float;
                     Dimen        : integer;
                 var ReferenceRow : TNvector;
                 var ChangingRow  : TNvector);

{-----------------------------------------------------------}
{- Input: Multiplier, Dimen, ReferenceRow, ChangingRow     -}
{- Output: ChangingRow                                     -}
{-                                                         -}
{- Row operation - adding a multiple of one row to another -}
{-----------------------------------------------------------}

var
  Term : integer;

begin
  for Term := 1 to Dimen do
    ChangingRow[Term] := ChangingRow[Term] + Multiplier*ReferenceRow[Term];
end; { procedure EROmultAdd }


procedure Inver(Dimen : integer;
            var Data  : TNmatrix;
            var Inv   : TNmatrix;
            var Error : byte);

{----------------------------------------------------------}
{- Input: Dimen, Data                                     -}
{- Output: Inv, Error                                     -}
{-                                                        -}
{- This procedure computes the inverse of the matrix Data -}
{- and stores it in the matrix Inv.  If the matrix Data   -}
{- is singular, then Error = 2 is returned.               -}
{----------------------------------------------------------}

var
  Divisor, Multiplier : Float;
  Row, ReferenceRow : integer;

procedure Pivot(Dimen        : integer;
                ReferenceRow : integer;
            var Data         : TNmatrix;
            var Inv          : TNmatrix;
            var Error        : byte);

{-------------------------------------------------------------}
{- Input: Dimen, ReferenceRow, Data, Inv                     -}
{- Output: Data, Inv, Error                                  -}
{-                                                           -}
{- This procedure searches the ReferenceRow column of        -}
{- the Data matrix for the first non-zero element below      -}
{- the diagonal. If it finds one, then the procedure         -}
{- switches rows so that the non-zero element is on the      -}
{- diagonal. This same operation is applied to the Inv       -}
{- matrix. If no non-zero element exists in a column, the    -}
{- matrix is singular and no inverse exists.                 -}
{-------------------------------------------------------------}

var
  NewRow : integer;

begin
  Error := 2;  { No inverse exists }
  NewRow := ReferenceRow;
  while (Error > 0) and (NewRow < Dimen) do
  { Try to find a       }
  { row with a non-zero }
  { diagonal element    }
  begin
    NewRow := Succ(NewRow);
    if ABS(Data[NewRow, ReferenceRow]) > TNNearlyZero then
    begin
      EROswitch(Data[NewRow], Data[ReferenceRow]);
      { Switch these two rows }
      EROswitch(Inv[NewRow], Inv[ReferenceRow]);
      Error := 0;
    end;
  end; { while }
end; { procedure Pivot }

begin { procedure Inver }
  { Make Data matrix upper triangular }
  ReferenceRow := 0;
  while (Error = 0) and (ReferenceRow < Dimen) do
  begin
    ReferenceRow := Succ(ReferenceRow);
    { Check to see if the diagonal element is zero }
    if ABS(Data[ReferenceRow, ReferenceRow]) < TNNearlyZero then
      Pivot(Dimen, ReferenceRow, Data, Inv, Error);
    if Error = 0 then
    begin
      Divisor := Data[ReferenceRow, ReferenceRow];
      EROdiv(Divisor, Dimen, Data[ReferenceRow]);
      EROdiv(Divisor, Dimen, Inv[ReferenceRow]);
      for Row := 1 to Dimen do
        { Make the ReferenceRow element of this row zero }
        if (Row <> ReferenceRow) and
           (ABS(Data[Row, ReferenceRow]) > TNNearlyZero) then
        begin
          Multiplier := -Data[Row, ReferenceRow] /
                         Data[ReferenceRow, ReferenceRow];
          EROmultAdd(Multiplier, Dimen, Data[ReferenceRow], Data[Row]);
          EROmultAdd(Multiplier, Dimen, Inv[ReferenceRow], Inv[Row]);
        end;
    end;
  end;
end; { procedure Inver }

begin { procedure Inverse }
  Initial(Dimen, Data, Inv, Error);
  if Dimen > 1 then
    Inver(Dimen, Data, Inv, Error);
end; { procedure Inverse }

procedure Gaussian_Elimination{(Dimen       : integer;
                               Coefficients : TNmatrix;
                               Constants    : TNvector;
                           var Solution     : TNvector;
                           var Error        : byte)};

procedure Initial(Dimen        : integer;
              var Coefficients : TNmatrix;
              var Constants    : TNvector;
              var Solution     : TNvector;
              var Error        : byte);

{----------------------------------------------------------}
{- Input: Dimen, Coefficients, Constants                  -}
{- Output: Solution, Error                                -}
{-                                                        -}
{- This procedure test for errors in the value of Dimen.  -}
{- This procedure also finds the solution for the         -}
{- trivial case Dimen = 1.                                -}
{----------------------------------------------------------}

begin
  Error := 0;
  if Dimen < 1 then
    Error := 1
  else
    if Dimen = 1 then
      if ABS(Coefficients[1, 1]) < TNNearlyZero then
        Error := 2
      else
        Solution[1] := Constants[1] / Coefficients[1, 1];
end; { procedure Initial }

procedure EROswitch(var Row1 : TNvector;
                    var Row2 : TNvector);

{-------------------------------------------------}
{- Input: Row1, Row2                             -}
{- Output: Row1, Row2                            -}
{-                                               -}
{- elementary row operation - switching two rows -}
{-------------------------------------------------}

var
  DummyRow : TNvector;

begin
  DummyRow := Row1;
  Row1 := Row2;
  Row2 := DummyRow;
end; { procedure EROswitch }

procedure EROmultAdd(Multiplier   : Float;
                     Dimen        : integer;
                 var ReferenceRow : TNvector;
                 var ChangingRow  : TNvector);

{-----------------------------------------------------------}
{- Input: Multiplier, Dimen, ReferenceRow, ChangingRow     -}
{- Output: ChangingRow                                     -}
{-                                                         -}
{- row operation - adding a multiple of one row to another -}
{-----------------------------------------------------------}

var
  Term : integer;

begin
  for Term := 1 to Dimen do
    ChangingRow[Term] := ChangingRow[Term] + Multiplier*ReferenceRow[Term];
end; { procedure EROmultAdd }

procedure UpperTriangular(Dimen        : integer;
                      var Coefficients : TNmatrix;
                      var Constants    : TNvector;
                      var Error        : byte);

{-----------------------------------------------------------------}
{- Input: Dimen, Coefficients, Constants                         -}
{- Output: Coefficients, Constants, Error                        -}
{-                                                               -}
{- This procedure makes the coefficient matrix upper triangular. -}
{- The operations which perform this are also performed on the   -}
{- Constants vector.                                             -}
{- If one of the main diagonal elements of the upper triangular  -}
{- matrix is zero, then the Coefficients matrix is singular and  -}
{- no solution exists (Error = 2 is returned).                   -}
{-----------------------------------------------------------------}

var
  Multiplier        : Float;
  Row, ReferenceRow : integer;

procedure Pivot(Dimen        : integer;
                ReferenceRow : integer;
            var Coefficients : TNmatrix;
            var Constants    : TNvector;
            var Error        : byte);

{--------------------------------------------------------------}
{- Input: Dimen, ReferenceRow, Coefficients                   -}
{- Output: Coefficients, Constants, Error                     -}
{-                                                            -}
{- This procedure searches the ReferenceRow column of the     -}
{- Coefficients matrix for the first non-zero element below   -}
{- the diagonal. If it finds one, then the procedure switches -}
{- rows so that the non-zero element is on the diagonal.      -}
{- It also switches the corresponding elements in the         -}
{- Constants vector. If it doesn't find one, the matrix is    -}
{- singular and no solution exists (Error = 2 is returned).   -}
{--------------------------------------------------------------}

var
  NewRow : integer;
  Dummy : Float;

begin
  Error := 2;          { No solution exists }
  NewRow := ReferenceRow;
  while (Error > 0) and (NewRow < Dimen) do    { Try to find a       }
                                               { row with a non-zero }
                                               { diagonal element    }
  begin
    NewRow := Succ(NewRow);
    if ABS(Coefficients[NewRow, ReferenceRow]) > TNNearlyZero then
    begin
      EROswitch(Coefficients[NewRow], Coefficients[ReferenceRow]);
      { Switch these two rows }
      Dummy := Constants[NewRow];
      Constants[NewRow] := Constants[ReferenceRow];
      Constants[ReferenceRow] := Dummy;
      Error := 0;    { Solution may exist }
    end;
  end;
end; { procedure Pivot }

begin { procedure UpperTriangular }
  ReferenceRow := 0;
  while (Error = 0) and (ReferenceRow < Dimen - 1) do
  begin
    ReferenceRow := Succ(ReferenceRow);
    { Check to see if the main diagonal element is zero }
    if ABS(Coefficients[ReferenceRow, ReferenceRow]) < TNNearlyZero then
      Pivot(Dimen, ReferenceRow, Coefficients, Constants, Error);
    if Error = 0 then
      for Row := ReferenceRow + 1 to Dimen do
        { Make the ReferenceRow element of this row zero }
        if ABS(Coefficients[Row, ReferenceRow]) > TNNearlyZero then
        begin
          Multiplier := -Coefficients[Row, ReferenceRow] /
                         Coefficients[ReferenceRow,ReferenceRow];
          EROmultAdd(Multiplier, Dimen,
                     Coefficients[ReferenceRow], Coefficients[Row]);
          Constants[Row] := Constants[Row] +
                            Multiplier * Constants[ReferenceRow];
        end;
  end; { while }
  if ABS(Coefficients[Dimen, Dimen]) < TNNearlyZero then
    Error := 2;    { No solution }
end; { procedure UpperTriangular }

procedure BackwardsSub(Dimen        : integer;
                   var Coefficients : TNmatrix;
                   var Constants    : TNvector;
                   var Solution     : TNvector);

{----------------------------------------------------------------}
{- Input: Dimen, Coefficients, Constants                        -}
{- Output: Solution                                             -}
{-                                                              -}
{- This procedure applies backwards substitution to the upper   -}
{- triangular Coefficients matrix and Constants vector. The     -}
{- resulting vector is the solution to the set of equations and -}
{- is returned in the vector Solution.                          -}
{----------------------------------------------------------------}

var
  Term, Row : integer;
  Sum : Float;

begin
  Term := Dimen;
  while Term >= 1 do
  begin
    Sum := 0;
    for Row := Term + 1 to Dimen do
      Sum := Sum + Coefficients[Term, Row] * Solution[Row];
    Solution[Term] := (Constants[Term] - Sum) / Coefficients[Term, Term];
    Term := Pred(Term);
  end;
end; { procedure BackwardsSub }

begin { procedure Gaussian_Elimination }
  Initial(Dimen, Coefficients, Constants, Solution, Error);
  if Dimen > 1 then
  begin
    UpperTriangular(Dimen, Coefficients, Constants, Error);
    if Error = 0 then
      BackwardsSub(Dimen, Coefficients, Constants, Solution);
  end;
end; { procedure Gaussian_Elimination }

procedure Partial_Pivoting{(Dimen       : integer;
                           Coefficients : TNmatrix;
                           Constants    : TNvector;
                       var Solution     : TNvector;
                       var Error        : byte)};


procedure Initial(Dimen        : integer;
              var Coefficients : TNmatrix;
              var Constants    : TNvector;
              var Solution     : TNvector;
              var Error        : byte);

{----------------------------------------------------------}
{- Input: Dimen, Coefficients, Constants                  -}
{- Output: Solution, Error                                -}
{-                                                        -}
{- This procedure test for errors in the value of Dimen.  -}
{- This procedure also finds the solution for the         -}
{- trivial case Dimen = 1.                                -}
{----------------------------------------------------------}

begin
  Error := 0;
  if Dimen < 1 then
    Error := 1
  else
    if Dimen = 1 then
      if ABS(Coefficients[1, 1]) < TNNearlyZero then
        Error := 2
      else
        Solution[1] := Constants[1] / Coefficients[1, 1];
end; { procedure Initial }

procedure EROswitch(var Row1 : TNvector;
                    var Row2 : TNvector);

{-------------------------------------------------}
{- Input: Row1, Row2                             -}
{- Output: Row1, Row2                            -}
{-                                               -}
{- elementary row operation - switching two rows -}
{-------------------------------------------------}

var
  DummyRow : TNvector;

begin
  DummyRow := Row1;
  Row1 := Row2;
  Row2 := DummyRow;
end; { procedure EROswitch }

procedure EROmultAdd(Multiplier   : Float;
                     Dimen        : integer;
                 var ReferenceRow : TNvector;
                 var ChangingRow  : TNvector);

{-----------------------------------------------------------}
{- Input: Multiplier, Dimen, ReferenceRow, ChangingRow     -}
{- Output: ChangingRow                                     -}
{-                                                         -}
{- Row operation - adding a multiple of one row to another -}
{-----------------------------------------------------------}

var
  Term : integer;

begin
  for Term := 1 to Dimen do
    ChangingRow[Term] := ChangingRow[Term] + Multiplier*ReferenceRow[Term];
end; { procedure EROmultAdd }

procedure UpperTriangular(Dimen        : integer;
                      var Coefficients : TNmatrix;
                      var Constants    : TNvector;
                      var Error        : byte);

{-----------------------------------------------------------------}
{- Input: Dimen, Coefficients, Constants                         -}
{- Output: Coefficients, Constants, Error                        -}
{-                                                               -}
{- This procedure makes the coefficient matrix upper triangular. -}
{- The operations which perform this are also performed on the   -}
{- Constants vector.                                             -}
{- If one of the main diagonal elements of the upper triangular  -}
{- matrix is zero, then the Coefficients matrix is singular and  -}
{- no solution exists (Error = 2 is returned).                   -}
{-----------------------------------------------------------------}

var
  Multiplier : Float;
  Row, ReferenceRow : integer;

procedure Pivot(Dimen        : integer;
                ReferenceRow : integer;
            var Coefficients : TNmatrix;
            var Constants    : TNvector;
            var Error        : byte);

{----------------------------------------------------------------}
{- Input: Dimen, ReferenceRow, Coefficients                     -}
{- Output: Coefficients, Constants, Error                       -}
{-                                                              -}
{- This procedure searches the ReferenceRow column of the       -}
{- Coefficients matrix for the largest non-zero element below   -}
{- the diagonal. If it finds one, then the procedure switches   -}
{- rows so that the largest non-zero element is on the          -}
{- diagonal. It also switches the corresponding elements in     -}
{- the Constants vector. If it doesn't find a non-zero element, -}
{- the matrix is singular and no solution exists                -}
{- (Error = 2 is returned).                                     -}
{----------------------------------------------------------------}

var
  PivotRow, Row : integer;
  Dummy : Float;

begin
  { First, find the row with the largest element }
  PivotRow := ReferenceRow;
  for Row := ReferenceRow + 1 to Dimen do
    if ABS(Coefficients[Row, ReferenceRow]) >
       ABS(Coefficients[PivotRow, ReferenceRow]) then
      PivotRow := Row;
  if PivotRow <> ReferenceRow then
    { Second, switch these two rows }
    begin
      EROswitch(Coefficients[PivotRow], Coefficients[ReferenceRow]);
      Dummy := Constants[PivotRow];
      Constants[PivotRow] := Constants[ReferenceRow];
      Constants[ReferenceRow] := Dummy;
    end
  else { If the diagonal element is zero, no solution exists }
    if ABS(Coefficients[ReferenceRow, ReferenceRow]) < TNNearlyZero then
      Error := 2;     { No solution }
end; { procedure Pivot }

begin { procedure UpperTriangular }
  { Make Coefficients matrix upper triangular }
  ReferenceRow := 0;
  while (Error = 0) and (ReferenceRow < Dimen - 1) do
  begin
    ReferenceRow := Succ(ReferenceRow);
    { Find row with largest element in this column }
    { and switch this row with the ReferenceRow    }
    Pivot(Dimen, ReferenceRow, Coefficients, Constants, Error);
    if Error = 0 then
      for Row := ReferenceRow + 1 to Dimen do
        { Make the ReferenceRow element of these rows zero }
        if ABS(Coefficients[Row, ReferenceRow]) > TNNearlyZero then
        begin
          Multiplier := -Coefficients[Row, ReferenceRow] /
                         Coefficients[ReferenceRow,ReferenceRow];
          EROmultAdd(Multiplier, Dimen,
                     Coefficients[ReferenceRow], Coefficients[Row]);
          Constants[Row] := Constants[Row] +
                            Multiplier * Constants[ReferenceRow];
        end;
  end;
  if ABS(Coefficients[Dimen, Dimen]) < TNNearlyZero then
    Error := 2;    { No solution }
end; { procedure UpperTriangular }

procedure BackwardsSub(Dimen        : integer;
                   var Coefficients : TNmatrix;
                   var Constants    : TNvector;
                   var Solution     : TNvector);

{----------------------------------------------------------------}
{- Input: Dimen, Coefficients, Constants                        -}
{- Output: Solution                                             -}
{-                                                              -}
{- This procedure applies backwards substitution to the upper   -}
{- triangular Coefficients matrix and Constants vector. The     -}
{- resulting vector is the solution to the set of equations and -}
{- is stored in the vector Solution.                            -}
{----------------------------------------------------------------}

var
  Term, Row : integer;
  Sum : Float;

begin
  Term := Dimen;
  while Term >= 1 do
  begin
    Sum := 0;
    for Row := Term + 1 to Dimen do
      Sum := Sum + Coefficients[Term, Row] * Solution[Row];
    Solution[Term] := (Constants[Term] - Sum) / Coefficients[Term, Term];
    Term := Pred(Term);
  end;
end; { procedure BackwardsSub }

begin  { procedure Partial_Pivoting }
  Initial(Dimen, Coefficients, Constants, Solution, Error);
  if Dimen > 1 then
  begin
    UpperTriangular(Dimen, Coefficients, Constants, Error);
    if Error = 0 then
      BackwardsSub(Dimen, Coefficients, Constants, Solution);
  end;
end; { procedure Partial_Pivoting }

procedure LU_Decompose{(Dimen       : integer;
                       Coefficients : TNmatrix;
                   var Decomp       : TNmatrix;
                   var Permute      : TNmatrix;
                   var Error        : byte)};


procedure TestInput(Dimen : integer;
                var Error : byte);

{---------------------------------------}
{- Input: Dimen                        -}
{- Output: Error                       -}
{-                                     -}
{- This procedure checks to see if the -}
{- value of Dimen is greater than 1.   -}
{---------------------------------------}

begin
  Error := 0;
  if Dimen < 1 then
    Error := 1;
end; { procedure TestInput }

function RowColumnMult(Row    : integer;
                   var Lower  : TNmatrix;
                       Column : integer;
                   var Upper  : TNmatrix) : Float;

{----------------------------------------------------}
{- Input: Row, Lower, Column, Upper                 -}
{- Function return: dot product of row Row of Lower -}
{-                  and column Column of Upper      -}
{----------------------------------------------------}

var
  Term : integer;
  Sum : Float;

begin
  Sum := 0;
  for Term := 1 to Row - 1 do
    Sum := Sum + Lower[Row, Term] * Upper[Term, Column];
  RowColumnMult := Sum;
end; { function RowColumnMult }


procedure Pivot(Dimen        : integer;
                ReferenceRow : integer;
            var Coefficients : TNmatrix;
            var Lower        : TNmatrix;
            var Upper        : TNmatrix;
            var Permute      : TNmatrix;
            var Error        : byte);

{----------------------------------------------------------------}
{- Input: Dimen, ReferenceRow, Coefficients,                    -}
{-        Lower, Upper, Permute                                 -}
{- Output: Coefficients, Lower, Permute, Error                  -}
{-                                                              -}
{- This procedure searches the ReferenceRow column of the       -}
{- Coefficients matrix for the element in the Row below the     -}
{- main diagonal which produces the largest value of            -}
{-                                                              -}
{-         Coefficients[Row, ReferenceRow] -                    -}
{-                                                              -}
{-          Sum K=1 to ReferenceRow - 1 of                      -}
{-                  Upper[Row, k] - Lower[k, ReferenceRow]      -}
{-                                                              -}
{- If it finds one, then the procedure switches                 -}
{- rows so that this element is on the main diagonal. The       -}
{- procedure also switches the corresponding elements in the    -}
{- Permute matrix and the Lower matrix. If the largest value of -}
{- the above expression is zero, then the matrix is singular    -}
{- and no solution exists (Error = 2 is returned).              -}
{----------------------------------------------------------------}

var
  PivotRow, Row : integer;
  ColumnMax, TestMax : Float;

procedure EROswitch(var Row1 : TNvector;
                    var Row2 : TNvector);

{-------------------------------------------------}
{- Input: Row1, Row2                             -}
{- Output: Row1, Row2                            -}
{-                                               -}
{- elementary row operation - switching two rows -}
{-------------------------------------------------}

var
  DummyRow : TNvector;

begin
  DummyRow := Row1;
  Row1 := Row2;
  Row2 := DummyRow;
end; { procedure EROswitch }

begin { procedure Pivot }
  { First, find the row with the largest TestMax }
  PivotRow := ReferenceRow;
  ColumnMax := ABS(Coefficients[ReferenceRow, ReferenceRow] -
               RowColumnMult(ReferenceRow, Lower, ReferenceRow, Upper));
  for Row := ReferenceRow + 1 to Dimen do
  begin
    TestMax := ABS(Coefficients[Row, ReferenceRow] -
               RowColumnMult(Row, Lower, ReferenceRow, Upper));

    if TestMax > ColumnMax then
    begin
      PivotRow := Row;
      ColumnMax := TestMax;
    end;
  end;

  if PivotRow <> ReferenceRow then
    { Second, switch these two rows }
    begin
      EROswitch(Coefficients[PivotRow], Coefficients[ReferenceRow]);
      EROswitch(Lower[PivotRow], Lower[ReferenceRow]);
      EROswitch(Permute[PivotRow], Permute[ReferenceRow]);
    end
  else
    { If ColumnMax is zero, no solution exists }
    if ColumnMax < TNNearlyZero then
      Error := 2;     { No solution exists }
end; { procedure Pivot }

procedure Decompose(Dimen        : integer;
                var Coefficients : TNmatrix;
                var Decomp       : TNmatrix;
                var Permute      : TNmatrix;
                var Error        : byte);

{---------------------------------------------------------}
{- Input: Dimen, Coefficients                            -}
{- Output: Decomp, Permute, Error                        -}
{-                                                       -}
{- This procedure decomposes the Coefficients matrix     -}
{- into two triangular matrices, a lower and an upper    -}
{- one.  The lower and upper matrices are combined       -}
{- into one matrix, Decomp.  The permutation matrix,     -}
{- Permute, records the effects of partial pivoting.     -}
{---------------------------------------------------------}

var
  Upper, Lower : TNmatrix;
  Term, Index : integer;

procedure Initialize(Dimen   : integer;
                 var Lower   : TNmatrix;
                 var Upper   : TNmatrix;
                 var Permute : TNmatrix);

{---------------------------------------------------}
{- Output: Dimen, Lower, Upper, Permute            -}
{-                                                 -}
{- This procedure initializes the above variables. -}
{- Lower and Upper are initialized to the zero     -}
{- matrix and Diag is initialized to the identity  -}
{- matrix.                                         -}
{---------------------------------------------------}

var
  Diag : integer;

begin
  FillChar(Upper, SizeOf(Upper), 0);
  FillChar(Lower, SizeOf(Lower), 0);
  FillChar(Permute, SizeOf(Permute), 0);
  for Diag := 1 to Dimen do
    Permute[Diag, Diag] := 1;
end; { procedure Initialize }

begin { procedure Decompose }
  Initialize(Dimen, Lower, Upper, Permute);

  {  partial pivoting on row 1 }
  Pivot(Dimen, 1, Coefficients, Lower, Upper, Permute, Error);
  if Error = 0 then
  begin
    Lower[1, 1] := 1;
    Upper[1, 1] := Coefficients[1, 1];

    for Term := 1 to Dimen do
    begin
      Lower[Term, 1] := Coefficients[Term, 1] / Upper[1, 1];
      Upper[1, Term] := Coefficients[1, Term] / Lower[1, 1];
    end;
  end;

  Term := 1;
  while (Error = 0) and (Term < Dimen - 1) do
  begin
    Term := Succ(Term);

    { perform partial pivoting on row Term }
    Pivot(Dimen, Term, Coefficients, Lower, Upper, Permute, Error);

    Lower[Term, Term] := 1;
    Upper[Term, Term] := Coefficients[Term, Term] -
                         RowColumnMult(Term, Lower, Term, Upper);

    if ABS(Upper[Term, Term]) < TNNearlyZero then
      Error := 2   { no solutions }
    else
      for Index := Term + 1 to Dimen do
      begin
        Upper[Term, Index] := Coefficients[Term, Index] -
                              RowColumnMult(Term, Lower, Index, Upper);
        Lower[Index, Term] := (Coefficients[Index, Term] -
                              RowColumnMult(Index, Lower, Term, Upper)) /
                              Upper[Term, Term];
      end;
  end;

  Lower[Dimen, Dimen] := 1;
  Upper[Dimen, Dimen] := Coefficients[Dimen, Dimen] -
                         RowColumnMult(Dimen, Lower, Dimen, Upper);
  if ABS(Upper[Dimen, Dimen]) < TNNearlyZero then
    Error := 2;
  { Combine the upper and lower triangular matrices into one }
  Decomp := Upper;

  for Term := 2 to Dimen do
    for Index := 1 to Term - 1 do
      Decomp[Term, Index] := Lower[Term, Index];
end; { procedure Decompose }

begin { procedure LU_Decompose }
  TestInput(Dimen, Error);
  if Error = 0 then
    if Dimen = 1 then
      begin
        Decomp := Coefficients;
        Permute[1, 1] := 1;
      end
    else
      Decompose(Dimen, Coefficients, Decomp, Permute, Error);
end; { procedure LU_Decompose }

procedure LU_Solve{(Dimen     : integer;
               var Decomp    : TNmatrix;
                   Constants : TNvector;
               var Permute   : TNmatrix;
               var Solution  : TNvector;
               var Error     : byte)};

procedure Initial(Dimen    : integer;
              var Solution : TNvector;
              var Error    : byte);

{----------------------------------------------------}
{- Input: Dimen                                     -}
{- Output: Solution, Error                          -}
{-                                                  -}
{- This procedure initializes the Solution vector.  -}
{- It also checks to see if the value of Dimen is   -}
{- greater than 1.                                  -}
{----------------------------------------------------}

begin
  Error := 0;
  FillChar(Solution, SizeOf(Solution), 0);
  if Dimen < 1 then
    Error := 1;
end; { procedure Initial }

procedure FindSolution(Dimen     : integer;
                   var Decomp    : TNmatrix;
                   var Constants : TNvector;
                   var Solution  : TNvector);

{---------------------------------------------------------------}
{- Input: Dimen, Decomp, Constants                             -}
{- Output: Solution                                            -}
{-                                                             -}
{- The Decom matrix contains a lower and upper triangular      -}
{- matrix.                                                     -}
{- This procedure performs a two step backwards substitution   -}
{- to compute the solution to the system of equations.  First, -}
{- forward substitution is applied to the lower triangular     -}
{- matrix and Constants vector yielding PartialSolution.  Then -}
{- backwards substitution is applied to the Upper matrix and   -}
{- the PartialSolution vector yielding Solution.               -}
{---------------------------------------------------------------}

var
  PartialSolution : TNvector;
  Term, Index : integer;
  Sum : Float;

begin { procedure FindSolution }
  { First solve the lower triangular matrix }
  PartialSolution[1] := Constants[1];
  for Term := 2 to Dimen do
  begin
    Sum := 0;
    for Index := 1 to Term - 1 do
      if Term = Index then
        Sum := Sum + PartialSolution[Index]
      else
        Sum := Sum + Decomp[Term, Index] * PartialSolution[Index];
    PartialSolution[Term] := Constants[Term] - Sum;
  end;
  { Then solve the upper triangular matrix }
  Solution[Dimen] := PartialSolution[Dimen] / Decomp[Dimen, Dimen];
  for Term := Dimen - 1 downto 1 do
  begin
    Sum := 0;
    for Index := Term + 1 to Dimen do
      Sum := Sum + Decomp[Term, Index] * Solution[Index];
    Solution[Term] := (PartialSolution[Term] - Sum)/Decomp[Term, Term];
  end;
end; { procedure FindSolution }

procedure PermuteConstants(Dimen     : integer;
                       var Permute   : TNmatrix;
                       var Constants : TNvector);

var
  Row, Column : integer;
  Entry : Float;
  TempConstants : TNvector;

begin
  for Row := 1 to Dimen do
  begin
    Entry := 0;
    for Column := 1 to Dimen do
      Entry := Entry + Permute[Row, Column] * Constants[Column];
    TempConstants[Row] := Entry;
  end;
  Constants := TempConstants;
end; { procedure PermuteConstants }

begin { procedure Solve_LU_Decompostion }
  Initial(Dimen, Solution, Error);
  if Error = 0 then
    PermuteConstants(Dimen, Permute, Constants);
  FindSolution(Dimen, Decomp, Constants, Solution);
end; { procedure LU_Solve }

procedure Gauss_Seidel{(Dimen      : integer;
                      Coefficients : TNmatrix;
                      Constants    : TNvector;
                      Tol          : Float;
                      MaxIter      : integer;
                  var Solution     : TNvector;
                  var Iter         : integer;
                  var Error        : byte)};


var
  Guess : TNvector;

procedure TestInput(Dimen        : integer;
                    Tol          : Float;
                    MaxIter      : integer;
                var Coefficients : TNmatrix;
                var Constants    : TNvector;
                var Solution     : TNvector;
                var Error        : byte);

{----------------------------------}
{- Input: Dimen, Tol, MaxIter     -}
{-        Coefficients,           -}
{-        Constants               -}
{- Output: Solution, Error        -}
{-                                -}
{- test the input data for errors -}
{- The procedure also finds the   -}
{- solution for the trivial case  -}
{- Dimen = 0.                     -}
{----------------------------------}

begin
  Error := 0;
  if Dimen < 1 then
    Error := 3
  else
    if Tol <= 0 then
      Error := 4
     else
       if MaxIter < 0 then
         Error := 5;
  if (Error = 0) and (Dimen = 1) then
  begin
    if ABS(Coefficients[1, 1]) < TNNearlyZero then
      Error := 6
    else
      Solution[1] := Constants[1] / Coefficients[1, 1];
  end;
end; { procedure TestInput }

procedure TestForDiagDominance(Dimen        : integer;
                           var Coefficients : TNmatrix;
                           var Error        : byte);

{-------------------------------------------------------------------}
{- Input: Dimen, Coefficients                                      -}
{- Output: Error                                                   -}
{-                                                                 -}
{- This procedure examines the Coefficients matrix to see if it is -}
{- diagonally dominant. If it is, then the Gauss-Seidel iterative  -}
{- method will converge to a solution of this system of equations; -}
{- if not, then convergence may not be possible with this method   -}
{- and Error = 1 (which is a warning) is returned. If one of the   -}
{- elements on the main diagonal of the Coefficients matrix is     -}
{- zero, then the matrix is singular and cannot be solved and      -}
{- Error = 6 is returned. In such a case, one of the direct        -}
{- methods for solving systems of equations (e.g. Gaussian         -}
{- elimination) should be used.                                    -}
{-------------------------------------------------------------------}

var
  Row, Column : integer;
  Sum : Float;

begin
  Row := 0;
  while (Row < Dimen) and (Error < 2) do
  begin
    Row := Succ(Row);
    Sum := 0;
    for Column := 1 to Dimen do
      if Column <> Row then
        Sum := Sum + ABS(Coefficients[Row, Column]);
    if Sum > ABS(Coefficients[Row, Row]) then
      Error := 1;  { WARNING! convergence may not be }
                   { possible because matrix isn't   }
                   { diagonally dominant             }
    if ABS(Coefficients[Row, Row]) < TNNearlyZero then
      Error := 6;  { Singular matrix - can't be solved  }
                   { by the Gauss-Seidel method.        }
  end; { while }
end; { procedure TestForDiagDominance }

procedure MakeInitialGuess(Dimen        : integer;
                       var Coefficients : TNmatrix;
                       var Constants    : TNvector;
                       var Guess        : TNvector);

{-------------------------------------------------------------------}
{- Input: Dimen, Coefficients, Constants                           -}
{- Output: Guess                                                   -}
{-                                                                 -}
{- This procedure creates an initial approximation to the solution -}
{- by dividing the Constants terms by the corresponding terms      -}
{- on the main diagonal of the Coefficients matrix.                -}
{-------------------------------------------------------------------}

var
  Term : integer;

begin
  FillChar(Guess, SizeOf(Guess), 0);
  for Term := 1 to Dimen do
    if ABS(Coefficients[Term, Term]) > TNNearlyZero then
      Guess[Term] := Constants[Term] / Coefficients[Term, Term];
end; { procedure MakeInitialGuess }

procedure TestForConvergence(Dimen     : integer;
                         var OldApprox : TNvector;
                         var NewApprox : TNvector;
                             Tol       : Float;
                         var Done      : boolean;
                         var Product   : Float;
                         var Error     : byte);

{----------------------------------------------------------------------}
{- Input: Dimen, OldApprox, NewApprox, Tol, Product                   -}
{- Output: Done, Product, Error                                       -}
{-                                                                    -}
{- This procedure determines if the sequence of approximations        -}
{- has converged. For convergence to occur, the relative difference   -}
{- between each Term of OldApprox and NewApprox must be less than     -}
{- the tolerance, Tol. If so, Done = TRUE is returned.                -}
{-                                                                    -}
{- This procedure also determines if the sequence of approximations   -}
{- is diverging. Product records the total fractional change from     -}
{- the initial guess to the current iteration.  If Product is greater -}
{- than 1E20, then the sequence is assumed to have diverged. If so,   -}
{- Error = 7 is returned.                                             -}
{----------------------------------------------------------------------}

var
  Term : integer;
  PartProd : Float;

begin
  Done := true;
  PartProd := 0;
  for Term := 1 to Dimen do
  begin
    if ABS(OldApprox[Term] - NewApprox[Term]) > ABS(NewApprox[Term] * Tol) then
      Done := false;
    if (ABS(OldApprox[Term]) > TNNearlyZero) and (Error = 1) then
      { This is part of the divergence test }
      PartProd := PartProd + ABS(NewApprox[Term] / OldApprox[Term]);
  end;
  Product := Product * PartProd / Dimen;
  if Product > 1E20 then
    Error := 7   { Sequence is diverging }
end; { procedure TestForConvergence }

procedure Iterate(Dimen        : integer;
              var Coefficients : TNmatrix;
              var Constants    : TNvector;
              var Guess        : TNvector;
                  Tol          : Float;
                  MaxIter      : integer;
              var Solution     : TNvector;
              var Iter         : integer;
              var Error        : byte);

{--------------------------------------------------------------}
{- Input: Dimen, Coefficients, Constants, Guess, Tol, MaxIter -}
{- Output: Solution, Iter, Error                              -}
{-                                                            -}
{- This procedure performs the Gauss-Seidel iteration and     -}
{- returns either an error or the approximated solution and   -}
{- the number of iterations.                                  -}
{--------------------------------------------------------------}

var
  Done : boolean;
  OldApprox, NewApprox : TNvector;
  Term, Loop : integer;
  FirstSum, SecondSum, Product : Float;

begin { procedure Iterate }
  Product := 1;
  Done := false;
  Iter := 0;
  NewApprox := Guess;
  OldApprox := Guess;
  while (Iter < MaxIter) and not(Done) and (Error <= 1) do
  begin
    Iter := Succ(Iter);
    for Term := 1 to Dimen do
    begin
      FirstSum := 0;
      SecondSum := 0;
      for Loop := 1 to Term - 1 do
        FirstSum := FirstSum + Coefficients[Term, Loop] * NewApprox[Loop];
        for Loop := Term + 1 to Dimen do
          SecondSum := SecondSum + Coefficients[Term, Loop] * OldApprox[Loop];
      NewApprox[Term] := (Constants[Term] - FirstSum - SecondSum) /
                          Coefficients[Term, Term];
    end;
    TestForConvergence(Dimen, OldApprox, NewApprox, Tol, Done, Product, Error);
    OldApprox := NewApprox;
  end; { while }
  if (Iter < MaxIter) and (Error = 1) then
    Error := 0;  { The sequence converged, }
                 {  disregard the warning  }
  if (Iter >= MaxIter) and (Error = 1) then
    Error := 1;  { Matrix is not diagonally dominant; }
                 { convergence is probably impossible }
  if (Iter >= MaxIter) and (Error = 0) then
     Error := 2; { Convergence IS possible;   }
                 { more iterations are needed }
  Solution := NewApprox;
end; { procedure Iterate }

begin  { procedure Gauss_Seidel }
  TestInput(Dimen, Tol, MaxIter, Coefficients, Constants, Solution, Error);
  if Dimen > 1 then
  begin
    TestForDiagDominance(Dimen, Coefficients, Error);
    if Error < 2 then
    begin
      MakeInitialGuess(Dimen, Coefficients, Constants, Guess);
      Iterate(Dimen, Coefficients, Constants, Guess, Tol,
              MaxIter, Solution, Iter, Error);
    end;
  end;
end; { procedure Gauss_Seidel }
