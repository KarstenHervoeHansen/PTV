onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -format Logic /testbench/clk
add wave -noupdate -format Logic /testbench/mreset
add wave -noupdate -format Logic /testbench/f4m
add wave -noupdate -format Logic /testbench/f2m
add wave -noupdate -format Logic /testbench/genlock_regen_1/f4m_1_del
add wave -noupdate -format Logic /testbench/genlock_regen_1/f4m_2_del
add wave -noupdate -format Logic /testbench/genlock_regen_1/f4m_pulse
add wave -noupdate -format Logic /testbench/genlock_regen_1/lookat_f4m_pulse
add wave -noupdate -format Literal /testbench/genlock_regen_1/f4m_count
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {0 ps} 0}
WaveRestoreZoom {0 ps} {1 ns}
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
