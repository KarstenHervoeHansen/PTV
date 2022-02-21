loadProjectFile -file "F:/PT8616/final_source/FPGA/gen_prom.ipf"
setMode -bs
setMode -ss
setMode -sm
setMode -hw140
setMode -spi
setMode -acecf
setMode -acempm
setMode -pff
setMode -bs
setMode -bs
setMode -pff
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
setMode -bs
setMode -bs
setMode -pff
setMode -pff
setCurrentDesign -version 0
setMode -pff
setCurrentDeviceChain -index 0
setCurrentDeviceChain -index 0
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr path -value "F:\PT8616\final_source\FPGA\/"
setAttribute -configdevice -attr name -value "final_FPGA"
generate
setCurrentDesign -version 0
setMode -bs
setMode -bs
setMode -pff
setMode -pff
setMode -bs
setMode -bs
setCable -port auto
Program -p 1 -e -v -defaultVersion 0 
assignFile -p 2 -file "F:/PT8616/final_source/FPGA/fpga_main.bit"
Program -p 2 -defaultVersion 0 
setMode -bs
deleteDevice -position 1
deleteDevice -position 1
setMode -pff
setMode -pff
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
setMode -bs
setMode -ss
setMode -sm
setMode -hw140
setMode -spi
setMode -acecf
setMode -acempm
setMode -pff
setMode -pff
setMode -pff
setMode -bs
