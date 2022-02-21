###########  Advanced dofile  ############
##  This dofile will not let the router continue routing 
##  if it is not making progress.
#
# This Do file was downloaded from the CCT Users Group online web page
#   at  www.cctug.org
#
#### To turn off repaint of the screen while loading do files
#    remove the comment from the line below
#
# no_repaint 1
#
#### Place your design rules in the "rules.do" file and reference them here
#    by removing the comment from the "do rules.do" line below
#
# Do rules.do
#
#### Fanouts should be complete before running this DO file
#    If you are unsure of the fanout command syntax, try the following:
#
# unit mil
# fanout 5 (pin_type active)(via_share on)(share_len 100)
#
unit mil
status_file $/SpecctraWithinLayout.STS
bestsave on $/SpecctraWithinLayout.WBEST
#
# You may override the grids specified in Layout from within this file
#
# grid wire 1
# grid via 1
#
#### Try both these router taxes together on dense boards
#
# tax cross 1.2
# tax squeeze .5
#
#################################
#### Initial Route phase
#
route 7
if (complete_wire < 100)
      then (clean 2)
#
#### Route phase 1
#
setexpr count (3)
while (count >0 && complete_wire < 100) 
      (
       setexpr comp_rate (complete_wire)
       route 5 11
       if (complete_wire < 100 && complete_wire > comp_rate)
          then (
                setexpr count (count - 1)
               )
          else (setexpr count (0))
      )
#
#### Route phase 2
#
if (complete_wire < 100)
      then (clean 2)
setexpr count2 (5)
while (count2 >0 && complete_wire < 100) 
      (       
       setexpr comp_rate2 (complete_wire)
       route 5 16
       if (complete_wire > comp_rate2)
          then (
                setexpr count2 (count2 - 1)
               )
          else (
                setexpr count2 (0)
               )
      )
#
#### Route phase 3
#
if (complete_wire < 100)
      then (clean 3)
setexpr count3 (10)
while (count3 >0 && complete_wire < 100) 
      (       
       setexpr comp_rate3 (complete_wire)
       route 5 16
       if (complete_wire > comp_rate3)
          then (
                setexpr count3 (count3 - 1)
               )
          else (
                filter 5
                limit cross 0
                route 25 16
                setexpr count3 (0)
               )
      )
#
#### Final Cleanup
#
clean 5
#
# uncomment out the miter commands below to miter your corners
#
 unit mil
 miter (slant 1000)
 miter (pin 50)
 miter (bend 32 2)
#
write routes $/SpecctraWithinLayout.RTE
report status

