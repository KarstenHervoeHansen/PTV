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
setMode -pff
setMode -pff
addConfigDevice -size 256 -name "xcf02s" -path "f:\pt8611\xilinx\fpga_version\v5_debug\genlock_monitoring/"
setSubmode -pffserial
addPromDevice -position 1 -size -1 -name "xcf02s"
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr size -value "0"
addCollection -name "genmon"
setAttribute -collection -attr dir -value "UP"
addDesign -version 0 -name "0000"
addDeviceChain -index 0
addDevice -position 1 -file "F:\PT8611\Xilinx\fpga_version\v5_debug\Genlock_monitoring\tri_level_module.bit"
setMode -pff
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -collection -attr dir -value "UP"
setAttribute -configdevice -attr path -value "f:\pt8611\xilinx\fpga_version\v5_debug\genlock_monitoring/"
setAttribute -collection -attr name -value "genmon"
generate
setCurrentDesign -version 0
setMode -bs
setCable -port auto
Identify
Identify
setAttribute -position 1 -attr devicePartName -value "xcf01s"
setAttribute -position 1 -attr configFileName -value "F:\PT8611\Xilinx\fpga_version\v5_debug\Genlock_monitoring\genmon.mcs"
Program -p 1 -e -v 
