loadProjectFile -file "C:/PT8616/final_source/FPGA/config.ipf"
setMode -ss
setMode -sm
setMode -hw140
setMode -spi
setMode -acecf
setMode -acempm
setMode -pff
setMode -bs
setMode -bs
setMode -bs
setMode -pff
setMode -pff
setMode -bs
setMode -pff
setMode -pff
setMode -bs
setMode -bs
setMode -pff
setMode -pff
setMode -bs
setMode -bs
setMode -pff
setMode -pff
setCurrentDesign -version 0
setMode -pff
setCurrentDeviceChain -index 0
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr path -value "c:\PT8616\final_source\FPGA\/"
setAttribute -configdevice -attr name -value "top"
generate
setCurrentDesign -version 0
setMode -bs
setMode -bs
setMode -pff
setMode -pff
assignFile -p 1 -file "C:/PT8616/final_source/released/SW_ver2.2/FPGA/fpga_main.bit"
setAttribute -position 1 -attr devicePartName -value "xc3s250e"
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr path -value "c:\PT8616\final_source\FPGA\/"
setAttribute -configdevice -attr name -value "top"
generate
setCurrentDesign -version 0
deletePromDevice -position 1
setMode -pff
setMode -pff
setCurrentDesign -version 0
setMode -pff
setCurrentDeviceChain -index 0
setMode -bs
setMode -bs
setMode -pff
setMode -pff
addConfigDevice  -name "top" -path "C:/PT8616/final_source/released/SW_ver2.2/FPGA"
setSubmode -pffserial
addDesign -version 0 -name "00"
setMode -pff
addDeviceChain -index 0
setMode -pff
addDeviceChain -index 0
setAttribute -configdevice -attr compressed -value "FALSE"
setAttribute -configdevice -attr compressed -value "FALSE"
setAttribute -configdevice -attr autoSize -value "TRUE"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr swapBit -value "FALSE"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr multiboot -value "FALSE"
setAttribute -configdevice -attr multiboot -value "FALSE"
setAttribute -configdevice -attr spiSelected -value "FALSE"
setAttribute -configdevice -attr spiSelected -value "FALSE"
setMode -pff
setCurrentDesign -version 0
deleteDevice -position 1
deleteDesign -version 0
setCurrentDesign -version -1
setMode -pff
setMode -pff
setMode -pff
setMode -bs
setMode -bs
setCurrentDesign -version 0
deleteDesign -version 0
setCurrentDesign -version -1
setMode -pff
addConfigDevice -size 512 -name "top" -path "C:/PT8616/final_source/released/SW_ver2.2/FPGA/"
setSubmode -pffserial
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr flashDataWidth -value ""
setMode -pff
setMode -bs
setMode -bs
setMode -pff
setMode -pff
setMode -pff
setMode -pff
setMode -bs
setMode -bs
setMode -pff
setMode -pff
addConfigDevice  -name "top" -path "C:\PT8616\final_source\released\SW_ver2.2\"
setSubmode -pffserial
addDesign -version 0 -name "00"
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
addPromDevice -p 1 -size 0 -name xcf02s
setMode -pff
setMode -pff
setMode -pff
setMode -pff
deletePromDevice -position 1
setCurrentDesign -version 0
deleteDesign -version 0
setCurrentDesign -version -1
setMode -pff
addConfigDevice -size 512 -name "top" -path "C:\PT8616\final_source\released\SW_ver2.2\/"
setSubmode -pffserial
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr flashDataWidth -value "8"
addPromDevice -p 1 -size 0 -name xcf02s
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr dir -value "UP"
addDesign -version 0 -name "0000"
setMode -pff
addDeviceChain -index 0
setAttribute -design -attr name -value "0"
addDevice -p 1 -file "C:/PT8616/final_source/released/SW_ver2.2/FPGA/fpga_main.bit"
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr path -value "C:\PT8616\final_source\released\SW_ver2.2\/"
setAttribute -configdevice -attr name -value "top"
generate
setCurrentDesign -version 0
saveProjectFile -file "C:/PT8616/final_source/released/SW_ver2.2/FPGA/config.ipf"
setMode -bs
deleteDevice -position 1
deleteDevice -position 1
setMode -ss
setMode -sm
setMode -hw140
setMode -spi
setMode -acecf
setMode -acempm
setMode -pff
deletePromDevice -position 1
setCurrentDesign -version 0
deleteDevice -position 1
deleteDesign -version 0
setCurrentDesign -version -1
setMode -pff
setMode -pff
setMode -bs
setMode -ss
setMode -sm
setMode -hw140
setMode -spi
setMode -acecf
setMode -acempm
setMode -pff
setMode -bs
setMode -pff
setMode -bs
