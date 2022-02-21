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

addPromDevice -position 1 -size 0 -name "xcf08p"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr swapBit -value "FALSE"
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -collection -attr dir -value "UP"
setMode -mpm
setMode -cf
setMode -dtconfig
setMode -bsfile
setMode -sm
setMode -ss
setMode -bs
addDevice -position 1 -part "xcf08p"
setAttribute -position 1 -attr configFileName -value "F:\PT8612\VHDL\xilinx\SDHD_module_v2\sdhd_lyd_noctrlpkt.mcs"

addDevice -position 2 -part "xc2vp20"
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
setCurrentDeviceChain -index 0
setMode -bs
setMode -pff
setMode -bs
setMode -pff
setCurrentDesign -version 0
deletePromDevice -position 1
setMode -pff
setMode -pff
addConfigDevice -size 1024 -name "xcf08p" -path "\\wts\data1\pt8612\vhdl\xilinx\sdhd_module_v2/"
setSubmode -pffserial
addPromDevice -position 1 -size -1 -name "xcf08p"
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr size -value "0"
addCollection -name "channelstatus_test3"
setAttribute -collection -attr dir -value "UP"
addDesign -version 0 -name "0000"
addDeviceChain -index 0
addDevice -position 1 -file "F:\PT8612\VHDL\xilinx\SDHD_module_v2\hd_gen_module.bit"
setMode -pff
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -collection -attr dir -value "UP"
setAttribute -configdevice -attr path -value "\\wts\data1\pt8612\vhdl\xilinx\sdhd_module_v2/"
setAttribute -collection -attr name -value "channelstatus_test3"
generate
setCurrentDesign -version 0
setMode -bs
Identify
setAttribute -position 1 -attr devicePartName -value "xcf08p"
setAttribute -position 1 -attr configFileName -value "F:\PT8612\VHDL\xilinx\SDHD_module_v2\channelstatus_test3.mcs"
Program -p 1 -e -v 
setAttribute -position 1 -attr devicePartName -value "xcf08p"
setAttribute -position 1 -attr configFileName -value "F:\PT8612\VHDL\xilinx\SDHD_module_v2 (slow clock)\slow_clock.mcs"
Program -p 1 -e -v 
setCable -port auto
