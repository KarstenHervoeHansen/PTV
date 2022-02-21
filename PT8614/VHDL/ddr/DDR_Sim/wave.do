onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -format Logic /test/clkin_p
add wave -noupdate -format Logic /test/clkin_n
add wave -noupdate -format Logic /test/rst
add wave -noupdate -format Literal /test/leds
add wave -noupdate -format Logic /test/ddr2_casb
add wave -noupdate -format Logic /test/ddr2_cke
add wave -noupdate -format Logic /test/ddr2_clk0
add wave -noupdate -format Logic /test/ddr2_clk0b
add wave -noupdate -format Logic /test/ddr2_csb
add wave -noupdate -format Logic /test/ddr2_rasb
add wave -noupdate -format Logic /test/ddr2_web
add wave -noupdate -format Logic /test/ddr2_odt0
add wave -noupdate -format Literal /test/ddr2_address
add wave -noupdate -format Literal /test/ddr2_ba
add wave -noupdate -format Literal /test/ddr2_dm
add wave -noupdate -format Literal /test/ddr2_dqs
add wave -noupdate -format Literal /test/ddr2_dqs_n
add wave -noupdate -format Literal /test/ddr2_dq
add wave -noupdate -format Logic /test/uut/system_controller_inst/reset
add wave -noupdate -format Logic /test/uut/system_controller_inst/clk
add wave -noupdate -format Logic /test/uut/system_controller_inst/clk90
add wave -noupdate -format Logic /test/uut/system_controller_inst/clk180
add wave -noupdate -format Logic /test/uut/system_controller_inst/dcm_lock
add wave -noupdate -format Logic /test/uut/system_controller_inst/burst_done
add wave -noupdate -format Literal /test/uut/system_controller_inst/user_input_address
add wave -noupdate -format Literal /test/uut/system_controller_inst/user_bank_address
add wave -noupdate -format Literal /test/uut/system_controller_inst/user_input_data
add wave -noupdate -format Literal /test/uut/system_controller_inst/user_data_mask
add wave -noupdate -format Logic /test/uut/system_controller_inst/auto_ref_req
add wave -noupdate -format Logic /test/uut/system_controller_inst/ar_done
add wave -noupdate -format Logic /test/uut/system_controller_inst/user_cmd_ack
add wave -noupdate -format Logic /test/uut/system_controller_inst/user_data_valid
add wave -noupdate -format Literal /test/uut/system_controller_inst/user_output_data
add wave -noupdate -format Logic /test/uut/system_controller_inst/init_val
add wave -noupdate -format Literal /test/uut/system_controller_inst/user_config_register1
add wave -noupdate -format Literal /test/uut/system_controller_inst/user_config_register2
add wave -noupdate -format Literal /test/uut/system_controller_inst/user_command_register
add wave -noupdate -format Logic /test/uut/system_controller_inst/tp1
add wave -noupdate -format Logic /test/uut/system_controller_inst/tp2
add wave -noupdate -format Logic /test/uut/system_controller_inst/tp3
add wave -noupdate -format Literal /test/uut/system_controller_inst/tp_port
add wave -noupdate -format Literal /test/uut/system_controller_inst/current_state
add wave -noupdate -format Literal /test/uut/system_controller_inst/next_state
add wave -noupdate -format Literal /test/uut/system_controller_inst/next_reg1
add wave -noupdate -format Literal /test/uut/system_controller_inst/next_reg2
add wave -noupdate -format Literal /test/uut/system_controller_inst/next_reg3
add wave -noupdate -format Literal /test/uut/system_controller_inst/next_loop_count
add wave -noupdate -format Literal /test/uut/system_controller_inst/loop_count
add wave -noupdate -format Literal /test/uut/system_controller_inst/next_addr
add wave -noupdate -format Literal /test/uut/system_controller_inst/next_input_data
add wave -noupdate -format Logic /test/uut/system_controller_inst/next_burst_done
add wave -noupdate -format Literal /test/uut/system_controller_inst/intern_reg0
add wave -noupdate -format Literal /test/uut/system_controller_inst/intern_reg1
add wave -noupdate -format Literal /test/uut/system_controller_inst/intern_reg2
add wave -noupdate -format Literal /test/uut/system_controller_inst/intern_reg3
add wave -noupdate -format Logic /test/uut/system_controller_inst/read_waiting
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {699999265 ps} 0}
configure wave -namecolwidth 150
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
update
WaveRestoreZoom {699991550 ps} {700007550 ps}
