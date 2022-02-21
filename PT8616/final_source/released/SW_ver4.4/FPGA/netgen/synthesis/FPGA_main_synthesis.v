////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2010 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: M.81d
//  \   \         Application: netgen
//  /   /         Filename: FPGA_main_synthesis.v
// /___/   /\     Timestamp: Mon Jun 06 13:06:32 2011
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -intstyle ise -insert_glbl true -w -dir netgen/synthesis -ofmt verilog -sim FPGA_main.ngc FPGA_main_synthesis.v 
// Device	: xc3s250e-5-vq100
// Input file	: FPGA_main.ngc
// Output file	: C:\PT8616\development_src\FPGA\netgen\synthesis\FPGA_main_synthesis.v
// # of Modules	: 1
// Design Name	: FPGA_main
// Xilinx        : C:\Xilinx\12.4\ISE_DS\ISE\
//             
// Purpose:    
//     This verilog netlist is a verification model and uses simulation 
//     primitives which may not represent the true implementation of the 
//     device, however the netlist is functionally correct and should not 
//     be modified. This file cannot be synthesized and should only be used 
//     with supported simulation tools.
//             
// Reference:  
//     Command Line Tools User Guide, Chapter 23 and Synthesis and Simulation Design Guide, Chapter 6
//             
////////////////////////////////////////////////////////////////////////////////

`timescale 1 ns/1 ps

module FPGA_main (
  SDA_io, rst_i, tp1_o, LTC_a_out, tp2_o, PPS_status_o, din_10mhz_o, led1_o, din_148mhz, tp3_o, sync_148mhz, NTP_PPS, h_d_o, clk_148_n, LTC_b_out, 
led2_o, clk_148_p, PPS_i, ext_int_o, MCU_RX, NTP_RX, sclk_148mhz, clk_10_i, UBX_TX_i, SCL_i, sync_10mhz_o, sclk_10mhz_o, f_d_o
);
  inout SDA_io;
  input rst_i;
  output tp1_o;
  output LTC_a_out;
  output tp2_o;
  output PPS_status_o;
  output din_10mhz_o;
  output led1_o;
  output din_148mhz;
  output tp3_o;
  output sync_148mhz;
  output NTP_PPS;
  output h_d_o;
  input clk_148_n;
  output LTC_b_out;
  output led2_o;
  input clk_148_p;
  input PPS_i;
  output ext_int_o;
  output MCU_RX;
  output NTP_RX;
  output sclk_148mhz;
  input clk_10_i;
  input UBX_TX_i;
  input SCL_i;
  output sync_10mhz_o;
  output sclk_10mhz_o;
  output f_d_o;
  wire I2C_byte_out_cmp_eq0000;
  wire I2C_byte_out_cmp_eq0001;
  wire I2C_byte_out_cmp_eq0002;
  wire I2C_byte_out_cmp_eq0003;
  wire I2C_byte_out_cmp_eq0004;
  wire I2C_byte_out_cmp_eq0005;
  wire I2C_byte_out_cmp_eq0006;
  wire I2C_byte_out_cmp_eq0007;
  wire I2C_byte_out_cmp_eq0008;
  wire I2C_byte_out_cmp_eq0009;
  wire I2C_byte_out_cmp_eq0010;
  wire I2C_byte_out_cmp_eq0011;
  wire I2C_byte_out_cmp_eq0012;
  wire I2C_byte_out_cmp_eq0013;
  wire I2C_byte_out_cmp_eq0014;
  wire I2C_byte_out_cmp_eq0015;
  wire I2C_byte_out_cmp_eq0016;
  wire I2C_byte_out_cmp_eq0017;
  wire I2C_byte_out_cmp_eq0018;
  wire I2C_byte_out_cmp_eq0019;
  wire I2C_byte_out_cmp_eq0020;
  wire I2C_byte_out_cmp_eq0021;
  wire I2C_byte_out_cmp_eq0022;
  wire I2C_byte_out_cmp_eq0024;
  wire I2C_byte_out_cmp_eq0025;
  wire I2C_byte_out_cmp_eq0026;
  wire I2C_byte_out_cmp_eq0027;
  wire I2C_byte_out_or0000_27;
  wire \Inst_ad5660_serial_DAC/N01 ;
  wire \Inst_ad5660_serial_DAC/N3 ;
  wire \Inst_ad5660_serial_DAC/N4 ;
  wire \Inst_ad5660_serial_DAC/N5 ;
  wire \Inst_ad5660_serial_DAC/data_o_46 ;
  wire \Inst_ad5660_serial_DAC/data_o_mux00011 ;
  wire \Inst_ad5660_serial_DAC/dec_clk_count_not0001 ;
  wire \Inst_ad5660_serial_DAC/sclk_53 ;
  wire \Inst_ad5660_serial_DAC/sclk_mux0000 ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000[0] ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000<10>1_80 ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000<11>1_81 ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000<12>1_82 ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000<13>1_83 ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000<14>1_84 ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000<15>1_85 ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000[16] ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000[17] ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000[18] ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000[19] ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000<1>1_90 ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000[20] ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000[21] ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000[22] ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000[23] ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000<2>1_95 ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000<3>1_96 ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000<4>1_97 ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000<5>1_98 ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000<6>1_99 ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000<7>1_100 ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000<8>1_101 ;
  wire \Inst_ad5660_serial_DAC/shift_register_mux0000<9>1_102 ;
  wire \Inst_ad5660_serial_DAC/state_FSM_FFd1_103 ;
  wire \Inst_ad5660_serial_DAC/state_FSM_FFd1-In ;
  wire \Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ;
  wire \Inst_ad5660_serial_DAC/state_FSM_FFd2-In11_106 ;
  wire \Inst_ad5660_serial_DAC/state_FSM_FFd3_107 ;
  wire \Inst_ad5660_serial_DAC/state_FSM_FFd3-In17 ;
  wire \Inst_ad5660_serial_DAC/state_FSM_FFd3-In171 ;
  wire \Inst_ad5660_serial_DAC/state_FSM_FFd3-In2_110 ;
  wire \Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ;
  wire \Inst_ad5660_serial_DAC/state_FSM_FFd4-In ;
  wire \Inst_ad5660_serial_DAC/state_FSM_FFd5_113 ;
  wire \Inst_ad5660_serial_DAC/state_FSM_FFd5-In_114 ;
  wire \Inst_ad5660_serial_DAC/state_FSM_FFd5-In1 ;
  wire \Inst_ad5660_serial_DAC/sync_o_116 ;
  wire \LTC_A/LTC_biphase_generator/Madd_ltc_frame_i_not0000<0>1_118 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_12_120 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_121 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_131_122 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_f6_123 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_f7_124 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_125 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_f5_126 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_f6_127 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_128 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f5_129 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f51 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f6_131 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_132 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_161_133 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_162_134 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_f5_135 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_f51 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_17_137 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_171_138 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_172_139 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_173_140 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_17_f5_141 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_18_142 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_181_143 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_182_144 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_19_145 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_6_f5_146 ;
  wire \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_8_f5_147 ;
  wire \LTC_A/LTC_biphase_generator/biphase_code_148 ;
  wire \LTC_A/LTC_biphase_generator/biphase_code_not0002 ;
  wire \LTC_A/LTC_biphase_generator/biphase_code_not000223_150 ;
  wire \LTC_A/LTC_biphase_generator/biphase_code_not00024_151 ;
  wire \LTC_A/LTC_biphase_generator/biphase_code_not000251_152 ;
  wire \LTC_A/LTC_biphase_generator/biphase_code_not0003 ;
  wire \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<1>_rt_158 ;
  wire \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<2>_rt_160 ;
  wire \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<3>_rt_162 ;
  wire \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<4>_rt_164 ;
  wire \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<5>_rt_166 ;
  wire \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<6>_rt_168 ;
  wire \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_xor<7>_rt_170 ;
  wire \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<0>_rt_172 ;
  wire \LTC_A/LTC_clockgenerator/N5 ;
  wire \LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ;
  wire \LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq000011_220 ;
  wire \LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq000024_221 ;
  wire \LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq000048_222 ;
  wire \LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq000061_223 ;
  wire \LTC_A/LTC_clockgenerator/ltc_bitcount_or0000 ;
  wire \LTC_A/LTC_clockgenerator/ltc_bitcount_or0000113_225 ;
  wire \LTC_A/LTC_clockgenerator/ltc_bitcount_or000014_226 ;
  wire \LTC_A/LTC_clockgenerator/ltc_clock_tick_o_227 ;
  wire \LTC_A/LTC_clockgenerator/ltc_clock_tick_o_or0000 ;
  wire \LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ;
  wire \LTC_A/LTC_clockgenerator/ltc_frame_tick_o_not0001 ;
  wire \LTC_A/LTC_clockgenerator/ltc_sync_o_231 ;
  wire \LTC_A/LTC_clockgenerator/period_count_mux0003[0] ;
  wire \LTC_A/LTC_clockgenerator/period_count_mux0003[10] ;
  wire \LTC_A/LTC_clockgenerator/period_count_mux0003<11>1 ;
  wire \LTC_A/LTC_clockgenerator/period_count_mux0003<12>1 ;
  wire \LTC_A/LTC_clockgenerator/period_count_mux0003[13] ;
  wire \LTC_A/LTC_clockgenerator/period_count_mux0003[14] ;
  wire \LTC_A/LTC_clockgenerator/period_count_mux0003[15] ;
  wire \LTC_A/LTC_clockgenerator/period_count_mux0003<1>1 ;
  wire \LTC_A/LTC_clockgenerator/period_count_mux0003<2>1 ;
  wire \LTC_A/LTC_clockgenerator/period_count_mux0003<4>1 ;
  wire \LTC_A/LTC_clockgenerator/period_count_mux0003[5] ;
  wire \LTC_A/LTC_clockgenerator/period_count_mux0003[6] ;
  wire \LTC_A/LTC_clockgenerator/period_count_mux0003[7] ;
  wire \LTC_A/LTC_clockgenerator/period_count_mux0003<8>1 ;
  wire \LTC_A/LTC_clockgenerator/period_count_mux0003<9>1 ;
  wire \LTC_A/LTC_clockgenerator/sync_i_inv ;
  wire \LTC_A/LTC_frame_count/Mcount_hour_units ;
  wire \LTC_A/LTC_frame_count/Mcount_hour_units1 ;
  wire \LTC_A/LTC_frame_count/Mcount_hour_units2 ;
  wire \LTC_A/LTC_frame_count/Mcount_hour_units3 ;
  wire \LTC_A/LTC_frame_count/Mcount_min_tens ;
  wire \LTC_A/LTC_frame_count/Mcount_min_tens1 ;
  wire \LTC_A/LTC_frame_count/Mcount_min_tens2 ;
  wire \LTC_A/LTC_frame_count/Mcount_min_units ;
  wire \LTC_A/LTC_frame_count/Mcount_min_units1 ;
  wire \LTC_A/LTC_frame_count/Mcount_min_units2 ;
  wire \LTC_A/LTC_frame_count/Mcount_min_units3 ;
  wire \LTC_A/LTC_frame_count/Mcount_sec_tens ;
  wire \LTC_A/LTC_frame_count/Mcount_sec_tens1 ;
  wire \LTC_A/LTC_frame_count/Mcount_sec_tens2 ;
  wire \LTC_A/LTC_frame_count/Mcount_sec_units ;
  wire \LTC_A/LTC_frame_count/Mcount_sec_units1 ;
  wire \LTC_A/LTC_frame_count/Mcount_sec_units2 ;
  wire \LTC_A/LTC_frame_count/Mcount_sec_units3 ;
  wire \LTC_A/LTC_frame_count/N01 ;
  wire \LTC_A/LTC_frame_count/N111 ;
  wire \LTC_A/LTC_frame_count/N12 ;
  wire \LTC_A/LTC_frame_count/N13 ;
  wire \LTC_A/LTC_frame_count/N49 ;
  wire \LTC_A/LTC_frame_count/N5 ;
  wire \LTC_A/LTC_frame_count/N50 ;
  wire \LTC_A/LTC_frame_count/N9 ;
  wire \LTC_A/LTC_frame_count/count_sec_306 ;
  wire \LTC_A/LTC_frame_count/count_sec_or0000 ;
  wire \LTC_A/LTC_frame_count/count_sec_or000032_308 ;
  wire \LTC_A/LTC_frame_count/count_sec_or000060 ;
  wire \LTC_A/LTC_frame_count/count_sec_or0000601_310 ;
  wire \LTC_A/LTC_frame_count/frame_tens_mux0002<5>127_SW1 ;
  wire \LTC_A/LTC_frame_count/frame_tens_mux0002<5>127_SW11_316 ;
  wire \LTC_A/LTC_frame_count/frame_tens_mux0002<5>15 ;
  wire \LTC_A/LTC_frame_count/frame_tens_mux0002<5>151_318 ;
  wire \LTC_A/LTC_frame_count/frame_tens_mux0002<5>152_319 ;
  wire \LTC_A/LTC_frame_count/frame_tens_mux0002<5>30_320 ;
  wire \LTC_A/LTC_frame_count/frame_tens_mux0002<5>52_321 ;
  wire \LTC_A/LTC_frame_count/frame_tens_mux0002<5>57_322 ;
  wire \LTC_A/LTC_frame_count/frame_units_mux0009<0>9_330 ;
  wire \LTC_A/LTC_frame_count/frame_units_mux0009<1>15_332 ;
  wire \LTC_A/LTC_frame_count/frame_units_mux0009<1>26_333 ;
  wire \LTC_A/LTC_frame_count/frame_units_mux0009<1>31_334 ;
  wire \LTC_A/LTC_frame_count/frame_units_mux0009<1>86_335 ;
  wire \LTC_A/LTC_frame_count/frame_units_mux0009<1>86_SW0 ;
  wire \LTC_A/LTC_frame_count/frame_units_mux0009<1>86_SW01_337 ;
  wire \LTC_A/LTC_frame_count/frame_units_mux0009<2>19_339 ;
  wire \LTC_A/LTC_frame_count/frame_units_mux0009<2>84_340 ;
  wire \LTC_A/LTC_frame_count/hour_tens_not0002_350 ;
  wire \LTC_A/LTC_frame_count/hour_units_and0000 ;
  wire \LTC_A/LTC_frame_count/hour_units_not0001 ;
  wire \LTC_A/LTC_frame_count/ltc_frame_tick_i_inv ;
  wire \LTC_A/LTC_frame_count/min_tens_and0000 ;
  wire \LTC_A/LTC_frame_count/min_tens_not0001 ;
  wire \LTC_A/LTC_frame_count/min_units_and0000 ;
  wire \LTC_A/LTC_frame_count/min_units_not0001 ;
  wire \LTC_A/LTC_frame_count/sec_tens_and0000 ;
  wire \LTC_A/LTC_frame_count/sec_tens_and0001 ;
  wire \LTC_A/LTC_frame_count/sec_tens_not0001 ;
  wire \LTC_A/LTC_frame_count/sec_units_and0000 ;
  wire \LTC_A/LTC_frame_count/sec_units_not0001 ;
  wire \LTC_A/Mcount_delay_count_low_cy<10>_rt_435 ;
  wire \LTC_A/Mcount_delay_count_low_cy<11>_rt_437 ;
  wire \LTC_A/Mcount_delay_count_low_cy<12>_rt_439 ;
  wire \LTC_A/Mcount_delay_count_low_cy<13>_rt_441 ;
  wire \LTC_A/Mcount_delay_count_low_cy<14>_rt_443 ;
  wire \LTC_A/Mcount_delay_count_low_cy<15>_rt_445 ;
  wire \LTC_A/Mcount_delay_count_low_cy<16>_rt_447 ;
  wire \LTC_A/Mcount_delay_count_low_cy<17>_rt_449 ;
  wire \LTC_A/Mcount_delay_count_low_cy<18>_rt_451 ;
  wire \LTC_A/Mcount_delay_count_low_cy<19>_rt_453 ;
  wire \LTC_A/Mcount_delay_count_low_cy<1>_rt_455 ;
  wire \LTC_A/Mcount_delay_count_low_cy<20>_rt_457 ;
  wire \LTC_A/Mcount_delay_count_low_cy<21>_rt_459 ;
  wire \LTC_A/Mcount_delay_count_low_cy<22>_rt_461 ;
  wire \LTC_A/Mcount_delay_count_low_cy<2>_rt_463 ;
  wire \LTC_A/Mcount_delay_count_low_cy<3>_rt_465 ;
  wire \LTC_A/Mcount_delay_count_low_cy<4>_rt_467 ;
  wire \LTC_A/Mcount_delay_count_low_cy<5>_rt_469 ;
  wire \LTC_A/Mcount_delay_count_low_cy<6>_rt_471 ;
  wire \LTC_A/Mcount_delay_count_low_cy<7>_rt_473 ;
  wire \LTC_A/Mcount_delay_count_low_cy<8>_rt_475 ;
  wire \LTC_A/Mcount_delay_count_low_cy<9>_rt_477 ;
  wire \LTC_A/Mcount_delay_count_low_xor<23>_rt_479 ;
  wire \LTC_A/Result<0>1 ;
  wire \LTC_A/Result<1>1 ;
  wire \LTC_A/Result<2>1 ;
  wire \LTC_A/Result<3>1 ;
  wire \LTC_A/delay_count_high_cmp_eq0000 ;
  wire \LTC_A/delay_count_low_or0000 ;
  wire \LTC_A/delay_sync_549 ;
  wire \LTC_A/delay_sync_not0001 ;
  wire \LTC_B/LTC_biphase_generator/Madd_ltc_frame_i_not0000<0>1_552 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_12_554 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_555 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_131_556 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_f6_557 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_f7_558 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_559 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_f5_560 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_f6_561 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_562 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f5_563 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f51 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f6_565 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_566 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_161_567 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_162_568 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_f5_569 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_f51 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_17_571 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_171_572 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_172_573 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_173_574 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_17_f5_575 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_18_576 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_181_577 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_182_578 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_19_579 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_6_f5_580 ;
  wire \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_8_f5_581 ;
  wire \LTC_B/LTC_biphase_generator/biphase_code_582 ;
  wire \LTC_B/LTC_biphase_generator/biphase_code_not0002 ;
  wire \LTC_B/LTC_biphase_generator/biphase_code_not000223_584 ;
  wire \LTC_B/LTC_biphase_generator/biphase_code_not00024_585 ;
  wire \LTC_B/LTC_biphase_generator/biphase_code_not000251_586 ;
  wire \LTC_B/LTC_biphase_generator/biphase_code_not0003 ;
  wire \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<1>_rt_592 ;
  wire \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<2>_rt_594 ;
  wire \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<3>_rt_596 ;
  wire \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<4>_rt_598 ;
  wire \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<5>_rt_600 ;
  wire \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<6>_rt_602 ;
  wire \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_xor<7>_rt_604 ;
  wire \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<0>_rt_606 ;
  wire \LTC_B/LTC_clockgenerator/N5 ;
  wire \LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ;
  wire \LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq000011_654 ;
  wire \LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq000024_655 ;
  wire \LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq000048_656 ;
  wire \LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq000061_657 ;
  wire \LTC_B/LTC_clockgenerator/ltc_bitcount_or0000 ;
  wire \LTC_B/LTC_clockgenerator/ltc_bitcount_or0000113_659 ;
  wire \LTC_B/LTC_clockgenerator/ltc_bitcount_or000014_660 ;
  wire \LTC_B/LTC_clockgenerator/ltc_clock_tick_o_661 ;
  wire \LTC_B/LTC_clockgenerator/ltc_clock_tick_o_or0000 ;
  wire \LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ;
  wire \LTC_B/LTC_clockgenerator/ltc_frame_tick_o_not0001 ;
  wire \LTC_B/LTC_clockgenerator/ltc_sync_o_665 ;
  wire \LTC_B/LTC_clockgenerator/period_count_mux0003[0] ;
  wire \LTC_B/LTC_clockgenerator/period_count_mux0003[10] ;
  wire \LTC_B/LTC_clockgenerator/period_count_mux0003<11>1 ;
  wire \LTC_B/LTC_clockgenerator/period_count_mux0003<12>1 ;
  wire \LTC_B/LTC_clockgenerator/period_count_mux0003[13] ;
  wire \LTC_B/LTC_clockgenerator/period_count_mux0003[14] ;
  wire \LTC_B/LTC_clockgenerator/period_count_mux0003[15] ;
  wire \LTC_B/LTC_clockgenerator/period_count_mux0003<1>1 ;
  wire \LTC_B/LTC_clockgenerator/period_count_mux0003<2>1 ;
  wire \LTC_B/LTC_clockgenerator/period_count_mux0003<4>1 ;
  wire \LTC_B/LTC_clockgenerator/period_count_mux0003[5] ;
  wire \LTC_B/LTC_clockgenerator/period_count_mux0003[6] ;
  wire \LTC_B/LTC_clockgenerator/period_count_mux0003[7] ;
  wire \LTC_B/LTC_clockgenerator/period_count_mux0003<8>1 ;
  wire \LTC_B/LTC_clockgenerator/period_count_mux0003<9>1 ;
  wire \LTC_B/LTC_clockgenerator/sync_i_inv ;
  wire \LTC_B/LTC_frame_count/Mcount_hour_units ;
  wire \LTC_B/LTC_frame_count/Mcount_hour_units1 ;
  wire \LTC_B/LTC_frame_count/Mcount_hour_units2 ;
  wire \LTC_B/LTC_frame_count/Mcount_hour_units3 ;
  wire \LTC_B/LTC_frame_count/Mcount_min_tens ;
  wire \LTC_B/LTC_frame_count/Mcount_min_tens1 ;
  wire \LTC_B/LTC_frame_count/Mcount_min_tens2 ;
  wire \LTC_B/LTC_frame_count/Mcount_min_units ;
  wire \LTC_B/LTC_frame_count/Mcount_min_units1 ;
  wire \LTC_B/LTC_frame_count/Mcount_min_units2 ;
  wire \LTC_B/LTC_frame_count/Mcount_min_units3 ;
  wire \LTC_B/LTC_frame_count/Mcount_sec_tens ;
  wire \LTC_B/LTC_frame_count/Mcount_sec_tens1 ;
  wire \LTC_B/LTC_frame_count/Mcount_sec_tens2 ;
  wire \LTC_B/LTC_frame_count/Mcount_sec_units ;
  wire \LTC_B/LTC_frame_count/Mcount_sec_units1 ;
  wire \LTC_B/LTC_frame_count/Mcount_sec_units2 ;
  wire \LTC_B/LTC_frame_count/Mcount_sec_units3 ;
  wire \LTC_B/LTC_frame_count/N01 ;
  wire \LTC_B/LTC_frame_count/N111 ;
  wire \LTC_B/LTC_frame_count/N12 ;
  wire \LTC_B/LTC_frame_count/N13 ;
  wire \LTC_B/LTC_frame_count/N49 ;
  wire \LTC_B/LTC_frame_count/N5 ;
  wire \LTC_B/LTC_frame_count/N50 ;
  wire \LTC_B/LTC_frame_count/N9 ;
  wire \LTC_B/LTC_frame_count/count_sec_740 ;
  wire \LTC_B/LTC_frame_count/count_sec_or0000 ;
  wire \LTC_B/LTC_frame_count/count_sec_or000032_742 ;
  wire \LTC_B/LTC_frame_count/count_sec_or000060 ;
  wire \LTC_B/LTC_frame_count/count_sec_or0000601_744 ;
  wire \LTC_B/LTC_frame_count/frame_tens_mux0002<5>127_SW1 ;
  wire \LTC_B/LTC_frame_count/frame_tens_mux0002<5>127_SW11_750 ;
  wire \LTC_B/LTC_frame_count/frame_tens_mux0002<5>15 ;
  wire \LTC_B/LTC_frame_count/frame_tens_mux0002<5>151_752 ;
  wire \LTC_B/LTC_frame_count/frame_tens_mux0002<5>152_753 ;
  wire \LTC_B/LTC_frame_count/frame_tens_mux0002<5>30_754 ;
  wire \LTC_B/LTC_frame_count/frame_tens_mux0002<5>52_755 ;
  wire \LTC_B/LTC_frame_count/frame_tens_mux0002<5>57_756 ;
  wire \LTC_B/LTC_frame_count/frame_units_mux0009<0>9_764 ;
  wire \LTC_B/LTC_frame_count/frame_units_mux0009<1>15_766 ;
  wire \LTC_B/LTC_frame_count/frame_units_mux0009<1>26_767 ;
  wire \LTC_B/LTC_frame_count/frame_units_mux0009<1>31_768 ;
  wire \LTC_B/LTC_frame_count/frame_units_mux0009<1>86_769 ;
  wire \LTC_B/LTC_frame_count/frame_units_mux0009<1>86_SW0 ;
  wire \LTC_B/LTC_frame_count/frame_units_mux0009<1>86_SW01_771 ;
  wire \LTC_B/LTC_frame_count/frame_units_mux0009<2>19_773 ;
  wire \LTC_B/LTC_frame_count/frame_units_mux0009<2>84_774 ;
  wire \LTC_B/LTC_frame_count/hour_tens_not0002_784 ;
  wire \LTC_B/LTC_frame_count/hour_units_and0000 ;
  wire \LTC_B/LTC_frame_count/hour_units_not0001 ;
  wire \LTC_B/LTC_frame_count/ltc_frame_tick_i_inv ;
  wire \LTC_B/LTC_frame_count/min_tens_and0000 ;
  wire \LTC_B/LTC_frame_count/min_tens_not0001 ;
  wire \LTC_B/LTC_frame_count/min_units_and0000 ;
  wire \LTC_B/LTC_frame_count/min_units_not0001 ;
  wire \LTC_B/LTC_frame_count/sec_tens_and0000 ;
  wire \LTC_B/LTC_frame_count/sec_tens_and0001 ;
  wire \LTC_B/LTC_frame_count/sec_tens_not0001 ;
  wire \LTC_B/LTC_frame_count/sec_units_and0000 ;
  wire \LTC_B/LTC_frame_count/sec_units_not0001 ;
  wire \LTC_B/Mcount_delay_count_low_cy<10>_rt_869 ;
  wire \LTC_B/Mcount_delay_count_low_cy<11>_rt_871 ;
  wire \LTC_B/Mcount_delay_count_low_cy<12>_rt_873 ;
  wire \LTC_B/Mcount_delay_count_low_cy<13>_rt_875 ;
  wire \LTC_B/Mcount_delay_count_low_cy<14>_rt_877 ;
  wire \LTC_B/Mcount_delay_count_low_cy<15>_rt_879 ;
  wire \LTC_B/Mcount_delay_count_low_cy<16>_rt_881 ;
  wire \LTC_B/Mcount_delay_count_low_cy<17>_rt_883 ;
  wire \LTC_B/Mcount_delay_count_low_cy<18>_rt_885 ;
  wire \LTC_B/Mcount_delay_count_low_cy<19>_rt_887 ;
  wire \LTC_B/Mcount_delay_count_low_cy<1>_rt_889 ;
  wire \LTC_B/Mcount_delay_count_low_cy<20>_rt_891 ;
  wire \LTC_B/Mcount_delay_count_low_cy<21>_rt_893 ;
  wire \LTC_B/Mcount_delay_count_low_cy<22>_rt_895 ;
  wire \LTC_B/Mcount_delay_count_low_cy<2>_rt_897 ;
  wire \LTC_B/Mcount_delay_count_low_cy<3>_rt_899 ;
  wire \LTC_B/Mcount_delay_count_low_cy<4>_rt_901 ;
  wire \LTC_B/Mcount_delay_count_low_cy<5>_rt_903 ;
  wire \LTC_B/Mcount_delay_count_low_cy<6>_rt_905 ;
  wire \LTC_B/Mcount_delay_count_low_cy<7>_rt_907 ;
  wire \LTC_B/Mcount_delay_count_low_cy<8>_rt_909 ;
  wire \LTC_B/Mcount_delay_count_low_cy<9>_rt_911 ;
  wire \LTC_B/Mcount_delay_count_low_xor<23>_rt_913 ;
  wire \LTC_B/Result<0>1 ;
  wire \LTC_B/Result<1>1 ;
  wire \LTC_B/Result<2>1 ;
  wire \LTC_B/Result<3>1 ;
  wire \LTC_B/delay_count_high_cmp_eq0000 ;
  wire \LTC_B/delay_count_low_or0000 ;
  wire \LTC_B/delay_sync_983 ;
  wire \LTC_B/delay_sync_not0001 ;
  wire \LTC_setup[0] ;
  wire \LTC_setup[1] ;
  wire \LTC_setup[2] ;
  wire \LTC_setup[4] ;
  wire \LTC_setup[5] ;
  wire \LTC_setup[6] ;
  wire \Mcompar_pps_status_cmp_gt0000_cy<11>_rt_1113 ;
  wire \Mcompar_pps_status_cmp_gt0000_cy<2>_rt_1116 ;
  wire \Mcompar_pps_status_cmp_gt0000_cy<9>_rt_1124 ;
  wire \Mcompar_pps_status_cmp_gt0000_lut[0] ;
  wire \Mcompar_pps_status_cmp_gt0000_lut[10] ;
  wire \Mcompar_pps_status_cmp_gt0000_lut[1] ;
  wire \Mcompar_pps_status_cmp_gt0000_lut[3] ;
  wire \Mcompar_pps_status_cmp_gt0000_lut[4] ;
  wire \Mcompar_pps_status_cmp_gt0000_lut[5] ;
  wire \Mcompar_pps_status_cmp_gt0000_lut[6] ;
  wire \Mcompar_pps_status_cmp_gt0000_lut[7] ;
  wire \Mcompar_pps_status_cmp_gt0000_lut[8] ;
  wire \Mcount_count_cy<0>_rt_1135 ;
  wire \Mcount_pps_status_count_cy<10>_rt_1177 ;
  wire \Mcount_pps_status_count_cy<11>_rt_1179 ;
  wire \Mcount_pps_status_count_cy<12>_rt_1181 ;
  wire \Mcount_pps_status_count_cy<13>_rt_1183 ;
  wire \Mcount_pps_status_count_cy<14>_rt_1185 ;
  wire \Mcount_pps_status_count_cy<15>_rt_1187 ;
  wire \Mcount_pps_status_count_cy<16>_rt_1189 ;
  wire \Mcount_pps_status_count_cy<17>_rt_1191 ;
  wire \Mcount_pps_status_count_cy<18>_rt_1193 ;
  wire \Mcount_pps_status_count_cy<19>_rt_1195 ;
  wire \Mcount_pps_status_count_cy<1>_rt_1197 ;
  wire \Mcount_pps_status_count_cy<20>_rt_1199 ;
  wire \Mcount_pps_status_count_cy<21>_rt_1201 ;
  wire \Mcount_pps_status_count_cy<22>_rt_1203 ;
  wire \Mcount_pps_status_count_cy<2>_rt_1205 ;
  wire \Mcount_pps_status_count_cy<3>_rt_1207 ;
  wire \Mcount_pps_status_count_cy<4>_rt_1209 ;
  wire \Mcount_pps_status_count_cy<5>_rt_1211 ;
  wire \Mcount_pps_status_count_cy<6>_rt_1213 ;
  wire \Mcount_pps_status_count_cy<7>_rt_1215 ;
  wire \Mcount_pps_status_count_cy<8>_rt_1217 ;
  wire \Mcount_pps_status_count_cy<9>_rt_1219 ;
  wire \Mcount_pps_status_count_xor<23>_rt_1221 ;
  wire N1;
  wire N10;
  wire N101;
  wire N102;
  wire N104;
  wire N105;
  wire N107;
  wire N108;
  wire N110;
  wire N111;
  wire N113;
  wire N114;
  wire N12;
  wire N123;
  wire N125;
  wire N133;
  wire N138;
  wire N14;
  wire N141;
  wire N142;
  wire N15;
  wire N152;
  wire N154;
  wire N156;
  wire N161;
  wire N17;
  wire N176;
  wire N179;
  wire N18;
  wire N181;
  wire N182;
  wire N185;
  wire N188;
  wire N19;
  wire N191;
  wire N194;
  wire N197;
  wire N199;
  wire N2;
  wire N20;
  wire N201;
  wire N2011;
  wire N203;
  wire N205;
  wire N207;
  wire N209;
  wire N21;
  wire N211;
  wire N213;
  wire N215;
  wire N217;
  wire N219;
  wire N220;
  wire N221;
  wire N223;
  wire N226;
  wire N229;
  wire N232;
  wire N235;
  wire N238;
  wire N24;
  wire N241;
  wire N244;
  wire N246;
  wire N248;
  wire N249;
  wire N251;
  wire N252;
  wire N254;
  wire N255;
  wire N257;
  wire N258;
  wire N26;
  wire N260;
  wire N261;
  wire N2611;
  wire N263;
  wire N264;
  wire N266;
  wire N267;
  wire N269;
  wire N270;
  wire N272;
  wire N274;
  wire N28;
  wire N284;
  wire N286;
  wire N288;
  wire N290;
  wire N292;
  wire N294;
  wire N296;
  wire N298;
  wire N30;
  wire N300;
  wire N302;
  wire N304;
  wire N306;
  wire N308;
  wire N310;
  wire N312;
  wire N316;
  wire N318;
  wire N320;
  wire N321;
  wire N322;
  wire N324;
  wire N326;
  wire N328;
  wire N330;
  wire N332;
  wire N334;
  wire N336;
  wire N338;
  wire N340;
  wire N342;
  wire N344;
  wire N346;
  wire N348;
  wire N350;
  wire N352;
  wire N354;
  wire N356;
  wire N358;
  wire N36;
  wire N360;
  wire N362;
  wire N364;
  wire N366;
  wire N367;
  wire N368;
  wire N369;
  wire N370;
  wire N371;
  wire N372;
  wire N373;
  wire N374;
  wire N375;
  wire N376;
  wire N377;
  wire N378;
  wire N379;
  wire N380;
  wire N381;
  wire N382;
  wire N383;
  wire N384;
  wire N385;
  wire N386;
  wire N387;
  wire N388;
  wire N389;
  wire N390;
  wire N391;
  wire N392;
  wire N393;
  wire N394;
  wire N395;
  wire N396;
  wire N4;
  wire N6;
  wire N63;
  wire N64;
  wire N74;
  wire N76;
  wire N8;
  wire N84;
  wire N85;
  wire N86;
  wire N87;
  wire N92;
  wire N93;
  wire N94;
  wire N96;
  wire N98;
  wire N991;
  wire NTP_RX_OBUF_1400;
  wire \OCXO_cv_word[0] ;
  wire \OCXO_cv_word[10] ;
  wire \OCXO_cv_word[12] ;
  wire \OCXO_cv_word[13] ;
  wire \OCXO_cv_word[14] ;
  wire \OCXO_cv_word[15] ;
  wire \OCXO_cv_word[2] ;
  wire \OCXO_cv_word[3] ;
  wire \OCXO_cv_word[4] ;
  wire \OCXO_cv_word[6] ;
  wire \OCXO_cv_word[8] ;
  wire \OCXO_cv_word[9] ;
  wire \OCXO_pll/phase_detect/Mcount_phase_counter_cy<0>_rt_1414 ;
  wire \OCXO_pll/phase_detect/Mcount_phase_counter_val ;
  wire \OCXO_pll/phase_detect/int_pps_tick_1494 ;
  wire \OCXO_pll/phase_detect/int_pps_tick_cmp_eq0000 ;
  wire \OCXO_pll/phase_detect/int_pps_tick_cmp_eq000017 ;
  wire \OCXO_pll/phase_detect/int_pps_tick_cmp_eq0000171_1497 ;
  wire \OCXO_pll/phase_detect/int_pps_tick_cmp_eq000032_1508 ;
  wire \OCXO_pll/phase_detect/phase_counter_and0000 ;
  wire \OCXO_pll/phase_detect/phase_counter_and0000_inv ;
  wire \OCXO_pll/phase_detect/phase_counter_cmp_eq0000 ;
  wire \OCXO_pll/phase_detect/phase_counter_cmp_eq000011 ;
  wire \OCXO_pll/phase_detect/phase_counter_cmp_eq0000111_1540 ;
  wire \OCXO_pll/phase_detect/phase_counter_cmp_eq00004_1541 ;
  wire \OCXO_pll/phase_detect/phase_counter_cst ;
  wire \OCXO_pll/phase_detect/update_tick_1570 ;
  wire \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<35>_rt_1641 ;
  wire \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<10>_rt_1698 ;
  wire \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<12>_rt_1701 ;
  wire \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<2>_rt_1708 ;
  wire \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<4>_rt_1711 ;
  wire \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<8>_rt_1716 ;
  wire \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[0] ;
  wire \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[11] ;
  wire \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[13] ;
  wire \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[14] ;
  wire \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[15] ;
  wire \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[16] ;
  wire \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[1] ;
  wire \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[3] ;
  wire \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[5] ;
  wire \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[6] ;
  wire \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[7] ;
  wire \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[9] ;
  wire \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<1>_rt_1736 ;
  wire \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<5>_rt_1741 ;
  wire \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<7>_rt_1744 ;
  wire \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<9>_rt_1747 ;
  wire \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[0] ;
  wire \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[10] ;
  wire \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[11] ;
  wire \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[12] ;
  wire \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[13] ;
  wire \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[2] ;
  wire \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[3] ;
  wire \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[4] ;
  wire \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[6] ;
  wire \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[8] ;
  wire \OCXO_pll/phase_loop/Mmux_integrator_limit1011_1758 ;
  wire \OCXO_pll/phase_loop/Mmux_integrator_limit1011_1_1759 ;
  wire \OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ;
  wire \OCXO_pll/phase_loop/N01 ;
  wire \OCXO_pll/phase_loop/N11 ;
  wire \OCXO_pll/phase_loop/N2 ;
  wire \OCXO_pll/phase_loop/dac_word_o[13] ;
  wire \OCXO_pll/phase_loop/dac_word_o<13>1_1765 ;
  wire \OCXO_pll/phase_loop/dac_word_o[15] ;
  wire \OCXO_pll/phase_loop/dac_word_o<15>1_1767 ;
  wire \OCXO_pll/phase_loop/dac_word_o[2] ;
  wire \OCXO_pll/phase_loop/dac_word_o<2>1_1769 ;
  wire \OCXO_pll/phase_loop/dac_word_o[4] ;
  wire \OCXO_pll/phase_loop/dac_word_o<4>1_1771 ;
  wire \OCXO_pll/phase_loop/dac_word_o[6] ;
  wire \OCXO_pll/phase_loop/dac_word_o<6>1_1773 ;
  wire \OCXO_pll/phase_loop/integrator_limit<0>_mand_1846 ;
  wire \OCXO_pll/phase_loop/integrator_limit<0>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit[10] ;
  wire \OCXO_pll/phase_loop/integrator_limit<11>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<11>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_limit[12] ;
  wire \OCXO_pll/phase_loop/integrator_limit[13] ;
  wire \OCXO_pll/phase_loop/integrator_limit[14] ;
  wire \OCXO_pll/phase_loop/integrator_limit[15] ;
  wire \OCXO_pll/phase_loop/integrator_limit<16>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<16>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_limit<17>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<17>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_limit<18>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<18>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_limit[19] ;
  wire \OCXO_pll/phase_loop/integrator_limit<1>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<20>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<20>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_limit[21] ;
  wire \OCXO_pll/phase_loop/integrator_limit<22>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<22>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_limit[23] ;
  wire \OCXO_pll/phase_loop/integrator_limit<24>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<24>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_limit<25>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<25>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_limit<26>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<26>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_limit<27>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<27>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_limit<28>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<28>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_limit<29>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<29>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_limit<2>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<30>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<30>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_limit<31>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<31>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_limit<32>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<32>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_limit<33>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<33>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_limit<34>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<34>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_limit<3>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<4>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<5>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<6>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<7>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit[8] ;
  wire \OCXO_pll/phase_loop/integrator_limit<9>_mand1 ;
  wire \OCXO_pll/phase_loop/integrator_limit<9>_mand3 ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[11] ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[16] ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[17] ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[18] ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[20] ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[22] ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[24] ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[25] ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[26] ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[27] ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[28] ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[29] ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[30] ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[31] ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[32] ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[33] ;
  wire \OCXO_pll/phase_loop/integrator_mux0003<33>1_1952 ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[34] ;
  wire \OCXO_pll/phase_loop/integrator_mux0003[9] ;
  wire \OCXO_pll/phase_loop/integrator_not0002 ;
  wire \OCXO_pll/phase_loop/out_sum_mux0001[10] ;
  wire \OCXO_pll/phase_loop/out_sum_mux0001[12] ;
  wire \OCXO_pll/phase_loop/out_sum_mux0001[14] ;
  wire \OCXO_pll/phase_loop/out_sum_mux0001[16] ;
  wire \OCXO_pll/phase_loop/out_sum_mux0001[17] ;
  wire \OCXO_pll/phase_loop/out_sum_mux0001[18] ;
  wire \OCXO_pll/phase_loop/out_sum_mux0001[19] ;
  wire \OCXO_pll/phase_loop/out_sum_mux0001[20] ;
  wire \OCXO_pll/phase_loop/out_sum_mux0001[21] ;
  wire \OCXO_pll/phase_loop/out_sum_mux0001[22] ;
  wire \OCXO_pll/phase_loop/out_sum_mux0001[23] ;
  wire \OCXO_pll/phase_loop/out_sum_mux0001[24] ;
  wire \OCXO_pll/phase_loop/out_sum_mux0001[3] ;
  wire \OCXO_pll/phase_loop/out_sum_mux0001[8] ;
  wire \OCXO_pll/phase_loop/out_sum_mux0001[9] ;
  wire \PLL_clock_gen/CLKFX_BUF ;
  wire PPS_i_IBUF_2001;
  wire \Result<0>1 ;
  wire \Result<10>1 ;
  wire \Result<11>1 ;
  wire \Result<12>1 ;
  wire \Result<13>1 ;
  wire \Result<14>1 ;
  wire \Result<15>1 ;
  wire \Result<16>1 ;
  wire \Result<17>1 ;
  wire \Result<18>1 ;
  wire \Result<19>1 ;
  wire \Result<1>1 ;
  wire \Result<20>1 ;
  wire \Result<2>1 ;
  wire \Result<3>1 ;
  wire \Result<4>1 ;
  wire \Result<5>1 ;
  wire \Result<6>1 ;
  wire \Result<7>1 ;
  wire \Result<8>1 ;
  wire \Result<9>1 ;
  wire SCL_i_IBUF_2049;
  wire \VCXO_pll/Mcount_ref_clk_count_cy<1>_rt_2054 ;
  wire \VCXO_pll/Mcount_ref_clk_count_cy<2>_rt_2056 ;
  wire \VCXO_pll/Mcount_ref_clk_count_cy<3>_rt_2058 ;
  wire \VCXO_pll/Mcount_ref_clk_count_cy<4>_rt_2060 ;
  wire \VCXO_pll/Mcount_ref_clk_count_cy<5>_rt_2062 ;
  wire \VCXO_pll/Mcount_ref_clk_count_cy<6>_rt_2064 ;
  wire \VCXO_pll/Mcount_ref_clk_count_cy<7>_rt_2066 ;
  wire \VCXO_pll/Mcount_ref_clk_count_xor<8>_rt_2068 ;
  wire \VCXO_pll/phase_detector/Mcount_phase_counter_cy<0>_rt_2237 ;
  wire \VCXO_pll/phase_detector/phase_counter_cmp_eq0000 ;
  wire \VCXO_pll/phase_detector/phase_counter_cmp_eq000018_2276 ;
  wire \VCXO_pll/phase_detector/phase_counter_cmp_eq00007_2277 ;
  wire \VCXO_pll/phase_detector/update_tick_2288 ;
  wire \VCXO_pll/phase_detector/update_tick_not0001 ;
  wire \VCXO_pll/pps_148_tick_o_2290 ;
  wire \VCXO_pll/ref_clk_2291 ;
  wire \VCXO_pll/ref_clk_count_cmp_eq0000 ;
  wire \VCXO_pll/ref_clk_count_cmp_eq000019_2302 ;
  wire \VCXO_pll/ref_clk_count_cmp_eq00009 ;
  wire \VCXO_pll/ref_clk_count_cmp_eq000091_2304 ;
  wire \VCXO_pll/ref_clk_not0001 ;
  wire \VCXO_pll/resync_148_period_2306 ;
  wire \VCXO_pll/resync_148_period_and0000 ;
  wire \VCXO_pll/resync_148_period_inv ;
  wire \VCXO_pll/update_dac_o_2309 ;
  wire clk_100;
  wire clk_10_i_IBUFG_2312;
  wire clk_10_i_IBUFG1;
  wire clk_148;
  wire clk_1481;
  wire \communication/Mcount_address ;
  wire \communication/Mcount_address1 ;
  wire \communication/Mcount_address2 ;
  wire \communication/Mcount_address3 ;
  wire \communication/Mcount_address4 ;
  wire \communication/Mcount_address5 ;
  wire \communication/Mcount_address6 ;
  wire \communication/Mcount_address7 ;
  wire \communication/Mshreg_update_line_0_2341 ;
  wire \communication/state_FSM_FFd1_2366 ;
  wire \communication/state_FSM_FFd1-In1 ;
  wire \communication/state_FSM_FFd1-In11_2368 ;
  wire \communication/state_FSM_FFd1-In12_2369 ;
  wire \communication/state_FSM_FFd2_2370 ;
  wire \communication/state_FSM_FFd2-In ;
  wire \communication/state_and0000 ;
  wire \communication/state_cmp_eq0002_inv ;
  wire \communication/state_cmp_eq0005 ;
  wire \communication/update_register_o_2379 ;
  wire \dac_OCXO/Mcount_bit_count_cy<0>_rt_2402 ;
  wire \dac_OCXO/Mcount_bit_count_val ;
  wire \dac_OCXO/bit_count_not000111 ;
  wire \dac_OCXO/bit_count_not0001111_2434 ;
  wire \dac_OCXO/bit_count_not00014_2435 ;
  wire \dac_OCXO/bit_count_not0001_inv ;
  wire \dac_OCXO/bit_tick_2437 ;
  wire \dac_OCXO/bit_tick_or0000 ;
  wire \dac_OCXO/shift_reg_not0001 ;
  wire \dac_OCXO/zsync_2488 ;
  wire \f_gen/Msub_count_addsub0000_cy<0>_rt_2494 ;
  wire \f_gen/count_mux0003[11] ;
  wire \f_gen/count_mux0003[12] ;
  wire \f_gen/count_mux0003[13] ;
  wire \f_gen/count_mux0003[14] ;
  wire \f_gen/count_mux0003[16] ;
  wire \f_gen/count_mux0003[17] ;
  wire \f_gen/count_mux0003[18] ;
  wire \f_gen/count_mux0003[19] ;
  wire \f_gen/count_mux0003[20] ;
  wire \f_gen/count_mux0003[21] ;
  wire \f_gen/count_mux0003[22] ;
  wire \f_gen/count_mux0003[23] ;
  wire \f_gen/count_mux0003[24] ;
  wire \f_gen/count_mux0003[2] ;
  wire \f_gen/count_mux0003[3] ;
  wire \f_gen/count_mux0003[4] ;
  wire \f_gen/count_mux0003[5] ;
  wire \f_gen/count_mux0003[6] ;
  wire \f_gen/count_mux0003[8] ;
  wire \f_gen/pulse_state_2639 ;
  wire \f_gen/pulse_state_cmp_eq0000 ;
  wire \f_gen/pulse_state_mux0001 ;
  wire genlock_register_0_0_2657;
  wire genlock_register_0_1_2658;
  wire genlock_register_0_2_2659;
  wire genlock_register_0_3_2660;
  wire genlock_register_0_4_2661;
  wire genlock_register_0_5_2662;
  wire genlock_register_0_6_2663;
  wire genlock_register_0_7_2664;
  wire genlock_register_0_and0000;
  wire genlock_register_10_0_2666;
  wire genlock_register_10_1_2667;
  wire genlock_register_10_2_2668;
  wire genlock_register_10_3_2669;
  wire genlock_register_10_4_2670;
  wire genlock_register_10_5_2671;
  wire genlock_register_10_6_2672;
  wire genlock_register_10_7_2673;
  wire genlock_register_10_and0000;
  wire genlock_register_11_0_2675;
  wire genlock_register_11_1_2676;
  wire genlock_register_11_2_2677;
  wire genlock_register_11_3_2678;
  wire genlock_register_11_4_2679;
  wire genlock_register_11_5_2680;
  wire genlock_register_11_6_2681;
  wire genlock_register_11_7_2682;
  wire genlock_register_11_and0000;
  wire genlock_register_12_0_2684;
  wire genlock_register_12_1_2685;
  wire genlock_register_12_2_2686;
  wire genlock_register_12_3_2687;
  wire genlock_register_12_4_2688;
  wire genlock_register_12_5_2689;
  wire genlock_register_12_6_2690;
  wire genlock_register_12_7_2691;
  wire genlock_register_12_and0000;
  wire genlock_register_13_0_2693;
  wire genlock_register_13_1_2694;
  wire genlock_register_13_2_2695;
  wire genlock_register_13_3_2696;
  wire genlock_register_13_4_2697;
  wire genlock_register_13_5_2698;
  wire genlock_register_13_6_2699;
  wire genlock_register_13_7_2700;
  wire genlock_register_13_and0000;
  wire genlock_register_14_0_2702;
  wire genlock_register_14_1_2703;
  wire genlock_register_14_2_2704;
  wire genlock_register_14_3_2705;
  wire genlock_register_14_4_2706;
  wire genlock_register_14_5_2707;
  wire genlock_register_14_6_2708;
  wire genlock_register_14_7_2709;
  wire genlock_register_14_and0000;
  wire genlock_register_15_0_2711;
  wire genlock_register_15_1_2712;
  wire genlock_register_15_2_2713;
  wire genlock_register_15_3_2714;
  wire genlock_register_15_4_2715;
  wire genlock_register_15_5_2716;
  wire genlock_register_15_6_2717;
  wire genlock_register_15_7_2718;
  wire genlock_register_15_and0000;
  wire genlock_register_16_0_2720;
  wire genlock_register_16_1_2721;
  wire genlock_register_16_2_2722;
  wire genlock_register_16_3_2723;
  wire genlock_register_16_4_2724;
  wire genlock_register_16_5_2725;
  wire genlock_register_16_6_2726;
  wire genlock_register_16_7_2727;
  wire genlock_register_16_and0000;
  wire genlock_register_17_0_2729;
  wire genlock_register_17_1_2730;
  wire genlock_register_17_2_2731;
  wire genlock_register_17_3_2732;
  wire genlock_register_17_4_2733;
  wire genlock_register_17_5_2734;
  wire genlock_register_17_6_2735;
  wire genlock_register_17_7_2736;
  wire genlock_register_17_and0000;
  wire genlock_register_18_0_2738;
  wire genlock_register_18_1_2739;
  wire genlock_register_18_2_2740;
  wire genlock_register_18_3_2741;
  wire genlock_register_18_4_2742;
  wire genlock_register_18_5_2743;
  wire genlock_register_18_6_2744;
  wire genlock_register_18_7_2745;
  wire genlock_register_18_and0000;
  wire genlock_register_19_0_2747;
  wire genlock_register_19_1_2748;
  wire genlock_register_19_2_2749;
  wire genlock_register_19_3_2750;
  wire genlock_register_19_4_2751;
  wire genlock_register_19_5_2752;
  wire genlock_register_19_6_2753;
  wire genlock_register_19_7_2754;
  wire genlock_register_19_and0000;
  wire genlock_register_1_0_2756;
  wire genlock_register_1_1_2757;
  wire genlock_register_1_2_2758;
  wire genlock_register_1_3_2759;
  wire genlock_register_1_4_2760;
  wire genlock_register_1_5_2761;
  wire genlock_register_1_6_2762;
  wire genlock_register_1_7_2763;
  wire genlock_register_1_and0000;
  wire genlock_register_20_0_2765;
  wire genlock_register_20_1_2766;
  wire genlock_register_20_2_2767;
  wire genlock_register_20_3_2768;
  wire genlock_register_20_4_2769;
  wire genlock_register_20_5_2770;
  wire genlock_register_20_6_2771;
  wire genlock_register_20_7_2772;
  wire genlock_register_20_and0000;
  wire genlock_register_21_0_2774;
  wire genlock_register_21_1_2775;
  wire genlock_register_21_2_2776;
  wire genlock_register_21_3_2777;
  wire genlock_register_21_4_2778;
  wire genlock_register_21_5_2779;
  wire genlock_register_21_6_2780;
  wire genlock_register_21_7_2781;
  wire genlock_register_21_and0000;
  wire genlock_register_22_0_2783;
  wire genlock_register_22_1_2784;
  wire genlock_register_22_2_2785;
  wire genlock_register_22_3_2786;
  wire genlock_register_22_4_2787;
  wire genlock_register_22_5_2788;
  wire genlock_register_22_6_2789;
  wire genlock_register_22_7_2790;
  wire genlock_register_22_and0000;
  wire genlock_register_2_0_2792;
  wire genlock_register_2_1_2793;
  wire genlock_register_2_2_2794;
  wire genlock_register_2_3_2795;
  wire genlock_register_2_4_2796;
  wire genlock_register_2_5_2797;
  wire genlock_register_2_6_2798;
  wire genlock_register_2_7_2799;
  wire genlock_register_2_and0000;
  wire genlock_register_3_0_2801;
  wire genlock_register_3_1_2802;
  wire genlock_register_3_2_2803;
  wire genlock_register_3_3_2804;
  wire genlock_register_3_4_2805;
  wire genlock_register_3_5_2806;
  wire genlock_register_3_6_2807;
  wire genlock_register_3_7_2808;
  wire genlock_register_3_and0000;
  wire genlock_register_4_0_2810;
  wire genlock_register_4_1_2811;
  wire genlock_register_4_2_2812;
  wire genlock_register_4_3_2813;
  wire genlock_register_4_4_2814;
  wire genlock_register_4_5_2815;
  wire genlock_register_4_6_2816;
  wire genlock_register_4_7_2817;
  wire genlock_register_4_and0000;
  wire genlock_register_5_0_2819;
  wire genlock_register_5_1_2820;
  wire genlock_register_5_2_2821;
  wire genlock_register_5_3_2822;
  wire genlock_register_5_4_2823;
  wire genlock_register_5_5_2824;
  wire genlock_register_5_6_2825;
  wire genlock_register_5_7_2826;
  wire genlock_register_5_and0000;
  wire genlock_register_6_0_2828;
  wire genlock_register_6_1_2829;
  wire genlock_register_6_2_2830;
  wire genlock_register_6_3_2831;
  wire genlock_register_6_4_2832;
  wire genlock_register_6_5_2833;
  wire genlock_register_6_6_2834;
  wire genlock_register_6_7_2835;
  wire genlock_register_6_and0000;
  wire genlock_register_7_0_2837;
  wire genlock_register_7_1_2838;
  wire genlock_register_7_2_2839;
  wire genlock_register_7_3_2840;
  wire genlock_register_7_4_2841;
  wire genlock_register_7_5_2842;
  wire genlock_register_7_6_2843;
  wire genlock_register_7_7_2844;
  wire genlock_register_7_and0000;
  wire genlock_register_8_0_2846;
  wire genlock_register_8_1_2847;
  wire genlock_register_8_2_2848;
  wire genlock_register_8_3_2849;
  wire genlock_register_8_4_2850;
  wire genlock_register_8_5_2851;
  wire genlock_register_8_6_2852;
  wire genlock_register_8_7_2853;
  wire genlock_register_8_and0000;
  wire genlock_register_9_0_2855;
  wire genlock_register_9_1_2856;
  wire genlock_register_9_2_2857;
  wire genlock_register_9_3_2858;
  wire genlock_register_9_4_2859;
  wire genlock_register_9_5_2860;
  wire genlock_register_9_6_2861;
  wire genlock_register_9_7_2862;
  wire genlock_register_9_and0000;
  wire \h_gen/Msub_count_addsub0000_cy<0>_rt_2866 ;
  wire \h_gen/count_mux0003[11] ;
  wire \h_gen/count_mux0003[12] ;
  wire \h_gen/count_mux0003[1] ;
  wire \h_gen/count_mux0003[2] ;
  wire \h_gen/count_mux0003[3] ;
  wire \h_gen/count_mux0003[4] ;
  wire \h_gen/count_mux0003[6] ;
  wire \h_gen/count_mux0003[7] ;
  wire \h_gen/pulse_state_3000 ;
  wire \h_gen/pulse_state_cmp_eq0000 ;
  wire \h_gen/pulse_state_mux0001 ;
  wire kvant_pps_3018;
  wire kvant_pps_not0001;
  wire led1_o_OBUF_3021;
  wire longer_tick_3023;
  wire longer_tick_not0001;
  wire longer_tick_not0001_inv;
  wire pps_status_3039;
  wire pps_status_cmp_gt0000;
  wire pps_status_count_or0000;
  wire rst_i_IBUF_3067;
  wire \serial_interface/Msub_time_out_addsub0000_cy<0>_rt_3071 ;
  wire \serial_interface/RW_dir_3113 ;
  wire \serial_interface/RW_dir_inv ;
  wire \serial_interface/SDA_out_3119 ;
  wire \serial_interface/SDA_out_mux00030_3120 ;
  wire \serial_interface/SDA_out_mux0003113_3121 ;
  wire \serial_interface/SDA_out_mux000313_3122 ;
  wire \serial_interface/SDA_out_mux0003144 ;
  wire \serial_interface/SDA_out_mux000328_3124 ;
  wire \serial_interface/SDA_out_mux000385_3125 ;
  wire \serial_interface/_mux0000 ;
  wire \serial_interface/bit_count_not0001 ;
  wire \serial_interface/bit_count_or0000 ;
  wire \serial_interface/bit_count_or0002 ;
  wire \serial_interface/command_mux0000<0>1_3142 ;
  wire \serial_interface/command_mux0000<0>2_3143 ;
  wire \serial_interface/command_mux0000<2>1_3146 ;
  wire \serial_interface/command_mux0000<2>2_3147 ;
  wire \serial_interface/recieve_byte_0_mux0001_3149 ;
  wire \serial_interface/recieve_byte_1_mux0001_3151 ;
  wire \serial_interface/recieve_byte_2_mux0001_3153 ;
  wire \serial_interface/recieve_byte_3_mux0001_3155 ;
  wire \serial_interface/recieve_byte_4_mux0001_3157 ;
  wire \serial_interface/recieve_byte_5_mux0001_3159 ;
  wire \serial_interface/recieve_byte_6_cmp_eq0000 ;
  wire \serial_interface/recieve_byte_6_mux0001_3162 ;
  wire \serial_interface/recieve_byte_7_mux0001_3164 ;
  wire \serial_interface/send_byte_mux0001<0>0_3181 ;
  wire \serial_interface/send_byte_mux0001<0>10_3182 ;
  wire \serial_interface/send_byte_mux0001<0>106_3183 ;
  wire \serial_interface/send_byte_mux0001<0>117_3184 ;
  wire \serial_interface/send_byte_mux0001<0>131_3185 ;
  wire \serial_interface/send_byte_mux0001<0>139_3186 ;
  wire \serial_interface/send_byte_mux0001<0>150_3187 ;
  wire \serial_interface/send_byte_mux0001<0>185_3188 ;
  wire \serial_interface/send_byte_mux0001<0>207 ;
  wire \serial_interface/send_byte_mux0001<0>22_3190 ;
  wire \serial_interface/send_byte_mux0001<0>27_3191 ;
  wire \serial_interface/send_byte_mux0001<0>37_3192 ;
  wire \serial_interface/send_byte_mux0001<0>5_3193 ;
  wire \serial_interface/send_byte_mux0001<0>50_3194 ;
  wire \serial_interface/send_byte_mux0001<0>55_3195 ;
  wire \serial_interface/send_byte_mux0001<0>67_3196 ;
  wire \serial_interface/send_byte_mux0001<0>72_3197 ;
  wire \serial_interface/send_byte_mux0001<0>82_3198 ;
  wire \serial_interface/send_byte_mux0001<1>0_3199 ;
  wire \serial_interface/send_byte_mux0001<1>104_3200 ;
  wire \serial_interface/send_byte_mux0001<1>117_3201 ;
  wire \serial_interface/send_byte_mux0001<1>122_3202 ;
  wire \serial_interface/send_byte_mux0001<1>134_3203 ;
  wire \serial_interface/send_byte_mux0001<1>139_3204 ;
  wire \serial_interface/send_byte_mux0001<1>14_3205 ;
  wire \serial_interface/send_byte_mux0001<1>149_3206 ;
  wire \serial_interface/send_byte_mux0001<1>171_3207 ;
  wire \serial_interface/send_byte_mux0001<1>19_3208 ;
  wire \serial_interface/send_byte_mux0001<1>192 ;
  wire \serial_interface/send_byte_mux0001<1>1921_3210 ;
  wire \serial_interface/send_byte_mux0001<1>1922_3211 ;
  wire \serial_interface/send_byte_mux0001<1>216 ;
  wire \serial_interface/send_byte_mux0001<1>2161 ;
  wire \serial_interface/send_byte_mux0001<1>29_3214 ;
  wire \serial_interface/send_byte_mux0001<1>43_3215 ;
  wire \serial_interface/send_byte_mux0001<1>48_3216 ;
  wire \serial_interface/send_byte_mux0001<1>49_3217 ;
  wire \serial_interface/send_byte_mux0001<1>5_3218 ;
  wire \serial_interface/send_byte_mux0001<1>72_3219 ;
  wire \serial_interface/send_byte_mux0001<1>77_3220 ;
  wire \serial_interface/send_byte_mux0001<1>89_3221 ;
  wire \serial_interface/send_byte_mux0001<1>94_3222 ;
  wire \serial_interface/send_byte_mux0001<2>0_3223 ;
  wire \serial_interface/send_byte_mux0001<2>10_3224 ;
  wire \serial_interface/send_byte_mux0001<2>106_3225 ;
  wire \serial_interface/send_byte_mux0001<2>117_3226 ;
  wire \serial_interface/send_byte_mux0001<2>131_3227 ;
  wire \serial_interface/send_byte_mux0001<2>136_3228 ;
  wire \serial_interface/send_byte_mux0001<2>146_3229 ;
  wire \serial_interface/send_byte_mux0001<2>181_3230 ;
  wire \serial_interface/send_byte_mux0001<2>203 ;
  wire \serial_interface/send_byte_mux0001<2>22_3232 ;
  wire \serial_interface/send_byte_mux0001<2>27_3233 ;
  wire \serial_interface/send_byte_mux0001<2>37_3234 ;
  wire \serial_interface/send_byte_mux0001<2>5_3235 ;
  wire \serial_interface/send_byte_mux0001<2>50_3236 ;
  wire \serial_interface/send_byte_mux0001<2>55_3237 ;
  wire \serial_interface/send_byte_mux0001<2>67_3238 ;
  wire \serial_interface/send_byte_mux0001<2>72_3239 ;
  wire \serial_interface/send_byte_mux0001<2>82_3240 ;
  wire \serial_interface/send_byte_mux0001<3>0_3241 ;
  wire \serial_interface/send_byte_mux0001<3>104_3242 ;
  wire \serial_interface/send_byte_mux0001<3>117_3243 ;
  wire \serial_interface/send_byte_mux0001<3>122_3244 ;
  wire \serial_interface/send_byte_mux0001<3>134_3245 ;
  wire \serial_interface/send_byte_mux0001<3>139_3246 ;
  wire \serial_interface/send_byte_mux0001<3>14_3247 ;
  wire \serial_interface/send_byte_mux0001<3>149_3248 ;
  wire \serial_interface/send_byte_mux0001<3>171_3249 ;
  wire \serial_interface/send_byte_mux0001<3>180 ;
  wire \serial_interface/send_byte_mux0001<3>1801_3251 ;
  wire \serial_interface/send_byte_mux0001<3>1802_3252 ;
  wire \serial_interface/send_byte_mux0001<3>19_3253 ;
  wire \serial_interface/send_byte_mux0001<3>216 ;
  wire \serial_interface/send_byte_mux0001<3>2161 ;
  wire \serial_interface/send_byte_mux0001<3>29_3256 ;
  wire \serial_interface/send_byte_mux0001<3>43_3257 ;
  wire \serial_interface/send_byte_mux0001<3>48_3258 ;
  wire \serial_interface/send_byte_mux0001<3>49_3259 ;
  wire \serial_interface/send_byte_mux0001<3>5_3260 ;
  wire \serial_interface/send_byte_mux0001<3>72_3261 ;
  wire \serial_interface/send_byte_mux0001<3>77_3262 ;
  wire \serial_interface/send_byte_mux0001<3>89_3263 ;
  wire \serial_interface/send_byte_mux0001<3>94_3264 ;
  wire \serial_interface/send_byte_mux0001<4>0_3265 ;
  wire \serial_interface/send_byte_mux0001<4>10_3266 ;
  wire \serial_interface/send_byte_mux0001<4>110_3267 ;
  wire \serial_interface/send_byte_mux0001<4>115_3268 ;
  wire \serial_interface/send_byte_mux0001<4>127_3269 ;
  wire \serial_interface/send_byte_mux0001<4>143_3270 ;
  wire \serial_interface/send_byte_mux0001<4>148_3271 ;
  wire \serial_interface/send_byte_mux0001<4>149_3272 ;
  wire \serial_interface/send_byte_mux0001<4>158_3273 ;
  wire \serial_interface/send_byte_mux0001<4>193_3274 ;
  wire \serial_interface/send_byte_mux0001<4>215 ;
  wire \serial_interface/send_byte_mux0001<4>22_3276 ;
  wire \serial_interface/send_byte_mux0001<4>27_3277 ;
  wire \serial_interface/send_byte_mux0001<4>37_3278 ;
  wire \serial_interface/send_byte_mux0001<4>5_3279 ;
  wire \serial_interface/send_byte_mux0001<4>50_3280 ;
  wire \serial_interface/send_byte_mux0001<4>55_3281 ;
  wire \serial_interface/send_byte_mux0001<4>67_3282 ;
  wire \serial_interface/send_byte_mux0001<4>75_3283 ;
  wire \serial_interface/send_byte_mux0001<4>86_3284 ;
  wire \serial_interface/send_byte_mux0001<5>0_3285 ;
  wire \serial_interface/send_byte_mux0001<5>104_3286 ;
  wire \serial_interface/send_byte_mux0001<5>117_3287 ;
  wire \serial_interface/send_byte_mux0001<5>122_3288 ;
  wire \serial_interface/send_byte_mux0001<5>134_3289 ;
  wire \serial_interface/send_byte_mux0001<5>139_3290 ;
  wire \serial_interface/send_byte_mux0001<5>14_3291 ;
  wire \serial_interface/send_byte_mux0001<5>149_3292 ;
  wire \serial_interface/send_byte_mux0001<5>171_3293 ;
  wire \serial_interface/send_byte_mux0001<5>19_3294 ;
  wire \serial_interface/send_byte_mux0001<5>192 ;
  wire \serial_interface/send_byte_mux0001<5>1921_3296 ;
  wire \serial_interface/send_byte_mux0001<5>1922_3297 ;
  wire \serial_interface/send_byte_mux0001<5>216 ;
  wire \serial_interface/send_byte_mux0001<5>2161 ;
  wire \serial_interface/send_byte_mux0001<5>29_3300 ;
  wire \serial_interface/send_byte_mux0001<5>43_3301 ;
  wire \serial_interface/send_byte_mux0001<5>48_3302 ;
  wire \serial_interface/send_byte_mux0001<5>49_3303 ;
  wire \serial_interface/send_byte_mux0001<5>5_3304 ;
  wire \serial_interface/send_byte_mux0001<5>72_3305 ;
  wire \serial_interface/send_byte_mux0001<5>77_3306 ;
  wire \serial_interface/send_byte_mux0001<5>89_3307 ;
  wire \serial_interface/send_byte_mux0001<5>94_3308 ;
  wire \serial_interface/send_byte_mux0001<6>0_3309 ;
  wire \serial_interface/send_byte_mux0001<6>10_3310 ;
  wire \serial_interface/send_byte_mux0001<6>106_3311 ;
  wire \serial_interface/send_byte_mux0001<6>117_3312 ;
  wire \serial_interface/send_byte_mux0001<6>131_3313 ;
  wire \serial_interface/send_byte_mux0001<6>136_3314 ;
  wire \serial_interface/send_byte_mux0001<6>146_3315 ;
  wire \serial_interface/send_byte_mux0001<6>181_3316 ;
  wire \serial_interface/send_byte_mux0001<6>203 ;
  wire \serial_interface/send_byte_mux0001<6>22_3318 ;
  wire \serial_interface/send_byte_mux0001<6>27_3319 ;
  wire \serial_interface/send_byte_mux0001<6>37_3320 ;
  wire \serial_interface/send_byte_mux0001<6>5_3321 ;
  wire \serial_interface/send_byte_mux0001<6>50_3322 ;
  wire \serial_interface/send_byte_mux0001<6>55_3323 ;
  wire \serial_interface/send_byte_mux0001<6>67_3324 ;
  wire \serial_interface/send_byte_mux0001<6>72_3325 ;
  wire \serial_interface/send_byte_mux0001<6>82_3326 ;
  wire \serial_interface/send_byte_mux0001<7>0_3327 ;
  wire \serial_interface/send_byte_mux0001<7>104_3328 ;
  wire \serial_interface/send_byte_mux0001<7>117_3329 ;
  wire \serial_interface/send_byte_mux0001<7>122_3330 ;
  wire \serial_interface/send_byte_mux0001<7>134_3331 ;
  wire \serial_interface/send_byte_mux0001<7>139_3332 ;
  wire \serial_interface/send_byte_mux0001<7>14_3333 ;
  wire \serial_interface/send_byte_mux0001<7>149_3334 ;
  wire \serial_interface/send_byte_mux0001<7>171_3335 ;
  wire \serial_interface/send_byte_mux0001<7>19_3336 ;
  wire \serial_interface/send_byte_mux0001<7>192 ;
  wire \serial_interface/send_byte_mux0001<7>1921_3338 ;
  wire \serial_interface/send_byte_mux0001<7>1922_3339 ;
  wire \serial_interface/send_byte_mux0001<7>216 ;
  wire \serial_interface/send_byte_mux0001<7>2161 ;
  wire \serial_interface/send_byte_mux0001<7>29_3342 ;
  wire \serial_interface/send_byte_mux0001<7>43_3343 ;
  wire \serial_interface/send_byte_mux0001<7>48_3344 ;
  wire \serial_interface/send_byte_mux0001<7>49_3345 ;
  wire \serial_interface/send_byte_mux0001<7>5_3346 ;
  wire \serial_interface/send_byte_mux0001<7>72_3347 ;
  wire \serial_interface/send_byte_mux0001<7>77_3348 ;
  wire \serial_interface/send_byte_mux0001<7>89_3349 ;
  wire \serial_interface/send_byte_mux0001<7>94_3350 ;
  wire \serial_interface/state_FSM_FFd1_3351 ;
  wire \serial_interface/state_FSM_FFd1-In ;
  wire \serial_interface/state_FSM_FFd1-In118_3353 ;
  wire \serial_interface/state_FSM_FFd1-In118_SW0 ;
  wire \serial_interface/state_FSM_FFd1-In118_SW01_3355 ;
  wire \serial_interface/state_FSM_FFd1-In16_3356 ;
  wire \serial_interface/state_FSM_FFd1-In27_3357 ;
  wire \serial_interface/state_FSM_FFd1-In89_3358 ;
  wire \serial_interface/state_FSM_FFd2_3359 ;
  wire \serial_interface/state_FSM_FFd2-In ;
  wire \serial_interface/state_FSM_FFd2-In115_3361 ;
  wire \serial_interface/state_FSM_FFd2-In140_3362 ;
  wire \serial_interface/state_FSM_FFd2-In16_3363 ;
  wire \serial_interface/state_FSM_FFd2-In38_3364 ;
  wire \serial_interface/state_FSM_FFd2-In55_3365 ;
  wire \serial_interface/state_FSM_FFd2-In72_3366 ;
  wire \serial_interface/state_FSM_FFd2-In78_3367 ;
  wire \serial_interface/state_FSM_FFd2-In99_3368 ;
  wire \serial_interface/state_FSM_FFd3_3369 ;
  wire \serial_interface/state_FSM_FFd3-In ;
  wire \serial_interface/state_FSM_FFd3-In129_3371 ;
  wire \serial_interface/state_FSM_FFd3-In13_3372 ;
  wire \serial_interface/state_FSM_FFd3-In164 ;
  wire \serial_interface/state_FSM_FFd3-In1641_3374 ;
  wire \serial_interface/state_FSM_FFd3-In169_3375 ;
  wire \serial_interface/state_FSM_FFd3-In193_3376 ;
  wire \serial_interface/state_FSM_FFd3-In33 ;
  wire \serial_interface/state_FSM_FFd3-In331_3378 ;
  wire \serial_interface/state_FSM_FFd3-In51 ;
  wire \serial_interface/state_FSM_FFd3-In511_3380 ;
  wire \serial_interface/state_FSM_FFd3-In88_3381 ;
  wire \serial_interface/state_FSM_FFd4_3382 ;
  wire \serial_interface/state_FSM_FFd4-In119_3383 ;
  wire \serial_interface/state_FSM_FFd4-In122_3384 ;
  wire \serial_interface/state_FSM_FFd4-In136_3385 ;
  wire \serial_interface/state_FSM_FFd4-In179_3386 ;
  wire \serial_interface/state_FSM_FFd4-In216 ;
  wire \serial_interface/state_FSM_FFd4-In40_3388 ;
  wire \serial_interface/state_FSM_FFd4-In5_3389 ;
  wire \serial_interface/state_FSM_FFd4-In66 ;
  wire \serial_interface/state_FSM_FFd4-In661_3391 ;
  wire \serial_interface/state_FSM_FFd4-In95 ;
  wire \serial_interface/state_FSM_FFd4-In951_3393 ;
  wire \serial_interface/state_FSM_N0 ;
  wire \serial_interface/state_FSM_N2 ;
  wire \serial_interface/state_cmp_eq0001 ;
  wire \serial_interface/state_cmp_eq0007 ;
  wire \serial_interface/state_cmp_eq0008 ;
  wire \serial_interface/state_cmp_eq0010_3399 ;
  wire \serial_interface/state_cmp_eq0011 ;
  wire \serial_interface/state_cmp_eq0021 ;
  wire \serial_interface/stop_o_3413 ;
  wire \serial_interface/stop_o_not0001 ;
  wire \serial_interface/time_out_mux0009<0>1 ;
  wire \serial_interface/time_out_mux0009[10] ;
  wire \serial_interface/time_out_mux0009[12] ;
  wire \serial_interface/time_out_mux0009[18] ;
  wire \serial_interface/time_out_mux0009[20] ;
  wire \serial_interface/time_out_mux0009[21] ;
  wire \serial_interface/time_out_mux0009[3] ;
  wire \serial_interface/time_out_mux0009[4] ;
  wire \serial_interface/time_out_mux0009[5] ;
  wire \serial_interface/time_out_mux0009[7] ;
  wire \serial_interface/time_out_mux0009[9] ;
  wire \serial_interface/time_out_or0000_3470 ;
  wire \system_control[0] ;
  wire \system_control[2] ;
  wire \system_control[3] ;
  wire \system_control[4] ;
  wire tp2_o_OBUF_3479;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_CLK0_UNCONNECTED ;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_CLK90_UNCONNECTED ;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_CLK180_UNCONNECTED ;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_CLK270_UNCONNECTED ;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_CLK2X_UNCONNECTED ;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_CLK2X180_UNCONNECTED ;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_CLKDV_UNCONNECTED ;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_CLKFX180_UNCONNECTED ;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_LOCKED_UNCONNECTED ;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_PSDONE_UNCONNECTED ;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_STATUS<7>_UNCONNECTED ;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_STATUS<6>_UNCONNECTED ;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_STATUS<5>_UNCONNECTED ;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_STATUS<4>_UNCONNECTED ;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_STATUS<3>_UNCONNECTED ;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_STATUS<2>_UNCONNECTED ;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_STATUS<1>_UNCONNECTED ;
  wire \NLW_PLL_clock_gen/DCM_SP_INST_STATUS<0>_UNCONNECTED ;
  wire [3 : 0] \Inst_ad5660_serial_DAC/Result ;
  wire [4 : 0] \Inst_ad5660_serial_DAC/bit_count ;
  wire [4 : 0] \Inst_ad5660_serial_DAC/bit_count_mux0000 ;
  wire [3 : 0] \Inst_ad5660_serial_DAC/dec_clk_count ;
  wire [23 : 0] \Inst_ad5660_serial_DAC/shift_register ;
  wire [1 : 0] \LTC_A/LTC_biphase_generator/Madd_ltc_frame_i_not0000 ;
  wire [6 : 5] \LTC_A/LTC_biphase_generator/ltc_frame_i_sub0000 ;
  wire [6 : 0] \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy ;
  wire [0 : 0] \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_lut ;
  wire [14 : 0] \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy ;
  wire [15 : 1] \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut ;
  wire [7 : 0] \LTC_A/LTC_clockgenerator/Result ;
  wire [7 : 0] \LTC_A/LTC_clockgenerator/ltc_bitcount ;
  wire [15 : 0] \LTC_A/LTC_clockgenerator/period_count ;
  wire [15 : 0] \LTC_A/LTC_clockgenerator/period_count_addsub0000 ;
  wire [1 : 0] \LTC_A/LTC_frame_count/frame_tens ;
  wire [5 : 4] \LTC_A/LTC_frame_count/frame_tens_mux0002 ;
  wire [1 : 0] \LTC_A/LTC_frame_count/frame_tens_o ;
  wire [3 : 0] \LTC_A/LTC_frame_count/frame_units ;
  wire [3 : 0] \LTC_A/LTC_frame_count/frame_units_mux0009 ;
  wire [3 : 0] \LTC_A/LTC_frame_count/frame_units_o ;
  wire [1 : 0] \LTC_A/LTC_frame_count/hour_tens ;
  wire [5 : 4] \LTC_A/LTC_frame_count/hour_tens_mux0000 ;
  wire [1 : 0] \LTC_A/LTC_frame_count/hour_tens_o ;
  wire [3 : 0] \LTC_A/LTC_frame_count/hour_units ;
  wire [3 : 0] \LTC_A/LTC_frame_count/hour_units_o ;
  wire [2 : 0] \LTC_A/LTC_frame_count/min_tens ;
  wire [2 : 0] \LTC_A/LTC_frame_count/min_tens_o ;
  wire [3 : 0] \LTC_A/LTC_frame_count/min_units ;
  wire [3 : 0] \LTC_A/LTC_frame_count/min_units_o ;
  wire [2 : 0] \LTC_A/LTC_frame_count/sec_tens ;
  wire [2 : 0] \LTC_A/LTC_frame_count/sec_tens_o ;
  wire [3 : 0] \LTC_A/LTC_frame_count/sec_units ;
  wire [3 : 0] \LTC_A/LTC_frame_count/sec_units_o ;
  wire [15 : 0] \LTC_A/Mcompar_delay_sync_cmp_eq0000_cy ;
  wire [15 : 0] \LTC_A/Mcompar_delay_sync_cmp_eq0000_lut ;
  wire [22 : 0] \LTC_A/Mcount_delay_count_low_cy ;
  wire [0 : 0] \LTC_A/Mcount_delay_count_low_lut ;
  wire [23 : 0] \LTC_A/Result ;
  wire [3 : 0] \LTC_A/delay_count_high ;
  wire [4 : 0] \LTC_A/delay_count_high_cmp_eq0000_wg_cy ;
  wire [5 : 0] \LTC_A/delay_count_high_cmp_eq0000_wg_lut ;
  wire [23 : 0] \LTC_A/delay_count_low ;
  wire [1 : 0] \LTC_B/LTC_biphase_generator/Madd_ltc_frame_i_not0000 ;
  wire [6 : 5] \LTC_B/LTC_biphase_generator/ltc_frame_i_sub0000 ;
  wire [6 : 0] \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy ;
  wire [0 : 0] \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_lut ;
  wire [14 : 0] \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy ;
  wire [15 : 1] \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut ;
  wire [7 : 0] \LTC_B/LTC_clockgenerator/Result ;
  wire [7 : 0] \LTC_B/LTC_clockgenerator/ltc_bitcount ;
  wire [15 : 0] \LTC_B/LTC_clockgenerator/period_count ;
  wire [15 : 0] \LTC_B/LTC_clockgenerator/period_count_addsub0000 ;
  wire [1 : 0] \LTC_B/LTC_frame_count/frame_tens ;
  wire [5 : 4] \LTC_B/LTC_frame_count/frame_tens_mux0002 ;
  wire [1 : 0] \LTC_B/LTC_frame_count/frame_tens_o ;
  wire [3 : 0] \LTC_B/LTC_frame_count/frame_units ;
  wire [3 : 0] \LTC_B/LTC_frame_count/frame_units_mux0009 ;
  wire [3 : 0] \LTC_B/LTC_frame_count/frame_units_o ;
  wire [1 : 0] \LTC_B/LTC_frame_count/hour_tens ;
  wire [5 : 4] \LTC_B/LTC_frame_count/hour_tens_mux0000 ;
  wire [1 : 0] \LTC_B/LTC_frame_count/hour_tens_o ;
  wire [3 : 0] \LTC_B/LTC_frame_count/hour_units ;
  wire [3 : 0] \LTC_B/LTC_frame_count/hour_units_o ;
  wire [2 : 0] \LTC_B/LTC_frame_count/min_tens ;
  wire [2 : 0] \LTC_B/LTC_frame_count/min_tens_o ;
  wire [3 : 0] \LTC_B/LTC_frame_count/min_units ;
  wire [3 : 0] \LTC_B/LTC_frame_count/min_units_o ;
  wire [2 : 0] \LTC_B/LTC_frame_count/sec_tens ;
  wire [2 : 0] \LTC_B/LTC_frame_count/sec_tens_o ;
  wire [3 : 0] \LTC_B/LTC_frame_count/sec_units ;
  wire [3 : 0] \LTC_B/LTC_frame_count/sec_units_o ;
  wire [15 : 0] \LTC_B/Mcompar_delay_sync_cmp_eq0000_cy ;
  wire [15 : 0] \LTC_B/Mcompar_delay_sync_cmp_eq0000_lut ;
  wire [22 : 0] \LTC_B/Mcount_delay_count_low_cy ;
  wire [0 : 0] \LTC_B/Mcount_delay_count_low_lut ;
  wire [23 : 0] \LTC_B/Result ;
  wire [3 : 0] \LTC_B/delay_count_high ;
  wire [4 : 0] \LTC_B/delay_count_high_cmp_eq0000_wg_cy ;
  wire [5 : 0] \LTC_B/delay_count_high_cmp_eq0000_wg_lut ;
  wire [23 : 0] \LTC_B/delay_count_low ;
  wire [31 : 0] LTC_a_delay;
  wire [5 : 0] LTC_a_frames;
  wire [5 : 0] LTC_a_hours;
  wire [6 : 0] LTC_a_mins;
  wire [6 : 0] LTC_a_secs;
  wire [31 : 0] LTC_b_delay;
  wire [5 : 0] LTC_b_frames;
  wire [5 : 0] LTC_b_hours;
  wire [6 : 0] LTC_b_mins;
  wire [6 : 0] LTC_b_secs;
  wire [11 : 0] Mcompar_pps_status_cmp_gt0000_cy;
  wire [19 : 0] Mcount_count_cy;
  wire [20 : 1] Mcount_count_lut;
  wire [22 : 0] Mcount_pps_status_count_cy;
  wire [0 : 0] Mcount_pps_status_count_lut;
  wire [25 : 0] \OCXO_pll/phase_detect/Mcount_phase_counter_cy ;
  wire [26 : 1] \OCXO_pll/phase_detect/Mcount_phase_counter_lut ;
  wire [26 : 0] \OCXO_pll/phase_detect/Result ;
  wire [4 : 0] \OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy ;
  wire [4 : 0] \OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_lut ;
  wire [26 : 0] \OCXO_pll/phase_detect/phase_counter ;
  wire [27 : 1] \OCXO_pll/phase_detect/phase_diff ;
  wire [34 : 0] \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy ;
  wire [34 : 0] \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut ;
  wire [26 : 0] \OCXO_pll/phase_loop/Madd_out_sum_cy ;
  wire [26 : 0] \OCXO_pll/phase_loop/Madd_out_sum_lut ;
  wire [16 : 0] \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy ;
  wire [13 : 0] \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy ;
  wire [35 : 0] \OCXO_pll/phase_loop/integrator ;
  wire [35 : 0] \OCXO_pll/phase_loop/integrator_addsub0000 ;
  wire [35 : 0] \OCXO_pll/phase_loop/integrator_mux0002 ;
  wire [27 : 0] \OCXO_pll/phase_loop/out_sum ;
  wire [23 : 0] Result;
  wire [7 : 0] \VCXO_pll/Mcount_ref_clk_count_cy ;
  wire [0 : 0] \VCXO_pll/Mcount_ref_clk_count_lut ;
  wire [8 : 0] \VCXO_pll/Result ;
  wire [24 : 3] \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy ;
  wire [25 : 3] \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut ;
  wire [16 : 6] \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy ;
  wire [17 : 6] \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut ;
  wire [25 : 3] \VCXO_pll/VCXO_phase_loop/Result ;
  wire [25 : 3] \VCXO_pll/VCXO_phase_loop/integrator ;
  wire [17 : 2] \VCXO_pll/VCXO_phase_loop/out_sum ;
  wire [17 : 6] \VCXO_pll/VCXO_phase_loop/out_sum_mux0001 ;
  wire [15 : 0] \VCXO_pll/cv_word_o ;
  wire [8 : 0] \VCXO_pll/phase_detector/Mcount_phase_counter_cy ;
  wire [9 : 1] \VCXO_pll/phase_detector/Mcount_phase_counter_lut ;
  wire [9 : 0] \VCXO_pll/phase_detector/Result ;
  wire [9 : 0] \VCXO_pll/phase_detector/phase_counter ;
  wire [9 : 0] \VCXO_pll/phase_detector/phase_diff ;
  wire [8 : 0] \VCXO_pll/ref_clk_count ;
  wire [6 : 0] \communication/Mcount_address_cy ;
  wire [7 : 0] \communication/Mcount_address_lut ;
  wire [7 : 0] \communication/address ;
  wire [7 : 0] \communication/address_o ;
  wire [7 : 0] \communication/address_o_mux0000 ;
  wire [1 : 0] \communication/stop_line ;
  wire [1 : 0] \communication/update_line ;
  wire [20 : 0] count;
  wire [6 : 0] \dac_OCXO/Mcount_bit_count_cy ;
  wire [7 : 1] \dac_OCXO/Mcount_bit_count_lut ;
  wire [7 : 0] \dac_OCXO/Result ;
  wire [7 : 0] \dac_OCXO/bit_count ;
  wire [23 : 0] \dac_OCXO/shift_reg ;
  wire [23 : 0] \dac_OCXO/shift_reg_mux0000 ;
  wire [30 : 0] \f_gen/Msub_count_addsub0000_cy ;
  wire [31 : 1] \f_gen/Msub_count_addsub0000_lut ;
  wire [31 : 0] \f_gen/count ;
  wire [31 : 0] \f_gen/count_addsub0000 ;
  wire [6 : 0] \f_gen/pulse_state_cmp_eq0000_wg_cy ;
  wire [7 : 0] \f_gen/pulse_state_cmp_eq0000_wg_lut ;
  wire [30 : 0] \h_gen/Msub_count_addsub0000_cy ;
  wire [31 : 1] \h_gen/Msub_count_addsub0000_lut ;
  wire [31 : 0] \h_gen/count ;
  wire [31 : 0] \h_gen/count_addsub0000 ;
  wire [6 : 0] \h_gen/pulse_state_cmp_eq0000_wg_cy ;
  wire [7 : 0] \h_gen/pulse_state_cmp_eq0000_wg_lut ;
  wire [4 : 0] longer_tick_cmp_eq0000_wg_cy;
  wire [5 : 0] longer_tick_cmp_eq0000_wg_lut;
  wire [1 : 0] pps_edge;
  wire [23 : 0] pps_status_count;
  wire [20 : 0] \serial_interface/Msub_time_out_addsub0000_cy ;
  wire [21 : 1] \serial_interface/Msub_time_out_addsub0000_lut ;
  wire [1 : 0] \serial_interface/SCL_delay ;
  wire [1 : 0] \serial_interface/SDA_delay ;
  wire [3 : 0] \serial_interface/bit_count ;
  wire [3 : 0] \serial_interface/bit_count_mux0002 ;
  wire [2 : 0] \serial_interface/command ;
  wire [2 : 0] \serial_interface/command_mux0000 ;
  wire [7 : 0] \serial_interface/recieve_byte ;
  wire [7 : 0] \serial_interface/recieved_byte_o ;
  wire [7 : 0] \serial_interface/send_byte ;
  wire [4 : 0] \serial_interface/state_cmp_eq0011_wg_cy ;
  wire [5 : 0] \serial_interface/state_cmp_eq0011_wg_lut ;
  wire [21 : 0] \serial_interface/time_out ;
  wire [21 : 0] \serial_interface/time_out_addsub0000 ;
  GND   XST_GND (
    .G(tp2_o_OBUF_3479)
  );
  VCC   XST_VCC (
    .P(N1)
  );
  FD   pps_edge_0 (
    .C(clk_100),
    .D(PPS_i_IBUF_2001),
    .Q(pps_edge[0])
  );
  FD   pps_edge_1 (
    .C(clk_100),
    .D(pps_edge[0]),
    .Q(pps_edge[1])
  );
  FDR   kvant_pps (
    .C(clk_100),
    .D(N1),
    .R(kvant_pps_not0001),
    .Q(kvant_pps_3018)
  );
  FDRSE   pps_status (
    .C(clk_10_i_IBUFG_2312),
    .CE(pps_status_cmp_gt0000),
    .D(tp2_o_OBUF_3479),
    .R(rst_i_IBUF_3067),
    .S(PPS_i_IBUF_2001),
    .Q(pps_status_3039)
  );
  FDE   genlock_register_20_0 (
    .C(clk_148),
    .CE(genlock_register_20_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_20_0_2765)
  );
  FDE   genlock_register_20_1 (
    .C(clk_148),
    .CE(genlock_register_20_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_20_1_2766)
  );
  FDE   genlock_register_20_2 (
    .C(clk_148),
    .CE(genlock_register_20_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_20_2_2767)
  );
  FDE   genlock_register_20_3 (
    .C(clk_148),
    .CE(genlock_register_20_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_20_3_2768)
  );
  FDE   genlock_register_20_4 (
    .C(clk_148),
    .CE(genlock_register_20_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_20_4_2769)
  );
  FDE   genlock_register_20_5 (
    .C(clk_148),
    .CE(genlock_register_20_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_20_5_2770)
  );
  FDE   genlock_register_20_6 (
    .C(clk_148),
    .CE(genlock_register_20_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_20_6_2771)
  );
  FDE   genlock_register_20_7 (
    .C(clk_148),
    .CE(genlock_register_20_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_20_7_2772)
  );
  FDE   genlock_register_22_0 (
    .C(clk_148),
    .CE(genlock_register_22_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_22_0_2783)
  );
  FDE   genlock_register_22_1 (
    .C(clk_148),
    .CE(genlock_register_22_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_22_1_2784)
  );
  FDE   genlock_register_22_2 (
    .C(clk_148),
    .CE(genlock_register_22_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_22_2_2785)
  );
  FDE   genlock_register_22_3 (
    .C(clk_148),
    .CE(genlock_register_22_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_22_3_2786)
  );
  FDE   genlock_register_22_4 (
    .C(clk_148),
    .CE(genlock_register_22_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_22_4_2787)
  );
  FDE   genlock_register_22_5 (
    .C(clk_148),
    .CE(genlock_register_22_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_22_5_2788)
  );
  FDE   genlock_register_22_6 (
    .C(clk_148),
    .CE(genlock_register_22_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_22_6_2789)
  );
  FDE   genlock_register_22_7 (
    .C(clk_148),
    .CE(genlock_register_22_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_22_7_2790)
  );
  FDE   genlock_register_21_0 (
    .C(clk_148),
    .CE(genlock_register_21_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_21_0_2774)
  );
  FDE   genlock_register_21_1 (
    .C(clk_148),
    .CE(genlock_register_21_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_21_1_2775)
  );
  FDE   genlock_register_21_2 (
    .C(clk_148),
    .CE(genlock_register_21_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_21_2_2776)
  );
  FDE   genlock_register_21_3 (
    .C(clk_148),
    .CE(genlock_register_21_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_21_3_2777)
  );
  FDE   genlock_register_21_4 (
    .C(clk_148),
    .CE(genlock_register_21_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_21_4_2778)
  );
  FDE   genlock_register_21_5 (
    .C(clk_148),
    .CE(genlock_register_21_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_21_5_2779)
  );
  FDE   genlock_register_21_6 (
    .C(clk_148),
    .CE(genlock_register_21_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_21_6_2780)
  );
  FDE   genlock_register_21_7 (
    .C(clk_148),
    .CE(genlock_register_21_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_21_7_2781)
  );
  FDE   genlock_register_19_0 (
    .C(clk_148),
    .CE(genlock_register_19_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_19_0_2747)
  );
  FDE   genlock_register_19_1 (
    .C(clk_148),
    .CE(genlock_register_19_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_19_1_2748)
  );
  FDE   genlock_register_19_2 (
    .C(clk_148),
    .CE(genlock_register_19_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_19_2_2749)
  );
  FDE   genlock_register_19_3 (
    .C(clk_148),
    .CE(genlock_register_19_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_19_3_2750)
  );
  FDE   genlock_register_19_4 (
    .C(clk_148),
    .CE(genlock_register_19_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_19_4_2751)
  );
  FDE   genlock_register_19_5 (
    .C(clk_148),
    .CE(genlock_register_19_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_19_5_2752)
  );
  FDE   genlock_register_19_6 (
    .C(clk_148),
    .CE(genlock_register_19_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_19_6_2753)
  );
  FDE   genlock_register_19_7 (
    .C(clk_148),
    .CE(genlock_register_19_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_19_7_2754)
  );
  FDE   genlock_register_18_0 (
    .C(clk_148),
    .CE(genlock_register_18_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_18_0_2738)
  );
  FDE   genlock_register_18_1 (
    .C(clk_148),
    .CE(genlock_register_18_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_18_1_2739)
  );
  FDE   genlock_register_18_2 (
    .C(clk_148),
    .CE(genlock_register_18_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_18_2_2740)
  );
  FDE   genlock_register_18_3 (
    .C(clk_148),
    .CE(genlock_register_18_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_18_3_2741)
  );
  FDE   genlock_register_18_4 (
    .C(clk_148),
    .CE(genlock_register_18_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_18_4_2742)
  );
  FDE   genlock_register_18_5 (
    .C(clk_148),
    .CE(genlock_register_18_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_18_5_2743)
  );
  FDE   genlock_register_18_6 (
    .C(clk_148),
    .CE(genlock_register_18_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_18_6_2744)
  );
  FDE   genlock_register_18_7 (
    .C(clk_148),
    .CE(genlock_register_18_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_18_7_2745)
  );
  FDE   genlock_register_15_0 (
    .C(clk_148),
    .CE(genlock_register_15_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_15_0_2711)
  );
  FDE   genlock_register_15_1 (
    .C(clk_148),
    .CE(genlock_register_15_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_15_1_2712)
  );
  FDE   genlock_register_15_2 (
    .C(clk_148),
    .CE(genlock_register_15_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_15_2_2713)
  );
  FDE   genlock_register_15_3 (
    .C(clk_148),
    .CE(genlock_register_15_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_15_3_2714)
  );
  FDE   genlock_register_15_4 (
    .C(clk_148),
    .CE(genlock_register_15_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_15_4_2715)
  );
  FDE   genlock_register_15_5 (
    .C(clk_148),
    .CE(genlock_register_15_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_15_5_2716)
  );
  FDE   genlock_register_15_6 (
    .C(clk_148),
    .CE(genlock_register_15_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_15_6_2717)
  );
  FDE   genlock_register_15_7 (
    .C(clk_148),
    .CE(genlock_register_15_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_15_7_2718)
  );
  FDE   genlock_register_17_0 (
    .C(clk_148),
    .CE(genlock_register_17_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_17_0_2729)
  );
  FDE   genlock_register_17_1 (
    .C(clk_148),
    .CE(genlock_register_17_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_17_1_2730)
  );
  FDE   genlock_register_17_2 (
    .C(clk_148),
    .CE(genlock_register_17_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_17_2_2731)
  );
  FDE   genlock_register_17_3 (
    .C(clk_148),
    .CE(genlock_register_17_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_17_3_2732)
  );
  FDE   genlock_register_17_4 (
    .C(clk_148),
    .CE(genlock_register_17_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_17_4_2733)
  );
  FDE   genlock_register_17_5 (
    .C(clk_148),
    .CE(genlock_register_17_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_17_5_2734)
  );
  FDE   genlock_register_17_6 (
    .C(clk_148),
    .CE(genlock_register_17_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_17_6_2735)
  );
  FDE   genlock_register_17_7 (
    .C(clk_148),
    .CE(genlock_register_17_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_17_7_2736)
  );
  FDE   genlock_register_16_0 (
    .C(clk_148),
    .CE(genlock_register_16_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_16_0_2720)
  );
  FDE   genlock_register_16_1 (
    .C(clk_148),
    .CE(genlock_register_16_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_16_1_2721)
  );
  FDE   genlock_register_16_2 (
    .C(clk_148),
    .CE(genlock_register_16_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_16_2_2722)
  );
  FDE   genlock_register_16_3 (
    .C(clk_148),
    .CE(genlock_register_16_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_16_3_2723)
  );
  FDE   genlock_register_16_4 (
    .C(clk_148),
    .CE(genlock_register_16_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_16_4_2724)
  );
  FDE   genlock_register_16_5 (
    .C(clk_148),
    .CE(genlock_register_16_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_16_5_2725)
  );
  FDE   genlock_register_16_6 (
    .C(clk_148),
    .CE(genlock_register_16_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_16_6_2726)
  );
  FDE   genlock_register_16_7 (
    .C(clk_148),
    .CE(genlock_register_16_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_16_7_2727)
  );
  FDE   genlock_register_14_0 (
    .C(clk_148),
    .CE(genlock_register_14_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_14_0_2702)
  );
  FDE   genlock_register_14_1 (
    .C(clk_148),
    .CE(genlock_register_14_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_14_1_2703)
  );
  FDE   genlock_register_14_2 (
    .C(clk_148),
    .CE(genlock_register_14_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_14_2_2704)
  );
  FDE   genlock_register_14_3 (
    .C(clk_148),
    .CE(genlock_register_14_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_14_3_2705)
  );
  FDE   genlock_register_14_4 (
    .C(clk_148),
    .CE(genlock_register_14_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_14_4_2706)
  );
  FDE   genlock_register_14_5 (
    .C(clk_148),
    .CE(genlock_register_14_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_14_5_2707)
  );
  FDE   genlock_register_14_6 (
    .C(clk_148),
    .CE(genlock_register_14_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_14_6_2708)
  );
  FDE   genlock_register_14_7 (
    .C(clk_148),
    .CE(genlock_register_14_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_14_7_2709)
  );
  FDE   genlock_register_13_0 (
    .C(clk_148),
    .CE(genlock_register_13_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_13_0_2693)
  );
  FDE   genlock_register_13_1 (
    .C(clk_148),
    .CE(genlock_register_13_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_13_1_2694)
  );
  FDE   genlock_register_13_2 (
    .C(clk_148),
    .CE(genlock_register_13_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_13_2_2695)
  );
  FDE   genlock_register_13_3 (
    .C(clk_148),
    .CE(genlock_register_13_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_13_3_2696)
  );
  FDE   genlock_register_13_4 (
    .C(clk_148),
    .CE(genlock_register_13_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_13_4_2697)
  );
  FDE   genlock_register_13_5 (
    .C(clk_148),
    .CE(genlock_register_13_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_13_5_2698)
  );
  FDE   genlock_register_13_6 (
    .C(clk_148),
    .CE(genlock_register_13_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_13_6_2699)
  );
  FDE   genlock_register_13_7 (
    .C(clk_148),
    .CE(genlock_register_13_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_13_7_2700)
  );
  FDE   genlock_register_10_0 (
    .C(clk_148),
    .CE(genlock_register_10_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_10_0_2666)
  );
  FDE   genlock_register_10_1 (
    .C(clk_148),
    .CE(genlock_register_10_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_10_1_2667)
  );
  FDE   genlock_register_10_2 (
    .C(clk_148),
    .CE(genlock_register_10_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_10_2_2668)
  );
  FDE   genlock_register_10_3 (
    .C(clk_148),
    .CE(genlock_register_10_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_10_3_2669)
  );
  FDE   genlock_register_10_4 (
    .C(clk_148),
    .CE(genlock_register_10_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_10_4_2670)
  );
  FDE   genlock_register_10_5 (
    .C(clk_148),
    .CE(genlock_register_10_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_10_5_2671)
  );
  FDE   genlock_register_10_6 (
    .C(clk_148),
    .CE(genlock_register_10_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_10_6_2672)
  );
  FDE   genlock_register_10_7 (
    .C(clk_148),
    .CE(genlock_register_10_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_10_7_2673)
  );
  FDE   genlock_register_12_0 (
    .C(clk_148),
    .CE(genlock_register_12_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_12_0_2684)
  );
  FDE   genlock_register_12_1 (
    .C(clk_148),
    .CE(genlock_register_12_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_12_1_2685)
  );
  FDE   genlock_register_12_2 (
    .C(clk_148),
    .CE(genlock_register_12_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_12_2_2686)
  );
  FDE   genlock_register_12_3 (
    .C(clk_148),
    .CE(genlock_register_12_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_12_3_2687)
  );
  FDE   genlock_register_12_4 (
    .C(clk_148),
    .CE(genlock_register_12_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_12_4_2688)
  );
  FDE   genlock_register_12_5 (
    .C(clk_148),
    .CE(genlock_register_12_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_12_5_2689)
  );
  FDE   genlock_register_12_6 (
    .C(clk_148),
    .CE(genlock_register_12_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_12_6_2690)
  );
  FDE   genlock_register_12_7 (
    .C(clk_148),
    .CE(genlock_register_12_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_12_7_2691)
  );
  FDE   genlock_register_11_0 (
    .C(clk_148),
    .CE(genlock_register_11_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_11_0_2675)
  );
  FDE   genlock_register_11_1 (
    .C(clk_148),
    .CE(genlock_register_11_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_11_1_2676)
  );
  FDE   genlock_register_11_2 (
    .C(clk_148),
    .CE(genlock_register_11_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_11_2_2677)
  );
  FDE   genlock_register_11_3 (
    .C(clk_148),
    .CE(genlock_register_11_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_11_3_2678)
  );
  FDE   genlock_register_11_4 (
    .C(clk_148),
    .CE(genlock_register_11_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_11_4_2679)
  );
  FDE   genlock_register_11_5 (
    .C(clk_148),
    .CE(genlock_register_11_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_11_5_2680)
  );
  FDE   genlock_register_11_6 (
    .C(clk_148),
    .CE(genlock_register_11_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_11_6_2681)
  );
  FDE   genlock_register_11_7 (
    .C(clk_148),
    .CE(genlock_register_11_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_11_7_2682)
  );
  FDE   genlock_register_9_0 (
    .C(clk_148),
    .CE(genlock_register_9_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_9_0_2855)
  );
  FDE   genlock_register_9_1 (
    .C(clk_148),
    .CE(genlock_register_9_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_9_1_2856)
  );
  FDE   genlock_register_9_2 (
    .C(clk_148),
    .CE(genlock_register_9_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_9_2_2857)
  );
  FDE   genlock_register_9_3 (
    .C(clk_148),
    .CE(genlock_register_9_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_9_3_2858)
  );
  FDE   genlock_register_9_4 (
    .C(clk_148),
    .CE(genlock_register_9_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_9_4_2859)
  );
  FDE   genlock_register_9_5 (
    .C(clk_148),
    .CE(genlock_register_9_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_9_5_2860)
  );
  FDE   genlock_register_9_6 (
    .C(clk_148),
    .CE(genlock_register_9_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_9_6_2861)
  );
  FDE   genlock_register_9_7 (
    .C(clk_148),
    .CE(genlock_register_9_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_9_7_2862)
  );
  FDE   genlock_register_8_0 (
    .C(clk_148),
    .CE(genlock_register_8_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_8_0_2846)
  );
  FDE   genlock_register_8_1 (
    .C(clk_148),
    .CE(genlock_register_8_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_8_1_2847)
  );
  FDE   genlock_register_8_2 (
    .C(clk_148),
    .CE(genlock_register_8_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_8_2_2848)
  );
  FDE   genlock_register_8_3 (
    .C(clk_148),
    .CE(genlock_register_8_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_8_3_2849)
  );
  FDE   genlock_register_8_4 (
    .C(clk_148),
    .CE(genlock_register_8_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_8_4_2850)
  );
  FDE   genlock_register_8_5 (
    .C(clk_148),
    .CE(genlock_register_8_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_8_5_2851)
  );
  FDE   genlock_register_8_6 (
    .C(clk_148),
    .CE(genlock_register_8_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_8_6_2852)
  );
  FDE   genlock_register_8_7 (
    .C(clk_148),
    .CE(genlock_register_8_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_8_7_2853)
  );
  FDE   genlock_register_5_0 (
    .C(clk_148),
    .CE(genlock_register_5_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_5_0_2819)
  );
  FDE   genlock_register_5_1 (
    .C(clk_148),
    .CE(genlock_register_5_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_5_1_2820)
  );
  FDE   genlock_register_5_2 (
    .C(clk_148),
    .CE(genlock_register_5_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_5_2_2821)
  );
  FDE   genlock_register_5_3 (
    .C(clk_148),
    .CE(genlock_register_5_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_5_3_2822)
  );
  FDE   genlock_register_5_4 (
    .C(clk_148),
    .CE(genlock_register_5_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_5_4_2823)
  );
  FDE   genlock_register_5_5 (
    .C(clk_148),
    .CE(genlock_register_5_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_5_5_2824)
  );
  FDE   genlock_register_5_6 (
    .C(clk_148),
    .CE(genlock_register_5_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_5_6_2825)
  );
  FDE   genlock_register_5_7 (
    .C(clk_148),
    .CE(genlock_register_5_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_5_7_2826)
  );
  FDE   genlock_register_7_0 (
    .C(clk_148),
    .CE(genlock_register_7_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_7_0_2837)
  );
  FDE   genlock_register_7_1 (
    .C(clk_148),
    .CE(genlock_register_7_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_7_1_2838)
  );
  FDE   genlock_register_7_2 (
    .C(clk_148),
    .CE(genlock_register_7_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_7_2_2839)
  );
  FDE   genlock_register_7_3 (
    .C(clk_148),
    .CE(genlock_register_7_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_7_3_2840)
  );
  FDE   genlock_register_7_4 (
    .C(clk_148),
    .CE(genlock_register_7_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_7_4_2841)
  );
  FDE   genlock_register_7_5 (
    .C(clk_148),
    .CE(genlock_register_7_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_7_5_2842)
  );
  FDE   genlock_register_7_6 (
    .C(clk_148),
    .CE(genlock_register_7_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_7_6_2843)
  );
  FDE   genlock_register_7_7 (
    .C(clk_148),
    .CE(genlock_register_7_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_7_7_2844)
  );
  FDE   genlock_register_6_0 (
    .C(clk_148),
    .CE(genlock_register_6_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_6_0_2828)
  );
  FDE   genlock_register_6_1 (
    .C(clk_148),
    .CE(genlock_register_6_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_6_1_2829)
  );
  FDE   genlock_register_6_2 (
    .C(clk_148),
    .CE(genlock_register_6_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_6_2_2830)
  );
  FDE   genlock_register_6_3 (
    .C(clk_148),
    .CE(genlock_register_6_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_6_3_2831)
  );
  FDE   genlock_register_6_4 (
    .C(clk_148),
    .CE(genlock_register_6_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_6_4_2832)
  );
  FDE   genlock_register_6_5 (
    .C(clk_148),
    .CE(genlock_register_6_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_6_5_2833)
  );
  FDE   genlock_register_6_6 (
    .C(clk_148),
    .CE(genlock_register_6_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_6_6_2834)
  );
  FDE   genlock_register_6_7 (
    .C(clk_148),
    .CE(genlock_register_6_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_6_7_2835)
  );
  FDE   genlock_register_4_0 (
    .C(clk_148),
    .CE(genlock_register_4_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_4_0_2810)
  );
  FDE   genlock_register_4_1 (
    .C(clk_148),
    .CE(genlock_register_4_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_4_1_2811)
  );
  FDE   genlock_register_4_2 (
    .C(clk_148),
    .CE(genlock_register_4_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_4_2_2812)
  );
  FDE   genlock_register_4_3 (
    .C(clk_148),
    .CE(genlock_register_4_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_4_3_2813)
  );
  FDE   genlock_register_4_4 (
    .C(clk_148),
    .CE(genlock_register_4_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_4_4_2814)
  );
  FDE   genlock_register_4_5 (
    .C(clk_148),
    .CE(genlock_register_4_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_4_5_2815)
  );
  FDE   genlock_register_4_6 (
    .C(clk_148),
    .CE(genlock_register_4_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_4_6_2816)
  );
  FDE   genlock_register_4_7 (
    .C(clk_148),
    .CE(genlock_register_4_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_4_7_2817)
  );
  FDE   genlock_register_3_0 (
    .C(clk_148),
    .CE(genlock_register_3_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_3_0_2801)
  );
  FDE   genlock_register_3_1 (
    .C(clk_148),
    .CE(genlock_register_3_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_3_1_2802)
  );
  FDE   genlock_register_3_2 (
    .C(clk_148),
    .CE(genlock_register_3_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_3_2_2803)
  );
  FDE   genlock_register_3_3 (
    .C(clk_148),
    .CE(genlock_register_3_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_3_3_2804)
  );
  FDE   genlock_register_3_4 (
    .C(clk_148),
    .CE(genlock_register_3_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_3_4_2805)
  );
  FDE   genlock_register_3_5 (
    .C(clk_148),
    .CE(genlock_register_3_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_3_5_2806)
  );
  FDE   genlock_register_3_6 (
    .C(clk_148),
    .CE(genlock_register_3_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_3_6_2807)
  );
  FDE   genlock_register_3_7 (
    .C(clk_148),
    .CE(genlock_register_3_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_3_7_2808)
  );
  FDE   genlock_register_2_0 (
    .C(clk_148),
    .CE(genlock_register_2_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_2_0_2792)
  );
  FDE   genlock_register_2_1 (
    .C(clk_148),
    .CE(genlock_register_2_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_2_1_2793)
  );
  FDE   genlock_register_2_2 (
    .C(clk_148),
    .CE(genlock_register_2_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_2_2_2794)
  );
  FDE   genlock_register_2_3 (
    .C(clk_148),
    .CE(genlock_register_2_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_2_3_2795)
  );
  FDE   genlock_register_2_4 (
    .C(clk_148),
    .CE(genlock_register_2_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_2_4_2796)
  );
  FDE   genlock_register_2_5 (
    .C(clk_148),
    .CE(genlock_register_2_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_2_5_2797)
  );
  FDE   genlock_register_2_6 (
    .C(clk_148),
    .CE(genlock_register_2_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_2_6_2798)
  );
  FDE   genlock_register_2_7 (
    .C(clk_148),
    .CE(genlock_register_2_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_2_7_2799)
  );
  FDE   genlock_register_1_0 (
    .C(clk_148),
    .CE(genlock_register_1_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_1_0_2756)
  );
  FDE   genlock_register_1_1 (
    .C(clk_148),
    .CE(genlock_register_1_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_1_1_2757)
  );
  FDE   genlock_register_1_2 (
    .C(clk_148),
    .CE(genlock_register_1_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_1_2_2758)
  );
  FDE   genlock_register_1_3 (
    .C(clk_148),
    .CE(genlock_register_1_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_1_3_2759)
  );
  FDE   genlock_register_1_4 (
    .C(clk_148),
    .CE(genlock_register_1_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_1_4_2760)
  );
  FDE   genlock_register_1_5 (
    .C(clk_148),
    .CE(genlock_register_1_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_1_5_2761)
  );
  FDE   genlock_register_1_6 (
    .C(clk_148),
    .CE(genlock_register_1_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_1_6_2762)
  );
  FDE   genlock_register_1_7 (
    .C(clk_148),
    .CE(genlock_register_1_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_1_7_2763)
  );
  FDE   genlock_register_0_0 (
    .C(clk_148),
    .CE(genlock_register_0_and0000),
    .D(\serial_interface/recieved_byte_o [0]),
    .Q(genlock_register_0_0_2657)
  );
  FDE   genlock_register_0_1 (
    .C(clk_148),
    .CE(genlock_register_0_and0000),
    .D(\serial_interface/recieved_byte_o [1]),
    .Q(genlock_register_0_1_2658)
  );
  FDE   genlock_register_0_2 (
    .C(clk_148),
    .CE(genlock_register_0_and0000),
    .D(\serial_interface/recieved_byte_o [2]),
    .Q(genlock_register_0_2_2659)
  );
  FDE   genlock_register_0_3 (
    .C(clk_148),
    .CE(genlock_register_0_and0000),
    .D(\serial_interface/recieved_byte_o [3]),
    .Q(genlock_register_0_3_2660)
  );
  FDE   genlock_register_0_4 (
    .C(clk_148),
    .CE(genlock_register_0_and0000),
    .D(\serial_interface/recieved_byte_o [4]),
    .Q(genlock_register_0_4_2661)
  );
  FDE   genlock_register_0_5 (
    .C(clk_148),
    .CE(genlock_register_0_and0000),
    .D(\serial_interface/recieved_byte_o [5]),
    .Q(genlock_register_0_5_2662)
  );
  FDE   genlock_register_0_6 (
    .C(clk_148),
    .CE(genlock_register_0_and0000),
    .D(\serial_interface/recieved_byte_o [6]),
    .Q(genlock_register_0_6_2663)
  );
  FDE   genlock_register_0_7 (
    .C(clk_148),
    .CE(genlock_register_0_and0000),
    .D(\serial_interface/recieved_byte_o [7]),
    .Q(genlock_register_0_7_2664)
  );
  FDE   LTC_a_delay_27 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_10_3_2669),
    .Q(LTC_a_delay[27])
  );
  FDE   LTC_a_delay_28 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_10_4_2670),
    .Q(LTC_a_delay[28])
  );
  FDE   LTC_a_delay_29 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_10_5_2671),
    .Q(LTC_a_delay[29])
  );
  FDE   LTC_a_secs_0 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_5_0_2819),
    .Q(LTC_a_secs[0])
  );
  FDE   LTC_a_secs_1 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_5_1_2820),
    .Q(LTC_a_secs[1])
  );
  FDE   LTC_a_secs_2 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_5_2_2821),
    .Q(LTC_a_secs[2])
  );
  FDE   LTC_a_secs_3 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_5_3_2822),
    .Q(LTC_a_secs[3])
  );
  FDE   LTC_a_secs_4 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_5_4_2823),
    .Q(LTC_a_secs[4])
  );
  FDE   LTC_a_secs_5 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_5_5_2824),
    .Q(LTC_a_secs[5])
  );
  FDE   LTC_a_secs_6 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_5_6_2825),
    .Q(LTC_a_secs[6])
  );
  FDE   LTC_a_frames_0 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_6_0_2828),
    .Q(LTC_a_frames[0])
  );
  FDE   LTC_a_frames_1 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_6_1_2829),
    .Q(LTC_a_frames[1])
  );
  FDE   LTC_a_frames_2 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_6_2_2830),
    .Q(LTC_a_frames[2])
  );
  FDE   LTC_a_frames_3 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_6_3_2831),
    .Q(LTC_a_frames[3])
  );
  FDE   LTC_a_frames_4 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_6_4_2832),
    .Q(LTC_a_frames[4])
  );
  FDE   LTC_a_frames_5 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_6_5_2833),
    .Q(LTC_a_frames[5])
  );
  FDE   LTC_b_secs_0 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_13_0_2693),
    .Q(LTC_b_secs[0])
  );
  FDE   LTC_b_secs_1 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_13_1_2694),
    .Q(LTC_b_secs[1])
  );
  FDE   LTC_b_secs_2 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_13_2_2695),
    .Q(LTC_b_secs[2])
  );
  FDE   LTC_b_secs_3 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_13_3_2696),
    .Q(LTC_b_secs[3])
  );
  FDE   LTC_b_secs_4 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_13_4_2697),
    .Q(LTC_b_secs[4])
  );
  FDE   LTC_b_secs_5 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_13_5_2698),
    .Q(LTC_b_secs[5])
  );
  FDE   LTC_b_secs_6 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_13_6_2699),
    .Q(LTC_b_secs[6])
  );
  FDE   LTC_a_hours_0 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_3_0_2801),
    .Q(LTC_a_hours[0])
  );
  FDE   LTC_a_hours_1 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_3_1_2802),
    .Q(LTC_a_hours[1])
  );
  FDE   LTC_a_hours_2 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_3_2_2803),
    .Q(LTC_a_hours[2])
  );
  FDE   LTC_a_hours_3 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_3_3_2804),
    .Q(LTC_a_hours[3])
  );
  FDE   LTC_a_hours_4 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_3_4_2805),
    .Q(LTC_a_hours[4])
  );
  FDE   LTC_a_hours_5 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_3_5_2806),
    .Q(LTC_a_hours[5])
  );
  FDE   LTC_b_delay_10 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_16_2_2722),
    .Q(LTC_b_delay[10])
  );
  FDE   LTC_b_delay_11 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_16_3_2723),
    .Q(LTC_b_delay[11])
  );
  FDE   LTC_b_delay_12 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_16_4_2724),
    .Q(LTC_b_delay[12])
  );
  FDE   LTC_b_delay_13 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_16_5_2725),
    .Q(LTC_b_delay[13])
  );
  FDE   LTC_b_delay_14 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_16_6_2726),
    .Q(LTC_b_delay[14])
  );
  FDE   LTC_b_delay_20 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_17_4_2733),
    .Q(LTC_b_delay[20])
  );
  FDE   LTC_b_delay_15 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_16_7_2727),
    .Q(LTC_b_delay[15])
  );
  FDE   LTC_b_delay_21 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_17_5_2734),
    .Q(LTC_b_delay[21])
  );
  FDE   LTC_b_delay_16 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_17_0_2729),
    .Q(LTC_b_delay[16])
  );
  FDE   LTC_b_hours_0 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_11_0_2675),
    .Q(LTC_b_hours[0])
  );
  FDE   LTC_b_hours_1 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_11_1_2676),
    .Q(LTC_b_hours[1])
  );
  FDE   LTC_b_hours_2 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_11_2_2677),
    .Q(LTC_b_hours[2])
  );
  FDE   LTC_b_hours_3 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_11_3_2678),
    .Q(LTC_b_hours[3])
  );
  FDE   LTC_b_hours_4 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_11_4_2679),
    .Q(LTC_b_hours[4])
  );
  FDE   LTC_b_hours_5 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_11_5_2680),
    .Q(LTC_b_hours[5])
  );
  FDE   LTC_b_delay_17 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_17_1_2730),
    .Q(LTC_b_delay[17])
  );
  FDE   LTC_b_delay_22 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_17_6_2735),
    .Q(LTC_b_delay[22])
  );
  FDE   LTC_b_delay_23 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_17_7_2736),
    .Q(LTC_b_delay[23])
  );
  FDE   LTC_b_delay_18 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_17_2_2731),
    .Q(LTC_b_delay[18])
  );
  FDE   LTC_b_delay_24 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_18_0_2738),
    .Q(LTC_b_delay[24])
  );
  FDE   LTC_b_delay_19 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_17_3_2732),
    .Q(LTC_b_delay[19])
  );
  FDE   LTC_b_delay_25 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_18_1_2739),
    .Q(LTC_b_delay[25])
  );
  FDE   LTC_b_delay_30 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_18_6_2744),
    .Q(LTC_b_delay[30])
  );
  FDE   LTC_b_delay_31 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_18_7_2745),
    .Q(LTC_b_delay[31])
  );
  FDE   LTC_b_delay_26 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_18_2_2740),
    .Q(LTC_b_delay[26])
  );
  FDE   LTC_b_delay_27 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_18_3_2741),
    .Q(LTC_b_delay[27])
  );
  FDE   LTC_b_delay_28 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_18_4_2742),
    .Q(LTC_b_delay[28])
  );
  FDE   LTC_a_mins_0 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_4_0_2810),
    .Q(LTC_a_mins[0])
  );
  FDE   LTC_a_mins_1 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_4_1_2811),
    .Q(LTC_a_mins[1])
  );
  FDE   LTC_a_mins_2 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_4_2_2812),
    .Q(LTC_a_mins[2])
  );
  FDE   LTC_a_mins_3 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_4_3_2813),
    .Q(LTC_a_mins[3])
  );
  FDE   LTC_a_mins_4 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_4_4_2814),
    .Q(LTC_a_mins[4])
  );
  FDE   LTC_a_mins_5 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_4_5_2815),
    .Q(LTC_a_mins[5])
  );
  FDE   LTC_a_mins_6 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_4_6_2816),
    .Q(LTC_a_mins[6])
  );
  FDE   LTC_b_delay_29 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_18_5_2743),
    .Q(LTC_b_delay[29])
  );
  FDE   LTC_setup_0 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_19_0_2747),
    .Q(\LTC_setup[0] )
  );
  FDE   LTC_setup_1 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_19_1_2748),
    .Q(\LTC_setup[1] )
  );
  FDE   LTC_setup_2 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_19_2_2749),
    .Q(\LTC_setup[2] )
  );
  FDE   LTC_setup_4 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_19_4_2751),
    .Q(\LTC_setup[4] )
  );
  FDE   LTC_setup_5 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_19_5_2752),
    .Q(\LTC_setup[5] )
  );
  FDE   LTC_setup_6 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_19_6_2753),
    .Q(\LTC_setup[6] )
  );
  FDE   LTC_b_mins_0 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_12_0_2684),
    .Q(LTC_b_mins[0])
  );
  FDE   LTC_b_mins_1 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_12_1_2685),
    .Q(LTC_b_mins[1])
  );
  FDE   LTC_b_mins_2 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_12_2_2686),
    .Q(LTC_b_mins[2])
  );
  FDE   LTC_b_mins_3 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_12_3_2687),
    .Q(LTC_b_mins[3])
  );
  FDE   LTC_b_mins_4 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_12_4_2688),
    .Q(LTC_b_mins[4])
  );
  FDE   LTC_b_mins_5 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_12_5_2689),
    .Q(LTC_b_mins[5])
  );
  FDE   LTC_b_mins_6 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_12_6_2690),
    .Q(LTC_b_mins[6])
  );
  FDE   LTC_b_delay_2 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_15_2_2713),
    .Q(LTC_b_delay[2])
  );
  FDE   LTC_b_delay_0 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_15_0_2711),
    .Q(LTC_b_delay[0])
  );
  FDE   LTC_b_delay_1 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_15_1_2712),
    .Q(LTC_b_delay[1])
  );
  FDE   LTC_b_delay_3 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_15_3_2714),
    .Q(LTC_b_delay[3])
  );
  FDE   LTC_b_delay_4 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_15_4_2715),
    .Q(LTC_b_delay[4])
  );
  FDE   LTC_b_delay_7 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_15_7_2718),
    .Q(LTC_b_delay[7])
  );
  FDE   LTC_b_delay_5 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_15_5_2716),
    .Q(LTC_b_delay[5])
  );
  FDE   LTC_b_delay_6 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_15_6_2717),
    .Q(LTC_b_delay[6])
  );
  FDE   LTC_b_delay_8 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_16_0_2720),
    .Q(LTC_b_delay[8])
  );
  FDE   LTC_a_delay_0 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_7_0_2837),
    .Q(LTC_a_delay[0])
  );
  FDE   system_control_0 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_20_0_2765),
    .Q(\system_control[0] )
  );
  FDE   system_control_2 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_20_2_2767),
    .Q(\system_control[2] )
  );
  FDE   system_control_3 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_20_3_2768),
    .Q(\system_control[3] )
  );
  FDE   system_control_4 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_20_4_2769),
    .Q(\system_control[4] )
  );
  FDE   LTC_b_delay_9 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_16_1_2721),
    .Q(LTC_b_delay[9])
  );
  FDE   LTC_a_delay_1 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_7_1_2838),
    .Q(LTC_a_delay[1])
  );
  FDE   LTC_a_delay_2 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_7_2_2839),
    .Q(LTC_a_delay[2])
  );
  FDE   LTC_a_delay_4 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_7_4_2841),
    .Q(LTC_a_delay[4])
  );
  FDE   LTC_a_delay_3 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_7_3_2840),
    .Q(LTC_a_delay[3])
  );
  FDSE   longer_tick (
    .C(clk_148),
    .CE(longer_tick_not0001_inv),
    .D(tp2_o_OBUF_3479),
    .S(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(longer_tick_3023)
  );
  FDE   LTC_a_delay_5 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_7_5_2842),
    .Q(LTC_a_delay[5])
  );
  FDE   LTC_a_delay_10 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_8_2_2848),
    .Q(LTC_a_delay[10])
  );
  FDE   LTC_a_delay_7 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_7_7_2844),
    .Q(LTC_a_delay[7])
  );
  FDE   LTC_a_delay_6 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_7_6_2843),
    .Q(LTC_a_delay[6])
  );
  FDE   LTC_a_delay_11 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_8_3_2849),
    .Q(LTC_a_delay[11])
  );
  FDE   LTC_a_delay_12 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_8_4_2850),
    .Q(LTC_a_delay[12])
  );
  FDE   LTC_a_delay_8 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_8_0_2846),
    .Q(LTC_a_delay[8])
  );
  FDE   LTC_a_delay_14 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_8_6_2852),
    .Q(LTC_a_delay[14])
  );
  FDE   LTC_a_delay_13 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_8_5_2851),
    .Q(LTC_a_delay[13])
  );
  FDE   LTC_a_delay_9 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_8_1_2847),
    .Q(LTC_a_delay[9])
  );
  FDE   LTC_a_delay_20 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_9_4_2859),
    .Q(LTC_a_delay[20])
  );
  FDE   LTC_a_delay_15 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_8_7_2853),
    .Q(LTC_a_delay[15])
  );
  FDE   LTC_a_delay_16 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_9_0_2855),
    .Q(LTC_a_delay[16])
  );
  FDE   LTC_b_frames_0 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_14_0_2702),
    .Q(LTC_b_frames[0])
  );
  FDE   LTC_b_frames_1 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_14_1_2703),
    .Q(LTC_b_frames[1])
  );
  FDE   LTC_b_frames_2 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_14_2_2704),
    .Q(LTC_b_frames[2])
  );
  FDE   LTC_b_frames_3 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_14_3_2705),
    .Q(LTC_b_frames[3])
  );
  FDE   LTC_b_frames_4 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_14_4_2706),
    .Q(LTC_b_frames[4])
  );
  FDE   LTC_b_frames_5 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_14_5_2707),
    .Q(LTC_b_frames[5])
  );
  FDE   LTC_a_delay_21 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_9_5_2860),
    .Q(LTC_a_delay[21])
  );
  FDE   LTC_a_delay_17 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_9_1_2856),
    .Q(LTC_a_delay[17])
  );
  FDE   LTC_a_delay_22 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_9_6_2861),
    .Q(LTC_a_delay[22])
  );
  FDE   LTC_a_delay_24 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_10_0_2666),
    .Q(LTC_a_delay[24])
  );
  FDE   LTC_a_delay_18 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_9_2_2857),
    .Q(LTC_a_delay[18])
  );
  FDE   LTC_a_delay_23 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_9_7_2862),
    .Q(LTC_a_delay[23])
  );
  FDE   LTC_a_delay_19 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_9_3_2858),
    .Q(LTC_a_delay[19])
  );
  FDE   LTC_a_delay_31 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_10_7_2673),
    .Q(LTC_a_delay[31])
  );
  FDE   LTC_a_delay_30 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_10_6_2672),
    .Q(LTC_a_delay[30])
  );
  FDE   LTC_a_delay_25 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_10_1_2667),
    .Q(LTC_a_delay[25])
  );
  FDE   LTC_a_delay_26 (
    .C(clk_148),
    .CE(\VCXO_pll/pps_148_tick_o_2290 ),
    .D(genlock_register_10_2_2668),
    .Q(LTC_a_delay[26])
  );
  FDRE   \f_gen/pulse_state  (
    .C(clk_148),
    .CE(\f_gen/pulse_state_cmp_eq0000 ),
    .D(\f_gen/pulse_state_mux0001 ),
    .R(tp2_o_OBUF_3479),
    .Q(\f_gen/pulse_state_2639 )
  );
  FD   \f_gen/count_24  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[24] ),
    .Q(\f_gen/count [24])
  );
  FD   \f_gen/count_23  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[23] ),
    .Q(\f_gen/count [23])
  );
  FD   \f_gen/count_22  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[22] ),
    .Q(\f_gen/count [22])
  );
  FD   \f_gen/count_21  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[21] ),
    .Q(\f_gen/count [21])
  );
  FD   \f_gen/count_20  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[20] ),
    .Q(\f_gen/count [20])
  );
  FD   \f_gen/count_19  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[19] ),
    .Q(\f_gen/count [19])
  );
  FD   \f_gen/count_18  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[18] ),
    .Q(\f_gen/count [18])
  );
  FD   \f_gen/count_17  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[17] ),
    .Q(\f_gen/count [17])
  );
  FD   \f_gen/count_16  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[16] ),
    .Q(\f_gen/count [16])
  );
  FD   \f_gen/count_14  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[14] ),
    .Q(\f_gen/count [14])
  );
  FD   \f_gen/count_13  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[13] ),
    .Q(\f_gen/count [13])
  );
  FD   \f_gen/count_12  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[12] ),
    .Q(\f_gen/count [12])
  );
  FD   \f_gen/count_11  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[11] ),
    .Q(\f_gen/count [11])
  );
  FD   \f_gen/count_8  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[8] ),
    .Q(\f_gen/count [8])
  );
  FD   \f_gen/count_6  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[6] ),
    .Q(\f_gen/count [6])
  );
  FD   \f_gen/count_5  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[5] ),
    .Q(\f_gen/count [5])
  );
  FD   \f_gen/count_4  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[4] ),
    .Q(\f_gen/count [4])
  );
  FD   \f_gen/count_3  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[3] ),
    .Q(\f_gen/count [3])
  );
  FD   \f_gen/count_2  (
    .C(clk_148),
    .D(\f_gen/count_mux0003[2] ),
    .Q(\f_gen/count [2])
  );
  FDRE   \h_gen/pulse_state  (
    .C(clk_148),
    .CE(\h_gen/pulse_state_cmp_eq0000 ),
    .D(\h_gen/pulse_state_mux0001 ),
    .R(tp2_o_OBUF_3479),
    .Q(\h_gen/pulse_state_3000 )
  );
  FD   \h_gen/count_12  (
    .C(clk_148),
    .D(\h_gen/count_mux0003[12] ),
    .Q(\h_gen/count [12])
  );
  FD   \h_gen/count_11  (
    .C(clk_148),
    .D(\h_gen/count_mux0003[11] ),
    .Q(\h_gen/count [11])
  );
  FD   \h_gen/count_7  (
    .C(clk_148),
    .D(\h_gen/count_mux0003[7] ),
    .Q(\h_gen/count [7])
  );
  FD   \h_gen/count_6  (
    .C(clk_148),
    .D(\h_gen/count_mux0003[6] ),
    .Q(\h_gen/count [6])
  );
  FD   \h_gen/count_4  (
    .C(clk_148),
    .D(\h_gen/count_mux0003[4] ),
    .Q(\h_gen/count [4])
  );
  FD   \h_gen/count_3  (
    .C(clk_148),
    .D(\h_gen/count_mux0003[3] ),
    .Q(\h_gen/count [3])
  );
  FD   \h_gen/count_2  (
    .C(clk_148),
    .D(\h_gen/count_mux0003[2] ),
    .Q(\h_gen/count [2])
  );
  FD   \h_gen/count_1  (
    .C(clk_148),
    .D(\h_gen/count_mux0003[1] ),
    .Q(\h_gen/count [1])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_0 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[0]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[0])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_1 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[1]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[1])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_2 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[2]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[2])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_3 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[3]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[3])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_4 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[4]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[4])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_5 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[5]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[5])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_6 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[6]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[6])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_7 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[7]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[7])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_8 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[8]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[8])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_9 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[9]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[9])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_10 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[10]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[10])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_11 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[11]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[11])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_12 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[12]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[12])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_13 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[13]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[13])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_14 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[14]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[14])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_15 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[15]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[15])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_16 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[16]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[16])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_17 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[17]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[17])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_18 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[18]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[18])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_19 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[19]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[19])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_20 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[20]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[20])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_21 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[21]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[21])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_22 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[22]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[22])
  );
  FDR #(
    .INIT ( 1'b0 ))
  pps_status_count_23 (
    .C(clk_10_i_IBUFG_2312),
    .D(Result[23]),
    .R(pps_status_count_or0000),
    .Q(pps_status_count[23])
  );
  FDSE #(
    .INIT ( 1'b0 ))
  count_0 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<0>1 ),
    .S(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[0])
  );
  FDSE #(
    .INIT ( 1'b0 ))
  count_1 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<1>1 ),
    .S(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[1])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  count_4 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<4>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[4])
  );
  FDSE #(
    .INIT ( 1'b0 ))
  count_2 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<2>1 ),
    .S(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[2])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  count_3 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<3>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[3])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  count_5 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<5>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[5])
  );
  FDSE #(
    .INIT ( 1'b0 ))
  count_6 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<6>1 ),
    .S(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[6])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  count_9 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<9>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[9])
  );
  FDSE #(
    .INIT ( 1'b0 ))
  count_7 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<7>1 ),
    .S(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[7])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  count_8 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<8>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[8])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  count_10 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<10>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[10])
  );
  FDSE #(
    .INIT ( 1'b0 ))
  count_11 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<11>1 ),
    .S(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[11])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  count_14 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<14>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[14])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  count_12 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<12>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[12])
  );
  FDSE #(
    .INIT ( 1'b0 ))
  count_13 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<13>1 ),
    .S(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[13])
  );
  FDSE #(
    .INIT ( 1'b0 ))
  count_15 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<15>1 ),
    .S(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[15])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  count_16 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<16>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[16])
  );
  FDSE #(
    .INIT ( 1'b0 ))
  count_17 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<17>1 ),
    .S(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[17])
  );
  FDSE #(
    .INIT ( 1'b0 ))
  count_18 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<18>1 ),
    .S(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[18])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  count_19 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<19>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[19])
  );
  FDSE #(
    .INIT ( 1'b0 ))
  count_20 (
    .C(clk_148),
    .CE(longer_tick_not0001),
    .D(\Result<20>1 ),
    .S(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(count[20])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \Mcompar_pps_status_cmp_gt0000_lut<0>  (
    .I0(pps_status_count[0]),
    .I1(pps_status_count[1]),
    .I2(pps_status_count[2]),
    .I3(pps_status_count[3]),
    .O(\Mcompar_pps_status_cmp_gt0000_lut[0] )
  );
  MUXCY   \Mcompar_pps_status_cmp_gt0000_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcompar_pps_status_cmp_gt0000_lut[0] ),
    .O(Mcompar_pps_status_cmp_gt0000_cy[0])
  );
  MUXCY   \Mcompar_pps_status_cmp_gt0000_cy<1>  (
    .CI(Mcompar_pps_status_cmp_gt0000_cy[0]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcompar_pps_status_cmp_gt0000_lut[1] ),
    .O(Mcompar_pps_status_cmp_gt0000_cy[1])
  );
  MUXCY   \Mcompar_pps_status_cmp_gt0000_cy<2>  (
    .CI(Mcompar_pps_status_cmp_gt0000_cy[1]),
    .DI(N1),
    .S(\Mcompar_pps_status_cmp_gt0000_cy<2>_rt_1116 ),
    .O(Mcompar_pps_status_cmp_gt0000_cy[2])
  );
  MUXCY   \Mcompar_pps_status_cmp_gt0000_cy<3>  (
    .CI(Mcompar_pps_status_cmp_gt0000_cy[2]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcompar_pps_status_cmp_gt0000_lut[3] ),
    .O(Mcompar_pps_status_cmp_gt0000_cy[3])
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \Mcompar_pps_status_cmp_gt0000_lut<4>  (
    .I0(pps_status_count[7]),
    .I1(pps_status_count[8]),
    .I2(pps_status_count[9]),
    .I3(pps_status_count[10]),
    .O(\Mcompar_pps_status_cmp_gt0000_lut[4] )
  );
  MUXCY   \Mcompar_pps_status_cmp_gt0000_cy<4>  (
    .CI(Mcompar_pps_status_cmp_gt0000_cy[3]),
    .DI(N1),
    .S(\Mcompar_pps_status_cmp_gt0000_lut[4] ),
    .O(Mcompar_pps_status_cmp_gt0000_cy[4])
  );
  MUXCY   \Mcompar_pps_status_cmp_gt0000_cy<5>  (
    .CI(Mcompar_pps_status_cmp_gt0000_cy[4]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcompar_pps_status_cmp_gt0000_lut[5] ),
    .O(Mcompar_pps_status_cmp_gt0000_cy[5])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Mcompar_pps_status_cmp_gt0000_lut<6>  (
    .I0(pps_status_count[12]),
    .I1(pps_status_count[13]),
    .O(\Mcompar_pps_status_cmp_gt0000_lut[6] )
  );
  MUXCY   \Mcompar_pps_status_cmp_gt0000_cy<6>  (
    .CI(Mcompar_pps_status_cmp_gt0000_cy[5]),
    .DI(N1),
    .S(\Mcompar_pps_status_cmp_gt0000_lut[6] ),
    .O(Mcompar_pps_status_cmp_gt0000_cy[6])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \Mcompar_pps_status_cmp_gt0000_lut<7>  (
    .I0(pps_status_count[14]),
    .I1(pps_status_count[15]),
    .I2(pps_status_count[16]),
    .I3(pps_status_count[17]),
    .O(\Mcompar_pps_status_cmp_gt0000_lut[7] )
  );
  MUXCY   \Mcompar_pps_status_cmp_gt0000_cy<7>  (
    .CI(Mcompar_pps_status_cmp_gt0000_cy[6]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcompar_pps_status_cmp_gt0000_lut[7] ),
    .O(Mcompar_pps_status_cmp_gt0000_cy[7])
  );
  LUT3 #(
    .INIT ( 8'h01 ))
  \Mcompar_pps_status_cmp_gt0000_lut<8>  (
    .I0(pps_status_count[18]),
    .I1(pps_status_count[19]),
    .I2(pps_status_count[20]),
    .O(\Mcompar_pps_status_cmp_gt0000_lut[8] )
  );
  MUXCY   \Mcompar_pps_status_cmp_gt0000_cy<8>  (
    .CI(Mcompar_pps_status_cmp_gt0000_cy[7]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcompar_pps_status_cmp_gt0000_lut[8] ),
    .O(Mcompar_pps_status_cmp_gt0000_cy[8])
  );
  MUXCY   \Mcompar_pps_status_cmp_gt0000_cy<9>  (
    .CI(Mcompar_pps_status_cmp_gt0000_cy[8]),
    .DI(N1),
    .S(\Mcompar_pps_status_cmp_gt0000_cy<9>_rt_1124 ),
    .O(Mcompar_pps_status_cmp_gt0000_cy[9])
  );
  MUXCY   \Mcompar_pps_status_cmp_gt0000_cy<10>  (
    .CI(Mcompar_pps_status_cmp_gt0000_cy[9]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcompar_pps_status_cmp_gt0000_lut[10] ),
    .O(Mcompar_pps_status_cmp_gt0000_cy[10])
  );
  MUXCY   \Mcompar_pps_status_cmp_gt0000_cy<11>  (
    .CI(Mcompar_pps_status_cmp_gt0000_cy[10]),
    .DI(N1),
    .S(\Mcompar_pps_status_cmp_gt0000_cy<11>_rt_1113 ),
    .O(Mcompar_pps_status_cmp_gt0000_cy[11])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\f_gen/Msub_count_addsub0000_cy<0>_rt_2494 ),
    .O(\f_gen/Msub_count_addsub0000_cy [0])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<0>  (
    .CI(N1),
    .LI(\f_gen/Msub_count_addsub0000_cy<0>_rt_2494 ),
    .O(\f_gen/count_addsub0000 [0])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<1>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [0]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [1]),
    .O(\f_gen/Msub_count_addsub0000_cy [1])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<1>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [0]),
    .LI(\f_gen/Msub_count_addsub0000_lut [1]),
    .O(\f_gen/count_addsub0000 [1])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<2>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [1]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [2]),
    .O(\f_gen/Msub_count_addsub0000_cy [2])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<2>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [1]),
    .LI(\f_gen/Msub_count_addsub0000_lut [2]),
    .O(\f_gen/count_addsub0000 [2])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<3>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [2]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [3]),
    .O(\f_gen/Msub_count_addsub0000_cy [3])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<3>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [2]),
    .LI(\f_gen/Msub_count_addsub0000_lut [3]),
    .O(\f_gen/count_addsub0000 [3])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<4>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [3]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [4]),
    .O(\f_gen/Msub_count_addsub0000_cy [4])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<4>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [3]),
    .LI(\f_gen/Msub_count_addsub0000_lut [4]),
    .O(\f_gen/count_addsub0000 [4])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<5>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [4]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [5]),
    .O(\f_gen/Msub_count_addsub0000_cy [5])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<5>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [4]),
    .LI(\f_gen/Msub_count_addsub0000_lut [5]),
    .O(\f_gen/count_addsub0000 [5])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<6>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [5]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [6]),
    .O(\f_gen/Msub_count_addsub0000_cy [6])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<6>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [5]),
    .LI(\f_gen/Msub_count_addsub0000_lut [6]),
    .O(\f_gen/count_addsub0000 [6])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<7>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [6]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [7]),
    .O(\f_gen/Msub_count_addsub0000_cy [7])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<7>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [6]),
    .LI(\f_gen/Msub_count_addsub0000_lut [7]),
    .O(\f_gen/count_addsub0000 [7])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<8>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [7]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [8]),
    .O(\f_gen/Msub_count_addsub0000_cy [8])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<8>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [7]),
    .LI(\f_gen/Msub_count_addsub0000_lut [8]),
    .O(\f_gen/count_addsub0000 [8])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<9>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [8]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [9]),
    .O(\f_gen/Msub_count_addsub0000_cy [9])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<9>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [8]),
    .LI(\f_gen/Msub_count_addsub0000_lut [9]),
    .O(\f_gen/count_addsub0000 [9])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<10>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [9]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [10]),
    .O(\f_gen/Msub_count_addsub0000_cy [10])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<10>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [9]),
    .LI(\f_gen/Msub_count_addsub0000_lut [10]),
    .O(\f_gen/count_addsub0000 [10])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<11>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [10]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [11]),
    .O(\f_gen/Msub_count_addsub0000_cy [11])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<11>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [10]),
    .LI(\f_gen/Msub_count_addsub0000_lut [11]),
    .O(\f_gen/count_addsub0000 [11])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<12>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [11]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [12]),
    .O(\f_gen/Msub_count_addsub0000_cy [12])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<12>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [11]),
    .LI(\f_gen/Msub_count_addsub0000_lut [12]),
    .O(\f_gen/count_addsub0000 [12])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<13>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [12]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [13]),
    .O(\f_gen/Msub_count_addsub0000_cy [13])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<13>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [12]),
    .LI(\f_gen/Msub_count_addsub0000_lut [13]),
    .O(\f_gen/count_addsub0000 [13])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<14>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [13]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [14]),
    .O(\f_gen/Msub_count_addsub0000_cy [14])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<14>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [13]),
    .LI(\f_gen/Msub_count_addsub0000_lut [14]),
    .O(\f_gen/count_addsub0000 [14])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<15>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [14]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [15]),
    .O(\f_gen/Msub_count_addsub0000_cy [15])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<15>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [14]),
    .LI(\f_gen/Msub_count_addsub0000_lut [15]),
    .O(\f_gen/count_addsub0000 [15])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<16>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [15]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [16]),
    .O(\f_gen/Msub_count_addsub0000_cy [16])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<16>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [15]),
    .LI(\f_gen/Msub_count_addsub0000_lut [16]),
    .O(\f_gen/count_addsub0000 [16])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<17>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [16]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [17]),
    .O(\f_gen/Msub_count_addsub0000_cy [17])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<17>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [16]),
    .LI(\f_gen/Msub_count_addsub0000_lut [17]),
    .O(\f_gen/count_addsub0000 [17])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<18>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [17]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [18]),
    .O(\f_gen/Msub_count_addsub0000_cy [18])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<18>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [17]),
    .LI(\f_gen/Msub_count_addsub0000_lut [18]),
    .O(\f_gen/count_addsub0000 [18])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<19>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [18]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [19]),
    .O(\f_gen/Msub_count_addsub0000_cy [19])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<19>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [18]),
    .LI(\f_gen/Msub_count_addsub0000_lut [19]),
    .O(\f_gen/count_addsub0000 [19])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<20>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [19]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [20]),
    .O(\f_gen/Msub_count_addsub0000_cy [20])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<20>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [19]),
    .LI(\f_gen/Msub_count_addsub0000_lut [20]),
    .O(\f_gen/count_addsub0000 [20])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<21>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [20]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [21]),
    .O(\f_gen/Msub_count_addsub0000_cy [21])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<21>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [20]),
    .LI(\f_gen/Msub_count_addsub0000_lut [21]),
    .O(\f_gen/count_addsub0000 [21])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<22>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [21]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [22]),
    .O(\f_gen/Msub_count_addsub0000_cy [22])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<22>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [21]),
    .LI(\f_gen/Msub_count_addsub0000_lut [22]),
    .O(\f_gen/count_addsub0000 [22])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<23>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [22]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [23]),
    .O(\f_gen/Msub_count_addsub0000_cy [23])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<23>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [22]),
    .LI(\f_gen/Msub_count_addsub0000_lut [23]),
    .O(\f_gen/count_addsub0000 [23])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<24>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [23]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [24]),
    .O(\f_gen/Msub_count_addsub0000_cy [24])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<24>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [23]),
    .LI(\f_gen/Msub_count_addsub0000_lut [24]),
    .O(\f_gen/count_addsub0000 [24])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<25>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [24]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [25]),
    .O(\f_gen/Msub_count_addsub0000_cy [25])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<25>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [24]),
    .LI(\f_gen/Msub_count_addsub0000_lut [25]),
    .O(\f_gen/count_addsub0000 [25])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<26>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [25]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [26]),
    .O(\f_gen/Msub_count_addsub0000_cy [26])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<26>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [25]),
    .LI(\f_gen/Msub_count_addsub0000_lut [26]),
    .O(\f_gen/count_addsub0000 [26])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<27>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [26]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [27]),
    .O(\f_gen/Msub_count_addsub0000_cy [27])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<27>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [26]),
    .LI(\f_gen/Msub_count_addsub0000_lut [27]),
    .O(\f_gen/count_addsub0000 [27])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<28>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [27]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [28]),
    .O(\f_gen/Msub_count_addsub0000_cy [28])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<28>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [27]),
    .LI(\f_gen/Msub_count_addsub0000_lut [28]),
    .O(\f_gen/count_addsub0000 [28])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<29>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [28]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [29]),
    .O(\f_gen/Msub_count_addsub0000_cy [29])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<29>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [28]),
    .LI(\f_gen/Msub_count_addsub0000_lut [29]),
    .O(\f_gen/count_addsub0000 [29])
  );
  MUXCY   \f_gen/Msub_count_addsub0000_cy<30>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [29]),
    .DI(N1),
    .S(\f_gen/Msub_count_addsub0000_lut [30]),
    .O(\f_gen/Msub_count_addsub0000_cy [30])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<30>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [29]),
    .LI(\f_gen/Msub_count_addsub0000_lut [30]),
    .O(\f_gen/count_addsub0000 [30])
  );
  XORCY   \f_gen/Msub_count_addsub0000_xor<31>  (
    .CI(\f_gen/Msub_count_addsub0000_cy [30]),
    .LI(\f_gen/Msub_count_addsub0000_lut [31]),
    .O(\f_gen/count_addsub0000 [31])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\h_gen/Msub_count_addsub0000_cy<0>_rt_2866 ),
    .O(\h_gen/Msub_count_addsub0000_cy [0])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<0>  (
    .CI(N1),
    .LI(\h_gen/Msub_count_addsub0000_cy<0>_rt_2866 ),
    .O(\h_gen/count_addsub0000 [0])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<1>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [0]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [1]),
    .O(\h_gen/Msub_count_addsub0000_cy [1])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<1>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [0]),
    .LI(\h_gen/Msub_count_addsub0000_lut [1]),
    .O(\h_gen/count_addsub0000 [1])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<2>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [1]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [2]),
    .O(\h_gen/Msub_count_addsub0000_cy [2])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<2>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [1]),
    .LI(\h_gen/Msub_count_addsub0000_lut [2]),
    .O(\h_gen/count_addsub0000 [2])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<3>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [2]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [3]),
    .O(\h_gen/Msub_count_addsub0000_cy [3])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<3>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [2]),
    .LI(\h_gen/Msub_count_addsub0000_lut [3]),
    .O(\h_gen/count_addsub0000 [3])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<4>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [3]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [4]),
    .O(\h_gen/Msub_count_addsub0000_cy [4])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<4>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [3]),
    .LI(\h_gen/Msub_count_addsub0000_lut [4]),
    .O(\h_gen/count_addsub0000 [4])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<5>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [4]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [5]),
    .O(\h_gen/Msub_count_addsub0000_cy [5])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<5>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [4]),
    .LI(\h_gen/Msub_count_addsub0000_lut [5]),
    .O(\h_gen/count_addsub0000 [5])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<6>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [5]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [6]),
    .O(\h_gen/Msub_count_addsub0000_cy [6])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<6>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [5]),
    .LI(\h_gen/Msub_count_addsub0000_lut [6]),
    .O(\h_gen/count_addsub0000 [6])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<7>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [6]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [7]),
    .O(\h_gen/Msub_count_addsub0000_cy [7])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<7>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [6]),
    .LI(\h_gen/Msub_count_addsub0000_lut [7]),
    .O(\h_gen/count_addsub0000 [7])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<8>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [7]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [8]),
    .O(\h_gen/Msub_count_addsub0000_cy [8])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<8>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [7]),
    .LI(\h_gen/Msub_count_addsub0000_lut [8]),
    .O(\h_gen/count_addsub0000 [8])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<9>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [8]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [9]),
    .O(\h_gen/Msub_count_addsub0000_cy [9])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<9>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [8]),
    .LI(\h_gen/Msub_count_addsub0000_lut [9]),
    .O(\h_gen/count_addsub0000 [9])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<10>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [9]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [10]),
    .O(\h_gen/Msub_count_addsub0000_cy [10])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<10>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [9]),
    .LI(\h_gen/Msub_count_addsub0000_lut [10]),
    .O(\h_gen/count_addsub0000 [10])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<11>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [10]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [11]),
    .O(\h_gen/Msub_count_addsub0000_cy [11])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<11>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [10]),
    .LI(\h_gen/Msub_count_addsub0000_lut [11]),
    .O(\h_gen/count_addsub0000 [11])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<12>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [11]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [12]),
    .O(\h_gen/Msub_count_addsub0000_cy [12])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<12>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [11]),
    .LI(\h_gen/Msub_count_addsub0000_lut [12]),
    .O(\h_gen/count_addsub0000 [12])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<13>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [12]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [13]),
    .O(\h_gen/Msub_count_addsub0000_cy [13])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<13>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [12]),
    .LI(\h_gen/Msub_count_addsub0000_lut [13]),
    .O(\h_gen/count_addsub0000 [13])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<14>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [13]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [14]),
    .O(\h_gen/Msub_count_addsub0000_cy [14])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<14>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [13]),
    .LI(\h_gen/Msub_count_addsub0000_lut [14]),
    .O(\h_gen/count_addsub0000 [14])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<15>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [14]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [15]),
    .O(\h_gen/Msub_count_addsub0000_cy [15])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<15>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [14]),
    .LI(\h_gen/Msub_count_addsub0000_lut [15]),
    .O(\h_gen/count_addsub0000 [15])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<16>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [15]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [16]),
    .O(\h_gen/Msub_count_addsub0000_cy [16])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<16>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [15]),
    .LI(\h_gen/Msub_count_addsub0000_lut [16]),
    .O(\h_gen/count_addsub0000 [16])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<17>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [16]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [17]),
    .O(\h_gen/Msub_count_addsub0000_cy [17])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<17>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [16]),
    .LI(\h_gen/Msub_count_addsub0000_lut [17]),
    .O(\h_gen/count_addsub0000 [17])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<18>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [17]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [18]),
    .O(\h_gen/Msub_count_addsub0000_cy [18])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<18>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [17]),
    .LI(\h_gen/Msub_count_addsub0000_lut [18]),
    .O(\h_gen/count_addsub0000 [18])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<19>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [18]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [19]),
    .O(\h_gen/Msub_count_addsub0000_cy [19])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<19>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [18]),
    .LI(\h_gen/Msub_count_addsub0000_lut [19]),
    .O(\h_gen/count_addsub0000 [19])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<20>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [19]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [20]),
    .O(\h_gen/Msub_count_addsub0000_cy [20])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<20>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [19]),
    .LI(\h_gen/Msub_count_addsub0000_lut [20]),
    .O(\h_gen/count_addsub0000 [20])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<21>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [20]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [21]),
    .O(\h_gen/Msub_count_addsub0000_cy [21])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<21>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [20]),
    .LI(\h_gen/Msub_count_addsub0000_lut [21]),
    .O(\h_gen/count_addsub0000 [21])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<22>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [21]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [22]),
    .O(\h_gen/Msub_count_addsub0000_cy [22])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<22>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [21]),
    .LI(\h_gen/Msub_count_addsub0000_lut [22]),
    .O(\h_gen/count_addsub0000 [22])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<23>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [22]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [23]),
    .O(\h_gen/Msub_count_addsub0000_cy [23])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<23>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [22]),
    .LI(\h_gen/Msub_count_addsub0000_lut [23]),
    .O(\h_gen/count_addsub0000 [23])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<24>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [23]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [24]),
    .O(\h_gen/Msub_count_addsub0000_cy [24])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<24>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [23]),
    .LI(\h_gen/Msub_count_addsub0000_lut [24]),
    .O(\h_gen/count_addsub0000 [24])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<25>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [24]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [25]),
    .O(\h_gen/Msub_count_addsub0000_cy [25])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<25>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [24]),
    .LI(\h_gen/Msub_count_addsub0000_lut [25]),
    .O(\h_gen/count_addsub0000 [25])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<26>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [25]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [26]),
    .O(\h_gen/Msub_count_addsub0000_cy [26])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<26>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [25]),
    .LI(\h_gen/Msub_count_addsub0000_lut [26]),
    .O(\h_gen/count_addsub0000 [26])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<27>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [26]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [27]),
    .O(\h_gen/Msub_count_addsub0000_cy [27])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<27>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [26]),
    .LI(\h_gen/Msub_count_addsub0000_lut [27]),
    .O(\h_gen/count_addsub0000 [27])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<28>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [27]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [28]),
    .O(\h_gen/Msub_count_addsub0000_cy [28])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<28>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [27]),
    .LI(\h_gen/Msub_count_addsub0000_lut [28]),
    .O(\h_gen/count_addsub0000 [28])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<29>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [28]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [29]),
    .O(\h_gen/Msub_count_addsub0000_cy [29])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<29>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [28]),
    .LI(\h_gen/Msub_count_addsub0000_lut [29]),
    .O(\h_gen/count_addsub0000 [29])
  );
  MUXCY   \h_gen/Msub_count_addsub0000_cy<30>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [29]),
    .DI(N1),
    .S(\h_gen/Msub_count_addsub0000_lut [30]),
    .O(\h_gen/Msub_count_addsub0000_cy [30])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<30>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [29]),
    .LI(\h_gen/Msub_count_addsub0000_lut [30]),
    .O(\h_gen/count_addsub0000 [30])
  );
  XORCY   \h_gen/Msub_count_addsub0000_xor<31>  (
    .CI(\h_gen/Msub_count_addsub0000_cy [30]),
    .LI(\h_gen/Msub_count_addsub0000_lut [31]),
    .O(\h_gen/count_addsub0000 [31])
  );
  MUXCY   \Mcount_pps_status_count_cy<0>  (
    .CI(tp2_o_OBUF_3479),
    .DI(N1),
    .S(Mcount_pps_status_count_lut[0]),
    .O(Mcount_pps_status_count_cy[0])
  );
  XORCY   \Mcount_pps_status_count_xor<0>  (
    .CI(tp2_o_OBUF_3479),
    .LI(Mcount_pps_status_count_lut[0]),
    .O(Result[0])
  );
  MUXCY   \Mcount_pps_status_count_cy<1>  (
    .CI(Mcount_pps_status_count_cy[0]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<1>_rt_1197 ),
    .O(Mcount_pps_status_count_cy[1])
  );
  XORCY   \Mcount_pps_status_count_xor<1>  (
    .CI(Mcount_pps_status_count_cy[0]),
    .LI(\Mcount_pps_status_count_cy<1>_rt_1197 ),
    .O(Result[1])
  );
  MUXCY   \Mcount_pps_status_count_cy<2>  (
    .CI(Mcount_pps_status_count_cy[1]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<2>_rt_1205 ),
    .O(Mcount_pps_status_count_cy[2])
  );
  XORCY   \Mcount_pps_status_count_xor<2>  (
    .CI(Mcount_pps_status_count_cy[1]),
    .LI(\Mcount_pps_status_count_cy<2>_rt_1205 ),
    .O(Result[2])
  );
  MUXCY   \Mcount_pps_status_count_cy<3>  (
    .CI(Mcount_pps_status_count_cy[2]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<3>_rt_1207 ),
    .O(Mcount_pps_status_count_cy[3])
  );
  XORCY   \Mcount_pps_status_count_xor<3>  (
    .CI(Mcount_pps_status_count_cy[2]),
    .LI(\Mcount_pps_status_count_cy<3>_rt_1207 ),
    .O(Result[3])
  );
  MUXCY   \Mcount_pps_status_count_cy<4>  (
    .CI(Mcount_pps_status_count_cy[3]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<4>_rt_1209 ),
    .O(Mcount_pps_status_count_cy[4])
  );
  XORCY   \Mcount_pps_status_count_xor<4>  (
    .CI(Mcount_pps_status_count_cy[3]),
    .LI(\Mcount_pps_status_count_cy<4>_rt_1209 ),
    .O(Result[4])
  );
  MUXCY   \Mcount_pps_status_count_cy<5>  (
    .CI(Mcount_pps_status_count_cy[4]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<5>_rt_1211 ),
    .O(Mcount_pps_status_count_cy[5])
  );
  XORCY   \Mcount_pps_status_count_xor<5>  (
    .CI(Mcount_pps_status_count_cy[4]),
    .LI(\Mcount_pps_status_count_cy<5>_rt_1211 ),
    .O(Result[5])
  );
  MUXCY   \Mcount_pps_status_count_cy<6>  (
    .CI(Mcount_pps_status_count_cy[5]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<6>_rt_1213 ),
    .O(Mcount_pps_status_count_cy[6])
  );
  XORCY   \Mcount_pps_status_count_xor<6>  (
    .CI(Mcount_pps_status_count_cy[5]),
    .LI(\Mcount_pps_status_count_cy<6>_rt_1213 ),
    .O(Result[6])
  );
  MUXCY   \Mcount_pps_status_count_cy<7>  (
    .CI(Mcount_pps_status_count_cy[6]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<7>_rt_1215 ),
    .O(Mcount_pps_status_count_cy[7])
  );
  XORCY   \Mcount_pps_status_count_xor<7>  (
    .CI(Mcount_pps_status_count_cy[6]),
    .LI(\Mcount_pps_status_count_cy<7>_rt_1215 ),
    .O(Result[7])
  );
  MUXCY   \Mcount_pps_status_count_cy<8>  (
    .CI(Mcount_pps_status_count_cy[7]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<8>_rt_1217 ),
    .O(Mcount_pps_status_count_cy[8])
  );
  XORCY   \Mcount_pps_status_count_xor<8>  (
    .CI(Mcount_pps_status_count_cy[7]),
    .LI(\Mcount_pps_status_count_cy<8>_rt_1217 ),
    .O(Result[8])
  );
  MUXCY   \Mcount_pps_status_count_cy<9>  (
    .CI(Mcount_pps_status_count_cy[8]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<9>_rt_1219 ),
    .O(Mcount_pps_status_count_cy[9])
  );
  XORCY   \Mcount_pps_status_count_xor<9>  (
    .CI(Mcount_pps_status_count_cy[8]),
    .LI(\Mcount_pps_status_count_cy<9>_rt_1219 ),
    .O(Result[9])
  );
  MUXCY   \Mcount_pps_status_count_cy<10>  (
    .CI(Mcount_pps_status_count_cy[9]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<10>_rt_1177 ),
    .O(Mcount_pps_status_count_cy[10])
  );
  XORCY   \Mcount_pps_status_count_xor<10>  (
    .CI(Mcount_pps_status_count_cy[9]),
    .LI(\Mcount_pps_status_count_cy<10>_rt_1177 ),
    .O(Result[10])
  );
  MUXCY   \Mcount_pps_status_count_cy<11>  (
    .CI(Mcount_pps_status_count_cy[10]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<11>_rt_1179 ),
    .O(Mcount_pps_status_count_cy[11])
  );
  XORCY   \Mcount_pps_status_count_xor<11>  (
    .CI(Mcount_pps_status_count_cy[10]),
    .LI(\Mcount_pps_status_count_cy<11>_rt_1179 ),
    .O(Result[11])
  );
  MUXCY   \Mcount_pps_status_count_cy<12>  (
    .CI(Mcount_pps_status_count_cy[11]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<12>_rt_1181 ),
    .O(Mcount_pps_status_count_cy[12])
  );
  XORCY   \Mcount_pps_status_count_xor<12>  (
    .CI(Mcount_pps_status_count_cy[11]),
    .LI(\Mcount_pps_status_count_cy<12>_rt_1181 ),
    .O(Result[12])
  );
  MUXCY   \Mcount_pps_status_count_cy<13>  (
    .CI(Mcount_pps_status_count_cy[12]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<13>_rt_1183 ),
    .O(Mcount_pps_status_count_cy[13])
  );
  XORCY   \Mcount_pps_status_count_xor<13>  (
    .CI(Mcount_pps_status_count_cy[12]),
    .LI(\Mcount_pps_status_count_cy<13>_rt_1183 ),
    .O(Result[13])
  );
  MUXCY   \Mcount_pps_status_count_cy<14>  (
    .CI(Mcount_pps_status_count_cy[13]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<14>_rt_1185 ),
    .O(Mcount_pps_status_count_cy[14])
  );
  XORCY   \Mcount_pps_status_count_xor<14>  (
    .CI(Mcount_pps_status_count_cy[13]),
    .LI(\Mcount_pps_status_count_cy<14>_rt_1185 ),
    .O(Result[14])
  );
  MUXCY   \Mcount_pps_status_count_cy<15>  (
    .CI(Mcount_pps_status_count_cy[14]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<15>_rt_1187 ),
    .O(Mcount_pps_status_count_cy[15])
  );
  XORCY   \Mcount_pps_status_count_xor<15>  (
    .CI(Mcount_pps_status_count_cy[14]),
    .LI(\Mcount_pps_status_count_cy<15>_rt_1187 ),
    .O(Result[15])
  );
  MUXCY   \Mcount_pps_status_count_cy<16>  (
    .CI(Mcount_pps_status_count_cy[15]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<16>_rt_1189 ),
    .O(Mcount_pps_status_count_cy[16])
  );
  XORCY   \Mcount_pps_status_count_xor<16>  (
    .CI(Mcount_pps_status_count_cy[15]),
    .LI(\Mcount_pps_status_count_cy<16>_rt_1189 ),
    .O(Result[16])
  );
  MUXCY   \Mcount_pps_status_count_cy<17>  (
    .CI(Mcount_pps_status_count_cy[16]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<17>_rt_1191 ),
    .O(Mcount_pps_status_count_cy[17])
  );
  XORCY   \Mcount_pps_status_count_xor<17>  (
    .CI(Mcount_pps_status_count_cy[16]),
    .LI(\Mcount_pps_status_count_cy<17>_rt_1191 ),
    .O(Result[17])
  );
  MUXCY   \Mcount_pps_status_count_cy<18>  (
    .CI(Mcount_pps_status_count_cy[17]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<18>_rt_1193 ),
    .O(Mcount_pps_status_count_cy[18])
  );
  XORCY   \Mcount_pps_status_count_xor<18>  (
    .CI(Mcount_pps_status_count_cy[17]),
    .LI(\Mcount_pps_status_count_cy<18>_rt_1193 ),
    .O(Result[18])
  );
  MUXCY   \Mcount_pps_status_count_cy<19>  (
    .CI(Mcount_pps_status_count_cy[18]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<19>_rt_1195 ),
    .O(Mcount_pps_status_count_cy[19])
  );
  XORCY   \Mcount_pps_status_count_xor<19>  (
    .CI(Mcount_pps_status_count_cy[18]),
    .LI(\Mcount_pps_status_count_cy<19>_rt_1195 ),
    .O(Result[19])
  );
  MUXCY   \Mcount_pps_status_count_cy<20>  (
    .CI(Mcount_pps_status_count_cy[19]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<20>_rt_1199 ),
    .O(Mcount_pps_status_count_cy[20])
  );
  XORCY   \Mcount_pps_status_count_xor<20>  (
    .CI(Mcount_pps_status_count_cy[19]),
    .LI(\Mcount_pps_status_count_cy<20>_rt_1199 ),
    .O(Result[20])
  );
  MUXCY   \Mcount_pps_status_count_cy<21>  (
    .CI(Mcount_pps_status_count_cy[20]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<21>_rt_1201 ),
    .O(Mcount_pps_status_count_cy[21])
  );
  XORCY   \Mcount_pps_status_count_xor<21>  (
    .CI(Mcount_pps_status_count_cy[20]),
    .LI(\Mcount_pps_status_count_cy<21>_rt_1201 ),
    .O(Result[21])
  );
  MUXCY   \Mcount_pps_status_count_cy<22>  (
    .CI(Mcount_pps_status_count_cy[21]),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_pps_status_count_cy<22>_rt_1203 ),
    .O(Mcount_pps_status_count_cy[22])
  );
  XORCY   \Mcount_pps_status_count_xor<22>  (
    .CI(Mcount_pps_status_count_cy[21]),
    .LI(\Mcount_pps_status_count_cy<22>_rt_1203 ),
    .O(Result[22])
  );
  XORCY   \Mcount_pps_status_count_xor<23>  (
    .CI(Mcount_pps_status_count_cy[22]),
    .LI(\Mcount_pps_status_count_xor<23>_rt_1221 ),
    .O(Result[23])
  );
  MUXCY   \Mcount_count_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\Mcount_count_cy<0>_rt_1135 ),
    .O(Mcount_count_cy[0])
  );
  XORCY   \Mcount_count_xor<0>  (
    .CI(N1),
    .LI(\Mcount_count_cy<0>_rt_1135 ),
    .O(\Result<0>1 )
  );
  MUXCY   \Mcount_count_cy<1>  (
    .CI(Mcount_count_cy[0]),
    .DI(N1),
    .S(Mcount_count_lut[1]),
    .O(Mcount_count_cy[1])
  );
  XORCY   \Mcount_count_xor<1>  (
    .CI(Mcount_count_cy[0]),
    .LI(Mcount_count_lut[1]),
    .O(\Result<1>1 )
  );
  MUXCY   \Mcount_count_cy<2>  (
    .CI(Mcount_count_cy[1]),
    .DI(N1),
    .S(Mcount_count_lut[2]),
    .O(Mcount_count_cy[2])
  );
  XORCY   \Mcount_count_xor<2>  (
    .CI(Mcount_count_cy[1]),
    .LI(Mcount_count_lut[2]),
    .O(\Result<2>1 )
  );
  MUXCY   \Mcount_count_cy<3>  (
    .CI(Mcount_count_cy[2]),
    .DI(N1),
    .S(Mcount_count_lut[3]),
    .O(Mcount_count_cy[3])
  );
  XORCY   \Mcount_count_xor<3>  (
    .CI(Mcount_count_cy[2]),
    .LI(Mcount_count_lut[3]),
    .O(\Result<3>1 )
  );
  MUXCY   \Mcount_count_cy<4>  (
    .CI(Mcount_count_cy[3]),
    .DI(N1),
    .S(Mcount_count_lut[4]),
    .O(Mcount_count_cy[4])
  );
  XORCY   \Mcount_count_xor<4>  (
    .CI(Mcount_count_cy[3]),
    .LI(Mcount_count_lut[4]),
    .O(\Result<4>1 )
  );
  MUXCY   \Mcount_count_cy<5>  (
    .CI(Mcount_count_cy[4]),
    .DI(N1),
    .S(Mcount_count_lut[5]),
    .O(Mcount_count_cy[5])
  );
  XORCY   \Mcount_count_xor<5>  (
    .CI(Mcount_count_cy[4]),
    .LI(Mcount_count_lut[5]),
    .O(\Result<5>1 )
  );
  MUXCY   \Mcount_count_cy<6>  (
    .CI(Mcount_count_cy[5]),
    .DI(N1),
    .S(Mcount_count_lut[6]),
    .O(Mcount_count_cy[6])
  );
  XORCY   \Mcount_count_xor<6>  (
    .CI(Mcount_count_cy[5]),
    .LI(Mcount_count_lut[6]),
    .O(\Result<6>1 )
  );
  MUXCY   \Mcount_count_cy<7>  (
    .CI(Mcount_count_cy[6]),
    .DI(N1),
    .S(Mcount_count_lut[7]),
    .O(Mcount_count_cy[7])
  );
  XORCY   \Mcount_count_xor<7>  (
    .CI(Mcount_count_cy[6]),
    .LI(Mcount_count_lut[7]),
    .O(\Result<7>1 )
  );
  MUXCY   \Mcount_count_cy<8>  (
    .CI(Mcount_count_cy[7]),
    .DI(N1),
    .S(Mcount_count_lut[8]),
    .O(Mcount_count_cy[8])
  );
  XORCY   \Mcount_count_xor<8>  (
    .CI(Mcount_count_cy[7]),
    .LI(Mcount_count_lut[8]),
    .O(\Result<8>1 )
  );
  MUXCY   \Mcount_count_cy<9>  (
    .CI(Mcount_count_cy[8]),
    .DI(N1),
    .S(Mcount_count_lut[9]),
    .O(Mcount_count_cy[9])
  );
  XORCY   \Mcount_count_xor<9>  (
    .CI(Mcount_count_cy[8]),
    .LI(Mcount_count_lut[9]),
    .O(\Result<9>1 )
  );
  MUXCY   \Mcount_count_cy<10>  (
    .CI(Mcount_count_cy[9]),
    .DI(N1),
    .S(Mcount_count_lut[10]),
    .O(Mcount_count_cy[10])
  );
  XORCY   \Mcount_count_xor<10>  (
    .CI(Mcount_count_cy[9]),
    .LI(Mcount_count_lut[10]),
    .O(\Result<10>1 )
  );
  MUXCY   \Mcount_count_cy<11>  (
    .CI(Mcount_count_cy[10]),
    .DI(N1),
    .S(Mcount_count_lut[11]),
    .O(Mcount_count_cy[11])
  );
  XORCY   \Mcount_count_xor<11>  (
    .CI(Mcount_count_cy[10]),
    .LI(Mcount_count_lut[11]),
    .O(\Result<11>1 )
  );
  MUXCY   \Mcount_count_cy<12>  (
    .CI(Mcount_count_cy[11]),
    .DI(N1),
    .S(Mcount_count_lut[12]),
    .O(Mcount_count_cy[12])
  );
  XORCY   \Mcount_count_xor<12>  (
    .CI(Mcount_count_cy[11]),
    .LI(Mcount_count_lut[12]),
    .O(\Result<12>1 )
  );
  MUXCY   \Mcount_count_cy<13>  (
    .CI(Mcount_count_cy[12]),
    .DI(N1),
    .S(Mcount_count_lut[13]),
    .O(Mcount_count_cy[13])
  );
  XORCY   \Mcount_count_xor<13>  (
    .CI(Mcount_count_cy[12]),
    .LI(Mcount_count_lut[13]),
    .O(\Result<13>1 )
  );
  MUXCY   \Mcount_count_cy<14>  (
    .CI(Mcount_count_cy[13]),
    .DI(N1),
    .S(Mcount_count_lut[14]),
    .O(Mcount_count_cy[14])
  );
  XORCY   \Mcount_count_xor<14>  (
    .CI(Mcount_count_cy[13]),
    .LI(Mcount_count_lut[14]),
    .O(\Result<14>1 )
  );
  MUXCY   \Mcount_count_cy<15>  (
    .CI(Mcount_count_cy[14]),
    .DI(N1),
    .S(Mcount_count_lut[15]),
    .O(Mcount_count_cy[15])
  );
  XORCY   \Mcount_count_xor<15>  (
    .CI(Mcount_count_cy[14]),
    .LI(Mcount_count_lut[15]),
    .O(\Result<15>1 )
  );
  MUXCY   \Mcount_count_cy<16>  (
    .CI(Mcount_count_cy[15]),
    .DI(N1),
    .S(Mcount_count_lut[16]),
    .O(Mcount_count_cy[16])
  );
  XORCY   \Mcount_count_xor<16>  (
    .CI(Mcount_count_cy[15]),
    .LI(Mcount_count_lut[16]),
    .O(\Result<16>1 )
  );
  MUXCY   \Mcount_count_cy<17>  (
    .CI(Mcount_count_cy[16]),
    .DI(N1),
    .S(Mcount_count_lut[17]),
    .O(Mcount_count_cy[17])
  );
  XORCY   \Mcount_count_xor<17>  (
    .CI(Mcount_count_cy[16]),
    .LI(Mcount_count_lut[17]),
    .O(\Result<17>1 )
  );
  MUXCY   \Mcount_count_cy<18>  (
    .CI(Mcount_count_cy[17]),
    .DI(N1),
    .S(Mcount_count_lut[18]),
    .O(Mcount_count_cy[18])
  );
  XORCY   \Mcount_count_xor<18>  (
    .CI(Mcount_count_cy[17]),
    .LI(Mcount_count_lut[18]),
    .O(\Result<18>1 )
  );
  MUXCY   \Mcount_count_cy<19>  (
    .CI(Mcount_count_cy[18]),
    .DI(N1),
    .S(Mcount_count_lut[19]),
    .O(Mcount_count_cy[19])
  );
  XORCY   \Mcount_count_xor<19>  (
    .CI(Mcount_count_cy[18]),
    .LI(Mcount_count_lut[19]),
    .O(\Result<19>1 )
  );
  XORCY   \Mcount_count_xor<20>  (
    .CI(Mcount_count_cy[19]),
    .LI(Mcount_count_lut[20]),
    .O(\Result<20>1 )
  );
  IBUFGDS #(
    .CAPACITANCE ( "DONT_CARE" ),
    .DIFF_TERM ( "FALSE" ),
    .IBUF_DELAY_VALUE ( "0" ),
    .IBUF_LOW_PWR ( "TRUE" ),
    .IOSTANDARD ( "LVDS_25" ))
  diff_buf_clk (
    .I(clk_148_p),
    .IB(clk_148_n),
    .O(clk_1481)
  );
  DCM_SP #(
    .CLKDV_DIVIDE ( 2.000000 ),
    .CLKFX_DIVIDE ( 1 ),
    .CLKFX_MULTIPLY ( 10 ),
    .CLKIN_DIVIDE_BY_2 ( "FALSE" ),
    .CLKIN_PERIOD ( 100.0000000000000000 ),
    .CLKOUT_PHASE_SHIFT ( "NONE" ),
    .CLK_FEEDBACK ( "NONE" ),
    .DESKEW_ADJUST ( "SYSTEM_SYNCHRONOUS" ),
    .DFS_FREQUENCY_MODE ( "LOW" ),
    .DLL_FREQUENCY_MODE ( "LOW" ),
    .DSS_MODE ( "NONE" ),
    .DUTY_CYCLE_CORRECTION ( "TRUE" ),
    .PHASE_SHIFT ( 0 ),
    .STARTUP_WAIT ( "FALSE" ),
    .FACTORY_JF ( 16'hC080 ))
  \PLL_clock_gen/DCM_SP_INST  (
    .CLKIN(clk_10_i_IBUFG1),
    .CLKFB(tp2_o_OBUF_3479),
    .RST(tp2_o_OBUF_3479),
    .DSSEN(tp2_o_OBUF_3479),
    .PSINCDEC(tp2_o_OBUF_3479),
    .PSEN(tp2_o_OBUF_3479),
    .PSCLK(tp2_o_OBUF_3479),
    .CLK0(\NLW_PLL_clock_gen/DCM_SP_INST_CLK0_UNCONNECTED ),
    .CLK90(\NLW_PLL_clock_gen/DCM_SP_INST_CLK90_UNCONNECTED ),
    .CLK180(\NLW_PLL_clock_gen/DCM_SP_INST_CLK180_UNCONNECTED ),
    .CLK270(\NLW_PLL_clock_gen/DCM_SP_INST_CLK270_UNCONNECTED ),
    .CLK2X(\NLW_PLL_clock_gen/DCM_SP_INST_CLK2X_UNCONNECTED ),
    .CLK2X180(\NLW_PLL_clock_gen/DCM_SP_INST_CLK2X180_UNCONNECTED ),
    .CLKDV(\NLW_PLL_clock_gen/DCM_SP_INST_CLKDV_UNCONNECTED ),
    .CLKFX(\PLL_clock_gen/CLKFX_BUF ),
    .CLKFX180(\NLW_PLL_clock_gen/DCM_SP_INST_CLKFX180_UNCONNECTED ),
    .LOCKED(\NLW_PLL_clock_gen/DCM_SP_INST_LOCKED_UNCONNECTED ),
    .PSDONE(\NLW_PLL_clock_gen/DCM_SP_INST_PSDONE_UNCONNECTED ),
    .STATUS({\NLW_PLL_clock_gen/DCM_SP_INST_STATUS<7>_UNCONNECTED , \NLW_PLL_clock_gen/DCM_SP_INST_STATUS<6>_UNCONNECTED , 
\NLW_PLL_clock_gen/DCM_SP_INST_STATUS<5>_UNCONNECTED , \NLW_PLL_clock_gen/DCM_SP_INST_STATUS<4>_UNCONNECTED , 
\NLW_PLL_clock_gen/DCM_SP_INST_STATUS<3>_UNCONNECTED , \NLW_PLL_clock_gen/DCM_SP_INST_STATUS<2>_UNCONNECTED , 
\NLW_PLL_clock_gen/DCM_SP_INST_STATUS<1>_UNCONNECTED , \NLW_PLL_clock_gen/DCM_SP_INST_STATUS<0>_UNCONNECTED })
  );
  BUFG   \PLL_clock_gen/CLKFX_BUFG_INST  (
    .I(\PLL_clock_gen/CLKFX_BUF ),
    .O(clk_100)
  );
  FD #(
    .INIT ( 1'b0 ))
  \serial_interface/state_FSM_FFd3  (
    .C(clk_148),
    .D(\serial_interface/state_FSM_FFd3-In ),
    .Q(\serial_interface/state_FSM_FFd3_3369 )
  );
  FD #(
    .INIT ( 1'b0 ))
  \serial_interface/state_FSM_FFd2  (
    .C(clk_148),
    .D(\serial_interface/state_FSM_FFd2-In ),
    .Q(\serial_interface/state_FSM_FFd2_3359 )
  );
  FD #(
    .INIT ( 1'b0 ))
  \serial_interface/state_FSM_FFd1  (
    .C(clk_148),
    .D(\serial_interface/state_FSM_FFd1-In ),
    .Q(\serial_interface/state_FSM_FFd1_3351 )
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<21>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [20]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [21]),
    .O(\serial_interface/time_out_addsub0000 [21])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<20>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [19]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [20]),
    .O(\serial_interface/time_out_addsub0000 [20])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<20>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [19]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [20]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [20])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<19>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [18]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [19]),
    .O(\serial_interface/time_out_addsub0000 [19])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<19>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [18]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [19]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [19])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<18>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [17]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [18]),
    .O(\serial_interface/time_out_addsub0000 [18])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<18>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [17]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [18]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [18])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<17>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [16]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [17]),
    .O(\serial_interface/time_out_addsub0000 [17])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<17>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [16]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [17]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [17])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<16>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [15]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [16]),
    .O(\serial_interface/time_out_addsub0000 [16])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<16>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [15]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [16]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [16])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<15>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [14]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [15]),
    .O(\serial_interface/time_out_addsub0000 [15])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<15>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [14]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [15]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [15])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<14>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [13]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [14]),
    .O(\serial_interface/time_out_addsub0000 [14])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<14>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [13]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [14]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [14])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<13>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [12]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [13]),
    .O(\serial_interface/time_out_addsub0000 [13])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<13>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [12]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [13]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [13])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<12>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [11]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [12]),
    .O(\serial_interface/time_out_addsub0000 [12])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<12>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [11]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [12]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [12])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<11>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [10]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [11]),
    .O(\serial_interface/time_out_addsub0000 [11])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<11>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [10]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [11]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [11])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<10>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [9]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [10]),
    .O(\serial_interface/time_out_addsub0000 [10])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<10>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [9]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [10]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [10])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<9>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [8]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [9]),
    .O(\serial_interface/time_out_addsub0000 [9])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<9>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [8]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [9]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [9])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<8>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [7]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [8]),
    .O(\serial_interface/time_out_addsub0000 [8])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<8>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [7]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [8]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [8])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<7>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [6]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [7]),
    .O(\serial_interface/time_out_addsub0000 [7])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<7>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [6]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [7]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [7])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<6>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [5]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [6]),
    .O(\serial_interface/time_out_addsub0000 [6])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<6>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [5]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [6]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [6])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<5>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [4]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [5]),
    .O(\serial_interface/time_out_addsub0000 [5])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<5>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [4]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [5]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [5])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<4>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [3]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [4]),
    .O(\serial_interface/time_out_addsub0000 [4])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<4>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [3]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [4]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [4])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<3>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [2]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [3]),
    .O(\serial_interface/time_out_addsub0000 [3])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<3>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [2]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [3]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [3])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<2>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [1]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [2]),
    .O(\serial_interface/time_out_addsub0000 [2])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<2>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [1]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [2]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [2])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<1>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [0]),
    .LI(\serial_interface/Msub_time_out_addsub0000_lut [1]),
    .O(\serial_interface/time_out_addsub0000 [1])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<1>  (
    .CI(\serial_interface/Msub_time_out_addsub0000_cy [0]),
    .DI(N1),
    .S(\serial_interface/Msub_time_out_addsub0000_lut [1]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [1])
  );
  XORCY   \serial_interface/Msub_time_out_addsub0000_xor<0>  (
    .CI(N1),
    .LI(\serial_interface/Msub_time_out_addsub0000_cy<0>_rt_3071 ),
    .O(\serial_interface/time_out_addsub0000 [0])
  );
  MUXCY   \serial_interface/Msub_time_out_addsub0000_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\serial_interface/Msub_time_out_addsub0000_cy<0>_rt_3071 ),
    .O(\serial_interface/Msub_time_out_addsub0000_cy [0])
  );
  FD   \serial_interface/SDA_delay_1  (
    .C(clk_148),
    .D(\serial_interface/SDA_delay [0]),
    .Q(\serial_interface/SDA_delay [1])
  );
  FD   \serial_interface/SDA_delay_0  (
    .C(clk_148),
    .D(N156),
    .Q(\serial_interface/SDA_delay [0])
  );
  FDE   \serial_interface/recieved_byte_o_7  (
    .C(clk_148),
    .CE(\serial_interface/state_cmp_eq0021 ),
    .D(\serial_interface/recieve_byte [7]),
    .Q(\serial_interface/recieved_byte_o [7])
  );
  FDE   \serial_interface/recieved_byte_o_6  (
    .C(clk_148),
    .CE(\serial_interface/state_cmp_eq0021 ),
    .D(\serial_interface/recieve_byte [6]),
    .Q(\serial_interface/recieved_byte_o [6])
  );
  FDE   \serial_interface/recieved_byte_o_5  (
    .C(clk_148),
    .CE(\serial_interface/state_cmp_eq0021 ),
    .D(\serial_interface/recieve_byte [5]),
    .Q(\serial_interface/recieved_byte_o [5])
  );
  FDE   \serial_interface/recieved_byte_o_4  (
    .C(clk_148),
    .CE(\serial_interface/state_cmp_eq0021 ),
    .D(\serial_interface/recieve_byte [4]),
    .Q(\serial_interface/recieved_byte_o [4])
  );
  FDE   \serial_interface/recieved_byte_o_3  (
    .C(clk_148),
    .CE(\serial_interface/state_cmp_eq0021 ),
    .D(\serial_interface/recieve_byte [3]),
    .Q(\serial_interface/recieved_byte_o [3])
  );
  FDE   \serial_interface/recieved_byte_o_2  (
    .C(clk_148),
    .CE(\serial_interface/state_cmp_eq0021 ),
    .D(\serial_interface/recieve_byte [2]),
    .Q(\serial_interface/recieved_byte_o [2])
  );
  FDE   \serial_interface/recieved_byte_o_1  (
    .C(clk_148),
    .CE(\serial_interface/state_cmp_eq0021 ),
    .D(\serial_interface/recieve_byte [1]),
    .Q(\serial_interface/recieved_byte_o [1])
  );
  FDE   \serial_interface/recieved_byte_o_0  (
    .C(clk_148),
    .CE(\serial_interface/state_cmp_eq0021 ),
    .D(\serial_interface/recieve_byte [0]),
    .Q(\serial_interface/recieved_byte_o [0])
  );
  FD   \serial_interface/RW_dir  (
    .C(clk_148),
    .D(\serial_interface/_mux0000 ),
    .Q(\serial_interface/RW_dir_3113 )
  );
  FD #(
    .INIT ( 1'b0 ))
  \serial_interface/command_2  (
    .C(clk_148),
    .D(\serial_interface/command_mux0000 [0]),
    .Q(\serial_interface/command [2])
  );
  FD #(
    .INIT ( 1'b0 ))
  \serial_interface/command_1  (
    .C(clk_148),
    .D(\serial_interface/command_mux0000 [1]),
    .Q(\serial_interface/command [1])
  );
  FD #(
    .INIT ( 1'b1 ))
  \serial_interface/command_0  (
    .C(clk_148),
    .D(\serial_interface/command_mux0000 [2]),
    .Q(\serial_interface/command [0])
  );
  FD   \serial_interface/recieve_byte_7  (
    .C(clk_148),
    .D(\serial_interface/recieve_byte_7_mux0001_3164 ),
    .Q(\serial_interface/recieve_byte [7])
  );
  FD   \serial_interface/SCL_delay_1  (
    .C(clk_148),
    .D(\serial_interface/SCL_delay [0]),
    .Q(\serial_interface/SCL_delay [1])
  );
  FD   \serial_interface/SCL_delay_0  (
    .C(clk_148),
    .D(SCL_i_IBUF_2049),
    .Q(\serial_interface/SCL_delay [0])
  );
  FD   \serial_interface/recieve_byte_5  (
    .C(clk_148),
    .D(\serial_interface/recieve_byte_5_mux0001_3159 ),
    .Q(\serial_interface/recieve_byte [5])
  );
  FD   \serial_interface/recieve_byte_4  (
    .C(clk_148),
    .D(\serial_interface/recieve_byte_4_mux0001_3157 ),
    .Q(\serial_interface/recieve_byte [4])
  );
  FD   \serial_interface/recieve_byte_6  (
    .C(clk_148),
    .D(\serial_interface/recieve_byte_6_mux0001_3162 ),
    .Q(\serial_interface/recieve_byte [6])
  );
  FD   \serial_interface/recieve_byte_2  (
    .C(clk_148),
    .D(\serial_interface/recieve_byte_2_mux0001_3153 ),
    .Q(\serial_interface/recieve_byte [2])
  );
  FD   \serial_interface/recieve_byte_1  (
    .C(clk_148),
    .D(\serial_interface/recieve_byte_1_mux0001_3151 ),
    .Q(\serial_interface/recieve_byte [1])
  );
  FD   \serial_interface/recieve_byte_3  (
    .C(clk_148),
    .D(\serial_interface/recieve_byte_3_mux0001_3155 ),
    .Q(\serial_interface/recieve_byte [3])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \serial_interface/bit_count_3  (
    .C(clk_148),
    .CE(\serial_interface/bit_count_not0001 ),
    .D(\serial_interface/bit_count_mux0002 [3]),
    .R(\serial_interface/bit_count_or0000 ),
    .Q(\serial_interface/bit_count [3])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \serial_interface/bit_count_2  (
    .C(clk_148),
    .CE(\serial_interface/bit_count_not0001 ),
    .D(\serial_interface/bit_count_mux0002 [2]),
    .R(\serial_interface/bit_count_or0000 ),
    .Q(\serial_interface/bit_count [2])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \serial_interface/bit_count_1  (
    .C(clk_148),
    .CE(\serial_interface/bit_count_not0001 ),
    .D(\serial_interface/bit_count_mux0002 [1]),
    .R(\serial_interface/bit_count_or0000 ),
    .Q(\serial_interface/bit_count [1])
  );
  FDRE #(
    .INIT ( 1'b0 ))
  \serial_interface/bit_count_0  (
    .C(clk_148),
    .CE(\serial_interface/bit_count_not0001 ),
    .D(\serial_interface/bit_count_mux0002 [0]),
    .R(\serial_interface/bit_count_or0000 ),
    .Q(\serial_interface/bit_count [0])
  );
  FD   \serial_interface/recieve_byte_0  (
    .C(clk_148),
    .D(\serial_interface/recieve_byte_0_mux0001_3149 ),
    .Q(\serial_interface/recieve_byte [0])
  );
  FD   \serial_interface/time_out_18  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009[3] ),
    .Q(\serial_interface/time_out [18])
  );
  FD   \serial_interface/time_out_17  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009[4] ),
    .Q(\serial_interface/time_out [17])
  );
  FD   \serial_interface/time_out_16  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009[5] ),
    .Q(\serial_interface/time_out [16])
  );
  FD   \serial_interface/time_out_14  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009[7] ),
    .Q(\serial_interface/time_out [14])
  );
  FD   \serial_interface/time_out_12  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009[9] ),
    .Q(\serial_interface/time_out [12])
  );
  FD   \serial_interface/time_out_11  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009[10] ),
    .Q(\serial_interface/time_out [11])
  );
  FD   \serial_interface/time_out_9  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009[12] ),
    .Q(\serial_interface/time_out [9])
  );
  FD   \serial_interface/time_out_3  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009[18] ),
    .Q(\serial_interface/time_out [3])
  );
  FD   \serial_interface/time_out_1  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009[20] ),
    .Q(\serial_interface/time_out [1])
  );
  FD   \serial_interface/time_out_0  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009[21] ),
    .Q(\serial_interface/time_out [0])
  );
  FDR   \serial_interface/stop_o  (
    .C(clk_148),
    .D(N1),
    .R(\serial_interface/stop_o_not0001 ),
    .Q(\serial_interface/stop_o_3413 )
  );
  XORCY   \dac_OCXO/Mcount_bit_count_xor<7>  (
    .CI(\dac_OCXO/Mcount_bit_count_cy [6]),
    .LI(\dac_OCXO/Mcount_bit_count_lut [7]),
    .O(\dac_OCXO/Result [7])
  );
  XORCY   \dac_OCXO/Mcount_bit_count_xor<6>  (
    .CI(\dac_OCXO/Mcount_bit_count_cy [5]),
    .LI(\dac_OCXO/Mcount_bit_count_lut [6]),
    .O(\dac_OCXO/Result [6])
  );
  MUXCY   \dac_OCXO/Mcount_bit_count_cy<6>  (
    .CI(\dac_OCXO/Mcount_bit_count_cy [5]),
    .DI(N1),
    .S(\dac_OCXO/Mcount_bit_count_lut [6]),
    .O(\dac_OCXO/Mcount_bit_count_cy [6])
  );
  XORCY   \dac_OCXO/Mcount_bit_count_xor<5>  (
    .CI(\dac_OCXO/Mcount_bit_count_cy [4]),
    .LI(\dac_OCXO/Mcount_bit_count_lut [5]),
    .O(\dac_OCXO/Result [5])
  );
  MUXCY   \dac_OCXO/Mcount_bit_count_cy<5>  (
    .CI(\dac_OCXO/Mcount_bit_count_cy [4]),
    .DI(N1),
    .S(\dac_OCXO/Mcount_bit_count_lut [5]),
    .O(\dac_OCXO/Mcount_bit_count_cy [5])
  );
  XORCY   \dac_OCXO/Mcount_bit_count_xor<4>  (
    .CI(\dac_OCXO/Mcount_bit_count_cy [3]),
    .LI(\dac_OCXO/Mcount_bit_count_lut [4]),
    .O(\dac_OCXO/Result [4])
  );
  MUXCY   \dac_OCXO/Mcount_bit_count_cy<4>  (
    .CI(\dac_OCXO/Mcount_bit_count_cy [3]),
    .DI(N1),
    .S(\dac_OCXO/Mcount_bit_count_lut [4]),
    .O(\dac_OCXO/Mcount_bit_count_cy [4])
  );
  XORCY   \dac_OCXO/Mcount_bit_count_xor<3>  (
    .CI(\dac_OCXO/Mcount_bit_count_cy [2]),
    .LI(\dac_OCXO/Mcount_bit_count_lut [3]),
    .O(\dac_OCXO/Result [3])
  );
  MUXCY   \dac_OCXO/Mcount_bit_count_cy<3>  (
    .CI(\dac_OCXO/Mcount_bit_count_cy [2]),
    .DI(N1),
    .S(\dac_OCXO/Mcount_bit_count_lut [3]),
    .O(\dac_OCXO/Mcount_bit_count_cy [3])
  );
  XORCY   \dac_OCXO/Mcount_bit_count_xor<2>  (
    .CI(\dac_OCXO/Mcount_bit_count_cy [1]),
    .LI(\dac_OCXO/Mcount_bit_count_lut [2]),
    .O(\dac_OCXO/Result [2])
  );
  MUXCY   \dac_OCXO/Mcount_bit_count_cy<2>  (
    .CI(\dac_OCXO/Mcount_bit_count_cy [1]),
    .DI(N1),
    .S(\dac_OCXO/Mcount_bit_count_lut [2]),
    .O(\dac_OCXO/Mcount_bit_count_cy [2])
  );
  XORCY   \dac_OCXO/Mcount_bit_count_xor<1>  (
    .CI(\dac_OCXO/Mcount_bit_count_cy [0]),
    .LI(\dac_OCXO/Mcount_bit_count_lut [1]),
    .O(\dac_OCXO/Result [1])
  );
  MUXCY   \dac_OCXO/Mcount_bit_count_cy<1>  (
    .CI(\dac_OCXO/Mcount_bit_count_cy [0]),
    .DI(N1),
    .S(\dac_OCXO/Mcount_bit_count_lut [1]),
    .O(\dac_OCXO/Mcount_bit_count_cy [1])
  );
  XORCY   \dac_OCXO/Mcount_bit_count_xor<0>  (
    .CI(N1),
    .LI(\dac_OCXO/Mcount_bit_count_cy<0>_rt_2402 ),
    .O(\dac_OCXO/Result [0])
  );
  MUXCY   \dac_OCXO/Mcount_bit_count_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\dac_OCXO/Mcount_bit_count_cy<0>_rt_2402 ),
    .O(\dac_OCXO/Mcount_bit_count_cy [0])
  );
  FDRS   \dac_OCXO/bit_count_7  (
    .C(clk_100),
    .D(\dac_OCXO/Result [7]),
    .R(\dac_OCXO/bit_count_not0001_inv ),
    .S(\OCXO_pll/phase_detect/update_tick_1570 ),
    .Q(\dac_OCXO/bit_count [7])
  );
  FDRS   \dac_OCXO/bit_count_6  (
    .C(clk_100),
    .D(\dac_OCXO/Result [6]),
    .R(\dac_OCXO/bit_count_not0001_inv ),
    .S(\OCXO_pll/phase_detect/update_tick_1570 ),
    .Q(\dac_OCXO/bit_count [6])
  );
  FDR   \dac_OCXO/bit_count_5  (
    .C(clk_100),
    .D(\dac_OCXO/Result [5]),
    .R(\dac_OCXO/Mcount_bit_count_val ),
    .Q(\dac_OCXO/bit_count [5])
  );
  FDR   \dac_OCXO/bit_count_4  (
    .C(clk_100),
    .D(\dac_OCXO/Result [4]),
    .R(\dac_OCXO/Mcount_bit_count_val ),
    .Q(\dac_OCXO/bit_count [4])
  );
  FDR   \dac_OCXO/bit_count_3  (
    .C(clk_100),
    .D(\dac_OCXO/Result [3]),
    .R(\dac_OCXO/Mcount_bit_count_val ),
    .Q(\dac_OCXO/bit_count [3])
  );
  FDRS   \dac_OCXO/bit_count_2  (
    .C(clk_100),
    .D(\dac_OCXO/Result [2]),
    .R(\dac_OCXO/bit_count_not0001_inv ),
    .S(\OCXO_pll/phase_detect/update_tick_1570 ),
    .Q(\dac_OCXO/bit_count [2])
  );
  FDRS   \dac_OCXO/bit_count_0  (
    .C(clk_100),
    .D(\dac_OCXO/Result [0]),
    .R(\dac_OCXO/bit_count_not0001_inv ),
    .S(\OCXO_pll/phase_detect/update_tick_1570 ),
    .Q(\dac_OCXO/bit_count [0])
  );
  FDRS   \dac_OCXO/bit_count_1  (
    .C(clk_100),
    .D(\dac_OCXO/Result [1]),
    .R(\dac_OCXO/bit_count_not0001_inv ),
    .S(\OCXO_pll/phase_detect/update_tick_1570 ),
    .Q(\dac_OCXO/bit_count [1])
  );
  FDSE   \dac_OCXO/zsync  (
    .C(clk_100),
    .CE(\dac_OCXO/bit_tick_2437 ),
    .D(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_detect/update_tick_1570 ),
    .Q(\dac_OCXO/zsync_2488 )
  );
  FDE   \dac_OCXO/shift_reg_23  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [23]),
    .Q(\dac_OCXO/shift_reg [23])
  );
  FDE   \dac_OCXO/shift_reg_22  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [22]),
    .Q(\dac_OCXO/shift_reg [22])
  );
  FDE   \dac_OCXO/shift_reg_21  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [21]),
    .Q(\dac_OCXO/shift_reg [21])
  );
  FDE   \dac_OCXO/shift_reg_20  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [20]),
    .Q(\dac_OCXO/shift_reg [20])
  );
  FDE   \dac_OCXO/shift_reg_19  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [19]),
    .Q(\dac_OCXO/shift_reg [19])
  );
  FDE   \dac_OCXO/shift_reg_18  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [18]),
    .Q(\dac_OCXO/shift_reg [18])
  );
  FDE   \dac_OCXO/shift_reg_17  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [17]),
    .Q(\dac_OCXO/shift_reg [17])
  );
  FDE   \dac_OCXO/shift_reg_16  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [16]),
    .Q(\dac_OCXO/shift_reg [16])
  );
  FDE   \dac_OCXO/shift_reg_15  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [15]),
    .Q(\dac_OCXO/shift_reg [15])
  );
  FDE   \dac_OCXO/shift_reg_14  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [14]),
    .Q(\dac_OCXO/shift_reg [14])
  );
  FDE   \dac_OCXO/shift_reg_13  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [13]),
    .Q(\dac_OCXO/shift_reg [13])
  );
  FDE   \dac_OCXO/shift_reg_12  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [12]),
    .Q(\dac_OCXO/shift_reg [12])
  );
  FDE   \dac_OCXO/shift_reg_11  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [11]),
    .Q(\dac_OCXO/shift_reg [11])
  );
  FDE   \dac_OCXO/shift_reg_10  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [10]),
    .Q(\dac_OCXO/shift_reg [10])
  );
  FDE   \dac_OCXO/shift_reg_9  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [9]),
    .Q(\dac_OCXO/shift_reg [9])
  );
  FDE   \dac_OCXO/shift_reg_8  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [8]),
    .Q(\dac_OCXO/shift_reg [8])
  );
  FDE   \dac_OCXO/shift_reg_7  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [7]),
    .Q(\dac_OCXO/shift_reg [7])
  );
  FDE   \dac_OCXO/shift_reg_6  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [6]),
    .Q(\dac_OCXO/shift_reg [6])
  );
  FDE   \dac_OCXO/shift_reg_5  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [5]),
    .Q(\dac_OCXO/shift_reg [5])
  );
  FDE   \dac_OCXO/shift_reg_4  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [4]),
    .Q(\dac_OCXO/shift_reg [4])
  );
  FDE   \dac_OCXO/shift_reg_3  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [3]),
    .Q(\dac_OCXO/shift_reg [3])
  );
  FDE   \dac_OCXO/shift_reg_2  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [2]),
    .Q(\dac_OCXO/shift_reg [2])
  );
  FDE   \dac_OCXO/shift_reg_1  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [1]),
    .Q(\dac_OCXO/shift_reg [1])
  );
  FDE   \dac_OCXO/shift_reg_0  (
    .C(clk_100),
    .CE(\dac_OCXO/shift_reg_not0001 ),
    .D(\dac_OCXO/shift_reg_mux0000 [0]),
    .Q(\dac_OCXO/shift_reg [0])
  );
  FDR   \dac_OCXO/bit_tick  (
    .C(clk_100),
    .D(N1),
    .R(\dac_OCXO/bit_tick_or0000 ),
    .Q(\dac_OCXO/bit_tick_2437 )
  );
  FD #(
    .INIT ( 1'b1 ))
  \Inst_ad5660_serial_DAC/state_FSM_FFd5  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/state_FSM_FFd5-In_114 ),
    .Q(\Inst_ad5660_serial_DAC/state_FSM_FFd5_113 )
  );
  FD #(
    .INIT ( 1'b0 ))
  \Inst_ad5660_serial_DAC/state_FSM_FFd4  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/state_FSM_FFd4-In ),
    .Q(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 )
  );
  FD #(
    .INIT ( 1'b0 ))
  \Inst_ad5660_serial_DAC/state_FSM_FFd1  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/state_FSM_FFd1-In ),
    .Q(\Inst_ad5660_serial_DAC/state_FSM_FFd1_103 )
  );
  FDR   \Inst_ad5660_serial_DAC/dec_clk_count_3  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/Result [3]),
    .R(\Inst_ad5660_serial_DAC/dec_clk_count_not0001 ),
    .Q(\Inst_ad5660_serial_DAC/dec_clk_count [3])
  );
  FDR   \Inst_ad5660_serial_DAC/dec_clk_count_2  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/Result [2]),
    .R(\Inst_ad5660_serial_DAC/dec_clk_count_not0001 ),
    .Q(\Inst_ad5660_serial_DAC/dec_clk_count [2])
  );
  FDR   \Inst_ad5660_serial_DAC/dec_clk_count_1  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/Result [1]),
    .R(\Inst_ad5660_serial_DAC/dec_clk_count_not0001 ),
    .Q(\Inst_ad5660_serial_DAC/dec_clk_count [1])
  );
  FDR   \Inst_ad5660_serial_DAC/dec_clk_count_0  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/Result [0]),
    .R(\Inst_ad5660_serial_DAC/dec_clk_count_not0001 ),
    .Q(\Inst_ad5660_serial_DAC/dec_clk_count [0])
  );
  FD   \Inst_ad5660_serial_DAC/sync_o  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/state_FSM_FFd5_113 ),
    .Q(\Inst_ad5660_serial_DAC/sync_o_116 )
  );
  FD   \Inst_ad5660_serial_DAC/sclk  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/sclk_mux0000 ),
    .Q(\Inst_ad5660_serial_DAC/sclk_53 )
  );
  FD   \Inst_ad5660_serial_DAC/shift_register_23  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000[23] ),
    .Q(\Inst_ad5660_serial_DAC/shift_register [23])
  );
  FD   \Inst_ad5660_serial_DAC/shift_register_22  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000[22] ),
    .Q(\Inst_ad5660_serial_DAC/shift_register [22])
  );
  FD   \Inst_ad5660_serial_DAC/shift_register_21  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000[21] ),
    .Q(\Inst_ad5660_serial_DAC/shift_register [21])
  );
  FD   \Inst_ad5660_serial_DAC/shift_register_20  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000[20] ),
    .Q(\Inst_ad5660_serial_DAC/shift_register [20])
  );
  FD   \Inst_ad5660_serial_DAC/shift_register_19  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000[19] ),
    .Q(\Inst_ad5660_serial_DAC/shift_register [19])
  );
  FD   \Inst_ad5660_serial_DAC/shift_register_18  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000[18] ),
    .Q(\Inst_ad5660_serial_DAC/shift_register [18])
  );
  FD   \Inst_ad5660_serial_DAC/shift_register_17  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000[17] ),
    .Q(\Inst_ad5660_serial_DAC/shift_register [17])
  );
  FD   \Inst_ad5660_serial_DAC/shift_register_16  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000[16] ),
    .Q(\Inst_ad5660_serial_DAC/shift_register [16])
  );
  FD   \Inst_ad5660_serial_DAC/shift_register_0  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000[0] ),
    .Q(\Inst_ad5660_serial_DAC/shift_register [0])
  );
  FD   \Inst_ad5660_serial_DAC/bit_count_4  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/bit_count_mux0000 [0]),
    .Q(\Inst_ad5660_serial_DAC/bit_count [4])
  );
  FD   \Inst_ad5660_serial_DAC/bit_count_3  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/bit_count_mux0000 [1]),
    .Q(\Inst_ad5660_serial_DAC/bit_count [3])
  );
  FD   \Inst_ad5660_serial_DAC/bit_count_2  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/bit_count_mux0000 [2]),
    .Q(\Inst_ad5660_serial_DAC/bit_count [2])
  );
  FD   \Inst_ad5660_serial_DAC/bit_count_1  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/bit_count_mux0000 [3]),
    .Q(\Inst_ad5660_serial_DAC/bit_count [1])
  );
  FD   \Inst_ad5660_serial_DAC/bit_count_0  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/bit_count_mux0000 [4]),
    .Q(\Inst_ad5660_serial_DAC/bit_count [0])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \communication/state_FSM_FFd2  (
    .C(clk_148),
    .D(\communication/state_FSM_FFd2-In ),
    .S(\communication/state_and0000 ),
    .Q(\communication/state_FSM_FFd2_2370 )
  );
  FDE   \communication/address_7  (
    .C(clk_148),
    .CE(\communication/state_FSM_FFd2_2370 ),
    .D(\communication/Mcount_address7 ),
    .Q(\communication/address [7])
  );
  FDE   \communication/address_6  (
    .C(clk_148),
    .CE(\communication/state_FSM_FFd2_2370 ),
    .D(\communication/Mcount_address6 ),
    .Q(\communication/address [6])
  );
  FDE   \communication/address_5  (
    .C(clk_148),
    .CE(\communication/state_FSM_FFd2_2370 ),
    .D(\communication/Mcount_address5 ),
    .Q(\communication/address [5])
  );
  FDE   \communication/address_4  (
    .C(clk_148),
    .CE(\communication/state_FSM_FFd2_2370 ),
    .D(\communication/Mcount_address4 ),
    .Q(\communication/address [4])
  );
  FDE   \communication/address_3  (
    .C(clk_148),
    .CE(\communication/state_FSM_FFd2_2370 ),
    .D(\communication/Mcount_address3 ),
    .Q(\communication/address [3])
  );
  FDE   \communication/address_2  (
    .C(clk_148),
    .CE(\communication/state_FSM_FFd2_2370 ),
    .D(\communication/Mcount_address2 ),
    .Q(\communication/address [2])
  );
  FDE   \communication/address_1  (
    .C(clk_148),
    .CE(\communication/state_FSM_FFd2_2370 ),
    .D(\communication/Mcount_address1 ),
    .Q(\communication/address [1])
  );
  FDE   \communication/address_0  (
    .C(clk_148),
    .CE(\communication/state_FSM_FFd2_2370 ),
    .D(\communication/Mcount_address ),
    .Q(\communication/address [0])
  );
  XORCY   \communication/Mcount_address_xor<7>  (
    .CI(\communication/Mcount_address_cy [6]),
    .LI(\communication/Mcount_address_lut [7]),
    .O(\communication/Mcount_address7 )
  );
  XORCY   \communication/Mcount_address_xor<6>  (
    .CI(\communication/Mcount_address_cy [5]),
    .LI(\communication/Mcount_address_lut [6]),
    .O(\communication/Mcount_address6 )
  );
  MUXCY   \communication/Mcount_address_cy<6>  (
    .CI(\communication/Mcount_address_cy [5]),
    .DI(tp2_o_OBUF_3479),
    .S(\communication/Mcount_address_lut [6]),
    .O(\communication/Mcount_address_cy [6])
  );
  XORCY   \communication/Mcount_address_xor<5>  (
    .CI(\communication/Mcount_address_cy [4]),
    .LI(\communication/Mcount_address_lut [5]),
    .O(\communication/Mcount_address5 )
  );
  MUXCY   \communication/Mcount_address_cy<5>  (
    .CI(\communication/Mcount_address_cy [4]),
    .DI(tp2_o_OBUF_3479),
    .S(\communication/Mcount_address_lut [5]),
    .O(\communication/Mcount_address_cy [5])
  );
  XORCY   \communication/Mcount_address_xor<4>  (
    .CI(\communication/Mcount_address_cy [3]),
    .LI(\communication/Mcount_address_lut [4]),
    .O(\communication/Mcount_address4 )
  );
  MUXCY   \communication/Mcount_address_cy<4>  (
    .CI(\communication/Mcount_address_cy [3]),
    .DI(tp2_o_OBUF_3479),
    .S(\communication/Mcount_address_lut [4]),
    .O(\communication/Mcount_address_cy [4])
  );
  XORCY   \communication/Mcount_address_xor<3>  (
    .CI(\communication/Mcount_address_cy [2]),
    .LI(\communication/Mcount_address_lut [3]),
    .O(\communication/Mcount_address3 )
  );
  MUXCY   \communication/Mcount_address_cy<3>  (
    .CI(\communication/Mcount_address_cy [2]),
    .DI(tp2_o_OBUF_3479),
    .S(\communication/Mcount_address_lut [3]),
    .O(\communication/Mcount_address_cy [3])
  );
  XORCY   \communication/Mcount_address_xor<2>  (
    .CI(\communication/Mcount_address_cy [1]),
    .LI(\communication/Mcount_address_lut [2]),
    .O(\communication/Mcount_address2 )
  );
  MUXCY   \communication/Mcount_address_cy<2>  (
    .CI(\communication/Mcount_address_cy [1]),
    .DI(tp2_o_OBUF_3479),
    .S(\communication/Mcount_address_lut [2]),
    .O(\communication/Mcount_address_cy [2])
  );
  XORCY   \communication/Mcount_address_xor<1>  (
    .CI(\communication/Mcount_address_cy [0]),
    .LI(\communication/Mcount_address_lut [1]),
    .O(\communication/Mcount_address1 )
  );
  MUXCY   \communication/Mcount_address_cy<1>  (
    .CI(\communication/Mcount_address_cy [0]),
    .DI(tp2_o_OBUF_3479),
    .S(\communication/Mcount_address_lut [1]),
    .O(\communication/Mcount_address_cy [1])
  );
  XORCY   \communication/Mcount_address_xor<0>  (
    .CI(\communication/state_cmp_eq0002_inv ),
    .LI(\communication/Mcount_address_lut [0]),
    .O(\communication/Mcount_address )
  );
  MUXCY   \communication/Mcount_address_cy<0>  (
    .CI(\communication/state_cmp_eq0002_inv ),
    .DI(tp2_o_OBUF_3479),
    .S(\communication/Mcount_address_lut [0]),
    .O(\communication/Mcount_address_cy [0])
  );
  FD   \communication/update_register_o  (
    .C(clk_148),
    .D(\communication/state_cmp_eq0005 ),
    .Q(\communication/update_register_o_2379 )
  );
  FD   \communication/address_o_7  (
    .C(clk_148),
    .D(\communication/address_o_mux0000 [7]),
    .Q(\communication/address_o [7])
  );
  FD   \communication/address_o_6  (
    .C(clk_148),
    .D(\communication/address_o_mux0000 [6]),
    .Q(\communication/address_o [6])
  );
  FD   \communication/address_o_5  (
    .C(clk_148),
    .D(\communication/address_o_mux0000 [5]),
    .Q(\communication/address_o [5])
  );
  FD   \communication/address_o_4  (
    .C(clk_148),
    .D(\communication/address_o_mux0000 [4]),
    .Q(\communication/address_o [4])
  );
  FD   \communication/address_o_3  (
    .C(clk_148),
    .D(\communication/address_o_mux0000 [3]),
    .Q(\communication/address_o [3])
  );
  FD   \communication/address_o_2  (
    .C(clk_148),
    .D(\communication/address_o_mux0000 [2]),
    .Q(\communication/address_o [2])
  );
  FD   \communication/address_o_1  (
    .C(clk_148),
    .D(\communication/address_o_mux0000 [1]),
    .Q(\communication/address_o [1])
  );
  FD   \communication/address_o_0  (
    .C(clk_148),
    .D(\communication/address_o_mux0000 [0]),
    .Q(\communication/address_o [0])
  );
  FD   \communication/update_line_1  (
    .C(clk_148),
    .D(\communication/update_line [0]),
    .Q(\communication/update_line [1])
  );
  FD   \communication/stop_line_1  (
    .C(clk_148),
    .D(\communication/stop_line [0]),
    .Q(\communication/stop_line [1])
  );
  FD   \communication/stop_line_0  (
    .C(clk_148),
    .D(\serial_interface/stop_o_3413 ),
    .Q(\communication/stop_line [0])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<26>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [25]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [26]),
    .O(\OCXO_pll/phase_detect/Result [26])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<25>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [24]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [25]),
    .O(\OCXO_pll/phase_detect/Result [25])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<25>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [24]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [25]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [25])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<24>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [23]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [24]),
    .O(\OCXO_pll/phase_detect/Result [24])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<24>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [23]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [24]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [24])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<23>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [22]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [23]),
    .O(\OCXO_pll/phase_detect/Result [23])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<23>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [22]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [23]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [23])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<22>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [21]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [22]),
    .O(\OCXO_pll/phase_detect/Result [22])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<22>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [21]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [22]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [22])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<21>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [20]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [21]),
    .O(\OCXO_pll/phase_detect/Result [21])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<21>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [20]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [21]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [21])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<20>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [19]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [20]),
    .O(\OCXO_pll/phase_detect/Result [20])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<20>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [19]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [20]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [20])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<19>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [18]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [19]),
    .O(\OCXO_pll/phase_detect/Result [19])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<19>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [18]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [19]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [19])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<18>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [17]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [18]),
    .O(\OCXO_pll/phase_detect/Result [18])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<18>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [17]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [18]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [18])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<17>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [16]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [17]),
    .O(\OCXO_pll/phase_detect/Result [17])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<17>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [16]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [17]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [17])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<16>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [15]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [16]),
    .O(\OCXO_pll/phase_detect/Result [16])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<16>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [15]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [16]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [16])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<15>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [14]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [15]),
    .O(\OCXO_pll/phase_detect/Result [15])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<15>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [14]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [15]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [15])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<14>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [13]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [14]),
    .O(\OCXO_pll/phase_detect/Result [14])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<14>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [13]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [14]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [14])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<13>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [12]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [13]),
    .O(\OCXO_pll/phase_detect/Result [13])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<13>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [12]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [13]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [13])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<12>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [11]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [12]),
    .O(\OCXO_pll/phase_detect/Result [12])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<12>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [11]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [12]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [12])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<11>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [10]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [11]),
    .O(\OCXO_pll/phase_detect/Result [11])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<11>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [10]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [11]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [11])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<10>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [9]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [10]),
    .O(\OCXO_pll/phase_detect/Result [10])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<10>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [9]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [10]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [10])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<9>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [8]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [9]),
    .O(\OCXO_pll/phase_detect/Result [9])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<9>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [8]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [9]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [9])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<8>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [7]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [8]),
    .O(\OCXO_pll/phase_detect/Result [8])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<8>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [7]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [8]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [8])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<7>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [6]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [7]),
    .O(\OCXO_pll/phase_detect/Result [7])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<7>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [6]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [7]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [7])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<6>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [5]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [6]),
    .O(\OCXO_pll/phase_detect/Result [6])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<6>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [5]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [6]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [6])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<5>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [4]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [5]),
    .O(\OCXO_pll/phase_detect/Result [5])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<5>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [4]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [5]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [5])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<4>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [3]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [4]),
    .O(\OCXO_pll/phase_detect/Result [4])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<4>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [3]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [4]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [4])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<3>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [2]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [3]),
    .O(\OCXO_pll/phase_detect/Result [3])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<3>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [2]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [3]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [3])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<2>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [1]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [2]),
    .O(\OCXO_pll/phase_detect/Result [2])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<2>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [1]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [2]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [2])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<1>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [0]),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [1]),
    .O(\OCXO_pll/phase_detect/Result [1])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<1>  (
    .CI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [0]),
    .DI(N1),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [1]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [1])
  );
  XORCY   \OCXO_pll/phase_detect/Mcount_phase_counter_xor<0>  (
    .CI(N1),
    .LI(\OCXO_pll/phase_detect/Mcount_phase_counter_cy<0>_rt_1414 ),
    .O(\OCXO_pll/phase_detect/Result [0])
  );
  MUXCY   \OCXO_pll/phase_detect/Mcount_phase_counter_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_detect/Mcount_phase_counter_cy<0>_rt_1414 ),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy [0])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_25  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [25]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [25])
  );
  FDR   \OCXO_pll/phase_detect/phase_counter_24  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [24]),
    .R(\OCXO_pll/phase_detect/Mcount_phase_counter_val ),
    .Q(\OCXO_pll/phase_detect/phase_counter [24])
  );
  FDR   \OCXO_pll/phase_detect/phase_counter_26  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [26]),
    .R(\OCXO_pll/phase_detect/Mcount_phase_counter_val ),
    .Q(\OCXO_pll/phase_detect/phase_counter [26])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_23  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [23]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [23])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_22  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [22]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [22])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_21  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [21]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [21])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_20  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [20]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [20])
  );
  FDR   \OCXO_pll/phase_detect/phase_counter_18  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [18]),
    .R(\OCXO_pll/phase_detect/Mcount_phase_counter_val ),
    .Q(\OCXO_pll/phase_detect/phase_counter [18])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_17  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [17]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [17])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_19  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [19]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [19])
  );
  FDR   \OCXO_pll/phase_detect/phase_counter_16  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [16]),
    .R(\OCXO_pll/phase_detect/Mcount_phase_counter_val ),
    .Q(\OCXO_pll/phase_detect/phase_counter [16])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_15  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [15]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [15])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_14  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [14]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [14])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_13  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [13]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [13])
  );
  FDR   \OCXO_pll/phase_detect/phase_counter_11  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [11]),
    .R(\OCXO_pll/phase_detect/Mcount_phase_counter_val ),
    .Q(\OCXO_pll/phase_detect/phase_counter [11])
  );
  FDR   \OCXO_pll/phase_detect/phase_counter_10  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [10]),
    .R(\OCXO_pll/phase_detect/Mcount_phase_counter_val ),
    .Q(\OCXO_pll/phase_detect/phase_counter [10])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_12  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [12]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [12])
  );
  FDR   \OCXO_pll/phase_detect/phase_counter_9  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [9]),
    .R(\OCXO_pll/phase_detect/Mcount_phase_counter_val ),
    .Q(\OCXO_pll/phase_detect/phase_counter [9])
  );
  FDR   \OCXO_pll/phase_detect/phase_counter_8  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [8]),
    .R(\OCXO_pll/phase_detect/Mcount_phase_counter_val ),
    .Q(\OCXO_pll/phase_detect/phase_counter [8])
  );
  FDR   \OCXO_pll/phase_detect/phase_counter_7  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [7]),
    .R(\OCXO_pll/phase_detect/Mcount_phase_counter_val ),
    .Q(\OCXO_pll/phase_detect/phase_counter [7])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_6  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [6]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [6])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_4  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [4]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [4])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_3  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [3]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [3])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_5  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [5]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [5])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_2  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [2]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [2])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_1  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [1]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [1])
  );
  FDRS   \OCXO_pll/phase_detect/phase_counter_0  (
    .C(clk_100),
    .D(\OCXO_pll/phase_detect/Result [0]),
    .R(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .S(\OCXO_pll/phase_detect/phase_counter_cst ),
    .Q(\OCXO_pll/phase_detect/phase_counter [0])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_27  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [26]),
    .Q(\OCXO_pll/phase_detect/phase_diff [27])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_26  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [25]),
    .Q(\OCXO_pll/phase_detect/phase_diff [26])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_25  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [24]),
    .Q(\OCXO_pll/phase_detect/phase_diff [25])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_24  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [23]),
    .Q(\OCXO_pll/phase_detect/phase_diff [24])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_23  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [22]),
    .Q(\OCXO_pll/phase_detect/phase_diff [23])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_22  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [21]),
    .Q(\OCXO_pll/phase_detect/phase_diff [22])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_21  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [20]),
    .Q(\OCXO_pll/phase_detect/phase_diff [21])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_20  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [19]),
    .Q(\OCXO_pll/phase_detect/phase_diff [20])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_19  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [18]),
    .Q(\OCXO_pll/phase_detect/phase_diff [19])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_18  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [17]),
    .Q(\OCXO_pll/phase_detect/phase_diff [18])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_17  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [16]),
    .Q(\OCXO_pll/phase_detect/phase_diff [17])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_16  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [15]),
    .Q(\OCXO_pll/phase_detect/phase_diff [16])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_15  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [14]),
    .Q(\OCXO_pll/phase_detect/phase_diff [15])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_14  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [13]),
    .Q(\OCXO_pll/phase_detect/phase_diff [14])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_13  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [12]),
    .Q(\OCXO_pll/phase_detect/phase_diff [13])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_12  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [11]),
    .Q(\OCXO_pll/phase_detect/phase_diff [12])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_11  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [10]),
    .Q(\OCXO_pll/phase_detect/phase_diff [11])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_10  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [9]),
    .Q(\OCXO_pll/phase_detect/phase_diff [10])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_9  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [8]),
    .Q(\OCXO_pll/phase_detect/phase_diff [9])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_8  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [7]),
    .Q(\OCXO_pll/phase_detect/phase_diff [8])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_7  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [6]),
    .Q(\OCXO_pll/phase_detect/phase_diff [7])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_6  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [5]),
    .Q(\OCXO_pll/phase_detect/phase_diff [6])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_5  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [4]),
    .Q(\OCXO_pll/phase_detect/phase_diff [5])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_4  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [3]),
    .Q(\OCXO_pll/phase_detect/phase_diff [4])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_3  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [2]),
    .Q(\OCXO_pll/phase_detect/phase_diff [3])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_2  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [1]),
    .Q(\OCXO_pll/phase_detect/phase_diff [2])
  );
  FDE   \OCXO_pll/phase_detect/phase_diff_1  (
    .C(clk_100),
    .CE(kvant_pps_3018),
    .D(\OCXO_pll/phase_detect/phase_counter [0]),
    .Q(\OCXO_pll/phase_detect/phase_diff [1])
  );
  FDE   \OCXO_pll/phase_detect/update_tick  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_detect/phase_counter_and0000_inv ),
    .D(\OCXO_pll/phase_detect/phase_counter_cmp_eq0000 ),
    .Q(\OCXO_pll/phase_detect/update_tick_1570 )
  );
  FDE   \OCXO_pll/phase_detect/int_pps_tick  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_detect/phase_counter_and0000_inv ),
    .D(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq0000 ),
    .Q(\OCXO_pll/phase_detect/int_pps_tick_1494 )
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<13>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [12]),
    .DI(\OCXO_pll/phase_loop/out_sum [27]),
    .S(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[13] ),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<12>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [11]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[12] ),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [12])
  );
  LUT3 #(
    .INIT ( 8'h01 ))
  \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut<12>  (
    .I0(\OCXO_pll/phase_loop/out_sum [24]),
    .I1(\OCXO_pll/phase_loop/out_sum [25]),
    .I2(\OCXO_pll/phase_loop/out_sum [26]),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[12] )
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<11>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [10]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[11] ),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [11])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut<11>  (
    .I0(\OCXO_pll/phase_loop/out_sum [20]),
    .I1(\OCXO_pll/phase_loop/out_sum [21]),
    .I2(\OCXO_pll/phase_loop/out_sum [22]),
    .I3(\OCXO_pll/phase_loop/out_sum [23]),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[11] )
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<10>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [9]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[10] ),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [10])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut<10>  (
    .I0(\OCXO_pll/phase_loop/out_sum [16]),
    .I1(\OCXO_pll/phase_loop/out_sum [17]),
    .I2(\OCXO_pll/phase_loop/out_sum [18]),
    .I3(\OCXO_pll/phase_loop/out_sum [19]),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[10] )
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<9>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [8]),
    .DI(N1),
    .S(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<9>_rt_1747 ),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [9])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<8>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [7]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[8] ),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [8])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<7>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [6]),
    .DI(N1),
    .S(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<7>_rt_1744 ),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [7])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<6>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [5]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[6] ),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [6])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<5>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [4]),
    .DI(N1),
    .S(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<5>_rt_1741 ),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [5])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<4>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [3]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[4] ),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [4])
  );
  LUT3 #(
    .INIT ( 8'h01 ))
  \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut<4>  (
    .I0(\OCXO_pll/phase_loop/out_sum [8]),
    .I1(\OCXO_pll/phase_loop/out_sum [9]),
    .I2(\OCXO_pll/phase_loop/out_sum [10]),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[4] )
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<3>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [2]),
    .DI(N1),
    .S(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[3] ),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [3])
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut<3>  (
    .I0(\OCXO_pll/phase_loop/out_sum [4]),
    .I1(\OCXO_pll/phase_loop/out_sum [5]),
    .I2(\OCXO_pll/phase_loop/out_sum [6]),
    .I3(\OCXO_pll/phase_loop/out_sum [7]),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[3] )
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<2>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [1]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[2] ),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [2])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<1>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [0]),
    .DI(N1),
    .S(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<1>_rt_1736 ),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [1])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[0] ),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [0])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<35>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [34]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<35>_rt_1641 ),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [35])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<34>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [33]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [34]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [34])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<34>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [33]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<34>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [34]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [34])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<34>  (
    .I0(\OCXO_pll/phase_loop/integrator [34]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[34] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [34])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<34>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [34]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<34>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<33>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [32]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [33]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [33])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<33>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [32]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<33>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [33]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [33])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<33>  (
    .I0(\OCXO_pll/phase_loop/integrator [33]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[33] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [33])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<33>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [33]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<33>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<32>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [31]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [32]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [32])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<32>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [31]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<32>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [32]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [32])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<32>  (
    .I0(\OCXO_pll/phase_loop/integrator [32]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[32] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [32])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<32>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [32]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<32>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<31>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [30]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [31]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [31])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<31>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [30]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<31>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [31]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [31])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<31>  (
    .I0(\OCXO_pll/phase_loop/integrator [31]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[31] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [31])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<31>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [31]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<31>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<30>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [29]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [30]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [30])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<30>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [29]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<30>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [30]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [30])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<30>  (
    .I0(\OCXO_pll/phase_loop/integrator [30]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[30] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [30])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<30>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [30]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<30>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<29>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [28]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [29]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [29])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<29>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [28]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<29>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [29]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [29])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<29>  (
    .I0(\OCXO_pll/phase_loop/integrator [29]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[29] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [29])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<29>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [29]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<29>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<28>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [27]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [28]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [28])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<28>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [27]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<28>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [28]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [28])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<28>  (
    .I0(\OCXO_pll/phase_loop/integrator [28]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[28] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [28])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<28>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [28]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<28>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<27>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [26]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [27]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [27])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<27>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [26]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<27>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [27]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [27])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<27>  (
    .I0(\OCXO_pll/phase_loop/integrator [27]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[27] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [27])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<27>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [27]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<27>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<26>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [25]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [26]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [26])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<26>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [25]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<26>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [26]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [26])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<26>  (
    .I0(\OCXO_pll/phase_loop/integrator [26]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[26] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [26])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<26>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [26]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<26>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<25>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [24]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [25]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [25])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<25>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [24]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<25>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [25]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [25])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<25>  (
    .I0(\OCXO_pll/phase_loop/integrator [25]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[25] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [25])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<25>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [25]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<25>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<24>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [23]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [24]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [24])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<24>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [23]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<24>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [24]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [24])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<24>  (
    .I0(\OCXO_pll/phase_loop/integrator [24]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[24] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [24])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<24>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [24]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<24>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<23>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [22]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [23]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [23])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<23>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [22]),
    .DI(\OCXO_pll/phase_loop/integrator_limit[23] ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [23]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [23])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<22>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [21]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [22]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [22])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<22>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [21]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<22>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [22]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [22])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<22>  (
    .I0(\OCXO_pll/phase_loop/integrator [22]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[22] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [22])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<22>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [22]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<22>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<21>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [20]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [21]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [21])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<21>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [20]),
    .DI(\OCXO_pll/phase_loop/integrator_limit[21] ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [21]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [21])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<20>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [19]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [20]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [20])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<20>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [19]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<20>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [20]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [20])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<20>  (
    .I0(\OCXO_pll/phase_loop/integrator [20]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[20] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [20])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<20>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [20]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<20>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<19>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [18]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [19]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [19])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<19>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [18]),
    .DI(\OCXO_pll/phase_loop/integrator_limit[19] ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [19]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [19])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<18>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [17]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [18]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [18])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<18>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [17]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<18>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [18]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [18])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<18>  (
    .I0(\OCXO_pll/phase_loop/integrator [18]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[18] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [18])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<18>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [18]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<18>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<17>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [16]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [17]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [17])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<17>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [16]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<17>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [17]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [17])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<17>  (
    .I0(\OCXO_pll/phase_loop/integrator [17]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[17] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [17])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<17>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [17]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<17>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<16>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [15]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [16]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [16])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<16>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [15]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<16>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [16]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [16])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<16>  (
    .I0(\OCXO_pll/phase_loop/integrator [16]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[16] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [16])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<16>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [16]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<16>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<15>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [14]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [15]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [15])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<15>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [14]),
    .DI(\OCXO_pll/phase_loop/integrator_limit[15] ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [15]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [15])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<14>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [13]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [14]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [14])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<14>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [13]),
    .DI(\OCXO_pll/phase_loop/integrator_limit[14] ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [14]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [14])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<13>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [12]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [13]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [13])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<13>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [12]),
    .DI(\OCXO_pll/phase_loop/integrator_limit[13] ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [13]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [13])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<12>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [11]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [12]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [12])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<12>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [11]),
    .DI(\OCXO_pll/phase_loop/integrator_limit[12] ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [12]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [12])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<11>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [10]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [11]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [11])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<11>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [10]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<11>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [11]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [11])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<11>  (
    .I0(\OCXO_pll/phase_loop/integrator [11]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[11] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [11])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<11>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [11]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<11>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<10>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [9]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [10]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [10])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<10>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [9]),
    .DI(\OCXO_pll/phase_loop/integrator_limit[10] ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [10]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [10])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<9>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [8]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [9]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [9])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<9>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [8]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<9>_mand3 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [9]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [9])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<9>  (
    .I0(\OCXO_pll/phase_loop/integrator [9]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[9] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [9])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<9>_mand_0  (
    .I0(\OCXO_pll/phase_loop/integrator [9]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<9>_mand3 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<8>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [7]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [8]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [8])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<8>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [7]),
    .DI(\OCXO_pll/phase_loop/integrator_limit[8] ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [8]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [8])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<7>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [6]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [7]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [7])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<7>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [6]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<7>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [7]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [7])
  );
  LUT4 #(
    .INIT ( 16'h9A30 ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<7>  (
    .I0(\OCXO_pll/phase_loop/integrator [7]),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [7]),
    .I3(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [7])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<7>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [7]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<7>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<6>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [5]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [6]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [6])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<6>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [5]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<6>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [6]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [6])
  );
  LUT4 #(
    .INIT ( 16'h9A30 ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<6>  (
    .I0(\OCXO_pll/phase_loop/integrator [6]),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [6]),
    .I3(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [6])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<6>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [6]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<6>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<5>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [4]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [5]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [5])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<5>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [4]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<5>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [5]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [5])
  );
  LUT4 #(
    .INIT ( 16'h9A30 ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<5>  (
    .I0(\OCXO_pll/phase_loop/integrator [5]),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [5]),
    .I3(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [5])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<5>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [5]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<5>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<4>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [3]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [4]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [4])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<4>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [3]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<4>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [4]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [4])
  );
  LUT4 #(
    .INIT ( 16'h9A30 ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<4>  (
    .I0(\OCXO_pll/phase_loop/integrator [4]),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [4]),
    .I3(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [4])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<4>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [4]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<4>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<3>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [2]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [3]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [3])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<3>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [2]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<3>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [3]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [3])
  );
  LUT4 #(
    .INIT ( 16'h9A30 ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<3>  (
    .I0(\OCXO_pll/phase_loop/integrator [3]),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [3]),
    .I3(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [3])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<3>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [3]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<3>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<2>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [1]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [2]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [2])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<2>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [1]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<2>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [2]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [2])
  );
  LUT4 #(
    .INIT ( 16'h9A30 ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<2>  (
    .I0(\OCXO_pll/phase_loop/integrator [2]),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [2]),
    .I3(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [2])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<2>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [2]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<2>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<1>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [0]),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [1]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [1])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<1>  (
    .CI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [0]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<1>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [1]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [1])
  );
  LUT4 #(
    .INIT ( 16'h9A30 ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<1>  (
    .I0(\OCXO_pll/phase_loop/integrator [1]),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [1]),
    .I3(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [1])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<1>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [1]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<1>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<0>  (
    .CI(tp2_o_OBUF_3479),
    .LI(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [0]),
    .O(\OCXO_pll/phase_loop/integrator_addsub0000 [0])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy<0>  (
    .CI(tp2_o_OBUF_3479),
    .DI(\OCXO_pll/phase_loop/integrator_limit<0>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [0]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_cy [0])
  );
  LUT4 #(
    .INIT ( 16'h9A30 ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<0>  (
    .I0(\OCXO_pll/phase_loop/integrator [0]),
    .I1(\system_control[3] ),
    .I2(tp2_o_OBUF_3479),
    .I3(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [0])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<0>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [0]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<0>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<27>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [26]),
    .LI(\OCXO_pll/phase_loop/integrator_mux0003<33>1_1952 ),
    .O(\OCXO_pll/phase_loop/out_sum [27])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<26>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [25]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [26]),
    .O(\OCXO_pll/phase_loop/out_sum [26])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<26>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [25]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<34>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [26]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [26])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<26>  (
    .I0(\OCXO_pll/phase_loop/integrator [34]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[32] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [26])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<34>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [34]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<34>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<25>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [24]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [25]),
    .O(\OCXO_pll/phase_loop/out_sum [25])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<25>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [24]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<33>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [25]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [25])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<25>  (
    .I0(\OCXO_pll/phase_loop/integrator [33]),
    .I1(\OCXO_pll/phase_loop/integrator_mux0003[31] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [25])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<33>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [33]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<33>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<24>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [23]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [24]),
    .O(\OCXO_pll/phase_loop/out_sum [24])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<24>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [23]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<32>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [24]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [24])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<24>  (
    .I0(\OCXO_pll/phase_loop/integrator [32]),
    .I1(\OCXO_pll/phase_loop/out_sum_mux0001[24] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [24])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<32>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [32]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<32>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<23>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [22]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [23]),
    .O(\OCXO_pll/phase_loop/out_sum [23])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<23>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [22]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<31>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [23]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [23])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<23>  (
    .I0(\OCXO_pll/phase_loop/integrator [31]),
    .I1(\OCXO_pll/phase_loop/out_sum_mux0001[23] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [23])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<31>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [31]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<31>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<22>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [21]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [22]),
    .O(\OCXO_pll/phase_loop/out_sum [22])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<22>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [21]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<30>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [22]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [22])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<22>  (
    .I0(\OCXO_pll/phase_loop/integrator [30]),
    .I1(\OCXO_pll/phase_loop/out_sum_mux0001[22] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [22])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<30>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [30]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<30>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<21>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [20]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [21]),
    .O(\OCXO_pll/phase_loop/out_sum [21])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<21>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [20]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<29>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [21]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [21])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<21>  (
    .I0(\OCXO_pll/phase_loop/integrator [29]),
    .I1(\OCXO_pll/phase_loop/out_sum_mux0001[21] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [21])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<29>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [29]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<29>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<20>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [19]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [20]),
    .O(\OCXO_pll/phase_loop/out_sum [20])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<20>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [19]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<28>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [20]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [20])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<20>  (
    .I0(\OCXO_pll/phase_loop/integrator [28]),
    .I1(\OCXO_pll/phase_loop/out_sum_mux0001[20] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [20])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<28>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [28]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<28>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<19>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [18]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [19]),
    .O(\OCXO_pll/phase_loop/out_sum [19])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<19>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [18]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<27>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [19]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [19])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<19>  (
    .I0(\OCXO_pll/phase_loop/integrator [27]),
    .I1(\OCXO_pll/phase_loop/out_sum_mux0001[19] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [19])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<27>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [27]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<27>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<18>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [17]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [18]),
    .O(\OCXO_pll/phase_loop/out_sum [18])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<18>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [17]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<26>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [18]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [18])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<18>  (
    .I0(\OCXO_pll/phase_loop/integrator [26]),
    .I1(\OCXO_pll/phase_loop/out_sum_mux0001[18] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1_1759 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [18])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<26>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [26]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1_1759 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<26>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<17>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [16]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [17]),
    .O(\OCXO_pll/phase_loop/out_sum [17])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<17>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [16]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<25>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [17]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [17])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<17>  (
    .I0(\OCXO_pll/phase_loop/integrator [25]),
    .I1(\OCXO_pll/phase_loop/out_sum_mux0001[17] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1_1759 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [17])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<25>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [25]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1_1759 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<25>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<16>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [15]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [16]),
    .O(\OCXO_pll/phase_loop/out_sum [16])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<16>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [15]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<24>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [16]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [16])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<16>  (
    .I0(\OCXO_pll/phase_loop/integrator [24]),
    .I1(\OCXO_pll/phase_loop/out_sum_mux0001[16] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1_1759 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [16])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<24>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [24]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1_1759 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<24>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<15>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [14]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [15]),
    .O(\OCXO_pll/phase_loop/out_sum [15])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<15>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [14]),
    .DI(\OCXO_pll/phase_loop/integrator_limit[23] ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [15]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [15])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<14>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [13]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [14]),
    .O(\OCXO_pll/phase_loop/out_sum [14])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<14>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [13]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<22>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [14]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [14])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<14>  (
    .I0(\OCXO_pll/phase_loop/integrator [22]),
    .I1(\OCXO_pll/phase_loop/out_sum_mux0001[14] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1_1759 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [14])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<22>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [22]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1_1759 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<22>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<13>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [12]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [13]),
    .O(\OCXO_pll/phase_loop/out_sum [13])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<13>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [12]),
    .DI(\OCXO_pll/phase_loop/integrator_limit[21] ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [13]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [13])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<12>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [11]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [12]),
    .O(\OCXO_pll/phase_loop/out_sum [12])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<12>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [11]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<20>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [12]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [12])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<12>  (
    .I0(\OCXO_pll/phase_loop/integrator [20]),
    .I1(\OCXO_pll/phase_loop/out_sum_mux0001[12] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1758 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [12])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<20>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [20]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1758 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<20>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<11>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [10]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [11]),
    .O(\OCXO_pll/phase_loop/out_sum [11])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<11>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [10]),
    .DI(\OCXO_pll/phase_loop/integrator_limit[19] ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [11]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [11])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<10>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [9]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [10]),
    .O(\OCXO_pll/phase_loop/out_sum [10])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<10>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [9]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<18>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [10]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [10])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<10>  (
    .I0(\OCXO_pll/phase_loop/integrator [18]),
    .I1(\OCXO_pll/phase_loop/out_sum_mux0001[10] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1758 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [10])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<18>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [18]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1758 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<18>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<9>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [8]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [9]),
    .O(\OCXO_pll/phase_loop/out_sum [9])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<9>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [8]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<17>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [9]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [9])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<9>  (
    .I0(\OCXO_pll/phase_loop/integrator [17]),
    .I1(\OCXO_pll/phase_loop/out_sum_mux0001[9] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1758 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [9])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<17>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [17]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1758 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<17>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<8>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [7]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [8]),
    .O(\OCXO_pll/phase_loop/out_sum [8])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<8>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [7]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<16>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [8]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [8])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<8>  (
    .I0(\OCXO_pll/phase_loop/integrator [16]),
    .I1(\OCXO_pll/phase_loop/out_sum_mux0001[8] ),
    .I2(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1758 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [8])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<16>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [16]),
    .I1(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1758 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<16>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<7>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [6]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [7]),
    .O(\OCXO_pll/phase_loop/out_sum [7])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<7>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [6]),
    .DI(\OCXO_pll/phase_loop/integrator_limit[15] ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [7]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [7])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<6>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [5]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [6]),
    .O(\OCXO_pll/phase_loop/out_sum [6])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<6>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [5]),
    .DI(\OCXO_pll/phase_loop/integrator_limit[14] ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [6]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [6])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<5>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [4]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [5]),
    .O(\OCXO_pll/phase_loop/out_sum [5])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<5>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [4]),
    .DI(\OCXO_pll/phase_loop/integrator_limit[13] ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [5]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [5])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<4>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [3]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [4]),
    .O(\OCXO_pll/phase_loop/out_sum [4])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<4>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [3]),
    .DI(\OCXO_pll/phase_loop/integrator_limit[12] ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [4]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [4])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<3>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [2]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [3]),
    .O(\OCXO_pll/phase_loop/out_sum [3])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<3>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [2]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<11>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [3]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [3])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<3>  (
    .I0(\OCXO_pll/phase_loop/integrator [11]),
    .I1(\OCXO_pll/phase_loop/out_sum_mux0001[3] ),
    .I2(\OCXO_pll/phase_loop/integrator_limit<0>_mand_1846 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [3])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<11>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [11]),
    .I1(\OCXO_pll/phase_loop/integrator_limit<0>_mand_1846 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<11>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<2>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [1]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [2]),
    .O(\OCXO_pll/phase_loop/out_sum [2])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<2>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [1]),
    .DI(\OCXO_pll/phase_loop/integrator_limit[10] ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [2]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [2])
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<1>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [0]),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [1]),
    .O(\OCXO_pll/phase_loop/out_sum [1])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<1>  (
    .CI(\OCXO_pll/phase_loop/Madd_out_sum_cy [0]),
    .DI(\OCXO_pll/phase_loop/integrator_limit<9>_mand1 ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [1]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [1])
  );
  MULT_AND   \OCXO_pll/phase_loop/integrator_limit<9>_mand  (
    .I0(\OCXO_pll/phase_loop/integrator [9]),
    .I1(\OCXO_pll/phase_loop/integrator_limit<0>_mand_1846 ),
    .LO(\OCXO_pll/phase_loop/integrator_limit<9>_mand1 )
  );
  XORCY   \OCXO_pll/phase_loop/Madd_out_sum_xor<0>  (
    .CI(tp2_o_OBUF_3479),
    .LI(\OCXO_pll/phase_loop/Madd_out_sum_lut [0]),
    .O(\OCXO_pll/phase_loop/out_sum [0])
  );
  MUXCY   \OCXO_pll/phase_loop/Madd_out_sum_cy<0>  (
    .CI(tp2_o_OBUF_3479),
    .DI(\OCXO_pll/phase_loop/integrator_limit[8] ),
    .S(\OCXO_pll/phase_loop/Madd_out_sum_lut [0]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_cy [0])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<16>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [15]),
    .DI(\OCXO_pll/phase_loop/integrator [35]),
    .S(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[16] ),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<15>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [14]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[15] ),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [15])
  );
  LUT3 #(
    .INIT ( 8'h01 ))
  \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut<15>  (
    .I0(\OCXO_pll/phase_loop/integrator [32]),
    .I1(\OCXO_pll/phase_loop/integrator [33]),
    .I2(\OCXO_pll/phase_loop/integrator [34]),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[15] )
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<14>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [13]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[14] ),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [14])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut<14>  (
    .I0(\OCXO_pll/phase_loop/integrator [28]),
    .I1(\OCXO_pll/phase_loop/integrator [29]),
    .I2(\OCXO_pll/phase_loop/integrator [30]),
    .I3(\OCXO_pll/phase_loop/integrator [31]),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[14] )
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<13>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [12]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[13] ),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [13])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut<13>  (
    .I0(\OCXO_pll/phase_loop/integrator [24]),
    .I1(\OCXO_pll/phase_loop/integrator [25]),
    .I2(\OCXO_pll/phase_loop/integrator [26]),
    .I3(\OCXO_pll/phase_loop/integrator [27]),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[13] )
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<12>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [11]),
    .DI(N1),
    .S(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<12>_rt_1701 ),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [12])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<11>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [10]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[11] ),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [11])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<10>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [9]),
    .DI(N1),
    .S(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<10>_rt_1698 ),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [10])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<9>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [8]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[9] ),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [9])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<8>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [7]),
    .DI(N1),
    .S(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<8>_rt_1716 ),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [8])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<7>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [6]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[7] ),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [7])
  );
  LUT3 #(
    .INIT ( 8'h01 ))
  \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut<7>  (
    .I0(\OCXO_pll/phase_loop/integrator [16]),
    .I1(\OCXO_pll/phase_loop/integrator [17]),
    .I2(\OCXO_pll/phase_loop/integrator [18]),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[7] )
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<6>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [5]),
    .DI(N1),
    .S(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[6] ),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [6])
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut<6>  (
    .I0(\OCXO_pll/phase_loop/integrator [15]),
    .I1(\OCXO_pll/phase_loop/integrator [14]),
    .I2(\OCXO_pll/phase_loop/integrator [13]),
    .I3(\OCXO_pll/phase_loop/integrator [12]),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[6] )
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<5>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [4]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[5] ),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [5])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<4>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [3]),
    .DI(N1),
    .S(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<4>_rt_1711 ),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [4])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<3>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [2]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[3] ),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [3])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<2>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [1]),
    .DI(N1),
    .S(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<2>_rt_1708 ),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [2])
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<1>  (
    .CI(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [0]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[1] ),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [1])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut<1>  (
    .I0(\OCXO_pll/phase_loop/integrator [4]),
    .I1(\OCXO_pll/phase_loop/integrator [5]),
    .I2(\OCXO_pll/phase_loop/integrator [6]),
    .I3(\OCXO_pll/phase_loop/integrator [7]),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[1] )
  );
  MUXCY   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[0] ),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [0])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut<0>  (
    .I0(\OCXO_pll/phase_loop/integrator [0]),
    .I1(\OCXO_pll/phase_loop/integrator [1]),
    .I2(\OCXO_pll/phase_loop/integrator [2]),
    .I3(\OCXO_pll/phase_loop/integrator [3]),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[0] )
  );
  FDE   \OCXO_pll/phase_loop/integrator_35  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [35]),
    .Q(\OCXO_pll/phase_loop/integrator [35])
  );
  FDE   \OCXO_pll/phase_loop/integrator_34  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [34]),
    .Q(\OCXO_pll/phase_loop/integrator [34])
  );
  FDE   \OCXO_pll/phase_loop/integrator_33  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [33]),
    .Q(\OCXO_pll/phase_loop/integrator [33])
  );
  FDE   \OCXO_pll/phase_loop/integrator_32  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [32]),
    .Q(\OCXO_pll/phase_loop/integrator [32])
  );
  FDE   \OCXO_pll/phase_loop/integrator_31  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [31]),
    .Q(\OCXO_pll/phase_loop/integrator [31])
  );
  FDE   \OCXO_pll/phase_loop/integrator_30  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [30]),
    .Q(\OCXO_pll/phase_loop/integrator [30])
  );
  FDE   \OCXO_pll/phase_loop/integrator_29  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [29]),
    .Q(\OCXO_pll/phase_loop/integrator [29])
  );
  FDE   \OCXO_pll/phase_loop/integrator_28  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [28]),
    .Q(\OCXO_pll/phase_loop/integrator [28])
  );
  FDE   \OCXO_pll/phase_loop/integrator_27  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [27]),
    .Q(\OCXO_pll/phase_loop/integrator [27])
  );
  FDE   \OCXO_pll/phase_loop/integrator_26  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [26]),
    .Q(\OCXO_pll/phase_loop/integrator [26])
  );
  FDE   \OCXO_pll/phase_loop/integrator_25  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [25]),
    .Q(\OCXO_pll/phase_loop/integrator [25])
  );
  FDE   \OCXO_pll/phase_loop/integrator_24  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [24]),
    .Q(\OCXO_pll/phase_loop/integrator [24])
  );
  FDE   \OCXO_pll/phase_loop/integrator_23  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [23]),
    .Q(\OCXO_pll/phase_loop/integrator [23])
  );
  FDE   \OCXO_pll/phase_loop/integrator_22  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [22]),
    .Q(\OCXO_pll/phase_loop/integrator [22])
  );
  FDE   \OCXO_pll/phase_loop/integrator_21  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [21]),
    .Q(\OCXO_pll/phase_loop/integrator [21])
  );
  FDE   \OCXO_pll/phase_loop/integrator_20  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [20]),
    .Q(\OCXO_pll/phase_loop/integrator [20])
  );
  FDE   \OCXO_pll/phase_loop/integrator_19  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [19]),
    .Q(\OCXO_pll/phase_loop/integrator [19])
  );
  FDE   \OCXO_pll/phase_loop/integrator_18  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [18]),
    .Q(\OCXO_pll/phase_loop/integrator [18])
  );
  FDE   \OCXO_pll/phase_loop/integrator_17  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [17]),
    .Q(\OCXO_pll/phase_loop/integrator [17])
  );
  FDE   \OCXO_pll/phase_loop/integrator_16  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [16]),
    .Q(\OCXO_pll/phase_loop/integrator [16])
  );
  FDE   \OCXO_pll/phase_loop/integrator_15  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [15]),
    .Q(\OCXO_pll/phase_loop/integrator [15])
  );
  FDE   \OCXO_pll/phase_loop/integrator_14  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [14]),
    .Q(\OCXO_pll/phase_loop/integrator [14])
  );
  FDE   \OCXO_pll/phase_loop/integrator_13  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [13]),
    .Q(\OCXO_pll/phase_loop/integrator [13])
  );
  FDE   \OCXO_pll/phase_loop/integrator_12  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [12]),
    .Q(\OCXO_pll/phase_loop/integrator [12])
  );
  FDE   \OCXO_pll/phase_loop/integrator_11  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [11]),
    .Q(\OCXO_pll/phase_loop/integrator [11])
  );
  FDE   \OCXO_pll/phase_loop/integrator_10  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [10]),
    .Q(\OCXO_pll/phase_loop/integrator [10])
  );
  FDE   \OCXO_pll/phase_loop/integrator_9  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [9]),
    .Q(\OCXO_pll/phase_loop/integrator [9])
  );
  FDE   \OCXO_pll/phase_loop/integrator_8  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [8]),
    .Q(\OCXO_pll/phase_loop/integrator [8])
  );
  FDE   \OCXO_pll/phase_loop/integrator_7  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [7]),
    .Q(\OCXO_pll/phase_loop/integrator [7])
  );
  FDE   \OCXO_pll/phase_loop/integrator_6  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [6]),
    .Q(\OCXO_pll/phase_loop/integrator [6])
  );
  FDE   \OCXO_pll/phase_loop/integrator_5  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [5]),
    .Q(\OCXO_pll/phase_loop/integrator [5])
  );
  FDE   \OCXO_pll/phase_loop/integrator_4  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [4]),
    .Q(\OCXO_pll/phase_loop/integrator [4])
  );
  FDE   \OCXO_pll/phase_loop/integrator_3  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [3]),
    .Q(\OCXO_pll/phase_loop/integrator [3])
  );
  FDE   \OCXO_pll/phase_loop/integrator_2  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [2]),
    .Q(\OCXO_pll/phase_loop/integrator [2])
  );
  FDE   \OCXO_pll/phase_loop/integrator_1  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [1]),
    .Q(\OCXO_pll/phase_loop/integrator [1])
  );
  FDE   \OCXO_pll/phase_loop/integrator_0  (
    .C(clk_100),
    .CE(\OCXO_pll/phase_loop/integrator_not0002 ),
    .D(\OCXO_pll/phase_loop/integrator_mux0002 [0]),
    .Q(\OCXO_pll/phase_loop/integrator [0])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/out_sum_2  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/integrator [10]),
    .Q(\VCXO_pll/VCXO_phase_loop/out_sum [2])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/out_sum_3  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/integrator [11]),
    .Q(\VCXO_pll/VCXO_phase_loop/out_sum [3])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/out_sum_4  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/integrator [12]),
    .Q(\VCXO_pll/VCXO_phase_loop/out_sum [4])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/out_sum_5  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/integrator [13]),
    .Q(\VCXO_pll/VCXO_phase_loop/out_sum [5])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/out_sum_6  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [6]),
    .Q(\VCXO_pll/VCXO_phase_loop/out_sum [6])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/out_sum_7  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [7]),
    .Q(\VCXO_pll/VCXO_phase_loop/out_sum [7])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/out_sum_8  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [8]),
    .Q(\VCXO_pll/VCXO_phase_loop/out_sum [8])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/out_sum_9  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [9]),
    .Q(\VCXO_pll/VCXO_phase_loop/out_sum [9])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/out_sum_10  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [10]),
    .Q(\VCXO_pll/VCXO_phase_loop/out_sum [10])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/out_sum_11  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [11]),
    .Q(\VCXO_pll/VCXO_phase_loop/out_sum [11])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/out_sum_12  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [12]),
    .Q(\VCXO_pll/VCXO_phase_loop/out_sum [12])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/out_sum_13  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [13]),
    .Q(\VCXO_pll/VCXO_phase_loop/out_sum [13])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/out_sum_14  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [14]),
    .Q(\VCXO_pll/VCXO_phase_loop/out_sum [14])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/out_sum_15  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [15]),
    .Q(\VCXO_pll/VCXO_phase_loop/out_sum [15])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/out_sum_16  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [16]),
    .Q(\VCXO_pll/VCXO_phase_loop/out_sum [16])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/out_sum_17  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [17]),
    .Q(\VCXO_pll/VCXO_phase_loop/out_sum [17])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_3  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [3]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [3])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_4  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [4]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [4])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_5  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [5]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [5])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_6  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [6]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [6])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_7  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [7]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [7])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_8  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [8]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [8])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_9  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [9]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [9])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_10  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [10]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [10])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_11  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [11]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [11])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_12  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [12]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [12])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_13  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [13]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [13])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_14  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [14]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [14])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_15  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [15]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [15])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_16  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [16]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [16])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_17  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [17]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [17])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_18  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [18]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [18])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_19  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [19]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [19])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_20  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [20]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [20])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_21  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [21]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [21])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_22  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [22]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [22])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_23  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [23]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [23])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_24  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [24]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [24])
  );
  FDE #(
    .INIT ( 1'b0 ))
  \VCXO_pll/VCXO_phase_loop/integrator_25  (
    .C(clk_148),
    .CE(\VCXO_pll/phase_detector/update_tick_2288 ),
    .D(\VCXO_pll/VCXO_phase_loop/Result [25]),
    .Q(\VCXO_pll/VCXO_phase_loop/integrator [25])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut<6>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [14]),
    .I1(\VCXO_pll/phase_detector/phase_diff [0]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [6])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy<6>  (
    .CI(tp2_o_OBUF_3479),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [14]),
    .S(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [6]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [6])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_xor<6>  (
    .CI(tp2_o_OBUF_3479),
    .LI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [6]),
    .O(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [6])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut<7>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [15]),
    .I1(\VCXO_pll/phase_detector/phase_diff [1]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [7])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy<7>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [6]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [15]),
    .S(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [7]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [7])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_xor<7>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [6]),
    .LI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [7]),
    .O(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [7])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut<8>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [16]),
    .I1(\VCXO_pll/phase_detector/phase_diff [2]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [8])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy<8>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [7]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [16]),
    .S(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [8]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [8])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_xor<8>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [7]),
    .LI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [8]),
    .O(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [8])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut<9>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [17]),
    .I1(\VCXO_pll/phase_detector/phase_diff [3]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [9])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy<9>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [8]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [17]),
    .S(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [9])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_xor<9>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [8]),
    .LI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [9]),
    .O(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [9])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut<10>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [18]),
    .I1(\VCXO_pll/phase_detector/phase_diff [4]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [10])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy<10>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [9]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [18]),
    .S(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [10]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [10])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_xor<10>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [9]),
    .LI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [10]),
    .O(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [10])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut<11>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [19]),
    .I1(\VCXO_pll/phase_detector/phase_diff [5]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [11])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy<11>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [10]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [19]),
    .S(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [11]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [11])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_xor<11>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [10]),
    .LI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [11]),
    .O(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [11])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut<12>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [20]),
    .I1(\VCXO_pll/phase_detector/phase_diff [6]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [12])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy<12>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [11]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [20]),
    .S(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [12]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [12])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_xor<12>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [11]),
    .LI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [12]),
    .O(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [12])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut<13>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [21]),
    .I1(\VCXO_pll/phase_detector/phase_diff [7]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [13])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy<13>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [12]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [21]),
    .S(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [13]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [13])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_xor<13>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [12]),
    .LI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [13]),
    .O(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [13])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut<14>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [22]),
    .I1(\VCXO_pll/phase_detector/phase_diff [8]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [14])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy<14>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [13]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [22]),
    .S(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [14]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [14])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_xor<14>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [13]),
    .LI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [14]),
    .O(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [14])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut<15>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [23]),
    .I1(\VCXO_pll/phase_detector/phase_diff [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [15])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy<15>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [14]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [23]),
    .S(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [15]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [15])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_xor<15>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [14]),
    .LI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [15]),
    .O(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [15])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut<16>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [24]),
    .I1(\VCXO_pll/phase_detector/phase_diff [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [16])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy<16>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [15]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [24]),
    .S(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [16]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [16])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_xor<16>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [15]),
    .LI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [16]),
    .O(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [16])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut<17>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [25]),
    .I1(\VCXO_pll/phase_detector/phase_diff [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [25])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_xor<17>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_cy [16]),
    .LI(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [17]),
    .O(\VCXO_pll/VCXO_phase_loop/out_sum_mux0001 [17])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<3>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [3]),
    .I1(\VCXO_pll/phase_detector/phase_diff [0]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [3])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<3>  (
    .CI(tp2_o_OBUF_3479),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [3]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [3]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [3])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<3>  (
    .CI(tp2_o_OBUF_3479),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [3]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [3])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<4>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [4]),
    .I1(\VCXO_pll/phase_detector/phase_diff [1]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [4])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<4>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [3]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [4]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [4]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [4])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<4>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [3]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [4]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [4])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<5>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [5]),
    .I1(\VCXO_pll/phase_detector/phase_diff [2]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [5])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<5>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [4]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [5]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [5]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [5])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<5>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [4]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [5]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [5])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<6>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [6]),
    .I1(\VCXO_pll/phase_detector/phase_diff [3]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [6])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<6>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [5]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [6]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [6]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [6])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<6>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [5]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [6]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [6])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<7>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [7]),
    .I1(\VCXO_pll/phase_detector/phase_diff [4]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [7])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<7>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [6]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [7]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [7]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [7])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<7>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [6]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [7]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [7])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<8>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [8]),
    .I1(\VCXO_pll/phase_detector/phase_diff [5]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [8])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<8>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [7]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [8]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [8]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [8])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<8>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [7]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [8]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [8])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<9>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [9]),
    .I1(\VCXO_pll/phase_detector/phase_diff [6]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [9])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<9>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [8]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [9]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [9])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<9>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [8]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [9])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<10>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [10]),
    .I1(\VCXO_pll/phase_detector/phase_diff [7]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [10])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<10>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [9]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [10]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [10]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [10])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<10>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [9]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [10]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [10])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<11>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [11]),
    .I1(\VCXO_pll/phase_detector/phase_diff [8]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [11])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<11>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [10]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [11]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [11]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [11])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<11>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [10]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [11]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [11])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<12>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [12]),
    .I1(\VCXO_pll/phase_detector/phase_diff [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [12])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<12>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [11]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [12]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [12]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [12])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<12>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [11]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [12]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [12])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<13>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [13]),
    .I1(\VCXO_pll/phase_detector/phase_diff [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [13])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<13>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [12]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [13]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [13]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [13])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<13>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [12]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [13]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [13])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<14>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [14]),
    .I1(\VCXO_pll/phase_detector/phase_diff [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [14])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<14>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [13]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [14]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [14]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [14])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<14>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [13]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [14]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [14])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<15>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [15]),
    .I1(\VCXO_pll/phase_detector/phase_diff [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [15])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<15>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [14]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [15]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [15]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [15])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<15>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [14]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [15]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [15])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<16>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [16]),
    .I1(\VCXO_pll/phase_detector/phase_diff [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [16])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<16>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [15]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [16]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [16]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [16])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<16>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [15]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [16]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [16])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<17>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [17]),
    .I1(\VCXO_pll/phase_detector/phase_diff [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [17])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<17>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [16]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [17]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [17]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [17])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<17>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [16]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [17]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [17])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<18>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [18]),
    .I1(\VCXO_pll/phase_detector/phase_diff [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [18])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<18>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [17]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [18]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [18]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [18])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<18>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [17]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [18]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [18])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<19>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [19]),
    .I1(\VCXO_pll/phase_detector/phase_diff [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [19])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<19>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [18]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [19]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [19]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [19])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<19>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [18]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [19]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [19])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<20>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [20]),
    .I1(\VCXO_pll/phase_detector/phase_diff [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [20])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<20>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [19]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [20]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [20]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [20])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<20>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [19]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [20]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [20])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<21>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [21]),
    .I1(\VCXO_pll/phase_detector/phase_diff [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [21])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<21>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [20]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [21]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [21]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [21])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<21>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [20]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [21]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [21])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<22>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [22]),
    .I1(\VCXO_pll/phase_detector/phase_diff [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [22])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<22>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [21]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [22]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [22]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [22])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<22>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [21]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [22]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [22])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<23>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [23]),
    .I1(\VCXO_pll/phase_detector/phase_diff [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [23])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<23>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [22]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [23]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [23]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [23])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<23>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [22]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [23]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [23])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut<24>  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [24]),
    .I1(\VCXO_pll/phase_detector/phase_diff [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [24])
  );
  MUXCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy<24>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [23]),
    .DI(\VCXO_pll/VCXO_phase_loop/integrator [24]),
    .S(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [24]),
    .O(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [24])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<24>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [23]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [24]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [24])
  );
  XORCY   \VCXO_pll/VCXO_phase_loop/Maccum_integrator_xor<25>  (
    .CI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_cy [24]),
    .LI(\VCXO_pll/VCXO_phase_loop/Maccum_integrator_lut [25]),
    .O(\VCXO_pll/VCXO_phase_loop/Result [25])
  );
  FDR   \VCXO_pll/phase_detector/update_tick  (
    .C(clk_148),
    .D(N1),
    .R(\VCXO_pll/phase_detector/update_tick_not0001 ),
    .Q(\VCXO_pll/phase_detector/update_tick_2288 )
  );
  FDE   \VCXO_pll/phase_detector/phase_diff_0  (
    .C(clk_148),
    .CE(\VCXO_pll/ref_clk_2291 ),
    .D(\VCXO_pll/phase_detector/phase_counter [0]),
    .Q(\VCXO_pll/phase_detector/phase_diff [0])
  );
  FDE   \VCXO_pll/phase_detector/phase_diff_1  (
    .C(clk_148),
    .CE(\VCXO_pll/ref_clk_2291 ),
    .D(\VCXO_pll/phase_detector/phase_counter [1]),
    .Q(\VCXO_pll/phase_detector/phase_diff [1])
  );
  FDE   \VCXO_pll/phase_detector/phase_diff_2  (
    .C(clk_148),
    .CE(\VCXO_pll/ref_clk_2291 ),
    .D(\VCXO_pll/phase_detector/phase_counter [2]),
    .Q(\VCXO_pll/phase_detector/phase_diff [2])
  );
  FDE   \VCXO_pll/phase_detector/phase_diff_3  (
    .C(clk_148),
    .CE(\VCXO_pll/ref_clk_2291 ),
    .D(\VCXO_pll/phase_detector/phase_counter [3]),
    .Q(\VCXO_pll/phase_detector/phase_diff [3])
  );
  FDE   \VCXO_pll/phase_detector/phase_diff_4  (
    .C(clk_148),
    .CE(\VCXO_pll/ref_clk_2291 ),
    .D(\VCXO_pll/phase_detector/phase_counter [4]),
    .Q(\VCXO_pll/phase_detector/phase_diff [4])
  );
  FDE   \VCXO_pll/phase_detector/phase_diff_5  (
    .C(clk_148),
    .CE(\VCXO_pll/ref_clk_2291 ),
    .D(\VCXO_pll/phase_detector/phase_counter [5]),
    .Q(\VCXO_pll/phase_detector/phase_diff [5])
  );
  FDE   \VCXO_pll/phase_detector/phase_diff_6  (
    .C(clk_148),
    .CE(\VCXO_pll/ref_clk_2291 ),
    .D(\VCXO_pll/phase_detector/phase_counter [6]),
    .Q(\VCXO_pll/phase_detector/phase_diff [6])
  );
  FDE   \VCXO_pll/phase_detector/phase_diff_7  (
    .C(clk_148),
    .CE(\VCXO_pll/ref_clk_2291 ),
    .D(\VCXO_pll/phase_detector/phase_counter [7]),
    .Q(\VCXO_pll/phase_detector/phase_diff [7])
  );
  FDE   \VCXO_pll/phase_detector/phase_diff_8  (
    .C(clk_148),
    .CE(\VCXO_pll/ref_clk_2291 ),
    .D(\VCXO_pll/phase_detector/phase_counter [8]),
    .Q(\VCXO_pll/phase_detector/phase_diff [8])
  );
  FDE   \VCXO_pll/phase_detector/phase_diff_9  (
    .C(clk_148),
    .CE(\VCXO_pll/ref_clk_2291 ),
    .D(\VCXO_pll/phase_detector/phase_counter [9]),
    .Q(\VCXO_pll/phase_detector/phase_diff [9])
  );
  FDR   \VCXO_pll/phase_detector/phase_counter_1  (
    .C(clk_148),
    .D(\VCXO_pll/phase_detector/Result [1]),
    .R(\VCXO_pll/phase_detector/phase_counter_cmp_eq0000 ),
    .Q(\VCXO_pll/phase_detector/phase_counter [1])
  );
  FDR   \VCXO_pll/phase_detector/phase_counter_0  (
    .C(clk_148),
    .D(\VCXO_pll/phase_detector/Result [0]),
    .R(\VCXO_pll/phase_detector/phase_counter_cmp_eq0000 ),
    .Q(\VCXO_pll/phase_detector/phase_counter [0])
  );
  FDR   \VCXO_pll/phase_detector/phase_counter_4  (
    .C(clk_148),
    .D(\VCXO_pll/phase_detector/Result [4]),
    .R(\VCXO_pll/phase_detector/phase_counter_cmp_eq0000 ),
    .Q(\VCXO_pll/phase_detector/phase_counter [4])
  );
  FDR   \VCXO_pll/phase_detector/phase_counter_2  (
    .C(clk_148),
    .D(\VCXO_pll/phase_detector/Result [2]),
    .R(\VCXO_pll/phase_detector/phase_counter_cmp_eq0000 ),
    .Q(\VCXO_pll/phase_detector/phase_counter [2])
  );
  FDS   \VCXO_pll/phase_detector/phase_counter_3  (
    .C(clk_148),
    .D(\VCXO_pll/phase_detector/Result [3]),
    .S(\VCXO_pll/phase_detector/phase_counter_cmp_eq0000 ),
    .Q(\VCXO_pll/phase_detector/phase_counter [3])
  );
  FDR   \VCXO_pll/phase_detector/phase_counter_7  (
    .C(clk_148),
    .D(\VCXO_pll/phase_detector/Result [7]),
    .R(\VCXO_pll/phase_detector/phase_counter_cmp_eq0000 ),
    .Q(\VCXO_pll/phase_detector/phase_counter [7])
  );
  FDS   \VCXO_pll/phase_detector/phase_counter_5  (
    .C(clk_148),
    .D(\VCXO_pll/phase_detector/Result [5]),
    .S(\VCXO_pll/phase_detector/phase_counter_cmp_eq0000 ),
    .Q(\VCXO_pll/phase_detector/phase_counter [5])
  );
  FDR   \VCXO_pll/phase_detector/phase_counter_6  (
    .C(clk_148),
    .D(\VCXO_pll/phase_detector/Result [6]),
    .R(\VCXO_pll/phase_detector/phase_counter_cmp_eq0000 ),
    .Q(\VCXO_pll/phase_detector/phase_counter [6])
  );
  FDS   \VCXO_pll/phase_detector/phase_counter_8  (
    .C(clk_148),
    .D(\VCXO_pll/phase_detector/Result [8]),
    .S(\VCXO_pll/phase_detector/phase_counter_cmp_eq0000 ),
    .Q(\VCXO_pll/phase_detector/phase_counter [8])
  );
  FDR   \VCXO_pll/phase_detector/phase_counter_9  (
    .C(clk_148),
    .D(\VCXO_pll/phase_detector/Result [9]),
    .R(\VCXO_pll/phase_detector/phase_counter_cmp_eq0000 ),
    .Q(\VCXO_pll/phase_detector/phase_counter [9])
  );
  MUXCY   \VCXO_pll/phase_detector/Mcount_phase_counter_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\VCXO_pll/phase_detector/Mcount_phase_counter_cy<0>_rt_2237 ),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [0])
  );
  XORCY   \VCXO_pll/phase_detector/Mcount_phase_counter_xor<0>  (
    .CI(N1),
    .LI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy<0>_rt_2237 ),
    .O(\VCXO_pll/phase_detector/Result [0])
  );
  MUXCY   \VCXO_pll/phase_detector/Mcount_phase_counter_cy<1>  (
    .CI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [0]),
    .DI(N1),
    .S(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [1]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [1])
  );
  XORCY   \VCXO_pll/phase_detector/Mcount_phase_counter_xor<1>  (
    .CI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [0]),
    .LI(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [1]),
    .O(\VCXO_pll/phase_detector/Result [1])
  );
  MUXCY   \VCXO_pll/phase_detector/Mcount_phase_counter_cy<2>  (
    .CI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [1]),
    .DI(N1),
    .S(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [2]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [2])
  );
  XORCY   \VCXO_pll/phase_detector/Mcount_phase_counter_xor<2>  (
    .CI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [1]),
    .LI(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [2]),
    .O(\VCXO_pll/phase_detector/Result [2])
  );
  MUXCY   \VCXO_pll/phase_detector/Mcount_phase_counter_cy<3>  (
    .CI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [2]),
    .DI(N1),
    .S(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [3]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [3])
  );
  XORCY   \VCXO_pll/phase_detector/Mcount_phase_counter_xor<3>  (
    .CI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [2]),
    .LI(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [3]),
    .O(\VCXO_pll/phase_detector/Result [3])
  );
  MUXCY   \VCXO_pll/phase_detector/Mcount_phase_counter_cy<4>  (
    .CI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [3]),
    .DI(N1),
    .S(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [4]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [4])
  );
  XORCY   \VCXO_pll/phase_detector/Mcount_phase_counter_xor<4>  (
    .CI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [3]),
    .LI(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [4]),
    .O(\VCXO_pll/phase_detector/Result [4])
  );
  MUXCY   \VCXO_pll/phase_detector/Mcount_phase_counter_cy<5>  (
    .CI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [4]),
    .DI(N1),
    .S(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [5]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [5])
  );
  XORCY   \VCXO_pll/phase_detector/Mcount_phase_counter_xor<5>  (
    .CI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [4]),
    .LI(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [5]),
    .O(\VCXO_pll/phase_detector/Result [5])
  );
  MUXCY   \VCXO_pll/phase_detector/Mcount_phase_counter_cy<6>  (
    .CI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [5]),
    .DI(N1),
    .S(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [6]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [6])
  );
  XORCY   \VCXO_pll/phase_detector/Mcount_phase_counter_xor<6>  (
    .CI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [5]),
    .LI(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [6]),
    .O(\VCXO_pll/phase_detector/Result [6])
  );
  MUXCY   \VCXO_pll/phase_detector/Mcount_phase_counter_cy<7>  (
    .CI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [6]),
    .DI(N1),
    .S(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [7]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [7])
  );
  XORCY   \VCXO_pll/phase_detector/Mcount_phase_counter_xor<7>  (
    .CI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [6]),
    .LI(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [7]),
    .O(\VCXO_pll/phase_detector/Result [7])
  );
  MUXCY   \VCXO_pll/phase_detector/Mcount_phase_counter_cy<8>  (
    .CI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [7]),
    .DI(N1),
    .S(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [8]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [8])
  );
  XORCY   \VCXO_pll/phase_detector/Mcount_phase_counter_xor<8>  (
    .CI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [7]),
    .LI(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [8]),
    .O(\VCXO_pll/phase_detector/Result [8])
  );
  XORCY   \VCXO_pll/phase_detector/Mcount_phase_counter_xor<9>  (
    .CI(\VCXO_pll/phase_detector/Mcount_phase_counter_cy [8]),
    .LI(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [9]),
    .O(\VCXO_pll/phase_detector/Result [9])
  );
  XORCY   \VCXO_pll/Mcount_ref_clk_count_xor<8>  (
    .CI(\VCXO_pll/Mcount_ref_clk_count_cy [7]),
    .LI(\VCXO_pll/Mcount_ref_clk_count_xor<8>_rt_2068 ),
    .O(\VCXO_pll/Result [8])
  );
  XORCY   \VCXO_pll/Mcount_ref_clk_count_xor<7>  (
    .CI(\VCXO_pll/Mcount_ref_clk_count_cy [6]),
    .LI(\VCXO_pll/Mcount_ref_clk_count_cy<7>_rt_2066 ),
    .O(\VCXO_pll/Result [7])
  );
  MUXCY   \VCXO_pll/Mcount_ref_clk_count_cy<7>  (
    .CI(\VCXO_pll/Mcount_ref_clk_count_cy [6]),
    .DI(tp2_o_OBUF_3479),
    .S(\VCXO_pll/Mcount_ref_clk_count_cy<7>_rt_2066 ),
    .O(\VCXO_pll/Mcount_ref_clk_count_cy [7])
  );
  XORCY   \VCXO_pll/Mcount_ref_clk_count_xor<6>  (
    .CI(\VCXO_pll/Mcount_ref_clk_count_cy [5]),
    .LI(\VCXO_pll/Mcount_ref_clk_count_cy<6>_rt_2064 ),
    .O(\VCXO_pll/Result [6])
  );
  MUXCY   \VCXO_pll/Mcount_ref_clk_count_cy<6>  (
    .CI(\VCXO_pll/Mcount_ref_clk_count_cy [5]),
    .DI(tp2_o_OBUF_3479),
    .S(\VCXO_pll/Mcount_ref_clk_count_cy<6>_rt_2064 ),
    .O(\VCXO_pll/Mcount_ref_clk_count_cy [6])
  );
  XORCY   \VCXO_pll/Mcount_ref_clk_count_xor<5>  (
    .CI(\VCXO_pll/Mcount_ref_clk_count_cy [4]),
    .LI(\VCXO_pll/Mcount_ref_clk_count_cy<5>_rt_2062 ),
    .O(\VCXO_pll/Result [5])
  );
  MUXCY   \VCXO_pll/Mcount_ref_clk_count_cy<5>  (
    .CI(\VCXO_pll/Mcount_ref_clk_count_cy [4]),
    .DI(tp2_o_OBUF_3479),
    .S(\VCXO_pll/Mcount_ref_clk_count_cy<5>_rt_2062 ),
    .O(\VCXO_pll/Mcount_ref_clk_count_cy [5])
  );
  XORCY   \VCXO_pll/Mcount_ref_clk_count_xor<4>  (
    .CI(\VCXO_pll/Mcount_ref_clk_count_cy [3]),
    .LI(\VCXO_pll/Mcount_ref_clk_count_cy<4>_rt_2060 ),
    .O(\VCXO_pll/Result [4])
  );
  MUXCY   \VCXO_pll/Mcount_ref_clk_count_cy<4>  (
    .CI(\VCXO_pll/Mcount_ref_clk_count_cy [3]),
    .DI(tp2_o_OBUF_3479),
    .S(\VCXO_pll/Mcount_ref_clk_count_cy<4>_rt_2060 ),
    .O(\VCXO_pll/Mcount_ref_clk_count_cy [4])
  );
  XORCY   \VCXO_pll/Mcount_ref_clk_count_xor<3>  (
    .CI(\VCXO_pll/Mcount_ref_clk_count_cy [2]),
    .LI(\VCXO_pll/Mcount_ref_clk_count_cy<3>_rt_2058 ),
    .O(\VCXO_pll/Result [3])
  );
  MUXCY   \VCXO_pll/Mcount_ref_clk_count_cy<3>  (
    .CI(\VCXO_pll/Mcount_ref_clk_count_cy [2]),
    .DI(tp2_o_OBUF_3479),
    .S(\VCXO_pll/Mcount_ref_clk_count_cy<3>_rt_2058 ),
    .O(\VCXO_pll/Mcount_ref_clk_count_cy [3])
  );
  XORCY   \VCXO_pll/Mcount_ref_clk_count_xor<2>  (
    .CI(\VCXO_pll/Mcount_ref_clk_count_cy [1]),
    .LI(\VCXO_pll/Mcount_ref_clk_count_cy<2>_rt_2056 ),
    .O(\VCXO_pll/Result [2])
  );
  MUXCY   \VCXO_pll/Mcount_ref_clk_count_cy<2>  (
    .CI(\VCXO_pll/Mcount_ref_clk_count_cy [1]),
    .DI(tp2_o_OBUF_3479),
    .S(\VCXO_pll/Mcount_ref_clk_count_cy<2>_rt_2056 ),
    .O(\VCXO_pll/Mcount_ref_clk_count_cy [2])
  );
  XORCY   \VCXO_pll/Mcount_ref_clk_count_xor<1>  (
    .CI(\VCXO_pll/Mcount_ref_clk_count_cy [0]),
    .LI(\VCXO_pll/Mcount_ref_clk_count_cy<1>_rt_2054 ),
    .O(\VCXO_pll/Result [1])
  );
  MUXCY   \VCXO_pll/Mcount_ref_clk_count_cy<1>  (
    .CI(\VCXO_pll/Mcount_ref_clk_count_cy [0]),
    .DI(tp2_o_OBUF_3479),
    .S(\VCXO_pll/Mcount_ref_clk_count_cy<1>_rt_2054 ),
    .O(\VCXO_pll/Mcount_ref_clk_count_cy [1])
  );
  XORCY   \VCXO_pll/Mcount_ref_clk_count_xor<0>  (
    .CI(tp2_o_OBUF_3479),
    .LI(\VCXO_pll/Mcount_ref_clk_count_lut [0]),
    .O(\VCXO_pll/Result [0])
  );
  MUXCY   \VCXO_pll/Mcount_ref_clk_count_cy<0>  (
    .CI(tp2_o_OBUF_3479),
    .DI(N1),
    .S(\VCXO_pll/Mcount_ref_clk_count_lut [0]),
    .O(\VCXO_pll/Mcount_ref_clk_count_cy [0])
  );
  FDR #(
    .INIT ( 1'b0 ))
  \VCXO_pll/ref_clk_count_8  (
    .C(clk_100),
    .D(\VCXO_pll/Result [8]),
    .R(\VCXO_pll/ref_clk_count_cmp_eq0000 ),
    .Q(\VCXO_pll/ref_clk_count [8])
  );
  FDR #(
    .INIT ( 1'b0 ))
  \VCXO_pll/ref_clk_count_7  (
    .C(clk_100),
    .D(\VCXO_pll/Result [7]),
    .R(\VCXO_pll/ref_clk_count_cmp_eq0000 ),
    .Q(\VCXO_pll/ref_clk_count [7])
  );
  FDR #(
    .INIT ( 1'b0 ))
  \VCXO_pll/ref_clk_count_6  (
    .C(clk_100),
    .D(\VCXO_pll/Result [6]),
    .R(\VCXO_pll/ref_clk_count_cmp_eq0000 ),
    .Q(\VCXO_pll/ref_clk_count [6])
  );
  FDR #(
    .INIT ( 1'b0 ))
  \VCXO_pll/ref_clk_count_5  (
    .C(clk_100),
    .D(\VCXO_pll/Result [5]),
    .R(\VCXO_pll/ref_clk_count_cmp_eq0000 ),
    .Q(\VCXO_pll/ref_clk_count [5])
  );
  FDR #(
    .INIT ( 1'b0 ))
  \VCXO_pll/ref_clk_count_4  (
    .C(clk_100),
    .D(\VCXO_pll/Result [4]),
    .R(\VCXO_pll/ref_clk_count_cmp_eq0000 ),
    .Q(\VCXO_pll/ref_clk_count [4])
  );
  FDR #(
    .INIT ( 1'b0 ))
  \VCXO_pll/ref_clk_count_3  (
    .C(clk_100),
    .D(\VCXO_pll/Result [3]),
    .R(\VCXO_pll/ref_clk_count_cmp_eq0000 ),
    .Q(\VCXO_pll/ref_clk_count [3])
  );
  FDR #(
    .INIT ( 1'b0 ))
  \VCXO_pll/ref_clk_count_2  (
    .C(clk_100),
    .D(\VCXO_pll/Result [2]),
    .R(\VCXO_pll/ref_clk_count_cmp_eq0000 ),
    .Q(\VCXO_pll/ref_clk_count [2])
  );
  FDR #(
    .INIT ( 1'b0 ))
  \VCXO_pll/ref_clk_count_1  (
    .C(clk_100),
    .D(\VCXO_pll/Result [1]),
    .R(\VCXO_pll/ref_clk_count_cmp_eq0000 ),
    .Q(\VCXO_pll/ref_clk_count [1])
  );
  FDR #(
    .INIT ( 1'b0 ))
  \VCXO_pll/ref_clk_count_0  (
    .C(clk_100),
    .D(\VCXO_pll/Result [0]),
    .R(\VCXO_pll/ref_clk_count_cmp_eq0000 ),
    .Q(\VCXO_pll/ref_clk_count [0])
  );
  FD   \VCXO_pll/cv_word_o_15  (
    .C(clk_148),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum [17]),
    .Q(\VCXO_pll/cv_word_o [15])
  );
  FD   \VCXO_pll/cv_word_o_14  (
    .C(clk_148),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum [16]),
    .Q(\VCXO_pll/cv_word_o [14])
  );
  FD   \VCXO_pll/cv_word_o_13  (
    .C(clk_148),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum [15]),
    .Q(\VCXO_pll/cv_word_o [13])
  );
  FD   \VCXO_pll/cv_word_o_12  (
    .C(clk_148),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum [14]),
    .Q(\VCXO_pll/cv_word_o [12])
  );
  FD   \VCXO_pll/cv_word_o_11  (
    .C(clk_148),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum [13]),
    .Q(\VCXO_pll/cv_word_o [11])
  );
  FD   \VCXO_pll/cv_word_o_10  (
    .C(clk_148),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum [12]),
    .Q(\VCXO_pll/cv_word_o [10])
  );
  FD   \VCXO_pll/cv_word_o_9  (
    .C(clk_148),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum [11]),
    .Q(\VCXO_pll/cv_word_o [9])
  );
  FD   \VCXO_pll/cv_word_o_8  (
    .C(clk_148),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum [10]),
    .Q(\VCXO_pll/cv_word_o [8])
  );
  FD   \VCXO_pll/cv_word_o_7  (
    .C(clk_148),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum [9]),
    .Q(\VCXO_pll/cv_word_o [7])
  );
  FD   \VCXO_pll/cv_word_o_6  (
    .C(clk_148),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum [8]),
    .Q(\VCXO_pll/cv_word_o [6])
  );
  FD   \VCXO_pll/cv_word_o_5  (
    .C(clk_148),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum [7]),
    .Q(\VCXO_pll/cv_word_o [5])
  );
  FD   \VCXO_pll/cv_word_o_4  (
    .C(clk_148),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum [6]),
    .Q(\VCXO_pll/cv_word_o [4])
  );
  FD   \VCXO_pll/cv_word_o_3  (
    .C(clk_148),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum [5]),
    .Q(\VCXO_pll/cv_word_o [3])
  );
  FD   \VCXO_pll/cv_word_o_2  (
    .C(clk_148),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum [4]),
    .Q(\VCXO_pll/cv_word_o [2])
  );
  FD   \VCXO_pll/cv_word_o_1  (
    .C(clk_148),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum [3]),
    .Q(\VCXO_pll/cv_word_o [1])
  );
  FD   \VCXO_pll/cv_word_o_0  (
    .C(clk_148),
    .D(\VCXO_pll/VCXO_phase_loop/out_sum [2]),
    .Q(\VCXO_pll/cv_word_o [0])
  );
  FDR   \VCXO_pll/pps_148_tick_o  (
    .C(clk_148),
    .D(\VCXO_pll/phase_detector/update_tick_2288 ),
    .R(\VCXO_pll/resync_148_period_inv ),
    .Q(\VCXO_pll/pps_148_tick_o_2290 )
  );
  FD   \VCXO_pll/update_dac_o  (
    .C(clk_148),
    .D(\VCXO_pll/phase_detector/update_tick_2288 ),
    .Q(\VCXO_pll/update_dac_o_2309 )
  );
  FDR   \VCXO_pll/ref_clk  (
    .C(clk_100),
    .D(N1),
    .R(\VCXO_pll/ref_clk_not0001 ),
    .Q(\VCXO_pll/ref_clk_2291 )
  );
  FDSE   \VCXO_pll/resync_148_period  (
    .C(clk_100),
    .CE(\VCXO_pll/resync_148_period_and0000 ),
    .D(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_detect/int_pps_tick_1494 ),
    .Q(\VCXO_pll/resync_148_period_2306 )
  );
  FDE #(
    .INIT ( 1'b1 ))
  \LTC_B/LTC_biphase_generator/biphase_code  (
    .C(clk_148),
    .CE(\LTC_B/LTC_biphase_generator/biphase_code_not0002 ),
    .D(\LTC_B/LTC_biphase_generator/biphase_code_not0003 ),
    .Q(\LTC_B/LTC_biphase_generator/biphase_code_582 )
  );
  MUXF7   \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_f7  (
    .I0(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f6_565 ),
    .I1(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_f6_561 ),
    .S(\LTC_B/LTC_biphase_generator/Madd_ltc_frame_i_not0000 [1]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_f7_558 )
  );
  MUXF6   \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f6  (
    .I0(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_17_f5_575 ),
    .I1(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_f51 ),
    .S(\LTC_B/LTC_biphase_generator/Madd_ltc_frame_i_not0000 [0]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f6_565 )
  );
  MUXF5   \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_17_f5  (
    .I0(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_19_579 ),
    .I1(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_182_578 ),
    .S(\LTC_B/LTC_biphase_generator/ltc_frame_i_sub0000 [5]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_17_f5_575 )
  );
  LUT2 #(
    .INIT ( 4'hD ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_19  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_B/LTC_frame_count/hour_units_o [0]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_19_579 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_182  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_B/LTC_frame_count/min_units_o [0]),
    .I2(\LTC_B/LTC_frame_count/sec_units_o [0]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_182_578 )
  );
  MUXF5   \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_f5_0  (
    .I0(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_181_577 ),
    .I1(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_173_574 ),
    .S(\LTC_B/LTC_biphase_generator/ltc_frame_i_sub0000 [5]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_f51 )
  );
  LUT2 #(
    .INIT ( 4'hD ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_181  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_B/LTC_frame_count/hour_units_o [1]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_181_577 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_173  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_B/LTC_frame_count/min_units_o [1]),
    .I2(\LTC_B/LTC_frame_count/sec_units_o [1]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_173_574 )
  );
  MUXF6   \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_f6  (
    .I0(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_f5_569 ),
    .I1(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f51 ),
    .S(\LTC_B/LTC_biphase_generator/Madd_ltc_frame_i_not0000 [0]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_f6_561 )
  );
  MUXF5   \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_f5  (
    .I0(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_18_576 ),
    .I1(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_172_573 ),
    .S(\LTC_B/LTC_biphase_generator/ltc_frame_i_sub0000 [5]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_f5_569 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_18  (
    .I0(\LTC_B/LTC_frame_count/hour_units_o [2]),
    .I1(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_18_576 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_172  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_B/LTC_frame_count/min_units_o [2]),
    .I2(\LTC_B/LTC_frame_count/sec_units_o [2]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_172_573 )
  );
  MUXF5   \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f5_0  (
    .I0(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_171_572 ),
    .I1(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_162_568 ),
    .S(\LTC_B/LTC_biphase_generator/ltc_frame_i_sub0000 [5]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f51 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_171  (
    .I0(\LTC_B/LTC_frame_count/hour_units_o [3]),
    .I1(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_171_572 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_162  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_B/LTC_frame_count/min_units_o [3]),
    .I2(\LTC_B/LTC_frame_count/sec_units_o [3]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_162_568 )
  );
  MUXF6   \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_f6  (
    .I0(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f5_563 ),
    .I1(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_f5_560 ),
    .S(\LTC_B/LTC_biphase_generator/Madd_ltc_frame_i_not0000 [0]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_f6_557 )
  );
  MUXF5   \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f5  (
    .I0(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_17_571 ),
    .I1(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_161_567 ),
    .S(\LTC_B/LTC_biphase_generator/ltc_frame_i_sub0000 [5]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f5_563 )
  );
  LUT2 #(
    .INIT ( 4'hD ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_17  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_B/LTC_frame_count/hour_tens_o [0]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_17_571 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_161  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_B/LTC_frame_count/min_tens_o [1]),
    .I2(\LTC_B/LTC_frame_count/sec_tens_o [1]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_161_567 )
  );
  MUXF5   \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_f5  (
    .I0(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_566 ),
    .I1(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_562 ),
    .S(\LTC_B/LTC_biphase_generator/ltc_frame_i_sub0000 [5]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_f5_560 )
  );
  LUT2 #(
    .INIT ( 4'hD ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_B/LTC_frame_count/hour_tens_o [1]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_566 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_B/LTC_frame_count/min_tens_o [2]),
    .I2(\LTC_B/LTC_frame_count/sec_tens_o [2]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_562 )
  );
  FDE   \LTC_B/LTC_frame_count/frame_units_0  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/frame_units_mux0009 [0]),
    .Q(\LTC_B/LTC_frame_count/frame_units [0])
  );
  FDE   \LTC_B/LTC_frame_count/frame_units_1  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/frame_units_mux0009 [1]),
    .Q(\LTC_B/LTC_frame_count/frame_units [1])
  );
  FDE   \LTC_B/LTC_frame_count/frame_units_2  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/frame_units_mux0009 [2]),
    .Q(\LTC_B/LTC_frame_count/frame_units [2])
  );
  FDE   \LTC_B/LTC_frame_count/frame_units_3  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/frame_units_mux0009 [3]),
    .Q(\LTC_B/LTC_frame_count/frame_units [3])
  );
  FDE   \LTC_B/LTC_frame_count/frame_tens_0  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/frame_tens_mux0002 [4]),
    .Q(\LTC_B/LTC_frame_count/frame_tens [0])
  );
  FDE   \LTC_B/LTC_frame_count/frame_tens_1  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/frame_tens_mux0002 [5]),
    .Q(\LTC_B/LTC_frame_count/frame_tens [1])
  );
  FDE   \LTC_B/LTC_frame_count/hour_tens_0  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/hour_tens_not0002_784 ),
    .D(\LTC_B/LTC_frame_count/hour_tens_mux0000 [4]),
    .Q(\LTC_B/LTC_frame_count/hour_tens [0])
  );
  FDE   \LTC_B/LTC_frame_count/hour_tens_1  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/hour_tens_not0002_784 ),
    .D(\LTC_B/LTC_frame_count/hour_tens_mux0000 [5]),
    .Q(\LTC_B/LTC_frame_count/hour_tens [1])
  );
  FDRE   \LTC_B/LTC_frame_count/count_sec  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/count_sec_or0000 ),
    .D(N1),
    .R(\LTC_B/LTC_frame_count/ltc_frame_tick_i_inv ),
    .Q(\LTC_B/LTC_frame_count/count_sec_740 )
  );
  FDE   \LTC_B/LTC_frame_count/hour_tens_o_0  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/hour_tens [1]),
    .Q(\LTC_B/LTC_frame_count/hour_tens_o [0])
  );
  FDE   \LTC_B/LTC_frame_count/hour_tens_o_1  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/hour_tens [0]),
    .Q(\LTC_B/LTC_frame_count/hour_tens_o [1])
  );
  FDE   \LTC_B/LTC_frame_count/min_tens_o_0  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/min_tens [2]),
    .Q(\LTC_B/LTC_frame_count/min_tens_o [0])
  );
  FDE   \LTC_B/LTC_frame_count/min_tens_o_1  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/min_tens [1]),
    .Q(\LTC_B/LTC_frame_count/min_tens_o [1])
  );
  FDE   \LTC_B/LTC_frame_count/min_tens_o_2  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/min_tens [0]),
    .Q(\LTC_B/LTC_frame_count/min_tens_o [2])
  );
  FDE   \LTC_B/LTC_frame_count/frame_units_o_0  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/frame_units [3]),
    .Q(\LTC_B/LTC_frame_count/frame_units_o [0])
  );
  FDE   \LTC_B/LTC_frame_count/frame_units_o_1  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/frame_units [2]),
    .Q(\LTC_B/LTC_frame_count/frame_units_o [1])
  );
  FDE   \LTC_B/LTC_frame_count/frame_units_o_2  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/frame_units [1]),
    .Q(\LTC_B/LTC_frame_count/frame_units_o [2])
  );
  FDE   \LTC_B/LTC_frame_count/frame_units_o_3  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/frame_units [0]),
    .Q(\LTC_B/LTC_frame_count/frame_units_o [3])
  );
  FDE   \LTC_B/LTC_frame_count/sec_units_o_0  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/sec_units [3]),
    .Q(\LTC_B/LTC_frame_count/sec_units_o [0])
  );
  FDE   \LTC_B/LTC_frame_count/sec_units_o_1  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/sec_units [2]),
    .Q(\LTC_B/LTC_frame_count/sec_units_o [1])
  );
  FDE   \LTC_B/LTC_frame_count/sec_units_o_2  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/sec_units [1]),
    .Q(\LTC_B/LTC_frame_count/sec_units_o [2])
  );
  FDE   \LTC_B/LTC_frame_count/sec_units_o_3  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/sec_units [0]),
    .Q(\LTC_B/LTC_frame_count/sec_units_o [3])
  );
  FDE   \LTC_B/LTC_frame_count/sec_tens_o_0  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/sec_tens [2]),
    .Q(\LTC_B/LTC_frame_count/sec_tens_o [0])
  );
  FDE   \LTC_B/LTC_frame_count/sec_tens_o_1  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/sec_tens [1]),
    .Q(\LTC_B/LTC_frame_count/sec_tens_o [1])
  );
  FDE   \LTC_B/LTC_frame_count/sec_tens_o_2  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/sec_tens [0]),
    .Q(\LTC_B/LTC_frame_count/sec_tens_o [2])
  );
  FDE   \LTC_B/LTC_frame_count/hour_units_o_0  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/hour_units [3]),
    .Q(\LTC_B/LTC_frame_count/hour_units_o [0])
  );
  FDE   \LTC_B/LTC_frame_count/hour_units_o_1  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/hour_units [2]),
    .Q(\LTC_B/LTC_frame_count/hour_units_o [1])
  );
  FDE   \LTC_B/LTC_frame_count/hour_units_o_2  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/hour_units [1]),
    .Q(\LTC_B/LTC_frame_count/hour_units_o [2])
  );
  FDE   \LTC_B/LTC_frame_count/hour_units_o_3  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/hour_units [0]),
    .Q(\LTC_B/LTC_frame_count/hour_units_o [3])
  );
  FDE   \LTC_B/LTC_frame_count/frame_tens_o_0  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/frame_tens [1]),
    .Q(\LTC_B/LTC_frame_count/frame_tens_o [0])
  );
  FDE   \LTC_B/LTC_frame_count/frame_tens_o_1  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/frame_tens [0]),
    .Q(\LTC_B/LTC_frame_count/frame_tens_o [1])
  );
  FDE   \LTC_B/LTC_frame_count/min_units_o_0  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/min_units [3]),
    .Q(\LTC_B/LTC_frame_count/min_units_o [0])
  );
  FDE   \LTC_B/LTC_frame_count/min_units_o_1  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/min_units [2]),
    .Q(\LTC_B/LTC_frame_count/min_units_o [1])
  );
  FDE   \LTC_B/LTC_frame_count/min_units_o_2  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/min_units [1]),
    .Q(\LTC_B/LTC_frame_count/min_units_o [2])
  );
  FDE   \LTC_B/LTC_frame_count/min_units_o_3  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .D(\LTC_B/LTC_frame_count/min_units [0]),
    .Q(\LTC_B/LTC_frame_count/min_units_o [3])
  );
  FDRE   \LTC_B/LTC_frame_count/sec_tens_0  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/sec_tens_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_sec_tens ),
    .R(\LTC_B/LTC_frame_count/sec_tens_and0000 ),
    .Q(\LTC_B/LTC_frame_count/sec_tens [0])
  );
  FDRE   \LTC_B/LTC_frame_count/sec_tens_1  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/sec_tens_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_sec_tens1 ),
    .R(\LTC_B/LTC_frame_count/sec_tens_and0000 ),
    .Q(\LTC_B/LTC_frame_count/sec_tens [1])
  );
  FDRE   \LTC_B/LTC_frame_count/sec_tens_2  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/sec_tens_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_sec_tens2 ),
    .R(\LTC_B/LTC_frame_count/sec_tens_and0000 ),
    .Q(\LTC_B/LTC_frame_count/sec_tens [2])
  );
  FDRE   \LTC_B/LTC_frame_count/min_units_0  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/min_units_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_min_units ),
    .R(\LTC_B/LTC_frame_count/min_units_and0000 ),
    .Q(\LTC_B/LTC_frame_count/min_units [0])
  );
  FDRE   \LTC_B/LTC_frame_count/min_units_1  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/min_units_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_min_units1 ),
    .R(\LTC_B/LTC_frame_count/min_units_and0000 ),
    .Q(\LTC_B/LTC_frame_count/min_units [1])
  );
  FDRE   \LTC_B/LTC_frame_count/min_units_2  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/min_units_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_min_units2 ),
    .R(\LTC_B/LTC_frame_count/min_units_and0000 ),
    .Q(\LTC_B/LTC_frame_count/min_units [2])
  );
  FDRE   \LTC_B/LTC_frame_count/min_units_3  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/min_units_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_min_units3 ),
    .R(\LTC_B/LTC_frame_count/min_units_and0000 ),
    .Q(\LTC_B/LTC_frame_count/min_units [3])
  );
  FDRE   \LTC_B/LTC_frame_count/sec_units_0  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/sec_units_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_sec_units ),
    .R(\LTC_B/LTC_frame_count/sec_units_and0000 ),
    .Q(\LTC_B/LTC_frame_count/sec_units [0])
  );
  FDRE   \LTC_B/LTC_frame_count/sec_units_1  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/sec_units_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_sec_units1 ),
    .R(\LTC_B/LTC_frame_count/sec_units_and0000 ),
    .Q(\LTC_B/LTC_frame_count/sec_units [1])
  );
  FDRE   \LTC_B/LTC_frame_count/sec_units_2  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/sec_units_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_sec_units2 ),
    .R(\LTC_B/LTC_frame_count/sec_units_and0000 ),
    .Q(\LTC_B/LTC_frame_count/sec_units [2])
  );
  FDRE   \LTC_B/LTC_frame_count/sec_units_3  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/sec_units_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_sec_units3 ),
    .R(\LTC_B/LTC_frame_count/sec_units_and0000 ),
    .Q(\LTC_B/LTC_frame_count/sec_units [3])
  );
  FDRE   \LTC_B/LTC_frame_count/min_tens_0  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/min_tens_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_min_tens ),
    .R(\LTC_B/LTC_frame_count/min_tens_and0000 ),
    .Q(\LTC_B/LTC_frame_count/min_tens [0])
  );
  FDRE   \LTC_B/LTC_frame_count/min_tens_1  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/min_tens_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_min_tens1 ),
    .R(\LTC_B/LTC_frame_count/min_tens_and0000 ),
    .Q(\LTC_B/LTC_frame_count/min_tens [1])
  );
  FDRE   \LTC_B/LTC_frame_count/min_tens_2  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/min_tens_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_min_tens2 ),
    .R(\LTC_B/LTC_frame_count/min_tens_and0000 ),
    .Q(\LTC_B/LTC_frame_count/min_tens [2])
  );
  FDRE   \LTC_B/LTC_frame_count/hour_units_0  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/hour_units_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_hour_units ),
    .R(\LTC_B/LTC_frame_count/hour_units_and0000 ),
    .Q(\LTC_B/LTC_frame_count/hour_units [0])
  );
  FDRE   \LTC_B/LTC_frame_count/hour_units_1  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/hour_units_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_hour_units1 ),
    .R(\LTC_B/LTC_frame_count/hour_units_and0000 ),
    .Q(\LTC_B/LTC_frame_count/hour_units [1])
  );
  FDRE   \LTC_B/LTC_frame_count/hour_units_2  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/hour_units_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_hour_units2 ),
    .R(\LTC_B/LTC_frame_count/hour_units_and0000 ),
    .Q(\LTC_B/LTC_frame_count/hour_units [2])
  );
  FDRE   \LTC_B/LTC_frame_count/hour_units_3  (
    .C(clk_148),
    .CE(\LTC_B/LTC_frame_count/hour_units_not0001 ),
    .D(\LTC_B/LTC_frame_count/Mcount_hour_units3 ),
    .R(\LTC_B/LTC_frame_count/hour_units_and0000 ),
    .Q(\LTC_B/LTC_frame_count/hour_units [3])
  );
  FD #(
    .INIT ( 1'b0 ))
  \LTC_B/LTC_clockgenerator/period_count_0  (
    .C(clk_148),
    .D(\LTC_B/LTC_clockgenerator/period_count_mux0003[15] ),
    .Q(\LTC_B/LTC_clockgenerator/period_count [0])
  );
  FD #(
    .INIT ( 1'b0 ))
  \LTC_B/LTC_clockgenerator/period_count_1  (
    .C(clk_148),
    .D(\LTC_B/LTC_clockgenerator/period_count_mux0003[14] ),
    .Q(\LTC_B/LTC_clockgenerator/period_count [1])
  );
  FD #(
    .INIT ( 1'b0 ))
  \LTC_B/LTC_clockgenerator/period_count_2  (
    .C(clk_148),
    .D(\LTC_B/LTC_clockgenerator/period_count_mux0003[13] ),
    .Q(\LTC_B/LTC_clockgenerator/period_count [2])
  );
  FD #(
    .INIT ( 1'b0 ))
  \LTC_B/LTC_clockgenerator/period_count_5  (
    .C(clk_148),
    .D(\LTC_B/LTC_clockgenerator/period_count_mux0003[10] ),
    .Q(\LTC_B/LTC_clockgenerator/period_count [5])
  );
  FD #(
    .INIT ( 1'b0 ))
  \LTC_B/LTC_clockgenerator/period_count_8  (
    .C(clk_148),
    .D(\LTC_B/LTC_clockgenerator/period_count_mux0003[7] ),
    .Q(\LTC_B/LTC_clockgenerator/period_count [8])
  );
  FD #(
    .INIT ( 1'b0 ))
  \LTC_B/LTC_clockgenerator/period_count_9  (
    .C(clk_148),
    .D(\LTC_B/LTC_clockgenerator/period_count_mux0003[6] ),
    .Q(\LTC_B/LTC_clockgenerator/period_count [9])
  );
  FD #(
    .INIT ( 1'b0 ))
  \LTC_B/LTC_clockgenerator/period_count_10  (
    .C(clk_148),
    .D(\LTC_B/LTC_clockgenerator/period_count_mux0003[5] ),
    .Q(\LTC_B/LTC_clockgenerator/period_count [10])
  );
  FD #(
    .INIT ( 1'b0 ))
  \LTC_B/LTC_clockgenerator/period_count_15  (
    .C(clk_148),
    .D(\LTC_B/LTC_clockgenerator/period_count_mux0003[0] ),
    .Q(\LTC_B/LTC_clockgenerator/period_count [15])
  );
  FDS   \LTC_B/LTC_clockgenerator/ltc_clock_tick_o  (
    .C(clk_148),
    .D(tp2_o_OBUF_3479),
    .S(\LTC_B/LTC_clockgenerator/ltc_clock_tick_o_or0000 ),
    .Q(\LTC_B/LTC_clockgenerator/ltc_clock_tick_o_661 )
  );
  FDR   \LTC_B/LTC_clockgenerator/ltc_sync_o  (
    .C(clk_148),
    .D(N1),
    .R(\LTC_B/LTC_clockgenerator/sync_i_inv ),
    .Q(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 )
  );
  FDSE   \LTC_B/LTC_clockgenerator/ltc_frame_tick_o  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_not0001 ),
    .D(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .S(\LTC_B/delay_sync_983 ),
    .Q(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 )
  );
  FDRE   \LTC_B/LTC_clockgenerator/ltc_bitcount_0  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .D(\LTC_B/LTC_clockgenerator/Result [0]),
    .R(\LTC_B/LTC_clockgenerator/ltc_bitcount_or0000 ),
    .Q(\LTC_B/LTC_clockgenerator/ltc_bitcount [0])
  );
  FDRE   \LTC_B/LTC_clockgenerator/ltc_bitcount_1  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .D(\LTC_B/LTC_clockgenerator/Result [1]),
    .R(\LTC_B/LTC_clockgenerator/ltc_bitcount_or0000 ),
    .Q(\LTC_B/LTC_clockgenerator/ltc_bitcount [1])
  );
  FDRE   \LTC_B/LTC_clockgenerator/ltc_bitcount_2  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .D(\LTC_B/LTC_clockgenerator/Result [2]),
    .R(\LTC_B/LTC_clockgenerator/ltc_bitcount_or0000 ),
    .Q(\LTC_B/LTC_clockgenerator/ltc_bitcount [2])
  );
  FDRE   \LTC_B/LTC_clockgenerator/ltc_bitcount_3  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .D(\LTC_B/LTC_clockgenerator/Result [3]),
    .R(\LTC_B/LTC_clockgenerator/ltc_bitcount_or0000 ),
    .Q(\LTC_B/LTC_clockgenerator/ltc_bitcount [3])
  );
  FDRE   \LTC_B/LTC_clockgenerator/ltc_bitcount_4  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .D(\LTC_B/LTC_clockgenerator/Result [4]),
    .R(\LTC_B/LTC_clockgenerator/ltc_bitcount_or0000 ),
    .Q(\LTC_B/LTC_clockgenerator/ltc_bitcount [4])
  );
  FDRE   \LTC_B/LTC_clockgenerator/ltc_bitcount_5  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .D(\LTC_B/LTC_clockgenerator/Result [5]),
    .R(\LTC_B/LTC_clockgenerator/ltc_bitcount_or0000 ),
    .Q(\LTC_B/LTC_clockgenerator/ltc_bitcount [5])
  );
  FDRE   \LTC_B/LTC_clockgenerator/ltc_bitcount_6  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .D(\LTC_B/LTC_clockgenerator/Result [6]),
    .R(\LTC_B/LTC_clockgenerator/ltc_bitcount_or0000 ),
    .Q(\LTC_B/LTC_clockgenerator/ltc_bitcount [6])
  );
  FDRE   \LTC_B/LTC_clockgenerator/ltc_bitcount_7  (
    .C(clk_148),
    .CE(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .D(\LTC_B/LTC_clockgenerator/Result [7]),
    .R(\LTC_B/LTC_clockgenerator/ltc_bitcount_or0000 ),
    .Q(\LTC_B/LTC_clockgenerator/ltc_bitcount [7])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<0>_rt_606 ),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [0])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_xor<0>  (
    .CI(N1),
    .LI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<0>_rt_606 ),
    .O(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [0])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<1>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [0]),
    .DI(N1),
    .S(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [1]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [1])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_xor<1>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [0]),
    .LI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [1]),
    .O(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [1])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<2>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [1]),
    .DI(N1),
    .S(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [2]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [2])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_xor<2>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [1]),
    .LI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [2]),
    .O(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [2])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<3>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [2]),
    .DI(N1),
    .S(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [3]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [3])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_xor<3>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [2]),
    .LI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [3]),
    .O(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [3])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<4>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [3]),
    .DI(N1),
    .S(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [4]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [4])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_xor<4>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [3]),
    .LI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [4]),
    .O(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [4])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<5>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [4]),
    .DI(N1),
    .S(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [5]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [5])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_xor<5>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [4]),
    .LI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [5]),
    .O(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [5])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<6>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [5]),
    .DI(N1),
    .S(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [6]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [6])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_xor<6>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [5]),
    .LI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [6]),
    .O(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [6])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<7>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [6]),
    .DI(N1),
    .S(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [7]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [7])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_xor<7>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [6]),
    .LI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [7]),
    .O(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [7])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<8>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [7]),
    .DI(N1),
    .S(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [8]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [8])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_xor<8>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [7]),
    .LI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [8]),
    .O(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [8])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<9>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [8]),
    .DI(N1),
    .S(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [9]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [9])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_xor<9>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [8]),
    .LI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [9]),
    .O(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [9])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<10>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [9]),
    .DI(N1),
    .S(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [10]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [10])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_xor<10>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [9]),
    .LI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [10]),
    .O(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [10])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<11>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [10]),
    .DI(N1),
    .S(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [11]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [11])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_xor<11>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [10]),
    .LI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [11]),
    .O(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [11])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<12>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [11]),
    .DI(N1),
    .S(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [12]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [12])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_xor<12>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [11]),
    .LI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [12]),
    .O(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [12])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<13>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [12]),
    .DI(N1),
    .S(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [13]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [13])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_xor<13>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [12]),
    .LI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [13]),
    .O(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [13])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<14>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [13]),
    .DI(N1),
    .S(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [14]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [14])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_xor<14>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [13]),
    .LI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [14]),
    .O(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [14])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_xor<15>  (
    .CI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy [14]),
    .LI(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [15]),
    .O(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [15])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<0>  (
    .CI(tp2_o_OBUF_3479),
    .DI(N1),
    .S(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_lut [0]),
    .O(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [0])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_xor<0>  (
    .CI(tp2_o_OBUF_3479),
    .LI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_lut [0]),
    .O(\LTC_B/LTC_clockgenerator/Result [0])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<1>  (
    .CI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [0]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<1>_rt_592 ),
    .O(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [1])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_xor<1>  (
    .CI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [0]),
    .LI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<1>_rt_592 ),
    .O(\LTC_B/LTC_clockgenerator/Result [1])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<2>  (
    .CI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [1]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<2>_rt_594 ),
    .O(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [2])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_xor<2>  (
    .CI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [1]),
    .LI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<2>_rt_594 ),
    .O(\LTC_B/LTC_clockgenerator/Result [2])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<3>  (
    .CI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [2]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<3>_rt_596 ),
    .O(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [3])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_xor<3>  (
    .CI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [2]),
    .LI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<3>_rt_596 ),
    .O(\LTC_B/LTC_clockgenerator/Result [3])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<4>  (
    .CI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [3]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<4>_rt_598 ),
    .O(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [4])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_xor<4>  (
    .CI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [3]),
    .LI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<4>_rt_598 ),
    .O(\LTC_B/LTC_clockgenerator/Result [4])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<5>  (
    .CI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [4]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<5>_rt_600 ),
    .O(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [5])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_xor<5>  (
    .CI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [4]),
    .LI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<5>_rt_600 ),
    .O(\LTC_B/LTC_clockgenerator/Result [5])
  );
  MUXCY   \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<6>  (
    .CI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [5]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<6>_rt_602 ),
    .O(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [6])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_xor<6>  (
    .CI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [5]),
    .LI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<6>_rt_602 ),
    .O(\LTC_B/LTC_clockgenerator/Result [6])
  );
  XORCY   \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_xor<7>  (
    .CI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy [6]),
    .LI(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_xor<7>_rt_604 ),
    .O(\LTC_B/LTC_clockgenerator/Result [7])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<23>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [22]),
    .LI(\LTC_B/Mcount_delay_count_low_xor<23>_rt_913 ),
    .O(\LTC_B/Result [23])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<22>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [21]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<22>_rt_895 ),
    .O(\LTC_B/Result [22])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<22>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [21]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<22>_rt_895 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [22])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<21>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [20]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<21>_rt_893 ),
    .O(\LTC_B/Result [21])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<21>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [20]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<21>_rt_893 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [21])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<20>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [19]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<20>_rt_891 ),
    .O(\LTC_B/Result [20])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<20>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [19]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<20>_rt_891 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [20])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<19>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [18]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<19>_rt_887 ),
    .O(\LTC_B/Result [19])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<19>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [18]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<19>_rt_887 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [19])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<18>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [17]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<18>_rt_885 ),
    .O(\LTC_B/Result [18])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<18>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [17]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<18>_rt_885 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [18])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<17>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [16]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<17>_rt_883 ),
    .O(\LTC_B/Result [17])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<17>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [16]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<17>_rt_883 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [17])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<16>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [15]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<16>_rt_881 ),
    .O(\LTC_B/Result [16])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<16>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [15]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<16>_rt_881 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [16])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<15>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [14]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<15>_rt_879 ),
    .O(\LTC_B/Result [15])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<15>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [14]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<15>_rt_879 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [15])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<14>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [13]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<14>_rt_877 ),
    .O(\LTC_B/Result [14])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<14>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [13]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<14>_rt_877 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [14])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<13>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [12]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<13>_rt_875 ),
    .O(\LTC_B/Result [13])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<13>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [12]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<13>_rt_875 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [13])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<12>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [11]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<12>_rt_873 ),
    .O(\LTC_B/Result [12])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<12>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [11]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<12>_rt_873 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [12])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<11>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [10]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<11>_rt_871 ),
    .O(\LTC_B/Result [11])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<11>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [10]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<11>_rt_871 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [11])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<10>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [9]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<10>_rt_869 ),
    .O(\LTC_B/Result [10])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<10>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [9]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<10>_rt_869 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [10])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<9>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [8]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<9>_rt_911 ),
    .O(\LTC_B/Result [9])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<9>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [8]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<9>_rt_911 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [9])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<8>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [7]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<8>_rt_909 ),
    .O(\LTC_B/Result [8])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<8>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [7]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<8>_rt_909 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [8])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<7>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [6]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<7>_rt_907 ),
    .O(\LTC_B/Result [7])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<7>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [6]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<7>_rt_907 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [7])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<6>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [5]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<6>_rt_905 ),
    .O(\LTC_B/Result [6])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<6>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [5]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<6>_rt_905 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [6])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<5>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [4]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<5>_rt_903 ),
    .O(\LTC_B/Result [5])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<5>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [4]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<5>_rt_903 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [5])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<4>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [3]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<4>_rt_901 ),
    .O(\LTC_B/Result [4])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<4>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [3]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<4>_rt_901 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [4])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<3>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [2]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<3>_rt_899 ),
    .O(\LTC_B/Result [3])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<3>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [2]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<3>_rt_899 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [3])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<2>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [1]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<2>_rt_897 ),
    .O(\LTC_B/Result [2])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<2>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [1]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<2>_rt_897 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [2])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<1>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [0]),
    .LI(\LTC_B/Mcount_delay_count_low_cy<1>_rt_889 ),
    .O(\LTC_B/Result [1])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<1>  (
    .CI(\LTC_B/Mcount_delay_count_low_cy [0]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcount_delay_count_low_cy<1>_rt_889 ),
    .O(\LTC_B/Mcount_delay_count_low_cy [1])
  );
  XORCY   \LTC_B/Mcount_delay_count_low_xor<0>  (
    .CI(tp2_o_OBUF_3479),
    .LI(\LTC_B/Mcount_delay_count_low_lut [0]),
    .O(\LTC_B/Result [0])
  );
  MUXCY   \LTC_B/Mcount_delay_count_low_cy<0>  (
    .CI(tp2_o_OBUF_3479),
    .DI(N1),
    .S(\LTC_B/Mcount_delay_count_low_lut [0]),
    .O(\LTC_B/Mcount_delay_count_low_cy [0])
  );
  MUXCY   \LTC_B/Mcompar_delay_sync_cmp_eq0000_cy<15>  (
    .CI(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [14]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [15]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [15])
  );
  LUT3 #(
    .INIT ( 8'h81 ))
  \LTC_B/Mcompar_delay_sync_cmp_eq0000_lut<15>  (
    .I0(tp2_o_OBUF_3479),
    .I1(LTC_b_delay[30]),
    .I2(LTC_b_delay[31]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [15])
  );
  MUXCY   \LTC_B/Mcompar_delay_sync_cmp_eq0000_cy<14>  (
    .CI(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [13]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [14]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [14])
  );
  LUT3 #(
    .INIT ( 8'h81 ))
  \LTC_B/Mcompar_delay_sync_cmp_eq0000_lut<14>  (
    .I0(tp2_o_OBUF_3479),
    .I1(LTC_b_delay[28]),
    .I2(LTC_b_delay[29]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [14])
  );
  MUXCY   \LTC_B/Mcompar_delay_sync_cmp_eq0000_cy<13>  (
    .CI(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [12]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [13]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [13])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_B/Mcompar_delay_sync_cmp_eq0000_lut<13>  (
    .I0(\LTC_B/delay_count_high [2]),
    .I1(LTC_b_delay[26]),
    .I2(\LTC_B/delay_count_high [3]),
    .I3(LTC_b_delay[27]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [13])
  );
  MUXCY   \LTC_B/Mcompar_delay_sync_cmp_eq0000_cy<12>  (
    .CI(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [11]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [12]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [12])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_B/Mcompar_delay_sync_cmp_eq0000_lut<12>  (
    .I0(\LTC_B/delay_count_high [0]),
    .I1(LTC_b_delay[24]),
    .I2(\LTC_B/delay_count_high [1]),
    .I3(LTC_b_delay[25]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [12])
  );
  MUXCY   \LTC_B/Mcompar_delay_sync_cmp_eq0000_cy<11>  (
    .CI(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [10]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [11]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [11])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_B/Mcompar_delay_sync_cmp_eq0000_lut<11>  (
    .I0(\LTC_B/delay_count_low [22]),
    .I1(LTC_b_delay[22]),
    .I2(\LTC_B/delay_count_low [23]),
    .I3(LTC_b_delay[23]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [11])
  );
  MUXCY   \LTC_B/Mcompar_delay_sync_cmp_eq0000_cy<10>  (
    .CI(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [9]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [10]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [10])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_B/Mcompar_delay_sync_cmp_eq0000_lut<10>  (
    .I0(\LTC_B/delay_count_low [20]),
    .I1(LTC_b_delay[20]),
    .I2(\LTC_B/delay_count_low [21]),
    .I3(LTC_b_delay[21]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [10])
  );
  MUXCY   \LTC_B/Mcompar_delay_sync_cmp_eq0000_cy<9>  (
    .CI(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [8]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [9]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [9])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_B/Mcompar_delay_sync_cmp_eq0000_lut<9>  (
    .I0(\LTC_B/delay_count_low [18]),
    .I1(LTC_b_delay[18]),
    .I2(\LTC_B/delay_count_low [19]),
    .I3(LTC_b_delay[19]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [9])
  );
  MUXCY   \LTC_B/Mcompar_delay_sync_cmp_eq0000_cy<8>  (
    .CI(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [7]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [8]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [8])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_B/Mcompar_delay_sync_cmp_eq0000_lut<8>  (
    .I0(\LTC_B/delay_count_low [16]),
    .I1(LTC_b_delay[16]),
    .I2(\LTC_B/delay_count_low [17]),
    .I3(LTC_b_delay[17]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [8])
  );
  MUXCY   \LTC_B/Mcompar_delay_sync_cmp_eq0000_cy<7>  (
    .CI(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [6]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [7]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [7])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_B/Mcompar_delay_sync_cmp_eq0000_lut<7>  (
    .I0(\LTC_B/delay_count_low [14]),
    .I1(LTC_b_delay[14]),
    .I2(\LTC_B/delay_count_low [15]),
    .I3(LTC_b_delay[15]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [7])
  );
  MUXCY   \LTC_B/Mcompar_delay_sync_cmp_eq0000_cy<6>  (
    .CI(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [5]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [6]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [6])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_B/Mcompar_delay_sync_cmp_eq0000_lut<6>  (
    .I0(\LTC_B/delay_count_low [12]),
    .I1(LTC_b_delay[12]),
    .I2(\LTC_B/delay_count_low [13]),
    .I3(LTC_b_delay[13]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [6])
  );
  MUXCY   \LTC_B/Mcompar_delay_sync_cmp_eq0000_cy<5>  (
    .CI(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [4]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [5]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [5])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_B/Mcompar_delay_sync_cmp_eq0000_lut<5>  (
    .I0(\LTC_B/delay_count_low [10]),
    .I1(LTC_b_delay[10]),
    .I2(\LTC_B/delay_count_low [11]),
    .I3(LTC_b_delay[11]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [5])
  );
  MUXCY   \LTC_B/Mcompar_delay_sync_cmp_eq0000_cy<4>  (
    .CI(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [3]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [4]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [4])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_B/Mcompar_delay_sync_cmp_eq0000_lut<4>  (
    .I0(\LTC_B/delay_count_low [8]),
    .I1(LTC_b_delay[8]),
    .I2(\LTC_B/delay_count_low [9]),
    .I3(LTC_b_delay[9]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [4])
  );
  MUXCY   \LTC_B/Mcompar_delay_sync_cmp_eq0000_cy<3>  (
    .CI(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [2]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [3]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [3])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_B/Mcompar_delay_sync_cmp_eq0000_lut<3>  (
    .I0(\LTC_B/delay_count_low [6]),
    .I1(LTC_b_delay[6]),
    .I2(\LTC_B/delay_count_low [7]),
    .I3(LTC_b_delay[7]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [3])
  );
  MUXCY   \LTC_B/Mcompar_delay_sync_cmp_eq0000_cy<2>  (
    .CI(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [1]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [2]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [2])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_B/Mcompar_delay_sync_cmp_eq0000_lut<2>  (
    .I0(\LTC_B/delay_count_low [4]),
    .I1(LTC_b_delay[4]),
    .I2(\LTC_B/delay_count_low [5]),
    .I3(LTC_b_delay[5]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [2])
  );
  MUXCY   \LTC_B/Mcompar_delay_sync_cmp_eq0000_cy<1>  (
    .CI(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [0]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [1]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [1])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_B/Mcompar_delay_sync_cmp_eq0000_lut<1>  (
    .I0(\LTC_B/delay_count_low [2]),
    .I1(LTC_b_delay[2]),
    .I2(\LTC_B/delay_count_low [3]),
    .I3(LTC_b_delay[3]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [1])
  );
  MUXCY   \LTC_B/Mcompar_delay_sync_cmp_eq0000_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [0]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [0])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_B/Mcompar_delay_sync_cmp_eq0000_lut<0>  (
    .I0(\LTC_B/delay_count_low [0]),
    .I1(LTC_b_delay[0]),
    .I2(\LTC_B/delay_count_low [1]),
    .I3(LTC_b_delay[1]),
    .O(\LTC_B/Mcompar_delay_sync_cmp_eq0000_lut [0])
  );
  FDRE   \LTC_B/delay_count_high_3  (
    .C(clk_148),
    .CE(\LTC_B/delay_count_high_cmp_eq0000 ),
    .D(\LTC_B/Result<3>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(\LTC_B/delay_count_high [3])
  );
  FDRE   \LTC_B/delay_count_high_2  (
    .C(clk_148),
    .CE(\LTC_B/delay_count_high_cmp_eq0000 ),
    .D(\LTC_B/Result<2>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(\LTC_B/delay_count_high [2])
  );
  FDRE   \LTC_B/delay_count_high_1  (
    .C(clk_148),
    .CE(\LTC_B/delay_count_high_cmp_eq0000 ),
    .D(\LTC_B/Result<1>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(\LTC_B/delay_count_high [1])
  );
  FDRE   \LTC_B/delay_count_high_0  (
    .C(clk_148),
    .CE(\LTC_B/delay_count_high_cmp_eq0000 ),
    .D(\LTC_B/Result<0>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(\LTC_B/delay_count_high [0])
  );
  FDR   \LTC_B/delay_count_low_23  (
    .C(clk_148),
    .D(\LTC_B/Result [23]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [23])
  );
  FDR   \LTC_B/delay_count_low_22  (
    .C(clk_148),
    .D(\LTC_B/Result [22]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [22])
  );
  FDR   \LTC_B/delay_count_low_21  (
    .C(clk_148),
    .D(\LTC_B/Result [21]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [21])
  );
  FDR   \LTC_B/delay_count_low_20  (
    .C(clk_148),
    .D(\LTC_B/Result [20]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [20])
  );
  FDR   \LTC_B/delay_count_low_19  (
    .C(clk_148),
    .D(\LTC_B/Result [19]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [19])
  );
  FDR   \LTC_B/delay_count_low_18  (
    .C(clk_148),
    .D(\LTC_B/Result [18]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [18])
  );
  FDR   \LTC_B/delay_count_low_17  (
    .C(clk_148),
    .D(\LTC_B/Result [17]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [17])
  );
  FDR   \LTC_B/delay_count_low_16  (
    .C(clk_148),
    .D(\LTC_B/Result [16]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [16])
  );
  FDR   \LTC_B/delay_count_low_15  (
    .C(clk_148),
    .D(\LTC_B/Result [15]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [15])
  );
  FDR   \LTC_B/delay_count_low_14  (
    .C(clk_148),
    .D(\LTC_B/Result [14]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [14])
  );
  FDR   \LTC_B/delay_count_low_13  (
    .C(clk_148),
    .D(\LTC_B/Result [13]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [13])
  );
  FDR   \LTC_B/delay_count_low_12  (
    .C(clk_148),
    .D(\LTC_B/Result [12]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [12])
  );
  FDR   \LTC_B/delay_count_low_11  (
    .C(clk_148),
    .D(\LTC_B/Result [11]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [11])
  );
  FDR   \LTC_B/delay_count_low_10  (
    .C(clk_148),
    .D(\LTC_B/Result [10]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [10])
  );
  FDR   \LTC_B/delay_count_low_9  (
    .C(clk_148),
    .D(\LTC_B/Result [9]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [9])
  );
  FDR   \LTC_B/delay_count_low_8  (
    .C(clk_148),
    .D(\LTC_B/Result [8]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [8])
  );
  FDR   \LTC_B/delay_count_low_7  (
    .C(clk_148),
    .D(\LTC_B/Result [7]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [7])
  );
  FDR   \LTC_B/delay_count_low_6  (
    .C(clk_148),
    .D(\LTC_B/Result [6]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [6])
  );
  FDR   \LTC_B/delay_count_low_5  (
    .C(clk_148),
    .D(\LTC_B/Result [5]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [5])
  );
  FDR   \LTC_B/delay_count_low_4  (
    .C(clk_148),
    .D(\LTC_B/Result [4]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [4])
  );
  FDR   \LTC_B/delay_count_low_3  (
    .C(clk_148),
    .D(\LTC_B/Result [3]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [3])
  );
  FDR   \LTC_B/delay_count_low_2  (
    .C(clk_148),
    .D(\LTC_B/Result [2]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [2])
  );
  FDR   \LTC_B/delay_count_low_1  (
    .C(clk_148),
    .D(\LTC_B/Result [1]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [1])
  );
  FDR   \LTC_B/delay_count_low_0  (
    .C(clk_148),
    .D(\LTC_B/Result [0]),
    .R(\LTC_B/delay_count_low_or0000 ),
    .Q(\LTC_B/delay_count_low [0])
  );
  FDR   \LTC_B/delay_sync  (
    .C(clk_148),
    .D(N1),
    .R(\LTC_B/delay_sync_not0001 ),
    .Q(\LTC_B/delay_sync_983 )
  );
  FDE #(
    .INIT ( 1'b1 ))
  \LTC_A/LTC_biphase_generator/biphase_code  (
    .C(clk_148),
    .CE(\LTC_A/LTC_biphase_generator/biphase_code_not0002 ),
    .D(\LTC_A/LTC_biphase_generator/biphase_code_not0003 ),
    .Q(\LTC_A/LTC_biphase_generator/biphase_code_148 )
  );
  MUXF7   \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_f7  (
    .I0(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f6_131 ),
    .I1(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_f6_127 ),
    .S(\LTC_A/LTC_biphase_generator/Madd_ltc_frame_i_not0000 [1]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_f7_124 )
  );
  MUXF6   \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f6  (
    .I0(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_17_f5_141 ),
    .I1(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_f51 ),
    .S(\LTC_A/LTC_biphase_generator/Madd_ltc_frame_i_not0000 [0]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f6_131 )
  );
  MUXF5   \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_17_f5  (
    .I0(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_19_145 ),
    .I1(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_182_144 ),
    .S(\LTC_A/LTC_biphase_generator/ltc_frame_i_sub0000 [5]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_17_f5_141 )
  );
  LUT2 #(
    .INIT ( 4'hD ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_19  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_A/LTC_frame_count/hour_units_o [0]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_19_145 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_182  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_A/LTC_frame_count/min_units_o [0]),
    .I2(\LTC_A/LTC_frame_count/sec_units_o [0]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_182_144 )
  );
  MUXF5   \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_f5_0  (
    .I0(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_181_143 ),
    .I1(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_173_140 ),
    .S(\LTC_A/LTC_biphase_generator/ltc_frame_i_sub0000 [5]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_f51 )
  );
  LUT2 #(
    .INIT ( 4'hD ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_181  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_A/LTC_frame_count/hour_units_o [1]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_181_143 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_173  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_A/LTC_frame_count/min_units_o [1]),
    .I2(\LTC_A/LTC_frame_count/sec_units_o [1]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_173_140 )
  );
  MUXF6   \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_f6  (
    .I0(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_f5_135 ),
    .I1(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f51 ),
    .S(\LTC_A/LTC_biphase_generator/Madd_ltc_frame_i_not0000 [0]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_f6_127 )
  );
  MUXF5   \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_f5  (
    .I0(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_18_142 ),
    .I1(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_172_139 ),
    .S(\LTC_A/LTC_biphase_generator/ltc_frame_i_sub0000 [5]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_f5_135 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_18  (
    .I0(\LTC_A/LTC_frame_count/hour_units_o [2]),
    .I1(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_18_142 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_172  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_A/LTC_frame_count/min_units_o [2]),
    .I2(\LTC_A/LTC_frame_count/sec_units_o [2]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_172_139 )
  );
  MUXF5   \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f5_0  (
    .I0(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_171_138 ),
    .I1(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_162_134 ),
    .S(\LTC_A/LTC_biphase_generator/ltc_frame_i_sub0000 [5]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f51 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_171  (
    .I0(\LTC_A/LTC_frame_count/hour_units_o [3]),
    .I1(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_171_138 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_162  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_A/LTC_frame_count/min_units_o [3]),
    .I2(\LTC_A/LTC_frame_count/sec_units_o [3]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_162_134 )
  );
  MUXF6   \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_f6  (
    .I0(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f5_129 ),
    .I1(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_f5_126 ),
    .S(\LTC_A/LTC_biphase_generator/Madd_ltc_frame_i_not0000 [0]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_f6_123 )
  );
  MUXF5   \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f5  (
    .I0(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_17_137 ),
    .I1(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_161_133 ),
    .S(\LTC_A/LTC_biphase_generator/ltc_frame_i_sub0000 [5]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_f5_129 )
  );
  LUT2 #(
    .INIT ( 4'hD ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_17  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_A/LTC_frame_count/hour_tens_o [0]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_17_137 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_161  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_A/LTC_frame_count/min_tens_o [1]),
    .I2(\LTC_A/LTC_frame_count/sec_tens_o [1]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_161_133 )
  );
  MUXF5   \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_f5  (
    .I0(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_132 ),
    .I1(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_128 ),
    .S(\LTC_A/LTC_biphase_generator/ltc_frame_i_sub0000 [5]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_f5_126 )
  );
  LUT2 #(
    .INIT ( 4'hD ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_A/LTC_frame_count/hour_tens_o [1]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_16_132 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_A/LTC_frame_count/min_tens_o [2]),
    .I2(\LTC_A/LTC_frame_count/sec_tens_o [2]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_15_128 )
  );
  FDE   \LTC_A/LTC_frame_count/frame_units_0  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/frame_units_mux0009 [0]),
    .Q(\LTC_A/LTC_frame_count/frame_units [0])
  );
  FDE   \LTC_A/LTC_frame_count/frame_units_1  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/frame_units_mux0009 [1]),
    .Q(\LTC_A/LTC_frame_count/frame_units [1])
  );
  FDE   \LTC_A/LTC_frame_count/frame_units_2  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/frame_units_mux0009 [2]),
    .Q(\LTC_A/LTC_frame_count/frame_units [2])
  );
  FDE   \LTC_A/LTC_frame_count/frame_units_3  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/frame_units_mux0009 [3]),
    .Q(\LTC_A/LTC_frame_count/frame_units [3])
  );
  FDE   \LTC_A/LTC_frame_count/frame_tens_0  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/frame_tens_mux0002 [4]),
    .Q(\LTC_A/LTC_frame_count/frame_tens [0])
  );
  FDE   \LTC_A/LTC_frame_count/frame_tens_1  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/frame_tens_mux0002 [5]),
    .Q(\LTC_A/LTC_frame_count/frame_tens [1])
  );
  FDE   \LTC_A/LTC_frame_count/hour_tens_0  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/hour_tens_not0002_350 ),
    .D(\LTC_A/LTC_frame_count/hour_tens_mux0000 [4]),
    .Q(\LTC_A/LTC_frame_count/hour_tens [0])
  );
  FDE   \LTC_A/LTC_frame_count/hour_tens_1  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/hour_tens_not0002_350 ),
    .D(\LTC_A/LTC_frame_count/hour_tens_mux0000 [5]),
    .Q(\LTC_A/LTC_frame_count/hour_tens [1])
  );
  FDRE   \LTC_A/LTC_frame_count/count_sec  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/count_sec_or0000 ),
    .D(N1),
    .R(\LTC_A/LTC_frame_count/ltc_frame_tick_i_inv ),
    .Q(\LTC_A/LTC_frame_count/count_sec_306 )
  );
  FDE   \LTC_A/LTC_frame_count/hour_tens_o_0  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/hour_tens [1]),
    .Q(\LTC_A/LTC_frame_count/hour_tens_o [0])
  );
  FDE   \LTC_A/LTC_frame_count/hour_tens_o_1  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/hour_tens [0]),
    .Q(\LTC_A/LTC_frame_count/hour_tens_o [1])
  );
  FDE   \LTC_A/LTC_frame_count/min_tens_o_0  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/min_tens [2]),
    .Q(\LTC_A/LTC_frame_count/min_tens_o [0])
  );
  FDE   \LTC_A/LTC_frame_count/min_tens_o_1  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/min_tens [1]),
    .Q(\LTC_A/LTC_frame_count/min_tens_o [1])
  );
  FDE   \LTC_A/LTC_frame_count/min_tens_o_2  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/min_tens [0]),
    .Q(\LTC_A/LTC_frame_count/min_tens_o [2])
  );
  FDE   \LTC_A/LTC_frame_count/frame_units_o_0  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/frame_units [3]),
    .Q(\LTC_A/LTC_frame_count/frame_units_o [0])
  );
  FDE   \LTC_A/LTC_frame_count/frame_units_o_1  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/frame_units [2]),
    .Q(\LTC_A/LTC_frame_count/frame_units_o [1])
  );
  FDE   \LTC_A/LTC_frame_count/frame_units_o_2  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/frame_units [1]),
    .Q(\LTC_A/LTC_frame_count/frame_units_o [2])
  );
  FDE   \LTC_A/LTC_frame_count/frame_units_o_3  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/frame_units [0]),
    .Q(\LTC_A/LTC_frame_count/frame_units_o [3])
  );
  FDE   \LTC_A/LTC_frame_count/sec_units_o_0  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/sec_units [3]),
    .Q(\LTC_A/LTC_frame_count/sec_units_o [0])
  );
  FDE   \LTC_A/LTC_frame_count/sec_units_o_1  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/sec_units [2]),
    .Q(\LTC_A/LTC_frame_count/sec_units_o [1])
  );
  FDE   \LTC_A/LTC_frame_count/sec_units_o_2  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/sec_units [1]),
    .Q(\LTC_A/LTC_frame_count/sec_units_o [2])
  );
  FDE   \LTC_A/LTC_frame_count/sec_units_o_3  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/sec_units [0]),
    .Q(\LTC_A/LTC_frame_count/sec_units_o [3])
  );
  FDE   \LTC_A/LTC_frame_count/sec_tens_o_0  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/sec_tens [2]),
    .Q(\LTC_A/LTC_frame_count/sec_tens_o [0])
  );
  FDE   \LTC_A/LTC_frame_count/sec_tens_o_1  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/sec_tens [1]),
    .Q(\LTC_A/LTC_frame_count/sec_tens_o [1])
  );
  FDE   \LTC_A/LTC_frame_count/sec_tens_o_2  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/sec_tens [0]),
    .Q(\LTC_A/LTC_frame_count/sec_tens_o [2])
  );
  FDE   \LTC_A/LTC_frame_count/hour_units_o_0  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/hour_units [3]),
    .Q(\LTC_A/LTC_frame_count/hour_units_o [0])
  );
  FDE   \LTC_A/LTC_frame_count/hour_units_o_1  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/hour_units [2]),
    .Q(\LTC_A/LTC_frame_count/hour_units_o [1])
  );
  FDE   \LTC_A/LTC_frame_count/hour_units_o_2  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/hour_units [1]),
    .Q(\LTC_A/LTC_frame_count/hour_units_o [2])
  );
  FDE   \LTC_A/LTC_frame_count/hour_units_o_3  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/hour_units [0]),
    .Q(\LTC_A/LTC_frame_count/hour_units_o [3])
  );
  FDE   \LTC_A/LTC_frame_count/frame_tens_o_0  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/frame_tens [1]),
    .Q(\LTC_A/LTC_frame_count/frame_tens_o [0])
  );
  FDE   \LTC_A/LTC_frame_count/frame_tens_o_1  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/frame_tens [0]),
    .Q(\LTC_A/LTC_frame_count/frame_tens_o [1])
  );
  FDE   \LTC_A/LTC_frame_count/min_units_o_0  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/min_units [3]),
    .Q(\LTC_A/LTC_frame_count/min_units_o [0])
  );
  FDE   \LTC_A/LTC_frame_count/min_units_o_1  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/min_units [2]),
    .Q(\LTC_A/LTC_frame_count/min_units_o [1])
  );
  FDE   \LTC_A/LTC_frame_count/min_units_o_2  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/min_units [1]),
    .Q(\LTC_A/LTC_frame_count/min_units_o [2])
  );
  FDE   \LTC_A/LTC_frame_count/min_units_o_3  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .D(\LTC_A/LTC_frame_count/min_units [0]),
    .Q(\LTC_A/LTC_frame_count/min_units_o [3])
  );
  FDRE   \LTC_A/LTC_frame_count/sec_tens_0  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/sec_tens_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_sec_tens ),
    .R(\LTC_A/LTC_frame_count/sec_tens_and0000 ),
    .Q(\LTC_A/LTC_frame_count/sec_tens [0])
  );
  FDRE   \LTC_A/LTC_frame_count/sec_tens_1  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/sec_tens_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_sec_tens1 ),
    .R(\LTC_A/LTC_frame_count/sec_tens_and0000 ),
    .Q(\LTC_A/LTC_frame_count/sec_tens [1])
  );
  FDRE   \LTC_A/LTC_frame_count/sec_tens_2  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/sec_tens_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_sec_tens2 ),
    .R(\LTC_A/LTC_frame_count/sec_tens_and0000 ),
    .Q(\LTC_A/LTC_frame_count/sec_tens [2])
  );
  FDRE   \LTC_A/LTC_frame_count/min_units_0  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/min_units_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_min_units ),
    .R(\LTC_A/LTC_frame_count/min_units_and0000 ),
    .Q(\LTC_A/LTC_frame_count/min_units [0])
  );
  FDRE   \LTC_A/LTC_frame_count/min_units_1  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/min_units_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_min_units1 ),
    .R(\LTC_A/LTC_frame_count/min_units_and0000 ),
    .Q(\LTC_A/LTC_frame_count/min_units [1])
  );
  FDRE   \LTC_A/LTC_frame_count/min_units_2  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/min_units_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_min_units2 ),
    .R(\LTC_A/LTC_frame_count/min_units_and0000 ),
    .Q(\LTC_A/LTC_frame_count/min_units [2])
  );
  FDRE   \LTC_A/LTC_frame_count/min_units_3  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/min_units_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_min_units3 ),
    .R(\LTC_A/LTC_frame_count/min_units_and0000 ),
    .Q(\LTC_A/LTC_frame_count/min_units [3])
  );
  FDRE   \LTC_A/LTC_frame_count/sec_units_0  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/sec_units_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_sec_units ),
    .R(\LTC_A/LTC_frame_count/sec_units_and0000 ),
    .Q(\LTC_A/LTC_frame_count/sec_units [0])
  );
  FDRE   \LTC_A/LTC_frame_count/sec_units_1  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/sec_units_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_sec_units1 ),
    .R(\LTC_A/LTC_frame_count/sec_units_and0000 ),
    .Q(\LTC_A/LTC_frame_count/sec_units [1])
  );
  FDRE   \LTC_A/LTC_frame_count/sec_units_2  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/sec_units_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_sec_units2 ),
    .R(\LTC_A/LTC_frame_count/sec_units_and0000 ),
    .Q(\LTC_A/LTC_frame_count/sec_units [2])
  );
  FDRE   \LTC_A/LTC_frame_count/sec_units_3  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/sec_units_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_sec_units3 ),
    .R(\LTC_A/LTC_frame_count/sec_units_and0000 ),
    .Q(\LTC_A/LTC_frame_count/sec_units [3])
  );
  FDRE   \LTC_A/LTC_frame_count/min_tens_0  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/min_tens_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_min_tens ),
    .R(\LTC_A/LTC_frame_count/min_tens_and0000 ),
    .Q(\LTC_A/LTC_frame_count/min_tens [0])
  );
  FDRE   \LTC_A/LTC_frame_count/min_tens_1  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/min_tens_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_min_tens1 ),
    .R(\LTC_A/LTC_frame_count/min_tens_and0000 ),
    .Q(\LTC_A/LTC_frame_count/min_tens [1])
  );
  FDRE   \LTC_A/LTC_frame_count/min_tens_2  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/min_tens_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_min_tens2 ),
    .R(\LTC_A/LTC_frame_count/min_tens_and0000 ),
    .Q(\LTC_A/LTC_frame_count/min_tens [2])
  );
  FDRE   \LTC_A/LTC_frame_count/hour_units_0  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/hour_units_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_hour_units ),
    .R(\LTC_A/LTC_frame_count/hour_units_and0000 ),
    .Q(\LTC_A/LTC_frame_count/hour_units [0])
  );
  FDRE   \LTC_A/LTC_frame_count/hour_units_1  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/hour_units_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_hour_units1 ),
    .R(\LTC_A/LTC_frame_count/hour_units_and0000 ),
    .Q(\LTC_A/LTC_frame_count/hour_units [1])
  );
  FDRE   \LTC_A/LTC_frame_count/hour_units_2  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/hour_units_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_hour_units2 ),
    .R(\LTC_A/LTC_frame_count/hour_units_and0000 ),
    .Q(\LTC_A/LTC_frame_count/hour_units [2])
  );
  FDRE   \LTC_A/LTC_frame_count/hour_units_3  (
    .C(clk_148),
    .CE(\LTC_A/LTC_frame_count/hour_units_not0001 ),
    .D(\LTC_A/LTC_frame_count/Mcount_hour_units3 ),
    .R(\LTC_A/LTC_frame_count/hour_units_and0000 ),
    .Q(\LTC_A/LTC_frame_count/hour_units [3])
  );
  FD #(
    .INIT ( 1'b0 ))
  \LTC_A/LTC_clockgenerator/period_count_0  (
    .C(clk_148),
    .D(\LTC_A/LTC_clockgenerator/period_count_mux0003[15] ),
    .Q(\LTC_A/LTC_clockgenerator/period_count [0])
  );
  FD #(
    .INIT ( 1'b0 ))
  \LTC_A/LTC_clockgenerator/period_count_1  (
    .C(clk_148),
    .D(\LTC_A/LTC_clockgenerator/period_count_mux0003[14] ),
    .Q(\LTC_A/LTC_clockgenerator/period_count [1])
  );
  FD #(
    .INIT ( 1'b0 ))
  \LTC_A/LTC_clockgenerator/period_count_2  (
    .C(clk_148),
    .D(\LTC_A/LTC_clockgenerator/period_count_mux0003[13] ),
    .Q(\LTC_A/LTC_clockgenerator/period_count [2])
  );
  FD #(
    .INIT ( 1'b0 ))
  \LTC_A/LTC_clockgenerator/period_count_5  (
    .C(clk_148),
    .D(\LTC_A/LTC_clockgenerator/period_count_mux0003[10] ),
    .Q(\LTC_A/LTC_clockgenerator/period_count [5])
  );
  FD #(
    .INIT ( 1'b0 ))
  \LTC_A/LTC_clockgenerator/period_count_8  (
    .C(clk_148),
    .D(\LTC_A/LTC_clockgenerator/period_count_mux0003[7] ),
    .Q(\LTC_A/LTC_clockgenerator/period_count [8])
  );
  FD #(
    .INIT ( 1'b0 ))
  \LTC_A/LTC_clockgenerator/period_count_9  (
    .C(clk_148),
    .D(\LTC_A/LTC_clockgenerator/period_count_mux0003[6] ),
    .Q(\LTC_A/LTC_clockgenerator/period_count [9])
  );
  FD #(
    .INIT ( 1'b0 ))
  \LTC_A/LTC_clockgenerator/period_count_10  (
    .C(clk_148),
    .D(\LTC_A/LTC_clockgenerator/period_count_mux0003[5] ),
    .Q(\LTC_A/LTC_clockgenerator/period_count [10])
  );
  FD #(
    .INIT ( 1'b0 ))
  \LTC_A/LTC_clockgenerator/period_count_15  (
    .C(clk_148),
    .D(\LTC_A/LTC_clockgenerator/period_count_mux0003[0] ),
    .Q(\LTC_A/LTC_clockgenerator/period_count [15])
  );
  FDS   \LTC_A/LTC_clockgenerator/ltc_clock_tick_o  (
    .C(clk_148),
    .D(tp2_o_OBUF_3479),
    .S(\LTC_A/LTC_clockgenerator/ltc_clock_tick_o_or0000 ),
    .Q(\LTC_A/LTC_clockgenerator/ltc_clock_tick_o_227 )
  );
  FDR   \LTC_A/LTC_clockgenerator/ltc_sync_o  (
    .C(clk_148),
    .D(N1),
    .R(\LTC_A/LTC_clockgenerator/sync_i_inv ),
    .Q(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 )
  );
  FDSE   \LTC_A/LTC_clockgenerator/ltc_frame_tick_o  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_not0001 ),
    .D(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .S(\LTC_A/delay_sync_549 ),
    .Q(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 )
  );
  FDRE   \LTC_A/LTC_clockgenerator/ltc_bitcount_0  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .D(\LTC_A/LTC_clockgenerator/Result [0]),
    .R(\LTC_A/LTC_clockgenerator/ltc_bitcount_or0000 ),
    .Q(\LTC_A/LTC_clockgenerator/ltc_bitcount [0])
  );
  FDRE   \LTC_A/LTC_clockgenerator/ltc_bitcount_1  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .D(\LTC_A/LTC_clockgenerator/Result [1]),
    .R(\LTC_A/LTC_clockgenerator/ltc_bitcount_or0000 ),
    .Q(\LTC_A/LTC_clockgenerator/ltc_bitcount [1])
  );
  FDRE   \LTC_A/LTC_clockgenerator/ltc_bitcount_2  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .D(\LTC_A/LTC_clockgenerator/Result [2]),
    .R(\LTC_A/LTC_clockgenerator/ltc_bitcount_or0000 ),
    .Q(\LTC_A/LTC_clockgenerator/ltc_bitcount [2])
  );
  FDRE   \LTC_A/LTC_clockgenerator/ltc_bitcount_3  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .D(\LTC_A/LTC_clockgenerator/Result [3]),
    .R(\LTC_A/LTC_clockgenerator/ltc_bitcount_or0000 ),
    .Q(\LTC_A/LTC_clockgenerator/ltc_bitcount [3])
  );
  FDRE   \LTC_A/LTC_clockgenerator/ltc_bitcount_4  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .D(\LTC_A/LTC_clockgenerator/Result [4]),
    .R(\LTC_A/LTC_clockgenerator/ltc_bitcount_or0000 ),
    .Q(\LTC_A/LTC_clockgenerator/ltc_bitcount [4])
  );
  FDRE   \LTC_A/LTC_clockgenerator/ltc_bitcount_5  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .D(\LTC_A/LTC_clockgenerator/Result [5]),
    .R(\LTC_A/LTC_clockgenerator/ltc_bitcount_or0000 ),
    .Q(\LTC_A/LTC_clockgenerator/ltc_bitcount [5])
  );
  FDRE   \LTC_A/LTC_clockgenerator/ltc_bitcount_6  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .D(\LTC_A/LTC_clockgenerator/Result [6]),
    .R(\LTC_A/LTC_clockgenerator/ltc_bitcount_or0000 ),
    .Q(\LTC_A/LTC_clockgenerator/ltc_bitcount [6])
  );
  FDRE   \LTC_A/LTC_clockgenerator/ltc_bitcount_7  (
    .C(clk_148),
    .CE(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .D(\LTC_A/LTC_clockgenerator/Result [7]),
    .R(\LTC_A/LTC_clockgenerator/ltc_bitcount_or0000 ),
    .Q(\LTC_A/LTC_clockgenerator/ltc_bitcount [7])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<0>_rt_172 ),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [0])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_xor<0>  (
    .CI(N1),
    .LI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<0>_rt_172 ),
    .O(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [0])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<1>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [0]),
    .DI(N1),
    .S(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [1]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [1])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_xor<1>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [0]),
    .LI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [1]),
    .O(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [1])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<2>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [1]),
    .DI(N1),
    .S(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [2]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [2])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_xor<2>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [1]),
    .LI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [2]),
    .O(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [2])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<3>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [2]),
    .DI(N1),
    .S(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [3]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [3])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_xor<3>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [2]),
    .LI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [3]),
    .O(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [3])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<4>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [3]),
    .DI(N1),
    .S(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [4]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [4])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_xor<4>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [3]),
    .LI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [4]),
    .O(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [4])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<5>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [4]),
    .DI(N1),
    .S(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [5]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [5])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_xor<5>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [4]),
    .LI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [5]),
    .O(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [5])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<6>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [5]),
    .DI(N1),
    .S(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [6]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [6])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_xor<6>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [5]),
    .LI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [6]),
    .O(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [6])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<7>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [6]),
    .DI(N1),
    .S(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [7]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [7])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_xor<7>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [6]),
    .LI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [7]),
    .O(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [7])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<8>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [7]),
    .DI(N1),
    .S(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [8]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [8])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_xor<8>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [7]),
    .LI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [8]),
    .O(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [8])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<9>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [8]),
    .DI(N1),
    .S(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [9]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [9])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_xor<9>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [8]),
    .LI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [9]),
    .O(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [9])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<10>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [9]),
    .DI(N1),
    .S(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [10]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [10])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_xor<10>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [9]),
    .LI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [10]),
    .O(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [10])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<11>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [10]),
    .DI(N1),
    .S(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [11]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [11])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_xor<11>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [10]),
    .LI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [11]),
    .O(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [11])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<12>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [11]),
    .DI(N1),
    .S(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [12]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [12])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_xor<12>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [11]),
    .LI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [12]),
    .O(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [12])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<13>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [12]),
    .DI(N1),
    .S(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [13]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [13])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_xor<13>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [12]),
    .LI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [13]),
    .O(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [13])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<14>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [13]),
    .DI(N1),
    .S(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [14]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [14])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_xor<14>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [13]),
    .LI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [14]),
    .O(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [14])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_xor<15>  (
    .CI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy [14]),
    .LI(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [15]),
    .O(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [15])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<0>  (
    .CI(tp2_o_OBUF_3479),
    .DI(N1),
    .S(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_lut [0]),
    .O(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [0])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_xor<0>  (
    .CI(tp2_o_OBUF_3479),
    .LI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_lut [0]),
    .O(\LTC_A/LTC_clockgenerator/Result [0])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<1>  (
    .CI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [0]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<1>_rt_158 ),
    .O(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [1])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_xor<1>  (
    .CI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [0]),
    .LI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<1>_rt_158 ),
    .O(\LTC_A/LTC_clockgenerator/Result [1])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<2>  (
    .CI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [1]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<2>_rt_160 ),
    .O(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [2])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_xor<2>  (
    .CI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [1]),
    .LI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<2>_rt_160 ),
    .O(\LTC_A/LTC_clockgenerator/Result [2])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<3>  (
    .CI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [2]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<3>_rt_162 ),
    .O(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [3])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_xor<3>  (
    .CI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [2]),
    .LI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<3>_rt_162 ),
    .O(\LTC_A/LTC_clockgenerator/Result [3])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<4>  (
    .CI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [3]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<4>_rt_164 ),
    .O(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [4])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_xor<4>  (
    .CI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [3]),
    .LI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<4>_rt_164 ),
    .O(\LTC_A/LTC_clockgenerator/Result [4])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<5>  (
    .CI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [4]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<5>_rt_166 ),
    .O(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [5])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_xor<5>  (
    .CI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [4]),
    .LI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<5>_rt_166 ),
    .O(\LTC_A/LTC_clockgenerator/Result [5])
  );
  MUXCY   \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<6>  (
    .CI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [5]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<6>_rt_168 ),
    .O(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [6])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_xor<6>  (
    .CI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [5]),
    .LI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<6>_rt_168 ),
    .O(\LTC_A/LTC_clockgenerator/Result [6])
  );
  XORCY   \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_xor<7>  (
    .CI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy [6]),
    .LI(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_xor<7>_rt_170 ),
    .O(\LTC_A/LTC_clockgenerator/Result [7])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<23>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [22]),
    .LI(\LTC_A/Mcount_delay_count_low_xor<23>_rt_479 ),
    .O(\LTC_A/Result [23])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<22>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [21]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<22>_rt_461 ),
    .O(\LTC_A/Result [22])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<22>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [21]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<22>_rt_461 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [22])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<21>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [20]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<21>_rt_459 ),
    .O(\LTC_A/Result [21])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<21>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [20]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<21>_rt_459 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [21])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<20>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [19]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<20>_rt_457 ),
    .O(\LTC_A/Result [20])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<20>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [19]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<20>_rt_457 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [20])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<19>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [18]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<19>_rt_453 ),
    .O(\LTC_A/Result [19])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<19>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [18]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<19>_rt_453 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [19])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<18>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [17]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<18>_rt_451 ),
    .O(\LTC_A/Result [18])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<18>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [17]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<18>_rt_451 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [18])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<17>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [16]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<17>_rt_449 ),
    .O(\LTC_A/Result [17])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<17>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [16]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<17>_rt_449 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [17])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<16>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [15]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<16>_rt_447 ),
    .O(\LTC_A/Result [16])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<16>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [15]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<16>_rt_447 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [16])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<15>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [14]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<15>_rt_445 ),
    .O(\LTC_A/Result [15])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<15>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [14]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<15>_rt_445 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [15])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<14>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [13]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<14>_rt_443 ),
    .O(\LTC_A/Result [14])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<14>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [13]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<14>_rt_443 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [14])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<13>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [12]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<13>_rt_441 ),
    .O(\LTC_A/Result [13])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<13>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [12]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<13>_rt_441 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [13])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<12>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [11]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<12>_rt_439 ),
    .O(\LTC_A/Result [12])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<12>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [11]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<12>_rt_439 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [12])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<11>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [10]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<11>_rt_437 ),
    .O(\LTC_A/Result [11])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<11>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [10]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<11>_rt_437 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [11])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<10>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [9]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<10>_rt_435 ),
    .O(\LTC_A/Result [10])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<10>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [9]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<10>_rt_435 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [10])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<9>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [8]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<9>_rt_477 ),
    .O(\LTC_A/Result [9])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<9>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [8]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<9>_rt_477 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [9])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<8>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [7]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<8>_rt_475 ),
    .O(\LTC_A/Result [8])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<8>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [7]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<8>_rt_475 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [8])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<7>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [6]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<7>_rt_473 ),
    .O(\LTC_A/Result [7])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<7>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [6]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<7>_rt_473 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [7])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<6>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [5]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<6>_rt_471 ),
    .O(\LTC_A/Result [6])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<6>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [5]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<6>_rt_471 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [6])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<5>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [4]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<5>_rt_469 ),
    .O(\LTC_A/Result [5])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<5>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [4]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<5>_rt_469 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [5])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<4>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [3]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<4>_rt_467 ),
    .O(\LTC_A/Result [4])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<4>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [3]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<4>_rt_467 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [4])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<3>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [2]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<3>_rt_465 ),
    .O(\LTC_A/Result [3])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<3>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [2]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<3>_rt_465 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [3])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<2>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [1]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<2>_rt_463 ),
    .O(\LTC_A/Result [2])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<2>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [1]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<2>_rt_463 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [2])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<1>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [0]),
    .LI(\LTC_A/Mcount_delay_count_low_cy<1>_rt_455 ),
    .O(\LTC_A/Result [1])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<1>  (
    .CI(\LTC_A/Mcount_delay_count_low_cy [0]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcount_delay_count_low_cy<1>_rt_455 ),
    .O(\LTC_A/Mcount_delay_count_low_cy [1])
  );
  XORCY   \LTC_A/Mcount_delay_count_low_xor<0>  (
    .CI(tp2_o_OBUF_3479),
    .LI(\LTC_A/Mcount_delay_count_low_lut [0]),
    .O(\LTC_A/Result [0])
  );
  MUXCY   \LTC_A/Mcount_delay_count_low_cy<0>  (
    .CI(tp2_o_OBUF_3479),
    .DI(N1),
    .S(\LTC_A/Mcount_delay_count_low_lut [0]),
    .O(\LTC_A/Mcount_delay_count_low_cy [0])
  );
  MUXCY   \LTC_A/Mcompar_delay_sync_cmp_eq0000_cy<15>  (
    .CI(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [14]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [15]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [15])
  );
  LUT3 #(
    .INIT ( 8'h81 ))
  \LTC_A/Mcompar_delay_sync_cmp_eq0000_lut<15>  (
    .I0(tp2_o_OBUF_3479),
    .I1(LTC_a_delay[30]),
    .I2(LTC_a_delay[31]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [15])
  );
  MUXCY   \LTC_A/Mcompar_delay_sync_cmp_eq0000_cy<14>  (
    .CI(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [13]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [14]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [14])
  );
  LUT3 #(
    .INIT ( 8'h81 ))
  \LTC_A/Mcompar_delay_sync_cmp_eq0000_lut<14>  (
    .I0(tp2_o_OBUF_3479),
    .I1(LTC_a_delay[28]),
    .I2(LTC_a_delay[29]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [14])
  );
  MUXCY   \LTC_A/Mcompar_delay_sync_cmp_eq0000_cy<13>  (
    .CI(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [12]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [13]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [13])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_A/Mcompar_delay_sync_cmp_eq0000_lut<13>  (
    .I0(\LTC_A/delay_count_high [2]),
    .I1(LTC_a_delay[26]),
    .I2(\LTC_A/delay_count_high [3]),
    .I3(LTC_a_delay[27]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [13])
  );
  MUXCY   \LTC_A/Mcompar_delay_sync_cmp_eq0000_cy<12>  (
    .CI(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [11]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [12]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [12])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_A/Mcompar_delay_sync_cmp_eq0000_lut<12>  (
    .I0(\LTC_A/delay_count_high [0]),
    .I1(LTC_a_delay[24]),
    .I2(\LTC_A/delay_count_high [1]),
    .I3(LTC_a_delay[25]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [12])
  );
  MUXCY   \LTC_A/Mcompar_delay_sync_cmp_eq0000_cy<11>  (
    .CI(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [10]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [11]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [11])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_A/Mcompar_delay_sync_cmp_eq0000_lut<11>  (
    .I0(\LTC_A/delay_count_low [22]),
    .I1(LTC_a_delay[22]),
    .I2(\LTC_A/delay_count_low [23]),
    .I3(LTC_a_delay[23]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [11])
  );
  MUXCY   \LTC_A/Mcompar_delay_sync_cmp_eq0000_cy<10>  (
    .CI(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [9]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [10]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [10])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_A/Mcompar_delay_sync_cmp_eq0000_lut<10>  (
    .I0(\LTC_A/delay_count_low [20]),
    .I1(LTC_a_delay[20]),
    .I2(\LTC_A/delay_count_low [21]),
    .I3(LTC_a_delay[21]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [10])
  );
  MUXCY   \LTC_A/Mcompar_delay_sync_cmp_eq0000_cy<9>  (
    .CI(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [8]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [9]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [9])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_A/Mcompar_delay_sync_cmp_eq0000_lut<9>  (
    .I0(\LTC_A/delay_count_low [18]),
    .I1(LTC_a_delay[18]),
    .I2(\LTC_A/delay_count_low [19]),
    .I3(LTC_a_delay[19]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [9])
  );
  MUXCY   \LTC_A/Mcompar_delay_sync_cmp_eq0000_cy<8>  (
    .CI(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [7]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [8]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [8])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_A/Mcompar_delay_sync_cmp_eq0000_lut<8>  (
    .I0(\LTC_A/delay_count_low [16]),
    .I1(LTC_a_delay[16]),
    .I2(\LTC_A/delay_count_low [17]),
    .I3(LTC_a_delay[17]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [8])
  );
  MUXCY   \LTC_A/Mcompar_delay_sync_cmp_eq0000_cy<7>  (
    .CI(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [6]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [7]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [7])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_A/Mcompar_delay_sync_cmp_eq0000_lut<7>  (
    .I0(\LTC_A/delay_count_low [14]),
    .I1(LTC_a_delay[14]),
    .I2(\LTC_A/delay_count_low [15]),
    .I3(LTC_a_delay[15]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [7])
  );
  MUXCY   \LTC_A/Mcompar_delay_sync_cmp_eq0000_cy<6>  (
    .CI(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [5]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [6]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [6])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_A/Mcompar_delay_sync_cmp_eq0000_lut<6>  (
    .I0(\LTC_A/delay_count_low [12]),
    .I1(LTC_a_delay[12]),
    .I2(\LTC_A/delay_count_low [13]),
    .I3(LTC_a_delay[13]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [6])
  );
  MUXCY   \LTC_A/Mcompar_delay_sync_cmp_eq0000_cy<5>  (
    .CI(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [4]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [5]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [5])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_A/Mcompar_delay_sync_cmp_eq0000_lut<5>  (
    .I0(\LTC_A/delay_count_low [10]),
    .I1(LTC_a_delay[10]),
    .I2(\LTC_A/delay_count_low [11]),
    .I3(LTC_a_delay[11]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [5])
  );
  MUXCY   \LTC_A/Mcompar_delay_sync_cmp_eq0000_cy<4>  (
    .CI(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [3]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [4]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [4])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_A/Mcompar_delay_sync_cmp_eq0000_lut<4>  (
    .I0(\LTC_A/delay_count_low [8]),
    .I1(LTC_a_delay[8]),
    .I2(\LTC_A/delay_count_low [9]),
    .I3(LTC_a_delay[9]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [4])
  );
  MUXCY   \LTC_A/Mcompar_delay_sync_cmp_eq0000_cy<3>  (
    .CI(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [2]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [3]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [3])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_A/Mcompar_delay_sync_cmp_eq0000_lut<3>  (
    .I0(\LTC_A/delay_count_low [6]),
    .I1(LTC_a_delay[6]),
    .I2(\LTC_A/delay_count_low [7]),
    .I3(LTC_a_delay[7]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [3])
  );
  MUXCY   \LTC_A/Mcompar_delay_sync_cmp_eq0000_cy<2>  (
    .CI(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [1]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [2]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [2])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_A/Mcompar_delay_sync_cmp_eq0000_lut<2>  (
    .I0(\LTC_A/delay_count_low [4]),
    .I1(LTC_a_delay[4]),
    .I2(\LTC_A/delay_count_low [5]),
    .I3(LTC_a_delay[5]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [2])
  );
  MUXCY   \LTC_A/Mcompar_delay_sync_cmp_eq0000_cy<1>  (
    .CI(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [0]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [1]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [1])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_A/Mcompar_delay_sync_cmp_eq0000_lut<1>  (
    .I0(\LTC_A/delay_count_low [2]),
    .I1(LTC_a_delay[2]),
    .I2(\LTC_A/delay_count_low [3]),
    .I3(LTC_a_delay[3]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [1])
  );
  MUXCY   \LTC_A/Mcompar_delay_sync_cmp_eq0000_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [0]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [0])
  );
  LUT4 #(
    .INIT ( 16'h9009 ))
  \LTC_A/Mcompar_delay_sync_cmp_eq0000_lut<0>  (
    .I0(\LTC_A/delay_count_low [0]),
    .I1(LTC_a_delay[0]),
    .I2(\LTC_A/delay_count_low [1]),
    .I3(LTC_a_delay[1]),
    .O(\LTC_A/Mcompar_delay_sync_cmp_eq0000_lut [0])
  );
  FDRE   \LTC_A/delay_count_high_3  (
    .C(clk_148),
    .CE(\LTC_A/delay_count_high_cmp_eq0000 ),
    .D(\LTC_A/Result<3>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(\LTC_A/delay_count_high [3])
  );
  FDRE   \LTC_A/delay_count_high_2  (
    .C(clk_148),
    .CE(\LTC_A/delay_count_high_cmp_eq0000 ),
    .D(\LTC_A/Result<2>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(\LTC_A/delay_count_high [2])
  );
  FDRE   \LTC_A/delay_count_high_1  (
    .C(clk_148),
    .CE(\LTC_A/delay_count_high_cmp_eq0000 ),
    .D(\LTC_A/Result<1>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(\LTC_A/delay_count_high [1])
  );
  FDRE   \LTC_A/delay_count_high_0  (
    .C(clk_148),
    .CE(\LTC_A/delay_count_high_cmp_eq0000 ),
    .D(\LTC_A/Result<0>1 ),
    .R(\VCXO_pll/pps_148_tick_o_2290 ),
    .Q(\LTC_A/delay_count_high [0])
  );
  FDR   \LTC_A/delay_count_low_23  (
    .C(clk_148),
    .D(\LTC_A/Result [23]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [23])
  );
  FDR   \LTC_A/delay_count_low_22  (
    .C(clk_148),
    .D(\LTC_A/Result [22]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [22])
  );
  FDR   \LTC_A/delay_count_low_21  (
    .C(clk_148),
    .D(\LTC_A/Result [21]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [21])
  );
  FDR   \LTC_A/delay_count_low_20  (
    .C(clk_148),
    .D(\LTC_A/Result [20]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [20])
  );
  FDR   \LTC_A/delay_count_low_19  (
    .C(clk_148),
    .D(\LTC_A/Result [19]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [19])
  );
  FDR   \LTC_A/delay_count_low_18  (
    .C(clk_148),
    .D(\LTC_A/Result [18]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [18])
  );
  FDR   \LTC_A/delay_count_low_17  (
    .C(clk_148),
    .D(\LTC_A/Result [17]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [17])
  );
  FDR   \LTC_A/delay_count_low_16  (
    .C(clk_148),
    .D(\LTC_A/Result [16]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [16])
  );
  FDR   \LTC_A/delay_count_low_15  (
    .C(clk_148),
    .D(\LTC_A/Result [15]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [15])
  );
  FDR   \LTC_A/delay_count_low_14  (
    .C(clk_148),
    .D(\LTC_A/Result [14]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [14])
  );
  FDR   \LTC_A/delay_count_low_13  (
    .C(clk_148),
    .D(\LTC_A/Result [13]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [13])
  );
  FDR   \LTC_A/delay_count_low_12  (
    .C(clk_148),
    .D(\LTC_A/Result [12]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [12])
  );
  FDR   \LTC_A/delay_count_low_11  (
    .C(clk_148),
    .D(\LTC_A/Result [11]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [11])
  );
  FDR   \LTC_A/delay_count_low_10  (
    .C(clk_148),
    .D(\LTC_A/Result [10]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [10])
  );
  FDR   \LTC_A/delay_count_low_9  (
    .C(clk_148),
    .D(\LTC_A/Result [9]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [9])
  );
  FDR   \LTC_A/delay_count_low_8  (
    .C(clk_148),
    .D(\LTC_A/Result [8]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [8])
  );
  FDR   \LTC_A/delay_count_low_7  (
    .C(clk_148),
    .D(\LTC_A/Result [7]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [7])
  );
  FDR   \LTC_A/delay_count_low_6  (
    .C(clk_148),
    .D(\LTC_A/Result [6]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [6])
  );
  FDR   \LTC_A/delay_count_low_5  (
    .C(clk_148),
    .D(\LTC_A/Result [5]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [5])
  );
  FDR   \LTC_A/delay_count_low_4  (
    .C(clk_148),
    .D(\LTC_A/Result [4]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [4])
  );
  FDR   \LTC_A/delay_count_low_3  (
    .C(clk_148),
    .D(\LTC_A/Result [3]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [3])
  );
  FDR   \LTC_A/delay_count_low_2  (
    .C(clk_148),
    .D(\LTC_A/Result [2]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [2])
  );
  FDR   \LTC_A/delay_count_low_1  (
    .C(clk_148),
    .D(\LTC_A/Result [1]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [1])
  );
  FDR   \LTC_A/delay_count_low_0  (
    .C(clk_148),
    .D(\LTC_A/Result [0]),
    .R(\LTC_A/delay_count_low_or0000 ),
    .Q(\LTC_A/delay_count_low [0])
  );
  FDR   \LTC_A/delay_sync  (
    .C(clk_148),
    .D(N1),
    .R(\LTC_A/delay_sync_not0001 ),
    .Q(\LTC_A/delay_sync_549 )
  );
  MUXCY   \longer_tick_cmp_eq0000_wg_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(longer_tick_cmp_eq0000_wg_lut[0]),
    .O(longer_tick_cmp_eq0000_wg_cy[0])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \longer_tick_cmp_eq0000_wg_lut<1>  (
    .I0(count[8]),
    .I1(count[6]),
    .I2(count[3]),
    .I3(count[7]),
    .O(longer_tick_cmp_eq0000_wg_lut[1])
  );
  MUXCY   \longer_tick_cmp_eq0000_wg_cy<1>  (
    .CI(longer_tick_cmp_eq0000_wg_cy[0]),
    .DI(tp2_o_OBUF_3479),
    .S(longer_tick_cmp_eq0000_wg_lut[1]),
    .O(longer_tick_cmp_eq0000_wg_cy[1])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \longer_tick_cmp_eq0000_wg_lut<2>  (
    .I0(count[9]),
    .I1(count[10]),
    .I2(count[5]),
    .I3(count[13]),
    .O(longer_tick_cmp_eq0000_wg_lut[2])
  );
  MUXCY   \longer_tick_cmp_eq0000_wg_cy<2>  (
    .CI(longer_tick_cmp_eq0000_wg_cy[1]),
    .DI(tp2_o_OBUF_3479),
    .S(longer_tick_cmp_eq0000_wg_lut[2]),
    .O(longer_tick_cmp_eq0000_wg_cy[2])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \longer_tick_cmp_eq0000_wg_lut<3>  (
    .I0(count[11]),
    .I1(count[12]),
    .I2(count[1]),
    .I3(count[14]),
    .O(longer_tick_cmp_eq0000_wg_lut[3])
  );
  MUXCY   \longer_tick_cmp_eq0000_wg_cy<3>  (
    .CI(longer_tick_cmp_eq0000_wg_cy[2]),
    .DI(tp2_o_OBUF_3479),
    .S(longer_tick_cmp_eq0000_wg_lut[3]),
    .O(longer_tick_cmp_eq0000_wg_cy[3])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \longer_tick_cmp_eq0000_wg_lut<4>  (
    .I0(count[15]),
    .I1(count[18]),
    .I2(count[0]),
    .I3(count[16]),
    .O(longer_tick_cmp_eq0000_wg_lut[4])
  );
  MUXCY   \longer_tick_cmp_eq0000_wg_cy<4>  (
    .CI(longer_tick_cmp_eq0000_wg_cy[3]),
    .DI(tp2_o_OBUF_3479),
    .S(longer_tick_cmp_eq0000_wg_lut[4]),
    .O(longer_tick_cmp_eq0000_wg_cy[4])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \longer_tick_cmp_eq0000_wg_lut<5>  (
    .I0(count[17]),
    .I1(count[19]),
    .I2(count[2]),
    .I3(count[20]),
    .O(longer_tick_cmp_eq0000_wg_lut[5])
  );
  MUXCY   \longer_tick_cmp_eq0000_wg_cy<5>  (
    .CI(longer_tick_cmp_eq0000_wg_cy[4]),
    .DI(tp2_o_OBUF_3479),
    .S(longer_tick_cmp_eq0000_wg_lut[5]),
    .O(longer_tick_not0001_inv)
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \f_gen/pulse_state_cmp_eq0000_wg_lut<0>  (
    .I0(\f_gen/count [8]),
    .I1(\f_gen/count [9]),
    .I2(\f_gen/count [7]),
    .I3(\f_gen/count [10]),
    .O(\f_gen/pulse_state_cmp_eq0000_wg_lut [0])
  );
  MUXCY   \f_gen/pulse_state_cmp_eq0000_wg_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\f_gen/pulse_state_cmp_eq0000_wg_lut [0]),
    .O(\f_gen/pulse_state_cmp_eq0000_wg_cy [0])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \f_gen/pulse_state_cmp_eq0000_wg_lut<1>  (
    .I0(\f_gen/count [11]),
    .I1(\f_gen/count [12]),
    .I2(\f_gen/count [6]),
    .I3(\f_gen/count [13]),
    .O(\f_gen/pulse_state_cmp_eq0000_wg_lut [1])
  );
  MUXCY   \f_gen/pulse_state_cmp_eq0000_wg_cy<1>  (
    .CI(\f_gen/pulse_state_cmp_eq0000_wg_cy [0]),
    .DI(tp2_o_OBUF_3479),
    .S(\f_gen/pulse_state_cmp_eq0000_wg_lut [1]),
    .O(\f_gen/pulse_state_cmp_eq0000_wg_cy [1])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \f_gen/pulse_state_cmp_eq0000_wg_lut<2>  (
    .I0(\f_gen/count [14]),
    .I1(\f_gen/count [15]),
    .I2(\f_gen/count [5]),
    .I3(\f_gen/count [16]),
    .O(\f_gen/pulse_state_cmp_eq0000_wg_lut [2])
  );
  MUXCY   \f_gen/pulse_state_cmp_eq0000_wg_cy<2>  (
    .CI(\f_gen/pulse_state_cmp_eq0000_wg_cy [1]),
    .DI(tp2_o_OBUF_3479),
    .S(\f_gen/pulse_state_cmp_eq0000_wg_lut [2]),
    .O(\f_gen/pulse_state_cmp_eq0000_wg_cy [2])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \f_gen/pulse_state_cmp_eq0000_wg_lut<3>  (
    .I0(\f_gen/count [17]),
    .I1(\f_gen/count [18]),
    .I2(\f_gen/count [4]),
    .I3(\f_gen/count [19]),
    .O(\f_gen/pulse_state_cmp_eq0000_wg_lut [3])
  );
  MUXCY   \f_gen/pulse_state_cmp_eq0000_wg_cy<3>  (
    .CI(\f_gen/pulse_state_cmp_eq0000_wg_cy [2]),
    .DI(tp2_o_OBUF_3479),
    .S(\f_gen/pulse_state_cmp_eq0000_wg_lut [3]),
    .O(\f_gen/pulse_state_cmp_eq0000_wg_cy [3])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \f_gen/pulse_state_cmp_eq0000_wg_lut<4>  (
    .I0(\f_gen/count [20]),
    .I1(\f_gen/count [21]),
    .I2(\f_gen/count [3]),
    .I3(\f_gen/count [22]),
    .O(\f_gen/pulse_state_cmp_eq0000_wg_lut [4])
  );
  MUXCY   \f_gen/pulse_state_cmp_eq0000_wg_cy<4>  (
    .CI(\f_gen/pulse_state_cmp_eq0000_wg_cy [3]),
    .DI(tp2_o_OBUF_3479),
    .S(\f_gen/pulse_state_cmp_eq0000_wg_lut [4]),
    .O(\f_gen/pulse_state_cmp_eq0000_wg_cy [4])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \f_gen/pulse_state_cmp_eq0000_wg_lut<5>  (
    .I0(\f_gen/count [23]),
    .I1(\f_gen/count [24]),
    .I2(\f_gen/count [2]),
    .I3(\f_gen/count [25]),
    .O(\f_gen/pulse_state_cmp_eq0000_wg_lut [5])
  );
  MUXCY   \f_gen/pulse_state_cmp_eq0000_wg_cy<5>  (
    .CI(\f_gen/pulse_state_cmp_eq0000_wg_cy [4]),
    .DI(tp2_o_OBUF_3479),
    .S(\f_gen/pulse_state_cmp_eq0000_wg_lut [5]),
    .O(\f_gen/pulse_state_cmp_eq0000_wg_cy [5])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \f_gen/pulse_state_cmp_eq0000_wg_lut<6>  (
    .I0(\f_gen/count [26]),
    .I1(\f_gen/count [27]),
    .I2(\f_gen/count [1]),
    .I3(\f_gen/count [28]),
    .O(\f_gen/pulse_state_cmp_eq0000_wg_lut [6])
  );
  MUXCY   \f_gen/pulse_state_cmp_eq0000_wg_cy<6>  (
    .CI(\f_gen/pulse_state_cmp_eq0000_wg_cy [5]),
    .DI(tp2_o_OBUF_3479),
    .S(\f_gen/pulse_state_cmp_eq0000_wg_lut [6]),
    .O(\f_gen/pulse_state_cmp_eq0000_wg_cy [6])
  );
  LUT4 #(
    .INIT ( 16'h0010 ))
  \f_gen/pulse_state_cmp_eq0000_wg_lut<7>  (
    .I0(\f_gen/count [29]),
    .I1(\f_gen/count [30]),
    .I2(\f_gen/count [0]),
    .I3(\f_gen/count [31]),
    .O(\f_gen/pulse_state_cmp_eq0000_wg_lut [7])
  );
  MUXCY   \f_gen/pulse_state_cmp_eq0000_wg_cy<7>  (
    .CI(\f_gen/pulse_state_cmp_eq0000_wg_cy [6]),
    .DI(tp2_o_OBUF_3479),
    .S(\f_gen/pulse_state_cmp_eq0000_wg_lut [7]),
    .O(\f_gen/pulse_state_cmp_eq0000 )
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \h_gen/pulse_state_cmp_eq0000_wg_lut<0>  (
    .I0(\h_gen/count [8]),
    .I1(\h_gen/count [9]),
    .I2(\h_gen/count [7]),
    .I3(\h_gen/count [10]),
    .O(\h_gen/pulse_state_cmp_eq0000_wg_lut [0])
  );
  MUXCY   \h_gen/pulse_state_cmp_eq0000_wg_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\h_gen/pulse_state_cmp_eq0000_wg_lut [0]),
    .O(\h_gen/pulse_state_cmp_eq0000_wg_cy [0])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \h_gen/pulse_state_cmp_eq0000_wg_lut<1>  (
    .I0(\h_gen/count [11]),
    .I1(\h_gen/count [12]),
    .I2(\h_gen/count [6]),
    .I3(\h_gen/count [13]),
    .O(\h_gen/pulse_state_cmp_eq0000_wg_lut [1])
  );
  MUXCY   \h_gen/pulse_state_cmp_eq0000_wg_cy<1>  (
    .CI(\h_gen/pulse_state_cmp_eq0000_wg_cy [0]),
    .DI(tp2_o_OBUF_3479),
    .S(\h_gen/pulse_state_cmp_eq0000_wg_lut [1]),
    .O(\h_gen/pulse_state_cmp_eq0000_wg_cy [1])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \h_gen/pulse_state_cmp_eq0000_wg_lut<2>  (
    .I0(\h_gen/count [14]),
    .I1(\h_gen/count [15]),
    .I2(\h_gen/count [5]),
    .I3(\h_gen/count [16]),
    .O(\h_gen/pulse_state_cmp_eq0000_wg_lut [2])
  );
  MUXCY   \h_gen/pulse_state_cmp_eq0000_wg_cy<2>  (
    .CI(\h_gen/pulse_state_cmp_eq0000_wg_cy [1]),
    .DI(tp2_o_OBUF_3479),
    .S(\h_gen/pulse_state_cmp_eq0000_wg_lut [2]),
    .O(\h_gen/pulse_state_cmp_eq0000_wg_cy [2])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \h_gen/pulse_state_cmp_eq0000_wg_lut<3>  (
    .I0(\h_gen/count [17]),
    .I1(\h_gen/count [18]),
    .I2(\h_gen/count [4]),
    .I3(\h_gen/count [19]),
    .O(\h_gen/pulse_state_cmp_eq0000_wg_lut [3])
  );
  MUXCY   \h_gen/pulse_state_cmp_eq0000_wg_cy<3>  (
    .CI(\h_gen/pulse_state_cmp_eq0000_wg_cy [2]),
    .DI(tp2_o_OBUF_3479),
    .S(\h_gen/pulse_state_cmp_eq0000_wg_lut [3]),
    .O(\h_gen/pulse_state_cmp_eq0000_wg_cy [3])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \h_gen/pulse_state_cmp_eq0000_wg_lut<4>  (
    .I0(\h_gen/count [20]),
    .I1(\h_gen/count [21]),
    .I2(\h_gen/count [3]),
    .I3(\h_gen/count [22]),
    .O(\h_gen/pulse_state_cmp_eq0000_wg_lut [4])
  );
  MUXCY   \h_gen/pulse_state_cmp_eq0000_wg_cy<4>  (
    .CI(\h_gen/pulse_state_cmp_eq0000_wg_cy [3]),
    .DI(tp2_o_OBUF_3479),
    .S(\h_gen/pulse_state_cmp_eq0000_wg_lut [4]),
    .O(\h_gen/pulse_state_cmp_eq0000_wg_cy [4])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \h_gen/pulse_state_cmp_eq0000_wg_lut<5>  (
    .I0(\h_gen/count [23]),
    .I1(\h_gen/count [24]),
    .I2(\h_gen/count [2]),
    .I3(\h_gen/count [25]),
    .O(\h_gen/pulse_state_cmp_eq0000_wg_lut [5])
  );
  MUXCY   \h_gen/pulse_state_cmp_eq0000_wg_cy<5>  (
    .CI(\h_gen/pulse_state_cmp_eq0000_wg_cy [4]),
    .DI(tp2_o_OBUF_3479),
    .S(\h_gen/pulse_state_cmp_eq0000_wg_lut [5]),
    .O(\h_gen/pulse_state_cmp_eq0000_wg_cy [5])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \h_gen/pulse_state_cmp_eq0000_wg_lut<6>  (
    .I0(\h_gen/count [26]),
    .I1(\h_gen/count [27]),
    .I2(\h_gen/count [1]),
    .I3(\h_gen/count [28]),
    .O(\h_gen/pulse_state_cmp_eq0000_wg_lut [6])
  );
  MUXCY   \h_gen/pulse_state_cmp_eq0000_wg_cy<6>  (
    .CI(\h_gen/pulse_state_cmp_eq0000_wg_cy [5]),
    .DI(tp2_o_OBUF_3479),
    .S(\h_gen/pulse_state_cmp_eq0000_wg_lut [6]),
    .O(\h_gen/pulse_state_cmp_eq0000_wg_cy [6])
  );
  LUT4 #(
    .INIT ( 16'h0010 ))
  \h_gen/pulse_state_cmp_eq0000_wg_lut<7>  (
    .I0(\h_gen/count [29]),
    .I1(\h_gen/count [30]),
    .I2(\h_gen/count [0]),
    .I3(\h_gen/count [31]),
    .O(\h_gen/pulse_state_cmp_eq0000_wg_lut [7])
  );
  MUXCY   \h_gen/pulse_state_cmp_eq0000_wg_cy<7>  (
    .CI(\h_gen/pulse_state_cmp_eq0000_wg_cy [6]),
    .DI(tp2_o_OBUF_3479),
    .S(\h_gen/pulse_state_cmp_eq0000_wg_lut [7]),
    .O(\h_gen/pulse_state_cmp_eq0000 )
  );
  LUT2 #(
    .INIT ( 4'h1 ))
  \serial_interface/state_cmp_eq0011_wg_lut<0>  (
    .I0(\serial_interface/time_out [4]),
    .I1(\serial_interface/time_out [8]),
    .O(\serial_interface/state_cmp_eq0011_wg_lut [0])
  );
  MUXCY   \serial_interface/state_cmp_eq0011_wg_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\serial_interface/state_cmp_eq0011_wg_lut [0]),
    .O(\serial_interface/state_cmp_eq0011_wg_cy [0])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \serial_interface/state_cmp_eq0011_wg_lut<1>  (
    .I0(\serial_interface/time_out [6]),
    .I1(\serial_interface/time_out [7]),
    .I2(\serial_interface/time_out [3]),
    .I3(\serial_interface/time_out [9]),
    .O(\serial_interface/state_cmp_eq0011_wg_lut [1])
  );
  MUXCY   \serial_interface/state_cmp_eq0011_wg_cy<1>  (
    .CI(\serial_interface/state_cmp_eq0011_wg_cy [0]),
    .DI(tp2_o_OBUF_3479),
    .S(\serial_interface/state_cmp_eq0011_wg_lut [1]),
    .O(\serial_interface/state_cmp_eq0011_wg_cy [1])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \serial_interface/state_cmp_eq0011_wg_lut<2>  (
    .I0(\serial_interface/time_out [10]),
    .I1(\serial_interface/time_out [13]),
    .I2(\serial_interface/time_out [5]),
    .I3(\serial_interface/time_out [11]),
    .O(\serial_interface/state_cmp_eq0011_wg_lut [2])
  );
  MUXCY   \serial_interface/state_cmp_eq0011_wg_cy<2>  (
    .CI(\serial_interface/state_cmp_eq0011_wg_cy [1]),
    .DI(tp2_o_OBUF_3479),
    .S(\serial_interface/state_cmp_eq0011_wg_lut [2]),
    .O(\serial_interface/state_cmp_eq0011_wg_cy [2])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \serial_interface/state_cmp_eq0011_wg_lut<3>  (
    .I0(\serial_interface/time_out [12]),
    .I1(\serial_interface/time_out [16]),
    .I2(\serial_interface/time_out [1]),
    .I3(\serial_interface/time_out [14]),
    .O(\serial_interface/state_cmp_eq0011_wg_lut [3])
  );
  MUXCY   \serial_interface/state_cmp_eq0011_wg_cy<3>  (
    .CI(\serial_interface/state_cmp_eq0011_wg_cy [2]),
    .DI(tp2_o_OBUF_3479),
    .S(\serial_interface/state_cmp_eq0011_wg_lut [3]),
    .O(\serial_interface/state_cmp_eq0011_wg_cy [3])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \serial_interface/state_cmp_eq0011_wg_lut<4>  (
    .I0(\serial_interface/time_out [15]),
    .I1(\serial_interface/time_out [19]),
    .I2(\serial_interface/time_out [0]),
    .I3(\serial_interface/time_out [17]),
    .O(\serial_interface/state_cmp_eq0011_wg_lut [4])
  );
  MUXCY   \serial_interface/state_cmp_eq0011_wg_cy<4>  (
    .CI(\serial_interface/state_cmp_eq0011_wg_cy [3]),
    .DI(tp2_o_OBUF_3479),
    .S(\serial_interface/state_cmp_eq0011_wg_lut [4]),
    .O(\serial_interface/state_cmp_eq0011_wg_cy [4])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \serial_interface/state_cmp_eq0011_wg_lut<5>  (
    .I0(\serial_interface/time_out [18]),
    .I1(\serial_interface/time_out [20]),
    .I2(\serial_interface/time_out [2]),
    .I3(\serial_interface/time_out [21]),
    .O(\serial_interface/state_cmp_eq0011_wg_lut [5])
  );
  MUXCY   \serial_interface/state_cmp_eq0011_wg_cy<5>  (
    .CI(\serial_interface/state_cmp_eq0011_wg_cy [4]),
    .DI(tp2_o_OBUF_3479),
    .S(\serial_interface/state_cmp_eq0011_wg_lut [5]),
    .O(\serial_interface/state_cmp_eq0011 )
  );
  LUT2 #(
    .INIT ( 4'h1 ))
  \OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_lut<0>  (
    .I0(\OCXO_pll/phase_detect/phase_counter [21]),
    .I1(\OCXO_pll/phase_detect/phase_counter [20]),
    .O(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_lut [0])
  );
  MUXCY   \OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_lut [0]),
    .O(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy [0])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_lut<1>  (
    .I0(\OCXO_pll/phase_detect/phase_counter [19]),
    .I1(\OCXO_pll/phase_detect/phase_counter [17]),
    .I2(\OCXO_pll/phase_detect/phase_counter [22]),
    .I3(\OCXO_pll/phase_detect/phase_counter [15]),
    .O(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_lut [1])
  );
  MUXCY   \OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy<1>  (
    .CI(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy [0]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_lut [1]),
    .O(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy [1])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_lut<2>  (
    .I0(\OCXO_pll/phase_detect/phase_counter [12]),
    .I1(\OCXO_pll/phase_detect/phase_counter [14]),
    .I2(\OCXO_pll/phase_detect/phase_counter [25]),
    .I3(\OCXO_pll/phase_detect/phase_counter [13]),
    .O(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_lut [2])
  );
  MUXCY   \OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy<2>  (
    .CI(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy [1]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_lut [2]),
    .O(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy [2])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_lut<3>  (
    .I0(\OCXO_pll/phase_detect/phase_counter [6]),
    .I1(\OCXO_pll/phase_detect/phase_counter [5]),
    .I2(\OCXO_pll/phase_detect/phase_counter [0]),
    .I3(\OCXO_pll/phase_detect/phase_counter [4]),
    .O(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_lut [3])
  );
  MUXCY   \OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy<3>  (
    .CI(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy [2]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_lut [3]),
    .O(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy [3])
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_lut<4>  (
    .I0(\OCXO_pll/phase_detect/phase_counter [3]),
    .I1(\OCXO_pll/phase_detect/phase_counter [1]),
    .I2(\OCXO_pll/phase_detect/phase_counter [23]),
    .I3(\OCXO_pll/phase_detect/phase_counter [2]),
    .O(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_lut [4])
  );
  MUXCY   \OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy<4>  (
    .CI(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy [3]),
    .DI(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_lut [4]),
    .O(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy [4])
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \LTC_B/delay_count_high_cmp_eq0000_wg_lut<0>  (
    .I0(\LTC_B/delay_count_low [8]),
    .I1(\LTC_B/delay_count_low [6]),
    .I2(\LTC_B/delay_count_low [4]),
    .I3(\LTC_B/delay_count_low [7]),
    .O(\LTC_B/delay_count_high_cmp_eq0000_wg_lut [0])
  );
  MUXCY   \LTC_B/delay_count_high_cmp_eq0000_wg_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/delay_count_high_cmp_eq0000_wg_lut [0]),
    .O(\LTC_B/delay_count_high_cmp_eq0000_wg_cy [0])
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \LTC_B/delay_count_high_cmp_eq0000_wg_lut<1>  (
    .I0(\LTC_B/delay_count_low [11]),
    .I1(\LTC_B/delay_count_low [9]),
    .I2(\LTC_B/delay_count_low [3]),
    .I3(\LTC_B/delay_count_low [10]),
    .O(\LTC_B/delay_count_high_cmp_eq0000_wg_lut [1])
  );
  MUXCY   \LTC_B/delay_count_high_cmp_eq0000_wg_cy<1>  (
    .CI(\LTC_B/delay_count_high_cmp_eq0000_wg_cy [0]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/delay_count_high_cmp_eq0000_wg_lut [1]),
    .O(\LTC_B/delay_count_high_cmp_eq0000_wg_cy [1])
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \LTC_B/delay_count_high_cmp_eq0000_wg_lut<2>  (
    .I0(\LTC_B/delay_count_low [14]),
    .I1(\LTC_B/delay_count_low [12]),
    .I2(\LTC_B/delay_count_low [5]),
    .I3(\LTC_B/delay_count_low [13]),
    .O(\LTC_B/delay_count_high_cmp_eq0000_wg_lut [2])
  );
  MUXCY   \LTC_B/delay_count_high_cmp_eq0000_wg_cy<2>  (
    .CI(\LTC_B/delay_count_high_cmp_eq0000_wg_cy [1]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/delay_count_high_cmp_eq0000_wg_lut [2]),
    .O(\LTC_B/delay_count_high_cmp_eq0000_wg_cy [2])
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \LTC_B/delay_count_high_cmp_eq0000_wg_lut<3>  (
    .I0(\LTC_B/delay_count_low [17]),
    .I1(\LTC_B/delay_count_low [15]),
    .I2(\LTC_B/delay_count_low [1]),
    .I3(\LTC_B/delay_count_low [16]),
    .O(\LTC_B/delay_count_high_cmp_eq0000_wg_lut [3])
  );
  MUXCY   \LTC_B/delay_count_high_cmp_eq0000_wg_cy<3>  (
    .CI(\LTC_B/delay_count_high_cmp_eq0000_wg_cy [2]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/delay_count_high_cmp_eq0000_wg_lut [3]),
    .O(\LTC_B/delay_count_high_cmp_eq0000_wg_cy [3])
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \LTC_B/delay_count_high_cmp_eq0000_wg_lut<4>  (
    .I0(\LTC_B/delay_count_low [20]),
    .I1(\LTC_B/delay_count_low [18]),
    .I2(\LTC_B/delay_count_low [0]),
    .I3(\LTC_B/delay_count_low [19]),
    .O(\LTC_B/delay_count_high_cmp_eq0000_wg_lut [4])
  );
  MUXCY   \LTC_B/delay_count_high_cmp_eq0000_wg_cy<4>  (
    .CI(\LTC_B/delay_count_high_cmp_eq0000_wg_cy [3]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/delay_count_high_cmp_eq0000_wg_lut [4]),
    .O(\LTC_B/delay_count_high_cmp_eq0000_wg_cy [4])
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \LTC_B/delay_count_high_cmp_eq0000_wg_lut<5>  (
    .I0(\LTC_B/delay_count_low [23]),
    .I1(\LTC_B/delay_count_low [21]),
    .I2(\LTC_B/delay_count_low [2]),
    .I3(\LTC_B/delay_count_low [22]),
    .O(\LTC_B/delay_count_high_cmp_eq0000_wg_lut [5])
  );
  MUXCY   \LTC_B/delay_count_high_cmp_eq0000_wg_cy<5>  (
    .CI(\LTC_B/delay_count_high_cmp_eq0000_wg_cy [4]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_B/delay_count_high_cmp_eq0000_wg_lut [5]),
    .O(\LTC_B/delay_count_high_cmp_eq0000 )
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \LTC_A/delay_count_high_cmp_eq0000_wg_lut<0>  (
    .I0(\LTC_A/delay_count_low [8]),
    .I1(\LTC_A/delay_count_low [6]),
    .I2(\LTC_A/delay_count_low [4]),
    .I3(\LTC_A/delay_count_low [7]),
    .O(\LTC_A/delay_count_high_cmp_eq0000_wg_lut [0])
  );
  MUXCY   \LTC_A/delay_count_high_cmp_eq0000_wg_cy<0>  (
    .CI(N1),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/delay_count_high_cmp_eq0000_wg_lut [0]),
    .O(\LTC_A/delay_count_high_cmp_eq0000_wg_cy [0])
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \LTC_A/delay_count_high_cmp_eq0000_wg_lut<1>  (
    .I0(\LTC_A/delay_count_low [11]),
    .I1(\LTC_A/delay_count_low [9]),
    .I2(\LTC_A/delay_count_low [3]),
    .I3(\LTC_A/delay_count_low [10]),
    .O(\LTC_A/delay_count_high_cmp_eq0000_wg_lut [1])
  );
  MUXCY   \LTC_A/delay_count_high_cmp_eq0000_wg_cy<1>  (
    .CI(\LTC_A/delay_count_high_cmp_eq0000_wg_cy [0]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/delay_count_high_cmp_eq0000_wg_lut [1]),
    .O(\LTC_A/delay_count_high_cmp_eq0000_wg_cy [1])
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \LTC_A/delay_count_high_cmp_eq0000_wg_lut<2>  (
    .I0(\LTC_A/delay_count_low [14]),
    .I1(\LTC_A/delay_count_low [12]),
    .I2(\LTC_A/delay_count_low [5]),
    .I3(\LTC_A/delay_count_low [13]),
    .O(\LTC_A/delay_count_high_cmp_eq0000_wg_lut [2])
  );
  MUXCY   \LTC_A/delay_count_high_cmp_eq0000_wg_cy<2>  (
    .CI(\LTC_A/delay_count_high_cmp_eq0000_wg_cy [1]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/delay_count_high_cmp_eq0000_wg_lut [2]),
    .O(\LTC_A/delay_count_high_cmp_eq0000_wg_cy [2])
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \LTC_A/delay_count_high_cmp_eq0000_wg_lut<3>  (
    .I0(\LTC_A/delay_count_low [17]),
    .I1(\LTC_A/delay_count_low [15]),
    .I2(\LTC_A/delay_count_low [1]),
    .I3(\LTC_A/delay_count_low [16]),
    .O(\LTC_A/delay_count_high_cmp_eq0000_wg_lut [3])
  );
  MUXCY   \LTC_A/delay_count_high_cmp_eq0000_wg_cy<3>  (
    .CI(\LTC_A/delay_count_high_cmp_eq0000_wg_cy [2]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/delay_count_high_cmp_eq0000_wg_lut [3]),
    .O(\LTC_A/delay_count_high_cmp_eq0000_wg_cy [3])
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \LTC_A/delay_count_high_cmp_eq0000_wg_lut<4>  (
    .I0(\LTC_A/delay_count_low [20]),
    .I1(\LTC_A/delay_count_low [18]),
    .I2(\LTC_A/delay_count_low [0]),
    .I3(\LTC_A/delay_count_low [19]),
    .O(\LTC_A/delay_count_high_cmp_eq0000_wg_lut [4])
  );
  MUXCY   \LTC_A/delay_count_high_cmp_eq0000_wg_cy<4>  (
    .CI(\LTC_A/delay_count_high_cmp_eq0000_wg_cy [3]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/delay_count_high_cmp_eq0000_wg_lut [4]),
    .O(\LTC_A/delay_count_high_cmp_eq0000_wg_cy [4])
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \LTC_A/delay_count_high_cmp_eq0000_wg_lut<5>  (
    .I0(\LTC_A/delay_count_low [23]),
    .I1(\LTC_A/delay_count_low [21]),
    .I2(\LTC_A/delay_count_low [2]),
    .I3(\LTC_A/delay_count_low [22]),
    .O(\LTC_A/delay_count_high_cmp_eq0000_wg_lut [5])
  );
  MUXCY   \LTC_A/delay_count_high_cmp_eq0000_wg_cy<5>  (
    .CI(\LTC_A/delay_count_high_cmp_eq0000_wg_cy [4]),
    .DI(tp2_o_OBUF_3479),
    .S(\LTC_A/delay_count_high_cmp_eq0000_wg_lut [5]),
    .O(\LTC_A/delay_count_high_cmp_eq0000 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \communication/state_FSM_Out51  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .O(\communication/state_cmp_eq0005 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Inst_ad5660_serial_DAC/state_FSM_FFd4-In1  (
    .I0(\VCXO_pll/update_dac_o_2309 ),
    .I1(\Inst_ad5660_serial_DAC/state_FSM_FFd5_113 ),
    .O(\Inst_ad5660_serial_DAC/state_FSM_FFd4-In )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \LTC_B/Mcount_delay_count_high_xor<1>11  (
    .I0(\LTC_B/delay_count_high [1]),
    .I1(\LTC_B/delay_count_high [0]),
    .O(\LTC_B/Result<1>1 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \LTC_A/Mcount_delay_count_high_xor<1>11  (
    .I0(\LTC_A/delay_count_high [1]),
    .I1(\LTC_A/delay_count_high [0]),
    .O(\LTC_A/Result<1>1 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \Inst_ad5660_serial_DAC/Mcount_dec_clk_count_xor<1>11  (
    .I0(\Inst_ad5660_serial_DAC/dec_clk_count [1]),
    .I1(\Inst_ad5660_serial_DAC/dec_clk_count [0]),
    .O(\Inst_ad5660_serial_DAC/Result [1])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \serial_interface/state_FSM_Out111  (
    .I0(\serial_interface/state_FSM_FFd4_3382 ),
    .I1(\serial_interface/state_FSM_FFd1_3351 ),
    .I2(\serial_interface/state_FSM_FFd3_3369 ),
    .O(\serial_interface/_mux0000 )
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \LTC_B/Mcount_delay_count_high_xor<2>11  (
    .I0(\LTC_B/delay_count_high [1]),
    .I1(\LTC_B/delay_count_high [2]),
    .I2(\LTC_B/delay_count_high [0]),
    .O(\LTC_B/Result<2>1 )
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \LTC_A/Mcount_delay_count_high_xor<2>11  (
    .I0(\LTC_A/delay_count_high [1]),
    .I1(\LTC_A/delay_count_high [2]),
    .I2(\LTC_A/delay_count_high [0]),
    .O(\LTC_A/Result<2>1 )
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \Inst_ad5660_serial_DAC/Mcount_dec_clk_count_xor<2>11  (
    .I0(\Inst_ad5660_serial_DAC/dec_clk_count [1]),
    .I1(\Inst_ad5660_serial_DAC/dec_clk_count [2]),
    .I2(\Inst_ad5660_serial_DAC/dec_clk_count [0]),
    .O(\Inst_ad5660_serial_DAC/Result [2])
  );
  LUT4 #(
    .INIT ( 16'h6AAA ))
  \LTC_B/Mcount_delay_count_high_xor<3>11  (
    .I0(\LTC_B/delay_count_high [3]),
    .I1(\LTC_B/delay_count_high [1]),
    .I2(\LTC_B/delay_count_high [0]),
    .I3(\LTC_B/delay_count_high [2]),
    .O(\LTC_B/Result<3>1 )
  );
  LUT4 #(
    .INIT ( 16'h6AAA ))
  \LTC_A/Mcount_delay_count_high_xor<3>11  (
    .I0(\LTC_A/delay_count_high [3]),
    .I1(\LTC_A/delay_count_high [1]),
    .I2(\LTC_A/delay_count_high [0]),
    .I3(\LTC_A/delay_count_high [2]),
    .O(\LTC_A/Result<3>1 )
  );
  LUT4 #(
    .INIT ( 16'h6AAA ))
  \Inst_ad5660_serial_DAC/Mcount_dec_clk_count_xor<3>11  (
    .I0(\Inst_ad5660_serial_DAC/dec_clk_count [3]),
    .I1(\Inst_ad5660_serial_DAC/dec_clk_count [1]),
    .I2(\Inst_ad5660_serial_DAC/dec_clk_count [0]),
    .I3(\Inst_ad5660_serial_DAC/dec_clk_count [2]),
    .O(\Inst_ad5660_serial_DAC/Result [3])
  );
  LUT2 #(
    .INIT ( 4'hE ))
  pps_status_count_or00001 (
    .I0(rst_i_IBUF_3067),
    .I1(PPS_i_IBUF_2001),
    .O(pps_status_count_or0000)
  );
  LUT3 #(
    .INIT ( 8'h08 ))
  \serial_interface/state_FSM_FFd1-In411  (
    .I0(\serial_interface/state_FSM_FFd3_3369 ),
    .I1(\serial_interface/state_FSM_FFd2_3359 ),
    .I2(\serial_interface/state_FSM_FFd4_3382 ),
    .O(\serial_interface/state_cmp_eq0021 )
  );
  LUT4 #(
    .INIT ( 16'h7F6F ))
  \serial_interface/bit_count_not00011  (
    .I0(\serial_interface/state_FSM_FFd4_3382 ),
    .I1(\serial_interface/state_FSM_FFd2_3359 ),
    .I2(\serial_interface/state_FSM_FFd3_3369 ),
    .I3(\serial_interface/state_FSM_FFd1_3351 ),
    .O(\serial_interface/bit_count_not0001 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/stop_o_not00011  (
    .I0(\serial_interface/state_FSM_FFd1_3351 ),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd4_3382 ),
    .I3(\serial_interface/state_FSM_FFd2_3359 ),
    .O(\serial_interface/stop_o_not0001 )
  );
  LUT2 #(
    .INIT ( 4'hE ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<10>11  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd3_107 ),
    .I1(\Inst_ad5660_serial_DAC/state_FSM_FFd1_103 ),
    .O(\Inst_ad5660_serial_DAC/N01 )
  );
  LUT3 #(
    .INIT ( 8'h28 ))
  \serial_interface/bit_count_mux0002<1>1  (
    .I0(\serial_interface/bit_count_or0002 ),
    .I1(\serial_interface/bit_count [1]),
    .I2(\serial_interface/bit_count [0]),
    .O(\serial_interface/bit_count_mux0002 [1])
  );
  LUT3 #(
    .INIT ( 8'h18 ))
  \serial_interface/bit_count_or00021  (
    .I0(\serial_interface/state_FSM_FFd4_3382 ),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd2_3359 ),
    .O(\serial_interface/bit_count_or0002 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<23>1  (
    .I0(\Inst_ad5660_serial_DAC/shift_register [22]),
    .I1(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I2(\Inst_ad5660_serial_DAC/shift_register [23]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000[23] )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<22>1  (
    .I0(\Inst_ad5660_serial_DAC/shift_register [21]),
    .I1(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I2(\Inst_ad5660_serial_DAC/shift_register [22]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000[22] )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<21>1  (
    .I0(\Inst_ad5660_serial_DAC/shift_register [20]),
    .I1(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I2(\Inst_ad5660_serial_DAC/shift_register [21]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000[21] )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<20>1  (
    .I0(\Inst_ad5660_serial_DAC/shift_register [19]),
    .I1(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I2(\Inst_ad5660_serial_DAC/shift_register [20]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000[20] )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<19>1  (
    .I0(\Inst_ad5660_serial_DAC/shift_register [18]),
    .I1(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I2(\Inst_ad5660_serial_DAC/shift_register [19]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000[19] )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<18>1  (
    .I0(\Inst_ad5660_serial_DAC/shift_register [17]),
    .I1(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I2(\Inst_ad5660_serial_DAC/shift_register [18]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000[18] )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<17>1  (
    .I0(\Inst_ad5660_serial_DAC/shift_register [16]),
    .I1(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I2(\Inst_ad5660_serial_DAC/shift_register [17]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000[17] )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<16>1  (
    .I0(\Inst_ad5660_serial_DAC/shift_register [15]),
    .I1(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I2(\Inst_ad5660_serial_DAC/shift_register [16]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000[16] )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<0>1  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I1(\VCXO_pll/cv_word_o [0]),
    .I2(\Inst_ad5660_serial_DAC/shift_register [0]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000[0] )
  );
  LUT4 #(
    .INIT ( 16'h28A0 ))
  \serial_interface/bit_count_mux0002<2>1  (
    .I0(\serial_interface/bit_count_or0002 ),
    .I1(\serial_interface/bit_count [0]),
    .I2(\serial_interface/bit_count [2]),
    .I3(\serial_interface/bit_count [1]),
    .O(\serial_interface/bit_count_mux0002 [2])
  );
  LUT4 #(
    .INIT ( 16'hBE14 ))
  \LTC_B/LTC_frame_count/Mcount_sec_units_xor<1>11  (
    .I0(\LTC_B/LTC_frame_count/sec_tens_and0001 ),
    .I1(\LTC_B/LTC_frame_count/sec_units [0]),
    .I2(\LTC_B/LTC_frame_count/sec_units [1]),
    .I3(LTC_b_secs[1]),
    .O(\LTC_B/LTC_frame_count/Mcount_sec_units1 )
  );
  LUT4 #(
    .INIT ( 16'hBE14 ))
  \LTC_B/LTC_frame_count/Mcount_sec_tens_xor<1>11  (
    .I0(\LTC_B/LTC_frame_count/sec_tens_and0001 ),
    .I1(\LTC_B/LTC_frame_count/sec_tens [0]),
    .I2(\LTC_B/LTC_frame_count/sec_tens [1]),
    .I3(LTC_b_secs[5]),
    .O(\LTC_B/LTC_frame_count/Mcount_sec_tens1 )
  );
  LUT4 #(
    .INIT ( 16'hBE14 ))
  \LTC_B/LTC_frame_count/Mcount_min_units_xor<1>11  (
    .I0(\LTC_B/LTC_frame_count/sec_tens_and0001 ),
    .I1(\LTC_B/LTC_frame_count/min_units [0]),
    .I2(\LTC_B/LTC_frame_count/min_units [1]),
    .I3(LTC_b_mins[1]),
    .O(\LTC_B/LTC_frame_count/Mcount_min_units1 )
  );
  LUT4 #(
    .INIT ( 16'hBE14 ))
  \LTC_B/LTC_frame_count/Mcount_min_tens_xor<1>11  (
    .I0(\LTC_B/LTC_frame_count/sec_tens_and0001 ),
    .I1(\LTC_B/LTC_frame_count/min_tens [0]),
    .I2(\LTC_B/LTC_frame_count/min_tens [1]),
    .I3(LTC_b_mins[5]),
    .O(\LTC_B/LTC_frame_count/Mcount_min_tens1 )
  );
  LUT4 #(
    .INIT ( 16'hBE14 ))
  \LTC_B/LTC_frame_count/Mcount_hour_units_xor<1>11  (
    .I0(\LTC_B/LTC_frame_count/sec_tens_and0001 ),
    .I1(\LTC_B/LTC_frame_count/hour_units [0]),
    .I2(\LTC_B/LTC_frame_count/hour_units [1]),
    .I3(LTC_b_hours[1]),
    .O(\LTC_B/LTC_frame_count/Mcount_hour_units1 )
  );
  LUT4 #(
    .INIT ( 16'hBE14 ))
  \LTC_A/LTC_frame_count/Mcount_sec_units_xor<1>11  (
    .I0(\LTC_A/LTC_frame_count/sec_tens_and0001 ),
    .I1(\LTC_A/LTC_frame_count/sec_units [0]),
    .I2(\LTC_A/LTC_frame_count/sec_units [1]),
    .I3(LTC_a_secs[1]),
    .O(\LTC_A/LTC_frame_count/Mcount_sec_units1 )
  );
  LUT4 #(
    .INIT ( 16'hBE14 ))
  \LTC_A/LTC_frame_count/Mcount_sec_tens_xor<1>11  (
    .I0(\LTC_A/LTC_frame_count/sec_tens_and0001 ),
    .I1(\LTC_A/LTC_frame_count/sec_tens [0]),
    .I2(\LTC_A/LTC_frame_count/sec_tens [1]),
    .I3(LTC_a_secs[5]),
    .O(\LTC_A/LTC_frame_count/Mcount_sec_tens1 )
  );
  LUT4 #(
    .INIT ( 16'hBE14 ))
  \LTC_A/LTC_frame_count/Mcount_min_units_xor<1>11  (
    .I0(\LTC_A/LTC_frame_count/sec_tens_and0001 ),
    .I1(\LTC_A/LTC_frame_count/min_units [0]),
    .I2(\LTC_A/LTC_frame_count/min_units [1]),
    .I3(LTC_a_mins[1]),
    .O(\LTC_A/LTC_frame_count/Mcount_min_units1 )
  );
  LUT4 #(
    .INIT ( 16'hBE14 ))
  \LTC_A/LTC_frame_count/Mcount_min_tens_xor<1>11  (
    .I0(\LTC_A/LTC_frame_count/sec_tens_and0001 ),
    .I1(\LTC_A/LTC_frame_count/min_tens [0]),
    .I2(\LTC_A/LTC_frame_count/min_tens [1]),
    .I3(LTC_a_mins[5]),
    .O(\LTC_A/LTC_frame_count/Mcount_min_tens1 )
  );
  LUT4 #(
    .INIT ( 16'hBE14 ))
  \LTC_A/LTC_frame_count/Mcount_hour_units_xor<1>11  (
    .I0(\LTC_A/LTC_frame_count/sec_tens_and0001 ),
    .I1(\LTC_A/LTC_frame_count/hour_units [0]),
    .I2(\LTC_A/LTC_frame_count/hour_units [1]),
    .I3(LTC_a_hours[1]),
    .O(\LTC_A/LTC_frame_count/Mcount_hour_units1 )
  );
  LUT4 #(
    .INIT ( 16'hD8A8 ))
  \Inst_ad5660_serial_DAC/bit_count_mux0000<3>1  (
    .I0(\Inst_ad5660_serial_DAC/bit_count [1]),
    .I1(\Inst_ad5660_serial_DAC/N4 ),
    .I2(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I3(\Inst_ad5660_serial_DAC/bit_count [0]),
    .O(\Inst_ad5660_serial_DAC/bit_count_mux0000 [3])
  );
  LUT4 #(
    .INIT ( 16'hA8D8 ))
  \Inst_ad5660_serial_DAC/bit_count_mux0000<1>2  (
    .I0(\Inst_ad5660_serial_DAC/bit_count [3]),
    .I1(\Inst_ad5660_serial_DAC/N4 ),
    .I2(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I3(\Inst_ad5660_serial_DAC/N3 ),
    .O(\Inst_ad5660_serial_DAC/bit_count_mux0000 [1])
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<9>_SW0  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I1(\Inst_ad5660_serial_DAC/shift_register [8]),
    .I2(\Inst_ad5660_serial_DAC/shift_register [9]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(N4)
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<8>_SW0  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I1(\Inst_ad5660_serial_DAC/shift_register [7]),
    .I2(\Inst_ad5660_serial_DAC/shift_register [8]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(N6)
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<7>_SW0  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I1(\Inst_ad5660_serial_DAC/shift_register [6]),
    .I2(\Inst_ad5660_serial_DAC/shift_register [7]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(N8)
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<6>_SW0  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I1(\Inst_ad5660_serial_DAC/shift_register [5]),
    .I2(\Inst_ad5660_serial_DAC/shift_register [6]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(N10)
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<5>_SW0  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I1(\Inst_ad5660_serial_DAC/shift_register [4]),
    .I2(\Inst_ad5660_serial_DAC/shift_register [5]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(N12)
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<4>_SW0  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I1(\Inst_ad5660_serial_DAC/shift_register [3]),
    .I2(\Inst_ad5660_serial_DAC/shift_register [4]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(N141)
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<3>_SW0  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I1(\Inst_ad5660_serial_DAC/shift_register [2]),
    .I2(\Inst_ad5660_serial_DAC/shift_register [3]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(N161)
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<2>_SW0  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I1(\Inst_ad5660_serial_DAC/shift_register [1]),
    .I2(\Inst_ad5660_serial_DAC/shift_register [2]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(N181)
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<1>_SW0  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I1(\Inst_ad5660_serial_DAC/shift_register [0]),
    .I2(\Inst_ad5660_serial_DAC/shift_register [1]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(N201)
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<15>_SW0  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I1(\Inst_ad5660_serial_DAC/shift_register [14]),
    .I2(\Inst_ad5660_serial_DAC/shift_register [15]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(N221)
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<14>_SW0  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I1(\Inst_ad5660_serial_DAC/shift_register [13]),
    .I2(\Inst_ad5660_serial_DAC/shift_register [14]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(N24)
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<13>_SW0  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I1(\Inst_ad5660_serial_DAC/shift_register [12]),
    .I2(\Inst_ad5660_serial_DAC/shift_register [13]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(N261)
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<12>_SW0  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I1(\Inst_ad5660_serial_DAC/shift_register [11]),
    .I2(\Inst_ad5660_serial_DAC/shift_register [12]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(N28)
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<11>_SW0  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I1(\Inst_ad5660_serial_DAC/shift_register [10]),
    .I2(\Inst_ad5660_serial_DAC/shift_register [11]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(N30)
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<10>_SW0  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I1(\Inst_ad5660_serial_DAC/shift_register [9]),
    .I2(\Inst_ad5660_serial_DAC/shift_register [10]),
    .I3(\Inst_ad5660_serial_DAC/N01 ),
    .O(N321)
  );
  LUT3 #(
    .INIT ( 8'hFE ))
  \Inst_ad5660_serial_DAC/state_FSM_FFd3-In2  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I1(\Inst_ad5660_serial_DAC/state_FSM_FFd1_103 ),
    .I2(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .O(\Inst_ad5660_serial_DAC/state_FSM_FFd3-In2_110 )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  genlock_register_9_and00001 (
    .I0(\communication/address_o [1]),
    .I1(N20),
    .I2(\communication/address_o [0]),
    .I3(\communication/update_register_o_2379 ),
    .O(genlock_register_9_and0000)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  genlock_register_6_and00001 (
    .I0(\communication/address_o [0]),
    .I1(N17),
    .I2(\communication/update_register_o_2379 ),
    .I3(\communication/address_o [1]),
    .O(genlock_register_6_and0000)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  genlock_register_5_and00001 (
    .I0(\communication/address_o [1]),
    .I1(N17),
    .I2(\communication/address_o [0]),
    .I3(\communication/update_register_o_2379 ),
    .O(genlock_register_5_and0000)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  genlock_register_2_and00001 (
    .I0(\communication/address_o [0]),
    .I1(N14),
    .I2(\communication/update_register_o_2379 ),
    .I3(\communication/address_o [1]),
    .O(genlock_register_2_and0000)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  genlock_register_22_and00001 (
    .I0(\communication/address_o [0]),
    .I1(N21),
    .I2(\communication/update_register_o_2379 ),
    .I3(\communication/address_o [1]),
    .O(genlock_register_22_and0000)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  genlock_register_21_and00001 (
    .I0(\communication/address_o [1]),
    .I1(N21),
    .I2(\communication/address_o [0]),
    .I3(\communication/update_register_o_2379 ),
    .O(genlock_register_21_and0000)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  genlock_register_1_and00001 (
    .I0(\communication/address_o [1]),
    .I1(N14),
    .I2(\communication/address_o [0]),
    .I3(\communication/update_register_o_2379 ),
    .O(genlock_register_1_and0000)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  genlock_register_18_and00001 (
    .I0(\communication/address_o [0]),
    .I1(N18),
    .I2(\communication/update_register_o_2379 ),
    .I3(\communication/address_o [1]),
    .O(genlock_register_18_and0000)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  genlock_register_17_and00001 (
    .I0(\communication/address_o [1]),
    .I1(N18),
    .I2(\communication/address_o [0]),
    .I3(\communication/update_register_o_2379 ),
    .O(genlock_register_17_and0000)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  genlock_register_14_and00001 (
    .I0(\communication/address_o [0]),
    .I1(N19),
    .I2(\communication/update_register_o_2379 ),
    .I3(\communication/address_o [1]),
    .O(genlock_register_14_and0000)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  genlock_register_13_and00001 (
    .I0(\communication/address_o [1]),
    .I1(N19),
    .I2(\communication/address_o [0]),
    .I3(\communication/update_register_o_2379 ),
    .O(genlock_register_13_and0000)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  genlock_register_10_and00001 (
    .I0(\communication/address_o [0]),
    .I1(N20),
    .I2(\communication/update_register_o_2379 ),
    .I3(\communication/address_o [1]),
    .O(genlock_register_10_and0000)
  );
  LUT4 #(
    .INIT ( 16'hFF80 ))
  \LTC_B/LTC_frame_count/min_units_not00011  (
    .I0(\LTC_B/LTC_frame_count/N12 ),
    .I1(\LTC_B/LTC_frame_count/count_sec_740 ),
    .I2(\LTC_B/LTC_frame_count/N49 ),
    .I3(\LTC_B/LTC_frame_count/sec_tens_and0001 ),
    .O(\LTC_B/LTC_frame_count/min_units_not0001 )
  );
  LUT4 #(
    .INIT ( 16'hFF80 ))
  \LTC_A/LTC_frame_count/min_units_not00011  (
    .I0(\LTC_A/LTC_frame_count/N12 ),
    .I1(\LTC_A/LTC_frame_count/count_sec_306 ),
    .I2(\LTC_A/LTC_frame_count/N49 ),
    .I3(\LTC_A/LTC_frame_count/sec_tens_and0001 ),
    .O(\LTC_A/LTC_frame_count/min_units_not0001 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \serial_interface/state_cmp_eq00011  (
    .I0(\serial_interface/SCL_delay [0]),
    .I1(\serial_interface/SCL_delay [1]),
    .O(\serial_interface/state_cmp_eq0001 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Inst_ad5660_serial_DAC/state_FSM_FFd1-In1  (
    .I0(\Inst_ad5660_serial_DAC/sclk_53 ),
    .I1(\Inst_ad5660_serial_DAC/N5 ),
    .O(\Inst_ad5660_serial_DAC/state_FSM_FFd1-In )
  );
  LUT3 #(
    .INIT ( 8'h01 ))
  \Inst_ad5660_serial_DAC/bit_count_mux0000<2>11  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd5_113 ),
    .I1(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I2(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .O(\Inst_ad5660_serial_DAC/N4 )
  );
  LUT4 #(
    .INIT ( 16'hBFFF ))
  \serial_interface/send_byte_mux0001<0>11  (
    .I0(\serial_interface/state_FSM_FFd1_3351 ),
    .I1(\serial_interface/state_FSM_FFd2_3359 ),
    .I2(\serial_interface/state_FSM_FFd4_3382 ),
    .I3(\serial_interface/state_FSM_FFd3_3369 ),
    .O(N2)
  );
  LUT4 #(
    .INIT ( 16'h7614 ))
  \serial_interface/recieve_byte_0_mux0001511  (
    .I0(\serial_interface/state_FSM_FFd4_3382 ),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd1_3351 ),
    .I3(\serial_interface/state_FSM_FFd2_3359 ),
    .O(N64)
  );
  LUT2 #(
    .INIT ( 4'hD ))
  \Inst_ad5660_serial_DAC/state_FSM_FFd2-In_SW0  (
    .I0(\Inst_ad5660_serial_DAC/N5 ),
    .I1(\Inst_ad5660_serial_DAC/sclk_53 ),
    .O(N36)
  );
  LUT4 #(
    .INIT ( 16'hFFDF ))
  \Inst_ad5660_serial_DAC/state_FSM_FFd5-In_SW0  (
    .I0(\Inst_ad5660_serial_DAC/bit_count [4]),
    .I1(\Inst_ad5660_serial_DAC/sclk_53 ),
    .I2(\Inst_ad5660_serial_DAC/N5 ),
    .I3(\Inst_ad5660_serial_DAC/bit_count [3]),
    .O(N63)
  );
  LUT4 #(
    .INIT ( 16'h11F1 ))
  \Inst_ad5660_serial_DAC/state_FSM_FFd5-In  (
    .I0(N63),
    .I1(\Inst_ad5660_serial_DAC/N3 ),
    .I2(\Inst_ad5660_serial_DAC/state_FSM_FFd5_113 ),
    .I3(\VCXO_pll/update_dac_o_2309 ),
    .O(\Inst_ad5660_serial_DAC/state_FSM_FFd5-In_114 )
  );
  LUT4 #(
    .INIT ( 16'hFFDF ))
  \LTC_B/LTC_frame_count/frame_units_mux0009<0>9  (
    .I0(\LTC_B/LTC_frame_count/frame_tens [1]),
    .I1(\LTC_B/LTC_frame_count/frame_units [3]),
    .I2(\LTC_setup[4] ),
    .I3(\LTC_B/LTC_frame_count/frame_units [1]),
    .O(\LTC_B/LTC_frame_count/frame_units_mux0009<0>9_764 )
  );
  LUT4 #(
    .INIT ( 16'hFFDF ))
  \LTC_A/LTC_frame_count/frame_units_mux0009<0>9  (
    .I0(\LTC_A/LTC_frame_count/frame_tens [1]),
    .I1(\LTC_A/LTC_frame_count/frame_units [3]),
    .I2(\LTC_setup[0] ),
    .I3(\LTC_A/LTC_frame_count/frame_units [1]),
    .O(\LTC_A/LTC_frame_count/frame_units_mux0009<0>9_330 )
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \communication/state_and000011  (
    .I0(\communication/update_line [1]),
    .I1(\communication/update_line [0]),
    .I2(\communication/state_FSM_FFd2_2370 ),
    .O(\communication/state_FSM_FFd2-In )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \LTC_B/LTC_frame_count/sec_tens_and00001  (
    .I0(\LTC_B/LTC_frame_count/sec_tens_and0001 ),
    .I1(\LTC_B/LTC_frame_count/N49 ),
    .I2(\LTC_B/LTC_frame_count/N12 ),
    .I3(\LTC_B/LTC_frame_count/count_sec_740 ),
    .O(\LTC_B/LTC_frame_count/sec_tens_and0000 )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \LTC_A/LTC_frame_count/sec_tens_and00001  (
    .I0(\LTC_A/LTC_frame_count/sec_tens_and0001 ),
    .I1(\LTC_A/LTC_frame_count/N49 ),
    .I2(\LTC_A/LTC_frame_count/N12 ),
    .I3(\LTC_A/LTC_frame_count/count_sec_306 ),
    .O(\LTC_A/LTC_frame_count/sec_tens_and0000 )
  );
  LUT4 #(
    .INIT ( 16'h2422 ))
  \LTC_B/LTC_frame_count/hour_units_and000011  (
    .I0(\LTC_B/LTC_frame_count/hour_units [3]),
    .I1(\LTC_B/LTC_frame_count/hour_units [1]),
    .I2(\LTC_B/LTC_frame_count/hour_tens [0]),
    .I3(\LTC_B/LTC_frame_count/hour_tens [1]),
    .O(\LTC_B/LTC_frame_count/N01 )
  );
  LUT4 #(
    .INIT ( 16'h2422 ))
  \LTC_A/LTC_frame_count/hour_units_and000011  (
    .I0(\LTC_A/LTC_frame_count/hour_units [3]),
    .I1(\LTC_A/LTC_frame_count/hour_units [1]),
    .I2(\LTC_A/LTC_frame_count/hour_tens [0]),
    .I3(\LTC_A/LTC_frame_count/hour_tens [1]),
    .O(\LTC_A/LTC_frame_count/N01 )
  );
  LUT4 #(
    .INIT ( 16'h0010 ))
  \serial_interface/recieve_byte_6_cmp_eq00001  (
    .I0(\serial_interface/bit_count [1]),
    .I1(\serial_interface/bit_count [3]),
    .I2(\serial_interface/bit_count [0]),
    .I3(\serial_interface/bit_count [2]),
    .O(\serial_interface/recieve_byte_6_cmp_eq0000 )
  );
  LUT4 #(
    .INIT ( 16'hFFDF ))
  \LTC_B/LTC_frame_count/frame_tens_mux0002<4>11  (
    .I0(\LTC_B/LTC_frame_count/frame_units [3]),
    .I1(\LTC_B/LTC_frame_count/frame_units [2]),
    .I2(\LTC_B/LTC_frame_count/frame_units [0]),
    .I3(\LTC_B/LTC_frame_count/frame_units [1]),
    .O(\LTC_B/LTC_frame_count/N5 )
  );
  LUT4 #(
    .INIT ( 16'hFFDF ))
  \LTC_A/LTC_frame_count/frame_tens_mux0002<4>11  (
    .I0(\LTC_A/LTC_frame_count/frame_units [3]),
    .I1(\LTC_A/LTC_frame_count/frame_units [2]),
    .I2(\LTC_A/LTC_frame_count/frame_units [0]),
    .I3(\LTC_A/LTC_frame_count/frame_units [1]),
    .O(\LTC_A/LTC_frame_count/N5 )
  );
  LUT4 #(
    .INIT ( 16'h8999 ))
  \LTC_B/LTC_frame_count/frame_tens_mux0002<4>_SW0  (
    .I0(\LTC_B/LTC_frame_count/N5 ),
    .I1(\LTC_B/LTC_frame_count/frame_tens [0]),
    .I2(\LTC_setup[5] ),
    .I3(\LTC_B/LTC_frame_count/frame_tens [1]),
    .O(N74)
  );
  LUT4 #(
    .INIT ( 16'h8999 ))
  \LTC_A/LTC_frame_count/frame_tens_mux0002<4>_SW0  (
    .I0(\LTC_A/LTC_frame_count/N5 ),
    .I1(\LTC_A/LTC_frame_count/frame_tens [0]),
    .I2(\LTC_setup[1] ),
    .I3(\LTC_A/LTC_frame_count/frame_tens [1]),
    .O(N76)
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \LTC_B/LTC_clockgenerator/ltc_bitcount_or000014  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [0]),
    .I1(\LTC_B/LTC_clockgenerator/ltc_bitcount [7]),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount [1]),
    .I3(\LTC_B/LTC_clockgenerator/ltc_bitcount [2]),
    .O(\LTC_B/LTC_clockgenerator/ltc_bitcount_or000014_660 )
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  \LTC_B/LTC_clockgenerator/ltc_bitcount_or0000113  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [3]),
    .I1(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount [4]),
    .I3(\LTC_B/LTC_clockgenerator/ltc_bitcount [6]),
    .O(\LTC_B/LTC_clockgenerator/ltc_bitcount_or0000113_659 )
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \LTC_A/LTC_clockgenerator/ltc_bitcount_or000014  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [0]),
    .I1(\LTC_A/LTC_clockgenerator/ltc_bitcount [7]),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount [1]),
    .I3(\LTC_A/LTC_clockgenerator/ltc_bitcount [2]),
    .O(\LTC_A/LTC_clockgenerator/ltc_bitcount_or000014_226 )
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  \LTC_A/LTC_clockgenerator/ltc_bitcount_or0000113  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [3]),
    .I1(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount [4]),
    .I3(\LTC_A/LTC_clockgenerator/ltc_bitcount [6]),
    .O(\LTC_A/LTC_clockgenerator/ltc_bitcount_or0000113_225 )
  );
  LUT3 #(
    .INIT ( 8'hC9 ))
  \LTC_B/LTC_biphase_generator/Madd_ltc_frame_i_sub0000_xor<6>11  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_B/LTC_clockgenerator/ltc_bitcount [7]),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount [6]),
    .O(\LTC_B/LTC_biphase_generator/ltc_frame_i_sub0000 [6])
  );
  LUT3 #(
    .INIT ( 8'hC9 ))
  \LTC_A/LTC_biphase_generator/Madd_ltc_frame_i_sub0000_xor<6>11  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_A/LTC_clockgenerator/ltc_bitcount [7]),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount [6]),
    .O(\LTC_A/LTC_biphase_generator/ltc_frame_i_sub0000 [6])
  );
  LUT4 #(
    .INIT ( 16'h0420 ))
  \LTC_B/LTC_frame_count/count_sec_or000032  (
    .I0(\LTC_setup[4] ),
    .I1(\LTC_B/LTC_frame_count/frame_units [0]),
    .I2(\LTC_B/LTC_frame_count/frame_units [2]),
    .I3(\LTC_B/LTC_frame_count/frame_units [1]),
    .O(\LTC_B/LTC_frame_count/count_sec_or000032_742 )
  );
  LUT4 #(
    .INIT ( 16'h0420 ))
  \LTC_A/LTC_frame_count/count_sec_or000032  (
    .I0(\LTC_setup[0] ),
    .I1(\LTC_A/LTC_frame_count/frame_units [0]),
    .I2(\LTC_A/LTC_frame_count/frame_units [2]),
    .I3(\LTC_A/LTC_frame_count/frame_units [1]),
    .O(\LTC_A/LTC_frame_count/count_sec_or000032_308 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \serial_interface/time_out_mux0009<21>1  (
    .I0(\serial_interface/time_out_or0000_3470 ),
    .I1(\serial_interface/time_out_addsub0000 [0]),
    .O(\serial_interface/time_out_mux0009[21] )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \serial_interface/time_out_mux0009<20>1  (
    .I0(\serial_interface/time_out_or0000_3470 ),
    .I1(\serial_interface/time_out_addsub0000 [1]),
    .O(\serial_interface/time_out_mux0009[20] )
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \serial_interface/recieve_byte_4_cmp_eq000011  (
    .I0(\serial_interface/bit_count [3]),
    .I1(\serial_interface/bit_count [1]),
    .I2(\serial_interface/bit_count [2]),
    .O(N86)
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \serial_interface/recieve_byte_2_cmp_eq000011  (
    .I0(\serial_interface/bit_count [3]),
    .I1(\serial_interface/bit_count [2]),
    .I2(\serial_interface/bit_count [1]),
    .O(N85)
  );
  LUT3 #(
    .INIT ( 8'h7F ))
  \Inst_ad5660_serial_DAC/bit_count_mux0000<1>11  (
    .I0(\Inst_ad5660_serial_DAC/bit_count [2]),
    .I1(\Inst_ad5660_serial_DAC/bit_count [1]),
    .I2(\Inst_ad5660_serial_DAC/bit_count [0]),
    .O(\Inst_ad5660_serial_DAC/N3 )
  );
  LUT4 #(
    .INIT ( 16'hD8A8 ))
  \Inst_ad5660_serial_DAC/bit_count_mux0000<0>  (
    .I0(\Inst_ad5660_serial_DAC/bit_count [4]),
    .I1(\Inst_ad5660_serial_DAC/N4 ),
    .I2(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I3(N92),
    .O(\Inst_ad5660_serial_DAC/bit_count_mux0000 [0])
  );
  LUT4 #(
    .INIT ( 16'h68AA ))
  \LTC_B/LTC_frame_count/frame_units_mux0009<3>_SW0  (
    .I0(\LTC_B/LTC_frame_count/frame_units [3]),
    .I1(\LTC_B/LTC_frame_count/frame_units [2]),
    .I2(\LTC_B/LTC_frame_count/frame_units [1]),
    .I3(\LTC_B/LTC_frame_count/frame_units [0]),
    .O(N94)
  );
  LUT4 #(
    .INIT ( 16'h68AA ))
  \LTC_A/LTC_frame_count/frame_units_mux0009<3>_SW0  (
    .I0(\LTC_A/LTC_frame_count/frame_units [3]),
    .I1(\LTC_A/LTC_frame_count/frame_units [2]),
    .I2(\LTC_A/LTC_frame_count/frame_units [1]),
    .I3(\LTC_A/LTC_frame_count/frame_units [0]),
    .O(N96)
  );
  LUT4 #(
    .INIT ( 16'hAA20 ))
  \serial_interface/recieve_byte_7_mux0001_SW0  (
    .I0(\serial_interface/recieve_byte [7]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd2_3359 ),
    .I3(N64),
    .O(N98)
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \serial_interface/recieve_byte_7_mux0001_SW1  (
    .I0(N15),
    .I1(\serial_interface/recieve_byte [7]),
    .I2(N64),
    .O(N991)
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \serial_interface/recieve_byte_7_mux0001  (
    .I0(N87),
    .I1(\serial_interface/bit_count [3]),
    .I2(N991),
    .I3(N98),
    .O(\serial_interface/recieve_byte_7_mux0001_3164 )
  );
  LUT4 #(
    .INIT ( 16'hAA20 ))
  \serial_interface/recieve_byte_5_mux0001_SW0  (
    .I0(\serial_interface/recieve_byte [5]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd2_3359 ),
    .I3(N64),
    .O(N101)
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \serial_interface/recieve_byte_5_mux0001_SW1  (
    .I0(N15),
    .I1(\serial_interface/recieve_byte [5]),
    .I2(N64),
    .O(N102)
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \serial_interface/recieve_byte_5_mux0001  (
    .I0(N86),
    .I1(\serial_interface/bit_count [0]),
    .I2(N102),
    .I3(N101),
    .O(\serial_interface/recieve_byte_5_mux0001_3159 )
  );
  LUT4 #(
    .INIT ( 16'hAA20 ))
  \serial_interface/recieve_byte_4_mux0001_SW0  (
    .I0(\serial_interface/recieve_byte [4]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd2_3359 ),
    .I3(N64),
    .O(N104)
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \serial_interface/recieve_byte_4_mux0001_SW1  (
    .I0(N15),
    .I1(\serial_interface/recieve_byte [4]),
    .I2(N64),
    .O(N105)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \serial_interface/recieve_byte_4_mux0001  (
    .I0(\serial_interface/bit_count [0]),
    .I1(N104),
    .I2(N86),
    .I3(N105),
    .O(\serial_interface/recieve_byte_4_mux0001_3157 )
  );
  LUT4 #(
    .INIT ( 16'hAA20 ))
  \serial_interface/recieve_byte_3_mux0001_SW0  (
    .I0(\serial_interface/recieve_byte [3]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd2_3359 ),
    .I3(N64),
    .O(N107)
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \serial_interface/recieve_byte_3_mux0001_SW1  (
    .I0(N15),
    .I1(\serial_interface/recieve_byte [3]),
    .I2(N64),
    .O(N108)
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \serial_interface/recieve_byte_3_mux0001  (
    .I0(N85),
    .I1(\serial_interface/bit_count [0]),
    .I2(N108),
    .I3(N107),
    .O(\serial_interface/recieve_byte_3_mux0001_3155 )
  );
  LUT4 #(
    .INIT ( 16'hAA20 ))
  \serial_interface/recieve_byte_2_mux0001_SW0  (
    .I0(\serial_interface/recieve_byte [2]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd2_3359 ),
    .I3(N64),
    .O(N110)
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \serial_interface/recieve_byte_2_mux0001_SW1  (
    .I0(N15),
    .I1(\serial_interface/recieve_byte [2]),
    .I2(N64),
    .O(N111)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \serial_interface/recieve_byte_2_mux0001  (
    .I0(\serial_interface/bit_count [0]),
    .I1(N110),
    .I2(N85),
    .I3(N111),
    .O(\serial_interface/recieve_byte_2_mux0001_3153 )
  );
  LUT4 #(
    .INIT ( 16'hAA20 ))
  \serial_interface/recieve_byte_1_mux0001_SW0  (
    .I0(\serial_interface/recieve_byte [1]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd2_3359 ),
    .I3(N64),
    .O(N113)
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \serial_interface/recieve_byte_1_mux0001_SW1  (
    .I0(N15),
    .I1(\serial_interface/recieve_byte [1]),
    .I2(N64),
    .O(N114)
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \serial_interface/recieve_byte_1_mux0001  (
    .I0(N84),
    .I1(\serial_interface/bit_count [0]),
    .I2(N114),
    .I3(N113),
    .O(\serial_interface/recieve_byte_1_mux0001_3151 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \serial_interface/time_out_mux0009<18>1  (
    .I0(\serial_interface/time_out_or0000_3470 ),
    .I1(\serial_interface/time_out_addsub0000 [3]),
    .O(\serial_interface/time_out_mux0009[18] )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \VCXO_pll/phase_detector/phase_counter_cmp_eq00007  (
    .I0(\VCXO_pll/phase_detector/phase_counter [5]),
    .I1(\VCXO_pll/phase_detector/phase_counter [6]),
    .I2(\VCXO_pll/phase_detector/phase_counter [4]),
    .I3(\VCXO_pll/phase_detector/phase_counter [7]),
    .O(\VCXO_pll/phase_detector/phase_counter_cmp_eq00007_2277 )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \VCXO_pll/phase_detector/phase_counter_cmp_eq000018  (
    .I0(\VCXO_pll/phase_detector/phase_counter [3]),
    .I1(\VCXO_pll/phase_detector/phase_counter [0]),
    .I2(\VCXO_pll/phase_detector/phase_counter [2]),
    .I3(\VCXO_pll/phase_detector/phase_counter [1]),
    .O(\VCXO_pll/phase_detector/phase_counter_cmp_eq000018_2276 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \serial_interface/time_out_mux0009<12>1  (
    .I0(\serial_interface/time_out_or0000_3470 ),
    .I1(\serial_interface/time_out_addsub0000 [9]),
    .O(\serial_interface/time_out_mux0009[12] )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \LTC_B/LTC_frame_count/sec_tens_and00011  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I1(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .O(\LTC_B/LTC_frame_count/sec_tens_and0001 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \LTC_A/LTC_frame_count/sec_tens_and00011  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I1(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .O(\LTC_A/LTC_frame_count/sec_tens_and0001 )
  );
  LUT3 #(
    .INIT ( 8'h4E ))
  \h_gen/count_mux0003<4>1  (
    .I0(\h_gen/pulse_state_cmp_eq0000 ),
    .I1(\h_gen/count_addsub0000 [4]),
    .I2(\h_gen/pulse_state_3000 ),
    .O(\h_gen/count_mux0003[4] )
  );
  LUT3 #(
    .INIT ( 8'h4E ))
  \h_gen/count_mux0003<3>1  (
    .I0(\h_gen/pulse_state_cmp_eq0000 ),
    .I1(\h_gen/count_addsub0000 [3]),
    .I2(\h_gen/pulse_state_3000 ),
    .O(\h_gen/count_mux0003[3] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \h_gen/count_mux0003<2>1  (
    .I0(\h_gen/pulse_state_cmp_eq0000 ),
    .I1(\h_gen/count_addsub0000 [2]),
    .I2(\h_gen/pulse_state_3000 ),
    .O(\h_gen/count_mux0003[2] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \f_gen/count_mux0003<8>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/count_addsub0000 [8]),
    .I2(\system_control[0] ),
    .O(\f_gen/count_mux0003[8] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \f_gen/count_mux0003<6>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/count_addsub0000 [6]),
    .I2(\f_gen/pulse_state_2639 ),
    .O(\f_gen/count_mux0003[6] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \f_gen/count_mux0003<4>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/count_addsub0000 [4]),
    .I2(\f_gen/pulse_state_2639 ),
    .O(\f_gen/count_mux0003[4] )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \LTC_B/LTC_frame_count/hour_tens_mux0000<5>1  (
    .I0(LTC_b_hours[5]),
    .I1(\LTC_B/LTC_frame_count/sec_tens_and0001 ),
    .I2(\LTC_B/LTC_frame_count/hour_tens [0]),
    .I3(\LTC_B/LTC_frame_count/N50 ),
    .O(\LTC_B/LTC_frame_count/hour_tens_mux0000 [5])
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  \LTC_B/LTC_frame_count/hour_tens_mux0000<4>11  (
    .I0(\LTC_B/LTC_frame_count/hour_units [3]),
    .I1(\LTC_B/LTC_frame_count/hour_units [1]),
    .I2(\LTC_B/LTC_frame_count/N13 ),
    .I3(\LTC_B/LTC_frame_count/hour_tens [1]),
    .O(\LTC_B/LTC_frame_count/N50 )
  );
  LUT4 #(
    .INIT ( 16'hF222 ))
  \LTC_B/LTC_frame_count/hour_tens_mux0000<4>1  (
    .I0(\LTC_B/LTC_frame_count/N50 ),
    .I1(\LTC_B/LTC_frame_count/hour_tens [0]),
    .I2(LTC_b_hours[4]),
    .I3(\LTC_B/LTC_frame_count/sec_tens_and0001 ),
    .O(\LTC_B/LTC_frame_count/hour_tens_mux0000 [4])
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \LTC_A/LTC_frame_count/hour_tens_mux0000<5>1  (
    .I0(LTC_a_hours[5]),
    .I1(\LTC_A/LTC_frame_count/sec_tens_and0001 ),
    .I2(\LTC_A/LTC_frame_count/hour_tens [0]),
    .I3(\LTC_A/LTC_frame_count/N50 ),
    .O(\LTC_A/LTC_frame_count/hour_tens_mux0000 [5])
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  \LTC_A/LTC_frame_count/hour_tens_mux0000<4>11  (
    .I0(\LTC_A/LTC_frame_count/hour_units [3]),
    .I1(\LTC_A/LTC_frame_count/hour_units [1]),
    .I2(\LTC_A/LTC_frame_count/N13 ),
    .I3(\LTC_A/LTC_frame_count/hour_tens [1]),
    .O(\LTC_A/LTC_frame_count/N50 )
  );
  LUT4 #(
    .INIT ( 16'hF222 ))
  \LTC_A/LTC_frame_count/hour_tens_mux0000<4>1  (
    .I0(\LTC_A/LTC_frame_count/N50 ),
    .I1(\LTC_A/LTC_frame_count/hour_tens [0]),
    .I2(LTC_a_hours[4]),
    .I3(\LTC_A/LTC_frame_count/sec_tens_and0001 ),
    .O(\LTC_A/LTC_frame_count/hour_tens_mux0000 [4])
  );
  LUT4 #(
    .INIT ( 16'h444E ))
  \h_gen/count_mux0003<7>1  (
    .I0(\h_gen/pulse_state_cmp_eq0000 ),
    .I1(\h_gen/count_addsub0000 [7]),
    .I2(\system_control[0] ),
    .I3(\h_gen/pulse_state_3000 ),
    .O(\h_gen/count_mux0003[7] )
  );
  LUT4 #(
    .INIT ( 16'hFAD8 ))
  \h_gen/count_mux0003<6>1  (
    .I0(\h_gen/pulse_state_cmp_eq0000 ),
    .I1(\system_control[0] ),
    .I2(\h_gen/count_addsub0000 [6]),
    .I3(\h_gen/pulse_state_3000 ),
    .O(\h_gen/count_mux0003[6] )
  );
  LUT4 #(
    .INIT ( 16'hDF8A ))
  \h_gen/count_mux0003<1>1  (
    .I0(\h_gen/pulse_state_cmp_eq0000 ),
    .I1(\h_gen/pulse_state_3000 ),
    .I2(\system_control[0] ),
    .I3(\h_gen/count_addsub0000 [1]),
    .O(\h_gen/count_mux0003[1] )
  );
  LUT4 #(
    .INIT ( 16'hDF8A ))
  \f_gen/count_mux0003<3>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\system_control[0] ),
    .I2(\f_gen/pulse_state_2639 ),
    .I3(\f_gen/count_addsub0000 [3]),
    .O(\f_gen/count_mux0003[3] )
  );
  LUT4 #(
    .INIT ( 16'hFAD8 ))
  \f_gen/count_mux0003<2>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/pulse_state_2639 ),
    .I2(\f_gen/count_addsub0000 [2]),
    .I3(\system_control[0] ),
    .O(\f_gen/count_mux0003[2] )
  );
  LUT4 #(
    .INIT ( 16'h444E ))
  \LTC_B/LTC_clockgenerator/period_count_mux0003<6>1  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_clock_tick_o_or0000 ),
    .I1(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [9]),
    .I2(\LTC_setup[4] ),
    .I3(\LTC_setup[5] ),
    .O(\LTC_B/LTC_clockgenerator/period_count_mux0003[6] )
  );
  LUT4 #(
    .INIT ( 16'h444E ))
  \LTC_B/LTC_clockgenerator/period_count_mux0003<5>1  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_clock_tick_o_or0000 ),
    .I1(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [10]),
    .I2(\LTC_setup[4] ),
    .I3(\LTC_setup[5] ),
    .O(\LTC_B/LTC_clockgenerator/period_count_mux0003[5] )
  );
  LUT4 #(
    .INIT ( 16'h44E4 ))
  \LTC_B/LTC_clockgenerator/period_count_mux0003<13>1  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_clock_tick_o_or0000 ),
    .I1(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [2]),
    .I2(\LTC_setup[4] ),
    .I3(\LTC_setup[5] ),
    .O(\LTC_B/LTC_clockgenerator/period_count_mux0003[13] )
  );
  LUT4 #(
    .INIT ( 16'h444E ))
  \LTC_A/LTC_clockgenerator/period_count_mux0003<6>1  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_clock_tick_o_or0000 ),
    .I1(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [9]),
    .I2(\LTC_setup[0] ),
    .I3(\LTC_setup[1] ),
    .O(\LTC_A/LTC_clockgenerator/period_count_mux0003[6] )
  );
  LUT4 #(
    .INIT ( 16'h444E ))
  \LTC_A/LTC_clockgenerator/period_count_mux0003<5>1  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_clock_tick_o_or0000 ),
    .I1(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [10]),
    .I2(\LTC_setup[0] ),
    .I3(\LTC_setup[1] ),
    .O(\LTC_A/LTC_clockgenerator/period_count_mux0003[5] )
  );
  LUT4 #(
    .INIT ( 16'h44E4 ))
  \LTC_A/LTC_clockgenerator/period_count_mux0003<13>1  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_clock_tick_o_or0000 ),
    .I1(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [2]),
    .I2(\LTC_setup[0] ),
    .I3(\LTC_setup[1] ),
    .O(\LTC_A/LTC_clockgenerator/period_count_mux0003[13] )
  );
  LUT4 #(
    .INIT ( 16'h7D28 ))
  \f_gen/count_mux0003<5>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/pulse_state_2639 ),
    .I2(\system_control[0] ),
    .I3(\f_gen/count_addsub0000 [5]),
    .O(\f_gen/count_mux0003[5] )
  );
  LUT4 #(
    .INIT ( 16'h7D28 ))
  \LTC_B/LTC_clockgenerator/period_count_mux0003<15>1  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_clock_tick_o_or0000 ),
    .I1(\LTC_setup[4] ),
    .I2(\LTC_setup[5] ),
    .I3(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [0]),
    .O(\LTC_B/LTC_clockgenerator/period_count_mux0003[15] )
  );
  LUT4 #(
    .INIT ( 16'h7D28 ))
  \LTC_A/LTC_clockgenerator/period_count_mux0003<15>1  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_clock_tick_o_or0000 ),
    .I1(\LTC_setup[0] ),
    .I2(\LTC_setup[1] ),
    .I3(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [0]),
    .O(\LTC_A/LTC_clockgenerator/period_count_mux0003[15] )
  );
  LUT4 #(
    .INIT ( 16'hFEFF ))
  \LTC_B/LTC_frame_count/frame_units_mux0009<2>19  (
    .I0(\LTC_B/LTC_frame_count/frame_tens [0]),
    .I1(\LTC_setup[5] ),
    .I2(\LTC_B/LTC_frame_count/frame_units [3]),
    .I3(\LTC_B/LTC_frame_count/frame_tens [1]),
    .O(\LTC_B/LTC_frame_count/frame_units_mux0009<2>19_773 )
  );
  LUT4 #(
    .INIT ( 16'hFEFF ))
  \LTC_A/LTC_frame_count/frame_units_mux0009<2>19  (
    .I0(\LTC_A/LTC_frame_count/frame_tens [0]),
    .I1(\LTC_setup[1] ),
    .I2(\LTC_A/LTC_frame_count/frame_units [3]),
    .I3(\LTC_A/LTC_frame_count/frame_tens [1]),
    .O(\LTC_A/LTC_frame_count/frame_units_mux0009<2>19_339 )
  );
  LUT4 #(
    .INIT ( 16'hE040 ))
  \serial_interface/SDA_out_mux000313  (
    .I0(\serial_interface/bit_count [2]),
    .I1(\serial_interface/send_byte [4]),
    .I2(\serial_interface/bit_count [1]),
    .I3(\serial_interface/send_byte [0]),
    .O(\serial_interface/SDA_out_mux000313_3122 )
  );
  LUT4 #(
    .INIT ( 16'h0E04 ))
  \serial_interface/SDA_out_mux000328  (
    .I0(\serial_interface/bit_count [2]),
    .I1(\serial_interface/send_byte [6]),
    .I2(\serial_interface/bit_count [1]),
    .I3(\serial_interface/send_byte [2]),
    .O(\serial_interface/SDA_out_mux000328_3124 )
  );
  LUT4 #(
    .INIT ( 16'hFAD8 ))
  \serial_interface/SDA_out_mux0003113  (
    .I0(\serial_interface/bit_count [0]),
    .I1(\serial_interface/SDA_out_mux000313_3122 ),
    .I2(\serial_interface/SDA_out_mux000385_3125 ),
    .I3(\serial_interface/SDA_out_mux000328_3124 ),
    .O(\serial_interface/SDA_out_mux0003113_3121 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \serial_interface/time_out_mux0009<10>1  (
    .I0(\serial_interface/time_out_or0000_3470 ),
    .I1(\serial_interface/time_out_addsub0000 [11]),
    .O(\serial_interface/time_out_mux0009[10] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \h_gen/count_mux0003<11>1  (
    .I0(\h_gen/pulse_state_cmp_eq0000 ),
    .I1(\h_gen/count_addsub0000 [11]),
    .I2(\h_gen/pulse_state_3000 ),
    .O(\h_gen/count_mux0003[11] )
  );
  LUT4 #(
    .INIT ( 16'hDF8A ))
  \f_gen/count_mux0003<11>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/pulse_state_2639 ),
    .I2(\system_control[0] ),
    .I3(\f_gen/count_addsub0000 [11]),
    .O(\f_gen/count_mux0003[11] )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \serial_interface/time_out_mux0009<9>1  (
    .I0(\serial_interface/time_out_or0000_3470 ),
    .I1(\serial_interface/time_out_addsub0000 [12]),
    .O(\serial_interface/time_out_mux0009[9] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \h_gen/count_mux0003<12>1  (
    .I0(\h_gen/pulse_state_cmp_eq0000 ),
    .I1(\h_gen/count_addsub0000 [12]),
    .I2(\h_gen/pulse_state_3000 ),
    .O(\h_gen/count_mux0003[12] )
  );
  LUT4 #(
    .INIT ( 16'hFAD8 ))
  \f_gen/count_mux0003<12>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/pulse_state_2639 ),
    .I2(\f_gen/count_addsub0000 [12]),
    .I3(\system_control[0] ),
    .O(\f_gen/count_mux0003[12] )
  );
  LUT4 #(
    .INIT ( 16'h444E ))
  \f_gen/count_mux0003<13>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/count_addsub0000 [13]),
    .I2(\system_control[0] ),
    .I3(\f_gen/pulse_state_2639 ),
    .O(\f_gen/count_mux0003[13] )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \serial_interface/time_out_mux0009<7>1  (
    .I0(\serial_interface/time_out_or0000_3470 ),
    .I1(\serial_interface/time_out_addsub0000 [14]),
    .O(\serial_interface/time_out_mux0009[7] )
  );
  LUT4 #(
    .INIT ( 16'h444E ))
  \f_gen/count_mux0003<14>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/count_addsub0000 [14]),
    .I2(\system_control[0] ),
    .I3(\f_gen/pulse_state_2639 ),
    .O(\f_gen/count_mux0003[14] )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \serial_interface/time_out_mux0009<5>1  (
    .I0(\serial_interface/time_out_or0000_3470 ),
    .I1(\serial_interface/time_out_addsub0000 [16]),
    .O(\serial_interface/time_out_mux0009[5] )
  );
  LUT4 #(
    .INIT ( 16'h44E4 ))
  \f_gen/count_mux0003<16>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/count_addsub0000 [16]),
    .I2(\system_control[0] ),
    .I3(\f_gen/pulse_state_2639 ),
    .O(\f_gen/count_mux0003[16] )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \serial_interface/time_out_mux0009<4>1  (
    .I0(\serial_interface/time_out_or0000_3470 ),
    .I1(\serial_interface/time_out_addsub0000 [17]),
    .O(\serial_interface/time_out_mux0009[4] )
  );
  LUT3 #(
    .INIT ( 8'h4E ))
  \f_gen/count_mux0003<17>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/count_addsub0000 [17]),
    .I2(\f_gen/pulse_state_2639 ),
    .O(\f_gen/count_mux0003[17] )
  );
  LUT4 #(
    .INIT ( 16'h0F01 ))
  \LTC_B/LTC_frame_count/frame_tens_mux0002<5>30  (
    .I0(\LTC_B/LTC_frame_count/frame_units [1]),
    .I1(\LTC_B/LTC_frame_count/frame_units [3]),
    .I2(\LTC_setup[4] ),
    .I3(\LTC_B/LTC_frame_count/frame_units [2]),
    .O(\LTC_B/LTC_frame_count/frame_tens_mux0002<5>30_754 )
  );
  LUT4 #(
    .INIT ( 16'hAA20 ))
  \LTC_B/LTC_frame_count/frame_tens_mux0002<5>52  (
    .I0(\LTC_setup[4] ),
    .I1(\LTC_B/LTC_frame_count/frame_units [3]),
    .I2(\LTC_B/LTC_frame_count/frame_units [0]),
    .I3(\LTC_B/LTC_frame_count/frame_units [1]),
    .O(\LTC_B/LTC_frame_count/frame_tens_mux0002<5>52_755 )
  );
  LUT4 #(
    .INIT ( 16'hFAC8 ))
  \LTC_B/LTC_frame_count/frame_tens_mux0002<5>57  (
    .I0(\LTC_B/LTC_frame_count/frame_tens [0]),
    .I1(\LTC_B/LTC_frame_count/frame_units [2]),
    .I2(\LTC_setup[5] ),
    .I3(\LTC_B/LTC_frame_count/frame_units [1]),
    .O(\LTC_B/LTC_frame_count/frame_tens_mux0002<5>57_756 )
  );
  LUT4 #(
    .INIT ( 16'h0F01 ))
  \LTC_A/LTC_frame_count/frame_tens_mux0002<5>30  (
    .I0(\LTC_A/LTC_frame_count/frame_units [1]),
    .I1(\LTC_A/LTC_frame_count/frame_units [3]),
    .I2(\LTC_setup[0] ),
    .I3(\LTC_A/LTC_frame_count/frame_units [2]),
    .O(\LTC_A/LTC_frame_count/frame_tens_mux0002<5>30_320 )
  );
  LUT4 #(
    .INIT ( 16'hAA20 ))
  \LTC_A/LTC_frame_count/frame_tens_mux0002<5>52  (
    .I0(\LTC_setup[0] ),
    .I1(\LTC_A/LTC_frame_count/frame_units [3]),
    .I2(\LTC_A/LTC_frame_count/frame_units [0]),
    .I3(\LTC_A/LTC_frame_count/frame_units [1]),
    .O(\LTC_A/LTC_frame_count/frame_tens_mux0002<5>52_321 )
  );
  LUT4 #(
    .INIT ( 16'hFAC8 ))
  \LTC_A/LTC_frame_count/frame_tens_mux0002<5>57  (
    .I0(\LTC_A/LTC_frame_count/frame_tens [0]),
    .I1(\LTC_A/LTC_frame_count/frame_units [2]),
    .I2(\LTC_setup[1] ),
    .I3(\LTC_A/LTC_frame_count/frame_units [1]),
    .O(\LTC_A/LTC_frame_count/frame_tens_mux0002<5>57_322 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \serial_interface/time_out_mux0009<3>1  (
    .I0(\serial_interface/time_out_or0000_3470 ),
    .I1(\serial_interface/time_out_addsub0000 [18]),
    .O(\serial_interface/time_out_mux0009[3] )
  );
  LUT4 #(
    .INIT ( 16'h44E4 ))
  \f_gen/count_mux0003<18>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/count_addsub0000 [18]),
    .I2(\system_control[0] ),
    .I3(\f_gen/pulse_state_2639 ),
    .O(\f_gen/count_mux0003[18] )
  );
  LUT2 #(
    .INIT ( 4'hE ))
  \LTC_B/delay_count_low_or00001  (
    .I0(\VCXO_pll/pps_148_tick_o_2290 ),
    .I1(\LTC_B/delay_count_high_cmp_eq0000 ),
    .O(\LTC_B/delay_count_low_or0000 )
  );
  LUT2 #(
    .INIT ( 4'hE ))
  \LTC_A/delay_count_low_or00001  (
    .I0(\VCXO_pll/pps_148_tick_o_2290 ),
    .I1(\LTC_A/delay_count_high_cmp_eq0000 ),
    .O(\LTC_A/delay_count_low_or0000 )
  );
  LUT4 #(
    .INIT ( 16'h444E ))
  \f_gen/count_mux0003<19>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/count_addsub0000 [19]),
    .I2(\system_control[0] ),
    .I3(\f_gen/pulse_state_2639 ),
    .O(\f_gen/count_mux0003[19] )
  );
  LUT4 #(
    .INIT ( 16'h44E4 ))
  \f_gen/count_mux0003<20>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/count_addsub0000 [20]),
    .I2(\system_control[0] ),
    .I3(\f_gen/pulse_state_2639 ),
    .O(\f_gen/count_mux0003[20] )
  );
  LUT3 #(
    .INIT ( 8'h80 ))
  \LTC_B/LTC_frame_count/hour_units_and00001  (
    .I0(\LTC_B/LTC_frame_count/N01 ),
    .I1(\LTC_B/LTC_frame_count/N111 ),
    .I2(\LTC_B/LTC_frame_count/N13 ),
    .O(\LTC_B/LTC_frame_count/hour_units_and0000 )
  );
  LUT3 #(
    .INIT ( 8'h80 ))
  \LTC_A/LTC_frame_count/hour_units_and00001  (
    .I0(\LTC_A/LTC_frame_count/N01 ),
    .I1(\LTC_A/LTC_frame_count/N111 ),
    .I2(\LTC_A/LTC_frame_count/N13 ),
    .O(\LTC_A/LTC_frame_count/hour_units_and0000 )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \LTC_B/LTC_frame_count/hour_tens_not000221  (
    .I0(\LTC_B/LTC_frame_count/min_tens [1]),
    .I1(\LTC_B/LTC_frame_count/min_tens [0]),
    .I2(\LTC_B/LTC_frame_count/min_tens [2]),
    .I3(\LTC_B/LTC_frame_count/N9 ),
    .O(\LTC_B/LTC_frame_count/N111 )
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  \LTC_B/LTC_frame_count/frame_units_mux0009<1>211  (
    .I0(\LTC_B/LTC_frame_count/sec_units [3]),
    .I1(\LTC_B/LTC_frame_count/sec_units [1]),
    .I2(\LTC_B/LTC_frame_count/sec_units [0]),
    .I3(\LTC_B/LTC_frame_count/sec_units [2]),
    .O(\LTC_B/LTC_frame_count/N12 )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \LTC_A/LTC_frame_count/hour_tens_not000221  (
    .I0(\LTC_A/LTC_frame_count/min_tens [1]),
    .I1(\LTC_A/LTC_frame_count/min_tens [0]),
    .I2(\LTC_A/LTC_frame_count/min_tens [2]),
    .I3(\LTC_A/LTC_frame_count/N9 ),
    .O(\LTC_A/LTC_frame_count/N111 )
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  \LTC_A/LTC_frame_count/frame_units_mux0009<1>211  (
    .I0(\LTC_A/LTC_frame_count/sec_units [3]),
    .I1(\LTC_A/LTC_frame_count/sec_units [1]),
    .I2(\LTC_A/LTC_frame_count/sec_units [0]),
    .I3(\LTC_A/LTC_frame_count/sec_units [2]),
    .O(\LTC_A/LTC_frame_count/N12 )
  );
  LUT4 #(
    .INIT ( 16'hFFDF ))
  \LTC_B/LTC_frame_count/hour_tens_not00021_SW0  (
    .I0(\LTC_B/LTC_frame_count/count_sec_740 ),
    .I1(\LTC_B/LTC_frame_count/min_units [2]),
    .I2(\LTC_B/LTC_frame_count/N12 ),
    .I3(\LTC_B/LTC_frame_count/min_units [1]),
    .O(N123)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \LTC_B/LTC_frame_count/hour_tens_not00021  (
    .I0(N123),
    .I1(\LTC_B/LTC_frame_count/min_units [3]),
    .I2(\LTC_B/LTC_frame_count/min_units [0]),
    .I3(\LTC_B/LTC_frame_count/N49 ),
    .O(\LTC_B/LTC_frame_count/N9 )
  );
  LUT4 #(
    .INIT ( 16'hFFDF ))
  \LTC_A/LTC_frame_count/hour_tens_not00021_SW0  (
    .I0(\LTC_A/LTC_frame_count/count_sec_306 ),
    .I1(\LTC_A/LTC_frame_count/min_units [2]),
    .I2(\LTC_A/LTC_frame_count/N12 ),
    .I3(\LTC_A/LTC_frame_count/min_units [1]),
    .O(N125)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \LTC_A/LTC_frame_count/hour_tens_not00021  (
    .I0(N125),
    .I1(\LTC_A/LTC_frame_count/min_units [3]),
    .I2(\LTC_A/LTC_frame_count/min_units [0]),
    .I3(\LTC_A/LTC_frame_count/N49 ),
    .O(\LTC_A/LTC_frame_count/N9 )
  );
  LUT4 #(
    .INIT ( 16'h444E ))
  \f_gen/count_mux0003<21>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/count_addsub0000 [21]),
    .I2(\system_control[0] ),
    .I3(\f_gen/pulse_state_2639 ),
    .O(\f_gen/count_mux0003[21] )
  );
  LUT4 #(
    .INIT ( 16'h444E ))
  \f_gen/count_mux0003<22>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/count_addsub0000 [22]),
    .I2(\system_control[0] ),
    .I3(\f_gen/pulse_state_2639 ),
    .O(\f_gen/count_mux0003[22] )
  );
  LUT2 #(
    .INIT ( 4'hD ))
  \communication/state_cmp_eq0002_inv1  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .O(\communication/state_cmp_eq0002_inv )
  );
  LUT4 #(
    .INIT ( 16'h44E4 ))
  \f_gen/count_mux0003<23>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/count_addsub0000 [23]),
    .I2(\system_control[0] ),
    .I3(\f_gen/pulse_state_2639 ),
    .O(\f_gen/count_mux0003[23] )
  );
  LUT4 #(
    .INIT ( 16'h444E ))
  \f_gen/count_mux0003<24>1  (
    .I0(\f_gen/pulse_state_cmp_eq0000 ),
    .I1(\f_gen/count_addsub0000 [24]),
    .I2(\system_control[0] ),
    .I3(\f_gen/pulse_state_2639 ),
    .O(\f_gen/count_mux0003[24] )
  );
  LUT2 #(
    .INIT ( 4'hE ))
  \LTC_B/LTC_clockgenerator/ltc_clock_tick_o_or00001  (
    .I0(\LTC_B/delay_sync_983 ),
    .I1(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .O(\LTC_B/LTC_clockgenerator/ltc_clock_tick_o_or0000 )
  );
  LUT2 #(
    .INIT ( 4'hE ))
  \LTC_A/LTC_clockgenerator/ltc_clock_tick_o_or00001  (
    .I0(\LTC_A/delay_sync_549 ),
    .I1(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .O(\LTC_A/LTC_clockgenerator/ltc_clock_tick_o_or0000 )
  );
  LUT3 #(
    .INIT ( 8'h08 ))
  \serial_interface/recieve_byte_1_cmp_eq000011  (
    .I0(\serial_interface/bit_count [2]),
    .I1(\serial_interface/bit_count [1]),
    .I2(\serial_interface/bit_count [3]),
    .O(N84)
  );
  LUT3 #(
    .INIT ( 8'h08 ))
  \LTC_B/LTC_frame_count/frame_units_mux0009<1>221  (
    .I0(\LTC_B/LTC_frame_count/sec_tens [2]),
    .I1(\LTC_B/LTC_frame_count/sec_tens [0]),
    .I2(\LTC_B/LTC_frame_count/sec_tens [1]),
    .O(\LTC_B/LTC_frame_count/N49 )
  );
  LUT3 #(
    .INIT ( 8'h08 ))
  \LTC_A/LTC_frame_count/frame_units_mux0009<1>221  (
    .I0(\LTC_A/LTC_frame_count/sec_tens [2]),
    .I1(\LTC_A/LTC_frame_count/sec_tens [0]),
    .I2(\LTC_A/LTC_frame_count/sec_tens [1]),
    .O(\LTC_A/LTC_frame_count/N49 )
  );
  LUT4 #(
    .INIT ( 16'hF222 ))
  \LTC_B/LTC_clockgenerator/period_count_mux0003<14>1  (
    .I0(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [1]),
    .I1(\LTC_B/LTC_clockgenerator/ltc_clock_tick_o_or0000 ),
    .I2(\LTC_setup[4] ),
    .I3(\LTC_B/LTC_clockgenerator/N5 ),
    .O(\LTC_B/LTC_clockgenerator/period_count_mux0003[14] )
  );
  LUT4 #(
    .INIT ( 16'h22F2 ))
  \LTC_B/LTC_clockgenerator/period_count_mux0003<10>1  (
    .I0(\LTC_B/LTC_clockgenerator/N5 ),
    .I1(\LTC_setup[4] ),
    .I2(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [5]),
    .I3(\LTC_B/LTC_clockgenerator/ltc_clock_tick_o_or0000 ),
    .O(\LTC_B/LTC_clockgenerator/period_count_mux0003[10] )
  );
  LUT4 #(
    .INIT ( 16'hF222 ))
  \LTC_A/LTC_clockgenerator/period_count_mux0003<14>1  (
    .I0(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [1]),
    .I1(\LTC_A/LTC_clockgenerator/ltc_clock_tick_o_or0000 ),
    .I2(\LTC_setup[0] ),
    .I3(\LTC_A/LTC_clockgenerator/N5 ),
    .O(\LTC_A/LTC_clockgenerator/period_count_mux0003[14] )
  );
  LUT4 #(
    .INIT ( 16'h22F2 ))
  \LTC_A/LTC_clockgenerator/period_count_mux0003<10>1  (
    .I0(\LTC_A/LTC_clockgenerator/N5 ),
    .I1(\LTC_setup[0] ),
    .I2(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [5]),
    .I3(\LTC_A/LTC_clockgenerator/ltc_clock_tick_o_or0000 ),
    .O(\LTC_A/LTC_clockgenerator/period_count_mux0003[10] )
  );
  LUT4 #(
    .INIT ( 16'h22A2 ))
  \serial_interface/state_FSM_FFd3-In13  (
    .I0(\serial_interface/state_FSM_FFd4_3382 ),
    .I1(\serial_interface/state_FSM_FFd2_3359 ),
    .I2(\serial_interface/SCL_delay [1]),
    .I3(\serial_interface/SCL_delay [0]),
    .O(\serial_interface/state_FSM_FFd3-In13_3372 )
  );
  LUT4 #(
    .INIT ( 16'hDC10 ))
  \serial_interface/state_FSM_FFd3-In129  (
    .I0(\serial_interface/state_FSM_FFd2_3359 ),
    .I1(\serial_interface/state_FSM_FFd1_3351 ),
    .I2(\serial_interface/command [0]),
    .I3(\serial_interface/command [1]),
    .O(\serial_interface/state_FSM_FFd3-In129_3371 )
  );
  LUT4 #(
    .INIT ( 16'hFF8A ))
  \serial_interface/state_FSM_FFd3-In169  (
    .I0(\serial_interface/state_FSM_FFd2_3359 ),
    .I1(\serial_interface/command [1]),
    .I2(\serial_interface/state_cmp_eq0007 ),
    .I3(\serial_interface/state_FSM_FFd3-In164 ),
    .O(\serial_interface/state_FSM_FFd3-In169_3375 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \serial_interface/state_FSM_FFd3-In204  (
    .I0(\serial_interface/state_FSM_N0 ),
    .I1(\serial_interface/state_FSM_FFd3-In193_3376 ),
    .O(\serial_interface/state_FSM_FFd3-In )
  );
  LUT4 #(
    .INIT ( 16'h0010 ))
  \LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq000011  (
    .I0(\LTC_B/LTC_clockgenerator/period_count [1]),
    .I1(\LTC_B/LTC_clockgenerator/period_count [2]),
    .I2(\LTC_B/LTC_clockgenerator/period_count [0]),
    .I3(\LTC_B/LTC_clockgenerator/period_count [3]),
    .O(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq000011_654 )
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq000024  (
    .I0(\LTC_B/LTC_clockgenerator/period_count [4]),
    .I1(\LTC_B/LTC_clockgenerator/period_count [5]),
    .I2(\LTC_B/LTC_clockgenerator/period_count [6]),
    .I3(\LTC_B/LTC_clockgenerator/period_count [7]),
    .O(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq000024_655 )
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq000048  (
    .I0(\LTC_B/LTC_clockgenerator/period_count [8]),
    .I1(\LTC_B/LTC_clockgenerator/period_count [9]),
    .I2(\LTC_B/LTC_clockgenerator/period_count [10]),
    .I3(\LTC_B/LTC_clockgenerator/period_count [11]),
    .O(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq000048_656 )
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq000061  (
    .I0(\LTC_B/LTC_clockgenerator/period_count [12]),
    .I1(\LTC_B/LTC_clockgenerator/period_count [13]),
    .I2(\LTC_B/LTC_clockgenerator/period_count [14]),
    .I3(\LTC_B/LTC_clockgenerator/period_count [15]),
    .O(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq000061_657 )
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq000075  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq000011_654 ),
    .I1(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq000024_655 ),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq000048_656 ),
    .I3(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq000061_657 ),
    .O(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 )
  );
  LUT4 #(
    .INIT ( 16'h0010 ))
  \LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq000011  (
    .I0(\LTC_A/LTC_clockgenerator/period_count [1]),
    .I1(\LTC_A/LTC_clockgenerator/period_count [2]),
    .I2(\LTC_A/LTC_clockgenerator/period_count [0]),
    .I3(\LTC_A/LTC_clockgenerator/period_count [3]),
    .O(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq000011_220 )
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq000024  (
    .I0(\LTC_A/LTC_clockgenerator/period_count [4]),
    .I1(\LTC_A/LTC_clockgenerator/period_count [5]),
    .I2(\LTC_A/LTC_clockgenerator/period_count [6]),
    .I3(\LTC_A/LTC_clockgenerator/period_count [7]),
    .O(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq000024_221 )
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq000048  (
    .I0(\LTC_A/LTC_clockgenerator/period_count [8]),
    .I1(\LTC_A/LTC_clockgenerator/period_count [9]),
    .I2(\LTC_A/LTC_clockgenerator/period_count [10]),
    .I3(\LTC_A/LTC_clockgenerator/period_count [11]),
    .O(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq000048_222 )
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq000061  (
    .I0(\LTC_A/LTC_clockgenerator/period_count [12]),
    .I1(\LTC_A/LTC_clockgenerator/period_count [13]),
    .I2(\LTC_A/LTC_clockgenerator/period_count [14]),
    .I3(\LTC_A/LTC_clockgenerator/period_count [15]),
    .O(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq000061_223 )
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq000075  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq000011_220 ),
    .I1(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq000024_221 ),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq000048_222 ),
    .I3(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq000061_223 ),
    .O(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 )
  );
  LUT4 #(
    .INIT ( 16'hFFDF ))
  \LTC_B/LTC_frame_count/frame_units_mux0009<1>15  (
    .I0(\LTC_B/LTC_frame_count/min_units [0]),
    .I1(\LTC_B/LTC_frame_count/min_units [1]),
    .I2(\LTC_B/LTC_frame_count/min_units [3]),
    .I3(\LTC_B/LTC_frame_count/min_units [2]),
    .O(\LTC_B/LTC_frame_count/frame_units_mux0009<1>15_766 )
  );
  LUT3 #(
    .INIT ( 8'h08 ))
  \LTC_B/LTC_frame_count/frame_units_mux0009<1>26  (
    .I0(\LTC_B/LTC_frame_count/N49 ),
    .I1(\LTC_setup[5] ),
    .I2(\LTC_setup[4] ),
    .O(\LTC_B/LTC_frame_count/frame_units_mux0009<1>26_767 )
  );
  LUT3 #(
    .INIT ( 8'h08 ))
  \LTC_B/LTC_frame_count/frame_units_mux0009<1>31  (
    .I0(\LTC_setup[6] ),
    .I1(\LTC_B/LTC_frame_count/frame_tens [1]),
    .I2(\LTC_B/LTC_frame_count/frame_tens [0]),
    .O(\LTC_B/LTC_frame_count/frame_units_mux0009<1>31_768 )
  );
  LUT4 #(
    .INIT ( 16'hFFDF ))
  \LTC_A/LTC_frame_count/frame_units_mux0009<1>15  (
    .I0(\LTC_A/LTC_frame_count/min_units [0]),
    .I1(\LTC_A/LTC_frame_count/min_units [1]),
    .I2(\LTC_A/LTC_frame_count/min_units [3]),
    .I3(\LTC_A/LTC_frame_count/min_units [2]),
    .O(\LTC_A/LTC_frame_count/frame_units_mux0009<1>15_332 )
  );
  LUT3 #(
    .INIT ( 8'h08 ))
  \LTC_A/LTC_frame_count/frame_units_mux0009<1>26  (
    .I0(\LTC_A/LTC_frame_count/N49 ),
    .I1(\LTC_setup[1] ),
    .I2(\LTC_setup[0] ),
    .O(\LTC_A/LTC_frame_count/frame_units_mux0009<1>26_333 )
  );
  LUT3 #(
    .INIT ( 8'h08 ))
  \LTC_A/LTC_frame_count/frame_units_mux0009<1>31  (
    .I0(\LTC_setup[2] ),
    .I1(\LTC_A/LTC_frame_count/frame_tens [1]),
    .I2(\LTC_A/LTC_frame_count/frame_tens [0]),
    .O(\LTC_A/LTC_frame_count/frame_units_mux0009<1>31_334 )
  );
  LUT4 #(
    .INIT ( 16'h2F23 ))
  \serial_interface/state_FSM_FFd1-In89  (
    .I0(\serial_interface/state_FSM_FFd4_3382 ),
    .I1(\serial_interface/command [1]),
    .I2(\serial_interface/command [2]),
    .I3(\serial_interface/state_FSM_FFd3_3369 ),
    .O(\serial_interface/state_FSM_FFd1-In89_3358 )
  );
  LUT3 #(
    .INIT ( 8'h01 ))
  \serial_interface/recieve_byte_7_cmp_eq000011  (
    .I0(\serial_interface/bit_count [2]),
    .I1(\serial_interface/bit_count [1]),
    .I2(\serial_interface/bit_count [0]),
    .O(N87)
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \serial_interface/state_cmp_eq00081  (
    .I0(\serial_interface/SCL_delay [1]),
    .I1(\serial_interface/SCL_delay [0]),
    .O(\serial_interface/state_cmp_eq0008 )
  );
  LUT4 #(
    .INIT ( 16'h0010 ))
  \serial_interface/state_FSM_FFd2-In38  (
    .I0(\serial_interface/SCL_delay [1]),
    .I1(\serial_interface/state_FSM_FFd1_3351 ),
    .I2(\serial_interface/SCL_delay [0]),
    .I3(\serial_interface/state_FSM_FFd2_3359 ),
    .O(\serial_interface/state_FSM_FFd2-In38_3364 )
  );
  LUT4 #(
    .INIT ( 16'hA888 ))
  \serial_interface/state_FSM_FFd2-In55  (
    .I0(\serial_interface/state_FSM_FFd3_3369 ),
    .I1(\serial_interface/state_FSM_FFd2-In38_3364 ),
    .I2(\serial_interface/state_FSM_FFd4_3382 ),
    .I3(\serial_interface/state_FSM_FFd2-In16_3363 ),
    .O(\serial_interface/state_FSM_FFd2-In55_3365 )
  );
  LUT3 #(
    .INIT ( 8'hFB ))
  \serial_interface/state_FSM_FFd2-In72  (
    .I0(\serial_interface/command [1]),
    .I1(\serial_interface/state_cmp_eq0011 ),
    .I2(\serial_interface/command [0]),
    .O(\serial_interface/state_FSM_FFd2-In72_3366 )
  );
  LUT4 #(
    .INIT ( 16'h3222 ))
  \serial_interface/state_FSM_FFd2-In115  (
    .I0(\serial_interface/state_FSM_FFd2-In99_3368 ),
    .I1(\serial_interface/state_cmp_eq0011 ),
    .I2(\serial_interface/state_FSM_N2 ),
    .I3(\serial_interface/state_FSM_FFd4_3382 ),
    .O(\serial_interface/state_FSM_FFd2-In115_3361 )
  );
  LUT3 #(
    .INIT ( 8'hA8 ))
  \serial_interface/state_FSM_FFd2-In140  (
    .I0(\serial_interface/state_FSM_FFd2_3359 ),
    .I1(\serial_interface/state_FSM_FFd2-In78_3367 ),
    .I2(\serial_interface/state_FSM_FFd2-In115_3361 ),
    .O(\serial_interface/state_FSM_FFd2-In140_3362 )
  );
  LUT3 #(
    .INIT ( 8'hA8 ))
  \serial_interface/state_FSM_FFd2-In167  (
    .I0(\serial_interface/state_FSM_N0 ),
    .I1(\serial_interface/state_FSM_FFd2-In55_3365 ),
    .I2(\serial_interface/state_FSM_FFd2-In140_3362 ),
    .O(\serial_interface/state_FSM_FFd2-In )
  );
  LUT4 #(
    .INIT ( 16'h11F1 ))
  \serial_interface/state_FSM_FFd2-In23  (
    .I0(\serial_interface/command [0]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_cmp_eq0010_3399 ),
    .I3(\serial_interface/state_cmp_eq0008 ),
    .O(\serial_interface/state_FSM_N2 )
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  I2C_byte_out_cmp_eq00091 (
    .I0(\communication/address_o [0]),
    .I1(\communication/address_o [5]),
    .I2(N20),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0009)
  );
  LUT4 #(
    .INIT ( 16'hFEFF ))
  \serial_interface/state_cmp_eq0010_SW0  (
    .I0(\serial_interface/recieve_byte [3]),
    .I1(\serial_interface/recieve_byte [2]),
    .I2(\serial_interface/recieve_byte [1]),
    .I3(\serial_interface/recieve_byte [4]),
    .O(N133)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \serial_interface/state_cmp_eq0010  (
    .I0(N133),
    .I1(\serial_interface/recieve_byte [6]),
    .I2(\serial_interface/recieve_byte [5]),
    .I3(\serial_interface/recieve_byte [7]),
    .O(\serial_interface/state_cmp_eq0010_3399 )
  );
  LUT4 #(
    .INIT ( 16'h01AB ))
  \serial_interface/state_FSM_FFd4-In119  (
    .I0(\serial_interface/state_FSM_FFd1_3351 ),
    .I1(\serial_interface/command [0]),
    .I2(\serial_interface/state_FSM_FFd3_3369 ),
    .I3(\serial_interface/command [1]),
    .O(\serial_interface/state_FSM_FFd4-In119_3383 )
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/state_FSM_FFd4-In136  (
    .I0(\serial_interface/state_FSM_FFd4-In119_3383 ),
    .I1(\serial_interface/state_FSM_FFd2_3359 ),
    .I2(\serial_interface/state_FSM_N2 ),
    .I3(\serial_interface/state_FSM_FFd4-In122_3384 ),
    .O(\serial_interface/state_FSM_FFd4-In136_3385 )
  );
  LUT4 #(
    .INIT ( 16'hAA20 ))
  \serial_interface/state_FSM_FFd4-In179  (
    .I0(\serial_interface/state_FSM_FFd4_3382 ),
    .I1(\serial_interface/state_cmp_eq0011 ),
    .I2(\serial_interface/state_FSM_FFd4-In136_3385 ),
    .I3(\serial_interface/state_FSM_FFd4-In95 ),
    .O(\serial_interface/state_FSM_FFd4-In179_3386 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \LTC_B/LTC_biphase_generator/Madd_ltc_frame_i_sub0000_xor<5>11  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [6]),
    .I1(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .O(\LTC_B/LTC_biphase_generator/ltc_frame_i_sub0000 [5])
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \LTC_A/LTC_biphase_generator/Madd_ltc_frame_i_sub0000_xor<5>11  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [6]),
    .I1(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .O(\LTC_A/LTC_biphase_generator/ltc_frame_i_sub0000 [5])
  );
  LUT3 #(
    .INIT ( 8'hD5 ))
  \LTC_B/LTC_biphase_generator/biphase_code_not00024  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [0]),
    .I1(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_6_f5_580 ),
    .I2(\LTC_B/LTC_biphase_generator/ltc_frame_i_sub0000 [6]),
    .O(\LTC_B/LTC_biphase_generator/biphase_code_not00024_585 )
  );
  LUT4 #(
    .INIT ( 16'h444E ))
  \LTC_B/LTC_biphase_generator/biphase_code_not000223  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [3]),
    .I1(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_f7_558 ),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .I3(\LTC_B/LTC_clockgenerator/ltc_bitcount [6]),
    .O(\LTC_B/LTC_biphase_generator/biphase_code_not000223_584 )
  );
  LUT4 #(
    .INIT ( 16'h0E04 ))
  \LTC_B/LTC_biphase_generator/biphase_code_not000251  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [4]),
    .I1(\LTC_B/LTC_biphase_generator/biphase_code_not000223_584 ),
    .I2(\LTC_B/LTC_biphase_generator/ltc_frame_i_sub0000 [6]),
    .I3(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_8_f5_581 ),
    .O(\LTC_B/LTC_biphase_generator/biphase_code_not000251_586 )
  );
  LUT3 #(
    .INIT ( 8'hA8 ))
  \LTC_B/LTC_biphase_generator/biphase_code_not000277  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_clock_tick_o_661 ),
    .I1(\LTC_B/LTC_biphase_generator/biphase_code_not00024_585 ),
    .I2(\LTC_B/LTC_biphase_generator/biphase_code_not000251_586 ),
    .O(\LTC_B/LTC_biphase_generator/biphase_code_not0002 )
  );
  LUT3 #(
    .INIT ( 8'hD5 ))
  \LTC_A/LTC_biphase_generator/biphase_code_not00024  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [0]),
    .I1(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_6_f5_146 ),
    .I2(\LTC_A/LTC_biphase_generator/ltc_frame_i_sub0000 [6]),
    .O(\LTC_A/LTC_biphase_generator/biphase_code_not00024_151 )
  );
  LUT4 #(
    .INIT ( 16'h444E ))
  \LTC_A/LTC_biphase_generator/biphase_code_not000223  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [3]),
    .I1(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_f7_124 ),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .I3(\LTC_A/LTC_clockgenerator/ltc_bitcount [6]),
    .O(\LTC_A/LTC_biphase_generator/biphase_code_not000223_150 )
  );
  LUT4 #(
    .INIT ( 16'h0E04 ))
  \LTC_A/LTC_biphase_generator/biphase_code_not000251  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [4]),
    .I1(\LTC_A/LTC_biphase_generator/biphase_code_not000223_150 ),
    .I2(\LTC_A/LTC_biphase_generator/ltc_frame_i_sub0000 [6]),
    .I3(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_8_f5_147 ),
    .O(\LTC_A/LTC_biphase_generator/biphase_code_not000251_152 )
  );
  LUT3 #(
    .INIT ( 8'hA8 ))
  \LTC_A/LTC_biphase_generator/biphase_code_not000277  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_clock_tick_o_227 ),
    .I1(\LTC_A/LTC_biphase_generator/biphase_code_not00024_151 ),
    .I2(\LTC_A/LTC_biphase_generator/biphase_code_not000251_152 ),
    .O(\LTC_A/LTC_biphase_generator/biphase_code_not0002 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  I2C_byte_out_or0000211 (
    .I0(\communication/address_o [1]),
    .I1(\communication/address_o [0]),
    .O(N93)
  );
  LUT2 #(
    .INIT ( 4'hE ))
  I2C_byte_out_or000011 (
    .I0(\communication/address_o [0]),
    .I1(\communication/address_o [1]),
    .O(N26)
  );
  LUT3 #(
    .INIT ( 8'h08 ))
  I2C_byte_out_cmp_eq002011 (
    .I0(\communication/address_o [2]),
    .I1(\communication/address_o [4]),
    .I2(\communication/address_o [3]),
    .O(N21)
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  I2C_byte_out_cmp_eq001611 (
    .I0(\communication/address_o [3]),
    .I1(\communication/address_o [4]),
    .I2(\communication/address_o [2]),
    .O(N18)
  );
  LUT3 #(
    .INIT ( 8'h08 ))
  I2C_byte_out_cmp_eq001211 (
    .I0(\communication/address_o [2]),
    .I1(\communication/address_o [3]),
    .I2(\communication/address_o [4]),
    .O(N19)
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  I2C_byte_out_cmp_eq000811 (
    .I0(\communication/address_o [4]),
    .I1(\communication/address_o [3]),
    .I2(\communication/address_o [2]),
    .O(N20)
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  I2C_byte_out_cmp_eq000411 (
    .I0(\communication/address_o [4]),
    .I1(\communication/address_o [2]),
    .I2(\communication/address_o [3]),
    .O(N17)
  );
  LUT3 #(
    .INIT ( 8'h01 ))
  I2C_byte_out_cmp_eq000011 (
    .I0(\communication/address_o [4]),
    .I1(\communication/address_o [3]),
    .I2(\communication/address_o [2]),
    .O(N14)
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  I2C_byte_out_cmp_eq00221 (
    .I0(N21),
    .I1(\communication/address_o [5]),
    .I2(\communication/address_o [1]),
    .I3(\communication/address_o [0]),
    .O(I2C_byte_out_cmp_eq0022)
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  I2C_byte_out_cmp_eq00211 (
    .I0(\communication/address_o [0]),
    .I1(\communication/address_o [5]),
    .I2(N21),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0021)
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  I2C_byte_out_cmp_eq00181 (
    .I0(N18),
    .I1(\communication/address_o [5]),
    .I2(\communication/address_o [1]),
    .I3(\communication/address_o [0]),
    .O(I2C_byte_out_cmp_eq0018)
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  I2C_byte_out_cmp_eq00171 (
    .I0(\communication/address_o [0]),
    .I1(\communication/address_o [5]),
    .I2(N18),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0017)
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  I2C_byte_out_cmp_eq00141 (
    .I0(N19),
    .I1(\communication/address_o [5]),
    .I2(\communication/address_o [1]),
    .I3(\communication/address_o [0]),
    .O(I2C_byte_out_cmp_eq0014)
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  I2C_byte_out_cmp_eq00131 (
    .I0(\communication/address_o [0]),
    .I1(\communication/address_o [5]),
    .I2(N19),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0013)
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  I2C_byte_out_cmp_eq00101 (
    .I0(N20),
    .I1(\communication/address_o [5]),
    .I2(\communication/address_o [1]),
    .I3(\communication/address_o [0]),
    .O(I2C_byte_out_cmp_eq0010)
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  I2C_byte_out_cmp_eq00061 (
    .I0(N17),
    .I1(\communication/address_o [5]),
    .I2(\communication/address_o [1]),
    .I3(\communication/address_o [0]),
    .O(I2C_byte_out_cmp_eq0006)
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  I2C_byte_out_cmp_eq00051 (
    .I0(\communication/address_o [0]),
    .I1(\communication/address_o [5]),
    .I2(N17),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0005)
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  I2C_byte_out_cmp_eq00021 (
    .I0(N14),
    .I1(\communication/address_o [5]),
    .I2(\communication/address_o [1]),
    .I3(\communication/address_o [0]),
    .O(I2C_byte_out_cmp_eq0002)
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  I2C_byte_out_cmp_eq00011 (
    .I0(\communication/address_o [0]),
    .I1(\communication/address_o [5]),
    .I2(N14),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0001)
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<6>5  (
    .I0(genlock_register_10_6_2672),
    .I1(I2C_byte_out_cmp_eq0010),
    .I2(I2C_byte_out_cmp_eq0027),
    .I3(\OCXO_pll/phase_detect/phase_diff [14]),
    .O(\serial_interface/send_byte_mux0001<6>5_3321 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<6>10  (
    .I0(genlock_register_11_6_2681),
    .I1(I2C_byte_out_cmp_eq0011),
    .I2(genlock_register_12_6_2690),
    .I3(I2C_byte_out_cmp_eq0012),
    .O(\serial_interface/send_byte_mux0001<6>10_3310 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<6>22  (
    .I0(genlock_register_13_6_2699),
    .I1(I2C_byte_out_cmp_eq0013),
    .I2(genlock_register_14_6_2708),
    .I3(I2C_byte_out_cmp_eq0014),
    .O(\serial_interface/send_byte_mux0001<6>22_3318 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<6>27  (
    .I0(genlock_register_15_6_2717),
    .I1(I2C_byte_out_cmp_eq0015),
    .I2(genlock_register_16_6_2726),
    .I3(I2C_byte_out_cmp_eq0016),
    .O(\serial_interface/send_byte_mux0001<6>27_3319 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<6>37  (
    .I0(\serial_interface/send_byte_mux0001<6>5_3321 ),
    .I1(\serial_interface/send_byte_mux0001<6>10_3310 ),
    .I2(\serial_interface/send_byte_mux0001<6>22_3318 ),
    .I3(\serial_interface/send_byte_mux0001<6>27_3319 ),
    .O(\serial_interface/send_byte_mux0001<6>37_3320 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<6>50  (
    .I0(genlock_register_17_6_2735),
    .I1(I2C_byte_out_cmp_eq0017),
    .I2(genlock_register_18_6_2744),
    .I3(I2C_byte_out_cmp_eq0018),
    .O(\serial_interface/send_byte_mux0001<6>50_3322 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<6>55  (
    .I0(genlock_register_19_6_2753),
    .I1(I2C_byte_out_cmp_eq0019),
    .I2(genlock_register_1_6_2762),
    .I3(I2C_byte_out_cmp_eq0001),
    .O(\serial_interface/send_byte_mux0001<6>55_3323 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<6>67  (
    .I0(genlock_register_20_6_2771),
    .I1(I2C_byte_out_cmp_eq0020),
    .I2(genlock_register_21_6_2780),
    .I3(I2C_byte_out_cmp_eq0021),
    .O(\serial_interface/send_byte_mux0001<6>67_3324 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<6>72  (
    .I0(genlock_register_22_6_2789),
    .I1(I2C_byte_out_cmp_eq0022),
    .I2(genlock_register_2_6_2798),
    .I3(I2C_byte_out_cmp_eq0002),
    .O(\serial_interface/send_byte_mux0001<6>72_3325 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<6>82  (
    .I0(\serial_interface/send_byte_mux0001<6>50_3322 ),
    .I1(\serial_interface/send_byte_mux0001<6>55_3323 ),
    .I2(\serial_interface/send_byte_mux0001<6>67_3324 ),
    .I3(\serial_interface/send_byte_mux0001<6>72_3325 ),
    .O(\serial_interface/send_byte_mux0001<6>82_3326 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<6>106  (
    .I0(genlock_register_3_6_2807),
    .I1(I2C_byte_out_cmp_eq0003),
    .I2(genlock_register_4_6_2816),
    .I3(I2C_byte_out_cmp_eq0004),
    .O(\serial_interface/send_byte_mux0001<6>106_3311 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<6>117  (
    .I0(genlock_register_6_6_2834),
    .I1(I2C_byte_out_cmp_eq0006),
    .I2(genlock_register_7_6_2843),
    .I3(I2C_byte_out_cmp_eq0007),
    .O(\serial_interface/send_byte_mux0001<6>117_3312 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<6>131  (
    .I0(genlock_register_8_6_2852),
    .I1(I2C_byte_out_cmp_eq0008),
    .I2(genlock_register_9_6_2861),
    .I3(I2C_byte_out_cmp_eq0009),
    .O(\serial_interface/send_byte_mux0001<6>131_3313 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<6>136  (
    .I0(I2C_byte_out_cmp_eq0026),
    .I1(\OCXO_pll/phase_detect/phase_diff [6]),
    .I2(genlock_register_0_6_2663),
    .I3(I2C_byte_out_cmp_eq0000),
    .O(\serial_interface/send_byte_mux0001<6>136_3314 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<2>5  (
    .I0(genlock_register_10_2_2668),
    .I1(I2C_byte_out_cmp_eq0010),
    .I2(I2C_byte_out_cmp_eq0027),
    .I3(\OCXO_pll/phase_detect/phase_diff [10]),
    .O(\serial_interface/send_byte_mux0001<2>5_3235 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<2>10  (
    .I0(genlock_register_11_2_2677),
    .I1(I2C_byte_out_cmp_eq0011),
    .I2(genlock_register_12_2_2686),
    .I3(I2C_byte_out_cmp_eq0012),
    .O(\serial_interface/send_byte_mux0001<2>10_3224 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<2>22  (
    .I0(genlock_register_13_2_2695),
    .I1(I2C_byte_out_cmp_eq0013),
    .I2(genlock_register_14_2_2704),
    .I3(I2C_byte_out_cmp_eq0014),
    .O(\serial_interface/send_byte_mux0001<2>22_3232 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<2>27  (
    .I0(genlock_register_15_2_2713),
    .I1(I2C_byte_out_cmp_eq0015),
    .I2(genlock_register_16_2_2722),
    .I3(I2C_byte_out_cmp_eq0016),
    .O(\serial_interface/send_byte_mux0001<2>27_3233 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<2>37  (
    .I0(\serial_interface/send_byte_mux0001<2>5_3235 ),
    .I1(\serial_interface/send_byte_mux0001<2>10_3224 ),
    .I2(\serial_interface/send_byte_mux0001<2>22_3232 ),
    .I3(\serial_interface/send_byte_mux0001<2>27_3233 ),
    .O(\serial_interface/send_byte_mux0001<2>37_3234 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<2>50  (
    .I0(genlock_register_17_2_2731),
    .I1(I2C_byte_out_cmp_eq0017),
    .I2(genlock_register_18_2_2740),
    .I3(I2C_byte_out_cmp_eq0018),
    .O(\serial_interface/send_byte_mux0001<2>50_3236 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<2>55  (
    .I0(genlock_register_19_2_2749),
    .I1(I2C_byte_out_cmp_eq0019),
    .I2(genlock_register_1_2_2758),
    .I3(I2C_byte_out_cmp_eq0001),
    .O(\serial_interface/send_byte_mux0001<2>55_3237 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<2>67  (
    .I0(genlock_register_20_2_2767),
    .I1(I2C_byte_out_cmp_eq0020),
    .I2(genlock_register_21_2_2776),
    .I3(I2C_byte_out_cmp_eq0021),
    .O(\serial_interface/send_byte_mux0001<2>67_3238 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<2>72  (
    .I0(genlock_register_22_2_2785),
    .I1(I2C_byte_out_cmp_eq0022),
    .I2(genlock_register_2_2_2794),
    .I3(I2C_byte_out_cmp_eq0002),
    .O(\serial_interface/send_byte_mux0001<2>72_3239 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<2>82  (
    .I0(\serial_interface/send_byte_mux0001<2>50_3236 ),
    .I1(\serial_interface/send_byte_mux0001<2>55_3237 ),
    .I2(\serial_interface/send_byte_mux0001<2>67_3238 ),
    .I3(\serial_interface/send_byte_mux0001<2>72_3239 ),
    .O(\serial_interface/send_byte_mux0001<2>82_3240 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<2>106  (
    .I0(genlock_register_3_2_2803),
    .I1(I2C_byte_out_cmp_eq0003),
    .I2(genlock_register_4_2_2812),
    .I3(I2C_byte_out_cmp_eq0004),
    .O(\serial_interface/send_byte_mux0001<2>106_3225 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<2>117  (
    .I0(genlock_register_6_2_2830),
    .I1(I2C_byte_out_cmp_eq0006),
    .I2(genlock_register_7_2_2839),
    .I3(I2C_byte_out_cmp_eq0007),
    .O(\serial_interface/send_byte_mux0001<2>117_3226 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<2>131  (
    .I0(genlock_register_8_2_2848),
    .I1(I2C_byte_out_cmp_eq0008),
    .I2(genlock_register_9_2_2857),
    .I3(I2C_byte_out_cmp_eq0009),
    .O(\serial_interface/send_byte_mux0001<2>131_3227 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<2>136  (
    .I0(I2C_byte_out_cmp_eq0026),
    .I1(\OCXO_pll/phase_detect/phase_diff [2]),
    .I2(genlock_register_0_2_2659),
    .I3(I2C_byte_out_cmp_eq0000),
    .O(\serial_interface/send_byte_mux0001<2>136_3228 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<0>5  (
    .I0(genlock_register_0_0_2657),
    .I1(I2C_byte_out_cmp_eq0000),
    .I2(I2C_byte_out_cmp_eq0027),
    .I3(\OCXO_pll/phase_detect/phase_diff [8]),
    .O(\serial_interface/send_byte_mux0001<0>5_3193 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<0>10  (
    .I0(genlock_register_10_0_2666),
    .I1(I2C_byte_out_cmp_eq0010),
    .I2(genlock_register_11_0_2675),
    .I3(I2C_byte_out_cmp_eq0011),
    .O(\serial_interface/send_byte_mux0001<0>10_3182 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<0>22  (
    .I0(genlock_register_12_0_2684),
    .I1(I2C_byte_out_cmp_eq0012),
    .I2(genlock_register_13_0_2693),
    .I3(I2C_byte_out_cmp_eq0013),
    .O(\serial_interface/send_byte_mux0001<0>22_3190 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<0>27  (
    .I0(genlock_register_14_0_2702),
    .I1(I2C_byte_out_cmp_eq0014),
    .I2(genlock_register_15_0_2711),
    .I3(I2C_byte_out_cmp_eq0015),
    .O(\serial_interface/send_byte_mux0001<0>27_3191 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<0>37  (
    .I0(\serial_interface/send_byte_mux0001<0>5_3193 ),
    .I1(\serial_interface/send_byte_mux0001<0>10_3182 ),
    .I2(\serial_interface/send_byte_mux0001<0>22_3190 ),
    .I3(\serial_interface/send_byte_mux0001<0>27_3191 ),
    .O(\serial_interface/send_byte_mux0001<0>37_3192 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<0>50  (
    .I0(genlock_register_16_0_2720),
    .I1(I2C_byte_out_cmp_eq0016),
    .I2(genlock_register_17_0_2729),
    .I3(I2C_byte_out_cmp_eq0017),
    .O(\serial_interface/send_byte_mux0001<0>50_3194 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<0>55  (
    .I0(genlock_register_18_0_2738),
    .I1(I2C_byte_out_cmp_eq0018),
    .I2(genlock_register_19_0_2747),
    .I3(I2C_byte_out_cmp_eq0019),
    .O(\serial_interface/send_byte_mux0001<0>55_3195 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<0>67  (
    .I0(genlock_register_1_0_2756),
    .I1(I2C_byte_out_cmp_eq0001),
    .I2(genlock_register_20_0_2765),
    .I3(I2C_byte_out_cmp_eq0020),
    .O(\serial_interface/send_byte_mux0001<0>67_3196 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<0>72  (
    .I0(genlock_register_21_0_2774),
    .I1(I2C_byte_out_cmp_eq0021),
    .I2(genlock_register_22_0_2783),
    .I3(I2C_byte_out_cmp_eq0022),
    .O(\serial_interface/send_byte_mux0001<0>72_3197 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<0>82  (
    .I0(\serial_interface/send_byte_mux0001<0>50_3194 ),
    .I1(\serial_interface/send_byte_mux0001<0>55_3195 ),
    .I2(\serial_interface/send_byte_mux0001<0>67_3196 ),
    .I3(\serial_interface/send_byte_mux0001<0>72_3197 ),
    .O(\serial_interface/send_byte_mux0001<0>82_3198 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<0>106  (
    .I0(genlock_register_2_0_2792),
    .I1(I2C_byte_out_cmp_eq0002),
    .I2(genlock_register_3_0_2801),
    .I3(I2C_byte_out_cmp_eq0003),
    .O(\serial_interface/send_byte_mux0001<0>106_3183 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<0>117  (
    .I0(genlock_register_5_0_2819),
    .I1(I2C_byte_out_cmp_eq0005),
    .I2(genlock_register_6_0_2828),
    .I3(I2C_byte_out_cmp_eq0006),
    .O(\serial_interface/send_byte_mux0001<0>117_3184 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<0>131  (
    .I0(genlock_register_7_0_2837),
    .I1(I2C_byte_out_cmp_eq0007),
    .I2(genlock_register_8_0_2846),
    .I3(I2C_byte_out_cmp_eq0008),
    .O(\serial_interface/send_byte_mux0001<0>131_3185 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<7>14  (
    .I0(I2C_byte_out_cmp_eq0003),
    .I1(genlock_register_3_7_2808),
    .I2(I2C_byte_out_cmp_eq0004),
    .I3(genlock_register_4_7_2817),
    .O(\serial_interface/send_byte_mux0001<7>14_3333 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<7>19  (
    .I0(I2C_byte_out_cmp_eq0005),
    .I1(genlock_register_5_7_2826),
    .I2(I2C_byte_out_cmp_eq0006),
    .I3(genlock_register_6_7_2835),
    .O(\serial_interface/send_byte_mux0001<7>19_3336 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<7>29  (
    .I0(I2C_byte_out_or0000_27),
    .I1(\serial_interface/send_byte_mux0001<7>5_3346 ),
    .I2(\serial_interface/send_byte_mux0001<7>14_3333 ),
    .I3(\serial_interface/send_byte_mux0001<7>19_3336 ),
    .O(\serial_interface/send_byte_mux0001<7>29_3342 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<7>43  (
    .I0(I2C_byte_out_cmp_eq0007),
    .I1(genlock_register_7_7_2844),
    .I2(I2C_byte_out_cmp_eq0008),
    .I3(genlock_register_8_7_2853),
    .O(\serial_interface/send_byte_mux0001<7>43_3343 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<7>48  (
    .I0(I2C_byte_out_cmp_eq0009),
    .I1(genlock_register_9_7_2862),
    .I2(I2C_byte_out_cmp_eq0010),
    .I3(genlock_register_10_7_2673),
    .O(\serial_interface/send_byte_mux0001<7>48_3344 )
  );
  LUT2 #(
    .INIT ( 4'hE ))
  \serial_interface/send_byte_mux0001<7>49  (
    .I0(\serial_interface/send_byte_mux0001<7>43_3343 ),
    .I1(\serial_interface/send_byte_mux0001<7>48_3344 ),
    .O(\serial_interface/send_byte_mux0001<7>49_3345 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<7>72  (
    .I0(I2C_byte_out_cmp_eq0011),
    .I1(genlock_register_11_7_2682),
    .I2(I2C_byte_out_cmp_eq0012),
    .I3(genlock_register_12_7_2691),
    .O(\serial_interface/send_byte_mux0001<7>72_3347 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<7>77  (
    .I0(I2C_byte_out_cmp_eq0013),
    .I1(genlock_register_13_7_2700),
    .I2(I2C_byte_out_cmp_eq0014),
    .I3(genlock_register_14_7_2709),
    .O(\serial_interface/send_byte_mux0001<7>77_3348 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<7>89  (
    .I0(I2C_byte_out_cmp_eq0015),
    .I1(genlock_register_15_7_2718),
    .I2(I2C_byte_out_cmp_eq0016),
    .I3(genlock_register_16_7_2727),
    .O(\serial_interface/send_byte_mux0001<7>89_3349 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<7>94  (
    .I0(I2C_byte_out_cmp_eq0017),
    .I1(genlock_register_17_7_2736),
    .I2(I2C_byte_out_cmp_eq0018),
    .I3(genlock_register_18_7_2745),
    .O(\serial_interface/send_byte_mux0001<7>94_3350 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<7>104  (
    .I0(\serial_interface/send_byte_mux0001<7>72_3347 ),
    .I1(\serial_interface/send_byte_mux0001<7>77_3348 ),
    .I2(\serial_interface/send_byte_mux0001<7>89_3349 ),
    .I3(\serial_interface/send_byte_mux0001<7>94_3350 ),
    .O(\serial_interface/send_byte_mux0001<7>104_3328 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<7>117  (
    .I0(I2C_byte_out_cmp_eq0019),
    .I1(genlock_register_19_7_2754),
    .I2(I2C_byte_out_cmp_eq0020),
    .I3(genlock_register_20_7_2772),
    .O(\serial_interface/send_byte_mux0001<7>117_3329 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<7>122  (
    .I0(I2C_byte_out_cmp_eq0021),
    .I1(genlock_register_21_7_2781),
    .I2(I2C_byte_out_cmp_eq0022),
    .I3(genlock_register_22_7_2790),
    .O(\serial_interface/send_byte_mux0001<7>122_3330 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<7>134  (
    .I0(I2C_byte_out_cmp_eq0026),
    .I1(\OCXO_pll/phase_detect/phase_diff [7]),
    .I2(I2C_byte_out_cmp_eq0027),
    .I3(\OCXO_pll/phase_detect/phase_diff [15]),
    .O(\serial_interface/send_byte_mux0001<7>134_3331 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<7>139  (
    .I0(I2C_byte_out_cmp_eq0000),
    .I1(genlock_register_0_7_2664),
    .I2(I2C_byte_out_cmp_eq0001),
    .I3(genlock_register_1_7_2763),
    .O(\serial_interface/send_byte_mux0001<7>139_3332 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<7>149  (
    .I0(\serial_interface/send_byte_mux0001<7>117_3329 ),
    .I1(\serial_interface/send_byte_mux0001<7>122_3330 ),
    .I2(\serial_interface/send_byte_mux0001<7>134_3331 ),
    .I3(\serial_interface/send_byte_mux0001<7>139_3332 ),
    .O(\serial_interface/send_byte_mux0001<7>149_3334 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<7>171  (
    .I0(\serial_interface/send_byte_mux0001<7>29_3342 ),
    .I1(\serial_interface/send_byte_mux0001<7>49_3345 ),
    .I2(\serial_interface/send_byte_mux0001<7>104_3328 ),
    .I3(\serial_interface/send_byte_mux0001<7>149_3334 ),
    .O(\serial_interface/send_byte_mux0001<7>171_3335 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<5>14  (
    .I0(I2C_byte_out_cmp_eq0003),
    .I1(genlock_register_3_5_2806),
    .I2(I2C_byte_out_cmp_eq0004),
    .I3(genlock_register_4_5_2815),
    .O(\serial_interface/send_byte_mux0001<5>14_3291 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<5>19  (
    .I0(I2C_byte_out_cmp_eq0005),
    .I1(genlock_register_5_5_2824),
    .I2(I2C_byte_out_cmp_eq0006),
    .I3(genlock_register_6_5_2833),
    .O(\serial_interface/send_byte_mux0001<5>19_3294 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<5>29  (
    .I0(I2C_byte_out_or0000_27),
    .I1(\serial_interface/send_byte_mux0001<5>5_3304 ),
    .I2(\serial_interface/send_byte_mux0001<5>14_3291 ),
    .I3(\serial_interface/send_byte_mux0001<5>19_3294 ),
    .O(\serial_interface/send_byte_mux0001<5>29_3300 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<5>43  (
    .I0(I2C_byte_out_cmp_eq0007),
    .I1(genlock_register_7_5_2842),
    .I2(I2C_byte_out_cmp_eq0008),
    .I3(genlock_register_8_5_2851),
    .O(\serial_interface/send_byte_mux0001<5>43_3301 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<5>48  (
    .I0(I2C_byte_out_cmp_eq0009),
    .I1(genlock_register_9_5_2860),
    .I2(I2C_byte_out_cmp_eq0010),
    .I3(genlock_register_10_5_2671),
    .O(\serial_interface/send_byte_mux0001<5>48_3302 )
  );
  LUT2 #(
    .INIT ( 4'hE ))
  \serial_interface/send_byte_mux0001<5>49  (
    .I0(\serial_interface/send_byte_mux0001<5>43_3301 ),
    .I1(\serial_interface/send_byte_mux0001<5>48_3302 ),
    .O(\serial_interface/send_byte_mux0001<5>49_3303 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<5>72  (
    .I0(I2C_byte_out_cmp_eq0011),
    .I1(genlock_register_11_5_2680),
    .I2(I2C_byte_out_cmp_eq0012),
    .I3(genlock_register_12_5_2689),
    .O(\serial_interface/send_byte_mux0001<5>72_3305 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<5>77  (
    .I0(I2C_byte_out_cmp_eq0013),
    .I1(genlock_register_13_5_2698),
    .I2(I2C_byte_out_cmp_eq0014),
    .I3(genlock_register_14_5_2707),
    .O(\serial_interface/send_byte_mux0001<5>77_3306 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<5>89  (
    .I0(I2C_byte_out_cmp_eq0015),
    .I1(genlock_register_15_5_2716),
    .I2(I2C_byte_out_cmp_eq0016),
    .I3(genlock_register_16_5_2725),
    .O(\serial_interface/send_byte_mux0001<5>89_3307 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<5>94  (
    .I0(I2C_byte_out_cmp_eq0017),
    .I1(genlock_register_17_5_2734),
    .I2(I2C_byte_out_cmp_eq0018),
    .I3(genlock_register_18_5_2743),
    .O(\serial_interface/send_byte_mux0001<5>94_3308 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<5>104  (
    .I0(\serial_interface/send_byte_mux0001<5>72_3305 ),
    .I1(\serial_interface/send_byte_mux0001<5>77_3306 ),
    .I2(\serial_interface/send_byte_mux0001<5>89_3307 ),
    .I3(\serial_interface/send_byte_mux0001<5>94_3308 ),
    .O(\serial_interface/send_byte_mux0001<5>104_3286 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<5>117  (
    .I0(I2C_byte_out_cmp_eq0019),
    .I1(genlock_register_19_5_2752),
    .I2(I2C_byte_out_cmp_eq0020),
    .I3(genlock_register_20_5_2770),
    .O(\serial_interface/send_byte_mux0001<5>117_3287 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<5>122  (
    .I0(I2C_byte_out_cmp_eq0021),
    .I1(genlock_register_21_5_2779),
    .I2(I2C_byte_out_cmp_eq0022),
    .I3(genlock_register_22_5_2788),
    .O(\serial_interface/send_byte_mux0001<5>122_3288 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<5>134  (
    .I0(I2C_byte_out_cmp_eq0026),
    .I1(\OCXO_pll/phase_detect/phase_diff [5]),
    .I2(I2C_byte_out_cmp_eq0027),
    .I3(\OCXO_pll/phase_detect/phase_diff [13]),
    .O(\serial_interface/send_byte_mux0001<5>134_3289 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<5>139  (
    .I0(I2C_byte_out_cmp_eq0000),
    .I1(genlock_register_0_5_2662),
    .I2(I2C_byte_out_cmp_eq0001),
    .I3(genlock_register_1_5_2761),
    .O(\serial_interface/send_byte_mux0001<5>139_3290 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<5>149  (
    .I0(\serial_interface/send_byte_mux0001<5>117_3287 ),
    .I1(\serial_interface/send_byte_mux0001<5>122_3288 ),
    .I2(\serial_interface/send_byte_mux0001<5>134_3289 ),
    .I3(\serial_interface/send_byte_mux0001<5>139_3290 ),
    .O(\serial_interface/send_byte_mux0001<5>149_3292 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<5>171  (
    .I0(\serial_interface/send_byte_mux0001<5>29_3300 ),
    .I1(\serial_interface/send_byte_mux0001<5>49_3303 ),
    .I2(\serial_interface/send_byte_mux0001<5>104_3286 ),
    .I3(\serial_interface/send_byte_mux0001<5>149_3292 ),
    .O(\serial_interface/send_byte_mux0001<5>171_3293 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<3>14  (
    .I0(I2C_byte_out_cmp_eq0003),
    .I1(genlock_register_3_3_2804),
    .I2(I2C_byte_out_cmp_eq0004),
    .I3(genlock_register_4_3_2813),
    .O(\serial_interface/send_byte_mux0001<3>14_3247 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<3>19  (
    .I0(I2C_byte_out_cmp_eq0005),
    .I1(genlock_register_5_3_2822),
    .I2(I2C_byte_out_cmp_eq0006),
    .I3(genlock_register_6_3_2831),
    .O(\serial_interface/send_byte_mux0001<3>19_3253 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<3>29  (
    .I0(I2C_byte_out_or0000_27),
    .I1(\serial_interface/send_byte_mux0001<3>5_3260 ),
    .I2(\serial_interface/send_byte_mux0001<3>14_3247 ),
    .I3(\serial_interface/send_byte_mux0001<3>19_3253 ),
    .O(\serial_interface/send_byte_mux0001<3>29_3256 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<3>43  (
    .I0(I2C_byte_out_cmp_eq0007),
    .I1(genlock_register_7_3_2840),
    .I2(I2C_byte_out_cmp_eq0008),
    .I3(genlock_register_8_3_2849),
    .O(\serial_interface/send_byte_mux0001<3>43_3257 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<3>48  (
    .I0(I2C_byte_out_cmp_eq0009),
    .I1(genlock_register_9_3_2858),
    .I2(I2C_byte_out_cmp_eq0010),
    .I3(genlock_register_10_3_2669),
    .O(\serial_interface/send_byte_mux0001<3>48_3258 )
  );
  LUT2 #(
    .INIT ( 4'hE ))
  \serial_interface/send_byte_mux0001<3>49  (
    .I0(\serial_interface/send_byte_mux0001<3>43_3257 ),
    .I1(\serial_interface/send_byte_mux0001<3>48_3258 ),
    .O(\serial_interface/send_byte_mux0001<3>49_3259 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<3>72  (
    .I0(I2C_byte_out_cmp_eq0011),
    .I1(genlock_register_11_3_2678),
    .I2(I2C_byte_out_cmp_eq0012),
    .I3(genlock_register_12_3_2687),
    .O(\serial_interface/send_byte_mux0001<3>72_3261 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<3>77  (
    .I0(I2C_byte_out_cmp_eq0013),
    .I1(genlock_register_13_3_2696),
    .I2(I2C_byte_out_cmp_eq0014),
    .I3(genlock_register_14_3_2705),
    .O(\serial_interface/send_byte_mux0001<3>77_3262 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<3>89  (
    .I0(I2C_byte_out_cmp_eq0015),
    .I1(genlock_register_15_3_2714),
    .I2(I2C_byte_out_cmp_eq0016),
    .I3(genlock_register_16_3_2723),
    .O(\serial_interface/send_byte_mux0001<3>89_3263 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<3>94  (
    .I0(I2C_byte_out_cmp_eq0017),
    .I1(genlock_register_17_3_2732),
    .I2(I2C_byte_out_cmp_eq0018),
    .I3(genlock_register_18_3_2741),
    .O(\serial_interface/send_byte_mux0001<3>94_3264 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<3>104  (
    .I0(\serial_interface/send_byte_mux0001<3>72_3261 ),
    .I1(\serial_interface/send_byte_mux0001<3>77_3262 ),
    .I2(\serial_interface/send_byte_mux0001<3>89_3263 ),
    .I3(\serial_interface/send_byte_mux0001<3>94_3264 ),
    .O(\serial_interface/send_byte_mux0001<3>104_3242 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<3>117  (
    .I0(I2C_byte_out_cmp_eq0019),
    .I1(genlock_register_19_3_2750),
    .I2(I2C_byte_out_cmp_eq0020),
    .I3(genlock_register_20_3_2768),
    .O(\serial_interface/send_byte_mux0001<3>117_3243 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<3>122  (
    .I0(I2C_byte_out_cmp_eq0021),
    .I1(genlock_register_21_3_2777),
    .I2(I2C_byte_out_cmp_eq0022),
    .I3(genlock_register_22_3_2786),
    .O(\serial_interface/send_byte_mux0001<3>122_3244 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<3>134  (
    .I0(I2C_byte_out_cmp_eq0026),
    .I1(\OCXO_pll/phase_detect/phase_diff [3]),
    .I2(I2C_byte_out_cmp_eq0027),
    .I3(\OCXO_pll/phase_detect/phase_diff [11]),
    .O(\serial_interface/send_byte_mux0001<3>134_3245 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<3>139  (
    .I0(I2C_byte_out_cmp_eq0000),
    .I1(genlock_register_0_3_2660),
    .I2(I2C_byte_out_cmp_eq0001),
    .I3(genlock_register_1_3_2759),
    .O(\serial_interface/send_byte_mux0001<3>139_3246 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<3>149  (
    .I0(\serial_interface/send_byte_mux0001<3>117_3243 ),
    .I1(\serial_interface/send_byte_mux0001<3>122_3244 ),
    .I2(\serial_interface/send_byte_mux0001<3>134_3245 ),
    .I3(\serial_interface/send_byte_mux0001<3>139_3246 ),
    .O(\serial_interface/send_byte_mux0001<3>149_3248 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<3>171  (
    .I0(\serial_interface/send_byte_mux0001<3>29_3256 ),
    .I1(\serial_interface/send_byte_mux0001<3>49_3259 ),
    .I2(\serial_interface/send_byte_mux0001<3>104_3242 ),
    .I3(\serial_interface/send_byte_mux0001<3>149_3248 ),
    .O(\serial_interface/send_byte_mux0001<3>171_3249 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<4>5  (
    .I0(genlock_register_16_4_2724),
    .I1(I2C_byte_out_cmp_eq0016),
    .I2(genlock_register_17_4_2733),
    .I3(I2C_byte_out_cmp_eq0017),
    .O(\serial_interface/send_byte_mux0001<4>5_3279 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<4>10  (
    .I0(genlock_register_18_4_2742),
    .I1(I2C_byte_out_cmp_eq0018),
    .I2(genlock_register_19_4_2751),
    .I3(I2C_byte_out_cmp_eq0019),
    .O(\serial_interface/send_byte_mux0001<4>10_3266 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<4>22  (
    .I0(genlock_register_1_4_2760),
    .I1(I2C_byte_out_cmp_eq0001),
    .I2(genlock_register_20_4_2769),
    .I3(I2C_byte_out_cmp_eq0020),
    .O(\serial_interface/send_byte_mux0001<4>22_3276 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<4>27  (
    .I0(genlock_register_21_4_2778),
    .I1(I2C_byte_out_cmp_eq0021),
    .I2(genlock_register_22_4_2787),
    .I3(I2C_byte_out_cmp_eq0022),
    .O(\serial_interface/send_byte_mux0001<4>27_3277 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<4>37  (
    .I0(\serial_interface/send_byte_mux0001<4>5_3279 ),
    .I1(\serial_interface/send_byte_mux0001<4>10_3266 ),
    .I2(\serial_interface/send_byte_mux0001<4>22_3276 ),
    .I3(\serial_interface/send_byte_mux0001<4>27_3277 ),
    .O(\serial_interface/send_byte_mux0001<4>37_3278 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<4>50  (
    .I0(genlock_register_12_4_2688),
    .I1(I2C_byte_out_cmp_eq0012),
    .I2(genlock_register_13_4_2697),
    .I3(I2C_byte_out_cmp_eq0013),
    .O(\serial_interface/send_byte_mux0001<4>50_3280 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<4>55  (
    .I0(genlock_register_14_4_2706),
    .I1(I2C_byte_out_cmp_eq0014),
    .I2(genlock_register_15_4_2715),
    .I3(I2C_byte_out_cmp_eq0015),
    .O(\serial_interface/send_byte_mux0001<4>55_3281 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<4>67  (
    .I0(genlock_register_10_4_2670),
    .I1(I2C_byte_out_cmp_eq0010),
    .I2(genlock_register_11_4_2679),
    .I3(I2C_byte_out_cmp_eq0011),
    .O(\serial_interface/send_byte_mux0001<4>67_3282 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<4>86  (
    .I0(\serial_interface/send_byte_mux0001<4>50_3280 ),
    .I1(\serial_interface/send_byte_mux0001<4>55_3281 ),
    .I2(\serial_interface/send_byte_mux0001<4>67_3282 ),
    .I3(\serial_interface/send_byte_mux0001<4>75_3283 ),
    .O(\serial_interface/send_byte_mux0001<4>86_3284 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<4>110  (
    .I0(genlock_register_2_4_2796),
    .I1(I2C_byte_out_cmp_eq0002),
    .I2(genlock_register_3_4_2805),
    .I3(I2C_byte_out_cmp_eq0003),
    .O(\serial_interface/send_byte_mux0001<4>110_3267 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<4>115  (
    .I0(genlock_register_4_4_2814),
    .I1(I2C_byte_out_cmp_eq0004),
    .I2(genlock_register_5_4_2823),
    .I3(I2C_byte_out_cmp_eq0005),
    .O(\serial_interface/send_byte_mux0001<4>115_3268 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<4>127  (
    .I0(genlock_register_6_4_2832),
    .I1(I2C_byte_out_cmp_eq0006),
    .I2(genlock_register_7_4_2841),
    .I3(I2C_byte_out_cmp_eq0007),
    .O(\serial_interface/send_byte_mux0001<4>127_3269 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<4>143  (
    .I0(genlock_register_8_4_2850),
    .I1(I2C_byte_out_cmp_eq0008),
    .I2(genlock_register_9_4_2859),
    .I3(I2C_byte_out_cmp_eq0009),
    .O(\serial_interface/send_byte_mux0001<4>143_3270 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<4>148  (
    .I0(I2C_byte_out_cmp_eq0026),
    .I1(\OCXO_pll/phase_detect/phase_diff [4]),
    .I2(I2C_byte_out_cmp_eq0027),
    .I3(\OCXO_pll/phase_detect/phase_diff [12]),
    .O(\serial_interface/send_byte_mux0001<4>148_3271 )
  );
  LUT2 #(
    .INIT ( 4'hE ))
  \serial_interface/send_byte_mux0001<4>149  (
    .I0(\serial_interface/send_byte_mux0001<4>143_3270 ),
    .I1(\serial_interface/send_byte_mux0001<4>148_3271 ),
    .O(\serial_interface/send_byte_mux0001<4>149_3272 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<4>158  (
    .I0(\serial_interface/send_byte_mux0001<4>110_3267 ),
    .I1(\serial_interface/send_byte_mux0001<4>115_3268 ),
    .I2(\serial_interface/send_byte_mux0001<4>127_3269 ),
    .I3(\serial_interface/send_byte_mux0001<4>149_3272 ),
    .O(\serial_interface/send_byte_mux0001<4>158_3273 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<1>14  (
    .I0(I2C_byte_out_cmp_eq0003),
    .I1(genlock_register_3_1_2802),
    .I2(I2C_byte_out_cmp_eq0004),
    .I3(genlock_register_4_1_2811),
    .O(\serial_interface/send_byte_mux0001<1>14_3205 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<1>19  (
    .I0(I2C_byte_out_cmp_eq0005),
    .I1(genlock_register_5_1_2820),
    .I2(I2C_byte_out_cmp_eq0006),
    .I3(genlock_register_6_1_2829),
    .O(\serial_interface/send_byte_mux0001<1>19_3208 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<1>29  (
    .I0(I2C_byte_out_or0000_27),
    .I1(\serial_interface/send_byte_mux0001<1>5_3218 ),
    .I2(\serial_interface/send_byte_mux0001<1>14_3205 ),
    .I3(\serial_interface/send_byte_mux0001<1>19_3208 ),
    .O(\serial_interface/send_byte_mux0001<1>29_3214 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<1>43  (
    .I0(I2C_byte_out_cmp_eq0007),
    .I1(genlock_register_7_1_2838),
    .I2(I2C_byte_out_cmp_eq0008),
    .I3(genlock_register_8_1_2847),
    .O(\serial_interface/send_byte_mux0001<1>43_3215 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<1>48  (
    .I0(I2C_byte_out_cmp_eq0009),
    .I1(genlock_register_9_1_2856),
    .I2(I2C_byte_out_cmp_eq0010),
    .I3(genlock_register_10_1_2667),
    .O(\serial_interface/send_byte_mux0001<1>48_3216 )
  );
  LUT2 #(
    .INIT ( 4'hE ))
  \serial_interface/send_byte_mux0001<1>49  (
    .I0(\serial_interface/send_byte_mux0001<1>43_3215 ),
    .I1(\serial_interface/send_byte_mux0001<1>48_3216 ),
    .O(\serial_interface/send_byte_mux0001<1>49_3217 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<1>72  (
    .I0(I2C_byte_out_cmp_eq0011),
    .I1(genlock_register_11_1_2676),
    .I2(I2C_byte_out_cmp_eq0012),
    .I3(genlock_register_12_1_2685),
    .O(\serial_interface/send_byte_mux0001<1>72_3219 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<1>77  (
    .I0(I2C_byte_out_cmp_eq0013),
    .I1(genlock_register_13_1_2694),
    .I2(I2C_byte_out_cmp_eq0014),
    .I3(genlock_register_14_1_2703),
    .O(\serial_interface/send_byte_mux0001<1>77_3220 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<1>89  (
    .I0(I2C_byte_out_cmp_eq0015),
    .I1(genlock_register_15_1_2712),
    .I2(I2C_byte_out_cmp_eq0016),
    .I3(genlock_register_16_1_2721),
    .O(\serial_interface/send_byte_mux0001<1>89_3221 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<1>94  (
    .I0(I2C_byte_out_cmp_eq0017),
    .I1(genlock_register_17_1_2730),
    .I2(I2C_byte_out_cmp_eq0018),
    .I3(genlock_register_18_1_2739),
    .O(\serial_interface/send_byte_mux0001<1>94_3222 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<1>104  (
    .I0(\serial_interface/send_byte_mux0001<1>72_3219 ),
    .I1(\serial_interface/send_byte_mux0001<1>77_3220 ),
    .I2(\serial_interface/send_byte_mux0001<1>89_3221 ),
    .I3(\serial_interface/send_byte_mux0001<1>94_3222 ),
    .O(\serial_interface/send_byte_mux0001<1>104_3200 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<1>117  (
    .I0(I2C_byte_out_cmp_eq0019),
    .I1(genlock_register_19_1_2748),
    .I2(I2C_byte_out_cmp_eq0020),
    .I3(genlock_register_20_1_2766),
    .O(\serial_interface/send_byte_mux0001<1>117_3201 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<1>122  (
    .I0(I2C_byte_out_cmp_eq0021),
    .I1(genlock_register_21_1_2775),
    .I2(I2C_byte_out_cmp_eq0022),
    .I3(genlock_register_22_1_2784),
    .O(\serial_interface/send_byte_mux0001<1>122_3202 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<1>134  (
    .I0(I2C_byte_out_cmp_eq0026),
    .I1(\OCXO_pll/phase_detect/phase_diff [1]),
    .I2(I2C_byte_out_cmp_eq0027),
    .I3(\OCXO_pll/phase_detect/phase_diff [9]),
    .O(\serial_interface/send_byte_mux0001<1>134_3203 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \serial_interface/send_byte_mux0001<1>139  (
    .I0(I2C_byte_out_cmp_eq0000),
    .I1(genlock_register_0_1_2658),
    .I2(I2C_byte_out_cmp_eq0001),
    .I3(genlock_register_1_1_2757),
    .O(\serial_interface/send_byte_mux0001<1>139_3204 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<1>149  (
    .I0(\serial_interface/send_byte_mux0001<1>117_3201 ),
    .I1(\serial_interface/send_byte_mux0001<1>122_3202 ),
    .I2(\serial_interface/send_byte_mux0001<1>134_3203 ),
    .I3(\serial_interface/send_byte_mux0001<1>139_3204 ),
    .O(\serial_interface/send_byte_mux0001<1>149_3206 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \serial_interface/send_byte_mux0001<1>171  (
    .I0(\serial_interface/send_byte_mux0001<1>29_3214 ),
    .I1(\serial_interface/send_byte_mux0001<1>49_3217 ),
    .I2(\serial_interface/send_byte_mux0001<1>104_3200 ),
    .I3(\serial_interface/send_byte_mux0001<1>149_3206 ),
    .O(\serial_interface/send_byte_mux0001<1>171_3207 )
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \dac_OCXO/shift_reg_mux0000<23>1  (
    .I0(\dac_OCXO/shift_reg [22]),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .O(\dac_OCXO/shift_reg_mux0000 [23])
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \dac_OCXO/shift_reg_mux0000<22>1  (
    .I0(\dac_OCXO/shift_reg [21]),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .O(\dac_OCXO/shift_reg_mux0000 [22])
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \dac_OCXO/shift_reg_mux0000<21>1  (
    .I0(\dac_OCXO/shift_reg [20]),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .O(\dac_OCXO/shift_reg_mux0000 [21])
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \dac_OCXO/shift_reg_mux0000<20>1  (
    .I0(\dac_OCXO/shift_reg [19]),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .O(\dac_OCXO/shift_reg_mux0000 [20])
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \dac_OCXO/shift_reg_mux0000<19>1  (
    .I0(\dac_OCXO/shift_reg [18]),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .O(\dac_OCXO/shift_reg_mux0000 [19])
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \dac_OCXO/shift_reg_mux0000<18>1  (
    .I0(\dac_OCXO/shift_reg [17]),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .O(\dac_OCXO/shift_reg_mux0000 [18])
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \dac_OCXO/shift_reg_mux0000<17>1  (
    .I0(\dac_OCXO/shift_reg [16]),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .O(\dac_OCXO/shift_reg_mux0000 [17])
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \dac_OCXO/shift_reg_mux0000<16>1  (
    .I0(\dac_OCXO/shift_reg [15]),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .O(\dac_OCXO/shift_reg_mux0000 [16])
  );
  LUT2 #(
    .INIT ( 4'hE ))
  \OCXO_pll/phase_loop/integrator_not00021  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(\system_control[2] ),
    .O(\OCXO_pll/phase_loop/integrator_not0002 )
  );
  LUT3 #(
    .INIT ( 8'hAE ))
  \dac_OCXO/shift_reg_not00011  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(\dac_OCXO/bit_tick_2437 ),
    .I2(\dac_OCXO/zsync_2488 ),
    .O(\dac_OCXO/shift_reg_not0001 )
  );
  LUT2 #(
    .INIT ( 4'hD ))
  kvant_pps_not00011 (
    .I0(pps_edge[0]),
    .I1(pps_edge[1]),
    .O(kvant_pps_not0001)
  );
  LUT4 #(
    .INIT ( 16'hFEFF ))
  \dac_OCXO/bit_tick_or00001  (
    .I0(\dac_OCXO/bit_count [1]),
    .I1(\dac_OCXO/bit_count [2]),
    .I2(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I3(\dac_OCXO/bit_count [0]),
    .O(\dac_OCXO/bit_tick_or0000 )
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \OCXO_pll/phase_detect/int_pps_tick_cmp_eq000032  (
    .I0(\OCXO_pll/phase_detect/phase_counter [26]),
    .I1(\OCXO_pll/phase_detect/phase_counter [7]),
    .I2(\OCXO_pll/phase_detect/phase_counter [8]),
    .I3(\OCXO_pll/phase_detect/phase_counter [10]),
    .O(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq000032_1508 )
  );
  LUT3 #(
    .INIT ( 8'h80 ))
  \OCXO_pll/phase_detect/int_pps_tick_cmp_eq000047  (
    .I0(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy [4]),
    .I1(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq000017 ),
    .I2(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq000032_1508 ),
    .O(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq0000 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \OCXO_pll/phase_detect/phase_counter_and00001  (
    .I0(\system_control[2] ),
    .I1(kvant_pps_3018),
    .O(\OCXO_pll/phase_detect/phase_counter_and0000 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \dac_OCXO/bit_count_not00014  (
    .I0(\dac_OCXO/bit_count [7]),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I2(\dac_OCXO/bit_count [3]),
    .I3(\dac_OCXO/bit_count [4]),
    .O(\dac_OCXO/bit_count_not00014_2435 )
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  \VCXO_pll/ref_clk_count_cmp_eq000019  (
    .I0(\VCXO_pll/ref_clk_count [8]),
    .I1(\VCXO_pll/ref_clk_count [4]),
    .I2(\VCXO_pll/ref_clk_count [1]),
    .I3(\VCXO_pll/ref_clk_count [5]),
    .O(\VCXO_pll/ref_clk_count_cmp_eq000019_2302 )
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \VCXO_pll/ref_clk_count_cmp_eq000020  (
    .I0(\VCXO_pll/ref_clk_count_cmp_eq00009 ),
    .I1(\VCXO_pll/ref_clk_count_cmp_eq000019_2302 ),
    .O(\VCXO_pll/ref_clk_count_cmp_eq0000 )
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \OCXO_pll/phase_detect/phase_counter_cmp_eq00004  (
    .I0(\OCXO_pll/phase_detect/phase_counter [7]),
    .I1(\OCXO_pll/phase_detect/phase_counter [8]),
    .I2(\OCXO_pll/phase_detect/phase_counter [16]),
    .I3(\OCXO_pll/phase_detect/phase_counter [18]),
    .O(\OCXO_pll/phase_detect/phase_counter_cmp_eq00004_1541 )
  );
  LUT3 #(
    .INIT ( 8'h80 ))
  \OCXO_pll/phase_detect/phase_counter_cmp_eq000022  (
    .I0(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy [4]),
    .I1(\OCXO_pll/phase_detect/phase_counter_cmp_eq00004_1541 ),
    .I2(\OCXO_pll/phase_detect/phase_counter_cmp_eq000011 ),
    .O(\OCXO_pll/phase_detect/phase_counter_cmp_eq0000 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_mux0003281  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [27]),
    .I2(\OCXO_pll/phase_detect/phase_diff [26]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[34] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_mux0003241  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [27]),
    .I2(\OCXO_pll/phase_detect/phase_diff [22]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[30] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_mux0003221  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [27]),
    .I2(\OCXO_pll/phase_detect/phase_diff [21]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[29] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_mux0003211  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [27]),
    .I2(\OCXO_pll/phase_detect/phase_diff [20]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[28] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_mux0003201  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [27]),
    .I2(\OCXO_pll/phase_detect/phase_diff [19]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[27] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_mux0003191  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [26]),
    .I2(\OCXO_pll/phase_detect/phase_diff [18]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[26] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_mux0003181  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [25]),
    .I2(\OCXO_pll/phase_detect/phase_diff [17]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[25] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_mux0003171  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [24]),
    .I2(\OCXO_pll/phase_detect/phase_diff [16]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[24] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_mux0003151  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [22]),
    .I2(\OCXO_pll/phase_detect/phase_diff [14]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[22] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_mux0003131  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [20]),
    .I2(\OCXO_pll/phase_detect/phase_diff [12]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[20] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_mux0003101  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [18]),
    .I2(\OCXO_pll/phase_detect/phase_diff [10]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[18] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_mux000391  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [17]),
    .I2(\OCXO_pll/phase_detect/phase_diff [9]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[17] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_mux000381  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [16]),
    .I2(\OCXO_pll/phase_detect/phase_diff [8]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[16] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_mux000331  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [11]),
    .I2(\OCXO_pll/phase_detect/phase_diff [3]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[11] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_mux0003361  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [9]),
    .I2(\OCXO_pll/phase_detect/phase_diff [1]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[9] )
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \OCXO_pll/phase_loop/integrator_mux0002<0>1  (
    .I0(\OCXO_pll/phase_loop/integrator_addsub0000 [0]),
    .I1(\system_control[2] ),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [0])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/integrator_mux0003<33>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [27]),
    .I2(\OCXO_pll/phase_detect/phase_diff [25]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[33] )
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \OCXO_pll/phase_loop/integrator_mux0002<1>1  (
    .I0(\OCXO_pll/phase_loop/integrator_addsub0000 [1]),
    .I1(\system_control[2] ),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [1])
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \OCXO_pll/phase_loop/integrator_mux0002<2>1  (
    .I0(\OCXO_pll/phase_loop/integrator_addsub0000 [2]),
    .I1(\system_control[2] ),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [2])
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \OCXO_pll/phase_loop/integrator_mux0002<3>1  (
    .I0(\OCXO_pll/phase_loop/integrator_addsub0000 [3]),
    .I1(\system_control[2] ),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [3])
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \OCXO_pll/phase_loop/integrator_mux0002<4>1  (
    .I0(\OCXO_pll/phase_loop/integrator_addsub0000 [4]),
    .I1(\system_control[2] ),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [4])
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \OCXO_pll/phase_loop/integrator_mux0002<5>1  (
    .I0(\OCXO_pll/phase_loop/integrator_addsub0000 [5]),
    .I1(\system_control[2] ),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [5])
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \OCXO_pll/phase_loop/integrator_mux0002<6>1  (
    .I0(\OCXO_pll/phase_loop/integrator_addsub0000 [6]),
    .I1(\system_control[2] ),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [6])
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \OCXO_pll/phase_loop/integrator_mux0002<7>1  (
    .I0(\OCXO_pll/phase_loop/integrator_addsub0000 [7]),
    .I1(\system_control[2] ),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [7])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<8>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [8]),
    .I2(genlock_register_21_0_2774),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [8])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/integrator_mux0003<32>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [27]),
    .I2(\OCXO_pll/phase_detect/phase_diff [24]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[32] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<9>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [9]),
    .I2(genlock_register_21_1_2775),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [9])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/integrator_mux0003<31>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [27]),
    .I2(\OCXO_pll/phase_detect/phase_diff [23]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003[31] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<10>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [10]),
    .I2(genlock_register_21_2_2776),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [10])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/out_sum_mux0001<24>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [26]),
    .I2(\OCXO_pll/phase_detect/phase_diff [22]),
    .O(\OCXO_pll/phase_loop/out_sum_mux0001[24] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<11>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [11]),
    .I2(genlock_register_21_3_2777),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [11])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/out_sum_mux0001<23>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [25]),
    .I2(\OCXO_pll/phase_detect/phase_diff [21]),
    .O(\OCXO_pll/phase_loop/out_sum_mux0001[23] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<12>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [12]),
    .I2(genlock_register_21_4_2778),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [12])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/out_sum_mux0001<22>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [24]),
    .I2(\OCXO_pll/phase_detect/phase_diff [20]),
    .O(\OCXO_pll/phase_loop/out_sum_mux0001[22] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<13>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [13]),
    .I2(genlock_register_21_5_2779),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [13])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/out_sum_mux0001<21>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [23]),
    .I2(\OCXO_pll/phase_detect/phase_diff [19]),
    .O(\OCXO_pll/phase_loop/out_sum_mux0001[21] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<14>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [14]),
    .I2(genlock_register_21_6_2780),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [14])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/out_sum_mux0001<20>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [22]),
    .I2(\OCXO_pll/phase_detect/phase_diff [18]),
    .O(\OCXO_pll/phase_loop/out_sum_mux0001[20] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<15>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [15]),
    .I2(genlock_register_21_7_2781),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [15])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/out_sum_mux0001<19>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [21]),
    .I2(\OCXO_pll/phase_detect/phase_diff [17]),
    .O(\OCXO_pll/phase_loop/out_sum_mux0001[19] )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<16>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [16]),
    .I2(genlock_register_22_0_2783),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [16])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/out_sum_mux0001<18>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [20]),
    .I2(\OCXO_pll/phase_detect/phase_diff [16]),
    .O(\OCXO_pll/phase_loop/out_sum_mux0001[18] )
  );
  LUT3 #(
    .INIT ( 8'hB8 ))
  \OCXO_pll/phase_loop/integrator_mux0002<17>1  (
    .I0(genlock_register_22_1_2784),
    .I1(\system_control[2] ),
    .I2(\OCXO_pll/phase_loop/integrator_addsub0000 [17]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [17])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/out_sum_mux0001<17>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [19]),
    .I2(\OCXO_pll/phase_detect/phase_diff [15]),
    .O(\OCXO_pll/phase_loop/out_sum_mux0001[17] )
  );
  LUT3 #(
    .INIT ( 8'hB8 ))
  \OCXO_pll/phase_loop/integrator_mux0002<18>1  (
    .I0(genlock_register_22_2_2785),
    .I1(\system_control[2] ),
    .I2(\OCXO_pll/phase_loop/integrator_addsub0000 [18]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [18])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/out_sum_mux0001<16>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [18]),
    .I2(\OCXO_pll/phase_detect/phase_diff [14]),
    .O(\OCXO_pll/phase_loop/out_sum_mux0001[16] )
  );
  LUT3 #(
    .INIT ( 8'hB8 ))
  \OCXO_pll/phase_loop/integrator_mux0002<19>1  (
    .I0(genlock_register_22_3_2786),
    .I1(\system_control[2] ),
    .I2(\OCXO_pll/phase_loop/integrator_addsub0000 [19]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [19])
  );
  LUT3 #(
    .INIT ( 8'hB8 ))
  \OCXO_pll/phase_loop/integrator_mux0002<20>1  (
    .I0(genlock_register_22_4_2787),
    .I1(\system_control[2] ),
    .I2(\OCXO_pll/phase_loop/integrator_addsub0000 [20]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [20])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/out_sum_mux0001<14>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [16]),
    .I2(\OCXO_pll/phase_detect/phase_diff [12]),
    .O(\OCXO_pll/phase_loop/out_sum_mux0001[14] )
  );
  LUT3 #(
    .INIT ( 8'hB8 ))
  \OCXO_pll/phase_loop/integrator_mux0002<21>1  (
    .I0(genlock_register_22_5_2788),
    .I1(\system_control[2] ),
    .I2(\OCXO_pll/phase_loop/integrator_addsub0000 [21]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [21])
  );
  LUT3 #(
    .INIT ( 8'hB8 ))
  \OCXO_pll/phase_loop/integrator_mux0002<22>1  (
    .I0(genlock_register_22_6_2789),
    .I1(\system_control[2] ),
    .I2(\OCXO_pll/phase_loop/integrator_addsub0000 [22]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [22])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/out_sum_mux0001<12>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [14]),
    .I2(\OCXO_pll/phase_detect/phase_diff [10]),
    .O(\OCXO_pll/phase_loop/out_sum_mux0001[12] )
  );
  LUT3 #(
    .INIT ( 8'hB8 ))
  \OCXO_pll/phase_loop/integrator_mux0002<23>1  (
    .I0(genlock_register_22_7_2790),
    .I1(\system_control[2] ),
    .I2(\OCXO_pll/phase_loop/integrator_addsub0000 [23]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [23])
  );
  LUT2 #(
    .INIT ( 4'h4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<24>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [24]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [24])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/out_sum_mux0001<10>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [12]),
    .I2(\OCXO_pll/phase_detect/phase_diff [8]),
    .O(\OCXO_pll/phase_loop/out_sum_mux0001[10] )
  );
  LUT2 #(
    .INIT ( 4'h4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<25>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [25]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [25])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/out_sum_mux0001<9>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [11]),
    .I2(\OCXO_pll/phase_detect/phase_diff [7]),
    .O(\OCXO_pll/phase_loop/out_sum_mux0001[9] )
  );
  LUT2 #(
    .INIT ( 4'h4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<26>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [26]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [26])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/out_sum_mux0001<8>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [10]),
    .I2(\OCXO_pll/phase_detect/phase_diff [6]),
    .O(\OCXO_pll/phase_loop/out_sum_mux0001[8] )
  );
  LUT2 #(
    .INIT ( 4'h4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<27>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [27]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [27])
  );
  LUT2 #(
    .INIT ( 4'h4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<28>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [28]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [28])
  );
  LUT2 #(
    .INIT ( 4'h4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<29>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [29]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [29])
  );
  LUT2 #(
    .INIT ( 4'h4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<30>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [30]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [30])
  );
  LUT2 #(
    .INIT ( 4'h4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<31>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [31]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [31])
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/out_sum_mux0001<3>1  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [5]),
    .I2(\OCXO_pll/phase_detect/phase_diff [1]),
    .O(\OCXO_pll/phase_loop/out_sum_mux0001[3] )
  );
  LUT2 #(
    .INIT ( 4'h4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<32>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [32]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [32])
  );
  LUT2 #(
    .INIT ( 4'h4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<33>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [33]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [33])
  );
  LUT2 #(
    .INIT ( 4'h4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<34>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [34]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [34])
  );
  LUT2 #(
    .INIT ( 4'h4 ))
  \OCXO_pll/phase_loop/integrator_mux0002<35>1  (
    .I0(\system_control[2] ),
    .I1(\OCXO_pll/phase_loop/integrator_addsub0000 [35]),
    .O(\OCXO_pll/phase_loop/integrator_mux0002 [35])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \OCXO_pll/phase_loop/dac_word_o<7>_SW0  (
    .I0(\system_control[4] ),
    .I1(genlock_register_21_7_2781),
    .O(N138)
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \OCXO_pll/phase_loop/dac_word_o<5>_SW0  (
    .I0(\system_control[4] ),
    .I1(genlock_register_21_5_2779),
    .O(N142)
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \OCXO_pll/phase_loop/dac_word_o<11>_SW0  (
    .I0(\system_control[4] ),
    .I1(genlock_register_22_3_2786),
    .O(N152)
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \OCXO_pll/phase_loop/dac_word_o<0>_SW0  (
    .I0(\system_control[4] ),
    .I1(genlock_register_21_0_2774),
    .O(N154)
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \OCXO_pll/phase_loop/dac_word_o<0>  (
    .I0(\OCXO_pll/phase_loop/N01 ),
    .I1(\OCXO_pll/phase_loop/out_sum [0]),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .I3(N154),
    .O(\OCXO_cv_word[0] )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \OCXO_pll/phase_loop/dac_word_o<9>1  (
    .I0(\system_control[4] ),
    .I1(genlock_register_22_1_2784),
    .I2(\OCXO_pll/phase_loop/out_sum [9]),
    .I3(\OCXO_pll/phase_loop/N11 ),
    .O(\OCXO_cv_word[9] )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \OCXO_pll/phase_loop/dac_word_o<8>1  (
    .I0(\system_control[4] ),
    .I1(genlock_register_22_0_2783),
    .I2(\OCXO_pll/phase_loop/out_sum [8]),
    .I3(\OCXO_pll/phase_loop/N11 ),
    .O(\OCXO_cv_word[8] )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \OCXO_pll/phase_loop/dac_word_o<3>1  (
    .I0(\system_control[4] ),
    .I1(genlock_register_21_3_2777),
    .I2(\OCXO_pll/phase_loop/out_sum [3]),
    .I3(\OCXO_pll/phase_loop/N11 ),
    .O(\OCXO_cv_word[3] )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \OCXO_pll/phase_loop/dac_word_o<14>1  (
    .I0(\system_control[4] ),
    .I1(genlock_register_22_6_2789),
    .I2(\OCXO_pll/phase_loop/out_sum [14]),
    .I3(\OCXO_pll/phase_loop/N11 ),
    .O(\OCXO_cv_word[14] )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \OCXO_pll/phase_loop/dac_word_o<12>1  (
    .I0(\system_control[4] ),
    .I1(genlock_register_22_4_2787),
    .I2(\OCXO_pll/phase_loop/out_sum [12]),
    .I3(\OCXO_pll/phase_loop/N11 ),
    .O(\OCXO_cv_word[12] )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \OCXO_pll/phase_loop/dac_word_o<10>1  (
    .I0(\system_control[4] ),
    .I1(genlock_register_22_2_2785),
    .I2(\OCXO_pll/phase_loop/out_sum [10]),
    .I3(\OCXO_pll/phase_loop/N11 ),
    .O(\OCXO_cv_word[10] )
  );
  IBUFG   clk_10_i_IBUFG (
    .I(clk_10_i),
    .O(clk_10_i_IBUFG1)
  );
  IBUF   rst_i_IBUF (
    .I(rst_i),
    .O(rst_i_IBUF_3067)
  );
  IBUF   PPS_i_IBUF (
    .I(PPS_i),
    .O(PPS_i_IBUF_2001)
  );
  IBUF   UBX_TX_i_IBUF (
    .I(UBX_TX_i),
    .O(NTP_RX_OBUF_1400)
  );
  IBUF   SCL_i_IBUF (
    .I(SCL_i),
    .O(SCL_i_IBUF_2049)
  );
  OBUF   tp1_o_OBUF (
    .I(\VCXO_pll/phase_detector/phase_counter [9]),
    .O(tp1_o)
  );
  OBUF   LTC_a_out_OBUF (
    .I(\LTC_A/LTC_biphase_generator/biphase_code_148 ),
    .O(LTC_a_out)
  );
  OBUF   tp2_o_OBUF (
    .I(tp2_o_OBUF_3479),
    .O(tp2_o)
  );
  OBUF   PPS_status_o_OBUF (
    .I(pps_status_3039),
    .O(PPS_status_o)
  );
  OBUF   din_10mhz_o_OBUF (
    .I(\dac_OCXO/shift_reg [23]),
    .O(din_10mhz_o)
  );
  OBUF   led1_o_OBUF (
    .I(led1_o_OBUF_3021),
    .O(led1_o)
  );
  OBUF   din_148mhz_OBUF (
    .I(\Inst_ad5660_serial_DAC/data_o_46 ),
    .O(din_148mhz)
  );
  OBUF   tp3_o_OBUF (
    .I(\VCXO_pll/ref_clk_2291 ),
    .O(tp3_o)
  );
  OBUF   sync_148mhz_OBUF (
    .I(\Inst_ad5660_serial_DAC/sync_o_116 ),
    .O(sync_148mhz)
  );
  OBUF   NTP_PPS_OBUF (
    .I(longer_tick_3023),
    .O(NTP_PPS)
  );
  OBUF   h_d_o_OBUF (
    .I(\h_gen/pulse_state_3000 ),
    .O(h_d_o)
  );
  OBUF   LTC_b_out_OBUF (
    .I(\LTC_B/LTC_biphase_generator/biphase_code_582 ),
    .O(LTC_b_out)
  );
  OBUF   led2_o_OBUF (
    .I(pps_status_3039),
    .O(led2_o)
  );
  OBUF   ext_int_o_OBUF (
    .I(\OCXO_pll/phase_detect/phase_counter [26]),
    .O(ext_int_o)
  );
  OBUF   MCU_RX_OBUF (
    .I(NTP_RX_OBUF_1400),
    .O(MCU_RX)
  );
  OBUF   NTP_RX_OBUF (
    .I(NTP_RX_OBUF_1400),
    .O(NTP_RX)
  );
  OBUF   sclk_148mhz_OBUF (
    .I(\Inst_ad5660_serial_DAC/sclk_53 ),
    .O(sclk_148mhz)
  );
  OBUF   sync_10mhz_o_OBUF (
    .I(\dac_OCXO/zsync_2488 ),
    .O(sync_10mhz_o)
  );
  OBUF   sclk_10mhz_o_OBUF (
    .I(\dac_OCXO/bit_count [2]),
    .O(sclk_10mhz_o)
  );
  OBUF   f_d_o_OBUF (
    .I(\f_gen/pulse_state_2639 ),
    .O(f_d_o)
  );
  FDR   \f_gen/count_31  (
    .C(clk_148),
    .D(\f_gen/count_addsub0000 [31]),
    .R(\f_gen/pulse_state_cmp_eq0000 ),
    .Q(\f_gen/count [31])
  );
  FDR   \f_gen/count_30  (
    .C(clk_148),
    .D(\f_gen/count_addsub0000 [30]),
    .R(\f_gen/pulse_state_cmp_eq0000 ),
    .Q(\f_gen/count [30])
  );
  FDR   \f_gen/count_29  (
    .C(clk_148),
    .D(\f_gen/count_addsub0000 [29]),
    .R(\f_gen/pulse_state_cmp_eq0000 ),
    .Q(\f_gen/count [29])
  );
  FDR   \f_gen/count_28  (
    .C(clk_148),
    .D(\f_gen/count_addsub0000 [28]),
    .R(\f_gen/pulse_state_cmp_eq0000 ),
    .Q(\f_gen/count [28])
  );
  FDR   \f_gen/count_27  (
    .C(clk_148),
    .D(\f_gen/count_addsub0000 [27]),
    .R(\f_gen/pulse_state_cmp_eq0000 ),
    .Q(\f_gen/count [27])
  );
  FDR   \f_gen/count_26  (
    .C(clk_148),
    .D(\f_gen/count_addsub0000 [26]),
    .R(\f_gen/pulse_state_cmp_eq0000 ),
    .Q(\f_gen/count [26])
  );
  FDR   \f_gen/count_25  (
    .C(clk_148),
    .D(\f_gen/count_addsub0000 [25]),
    .R(\f_gen/pulse_state_cmp_eq0000 ),
    .Q(\f_gen/count [25])
  );
  FDR   \f_gen/count_15  (
    .C(clk_148),
    .D(\f_gen/count_addsub0000 [15]),
    .R(\f_gen/pulse_state_cmp_eq0000 ),
    .Q(\f_gen/count [15])
  );
  FDS   \f_gen/count_10  (
    .C(clk_148),
    .D(\f_gen/pulse_state_cmp_eq0000 ),
    .S(\f_gen/count_addsub0000 [10]),
    .Q(\f_gen/count [10])
  );
  FDS   \f_gen/count_9  (
    .C(clk_148),
    .D(\f_gen/pulse_state_cmp_eq0000 ),
    .S(\f_gen/count_addsub0000 [9]),
    .Q(\f_gen/count [9])
  );
  FDR   \f_gen/count_7  (
    .C(clk_148),
    .D(\f_gen/count_addsub0000 [7]),
    .R(\f_gen/pulse_state_cmp_eq0000 ),
    .Q(\f_gen/count [7])
  );
  FDS   \f_gen/count_1  (
    .C(clk_148),
    .D(\f_gen/pulse_state_cmp_eq0000 ),
    .S(\f_gen/count_addsub0000 [1]),
    .Q(\f_gen/count [1])
  );
  FDR   \f_gen/count_0  (
    .C(clk_148),
    .D(\f_gen/count_addsub0000 [0]),
    .R(\f_gen/pulse_state_cmp_eq0000 ),
    .Q(\f_gen/count [0])
  );
  FDR   \h_gen/count_31  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [31]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [31])
  );
  FDR   \h_gen/count_30  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [30]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [30])
  );
  FDR   \h_gen/count_29  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [29]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [29])
  );
  FDR   \h_gen/count_28  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [28]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [28])
  );
  FDR   \h_gen/count_27  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [27]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [27])
  );
  FDR   \h_gen/count_26  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [26]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [26])
  );
  FDR   \h_gen/count_25  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [25]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [25])
  );
  FDR   \h_gen/count_24  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [24]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [24])
  );
  FDR   \h_gen/count_23  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [23]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [23])
  );
  FDR   \h_gen/count_22  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [22]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [22])
  );
  FDR   \h_gen/count_21  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [21]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [21])
  );
  FDR   \h_gen/count_20  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [20]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [20])
  );
  FDR   \h_gen/count_19  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [19]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [19])
  );
  FDR   \h_gen/count_18  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [18]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [18])
  );
  FDR   \h_gen/count_17  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [17]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [17])
  );
  FDR   \h_gen/count_16  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [16]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [16])
  );
  FDR   \h_gen/count_15  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [15]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [15])
  );
  FDR   \h_gen/count_14  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [14]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [14])
  );
  FDR   \h_gen/count_13  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [13]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [13])
  );
  FDS   \h_gen/count_10  (
    .C(clk_148),
    .D(\h_gen/pulse_state_cmp_eq0000 ),
    .S(\h_gen/count_addsub0000 [10]),
    .Q(\h_gen/count [10])
  );
  FDS   \h_gen/count_9  (
    .C(clk_148),
    .D(\h_gen/pulse_state_cmp_eq0000 ),
    .S(\h_gen/count_addsub0000 [9]),
    .Q(\h_gen/count [9])
  );
  FDR   \h_gen/count_8  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [8]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [8])
  );
  FDS   \h_gen/count_5  (
    .C(clk_148),
    .D(\h_gen/pulse_state_cmp_eq0000 ),
    .S(\h_gen/count_addsub0000 [5]),
    .Q(\h_gen/count [5])
  );
  FDR   \h_gen/count_0  (
    .C(clk_148),
    .D(\h_gen/count_addsub0000 [0]),
    .R(\h_gen/pulse_state_cmp_eq0000 ),
    .Q(\h_gen/count [0])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \serial_interface/state_FSM_FFd4  (
    .C(clk_148),
    .D(\serial_interface/state_FSM_FFd4-In216 ),
    .S(\serial_interface/state_FSM_FFd4-In5_3389 ),
    .Q(\serial_interface/state_FSM_FFd4_3382 )
  );
  FDS   \serial_interface/send_byte_7  (
    .C(clk_148),
    .D(\serial_interface/send_byte_mux0001<7>216 ),
    .S(\serial_interface/send_byte_mux0001<7>0_3327 ),
    .Q(\serial_interface/send_byte [7])
  );
  FDS   \serial_interface/send_byte_6  (
    .C(clk_148),
    .D(\serial_interface/send_byte_mux0001<6>203 ),
    .S(\serial_interface/send_byte_mux0001<6>0_3309 ),
    .Q(\serial_interface/send_byte [6])
  );
  FDS   \serial_interface/send_byte_5  (
    .C(clk_148),
    .D(\serial_interface/send_byte_mux0001<5>216 ),
    .S(\serial_interface/send_byte_mux0001<5>0_3285 ),
    .Q(\serial_interface/send_byte [5])
  );
  FDS   \serial_interface/send_byte_4  (
    .C(clk_148),
    .D(\serial_interface/send_byte_mux0001<4>215 ),
    .S(\serial_interface/send_byte_mux0001<4>0_3265 ),
    .Q(\serial_interface/send_byte [4])
  );
  FDS   \serial_interface/send_byte_3  (
    .C(clk_148),
    .D(\serial_interface/send_byte_mux0001<3>216 ),
    .S(\serial_interface/send_byte_mux0001<3>0_3241 ),
    .Q(\serial_interface/send_byte [3])
  );
  FDS   \serial_interface/send_byte_2  (
    .C(clk_148),
    .D(\serial_interface/send_byte_mux0001<2>203 ),
    .S(\serial_interface/send_byte_mux0001<2>0_3223 ),
    .Q(\serial_interface/send_byte [2])
  );
  FDS   \serial_interface/send_byte_1  (
    .C(clk_148),
    .D(\serial_interface/send_byte_mux0001<1>216 ),
    .S(\serial_interface/send_byte_mux0001<1>0_3199 ),
    .Q(\serial_interface/send_byte [1])
  );
  FDS   \serial_interface/send_byte_0  (
    .C(clk_148),
    .D(\serial_interface/send_byte_mux0001<0>207 ),
    .S(\serial_interface/send_byte_mux0001<0>0_3181 ),
    .Q(\serial_interface/send_byte [0])
  );
  FDS   \serial_interface/time_out_21  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009<0>1 ),
    .S(\serial_interface/time_out_addsub0000 [21]),
    .Q(\serial_interface/time_out [21])
  );
  FDS   \serial_interface/time_out_20  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009<0>1 ),
    .S(\serial_interface/time_out_addsub0000 [20]),
    .Q(\serial_interface/time_out [20])
  );
  FDS   \serial_interface/time_out_19  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009<0>1 ),
    .S(\serial_interface/time_out_addsub0000 [19]),
    .Q(\serial_interface/time_out [19])
  );
  FDS   \serial_interface/time_out_15  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009<0>1 ),
    .S(\serial_interface/time_out_addsub0000 [15]),
    .Q(\serial_interface/time_out [15])
  );
  FDS   \serial_interface/time_out_13  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009<0>1 ),
    .S(\serial_interface/time_out_addsub0000 [13]),
    .Q(\serial_interface/time_out [13])
  );
  FDS   \serial_interface/time_out_10  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009<0>1 ),
    .S(\serial_interface/time_out_addsub0000 [10]),
    .Q(\serial_interface/time_out [10])
  );
  FDS   \serial_interface/time_out_8  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009<0>1 ),
    .S(\serial_interface/time_out_addsub0000 [8]),
    .Q(\serial_interface/time_out [8])
  );
  FDS   \serial_interface/time_out_7  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009<0>1 ),
    .S(\serial_interface/time_out_addsub0000 [7]),
    .Q(\serial_interface/time_out [7])
  );
  FDS   \serial_interface/time_out_6  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009<0>1 ),
    .S(\serial_interface/time_out_addsub0000 [6]),
    .Q(\serial_interface/time_out [6])
  );
  FDS   \serial_interface/time_out_5  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009<0>1 ),
    .S(\serial_interface/time_out_addsub0000 [5]),
    .Q(\serial_interface/time_out [5])
  );
  FDS   \serial_interface/time_out_4  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009<0>1 ),
    .S(\serial_interface/time_out_addsub0000 [4]),
    .Q(\serial_interface/time_out [4])
  );
  FDS   \serial_interface/time_out_2  (
    .C(clk_148),
    .D(\serial_interface/time_out_mux0009<0>1 ),
    .S(\serial_interface/time_out_addsub0000 [2]),
    .Q(\serial_interface/time_out [2])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \Inst_ad5660_serial_DAC/state_FSM_FFd3  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/state_FSM_FFd3-In17 ),
    .S(\Inst_ad5660_serial_DAC/state_FSM_FFd3-In2_110 ),
    .Q(\Inst_ad5660_serial_DAC/state_FSM_FFd3_107 )
  );
  FDS   \Inst_ad5660_serial_DAC/shift_register_15  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000<15>1_85 ),
    .S(N221),
    .Q(\Inst_ad5660_serial_DAC/shift_register [15])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<15>1  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I1(\VCXO_pll/cv_word_o [15]),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000<15>1_85 )
  );
  FDS   \Inst_ad5660_serial_DAC/shift_register_14  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000<14>1_84 ),
    .S(N24),
    .Q(\Inst_ad5660_serial_DAC/shift_register [14])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<14>1  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I1(\VCXO_pll/cv_word_o [14]),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000<14>1_84 )
  );
  FDS   \Inst_ad5660_serial_DAC/shift_register_13  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000<13>1_83 ),
    .S(N261),
    .Q(\Inst_ad5660_serial_DAC/shift_register [13])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<13>1  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I1(\VCXO_pll/cv_word_o [13]),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000<13>1_83 )
  );
  FDS   \Inst_ad5660_serial_DAC/shift_register_12  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000<12>1_82 ),
    .S(N28),
    .Q(\Inst_ad5660_serial_DAC/shift_register [12])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<12>1  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I1(\VCXO_pll/cv_word_o [12]),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000<12>1_82 )
  );
  FDS   \Inst_ad5660_serial_DAC/shift_register_11  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000<11>1_81 ),
    .S(N30),
    .Q(\Inst_ad5660_serial_DAC/shift_register [11])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<11>1  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I1(\VCXO_pll/cv_word_o [11]),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000<11>1_81 )
  );
  FDS   \Inst_ad5660_serial_DAC/shift_register_10  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000<10>1_80 ),
    .S(N321),
    .Q(\Inst_ad5660_serial_DAC/shift_register [10])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<10>1  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I1(\VCXO_pll/cv_word_o [10]),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000<10>1_80 )
  );
  FDS   \Inst_ad5660_serial_DAC/shift_register_9  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000<9>1_102 ),
    .S(N4),
    .Q(\Inst_ad5660_serial_DAC/shift_register [9])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<9>1  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I1(\VCXO_pll/cv_word_o [9]),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000<9>1_102 )
  );
  FDS   \Inst_ad5660_serial_DAC/shift_register_8  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000<8>1_101 ),
    .S(N6),
    .Q(\Inst_ad5660_serial_DAC/shift_register [8])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<8>1  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I1(\VCXO_pll/cv_word_o [8]),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000<8>1_101 )
  );
  FDS   \Inst_ad5660_serial_DAC/shift_register_7  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000<7>1_100 ),
    .S(N8),
    .Q(\Inst_ad5660_serial_DAC/shift_register [7])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<7>1  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I1(\VCXO_pll/cv_word_o [7]),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000<7>1_100 )
  );
  FDS   \Inst_ad5660_serial_DAC/shift_register_6  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000<6>1_99 ),
    .S(N10),
    .Q(\Inst_ad5660_serial_DAC/shift_register [6])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<6>1  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I1(\VCXO_pll/cv_word_o [6]),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000<6>1_99 )
  );
  FDS   \Inst_ad5660_serial_DAC/shift_register_5  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000<5>1_98 ),
    .S(N12),
    .Q(\Inst_ad5660_serial_DAC/shift_register [5])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<5>1  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I1(\VCXO_pll/cv_word_o [5]),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000<5>1_98 )
  );
  FDS   \Inst_ad5660_serial_DAC/shift_register_4  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000<4>1_97 ),
    .S(N141),
    .Q(\Inst_ad5660_serial_DAC/shift_register [4])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<4>1  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I1(\VCXO_pll/cv_word_o [4]),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000<4>1_97 )
  );
  FDS   \Inst_ad5660_serial_DAC/shift_register_3  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000<3>1_96 ),
    .S(N161),
    .Q(\Inst_ad5660_serial_DAC/shift_register [3])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<3>1  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I1(\VCXO_pll/cv_word_o [3]),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000<3>1_96 )
  );
  FDS   \Inst_ad5660_serial_DAC/shift_register_2  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000<2>1_95 ),
    .S(N181),
    .Q(\Inst_ad5660_serial_DAC/shift_register [2])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<2>1  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I1(\VCXO_pll/cv_word_o [2]),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000<2>1_95 )
  );
  FDS   \Inst_ad5660_serial_DAC/shift_register_1  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/shift_register_mux0000<1>1_90 ),
    .S(N201),
    .Q(\Inst_ad5660_serial_DAC/shift_register [1])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \Inst_ad5660_serial_DAC/shift_register_mux0000<1>1  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .I1(\VCXO_pll/cv_word_o [1]),
    .O(\Inst_ad5660_serial_DAC/shift_register_mux0000<1>1_90 )
  );
  FDS   \Inst_ad5660_serial_DAC/data_o  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/data_o_mux00011 ),
    .S(\Inst_ad5660_serial_DAC/shift_register [23]),
    .Q(\Inst_ad5660_serial_DAC/data_o_46 )
  );
  FDS #(
    .INIT ( 1'b0 ))
  \LTC_B/LTC_clockgenerator/period_count_3  (
    .C(clk_148),
    .D(\LTC_B/LTC_clockgenerator/period_count_mux0003<12>1 ),
    .S(\LTC_B/LTC_clockgenerator/N5 ),
    .Q(\LTC_B/LTC_clockgenerator/period_count [3])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \LTC_B/LTC_clockgenerator/period_count_4  (
    .C(clk_148),
    .D(\LTC_B/LTC_clockgenerator/period_count_mux0003<11>1 ),
    .S(\LTC_B/LTC_clockgenerator/N5 ),
    .Q(\LTC_B/LTC_clockgenerator/period_count [4])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \LTC_B/LTC_clockgenerator/period_count_6  (
    .C(clk_148),
    .D(\LTC_B/LTC_clockgenerator/period_count_mux0003<9>1 ),
    .S(\LTC_B/LTC_clockgenerator/N5 ),
    .Q(\LTC_B/LTC_clockgenerator/period_count [6])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \LTC_B/LTC_clockgenerator/period_count_7  (
    .C(clk_148),
    .D(\LTC_B/LTC_clockgenerator/period_count_mux0003<8>1 ),
    .S(\LTC_B/LTC_clockgenerator/N5 ),
    .Q(\LTC_B/LTC_clockgenerator/period_count [7])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \LTC_B/LTC_clockgenerator/period_count_11  (
    .C(clk_148),
    .D(\LTC_B/LTC_clockgenerator/period_count_mux0003<4>1 ),
    .S(\LTC_B/LTC_clockgenerator/N5 ),
    .Q(\LTC_B/LTC_clockgenerator/period_count [11])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \LTC_B/LTC_clockgenerator/period_count_12  (
    .C(clk_148),
    .D(\LTC_B/LTC_clockgenerator/ltc_clock_tick_o_or0000 ),
    .S(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [12]),
    .Q(\LTC_B/LTC_clockgenerator/period_count [12])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \LTC_B/LTC_clockgenerator/period_count_13  (
    .C(clk_148),
    .D(\LTC_B/LTC_clockgenerator/period_count_mux0003<2>1 ),
    .S(\LTC_B/LTC_clockgenerator/N5 ),
    .Q(\LTC_B/LTC_clockgenerator/period_count [13])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \LTC_B/LTC_clockgenerator/period_count_14  (
    .C(clk_148),
    .D(\LTC_B/LTC_clockgenerator/period_count_mux0003<1>1 ),
    .S(\LTC_B/LTC_clockgenerator/N5 ),
    .Q(\LTC_B/LTC_clockgenerator/period_count [14])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \LTC_A/LTC_clockgenerator/period_count_3  (
    .C(clk_148),
    .D(\LTC_A/LTC_clockgenerator/period_count_mux0003<12>1 ),
    .S(\LTC_A/LTC_clockgenerator/N5 ),
    .Q(\LTC_A/LTC_clockgenerator/period_count [3])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \LTC_A/LTC_clockgenerator/period_count_4  (
    .C(clk_148),
    .D(\LTC_A/LTC_clockgenerator/period_count_mux0003<11>1 ),
    .S(\LTC_A/LTC_clockgenerator/N5 ),
    .Q(\LTC_A/LTC_clockgenerator/period_count [4])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \LTC_A/LTC_clockgenerator/period_count_6  (
    .C(clk_148),
    .D(\LTC_A/LTC_clockgenerator/period_count_mux0003<9>1 ),
    .S(\LTC_A/LTC_clockgenerator/N5 ),
    .Q(\LTC_A/LTC_clockgenerator/period_count [6])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \LTC_A/LTC_clockgenerator/period_count_7  (
    .C(clk_148),
    .D(\LTC_A/LTC_clockgenerator/period_count_mux0003<8>1 ),
    .S(\LTC_A/LTC_clockgenerator/N5 ),
    .Q(\LTC_A/LTC_clockgenerator/period_count [7])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \LTC_A/LTC_clockgenerator/period_count_11  (
    .C(clk_148),
    .D(\LTC_A/LTC_clockgenerator/period_count_mux0003<4>1 ),
    .S(\LTC_A/LTC_clockgenerator/N5 ),
    .Q(\LTC_A/LTC_clockgenerator/period_count [11])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \LTC_A/LTC_clockgenerator/period_count_12  (
    .C(clk_148),
    .D(\LTC_A/LTC_clockgenerator/ltc_clock_tick_o_or0000 ),
    .S(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [12]),
    .Q(\LTC_A/LTC_clockgenerator/period_count [12])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \LTC_A/LTC_clockgenerator/period_count_13  (
    .C(clk_148),
    .D(\LTC_A/LTC_clockgenerator/period_count_mux0003<2>1 ),
    .S(\LTC_A/LTC_clockgenerator/N5 ),
    .Q(\LTC_A/LTC_clockgenerator/period_count [13])
  );
  FDS #(
    .INIT ( 1'b0 ))
  \LTC_A/LTC_clockgenerator/period_count_14  (
    .C(clk_148),
    .D(\LTC_A/LTC_clockgenerator/period_count_mux0003<1>1 ),
    .S(\LTC_A/LTC_clockgenerator/N5 ),
    .Q(\LTC_A/LTC_clockgenerator/period_count [14])
  );
  FDRS #(
    .INIT ( 1'b0 ))
  \communication/state_FSM_FFd1  (
    .C(clk_148),
    .D(\communication/state_FSM_FFd1-In1 ),
    .R(\communication/state_and0000 ),
    .S(\communication/state_FSM_FFd2_2370 ),
    .Q(\communication/state_FSM_FFd1_2366 )
  );
  FDRS #(
    .INIT ( 1'b0 ))
  \Inst_ad5660_serial_DAC/state_FSM_FFd2  (
    .C(clk_148),
    .D(\Inst_ad5660_serial_DAC/state_FSM_FFd2-In11_106 ),
    .R(N36),
    .S(\Inst_ad5660_serial_DAC/N3 ),
    .Q(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 )
  );
  LUT2 #(
    .INIT ( 4'hD ))
  \Inst_ad5660_serial_DAC/state_FSM_FFd2-In11  (
    .I0(\Inst_ad5660_serial_DAC/bit_count [4]),
    .I1(\Inst_ad5660_serial_DAC/bit_count [3]),
    .O(\Inst_ad5660_serial_DAC/state_FSM_FFd2-In11_106 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcompar_pps_status_cmp_gt0000_cy<2>_rt  (
    .I0(pps_status_count[5]),
    .O(\Mcompar_pps_status_cmp_gt0000_cy<2>_rt_1116 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcompar_pps_status_cmp_gt0000_cy<9>_rt  (
    .I0(pps_status_count[21]),
    .O(\Mcompar_pps_status_cmp_gt0000_cy<9>_rt_1124 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcompar_pps_status_cmp_gt0000_cy<11>_rt  (
    .I0(pps_status_count[23]),
    .O(\Mcompar_pps_status_cmp_gt0000_cy<11>_rt_1113 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \f_gen/Msub_count_addsub0000_cy<0>_rt  (
    .I0(\f_gen/count [0]),
    .O(\f_gen/Msub_count_addsub0000_cy<0>_rt_2494 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \h_gen/Msub_count_addsub0000_cy<0>_rt  (
    .I0(\h_gen/count [0]),
    .O(\h_gen/Msub_count_addsub0000_cy<0>_rt_2866 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<1>_rt  (
    .I0(pps_status_count[1]),
    .O(\Mcount_pps_status_count_cy<1>_rt_1197 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<2>_rt  (
    .I0(pps_status_count[2]),
    .O(\Mcount_pps_status_count_cy<2>_rt_1205 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<3>_rt  (
    .I0(pps_status_count[3]),
    .O(\Mcount_pps_status_count_cy<3>_rt_1207 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<4>_rt  (
    .I0(pps_status_count[4]),
    .O(\Mcount_pps_status_count_cy<4>_rt_1209 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<5>_rt  (
    .I0(pps_status_count[5]),
    .O(\Mcount_pps_status_count_cy<5>_rt_1211 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<6>_rt  (
    .I0(pps_status_count[6]),
    .O(\Mcount_pps_status_count_cy<6>_rt_1213 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<7>_rt  (
    .I0(pps_status_count[7]),
    .O(\Mcount_pps_status_count_cy<7>_rt_1215 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<8>_rt  (
    .I0(pps_status_count[8]),
    .O(\Mcount_pps_status_count_cy<8>_rt_1217 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<9>_rt  (
    .I0(pps_status_count[9]),
    .O(\Mcount_pps_status_count_cy<9>_rt_1219 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<10>_rt  (
    .I0(pps_status_count[10]),
    .O(\Mcount_pps_status_count_cy<10>_rt_1177 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<11>_rt  (
    .I0(pps_status_count[11]),
    .O(\Mcount_pps_status_count_cy<11>_rt_1179 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<12>_rt  (
    .I0(pps_status_count[12]),
    .O(\Mcount_pps_status_count_cy<12>_rt_1181 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<13>_rt  (
    .I0(pps_status_count[13]),
    .O(\Mcount_pps_status_count_cy<13>_rt_1183 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<14>_rt  (
    .I0(pps_status_count[14]),
    .O(\Mcount_pps_status_count_cy<14>_rt_1185 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<15>_rt  (
    .I0(pps_status_count[15]),
    .O(\Mcount_pps_status_count_cy<15>_rt_1187 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<16>_rt  (
    .I0(pps_status_count[16]),
    .O(\Mcount_pps_status_count_cy<16>_rt_1189 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<17>_rt  (
    .I0(pps_status_count[17]),
    .O(\Mcount_pps_status_count_cy<17>_rt_1191 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<18>_rt  (
    .I0(pps_status_count[18]),
    .O(\Mcount_pps_status_count_cy<18>_rt_1193 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<19>_rt  (
    .I0(pps_status_count[19]),
    .O(\Mcount_pps_status_count_cy<19>_rt_1195 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<20>_rt  (
    .I0(pps_status_count[20]),
    .O(\Mcount_pps_status_count_cy<20>_rt_1199 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<21>_rt  (
    .I0(pps_status_count[21]),
    .O(\Mcount_pps_status_count_cy<21>_rt_1201 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_cy<22>_rt  (
    .I0(pps_status_count[22]),
    .O(\Mcount_pps_status_count_cy<22>_rt_1203 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_count_cy<0>_rt  (
    .I0(count[0]),
    .O(\Mcount_count_cy<0>_rt_1135 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \serial_interface/Msub_time_out_addsub0000_cy<0>_rt  (
    .I0(\serial_interface/time_out [0]),
    .O(\serial_interface/Msub_time_out_addsub0000_cy<0>_rt_3071 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \dac_OCXO/Mcount_bit_count_cy<0>_rt  (
    .I0(\dac_OCXO/bit_count [0]),
    .O(\dac_OCXO/Mcount_bit_count_cy<0>_rt_2402 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \OCXO_pll/phase_detect/Mcount_phase_counter_cy<0>_rt  (
    .I0(\OCXO_pll/phase_detect/phase_counter [0]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_cy<0>_rt_1414 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<9>_rt  (
    .I0(\OCXO_pll/phase_loop/out_sum [15]),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<9>_rt_1747 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<7>_rt  (
    .I0(\OCXO_pll/phase_loop/out_sum [13]),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<7>_rt_1744 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<5>_rt  (
    .I0(\OCXO_pll/phase_loop/out_sum [11]),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<5>_rt_1741 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<1>_rt  (
    .I0(\OCXO_pll/phase_loop/out_sum [2]),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy<1>_rt_1736 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<12>_rt  (
    .I0(\OCXO_pll/phase_loop/integrator [23]),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<12>_rt_1701 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<10>_rt  (
    .I0(\OCXO_pll/phase_loop/integrator [21]),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<10>_rt_1698 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<8>_rt  (
    .I0(\OCXO_pll/phase_loop/integrator [19]),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<8>_rt_1716 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<4>_rt  (
    .I0(\OCXO_pll/phase_loop/integrator [10]),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<4>_rt_1711 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<2>_rt  (
    .I0(\OCXO_pll/phase_loop/integrator [8]),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy<2>_rt_1708 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \VCXO_pll/phase_detector/Mcount_phase_counter_cy<0>_rt  (
    .I0(\VCXO_pll/phase_detector/phase_counter [0]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_cy<0>_rt_2237 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \VCXO_pll/Mcount_ref_clk_count_cy<7>_rt  (
    .I0(\VCXO_pll/ref_clk_count [7]),
    .O(\VCXO_pll/Mcount_ref_clk_count_cy<7>_rt_2066 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \VCXO_pll/Mcount_ref_clk_count_cy<6>_rt  (
    .I0(\VCXO_pll/ref_clk_count [6]),
    .O(\VCXO_pll/Mcount_ref_clk_count_cy<6>_rt_2064 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \VCXO_pll/Mcount_ref_clk_count_cy<5>_rt  (
    .I0(\VCXO_pll/ref_clk_count [5]),
    .O(\VCXO_pll/Mcount_ref_clk_count_cy<5>_rt_2062 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \VCXO_pll/Mcount_ref_clk_count_cy<4>_rt  (
    .I0(\VCXO_pll/ref_clk_count [4]),
    .O(\VCXO_pll/Mcount_ref_clk_count_cy<4>_rt_2060 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \VCXO_pll/Mcount_ref_clk_count_cy<3>_rt  (
    .I0(\VCXO_pll/ref_clk_count [3]),
    .O(\VCXO_pll/Mcount_ref_clk_count_cy<3>_rt_2058 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \VCXO_pll/Mcount_ref_clk_count_cy<2>_rt  (
    .I0(\VCXO_pll/ref_clk_count [2]),
    .O(\VCXO_pll/Mcount_ref_clk_count_cy<2>_rt_2056 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \VCXO_pll/Mcount_ref_clk_count_cy<1>_rt  (
    .I0(\VCXO_pll/ref_clk_count [1]),
    .O(\VCXO_pll/Mcount_ref_clk_count_cy<1>_rt_2054 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<0>_rt  (
    .I0(\LTC_B/LTC_clockgenerator/period_count [0]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_cy<0>_rt_606 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<1>_rt  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [1]),
    .O(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<1>_rt_592 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<2>_rt  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [2]),
    .O(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<2>_rt_594 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<3>_rt  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [3]),
    .O(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<3>_rt_596 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<4>_rt  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [4]),
    .O(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<4>_rt_598 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<5>_rt  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .O(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<5>_rt_600 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<6>_rt  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [6]),
    .O(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_cy<6>_rt_602 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<22>_rt  (
    .I0(\LTC_B/delay_count_low [22]),
    .O(\LTC_B/Mcount_delay_count_low_cy<22>_rt_895 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<21>_rt  (
    .I0(\LTC_B/delay_count_low [21]),
    .O(\LTC_B/Mcount_delay_count_low_cy<21>_rt_893 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<20>_rt  (
    .I0(\LTC_B/delay_count_low [20]),
    .O(\LTC_B/Mcount_delay_count_low_cy<20>_rt_891 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<19>_rt  (
    .I0(\LTC_B/delay_count_low [19]),
    .O(\LTC_B/Mcount_delay_count_low_cy<19>_rt_887 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<18>_rt  (
    .I0(\LTC_B/delay_count_low [18]),
    .O(\LTC_B/Mcount_delay_count_low_cy<18>_rt_885 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<17>_rt  (
    .I0(\LTC_B/delay_count_low [17]),
    .O(\LTC_B/Mcount_delay_count_low_cy<17>_rt_883 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<16>_rt  (
    .I0(\LTC_B/delay_count_low [16]),
    .O(\LTC_B/Mcount_delay_count_low_cy<16>_rt_881 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<15>_rt  (
    .I0(\LTC_B/delay_count_low [15]),
    .O(\LTC_B/Mcount_delay_count_low_cy<15>_rt_879 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<14>_rt  (
    .I0(\LTC_B/delay_count_low [14]),
    .O(\LTC_B/Mcount_delay_count_low_cy<14>_rt_877 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<13>_rt  (
    .I0(\LTC_B/delay_count_low [13]),
    .O(\LTC_B/Mcount_delay_count_low_cy<13>_rt_875 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<12>_rt  (
    .I0(\LTC_B/delay_count_low [12]),
    .O(\LTC_B/Mcount_delay_count_low_cy<12>_rt_873 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<11>_rt  (
    .I0(\LTC_B/delay_count_low [11]),
    .O(\LTC_B/Mcount_delay_count_low_cy<11>_rt_871 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<10>_rt  (
    .I0(\LTC_B/delay_count_low [10]),
    .O(\LTC_B/Mcount_delay_count_low_cy<10>_rt_869 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<9>_rt  (
    .I0(\LTC_B/delay_count_low [9]),
    .O(\LTC_B/Mcount_delay_count_low_cy<9>_rt_911 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<8>_rt  (
    .I0(\LTC_B/delay_count_low [8]),
    .O(\LTC_B/Mcount_delay_count_low_cy<8>_rt_909 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<7>_rt  (
    .I0(\LTC_B/delay_count_low [7]),
    .O(\LTC_B/Mcount_delay_count_low_cy<7>_rt_907 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<6>_rt  (
    .I0(\LTC_B/delay_count_low [6]),
    .O(\LTC_B/Mcount_delay_count_low_cy<6>_rt_905 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<5>_rt  (
    .I0(\LTC_B/delay_count_low [5]),
    .O(\LTC_B/Mcount_delay_count_low_cy<5>_rt_903 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<4>_rt  (
    .I0(\LTC_B/delay_count_low [4]),
    .O(\LTC_B/Mcount_delay_count_low_cy<4>_rt_901 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<3>_rt  (
    .I0(\LTC_B/delay_count_low [3]),
    .O(\LTC_B/Mcount_delay_count_low_cy<3>_rt_899 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<2>_rt  (
    .I0(\LTC_B/delay_count_low [2]),
    .O(\LTC_B/Mcount_delay_count_low_cy<2>_rt_897 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_cy<1>_rt  (
    .I0(\LTC_B/delay_count_low [1]),
    .O(\LTC_B/Mcount_delay_count_low_cy<1>_rt_889 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<0>_rt  (
    .I0(\LTC_A/LTC_clockgenerator/period_count [0]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_cy<0>_rt_172 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<1>_rt  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [1]),
    .O(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<1>_rt_158 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<2>_rt  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [2]),
    .O(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<2>_rt_160 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<3>_rt  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [3]),
    .O(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<3>_rt_162 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<4>_rt  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [4]),
    .O(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<4>_rt_164 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<5>_rt  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .O(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<5>_rt_166 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<6>_rt  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [6]),
    .O(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_cy<6>_rt_168 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<22>_rt  (
    .I0(\LTC_A/delay_count_low [22]),
    .O(\LTC_A/Mcount_delay_count_low_cy<22>_rt_461 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<21>_rt  (
    .I0(\LTC_A/delay_count_low [21]),
    .O(\LTC_A/Mcount_delay_count_low_cy<21>_rt_459 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<20>_rt  (
    .I0(\LTC_A/delay_count_low [20]),
    .O(\LTC_A/Mcount_delay_count_low_cy<20>_rt_457 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<19>_rt  (
    .I0(\LTC_A/delay_count_low [19]),
    .O(\LTC_A/Mcount_delay_count_low_cy<19>_rt_453 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<18>_rt  (
    .I0(\LTC_A/delay_count_low [18]),
    .O(\LTC_A/Mcount_delay_count_low_cy<18>_rt_451 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<17>_rt  (
    .I0(\LTC_A/delay_count_low [17]),
    .O(\LTC_A/Mcount_delay_count_low_cy<17>_rt_449 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<16>_rt  (
    .I0(\LTC_A/delay_count_low [16]),
    .O(\LTC_A/Mcount_delay_count_low_cy<16>_rt_447 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<15>_rt  (
    .I0(\LTC_A/delay_count_low [15]),
    .O(\LTC_A/Mcount_delay_count_low_cy<15>_rt_445 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<14>_rt  (
    .I0(\LTC_A/delay_count_low [14]),
    .O(\LTC_A/Mcount_delay_count_low_cy<14>_rt_443 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<13>_rt  (
    .I0(\LTC_A/delay_count_low [13]),
    .O(\LTC_A/Mcount_delay_count_low_cy<13>_rt_441 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<12>_rt  (
    .I0(\LTC_A/delay_count_low [12]),
    .O(\LTC_A/Mcount_delay_count_low_cy<12>_rt_439 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<11>_rt  (
    .I0(\LTC_A/delay_count_low [11]),
    .O(\LTC_A/Mcount_delay_count_low_cy<11>_rt_437 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<10>_rt  (
    .I0(\LTC_A/delay_count_low [10]),
    .O(\LTC_A/Mcount_delay_count_low_cy<10>_rt_435 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<9>_rt  (
    .I0(\LTC_A/delay_count_low [9]),
    .O(\LTC_A/Mcount_delay_count_low_cy<9>_rt_477 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<8>_rt  (
    .I0(\LTC_A/delay_count_low [8]),
    .O(\LTC_A/Mcount_delay_count_low_cy<8>_rt_475 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<7>_rt  (
    .I0(\LTC_A/delay_count_low [7]),
    .O(\LTC_A/Mcount_delay_count_low_cy<7>_rt_473 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<6>_rt  (
    .I0(\LTC_A/delay_count_low [6]),
    .O(\LTC_A/Mcount_delay_count_low_cy<6>_rt_471 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<5>_rt  (
    .I0(\LTC_A/delay_count_low [5]),
    .O(\LTC_A/Mcount_delay_count_low_cy<5>_rt_469 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<4>_rt  (
    .I0(\LTC_A/delay_count_low [4]),
    .O(\LTC_A/Mcount_delay_count_low_cy<4>_rt_467 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<3>_rt  (
    .I0(\LTC_A/delay_count_low [3]),
    .O(\LTC_A/Mcount_delay_count_low_cy<3>_rt_465 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<2>_rt  (
    .I0(\LTC_A/delay_count_low [2]),
    .O(\LTC_A/Mcount_delay_count_low_cy<2>_rt_463 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_cy<1>_rt  (
    .I0(\LTC_A/delay_count_low [1]),
    .O(\LTC_A/Mcount_delay_count_low_cy<1>_rt_455 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \Mcount_pps_status_count_xor<23>_rt  (
    .I0(pps_status_count[23]),
    .O(\Mcount_pps_status_count_xor<23>_rt_1221 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<35>_rt  (
    .I0(\OCXO_pll/phase_detect/phase_diff [27]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_xor<35>_rt_1641 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \OCXO_pll/phase_loop/integrator_mux0003<33>11  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [27]),
    .I2(\OCXO_pll/phase_detect/phase_diff [25]),
    .O(\OCXO_pll/phase_loop/integrator_mux0003<33>1_1952 )
  );
  LUT2 #(
    .INIT ( 4'h6 ))
  \VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut<17>1  (
    .I0(\VCXO_pll/VCXO_phase_loop/integrator [25]),
    .I1(\VCXO_pll/phase_detector/phase_diff [9]),
    .O(\VCXO_pll/VCXO_phase_loop/Madd_out_sum_mux0001_lut [17])
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \VCXO_pll/Mcount_ref_clk_count_xor<8>_rt  (
    .I0(\VCXO_pll/ref_clk_count [8]),
    .O(\VCXO_pll/Mcount_ref_clk_count_xor<8>_rt_2068 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_xor<7>_rt  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [7]),
    .O(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_xor<7>_rt_604 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_B/Mcount_delay_count_low_xor<23>_rt  (
    .I0(\LTC_B/delay_count_low [23]),
    .O(\LTC_B/Mcount_delay_count_low_xor<23>_rt_913 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_xor<7>_rt  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [7]),
    .O(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_xor<7>_rt_170 )
  );
  LUT1 #(
    .INIT ( 2'h2 ))
  \LTC_A/Mcount_delay_count_low_xor<23>_rt  (
    .I0(\LTC_A/delay_count_low [23]),
    .O(\LTC_A/Mcount_delay_count_low_xor<23>_rt_479 )
  );
  LUT4 #(
    .INIT ( 16'h9A30 ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<1>  (
    .I0(\OCXO_pll/phase_loop/integrator [9]),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [3]),
    .I3(\OCXO_pll/phase_loop/integrator_limit<0>_mand_1846 ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [1])
  );
  LUT4 #(
    .INIT ( 16'h27D8 ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<7>  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [5]),
    .I2(\OCXO_pll/phase_detect/phase_diff [9]),
    .I3(\OCXO_pll/phase_loop/integrator_limit[15] ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [7])
  );
  LUT4 #(
    .INIT ( 16'h27D8 ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<11>  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [9]),
    .I2(\OCXO_pll/phase_detect/phase_diff [13]),
    .I3(\OCXO_pll/phase_loop/integrator_limit[19] ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [11])
  );
  LUT4 #(
    .INIT ( 16'h27D8 ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<13>  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [11]),
    .I2(\OCXO_pll/phase_detect/phase_diff [15]),
    .I3(\OCXO_pll/phase_loop/integrator_limit[21] ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [13])
  );
  LUT4 #(
    .INIT ( 16'h1BE4 ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<15>  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [17]),
    .I2(\OCXO_pll/phase_detect/phase_diff [13]),
    .I3(\OCXO_pll/phase_loop/integrator_limit[23] ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [15])
  );
  LUT3 #(
    .INIT ( 8'hB4 ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<0>  (
    .I0(\system_control[3] ),
    .I1(\OCXO_pll/phase_detect/phase_diff [2]),
    .I2(\OCXO_pll/phase_loop/integrator_limit[8] ),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [0])
  );
  LUT3 #(
    .INIT ( 8'h23 ))
  \OCXO_pll/phase_loop/integrator_limit<8>1  (
    .I0(\OCXO_pll/phase_loop/integrator [8]),
    .I1(\OCXO_pll/phase_loop/integrator [35]),
    .I2(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16]),
    .O(\OCXO_pll/phase_loop/integrator_limit[8] )
  );
  LUT3 #(
    .INIT ( 8'h23 ))
  \OCXO_pll/phase_loop/integrator_limit<10>1  (
    .I0(\OCXO_pll/phase_loop/integrator [10]),
    .I1(\OCXO_pll/phase_loop/integrator [35]),
    .I2(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16]),
    .O(\OCXO_pll/phase_loop/integrator_limit[10] )
  );
  LUT3 #(
    .INIT ( 8'h23 ))
  \OCXO_pll/phase_loop/integrator_limit<12>1  (
    .I0(\OCXO_pll/phase_loop/integrator [12]),
    .I1(\OCXO_pll/phase_loop/integrator [35]),
    .I2(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16]),
    .O(\OCXO_pll/phase_loop/integrator_limit[12] )
  );
  LUT3 #(
    .INIT ( 8'h23 ))
  \OCXO_pll/phase_loop/integrator_limit<13>1  (
    .I0(\OCXO_pll/phase_loop/integrator [13]),
    .I1(\OCXO_pll/phase_loop/integrator [35]),
    .I2(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16]),
    .O(\OCXO_pll/phase_loop/integrator_limit[13] )
  );
  LUT3 #(
    .INIT ( 8'h23 ))
  \OCXO_pll/phase_loop/integrator_limit<14>1  (
    .I0(\OCXO_pll/phase_loop/integrator [14]),
    .I1(\OCXO_pll/phase_loop/integrator [35]),
    .I2(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16]),
    .O(\OCXO_pll/phase_loop/integrator_limit[14] )
  );
  LUT3 #(
    .INIT ( 8'h23 ))
  \OCXO_pll/phase_loop/integrator_limit<15>1  (
    .I0(\OCXO_pll/phase_loop/integrator [15]),
    .I1(\OCXO_pll/phase_loop/integrator [35]),
    .I2(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16]),
    .O(\OCXO_pll/phase_loop/integrator_limit[15] )
  );
  LUT3 #(
    .INIT ( 8'h23 ))
  \OCXO_pll/phase_loop/integrator_limit<19>1  (
    .I0(\OCXO_pll/phase_loop/integrator [19]),
    .I1(\OCXO_pll/phase_loop/integrator [35]),
    .I2(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16]),
    .O(\OCXO_pll/phase_loop/integrator_limit[19] )
  );
  LUT3 #(
    .INIT ( 8'h23 ))
  \OCXO_pll/phase_loop/integrator_limit<21>1  (
    .I0(\OCXO_pll/phase_loop/integrator [21]),
    .I1(\OCXO_pll/phase_loop/integrator [35]),
    .I2(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16]),
    .O(\OCXO_pll/phase_loop/integrator_limit[21] )
  );
  LUT3 #(
    .INIT ( 8'h23 ))
  \OCXO_pll/phase_loop/integrator_limit<23>1  (
    .I0(\OCXO_pll/phase_loop/integrator [23]),
    .I1(\OCXO_pll/phase_loop/integrator [35]),
    .I2(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16]),
    .O(\OCXO_pll/phase_loop/integrator_limit[23] )
  );
  LUT2 #(
    .INIT ( 4'h4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_limit1011  (
    .I0(\OCXO_pll/phase_loop/integrator [35]),
    .I1(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16]),
    .O(\OCXO_pll/phase_loop/integrator_limit<0>_mand_1846 )
  );
  LUT4 #(
    .INIT ( 16'hE444 ))
  \OCXO_pll/phase_loop/dac_word_o<14>1_SW0  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(\dac_OCXO/shift_reg [13]),
    .I2(\system_control[4] ),
    .I3(genlock_register_22_6_2789),
    .O(N176)
  );
  LUT4 #(
    .INIT ( 16'hE444 ))
  \OCXO_pll/phase_loop/dac_word_o<12>1_SW0  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(\dac_OCXO/shift_reg [11]),
    .I2(\system_control[4] ),
    .I3(genlock_register_22_4_2787),
    .O(N179)
  );
  LUT4 #(
    .INIT ( 16'hE444 ))
  \OCXO_pll/phase_loop/dac_word_o<10>1_SW0  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(\dac_OCXO/shift_reg [9]),
    .I2(\system_control[4] ),
    .I3(genlock_register_22_2_2785),
    .O(N182)
  );
  LUT4 #(
    .INIT ( 16'hE444 ))
  \OCXO_pll/phase_loop/dac_word_o<9>1_SW0  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(\dac_OCXO/shift_reg [8]),
    .I2(\system_control[4] ),
    .I3(genlock_register_22_1_2784),
    .O(N185)
  );
  LUT4 #(
    .INIT ( 16'hE444 ))
  \OCXO_pll/phase_loop/dac_word_o<8>1_SW0  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(\dac_OCXO/shift_reg [7]),
    .I2(\system_control[4] ),
    .I3(genlock_register_22_0_2783),
    .O(N188)
  );
  LUT4 #(
    .INIT ( 16'hE444 ))
  \OCXO_pll/phase_loop/dac_word_o<3>1_SW0  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(\dac_OCXO/shift_reg [2]),
    .I2(\system_control[4] ),
    .I3(genlock_register_21_3_2777),
    .O(N191)
  );
  LUT4 #(
    .INIT ( 16'hE444 ))
  \OCXO_pll/phase_loop/dac_word_o<1>1_SW0  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(\dac_OCXO/shift_reg [0]),
    .I2(\system_control[4] ),
    .I3(genlock_register_21_1_2775),
    .O(N194)
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \OCXO_pll/phase_loop/integrator_limit<10>1_SW0  (
    .I0(\OCXO_pll/phase_detect/phase_diff [4]),
    .I1(\system_control[3] ),
    .O(N197)
  );
  LUT4 #(
    .INIT ( 16'hD2C3 ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<2>  (
    .I0(\OCXO_pll/phase_loop/integrator [10]),
    .I1(\OCXO_pll/phase_loop/integrator [35]),
    .I2(N197),
    .I3(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [2])
  );
  LUT3 #(
    .INIT ( 8'h1D ))
  \OCXO_pll/phase_loop/integrator_limit<12>1_SW0  (
    .I0(\OCXO_pll/phase_detect/phase_diff [6]),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [2]),
    .O(N199)
  );
  LUT4 #(
    .INIT ( 16'h2D3C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<4>  (
    .I0(\OCXO_pll/phase_loop/integrator [12]),
    .I1(\OCXO_pll/phase_loop/integrator [35]),
    .I2(N199),
    .I3(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [4])
  );
  LUT3 #(
    .INIT ( 8'h1D ))
  \OCXO_pll/phase_loop/integrator_limit<13>1_SW0  (
    .I0(\OCXO_pll/phase_detect/phase_diff [7]),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [3]),
    .O(N2011)
  );
  LUT4 #(
    .INIT ( 16'h2D3C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<5>  (
    .I0(\OCXO_pll/phase_loop/integrator [13]),
    .I1(\OCXO_pll/phase_loop/integrator [35]),
    .I2(N2011),
    .I3(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [5])
  );
  LUT3 #(
    .INIT ( 8'h1D ))
  \OCXO_pll/phase_loop/integrator_limit<14>1_SW0  (
    .I0(\OCXO_pll/phase_detect/phase_diff [8]),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [4]),
    .O(N203)
  );
  LUT4 #(
    .INIT ( 16'h2D3C ))
  \OCXO_pll/phase_loop/Madd_out_sum_lut<6>  (
    .I0(\OCXO_pll/phase_loop/integrator [14]),
    .I1(\OCXO_pll/phase_loop/integrator [35]),
    .I2(N203),
    .I3(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16]),
    .O(\OCXO_pll/phase_loop/Madd_out_sum_lut [6])
  );
  LUT4 #(
    .INIT ( 16'hCAAA ))
  \dac_OCXO/shift_reg_mux0000<14>1  (
    .I0(N176),
    .I1(N205),
    .I2(N396),
    .I3(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .O(\dac_OCXO/shift_reg_mux0000 [14])
  );
  LUT4 #(
    .INIT ( 16'hCAAA ))
  \dac_OCXO/shift_reg_mux0000<12>1  (
    .I0(N179),
    .I1(N207),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .I3(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .O(\dac_OCXO/shift_reg_mux0000 [12])
  );
  LUT4 #(
    .INIT ( 16'hCAAA ))
  \dac_OCXO/shift_reg_mux0000<10>1  (
    .I0(N182),
    .I1(N209),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .I3(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .O(\dac_OCXO/shift_reg_mux0000 [10])
  );
  LUT4 #(
    .INIT ( 16'hCAAA ))
  \dac_OCXO/shift_reg_mux0000<9>1  (
    .I0(N185),
    .I1(N211),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .I3(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .O(\dac_OCXO/shift_reg_mux0000 [9])
  );
  LUT4 #(
    .INIT ( 16'hCAAA ))
  \dac_OCXO/shift_reg_mux0000<8>1  (
    .I0(N188),
    .I1(N213),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .I3(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .O(\dac_OCXO/shift_reg_mux0000 [8])
  );
  LUT4 #(
    .INIT ( 16'hCAAA ))
  \dac_OCXO/shift_reg_mux0000<3>1  (
    .I0(N191),
    .I1(N215),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .I3(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .O(\dac_OCXO/shift_reg_mux0000 [3])
  );
  LUT4 #(
    .INIT ( 16'hCAAA ))
  \dac_OCXO/shift_reg_mux0000<1>1  (
    .I0(N194),
    .I1(N217),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .I3(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .O(\dac_OCXO/shift_reg_mux0000 [1])
  );
  LUT3 #(
    .INIT ( 8'h01 ))
  \OCXO_pll/phase_loop/dac_word_o<0>11  (
    .I0(\system_control[4] ),
    .I1(\OCXO_pll/phase_loop/out_sum [27]),
    .I2(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .O(\OCXO_pll/phase_loop/N01 )
  );
  LUT4 #(
    .INIT ( 16'hF0E2 ))
  \dac_OCXO/shift_reg_mux0000<0>1  (
    .I0(N219),
    .I1(\OCXO_pll/phase_loop/out_sum [27]),
    .I2(N220),
    .I3(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .O(\dac_OCXO/shift_reg_mux0000 [0])
  );
  LUT4 #(
    .INIT ( 16'hF0E2 ))
  \dac_OCXO/shift_reg_mux0000<7>1  (
    .I0(N258),
    .I1(\OCXO_pll/phase_loop/out_sum [27]),
    .I2(N223),
    .I3(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .O(\dac_OCXO/shift_reg_mux0000 [7])
  );
  LUT4 #(
    .INIT ( 16'hF0E2 ))
  \dac_OCXO/shift_reg_mux0000<6>1  (
    .I0(N2611),
    .I1(\OCXO_pll/phase_loop/out_sum [27]),
    .I2(N226),
    .I3(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .O(\dac_OCXO/shift_reg_mux0000 [6])
  );
  LUT4 #(
    .INIT ( 16'hF0E2 ))
  \dac_OCXO/shift_reg_mux0000<5>1  (
    .I0(N264),
    .I1(\OCXO_pll/phase_loop/out_sum [27]),
    .I2(N229),
    .I3(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .O(\dac_OCXO/shift_reg_mux0000 [5])
  );
  LUT4 #(
    .INIT ( 16'hF0E2 ))
  \dac_OCXO/shift_reg_mux0000<4>1  (
    .I0(N267),
    .I1(\OCXO_pll/phase_loop/out_sum [27]),
    .I2(N232),
    .I3(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .O(\dac_OCXO/shift_reg_mux0000 [4])
  );
  LUT4 #(
    .INIT ( 16'hF0E2 ))
  \dac_OCXO/shift_reg_mux0000<2>1  (
    .I0(N270),
    .I1(\OCXO_pll/phase_loop/out_sum [27]),
    .I2(N235),
    .I3(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .O(\dac_OCXO/shift_reg_mux0000 [2])
  );
  LUT4 #(
    .INIT ( 16'hF0E2 ))
  \dac_OCXO/shift_reg_mux0000<15>1  (
    .I0(N249),
    .I1(\OCXO_pll/phase_loop/out_sum [27]),
    .I2(N238),
    .I3(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .O(\dac_OCXO/shift_reg_mux0000 [15])
  );
  LUT4 #(
    .INIT ( 16'hF0E2 ))
  \dac_OCXO/shift_reg_mux0000<13>1  (
    .I0(N252),
    .I1(\OCXO_pll/phase_loop/out_sum [27]),
    .I2(N241),
    .I3(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .O(\dac_OCXO/shift_reg_mux0000 [13])
  );
  LUT4 #(
    .INIT ( 16'hF0E2 ))
  \dac_OCXO/shift_reg_mux0000<11>1  (
    .I0(N255),
    .I1(\OCXO_pll/phase_loop/out_sum [27]),
    .I2(N244),
    .I3(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .O(\dac_OCXO/shift_reg_mux0000 [11])
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \OCXO_pll/phase_loop/dac_word_o<0>_SW1_SW0  (
    .I0(\OCXO_pll/phase_loop/out_sum [0]),
    .I1(\system_control[4] ),
    .O(N246)
  );
  LUT4 #(
    .INIT ( 16'h569A ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<10>  (
    .I0(\OCXO_pll/phase_loop/integrator_limit[10] ),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [10]),
    .I3(\OCXO_pll/phase_detect/phase_diff [2]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [10])
  );
  LUT4 #(
    .INIT ( 16'hEEF0 ))
  \OCXO_pll/phase_loop/dac_word_o<10>11_SW0  (
    .I0(\dac_OCXO/shift_reg [13]),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I2(N176),
    .I3(\OCXO_pll/phase_loop/out_sum [14]),
    .O(N205)
  );
  LUT3 #(
    .INIT ( 8'h9A ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<8>  (
    .I0(\OCXO_pll/phase_loop/integrator_limit[8] ),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [8]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [8])
  );
  LUT4 #(
    .INIT ( 16'h6664 ))
  \LTC_B/LTC_frame_count/frame_units_mux0009<1>86  (
    .I0(\LTC_B/LTC_frame_count/frame_units [0]),
    .I1(\LTC_B/LTC_frame_count/frame_units [1]),
    .I2(\LTC_B/LTC_frame_count/frame_units [2]),
    .I3(N272),
    .O(\LTC_B/LTC_frame_count/frame_units_mux0009<1>86_769 )
  );
  LUT4 #(
    .INIT ( 16'h6664 ))
  \LTC_A/LTC_frame_count/frame_units_mux0009<1>86  (
    .I0(\LTC_A/LTC_frame_count/frame_units [0]),
    .I1(\LTC_A/LTC_frame_count/frame_units [1]),
    .I2(\LTC_A/LTC_frame_count/frame_units [2]),
    .I3(N274),
    .O(\LTC_A/LTC_frame_count/frame_units_mux0009<1>86_335 )
  );
  LUT4 #(
    .INIT ( 16'hA046 ))
  \serial_interface/time_out_or0000  (
    .I0(\serial_interface/state_FSM_FFd3_3369 ),
    .I1(\serial_interface/state_FSM_FFd1_3351 ),
    .I2(\serial_interface/state_FSM_FFd2_3359 ),
    .I3(\serial_interface/state_FSM_FFd4_3382 ),
    .O(\serial_interface/time_out_or0000_3470 )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \serial_interface/SDA_out_mux00031441  (
    .I0(\serial_interface/state_FSM_FFd2_3359 ),
    .I1(\serial_interface/state_FSM_FFd4_3382 ),
    .I2(\serial_interface/state_FSM_FFd3_3369 ),
    .I3(\serial_interface/SDA_out_mux0003113_3121 ),
    .O(\serial_interface/SDA_out_mux0003144 )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \VCXO_pll/phase_detector/phase_counter_cmp_eq000027  (
    .I0(\VCXO_pll/phase_detector/phase_counter [8]),
    .I1(\VCXO_pll/phase_detector/phase_counter [9]),
    .I2(\VCXO_pll/phase_detector/phase_counter_cmp_eq00007_2277 ),
    .I3(\VCXO_pll/phase_detector/phase_counter_cmp_eq000018_2276 ),
    .O(\VCXO_pll/phase_detector/phase_counter_cmp_eq0000 )
  );
  LUT4 #(
    .INIT ( 16'hA888 ))
  \serial_interface/state_FSM_FFd1-In145  (
    .I0(\serial_interface/state_FSM_N0 ),
    .I1(\serial_interface/state_FSM_FFd1-In118_3353 ),
    .I2(\serial_interface/state_FSM_FFd2_3359 ),
    .I3(\serial_interface/state_FSM_FFd1-In27_3357 ),
    .O(\serial_interface/state_FSM_FFd1-In )
  );
  LUT4 #(
    .INIT ( 16'hAB01 ))
  \LTC_B/LTC_frame_count/frame_units_mux0009<0>38  (
    .I0(\LTC_B/LTC_frame_count/sec_tens_and0001 ),
    .I1(\LTC_B/LTC_frame_count/frame_units [0]),
    .I2(N284),
    .I3(LTC_b_frames[0]),
    .O(\LTC_B/LTC_frame_count/frame_units_mux0009 [0])
  );
  LUT4 #(
    .INIT ( 16'hAB01 ))
  \LTC_A/LTC_frame_count/frame_units_mux0009<0>38  (
    .I0(\LTC_A/LTC_frame_count/sec_tens_and0001 ),
    .I1(\LTC_A/LTC_frame_count/frame_units [0]),
    .I2(N286),
    .I3(LTC_a_frames[0]),
    .O(\LTC_A/LTC_frame_count/frame_units_mux0009 [0])
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/send_byte_mux0001<6>181_SW0  (
    .I0(\serial_interface/send_byte_mux0001<6>37_3320 ),
    .I1(I2C_byte_out_cmp_eq0025),
    .I2(\OCXO_cv_word[14] ),
    .I3(\serial_interface/send_byte_mux0001<6>82_3326 ),
    .O(N288)
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/send_byte_mux0001<6>181  (
    .I0(\serial_interface/send_byte_mux0001<6>146_3315 ),
    .I1(I2C_byte_out_cmp_eq0024),
    .I2(\OCXO_cv_word[6] ),
    .I3(N288),
    .O(\serial_interface/send_byte_mux0001<6>181_3316 )
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/send_byte_mux0001<2>181_SW0  (
    .I0(\serial_interface/send_byte_mux0001<2>37_3234 ),
    .I1(I2C_byte_out_cmp_eq0025),
    .I2(\OCXO_cv_word[10] ),
    .I3(\serial_interface/send_byte_mux0001<2>82_3240 ),
    .O(N290)
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/send_byte_mux0001<2>181  (
    .I0(\serial_interface/send_byte_mux0001<2>146_3229 ),
    .I1(I2C_byte_out_cmp_eq0024),
    .I2(\OCXO_cv_word[2] ),
    .I3(N290),
    .O(\serial_interface/send_byte_mux0001<2>181_3230 )
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/send_byte_mux0001<0>185_SW0  (
    .I0(\serial_interface/send_byte_mux0001<0>37_3192 ),
    .I1(I2C_byte_out_cmp_eq0025),
    .I2(\OCXO_cv_word[8] ),
    .I3(\serial_interface/send_byte_mux0001<0>82_3198 ),
    .O(N292)
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/send_byte_mux0001<0>185  (
    .I0(\serial_interface/send_byte_mux0001<0>150_3187 ),
    .I1(I2C_byte_out_cmp_eq0024),
    .I2(\OCXO_cv_word[0] ),
    .I3(N292),
    .O(\serial_interface/send_byte_mux0001<0>185_3188 )
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/send_byte_mux0001<4>193_SW0  (
    .I0(\serial_interface/send_byte_mux0001<4>37_3278 ),
    .I1(I2C_byte_out_cmp_eq0025),
    .I2(\OCXO_cv_word[12] ),
    .I3(\serial_interface/send_byte_mux0001<4>86_3284 ),
    .O(N294)
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/send_byte_mux0001<4>193  (
    .I0(\serial_interface/send_byte_mux0001<4>158_3273 ),
    .I1(I2C_byte_out_cmp_eq0024),
    .I2(\OCXO_cv_word[4] ),
    .I3(N294),
    .O(\serial_interface/send_byte_mux0001<4>193_3274 )
  );
  LUT3 #(
    .INIT ( 8'hFE ))
  \serial_interface/send_byte_mux0001<6>146_SW0  (
    .I0(\serial_interface/send_byte_mux0001<6>117_3312 ),
    .I1(\serial_interface/send_byte_mux0001<6>131_3313 ),
    .I2(\serial_interface/send_byte_mux0001<6>136_3314 ),
    .O(N296)
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/send_byte_mux0001<6>146  (
    .I0(\serial_interface/send_byte_mux0001<6>106_3311 ),
    .I1(I2C_byte_out_cmp_eq0005),
    .I2(genlock_register_5_6_2825),
    .I3(N296),
    .O(\serial_interface/send_byte_mux0001<6>146_3315 )
  );
  LUT3 #(
    .INIT ( 8'hFE ))
  \serial_interface/send_byte_mux0001<2>146_SW0  (
    .I0(\serial_interface/send_byte_mux0001<2>117_3226 ),
    .I1(\serial_interface/send_byte_mux0001<2>131_3227 ),
    .I2(\serial_interface/send_byte_mux0001<2>136_3228 ),
    .O(N298)
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/send_byte_mux0001<2>146  (
    .I0(\serial_interface/send_byte_mux0001<2>106_3225 ),
    .I1(I2C_byte_out_cmp_eq0005),
    .I2(genlock_register_5_2_2821),
    .I3(N298),
    .O(\serial_interface/send_byte_mux0001<2>146_3229 )
  );
  LUT3 #(
    .INIT ( 8'hFE ))
  \serial_interface/send_byte_mux0001<0>150_SW0  (
    .I0(\serial_interface/send_byte_mux0001<0>117_3184 ),
    .I1(\serial_interface/send_byte_mux0001<0>131_3185 ),
    .I2(\serial_interface/send_byte_mux0001<0>139_3186 ),
    .O(N300)
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/send_byte_mux0001<0>150  (
    .I0(\serial_interface/send_byte_mux0001<0>106_3183 ),
    .I1(I2C_byte_out_cmp_eq0004),
    .I2(genlock_register_4_0_2810),
    .I3(N300),
    .O(\serial_interface/send_byte_mux0001<0>150_3187 )
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \LTC_B/LTC_frame_count/count_sec_or000083_SW0  (
    .I0(\LTC_B/LTC_frame_count/frame_units [3]),
    .I1(\LTC_B/LTC_frame_count/count_sec_or000032_742 ),
    .I2(\LTC_setup[5] ),
    .O(N302)
  );
  LUT4 #(
    .INIT ( 16'h0C08 ))
  \LTC_B/LTC_frame_count/count_sec_or000083  (
    .I0(\LTC_B/LTC_frame_count/count_sec_or000060 ),
    .I1(\LTC_B/LTC_frame_count/frame_tens [1]),
    .I2(\LTC_B/LTC_frame_count/frame_tens [0]),
    .I3(N302),
    .O(\LTC_B/LTC_frame_count/count_sec_or0000 )
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \LTC_A/LTC_frame_count/count_sec_or000083_SW0  (
    .I0(\LTC_A/LTC_frame_count/frame_units [3]),
    .I1(\LTC_A/LTC_frame_count/count_sec_or000032_308 ),
    .I2(\LTC_setup[1] ),
    .O(N304)
  );
  LUT4 #(
    .INIT ( 16'h0C08 ))
  \LTC_A/LTC_frame_count/count_sec_or000083  (
    .I0(\LTC_A/LTC_frame_count/count_sec_or000060 ),
    .I1(\LTC_A/LTC_frame_count/frame_tens [1]),
    .I2(\LTC_A/LTC_frame_count/frame_tens [0]),
    .I3(N304),
    .O(\LTC_A/LTC_frame_count/count_sec_or0000 )
  );
  LUT4 #(
    .INIT ( 16'h569A ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<12>  (
    .I0(\OCXO_pll/phase_loop/integrator_limit[12] ),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [12]),
    .I3(\OCXO_pll/phase_detect/phase_diff [4]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [12])
  );
  LUT4 #(
    .INIT ( 16'h569A ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<13>  (
    .I0(\OCXO_pll/phase_loop/integrator_limit[13] ),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [13]),
    .I3(\OCXO_pll/phase_detect/phase_diff [5]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [13])
  );
  LUT4 #(
    .INIT ( 16'h569A ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<14>  (
    .I0(\OCXO_pll/phase_loop/integrator_limit[14] ),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [14]),
    .I3(\OCXO_pll/phase_detect/phase_diff [6]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [14])
  );
  LUT4 #(
    .INIT ( 16'h569A ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<15>  (
    .I0(\OCXO_pll/phase_loop/integrator_limit[15] ),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [15]),
    .I3(\OCXO_pll/phase_detect/phase_diff [7]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [15])
  );
  LUT4 #(
    .INIT ( 16'h569A ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<19>  (
    .I0(\OCXO_pll/phase_loop/integrator_limit[19] ),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [19]),
    .I3(\OCXO_pll/phase_detect/phase_diff [11]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [19])
  );
  LUT4 #(
    .INIT ( 16'h569A ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<21>  (
    .I0(\OCXO_pll/phase_loop/integrator_limit[21] ),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [21]),
    .I3(\OCXO_pll/phase_detect/phase_diff [13]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [21])
  );
  LUT4 #(
    .INIT ( 16'h569A ))
  \OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut<23>  (
    .I0(\OCXO_pll/phase_loop/integrator_limit[23] ),
    .I1(\system_control[3] ),
    .I2(\OCXO_pll/phase_detect/phase_diff [23]),
    .I3(\OCXO_pll/phase_detect/phase_diff [15]),
    .O(\OCXO_pll/phase_loop/Madd_integrator_addsub0000_lut [23])
  );
  LUT3 #(
    .INIT ( 8'h32 ))
  \serial_interface/state_FSM_FFd3-In88_SW0  (
    .I0(\serial_interface/state_FSM_FFd3-In33 ),
    .I1(\serial_interface/state_cmp_eq0011 ),
    .I2(\serial_interface/state_FSM_FFd3-In51 ),
    .O(N306)
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/state_FSM_FFd3-In88  (
    .I0(\serial_interface/state_FSM_FFd3-In13_3372 ),
    .I1(\serial_interface/command [2]),
    .I2(\serial_interface/state_FSM_FFd1_3351 ),
    .I3(N306),
    .O(\serial_interface/state_FSM_FFd3-In88_3381 )
  );
  LUT4 #(
    .INIT ( 16'hDF57 ))
  \serial_interface/state_FSM_FFd4-In40_SW0  (
    .I0(\serial_interface/state_cmp_eq0008 ),
    .I1(\serial_interface/command [0]),
    .I2(\serial_interface/command [1]),
    .I3(\serial_interface/recieve_byte [0]),
    .O(N308)
  );
  LUT4 #(
    .INIT ( 16'hAB01 ))
  \serial_interface/state_FSM_FFd4-In40  (
    .I0(\serial_interface/state_FSM_FFd3_3369 ),
    .I1(\serial_interface/state_FSM_FFd4_3382 ),
    .I2(N308),
    .I3(\serial_interface/command [2]),
    .O(\serial_interface/state_FSM_FFd4-In40_3388 )
  );
  LUT4 #(
    .INIT ( 16'h6420 ))
  \serial_interface/state_FSM_FFd1-In27  (
    .I0(\serial_interface/state_FSM_FFd3_3369 ),
    .I1(\serial_interface/state_FSM_FFd4_3382 ),
    .I2(\serial_interface/state_FSM_FFd1-In16_3356 ),
    .I3(N310),
    .O(\serial_interface/state_FSM_FFd1-In27_3357 )
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/state_FSM_FFd4-In2161_SW0  (
    .I0(\serial_interface/state_FSM_FFd4-In179_3386 ),
    .I1(\serial_interface/state_FSM_FFd1_3351 ),
    .I2(\serial_interface/state_FSM_FFd4-In40_3388 ),
    .I3(\serial_interface/state_FSM_FFd4-In66 ),
    .O(N312)
  );
  LUT4 #(
    .INIT ( 16'hCC4C ))
  \serial_interface/state_FSM_FFd4-In2161  (
    .I0(\serial_interface/SDA_delay [0]),
    .I1(N312),
    .I2(\serial_interface/state_cmp_eq0001 ),
    .I3(\serial_interface/SDA_delay [1]),
    .O(\serial_interface/state_FSM_FFd4-In216 )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \OCXO_pll/phase_detect/phase_counter_cst1  (
    .I0(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .I1(\OCXO_pll/phase_detect/phase_counter_cmp_eq00004_1541 ),
    .I2(\OCXO_pll/phase_detect/phase_counter_cmp_eq000011 ),
    .I3(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy [4]),
    .O(\OCXO_pll/phase_detect/phase_counter_cst )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  I2C_byte_out_cmp_eq00241 (
    .I0(\communication/address_o [1]),
    .I1(\communication/address_o [5]),
    .I2(N14),
    .I3(\communication/address_o [0]),
    .O(I2C_byte_out_cmp_eq0024)
  );
  LUT3 #(
    .INIT ( 8'hA8 ))
  \LTC_B/LTC_clockgenerator/period_count_mux0003<10>11  (
    .I0(\LTC_setup[5] ),
    .I1(\LTC_B/delay_sync_983 ),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .O(\LTC_B/LTC_clockgenerator/N5 )
  );
  LUT3 #(
    .INIT ( 8'hA8 ))
  \LTC_A/LTC_clockgenerator/period_count_mux0003<10>11  (
    .I0(\LTC_setup[1] ),
    .I1(\LTC_A/delay_sync_549 ),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .O(\LTC_A/LTC_clockgenerator/N5 )
  );
  LUT2 #(
    .INIT ( 4'h1 ))
  \dac_OCXO/bit_count_not0001_inv1  (
    .I0(\dac_OCXO/bit_count_not00014_2435 ),
    .I1(\dac_OCXO/bit_count_not000111 ),
    .O(\dac_OCXO/bit_count_not0001_inv )
  );
  LUT3 #(
    .INIT ( 8'hF1 ))
  \dac_OCXO/Mcount_bit_count_val1  (
    .I0(\dac_OCXO/bit_count_not00014_2435 ),
    .I1(\dac_OCXO/bit_count_not000111 ),
    .I2(\OCXO_pll/phase_detect/update_tick_1570 ),
    .O(\dac_OCXO/Mcount_bit_count_val )
  );
  LUT2 #(
    .INIT ( 4'h7 ))
  \OCXO_pll/phase_detect/phase_counter_and0000_inv1  (
    .I0(\system_control[2] ),
    .I1(kvant_pps_3018),
    .O(\OCXO_pll/phase_detect/phase_counter_and0000_inv )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  I2C_byte_out_cmp_eq00251 (
    .I0(\communication/address_o [0]),
    .I1(N14),
    .I2(\communication/address_o [1]),
    .I3(\communication/address_o [5]),
    .O(I2C_byte_out_cmp_eq0025)
  );
  LUT4 #(
    .INIT ( 16'h0C08 ))
  \serial_interface/send_byte_mux0001<4>75  (
    .I0(\communication/address_o [5]),
    .I1(N14),
    .I2(N26),
    .I3(genlock_register_0_4_2661),
    .O(\serial_interface/send_byte_mux0001<4>75_3283 )
  );
  LUT4 #(
    .INIT ( 16'hFF80 ))
  \OCXO_pll/phase_detect/Mcount_phase_counter_val1  (
    .I0(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq00001_wg_cy [4]),
    .I1(\OCXO_pll/phase_detect/phase_counter_cmp_eq00004_1541 ),
    .I2(\OCXO_pll/phase_detect/phase_counter_cmp_eq000011 ),
    .I3(\OCXO_pll/phase_detect/phase_counter_and0000 ),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_val )
  );
  LUT2 #(
    .INIT ( 4'h7 ))
  \VCXO_pll/ref_clk_not00011  (
    .I0(\VCXO_pll/ref_clk_count_cmp_eq00009 ),
    .I1(\VCXO_pll/ref_clk_count_cmp_eq000019_2302 ),
    .O(\VCXO_pll/ref_clk_not0001 )
  );
  LUT3 #(
    .INIT ( 8'h08 ))
  \VCXO_pll/resync_148_period_and00001  (
    .I0(\VCXO_pll/ref_clk_count_cmp_eq00009 ),
    .I1(\VCXO_pll/ref_clk_count_cmp_eq000019_2302 ),
    .I2(\VCXO_pll/ref_clk_2291 ),
    .O(\VCXO_pll/resync_148_period_and0000 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [1]),
    .I1(\LTC_B/LTC_frame_count/frame_tens_o [1]),
    .I2(\LTC_B/LTC_frame_count/frame_tens_o [0]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_559 )
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_131  (
    .I0(\LTC_setup[6] ),
    .I1(\LTC_B/LTC_clockgenerator/ltc_bitcount [1]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_131_556 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [1]),
    .I1(\LTC_B/LTC_frame_count/frame_units_o [3]),
    .I2(\LTC_B/LTC_frame_count/frame_units_o [2]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_555 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_12  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [1]),
    .I1(\LTC_B/LTC_frame_count/frame_units_o [1]),
    .I2(\LTC_B/LTC_frame_count/frame_units_o [0]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_12_554 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [1]),
    .I1(\LTC_A/LTC_frame_count/frame_tens_o [1]),
    .I2(\LTC_A/LTC_frame_count/frame_tens_o [0]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_125 )
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_131  (
    .I0(\LTC_setup[2] ),
    .I1(\LTC_A/LTC_clockgenerator/ltc_bitcount [1]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_131_122 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [1]),
    .I1(\LTC_A/LTC_frame_count/frame_units_o [3]),
    .I2(\LTC_A/LTC_frame_count/frame_units_o [2]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_121 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_12  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [1]),
    .I1(\LTC_A/LTC_frame_count/frame_units_o [1]),
    .I2(\LTC_A/LTC_frame_count/frame_units_o [0]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_12_120 )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \serial_interface/state_cmp_eq00071  (
    .I0(\serial_interface/bit_count [3]),
    .I1(\serial_interface/bit_count [2]),
    .I2(\serial_interface/bit_count [1]),
    .I3(\serial_interface/bit_count [0]),
    .O(\serial_interface/state_cmp_eq0007 )
  );
  LUT4 #(
    .INIT ( 16'hFF08 ))
  \serial_interface/state_FSM_FFd1-In16  (
    .I0(\serial_interface/command [1]),
    .I1(\serial_interface/SCL_delay [1]),
    .I2(\serial_interface/SCL_delay [0]),
    .I3(\serial_interface/command [2]),
    .O(\serial_interface/state_FSM_FFd1-In16_3356 )
  );
  LUT3 #(
    .INIT ( 8'hC4 ))
  \serial_interface/state_FSM_FFd4-In122  (
    .I0(\serial_interface/SCL_delay [1]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/SCL_delay [0]),
    .O(\serial_interface/state_FSM_FFd4-In122_3384 )
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  \serial_interface/recieve_byte_0_mux000121  (
    .I0(\serial_interface/state_FSM_FFd2_3359 ),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(N156),
    .I3(\serial_interface/state_FSM_FFd4_3382 ),
    .O(N15)
  );
  LUT4 #(
    .INIT ( 16'hF77F ))
  \serial_interface/state_FSM_FFd2-In11  (
    .I0(\serial_interface/SCL_delay [0]),
    .I1(\serial_interface/SCL_delay [1]),
    .I2(\serial_interface/SDA_delay [0]),
    .I3(\serial_interface/SDA_delay [1]),
    .O(\serial_interface/state_FSM_N0 )
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \Inst_ad5660_serial_DAC/bit_count_mux0000<0>_SW0  (
    .I0(\Inst_ad5660_serial_DAC/bit_count [3]),
    .I1(\Inst_ad5660_serial_DAC/bit_count [2]),
    .I2(\Inst_ad5660_serial_DAC/bit_count [1]),
    .I3(\Inst_ad5660_serial_DAC/bit_count [0]),
    .O(N92)
  );
  LUT4 #(
    .INIT ( 16'hA8B9 ))
  \serial_interface/bit_count_or00001  (
    .I0(\serial_interface/state_FSM_FFd1_3351 ),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd4_3382 ),
    .I3(\serial_interface/state_FSM_FFd2_3359 ),
    .O(\serial_interface/bit_count_or0000 )
  );
  LUT4 #(
    .INIT ( 16'h0010 ))
  \communication/state_and00002  (
    .I0(\communication/update_line [1]),
    .I1(\communication/state_FSM_FFd2_2370 ),
    .I2(\communication/update_line [0]),
    .I3(\communication/state_FSM_FFd1_2366 ),
    .O(\communication/state_and0000 )
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \serial_interface/SDA_out_mux00030  (
    .I0(\serial_interface/state_FSM_FFd3_3369 ),
    .I1(\serial_interface/state_FSM_FFd4_3382 ),
    .I2(\serial_interface/state_FSM_FFd2_3359 ),
    .I3(\serial_interface/SDA_out_3119 ),
    .O(\serial_interface/SDA_out_mux00030_3120 )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \serial_interface/state_FSM_FFd4-In5  (
    .I0(\serial_interface/SDA_delay [0]),
    .I1(\serial_interface/SCL_delay [1]),
    .I2(\serial_interface/SDA_delay [1]),
    .I3(\serial_interface/SCL_delay [0]),
    .O(\serial_interface/state_FSM_FFd4-In5_3389 )
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \serial_interface/send_byte_mux0001<6>0  (
    .I0(\serial_interface/send_byte [6]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd4_3382 ),
    .I3(\serial_interface/state_FSM_FFd2_3359 ),
    .O(\serial_interface/send_byte_mux0001<6>0_3309 )
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \serial_interface/send_byte_mux0001<2>0  (
    .I0(\serial_interface/send_byte [2]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd4_3382 ),
    .I3(\serial_interface/state_FSM_FFd2_3359 ),
    .O(\serial_interface/send_byte_mux0001<2>0_3223 )
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \serial_interface/send_byte_mux0001<0>0  (
    .I0(\serial_interface/send_byte [0]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd4_3382 ),
    .I3(\serial_interface/state_FSM_FFd2_3359 ),
    .O(\serial_interface/send_byte_mux0001<0>0_3181 )
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \serial_interface/send_byte_mux0001<7>0  (
    .I0(\serial_interface/send_byte [7]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd4_3382 ),
    .I3(\serial_interface/state_FSM_FFd2_3359 ),
    .O(\serial_interface/send_byte_mux0001<7>0_3327 )
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \serial_interface/send_byte_mux0001<5>0  (
    .I0(\serial_interface/send_byte [5]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd4_3382 ),
    .I3(\serial_interface/state_FSM_FFd2_3359 ),
    .O(\serial_interface/send_byte_mux0001<5>0_3285 )
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \serial_interface/send_byte_mux0001<3>0  (
    .I0(\serial_interface/send_byte [3]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd4_3382 ),
    .I3(\serial_interface/state_FSM_FFd2_3359 ),
    .O(\serial_interface/send_byte_mux0001<3>0_3241 )
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \serial_interface/send_byte_mux0001<4>0  (
    .I0(\serial_interface/send_byte [4]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd4_3382 ),
    .I3(\serial_interface/state_FSM_FFd2_3359 ),
    .O(\serial_interface/send_byte_mux0001<4>0_3265 )
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \serial_interface/send_byte_mux0001<1>0  (
    .I0(\serial_interface/send_byte [1]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd4_3382 ),
    .I3(\serial_interface/state_FSM_FFd2_3359 ),
    .O(\serial_interface/send_byte_mux0001<1>0_3199 )
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \LTC_B/LTC_frame_count/sec_units_not00011  (
    .I0(\LTC_B/LTC_frame_count/count_sec_740 ),
    .I1(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .O(\LTC_B/LTC_frame_count/sec_units_not0001 )
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \LTC_A/LTC_frame_count/sec_units_not00011  (
    .I0(\LTC_A/LTC_frame_count/count_sec_306 ),
    .I1(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .O(\LTC_A/LTC_frame_count/sec_units_not0001 )
  );
  LUT3 #(
    .INIT ( 8'hB1 ))
  \Inst_ad5660_serial_DAC/sclk_mux00001  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd3_107 ),
    .I1(\Inst_ad5660_serial_DAC/state_FSM_FFd1_103 ),
    .I2(\Inst_ad5660_serial_DAC/sclk_53 ),
    .O(\Inst_ad5660_serial_DAC/sclk_mux0000 )
  );
  LUT4 #(
    .INIT ( 16'h4446 ))
  \Inst_ad5660_serial_DAC/bit_count_mux0000<4>1  (
    .I0(\Inst_ad5660_serial_DAC/bit_count [0]),
    .I1(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I2(\Inst_ad5660_serial_DAC/state_FSM_FFd5_113 ),
    .I3(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .O(\Inst_ad5660_serial_DAC/bit_count_mux0000 [4])
  );
  LUT4 #(
    .INIT ( 16'h0018 ))
  \serial_interface/bit_count_mux0002<0>1  (
    .I0(\serial_interface/state_FSM_FFd4_3382 ),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd2_3359 ),
    .I3(\serial_interface/bit_count [0]),
    .O(\serial_interface/bit_count_mux0002 [0])
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \communication/address_o_mux0000<7>1  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\serial_interface/recieved_byte_o [7]),
    .I3(\communication/address [7]),
    .O(\communication/address_o_mux0000 [7])
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \communication/address_o_mux0000<6>1  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\serial_interface/recieved_byte_o [6]),
    .I3(\communication/address [6]),
    .O(\communication/address_o_mux0000 [6])
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \communication/address_o_mux0000<5>1  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\serial_interface/recieved_byte_o [5]),
    .I3(\communication/address [5]),
    .O(\communication/address_o_mux0000 [5])
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \communication/address_o_mux0000<4>1  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\serial_interface/recieved_byte_o [4]),
    .I3(\communication/address [4]),
    .O(\communication/address_o_mux0000 [4])
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \communication/address_o_mux0000<3>1  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\serial_interface/recieved_byte_o [3]),
    .I3(\communication/address [3]),
    .O(\communication/address_o_mux0000 [3])
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \communication/address_o_mux0000<2>1  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\serial_interface/recieved_byte_o [2]),
    .I3(\communication/address [2]),
    .O(\communication/address_o_mux0000 [2])
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \communication/address_o_mux0000<1>1  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\serial_interface/recieved_byte_o [1]),
    .I3(\communication/address [1]),
    .O(\communication/address_o_mux0000 [1])
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \communication/address_o_mux0000<0>1  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\serial_interface/recieved_byte_o [0]),
    .I3(\communication/address [0]),
    .O(\communication/address_o_mux0000 [0])
  );
  LUT4 #(
    .INIT ( 16'hB313 ))
  \LTC_B/LTC_frame_count/Mcount_sec_units_xor<0>11  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I1(\LTC_B/LTC_frame_count/sec_units [0]),
    .I2(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I3(LTC_b_secs[0]),
    .O(\LTC_B/LTC_frame_count/Mcount_sec_units )
  );
  LUT4 #(
    .INIT ( 16'hB313 ))
  \LTC_B/LTC_frame_count/Mcount_sec_tens_xor<0>11  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I1(\LTC_B/LTC_frame_count/sec_tens [0]),
    .I2(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I3(LTC_b_secs[4]),
    .O(\LTC_B/LTC_frame_count/Mcount_sec_tens )
  );
  LUT4 #(
    .INIT ( 16'hB313 ))
  \LTC_B/LTC_frame_count/Mcount_min_units_xor<0>11  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I1(\LTC_B/LTC_frame_count/min_units [0]),
    .I2(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I3(LTC_b_mins[0]),
    .O(\LTC_B/LTC_frame_count/Mcount_min_units )
  );
  LUT4 #(
    .INIT ( 16'hB313 ))
  \LTC_B/LTC_frame_count/Mcount_min_tens_xor<0>11  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I1(\LTC_B/LTC_frame_count/min_tens [0]),
    .I2(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I3(LTC_b_mins[4]),
    .O(\LTC_B/LTC_frame_count/Mcount_min_tens )
  );
  LUT4 #(
    .INIT ( 16'hB313 ))
  \LTC_B/LTC_frame_count/Mcount_hour_units_xor<0>11  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I1(\LTC_B/LTC_frame_count/hour_units [0]),
    .I2(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I3(LTC_b_hours[0]),
    .O(\LTC_B/LTC_frame_count/Mcount_hour_units )
  );
  LUT4 #(
    .INIT ( 16'hB313 ))
  \LTC_A/LTC_frame_count/Mcount_sec_units_xor<0>11  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I1(\LTC_A/LTC_frame_count/sec_units [0]),
    .I2(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I3(LTC_a_secs[0]),
    .O(\LTC_A/LTC_frame_count/Mcount_sec_units )
  );
  LUT4 #(
    .INIT ( 16'hB313 ))
  \LTC_A/LTC_frame_count/Mcount_sec_tens_xor<0>11  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I1(\LTC_A/LTC_frame_count/sec_tens [0]),
    .I2(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I3(LTC_a_secs[4]),
    .O(\LTC_A/LTC_frame_count/Mcount_sec_tens )
  );
  LUT4 #(
    .INIT ( 16'hB313 ))
  \LTC_A/LTC_frame_count/Mcount_min_units_xor<0>11  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I1(\LTC_A/LTC_frame_count/min_units [0]),
    .I2(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I3(LTC_a_mins[0]),
    .O(\LTC_A/LTC_frame_count/Mcount_min_units )
  );
  LUT4 #(
    .INIT ( 16'hB313 ))
  \LTC_A/LTC_frame_count/Mcount_min_tens_xor<0>11  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I1(\LTC_A/LTC_frame_count/min_tens [0]),
    .I2(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I3(LTC_a_mins[4]),
    .O(\LTC_A/LTC_frame_count/Mcount_min_tens )
  );
  LUT4 #(
    .INIT ( 16'hB313 ))
  \LTC_A/LTC_frame_count/Mcount_hour_units_xor<0>11  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I1(\LTC_A/LTC_frame_count/hour_units [0]),
    .I2(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I3(LTC_a_hours[0]),
    .O(\LTC_A/LTC_frame_count/Mcount_hour_units )
  );
  LUT3 #(
    .INIT ( 8'h01 ))
  \Inst_ad5660_serial_DAC/data_o_mux000111  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .I1(\Inst_ad5660_serial_DAC/state_FSM_FFd3_107 ),
    .I2(\Inst_ad5660_serial_DAC/state_FSM_FFd1_103 ),
    .O(\Inst_ad5660_serial_DAC/data_o_mux00011 )
  );
  LUT4 #(
    .INIT ( 16'h3732 ))
  \LTC_B/LTC_clockgenerator/period_count_mux0003<11>11  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .I1(\LTC_setup[4] ),
    .I2(\LTC_B/delay_sync_983 ),
    .I3(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [4]),
    .O(\LTC_B/LTC_clockgenerator/period_count_mux0003<11>1 )
  );
  LUT4 #(
    .INIT ( 16'h3732 ))
  \LTC_A/LTC_clockgenerator/period_count_mux0003<11>11  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .I1(\LTC_setup[0] ),
    .I2(\LTC_A/delay_sync_549 ),
    .I3(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [4]),
    .O(\LTC_A/LTC_clockgenerator/period_count_mux0003<11>1 )
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \LTC_B/LTC_clockgenerator/period_count_mux0003<12>11  (
    .I0(\LTC_B/delay_sync_983 ),
    .I1(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [3]),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .O(\LTC_B/LTC_clockgenerator/period_count_mux0003<12>1 )
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \LTC_A/LTC_clockgenerator/period_count_mux0003<12>11  (
    .I0(\LTC_A/delay_sync_549 ),
    .I1(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [3]),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .O(\LTC_A/LTC_clockgenerator/period_count_mux0003<12>1 )
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \LTC_B/LTC_clockgenerator/period_count_mux0003<9>11  (
    .I0(\LTC_B/delay_sync_983 ),
    .I1(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [6]),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .O(\LTC_B/LTC_clockgenerator/period_count_mux0003<9>1 )
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \LTC_A/LTC_clockgenerator/period_count_mux0003<9>11  (
    .I0(\LTC_A/delay_sync_549 ),
    .I1(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [6]),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .O(\LTC_A/LTC_clockgenerator/period_count_mux0003<9>1 )
  );
  LUT4 #(
    .INIT ( 16'h3732 ))
  \LTC_B/LTC_clockgenerator/period_count_mux0003<7>1  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .I1(\LTC_setup[5] ),
    .I2(\LTC_B/delay_sync_983 ),
    .I3(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [8]),
    .O(\LTC_B/LTC_clockgenerator/period_count_mux0003[7] )
  );
  LUT4 #(
    .INIT ( 16'h3732 ))
  \LTC_A/LTC_clockgenerator/period_count_mux0003<7>1  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .I1(\LTC_setup[1] ),
    .I2(\LTC_A/delay_sync_549 ),
    .I3(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [8]),
    .O(\LTC_A/LTC_clockgenerator/period_count_mux0003[7] )
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \LTC_B/LTC_clockgenerator/period_count_mux0003<8>11  (
    .I0(\LTC_B/delay_sync_983 ),
    .I1(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [7]),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .O(\LTC_B/LTC_clockgenerator/period_count_mux0003<8>1 )
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \LTC_A/LTC_clockgenerator/period_count_mux0003<8>11  (
    .I0(\LTC_A/delay_sync_549 ),
    .I1(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [7]),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .O(\LTC_A/LTC_clockgenerator/period_count_mux0003<8>1 )
  );
  LUT4 #(
    .INIT ( 16'h3732 ))
  \LTC_B/LTC_clockgenerator/period_count_mux0003<0>1  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .I1(\LTC_setup[5] ),
    .I2(\LTC_B/delay_sync_983 ),
    .I3(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [15]),
    .O(\LTC_B/LTC_clockgenerator/period_count_mux0003[0] )
  );
  LUT4 #(
    .INIT ( 16'h3732 ))
  \LTC_A/LTC_clockgenerator/period_count_mux0003<0>1  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .I1(\LTC_setup[1] ),
    .I2(\LTC_A/delay_sync_549 ),
    .I3(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [15]),
    .O(\LTC_A/LTC_clockgenerator/period_count_mux0003[0] )
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  genlock_register_7_and00001 (
    .I0(\communication/address_o [1]),
    .I1(\communication/address_o [0]),
    .I2(\communication/update_register_o_2379 ),
    .I3(N17),
    .O(genlock_register_7_and0000)
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  genlock_register_3_and00001 (
    .I0(\communication/address_o [1]),
    .I1(\communication/address_o [0]),
    .I2(\communication/update_register_o_2379 ),
    .I3(N14),
    .O(genlock_register_3_and0000)
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  genlock_register_19_and00001 (
    .I0(\communication/address_o [1]),
    .I1(\communication/address_o [0]),
    .I2(\communication/update_register_o_2379 ),
    .I3(N18),
    .O(genlock_register_19_and0000)
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  genlock_register_15_and00001 (
    .I0(\communication/address_o [1]),
    .I1(\communication/address_o [0]),
    .I2(\communication/update_register_o_2379 ),
    .I3(N19),
    .O(genlock_register_15_and0000)
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  genlock_register_11_and00001 (
    .I0(\communication/address_o [1]),
    .I1(\communication/address_o [0]),
    .I2(\communication/update_register_o_2379 ),
    .I3(N20),
    .O(genlock_register_11_and0000)
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \LTC_B/LTC_frame_count/sec_tens_not00011  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I1(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I2(\LTC_B/LTC_frame_count/count_sec_740 ),
    .I3(\LTC_B/LTC_frame_count/N12 ),
    .O(\LTC_B/LTC_frame_count/sec_tens_not0001 )
  );
  LUT4 #(
    .INIT ( 16'hF888 ))
  \LTC_A/LTC_frame_count/sec_tens_not00011  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I1(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I2(\LTC_A/LTC_frame_count/count_sec_306 ),
    .I3(\LTC_A/LTC_frame_count/N12 ),
    .O(\LTC_A/LTC_frame_count/sec_tens_not0001 )
  );
  LUT4 #(
    .INIT ( 16'h0010 ))
  I2C_byte_out_cmp_eq00082 (
    .I0(\communication/address_o [5]),
    .I1(\communication/address_o [0]),
    .I2(N20),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0008)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  I2C_byte_out_cmp_eq00071 (
    .I0(\communication/address_o [5]),
    .I1(\communication/address_o [0]),
    .I2(N17),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0007)
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  I2C_byte_out_cmp_eq00271 (
    .I0(\communication/address_o [5]),
    .I1(\communication/address_o [0]),
    .I2(N17),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0027)
  );
  LUT4 #(
    .INIT ( 16'h0010 ))
  I2C_byte_out_cmp_eq00201 (
    .I0(\communication/address_o [5]),
    .I1(\communication/address_o [0]),
    .I2(N21),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0020)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  I2C_byte_out_cmp_eq00191 (
    .I0(\communication/address_o [5]),
    .I1(\communication/address_o [0]),
    .I2(N18),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0019)
  );
  LUT4 #(
    .INIT ( 16'h0010 ))
  I2C_byte_out_cmp_eq00161 (
    .I0(\communication/address_o [5]),
    .I1(\communication/address_o [0]),
    .I2(N18),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0016)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  I2C_byte_out_cmp_eq00151 (
    .I0(\communication/address_o [5]),
    .I1(\communication/address_o [0]),
    .I2(N19),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0015)
  );
  LUT4 #(
    .INIT ( 16'h0010 ))
  I2C_byte_out_cmp_eq00121 (
    .I0(\communication/address_o [5]),
    .I1(\communication/address_o [0]),
    .I2(N19),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0012)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  I2C_byte_out_cmp_eq00111 (
    .I0(\communication/address_o [5]),
    .I1(\communication/address_o [0]),
    .I2(N20),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0011)
  );
  LUT4 #(
    .INIT ( 16'h0010 ))
  I2C_byte_out_cmp_eq00041 (
    .I0(\communication/address_o [5]),
    .I1(\communication/address_o [0]),
    .I2(N17),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0004)
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  I2C_byte_out_cmp_eq00031 (
    .I0(\communication/address_o [5]),
    .I1(\communication/address_o [0]),
    .I2(N14),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0003)
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/send_byte_mux0001<7>5  (
    .I0(\communication/address_o [7]),
    .I1(genlock_register_2_7_2799),
    .I2(I2C_byte_out_cmp_eq0002),
    .I3(\communication/address_o [6]),
    .O(\serial_interface/send_byte_mux0001<7>5_3346 )
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/send_byte_mux0001<5>5  (
    .I0(\communication/address_o [7]),
    .I1(genlock_register_2_5_2797),
    .I2(I2C_byte_out_cmp_eq0002),
    .I3(\communication/address_o [6]),
    .O(\serial_interface/send_byte_mux0001<5>5_3304 )
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/send_byte_mux0001<3>5  (
    .I0(\communication/address_o [7]),
    .I1(genlock_register_2_3_2795),
    .I2(I2C_byte_out_cmp_eq0002),
    .I3(\communication/address_o [6]),
    .O(\serial_interface/send_byte_mux0001<3>5_3260 )
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \serial_interface/send_byte_mux0001<1>5  (
    .I0(\communication/address_o [7]),
    .I1(genlock_register_2_1_2793),
    .I2(I2C_byte_out_cmp_eq0002),
    .I3(\communication/address_o [6]),
    .O(\serial_interface/send_byte_mux0001<1>5_3218 )
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  \serial_interface/state_FSM_FFd2-In78  (
    .I0(\serial_interface/state_cmp_eq0007 ),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd2-In72_3366 ),
    .I3(\serial_interface/state_FSM_FFd4_3382 ),
    .O(\serial_interface/state_FSM_FFd2-In78_3367 )
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  genlock_register_8_and00001 (
    .I0(\communication/update_register_o_2379 ),
    .I1(\communication/address_o [0]),
    .I2(N20),
    .I3(\communication/address_o [1]),
    .O(genlock_register_8_and0000)
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  genlock_register_4_and00001 (
    .I0(\communication/update_register_o_2379 ),
    .I1(\communication/address_o [0]),
    .I2(N17),
    .I3(\communication/address_o [1]),
    .O(genlock_register_4_and0000)
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  genlock_register_20_and00001 (
    .I0(\communication/update_register_o_2379 ),
    .I1(\communication/address_o [0]),
    .I2(N21),
    .I3(\communication/address_o [1]),
    .O(genlock_register_20_and0000)
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  genlock_register_16_and00001 (
    .I0(\communication/update_register_o_2379 ),
    .I1(\communication/address_o [0]),
    .I2(N18),
    .I3(\communication/address_o [1]),
    .O(genlock_register_16_and0000)
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  genlock_register_12_and00001 (
    .I0(\communication/update_register_o_2379 ),
    .I1(\communication/address_o [0]),
    .I2(N19),
    .I3(\communication/address_o [1]),
    .O(genlock_register_12_and0000)
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  genlock_register_0_and00001 (
    .I0(\communication/update_register_o_2379 ),
    .I1(\communication/address_o [0]),
    .I2(N14),
    .I3(\communication/address_o [1]),
    .O(genlock_register_0_and0000)
  );
  LUT3 #(
    .INIT ( 8'hC4 ))
  \OCXO_pll/phase_loop/dac_word_o<0>11_SW0  (
    .I0(\system_control[4] ),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I2(genlock_register_21_0_2774),
    .O(N219)
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \OCXO_pll/phase_loop/dac_word_o<10>11  (
    .I0(\system_control[4] ),
    .I1(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_cy [13]),
    .I2(\OCXO_pll/phase_loop/out_sum [27]),
    .O(\OCXO_pll/phase_loop/N11 )
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \LTC_B/LTC_clockgenerator/period_count_mux0003<1>11  (
    .I0(\LTC_B/delay_sync_983 ),
    .I1(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [14]),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .O(\LTC_B/LTC_clockgenerator/period_count_mux0003<1>1 )
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \LTC_A/LTC_clockgenerator/period_count_mux0003<1>11  (
    .I0(\LTC_A/delay_sync_549 ),
    .I1(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [14]),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .O(\LTC_A/LTC_clockgenerator/period_count_mux0003<1>1 )
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \LTC_B/LTC_clockgenerator/period_count_mux0003<2>11  (
    .I0(\LTC_B/delay_sync_983 ),
    .I1(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [13]),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .O(\LTC_B/LTC_clockgenerator/period_count_mux0003<2>1 )
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \LTC_A/LTC_clockgenerator/period_count_mux0003<2>11  (
    .I0(\LTC_A/delay_sync_549 ),
    .I1(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [13]),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .O(\LTC_A/LTC_clockgenerator/period_count_mux0003<2>1 )
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \LTC_B/LTC_clockgenerator/period_count_mux0003<4>11  (
    .I0(\LTC_B/delay_sync_983 ),
    .I1(\LTC_B/LTC_clockgenerator/period_count_addsub0000 [11]),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .O(\LTC_B/LTC_clockgenerator/period_count_mux0003<4>1 )
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \LTC_A/LTC_clockgenerator/period_count_mux0003<4>11  (
    .I0(\LTC_A/delay_sync_549 ),
    .I1(\LTC_A/LTC_clockgenerator/period_count_addsub0000 [11]),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .O(\LTC_A/LTC_clockgenerator/period_count_mux0003<4>1 )
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_B/LTC_frame_count/frame_tens_mux0002<4>  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I1(N74),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I3(LTC_b_frames[4]),
    .O(\LTC_B/LTC_frame_count/frame_tens_mux0002 [4])
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_A/LTC_frame_count/frame_tens_mux0002<4>  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I1(N76),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I3(LTC_a_frames[4]),
    .O(\LTC_A/LTC_frame_count/frame_tens_mux0002 [4])
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_B/LTC_frame_count/frame_units_mux0009<3>  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I1(N94),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I3(LTC_b_frames[3]),
    .O(\LTC_B/LTC_frame_count/frame_units_mux0009 [3])
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_A/LTC_frame_count/frame_units_mux0009<3>  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I1(N96),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I3(LTC_a_frames[3]),
    .O(\LTC_A/LTC_frame_count/frame_units_mux0009 [3])
  );
  LUT3 #(
    .INIT ( 8'h57 ))
  \serial_interface/state_FSM_FFd2-In99_SW0  (
    .I0(\serial_interface/SCL_delay [1]),
    .I1(\serial_interface/command [1]),
    .I2(\serial_interface/state_FSM_FFd4_3382 ),
    .O(N316)
  );
  LUT4 #(
    .INIT ( 16'h0C08 ))
  \serial_interface/state_FSM_FFd2-In99  (
    .I0(\serial_interface/SCL_delay [0]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/command [2]),
    .I3(N316),
    .O(\serial_interface/state_FSM_FFd2-In99_3368 )
  );
  LUT4 #(
    .INIT ( 16'h9555 ))
  \serial_interface/bit_count_mux0002<3>_SW1  (
    .I0(\serial_interface/bit_count [3]),
    .I1(\serial_interface/bit_count [0]),
    .I2(\serial_interface/bit_count [1]),
    .I3(\serial_interface/bit_count [2]),
    .O(N318)
  );
  LUT4 #(
    .INIT ( 16'h0018 ))
  \serial_interface/bit_count_mux0002<3>  (
    .I0(\serial_interface/state_FSM_FFd3_3369 ),
    .I1(\serial_interface/state_FSM_FFd4_3382 ),
    .I2(\serial_interface/state_FSM_FFd2_3359 ),
    .I3(N318),
    .O(\serial_interface/bit_count_mux0002 [3])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \LTC_B/LTC_frame_count/Mcount_sec_units_xor<2>1_SW1  (
    .I0(\LTC_B/LTC_frame_count/sec_units [0]),
    .I1(\LTC_B/LTC_frame_count/sec_units [2]),
    .I2(\LTC_B/LTC_frame_count/sec_units [1]),
    .O(N320)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_B/LTC_frame_count/Mcount_sec_units_xor<2>1  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I1(N320),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I3(LTC_b_secs[2]),
    .O(\LTC_B/LTC_frame_count/Mcount_sec_units2 )
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \LTC_B/LTC_frame_count/Mcount_sec_tens_xor<2>1_SW1  (
    .I0(\LTC_B/LTC_frame_count/sec_tens [0]),
    .I1(\LTC_B/LTC_frame_count/sec_tens [2]),
    .I2(\LTC_B/LTC_frame_count/sec_tens [1]),
    .O(N322)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_B/LTC_frame_count/Mcount_sec_tens_xor<2>1  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I1(N322),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I3(LTC_b_secs[6]),
    .O(\LTC_B/LTC_frame_count/Mcount_sec_tens2 )
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \LTC_B/LTC_frame_count/Mcount_min_units_xor<2>1_SW1  (
    .I0(\LTC_B/LTC_frame_count/min_units [0]),
    .I1(\LTC_B/LTC_frame_count/min_units [2]),
    .I2(\LTC_B/LTC_frame_count/min_units [1]),
    .O(N324)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_B/LTC_frame_count/Mcount_min_units_xor<2>1  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I1(N324),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I3(LTC_b_mins[2]),
    .O(\LTC_B/LTC_frame_count/Mcount_min_units2 )
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \LTC_B/LTC_frame_count/Mcount_min_tens_xor<2>1_SW1  (
    .I0(\LTC_B/LTC_frame_count/min_tens [0]),
    .I1(\LTC_B/LTC_frame_count/min_tens [2]),
    .I2(\LTC_B/LTC_frame_count/min_tens [1]),
    .O(N326)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_B/LTC_frame_count/Mcount_min_tens_xor<2>1  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I1(N326),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I3(LTC_b_mins[6]),
    .O(\LTC_B/LTC_frame_count/Mcount_min_tens2 )
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \LTC_A/LTC_frame_count/Mcount_sec_units_xor<2>1_SW1  (
    .I0(\LTC_A/LTC_frame_count/sec_units [0]),
    .I1(\LTC_A/LTC_frame_count/sec_units [2]),
    .I2(\LTC_A/LTC_frame_count/sec_units [1]),
    .O(N328)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_A/LTC_frame_count/Mcount_sec_units_xor<2>1  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I1(N328),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I3(LTC_a_secs[2]),
    .O(\LTC_A/LTC_frame_count/Mcount_sec_units2 )
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \LTC_A/LTC_frame_count/Mcount_sec_tens_xor<2>1_SW1  (
    .I0(\LTC_A/LTC_frame_count/sec_tens [0]),
    .I1(\LTC_A/LTC_frame_count/sec_tens [2]),
    .I2(\LTC_A/LTC_frame_count/sec_tens [1]),
    .O(N330)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_A/LTC_frame_count/Mcount_sec_tens_xor<2>1  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I1(N330),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I3(LTC_a_secs[6]),
    .O(\LTC_A/LTC_frame_count/Mcount_sec_tens2 )
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \LTC_A/LTC_frame_count/Mcount_min_units_xor<2>1_SW1  (
    .I0(\LTC_A/LTC_frame_count/min_units [0]),
    .I1(\LTC_A/LTC_frame_count/min_units [2]),
    .I2(\LTC_A/LTC_frame_count/min_units [1]),
    .O(N332)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_A/LTC_frame_count/Mcount_min_units_xor<2>1  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I1(N332),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I3(LTC_a_mins[2]),
    .O(\LTC_A/LTC_frame_count/Mcount_min_units2 )
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \LTC_A/LTC_frame_count/Mcount_min_tens_xor<2>1_SW1  (
    .I0(\LTC_A/LTC_frame_count/min_tens [0]),
    .I1(\LTC_A/LTC_frame_count/min_tens [2]),
    .I2(\LTC_A/LTC_frame_count/min_tens [1]),
    .O(N334)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_A/LTC_frame_count/Mcount_min_tens_xor<2>1  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I1(N334),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I3(LTC_a_mins[6]),
    .O(\LTC_A/LTC_frame_count/Mcount_min_tens2 )
  );
  LUT4 #(
    .INIT ( 16'h6AAA ))
  \LTC_B/LTC_frame_count/Mcount_sec_units_xor<3>1_SW1  (
    .I0(\LTC_B/LTC_frame_count/sec_units [3]),
    .I1(\LTC_B/LTC_frame_count/sec_units [0]),
    .I2(\LTC_B/LTC_frame_count/sec_units [1]),
    .I3(\LTC_B/LTC_frame_count/sec_units [2]),
    .O(N336)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_B/LTC_frame_count/Mcount_sec_units_xor<3>1  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I1(N336),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I3(LTC_b_secs[3]),
    .O(\LTC_B/LTC_frame_count/Mcount_sec_units3 )
  );
  LUT4 #(
    .INIT ( 16'h6AAA ))
  \LTC_B/LTC_frame_count/Mcount_min_units_xor<3>1_SW1  (
    .I0(\LTC_B/LTC_frame_count/min_units [3]),
    .I1(\LTC_B/LTC_frame_count/min_units [0]),
    .I2(\LTC_B/LTC_frame_count/min_units [1]),
    .I3(\LTC_B/LTC_frame_count/min_units [2]),
    .O(N338)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_B/LTC_frame_count/Mcount_min_units_xor<3>1  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I1(N338),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I3(LTC_b_mins[3]),
    .O(\LTC_B/LTC_frame_count/Mcount_min_units3 )
  );
  LUT4 #(
    .INIT ( 16'h6AAA ))
  \LTC_B/LTC_frame_count/Mcount_hour_units_xor<3>1_SW1  (
    .I0(\LTC_B/LTC_frame_count/hour_units [3]),
    .I1(\LTC_B/LTC_frame_count/hour_units [0]),
    .I2(\LTC_B/LTC_frame_count/hour_units [1]),
    .I3(\LTC_B/LTC_frame_count/hour_units [2]),
    .O(N340)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_B/LTC_frame_count/Mcount_hour_units_xor<3>1  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I1(N340),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I3(LTC_b_hours[3]),
    .O(\LTC_B/LTC_frame_count/Mcount_hour_units3 )
  );
  LUT4 #(
    .INIT ( 16'h6AAA ))
  \LTC_A/LTC_frame_count/Mcount_sec_units_xor<3>1_SW1  (
    .I0(\LTC_A/LTC_frame_count/sec_units [3]),
    .I1(\LTC_A/LTC_frame_count/sec_units [0]),
    .I2(\LTC_A/LTC_frame_count/sec_units [1]),
    .I3(\LTC_A/LTC_frame_count/sec_units [2]),
    .O(N342)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_A/LTC_frame_count/Mcount_sec_units_xor<3>1  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I1(N342),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I3(LTC_a_secs[3]),
    .O(\LTC_A/LTC_frame_count/Mcount_sec_units3 )
  );
  LUT4 #(
    .INIT ( 16'h6AAA ))
  \LTC_A/LTC_frame_count/Mcount_min_units_xor<3>1_SW1  (
    .I0(\LTC_A/LTC_frame_count/min_units [3]),
    .I1(\LTC_A/LTC_frame_count/min_units [0]),
    .I2(\LTC_A/LTC_frame_count/min_units [1]),
    .I3(\LTC_A/LTC_frame_count/min_units [2]),
    .O(N344)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_A/LTC_frame_count/Mcount_min_units_xor<3>1  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I1(N344),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I3(LTC_a_mins[3]),
    .O(\LTC_A/LTC_frame_count/Mcount_min_units3 )
  );
  LUT4 #(
    .INIT ( 16'h6AAA ))
  \LTC_A/LTC_frame_count/Mcount_hour_units_xor<3>1_SW1  (
    .I0(\LTC_A/LTC_frame_count/hour_units [3]),
    .I1(\LTC_A/LTC_frame_count/hour_units [0]),
    .I2(\LTC_A/LTC_frame_count/hour_units [1]),
    .I3(\LTC_A/LTC_frame_count/hour_units [2]),
    .O(N346)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_A/LTC_frame_count/Mcount_hour_units_xor<3>1  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I1(N346),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I3(LTC_a_hours[3]),
    .O(\LTC_A/LTC_frame_count/Mcount_hour_units3 )
  );
  LUT4 #(
    .INIT ( 16'hDF8A ))
  \OCXO_pll/phase_loop/dac_word_o<15>_SW1_SW1  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(genlock_register_22_7_2790),
    .I2(\system_control[4] ),
    .I3(\dac_OCXO/shift_reg [14]),
    .O(N249)
  );
  LUT4 #(
    .INIT ( 16'hDF8A ))
  \OCXO_pll/phase_loop/dac_word_o<13>_SW1_SW1  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(genlock_register_22_5_2788),
    .I2(\system_control[4] ),
    .I3(\dac_OCXO/shift_reg [12]),
    .O(N252)
  );
  LUT4 #(
    .INIT ( 16'hDF8A ))
  \OCXO_pll/phase_loop/dac_word_o<11>_SW1_SW1  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(genlock_register_22_3_2786),
    .I2(\system_control[4] ),
    .I3(\dac_OCXO/shift_reg [10]),
    .O(N255)
  );
  LUT4 #(
    .INIT ( 16'hDF8A ))
  \OCXO_pll/phase_loop/dac_word_o<7>_SW1_SW1  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(genlock_register_21_7_2781),
    .I2(\system_control[4] ),
    .I3(\dac_OCXO/shift_reg [6]),
    .O(N258)
  );
  LUT4 #(
    .INIT ( 16'hDF8A ))
  \OCXO_pll/phase_loop/dac_word_o<6>_SW1_SW1  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(genlock_register_21_6_2780),
    .I2(\system_control[4] ),
    .I3(\dac_OCXO/shift_reg [5]),
    .O(N2611)
  );
  LUT4 #(
    .INIT ( 16'hDF8A ))
  \OCXO_pll/phase_loop/dac_word_o<5>_SW1_SW1  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(genlock_register_21_5_2779),
    .I2(\system_control[4] ),
    .I3(\dac_OCXO/shift_reg [4]),
    .O(N264)
  );
  LUT4 #(
    .INIT ( 16'hDF8A ))
  \OCXO_pll/phase_loop/dac_word_o<4>_SW1_SW1  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(genlock_register_21_4_2778),
    .I2(\system_control[4] ),
    .I3(\dac_OCXO/shift_reg [3]),
    .O(N267)
  );
  LUT4 #(
    .INIT ( 16'hDF8A ))
  \OCXO_pll/phase_loop/dac_word_o<2>_SW1_SW1  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(genlock_register_21_2_2776),
    .I2(\system_control[4] ),
    .I3(\dac_OCXO/shift_reg [1]),
    .O(N270)
  );
  LUT4 #(
    .INIT ( 16'hE444 ))
  \OCXO_pll/phase_loop/dac_word_o<15>_SW1_SW0  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(\dac_OCXO/shift_reg [14]),
    .I2(\system_control[4] ),
    .I3(genlock_register_22_7_2790),
    .O(N248)
  );
  LUT4 #(
    .INIT ( 16'hE444 ))
  \OCXO_pll/phase_loop/dac_word_o<13>_SW1_SW0  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(\dac_OCXO/shift_reg [12]),
    .I2(\system_control[4] ),
    .I3(genlock_register_22_5_2788),
    .O(N251)
  );
  LUT4 #(
    .INIT ( 16'hE444 ))
  \OCXO_pll/phase_loop/dac_word_o<11>_SW1_SW0  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(\dac_OCXO/shift_reg [10]),
    .I2(\system_control[4] ),
    .I3(genlock_register_22_3_2786),
    .O(N254)
  );
  LUT4 #(
    .INIT ( 16'hE444 ))
  \OCXO_pll/phase_loop/dac_word_o<7>_SW1_SW0  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(\dac_OCXO/shift_reg [6]),
    .I2(\system_control[4] ),
    .I3(genlock_register_21_7_2781),
    .O(N257)
  );
  LUT4 #(
    .INIT ( 16'hE444 ))
  \OCXO_pll/phase_loop/dac_word_o<6>_SW1_SW0  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(\dac_OCXO/shift_reg [5]),
    .I2(\system_control[4] ),
    .I3(genlock_register_21_6_2780),
    .O(N260)
  );
  LUT4 #(
    .INIT ( 16'hE444 ))
  \OCXO_pll/phase_loop/dac_word_o<5>_SW1_SW0  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(\dac_OCXO/shift_reg [4]),
    .I2(\system_control[4] ),
    .I3(genlock_register_21_5_2779),
    .O(N263)
  );
  LUT4 #(
    .INIT ( 16'hE444 ))
  \OCXO_pll/phase_loop/dac_word_o<4>_SW1_SW0  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(\dac_OCXO/shift_reg [3]),
    .I2(\system_control[4] ),
    .I3(genlock_register_21_4_2778),
    .O(N266)
  );
  LUT4 #(
    .INIT ( 16'hE444 ))
  \OCXO_pll/phase_loop/dac_word_o<2>_SW1_SW0  (
    .I0(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I1(\dac_OCXO/shift_reg [1]),
    .I2(\system_control[4] ),
    .I3(genlock_register_21_2_2776),
    .O(N269)
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  I2C_byte_out_cmp_eq00261 (
    .I0(N14),
    .I1(\communication/address_o [5]),
    .I2(\communication/address_o [1]),
    .I3(\communication/address_o [0]),
    .O(I2C_byte_out_cmp_eq0026)
  );
  LUT3 #(
    .INIT ( 8'h2A ))
  \LTC_B/LTC_frame_count/min_tens_and00001  (
    .I0(\LTC_B/LTC_frame_count/N111 ),
    .I1(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .O(\LTC_B/LTC_frame_count/min_tens_and0000 )
  );
  LUT3 #(
    .INIT ( 8'h2A ))
  \LTC_A/LTC_frame_count/min_tens_and00001  (
    .I0(\LTC_A/LTC_frame_count/N111 ),
    .I1(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .O(\LTC_A/LTC_frame_count/min_tens_and0000 )
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \LTC_B/LTC_frame_count/hour_units_not00011  (
    .I0(\LTC_B/LTC_frame_count/N111 ),
    .I1(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .O(\LTC_B/LTC_frame_count/hour_units_not0001 )
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \LTC_A/LTC_frame_count/hour_units_not00011  (
    .I0(\LTC_A/LTC_frame_count/N111 ),
    .I1(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .O(\LTC_A/LTC_frame_count/hour_units_not0001 )
  );
  LUT3 #(
    .INIT ( 8'h2A ))
  \LTC_B/LTC_frame_count/min_units_and00001  (
    .I0(\LTC_B/LTC_frame_count/N9 ),
    .I1(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .O(\LTC_B/LTC_frame_count/min_units_and0000 )
  );
  LUT3 #(
    .INIT ( 8'h2A ))
  \LTC_A/LTC_frame_count/min_units_and00001  (
    .I0(\LTC_A/LTC_frame_count/N9 ),
    .I1(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .O(\LTC_A/LTC_frame_count/min_units_and0000 )
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \LTC_B/LTC_frame_count/min_tens_not00011  (
    .I0(\LTC_B/LTC_frame_count/N9 ),
    .I1(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .O(\LTC_B/LTC_frame_count/min_tens_not0001 )
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \LTC_A/LTC_frame_count/min_tens_not00011  (
    .I0(\LTC_A/LTC_frame_count/N9 ),
    .I1(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .O(\LTC_A/LTC_frame_count/min_tens_not0001 )
  );
  LUT4 #(
    .INIT ( 16'hFF80 ))
  \LTC_B/LTC_clockgenerator/ltc_bitcount_or00001  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .I1(\LTC_B/LTC_clockgenerator/ltc_bitcount_or000014_660 ),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount_or0000113_659 ),
    .I3(\LTC_B/delay_sync_983 ),
    .O(\LTC_B/LTC_clockgenerator/ltc_bitcount_or0000 )
  );
  LUT4 #(
    .INIT ( 16'hFF80 ))
  \LTC_A/LTC_clockgenerator/ltc_bitcount_or00001  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .I1(\LTC_A/LTC_clockgenerator/ltc_bitcount_or000014_226 ),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount_or0000113_225 ),
    .I3(\LTC_A/delay_sync_549 ),
    .O(\LTC_A/LTC_clockgenerator/ltc_bitcount_or0000 )
  );
  LUT3 #(
    .INIT ( 8'hD5 ))
  \LTC_B/LTC_clockgenerator/ltc_frame_tick_o_not00011  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .I1(\LTC_B/LTC_clockgenerator/ltc_bitcount_or000014_660 ),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount_or0000113_659 ),
    .O(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_not0001 )
  );
  LUT3 #(
    .INIT ( 8'hD5 ))
  \LTC_A/LTC_clockgenerator/ltc_frame_tick_o_not00011  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount_cmp_eq0000 ),
    .I1(\LTC_A/LTC_clockgenerator/ltc_bitcount_or000014_226 ),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount_or0000113_225 ),
    .O(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_not0001 )
  );
  LUT4 #(
    .INIT ( 16'h0888 ))
  \LTC_B/LTC_frame_count/sec_units_and00001  (
    .I0(\LTC_B/LTC_frame_count/N12 ),
    .I1(\LTC_B/LTC_frame_count/count_sec_740 ),
    .I2(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I3(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .O(\LTC_B/LTC_frame_count/sec_units_and0000 )
  );
  LUT4 #(
    .INIT ( 16'h0888 ))
  \LTC_A/LTC_frame_count/sec_units_and00001  (
    .I0(\LTC_A/LTC_frame_count/N12 ),
    .I1(\LTC_A/LTC_frame_count/count_sec_306 ),
    .I2(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I3(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .O(\LTC_A/LTC_frame_count/sec_units_and0000 )
  );
  LUT4 #(
    .INIT ( 16'h22A2 ))
  \serial_interface/state_FSM_FFd1-In118  (
    .I0(\serial_interface/state_FSM_FFd1_3351 ),
    .I1(N348),
    .I2(\serial_interface/state_FSM_FFd1-In89_3358 ),
    .I3(\serial_interface/state_cmp_eq0011 ),
    .O(\serial_interface/state_FSM_FFd1-In118_3353 )
  );
  LUT3 #(
    .INIT ( 8'hF7 ))
  \LTC_B/LTC_frame_count/hour_tens_not0002_SW1  (
    .I0(\LTC_B/LTC_frame_count/N111 ),
    .I1(\LTC_B/LTC_frame_count/hour_units [0]),
    .I2(\LTC_B/LTC_frame_count/hour_units [2]),
    .O(N350)
  );
  LUT4 #(
    .INIT ( 16'hF222 ))
  \LTC_B/LTC_frame_count/hour_tens_not0002  (
    .I0(\LTC_B/LTC_frame_count/N01 ),
    .I1(N350),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I3(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .O(\LTC_B/LTC_frame_count/hour_tens_not0002_784 )
  );
  LUT3 #(
    .INIT ( 8'hF7 ))
  \LTC_A/LTC_frame_count/hour_tens_not0002_SW1  (
    .I0(\LTC_A/LTC_frame_count/N111 ),
    .I1(\LTC_A/LTC_frame_count/hour_units [0]),
    .I2(\LTC_A/LTC_frame_count/hour_units [2]),
    .O(N352)
  );
  LUT4 #(
    .INIT ( 16'hF222 ))
  \LTC_A/LTC_frame_count/hour_tens_not0002  (
    .I0(\LTC_A/LTC_frame_count/N01 ),
    .I1(N352),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I3(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .O(\LTC_A/LTC_frame_count/hour_tens_not0002_350 )
  );
  LUT4 #(
    .INIT ( 16'h0010 ))
  \LTC_B/LTC_frame_count/frame_units_mux0009<0>38_SW0  (
    .I0(\LTC_setup[5] ),
    .I1(\LTC_B/LTC_frame_count/frame_tens [0]),
    .I2(\LTC_B/LTC_frame_count/frame_units [2]),
    .I3(\LTC_B/LTC_frame_count/frame_units_mux0009<0>9_764 ),
    .O(N284)
  );
  LUT4 #(
    .INIT ( 16'h0010 ))
  \LTC_A/LTC_frame_count/frame_units_mux0009<0>38_SW0  (
    .I0(\LTC_setup[1] ),
    .I1(\LTC_A/LTC_frame_count/frame_tens [0]),
    .I2(\LTC_A/LTC_frame_count/frame_units [2]),
    .I3(\LTC_A/LTC_frame_count/frame_units_mux0009<0>9_330 ),
    .O(N286)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_B/LTC_frame_count/frame_tens_mux0002<5>127  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I1(N354),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I3(LTC_b_frames[5]),
    .O(\LTC_B/LTC_frame_count/frame_tens_mux0002 [5])
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_A/LTC_frame_count/frame_tens_mux0002<5>127  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I1(N356),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I3(LTC_a_frames[5]),
    .O(\LTC_A/LTC_frame_count/frame_tens_mux0002 [5])
  );
  LUT4 #(
    .INIT ( 16'hBFFF ))
  \VCXO_pll/phase_detector/update_tick_not00011  (
    .I0(\VCXO_pll/phase_detector/phase_counter [8]),
    .I1(\VCXO_pll/phase_detector/phase_counter [9]),
    .I2(\VCXO_pll/phase_detector/phase_counter_cmp_eq00007_2277 ),
    .I3(\VCXO_pll/phase_detector/phase_counter_cmp_eq000018_2276 ),
    .O(\VCXO_pll/phase_detector/update_tick_not0001 )
  );
  LUT4 #(
    .INIT ( 16'h0444 ))
  \LTC_B/LTC_frame_count/Mcount_hour_units_xor<2>111  (
    .I0(\LTC_B/LTC_frame_count/hour_units [2]),
    .I1(\LTC_B/LTC_frame_count/hour_units [0]),
    .I2(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I3(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .O(\LTC_B/LTC_frame_count/N13 )
  );
  LUT4 #(
    .INIT ( 16'h0444 ))
  \LTC_A/LTC_frame_count/Mcount_hour_units_xor<2>111  (
    .I0(\LTC_A/LTC_frame_count/hour_units [2]),
    .I1(\LTC_A/LTC_frame_count/hour_units [0]),
    .I2(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I3(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .O(\LTC_A/LTC_frame_count/N13 )
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \communication/Mcount_address_lut<0>  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\serial_interface/recieved_byte_o [0]),
    .I3(\communication/address [0]),
    .O(\communication/Mcount_address_lut [0])
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \communication/Mcount_address_lut<1>  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\serial_interface/recieved_byte_o [1]),
    .I3(\communication/address [1]),
    .O(\communication/Mcount_address_lut [1])
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \communication/Mcount_address_lut<2>  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\serial_interface/recieved_byte_o [2]),
    .I3(\communication/address [2]),
    .O(\communication/Mcount_address_lut [2])
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \communication/Mcount_address_lut<3>  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\serial_interface/recieved_byte_o [3]),
    .I3(\communication/address [3]),
    .O(\communication/Mcount_address_lut [3])
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \communication/Mcount_address_lut<4>  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\serial_interface/recieved_byte_o [4]),
    .I3(\communication/address [4]),
    .O(\communication/Mcount_address_lut [4])
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \communication/Mcount_address_lut<5>  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\serial_interface/recieved_byte_o [5]),
    .I3(\communication/address [5]),
    .O(\communication/Mcount_address_lut [5])
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \communication/Mcount_address_lut<6>  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\serial_interface/recieved_byte_o [6]),
    .I3(\communication/address [6]),
    .O(\communication/Mcount_address_lut [6])
  );
  LUT4 #(
    .INIT ( 16'hFD20 ))
  \communication/Mcount_address_lut<7>  (
    .I0(\communication/state_FSM_FFd2_2370 ),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\serial_interface/recieved_byte_o [7]),
    .I3(\communication/address [7]),
    .O(\communication/Mcount_address_lut [7])
  );
  LUT4 #(
    .INIT ( 16'h5BF9 ))
  \serial_interface/time_out_mux0009<0>11  (
    .I0(\serial_interface/state_FSM_FFd3_3369 ),
    .I1(\serial_interface/state_FSM_FFd1_3351 ),
    .I2(\serial_interface/state_FSM_FFd4_3382 ),
    .I3(\serial_interface/state_FSM_FFd2_3359 ),
    .O(\serial_interface/time_out_mux0009<0>1 )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \serial_interface/state_FSM_FFd1-In27_SW0  (
    .I0(\serial_interface/SCL_delay [0]),
    .I1(\serial_interface/SCL_delay [1]),
    .I2(\serial_interface/command [0]),
    .I3(\serial_interface/state_cmp_eq0010_3399 ),
    .O(N310)
  );
  LUT4 #(
    .INIT ( 16'h0010 ))
  I2C_byte_out_cmp_eq00001 (
    .I0(\communication/address_o [5]),
    .I1(\communication/address_o [0]),
    .I2(N14),
    .I3(\communication/address_o [1]),
    .O(I2C_byte_out_cmp_eq0000)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_B/LTC_frame_count/frame_units_mux0009<1>115  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I1(\LTC_B/LTC_frame_count/frame_units_mux0009<1>86_769 ),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I3(LTC_b_frames[1]),
    .O(\LTC_B/LTC_frame_count/frame_units_mux0009 [1])
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_A/LTC_frame_count/frame_units_mux0009<1>115  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I1(\LTC_A/LTC_frame_count/frame_units_mux0009<1>86_335 ),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I3(LTC_a_frames[1]),
    .O(\LTC_A/LTC_frame_count/frame_units_mux0009 [1])
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_B/LTC_frame_count/frame_units_mux0009<2>113  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I1(\LTC_B/LTC_frame_count/frame_units_mux0009<2>84_774 ),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I3(LTC_b_frames[2]),
    .O(\LTC_B/LTC_frame_count/frame_units_mux0009 [2])
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_A/LTC_frame_count/frame_units_mux0009<2>113  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I1(\LTC_A/LTC_frame_count/frame_units_mux0009<2>84_340 ),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I3(LTC_a_frames[2]),
    .O(\LTC_A/LTC_frame_count/frame_units_mux0009 [2])
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \LTC_B/LTC_frame_count/Mcount_hour_units_xor<2>1_SW0  (
    .I0(\LTC_B/LTC_frame_count/hour_units [0]),
    .I1(\LTC_B/LTC_frame_count/hour_units [2]),
    .I2(\LTC_B/LTC_frame_count/hour_units [1]),
    .O(N358)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_B/LTC_frame_count/Mcount_hour_units_xor<2>1  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .I1(N358),
    .I2(\LTC_B/LTC_clockgenerator/ltc_sync_o_665 ),
    .I3(LTC_b_hours[2]),
    .O(\LTC_B/LTC_frame_count/Mcount_hour_units2 )
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \LTC_A/LTC_frame_count/Mcount_hour_units_xor<2>1_SW0  (
    .I0(\LTC_A/LTC_frame_count/hour_units [0]),
    .I1(\LTC_A/LTC_frame_count/hour_units [2]),
    .I2(\LTC_A/LTC_frame_count/hour_units [1]),
    .O(N360)
  );
  LUT4 #(
    .INIT ( 16'hEC4C ))
  \LTC_A/LTC_frame_count/Mcount_hour_units_xor<2>1  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .I1(N360),
    .I2(\LTC_A/LTC_clockgenerator/ltc_sync_o_231 ),
    .I3(LTC_a_hours[2]),
    .O(\LTC_A/LTC_frame_count/Mcount_hour_units2 )
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  \serial_interface/send_byte_mux0001<6>2031  (
    .I0(\serial_interface/send_byte_mux0001<6>181_3316 ),
    .I1(\communication/address_o [7]),
    .I2(N2),
    .I3(\communication/address_o [6]),
    .O(\serial_interface/send_byte_mux0001<6>203 )
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  \serial_interface/send_byte_mux0001<4>2151  (
    .I0(\serial_interface/send_byte_mux0001<4>193_3274 ),
    .I1(\communication/address_o [7]),
    .I2(N2),
    .I3(\communication/address_o [6]),
    .O(\serial_interface/send_byte_mux0001<4>215 )
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  \serial_interface/send_byte_mux0001<2>2031  (
    .I0(\serial_interface/send_byte_mux0001<2>181_3230 ),
    .I1(\communication/address_o [7]),
    .I2(N2),
    .I3(\communication/address_o [6]),
    .O(\serial_interface/send_byte_mux0001<2>203 )
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  \serial_interface/send_byte_mux0001<0>2071  (
    .I0(\serial_interface/send_byte_mux0001<0>185_3188 ),
    .I1(\communication/address_o [7]),
    .I2(N2),
    .I3(\communication/address_o [6]),
    .O(\serial_interface/send_byte_mux0001<0>207 )
  );
  LUT4 #(
    .INIT ( 16'hB111 ))
  \serial_interface/state_FSM_FFd2-In16_SW0  (
    .I0(\serial_interface/SCL_delay [1]),
    .I1(\serial_interface/state_cmp_eq0011 ),
    .I2(\serial_interface/bit_count [3]),
    .I3(N87),
    .O(N362)
  );
  LUT4 #(
    .INIT ( 16'h4EFF ))
  \serial_interface/state_FSM_FFd2-In16  (
    .I0(\serial_interface/SCL_delay [0]),
    .I1(N362),
    .I2(\serial_interface/state_cmp_eq0011 ),
    .I3(\serial_interface/state_FSM_FFd2_3359 ),
    .O(\serial_interface/state_FSM_FFd2-In16_3363 )
  );
  LUT4 #(
    .INIT ( 16'hDFBB ))
  \serial_interface/send_byte_mux0001<0>139_SW0  (
    .I0(\communication/address_o [0]),
    .I1(\communication/address_o [5]),
    .I2(genlock_register_9_0_2855),
    .I3(\communication/address_o [3]),
    .O(N364)
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \serial_interface/send_byte_mux0001<0>139  (
    .I0(\communication/address_o [1]),
    .I1(\communication/address_o [2]),
    .I2(\communication/address_o [4]),
    .I3(N364),
    .O(\serial_interface/send_byte_mux0001<0>139_3186 )
  );
  LUT2 #(
    .INIT ( 4'h4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_limit1011_1  (
    .I0(\OCXO_pll/phase_loop/integrator [35]),
    .I1(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16]),
    .O(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1758 )
  );
  LUT2 #(
    .INIT ( 4'h4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_limit1011_2  (
    .I0(\OCXO_pll/phase_loop/integrator [35]),
    .I1(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16]),
    .O(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_1_1759 )
  );
  LUT2 #(
    .INIT ( 4'h4 ))
  \OCXO_pll/phase_loop/Mmux_integrator_limit1011_3  (
    .I0(\OCXO_pll/phase_loop/integrator [35]),
    .I1(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_cy [16]),
    .O(\OCXO_pll/phase_loop/Mmux_integrator_limit1011_2_1760 )
  );
  MUXF5   \LTC_B/LTC_frame_count/frame_units_mux0009<2>84  (
    .I0(N366),
    .I1(N367),
    .S(\LTC_B/LTC_frame_count/frame_units_mux0009<2>19_773 ),
    .O(\LTC_B/LTC_frame_count/frame_units_mux0009<2>84_774 )
  );
  LUT4 #(
    .INIT ( 16'h682A ))
  \LTC_B/LTC_frame_count/frame_units_mux0009<2>84_F  (
    .I0(\LTC_B/LTC_frame_count/frame_units [2]),
    .I1(\LTC_B/LTC_frame_count/frame_units [0]),
    .I2(\LTC_B/LTC_frame_count/frame_units [1]),
    .I3(\LTC_setup[4] ),
    .O(N366)
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \LTC_B/LTC_frame_count/frame_units_mux0009<2>84_G  (
    .I0(\LTC_B/LTC_frame_count/frame_units [1]),
    .I1(\LTC_B/LTC_frame_count/frame_units [2]),
    .I2(\LTC_B/LTC_frame_count/frame_units [0]),
    .O(N367)
  );
  MUXF5   \LTC_A/LTC_frame_count/frame_units_mux0009<2>84  (
    .I0(N368),
    .I1(N369),
    .S(\LTC_A/LTC_frame_count/frame_units_mux0009<2>19_339 ),
    .O(\LTC_A/LTC_frame_count/frame_units_mux0009<2>84_340 )
  );
  LUT4 #(
    .INIT ( 16'h682A ))
  \LTC_A/LTC_frame_count/frame_units_mux0009<2>84_F  (
    .I0(\LTC_A/LTC_frame_count/frame_units [2]),
    .I1(\LTC_A/LTC_frame_count/frame_units [0]),
    .I2(\LTC_A/LTC_frame_count/frame_units [1]),
    .I3(\LTC_setup[0] ),
    .O(N368)
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \LTC_A/LTC_frame_count/frame_units_mux0009<2>84_G  (
    .I0(\LTC_A/LTC_frame_count/frame_units [1]),
    .I1(\LTC_A/LTC_frame_count/frame_units [2]),
    .I2(\LTC_A/LTC_frame_count/frame_units [0]),
    .O(N369)
  );
  MUXF5   I2C_byte_out_or0000 (
    .I0(N370),
    .I1(N371),
    .S(\communication/address_o [5]),
    .O(I2C_byte_out_or0000_27)
  );
  LUT4 #(
    .INIT ( 16'hA888 ))
  I2C_byte_out_or0000_F (
    .I0(\communication/address_o [4]),
    .I1(\communication/address_o [3]),
    .I2(\communication/address_o [2]),
    .I3(N93),
    .O(N370)
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  I2C_byte_out_or0000_G (
    .I0(\communication/address_o [3]),
    .I1(\communication/address_o [2]),
    .I2(N26),
    .I3(\communication/address_o [4]),
    .O(N371)
  );
  MUXF5   \serial_interface/SDA_out_mux000385  (
    .I0(N372),
    .I1(N373),
    .S(\serial_interface/bit_count [1]),
    .O(\serial_interface/SDA_out_mux000385_3125 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \serial_interface/SDA_out_mux000385_F  (
    .I0(\serial_interface/bit_count [2]),
    .I1(\serial_interface/send_byte [7]),
    .I2(\serial_interface/send_byte [3]),
    .O(N372)
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \serial_interface/SDA_out_mux000385_G  (
    .I0(\serial_interface/bit_count [2]),
    .I1(\serial_interface/send_byte [5]),
    .I2(\serial_interface/send_byte [1]),
    .O(N373)
  );
  MUXF5   \LTC_B/LTC_biphase_generator/Madd_ltc_frame_i_not0000<0>1  (
    .I0(N374),
    .I1(N375),
    .S(\LTC_B/LTC_clockgenerator/ltc_bitcount [1]),
    .O(\LTC_B/LTC_biphase_generator/Madd_ltc_frame_i_not0000<0>1_552 )
  );
  LUT4 #(
    .INIT ( 16'h2F23 ))
  \LTC_B/LTC_biphase_generator/Madd_ltc_frame_i_not0000<0>1_F  (
    .I0(\LTC_B/LTC_frame_count/min_tens_o [0]),
    .I1(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount [6]),
    .I3(\LTC_B/LTC_frame_count/sec_tens_o [0]),
    .O(N374)
  );
  LUT2 #(
    .INIT ( 4'h1 ))
  \LTC_B/LTC_biphase_generator/Madd_ltc_frame_i_not0000<0>1_G  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_B/LTC_clockgenerator/ltc_bitcount [6]),
    .O(N375)
  );
  MUXF5   \LTC_A/LTC_biphase_generator/Madd_ltc_frame_i_not0000<0>1  (
    .I0(N376),
    .I1(N377),
    .S(\LTC_A/LTC_clockgenerator/ltc_bitcount [1]),
    .O(\LTC_A/LTC_biphase_generator/Madd_ltc_frame_i_not0000<0>1_118 )
  );
  LUT4 #(
    .INIT ( 16'h2F23 ))
  \LTC_A/LTC_biphase_generator/Madd_ltc_frame_i_not0000<0>1_F  (
    .I0(\LTC_A/LTC_frame_count/min_tens_o [0]),
    .I1(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount [6]),
    .I3(\LTC_A/LTC_frame_count/sec_tens_o [0]),
    .O(N376)
  );
  LUT2 #(
    .INIT ( 4'h1 ))
  \LTC_A/LTC_biphase_generator/Madd_ltc_frame_i_not0000<0>1_G  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_A/LTC_clockgenerator/ltc_bitcount [6]),
    .O(N377)
  );
  MUXF5   \serial_interface/recieve_byte_6_mux0001  (
    .I0(N378),
    .I1(N379),
    .S(\serial_interface/recieve_byte_6_cmp_eq0000 ),
    .O(\serial_interface/recieve_byte_6_mux0001_3162 )
  );
  LUT4 #(
    .INIT ( 16'hAA20 ))
  \serial_interface/recieve_byte_6_mux0001_F  (
    .I0(\serial_interface/recieve_byte [6]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd2_3359 ),
    .I3(N64),
    .O(N378)
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \serial_interface/recieve_byte_6_mux0001_G  (
    .I0(N15),
    .I1(\serial_interface/recieve_byte [6]),
    .I2(N64),
    .O(N379)
  );
  MUXF5   \serial_interface/command_mux0000<1>  (
    .I0(N380),
    .I1(N381),
    .S(\serial_interface/command [1]),
    .O(\serial_interface/command_mux0000 [1])
  );
  LUT2 #(
    .INIT ( 4'h8 ))
  \serial_interface/command_mux0000<1>_F  (
    .I0(\serial_interface/state_FSM_FFd1_3351 ),
    .I1(\serial_interface/state_FSM_FFd4_3382 ),
    .O(N380)
  );
  LUT4 #(
    .INIT ( 16'hFEF6 ))
  \serial_interface/command_mux0000<1>_G  (
    .I0(\serial_interface/state_FSM_FFd1_3351 ),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd2_3359 ),
    .I3(\serial_interface/state_FSM_FFd4_3382 ),
    .O(N381)
  );
  MUXF5   \Inst_ad5660_serial_DAC/bit_count_mux0000<2>  (
    .I0(N382),
    .I1(N383),
    .S(\Inst_ad5660_serial_DAC/state_FSM_FFd2_105 ),
    .O(\Inst_ad5660_serial_DAC/bit_count_mux0000 [2])
  );
  LUT3 #(
    .INIT ( 8'h04 ))
  \Inst_ad5660_serial_DAC/bit_count_mux0000<2>_F  (
    .I0(\Inst_ad5660_serial_DAC/state_FSM_FFd5_113 ),
    .I1(\Inst_ad5660_serial_DAC/bit_count [2]),
    .I2(\Inst_ad5660_serial_DAC/state_FSM_FFd4_111 ),
    .O(N382)
  );
  LUT3 #(
    .INIT ( 8'h6C ))
  \Inst_ad5660_serial_DAC/bit_count_mux0000<2>_G  (
    .I0(\Inst_ad5660_serial_DAC/bit_count [1]),
    .I1(\Inst_ad5660_serial_DAC/bit_count [2]),
    .I2(\Inst_ad5660_serial_DAC/bit_count [0]),
    .O(N383)
  );
  MUXF5   \serial_interface/recieve_byte_0_mux0001  (
    .I0(N384),
    .I1(N385),
    .S(\serial_interface/state_cmp_eq0007 ),
    .O(\serial_interface/recieve_byte_0_mux0001_3149 )
  );
  LUT4 #(
    .INIT ( 16'hAA20 ))
  \serial_interface/recieve_byte_0_mux0001_F  (
    .I0(\serial_interface/recieve_byte [0]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd2_3359 ),
    .I3(N64),
    .O(N384)
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \serial_interface/recieve_byte_0_mux0001_G  (
    .I0(N15),
    .I1(\serial_interface/recieve_byte [0]),
    .I2(N64),
    .O(N385)
  );
  MUXF5   \serial_interface/state_FSM_FFd3-In193  (
    .I0(N386),
    .I1(N387),
    .S(\serial_interface/state_FSM_FFd3-In88_3381 ),
    .O(\serial_interface/state_FSM_FFd3-In193_3376 )
  );
  LUT4 #(
    .INIT ( 16'hDC10 ))
  \serial_interface/state_FSM_FFd3-In193_F  (
    .I0(\serial_interface/state_FSM_FFd3_3369 ),
    .I1(\serial_interface/state_FSM_FFd4_3382 ),
    .I2(\serial_interface/state_FSM_FFd3-In169_3375 ),
    .I3(\serial_interface/state_FSM_FFd3-In129_3371 ),
    .O(N386)
  );
  LUT4 #(
    .INIT ( 16'hFFE2 ))
  \serial_interface/state_FSM_FFd3-In193_G  (
    .I0(\serial_interface/state_FSM_FFd3-In169_3375 ),
    .I1(\serial_interface/state_FSM_FFd4_3382 ),
    .I2(\serial_interface/state_FSM_FFd3-In129_3371 ),
    .I3(\serial_interface/state_FSM_FFd3_3369 ),
    .O(N387)
  );
  MUXF5   \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_8_f5  (
    .I0(N388),
    .I1(N389),
    .S(\LTC_B/LTC_clockgenerator/ltc_bitcount [3]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_8_f5_581 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_8_f5_F  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [2]),
    .I1(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_f6_557 ),
    .I2(\LTC_B/LTC_biphase_generator/Madd_ltc_frame_i_not0000<0>1_552 ),
    .O(N388)
  );
  LUT4 #(
    .INIT ( 16'h1011 ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_8_f5_G  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_B/LTC_clockgenerator/ltc_bitcount [6]),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount [1]),
    .I3(\LTC_B/LTC_clockgenerator/ltc_bitcount [2]),
    .O(N389)
  );
  MUXF5   \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_8_f5  (
    .I0(N390),
    .I1(N391),
    .S(\LTC_A/LTC_clockgenerator/ltc_bitcount [3]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_8_f5_147 )
  );
  LUT3 #(
    .INIT ( 8'hE4 ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_8_f5_F  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [2]),
    .I1(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_f6_123 ),
    .I2(\LTC_A/LTC_biphase_generator/Madd_ltc_frame_i_not0000<0>1_118 ),
    .O(N390)
  );
  LUT4 #(
    .INIT ( 16'h1011 ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_8_f5_G  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [5]),
    .I1(\LTC_A/LTC_clockgenerator/ltc_bitcount [6]),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount [1]),
    .I3(\LTC_A/LTC_clockgenerator/ltc_bitcount [2]),
    .O(N391)
  );
  MUXF5   \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_6_f5  (
    .I0(N392),
    .I1(N393),
    .S(\LTC_B/LTC_clockgenerator/ltc_bitcount [2]),
    .O(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_6_f5_580 )
  );
  LUT4 #(
    .INIT ( 16'h0E04 ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_6_f5_F  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [4]),
    .I1(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_555 ),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount [3]),
    .I3(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_559 ),
    .O(N392)
  );
  LUT4 #(
    .INIT ( 16'h0E04 ))
  \LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_6_f5_G  (
    .I0(\LTC_B/LTC_clockgenerator/ltc_bitcount [4]),
    .I1(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_12_554 ),
    .I2(\LTC_B/LTC_clockgenerator/ltc_bitcount [3]),
    .I3(\LTC_B/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_131_556 ),
    .O(N393)
  );
  MUXF5   \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_6_f5  (
    .I0(N394),
    .I1(N395),
    .S(\LTC_A/LTC_clockgenerator/ltc_bitcount [2]),
    .O(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_6_f5_146 )
  );
  LUT4 #(
    .INIT ( 16'h0E04 ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_6_f5_F  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [4]),
    .I1(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_13_121 ),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount [3]),
    .I3(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_14_125 ),
    .O(N394)
  );
  LUT4 #(
    .INIT ( 16'h0E04 ))
  \LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_6_f5_G  (
    .I0(\LTC_A/LTC_clockgenerator/ltc_bitcount [4]),
    .I1(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_12_120 ),
    .I2(\LTC_A/LTC_clockgenerator/ltc_bitcount [3]),
    .I3(\LTC_A/LTC_biphase_generator/Mmux_ltc_frame_i_mux0000_131_122 ),
    .O(N395)
  );
  BUFG   clk_148_BUFG (
    .I(clk_1481),
    .O(clk_148)
  );
  BUFG   clk_10_i_IBUFG_BUFG (
    .I(clk_10_i_IBUFG1),
    .O(clk_10_i_IBUFG_2312)
  );
  INV   \Mcompar_pps_status_cmp_gt0000_lut<1>_INV_0  (
    .I(pps_status_count[4]),
    .O(\Mcompar_pps_status_cmp_gt0000_lut[1] )
  );
  INV   \Mcompar_pps_status_cmp_gt0000_lut<3>_INV_0  (
    .I(pps_status_count[6]),
    .O(\Mcompar_pps_status_cmp_gt0000_lut[3] )
  );
  INV   \Mcompar_pps_status_cmp_gt0000_lut<5>_INV_0  (
    .I(pps_status_count[11]),
    .O(\Mcompar_pps_status_cmp_gt0000_lut[5] )
  );
  INV   \Mcompar_pps_status_cmp_gt0000_lut<10>_INV_0  (
    .I(pps_status_count[22]),
    .O(\Mcompar_pps_status_cmp_gt0000_lut[10] )
  );
  INV   \Mcompar_pps_status_cmp_gt0000_cy<11>_inv_INV_0  (
    .I(Mcompar_pps_status_cmp_gt0000_cy[11]),
    .O(pps_status_cmp_gt0000)
  );
  INV   \f_gen/Msub_count_addsub0000_lut<1>_INV_0  (
    .I(\f_gen/count [1]),
    .O(\f_gen/Msub_count_addsub0000_lut [1])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<2>_INV_0  (
    .I(\f_gen/count [2]),
    .O(\f_gen/Msub_count_addsub0000_lut [2])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<3>_INV_0  (
    .I(\f_gen/count [3]),
    .O(\f_gen/Msub_count_addsub0000_lut [3])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<4>_INV_0  (
    .I(\f_gen/count [4]),
    .O(\f_gen/Msub_count_addsub0000_lut [4])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<5>_INV_0  (
    .I(\f_gen/count [5]),
    .O(\f_gen/Msub_count_addsub0000_lut [5])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<6>_INV_0  (
    .I(\f_gen/count [6]),
    .O(\f_gen/Msub_count_addsub0000_lut [6])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<7>_INV_0  (
    .I(\f_gen/count [7]),
    .O(\f_gen/Msub_count_addsub0000_lut [7])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<8>_INV_0  (
    .I(\f_gen/count [8]),
    .O(\f_gen/Msub_count_addsub0000_lut [8])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<9>_INV_0  (
    .I(\f_gen/count [9]),
    .O(\f_gen/Msub_count_addsub0000_lut [9])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<10>_INV_0  (
    .I(\f_gen/count [10]),
    .O(\f_gen/Msub_count_addsub0000_lut [10])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<11>_INV_0  (
    .I(\f_gen/count [11]),
    .O(\f_gen/Msub_count_addsub0000_lut [11])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<12>_INV_0  (
    .I(\f_gen/count [12]),
    .O(\f_gen/Msub_count_addsub0000_lut [12])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<13>_INV_0  (
    .I(\f_gen/count [13]),
    .O(\f_gen/Msub_count_addsub0000_lut [13])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<14>_INV_0  (
    .I(\f_gen/count [14]),
    .O(\f_gen/Msub_count_addsub0000_lut [14])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<15>_INV_0  (
    .I(\f_gen/count [15]),
    .O(\f_gen/Msub_count_addsub0000_lut [15])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<16>_INV_0  (
    .I(\f_gen/count [16]),
    .O(\f_gen/Msub_count_addsub0000_lut [16])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<17>_INV_0  (
    .I(\f_gen/count [17]),
    .O(\f_gen/Msub_count_addsub0000_lut [17])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<18>_INV_0  (
    .I(\f_gen/count [18]),
    .O(\f_gen/Msub_count_addsub0000_lut [18])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<19>_INV_0  (
    .I(\f_gen/count [19]),
    .O(\f_gen/Msub_count_addsub0000_lut [19])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<20>_INV_0  (
    .I(\f_gen/count [20]),
    .O(\f_gen/Msub_count_addsub0000_lut [20])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<21>_INV_0  (
    .I(\f_gen/count [21]),
    .O(\f_gen/Msub_count_addsub0000_lut [21])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<22>_INV_0  (
    .I(\f_gen/count [22]),
    .O(\f_gen/Msub_count_addsub0000_lut [22])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<23>_INV_0  (
    .I(\f_gen/count [23]),
    .O(\f_gen/Msub_count_addsub0000_lut [23])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<24>_INV_0  (
    .I(\f_gen/count [24]),
    .O(\f_gen/Msub_count_addsub0000_lut [24])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<25>_INV_0  (
    .I(\f_gen/count [25]),
    .O(\f_gen/Msub_count_addsub0000_lut [25])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<26>_INV_0  (
    .I(\f_gen/count [26]),
    .O(\f_gen/Msub_count_addsub0000_lut [26])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<27>_INV_0  (
    .I(\f_gen/count [27]),
    .O(\f_gen/Msub_count_addsub0000_lut [27])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<28>_INV_0  (
    .I(\f_gen/count [28]),
    .O(\f_gen/Msub_count_addsub0000_lut [28])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<29>_INV_0  (
    .I(\f_gen/count [29]),
    .O(\f_gen/Msub_count_addsub0000_lut [29])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<30>_INV_0  (
    .I(\f_gen/count [30]),
    .O(\f_gen/Msub_count_addsub0000_lut [30])
  );
  INV   \f_gen/Msub_count_addsub0000_lut<31>_INV_0  (
    .I(\f_gen/count [31]),
    .O(\f_gen/Msub_count_addsub0000_lut [31])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<1>_INV_0  (
    .I(\h_gen/count [1]),
    .O(\h_gen/Msub_count_addsub0000_lut [1])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<2>_INV_0  (
    .I(\h_gen/count [2]),
    .O(\h_gen/Msub_count_addsub0000_lut [2])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<3>_INV_0  (
    .I(\h_gen/count [3]),
    .O(\h_gen/Msub_count_addsub0000_lut [3])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<4>_INV_0  (
    .I(\h_gen/count [4]),
    .O(\h_gen/Msub_count_addsub0000_lut [4])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<5>_INV_0  (
    .I(\h_gen/count [5]),
    .O(\h_gen/Msub_count_addsub0000_lut [5])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<6>_INV_0  (
    .I(\h_gen/count [6]),
    .O(\h_gen/Msub_count_addsub0000_lut [6])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<7>_INV_0  (
    .I(\h_gen/count [7]),
    .O(\h_gen/Msub_count_addsub0000_lut [7])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<8>_INV_0  (
    .I(\h_gen/count [8]),
    .O(\h_gen/Msub_count_addsub0000_lut [8])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<9>_INV_0  (
    .I(\h_gen/count [9]),
    .O(\h_gen/Msub_count_addsub0000_lut [9])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<10>_INV_0  (
    .I(\h_gen/count [10]),
    .O(\h_gen/Msub_count_addsub0000_lut [10])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<11>_INV_0  (
    .I(\h_gen/count [11]),
    .O(\h_gen/Msub_count_addsub0000_lut [11])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<12>_INV_0  (
    .I(\h_gen/count [12]),
    .O(\h_gen/Msub_count_addsub0000_lut [12])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<13>_INV_0  (
    .I(\h_gen/count [13]),
    .O(\h_gen/Msub_count_addsub0000_lut [13])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<14>_INV_0  (
    .I(\h_gen/count [14]),
    .O(\h_gen/Msub_count_addsub0000_lut [14])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<15>_INV_0  (
    .I(\h_gen/count [15]),
    .O(\h_gen/Msub_count_addsub0000_lut [15])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<16>_INV_0  (
    .I(\h_gen/count [16]),
    .O(\h_gen/Msub_count_addsub0000_lut [16])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<17>_INV_0  (
    .I(\h_gen/count [17]),
    .O(\h_gen/Msub_count_addsub0000_lut [17])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<18>_INV_0  (
    .I(\h_gen/count [18]),
    .O(\h_gen/Msub_count_addsub0000_lut [18])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<19>_INV_0  (
    .I(\h_gen/count [19]),
    .O(\h_gen/Msub_count_addsub0000_lut [19])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<20>_INV_0  (
    .I(\h_gen/count [20]),
    .O(\h_gen/Msub_count_addsub0000_lut [20])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<21>_INV_0  (
    .I(\h_gen/count [21]),
    .O(\h_gen/Msub_count_addsub0000_lut [21])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<22>_INV_0  (
    .I(\h_gen/count [22]),
    .O(\h_gen/Msub_count_addsub0000_lut [22])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<23>_INV_0  (
    .I(\h_gen/count [23]),
    .O(\h_gen/Msub_count_addsub0000_lut [23])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<24>_INV_0  (
    .I(\h_gen/count [24]),
    .O(\h_gen/Msub_count_addsub0000_lut [24])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<25>_INV_0  (
    .I(\h_gen/count [25]),
    .O(\h_gen/Msub_count_addsub0000_lut [25])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<26>_INV_0  (
    .I(\h_gen/count [26]),
    .O(\h_gen/Msub_count_addsub0000_lut [26])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<27>_INV_0  (
    .I(\h_gen/count [27]),
    .O(\h_gen/Msub_count_addsub0000_lut [27])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<28>_INV_0  (
    .I(\h_gen/count [28]),
    .O(\h_gen/Msub_count_addsub0000_lut [28])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<29>_INV_0  (
    .I(\h_gen/count [29]),
    .O(\h_gen/Msub_count_addsub0000_lut [29])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<30>_INV_0  (
    .I(\h_gen/count [30]),
    .O(\h_gen/Msub_count_addsub0000_lut [30])
  );
  INV   \h_gen/Msub_count_addsub0000_lut<31>_INV_0  (
    .I(\h_gen/count [31]),
    .O(\h_gen/Msub_count_addsub0000_lut [31])
  );
  INV   \Mcount_pps_status_count_lut<0>_INV_0  (
    .I(pps_status_count[0]),
    .O(Mcount_pps_status_count_lut[0])
  );
  INV   \Mcount_count_lut<1>_INV_0  (
    .I(count[1]),
    .O(Mcount_count_lut[1])
  );
  INV   \Mcount_count_lut<2>_INV_0  (
    .I(count[2]),
    .O(Mcount_count_lut[2])
  );
  INV   \Mcount_count_lut<3>_INV_0  (
    .I(count[3]),
    .O(Mcount_count_lut[3])
  );
  INV   \Mcount_count_lut<4>_INV_0  (
    .I(count[4]),
    .O(Mcount_count_lut[4])
  );
  INV   \Mcount_count_lut<5>_INV_0  (
    .I(count[5]),
    .O(Mcount_count_lut[5])
  );
  INV   \Mcount_count_lut<6>_INV_0  (
    .I(count[6]),
    .O(Mcount_count_lut[6])
  );
  INV   \Mcount_count_lut<7>_INV_0  (
    .I(count[7]),
    .O(Mcount_count_lut[7])
  );
  INV   \Mcount_count_lut<8>_INV_0  (
    .I(count[8]),
    .O(Mcount_count_lut[8])
  );
  INV   \Mcount_count_lut<9>_INV_0  (
    .I(count[9]),
    .O(Mcount_count_lut[9])
  );
  INV   \Mcount_count_lut<10>_INV_0  (
    .I(count[10]),
    .O(Mcount_count_lut[10])
  );
  INV   \Mcount_count_lut<11>_INV_0  (
    .I(count[11]),
    .O(Mcount_count_lut[11])
  );
  INV   \Mcount_count_lut<12>_INV_0  (
    .I(count[12]),
    .O(Mcount_count_lut[12])
  );
  INV   \Mcount_count_lut<13>_INV_0  (
    .I(count[13]),
    .O(Mcount_count_lut[13])
  );
  INV   \Mcount_count_lut<14>_INV_0  (
    .I(count[14]),
    .O(Mcount_count_lut[14])
  );
  INV   \Mcount_count_lut<15>_INV_0  (
    .I(count[15]),
    .O(Mcount_count_lut[15])
  );
  INV   \Mcount_count_lut<16>_INV_0  (
    .I(count[16]),
    .O(Mcount_count_lut[16])
  );
  INV   \Mcount_count_lut<17>_INV_0  (
    .I(count[17]),
    .O(Mcount_count_lut[17])
  );
  INV   \Mcount_count_lut<18>_INV_0  (
    .I(count[18]),
    .O(Mcount_count_lut[18])
  );
  INV   \Mcount_count_lut<19>_INV_0  (
    .I(count[19]),
    .O(Mcount_count_lut[19])
  );
  INV   \Mcount_count_lut<20>_INV_0  (
    .I(count[20]),
    .O(Mcount_count_lut[20])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<21>_INV_0  (
    .I(\serial_interface/time_out [21]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [21])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<20>_INV_0  (
    .I(\serial_interface/time_out [20]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [20])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<19>_INV_0  (
    .I(\serial_interface/time_out [19]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [19])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<18>_INV_0  (
    .I(\serial_interface/time_out [18]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [18])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<17>_INV_0  (
    .I(\serial_interface/time_out [17]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [17])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<16>_INV_0  (
    .I(\serial_interface/time_out [16]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [16])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<15>_INV_0  (
    .I(\serial_interface/time_out [15]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [15])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<14>_INV_0  (
    .I(\serial_interface/time_out [14]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [14])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<13>_INV_0  (
    .I(\serial_interface/time_out [13]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [13])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<12>_INV_0  (
    .I(\serial_interface/time_out [12]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [12])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<11>_INV_0  (
    .I(\serial_interface/time_out [11]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [11])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<10>_INV_0  (
    .I(\serial_interface/time_out [10]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [10])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<9>_INV_0  (
    .I(\serial_interface/time_out [9]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [9])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<8>_INV_0  (
    .I(\serial_interface/time_out [8]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [8])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<7>_INV_0  (
    .I(\serial_interface/time_out [7]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [7])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<6>_INV_0  (
    .I(\serial_interface/time_out [6]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [6])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<5>_INV_0  (
    .I(\serial_interface/time_out [5]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [5])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<4>_INV_0  (
    .I(\serial_interface/time_out [4]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [4])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<3>_INV_0  (
    .I(\serial_interface/time_out [3]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [3])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<2>_INV_0  (
    .I(\serial_interface/time_out [2]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [2])
  );
  INV   \serial_interface/Msub_time_out_addsub0000_lut<1>_INV_0  (
    .I(\serial_interface/time_out [1]),
    .O(\serial_interface/Msub_time_out_addsub0000_lut [1])
  );
  INV   \dac_OCXO/Mcount_bit_count_lut<7>_INV_0  (
    .I(\dac_OCXO/bit_count [7]),
    .O(\dac_OCXO/Mcount_bit_count_lut [7])
  );
  INV   \dac_OCXO/Mcount_bit_count_lut<6>_INV_0  (
    .I(\dac_OCXO/bit_count [6]),
    .O(\dac_OCXO/Mcount_bit_count_lut [6])
  );
  INV   \dac_OCXO/Mcount_bit_count_lut<5>_INV_0  (
    .I(\dac_OCXO/bit_count [5]),
    .O(\dac_OCXO/Mcount_bit_count_lut [5])
  );
  INV   \dac_OCXO/Mcount_bit_count_lut<4>_INV_0  (
    .I(\dac_OCXO/bit_count [4]),
    .O(\dac_OCXO/Mcount_bit_count_lut [4])
  );
  INV   \dac_OCXO/Mcount_bit_count_lut<3>_INV_0  (
    .I(\dac_OCXO/bit_count [3]),
    .O(\dac_OCXO/Mcount_bit_count_lut [3])
  );
  INV   \dac_OCXO/Mcount_bit_count_lut<2>_INV_0  (
    .I(\dac_OCXO/bit_count [2]),
    .O(\dac_OCXO/Mcount_bit_count_lut [2])
  );
  INV   \dac_OCXO/Mcount_bit_count_lut<1>_INV_0  (
    .I(\dac_OCXO/bit_count [1]),
    .O(\dac_OCXO/Mcount_bit_count_lut [1])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<26>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [26]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [26])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<25>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [25]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [25])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<24>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [24]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [24])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<23>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [23]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [23])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<22>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [22]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [22])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<21>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [21]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [21])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<20>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [20]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [20])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<19>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [19]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [19])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<18>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [18]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [18])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<17>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [17]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [17])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<16>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [16]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [16])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<15>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [15]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [15])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<14>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [14]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [14])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<13>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [13]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [13])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<12>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [12]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [12])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<11>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [11]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [11])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<10>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [10]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [10])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<9>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [9]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [9])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<8>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [8]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [8])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<7>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [7]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [7])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<6>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [6]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [6])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<5>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [5]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [5])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<4>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [4]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [4])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<3>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [3]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [3])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<2>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [2]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [2])
  );
  INV   \OCXO_pll/phase_detect/Mcount_phase_counter_lut<1>_INV_0  (
    .I(\OCXO_pll/phase_detect/phase_counter [1]),
    .O(\OCXO_pll/phase_detect/Mcount_phase_counter_lut [1])
  );
  INV   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut<8>_INV_0  (
    .I(\OCXO_pll/phase_loop/out_sum [14]),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[8] )
  );
  INV   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut<6>_INV_0  (
    .I(\OCXO_pll/phase_loop/out_sum [12]),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[6] )
  );
  INV   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut<2>_INV_0  (
    .I(\OCXO_pll/phase_loop/out_sum [3]),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[2] )
  );
  INV   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut<0>_INV_0  (
    .I(\OCXO_pll/phase_loop/out_sum [1]),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[0] )
  );
  INV   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut<11>_INV_0  (
    .I(\OCXO_pll/phase_loop/integrator [22]),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[11] )
  );
  INV   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut<9>_INV_0  (
    .I(\OCXO_pll/phase_loop/integrator [20]),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[9] )
  );
  INV   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut<5>_INV_0  (
    .I(\OCXO_pll/phase_loop/integrator [11]),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[5] )
  );
  INV   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut<3>_INV_0  (
    .I(\OCXO_pll/phase_loop/integrator [9]),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[3] )
  );
  INV   \VCXO_pll/phase_detector/Mcount_phase_counter_lut<1>_INV_0  (
    .I(\VCXO_pll/phase_detector/phase_counter [1]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [1])
  );
  INV   \VCXO_pll/phase_detector/Mcount_phase_counter_lut<2>_INV_0  (
    .I(\VCXO_pll/phase_detector/phase_counter [2]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [2])
  );
  INV   \VCXO_pll/phase_detector/Mcount_phase_counter_lut<3>_INV_0  (
    .I(\VCXO_pll/phase_detector/phase_counter [3]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [3])
  );
  INV   \VCXO_pll/phase_detector/Mcount_phase_counter_lut<4>_INV_0  (
    .I(\VCXO_pll/phase_detector/phase_counter [4]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [4])
  );
  INV   \VCXO_pll/phase_detector/Mcount_phase_counter_lut<5>_INV_0  (
    .I(\VCXO_pll/phase_detector/phase_counter [5]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [5])
  );
  INV   \VCXO_pll/phase_detector/Mcount_phase_counter_lut<6>_INV_0  (
    .I(\VCXO_pll/phase_detector/phase_counter [6]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [6])
  );
  INV   \VCXO_pll/phase_detector/Mcount_phase_counter_lut<7>_INV_0  (
    .I(\VCXO_pll/phase_detector/phase_counter [7]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [7])
  );
  INV   \VCXO_pll/phase_detector/Mcount_phase_counter_lut<8>_INV_0  (
    .I(\VCXO_pll/phase_detector/phase_counter [8]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [8])
  );
  INV   \VCXO_pll/phase_detector/Mcount_phase_counter_lut<9>_INV_0  (
    .I(\VCXO_pll/phase_detector/phase_counter [9]),
    .O(\VCXO_pll/phase_detector/Mcount_phase_counter_lut [9])
  );
  INV   \VCXO_pll/Mcount_ref_clk_count_lut<0>_INV_0  (
    .I(\VCXO_pll/ref_clk_count [0]),
    .O(\VCXO_pll/Mcount_ref_clk_count_lut [0])
  );
  INV   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut<1>_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/period_count [1]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [1])
  );
  INV   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut<2>_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/period_count [2]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [2])
  );
  INV   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut<3>_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/period_count [3]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [3])
  );
  INV   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut<4>_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/period_count [4]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [4])
  );
  INV   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut<5>_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/period_count [5]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [5])
  );
  INV   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut<6>_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/period_count [6]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [6])
  );
  INV   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut<7>_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/period_count [7]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [7])
  );
  INV   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut<8>_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/period_count [8]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [8])
  );
  INV   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut<9>_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/period_count [9]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [9])
  );
  INV   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut<10>_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/period_count [10]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [10])
  );
  INV   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut<11>_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/period_count [11]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [11])
  );
  INV   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut<12>_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/period_count [12]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [12])
  );
  INV   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut<13>_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/period_count [13]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [13])
  );
  INV   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut<14>_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/period_count [14]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [14])
  );
  INV   \LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut<15>_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/period_count [15]),
    .O(\LTC_B/LTC_clockgenerator/Msub_period_count_addsub0000_lut [15])
  );
  INV   \LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_lut<0>_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/ltc_bitcount [0]),
    .O(\LTC_B/LTC_clockgenerator/Mcount_ltc_bitcount_lut [0])
  );
  INV   \LTC_B/Mcount_delay_count_low_lut<0>_INV_0  (
    .I(\LTC_B/delay_count_low [0]),
    .O(\LTC_B/Mcount_delay_count_low_lut [0])
  );
  INV   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut<1>_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/period_count [1]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [1])
  );
  INV   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut<2>_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/period_count [2]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [2])
  );
  INV   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut<3>_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/period_count [3]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [3])
  );
  INV   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut<4>_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/period_count [4]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [4])
  );
  INV   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut<5>_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/period_count [5]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [5])
  );
  INV   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut<6>_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/period_count [6]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [6])
  );
  INV   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut<7>_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/period_count [7]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [7])
  );
  INV   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut<8>_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/period_count [8]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [8])
  );
  INV   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut<9>_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/period_count [9]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [9])
  );
  INV   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut<10>_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/period_count [10]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [10])
  );
  INV   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut<11>_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/period_count [11]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [11])
  );
  INV   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut<12>_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/period_count [12]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [12])
  );
  INV   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut<13>_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/period_count [13]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [13])
  );
  INV   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut<14>_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/period_count [14]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [14])
  );
  INV   \LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut<15>_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/period_count [15]),
    .O(\LTC_A/LTC_clockgenerator/Msub_period_count_addsub0000_lut [15])
  );
  INV   \LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_lut<0>_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/ltc_bitcount [0]),
    .O(\LTC_A/LTC_clockgenerator/Mcount_ltc_bitcount_lut [0])
  );
  INV   \LTC_A/Mcount_delay_count_low_lut<0>_INV_0  (
    .I(\LTC_A/delay_count_low [0]),
    .O(\LTC_A/Mcount_delay_count_low_lut [0])
  );
  INV   \longer_tick_cmp_eq0000_wg_lut<0>_INV_0  (
    .I(count[4]),
    .O(longer_tick_cmp_eq0000_wg_lut[0])
  );
  INV   \VCXO_pll/resync_148_period_inv1_INV_0  (
    .I(\VCXO_pll/resync_148_period_2306 ),
    .O(\VCXO_pll/resync_148_period_inv )
  );
  INV   led1_o1_INV_0 (
    .I(\system_control[3] ),
    .O(led1_o_OBUF_3021)
  );
  INV   \h_gen/pulse_state_mux00011_INV_0  (
    .I(\h_gen/pulse_state_3000 ),
    .O(\h_gen/pulse_state_mux0001 )
  );
  INV   \f_gen/pulse_state_mux00011_INV_0  (
    .I(\f_gen/pulse_state_2639 ),
    .O(\f_gen/pulse_state_mux0001 )
  );
  INV   \LTC_B/Mcount_delay_count_high_xor<0>11_INV_0  (
    .I(\LTC_B/delay_count_high [0]),
    .O(\LTC_B/Result<0>1 )
  );
  INV   \LTC_B/LTC_biphase_generator/biphase_code_not00031_INV_0  (
    .I(\LTC_B/LTC_biphase_generator/biphase_code_582 ),
    .O(\LTC_B/LTC_biphase_generator/biphase_code_not0003 )
  );
  INV   \LTC_A/Mcount_delay_count_high_xor<0>11_INV_0  (
    .I(\LTC_A/delay_count_high [0]),
    .O(\LTC_A/Result<0>1 )
  );
  INV   \LTC_A/LTC_biphase_generator/biphase_code_not00031_INV_0  (
    .I(\LTC_A/LTC_biphase_generator/biphase_code_148 ),
    .O(\LTC_A/LTC_biphase_generator/biphase_code_not0003 )
  );
  INV   \Inst_ad5660_serial_DAC/Mcount_dec_clk_count_xor<0>11_INV_0  (
    .I(\Inst_ad5660_serial_DAC/dec_clk_count [0]),
    .O(\Inst_ad5660_serial_DAC/Result [0])
  );
  INV   \LTC_B/LTC_frame_count/ltc_frame_tick_i_inv1_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/ltc_frame_tick_o_663 ),
    .O(\LTC_B/LTC_frame_count/ltc_frame_tick_i_inv )
  );
  INV   \LTC_B/LTC_clockgenerator/sync_i_inv1_INV_0  (
    .I(\LTC_B/delay_sync_983 ),
    .O(\LTC_B/LTC_clockgenerator/sync_i_inv )
  );
  INV   \LTC_A/LTC_frame_count/ltc_frame_tick_i_inv1_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/ltc_frame_tick_o_229 ),
    .O(\LTC_A/LTC_frame_count/ltc_frame_tick_i_inv )
  );
  INV   \LTC_A/LTC_clockgenerator/sync_i_inv1_INV_0  (
    .I(\LTC_A/delay_sync_549 ),
    .O(\LTC_A/LTC_clockgenerator/sync_i_inv )
  );
  INV   \Inst_ad5660_serial_DAC/dec_clk_count_not00011_INV_0  (
    .I(\Inst_ad5660_serial_DAC/state_FSM_FFd3_107 ),
    .O(\Inst_ad5660_serial_DAC/dec_clk_count_not0001 )
  );
  INV   \serial_interface/RW_dir_inv1_INV_0  (
    .I(\serial_interface/RW_dir_3113 ),
    .O(\serial_interface/RW_dir_inv )
  );
  INV   longer_tick_not00011_INV_0 (
    .I(longer_tick_not0001_inv),
    .O(longer_tick_not0001)
  );
  INV   \LTC_B/delay_sync_not00011_INV_0  (
    .I(\LTC_B/Mcompar_delay_sync_cmp_eq0000_cy [15]),
    .O(\LTC_B/delay_sync_not0001 )
  );
  INV   \LTC_A/delay_sync_not00011_INV_0  (
    .I(\LTC_A/Mcompar_delay_sync_cmp_eq0000_cy [15]),
    .O(\LTC_A/delay_sync_not0001 )
  );
  INV   \LTC_B/LTC_biphase_generator/Madd_ltc_frame_i_not0000<1>1_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/ltc_bitcount [2]),
    .O(\LTC_B/LTC_biphase_generator/Madd_ltc_frame_i_not0000 [1])
  );
  INV   \LTC_A/LTC_biphase_generator/Madd_ltc_frame_i_not0000<1>1_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/ltc_bitcount [2]),
    .O(\LTC_A/LTC_biphase_generator/Madd_ltc_frame_i_not0000 [1])
  );
  INV   \LTC_B/LTC_biphase_generator/Madd_ltc_frame_i_not0000<0>2_INV_0  (
    .I(\LTC_B/LTC_clockgenerator/ltc_bitcount [1]),
    .O(\LTC_B/LTC_biphase_generator/Madd_ltc_frame_i_not0000 [0])
  );
  INV   \LTC_A/LTC_biphase_generator/Madd_ltc_frame_i_not0000<0>2_INV_0  (
    .I(\LTC_A/LTC_clockgenerator/ltc_bitcount [1]),
    .O(\LTC_A/LTC_biphase_generator/Madd_ltc_frame_i_not0000 [0])
  );
  INV   \OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut<16>1_INV_0  (
    .I(\OCXO_pll/phase_loop/integrator [35]),
    .O(\OCXO_pll/phase_loop/Mcompar_integrator_limit_cmp_gt0000_lut[16] )
  );
  INV   \OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut<13>1_INV_0  (
    .I(\OCXO_pll/phase_loop/out_sum [27]),
    .O(\OCXO_pll/phase_loop/Mcompar_out_sum_limit_cmp_gt0000_lut[13] )
  );
  IOBUF   SDA_io_IOBUF (
    .I(\serial_interface/SDA_out_3119 ),
    .T(\serial_interface/RW_dir_inv ),
    .O(N156),
    .IO(SDA_io)
  );
  FDS   \serial_interface/SDA_out  (
    .C(clk_148),
    .D(\serial_interface/SDA_out_mux0003144 ),
    .S(\serial_interface/SDA_out_mux00030_3120 ),
    .Q(\serial_interface/SDA_out_3119 )
  );
  LUT4 #(
    .INIT ( 16'hF8A8 ))
  \serial_interface/command_mux0000<0>1  (
    .I0(\serial_interface/command [2]),
    .I1(\serial_interface/state_FSM_FFd2_3359 ),
    .I2(\serial_interface/state_FSM_FFd3_3369 ),
    .I3(\serial_interface/state_FSM_FFd1_3351 ),
    .O(\serial_interface/command_mux0000<0>1_3142 )
  );
  LUT4 #(
    .INIT ( 16'hEAE8 ))
  \serial_interface/command_mux0000<0>2  (
    .I0(\serial_interface/command [2]),
    .I1(\serial_interface/state_FSM_FFd3_3369 ),
    .I2(\serial_interface/state_FSM_FFd1_3351 ),
    .I3(\serial_interface/state_FSM_FFd2_3359 ),
    .O(\serial_interface/command_mux0000<0>2_3143 )
  );
  MUXF5   \serial_interface/command_mux0000<0>_f5  (
    .I0(\serial_interface/command_mux0000<0>2_3143 ),
    .I1(\serial_interface/command_mux0000<0>1_3142 ),
    .S(\serial_interface/state_FSM_FFd4_3382 ),
    .O(\serial_interface/command_mux0000 [0])
  );
  LUT4 #(
    .INIT ( 16'h0010 ))
  \Inst_ad5660_serial_DAC/state_FSM_FFd5-In11  (
    .I0(\Inst_ad5660_serial_DAC/dec_clk_count [0]),
    .I1(\Inst_ad5660_serial_DAC/dec_clk_count [3]),
    .I2(\Inst_ad5660_serial_DAC/dec_clk_count [1]),
    .I3(\Inst_ad5660_serial_DAC/dec_clk_count [2]),
    .O(\Inst_ad5660_serial_DAC/state_FSM_FFd5-In1 )
  );
  MUXF5   \Inst_ad5660_serial_DAC/state_FSM_FFd5-In1_f5  (
    .I0(tp2_o_OBUF_3479),
    .I1(\Inst_ad5660_serial_DAC/state_FSM_FFd5-In1 ),
    .S(\Inst_ad5660_serial_DAC/state_FSM_FFd3_107 ),
    .O(\Inst_ad5660_serial_DAC/N5 )
  );
  LUT4 #(
    .INIT ( 16'hC8CD ))
  \serial_interface/command_mux0000<2>1  (
    .I0(\serial_interface/state_FSM_FFd2_3359 ),
    .I1(\serial_interface/command [0]),
    .I2(\serial_interface/state_FSM_FFd3_3369 ),
    .I3(\serial_interface/state_FSM_FFd1_3351 ),
    .O(\serial_interface/command_mux0000<2>1_3146 )
  );
  LUT4 #(
    .INIT ( 16'hBF01 ))
  \serial_interface/command_mux0000<2>2  (
    .I0(\serial_interface/state_FSM_FFd2_3359 ),
    .I1(\serial_interface/state_FSM_FFd1_3351 ),
    .I2(\serial_interface/state_FSM_FFd3_3369 ),
    .I3(\serial_interface/command [0]),
    .O(\serial_interface/command_mux0000<2>2_3147 )
  );
  MUXF5   \serial_interface/command_mux0000<2>_f5  (
    .I0(\serial_interface/command_mux0000<2>2_3147 ),
    .I1(\serial_interface/command_mux0000<2>1_3146 ),
    .S(\serial_interface/state_FSM_FFd4_3382 ),
    .O(\serial_interface/command_mux0000 [2])
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  \LTC_B/LTC_frame_count/count_sec_or0000601  (
    .I0(\LTC_B/LTC_frame_count/frame_units [0]),
    .I1(\LTC_B/LTC_frame_count/frame_units [2]),
    .I2(\LTC_B/LTC_frame_count/frame_units [3]),
    .I3(\LTC_B/LTC_frame_count/frame_units [1]),
    .O(\LTC_B/LTC_frame_count/count_sec_or0000601_744 )
  );
  MUXF5   \LTC_B/LTC_frame_count/count_sec_or000060_f5  (
    .I0(tp2_o_OBUF_3479),
    .I1(\LTC_B/LTC_frame_count/count_sec_or0000601_744 ),
    .S(\LTC_setup[5] ),
    .O(\LTC_B/LTC_frame_count/count_sec_or000060 )
  );
  LUT4 #(
    .INIT ( 16'h0020 ))
  \LTC_A/LTC_frame_count/count_sec_or0000601  (
    .I0(\LTC_A/LTC_frame_count/frame_units [0]),
    .I1(\LTC_A/LTC_frame_count/frame_units [2]),
    .I2(\LTC_A/LTC_frame_count/frame_units [3]),
    .I3(\LTC_A/LTC_frame_count/frame_units [1]),
    .O(\LTC_A/LTC_frame_count/count_sec_or0000601_310 )
  );
  MUXF5   \LTC_A/LTC_frame_count/count_sec_or000060_f5  (
    .I0(tp2_o_OBUF_3479),
    .I1(\LTC_A/LTC_frame_count/count_sec_or0000601_310 ),
    .S(\LTC_setup[1] ),
    .O(\LTC_A/LTC_frame_count/count_sec_or000060 )
  );
  LUT2 #(
    .INIT ( 4'h1 ))
  \LTC_B/LTC_frame_count/frame_tens_mux0002<5>151  (
    .I0(\LTC_B/LTC_frame_count/frame_units [0]),
    .I1(\LTC_B/LTC_frame_count/frame_units [2]),
    .O(\LTC_B/LTC_frame_count/frame_tens_mux0002<5>151_752 )
  );
  LUT4 #(
    .INIT ( 16'h11F1 ))
  \LTC_B/LTC_frame_count/frame_tens_mux0002<5>152  (
    .I0(\LTC_B/LTC_frame_count/frame_units [0]),
    .I1(\LTC_B/LTC_frame_count/frame_units [2]),
    .I2(\LTC_B/LTC_frame_count/frame_units [3]),
    .I3(\LTC_B/LTC_frame_count/frame_tens [0]),
    .O(\LTC_B/LTC_frame_count/frame_tens_mux0002<5>152_753 )
  );
  MUXF5   \LTC_B/LTC_frame_count/frame_tens_mux0002<5>15_f5  (
    .I0(\LTC_B/LTC_frame_count/frame_tens_mux0002<5>152_753 ),
    .I1(\LTC_B/LTC_frame_count/frame_tens_mux0002<5>151_752 ),
    .S(\LTC_setup[5] ),
    .O(\LTC_B/LTC_frame_count/frame_tens_mux0002<5>15 )
  );
  LUT2 #(
    .INIT ( 4'h1 ))
  \LTC_A/LTC_frame_count/frame_tens_mux0002<5>151  (
    .I0(\LTC_A/LTC_frame_count/frame_units [0]),
    .I1(\LTC_A/LTC_frame_count/frame_units [2]),
    .O(\LTC_A/LTC_frame_count/frame_tens_mux0002<5>151_318 )
  );
  LUT4 #(
    .INIT ( 16'h11F1 ))
  \LTC_A/LTC_frame_count/frame_tens_mux0002<5>152  (
    .I0(\LTC_A/LTC_frame_count/frame_units [0]),
    .I1(\LTC_A/LTC_frame_count/frame_units [2]),
    .I2(\LTC_A/LTC_frame_count/frame_units [3]),
    .I3(\LTC_A/LTC_frame_count/frame_tens [0]),
    .O(\LTC_A/LTC_frame_count/frame_tens_mux0002<5>152_319 )
  );
  MUXF5   \LTC_A/LTC_frame_count/frame_tens_mux0002<5>15_f5  (
    .I0(\LTC_A/LTC_frame_count/frame_tens_mux0002<5>152_319 ),
    .I1(\LTC_A/LTC_frame_count/frame_tens_mux0002<5>151_318 ),
    .S(\LTC_setup[1] ),
    .O(\LTC_A/LTC_frame_count/frame_tens_mux0002<5>15 )
  );
  LUT4 #(
    .INIT ( 16'hFF31 ))
  \serial_interface/state_FSM_FFd3-In331  (
    .I0(\serial_interface/SCL_delay [0]),
    .I1(\serial_interface/state_FSM_FFd2_3359 ),
    .I2(\serial_interface/SCL_delay [1]),
    .I3(\serial_interface/state_FSM_FFd1_3351 ),
    .O(\serial_interface/state_FSM_FFd3-In331_3378 )
  );
  MUXF5   \serial_interface/state_FSM_FFd3-In33_f5  (
    .I0(\serial_interface/state_FSM_FFd3-In331_3378 ),
    .I1(N1),
    .S(\serial_interface/state_FSM_FFd4_3382 ),
    .O(\serial_interface/state_FSM_FFd3-In33 )
  );
  LUT4 #(
    .INIT ( 16'h5155 ))
  \serial_interface/state_FSM_FFd3-In511  (
    .I0(\serial_interface/command [2]),
    .I1(\serial_interface/SCL_delay [1]),
    .I2(\serial_interface/SCL_delay [0]),
    .I3(\serial_interface/command [1]),
    .O(\serial_interface/state_FSM_FFd3-In511_3380 )
  );
  MUXF5   \serial_interface/state_FSM_FFd3-In51_f5  (
    .I0(tp2_o_OBUF_3479),
    .I1(\serial_interface/state_FSM_FFd3-In511_3380 ),
    .S(\serial_interface/state_FSM_FFd2_3359 ),
    .O(\serial_interface/state_FSM_FFd3-In51 )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \serial_interface/state_FSM_FFd3-In1641  (
    .I0(\serial_interface/SCL_delay [0]),
    .I1(\serial_interface/command [0]),
    .I2(\serial_interface/recieve_byte [0]),
    .I3(\serial_interface/SCL_delay [1]),
    .O(\serial_interface/state_FSM_FFd3-In1641_3374 )
  );
  MUXF5   \serial_interface/state_FSM_FFd3-In164_f5  (
    .I0(tp2_o_OBUF_3479),
    .I1(\serial_interface/state_FSM_FFd3-In1641_3374 ),
    .S(\serial_interface/state_FSM_FFd1_3351 ),
    .O(\serial_interface/state_FSM_FFd3-In164 )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \serial_interface/state_FSM_FFd4-In661  (
    .I0(\serial_interface/state_FSM_FFd3_3369 ),
    .I1(\serial_interface/command [0]),
    .I2(\serial_interface/state_FSM_FFd2_3359 ),
    .I3(\serial_interface/state_cmp_eq0007 ),
    .O(\serial_interface/state_FSM_FFd4-In661_3391 )
  );
  MUXF5   \serial_interface/state_FSM_FFd4-In66_f5  (
    .I0(\serial_interface/state_FSM_FFd4-In661_3391 ),
    .I1(tp2_o_OBUF_3479),
    .S(\serial_interface/state_FSM_FFd4_3382 ),
    .O(\serial_interface/state_FSM_FFd4-In66 )
  );
  LUT4 #(
    .INIT ( 16'h2FAF ))
  \serial_interface/state_FSM_FFd4-In951  (
    .I0(\serial_interface/state_cmp_eq0008 ),
    .I1(N87),
    .I2(\serial_interface/state_FSM_FFd2_3359 ),
    .I3(\serial_interface/bit_count [3]),
    .O(\serial_interface/state_FSM_FFd4-In951_3393 )
  );
  MUXF5   \serial_interface/state_FSM_FFd4-In95_f5  (
    .I0(tp2_o_OBUF_3479),
    .I1(\serial_interface/state_FSM_FFd4-In951_3393 ),
    .S(\serial_interface/state_FSM_FFd3_3369 ),
    .O(\serial_interface/state_FSM_FFd4-In95 )
  );
  LUT4 #(
    .INIT ( 16'hAAA8 ))
  \serial_interface/send_byte_mux0001<7>1921  (
    .I0(I2C_byte_out_cmp_eq0024),
    .I1(\OCXO_pll/phase_loop/N01 ),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .I3(N138),
    .O(\serial_interface/send_byte_mux0001<7>1921_3338 )
  );
  LUT3 #(
    .INIT ( 8'hA8 ))
  \serial_interface/send_byte_mux0001<7>1922  (
    .I0(I2C_byte_out_cmp_eq0024),
    .I1(\OCXO_pll/phase_loop/N01 ),
    .I2(N138),
    .O(\serial_interface/send_byte_mux0001<7>1922_3339 )
  );
  MUXF5   \serial_interface/send_byte_mux0001<7>192_f5  (
    .I0(\serial_interface/send_byte_mux0001<7>1922_3339 ),
    .I1(\serial_interface/send_byte_mux0001<7>1921_3338 ),
    .S(\OCXO_pll/phase_loop/out_sum [7]),
    .O(\serial_interface/send_byte_mux0001<7>192 )
  );
  LUT4 #(
    .INIT ( 16'hAAA8 ))
  \serial_interface/send_byte_mux0001<5>1921  (
    .I0(I2C_byte_out_cmp_eq0024),
    .I1(\OCXO_pll/phase_loop/N01 ),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .I3(N142),
    .O(\serial_interface/send_byte_mux0001<5>1921_3296 )
  );
  LUT3 #(
    .INIT ( 8'hA8 ))
  \serial_interface/send_byte_mux0001<5>1922  (
    .I0(I2C_byte_out_cmp_eq0024),
    .I1(\OCXO_pll/phase_loop/N01 ),
    .I2(N142),
    .O(\serial_interface/send_byte_mux0001<5>1922_3297 )
  );
  MUXF5   \serial_interface/send_byte_mux0001<5>192_f5  (
    .I0(\serial_interface/send_byte_mux0001<5>1922_3297 ),
    .I1(\serial_interface/send_byte_mux0001<5>1921_3296 ),
    .S(\OCXO_pll/phase_loop/out_sum [5]),
    .O(\serial_interface/send_byte_mux0001<5>192 )
  );
  LUT4 #(
    .INIT ( 16'hAAA8 ))
  \serial_interface/send_byte_mux0001<3>1801  (
    .I0(I2C_byte_out_cmp_eq0025),
    .I1(\OCXO_pll/phase_loop/N01 ),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .I3(N152),
    .O(\serial_interface/send_byte_mux0001<3>1801_3251 )
  );
  LUT3 #(
    .INIT ( 8'hA8 ))
  \serial_interface/send_byte_mux0001<3>1802  (
    .I0(I2C_byte_out_cmp_eq0025),
    .I1(\OCXO_pll/phase_loop/N01 ),
    .I2(N152),
    .O(\serial_interface/send_byte_mux0001<3>1802_3252 )
  );
  MUXF5   \serial_interface/send_byte_mux0001<3>180_f5  (
    .I0(\serial_interface/send_byte_mux0001<3>1802_3252 ),
    .I1(\serial_interface/send_byte_mux0001<3>1801_3251 ),
    .S(\OCXO_pll/phase_loop/out_sum [11]),
    .O(\serial_interface/send_byte_mux0001<3>180 )
  );
  LUT4 #(
    .INIT ( 16'hA888 ))
  \serial_interface/send_byte_mux0001<1>1921  (
    .I0(I2C_byte_out_cmp_eq0024),
    .I1(genlock_register_21_1_2775),
    .I2(\OCXO_pll/phase_loop/out_sum [1]),
    .I3(\OCXO_pll/phase_loop/N11 ),
    .O(\serial_interface/send_byte_mux0001<1>1921_3210 )
  );
  LUT3 #(
    .INIT ( 8'h80 ))
  \serial_interface/send_byte_mux0001<1>1922  (
    .I0(I2C_byte_out_cmp_eq0024),
    .I1(\OCXO_pll/phase_loop/out_sum [1]),
    .I2(\OCXO_pll/phase_loop/N11 ),
    .O(\serial_interface/send_byte_mux0001<1>1922_3211 )
  );
  MUXF5   \serial_interface/send_byte_mux0001<1>192_f5  (
    .I0(\serial_interface/send_byte_mux0001<1>1922_3211 ),
    .I1(\serial_interface/send_byte_mux0001<1>1921_3210 ),
    .S(\system_control[4] ),
    .O(\serial_interface/send_byte_mux0001<1>192 )
  );
  LUT4 #(
    .INIT ( 16'h0001 ))
  \OCXO_pll/phase_detect/int_pps_tick_cmp_eq0000171  (
    .I0(\OCXO_pll/phase_detect/phase_counter [11]),
    .I1(\OCXO_pll/phase_detect/phase_counter [18]),
    .I2(\OCXO_pll/phase_detect/phase_counter [24]),
    .I3(\OCXO_pll/phase_detect/phase_counter [16]),
    .O(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq0000171_1497 )
  );
  MUXF5   \OCXO_pll/phase_detect/int_pps_tick_cmp_eq000017_f5  (
    .I0(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq0000171_1497 ),
    .I1(tp2_o_OBUF_3479),
    .S(\OCXO_pll/phase_detect/phase_counter [9]),
    .O(\OCXO_pll/phase_detect/int_pps_tick_cmp_eq000017 )
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \dac_OCXO/bit_count_not0001111  (
    .I0(\dac_OCXO/bit_count [2]),
    .I1(\dac_OCXO/bit_count [0]),
    .I2(\dac_OCXO/bit_count [1]),
    .I3(\dac_OCXO/bit_count [5]),
    .O(\dac_OCXO/bit_count_not0001111_2434 )
  );
  MUXF5   \dac_OCXO/bit_count_not000111_f5  (
    .I0(\dac_OCXO/bit_count_not0001111_2434 ),
    .I1(N1),
    .S(\dac_OCXO/bit_count [6]),
    .O(\dac_OCXO/bit_count_not000111 )
  );
  LUT4 #(
    .INIT ( 16'h4000 ))
  \VCXO_pll/ref_clk_count_cmp_eq000091  (
    .I0(\VCXO_pll/ref_clk_count [6]),
    .I1(\VCXO_pll/ref_clk_count [0]),
    .I2(\VCXO_pll/ref_clk_count [2]),
    .I3(\VCXO_pll/ref_clk_count [3]),
    .O(\VCXO_pll/ref_clk_count_cmp_eq000091_2304 )
  );
  MUXF5   \VCXO_pll/ref_clk_count_cmp_eq00009_f5  (
    .I0(tp2_o_OBUF_3479),
    .I1(\VCXO_pll/ref_clk_count_cmp_eq000091_2304 ),
    .S(\VCXO_pll/ref_clk_count [7]),
    .O(\VCXO_pll/ref_clk_count_cmp_eq00009 )
  );
  LUT4 #(
    .INIT ( 16'h8000 ))
  \OCXO_pll/phase_detect/phase_counter_cmp_eq0000111  (
    .I0(\OCXO_pll/phase_detect/phase_counter [11]),
    .I1(\OCXO_pll/phase_detect/phase_counter [10]),
    .I2(\OCXO_pll/phase_detect/phase_counter [24]),
    .I3(\OCXO_pll/phase_detect/phase_counter [26]),
    .O(\OCXO_pll/phase_detect/phase_counter_cmp_eq0000111_1540 )
  );
  MUXF5   \OCXO_pll/phase_detect/phase_counter_cmp_eq000011_f5  (
    .I0(tp2_o_OBUF_3479),
    .I1(\OCXO_pll/phase_detect/phase_counter_cmp_eq0000111_1540 ),
    .S(\OCXO_pll/phase_detect/phase_counter [9]),
    .O(\OCXO_pll/phase_detect/phase_counter_cmp_eq000011 )
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \OCXO_pll/phase_loop/dac_word_o<6>1  (
    .I0(\OCXO_pll/phase_loop/N01 ),
    .I1(\OCXO_pll/phase_loop/out_sum [6]),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .I3(genlock_register_21_6_2780),
    .O(\OCXO_pll/phase_loop/dac_word_o[6] )
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \OCXO_pll/phase_loop/dac_word_o<6>2  (
    .I0(\OCXO_pll/phase_loop/N01 ),
    .I1(\OCXO_pll/phase_loop/out_sum [6]),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .O(\OCXO_pll/phase_loop/dac_word_o<6>1_1773 )
  );
  MUXF5   \OCXO_pll/phase_loop/dac_word_o<6>_f5  (
    .I0(\OCXO_pll/phase_loop/dac_word_o<6>1_1773 ),
    .I1(\OCXO_pll/phase_loop/dac_word_o[6] ),
    .S(\system_control[4] ),
    .O(\OCXO_cv_word[6] )
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \OCXO_pll/phase_loop/dac_word_o<4>1  (
    .I0(\OCXO_pll/phase_loop/N01 ),
    .I1(\OCXO_pll/phase_loop/out_sum [4]),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .I3(genlock_register_21_4_2778),
    .O(\OCXO_pll/phase_loop/dac_word_o[4] )
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \OCXO_pll/phase_loop/dac_word_o<4>2  (
    .I0(\OCXO_pll/phase_loop/N01 ),
    .I1(\OCXO_pll/phase_loop/out_sum [4]),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .O(\OCXO_pll/phase_loop/dac_word_o<4>1_1771 )
  );
  MUXF5   \OCXO_pll/phase_loop/dac_word_o<4>_f5  (
    .I0(\OCXO_pll/phase_loop/dac_word_o<4>1_1771 ),
    .I1(\OCXO_pll/phase_loop/dac_word_o[4] ),
    .S(\system_control[4] ),
    .O(\OCXO_cv_word[4] )
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \OCXO_pll/phase_loop/dac_word_o<2>1  (
    .I0(\OCXO_pll/phase_loop/N01 ),
    .I1(\OCXO_pll/phase_loop/out_sum [2]),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .I3(genlock_register_21_2_2776),
    .O(\OCXO_pll/phase_loop/dac_word_o[2] )
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \OCXO_pll/phase_loop/dac_word_o<2>2  (
    .I0(\OCXO_pll/phase_loop/N01 ),
    .I1(\OCXO_pll/phase_loop/out_sum [2]),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .O(\OCXO_pll/phase_loop/dac_word_o<2>1_1769 )
  );
  MUXF5   \OCXO_pll/phase_loop/dac_word_o<2>_f5  (
    .I0(\OCXO_pll/phase_loop/dac_word_o<2>1_1769 ),
    .I1(\OCXO_pll/phase_loop/dac_word_o[2] ),
    .S(\system_control[4] ),
    .O(\OCXO_cv_word[2] )
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \OCXO_pll/phase_loop/dac_word_o<15>1  (
    .I0(\OCXO_pll/phase_loop/N01 ),
    .I1(\OCXO_pll/phase_loop/out_sum [15]),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .I3(genlock_register_22_7_2790),
    .O(\OCXO_pll/phase_loop/dac_word_o[15] )
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \OCXO_pll/phase_loop/dac_word_o<15>2  (
    .I0(\OCXO_pll/phase_loop/N01 ),
    .I1(\OCXO_pll/phase_loop/out_sum [15]),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .O(\OCXO_pll/phase_loop/dac_word_o<15>1_1767 )
  );
  MUXF5   \OCXO_pll/phase_loop/dac_word_o<15>_f5  (
    .I0(\OCXO_pll/phase_loop/dac_word_o<15>1_1767 ),
    .I1(\OCXO_pll/phase_loop/dac_word_o[15] ),
    .S(\system_control[4] ),
    .O(\OCXO_cv_word[15] )
  );
  LUT4 #(
    .INIT ( 16'hFFEA ))
  \OCXO_pll/phase_loop/dac_word_o<13>1  (
    .I0(\OCXO_pll/phase_loop/N01 ),
    .I1(\OCXO_pll/phase_loop/out_sum [13]),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .I3(genlock_register_22_5_2788),
    .O(\OCXO_pll/phase_loop/dac_word_o[13] )
  );
  LUT3 #(
    .INIT ( 8'hEA ))
  \OCXO_pll/phase_loop/dac_word_o<13>2  (
    .I0(\OCXO_pll/phase_loop/N01 ),
    .I1(\OCXO_pll/phase_loop/out_sum [13]),
    .I2(\OCXO_pll/phase_loop/N2 ),
    .O(\OCXO_pll/phase_loop/dac_word_o<13>1_1765 )
  );
  MUXF5   \OCXO_pll/phase_loop/dac_word_o<13>_f5  (
    .I0(\OCXO_pll/phase_loop/dac_word_o<13>1_1765 ),
    .I1(\OCXO_pll/phase_loop/dac_word_o[13] ),
    .S(\system_control[4] ),
    .O(\OCXO_cv_word[13] )
  );
  LUT4 #(
    .INIT ( 16'hA888 ))
  \serial_interface/send_byte_mux0001<7>21611  (
    .I0(N2),
    .I1(\serial_interface/send_byte_mux0001<7>171_3335 ),
    .I2(I2C_byte_out_cmp_eq0025),
    .I3(\OCXO_cv_word[15] ),
    .O(\serial_interface/send_byte_mux0001<7>2161 )
  );
  MUXF5   \serial_interface/send_byte_mux0001<7>2161_f5  (
    .I0(\serial_interface/send_byte_mux0001<7>2161 ),
    .I1(N2),
    .S(\serial_interface/send_byte_mux0001<7>192 ),
    .O(\serial_interface/send_byte_mux0001<7>216 )
  );
  LUT4 #(
    .INIT ( 16'hA888 ))
  \serial_interface/send_byte_mux0001<5>21611  (
    .I0(N2),
    .I1(\serial_interface/send_byte_mux0001<5>171_3293 ),
    .I2(I2C_byte_out_cmp_eq0025),
    .I3(\OCXO_cv_word[13] ),
    .O(\serial_interface/send_byte_mux0001<5>2161 )
  );
  MUXF5   \serial_interface/send_byte_mux0001<5>2161_f5  (
    .I0(\serial_interface/send_byte_mux0001<5>2161 ),
    .I1(N2),
    .S(\serial_interface/send_byte_mux0001<5>192 ),
    .O(\serial_interface/send_byte_mux0001<5>216 )
  );
  LUT4 #(
    .INIT ( 16'hA888 ))
  \serial_interface/send_byte_mux0001<3>21611  (
    .I0(N2),
    .I1(\serial_interface/send_byte_mux0001<3>171_3249 ),
    .I2(I2C_byte_out_cmp_eq0024),
    .I3(\OCXO_cv_word[3] ),
    .O(\serial_interface/send_byte_mux0001<3>2161 )
  );
  MUXF5   \serial_interface/send_byte_mux0001<3>2161_f5  (
    .I0(\serial_interface/send_byte_mux0001<3>2161 ),
    .I1(N2),
    .S(\serial_interface/send_byte_mux0001<3>180 ),
    .O(\serial_interface/send_byte_mux0001<3>216 )
  );
  LUT4 #(
    .INIT ( 16'hA888 ))
  \serial_interface/send_byte_mux0001<1>21611  (
    .I0(N2),
    .I1(\serial_interface/send_byte_mux0001<1>171_3207 ),
    .I2(I2C_byte_out_cmp_eq0025),
    .I3(\OCXO_cv_word[9] ),
    .O(\serial_interface/send_byte_mux0001<1>2161 )
  );
  MUXF5   \serial_interface/send_byte_mux0001<1>2161_f5  (
    .I0(\serial_interface/send_byte_mux0001<1>2161 ),
    .I1(N2),
    .S(\serial_interface/send_byte_mux0001<1>192 ),
    .O(\serial_interface/send_byte_mux0001<1>216 )
  );
  LUT4 #(
    .INIT ( 16'hFEFF ))
  \Inst_ad5660_serial_DAC/state_FSM_FFd3-In1711  (
    .I0(\Inst_ad5660_serial_DAC/dec_clk_count [2]),
    .I1(\Inst_ad5660_serial_DAC/dec_clk_count [0]),
    .I2(\Inst_ad5660_serial_DAC/dec_clk_count [3]),
    .I3(\Inst_ad5660_serial_DAC/dec_clk_count [1]),
    .O(\Inst_ad5660_serial_DAC/state_FSM_FFd3-In171 )
  );
  MUXF5   \Inst_ad5660_serial_DAC/state_FSM_FFd3-In171_f5  (
    .I0(tp2_o_OBUF_3479),
    .I1(\Inst_ad5660_serial_DAC/state_FSM_FFd3-In171 ),
    .S(\Inst_ad5660_serial_DAC/state_FSM_FFd3_107 ),
    .O(\Inst_ad5660_serial_DAC/state_FSM_FFd3-In17 )
  );
  LUT3 #(
    .INIT ( 8'hC4 ))
  \communication/state_FSM_FFd1-In11  (
    .I0(\communication/stop_line [0]),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\communication/stop_line [1]),
    .O(\communication/state_FSM_FFd1-In11_2368 )
  );
  LUT4 #(
    .INIT ( 16'hCC8C ))
  \communication/state_FSM_FFd1-In12  (
    .I0(\communication/stop_line [1]),
    .I1(\communication/state_FSM_FFd1_2366 ),
    .I2(\communication/stop_line [0]),
    .I3(\communication/update_line [0]),
    .O(\communication/state_FSM_FFd1-In12_2369 )
  );
  MUXF5   \communication/state_FSM_FFd1-In1_f5  (
    .I0(\communication/state_FSM_FFd1-In12_2369 ),
    .I1(\communication/state_FSM_FFd1-In11_2368 ),
    .S(\communication/update_line [1]),
    .O(\communication/state_FSM_FFd1-In1 )
  );
  LUT4 #(
    .INIT ( 16'hD555 ))
  \LTC_B/LTC_frame_count/frame_units_mux0009<1>86_SW01  (
    .I0(\LTC_B/LTC_frame_count/frame_units [3]),
    .I1(\LTC_B/LTC_frame_count/frame_units_mux0009<1>15_766 ),
    .I2(\LTC_B/LTC_frame_count/frame_units_mux0009<1>26_767 ),
    .I3(\LTC_B/LTC_frame_count/frame_units_mux0009<1>31_768 ),
    .O(\LTC_B/LTC_frame_count/frame_units_mux0009<1>86_SW0 )
  );
  MUXF5   \LTC_B/LTC_frame_count/frame_units_mux0009<1>86_SW0_f5  (
    .I0(\LTC_B/LTC_frame_count/frame_units_mux0009<1>86_SW01_771 ),
    .I1(\LTC_B/LTC_frame_count/frame_units_mux0009<1>86_SW0 ),
    .S(\LTC_B/LTC_frame_count/N12 ),
    .O(N272)
  );
  LUT4 #(
    .INIT ( 16'hD555 ))
  \LTC_A/LTC_frame_count/frame_units_mux0009<1>86_SW01  (
    .I0(\LTC_A/LTC_frame_count/frame_units [3]),
    .I1(\LTC_A/LTC_frame_count/frame_units_mux0009<1>15_332 ),
    .I2(\LTC_A/LTC_frame_count/frame_units_mux0009<1>26_333 ),
    .I3(\LTC_A/LTC_frame_count/frame_units_mux0009<1>31_334 ),
    .O(\LTC_A/LTC_frame_count/frame_units_mux0009<1>86_SW0 )
  );
  MUXF5   \LTC_A/LTC_frame_count/frame_units_mux0009<1>86_SW0_f5  (
    .I0(\LTC_A/LTC_frame_count/frame_units_mux0009<1>86_SW01_337 ),
    .I1(\LTC_A/LTC_frame_count/frame_units_mux0009<1>86_SW0 ),
    .S(\LTC_A/LTC_frame_count/N12 ),
    .O(N274)
  );
  LUT4 #(
    .INIT ( 16'hFFF1 ))
  \serial_interface/state_FSM_FFd1-In118_SW01  (
    .I0(\serial_interface/command [0]),
    .I1(\serial_interface/command [1]),
    .I2(\serial_interface/state_FSM_FFd3_3369 ),
    .I3(\serial_interface/state_FSM_FFd4_3382 ),
    .O(\serial_interface/state_FSM_FFd1-In118_SW0 )
  );
  LUT3 #(
    .INIT ( 8'hFE ))
  \serial_interface/state_FSM_FFd1-In118_SW02  (
    .I0(\serial_interface/state_FSM_FFd3_3369 ),
    .I1(\serial_interface/state_cmp_eq0011 ),
    .I2(\serial_interface/state_FSM_FFd4_3382 ),
    .O(\serial_interface/state_FSM_FFd1-In118_SW01_3355 )
  );
  MUXF5   \serial_interface/state_FSM_FFd1-In118_SW0_f5  (
    .I0(\serial_interface/state_FSM_FFd1-In118_SW01_3355 ),
    .I1(\serial_interface/state_FSM_FFd1-In118_SW0 ),
    .S(\serial_interface/state_cmp_eq0008 ),
    .O(N348)
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \LTC_B/LTC_frame_count/frame_tens_mux0002<5>127_SW11  (
    .I0(\LTC_B/LTC_frame_count/frame_tens_mux0002<5>15 ),
    .I1(\LTC_B/LTC_frame_count/frame_tens_mux0002<5>30_754 ),
    .I2(\LTC_B/LTC_frame_count/frame_tens_mux0002<5>52_755 ),
    .I3(\LTC_B/LTC_frame_count/frame_tens_mux0002<5>57_756 ),
    .O(\LTC_B/LTC_frame_count/frame_tens_mux0002<5>127_SW1 )
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \LTC_B/LTC_frame_count/frame_tens_mux0002<5>127_SW12  (
    .I0(\LTC_B/LTC_frame_count/frame_tens [0]),
    .I1(\LTC_B/LTC_frame_count/N5 ),
    .O(\LTC_B/LTC_frame_count/frame_tens_mux0002<5>127_SW11_750 )
  );
  MUXF5   \LTC_B/LTC_frame_count/frame_tens_mux0002<5>127_SW1_f5  (
    .I0(\LTC_B/LTC_frame_count/frame_tens_mux0002<5>127_SW11_750 ),
    .I1(\LTC_B/LTC_frame_count/frame_tens_mux0002<5>127_SW1 ),
    .S(\LTC_B/LTC_frame_count/frame_tens [1]),
    .O(N354)
  );
  LUT4 #(
    .INIT ( 16'hFFFE ))
  \LTC_A/LTC_frame_count/frame_tens_mux0002<5>127_SW11  (
    .I0(\LTC_A/LTC_frame_count/frame_tens_mux0002<5>15 ),
    .I1(\LTC_A/LTC_frame_count/frame_tens_mux0002<5>30_320 ),
    .I2(\LTC_A/LTC_frame_count/frame_tens_mux0002<5>52_321 ),
    .I3(\LTC_A/LTC_frame_count/frame_tens_mux0002<5>57_322 ),
    .O(\LTC_A/LTC_frame_count/frame_tens_mux0002<5>127_SW1 )
  );
  LUT2 #(
    .INIT ( 4'h2 ))
  \LTC_A/LTC_frame_count/frame_tens_mux0002<5>127_SW12  (
    .I0(\LTC_A/LTC_frame_count/frame_tens [0]),
    .I1(\LTC_A/LTC_frame_count/N5 ),
    .O(\LTC_A/LTC_frame_count/frame_tens_mux0002<5>127_SW11_316 )
  );
  MUXF5   \LTC_A/LTC_frame_count/frame_tens_mux0002<5>127_SW1_f5  (
    .I0(\LTC_A/LTC_frame_count/frame_tens_mux0002<5>127_SW11_316 ),
    .I1(\LTC_A/LTC_frame_count/frame_tens_mux0002<5>127_SW1 ),
    .S(\LTC_A/LTC_frame_count/frame_tens [1]),
    .O(N356)
  );
  LUT2_D #(
    .INIT ( 4'h1 ))
  \OCXO_pll/phase_loop/dac_word_o<0>21  (
    .I0(\system_control[4] ),
    .I1(\OCXO_pll/phase_loop/out_sum [27]),
    .LO(N396),
    .O(\OCXO_pll/phase_loop/N2 )
  );
  LUT4_L #(
    .INIT ( 16'h88C8 ))
  \OCXO_pll/phase_loop/dac_word_o<0>11_SW1  (
    .I0(N154),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I2(N246),
    .I3(\OCXO_pll/phase_loop/out_sum [27]),
    .LO(N220)
  );
  LUT4_L #(
    .INIT ( 16'hAACA ))
  \OCXO_pll/phase_loop/dac_word_o<0>11_SW13  (
    .I0(N248),
    .I1(N249),
    .I2(\OCXO_pll/phase_loop/out_sum [15]),
    .I3(\OCXO_pll/phase_loop/out_sum [27]),
    .LO(N238)
  );
  LUT4_L #(
    .INIT ( 16'hAACA ))
  \OCXO_pll/phase_loop/dac_word_o<0>11_SW15  (
    .I0(N251),
    .I1(N252),
    .I2(\OCXO_pll/phase_loop/out_sum [13]),
    .I3(\OCXO_pll/phase_loop/out_sum [27]),
    .LO(N241)
  );
  LUT4_L #(
    .INIT ( 16'hAACA ))
  \OCXO_pll/phase_loop/dac_word_o<0>11_SW17  (
    .I0(N254),
    .I1(N255),
    .I2(\OCXO_pll/phase_loop/out_sum [11]),
    .I3(\OCXO_pll/phase_loop/out_sum [27]),
    .LO(N244)
  );
  LUT4_L #(
    .INIT ( 16'hAACA ))
  \OCXO_pll/phase_loop/dac_word_o<0>11_SW3  (
    .I0(N257),
    .I1(N258),
    .I2(\OCXO_pll/phase_loop/out_sum [7]),
    .I3(\OCXO_pll/phase_loop/out_sum [27]),
    .LO(N223)
  );
  LUT4_L #(
    .INIT ( 16'hAACA ))
  \OCXO_pll/phase_loop/dac_word_o<0>11_SW5  (
    .I0(N260),
    .I1(N2611),
    .I2(\OCXO_pll/phase_loop/out_sum [6]),
    .I3(\OCXO_pll/phase_loop/out_sum [27]),
    .LO(N226)
  );
  LUT4_L #(
    .INIT ( 16'hAACA ))
  \OCXO_pll/phase_loop/dac_word_o<0>11_SW7  (
    .I0(N263),
    .I1(N264),
    .I2(\OCXO_pll/phase_loop/out_sum [5]),
    .I3(\OCXO_pll/phase_loop/out_sum [27]),
    .LO(N229)
  );
  LUT4_L #(
    .INIT ( 16'hAACA ))
  \OCXO_pll/phase_loop/dac_word_o<0>11_SW9  (
    .I0(N266),
    .I1(N267),
    .I2(\OCXO_pll/phase_loop/out_sum [4]),
    .I3(\OCXO_pll/phase_loop/out_sum [27]),
    .LO(N232)
  );
  LUT4_L #(
    .INIT ( 16'hAACA ))
  \OCXO_pll/phase_loop/dac_word_o<0>11_SW11  (
    .I0(N269),
    .I1(N270),
    .I2(\OCXO_pll/phase_loop/out_sum [2]),
    .I3(\OCXO_pll/phase_loop/out_sum [27]),
    .LO(N235)
  );
  LUT4_L #(
    .INIT ( 16'hEEF0 ))
  \OCXO_pll/phase_loop/dac_word_o<10>11_SW1  (
    .I0(\dac_OCXO/shift_reg [11]),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I2(N179),
    .I3(\OCXO_pll/phase_loop/out_sum [12]),
    .LO(N207)
  );
  LUT4_L #(
    .INIT ( 16'hEEF0 ))
  \OCXO_pll/phase_loop/dac_word_o<10>11_SW2  (
    .I0(\dac_OCXO/shift_reg [9]),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I2(N182),
    .I3(\OCXO_pll/phase_loop/out_sum [10]),
    .LO(N209)
  );
  LUT4_L #(
    .INIT ( 16'hEEF0 ))
  \OCXO_pll/phase_loop/dac_word_o<10>11_SW3  (
    .I0(\dac_OCXO/shift_reg [8]),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I2(N185),
    .I3(\OCXO_pll/phase_loop/out_sum [9]),
    .LO(N211)
  );
  LUT4_L #(
    .INIT ( 16'hEEF0 ))
  \OCXO_pll/phase_loop/dac_word_o<10>11_SW4  (
    .I0(\dac_OCXO/shift_reg [7]),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I2(N188),
    .I3(\OCXO_pll/phase_loop/out_sum [8]),
    .LO(N213)
  );
  LUT4_L #(
    .INIT ( 16'hFAD8 ))
  \OCXO_pll/phase_loop/dac_word_o<10>11_SW5  (
    .I0(\OCXO_pll/phase_loop/out_sum [3]),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I2(N191),
    .I3(\dac_OCXO/shift_reg [2]),
    .LO(N215)
  );
  LUT4_L #(
    .INIT ( 16'hFAD8 ))
  \OCXO_pll/phase_loop/dac_word_o<10>11_SW6  (
    .I0(\OCXO_pll/phase_loop/out_sum [1]),
    .I1(\OCXO_pll/phase_detect/update_tick_1570 ),
    .I2(N194),
    .I3(\dac_OCXO/shift_reg [0]),
    .LO(N217)
  );
  SRL16 #(
    .INIT ( 16'h0000 ))
  \communication/Mshreg_update_line_0  (
    .A0(tp2_o_OBUF_3479),
    .A1(tp2_o_OBUF_3479),
    .A2(tp2_o_OBUF_3479),
    .A3(tp2_o_OBUF_3479),
    .CLK(clk_148),
    .D(\serial_interface/state_cmp_eq0021 ),
    .Q(\communication/Mshreg_update_line_0_2341 )
  );
  FD   \communication/update_line_0  (
    .C(clk_148),
    .D(\communication/Mshreg_update_line_0_2341 ),
    .Q(\communication/update_line [0])
  );
  INV   \LTC_B/LTC_frame_count/frame_units_mux0009<1>86_SW02_INV_0  (
    .I(\LTC_B/LTC_frame_count/frame_units [3]),
    .O(\LTC_B/LTC_frame_count/frame_units_mux0009<1>86_SW01_771 )
  );
  INV   \LTC_A/LTC_frame_count/frame_units_mux0009<1>86_SW02_INV_0  (
    .I(\LTC_A/LTC_frame_count/frame_units [3]),
    .O(\LTC_A/LTC_frame_count/frame_units_mux0009<1>86_SW01_337 )
  );
endmodule


`ifndef GLBL
`define GLBL

`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;

    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (weak1, weak0) GSR = GSR_int;
    assign (weak1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

endmodule

`endif

