unit Matrix;

{----------------------------------------------------------------------------}
{-                                                                          -}
{-     Turbo Pascal Numerical Methods Toolbox                               -}
{-     Copyright (c) 1986, 87 by Borland International, Inc.                -}
{-                                                                          -}
{-  This unit provides procedures for dealing with systems of linear        -}
{-  equations.                                                              -}
{-                                                                          -}
{----------------------------------------------------------------------------}

{$I Float.inc} { Determines the setting of the $N compiler directive }

interface

{$IFOPT N+}
type
  Float = Double; { 8 byte real, requires 8087 math chip }

const
  TNNearlyZero = 1E-015;
{$ELSE}
type
  Float = real;   { 6 byte real, no math chip required }

const
  TNNearlyZero = 1E-07;
{$ENDIF}

  TNArraySize = 30;               { Size of the matrix }

type
  TNvector = array[1..TNArraySize] of Float;
  TNmatrix = array[1..TNArraySize] of TNvector;

procedure Determinant(Dimen : integer;
                      Data  : TNmatrix;
                  var Det   : Float;
                  var Error : byte);

{----------------------------------------------------------------------------}
{-                                                                          -}
{-     Input: Dimen, Data                                                   -}
{-     Output: Det, Error                                                   -}
{-                                                                          -}
{-             Purpose : Calculate the determinant of a matrix by           -}
{-                       making it upper-triangular and then                -}
{-                       taking the product of the diagonal elements.       -}
{-                                                                          -}
{-  User-defined Types : TNvector = array[1..TNArraySize] of real;          -}
{-                       TNmatrix = array[1..TNArraySize] of TNvector       -}
{-                                                                          -}
{-    Global Variables : Dimen : integer;  Dimension of the square matrix   -}
{-                       Data  : TNmatrix; Square matrix                    -}
{-                       Det   : real;     Determinant of Data              -}
{-                       Error : integer;  Flags if something goes wrong    -}
{-                                                                          -}
{-              Errors : 0: No errors;                                      -}
{-                       1: Dimen < 1                                       -}
{-                                                                          -}
{----------------------------------------------------------------------------}

procedure Inverse(Dimen : integer;
                  Data  : TNmatrix;
              var Inv   : TNmatrix;
              var Error : byte);

{----------------------------------------------------------------------------}
{-                                                                          -}
{-                Input: Dimen, Data                                        -}
{-               Output: Inv, Error                                         -}
{-                                                                          -}
{-             Purpose : calculate the inverse of a matrix with             -}
{-                       Gauss-Jordan elimination.                          -}
{-                                                                          -}
{-  User-defined Types : TNvector = array[1..TNArraySize] of real;          -}
{-                       TNmatrix = array[1..TNArraySize] of TNvector       -}
{-                                                                          -}
{-    Global Variables : Dimen : integer;   Dimension of the square matrix  -}
{-                       Data  : TNmatrix;  Square matrix                   -}
{-                       Inv   : TNmatrix;  Inverse of Data                 -}
{-                       Error : integer;   Flags if something goes wrong   -}
{-                                                                          -}
{-              Errors : 0: No errors;                                      -}
{-                       1: Dimen < 1                                       -}
{-                       2: no inverse exists                               -}
{-                                                                          -}
{----------------------------------------------------------------------------}

procedure Gaussian_Elimination(Dimen        : integer;
                               Coefficients : TNmatrix;
                               Constants    : TNvector;
                           var Solution     : TNvector;
                           var Error        : byte);

{----------------------------------------------------------------------------}
{-                                                                          -}
{-                Input: Dimen, Coefficients, Constants                     -}
{-               Output: Solution, Error                                    -}
{-                                                                          -}
{-             Purpose : Calculate the solution of a linear set of          -}
{-                       equations using Gaussian elimination and           -}
{-                       backwards substitution.                            -}
{-                                                                          -}
{-  User-defined Types : TNvector = array[1..TNArraySize] of real           -}
{-                       TNmatrix = array[1..TNArraySize] of TNvector       -}
{-                                                                          -}
{-    Global Variables : Dimen : integer;         Dimension of the square   -}
{-                                                matrix                    -}
{-                       Coefficients : TNmatrix; Square matrix             -}
{-                       Constants    : TNvector; Constants of each equation-}
{-                       Solution     : TNvector; Unique solution to the    -}
{-                                                set of equations          -}
{-                       Error        : integer;  Flags if something goes   -}
{-                                                wrong.                    -}
{-                                                                          -}
{-              Errors:  0: No errors;                                      -}
{-                       1: Dimen < 1                                       -}
{-                       2: no solution exists                              -}
{-                                                                          -}
{-                                                                          -}
{----------------------------------------------------------------------------}

procedure Partial_Pivoting(Dimen        : integer;
                           Coefficients : TNmatrix;
                           Constants    : TNvector;
                       var Solution     : TNvector;
                       var Error        : byte);

{----------------------------------------------------------------------------}
{-                                                                          -}
{-                Input: Dimen, Coefficients, Constants                     -}
{-               Output: Solution, Error                                    -}
{-                                                                          -}
{-             Purpose : Calculate the solution of a linear set of          -}
{-                       equations using Gaussian elimination, maximal      -}
{-                       pivoting and backwards substitution.               -}
{-                                                                          -}
{-  User-defined Types : TNvector = array[1..TNArraySize] of real;          -}
{-                       TNmatrix = array[1..TNArraySize] of TNvector       -}
{-                                                                          -}
{-    Global Variables : Dimen        : integer;  Dimen of the square       -}
{-                                                matrix                    -}
{-                       Coefficients : TNmatrix; Square matrix             -}
{-                       Constants    : TNvector; Constants of each equation-}
{-                       Solution     : TNvector; Unique solution to the    -}
{-                                                set of equations          -}
{-                       Error        : integer;  Flags if something goes   -}
{-                                                wrong.                    -}
{-                                                                          -}
{-              Errors : 0: No errors;                                      -}
{-                       1: Dimen < 2                                       -}
{-                       2: no solution exists                              -}
{-                                                                          -}
{----------------------------------------------------------------------------}

procedure LU_Decompose(Dimen        : integer;
                       Coefficients : TNmatrix;
                   var Decomp       : TNmatrix;
                   var Permute      : TNmatrix;
                   var Error        : byte);

{----------------------------------------------------------------------------}
{-                                                                          -}
{-                Input: Dimen, Coefficients                                -}
{-               Output: Decomp, Permute, Error                             -}
{-                                                                          -}
{-             Purpose : Decompose a square matrix into an upper            -}
{-                       triangular and lower triangular matrix such that   -}
{-                       the product of the two triangular matrices is      -}
{-                       the original matrix. This procedure also returns   -}
{-                       a permutation matrix which records the             -}
{-                       permutations resulting from partial pivoting.      -}
{-                                                                          -}
{-  User-defined Types : TNvector = array[1..TNArraySize] of real           -}
{-                       TNmatrix = array[1..TNArraySize] of TNvector       -}
{-                                                                          -}
{-    Global Variables : Dimen        : integer;  Dimen of the coefficients -}
{-                                                Matrix                    -}
{-                       Coefficients : TNmatrix; Coefficients matrix       -}
{-                       Decomp       : TNmatrix; Decomposition of          -}
{-                                                Coefficients matrix       -}
{-                       Permute      : TNmatrix; Record of partial         -}
{-                                                Pivoting                  -}
{-                       Error        : integer;  Flags if something goes   -}
{-                                                wrong.                    -}
{-                                                                          -}
{-              Errors : 0: No errors;                                      -}
{-                       1: Dimen < 1                                       -}
{-                       2: No decomposition possible; singular matrix      -}
{-                                                                          -}
{----------------------------------------------------------------------------}

procedure LU_Solve(Dimen     : integer;
               var Decomp    : TNmatrix;
                   Constants : TNvector;
               var Permute   : TNmatrix;
               var Solution  : TNvector;
               var Error     : byte);
{----------------------------------------------------------------------------}
{-                                                                          -}
{-                Input: Dimen, Decomp, Constants, Permute                  -}
{-               Output: Solution, Error                                    -}
{-                                                                          -}
{-             Purpose : Calculate the solution of a linear set of          -}
{-                       equations using an LU decomposed matrix, a         -}
{-                       permutation matrix and backwards and forward       -}
{-                       substitution.                                      -}
{-                                                                          -}
{-  User_defined Types : TNvector = array[1..TNArraySize] of real           -}
{-                       TNmatrix = array[1..TNArraySize] of TNvector       -}
{-                                                                          -}
{-    Global Variables : Dimen     : integer;  Dimen of the square          -}
{-                                             matrix                       -}
{-                       Decomp    : TNmatrix; Decomposition of             -}
{-                                             coefficient matrix           -}
{-                       Constants : TNvector; Constants of each equation   -}
{-                       Permute   : TNmatrix; Permutation matrix from      -}
{-                                             partial pivoting             -}
{-                       Solution  : TNvector; Unique solution to the       -}
{-                                             set of equations             -}
{-                       Error     : integer;  Flags if something goes      -}
{-                                             wrong.                       -}
{-                                                                          -}
{-              Errors : 0: No errors;                                      -}
{-                       1: Dimen < 1                                       -}
{-                                                                          -}
{----------------------------------------------------------------------------}

procedure Gauss_Seidel(Dimen       : integer;
                      Coefficients : TNmatrix;
                      Constants    : TNvector;
                      Tol          : Float;
                      MaxIter      : integer;
                  var Solution     : TNvector;
                  var Iter         : integer;
                  var Error        : byte);

{----------------------------------------------------------------------------}
{-                                                                          -}
{-                Input: Dimen, Coefficients, Constants, Tol, MaxIter       -}
{-               Output: Solution, Iter, Error                              -}
{-                                                                          -}
{-             Purpose : Calculate the solution of a linear set of          -}
{-                       equations using Gauss - Seidel iteration.          -}
{-                                                                          -}
{-  User-defined Types : TNvector = array[1..TNArraySize] of real           -}
{-                       TNmatrix = array[1..TNArraySize] of TNvector       -}
{-                                                                          -}
{-    Global Variables : Dimen : integer;         Dimen of the square       -}
{-                                                matrix                    -}
{-                       Coefficients : TNmatrix; Square matrix             -}
{-                       Constants    : TNvector; Constants of each equation-}
{-                       Tol          : real;     Tolerance in answer       -}
{-                       MaxIter      : integer;  Maximum number of         -}
{-                                                iterations allowed        -}
{-                       Solution     : TNvector; Unique solution to the    -}
{-                                                set of equations          -}
{-                       Iter         : integer;  Number of iterations      -}
{-                       Error        : integer;  Flags if something goes   -}
{-                                                wrong.                    -}
{-                                                                          -}
{-              Errors : 0: No errors;                                      -}
{-                       1: Iter >= MaxIter and matrix not                  -}
{-                          diagonally dominant                             -}
{-                       2: Iter >= MaxIter                                 -}
{-                       3: Dimen < 1                                       -}
{-                       4: Tol <= 0                                        -}
{-                       5: MaxIter < 0                                     -}
{-                       6: Zero on the diagonal of                         -}
{-                          the Coefficients matrix                         -}
{-                       7: Diverging                                       -}
{-                                                                          -}
{-                Note:  If the Gauss-Seidel iterative method is            -}
{-                       applied to an underdetermined system of equations  -}
{-                       (i.e. one of the equations is a linear             -}
{-                       superposition of the others) it will converge      -}
{-                       to a (non-unique) solution. The Gauss-Seidel       -}
{-                       method is unable to distinguish between unique     -}
{-                       and non-unique solutions.                          -}
{-                       If you are concerned that  your equations          -}
{-                       may be underdetermined, solve them with            -}
{-                       Gaussian elimination (GAUSELIM.INC or              -}
{-                       PARTPIVT.INC                                       -}
{-                                                                          -}
{----------------------------------------------------------------------------}

implementation

{$I Matrix.inc}

end. { Matrix }
