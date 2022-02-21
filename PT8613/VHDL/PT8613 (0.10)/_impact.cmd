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
addConfigDevice -size 0 -name "xcf08p" -path "\\wts\data1\PT8613\VHDL\SDHD_module_v2/"
setAttribute -configdevice -attr size -value "0"
setAttribute -configdevice -attr reseveSize -value "0"
setSubmode -pffserial
setAttribute -configdevice -attr activeCollection -value "PT8613_top"
addCollection -name "PT8613_top"
addDesign -version 0 -name "0"
addDeviceChain -index 0
addDevice -position 1 -file "F:\PT8613\VHDL\SDHD_module_v2\hd_gen_module.bit"
setMode -pff
setMode -bs
setMode -ss
setMode -sm
setMode -bsfile
setMode -dtconfig
setMode -cf
setMode -mpm
setMode -pff
setMode -pff
setMode -pff
setMode -pff
addConfigDevice -size 1024 -name "xcf08p" -path "\\wts\data1\pt8613\vhdl\sdhd_module_v2_ibc2006/"
setSubmode -pffserial
addPromDevice -position 1 -size -1 -name "xcf08p"
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr size -value "0"
addCollection -name "pt8613_vcxo"
setAttribute -collection -attr dir -value "UP"
addDesign -version 0 -name "0000"
addDeviceChain -index 0
addDevice -position 1 -file "F:\PT8613\VHDL\SDHD_module_v2_IBC2006\hd_gen_module.bit"
setMode -pff
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -collection -attr dir -value "UP"
setAttribute -configdevice -attr path -value "\\wts\data1\pt8613\vhdl\sdhd_module_v2_ibc2006/"
setAttribute -collection -attr name -value "pt8613_vcxo"
generate
setCurrentDesign -version 0
