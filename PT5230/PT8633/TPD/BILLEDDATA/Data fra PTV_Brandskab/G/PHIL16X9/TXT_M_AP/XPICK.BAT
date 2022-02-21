rem SUBST D: e:\
@ECHO OFF
REM Generating test pattern Philips 16x9 for PT8603
REM --------------------------------------------
REM
REM Author:         J›rgen Parbst
REM
REM First created:  01/10/97
REM
REM --------------------------------------------



 ECHO Listing line list !
 ECHO.
 REM Number 1 to 576 is PAL-G 16X9 Philips test pattern.

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
 dataset DacOffset1=64
 dataset DacOffset2=512
 dataset DacOffset3=512
 dataset DacResolution1=0.799086758
 dataset DacResolution2=0.682493230
 dataset DacResolution3=0.960592647
 dataset RoundoffNumber1=1
 dataset RoundoffNumber2=1
 dataset RoundoffNumber3=1
 dataset Frequency=13.5
 dataset NumberofActiveLines=575
 dataset NumberofActiveSamples=702
 dataset Channel4SubsamplingRate=1



CLS
ECHO Storing Philips 4X3 test pattern.
ECHO ----------------------------------
ECHO.

 CALL YP_PHIL.BAT PBFL:1 PBLL:74
 CALL YP_PHIL.BAT PBFL:75 PBLL:149
 CALL YP_PHIL.BAT PBFL:150 PBLL:224
 CALL YP_PHIL.BAT PBFL:225 PBLL:310
 CALL YP_PHIL.BAT PBFL:300 PBLL:374
 CALL YP_PHIL.BAT PBFL:375 PBLL:449
 CALL YP_PHIL.BAT PBFL:450 PBLL:524
 CALL YP_PHIL.BAT PBFL:525 PBLL:576

rem SUBST D: /D
