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
setMode -pff
addConfigDevice -size 1 -name "pff" -path "F:\pt8612\vhdl\xilinx\sdhd_module_v2_jitter_test/"
setSubmode -pffserial
addPromDevice -position 1 -size -1 -name "xcf08p"
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr size -value "0"
addCollection -name "top"
setAttribute -collection -attr dir -value "UP"
addDesign -version 0 -name "0000"
addDeviceChain -index 0
addDevice -position 1 -file "F:\pt8612\vhdl\xilinx\sdhd_module_v2_jitter_test\hd_gen_module.bit"
setMode -pff
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -collection -attr dir -value "UP"
setAttribute -configdevice -attr path -value "F:\pt8612\vhdl\xilinx\sdhd_module_v2_jitter_test/"
setAttribute -collection -attr name -value "top"
generate
setCurrentDesign -version 0
setMode -bs
setCable -port auto
Identify
identifyMPM
setAttribute -position 1 -attr devicePartName -value "xcf08p"
setAttribute -position 1 -attr configFileName -value "F:\PT8612\VHDL\xilinx\SDHD_module_v2_jitter_test\top.mcs"
Program -p 1 -e -v 
quit
