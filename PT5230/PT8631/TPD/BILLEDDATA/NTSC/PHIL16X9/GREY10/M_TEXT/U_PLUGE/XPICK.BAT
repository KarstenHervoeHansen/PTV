rem SUBST D: e:\
@ECHO OFF
REM Generating test pattern Philips 4x3 for PT8631NTSC
REM --------------------------------------------
REM
REM Author:         J›rgen Parbst
REM
REM First created:  01/10/97
REM Modified for NTSC: 990406 HCM
REM --------------------------------------------



 ECHO Listing line list !
 ECHO.
 REM Number 1 to 486 is NTSC 4X3 Philips test pattern.

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
 dataset SampleOffset1=10.672
 dataset SampleOffset2=10.672
 dataset SampleOffset3=10.672
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
 dataset NumberofActiveLines=485
 dataset NumberofActiveSamples=710
 dataset Channel4SubsamplingRate=1



CLS
ECHO Storing Philips 4X3 test pattern.
ECHO ----------------------------------
ECHO.

 CALL mx_PHIl.BAT PBFL:1 PBLL:74
 CALL mx_PHIl.BAT PBFL:75 PBLL:149
 CALL mx_PHIl.BAT PBFL:150 PBLL:224
 CALL mx_PHIl.BAT PBFL:225 PBLL:299
 CALL mx_PHIl.BAT PBFL:300 PBLL:374
 CALL mx_PHIl.BAT PBFL:375 PBLL:449
 CALL mx_PHIl.BAT PBFL:450 PBLL:486


rem SUBST D: /D
