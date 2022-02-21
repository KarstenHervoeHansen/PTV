onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -format Logic /testbench/clk
add wave -noupdate -format Logic /testbench/reset
add wave -noupdate -format Logic /testbench/sync_reset
add wave -noupdate -format Logic /testbench/enable
add wave -noupdate -format Literal /testbench/activedata
add wave -noupdate -format Literal /testbench/line
add wave -noupdate -format Literal /testbench/mux
add wave -noupdate -format Literal /testbench/fvh
add wave -noupdate -format Logic /testbench/clockout
add wave -noupdate -format Literal /testbench/dataout
add wave -noupdate -format Literal /testbench/counterout
add wave -noupdate -format Literal /testbench/audio_test/clkmux_in
add wave -noupdate -format Logic /testbench/audio_test/videoclk_in
add wave -noupdate -format Logic /testbench/audio_test/reset_in
add wave -noupdate -format Logic /testbench/audio_test/sync_reset_in
add wave -noupdate -format Literal /testbench/audio_test/activedata_in
add wave -noupdate -format Literal /testbench/audio_test/audiodata_out
add wave -noupdate -format Logic /testbench/audio_test/audioclk_out
add wave -noupdate -format Literal /testbench/audio_test/counter_buffer
add wave -noupdate -format Literal /testbench/audio_test/output_counter_buffer
add wave -noupdate -format Literal /testbench/audio_test/compare
add wave -noupdate -format Literal /testbench/audio_test/interval
add wave -noupdate -format Literal /testbench/audio_test/counter
add wave -noupdate -format Literal /testbench/audio_test/temp_clk_counter
add wave -noupdate -format Literal /testbench/audio_test/index
add wave -noupdate -format Literal /testbench/audio_test/sample_per_line
add wave -noupdate -format Logic /testbench/audio_test/eav_tick
add wave -noupdate -format Logic /testbench/audio_test/h_delayed_1
add wave -noupdate -format Literal /testbench/audio_test/current_state
add wave -noupdate -format Literal /testbench/audio_test/next_state
add wave -noupdate -format Literal /testbench/audio_test/sample_pos
add wave -noupdate -format Logic /testbench/audio_test/jump_point
add wave -noupdate -format Logic /testbench/audio_test/enable_in
add wave -noupdate -format Logic /testbench/audio_test/switch_point
add wave -noupdate -format Literal /testbench/audio_test/eav_count
add wave -noupdate -format Literal /testbench/audio_test/next_eav_count
add wave -noupdate -format Literal /testbench/audio_test/line_o
add wave -noupdate -format Literal /testbench/audio_test/video_count
add wave -noupdate -format Literal /testbench/audio_test/fvh_i
add wave -noupdate -format Literal /testbench/audio_test/current_groupnr
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {206855800 ps} 0}
WaveRestoreZoom {206547920 ps} {207190480 ps}
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
