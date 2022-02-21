setPreference -pref AutoSignature:FALSE
setPreference -pref KeepSVF:FALSE
setPreference -pref ConcurrentMode:FALSE
setPreference -pref UseHighz:FALSE
setPreference -pref UserLevel:NOVICE
setPreference -pref svfUseTime:FALSE
setMode -pff
setMode -pff
addConfigDevice  -name "ddr_test1" -path "C:\MyProject\DDR2interface_test\PT8612_with_ddr\"
setSubmode -pffserial
addDesign -version 0 -name ""
setCurrentDesign -version 0
setMode -pff
addDeviceChain -index 0
setAttribute -configdevice -attr compressed -value "FALSE"
setAttribute -configdevice -attr compressed -value "FALSE"
setAttribute -configdevice -attr autoSize -value "FALSE"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr swapBit -value "FALSE"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr multiboot -value "FALSE"
setAttribute -configdevice -attr multiboot -value "FALSE"
setAttribute -configdevice -attr spiSelected -value "FALSE"
setAttribute -configdevice -attr spiSelected -value "FALSE"
addPromDevice -p 1 -size 0 -name xcf08p
setMode -pff
setMode -pff
deletePromDevice -position 1
setCurrentDesign -version 0
setCurrentDeviceChain -index 0
setCurrentDeviceChain -index 0
deleteDesign -version 0
setCurrentDesign -version -1
setMode -pff
addConfigDevice -size 512 -name "ddr_test1" -path "C:\MyProject\DDR2interface_test\PT8612_with_ddr\/"
setSubmode -pffserial
setAttribute -configdevice -attr dir -value "UP"
addPromDevice -p 1 -size 0 -name xcf08p
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr dir -value "UP"
addDesign -version 0 -name "0000"
setMode -pff
addDeviceChain -index 0
setCurrentDesign -version 0
setCurrentDeviceChain -index 0
setCurrentDesign -version 0
setCurrentDeviceChain -index 0
setAttribute -design -attr name -value 0
addDevice -p 1 -file "C:/MyProject/DDR2interface_test/PT8612_with_ddr/hd_gen_module.bit"
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr path -value "C:\MyProject\DDR2interface_test\PT8612_with_ddr\/"
setAttribute -configdevice -attr name -value "ddr_test1"
generate
setCurrentDesign -version 0
setCurrentDesign -version 0
setMode -bs
setMode -bs
setCable -port auto
Identify 
identifyMPM 
assignFile -p 1 -file "C:/MyProject/DDR2interface_test/PT8612_with_ddr/ddr_test1.mcs"
setAttribute -position 1 -attr packageName -value "(null)"
Program -p 1 -defaultVersion 0 
setMode -pff
setMode -pff
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr path -value "C:\MyProject\DDR2interface_test\PT8612_with_ddr\/"
setAttribute -configdevice -attr name -value "ddr_test1"
generate
setCurrentDesign -version 0
setCurrentDesign -version 0
setMode -bs
setMode -bs
setCable -port auto
Program -p 1 -defaultVersion 0 
Erase -p 1 
Verify -p 1 
Program -p 1 -defaultVersion 0 
setMode -pff
setMode -pff
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr path -value "C:\MyProject\DDR2interface_test\PT8612_with_ddr\/"
setAttribute -configdevice -attr name -value "ddr_test1"
generate
setCurrentDesign -version 0
setCurrentDesign -version 0
setMode -bs
setMode -bs
Program -p 1 -defaultVersion 0 
Erase -p 1 
Program -p 1 -defaultVersion 0 
assignFile -p 1 -file "C:/MyProject/DDR2interface_test/PT8612 (1.0.1.5)/ddr2.mcs"
setAttribute -position 1 -attr packageName -value "(null)"
Program -p 1 -defaultVersion 0 
Erase -p 1 
Program -p 1 -defaultVersion 0 
assignFile -p 1 -file "C:/MyProject/DDR2i