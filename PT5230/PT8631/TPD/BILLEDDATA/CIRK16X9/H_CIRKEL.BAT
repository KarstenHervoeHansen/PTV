
	@ECHO OFF
	REM Generating 16X9CIRKEL test pattern for the PT5210-30 PAL-G version
	REM ------------------------------------------------------------
	REM
	REM Author:         H-C M›lstrand
	REM
	REM First created:  980914
	REM
	REM 
	REM 
	REM ------------------------------------------------------------

	IF "%1"=="" GOTO LABEL1
	SET C= %1
	:LABEL1
	IF "%2"=="" GOTO LABEL2
	SET C=%C% %2
	:LABEL2


	CLS
	 ECHO --- 16X9CIRKEL TEST PATTERN ---
	 ECHO --- Processing line %1 to %2 ---
	 ECHO.

	 ECHO Listing lines !
	 ECHO.
	 REM ---------------------------------------------------------------
	 REM ------------------------ FIELD 1 AND 2 ------------------------
	 REM ---------------------------------------------------------------
	 REM Number  1 is line  23 field 1 (half line).
	 REM Number  2 is line 336 field 2 of active picture.
	 REM Number  3 is line  24 field 1 of active picture.
	 REM Number  4 is line 337 field 2 of active picture.
	 REM Number  5 is line  25 field 1 of active picture.
	 REM          .
	 REM          .
	 REM          .
	 REM Number  572 is line 621 field 2 of active picture.
	 REM Number  573 is line 309 field 1 of active picture.
	 REM Number  574 is line 622 field 2 of active picture (no burst).
	 REM Number  575 is line 310 field 1 of active picture (no burst).
	 REM Number  576 is line 623 field 2 (half line).
	 REM ---------------------------------------------------------------
	 



 ECHO --- INITIALIZING BUFFERS ---
 ECHO.
 ECHO Storing aspect ratio !
 ECHO.
  dataset AspectRatio=1.7777777777777778

 ECHO.
 ECHO Erasing buffer files !
 ECHO.
  square FL=1.0 LL=576.0 FS=1.000 LS=1024.000 A1=0 A2=0 A3=0%C%

 ECHO Adding cirkel !
 ECHO.
  circle CX=492.750 CY=287.5 RAD=252.000 W=200 A1=700%C%
  circle CX=492.750 CY=287.5 RAD=252.000 W=200 A2=0%C%
  circle CX=492.750 CY=287.5 RAD=252.000 W=200 A3=0%C%



 ECHO --- STORING FIELD 1 AND 2 ---
 ECHO.
 ECHO Storing buffers in channels !
 ECHO.
  convert FL=1.0 LL=576.0 FS=1.000 LS=1024.000 C=1%C%
  convert FL=1.0 LL=576.0 FS=1.000 LS=1024.000 C=2%C%
  convert FL=1.0 LL=576.0 FS=1.000 LS=1024.000 C=3%C%


