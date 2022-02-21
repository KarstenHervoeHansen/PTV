setPreference -pref UserLevel:NOVICE
setPreference -pref MessageLevel:DETAILED
setPreference -pref ConcurrentMode:TRUE
setPreference -pref UseHighz:FALSE
setPreference -pref ConfigOnFailure:STOP
setPreference -pref StartupCLock:AUTO_CORRECTION
setPreference -pref AutoSignature:FALSE
setPreference -pref KeepSVF:FALSE
setPreference -pref svfUseTime:FALSE
setPreference -pref UserLevel:NOVICE
setPreference -pref MessageLevel:DETAILED
setPreference -pref ConcurrentMode:TRUE
setPreference -pref UseHighz:FALSE
setPreference -pref ConfigOnFailure:STOP
setPreference -pref StartupCLock:AUTO_CORRECTION
setPreference -pref AutoSignature:FALSE
setPreference -pref KeepSVF:FALSE
setPreference -pref svfUseTime:FALSE
setMode -pff
setMode -pff
setMode -pff
addConfigDevice -size 128 -name "xcf01s" -path "f:\pt8611\xilinx\fpga_version\v5_debug\v5.02_debug/"
setSubmode -pffserial
addPromDevice -position 1 -size -1 -name "xcf01s"
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr size -value "0"
addCollection -name "v502db2"
setAttribute -collection -attr dir -value "UP"
addDesign -version 0 -name "0000"
addDeviceChain -index 0
addDevice -position 1 -file "F:\PT8611\Xilinx\fpga_version\v5_debug\v5.02_debug\Tri_level_sync_generator\tri_level_sync_generator.bit"
setMode -pff
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -collection -attr dir -value "UP"
setAttribute -configdevice -attr path -value "f:\pt8611\xilinx\fpga_version\v5_debug\v5.02_debug/"
setAttribute -collection -attr name -value "v502db2"
generate
setMode -bs
setCable -port auto
Identify
setAttribute -position 1 -attr devicePartName -value "xcf01s"
setAttribute -position 1 -attr configFileName -value "F:\PT8611\Xilinx\fpga_version\v5_debug\v5.02_debug\Tri_level_sync_generator\v502.mcs"
Program -p 1 -e -v 
setAttribute -position 1 -attr devicePartName -value "xcf01s"
setAttribute -position 1 -attr configFileName -value "F:\PT8611\Xilinx\fpga_version\backup\v5.01\latest_promfile\tsg_4ch.mcs"
setCable -port auto
Program -p 1 -e -v 
setAttribute -position 1 -attr devicePartName -value "xcf01s"
setAttribute -position 1 -attr configFileName -value "F:\PT8611\Xilinx\fpga_version\v5_debug\v5.01_debug\v501db1.mcs"
Program -p 1 -e -v 
