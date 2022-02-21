--------------------------------------------------------------------------------
-- Copyright (c) 1995-2005 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____
--  /   /\/   /
-- /___/  \  /    Vendor: Xilinx
-- \   \   \/     Version: H.41
--  \   \         Application: netgen
--  /   /         Filename: audio_clk_synthesis.vhd
-- /___/   /\     Timestamp: Mon Mar 20 13:49:25 2006
-- \   \  /  \ 
--  \___\/\___\
--             
-- Command	: -intstyle ise -ar Structure -w -ofmt vhdl -sim audio_clk.ngc audio_clk_synthesis.vhd 
-- Device	: xc2vp20-6-ff896
-- Input file	: audio_clk.ngc
-- Output file	: audio_clk_synthesis.vhd
-- # of Entities	: 1
-- Design Name	: audio_clk
-- Xilinx	: C:/Xilinx71
--             
-- Purpose:    
--     This VHDL netlist is a verification model and uses simulation 
--     primitives which may not represent the true implementation of the 
--     device, however the netlist is functionally correct and should not 
--     be modified. This file cannot be synthesized and should only be used 
--     with supported simulation tools.
--             
-- Reference:  
--     Development System Reference Guide, Chapter 23
--     Synthesis and Verification Design Guide, Chapter 6
--             
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity audio_clk is
  port (
    sync_reset_in : in STD_LOGIC := 'X'; 
    reset_in : in STD_LOGIC := 'X'; 
    videoclk_in : in STD_LOGIC := 'X'; 
    audioclk_out : out STD_LOGIC; 
    clkmux_in : in STD_LOGIC_VECTOR ( 2 downto 0 ); 
    audioluma_out : out STD_LOGIC_VECTOR ( 9 downto 0 ); 
    audiochroma_out : out STD_LOGIC_VECTOR ( 9 downto 0 ) 
  );
end audio_clk;

architecture Structure of audio_clk is

  type STD_LOGIC_VECTOR1 is array (natural range <>) of STD_LOGIC;
  type STD_LOGIC_VECTOR2 is array (natural range <>, natural range <>) of STD_LOGIC;
  type STD_LOGIC_VECTOR3 is array (natural range <>, natural range <>, natural range <>) of STD_LOGIC;
  type STD_LOGIC_VECTOR4 is array (natural range <>, natural range <>, natural range <>, natural range <>) of STD_LOGIC;

  signal sync_reset_in_IBUF : STD_LOGIC; 
  signal reset_in_IBUF : STD_LOGIC; 
  signal audio_clk_n0256_4_cyo : STD_LOGIC; 
  signal audio_clk_n0253_21_cyo : STD_LOGIC; 
  signal videoclk_in_BUFGP : STD_LOGIC; 
  signal audioclk_out_OBUF : STD_LOGIC; 
  signal audio_clk_n0250_5_cyo : STD_LOGIC; 
  signal audio_clk_n0250_4_cyo : STD_LOGIC; 
  signal audio_clk_n0250_2_cyo : STD_LOGIC; 
  signal N146 : STD_LOGIC; 
  signal enable_in : STD_LOGIC; 
  signal audio_clk_n0250_3_cyo : STD_LOGIC; 
  signal N20 : STD_LOGIC; 
  signal h_delayed_1 : STD_LOGIC; 
  signal audio_clk_n0256_1_cyo : STD_LOGIC; 
  signal nor_cyo : STD_LOGIC; 
  signal audio_clk_n0256_10_cyo : STD_LOGIC; 
  signal Q_n0222 : STD_LOGIC; 
  signal Q_n0226 : STD_LOGIC; 
  signal audio_clk_n0250_1_cyo : STD_LOGIC; 
  signal Q_n0239 : STD_LOGIC; 
  signal N138 : STD_LOGIC; 
  signal audio_clk_n0256_2_cyo : STD_LOGIC; 
  signal audio_clk_n0453_0_cyo : STD_LOGIC; 
  signal Q_n0310 : STD_LOGIC; 
  signal XNor_stage_cyo2 : STD_LOGIC; 
  signal output_counter_buffer_1_9 : STD_LOGIC; 
  signal Q_n0262 : STD_LOGIC; 
  signal N30 : STD_LOGIC; 
  signal Q_n0263 : STD_LOGIC; 
  signal N54 : STD_LOGIC; 
  signal Q_n0264 : STD_LOGIC; 
  signal N81 : STD_LOGIC; 
  signal Q_n0265 : STD_LOGIC; 
  signal Q_n0270 : STD_LOGIC; 
  signal Q_n0266 : STD_LOGIC; 
  signal audio_clk_n0253_15_cyo : STD_LOGIC; 
  signal Q_n0267 : STD_LOGIC; 
  signal Q_n0272 : STD_LOGIC; 
  signal Q_n0268 : STD_LOGIC; 
  signal state_count_4_rt : STD_LOGIC; 
  signal Q_n0269 : STD_LOGIC; 
  signal N182 : STD_LOGIC; 
  signal Q_n0280 : STD_LOGIC; 
  signal output_counter_buffer_1_0 : STD_LOGIC; 
  signal state_count_3_rt : STD_LOGIC; 
  signal Q_n0291 : STD_LOGIC; 
  signal Q_n0292 : STD_LOGIC; 
  signal audio_clk_n0253_12_cyo : STD_LOGIC; 
  signal Q_n0293 : STD_LOGIC; 
  signal Q_n0289 : STD_LOGIC; 
  signal Q_n0294 : STD_LOGIC; 
  signal Q_n0295 : STD_LOGIC; 
  signal sample_pos_N0 : STD_LOGIC; 
  signal N4 : STD_LOGIC; 
  signal Q_n0296 : STD_LOGIC; 
  signal counter_buffer_0_9 : STD_LOGIC; 
  signal Q_n0246_5_Q : STD_LOGIC; 
  signal N186 : STD_LOGIC; 
  signal N72 : STD_LOGIC; 
  signal Q_n0405 : STD_LOGIC; 
  signal counter_buffer_0_8 : STD_LOGIC; 
  signal N221 : STD_LOGIC; 
  signal N144 : STD_LOGIC; 
  signal N218 : STD_LOGIC; 
  signal N1542 : STD_LOGIC; 
  signal N78 : STD_LOGIC; 
  signal Q_n0365 : STD_LOGIC; 
  signal Q_n0415 : STD_LOGIC; 
  signal Q_n0371 : STD_LOGIC; 
  signal Q_n0421 : STD_LOGIC; 
  signal N137 : STD_LOGIC; 
  signal Q_n0417 : STD_LOGIC; 
  signal Q_n0368 : STD_LOGIC; 
  signal And_cyo2 : STD_LOGIC; 
  signal Q_n0418 : STD_LOGIC; 
  signal Q_n0423 : STD_LOGIC; 
  signal Q_n0419 : STD_LOGIC; 
  signal Q_n0376 : STD_LOGIC; 
  signal Q_n0426 : STD_LOGIC; 
  signal counter_buffer_0_12 : STD_LOGIC; 
  signal Q_n0428 : STD_LOGIC; 
  signal audio_clk_n0453_6_cyo : STD_LOGIC; 
  signal counter_buffer_0_0 : STD_LOGIC; 
  signal counter_buffer_0_11 : STD_LOGIC; 
  signal N110 : STD_LOGIC; 
  signal counter_buffer_0_3 : STD_LOGIC; 
  signal N242 : STD_LOGIC; 
  signal output_counter_buffer_1_4 : STD_LOGIC; 
  signal N244 : STD_LOGIC; 
  signal N42 : STD_LOGIC; 
  signal audio_clk_n0253_0_cyo : STD_LOGIC; 
  signal N246 : STD_LOGIC; 
  signal audio_clk_n0253_7_cyo : STD_LOGIC; 
  signal N37 : STD_LOGIC; 
  signal N43 : STD_LOGIC; 
  signal nor_cyo2 : STD_LOGIC; 
  signal counter_buffer_0_10 : STD_LOGIC; 
  signal N1421 : STD_LOGIC; 
  signal video_count_1_rt : STD_LOGIC; 
  signal output_counter_buffer_0_8 : STD_LOGIC; 
  signal counter_buffer_2_6 : STD_LOGIC; 
  signal output_counter_buffer_0_12 : STD_LOGIC; 
  signal XNor_stage_cyo3 : STD_LOGIC; 
  signal audio_clk_n0250_0_cyo : STD_LOGIC; 
  signal audio_clk_n0253_18_cyo : STD_LOGIC; 
  signal output_counter_buffer_0_1 : STD_LOGIC; 
  signal output_counter_buffer_1_7 : STD_LOGIC; 
  signal audio_clk_n0253_16_cyo : STD_LOGIC; 
  signal audio_clk_n0453_2_cyo : STD_LOGIC; 
  signal N1540 : STD_LOGIC; 
  signal Q_n0459 : STD_LOGIC; 
  signal counter_buffer_0_7 : STD_LOGIC; 
  signal counter_buffer_2_7 : STD_LOGIC; 
  signal counter_buffer_0_1 : STD_LOGIC; 
  signal audio_clk_n0250_8_cyo : STD_LOGIC; 
  signal counter_buffer_0_2 : STD_LOGIC; 
  signal audio_clk_n0253_17_cyo : STD_LOGIC; 
  signal output_counter_buffer_0_7 : STD_LOGIC; 
  signal output_counter_buffer_1_3 : STD_LOGIC; 
  signal output_counter_buffer_0_0 : STD_LOGIC; 
  signal N39 : STD_LOGIC; 
  signal output_counter_buffer_0_2 : STD_LOGIC; 
  signal counter_buffer_2_8 : STD_LOGIC; 
  signal N55 : STD_LOGIC; 
  signal N145 : STD_LOGIC; 
  signal counter_buffer_2_5 : STD_LOGIC; 
  signal nor_cyo6 : STD_LOGIC; 
  signal output_counter_buffer_0_5 : STD_LOGIC; 
  signal counter_buffer_2_9 : STD_LOGIC; 
  signal counter_buffer_2_0 : STD_LOGIC; 
  signal audio_clk_n0256_5_cyo : STD_LOGIC; 
  signal output_counter_buffer_0_11 : STD_LOGIC; 
  signal output_counter_buffer_0_10 : STD_LOGIC; 
  signal output_counter_buffer_1_2 : STD_LOGIC; 
  signal audio_clk_n0250_9_cyo : STD_LOGIC; 
  signal N52 : STD_LOGIC; 
  signal output_counter_buffer_0_9 : STD_LOGIC; 
  signal N56 : STD_LOGIC; 
  signal counter_buffer_2_10 : STD_LOGIC; 
  signal counter_buffer_2_1 : STD_LOGIC; 
  signal audio_clk_n0256_7_cyo : STD_LOGIC; 
  signal temp_clk_counter_11_rt : STD_LOGIC; 
  signal audio_clk_n0256_15_cyo : STD_LOGIC; 
  signal counter_6_rt1 : STD_LOGIC; 
  signal counter_buffer_2_11 : STD_LOGIC; 
  signal output_counter_buffer_1_10 : STD_LOGIC; 
  signal audio_clk_n0250_7_cyo : STD_LOGIC; 
  signal And_cyo : STD_LOGIC; 
  signal counter_buffer_2_2 : STD_LOGIC; 
  signal audio_clk_n0256_0_cyo : STD_LOGIC; 
  signal counter_buffer_1_8 : STD_LOGIC; 
  signal counter_buffer_1_6 : STD_LOGIC; 
  signal audio_clk_n0250_6_cyo : STD_LOGIC; 
  signal counter_buffer_0_5 : STD_LOGIC; 
  signal audio_clk_n0453_9_cyo : STD_LOGIC; 
  signal audio_clk_n0453_5_cyo : STD_LOGIC; 
  signal aes_count_7_rt : STD_LOGIC; 
  signal counter_buffer_0_4 : STD_LOGIC; 
  signal N59 : STD_LOGIC; 
  signal counter_buffer_2_12 : STD_LOGIC; 
  signal counter_buffer_2_3 : STD_LOGIC; 
  signal XNor_stage_cyo : STD_LOGIC; 
  signal counter_buffer_2_4 : STD_LOGIC; 
  signal nor_cyo4 : STD_LOGIC; 
  signal output_counter_buffer_1_1 : STD_LOGIC; 
  signal counter_buffer_1_5 : STD_LOGIC; 
  signal audio_clk_n0453_8_cyo : STD_LOGIC; 
  signal counter_buffer_1_7 : STD_LOGIC; 
  signal N32 : STD_LOGIC; 
  signal video_count_11_rt : STD_LOGIC; 
  signal counter_buffer_1_9 : STD_LOGIC; 
  signal Q_n0246_8_Q : STD_LOGIC; 
  signal N31 : STD_LOGIC; 
  signal Q_n0246_16_Q : STD_LOGIC; 
  signal eav_tick : STD_LOGIC; 
  signal Q_n0246_15_Q : STD_LOGIC; 
  signal control_b1 : STD_LOGIC; 
  signal Q_n0246_14_Q : STD_LOGIC; 
  signal N40 : STD_LOGIC; 
  signal N53 : STD_LOGIC; 
  signal Q_n0246_17_Q : STD_LOGIC; 
  signal Q_n0246_13_Q : STD_LOGIC; 
  signal Q_n0246_18_Q : STD_LOGIC; 
  signal Q_n0246_12_Q : STD_LOGIC; 
  signal Q_n0246_2_Q : STD_LOGIC; 
  signal Q_n0246_11_Q : STD_LOGIC; 
  signal counter_13_rt1 : STD_LOGIC; 
  signal Q_n0246_10_Q : STD_LOGIC; 
  signal Q_n0246_9_Q : STD_LOGIC; 
  signal output_counter_buffer_1_6 : STD_LOGIC; 
  signal Q_n0246_21_Q : STD_LOGIC; 
  signal Q_n0246_20_Q : STD_LOGIC; 
  signal output_counter_buffer_1_11 : STD_LOGIC; 
  signal output_counter_buffer_1_5 : STD_LOGIC; 
  signal N35 : STD_LOGIC; 
  signal Q_n0246_22_Q : STD_LOGIC; 
  signal N51 : STD_LOGIC; 
  signal counter_buffer_1_10 : STD_LOGIC; 
  signal N41 : STD_LOGIC; 
  signal output_counter_buffer_0_4 : STD_LOGIC; 
  signal XNor_stage_cyo1 : STD_LOGIC; 
  signal N142 : STD_LOGIC; 
  signal Q_n0246_19_Q : STD_LOGIC; 
  signal nor_cyo3 : STD_LOGIC; 
  signal N0 : STD_LOGIC; 
  signal N248 : STD_LOGIC; 
  signal audioluma_out_6 : STD_LOGIC; 
  signal N235 : STD_LOGIC; 
  signal N237 : STD_LOGIC; 
  signal N240 : STD_LOGIC; 
  signal audiochroma_out_9 : STD_LOGIC; 
  signal audiochroma_out_8 : STD_LOGIC; 
  signal audiochroma_out_7 : STD_LOGIC; 
  signal audiochroma_out_6 : STD_LOGIC; 
  signal audiochroma_out_5 : STD_LOGIC; 
  signal audiochroma_out_4 : STD_LOGIC; 
  signal audiochroma_out_3 : STD_LOGIC; 
  signal audiochroma_out_2 : STD_LOGIC; 
  signal audiochroma_out_1 : STD_LOGIC; 
  signal audiochroma_out_0 : STD_LOGIC; 
  signal clkmux_in_2_IBUF : STD_LOGIC; 
  signal clkmux_in_1_IBUF : STD_LOGIC; 
  signal clkmux_in_0_IBUF : STD_LOGIC; 
  signal N36 : STD_LOGIC; 
  signal counter_buffer_1_0 : STD_LOGIC; 
  signal counter_buffer_1_1 : STD_LOGIC; 
  signal counter_buffer_1_2 : STD_LOGIC; 
  signal counter_buffer_1_3 : STD_LOGIC; 
  signal counter_buffer_1_4 : STD_LOGIC; 
  signal audio_clk_n0256_18_cyo : STD_LOGIC; 
  signal audio_clk_n0253_19_cyo : STD_LOGIC; 
  signal audio_clk_n0253_20_cyo : STD_LOGIC; 
  signal audio_clk_n0256_6_cyo : STD_LOGIC; 
  signal N64 : STD_LOGIC; 
  signal audio_clk_n0250_10_cyo : STD_LOGIC; 
  signal N66 : STD_LOGIC; 
  signal audio_clk_n0256_13_cyo : STD_LOGIC; 
  signal audio_clk_n0253_5_cyo : STD_LOGIC; 
  signal audio_clk_n0253_4_cyo : STD_LOGIC; 
  signal N50 : STD_LOGIC; 
  signal audio_clk_n0253_6_cyo : STD_LOGIC; 
  signal audio_clk_n0253_9_cyo : STD_LOGIC; 
  signal audio_clk_n0253_8_cyo : STD_LOGIC; 
  signal N45 : STD_LOGIC; 
  signal audio_clk_n0253_3_cyo : STD_LOGIC; 
  signal And_cyo1 : STD_LOGIC; 
  signal audio_clk_n0253_13_cyo : STD_LOGIC; 
  signal audio_clk_n0253_14_cyo : STD_LOGIC; 
  signal N143 : STD_LOGIC; 
  signal N58 : STD_LOGIC; 
  signal audio_clk_n0256_11_cyo : STD_LOGIC; 
  signal audio_clk_n0256_14_cyo : STD_LOGIC; 
  signal N65 : STD_LOGIC; 
  signal N1564 : STD_LOGIC; 
  signal N101 : STD_LOGIC; 
  signal Q_n0271_12_Q : STD_LOGIC; 
  signal Q_n0271_11_Q : STD_LOGIC; 
  signal Q_n0271_10_Q : STD_LOGIC; 
  signal Q_n0271_9_Q : STD_LOGIC; 
  signal Q_n0271_8_Q : STD_LOGIC; 
  signal Q_n0271_0_Q : STD_LOGIC; 
  signal audio_clk_n0453_7_cyo : STD_LOGIC; 
  signal N33 : STD_LOGIC; 
  signal output_counter_buffer_0_3 : STD_LOGIC; 
  signal nor_cyo7 : STD_LOGIC; 
  signal N47 : STD_LOGIC; 
  signal N48 : STD_LOGIC; 
  signal N49 : STD_LOGIC; 
  signal XNor_stage_cyo5 : STD_LOGIC; 
  signal audio_clk_n0253_2_cyo : STD_LOGIC; 
  signal counter_buffer_1_11 : STD_LOGIC; 
  signal audio_clk_n0253_10_cyo : STD_LOGIC; 
  signal audio_clk_n0253_11_cyo : STD_LOGIC; 
  signal nor_cyo1 : STD_LOGIC; 
  signal audio_clk_n0453_10_cyo : STD_LOGIC; 
  signal audio_clk_n0453_4_cyo : STD_LOGIC; 
  signal audio_clk_n0453_1_cyo : STD_LOGIC; 
  signal output_counter_buffer_0_6 : STD_LOGIC; 
  signal state_count_5_rt : STD_LOGIC; 
  signal XNor_stage_cyo4 : STD_LOGIC; 
  signal N1 : STD_LOGIC; 
  signal N34 : STD_LOGIC; 
  signal audio_clk_n0453_3_cyo : STD_LOGIC; 
  signal Q_n0246_7_Q : STD_LOGIC; 
  signal N38 : STD_LOGIC; 
  signal output_counter_buffer_1_8 : STD_LOGIC; 
  signal And_cyo3 : STD_LOGIC; 
  signal nor_cyo5 : STD_LOGIC; 
  signal N44 : STD_LOGIC; 
  signal audio_clk_n0253_1_cyo : STD_LOGIC; 
  signal counter_buffer_0_6 : STD_LOGIC; 
  signal N214 : STD_LOGIC; 
  signal audio_clk_n0256_8_cyo : STD_LOGIC; 
  signal audio_clk_n0256_12_cyo : STD_LOGIC; 
  signal N250 : STD_LOGIC; 
  signal audio_clk_n0256_9_cyo : STD_LOGIC; 
  signal N61 : STD_LOGIC; 
  signal N63 : STD_LOGIC; 
  signal audio_clk_n0256_17_cyo : STD_LOGIC; 
  signal N134 : STD_LOGIC; 
  signal N147 : STD_LOGIC; 
  signal N57 : STD_LOGIC; 
  signal N60 : STD_LOGIC; 
  signal N62 : STD_LOGIC; 
  signal audio_clk_n0256_16_cyo : STD_LOGIC; 
  signal N1401 : STD_LOGIC; 
  signal N9 : STD_LOGIC; 
  signal audio_clk_n0256_3_cyo : STD_LOGIC; 
  signal N70 : STD_LOGIC; 
  signal audio_clk_n0256_21_cyo : STD_LOGIC; 
  signal N69 : STD_LOGIC; 
  signal audio_clk_n0256_20_cyo : STD_LOGIC; 
  signal N68 : STD_LOGIC; 
  signal audio_clk_n0256_19_cyo : STD_LOGIC; 
  signal N67 : STD_LOGIC; 
  signal N10 : STD_LOGIC; 
  signal audio_clk_n0242_0_cyo : STD_LOGIC; 
  signal audio_clk_n0242_1_cyo : STD_LOGIC; 
  signal audio_clk_n0242_2_cyo : STD_LOGIC; 
  signal audio_clk_n0242_3_cyo : STD_LOGIC; 
  signal audio_clk_n0242_4_cyo : STD_LOGIC; 
  signal audio_clk_n0242_5_cyo : STD_LOGIC; 
  signal audio_clk_n0242_6_cyo : STD_LOGIC; 
  signal N216 : STD_LOGIC; 
  signal N11 : STD_LOGIC; 
  signal audio_clk_n0244_0_cyo : STD_LOGIC; 
  signal audio_clk_n0244_1_cyo : STD_LOGIC; 
  signal audio_clk_n0244_2_cyo : STD_LOGIC; 
  signal audio_clk_n0244_3_cyo : STD_LOGIC; 
  signal audio_clk_n0244_4_cyo : STD_LOGIC; 
  signal N12 : STD_LOGIC; 
  signal audio_clk_n0247_0_cyo : STD_LOGIC; 
  signal N13 : STD_LOGIC; 
  signal audio_clk_n0247_1_cyo : STD_LOGIC; 
  signal N14 : STD_LOGIC; 
  signal audio_clk_n0247_2_cyo : STD_LOGIC; 
  signal N15 : STD_LOGIC; 
  signal audio_clk_n0247_3_cyo : STD_LOGIC; 
  signal N16 : STD_LOGIC; 
  signal audio_clk_n0247_4_cyo : STD_LOGIC; 
  signal N17 : STD_LOGIC; 
  signal audio_clk_n0247_5_cyo : STD_LOGIC; 
  signal N18 : STD_LOGIC; 
  signal audio_clk_n0247_6_cyo : STD_LOGIC; 
  signal N19 : STD_LOGIC; 
  signal audio_clk_n0247_7_cyo : STD_LOGIC; 
  signal N203 : STD_LOGIC; 
  signal CHOICE182 : STD_LOGIC; 
  signal CHOICE118 : STD_LOGIC; 
  signal CHOICE97 : STD_LOGIC; 
  signal N654 : STD_LOGIC; 
  signal CHOICE48 : STD_LOGIC; 
  signal N717 : STD_LOGIC; 
  signal state_count_2_rt : STD_LOGIC; 
  signal N691 : STD_LOGIC; 
  signal N646 : STD_LOGIC; 
  signal N673 : STD_LOGIC; 
  signal video_count_2_rt : STD_LOGIC; 
  signal CHOICE65 : STD_LOGIC; 
  signal N1556 : STD_LOGIC; 
  signal CHOICE148 : STD_LOGIC; 
  signal CHOICE6 : STD_LOGIC; 
  signal N1546 : STD_LOGIC; 
  signal N701 : STD_LOGIC; 
  signal N697 : STD_LOGIC; 
  signal counter_13_rt : STD_LOGIC; 
  signal CHOICE88 : STD_LOGIC; 
  signal N653 : STD_LOGIC; 
  signal video_count_3_rt : STD_LOGIC; 
  signal CHOICE169 : STD_LOGIC; 
  signal temp_clk_counter_1_rt : STD_LOGIC; 
  signal video_count_10_rt : STD_LOGIC; 
  signal N670 : STD_LOGIC; 
  signal CHOICE13 : STD_LOGIC; 
  signal N644 : STD_LOGIC; 
  signal N640 : STD_LOGIC; 
  signal CHOICE176 : STD_LOGIC; 
  signal CHOICE62 : STD_LOGIC; 
  signal CHOICE137 : STD_LOGIC; 
  signal CHOICE164 : STD_LOGIC; 
  signal CHOICE58 : STD_LOGIC; 
  signal CHOICE163 : STD_LOGIC; 
  signal CHOICE102 : STD_LOGIC; 
  signal state_count_1_rt : STD_LOGIC; 
  signal CHOICE57 : STD_LOGIC; 
  signal N1568 : STD_LOGIC; 
  signal CHOICE37 : STD_LOGIC; 
  signal CHOICE109 : STD_LOGIC; 
  signal CHOICE130 : STD_LOGIC; 
  signal N1554 : STD_LOGIC; 
  signal CHOICE112 : STD_LOGIC; 
  signal video_count_4_rt : STD_LOGIC; 
  signal CHOICE82 : STD_LOGIC; 
  signal CHOICE175 : STD_LOGIC; 
  signal CHOICE22 : STD_LOGIC; 
  signal CHOICE126 : STD_LOGIC; 
  signal CHOICE145 : STD_LOGIC; 
  signal CHOICE123 : STD_LOGIC; 
  signal CHOICE71 : STD_LOGIC; 
  signal CHOICE183 : STD_LOGIC; 
  signal CHOICE23 : STD_LOGIC; 
  signal CHOICE105 : STD_LOGIC; 
  signal CHOICE129 : STD_LOGIC; 
  signal CHOICE91 : STD_LOGIC; 
  signal CHOICE117 : STD_LOGIC; 
  signal CHOICE188 : STD_LOGIC; 
  signal N1570 : STD_LOGIC; 
  signal CHOICE41 : STD_LOGIC; 
  signal CHOICE68 : STD_LOGIC; 
  signal CHOICE85 : STD_LOGIC; 
  signal CHOICE139 : STD_LOGIC; 
  signal CHOICE108 : STD_LOGIC; 
  signal video_count_5_rt : STD_LOGIC; 
  signal CHOICE31 : STD_LOGIC; 
  signal video_count_6_rt : STD_LOGIC; 
  signal CHOICE189 : STD_LOGIC; 
  signal CHOICE77 : STD_LOGIC; 
  signal CHOICE54 : STD_LOGIC; 
  signal CHOICE51 : STD_LOGIC; 
  signal CHOICE96 : STD_LOGIC; 
  signal CHOICE40 : STD_LOGIC; 
  signal CHOICE34 : STD_LOGIC; 
  signal CHOICE151 : STD_LOGIC; 
  signal video_count_7_rt : STD_LOGIC; 
  signal CHOICE158 : STD_LOGIC; 
  signal CHOICE172 : STD_LOGIC; 
  signal video_count_9_rt : STD_LOGIC; 
  signal CHOICE377 : STD_LOGIC; 
  signal CHOICE393 : STD_LOGIC; 
  signal counter_0_rt1 : STD_LOGIC; 
  signal CHOICE270 : STD_LOGIC; 
  signal video_count_8_rt : STD_LOGIC; 
  signal CHOICE281 : STD_LOGIC; 
  signal CHOICE194 : STD_LOGIC; 
  signal CHOICE259 : STD_LOGIC; 
  signal CHOICE269 : STD_LOGIC; 
  signal CHOICE357 : STD_LOGIC; 
  signal N1550 : STD_LOGIC; 
  signal CHOICE232 : STD_LOGIC; 
  signal N1560 : STD_LOGIC; 
  signal CHOICE233 : STD_LOGIC; 
  signal CHOICE274 : STD_LOGIC; 
  signal CHOICE264 : STD_LOGIC; 
  signal CHOICE363 : STD_LOGIC; 
  signal N1552 : STD_LOGIC; 
  signal CHOICE212 : STD_LOGIC; 
  signal CHOICE298 : STD_LOGIC; 
  signal CHOICE245 : STD_LOGIC; 
  signal CHOICE199 : STD_LOGIC; 
  signal CHOICE418 : STD_LOGIC; 
  signal CHOICE251 : STD_LOGIC; 
  signal CHOICE215 : STD_LOGIC; 
  signal CHOICE416 : STD_LOGIC; 
  signal N1572 : STD_LOGIC; 
  signal CHOICE366 : STD_LOGIC; 
  signal CHOICE313 : STD_LOGIC; 
  signal CHOICE334 : STD_LOGIC; 
  signal CHOICE350 : STD_LOGIC; 
  signal N1548 : STD_LOGIC; 
  signal N1574 : STD_LOGIC; 
  signal CHOICE316 : STD_LOGIC; 
  signal temp_clk_counter_3_rt : STD_LOGIC; 
  signal CHOICE206 : STD_LOGIC; 
  signal CHOICE347 : STD_LOGIC; 
  signal temp_clk_counter_2_rt : STD_LOGIC; 
  signal CHOICE282 : STD_LOGIC; 
  signal CHOICE344 : STD_LOGIC; 
  signal CHOICE406 : STD_LOGIC; 
  signal CHOICE219 : STD_LOGIC; 
  signal CHOICE208 : STD_LOGIC; 
  signal aes_count_6_rt : STD_LOGIC; 
  signal CHOICE286 : STD_LOGIC; 
  signal N1558 : STD_LOGIC; 
  signal CHOICE273 : STD_LOGIC; 
  signal CHOICE362 : STD_LOGIC; 
  signal N1566 : STD_LOGIC; 
  signal N1544 : STD_LOGIC; 
  signal CHOICE445 : STD_LOGIC; 
  signal CHOICE481 : STD_LOGIC; 
  signal CHOICE488 : STD_LOGIC; 
  signal CHOICE470 : STD_LOGIC; 
  signal CHOICE452 : STD_LOGIC; 
  signal CHOICE425 : STD_LOGIC; 
  signal CHOICE517 : STD_LOGIC; 
  signal CHOICE506 : STD_LOGIC; 
  signal CHOICE508 : STD_LOGIC; 
  signal CHOICE434 : STD_LOGIC; 
  signal counter_4_rt1 : STD_LOGIC; 
  signal CHOICE427 : STD_LOGIC; 
  signal CHOICE436 : STD_LOGIC; 
  signal CHOICE443 : STD_LOGIC; 
  signal CHOICE497 : STD_LOGIC; 
  signal N1513 : STD_LOGIC; 
  signal CHOICE490 : STD_LOGIC; 
  signal CHOICE461 : STD_LOGIC; 
  signal CHOICE527 : STD_LOGIC; 
  signal CHOICE454 : STD_LOGIC; 
  signal CHOICE479 : STD_LOGIC; 
  signal CHOICE463 : STD_LOGIC; 
  signal CHOICE515 : STD_LOGIC; 
  signal CHOICE472 : STD_LOGIC; 
  signal CHOICE499 : STD_LOGIC; 
  signal temp_clk_counter_10_rt : STD_LOGIC; 
  signal temp_clk_counter_9_rt : STD_LOGIC; 
  signal temp_clk_counter_8_rt : STD_LOGIC; 
  signal temp_clk_counter_7_rt : STD_LOGIC; 
  signal temp_clk_counter_4_rt : STD_LOGIC; 
  signal temp_clk_counter_6_rt : STD_LOGIC; 
  signal temp_clk_counter_5_rt : STD_LOGIC; 
  signal counter_6_rt : STD_LOGIC; 
  signal counter_4_rt : STD_LOGIC; 
  signal counter_0_rt : STD_LOGIC; 
  signal aes_count_1_rt : STD_LOGIC; 
  signal aes_count_2_rt : STD_LOGIC; 
  signal aes_count_3_rt : STD_LOGIC; 
  signal aes_count_4_rt : STD_LOGIC; 
  signal aes_count_5_rt : STD_LOGIC; 
  signal Q_n04591 : STD_LOGIC; 
  signal N1576 : STD_LOGIC; 
  signal N1577 : STD_LOGIC; 
  signal N1578 : STD_LOGIC; 
  signal N1579 : STD_LOGIC; 
  signal N1580 : STD_LOGIC; 
  signal N1581 : STD_LOGIC; 
  signal N1582 : STD_LOGIC; 
  signal N1583 : STD_LOGIC; 
  signal N1584 : STD_LOGIC; 
  signal N1585 : STD_LOGIC; 
  signal N1586 : STD_LOGIC; 
  signal N1587 : STD_LOGIC; 
  signal N1588 : STD_LOGIC; 
  signal N1589 : STD_LOGIC; 
  signal N1590 : STD_LOGIC; 
  signal N1591 : STD_LOGIC; 
  signal N1592 : STD_LOGIC; 
  signal N1593 : STD_LOGIC; 
  signal N1594 : STD_LOGIC; 
  signal N1595 : STD_LOGIC; 
  signal N1596 : STD_LOGIC; 
  signal N1597 : STD_LOGIC; 
  signal N1598 : STD_LOGIC; 
  signal audio_chroma : STD_LOGIC_VECTOR ( 9 downto 0 ); 
  signal Q_n0256 : STD_LOGIC_VECTOR ( 22 downto 0 ); 
  signal Q_n0231 : STD_LOGIC_VECTOR ( 22 downto 0 ); 
  signal Q_n0213 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0020 : STD_LOGIC_VECTOR ( 12 downto 0 ); 
  signal counter : STD_LOGIC_VECTOR ( 22 downto 0 ); 
  signal Q_n0214 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal current_state : STD_LOGIC_VECTOR ( 12 downto 0 ); 
  signal video_count : STD_LOGIC_VECTOR ( 11 downto 0 ); 
  signal ecc5 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0219 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0218 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Mxor_n0125_Xo : STD_LOGIC_VECTOR ( 5 downto 4 ); 
  signal interval : STD_LOGIC_VECTOR ( 6 downto 6 ); 
  signal Q_n0233 : STD_LOGIC_VECTOR ( 12 downto 0 ); 
  signal ecc3 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0240 : STD_LOGIC_VECTOR ( 11 downto 0 ); 
  signal Q_n0229 : STD_LOGIC_VECTOR ( 11 downto 0 ); 
  signal Q_n0217 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0216 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0250 : STD_LOGIC_VECTOR ( 11 downto 1 ); 
  signal Q_n0232 : STD_LOGIC_VECTOR ( 9 downto 0 ); 
  signal Mxor_n0055_Xo : STD_LOGIC_VECTOR ( 5 downto 4 ); 
  signal Q_n0253 : STD_LOGIC_VECTOR ( 22 downto 1 ); 
  signal Q_n0228 : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal Q_n0230 : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal cs_chroma : STD_LOGIC_VECTOR ( 8 downto 0 ); 
  signal Q_n0225 : STD_LOGIC_VECTOR ( 5 downto 0 ); 
  signal Q_n0238 : STD_LOGIC_VECTOR ( 12 downto 0 ); 
  signal state_count : STD_LOGIC_VECTOR ( 5 downto 0 ); 
  signal Q_n0244 : STD_LOGIC_VECTOR ( 5 downto 1 ); 
  signal Q_n0224 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal Q_n0223 : STD_LOGIC_VECTOR ( 11 downto 0 ); 
  signal fvh_i : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal Q_n0234 : STD_LOGIC_VECTOR ( 11 downto 0 ); 
  signal ecc2 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal ecc4 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0453 : STD_LOGIC_VECTOR ( 11 downto 1 ); 
  signal Q_n0236 : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal Mxor_n0119_Xo : STD_LOGIC_VECTOR ( 5 downto 4 ); 
  signal sample_pos : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal Q_n0237 : STD_LOGIC_VECTOR ( 8 downto 0 ); 
  signal audio_chan : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal ecc1 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal temp_clk_counter : STD_LOGIC_VECTOR ( 11 downto 0 ); 
  signal Mxor_n0022_Xo : STD_LOGIC_VECTOR ( 5 downto 4 ); 
  signal Q_n0220 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal ecc0 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0235 : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal Mxor_n0128_Xo : STD_LOGIC_VECTOR ( 5 downto 4 ); 
  signal output_counter_buffer : STD_LOGIC_VECTOR2 ( 2 downto 2 , 0 downto 0 ); 
  signal sample_per_line : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal Mxor_n0122_Xo : STD_LOGIC_VECTOR ( 5 downto 4 ); 
  signal Mxor_n0116_Xo : STD_LOGIC_VECTOR ( 5 downto 4 ); 
  signal Q_n0026 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal Q_n0056 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal Q_n0120 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal Q_n0247 : STD_LOGIC_VECTOR ( 8 downto 1 ); 
  signal Q_n0117 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal Q_n0123 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal Q_n0126 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal Q_n0132 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal Q_n0129 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal aes_count : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0242 : STD_LOGIC_VECTOR ( 7 downto 1 ); 
  signal next_aes_count : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal audio_group : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal Mxor_n0131_Xo : STD_LOGIC_VECTOR ( 5 downto 4 ); 
  signal Q_n0023 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal Q_n0448 : STD_LOGIC_VECTOR ( 1 downto 1 ); 
  signal index : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal Q_n0210 : STD_LOGIC_VECTOR ( 12 downto 0 ); 
  signal next_state : STD_LOGIC_VECTOR ( 12 downto 0 ); 
  signal Q_n0211 : STD_LOGIC_VECTOR ( 12 downto 0 ); 
begin
  XST_GND : GND
    port map (
      G => output_counter_buffer(2, 0)
    );
  Q_n0238_2_23 : LUT4_L
    generic map(
      INIT => X"3222"
    )
    port map (
      I0 => CHOICE418,
      I1 => sync_reset_in_IBUF,
      I2 => N72,
      I3 => counter_buffer_2_2,
      LO => Q_n0238(2)
    );
  audio_clk_n0256_4_xor : XORCY
    port map (
      CI => audio_clk_n0256_3_cyo,
      LI => counter_4_rt,
      O => Q_n0256(4)
    );
  Q_n0238_3_23 : LUT4_L
    generic map(
      INIT => X"3222"
    )
    port map (
      I0 => CHOICE427,
      I1 => sync_reset_in_IBUF,
      I2 => N72,
      I3 => counter_buffer_2_3,
      LO => Q_n0238(3)
    );
  Q_n0238_4_23 : LUT4_L
    generic map(
      INIT => X"3222"
    )
    port map (
      I0 => CHOICE436,
      I1 => sync_reset_in_IBUF,
      I2 => N72,
      I3 => counter_buffer_2_4,
      LO => Q_n0238(4)
    );
  Q_n0238_5_23 : LUT4_L
    generic map(
      INIT => X"3222"
    )
    port map (
      I0 => CHOICE445,
      I1 => sync_reset_in_IBUF,
      I2 => N72,
      I3 => counter_buffer_2_5,
      LO => Q_n0238(5)
    );
  Q_n0238_6_23 : LUT4_L
    generic map(
      INIT => X"3222"
    )
    port map (
      I0 => CHOICE454,
      I1 => sync_reset_in_IBUF,
      I2 => N72,
      I3 => counter_buffer_2_6,
      LO => Q_n0238(6)
    );
  Q_n0238_7_23 : LUT4_L
    generic map(
      INIT => X"3222"
    )
    port map (
      I0 => CHOICE472,
      I1 => sync_reset_in_IBUF,
      I2 => N72,
      I3 => counter_buffer_2_7,
      LO => Q_n0238(7)
    );
  Q_n0238_8_23 : LUT4_L
    generic map(
      INIT => X"3222"
    )
    port map (
      I0 => CHOICE463,
      I1 => sync_reset_in_IBUF,
      I2 => N72,
      I3 => counter_buffer_2_8,
      LO => Q_n0238(8)
    );
  Q_n0238_9_23 : LUT4_L
    generic map(
      INIT => X"3222"
    )
    port map (
      I0 => CHOICE481,
      I1 => sync_reset_in_IBUF,
      I2 => N72,
      I3 => counter_buffer_2_9,
      LO => Q_n0238(9)
    );
  Q_n0238_10_23 : LUT4_L
    generic map(
      INIT => X"3222"
    )
    port map (
      I0 => CHOICE499,
      I1 => sync_reset_in_IBUF,
      I2 => N72,
      I3 => counter_buffer_2_10,
      LO => Q_n0238(10)
    );
  Q_n0238_11_23 : LUT4_L
    generic map(
      INIT => X"3222"
    )
    port map (
      I0 => CHOICE490,
      I1 => sync_reset_in_IBUF,
      I2 => N72,
      I3 => counter_buffer_2_11,
      LO => Q_n0238(11)
    );
  Q_n0238_12_Q : LUT4_L
    generic map(
      INIT => X"2033"
    )
    port map (
      I0 => counter_buffer_2_12,
      I1 => sync_reset_in_IBUF,
      I2 => N1595,
      I3 => N717,
      LO => Q_n0238(12)
    );
  Q_n0229_0_2 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => temp_clk_counter(0),
      I1 => N250,
      O => Q_n0229(0)
    );
  Q_n0237_1_1 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => Q_n0247(1),
      I1 => N146,
      LO => Q_n0237(1)
    );
  Q_n0237_2_1 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => Q_n0247(2),
      I1 => N146,
      LO => Q_n0237(2)
    );
  Q_n0237_3_1 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => Q_n0247(3),
      I1 => N146,
      LO => Q_n0237(3)
    );
  Q_n0237_4_1 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => Q_n0247(4),
      I1 => N146,
      LO => Q_n0237(4)
    );
  Q_n0237_5_1 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => Q_n0247(5),
      I1 => N146,
      LO => Q_n0237(5)
    );
  Q_n0237_6_1 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => Q_n0247(6),
      I1 => N146,
      LO => Q_n0237(6)
    );
  Q_n0237_7_1 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => Q_n0247(7),
      I1 => N146,
      LO => Q_n0237(7)
    );
  Q_n0237_8_1 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => Q_n0247(8),
      I1 => N146,
      LO => Q_n0237(8)
    );
  video_count_1_rt_0 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => video_count(1),
      O => video_count_1_rt
    );
  state_count_4_rt_1 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => state_count(4),
      O => state_count_4_rt
    );
  Ker1421 : LUT3
    generic map(
      INIT => X"01"
    )
    port map (
      I0 => current_state(11),
      I1 => sync_reset_in_IBUF,
      I2 => current_state(10),
      O => N1421
    );
  Q_n0235_0_Q : LUT4_L
    generic map(
      INIT => X"0213"
    )
    port map (
      I0 => N1585,
      I1 => sync_reset_in_IBUF,
      I2 => N654,
      I3 => N653,
      LO => Q_n0235(0)
    );
  Q_n0235_1_56 : LUT4_L
    generic map(
      INIT => X"2033"
    )
    port map (
      I0 => current_state(10),
      I1 => sync_reset_in_IBUF,
      I2 => CHOICE298,
      I3 => N1558,
      LO => Q_n0235(1)
    );
  Q_n04211 : LUT4
    generic map(
      INIT => X"FF20"
    )
    port map (
      I0 => fvh_i(0),
      I1 => h_delayed_1,
      I2 => N1542,
      I3 => sync_reset_in_IBUF,
      O => Q_n0421
    );
  Q_n04211_SW0 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => enable_in,
      I1 => reset_in_IBUF,
      O => N1542
    );
  audio_clk_n0256_1_xor : XORCY
    port map (
      CI => audio_clk_n0256_0_cyo,
      LI => N52,
      O => Q_n0256(1)
    );
  Q_n0225_2_13 : LUT4
    generic map(
      INIT => X"F780"
    )
    port map (
      I0 => state_count(0),
      I1 => N1401,
      I2 => state_count(1),
      I3 => state_count(2),
      O => CHOICE406
    );
  Q_n0229_0_1 : LUT4
    generic map(
      INIT => X"FF20"
    )
    port map (
      I0 => fvh_i(0),
      I1 => h_delayed_1,
      I2 => reset_in_IBUF,
      I3 => sync_reset_in_IBUF,
      O => N250
    );
  XST_VCC : VCC
    port map (
      P => N1
    );
  Q_n0240_1_2 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => sync_reset_in_IBUF,
      I1 => N248,
      O => Q_n0240(1)
    );
  Q_n0240_1_1 : LUT4
    generic map(
      INIT => X"2373"
    )
    port map (
      I0 => index(1),
      I1 => counter_buffer_1_1,
      I2 => index(0),
      I3 => temp_clk_counter(1),
      O => N248
    );
  current_state_10 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0211(10),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => current_state(10)
    );
  Q_n0240_3_2 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => sync_reset_in_IBUF,
      I1 => N246,
      O => Q_n0240(3)
    );
  audio_clk_n0253_20_xor : XORCY
    port map (
      CI => audio_clk_n0253_19_cyo,
      LI => Q_n0246_20_Q,
      O => Q_n0253(20)
    );
  Q_n0240_3_1 : LUT4
    generic map(
      INIT => X"2373"
    )
    port map (
      I0 => index(1),
      I1 => counter_buffer_1_3,
      I2 => index(0),
      I3 => temp_clk_counter(3),
      O => N246
    );
  Q_n0240_2_2 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => sync_reset_in_IBUF,
      I1 => N244,
      O => Q_n0240(2)
    );
  Q_n0240_2_1 : LUT4
    generic map(
      INIT => X"2373"
    )
    port map (
      I0 => index(1),
      I1 => counter_buffer_1_2,
      I2 => index(0),
      I3 => temp_clk_counter(2),
      O => N244
    );
  Q_n0240_5_2 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => sync_reset_in_IBUF,
      I1 => N242,
      O => Q_n0240(5)
    );
  Q_n0240_5_1 : LUT4
    generic map(
      INIT => X"2373"
    )
    port map (
      I0 => index(1),
      I1 => counter_buffer_1_5,
      I2 => index(0),
      I3 => temp_clk_counter(5),
      O => N242
    );
  audio_clk_n0253_13_cy : MUXCY
    port map (
      CI => audio_clk_n0253_12_cyo,
      DI => output_counter_buffer(2, 0),
      S => Q_n0246_13_Q,
      O => audio_clk_n0253_13_cyo
    );
  Q_n0240_11_2 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => sync_reset_in_IBUF,
      I1 => N240,
      O => Q_n0240(11)
    );
  Q_n0240_11_1 : LUT4
    generic map(
      INIT => X"2373"
    )
    port map (
      I0 => index(1),
      I1 => counter_buffer_1_11,
      I2 => index(0),
      I3 => temp_clk_counter(11),
      O => N240
    );
  counter_4_rt1_2 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter(4),
      LO => counter_4_rt1
    );
  Q_n0240_10_2 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => sync_reset_in_IBUF,
      I1 => N237,
      O => Q_n0240(10)
    );
  Q_n0240_10_1 : LUT4
    generic map(
      INIT => X"2373"
    )
    port map (
      I0 => index(1),
      I1 => counter_buffer_1_10,
      I2 => index(0),
      I3 => temp_clk_counter(10),
      O => N237
    );
  Q_n0240_0_2 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => sync_reset_in_IBUF,
      I1 => N235,
      O => Q_n0240(0)
    );
  Q_n0240_0_1 : LUT4
    generic map(
      INIT => X"2373"
    )
    port map (
      I0 => index(1),
      I1 => counter_buffer_1_0,
      I2 => index(0),
      I3 => temp_clk_counter(0),
      O => N235
    );
  Q_n0229_9_2 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0453(9),
      I1 => N250,
      O => Q_n0229(9)
    );
  audioluma_out_6_OBUF : OBUF
    port map (
      I => audioluma_out_6,
      O => audioluma_out(6)
    );
  Q_n0229_8_2 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0453(8),
      I1 => N250,
      O => Q_n0229(8)
    );
  audioluma_out_5_OBUF : OBUF
    port map (
      I => output_counter_buffer(2, 0),
      O => audioluma_out(5)
    );
  Q_n0214_7_1 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => ecc2(7),
      I1 => current_state(10),
      I2 => current_state(11),
      I3 => sync_reset_in_IBUF,
      O => Q_n0214(7)
    );
  Q_n0214_6_1 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => ecc2(6),
      I1 => current_state(10),
      I2 => current_state(11),
      I3 => sync_reset_in_IBUF,
      O => Q_n0214(6)
    );
  Q_n0214_5_1 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => ecc2(5),
      I1 => current_state(10),
      I2 => current_state(11),
      I3 => sync_reset_in_IBUF,
      O => Q_n0214(5)
    );
  Q_n0214_4_1 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => ecc2(4),
      I1 => current_state(10),
      I2 => current_state(11),
      I3 => sync_reset_in_IBUF,
      O => Q_n0214(4)
    );
  Q_n0214_3_1 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => ecc2(3),
      I1 => current_state(10),
      I2 => current_state(11),
      I3 => sync_reset_in_IBUF,
      O => Q_n0214(3)
    );
  Q_n0214_2_1 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => ecc2(2),
      I1 => current_state(10),
      I2 => current_state(11),
      I3 => sync_reset_in_IBUF,
      O => Q_n0214(2)
    );
  Q_n0214_1_1 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => ecc2(1),
      I1 => current_state(10),
      I2 => current_state(11),
      I3 => sync_reset_in_IBUF,
      O => Q_n0214(1)
    );
  Q_n0214_0_1 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => ecc2(0),
      I1 => current_state(10),
      I2 => current_state(11),
      I3 => sync_reset_in_IBUF,
      O => Q_n0214(0)
    );
  Q_n0240_9_2 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => sync_reset_in_IBUF,
      I1 => N221,
      O => Q_n0240(9)
    );
  Q_n0240_9_1 : LUT4
    generic map(
      INIT => X"2373"
    )
    port map (
      I0 => index(1),
      I1 => counter_buffer_1_9,
      I2 => index(0),
      I3 => temp_clk_counter(9),
      O => N221
    );
  Q_n04231 : LUT4
    generic map(
      INIT => X"FCF8"
    )
    port map (
      I0 => nor_cyo7,
      I1 => enable_in,
      I2 => sync_reset_in_IBUF,
      I3 => eav_tick,
      O => Q_n0423
    );
  Q_n0240_6_2 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => sync_reset_in_IBUF,
      I1 => N218,
      O => Q_n0240(6)
    );
  Q_n0240_6_1 : LUT4
    generic map(
      INIT => X"2373"
    )
    port map (
      I0 => index(1),
      I1 => counter_buffer_1_6,
      I2 => index(0),
      I3 => temp_clk_counter(6),
      O => N218
    );
  Q_n0240_7_2 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => sync_reset_in_IBUF,
      I1 => N216,
      O => Q_n0240(7)
    );
  Q_n0240_7_1 : LUT4
    generic map(
      INIT => X"2373"
    )
    port map (
      I0 => index(1),
      I1 => counter_buffer_1_7,
      I2 => index(0),
      I3 => temp_clk_counter(7),
      O => N216
    );
  Q_n0228_1_G : LUT4
    generic map(
      INIT => X"3222"
    )
    port map (
      I0 => audio_group(1),
      I1 => sync_reset_in_IBUF,
      I2 => audio_group(0),
      I3 => current_state(10),
      O => N1577
    );
  Q_n0240_8_2 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => sync_reset_in_IBUF,
      I1 => N214,
      O => Q_n0240(8)
    );
  Q_n0240_8_1 : LUT4
    generic map(
      INIT => X"2373"
    )
    port map (
      I0 => index(1),
      I1 => counter_buffer_1_8,
      I2 => index(0),
      I3 => temp_clk_counter(8),
      O => N214
    );
  Q_n0229_7_2 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0453(7),
      I1 => N250,
      O => Q_n0229(7)
    );
  audioluma_out_4_OBUF : OBUF
    port map (
      I => output_counter_buffer(2, 0),
      O => audioluma_out(4)
    );
  Q_n0229_6_2 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0453(6),
      I1 => N250,
      O => Q_n0229(6)
    );
  audioluma_out_3_OBUF : OBUF
    port map (
      I => output_counter_buffer(2, 0),
      O => audioluma_out(3)
    );
  Q_n0229_5_2 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0453(5),
      I1 => N250,
      O => Q_n0229(5)
    );
  audioluma_out_2_OBUF : OBUF
    port map (
      I => output_counter_buffer(2, 0),
      O => audioluma_out(2)
    );
  Q_n0229_4_2 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0453(4),
      I1 => N250,
      O => Q_n0229(4)
    );
  audioluma_out_1_OBUF : OBUF
    port map (
      I => output_counter_buffer(2, 0),
      O => audioluma_out(1)
    );
  Q_n0229_3_2 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0453(3),
      I1 => N250,
      O => Q_n0229(3)
    );
  audioluma_out_0_OBUF : OBUF
    port map (
      I => output_counter_buffer(2, 0),
      O => audioluma_out(0)
    );
  Q_n0229_2_2 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0453(2),
      I1 => N250,
      O => Q_n0229(2)
    );
  audiochroma_out_9_OBUF : OBUF
    port map (
      I => audiochroma_out_9,
      O => audiochroma_out(9)
    );
  Q_n0229_1_2 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0453(1),
      I1 => N250,
      O => Q_n0229(1)
    );
  audiochroma_out_8_OBUF : OBUF
    port map (
      I => audiochroma_out_8,
      O => audiochroma_out(8)
    );
  Q_n0229_11_2 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0453(11),
      I1 => N250,
      O => Q_n0229(11)
    );
  audiochroma_out_7_OBUF : OBUF
    port map (
      I => audiochroma_out_7,
      O => audiochroma_out(7)
    );
  Q_n0229_10_2 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0453(10),
      I1 => N250,
      O => Q_n0229(10)
    );
  audiochroma_out_6_OBUF : OBUF
    port map (
      I => audiochroma_out_6,
      O => audiochroma_out(6)
    );
  interval_6_1 : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => clkmux_in_0_IBUF,
      I1 => clkmux_in_1_IBUF,
      I2 => clkmux_in_2_IBUF,
      O => interval(6)
    );
  Q_n0210_12_1_INV_0 : INV
    port map (
      I => sync_reset_in_IBUF,
      O => Q_n0210(12)
    );
  Q_n0228_0_Q : LUT4_L
    generic map(
      INIT => X"FEF4"
    )
    port map (
      I0 => audio_group(0),
      I1 => current_state(10),
      I2 => N646,
      I3 => Q_n0365,
      LO => Q_n0228(0)
    );
  video_count_9_rt_3 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => video_count(9),
      O => video_count_9_rt
    );
  Q_n0228_2_68 : LUT4_L
    generic map(
      INIT => X"3222"
    )
    port map (
      I0 => CHOICE316,
      I1 => sync_reset_in_IBUF,
      I2 => Q_n0365,
      I3 => audio_group(2),
      LO => Q_n0228(2)
    );
  Q_n0225_0_76 : LUT4_L
    generic map(
      INIT => X"3222"
    )
    port map (
      I0 => CHOICE334,
      I1 => sync_reset_in_IBUF,
      I2 => Q_n0405,
      I3 => state_count(0),
      LO => Q_n0225(0)
    );
  Q_n0272238 : LUT4
    generic map(
      INIT => X"FFBA"
    )
    port map (
      I0 => CHOICE274,
      I1 => audio_chroma(8),
      I2 => current_state(10),
      I3 => CHOICE282,
      O => CHOICE286
    );
  Ker48 : LUT3_L
    generic map(
      INIT => X"1F"
    )
    port map (
      I0 => video_count(10),
      I1 => video_count(7),
      I2 => Q_n0280,
      LO => CHOICE206
    );
  Q_n0225_3_Q : LUT4_L
    generic map(
      INIT => X"2033"
    )
    port map (
      I0 => Q_n0244(3),
      I1 => sync_reset_in_IBUF,
      I2 => N9,
      I3 => N691,
      LO => Q_n0225(3)
    );
  Q_n0225_4_Q : LUT4_L
    generic map(
      INIT => X"2033"
    )
    port map (
      I0 => Q_n0244(4),
      I1 => sync_reset_in_IBUF,
      I2 => N9,
      I3 => N697,
      LO => Q_n0225(4)
    );
  Q_n0234_5_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_1_5,
      I1 => sync_reset_in_IBUF,
      O => Q_n0234(5)
    );
  Q_n0224_0_38 : LUT4_L
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => fvh_i(0),
      I1 => N4,
      I2 => Q_n0280,
      I3 => CHOICE233,
      LO => Q_n0224(0)
    );
  sample_pos_0 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0235(0),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => sample_pos(0)
    );
  Q_n0210_11_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(11),
      I1 => sync_reset_in_IBUF,
      O => Q_n0210(11)
    );
  Q_n0237_0_1 : LUT3_L
    generic map(
      INIT => X"28"
    )
    port map (
      I0 => N1597,
      I1 => cs_chroma(0),
      I2 => audio_chroma(0),
      LO => Q_n0237(0)
    );
  Q_n0223_1_1 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => Q_n0250(1),
      I1 => N4,
      LO => Q_n0223(1)
    );
  Q_n0223_2_1 : LUT3_L
    generic map(
      INIT => X"EA"
    )
    port map (
      I0 => N1598,
      I1 => Q_n0250(2),
      I2 => N4,
      LO => Q_n0223(2)
    );
  Q_n0223_3_1 : LUT3_L
    generic map(
      INIT => X"EA"
    )
    port map (
      I0 => N81,
      I1 => Q_n0250(3),
      I2 => N4,
      LO => Q_n0223(3)
    );
  Q_n0223_4_1 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => Q_n0250(4),
      I1 => N4,
      LO => Q_n0223(4)
    );
  Q_n0223_5_1 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => Q_n0250(5),
      I1 => N4,
      LO => Q_n0223(5)
    );
  Q_n0223_6_1 : LUT3_L
    generic map(
      INIT => X"EA"
    )
    port map (
      I0 => N81,
      I1 => Q_n0250(6),
      I2 => N4,
      LO => Q_n0223(6)
    );
  Q_n0223_7_1 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => Q_n0250(7),
      I1 => N4,
      LO => Q_n0223(7)
    );
  Q_n0223_8_1 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => Q_n0250(8),
      I1 => N4,
      LO => Q_n0223(8)
    );
  Q_n0223_9_1 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => Q_n0250(9),
      I1 => N4,
      LO => Q_n0223(9)
    );
  Q_n0223_10_1 : LUT3_L
    generic map(
      INIT => X"EA"
    )
    port map (
      I0 => N81,
      I1 => Q_n0250(10),
      I2 => N4,
      LO => Q_n0223(10)
    );
  Q_n0223_11_1 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => Q_n0250(11),
      I1 => N4,
      LO => Q_n0223(11)
    );
  Q_n0210_10_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(10),
      I1 => sync_reset_in_IBUF,
      O => Q_n0210(10)
    );
  Q_n0225_5_Q : LUT4_L
    generic map(
      INIT => X"2033"
    )
    port map (
      I0 => Q_n0244(5),
      I1 => sync_reset_in_IBUF,
      I2 => N9,
      I3 => N701,
      LO => Q_n0225(5)
    );
  Mxor_n0055_Xo_5_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => audio_chroma(3),
      I1 => audio_chroma(2),
      I2 => audio_chroma(1),
      I3 => audio_chroma(0),
      O => Mxor_n0055_Xo(5)
    );
  Q_n0236_2_69 : LUT4_L
    generic map(
      INIT => X"3222"
    )
    port map (
      I0 => CHOICE366,
      I1 => sync_reset_in_IBUF,
      I2 => CHOICE377,
      I3 => current_state(8),
      LO => Q_n0236(2)
    );
  Mxor_n0055_Xo_4_1 : LUT4_L
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => audio_chroma(7),
      I1 => audio_chroma(6),
      I2 => audio_chroma(5),
      I3 => audio_chroma(4),
      LO => Mxor_n0055_Xo(4)
    );
  Ker1431 : LUT2_D
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => index(1),
      I1 => index(0),
      LO => N1582,
      O => N143
    );
  Ker0 : LUT4
    generic map(
      INIT => X"FFBA"
    )
    port map (
      I0 => N1513,
      I1 => Q_n0289,
      I2 => current_state(10),
      I3 => N1594,
      O => N0
    );
  Ker1371 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => current_state(3),
      I1 => current_state(1),
      I2 => current_state(2),
      O => N137
    );
  Mxor_n0116_Xo_5_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => ecc0(3),
      I1 => ecc0(2),
      I2 => ecc0(1),
      I3 => ecc0(0),
      O => Mxor_n0116_Xo(5)
    );
  Q_n0232_1_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => audio_chroma(1),
      I1 => sync_reset_in_IBUF,
      O => Q_n0232(1)
    );
  Mxor_n0116_Xo_4_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => ecc0(7),
      I1 => ecc0(6),
      I2 => ecc0(5),
      I3 => ecc0(4),
      O => Mxor_n0116_Xo(4)
    );
  Ker11 : LUT3
    generic map(
      INIT => X"7A"
    )
    port map (
      I0 => index(1),
      I1 => N1588,
      I2 => index(0),
      O => N110
    );
  Q_n0210_9_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(9),
      I1 => sync_reset_in_IBUF,
      O => Q_n0210(9)
    );
  Q_n0210_8_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(8),
      I1 => sync_reset_in_IBUF,
      O => Q_n0210(8)
    );
  Q_n0210_7_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(7),
      I1 => sync_reset_in_IBUF,
      O => Q_n0210(7)
    );
  Q_n0210_6_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(6),
      I1 => sync_reset_in_IBUF,
      O => Q_n0210(6)
    );
  Q_n0271_10_2 : LUT4
    generic map(
      INIT => X"0008"
    )
    port map (
      I0 => current_state(9),
      I1 => state_count(0),
      I2 => state_count(1),
      I3 => N186,
      O => Q_n0271_10_Q
    );
  Q_n0271_10_1 : LUT4
    generic map(
      INIT => X"FEFF"
    )
    port map (
      I0 => state_count(5),
      I1 => state_count(3),
      I2 => state_count(4),
      I3 => state_count(2),
      O => N186
    );
  Q_n0210_5_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(5),
      I1 => sync_reset_in_IBUF,
      O => Q_n0210(5)
    );
  Q_n0233_7_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_0_7,
      I1 => sync_reset_in_IBUF,
      O => Q_n0233(7)
    );
  Q_n0233_9_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_0_9,
      I1 => sync_reset_in_IBUF,
      O => Q_n0233(9)
    );
  Mxor_n0122_Xo_5_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => ecc2(3),
      I1 => ecc2(2),
      I2 => ecc2(1),
      I3 => ecc2(0),
      O => Mxor_n0122_Xo(5)
    );
  Q_n0233_6_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_0_6,
      I1 => sync_reset_in_IBUF,
      O => Q_n0233(6)
    );
  Mxor_n0122_Xo_4_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => ecc2(7),
      I1 => ecc2(6),
      I2 => ecc2(5),
      I3 => ecc2(4),
      O => Mxor_n0122_Xo(4)
    );
  Q_n0233_8_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_0_8,
      I1 => sync_reset_in_IBUF,
      O => Q_n0233(8)
    );
  Q_n0233_11_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_0_11,
      I1 => sync_reset_in_IBUF,
      O => Q_n0233(11)
    );
  Q_n0232_0_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => audio_chroma(0),
      I1 => sync_reset_in_IBUF,
      O => Q_n0232(0)
    );
  Q_n0210_4_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(4),
      I1 => sync_reset_in_IBUF,
      O => Q_n0210(4)
    );
  Q_n0240_4_2 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => sync_reset_in_IBUF,
      I1 => N182,
      O => Q_n0240(4)
    );
  Q_n0240_4_1 : LUT4
    generic map(
      INIT => X"2373"
    )
    port map (
      I0 => index(1),
      I1 => counter_buffer_1_4,
      I2 => index(0),
      I3 => temp_clk_counter(4),
      O => N182
    );
  Q_n0426_4 : LUT4
    generic map(
      INIT => X"FCF8"
    )
    port map (
      I0 => nor_cyo7,
      I1 => enable_in,
      I2 => sync_reset_in_IBUF,
      I3 => N1556,
      O => Q_n0426
    );
  Ker1451 : LUT4_D
    generic map(
      INIT => X"0008"
    )
    port map (
      I0 => current_state(10),
      I1 => audio_group(2),
      I2 => audio_group(0),
      I3 => audio_group(1),
      LO => N1583,
      O => N145
    );
  Q_n02391 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => fvh_i(0),
      I1 => sync_reset_in_IBUF,
      O => Q_n0239
    );
  Q_n0246_20_1 : LUT3_L
    generic map(
      INIT => X"E2"
    )
    port map (
      I0 => counter(20),
      I1 => nor_cyo7,
      I2 => Q_n0256(20),
      LO => Q_n0246_20_Q
    );
  Q_n0418_5 : LUT4
    generic map(
      INIT => X"FCF4"
    )
    port map (
      I0 => current_state(9),
      I1 => enable_in,
      I2 => sync_reset_in_IBUF,
      I3 => N640,
      O => Q_n0418
    );
  Q_n0246_19_1 : LUT3_L
    generic map(
      INIT => X"E2"
    )
    port map (
      I0 => counter(19),
      I1 => nor_cyo7,
      I2 => Q_n0256(19),
      LO => Q_n0246_19_Q
    );
  Q_n0246_18_1 : LUT3_L
    generic map(
      INIT => X"E2"
    )
    port map (
      I0 => counter(18),
      I1 => nor_cyo7,
      I2 => Q_n0256(18),
      LO => Q_n0246_18_Q
    );
  ecc2_1 : FDCE
    port map (
      D => Q_n0216(1),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc2(1)
    );
  counter_3 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(3),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(3)
    );
  counter_14 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(14),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(14)
    );
  temp_clk_counter_10 : FDCE
    port map (
      D => Q_n0229(10),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => temp_clk_counter(10)
    );
  ecc4_6 : FDCE
    port map (
      D => Q_n0218(6),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc4(6)
    );
  Q_n0246_17_1 : LUT3_L
    generic map(
      INIT => X"E2"
    )
    port map (
      I0 => counter(17),
      I1 => nor_cyo7,
      I2 => Q_n0256(17),
      LO => Q_n0246_17_Q
    );
  counter_20 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(20),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(20)
    );
  Q_n0246_16_1 : LUT3_L
    generic map(
      INIT => X"E2"
    )
    port map (
      I0 => counter(16),
      I1 => nor_cyo7,
      I2 => Q_n0256(16),
      LO => Q_n0246_16_Q
    );
  Q_n04051 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => N134,
      I1 => current_state(10),
      I2 => current_state(11),
      O => Q_n0405
    );
  Q_n0220_0_1 : LUT4_L
    generic map(
      INIT => X"5140"
    )
    port map (
      I0 => sync_reset_in_IBUF,
      I1 => aes_count(0),
      I2 => N0,
      I3 => N145,
      LO => Q_n0220(0)
    );
  audiochroma_out_4_6 : FDCE
    port map (
      D => Q_n0232(4),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audiochroma_out_4
    );
  Q_n0246_15_1 : LUT3_L
    generic map(
      INIT => X"E2"
    )
    port map (
      I0 => counter(15),
      I1 => nor_cyo7,
      I2 => Q_n0256(15),
      LO => Q_n0246_15_Q
    );
  Mxor_n0119_Xo_5_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => ecc1(3),
      I1 => ecc1(2),
      I2 => ecc1(1),
      I3 => ecc1(0),
      O => Mxor_n0119_Xo(5)
    );
  Q_n0233_10_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_0_10,
      I1 => sync_reset_in_IBUF,
      O => Q_n0233(10)
    );
  Q_n0246_14_1 : LUT3_L
    generic map(
      INIT => X"E2"
    )
    port map (
      I0 => counter(14),
      I1 => nor_cyo7,
      I2 => Q_n0256(14),
      LO => Q_n0246_14_Q
    );
  counter_4 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(4),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(4)
    );
  ecc4_5 : FDCE
    port map (
      D => Q_n0218(5),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc4(5)
    );
  state_count_4 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0225(4),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => state_count(4)
    );
  ecc4_4 : FDCE
    port map (
      D => Q_n0218(4),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc4(4)
    );
  ecc2_4 : FDCE
    port map (
      D => Q_n0216(4),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc2(4)
    );
  ecc5_5 : FDCE
    port map (
      D => Q_n0219(5),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc5(5)
    );
  counter_13_rt_7 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter(13),
      LO => counter_13_rt
    );
  state_count_2 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0225(2),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => state_count(2)
    );
  ecc4_2 : FDCE
    port map (
      D => Q_n0218(2),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc4(2)
    );
  state_count_3 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0225(3),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => state_count(3)
    );
  ecc4_3 : FDCE
    port map (
      D => Q_n0218(3),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc4(3)
    );
  ecc2_3 : FDCE
    port map (
      D => Q_n0216(3),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc2(3)
    );
  Q_n0246_13_1 : LUT3_L
    generic map(
      INIT => X"E2"
    )
    port map (
      I0 => counter(13),
      I1 => nor_cyo7,
      I2 => Q_n0256(13),
      LO => Q_n0246_13_Q
    );
  state_count_1 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0225(1),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => state_count(1)
    );
  ecc4_1 : FDCE
    port map (
      D => Q_n0218(1),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc4(1)
    );
  ecc2_2 : FDCE
    port map (
      D => Q_n0216(2),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc2(2)
    );
  state_count_0 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0225(0),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => state_count(0)
    );
  counter_9 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(9),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(9)
    );
  audio_chroma_9 : LD
    port map (
      D => Q_n0272,
      G => Q_n0459,
      Q => audio_chroma(9)
    );
  ecc4_0 : FDCE
    port map (
      D => Q_n0218(0),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc4(0)
    );
  audio_clk_n0253_16_xor : XORCY
    port map (
      CI => audio_clk_n0253_15_cyo,
      LI => Q_n0246_16_Q,
      O => Q_n0253(16)
    );
  ecc3_0 : FDCE
    port map (
      D => Q_n0217(0),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc3(0)
    );
  Q_n0246_12_1 : LUT3_L
    generic map(
      INIT => X"E2"
    )
    port map (
      I0 => counter(12),
      I1 => nor_cyo7,
      I2 => Q_n0256(12),
      LO => Q_n0246_12_Q
    );
  ecc3_3 : FDCE
    port map (
      D => Q_n0217(3),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc3(3)
    );
  next_state_10 : LD
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0271_10_Q,
      G => Q_n0459,
      Q => next_state(10)
    );
  Q_n03761 : LUT4
    generic map(
      INIT => X"0008"
    )
    port map (
      I0 => Q_n0310,
      I1 => audio_chan(2),
      I2 => audio_chan(0),
      I3 => audio_chan(1),
      O => Q_n0376
    );
  Ker1401 : LUT3_D
    generic map(
      INIT => X"01"
    )
    port map (
      I0 => state_count(5),
      I1 => state_count(3),
      I2 => state_count(4),
      LO => N1584,
      O => N1401
    );
  audiochroma_out_0_8 : FDCE
    port map (
      D => Q_n0232(0),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audiochroma_out_0
    );
  Mxor_n0022_Xo_6_1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => Mxor_n0022_Xo(4),
      I1 => Mxor_n0022_Xo(5),
      O => Q_n0023(0)
    );
  Q_n0219_4_1 : LUT3
    generic map(
      INIT => X"28"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(4),
      I2 => audio_chroma(4),
      O => Q_n0219(4)
    );
  Mxor_n0119_Xo_4_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => ecc1(7),
      I1 => ecc1(6),
      I2 => ecc1(5),
      I3 => ecc1(4),
      O => Mxor_n0119_Xo(4)
    );
  Mxor_n0128_Xo_6_1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => Mxor_n0128_Xo(4),
      I1 => Mxor_n0128_Xo(5),
      O => Q_n0129(0)
    );
  audio_clk_n0253_12_xor : XORCY
    port map (
      CI => audio_clk_n0253_11_cyo,
      LI => Q_n0246_12_Q,
      O => Q_n0253(12)
    );
  Mxor_n0131_Xo_6_1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => Mxor_n0131_Xo(4),
      I1 => Mxor_n0131_Xo(5),
      O => Q_n0132(0)
    );
  Q_n0234_2_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_1_2,
      I1 => sync_reset_in_IBUF,
      O => Q_n0234(2)
    );
  Q_n027061_SW0 : LUT4
    generic map(
      INIT => X"CCC8"
    )
    port map (
      I0 => CHOICE350,
      I1 => N144,
      I2 => CHOICE347,
      I3 => CHOICE344,
      O => N1572
    );
  Q_n0271_0_1 : LUT4
    generic map(
      INIT => X"2F22"
    )
    port map (
      I0 => current_state(10),
      I1 => Q_n0368,
      I2 => N1401,
      I3 => current_state(12),
      O => Q_n0271_0_Q
    );
  Q_n0246_11_1 : LUT3_L
    generic map(
      INIT => X"E2"
    )
    port map (
      I0 => counter(11),
      I1 => nor_cyo7,
      I2 => Q_n0256(11),
      LO => Q_n0246_11_Q
    );
  Q_n03681 : LUT4
    generic map(
      INIT => X"0090"
    )
    port map (
      I0 => sample_per_line(0),
      I1 => sample_pos(0),
      I2 => N1586,
      I3 => sample_pos(1),
      O => Q_n0368
    );
  ecc3_6 : FDCE
    port map (
      D => Q_n0217(6),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc3(6)
    );
  Q_n0246_10_1 : LUT3_L
    generic map(
      INIT => X"E2"
    )
    port map (
      I0 => counter(10),
      I1 => nor_cyo7,
      I2 => Q_n0256(10),
      LO => Q_n0246_10_Q
    );
  Q_n03651 : LUT3_D
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => N134,
      I1 => current_state(8),
      I2 => current_state(9),
      LO => N1585,
      O => Q_n0365
    );
  Q_n0246_9_1 : LUT3_L
    generic map(
      INIT => X"E2"
    )
    port map (
      I0 => counter(9),
      I1 => nor_cyo7,
      I2 => Q_n0256(9),
      LO => Q_n0246_9_Q
    );
  ecc5_6 : FDCE
    port map (
      D => Q_n0219(6),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc5(6)
    );
  counter_13 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(13),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(13)
    );
  counter_16 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(16),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(16)
    );
  audio_clk_n0253_12_cy : MUXCY
    port map (
      CI => audio_clk_n0253_11_cyo,
      DI => output_counter_buffer(2, 0),
      S => Q_n0246_12_Q,
      O => audio_clk_n0253_12_cyo
    );
  Q_n0236_0_G : LUT3_L
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => N101,
      I1 => sync_reset_in_IBUF,
      I2 => current_state(11),
      LO => N1579
    );
  Q_n03711 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => state_count(5),
      I1 => state_count(3),
      I2 => state_count(4),
      I3 => state_count(2),
      O => Q_n0371
    );
  ecc2_5 : FDCE
    port map (
      D => Q_n0216(5),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc2(5)
    );
  ecc2_6 : FDCE
    port map (
      D => Q_n0216(6),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc2(6)
    );
  counter_17 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(17),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(17)
    );
  ecc1_0 : FDCE
    port map (
      D => Q_n0214(0),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc1(0)
    );
  ecc1_1 : FDCE
    port map (
      D => Q_n0214(1),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc1(1)
    );
  ecc1_2 : FDCE
    port map (
      D => Q_n0214(2),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc1(2)
    );
  ecc1_3 : FDCE
    port map (
      D => Q_n0214(3),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc1(3)
    );
  ecc1_4 : FDCE
    port map (
      D => Q_n0214(4),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc1(4)
    );
  ecc1_5 : FDCE
    port map (
      D => Q_n0214(5),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc1(5)
    );
  ecc1_6 : FDCE
    port map (
      D => Q_n0214(6),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc1(6)
    );
  counter_15 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(15),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(15)
    );
  ecc0_0 : FDCE
    port map (
      D => Q_n0213(0),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc0(0)
    );
  ecc0_1 : FDCE
    port map (
      D => Q_n0213(1),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc0(1)
    );
  ecc0_2 : FDCE
    port map (
      D => Q_n0213(2),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc0(2)
    );
  ecc0_3 : FDCE
    port map (
      D => Q_n0213(3),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc0(3)
    );
  ecc0_4 : FDCE
    port map (
      D => Q_n0213(4),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc0(4)
    );
  ecc0_5 : FDCE
    port map (
      D => Q_n0213(5),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc0(5)
    );
  ecc0_6 : FDCE
    port map (
      D => Q_n0213(6),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc0(6)
    );
  counter_10 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(10),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(10)
    );
  counter_8 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(8),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(8)
    );
  aes_count_0 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0220(0),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => aes_count(0)
    );
  aes_count_1 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0220(1),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => aes_count(1)
    );
  aes_count_2 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0220(2),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => aes_count(2)
    );
  aes_count_3 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0220(3),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => aes_count(3)
    );
  aes_count_4 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0220(4),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => aes_count(4)
    );
  aes_count_5 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0220(5),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => aes_count(5)
    );
  aes_count_6 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0220(6),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => aes_count(6)
    );
  counter_7 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(7),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(7)
    );
  counter_6 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(6),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(6)
    );
  video_count_0 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0223(0),
      CE => enable_in,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => video_count(0)
    );
  video_count_1 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0223(1),
      CE => enable_in,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => video_count(1)
    );
  video_count_2 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0223(2),
      CE => enable_in,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => video_count(2)
    );
  video_count_3 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0223(3),
      CE => enable_in,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => video_count(3)
    );
  video_count_4 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0223(4),
      CE => enable_in,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => video_count(4)
    );
  video_count_5 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0223(5),
      CE => enable_in,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => video_count(5)
    );
  video_count_6 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0223(6),
      CE => enable_in,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => video_count(6)
    );
  video_count_7 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0223(7),
      CE => enable_in,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => video_count(7)
    );
  video_count_8 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0223(8),
      CE => enable_in,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => video_count(8)
    );
  video_count_9 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0223(9),
      CE => enable_in,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => video_count(9)
    );
  video_count_10 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0223(10),
      CE => enable_in,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => video_count(10)
    );
  counter_5 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(5),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(5)
    );
  fvh_i_0 : FDCE
    port map (
      D => Q_n0224(0),
      CE => enable_in,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => fvh_i(0)
    );
  audiochroma_out_5_9 : FDCE
    port map (
      D => Q_n0232(5),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audiochroma_out_5
    );
  video_count_8_rt_10 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => video_count(8),
      O => video_count_8_rt
    );
  aes_count_5_rt_11 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => aes_count(5),
      LO => aes_count_5_rt
    );
  ecc2_0 : FDCE
    port map (
      D => Q_n0216(0),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc2(0)
    );
  audiochroma_out_1_12 : FDCE
    port map (
      D => Q_n0232(1),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audiochroma_out_1
    );
  audio_clk_n0253_14_xor : XORCY
    port map (
      CI => audio_clk_n0253_13_cyo,
      LI => Q_n0246_14_Q,
      O => Q_n0253(14)
    );
  audiochroma_out_2_13 : FDCE
    port map (
      D => Q_n0232(2),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audiochroma_out_2
    );
  audiochroma_out_3_14 : FDCE
    port map (
      D => Q_n0232(3),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audiochroma_out_3
    );
  Q_n0233_12_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_0_12,
      I1 => sync_reset_in_IBUF,
      O => Q_n0233(12)
    );
  state_count_3_rt_15 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => state_count(3),
      O => state_count_3_rt
    );
  ecc3_1 : FDCE
    port map (
      D => Q_n0217(1),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc3(1)
    );
  ecc3_2 : FDCE
    port map (
      D => Q_n0217(2),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc3(2)
    );
  Q_n02911 : LUT3
    generic map(
      INIT => X"01"
    )
    port map (
      I0 => state_count(1),
      I1 => state_count(2),
      I2 => state_count(0),
      O => Q_n0291
    );
  Q_n0246_8_1 : LUT3_L
    generic map(
      INIT => X"D8"
    )
    port map (
      I0 => nor_cyo7,
      I1 => Q_n0256(8),
      I2 => counter(8),
      LO => Q_n0246_8_Q
    );
  Q_n0246_7_1 : LUT3_L
    generic map(
      INIT => X"D8"
    )
    port map (
      I0 => nor_cyo7,
      I1 => Q_n0256(7),
      I2 => counter(7),
      LO => Q_n0246_7_Q
    );
  Q_n02961 : LUT3
    generic map(
      INIT => X"08"
    )
    port map (
      I0 => state_count(2),
      I1 => state_count(0),
      I2 => state_count(1),
      O => Q_n0296
    );
  Q_n02951 : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => state_count(2),
      I1 => state_count(1),
      I2 => state_count(0),
      O => Q_n0295
    );
  Ker1441 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => current_state(9),
      I1 => state_count(5),
      I2 => state_count(3),
      I3 => state_count(4),
      O => N144
    );
  Q_n02931 : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => state_count(1),
      I1 => state_count(2),
      I2 => state_count(0),
      O => Q_n0293
    );
  Q_n02921 : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => state_count(0),
      I1 => state_count(2),
      I2 => state_count(1),
      O => Q_n0292
    );
  Q_n0234_3_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_1_3,
      I1 => sync_reset_in_IBUF,
      O => Q_n0234(3)
    );
  Ker1381 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => current_state(4),
      I1 => current_state(5),
      I2 => current_state(1),
      I3 => current_state(2),
      O => N138
    );
  Q_n02891 : LUT3_D
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => audio_group(2),
      I1 => audio_group(0),
      I2 => audio_group(1),
      LO => N1586,
      O => Q_n0289
    );
  Q_n0280_16 : LUT4
    generic map(
      INIT => X"0008"
    )
    port map (
      I0 => video_count(0),
      I1 => video_count(1),
      I2 => video_count(5),
      I3 => N644,
      O => Q_n0280
    );
  Q_n027925 : LUT4_D
    generic map(
      INIT => X"0800"
    )
    port map (
      I0 => next_aes_count(6),
      I1 => next_aes_count(7),
      I2 => N1544,
      I3 => CHOICE527,
      LO => N1587,
      O => Q_n0448(1)
    );
  Q_n0233_3_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_0_3,
      I1 => sync_reset_in_IBUF,
      O => Q_n0233(3)
    );
  Q_n026354 : LUT4
    generic map(
      INIT => X"1200"
    )
    port map (
      I0 => audio_group(1),
      I1 => audio_group(2),
      I2 => audio_group(0),
      I3 => current_state(3),
      O => CHOICE182
    );
  Q_n0246_5_1 : LUT3_L
    generic map(
      INIT => X"CA"
    )
    port map (
      I0 => counter(5),
      I1 => Q_n0256(5),
      I2 => nor_cyo7,
      LO => Q_n0246_5_Q
    );
  Q_n0426_SW1 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => eav_tick,
      I1 => index(1),
      I2 => index(0),
      O => N1556
    );
  Mxor_n0125_Xo_6_1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => Mxor_n0125_Xo(4),
      I1 => Mxor_n0125_Xo(5),
      O => Q_n0126(0)
    );
  Mxor_n0119_Xo_6_1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => Mxor_n0119_Xo(4),
      I1 => Mxor_n0119_Xo(5),
      O => Q_n0120(0)
    );
  Mxor_n0122_Xo_6_1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => Mxor_n0122_Xo(4),
      I1 => Mxor_n0122_Xo(5),
      O => Q_n0123(0)
    );
  Q_n0233_5_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_0_5,
      I1 => sync_reset_in_IBUF,
      O => Q_n0233(5)
    );
  aes_count_7_rt_17 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => aes_count(7),
      LO => aes_count_7_rt
    );
  audio_clk_n0253_6_lut : LUT4_L
    generic map(
      INIT => X"56A6"
    )
    port map (
      I0 => interval(6),
      I1 => counter(6),
      I2 => nor_cyo7,
      I3 => Q_n0256(6),
      LO => N51
    );
  state_count_5_rt_18 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => state_count(5),
      O => state_count_5_rt
    );
  counter_6_rt1_19 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter(6),
      LO => counter_6_rt1
    );
  Mxor_n0116_Xo_6_1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => Mxor_n0116_Xo(4),
      I1 => Mxor_n0116_Xo(5),
      O => Q_n0117(0)
    );
  Mxor_n0055_Xo_6_1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => Mxor_n0055_Xo(4),
      I1 => Mxor_n0055_Xo(5),
      O => Q_n0056(0)
    );
  audio_clk_n0256_12_xor : XORCY
    port map (
      CI => audio_clk_n0256_11_cyo,
      LI => N61,
      O => Q_n0256(12)
    );
  audio_clk_n0253_4_lut : LUT4_L
    generic map(
      INIT => X"56A6"
    )
    port map (
      I0 => interval(6),
      I1 => counter(4),
      I2 => nor_cyo7,
      I3 => Q_n0256(4),
      LO => N50
    );
  video_count_11_rt_20 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => video_count(11),
      O => video_count_11_rt
    );
  counter_13_rt1_21 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter(13),
      LO => counter_13_rt1
    );
  Q_n0210_3_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(3),
      I1 => sync_reset_in_IBUF,
      O => Q_n0210(3)
    );
  Q_n0210_2_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(2),
      I1 => sync_reset_in_IBUF,
      O => Q_n0210(2)
    );
  Q_n0210_1_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(1),
      I1 => sync_reset_in_IBUF,
      O => Q_n0210(1)
    );
  Q_n0210_0_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(0),
      I1 => sync_reset_in_IBUF,
      O => Q_n0210(0)
    );
  Q_n0211_12_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => next_state(12),
      I1 => sync_reset_in_IBUF,
      O => Q_n0211(12)
    );
  Q_n0211_11_1 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => next_state(11),
      I1 => sync_reset_in_IBUF,
      O => Q_n0211(11)
    );
  Q_n0211_10_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => next_state(10),
      I1 => sync_reset_in_IBUF,
      O => Q_n0211(10)
    );
  Q_n0211_9_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => next_state(9),
      I1 => sync_reset_in_IBUF,
      O => Q_n0211(9)
    );
  Q_n0211_8_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => next_state(8),
      I1 => sync_reset_in_IBUF,
      O => Q_n0211(8)
    );
  Q_n0211_7_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => next_state(7),
      I1 => sync_reset_in_IBUF,
      O => Q_n0211(7)
    );
  Q_n0238_12_SW0 : LUT3
    generic map(
      INIT => X"7F"
    )
    port map (
      I0 => counter_buffer_0_12,
      I1 => N110,
      I2 => nor_cyo7,
      O => N717
    );
  Q_n04171 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => enable_in,
      I1 => sync_reset_in_IBUF,
      O => Q_n0417
    );
  Q_n04191_INV_0 : INV
    port map (
      I => enable_in,
      O => Q_n0419
    );
  Q_n04151_SW0 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => enable_in,
      I1 => index(0),
      O => N1540
    );
  Q_n0238_10_8 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => CHOICE497,
      I1 => nor_cyo7,
      LO => CHOICE499
    );
  counter_0_rt1_22 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter(0),
      LO => counter_0_rt1
    );
  Q_n04592 : LUT4
    generic map(
      INIT => X"1F3F"
    )
    port map (
      I0 => state_count(2),
      I1 => N142,
      I2 => current_state(9),
      I3 => state_count(1),
      O => Q_n04591
    );
  Q_n0238_0_23 : LUT4_L
    generic map(
      INIT => X"3222"
    )
    port map (
      I0 => CHOICE508,
      I1 => sync_reset_in_IBUF,
      I2 => N72,
      I3 => counter_buffer_2_0,
      LO => Q_n0238(0)
    );
  Q_n0238_1_23 : LUT4_L
    generic map(
      INIT => X"3222"
    )
    port map (
      I0 => CHOICE517,
      I1 => sync_reset_in_IBUF,
      I2 => N72,
      I3 => counter_buffer_2_1,
      LO => Q_n0238(1)
    );
  counter_buffer_2_12_23 : FDCE
    port map (
      D => Q_n0210(12),
      CE => Q_n0415,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_2_12
    );
  current_state_3 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0211(3),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => current_state(3)
    );
  Mxor_n0125_Xo_5_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => ecc3(3),
      I1 => ecc3(2),
      I2 => ecc3(1),
      I3 => ecc3(0),
      O => Mxor_n0125_Xo(5)
    );
  ecc0_7 : FDCE
    port map (
      D => Q_n0213(7),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc0(7)
    );
  ecc1_7 : FDCE
    port map (
      D => Q_n0214(7),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc1(7)
    );
  enable_in_24 : FDC
    port map (
      D => Q_n0419,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => enable_in
    );
  ecc2_7 : FDCE
    port map (
      D => Q_n0216(7),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc2(7)
    );
  ecc3_7 : FDCE
    port map (
      D => Q_n0217(7),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc3(7)
    );
  ecc4_7 : FDCE
    port map (
      D => Q_n0218(7),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc4(7)
    );
  ecc5_7 : FDCE
    port map (
      D => Q_n0219(7),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc5(7)
    );
  aes_count_7 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0220(7),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => aes_count(7)
    );
  audio_chroma_8 : LD
    port map (
      D => Q_n0270,
      G => Q_n0459,
      Q => audio_chroma(8)
    );
  sample_per_line_0 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0222,
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => sample_per_line(0)
    );
  video_count_11 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0223(11),
      CE => enable_in,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => video_count(11)
    );
  counter_21 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(21),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(21)
    );
  state_count_5 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0225(5),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => state_count(5)
    );
  audioclk_out_25 : FDCE
    port map (
      D => Q_n0226,
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audioclk_out_OBUF
    );
  Q_n0233_2_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_0_2,
      I1 => sync_reset_in_IBUF,
      O => Q_n0233(2)
    );
  current_state_4 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0211(4),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => current_state(4)
    );
  temp_clk_counter_11 : FDCE
    port map (
      D => Q_n0229(11),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => temp_clk_counter(11)
    );
  index_1 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0230(1),
      CE => Q_n0423,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => index(1)
    );
  counter_22 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(22),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(22)
    );
  audiochroma_out_9_26 : FDCE
    port map (
      D => Q_n0232(9),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audiochroma_out_9
    );
  output_counter_buffer_0_12_27 : FDCE
    port map (
      D => Q_n0233(12),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_0_12
    );
  audio_clk_n0253_11_xor : XORCY
    port map (
      CI => audio_clk_n0253_10_cyo,
      LI => Q_n0246_11_Q,
      O => Q_n0253(11)
    );
  sample_pos_1 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0235(1),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => sample_pos(1)
    );
  current_state_5 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0211(5),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => current_state(5)
    );
  cs_chroma_8 : FDCE
    port map (
      D => Q_n0237(8),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => cs_chroma(8)
    );
  counter_buffer_0_12_28 : FDCE
    port map (
      D => Q_n0238(12),
      CE => Q_n0426,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_0_12
    );
  h_delayed_1_29 : FDCE
    port map (
      D => Q_n0239,
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => h_delayed_1
    );
  audio_clk_n0253_10_xor : XORCY
    port map (
      CI => audio_clk_n0253_9_cyo,
      LI => Q_n0246_10_Q,
      O => Q_n0253(10)
    );
  audio_clk_n0242_7_xor : XORCY
    port map (
      CI => audio_clk_n0242_6_cyo,
      LI => aes_count_7_rt,
      O => Q_n0242(7)
    );
  audio_clk_n0244_5_xor : XORCY
    port map (
      CI => audio_clk_n0244_4_cyo,
      LI => state_count_5_rt,
      O => Q_n0244(5)
    );
  audio_clk_n0247_8_xor : XORCY
    port map (
      CI => audio_clk_n0247_7_cyo,
      LI => N20,
      O => Q_n0247(8)
    );
  audio_clk_n0256_12_lut : LUT4_L
    generic map(
      INIT => X"F0B4"
    )
    port map (
      I0 => clkmux_in_2_IBUF,
      I1 => clkmux_in_0_IBUF,
      I2 => counter(12),
      I3 => clkmux_in_1_IBUF,
      LO => N61
    );
  Mxor_n0125_Xo_4_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => ecc3(7),
      I1 => ecc3(6),
      I2 => ecc3(5),
      I3 => ecc3(4),
      O => Mxor_n0125_Xo(4)
    );
  audio_clk_n0250_11_xor : XORCY
    port map (
      CI => audio_clk_n0250_10_cyo,
      LI => video_count_11_rt,
      O => Q_n0250(11)
    );
  audio_clk_n0453_11_xor : XORCY
    port map (
      CI => audio_clk_n0453_10_cyo,
      LI => temp_clk_counter_11_rt,
      O => Q_n0453(11)
    );
  audio_clk_n0256_8_cy : MUXCY
    port map (
      CI => audio_clk_n0256_7_cyo,
      DI => counter(8),
      S => N57,
      O => audio_clk_n0256_8_cyo
    );
  audio_clk_n0253_22_xor : XORCY
    port map (
      CI => audio_clk_n0253_21_cyo,
      LI => Q_n0246_22_Q,
      O => Q_n0253(22)
    );
  audio_clk_n0256_8_xor : XORCY
    port map (
      CI => audio_clk_n0256_7_cyo,
      LI => N57,
      O => Q_n0256(8)
    );
  audio_clk_n0256_9_cy : MUXCY
    port map (
      CI => audio_clk_n0256_8_cyo,
      DI => N1,
      S => N58,
      O => audio_clk_n0256_9_cyo
    );
  audio_clk_n0256_22_xor : XORCY
    port map (
      CI => audio_clk_n0256_21_cyo,
      LI => N70,
      O => Q_n0256(22)
    );
  Q_n0233_4_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_0_4,
      I1 => sync_reset_in_IBUF,
      O => Q_n0233(4)
    );
  Q_n04591_30 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => state_count(3),
      I1 => state_count(4),
      I2 => state_count(5),
      O => N142
    );
  audio_clk_n0453_10_xor : XORCY
    port map (
      CI => audio_clk_n0453_9_cyo,
      LI => temp_clk_counter_10_rt,
      O => Q_n0453(10)
    );
  audio_clk_n0453_10_cy : MUXCY
    port map (
      CI => audio_clk_n0453_9_cyo,
      DI => output_counter_buffer(2, 0),
      S => temp_clk_counter_10_rt,
      O => audio_clk_n0453_10_cyo
    );
  audio_clk_n0453_9_xor : XORCY
    port map (
      CI => audio_clk_n0453_8_cyo,
      LI => temp_clk_counter_9_rt,
      O => Q_n0453(9)
    );
  audio_clk_n0453_9_cy : MUXCY
    port map (
      CI => audio_clk_n0453_8_cyo,
      DI => output_counter_buffer(2, 0),
      S => temp_clk_counter_9_rt,
      O => audio_clk_n0453_9_cyo
    );
  audio_clk_n0453_8_xor : XORCY
    port map (
      CI => audio_clk_n0453_7_cyo,
      LI => temp_clk_counter_8_rt,
      O => Q_n0453(8)
    );
  audio_clk_n0453_8_cy : MUXCY
    port map (
      CI => audio_clk_n0453_7_cyo,
      DI => output_counter_buffer(2, 0),
      S => temp_clk_counter_8_rt,
      O => audio_clk_n0453_8_cyo
    );
  audio_clk_n0453_7_xor : XORCY
    port map (
      CI => audio_clk_n0453_6_cyo,
      LI => temp_clk_counter_7_rt,
      O => Q_n0453(7)
    );
  audio_clk_n0453_7_cy : MUXCY
    port map (
      CI => audio_clk_n0453_6_cyo,
      DI => output_counter_buffer(2, 0),
      S => temp_clk_counter_7_rt,
      O => audio_clk_n0453_7_cyo
    );
  audio_clk_n0453_6_xor : XORCY
    port map (
      CI => audio_clk_n0453_5_cyo,
      LI => temp_clk_counter_6_rt,
      O => Q_n0453(6)
    );
  audio_clk_n0453_4_cy : MUXCY
    port map (
      CI => audio_clk_n0453_3_cyo,
      DI => output_counter_buffer(2, 0),
      S => temp_clk_counter_4_rt,
      O => audio_clk_n0453_4_cyo
    );
  audio_clk_n0453_6_cy : MUXCY
    port map (
      CI => audio_clk_n0453_5_cyo,
      DI => output_counter_buffer(2, 0),
      S => temp_clk_counter_6_rt,
      O => audio_clk_n0453_6_cyo
    );
  audio_clk_n0453_5_cy : MUXCY
    port map (
      CI => audio_clk_n0453_4_cyo,
      DI => output_counter_buffer(2, 0),
      S => temp_clk_counter_5_rt,
      O => audio_clk_n0453_5_cyo
    );
  audio_clk_n0453_4_xor : XORCY
    port map (
      CI => audio_clk_n0453_3_cyo,
      LI => temp_clk_counter_4_rt,
      O => Q_n0453(4)
    );
  audio_clk_n0453_5_xor : XORCY
    port map (
      CI => audio_clk_n0453_4_cyo,
      LI => temp_clk_counter_5_rt,
      O => Q_n0453(5)
    );
  audio_chroma_0 : LD
    port map (
      D => Q_n0262,
      G => Q_n0459,
      Q => audio_chroma(0)
    );
  audio_chroma_1 : LD
    port map (
      D => Q_n0263,
      G => Q_n0459,
      Q => audio_chroma(1)
    );
  audio_chroma_2 : LD
    port map (
      D => Q_n0264,
      G => Q_n0459,
      Q => audio_chroma(2)
    );
  audio_chroma_3 : LD
    port map (
      D => Q_n0265,
      G => Q_n0459,
      Q => audio_chroma(3)
    );
  audio_chroma_4 : LD
    port map (
      D => Q_n0266,
      G => Q_n0459,
      Q => audio_chroma(4)
    );
  audio_chroma_5 : LD
    port map (
      D => Q_n0267,
      G => Q_n0459,
      Q => audio_chroma(5)
    );
  audio_chroma_6 : LD
    port map (
      D => Q_n0268,
      G => Q_n0459,
      Q => audio_chroma(6)
    );
  audio_chroma_7 : LD
    port map (
      D => Q_n0269,
      G => Q_n0459,
      Q => audio_chroma(7)
    );
  counter_2 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(2),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(2)
    );
  Q_n0234_7_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_1_7,
      I1 => sync_reset_in_IBUF,
      O => Q_n0234(7)
    );
  Q_n0234_9_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_1_9,
      I1 => sync_reset_in_IBUF,
      O => Q_n0234(9)
    );
  Mxor_n0131_Xo_5_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => ecc5(3),
      I1 => ecc5(2),
      I2 => ecc5(1),
      I3 => ecc5(0),
      O => Mxor_n0131_Xo(5)
    );
  Q_n0234_6_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_1_6,
      I1 => sync_reset_in_IBUF,
      O => Q_n0234(6)
    );
  Mxor_n0131_Xo_4_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => ecc5(7),
      I1 => ecc5(6),
      I2 => ecc5(5),
      I3 => ecc5(4),
      O => Mxor_n0131_Xo(4)
    );
  Q_n0234_8_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_1_8,
      I1 => sync_reset_in_IBUF,
      O => Q_n0234(8)
    );
  audioluma_out_6_31 : FDCE
    port map (
      D => Q_n0210(12),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audioluma_out_6
    );
  Q_n0234_11_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_1_11,
      I1 => sync_reset_in_IBUF,
      O => Q_n0234(11)
    );
  eav_tick1 : LUT4_D
    generic map(
      INIT => X"0008"
    )
    port map (
      I0 => fvh_i(0),
      I1 => reset_in_IBUF,
      I2 => sync_reset_in_IBUF,
      I3 => h_delayed_1,
      LO => N1588,
      O => eav_tick
    );
  next_aes_count_7_1 : LUT4_D
    generic map(
      INIT => X"ECA0"
    )
    port map (
      I0 => aes_count(7),
      I1 => N145,
      I2 => N0,
      I3 => Q_n0242(7),
      LO => N1589,
      O => next_aes_count(7)
    );
  next_aes_count_6_1 : LUT4
    generic map(
      INIT => X"ECA0"
    )
    port map (
      I0 => aes_count(6),
      I1 => N145,
      I2 => N0,
      I3 => Q_n0242(6),
      O => next_aes_count(6)
    );
  next_aes_count_5_1 : LUT4
    generic map(
      INIT => X"ECA0"
    )
    port map (
      I0 => aes_count(5),
      I1 => N1583,
      I2 => N0,
      I3 => Q_n0242(5),
      O => next_aes_count(5)
    );
  next_aes_count_4_1 : LUT4_D
    generic map(
      INIT => X"ECA0"
    )
    port map (
      I0 => aes_count(4),
      I1 => N145,
      I2 => N0,
      I3 => Q_n0242(4),
      LO => N1590,
      O => next_aes_count(4)
    );
  next_aes_count_3_1 : LUT4_D
    generic map(
      INIT => X"ECA0"
    )
    port map (
      I0 => aes_count(3),
      I1 => N145,
      I2 => N0,
      I3 => Q_n0242(3),
      LO => N1591,
      O => next_aes_count(3)
    );
  next_aes_count_2_1 : LUT4_D
    generic map(
      INIT => X"ECA0"
    )
    port map (
      I0 => aes_count(2),
      I1 => N145,
      I2 => N0,
      I3 => Q_n0242(2),
      LO => N1592,
      O => next_aes_count(2)
    );
  next_aes_count_1_1 : LUT4_D
    generic map(
      INIT => X"ECA0"
    )
    port map (
      I0 => aes_count(1),
      I1 => N145,
      I2 => N0,
      I3 => Q_n0242(1),
      LO => N1593,
      O => next_aes_count(1)
    );
  Q_n0246_22_1 : LUT3_L
    generic map(
      INIT => X"E2"
    )
    port map (
      I0 => counter(22),
      I1 => nor_cyo7,
      I2 => Q_n0256(22),
      LO => Q_n0246_22_Q
    );
  audiochroma_out_6_32 : FDCE
    port map (
      D => Q_n0232(6),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audiochroma_out_6
    );
  audiochroma_out_7_33 : FDCE
    port map (
      D => Q_n0232(7),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audiochroma_out_7
    );
  audiochroma_out_8_34 : FDCE
    port map (
      D => Q_n0232(8),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audiochroma_out_8
    );
  Q_n026286 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => Q_n0020(0),
      I1 => current_state(6),
      I2 => CHOICE158,
      I3 => CHOICE164,
      O => Q_n0262
    );
  audioluma_out_8_OBUF : OBUF
    port map (
      I => output_counter_buffer(2, 0),
      O => audioluma_out(8)
    );
  Q_n026390 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => Q_n0020(1),
      I1 => current_state(6),
      I2 => CHOICE183,
      I3 => CHOICE189,
      O => Q_n0263
    );
  Ker134_SW0_SW0 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => current_state(6),
      I1 => current_state(3),
      O => N1546
    );
  audioluma_out_7_OBUF : OBUF
    port map (
      I => output_counter_buffer(2, 0),
      O => audioluma_out(7)
    );
  Q_n026569 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => cs_chroma(3),
      I1 => current_state(10),
      I2 => CHOICE91,
      I3 => CHOICE97,
      O => Q_n0265
    );
  Q_n026680 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => cs_chroma(4),
      I1 => current_state(10),
      I2 => CHOICE112,
      I3 => CHOICE118,
      O => Q_n0266
    );
  Q_n026761 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => Q_n0020(5),
      I1 => current_state(6),
      I2 => N137,
      I3 => CHOICE58,
      O => Q_n0267
    );
  Q_n026876 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => current_state(10),
      I1 => cs_chroma(6),
      I2 => CHOICE139,
      I3 => N1554,
      O => Q_n0268
    );
  Q_n026961 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => Q_n0020(7),
      I1 => current_state(6),
      I2 => N137,
      I3 => CHOICE41,
      O => Q_n0269
    );
  temp_clk_counter_2_rt_35 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(2),
      O => temp_clk_counter_2_rt
    );
  Q_n0271_12_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => current_state(12),
      I1 => N1401,
      I2 => current_state(11),
      I3 => eav_tick,
      O => Q_n0271_12_Q
    );
  Q_n0271_11_1 : LUT4
    generic map(
      INIT => X"F222"
    )
    port map (
      I0 => current_state(11),
      I1 => eav_tick,
      I2 => Q_n0368,
      I3 => current_state(10),
      O => Q_n0271_11_Q
    );
  Q_n0211_6_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => next_state(6),
      I1 => sync_reset_in_IBUF,
      O => Q_n0211(6)
    );
  Q_n0271_9_Q_36 : LUT4
    generic map(
      INIT => X"F222"
    )
    port map (
      I0 => N144,
      I1 => N203,
      I2 => Q_n0376,
      I3 => current_state(8),
      O => Q_n0271_9_Q
    );
  Q_n0271_8_1 : LUT3
    generic map(
      INIT => X"F4"
    )
    port map (
      I0 => Q_n0376,
      I1 => current_state(8),
      I2 => current_state(7),
      O => Q_n0271_8_Q
    );
  Q_n0211_5_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => next_state(5),
      I1 => sync_reset_in_IBUF,
      O => Q_n0211(5)
    );
  Q_n0211_4_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => next_state(4),
      I1 => sync_reset_in_IBUF,
      O => Q_n0211(4)
    );
  Q_n0211_3_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => next_state(3),
      I1 => sync_reset_in_IBUF,
      O => Q_n0211(3)
    );
  Q_n0211_2_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => next_state(2),
      I1 => sync_reset_in_IBUF,
      O => Q_n0211(2)
    );
  Q_n0211_1_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => next_state(1),
      I1 => sync_reset_in_IBUF,
      O => Q_n0211(1)
    );
  Q_n0211_0_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => next_state(0),
      I1 => sync_reset_in_IBUF,
      O => Q_n0211(0)
    );
  Q_n02221 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => index(1),
      I1 => sync_reset_in_IBUF,
      O => Q_n0222
    );
  Q_n0234_4_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_1_4,
      I1 => sync_reset_in_IBUF,
      O => Q_n0234(4)
    );
  Q_n027080 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => Q_n0023(0),
      I1 => current_state(6),
      I2 => N78,
      I3 => CHOICE362,
      O => CHOICE363
    );
  Q_n0230_1_1 : LUT4
    generic map(
      INIT => X"1200"
    )
    port map (
      I0 => index(1),
      I1 => sync_reset_in_IBUF,
      I2 => index(0),
      I3 => nor_cyo7,
      O => Q_n0230(1)
    );
  audio_clk_n0256_6_xor : XORCY
    port map (
      CI => audio_clk_n0256_5_cyo,
      LI => counter_6_rt,
      O => Q_n0256(6)
    );
  audio_clk_n0256_7_lut_INV_0 : INV
    port map (
      I => counter(7),
      O => N56
    );
  temp_clk_counter_6 : FDCE
    port map (
      D => Q_n0229(6),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => temp_clk_counter(6)
    );
  temp_clk_counter_5 : FDCE
    port map (
      D => Q_n0229(5),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => temp_clk_counter(5)
    );
  temp_clk_counter_4 : FDCE
    port map (
      D => Q_n0229(4),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => temp_clk_counter(4)
    );
  ecc3_4 : FDCE
    port map (
      D => Q_n0217(4),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc3(4)
    );
  counter_1 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(1),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(1)
    );
  temp_clk_counter_7 : FDCE
    port map (
      D => Q_n0229(7),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => temp_clk_counter(7)
    );
  temp_clk_counter_0 : FDCE
    port map (
      D => Q_n0229(0),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => temp_clk_counter(0)
    );
  ecc3_5 : FDCE
    port map (
      D => Q_n0217(5),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc3(5)
    );
  Q_n0233_1_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_0_1,
      I1 => sync_reset_in_IBUF,
      O => Q_n0233(1)
    );
  temp_clk_counter_1 : FDCE
    port map (
      D => Q_n0229(1),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => temp_clk_counter(1)
    );
  temp_clk_counter_3 : FDCE
    port map (
      D => Q_n0229(3),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => temp_clk_counter(3)
    );
  temp_clk_counter_2 : FDCE
    port map (
      D => Q_n0229(2),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => temp_clk_counter(2)
    );
  temp_clk_counter_8 : FDCE
    port map (
      D => Q_n0229(8),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => temp_clk_counter(8)
    );
  counter_buffer_1_0_37 : FDCE
    port map (
      D => Q_n0240(0),
      CE => Q_n0428,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_1_0
    );
  counter_buffer_1_1_38 : FDCE
    port map (
      D => Q_n0240(1),
      CE => Q_n0428,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_1_1
    );
  counter_buffer_1_2_39 : FDCE
    port map (
      D => Q_n0240(2),
      CE => Q_n0428,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_1_2
    );
  counter_buffer_1_3_40 : FDCE
    port map (
      D => Q_n0240(3),
      CE => Q_n0428,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_1_3
    );
  counter_buffer_1_4_41 : FDCE
    port map (
      D => Q_n0240(4),
      CE => Q_n0428,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_1_4
    );
  counter_buffer_1_5_42 : FDCE
    port map (
      D => Q_n0240(5),
      CE => Q_n0428,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_1_5
    );
  counter_buffer_1_6_43 : FDCE
    port map (
      D => Q_n0240(6),
      CE => Q_n0428,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_1_6
    );
  counter_buffer_1_7_44 : FDCE
    port map (
      D => Q_n0240(7),
      CE => Q_n0428,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_1_7
    );
  counter_buffer_1_8_45 : FDCE
    port map (
      D => Q_n0240(8),
      CE => Q_n0428,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_1_8
    );
  counter_buffer_1_9_46 : FDCE
    port map (
      D => Q_n0240(9),
      CE => Q_n0428,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_1_9
    );
  counter_buffer_1_10_47 : FDCE
    port map (
      D => Q_n0240(10),
      CE => Q_n0428,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_1_10
    );
  counter_buffer_1_11_48 : FDCE
    port map (
      D => Q_n0240(11),
      CE => Q_n0428,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_1_11
    );
  counter_0 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(0),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(0)
    );
  counter_buffer_0_0_49 : FDCE
    port map (
      D => Q_n0238(0),
      CE => Q_n0426,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_0_0
    );
  counter_buffer_0_1_50 : FDCE
    port map (
      D => Q_n0238(1),
      CE => Q_n0426,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_0_1
    );
  counter_buffer_0_2_51 : FDCE
    port map (
      D => Q_n0238(2),
      CE => Q_n0426,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_0_2
    );
  counter_buffer_0_3_52 : FDCE
    port map (
      D => Q_n0238(3),
      CE => Q_n0426,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_0_3
    );
  counter_buffer_0_4_53 : FDCE
    port map (
      D => Q_n0238(4),
      CE => Q_n0426,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_0_4
    );
  counter_buffer_0_5_54 : FDCE
    port map (
      D => Q_n0238(5),
      CE => Q_n0426,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_0_5
    );
  counter_buffer_0_6_55 : FDCE
    port map (
      D => Q_n0238(6),
      CE => Q_n0426,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_0_6
    );
  counter_buffer_0_7_56 : FDCE
    port map (
      D => Q_n0238(7),
      CE => Q_n0426,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_0_7
    );
  counter_buffer_0_8_57 : FDCE
    port map (
      D => Q_n0238(8),
      CE => Q_n0426,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_0_8
    );
  counter_buffer_0_9_58 : FDCE
    port map (
      D => Q_n0238(9),
      CE => Q_n0426,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_0_9
    );
  counter_buffer_0_10_59 : FDCE
    port map (
      D => Q_n0238(10),
      CE => Q_n0426,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_0_10
    );
  counter_buffer_0_11_60 : FDCE
    port map (
      D => Q_n0238(11),
      CE => Q_n0426,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_0_11
    );
  index_0 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0230(0),
      CE => Q_n0423,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => index(0)
    );
  output_counter_buffer_1_0_61 : FDCE
    port map (
      D => Q_n0234(0),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_1_0
    );
  output_counter_buffer_1_1_62 : FDCE
    port map (
      D => Q_n0234(1),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_1_1
    );
  output_counter_buffer_1_2_63 : FDCE
    port map (
      D => Q_n0234(2),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_1_2
    );
  output_counter_buffer_1_3_64 : FDCE
    port map (
      D => Q_n0234(3),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_1_3
    );
  output_counter_buffer_1_4_65 : FDCE
    port map (
      D => Q_n0234(4),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_1_4
    );
  output_counter_buffer_1_5_66 : FDCE
    port map (
      D => Q_n0234(5),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_1_5
    );
  output_counter_buffer_1_6_67 : FDCE
    port map (
      D => Q_n0234(6),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_1_6
    );
  output_counter_buffer_1_7_68 : FDCE
    port map (
      D => Q_n0234(7),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_1_7
    );
  output_counter_buffer_1_8_69 : FDCE
    port map (
      D => Q_n0234(8),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_1_8
    );
  output_counter_buffer_1_9_70 : FDCE
    port map (
      D => Q_n0234(9),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_1_9
    );
  output_counter_buffer_1_10_71 : FDCE
    port map (
      D => Q_n0234(10),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_1_10
    );
  output_counter_buffer_1_11_72 : FDCE
    port map (
      D => Q_n0234(11),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_1_11
    );
  counter_19 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(19),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(19)
    );
  output_counter_buffer_0_0_73 : FDCE
    port map (
      D => Q_n0233(0),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_0_0
    );
  output_counter_buffer_0_1_74 : FDCE
    port map (
      D => Q_n0233(1),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_0_1
    );
  output_counter_buffer_0_2_75 : FDCE
    port map (
      D => Q_n0233(2),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_0_2
    );
  output_counter_buffer_0_3_76 : FDCE
    port map (
      D => Q_n0233(3),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_0_3
    );
  output_counter_buffer_0_4_77 : FDCE
    port map (
      D => Q_n0233(4),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_0_4
    );
  output_counter_buffer_0_5_78 : FDCE
    port map (
      D => Q_n0233(5),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_0_5
    );
  output_counter_buffer_0_6_79 : FDCE
    port map (
      D => Q_n0233(6),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_0_6
    );
  output_counter_buffer_0_7_80 : FDCE
    port map (
      D => Q_n0233(7),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_0_7
    );
  output_counter_buffer_0_8_81 : FDCE
    port map (
      D => Q_n0233(8),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_0_8
    );
  output_counter_buffer_0_9_82 : FDCE
    port map (
      D => Q_n0233(9),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_0_9
    );
  output_counter_buffer_0_10_83 : FDCE
    port map (
      D => Q_n0233(10),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_0_10
    );
  output_counter_buffer_0_11_84 : FDCE
    port map (
      D => Q_n0233(11),
      CE => Q_n0421,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => output_counter_buffer_0_11
    );
  temp_clk_counter_9 : FDCE
    port map (
      D => Q_n0229(9),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => temp_clk_counter(9)
    );
  counter_buffer_2_0_85 : FDCE
    port map (
      D => Q_n0210(0),
      CE => Q_n0415,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_2_0
    );
  counter_buffer_2_1_86 : FDCE
    port map (
      D => Q_n0210(1),
      CE => Q_n0415,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_2_1
    );
  counter_buffer_2_2_87 : FDCE
    port map (
      D => Q_n0210(2),
      CE => Q_n0415,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_2_2
    );
  counter_buffer_2_3_88 : FDCE
    port map (
      D => Q_n0210(3),
      CE => Q_n0415,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_2_3
    );
  counter_buffer_2_4_89 : FDCE
    port map (
      D => Q_n0210(4),
      CE => Q_n0415,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_2_4
    );
  counter_buffer_2_5_90 : FDCE
    port map (
      D => Q_n0210(5),
      CE => Q_n0415,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_2_5
    );
  counter_buffer_2_6_91 : FDCE
    port map (
      D => Q_n0210(6),
      CE => Q_n0415,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_2_6
    );
  counter_buffer_2_7_92 : FDCE
    port map (
      D => Q_n0210(7),
      CE => Q_n0415,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_2_7
    );
  counter_buffer_2_8_93 : FDCE
    port map (
      D => Q_n0210(8),
      CE => Q_n0415,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_2_8
    );
  counter_buffer_2_9_94 : FDCE
    port map (
      D => Q_n0210(9),
      CE => Q_n0415,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_2_9
    );
  counter_buffer_2_10_95 : FDCE
    port map (
      D => Q_n0210(10),
      CE => Q_n0415,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_2_10
    );
  counter_buffer_2_11_96 : FDCE
    port map (
      D => Q_n0210(11),
      CE => Q_n0415,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter_buffer_2_11
    );
  counter_18 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(18),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(18)
    );
  cs_chroma_0 : FDCE
    port map (
      D => Q_n0237(0),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => cs_chroma(0)
    );
  cs_chroma_1 : FDCE
    port map (
      D => Q_n0237(1),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => cs_chroma(1)
    );
  cs_chroma_2 : FDCE
    port map (
      D => Q_n0237(2),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => cs_chroma(2)
    );
  cs_chroma_3 : FDCE
    port map (
      D => Q_n0237(3),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => cs_chroma(3)
    );
  cs_chroma_4 : FDCE
    port map (
      D => Q_n0237(4),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => cs_chroma(4)
    );
  cs_chroma_5 : FDCE
    port map (
      D => Q_n0237(5),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => cs_chroma(5)
    );
  cs_chroma_6 : FDCE
    port map (
      D => Q_n0237(6),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => cs_chroma(6)
    );
  cs_chroma_7 : FDCE
    port map (
      D => Q_n0237(7),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => cs_chroma(7)
    );
  counter_11 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(11),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(11)
    );
  Mxor_n0128_Xo_5_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => ecc4(3),
      I1 => ecc4(2),
      I2 => ecc4(1),
      I3 => ecc4(0),
      O => Mxor_n0128_Xo(5)
    );
  Q_n0234_10_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_1_10,
      I1 => sync_reset_in_IBUF,
      O => Q_n0234(10)
    );
  Mxor_n0128_Xo_4_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => ecc4(7),
      I1 => ecc4(6),
      I2 => ecc4(5),
      I3 => ecc4(4),
      O => Mxor_n0128_Xo(4)
    );
  Q_n0233_0_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_0_0,
      I1 => sync_reset_in_IBUF,
      O => Q_n0233(0)
    );
  Ker10 : LUT4
    generic map(
      INIT => X"FFBA"
    )
    port map (
      I0 => N670,
      I1 => Q_n0310,
      I2 => current_state(8),
      I3 => N134,
      O => N101
    );
  Q_n04281 : LUT3
    generic map(
      INIT => X"EA"
    )
    port map (
      I0 => sync_reset_in_IBUF,
      I1 => nor_cyo7,
      I2 => enable_in,
      O => Q_n0428
    );
  Mxor_n0022_Xo_5_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => Q_n0020(3),
      I1 => Q_n0020(2),
      I2 => Q_n0020(1),
      I3 => Q_n0020(0),
      O => Mxor_n0022_Xo(5)
    );
  N91 : LUT4
    generic map(
      INIT => X"F222"
    )
    port map (
      I0 => current_state(8),
      I1 => Q_n0310,
      I2 => current_state(12),
      I3 => N1401,
      O => N9
    );
  counter_12 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0231(12),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => counter(12)
    );
  ecc5_0 : FDCE
    port map (
      D => Q_n0219(0),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc5(0)
    );
  ecc5_1 : FDCE
    port map (
      D => Q_n0219(1),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc5(1)
    );
  ecc5_2 : FDCE
    port map (
      D => Q_n0219(2),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc5(2)
    );
  ecc5_3 : FDCE
    port map (
      D => Q_n0219(3),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc5(3)
    );
  ecc5_4 : FDCE
    port map (
      D => Q_n0219(4),
      CE => Q_n0418,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => ecc5(4)
    );
  audio_clk_n0256_5_lut_INV_0 : INV
    port map (
      I => counter(5),
      O => N55
    );
  audio_clk_n0256_5_xor : XORCY
    port map (
      CI => audio_clk_n0256_4_cyo,
      LI => N55,
      O => Q_n0256(5)
    );
  audio_clk_n0256_6_cy : MUXCY
    port map (
      CI => audio_clk_n0256_5_cyo,
      DI => output_counter_buffer(2, 0),
      S => counter_6_rt,
      O => audio_clk_n0256_6_cyo
    );
  audio_clk_n0256_5_cy : MUXCY
    port map (
      CI => audio_clk_n0256_4_cyo,
      DI => N1,
      S => N55,
      O => audio_clk_n0256_5_cyo
    );
  audio_clk_n0256_3_cy : MUXCY
    port map (
      CI => audio_clk_n0256_2_cyo,
      DI => N1,
      S => N54,
      O => audio_clk_n0256_3_cyo
    );
  audio_clk_n0256_3_xor : XORCY
    port map (
      CI => audio_clk_n0256_2_cyo,
      LI => N54,
      O => Q_n0256(3)
    );
  audio_clk_n0256_4_cy : MUXCY
    port map (
      CI => audio_clk_n0256_3_cyo,
      DI => output_counter_buffer(2, 0),
      S => counter_4_rt,
      O => audio_clk_n0256_4_cyo
    );
  XNor_stagelut3 : LUT4_L
    generic map(
      INIT => X"1E0F"
    )
    port map (
      I0 => clkmux_in_2_IBUF,
      I1 => clkmux_in_1_IBUF,
      I2 => counter(10),
      I3 => clkmux_in_0_IBUF,
      LO => N39
    );
  audio_clk_n0256_2_xor : XORCY
    port map (
      CI => audio_clk_n0256_1_cyo,
      LI => N53,
      O => Q_n0256(2)
    );
  audio_clk_n0256_3_lut_INV_0 : INV
    port map (
      I => counter(3),
      O => N54
    );
  audio_clk_n0256_2_cy : MUXCY
    port map (
      CI => audio_clk_n0256_1_cyo,
      DI => counter(2),
      S => N53,
      O => audio_clk_n0256_2_cyo
    );
  audio_clk_n0256_0_xor : XORCY
    port map (
      CI => N1,
      LI => counter_0_rt,
      O => Q_n0256(0)
    );
  XNor_stagelut2 : LUT4_L
    generic map(
      INIT => X"1E0F"
    )
    port map (
      I0 => clkmux_in_2_IBUF,
      I1 => clkmux_in_1_IBUF,
      I2 => counter(8),
      I3 => clkmux_in_0_IBUF,
      LO => N37
    );
  audio_clk_n0256_1_cy : MUXCY
    port map (
      CI => audio_clk_n0256_0_cyo,
      DI => counter(1),
      S => N52,
      O => audio_clk_n0256_1_cyo
    );
  audio_clk_n0253_21_cy : MUXCY
    port map (
      CI => audio_clk_n0253_20_cyo,
      DI => output_counter_buffer(2, 0),
      S => Q_n0246_21_Q,
      O => audio_clk_n0253_21_cyo
    );
  audio_clk_n0256_13_xor : XORCY
    port map (
      CI => audio_clk_n0256_12_cyo,
      LI => counter_13_rt,
      O => Q_n0256(13)
    );
  audio_clk_n0256_0_cy : MUXCY
    port map (
      CI => N1,
      DI => output_counter_buffer(2, 0),
      S => counter_0_rt,
      O => audio_clk_n0256_0_cyo
    );
  audio_clk_n0253_21_xor : XORCY
    port map (
      CI => audio_clk_n0253_20_cyo,
      LI => Q_n0246_21_Q,
      O => Q_n0253(21)
    );
  audio_clk_n0253_19_cy : MUXCY
    port map (
      CI => audio_clk_n0253_18_cyo,
      DI => output_counter_buffer(2, 0),
      S => Q_n0246_19_Q,
      O => audio_clk_n0253_19_cyo
    );
  audio_clk_n0253_19_xor : XORCY
    port map (
      CI => audio_clk_n0253_18_cyo,
      LI => Q_n0246_19_Q,
      O => Q_n0253(19)
    );
  audio_clk_n0253_20_cy : MUXCY
    port map (
      CI => audio_clk_n0253_19_cyo,
      DI => output_counter_buffer(2, 0),
      S => Q_n0246_20_Q,
      O => audio_clk_n0253_20_cyo
    );
  audio_clk_n0253_17_cy : MUXCY
    port map (
      CI => audio_clk_n0253_16_cyo,
      DI => output_counter_buffer(2, 0),
      S => Q_n0246_17_Q,
      O => audio_clk_n0253_17_cyo
    );
  audio_clk_n0253_18_cy : MUXCY
    port map (
      CI => audio_clk_n0253_17_cyo,
      DI => output_counter_buffer(2, 0),
      S => Q_n0246_18_Q,
      O => audio_clk_n0253_18_cyo
    );
  audio_clk_n0253_18_xor : XORCY
    port map (
      CI => audio_clk_n0253_17_cyo,
      LI => Q_n0246_18_Q,
      O => Q_n0253(18)
    );
  audio_clk_n0253_17_xor : XORCY
    port map (
      CI => audio_clk_n0253_16_cyo,
      LI => Q_n0246_17_Q,
      O => Q_n0253(17)
    );
  audio_clk_n0253_15_cy : MUXCY
    port map (
      CI => audio_clk_n0253_14_cyo,
      DI => output_counter_buffer(2, 0),
      S => Q_n0246_15_Q,
      O => audio_clk_n0253_15_cyo
    );
  audio_clk_n0253_15_xor : XORCY
    port map (
      CI => audio_clk_n0253_14_cyo,
      LI => Q_n0246_15_Q,
      O => Q_n0253(15)
    );
  audio_clk_n0253_16_cy : MUXCY
    port map (
      CI => audio_clk_n0253_15_cyo,
      DI => output_counter_buffer(2, 0),
      S => Q_n0246_16_Q,
      O => audio_clk_n0253_16_cyo
    );
  Q_n0219_3_1 : LUT3
    generic map(
      INIT => X"28"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(3),
      I2 => audio_chroma(3),
      O => Q_n0219(3)
    );
  Q_n0219_1_1 : LUT3
    generic map(
      INIT => X"28"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(1),
      I2 => audio_chroma(1),
      O => Q_n0219(1)
    );
  Q_n0219_0_1 : LUT3
    generic map(
      INIT => X"28"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(0),
      I2 => audio_chroma(0),
      O => Q_n0219(0)
    );
  Q_n0219_2_1 : LUT3
    generic map(
      INIT => X"28"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(2),
      I2 => audio_chroma(2),
      O => Q_n0219(2)
    );
  Q_n0219_7_1 : LUT3
    generic map(
      INIT => X"28"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(7),
      I2 => audio_chroma(7),
      O => Q_n0219(7)
    );
  Q_n0219_6_1 : LUT3
    generic map(
      INIT => X"28"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(6),
      I2 => audio_chroma(6),
      O => Q_n0219(6)
    );
  Q_n0219_5_1 : LUT3
    generic map(
      INIT => X"28"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(5),
      I2 => audio_chroma(5),
      O => Q_n0219(5)
    );
  audio_clk_n0253_13_xor : XORCY
    port map (
      CI => audio_clk_n0253_12_cyo,
      LI => Q_n0246_13_Q,
      O => Q_n0253(13)
    );
  audio_clk_n0256_7_cy : MUXCY
    port map (
      CI => audio_clk_n0256_6_cyo,
      DI => N1,
      S => N56,
      O => audio_clk_n0256_7_cyo
    );
  next_state_12 : LD
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0271_12_Q,
      G => Q_n0459,
      Q => next_state(12)
    );
  next_state_11 : LD
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0271_11_Q,
      G => Q_n0459,
      Q => next_state(11)
    );
  next_state_0 : LD
    generic map(
      INIT => '1'
    )
    port map (
      D => Q_n0271_0_Q,
      G => Q_n0459,
      Q => next_state(0)
    );
  next_state_1 : LD
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state(0),
      G => Q_n0459,
      Q => next_state(1)
    );
  next_state_2 : LD
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state(1),
      G => Q_n0459,
      Q => next_state(2)
    );
  next_state_3 : LD
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state(2),
      G => Q_n0459,
      Q => next_state(3)
    );
  next_state_4 : LD
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state(3),
      G => Q_n0459,
      Q => next_state(4)
    );
  next_state_5 : LD
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state(4),
      G => Q_n0459,
      Q => next_state(5)
    );
  next_state_6 : LD
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state(5),
      G => Q_n0459,
      Q => next_state(6)
    );
  next_state_7 : LD
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state(6),
      G => Q_n0459,
      Q => next_state(7)
    );
  next_state_8 : LD
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0271_8_Q,
      G => Q_n0459,
      Q => next_state(8)
    );
  next_state_9 : LD
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0271_9_Q,
      G => Q_n0459,
      Q => next_state(9)
    );
  current_state_9 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0211(9),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => current_state(9)
    );
  current_state_8 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0211(8),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => current_state(8)
    );
  current_state_7 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0211(7),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => current_state(7)
    );
  current_state_2 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0211(2),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => current_state(2)
    );
  audio_clk_n0256_9_lut_INV_0 : INV
    port map (
      I => counter(9),
      O => N58
    );
  current_state_6 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0211(6),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => current_state(6)
    );
  audio_clk_n0253_14_cy : MUXCY
    port map (
      CI => audio_clk_n0253_13_cyo,
      DI => output_counter_buffer(2, 0),
      S => Q_n0246_14_Q,
      O => audio_clk_n0253_14_cyo
    );
  audio_clk_n0256_7_xor : XORCY
    port map (
      CI => audio_clk_n0256_6_cyo,
      LI => N56,
      O => Q_n0256(7)
    );
  audio_chan_2 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0236(2),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audio_chan(2)
    );
  audio_chan_1 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0236(1),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audio_chan(1)
    );
  audio_chan_0 : FDPE
    generic map(
      INIT => '1'
    )
    port map (
      D => Q_n0236(0),
      CE => Q_n0417,
      PRE => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audio_chan(0)
    );
  audio_group_2 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0228(2),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audio_group(2)
    );
  audio_group_1 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0228(1),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audio_group(1)
    );
  audio_group_0 : FDPE
    generic map(
      INIT => '1'
    )
    port map (
      D => Q_n0228(0),
      CE => Q_n0417,
      PRE => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => audio_group(0)
    );
  current_state_12 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0211(12),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => current_state(12)
    );
  current_state_11 : FDPE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0211(11),
      CE => Q_n0417,
      PRE => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => current_state(11)
    );
  current_state_0 : FDCE
    generic map(
      INIT => '1'
    )
    port map (
      D => Q_n0211(0),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => current_state(0)
    );
  current_state_1 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0211(1),
      CE => Q_n0417,
      CLR => sample_pos_N0,
      C => videoclk_in_BUFGP,
      Q => current_state(1)
    );
  audio_clk_n0242_0_lut_INV_0 : INV
    port map (
      I => aes_count(0),
      O => N10
    );
  audio_clk_n0242_0_cy : MUXCY
    port map (
      CI => output_counter_buffer(2, 0),
      DI => N1,
      S => N10,
      O => audio_clk_n0242_0_cyo
    );
  audiochroma_out_0_OBUF : OBUF
    port map (
      I => audiochroma_out_0,
      O => audiochroma_out(0)
    );
  audio_clk_n0242_1_cy : MUXCY
    port map (
      CI => audio_clk_n0242_0_cyo,
      DI => output_counter_buffer(2, 0),
      S => aes_count_1_rt,
      O => audio_clk_n0242_1_cyo
    );
  audio_clk_n0242_1_xor : XORCY
    port map (
      CI => audio_clk_n0242_0_cyo,
      LI => aes_count_1_rt,
      O => Q_n0242(1)
    );
  audio_clk_n0242_2_cy : MUXCY
    port map (
      CI => audio_clk_n0242_1_cyo,
      DI => output_counter_buffer(2, 0),
      S => aes_count_2_rt,
      O => audio_clk_n0242_2_cyo
    );
  audio_clk_n0242_2_xor : XORCY
    port map (
      CI => audio_clk_n0242_1_cyo,
      LI => aes_count_2_rt,
      O => Q_n0242(2)
    );
  audio_clk_n0242_3_cy : MUXCY
    port map (
      CI => audio_clk_n0242_2_cyo,
      DI => output_counter_buffer(2, 0),
      S => aes_count_3_rt,
      O => audio_clk_n0242_3_cyo
    );
  audio_clk_n0242_3_xor : XORCY
    port map (
      CI => audio_clk_n0242_2_cyo,
      LI => aes_count_3_rt,
      O => Q_n0242(3)
    );
  audio_clk_n0242_4_cy : MUXCY
    port map (
      CI => audio_clk_n0242_3_cyo,
      DI => output_counter_buffer(2, 0),
      S => aes_count_4_rt,
      O => audio_clk_n0242_4_cyo
    );
  audio_clk_n0242_4_xor : XORCY
    port map (
      CI => audio_clk_n0242_3_cyo,
      LI => aes_count_4_rt,
      O => Q_n0242(4)
    );
  audio_clk_n0242_5_cy : MUXCY
    port map (
      CI => audio_clk_n0242_4_cyo,
      DI => output_counter_buffer(2, 0),
      S => aes_count_5_rt,
      O => audio_clk_n0242_5_cyo
    );
  audio_clk_n0242_5_xor : XORCY
    port map (
      CI => audio_clk_n0242_4_cyo,
      LI => aes_count_5_rt,
      O => Q_n0242(5)
    );
  audio_clk_n0242_6_cy : MUXCY
    port map (
      CI => audio_clk_n0242_5_cyo,
      DI => output_counter_buffer(2, 0),
      S => aes_count_6_rt,
      O => audio_clk_n0242_6_cyo
    );
  audio_clk_n0242_6_xor : XORCY
    port map (
      CI => audio_clk_n0242_5_cyo,
      LI => aes_count_6_rt,
      O => Q_n0242(6)
    );
  Mxor_n0022_Xo_4_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => Q_n0020(7),
      I1 => Q_n0020(6),
      I2 => Q_n0020(5),
      I3 => Q_n0020(4),
      O => Mxor_n0022_Xo(4)
    );
  audio_clk_n0244_0_lut_INV_0 : INV
    port map (
      I => state_count(0),
      O => N11
    );
  audio_clk_n0244_0_cy : MUXCY
    port map (
      CI => output_counter_buffer(2, 0),
      DI => N1,
      S => N11,
      O => audio_clk_n0244_0_cyo
    );
  audiochroma_out_1_OBUF : OBUF
    port map (
      I => audiochroma_out_1,
      O => audiochroma_out(1)
    );
  audio_clk_n0244_1_cy : MUXCY
    port map (
      CI => audio_clk_n0244_0_cyo,
      DI => output_counter_buffer(2, 0),
      S => state_count_1_rt,
      O => audio_clk_n0244_1_cyo
    );
  audio_clk_n0244_1_xor : XORCY
    port map (
      CI => audio_clk_n0244_0_cyo,
      LI => state_count_1_rt,
      O => Q_n0244(1)
    );
  audio_clk_n0244_2_cy : MUXCY
    port map (
      CI => audio_clk_n0244_1_cyo,
      DI => output_counter_buffer(2, 0),
      S => state_count_2_rt,
      O => audio_clk_n0244_2_cyo
    );
  audio_clk_n0244_2_xor : XORCY
    port map (
      CI => audio_clk_n0244_1_cyo,
      LI => state_count_2_rt,
      O => Q_n0244(2)
    );
  audio_clk_n0244_3_cy : MUXCY
    port map (
      CI => audio_clk_n0244_2_cyo,
      DI => output_counter_buffer(2, 0),
      S => state_count_3_rt,
      O => audio_clk_n0244_3_cyo
    );
  audio_clk_n0244_3_xor : XORCY
    port map (
      CI => audio_clk_n0244_2_cyo,
      LI => state_count_3_rt,
      O => Q_n0244(3)
    );
  audio_clk_n0244_4_cy : MUXCY
    port map (
      CI => audio_clk_n0244_3_cyo,
      DI => output_counter_buffer(2, 0),
      S => state_count_4_rt,
      O => audio_clk_n0244_4_cyo
    );
  audio_clk_n0244_4_xor : XORCY
    port map (
      CI => audio_clk_n0244_3_cyo,
      LI => state_count_4_rt,
      O => Q_n0244(4)
    );
  Ker1471 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => state_count(0),
      I1 => state_count(1),
      O => N147
    );
  audio_clk_n0247_0_lut : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => cs_chroma(0),
      I1 => audio_chroma(0),
      O => N12
    );
  audio_clk_n0247_0_cy : MUXCY
    port map (
      CI => output_counter_buffer(2, 0),
      DI => cs_chroma(0),
      S => N12,
      O => audio_clk_n0247_0_cyo
    );
  audiochroma_out_2_OBUF : OBUF
    port map (
      I => audiochroma_out_2,
      O => audiochroma_out(2)
    );
  audio_clk_n0247_1_lut : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => cs_chroma(1),
      I1 => audio_chroma(1),
      O => N13
    );
  audio_clk_n0247_1_cy : MUXCY
    port map (
      CI => audio_clk_n0247_0_cyo,
      DI => cs_chroma(1),
      S => N13,
      O => audio_clk_n0247_1_cyo
    );
  audio_clk_n0247_1_xor : XORCY
    port map (
      CI => audio_clk_n0247_0_cyo,
      LI => N13,
      O => Q_n0247(1)
    );
  audio_clk_n0247_2_lut : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => cs_chroma(2),
      I1 => audio_chroma(2),
      O => N14
    );
  audio_clk_n0247_2_cy : MUXCY
    port map (
      CI => audio_clk_n0247_1_cyo,
      DI => cs_chroma(2),
      S => N14,
      O => audio_clk_n0247_2_cyo
    );
  audio_clk_n0247_2_xor : XORCY
    port map (
      CI => audio_clk_n0247_1_cyo,
      LI => N14,
      O => Q_n0247(2)
    );
  audio_clk_n0247_3_lut : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => cs_chroma(3),
      I1 => audio_chroma(3),
      O => N15
    );
  audio_clk_n0247_3_cy : MUXCY
    port map (
      CI => audio_clk_n0247_2_cyo,
      DI => cs_chroma(3),
      S => N15,
      O => audio_clk_n0247_3_cyo
    );
  audio_clk_n0247_3_xor : XORCY
    port map (
      CI => audio_clk_n0247_2_cyo,
      LI => N15,
      O => Q_n0247(3)
    );
  audio_clk_n0247_4_lut : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => cs_chroma(4),
      I1 => audio_chroma(4),
      O => N16
    );
  audio_clk_n0247_4_cy : MUXCY
    port map (
      CI => audio_clk_n0247_3_cyo,
      DI => cs_chroma(4),
      S => N16,
      O => audio_clk_n0247_4_cyo
    );
  audio_clk_n0247_4_xor : XORCY
    port map (
      CI => audio_clk_n0247_3_cyo,
      LI => N16,
      O => Q_n0247(4)
    );
  audio_clk_n0247_5_lut : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => cs_chroma(5),
      I1 => audio_chroma(5),
      O => N17
    );
  audio_clk_n0247_5_cy : MUXCY
    port map (
      CI => audio_clk_n0247_4_cyo,
      DI => cs_chroma(5),
      S => N17,
      O => audio_clk_n0247_5_cyo
    );
  audio_clk_n0247_5_xor : XORCY
    port map (
      CI => audio_clk_n0247_4_cyo,
      LI => N17,
      O => Q_n0247(5)
    );
  audio_clk_n0247_6_lut : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => cs_chroma(6),
      I1 => audio_chroma(6),
      O => N18
    );
  audio_clk_n0247_6_cy : MUXCY
    port map (
      CI => audio_clk_n0247_5_cyo,
      DI => cs_chroma(6),
      S => N18,
      O => audio_clk_n0247_6_cyo
    );
  audio_clk_n0247_6_xor : XORCY
    port map (
      CI => audio_clk_n0247_5_cyo,
      LI => N18,
      O => Q_n0247(6)
    );
  audio_clk_n0247_7_lut : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => cs_chroma(7),
      I1 => audio_chroma(7),
      O => N19
    );
  audio_clk_n0247_7_cy : MUXCY
    port map (
      CI => audio_clk_n0247_6_cyo,
      DI => cs_chroma(7),
      S => N19,
      O => audio_clk_n0247_7_cyo
    );
  audio_clk_n0247_7_xor : XORCY
    port map (
      CI => audio_clk_n0247_6_cyo,
      LI => N19,
      O => Q_n0247(7)
    );
  audio_clk_n0247_8_lut : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => cs_chroma(8),
      I1 => audio_chroma(8),
      O => N20
    );
  Ker134 : LUT4_D
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => current_state(0),
      I1 => current_state(7),
      I2 => N1546,
      I3 => N138,
      LO => N1594,
      O => N134
    );
  Mxor_n0025_Xo_3_1_SW0 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => Q_n0020(9),
      I1 => Q_n0020(8),
      I2 => Q_n0020(11),
      I3 => Q_n0020(10),
      O => N1564
    );
  Mxor_n0025_Xo_3_1 : LUT4
    generic map(
      INIT => X"AA9A"
    )
    port map (
      I0 => N1564,
      I1 => sample_pos(0),
      I2 => output_counter_buffer_0_12,
      I3 => sample_pos(1),
      O => Q_n0026(0)
    );
  audio_clk_n0256_13_cy : MUXCY
    port map (
      CI => audio_clk_n0256_12_cyo,
      DI => output_counter_buffer(2, 0),
      S => counter_13_rt,
      O => audio_clk_n0256_13_cyo
    );
  audio_clk_n0256_22_lut_INV_0 : INV
    port map (
      I => counter(22),
      O => N70
    );
  audio_clk_n0256_21_xor : XORCY
    port map (
      CI => audio_clk_n0256_20_cyo,
      LI => N69,
      O => Q_n0256(21)
    );
  audio_clk_n0256_21_cy : MUXCY
    port map (
      CI => audio_clk_n0256_20_cyo,
      DI => N1,
      S => N69,
      O => audio_clk_n0256_21_cyo
    );
  audio_clk_n0256_21_lut_INV_0 : INV
    port map (
      I => counter(21),
      O => N69
    );
  audio_clk_n0256_20_xor : XORCY
    port map (
      CI => audio_clk_n0256_19_cyo,
      LI => N68,
      O => Q_n0256(20)
    );
  audio_clk_n0256_20_cy : MUXCY
    port map (
      CI => audio_clk_n0256_19_cyo,
      DI => N1,
      S => N68,
      O => audio_clk_n0256_20_cyo
    );
  audio_clk_n0256_20_lut_INV_0 : INV
    port map (
      I => counter(20),
      O => N68
    );
  audio_clk_n0256_19_xor : XORCY
    port map (
      CI => audio_clk_n0256_18_cyo,
      LI => N67,
      O => Q_n0256(19)
    );
  audio_clk_n0256_19_cy : MUXCY
    port map (
      CI => audio_clk_n0256_18_cyo,
      DI => N1,
      S => N67,
      O => audio_clk_n0256_19_cyo
    );
  audio_clk_n0256_19_lut_INV_0 : INV
    port map (
      I => counter(19),
      O => N67
    );
  audio_clk_n0256_18_xor : XORCY
    port map (
      CI => audio_clk_n0256_17_cyo,
      LI => N66,
      O => Q_n0256(18)
    );
  audio_clk_n0256_18_cy : MUXCY
    port map (
      CI => audio_clk_n0256_17_cyo,
      DI => N1,
      S => N66,
      O => audio_clk_n0256_18_cyo
    );
  audio_clk_n0256_18_lut_INV_0 : INV
    port map (
      I => counter(18),
      O => N66
    );
  audio_clk_n0256_17_xor : XORCY
    port map (
      CI => audio_clk_n0256_16_cyo,
      LI => N65,
      O => Q_n0256(17)
    );
  audio_clk_n0256_17_cy : MUXCY
    port map (
      CI => audio_clk_n0256_16_cyo,
      DI => counter(17),
      S => N65,
      O => audio_clk_n0256_17_cyo
    );
  next_aes_count_0_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => aes_count(0),
      I1 => N145,
      I2 => N0,
      O => next_aes_count(0)
    );
  audio_clk_n0256_16_xor : XORCY
    port map (
      CI => audio_clk_n0256_15_cyo,
      LI => N64,
      O => Q_n0256(16)
    );
  audio_clk_n0256_16_cy : MUXCY
    port map (
      CI => audio_clk_n0256_15_cyo,
      DI => N1,
      S => N64,
      O => audio_clk_n0256_16_cyo
    );
  audio_clk_n0256_16_lut_INV_0 : INV
    port map (
      I => counter(16),
      O => N64
    );
  audio_clk_n0256_15_xor : XORCY
    port map (
      CI => audio_clk_n0256_14_cyo,
      LI => N63,
      O => Q_n0256(15)
    );
  audio_clk_n0256_15_cy : MUXCY
    port map (
      CI => audio_clk_n0256_14_cyo,
      DI => N1,
      S => N63,
      O => audio_clk_n0256_15_cyo
    );
  audio_clk_n0256_15_lut_INV_0 : INV
    port map (
      I => counter(15),
      O => N63
    );
  audio_clk_n0256_14_xor : XORCY
    port map (
      CI => audio_clk_n0256_13_cyo,
      LI => N62,
      O => Q_n0256(14)
    );
  audio_clk_n0250_0_lut_INV_0 : INV
    port map (
      I => video_count(0),
      O => N30
    );
  audio_clk_n0250_0_cy : MUXCY
    port map (
      CI => output_counter_buffer(2, 0),
      DI => N1,
      S => N30,
      O => audio_clk_n0250_0_cyo
    );
  audiochroma_out_3_OBUF : OBUF
    port map (
      I => audiochroma_out_3,
      O => audiochroma_out(3)
    );
  audio_clk_n0250_1_cy : MUXCY
    port map (
      CI => audio_clk_n0250_0_cyo,
      DI => output_counter_buffer(2, 0),
      S => video_count_1_rt,
      O => audio_clk_n0250_1_cyo
    );
  audio_clk_n0250_1_xor : XORCY
    port map (
      CI => audio_clk_n0250_0_cyo,
      LI => video_count_1_rt,
      O => Q_n0250(1)
    );
  audio_clk_n0250_2_cy : MUXCY
    port map (
      CI => audio_clk_n0250_1_cyo,
      DI => output_counter_buffer(2, 0),
      S => video_count_2_rt,
      O => audio_clk_n0250_2_cyo
    );
  audio_clk_n0250_2_xor : XORCY
    port map (
      CI => audio_clk_n0250_1_cyo,
      LI => video_count_2_rt,
      O => Q_n0250(2)
    );
  audio_clk_n0250_3_cy : MUXCY
    port map (
      CI => audio_clk_n0250_2_cyo,
      DI => output_counter_buffer(2, 0),
      S => video_count_3_rt,
      O => audio_clk_n0250_3_cyo
    );
  audio_clk_n0250_3_xor : XORCY
    port map (
      CI => audio_clk_n0250_2_cyo,
      LI => video_count_3_rt,
      O => Q_n0250(3)
    );
  audio_clk_n0250_4_cy : MUXCY
    port map (
      CI => audio_clk_n0250_3_cyo,
      DI => output_counter_buffer(2, 0),
      S => video_count_4_rt,
      O => audio_clk_n0250_4_cyo
    );
  audio_clk_n0250_4_xor : XORCY
    port map (
      CI => audio_clk_n0250_3_cyo,
      LI => video_count_4_rt,
      O => Q_n0250(4)
    );
  audio_clk_n0250_5_cy : MUXCY
    port map (
      CI => audio_clk_n0250_4_cyo,
      DI => output_counter_buffer(2, 0),
      S => video_count_5_rt,
      O => audio_clk_n0250_5_cyo
    );
  audio_clk_n0250_5_xor : XORCY
    port map (
      CI => audio_clk_n0250_4_cyo,
      LI => video_count_5_rt,
      O => Q_n0250(5)
    );
  audio_clk_n0250_6_cy : MUXCY
    port map (
      CI => audio_clk_n0250_5_cyo,
      DI => output_counter_buffer(2, 0),
      S => video_count_6_rt,
      O => audio_clk_n0250_6_cyo
    );
  audio_clk_n0250_6_xor : XORCY
    port map (
      CI => audio_clk_n0250_5_cyo,
      LI => video_count_6_rt,
      O => Q_n0250(6)
    );
  audio_clk_n0250_7_cy : MUXCY
    port map (
      CI => audio_clk_n0250_6_cyo,
      DI => output_counter_buffer(2, 0),
      S => video_count_7_rt,
      O => audio_clk_n0250_7_cyo
    );
  audio_clk_n0250_7_xor : XORCY
    port map (
      CI => audio_clk_n0250_6_cyo,
      LI => video_count_7_rt,
      O => Q_n0250(7)
    );
  audio_clk_n0250_8_cy : MUXCY
    port map (
      CI => audio_clk_n0250_7_cyo,
      DI => output_counter_buffer(2, 0),
      S => video_count_8_rt,
      O => audio_clk_n0250_8_cyo
    );
  audio_clk_n0250_8_xor : XORCY
    port map (
      CI => audio_clk_n0250_7_cyo,
      LI => video_count_8_rt,
      O => Q_n0250(8)
    );
  audio_clk_n0250_9_cy : MUXCY
    port map (
      CI => audio_clk_n0250_8_cyo,
      DI => output_counter_buffer(2, 0),
      S => video_count_9_rt,
      O => audio_clk_n0250_9_cyo
    );
  audio_clk_n0250_9_xor : XORCY
    port map (
      CI => audio_clk_n0250_8_cyo,
      LI => video_count_9_rt,
      O => Q_n0250(9)
    );
  audio_clk_n0250_10_cy : MUXCY
    port map (
      CI => audio_clk_n0250_9_cyo,
      DI => output_counter_buffer(2, 0),
      S => video_count_10_rt,
      O => audio_clk_n0250_10_cyo
    );
  audio_clk_n0250_10_xor : XORCY
    port map (
      CI => audio_clk_n0250_9_cyo,
      LI => video_count_10_rt,
      O => Q_n0250(10)
    );
  audio_clk_n0256_14_cy : MUXCY
    port map (
      CI => audio_clk_n0256_13_cyo,
      DI => N1,
      S => N62,
      O => audio_clk_n0256_14_cyo
    );
  audio_clk_n0453_0_lut_INV_0 : INV
    port map (
      I => temp_clk_counter(0),
      O => N31
    );
  audio_clk_n0453_0_cy : MUXCY
    port map (
      CI => output_counter_buffer(2, 0),
      DI => N1,
      S => N31,
      O => audio_clk_n0453_0_cyo
    );
  audiochroma_out_5_OBUF : OBUF
    port map (
      I => audiochroma_out_5,
      O => audiochroma_out(5)
    );
  audio_clk_n0453_1_cy : MUXCY
    port map (
      CI => audio_clk_n0453_0_cyo,
      DI => output_counter_buffer(2, 0),
      S => temp_clk_counter_1_rt,
      O => audio_clk_n0453_1_cyo
    );
  audio_clk_n0453_1_xor : XORCY
    port map (
      CI => audio_clk_n0453_0_cyo,
      LI => temp_clk_counter_1_rt,
      O => Q_n0453(1)
    );
  audio_clk_n0453_2_cy : MUXCY
    port map (
      CI => audio_clk_n0453_1_cyo,
      DI => output_counter_buffer(2, 0),
      S => temp_clk_counter_2_rt,
      O => audio_clk_n0453_2_cyo
    );
  audio_clk_n0453_2_xor : XORCY
    port map (
      CI => audio_clk_n0453_1_cyo,
      LI => temp_clk_counter_2_rt,
      O => Q_n0453(2)
    );
  audio_clk_n0453_3_cy : MUXCY
    port map (
      CI => audio_clk_n0453_2_cyo,
      DI => output_counter_buffer(2, 0),
      S => temp_clk_counter_3_rt,
      O => audio_clk_n0453_3_cyo
    );
  audio_clk_n0453_3_xor : XORCY
    port map (
      CI => audio_clk_n0453_2_cyo,
      LI => temp_clk_counter_3_rt,
      O => Q_n0453(3)
    );
  sample_pos_N01_INV_0 : INV
    port map (
      I => reset_in_IBUF,
      O => sample_pos_N0
    );
  Ker721 : LUT4_D
    generic map(
      INIT => X"80FF"
    )
    port map (
      I0 => index(0),
      I1 => index(1),
      I2 => eav_tick,
      I3 => nor_cyo7,
      LO => N1595,
      O => N72
    );
  Q_n0246_21_1 : LUT3_L
    generic map(
      INIT => X"E2"
    )
    port map (
      I0 => counter(21),
      I1 => nor_cyo7,
      I2 => Q_n0256(21),
      LO => Q_n0246_21_Q
    );
  Q_n02261 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => nor_cyo7,
      I1 => sync_reset_in_IBUF,
      O => Q_n0226
    );
  Q_n0231_22_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(22),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(22)
    );
  audio_clk_n0256_10_cy : MUXCY
    port map (
      CI => audio_clk_n0256_9_cyo,
      DI => counter(10),
      S => N59,
      O => audio_clk_n0256_10_cyo
    );
  audio_clk_n0256_10_xor : XORCY
    port map (
      CI => audio_clk_n0256_9_cyo,
      LI => N59,
      O => Q_n0256(10)
    );
  Q_n0231_21_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(21),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(21)
    );
  audio_clk_n0256_11_xor : XORCY
    port map (
      CI => audio_clk_n0256_10_cyo,
      LI => N60,
      O => Q_n0256(11)
    );
  Q_n0231_20_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(20),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(20)
    );
  Q_n0231_19_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(19),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(19)
    );
  Q_n0231_18_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(18),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(18)
    );
  Q_n0231_17_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(17),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(17)
    );
  Q_n0231_16_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(16),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(16)
    );
  audio_clk_n0256_8_lut : LUT4_L
    generic map(
      INIT => X"1E0F"
    )
    port map (
      I0 => clkmux_in_2_IBUF,
      I1 => clkmux_in_1_IBUF,
      I2 => counter(8),
      I3 => clkmux_in_0_IBUF,
      LO => N57
    );
  audio_clk_n0256_9_xor : XORCY
    port map (
      CI => audio_clk_n0256_8_cyo,
      LI => N58,
      O => Q_n0256(9)
    );
  Q_n04151 : LUT4
    generic map(
      INIT => X"EAAA"
    )
    port map (
      I0 => sync_reset_in_IBUF,
      I1 => index(1),
      I2 => nor_cyo7,
      I3 => N1540,
      O => Q_n0415
    );
  Q_n0231_15_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(15),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(15)
    );
  Q_n0231_14_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(14),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(14)
    );
  Q_n0231_13_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(13),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(13)
    );
  Q_n0220_1_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => N1593,
      I1 => sync_reset_in_IBUF,
      LO => Q_n0220(1)
    );
  Q_n0220_2_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => N1592,
      I1 => sync_reset_in_IBUF,
      LO => Q_n0220(2)
    );
  Q_n0220_3_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => next_aes_count(3),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0220(3)
    );
  Q_n0220_4_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => next_aes_count(4),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0220(4)
    );
  Q_n0220_5_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => next_aes_count(5),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0220(5)
    );
  Q_n0220_6_1 : LUT3_L
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => next_aes_count(6),
      I1 => sync_reset_in_IBUF,
      I2 => N1587,
      LO => Q_n0220(6)
    );
  Q_n0220_7_1 : LUT3_L
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => N1589,
      I1 => sync_reset_in_IBUF,
      I2 => Q_n0448(1),
      LO => Q_n0220(7)
    );
  Q_n0231_12_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(12),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(12)
    );
  Q_n0231_11_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(11),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(11)
    );
  audio_clk_n0256_14_lut_INV_0 : INV
    port map (
      I => counter(14),
      O => N62
    );
  Q_n0234_1_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_1_1,
      I1 => sync_reset_in_IBUF,
      O => Q_n0234(1)
    );
  Q_n0234_0_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter_buffer_1_0,
      I1 => sync_reset_in_IBUF,
      O => Q_n0234(0)
    );
  audio_clk_n0253_10_cy : MUXCY
    port map (
      CI => audio_clk_n0253_9_cyo,
      DI => output_counter_buffer(2, 0),
      S => Q_n0246_10_Q,
      O => audio_clk_n0253_10_cyo
    );
  audio_clk_n0253_9_xor : XORCY
    port map (
      CI => audio_clk_n0253_8_cyo,
      LI => Q_n0246_9_Q,
      O => Q_n0253(9)
    );
  audio_clk_n0253_9_cy : MUXCY
    port map (
      CI => audio_clk_n0253_8_cyo,
      DI => output_counter_buffer(2, 0),
      S => Q_n0246_9_Q,
      O => audio_clk_n0253_9_cyo
    );
  audio_clk_n0253_8_xor : XORCY
    port map (
      CI => audio_clk_n0253_7_cyo,
      LI => Q_n0246_8_Q,
      O => Q_n0253(8)
    );
  audio_clk_n0253_8_cy : MUXCY
    port map (
      CI => audio_clk_n0253_7_cyo,
      DI => output_counter_buffer(2, 0),
      S => Q_n0246_8_Q,
      O => audio_clk_n0253_8_cyo
    );
  audio_clk_n0253_7_xor : XORCY
    port map (
      CI => audio_clk_n0253_6_cyo,
      LI => Q_n0246_7_Q,
      O => Q_n0253(7)
    );
  Q_n0218_0_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(0),
      I2 => audio_chroma(0),
      I3 => ecc5(0),
      O => Q_n0218(0)
    );
  Q_n0218_1_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(1),
      I2 => audio_chroma(1),
      I3 => ecc5(1),
      O => Q_n0218(1)
    );
  Q_n0218_2_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(2),
      I2 => audio_chroma(2),
      I3 => ecc5(2),
      O => Q_n0218(2)
    );
  Q_n0218_3_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(3),
      I2 => audio_chroma(3),
      I3 => ecc5(3),
      O => Q_n0218(3)
    );
  Q_n0218_4_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(4),
      I2 => audio_chroma(4),
      I3 => ecc5(4),
      O => Q_n0218(4)
    );
  Q_n0218_5_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(5),
      I2 => audio_chroma(5),
      I3 => ecc5(5),
      O => Q_n0218(5)
    );
  Q_n0218_6_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(6),
      I2 => audio_chroma(6),
      I3 => ecc5(6),
      O => Q_n0218(6)
    );
  Q_n0218_7_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(7),
      I2 => audio_chroma(7),
      I3 => ecc5(7),
      O => Q_n0218(7)
    );
  Q_n0217_0_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(0),
      I2 => audio_chroma(0),
      I3 => ecc4(0),
      O => Q_n0217(0)
    );
  Q_n0217_1_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(1),
      I2 => audio_chroma(1),
      I3 => ecc4(1),
      O => Q_n0217(1)
    );
  Q_n0217_2_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(2),
      I2 => audio_chroma(2),
      I3 => ecc4(2),
      O => Q_n0217(2)
    );
  Q_n0217_3_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(3),
      I2 => audio_chroma(3),
      I3 => ecc4(3),
      O => Q_n0217(3)
    );
  Q_n0217_4_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(4),
      I2 => audio_chroma(4),
      I3 => ecc4(4),
      O => Q_n0217(4)
    );
  Q_n0217_5_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(5),
      I2 => audio_chroma(5),
      I3 => ecc4(5),
      O => Q_n0217(5)
    );
  Q_n0217_6_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(6),
      I2 => audio_chroma(6),
      I3 => ecc4(6),
      O => Q_n0217(6)
    );
  Q_n0217_7_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(7),
      I2 => audio_chroma(7),
      I3 => ecc4(7),
      O => Q_n0217(7)
    );
  Q_n0216_0_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(0),
      I2 => audio_chroma(0),
      I3 => ecc3(0),
      O => Q_n0216(0)
    );
  Q_n0216_1_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(1),
      I2 => audio_chroma(1),
      I3 => ecc3(1),
      O => Q_n0216(1)
    );
  Q_n0216_2_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(2),
      I2 => audio_chroma(2),
      I3 => ecc3(2),
      O => Q_n0216(2)
    );
  Q_n0216_3_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(3),
      I2 => audio_chroma(3),
      I3 => ecc3(3),
      O => Q_n0216(3)
    );
  Q_n0216_4_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(4),
      I2 => audio_chroma(4),
      I3 => ecc3(4),
      O => Q_n0216(4)
    );
  Q_n0216_5_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(5),
      I2 => audio_chroma(5),
      I3 => ecc3(5),
      O => Q_n0216(5)
    );
  Q_n0216_6_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(6),
      I2 => audio_chroma(6),
      I3 => ecc3(6),
      O => Q_n0216(6)
    );
  Q_n0216_7_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(7),
      I2 => audio_chroma(7),
      I3 => ecc3(7),
      O => Q_n0216(7)
    );
  Q_n0231_10_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(10),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(10)
    );
  Q_n0231_9_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(9),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(9)
    );
  Q_n0231_8_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(8),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(8)
    );
  Q_n0231_7_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(7),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(7)
    );
  Q_n0231_6_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(6),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(6)
    );
  Q_n0231_5_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(5),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(5)
    );
  Q_n0231_4_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(4),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(4)
    );
  Q_n0231_3_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(3),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(3)
    );
  Q_n0213_0_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(0),
      I2 => audio_chroma(0),
      I3 => ecc1(0),
      O => Q_n0213(0)
    );
  Q_n0213_1_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(1),
      I2 => audio_chroma(1),
      I3 => ecc1(1),
      O => Q_n0213(1)
    );
  Q_n0213_2_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(2),
      I2 => audio_chroma(2),
      I3 => ecc1(2),
      O => Q_n0213(2)
    );
  Q_n0213_3_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(3),
      I2 => audio_chroma(3),
      I3 => ecc1(3),
      O => Q_n0213(3)
    );
  Q_n0213_4_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(4),
      I2 => audio_chroma(4),
      I3 => ecc1(4),
      O => Q_n0213(4)
    );
  Q_n0213_5_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(5),
      I2 => audio_chroma(5),
      I3 => ecc1(5),
      O => Q_n0213(5)
    );
  Q_n0213_6_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(6),
      I2 => audio_chroma(6),
      I3 => ecc1(6),
      O => Q_n0213(6)
    );
  Q_n0213_7_1 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N1421,
      I1 => ecc0(7),
      I2 => audio_chroma(7),
      I3 => ecc1(7),
      O => Q_n0213(7)
    );
  Q_n0231_2_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(2),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(2)
    );
  Q_n0231_1_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Q_n0253(1),
      I1 => sync_reset_in_IBUF,
      LO => Q_n0231(1)
    );
  Q_n0231_0_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => N47,
      I1 => sync_reset_in_IBUF,
      O => Q_n0231(0)
    );
  Q_n0232_9_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => audio_chroma(9),
      I1 => sync_reset_in_IBUF,
      O => Q_n0232(9)
    );
  Q_n0232_8_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => audio_chroma(8),
      I1 => sync_reset_in_IBUF,
      O => Q_n0232(8)
    );
  Q_n0232_7_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => audio_chroma(7),
      I1 => sync_reset_in_IBUF,
      O => Q_n0232(7)
    );
  audio_clk_n0256_11_lut_INV_0 : INV
    port map (
      I => counter(11),
      O => N60
    );
  Q_n0232_6_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => audio_chroma(6),
      I1 => sync_reset_in_IBUF,
      O => Q_n0232(6)
    );
  Q_n0232_5_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => audio_chroma(5),
      I1 => sync_reset_in_IBUF,
      O => Q_n0232(5)
    );
  audio_clk_n0256_12_cy : MUXCY
    port map (
      CI => audio_clk_n0256_11_cyo,
      DI => counter(12),
      S => N61,
      O => audio_clk_n0256_12_cyo
    );
  Q_n0020_12_1 : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => output_counter_buffer_0_12,
      I1 => sample_pos(1),
      I2 => sample_pos(0),
      O => Q_n0020(12)
    );
  Q_n0232_4_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => audio_chroma(4),
      I1 => sync_reset_in_IBUF,
      O => Q_n0232(4)
    );
  audio_clk_n0256_11_cy : MUXCY
    port map (
      CI => audio_clk_n0256_10_cyo,
      DI => N1,
      S => N60,
      O => audio_clk_n0256_11_cyo
    );
  Q_n0020_1_1 : LUT4
    generic map(
      INIT => X"5140"
    )
    port map (
      I0 => sample_pos(1),
      I1 => sample_pos(0),
      I2 => output_counter_buffer_1_1,
      I3 => output_counter_buffer_0_1,
      O => Q_n0020(1)
    );
  Q_n0020_2_1 : LUT4
    generic map(
      INIT => X"5140"
    )
    port map (
      I0 => sample_pos(1),
      I1 => sample_pos(0),
      I2 => output_counter_buffer_1_2,
      I3 => output_counter_buffer_0_2,
      O => Q_n0020(2)
    );
  Q_n0020_3_1 : LUT4
    generic map(
      INIT => X"5140"
    )
    port map (
      I0 => sample_pos(1),
      I1 => sample_pos(0),
      I2 => output_counter_buffer_1_3,
      I3 => output_counter_buffer_0_3,
      O => Q_n0020(3)
    );
  Q_n0020_4_1 : LUT4
    generic map(
      INIT => X"5140"
    )
    port map (
      I0 => sample_pos(1),
      I1 => sample_pos(0),
      I2 => output_counter_buffer_1_4,
      I3 => output_counter_buffer_0_4,
      O => Q_n0020(4)
    );
  Q_n0020_5_1 : LUT4
    generic map(
      INIT => X"5140"
    )
    port map (
      I0 => sample_pos(1),
      I1 => sample_pos(0),
      I2 => output_counter_buffer_1_5,
      I3 => output_counter_buffer_0_5,
      O => Q_n0020(5)
    );
  Q_n0020_6_1 : LUT4
    generic map(
      INIT => X"5140"
    )
    port map (
      I0 => sample_pos(1),
      I1 => sample_pos(0),
      I2 => output_counter_buffer_1_6,
      I3 => output_counter_buffer_0_6,
      O => Q_n0020(6)
    );
  Q_n0020_7_1 : LUT4
    generic map(
      INIT => X"5140"
    )
    port map (
      I0 => sample_pos(1),
      I1 => sample_pos(0),
      I2 => output_counter_buffer_1_7,
      I3 => output_counter_buffer_0_7,
      O => Q_n0020(7)
    );
  Q_n0020_8_1 : LUT4
    generic map(
      INIT => X"5140"
    )
    port map (
      I0 => sample_pos(1),
      I1 => sample_pos(0),
      I2 => output_counter_buffer_1_8,
      I3 => output_counter_buffer_0_8,
      O => Q_n0020(8)
    );
  Q_n0020_9_1 : LUT4
    generic map(
      INIT => X"5140"
    )
    port map (
      I0 => sample_pos(1),
      I1 => sample_pos(0),
      I2 => output_counter_buffer_1_9,
      I3 => output_counter_buffer_0_9,
      O => Q_n0020(9)
    );
  Q_n0232_3_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => audio_chroma(3),
      I1 => sync_reset_in_IBUF,
      O => Q_n0232(3)
    );
  Q_n0020_0_1 : LUT4
    generic map(
      INIT => X"5140"
    )
    port map (
      I0 => sample_pos(1),
      I1 => sample_pos(0),
      I2 => output_counter_buffer_1_0,
      I3 => output_counter_buffer_0_0,
      O => Q_n0020(0)
    );
  Q_n0020_10_1 : LUT4
    generic map(
      INIT => X"5140"
    )
    port map (
      I0 => sample_pos(1),
      I1 => sample_pos(0),
      I2 => output_counter_buffer_1_10,
      I3 => output_counter_buffer_0_10,
      O => Q_n0020(10)
    );
  Q_n0020_11_1 : LUT4
    generic map(
      INIT => X"5140"
    )
    port map (
      I0 => sample_pos(1),
      I1 => sample_pos(0),
      I2 => output_counter_buffer_1_11,
      I3 => output_counter_buffer_0_11,
      O => Q_n0020(11)
    );
  audio_clk_n0253_11_cy : MUXCY
    port map (
      CI => audio_clk_n0253_10_cyo,
      DI => output_counter_buffer(2, 0),
      S => Q_n0246_11_Q,
      O => audio_clk_n0253_11_cyo
    );
  Q_n0232_2_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => audio_chroma(2),
      I1 => sync_reset_in_IBUF,
      O => Q_n0232(2)
    );
  Andcy : MUXCY
    port map (
      CI => N1,
      DI => output_counter_buffer(2, 0),
      S => counter_0_rt1,
      O => And_cyo
    );
  Q_n0246_2_1 : LUT3_L
    generic map(
      INIT => X"CA"
    )
    port map (
      I0 => counter(2),
      I1 => Q_n0256(2),
      I2 => nor_cyo7,
      LO => Q_n0246_2_Q
    );
  XNor_stagecy : MUXCY
    port map (
      CI => And_cyo,
      DI => counter(1),
      S => N32,
      O => XNor_stage_cyo
    );
  XNor_stagelut : LUT4_L
    generic map(
      INIT => X"F0B4"
    )
    port map (
      I0 => clkmux_in_2_IBUF,
      I1 => clkmux_in_0_IBUF,
      I2 => counter(1),
      I3 => clkmux_in_1_IBUF,
      LO => N32
    );
  XNor_stagecy_rn_0 : MUXCY
    port map (
      CI => XNor_stage_cyo,
      DI => counter(2),
      S => N33,
      O => XNor_stage_cyo1
    );
  norlut_INV_0 : INV
    port map (
      I => counter(3),
      O => N34
    );
  norcy : MUXCY
    port map (
      CI => XNor_stage_cyo1,
      DI => N1,
      S => N34,
      O => nor_cyo
    );
  Andcy_rn_0 : MUXCY
    port map (
      CI => nor_cyo,
      DI => output_counter_buffer(2, 0),
      S => counter_4_rt1,
      O => And_cyo1
    );
  norlut1_INV_0 : INV
    port map (
      I => counter(5),
      O => N35
    );
  norcy_rn_0 : MUXCY
    port map (
      CI => And_cyo1,
      DI => N1,
      S => N35,
      O => nor_cyo1
    );
  Andcy_rn_1 : MUXCY
    port map (
      CI => nor_cyo1,
      DI => output_counter_buffer(2, 0),
      S => counter_6_rt1,
      O => And_cyo2
    );
  norlut2_INV_0 : INV
    port map (
      I => counter(7),
      O => N36
    );
  norcy_rn_1 : MUXCY
    port map (
      CI => And_cyo2,
      DI => N1,
      S => N36,
      O => nor_cyo2
    );
  XNor_stagelut1 : LUT4_L
    generic map(
      INIT => X"F0B4"
    )
    port map (
      I0 => clkmux_in_2_IBUF,
      I1 => clkmux_in_0_IBUF,
      I2 => counter(2),
      I3 => clkmux_in_1_IBUF,
      LO => N33
    );
  XNor_stagecy_rn_1 : MUXCY
    port map (
      CI => nor_cyo2,
      DI => counter(8),
      S => N37,
      O => XNor_stage_cyo2
    );
  norlut3_INV_0 : INV
    port map (
      I => counter(9),
      O => N38
    );
  norcy_rn_2 : MUXCY
    port map (
      CI => XNor_stage_cyo2,
      DI => N1,
      S => N38,
      O => nor_cyo3
    );
  audio_clk_n0256_1_lut : LUT4_L
    generic map(
      INIT => X"F0B4"
    )
    port map (
      I0 => clkmux_in_2_IBUF,
      I1 => clkmux_in_0_IBUF,
      I2 => counter(1),
      I3 => clkmux_in_1_IBUF,
      LO => N52
    );
  XNor_stagecy_rn_2 : MUXCY
    port map (
      CI => nor_cyo3,
      DI => counter(10),
      S => N39,
      O => XNor_stage_cyo3
    );
  norlut4_INV_0 : INV
    port map (
      I => counter(11),
      O => N40
    );
  norcy_rn_3 : MUXCY
    port map (
      CI => XNor_stage_cyo3,
      DI => N1,
      S => N40,
      O => nor_cyo4
    );
  audio_clk_n0256_2_lut : LUT4_L
    generic map(
      INIT => X"F0B4"
    )
    port map (
      I0 => clkmux_in_2_IBUF,
      I1 => clkmux_in_0_IBUF,
      I2 => counter(2),
      I3 => clkmux_in_1_IBUF,
      LO => N53
    );
  XNor_stagecy_rn_3 : MUXCY
    port map (
      CI => nor_cyo4,
      DI => counter(12),
      S => N41,
      O => XNor_stage_cyo4
    );
  Andcy_rn_2 : MUXCY
    port map (
      CI => XNor_stage_cyo4,
      DI => output_counter_buffer(2, 0),
      S => counter_13_rt1,
      O => And_cyo3
    );
  norlut5 : LUT3_L
    generic map(
      INIT => X"01"
    )
    port map (
      I0 => counter(14),
      I1 => counter(15),
      I2 => counter(16),
      LO => N42
    );
  norcy_rn_4 : MUXCY
    port map (
      CI => And_cyo3,
      DI => N1,
      S => N42,
      O => nor_cyo5
    );
  audio_clk_n0256_17_lut : LUT4_L
    generic map(
      INIT => X"1E0F"
    )
    port map (
      I0 => clkmux_in_2_IBUF,
      I1 => clkmux_in_1_IBUF,
      I2 => counter(17),
      I3 => clkmux_in_0_IBUF,
      LO => N65
    );
  XNor_stagecy_rn_4 : MUXCY
    port map (
      CI => nor_cyo5,
      DI => counter(17),
      S => N43,
      O => XNor_stage_cyo5
    );
  norlut6 : LUT4_L
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => counter(18),
      I1 => counter(19),
      I2 => counter(20),
      I3 => counter(21),
      LO => N44
    );
  norcy_rn_5 : MUXCY
    port map (
      CI => XNor_stage_cyo5,
      DI => N1,
      S => N44,
      O => nor_cyo6
    );
  norlut7_INV_0 : INV
    port map (
      I => counter(22),
      O => N45
    );
  norcy_rn_6 : MUXCY
    port map (
      CI => nor_cyo6,
      DI => N1,
      S => N45,
      O => nor_cyo7
    );
  audio_clk_n0253_3_lut : LUT3_L
    generic map(
      INIT => X"35"
    )
    port map (
      I0 => counter(3),
      I1 => Q_n0256(3),
      I2 => nor_cyo7,
      LO => N49
    );
  audio_clk_n0253_0_cy : MUXCY
    port map (
      CI => output_counter_buffer(2, 0),
      DI => interval(6),
      S => N1596,
      O => audio_clk_n0253_0_cyo
    );
  audiochroma_out_4_OBUF : OBUF
    port map (
      I => audiochroma_out_4,
      O => audiochroma_out(4)
    );
  temp_clk_counter_11_rt_97 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(11),
      O => temp_clk_counter_11_rt
    );
  audio_clk_n0253_1_cy : MUXCY
    port map (
      CI => audio_clk_n0253_0_cyo,
      DI => interval(6),
      S => N48,
      O => audio_clk_n0253_1_cyo
    );
  audio_clk_n0253_1_xor : XORCY
    port map (
      CI => audio_clk_n0253_0_cyo,
      LI => N48,
      O => Q_n0253(1)
    );
  audio_clk_n0253_2_cy : MUXCY
    port map (
      CI => audio_clk_n0253_1_cyo,
      DI => output_counter_buffer(2, 0),
      S => Q_n0246_2_Q,
      O => audio_clk_n0253_2_cyo
    );
  audio_clk_n0253_2_xor : XORCY
    port map (
      CI => audio_clk_n0253_1_cyo,
      LI => Q_n0246_2_Q,
      O => Q_n0253(2)
    );
  XNor_stagelut4 : LUT4_L
    generic map(
      INIT => X"F0B4"
    )
    port map (
      I0 => clkmux_in_2_IBUF,
      I1 => clkmux_in_0_IBUF,
      I2 => counter(12),
      I3 => clkmux_in_1_IBUF,
      LO => N41
    );
  audio_clk_n0253_3_cy : MUXCY
    port map (
      CI => audio_clk_n0253_2_cyo,
      DI => N1,
      S => N49,
      O => audio_clk_n0253_3_cyo
    );
  audio_clk_n0253_3_xor : XORCY
    port map (
      CI => audio_clk_n0253_2_cyo,
      LI => N49,
      O => Q_n0253(3)
    );
  audio_clk_n0253_0_lut : LUT4_D
    generic map(
      INIT => X"56A6"
    )
    port map (
      I0 => interval(6),
      I1 => counter(0),
      I2 => nor_cyo7,
      I3 => Q_n0256(0),
      LO => N1596,
      O => N47
    );
  audio_clk_n0253_4_cy : MUXCY
    port map (
      CI => audio_clk_n0253_3_cyo,
      DI => interval(6),
      S => N50,
      O => audio_clk_n0253_4_cyo
    );
  audio_clk_n0253_4_xor : XORCY
    port map (
      CI => audio_clk_n0253_3_cyo,
      LI => N50,
      O => Q_n0253(4)
    );
  audio_clk_n0253_5_cy : MUXCY
    port map (
      CI => audio_clk_n0253_4_cyo,
      DI => output_counter_buffer(2, 0),
      S => Q_n0246_5_Q,
      O => audio_clk_n0253_5_cyo
    );
  audio_clk_n0253_5_xor : XORCY
    port map (
      CI => audio_clk_n0253_4_cyo,
      LI => Q_n0246_5_Q,
      O => Q_n0253(5)
    );
  audio_clk_n0253_1_lut : LUT4_L
    generic map(
      INIT => X"56A6"
    )
    port map (
      I0 => interval(6),
      I1 => counter(1),
      I2 => nor_cyo7,
      I3 => Q_n0256(1),
      LO => N48
    );
  audio_clk_n0253_6_cy : MUXCY
    port map (
      CI => audio_clk_n0253_5_cyo,
      DI => interval(6),
      S => N51,
      O => audio_clk_n0253_6_cyo
    );
  audio_clk_n0253_6_xor : XORCY
    port map (
      CI => audio_clk_n0253_5_cyo,
      LI => N51,
      O => Q_n0253(6)
    );
  audio_clk_n0253_7_cy : MUXCY
    port map (
      CI => audio_clk_n0253_6_cyo,
      DI => output_counter_buffer(2, 0),
      S => Q_n0246_7_Q,
      O => audio_clk_n0253_7_cyo
    );
  Q_n0271_9_SW0 : LUT3
    generic map(
      INIT => X"A8"
    )
    port map (
      I0 => state_count(2),
      I1 => state_count(0),
      I2 => state_count(1),
      O => N203
    );
  Q_n0225_5_SW0 : LUT3_L
    generic map(
      INIT => X"1F"
    )
    port map (
      I0 => Q_n0405,
      I1 => current_state(9),
      I2 => state_count(5),
      LO => N701
    );
  Q_n0225_4_SW0 : LUT3_L
    generic map(
      INIT => X"1F"
    )
    port map (
      I0 => Q_n0405,
      I1 => current_state(9),
      I2 => state_count(4),
      LO => N697
    );
  Q_n0225_3_SW0 : LUT3_L
    generic map(
      INIT => X"1F"
    )
    port map (
      I0 => Q_n0405,
      I1 => current_state(9),
      I2 => state_count(3),
      LO => N691
    );
  Q_n0236_1_SW1 : LUT3
    generic map(
      INIT => X"F7"
    )
    port map (
      I0 => current_state(8),
      I1 => audio_chan(0),
      I2 => sync_reset_in_IBUF,
      O => N673
    );
  Q_n0230_0_1 : LUT4
    generic map(
      INIT => X"0602"
    )
    port map (
      I0 => nor_cyo7,
      I1 => index(0),
      I2 => sync_reset_in_IBUF,
      I3 => index(1),
      O => Q_n0230(0)
    );
  Ker10_SW0 : LUT3_L
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => current_state(9),
      I1 => current_state(12),
      I2 => current_state(10),
      LO => N670
    );
  temp_clk_counter_1_rt_98 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(1),
      O => temp_clk_counter_1_rt
    );
  video_count_10_rt_99 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => video_count(10),
      O => video_count_10_rt
    );
  Q_n0235_0_SW1 : LUT4
    generic map(
      INIT => X"3133"
    )
    port map (
      I0 => current_state(10),
      I1 => sample_pos(0),
      I2 => Q_n0368,
      I3 => Q_n0289,
      O => N654
    );
  Q_n0235_0_SW0 : LUT4
    generic map(
      INIT => X"FBBF"
    )
    port map (
      I0 => Q_n0368,
      I1 => current_state(10),
      I2 => Q_n0289,
      I3 => sample_pos(0),
      O => N653
    );
  video_count_6_rt_100 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => video_count(6),
      O => video_count_6_rt
    );
  video_count_2_rt_101 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => video_count(2),
      O => video_count_2_rt
    );
  Q_n026535 : LUT4
    generic map(
      INIT => X"CCC8"
    )
    port map (
      I0 => CHOICE82,
      I1 => N144,
      I2 => CHOICE85,
      I3 => CHOICE88,
      O => CHOICE91
    );
  Q_n0228_0_SW0 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => current_state(12),
      I1 => current_state(11),
      I2 => sync_reset_in_IBUF,
      O => N646
    );
  Q_n0280_SW0 : LUT2_L
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => video_count(8),
      I1 => video_count(9),
      LO => N644
    );
  Q_n0418_SW0 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => current_state(11),
      I1 => current_state(10),
      O => N640
    );
  Ker7836 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => current_state(2),
      I1 => CHOICE23,
      I2 => current_state(1),
      O => N78
    );
  Q_n026310 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0291,
      I1 => ecc0(1),
      I2 => Q_n0292,
      I3 => ecc1(1),
      O => CHOICE172
    );
  Q_n026414 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0291,
      I1 => ecc0(2),
      I2 => Q_n0292,
      I3 => ecc1(2),
      O => CHOICE68
    );
  Q_n025912 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => aes_count(0),
      I1 => aes_count(1),
      I2 => aes_count(2),
      I3 => aes_count(3),
      O => CHOICE6
    );
  Q_n026454_SW0 : LUT4
    generic map(
      INIT => X"CCC8"
    )
    port map (
      I0 => CHOICE71,
      I1 => N144,
      I2 => CHOICE68,
      I3 => CHOICE65,
      O => N1568
    );
  Q_n026384 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => current_state(1),
      I1 => current_state(2),
      I2 => CHOICE188,
      O => CHOICE189
    );
  Q_n026815 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0293,
      I1 => ecc2(6),
      I2 => Q_n0294,
      I3 => ecc3(6),
      O => CHOICE129
    );
  Q_n026749 : LUT3
    generic map(
      INIT => X"EA"
    )
    port map (
      I0 => CHOICE57,
      I1 => cs_chroma(5),
      I2 => current_state(10),
      O => CHOICE58
    );
  Q_n026382 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0020(9),
      I1 => current_state(7),
      I2 => cs_chroma(1),
      I3 => current_state(10),
      O => CHOICE188
    );
  Q_n02685 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0295,
      I1 => ecc4(6),
      I2 => Q_n0296,
      I3 => ecc5(6),
      O => CHOICE123
    );
  Q_n026561 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0020(11),
      I1 => current_state(7),
      I2 => Q_n0020(3),
      I3 => current_state(6),
      O => CHOICE96
    );
  Q_n026454 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => current_state(7),
      I1 => Q_n0020(10),
      I2 => N137,
      I3 => N1568,
      O => CHOICE77
    );
  Q_n02635 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0295,
      I1 => ecc4(1),
      I2 => Q_n0296,
      I3 => ecc5(1),
      O => CHOICE169
    );
  video_count_3_rt_102 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => video_count(3),
      O => video_count_3_rt
    );
  Q_n026463 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => CHOICE62,
      I1 => CHOICE77,
      O => Q_n0264
    );
  Q_n026419 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0293,
      I1 => ecc2(2),
      I2 => Q_n0294,
      I3 => ecc3(2),
      O => CHOICE71
    );
  Q_n026615 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0293,
      I1 => ecc2(4),
      I2 => Q_n0294,
      I3 => ecc3(4),
      O => CHOICE108
    );
  Q_n02941 : LUT3
    generic map(
      INIT => X"08"
    )
    port map (
      I0 => state_count(0),
      I1 => state_count(1),
      I2 => state_count(2),
      O => Q_n0294
    );
  Q_n02698 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0295,
      I1 => ecc4(7),
      I2 => Q_n0296,
      I3 => ecc5(7),
      O => CHOICE31
    );
  Q_n02678 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0295,
      I1 => ecc4(5),
      I2 => Q_n0296,
      I3 => ecc5(5),
      O => CHOICE48
    );
  Q_n026674 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => current_state(2),
      I1 => current_state(5),
      I2 => CHOICE117,
      O => CHOICE118
    );
  Q_n026510 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0291,
      I1 => ecc0(3),
      I2 => Q_n0292,
      I3 => ecc1(3),
      O => CHOICE85
    );
  Q_n026357 : LUT4
    generic map(
      INIT => X"FCF8"
    )
    port map (
      I0 => CHOICE169,
      I1 => N144,
      I2 => CHOICE182,
      I3 => CHOICE176,
      O => CHOICE183
    );
  Q_n025925 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => aes_count(4),
      I1 => aes_count(5),
      I2 => aes_count(6),
      I3 => aes_count(7),
      O => CHOICE13
    );
  Q_n02665 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0295,
      I1 => ecc4(4),
      I2 => Q_n0296,
      I3 => ecc5(4),
      O => CHOICE102
    );
  Q_n025926 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => CHOICE6,
      I1 => CHOICE13,
      O => control_b1
    );
  Q_n02644 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0020(2),
      I1 => current_state(6),
      I2 => cs_chroma(2),
      I3 => current_state(10),
      O => CHOICE62
    );
  state_count_1_rt_103 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => state_count(1),
      O => state_count_1_rt
    );
  Q_n026253_SW0 : LUT4
    generic map(
      INIT => X"CCC8"
    )
    port map (
      I0 => CHOICE151,
      I1 => N144,
      I2 => CHOICE148,
      I3 => CHOICE145,
      O => N1570
    );
  Q_n026876_SW0 : LUT4
    generic map(
      INIT => X"FCF8"
    )
    port map (
      I0 => CHOICE130,
      I1 => N144,
      I2 => CHOICE137,
      I3 => CHOICE123,
      O => N1554
    );
  Q_n026913 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0291,
      I1 => ecc0(7),
      I2 => Q_n0292,
      I3 => ecc1(7),
      O => CHOICE34
    );
  Q_n026215 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0293,
      I1 => ecc2(0),
      I2 => Q_n0294,
      I3 => ecc3(0),
      O => CHOICE151
    );
  Q_n026280 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => current_state(1),
      I1 => current_state(2),
      I2 => CHOICE163,
      O => CHOICE164
    );
  Q_n026713 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0291,
      I1 => ecc0(5),
      I2 => Q_n0292,
      I3 => ecc1(5),
      O => CHOICE51
    );
  Q_n027254 : LUT4_L
    generic map(
      INIT => X"F323"
    )
    port map (
      I0 => state_count(0),
      I1 => Q_n0056(0),
      I2 => Q_n0371,
      I3 => CHOICE245,
      LO => CHOICE251
    );
  Q_n026278 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0020(8),
      I1 => current_state(7),
      I2 => cs_chroma(0),
      I3 => current_state(10),
      O => CHOICE163
    );
  Q_n026810 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0291,
      I1 => ecc0(6),
      I2 => Q_n0292,
      I3 => ecc1(6),
      O => CHOICE126
    );
  Q_n026866 : LUT3
    generic map(
      INIT => X"EA"
    )
    port map (
      I0 => N137,
      I1 => Q_n0020(6),
      I2 => current_state(6),
      O => CHOICE139
    );
  Q_n026918 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0293,
      I1 => ecc2(7),
      I2 => Q_n0294,
      I3 => ecc3(7),
      O => CHOICE37
    );
  Q_n026647 : LUT4
    generic map(
      INIT => X"FCF8"
    )
    port map (
      I0 => CHOICE102,
      I1 => N144,
      I2 => current_state(1),
      I3 => CHOICE109,
      O => CHOICE112
    );
  video_count_4_rt_104 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => video_count(4),
      O => video_count_4_rt
    );
  Q_n026672 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0020(12),
      I1 => current_state(7),
      I2 => Q_n0020(4),
      I3 => current_state(6),
      O => CHOICE117
    );
  Q_n026563 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => current_state(5),
      I1 => N78,
      I2 => CHOICE96,
      O => CHOICE97
    );
  Q_n02649 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0295,
      I1 => ecc4(2),
      I2 => Q_n0296,
      I3 => ecc5(2),
      O => CHOICE65
    );
  Q_n026718 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0293,
      I1 => ecc2(5),
      I2 => Q_n0294,
      I3 => ecc3(5),
      O => CHOICE54
    );
  Q_n026210 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0291,
      I1 => ecc0(0),
      I2 => Q_n0292,
      I3 => ecc1(0),
      O => CHOICE148
    );
  video_count_5_rt_105 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => video_count(5),
      O => video_count_5_rt
    );
  Q_n026616 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => CHOICE105,
      I1 => CHOICE108,
      O => CHOICE109
    );
  Q_n026863 : LUT4
    generic map(
      INIT => X"C040"
    )
    port map (
      I0 => Q_n0371,
      I1 => current_state(8),
      I2 => control_b1,
      I3 => N147,
      O => CHOICE137
    );
  Q_n026738 : LUT4
    generic map(
      INIT => X"CCC8"
    )
    port map (
      I0 => CHOICE48,
      I1 => N144,
      I2 => CHOICE51,
      I3 => CHOICE54,
      O => CHOICE57
    );
  Q_n026610 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0291,
      I1 => ecc0(4),
      I2 => Q_n0292,
      I3 => ecc1(4),
      O => CHOICE105
    );
  Ker7815 : LUT4
    generic map(
      INIT => X"0008"
    )
    port map (
      I0 => audio_chan(0),
      I1 => current_state(8),
      I2 => audio_chan(2),
      I3 => state_count(0),
      O => CHOICE22
    );
  Q_n02655 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0295,
      I1 => ecc4(3),
      I2 => Q_n0296,
      I3 => ecc5(3),
      O => CHOICE82
    );
  Q_n026316 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => CHOICE172,
      I1 => CHOICE175,
      O => CHOICE176
    );
  Q_n026253 : LUT4
    generic map(
      INIT => X"FF20"
    )
    port map (
      I0 => current_state(3),
      I1 => audio_group(2),
      I2 => audio_group(0),
      I3 => N1570,
      O => CHOICE158
    );
  Q_n026315 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0293,
      I1 => ecc2(1),
      I2 => Q_n0294,
      I3 => ecc3(1),
      O => CHOICE175
    );
  Ker7819 : LUT4
    generic map(
      INIT => X"2000"
    )
    port map (
      I0 => Q_n0371,
      I1 => state_count(1),
      I2 => control_b1,
      I3 => CHOICE22,
      O => CHOICE23
    );
  Q_n026949 : LUT3
    generic map(
      INIT => X"EA"
    )
    port map (
      I0 => CHOICE40,
      I1 => cs_chroma(7),
      I2 => current_state(10),
      O => CHOICE41
    );
  Q_n026938 : LUT4
    generic map(
      INIT => X"CCC8"
    )
    port map (
      I0 => CHOICE31,
      I1 => N144,
      I2 => CHOICE34,
      I3 => CHOICE37,
      O => CHOICE40
    );
  Q_n026816 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => CHOICE126,
      I1 => CHOICE129,
      O => CHOICE130
    );
  Q_n026515 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0293,
      I1 => ecc2(3),
      I2 => Q_n0294,
      I3 => ecc3(3),
      O => CHOICE88
    );
  Q_n0223_0_1 : LUT2_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => N4,
      I1 => video_count(0),
      LO => Q_n0223(0)
    );
  Q_n02625 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0295,
      I1 => ecc4(0),
      I2 => Q_n0296,
      I3 => ecc5(0),
      O => CHOICE145
    );
  Q_n0224_0_20 : LUT4
    generic map(
      INIT => X"2000"
    )
    port map (
      I0 => video_count(7),
      I1 => video_count(6),
      I2 => video_count(11),
      I3 => CHOICE232,
      O => CHOICE233
    );
  Ker411 : LUT2_L
    generic map(
      INIT => X"D"
    )
    port map (
      I0 => video_count(4),
      I1 => video_count(10),
      LO => CHOICE208
    );
  Q_n0235_1_56_SW0 : LUT4_L
    generic map(
      INIT => X"01FF"
    )
    port map (
      I0 => current_state(8),
      I1 => N134,
      I2 => current_state(9),
      I3 => sample_pos(1),
      LO => N1558
    );
  Q_n0272207 : LUT4
    generic map(
      INIT => X"EAAE"
    )
    port map (
      I0 => N1574,
      I1 => current_state(6),
      I2 => Mxor_n0022_Xo(5),
      I3 => Mxor_n0022_Xo(4),
      O => CHOICE281
    );
  Ker473 : LUT4
    generic map(
      INIT => X"FEF4"
    )
    port map (
      I0 => video_count(11),
      I1 => CHOICE219,
      I2 => CHOICE206,
      I3 => CHOICE212,
      O => N4
    );
  Q_n027297 : LUT4
    generic map(
      INIT => X"2F22"
    )
    port map (
      I0 => Q_n0296,
      I1 => Q_n0132(0),
      I2 => Q_n0129(0),
      I3 => Q_n0295,
      O => CHOICE259
    );
  Q_n027077 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0026(0),
      I1 => current_state(7),
      I2 => cs_chroma(8),
      I3 => current_state(10),
      O => CHOICE362
    );
  Q_n0272181 : LUT3
    generic map(
      INIT => X"F4"
    )
    port map (
      I0 => Q_n0026(0),
      I1 => current_state(7),
      I2 => CHOICE273,
      O => CHOICE274
    );
  Ker1461 : LUT4_D
    generic map(
      INIT => X"3332"
    )
    port map (
      I0 => current_state(8),
      I1 => sync_reset_in_IBUF,
      I2 => current_state(9),
      I3 => N134,
      LO => N1597,
      O => N146
    );
  Q_n027019 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0117(0),
      I1 => Q_n0291,
      I2 => Q_n0120(0),
      I3 => Q_n0292,
      O => CHOICE347
    );
  Q_n03101 : LUT4
    generic map(
      INIT => X"2000"
    )
    port map (
      I0 => state_count(1),
      I1 => state_count(2),
      I2 => N1584,
      I3 => state_count(0),
      O => Q_n0310
    );
  Ker817 : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => video_count(6),
      I1 => video_count(2),
      I2 => video_count(4),
      O => CHOICE194
    );
  Q_n0228_2_38 : LUT3_L
    generic map(
      INIT => X"08"
    )
    port map (
      I0 => CHOICE313,
      I1 => current_state(10),
      I2 => Q_n0368,
      LO => CHOICE316
    );
  Q_n0225_0_48 : LUT4_L
    generic map(
      INIT => X"32E2"
    )
    port map (
      I0 => N9,
      I1 => state_count(0),
      I2 => current_state(9),
      I3 => N1560,
      LO => CHOICE334
    );
  Q_n0238_2_5 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => counter_buffer_0_2,
      I1 => N110,
      I2 => temp_clk_counter(2),
      I3 => N1582,
      O => CHOICE416
    );
  temp_clk_counter_3_rt_106 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(3),
      O => temp_clk_counter_3_rt
    );
  Q_n0272251 : LUT3_L
    generic map(
      INIT => X"EA"
    )
    port map (
      I0 => CHOICE286,
      I1 => current_state(8),
      I2 => CHOICE251,
      LO => Q_n0272
    );
  Q_n0225_1_82 : LUT4_L
    generic map(
      INIT => X"2033"
    )
    port map (
      I0 => N9,
      I1 => sync_reset_in_IBUF,
      I2 => Q_n0244(1),
      I3 => N1552,
      LO => Q_n0225(1)
    );
  Ker421 : LUT4
    generic map(
      INIT => X"FEFF"
    )
    port map (
      I0 => CHOICE208,
      I1 => video_count(3),
      I2 => video_count(6),
      I3 => video_count(2),
      O => CHOICE212
    );
  Q_n027229 : LUT4
    generic map(
      INIT => X"7776"
    )
    port map (
      I0 => state_count(0),
      I1 => state_count(1),
      I2 => audio_chan(2),
      I3 => N1566,
      O => CHOICE245
    );
  Q_n0272211 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => current_state(12),
      I1 => N138,
      I2 => CHOICE281,
      O => CHOICE282
    );
  Q_n0225_0_48_SW0 : LUT3
    generic map(
      INIT => X"2A"
    )
    port map (
      I0 => N1401,
      I1 => state_count(1),
      I2 => state_count(2),
      O => N1560
    );
  Q_n0236_2_38 : LUT4
    generic map(
      INIT => X"6860"
    )
    port map (
      I0 => audio_chan(0),
      I1 => audio_chan(1),
      I2 => audio_chan(2),
      I3 => Q_n0310,
      O => CHOICE377
    );
  Q_n0272207_SW1 : LUT3
    generic map(
      INIT => X"C4"
    )
    port map (
      I0 => audio_group(1),
      I1 => current_state(3),
      I2 => audio_group(2),
      O => N1574
    );
  Q_n0225_2_42 : LUT4_L
    generic map(
      INIT => X"2033"
    )
    port map (
      I0 => N9,
      I1 => sync_reset_in_IBUF,
      I2 => Q_n0244(2),
      I3 => N1550,
      LO => Q_n0225(2)
    );
  Q_n027014 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0129(0),
      I1 => Q_n0295,
      I2 => Q_n0132(0),
      I3 => Q_n0296,
      O => CHOICE344
    );
  Q_n0272119 : LUT4
    generic map(
      INIT => X"2F22"
    )
    port map (
      I0 => Q_n0294,
      I1 => Q_n0126(0),
      I2 => Q_n0123(0),
      I3 => Q_n0293,
      O => CHOICE269
    );
  Ker8118 : LUT4
    generic map(
      INIT => X"0008"
    )
    port map (
      I0 => video_count(10),
      I1 => video_count(3),
      I2 => video_count(7),
      I3 => video_count(11),
      O => CHOICE199
    );
  aes_count_6_rt_107 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => aes_count(6),
      LO => aes_count_6_rt
    );
  Ker433 : LUT2_L
    generic map(
      INIT => X"D"
    )
    port map (
      I0 => video_count(6),
      I1 => video_count(2),
      LO => CHOICE215
    );
  Q_n027061 : LUT4
    generic map(
      INIT => X"FF20"
    )
    port map (
      I0 => current_state(3),
      I1 => audio_group(2),
      I2 => audio_group(1),
      I3 => N1572,
      O => CHOICE357
    );
  Q_n0225_1_82_SW0 : LUT4_L
    generic map(
      INIT => X"153F"
    )
    port map (
      I0 => Q_n0405,
      I1 => CHOICE393,
      I2 => current_state(9),
      I3 => state_count(1),
      LO => N1552
    );
  Q_n0235_1_29 : LUT4
    generic map(
      INIT => X"2A6A"
    )
    port map (
      I0 => sample_pos(1),
      I1 => Q_n0289,
      I2 => sample_pos(0),
      I3 => sample_per_line(0),
      O => CHOICE298
    );
  Q_n027024 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0123(0),
      I1 => Q_n0293,
      I2 => Q_n0126(0),
      I3 => Q_n0294,
      O => CHOICE350
    );
  Q_n027229_SW0 : LUT3
    generic map(
      INIT => X"7F"
    )
    port map (
      I0 => audio_chan(0),
      I1 => CHOICE6,
      I2 => CHOICE13,
      O => N1566
    );
  Q_n0225_2_42_SW0 : LUT4_L
    generic map(
      INIT => X"153F"
    )
    port map (
      I0 => Q_n0405,
      I1 => CHOICE406,
      I2 => current_state(9),
      I3 => state_count(2),
      LO => N1550
    );
  Ker8130 : LUT3_D
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => Q_n0280,
      I1 => CHOICE194,
      I2 => CHOICE199,
      LO => N1598,
      O => N81
    );
  Q_n0236_2_0 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => audio_chan(2),
      I1 => N101,
      LO => CHOICE366
    );
  video_count_7_rt_108 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => video_count(7),
      O => video_count_7_rt
    );
  Q_n0272120 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => CHOICE264,
      I1 => CHOICE269,
      O => CHOICE270
    );
  state_count_2_rt_109 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => state_count(2),
      O => state_count_2_rt
    );
  Q_n027095 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => Q_n0056(0),
      I1 => N1548,
      I2 => CHOICE357,
      I3 => CHOICE363,
      O => Q_n0270
    );
  Q_n0272108 : LUT4
    generic map(
      INIT => X"2F22"
    )
    port map (
      I0 => Q_n0292,
      I1 => Q_n0120(0),
      I2 => Q_n0117(0),
      I3 => Q_n0291,
      O => CHOICE264
    );
  Q_n0225_1_37 : LUT4
    generic map(
      INIT => X"9AAA"
    )
    port map (
      I0 => state_count(1),
      I1 => state_count(2),
      I2 => state_count(0),
      I3 => N1401,
      O => CHOICE393
    );
  Q_n0228_2_32 : LUT3
    generic map(
      INIT => X"68"
    )
    port map (
      I0 => audio_group(0),
      I1 => audio_group(1),
      I2 => audio_group(2),
      O => CHOICE313
    );
  Ker443 : LUT4
    generic map(
      INIT => X"FEFF"
    )
    port map (
      I0 => CHOICE215,
      I1 => video_count(4),
      I2 => video_count(7),
      I3 => video_count(3),
      O => CHOICE219
    );
  Q_n0236_1_G : LUT4_L
    generic map(
      INIT => X"0F04"
    )
    port map (
      I0 => audio_chan(0),
      I1 => current_state(8),
      I2 => sync_reset_in_IBUF,
      I3 => N101,
      LO => N1581
    );
  Q_n0272166 : LUT4
    generic map(
      INIT => X"FCF8"
    )
    port map (
      I0 => CHOICE259,
      I1 => N144,
      I2 => current_state(11),
      I3 => CHOICE270,
      O => CHOICE273
    );
  Q_n0224_0_16 : LUT4
    generic map(
      INIT => X"0008"
    )
    port map (
      I0 => video_count(2),
      I1 => video_count(4),
      I2 => video_count(10),
      I3 => video_count(3),
      O => CHOICE232
    );
  XNor_stagelut5 : LUT4_L
    generic map(
      INIT => X"1E0F"
    )
    port map (
      I0 => clkmux_in_2_IBUF,
      I1 => clkmux_in_1_IBUF,
      I2 => counter(17),
      I3 => clkmux_in_0_IBUF,
      LO => N43
    );
  Q_n0238_2_8 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => CHOICE416,
      I1 => nor_cyo7,
      LO => CHOICE418
    );
  Q_n027925_SW0 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => N1591,
      I1 => next_aes_count(2),
      I2 => next_aes_count(1),
      I3 => next_aes_count(0),
      O => N1544
    );
  Q_n0238_11_8 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => CHOICE488,
      I1 => nor_cyo7,
      LO => CHOICE490
    );
  Q_n0238_11_5 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => counter_buffer_0_11,
      I1 => N110,
      I2 => temp_clk_counter(11),
      I3 => N143,
      O => CHOICE488
    );
  Q_n0238_8_8 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => CHOICE461,
      I1 => nor_cyo7,
      LO => CHOICE463
    );
  Q_n0238_0_8 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => CHOICE506,
      I1 => nor_cyo7,
      LO => CHOICE508
    );
  Q_n0238_5_8 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => CHOICE443,
      I1 => nor_cyo7,
      LO => CHOICE445
    );
  audio_clk_n0256_10_lut : LUT4_L
    generic map(
      INIT => X"1E0F"
    )
    port map (
      I0 => clkmux_in_2_IBUF,
      I1 => clkmux_in_1_IBUF,
      I2 => counter(10),
      I3 => clkmux_in_0_IBUF,
      LO => N59
    );
  Q_n0238_6_8 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => CHOICE452,
      I1 => nor_cyo7,
      LO => CHOICE454
    );
  Q_n0238_3_5 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => counter_buffer_0_3,
      I1 => N110,
      I2 => temp_clk_counter(3),
      I3 => N143,
      O => CHOICE425
    );
  Q_n0238_10_5 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => counter_buffer_0_10,
      I1 => N110,
      I2 => temp_clk_counter(10),
      I3 => N143,
      O => CHOICE497
    );
  Q_n0238_1_8 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => CHOICE515,
      I1 => nor_cyo7,
      LO => CHOICE517
    );
  Q_n0238_8_5 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => counter_buffer_0_8,
      I1 => N110,
      I2 => temp_clk_counter(8),
      I3 => N143,
      O => CHOICE461
    );
  Q_n0238_4_5 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => counter_buffer_0_4,
      I1 => N110,
      I2 => temp_clk_counter(4),
      I3 => N143,
      O => CHOICE434
    );
  Q_n0238_7_8 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => CHOICE470,
      I1 => nor_cyo7,
      LO => CHOICE472
    );
  Q_n0238_0_5 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => counter_buffer_0_0,
      I1 => N110,
      I2 => temp_clk_counter(0),
      I3 => N143,
      O => CHOICE506
    );
  Ker0_SW0 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => current_state(12),
      I1 => current_state(11),
      I2 => current_state(8),
      I3 => current_state(9),
      O => N1513
    );
  Q_n0238_1_5 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => counter_buffer_0_1,
      I1 => N110,
      I2 => temp_clk_counter(1),
      I3 => N143,
      O => CHOICE515
    );
  Q_n0238_9_5 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => counter_buffer_0_9,
      I1 => N110,
      I2 => temp_clk_counter(9),
      I3 => N143,
      O => CHOICE479
    );
  Q_n0238_3_8 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => CHOICE425,
      I1 => nor_cyo7,
      LO => CHOICE427
    );
  Q_n027910 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => N1590,
      I1 => next_aes_count(5),
      O => CHOICE527
    );
  Q_n0238_4_8 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => CHOICE434,
      I1 => nor_cyo7,
      LO => CHOICE436
    );
  Q_n0238_9_8 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => CHOICE479,
      I1 => nor_cyo7,
      LO => CHOICE481
    );
  Q_n0238_6_5 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => counter_buffer_0_6,
      I1 => N110,
      I2 => temp_clk_counter(6),
      I3 => N143,
      O => CHOICE452
    );
  Q_n0238_7_5 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => counter_buffer_0_7,
      I1 => N110,
      I2 => temp_clk_counter(7),
      I3 => N143,
      O => CHOICE470
    );
  Q_n0238_5_5 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => counter_buffer_0_5,
      I1 => N110,
      I2 => temp_clk_counter(5),
      I3 => N143,
      O => CHOICE443
    );
  videoclk_in_BUFGP_110 : BUFGP
    port map (
      I => videoclk_in,
      O => videoclk_in_BUFGP
    );
  sync_reset_in_IBUF_111 : IBUF
    port map (
      I => sync_reset_in,
      O => sync_reset_in_IBUF
    );
  reset_in_IBUF_112 : IBUF
    port map (
      I => reset_in,
      O => reset_in_IBUF
    );
  clkmux_in_2_IBUF_113 : IBUF
    port map (
      I => clkmux_in(2),
      O => clkmux_in_2_IBUF
    );
  clkmux_in_1_IBUF_114 : IBUF
    port map (
      I => clkmux_in(1),
      O => clkmux_in_1_IBUF
    );
  clkmux_in_0_IBUF_115 : IBUF
    port map (
      I => clkmux_in(0),
      O => clkmux_in_0_IBUF
    );
  audioclk_out_OBUF_116 : OBUF
    port map (
      I => audioclk_out_OBUF,
      O => audioclk_out
    );
  audioluma_out_9_OBUF : OBUF
    port map (
      I => output_counter_buffer(2, 0),
      O => audioluma_out(9)
    );
  temp_clk_counter_10_rt_117 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(10),
      O => temp_clk_counter_10_rt
    );
  temp_clk_counter_9_rt_118 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(9),
      O => temp_clk_counter_9_rt
    );
  temp_clk_counter_8_rt_119 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(8),
      O => temp_clk_counter_8_rt
    );
  temp_clk_counter_7_rt_120 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(7),
      O => temp_clk_counter_7_rt
    );
  temp_clk_counter_4_rt_121 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(4),
      O => temp_clk_counter_4_rt
    );
  temp_clk_counter_6_rt_122 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(6),
      O => temp_clk_counter_6_rt
    );
  temp_clk_counter_5_rt_123 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => temp_clk_counter(5),
      O => temp_clk_counter_5_rt
    );
  counter_6_rt_124 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter(6),
      LO => counter_6_rt
    );
  counter_4_rt_125 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter(4),
      LO => counter_4_rt
    );
  counter_0_rt_126 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => counter(0),
      LO => counter_0_rt
    );
  aes_count_1_rt_127 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => aes_count(1),
      LO => aes_count_1_rt
    );
  aes_count_2_rt_128 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => aes_count(2),
      LO => aes_count_2_rt
    );
  aes_count_3_rt_129 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => aes_count(3),
      LO => aes_count_3_rt
    );
  aes_count_4_rt_130 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => aes_count(4),
      LO => aes_count_4_rt
    );
  Q_n027095_SW0 : LUT4
    generic map(
      INIT => X"A222"
    )
    port map (
      I0 => current_state(8),
      I1 => Q_n0371,
      I2 => state_count(0),
      I3 => state_count(1),
      O => N1548
    );
  Q_n0459_BUFG : BUFG
    port map (
      I => Q_n04591,
      O => Q_n0459
    );
  Q_n0228_1_Q : MUXF5
    port map (
      I0 => N1576,
      I1 => N1577,
      S => Q_n0365,
      O => Q_n0228(1)
    );
  Q_n0228_1_F : LUT4
    generic map(
      INIT => X"1200"
    )
    port map (
      I0 => audio_group(1),
      I1 => sync_reset_in_IBUF,
      I2 => audio_group(0),
      I3 => current_state(10),
      O => N1576
    );
  Q_n0236_0_Q : MUXF5
    port map (
      I0 => N1578,
      I1 => N1579,
      S => audio_chan(0),
      O => Q_n0236(0)
    );
  Q_n0236_0_F : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => current_state(8),
      I1 => Q_n0310,
      I2 => sync_reset_in_IBUF,
      I3 => current_state(11),
      O => N1578
    );
  Q_n0236_1_Q : MUXF5
    port map (
      I0 => N1580,
      I1 => N1581,
      S => audio_chan(1),
      O => Q_n0236(1)
    );
  Q_n0236_1_F : LUT4
    generic map(
      INIT => X"0008"
    )
    port map (
      I0 => N147,
      I1 => N1401,
      I2 => state_count(2),
      I3 => N673,
      O => N1580
    );

end Structure;

