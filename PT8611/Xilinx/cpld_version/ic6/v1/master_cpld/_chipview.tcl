set _IsWindows [string equal $tcl_platform(platform) windows]
set _ChipViewExe "ChipView"
if { $_IsWindows } {set _ChipViewExe "ChipView.bat"}
set _ucfFile {C:\PT-Trilevel\xilinx\IC6\v1\master_cpld\master_cpld.ucf}
regsub -all {\B} $_ucfFile / _ucfFile
if { $_IsWindows } {
regsub -all / $_ucfFile \\ _ucfFile
}
set _ErrorCode 0
if {[string compare -nocase -length 4 [string range xc95144xv 4 4] "*"]==0} {
exeputs "ERROR:Pin Assignment ChipViewer can not be launched for and Auto device. \nERROR:Please select a specific CPLD device before running this process."
exit 1
}
if ![file exists $_ucfFile] {
set _ucfFileHandle [open $_ucfFile a]
puts $_ucfFileHandle "//"
close $_ucfFileHandle }
file delete -force "_test.dat"
file copy -force $_ucfFile "_test.ucf"
set _ErrorCode [catch {exec $_ChipViewExe -f master_cpld.ngd -uc $_ucfFile -dev XC95144XV-5-TQ100} _Results]
exeputs $_Results
exeputs "\n"
file copy -force $_ucfFile "_test2.ucf"
exec chkdate
if [file exists "_test.dat"] {
if [file exists "master_cpld.ngd"] {
file rename -force "master_cpld.ngd" "last_used.ngd"
exeputs "The Implement Design process will now be reset so that your constraint changes will be read.\n" }
} else { exeputs "Existing implementation results (if any) will be retained.\n" }
file delete -force _test.ucf _test2.ucf _test.dat
exit $_ErrorCode
