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
addDevice -position 1 -file "F:\PT8612\Xilinx\vhdl\HD_Module\hd_gen_module.bit"

setMode -sm
setMode -ss
setMode -bs
setMode -bsfile
setMode -bs
setMode -ss
setMode -sm
setMode -bsfile
setMode -dtconfig
setMode -cf
setMode -mpm
setMode -pff
setMode -bsfile
setMode -bs
setCable -port auto
Identify
addDevice -position 2 -file "F:\PT8612\Xilinx\vhdl\HD_Module\hd_gen_module.bit"
deleteDevice -position 3
Program -p 2 -v 
