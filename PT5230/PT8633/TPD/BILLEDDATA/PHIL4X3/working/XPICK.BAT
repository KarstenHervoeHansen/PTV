REM Generating test pattern Philips 4x3 for PT8603
REM --------------------------------------------
REM
REM Author:         J›rgen Parbst
REM
REM First created:  01/10/97
REM Editeret til, udvidet felt til bev‘gelig pind m. lydmarkeringer. HCM 9810
REM --------------------------------------------
REM Modified by JK to run without RAM drivr on D:
REM on 04.06.2003
ECHO Preparing system files for data storage.
ECHO ----------------------------------------
 ECHO Storing setup values !
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
ECHO Storing Philips 4X3 test pattern******* CALLING Z_PHIL.BAT ********.
 CALL z_PHIL.BAT
 CALL makbmp.BAT
