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
setCable -port auto
Identify
setMode -pff
setMode -pff
setMode -pff
addConfigDevice -size 1024 -name "xcf08p" -path "F:\pt8612\vhdl\xilinx\sdhd_module_v2/"
setSubmode -pffserial
addPromDevice -position 1 -size -1 -name "xcf08p"
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr size -value "0"
addCollection -name "sync_test1"
setAttribute -collection -attr dir -value "UP"
addDesign -version 0 -name "0000"
addDeviceChain -index 0
addDevice -position 1 -file "F:\PT8612\VHDL\xilinx\SDHD_module_v2\hd_gen_module.bit"
setMode -pff
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -collection -attr dir -value "UP"
setAttribute -configdevice -attr path -value "F:\pt8612\vhdl\xilinx\sdhd_module_v2/"
setAttribute -collection -attr name -value "sync_test1"
generate
setCurrentDesign -version 0
setCurrentDesign -version 0
setMode -bs
setAttribute -position 1 -attr devicePartName -value "xcf08p"
setAttribute -position 1 -attr configFileName -value "F:\PT8612\VHDL\xilinx\SDHD_module_v2\sync_test1.mcs"
Program -p 1 -e -v 
Program -p 1 -e -v 
Verify -p 1 
Verify -p 1 
Verify -p 1 
Verify -p 1 
Verify -p 1 
ReadIdcode -p 2 
ReadIdcode -p 1 
ReadIdcode -p 1 
Checksum -p 1 
Erase -p 1 
Program -p 1 -e -v 
Program -p 1 -e -v 
Program -p 1 -e -v 
deleteDevice -position 1
deleteDevice -position 1
Identify
setAttribute -position 1 -attr devicePartName -value "xcf08p"
setAttribute -position 1 -attr configFileName -value "F:\PT8612\VHDL\xilinx\SDHD_module_v2\sync_test1.mcs"
Program -p 1 -e -v 
Program -p 1 -e -v 
Program -p 1 -e -v 
Program -p 1 -e -v 
