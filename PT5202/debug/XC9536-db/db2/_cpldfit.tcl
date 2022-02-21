set _rspFileHandle [open _cpldfit.rsp w]
puts $_rspFileHandle "-p xc9536xl-5-VQ44"
puts $_rspFileHandle "-optimize speed"
puts $_rspFileHandle "-loc on"
puts $_rspFileHandle "-slew fast"
puts $_rspFileHandle "-init low"
if {[string compare "Template Controlled" "Template Controlled"]==0} {
puts $_rspFileHandle "-pterms 25" } else {
puts $_rspFileHandle "-pterms Template Controlled" }
puts $_rspFileHandle "-power std"
puts $_rspFileHandle "-inputs 54"
close $_rspFileHandle
set _ErrorCode [catch {exec cpldfit -f _cpldfit.rsp addresswatch.ngd} _Results]
exeputs $_Results
exit $_ErrorCode
