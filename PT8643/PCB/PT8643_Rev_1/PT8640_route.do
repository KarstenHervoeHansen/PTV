# start of route file
#################################
#### Custom Route introduction

set dofile_auto_repaint on

vset system Place_error off
vset system Place_front off
vset system Place_back off

set banner 0
set banner 9
set slp

create $/window.scr
append  #
append  # Custom Route - Requires fanout complete.
append  #
append  # Provides and guides you through an enhanced route strategy.
append  #
append  # Allows user intervention to monitor performance and improve router strategy.
append  #
append  # Optional setup may be performed before continuing(display, layer bias, grid, object selection, etc).
append  #
close
report file $/window.scr
if (&slp) (sleep 5) else ( command )

#################################
#### Check for from to max length less than manhattan.

check
report place_length window

#################################
#### Write banner.

set banner 0
set banner 9
set slp

create $/window.scr
append  #
append  # Design rules have been entered and this report shows the rules as the router sees them.
append  #
append  # If you see (Max < Manhattan) in the report it is suggested that you modify placement to eliminate this error before continuing.
append  #
append  # If the report looks acceptable, select Continue to route this design.
append  #
append  # Watch for messages in this area to monitor route progress.
append  #
close
report file $/window.scr
if (&slp) (sleep 5) else ( command )

## Initial Route phase based on evaluate (unconnect_wire).
## The number of unconnects set here is arbitrary.
#################################
#### Initial Route phase

#################################
#### Write banner.

set banner 0
set banner 5
set slp

create $/window.scr
append  #
append  #
append  # Running Initial Route passes 1 thru 5.
append  #
append  #
close
report file $/window.scr
#if (&slp) (sleep 5) else ( command )

#################################
#### Setup save files.

autosave on $/aut.w
bestsave on $/bst.w
status_file $/rte.sts
set update_interval 100

#################################
#### Begin routing.

route 5

#################################
#### Write banner.

set banner 0
set banner 7
set slp

create $/window.scr
append  #
append  # Stop if route results do not meet first 5 route pass performance criteria:
append  #
append  # After pass 1 the total conflicts should be less than or equal to 5 times the number of connections.
append  # After pass 2 thru 5 the rate reduction per pass should be greater than or equal to 20
append  # After pass 5 there should be no unroutes.
append  #
close
report file $/window.scr
if (&slp) (sleep 5) else ( command )

#################################
#### Write files.

write session $/rt5.ses
write wire $/rt5.w

set auto_remove off

#################################
#### Write banner.

set banner 0
set banner 5
set slp

create $/window.scr
append  #
append  #
append  # Running Initial Route passes 6 thru 25.
append  #
append  #
close
report file $/window.scr
#if (&slp) (sleep 5) else ( command )

route 20 6

#################################
#### Write banner.

set banner 0
set banner 5
set slp

create $/window.scr
append  #
append  #
append  # Running 2 clean passes.
append  #
append  #
close
report file $/window.scr
#if (&slp) (sleep 5) else ( command )

clean 2

write session $/rt25.ses
            
#################################
#### First Route Phase

#################################
#### Write banner.

set banner 0
set banner 5
set slp

create $/window.scr
append  #
append  #
append  # Running Route Phase 
append  #
append  #
close
report file $/window.scr
#if (&slp) (sleep 5) else ( command )

setexpr count (5)
while (count > 0 && count >0 && unconnect_wire <= 20) 
      (
       setexpr unconn_wire (unconnect_wire)
       route 10 16
       if (unconnect_wire <= 20 && unconnect_wire <= unconn_wire)
          then (
                setexpr count (count - 1)
               )
          else (
                setexpr count (0)
                tax cross .5
                tax squeeze .2
               )
      )

#################################
#### Second Route Phase

# Set same net checking on
set same_net_checking on

# Read in noise and xtalk rules
do $/D:/Dokumenter/PT5300_NTP/PT8640/PCB/PT8640_noise.do
do $/D:/Dokumenter/PT5300_NTP/PT8640/PCB/PT8640_xtalk.do

if (unconnect_wire <= 20)
      then (clean 2)

setexpr count2 (5)
while (count2 >0 && unconnect_wire <= 20) 
      (       
       setexpr unconn_wire2 (complete_wire)
       route 10 16
       if (unconnect_wire <= unconn_wire2)
          then (
                setexpr count2 (count2 - 1)
               )
          else (
                setexpr count2 (0)
                tax cross .5
                tax squeeze .2
               )
      )
#################################
# Read diff pair timing rules
do $/D:/Dokumenter/PT5300_NTP/PT8640/PCB/PT8640_diffpair_timing.do

#################################
#### Third Route Phase

if (unconnect_wire <= 20)
      then (clean 2)

setexpr count3 (5)
while (count3 >0 && unconnect_wire <= 20) 
      (       
       setexpr unconn_wire3 (unconnect_wire)
       route 15 16
       if (unconnect_wire <= unconn_wire3)
          then (
                setexpr count3 (count3 - 1)
                clean
               )
          else  (
                setexpr count3 (0)
                tax cross .5
                tax squeeze .2
                )
      )

#################################
#### Fourth Route Phase

if (unconnect_wire <= 20)
      then (clean 2)

setexpr count4 (6)
while (count4 >0 && unconnect_wire <= 20) 
      (       
       setexpr unconn_wire4 (unconnect_wire)
       route 25 16
       if (unconnect_wire <= unconn_wire4)
          then (
                setexpr count4 (count4 - 1)
                clean
               )
          else (
                     setexpr count4 (0)
                    )
      )

#################################
#### Final Cleanup

write routes $/temp.w
delete all wires
read routes $/temp.w
clean 4
write routes $/final.w
report status $/final.sts

#################################
#### Write banner.

set banner 0
set banner 5
set slp

create $/window.scr
append  #
append  #
append  # Select Continue to automatically import result into Allegro 
append  #
append  #
close
report file $/window.scr
if (&slp) (sleep 5) else ( command )


# end of do file
