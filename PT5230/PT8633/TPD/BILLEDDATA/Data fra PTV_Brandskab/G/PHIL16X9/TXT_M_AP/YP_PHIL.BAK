	@ECHO OFF
	REM Generating Philips 16:9 test pattern for the PT8603 PAL-G version
	REM ------------------------------------------------------------
	REM
	REM Author:         J›rgen Parbst
	REM
	REM First created:  30/08/93
	REM
	REM New update:     13/01/94 Adding pattern.
	REM New update:     10/10/97 Adding DATA FOR PT8603
	REM
	REM ------------------------------------------------------------

	IF "%1"=="" GOTO LABEL1
	SET C= %1
	:LABEL1
	IF "%2"=="" GOTO LABEL2
	SET C=%C% %2
	:LABEL2



	CLS
	 ECHO --- PHILIPS 16:9 TEST PATTERN ---
	 ECHO --- Processing line %1 to %2 ---
	 ECHO.

	 ECHO Listing lines !
	 ECHO.  	
	 REM ---------------------------------------------------------------
	 REM ------------------------ FIELD 1 AND 2 ------------------------
	 REM ---------------------------------------------------------------
	 REM Number 2700 is line  23 field 1 (half line).
	 REM Number 2701 is line 336 field 2 of active picture.
	 REM Number 2702 is line  24 field 1 of active picture.
	 REM Number 2703 is line 337 field 2 of active picture.
	 REM Number 2704 is line  25 field 1 of active picture.
	 REM          .
	 REM          .
	 REM          .
	 REM Number 2968 is line 157 field 1 of active picture (NO date, NO clock).
	 REM Number 2969 is line 470 field 2 of active picture (NO date, NO clock).
	 REM Number 2970 is line 158 field 1 of active picture (NO date, NO clock).
	 REM          .
	 REM Number 3005 is line 488 field 2 of active picture (NO date, NO clock).
	 REM Number 3006 is line 176 field 1 of active picture (NO date, NO clock).
	 REM Number 3007 is line 489 field 2 of active picture (NO date, NO clock).
	 REM          .
	 REM          .
	 REM          .
	 REM Number 3271 is line 621 field 2 of active picture.
	 REM Number 3272 is line 309 field 1 of active picture.
	 REM Number 3273 is line 622 field 2 of active picture (no burst).
	 REM Number 3274 is line 310 field 1 of active picture (no burst).
	 REM Number 3275 is line 623 field 2 (half line).
	 REM ---------------------------------------------------------------



 ECHO --- INITIALIZING BUFFERS ---
 ECHO.
 ECHO Storing aspect ratio !
 ECHO.
  dataset AspectRatio=1.7777777777777778

 ECHO Erasing buffer files !
 ECHO.
  square FL=1.0 LL=576.0 FS=1 LS=1024 A1=0 A2=0 A3=0%C%

 ECHO --- GENERATING ELEMENTS OUTSIDE CIRCLE ---
 ECHO.
 ECHO Adding border castellation !
 ECHO.
  square FL=2.0 LL=14.0 FS=141.750 LS=161.055 A1=700 FR=200%C%
  square FL=2.0 LL=14.0 FS=189.898 LS=218.741 A1=700%C%
  square FL=2.0 LL=14.0 FS=247.584 LS=276.427 A1=700%C%
  square FL=2.0 LL=14.0 FS=305.270 LS=334.113 A1=700%C%
  square FL=2.0 LL=14.0 FS=362.957 LS=391.799 A1=700%C%
  square FL=2.0 LL=14.0 FS=420.642 LS=449.486 A1=700%C%
  square FL=2.0 LL=14.0 FS=478.329 LS=507.171 A1=700%C%
  square FL=2.0 LL=14.0 FS=536.015 LS=564.858 A1=700%C%
  square FL=1.0 LL=14.0 FS=593.701 LS=622.544 A1=700%C%
  square FL=1.0 LL=14.0 FS=651.387 LS=680.230 A1=700%C%
  square FL=1.0 LL=14.0 FS=709.073 LS=737.902 A1=700%C%
  square FL=1.0 LL=14.0 FS=766.759 LS=795.602 A1=700%C%
  square FL=1.0 LL=14.0 FS=824.445 LS=843.750 A1=700 LR=200%C%
  square FL=59.0 LL=98.0 FS=141.750 LS=161.055 A1=700 FR=200%C%
  square FL=59.0 LL=98.0 FS=824.445 LS=843.750 A1=700 LR=200%C%
  square FL=143.0 LL=182.0 FS=141.750 LS=161.055 A1=700 FR=200%C%
  square FL=143.0 LL=182.0 FS=824.445 LS=843.750 A1=700 LR=200%C%
  square FL=227.0 LL=266.0 FS=141.750 LS=161.055 A1=700 FR=200%C%
  square FL=227.0 LL=266.0 FS=824.445 LS=843.750 A1=700 LR=200%C%
  square FL=311.0 LL=350.0 FS=141.750 LS=161.055 A1=700 FR=200%C%
  square FL=311.0 LL=350.0 FS=824.445 LS=843.750 A1=700 LR=200%C%
  square FL=395.0 LL=434.0 FS=141.750 LS=161.055 A1=700 FR=200%C%
  square FL=395.0 LL=434.0 FS=824.445 LS=843.750 A1=700 LR=200%C%
  square FL=479.0 LL=518.0 FS=141.750 LS=161.055 A1=700 FR=200%C%
  square FL=479.0 LL=518.0 FS=824.445 LS=843.750 A1=700 LR=200%C%
  square FL=563.0 LL=576.0 FS=141.750 LS=161.055 A1=700 FR=200%C%
  square FL=563.0 LL=576.0 FS=189.898 LS=218.741 A1=700%C%
  square FL=563.0 LL=576.0 FS=247.584 LS=276.427 A1=700%C%
  square FL=563.0 LL=576.0 FS=305.270 LS=334.113 A1=700%C%
  square FL=563.0 LL=576.0 FS=362.957 LS=391.799 A1=700%C%
  square FL=563.0 LL=575.0 FS=420.642 LS=449.486 A1=700%C%
  square FL=563.0 LL=575.0 FS=478.329 LS=507.171 A1=700%C%
  square FL=563.0 LL=575.0 FS=536.015 LS=564.858 A1=700%C%
  square FL=563.0 LL=575.0 FS=593.701 LS=622.544 A1=700%C%
  square FL=563.0 LL=575.0 FS=651.387 LS=680.230 A1=700%C%
  square FL=563.0 LL=575.0 FS=709.073 LS=737.902 A1=700%C%
  square FL=563.0 LL=575.0 FS=766.759 LS=795.602 A1=700%C%
  square FL=563.0 LL=575.0 FS=824.445 LS=843.750 A1=700 LR=200%C%


 ECHO Adding background !
 ECHO.
  square FL=17.0 LL=560.0 FS=161.055 LS=824.445 A1=336%C%

 ECHO Adding R-Y, B-Y bars !
 ECHO.
  square FL=59.0 LL=140.0 FS=276.427 LS=312.106 A2=0 A3=-165%C%
  square FL=59.0 LL=140.0 FS=305.270 LS=334.113 A2=136.792 A3=-92.267 FR=275%C%
  square FL=59.0 LL=140.0 FS=651.387 LS=685.110 A2=136.792 A3=-92.267%C%
  square FL=59.0 LL=140.0 FS=680.230 LS=709.073 A2=-165 A3=0 FR=275%C%
  square FL=141.0 LL=288.0 FS=276.427 LS=305.270 A2=0 A3=-165%C%
  square FL=141.0 LL=288.0 FS=680.230 LS=709.073 A2=-165 A3=0%C%
  square FL=289.0 LL=436.0 FS=276.427 LS=305.270 A2=0 A3=165%C%
  square FL=289.0 LL=436.0 FS=680.230 LS=709.073 A2=165 A3=0%C%
  square FL=437.0 LL=518.0 FS=276.427 LS=312.106 A2=0 A3=165%C%
  square FL=437.0 LL=518.0 FS=305.270 LS=334.113 A2=-136.792 A3=92.267 FR=275%C%
  square FL=437.0 LL=518.0 FS=651.383 LS=685.110 A2=-136.792 A3=92.267%C%
  square FL=437.0 LL=518.0 FS=680.230 LS=709.073 A2=165 A3=0 FR=275%C%

 ECHO Adding ›vre venstre dirg. feldt !
 ECHO.
  square FL=183.5 LL=267.5 FS=218.741 LS=276.427 A1=0%C%
  dialine FL=183.5 LL=267.5 FS=218.741 LS=276.427 RES=300 S=-1 A1=700%C%

 ECHO Adding ›vre h›jre dirg. feldt !
 ECHO.
  square FL=183.5 LL=267.5 FS=709.073 LS=766.759 A1=0%C%
  dialine FL=183.5 LL=267.5 FS=709.073 LS=766.759 RES=300 S=1 A1=700%C%

 ECHO Adding nedre venstre dirg. feldt !
 ECHO.
  square FL=309.5 LL=393.5 FS=218.741 LS=276.427 A1=0%C%
  dialine FL=309.5 LL=393.5 FS=218.741 LS=276.427 RES=200 S=1 A1=700%C%

 ECHO Adding nedre h›jre dirg. feldt !
 ECHO.
  square FL=309.5 LL=393.5 FS=709.073 LS=766.759 A1=0%C%
  dialine FL=309.5 LL=393.5 FS=709.073 LS=766.759 RES=200 S=-1 A1=700%C%

 ECHO Adding left vertical sweep !
 ECHO.
  square FL=185.0 LL=392.0 FS=189.898 LS=218.741 A1=0%C%
  vsweep FL=185.0 LL=392.0 FS=189.898 LS=218.741 FRES=75 LRES=300 A1=700%C%

 ECHO Adding right vertical sweep !
 ECHO.
  square FL=185.0 LL=392.0 FS=766.759 LS=795.602 A1=0%C%
  vsweep FL=185.0 LL=392.0 FS=766.759 LS=795.602 FRES=300 LRES=75 A1=700%C%

 CALL yP_CORN.BAT%C%


 ECHO Adding chroma grid !
 ECHO.
  square FL=225.0 LL=226.0 FS=161.055 LS=189.898 A2=0 A3=0%C%
  square FL=225.0 LL=226.0 FS=795.602 LS=824.445 A2=0 A3=0%C%
  pulse FL=203.0 LL=266.0 CX=161.055 W=275 A2=0 A3=0%C%
  pulse FL=203.0 LL=266.0 CX=189.898 W=275 A2=0 A3=0%C%
  pulse FL=59.0 LL=518.0 CX=276.427 W=275 A2=0 A3=0%C%
  pulse FL=141.0 LL=436.0 CX=305.270 W=275 A2=0 A3=0%C%
  pulse FL=59.0 LL=140.0 CX=334.113 W=275 A2=0 A3=0%C%
  pulse FL=437.0 LL=518.0 CX=334.113 W=275 A2=0 A3=0%C%
  pulse FL=59.0 LL=140.0 CX=651.387 W=275 A2=0 A3=0%C%
  pulse FL=437.0 LL=518.0 CX=651.387 W=275 A2=0 A3=0%C%
  pulse FL=141.0 LL=436.0 CX=680.230 W=275 A2=0 A3=0%C%
  pulse FL=59.0 LL=518.0 CX=709.073 W=275 A2=0 A3=0%C%
  pulse FL=203.0 LL=266.0 CX=795.602 W=275 A2=0 A3=0%C%
  pulse FL=203.0 LL=266.0 CX=824.445 W=275 A2=0 A3=0%C%

 ECHO Adding horizontal grid !
 ECHO.
  square FL=15.0 LL=16.0 FS=141.750 LS=843.750 A1=700 FR=200 LR=200%C%
  square FL=57.0 LL=58.0 FS=141.750 LS=186.500 A1=700 FR=200%C%
  square FL=57.0 LL=58.0 FS=250.982 LS=734.518 A1=700%C%
  square FL=57.0 LL=58.0 FS=799.000 LS=843.750 A1=700 LR=200%C%
  square FL=99.0 LL=100.0 FS=141.750 LS=175.476 A1=700 FR=200%C%
  square FL=99.0 LL=100.0 FS=262.006 LS=276.427 A1=700%C%
  square FL=99.0 LL=100.0 FS=334.113 LS=651.387 A1=700%C%
  square FL=99.0 LL=100.0 FS=709.073 LS=723.494 A1=700%C%
  square FL=99.0 LL=100.0 FS=810.024 LS=843.750 A1=700 LR=200%C%
  square FL=141.0 LL=142.0 FS=141.750 LS=186.500 A1=700 FR=200%C%
  square FL=141.0 LL=142.0 FS=250.982 LS=276.427 A1=700%C%
  square FL=141.0 LL=142.0 FS=305.270 LS=680.230 A1=700%C%
  square FL=141.0 LL=142.0 FS=709.073 LS=734.518 A1=700%C%
  square FL=141.0 LL=142.0 FS=799.000 LS=843.750 A1=700 LR=200%C%
  square FL=183.0 LL=184.0 FS=141.750 LS=276.427 A1=700 FR=200%C%
  square FL=183.0 LL=184.0 FS=305.270 LS=680.230 A1=700%C%
  square FL=183.0 LL=184.0 FS=709.073 LS=843.750 A1=700 LR=200%C%
  square FL=225.0 LL=226.0 FS=141.750 LS=189.898 A1=700 FR=200%C%
  square FL=225.0 LL=226.0 FS=305.270 LS=680.230 A1=700%C%
  square FL=225.0 LL=226.0 FS=795.602 LS=843.750 A1=700 LR=200%C%
  square FL=267.0 LL=268.0 FS=141.750 LS=189.898 A1=700 FR=200%C%
  square FL=267.0 LL=268.0 FS=218.741 LS=276.427 A1=700%C%
  square FL=267.0 LL=268.0 FS=305.270 LS=680.230 A1=700%C%
  square FL=267.0 LL=268.0 FS=709.073 LS=766.759 A1=700%C%
  square FL=267.0 LL=268.0 FS=795.602 LS=843.750 A1=700 LR=200%C%
  square FL=309.0 LL=310.0 FS=141.750 LS=189.898 A1=700 FR=200%C%
  square FL=309.0 LL=310.0 FS=218.741 LS=276.427 A1=700%C%
  square FL=309.0 LL=310.0 FS=305.270 LS=680.230 A1=700%C%
  square FL=309.0 LL=310.0 FS=709.073 LS=766.759 A1=700%C%
  square FL=309.0 LL=310.0 FS=795.602 LS=843.750 A1=700 LR=200%C%
  square FL=351.0 LL=352.0 FS=141.750 LS=189.898 A1=700 FR=200%C%
  square FL=351.0 LL=352.0 FS=305.270 LS=680.230 A1=700%C%
  square FL=351.0 LL=352.0 FS=795.602 LS=843.750 A1=700 LR=200%C%
  square FL=393.0 LL=394.0 FS=141.750 LS=276.427 A1=700 FR=200%C%
  square FL=393.0 LL=394.0 FS=305.270 LS=680.230 A1=700%C%
  square FL=393.0 LL=394.0 FS=709.073 LS=843.750 A1=700 LR=200%C%
  square FL=435.0 LL=436.0 FS=141.750 LS=186.500 A1=700 FR=200%C%
  square FL=435.0 LL=436.0 FS=250.982 LS=276.427 A1=700%C%
  square FL=435.0 LL=436.0 FS=305.270 LS=680.230 A1=700%C%
  square FL=435.0 LL=436.0 FS=709.073 LS=734.518 A1=700%C%
  square FL=435.0 LL=436.0 FS=799.000 LS=843.750 A1=700 LR=200%C%
  square FL=477.0 LL=478.0 FS=141.750 LS=175.476 A1=700 FR=200%C%
  square FL=477.0 LL=478.0 FS=262.006 LS=276.427 A1=700%C%
  square FL=477.0 LL=478.0 FS=334.113 LS=651.387 A1=700%C%
  square FL=477.0 LL=478.0 FS=709.073 LS=723.494 A1=700%C%
  square FL=477.0 LL=478.0 FS=810.024 LS=843.750 A1=700 LR=200%C%
  square FL=519.0 LL=520.0 FS=141.750 LS=186.500 A1=700 FR=200%C%
  square FL=519.0 LL=520.0 FS=250.982 LS=734.518 A1=700%C%
  square FL=519.0 LL=520.0 FS=799.000 LS=843.750 A1=700 LR=200%C%
  square FL=561.0 LL=562.0 FS=141.750 LS=843.750 A1=700 FR=200 LR=200%C%

 ECHO Adding vertical grid !
 ECHO.
  pulse FL=2.0 LL=576.0 CX=161.055 W=200 A1=700%C%
  pulse FL=2.0 LL=53.0 CX=189.898 W=200 A1=700%C%
  pulse FL=146.0 LL=431.0 CX=189.898 W=200 A1=700%C%
  pulse FL=525.0 LL=576.0 CX=189.898 W=200 A1=700%C%
  pulse FL=2.0 LL=37.0 CX=218.741 W=200 A1=700%C%
  pulse FL=163.0 LL=414.0 CX=218.741 W=200 A1=700%C%
  pulse FL=541.0 LL=576.0 CX=218.741 W=200 A1=700%C%
  pulse FL=2.0 LL=53.0 CX=247.584 W=200 A1=700%C%
  pulse FL=146.0 LL=184.0 CX=247.584 W=200 A1=700%C%
  pulse FL=267.0 LL=310.0 CX=247.584 W=200 A1=700%C%
  pulse FL=393.0 LL=431.0 CX=247.584 W=200 A1=700%C%
  pulse FL=525.0 LL=576.0 CX=247.584 W=200 A1=700%C%
  pulse FL=2.0 LL=576.0 CX=276.427 W=200 A1=700%C%
  pulse FL=2.0 LL=58.0 CX=305.270 W=200 A1=700%C%
  pulse FL=141.0 LL=436.0 CX=305.270 W=200 A1=700%C%
  pulse FL=519.0 LL=576.0 CX=305.270 W=200 A1=700%C%
  pulse FL=2.0 LL=576.0 CX=334.113 W=200 A1=700%C%
  pulse FL=2.0 LL=576.0 CX=362.957 W=200 A1=700%C%
  pulse FL=2.0 LL=576.0 CX=391.799 W=200 A1=700%C%
  pulse FL=2.0 LL=575.0 CX=420.642 W=200 A1=700%C%
  pulse FL=2.0 LL=575.0 CX=449.486 W=200 A1=700%C%
  pulse FL=2.0 LL=575.0 CX=478.329 W=200 A1=700%C%
  pulse FL=2.0 LL=575.0 CX=507.171 W=200 A1=700%C%
  pulse FL=2.0 LL=575.0 CX=536.015 W=200 A1=700%C%
  pulse FL=2.0 LL=575.0 CX=564.858 W=200 A1=700%C%
  pulse FL=1.0 LL=575.0 CX=593.701 W=200 A1=700%C%
  pulse FL=1.0 LL=575.0 CX=622.544 W=200 A1=700%C%
  pulse FL=1.0 LL=575.0 CX=651.387 W=200 A1=700%C%
  pulse FL=1.0 LL=58.0 CX=680.230 W=200 A1=700%C%
  pulse FL=141.0 LL=436.0 CX=680.230 W=200 A1=700%C%
  pulse FL=519.0 LL=575.0 CX=680.230 W=200 A1=700%C%
  pulse FL=1.0 LL=575.0 CX=709.073 W=200 A1=700%C%
  pulse FL=1.0 LL=53.0 CX=737.902 W=200 A1=700%C%
  pulse FL=146.0 LL=184.0 CX=737.902 W=200 A1=700%C%
  pulse FL=267.0 LL=310.0 CX=737.902 W=200 A1=700%C%
  pulse FL=393.0 LL=431.0 CX=737.902 W=200 A1=700%C%
  pulse FL=525.0 LL=575.0 CX=737.902 W=200 A1=700%C%
  pulse FL=1.0 LL=37.0 CX=766.759 W=200 A1=700%C%
  pulse FL=163.0 LL=414.0 CX=766.759 W=200 A1=700%C%
  pulse FL=541.0 LL=575.0 CX=766.759 W=200 A1=700%C%
  pulse FL=1.0 LL=53.0 CX=795.602 W=200 A1=700%C%
  pulse FL=146.0 LL=431.0 CX=795.602 W=200 A1=700%C%
  pulse FL=525.0 LL=575.0 CX=795.602 W=200 A1=700%C%
  pulse FL=1.0 LL=575.0 CX=824.445 W=200 A1=700%C%

 ECHO Adding overscan marks !
 ECHO.
  pulse FL=276.0 LL=301.0 CX=169.830 W=200 A1=700%C%
  pulse FL=276.0 LL=301.0 CX=176.850 W=200 A1=700%C%
  pulse FL=276.0 LL=301.0 CX=183.870 W=200 A1=700%C%
  pulse FL=276.0 LL=301.0 CX=801.630 W=200 A1=700%C%
  pulse FL=276.0 LL=301.0 CX=808.650 W=200 A1=700%C%
  pulse FL=276.0 LL=301.0 CX=815.670 W=200 A1=700%C%

 ECHO --- GENERATING ELEMENTS INSIDE CIRCLE ---
 ECHO.
 CALL yP_CIRCL.BAT%C%

 ECHO Adding horizontal grid !
 ECHO.
  square FL=23.0 LL=24.0 FS=485.539 LS=499.961 A1=700 FR=200 LR=200%C%
  square FL=29.0 LL=30.0 FS=485.539 LS=499.961 A1=700 FR=200 LR=200%C%
  square FL=547.0 LL=548.0 FS=485.539 LS=499.961 A1=700 FR=200 LR=200%C%
  square FL=553.0 LL=554.0 FS=485.539 LS=499.961 A1=700 FR=200 LR=200%C%


 ECHO --- STORING FIELD 1 AND 2 ---
 ECHO.
 ECHO Storing buffers in channels !
 ECHO.
  convert FL=1.0 LL=576.0 FS=1 LS=1024 C=1%C%
  convert FL=1.0 LL=576.0 FS=1 LS=1024 C=2%C%
  convert FL=1.0 LL=576.0 FS=1 LS=1024 C=3%C%


