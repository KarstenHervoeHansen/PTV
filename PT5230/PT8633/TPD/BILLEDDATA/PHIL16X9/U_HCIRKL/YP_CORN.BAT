ECHO OFF
REM Generating the corner circles for the Philips test pattern.
REM -----------------------------------------------------------
REM
REM Author:         Mikael Nadelmann
REM
REM First created:  04/06/93
REM
REM New update:     04/06/93  Adding upper left circle.
REM New update:     07/06/93  Adding more of upper left circle.
REM New update:     10/10/97  Adding PT8603
REM
REM -----------------------------------------------------------

IF "%1"=="" GOTO LABEL1
SET C= %1
:LABEL1
IF "%2"=="" GOTO LABEL2
SET C=%C% %2
:LABEL2



ECHO Generating picture in upper left corner !
ECHO -----------------------------------------
ECHO.

 ECHO Adding wedges !
 ECHO.
  square FL=34.0 LL=165.0 FS=173.181 LS=264.529 A1=0%C%
  hwedge CY=99.5 FS=175.485 LS=218.741 FVR=150 LVR=350 WOW=5 A1=700%C%
  hwedge CY=99.5 FS=218.741 LS=261.997 FVR=450 LVR=250 WOW=7 A1=700%C%
  vwedge CX=218.741 FL=36.5 LL=99.5 FHR=150 LHR=350 WOW=5 A1=700%C%
  vwedge CX=218.741 FL=99.5 LL=162.5 FHR=450 LHR=250 WOW=7 A1=700%C%
  ocircle CX=218.741 CY=99.5 FL=34.0 LL=165.0 FS=173.181 LS=264.529 RAD=63.000 A1=336%C%
  icircle CX=218.741 CY=99.5 RAD=21.000 A1=0%C%

 ECHO Adding circle markers !
 ECHO.
  circle CX=218.741 CY=99.5 W=210 RAD=35.000 FA=286.350 LA=343.650 A1=700%C%
  circle CX=218.741 CY=99.5 W=210 RAD=49.000 FA=106.671 LA=163.033 A1=700%C%

 ECHO Adding dot markers !
 ECHO.
  line FL=50.0 LL=51.0 FS=218.741 LS=218.741 W=200 A1=0%C%
  line FL=64.0 LL=65.0 FS=218.741 LS=218.741 W=200 A1=0%C%
  line FL=134.0 LL=135.0 FS=218.741 LS=218.741 W=200 A1=0%C%
  line FL=148.0 LL=149.0 FS=218.741 LS=218.741 W=200 A1=0%C%
  line FL=98.0 LL=101.0 FS=194.710 LS=194.710 W=200 A1=0%C%
  line FL=98.0 LL=101.0 FS=242.772 LS=242.772 W=200 A1=0%C%
  line FL=98.0 LL=101.0 FS=185.098 LS=185.098 W=200 A1=0%C%
  line FL=98.0 LL=101.0 FS=252.385 LS=252.385 W=200 A1=0%C%

 ECHO Adding numbers !
 ECHO.
  square FL=59.0 LL=70.0 FS=187.644 LS=202.869 A1=0%C%
  number CX=191.075 CY=64.9 W=180 NC=2 A1=700%C%
  number CX=199.200 CY=64.9 W=180 NC=0 A1=700%C%
  square FL=119.0 LL=130.0 FS=227.609 LS=242.833 A1=0%C%
  number CX=232.246 CY=124.2 W=180 NC=3 A1=700%C%
  number CX=239.990 CY=124.2 W=180 NC=5 A1=700%C%

 ECHO Adding circles !
 ECHO.
  circle CX=218.741 CY=99.5 W=200 RAD=63.000 A1=700%C%
  circle CX=218.741 CY=99.5 W=118.306 RAD=13.333 A1=700%C%
  circle CX=218.741 CY=99.5 W=118.306 RAD=17.167 A1=700%C%
  circle CX=218.741 CY=99.5 W=118.306 RAD=21.000 A1=700%C%



ECHO Generating picture in upper right corner !
ECHO ------------------------------------------
ECHO.

 ECHO Adding wedges !
 ECHO.
  square FL=34.0 LL=165.0 FS=723.503 LS=810.015 A1=0%C%
  hwedge CY=99.5 FS=723.503 LS=766.759 FVR=250 LVR=450 WOW=7 A1=700%C%
  hwedge CY=99.5 FS=766.759 LS=810.015 FVR=350 LVR=150 WOW=5 A1=700%C%
  vwedge CX=766.759 FL=36.5 LL=99.5 FHR=150 LHR=350 WOW=5 A1=700%C%
  vwedge CX=766.759 FL=99.5 LL=162.5 FHR=450 LHR=250 WOW=7 A1=700%C%
  ocircle CX=766.759 CY=99.5 FL=34.0 LL=165.0 FS=723.172 LS=810.333 RAD=63.000 A1=336%C%
  icircle CX=766.759 CY=99.5 RAD=21.000 A1=0%C%

 ECHO Adding circle markers !
 ECHO.
  circle CX=766.759 CY=99.5 W=210 RAD=49.000 FA=16.350 LA=73.650 A1=700%C%
  circle CX=766.759 CY=99.5 W=210 RAD=35.000 FA=196.967 LA=253.033 A1=700%C%

 ECHO Adding dot markers !
 ECHO.
  line FL=50.0 LL=51.0 FS=766.759 LS=766.759 W=200 A1=0%C%
  line FL=64.0 LL=65.0 FS=766.759 LS=766.759 W=200 A1=0%C%
  line FL=134.0 LL=135.0 FS=766.759 LS=766.759 W=200 A1=0%C%
  line FL=148.0 LL=149.0 FS=766.759 LS=766.759 W=200 A1=0%C%
  line FL=98.0 LL=101.0 FS=733.115 LS=733.115 W=200 A1=0%C%
  line FL=98.0 LL=101.0 FS=742.762 LS=742.762 W=200 A1=0%C%
  line FL=98.0 LL=101.0 FS=790.879 LS=790.879 W=200 A1=0%C%
  line FL=98.0 LL=101.0 FS=800.402 LS=800.402 W=200 A1=0%C%

 ECHO Adding numbers !
 ECHO.
 square FL=60.0 LL=71.0 FS=782.929 LS=797.011 A1=0%C%
 number CX=785.539 CY=65.9 W=180 NC=2 A1=700%C%
 number CX=794.045 CY=65.9 W=180 NC=0 A1=700%C%
 square FL=119.0 LL=130.0 FS=742.583 LS=756.666 A1=0%C%
 number CX=745.510 CY=124.2 W=180 NC=3 A1=700%C%
 number CX=753.635 CY=124.2 W=180 NC=5 A1=700%C%

 ECHO Adding circles !
 ECHO.
  circle CX=766.759 CY=99.5 W=200 RAD=63.000 A1=700%C%
  circle CX=766.759 CY=99.5 W=118.306 RAD=13.333 A1=700%C%
  circle CX=766.759 CY=99.5 W=118.306 RAD=17.167 A1=700%C%
  circle CX=766.759 CY=99.5 W=118.306 RAD=21.000 A1=700%C%


ECHO Generating picture in lower left corner !
ECHO -----------------------------------------
ECHO.

 ECHO Adding wedges !
 ECHO.
  square FL=412.0 LL=543.0 FS=173.181 LS=264.529 A1=0%C%
  hwedge CY=477.5 FS=175.485 LS=218.741 FVR=150 LVR=350 WOW=5 A1=700%C%
  hwedge CY=477.5 FS=218.741 LS=261.997 FVR=450 LVR=250 WOW=7 A1=700%C%
  vwedge CX=218.741 FL=414.5 LL=477.5 FHR=250 LHR=450 WOW=7 A1=700%C%
  vwedge CX=218.741 FL=477.5 LL=540.5 FHR=350 LHR=150 WOW=5 A1=700%C%
  ocircle CX=218.741 CY=477.5 FL=412.0 LL=543.0 FS=173.181 LS=264.529 RAD=63.000 A1=336%C%
  icircle CX=218.741 CY=477.5 RAD=21.000 A1=0%C%

 ECHO Adding circle markers !
 ECHO.
  circle CX=218.741 CY=477.5 W=210 RAD=49.000 FA=196.350 LA=253.650 A1=700%C%
  circle CX=218.741 CY=477.5 W=210 RAD=35.000 FA=16.967 LA=73.033 A1=700%C%

 ECHO Adding dot markers !
 ECHO.
  line FL=428.0 LL=429.0 FS=218.741 LS=218.741 W=200 A1=0%C%
  line FL=442.0 LL=443.0 FS=218.741 LS=218.741 W=200 A1=0%C%
  line FL=512.0 LL=513.0 FS=218.741 LS=218.741 W=200 A1=0%C%
  line FL=526.0 LL=527.0 FS=218.741 LS=218.741 W=200 A1=0%C%
  line FL=476.0 LL=479.0 FS=194.710 LS=194.710 W=200 A1=0%C%
  line FL=476.0 LL=479.0 FS=242.772 LS=242.771 W=200 A1=0%C%
  line FL=476.0 LL=479.0 FS=185.098 LS=185.098 W=200 A1=0%C%
  line FL=476.0 LL=479.0 FS=252.385 LS=252.385 W=200 A1=0%C%

 ECHO Adding numbers !
 ECHO.
  square FL=507.0 LL=518.0 FS=188.786 LS=202.869 A1=0%C%
  number CX=192.217 CY=512.1 W=180 NC=2 A1=700%C%
  number CX=200.349 CY=512.1 W=180 NC=0 A1=700%C%
  square FL=447.0 LL=458.0 FS=228.370 LS=243.214 A1=0%C%
  number CX=231.865 CY=452.8 W=180 NC=3 A1=700%C%
  number CX=239.990 CY=452.8 W=180 NC=5 A1=700%C%

 ECHO Adding circles !
 ECHO.
  circle CX=218.741 CY=477.5 W=200 RAD=63.000 A1=700%C%
  circle CX=218.741 CY=477.5 W=118.306 RAD=13.333 A1=700%C%
  circle CX=218.741 CY=477.5 W=118.306 RAD=17.167 A1=700%C%
  circle CX=218.741 CY=477.5 W=118.306 RAD=21.000 A1=700%C%



ECHO Generating picture in lower right corner !
ECHO ------------------------------------------
ECHO.

 ECHO Adding wedges !
 ECHO.
  square FL=412.0 LL=543.0 FS=723.503 LS=810.015 A1=0%C%
  hwedge CY=477.5 FS=723.503 LS=766.759 FVR=250 LVR=450 WOW=7 A1=700%C%
  hwedge CY=477.5 FS=766.759 LS=810.015 FVR=350 LVR=150 WOW=5 A1=700%C%
  vwedge CX=766.759 FL=414.5 LL=477.5 FHR=250 LHR=450 WOW=7 A1=700%C%
  vwedge CX=766.759 FL=477.5 LL=540.5 FHR=350 LHR=150 WOW=5 A1=700%C%
  ocircle CX=766.759 CY=477.5 FL=412.0 LL=543.0 FS=723.172 LS=810.333 RAD=63.000 A1=336%C%
  icircle CX=766.759 CY=477.5 RAD=21.000 A1=0%C%

 ECHO Adding circle markers !
 ECHO.
  circle CX=766.759 CY=477.5 W=210 RAD=49.000 FA=286.350 LA=343.650 A1=700%C%
  circle CX=766.759 CY=477.5 W=210 RAD=35.000 FA=106.967 LA=163.033 A1=700%C%

 ECHO Adding dot markers !
 ECHO.
  line FL=428.0 LL=429.0 FS=766.759 LS=766.759 W=200 A1=0%C%
  line FL=442.0 LL=443.0 FS=766.759 LS=766.759 W=200 A1=0%C%
  line FL=512.0 LL=513.0 FS=766.759 LS=766.759 W=200 A1=0%C%
  line FL=526.0 LL=527.0 FS=766.759 LS=766.759 W=200 A1=0%C%
  line FL=476.0 LL=479.0 FS=733.115 LS=733.115 W=200 A1=0%C%
  line FL=476.0 LL=479.0 FS=742.762 LS=742.762 W=200 A1=0%C%
  line FL=476.0 LL=479.0 FS=790.879 LS=790.879 W=200 A1=0%C%
  line FL=476.0 LL=479.0 FS=800.402 LS=800.402 W=200 A1=0%C%

 ECHO Adding numbers !
 ECHO.
 square FL=507.0 LL=518.0 FS=783.309 LS=797.011 A1=0%C%
 number CX=785.920 CY=512.1 W=180 NC=2 A1=700%C%
 number CX=794.045 CY=512.1 W=180 NC=0 A1=700%C%
 square FL=447.0 LL=458.0 FS=742.964 LS=757.047 A1=0%C%
 number CX=745.891 CY=452.8 W=180 NC=3 A1=700%C%
 number CX=754.015 CY=452.8 W=180 NC=5 A1=700%C%

 ECHO Adding circles !
 ECHO.
  circle CX=766.759 CY=477.5 W=200 RAD=63.000 A1=700%C%
  circle CX=766.759 CY=477.5 W=118.306 RAD=13.333 A1=700%C%
  circle CX=766.759 CY=477.5 W=118.306 RAD=17.167 A1=700%C%
  circle CX=766.759 CY=477.5 W=118.306 RAD=21.000 A1=700%C%
                                                               	
