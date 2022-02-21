setMode -pff
setMode -pff
addConfigDevice  -name "top" -path "C:\MATHIAS_WORK\PT8612\PT8612 (work)\"
setSubmode -pffserial
setAttribute -configdevice -attr multibootBpiType -value ""
addDesign -version 0 -name ""
setMode -pff
addDeviceChain -index 0
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
addPromDevice -p 1 -size 0 -name xcf16p
setMode -pff
setMode -pff
deletePromDevice -position 1
setCurrentDesign -version 0
setCurrentDeviceChain -index 0
setCurrentDeviceChain -index 0
deleteDesign -version 0
setCurrentDesign -version -1
setMode -pff
addConfigDevice -size 512 -name "top" -path "C:\MATHIAS_WORK\PT8612\PT8612 (work)\/"
setSubmode -pffserial
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr flashDataWidth -value "8"
addPromDevice -p 1 -size 0 -name xcf16p
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr dir -value "UP"
addDesign -version 0 -name "0000"
setMode -pff
addDeviceChain -index 0
setCurrentDeviceChain -index 0
setCurrentDeviceChain -index 0
setAttribute -design -attr name -value "0"
addDevice -p 1 -file "C:/MATHIAS_WORK/PT8612/PT8612 (work)/hd_gen_module.bit"
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr path -value "C:\MATHIAS_WORK\PT8612\PT8612 (work)\/"
setAttribute -configdevice -attr name -value "top"
generate
setCurrentDesign -version 0
setMode -bs
setMode -bs
setCable -port usb21 -baud -1
setCable -port usb21 -baud 750000
Identify 
identifyMPM 
assignFile -p 1 -file "C:/MATHIAS_WORK/PT8612/PT8612 (work)/top.mcs"
setAttribute -position 1 -attr packageName -value "(null)"
Program -p 1 -e -loadfpga -defaultVersion 0 
Program -p 1 -e -v -loadfpga -defaultVersion 0 
setMode -pff
setMode -pff
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr path -value "C:\MATHIAS_WORK\PT8612\PT8612 (work)\/"
setAttribute -configdevice -attr name -value "top"
generate
setCurrentDesign -version 0
cutDevice -p 1
deletePromDevice -position 1
setMode -pff
setMode -pff
setCurrentDesign -version 0
setMode -pff
setCurrentDeviceChain -index 0
setMode -pff
setCurrentDesign -version 0
setCurrentDeviceChain -index 0
setCurrentDeviceChain -index 0
deleteDesign -version 0
setCurrentDesign -version -1
setMode -pff
setMode -bs
setMode -bs
setMode -pff
setMode -pff
addConfigDevice  -name "top" -path "C:\MATHIAS_WORK\PT8612\PT8612 (work)\"
setSubmode -pffserial
setAttribute -configdevice -attr multibootBpiType -value ""
addDesign -version 0 -name ""
setMode -pff
addDeviceChain -index 0
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