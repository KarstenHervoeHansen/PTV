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
addConfigDevice -size 1 -name "pff" -path "f:\pt8611\xilinx\fpga_version\v5_debug\trilevelmodule2/"
setSubmode -pffserial
addPromDevice -position 1 -size -1 -name "xcf02s"
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr size -value "0"
addCollection -name "trilevelmodule2"
setAttribute -collection -attr dir -value "UP"
addDesign -version 0 -name "0000"
addDeviceChain -index 0
addDevice -position 1 -file "F:\PT8611\Xilinx\fpga_version\v5_debug\TrilevelModule2\tri_level_module.bit"
setMode -pff
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -collection -attr dir -value "UP"
setAttribute -configdevice -attr path -value "f:\pt8611\xilinx\fpga_version\v5_debug\trilevelmodule2/"
setAttribute -collection -attr name -value "trilevelmodule2"
generate
setMode -bs
setCable -port auto
Identify
identifyMPM
setAttribute -position 1 -attr devicePartName -value "xcf01s"
setAttribute -position 1 -attr configFileName -value "F:\PT8611\Xilinx\fpga_version\v5_debug\TrilevelModule2\trilevelmodule2.mcs"
Program -p 1 -e -v 
