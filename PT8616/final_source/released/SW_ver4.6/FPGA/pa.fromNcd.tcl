
# PlanAhead Launch Script for Post PAR Floorplanning, created by Project Navigator

create_project -name FPGA -dir "C:/PT8616/development_src/FPGA/planAhead_run_2" -part xc3s250evq100-5
set srcset [get_property srcset [current_run -impl]]
set_property design_mode GateLvl $srcset
set_property edif_top_file "C:/PT8616/development_src/FPGA/FPGA_main.ngc" [ get_property srcset [ current_run ] ]
add_files -norecurse { {C:/PT8616/development_src/FPGA} }
add_files "C:/PT8616/development_src/FPGA/Chipscope.ncf" "C:/PT8616/development_src/FPGA/Chipscope_ctrl.ncf" -fileset [get_property constrset [current_run]]
set_param project.paUcfFile  "constraints.ucf"
add_files "constraints.ucf" -fileset [get_property constrset [current_run]]
open_netlist_design
read_xdl -file "C:/PT8616/development_src/FPGA/FPGA_main.ncd"
if {[catch {read_twx -name results_1 -file "C:/PT8616/development_src/FPGA/FPGA_main.twx"} eInfo]} {
   puts "WARNING: there was a problem importing \"C:/PT8616/development_src/FPGA/FPGA_main.twx\": $eInfo"
}
