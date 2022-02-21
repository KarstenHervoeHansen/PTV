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
setMode -bs
setMode -bs
Program -p 2 
setMode -pff
setMode -pff
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr path -value "C:/PT8616/development_src/FPGA/"
setAttribute -configdevice -attr name -value "top"
generate
setCurrentDesign -version 0
setMode -bs
setMode -bs
setMode -pff
setMode -pff
setMode -bs
setMode -bs
Program -p 1 -e -v 
setMode -pff
setMode -pff
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr path -value "C:/PT8616/development_src/FPGA/"
setAttribute -configdevice -attr name -value "top"
generate
setCurrentDesign -version 0
setMode -bs
setMode -bs
Program -p 1 -e -v 
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
