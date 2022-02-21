setMode -pff
setMode -pff
addConfigDevice  -name "PT8612(1.8.1.6)" -path "F:\PT8613\VHDL\PT8612 (1.8.1.6)\"
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
addConfigDevice -size 512 -name "PT8612(1.8.1.6)" -path "F:\PT8613\VHDL\PT8612 (1.8.1.6)\/"
setSubmode -pffserial
setAttribute -configdevice -attr dir -value "UP"
addPromDevice -p 1 -size 0 -name xcf08p
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr dir -value "UP"
addDesign -version 0 -name "0000"
setMode -pff
addDeviceChain -index 0
setCurrentDeviceChain -index 0
setCurrentDeviceChain -index 0
setAttribute -design -attr name -value "0"
addDevice -p 1 -file "F:/PT8613/VHDL/PT8612 (1.8.1.6)/hd_gen_module.bit"
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr path -value "F:\PT8613\VHDL\PT8612 (1.8.1.6)\/"
setAttribute -configdevice -attr name -value "PT8612(1.8.1.6)"
generate
setCurrentDesign -version 0
setMode -bs
setMode -ss
setMode -sm
setMode -hw140
setMode -spi
setMode -acecf
setMode -acempm
setMode -pff
deletePromDevice -position 1
setCurrentDesign -version 0
setCurrentDeviceChain -index 0
deleteDevice -position 1
setCurrentDeviceChain -index 0
deleteDesign -version 0
setCurrentDesign -version -1
setMode -pff
setMode -pff
setMode -pff
setMode -pff
