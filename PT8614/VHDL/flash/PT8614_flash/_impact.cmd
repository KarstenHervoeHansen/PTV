setMode -bs
setCable -port auto
identify 
identifyMPM 
assignFile -p 1 -file "implementation/download.bit"
program -p 1 
quit
