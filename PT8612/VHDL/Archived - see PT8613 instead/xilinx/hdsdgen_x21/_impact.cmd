setPreference -pref UserLevel:NOVICE
setPreference -pref MessageLevel:DETAILED
setPreference -pref ConcurrentMode:FALSE
setPreference -pref UseHighz:FALSE
setPreference -pref ConfigOnFailure:STOP
setPreference -pref StartupCLock:AUTO_CORRECTION
setPreference -pref AutoSignature:FALSE
setPreference -pref KeepSVF:FALSE
setPreference -pref svfUseTime:FALSE
setPreference -pref UserLevel:NOVICE
setPreference -pref MessageLevel:DETAILED
setPreference -pref ConcurrentMode:FALSE
setPreference -pref UseHighz:FALSE
setPreference -pref ConfigOnFailure:STOP
setPreference -pref StartupCLock:AUTO_CORRECTION
setPreference -pref AutoSignature:FALSE
setPreference -pref KeepSVF:FALSE
setPreference -pref svfUseTime:FALSE
setMode -bs
setPreference -pref UserLevel:Novice
setMode -pff
setMode -mpm
setMode -cf
setMode -dtconfig
setMode -bsfile
setMode -sm
setMode -ss
setMode -bs
addDevice -position 1 -part "xcf08p"
addDevice -position 2 -file "C:\MyProject\8612\hdsdgen_x20\hdvb0.bit"

setCable -port lpt1
setMode -bs
setMode -bs
setMode -ss
setMode -sm
setMode -bsfile
setMode -dtconfig
setMode -cf
setMode -mpm
setMode -pff
setMode -bs
setCable -port auto
Program -p 2 -v 
addDevice -position 2 -file "F:\PT8612\VHDL\xilinx\hdsdgen_x21\hdvb0.bit"
deleteDevice -position 3
Verify -p 2 
Program -p 2 -v 
setCable -port auto
Program -p 2 -v 
addDevice -position 2 -file "C:\MyProject\8612\hdsdgen_x21\hdvb0.bit"
deleteDevice -position 3
Program -p 2 -v 
