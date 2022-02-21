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
addConfigDevice -size 0 -name "xcf08p" -path "F:\pt8612\vhdl\xilinx\sdhd_module_v2/"
setAttribute -configdevice -attr size -value "0"
setAttribute -configdevice -attr reseveSize -value "0"
setSubmode -pffserial
setAttribute -configdevice -attr activeCollection -value "sdhd_lyd_noctrlpkt"
addCollection -name "sdhd_lyd_noctrlpkt"
addDesign -version 0 -name "0"
addDeviceChain -index 0
addDevice -position 1 -file "F:\PT8612\VHDL\xilinx\SDHD_module_v2\hd_gen_module.bit"
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
Identify
