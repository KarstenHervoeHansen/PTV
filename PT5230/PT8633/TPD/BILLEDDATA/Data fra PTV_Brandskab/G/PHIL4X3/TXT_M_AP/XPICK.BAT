rem SUBST D: e:\
@ECHO OFF
REM Generating PT8601 test pattern Philips 4X3
REM --------------------------------------------
REM
REM Author:         J›rgen Parbst
REM
REM First created:  26/09/97
REM
REM --------------------------------------------



 ECHO Listing line list !
 ECHO.
 REM Number 1 to 576 is PAL-G 4:3 Philips test pattern.

CLS
ECHO Preparing system files for data storage.
ECHO ----------------------------------------
ECHO.

 ECHO Storing setup values !
 ECHO.
 dataset InnerFirstSample=1025
 dataset InnerLastSample=1025
 dataset LineLastSample=1024
 dataset BlockSize=1024
 dataset LineOffset=1
 dataset SampleOffset1=10.25
 dataset SampleOffset2=10.25
 dataset SampleOffset3=10.25
 dataset DacOffset1=3072
 dataset DacOffset2=512
 dataset DacOffset3=512
 dataset DacResolution1=-0.3125
 dataset DacResolution2=-1.25
 dataset DacResolution3=-1.25
 dataset RoundoffNumber1=1
 dataset RoundoffNumber2=1
 dataset RoundoffNumber3=1
 dataset Frequency=13.5
 dataset NumberofActiveLines=575
 dataset NumberofActiveSamples=702
 dataset Channel4SubsamplingRate=1



CLS
ECHO Storing Philips 4:3 test pattern.
ECHO ----------------------------------
ECHO.

 CALL Z_PHIL.BAT PBFL:1 PBLL:74
 CALL Z_PHIL.BAT PBFL:75 PBLL:149
 CALL Z_PHIL.BAT PBFL:150 PBLL:224
 CALL Z_PHIL.BAT PBFL:225 PBLL:299
 CALL Z_PHIL.BAT PBFL:300 PBLL:374
 CALL Z_PHIL.BAT PBFL:375 PBLL:449
 CALL Z_PHIL.BAT PBFL:450 PBLL:524
 CALL Z_PHIL.BAT PBFL:525 PBLL:576

rem SUBST D: /D
