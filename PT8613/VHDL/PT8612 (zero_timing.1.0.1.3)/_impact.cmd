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
setMode -pff
setMode -pff
setMode -pff
addConfigDevice -size 1024 -name "xcf08p" -path "F:\pt8613\vhdl\pt8613 (work)/"
setSubmode -pffserial
addPromDevice -position 1 -size -1 -name "xcf08p"
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr size -value "0"
addCollection -name "pt8612_zero_timing_1_3"
setAttribute -collection -attr dir -value "UP"
addDesign -version 0 -name "0000"
addDeviceChain -index 0
addDevice -position 1 -file "F:\PT8613\VHDL\PT8612 (zero_timing.1.3)\hd_gen_module.bit"
setMode -pff
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -collection -attr dir -value "UP"
setAttribute -configdevice -attr path -value "F:\pt8613\vhdl\pt8613 (work)/"
setAttribute -collection -attr name -value "pt8612_zero_timing_1_3"
generate
setCurrentDesign -version 0
setMode -bs
setCable -port auto
Identify
setAttribute -position 1 -attr devicePartName -value "xcf08p"
setAttribute -position 1 -attr configFileName -value "F:\PT8613\VHDL\PT8612 (zero_timing.1.3)\pt8612_zero_timing_1_3.mcs"
Program -p 1 -e -v 
setCable -port usb21 -baud 750000
Program -p 1 -e -v 
