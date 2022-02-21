set _rspFileHandle [open _cpldfit.rsp w]
puts $_rspFileHandle "-p xc95144xv-5-TQ100"
puts $_rspFileHandle "-optimize density"
puts $_rspFileHandle "-loc on"
puts $_rspFileHandle "-slew fast"
puts $_rspFileHandle "-init low"
if {[string compare "90" "Template Controlled"]==0} {
puts $_rspFileHandle "-pterms 90" } else {
puts $_rspFileHandle "-pterms 90" }
puts $_rspFileHandle "-nogclkopt"
puts $_rspFileHandle "-nogtsopt"
puts $_rspFileHandle "-nogsropt"
puts $_rspFileHandle "-power std"
puts $_rspFileHandle "-inputs 50"
close $_rspFileHandle
set _ErrorCode [catch {exec cpldfit -f _cpldfit.rsp tri_level_sync_generator.ngd} _Results]
exeputs $_Results
exit $_ErrorCode
