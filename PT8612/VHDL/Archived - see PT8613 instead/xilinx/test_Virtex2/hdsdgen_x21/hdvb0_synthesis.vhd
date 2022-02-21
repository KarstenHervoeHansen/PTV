--------------------------------------------------------------------------------
-- Copyright (c) 1995-2005 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____
--  /   /\/   /
-- /___/  \  /    Vendor: Xilinx
-- \   \   \/     Version: H.41
--  \   \         Application: netgen
--  /   /         Filename: hdvb0_synthesis.vhd
-- /___/   /\     Timestamp: Thu Oct 06 10:20:56 2005
-- \   \  /  \ 
--  \___\/\___\
--             
-- Command	: -intstyle ise -ar Structure -w -ofmt vhdl -sim hdvb0.ngc hdvb0_synthesis.vhd 
-- Device	: xc4vfx20-12-ff672
-- Input file	: hdvb0.ngc
-- Output file	: hdvb0_synthesis.vhd
-- # of Entities	: 1
-- Design Name	: hdvb0
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

entity hdvb0 is
  port (
    reset_n : in STD_LOGIC := 'X'; 
    CS : in STD_LOGIC := 'X'; 
    f7417 : in STD_LOGIC := 'X'; 
    f7425 : in STD_LOGIC := 'X'; 
    MOSI : in STD_LOGIC := 'X'; 
    SCK : in STD_LOGIC := 'X'; 
    TP1 : out STD_LOGIC; 
    TP2 : out STD_LOGIC; 
    TP3 : out STD_LOGIC; 
    TP4 : out STD_LOGIC; 
    TXp_OUT_113a : out STD_LOGIC; 
    TXp_OUT_113b : out STD_LOGIC; 
    TXn_OUT_113a : out STD_LOGIC; 
    TXn_OUT_113b : out STD_LOGIC; 
    Debug9 : out STD_LOGIC_VECTOR ( 9 downto 0 ) 
  );
end hdvb0;

architecture Structure of hdvb0 is
  signal reset_n_IBUF : STD_LOGIC; 
  signal CS_IBUF : STD_LOGIC; 
  signal mod0_sd_hd : STD_LOGIC; 
  signal TP4_OBUF : STD_LOGIC; 
  signal f7417_IBUFG : STD_LOGIC; 
  signal f7425_IBUFG : STD_LOGIC; 
  signal MOSI_IBUF : STD_LOGIC; 
  signal SCK_BUFGP : STD_LOGIC; 
  signal mod7_counter_state_counter_n0009_20_cyo : STD_LOGIC; 
  signal CHOICE14403 : STD_LOGIC; 
  signal mod7_counter_count_4_n0001 : STD_LOGIC; 
  signal mod5_hdframe_line_clk : STD_LOGIC; 
  signal TXRIOA_USRCLK : STD_LOGIC; 
  signal TXRIOA_REFCLK : STD_LOGIC; 
  signal txclka2 : STD_LOGIC; 
  signal mod5_hdframe_sav : STD_LOGIC; 
  signal tx_clk1_out_113a : STD_LOGIC; 
  signal sif_rst : STD_LOGIC; 
  signal CHOICE14728 : STD_LOGIC; 
  signal mod0_n0271 : STD_LOGIC; 
  signal N8093 : STD_LOGIC; 
  signal mod0_serial_interface_bitptr_n0000_3_cyo : STD_LOGIC; 
  signal mod7_counter_N5 : STD_LOGIC; 
  signal mod7_counter_state_counter_n0009_14_cyo : STD_LOGIC; 
  signal mod7_counter_N6 : STD_LOGIC; 
  signal mod7_counter_count_0_n0001 : STD_LOGIC; 
  signal mod7_counter_N4 : STD_LOGIC; 
  signal mod7_counter_state_counter_n0009_13_cyo : STD_LOGIC; 
  signal txclka : STD_LOGIC; 
  signal mod7_counter_count_5_n0001 : STD_LOGIC; 
  signal mod7_counter_count_1_n0000 : STD_LOGIC; 
  signal mod7_counter_count_4_n0000 : STD_LOGIC; 
  signal mod7_counter_N10 : STD_LOGIC; 
  signal mod7_counter_count_6_n0000 : STD_LOGIC; 
  signal mod7_counter_count_0_n0000 : STD_LOGIC; 
  signal mod7_counter_count_2_n0001 : STD_LOGIC; 
  signal mod7_counter_N01 : STD_LOGIC; 
  signal mod6_reset_sync : STD_LOGIC; 
  signal mod6_scram20_inst_N11 : STD_LOGIC; 
  signal mod6_scram20_inst_N13 : STD_LOGIC; 
  signal mod6_scram20_inst_Mxor_n0041_net3 : STD_LOGIC; 
  signal mod6_scram20_inst_Mxor_n0041_net0 : STD_LOGIC; 
  signal mod6_scram20_inst_n0001 : STD_LOGIC; 
  signal mod6_scram20_inst_n0010 : STD_LOGIC; 
  signal mod6_scram20_inst_n0006 : STD_LOGIC; 
  signal mod6_scram20_inst_n0011 : STD_LOGIC; 
  signal mod6_scram20_inst_n0007 : STD_LOGIC; 
  signal mod6_scram20_inst_n0012 : STD_LOGIC; 
  signal mod6_scram20_inst_n0008 : STD_LOGIC; 
  signal mod6_scram20_inst_n0013 : STD_LOGIC; 
  signal mod6_scram20_inst_n0009 : STD_LOGIC; 
  signal mod6_scram20_inst_n0014 : STD_LOGIC; 
  signal mod6_scram20_inst_n0023 : STD_LOGIC; 
  signal mod6_scram20_inst_n0025 : STD_LOGIC; 
  signal mod6_scram20_inst_n0030 : STD_LOGIC; 
  signal mod6_scram20_inst_n0031 : STD_LOGIC; 
  signal mod6_scram20_inst_n0027 : STD_LOGIC; 
  signal mod6_scram20_inst_n0032 : STD_LOGIC; 
  signal mod6_scram20_inst_n0028 : STD_LOGIC; 
  signal mod6_scram20_inst_n0033 : STD_LOGIC; 
  signal mod6_scram20_inst_n0029 : STD_LOGIC; 
  signal mod6_scram20_inst_N19 : STD_LOGIC; 
  signal mod6_scram20_inst_n0037 : STD_LOGIC; 
  signal N8155 : STD_LOGIC; 
  signal mod6_scram20_inst_N23 : STD_LOGIC; 
  signal mod6_scram20_inst_N9 : STD_LOGIC; 
  signal CHOICE14810 : STD_LOGIC; 
  signal mod6_scram20_inst_N25 : STD_LOGIC; 
  signal mod6_scram20_inst_N2 : STD_LOGIC; 
  signal N8253 : STD_LOGIC; 
  signal mod6_scram20_inst_N21 : STD_LOGIC; 
  signal mod6_scram20_inst_N17 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_8_rt1 : STD_LOGIC; 
  signal mod6_scram20_inst_Mxor_n0041_net1 : STD_LOGIC; 
  signal mod6_scram20_inst_Mxor_n0041_net2 : STD_LOGIC; 
  signal mod6_scram20_inst_N7 : STD_LOGIC; 
  signal mod6_scram20_inst_N15 : STD_LOGIC; 
  signal mod7_h_state_FFd1_N0 : STD_LOGIC; 
  signal mod7_N300 : STD_LOGIC; 
  signal N8247 : STD_LOGIC; 
  signal mod7_n0315_28_Q : STD_LOGIC; 
  signal mod7_N299 : STD_LOGIC; 
  signal mod7_yramp_en : STD_LOGIC; 
  signal CHOICE14815 : STD_LOGIC; 
  signal mod7_n0022 : STD_LOGIC; 
  signal mod7_n0023 : STD_LOGIC; 
  signal mod7_n0024 : STD_LOGIC; 
  signal mod7_n0025 : STD_LOGIC; 
  signal mod7_n0030 : STD_LOGIC; 
  signal mod7_n0026 : STD_LOGIC; 
  signal mod7_n0031 : STD_LOGIC; 
  signal mod7_n0027 : STD_LOGIC; 
  signal mod7_n0032 : STD_LOGIC; 
  signal mod7_n0028 : STD_LOGIC; 
  signal mod7_n0029 : STD_LOGIC; 
  signal mod7_n0035 : STD_LOGIC; 
  signal N8033 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_N4 : STD_LOGIC; 
  signal mod7_tp_17_Q : STD_LOGIC; 
  signal mod7_N231 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_tp_22_MUXF5 : STD_LOGIC; 
  signal N8243 : STD_LOGIC; 
  signal mod7_n0052 : STD_LOGIC; 
  signal mod7_tp_5_Q : STD_LOGIC; 
  signal mod7_N124 : STD_LOGIC; 
  signal mod7_tp_19_Q : STD_LOGIC; 
  signal mod7_N282 : STD_LOGIC; 
  signal CHOICE15150 : STD_LOGIC; 
  signal mod7_N233 : STD_LOGIC; 
  signal mod7_n0061 : STD_LOGIC; 
  signal mod7_N225 : STD_LOGIC; 
  signal mod7_N257 : STD_LOGIC; 
  signal mod7_N302 : STD_LOGIC; 
  signal mod7_N241 : STD_LOGIC; 
  signal mod7_N199 : STD_LOGIC; 
  signal N8433 : STD_LOGIC; 
  signal CHOICE15451 : STD_LOGIC; 
  signal mod7_N202 : STD_LOGIC; 
  signal N8464 : STD_LOGIC; 
  signal mod7_N242 : STD_LOGIC; 
  signal mod7_N260 : STD_LOGIC; 
  signal mod7_N133 : STD_LOGIC; 
  signal mod7_N227 : STD_LOGIC; 
  signal mod7_n0065 : STD_LOGIC; 
  signal mod7_n0070 : STD_LOGIC; 
  signal mod7_N235 : STD_LOGIC; 
  signal mod7_n0066 : STD_LOGIC; 
  signal mod7_n0071 : STD_LOGIC; 
  signal mod7_n0067 : STD_LOGIC; 
  signal mod7_N57 : STD_LOGIC; 
  signal CHOICE14363 : STD_LOGIC; 
  signal mod7_n0122 : STD_LOGIC; 
  signal mod7_n0068 : STD_LOGIC; 
  signal mod7_N223 : STD_LOGIC; 
  signal mod7_counter_state_counter_n0009_19_cyo : STD_LOGIC; 
  signal mod7_n0069 : STD_LOGIC; 
  signal N8377 : STD_LOGIC; 
  signal CHOICE15152 : STD_LOGIC; 
  signal mod7_N93 : STD_LOGIC; 
  signal mod7_n0075 : STD_LOGIC; 
  signal mod7_n0315_14_Q : STD_LOGIC; 
  signal N8472 : STD_LOGIC; 
  signal mod7_h_state_FFd3 : STD_LOGIC; 
  signal mod7_n0076 : STD_LOGIC; 
  signal mod7_counter_state_counter_n0009_15_cyo : STD_LOGIC; 
  signal mod7_N253 : STD_LOGIC; 
  signal mod7_tp_22_Q : STD_LOGIC; 
  signal mod7_n0077 : STD_LOGIC; 
  signal mod7_video_sm_active_line_n0000_5_cyo : STD_LOGIC; 
  signal mod7_N120 : STD_LOGIC; 
  signal mod7_N132 : STD_LOGIC; 
  signal mod7_n0078 : STD_LOGIC; 
  signal mod7_N277 : STD_LOGIC; 
  signal mod7_tp_20_Q : STD_LOGIC; 
  signal mod7_n0315_15_Q : STD_LOGIC; 
  signal mod7_n0079 : STD_LOGIC; 
  signal mod7_N278 : STD_LOGIC; 
  signal mod7_N105 : STD_LOGIC; 
  signal mod7_h_state_FFd2 : STD_LOGIC; 
  signal mod7_N279 : STD_LOGIC; 
  signal mod7_N236 : STD_LOGIC; 
  signal mod7_N229 : STD_LOGIC; 
  signal mod7_n0086 : STD_LOGIC; 
  signal mod7_n0271 : STD_LOGIC; 
  signal mod7_N84 : STD_LOGIC; 
  signal mod7_N256 : STD_LOGIC; 
  signal mod7_video_sm_active_line_n0000_6_cyo : STD_LOGIC; 
  signal mod7_n0315_6_Q : STD_LOGIC; 
  signal mod7_N181 : STD_LOGIC; 
  signal mod7_N118 : STD_LOGIC; 
  signal mod7_h_state_FFd2_In : STD_LOGIC; 
  signal mod7_N298 : STD_LOGIC; 
  signal mod7_N35 : STD_LOGIC; 
  signal mod7_tp_0_Q : STD_LOGIC; 
  signal mod7_N304 : STD_LOGIC; 
  signal mod7_N285 : STD_LOGIC; 
  signal mod7_N79 : STD_LOGIC; 
  signal mod7_n0150 : STD_LOGIC; 
  signal mod7_N49 : STD_LOGIC; 
  signal mod7_N182 : STD_LOGIC; 
  signal mod7_N230 : STD_LOGIC; 
  signal mod7_N292 : STD_LOGIC; 
  signal CHOICE14797 : STD_LOGIC; 
  signal mod7_N251 : STD_LOGIC; 
  signal mod7_N75 : STD_LOGIC; 
  signal mod7_N234 : STD_LOGIC; 
  signal mod7_N36 : STD_LOGIC; 
  signal mod7_n0099 : STD_LOGIC; 
  signal N8439 : STD_LOGIC; 
  signal CHOICE14747 : STD_LOGIC; 
  signal mod7_N25 : STD_LOGIC; 
  signal mod7_N72 : STD_LOGIC; 
  signal mod7_N252 : STD_LOGIC; 
  signal mod7_N263 : STD_LOGIC; 
  signal mod7_N276 : STD_LOGIC; 
  signal mod7_N249 : STD_LOGIC; 
  signal mod7_N217 : STD_LOGIC; 
  signal mod7_N139 : STD_LOGIC; 
  signal mod7_N143 : STD_LOGIC; 
  signal CHOICE14384 : STD_LOGIC; 
  signal mod7_N259 : STD_LOGIC; 
  signal N8460 : STD_LOGIC; 
  signal mod7_N293 : STD_LOGIC; 
  signal mod7_N287 : STD_LOGIC; 
  signal mod7_N270 : STD_LOGIC; 
  signal mod7_N137 : STD_LOGIC; 
  signal mod7_N142 : STD_LOGIC; 
  signal mod7_tp_1_Q : STD_LOGIC; 
  signal mod7_N240 : STD_LOGIC; 
  signal mod7_N264 : STD_LOGIC; 
  signal mod7_N115 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_N2 : STD_LOGIC; 
  signal mod7_N7 : STD_LOGIC; 
  signal CHOICE14873 : STD_LOGIC; 
  signal mod7_N273 : STD_LOGIC; 
  signal mod7_N10 : STD_LOGIC; 
  signal mod7_N28 : STD_LOGIC; 
  signal mod7_N61 : STD_LOGIC; 
  signal mod7_n0223 : STD_LOGIC; 
  signal mod7_N216 : STD_LOGIC; 
  signal mod7_N54 : STD_LOGIC; 
  signal mod7_h_state_FFd1 : STD_LOGIC; 
  signal mod7_N6 : STD_LOGIC; 
  signal mod7_N173 : STD_LOGIC; 
  signal mod7_N271 : STD_LOGIC; 
  signal mod7_h_state_FFd4 : STD_LOGIC; 
  signal mod7_tp_4_Q : STD_LOGIC; 
  signal mod7_video_sm_active_line_n0000_7_cyo : STD_LOGIC; 
  signal mod7_h_state_FFd4_In : STD_LOGIC; 
  signal mod7_N250 : STD_LOGIC; 
  signal mod7_N4 : STD_LOGIC; 
  signal mod7_n0176 : STD_LOGIC; 
  signal mod7_N295 : STD_LOGIC; 
  signal mod7_N106 : STD_LOGIC; 
  signal mod7_N24 : STD_LOGIC; 
  signal mod7_N265 : STD_LOGIC; 
  signal mod7_N237 : STD_LOGIC; 
  signal mod7_N232 : STD_LOGIC; 
  signal mod7_N163 : STD_LOGIC; 
  signal mod7_N51 : STD_LOGIC; 
  signal mod7_video_sm_active_line_n0000_4_cyo : STD_LOGIC; 
  signal mod7_n0229 : STD_LOGIC; 
  signal N8027 : STD_LOGIC; 
  signal mod7_N3 : STD_LOGIC; 
  signal mod7_video_sm_active_line_n0000_3_cyo : STD_LOGIC; 
  signal CHOICE15145 : STD_LOGIC; 
  signal CHOICE14607 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_N5 : STD_LOGIC; 
  signal mod7_video_sm_active_line_n0000_2_cyo : STD_LOGIC; 
  signal mod7_N289 : STD_LOGIC; 
  signal mod7_N186 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_tp_22_MUXF51 : STD_LOGIC; 
  signal mod7_video_sm_active_line_n0000_1_cyo : STD_LOGIC; 
  signal N8486 : STD_LOGIC; 
  signal mod7_N262 : STD_LOGIC; 
  signal mod7_N190 : STD_LOGIC; 
  signal mod7_N254 : STD_LOGIC; 
  signal mod7_N185 : STD_LOGIC; 
  signal N8019 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_N6 : STD_LOGIC; 
  signal mod7_video_sm_active_line_n0000_0_cyo : STD_LOGIC; 
  signal mod7_N154 : STD_LOGIC; 
  signal mod7_N291 : STD_LOGIC; 
  signal mod7_n0315_5_Q : STD_LOGIC; 
  signal CHOICE14775 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_N7 : STD_LOGIC; 
  signal mod7_N184 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_N10 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_tp_22_MUXF52 : STD_LOGIC; 
  signal mod7_N255 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_N11 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_N8 : STD_LOGIC; 
  signal mod7_h_state_FFd1_In : STD_LOGIC; 
  signal N7977 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_tp_22_MUXF54 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_N9 : STD_LOGIC; 
  signal mod7_tp_21_Q : STD_LOGIC; 
  signal CHOICE14773 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_N12 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_tp_22_MUXF53 : STD_LOGIC; 
  signal mod7_h_state_FFd3_In : STD_LOGIC; 
  signal mod7_n0315_30_Q : STD_LOGIC; 
  signal mod7_n0315_22_Q : STD_LOGIC; 
  signal mod7_MUX_BLOCK_tp_22_MUXF56 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_N13 : STD_LOGIC; 
  signal mod7_n0315_31_Q : STD_LOGIC; 
  signal mod7_n0315_23_Q : STD_LOGIC; 
  signal mod7_MUX_BLOCK_N16 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_tp_22_MUXF55 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_N17 : STD_LOGIC; 
  signal mod7_n0257 : STD_LOGIC; 
  signal N8259 : STD_LOGIC; 
  signal mod7_n0263 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_N14 : STD_LOGIC; 
  signal mod7_n0315_29_Q : STD_LOGIC; 
  signal mod7_MUX_BLOCK_tp_22_MUXF57 : STD_LOGIC; 
  signal mod7_MUX_BLOCK_N15 : STD_LOGIC; 
  signal mod7_video_sm_active_line_n0000_9_cyo : STD_LOGIC; 
  signal mod7_video_sm_active_line_n0000_8_cyo : STD_LOGIC; 
  signal mod7_MUX_BLOCK_N3 : STD_LOGIC; 
  signal mod7_counter_count_7_n0001 : STD_LOGIC; 
  signal N8400 : STD_LOGIC; 
  signal CHOICE14875 : STD_LOGIC; 
  signal mod5_hdframe_linecount_5_rt1 : STD_LOGIC; 
  signal N5872 : STD_LOGIC; 
  signal mod0_bitptr_5_rt : STD_LOGIC; 
  signal N8233 : STD_LOGIC; 
  signal N8245 : STD_LOGIC; 
  signal CHOICE15193 : STD_LOGIC; 
  signal CHOICE15112 : STD_LOGIC; 
  signal mod5_field1_vblnk1_0_Q : STD_LOGIC; 
  signal mod5_field1_vblnk1_4_Q : STD_LOGIC; 
  signal N8085 : STD_LOGIC; 
  signal CHOICE14872 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_5_rt : STD_LOGIC; 
  signal mod5_field1_vblnk1_1_Q : STD_LOGIC; 
  signal CHOICE14981 : STD_LOGIC; 
  signal mod0_n0264 : STD_LOGIC; 
  signal N8077 : STD_LOGIC; 
  signal CHOICE15055 : STD_LOGIC; 
  signal N8373 : STD_LOGIC; 
  signal N8381 : STD_LOGIC; 
  signal CHOICE14415 : STD_LOGIC; 
  signal CHOICE15402 : STD_LOGIC; 
  signal CHOICE15305 : STD_LOGIC; 
  signal mod5_field2_vblnk1_0_Q : STD_LOGIC; 
  signal mod5_hdframe_samplecount_8_rt3 : STD_LOGIC; 
  signal CHOICE15423 : STD_LOGIC; 
  signal mod5_tsl_2_Q : STD_LOGIC; 
  signal CHOICE15138 : STD_LOGIC; 
  signal CHOICE14855 : STD_LOGIC; 
  signal mod5_tsl_4_Q : STD_LOGIC; 
  signal CHOICE15446 : STD_LOGIC; 
  signal mod5_field2_vblnk1_5_Q : STD_LOGIC; 
  signal N8078 : STD_LOGIC; 
  signal CHOICE14406 : STD_LOGIC; 
  signal N8087 : STD_LOGIC; 
  signal CHOICE15444 : STD_LOGIC; 
  signal N8371 : STD_LOGIC; 
  signal N8089 : STD_LOGIC; 
  signal CHOICE15035 : STD_LOGIC; 
  signal N7973 : STD_LOGIC; 
  signal CHOICE14547 : STD_LOGIC; 
  signal N8079 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_2_rt1 : STD_LOGIC; 
  signal CHOICE14645 : STD_LOGIC; 
  signal N8068 : STD_LOGIC; 
  signal mod7_color_N47 : STD_LOGIC; 
  signal mod7_color_N59 : STD_LOGIC; 
  signal mod7_color_N81 : STD_LOGIC; 
  signal mod7_color_N102 : STD_LOGIC; 
  signal mod7_color_N125 : STD_LOGIC; 
  signal mod7_color_yramp_0_9_N0 : STD_LOGIC; 
  signal mod7_color_N106 : STD_LOGIC; 
  signal mod7_color_N19 : STD_LOGIC; 
  signal CHOICE15447 : STD_LOGIC; 
  signal mod7_color_N97 : STD_LOGIC; 
  signal N7986 : STD_LOGIC; 
  signal mod7_color_N21 : STD_LOGIC; 
  signal mod7_color_v1h2_cb_6_Q : STD_LOGIC; 
  signal mod7_color_N42 : STD_LOGIC; 
  signal mod7_color_N22 : STD_LOGIC; 
  signal mod7_color_N28 : STD_LOGIC; 
  signal mod7_color_N134 : STD_LOGIC; 
  signal mod7_color_N26 : STD_LOGIC; 
  signal N8043 : STD_LOGIC; 
  signal mod7_color_n0011 : STD_LOGIC; 
  signal mod7_color_n0012 : STD_LOGIC; 
  signal mod7_color_N45 : STD_LOGIC; 
  signal mod7_color_N101 : STD_LOGIC; 
  signal mod7_color_n0013 : STD_LOGIC; 
  signal mod7_color_N121 : STD_LOGIC; 
  signal mod7_color_n0014 : STD_LOGIC; 
  signal mod7_color_N137 : STD_LOGIC; 
  signal mod7_color_n0020 : STD_LOGIC; 
  signal mod7_color_n0015 : STD_LOGIC; 
  signal mod7_color_N29 : STD_LOGIC; 
  signal mod7_color_n0016 : STD_LOGIC; 
  signal mod7_color_N140 : STD_LOGIC; 
  signal mod7_color_N100 : STD_LOGIC; 
  signal mod7_color_n0022 : STD_LOGIC; 
  signal mod7_color_N37 : STD_LOGIC; 
  signal mod7_color_n0023 : STD_LOGIC; 
  signal mod7_color_n0018 : STD_LOGIC; 
  signal mod7_color_n0024 : STD_LOGIC; 
  signal mod7_color_n0019 : STD_LOGIC; 
  signal mod7_color_N67 : STD_LOGIC; 
  signal mod7_color_n0025 : STD_LOGIC; 
  signal mod7_color_n0031 : STD_LOGIC; 
  signal mod7_color_N43 : STD_LOGIC; 
  signal mod7_color_color_lut_yramp_n0000_6_cyo : STD_LOGIC; 
  signal mod7_color_N41 : STD_LOGIC; 
  signal mod7_color_N3 : STD_LOGIC; 
  signal mod7_color_N40 : STD_LOGIC; 
  signal mod7_color_color_lut_yramp_n0000_0_cyo : STD_LOGIC; 
  signal mod7_color_N39 : STD_LOGIC; 
  signal mod7_color_N132 : STD_LOGIC; 
  signal mod7_color_N79 : STD_LOGIC; 
  signal mod7_color_N38 : STD_LOGIC; 
  signal mod7_color_n0041 : STD_LOGIC; 
  signal mod7_color_n0036 : STD_LOGIC; 
  signal mod7_color_N01 : STD_LOGIC; 
  signal mod7_color_n0037 : STD_LOGIC; 
  signal mod7_color_N118 : STD_LOGIC; 
  signal mod7_color_N80 : STD_LOGIC; 
  signal mod7_color_N128 : STD_LOGIC; 
  signal mod7_color_n0039 : STD_LOGIC; 
  signal mod7_color_N77 : STD_LOGIC; 
  signal mod7_color_color_lut_yramp_n0000_5_cyo : STD_LOGIC; 
  signal mod7_color_N115 : STD_LOGIC; 
  signal mod7_color_N135 : STD_LOGIC; 
  signal mod7_color_color_lut_yramp_n0000_1_cyo : STD_LOGIC; 
  signal mod7_color_N107 : STD_LOGIC; 
  signal mod7_color_N33 : STD_LOGIC; 
  signal mod7_color_N57 : STD_LOGIC; 
  signal mod7_color_N82 : STD_LOGIC; 
  signal mod7_color_color_lut_yramp_n0000_8_cyo : STD_LOGIC; 
  signal mod7_color_N61 : STD_LOGIC; 
  signal mod7_color_N78 : STD_LOGIC; 
  signal mod7_color_color_lut_yramp_n0000_7_cyo : STD_LOGIC; 
  signal mod7_color_N24 : STD_LOGIC; 
  signal mod7_color_N55 : STD_LOGIC; 
  signal mod7_color_color_lut_yramp_n0000_2_cyo : STD_LOGIC; 
  signal mod7_color_color_lut_yramp_n0000_4_cyo : STD_LOGIC; 
  signal mod7_color_N133 : STD_LOGIC; 
  signal mod7_color_N129 : STD_LOGIC; 
  signal mod7_color_N122 : STD_LOGIC; 
  signal CHOICE15392 : STD_LOGIC; 
  signal mod7_color_N120 : STD_LOGIC; 
  signal mod7_color_N91 : STD_LOGIC; 
  signal mod7_color_N123 : STD_LOGIC; 
  signal mod7_color_N117 : STD_LOGIC; 
  signal mod7_color_N98 : STD_LOGIC; 
  signal mod7_color_v1h2_cb_8_Q : STD_LOGIC; 
  signal mod7_color_v0h2_y_2_Q : STD_LOGIC; 
  signal mod7_color_color_lut_yramp_n0000_3_cyo : STD_LOGIC; 
  signal mod7_color_N126 : STD_LOGIC; 
  signal mod7_color_N58 : STD_LOGIC; 
  signal mod7_color_v0h2_y_0_Q : STD_LOGIC; 
  signal mod7_color_N51 : STD_LOGIC; 
  signal mod7_color_N96 : STD_LOGIC; 
  signal mod7_color_n0081 : STD_LOGIC; 
  signal mod5_hdframe_N140 : STD_LOGIC; 
  signal mod5_hdframe_N194 : STD_LOGIC; 
  signal CHOICE15322 : STD_LOGIC; 
  signal mod0_serial_interface_bitptr_n0000_2_cyo : STD_LOGIC; 
  signal mod5_hdframe_N521 : STD_LOGIC; 
  signal mod0_n0260 : STD_LOGIC; 
  signal mod5_hdframe_n0086_7_Q : STD_LOGIC; 
  signal mod5_hdframe_N54 : STD_LOGIC; 
  signal mod5_hdframe_n0086_6_Q : STD_LOGIC; 
  signal mod5_hdframe_n0086_5_Q : STD_LOGIC; 
  signal mod5_hdframe_n0086_3_Q : STD_LOGIC; 
  signal mod5_hdframe_n0087_5_Q : STD_LOGIC; 
  signal CHOICE14988 : STD_LOGIC; 
  signal mod5_hdframe_N901 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_6_rt : STD_LOGIC; 
  signal mod5_hdframe_N301 : STD_LOGIC; 
  signal mod0_N5 : STD_LOGIC; 
  signal mod5_hdframe_N871 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo18 : STD_LOGIC; 
  signal mod5_hdframe_h : STD_LOGIC; 
  signal mod5_hdframe_N125 : STD_LOGIC; 
  signal mod5_hdframe_v : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo17 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_0_4_n0000 : STD_LOGIC; 
  signal mod5_hdframe_f : STD_LOGIC; 
  signal CHOICE14417 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_0_3_n0000 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_0_7_n0001 : STD_LOGIC; 
  signal CHOICE14426 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo20 : STD_LOGIC; 
  signal CHOICE14555 : STD_LOGIC; 
  signal mod0_n0048 : STD_LOGIC; 
  signal CHOICE14686 : STD_LOGIC; 
  signal mod5_hdframe_N139 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo51 : STD_LOGIC; 
  signal mod5_hdframe_N221 : STD_LOGIC; 
  signal mod5_hdframe_N215 : STD_LOGIC; 
  signal CHOICE14898 : STD_LOGIC; 
  signal mod5_hdframe_n0087_11_Q : STD_LOGIC; 
  signal mod5_hdframe_n0083_2_Q : STD_LOGIC; 
  signal mod5_hdframe_n0087_10_Q : STD_LOGIC; 
  signal mod5_hdframe_n0083_3_Q : STD_LOGIC; 
  signal N8062 : STD_LOGIC; 
  signal mod5_hdframe_n0083_5_Q : STD_LOGIC; 
  signal mod5_hdframe_n0087_7_Q : STD_LOGIC; 
  signal mod5_hdframe_n0083_6_Q : STD_LOGIC; 
  signal mod5_hdframe_n0083_7_Q : STD_LOGIC; 
  signal mod0_N11 : STD_LOGIC; 
  signal mod0_N26 : STD_LOGIC; 
  signal mod5_hdframe_N21 : STD_LOGIC; 
  signal mod5_hdframe_n0086_10_Q : STD_LOGIC; 
  signal mod5_hdframe_n0087_9_Q : STD_LOGIC; 
  signal N6699 : STD_LOGIC; 
  signal CHOICE15213 : STD_LOGIC; 
  signal CHOICE15212 : STD_LOGIC; 
  signal N7980 : STD_LOGIC; 
  signal mod7_h_state_FFd1_2 : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0088_3_cyo : STD_LOGIC; 
  signal mod5_hdframe_N46 : STD_LOGIC; 
  signal CHOICE15007 : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0088_2_cyo : STD_LOGIC; 
  signal mod0_n0286 : STD_LOGIC; 
  signal CHOICE14717 : STD_LOGIC; 
  signal mod0_new_delay_N0 : STD_LOGIC; 
  signal mod5_hdframe_n0088_3_Q : STD_LOGIC; 
  signal mod5_hdframe_n0100 : STD_LOGIC; 
  signal CHOICE14593 : STD_LOGIC; 
  signal mod5_hdframe_n0101 : STD_LOGIC; 
  signal mod0_n0266 : STD_LOGIC; 
  signal mod5_hdframe_n0088_5_Q : STD_LOGIC; 
  signal N8452 : STD_LOGIC; 
  signal mod5_hdframe_n0088_6_Q : STD_LOGIC; 
  signal mod5_hdframe_n0103 : STD_LOGIC; 
  signal mod5_hdframe_n0088_2_Q : STD_LOGIC; 
  signal mod5_hdframe_n0088_7_Q : STD_LOGIC; 
  signal mod5_hdframe_n0104 : STD_LOGIC; 
  signal CHOICE14601 : STD_LOGIC; 
  signal mod5_hdframe_n0087_1_Q : STD_LOGIC; 
  signal mod5_hdframe_n0105 : STD_LOGIC; 
  signal mod5_hdframe_N206 : STD_LOGIC; 
  signal mod5_hdframe_n0088_9_Q : STD_LOGIC; 
  signal mod5_hdframe_n0087_2_Q : STD_LOGIC; 
  signal mod5_hdframe_n0106 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo19 : STD_LOGIC; 
  signal mod5_hdframe_n0088_10_Q : STD_LOGIC; 
  signal mod5_hdframe_n0107 : STD_LOGIC; 
  signal mod5_hdframe_n0112 : STD_LOGIC; 
  signal mod5_hdframe_n0088_11_Q : STD_LOGIC; 
  signal mod5_hdframe_N196 : STD_LOGIC; 
  signal mod5_hdframe_N881 : STD_LOGIC; 
  signal mod5_hdframe_N202 : STD_LOGIC; 
  signal mod5_hdframe_N551 : STD_LOGIC; 
  signal mod5_hdframe_n0114 : STD_LOGIC; 
  signal mod5_hdframe_n0065 : STD_LOGIC; 
  signal mod5_hdframe_N214 : STD_LOGIC; 
  signal mod5_hdframe_n0115 : STD_LOGIC; 
  signal N8502 : STD_LOGIC; 
  signal mod5_hdframe_n0116 : STD_LOGIC; 
  signal mod5_hdframe_n0121 : STD_LOGIC; 
  signal mod5_hdframe_n0067 : STD_LOGIC; 
  signal N8048 : STD_LOGIC; 
  signal mod5_hdframe_And_cyo5 : STD_LOGIC; 
  signal N8038 : STD_LOGIC; 
  signal N8498 : STD_LOGIC; 
  signal mod5_hdframe_n0123 : STD_LOGIC; 
  signal CHOICE15316 : STD_LOGIC; 
  signal mod5_hdframe_n0119 : STD_LOGIC; 
  signal mod5_hdframe_n0124 : STD_LOGIC; 
  signal mod5_hdframe_N213 : STD_LOGIC; 
  signal mod5_hdframe_n0125 : STD_LOGIC; 
  signal mod5_hdframe_N205 : STD_LOGIC; 
  signal mod5_hdframe_n0126 : STD_LOGIC; 
  signal mod5_hdframe_n0087_3_Q : STD_LOGIC; 
  signal N8064 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo17 : STD_LOGIC; 
  signal mod5_hdframe_n0132 : STD_LOGIC; 
  signal mod5_hdframe_n0078 : STD_LOGIC; 
  signal mod5_hdframe_n0133 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo22 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo31 : STD_LOGIC; 
  signal mod5_hdframe_N197 : STD_LOGIC; 
  signal mod5_hdframe_n0135 : STD_LOGIC; 
  signal CHOICE14516 : STD_LOGIC; 
  signal mod5_hdframe_N204 : STD_LOGIC; 
  signal mod5_hdframe_n0094 : STD_LOGIC; 
  signal CHOICE14678 : STD_LOGIC; 
  signal mod5_hdframe_n0083_11_Q : STD_LOGIC; 
  signal mod5_hdframe_n0095 : STD_LOGIC; 
  signal mod5_hdframe_n0083_9_Q : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo23 : STD_LOGIC; 
  signal mod5_hdframe_n0096 : STD_LOGIC; 
  signal CHOICE15148 : STD_LOGIC; 
  signal mod5_hdframe_n0097 : STD_LOGIC; 
  signal mod5_hdframe_N531 : STD_LOGIC; 
  signal N8041 : STD_LOGIC; 
  signal mod5_hdframe_N22 : STD_LOGIC; 
  signal mod5_hdframe_n0099 : STD_LOGIC; 
  signal CHOICE14631 : STD_LOGIC; 
  signal mod5_hdframe_n0086_11_Q : STD_LOGIC; 
  signal N8072 : STD_LOGIC; 
  signal mod5_hdframe_n0087_6_Q : STD_LOGIC; 
  signal CHOICE15215 : STD_LOGIC; 
  signal mod5_hdframe_n0156 : STD_LOGIC; 
  signal N8070 : STD_LOGIC; 
  signal mod5_hdframe_N561 : STD_LOGIC; 
  signal CHOICE15224 : STD_LOGIC; 
  signal mod5_hdframe_n0086_9_Q : STD_LOGIC; 
  signal CHOICE14506 : STD_LOGIC; 
  signal mod0_N30 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo25 : STD_LOGIC; 
  signal mod5_hdframe_N218 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo24 : STD_LOGIC; 
  signal mod5_hdframe_N223 : STD_LOGIC; 
  signal mod5_hdframe_N18 : STD_LOGIC; 
  signal mod5_hdframe_N212 : STD_LOGIC; 
  signal mod5_hdframe_n0083_10_Q : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo33 : STD_LOGIC; 
  signal CHOICE15155 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo53 : STD_LOGIC; 
  signal mod5_hdframe_N217 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_0_7_n0002 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo9 : STD_LOGIC; 
  signal mod5_hdframe_N219 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo52 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo50 : STD_LOGIC; 
  signal mod5_hdframe_N138 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo19 : STD_LOGIC; 
  signal mod5_hdframe_N137 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo18 : STD_LOGIC; 
  signal mod5_hdframe_N136 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo17 : STD_LOGIC; 
  signal mod5_hdframe_N135 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo16 : STD_LOGIC; 
  signal mod5_hdframe_N134 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo15 : STD_LOGIC; 
  signal mod5_hdframe_N133 : STD_LOGIC; 
  signal mod5_hdframe_N132 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo14 : STD_LOGIC; 
  signal mod5_hdframe_N131 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo13 : STD_LOGIC; 
  signal mod5_hdframe_N130 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo12 : STD_LOGIC; 
  signal mod5_hdframe_N129 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo11 : STD_LOGIC; 
  signal mod5_hdframe_N128 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo10 : STD_LOGIC; 
  signal mod5_hdframe_N127 : STD_LOGIC; 
  signal mod5_hdframe_N126 : STD_LOGIC; 
  signal mod5_hdframe_N193 : STD_LOGIC; 
  signal mod5_hdframe_N192 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo16 : STD_LOGIC; 
  signal mod5_hdframe_N191 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo15 : STD_LOGIC; 
  signal mod5_hdframe_N190 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo14 : STD_LOGIC; 
  signal mod5_hdframe_N189 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo13 : STD_LOGIC; 
  signal mod5_hdframe_N188 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo12 : STD_LOGIC; 
  signal mod5_hdframe_N187 : STD_LOGIC; 
  signal mod5_hdframe_N186 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo11 : STD_LOGIC; 
  signal mod5_hdframe_N185 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo10 : STD_LOGIC; 
  signal mod5_hdframe_N184 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo9 : STD_LOGIC; 
  signal mod5_hdframe_N183 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo8 : STD_LOGIC; 
  signal mod5_hdframe_N182 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo7 : STD_LOGIC; 
  signal mod5_hdframe_N181 : STD_LOGIC; 
  signal mod5_hdframe_N180 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo6 : STD_LOGIC; 
  signal mod5_hdframe_N179 : STD_LOGIC; 
  signal mod5_hdframe_And_cyo3 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo15 : STD_LOGIC; 
  signal mod5_hdframe_N178 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo14 : STD_LOGIC; 
  signal mod5_hdframe_N177 : STD_LOGIC; 
  signal mod5_hdframe_N176 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo5 : STD_LOGIC; 
  signal mod5_hdframe_N175 : STD_LOGIC; 
  signal mod5_hdframe_And_cyo2 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo13 : STD_LOGIC; 
  signal mod5_hdframe_N174 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo12 : STD_LOGIC; 
  signal mod5_hdframe_N173 : STD_LOGIC; 
  signal mod5_hdframe_N172 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo49 : STD_LOGIC; 
  signal N8067 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo48 : STD_LOGIC; 
  signal mod5_hdframe_N170 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo47 : STD_LOGIC; 
  signal CHOICE15158 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo46 : STD_LOGIC; 
  signal mod5_hdframe_N168 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo11 : STD_LOGIC; 
  signal mod5_hdframe_N167 : STD_LOGIC; 
  signal mod5_hdframe_And_cyo1 : STD_LOGIC; 
  signal mod5_hdframe_N166 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo10 : STD_LOGIC; 
  signal mod5_hdframe_N165 : STD_LOGIC; 
  signal mod5_hdframe_N164 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo30 : STD_LOGIC; 
  signal mod5_hdframe_N163 : STD_LOGIC; 
  signal mod5_hdframe_And_cyo : STD_LOGIC; 
  signal mod5_hdframe_N162 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo9 : STD_LOGIC; 
  signal mod5_hdframe_N161 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo8 : STD_LOGIC; 
  signal mod5_hdframe_N160 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo7 : STD_LOGIC; 
  signal mod5_hdframe_N158 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo45 : STD_LOGIC; 
  signal CHOICE15245 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo44 : STD_LOGIC; 
  signal mod5_hdframe_N156 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo43 : STD_LOGIC; 
  signal CHOICE15431 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo42 : STD_LOGIC; 
  signal mod5_hdframe_N154 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo6 : STD_LOGIC; 
  signal mod5_hdframe_N153 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo5 : STD_LOGIC; 
  signal mod5_hdframe_N152 : STD_LOGIC; 
  signal mod5_hdframe_N151 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo29 : STD_LOGIC; 
  signal mod5_hdframe_N150 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo28 : STD_LOGIC; 
  signal mod5_hdframe_N149 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo27 : STD_LOGIC; 
  signal mod5_hdframe_N148 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo26 : STD_LOGIC; 
  signal mod5_hdframe_N147 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo25 : STD_LOGIC; 
  signal mod5_hdframe_N146 : STD_LOGIC; 
  signal mod5_hdframe_N201 : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_samplecount_n0000_10_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_samplecount_n0000_9_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_samplecount_n0000_8_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_samplecount_n0000_7_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_samplecount_n0000_6_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_samplecount_n0000_5_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_samplecount_n0000_4_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_samplecount_n0000_3_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_samplecount_n0000_2_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_samplecount_n0000_1_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_samplecount_n0000_0_cyo : STD_LOGIC; 
  signal mod5_hdframe_N145 : STD_LOGIC; 
  signal mod5_hdframe_N144 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo24 : STD_LOGIC; 
  signal mod5_hdframe_N143 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo23 : STD_LOGIC; 
  signal mod5_hdframe_N142 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo22 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_0_8_n0000 : STD_LOGIC; 
  signal mod5_hdframe_N141 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo21 : STD_LOGIC; 
  signal mod7_counter_count_1_n0001 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo18 : STD_LOGIC; 
  signal CHOICE15097 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo21 : STD_LOGIC; 
  signal N7984 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_0_0_n0000 : STD_LOGIC; 
  signal mod5_hdframe_N209 : STD_LOGIC; 
  signal mod5_hdframe_N210 : STD_LOGIC; 
  signal mod5_hdframe_N211 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo19 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo21 : STD_LOGIC; 
  signal mod5_hdframe_N203 : STD_LOGIC; 
  signal mod5_hdframe_N198 : STD_LOGIC; 
  signal mod5_hdframe_N199 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo16 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo20 : STD_LOGIC; 
  signal mod5_hdframe_N511 : STD_LOGIC; 
  signal mod5_hdframe_N541 : STD_LOGIC; 
  signal mod5_hdframe_N571 : STD_LOGIC; 
  signal mod5_hdframe_N891 : STD_LOGIC; 
  signal mod5_hdframe_N200 : STD_LOGIC; 
  signal mod5_hdframe_N45 : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0088_1_cyo : STD_LOGIC; 
  signal mod5_hdframe_N44 : STD_LOGIC; 
  signal mod5_hdframe_N195 : STD_LOGIC; 
  signal CHOICE15398 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_0_9_n0000 : STD_LOGIC; 
  signal mod5_hdframe_And_cyo7 : STD_LOGIC; 
  signal mod5_hdframe_N216 : STD_LOGIC; 
  signal N8507 : STD_LOGIC; 
  signal CHOICE14783 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo32 : STD_LOGIC; 
  signal mod5_hdframe_N208 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo20 : STD_LOGIC; 
  signal CHOICE14512 : STD_LOGIC; 
  signal mod5_hdframe_N207 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo34 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_0_6_n0000 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_0_5_n0000 : STD_LOGIC; 
  signal N8500 : STD_LOGIC; 
  signal CHOICE15185 : STD_LOGIC; 
  signal mod5_hdframe_And_cyo6 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_0_7_n0000 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_0_2_n0000 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_0_1_n0000 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_0_10_n0000 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_0_1 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_0_11_n0000 : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0083_2_cyo : STD_LOGIC; 
  signal mod5_hdframe_N15 : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0083_3_cyo : STD_LOGIC; 
  signal mod0_n0047 : STD_LOGIC; 
  signal mod5_hdframe_N17 : STD_LOGIC; 
  signal mod0_N32 : STD_LOGIC; 
  signal CHOICE15313 : STD_LOGIC; 
  signal CHOICE14424 : STD_LOGIC; 
  signal mod5_hdframe_N24 : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0086_2_cyo : STD_LOGIC; 
  signal mod5_hdframe_N25 : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0086_3_cyo : STD_LOGIC; 
  signal mod0_n0046 : STD_LOGIC; 
  signal CHOICE14399 : STD_LOGIC; 
  signal mod0_n0269 : STD_LOGIC; 
  signal CHOICE15264 : STD_LOGIC; 
  signal mod0_n0275 : STD_LOGIC; 
  signal CHOICE14643 : STD_LOGIC; 
  signal mod0_n0262 : STD_LOGIC; 
  signal CHOICE14396 : STD_LOGIC; 
  signal CHOICE14852 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_10_rt2 : STD_LOGIC; 
  signal mod0_serial_interface_bitptr_n0000_1_cyo : STD_LOGIC; 
  signal CHOICE14394 : STD_LOGIC; 
  signal mod0_serial_interface_bitptr_n0000_0_cyo : STD_LOGIC; 
  signal N8013 : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0087_1_cyo : STD_LOGIC; 
  signal mod5_hdframe_N34 : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0087_2_cyo : STD_LOGIC; 
  signal mod5_hdframe_N35 : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0087_3_cyo : STD_LOGIC; 
  signal mod0_N3 : STD_LOGIC; 
  signal mod0_serial_interface_bitptr_n0000_4_cyo : STD_LOGIC; 
  signal CHOICE15308 : STD_LOGIC; 
  signal mod5_hdframe_N55 : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0092_0_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0092_1_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0092_2_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0092_3_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0092_4_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0092_5_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0092_6_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0092_7_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0092_8_cyo : STD_LOGIC; 
  signal mod5_hdframe_hd_framegenerator_n0092_9_cyo : STD_LOGIC; 
  signal mod5_hdframe_And_cyo4 : STD_LOGIC; 
  signal mod5_hdframe_N56 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo : STD_LOGIC; 
  signal mod5_hdframe_N57 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo1 : STD_LOGIC; 
  signal mod5_hdframe_N58 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo : STD_LOGIC; 
  signal mod5_hdframe_N59 : STD_LOGIC; 
  signal mod5_hdframe_N60 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo : STD_LOGIC; 
  signal mod5_hdframe_N61 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo1 : STD_LOGIC; 
  signal mod5_hdframe_N62 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo2 : STD_LOGIC; 
  signal mod5_hdframe_N63 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo3 : STD_LOGIC; 
  signal mod5_hdframe_N64 : STD_LOGIC; 
  signal mod5_hdframe_Neq_stage_cyo4 : STD_LOGIC; 
  signal mod5_hdframe_N65 : STD_LOGIC; 
  signal mod5_hdframe_N66 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo1 : STD_LOGIC; 
  signal mod5_hdframe_N67 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo2 : STD_LOGIC; 
  signal mod5_hdframe_N68 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo3 : STD_LOGIC; 
  signal mod5_hdframe_N69 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo4 : STD_LOGIC; 
  signal mod5_hdframe_N70 : STD_LOGIC; 
  signal mod5_hdframe_N71 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo2 : STD_LOGIC; 
  signal mod5_hdframe_N72 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo3 : STD_LOGIC; 
  signal mod5_hdframe_N73 : STD_LOGIC; 
  signal mod5_hdframe_nor_cyo4 : STD_LOGIC; 
  signal mod5_hdframe_N75 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo : STD_LOGIC; 
  signal mod5_hdframe_N76 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo1 : STD_LOGIC; 
  signal mod5_hdframe_N77 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo2 : STD_LOGIC; 
  signal mod5_hdframe_N78 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo3 : STD_LOGIC; 
  signal mod5_hdframe_N79 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo4 : STD_LOGIC; 
  signal mod5_hdframe_N80 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo5 : STD_LOGIC; 
  signal mod5_hdframe_N81 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo6 : STD_LOGIC; 
  signal mod5_hdframe_N82 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo7 : STD_LOGIC; 
  signal mod5_hdframe_N83 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo8 : STD_LOGIC; 
  signal mod5_hdframe_N84 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo9 : STD_LOGIC; 
  signal mod5_hdframe_N85 : STD_LOGIC; 
  signal mod5_hdframe_N86 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo10 : STD_LOGIC; 
  signal mod5_hdframe_N87 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo11 : STD_LOGIC; 
  signal mod5_hdframe_N88 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo12 : STD_LOGIC; 
  signal mod5_hdframe_N89 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo13 : STD_LOGIC; 
  signal mod5_hdframe_N90 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo14 : STD_LOGIC; 
  signal mod5_hdframe_N91 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo15 : STD_LOGIC; 
  signal mod5_hdframe_N92 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo16 : STD_LOGIC; 
  signal mod5_hdframe_N93 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo17 : STD_LOGIC; 
  signal mod5_hdframe_N94 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo18 : STD_LOGIC; 
  signal mod5_hdframe_N95 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo19 : STD_LOGIC; 
  signal mod5_hdframe_N96 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo20 : STD_LOGIC; 
  signal mod5_hdframe_N98 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo21 : STD_LOGIC; 
  signal mod5_hdframe_N99 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo22 : STD_LOGIC; 
  signal mod5_hdframe_N100 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo23 : STD_LOGIC; 
  signal mod5_hdframe_N101 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo24 : STD_LOGIC; 
  signal mod5_hdframe_N102 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo25 : STD_LOGIC; 
  signal mod5_hdframe_N103 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo26 : STD_LOGIC; 
  signal mod5_hdframe_N104 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo27 : STD_LOGIC; 
  signal mod5_hdframe_N105 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo28 : STD_LOGIC; 
  signal mod5_hdframe_N106 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo29 : STD_LOGIC; 
  signal mod5_hdframe_N107 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo30 : STD_LOGIC; 
  signal mod5_hdframe_N108 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo31 : STD_LOGIC; 
  signal mod5_hdframe_N110 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo32 : STD_LOGIC; 
  signal mod5_hdframe_N111 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo33 : STD_LOGIC; 
  signal mod5_hdframe_N112 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo34 : STD_LOGIC; 
  signal mod5_hdframe_N113 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo35 : STD_LOGIC; 
  signal mod5_hdframe_N114 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo36 : STD_LOGIC; 
  signal CHOICE14827 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo37 : STD_LOGIC; 
  signal CHOICE14591 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo38 : STD_LOGIC; 
  signal mod5_hdframe_N117 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo39 : STD_LOGIC; 
  signal mod5_hdframe_N118 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo40 : STD_LOGIC; 
  signal mod5_hdframe_N119 : STD_LOGIC; 
  signal mod5_hdframe_XNor_stage_cyo41 : STD_LOGIC; 
  signal mod5_hdframe_N120 : STD_LOGIC; 
  signal mod5_hdframe_N121 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo5 : STD_LOGIC; 
  signal mod5_hdframe_N122 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo6 : STD_LOGIC; 
  signal mod5_hdframe_N123 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo7 : STD_LOGIC; 
  signal mod5_hdframe_N124 : STD_LOGIC; 
  signal mod5_hdframe_Eq_stage_cyo8 : STD_LOGIC; 
  signal mod7_counter_state_counter_n0009_12_cyo : STD_LOGIC; 
  signal mod7_counter_N3 : STD_LOGIC; 
  signal mod7_counter_state_counter_n0009_11_cyo : STD_LOGIC; 
  signal mod7_counter_N12 : STD_LOGIC; 
  signal mod7_counter_count_6_n0001 : STD_LOGIC; 
  signal mod7_counter_count_8_n0001 : STD_LOGIC; 
  signal mod7_counter_count_5_n0000 : STD_LOGIC; 
  signal mod7_counter_count_7_n0000 : STD_LOGIC; 
  signal mod7_counter_count_3_n0001 : STD_LOGIC; 
  signal mod7_counter_count_2_n0000 : STD_LOGIC; 
  signal mod7_counter_count_8_n0000 : STD_LOGIC; 
  signal mod7_counter_state_counter_n0009_18_cyo : STD_LOGIC; 
  signal mod7_counter_N9 : STD_LOGIC; 
  signal mod7_counter_state_counter_n0009_17_cyo : STD_LOGIC; 
  signal mod7_counter_load : STD_LOGIC; 
  signal CHOICE14842 : STD_LOGIC; 
  signal mod7_counter_count_3_n0000 : STD_LOGIC; 
  signal mod7_counter_N11 : STD_LOGIC; 
  signal mod7_counter_N8 : STD_LOGIC; 
  signal mod7_counter_state_counter_n0009_16_cyo : STD_LOGIC; 
  signal mod7_counter_N7 : STD_LOGIC; 
  signal N113 : STD_LOGIC; 
  signal N8277 : STD_LOGIC; 
  signal N117 : STD_LOGIC; 
  signal N119 : STD_LOGIC; 
  signal mod5_hdframe_linecount_8_rt : STD_LOGIC; 
  signal mod5_hdframe_linecount_9_rt : STD_LOGIC; 
  signal N124 : STD_LOGIC; 
  signal N125 : STD_LOGIC; 
  signal N8352 : STD_LOGIC; 
  signal CHOICE12700 : STD_LOGIC; 
  signal CHOICE14587 : STD_LOGIC; 
  signal N8091 : STD_LOGIC; 
  signal CHOICE12563 : STD_LOGIC; 
  signal N1271 : STD_LOGIC; 
  signal N1258 : STD_LOGIC; 
  signal CHOICE14472 : STD_LOGIC; 
  signal N1254 : STD_LOGIC; 
  signal N8474 : STD_LOGIC; 
  signal CHOICE12361 : STD_LOGIC; 
  signal N1234 : STD_LOGIC; 
  signal N8209 : STD_LOGIC; 
  signal CHOICE14718 : STD_LOGIC; 
  signal N1229 : STD_LOGIC; 
  signal N8241 : STD_LOGIC; 
  signal N1207 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_7_rt : STD_LOGIC; 
  signal CHOICE12459 : STD_LOGIC; 
  signal CHOICE14843 : STD_LOGIC; 
  signal N8135 : STD_LOGIC; 
  signal mod7_color_yramp_2_rt : STD_LOGIC; 
  signal N8354 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_0_rt : STD_LOGIC; 
  signal N8061 : STD_LOGIC; 
  signal CHOICE14387 : STD_LOGIC; 
  signal N8141 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_1_rt1 : STD_LOGIC; 
  signal N8139 : STD_LOGIC; 
  signal CHOICE12674 : STD_LOGIC; 
  signal N8343 : STD_LOGIC; 
  signal mod7_counter_count_0_rt : STD_LOGIC; 
  signal N8137 : STD_LOGIC; 
  signal mod7_active_line_10_rt : STD_LOGIC; 
  signal CHOICE14358 : STD_LOGIC; 
  signal CHOICE12692 : STD_LOGIC; 
  signal N8456 : STD_LOGIC; 
  signal CHOICE14966 : STD_LOGIC; 
  signal N8235 : STD_LOGIC; 
  signal N8454 : STD_LOGIC; 
  signal mod7_active_line_8_rt : STD_LOGIC; 
  signal CHOICE12759 : STD_LOGIC; 
  signal mod7_active_line_6_rt : STD_LOGIC; 
  signal CHOICE12728 : STD_LOGIC; 
  signal CHOICE12520 : STD_LOGIC; 
  signal CHOICE14434 : STD_LOGIC; 
  signal CHOICE12384 : STD_LOGIC; 
  signal CHOICE12553 : STD_LOGIC; 
  signal CHOICE12733 : STD_LOGIC; 
  signal CHOICE15259 : STD_LOGIC; 
  signal CHOICE14675 : STD_LOGIC; 
  signal CHOICE12623 : STD_LOGIC; 
  signal CHOICE12425 : STD_LOGIC; 
  signal CHOICE12414 : STD_LOGIC; 
  signal CHOICE12436 : STD_LOGIC; 
  signal CHOICE12504 : STD_LOGIC; 
  signal CHOICE14477 : STD_LOGIC; 
  signal CHOICE14813 : STD_LOGIC; 
  signal N7378 : STD_LOGIC; 
  signal CHOICE12519 : STD_LOGIC; 
  signal mod7_color_yramp_1_rt : STD_LOGIC; 
  signal CHOICE15328 : STD_LOGIC; 
  signal N8045 : STD_LOGIC; 
  signal CHOICE12552 : STD_LOGIC; 
  signal CHOICE12727 : STD_LOGIC; 
  signal CHOICE14749 : STD_LOGIC; 
  signal N8440 : STD_LOGIC; 
  signal mod7_color_yramp_4_rt : STD_LOGIC; 
  signal CHOICE14984 : STD_LOGIC; 
  signal N8197 : STD_LOGIC; 
  signal CHOICE14942 : STD_LOGIC; 
  signal N8466 : STD_LOGIC; 
  signal mod7_color_yramp_5_rt : STD_LOGIC; 
  signal N8205 : STD_LOGIC; 
  signal N8269 : STD_LOGIC; 
  signal N8076 : STD_LOGIC; 
  signal CHOICE12534 : STD_LOGIC; 
  signal N8195 : STD_LOGIC; 
  signal CHOICE12464 : STD_LOGIC; 
  signal CHOICE14994 : STD_LOGIC; 
  signal N8281 : STD_LOGIC; 
  signal mod7_active_line_7_rt : STD_LOGIC; 
  signal CHOICE12645 : STD_LOGIC; 
  signal CHOICE12576 : STD_LOGIC; 
  signal CHOICE12614 : STD_LOGIC; 
  signal CHOICE15302 : STD_LOGIC; 
  signal CHOICE12599 : STD_LOGIC; 
  signal CHOICE12757 : STD_LOGIC; 
  signal CHOICE12452 : STD_LOGIC; 
  signal N8363 : STD_LOGIC; 
  signal CHOICE12483 : STD_LOGIC; 
  signal CHOICE12509 : STD_LOGIC; 
  signal CHOICE15239 : STD_LOGIC; 
  signal N8193 : STD_LOGIC; 
  signal N8325 : STD_LOGIC; 
  signal CHOICE12717 : STD_LOGIC; 
  signal CHOICE12431 : STD_LOGIC; 
  signal CHOICE12472 : STD_LOGIC; 
  signal CHOICE12490 : STD_LOGIC; 
  signal N8203 : STD_LOGIC; 
  signal CHOICE12624 : STD_LOGIC; 
  signal N8231 : STD_LOGIC; 
  signal N8133 : STD_LOGIC; 
  signal CHOICE12475 : STD_LOGIC; 
  signal mod7_color_yramp_7_rt : STD_LOGIC; 
  signal CHOICE12608 : STD_LOGIC; 
  signal CHOICE12713 : STD_LOGIC; 
  signal CHOICE12743 : STD_LOGIC; 
  signal CHOICE12403 : STD_LOGIC; 
  signal N8015 : STD_LOGIC; 
  signal CHOICE14612 : STD_LOGIC; 
  signal CHOICE12429 : STD_LOGIC; 
  signal CHOICE12551 : STD_LOGIC; 
  signal CHOICE12601 : STD_LOGIC; 
  signal mod7_color_yramp_6_rt : STD_LOGIC; 
  signal mod7_color_yramp_3_rt : STD_LOGIC; 
  signal N8339 : STD_LOGIC; 
  signal CHOICE12742 : STD_LOGIC; 
  signal mod5_hdframe_linecount_10_rt : STD_LOGIC; 
  signal CHOICE15442 : STD_LOGIC; 
  signal mod7_color_yramp_8_rt : STD_LOGIC; 
  signal CHOICE14364 : STD_LOGIC; 
  signal CHOICE12564 : STD_LOGIC; 
  signal CHOICE14819 : STD_LOGIC; 
  signal CHOICE12670 : STD_LOGIC; 
  signal CHOICE12415 : STD_LOGIC; 
  signal CHOICE12710 : STD_LOGIC; 
  signal N8207 : STD_LOGIC; 
  signal CHOICE15222 : STD_LOGIC; 
  signal CHOICE12409 : STD_LOGIC; 
  signal CHOICE12668 : STD_LOGIC; 
  signal N8478 : STD_LOGIC; 
  signal CHOICE15045 : STD_LOGIC; 
  signal CHOICE12496 : STD_LOGIC; 
  signal CHOICE12669 : STD_LOGIC; 
  signal CHOICE12711 : STD_LOGIC; 
  signal N8482 : STD_LOGIC; 
  signal CHOICE13792 : STD_LOGIC; 
  signal CHOICE14883 : STD_LOGIC; 
  signal mod0_bitptr_1_rt : STD_LOGIC; 
  signal CHOICE13241 : STD_LOGIC; 
  signal CHOICE13521 : STD_LOGIC; 
  signal N8293 : STD_LOGIC; 
  signal CHOICE12955 : STD_LOGIC; 
  signal N8476 : STD_LOGIC; 
  signal CHOICE12773 : STD_LOGIC; 
  signal CHOICE14276 : STD_LOGIC; 
  signal N7982 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_2_rt : STD_LOGIC; 
  signal N8143 : STD_LOGIC; 
  signal CHOICE13963 : STD_LOGIC; 
  signal N8379 : STD_LOGIC; 
  signal N8125 : STD_LOGIC; 
  signal N8470 : STD_LOGIC; 
  signal CHOICE14142 : STD_LOGIC; 
  signal N8279 : STD_LOGIC; 
  signal N7974 : STD_LOGIC; 
  signal CHOICE14107 : STD_LOGIC; 
  signal CHOICE14861 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_10_rt3 : STD_LOGIC; 
  signal CHOICE13783 : STD_LOGIC; 
  signal CHOICE13140 : STD_LOGIC; 
  signal CHOICE14273 : STD_LOGIC; 
  signal CHOICE13167 : STD_LOGIC; 
  signal N8450 : STD_LOGIC; 
  signal CHOICE13555 : STD_LOGIC; 
  signal CHOICE14119 : STD_LOGIC; 
  signal N8428 : STD_LOGIC; 
  signal mod5_hdframe_linecount_6_rt1 : STD_LOGIC; 
  signal CHOICE14995 : STD_LOGIC; 
  signal CHOICE14281 : STD_LOGIC; 
  signal N7972 : STD_LOGIC; 
  signal CHOICE14286 : STD_LOGIC; 
  signal N8383 : STD_LOGIC; 
  signal CHOICE12779 : STD_LOGIC; 
  signal N8423 : STD_LOGIC; 
  signal CHOICE14005 : STD_LOGIC; 
  signal CHOICE14270 : STD_LOGIC; 
  signal CHOICE14152 : STD_LOGIC; 
  signal CHOICE13913 : STD_LOGIC; 
  signal N8329 : STD_LOGIC; 
  signal CHOICE14284 : STD_LOGIC; 
  signal mod5_hdframe_linecount_1_rt : STD_LOGIC; 
  signal CHOICE13541 : STD_LOGIC; 
  signal CHOICE12942 : STD_LOGIC; 
  signal N8263 : STD_LOGIC; 
  signal CHOICE12783 : STD_LOGIC; 
  signal CHOICE13251 : STD_LOGIC; 
  signal CHOICE12958 : STD_LOGIC; 
  signal CHOICE13861 : STD_LOGIC; 
  signal N8333 : STD_LOGIC; 
  signal CHOICE13527 : STD_LOGIC; 
  signal CHOICE13558 : STD_LOGIC; 
  signal CHOICE15109 : STD_LOGIC; 
  signal CHOICE13537 : STD_LOGIC; 
  signal CHOICE14267 : STD_LOGIC; 
  signal CHOICE13796 : STD_LOGIC; 
  signal N8484 : STD_LOGIC; 
  signal CHOICE14743 : STD_LOGIC; 
  signal N8425 : STD_LOGIC; 
  signal CHOICE14018 : STD_LOGIC; 
  signal N8123 : STD_LOGIC; 
  signal mod5_hdframe_linecount_7_rt : STD_LOGIC; 
  signal N7983 : STD_LOGIC; 
  signal CHOICE12987 : STD_LOGIC; 
  signal CHOICE13094 : STD_LOGIC; 
  signal CHOICE13367 : STD_LOGIC; 
  signal CHOICE13802 : STD_LOGIC; 
  signal CHOICE14259 : STD_LOGIC; 
  signal CHOICE14603 : STD_LOGIC; 
  signal N7969 : STD_LOGIC; 
  signal CHOICE13369 : STD_LOGIC; 
  signal CHOICE13192 : STD_LOGIC; 
  signal CHOICE12965 : STD_LOGIC; 
  signal CHOICE14116 : STD_LOGIC; 
  signal N8331 : STD_LOGIC; 
  signal CHOICE12793 : STD_LOGIC; 
  signal N8415 : STD_LOGIC; 
  signal CHOICE13734 : STD_LOGIC; 
  signal CHOICE12803 : STD_LOGIC; 
  signal CHOICE13419 : STD_LOGIC; 
  signal N8321 : STD_LOGIC; 
  signal CHOICE14264 : STD_LOGIC; 
  signal CHOICE14026 : STD_LOGIC; 
  signal N8161 : STD_LOGIC; 
  signal CHOICE15025 : STD_LOGIC; 
  signal CHOICE13955 : STD_LOGIC; 
  signal CHOICE13556 : STD_LOGIC; 
  signal CHOICE13451 : STD_LOGIC; 
  signal N8053 : STD_LOGIC; 
  signal N8035 : STD_LOGIC; 
  signal CHOICE13232 : STD_LOGIC; 
  signal CHOICE12798 : STD_LOGIC; 
  signal N8317 : STD_LOGIC; 
  signal N8494 : STD_LOGIC; 
  signal CHOICE13571 : STD_LOGIC; 
  signal CHOICE14048 : STD_LOGIC; 
  signal CHOICE12834 : STD_LOGIC; 
  signal CHOICE12972 : STD_LOGIC; 
  signal mod0_bitptr_2_rt : STD_LOGIC; 
  signal CHOICE13397 : STD_LOGIC; 
  signal CHOICE13203 : STD_LOGIC; 
  signal CHOICE14250 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_8_rt : STD_LOGIC; 
  signal CHOICE14133 : STD_LOGIC; 
  signal CHOICE15066 : STD_LOGIC; 
  signal CHOICE13185 : STD_LOGIC; 
  signal N8311 : STD_LOGIC; 
  signal CHOICE14389 : STD_LOGIC; 
  signal CHOICE13779 : STD_LOGIC; 
  signal CHOICE12811 : STD_LOGIC; 
  signal CHOICE14038 : STD_LOGIC; 
  signal CHOICE14251 : STD_LOGIC; 
  signal CHOICE14356 : STD_LOGIC; 
  signal CHOICE14239 : STD_LOGIC; 
  signal CHOICE12835 : STD_LOGIC; 
  signal CHOICE13818 : STD_LOGIC; 
  signal CHOICE13201 : STD_LOGIC; 
  signal CHOICE14490 : STD_LOGIC; 
  signal N8347 : STD_LOGIC; 
  signal CHOICE14128 : STD_LOGIC; 
  signal CHOICE14245 : STD_LOGIC; 
  signal CHOICE13679 : STD_LOGIC; 
  signal CHOICE13924 : STD_LOGIC; 
  signal CHOICE13340 : STD_LOGIC; 
  signal N8115 : STD_LOGIC; 
  signal N8219 : STD_LOGIC; 
  signal CHOICE12816 : STD_LOGIC; 
  signal CHOICE13489 : STD_LOGIC; 
  signal CHOICE14378 : STD_LOGIC; 
  signal CHOICE13584 : STD_LOGIC; 
  signal CHOICE14124 : STD_LOGIC; 
  signal CHOICE13296 : STD_LOGIC; 
  signal N8213 : STD_LOGIC; 
  signal N8131 : STD_LOGIC; 
  signal CHOICE13270 : STD_LOGIC; 
  signal CHOICE14091 : STD_LOGIC; 
  signal CHOICE13413 : STD_LOGIC; 
  signal CHOICE14236 : STD_LOGIC; 
  signal CHOICE13604 : STD_LOGIC; 
  signal mod0_bitptr_3_rt : STD_LOGIC; 
  signal CHOICE13898 : STD_LOGIC; 
  signal CHOICE13706 : STD_LOGIC; 
  signal N8446 : STD_LOGIC; 
  signal N8187 : STD_LOGIC; 
  signal CHOICE12821 : STD_LOGIC; 
  signal CHOICE13384 : STD_LOGIC; 
  signal N8458 : STD_LOGIC; 
  signal CHOICE13213 : STD_LOGIC; 
  signal CHOICE14234 : STD_LOGIC; 
  signal CHOICE14225 : STD_LOGIC; 
  signal CHOICE12823 : STD_LOGIC; 
  signal CHOICE12993 : STD_LOGIC; 
  signal CHOICE12839 : STD_LOGIC; 
  signal mod0_bitptr_4_rt : STD_LOGIC; 
  signal CHOICE14098 : STD_LOGIC; 
  signal CHOICE13281 : STD_LOGIC; 
  signal N8417 : STD_LOGIC; 
  signal CHOICE14101 : STD_LOGIC; 
  signal mod5_hdframe_linecount_2_rt : STD_LOGIC; 
  signal CHOICE13838 : STD_LOGIC; 
  signal CHOICE13968 : STD_LOGIC; 
  signal CHOICE14114 : STD_LOGIC; 
  signal CHOICE14136 : STD_LOGIC; 
  signal CHOICE13817 : STD_LOGIC; 
  signal CHOICE13995 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_1_rt : STD_LOGIC; 
  signal CHOICE14095 : STD_LOGIC; 
  signal CHOICE14228 : STD_LOGIC; 
  signal CHOICE14522 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_10_rt : STD_LOGIC; 
  signal N8211 : STD_LOGIC; 
  signal CHOICE12970 : STD_LOGIC; 
  signal mod7_active_line_4_rt : STD_LOGIC; 
  signal mod5_hdframe_linecount_3_rt : STD_LOGIC; 
  signal CHOICE14088 : STD_LOGIC; 
  signal N8396 : STD_LOGIC; 
  signal CHOICE13520 : STD_LOGIC; 
  signal CHOICE13628 : STD_LOGIC; 
  signal CHOICE14695 : STD_LOGIC; 
  signal CHOICE15417 : STD_LOGIC; 
  signal CHOICE15183 : STD_LOGIC; 
  signal mod7_active_line_1_rt : STD_LOGIC; 
  signal CHOICE14199 : STD_LOGIC; 
  signal CHOICE13046 : STD_LOGIC; 
  signal CHOICE14222 : STD_LOGIC; 
  signal CHOICE13272 : STD_LOGIC; 
  signal CHOICE15243 : STD_LOGIC; 
  signal N8223 : STD_LOGIC; 
  signal CHOICE13733 : STD_LOGIC; 
  signal CHOICE14535 : STD_LOGIC; 
  signal CHOICE15411 : STD_LOGIC; 
  signal CHOICE13753 : STD_LOGIC; 
  signal CHOICE13026 : STD_LOGIC; 
  signal CHOICE12848 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_8_rt2 : STD_LOGIC; 
  signal CHOICE12971 : STD_LOGIC; 
  signal mod5_hdframe_linecount_4_rt : STD_LOGIC; 
  signal CHOICE13018 : STD_LOGIC; 
  signal CHOICE12879 : STD_LOGIC; 
  signal CHOICE13986 : STD_LOGIC; 
  signal CHOICE13984 : STD_LOGIC; 
  signal N8413 : STD_LOGIC; 
  signal CHOICE13448 : STD_LOGIC; 
  signal CHOICE13327 : STD_LOGIC; 
  signal CHOICE13365 : STD_LOGIC; 
  signal CHOICE14069 : STD_LOGIC; 
  signal N7987 : STD_LOGIC; 
  signal CHOICE15065 : STD_LOGIC; 
  signal N7989 : STD_LOGIC; 
  signal CHOICE14218 : STD_LOGIC; 
  signal N8427 : STD_LOGIC; 
  signal CHOICE13736 : STD_LOGIC; 
  signal N7992 : STD_LOGIC; 
  signal N8488 : STD_LOGIC; 
  signal CHOICE13911 : STD_LOGIC; 
  signal N8341 : STD_LOGIC; 
  signal CHOICE15163 : STD_LOGIC; 
  signal CHOICE13115 : STD_LOGIC; 
  signal N8165 : STD_LOGIC; 
  signal CHOICE12856 : STD_LOGIC; 
  signal N8437 : STD_LOGIC; 
  signal CHOICE14242 : STD_LOGIC; 
  signal CHOICE14247 : STD_LOGIC; 
  signal CHOICE13028 : STD_LOGIC; 
  signal CHOICE13921 : STD_LOGIC; 
  signal CHOICE14204 : STD_LOGIC; 
  signal N8435 : STD_LOGIC; 
  signal CHOICE13149 : STD_LOGIC; 
  signal CHOICE12858 : STD_LOGIC; 
  signal CHOICE13004 : STD_LOGIC; 
  signal CHOICE13910 : STD_LOGIC; 
  signal N7990 : STD_LOGIC; 
  signal CHOICE12859 : STD_LOGIC; 
  signal N7988 : STD_LOGIC; 
  signal CHOICE13640 : STD_LOGIC; 
  signal CHOICE13271 : STD_LOGIC; 
  signal CHOICE14778 : STD_LOGIC; 
  signal CHOICE13189 : STD_LOGIC; 
  signal CHOICE13732 : STD_LOGIC; 
  signal CHOICE13494 : STD_LOGIC; 
  signal CHOICE14411 : STD_LOGIC; 
  signal CHOICE12861 : STD_LOGIC; 
  signal CHOICE13554 : STD_LOGIC; 
  signal N7981 : STD_LOGIC; 
  signal N8445 : STD_LOGIC; 
  signal CHOICE13777 : STD_LOGIC; 
  signal CHOICE15061 : STD_LOGIC; 
  signal CHOICE13991 : STD_LOGIC; 
  signal CHOICE13273 : STD_LOGIC; 
  signal N8360 : STD_LOGIC; 
  signal N8387 : STD_LOGIC; 
  signal CHOICE13827 : STD_LOGIC; 
  signal N8335 : STD_LOGIC; 
  signal CHOICE14195 : STD_LOGIC; 
  signal CHOICE13954 : STD_LOGIC; 
  signal CHOICE13295 : STD_LOGIC; 
  signal N8181 : STD_LOGIC; 
  signal CHOICE13233 : STD_LOGIC; 
  signal CHOICE13922 : STD_LOGIC; 
  signal N8361 : STD_LOGIC; 
  signal mod5_hdframe_linecount_5_rt : STD_LOGIC; 
  signal CHOICE13044 : STD_LOGIC; 
  signal CHOICE13525 : STD_LOGIC; 
  signal CHOICE13205 : STD_LOGIC; 
  signal CHOICE14519 : STD_LOGIC; 
  signal CHOICE13465 : STD_LOGIC; 
  signal N8169 : STD_LOGIC; 
  signal CHOICE13758 : STD_LOGIC; 
  signal CHOICE14655 : STD_LOGIC; 
  signal CHOICE14737 : STD_LOGIC; 
  signal N8409 : STD_LOGIC; 
  signal CHOICE13992 : STD_LOGIC; 
  signal CHOICE14187 : STD_LOGIC; 
  signal mod7_active_line_2_rt : STD_LOGIC; 
  signal CHOICE13912 : STD_LOGIC; 
  signal CHOICE14173 : STD_LOGIC; 
  signal CHOICE12885 : STD_LOGIC; 
  signal CHOICE13302 : STD_LOGIC; 
  signal CHOICE14080 : STD_LOGIC; 
  signal CHOICE13383 : STD_LOGIC; 
  signal CHOICE15169 : STD_LOGIC; 
  signal CHOICE13439 : STD_LOGIC; 
  signal CHOICE12886 : STD_LOGIC; 
  signal CHOICE12905 : STD_LOGIC; 
  signal CHOICE13366 : STD_LOGIC; 
  signal CHOICE15339 : STD_LOGIC; 
  signal CHOICE13172 : STD_LOGIC; 
  signal CHOICE14059 : STD_LOGIC; 
  signal CHOICE13166 : STD_LOGIC; 
  signal CHOICE14212 : STD_LOGIC; 
  signal CHOICE13881 : STD_LOGIC; 
  signal N8147 : STD_LOGIC; 
  signal CHOICE13128 : STD_LOGIC; 
  signal N8225 : STD_LOGIC; 
  signal CHOICE13202 : STD_LOGIC; 
  signal N8287 : STD_LOGIC; 
  signal N8255 : STD_LOGIC; 
  signal mod7_active_line_9_rt : STD_LOGIC; 
  signal CHOICE13006 : STD_LOGIC; 
  signal CHOICE13177 : STD_LOGIC; 
  signal N8407 : STD_LOGIC; 
  signal N8319 : STD_LOGIC; 
  signal CHOICE13629 : STD_LOGIC; 
  signal CHOICE12893 : STD_LOGIC; 
  signal N8492 : STD_LOGIC; 
  signal N8297 : STD_LOGIC; 
  signal N8385 : STD_LOGIC; 
  signal CHOICE12908 : STD_LOGIC; 
  signal CHOICE13054 : STD_LOGIC; 
  signal CHOICE13828 : STD_LOGIC; 
  signal CHOICE14039 : STD_LOGIC; 
  signal CHOICE14181 : STD_LOGIC; 
  signal N8119 : STD_LOGIC; 
  signal CHOICE12895 : STD_LOGIC; 
  signal N8159 : STD_LOGIC; 
  signal CHOICE13515 : STD_LOGIC; 
  signal CHOICE13926 : STD_LOGIC; 
  signal CHOICE13130 : STD_LOGIC; 
  signal CHOICE14177 : STD_LOGIC; 
  signal CHOICE14649 : STD_LOGIC; 
  signal CHOICE13042 : STD_LOGIC; 
  signal CHOICE14172 : STD_LOGIC; 
  signal CHOICE13874 : STD_LOGIC; 
  signal CHOICE14175 : STD_LOGIC; 
  signal mod7_active_line_3_rt : STD_LOGIC; 
  signal mod5_hdframe_linecount_6_rt : STD_LOGIC; 
  signal N4283 : STD_LOGIC; 
  signal CHOICE14017 : STD_LOGIC; 
  signal N8221 : STD_LOGIC; 
  signal N8291 : STD_LOGIC; 
  signal CHOICE13650 : STD_LOGIC; 
  signal CHOICE14210 : STD_LOGIC; 
  signal CHOICE13748 : STD_LOGIC; 
  signal N8173 : STD_LOGIC; 
  signal N8251 : STD_LOGIC; 
  signal N7971 : STD_LOGIC; 
  signal CHOICE14500 : STD_LOGIC; 
  signal N8185 : STD_LOGIC; 
  signal CHOICE13370 : STD_LOGIC; 
  signal CHOICE13069 : STD_LOGIC; 
  signal CHOICE13219 : STD_LOGIC; 
  signal CHOICE14159 : STD_LOGIC; 
  signal CHOICE15015 : STD_LOGIC; 
  signal N8163 : STD_LOGIC; 
  signal CHOICE13976 : STD_LOGIC; 
  signal N8215 : STD_LOGIC; 
  signal CHOICE13735 : STD_LOGIC; 
  signal CHOICE14042 : STD_LOGIC; 
  signal CHOICE13482 : STD_LOGIC; 
  signal N8191 : STD_LOGIC; 
  signal CHOICE13750 : STD_LOGIC; 
  signal CHOICE13314 : STD_LOGIC; 
  signal CHOICE13068 : STD_LOGIC; 
  signal CHOICE14168 : STD_LOGIC; 
  signal CHOICE13118 : STD_LOGIC; 
  signal CHOICE12940 : STD_LOGIC; 
  signal N8217 : STD_LOGIC; 
  signal CHOICE14630 : STD_LOGIC; 
  signal CHOICE12920 : STD_LOGIC; 
  signal N8345 : STD_LOGIC; 
  signal CHOICE13972 : STD_LOGIC; 
  signal CHOICE13305 : STD_LOGIC; 
  signal CHOICE13570 : STD_LOGIC; 
  signal N4514 : STD_LOGIC; 
  signal N8183 : STD_LOGIC; 
  signal N8358 : STD_LOGIC; 
  signal CHOICE14166 : STD_LOGIC; 
  signal N8227 : STD_LOGIC; 
  signal CHOICE14156 : STD_LOGIC; 
  signal N8301 : STD_LOGIC; 
  signal CHOICE15137 : STD_LOGIC; 
  signal N8026 : STD_LOGIC; 
  signal CHOICE12943 : STD_LOGIC; 
  signal mod7_color_yramp_9_rt : STD_LOGIC; 
  signal CHOICE13120 : STD_LOGIC; 
  signal CHOICE13400 : STD_LOGIC; 
  signal N7970 : STD_LOGIC; 
  signal CHOICE13545 : STD_LOGIC; 
  signal CHOICE13430 : STD_LOGIC; 
  signal CHOICE14502 : STD_LOGIC; 
  signal CHOICE14164 : STD_LOGIC; 
  signal mod7_active_line_5_rt : STD_LOGIC; 
  signal CHOICE13398 : STD_LOGIC; 
  signal CHOICE13065 : STD_LOGIC; 
  signal N8398 : STD_LOGIC; 
  signal N8149 : STD_LOGIC; 
  signal CHOICE14207 : STD_LOGIC; 
  signal N8305 : STD_LOGIC; 
  signal CHOICE12930 : STD_LOGIC; 
  signal N8337 : STD_LOGIC; 
  signal CHOICE13023 : STD_LOGIC; 
  signal CHOICE13326 : STD_LOGIC; 
  signal N8421 : STD_LOGIC; 
  signal CHOICE13135 : STD_LOGIC; 
  signal CHOICE14349 : STD_LOGIC; 
  signal CHOICE13168 : STD_LOGIC; 
  signal CHOICE13504 : STD_LOGIC; 
  signal N8468 : STD_LOGIC; 
  signal N8299 : STD_LOGIC; 
  signal CHOICE12933 : STD_LOGIC; 
  signal CHOICE15030 : STD_LOGIC; 
  signal N8267 : STD_LOGIC; 
  signal N8083 : STD_LOGIC; 
  signal CHOICE15096 : STD_LOGIC; 
  signal N8036 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_9_rt : STD_LOGIC; 
  signal CHOICE15251 : STD_LOGIC; 
  signal CHOICE14608 : STD_LOGIC; 
  signal CHOICE14781 : STD_LOGIC; 
  signal CHOICE14291 : STD_LOGIC; 
  signal N8462 : STD_LOGIC; 
  signal N8153 : STD_LOGIC; 
  signal CHOICE15227 : STD_LOGIC; 
  signal CHOICE15051 : STD_LOGIC; 
  signal CHOICE14788 : STD_LOGIC; 
  signal CHOICE15234 : STD_LOGIC; 
  signal CHOICE14293 : STD_LOGIC; 
  signal CHOICE15311 : STD_LOGIC; 
  signal CHOICE14685 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_10_rt1 : STD_LOGIC; 
  signal CHOICE15284 : STD_LOGIC; 
  signal CHOICE15188 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_4_rt : STD_LOGIC; 
  signal CHOICE14753 : STD_LOGIC; 
  signal CHOICE14324 : STD_LOGIC; 
  signal CHOICE15325 : STD_LOGIC; 
  signal N7975 : STD_LOGIC; 
  signal CHOICE15131 : STD_LOGIC; 
  signal N8151 : STD_LOGIC; 
  signal CHOICE14573 : STD_LOGIC; 
  signal CHOICE15490 : STD_LOGIC; 
  signal CHOICE14581 : STD_LOGIC; 
  signal CHOICE15272 : STD_LOGIC; 
  signal CHOICE14659 : STD_LOGIC; 
  signal CHOICE14300 : STD_LOGIC; 
  signal CHOICE15493 : STD_LOGIC; 
  signal CHOICE15362 : STD_LOGIC; 
  signal CHOICE14792 : STD_LOGIC; 
  signal N8448 : STD_LOGIC; 
  signal N8059 : STD_LOGIC; 
  signal CHOICE15176 : STD_LOGIC; 
  signal CHOICE14597 : STD_LOGIC; 
  signal CHOICE15081 : STD_LOGIC; 
  signal CHOICE15369 : STD_LOGIC; 
  signal CHOICE14327 : STD_LOGIC; 
  signal CHOICE14924 : STD_LOGIC; 
  signal CHOICE15277 : STD_LOGIC; 
  signal CHOICE14786 : STD_LOGIC; 
  signal CHOICE14304 : STD_LOGIC; 
  signal CHOICE15020 : STD_LOGIC; 
  signal CHOICE14480 : STD_LOGIC; 
  signal CHOICE15486 : STD_LOGIC; 
  signal CHOICE14940 : STD_LOGIC; 
  signal N8023 : STD_LOGIC; 
  signal N8056 : STD_LOGIC; 
  signal CHOICE14592 : STD_LOGIC; 
  signal CHOICE15485 : STD_LOGIC; 
  signal N8074 : STD_LOGIC; 
  signal CHOICE14693 : STD_LOGIC; 
  signal CHOICE14990 : STD_LOGIC; 
  signal CHOICE14576 : STD_LOGIC; 
  signal N6921 : STD_LOGIC; 
  signal CHOICE14307 : STD_LOGIC; 
  signal N8257 : STD_LOGIC; 
  signal N8145 : STD_LOGIC; 
  signal CHOICE15397 : STD_LOGIC; 
  signal CHOICE15282 : STD_LOGIC; 
  signal N8129 : STD_LOGIC; 
  signal CHOICE14309 : STD_LOGIC; 
  signal N8065 : STD_LOGIC; 
  signal CHOICE15359 : STD_LOGIC; 
  signal CHOICE14572 : STD_LOGIC; 
  signal CHOICE15043 : STD_LOGIC; 
  signal CHOICE14596 : STD_LOGIC; 
  signal CHOICE14647 : STD_LOGIC; 
  signal CHOICE14456 : STD_LOGIC; 
  signal CHOICE15482 : STD_LOGIC; 
  signal CHOICE15004 : STD_LOGIC; 
  signal N8265 : STD_LOGIC; 
  signal CHOICE14467 : STD_LOGIC; 
  signal CHOICE15074 : STD_LOGIC; 
  signal N8239 : STD_LOGIC; 
  signal CHOICE14463 : STD_LOGIC; 
  signal CHOICE15479 : STD_LOGIC; 
  signal N8157 : STD_LOGIC; 
  signal CHOICE14658 : STD_LOGIC; 
  signal N8047 : STD_LOGIC; 
  signal CHOICE14315 : STD_LOGIC; 
  signal CHOICE15279 : STD_LOGIC; 
  signal CHOICE15320 : STD_LOGIC; 
  signal CHOICE14392 : STD_LOGIC; 
  signal CHOICE14699 : STD_LOGIC; 
  signal CHOICE15197 : STD_LOGIC; 
  signal CHOICE15360 : STD_LOGIC; 
  signal CHOICE15478 : STD_LOGIC; 
  signal CHOICE15256 : STD_LOGIC; 
  signal CHOICE14979 : STD_LOGIC; 
  signal CHOICE15070 : STD_LOGIC; 
  signal N8052 : STD_LOGIC; 
  signal CHOICE15248 : STD_LOGIC; 
  signal CHOICE14419 : STD_LOGIC; 
  signal CHOICE15441 : STD_LOGIC; 
  signal CHOICE14484 : STD_LOGIC; 
  signal CHOICE14899 : STD_LOGIC; 
  signal CHOICE15342 : STD_LOGIC; 
  signal CHOICE14667 : STD_LOGIC; 
  signal N8504 : STD_LOGIC; 
  signal CHOICE14442 : STD_LOGIC; 
  signal CHOICE15024 : STD_LOGIC; 
  signal CHOICE15028 : STD_LOGIC; 
  signal CHOICE14636 : STD_LOGIC; 
  signal CHOICE14335 : STD_LOGIC; 
  signal CHOICE14662 : STD_LOGIC; 
  signal N7978 : STD_LOGIC; 
  signal N8081 : STD_LOGIC; 
  signal CHOICE14509 : STD_LOGIC; 
  signal CHOICE14854 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_11_rt : STD_LOGIC; 
  signal N8024 : STD_LOGIC; 
  signal CHOICE14459 : STD_LOGIC; 
  signal CHOICE15420 : STD_LOGIC; 
  signal N8055 : STD_LOGIC; 
  signal CHOICE14973 : STD_LOGIC; 
  signal N8490 : STD_LOGIC; 
  signal CHOICE14583 : STD_LOGIC; 
  signal CHOICE14370 : STD_LOGIC; 
  signal CHOICE14339 : STD_LOGIC; 
  signal CHOICE15151 : STD_LOGIC; 
  signal CHOICE14958 : STD_LOGIC; 
  signal N7976 : STD_LOGIC; 
  signal CHOICE14906 : STD_LOGIC; 
  signal mod7_h_state_FFd4_2 : STD_LOGIC; 
  signal CHOICE14941 : STD_LOGIC; 
  signal CHOICE14374 : STD_LOGIC; 
  signal CHOICE15301 : STD_LOGIC; 
  signal CHOICE14944 : STD_LOGIC; 
  signal CHOICE15412 : STD_LOGIC; 
  signal CHOICE14342 : STD_LOGIC; 
  signal CHOICE14440 : STD_LOGIC; 
  signal CHOICE15010 : STD_LOGIC; 
  signal CHOICE14720 : STD_LOGIC; 
  signal CHOICE14526 : STD_LOGIC; 
  signal N8039 : STD_LOGIC; 
  signal CHOICE14864 : STD_LOGIC; 
  signal mod5_hdframe_samplecount_3_rt : STD_LOGIC; 
  signal CHOICE14495 : STD_LOGIC; 
  signal N8058 : STD_LOGIC; 
  signal CHOICE15220 : STD_LOGIC; 
  signal CHOICE14878 : STD_LOGIC; 
  signal CHOICE14346 : STD_LOGIC; 
  signal CHOICE15180 : STD_LOGIC; 
  signal CHOICE15103 : STD_LOGIC; 
  signal CHOICE15231 : STD_LOGIC; 
  signal CHOICE14732 : STD_LOGIC; 
  signal CHOICE14828 : STD_LOGIC; 
  signal CHOICE14849 : STD_LOGIC; 
  signal CHOICE15454 : STD_LOGIC; 
  signal CHOICE14740 : STD_LOGIC; 
  signal N8261 : STD_LOGIC; 
  signal CHOICE14538 : STD_LOGIC; 
  signal mod7_h_state_FFd3_1 : STD_LOGIC; 
  signal mod7_h_state_FFd1_1 : STD_LOGIC; 
  signal mod7_h_state_FFd4_1 : STD_LOGIC; 
  signal mod7_h_state_FFd2_1 : STD_LOGIC; 
  signal mod7_h_state_FFd3_2 : STD_LOGIC; 
  signal mod7_h_state_FFd2_2 : STD_LOGIC; 
  signal mod7_h_state_FFd3_3 : STD_LOGIC; 
  signal mod7_h_state_FFd2_3 : STD_LOGIC; 
  signal mod7_h_state_FFd4_3 : STD_LOGIC; 
  signal mod7_h_state_FFd1_3 : STD_LOGIC; 
  signal mod7_color_Ker1211 : STD_LOGIC; 
  signal mod7_h_state_FFd4_4 : STD_LOGIC; 
  signal mod7_h_state_FFd1_4 : STD_LOGIC; 
  signal mod7_h_state_FFd3_4 : STD_LOGIC; 
  signal mod7_h_state_FFd2_4 : STD_LOGIC; 
  signal mod7_h_state_FFd4_5 : STD_LOGIC; 
  signal mod7_h_state_FFd1_5 : STD_LOGIC; 
  signal mod7_h_state_FFd3_5 : STD_LOGIC; 
  signal mod7_h_state_FFd2_5 : STD_LOGIC; 
  signal txclka21 : STD_LOGIC; 
  signal N8510 : STD_LOGIC; 
  signal N8511 : STD_LOGIC; 
  signal N8512 : STD_LOGIC; 
  signal N8513 : STD_LOGIC; 
  signal N8514 : STD_LOGIC; 
  signal N8515 : STD_LOGIC; 
  signal N8516 : STD_LOGIC; 
  signal N8517 : STD_LOGIC; 
  signal N8518 : STD_LOGIC; 
  signal N8519 : STD_LOGIC; 
  signal N8520 : STD_LOGIC; 
  signal N8521 : STD_LOGIC; 
  signal N8522 : STD_LOGIC; 
  signal N8523 : STD_LOGIC; 
  signal N8524 : STD_LOGIC; 
  signal N8525 : STD_LOGIC; 
  signal N8526 : STD_LOGIC; 
  signal N8527 : STD_LOGIC; 
  signal N8528 : STD_LOGIC; 
  signal N8529 : STD_LOGIC; 
  signal N8530 : STD_LOGIC; 
  signal N8531 : STD_LOGIC; 
  signal N8532 : STD_LOGIC; 
  signal N8533 : STD_LOGIC; 
  signal N8534 : STD_LOGIC; 
  signal N8535 : STD_LOGIC; 
  signal N8536 : STD_LOGIC; 
  signal N8537 : STD_LOGIC; 
  signal N8538 : STD_LOGIC; 
  signal N8539 : STD_LOGIC; 
  signal N8540 : STD_LOGIC; 
  signal N8541 : STD_LOGIC; 
  signal N8542 : STD_LOGIC; 
  signal N8543 : STD_LOGIC; 
  signal N8544 : STD_LOGIC; 
  signal N8545 : STD_LOGIC; 
  signal N8546 : STD_LOGIC; 
  signal N8547 : STD_LOGIC; 
  signal N8548 : STD_LOGIC; 
  signal N8549 : STD_LOGIC; 
  signal N8550 : STD_LOGIC; 
  signal N8551 : STD_LOGIC; 
  signal N8552 : STD_LOGIC; 
  signal N8553 : STD_LOGIC; 
  signal N8554 : STD_LOGIC; 
  signal N8555 : STD_LOGIC; 
  signal N8556 : STD_LOGIC; 
  signal N8557 : STD_LOGIC; 
  signal N8558 : STD_LOGIC; 
  signal N8559 : STD_LOGIC; 
  signal N8560 : STD_LOGIC; 
  signal N8561 : STD_LOGIC; 
  signal N8562 : STD_LOGIC; 
  signal N8563 : STD_LOGIC; 
  signal N8564 : STD_LOGIC; 
  signal N8565 : STD_LOGIC; 
  signal N8566 : STD_LOGIC; 
  signal N8567 : STD_LOGIC; 
  signal N8568 : STD_LOGIC; 
  signal N8569 : STD_LOGIC; 
  signal N8570 : STD_LOGIC; 
  signal N8571 : STD_LOGIC; 
  signal N8572 : STD_LOGIC; 
  signal N8573 : STD_LOGIC; 
  signal N8574 : STD_LOGIC; 
  signal N8575 : STD_LOGIC; 
  signal N8576 : STD_LOGIC; 
  signal N8577 : STD_LOGIC; 
  signal N8578 : STD_LOGIC; 
  signal N8579 : STD_LOGIC; 
  signal N8580 : STD_LOGIC; 
  signal N8581 : STD_LOGIC; 
  signal N8582 : STD_LOGIC; 
  signal N8583 : STD_LOGIC; 
  signal N8584 : STD_LOGIC; 
  signal N8585 : STD_LOGIC; 
  signal N8586 : STD_LOGIC; 
  signal N8587 : STD_LOGIC; 
  signal N8588 : STD_LOGIC; 
  signal N8589 : STD_LOGIC; 
  signal N8590 : STD_LOGIC; 
  signal N8591 : STD_LOGIC; 
  signal N8592 : STD_LOGIC; 
  signal N8593 : STD_LOGIC; 
  signal N8594 : STD_LOGIC; 
  signal N8595 : STD_LOGIC; 
  signal N8596 : STD_LOGIC; 
  signal N8597 : STD_LOGIC; 
  signal N8598 : STD_LOGIC; 
  signal N8599 : STD_LOGIC; 
  signal N8600 : STD_LOGIC; 
  signal N8601 : STD_LOGIC; 
  signal N8602 : STD_LOGIC; 
  signal N8603 : STD_LOGIC; 
  signal N8604 : STD_LOGIC; 
  signal N8605 : STD_LOGIC; 
  signal N8606 : STD_LOGIC; 
  signal N8607 : STD_LOGIC; 
  signal N8608 : STD_LOGIC; 
  signal N8609 : STD_LOGIC; 
  signal N8610 : STD_LOGIC; 
  signal N8611 : STD_LOGIC; 
  signal N8612 : STD_LOGIC; 
  signal N8613 : STD_LOGIC; 
  signal N8614 : STD_LOGIC; 
  signal N8615 : STD_LOGIC; 
  signal N8616 : STD_LOGIC; 
  signal N8617 : STD_LOGIC; 
  signal N8618 : STD_LOGIC; 
  signal N8619 : STD_LOGIC; 
  signal N8620 : STD_LOGIC; 
  signal N8621 : STD_LOGIC; 
  signal N8622 : STD_LOGIC; 
  signal N8623 : STD_LOGIC; 
  signal N8624 : STD_LOGIC; 
  signal N8625 : STD_LOGIC; 
  signal N8626 : STD_LOGIC; 
  signal N8627 : STD_LOGIC; 
  signal N8628 : STD_LOGIC; 
  signal N8629 : STD_LOGIC; 
  signal N8630 : STD_LOGIC; 
  signal N8631 : STD_LOGIC; 
  signal N8632 : STD_LOGIC; 
  signal N8633 : STD_LOGIC; 
  signal N8634 : STD_LOGIC; 
  signal N8635 : STD_LOGIC; 
  signal N8636 : STD_LOGIC; 
  signal N8637 : STD_LOGIC; 
  signal N8638 : STD_LOGIC; 
  signal N8639 : STD_LOGIC; 
  signal N8640 : STD_LOGIC; 
  signal N8641 : STD_LOGIC; 
  signal N8642 : STD_LOGIC; 
  signal N8643 : STD_LOGIC; 
  signal N8644 : STD_LOGIC; 
  signal N8645 : STD_LOGIC; 
  signal N8646 : STD_LOGIC; 
  signal N8647 : STD_LOGIC; 
  signal N8648 : STD_LOGIC; 
  signal N8649 : STD_LOGIC; 
  signal N8650 : STD_LOGIC; 
  signal N8651 : STD_LOGIC; 
  signal N8652 : STD_LOGIC; 
  signal N8653 : STD_LOGIC; 
  signal N8654 : STD_LOGIC; 
  signal N8655 : STD_LOGIC; 
  signal txda_9_1 : STD_LOGIC; 
  signal txda_19_1 : STD_LOGIC; 
  signal mod0_sd_hd_1 : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_CHBONDO_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_CHBONDO_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_CHBONDO_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_CHBONDO_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_CHBONDO_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXSTATUS_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXSTATUS_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXSTATUS_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXSTATUS_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXSTATUS_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXSTATUS_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISCOMMA_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISCOMMA_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISCOMMA_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISCOMMA_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISCOMMA_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISCOMMA_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISCOMMA_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISCOMMA_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISK_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISK_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISK_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISK_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISK_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISK_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISK_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISK_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_63_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_62_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_61_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_60_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_59_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_58_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_57_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_56_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_55_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_54_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_53_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_52_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_51_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_50_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_49_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_48_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_47_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_46_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_45_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_44_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_43_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_42_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_41_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_40_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_39_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_38_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_37_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_36_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_35_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_34_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_33_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_32_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_31_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_30_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_29_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_28_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_27_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_26_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_25_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_24_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_23_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_22_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_21_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_20_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_19_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_18_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_17_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_16_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_15_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_9_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_8_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDISPERR_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDISPERR_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDISPERR_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDISPERR_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDISPERR_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDISPERR_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDISPERR_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXDISPERR_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXLOSSOFSYNC_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXLOSSOFSYNC_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXNOTINTABLE_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXNOTINTABLE_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXNOTINTABLE_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXNOTINTABLE_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXNOTINTABLE_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXNOTINTABLE_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXNOTINTABLE_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXNOTINTABLE_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXRUNDISP_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXRUNDISP_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXRUNDISP_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXRUNDISP_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXRUNDISP_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXRUNDISP_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXRUNDISP_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXRUNDISP_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXKERR_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXKERR_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXKERR_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXKERR_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXKERR_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXKERR_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXKERR_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXKERR_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXRUNDISP_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXRUNDISP_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXRUNDISP_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXRUNDISP_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXRUNDISP_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXRUNDISP_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXRUNDISP_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXRUNDISP_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_31_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_30_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_29_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_28_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_27_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_26_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_25_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_24_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_23_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_22_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_21_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_20_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_19_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_18_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_17_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_16_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_15_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_9_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_8_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_31_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_30_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_29_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_28_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_27_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_26_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_25_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_24_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_23_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_22_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_21_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_20_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_19_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_18_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_17_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_16_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_15_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_9_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_8_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_DO_15_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_DO_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_DO_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_DO_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_DO_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_DO_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_DO_9_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_DO_8_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_DO_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_DO_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_DO_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_DO_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_DO_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_DO_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_DO_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_DO_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_15_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_9_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_8_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCOMMADET_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXREALIGN_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXBUFERR_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXBUFERR_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXRECCLK1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXRECCLK2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXOUTCLK2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXLOCK_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXLOCK_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCYCLELIMIT_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCYCLELIMIT_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXCALFAIL_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXCALFAIL_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXSIGDET_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_DRDY_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXMCLK_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_TXPCSHCLKOUT_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113a_GT11_CUSTOM_INST_RXPCSHCLKOUT_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_CHBONDO_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_CHBONDO_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_CHBONDO_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_CHBONDO_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_CHBONDO_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXSTATUS_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXSTATUS_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXSTATUS_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXSTATUS_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXSTATUS_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXSTATUS_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISCOMMA_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISCOMMA_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISCOMMA_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISCOMMA_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISCOMMA_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISCOMMA_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISCOMMA_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISCOMMA_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISK_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISK_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISK_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISK_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISK_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISK_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISK_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISK_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_63_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_62_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_61_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_60_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_59_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_58_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_57_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_56_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_55_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_54_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_53_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_52_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_51_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_50_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_49_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_48_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_47_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_46_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_45_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_44_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_43_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_42_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_41_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_40_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_39_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_38_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_37_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_36_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_35_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_34_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_33_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_32_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_31_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_30_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_29_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_28_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_27_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_26_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_25_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_24_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_23_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_22_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_21_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_20_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_19_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_18_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_17_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_16_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_15_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_9_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_8_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDISPERR_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDISPERR_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDISPERR_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDISPERR_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDISPERR_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDISPERR_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDISPERR_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXDISPERR_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXLOSSOFSYNC_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXLOSSOFSYNC_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXNOTINTABLE_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXNOTINTABLE_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXNOTINTABLE_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXNOTINTABLE_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXNOTINTABLE_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXNOTINTABLE_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXNOTINTABLE_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXNOTINTABLE_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXRUNDISP_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXRUNDISP_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXRUNDISP_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXRUNDISP_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXRUNDISP_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXRUNDISP_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXRUNDISP_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXRUNDISP_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXKERR_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXKERR_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXKERR_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXKERR_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXKERR_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXKERR_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXKERR_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXKERR_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXRUNDISP_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXRUNDISP_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXRUNDISP_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXRUNDISP_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXRUNDISP_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXRUNDISP_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXRUNDISP_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXRUNDISP_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_31_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_30_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_29_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_28_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_27_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_26_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_25_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_24_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_23_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_22_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_21_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_20_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_19_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_18_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_17_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_16_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_15_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_9_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_8_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_31_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_30_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_29_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_28_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_27_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_26_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_25_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_24_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_23_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_22_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_21_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_20_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_19_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_18_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_17_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_16_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_15_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_9_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_8_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_DO_15_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_DO_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_DO_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_DO_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_DO_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_DO_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_DO_9_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_DO_8_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_DO_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_DO_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_DO_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_DO_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_DO_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_DO_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_DO_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_DO_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_15_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_9_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_8_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCOMMADET_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXREALIGN_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXBUFERR_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXBUFERR_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXRECCLK1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXRECCLK2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXOUTCLK1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXOUTCLK2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXLOCK_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXLOCK_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCYCLELIMIT_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCYCLELIMIT_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXCALFAIL_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXCALFAIL_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXSIGDET_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_DRDY_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXMCLK_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_TXPCSHCLKOUT_UNCONNECTED : STD_LOGIC; 
  signal NLW_mgt113b_GT11_CUSTOM_INST_RXPCSHCLKOUT_UNCONNECTED : STD_LOGIC; 
  signal txda : STD_LOGIC_VECTOR ( 19 downto 0 ); 
  signal mod7_counter_n0002 : STD_LOGIC_VECTOR ( 10 downto 0 ); 
  signal tx292data_ina : STD_LOGIC_VECTOR ( 19 downto 0 ); 
  signal mod0_pattern : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal mod5_hdframe_fvh_out : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal mod5_hdframe_chroma : STD_LOGIC_VECTOR ( 9 downto 0 ); 
  signal mod5_hdframe_luma : STD_LOGIC_VECTOR ( 9 downto 0 ); 
  signal mod7_counter_n0009 : STD_LOGIC_VECTOR ( 21 downto 11 ); 
  signal mod0_hd_format_tmp : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal sif_hd_format : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal mod0_bitptr : STD_LOGIC_VECTOR ( 5 downto 0 ); 
  signal mod6_scram20_inst_dout : STD_LOGIC_VECTOR ( 19 downto 0 ); 
  signal mod0_tp_option : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal mod6_scram20_inst_n0165 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_A : STD_LOGIC_VECTOR ( 19 downto 0 ); 
  signal mod6_scram20_inst_S : STD_LOGIC_VECTOR ( 19 downto 0 ); 
  signal mod6_scram20_inst_N : STD_LOGIC_VECTOR ( 19 downto 0 ); 
  signal mod6_scram20_inst_n0184 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_N2_0 : STD_LOGIC_VECTOR ( 19 downto 0 ); 
  signal mod6_scram20_inst_n0148 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_n0182 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_n0166 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_n0181 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_n0147 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_Mxor_n0025_Xo : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_n0173 : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal mod6_scram20_inst_Mxor_n0010_Xo : STD_LOGIC_VECTOR ( 2 downto 2 ); 
  signal mod6_scram20_inst_n0146 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_n0172 : STD_LOGIC_VECTOR ( 1 downto 1 ); 
  signal mod6_scram20_inst_n0159 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_n0168 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_Mxor_n0009_Xo : STD_LOGIC_VECTOR ( 2 downto 2 ); 
  signal mod6_scram20_inst_n0157 : STD_LOGIC_VECTOR ( 2 downto 2 ); 
  signal mod6_scram20_inst_n0145 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_n0163 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_n0162 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_n0161 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_n0156 : STD_LOGIC_VECTOR ( 2 downto 2 ); 
  signal mod6_scram20_inst_n0160 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_n0183 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_n0164 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod6_scram20_inst_Mxor_n0007_Xo : STD_LOGIC_VECTOR ( 1 downto 1 ); 
  signal mod6_scram20_inst_n0154 : STD_LOGIC_VECTOR ( 2 downto 2 ); 
  signal mod7_n0034 : STD_LOGIC_VECTOR ( 8 downto 0 ); 
  signal mod7_counter_count : STD_LOGIC_VECTOR ( 10 downto 0 ); 
  signal mod7_v_state : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal mod7_n0036 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal mod7_active_line : STD_LOGIC_VECTOR ( 10 downto 0 ); 
  signal mod7_active_line_n0000 : STD_LOGIC_VECTOR ( 10 downto 1 ); 
  signal mod7_load_val : STD_LOGIC_VECTOR ( 8 downto 0 ); 
  signal mod7_n0316 : STD_LOGIC_VECTOR ( 3 downto 3 ); 
  signal mod5_asl : STD_LOGIC_VECTOR ( 7 downto 7 ); 
  signal mod7_color_yramp_n0000 : STD_LOGIC_VECTOR ( 9 downto 1 ); 
  signal mod7_color_yramp : STD_LOGIC_VECTOR ( 9 downto 0 ); 
  signal mod7_color_v1h8_y : STD_LOGIC_VECTOR ( 9 downto 9 ); 
  signal mod7_color_v1h9_y : STD_LOGIC_VECTOR ( 5 downto 5 ); 
  signal mod7_color_v0h5_cr : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod7_color_v1h4_y : STD_LOGIC_VECTOR ( 9 downto 9 ); 
  signal mod5_hdframe_samplecount : STD_LOGIC_VECTOR ( 11 downto 0 ); 
  signal mod5_hdframe_chroma_out : STD_LOGIC_VECTOR ( 9 downto 0 ); 
  signal mod5_hdframe_samplecount_n0000 : STD_LOGIC_VECTOR ( 11 downto 1 ); 
  signal mod5_hdframe_linecount : STD_LOGIC_VECTOR ( 10 downto 0 ); 
  signal mod5_hdframe_chroma_crc : STD_LOGIC_VECTOR ( 17 downto 0 ); 
  signal mod5_hdframe_n0177 : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal mod5_hdframe_n0075 : STD_LOGIC_VECTOR ( 9 downto 0 ); 
  signal mod5_hdframe_n0079 : STD_LOGIC_VECTOR ( 17 downto 0 ); 
  signal mod5_hdframe_luma_crc : STD_LOGIC_VECTOR ( 17 downto 0 ); 
  signal mod5_hdframe_luma_out : STD_LOGIC_VECTOR ( 9 downto 0 ); 
  signal mod5_hdframe_n0070 : STD_LOGIC_VECTOR ( 9 downto 0 ); 
  signal mod5_hdframe_n0074 : STD_LOGIC_VECTOR ( 17 downto 0 ); 
  signal mod0_bitptr_n0000 : STD_LOGIC_VECTOR ( 5 downto 1 ); 
  signal mod5_hdframe_n0190 : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal mod5_hdframe_n0192 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod5_hdframe_n0069 : STD_LOGIC_VECTOR ( 10 downto 0 ); 
  signal mod5_hdframe_n0092 : STD_LOGIC_VECTOR ( 10 downto 1 ); 
  signal mod5_hdframe_n0173 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod5_hdframe_n0179 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod5_hdframe_n0171 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod5_hdframe_n0175 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod5_hdframe_n0193 : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal mod5_hdframe_n0191 : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal mod5_hdframe_luma_crc0 : STD_LOGIC_VECTOR ( 16 downto 14 ); 
  signal mod5_hdframe_chroma_crc0 : STD_LOGIC_VECTOR ( 16 downto 14 ); 
  signal mod5_hdframe_MUX_BLOCK_xyz : STD_LOGIC_VECTOR ( 5 downto 5 ); 
begin
  clockbuf2 : BUFG
    port map (
      I => f7417_IBUFG,
      O => TXRIOA_REFCLK
    );
  clockbuf1 : BUFG
    port map (
      I => f7425_IBUFG,
      O => txclka
    );
  txda_19 : FD
    port map (
      D => mod6_scram20_inst_dout(0),
      C => TXRIOA_USRCLK,
      Q => txda(19)
    );
  clockbuf3 : BUFG
    port map (
      I => tx_clk1_out_113a,
      O => TXRIOA_USRCLK
    );
  tx292data_ina_0 : FD
    port map (
      D => mod5_hdframe_chroma(0),
      C => txclka2,
      Q => tx292data_ina(0)
    );
  sif_rst_1 : FD
    port map (
      D => CS_IBUF,
      C => txclka,
      Q => sif_rst
    );
  mgt113a_GT11_CUSTOM_INST : GT11
    generic map(
      ALIGN_COMMA_WORD => 1,
      BANDGAPSEL => FALSE,
      BIASRESSEL => FALSE,
      CCCB_ARBITRATOR_DISABLE => FALSE,
      CHAN_BOND_LIMIT => 16,
      CHAN_BOND_MODE => "NONE",
      CHAN_BOND_ONE_SHOT => FALSE,
      CHAN_BOND_SEQ_1_1 => "00000000000",
      CHAN_BOND_SEQ_1_2 => "00000000000",
      CHAN_BOND_SEQ_1_3 => "00000000000",
      CHAN_BOND_SEQ_1_4 => "00000000000",
      CHAN_BOND_SEQ_1_MASK => "1110",
      CHAN_BOND_SEQ_2_1 => "00000000000",
      CHAN_BOND_SEQ_2_2 => "00000000000",
      CHAN_BOND_SEQ_2_3 => "00000000000",
      CHAN_BOND_SEQ_2_USE => FALSE,
      CHAN_BOND_SEQ_LEN => 1,
      CLK_CORRECT_USE => FALSE,
      CLK_COR_8B10B_DE => TRUE,
      CLK_COR_MAX_LAT => 44,
      CLK_COR_MIN_LAT => 36,
      CHAN_BOND_SEQ_2_MASK => "1110",
      CLK_COR_SEQ_1_1 => "00010111100",
      CLK_COR_SEQ_1_2 => "00010111100",
      CLK_COR_SEQ_1_3 => "00010111100",
      TXDATA_SEL => "00",
      RXDATA_SEL => "00",
      CLK_COR_SEQ_2_1 => "00010111100",
      CLK_COR_SEQ_2_2 => "00010111100",
      CLK_COR_SEQ_2_3 => "00010111100",
      CLK_COR_SEQ_2_USE => FALSE,
      CLK_COR_SEQ_DROP => FALSE,
      CLK_COR_SEQ_LEN => 1,
      COMMA32 => FALSE,
      RXSLOWDOWN_CAL => "00",
      DEC_MCOMMA_DETECT => FALSE,
      DEC_PCOMMA_DETECT => FALSE,
      DEC_VALID_COMMA_ONLY => FALSE,
      CLK_COR_SEQ_1_4 => "00010111100",
      DIGRX_SYNC_MODE => FALSE,
      ENABLE_DCDR => FALSE,
      FDET_HYS_SEL => "100",
      CYCLE_LIMIT_SEL => "00",
      FDET_LCK_SEL => "001",
      RXVCODAC_INIT => "0001000001",
      GT11_MODE => "SINGLE",
      PMAVBGCTRL => "00000",
      FDET_LCK_CAL => "100",
      COMMA_10B_MASK => X"3FF",
      MCOMMA_DETECT => FALSE,
      OPPOSITE_SELECT => FALSE,
      MCOMMA_32B_VALUE => X"000000F6",
      PCOMMA_DETECT => FALSE,
      PCS_BIT_SLIP => FALSE,
      PMACLKENABLE => TRUE,
      PMACOREPWRENABLE => TRUE,
      PMAIREFTRIM => "0111",
      PMA_BIT_SLIP => FALSE,
      POWER_ENABLE => TRUE,
      REPEATER => FALSE,
      RXACTST => FALSE,
      RXRCPADJ => "010",
      RXAFEPD => FALSE,
      RXAFETST => FALSE,
      RXAPD => FALSE,
      RXBY_32 => TRUE,
      RXAFEEQ => "000000000",
      RXCLK0_FORCE_PMACLK => TRUE,
      RXRIBADJ => "11",
      RXCPSEL => TRUE,
      RXCPTST => FALSE,
      RXCRCCLOCKDOUBLE => FALSE,
      RXCRCENABLE => FALSE,
      SLOWDOWN_CAL => "00",
      RXCRCINVERTGEN => FALSE,
      RXCRCSAMECLOCK => FALSE,
      RXLOOPFILT => "1111",
      PCOMMA_32B_VALUE => X"F6F62828",
      DIGRX_FWDCLK => "10",
      RXDCCOUPLE => FALSE,
      RXDIGRESET => FALSE,
      RXDIGRX => FALSE,
      RXFECONTROL2 => "000",
      RXFDCAL_CLOCK_DIVIDE => "TWO",
      RXFDET_HYS_SEL => "001",
      RXCYCLE_LIMIT_SEL => "00",
      RXFDET_LCK_SEL => "111",
      DCDR_FILTER => "000",
      RXLKADJ => "00000",
      RXCDRLOS => "000000",
      RXASYNCDIVIDE => "00",
      RXLB => FALSE,
      RXCMADJ => "10",
      RXLKAPD => FALSE,
      RXFDET_LCK_CAL => "101",
      RXOUTDIV2SEL => 2,
      RXPD => FALSE,
      RXPDDTST => TRUE,
      RXPLLNDIVSEL => 8,
      RXPMACLKSEL => "GREFCLK",
      RXCLKMODE => "100111",
      RXRCPPD => FALSE,
      RXRECCLK1_USE_SYNC => FALSE,
      RXFETUNE => "01",
      RXRPDPD => FALSE,
      RXRSDPD => FALSE,
      RXFDET_HYS_CAL => "010",
      RXUSRDIVISOR => 1,
      RXLOOPCAL_WAIT => "00",
      RXVCO_CTRL_ENABLE => TRUE,
      RX_BUFFER_USE => TRUE,
      RXCRCINITVAL => X"FFFFFFFF",
      SAMPLE_8X => TRUE,
      SH_CNT_MAX => 64,
      SH_INVALID_CNT_MAX => 16,
      FDET_HYS_CAL => "010",
      TXABPMACLKSEL => "GREFCLK",
      TXAPD => FALSE,
      TXAREFBIASSEL => TRUE,
      TXCLKMODE => "0100",
      TXCLK0_FORCE_PMACLK => TRUE,
      TXPRE_PRDRV_DAC => "111",
      TXCPSEL => TRUE,
      TXCRCCLOCKDOUBLE => FALSE,
      TXCRCENABLE => FALSE,
      VCODAC_INIT => "0000000000",
      TXCRCINVERTGEN => FALSE,
      TXCRCSAMECLOCK => FALSE,
      TXLOOPFILT => "1111",
      CLK_COR_SEQ_1_MASK => "0000",
      TXASYNCDIVIDE => "01",
      TXPOST_TAP_DAC => "00111",
      TXDIGPD => FALSE,
      TXFDCAL_CLOCK_DIVIDE => "NONE",
      TXHIGHSIGNALEN => TRUE,
      RXEQ => X"4000000000000000",
      TXLVLSHFTPD => FALSE,
      TXOUTCLK1_USE_SYNC => FALSE,
      TXOUTDIV2SEL => 4,
      TXPD => FALSE,
      TXPHASESEL => FALSE,
      TXPLLNDIVSEL => 40,
      TXTERMTRIM => "1100",
      TXPOST_PRDRV_DAC => "000",
      TXPOST_TAP_PD => TRUE,
      TXCTRL1 => X"200",
      TXPRE_TAP_PD => TRUE,
      TXSLEWRATE => FALSE,
      TXPRE_TAP_DAC => "00000",
      TX_BUFFER_USE => TRUE,
      TXCRCINITVAL => X"FFFFFFFF",
      LOOPCAL_WAIT => "00",
      VCO_CTRL_ENABLE => TRUE,
      IREFBIASMODE => "11",
      CHAN_BOND_SEQ_2_4 => "00000000000",
      CLK_COR_SEQ_2_4 => "00010111100",
      CLK_COR_SEQ_2_MASK => "0000",
      RX_CLOCK_DIVIDER => "10",
      TX_CLOCK_DIVIDER => "00",
      PMAVREFTRIM => "0111",
      VREFBIASMODE => "11",
      RXFECONTROL1 => "00",
      TXDAT_PRDRV_DAC => "111",
      TXDAT_TAP_DAC => "10000",
      RXCTRL1 => X"200"
    )
    port map (
      ENCHANSYNC => mod5_field1_vblnk1_4_Q,
      ENMCOMMAALIGN => mod5_tsl_4_Q,
      ENPCOMMAALIGN => mod5_tsl_4_Q,
      POWERDOWN => mod5_field1_vblnk1_4_Q,
      RXBLOCKSYNC64B66BUSE => mod5_field1_vblnk1_4_Q,
      RXCOMMADETUSE => mod5_field1_vblnk1_4_Q,
      RXDEC64B66BUSE => mod5_field1_vblnk1_4_Q,
      RXDEC8B10BUSE => mod5_field1_vblnk1_4_Q,
      RXDESCRAM64B66BUSE => mod5_field1_vblnk1_4_Q,
      RXIGNOREBTF => mod5_field1_vblnk1_4_Q,
      RXPOLARITY => mod5_field1_vblnk1_4_Q,
      RXRESET => mod5_field1_vblnk1_4_Q,
      RXSLIDE => mod5_field1_vblnk1_4_Q,
      RXUSRCLK => mod5_field1_vblnk1_4_Q,
      RXUSRCLK2 => TXRIOA_USRCLK,
      TXENC64B66BUSE => mod5_field1_vblnk1_4_Q,
      TXENC8B10BUSE => mod5_field1_vblnk1_4_Q,
      TXGEARBOX64B66BUSE => mod5_field1_vblnk1_4_Q,
      TXINHIBIT => mod5_field1_vblnk1_4_Q,
      TXPOLARITY => mod5_field1_vblnk1_4_Q,
      TXRESET => mod5_field1_vblnk1_4_Q,
      TXSCRAM64B66BUSE => mod5_field1_vblnk1_4_Q,
      TXUSRCLK => TXRIOA_USRCLK,
      TXUSRCLK2 => TXRIOA_USRCLK,
      RXCLKSTABLE => mod5_tsl_4_Q,
      RXPMARESET => mod5_field1_vblnk1_4_Q,
      TXCLKSTABLE => mod5_tsl_4_Q,
      TXPMARESET => mod5_field1_vblnk1_4_Q,
      RXCRCDATAVALID => mod5_field1_vblnk1_4_Q,
      RXCRCINIT => mod5_field1_vblnk1_4_Q,
      RXCRCRESET => mod5_tsl_4_Q,
      RXCRCPD => mod5_field1_vblnk1_4_Q,
      RXCRCCLK => mod5_field1_vblnk1_4_Q,
      RXCRCINTCLK => mod5_field1_vblnk1_4_Q,
      TXCRCDATAVALID => mod5_field1_vblnk1_4_Q,
      TXCRCINIT => mod5_field1_vblnk1_4_Q,
      TXCRCRESET => mod5_tsl_4_Q,
      TXCRCPD => mod5_field1_vblnk1_4_Q,
      TXCRCCLK => mod5_field1_vblnk1_4_Q,
      TXCRCINTCLK => mod5_field1_vblnk1_4_Q,
      TXSYNC => mod5_field1_vblnk1_4_Q,
      RXSYNC => mod5_field1_vblnk1_4_Q,
      TXENOOB => mod5_field1_vblnk1_4_Q,
      DCLK => mod5_field1_vblnk1_4_Q,
      DEN => mod5_field1_vblnk1_4_Q,
      DWE => mod5_field1_vblnk1_4_Q,
      RX1P => mod5_field1_vblnk1_4_Q,
      RX1N => mod5_field1_vblnk1_4_Q,
      GREFCLK => TXRIOA_REFCLK,
      REFCLK1 => mod5_field1_vblnk1_4_Q,
      REFCLK2 => mod5_field1_vblnk1_4_Q,
      RXCOMMADET => NLW_mgt113a_GT11_CUSTOM_INST_RXCOMMADET_UNCONNECTED,
      RXREALIGN => NLW_mgt113a_GT11_CUSTOM_INST_RXREALIGN_UNCONNECTED,
      RXBUFERR => NLW_mgt113a_GT11_CUSTOM_INST_RXBUFERR_UNCONNECTED,
      TXBUFERR => NLW_mgt113a_GT11_CUSTOM_INST_TXBUFERR_UNCONNECTED,
      RXRECCLK1 => NLW_mgt113a_GT11_CUSTOM_INST_RXRECCLK1_UNCONNECTED,
      RXRECCLK2 => NLW_mgt113a_GT11_CUSTOM_INST_RXRECCLK2_UNCONNECTED,
      TXOUTCLK1 => tx_clk1_out_113a,
      TXOUTCLK2 => NLW_mgt113a_GT11_CUSTOM_INST_TXOUTCLK2_UNCONNECTED,
      RXLOCK => NLW_mgt113a_GT11_CUSTOM_INST_RXLOCK_UNCONNECTED,
      TXLOCK => NLW_mgt113a_GT11_CUSTOM_INST_TXLOCK_UNCONNECTED,
      RXCYCLELIMIT => NLW_mgt113a_GT11_CUSTOM_INST_RXCYCLELIMIT_UNCONNECTED,
      TXCYCLELIMIT => NLW_mgt113a_GT11_CUSTOM_INST_TXCYCLELIMIT_UNCONNECTED,
      RXCALFAIL => NLW_mgt113a_GT11_CUSTOM_INST_RXCALFAIL_UNCONNECTED,
      TXCALFAIL => NLW_mgt113a_GT11_CUSTOM_INST_TXCALFAIL_UNCONNECTED,
      RXSIGDET => NLW_mgt113a_GT11_CUSTOM_INST_RXSIGDET_UNCONNECTED,
      DRDY => NLW_mgt113a_GT11_CUSTOM_INST_DRDY_UNCONNECTED,
      RXMCLK => NLW_mgt113a_GT11_CUSTOM_INST_RXMCLK_UNCONNECTED,
      TX1P => TXp_OUT_113a,
      TX1N => TXn_OUT_113a,
      TXPCSHCLKOUT => NLW_mgt113a_GT11_CUSTOM_INST_TXPCSHCLKOUT_UNCONNECTED,
      RXPCSHCLKOUT => NLW_mgt113a_GT11_CUSTOM_INST_RXPCSHCLKOUT_UNCONNECTED,
      CHBONDI(4) => mod5_field1_vblnk1_4_Q,
      CHBONDI(3) => mod5_field1_vblnk1_4_Q,
      CHBONDI(2) => mod5_field1_vblnk1_4_Q,
      CHBONDI(1) => mod5_field1_vblnk1_4_Q,
      CHBONDI(0) => mod5_field1_vblnk1_4_Q,
      LOOPBACK(1) => mod5_field1_vblnk1_4_Q,
      LOOPBACK(0) => mod5_field1_vblnk1_4_Q,
      RXDATAWIDTH(1) => mod5_field1_vblnk1_4_Q,
      RXDATAWIDTH(0) => mod5_field1_vblnk1_4_Q,
      RXINTDATAWIDTH(1) => mod5_tsl_4_Q,
      RXINTDATAWIDTH(0) => mod5_field1_vblnk1_4_Q,
      TXBYPASS8B10B(7) => mod5_tsl_4_Q,
      TXBYPASS8B10B(6) => mod5_tsl_4_Q,
      TXBYPASS8B10B(5) => mod5_tsl_4_Q,
      TXBYPASS8B10B(4) => mod5_tsl_4_Q,
      TXBYPASS8B10B(3) => mod5_tsl_4_Q,
      TXBYPASS8B10B(2) => mod5_tsl_4_Q,
      TXBYPASS8B10B(1) => mod5_tsl_4_Q,
      TXBYPASS8B10B(0) => mod5_tsl_4_Q,
      TXCHARDISPMODE(7) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPMODE(6) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPMODE(5) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPMODE(4) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPMODE(3) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPMODE(2) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPMODE(1) => txda(19),
      TXCHARDISPMODE(0) => txda(9),
      TXCHARDISPVAL(7) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPVAL(6) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPVAL(5) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPVAL(4) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPVAL(3) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPVAL(2) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPVAL(1) => txda(18),
      TXCHARDISPVAL(0) => txda(8),
      TXCHARISK(7) => mod5_field1_vblnk1_4_Q,
      TXCHARISK(6) => mod5_field1_vblnk1_4_Q,
      TXCHARISK(5) => mod5_field1_vblnk1_4_Q,
      TXCHARISK(4) => mod5_field1_vblnk1_4_Q,
      TXCHARISK(3) => mod5_field1_vblnk1_4_Q,
      TXCHARISK(2) => mod5_field1_vblnk1_4_Q,
      TXCHARISK(1) => mod5_field1_vblnk1_4_Q,
      TXCHARISK(0) => mod5_field1_vblnk1_4_Q,
      TXDATA(63) => mod5_field1_vblnk1_4_Q,
      TXDATA(62) => mod5_field1_vblnk1_4_Q,
      TXDATA(61) => mod5_field1_vblnk1_4_Q,
      TXDATA(60) => mod5_field1_vblnk1_4_Q,
      TXDATA(59) => mod5_field1_vblnk1_4_Q,
      TXDATA(58) => mod5_field1_vblnk1_4_Q,
      TXDATA(57) => mod5_field1_vblnk1_4_Q,
      TXDATA(56) => mod5_field1_vblnk1_4_Q,
      TXDATA(55) => mod5_field1_vblnk1_4_Q,
      TXDATA(54) => mod5_field1_vblnk1_4_Q,
      TXDATA(53) => mod5_field1_vblnk1_4_Q,
      TXDATA(52) => mod5_field1_vblnk1_4_Q,
      TXDATA(51) => mod5_field1_vblnk1_4_Q,
      TXDATA(50) => mod5_field1_vblnk1_4_Q,
      TXDATA(49) => mod5_field1_vblnk1_4_Q,
      TXDATA(48) => mod5_field1_vblnk1_4_Q,
      TXDATA(47) => mod5_field1_vblnk1_4_Q,
      TXDATA(46) => mod5_field1_vblnk1_4_Q,
      TXDATA(45) => mod5_field1_vblnk1_4_Q,
      TXDATA(44) => mod5_field1_vblnk1_4_Q,
      TXDATA(43) => mod5_field1_vblnk1_4_Q,
      TXDATA(42) => mod5_field1_vblnk1_4_Q,
      TXDATA(41) => mod5_field1_vblnk1_4_Q,
      TXDATA(40) => mod5_field1_vblnk1_4_Q,
      TXDATA(39) => mod5_field1_vblnk1_4_Q,
      TXDATA(38) => mod5_field1_vblnk1_4_Q,
      TXDATA(37) => mod5_field1_vblnk1_4_Q,
      TXDATA(36) => mod5_field1_vblnk1_4_Q,
      TXDATA(35) => mod5_field1_vblnk1_4_Q,
      TXDATA(34) => mod5_field1_vblnk1_4_Q,
      TXDATA(33) => mod5_field1_vblnk1_4_Q,
      TXDATA(32) => mod5_field1_vblnk1_4_Q,
      TXDATA(31) => mod5_field1_vblnk1_4_Q,
      TXDATA(30) => mod5_field1_vblnk1_4_Q,
      TXDATA(29) => mod5_field1_vblnk1_4_Q,
      TXDATA(28) => mod5_field1_vblnk1_4_Q,
      TXDATA(27) => mod5_field1_vblnk1_4_Q,
      TXDATA(26) => mod5_field1_vblnk1_4_Q,
      TXDATA(25) => mod5_field1_vblnk1_4_Q,
      TXDATA(24) => mod5_field1_vblnk1_4_Q,
      TXDATA(23) => mod5_field1_vblnk1_4_Q,
      TXDATA(22) => mod5_field1_vblnk1_4_Q,
      TXDATA(21) => mod5_field1_vblnk1_4_Q,
      TXDATA(20) => mod5_field1_vblnk1_4_Q,
      TXDATA(19) => mod5_field1_vblnk1_4_Q,
      TXDATA(18) => mod5_field1_vblnk1_4_Q,
      TXDATA(17) => mod5_field1_vblnk1_4_Q,
      TXDATA(16) => mod5_field1_vblnk1_4_Q,
      TXDATA(15) => txda(17),
      TXDATA(14) => txda(16),
      TXDATA(13) => txda(15),
      TXDATA(12) => txda(14),
      TXDATA(11) => txda(13),
      TXDATA(10) => txda(12),
      TXDATA(9) => txda(11),
      TXDATA(8) => txda(10),
      TXDATA(7) => txda(7),
      TXDATA(6) => txda(6),
      TXDATA(5) => txda(5),
      TXDATA(4) => txda(4),
      TXDATA(3) => txda(3),
      TXDATA(2) => txda(2),
      TXDATA(1) => txda(1),
      TXDATA(0) => txda(0),
      TXDATAWIDTH(1) => mod5_field1_vblnk1_4_Q,
      TXDATAWIDTH(0) => mod5_tsl_4_Q,
      TXINTDATAWIDTH(1) => mod5_tsl_4_Q,
      TXINTDATAWIDTH(0) => mod5_tsl_4_Q,
      RXCRCIN(63) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(62) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(61) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(60) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(59) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(58) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(57) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(56) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(55) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(54) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(53) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(52) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(51) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(50) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(49) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(48) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(47) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(46) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(45) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(44) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(43) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(42) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(41) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(40) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(39) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(38) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(37) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(36) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(35) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(34) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(33) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(32) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(31) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(30) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(29) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(28) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(27) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(26) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(25) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(24) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(23) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(22) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(21) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(20) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(19) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(18) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(17) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(16) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(15) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(14) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(13) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(12) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(11) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(10) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(9) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(8) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(7) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(6) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(5) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(4) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(3) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(2) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(1) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(0) => mod5_field1_vblnk1_4_Q,
      RXCRCDATAWIDTH(2) => mod5_field1_vblnk1_4_Q,
      RXCRCDATAWIDTH(1) => mod5_field1_vblnk1_4_Q,
      RXCRCDATAWIDTH(0) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(63) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(62) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(61) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(60) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(59) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(58) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(57) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(56) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(55) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(54) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(53) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(52) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(51) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(50) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(49) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(48) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(47) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(46) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(45) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(44) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(43) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(42) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(41) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(40) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(39) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(38) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(37) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(36) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(35) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(34) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(33) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(32) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(31) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(30) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(29) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(28) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(27) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(26) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(25) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(24) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(23) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(22) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(21) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(20) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(19) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(18) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(17) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(16) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(15) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(14) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(13) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(12) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(11) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(10) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(9) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(8) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(7) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(6) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(5) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(4) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(3) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(2) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(1) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(0) => mod5_field1_vblnk1_4_Q,
      TXCRCDATAWIDTH(2) => mod5_field1_vblnk1_4_Q,
      TXCRCDATAWIDTH(1) => mod5_field1_vblnk1_4_Q,
      TXCRCDATAWIDTH(0) => mod5_field1_vblnk1_4_Q,
      DADDR(7) => mod5_field1_vblnk1_4_Q,
      DADDR(6) => mod5_field1_vblnk1_4_Q,
      DADDR(5) => mod5_field1_vblnk1_4_Q,
      DADDR(4) => mod5_field1_vblnk1_4_Q,
      DADDR(3) => mod5_field1_vblnk1_4_Q,
      DADDR(2) => mod5_field1_vblnk1_4_Q,
      DADDR(1) => mod5_field1_vblnk1_4_Q,
      DADDR(0) => mod5_field1_vblnk1_4_Q,
      DI(15) => mod5_field1_vblnk1_4_Q,
      DI(14) => mod5_field1_vblnk1_4_Q,
      DI(13) => mod5_field1_vblnk1_4_Q,
      DI(12) => mod5_field1_vblnk1_4_Q,
      DI(11) => mod5_field1_vblnk1_4_Q,
      DI(10) => mod5_field1_vblnk1_4_Q,
      DI(9) => mod5_field1_vblnk1_4_Q,
      DI(8) => mod5_field1_vblnk1_4_Q,
      DI(7) => mod5_field1_vblnk1_4_Q,
      DI(6) => mod5_field1_vblnk1_4_Q,
      DI(5) => mod5_field1_vblnk1_4_Q,
      DI(4) => mod5_field1_vblnk1_4_Q,
      DI(3) => mod5_field1_vblnk1_4_Q,
      DI(2) => mod5_field1_vblnk1_4_Q,
      DI(1) => mod5_field1_vblnk1_4_Q,
      DI(0) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(15) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(14) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(13) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(12) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(11) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(10) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(9) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(8) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(7) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(6) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(5) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(4) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(3) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(2) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(1) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(0) => mod5_field1_vblnk1_4_Q,
      CHBONDO(4) => NLW_mgt113a_GT11_CUSTOM_INST_CHBONDO_4_UNCONNECTED,
      CHBONDO(3) => NLW_mgt113a_GT11_CUSTOM_INST_CHBONDO_3_UNCONNECTED,
      CHBONDO(2) => NLW_mgt113a_GT11_CUSTOM_INST_CHBONDO_2_UNCONNECTED,
      CHBONDO(1) => NLW_mgt113a_GT11_CUSTOM_INST_CHBONDO_1_UNCONNECTED,
      CHBONDO(0) => NLW_mgt113a_GT11_CUSTOM_INST_CHBONDO_0_UNCONNECTED,
      RXSTATUS(5) => NLW_mgt113a_GT11_CUSTOM_INST_RXSTATUS_5_UNCONNECTED,
      RXSTATUS(4) => NLW_mgt113a_GT11_CUSTOM_INST_RXSTATUS_4_UNCONNECTED,
      RXSTATUS(3) => NLW_mgt113a_GT11_CUSTOM_INST_RXSTATUS_3_UNCONNECTED,
      RXSTATUS(2) => NLW_mgt113a_GT11_CUSTOM_INST_RXSTATUS_2_UNCONNECTED,
      RXSTATUS(1) => NLW_mgt113a_GT11_CUSTOM_INST_RXSTATUS_1_UNCONNECTED,
      RXSTATUS(0) => NLW_mgt113a_GT11_CUSTOM_INST_RXSTATUS_0_UNCONNECTED,
      RXCHARISCOMMA(7) => NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISCOMMA_7_UNCONNECTED,
      RXCHARISCOMMA(6) => NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISCOMMA_6_UNCONNECTED,
      RXCHARISCOMMA(5) => NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISCOMMA_5_UNCONNECTED,
      RXCHARISCOMMA(4) => NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISCOMMA_4_UNCONNECTED,
      RXCHARISCOMMA(3) => NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISCOMMA_3_UNCONNECTED,
      RXCHARISCOMMA(2) => NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISCOMMA_2_UNCONNECTED,
      RXCHARISCOMMA(1) => NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISCOMMA_1_UNCONNECTED,
      RXCHARISCOMMA(0) => NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISCOMMA_0_UNCONNECTED,
      RXCHARISK(7) => NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISK_7_UNCONNECTED,
      RXCHARISK(6) => NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISK_6_UNCONNECTED,
      RXCHARISK(5) => NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISK_5_UNCONNECTED,
      RXCHARISK(4) => NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISK_4_UNCONNECTED,
      RXCHARISK(3) => NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISK_3_UNCONNECTED,
      RXCHARISK(2) => NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISK_2_UNCONNECTED,
      RXCHARISK(1) => NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISK_1_UNCONNECTED,
      RXCHARISK(0) => NLW_mgt113a_GT11_CUSTOM_INST_RXCHARISK_0_UNCONNECTED,
      RXDATA(63) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_63_UNCONNECTED,
      RXDATA(62) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_62_UNCONNECTED,
      RXDATA(61) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_61_UNCONNECTED,
      RXDATA(60) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_60_UNCONNECTED,
      RXDATA(59) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_59_UNCONNECTED,
      RXDATA(58) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_58_UNCONNECTED,
      RXDATA(57) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_57_UNCONNECTED,
      RXDATA(56) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_56_UNCONNECTED,
      RXDATA(55) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_55_UNCONNECTED,
      RXDATA(54) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_54_UNCONNECTED,
      RXDATA(53) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_53_UNCONNECTED,
      RXDATA(52) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_52_UNCONNECTED,
      RXDATA(51) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_51_UNCONNECTED,
      RXDATA(50) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_50_UNCONNECTED,
      RXDATA(49) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_49_UNCONNECTED,
      RXDATA(48) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_48_UNCONNECTED,
      RXDATA(47) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_47_UNCONNECTED,
      RXDATA(46) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_46_UNCONNECTED,
      RXDATA(45) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_45_UNCONNECTED,
      RXDATA(44) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_44_UNCONNECTED,
      RXDATA(43) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_43_UNCONNECTED,
      RXDATA(42) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_42_UNCONNECTED,
      RXDATA(41) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_41_UNCONNECTED,
      RXDATA(40) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_40_UNCONNECTED,
      RXDATA(39) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_39_UNCONNECTED,
      RXDATA(38) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_38_UNCONNECTED,
      RXDATA(37) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_37_UNCONNECTED,
      RXDATA(36) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_36_UNCONNECTED,
      RXDATA(35) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_35_UNCONNECTED,
      RXDATA(34) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_34_UNCONNECTED,
      RXDATA(33) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_33_UNCONNECTED,
      RXDATA(32) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_32_UNCONNECTED,
      RXDATA(31) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_31_UNCONNECTED,
      RXDATA(30) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_30_UNCONNECTED,
      RXDATA(29) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_29_UNCONNECTED,
      RXDATA(28) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_28_UNCONNECTED,
      RXDATA(27) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_27_UNCONNECTED,
      RXDATA(26) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_26_UNCONNECTED,
      RXDATA(25) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_25_UNCONNECTED,
      RXDATA(24) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_24_UNCONNECTED,
      RXDATA(23) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_23_UNCONNECTED,
      RXDATA(22) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_22_UNCONNECTED,
      RXDATA(21) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_21_UNCONNECTED,
      RXDATA(20) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_20_UNCONNECTED,
      RXDATA(19) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_19_UNCONNECTED,
      RXDATA(18) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_18_UNCONNECTED,
      RXDATA(17) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_17_UNCONNECTED,
      RXDATA(16) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_16_UNCONNECTED,
      RXDATA(15) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_15_UNCONNECTED,
      RXDATA(14) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_14_UNCONNECTED,
      RXDATA(13) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_13_UNCONNECTED,
      RXDATA(12) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_12_UNCONNECTED,
      RXDATA(11) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_11_UNCONNECTED,
      RXDATA(10) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_10_UNCONNECTED,
      RXDATA(9) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_9_UNCONNECTED,
      RXDATA(8) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_8_UNCONNECTED,
      RXDATA(7) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_7_UNCONNECTED,
      RXDATA(6) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_6_UNCONNECTED,
      RXDATA(5) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_5_UNCONNECTED,
      RXDATA(4) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_4_UNCONNECTED,
      RXDATA(3) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_3_UNCONNECTED,
      RXDATA(2) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_2_UNCONNECTED,
      RXDATA(1) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_1_UNCONNECTED,
      RXDATA(0) => NLW_mgt113a_GT11_CUSTOM_INST_RXDATA_0_UNCONNECTED,
      RXDISPERR(7) => NLW_mgt113a_GT11_CUSTOM_INST_RXDISPERR_7_UNCONNECTED,
      RXDISPERR(6) => NLW_mgt113a_GT11_CUSTOM_INST_RXDISPERR_6_UNCONNECTED,
      RXDISPERR(5) => NLW_mgt113a_GT11_CUSTOM_INST_RXDISPERR_5_UNCONNECTED,
      RXDISPERR(4) => NLW_mgt113a_GT11_CUSTOM_INST_RXDISPERR_4_UNCONNECTED,
      RXDISPERR(3) => NLW_mgt113a_GT11_CUSTOM_INST_RXDISPERR_3_UNCONNECTED,
      RXDISPERR(2) => NLW_mgt113a_GT11_CUSTOM_INST_RXDISPERR_2_UNCONNECTED,
      RXDISPERR(1) => NLW_mgt113a_GT11_CUSTOM_INST_RXDISPERR_1_UNCONNECTED,
      RXDISPERR(0) => NLW_mgt113a_GT11_CUSTOM_INST_RXDISPERR_0_UNCONNECTED,
      RXLOSSOFSYNC(1) => NLW_mgt113a_GT11_CUSTOM_INST_RXLOSSOFSYNC_1_UNCONNECTED,
      RXLOSSOFSYNC(0) => NLW_mgt113a_GT11_CUSTOM_INST_RXLOSSOFSYNC_0_UNCONNECTED,
      RXNOTINTABLE(7) => NLW_mgt113a_GT11_CUSTOM_INST_RXNOTINTABLE_7_UNCONNECTED,
      RXNOTINTABLE(6) => NLW_mgt113a_GT11_CUSTOM_INST_RXNOTINTABLE_6_UNCONNECTED,
      RXNOTINTABLE(5) => NLW_mgt113a_GT11_CUSTOM_INST_RXNOTINTABLE_5_UNCONNECTED,
      RXNOTINTABLE(4) => NLW_mgt113a_GT11_CUSTOM_INST_RXNOTINTABLE_4_UNCONNECTED,
      RXNOTINTABLE(3) => NLW_mgt113a_GT11_CUSTOM_INST_RXNOTINTABLE_3_UNCONNECTED,
      RXNOTINTABLE(2) => NLW_mgt113a_GT11_CUSTOM_INST_RXNOTINTABLE_2_UNCONNECTED,
      RXNOTINTABLE(1) => NLW_mgt113a_GT11_CUSTOM_INST_RXNOTINTABLE_1_UNCONNECTED,
      RXNOTINTABLE(0) => NLW_mgt113a_GT11_CUSTOM_INST_RXNOTINTABLE_0_UNCONNECTED,
      RXRUNDISP(7) => NLW_mgt113a_GT11_CUSTOM_INST_RXRUNDISP_7_UNCONNECTED,
      RXRUNDISP(6) => NLW_mgt113a_GT11_CUSTOM_INST_RXRUNDISP_6_UNCONNECTED,
      RXRUNDISP(5) => NLW_mgt113a_GT11_CUSTOM_INST_RXRUNDISP_5_UNCONNECTED,
      RXRUNDISP(4) => NLW_mgt113a_GT11_CUSTOM_INST_RXRUNDISP_4_UNCONNECTED,
      RXRUNDISP(3) => NLW_mgt113a_GT11_CUSTOM_INST_RXRUNDISP_3_UNCONNECTED,
      RXRUNDISP(2) => NLW_mgt113a_GT11_CUSTOM_INST_RXRUNDISP_2_UNCONNECTED,
      RXRUNDISP(1) => NLW_mgt113a_GT11_CUSTOM_INST_RXRUNDISP_1_UNCONNECTED,
      RXRUNDISP(0) => NLW_mgt113a_GT11_CUSTOM_INST_RXRUNDISP_0_UNCONNECTED,
      TXKERR(7) => NLW_mgt113a_GT11_CUSTOM_INST_TXKERR_7_UNCONNECTED,
      TXKERR(6) => NLW_mgt113a_GT11_CUSTOM_INST_TXKERR_6_UNCONNECTED,
      TXKERR(5) => NLW_mgt113a_GT11_CUSTOM_INST_TXKERR_5_UNCONNECTED,
      TXKERR(4) => NLW_mgt113a_GT11_CUSTOM_INST_TXKERR_4_UNCONNECTED,
      TXKERR(3) => NLW_mgt113a_GT11_CUSTOM_INST_TXKERR_3_UNCONNECTED,
      TXKERR(2) => NLW_mgt113a_GT11_CUSTOM_INST_TXKERR_2_UNCONNECTED,
      TXKERR(1) => NLW_mgt113a_GT11_CUSTOM_INST_TXKERR_1_UNCONNECTED,
      TXKERR(0) => NLW_mgt113a_GT11_CUSTOM_INST_TXKERR_0_UNCONNECTED,
      TXRUNDISP(7) => NLW_mgt113a_GT11_CUSTOM_INST_TXRUNDISP_7_UNCONNECTED,
      TXRUNDISP(6) => NLW_mgt113a_GT11_CUSTOM_INST_TXRUNDISP_6_UNCONNECTED,
      TXRUNDISP(5) => NLW_mgt113a_GT11_CUSTOM_INST_TXRUNDISP_5_UNCONNECTED,
      TXRUNDISP(4) => NLW_mgt113a_GT11_CUSTOM_INST_TXRUNDISP_4_UNCONNECTED,
      TXRUNDISP(3) => NLW_mgt113a_GT11_CUSTOM_INST_TXRUNDISP_3_UNCONNECTED,
      TXRUNDISP(2) => NLW_mgt113a_GT11_CUSTOM_INST_TXRUNDISP_2_UNCONNECTED,
      TXRUNDISP(1) => NLW_mgt113a_GT11_CUSTOM_INST_TXRUNDISP_1_UNCONNECTED,
      TXRUNDISP(0) => NLW_mgt113a_GT11_CUSTOM_INST_TXRUNDISP_0_UNCONNECTED,
      RXCRCOUT(31) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_31_UNCONNECTED,
      RXCRCOUT(30) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_30_UNCONNECTED,
      RXCRCOUT(29) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_29_UNCONNECTED,
      RXCRCOUT(28) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_28_UNCONNECTED,
      RXCRCOUT(27) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_27_UNCONNECTED,
      RXCRCOUT(26) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_26_UNCONNECTED,
      RXCRCOUT(25) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_25_UNCONNECTED,
      RXCRCOUT(24) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_24_UNCONNECTED,
      RXCRCOUT(23) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_23_UNCONNECTED,
      RXCRCOUT(22) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_22_UNCONNECTED,
      RXCRCOUT(21) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_21_UNCONNECTED,
      RXCRCOUT(20) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_20_UNCONNECTED,
      RXCRCOUT(19) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_19_UNCONNECTED,
      RXCRCOUT(18) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_18_UNCONNECTED,
      RXCRCOUT(17) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_17_UNCONNECTED,
      RXCRCOUT(16) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_16_UNCONNECTED,
      RXCRCOUT(15) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_15_UNCONNECTED,
      RXCRCOUT(14) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_14_UNCONNECTED,
      RXCRCOUT(13) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_13_UNCONNECTED,
      RXCRCOUT(12) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_12_UNCONNECTED,
      RXCRCOUT(11) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_11_UNCONNECTED,
      RXCRCOUT(10) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_10_UNCONNECTED,
      RXCRCOUT(9) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_9_UNCONNECTED,
      RXCRCOUT(8) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_8_UNCONNECTED,
      RXCRCOUT(7) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_7_UNCONNECTED,
      RXCRCOUT(6) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_6_UNCONNECTED,
      RXCRCOUT(5) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_5_UNCONNECTED,
      RXCRCOUT(4) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_4_UNCONNECTED,
      RXCRCOUT(3) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_3_UNCONNECTED,
      RXCRCOUT(2) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_2_UNCONNECTED,
      RXCRCOUT(1) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_1_UNCONNECTED,
      RXCRCOUT(0) => NLW_mgt113a_GT11_CUSTOM_INST_RXCRCOUT_0_UNCONNECTED,
      TXCRCOUT(31) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_31_UNCONNECTED,
      TXCRCOUT(30) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_30_UNCONNECTED,
      TXCRCOUT(29) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_29_UNCONNECTED,
      TXCRCOUT(28) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_28_UNCONNECTED,
      TXCRCOUT(27) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_27_UNCONNECTED,
      TXCRCOUT(26) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_26_UNCONNECTED,
      TXCRCOUT(25) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_25_UNCONNECTED,
      TXCRCOUT(24) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_24_UNCONNECTED,
      TXCRCOUT(23) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_23_UNCONNECTED,
      TXCRCOUT(22) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_22_UNCONNECTED,
      TXCRCOUT(21) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_21_UNCONNECTED,
      TXCRCOUT(20) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_20_UNCONNECTED,
      TXCRCOUT(19) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_19_UNCONNECTED,
      TXCRCOUT(18) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_18_UNCONNECTED,
      TXCRCOUT(17) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_17_UNCONNECTED,
      TXCRCOUT(16) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_16_UNCONNECTED,
      TXCRCOUT(15) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_15_UNCONNECTED,
      TXCRCOUT(14) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_14_UNCONNECTED,
      TXCRCOUT(13) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_13_UNCONNECTED,
      TXCRCOUT(12) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_12_UNCONNECTED,
      TXCRCOUT(11) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_11_UNCONNECTED,
      TXCRCOUT(10) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_10_UNCONNECTED,
      TXCRCOUT(9) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_9_UNCONNECTED,
      TXCRCOUT(8) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_8_UNCONNECTED,
      TXCRCOUT(7) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_7_UNCONNECTED,
      TXCRCOUT(6) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_6_UNCONNECTED,
      TXCRCOUT(5) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_5_UNCONNECTED,
      TXCRCOUT(4) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_4_UNCONNECTED,
      TXCRCOUT(3) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_3_UNCONNECTED,
      TXCRCOUT(2) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_2_UNCONNECTED,
      TXCRCOUT(1) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_1_UNCONNECTED,
      TXCRCOUT(0) => NLW_mgt113a_GT11_CUSTOM_INST_TXCRCOUT_0_UNCONNECTED,
      DO(15) => NLW_mgt113a_GT11_CUSTOM_INST_DO_15_UNCONNECTED,
      DO(14) => NLW_mgt113a_GT11_CUSTOM_INST_DO_14_UNCONNECTED,
      DO(13) => NLW_mgt113a_GT11_CUSTOM_INST_DO_13_UNCONNECTED,
      DO(12) => NLW_mgt113a_GT11_CUSTOM_INST_DO_12_UNCONNECTED,
      DO(11) => NLW_mgt113a_GT11_CUSTOM_INST_DO_11_UNCONNECTED,
      DO(10) => NLW_mgt113a_GT11_CUSTOM_INST_DO_10_UNCONNECTED,
      DO(9) => NLW_mgt113a_GT11_CUSTOM_INST_DO_9_UNCONNECTED,
      DO(8) => NLW_mgt113a_GT11_CUSTOM_INST_DO_8_UNCONNECTED,
      DO(7) => NLW_mgt113a_GT11_CUSTOM_INST_DO_7_UNCONNECTED,
      DO(6) => NLW_mgt113a_GT11_CUSTOM_INST_DO_6_UNCONNECTED,
      DO(5) => NLW_mgt113a_GT11_CUSTOM_INST_DO_5_UNCONNECTED,
      DO(4) => NLW_mgt113a_GT11_CUSTOM_INST_DO_4_UNCONNECTED,
      DO(3) => NLW_mgt113a_GT11_CUSTOM_INST_DO_3_UNCONNECTED,
      DO(2) => NLW_mgt113a_GT11_CUSTOM_INST_DO_2_UNCONNECTED,
      DO(1) => NLW_mgt113a_GT11_CUSTOM_INST_DO_1_UNCONNECTED,
      DO(0) => NLW_mgt113a_GT11_CUSTOM_INST_DO_0_UNCONNECTED,
      COMBUSOUT(15) => NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_15_UNCONNECTED,
      COMBUSOUT(14) => NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_14_UNCONNECTED,
      COMBUSOUT(13) => NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_13_UNCONNECTED,
      COMBUSOUT(12) => NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_12_UNCONNECTED,
      COMBUSOUT(11) => NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_11_UNCONNECTED,
      COMBUSOUT(10) => NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_10_UNCONNECTED,
      COMBUSOUT(9) => NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_9_UNCONNECTED,
      COMBUSOUT(8) => NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_8_UNCONNECTED,
      COMBUSOUT(7) => NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_7_UNCONNECTED,
      COMBUSOUT(6) => NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_6_UNCONNECTED,
      COMBUSOUT(5) => NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_5_UNCONNECTED,
      COMBUSOUT(4) => NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_4_UNCONNECTED,
      COMBUSOUT(3) => NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_3_UNCONNECTED,
      COMBUSOUT(2) => NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_2_UNCONNECTED,
      COMBUSOUT(1) => NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_1_UNCONNECTED,
      COMBUSOUT(0) => NLW_mgt113a_GT11_CUSTOM_INST_COMBUSOUT_0_UNCONNECTED
    );
  mgt113b_GT11_CUSTOM_INST : GT11
    generic map(
      ALIGN_COMMA_WORD => 1,
      BANDGAPSEL => FALSE,
      BIASRESSEL => FALSE,
      CCCB_ARBITRATOR_DISABLE => FALSE,
      CHAN_BOND_LIMIT => 16,
      CHAN_BOND_MODE => "NONE",
      CHAN_BOND_ONE_SHOT => FALSE,
      CHAN_BOND_SEQ_1_1 => "00000000000",
      CHAN_BOND_SEQ_1_2 => "00000000000",
      CHAN_BOND_SEQ_1_3 => "00000000000",
      CHAN_BOND_SEQ_1_4 => "00000000000",
      CHAN_BOND_SEQ_1_MASK => "1110",
      CHAN_BOND_SEQ_2_1 => "00000000000",
      CHAN_BOND_SEQ_2_2 => "00000000000",
      CHAN_BOND_SEQ_2_3 => "00000000000",
      CHAN_BOND_SEQ_2_USE => FALSE,
      CHAN_BOND_SEQ_LEN => 1,
      CLK_CORRECT_USE => FALSE,
      CLK_COR_8B10B_DE => TRUE,
      CLK_COR_MAX_LAT => 44,
      CLK_COR_MIN_LAT => 36,
      CHAN_BOND_SEQ_2_MASK => "1110",
      CLK_COR_SEQ_1_1 => "00010111100",
      CLK_COR_SEQ_1_2 => "00010111100",
      CLK_COR_SEQ_1_3 => "00010111100",
      TXDATA_SEL => "00",
      RXDATA_SEL => "00",
      CLK_COR_SEQ_2_1 => "00010111100",
      CLK_COR_SEQ_2_2 => "00010111100",
      CLK_COR_SEQ_2_3 => "00010111100",
      CLK_COR_SEQ_2_USE => FALSE,
      CLK_COR_SEQ_DROP => FALSE,
      CLK_COR_SEQ_LEN => 1,
      COMMA32 => FALSE,
      RXSLOWDOWN_CAL => "00",
      DEC_MCOMMA_DETECT => FALSE,
      DEC_PCOMMA_DETECT => FALSE,
      DEC_VALID_COMMA_ONLY => FALSE,
      CLK_COR_SEQ_1_4 => "00010111100",
      DIGRX_SYNC_MODE => FALSE,
      ENABLE_DCDR => FALSE,
      FDET_HYS_SEL => "100",
      CYCLE_LIMIT_SEL => "00",
      FDET_LCK_SEL => "001",
      RXVCODAC_INIT => "0001000001",
      GT11_MODE => "SINGLE",
      PMAVBGCTRL => "00000",
      FDET_LCK_CAL => "100",
      COMMA_10B_MASK => X"3FF",
      MCOMMA_DETECT => FALSE,
      OPPOSITE_SELECT => FALSE,
      MCOMMA_32B_VALUE => X"000000F6",
      PCOMMA_DETECT => FALSE,
      PCS_BIT_SLIP => FALSE,
      PMACLKENABLE => TRUE,
      PMACOREPWRENABLE => TRUE,
      PMAIREFTRIM => "0111",
      PMA_BIT_SLIP => FALSE,
      POWER_ENABLE => TRUE,
      REPEATER => FALSE,
      RXACTST => FALSE,
      RXRCPADJ => "010",
      RXAFEPD => FALSE,
      RXAFETST => FALSE,
      RXAPD => FALSE,
      RXBY_32 => TRUE,
      RXAFEEQ => "000000000",
      RXCLK0_FORCE_PMACLK => TRUE,
      RXRIBADJ => "11",
      RXCPSEL => TRUE,
      RXCPTST => FALSE,
      RXCRCCLOCKDOUBLE => FALSE,
      RXCRCENABLE => FALSE,
      SLOWDOWN_CAL => "00",
      RXCRCINVERTGEN => FALSE,
      RXCRCSAMECLOCK => FALSE,
      RXLOOPFILT => "1111",
      PCOMMA_32B_VALUE => X"F6F62828",
      DIGRX_FWDCLK => "10",
      RXDCCOUPLE => FALSE,
      RXDIGRESET => FALSE,
      RXDIGRX => FALSE,
      RXFECONTROL2 => "000",
      RXFDCAL_CLOCK_DIVIDE => "TWO",
      RXFDET_HYS_SEL => "001",
      RXCYCLE_LIMIT_SEL => "00",
      RXFDET_LCK_SEL => "111",
      DCDR_FILTER => "000",
      RXLKADJ => "00000",
      RXCDRLOS => "000000",
      RXASYNCDIVIDE => "00",
      RXLB => FALSE,
      RXCMADJ => "10",
      RXLKAPD => FALSE,
      RXFDET_LCK_CAL => "101",
      RXOUTDIV2SEL => 2,
      RXPD => FALSE,
      RXPDDTST => TRUE,
      RXPLLNDIVSEL => 8,
      RXPMACLKSEL => "GREFCLK",
      RXCLKMODE => "100111",
      RXRCPPD => FALSE,
      RXRECCLK1_USE_SYNC => FALSE,
      RXFETUNE => "01",
      RXRPDPD => FALSE,
      RXRSDPD => FALSE,
      RXFDET_HYS_CAL => "010",
      RXUSRDIVISOR => 1,
      RXLOOPCAL_WAIT => "00",
      RXVCO_CTRL_ENABLE => TRUE,
      RX_BUFFER_USE => TRUE,
      RXCRCINITVAL => X"FFFFFFFF",
      SAMPLE_8X => TRUE,
      SH_CNT_MAX => 64,
      SH_INVALID_CNT_MAX => 16,
      FDET_HYS_CAL => "010",
      TXABPMACLKSEL => "GREFCLK",
      TXAPD => FALSE,
      TXAREFBIASSEL => TRUE,
      TXCLKMODE => "0100",
      TXCLK0_FORCE_PMACLK => TRUE,
      TXPRE_PRDRV_DAC => "111",
      TXCPSEL => TRUE,
      TXCRCCLOCKDOUBLE => FALSE,
      TXCRCENABLE => FALSE,
      VCODAC_INIT => "0000000000",
      TXCRCINVERTGEN => FALSE,
      TXCRCSAMECLOCK => FALSE,
      TXLOOPFILT => "1111",
      CLK_COR_SEQ_1_MASK => "0000",
      TXASYNCDIVIDE => "01",
      TXPOST_TAP_DAC => "00111",
      TXDIGPD => FALSE,
      TXFDCAL_CLOCK_DIVIDE => "NONE",
      TXHIGHSIGNALEN => TRUE,
      RXEQ => X"4000000000000000",
      TXLVLSHFTPD => FALSE,
      TXOUTCLK1_USE_SYNC => FALSE,
      TXOUTDIV2SEL => 4,
      TXPD => FALSE,
      TXPHASESEL => FALSE,
      TXPLLNDIVSEL => 40,
      TXTERMTRIM => "1100",
      TXPOST_PRDRV_DAC => "000",
      TXPOST_TAP_PD => TRUE,
      TXCTRL1 => X"200",
      TXPRE_TAP_PD => TRUE,
      TXSLEWRATE => FALSE,
      TXPRE_TAP_DAC => "00000",
      TX_BUFFER_USE => TRUE,
      TXCRCINITVAL => X"FFFFFFFF",
      LOOPCAL_WAIT => "00",
      VCO_CTRL_ENABLE => TRUE,
      IREFBIASMODE => "11",
      CHAN_BOND_SEQ_2_4 => "00000000000",
      CLK_COR_SEQ_2_4 => "00010111100",
      CLK_COR_SEQ_2_MASK => "0000",
      RX_CLOCK_DIVIDER => "10",
      TX_CLOCK_DIVIDER => "00",
      PMAVREFTRIM => "0111",
      VREFBIASMODE => "11",
      RXFECONTROL1 => "00",
      TXDAT_PRDRV_DAC => "111",
      TXDAT_TAP_DAC => "10000",
      RXCTRL1 => X"200"
    )
    port map (
      ENCHANSYNC => mod5_field1_vblnk1_4_Q,
      ENMCOMMAALIGN => mod5_tsl_4_Q,
      ENPCOMMAALIGN => mod5_tsl_4_Q,
      POWERDOWN => mod5_field1_vblnk1_4_Q,
      RXBLOCKSYNC64B66BUSE => mod5_field1_vblnk1_4_Q,
      RXCOMMADETUSE => mod5_field1_vblnk1_4_Q,
      RXDEC64B66BUSE => mod5_field1_vblnk1_4_Q,
      RXDEC8B10BUSE => mod5_field1_vblnk1_4_Q,
      RXDESCRAM64B66BUSE => mod5_field1_vblnk1_4_Q,
      RXIGNOREBTF => mod5_field1_vblnk1_4_Q,
      RXPOLARITY => mod5_field1_vblnk1_4_Q,
      RXRESET => mod5_field1_vblnk1_4_Q,
      RXSLIDE => mod5_field1_vblnk1_4_Q,
      RXUSRCLK => mod5_field1_vblnk1_4_Q,
      RXUSRCLK2 => TXRIOA_USRCLK,
      TXENC64B66BUSE => mod5_field1_vblnk1_4_Q,
      TXENC8B10BUSE => mod5_field1_vblnk1_4_Q,
      TXGEARBOX64B66BUSE => mod5_field1_vblnk1_4_Q,
      TXINHIBIT => mod5_field1_vblnk1_4_Q,
      TXPOLARITY => mod5_field1_vblnk1_4_Q,
      TXRESET => mod5_field1_vblnk1_4_Q,
      TXSCRAM64B66BUSE => mod5_field1_vblnk1_4_Q,
      TXUSRCLK => TXRIOA_USRCLK,
      TXUSRCLK2 => TXRIOA_USRCLK,
      RXCLKSTABLE => mod5_tsl_4_Q,
      RXPMARESET => mod5_field1_vblnk1_4_Q,
      TXCLKSTABLE => mod5_tsl_4_Q,
      TXPMARESET => mod5_field1_vblnk1_4_Q,
      RXCRCDATAVALID => mod5_field1_vblnk1_4_Q,
      RXCRCINIT => mod5_field1_vblnk1_4_Q,
      RXCRCRESET => mod5_tsl_4_Q,
      RXCRCPD => mod5_field1_vblnk1_4_Q,
      RXCRCCLK => mod5_field1_vblnk1_4_Q,
      RXCRCINTCLK => mod5_field1_vblnk1_4_Q,
      TXCRCDATAVALID => mod5_field1_vblnk1_4_Q,
      TXCRCINIT => mod5_field1_vblnk1_4_Q,
      TXCRCRESET => mod5_tsl_4_Q,
      TXCRCPD => mod5_field1_vblnk1_4_Q,
      TXCRCCLK => mod5_field1_vblnk1_4_Q,
      TXCRCINTCLK => mod5_field1_vblnk1_4_Q,
      TXSYNC => mod5_field1_vblnk1_4_Q,
      RXSYNC => mod5_field1_vblnk1_4_Q,
      TXENOOB => mod5_field1_vblnk1_4_Q,
      DCLK => mod5_field1_vblnk1_4_Q,
      DEN => mod5_field1_vblnk1_4_Q,
      DWE => mod5_field1_vblnk1_4_Q,
      RX1P => mod5_field1_vblnk1_4_Q,
      RX1N => mod5_field1_vblnk1_4_Q,
      GREFCLK => TXRIOA_REFCLK,
      REFCLK1 => mod5_field1_vblnk1_4_Q,
      REFCLK2 => mod5_field1_vblnk1_4_Q,
      RXCOMMADET => NLW_mgt113b_GT11_CUSTOM_INST_RXCOMMADET_UNCONNECTED,
      RXREALIGN => NLW_mgt113b_GT11_CUSTOM_INST_RXREALIGN_UNCONNECTED,
      RXBUFERR => NLW_mgt113b_GT11_CUSTOM_INST_RXBUFERR_UNCONNECTED,
      TXBUFERR => NLW_mgt113b_GT11_CUSTOM_INST_TXBUFERR_UNCONNECTED,
      RXRECCLK1 => NLW_mgt113b_GT11_CUSTOM_INST_RXRECCLK1_UNCONNECTED,
      RXRECCLK2 => NLW_mgt113b_GT11_CUSTOM_INST_RXRECCLK2_UNCONNECTED,
      TXOUTCLK1 => NLW_mgt113b_GT11_CUSTOM_INST_TXOUTCLK1_UNCONNECTED,
      TXOUTCLK2 => NLW_mgt113b_GT11_CUSTOM_INST_TXOUTCLK2_UNCONNECTED,
      RXLOCK => NLW_mgt113b_GT11_CUSTOM_INST_RXLOCK_UNCONNECTED,
      TXLOCK => NLW_mgt113b_GT11_CUSTOM_INST_TXLOCK_UNCONNECTED,
      RXCYCLELIMIT => NLW_mgt113b_GT11_CUSTOM_INST_RXCYCLELIMIT_UNCONNECTED,
      TXCYCLELIMIT => NLW_mgt113b_GT11_CUSTOM_INST_TXCYCLELIMIT_UNCONNECTED,
      RXCALFAIL => NLW_mgt113b_GT11_CUSTOM_INST_RXCALFAIL_UNCONNECTED,
      TXCALFAIL => NLW_mgt113b_GT11_CUSTOM_INST_TXCALFAIL_UNCONNECTED,
      RXSIGDET => NLW_mgt113b_GT11_CUSTOM_INST_RXSIGDET_UNCONNECTED,
      DRDY => NLW_mgt113b_GT11_CUSTOM_INST_DRDY_UNCONNECTED,
      RXMCLK => NLW_mgt113b_GT11_CUSTOM_INST_RXMCLK_UNCONNECTED,
      TX1P => TXp_OUT_113b,
      TX1N => TXn_OUT_113b,
      TXPCSHCLKOUT => NLW_mgt113b_GT11_CUSTOM_INST_TXPCSHCLKOUT_UNCONNECTED,
      RXPCSHCLKOUT => NLW_mgt113b_GT11_CUSTOM_INST_RXPCSHCLKOUT_UNCONNECTED,
      CHBONDI(4) => mod5_field1_vblnk1_4_Q,
      CHBONDI(3) => mod5_field1_vblnk1_4_Q,
      CHBONDI(2) => mod5_field1_vblnk1_4_Q,
      CHBONDI(1) => mod5_field1_vblnk1_4_Q,
      CHBONDI(0) => mod5_field1_vblnk1_4_Q,
      LOOPBACK(1) => mod5_field1_vblnk1_4_Q,
      LOOPBACK(0) => mod5_field1_vblnk1_4_Q,
      RXDATAWIDTH(1) => mod5_field1_vblnk1_4_Q,
      RXDATAWIDTH(0) => mod5_field1_vblnk1_4_Q,
      RXINTDATAWIDTH(1) => mod5_tsl_4_Q,
      RXINTDATAWIDTH(0) => mod5_field1_vblnk1_4_Q,
      TXBYPASS8B10B(7) => mod5_tsl_4_Q,
      TXBYPASS8B10B(6) => mod5_tsl_4_Q,
      TXBYPASS8B10B(5) => mod5_tsl_4_Q,
      TXBYPASS8B10B(4) => mod5_tsl_4_Q,
      TXBYPASS8B10B(3) => mod5_tsl_4_Q,
      TXBYPASS8B10B(2) => mod5_tsl_4_Q,
      TXBYPASS8B10B(1) => mod5_tsl_4_Q,
      TXBYPASS8B10B(0) => mod5_tsl_4_Q,
      TXCHARDISPMODE(7) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPMODE(6) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPMODE(5) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPMODE(4) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPMODE(3) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPMODE(2) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPMODE(1) => txda(19),
      TXCHARDISPMODE(0) => txda(9),
      TXCHARDISPVAL(7) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPVAL(6) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPVAL(5) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPVAL(4) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPVAL(3) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPVAL(2) => mod5_field1_vblnk1_4_Q,
      TXCHARDISPVAL(1) => txda(18),
      TXCHARDISPVAL(0) => txda(8),
      TXCHARISK(7) => mod5_field1_vblnk1_4_Q,
      TXCHARISK(6) => mod5_field1_vblnk1_4_Q,
      TXCHARISK(5) => mod5_field1_vblnk1_4_Q,
      TXCHARISK(4) => mod5_field1_vblnk1_4_Q,
      TXCHARISK(3) => mod5_field1_vblnk1_4_Q,
      TXCHARISK(2) => mod5_field1_vblnk1_4_Q,
      TXCHARISK(1) => mod5_field1_vblnk1_4_Q,
      TXCHARISK(0) => mod5_field1_vblnk1_4_Q,
      TXDATA(63) => mod5_field1_vblnk1_4_Q,
      TXDATA(62) => mod5_field1_vblnk1_4_Q,
      TXDATA(61) => mod5_field1_vblnk1_4_Q,
      TXDATA(60) => mod5_field1_vblnk1_4_Q,
      TXDATA(59) => mod5_field1_vblnk1_4_Q,
      TXDATA(58) => mod5_field1_vblnk1_4_Q,
      TXDATA(57) => mod5_field1_vblnk1_4_Q,
      TXDATA(56) => mod5_field1_vblnk1_4_Q,
      TXDATA(55) => mod5_field1_vblnk1_4_Q,
      TXDATA(54) => mod5_field1_vblnk1_4_Q,
      TXDATA(53) => mod5_field1_vblnk1_4_Q,
      TXDATA(52) => mod5_field1_vblnk1_4_Q,
      TXDATA(51) => mod5_field1_vblnk1_4_Q,
      TXDATA(50) => mod5_field1_vblnk1_4_Q,
      TXDATA(49) => mod5_field1_vblnk1_4_Q,
      TXDATA(48) => mod5_field1_vblnk1_4_Q,
      TXDATA(47) => mod5_field1_vblnk1_4_Q,
      TXDATA(46) => mod5_field1_vblnk1_4_Q,
      TXDATA(45) => mod5_field1_vblnk1_4_Q,
      TXDATA(44) => mod5_field1_vblnk1_4_Q,
      TXDATA(43) => mod5_field1_vblnk1_4_Q,
      TXDATA(42) => mod5_field1_vblnk1_4_Q,
      TXDATA(41) => mod5_field1_vblnk1_4_Q,
      TXDATA(40) => mod5_field1_vblnk1_4_Q,
      TXDATA(39) => mod5_field1_vblnk1_4_Q,
      TXDATA(38) => mod5_field1_vblnk1_4_Q,
      TXDATA(37) => mod5_field1_vblnk1_4_Q,
      TXDATA(36) => mod5_field1_vblnk1_4_Q,
      TXDATA(35) => mod5_field1_vblnk1_4_Q,
      TXDATA(34) => mod5_field1_vblnk1_4_Q,
      TXDATA(33) => mod5_field1_vblnk1_4_Q,
      TXDATA(32) => mod5_field1_vblnk1_4_Q,
      TXDATA(31) => mod5_field1_vblnk1_4_Q,
      TXDATA(30) => mod5_field1_vblnk1_4_Q,
      TXDATA(29) => mod5_field1_vblnk1_4_Q,
      TXDATA(28) => mod5_field1_vblnk1_4_Q,
      TXDATA(27) => mod5_field1_vblnk1_4_Q,
      TXDATA(26) => mod5_field1_vblnk1_4_Q,
      TXDATA(25) => mod5_field1_vblnk1_4_Q,
      TXDATA(24) => mod5_field1_vblnk1_4_Q,
      TXDATA(23) => mod5_field1_vblnk1_4_Q,
      TXDATA(22) => mod5_field1_vblnk1_4_Q,
      TXDATA(21) => mod5_field1_vblnk1_4_Q,
      TXDATA(20) => mod5_field1_vblnk1_4_Q,
      TXDATA(19) => mod5_field1_vblnk1_4_Q,
      TXDATA(18) => mod5_field1_vblnk1_4_Q,
      TXDATA(17) => mod5_field1_vblnk1_4_Q,
      TXDATA(16) => mod5_field1_vblnk1_4_Q,
      TXDATA(15) => txda(17),
      TXDATA(14) => txda(16),
      TXDATA(13) => txda(15),
      TXDATA(12) => txda(14),
      TXDATA(11) => txda(13),
      TXDATA(10) => txda(12),
      TXDATA(9) => txda(11),
      TXDATA(8) => txda(10),
      TXDATA(7) => txda(7),
      TXDATA(6) => txda(6),
      TXDATA(5) => txda(5),
      TXDATA(4) => txda(4),
      TXDATA(3) => txda(3),
      TXDATA(2) => txda(2),
      TXDATA(1) => txda(1),
      TXDATA(0) => txda(0),
      TXDATAWIDTH(1) => mod5_field1_vblnk1_4_Q,
      TXDATAWIDTH(0) => mod5_tsl_4_Q,
      TXINTDATAWIDTH(1) => mod5_tsl_4_Q,
      TXINTDATAWIDTH(0) => mod5_tsl_4_Q,
      RXCRCIN(63) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(62) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(61) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(60) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(59) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(58) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(57) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(56) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(55) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(54) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(53) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(52) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(51) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(50) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(49) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(48) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(47) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(46) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(45) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(44) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(43) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(42) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(41) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(40) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(39) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(38) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(37) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(36) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(35) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(34) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(33) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(32) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(31) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(30) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(29) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(28) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(27) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(26) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(25) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(24) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(23) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(22) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(21) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(20) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(19) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(18) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(17) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(16) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(15) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(14) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(13) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(12) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(11) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(10) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(9) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(8) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(7) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(6) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(5) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(4) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(3) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(2) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(1) => mod5_field1_vblnk1_4_Q,
      RXCRCIN(0) => mod5_field1_vblnk1_4_Q,
      RXCRCDATAWIDTH(2) => mod5_field1_vblnk1_4_Q,
      RXCRCDATAWIDTH(1) => mod5_field1_vblnk1_4_Q,
      RXCRCDATAWIDTH(0) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(63) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(62) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(61) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(60) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(59) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(58) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(57) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(56) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(55) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(54) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(53) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(52) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(51) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(50) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(49) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(48) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(47) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(46) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(45) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(44) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(43) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(42) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(41) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(40) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(39) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(38) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(37) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(36) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(35) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(34) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(33) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(32) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(31) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(30) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(29) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(28) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(27) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(26) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(25) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(24) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(23) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(22) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(21) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(20) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(19) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(18) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(17) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(16) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(15) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(14) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(13) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(12) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(11) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(10) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(9) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(8) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(7) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(6) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(5) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(4) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(3) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(2) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(1) => mod5_field1_vblnk1_4_Q,
      TXCRCIN(0) => mod5_field1_vblnk1_4_Q,
      TXCRCDATAWIDTH(2) => mod5_field1_vblnk1_4_Q,
      TXCRCDATAWIDTH(1) => mod5_field1_vblnk1_4_Q,
      TXCRCDATAWIDTH(0) => mod5_field1_vblnk1_4_Q,
      DADDR(7) => mod5_field1_vblnk1_4_Q,
      DADDR(6) => mod5_field1_vblnk1_4_Q,
      DADDR(5) => mod5_field1_vblnk1_4_Q,
      DADDR(4) => mod5_field1_vblnk1_4_Q,
      DADDR(3) => mod5_field1_vblnk1_4_Q,
      DADDR(2) => mod5_field1_vblnk1_4_Q,
      DADDR(1) => mod5_field1_vblnk1_4_Q,
      DADDR(0) => mod5_field1_vblnk1_4_Q,
      DI(15) => mod5_field1_vblnk1_4_Q,
      DI(14) => mod5_field1_vblnk1_4_Q,
      DI(13) => mod5_field1_vblnk1_4_Q,
      DI(12) => mod5_field1_vblnk1_4_Q,
      DI(11) => mod5_field1_vblnk1_4_Q,
      DI(10) => mod5_field1_vblnk1_4_Q,
      DI(9) => mod5_field1_vblnk1_4_Q,
      DI(8) => mod5_field1_vblnk1_4_Q,
      DI(7) => mod5_field1_vblnk1_4_Q,
      DI(6) => mod5_field1_vblnk1_4_Q,
      DI(5) => mod5_field1_vblnk1_4_Q,
      DI(4) => mod5_field1_vblnk1_4_Q,
      DI(3) => mod5_field1_vblnk1_4_Q,
      DI(2) => mod5_field1_vblnk1_4_Q,
      DI(1) => mod5_field1_vblnk1_4_Q,
      DI(0) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(15) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(14) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(13) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(12) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(11) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(10) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(9) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(8) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(7) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(6) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(5) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(4) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(3) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(2) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(1) => mod5_field1_vblnk1_4_Q,
      COMBUSIN(0) => mod5_field1_vblnk1_4_Q,
      CHBONDO(4) => NLW_mgt113b_GT11_CUSTOM_INST_CHBONDO_4_UNCONNECTED,
      CHBONDO(3) => NLW_mgt113b_GT11_CUSTOM_INST_CHBONDO_3_UNCONNECTED,
      CHBONDO(2) => NLW_mgt113b_GT11_CUSTOM_INST_CHBONDO_2_UNCONNECTED,
      CHBONDO(1) => NLW_mgt113b_GT11_CUSTOM_INST_CHBONDO_1_UNCONNECTED,
      CHBONDO(0) => NLW_mgt113b_GT11_CUSTOM_INST_CHBONDO_0_UNCONNECTED,
      RXSTATUS(5) => NLW_mgt113b_GT11_CUSTOM_INST_RXSTATUS_5_UNCONNECTED,
      RXSTATUS(4) => NLW_mgt113b_GT11_CUSTOM_INST_RXSTATUS_4_UNCONNECTED,
      RXSTATUS(3) => NLW_mgt113b_GT11_CUSTOM_INST_RXSTATUS_3_UNCONNECTED,
      RXSTATUS(2) => NLW_mgt113b_GT11_CUSTOM_INST_RXSTATUS_2_UNCONNECTED,
      RXSTATUS(1) => NLW_mgt113b_GT11_CUSTOM_INST_RXSTATUS_1_UNCONNECTED,
      RXSTATUS(0) => NLW_mgt113b_GT11_CUSTOM_INST_RXSTATUS_0_UNCONNECTED,
      RXCHARISCOMMA(7) => NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISCOMMA_7_UNCONNECTED,
      RXCHARISCOMMA(6) => NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISCOMMA_6_UNCONNECTED,
      RXCHARISCOMMA(5) => NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISCOMMA_5_UNCONNECTED,
      RXCHARISCOMMA(4) => NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISCOMMA_4_UNCONNECTED,
      RXCHARISCOMMA(3) => NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISCOMMA_3_UNCONNECTED,
      RXCHARISCOMMA(2) => NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISCOMMA_2_UNCONNECTED,
      RXCHARISCOMMA(1) => NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISCOMMA_1_UNCONNECTED,
      RXCHARISCOMMA(0) => NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISCOMMA_0_UNCONNECTED,
      RXCHARISK(7) => NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISK_7_UNCONNECTED,
      RXCHARISK(6) => NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISK_6_UNCONNECTED,
      RXCHARISK(5) => NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISK_5_UNCONNECTED,
      RXCHARISK(4) => NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISK_4_UNCONNECTED,
      RXCHARISK(3) => NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISK_3_UNCONNECTED,
      RXCHARISK(2) => NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISK_2_UNCONNECTED,
      RXCHARISK(1) => NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISK_1_UNCONNECTED,
      RXCHARISK(0) => NLW_mgt113b_GT11_CUSTOM_INST_RXCHARISK_0_UNCONNECTED,
      RXDATA(63) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_63_UNCONNECTED,
      RXDATA(62) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_62_UNCONNECTED,
      RXDATA(61) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_61_UNCONNECTED,
      RXDATA(60) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_60_UNCONNECTED,
      RXDATA(59) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_59_UNCONNECTED,
      RXDATA(58) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_58_UNCONNECTED,
      RXDATA(57) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_57_UNCONNECTED,
      RXDATA(56) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_56_UNCONNECTED,
      RXDATA(55) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_55_UNCONNECTED,
      RXDATA(54) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_54_UNCONNECTED,
      RXDATA(53) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_53_UNCONNECTED,
      RXDATA(52) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_52_UNCONNECTED,
      RXDATA(51) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_51_UNCONNECTED,
      RXDATA(50) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_50_UNCONNECTED,
      RXDATA(49) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_49_UNCONNECTED,
      RXDATA(48) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_48_UNCONNECTED,
      RXDATA(47) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_47_UNCONNECTED,
      RXDATA(46) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_46_UNCONNECTED,
      RXDATA(45) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_45_UNCONNECTED,
      RXDATA(44) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_44_UNCONNECTED,
      RXDATA(43) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_43_UNCONNECTED,
      RXDATA(42) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_42_UNCONNECTED,
      RXDATA(41) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_41_UNCONNECTED,
      RXDATA(40) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_40_UNCONNECTED,
      RXDATA(39) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_39_UNCONNECTED,
      RXDATA(38) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_38_UNCONNECTED,
      RXDATA(37) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_37_UNCONNECTED,
      RXDATA(36) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_36_UNCONNECTED,
      RXDATA(35) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_35_UNCONNECTED,
      RXDATA(34) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_34_UNCONNECTED,
      RXDATA(33) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_33_UNCONNECTED,
      RXDATA(32) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_32_UNCONNECTED,
      RXDATA(31) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_31_UNCONNECTED,
      RXDATA(30) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_30_UNCONNECTED,
      RXDATA(29) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_29_UNCONNECTED,
      RXDATA(28) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_28_UNCONNECTED,
      RXDATA(27) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_27_UNCONNECTED,
      RXDATA(26) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_26_UNCONNECTED,
      RXDATA(25) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_25_UNCONNECTED,
      RXDATA(24) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_24_UNCONNECTED,
      RXDATA(23) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_23_UNCONNECTED,
      RXDATA(22) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_22_UNCONNECTED,
      RXDATA(21) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_21_UNCONNECTED,
      RXDATA(20) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_20_UNCONNECTED,
      RXDATA(19) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_19_UNCONNECTED,
      RXDATA(18) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_18_UNCONNECTED,
      RXDATA(17) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_17_UNCONNECTED,
      RXDATA(16) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_16_UNCONNECTED,
      RXDATA(15) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_15_UNCONNECTED,
      RXDATA(14) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_14_UNCONNECTED,
      RXDATA(13) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_13_UNCONNECTED,
      RXDATA(12) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_12_UNCONNECTED,
      RXDATA(11) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_11_UNCONNECTED,
      RXDATA(10) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_10_UNCONNECTED,
      RXDATA(9) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_9_UNCONNECTED,
      RXDATA(8) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_8_UNCONNECTED,
      RXDATA(7) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_7_UNCONNECTED,
      RXDATA(6) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_6_UNCONNECTED,
      RXDATA(5) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_5_UNCONNECTED,
      RXDATA(4) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_4_UNCONNECTED,
      RXDATA(3) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_3_UNCONNECTED,
      RXDATA(2) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_2_UNCONNECTED,
      RXDATA(1) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_1_UNCONNECTED,
      RXDATA(0) => NLW_mgt113b_GT11_CUSTOM_INST_RXDATA_0_UNCONNECTED,
      RXDISPERR(7) => NLW_mgt113b_GT11_CUSTOM_INST_RXDISPERR_7_UNCONNECTED,
      RXDISPERR(6) => NLW_mgt113b_GT11_CUSTOM_INST_RXDISPERR_6_UNCONNECTED,
      RXDISPERR(5) => NLW_mgt113b_GT11_CUSTOM_INST_RXDISPERR_5_UNCONNECTED,
      RXDISPERR(4) => NLW_mgt113b_GT11_CUSTOM_INST_RXDISPERR_4_UNCONNECTED,
      RXDISPERR(3) => NLW_mgt113b_GT11_CUSTOM_INST_RXDISPERR_3_UNCONNECTED,
      RXDISPERR(2) => NLW_mgt113b_GT11_CUSTOM_INST_RXDISPERR_2_UNCONNECTED,
      RXDISPERR(1) => NLW_mgt113b_GT11_CUSTOM_INST_RXDISPERR_1_UNCONNECTED,
      RXDISPERR(0) => NLW_mgt113b_GT11_CUSTOM_INST_RXDISPERR_0_UNCONNECTED,
      RXLOSSOFSYNC(1) => NLW_mgt113b_GT11_CUSTOM_INST_RXLOSSOFSYNC_1_UNCONNECTED,
      RXLOSSOFSYNC(0) => NLW_mgt113b_GT11_CUSTOM_INST_RXLOSSOFSYNC_0_UNCONNECTED,
      RXNOTINTABLE(7) => NLW_mgt113b_GT11_CUSTOM_INST_RXNOTINTABLE_7_UNCONNECTED,
      RXNOTINTABLE(6) => NLW_mgt113b_GT11_CUSTOM_INST_RXNOTINTABLE_6_UNCONNECTED,
      RXNOTINTABLE(5) => NLW_mgt113b_GT11_CUSTOM_INST_RXNOTINTABLE_5_UNCONNECTED,
      RXNOTINTABLE(4) => NLW_mgt113b_GT11_CUSTOM_INST_RXNOTINTABLE_4_UNCONNECTED,
      RXNOTINTABLE(3) => NLW_mgt113b_GT11_CUSTOM_INST_RXNOTINTABLE_3_UNCONNECTED,
      RXNOTINTABLE(2) => NLW_mgt113b_GT11_CUSTOM_INST_RXNOTINTABLE_2_UNCONNECTED,
      RXNOTINTABLE(1) => NLW_mgt113b_GT11_CUSTOM_INST_RXNOTINTABLE_1_UNCONNECTED,
      RXNOTINTABLE(0) => NLW_mgt113b_GT11_CUSTOM_INST_RXNOTINTABLE_0_UNCONNECTED,
      RXRUNDISP(7) => NLW_mgt113b_GT11_CUSTOM_INST_RXRUNDISP_7_UNCONNECTED,
      RXRUNDISP(6) => NLW_mgt113b_GT11_CUSTOM_INST_RXRUNDISP_6_UNCONNECTED,
      RXRUNDISP(5) => NLW_mgt113b_GT11_CUSTOM_INST_RXRUNDISP_5_UNCONNECTED,
      RXRUNDISP(4) => NLW_mgt113b_GT11_CUSTOM_INST_RXRUNDISP_4_UNCONNECTED,
      RXRUNDISP(3) => NLW_mgt113b_GT11_CUSTOM_INST_RXRUNDISP_3_UNCONNECTED,
      RXRUNDISP(2) => NLW_mgt113b_GT11_CUSTOM_INST_RXRUNDISP_2_UNCONNECTED,
      RXRUNDISP(1) => NLW_mgt113b_GT11_CUSTOM_INST_RXRUNDISP_1_UNCONNECTED,
      RXRUNDISP(0) => NLW_mgt113b_GT11_CUSTOM_INST_RXRUNDISP_0_UNCONNECTED,
      TXKERR(7) => NLW_mgt113b_GT11_CUSTOM_INST_TXKERR_7_UNCONNECTED,
      TXKERR(6) => NLW_mgt113b_GT11_CUSTOM_INST_TXKERR_6_UNCONNECTED,
      TXKERR(5) => NLW_mgt113b_GT11_CUSTOM_INST_TXKERR_5_UNCONNECTED,
      TXKERR(4) => NLW_mgt113b_GT11_CUSTOM_INST_TXKERR_4_UNCONNECTED,
      TXKERR(3) => NLW_mgt113b_GT11_CUSTOM_INST_TXKERR_3_UNCONNECTED,
      TXKERR(2) => NLW_mgt113b_GT11_CUSTOM_INST_TXKERR_2_UNCONNECTED,
      TXKERR(1) => NLW_mgt113b_GT11_CUSTOM_INST_TXKERR_1_UNCONNECTED,
      TXKERR(0) => NLW_mgt113b_GT11_CUSTOM_INST_TXKERR_0_UNCONNECTED,
      TXRUNDISP(7) => NLW_mgt113b_GT11_CUSTOM_INST_TXRUNDISP_7_UNCONNECTED,
      TXRUNDISP(6) => NLW_mgt113b_GT11_CUSTOM_INST_TXRUNDISP_6_UNCONNECTED,
      TXRUNDISP(5) => NLW_mgt113b_GT11_CUSTOM_INST_TXRUNDISP_5_UNCONNECTED,
      TXRUNDISP(4) => NLW_mgt113b_GT11_CUSTOM_INST_TXRUNDISP_4_UNCONNECTED,
      TXRUNDISP(3) => NLW_mgt113b_GT11_CUSTOM_INST_TXRUNDISP_3_UNCONNECTED,
      TXRUNDISP(2) => NLW_mgt113b_GT11_CUSTOM_INST_TXRUNDISP_2_UNCONNECTED,
      TXRUNDISP(1) => NLW_mgt113b_GT11_CUSTOM_INST_TXRUNDISP_1_UNCONNECTED,
      TXRUNDISP(0) => NLW_mgt113b_GT11_CUSTOM_INST_TXRUNDISP_0_UNCONNECTED,
      RXCRCOUT(31) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_31_UNCONNECTED,
      RXCRCOUT(30) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_30_UNCONNECTED,
      RXCRCOUT(29) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_29_UNCONNECTED,
      RXCRCOUT(28) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_28_UNCONNECTED,
      RXCRCOUT(27) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_27_UNCONNECTED,
      RXCRCOUT(26) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_26_UNCONNECTED,
      RXCRCOUT(25) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_25_UNCONNECTED,
      RXCRCOUT(24) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_24_UNCONNECTED,
      RXCRCOUT(23) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_23_UNCONNECTED,
      RXCRCOUT(22) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_22_UNCONNECTED,
      RXCRCOUT(21) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_21_UNCONNECTED,
      RXCRCOUT(20) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_20_UNCONNECTED,
      RXCRCOUT(19) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_19_UNCONNECTED,
      RXCRCOUT(18) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_18_UNCONNECTED,
      RXCRCOUT(17) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_17_UNCONNECTED,
      RXCRCOUT(16) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_16_UNCONNECTED,
      RXCRCOUT(15) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_15_UNCONNECTED,
      RXCRCOUT(14) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_14_UNCONNECTED,
      RXCRCOUT(13) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_13_UNCONNECTED,
      RXCRCOUT(12) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_12_UNCONNECTED,
      RXCRCOUT(11) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_11_UNCONNECTED,
      RXCRCOUT(10) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_10_UNCONNECTED,
      RXCRCOUT(9) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_9_UNCONNECTED,
      RXCRCOUT(8) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_8_UNCONNECTED,
      RXCRCOUT(7) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_7_UNCONNECTED,
      RXCRCOUT(6) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_6_UNCONNECTED,
      RXCRCOUT(5) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_5_UNCONNECTED,
      RXCRCOUT(4) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_4_UNCONNECTED,
      RXCRCOUT(3) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_3_UNCONNECTED,
      RXCRCOUT(2) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_2_UNCONNECTED,
      RXCRCOUT(1) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_1_UNCONNECTED,
      RXCRCOUT(0) => NLW_mgt113b_GT11_CUSTOM_INST_RXCRCOUT_0_UNCONNECTED,
      TXCRCOUT(31) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_31_UNCONNECTED,
      TXCRCOUT(30) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_30_UNCONNECTED,
      TXCRCOUT(29) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_29_UNCONNECTED,
      TXCRCOUT(28) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_28_UNCONNECTED,
      TXCRCOUT(27) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_27_UNCONNECTED,
      TXCRCOUT(26) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_26_UNCONNECTED,
      TXCRCOUT(25) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_25_UNCONNECTED,
      TXCRCOUT(24) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_24_UNCONNECTED,
      TXCRCOUT(23) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_23_UNCONNECTED,
      TXCRCOUT(22) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_22_UNCONNECTED,
      TXCRCOUT(21) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_21_UNCONNECTED,
      TXCRCOUT(20) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_20_UNCONNECTED,
      TXCRCOUT(19) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_19_UNCONNECTED,
      TXCRCOUT(18) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_18_UNCONNECTED,
      TXCRCOUT(17) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_17_UNCONNECTED,
      TXCRCOUT(16) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_16_UNCONNECTED,
      TXCRCOUT(15) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_15_UNCONNECTED,
      TXCRCOUT(14) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_14_UNCONNECTED,
      TXCRCOUT(13) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_13_UNCONNECTED,
      TXCRCOUT(12) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_12_UNCONNECTED,
      TXCRCOUT(11) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_11_UNCONNECTED,
      TXCRCOUT(10) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_10_UNCONNECTED,
      TXCRCOUT(9) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_9_UNCONNECTED,
      TXCRCOUT(8) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_8_UNCONNECTED,
      TXCRCOUT(7) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_7_UNCONNECTED,
      TXCRCOUT(6) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_6_UNCONNECTED,
      TXCRCOUT(5) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_5_UNCONNECTED,
      TXCRCOUT(4) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_4_UNCONNECTED,
      TXCRCOUT(3) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_3_UNCONNECTED,
      TXCRCOUT(2) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_2_UNCONNECTED,
      TXCRCOUT(1) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_1_UNCONNECTED,
      TXCRCOUT(0) => NLW_mgt113b_GT11_CUSTOM_INST_TXCRCOUT_0_UNCONNECTED,
      DO(15) => NLW_mgt113b_GT11_CUSTOM_INST_DO_15_UNCONNECTED,
      DO(14) => NLW_mgt113b_GT11_CUSTOM_INST_DO_14_UNCONNECTED,
      DO(13) => NLW_mgt113b_GT11_CUSTOM_INST_DO_13_UNCONNECTED,
      DO(12) => NLW_mgt113b_GT11_CUSTOM_INST_DO_12_UNCONNECTED,
      DO(11) => NLW_mgt113b_GT11_CUSTOM_INST_DO_11_UNCONNECTED,
      DO(10) => NLW_mgt113b_GT11_CUSTOM_INST_DO_10_UNCONNECTED,
      DO(9) => NLW_mgt113b_GT11_CUSTOM_INST_DO_9_UNCONNECTED,
      DO(8) => NLW_mgt113b_GT11_CUSTOM_INST_DO_8_UNCONNECTED,
      DO(7) => NLW_mgt113b_GT11_CUSTOM_INST_DO_7_UNCONNECTED,
      DO(6) => NLW_mgt113b_GT11_CUSTOM_INST_DO_6_UNCONNECTED,
      DO(5) => NLW_mgt113b_GT11_CUSTOM_INST_DO_5_UNCONNECTED,
      DO(4) => NLW_mgt113b_GT11_CUSTOM_INST_DO_4_UNCONNECTED,
      DO(3) => NLW_mgt113b_GT11_CUSTOM_INST_DO_3_UNCONNECTED,
      DO(2) => NLW_mgt113b_GT11_CUSTOM_INST_DO_2_UNCONNECTED,
      DO(1) => NLW_mgt113b_GT11_CUSTOM_INST_DO_1_UNCONNECTED,
      DO(0) => NLW_mgt113b_GT11_CUSTOM_INST_DO_0_UNCONNECTED,
      COMBUSOUT(15) => NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_15_UNCONNECTED,
      COMBUSOUT(14) => NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_14_UNCONNECTED,
      COMBUSOUT(13) => NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_13_UNCONNECTED,
      COMBUSOUT(12) => NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_12_UNCONNECTED,
      COMBUSOUT(11) => NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_11_UNCONNECTED,
      COMBUSOUT(10) => NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_10_UNCONNECTED,
      COMBUSOUT(9) => NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_9_UNCONNECTED,
      COMBUSOUT(8) => NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_8_UNCONNECTED,
      COMBUSOUT(7) => NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_7_UNCONNECTED,
      COMBUSOUT(6) => NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_6_UNCONNECTED,
      COMBUSOUT(5) => NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_5_UNCONNECTED,
      COMBUSOUT(4) => NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_4_UNCONNECTED,
      COMBUSOUT(3) => NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_3_UNCONNECTED,
      COMBUSOUT(2) => NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_2_UNCONNECTED,
      COMBUSOUT(1) => NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_1_UNCONNECTED,
      COMBUSOUT(0) => NLW_mgt113b_GT11_CUSTOM_INST_COMBUSOUT_0_UNCONNECTED
    );
  mod7_Ker721 : LUT3_D
    generic map(
      INIT => X"28"
    )
    port map (
      I0 => mod7_h_state_FFd2_5,
      I1 => mod7_h_state_FFd4,
      I2 => mod7_h_state_FFd3,
      LO => N8576,
      O => mod7_N72
    );
  XST_VCC : VCC
    port map (
      P => mod5_tsl_4_Q
    );
  mod5_hdframe_Ker901 : LUT4_D
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod5_hdframe_n0114,
      I1 => mod5_hdframe_n0115,
      I2 => mod5_hdframe_n0132,
      I3 => mod5_hdframe_n0133,
      LO => N8577,
      O => mod5_hdframe_N901
    );
  XST_GND : GND
    port map (
      G => mod5_field1_vblnk1_4_Q
    );
  txclka2_2 : FDR
    port map (
      D => mod5_tsl_4_Q,
      R => txclka2,
      C => txclka,
      Q => txclka21
    );
  mod7_h_state_FFd1_N01_INV_0 : INV
    port map (
      I => sif_rst,
      O => mod7_h_state_FFd1_N0
    );
  mod6_scram20_inst_Mxor_n0030_Xo_1_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_A(5),
      I1 => mod6_scram20_inst_S(4),
      I2 => mod6_scram20_inst_S(5),
      I3 => mod6_scram20_inst_S(9),
      O => mod6_scram20_inst_N7
    );
  mod5_hdframe_n0075_3_39 : LUT4
    generic map(
      INIT => X"5545"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => mod7_color_N39,
      I2 => N8043,
      I3 => mod7_color_N61,
      O => CHOICE15451
    );
  mod0_Ker51 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => reset_n_IBUF,
      I1 => CS_IBUF,
      I2 => mod0_bitptr(5),
      O => mod0_N5
    );
  txda_0 : FD
    port map (
      D => mod6_scram20_inst_dout(19),
      C => TXRIOA_USRCLK,
      Q => txda(0)
    );
  txda_1 : FD
    port map (
      D => mod6_scram20_inst_dout(18),
      C => TXRIOA_USRCLK,
      Q => txda(1)
    );
  txda_2 : FD
    port map (
      D => mod6_scram20_inst_dout(17),
      C => TXRIOA_USRCLK,
      Q => txda(2)
    );
  txda_3 : FD
    port map (
      D => mod6_scram20_inst_dout(16),
      C => TXRIOA_USRCLK,
      Q => txda(3)
    );
  txda_4 : FD
    port map (
      D => mod6_scram20_inst_dout(15),
      C => TXRIOA_USRCLK,
      Q => txda(4)
    );
  txda_5 : FD
    port map (
      D => mod6_scram20_inst_dout(14),
      C => TXRIOA_USRCLK,
      Q => txda(5)
    );
  txda_6 : FD
    port map (
      D => mod6_scram20_inst_dout(13),
      C => TXRIOA_USRCLK,
      Q => txda(6)
    );
  txda_7 : FD
    port map (
      D => mod6_scram20_inst_dout(12),
      C => TXRIOA_USRCLK,
      Q => txda(7)
    );
  txda_8 : FD
    port map (
      D => mod6_scram20_inst_dout(11),
      C => TXRIOA_USRCLK,
      Q => txda(8)
    );
  txda_9 : FD
    port map (
      D => mod6_scram20_inst_dout(10),
      C => TXRIOA_USRCLK,
      Q => txda(9)
    );
  txda_10 : FD
    port map (
      D => mod6_scram20_inst_dout(9),
      C => TXRIOA_USRCLK,
      Q => txda(10)
    );
  txda_11 : FD
    port map (
      D => mod6_scram20_inst_dout(8),
      C => TXRIOA_USRCLK,
      Q => txda(11)
    );
  txda_12 : FD
    port map (
      D => mod6_scram20_inst_dout(7),
      C => TXRIOA_USRCLK,
      Q => txda(12)
    );
  txda_13 : FD
    port map (
      D => mod6_scram20_inst_dout(6),
      C => TXRIOA_USRCLK,
      Q => txda(13)
    );
  txda_14 : FD
    port map (
      D => mod6_scram20_inst_dout(5),
      C => TXRIOA_USRCLK,
      Q => txda(14)
    );
  txda_15 : FD
    port map (
      D => mod6_scram20_inst_dout(4),
      C => TXRIOA_USRCLK,
      Q => txda(15)
    );
  txda_16 : FD
    port map (
      D => mod6_scram20_inst_dout(3),
      C => TXRIOA_USRCLK,
      Q => txda(16)
    );
  txda_17 : FD
    port map (
      D => mod6_scram20_inst_dout(2),
      C => TXRIOA_USRCLK,
      Q => txda(17)
    );
  txda_18 : FD
    port map (
      D => mod6_scram20_inst_dout(1),
      C => TXRIOA_USRCLK,
      Q => txda(18)
    );
  tx292data_ina_19 : FD
    port map (
      D => mod5_hdframe_luma(9),
      C => txclka2,
      Q => tx292data_ina(19)
    );
  tx292data_ina_18 : FD
    port map (
      D => mod5_hdframe_luma(8),
      C => txclka2,
      Q => tx292data_ina(18)
    );
  tx292data_ina_17 : FD
    port map (
      D => mod5_hdframe_luma(7),
      C => txclka2,
      Q => tx292data_ina(17)
    );
  tx292data_ina_16 : FD
    port map (
      D => mod5_hdframe_luma(6),
      C => txclka2,
      Q => tx292data_ina(16)
    );
  tx292data_ina_15 : FD
    port map (
      D => mod5_hdframe_luma(5),
      C => txclka2,
      Q => tx292data_ina(15)
    );
  tx292data_ina_14 : FD
    port map (
      D => mod5_hdframe_luma(4),
      C => txclka2,
      Q => tx292data_ina(14)
    );
  tx292data_ina_13 : FD
    port map (
      D => mod5_hdframe_luma(3),
      C => txclka2,
      Q => tx292data_ina(13)
    );
  tx292data_ina_12 : FD
    port map (
      D => mod5_hdframe_luma(2),
      C => txclka2,
      Q => tx292data_ina(12)
    );
  tx292data_ina_11 : FD
    port map (
      D => mod5_hdframe_luma(1),
      C => txclka2,
      Q => tx292data_ina(11)
    );
  tx292data_ina_10 : FD
    port map (
      D => mod5_hdframe_luma(0),
      C => txclka2,
      Q => tx292data_ina(10)
    );
  tx292data_ina_9 : FD
    port map (
      D => mod5_hdframe_chroma(9),
      C => txclka2,
      Q => tx292data_ina(9)
    );
  tx292data_ina_8 : FD
    port map (
      D => mod5_hdframe_chroma(8),
      C => txclka2,
      Q => tx292data_ina(8)
    );
  tx292data_ina_7 : FD
    port map (
      D => mod5_hdframe_chroma(7),
      C => txclka2,
      Q => tx292data_ina(7)
    );
  tx292data_ina_6 : FD
    port map (
      D => mod5_hdframe_chroma(6),
      C => txclka2,
      Q => tx292data_ina(6)
    );
  tx292data_ina_5 : FD
    port map (
      D => mod5_hdframe_chroma(5),
      C => txclka2,
      Q => tx292data_ina(5)
    );
  tx292data_ina_4 : FD
    port map (
      D => mod5_hdframe_chroma(4),
      C => txclka2,
      Q => tx292data_ina(4)
    );
  tx292data_ina_3 : FD
    port map (
      D => mod5_hdframe_chroma(3),
      C => txclka2,
      Q => tx292data_ina(3)
    );
  tx292data_ina_2 : FD
    port map (
      D => mod5_hdframe_chroma(2),
      C => txclka2,
      Q => tx292data_ina(2)
    );
  tx292data_ina_1 : FD
    port map (
      D => mod5_hdframe_chroma(1),
      C => txclka2,
      Q => tx292data_ina(1)
    );
  mod6_reset_sync_3 : FDR
    port map (
      D => mod5_tsl_4_Q,
      R => mod0_sd_hd,
      C => txclka2,
      Q => mod6_reset_sync
    );
  mod6_scram20_inst_Mxor_n0031_Result1 : LUT3
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => mod6_scram20_inst_A(4),
      I1 => mod6_scram20_inst_S(1),
      I2 => mod6_scram20_inst_S(5),
      O => mod6_scram20_inst_n0031
    );
  mod6_scram20_inst_Mxor_n0011_Xo_2_2 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_A(14),
      I1 => mod6_scram20_inst_A(4),
      I2 => mod6_scram20_inst_A(5),
      I3 => mod6_scram20_inst_N25,
      O => mod6_scram20_inst_n0011
    );
  mod6_scram20_inst_Mxor_n0050_Result1 : LUT3
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => mod6_scram20_inst_S(7),
      I1 => mod6_scram20_inst_A(2),
      I2 => mod6_scram20_inst_A(12),
      O => mod6_scram20_inst_n0157(2)
    );
  mod6_scram20_inst_A_2 : FDC
    port map (
      D => tx292data_ina(2),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(2)
    );
  mod6_scram20_inst_A_3 : FDC
    port map (
      D => tx292data_ina(3),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(3)
    );
  mod6_scram20_inst_A_4 : FDC
    port map (
      D => tx292data_ina(4),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(4)
    );
  mod6_scram20_inst_A_5 : FDC
    port map (
      D => tx292data_ina(5),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(5)
    );
  mod6_scram20_inst_A_6 : FDC
    port map (
      D => tx292data_ina(6),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(6)
    );
  mod6_scram20_inst_A_7 : FDC
    port map (
      D => tx292data_ina(7),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(7)
    );
  mod6_scram20_inst_A_8 : FDC
    port map (
      D => tx292data_ina(8),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(8)
    );
  mod6_scram20_inst_A_9 : FDC
    port map (
      D => tx292data_ina(9),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(9)
    );
  mod6_scram20_inst_A_10 : FDC
    port map (
      D => tx292data_ina(10),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(10)
    );
  mod6_scram20_inst_A_11 : FDC
    port map (
      D => tx292data_ina(11),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(11)
    );
  mod6_scram20_inst_A_12 : FDC
    port map (
      D => tx292data_ina(12),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(12)
    );
  mod6_scram20_inst_A_13 : FDC
    port map (
      D => tx292data_ina(13),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(13)
    );
  mod6_scram20_inst_A_14 : FDC
    port map (
      D => tx292data_ina(14),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(14)
    );
  mod6_scram20_inst_A_15 : FDC
    port map (
      D => tx292data_ina(15),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(15)
    );
  mod6_scram20_inst_A_16 : FDC
    port map (
      D => tx292data_ina(16),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(16)
    );
  mod6_scram20_inst_A_17 : FDC
    port map (
      D => tx292data_ina(17),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(17)
    );
  mod6_scram20_inst_A_18 : FDC
    port map (
      D => tx292data_ina(18),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(18)
    );
  mod6_scram20_inst_A_19 : FDC
    port map (
      D => tx292data_ina(19),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(19)
    );
  mod6_scram20_inst_dout_1 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(1),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(1)
    );
  mod6_scram20_inst_Mxor_n0035_Result1 : LUT3
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => mod6_scram20_inst_S(8),
      I1 => mod6_scram20_inst_A(1),
      I2 => mod6_scram20_inst_S(4),
      O => mod6_scram20_inst_n0173(0)
    );
  mod6_scram20_inst_Mxor_n0048_Result1 : LUT3
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => mod6_scram20_inst_S(6),
      I1 => mod6_scram20_inst_A(3),
      I2 => mod6_scram20_inst_A(13),
      O => mod6_scram20_inst_n0156(2)
    );
  mod6_scram20_inst_Mxor_n0011_Xo_2_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_A(9),
      I1 => mod6_scram20_inst_S(1),
      I2 => mod6_scram20_inst_S(4),
      I3 => mod6_scram20_inst_S(5),
      O => mod6_scram20_inst_N25
    );
  mod6_scram20_inst_Mxor_n0053_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod6_scram20_inst_S(3),
      I1 => mod6_scram20_inst_A(6),
      O => mod6_scram20_inst_n0173(1)
    );
  mod6_scram20_inst_Mxor_n0010_Xo_2_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_A(15),
      I1 => mod6_scram20_inst_n0173(1),
      I2 => mod6_scram20_inst_S(4),
      I3 => mod6_scram20_inst_A(5),
      O => mod6_scram20_inst_Mxor_n0010_Xo(2)
    );
  mod6_scram20_inst_Mxor_n0009_Xo_2_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_A(16),
      I1 => mod6_scram20_inst_A(6),
      I2 => mod6_scram20_inst_A(11),
      I3 => mod6_scram20_inst_n0172(1),
      O => mod6_scram20_inst_Mxor_n0009_Xo(2)
    );
  mod6_scram20_inst_Mxor_n0037_Result1 : LUT4_D
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_S(2),
      I1 => mod6_scram20_inst_S(3),
      I2 => mod6_scram20_inst_S(4),
      I3 => N8586,
      LO => N8578,
      O => mod6_scram20_inst_N2
    );
  mod6_scram20_inst_Mxor_n0049_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod6_scram20_inst_S(3),
      I1 => mod6_scram20_inst_A(7),
      O => mod6_scram20_inst_n0172(1)
    );
  mod6_scram20_inst_Mxor_n0033_Result1 : LUT3
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => mod6_scram20_inst_A(2),
      I1 => mod6_scram20_inst_S(3),
      I2 => mod6_scram20_inst_S(7),
      O => mod6_scram20_inst_n0033
    );
  mod6_scram20_inst_Mxor_n0047_Result1 : LUT3
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => mod6_scram20_inst_S(9),
      I1 => mod6_scram20_inst_A(0),
      I2 => mod6_scram20_inst_S(5),
      O => mod6_scram20_inst_n0154(2)
    );
  mod6_scram20_inst_Mxor_n0007_Xo_1_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_S(1),
      I1 => mod6_scram20_inst_A(9),
      I2 => mod6_scram20_inst_S(2),
      I3 => mod6_scram20_inst_A(8),
      O => mod6_scram20_inst_Mxor_n0007_Xo(1)
    );
  mod5_hdframe_samplecount_8_rt1_4 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(8),
      O => mod5_hdframe_samplecount_8_rt1
    );
  mod6_scram20_inst_Mxor_n0032_Result1 : LUT3
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => mod6_scram20_inst_A(3),
      I1 => mod6_scram20_inst_S(2),
      I2 => mod6_scram20_inst_S(6),
      O => mod6_scram20_inst_n0032
    );
  mod6_scram20_inst_Mxor_n0025_Xo_0_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_S(9),
      I1 => mod6_scram20_inst_A(0),
      I2 => mod6_scram20_inst_S(1),
      I3 => mod6_scram20_inst_A(9),
      O => mod6_scram20_inst_Mxor_n0025_Xo(0)
    );
  mod6_scram20_inst_Mxor_n0041_inst_lut4_31 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_S(8),
      I1 => mod6_scram20_inst_S(7),
      I2 => mod6_scram20_inst_S(6),
      I3 => mod6_scram20_inst_S(5),
      O => mod6_scram20_inst_Mxor_n0041_net3
    );
  mod6_scram20_inst_Mxor_n0040_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod6_scram20_inst_n0183(0),
      I1 => mod6_scram20_inst_S(4),
      O => mod6_scram20_inst_n0182(0)
    );
  mod5_hdframe_n0075_6_148_SW0 : LUT3
    generic map(
      INIT => X"DF"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_n0116,
      I2 => mod5_hdframe_chroma_crc(15),
      O => N8026
    );
  mod7_h_state_Out21 : LUT4_D
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod7_h_state_FFd3_5,
      I1 => mod7_h_state_FFd2_5,
      I2 => mod7_h_state_FFd4,
      I3 => mod7_h_state_FFd1,
      LO => N8579,
      O => mod7_n0023
    );
  mod6_scram20_inst_Mxor_n0012_Xo_1_2 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_A(13),
      I1 => mod6_scram20_inst_A(3),
      I2 => mod6_scram20_inst_A(4),
      I3 => mod6_scram20_inst_N23,
      O => mod6_scram20_inst_n0012
    );
  mod6_scram20_inst_Mxor_n0036_Result1 : LUT2_L
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod6_scram20_inst_N(1),
      I1 => mod6_scram20_inst_S(0),
      LO => mod6_scram20_inst_n0184(0)
    );
  mod6_scram20_inst_Mxor_n0010_Xo_3_1 : LUT3
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => mod6_scram20_inst_Mxor_n0010_Xo(2),
      I1 => mod6_scram20_inst_n0154(2),
      I2 => mod6_scram20_inst_A(10),
      O => mod6_scram20_inst_n0010
    );
  mod6_scram20_inst_Mxor_n0012_Xo_1_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_A(8),
      I1 => mod6_scram20_inst_S(2),
      I2 => mod6_scram20_inst_S(5),
      I3 => mod6_scram20_inst_S(6),
      O => mod6_scram20_inst_N23
    );
  mod6_scram20_inst_Mxor_n0013_Xo_1_2 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_A(12),
      I1 => mod6_scram20_inst_A(2),
      I2 => mod6_scram20_inst_A(3),
      I3 => mod6_scram20_inst_N21,
      O => mod6_scram20_inst_n0013
    );
  mod6_scram20_inst_Mxor_n0013_Xo_1_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_A(7),
      I1 => mod6_scram20_inst_S(3),
      I2 => mod6_scram20_inst_S(6),
      I3 => mod6_scram20_inst_S(7),
      O => mod6_scram20_inst_N21
    );
  mod6_scram20_inst_Mxor_n0014_Xo_2_2 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_A(11),
      I1 => mod6_scram20_inst_A(1),
      I2 => mod6_scram20_inst_A(2),
      I3 => mod6_scram20_inst_N19,
      O => mod6_scram20_inst_n0014
    );
  mod6_scram20_inst_Mxor_n0014_Xo_2_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_A(6),
      I1 => mod6_scram20_inst_S(4),
      I2 => mod6_scram20_inst_S(7),
      I3 => mod6_scram20_inst_S(8),
      O => mod6_scram20_inst_N19
    );
  mod6_scram20_inst_Mxor_n0023_Xo_2_2 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_A(0),
      I1 => mod6_scram20_inst_A(10),
      I2 => mod6_scram20_inst_A(1),
      I3 => mod6_scram20_inst_N17,
      O => mod6_scram20_inst_n0023
    );
  mod6_scram20_inst_Mxor_n0023_Xo_2_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_A(5),
      I1 => mod6_scram20_inst_S(5),
      I2 => mod6_scram20_inst_S(8),
      I3 => mod6_scram20_inst_S(9),
      O => mod6_scram20_inst_N17
    );
  mod5_hdframe_Ker53_rn_0111_G : LUT4_L
    generic map(
      INIT => X"FF06"
    )
    port map (
      I0 => mod5_hdframe_f,
      I1 => mod5_hdframe_h,
      I2 => mod5_hdframe_n0132,
      I3 => mod5_hdframe_N301,
      LO => N8511
    );
  mod6_scram20_inst_Mxor_n0025_Xo_1_2 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod6_scram20_inst_A(0),
      I1 => mod6_scram20_inst_N15,
      O => mod6_scram20_inst_n0025
    );
  mod5_hdframe_n0079_12_SW0 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod5_hdframe_chroma_crc(9),
      I1 => mod5_hdframe_chroma_out(9),
      I2 => mod5_hdframe_chroma_out(8),
      I3 => mod5_hdframe_chroma_crc(8),
      O => N8454
    );
  mod6_scram20_inst_Mxor_n0025_Xo_1_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_A(4),
      I1 => mod6_scram20_inst_A(9),
      I2 => mod6_scram20_inst_S(1),
      I3 => mod6_scram20_inst_S(9),
      O => mod6_scram20_inst_N15
    );
  mod6_scram20_inst_Mxor_n0026_Result1 : LUT3_D
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => mod6_scram20_inst_N(1),
      I1 => mod6_scram20_inst_S(0),
      I2 => mod6_scram20_inst_S(19),
      LO => N8580,
      O => mod6_scram20_inst_n0168(0)
    );
  mod6_scram20_inst_Mxor_n0019_Result1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_n0165(0),
      I1 => mod6_scram20_inst_S(16),
      I2 => mod6_scram20_inst_S(15),
      I3 => mod6_scram20_inst_S(14),
      O => mod6_scram20_inst_n0162(0)
    );
  mod6_scram20_inst_Mxor_n0022_Result1 : LUT3_D
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => mod6_scram20_inst_S(18),
      I1 => mod6_scram20_inst_S(17),
      I2 => N8580,
      LO => N8581,
      O => mod6_scram20_inst_n0165(0)
    );
  mod7_n0034_7_145 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_h_state_FFd4,
      I1 => mod7_h_state_FFd3,
      I2 => mod7_h_state_FFd1,
      I3 => mod7_tp_19_Q,
      O => CHOICE14247
    );
  mod6_scram20_inst_Mxor_n0020_Result1 : LUT3_D
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => mod6_scram20_inst_S(16),
      I1 => mod6_scram20_inst_S(15),
      I2 => mod6_scram20_inst_n0165(0),
      LO => N8582,
      O => mod6_scram20_inst_n0163(0)
    );
  mod7_n0034_4_23 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_h_state_FFd1,
      I1 => mod7_h_state_FFd4,
      I2 => mod7_h_state_FFd3,
      I3 => mod7_tp_19_Q,
      O => CHOICE14300
    );
  mod6_scram20_inst_Mxor_n0018_Result1 : LUT3_D
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => mod6_scram20_inst_S(14),
      I1 => mod6_scram20_inst_S(13),
      I2 => N8582,
      LO => N8583,
      O => mod6_scram20_inst_n0161(0)
    );
  mod6_scram20_inst_Mxor_n0039_Result1 : LUT3
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => mod6_scram20_inst_n0183(0),
      I1 => mod6_scram20_inst_S(4),
      I2 => mod6_scram20_inst_S(3),
      O => mod6_scram20_inst_n0181(0)
    );
  mod6_scram20_inst_Mxor_n0027_Xo_1_2 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod6_scram20_inst_A(3),
      I1 => mod6_scram20_inst_N13,
      O => mod6_scram20_inst_n0027
    );
  mod6_scram20_inst_Mxor_n0027_Xo_1_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_A(8),
      I1 => mod6_scram20_inst_S(1),
      I2 => mod6_scram20_inst_S(2),
      I3 => mod6_scram20_inst_S(6),
      O => mod6_scram20_inst_N13
    );
  mod6_scram20_inst_Mxor_n0028_Xo_1_2 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod6_scram20_inst_A(2),
      I1 => mod6_scram20_inst_N11,
      O => mod6_scram20_inst_n0028
    );
  mod6_scram20_inst_N_6 : FDC
    port map (
      D => mod6_scram20_inst_n0001,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(6)
    );
  mod6_scram20_inst_N_7 : FDC
    port map (
      D => mod6_scram20_inst_n0145(0),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(7)
    );
  mod6_scram20_inst_N_8 : FDC
    port map (
      D => mod6_scram20_inst_n0146(0),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(8)
    );
  mod6_scram20_inst_N_9 : FDC
    port map (
      D => N8584,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(9)
    );
  mod6_scram20_inst_dout_2 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(2),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(2)
    );
  mod6_scram20_inst_S_0 : FDC
    port map (
      D => mod6_scram20_inst_n0154(2),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(0)
    );
  mod6_scram20_inst_S_1 : FDC
    port map (
      D => mod6_scram20_inst_n0006,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(1)
    );
  mod6_scram20_inst_S_2 : FDC
    port map (
      D => mod6_scram20_inst_n0007,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(2)
    );
  mod6_scram20_inst_S_3 : FDC
    port map (
      D => mod6_scram20_inst_n0008,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(3)
    );
  mod6_scram20_inst_S_4 : FDC
    port map (
      D => mod6_scram20_inst_n0009,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(4)
    );
  mod6_scram20_inst_S_5 : FDC
    port map (
      D => mod6_scram20_inst_n0010,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(5)
    );
  mod6_scram20_inst_S_6 : FDC
    port map (
      D => mod6_scram20_inst_n0011,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(6)
    );
  mod6_scram20_inst_S_7 : FDC
    port map (
      D => mod6_scram20_inst_n0012,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(7)
    );
  mod6_scram20_inst_S_8 : FDC
    port map (
      D => mod6_scram20_inst_n0013,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(8)
    );
  mod6_scram20_inst_N2_0_5 : FDC
    port map (
      D => mod6_scram20_inst_N(0),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(0)
    );
  mod6_scram20_inst_S_9 : FDC
    port map (
      D => mod6_scram20_inst_n0014,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(9)
    );
  mod6_scram20_inst_N2_1 : FDC
    port map (
      D => mod6_scram20_inst_N(19),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(1)
    );
  mod6_scram20_inst_A_0 : FDC
    port map (
      D => tx292data_ina(0),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(0)
    );
  mod6_scram20_inst_N2_2 : FDC
    port map (
      D => mod6_scram20_inst_N(18),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(2)
    );
  mod6_scram20_inst_N_10 : FDC
    port map (
      D => mod6_scram20_inst_n0148(0),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(10)
    );
  mod6_scram20_inst_N2_3 : FDC
    port map (
      D => mod6_scram20_inst_N(17),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(3)
    );
  mod6_scram20_inst_N_11 : FDC
    port map (
      D => N8585,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(11)
    );
  mod6_scram20_inst_N2_4 : FDC
    port map (
      D => mod6_scram20_inst_N(16),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(4)
    );
  mod6_scram20_inst_N_12 : FDC
    port map (
      D => mod6_scram20_inst_n0160(0),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(12)
    );
  mod6_scram20_inst_N2_5 : FDC
    port map (
      D => mod6_scram20_inst_N(15),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(5)
    );
  mod6_scram20_inst_N_13 : FDC
    port map (
      D => N8583,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(13)
    );
  mod6_scram20_inst_N2_6 : FDC
    port map (
      D => mod6_scram20_inst_N(14),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(6)
    );
  mod6_scram20_inst_N_14 : FDC
    port map (
      D => mod6_scram20_inst_n0162(0),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(14)
    );
  mod6_scram20_inst_N2_7 : FDC
    port map (
      D => mod6_scram20_inst_N(13),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(7)
    );
  mod6_scram20_inst_N_15 : FDC
    port map (
      D => N8582,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(15)
    );
  mod6_scram20_inst_N2_8 : FDC
    port map (
      D => mod6_scram20_inst_N(12),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(8)
    );
  mod6_scram20_inst_N_16 : FDC
    port map (
      D => mod6_scram20_inst_n0164(0),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(16)
    );
  mod6_scram20_inst_N2_9 : FDC
    port map (
      D => mod6_scram20_inst_N(11),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(9)
    );
  mod6_scram20_inst_N_17 : FDC
    port map (
      D => N8581,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(17)
    );
  mod6_scram20_inst_S_10 : FDC
    port map (
      D => mod6_scram20_inst_n0023,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(10)
    );
  mod6_scram20_inst_N_18 : FDC
    port map (
      D => mod6_scram20_inst_n0166(0),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(18)
    );
  mod6_scram20_inst_S_11 : FDC
    port map (
      D => mod6_scram20_inst_n0025,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(11)
    );
  mod6_scram20_inst_N_19 : FDC
    port map (
      D => N8580,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(19)
    );
  mod6_scram20_inst_S_12 : FDC
    port map (
      D => mod6_scram20_inst_n0027,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(12)
    );
  mod6_scram20_inst_S_13 : FDC
    port map (
      D => mod6_scram20_inst_n0028,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(13)
    );
  mod6_scram20_inst_N2_10 : FDC
    port map (
      D => mod6_scram20_inst_N(10),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(10)
    );
  mod6_scram20_inst_S_14 : FDC
    port map (
      D => mod6_scram20_inst_n0029,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(14)
    );
  mod6_scram20_inst_N2_11 : FDC
    port map (
      D => mod6_scram20_inst_N(9),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(11)
    );
  mod6_scram20_inst_S_15 : FDC
    port map (
      D => mod6_scram20_inst_n0030,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(15)
    );
  mod6_scram20_inst_N2_12 : FDC
    port map (
      D => mod6_scram20_inst_N(8),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(12)
    );
  mod6_scram20_inst_S_16 : FDC
    port map (
      D => mod6_scram20_inst_n0031,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(16)
    );
  mod6_scram20_inst_N2_13 : FDC
    port map (
      D => mod6_scram20_inst_N(7),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(13)
    );
  mod6_scram20_inst_S_17 : FDC
    port map (
      D => mod6_scram20_inst_n0032,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(17)
    );
  mod6_scram20_inst_N2_14 : FDC
    port map (
      D => mod6_scram20_inst_N(6),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(14)
    );
  mod6_scram20_inst_S_18 : FDC
    port map (
      D => mod6_scram20_inst_n0033,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(18)
    );
  mod6_scram20_inst_N2_15 : FDC
    port map (
      D => mod6_scram20_inst_N(5),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(15)
    );
  mod6_scram20_inst_dout_0 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(0),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(0)
    );
  mod6_scram20_inst_S_19 : FDC
    port map (
      D => mod6_scram20_inst_n0173(0),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_S(19)
    );
  mod6_scram20_inst_N2_16 : FDC
    port map (
      D => mod6_scram20_inst_N(4),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(16)
    );
  mod6_scram20_inst_N2_17 : FDC
    port map (
      D => mod6_scram20_inst_N(3),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(17)
    );
  mod6_scram20_inst_N2_18 : FDC
    port map (
      D => mod6_scram20_inst_N(2),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(18)
    );
  mod6_scram20_inst_N_0 : FDC
    port map (
      D => mod6_scram20_inst_n0184(0),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(0)
    );
  mod6_scram20_inst_N2_19 : FDC
    port map (
      D => mod6_scram20_inst_N(1),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N2_0(19)
    );
  mod6_scram20_inst_N_1 : FDC
    port map (
      D => mod6_scram20_inst_n0037,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(1)
    );
  mod6_scram20_inst_N_2 : FDC
    port map (
      D => N8578,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(2)
    );
  mod6_scram20_inst_N_3 : FDC
    port map (
      D => mod6_scram20_inst_n0181(0),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(3)
    );
  mod6_scram20_inst_N_4 : FDC
    port map (
      D => mod6_scram20_inst_n0182(0),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(4)
    );
  mod6_scram20_inst_N_5 : FDC
    port map (
      D => N8586,
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_N(5)
    );
  mod5_hdframe_n0075_9_256 : LUT4
    generic map(
      INIT => X"5410"
    )
    port map (
      I0 => mod7_h_state_FFd2_4,
      I1 => mod7_h_state_FFd1_4,
      I2 => mod7_h_state_FFd4_5,
      I3 => mod7_h_state_FFd3_5,
      O => CHOICE14864
    );
  mod6_scram20_inst_Mxor_n0004_Result1 : LUT3_D
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => mod6_scram20_inst_S(10),
      I1 => mod6_scram20_inst_S(9),
      I2 => mod6_scram20_inst_n0159(0),
      LO => N8584,
      O => mod6_scram20_inst_n0147(0)
    );
  mod5_hdframe_n0070_3_67 : LUT4
    generic map(
      INIT => X"8880"
    )
    port map (
      I0 => mod7_color_v1h9_y(5),
      I1 => mod7_color_yramp(3),
      I2 => mod7_color_n0041,
      I3 => mod7_color_N01,
      O => CHOICE15151
    );
  mod6_scram20_inst_Mxor_n0016_Result1 : LUT3_D
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => mod6_scram20_inst_S(12),
      I1 => mod6_scram20_inst_S(11),
      I2 => mod6_scram20_inst_n0161(0),
      LO => N8585,
      O => mod6_scram20_inst_n0159(0)
    );
  mod6_scram20_inst_A_1 : FDC
    port map (
      D => tx292data_ina(1),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_A(1)
    );
  mod6_scram20_inst_Mxor_n0008_Xo_3_1_SW0 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_S(2),
      I1 => mod6_scram20_inst_S(1),
      I2 => mod6_scram20_inst_A(8),
      I3 => mod6_scram20_inst_A(17),
      O => N8253
    );
  mod6_scram20_inst_Mxor_n0009_Xo_3_1 : LUT3
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => mod6_scram20_inst_Mxor_n0009_Xo(2),
      I1 => mod6_scram20_inst_n0173(0),
      I2 => mod6_scram20_inst_S(2),
      O => mod6_scram20_inst_n0009
    );
  mod6_scram20_inst_Mxor_n0007_Xo_3_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_Mxor_n0007_Xo(1),
      I1 => mod6_scram20_inst_A(18),
      I2 => mod6_scram20_inst_n0156(2),
      I3 => mod6_scram20_inst_n0154(2),
      O => mod6_scram20_inst_n0007
    );
  mod6_scram20_inst_Mxor_n0006_Xo_4_1_SW0 : LUT3
    generic map(
      INIT => X"69"
    )
    port map (
      I0 => mod6_scram20_inst_A(19),
      I1 => mod6_scram20_inst_A(14),
      I2 => mod6_scram20_inst_A(10),
      O => N8155
    );
  mod6_scram20_inst_Mxor_n0006_Xo_4_1 : LUT4
    generic map(
      INIT => X"9669"
    )
    port map (
      I0 => mod6_scram20_inst_n0173(0),
      I1 => mod6_scram20_inst_Mxor_n0025_Xo(0),
      I2 => mod6_scram20_inst_A(4),
      I3 => N8155,
      O => mod6_scram20_inst_n0006
    );
  mod6_scram20_inst_Mxor_n0028_Xo_1_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_A(7),
      I1 => mod6_scram20_inst_S(2),
      I2 => mod6_scram20_inst_S(3),
      I3 => mod6_scram20_inst_S(7),
      O => mod6_scram20_inst_N11
    );
  mod6_scram20_inst_dout_19 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(19),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(19)
    );
  mod6_scram20_inst_dout_18 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(18),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(18)
    );
  mod6_scram20_inst_dout_17 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(17),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(17)
    );
  mod6_scram20_inst_dout_16 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(16),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(16)
    );
  mod6_scram20_inst_dout_15 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(15),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(15)
    );
  mod6_scram20_inst_dout_14 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(14),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(14)
    );
  mod6_scram20_inst_dout_13 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(13),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(13)
    );
  mod6_scram20_inst_dout_12 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(12),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(12)
    );
  mod6_scram20_inst_dout_11 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(11),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(11)
    );
  mod6_scram20_inst_dout_10 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(10),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(10)
    );
  mod6_scram20_inst_dout_9 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(9),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(9)
    );
  mod6_scram20_inst_dout_8 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(8),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(8)
    );
  mod6_scram20_inst_dout_7 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(7),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(7)
    );
  mod6_scram20_inst_dout_6 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(6),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(6)
    );
  mod6_scram20_inst_dout_5 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(5),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(5)
    );
  mod6_scram20_inst_dout_4 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(4),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(4)
    );
  mod6_scram20_inst_dout_3 : FDC
    port map (
      D => mod6_scram20_inst_N2_0(3),
      CLR => mod6_reset_sync,
      C => txclka2,
      Q => mod6_scram20_inst_dout(3)
    );
  mod6_scram20_inst_Mxor_n0041_inst_lut4_41 : LUT4_D
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_Mxor_n0041_net0,
      I1 => mod6_scram20_inst_Mxor_n0041_net1,
      I2 => mod6_scram20_inst_Mxor_n0041_net2,
      I3 => mod6_scram20_inst_Mxor_n0041_net3,
      LO => N8586,
      O => mod6_scram20_inst_n0183(0)
    );
  mod6_scram20_inst_Mxor_n0008_Xo_3_1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_n0157(2),
      I1 => mod6_scram20_inst_S(3),
      I2 => mod6_scram20_inst_A(7),
      I3 => N8253,
      O => mod6_scram20_inst_n0008
    );
  mod6_scram20_inst_Mxor_n0041_inst_lut4_01 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_n0184(0),
      I1 => mod6_scram20_inst_S(19),
      I2 => mod6_scram20_inst_S(18),
      I3 => mod6_scram20_inst_S(17),
      O => mod6_scram20_inst_Mxor_n0041_net0
    );
  mod6_scram20_inst_Mxor_n0041_inst_lut4_11 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_S(16),
      I1 => mod6_scram20_inst_S(15),
      I2 => mod6_scram20_inst_S(14),
      I3 => mod6_scram20_inst_S(13),
      O => mod6_scram20_inst_Mxor_n0041_net1
    );
  mod6_scram20_inst_Mxor_n0041_inst_lut4_21 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_S(12),
      I1 => mod6_scram20_inst_S(11),
      I2 => mod6_scram20_inst_S(10),
      I3 => mod6_scram20_inst_S(9),
      O => mod6_scram20_inst_Mxor_n0041_net2
    );
  mod6_scram20_inst_Mxor_n0029_Result2 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod6_scram20_inst_A(1),
      I1 => mod6_scram20_inst_N9,
      O => mod6_scram20_inst_n0029
    );
  mod6_scram20_inst_Mxor_n0029_Result1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_A(6),
      I1 => mod6_scram20_inst_S(3),
      I2 => mod6_scram20_inst_S(4),
      I3 => mod6_scram20_inst_S(8),
      O => mod6_scram20_inst_N9
    );
  mod6_scram20_inst_Mxor_n0030_Xo_1_2 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod6_scram20_inst_A(0),
      I1 => mod6_scram20_inst_N7,
      O => mod6_scram20_inst_n0030
    );
  mod6_scram20_inst_Mxor_n0037_Result2 : LUT2_L
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod6_scram20_inst_S(1),
      I1 => mod6_scram20_inst_N2,
      LO => mod6_scram20_inst_n0037
    );
  mod7_video_sm_active_line_n0000_9_cy : MUXCY
    port map (
      CI => mod7_video_sm_active_line_n0000_8_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_active_line_9_rt,
      O => mod7_video_sm_active_line_n0000_9_cyo
    );
  mod7_load_val_8 : FDC
    port map (
      D => mod7_n0034(8),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_load_val(8)
    );
  mod7_video_sm_active_line_n0000_9_xor : XORCY
    port map (
      CI => mod7_video_sm_active_line_n0000_8_cyo,
      LI => mod7_active_line_9_rt,
      O => mod7_active_line_n0000(9)
    );
  mod7_n00431 : LUT4
    generic map(
      INIT => X"0207"
    )
    port map (
      I0 => mod5_field2_vblnk1_5_Q,
      I1 => mod7_active_line(9),
      I2 => mod7_active_line(10),
      I3 => mod7_active_line(0),
      O => mod7_n0316(3)
    );
  mod7_n0316_3_14 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_n0316(3),
      I1 => mod7_v_state(3),
      I2 => mod7_n0315_15_Q,
      O => mod7_MUX_BLOCK_N16
    );
  mod7_n0047_3_141 : LUT4
    generic map(
      INIT => X"F4FE"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(8),
      I2 => N8297,
      I3 => mod7_active_line(5),
      O => CHOICE13802
    );
  mod7_n0034_8_149 : LUT2_L
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => CHOICE14276,
      I1 => CHOICE14286,
      LO => mod7_n0034(8)
    );
  mod5_Mrom_n0003_inst_lut4_401 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(0),
      I2 => mod0_hd_format_tmp(3),
      I3 => mod0_hd_format_tmp(1),
      O => mod5_tsl_2_Q
    );
  mod7_v_state_1 : FDC_1
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_n0036(1),
      CLR => mod7_tp_20_Q,
      C => mod5_hdframe_line_clk,
      Q => mod7_v_state(1)
    );
  mod7_yramp_en_6 : FDE
    port map (
      D => mod7_n0035,
      CE => sif_rst,
      C => txclka2,
      Q => mod7_yramp_en
    );
  mod7_n0046_1_11 : LUT4
    generic map(
      INIT => X"5510"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod0_pattern(2),
      I2 => mod7_active_line(9),
      I3 => mod7_active_line(1),
      O => CHOICE12885
    );
  mod7_v_state_3 : FDC_1
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_n0036(3),
      CLR => mod7_tp_20_Q,
      C => mod5_hdframe_line_clk,
      Q => mod7_v_state(3)
    );
  mod7_n0044_0_108 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => mod7_N24,
      I1 => mod7_v_state(0),
      I2 => CHOICE13054,
      I3 => CHOICE13069,
      O => mod7_n0315_28_Q
    );
  mod7_n0044_1_110 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE12803,
      I1 => mod7_v_state(1),
      I2 => CHOICE12779,
      I3 => CHOICE12783,
      O => mod7_n0315_29_Q
    );
  mod7_n0044_2_140 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE12839,
      I1 => mod7_v_state(2),
      I2 => CHOICE12811,
      I3 => CHOICE12823,
      O => mod7_n0315_30_Q
    );
  mod7_n0044_3_1122 : LUT4
    generic map(
      INIT => X"5540"
    )
    port map (
      I0 => mod7_n0061,
      I1 => CHOICE13205,
      I2 => mod7_v_state(3),
      I3 => CHOICE13370,
      O => mod7_n0315_31_Q
    );
  mod7_h_state_Out151 : LUT4_D
    generic map(
      INIT => X"F8B8"
    )
    port map (
      I0 => mod7_h_state_FFd3_1,
      I1 => mod7_h_state_FFd1_1,
      I2 => mod7_h_state_FFd4_1,
      I3 => mod7_h_state_FFd2_1,
      LO => N8587,
      O => mod7_tp_0_Q
    );
  mod7_n0044_3_957_SW0 : LUT4
    generic map(
      INIT => X"028A"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(6),
      I3 => mod7_active_line(4),
      O => N8191
    );
  mod7_n0034_8_141 : LUT4_L
    generic map(
      INIT => X"FFA8"
    )
    port map (
      I0 => CHOICE14284,
      I1 => mod7_n0029,
      I2 => CHOICE14281,
      I3 => mod7_N28,
      LO => CHOICE14286
    );
  mod7_counter_state_counter_n0009_14_cy : MUXCY
    port map (
      CI => mod7_counter_state_counter_n0009_13_cyo,
      DI => mod5_tsl_4_Q,
      S => mod7_counter_N5,
      O => mod7_counter_state_counter_n0009_14_cyo
    );
  mod7_v_state_2 : FDC_1
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_n0036(2),
      CLR => mod7_tp_20_Q,
      C => mod5_hdframe_line_clk,
      Q => mod7_v_state(2)
    );
  mod7_n0034_1_57 : LUT4
    generic map(
      INIT => X"F5F4"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => CHOICE14177,
      I2 => CHOICE14173,
      I3 => CHOICE14175,
      O => CHOICE14181
    );
  mod7_n0034_1_224 : LUT4_L
    generic map(
      INIT => X"FFA8"
    )
    port map (
      I0 => mod7_load_val(1),
      I1 => CHOICE14181,
      I2 => CHOICE14168,
      I3 => CHOICE14212,
      LO => mod7_n0034(1)
    );
  mod7_n0034_2_237 : LUT4_L
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE14480,
      I1 => mod7_load_val(2),
      I2 => mod7_N57,
      I3 => CHOICE14442,
      LO => mod7_n0034(2)
    );
  mod7_color_Ker41_SW0 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_tp_option(1),
      I2 => mod0_tp_option(0),
      I3 => mod7_color_v1h4_y(9),
      O => N6699
    );
  mod7_n0034_4_182 : LUT4_L
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE14327,
      I1 => mod7_N28,
      I2 => CHOICE14315,
      I3 => CHOICE14307,
      LO => mod7_n0034(4)
    );
  mod7_n0034_5_188 : LUT4_L
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE14426,
      I1 => mod7_tp_21_Q,
      I2 => CHOICE14384,
      I3 => CHOICE14403,
      LO => mod7_n0034(5)
    );
  mod5_hdframe_n0075_3_106_SW1 : LUT4
    generic map(
      INIT => X"8A02"
    )
    port map (
      I0 => mod5_hdframe_N871,
      I1 => mod5_hdframe_n0115,
      I2 => N8507,
      I3 => mod5_hdframe_linecount(8),
      O => N8053
    );
  mod7_n0034_7_171 : LUT4_L
    generic map(
      INIT => X"FFA8"
    )
    port map (
      I0 => mod7_load_val(7),
      I1 => CHOICE14225,
      I2 => CHOICE14228,
      I3 => CHOICE14251,
      LO => mod7_n0034(7)
    );
  mod7_n00401 : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => mod0_hd_format_tmp(1),
      I1 => mod0_hd_format_tmp(2),
      I2 => mod0_hd_format_tmp(3),
      O => mod7_tp_21_Q
    );
  mod7_Ker2425_G : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => mod7_N137,
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(6),
      I3 => mod7_active_line(3),
      O => N8513
    );
  mod7_h_state_FFd2_7 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd2_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd2
    );
  mod7_Ker3041 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(0),
      O => mod7_N304
    );
  mod7_h_state_FFd3_8 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd3_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd3
    );
  mod7_Ker3031 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_pattern(2),
      I2 => mod0_pattern(0),
      O => mod7_tp_19_Q
    );
  mod7_h_state_FFd4_9 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd4_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd4
    );
  mod6_scram20_inst_Mxor_n0003_Result1 : LUT4_L
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => N8585,
      I1 => mod6_scram20_inst_S(10),
      I2 => mod6_scram20_inst_S(9),
      I3 => mod6_scram20_inst_S(8),
      LO => mod6_scram20_inst_n0146(0)
    );
  mod7_Ker3021 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(5),
      O => mod7_N302
    );
  mod7_n0046_2_148_G : LUT3
    generic map(
      INIT => X"01"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(3),
      I2 => N8333,
      O => N8515
    );
  mod7_Ker300 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(6),
      I2 => mod7_active_line(8),
      I3 => N113,
      O => mod7_N300
    );
  mod7_load_val_7 : FDC
    port map (
      D => mod7_n0034(7),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_load_val(7)
    );
  mod5_hdframe_n0070_5_208_SW1_SW0 : LUT4
    generic map(
      INIT => X"45EF"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_n0135,
      I2 => mod5_hdframe_nor_cyo25,
      I3 => mod5_hdframe_luma_crc(14),
      O => N8335
    );
  mod7_h_state_Out31 : LUT4_D
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_h_state_FFd4_4,
      I1 => mod7_h_state_FFd1_4,
      I2 => mod7_h_state_FFd3_4,
      I3 => mod7_h_state_FFd2_4,
      LO => N8588,
      O => mod7_n0024
    );
  mod5_hdframe_hd_framegenerator_n0083_10_lut2 : LUT3
    generic map(
      INIT => X"15"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(2),
      O => N7981
    );
  mod7_h_state_Out41 : LUT3_D
    generic map(
      INIT => X"04"
    )
    port map (
      I0 => mod7_h_state_FFd4,
      I1 => mod7_h_state_FFd2_5,
      I2 => mod7_h_state_FFd3,
      LO => N8589,
      O => mod7_n0025
    );
  mod5_hdframe_linecount_7_rt_10 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_linecount(7),
      O => mod5_hdframe_linecount_7_rt
    );
  mod7_h_state_Out51 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod7_h_state_FFd4_5,
      I1 => mod7_h_state_FFd3_5,
      I2 => mod7_h_state_FFd2_5,
      O => mod7_n0026
    );
  mod7_h_state_Out61 : LUT3_D
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => mod7_h_state_FFd4,
      I2 => mod7_h_state_FFd3,
      LO => N8590,
      O => mod7_n0027
    );
  mod7_h_state_Out71 : LUT3_D
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => mod7_h_state_FFd4,
      I2 => mod7_h_state_FFd3,
      LO => N8591,
      O => mod7_n0028
    );
  mod7_h_state_Out81 : LUT3_D
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod7_h_state_FFd4,
      I1 => mod7_h_state_FFd3,
      I2 => mod7_h_state_FFd1,
      LO => N8592,
      O => mod7_n0029
    );
  mod7_color_Ker3734 : LUT4
    generic map(
      INIT => X"FF40"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod7_color_N115,
      I2 => mod7_color_N121,
      I3 => N8642,
      O => CHOICE15282
    );
  mod7_active_line_10 : FDC_1
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_active_line_n0000(10),
      CLR => mod5_hdframe_fvh_out(1),
      C => mod5_hdframe_fvh_out(0),
      Q => mod7_active_line(10)
    );
  mod7_n00701 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_N256,
      I1 => mod7_active_line(1),
      I2 => mod7_N235,
      I3 => mod7_active_line(4),
      O => mod7_n0070
    );
  mod7_n0316_3_6 : LUT4
    generic map(
      INIT => X"FAD8"
    )
    port map (
      I0 => mod7_n0316(3),
      I1 => CHOICE12895,
      I2 => mod7_v_state(1),
      I3 => CHOICE12908,
      O => mod7_MUX_BLOCK_N8
    );
  mod7_tp_22_rn_1 : MUXF5
    port map (
      I0 => mod7_MUX_BLOCK_N7,
      I1 => mod7_MUX_BLOCK_N6,
      S => mod7_tp_22_Q,
      O => mod7_MUX_BLOCK_tp_22_MUXF52
    );
  mod7_n02711 : LUT3
    generic map(
      INIT => X"04"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(9),
      I2 => mod7_active_line(7),
      O => mod7_n0271
    );
  mod5_hdframe_Neq_stagelut22 : LUT4_L
    generic map(
      INIT => X"B400"
    )
    port map (
      I0 => sif_hd_format(0),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(1),
      I3 => mod5_hdframe_samplecount_0_1,
      LO => mod5_hdframe_N193
    );
  mod7_n0316_3_7 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_n0316(3),
      I1 => mod7_v_state(1),
      I2 => mod7_n0315_5_Q,
      O => mod7_MUX_BLOCK_N9
    );
  mod7_tp_22_rn_2 : MUXF5
    port map (
      I0 => mod7_MUX_BLOCK_N9,
      I1 => mod7_MUX_BLOCK_N8,
      S => mod7_tp_22_Q,
      O => mod7_MUX_BLOCK_tp_22_MUXF53
    );
  mod7_Mmux_n0036_n0036_0_n0036_0_Q : MUXF6
    port map (
      I0 => mod7_MUX_BLOCK_tp_22_MUXF53,
      I1 => mod7_MUX_BLOCK_tp_22_MUXF52,
      S => mod5_field2_vblnk1_5_Q,
      O => mod7_n0036(1)
    );
  mod7_n0044_1_94_SW0_G : LUT4
    generic map(
      INIT => X"BFFF"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(0),
      I2 => mod7_active_line(8),
      I3 => CHOICE12798,
      O => N8517
    );
  mod7_n0316_3_8 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_n0316(3),
      I1 => mod7_v_state(2),
      I2 => mod7_n0315_30_Q,
      O => mod7_MUX_BLOCK_N10
    );
  mod7_n0316_3_9 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_n0316(3),
      I1 => mod7_v_state(2),
      I2 => mod7_n0315_22_Q,
      O => mod7_MUX_BLOCK_N11
    );
  mod7_n0046_1_12 : LUT3
    generic map(
      INIT => X"BA"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(7),
      I2 => mod7_active_line(4),
      O => CHOICE12886
    );
  mod7_tp_22_rn_3 : MUXF5
    port map (
      I0 => mod7_MUX_BLOCK_N11,
      I1 => mod7_MUX_BLOCK_N10,
      S => mod7_tp_22_Q,
      O => mod7_MUX_BLOCK_tp_22_MUXF54
    );
  mod7_n0316_3_10 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_n0316(3),
      I1 => mod7_v_state(2),
      I2 => mod7_n0315_14_Q,
      O => mod7_MUX_BLOCK_N12
    );
  mod7_n0316_3_11 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_n0316(3),
      I1 => mod7_v_state(2),
      I2 => mod7_n0315_6_Q,
      O => mod7_MUX_BLOCK_N13
    );
  mod7_tp_22_rn_4 : MUXF5
    port map (
      I0 => mod7_MUX_BLOCK_N13,
      I1 => mod7_MUX_BLOCK_N12,
      S => mod7_tp_22_Q,
      O => mod7_MUX_BLOCK_tp_22_MUXF55
    );
  mod7_Mmux_n0036_n0036_0_n0036_0_rn_0 : MUXF6
    port map (
      I0 => mod7_MUX_BLOCK_tp_22_MUXF55,
      I1 => mod7_MUX_BLOCK_tp_22_MUXF54,
      S => mod5_field2_vblnk1_5_Q,
      O => mod7_n0036(2)
    );
  mod7_n02631 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(1),
      I3 => mod7_active_line(3),
      O => mod7_n0263
    );
  mod7_n0316_3_12 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_n0316(3),
      I1 => mod7_v_state(3),
      I2 => mod7_n0315_31_Q,
      O => mod7_MUX_BLOCK_N14
    );
  mod7_n0316_3_13 : LUT3_L
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_n0316(3),
      I1 => mod7_v_state(3),
      I2 => mod7_n0315_23_Q,
      LO => mod7_MUX_BLOCK_N15
    );
  mod7_tp_22_rn_5 : MUXF5
    port map (
      I0 => mod7_MUX_BLOCK_N15,
      I1 => mod7_MUX_BLOCK_N14,
      S => mod7_tp_22_Q,
      O => mod7_MUX_BLOCK_tp_22_MUXF56
    );
  mod7_video_sm_active_line_n0000_0_lut_INV_0 : INV
    port map (
      I => mod7_active_line(0),
      O => mod7_N3
    );
  mod7_video_sm_active_line_n0000_0_cy : MUXCY
    port map (
      CI => mod5_field1_vblnk1_4_Q,
      DI => mod5_tsl_4_Q,
      S => mod7_N3,
      O => mod7_video_sm_active_line_n0000_0_cyo
    );
  mod5_hdframe_n0075_7_89_SW0 : LUT4_L
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE14720,
      I1 => mod7_color_N45,
      I2 => mod7_color_N96,
      I3 => mod7_color_N33,
      LO => N8083
    );
  mod5_hdframe_n0075_3_21 : LUT4_L
    generic map(
      INIT => X"EAAA"
    )
    port map (
      I0 => mod7_color_n0022,
      I1 => mod7_color_N121,
      I2 => mod7_color_N134,
      I3 => mod7_color_v0h5_cr(0),
      LO => CHOICE15444
    );
  mod7_video_sm_active_line_n0000_1_cy : MUXCY
    port map (
      CI => mod7_video_sm_active_line_n0000_0_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_active_line_1_rt,
      O => mod7_video_sm_active_line_n0000_1_cyo
    );
  mod7_video_sm_active_line_n0000_1_xor : XORCY
    port map (
      CI => mod7_video_sm_active_line_n0000_0_cyo,
      LI => mod7_active_line_1_rt,
      O => mod7_active_line_n0000(1)
    );
  mod7_video_sm_active_line_n0000_2_cy : MUXCY
    port map (
      CI => mod7_video_sm_active_line_n0000_1_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_active_line_2_rt,
      O => mod7_video_sm_active_line_n0000_2_cyo
    );
  mod7_video_sm_active_line_n0000_2_xor : XORCY
    port map (
      CI => mod7_video_sm_active_line_n0000_1_cyo,
      LI => mod7_active_line_2_rt,
      O => mod7_active_line_n0000(2)
    );
  mod7_video_sm_active_line_n0000_3_cy : MUXCY
    port map (
      CI => mod7_video_sm_active_line_n0000_2_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_active_line_3_rt,
      O => mod7_video_sm_active_line_n0000_3_cyo
    );
  mod7_video_sm_active_line_n0000_3_xor : XORCY
    port map (
      CI => mod7_video_sm_active_line_n0000_2_cyo,
      LI => mod7_active_line_3_rt,
      O => mod7_active_line_n0000(3)
    );
  mod7_h_state_Out01 : LUT4_D
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod7_h_state_FFd2_4,
      I1 => mod7_h_state_FFd4_4,
      I2 => mod7_h_state_FFd1_5,
      I3 => mod7_h_state_FFd3_4,
      LO => N8593,
      O => CHOICE14872
    );
  mod7_video_sm_active_line_n0000_4_cy : MUXCY
    port map (
      CI => mod7_video_sm_active_line_n0000_3_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_active_line_4_rt,
      O => mod7_video_sm_active_line_n0000_4_cyo
    );
  mod7_video_sm_active_line_n0000_4_xor : XORCY
    port map (
      CI => mod7_video_sm_active_line_n0000_3_cyo,
      LI => mod7_active_line_4_rt,
      O => mod7_active_line_n0000(4)
    );
  mod7_video_sm_active_line_n0000_5_cy : MUXCY
    port map (
      CI => mod7_video_sm_active_line_n0000_4_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_active_line_5_rt,
      O => mod7_video_sm_active_line_n0000_5_cyo
    );
  mod7_video_sm_active_line_n0000_5_xor : XORCY
    port map (
      CI => mod7_video_sm_active_line_n0000_4_cyo,
      LI => mod7_active_line_5_rt,
      O => mod7_active_line_n0000(5)
    );
  mod7_Ker2541 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(4),
      O => mod7_N254
    );
  mod7_video_sm_active_line_n0000_6_cy : MUXCY
    port map (
      CI => mod7_video_sm_active_line_n0000_5_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_active_line_6_rt,
      O => mod7_video_sm_active_line_n0000_6_cyo
    );
  mod7_video_sm_active_line_n0000_6_xor : XORCY
    port map (
      CI => mod7_video_sm_active_line_n0000_5_cyo,
      LI => mod7_active_line_6_rt,
      O => mod7_active_line_n0000(6)
    );
  mod7_video_sm_active_line_n0000_7_cy : MUXCY
    port map (
      CI => mod7_video_sm_active_line_n0000_6_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_active_line_7_rt,
      O => mod7_video_sm_active_line_n0000_7_cyo
    );
  mod7_video_sm_active_line_n0000_7_xor : XORCY
    port map (
      CI => mod7_video_sm_active_line_n0000_6_cyo,
      LI => mod7_active_line_7_rt,
      O => mod7_active_line_n0000(7)
    );
  mod7_Ker2371 : LUT3_D
    generic map(
      INIT => X"01"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => mod7_h_state_FFd4,
      I2 => mod7_h_state_FFd1,
      LO => N8594,
      O => mod7_N237
    );
  mod7_Ker2951 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(5),
      O => mod7_N295
    );
  mod7_Ker2501 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_active_line(7),
      O => mod7_N250
    );
  mod7_h_state_Out91 : LUT3_D
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod7_h_state_FFd1_5,
      I1 => mod7_h_state_FFd4_5,
      I2 => mod7_h_state_FFd3_5,
      LO => N8595,
      O => mod7_n0030
    );
  mod7_n02571 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(3),
      I2 => mod7_active_line(2),
      I3 => mod7_active_line(7),
      O => mod7_n0257
    );
  mod7_Ker2321 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_N289,
      I1 => mod7_active_line(9),
      I2 => mod7_N254,
      I3 => mod7_active_line(3),
      O => mod7_N232
    );
  mod7_color_Ker1341 : LUT4_D
    generic map(
      INIT => X"0010"
    )
    port map (
      I0 => mod7_tp_4_Q,
      I1 => mod7_tp_5_Q,
      I2 => mod7_h_state_FFd4_3,
      I3 => mod7_h_state_FFd1_3,
      LO => N8596,
      O => mod7_color_N134
    );
  mod7_n0044_3_366 : LUT3
    generic map(
      INIT => X"28"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(0),
      I2 => mod7_active_line(1),
      O => CHOICE13172
    );
  mod7_n0316_3_3 : LUT4
    generic map(
      INIT => X"FAD8"
    )
    port map (
      I0 => mod7_n0316(3),
      I1 => mod7_n0071,
      I2 => mod7_v_state(0),
      I3 => CHOICE13046,
      O => mod7_MUX_BLOCK_N5
    );
  mod7_Ker62 : LUT3
    generic map(
      INIT => X"D5"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(1),
      I2 => mod0_pattern(0),
      O => mod7_N6
    );
  mod7_n00691_SW0 : LUT4
    generic map(
      INIT => X"FFEF"
    )
    port map (
      I0 => mod7_N75,
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(3),
      I3 => mod7_N185,
      O => N8486
    );
  mod7_active_line_6 : FDC_1
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_active_line_n0000(6),
      CLR => mod5_hdframe_fvh_out(1),
      C => mod5_hdframe_fvh_out(0),
      Q => mod7_active_line(6)
    );
  mod7_n02231 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(8),
      I2 => mod7_active_line(2),
      I3 => mod7_active_line(7),
      O => mod7_n0223
    );
  mod7_n02291 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(1),
      I3 => mod7_active_line(8),
      O => mod7_n0229
    );
  mod7_Ker71 : LUT3
    generic map(
      INIT => X"DF"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_tp_21_Q,
      I2 => mod7_v_state(2),
      O => mod7_N7
    );
  mod7_Ker2161 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_n0022,
      I1 => mod7_v_state(3),
      LO => mod7_N216
    );
  mod7_Ker2401 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(4),
      I2 => mod7_active_line(7),
      I3 => mod7_active_line(1),
      O => mod7_N240
    );
  mod7_Ker101 : LUT4
    generic map(
      INIT => X"7555"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_v_state(3),
      I2 => sif_hd_format(2),
      I3 => sif_hd_format(1),
      O => mod7_N10
    );
  mod7_h_state_FFd4_In74 : LUT4
    generic map(
      INIT => X"D5FF"
    )
    port map (
      I0 => CHOICE14005,
      I1 => mod7_h_state_FFd3,
      I2 => mod7_h_state_FFd1,
      I3 => CHOICE14018,
      O => CHOICE13995
    );
  mod7_Ker1371 : LUT3
    generic map(
      INIT => X"95"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(0),
      I2 => mod0_pattern(1),
      O => mod7_N137
    );
  mod7_n0034_7_90_SW0 : LUT4_L
    generic map(
      INIT => X"BA10"
    )
    port map (
      I0 => mod7_v_state(2),
      I1 => mod7_h_state_FFd3,
      I2 => mod7_h_state_FFd2,
      I3 => mod7_n0024,
      LO => N8460
    );
  mod7_Ker1391 : LUT4
    generic map(
      INIT => X"2030"
    )
    port map (
      I0 => mod7_h_state_FFd4,
      I1 => mod7_h_state_FFd3,
      I2 => mod7_h_state_FFd2,
      I3 => mod7_v_state(2),
      O => mod7_N139
    );
  mod7_Ker2631 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => mod7_active_line(7),
      O => mod7_N263
    );
  mod7_h_state_Out111 : LUT3
    generic map(
      INIT => X"04"
    )
    port map (
      I0 => mod7_h_state_FFd4,
      I1 => mod7_h_state_FFd1,
      I2 => mod7_h_state_FFd3,
      O => mod7_n0032
    );
  mod7_counter_n0002_8_1 : LUT3_L
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => N8644,
      I1 => mod7_counter_n0009(19),
      I2 => mod7_load_val(8),
      LO => mod7_counter_n0002(8)
    );
  mod7_Ker2931 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(7),
      O => mod7_N293
    );
  mod7_color_Ker91_SW0 : LUT3
    generic map(
      INIT => X"B5"
    )
    port map (
      I0 => mod0_pattern(0),
      I1 => mod0_tp_option(2),
      I2 => mod0_pattern(2),
      O => N1254
    );
  mod7_Ker2761 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(5),
      O => mod7_N276
    );
  mod7_Ker25100 : LUT4_D
    generic map(
      INIT => X"FFEC"
    )
    port map (
      I0 => mod7_h_state_FFd4,
      I1 => CHOICE14080,
      I2 => N8385,
      I3 => CHOICE14059,
      LO => N8597,
      O => mod7_N25
    );
  mod7_active_line_4 : FDC_1
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_active_line_n0000(4),
      CLR => mod5_hdframe_fvh_out(1),
      C => mod5_hdframe_fvh_out(0),
      Q => mod7_active_line(4)
    );
  mod7_Ker23499 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod7_n0122,
      I2 => mod7_n0099,
      I3 => N8325,
      O => mod7_N234
    );
  mod6_scram20_inst_Mxor_n0017_Result1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_n0163(0),
      I1 => mod6_scram20_inst_S(14),
      I2 => mod6_scram20_inst_S(13),
      I3 => mod6_scram20_inst_S(12),
      O => mod6_scram20_inst_n0160(0)
    );
  mod7_color_Ker77_SW0 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => mod7_color_N137,
      I1 => mod7_color_N122,
      I2 => N8243,
      I3 => mod7_color_N123,
      O => N1234
    );
  mod7_tp_22_1 : MUXF5
    port map (
      I0 => mod7_MUX_BLOCK_N3,
      I1 => mod7_MUX_BLOCK_N2,
      S => mod7_tp_22_Q,
      O => mod7_MUX_BLOCK_tp_22_MUXF5
    );
  mod7_tp_22_rn_0 : MUXF5
    port map (
      I0 => mod7_MUX_BLOCK_N5,
      I1 => mod7_MUX_BLOCK_N4,
      S => mod7_tp_22_Q,
      O => mod7_MUX_BLOCK_tp_22_MUXF51
    );
  mod7_n00681 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(4),
      I2 => mod7_N231,
      I3 => mod7_active_line(0),
      O => mod7_n0068
    );
  mod7_Ker163 : LUT4
    generic map(
      INIT => X"8FF8"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => N8277,
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(8),
      O => mod7_N163
    );
  mod7_Ker2551 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(4),
      O => mod7_N255
    );
  mod7_Ker1841 : LUT4
    generic map(
      INIT => X"66F6"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(4),
      I3 => mod7_active_line(8),
      O => mod7_N184
    );
  mod7_n0045_1_151_SW0_SW1_SW0 : LUT4
    generic map(
      INIT => X"C9FF"
    )
    port map (
      I0 => mod0_pattern(0),
      I1 => mod0_pattern(2),
      I2 => mod0_pattern(1),
      I3 => mod7_active_line(6),
      O => N8488
    );
  mod7_Ker1851 : LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_active_line(6),
      O => mod7_N185
    );
  mod7_Ker2621 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(3),
      O => mod7_N262
    );
  mod7_Ker1861 : LUT4
    generic map(
      INIT => X"4F44"
    )
    port map (
      I0 => mod7_tp_21_Q,
      I1 => mod7_n0030,
      I2 => mod7_v_state(3),
      I3 => N8611,
      O => mod7_N186
    );
  mod7_color_Ker1071 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => CHOICE15007,
      I1 => mod0_pattern(1),
      I2 => mod7_color_N134,
      I3 => mod7_color_N122,
      O => mod7_color_N107
    );
  mod7_color_Ker981 : LUT4_D
    generic map(
      INIT => X"E000"
    )
    port map (
      I0 => mod7_color_v0h2_y_2_Q,
      I1 => mod7_color_v0h2_y_0_Q,
      I2 => mod7_color_N123,
      I3 => mod7_color_N140,
      LO => N8598,
      O => mod7_color_N98
    );
  mod7_n01501 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(6),
      I3 => mod7_active_line(2),
      O => mod7_n0150
    );
  mod7_Ker1421 : LUT4
    generic map(
      INIT => X"F2FA"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(0),
      I2 => mod7_N233,
      I3 => mod7_active_line(4),
      O => mod7_N142
    );
  mod7_Ker2891 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(7),
      O => mod7_N289
    );
  mod5_hdframe_n0075_1_26 : LUT4
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_color_v0h2_y_2_Q,
      I1 => mod7_color_N122,
      I2 => mod7_color_N134,
      I3 => mod7_color_n0012,
      O => CHOICE14601
    );
  mod7_Ker1731 : LUT4
    generic map(
      INIT => X"BFFF"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(1),
      I2 => mod0_pattern(0),
      I3 => mod7_tp_21_Q,
      O => mod7_N173
    );
  mod7_n0046_3_848 : LUT4
    generic map(
      INIT => X"EEAE"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => N8279,
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(4),
      O => CHOICE13640
    );
  mod7_Ker2651 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(3),
      O => mod7_N265
    );
  mod7_Ker511 : LUT2_D
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_h_state_FFd3,
      I1 => mod7_h_state_FFd2,
      LO => N8599,
      O => mod7_N51
    );
  mod7_h_state_FFd3_In80 : LUT4
    generic map(
      INIT => X"BAAA"
    )
    port map (
      I0 => CHOICE13954,
      I1 => mod7_h_state_FFd3,
      I2 => mod7_h_state_FFd1,
      I3 => mod7_n0052,
      O => CHOICE13955
    );
  mod7_color_Ker25_SW0 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_color_v1h4_y(9),
      I1 => mod0_tp_option(1),
      I2 => mod0_pattern(1),
      I3 => mod0_tp_option(0),
      O => N1258
    );
  mod7_load_val_5 : FDC
    port map (
      D => mod7_n0034(5),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_load_val(5)
    );
  mod7_Ker2483 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => mod7_n0061,
      I1 => mod7_v_state(0),
      I2 => CHOICE13413,
      I3 => CHOICE13419,
      O => mod7_N24
    );
  mod5_Mrom_n0003_inst_lut4_4011 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(1),
      O => N7969
    );
  mod7_h_state_FFd3_In102 : LUT4
    generic map(
      INIT => X"FF28"
    )
    port map (
      I0 => mod7_h_state_FFd4,
      I1 => mod7_n0052,
      I2 => mod7_h_state_FFd3,
      I3 => CHOICE13955,
      O => mod7_h_state_FFd3_In
    );
  mod7_n0316_3_4 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_n0316(3),
      I1 => mod7_v_state(1),
      I2 => mod7_n0315_29_Q,
      O => mod7_MUX_BLOCK_N6
    );
  mod7_h_state_Out11 : LUT4_D
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod7_h_state_FFd4,
      I1 => mod7_h_state_FFd3,
      I2 => mod7_h_state_FFd1,
      I3 => mod7_h_state_FFd2,
      LO => N8600,
      O => mod7_n0022
    );
  mod7_color_Ker82_SW0 : LUT4_D
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => mod7_color_N121,
      I1 => mod7_color_N137,
      I2 => mod7_color_v1h4_y(9),
      I3 => mod7_color_n0019,
      LO => N8601,
      O => N1229
    );
  mod7_n0047_1_87 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE13750,
      I1 => mod7_v_state(1),
      I2 => mod7_N163,
      I3 => CHOICE13758,
      O => mod7_n0315_5_Q
    );
  mod7_n0045_3_39 : LUT4
    generic map(
      INIT => X"AF23"
    )
    port map (
      I0 => mod7_N202,
      I1 => N8337,
      I2 => mod7_N287,
      I3 => CHOICE14042,
      O => mod7_n0315_23_Q
    );
  mod7_Ker2711 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(2),
      O => mod7_N271
    );
  mod7_Ker541 : LUT4_D
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => mod7_n0022,
      I1 => mod7_tp_17_Q,
      I2 => mod7_n0023,
      I3 => mod7_n0027,
      LO => N8602,
      O => mod7_N54
    );
  mod7_Ker41 : LUT3
    generic map(
      INIT => X"D7"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(0),
      I2 => mod0_pattern(1),
      O => mod7_N4
    );
  mod7_Ker2731 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(6),
      O => mod7_N273
    );
  mod7_n00711 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod7_N230,
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(2),
      I3 => mod7_active_line(9),
      O => mod7_n0071
    );
  mod7_n0316_3_Q : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_n0316(3),
      I1 => mod7_v_state(0),
      I2 => mod7_n0315_28_Q,
      O => mod7_MUX_BLOCK_N2
    );
  mod7_Ker2701 : LUT2
    generic map(
      INIT => X"D"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(6),
      O => mod7_N270
    );
  mod7_Ker2591 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(3),
      O => mod7_N259
    );
  mod7_Ker2171 : LUT2
    generic map(
      INIT => X"D"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(4),
      O => mod7_N217
    );
  mod7_Ker2521 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(2),
      O => mod7_N252
    );
  mod7_active_line_8 : FDC_1
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_active_line_n0000(8),
      CLR => mod5_hdframe_fvh_out(1),
      C => mod5_hdframe_fvh_out(0),
      Q => mod7_active_line(8)
    );
  mod7_Ker2871 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_N230,
      I1 => mod7_active_line(0),
      O => mod7_N287
    );
  mod5_hdframe_n0075_3_18 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_chroma_crc(12),
      O => CHOICE15442
    );
  mod7_Ker249184 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => CHOICE12504,
      I1 => CHOICE12509,
      I2 => CHOICE12520,
      I3 => CHOICE12553,
      O => mod7_N249
    );
  mod7_n0034_0_108 : LUT4
    generic map(
      INIT => X"8286"
    )
    port map (
      I0 => mod7_h_state_FFd1,
      I1 => mod7_h_state_FFd4,
      I2 => mod7_h_state_FFd3,
      I3 => mod7_h_state_FFd2,
      O => CHOICE14114
    );
  mod7_n0034_2_153 : LUT4
    generic map(
      INIT => X"BAAA"
    )
    port map (
      I0 => CHOICE14463,
      I1 => mod7_tp_17_Q,
      I2 => mod7_N154,
      I3 => mod7_n0023,
      O => CHOICE14467
    );
  mod7_Ker36 : LUT4
    generic map(
      INIT => X"EEAE"
    )
    port map (
      I0 => N4283,
      I1 => mod7_tp_21_Q,
      I2 => mod7_n0086,
      I3 => mod7_n0028,
      O => mod7_N36
    );
  mod7_Ker751 : LUT2
    generic map(
      INIT => X"D"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(7),
      O => mod7_N75
    );
  mod7_Ker2301 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod7_N262,
      I1 => mod7_active_line(4),
      I2 => mod7_active_line(8),
      I3 => mod7_N124,
      O => mod7_N230
    );
  mod7_n0045_0_98 : LUT4
    generic map(
      INIT => X"EEEF"
    )
    port map (
      I0 => N8169,
      I1 => CHOICE13398,
      I2 => mod7_active_line(2),
      I3 => mod7_active_line(6),
      O => CHOICE13400
    );
  mod7_Ker2861 : LUT3
    generic map(
      INIT => X"28"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(0),
      I2 => mod0_pattern(1),
      O => mod7_tp_17_Q
    );
  mod5_hdframe_samplecount_2_rt1_11 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(2),
      O => mod5_hdframe_samplecount_2_rt1
    );
  mod7_Ker2511 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(1),
      O => mod7_N251
    );
  mod7_n0046_3_152 : LUT4
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => mod7_n0229,
      I1 => N8396,
      I2 => mod7_active_line(1),
      I3 => mod7_active_line(5),
      O => CHOICE13465
    );
  mod5_hdframe_n0070_9_42_SW0 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => CHOICE15066,
      I1 => CHOICE15065,
      I2 => CHOICE15070,
      O => N8013
    );
  mod7_Ker2851 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(2),
      O => mod7_N285
    );
  mod7_h_state_FFd4_In63 : LUT4
    generic map(
      INIT => X"0880"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod5_hdframe_sav,
      I2 => mod0_pattern(0),
      I3 => mod0_pattern(1),
      O => CHOICE13991
    );
  mod7_Ker1901 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => mod7_h_state_FFd4,
      I2 => mod7_h_state_FFd3,
      I3 => mod7_tp_19_Q,
      O => mod7_N190
    );
  mod7_Ker2561 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(0),
      O => mod7_N256
    );
  mod7_Ker229412 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => CHOICE12564,
      I1 => CHOICE12576,
      I2 => CHOICE12601,
      I3 => CHOICE12670,
      O => mod7_N229
    );
  mod7_h_state_FFd2_In_12 : LUT4
    generic map(
      INIT => X"2AEA"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => CHOICE14018,
      I2 => CHOICE14005,
      I3 => N8235,
      O => mod7_h_state_FFd2_In
    );
  mod7_h_state_FFd4_In92 : LUT4
    generic map(
      INIT => X"FEEE"
    )
    port map (
      I0 => CHOICE13986,
      I1 => CHOICE13992,
      I2 => mod7_h_state_FFd4,
      I3 => CHOICE13995,
      O => mod7_h_state_FFd4_In
    );
  mod7_color_Ker1151 : LUT4_D
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod7_v_state(1),
      I1 => mod7_v_state(2),
      I2 => mod7_h_state_FFd1_3,
      I3 => mod7_h_state_FFd4_3,
      LO => N8603,
      O => mod7_color_N115
    );
  mod7_Ker8431 : LUT4
    generic map(
      INIT => X"AABA"
    )
    port map (
      I0 => CHOICE12384,
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(8),
      I3 => mod7_active_line(6),
      O => mod7_N84
    );
  mod7_active_line_2 : FDC_1
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_active_line_n0000(2),
      CLR => mod5_hdframe_fvh_out(1),
      C => mod5_hdframe_fvh_out(0),
      Q => mod7_active_line(2)
    );
  mod7_Ker2361 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(8),
      I2 => mod7_active_line(3),
      I3 => mod7_N185,
      O => mod7_N236
    );
  mod7_active_line_7 : FDC_1
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_active_line_n0000(7),
      CLR => mod5_hdframe_fvh_out(1),
      C => mod5_hdframe_fvh_out(0),
      Q => mod7_active_line(7)
    );
  mod7_n0034_2_46 : LUT4
    generic map(
      INIT => X"EEEF"
    )
    port map (
      I0 => mod7_N79,
      I1 => CHOICE14440,
      I2 => mod7_tp_17_Q,
      I3 => N8129,
      O => CHOICE14442
    );
  mod7_active_line_5 : FDC_1
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_active_line_n0000(5),
      CLR => mod5_hdframe_fvh_out(1),
      C => mod5_hdframe_fvh_out(0),
      Q => mod7_active_line(5)
    );
  mod7_Mmux_n0036_n0036_0_Q : MUXF6
    port map (
      I0 => mod7_MUX_BLOCK_tp_22_MUXF51,
      I1 => mod7_MUX_BLOCK_tp_22_MUXF5,
      S => mod5_field2_vblnk1_5_Q,
      O => mod7_n0036(0)
    );
  mod7_Ker105 : LUT4
    generic map(
      INIT => X"FA72"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(1),
      I2 => N124,
      I3 => N125,
      O => mod7_N105
    );
  mod7_Ker3566 : LUT4
    generic map(
      INIT => X"FF46"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_active_line(7),
      I2 => mod7_active_line(1),
      I3 => CHOICE12403,
      O => mod7_N35
    );
  mod5_hdframe_n0070_5_153 : LUT4
    generic map(
      INIT => X"C080"
    )
    port map (
      I0 => mod0_tp_option(1),
      I1 => mod7_color_N121,
      I2 => mod7_color_N115,
      I3 => mod0_tp_option(0),
      O => CHOICE15417
    );
  mod7_Ker2531 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(5),
      O => mod7_N253
    );
  mod7_Ker229135_SW0_SW0 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(5),
      O => N8472
    );
  mod7_n0047_1_54 : LUT4
    generic map(
      INIT => X"EFFE"
    )
    port map (
      I0 => mod7_N106,
      I1 => CHOICE13753,
      I2 => mod7_active_line(9),
      I3 => mod7_active_line(2),
      O => CHOICE13758
    );
  mod7_n0046_3_960_SW0 : LUT4
    generic map(
      INIT => X"E4F4"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(8),
      I2 => mod7_active_line(4),
      I3 => mod7_N133,
      O => N8185
    );
  mod7_n01221 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(5),
      O => mod7_n0122
    );
  mod7_n0316_3_15 : LUT4
    generic map(
      INIT => X"44E4"
    )
    port map (
      I0 => mod7_n0316(3),
      I1 => mod7_v_state(3),
      I2 => CHOICE13913,
      I3 => mod7_n0071,
      O => mod7_MUX_BLOCK_N17
    );
  mod7_h_state_Out101 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => mod7_h_state_FFd1,
      I1 => mod7_h_state_FFd4,
      I2 => mod7_h_state_FFd3,
      O => mod7_n0031
    );
  mod7_video_sm_active_line_n0000_10_xor : XORCY
    port map (
      CI => mod7_video_sm_active_line_n0000_9_cyo,
      LI => mod7_active_line_10_rt,
      O => mod7_active_line_n0000(10)
    );
  mod7_h_state_FFd1_13 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd1_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd1
    );
  mod7_n0034_3_134 : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => mod7_n0025,
      I1 => mod7_tp_17_Q,
      I2 => CHOICE14363,
      O => CHOICE14364
    );
  mod7_video_sm_active_line_n0000_8_cy : MUXCY
    port map (
      CI => mod7_video_sm_active_line_n0000_7_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_active_line_8_rt,
      O => mod7_video_sm_active_line_n0000_8_cyo
    );
  mod7_Ker1151 : LUT4
    generic map(
      INIT => X"FFEF"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_N137,
      I2 => mod7_active_line(3),
      I3 => mod7_active_line(6),
      O => mod7_N115
    );
  mod7_Ker1821 : LUT3
    generic map(
      INIT => X"DF"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_pattern(2),
      I2 => mod0_pattern(0),
      O => mod7_N182
    );
  mod7_Ker1321 : LUT4
    generic map(
      INIT => X"66F6"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(4),
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(1),
      O => mod7_N132
    );
  mod7_n0044_3_1047_SW1 : LUT4
    generic map(
      INIT => X"8F88"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(6),
      I3 => mod7_active_line(5),
      O => N8433
    );
  mod7_Ker1541 : LUT4
    generic map(
      INIT => X"FBFF"
    )
    port map (
      I0 => mod7_v_state(2),
      I1 => mod0_pattern(1),
      I2 => mod0_pattern(2),
      I3 => mod0_pattern(0),
      O => mod7_N154
    );
  mod7_Ker2571 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_v_state(2),
      I1 => mod0_pattern(1),
      I2 => mod0_pattern(0),
      I3 => mod0_pattern(2),
      O => mod7_N257
    );
  mod7_Ker1331 : LUT2
    generic map(
      INIT => X"D"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(1),
      O => mod7_N133
    );
  mod7_n0034_7_50_SW0 : LUT4_L
    generic map(
      INIT => X"1333"
    )
    port map (
      I0 => mod7_N237,
      I1 => mod7_n0024,
      I2 => mod7_h_state_FFd3,
      I3 => mod7_tp_17_Q,
      LO => N8464
    );
  mod7_Ker1201 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => CHOICE14459,
      I1 => mod7_h_state_FFd4,
      I2 => mod7_h_state_FFd2,
      I3 => mod7_h_state_FFd3,
      O => mod7_N120
    );
  mod5_hdframe_XNor_stagelut54 : LUT4_L
    generic map(
      INIT => X"5666"
    )
    port map (
      I0 => mod5_hdframe_samplecount(9),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(2),
      I3 => mod0_hd_format_tmp(1),
      LO => mod5_hdframe_N221
    );
  mod7_n0044_3_198_SW2 : LUT4
    generic map(
      INIT => X"F5F4"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_active_line(8),
      I2 => CHOICE13120,
      I3 => mod7_active_line(3),
      O => N8446
    );
  mod7_Ker2821 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(1),
      O => mod7_N282
    );
  mod7_Ker1241 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(7),
      O => mod7_N124
    );
  mod7_Mmux_n0036_n0036_0_n0036_0_rn_1 : MUXF6
    port map (
      I0 => mod7_MUX_BLOCK_tp_22_MUXF57,
      I1 => mod7_MUX_BLOCK_tp_22_MUXF56,
      S => mod5_field2_vblnk1_5_Q,
      O => mod7_n0036(3)
    );
  mod7_tp_22_rn_6 : MUXF5
    port map (
      I0 => mod7_MUX_BLOCK_N17,
      I1 => mod7_MUX_BLOCK_N16,
      S => mod7_tp_22_Q,
      O => mod7_MUX_BLOCK_tp_22_MUXF57
    );
  mod7_Ker2311 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_N295,
      I1 => mod7_N75,
      I2 => mod7_active_line(1),
      I3 => mod7_active_line(6),
      O => mod7_N231
    );
  mod7_n00991 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(8),
      O => mod7_n0099
    );
  mod5_hdframe_hd_framegenerator_n0088_11_lut3 : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(3),
      O => N7974
    );
  mod5_hdframe_hd_framegenerator_n0083_9_lut1 : LUT4
    generic map(
      INIT => X"0025"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(3),
      O => N7980
    );
  mod7_load_val_2 : FDC
    port map (
      D => mod7_n0034(2),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_load_val(2)
    );
  mod7_load_val_3 : FDC
    port map (
      D => mod7_n0034(3),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_load_val(3)
    );
  mod7_load_val_4 : FDC
    port map (
      D => mod7_n0034(4),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_load_val(4)
    );
  mod7_v_state_0 : FDP_1
    generic map(
      INIT => '1'
    )
    port map (
      D => mod7_n0036(0),
      PRE => mod7_tp_20_Q,
      C => mod5_hdframe_line_clk,
      Q => mod7_v_state(0)
    );
  mod7_n0046_2_170 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => CHOICE12987,
      I1 => mod7_N277,
      I2 => N8161,
      I3 => mod7_v_state(2),
      O => mod7_n0315_14_Q
    );
  mod7_Ker22999_SW0 : LUT3
    generic map(
      INIT => X"6E"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(5),
      O => N8281
    );
  mod7_vstate_1_1 : LUT3
    generic map(
      INIT => X"F1"
    )
    port map (
      I0 => mod7_v_state(0),
      I1 => mod7_v_state(1),
      I2 => mod7_v_state(2),
      O => mod7_tp_5_Q
    );
  mod7_active_line_9 : FDC_1
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_active_line_n0000(9),
      CLR => mod5_hdframe_fvh_out(1),
      C => mod5_hdframe_fvh_out(0),
      Q => mod7_active_line(9)
    );
  mod7_Ker249100_SW0 : LUT4
    generic map(
      INIT => X"44C4"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(8),
      I2 => mod7_active_line(1),
      I3 => mod7_active_line(5),
      O => N8439
    );
  mod7_video_sm_active_line_n0000_8_xor : XORCY
    port map (
      CI => mod7_video_sm_active_line_n0000_7_cyo,
      LI => mod7_active_line_8_rt,
      O => mod7_active_line_n0000(8)
    );
  mod7_n00861 : LUT4_D
    generic map(
      INIT => X"FFEF"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => mod7_h_state_FFd3,
      I2 => mod7_h_state_FFd1,
      I3 => mod7_h_state_FFd4,
      LO => N8604,
      O => mod7_n0086
    );
  mod7_Ker2791 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(5),
      O => mod7_N279
    );
  mod7_Ker2781 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => mod7_active_line(1),
      O => mod7_N278
    );
  mod7_Ker2771 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(2),
      O => mod7_N277
    );
  mod0_bitptr_3 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod0_bitptr_n0000(3),
      CLR => CS_IBUF,
      C => SCK_BUFGP,
      Q => mod0_bitptr(3)
    );
  mod7_n0046_3_1233 : LUT4
    generic map(
      INIT => X"5540"
    )
    port map (
      I0 => mod7_n0071,
      I1 => CHOICE13558,
      I2 => mod7_v_state(3),
      I3 => CHOICE13736,
      O => mod7_n0315_15_Q
    );
  mod7_active_line_3 : FDC_1
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_active_line_n0000(3),
      CLR => mod5_hdframe_fvh_out(1),
      C => mod5_hdframe_fvh_out(0),
      Q => mod7_active_line(3)
    );
  mod7_Ker2421 : LUT3
    generic map(
      INIT => X"04"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(8),
      I2 => mod7_active_line(7),
      O => mod7_N242
    );
  mod7_n00791 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(1),
      I2 => mod7_N232,
      I3 => mod7_active_line(2),
      O => mod7_n0079
    );
  mod7_Ker2351 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod7_N293,
      I1 => mod7_active_line(6),
      I2 => mod7_active_line(3),
      I3 => mod7_active_line(5),
      O => mod7_N235
    );
  mod7_n00781 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(1),
      I2 => mod7_N232,
      I3 => mod7_active_line(6),
      O => mod7_n0078
    );
  mod7_n00751 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_N251,
      I1 => mod7_active_line(5),
      I2 => mod7_N236,
      I3 => mod7_active_line(7),
      O => mod7_n0075
    );
  mod7_n0034_4_130_SW0_SW0 : LUT4_L
    generic map(
      INIT => X"FF8B"
    )
    port map (
      I0 => mod7_v_state(2),
      I1 => mod7_tp_19_Q,
      I2 => mod7_tp_21_Q,
      I3 => mod7_h_state_FFd1,
      LO => N8377
    );
  mod5_hdframe_n0070_9_89_SW1 : LUT4
    generic map(
      INIT => X"F0FB"
    )
    port map (
      I0 => mod5_hdframe_luma_crc(17),
      I1 => mod5_hdframe_n0100,
      I2 => mod5_hdframe_n0116,
      I3 => mod5_hdframe_N881,
      O => N8024
    );
  mod7_Ker57 : LUT4
    generic map(
      INIT => X"FEEE"
    )
    port map (
      I0 => N8646,
      I1 => N4514,
      I2 => mod7_n0027,
      I3 => mod7_tp_21_Q,
      O => mod7_N57
    );
  mod7_active_line_1 : FDC_1
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_active_line_n0000(1),
      CLR => mod5_hdframe_fvh_out(1),
      C => mod5_hdframe_fvh_out(0),
      Q => mod7_active_line(1)
    );
  mod7_n0047_2_114 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE13018,
      I1 => mod7_v_state(2),
      I2 => CHOICE12993,
      I3 => CHOICE13006,
      O => mod7_n0315_6_Q
    );
  mod7_n00691 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(2),
      I3 => N8486,
      O => mod7_n0069
    );
  mod7_active_line_0 : FDC_1
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_N3,
      CLR => mod5_hdframe_fvh_out(1),
      C => mod5_hdframe_fvh_out(0),
      Q => mod7_active_line(0)
    );
  mod7_n00671 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => mod7_N235,
      I1 => mod7_active_line(4),
      I2 => mod7_active_line(8),
      I3 => mod7_N278,
      O => mod7_n0067
    );
  mod7_n00661 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => mod7_N240,
      I1 => mod7_N271,
      I2 => mod7_active_line(3),
      I3 => mod7_N304,
      O => mod7_n0066
    );
  mod7_n00651 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => mod7_N231,
      I2 => mod7_active_line(4),
      I3 => mod7_active_line(2),
      O => mod7_n0065
    );
  mod7_Ker2601 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(1),
      O => mod7_N260
    );
  mod7_Ker2021 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(2),
      O => mod7_N202
    );
  mod7_n00761 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_N302,
      I1 => mod7_active_line(1),
      I2 => mod7_N236,
      I3 => mod7_active_line(2),
      O => mod7_n0076
    );
  mod7_Ker2411 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(3),
      O => mod7_N241
    );
  mod7_ff_cb1 : LUT3
    generic map(
      INIT => X"27"
    )
    port map (
      I0 => mod0_pattern(0),
      I1 => mod0_pattern(1),
      I2 => mod0_pattern(2),
      O => mod7_tp_20_Q
    );
  mod7_load_val_6 : FDC
    port map (
      D => mod7_n0034(6),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_load_val(6)
    );
  mod7_h_state_Out141 : LUT4_D
    generic map(
      INIT => X"AADA"
    )
    port map (
      I0 => mod7_h_state_FFd3_2,
      I1 => mod7_h_state_FFd2_2,
      I2 => mod7_h_state_FFd1_2,
      I3 => mod7_h_state_FFd4_2,
      LO => N8605,
      O => mod7_tp_1_Q
    );
  mod7_load_val_0 : FDC
    port map (
      D => mod7_n0034(0),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_load_val(0)
    );
  mod7_load_val_1 : FDC
    port map (
      D => mod7_n0034(1),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_load_val(1)
    );
  mod7_vstate_0_1 : LUT3
    generic map(
      INIT => X"F1"
    )
    port map (
      I0 => mod7_v_state(0),
      I1 => mod7_v_state(2),
      I2 => mod7_v_state(1),
      O => mod7_tp_4_Q
    );
  mod7_tp_22_2 : LUT4
    generic map(
      INIT => X"0155"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(0),
      I2 => mod0_hd_format_tmp(1),
      I3 => mod0_hd_format_tmp(2),
      O => mod7_tp_22_Q
    );
  mod5_hdframe_n0075_1_50 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE14607,
      I1 => mod5_hdframe_samplecount(0),
      I2 => CHOICE14596,
      I3 => CHOICE14597,
      O => CHOICE14608
    );
  mod7_Ker79_G : LUT3_L
    generic map(
      INIT => X"5D"
    )
    port map (
      I0 => mod7_n0086,
      I1 => mod7_n0030,
      I2 => mod7_N227,
      LO => N8519
    );
  mod0_bitptr_2 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod0_bitptr_n0000(2),
      CLR => CS_IBUF,
      C => SCK_BUFGP,
      Q => mod0_bitptr(2)
    );
  mod5_hdframe_norlut18 : LUT2_L
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => mod5_hdframe_samplecount(6),
      I1 => mod5_hdframe_samplecount(11),
      LO => mod5_hdframe_N200
    );
  mod5_hdframe_n0070_9_52 : LUT4
    generic map(
      INIT => X"FFEC"
    )
    port map (
      I0 => mod7_color_yramp(9),
      I1 => mod7_color_N82,
      I2 => CHOICE15061,
      I3 => N8013,
      O => CHOICE15074
    );
  mod5_Mrom_n0003_inst_lut4_501 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(1),
      O => mod5_field1_vblnk1_0_Q
    );
  mod5_hdframe_XNor_stagelut46 : LUT4
    generic map(
      INIT => X"5666"
    )
    port map (
      I0 => mod5_hdframe_samplecount(9),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(2),
      I3 => mod0_hd_format_tmp(1),
      O => mod5_hdframe_N156
    );
  mod5_Mrom_n0003_inst_lut4_511 : LUT4
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => mod0_hd_format_tmp(1),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(2),
      O => mod5_field1_vblnk1_1_Q
    );
  mod5_hdframe_n0075_6_85 : LUT4_L
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => N8033,
      I1 => mod7_color_N28,
      I2 => mod7_color_N79,
      I3 => mod7_color_N39,
      LO => CHOICE14783
    );
  mod7_n0044_3_838 : LUT4
    generic map(
      INIT => X"32FF"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(6),
      I2 => mod7_active_line(7),
      I3 => mod7_active_line(0),
      O => CHOICE13302
    );
  mod5_hdframe_n0070_6_59 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_luma_crc(16),
      I2 => mod5_hdframe_n0175(0),
      I3 => mod5_hdframe_n0173(0),
      O => CHOICE14924
    );
  mod5_hdframe_n0075_9_388 : LUT4
    generic map(
      INIT => X"0702"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_chroma_crc(17),
      I2 => mod5_hdframe_n0116,
      I3 => CHOICE14878,
      O => CHOICE14883
    );
  mod5_hdframe_linecount_5_rt1_14 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_linecount(5),
      O => mod5_hdframe_linecount_5_rt1
    );
  mod7_n0034_3_44 : LUT4
    generic map(
      INIT => X"F5F4"
    )
    port map (
      I0 => mod7_N227,
      I1 => mod7_n0030,
      I2 => mod7_N36,
      I3 => mod7_n0028,
      O => CHOICE14346
    );
  mod5_asl_7_1 : LUT3
    generic map(
      INIT => X"15"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(2),
      I2 => mod0_hd_format_tmp(1),
      O => mod5_asl(7)
    );
  mod5_hdframe_Eq_stagelut31 : LUT4_L
    generic map(
      INIT => X"1411"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => mod5_hdframe_samplecount(1),
      I2 => sif_hd_format(0),
      I3 => sif_hd_format(2),
      LO => mod5_hdframe_N146
    );
  mod5_hdframe_n0070_5_174 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE15412,
      I1 => mod7_color_N128,
      I2 => N8085,
      I3 => CHOICE15398,
      O => CHOICE15423
    );
  mod7_n0316_3_1 : LUT4
    generic map(
      INIT => X"F8B8"
    )
    port map (
      I0 => mod7_N24,
      I1 => mod7_n0316(3),
      I2 => mod7_v_state(0),
      I3 => CHOICE13400,
      O => mod7_MUX_BLOCK_N3
    );
  mod0_tp_option_11 : LUT3_L
    generic map(
      INIT => X"AC"
    )
    port map (
      I0 => MOSI_IBUF,
      I1 => mod0_tp_option(1),
      I2 => mod0_n0047,
      LO => N8077
    );
  mod5_hdframe_n0075_9_278 : LUT4
    generic map(
      INIT => X"FF32"
    )
    port map (
      I0 => mod7_color_N78,
      I1 => mod7_tp_4_Q,
      I2 => CHOICE14864,
      I3 => N8593,
      O => CHOICE14873
    );
  mod7_n0034_5_101 : LUT4_L
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_N7,
      I1 => mod7_h_state_FFd1,
      I2 => mod7_h_state_FFd3,
      I3 => mod7_h_state_FFd4,
      LO => CHOICE14406
    );
  mod5_hdframe_n0075_9_33 : LUT4_L
    generic map(
      INIT => X"FFEF"
    )
    port map (
      I0 => mod7_h_state_FFd2_5,
      I1 => CHOICE14810,
      I2 => mod7_tp_1_Q,
      I3 => mod7_tp_0_Q,
      LO => CHOICE14813
    );
  mod0_bitptr_1 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod0_bitptr_n0000(1),
      CLR => CS_IBUF,
      C => SCK_BUFGP,
      Q => mod0_bitptr(1)
    );
  mod5_hdframe_samplecount_0_1_15 : FDCP
    port map (
      D => mod5_hdframe_samplecount_0_0_n0000,
      CLR => mod7_h_state_FFd1_N0,
      PRE => mod5_field1_vblnk1_4_Q,
      C => txclka2,
      Q => mod5_hdframe_samplecount_0_1
    );
  mod5_hdframe_n0070_6_193_SW0 : LUT4_L
    generic map(
      INIT => X"FFA6"
    )
    port map (
      I0 => mod7_h_state_FFd3,
      I1 => mod7_h_state_FFd1,
      I2 => mod7_h_state_FFd4,
      I3 => N8373,
      LO => N8245
    );
  mod7_n0047_3_141_SW0 : LUT3
    generic map(
      INIT => X"8A"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(9),
      I2 => mod7_active_line(7),
      O => N8297
    );
  mod7_color_Ker3752 : LUT4_D
    generic map(
      INIT => X"FFA8"
    )
    port map (
      I0 => mod7_color_v1h8_y(9),
      I1 => mod7_color_n0020,
      I2 => mod7_color_n0036,
      I3 => CHOICE15284,
      LO => N8606,
      O => mod7_color_N37
    );
  mod0_bitptr_0 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod0_N3,
      CLR => CS_IBUF,
      C => SCK_BUFGP,
      Q => mod0_bitptr(0)
    );
  mod5_hdframe_n0075_0_7 : LUT4_L
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod7_color_N126,
      I1 => mod7_color_n0019,
      I2 => mod7_color_n0018,
      I3 => mod7_color_n0023,
      LO => CHOICE15028
    );
  mod5_hdframe_n0070_8_88 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => mod7_color_N106,
      I1 => CHOICE14576,
      I2 => CHOICE14581,
      I3 => mod7_color_N40,
      O => CHOICE14583
    );
  mod7_n0034_3_210 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => N8255,
      I1 => mod7_tp_21_Q,
      I2 => CHOICE14358,
      I3 => CHOICE14364,
      O => CHOICE14378
    );
  mod5_hdframe_n0074_3_1 : LUT4
    generic map(
      INIT => X"0220"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_n0179(0),
      I3 => mod5_hdframe_luma_crc(13),
      O => mod5_hdframe_n0074(3)
    );
  mod7_n0034_2_191_SW0 : LUT4
    generic map(
      INIT => X"BAAA"
    )
    port map (
      I0 => mod7_n0022,
      I1 => mod7_h_state_FFd4,
      I2 => mod7_h_state_FFd2,
      I3 => mod7_tp_17_Q,
      O => N8462
    );
  mod5_hdframe_n0070_9_170 : LUT4_L
    generic map(
      INIT => X"2882"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_n0177(1),
      I2 => mod5_hdframe_n0171(0),
      I3 => mod5_hdframe_n0179(0),
      LO => CHOICE15096
    );
  mod5_hdframe_n0070_5_67 : LUT4_L
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_color_N115,
      I2 => mod7_color_N122,
      I3 => mod7_color_n0023,
      LO => CHOICE15397
    );
  mod7_n0034_3_118 : LUT3
    generic map(
      INIT => X"A8"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => CHOICE14872,
      I2 => CHOICE14356,
      O => CHOICE14358
    );
  mod7_color_Ker2211 : LUT4_D
    generic map(
      INIT => X"FAF8"
    )
    port map (
      I0 => mod7_color_v0h2_y_2_Q,
      I1 => mod7_color_n0015,
      I2 => mod7_color_N97,
      I3 => mod7_color_n0012,
      LO => N8607,
      O => CHOICE15342
    );
  mod7_h_state_FFd1_In_G : LUT4
    generic map(
      INIT => X"EAAA"
    )
    port map (
      I0 => mod7_h_state_FFd1,
      I1 => mod7_h_state_FFd2,
      I2 => mod7_h_state_FFd3,
      I3 => mod7_n0052,
      O => N8521
    );
  mod5_hdframe_n0075_9_16 : LUT4
    generic map(
      INIT => X"BFFF"
    )
    port map (
      I0 => mod0_tp_option(0),
      I1 => mod0_tp_option(1),
      I2 => mod0_pattern(1),
      I3 => mod0_pattern(2),
      O => CHOICE14810
    );
  mod5_hdframe_n0070_3_150 : LUT4_L
    generic map(
      INIT => X"5510"
    )
    port map (
      I0 => mod5_hdframe_n0115,
      I1 => mod5_hdframe_n0116,
      I2 => CHOICE15158,
      I3 => CHOICE15137,
      LO => CHOICE15163
    );
  mod5_hdframe_n0070_0_65 : LUT4_L
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => mod7_color_N98,
      I1 => mod7_color_N101,
      I2 => mod7_color_N129,
      I3 => CHOICE14675,
      LO => CHOICE14678
    );
  mod5_hdframe_n0070_6_469_SW1 : LUT4
    generic map(
      INIT => X"888C"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => mod7_tp_5_Q,
      I2 => CHOICE14942,
      I3 => N8649,
      O => N8079
    );
  mod0_serial_interface_bitptr_n0000_5_xor : XORCY
    port map (
      CI => mod0_serial_interface_bitptr_n0000_4_cyo,
      LI => mod0_bitptr_5_rt,
      O => mod0_bitptr_n0000(5)
    );
  mod7_n0034_2_191 : LUT4
    generic map(
      INIT => X"FEEE"
    )
    port map (
      I0 => N8643,
      I1 => CHOICE14472,
      I2 => mod7_v_state(3),
      I3 => N8462,
      O => CHOICE14477
    );
  mod5_hdframe_samplecount_11_rt_16 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(11),
      O => mod5_hdframe_samplecount_11_rt
    );
  mod5_hdframe_n0070_0_16_SW0 : LUT4_L
    generic map(
      INIT => X"FFEF"
    )
    port map (
      I0 => mod7_tp_5_Q,
      I1 => mod7_h_state_FFd2,
      I2 => mod7_color_N135,
      I3 => mod7_tp_1_Q,
      LO => N8490
    );
  mod5_hdframe_n0075_5_60 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_chroma_crc(15),
      I2 => mod5_hdframe_n0193(0),
      I3 => mod5_hdframe_n0193(1),
      O => CHOICE15212
    );
  mod0_bitptr_5 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod0_bitptr_n0000(5),
      CLR => CS_IBUF,
      C => SCK_BUFGP,
      Q => mod0_bitptr(5)
    );
  mod7_n0034_3_69 : LUT4_L
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => mod7_N57,
      I1 => CHOICE14339,
      I2 => CHOICE14342,
      I3 => CHOICE14346,
      LO => CHOICE14349
    );
  mod7_Ker2512 : LUT4
    generic map(
      INIT => X"5300"
    )
    port map (
      I0 => mod7_tp_21_Q,
      I1 => mod7_tp_19_Q,
      I2 => mod7_tp_17_Q,
      I3 => N8579,
      O => CHOICE14059
    );
  mod5_hdframe_n0079_14_1 : LUT4
    generic map(
      INIT => X"0220"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_chroma_crc(6),
      I3 => mod5_hdframe_chroma_out(6),
      O => mod5_hdframe_n0079(14)
    );
  mod7_n0034_5_112_G : LUT4_L
    generic map(
      INIT => X"028A"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => mod7_h_state_FFd4,
      I2 => mod7_N257,
      I3 => mod7_tp_19_Q,
      LO => N8523
    );
  mod7_color_color_lut_yramp_n0000_4_cy : MUXCY
    port map (
      CI => mod7_color_color_lut_yramp_n0000_3_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_color_yramp_4_rt,
      O => mod7_color_color_lut_yramp_n0000_4_cyo
    );
  mod5_hdframe_n0075_2_28_SW0 : LUT4_L
    generic map(
      INIT => X"5777"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod7_color_N117,
      I2 => mod7_color_n0023,
      I3 => mod7_color_N126,
      LO => N8133
    );
  mod7_color_v1h2_cb_8_Q_17 : LUT4
    generic map(
      INIT => X"0828"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_pattern(0),
      I2 => mod0_pattern(2),
      I3 => N117,
      O => mod7_color_v1h2_cb_8_Q
    );
  mod0_hd_format_1_1 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => mod0_hd_format_tmp(1),
      I1 => mod0_hd_format_tmp(3),
      O => sif_hd_format(1)
    );
  mod7_n00611 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod7_N230,
      I1 => mod7_active_line(0),
      I2 => mod7_active_line(1),
      I3 => mod7_active_line(2),
      O => mod7_n0061
    );
  mod5_hdframe_Eq_stagelut12 : LUT4_L
    generic map(
      INIT => X"07F8"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(3),
      I3 => mod5_hdframe_linecount(10),
      LO => mod5_hdframe_N126
    );
  mod0_serial_interface_bitptr_n0000_0_lut_INV_0 : INV
    port map (
      I => mod0_bitptr(0),
      O => mod0_N3
    );
  mod7_color_Ker1351 : LUT4
    generic map(
      INIT => X"F100"
    )
    port map (
      I0 => mod7_v_state(2),
      I1 => mod7_v_state(0),
      I2 => mod7_v_state(1),
      I3 => mod7_tp_0_Q,
      O => mod7_color_N135
    );
  mod7_color_Ker1401 : LUT4
    generic map(
      INIT => X"000E"
    )
    port map (
      I0 => mod7_v_state(0),
      I1 => mod7_v_state(2),
      I2 => mod7_v_state(1),
      I3 => N8587,
      O => mod7_color_N140
    );
  mod7_color_n00121 : LUT4_D
    generic map(
      INIT => X"1000"
    )
    port map (
      I0 => mod7_tp_5_Q,
      I1 => mod7_h_state_FFd1,
      I2 => mod7_color_N122,
      I3 => mod7_color_N140,
      LO => N8608,
      O => mod7_color_n0012
    );
  mod7_counter_state_counter_n0009_17_lut_INV_0 : INV
    port map (
      I => mod7_counter_count(6),
      O => mod7_counter_N8
    );
  mod7_counter_state_counter_n0009_16_xor : XORCY
    port map (
      CI => mod7_counter_state_counter_n0009_15_cyo,
      LI => mod7_counter_N7,
      O => mod7_counter_n0009(16)
    );
  mod7_counter_state_counter_n0009_16_cy : MUXCY
    port map (
      CI => mod7_counter_state_counter_n0009_15_cyo,
      DI => mod5_tsl_4_Q,
      S => mod7_counter_N7,
      O => mod7_counter_state_counter_n0009_16_cyo
    );
  mod7_counter_state_counter_n0009_16_lut_INV_0 : INV
    port map (
      I => mod7_counter_count(5),
      O => mod7_counter_N7
    );
  mod7_color_color_lut_yramp_n0000_1_cy : MUXCY
    port map (
      CI => mod7_color_color_lut_yramp_n0000_0_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_color_yramp_1_rt,
      O => mod7_color_color_lut_yramp_n0000_1_cyo
    );
  mod7_counter_state_counter_n0009_15_cy : MUXCY
    port map (
      CI => mod7_counter_state_counter_n0009_14_cyo,
      DI => mod5_tsl_4_Q,
      S => mod7_counter_N6,
      O => mod7_counter_state_counter_n0009_15_cyo
    );
  mod7_counter_state_counter_n0009_15_lut_INV_0 : INV
    port map (
      I => mod7_counter_count(4),
      O => mod7_counter_N6
    );
  mod7_counter_state_counter_n0009_14_xor : XORCY
    port map (
      CI => mod7_counter_state_counter_n0009_13_cyo,
      LI => mod7_counter_N5,
      O => mod7_counter_n0009(14)
    );
  mod7_color_Ker106 : LUT4_D
    generic map(
      INIT => X"80CC"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_color_N115,
      I2 => mod7_color_N122,
      I3 => N8241,
      LO => N8609,
      O => mod7_color_N106
    );
  mod7_color_Ker291 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_color_n0019,
      I2 => N5872,
      I3 => N8045,
      O => mod7_color_N29
    );
  mod7_color_v1h9_y_5_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(0),
      I2 => mod0_pattern(1),
      O => mod7_color_v1h9_y(5)
    );
  mod7_color_Ker1301 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => mod0_pattern(0),
      I1 => mod0_pattern(2),
      O => CHOICE15007
    );
  mod7_color_color_lut_yramp_n0000_3_xor : XORCY
    port map (
      CI => mod7_color_color_lut_yramp_n0000_2_cyo,
      LI => mod7_color_yramp_3_rt,
      O => mod7_color_yramp_n0000(3)
    );
  mod7_color_Ker1001 : LUT4_D
    generic map(
      INIT => X"BA30"
    )
    port map (
      I0 => mod7_color_v0h2_y_0_Q,
      I1 => mod7_tp_1_Q,
      I2 => mod7_color_N118,
      I3 => mod7_color_n0014,
      LO => N8610,
      O => mod7_color_N100
    );
  mod7_color_Ker191 : LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      I0 => mod0_pattern(0),
      I1 => mod0_pattern(1),
      O => mod7_color_N19
    );
  mod7_Ker2911 : LUT4_D
    generic map(
      INIT => X"6000"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_pattern(0),
      I2 => mod0_pattern(2),
      I3 => mod7_n0024,
      LO => N8611,
      O => mod7_N291
    );
  mod7_counter_state_counter_n0009_20_xor : XORCY
    port map (
      CI => mod7_counter_state_counter_n0009_19_cyo,
      LI => mod7_counter_N11,
      O => mod7_counter_n0009(20)
    );
  mod7_color_Ker1231 : LUT4
    generic map(
      INIT => X"0004"
    )
    port map (
      I0 => mod7_tp_5_Q,
      I1 => mod7_h_state_FFd1_4,
      I2 => mod7_h_state_FFd2_4,
      I3 => N8605,
      O => mod7_color_N123
    );
  mod7_color_color_lut_yramp_n0000_1_xor : XORCY
    port map (
      CI => mod7_color_color_lut_yramp_n0000_0_cyo,
      LI => mod7_color_yramp_1_rt,
      O => mod7_color_yramp_n0000(1)
    );
  mod7_Ker2751 : LUT4
    generic map(
      INIT => X"0880"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod7_v_state(3),
      I2 => mod0_pattern(0),
      I3 => mod0_pattern(1),
      O => CHOICE14459
    );
  mod7_color_Ker1201 : LUT4_D
    generic map(
      INIT => X"88A8"
    )
    port map (
      I0 => mod7_h_state_FFd2_3,
      I1 => mod7_h_state_FFd3_3,
      I2 => mod7_h_state_FFd1_3,
      I3 => mod7_h_state_FFd4_3,
      LO => N8612,
      O => mod7_color_N120
    );
  mod7_counter_state_counter_n0009_17_cy : MUXCY
    port map (
      CI => mod7_counter_state_counter_n0009_16_cyo,
      DI => mod5_tsl_4_Q,
      S => mod7_counter_N8,
      O => mod7_counter_state_counter_n0009_17_cyo
    );
  TP4_OBUF_18 : OBUF
    port map (
      I => TP4_OBUF,
      O => TP4
    );
  mod7_color_color_lut_yramp_n0000_3_cy : MUXCY
    port map (
      CI => mod7_color_color_lut_yramp_n0000_2_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_color_yramp_3_rt,
      O => mod7_color_color_lut_yramp_n0000_3_cyo
    );
  mod0_hd_format_0_1 : LUT4
    generic map(
      INIT => X"020A"
    )
    port map (
      I0 => mod0_hd_format_tmp(0),
      I1 => mod0_hd_format_tmp(2),
      I2 => mod0_hd_format_tmp(3),
      I3 => mod0_hd_format_tmp(1),
      O => sif_hd_format(0)
    );
  mod7_color_Ker1221 : LUT4_D
    generic map(
      INIT => X"2030"
    )
    port map (
      I0 => mod7_h_state_FFd4_2,
      I1 => mod7_h_state_FFd3_3,
      I2 => mod7_h_state_FFd2_3,
      I3 => mod7_h_state_FFd1_3,
      LO => N8613,
      O => mod7_color_N122
    );
  mod7_color_color_lut_yramp_n0000_0_lut_INV_0 : INV
    port map (
      I => mod7_color_yramp(0),
      O => mod7_color_N3
    );
  mod7_color_color_lut_yramp_n0000_2_xor : XORCY
    port map (
      CI => mod7_color_color_lut_yramp_n0000_1_cyo,
      LI => mod7_color_yramp_2_rt,
      O => mod7_color_yramp_n0000(2)
    );
  mod7_color_color_lut_yramp_n0000_7_xor : XORCY
    port map (
      CI => mod7_color_color_lut_yramp_n0000_6_cyo,
      LI => mod7_color_yramp_7_rt,
      O => mod7_color_yramp_n0000(7)
    );
  mod7_counter_state_counter_n0009_20_cy : MUXCY
    port map (
      CI => mod7_counter_state_counter_n0009_19_cyo,
      DI => mod5_tsl_4_Q,
      S => mod7_counter_N11,
      O => mod7_counter_state_counter_n0009_20_cyo
    );
  mod7_color_Ker1011 : LUT4_D
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod7_color_v1h4_y(9),
      I2 => mod7_color_n0023,
      I3 => mod0_tp_option(2),
      LO => N8614,
      O => mod7_color_N101
    );
  mod7_color_v1h2_cb_6_Q_19 : LUT4
    generic map(
      INIT => X"0828"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_pattern(0),
      I2 => mod0_pattern(2),
      I3 => N119,
      O => mod7_color_v1h2_cb_6_Q
    );
  mod0_new_delay_N01_INV_0 : INV
    port map (
      I => reset_n_IBUF,
      O => mod0_new_delay_N0
    );
  mod5_hdframe_Eq_stagelut14 : LUT3_L
    generic map(
      INIT => X"09"
    )
    port map (
      I0 => mod5_field2_vblnk1_5_Q,
      I1 => mod5_hdframe_linecount(3),
      I2 => mod5_hdframe_linecount(2),
      LO => mod5_hdframe_N128
    );
  mod7_counter_state_counter_n0009_21_lut_INV_0 : INV
    port map (
      I => mod7_counter_count(10),
      O => mod7_counter_N12
    );
  mod7_color_color_lut_yramp_n0000_5_xor : XORCY
    port map (
      CI => mod7_color_color_lut_yramp_n0000_4_cyo,
      LI => mod7_color_yramp_5_rt,
      O => mod7_color_yramp_n0000(5)
    );
  mod7_color_Ker281 : LUT4_D
    generic map(
      INIT => X"AE0C"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_color_N133,
      I2 => mod7_color_N47,
      I3 => mod7_color_n0020,
      LO => N8615,
      O => mod7_color_N28
    );
  mod7_counter_state_counter_n0009_19_lut_INV_0 : INV
    port map (
      I => mod7_counter_count(8),
      O => mod7_counter_N10
    );
  mod7_n00771 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_N231,
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(9),
      I3 => mod7_active_line(4),
      O => mod7_n0077
    );
  mod7_color_color_lut_yramp_n0000_4_xor : XORCY
    port map (
      CI => mod7_color_color_lut_yramp_n0000_3_cyo,
      LI => mod7_color_yramp_4_rt,
      O => mod7_color_yramp_n0000(4)
    );
  mod7_color_color_lut_yramp_n0000_2_cy : MUXCY
    port map (
      CI => mod7_color_color_lut_yramp_n0000_1_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_color_yramp_2_rt,
      O => mod7_color_color_lut_yramp_n0000_2_cyo
    );
  mod7_color_n00111 : LUT2_D
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_color_Ker1211,
      I1 => mod7_color_N134,
      LO => N8616,
      O => mod7_color_n0011
    );
  mod5_hdframe_n0070_4_102 : LUT4
    generic map(
      INIT => X"2820"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_pattern(2),
      I2 => mod0_pattern(0),
      I3 => mod7_color_yramp(4),
      O => CHOICE15316
    );
  mod7_color_v1h4_y_9_1 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(0),
      O => mod7_color_v1h4_y(9)
    );
  mod7_counter_state_counter_n0009_15_xor : XORCY
    port map (
      CI => mod7_counter_state_counter_n0009_14_cyo,
      LI => mod7_counter_N6,
      O => mod7_counter_n0009(15)
    );
  mod0_serial_interface_bitptr_n0000_0_cy : MUXCY
    port map (
      CI => mod5_field1_vblnk1_4_Q,
      DI => mod5_tsl_4_Q,
      S => mod0_N3,
      O => mod0_serial_interface_bitptr_n0000_0_cyo
    );
  mod7_color_Ker61 : LUT4
    generic map(
      INIT => X"FFA8"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_color_n0019,
      I2 => mod7_color_n0024,
      I3 => N1271,
      O => mod7_color_N61
    );
  mod7_counter_state_counter_n0009_18_lut_INV_0 : INV
    port map (
      I => mod7_counter_count(7),
      O => mod7_counter_N9
    );
  mod7_color_color_lut_yramp_n0000_5_cy : MUXCY
    port map (
      CI => mod7_color_color_lut_yramp_n0000_4_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_color_yramp_5_rt,
      O => mod7_color_color_lut_yramp_n0000_5_cyo
    );
  mod7_color_color_lut_yramp_n0000_7_cy : MUXCY
    port map (
      CI => mod7_color_color_lut_yramp_n0000_6_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_color_yramp_7_rt,
      O => mod7_color_color_lut_yramp_n0000_7_cyo
    );
  mod7_counter_state_counter_n0009_20_lut_INV_0 : INV
    port map (
      I => mod7_counter_count(9),
      O => mod7_counter_N11
    );
  mod7_color_n00231 : LUT4_D
    generic map(
      INIT => X"2000"
    )
    port map (
      I0 => mod7_tp_5_Q,
      I1 => mod7_h_state_FFd1_4,
      I2 => mod7_color_Ker1211,
      I3 => mod7_color_N140,
      LO => N8617,
      O => mod7_color_n0023
    );
  mod5_hdframe_n0075_0_69 : LUT4
    generic map(
      INIT => X"2AA8"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => mod0_pattern(1),
      I2 => mod0_pattern(0),
      I3 => mod0_pattern(2),
      O => CHOICE15045
    );
  mod0_hd_format_2_1 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(3),
      O => sif_hd_format(2)
    );
  mod7_counter_state_counter_n0009_17_xor : XORCY
    port map (
      CI => mod7_counter_state_counter_n0009_16_cyo,
      LI => mod7_counter_N8,
      O => mod7_counter_n0009(17)
    );
  mod7_color_color_lut_yramp_n0000_0_cy : MUXCY
    port map (
      CI => mod5_field1_vblnk1_4_Q,
      DI => mod5_tsl_4_Q,
      S => mod7_color_N3,
      O => mod7_color_color_lut_yramp_n0000_0_cyo
    );
  mod7_color_Ker24 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => mod7_color_v0h2_y_2_Q,
      I1 => mod7_color_n0016,
      I2 => N6921,
      I3 => N8614,
      O => mod7_color_N24
    );
  mod7_counter_state_counter_n0009_18_xor : XORCY
    port map (
      CI => mod7_counter_state_counter_n0009_17_cyo,
      LI => mod7_counter_N9,
      O => mod7_counter_n0009(18)
    );
  mod7_counter_state_counter_n0009_19_xor : XORCY
    port map (
      CI => mod7_counter_state_counter_n0009_18_cyo,
      LI => mod7_counter_N10,
      O => mod7_counter_n0009(19)
    );
  mod7_color_Ker261 : LUT4
    generic map(
      INIT => X"C080"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => mod7_color_N132,
      I2 => mod7_color_v1h9_y(5),
      I3 => mod7_color_N121,
      O => mod7_color_N26
    );
  mod7_color_Ker77 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_color_n0018,
      I2 => N1234,
      I3 => N8598,
      O => mod7_color_N77
    );
  mod7_color_n00251 : LUT4_D
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod7_h_state_FFd3_4,
      I1 => mod7_h_state_FFd4_4,
      I2 => mod7_h_state_FFd2_4,
      I3 => N8239,
      LO => N8618,
      O => mod7_color_n0025
    );
  mod7_counter_n0002_4_1 : LUT3_L
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_counter_N01,
      I1 => mod7_counter_n0009(15),
      I2 => mod7_load_val(4),
      LO => mod7_counter_n0002(4)
    );
  mod7_color_yramp_7 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_color_yramp_n0000(7),
      CE => mod7_color_n0081,
      CLR => mod7_color_yramp_0_9_N0,
      C => txclka2,
      Q => mod7_color_yramp(7)
    );
  mod7_color_yramp_6 : FDPE
    generic map(
      INIT => '1'
    )
    port map (
      D => mod7_color_yramp_n0000(6),
      CE => mod7_color_n0081,
      PRE => mod7_color_yramp_0_9_N0,
      C => txclka2,
      Q => mod7_color_yramp(6)
    );
  mod7_color_yramp_5 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_color_yramp_n0000(5),
      CE => mod7_color_n0081,
      CLR => mod7_color_yramp_0_9_N0,
      C => txclka2,
      Q => mod7_color_yramp(5)
    );
  mod7_color_color_lut_yramp_n0000_8_cy : MUXCY
    port map (
      CI => mod7_color_color_lut_yramp_n0000_7_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_color_yramp_8_rt,
      O => mod7_color_color_lut_yramp_n0000_8_cyo
    );
  mod7_color_yramp_4 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_color_yramp_n0000(4),
      CE => mod7_color_n0081,
      CLR => mod7_color_yramp_0_9_N0,
      C => txclka2,
      Q => mod7_color_yramp(4)
    );
  mod7_color_color_lut_yramp_n0000_6_cy : MUXCY
    port map (
      CI => mod7_color_color_lut_yramp_n0000_5_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_color_yramp_6_rt,
      O => mod7_color_color_lut_yramp_n0000_6_cyo
    );
  mod7_n0034_5_15 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE14872,
      I1 => N8599,
      I2 => mod7_load_val(5),
      I3 => CHOICE14459,
      O => CHOICE14387
    );
  mod7_color_color_lut_yramp_n0000_6_xor : XORCY
    port map (
      CI => mod7_color_color_lut_yramp_n0000_5_cyo,
      LI => mod7_color_yramp_6_rt,
      O => mod7_color_yramp_n0000(6)
    );
  mod7_counter_state_counter_n0009_18_cy : MUXCY
    port map (
      CI => mod7_counter_state_counter_n0009_17_cyo,
      DI => mod5_tsl_4_Q,
      S => mod7_counter_N9,
      O => mod7_counter_state_counter_n0009_18_cyo
    );
  mod5_hdframe_n0075_9_144_SW1 : LUT4
    generic map(
      INIT => X"EAAA"
    )
    port map (
      I0 => mod0_pattern(0),
      I1 => mod0_tp_option(1),
      I2 => mod0_tp_option(0),
      I3 => mod0_pattern(2),
      O => N8400
    );
  mod7_n0034_5_18 : LUT4
    generic map(
      INIT => X"0880"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_h_state_FFd1,
      I2 => mod7_h_state_FFd4,
      I3 => mod7_h_state_FFd3,
      O => CHOICE14389
    );
  mod5_hdframe_n0070_6_100 : LUT4
    generic map(
      INIT => X"0155"
    )
    port map (
      I0 => mod7_h_state_FFd1_4,
      I1 => mod0_tp_option(2),
      I2 => mod7_h_state_FFd4_5,
      I3 => mod7_h_state_FFd3_5,
      O => CHOICE14940
    );
  mod7_color_Ker43_SW0 : LUT4
    generic map(
      INIT => X"EEC0"
    )
    port map (
      I0 => mod7_color_v1h4_y(9),
      I1 => mod7_tp_19_Q,
      I2 => mod7_color_n0018,
      I3 => N8623,
      O => N7378
    );
  mod7_color_Ker671 : LUT4_D
    generic map(
      INIT => X"ECA0"
    )
    port map (
      I0 => mod7_color_v0h5_cr(0),
      I1 => mod7_tp_19_Q,
      I2 => mod7_color_n0013,
      I3 => mod7_color_n0024,
      LO => N8619,
      O => mod7_color_N67
    );
  mod7_color_color_lut_yramp_n0000_8_xor : XORCY
    port map (
      CI => mod7_color_color_lut_yramp_n0000_7_cyo,
      LI => mod7_color_yramp_8_rt,
      O => mod7_color_yramp_n0000(8)
    );
  mod7_color_n00411 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod7_h_state_FFd4_4,
      I1 => mod7_h_state_FFd3_4,
      I2 => mod7_h_state_FFd2_4,
      I3 => N8634,
      O => mod7_color_n0041
    );
  mod7_color_Ker1291 : LUT4
    generic map(
      INIT => X"7A00"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(1),
      I2 => mod0_pattern(0),
      I3 => mod7_color_n0012,
      O => mod7_color_N129
    );
  mod7_color_n00391 : LUT4_D
    generic map(
      INIT => X"2000"
    )
    port map (
      I0 => mod7_tp_5_Q,
      I1 => mod7_h_state_FFd1_5,
      I2 => mod7_color_N120,
      I3 => mod7_color_N140,
      LO => N8620,
      O => mod7_color_n0039
    );
  mod7_color_Ker8050 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => N8458,
      I1 => mod7_v_state(2),
      I2 => mod7_h_state_FFd2,
      I3 => N8381,
      O => mod7_color_N80
    );
  mod7_color_n00371 : LUT2_D
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_color_N121,
      I1 => mod7_color_N132,
      LO => N8621,
      O => mod7_color_n0037
    );
  mod7_color_n00361 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod7_tp_4_Q,
      I1 => mod7_tp_0_Q,
      I2 => mod7_color_N123,
      O => mod7_color_n0036
    );
  mod7_Ker22230_G : LUT4
    generic map(
      INIT => X"8999"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(7),
      I2 => mod7_active_line(4),
      I3 => mod7_active_line(6),
      O => N8525
    );
  mod7_color_Ker391 : LUT4
    generic map(
      INIT => X"EAC0"
    )
    port map (
      I0 => mod7_color_v0h2_y_2_Q,
      I1 => mod7_color_v0h2_y_0_Q,
      I2 => mod7_color_n0013,
      I3 => N8625,
      O => mod7_color_N39
    );
  mod5_hdframe_n0075_0_66 : LUT4
    generic map(
      INIT => X"AABA"
    )
    port map (
      I0 => mod7_color_N134,
      I1 => mod7_tp_5_Q,
      I2 => mod7_color_N140,
      I3 => mod7_h_state_FFd1,
      O => CHOICE15043
    );
  mod7_color_Ker41 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => N6699,
      I1 => mod7_color_n0018,
      I2 => mod7_color_N91,
      I3 => N8619,
      O => mod7_color_N41
    );
  mod7_color_n00311 : LUT4
    generic map(
      INIT => X"1000"
    )
    port map (
      I0 => mod7_tp_5_Q,
      I1 => mod7_h_state_FFd1_5,
      I2 => N8654,
      I3 => mod7_color_N140,
      O => mod7_color_n0031
    );
  mod7_color_Ker211 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => mod7_color_N80,
      I1 => mod7_color_N97,
      I2 => mod7_color_N102,
      I3 => N8606,
      O => mod7_color_N21
    );
  mod5_hdframe_n0070_9_221_SW1 : LUT4
    generic map(
      INIT => X"F3F1"
    )
    port map (
      I0 => mod5_hdframe_n0114,
      I1 => mod5_hdframe_n0132,
      I2 => N8638,
      I3 => CHOICE15103,
      O => N8039
    );
  mod5_hdframe_hd_framegenerator_n0088_11_lut2 : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(3),
      O => N7973
    );
  mod5_hdframe_n0070_6_116 : LUT4
    generic map(
      INIT => X"F100"
    )
    port map (
      I0 => mod7_v_state(1),
      I1 => mod7_v_state(0),
      I2 => mod7_v_state(2),
      I3 => CHOICE14941,
      O => CHOICE14942
    );
  mod7_color_Ker43 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => mod7_color_v0h2_y_0_Q,
      I1 => mod7_color_n0031,
      I2 => N7378,
      I3 => N8637,
      O => mod7_color_N43
    );
  mod5_hdframe_n0075_7_63 : LUT4
    generic map(
      INIT => X"EAC0"
    )
    port map (
      I0 => mod7_color_v0h2_y_0_Q,
      I1 => mod7_color_v0h2_y_2_Q,
      I2 => mod7_color_n0015,
      I3 => mod7_color_n0012,
      O => CHOICE14720
    );
  mod7_color_n00241 : LUT4_D
    generic map(
      INIT => X"2000"
    )
    port map (
      I0 => mod7_tp_5_Q,
      I1 => mod7_h_state_FFd1_5,
      I2 => mod7_color_N122,
      I3 => mod7_color_N140,
      LO => N8622,
      O => mod7_color_n0024
    );
  mod7_color_Ker45 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_color_n0019,
      I2 => N8653,
      I3 => mod7_color_N98,
      O => mod7_color_N45
    );
  mod5_hdframe_n0070_7_195 : LUT4_L
    generic map(
      INIT => X"BAAA"
    )
    port map (
      I0 => mod5_hdframe_N561,
      I1 => mod5_hdframe_n0115,
      I2 => mod5_hdframe_N871,
      I3 => CHOICE14655,
      LO => mod5_hdframe_n0070(7)
    );
  mod5_hdframe_n01561 : LUT3
    generic map(
      INIT => X"BA"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_n0101,
      O => mod5_hdframe_n0156
    );
  mod7_color_n00201 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_color_N115,
      I1 => N8612,
      O => mod7_color_n0020
    );
  mod7_color_n00191 : LUT2_D
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_color_N115,
      I1 => mod7_color_N122,
      LO => N8623,
      O => mod7_color_n0019
    );
  mod7_color_n00181 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => N8603,
      I1 => mod7_color_Ker1211,
      O => mod7_color_n0018
    );
  mod5_hdframe_n0070_4_185 : LUT4_L
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE15302,
      I1 => mod5_hdframe_N881,
      I2 => mod7_color_N21,
      I3 => CHOICE15325,
      LO => CHOICE15328
    );
  mod7_color_n00161 : LUT2_D
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_color_N123,
      I1 => mod7_color_N140,
      LO => N8624,
      O => mod7_color_n0016
    );
  mod7_color_n00151 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_color_N120,
      I1 => N8596,
      O => mod7_color_n0015
    );
  mod7_color_n00141 : LUT4_D
    generic map(
      INIT => X"1000"
    )
    port map (
      I0 => mod7_tp_5_Q,
      I1 => mod7_h_state_FFd1_5,
      I2 => mod7_color_N120,
      I3 => mod7_color_N140,
      LO => N8625,
      O => mod7_color_n0014
    );
  mod7_color_n00131 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => N8613,
      I1 => mod7_color_N134,
      O => mod7_color_n0013
    );
  mod7_color_Ker811 : LUT4_D
    generic map(
      INIT => X"FFA8"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_color_n0019,
      I2 => mod7_color_n0024,
      I3 => mod7_color_N59,
      LO => N8626,
      O => mod7_color_N81
    );
  mod7_color_yramp_9_N01_INV_0 : INV
    port map (
      I => mod7_yramp_en,
      O => mod7_color_yramp_0_9_N0
    );
  mod7_color_color_lut_yramp_n0000_9_xor : XORCY
    port map (
      CI => mod7_color_color_lut_yramp_n0000_8_cyo,
      LI => mod7_color_yramp_9_rt,
      O => mod7_color_yramp_n0000(9)
    );
  mod7_color_yramp_9 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_color_yramp_n0000(9),
      CE => mod7_color_n0081,
      CLR => mod7_color_yramp_0_9_N0,
      C => txclka2,
      Q => mod7_color_yramp(9)
    );
  mod7_color_yramp_8 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_color_yramp_n0000(8),
      CE => mod7_color_n0081,
      CLR => mod7_color_yramp_0_9_N0,
      C => txclka2,
      Q => mod7_color_yramp(8)
    );
  mod7_color_yramp_0 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_color_N3,
      CE => mod7_color_n0081,
      CLR => mod7_color_yramp_0_9_N0,
      C => txclka2,
      Q => mod7_color_yramp(0)
    );
  mod7_color_yramp_1 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_color_yramp_n0000(1),
      CE => mod7_color_n0081,
      CLR => mod7_color_yramp_0_9_N0,
      C => txclka2,
      Q => mod7_color_yramp(1)
    );
  mod7_color_yramp_2 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_color_yramp_n0000(2),
      CE => mod7_color_n0081,
      CLR => mod7_color_yramp_0_9_N0,
      C => txclka2,
      Q => mod7_color_yramp(2)
    );
  mod7_color_yramp_3 : FDCE
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_color_yramp_n0000(3),
      CE => mod7_color_n0081,
      CLR => mod7_color_yramp_0_9_N0,
      C => txclka2,
      Q => mod7_color_yramp(3)
    );
  mod7_color_Ker1211_20 : LUT4_D
    generic map(
      INIT => X"2212"
    )
    port map (
      I0 => mod7_h_state_FFd3_2,
      I1 => mod7_h_state_FFd2_2,
      I2 => mod7_h_state_FFd1_2,
      I3 => mod7_h_state_FFd4_2,
      LO => N8627,
      O => mod7_color_N121
    );
  mod7_color_Ker971 : LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      I0 => mod0_pattern(0),
      I1 => mod0_pattern(2),
      I2 => mod0_pattern(1),
      I3 => N8618,
      O => mod7_color_N97
    );
  mod7_color_Ker01 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => N8631,
      I1 => mod7_h_state_FFd2,
      O => mod7_color_N01
    );
  mod7_color_Ker1261 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod0_pattern(0),
      I1 => mod0_pattern(2),
      O => mod7_color_N126
    );
  mod7_counter_state_counter_n0009_19_cy : MUXCY
    port map (
      CI => mod7_counter_state_counter_n0009_18_cyo,
      DI => mod5_tsl_4_Q,
      S => mod7_counter_N10,
      O => mod7_counter_state_counter_n0009_19_cyo
    );
  mod5_hdframe_Ker871 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod5_hdframe_n0107,
      I1 => mod5_hdframe_n0114,
      I2 => N8632,
      I3 => mod5_hdframe_n0112,
      O => mod5_hdframe_N871
    );
  mod7_color_Ker791 : LUT4
    generic map(
      INIT => X"FF80"
    )
    port map (
      I0 => mod7_color_v0h2_y_2_Q,
      I1 => mod7_color_N134,
      I2 => mod7_color_N121,
      I3 => mod7_color_n0022,
      O => mod7_color_N79
    );
  mod5_hdframe_n0075_9_208 : LUT4_L
    generic map(
      INIT => X"F100"
    )
    port map (
      I0 => mod7_v_state(2),
      I1 => mod7_v_state(0),
      I2 => mod7_v_state(1),
      I3 => mod7_h_state_FFd1_4,
      LO => CHOICE14852
    );
  mod7_color_Ker42 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => mod7_color_v0h5_cr(0),
      I1 => mod7_color_n0015,
      I2 => N5872,
      I3 => mod7_color_N67,
      O => mod7_color_N42
    );
  mod7_counter_n0002_6_1 : LUT3_L
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_counter_N01,
      I1 => mod7_counter_n0009(17),
      I2 => mod7_load_val(6),
      LO => mod7_counter_n0002(6)
    );
  mod5_hdframe_n0070_6_418_SW0 : LUT4
    generic map(
      INIT => X"BA10"
    )
    port map (
      I0 => mod0_pattern(0),
      I1 => mod0_pattern(2),
      I2 => CHOICE15004,
      I3 => mod7_color_N120,
      O => N8093
    );
  mod7_color_Ker3740 : LUT4
    generic map(
      INIT => X"FF20"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(0),
      I2 => CHOICE15282,
      I3 => CHOICE15279,
      O => CHOICE15284
    );
  mod7_color_Ker91 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => N1254,
      I2 => N8617,
      O => mod7_color_N91
    );
  mod7_color_Ker25_SW2 : LUT4_L
    generic map(
      INIT => X"0013"
    )
    port map (
      I0 => N1258,
      I1 => CHOICE15447,
      I2 => mod7_color_n0018,
      I3 => mod7_color_N129,
      LO => N8043
    );
  mod7_color_Ker571 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => mod7_color_n0041,
      I1 => N8620,
      I2 => mod7_color_n0024,
      O => mod7_color_N57
    );
  mod7_color_Ker781 : LUT3
    generic map(
      INIT => X"81"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(1),
      I2 => mod0_pattern(0),
      O => mod7_color_N78
    );
  mod7_color_Ker551 : LUT3
    generic map(
      INIT => X"DF"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(0),
      I2 => mod0_pattern(1),
      O => mod7_color_N55
    );
  mod7_color_Ker1181 : LUT4_D
    generic map(
      INIT => X"0800"
    )
    port map (
      I0 => mod7_tp_5_Q,
      I1 => mod7_tp_4_Q,
      I2 => mod7_h_state_FFd2,
      I3 => mod7_tp_0_Q,
      LO => N8628,
      O => mod7_color_N118
    );
  mod5_hdframe_Eq_stagelut38 : LUT4_L
    generic map(
      INIT => X"60A0"
    )
    port map (
      I0 => mod5_hdframe_samplecount(9),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(10),
      I3 => sif_hd_format(1),
      LO => mod5_hdframe_N164
    );
  mod5_hdframe_n0079_13_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => N8650,
      I1 => mod5_hdframe_n0190(1),
      I2 => mod5_hdframe_chroma_out(9),
      I3 => mod5_hdframe_chroma_crc(9),
      LO => mod5_hdframe_n0079(13)
    );
  mod5_hdframe_Mxor_n0154_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_chroma_out(0),
      I1 => mod5_hdframe_chroma_crc(0),
      O => mod5_hdframe_n0190(0)
    );
  mod7_h_state_FFd1_2_21 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd1_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd1_2
    );
  mod5_hdframe_samplecount_3_n00001 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => mod5_hdframe_n0107,
      I1 => sif_rst,
      I2 => mod5_hdframe_samplecount_n0000(3),
      O => mod5_hdframe_samplecount_0_3_n0000
    );
  mod5_hdframe_Mxor_n0153_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_chroma_out(1),
      I1 => mod5_hdframe_chroma_crc(1),
      O => mod5_hdframe_n0193(0)
    );
  mod5_hdframe_Mxor_n0152_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_chroma_out(2),
      I1 => mod5_hdframe_chroma_crc(2),
      O => mod5_hdframe_n0193(1)
    );
  mod7_n0046_3_692_G : LUT4
    generic map(
      INIT => X"FFEF"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(1),
      I2 => mod7_N236,
      I3 => mod7_active_line(5),
      O => N8527
    );
  mod5_hdframe_samplecount_4_n00001 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => mod5_hdframe_n0107,
      I1 => sif_rst,
      I2 => mod5_hdframe_samplecount_n0000(4),
      O => mod5_hdframe_samplecount_0_4_n0000
    );
  mod5_hdframe_Mxor_n0151_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_chroma_out(3),
      I1 => mod5_hdframe_chroma_crc(3),
      O => mod5_hdframe_n0192(0)
    );
  mod5_hdframe_n007821 : LUT4
    generic map(
      INIT => X"2030"
    )
    port map (
      I0 => mod5_hdframe_n0112,
      I1 => N8251,
      I2 => CHOICE14142,
      I3 => mod5_hdframe_n0107,
      O => mod5_hdframe_n0078
    );
  mod5_hdframe_Mxor_n0149_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_chroma_out(4),
      I1 => mod5_hdframe_chroma_crc(4),
      O => mod5_hdframe_n0191(0)
    );
  mod5_hdframe_n0075_8_59 : LUT4_D
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_n0191(0),
      I2 => mod5_hdframe_n0190(0),
      I3 => mod5_hdframe_n0190(1),
      LO => N8629,
      O => CHOICE15359
    );
  mod5_hdframe_n0075_0_184 : LUT4_L
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => mod5_hdframe_N301,
      I1 => mod5_hdframe_N901,
      I2 => CHOICE15024,
      I3 => CHOICE15055,
      LO => mod5_hdframe_n0075(0)
    );
  mod5_hdframe_Mxor_n0147_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_chroma_out(5),
      I1 => mod5_hdframe_chroma_crc(5),
      O => mod5_hdframe_n0190(1)
    );
  mod5_hdframe_norcy_rn_17 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo16,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N200,
      O => mod5_hdframe_nor_cyo17
    );
  mod5_hdframe_n0075_1_123 : LUT4_L
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => mod5_hdframe_N301,
      I1 => mod5_hdframe_N901,
      I2 => CHOICE14591,
      I3 => CHOICE14612,
      LO => mod5_hdframe_n0075(1)
    );
  mod5_hdframe_n0075_2_140 : LUT4_L
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => mod5_hdframe_N511,
      I1 => mod5_hdframe_N871,
      I2 => CHOICE15243,
      I3 => CHOICE15264,
      LO => mod5_hdframe_n0075(2)
    );
  mod5_hdframe_n0075_3_146 : LUT4_L
    generic map(
      INIT => X"FAEE"
    )
    port map (
      I0 => mod5_hdframe_N521,
      I1 => N8052,
      I2 => N8053,
      I3 => CHOICE15454,
      LO => mod5_hdframe_n0075(3)
    );
  mod5_hdframe_n0075_4_203 : LUT4_L
    generic map(
      INIT => X"FAEE"
    )
    port map (
      I0 => mod5_hdframe_N531,
      I1 => N8055,
      I2 => N8056,
      I3 => CHOICE15493,
      LO => mod5_hdframe_n0075(4)
    );
  mod5_hdframe_Andlut2 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => mod5_hdframe_samplecount(1),
      LO => mod5_hdframe_N201
    );
  mod5_hdframe_n0075_5_241 : LUT4_L
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => mod5_hdframe_N541,
      I1 => mod5_hdframe_N871,
      I2 => CHOICE15197,
      I3 => CHOICE15239,
      LO => mod5_hdframe_n0075(5)
    );
  mod5_hdframe_n0075_6_200 : LUT4_L
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => mod5_hdframe_N551,
      I1 => mod5_hdframe_N901,
      I2 => CHOICE14773,
      I3 => CHOICE14797,
      LO => mod5_hdframe_n0075(6)
    );
  mod5_hdframe_n0075_7_160 : LUT4_L
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => mod5_hdframe_N561,
      I1 => mod5_hdframe_N901,
      I2 => CHOICE14717,
      I3 => CHOICE14732,
      LO => mod5_hdframe_n0075(7)
    );
  mod5_hdframe_n0075_8_152 : LUT4_L
    generic map(
      INIT => X"FAEE"
    )
    port map (
      I0 => mod5_hdframe_N571,
      I1 => N8064,
      I2 => N8065,
      I3 => CHOICE15369,
      LO => mod5_hdframe_n0075(8)
    );
  mod7_Ker2921 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => mod7_h_state_FFd4,
      I2 => mod7_v_state(3),
      I3 => mod7_h_state_FFd3,
      O => mod7_N292
    );
  mod5_hdframe_samplecount_5_n00001 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => mod5_hdframe_n0107,
      I1 => sif_rst,
      I2 => mod5_hdframe_samplecount_n0000(5),
      O => mod5_hdframe_samplecount_0_5_n0000
    );
  mod5_hdframe_n0070_1_21_G : LUT4_L
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_color_N134,
      I1 => mod7_color_N122,
      I2 => mod7_color_v0h2_y_2_Q,
      I3 => mod7_color_v0h2_y_0_Q,
      LO => N8529
    );
  mod5_hdframe_samplecount_6_n00001 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => mod5_hdframe_n0107,
      I1 => sif_rst,
      I2 => mod5_hdframe_samplecount_n0000(6),
      O => mod5_hdframe_samplecount_0_6_n0000
    );
  mod5_hdframe_Eq_stagecy_rn_40 : MUXCY
    port map (
      CI => mod5_hdframe_And_cyo5,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N206,
      O => mod5_hdframe_Eq_stage_cyo32
    );
  mod6_scram20_inst_Mxor_n0002_Result1 : LUT3_L
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => mod6_scram20_inst_S(7),
      I1 => mod6_scram20_inst_S(8),
      I2 => mod6_scram20_inst_n0147(0),
      LO => mod6_scram20_inst_n0145(0)
    );
  mod7_color_Ker381 : LUT4_D
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_color_v1h9_y(5),
      I1 => mod7_color_N132,
      I2 => mod7_h_state_FFd2,
      I3 => mod7_color_n0041,
      LO => N8630,
      O => mod7_color_N38
    );
  mod5_hdframe_Ker57 : LUT4
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => mod5_hdframe_n0094,
      I1 => mod5_hdframe_n0095,
      I2 => mod5_hdframe_n0132,
      I3 => N8135,
      O => mod5_hdframe_N571
    );
  mod5_hdframe_n0070_1_1171_G : LUT4_L
    generic map(
      INIT => X"BAAA"
    )
    port map (
      I0 => mod5_hdframe_N301,
      I1 => mod5_hdframe_n0115,
      I2 => mod5_hdframe_N871,
      I3 => mod5_hdframe_luma_crc(11),
      LO => N8531
    );
  mod5_hdframe_norlut19 : LUT4_L
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => mod5_hdframe_samplecount(1),
      I2 => mod5_hdframe_samplecount(3),
      I3 => mod5_hdframe_samplecount(4),
      LO => mod5_hdframe_N204
    );
  mod5_hdframe_Eq_stagelut41 : LUT4_L
    generic map(
      INIT => X"60A0"
    )
    port map (
      I0 => mod5_hdframe_samplecount(7),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(8),
      I3 => sif_hd_format(1),
      LO => mod5_hdframe_N206
    );
  mod5_hdframe_norlut21 : LUT4_L
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod5_hdframe_samplecount(1),
      I1 => mod5_hdframe_samplecount(3),
      I2 => mod5_hdframe_samplecount(4),
      I3 => mod5_hdframe_samplecount(5),
      LO => mod5_hdframe_N208
    );
  mod5_hdframe_norcy_rn_20 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N208,
      O => mod5_hdframe_nor_cyo20
    );
  mod5_hdframe_norlut22 : LUT2_L
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => mod5_hdframe_samplecount(6),
      I1 => mod5_hdframe_samplecount(11),
      LO => mod5_hdframe_N209
    );
  mod5_hdframe_n0074_13_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0099,
      I1 => mod5_hdframe_n0177(1),
      I2 => mod5_hdframe_luma_out(9),
      I3 => mod5_hdframe_luma_crc(9),
      LO => mod5_hdframe_n0074(13)
    );
  mod7_color_Ker3723 : LUT4_L
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => mod7_color_N133,
      I1 => CHOICE15272,
      I2 => mod7_color_N123,
      I3 => CHOICE15277,
      LO => CHOICE15279
    );
  mod5_hdframe_samplecount_7_n00001 : LUT4
    generic map(
      INIT => X"A280"
    )
    port map (
      I0 => mod5_hdframe_n0107,
      I1 => sif_rst,
      I2 => mod5_hdframe_samplecount_n0000(7),
      I3 => mod5_asl(7),
      O => mod5_hdframe_samplecount_0_7_n0000
    );
  mod7_n0044_3_843 : LUT4
    generic map(
      INIT => X"EEEF"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_active_line(6),
      I2 => mod7_active_line(3),
      I3 => mod7_active_line(8),
      O => CHOICE13305
    );
  mod7_color_n00711 : LUT3
    generic map(
      INIT => X"6E"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(0),
      I2 => mod0_pattern(1),
      O => mod7_color_v0h2_y_0_Q
    );
  mod5_hdframe_samplecount_8_n00001 : LUT4
    generic map(
      INIT => X"A280"
    )
    port map (
      I0 => mod5_hdframe_n0107,
      I1 => sif_rst,
      I2 => mod5_hdframe_samplecount_n0000(8),
      I3 => mod5_tsl_4_Q,
      O => mod5_hdframe_samplecount_0_8_n0000
    );
  mod5_hdframe_n0070_0_206 : LUT4_L
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => mod5_hdframe_N301,
      I1 => mod5_hdframe_N901,
      I2 => CHOICE14658,
      I3 => CHOICE14699,
      LO => mod5_hdframe_n0070(0)
    );
  mod5_hdframe_n0070_0_113 : LUT4
    generic map(
      INIT => X"2030"
    )
    port map (
      I0 => CHOICE14686,
      I1 => mod0_pattern(0),
      I2 => mod0_pattern(2),
      I3 => N8448,
      O => CHOICE14693
    );
  mod5_hdframe_n0070_2_162 : LUT4_L
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => mod5_hdframe_N511,
      I1 => mod5_hdframe_N871,
      I2 => CHOICE15243,
      I3 => CHOICE15193,
      LO => mod5_hdframe_n0070(2)
    );
  mod5_hdframe_n0070_3_187 : LUT4_L
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => mod5_hdframe_N521,
      I1 => mod5_hdframe_N871,
      I2 => CHOICE15131,
      I3 => CHOICE15163,
      LO => mod5_hdframe_n0070(3)
    );
  mod5_hdframe_n0070_4_261 : LUT4_L
    generic map(
      INIT => X"FAEE"
    )
    port map (
      I0 => mod5_hdframe_N531,
      I1 => N8061,
      I2 => N8062,
      I3 => CHOICE15328,
      LO => mod5_hdframe_n0070(4)
    );
  mod5_hdframe_n0070_5_284 : LUT4_L
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => mod5_hdframe_N541,
      I1 => mod5_hdframe_N871,
      I2 => CHOICE15197,
      I3 => CHOICE15431,
      LO => mod5_hdframe_n0070(5)
    );
  mod5_hdframe_n0070_6_569 : LUT4_L
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => mod5_hdframe_N551,
      I1 => mod5_hdframe_N901,
      I2 => CHOICE14924,
      I3 => CHOICE15020,
      LO => mod5_hdframe_n0070(6)
    );
  mod5_hdframe_n0075_9_509 : LUT4_L
    generic map(
      INIT => X"F5F4"
    )
    port map (
      I0 => mod5_hdframe_n0114,
      I1 => CHOICE14899,
      I2 => CHOICE15103,
      I3 => CHOICE14883,
      LO => CHOICE14906
    );
  mod5_hdframe_n0070_8_158 : LUT4_L
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => mod5_hdframe_N571,
      I1 => mod5_hdframe_N901,
      I2 => CHOICE14572,
      I3 => CHOICE14587,
      LO => mod5_hdframe_n0070(8)
    );
  mod5_hdframe_n0070_6_265_SW0 : LUT4_L
    generic map(
      INIT => X"CA8A"
    )
    port map (
      I0 => mod0_pattern(0),
      I1 => mod0_pattern(2),
      I2 => mod7_h_state_FFd2_4,
      I3 => mod7_color_yramp(6),
      LO => N8157
    );
  mod0_hd_format_tmp_0 : FDCE_1
    port map (
      D => MOSI_IBUF,
      CE => mod0_n0262,
      CLR => mod0_new_delay_N0,
      C => SCK_BUFGP,
      Q => mod0_hd_format_tmp(0)
    );
  mod5_hdframe_n0070_5_159 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod7_color_yramp(5),
      I1 => mod7_color_N01,
      I2 => mod7_color_n0039,
      I3 => mod7_color_n0024,
      O => CHOICE15420
    );
  mod5_hdframe_samplecount_9_n00001 : LUT4
    generic map(
      INIT => X"A280"
    )
    port map (
      I0 => mod5_hdframe_n0107,
      I1 => sif_rst,
      I2 => mod5_hdframe_samplecount_n0000(9),
      I3 => mod5_asl(7),
      O => mod5_hdframe_samplecount_0_9_n0000
    );
  mod7_color_Ker2220 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => CHOICE15339,
      I1 => mod7_color_N77,
      I2 => N8607,
      O => mod7_color_N22
    );
  mod5_hdframe_n0075_5_61 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_chroma_crc(14),
      O => CHOICE15213
    );
  mod5_hdframe_samplecount_10_n00001 : LUT4
    generic map(
      INIT => X"A280"
    )
    port map (
      I0 => mod5_hdframe_n0107,
      I1 => sif_rst,
      I2 => mod5_hdframe_samplecount_n0000(10),
      I3 => mod5_tsl_4_Q,
      O => mod5_hdframe_samplecount_0_10_n0000
    );
  mod0_pattern_1 : FDCE_1
    port map (
      D => MOSI_IBUF,
      CE => mod0_n0264,
      CLR => mod0_new_delay_N0,
      C => SCK_BUFGP,
      Q => mod0_pattern(1)
    );
  mod7_n0034_4_77 : LUT4_L
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod7_load_val(4),
      I1 => CHOICE14291,
      I2 => CHOICE14293,
      I3 => CHOICE14304,
      LO => CHOICE14307
    );
  mod5_hdframe_samplecount_11_n00001 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => mod5_hdframe_n0107,
      I1 => sif_rst,
      I2 => mod5_hdframe_samplecount_n0000(11),
      O => mod5_hdframe_samplecount_0_11_n0000
    );
  mod7_n0034_5_9 : LUT4
    generic map(
      INIT => X"A8AA"
    )
    port map (
      I0 => mod7_load_val(5),
      I1 => mod7_N292,
      I2 => mod7_N93,
      I3 => mod7_n0086,
      O => CHOICE14384
    );
  mod7_n0034_1_37 : LUT4
    generic map(
      INIT => X"C040"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_tp_21_Q,
      I2 => mod7_n0025,
      I3 => mod7_v_state(2),
      O => CHOICE14175
    );
  mod5_hdframe_samplecount_1_n00001 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => mod5_hdframe_n0107,
      I1 => sif_rst,
      I2 => mod5_hdframe_samplecount_n0000(1),
      O => mod5_hdframe_samplecount_0_1_n0000
    );
  mod5_hdframe_n00651 : LUT4
    generic map(
      INIT => X"88A8"
    )
    port map (
      I0 => sif_rst,
      I1 => mod5_hdframe_n0094,
      I2 => mod5_hdframe_n0132,
      I3 => mod5_hdframe_n0095,
      O => mod5_hdframe_n0065
    );
  mod5_hdframe_Mxor_n0145_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_luma_out(1),
      I1 => mod5_hdframe_luma_crc(1),
      O => mod5_hdframe_n0177(0)
    );
  mod5_hdframe_Mxor_n0144_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_luma_out(2),
      I1 => mod5_hdframe_luma_crc(2),
      O => mod5_hdframe_n0175(0)
    );
  mod5_hdframe_Mxor_n0143_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_luma_out(3),
      I1 => mod5_hdframe_luma_crc(3),
      O => mod5_hdframe_n0173(0)
    );
  mod5_hdframe_Mxor_n0142_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_luma_out(0),
      I1 => mod5_hdframe_luma_crc(0),
      O => mod5_hdframe_n0179(0)
    );
  mod5_hdframe_Mxor_n0141_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_luma_out(4),
      I1 => mod5_hdframe_luma_crc(4),
      O => mod5_hdframe_n0171(0)
    );
  mod5_hdframe_Mxor_n0140_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_luma_out(5),
      I1 => mod5_hdframe_luma_crc(5),
      O => mod5_hdframe_n0177(1)
    );
  mod5_hdframe_Eq_stagelut1 : LUT4_L
    generic map(
      INIT => X"60A0"
    )
    port map (
      I0 => mod5_hdframe_samplecount(9),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(10),
      I3 => sif_hd_format(1),
      LO => mod5_hdframe_N59
    );
  mod5_hdframe_Eq_stagelut43 : LUT4_L
    generic map(
      INIT => X"60A0"
    )
    port map (
      I0 => mod5_hdframe_samplecount(7),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(8),
      I3 => sif_hd_format(1),
      LO => mod5_hdframe_N211
    );
  mod5_hdframe_norcy_rn_22 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N213,
      O => mod5_hdframe_nor_cyo22
    );
  mod5_hdframe_Ker54 : LUT4
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => mod5_hdframe_n0094,
      I1 => mod5_hdframe_n0095,
      I2 => mod5_hdframe_n0132,
      I3 => N8137,
      O => mod5_hdframe_N541
    );
  mod5_hdframe_n0075_1_2 : LUT3_L
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => mod7_color_v0h2_y_0_Q,
      I1 => mod7_color_N134,
      I2 => mod7_color_N121,
      LO => CHOICE14593
    );
  mod5_hdframe_n0079_8_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0099,
      I1 => mod5_hdframe_n0191(0),
      I2 => mod5_hdframe_n0190(0),
      I3 => mod5_hdframe_n0190(1),
      LO => mod5_hdframe_n0079(8)
    );
  mod5_hdframe_Andlut3 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => mod5_hdframe_samplecount(2),
      LO => mod5_hdframe_N210
    );
  mod5_hdframe_Mxor_n0049_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_chroma_out(7),
      I1 => mod5_hdframe_chroma_crc(7),
      O => mod5_hdframe_chroma_crc0(15)
    );
  mod5_hdframe_n0079_7_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0099,
      I1 => mod5_hdframe_chroma_crc(17),
      I2 => mod5_hdframe_n0191(0),
      I3 => mod5_hdframe_n0192(0),
      LO => mod5_hdframe_n0079(7)
    );
  mod5_hdframe_Mxor_n0048_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_chroma_out(8),
      I1 => mod5_hdframe_chroma_crc(8),
      O => mod5_hdframe_chroma_crc0(16)
    );
  mod5_hdframe_n0079_6_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0099,
      I1 => mod5_hdframe_chroma_crc(16),
      I2 => mod5_hdframe_n0192(0),
      I3 => mod5_hdframe_n0193(1),
      LO => mod5_hdframe_n0079(6)
    );
  mod5_hdframe_n0079_12_Q : LUT4
    generic map(
      INIT => X"0220"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_n0191(0),
      I3 => N8454,
      O => mod5_hdframe_n0079(12)
    );
  mod5_hdframe_n0074_4_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0099,
      I1 => mod5_hdframe_luma_crc(14),
      I2 => mod5_hdframe_n0177(0),
      I3 => mod5_hdframe_n0179(0),
      LO => mod5_hdframe_n0074(4)
    );
  mod5_hdframe_n0074_9_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0099,
      I1 => mod5_hdframe_luma_crc0(14),
      I2 => mod5_hdframe_n0177(0),
      I3 => mod5_hdframe_n0177(1),
      LO => mod5_hdframe_n0074(9)
    );
  mod5_hdframe_Ker56 : LUT4
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => mod5_hdframe_n0094,
      I1 => mod5_hdframe_n0095,
      I2 => mod5_hdframe_n0132,
      I3 => N8141,
      O => mod5_hdframe_N561
    );
  mod5_hdframe_Mxor_n0050_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_chroma_out(6),
      I1 => mod5_hdframe_chroma_crc(6),
      O => mod5_hdframe_chroma_crc0(14)
    );
  mod7_color_Ker1321 : LUT4_D
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_v_state(2),
      I1 => mod7_v_state(1),
      I2 => mod7_h_state_FFd4_3,
      I3 => mod7_h_state_FFd1_3,
      LO => N8631,
      O => mod7_color_N132
    );
  mod7_color_Ker3714 : LUT4
    generic map(
      INIT => X"40C0"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod7_tp_4_Q,
      I2 => mod7_tp_0_Q,
      I3 => mod7_color_v1h4_y(9),
      O => CHOICE15277
    );
  mod5_hdframe_samplecount_2_n00001 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => mod5_hdframe_n0107,
      I1 => sif_rst,
      I2 => mod5_hdframe_samplecount_n0000(2),
      O => mod5_hdframe_samplecount_0_2_n0000
    );
  mod7_n0034_3_23 : LUT4
    generic map(
      INIT => X"5554"
    )
    port map (
      I0 => mod7_tp_21_Q,
      I1 => mod7_n0024,
      I2 => mod7_n0030,
      I3 => CHOICE14335,
      O => CHOICE14339
    );
  mod5_hdframe_n0070_0_83 : LUT4_L
    generic map(
      INIT => X"FFC8"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => mod7_color_N133,
      I2 => mod7_tp_1_Q,
      I3 => mod7_color_n0020,
      LO => CHOICE14685
    );
  mod5_hdframe_n01551_SW0 : LUT2
    generic map(
      INIT => X"D"
    )
    port map (
      I0 => mod5_hdframe_linecount(0),
      I1 => mod5_hdframe_linecount(3),
      O => N8072
    );
  mod5_hdframe_luma_crc_12 : FDCE
    port map (
      D => mod5_hdframe_n0074(12),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(12)
    );
  mod5_hdframe_luma_crc_5 : FDCE
    port map (
      D => mod5_hdframe_n0074(5),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(5)
    );
  mod5_hdframe_luma_crc_10 : FDCE
    port map (
      D => mod5_hdframe_n0074(10),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(10)
    );
  mod5_hdframe_luma_crc_8 : FDCE
    port map (
      D => mod5_hdframe_n0074(8),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(8)
    );
  mod7_color_Ker471 : LUT4
    generic map(
      INIT => X"FFA6"
    )
    port map (
      I0 => mod7_h_state_FFd3_4,
      I1 => mod7_h_state_FFd1_4,
      I2 => mod7_h_state_FFd4_5,
      I3 => mod7_h_state_FFd2_4,
      O => mod7_color_N47
    );
  mod5_hdframe_luma_crc_7 : FDCE
    port map (
      D => mod5_hdframe_n0074(7),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(7)
    );
  mod5_hdframe_n01331 : LUT2
    generic map(
      INIT => X"D"
    )
    port map (
      I0 => mod5_hdframe_n0107,
      I1 => mod5_hdframe_n0112,
      O => mod5_hdframe_n0133
    );
  mod5_hdframe_n01321 : LUT4_D
    generic map(
      INIT => X"7FFF"
    )
    port map (
      I0 => mod5_hdframe_n0103,
      I1 => mod5_hdframe_n0104,
      I2 => mod5_hdframe_n0106,
      I3 => mod5_hdframe_n0105,
      LO => N8632,
      O => mod5_hdframe_n0132
    );
  mod5_hdframe_luma_crc_11 : FDCE
    port map (
      D => mod5_hdframe_n0074(11),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(11)
    );
  mod5_hdframe_luma_crc_4 : FDCE
    port map (
      D => mod5_hdframe_n0074(4),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(4)
    );
  mod5_hdframe_luma_crc_6 : FDCE
    port map (
      D => mod5_hdframe_n0074(6),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(6)
    );
  mod5_hdframe_luma_crc_9 : FDCE
    port map (
      D => mod5_hdframe_n0074(9),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(9)
    );
  mod5_hdframe_luma_crc_3 : FDCE
    port map (
      D => mod5_hdframe_n0074(3),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(3)
    );
  mod7_n0034_6_183_SW1 : LUT4
    generic map(
      INIT => X"DD3B"
    )
    port map (
      I0 => mod7_h_state_FFd4,
      I1 => mod7_h_state_FFd3,
      I2 => mod7_tp_21_Q,
      I3 => CHOICE14459,
      O => N8452
    );
  mod5_hdframe_n00931 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod5_hdframe_N891,
      I1 => mod5_hdframe_linecount(0),
      I2 => mod5_hdframe_linecount(3),
      O => TP4_OBUF
    );
  mod5_hdframe_n0075_9_538 : LUT4_L
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => mod5_hdframe_n0094,
      I1 => mod5_hdframe_n0095,
      I2 => mod5_hdframe_n0132,
      I3 => CHOICE14906,
      LO => mod5_hdframe_n0075(9)
    );
  mod7_n0034_0_26 : LUT4_L
    generic map(
      INIT => X"8F88"
    )
    port map (
      I0 => mod7_n0022,
      I1 => mod7_v_state(3),
      I2 => mod7_N173,
      I3 => CHOICE14088,
      LO => CHOICE14091
    );
  mod5_hdframe_n0079_2_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_chroma_crc(12),
      O => mod5_hdframe_n0079(2)
    );
  mod5_hdframe_norcy_rn_18 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N204,
      O => mod5_hdframe_nor_cyo18
    );
  mod5_hdframe_norcy_rn_21 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo20,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N209,
      O => mod5_hdframe_nor_cyo21
    );
  mod5_hdframe_Andcy_rn_5 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo21,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N210,
      O => mod5_hdframe_And_cyo6
    );
  mod5_hdframe_Eq_stagecy_rn_42 : MUXCY
    port map (
      CI => mod5_hdframe_And_cyo6,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N211,
      O => mod5_hdframe_Eq_stage_cyo33
    );
  mod5_hdframe_norlut23 : LUT4_L
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod5_hdframe_samplecount(5),
      I1 => mod5_hdframe_samplecount(4),
      I2 => mod5_hdframe_samplecount(3),
      I3 => mod5_hdframe_samplecount_0_1,
      LO => mod5_hdframe_N213
    );
  mod5_hdframe_Neq_stagelut8 : LUT4_L
    generic map(
      INIT => X"60A0"
    )
    port map (
      I0 => mod5_hdframe_samplecount(7),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(8),
      I3 => sif_hd_format(1),
      LO => mod5_hdframe_N179
    );
  mod5_hdframe_norlut20 : LUT3_L
    generic map(
      INIT => X"01"
    )
    port map (
      I0 => mod5_hdframe_samplecount(5),
      I1 => mod5_hdframe_samplecount(6),
      I2 => mod5_hdframe_samplecount(11),
      LO => mod5_hdframe_N205
    );
  mod5_hdframe_norcy_rn_19 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo18,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N205,
      O => mod5_hdframe_nor_cyo19
    );
  mod5_hdframe_Andcy_rn_4 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo19,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_samplecount_2_rt,
      O => mod5_hdframe_And_cyo5
    );
  mod5_hdframe_sav_22 : FDE
    port map (
      D => mod5_hdframe_n0094,
      CE => mod5_hdframe_n0065,
      C => txclka2,
      Q => mod5_hdframe_sav
    );
  mod5_hdframe_line_clk_23 : FDE
    port map (
      D => mod5_hdframe_n0095,
      CE => mod5_hdframe_n0067,
      C => txclka2,
      Q => mod5_hdframe_line_clk
    );
  mod5_hdframe_n0079_1_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_chroma_crc(11),
      O => mod5_hdframe_n0079(1)
    );
  mod5_hdframe_chroma_9 : FD
    port map (
      D => mod5_hdframe_chroma_out(9),
      C => txclka2,
      Q => mod5_hdframe_chroma(9)
    );
  mod5_hdframe_luma_out_9 : FDC
    port map (
      D => mod5_hdframe_n0070(9),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_out(9)
    );
  mod5_hdframe_luma_9 : FD
    port map (
      D => mod5_hdframe_luma_out(9),
      C => txclka2,
      Q => mod5_hdframe_luma(9)
    );
  mod5_hdframe_f_24 : FDC_1
    port map (
      D => N8048,
      CLR => mod7_h_state_FFd1_N0,
      C => mod5_hdframe_line_clk,
      Q => mod5_hdframe_f
    );
  mod5_hdframe_h_25 : FDP
    port map (
      D => mod5_hdframe_nor_cyo7,
      PRE => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_h
    );
  mod5_hdframe_luma_crc_17 : FDCE
    port map (
      D => mod5_hdframe_n0074(17),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(17)
    );
  mod5_hdframe_chroma_out_9 : FDC
    port map (
      D => mod5_hdframe_n0075(9),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_out(9)
    );
  mod5_hdframe_v_26 : FDP_1
    port map (
      D => N8047,
      PRE => mod7_h_state_FFd1_N0,
      C => mod5_hdframe_line_clk,
      Q => mod5_hdframe_v
    );
  mod0_hd_format_tmp_1 : FDCE_1
    port map (
      D => MOSI_IBUF,
      CE => mod0_n0266,
      CLR => mod0_new_delay_N0,
      C => SCK_BUFGP,
      Q => mod0_hd_format_tmp(1)
    );
  mod5_hdframe_chroma_crc_17 : FDCE
    port map (
      D => mod5_hdframe_n0079(17),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(17)
    );
  mod6_scram20_inst_Mxor_n0024_Result1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_N(1),
      I1 => mod6_scram20_inst_S(0),
      I2 => mod6_scram20_inst_S(19),
      I3 => mod6_scram20_inst_S(18),
      O => mod6_scram20_inst_n0166(0)
    );
  mod5_hdframe_v1_SW0 : LUT2_L
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => mod5_hdframe_v,
      I1 => mod5_hdframe_n0123,
      LO => N8070
    );
  mod5_hdframe_n00671 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0107,
      I1 => sif_rst,
      O => mod5_hdframe_n0067
    );
  mod5_hdframe_hd_framegenerator_n0083_11_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7972,
      O => mod5_hdframe_n0083_11_Q
    );
  mod5_hdframe_n0069_10_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0092(10),
      I1 => mod5_hdframe_n0096,
      O => mod5_hdframe_n0069(10)
    );
  mod5_hdframe_n0074_8_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0099,
      I1 => mod5_hdframe_n0177(1),
      I2 => mod5_hdframe_n0179(0),
      I3 => mod5_hdframe_n0171(0),
      LO => mod5_hdframe_n0074(8)
    );
  mod5_hdframe_hd_framegenerator_n0086_11_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7973,
      O => mod5_hdframe_n0086_11_Q
    );
  mod5_hdframe_hd_framegenerator_n0087_11_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7974,
      O => mod5_hdframe_n0087_11_Q
    );
  mod5_hdframe_hd_framegenerator_n0088_11_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => mod5_hdframe_N54,
      O => mod5_hdframe_n0088_11_Q
    );
  mod5_hdframe_samplecount_7_n00011 : LUT4
    generic map(
      INIT => X"5540"
    )
    port map (
      I0 => sif_rst,
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(2),
      I3 => mod0_hd_format_tmp(3),
      O => mod5_hdframe_samplecount_0_7_n0001
    );
  mod5_hdframe_hd_framegenerator_n0088_10_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7975,
      O => mod5_hdframe_n0088_10_Q
    );
  mod5_hdframe_n0074_7_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0099,
      I1 => mod5_hdframe_luma_crc(17),
      I2 => mod5_hdframe_n0173(0),
      I3 => mod5_hdframe_n0171(0),
      LO => mod5_hdframe_n0074(7)
    );
  mod5_hdframe_hd_framegenerator_n0092_10_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_9_cyo,
      LI => mod5_hdframe_linecount_10_rt,
      O => mod5_hdframe_n0092(10)
    );
  mod5_hdframe_n0075_9_83_SW0 : LUT4
    generic map(
      INIT => X"BBBA"
    )
    port map (
      I0 => mod7_tp_5_Q,
      I1 => mod7_color_N78,
      I2 => mod7_h_state_FFd2,
      I3 => mod7_tp_1_Q,
      O => N8089
    );
  mod5_hdframe_Eq_stagecy_rn_35 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo29,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N151,
      O => mod5_hdframe_n0094
    );
  mod5_hdframe_Eq_stagecy_rn_0 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N59,
      O => mod5_hdframe_n0095
    );
  mod5_hdframe_Neq_stagecy_rn_4 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo4,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N65,
      O => mod5_hdframe_n0096
    );
  mod5_hdframe_Eq_stagecy_rn_5 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo4,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N70,
      O => mod5_hdframe_n0097
    );
  mod5_hdframe_n0075_6_94 : LUT4
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_color_v0h5_cr(0),
      I1 => mod7_color_N123,
      I2 => mod7_color_N140,
      I3 => mod7_color_n0012,
      O => CHOICE14788
    );
  mod5_hdframe_Eq_stagecy_rn_37 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo30,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N164,
      O => mod5_hdframe_n0100
    );
  mod5_hdframe_norcy_rn_11 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo49,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N172,
      O => mod5_hdframe_n0101
    );
  mod7_n0034_0_95 : LUT4
    generic map(
      INIT => X"A8AD"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => mod7_h_state_FFd3,
      I2 => mod7_h_state_FFd4,
      I3 => mod7_h_state_FFd1,
      O => CHOICE14107
    );
  mod5_hdframe_Neq_stagecy_rn_6 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo5,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N176,
      O => mod5_hdframe_n0103
    );
  mod5_hdframe_Neq_stagecy_rn_8 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo6,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N180,
      O => mod5_hdframe_n0104
    );
  mod5_hdframe_Neq_stagecy_rn_14 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo11,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N186,
      O => mod5_hdframe_n0105
    );
  mod5_hdframe_Neq_stagecy_rn_20 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo16,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N192,
      O => mod5_hdframe_n0106
    );
  mod5_hdframe_Neq_stagecy_rn_26 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo21,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N198,
      O => mod5_hdframe_n0107
    );
  mod5_hdframe_luma_crc_2 : FDCE
    port map (
      D => mod5_hdframe_n0074(2),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(2)
    );
  mod5_hdframe_luma_crc_1 : FDCE
    port map (
      D => mod5_hdframe_n0074(1),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(1)
    );
  mod5_hdframe_luma_crc_0 : FDCE
    port map (
      D => mod5_hdframe_n0074(0),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(0)
    );
  mod5_hdframe_luma_crc_13 : FDCE
    port map (
      D => mod5_hdframe_n0074(13),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(13)
    );
  mod5_hdframe_Eq_stagecy_rn_39 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo31,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N203,
      O => mod5_hdframe_n0112
    );
  mod5_hdframe_chroma_crc_16 : FDCE
    port map (
      D => mod5_hdframe_n0079(16),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(16)
    );
  mod5_hdframe_Eq_stagecy_rn_41 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo32,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N207,
      O => mod5_hdframe_n0114
    );
  mod5_hdframe_Eq_stagecy_rn_43 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo33,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N212,
      O => mod5_hdframe_n0115
    );
  mod5_hdframe_Eq_stagecy_rn_45 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo34,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N217,
      O => mod5_hdframe_n0116
    );
  mod5_hdframe_v1 : LUT4_L
    generic map(
      INIT => X"2223"
    )
    port map (
      I0 => mod5_hdframe_n0125,
      I1 => mod5_hdframe_n0126,
      I2 => mod5_hdframe_n0124,
      I3 => N8070,
      LO => N8047
    );
  mod5_hdframe_n0075_8_114_SW1 : LUT4
    generic map(
      INIT => X"888C"
    )
    port map (
      I0 => CHOICE15359,
      I1 => N8577,
      I2 => mod5_hdframe_n0116,
      I3 => N8504,
      O => N8065
    );
  mod5_hdframe_XNor_stagecy_rn_9 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo9,
      DI => mod5_hdframe_linecount(10),
      S => mod5_hdframe_N85,
      O => mod5_hdframe_n0119
    );
  mod5_hdframe_Neq_stagelut16 : LUT3_L
    generic map(
      INIT => X"09"
    )
    port map (
      I0 => mod5_hdframe_n0087_1_Q,
      I1 => mod5_hdframe_samplecount(1),
      I2 => mod5_hdframe_samplecount_0_1,
      LO => mod5_hdframe_N187
    );
  mod5_hdframe_XNor_stagecy_rn_42 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo41,
      DI => mod5_hdframe_linecount(10),
      S => mod5_hdframe_N120,
      O => mod5_hdframe_n0121
    );
  mod5_hdframe_Eq_stagecy_rn_11 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo9,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N126,
      O => mod5_hdframe_n0123
    );
  mod5_hdframe_Eq_stagecy_rn_17 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo14,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N132,
      O => mod5_hdframe_n0124
    );
  mod5_hdframe_Eq_stagecy_rn_23 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo19,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N138,
      O => mod5_hdframe_n0125
    );
  mod5_hdframe_Eq_stagecy_rn_29 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo24,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N144,
      O => mod5_hdframe_n0126
    );
  mod5_hdframe_chroma_crc_15 : FDCE
    port map (
      D => mod5_hdframe_n0079(15),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(15)
    );
  mod5_hdframe_n0074_12_Q : LUT4
    generic map(
      INIT => X"0220"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_n0171(0),
      I3 => N8456,
      O => mod5_hdframe_n0074(12)
    );
  mod5_hdframe_Mxor_n0032_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_luma_out(8),
      I1 => mod5_hdframe_luma_crc(8),
      O => mod5_hdframe_luma_crc0(16)
    );
  mod5_hdframe_Mxor_n0033_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_luma_out(7),
      I1 => mod5_hdframe_luma_crc(7),
      O => mod5_hdframe_luma_crc0(15)
    );
  mod5_hdframe_Mxor_n0034_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => mod5_hdframe_luma_out(6),
      I1 => mod5_hdframe_luma_crc(6),
      O => mod5_hdframe_luma_crc0(14)
    );
  mod5_hdframe_n0070_4_224_SW1 : LUT4
    generic map(
      INIT => X"8A02"
    )
    port map (
      I0 => mod5_hdframe_N871,
      I1 => mod5_hdframe_n0115,
      I2 => N8502,
      I3 => mod5_hdframe_linecount(9),
      O => N8062
    );
  mod5_hdframe_n0079_9_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0099,
      I1 => mod5_hdframe_chroma_crc0(14),
      I2 => mod5_hdframe_n0193(0),
      I3 => mod5_hdframe_n0190(1),
      LO => mod5_hdframe_n0079(9)
    );
  mod5_hdframe_n0075_3_106_SW1_SW0 : LUT4_L
    generic map(
      INIT => X"2882"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_chroma_out(0),
      I2 => mod5_hdframe_chroma_crc(0),
      I3 => mod5_hdframe_chroma_crc(13),
      LO => N8507
    );
  mod5_hdframe_n0079_4_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0099,
      I1 => mod5_hdframe_chroma_crc(14),
      I2 => mod5_hdframe_n0190(0),
      I3 => mod5_hdframe_n0193(0),
      LO => mod5_hdframe_n0079(4)
    );
  mod5_hdframe_Andcy_rn_3 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo17,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N201,
      O => mod5_hdframe_And_cyo4
    );
  mod5_hdframe_n0079_10_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0099,
      I1 => mod5_hdframe_chroma_crc0(14),
      I2 => mod5_hdframe_n0193(1),
      I3 => mod5_hdframe_chroma_crc0(15),
      LO => mod5_hdframe_n0079(10)
    );
  mod5_hdframe_Ker55 : LUT4
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => mod5_hdframe_n0094,
      I1 => mod5_hdframe_n0095,
      I2 => mod5_hdframe_n0132,
      I3 => N8139,
      O => mod5_hdframe_N551
    );
  mod5_hdframe_n0079_11_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0099,
      I1 => mod5_hdframe_chroma_crc0(16),
      I2 => mod5_hdframe_n0192(0),
      I3 => mod5_hdframe_chroma_crc0(15),
      LO => mod5_hdframe_n0079(11)
    );
  mod5_hdframe_n0069_9_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0092(9),
      I1 => mod5_hdframe_n0096,
      O => mod5_hdframe_n0069(9)
    );
  mod5_hdframe_luma_crc_15 : FDCE
    port map (
      D => mod5_hdframe_n0074(15),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(15)
    );
  mod5_hdframe_luma_crc_16 : FDCE
    port map (
      D => mod5_hdframe_n0074(16),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(16)
    );
  mod5_hdframe_luma_0 : FD
    port map (
      D => mod5_hdframe_luma_out(0),
      C => txclka2,
      Q => mod5_hdframe_luma(0)
    );
  mod5_hdframe_luma_1 : FD
    port map (
      D => mod5_hdframe_luma_out(1),
      C => txclka2,
      Q => mod5_hdframe_luma(1)
    );
  mod5_hdframe_luma_2 : FD
    port map (
      D => mod5_hdframe_luma_out(2),
      C => txclka2,
      Q => mod5_hdframe_luma(2)
    );
  mod5_hdframe_luma_3 : FD
    port map (
      D => mod5_hdframe_luma_out(3),
      C => txclka2,
      Q => mod5_hdframe_luma(3)
    );
  mod5_hdframe_luma_4 : FD
    port map (
      D => mod5_hdframe_luma_out(4),
      C => txclka2,
      Q => mod5_hdframe_luma(4)
    );
  mod5_hdframe_luma_5 : FD
    port map (
      D => mod5_hdframe_luma_out(5),
      C => txclka2,
      Q => mod5_hdframe_luma(5)
    );
  mod5_hdframe_luma_6 : FD
    port map (
      D => mod5_hdframe_luma_out(6),
      C => txclka2,
      Q => mod5_hdframe_luma(6)
    );
  mod5_hdframe_luma_7 : FD
    port map (
      D => mod5_hdframe_luma_out(7),
      C => txclka2,
      Q => mod5_hdframe_luma(7)
    );
  mod5_hdframe_luma_8 : FD
    port map (
      D => mod5_hdframe_luma_out(8),
      C => txclka2,
      Q => mod5_hdframe_luma(8)
    );
  mod5_hdframe_chroma_0 : FD
    port map (
      D => mod5_hdframe_chroma_out(0),
      C => txclka2,
      Q => mod5_hdframe_chroma(0)
    );
  mod5_hdframe_chroma_1 : FD
    port map (
      D => mod5_hdframe_chroma_out(1),
      C => txclka2,
      Q => mod5_hdframe_chroma(1)
    );
  mod5_hdframe_chroma_2 : FD
    port map (
      D => mod5_hdframe_chroma_out(2),
      C => txclka2,
      Q => mod5_hdframe_chroma(2)
    );
  mod5_hdframe_chroma_3 : FD
    port map (
      D => mod5_hdframe_chroma_out(3),
      C => txclka2,
      Q => mod5_hdframe_chroma(3)
    );
  mod5_hdframe_chroma_4 : FD
    port map (
      D => mod5_hdframe_chroma_out(4),
      C => txclka2,
      Q => mod5_hdframe_chroma(4)
    );
  mod5_hdframe_chroma_5 : FD
    port map (
      D => mod5_hdframe_chroma_out(5),
      C => txclka2,
      Q => mod5_hdframe_chroma(5)
    );
  mod5_hdframe_chroma_6 : FD
    port map (
      D => mod5_hdframe_chroma_out(6),
      C => txclka2,
      Q => mod5_hdframe_chroma(6)
    );
  mod5_hdframe_chroma_7 : FD
    port map (
      D => mod5_hdframe_chroma_out(7),
      C => txclka2,
      Q => mod5_hdframe_chroma(7)
    );
  mod5_hdframe_chroma_8 : FD
    port map (
      D => mod5_hdframe_chroma_out(8),
      C => txclka2,
      Q => mod5_hdframe_chroma(8)
    );
  mod5_hdframe_fvh_out_1 : FDE
    port map (
      D => mod5_hdframe_v,
      CE => mod5_hdframe_n0078,
      C => txclka2,
      Q => mod5_hdframe_fvh_out(1)
    );
  mod5_hdframe_chroma_out_0 : FDC
    port map (
      D => mod5_hdframe_n0075(0),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_out(0)
    );
  mod5_hdframe_chroma_out_1 : FDC
    port map (
      D => mod5_hdframe_n0075(1),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_out(1)
    );
  mod5_hdframe_chroma_out_2 : FDC
    port map (
      D => mod5_hdframe_n0075(2),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_out(2)
    );
  mod5_hdframe_chroma_out_3 : FDC
    port map (
      D => mod5_hdframe_n0075(3),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_out(3)
    );
  mod5_hdframe_chroma_out_4 : FDC
    port map (
      D => mod5_hdframe_n0075(4),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_out(4)
    );
  mod5_hdframe_chroma_out_5 : FDC
    port map (
      D => mod5_hdframe_n0075(5),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_out(5)
    );
  mod5_hdframe_chroma_out_6 : FDC
    port map (
      D => mod5_hdframe_n0075(6),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_out(6)
    );
  mod5_hdframe_chroma_out_7 : FDC
    port map (
      D => mod5_hdframe_n0075(7),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_out(7)
    );
  mod5_hdframe_chroma_out_8 : FDC
    port map (
      D => mod5_hdframe_n0075(8),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_out(8)
    );
  mod5_hdframe_luma_crc_14 : FDCE
    port map (
      D => mod5_hdframe_n0074(14),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_crc(14)
    );
  mod5_hdframe_luma_out_0 : FDC
    port map (
      D => mod5_hdframe_n0070(0),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_out(0)
    );
  mod5_hdframe_luma_out_1 : FDC
    port map (
      D => mod5_hdframe_n0070(1),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_out(1)
    );
  mod5_hdframe_luma_out_2 : FDC
    port map (
      D => mod5_hdframe_n0070(2),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_out(2)
    );
  mod5_hdframe_luma_out_3 : FDC
    port map (
      D => mod5_hdframe_n0070(3),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_out(3)
    );
  mod5_hdframe_luma_out_4 : FDC
    port map (
      D => mod5_hdframe_n0070(4),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_out(4)
    );
  mod5_hdframe_luma_out_5 : FDC
    port map (
      D => mod5_hdframe_n0070(5),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_out(5)
    );
  mod5_hdframe_luma_out_6 : FDC
    port map (
      D => mod5_hdframe_n0070(6),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_out(6)
    );
  mod5_hdframe_luma_out_7 : FDC
    port map (
      D => mod5_hdframe_n0070(7),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_out(7)
    );
  mod5_hdframe_luma_out_8 : FDC
    port map (
      D => mod5_hdframe_n0070(8),
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_luma_out(8)
    );
  mod5_hdframe_fvh_out_0 : FDE
    port map (
      D => mod5_hdframe_h,
      CE => mod5_hdframe_n0078,
      C => txclka2,
      Q => mod5_hdframe_fvh_out(0)
    );
  mod5_hdframe_chroma_crc_0 : FDCE
    port map (
      D => mod5_hdframe_n0079(0),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(0)
    );
  mod5_hdframe_chroma_crc_1 : FDCE
    port map (
      D => mod5_hdframe_n0079(1),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(1)
    );
  mod5_hdframe_chroma_crc_2 : FDCE
    port map (
      D => mod5_hdframe_n0079(2),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(2)
    );
  mod5_hdframe_chroma_crc_3 : FDCE
    port map (
      D => mod5_hdframe_n0079(3),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(3)
    );
  mod5_hdframe_chroma_crc_4 : FDCE
    port map (
      D => mod5_hdframe_n0079(4),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(4)
    );
  mod5_hdframe_chroma_crc_5 : FDCE
    port map (
      D => mod5_hdframe_n0079(5),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(5)
    );
  mod5_hdframe_chroma_crc_6 : FDCE
    port map (
      D => mod5_hdframe_n0079(6),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(6)
    );
  mod5_hdframe_chroma_crc_7 : FDCE
    port map (
      D => mod5_hdframe_n0079(7),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(7)
    );
  mod5_hdframe_chroma_crc_8 : FDCE
    port map (
      D => mod5_hdframe_n0079(8),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(8)
    );
  mod5_hdframe_chroma_crc_9 : FDCE
    port map (
      D => mod5_hdframe_n0079(9),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(9)
    );
  mod5_hdframe_chroma_crc_10 : FDCE
    port map (
      D => mod5_hdframe_n0079(10),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(10)
    );
  mod5_hdframe_chroma_crc_11 : FDCE
    port map (
      D => mod5_hdframe_n0079(11),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(11)
    );
  mod5_hdframe_chroma_crc_12 : FDCE
    port map (
      D => mod5_hdframe_n0079(12),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(12)
    );
  mod5_hdframe_chroma_crc_13 : FDCE
    port map (
      D => mod5_hdframe_n0079(13),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(13)
    );
  mod5_hdframe_chroma_crc_14 : FDCE
    port map (
      D => mod5_hdframe_n0079(14),
      CE => mod5_hdframe_n0156,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_chroma_crc(14)
    );
  mod5_hdframe_linecount_10 : FDC
    port map (
      D => mod5_hdframe_n0069(10),
      CLR => mod7_h_state_FFd1_N0,
      C => mod5_hdframe_line_clk,
      Q => mod5_hdframe_linecount(10)
    );
  mod5_hdframe_linecount_9 : FDC
    port map (
      D => mod5_hdframe_n0069(9),
      CLR => mod7_h_state_FFd1_N0,
      C => mod5_hdframe_line_clk,
      Q => mod5_hdframe_linecount(9)
    );
  mod5_hdframe_linecount_0 : FDP
    port map (
      D => mod5_hdframe_n0069(0),
      PRE => mod7_h_state_FFd1_N0,
      C => mod5_hdframe_line_clk,
      Q => mod5_hdframe_linecount(0)
    );
  mod5_hdframe_linecount_1 : FDC
    port map (
      D => mod5_hdframe_n0069(1),
      CLR => mod7_h_state_FFd1_N0,
      C => mod5_hdframe_line_clk,
      Q => mod5_hdframe_linecount(1)
    );
  mod5_hdframe_linecount_2 : FDC
    port map (
      D => mod5_hdframe_n0069(2),
      CLR => mod7_h_state_FFd1_N0,
      C => mod5_hdframe_line_clk,
      Q => mod5_hdframe_linecount(2)
    );
  mod5_hdframe_linecount_3 : FDC
    port map (
      D => mod5_hdframe_n0069(3),
      CLR => mod7_h_state_FFd1_N0,
      C => mod5_hdframe_line_clk,
      Q => mod5_hdframe_linecount(3)
    );
  mod5_hdframe_linecount_4 : FDC
    port map (
      D => mod5_hdframe_n0069(4),
      CLR => mod7_h_state_FFd1_N0,
      C => mod5_hdframe_line_clk,
      Q => mod5_hdframe_linecount(4)
    );
  mod5_hdframe_linecount_5 : FDC
    port map (
      D => mod5_hdframe_n0069(5),
      CLR => mod7_h_state_FFd1_N0,
      C => mod5_hdframe_line_clk,
      Q => mod5_hdframe_linecount(5)
    );
  mod5_hdframe_linecount_6 : FDC
    port map (
      D => mod5_hdframe_n0069(6),
      CLR => mod7_h_state_FFd1_N0,
      C => mod5_hdframe_line_clk,
      Q => mod5_hdframe_linecount(6)
    );
  mod5_hdframe_linecount_7 : FDC
    port map (
      D => mod5_hdframe_n0069(7),
      CLR => mod7_h_state_FFd1_N0,
      C => mod5_hdframe_line_clk,
      Q => mod5_hdframe_linecount(7)
    );
  mod5_hdframe_linecount_8 : FDC
    port map (
      D => mod5_hdframe_n0069(8),
      CLR => mod7_h_state_FFd1_N0,
      C => mod5_hdframe_line_clk,
      Q => mod5_hdframe_linecount(8)
    );
  mod5_hdframe_n0069_8_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0092(8),
      I1 => mod5_hdframe_n0096,
      O => mod5_hdframe_n0069(8)
    );
  mod5_hdframe_n0069_7_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0092(7),
      I1 => mod5_hdframe_n0096,
      O => mod5_hdframe_n0069(7)
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_11_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_10_cyo,
      LI => mod5_hdframe_samplecount_11_rt,
      O => mod5_hdframe_samplecount_n0000(11)
    );
  mod5_hdframe_n0069_6_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0092(6),
      I1 => mod5_hdframe_n0096,
      O => mod5_hdframe_n0069(6)
    );
  mod5_hdframe_n0069_5_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0092(5),
      I1 => mod5_hdframe_n0096,
      O => mod5_hdframe_n0069(5)
    );
  mod5_hdframe_n0069_4_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0092(4),
      I1 => mod5_hdframe_n0096,
      O => mod5_hdframe_n0069(4)
    );
  mod5_hdframe_n0069_3_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0092(3),
      I1 => mod5_hdframe_n0096,
      O => mod5_hdframe_n0069(3)
    );
  mod5_hdframe_n0069_2_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0092(2),
      I1 => mod5_hdframe_n0096,
      O => mod5_hdframe_n0069(2)
    );
  mod5_hdframe_n0069_1_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0092(1),
      I1 => mod5_hdframe_n0096,
      O => mod5_hdframe_n0069(1)
    );
  mod5_hdframe_Neq_stagelut : LUT4
    generic map(
      INIT => X"420A"
    )
    port map (
      I0 => mod5_hdframe_linecount(0),
      I1 => sif_hd_format(1),
      I2 => mod5_hdframe_linecount(1),
      I3 => sif_hd_format(2),
      O => mod5_hdframe_N60
    );
  mod7_color_Ker331 : LUT4
    generic map(
      INIT => X"FF80"
    )
    port map (
      I0 => mod7_color_v1h2_cb_6_Q,
      I1 => mod7_color_N115,
      I2 => mod7_color_N121,
      I3 => mod7_color_N39,
      O => mod7_color_N33
    );
  mod5_hdframe_n0079_0_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_chroma_crc(10),
      O => mod5_hdframe_n0079(0)
    );
  mod5_hdframe_n0074_17_1 : LUT4
    generic map(
      INIT => X"0220"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_luma_crc(9),
      I3 => mod5_hdframe_luma_out(9),
      O => mod5_hdframe_n0074(17)
    );
  mod5_hdframe_n0074_16_1 : LUT4
    generic map(
      INIT => X"0220"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_luma_crc(8),
      I3 => mod5_hdframe_luma_out(8),
      O => mod5_hdframe_n0074(16)
    );
  mod5_hdframe_samplecount_11 : FDCP
    port map (
      D => mod5_hdframe_samplecount_0_11_n0000,
      CLR => mod7_h_state_FFd1_N0,
      PRE => mod5_field1_vblnk1_4_Q,
      C => txclka2,
      Q => mod5_hdframe_samplecount(11)
    );
  mod5_hdframe_samplecount_10 : FDCP
    port map (
      D => mod5_hdframe_samplecount_0_10_n0000,
      CLR => mod5_field1_vblnk1_4_Q,
      PRE => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_samplecount(10)
    );
  mod5_hdframe_samplecount_0 : FDCP
    port map (
      D => mod5_hdframe_samplecount_0_0_n0000,
      CLR => mod7_h_state_FFd1_N0,
      PRE => mod5_field1_vblnk1_4_Q,
      C => txclka2,
      Q => mod5_hdframe_samplecount(0)
    );
  mod5_hdframe_samplecount_1 : FDCP
    port map (
      D => mod5_hdframe_samplecount_0_1_n0000,
      CLR => mod7_h_state_FFd1_N0,
      PRE => mod5_field1_vblnk1_4_Q,
      C => txclka2,
      Q => mod5_hdframe_samplecount(1)
    );
  mod5_hdframe_samplecount_2 : FDCP
    port map (
      D => mod5_hdframe_samplecount_0_2_n0000,
      CLR => mod7_h_state_FFd1_N0,
      PRE => mod5_field1_vblnk1_4_Q,
      C => txclka2,
      Q => mod5_hdframe_samplecount(2)
    );
  mod5_hdframe_samplecount_3 : FDCP
    port map (
      D => mod5_hdframe_samplecount_0_3_n0000,
      CLR => mod7_h_state_FFd1_N0,
      PRE => mod5_field1_vblnk1_4_Q,
      C => txclka2,
      Q => mod5_hdframe_samplecount(3)
    );
  mod5_hdframe_samplecount_4 : FDCP
    port map (
      D => mod5_hdframe_samplecount_0_4_n0000,
      CLR => mod7_h_state_FFd1_N0,
      PRE => mod5_field1_vblnk1_4_Q,
      C => txclka2,
      Q => mod5_hdframe_samplecount(4)
    );
  mod5_hdframe_samplecount_5 : FDCP
    port map (
      D => mod5_hdframe_samplecount_0_5_n0000,
      CLR => mod7_h_state_FFd1_N0,
      PRE => mod5_field1_vblnk1_4_Q,
      C => txclka2,
      Q => mod5_hdframe_samplecount(5)
    );
  mod5_hdframe_samplecount_6 : FDCP
    port map (
      D => mod5_hdframe_samplecount_0_6_n0000,
      CLR => mod7_h_state_FFd1_N0,
      PRE => mod5_field1_vblnk1_4_Q,
      C => txclka2,
      Q => mod5_hdframe_samplecount(6)
    );
  mod5_hdframe_samplecount_7 : FDCP
    port map (
      D => mod5_hdframe_samplecount_0_7_n0000,
      CLR => mod5_hdframe_samplecount_0_7_n0001,
      PRE => mod5_hdframe_samplecount_0_7_n0002,
      C => txclka2,
      Q => mod5_hdframe_samplecount(7)
    );
  mod5_hdframe_samplecount_8 : FDCP
    port map (
      D => mod5_hdframe_samplecount_0_8_n0000,
      CLR => mod5_field1_vblnk1_4_Q,
      PRE => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod5_hdframe_samplecount(8)
    );
  mod5_hdframe_samplecount_9 : FDCP
    port map (
      D => mod5_hdframe_samplecount_0_9_n0000,
      CLR => mod5_hdframe_samplecount_0_7_n0001,
      PRE => mod5_hdframe_samplecount_0_7_n0002,
      C => txclka2,
      Q => mod5_hdframe_samplecount(9)
    );
  mod5_hdframe_n0074_15_1 : LUT4
    generic map(
      INIT => X"0220"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_luma_crc(7),
      I3 => mod5_hdframe_luma_out(7),
      O => mod5_hdframe_n0074(15)
    );
  mod5_hdframe_n0074_14_1 : LUT4
    generic map(
      INIT => X"0220"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_luma_crc(6),
      I3 => mod5_hdframe_luma_out(6),
      O => mod5_hdframe_n0074(14)
    );
  mod5_hdframe_n0074_2_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_luma_crc(12),
      O => mod5_hdframe_n0074(2)
    );
  mod5_hdframe_n0074_1_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_luma_crc(11),
      O => mod5_hdframe_n0074(1)
    );
  mod5_hdframe_Ker8934 : LUT4_D
    generic map(
      INIT => X"1000"
    )
    port map (
      I0 => mod5_hdframe_linecount(10),
      I1 => mod5_hdframe_linecount(2),
      I2 => CHOICE14547,
      I3 => CHOICE14555,
      LO => N8633,
      O => mod5_hdframe_N891
    );
  mod5_hdframe_n0074_0_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_luma_crc(10),
      O => mod5_hdframe_n0074(0)
    );
  mod5_hdframe_n0079_17_1 : LUT4
    generic map(
      INIT => X"0220"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_chroma_crc(9),
      I3 => mod5_hdframe_chroma_out(9),
      O => mod5_hdframe_n0079(17)
    );
  mod5_hdframe_n0079_16_1 : LUT4
    generic map(
      INIT => X"0220"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_chroma_crc(8),
      I3 => mod5_hdframe_chroma_out(8),
      O => mod5_hdframe_n0079(16)
    );
  mod5_hdframe_n0079_15_1 : LUT4
    generic map(
      INIT => X"0220"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_chroma_crc(7),
      I3 => mod5_hdframe_chroma_out(7),
      O => mod5_hdframe_n0079(15)
    );
  mod5_hdframe_n0079_5_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0099,
      I1 => mod5_hdframe_chroma_crc(15),
      I2 => mod5_hdframe_n0193(0),
      I3 => mod5_hdframe_n0193(1),
      LO => mod5_hdframe_n0079(5)
    );
  mod5_hdframe_norcy_rn_25 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo53,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N223,
      O => mod5_hdframe_nor_cyo25
    );
  mod5_hdframe_norlut26_INV_0 : INV
    port map (
      I => mod5_hdframe_samplecount(11),
      O => mod5_hdframe_N223
    );
  mod5_hdframe_XNor_stagecy_rn_54 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo52,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_samplecount_10_rt,
      O => mod5_hdframe_XNor_stage_cyo53
    );
  mod5_hdframe_n0070_6_355 : LUT3
    generic map(
      INIT => X"C8"
    )
    port map (
      I0 => mod7_tp_4_Q,
      I1 => mod7_h_state_FFd2,
      I2 => mod7_tp_1_Q,
      O => CHOICE14990
    );
  mod5_hdframe_XNor_stagecy_rn_53 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo51,
      DI => mod5_asl(7),
      S => mod5_hdframe_N221,
      O => mod5_hdframe_XNor_stage_cyo52
    );
  mod5_hdframe_XNor_stagelut52 : LUT4_L
    generic map(
      INIT => X"5666"
    )
    port map (
      I0 => mod5_hdframe_samplecount(7),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(2),
      I3 => mod0_hd_format_tmp(1),
      LO => mod5_hdframe_N219
    );
  mod5_hdframe_XNor_stagecy_rn_52 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo50,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_samplecount_8_rt,
      O => mod5_hdframe_XNor_stage_cyo51
    );
  mod5_hdframe_n0075_0_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_chroma_crc(9),
      O => CHOICE15025
    );
  mod5_hdframe_XNor_stagecy_rn_51 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo24,
      DI => mod5_asl(7),
      S => mod5_hdframe_N219,
      O => mod5_hdframe_XNor_stage_cyo50
    );
  mod7_color_n00411_SW1 : LUT3_D
    generic map(
      INIT => X"DF"
    )
    port map (
      I0 => mod7_v_state(2),
      I1 => mod7_v_state(1),
      I2 => mod7_h_state_FFd1_3,
      LO => N8634,
      O => N8239
    );
  mod5_hdframe_norcy_rn_24 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N218,
      O => mod5_hdframe_nor_cyo24
    );
  mod5_hdframe_norlut25 : LUT4_L
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod5_hdframe_samplecount(3),
      I1 => mod5_hdframe_samplecount(4),
      I2 => mod5_hdframe_samplecount(5),
      I3 => mod5_hdframe_samplecount(6),
      LO => mod5_hdframe_N218
    );
  mod5_hdframe_Eq_stagelut37 : LUT4_L
    generic map(
      INIT => X"60A0"
    )
    port map (
      I0 => mod5_hdframe_samplecount(7),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(8),
      I3 => sif_hd_format(1),
      LO => mod5_hdframe_N163
    );
  mod5_hdframe_Eq_stagecy_rn_44 : MUXCY
    port map (
      CI => mod5_hdframe_And_cyo7,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N216,
      O => mod5_hdframe_Eq_stage_cyo34
    );
  mod5_hdframe_Neq_stagelut9 : LUT4_L
    generic map(
      INIT => X"60A0"
    )
    port map (
      I0 => mod5_hdframe_samplecount(9),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(10),
      I3 => sif_hd_format(1),
      LO => mod5_hdframe_N180
    );
  mod5_hdframe_Andcy_rn_6 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo23,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N215,
      O => mod5_hdframe_And_cyo7
    );
  mod5_hdframe_Andlut4 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_samplecount(1),
      I1 => mod5_hdframe_samplecount(2),
      LO => mod5_hdframe_N215
    );
  mod5_hdframe_norcy_rn_23 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo22,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N214,
      O => mod5_hdframe_nor_cyo23
    );
  mod5_hdframe_norlut24 : LUT2_L
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => mod5_hdframe_samplecount(6),
      I1 => mod5_hdframe_samplecount(11),
      LO => mod5_hdframe_N214
    );
  mod5_hdframe_hd_framegenerator_n0083_2_cy : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => N7969,
      O => mod5_hdframe_hd_framegenerator_n0083_2_cyo
    );
  mod5_hdframe_hd_framegenerator_n0083_2_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7969,
      O => mod5_hdframe_n0083_2_Q
    );
  mod5_hdframe_hd_framegenerator_n0083_9_lut2 : LUT4
    generic map(
      INIT => X"0025"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(3),
      O => N7986
    );
  mod5_hdframe_hd_framegenerator_n0083_3_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0083_2_cyo,
      DI => mod5_tsl_4_Q,
      S => N7970,
      O => mod5_hdframe_hd_framegenerator_n0083_3_cyo
    );
  mod5_hdframe_hd_framegenerator_n0083_3_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0083_2_cyo,
      LI => N7970,
      O => mod5_hdframe_n0083_3_Q
    );
  mod0_serial_interface_bitptr_n0000_4_xor : XORCY
    port map (
      CI => mod0_serial_interface_bitptr_n0000_3_cyo,
      LI => mod0_bitptr_4_rt,
      O => mod0_bitptr_n0000(4)
    );
  mod0_n02602 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod0_bitptr(0),
      I1 => mod0_bitptr(1),
      I2 => mod0_bitptr(2),
      I3 => mod0_N32,
      O => mod0_n0260
    );
  TP3_OBUF : OBUF
    port map (
      I => mod0_sd_hd_1,
      O => TP3
    );
  mod5_hdframe_hd_framegenerator_n0083_3_lut : LUT4
    generic map(
      INIT => X"FEBA"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(2),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(1),
      O => mod5_hdframe_N15
    );
  mod0_n02601 : LUT4_L
    generic map(
      INIT => X"FBFF"
    )
    port map (
      I0 => CS_IBUF,
      I1 => mod0_bitptr(4),
      I2 => mod0_bitptr(3),
      I3 => mod0_bitptr(5),
      LO => mod0_N32
    );
  mod5_hdframe_hd_framegenerator_n0083_5_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7976,
      O => mod5_hdframe_n0083_5_Q
    );
  mod5_hdframe_hd_framegenerator_n0083_5_lut : LUT4
    generic map(
      INIT => X"1131"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(1),
      O => mod5_hdframe_N17
    );
  mod0_n02622 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod0_bitptr(0),
      I1 => mod0_bitptr(1),
      I2 => mod0_bitptr(2),
      I3 => N8635,
      O => mod0_n0262
    );
  mod5_hdframe_hd_framegenerator_n0083_6_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7977,
      O => mod5_hdframe_n0083_6_Q
    );
  mod7_n0034_2_170 : LUT4
    generic map(
      INIT => X"44C4"
    )
    port map (
      I0 => N8231,
      I1 => mod7_tp_19_Q,
      I2 => mod7_N139,
      I3 => mod7_tp_17_Q,
      O => CHOICE14472
    );
  mod0_n02621 : LUT4_D
    generic map(
      INIT => X"FBFF"
    )
    port map (
      I0 => CS_IBUF,
      I1 => mod0_bitptr(3),
      I2 => mod0_bitptr(4),
      I3 => mod0_bitptr(5),
      LO => N8635,
      O => mod0_N30
    );
  mod5_hdframe_hd_framegenerator_n0083_7_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7978,
      O => mod5_hdframe_n0083_7_Q
    );
  mod5_hdframe_n0070_2_17 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => N8019,
      I1 => N8621,
      I2 => mod7_color_n0039,
      I3 => mod7_color_n0024,
      O => CHOICE15176
    );
  mod0_n02642 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod0_bitptr(0),
      I1 => mod0_bitptr(1),
      I2 => mod0_bitptr(2),
      I3 => mod0_N30,
      O => mod0_n0264
    );
  mod5_hdframe_n0070_9_16 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => mod7_color_N126,
      I1 => mod0_pattern(1),
      I2 => mod7_color_N132,
      I3 => mod7_color_N121,
      O => CHOICE15066
    );
  mod5_hdframe_hd_framegenerator_n0083_6_lut : LUT4
    generic map(
      INIT => X"0207"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(3),
      I3 => mod0_hd_format_tmp(0),
      O => mod5_hdframe_N18
    );
  mod7_color_Ker8050_SW1 : LUT4_L
    generic map(
      INIT => X"E7EF"
    )
    port map (
      I0 => mod7_h_state_FFd1_4,
      I1 => mod7_h_state_FFd3_4,
      I2 => mod7_h_state_FFd4_5,
      I3 => mod7_tp_17_Q,
      LO => N8381
    );
  mod5_hdframe_hd_framegenerator_n0083_9_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7980,
      O => mod5_hdframe_n0083_9_Q
    );
  mod7_Ker2991 : LUT3
    generic map(
      INIT => X"8A"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(4),
      I2 => mod7_active_line(5),
      O => mod7_N299
    );
  mod0_n02662 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod0_bitptr(0),
      I1 => mod0_bitptr(1),
      I2 => mod0_bitptr(2),
      I3 => N8636,
      O => mod0_n0266
    );
  mod5_hdframe_hd_framegenerator_n0083_10_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7981,
      O => mod5_hdframe_n0083_10_Q
    );
  mod5_hdframe_n0070_2_125 : LUT4_L
    generic map(
      INIT => X"2320"
    )
    port map (
      I0 => mod5_hdframe_luma_crc(12),
      I1 => mod5_hdframe_n0115,
      I2 => mod5_hdframe_n0116,
      I3 => CHOICE15188,
      LO => CHOICE15193
    );
  mod0_n02661 : LUT4_D
    generic map(
      INIT => X"FFEF"
    )
    port map (
      I0 => mod0_bitptr(3),
      I1 => mod0_bitptr(4),
      I2 => mod0_bitptr(5),
      I3 => CS_IBUF,
      LO => N8636,
      O => mod0_N26
    );
  mod0_pattern_2 : FDCE_1
    port map (
      D => MOSI_IBUF,
      CE => mod0_n0269,
      CLR => mod0_new_delay_N0,
      C => SCK_BUFGP,
      Q => mod0_pattern(2)
    );
  mod0_hd_format_tmp_2 : FDCE_1
    port map (
      D => MOSI_IBUF,
      CE => mod0_n0271,
      CLR => mod0_new_delay_N0,
      C => SCK_BUFGP,
      Q => mod0_hd_format_tmp(2)
    );
  TP2_OBUF : OBUF
    port map (
      I => txda_19_1,
      O => TP2
    );
  mod5_hdframe_hd_framegenerator_n0083_9_lut : LUT4
    generic map(
      INIT => X"0025"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(3),
      O => mod5_hdframe_N21
    );
  mod5_hdframe_hd_framegenerator_n0086_2_cy : MUXCY
    port map (
      CI => mod5_field1_vblnk1_4_Q,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N24,
      O => mod5_hdframe_hd_framegenerator_n0086_2_cyo
    );
  TP1_OBUF : OBUF
    port map (
      I => txda_9_1,
      O => TP1
    );
  mod5_hdframe_hd_framegenerator_n0086_2_lut : LUT4
    generic map(
      INIT => X"FFEF"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(0),
      I2 => mod0_hd_format_tmp(2),
      I3 => mod0_hd_format_tmp(1),
      O => mod5_hdframe_N24
    );
  mod5_hdframe_hd_framegenerator_n0086_3_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0086_2_cyo,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N25,
      O => mod5_hdframe_hd_framegenerator_n0086_3_cyo
    );
  mod5_hdframe_hd_framegenerator_n0086_3_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0086_2_cyo,
      LI => mod5_hdframe_N25,
      O => mod5_hdframe_n0086_3_Q
    );
  mod0_n02692 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod0_bitptr(1),
      I1 => mod0_bitptr(0),
      I2 => mod0_bitptr(2),
      I3 => mod0_N30,
      O => mod0_n0269
    );
  mod5_hdframe_Ker8925 : LUT3
    generic map(
      INIT => X"01"
    )
    port map (
      I0 => mod5_hdframe_linecount(6),
      I1 => mod5_hdframe_linecount(7),
      I2 => mod5_hdframe_linecount(4),
      O => CHOICE14555
    );
  MOSI_IBUF_27 : IBUF
    port map (
      I => MOSI,
      O => MOSI_IBUF
    );
  mod5_hdframe_n0070_2_2 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_luma_crc(11),
      O => CHOICE15169
    );
  mod0_n02712 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod0_bitptr(1),
      I1 => mod0_bitptr(0),
      I2 => mod0_bitptr(2),
      I3 => mod0_N26,
      O => mod0_n0271
    );
  mod5_hdframe_hd_framegenerator_n0086_5_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7982,
      O => mod5_hdframe_n0086_5_Q
    );
  mod5_hdframe_hd_framegenerator_n0086_3_lut : LUT4
    generic map(
      INIT => X"FEBA"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(2),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(1),
      O => mod5_hdframe_N25
    );
  mod7_n0034_3_210_SW0 : LUT4_L
    generic map(
      INIT => X"32FA"
    )
    port map (
      I0 => CHOICE14374,
      I1 => sif_hd_format(2),
      I2 => CHOICE14370,
      I3 => sif_hd_format(1),
      LO => N8255
    );
  mod5_hdframe_hd_framegenerator_n0086_6_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7983,
      O => mod5_hdframe_n0086_6_Q
    );
  mod7_n0034_2_170_SW0 : LUT4_L
    generic map(
      INIT => X"BFFF"
    )
    port map (
      I0 => mod7_h_state_FFd4,
      I1 => mod7_h_state_FFd3,
      I2 => mod7_h_state_FFd1,
      I3 => mod7_v_state(2),
      LO => N8231
    );
  mod0_Ker11 : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => N8074,
      I1 => mod0_bitptr(5),
      I2 => mod0_bitptr(3),
      I3 => mod0_bitptr(4),
      O => mod0_N11
    );
  mod5_hdframe_hd_framegenerator_n0086_7_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7984,
      O => mod5_hdframe_n0086_7_Q
    );
  mod5_hdframe_n0070_9_171 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => mod5_hdframe_n0115,
      I1 => CHOICE15096,
      O => CHOICE15097
    );
  mod0_hd_format_tmp_3 : FDCE_1
    port map (
      D => MOSI_IBUF,
      CE => mod0_n0275,
      CLR => mod0_new_delay_N0,
      C => SCK_BUFGP,
      Q => mod0_hd_format_tmp(3)
    );
  mod5_hdframe_n0070_1_70 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE14737,
      I1 => mod5_hdframe_N881,
      I2 => CHOICE14749,
      I3 => N8081,
      O => CHOICE14753
    );
  mod5_hdframe_n0070_0_56 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_pattern(0),
      I2 => mod7_color_N57,
      I3 => mod0_pattern(2),
      O => CHOICE14675
    );
  mod0_n02752 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod0_bitptr(0),
      I1 => mod0_bitptr(1),
      I2 => mod0_bitptr(2),
      I3 => mod0_N26,
      O => mod0_n0275
    );
  mod5_hdframe_hd_framegenerator_n0086_9_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7986,
      O => mod5_hdframe_n0086_9_Q
    );
  mod5_hdframe_hd_framegenerator_n0083_3_lut3 : LUT4
    generic map(
      INIT => X"FEBA"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(2),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(1),
      O => N7984
    );
  mod5_hdframe_n0075_9_204 : LUT4
    generic map(
      INIT => X"F0D0"
    )
    port map (
      I0 => mod0_pattern(0),
      I1 => mod7_tp_4_Q,
      I2 => mod7_tp_5_Q,
      I3 => mod7_h_state_FFd1_4,
      O => CHOICE14849
    );
  mod5_hdframe_hd_framegenerator_n0086_10_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7987,
      O => mod5_hdframe_n0086_10_Q
    );
  mod7_n0034_8_129 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_pattern(2),
      I2 => mod0_pattern(0),
      I3 => mod7_tp_21_Q,
      O => CHOICE14284
    );
  mod5_hdframe_Eq_stagecy_rn_38 : MUXCY
    port map (
      CI => mod5_hdframe_And_cyo4,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N202,
      O => mod5_hdframe_Eq_stage_cyo31
    );
  mod5_hdframe_hd_framegenerator_n0087_1_cy : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => N7971,
      O => mod5_hdframe_hd_framegenerator_n0087_1_cyo
    );
  mod5_hdframe_hd_framegenerator_n0087_1_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7971,
      O => mod5_hdframe_n0087_1_Q
    );
  mod5_hdframe_hd_framegenerator_n0083_6_lut1 : LUT4
    generic map(
      INIT => X"0207"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(3),
      I3 => mod0_hd_format_tmp(0),
      O => N7977
    );
  mod5_hdframe_hd_framegenerator_n0087_2_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0087_1_cyo,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N34,
      O => mod5_hdframe_hd_framegenerator_n0087_2_cyo
    );
  mod5_hdframe_hd_framegenerator_n0087_2_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0087_1_cyo,
      LI => mod5_hdframe_N34,
      O => mod5_hdframe_n0087_2_Q
    );
  mod5_hdframe_hd_framegenerator_n0087_2_lut : LUT4
    generic map(
      INIT => X"FFEF"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(0),
      I2 => mod0_hd_format_tmp(2),
      I3 => mod0_hd_format_tmp(1),
      O => mod5_hdframe_N34
    );
  mod5_hdframe_hd_framegenerator_n0087_3_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0087_2_cyo,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N35,
      O => mod5_hdframe_hd_framegenerator_n0087_3_cyo
    );
  mod5_hdframe_hd_framegenerator_n0087_3_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0087_2_cyo,
      LI => mod5_hdframe_N35,
      O => mod5_hdframe_n0087_3_Q
    );
  mod0_n0046_28 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod0_N5,
      I1 => mod0_bitptr(4),
      I2 => mod0_bitptr(3),
      I3 => N1207,
      O => mod0_n0046
    );
  mod0_tp_option_2 : FD_1
    port map (
      D => N8076,
      C => SCK_BUFGP,
      Q => mod0_tp_option(2)
    );
  CS_IBUF_29 : IBUF
    port map (
      I => CS,
      O => CS_IBUF
    );
  mod5_hdframe_n0075_9_303 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => CHOICE14861,
      I2 => CHOICE14873,
      I3 => CHOICE14855,
      O => CHOICE14875
    );
  mod0_tp_option_1 : FD_1
    port map (
      D => N8077,
      C => SCK_BUFGP,
      Q => mod0_tp_option(1)
    );
  mod5_hdframe_hd_framegenerator_n0087_5_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7988,
      O => mod5_hdframe_n0087_5_Q
    );
  mod5_hdframe_n0070_5_247 : LUT4_L
    generic map(
      INIT => X"3335"
    )
    port map (
      I0 => N8058,
      I1 => N8059,
      I2 => mod7_color_N22,
      I3 => CHOICE15423,
      LO => CHOICE15431
    );
  mod0_tp_option_0 : FDE_1
    port map (
      D => MOSI_IBUF,
      CE => mod0_n0046,
      C => SCK_BUFGP,
      Q => mod0_tp_option(0)
    );
  mod5_hdframe_hd_framegenerator_n0087_6_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7989,
      O => mod5_hdframe_n0087_6_Q
    );
  mod5_hdframe_n0075_4_163_SW1 : LUT4
    generic map(
      INIT => X"8A02"
    )
    port map (
      I0 => mod5_hdframe_N871,
      I1 => mod5_hdframe_n0115,
      I2 => N8500,
      I3 => mod5_hdframe_linecount(9),
      O => N8056
    );
  mod0_pattern_0 : FDCE_1
    port map (
      D => MOSI_IBUF,
      CE => mod0_n0260,
      CLR => mod0_new_delay_N0,
      C => SCK_BUFGP,
      Q => mod0_pattern(0)
    );
  mod5_hdframe_hd_framegenerator_n0087_7_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7990,
      O => mod5_hdframe_n0087_7_Q
    );
  mod5_hdframe_n0075_9_68 : LUT4_L
    generic map(
      INIT => X"FFB0"
    )
    port map (
      I0 => mod7_color_N55,
      I1 => mod7_tp_5_Q,
      I2 => mod7_h_state_FFd1,
      I3 => mod7_color_N140,
      LO => CHOICE14819
    );
  mod0_bitptr_4 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod0_bitptr_n0000(4),
      CLR => CS_IBUF,
      C => SCK_BUFGP,
      Q => mod0_bitptr(4)
    );
  mod7_n0034_2_112 : LUT4
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => mod7_n0031,
      I1 => N8153,
      I2 => mod7_v_state(3),
      I3 => N8602,
      O => CHOICE14456
    );
  mod5_hdframe_n0070_6_235 : LUT4
    generic map(
      INIT => X"A8FF"
    )
    port map (
      I0 => mod7_tp_5_Q,
      I1 => mod7_tp_4_Q,
      I2 => CHOICE15007,
      I3 => mod0_pattern(1),
      O => CHOICE14966
    );
  mod0_sd_hd_30 : FDCE_1
    port map (
      D => MOSI_IBUF,
      CE => mod0_n0286,
      CLR => mod0_new_delay_N0,
      C => SCK_BUFGP,
      Q => mod0_sd_hd
    );
  mod5_hdframe_hd_framegenerator_n0087_9_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => N7992,
      O => mod5_hdframe_n0087_9_Q
    );
  mod0_Ker51_SW0 : LUT2
    generic map(
      INIT => X"D"
    )
    port map (
      I0 => reset_n_IBUF,
      I1 => CS_IBUF,
      O => N8074
    );
  mod0_n00471 : LUT4_L
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => mod0_bitptr(0),
      I1 => mod0_bitptr(2),
      I2 => mod0_bitptr(1),
      I3 => mod0_N11,
      LO => mod0_n0047
    );
  mod5_hdframe_hd_framegenerator_n0087_10_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => mod5_hdframe_N22,
      O => mod5_hdframe_n0087_10_Q
    );
  mod5_hdframe_n0070_6_526 : LUT4_L
    generic map(
      INIT => X"0B08"
    )
    port map (
      I0 => mod5_hdframe_luma_crc(15),
      I1 => mod5_hdframe_n0100,
      I2 => mod5_hdframe_n0116,
      I3 => CHOICE15015,
      LO => CHOICE15020
    );
  mod5_hdframe_n0074_6_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0099,
      I1 => mod5_hdframe_luma_crc(16),
      I2 => mod5_hdframe_n0175(0),
      I3 => mod5_hdframe_n0173(0),
      LO => mod5_hdframe_n0074(6)
    );
  mod0_n00481 : LUT4_L
    generic map(
      INIT => X"4000"
    )
    port map (
      I0 => mod0_bitptr(0),
      I1 => mod0_bitptr(1),
      I2 => mod0_bitptr(2),
      I3 => mod0_N11,
      LO => mod0_n0048
    );
  mod0_serial_interface_bitptr_n0000_4_cy : MUXCY
    port map (
      CI => mod0_serial_interface_bitptr_n0000_3_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod0_bitptr_4_rt,
      O => mod0_serial_interface_bitptr_n0000_4_cyo
    );
  mod5_hdframe_samplecount_7_n00021 : LUT4
    generic map(
      INIT => X"0103"
    )
    port map (
      I0 => mod0_hd_format_tmp(1),
      I1 => sif_rst,
      I2 => mod0_hd_format_tmp(3),
      I3 => mod0_hd_format_tmp(2),
      O => mod5_hdframe_samplecount_0_7_n0002
    );
  mod5_hdframe_hd_framegenerator_n0088_1_cy : MUXCY
    port map (
      CI => mod5_field1_vblnk1_4_Q,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N44,
      O => mod5_hdframe_hd_framegenerator_n0088_1_cyo
    );
  reset_n_IBUF_31 : IBUF
    port map (
      I => reset_n,
      O => reset_n_IBUF
    );
  mod5_hdframe_hd_framegenerator_n0087_3_lut : LUT4
    generic map(
      INIT => X"FEBA"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(2),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(1),
      O => mod5_hdframe_N35
    );
  mod5_hdframe_hd_framegenerator_n0088_2_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0088_1_cyo,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N45,
      O => mod5_hdframe_hd_framegenerator_n0088_2_cyo
    );
  mod5_hdframe_hd_framegenerator_n0088_2_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0088_1_cyo,
      LI => mod5_hdframe_N45,
      O => mod5_hdframe_n0088_2_Q
    );
  mod5_hdframe_hd_framegenerator_n0088_2_lut : LUT4
    generic map(
      INIT => X"FFEF"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(0),
      I2 => mod0_hd_format_tmp(2),
      I3 => mod0_hd_format_tmp(1),
      O => mod5_hdframe_N45
    );
  mod5_hdframe_hd_framegenerator_n0088_3_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0088_2_cyo,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N46,
      O => mod5_hdframe_hd_framegenerator_n0088_3_cyo
    );
  mod5_hdframe_hd_framegenerator_n0088_3_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0088_2_cyo,
      LI => mod5_hdframe_N46,
      O => mod5_hdframe_n0088_3_Q
    );
  mod0_n02862 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod0_bitptr(0),
      I1 => mod0_bitptr(1),
      I2 => mod0_bitptr(2),
      I3 => mod0_N26,
      O => mod0_n0286
    );
  mod7_color_Ker376 : LUT4
    generic map(
      INIT => X"1411"
    )
    port map (
      I0 => mod7_h_state_FFd2_4,
      I1 => mod7_h_state_FFd3_4,
      I2 => mod7_h_state_FFd4_5,
      I3 => mod7_h_state_FFd1_5,
      O => CHOICE15272
    );
  f7425_IBUFG_32 : IBUFG
    port map (
      I => f7425,
      O => f7425_IBUFG
    );
  mod7_n0034_0_115_SW0 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_v_state(2),
      I1 => mod7_h_state_FFd2,
      I2 => mod7_tp_19_Q,
      I3 => mod7_h_state_FFd4,
      O => N8131
    );
  mod0_serial_interface_bitptr_n0000_3_xor : XORCY
    port map (
      CI => mod0_serial_interface_bitptr_n0000_2_cyo,
      LI => mod0_bitptr_3_rt,
      O => mod0_bitptr_n0000(3)
    );
  mod5_hdframe_hd_framegenerator_n0088_5_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => mod5_hdframe_N17,
      O => mod5_hdframe_n0088_5_Q
    );
  mod7_n0034_3_175 : LUT4
    generic map(
      INIT => X"88A8"
    )
    port map (
      I0 => mod7_N257,
      I1 => mod7_n0030,
      I2 => N8576,
      I3 => mod7_tp_17_Q,
      O => CHOICE14370
    );
  mod0_serial_interface_bitptr_n0000_3_cy : MUXCY
    port map (
      CI => mod0_serial_interface_bitptr_n0000_2_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod0_bitptr_3_rt,
      O => mod0_serial_interface_bitptr_n0000_3_cyo
    );
  mod5_hdframe_hd_framegenerator_n0088_6_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => mod5_hdframe_N18,
      O => mod5_hdframe_n0088_6_Q
    );
  mod5_hdframe_n0075_2_101 : LUT4_L
    generic map(
      INIT => X"2320"
    )
    port map (
      I0 => mod5_hdframe_chroma_crc(12),
      I1 => mod5_hdframe_n0115,
      I2 => mod5_hdframe_n0116,
      I3 => CHOICE15259,
      LO => CHOICE15264
    );
  mod0_serial_interface_bitptr_n0000_2_xor : XORCY
    port map (
      CI => mod0_serial_interface_bitptr_n0000_1_cyo,
      LI => mod0_bitptr_2_rt,
      O => mod0_bitptr_n0000(2)
    );
  mod5_hdframe_hd_framegenerator_n0088_7_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => mod5_hdframe_N15,
      O => mod5_hdframe_n0088_7_Q
    );
  mod5_hdframe_n0070_9_24 : LUT4
    generic map(
      INIT => X"F2F0"
    )
    port map (
      I0 => mod7_color_v1h9_y(5),
      I1 => mod0_tp_option(1),
      I2 => mod7_color_n0022,
      I3 => mod7_color_n0018,
      O => CHOICE15070
    );
  mod0_serial_interface_bitptr_n0000_2_cy : MUXCY
    port map (
      CI => mod0_serial_interface_bitptr_n0000_1_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod0_bitptr_2_rt,
      O => mod0_serial_interface_bitptr_n0000_2_cyo
    );
  mod7_color_Ker1281 : LUT4_D
    generic map(
      INIT => X"7E00"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(0),
      I2 => mod0_pattern(1),
      I3 => mod7_color_n0014,
      LO => N8637,
      O => mod7_color_N128
    );
  mod7_color_Ker581 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => mod7_color_N132,
      I1 => mod7_color_N121,
      I2 => mod7_color_n0039,
      I3 => N8622,
      O => mod7_color_N58
    );
  mod0_serial_interface_bitptr_n0000_1_xor : XORCY
    port map (
      CI => mod0_serial_interface_bitptr_n0000_0_cyo,
      LI => mod0_bitptr_1_rt,
      O => mod0_bitptr_n0000(1)
    );
  mod5_hdframe_hd_framegenerator_n0088_9_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => mod5_hdframe_N21,
      O => mod5_hdframe_n0088_9_Q
    );
  mod5_hdframe_hd_framegenerator_n0088_3_lut : LUT4
    generic map(
      INIT => X"FEBA"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(2),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(1),
      O => mod5_hdframe_N46
    );
  mod0_serial_interface_bitptr_n0000_1_cy : MUXCY
    port map (
      CI => mod0_serial_interface_bitptr_n0000_0_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod0_bitptr_1_rt,
      O => mod0_serial_interface_bitptr_n0000_1_cyo
    );
  mod5_hdframe_hd_framegenerator_n0092_0_lut_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(0),
      O => mod5_hdframe_N55
    );
  mod5_hdframe_hd_framegenerator_n0092_0_cy : MUXCY
    port map (
      CI => mod5_field1_vblnk1_4_Q,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N55,
      O => mod5_hdframe_hd_framegenerator_n0092_0_cyo
    );
  f7417_IBUFG_33 : IBUFG
    port map (
      I => f7417,
      O => f7417_IBUFG
    );
  mod5_hdframe_hd_framegenerator_n0092_1_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_0_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_linecount_1_rt,
      O => mod5_hdframe_hd_framegenerator_n0092_1_cyo
    );
  mod5_hdframe_hd_framegenerator_n0092_1_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_0_cyo,
      LI => mod5_hdframe_linecount_1_rt,
      O => mod5_hdframe_n0092(1)
    );
  mod5_hdframe_hd_framegenerator_n0092_2_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_1_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_linecount_2_rt,
      O => mod5_hdframe_hd_framegenerator_n0092_2_cyo
    );
  mod5_hdframe_hd_framegenerator_n0092_2_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_1_cyo,
      LI => mod5_hdframe_linecount_2_rt,
      O => mod5_hdframe_n0092(2)
    );
  mod5_hdframe_hd_framegenerator_n0092_3_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_2_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_linecount_3_rt,
      O => mod5_hdframe_hd_framegenerator_n0092_3_cyo
    );
  mod5_hdframe_hd_framegenerator_n0092_3_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_2_cyo,
      LI => mod5_hdframe_linecount_3_rt,
      O => mod5_hdframe_n0092(3)
    );
  mod5_hdframe_hd_framegenerator_n0092_4_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_3_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_linecount_4_rt,
      O => mod5_hdframe_hd_framegenerator_n0092_4_cyo
    );
  mod5_hdframe_hd_framegenerator_n0092_4_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_3_cyo,
      LI => mod5_hdframe_linecount_4_rt,
      O => mod5_hdframe_n0092(4)
    );
  mod5_hdframe_hd_framegenerator_n0092_5_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_4_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_linecount_5_rt,
      O => mod5_hdframe_hd_framegenerator_n0092_5_cyo
    );
  mod5_hdframe_hd_framegenerator_n0092_5_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_4_cyo,
      LI => mod5_hdframe_linecount_5_rt,
      O => mod5_hdframe_n0092(5)
    );
  mod5_hdframe_hd_framegenerator_n0092_6_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_5_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_linecount_6_rt,
      O => mod5_hdframe_hd_framegenerator_n0092_6_cyo
    );
  mod5_hdframe_hd_framegenerator_n0092_6_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_5_cyo,
      LI => mod5_hdframe_linecount_6_rt,
      O => mod5_hdframe_n0092(6)
    );
  mod5_hdframe_hd_framegenerator_n0092_7_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_6_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_linecount_7_rt,
      O => mod5_hdframe_hd_framegenerator_n0092_7_cyo
    );
  mod5_hdframe_hd_framegenerator_n0092_7_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_6_cyo,
      LI => mod5_hdframe_linecount_7_rt,
      O => mod5_hdframe_n0092(7)
    );
  mod5_hdframe_hd_framegenerator_n0092_8_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_7_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_linecount_8_rt,
      O => mod5_hdframe_hd_framegenerator_n0092_8_cyo
    );
  mod5_hdframe_hd_framegenerator_n0092_8_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_7_cyo,
      LI => mod5_hdframe_linecount_8_rt,
      O => mod5_hdframe_n0092(8)
    );
  mod5_hdframe_hd_framegenerator_n0092_9_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_8_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_linecount_9_rt,
      O => mod5_hdframe_hd_framegenerator_n0092_9_cyo
    );
  mod5_hdframe_hd_framegenerator_n0092_9_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_n0092_8_cyo,
      LI => mod5_hdframe_linecount_9_rt,
      O => mod5_hdframe_n0092(9)
    );
  mod5_hdframe_n0074_11_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0099,
      I1 => mod5_hdframe_luma_crc0(16),
      I2 => mod5_hdframe_n0173(0),
      I3 => mod5_hdframe_luma_crc0(15),
      LO => mod5_hdframe_n0074(11)
    );
  mod5_hdframe_xyz_5_1 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => mod5_hdframe_h,
      I1 => mod5_hdframe_v,
      O => mod5_hdframe_MUX_BLOCK_xyz(5)
    );
  mod5_hdframe_Ker301 : LUT2_D
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => mod5_hdframe_n0094,
      I1 => mod5_hdframe_n0095,
      LO => N8638,
      O => mod5_hdframe_N301
    );
  mod7_color_Ker1021 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_pattern(0),
      I2 => N8608,
      I3 => mod0_pattern(2),
      O => mod7_color_N102
    );
  mod5_hdframe_n0074_5_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0099,
      I1 => mod5_hdframe_luma_crc(15),
      I2 => mod5_hdframe_n0177(0),
      I3 => mod5_hdframe_n0175(0),
      LO => mod5_hdframe_n0074(5)
    );
  mod5_hdframe_norlut : LUT4_L
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => mod5_hdframe_samplecount(1),
      I2 => mod5_hdframe_samplecount(2),
      I3 => mod5_hdframe_samplecount(3),
      LO => mod5_hdframe_N56
    );
  mod5_hdframe_norcy : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N56,
      O => mod5_hdframe_nor_cyo
    );
  mod5_hdframe_norlut1 : LUT4_L
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod5_hdframe_samplecount(4),
      I1 => mod5_hdframe_samplecount(5),
      I2 => mod5_hdframe_samplecount(6),
      I3 => mod5_hdframe_samplecount(11),
      LO => mod5_hdframe_N57
    );
  mod5_hdframe_norcy_rn_0 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N57,
      O => mod5_hdframe_nor_cyo1
    );
  mod5_hdframe_Eq_stagelut42 : LUT4_L
    generic map(
      INIT => X"60A0"
    )
    port map (
      I0 => mod5_hdframe_samplecount(9),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(10),
      I3 => sif_hd_format(1),
      LO => mod5_hdframe_N207
    );
  mod5_hdframe_Eq_stagecy : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo1,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N58,
      O => mod5_hdframe_Eq_stage_cyo
    );
  mod5_hdframe_Eq_stagelut : LUT4_L
    generic map(
      INIT => X"60A0"
    )
    port map (
      I0 => mod5_hdframe_samplecount(7),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(8),
      I3 => sif_hd_format(1),
      LO => mod5_hdframe_N58
    );
  mod7_n0034_0_62 : LUT4
    generic map(
      INIT => X"BAAA"
    )
    port map (
      I0 => mod7_n0025,
      I1 => mod7_tp_19_Q,
      I2 => mod7_tp_21_Q,
      I3 => mod7_n0030,
      O => CHOICE14101
    );
  mod5_hdframe_Neq_stagecy : MUXCY
    port map (
      CI => mod5_field1_vblnk1_4_Q,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N60,
      O => mod5_hdframe_Neq_stage_cyo
    );
  mod5_hdframe_n0069_0_1 : LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      I0 => mod5_hdframe_n0096,
      I1 => mod5_hdframe_linecount(0),
      O => mod5_hdframe_n0069(0)
    );
  mod5_hdframe_Neq_stagecy_rn_0 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N61,
      O => mod5_hdframe_Neq_stage_cyo1
    );
  mod5_hdframe_Neq_stagelut2 : LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_linecount(4),
      I1 => mod5_field1_vblnk1_4_Q,
      I2 => mod5_hdframe_linecount(5),
      I3 => mod5_tsl_4_Q,
      O => mod5_hdframe_N62
    );
  mod5_hdframe_Neq_stagecy_rn_1 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo1,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N62,
      O => mod5_hdframe_Neq_stage_cyo2
    );
  mod5_hdframe_Neq_stagelut1 : LUT4
    generic map(
      INIT => X"8222"
    )
    port map (
      I0 => mod5_hdframe_linecount(2),
      I1 => mod5_hdframe_linecount(3),
      I2 => sif_hd_format(1),
      I3 => sif_hd_format(2),
      O => mod5_hdframe_N61
    );
  mod5_hdframe_Neq_stagecy_rn_2 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo2,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N63,
      O => mod5_hdframe_Neq_stage_cyo3
    );
  mod5_hdframe_Neq_stagelut3 : LUT4
    generic map(
      INIT => X"8222"
    )
    port map (
      I0 => mod5_hdframe_linecount(6),
      I1 => mod5_hdframe_linecount(7),
      I2 => sif_hd_format(1),
      I3 => sif_hd_format(2),
      O => mod5_hdframe_N63
    );
  mod5_hdframe_Neq_stagecy_rn_3 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo3,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N64,
      O => mod5_hdframe_Neq_stage_cyo4
    );
  mod5_hdframe_Neq_stagelut4 : LUT4
    generic map(
      INIT => X"4111"
    )
    port map (
      I0 => mod5_hdframe_linecount(8),
      I1 => mod5_hdframe_linecount(9),
      I2 => sif_hd_format(1),
      I3 => sif_hd_format(2),
      O => mod5_hdframe_N64
    );
  mod5_hdframe_Eq_stagelut24 : LUT3_L
    generic map(
      INIT => X"81"
    )
    port map (
      I0 => mod5_hdframe_linecount(0),
      I1 => mod5_field2_vblnk1_5_Q,
      I2 => mod5_hdframe_linecount(1),
      LO => mod5_hdframe_N66
    );
  mod5_hdframe_Eq_stagecy_rn_1 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N66,
      O => mod5_hdframe_Eq_stage_cyo1
    );
  mod5_hdframe_Eq_stagelut310 : LUT2_L
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => mod5_hdframe_linecount(2),
      I1 => mod5_hdframe_linecount(3),
      LO => mod5_hdframe_N67
    );
  mod5_hdframe_Eq_stagecy_rn_2 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo1,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N67,
      O => mod5_hdframe_Eq_stage_cyo2
    );
  mod5_hdframe_Eq_stagelut47 : LUT3_L
    generic map(
      INIT => X"81"
    )
    port map (
      I0 => mod5_hdframe_linecount(4),
      I1 => mod5_field2_vblnk1_5_Q,
      I2 => mod5_hdframe_linecount(5),
      LO => mod5_hdframe_N68
    );
  mod5_hdframe_Eq_stagecy_rn_3 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo2,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N68,
      O => mod5_hdframe_Eq_stage_cyo3
    );
  mod5_hdframe_Eq_stagelut51 : LUT2_L
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => mod5_hdframe_linecount(6),
      I1 => mod5_hdframe_linecount(7),
      LO => mod5_hdframe_N69
    );
  mod5_hdframe_Eq_stagecy_rn_4 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo3,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N69,
      O => mod5_hdframe_Eq_stage_cyo4
    );
  mod5_hdframe_Eq_stagelut23 : LUT3_L
    generic map(
      INIT => X"09"
    )
    port map (
      I0 => mod5_field2_vblnk1_5_Q,
      I1 => mod5_hdframe_linecount(9),
      I2 => mod5_hdframe_linecount(8),
      LO => mod5_hdframe_N137
    );
  mod5_hdframe_norlut2 : LUT4_L
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => mod5_hdframe_samplecount(1),
      I2 => mod5_hdframe_samplecount(2),
      I3 => mod5_hdframe_samplecount(3),
      LO => mod5_hdframe_N71
    );
  mod5_hdframe_norcy_rn_1 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N71,
      O => mod5_hdframe_nor_cyo2
    );
  mod5_hdframe_norlut3 : LUT4_L
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod5_hdframe_samplecount(4),
      I1 => mod5_hdframe_samplecount(5),
      I2 => mod5_hdframe_samplecount(6),
      I3 => mod5_hdframe_samplecount(7),
      LO => mod5_hdframe_N72
    );
  mod5_hdframe_norcy_rn_2 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo2,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N72,
      O => mod5_hdframe_nor_cyo3
    );
  mod5_hdframe_norlut4 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod5_hdframe_samplecount(8),
      I1 => mod5_hdframe_samplecount(9),
      I2 => mod5_hdframe_samplecount(10),
      I3 => mod5_hdframe_samplecount(11),
      O => mod5_hdframe_N73
    );
  mod5_hdframe_norcy_rn_3 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo3,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N73,
      O => mod5_hdframe_nor_cyo4
    );
  mod7_color_v1h8_y_9_1 : LUT3
    generic map(
      INIT => X"64"
    )
    port map (
      I0 => mod0_pattern(0),
      I1 => mod0_pattern(2),
      I2 => mod0_pattern(1),
      O => mod7_color_v1h8_y(9)
    );
  mod5_hdframe_XNor_stagecy : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_hdframe_linecount(0),
      S => mod5_hdframe_N75,
      O => mod5_hdframe_XNor_stage_cyo
    );
  mod5_hdframe_XNor_stagelut1 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => mod5_hdframe_linecount(1),
      I1 => mod5_field1_vblnk1_0_Q,
      O => mod5_hdframe_N76
    );
  mod5_hdframe_XNor_stagecy_rn_0 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo,
      DI => mod5_hdframe_linecount(1),
      S => mod5_hdframe_N76,
      O => mod5_hdframe_XNor_stage_cyo1
    );
  mod5_hdframe_XNor_stagelut2 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => mod5_hdframe_linecount(2),
      I1 => mod5_field1_vblnk1_0_Q,
      O => mod5_hdframe_N77
    );
  mod5_hdframe_XNor_stagecy_rn_1 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo1,
      DI => mod5_hdframe_linecount(2),
      S => mod5_hdframe_N77,
      O => mod5_hdframe_XNor_stage_cyo2
    );
  mod5_hdframe_XNor_stagelut3 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => mod5_hdframe_linecount(3),
      I1 => mod5_field1_vblnk1_0_Q,
      O => mod5_hdframe_N78
    );
  mod5_hdframe_XNor_stagecy_rn_2 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo2,
      DI => mod5_hdframe_linecount(3),
      S => mod5_hdframe_N78,
      O => mod5_hdframe_XNor_stage_cyo3
    );
  mod5_hdframe_hd_framegenerator_n0083_3_lut2 : LUT4
    generic map(
      INIT => X"FEBA"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(2),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(1),
      O => N7978
    );
  mod5_hdframe_XNor_stagecy_rn_3 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo3,
      DI => mod5_hdframe_linecount(4),
      S => mod5_hdframe_N79,
      O => mod5_hdframe_XNor_stage_cyo4
    );
  mod5_hdframe_XNor_stagelut5 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => mod5_hdframe_linecount(5),
      I1 => mod5_field2_vblnk1_5_Q,
      O => mod5_hdframe_N80
    );
  mod5_hdframe_XNor_stagecy_rn_4 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo4,
      DI => mod5_hdframe_linecount(5),
      S => mod5_hdframe_N80,
      O => mod5_hdframe_XNor_stage_cyo5
    );
  mod5_hdframe_XNor_stagelut61_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(6),
      O => mod5_hdframe_N81
    );
  mod5_hdframe_XNor_stagecy_rn_5 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo5,
      DI => mod5_hdframe_linecount(6),
      S => mod5_hdframe_N81,
      O => mod5_hdframe_XNor_stage_cyo6
    );
  mod5_hdframe_XNor_stagelut71_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(7),
      O => mod5_hdframe_N82
    );
  mod5_hdframe_XNor_stagecy_rn_6 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo6,
      DI => mod5_hdframe_linecount(7),
      S => mod5_hdframe_N82,
      O => mod5_hdframe_XNor_stage_cyo7
    );
  mod5_hdframe_XNor_stagelut81_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(8),
      O => mod5_hdframe_N83
    );
  mod5_hdframe_XNor_stagecy_rn_7 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo7,
      DI => mod5_hdframe_linecount(8),
      S => mod5_hdframe_N83,
      O => mod5_hdframe_XNor_stage_cyo8
    );
  mod5_hdframe_XNor_stagelut9 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => mod5_hdframe_linecount(9),
      I1 => mod5_field2_vblnk1_5_Q,
      O => mod5_hdframe_N84
    );
  mod5_hdframe_XNor_stagecy_rn_8 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo8,
      DI => mod5_hdframe_linecount(9),
      S => mod5_hdframe_N84,
      O => mod5_hdframe_XNor_stage_cyo9
    );
  mod5_hdframe_XNor_stagelut101_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(10),
      O => mod5_hdframe_N85
    );
  mod5_hdframe_XNor_stagelut11 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => mod5_hdframe_linecount(0),
      I1 => mod5_field2_vblnk1_5_Q,
      O => mod5_hdframe_N86
    );
  mod5_hdframe_XNor_stagecy_rn_10 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_hdframe_linecount(0),
      S => mod5_hdframe_N86,
      O => mod5_hdframe_XNor_stage_cyo10
    );
  mod5_hdframe_XNor_stagelut12 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => mod5_hdframe_linecount(1),
      I1 => mod5_field1_vblnk1_1_Q,
      O => mod5_hdframe_N87
    );
  mod5_hdframe_XNor_stagecy_rn_11 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo10,
      DI => mod5_hdframe_linecount(1),
      S => mod5_hdframe_N87,
      O => mod5_hdframe_XNor_stage_cyo11
    );
  mod5_hdframe_hd_framegenerator_n0088_11_lut : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(3),
      O => mod5_hdframe_N54
    );
  mod5_hdframe_XNor_stagecy_rn_12 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo11,
      DI => mod5_hdframe_linecount(2),
      S => mod5_hdframe_N88,
      O => mod5_hdframe_XNor_stage_cyo12
    );
  mod5_hdframe_XNor_stagelut4 : LUT4
    generic map(
      INIT => X"3336"
    )
    port map (
      I0 => mod0_hd_format_tmp(1),
      I1 => mod5_hdframe_linecount(4),
      I2 => mod0_hd_format_tmp(3),
      I3 => mod0_hd_format_tmp(2),
      O => mod5_hdframe_N79
    );
  mod5_hdframe_XNor_stagecy_rn_13 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo12,
      DI => mod5_hdframe_linecount(3),
      S => mod5_hdframe_N89,
      O => mod5_hdframe_XNor_stage_cyo13
    );
  mod5_hdframe_XNor_stagelut13 : LUT4
    generic map(
      INIT => X"3336"
    )
    port map (
      I0 => mod0_hd_format_tmp(1),
      I1 => mod5_hdframe_linecount(2),
      I2 => mod0_hd_format_tmp(3),
      I3 => mod0_hd_format_tmp(2),
      O => mod5_hdframe_N88
    );
  mod5_hdframe_XNor_stagecy_rn_14 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo13,
      DI => mod5_hdframe_linecount(4),
      S => mod5_hdframe_N90,
      O => mod5_hdframe_XNor_stage_cyo14
    );
  mod5_hdframe_hd_framegenerator_n0083_10_lut : LUT3
    generic map(
      INIT => X"15"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(2),
      O => mod5_hdframe_N22
    );
  mod5_hdframe_XNor_stagecy_rn_15 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo14,
      DI => mod5_hdframe_linecount(5),
      S => mod5_hdframe_N91,
      O => mod5_hdframe_XNor_stage_cyo15
    );
  mod5_hdframe_XNor_stagelut171_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(6),
      O => mod5_hdframe_N92
    );
  mod5_hdframe_XNor_stagecy_rn_16 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo15,
      DI => mod5_hdframe_linecount(6),
      S => mod5_hdframe_N92,
      O => mod5_hdframe_XNor_stage_cyo16
    );
  mod5_hdframe_XNor_stagelut181_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(7),
      O => mod5_hdframe_N93
    );
  mod5_hdframe_XNor_stagecy_rn_17 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo16,
      DI => mod5_hdframe_linecount(7),
      S => mod5_hdframe_N93,
      O => mod5_hdframe_XNor_stage_cyo17
    );
  mod5_hdframe_XNor_stagelut191_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(8),
      O => mod5_hdframe_N94
    );
  mod5_hdframe_XNor_stagecy_rn_18 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo17,
      DI => mod5_hdframe_linecount(8),
      S => mod5_hdframe_N94,
      O => mod5_hdframe_XNor_stage_cyo18
    );
  mod5_hdframe_XNor_stagelut201_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(9),
      O => mod5_hdframe_N95
    );
  mod5_hdframe_XNor_stagecy_rn_19 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo18,
      DI => mod5_hdframe_linecount(9),
      S => mod5_hdframe_N95,
      O => mod5_hdframe_XNor_stage_cyo19
    );
  mod5_hdframe_XNor_stagelut211_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(10),
      O => mod5_hdframe_N96
    );
  mod5_hdframe_XNor_stagecy_rn_20 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo19,
      DI => mod5_hdframe_linecount(10),
      S => mod5_hdframe_N96,
      O => mod5_hdframe_XNor_stage_cyo20
    );
  mod5_hdframe_XNor_stagelut22 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => mod5_hdframe_linecount(0),
      I1 => mod5_field1_vblnk1_0_Q,
      O => mod5_hdframe_N98
    );
  mod5_hdframe_XNor_stagecy_rn_21 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_hdframe_linecount(0),
      S => mod5_hdframe_N98,
      O => mod5_hdframe_XNor_stage_cyo21
    );
  mod5_hdframe_XNor_stagelut23 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => mod5_hdframe_linecount(1),
      I1 => mod5_field1_vblnk1_0_Q,
      O => mod5_hdframe_N99
    );
  mod5_hdframe_XNor_stagecy_rn_22 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo21,
      DI => mod5_hdframe_linecount(1),
      S => mod5_hdframe_N99,
      O => mod5_hdframe_XNor_stage_cyo22
    );
  mod5_hdframe_XNor_stagelut241_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(2),
      O => mod5_hdframe_N100
    );
  mod5_hdframe_XNor_stagecy_rn_23 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo22,
      DI => mod5_hdframe_linecount(2),
      S => mod5_hdframe_N100,
      O => mod5_hdframe_XNor_stage_cyo23
    );
  mod5_hdframe_XNor_stagelut25 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => mod5_hdframe_linecount(3),
      I1 => mod5_field2_vblnk1_5_Q,
      O => mod5_hdframe_N101
    );
  mod5_hdframe_XNor_stagecy_rn_24 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo23,
      DI => mod5_hdframe_linecount(3),
      S => mod5_hdframe_N101,
      O => mod5_hdframe_XNor_stage_cyo24
    );
  mod5_hdframe_XNor_stagelut26 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => mod5_hdframe_linecount(4),
      I1 => mod5_field1_vblnk1_0_Q,
      O => mod5_hdframe_N102
    );
  mod5_hdframe_XNor_stagecy_rn_25 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo24,
      DI => mod5_hdframe_linecount(4),
      S => mod5_hdframe_N102,
      O => mod5_hdframe_XNor_stage_cyo25
    );
  mod5_hdframe_XNor_stagelut271_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(5),
      O => mod5_hdframe_N103
    );
  mod5_hdframe_XNor_stagecy_rn_26 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo25,
      DI => mod5_hdframe_linecount(5),
      S => mod5_hdframe_N103,
      O => mod5_hdframe_XNor_stage_cyo26
    );
  mod5_hdframe_XNor_stagelut28 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => mod5_hdframe_linecount(6),
      I1 => mod5_field2_vblnk1_5_Q,
      O => mod5_hdframe_N104
    );
  mod5_hdframe_XNor_stagecy_rn_27 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo26,
      DI => mod5_hdframe_linecount(6),
      S => mod5_hdframe_N104,
      O => mod5_hdframe_XNor_stage_cyo27
    );
  mod5_hdframe_XNor_stagelut291_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(7),
      O => mod5_hdframe_N105
    );
  mod5_hdframe_XNor_stagecy_rn_28 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo27,
      DI => mod5_hdframe_linecount(7),
      S => mod5_hdframe_N105,
      O => mod5_hdframe_XNor_stage_cyo28
    );
  mod5_hdframe_XNor_stagelut301_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(8),
      O => mod5_hdframe_N106
    );
  mod5_hdframe_XNor_stagecy_rn_29 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo28,
      DI => mod5_hdframe_linecount(8),
      S => mod5_hdframe_N106,
      O => mod5_hdframe_XNor_stage_cyo29
    );
  mod5_hdframe_XNor_stagelut31 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => mod5_hdframe_linecount(9),
      I1 => mod5_field2_vblnk1_5_Q,
      O => mod5_hdframe_N107
    );
  mod5_hdframe_XNor_stagecy_rn_30 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo29,
      DI => mod5_hdframe_linecount(9),
      S => mod5_hdframe_N107,
      O => mod5_hdframe_XNor_stage_cyo30
    );
  mod5_hdframe_XNor_stagelut321_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(10),
      O => mod5_hdframe_N108
    );
  mod5_hdframe_XNor_stagecy_rn_31 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo30,
      DI => mod5_hdframe_linecount(10),
      S => mod5_hdframe_N108,
      O => mod5_hdframe_XNor_stage_cyo31
    );
  mod5_hdframe_XNor_stagelut33 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => mod5_hdframe_linecount(0),
      I1 => mod5_field1_vblnk1_0_Q,
      O => mod5_hdframe_N110
    );
  mod5_hdframe_XNor_stagecy_rn_32 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_hdframe_linecount(0),
      S => mod5_hdframe_N110,
      O => mod5_hdframe_XNor_stage_cyo32
    );
  mod5_hdframe_XNor_stagelut34 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => mod5_hdframe_linecount(1),
      I1 => mod5_field1_vblnk1_1_Q,
      O => mod5_hdframe_N111
    );
  mod5_hdframe_XNor_stagecy_rn_33 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo32,
      DI => mod5_hdframe_linecount(1),
      S => mod5_hdframe_N111,
      O => mod5_hdframe_XNor_stage_cyo33
    );
  mod5_hdframe_XNor_stagelut15 : LUT4
    generic map(
      INIT => X"AA96"
    )
    port map (
      I0 => mod5_hdframe_linecount(4),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(2),
      I3 => mod0_hd_format_tmp(3),
      O => mod5_hdframe_N90
    );
  mod5_hdframe_XNor_stagecy_rn_34 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo33,
      DI => mod5_hdframe_linecount(2),
      S => mod5_hdframe_N112,
      O => mod5_hdframe_XNor_stage_cyo34
    );
  mod5_hdframe_XNor_stagelut14 : LUT4
    generic map(
      INIT => X"CCC9"
    )
    port map (
      I0 => mod0_hd_format_tmp(1),
      I1 => mod5_hdframe_linecount(3),
      I2 => mod0_hd_format_tmp(3),
      I3 => mod0_hd_format_tmp(2),
      O => mod5_hdframe_N89
    );
  mod5_hdframe_XNor_stagecy_rn_35 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo34,
      DI => mod5_hdframe_linecount(3),
      S => mod5_hdframe_N113,
      O => mod5_hdframe_XNor_stage_cyo35
    );
  mod5_hdframe_XNor_stagelut371_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(4),
      O => mod5_hdframe_N114
    );
  mod5_hdframe_XNor_stagecy_rn_36 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo35,
      DI => mod5_hdframe_linecount(4),
      S => mod5_hdframe_N114,
      O => mod5_hdframe_XNor_stage_cyo36
    );
  mod5_hdframe_n0075_7_89 : LUT4
    generic map(
      INIT => X"FE54"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => mod7_color_N42,
      I2 => mod7_color_N24,
      I3 => N8083,
      O => CHOICE14728
    );
  mod5_hdframe_XNor_stagecy_rn_37 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo36,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_linecount_5_rt1,
      O => mod5_hdframe_XNor_stage_cyo37
    );
  mod5_hdframe_n0075_6_114 : LUT4
    generic map(
      INIT => X"5554"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => CHOICE14788,
      I2 => CHOICE14786,
      I3 => CHOICE14783,
      O => CHOICE14792
    );
  mod5_hdframe_XNor_stagecy_rn_38 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo37,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_linecount_6_rt1,
      O => mod5_hdframe_XNor_stage_cyo38
    );
  mod7_n0047_0_69_SW0 : LUT4
    generic map(
      INIT => X"0103"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(1),
      I2 => mod0_pattern(0),
      I3 => mod7_active_line(6),
      O => N8165
    );
  mod5_hdframe_XNor_stagecy_rn_39 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo38,
      DI => mod5_hdframe_linecount(7),
      S => mod5_hdframe_N117,
      O => mod5_hdframe_XNor_stage_cyo39
    );
  mod5_hdframe_XNor_stagelut411_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(8),
      O => mod5_hdframe_N118
    );
  mod5_hdframe_XNor_stagecy_rn_40 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo39,
      DI => mod5_hdframe_linecount(8),
      S => mod5_hdframe_N118,
      O => mod5_hdframe_XNor_stage_cyo40
    );
  mod5_hdframe_XNor_stagelut40 : LUT4
    generic map(
      INIT => X"AA95"
    )
    port map (
      I0 => mod5_hdframe_linecount(7),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(2),
      I3 => mod0_hd_format_tmp(3),
      O => mod5_hdframe_N117
    );
  mod5_hdframe_XNor_stagecy_rn_41 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo40,
      DI => mod5_hdframe_linecount(9),
      S => mod5_hdframe_N119,
      O => mod5_hdframe_XNor_stage_cyo41
    );
  mod5_hdframe_XNor_stagelut16 : LUT4
    generic map(
      INIT => X"5659"
    )
    port map (
      I0 => mod5_hdframe_linecount(5),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(3),
      I3 => mod0_hd_format_tmp(2),
      O => mod5_hdframe_N91
    );
  mod5_hdframe_Eq_stagelut7 : LUT4_L
    generic map(
      INIT => X"8421"
    )
    port map (
      I0 => mod5_field1_vblnk1_1_Q,
      I1 => mod5_field1_vblnk1_0_Q,
      I2 => mod5_hdframe_linecount(1),
      I3 => mod5_hdframe_linecount(0),
      LO => mod5_hdframe_N121
    );
  mod5_hdframe_Eq_stagecy_rn_6 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N121,
      O => mod5_hdframe_Eq_stage_cyo5
    );
  mod5_hdframe_Eq_stagelut44 : LUT4_L
    generic map(
      INIT => X"60A0"
    )
    port map (
      I0 => mod5_hdframe_samplecount(9),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(10),
      I3 => sif_hd_format(1),
      LO => mod5_hdframe_N212
    );
  mod5_hdframe_Eq_stagecy_rn_7 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo5,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N122,
      O => mod5_hdframe_Eq_stage_cyo6
    );
  mod5_hdframe_Eq_stagelut9 : LUT4_L
    generic map(
      INIT => X"8421"
    )
    port map (
      I0 => mod5_tsl_4_Q,
      I1 => mod5_field1_vblnk1_4_Q,
      I2 => mod5_hdframe_linecount(5),
      I3 => mod5_hdframe_linecount(4),
      LO => mod5_hdframe_N123
    );
  mod5_hdframe_Eq_stagecy_rn_8 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo6,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N123,
      O => mod5_hdframe_Eq_stage_cyo7
    );
  mod5_hdframe_Eq_stagelut6 : LUT3_L
    generic map(
      INIT => X"09"
    )
    port map (
      I0 => mod5_field2_vblnk1_5_Q,
      I1 => mod5_hdframe_linecount(9),
      I2 => mod5_hdframe_linecount(8),
      LO => mod5_hdframe_N70
    );
  mod5_hdframe_Eq_stagecy_rn_9 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo7,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N124,
      O => mod5_hdframe_Eq_stage_cyo8
    );
  mod5_hdframe_n0070_4_224_SW1_SW0 : LUT4_L
    generic map(
      INIT => X"2882"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_luma_crc(14),
      I2 => mod5_hdframe_n0177(0),
      I3 => mod5_hdframe_n0179(0),
      LO => N8502
    );
  mod5_hdframe_Eq_stagecy_rn_10 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo8,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N125,
      O => mod5_hdframe_Eq_stage_cyo9
    );
  mod5_hdframe_Eq_stagelut10 : LUT4_L
    generic map(
      INIT => X"8700"
    )
    port map (
      I0 => sif_hd_format(2),
      I1 => sif_hd_format(1),
      I2 => mod5_hdframe_linecount(7),
      I3 => mod5_hdframe_linecount(6),
      LO => mod5_hdframe_N124
    );
  mod5_hdframe_Eq_stagelut131 : LUT3_L
    generic map(
      INIT => X"81"
    )
    port map (
      I0 => mod5_hdframe_linecount(0),
      I1 => mod5_field1_vblnk1_0_Q,
      I2 => mod5_hdframe_linecount(1),
      LO => mod5_hdframe_N127
    );
  mod5_hdframe_Eq_stagecy_rn_12 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N127,
      O => mod5_hdframe_Eq_stage_cyo10
    );
  mod7_color_n00221 : LUT3_D
    generic map(
      INIT => X"04"
    )
    port map (
      I0 => mod7_h_state_FFd2_4,
      I1 => mod7_color_N132,
      I2 => mod7_tp_1_Q,
      LO => N8639,
      O => mod7_color_n0022
    );
  mod5_hdframe_Eq_stagecy_rn_13 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo10,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N128,
      O => mod5_hdframe_Eq_stage_cyo11
    );
  mod5_hdframe_Eq_stagelut15 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_linecount(4),
      I1 => mod5_field1_vblnk1_0_Q,
      I2 => mod5_hdframe_linecount(5),
      I3 => mod5_field1_vblnk1_4_Q,
      LO => mod5_hdframe_N129
    );
  mod5_hdframe_Eq_stagecy_rn_14 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo11,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N129,
      O => mod5_hdframe_Eq_stage_cyo12
    );
  mod5_hdframe_Eq_stagelut16 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_linecount(6),
      I1 => mod5_field2_vblnk1_5_Q,
      I2 => mod5_hdframe_linecount(7),
      I3 => mod5_field1_vblnk1_4_Q,
      LO => mod5_hdframe_N130
    );
  mod5_hdframe_Eq_stagecy_rn_15 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo12,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N130,
      O => mod5_hdframe_Eq_stage_cyo13
    );
  mod5_hdframe_Eq_stagelut11 : LUT4_L
    generic map(
      INIT => X"0807"
    )
    port map (
      I0 => sif_hd_format(2),
      I1 => sif_hd_format(1),
      I2 => mod5_hdframe_linecount(8),
      I3 => mod5_hdframe_linecount(9),
      LO => mod5_hdframe_N125
    );
  mod5_hdframe_Eq_stagecy_rn_16 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo13,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N131,
      O => mod5_hdframe_Eq_stage_cyo14
    );
  mod5_hdframe_Eq_stagelut181_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(10),
      O => mod5_hdframe_N132
    );
  mod5_hdframe_Eq_stagelut19 : LUT4_L
    generic map(
      INIT => X"8421"
    )
    port map (
      I0 => mod5_field1_vblnk1_0_Q,
      I1 => mod5_field2_vblnk1_0_Q,
      I2 => mod5_hdframe_linecount(1),
      I3 => mod5_hdframe_linecount(0),
      LO => mod5_hdframe_N133
    );
  mod5_hdframe_Eq_stagecy_rn_18 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N133,
      O => mod5_hdframe_Eq_stage_cyo15
    );
  mod5_hdframe_Eq_stagelut201 : LUT3_L
    generic map(
      INIT => X"81"
    )
    port map (
      I0 => mod5_hdframe_linecount(2),
      I1 => mod5_field1_vblnk1_0_Q,
      I2 => mod5_hdframe_linecount(3),
      LO => mod5_hdframe_N134
    );
  mod5_hdframe_Eq_stagecy_rn_19 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo15,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N134,
      O => mod5_hdframe_Eq_stage_cyo16
    );
  mod5_hdframe_Eq_stagelut21 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_linecount(4),
      I1 => mod5_field2_vblnk1_0_Q,
      I2 => mod5_hdframe_linecount(5),
      I3 => mod5_field2_vblnk1_5_Q,
      LO => mod5_hdframe_N135
    );
  mod5_hdframe_Eq_stagecy_rn_20 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo16,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N135,
      O => mod5_hdframe_Eq_stage_cyo17
    );
  mod5_hdframe_Eq_stagelut221 : LUT2_L
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => mod5_hdframe_linecount(6),
      I1 => mod5_hdframe_linecount(7),
      LO => mod5_hdframe_N136
    );
  mod5_hdframe_Eq_stagecy_rn_21 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo17,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N136,
      O => mod5_hdframe_Eq_stage_cyo18
    );
  mod5_hdframe_Eq_stagelut17 : LUT3_L
    generic map(
      INIT => X"09"
    )
    port map (
      I0 => mod5_field2_vblnk1_5_Q,
      I1 => mod5_hdframe_linecount(9),
      I2 => mod5_hdframe_linecount(8),
      LO => mod5_hdframe_N131
    );
  mod5_hdframe_Eq_stagecy_rn_22 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo18,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N137,
      O => mod5_hdframe_Eq_stage_cyo19
    );
  mod5_hdframe_Eq_stagelut241_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(10),
      O => mod5_hdframe_N138
    );
  mod5_hdframe_Eq_stagelut25 : LUT4_L
    generic map(
      INIT => X"8421"
    )
    port map (
      I0 => mod5_field1_vblnk1_1_Q,
      I1 => mod5_field2_vblnk1_5_Q,
      I2 => mod5_hdframe_linecount(1),
      I3 => mod5_hdframe_linecount(0),
      LO => mod5_hdframe_N139
    );
  mod5_hdframe_Eq_stagecy_rn_24 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N139,
      O => mod5_hdframe_Eq_stage_cyo20
    );
  mod5_hdframe_Eq_stagelut8 : LUT4_L
    generic map(
      INIT => X"0816"
    )
    port map (
      I0 => sif_hd_format(2),
      I1 => sif_hd_format(1),
      I2 => mod5_hdframe_linecount(2),
      I3 => mod5_hdframe_linecount(3),
      LO => mod5_hdframe_N122
    );
  mod5_hdframe_Eq_stagecy_rn_25 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo20,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N140,
      O => mod5_hdframe_Eq_stage_cyo21
    );
  mod5_hdframe_Eq_stagelut26 : LUT4_L
    generic map(
      INIT => X"5042"
    )
    port map (
      I0 => mod5_hdframe_linecount(2),
      I1 => sif_hd_format(1),
      I2 => mod5_hdframe_linecount(3),
      I3 => sif_hd_format(2),
      LO => mod5_hdframe_N140
    );
  mod5_hdframe_Eq_stagecy_rn_26 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo21,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N141,
      O => mod5_hdframe_Eq_stage_cyo22
    );
  mod5_hdframe_Eq_stagelut281 : LUT2_L
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => mod5_hdframe_linecount(6),
      I1 => mod5_hdframe_linecount(7),
      LO => mod5_hdframe_N142
    );
  mod5_hdframe_Eq_stagecy_rn_27 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo22,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N142,
      O => mod5_hdframe_Eq_stage_cyo23
    );
  mod5_hdframe_Eq_stagelut291 : LUT2_L
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => mod5_hdframe_linecount(8),
      I1 => mod5_hdframe_linecount(9),
      LO => mod5_hdframe_N143
    );
  mod5_hdframe_Eq_stagecy_rn_28 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo23,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N143,
      O => mod5_hdframe_Eq_stage_cyo24
    );
  mod5_hdframe_Eq_stagelut301_INV_0 : INV
    port map (
      I => mod5_hdframe_linecount(10),
      O => mod5_hdframe_N144
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_0_lut_INV_0 : INV
    port map (
      I => mod5_hdframe_samplecount(0),
      O => mod5_hdframe_N145
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_0_cy : MUXCY
    port map (
      CI => mod5_field1_vblnk1_4_Q,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N145,
      O => mod5_hdframe_hd_framegenerator_samplecount_n0000_0_cyo
    );
  SCK_BUFGP_34 : BUFGP
    port map (
      I => SCK,
      O => SCK_BUFGP
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_1_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_0_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_samplecount_1_rt,
      O => mod5_hdframe_hd_framegenerator_samplecount_n0000_1_cyo
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_1_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_0_cyo,
      LI => mod5_hdframe_samplecount_1_rt,
      O => mod5_hdframe_samplecount_n0000(1)
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_2_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_1_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_samplecount_2_rt1,
      O => mod5_hdframe_hd_framegenerator_samplecount_n0000_2_cyo
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_2_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_1_cyo,
      LI => mod5_hdframe_samplecount_2_rt1,
      O => mod5_hdframe_samplecount_n0000(2)
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_3_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_2_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_samplecount_3_rt,
      O => mod5_hdframe_hd_framegenerator_samplecount_n0000_3_cyo
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_3_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_2_cyo,
      LI => mod5_hdframe_samplecount_3_rt,
      O => mod5_hdframe_samplecount_n0000(3)
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_4_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_3_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_samplecount_4_rt,
      O => mod5_hdframe_hd_framegenerator_samplecount_n0000_4_cyo
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_4_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_3_cyo,
      LI => mod5_hdframe_samplecount_4_rt,
      O => mod5_hdframe_samplecount_n0000(4)
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_5_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_4_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_samplecount_5_rt,
      O => mod5_hdframe_hd_framegenerator_samplecount_n0000_5_cyo
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_5_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_4_cyo,
      LI => mod5_hdframe_samplecount_5_rt,
      O => mod5_hdframe_samplecount_n0000(5)
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_6_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_5_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_samplecount_6_rt,
      O => mod5_hdframe_hd_framegenerator_samplecount_n0000_6_cyo
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_6_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_5_cyo,
      LI => mod5_hdframe_samplecount_6_rt,
      O => mod5_hdframe_samplecount_n0000(6)
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_7_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_6_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_samplecount_7_rt,
      O => mod5_hdframe_hd_framegenerator_samplecount_n0000_7_cyo
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_7_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_6_cyo,
      LI => mod5_hdframe_samplecount_7_rt,
      O => mod5_hdframe_samplecount_n0000(7)
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_8_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_7_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_samplecount_8_rt1,
      O => mod5_hdframe_hd_framegenerator_samplecount_n0000_8_cyo
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_8_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_7_cyo,
      LI => mod5_hdframe_samplecount_8_rt1,
      O => mod5_hdframe_samplecount_n0000(8)
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_9_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_8_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_samplecount_9_rt,
      O => mod5_hdframe_hd_framegenerator_samplecount_n0000_9_cyo
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_9_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_8_cyo,
      LI => mod5_hdframe_samplecount_9_rt,
      O => mod5_hdframe_samplecount_n0000(9)
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_10_cy : MUXCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_9_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_samplecount_10_rt1,
      O => mod5_hdframe_hd_framegenerator_samplecount_n0000_10_cyo
    );
  mod5_hdframe_hd_framegenerator_samplecount_n0000_10_xor : XORCY
    port map (
      CI => mod5_hdframe_hd_framegenerator_samplecount_n0000_9_cyo,
      LI => mod5_hdframe_samplecount_10_rt1,
      O => mod5_hdframe_samplecount_n0000(10)
    );
  mod5_hdframe_n0074_10_1 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0099,
      I1 => mod5_hdframe_luma_crc0(15),
      I2 => mod5_hdframe_n0175(0),
      I3 => mod5_hdframe_luma_crc0(14),
      LO => mod5_hdframe_n0074(10)
    );
  mod7_color_Ker511_G : LUT4_L
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_color_N134,
      I1 => mod7_color_N120,
      I2 => mod7_color_v0h2_y_2_Q,
      I3 => mod7_color_v0h2_y_0_Q,
      LO => N8533
    );
  mod5_hdframe_Eq_stagecy_rn_30 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N146,
      O => mod5_hdframe_Eq_stage_cyo25
    );
  mod5_hdframe_Eq_stagelut32 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(2),
      I1 => mod5_hdframe_n0083_2_Q,
      I2 => mod5_hdframe_samplecount(3),
      I3 => mod5_hdframe_n0083_3_Q,
      LO => mod5_hdframe_N147
    );
  mod5_hdframe_Eq_stagecy_rn_31 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo25,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N147,
      O => mod5_hdframe_Eq_stage_cyo26
    );
  mod5_hdframe_Eq_stagelut33 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(4),
      I1 => mod5_hdframe_hd_framegenerator_n0083_3_cyo,
      I2 => mod5_hdframe_samplecount(5),
      I3 => mod5_hdframe_n0083_5_Q,
      LO => mod5_hdframe_N148
    );
  mod5_hdframe_Eq_stagecy_rn_32 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo26,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N148,
      O => mod5_hdframe_Eq_stage_cyo27
    );
  mod5_hdframe_Eq_stagelut34 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(6),
      I1 => mod5_hdframe_n0083_6_Q,
      I2 => mod5_hdframe_samplecount(7),
      I3 => mod5_hdframe_n0083_7_Q,
      LO => mod5_hdframe_N149
    );
  mod5_hdframe_Eq_stagecy_rn_33 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo27,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N149,
      O => mod5_hdframe_Eq_stage_cyo28
    );
  mod5_hdframe_Eq_stagelut35 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(8),
      I1 => mod5_field1_vblnk1_4_Q,
      I2 => mod5_hdframe_samplecount(9),
      I3 => mod5_hdframe_n0083_9_Q,
      LO => mod5_hdframe_N150
    );
  mod5_hdframe_Eq_stagecy_rn_34 : MUXCY
    port map (
      CI => mod5_hdframe_Eq_stage_cyo28,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N150,
      O => mod5_hdframe_Eq_stage_cyo29
    );
  mod5_hdframe_Eq_stagelut36 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(10),
      I1 => mod5_hdframe_n0083_10_Q,
      I2 => mod5_hdframe_samplecount(11),
      I3 => mod5_hdframe_n0083_11_Q,
      LO => mod5_hdframe_N151
    );
  mod5_hdframe_norlut5 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod5_hdframe_samplecount(2),
      I1 => mod5_hdframe_samplecount(3),
      I2 => mod5_hdframe_samplecount(4),
      I3 => mod5_hdframe_samplecount(5),
      O => mod5_hdframe_N152
    );
  mod5_hdframe_norcy_rn_4 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N152,
      O => mod5_hdframe_nor_cyo5
    );
  mod5_hdframe_norlut6_INV_0 : INV
    port map (
      I => mod5_hdframe_samplecount(6),
      O => mod5_hdframe_N153
    );
  mod5_hdframe_norcy_rn_5 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo5,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N153,
      O => mod5_hdframe_nor_cyo6
    );
  mod7_n0045_3_0 : LUT3
    generic map(
      INIT => X"A8"
    )
    port map (
      I0 => mod7_v_state(3),
      I1 => CHOICE12713,
      I2 => CHOICE12759,
      O => CHOICE14042
    );
  mod5_hdframe_XNor_stagecy_rn_43 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo6,
      DI => mod5_asl(7),
      S => mod5_hdframe_N154,
      O => mod5_hdframe_XNor_stage_cyo42
    );
  mod7_n0034_6_64_SW0 : LUT4_L
    generic map(
      INIT => X"FFEC"
    )
    port map (
      I0 => CHOICE14490,
      I1 => CHOICE14484,
      I2 => mod7_n0028,
      I3 => CHOICE14495,
      LO => N8149
    );
  mod5_hdframe_XNor_stagecy_rn_44 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo42,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_samplecount_8_rt2,
      O => mod5_hdframe_XNor_stage_cyo43
    );
  mod7_color_Ker961 : LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(0),
      I2 => mod0_pattern(1),
      I3 => mod7_color_n0023,
      O => mod7_color_N96
    );
  mod5_hdframe_XNor_stagecy_rn_45 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo43,
      DI => mod5_asl(7),
      S => mod5_hdframe_N156,
      O => mod5_hdframe_XNor_stage_cyo44
    );
  mod7_n0034_6_4 : LUT4
    generic map(
      INIT => X"8A82"
    )
    port map (
      I0 => mod7_h_state_FFd1,
      I1 => mod7_h_state_FFd4,
      I2 => mod7_h_state_FFd3,
      I3 => mod7_v_state(2),
      O => CHOICE14484
    );
  mod5_hdframe_XNor_stagecy_rn_46 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo44,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_samplecount_10_rt2,
      O => mod5_hdframe_XNor_stage_cyo45
    );
  mod5_hdframe_norlut7_INV_0 : INV
    port map (
      I => mod5_hdframe_samplecount(11),
      O => mod5_hdframe_N158
    );
  mod5_hdframe_norcy_rn_6 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo45,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N158,
      O => mod5_hdframe_nor_cyo7
    );
  mod5_hdframe_norlut8 : LUT4_L
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod5_hdframe_samplecount(3),
      I1 => mod5_hdframe_samplecount(4),
      I2 => mod5_hdframe_samplecount(5),
      I3 => mod5_hdframe_samplecount(6),
      LO => mod5_hdframe_N160
    );
  mod5_hdframe_norcy_rn_7 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N160,
      O => mod5_hdframe_nor_cyo8
    );
  mod5_hdframe_norlut9_INV_0 : INV
    port map (
      I => mod5_hdframe_samplecount(11),
      O => mod5_hdframe_N161
    );
  mod5_hdframe_norcy_rn_8 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo8,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N161,
      O => mod5_hdframe_nor_cyo9
    );
  mod5_hdframe_Andlut : LUT3_L
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => mod5_hdframe_samplecount(1),
      I1 => mod5_hdframe_samplecount(2),
      I2 => mod5_hdframe_samplecount_0_1,
      LO => mod5_hdframe_N162
    );
  mod5_hdframe_Andcy : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo9,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N162,
      O => mod5_hdframe_And_cyo
    );
  mod5_hdframe_Eq_stagelut45 : LUT4_L
    generic map(
      INIT => X"60A0"
    )
    port map (
      I0 => mod5_hdframe_samplecount(7),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(8),
      I3 => sif_hd_format(1),
      LO => mod5_hdframe_N216
    );
  mod5_hdframe_Eq_stagecy_rn_36 : MUXCY
    port map (
      CI => mod5_hdframe_And_cyo,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N163,
      O => mod5_hdframe_Eq_stage_cyo30
    );
  mod5_hdframe_Eq_stagelut46 : LUT4_L
    generic map(
      INIT => X"60A0"
    )
    port map (
      I0 => mod5_hdframe_samplecount(9),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(10),
      I3 => sif_hd_format(1),
      LO => mod5_hdframe_N217
    );
  mod5_hdframe_norlut10_INV_0 : INV
    port map (
      I => mod5_hdframe_samplecount(0),
      O => mod5_hdframe_N165
    );
  mod5_hdframe_norcy_rn_9 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N165,
      O => mod5_hdframe_nor_cyo10
    );
  mod5_hdframe_Andlut1 : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_samplecount(1),
      I1 => mod5_hdframe_samplecount(2),
      LO => mod5_hdframe_N166
    );
  mod5_hdframe_Andcy_rn_0 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo10,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N166,
      O => mod5_hdframe_And_cyo1
    );
  mod5_hdframe_norlut11 : LUT4_L
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod5_hdframe_samplecount(3),
      I1 => mod5_hdframe_samplecount(4),
      I2 => mod5_hdframe_samplecount(5),
      I3 => mod5_hdframe_samplecount(6),
      LO => mod5_hdframe_N167
    );
  mod5_hdframe_norcy_rn_10 : MUXCY
    port map (
      CI => mod5_hdframe_And_cyo1,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N167,
      O => mod5_hdframe_nor_cyo11
    );
  mod5_hdframe_Neq_stagelut5 : LUT4
    generic map(
      INIT => X"5666"
    )
    port map (
      I0 => mod5_hdframe_linecount(10),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(2),
      I3 => mod0_hd_format_tmp(1),
      O => mod5_hdframe_N65
    );
  mod5_hdframe_XNor_stagecy_rn_47 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo11,
      DI => mod5_asl(7),
      S => mod5_hdframe_N168,
      O => mod5_hdframe_XNor_stage_cyo46
    );
  mod5_hdframe_n0070_0_113_SW0 : LUT4_L
    generic map(
      INIT => X"070F"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod7_color_yramp(0),
      I2 => mod7_color_n0036,
      I3 => mod7_color_N57,
      LO => N8448
    );
  mod5_hdframe_XNor_stagecy_rn_48 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo46,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_samplecount_8_rt3,
      O => mod5_hdframe_XNor_stage_cyo47
    );
  mod5_hdframe_XNor_stagelut44 : LUT4
    generic map(
      INIT => X"5666"
    )
    port map (
      I0 => mod5_hdframe_samplecount(7),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(2),
      I3 => mod0_hd_format_tmp(1),
      O => mod5_hdframe_N154
    );
  mod5_hdframe_XNor_stagecy_rn_49 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo47,
      DI => mod5_asl(7),
      S => mod5_hdframe_N170,
      O => mod5_hdframe_XNor_stage_cyo48
    );
  mod5_hdframe_n0070_5_142_SW0 : LUT4_L
    generic map(
      INIT => X"FFA8"
    )
    port map (
      I0 => mod7_color_v1h9_y(5),
      I1 => CHOICE15417,
      I2 => CHOICE15420,
      I3 => CHOICE15402,
      LO => N8085
    );
  mod5_hdframe_XNor_stagecy_rn_50 : MUXCY
    port map (
      CI => mod5_hdframe_XNor_stage_cyo48,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_samplecount_10_rt3,
      O => mod5_hdframe_XNor_stage_cyo49
    );
  mod5_hdframe_norlut12_INV_0 : INV
    port map (
      I => mod5_hdframe_samplecount(11),
      O => mod5_hdframe_N172
    );
  mod5_hdframe_norlut13 : LUT4_L
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod5_hdframe_samplecount(1),
      I1 => mod5_hdframe_samplecount(2),
      I2 => mod5_hdframe_samplecount(3),
      I3 => mod5_hdframe_samplecount(4),
      LO => mod5_hdframe_N173
    );
  mod5_hdframe_norcy_rn_12 : MUXCY
    port map (
      CI => mod5_field1_vblnk1_4_Q,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N173,
      O => mod5_hdframe_nor_cyo12
    );
  mod5_hdframe_norlut14 : LUT3_L
    generic map(
      INIT => X"01"
    )
    port map (
      I0 => mod5_hdframe_samplecount(5),
      I1 => mod5_hdframe_samplecount(6),
      I2 => mod5_hdframe_samplecount(11),
      LO => mod5_hdframe_N174
    );
  mod5_hdframe_norcy_rn_13 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo12,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N174,
      O => mod5_hdframe_nor_cyo13
    );
  mod5_hdframe_Andcy_rn_1 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo13,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_samplecount_0_rt,
      O => mod5_hdframe_And_cyo2
    );
  mod5_hdframe_Eq_stagelut39 : LUT4_L
    generic map(
      INIT => X"60A0"
    )
    port map (
      I0 => mod5_hdframe_samplecount(7),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(8),
      I3 => sif_hd_format(1),
      LO => mod5_hdframe_N202
    );
  mod5_hdframe_Neq_stagecy_rn_5 : MUXCY
    port map (
      CI => mod5_hdframe_And_cyo2,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N175,
      O => mod5_hdframe_Neq_stage_cyo5
    );
  mod5_hdframe_Eq_stagelut40 : LUT4_L
    generic map(
      INIT => X"60A0"
    )
    port map (
      I0 => mod5_hdframe_samplecount(9),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(10),
      I3 => sif_hd_format(1),
      LO => mod5_hdframe_N203
    );
  mod5_hdframe_norlut15 : LUT4_L
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod5_hdframe_samplecount_0_1,
      I1 => mod5_hdframe_samplecount(2),
      I2 => mod5_hdframe_samplecount(3),
      I3 => mod5_hdframe_samplecount(4),
      LO => mod5_hdframe_N177
    );
  mod5_hdframe_norcy_rn_14 : MUXCY
    port map (
      CI => mod5_field1_vblnk1_4_Q,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N177,
      O => mod5_hdframe_nor_cyo14
    );
  mod5_hdframe_norlut16 : LUT3_L
    generic map(
      INIT => X"01"
    )
    port map (
      I0 => mod5_hdframe_samplecount(5),
      I1 => mod5_hdframe_samplecount(6),
      I2 => mod5_hdframe_samplecount(11),
      LO => mod5_hdframe_N178
    );
  mod5_hdframe_norcy_rn_15 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo14,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N178,
      O => mod5_hdframe_nor_cyo15
    );
  mod5_hdframe_Andcy_rn_2 : MUXCY
    port map (
      CI => mod5_hdframe_nor_cyo15,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_samplecount_1_rt1,
      O => mod5_hdframe_And_cyo3
    );
  mod5_hdframe_Neq_stagelut6 : LUT4_L
    generic map(
      INIT => X"60A0"
    )
    port map (
      I0 => mod5_hdframe_samplecount(7),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(8),
      I3 => sif_hd_format(1),
      LO => mod5_hdframe_N175
    );
  mod5_hdframe_Neq_stagecy_rn_7 : MUXCY
    port map (
      CI => mod5_hdframe_And_cyo3,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N179,
      O => mod5_hdframe_Neq_stage_cyo6
    );
  mod5_hdframe_Neq_stagelut7 : LUT4_L
    generic map(
      INIT => X"60A0"
    )
    port map (
      I0 => mod5_hdframe_samplecount(9),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(10),
      I3 => sif_hd_format(1),
      LO => mod5_hdframe_N176
    );
  mod5_hdframe_n0075_5_64 : LUT4_L
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => mod7_color_N123,
      I1 => mod7_color_N140,
      I2 => mod7_color_n0014,
      I3 => mod7_color_n0012,
      LO => CHOICE15215
    );
  mod5_hdframe_Neq_stagecy_rn_9 : MUXCY
    port map (
      CI => mod5_field1_vblnk1_4_Q,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N181,
      O => mod5_hdframe_Neq_stage_cyo7
    );
  mod7_Ker2252 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => mod7_v_state(2),
      I1 => mod7_tp_19_Q,
      I2 => mod7_n0024,
      O => mod7_N225
    );
  mod5_hdframe_Neq_stagecy_rn_10 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo7,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N182,
      O => mod5_hdframe_Neq_stage_cyo8
    );
  mod5_hdframe_Neq_stagelut12 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(4),
      I1 => mod5_hdframe_hd_framegenerator_n0086_3_cyo,
      I2 => mod5_hdframe_samplecount(5),
      I3 => mod5_hdframe_n0086_5_Q,
      LO => mod5_hdframe_N183
    );
  mod5_hdframe_Neq_stagecy_rn_11 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo8,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N183,
      O => mod5_hdframe_Neq_stage_cyo9
    );
  mod5_hdframe_Neq_stagelut13 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(6),
      I1 => mod5_hdframe_n0086_6_Q,
      I2 => mod5_hdframe_samplecount(7),
      I3 => mod5_hdframe_n0086_7_Q,
      LO => mod5_hdframe_N184
    );
  mod5_hdframe_Neq_stagecy_rn_12 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo9,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N184,
      O => mod5_hdframe_Neq_stage_cyo10
    );
  mod5_hdframe_Neq_stagelut14 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(8),
      I1 => mod5_field1_vblnk1_4_Q,
      I2 => mod5_hdframe_samplecount(9),
      I3 => mod5_hdframe_n0086_9_Q,
      LO => mod5_hdframe_N185
    );
  mod5_hdframe_Neq_stagecy_rn_13 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo10,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N185,
      O => mod5_hdframe_Neq_stage_cyo11
    );
  mod5_hdframe_Neq_stagelut15 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(10),
      I1 => mod5_hdframe_n0086_10_Q,
      I2 => mod5_hdframe_samplecount(11),
      I3 => mod5_hdframe_n0086_11_Q,
      LO => mod5_hdframe_N186
    );
  mod5_hdframe_n0070_5_208_SW0_G : LUT4_L
    generic map(
      INIT => X"FF69"
    )
    port map (
      I0 => mod5_hdframe_luma_crc(15),
      I1 => mod5_hdframe_n0177(0),
      I2 => mod5_hdframe_n0175(0),
      I3 => mod5_hdframe_n0115,
      LO => N8535
    );
  mod5_hdframe_Neq_stagecy_rn_15 : MUXCY
    port map (
      CI => mod5_field1_vblnk1_4_Q,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N187,
      O => mod5_hdframe_Neq_stage_cyo12
    );
  mod5_hdframe_Neq_stagelut17 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(2),
      I1 => mod5_hdframe_n0087_2_Q,
      I2 => mod5_hdframe_samplecount(3),
      I3 => mod5_hdframe_n0087_3_Q,
      LO => mod5_hdframe_N188
    );
  mod5_hdframe_Neq_stagecy_rn_16 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo12,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N188,
      O => mod5_hdframe_Neq_stage_cyo13
    );
  mod5_hdframe_Neq_stagelut18 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(4),
      I1 => mod5_hdframe_hd_framegenerator_n0087_3_cyo,
      I2 => mod5_hdframe_samplecount(5),
      I3 => mod5_hdframe_n0087_5_Q,
      LO => mod5_hdframe_N189
    );
  mod5_hdframe_Neq_stagecy_rn_17 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo13,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N189,
      O => mod5_hdframe_Neq_stage_cyo14
    );
  mod5_hdframe_Neq_stagelut19 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(6),
      I1 => mod5_hdframe_n0087_6_Q,
      I2 => mod5_hdframe_samplecount(7),
      I3 => mod5_hdframe_n0087_7_Q,
      LO => mod5_hdframe_N190
    );
  mod5_hdframe_Neq_stagecy_rn_18 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo14,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N190,
      O => mod5_hdframe_Neq_stage_cyo15
    );
  mod5_hdframe_Neq_stagelut20 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(8),
      I1 => mod5_field1_vblnk1_4_Q,
      I2 => mod5_hdframe_samplecount(9),
      I3 => mod5_hdframe_n0087_9_Q,
      LO => mod5_hdframe_N191
    );
  mod5_hdframe_Neq_stagecy_rn_19 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo15,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N191,
      O => mod5_hdframe_Neq_stage_cyo16
    );
  mod5_hdframe_Neq_stagelut21 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(10),
      I1 => mod5_hdframe_n0087_10_Q,
      I2 => mod5_hdframe_samplecount(11),
      I3 => mod5_hdframe_n0087_11_Q,
      LO => mod5_hdframe_N192
    );
  mod5_hdframe_Neq_stagelut10 : LUT4_L
    generic map(
      INIT => X"4B00"
    )
    port map (
      I0 => sif_hd_format(0),
      I1 => sif_hd_format(2),
      I2 => mod5_hdframe_samplecount(1),
      I3 => mod5_hdframe_samplecount_0_1,
      LO => mod5_hdframe_N181
    );
  mod5_hdframe_Neq_stagecy_rn_21 : MUXCY
    port map (
      CI => mod5_field1_vblnk1_4_Q,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N193,
      O => mod5_hdframe_Neq_stage_cyo17
    );
  mod5_hdframe_Neq_stagelut23 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(2),
      I1 => mod5_hdframe_n0088_2_Q,
      I2 => mod5_hdframe_samplecount(3),
      I3 => mod5_hdframe_n0088_3_Q,
      LO => mod5_hdframe_N194
    );
  mod5_hdframe_Neq_stagecy_rn_22 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo17,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N194,
      O => mod5_hdframe_Neq_stage_cyo18
    );
  mod5_hdframe_Neq_stagelut24 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(4),
      I1 => mod5_hdframe_hd_framegenerator_n0088_3_cyo,
      I2 => mod5_hdframe_samplecount(5),
      I3 => mod5_hdframe_n0088_5_Q,
      LO => mod5_hdframe_N195
    );
  mod5_hdframe_Neq_stagecy_rn_23 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo18,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N195,
      O => mod5_hdframe_Neq_stage_cyo19
    );
  mod5_hdframe_Neq_stagelut25 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(6),
      I1 => mod5_hdframe_n0088_6_Q,
      I2 => mod5_hdframe_samplecount(7),
      I3 => mod5_hdframe_n0088_7_Q,
      LO => mod5_hdframe_N196
    );
  mod5_hdframe_Neq_stagecy_rn_24 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo19,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N196,
      O => mod5_hdframe_Neq_stage_cyo20
    );
  mod5_hdframe_Neq_stagelut26 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(8),
      I1 => mod5_field1_vblnk1_4_Q,
      I2 => mod5_hdframe_samplecount(9),
      I3 => mod5_hdframe_n0088_9_Q,
      LO => mod5_hdframe_N197
    );
  mod5_hdframe_Neq_stagecy_rn_25 : MUXCY
    port map (
      CI => mod5_hdframe_Neq_stage_cyo20,
      DI => mod5_tsl_4_Q,
      S => mod5_hdframe_N197,
      O => mod5_hdframe_Neq_stage_cyo21
    );
  mod5_hdframe_Neq_stagelut27 : LUT4_L
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => mod5_hdframe_samplecount(10),
      I1 => mod5_hdframe_n0088_10_Q,
      I2 => mod5_hdframe_samplecount(11),
      I3 => mod5_hdframe_n0088_11_Q,
      LO => mod5_hdframe_N198
    );
  mod5_hdframe_norlut17 : LUT4_L
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod5_hdframe_samplecount(2),
      I1 => mod5_hdframe_samplecount(3),
      I2 => mod5_hdframe_samplecount(4),
      I3 => mod5_hdframe_samplecount(5),
      LO => mod5_hdframe_N199
    );
  mod5_hdframe_norcy_rn_16 : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod5_hdframe_N199,
      O => mod5_hdframe_nor_cyo16
    );
  mod7_counter_n0002_5_1 : LUT3_L
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_counter_N01,
      I1 => mod7_counter_n0009(16),
      I2 => mod7_load_val(5),
      LO => mod7_counter_n0002(5)
    );
  mod7_n0046_3_367_SW0 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(8),
      I2 => mod7_N273,
      I3 => mod7_N254,
      O => N8115
    );
  mod7_counter_count_0_n00011 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(0),
      O => mod7_counter_count_0_n0001
    );
  mod7_counter_state_counter_n0009_21_xor : XORCY
    port map (
      CI => mod7_counter_state_counter_n0009_20_cyo,
      LI => mod7_counter_N12,
      O => mod7_counter_n0009(21)
    );
  mod7_counter_load_35 : FDC_1
    port map (
      D => mod5_hdframe_sav,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_counter_load
    );
  mod7_counter_count_1_n00001 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(1),
      O => mod7_counter_count_1_n0000
    );
  mod7_counter_count_10 : FDCP
    port map (
      D => mod7_counter_n0002(10),
      CLR => mod7_counter_load,
      PRE => mod5_field1_vblnk1_4_Q,
      C => txclka2,
      Q => mod7_counter_count(10)
    );
  mod7_counter_count_9 : FDCP
    port map (
      D => mod7_counter_n0002(9),
      CLR => mod7_counter_load,
      PRE => mod5_field1_vblnk1_4_Q,
      C => txclka2,
      Q => mod7_counter_count(9)
    );
  mod7_counter_count_0 : FDCP
    port map (
      D => mod7_counter_n0002(0),
      CLR => mod7_counter_count_0_n0000,
      PRE => mod7_counter_count_0_n0001,
      C => txclka2,
      Q => mod7_counter_count(0)
    );
  mod7_counter_count_1 : FDCP
    port map (
      D => mod7_counter_n0002(1),
      CLR => mod7_counter_count_1_n0000,
      PRE => mod7_counter_count_1_n0001,
      C => txclka2,
      Q => mod7_counter_count(1)
    );
  mod7_counter_count_2 : FDCP
    port map (
      D => mod7_counter_n0002(2),
      CLR => mod7_counter_count_2_n0000,
      PRE => mod7_counter_count_2_n0001,
      C => txclka2,
      Q => mod7_counter_count(2)
    );
  mod7_counter_count_3 : FDCP
    port map (
      D => mod7_counter_n0002(3),
      CLR => mod7_counter_count_3_n0000,
      PRE => mod7_counter_count_3_n0001,
      C => txclka2,
      Q => mod7_counter_count(3)
    );
  mod7_counter_count_4 : FDCP
    port map (
      D => mod7_counter_n0002(4),
      CLR => mod7_counter_count_4_n0000,
      PRE => mod7_counter_count_4_n0001,
      C => txclka2,
      Q => mod7_counter_count(4)
    );
  mod7_counter_count_5 : FDCP
    port map (
      D => mod7_counter_n0002(5),
      CLR => mod7_counter_count_5_n0000,
      PRE => mod7_counter_count_5_n0001,
      C => txclka2,
      Q => mod7_counter_count(5)
    );
  mod7_counter_count_6 : FDCP
    port map (
      D => mod7_counter_n0002(6),
      CLR => mod7_counter_count_6_n0000,
      PRE => mod7_counter_count_6_n0001,
      C => txclka2,
      Q => mod7_counter_count(6)
    );
  mod7_counter_count_7 : FDCP
    port map (
      D => mod7_counter_n0002(7),
      CLR => mod7_counter_count_7_n0000,
      PRE => mod7_counter_count_7_n0001,
      C => txclka2,
      Q => mod7_counter_count(7)
    );
  mod7_counter_count_8 : FDCP
    port map (
      D => mod7_counter_n0002(8),
      CLR => mod7_counter_count_8_n0000,
      PRE => mod7_counter_count_8_n0001,
      C => txclka2,
      Q => mod7_counter_count(8)
    );
  mod7_counter_count_1_n00011 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(1),
      O => mod7_counter_count_1_n0001
    );
  mod7_counter_count_2_n00001 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(2),
      O => mod7_counter_count_2_n0000
    );
  mod7_counter_count_2_n00011 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(2),
      O => mod7_counter_count_2_n0001
    );
  mod7_counter_count_3_n00001 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(3),
      O => mod7_counter_count_3_n0000
    );
  mod7_counter_count_3_n00011 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(3),
      O => mod7_counter_count_3_n0001
    );
  mod7_counter_count_4_n00001 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(4),
      O => mod7_counter_count_4_n0000
    );
  mod7_counter_count_4_n00011 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(4),
      O => mod7_counter_count_4_n0001
    );
  mod7_counter_count_5_n00001 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(5),
      O => mod7_counter_count_5_n0000
    );
  mod7_counter_count_5_n00011 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(5),
      O => mod7_counter_count_5_n0001
    );
  mod7_counter_count_6_n00001 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(6),
      O => mod7_counter_count_6_n0000
    );
  mod7_counter_count_6_n00011 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(6),
      O => mod7_counter_count_6_n0001
    );
  mod7_counter_count_7_n00001 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(7),
      O => mod7_counter_count_7_n0000
    );
  mod7_counter_count_7_n00011 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(7),
      O => mod7_counter_count_7_n0001
    );
  mod7_counter_count_8_n00001 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(8),
      O => mod7_counter_count_8_n0000
    );
  mod7_counter_count_8_n00011 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(8),
      O => mod7_counter_count_8_n0001
    );
  mod7_counter_count_0_n00001 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_counter_load,
      I1 => mod7_load_val(0),
      O => mod7_counter_count_0_n0000
    );
  mod5_hdframe_n0070_6_469_SW0 : LUT4_L
    generic map(
      INIT => X"AB00"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => CHOICE14944,
      I2 => CHOICE14942,
      I3 => N8498,
      LO => N8078
    );
  mod7_counter_n0002_3_1 : LUT3_L
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_counter_N01,
      I1 => mod7_counter_n0009(14),
      I2 => mod7_load_val(3),
      LO => mod7_counter_n0002(3)
    );
  mod7_counter_n0002_2_1 : LUT3_L
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_counter_N01,
      I1 => mod7_counter_n0009(13),
      I2 => mod7_load_val(2),
      LO => mod7_counter_n0002(2)
    );
  mod7_counter_n0002_1_1 : LUT3_L
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_counter_N01,
      I1 => mod7_counter_n0009(12),
      I2 => mod7_load_val(1),
      LO => mod7_counter_n0002(1)
    );
  mod7_counter_n0002_9_1 : LUT4
    generic map(
      INIT => X"020A"
    )
    port map (
      I0 => mod7_counter_n0009(20),
      I1 => CHOICE14039,
      I2 => mod7_counter_load,
      I3 => CHOICE14026,
      O => mod7_counter_n0002(9)
    );
  mod7_counter_n0002_7_1 : LUT3_L
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_counter_N01,
      I1 => mod7_counter_n0009(18),
      I2 => mod7_load_val(7),
      LO => mod7_counter_n0002(7)
    );
  mod7_counter_n0002_0_1 : LUT3_L
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => mod7_counter_N01,
      I1 => mod7_counter_n0009(11),
      I2 => mod7_load_val(0),
      LO => mod7_counter_n0002(0)
    );
  mod7_counter_state_counter_n0009_11_cy : MUXCY
    port map (
      CI => mod5_tsl_4_Q,
      DI => mod5_field1_vblnk1_4_Q,
      S => mod7_counter_count_0_rt,
      O => mod7_counter_state_counter_n0009_11_cyo
    );
  mod7_counter_state_counter_n0009_11_xor : XORCY
    port map (
      CI => mod5_tsl_4_Q,
      LI => mod7_counter_count_0_rt,
      O => mod7_counter_n0009(11)
    );
  mod7_counter_state_counter_n0009_12_lut_INV_0 : INV
    port map (
      I => mod7_counter_count(1),
      O => mod7_counter_N3
    );
  mod7_counter_state_counter_n0009_12_cy : MUXCY
    port map (
      CI => mod7_counter_state_counter_n0009_11_cyo,
      DI => mod5_tsl_4_Q,
      S => mod7_counter_N3,
      O => mod7_counter_state_counter_n0009_12_cyo
    );
  mod7_counter_state_counter_n0009_12_xor : XORCY
    port map (
      CI => mod7_counter_state_counter_n0009_11_cyo,
      LI => mod7_counter_N3,
      O => mod7_counter_n0009(12)
    );
  mod7_counter_state_counter_n0009_13_lut_INV_0 : INV
    port map (
      I => mod7_counter_count(2),
      O => mod7_counter_N4
    );
  mod7_counter_state_counter_n0009_13_cy : MUXCY
    port map (
      CI => mod7_counter_state_counter_n0009_12_cyo,
      DI => mod5_tsl_4_Q,
      S => mod7_counter_N4,
      O => mod7_counter_state_counter_n0009_13_cyo
    );
  mod7_counter_state_counter_n0009_13_xor : XORCY
    port map (
      CI => mod7_counter_state_counter_n0009_12_cyo,
      LI => mod7_counter_N4,
      O => mod7_counter_n0009(13)
    );
  mod7_counter_state_counter_n0009_14_lut_INV_0 : INV
    port map (
      I => mod7_counter_count(3),
      O => mod7_counter_N5
    );
  mod7_color_Ker8050_SW0 : LUT2
    generic map(
      INIT => X"D"
    )
    port map (
      I0 => mod7_v_state(0),
      I1 => mod7_v_state(1),
      O => N8458
    );
  mod7_Ker163_SW1 : LUT3
    generic map(
      INIT => X"97"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(1),
      I2 => mod0_pattern(0),
      O => N8277
    );
  mod7_color_v1h2_cb_8_SW0 : LUT2
    generic map(
      INIT => X"D"
    )
    port map (
      I0 => mod0_tp_option(1),
      I1 => mod0_tp_option(0),
      O => N117
    );
  mod7_color_v1h2_cb_6_SW0 : LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      I0 => mod0_tp_option(1),
      I1 => mod0_tp_option(0),
      O => N119
    );
  mod5_hdframe_linecount_8_rt_36 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_linecount(8),
      O => mod5_hdframe_linecount_8_rt
    );
  mod5_hdframe_linecount_9_rt_37 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_linecount(9),
      O => mod5_hdframe_linecount_9_rt
    );
  mod7_Ker105_SW0 : LUT4
    generic map(
      INIT => X"FEBA"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(9),
      I2 => mod7_N124,
      I3 => mod7_N185,
      O => N124
    );
  mod7_Ker105_SW1 : LUT4
    generic map(
      INIT => X"FF5D"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_N124,
      I2 => mod7_active_line(9),
      I3 => mod7_active_line(6),
      O => N125
    );
  mod7_color_Ker106_SW1 : LUT4
    generic map(
      INIT => X"F7FF"
    )
    port map (
      I0 => mod0_tp_option(0),
      I1 => mod7_color_v1h9_y(5),
      I2 => mod0_tp_option(1),
      I3 => N8627,
      O => N8241
    );
  mod0_n0046_SW0 : LUT3_L
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => mod0_bitptr(2),
      I1 => mod0_bitptr(1),
      I2 => mod0_bitptr(0),
      LO => N1207
    );
  mod5_hdframe_samplecount_0_rt_38 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      LO => mod5_hdframe_samplecount_0_rt
    );
  mod5_hdframe_Ker56_SW2 : LUT4_L
    generic map(
      INIT => X"AAC0"
    )
    port map (
      I0 => mod5_hdframe_v,
      I1 => mod5_hdframe_linecount(5),
      I2 => mod5_hdframe_n0114,
      I3 => mod5_hdframe_n0133,
      LO => N8141
    );
  mod5_hdframe_samplecount_1_rt1_39 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(1),
      LO => mod5_hdframe_samplecount_1_rt1
    );
  mod5_hdframe_Ker55_SW2 : LUT4_L
    generic map(
      INIT => X"CCA0"
    )
    port map (
      I0 => mod5_hdframe_linecount(4),
      I1 => mod5_hdframe_h,
      I2 => mod5_hdframe_n0114,
      I3 => mod5_hdframe_n0133,
      LO => N8139
    );
  mod7_counter_count_0_rt_40 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_counter_count(0),
      O => mod7_counter_count_0_rt
    );
  mod5_hdframe_Ker54_SW2 : LUT4_L
    generic map(
      INIT => X"33A0"
    )
    port map (
      I0 => mod5_hdframe_linecount(3),
      I1 => mod5_hdframe_MUX_BLOCK_xyz(5),
      I2 => mod5_hdframe_n0114,
      I3 => mod5_hdframe_n0133,
      LO => N8137
    );
  mod7_active_line_10_rt_41 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(10),
      O => mod7_active_line_10_rt
    );
  mod5_hdframe_Ker57_SW2 : LUT4_L
    generic map(
      INIT => X"AAC0"
    )
    port map (
      I0 => mod5_hdframe_f,
      I1 => mod5_hdframe_linecount(6),
      I2 => mod5_hdframe_n0114,
      I3 => mod5_hdframe_n0133,
      LO => N8135
    );
  mod5_hdframe_samplecount_0_n00001 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => sif_rst,
      I1 => mod5_hdframe_samplecount(0),
      I2 => mod5_hdframe_n0107,
      O => mod5_hdframe_samplecount_0_0_n0000
    );
  mod6_scram20_inst_Mxor_n0021_Result1 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_n0168(0),
      I1 => mod6_scram20_inst_S(18),
      I2 => mod6_scram20_inst_S(17),
      I3 => mod6_scram20_inst_S(16),
      O => mod6_scram20_inst_n0164(0)
    );
  mod7_Ker1067 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(7),
      O => CHOICE12361
    );
  mod5_hdframe_n0074_12_SW0 : LUT4
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod5_hdframe_luma_crc(9),
      I1 => mod5_hdframe_luma_out(9),
      I2 => mod5_hdframe_luma_out(8),
      I3 => mod5_hdframe_luma_crc(8),
      O => N8456
    );
  mod7_h_state_FFd2_In_SW1 : LUT4
    generic map(
      INIT => X"E555"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => mod7_h_state_FFd1,
      I2 => mod7_h_state_FFd4,
      I3 => mod7_h_state_FFd3,
      O => N8235
    );
  mod5_hdframe_n0070_6_400_SW0 : LUT4
    generic map(
      INIT => X"11F1"
    )
    port map (
      I0 => mod7_v_state(2),
      I1 => mod7_v_state(0),
      I2 => mod7_h_state_FFd2_2,
      I3 => mod0_pattern(1),
      O => N8257
    );
  mod5_hdframe_n0070_6_104_SW0 : LUT4
    generic map(
      INIT => X"B999"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(0),
      I2 => mod7_tp_0_Q,
      I3 => mod7_color_yramp(6),
      O => N8339
    );
  mod5_hdframe_n0075_3_26 : LUT3
    generic map(
      INIT => X"A8"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => CHOICE15444,
      I2 => mod7_color_N41,
      O => CHOICE15446
    );
  mod7_active_line_6_rt_42 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(6),
      O => mod7_active_line_6_rt
    );
  mod5_Mrom_n0003_inst_lut4_3911 : LUT4
    generic map(
      INIT => X"EEAE"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(2),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(1),
      O => N7971
    );
  mod5_hdframe_XNor_stagelut36 : LUT4
    generic map(
      INIT => X"AA95"
    )
    port map (
      I0 => mod5_hdframe_linecount(3),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(2),
      I3 => mod0_hd_format_tmp(3),
      O => mod5_hdframe_N113
    );
  mod7_Ker22313 : LUT4
    generic map(
      INIT => X"EEAE"
    )
    port map (
      I0 => N8193,
      I1 => mod7_active_line(0),
      I2 => mod7_active_line(6),
      I3 => mod7_active_line(7),
      O => CHOICE12759
    );
  mod7_Ker229135_SW0 : LUT4
    generic map(
      INIT => X"FFD5"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(1),
      I2 => N8472,
      I3 => mod7_active_line(0),
      O => N8363
    );
  mod7_Ker300_SW0 : LUT3
    generic map(
      INIT => X"DF"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(7),
      I2 => mod7_active_line(4),
      O => N113
    );
  mod5_hdframe_XNor_stagelut43 : LUT4
    generic map(
      INIT => X"5666"
    )
    port map (
      I0 => mod5_hdframe_linecount(10),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(2),
      I3 => mod0_hd_format_tmp(1),
      O => mod5_hdframe_N120
    );
  mod7_Ker14377 : LUT3
    generic map(
      INIT => X"8A"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(7),
      I2 => mod7_active_line(8),
      O => CHOICE12464
    );
  mod7_Ker4974 : LUT4
    generic map(
      INIT => X"1101"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(1),
      I2 => mod7_N75,
      I3 => mod7_active_line(3),
      O => CHOICE12490
    );
  mod7_color_yramp_6_rt_43 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_color_yramp(6),
      O => mod7_color_yramp_6_rt
    );
  mod7_Ker23417 : LUT4
    generic map(
      INIT => X"2A2F"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(3),
      I2 => mod7_active_line(2),
      I3 => mod7_active_line(4),
      O => CHOICE12425
    );
  mod7_Ker8420_G : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_active_line(7),
      O => N8537
    );
  mod7_Ker229214 : LUT2
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_N249,
      I1 => mod7_N84,
      O => CHOICE12623
    );
  mod7_Ker22283 : LUT4
    generic map(
      INIT => X"FEEE"
    )
    port map (
      I0 => mod7_N240,
      I1 => N8195,
      I2 => mod7_active_line(2),
      I3 => mod7_N233,
      O => CHOICE12757
    );
  mod7_Ker14394_SW0 : LUT3
    generic map(
      INIT => X"A8"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => CHOICE12459,
      I2 => CHOICE12452,
      O => N8197
    );
  mod7_Ker22178 : LUT4
    generic map(
      INIT => X"EFFE"
    )
    port map (
      I0 => mod7_N260,
      I1 => mod7_N49,
      I2 => mod7_active_line(6),
      I3 => mod7_active_line(4),
      O => CHOICE12727
    );
  mod7_color_Ker581_SW0 : LUT4
    generic map(
      INIT => X"2820"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_pattern(2),
      I2 => mod0_pattern(0),
      I3 => mod7_color_yramp(2),
      O => N8019
    );
  mod7_Ker10651_SW1 : LUT4
    generic map(
      INIT => X"FE7E"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(4),
      I2 => mod7_active_line(9),
      I3 => mod7_active_line(8),
      O => N8343
    );
  mod5_hdframe_XNor_stagelut35 : LUT4
    generic map(
      INIT => X"3336"
    )
    port map (
      I0 => mod0_hd_format_tmp(1),
      I1 => mod5_hdframe_linecount(2),
      I2 => mod0_hd_format_tmp(3),
      I3 => mod0_hd_format_tmp(2),
      O => mod5_hdframe_N112
    );
  mod7_color_yramp_8_rt_44 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_color_yramp(8),
      O => mod7_color_yramp_8_rt
    );
  mod7_color_yramp_7_rt_45 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_color_yramp(7),
      O => mod7_color_yramp_7_rt
    );
  mod7_Ker22916 : LUT4
    generic map(
      INIT => X"FFD5"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(6),
      O => CHOICE12563
    );
  mod5_hdframe_n0075_3_29 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => mod7_color_N120,
      I1 => mod7_color_N134,
      I2 => mod0_pattern(1),
      I3 => CHOICE15007,
      O => CHOICE15447
    );
  mod7_Ker22283_SW0_SW2 : LUT4
    generic map(
      INIT => X"FF27"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(2),
      I2 => mod7_N75,
      I3 => mod7_N242,
      O => N8478
    );
  mod7_Ker22134_SW0 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE12711,
      I1 => CHOICE12700,
      I2 => CHOICE12692,
      I3 => CHOICE12674,
      O => N8203
    );
  mod7_Ker10651_SW0 : LUT3
    generic map(
      INIT => X"76"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(8),
      I2 => mod7_active_line(6),
      O => N8474
    );
  mod5_hdframe_n0075_9_105 : LUT4
    generic map(
      INIT => X"5551"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => N8089,
      I2 => CHOICE14815,
      I3 => CHOICE14819,
      O => CHOICE14827
    );
  mod5_hdframe_n0075_2_59 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE15245,
      I1 => mod5_hdframe_N881,
      I2 => CHOICE15248,
      I3 => CHOICE15256,
      O => CHOICE15259
    );
  mod7_color_yramp_5_rt_46 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_color_yramp(5),
      O => mod7_color_yramp_5_rt
    );
  mod7_Ker2271 : LUT3
    generic map(
      INIT => X"68"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(0),
      I2 => mod0_pattern(1),
      O => mod7_N227
    );
  mod7_Ker14394 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_N184,
      I2 => CHOICE12464,
      I3 => N8197,
      O => mod7_N143
    );
  mod5_hdframe_n0075_3_64 : LUT4_L
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE15442,
      I1 => mod5_hdframe_N881,
      I2 => CHOICE15446,
      I3 => CHOICE15451,
      LO => CHOICE15454
    );
  mod7_Ker23428 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(4),
      O => CHOICE12429
    );
  mod7_color_yramp_2_rt_47 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_color_yramp(2),
      O => mod7_color_yramp_2_rt
    );
  mod7_Ker249143_SW0 : LUT4
    generic map(
      INIT => X"8F88"
    )
    port map (
      I0 => mod7_N287,
      I1 => mod7_N252,
      I2 => mod7_active_line(8),
      I3 => mod7_active_line(4),
      O => N8269
    );
  mod7_Ker249165 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => CHOICE12534,
      I1 => CHOICE12552,
      O => CHOICE12553
    );
  mod7_Ker10651 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => N8343,
      I2 => N8474,
      O => mod7_N106
    );
  mod7_Ker229135 : LUT4
    generic map(
      INIT => X"40C0"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => N8363,
      I2 => CHOICE12599,
      I3 => N8281,
      O => CHOICE12601
    );
  mod7_Ker22283_SW0 : LUT4
    generic map(
      INIT => X"FE54"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => mod7_active_line(6),
      I2 => N8478,
      I3 => N8352,
      O => N8195
    );
  mod7_Ker22313_SW0 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE12757,
      I1 => CHOICE12743,
      I2 => CHOICE12728,
      I3 => CHOICE12717,
      O => N8193
    );
  mod7_Ker22954 : LUT4
    generic map(
      INIT => X"FD75"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(4),
      I3 => N8482,
      O => CHOICE12576
    );
  mod7_Ker24948 : LUT4
    generic map(
      INIT => X"AF8C"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_N270,
      I2 => mod7_active_line(0),
      I3 => mod7_active_line(1),
      O => CHOICE12519
    );
  mod5_hdframe_n0075_5_101 : LUT4
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => mod7_color_v0h2_y_0_Q,
      I2 => CHOICE15215,
      I3 => CHOICE15222,
      O => CHOICE15224
    );
  mod5_hdframe_n0070_0_16 : LUT4
    generic map(
      INIT => X"A2F2"
    )
    port map (
      I0 => mod7_color_N118,
      I1 => mod7_h_state_FFd3,
      I2 => mod7_h_state_FFd1,
      I3 => N8490,
      O => CHOICE14667
    );
  mod7_Ker4941 : LUT3
    generic map(
      INIT => X"8A"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(6),
      I2 => mod7_active_line(2),
      O => CHOICE12483
    );
  mod7_Ker24910 : LUT4
    generic map(
      INIT => X"AF23"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(0),
      I2 => mod7_N242,
      I3 => mod7_active_line(8),
      O => CHOICE12504
    );
  mod7_Ker23442 : LUT4
    generic map(
      INIT => X"5540"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(5),
      I2 => CHOICE12425,
      I3 => CHOICE12429,
      O => CHOICE12431
    );
  mod7_Ker249100 : LUT4
    generic map(
      INIT => X"AB01"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(7),
      I2 => N8439,
      I3 => N8440,
      O => CHOICE12534
    );
  mod7_Ker229188 : LUT3
    generic map(
      INIT => X"5D"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => mod7_active_line(6),
      I2 => mod7_active_line(7),
      O => CHOICE12614
    );
  mod7_Ker14327 : LUT4
    generic map(
      INIT => X"95D5"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(1),
      I2 => mod0_pattern(0),
      I3 => mod7_active_line(7),
      O => CHOICE12452
    );
  mod7_Ker229380_SW0 : LUT4
    generic map(
      INIT => X"FBFF"
    )
    port map (
      I0 => mod7_n0068,
      I1 => CHOICE12668,
      I2 => mod7_n0099,
      I3 => CHOICE12645,
      O => N8205
    );
  mod7_h_state_FFd2_2_48 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd2_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd2_2
    );
  mod7_Ker249100_SW1 : LUT4
    generic map(
      INIT => X"D5FF"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(3),
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(8),
      O => N8440
    );
  mod5_hdframe_f1 : LUT4_L
    generic map(
      INIT => X"F8FA"
    )
    port map (
      I0 => mod5_hdframe_f,
      I1 => N8072,
      I2 => mod5_hdframe_n0097,
      I3 => N8633,
      LO => N8048
    );
  mod7_Ker4988 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(8),
      O => CHOICE12496
    );
  mod5_hdframe_n0075_8_63 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => mod7_color_n0018,
      I1 => mod7_color_v1h2_cb_6_Q,
      I2 => mod7_color_n0023,
      I3 => mod7_tp_19_Q,
      O => CHOICE15362
    );
  mod7_Ker617 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_N230,
      I2 => mod7_active_line(1),
      I3 => mod7_active_line(2),
      O => CHOICE12409
    );
  mod5_hdframe_n0070_3_111 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE15138,
      I1 => mod5_hdframe_N881,
      I2 => mod7_color_N77,
      I3 => CHOICE15155,
      O => CHOICE15158
    );
  mod7_Ker4910 : LUT4
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(6),
      I2 => mod7_active_line(7),
      I3 => mod7_active_line(8),
      O => CHOICE12472
    );
  mod7_Ker22239 : LUT4
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => mod7_N84,
      I1 => CHOICE12742,
      I2 => mod7_tp_17_Q,
      I3 => CHOICE12733,
      O => CHOICE12743
    );
  mod7_Ker224 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(0),
      I2 => mod7_active_line(4),
      I3 => mod7_active_line(2),
      O => CHOICE12674
    );
  mod7_Ker22511 : LUT4
    generic map(
      INIT => X"028A"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(4),
      I3 => N8482,
      O => CHOICE12692
    );
  mod5_hdframe_n0070_3_18 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_luma_crc(12),
      O => CHOICE15138
    );
  mod7_color_yramp_1_rt_49 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_color_yramp(1),
      O => mod7_color_yramp_1_rt
    );
  mod5_hdframe_n0070_3_51 : LUT4_L
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_color_v0h5_cr(0),
      I1 => mod7_color_N120,
      I2 => mod7_color_N134,
      I3 => mod7_color_n0014,
      LO => CHOICE15148
    );
  mod7_Ker49109_SW0_SW0 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE12496,
      I1 => CHOICE12490,
      I2 => CHOICE12483,
      I3 => CHOICE12475,
      O => N8354
    );
  mod5_hdframe_n0075_9_457 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => mod5_hdframe_n0115,
      I1 => CHOICE14898,
      O => CHOICE14899
    );
  mod7_Ker23461 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(7),
      I3 => mod7_active_line(3),
      O => CHOICE12436
    );
  mod7_color_yramp_4_rt_50 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_color_yramp(4),
      O => mod7_color_yramp_4_rt
    );
  mod7_Ker22134 : LUT4
    generic map(
      INIT => X"F1F3"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_N270,
      I2 => N8203,
      I3 => mod7_active_line(2),
      O => CHOICE12713
    );
  mod7_active_line_8_rt_51 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(8),
      O => mod7_active_line_8_rt
    );
  mod7_Ker2331 : LUT3
    generic map(
      INIT => X"04"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(8),
      I2 => mod7_active_line(7),
      O => mod7_N233
    );
  mod5_hdframe_n0070_7_69_SW0 : LUT4
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod7_color_N01,
      I2 => mod7_color_yramp(7),
      I3 => mod7_color_n0018,
      O => N8261
    );
  mod5_hdframe_n0070_6_297 : LUT4
    generic map(
      INIT => X"3133"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod7_tp_4_Q,
      I2 => mod7_color_yramp(6),
      I3 => mod7_color_v1h4_y(9),
      O => CHOICE14979
    );
  mod7_Ker22152 : LUT4
    generic map(
      INIT => X"BA10"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(8),
      I3 => mod7_active_line(1),
      O => CHOICE12717
    );
  mod7_Ker22511_SW1 : LUT3
    generic map(
      INIT => X"A8"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(4),
      O => N8482
    );
  mod7_n0034_3_185 : LUT4
    generic map(
      INIT => X"88A8"
    )
    port map (
      I0 => mod7_N257,
      I1 => N8592,
      I2 => mod7_n0028,
      I3 => mod7_tp_17_Q,
      O => CHOICE14374
    );
  mod7_color_yramp_3_rt_52 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_color_yramp(3),
      O => mod7_color_yramp_3_rt
    );
  mod7_Ker3549_G : LUT4
    generic map(
      INIT => X"FBFF"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(7),
      I3 => mod7_active_line(8),
      O => N8539
    );
  mod7_Ker22113 : LUT4
    generic map(
      INIT => X"F5F4"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => mod7_N285,
      I2 => CHOICE12710,
      I3 => mod7_N282,
      O => CHOICE12711
    );
  mod7_Ker6123 : LUT3
    generic map(
      INIT => X"8A"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(4),
      I2 => mod7_active_line(5),
      O => CHOICE12414
    );
  mod7_Ker229283 : LUT4
    generic map(
      INIT => X"FEEE"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => N8466,
      I2 => mod7_active_line(1),
      I3 => mod7_active_line(5),
      O => CHOICE12645
    );
  mod7_Ker22283_SW0_SW1 : LUT4
    generic map(
      INIT => X"F8B8"
    )
    port map (
      I0 => mod7_N242,
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(6),
      O => N8352
    );
  mod7_Ker229397 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => CHOICE12608,
      I1 => CHOICE12614,
      I2 => CHOICE12624,
      I3 => CHOICE12669,
      O => CHOICE12670
    );
  mod7_Ker14340 : LUT4
    generic map(
      INIT => X"111F"
    )
    port map (
      I0 => mod0_pattern(0),
      I1 => mod0_pattern(1),
      I2 => mod7_active_line(7),
      I3 => mod7_active_line(3),
      O => CHOICE12459
    );
  mod7_Ker6127 : LUT4
    generic map(
      INIT => X"F5F4"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(4),
      I2 => CHOICE12414,
      I3 => CHOICE12409,
      O => CHOICE12415
    );
  mod5_hdframe_n0070_5_107 : LUT4
    generic map(
      INIT => X"2820"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_pattern(2),
      I2 => mod0_pattern(0),
      I3 => mod7_color_yramp(5),
      O => CHOICE15411
    );
  mod7_Ker229124 : LUT4
    generic map(
      INIT => X"FF5D"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(4),
      I2 => mod7_active_line(1),
      I3 => mod7_active_line(2),
      O => CHOICE12599
    );
  mod7_Ker6135 : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(6),
      I2 => CHOICE12415,
      O => mod7_N61
    );
  mod7_n0034_5_32 : LUT3_L
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => CHOICE14387,
      I1 => CHOICE14389,
      I2 => CHOICE14392,
      LO => CHOICE14394
    );
  mod5_hdframe_n0075_0_0 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_chroma_crc(10),
      O => CHOICE15024
    );
  mod5_hdframe_n0070_9_15 : LUT4_L
    generic map(
      INIT => X"EA40"
    )
    port map (
      I0 => mod7_color_N134,
      I1 => N8068,
      I2 => mod7_color_N140,
      I3 => N8067,
      LO => CHOICE15065
    );
  mod7_n0034_5_49 : LUT4
    generic map(
      INIT => X"A280"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_v_state(2),
      I2 => mod7_n0024,
      I3 => mod7_n0025,
      O => CHOICE14399
    );
  mod5_hdframe_n0070_5_110 : LUT4
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => CHOICE15411,
      I1 => mod7_color_N121,
      I2 => mod7_color_N132,
      I3 => mod7_color_n0041,
      O => CHOICE15412
    );
  mod7_Ker23499_SW0 : LUT4
    generic map(
      INIT => X"F11F"
    )
    port map (
      I0 => CHOICE12436,
      I1 => CHOICE12431,
      I2 => mod0_pattern(1),
      I3 => mod0_pattern(0),
      O => N8325
    );
  mod7_active_line_7_rt_53 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(7),
      O => mod7_active_line_7_rt
    );
  mod7_n0034_3_234 : LUT4_L
    generic map(
      INIT => X"FEEE"
    )
    port map (
      I0 => mod7_N25,
      I1 => CHOICE14378,
      I2 => mod7_load_val(3),
      I3 => CHOICE14349,
      LO => mod7_n0034(3)
    );
  mod7_Ker24954 : LUT4
    generic map(
      INIT => X"888C"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => CHOICE12519,
      I2 => mod7_active_line(6),
      I3 => mod7_active_line(5),
      O => CHOICE12520
    );
  mod7_Ker229380 : LUT4
    generic map(
      INIT => X"1101"
    )
    port map (
      I0 => mod7_n0122,
      I1 => N8205,
      I2 => mod7_N233,
      I3 => mod7_active_line(6),
      O => CHOICE12669
    );
  mod7_Ker49109_SW0 : LUT4
    generic map(
      INIT => X"FF7A"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_active_line(8),
      I2 => mod7_active_line(0),
      I3 => N8354,
      O => N8207
    );
  mod7_Ker22205 : LUT3
    generic map(
      INIT => X"BA"
    )
    port map (
      I0 => mod7_n0068,
      I1 => mod7_n0176,
      I2 => mod7_n0069,
      O => CHOICE12733
    );
  mod5_hdframe_n0070_0_84 : LUT4
    generic map(
      INIT => X"FF70"
    )
    port map (
      I0 => mod0_tp_option(0),
      I1 => mod0_pattern(1),
      I2 => mod7_color_n0018,
      I3 => CHOICE14685,
      O => CHOICE14686
    );
  mod5_hdframe_n0070_7_60 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_luma_crc(16),
      O => CHOICE14631
    );
  mod7_n0034_5_24 : LUT4
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_n0025,
      I2 => mod7_v_state(3),
      I3 => mod7_n0030,
      O => CHOICE14392
    );
  mod7_color_Ker61_SW0 : LUT4_L
    generic map(
      INIT => X"C8C0"
    )
    port map (
      I0 => mod0_tp_option(2),
      I1 => mod7_color_v1h9_y(5),
      I2 => mod7_color_n0025,
      I3 => mod7_color_n0023,
      LO => N1271
    );
  mod5_hdframe_n0070_7_59 : LUT4_D
    generic map(
      INIT => X"9600"
    )
    port map (
      I0 => mod5_hdframe_luma_crc(17),
      I1 => mod5_hdframe_n0173(0),
      I2 => mod5_hdframe_n0171(0),
      I3 => mod5_hdframe_n0116,
      LO => N8640,
      O => CHOICE14630
    );
  mod5_hdframe_n0070_6_104 : LUT4_L
    generic map(
      INIT => X"F3F1"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod7_tp_4_Q,
      I2 => CHOICE14940,
      I3 => N8339,
      LO => CHOICE14941
    );
  mod5_hdframe_n0075_9_51 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => mod7_color_N47,
      I1 => mod7_tp_5_Q,
      I2 => mod7_tp_4_Q,
      I3 => CHOICE14813,
      O => CHOICE14815
    );
  mod7_Ker22310 : LUT4_D
    generic map(
      INIT => X"4FCF"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_N51,
      I2 => mod7_n0086,
      I3 => mod7_v_state(3),
      LO => N8641,
      O => mod7_N223
    );
  mod7_Ker229176 : LUT4
    generic map(
      INIT => X"45EF"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(8),
      I3 => mod7_active_line(1),
      O => CHOICE12608
    );
  mod7_Ker229220 : LUT4
    generic map(
      INIT => X"44C4"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => CHOICE12623,
      I2 => mod7_active_line(3),
      I3 => mod7_active_line(2),
      O => CHOICE12624
    );
  mod7_Ker24919 : LUT4
    generic map(
      INIT => X"45EF"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(3),
      I2 => mod7_active_line(8),
      I3 => mod7_active_line(6),
      O => CHOICE12509
    );
  mod7_Ker49109 : LUT4
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => N8207,
      I1 => mod7_N298,
      I2 => mod7_active_line(3),
      I3 => CHOICE12472,
      O => mod7_N49
    );
  mod7_Ker229361 : LUT4
    generic map(
      INIT => X"0F01"
    )
    port map (
      I0 => mod7_N233,
      I1 => mod7_N242,
      I2 => N8209,
      I3 => mod7_active_line(0),
      O => CHOICE12668
    );
  mod7_Ker22922 : LUT4
    generic map(
      INIT => X"44C4"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => CHOICE12563,
      I2 => mod7_active_line(6),
      I3 => mod7_active_line(2),
      O => CHOICE12564
    );
  mod7_Ker22183 : LUT4
    generic map(
      INIT => X"EEAE"
    )
    port map (
      I0 => CHOICE12727,
      I1 => mod7_active_line(0),
      I2 => mod7_active_line(3),
      I3 => mod7_active_line(2),
      O => CHOICE12728
    );
  mod7_Ker249153 : LUT4
    generic map(
      INIT => X"2030"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => mod7_N298,
      I2 => CHOICE12551,
      I3 => mod7_active_line(4),
      O => CHOICE12552
    );
  mod7_n0046_3_776_G : LUT4
    generic map(
      INIT => X"0103"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(8),
      I2 => mod7_N271,
      I3 => mod7_active_line(7),
      O => N8541
    );
  mod7_Ker249143 : LUT4
    generic map(
      INIT => X"3133"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => N8269,
      I2 => mod7_active_line(0),
      I3 => mod7_N230,
      O => CHOICE12551
    );
  mod7_n0046_3_1203 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => CHOICE13571,
      I1 => CHOICE13584,
      I2 => CHOICE13629,
      I3 => CHOICE13735,
      O => CHOICE13736
    );
  mod7_Ker229283_SW1 : LUT3
    generic map(
      INIT => X"5D"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(4),
      I2 => mod7_active_line(2),
      O => N8466
    );
  mod7_n0034_6_246 : LUT3_L
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => CHOICE14538,
      I1 => CHOICE14519,
      I2 => CHOICE14509,
      LO => mod7_n0034(6)
    );
  mod7_n0046_3_486 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => mod7_N271,
      I1 => mod7_active_line(6),
      I2 => N8299,
      I3 => mod7_active_line(4),
      O => CHOICE13554
    );
  mod5_hdframe_n0075_7_59 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_chroma_crc(17),
      I2 => mod5_hdframe_n0191(0),
      I3 => mod5_hdframe_n0192(0),
      O => CHOICE14717
    );
  mod7_h_state_FFd3_In69_SW0 : LUT4
    generic map(
      INIT => X"2AAE"
    )
    port map (
      I0 => mod7_h_state_FFd3,
      I1 => mod5_hdframe_sav,
      I2 => mod7_h_state_FFd1,
      I3 => mod7_tp_17_Q,
      O => N8159
    );
  mod7_n0047_3_544_SW1 : LUT4
    generic map(
      INIT => X"F272"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(8),
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(9),
      O => N8427
    );
  mod5_hdframe_linecount_1_rt_54 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_linecount(1),
      O => mod5_hdframe_linecount_1_rt
    );
  mod7_n0034_8_120 : LUT4
    generic map(
      INIT => X"5510"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_v_state(2),
      I2 => mod7_n0028,
      I3 => CHOICE14872,
      O => CHOICE14281
    );
  mod5_hdframe_XNor_stagelut48 : LUT4_L
    generic map(
      INIT => X"5666"
    )
    port map (
      I0 => mod5_hdframe_samplecount(7),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(2),
      I3 => mod0_hd_format_tmp(1),
      LO => mod5_hdframe_N168
    );
  mod7_n0047_2_8 : LUT4
    generic map(
      INIT => X"6E66"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(8),
      I2 => mod7_n0176,
      I3 => mod7_N4,
      O => CHOICE12993
    );
  mod7_n0046_3_16 : LUT4
    generic map(
      INIT => X"2A88"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(1),
      O => CHOICE13430
    );
  mod7_n0046_1_32_SW0 : LUT4
    generic map(
      INIT => X"FEBA"
    )
    port map (
      I0 => mod7_N105,
      I1 => mod7_active_line(9),
      I2 => mod7_active_line(6),
      I3 => N8277,
      O => N8407
    );
  mod7_n0044_3_1047_SW0 : LUT4
    generic map(
      INIT => X"A8FF"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_N282,
      I2 => mod7_N260,
      I3 => CHOICE13366,
      O => N8187
    );
  mod7_n0047_2_41_SW0 : LUT3
    generic map(
      INIT => X"BA"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(6),
      I2 => mod7_active_line(8),
      O => N8211
    );
  mod7_n0034_6_18 : LUT4
    generic map(
      INIT => X"FEBA"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_tp_19_Q,
      I2 => mod7_load_val(6),
      I3 => mod7_v_state(2),
      O => CHOICE14490
    );
  mod7_n0046_3_1187 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => CHOICE13640,
      I1 => CHOICE13650,
      I2 => CHOICE13679,
      I3 => CHOICE13734,
      O => CHOICE13735
    );
  mod7_color_n008132_SW0 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => mod7_color_yramp(7),
      I1 => mod7_color_yramp(3),
      I2 => mod7_color_yramp(8),
      I3 => mod7_color_yramp(9),
      O => N8143
    );
  mod7_n0046_1_32 : LUT4
    generic map(
      INIT => X"FFDA"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(7),
      I2 => mod7_active_line(8),
      I3 => N8407,
      O => CHOICE12893
    );
  mod5_hdframe_n0075_6_78 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => mod7_color_N98,
      I2 => mod7_color_N42,
      I3 => CHOICE14778,
      O => CHOICE14781
    );
  mod7_n005212 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod7_counter_count(1),
      I1 => mod7_counter_count(6),
      I2 => mod7_counter_count(7),
      I3 => mod7_counter_count(8),
      O => CHOICE14005
    );
  mod7_n0044_3_269 : LUT4
    generic map(
      INIT => X"E444"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => N8409,
      I2 => mod7_active_line(2),
      I3 => mod7_active_line(1),
      O => CHOICE13149
    );
  mod7_n0034_1_33 : LUT4
    generic map(
      INIT => X"88A8"
    )
    port map (
      I0 => CHOICE14172,
      I1 => mod7_N292,
      I2 => mod7_n0023,
      I3 => mod7_v_state(3),
      O => CHOICE14173
    );
  mod7_n0047_3_109 : LUT4
    generic map(
      INIT => X"8F88"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(8),
      I2 => mod7_tp_17_Q,
      I3 => CHOICE13792,
      O => CHOICE13796
    );
  mod5_hdframe_samplecount_10_rt3_55 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(10),
      O => mod5_hdframe_samplecount_10_rt3
    );
  mod5_Mrom_n0003_inst_lut4_510 : LUT3
    generic map(
      INIT => X"01"
    )
    port map (
      I0 => mod0_hd_format_tmp(1),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(2),
      O => mod5_field2_vblnk1_0_Q
    );
  mod7_Ker22107 : LUT4
    generic map(
      INIT => X"2030"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(8),
      I3 => mod7_active_line(4),
      O => CHOICE12710
    );
  mod7_n0034_8_97 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod7_load_val(8),
      I1 => CHOICE14273,
      I2 => mod7_N186,
      I3 => CHOICE14270,
      O => CHOICE14276
    );
  mod5_hdframe_XNor_stagelut42 : LUT4
    generic map(
      INIT => X"AA95"
    )
    port map (
      I0 => mod5_hdframe_linecount(9),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(2),
      I3 => mod0_hd_format_tmp(3),
      O => mod5_hdframe_N119
    );
  mod7_n0046_1_51 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod7_v_state(1),
      I1 => CHOICE12885,
      I2 => CHOICE12886,
      I3 => CHOICE12893,
      O => CHOICE12895
    );
  mod7_Ker4919 : LUT4
    generic map(
      INIT => X"5040"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => mod7_active_line(1),
      I2 => mod7_N230,
      I3 => mod7_active_line(2),
      O => CHOICE12475
    );
  mod7_n0034_1_29 : LUT4
    generic map(
      INIT => X"0880"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod7_tp_21_Q,
      I2 => mod0_pattern(0),
      I3 => mod0_pattern(1),
      O => CHOICE14172
    );
  mod7_Ker2579 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => mod7_v_state(3),
      I2 => mod7_tp_17_Q,
      I3 => N8387,
      O => CHOICE14080
    );
  mod7_n0046_3_848_SW0 : LUT4
    generic map(
      INIT => X"1101"
    )
    port map (
      I0 => mod7_N271,
      I1 => mod7_N255,
      I2 => mod7_N277,
      I3 => mod7_active_line(7),
      O => N8279
    );
  mod7_n0044_3_530 : LUT4
    generic map(
      INIT => X"135F"
    )
    port map (
      I0 => mod7_N259,
      I1 => mod7_N279,
      I2 => mod7_active_line(4),
      I3 => mod7_active_line(0),
      O => CHOICE13213
    );
  mod7_n0046_3_1172 : LUT4
    generic map(
      INIT => X"C040"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => CHOICE13733,
      I2 => CHOICE13706,
      I3 => N8358,
      O => CHOICE13734
    );
  mod7_color_n008123 : LUT4
    generic map(
      INIT => X"FFEF"
    )
    port map (
      I0 => mod7_color_yramp(4),
      I1 => mod7_color_yramp(1),
      I2 => mod7_color_yramp(5),
      I3 => mod7_color_yramp(0),
      O => CHOICE12773
    );
  mod5_hdframe_n0075_1_43 : LUT4_L
    generic map(
      INIT => X"5554"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => mod7_color_N28,
      I2 => CHOICE14603,
      I3 => CHOICE14601,
      LO => CHOICE14607
    );
  mod7_Ker2579_SW1 : LUT4_L
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_h_state_FFd3,
      I1 => mod0_hd_format_tmp(2),
      I2 => mod0_hd_format_tmp(1),
      I3 => mod0_hd_format_tmp(3),
      LO => N8387
    );
  mod7_active_line_5_rt_56 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(5),
      O => mod7_active_line_5_rt
    );
  mod7_n0034_8_76 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod7_tp_21_Q,
      I1 => mod7_N264,
      I2 => mod7_N223,
      I3 => mod7_N120,
      O => CHOICE14273
    );
  mod7_n0044_3_269_SW0 : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(8),
      I2 => mod7_active_line(7),
      O => N8409
    );
  mod7_n0046_3_533 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE13556,
      I1 => CHOICE13521,
      I2 => CHOICE13482,
      I3 => N8119,
      O => CHOICE13558
    );
  mod7_color_n008132 : LUT4
    generic map(
      INIT => X"FBFF"
    )
    port map (
      I0 => mod7_color_yramp(6),
      I1 => mod7_color_yramp(2),
      I2 => CHOICE12773,
      I3 => N8143,
      O => mod7_color_n0081
    );
  mod7_active_line_3_rt_57 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(3),
      O => mod7_active_line_3_rt
    );
  mod5_hdframe_n0070_6_333 : LUT4
    generic map(
      INIT => X"FFD5"
    )
    port map (
      I0 => mod5_hdframe_nor_cyo25,
      I1 => CHOICE14958,
      I2 => mod7_h_state_FFd1,
      I3 => N8145,
      O => CHOICE14984
    );
  mod7_n0034_1_17 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => mod7_N79,
      I1 => CHOICE14164,
      I2 => CHOICE14166,
      O => CHOICE14168
    );
  mod5_hdframe_n0075_6_61 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => mod7_color_v1h2_cb_8_Q,
      I1 => mod7_color_N115,
      I2 => mod7_color_N121,
      O => CHOICE14775
    );
  mod7_h_state_FFd2_1_58 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd2_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd2_1
    );
  mod7_active_line_4_rt_59 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(4),
      O => mod7_active_line_4_rt
    );
  mod5_hdframe_Ker51_rn_0111_G : LUT4_L
    generic map(
      INIT => X"FF09"
    )
    port map (
      I0 => mod5_hdframe_MUX_BLOCK_xyz(5),
      I1 => mod5_hdframe_f,
      I2 => mod5_hdframe_n0132,
      I3 => mod5_hdframe_N301,
      LO => N8543
    );
  mod7_n0044_3_332_SW1 : LUT4
    generic map(
      INIT => X"BFFF"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(3),
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(6),
      O => N8437
    );
  mod7_n0034_8_58 : LUT3_L
    generic map(
      INIT => X"54"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => CHOICE14259,
      I2 => CHOICE14267,
      LO => CHOICE14270
    );
  mod7_n0046_3_1154 : LUT4
    generic map(
      INIT => X"020A"
    )
    port map (
      I0 => CHOICE13732,
      I1 => mod7_active_line(2),
      I2 => mod7_n0271,
      I3 => N8173,
      O => CHOICE13733
    );
  mod5_hdframe_XNor_stagelut50 : LUT4
    generic map(
      INIT => X"5666"
    )
    port map (
      I0 => mod5_hdframe_samplecount(9),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(2),
      I3 => mod0_hd_format_tmp(1),
      O => mod5_hdframe_N170
    );
  mod7_n0047_2_34 : LUT4
    generic map(
      INIT => X"888C"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(8),
      I3 => mod7_active_line(9),
      O => CHOICE13004
    );
  mod7_n01761 : LUT3
    generic map(
      INIT => X"36"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_pattern(2),
      I2 => mod0_pattern(0),
      O => mod7_n0176
    );
  mod7_Ker2283 : LUT4
    generic map(
      INIT => X"888C"
    )
    port map (
      I0 => N8281,
      I1 => mod7_active_line(3),
      I2 => mod7_active_line(0),
      I3 => mod7_active_line(5),
      O => CHOICE12700
    );
  mod7_n0047_2_41_SW1 : LUT4
    generic map(
      INIT => X"F77F"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod0_pattern(2),
      I2 => mod0_pattern(1),
      I3 => mod0_pattern(0),
      O => N8435
    );
  mod5_hdframe_XNor_stagelut : LUT4
    generic map(
      INIT => X"3336"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod5_hdframe_linecount(0),
      I2 => mod0_hd_format_tmp(1),
      I3 => mod0_hd_format_tmp(2),
      O => mod5_hdframe_N75
    );
  mod7_n0034_1_40 : LUT4
    generic map(
      INIT => X"F2FA"
    )
    port map (
      I0 => mod7_n0026,
      I1 => mod7_tp_19_Q,
      I2 => mod7_n0024,
      I3 => mod7_tp_21_Q,
      O => CHOICE14177
    );
  mod7_n0046_3_367_SW2 : LUT4
    generic map(
      INIT => X"5153"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(7),
      I2 => mod7_active_line(2),
      I3 => mod7_active_line(3),
      O => N8468
    );
  mod0_bitptr_4_rt_60 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod0_bitptr(4),
      O => mod0_bitptr_4_rt
    );
  mod7_n0046_3_265 : LUT4
    generic map(
      INIT => X"444E"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(7),
      I2 => mod7_active_line(8),
      I3 => mod7_active_line(6),
      O => CHOICE13489
    );
  mod7_Ker2641 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_h_state_FFd3,
      I2 => N8594,
      O => mod7_N264
    );
  mod7_active_line_2_rt_61 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(2),
      O => mod7_active_line_2_rt
    );
  mod7_n0047_2_41 : LUT4
    generic map(
      INIT => X"FEEE"
    )
    port map (
      I0 => CHOICE13004,
      I1 => mod7_N106,
      I2 => N8211,
      I3 => N8435,
      O => CHOICE13006
    );
  mod7_n0034_8_33 : LUT4
    generic map(
      INIT => X"FFEC"
    )
    port map (
      I0 => CHOICE14264,
      I1 => mod7_N199,
      I2 => mod7_n0024,
      I3 => N8123,
      O => CHOICE14267
    );
  mod7_n0044_1_16 : LUT4
    generic map(
      INIT => X"FFD5"
    )
    port map (
      I0 => mod7_N270,
      I1 => mod7_N75,
      I2 => mod7_active_line(5),
      I3 => mod7_N143,
      O => CHOICE12783
    );
  mod7_n0044_3_494_SW0_SW0 : LUT3
    generic map(
      INIT => X"6E"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(0),
      O => N8347
    );
  mod7_n0046_1_79 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_N4,
      I2 => mod7_n0176,
      I3 => mod7_active_line(1),
      O => CHOICE12905
    );
  mod7_n0046_3_1147_G : LUT3
    generic map(
      INIT => X"EF"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_active_line(3),
      I2 => mod7_active_line(5),
      O => N8545
    );
  mod7_n0044_3_869 : LUT4
    generic map(
      INIT => X"AF8C"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_N270,
      I2 => N8301,
      I3 => mod7_active_line(0),
      O => CHOICE13314
    );
  mod7_n0044_3_869_SW0 : LUT4
    generic map(
      INIT => X"D8F8"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(8),
      I3 => mod7_active_line(5),
      O => N8301
    );
  mod7_n0044_3_542 : LUT4
    generic map(
      INIT => X"5F13"
    )
    port map (
      I0 => mod7_N259,
      I1 => mod7_active_line(4),
      I2 => mod7_active_line(0),
      I3 => mod7_N202,
      O => CHOICE13219
    );
  mod7_n0046_3_877 : LUT4
    generic map(
      INIT => X"3133"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_n0229,
      I2 => mod7_active_line(5),
      I3 => N8484,
      O => CHOICE13650
    );
  mod7_n0044_3_703 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_N249,
      I1 => mod7_N299,
      I2 => mod7_n0067,
      I3 => mod7_N233,
      O => CHOICE13270
    );
  mod7_n0047_3_440 : LUT4
    generic map(
      INIT => X"5F13"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(9),
      I2 => N8421,
      I3 => mod7_active_line(4),
      O => CHOICE13881
    );
  mod7_active_line_1_rt_62 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(1),
      O => mod7_active_line_1_rt
    );
  mod7_n0034_8_33_SW0 : LUT4_L
    generic map(
      INIT => X"5540"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_n0026,
      I2 => mod7_tp_21_Q,
      I3 => mod7_n0030,
      LO => N8123
    );
  mod7_n0046_3_400 : LUT4
    generic map(
      INIT => X"5510"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(6),
      I2 => mod7_active_line(9),
      I3 => mod7_active_line(8),
      O => CHOICE13525
    );
  mod7_n0034_2_223 : LUT4_L
    generic map(
      INIT => X"FEBA"
    )
    port map (
      I0 => CHOICE14467,
      I1 => mod7_tp_21_Q,
      I2 => CHOICE14456,
      I3 => CHOICE14477,
      LO => CHOICE14480
    );
  mod7_color_Ker1331 : LUT4_D
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_v_state(1),
      I1 => mod7_v_state(2),
      I2 => mod7_h_state_FFd4_4,
      I3 => mod7_h_state_FFd1_4,
      LO => N8642,
      O => mod7_color_N133
    );
  mod5_hdframe_n0075_5_129 : LUT4_L
    generic map(
      INIT => X"5554"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => mod7_color_N102,
      I2 => CHOICE15227,
      I3 => mod7_color_N41,
      LO => CHOICE15231
    );
  mod7_Ker2828 : LUT4_L
    generic map(
      INIT => X"5510"
    )
    port map (
      I0 => mod7_tp_21_Q,
      I1 => mod7_tp_17_Q,
      I2 => CHOICE14152,
      I3 => CHOICE14156,
      LO => CHOICE14159
    );
  mod7_n0044_3_1078_SW0 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => CHOICE13314,
      I1 => CHOICE13305,
      I2 => CHOICE13302,
      I3 => CHOICE13296,
      O => N8221
    );
  mod7_n0044_3_705 : LUT4
    generic map(
      INIT => X"0207"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => mod7_N251,
      I2 => N8163,
      I3 => mod7_active_line(7),
      O => CHOICE13271
    );
  mod7_counter_n0002_10_1 : LUT4
    generic map(
      INIT => X"020A"
    )
    port map (
      I0 => mod7_counter_n0009(21),
      I1 => CHOICE14039,
      I2 => mod7_counter_load,
      I3 => CHOICE14026,
      O => mod7_counter_n0002(10)
    );
  mod0_bitptr_2_rt_63 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod0_bitptr(2),
      O => mod0_bitptr_2_rt
    );
  mod7_Ker229361_SW0_G : LUT3
    generic map(
      INIT => X"BA"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(4),
      I2 => mod7_active_line(6),
      O => N8547
    );
  mod5_hdframe_samplecount_8_rt_64 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(8),
      LO => mod5_hdframe_samplecount_8_rt
    );
  mod5_hdframe_n0075_5_115 : LUT4
    generic map(
      INIT => X"FFA8"
    )
    port map (
      I0 => mod7_color_v0h2_y_0_Q,
      I1 => mod7_color_n0015,
      I2 => mod7_color_n0011,
      I3 => N8615,
      O => CHOICE15227
    );
  mod7_n0034_8_16 : LUT4
    generic map(
      INIT => X"7077"
    )
    port map (
      I0 => sif_hd_format(1),
      I1 => sif_hd_format(2),
      I2 => mod7_n0025,
      I3 => N8263,
      O => CHOICE14259
    );
  mod7_Ker2544 : LUT4_L
    generic map(
      INIT => X"3931"
    )
    port map (
      I0 => mod7_v_state(3),
      I1 => mod7_h_state_FFd3,
      I2 => mod7_tp_21_Q,
      I3 => mod7_tp_17_Q,
      LO => CHOICE14069
    );
  mod5_hdframe_hd_framegenerator_n0083_10_lut1 : LUT3
    generic map(
      INIT => X"15"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(2),
      O => N7975
    );
  mod7_active_line_9_rt_65 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_active_line(9),
      O => mod7_active_line_9_rt
    );
  mod7_n0044_3_722 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => CHOICE13241,
      I1 => CHOICE13251,
      I2 => CHOICE13271,
      O => CHOICE13272
    );
  mod7_n0047_3_90_G : LUT4
    generic map(
      INIT => X"EEEB"
    )
    port map (
      I0 => mod7_n0078,
      I1 => mod0_pattern(2),
      I2 => mod0_pattern(1),
      I3 => mod0_pattern(0),
      O => N8549
    );
  mod0_bitptr_1_rt_66 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod0_bitptr(1),
      O => mod0_bitptr_1_rt
    );
  mod5_hdframe_samplecount_9_rt_67 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(9),
      O => mod5_hdframe_samplecount_9_rt
    );
  mod7_Ker1181 : LUT4_D
    generic map(
      INIT => X"BAAA"
    )
    port map (
      I0 => mod7_n0029,
      I1 => mod7_tp_17_Q,
      I2 => mod7_h_state_FFd3,
      I3 => mod7_N237,
      LO => N8643,
      O => mod7_N118
    );
  mod7_n0044_3_44 : LUT4
    generic map(
      INIT => X"F111"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => N8423,
      I2 => mod7_N260,
      I3 => mod7_active_line(0),
      O => CHOICE13094
    );
  mod7_n0046_3_152_SW1 : LUT4
    generic map(
      INIT => X"6766"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(4),
      I2 => mod7_active_line(1),
      I3 => mod7_active_line(9),
      O => N8396
    );
  mod7_n0046_0_102_SW0 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(4),
      I3 => mod7_N115,
      O => N8215
    );
  mod7_h_state_FFd4_In68 : LUT4
    generic map(
      INIT => X"A280"
    )
    port map (
      I0 => CHOICE13991,
      I1 => mod7_h_state_FFd3,
      I2 => mod7_h_state_FFd1,
      I3 => mod7_N237,
      O => CHOICE13992
    );
  mod5_hdframe_hd_framegenerator_n0083_5_lut1 : LUT4
    generic map(
      INIT => X"1131"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(1),
      O => N7976
    );
  mod7_n0034_8_16_SW0 : LUT4_L
    generic map(
      INIT => X"000B"
    )
    port map (
      I0 => mod7_N257,
      I1 => mod7_N72,
      I2 => mod7_N190,
      I3 => mod7_n0024,
      LO => N8263
    );
  mod7_n0046_3_1172_SW0 : LUT4
    generic map(
      INIT => X"C1C0"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(7),
      O => N8358
    );
  mod7_n0044_3_739 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => CHOICE13213,
      I1 => CHOICE13219,
      I2 => CHOICE13233,
      I3 => CHOICE13272,
      O => CHOICE13273
    );
  mod7_n0046_3_1068 : LUT4
    generic map(
      INIT => X"FA72"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(9),
      I2 => N8305,
      I3 => mod7_active_line(5),
      O => CHOICE13706
    );
  mod7_n0045_0_22 : LUT4
    generic map(
      INIT => X"3133"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(2),
      I3 => mod7_active_line(0),
      O => CHOICE13383
    );
  mod7_n0046_1_98_G : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_N236,
      I1 => mod7_tp_17_Q,
      I2 => mod7_N251,
      I3 => mod7_active_line(7),
      O => N8551
    );
  mod7_n0034_2_13 : LUT4
    generic map(
      INIT => X"DC54"
    )
    port map (
      I0 => mod7_N257,
      I1 => mod7_n0026,
      I2 => N8590,
      I3 => mod7_tp_21_Q,
      O => CHOICE14434
    );
  mod7_n0044_1_59 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_active_line(6),
      I2 => mod7_active_line(4),
      O => CHOICE12798
    );
  mod7_n0046_3_78 : LUT4
    generic map(
      INIT => X"AABA"
    )
    port map (
      I0 => mod7_n0075,
      I1 => mod7_n0076,
      I2 => mod7_n0077,
      I3 => mod7_tp_19_Q,
      O => CHOICE13448
    );
  mod5_hdframe_n0075_1_29 : LUT4
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_color_v0h2_y_0_Q,
      I1 => mod7_color_N120,
      I2 => mod7_color_N134,
      I3 => mod7_color_n0014,
      O => CHOICE14603
    );
  mod5_hdframe_n0075_3_17 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_chroma_out(0),
      I2 => mod5_hdframe_chroma_crc(0),
      I3 => mod5_hdframe_chroma_crc(13),
      LO => CHOICE15441
    );
  mod5_hdframe_hd_framegenerator_n0083_6_lut2 : LUT4
    generic map(
      INIT => X"0207"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(3),
      I3 => mod0_hd_format_tmp(0),
      O => N7983
    );
  mod7_n0034_4_92 : LUT4
    generic map(
      INIT => X"EEAE"
    )
    port map (
      I0 => mod7_n0029,
      I1 => CHOICE14872,
      I2 => mod7_tp_21_Q,
      I3 => mod7_tp_19_Q,
      O => CHOICE14309
    );
  mod7_n0047_3_421 : LUT4
    generic map(
      INIT => X"135F"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(7),
      I2 => mod7_active_line(8),
      I3 => N8287,
      O => CHOICE13874
    );
  mod7_n0046_0_19 : LUT3
    generic map(
      INIT => X"15"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(5),
      O => CHOICE12920
    );
  mod7_n0034_6_221 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod7_load_val(6),
      I1 => mod7_N93,
      I2 => CHOICE14526,
      I3 => CHOICE14535,
      O => CHOICE14538
    );
  mod7_n0047_3_440_SW0 : LUT4
    generic map(
      INIT => X"EEEF"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(2),
      I3 => mod7_active_line(6),
      O => N8421
    );
  mod7_n0034_7_156 : LUT2_L
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => CHOICE14245,
      I1 => CHOICE14250,
      LO => CHOICE14251
    );
  mod5_hdframe_n0070_5_208_SW1 : LUT4
    generic map(
      INIT => X"AAFE"
    )
    port map (
      I0 => mod5_hdframe_n0115,
      I1 => mod5_hdframe_n0116,
      I2 => N8335,
      I3 => CHOICE15392,
      O => N8059
    );
  mod7_n0034_7_153 : LUT4
    generic map(
      INIT => X"FFEC"
    )
    port map (
      I0 => mod7_N182,
      I1 => CHOICE14247,
      I2 => CHOICE14872,
      I3 => N8597,
      O => CHOICE14250
    );
  mod7_n0034_8_27 : LUT4
    generic map(
      INIT => X"BFFF"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(1),
      I2 => mod0_pattern(0),
      I3 => mod7_v_state(2),
      O => CHOICE14264
    );
  mod7_Ker285 : LUT4
    generic map(
      INIT => X"C8C0"
    )
    port map (
      I0 => mod7_v_state(2),
      I1 => mod7_tp_19_Q,
      I2 => mod7_n0023,
      I3 => N8589,
      O => CHOICE14152
    );
  mod7_Ker2834 : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => mod7_v_state(3),
      I1 => mod7_N264,
      I2 => CHOICE14159,
      O => mod7_N28
    );
  mod7_Ker57_SW0 : LUT4
    generic map(
      INIT => X"028A"
    )
    port map (
      I0 => mod7_n0024,
      I1 => mod7_tp_17_Q,
      I2 => mod7_N257,
      I3 => mod7_v_state(3),
      O => N4514
    );
  mod5_hdframe_n0075_9_246 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => mod7_h_state_FFd1,
      I1 => mod7_v_state(2),
      I2 => N8383,
      O => CHOICE14861
    );
  mod5_hdframe_linecount_3_rt_68 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_linecount(3),
      O => mod5_hdframe_linecount_3_rt
    );
  mod7_n0044_1_47 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_N4,
      I1 => mod7_active_line(6),
      I2 => mod7_n0176,
      I3 => mod7_active_line(0),
      O => CHOICE12793
    );
  mod7_n0047_3_421_SW0 : LUT4
    generic map(
      INIT => X"BBBF"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(2),
      I3 => mod7_active_line(6),
      O => N8287
    );
  mod7_n0044_1_94 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(5),
      I3 => N8329,
      O => CHOICE12803
    );
  mod7_n003531 : LUT4_L
    generic map(
      INIT => X"FF27"
    )
    port map (
      I0 => mod7_n0029,
      I1 => mod7_n0052,
      I2 => mod7_N237,
      I3 => CHOICE13963,
      LO => CHOICE13968
    );
  mod7_n0045_0_27 : LUT4
    generic map(
      INIT => X"F3F1"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(6),
      I2 => CHOICE13383,
      I3 => mod7_N6,
      O => CHOICE13384
    );
  mod7_n0044_3_44_SW1 : LUT4
    generic map(
      INIT => X"8A8F"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(8),
      I3 => mod7_active_line(3),
      O => N8423
    );
  mod7_n0034_1_9 : LUT4
    generic map(
      INIT => X"8F88"
    )
    port map (
      I0 => mod7_N51,
      I1 => mod7_N10,
      I2 => mod7_v_state(3),
      I3 => mod7_n0024,
      O => CHOICE14166
    );
  mod5_hdframe_hd_framegenerator_n0088_11_lut1 : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(3),
      O => N7972
    );
  mod7_n0044_3_332 : LUT4
    generic map(
      INIT => X"BAAA"
    )
    port map (
      I0 => CHOICE13166,
      I1 => mod7_active_line(0),
      I2 => mod7_active_line(8),
      I3 => N8437,
      O => CHOICE13167
    );
  mod5_hdframe_n0079_3_1 : LUT4
    generic map(
      INIT => X"0220"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      I2 => mod5_hdframe_n0190(0),
      I3 => mod5_hdframe_chroma_crc(13),
      O => mod5_hdframe_n0079(3)
    );
  mod7_h_state_FFd4_2_69 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd4_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd4_2
    );
  mod7_n0034_7_132 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod7_tp_21_Q,
      I1 => CHOICE14239,
      I2 => CHOICE14242,
      I3 => CHOICE14236,
      O => CHOICE14245
    );
  mod7_n0045_2_41 : LUT4
    generic map(
      INIT => X"AF8C"
    )
    port map (
      I0 => mod7_N202,
      I1 => CHOICE13922,
      I2 => mod7_N287,
      I3 => CHOICE13926,
      O => mod7_n0315_22_Q
    );
  mod7_n0046_3_877_SW0 : LUT3
    generic map(
      INIT => X"7F"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_active_line(6),
      I2 => mod7_active_line(7),
      O => N8484
    );
  mod5_hdframe_n0075_1_88 : LUT4_L
    generic map(
      INIT => X"5444"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => CHOICE14592,
      I2 => mod5_hdframe_N881,
      I3 => CHOICE14608,
      LO => CHOICE14612
    );
  mod7_n0046_3_209_SW0 : LUT3
    generic map(
      INIT => X"7F"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(5),
      O => N8413
    );
  mod7_n0044_3_347 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE13135,
      I1 => CHOICE13140,
      I2 => CHOICE13149,
      I3 => CHOICE13167,
      O => CHOICE13168
    );
  mod5_hdframe_n007821_SW0 : LUT3_L
    generic map(
      INIT => X"EF"
    )
    port map (
      I0 => mod5_hdframe_n0094,
      I1 => mod5_hdframe_n0095,
      I2 => sif_rst,
      LO => N8251
    );
  mod7_Ker25100_SW1 : LUT4
    generic map(
      INIT => X"BA10"
    )
    port map (
      I0 => mod7_h_state_FFd1,
      I1 => mod7_h_state_FFd2,
      I2 => CHOICE14069,
      I3 => mod7_h_state_FFd3,
      O => N8385
    );
  mod7_n0046_0_44_SW0 : LUT4
    generic map(
      INIT => X"81B3"
    )
    port map (
      I0 => mod0_pattern(0),
      I1 => mod0_pattern(2),
      I2 => mod0_pattern(1),
      I3 => mod7_active_line(7),
      O => N8398
    );
  mod7_n0046_3_427_G : LUT4
    generic map(
      INIT => X"2064"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(9),
      I3 => mod7_active_line(3),
      O => N8553
    );
  mod5_hdframe_Eq_stagelut27 : LUT4_L
    generic map(
      INIT => X"1482"
    )
    port map (
      I0 => mod5_hdframe_linecount(4),
      I1 => sif_hd_format(1),
      I2 => sif_hd_format(2),
      I3 => mod5_hdframe_linecount(5),
      LO => mod5_hdframe_N141
    );
  mod7_n0046_3_1154_SW0 : LUT4
    generic map(
      INIT => X"4F4C"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(9),
      I2 => mod7_active_line(4),
      I3 => mod7_active_line(5),
      O => N8173
    );
  mod7_n0034_7_109 : LUT4
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_load_val(7),
      I1 => mod7_n0026,
      I2 => mod7_N182,
      I3 => N8641,
      O => CHOICE14242
    );
  mod7_n0044_2_15_SW0 : LUT4
    generic map(
      INIT => X"AE8C"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(4),
      I3 => mod7_active_line(5),
      O => N8311
    );
  mod7_h_state_FFd4_In29 : LUT4
    generic map(
      INIT => X"2332"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => mod7_h_state_FFd4,
      I2 => mod7_h_state_FFd3,
      I3 => mod7_h_state_FFd1,
      O => CHOICE13984
    );
  mod7_n0046_0_44 : LUT4
    generic map(
      INIT => X"F5F4"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(1),
      I2 => N8398,
      I3 => mod7_active_line(5),
      O => CHOICE12930
    );
  mod7_n0045_2_20 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => mod7_n0069,
      I1 => mod7_N229,
      I2 => mod7_tp_17_Q,
      O => CHOICE13924
    );
  mod5_hdframe_n00784 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => mod5_hdframe_n0103,
      I1 => mod5_hdframe_n0104,
      I2 => mod5_hdframe_n0106,
      I3 => mod5_hdframe_n0105,
      O => CHOICE14142
    );
  mod7_n0047_1_42 : LUT3
    generic map(
      INIT => X"8A"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_N4,
      I2 => mod7_active_line(6),
      O => CHOICE13753
    );
  mod7_n0044_2_15 : LUT4
    generic map(
      INIT => X"FEEE"
    )
    port map (
      I0 => mod7_N142,
      I1 => N8311,
      I2 => mod7_active_line(6),
      I3 => mod7_N217,
      O => CHOICE12811
    );
  mod7_n0047_3_263 : LUT4
    generic map(
      INIT => X"888C"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => CHOICE13827,
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(6),
      O => CHOICE13828
    );
  mod7_n0046_3_453 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_N276,
      I2 => mod7_active_line(9),
      I3 => mod7_N253,
      O => CHOICE13541
    );
  mod7_n0046_3_367 : LUT4
    generic map(
      INIT => X"FEEE"
    )
    port map (
      I0 => CHOICE13515,
      I1 => N8115,
      I2 => mod7_active_line(9),
      I3 => N8468,
      O => CHOICE13520
    );
  mod7_n0034_7_103 : LUT4
    generic map(
      INIT => X"FEEE"
    )
    port map (
      I0 => CHOICE14872,
      I1 => mod7_n0029,
      I2 => mod7_n0030,
      I3 => mod7_N227,
      O => CHOICE14239
    );
  mod7_n0047_0_15 : LUT4
    generic map(
      INIT => X"5554"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(3),
      I2 => CHOICE13023,
      I3 => mod7_active_line(4),
      O => CHOICE13026
    );
  mod7_n0045_2_13 : LUT4
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => CHOICE13921,
      I1 => mod7_N229,
      I2 => mod7_tp_19_Q,
      I3 => mod7_v_state(2),
      O => CHOICE13922
    );
  mod5_hdframe_samplecount_10_rt_70 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(10),
      LO => mod5_hdframe_samplecount_10_rt
    );
  mod7_n0046_3_533_SW0_SW0 : LUT4
    generic map(
      INIT => X"3676"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(9),
      I2 => mod7_active_line(6),
      I3 => mod7_active_line(7),
      O => N8345
    );
  mod7_n0046_0_61 : LUT4
    generic map(
      INIT => X"5554"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(3),
      I2 => CHOICE12930,
      I3 => mod7_N185,
      O => CHOICE12933
    );
  mod7_n0034_0_235 : LUT4_L
    generic map(
      INIT => X"FFEC"
    )
    port map (
      I0 => mod7_load_val(0),
      I1 => CHOICE14119,
      I2 => CHOICE14136,
      I3 => N8125,
      LO => mod7_n0034(0)
    );
  mod7_n0046_3_972_SW1_SW0 : LUT4
    generic map(
      INIT => X"EAC8"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(9),
      I3 => mod7_active_line(1),
      O => N8494
    );
  mod7_n0047_0_5 : LUT4
    generic map(
      INIT => X"4FCF"
    )
    port map (
      I0 => mod7_n0176,
      I1 => mod7_N6,
      I2 => mod7_active_line(8),
      I3 => mod7_active_line(6),
      O => CHOICE13023
    );
  mod7_n0047_0_21 : LUT4
    generic map(
      INIT => X"F2FA"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(1),
      I3 => mod7_active_line(7),
      O => CHOICE13028
    );
  mod7_n0044_1_8 : LUT4
    generic map(
      INIT => X"F7FE"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(0),
      I2 => mod7_N233,
      I3 => mod7_active_line(4),
      O => CHOICE12779
    );
  mod7_color_Ker82 : LUT4
    generic map(
      INIT => X"FEF0"
    )
    port map (
      I0 => mod7_color_v0h2_y_2_Q,
      I1 => mod7_color_v0h2_y_0_Q,
      I2 => N8601,
      I3 => mod7_color_n0031,
      O => mod7_color_N82
    );
  mod7_n0045_2_10 : LUT3
    generic map(
      INIT => X"04"
    )
    port map (
      I0 => mod7_n0069,
      I1 => mod7_n0070,
      I2 => mod7_tp_17_Q,
      O => CHOICE13921
    );
  mod7_n0044_2_29 : LUT4
    generic map(
      INIT => X"5510"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(3),
      I2 => mod7_active_line(8),
      I3 => mod7_N278,
      O => CHOICE12816
    );
  mod7_n0044_3_767 : LUT4
    generic map(
      INIT => X"AF23"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_N263,
      I2 => mod7_N250,
      I3 => mod7_active_line(5),
      O => CHOICE13281
    );
  mod7_n0044_2_47_SW0 : LUT4
    generic map(
      INIT => X"EAAA"
    )
    port map (
      I0 => CHOICE12816,
      I1 => mod7_active_line(8),
      I2 => mod7_N4,
      I3 => N8488,
      O => N8213
    );
  mod5_hdframe_n0070_1_70_SW0 : LUT4_L
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE15342,
      I1 => CHOICE15339,
      I2 => mod7_color_N77,
      I3 => CHOICE14740,
      LO => N8081
    );
  mod7_color_n00291 : LUT3
    generic map(
      INIT => X"04"
    )
    port map (
      I0 => mod0_pattern(0),
      I1 => mod0_pattern(1),
      I2 => mod0_pattern(2),
      O => mod7_color_v0h2_y_2_Q
    );
  mod7_n0034_7_90 : LUT3
    generic map(
      INIT => X"EC"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_n0023,
      I2 => N8460,
      O => CHOICE14234
    );
  mod7_Ker36_SW0 : LUT4_L
    generic map(
      INIT => X"5510"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_tp_19_Q,
      I2 => mod7_n0026,
      I3 => mod7_n0027,
      LO => N4283
    );
  mod7_n0044_3_327_G : LUT4
    generic map(
      INIT => X"0155"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(6),
      I2 => mod7_active_line(1),
      I3 => mod7_active_line(5),
      O => N8555
    );
  mod7_n0044_3_575 : LUT4
    generic map(
      INIT => X"75FF"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(3),
      I3 => mod7_active_line(4),
      O => CHOICE13232
    );
  mod7_n0034_0_198 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE14124,
      I1 => mod7_N36,
      I2 => CHOICE14128,
      I3 => CHOICE14133,
      O => CHOICE14136
    );
  mod7_n0047_3_69 : LUT4
    generic map(
      INIT => X"888C"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(7),
      I2 => mod7_active_line(6),
      I3 => mod7_active_line(2),
      O => CHOICE13783
    );
  mod7_n0045_0_98_SW0 : LUT4
    generic map(
      INIT => X"FAD8"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(0),
      I2 => CHOICE13384,
      I3 => mod7_active_line(4),
      O => N8169
    );
  mod5_hdframe_n0070_6_193_SW0_SW0 : LUT4
    generic map(
      INIT => X"FF9F"
    )
    port map (
      I0 => mod0_pattern(0),
      I1 => mod0_pattern(1),
      I2 => mod0_pattern(2),
      I3 => mod7_h_state_FFd2_5,
      O => N8373
    );
  mod7_n0047_1_29 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod7_N232,
      I1 => mod7_active_line(1),
      I2 => CHOICE13748,
      O => CHOICE13750
    );
  mod7_n0046_3_403 : LUT4
    generic map(
      INIT => X"AABA"
    )
    port map (
      I0 => mod7_n0263,
      I1 => mod7_active_line(6),
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(3),
      O => CHOICE13527
    );
  mod7_n0046_3_92 : LUT4
    generic map(
      INIT => X"5510"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_n0176,
      I2 => mod7_n0076,
      I3 => CHOICE13448,
      O => CHOICE13451
    );
  mod7_n0046_3_486_SW0 : LUT3
    generic map(
      INIT => X"F6"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(3),
      O => N8299
    );
  mod7_n0046_3_215_SW1 : LUT4
    generic map(
      INIT => X"FEBA"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(7),
      I2 => N8413,
      I3 => mod7_active_line(2),
      O => N8450
    );
  mod7_n0047_2_104_G : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_N300,
      I2 => mod7_active_line(3),
      I3 => mod7_active_line(1),
      O => N8557
    );
  mod5_hdframe_hd_framegenerator_n0083_5_lut2 : LUT4
    generic map(
      INIT => X"1131"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(1),
      O => N7982
    );
  mod7_n0044_3_375 : LUT4
    generic map(
      INIT => X"F111"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => mod7_N270,
      I2 => mod7_active_line(4),
      I3 => mod7_N282,
      O => CHOICE13177
    );
  mod7_n0044_2_47 : LUT4
    generic map(
      INIT => X"FEEE"
    )
    port map (
      I0 => CHOICE12821,
      I1 => N8213,
      I2 => mod7_active_line(3),
      I3 => mod7_N75,
      O => CHOICE12823
    );
  mod7_n0047_3_258 : LUT4
    generic map(
      INIT => X"FBBF"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_N232,
      I2 => mod7_active_line(6),
      I3 => mod7_active_line(2),
      O => CHOICE13827
    );
  mod7_n0046_3_620 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(2),
      I2 => N8470,
      I3 => N8492,
      O => CHOICE13584
    );
  mod5_hdframe_n0070_3_0 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0115,
      I1 => mod5_hdframe_linecount(8),
      O => CHOICE15131
    );
  mod5_hdframe_hd_framegenerator_n0088_1_lut : LUT4
    generic map(
      INIT => X"1131"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(1),
      O => mod5_hdframe_N44
    );
  mod7_n0034_0_176 : LUT4
    generic map(
      INIT => X"1110"
    )
    port map (
      I0 => mod7_tp_21_Q,
      I1 => mod7_v_state(2),
      I2 => mod7_n0026,
      I3 => mod7_n0030,
      O => CHOICE14133
    );
  mod7_n0046_0_84 : LUT4
    generic map(
      INIT => X"0103"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(1),
      I2 => mod0_pattern(0),
      I3 => mod7_active_line(7),
      O => CHOICE12940
    );
  mod7_n0046_3_972 : LUT4
    generic map(
      INIT => X"5115"
    )
    port map (
      I0 => N8476,
      I1 => mod7_active_line(6),
      I2 => mod7_active_line(1),
      I3 => mod7_active_line(2),
      O => CHOICE13679
    );
  mod7_n0044_3_580 : LUT4
    generic map(
      INIT => X"44C4"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => CHOICE13232,
      I2 => mod7_N202,
      I3 => mod7_N253,
      O => CHOICE13233
    );
  mod7_n0046_0_102 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE12940,
      I1 => CHOICE12933,
      I2 => CHOICE12920,
      I3 => N8215,
      O => CHOICE12942
    );
  mod7_n0045_3_39_SW0 : LUT3_L
    generic map(
      INIT => X"DF"
    )
    port map (
      I0 => CHOICE14048,
      I1 => mod7_n0069,
      I2 => mod7_n0070,
      LO => N8337
    );
  mod7_n0034_7_50 : LUT4
    generic map(
      INIT => X"F111"
    )
    port map (
      I0 => mod7_v_state(3),
      I1 => N8464,
      I2 => mod7_N7,
      I3 => mod7_n0030,
      O => CHOICE14228
    );
  mod7_h_state_FFd4_In5 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod7_h_state_FFd3,
      I1 => mod7_v_state(3),
      I2 => mod7_h_state_FFd2,
      O => CHOICE13976
    );
  mod7_n0044_3_928 : LUT4
    generic map(
      INIT => X"888C"
    )
    port map (
      I0 => mod7_N217,
      I1 => CHOICE13326,
      I2 => mod7_active_line(2),
      I3 => mod7_active_line(1),
      O => CHOICE13327
    );
  mod7_color_Ker77_SW0_SW0 : LUT4_L
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod7_color_v1h4_y(9),
      I2 => mod7_tp_4_Q,
      I3 => mod7_tp_0_Q,
      LO => N8243
    );
  mod7_n0044_3_922 : LUT3
    generic map(
      INIT => X"D7"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(0),
      I2 => mod7_active_line(1),
      O => CHOICE13326
    );
  mod7_n0046_0_118 : LUT4
    generic map(
      INIT => X"FFD5"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(7),
      I3 => CHOICE12942,
      O => CHOICE12943
    );
  mod7_n0045_3_13 : LUT4
    generic map(
      INIT => X"BA10"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_tp_19_Q,
      I2 => mod7_v_state(3),
      I3 => mod7_N229,
      O => CHOICE14048
    );
  mod7_n0047_3_207 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE13783,
      I1 => CHOICE13796,
      I2 => CHOICE13802,
      I3 => CHOICE13817,
      O => CHOICE13818
    );
  mod5_hdframe_n0075_6_91 : LUT4
    generic map(
      INIT => X"FFA8"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_color_n0019,
      I2 => mod7_color_n0023,
      I3 => N8651,
      O => CHOICE14786
    );
  mod7_n0047_3_50 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => mod7_N299,
      I1 => mod7_N298,
      I2 => CHOICE13777,
      I3 => N8341,
      O => CHOICE13779
    );
  mod7_n0034_7_37 : LUT4
    generic map(
      INIT => X"5510"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_tp_21_Q,
      I2 => CHOICE14218,
      I3 => CHOICE14222,
      O => CHOICE14225
    );
  mod5_hdframe_n0070_2_40 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => mod7_color_yramp(2),
      I1 => mod7_color_N38,
      I2 => CHOICE15180,
      I3 => mod7_color_N91,
      O => CHOICE15183
    );
  mod7_n0044_3_116 : LUT4
    generic map(
      INIT => X"AABA"
    )
    port map (
      I0 => mod7_n0065,
      I1 => mod7_n0066,
      I2 => mod7_n0067,
      I3 => mod7_tp_19_Q,
      O => CHOICE13115
    );
  mod7_n0034_0_167 : LUT4
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => mod7_n0024,
      I1 => mod7_n0025,
      I2 => mod7_v_state(3),
      I3 => mod7_n0027,
      O => CHOICE14128
    );
  mod7_n0047_3_594 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE13912,
      I1 => mod7_v_state(3),
      I2 => CHOICE13779,
      I3 => CHOICE13818,
      O => CHOICE13913
    );
  mod5_hdframe_n0070_9_89_SW0 : LUT3
    generic map(
      INIT => X"EF"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_luma_crc(17),
      I2 => mod5_hdframe_n0100,
      O => N8023
    );
  mod5_hdframe_hd_framegenerator_n0083_3_lut1 : LUT4
    generic map(
      INIT => X"FEBA"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(2),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(1),
      O => N7970
    );
  mod7_n003558 : LUT3_L
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => mod7_yramp_en,
      I1 => CHOICE13968,
      I2 => CHOICE13972,
      LO => mod7_n0035
    );
  mod7_n0044_2_41 : LUT4
    generic map(
      INIT => X"AABA"
    )
    port map (
      I0 => mod7_N184,
      I1 => mod7_active_line(0),
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(6),
      O => CHOICE12821
    );
  mod7_n0047_3_192 : LUT4
    generic map(
      INIT => X"EEAE"
    )
    port map (
      I0 => N8183,
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(7),
      I3 => mod7_active_line(9),
      O => CHOICE13817
    );
  mod7_n003556 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => N8648,
      I1 => mod7_v_state(2),
      I2 => mod7_h_state_FFd3,
      I3 => mod7_N237,
      O => CHOICE13972
    );
  mod7_n0047_3_578 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => CHOICE13828,
      I1 => CHOICE13838,
      I2 => CHOICE13861,
      I3 => CHOICE13911,
      O => CHOICE13912
    );
  mod7_n0046_3_1068_SW0 : LUT4
    generic map(
      INIT => X"1131"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(8),
      I2 => mod7_active_line(6),
      I3 => mod7_active_line(2),
      O => N8305
    );
  mod7_n0034_1_6 : LUT4_L
    generic map(
      INIT => X"020A"
    )
    port map (
      I0 => mod7_n0025,
      I1 => mod7_v_state(2),
      I2 => mod7_tp_21_Q,
      I3 => mod7_tp_19_Q,
      LO => CHOICE14164
    );
  mod7_n0047_3_363 : LUT4
    generic map(
      INIT => X"0103"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_N61,
      I2 => N8217,
      I3 => mod7_active_line(9),
      O => CHOICE13861
    );
  mod7_n0044_2_102 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_N304,
      I2 => mod7_N254,
      I3 => mod7_active_line(2),
      O => CHOICE12834
    );
  mod7_n0046_3_215 : LUT4
    generic map(
      INIT => X"FEBA"
    )
    port map (
      I0 => mod7_n0223,
      I1 => mod7_active_line(6),
      I2 => mod7_active_line(7),
      I3 => N8450,
      O => CHOICE13482
    );
  mod5_hdframe_n0070_6_193 : LUT4
    generic map(
      INIT => X"FFA8"
    )
    port map (
      I0 => mod7_tp_5_Q,
      I1 => mod7_color_yramp(6),
      I2 => mod7_tp_0_Q,
      I3 => N8245,
      O => CHOICE14958
    );
  mod7_n0047_0_59 : LUT4
    generic map(
      INIT => X"A8AA"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_N115,
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(4),
      O => CHOICE13042
    );
  mod7_n0044_2_105 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(7),
      I2 => CHOICE12834,
      O => CHOICE12835
    );
  mod7_n0047_3_560 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => CHOICE13874,
      I1 => CHOICE13881,
      I2 => CHOICE13910,
      O => CHOICE13911
    );
  mod7_n0044_3_398_SW1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(3),
      O => N8425
    );
  mod7_n0034_0_155 : LUT4_L
    generic map(
      INIT => X"EAAA"
    )
    port map (
      I0 => N8379,
      I1 => mod7_N237,
      I2 => mod7_h_state_FFd3,
      I3 => mod7_tp_17_Q,
      LO => CHOICE14124
    );
  mod7_n0034_5_147 : LUT4
    generic map(
      INIT => X"EEAE"
    )
    port map (
      I0 => CHOICE14417,
      I1 => CHOICE14872,
      I2 => mod7_tp_19_Q,
      I3 => mod7_tp_17_Q,
      O => CHOICE14419
    );
  mod7_n0034_0_155_SW0 : LUT4
    generic map(
      INIT => X"F5F4"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_n0030,
      I2 => mod7_N120,
      I3 => N8591,
      O => N8379
    );
  mod5_hdframe_linecount_6_rt_71 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_linecount(6),
      O => mod5_hdframe_linecount_6_rt
    );
  mod7_n0044_3_130 : LUT4
    generic map(
      INIT => X"5510"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_n0176,
      I2 => mod7_n0066,
      I3 => CHOICE13115,
      O => CHOICE13118
    );
  mod7_counter_Ker062 : LUT3_D
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => CHOICE14026,
      I1 => CHOICE14039,
      I2 => mod7_counter_load,
      LO => N8644,
      O => mod7_counter_N01
    );
  mod5_hdframe_Ker52_rn_0111_G : LUT4_L
    generic map(
      INIT => X"FF06"
    )
    port map (
      I0 => mod5_hdframe_f,
      I1 => mod5_hdframe_v,
      I2 => mod5_hdframe_n0132,
      I3 => mod5_hdframe_N301,
      LO => N8559
    );
  mod7_n0044_2_125_SW0 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => mod7_active_line(4),
      I2 => mod7_tp_19_Q,
      I3 => mod7_N235,
      O => N8219
    );
  mod7_n0044_3_615 : LUT4
    generic map(
      INIT => X"135F"
    )
    port map (
      I0 => mod7_N251,
      I1 => mod7_N265,
      I2 => mod7_active_line(8),
      I3 => mod7_active_line(0),
      O => CHOICE13241
    );
  mod7_n0047_3_363_SW0_SW0 : LUT4
    generic map(
      INIT => X"AE8C"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(3),
      I2 => mod7_active_line(2),
      I3 => mod7_active_line(6),
      O => N8417
    );
  mod7_n0044_2_125 : LUT4
    generic map(
      INIT => X"5040"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => CHOICE12835,
      I2 => mod7_active_line(8),
      I3 => N8219,
      O => CHOICE12839
    );
  mod7_n0047_0_69 : LUT4
    generic map(
      INIT => X"FEEF"
    )
    port map (
      I0 => CHOICE13042,
      I1 => N8165,
      I2 => mod7_active_line(2),
      I3 => mod7_active_line(6),
      O => CHOICE13044
    );
  mod5_hdframe_n0070_0_6 : LUT4_L
    generic map(
      INIT => X"FEFC"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_color_N107,
      I2 => mod7_color_N79,
      I3 => mod7_color_n0036,
      LO => CHOICE14662
    );
  mod7_n0044_3_398 : LUT4
    generic map(
      INIT => X"1110"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => N8425,
      I2 => mod7_active_line(0),
      I3 => mod7_active_line(4),
      O => CHOICE13185
    );
  mod7_color_v0h5_cr_0_1 : LUT3
    generic map(
      INIT => X"7E"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_pattern(0),
      I2 => mod0_pattern(2),
      O => mod7_color_v0h5_cr(0)
    );
  mod7_n0047_3_544 : LUT4
    generic map(
      INIT => X"028A"
    )
    port map (
      I0 => CHOICE13898,
      I1 => mod7_active_line(6),
      I2 => N8427,
      I3 => N8428,
      O => CHOICE13910
    );
  mod5_hdframe_n01351 : LUT4
    generic map(
      INIT => X"FF5D"
    )
    port map (
      I0 => mod5_hdframe_XNor_stage_cyo20,
      I1 => mod5_hdframe_n0119,
      I2 => mod5_hdframe_XNor_stage_cyo31,
      I3 => mod5_hdframe_n0121,
      O => mod5_hdframe_n0135
    );
  mod5_hdframe_n0070_0_36_SW0 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => mod7_color_yramp(0),
      I1 => mod7_color_N26,
      I2 => CHOICE14667,
      I3 => CHOICE14662,
      O => N8091
    );
  mod7_counter_Ker039 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => CHOICE14038,
      I1 => mod7_counter_count(0),
      I2 => mod7_counter_count(4),
      I3 => mod7_counter_count(5),
      O => CHOICE14039
    );
  mod5_hdframe_n0070_2_32 : LUT4_L
    generic map(
      INIT => X"C080"
    )
    port map (
      I0 => mod0_tp_option(1),
      I1 => mod7_color_v1h9_y(5),
      I2 => mod7_color_n0018,
      I3 => mod0_tp_option(0),
      LO => CHOICE15180
    );
  mod7_n0034_0_137 : LUT4
    generic map(
      INIT => X"5540"
    )
    port map (
      I0 => mod7_tp_21_Q,
      I1 => CHOICE14107,
      I2 => mod7_tp_17_Q,
      I3 => CHOICE14116,
      O => CHOICE14119
    );
  mod5_hdframe_n0070_0_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_luma_crc(9),
      O => CHOICE14659
    );
  mod7_n0044_3_149 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => mod7_N299,
      I1 => mod7_N35,
      I2 => mod7_N49,
      I3 => CHOICE13118,
      O => CHOICE13120
    );
  mod7_n0047_0_96 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod7_v_state(0),
      I1 => CHOICE13026,
      I2 => CHOICE13028,
      I3 => CHOICE13044,
      O => CHOICE13046
    );
  mod7_n0046_2_19_G : LUT4
    generic map(
      INIT => X"A090"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(1),
      I2 => mod7_N4,
      I3 => mod0_pattern(0),
      O => N8561
    );
  mod5_hdframe_n0070_5_68 : LUT4
    generic map(
      INIT => X"FFA8"
    )
    port map (
      I0 => mod7_color_v0h2_y_2_Q,
      I1 => mod7_color_n0013,
      I2 => mod7_color_n0031,
      I3 => CHOICE15397,
      O => CHOICE15398
    );
  mod5_hdframe_hd_framegenerator_n0083_9_lut3 : LUT4
    generic map(
      INIT => X"0025"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(3),
      O => N7992
    );
  mod5_hdframe_hd_framegenerator_n0083_3_lut4 : LUT4
    generic map(
      INIT => X"FEBA"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(2),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(1),
      O => N7990
    );
  mod5_hdframe_hd_framegenerator_n0083_6_lut3 : LUT4
    generic map(
      INIT => X"0207"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(3),
      I3 => mod0_hd_format_tmp(0),
      O => N7989
    );
  mod7_n0046_3_276 : LUT4
    generic map(
      INIT => X"2722"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(9),
      I2 => mod7_active_line(6),
      I3 => mod7_active_line(5),
      O => CHOICE13494
    );
  mod5_hdframe_n0075_8_60 : LUT2_D
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_chroma_crc(17),
      LO => N8645,
      O => CHOICE15360
    );
  mod5_hdframe_n0070_7_101_SW0 : LUT4_L
    generic map(
      INIT => X"BAAA"
    )
    port map (
      I0 => mod7_color_N107,
      I1 => N1254,
      I2 => mod7_color_n0023,
      I3 => mod0_pattern(1),
      LO => N8259
    );
  mod7_n0047_3_356_SW0 : LUT3
    generic map(
      INIT => X"EF"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(3),
      I2 => mod7_active_line(7),
      O => N8291
    );
  mod7_n0034_2_145 : LUT4_L
    generic map(
      INIT => X"8F88"
    )
    port map (
      I0 => CHOICE14459,
      I1 => mod7_n0028,
      I2 => mod7_N154,
      I3 => mod7_n0029,
      LO => CHOICE14463
    );
  mod5_hdframe_n0075_2_12 : LUT4
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_color_v0h5_cr(0),
      I1 => mod7_color_N122,
      I2 => mod7_color_N134,
      I3 => mod7_color_n0012,
      O => CHOICE15251
    );
  mod7_n0044_3_1035 : LUT4
    generic map(
      INIT => X"EEEF"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(4),
      I3 => mod7_active_line(8),
      O => CHOICE13365
    );
  mod7_n0316_3_2 : LUT4
    generic map(
      INIT => X"F8B8"
    )
    port map (
      I0 => mod7_n0071,
      I1 => mod7_n0316(3),
      I2 => mod7_v_state(0),
      I3 => CHOICE12943,
      O => mod7_MUX_BLOCK_N4
    );
  mod7_counter_Ker034 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod7_counter_count(6),
      I1 => mod7_counter_count(7),
      I2 => mod7_counter_count(1),
      I3 => mod7_counter_count(2),
      O => CHOICE14038
    );
  mod7_n0046_3_380 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE13489,
      I1 => CHOICE13494,
      I2 => CHOICE13504,
      I3 => CHOICE13520,
      O => CHOICE13521
    );
  mod7_n0045_1_16 : LUT4
    generic map(
      INIT => X"2AAA"
    )
    port map (
      I0 => CHOICE12361,
      I1 => mod7_active_line(6),
      I2 => mod7_active_line(4),
      I3 => mod7_active_line(0),
      O => CHOICE12848
    );
  mod5_hdframe_linecount_6_rt1_72 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_linecount(6),
      O => mod5_hdframe_linecount_6_rt1
    );
  mod7_n0047_3_544_SW2 : LUT4
    generic map(
      INIT => X"FBFF"
    )
    port map (
      I0 => N8293,
      I1 => mod7_active_line(7),
      I2 => mod7_active_line(9),
      I3 => mod7_active_line(8),
      O => N8428
    );
  mod7_n0047_1_18_G : LUT4
    generic map(
      INIT => X"0220"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod7_active_line(6),
      I2 => mod0_pattern(1),
      I3 => mod0_pattern(0),
      O => N8563
    );
  mod7_n0034_0_115 : LUT4
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => CHOICE14114,
      I1 => N8131,
      I2 => mod7_v_state(2),
      I3 => mod7_n0023,
      O => CHOICE14116
    );
  mod7_n0047_3_34 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => mod7_N84,
      I1 => CHOICE12415,
      I2 => N8317,
      O => CHOICE13777
    );
  mod7_n0034_1_195 : LUT4
    generic map(
      INIT => X"5540"
    )
    port map (
      I0 => mod7_tp_21_Q,
      I1 => mod7_v_state(3),
      I2 => mod7_N54,
      I3 => CHOICE14207,
      O => CHOICE14210
    );
  mod7_n0044_3_1040 : LUT4
    generic map(
      INIT => X"020A"
    )
    port map (
      I0 => CHOICE13365,
      I1 => mod7_N278,
      I2 => mod7_n0150,
      I3 => mod7_active_line(3),
      O => CHOICE13366
    );
  mod0_bitptr_5_rt_73 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod0_bitptr(5),
      O => mod0_bitptr_5_rt
    );
  mod7_n0044_3_420 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => mod7_N253,
      I2 => mod7_active_line(4),
      I3 => mod7_N265,
      O => CHOICE13189
    );
  mod5_Mrom_n0003_inst_lut4_101 : LUT4
    generic map(
      INIT => X"1101"
    )
    port map (
      I0 => mod0_hd_format_tmp(1),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(2),
      I3 => mod0_hd_format_tmp(0),
      O => mod5_field2_vblnk1_5_Q
    );
  mod7_n0047_3_523_SW0 : LUT3
    generic map(
      INIT => X"EF"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(4),
      I2 => mod7_active_line(5),
      O => N8293
    );
  mod5_hdframe_n0075_0_20_SW0 : LUT3
    generic map(
      INIT => X"A8"
    )
    port map (
      I0 => mod7_color_N134,
      I1 => mod7_color_N122,
      I2 => mod7_color_N121,
      O => N8087
    );
  mod7_n0047_3_34_SW0 : LUT4
    generic map(
      INIT => X"EAC8"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(8),
      I2 => mod7_active_line(1),
      I3 => mod7_active_line(6),
      O => N8317
    );
  mod7_n0034_3_35 : LUT4
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_tp_21_Q,
      I1 => mod7_n0026,
      I2 => mod7_tp_17_Q,
      I3 => mod7_N199,
      O => CHOICE14342
    );
  mod7_n0044_3_1047 : LUT3
    generic map(
      INIT => X"23"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => N8187,
      I2 => N8433,
      O => CHOICE13367
    );
  mod7_n003515 : LUT4
    generic map(
      INIT => X"A2AA"
    )
    port map (
      I0 => mod7_N237,
      I1 => mod5_hdframe_sav,
      I2 => mod7_h_state_FFd3,
      I3 => mod7_tp_17_Q,
      O => CHOICE13963
    );
  mod7_n0034_1_186 : LUT3_L
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => mod7_n0025,
      I1 => mod7_tp_17_Q,
      I2 => mod7_n0032,
      LO => CHOICE14207
    );
  mod7_n0034_3_109 : LUT4_L
    generic map(
      INIT => X"44E4"
    )
    port map (
      I0 => mod7_v_state(2),
      I1 => mod7_n0030,
      I2 => mod7_n0026,
      I3 => mod7_tp_17_Q,
      LO => CHOICE14356
    );
  mod5_hdframe_Ker8912 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod5_hdframe_linecount(5),
      I1 => mod5_hdframe_linecount(8),
      I2 => mod5_hdframe_linecount(9),
      I3 => mod5_hdframe_linecount(1),
      O => CHOICE14547
    );
  mod7_n0034_6_183 : LUT3_L
    generic map(
      INIT => X"BA"
    )
    port map (
      I0 => N8233,
      I1 => N8452,
      I2 => mod7_h_state_FFd2,
      LO => CHOICE14535
    );
  mod7_n0046_2_58 : LUT3
    generic map(
      INIT => X"04"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(6),
      I2 => mod7_active_line(1),
      O => CHOICE12965
    );
  mod7_n0044_3_425 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_N251,
      I2 => mod7_active_line(0),
      I3 => mod7_N265,
      O => CHOICE13192
    );
  mod7_n0046_3_612_SW1 : LUT3
    generic map(
      INIT => X"DF"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(4),
      I2 => mod7_N231,
      O => N8470
    );
  mod7_n0034_1_182 : LUT4
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_tp_21_Q,
      I1 => mod7_n0030,
      I2 => mod7_N257,
      I3 => mod7_n0029,
      O => CHOICE14204
    );
  mod7_n0045_1_39 : LUT4
    generic map(
      INIT => X"FFEF"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => mod7_active_line(3),
      I2 => mod7_active_line(8),
      I3 => mod7_active_line(4),
      O => CHOICE12856
    );
  mod7_Ker931 : LUT4_D
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_h_state_FFd3,
      I2 => mod7_N237,
      I3 => mod7_v_state(3),
      LO => N8646,
      O => mod7_N93
    );
  mod7_n0046_3_310_G : LUT4
    generic map(
      INIT => X"15DD"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(4),
      I3 => mod7_active_line(6),
      O => N8565
    );
  mod5_hdframe_linecount_5_rt_74 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_linecount(5),
      O => mod5_hdframe_linecount_5_rt
    );
  mod5_hdframe_linecount_2_rt_75 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_linecount(2),
      O => mod5_hdframe_linecount_2_rt
    );
  mod7_n0044_0_15 : LUT4
    generic map(
      INIT => X"BF15"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(0),
      I2 => mod7_active_line(4),
      I3 => mod7_active_line(1),
      O => CHOICE13054
    );
  mod7_color_Ker331_SW0 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => mod7_color_v1h2_cb_6_Q,
      I1 => mod7_color_N115,
      I2 => mod7_color_N121,
      O => N8033
    );
  mod7_n0045_1_46 : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => CHOICE12856,
      I2 => mod7_N143,
      O => CHOICE12858
    );
  mod7_color_yramp_9_rt_76 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod7_color_yramp(9),
      O => mod7_color_yramp_9_rt
    );
  mod7_n0044_3_177 : LUT4
    generic map(
      INIT => X"EEAE"
    )
    port map (
      I0 => N8319,
      I1 => mod7_N250,
      I2 => mod7_active_line(3),
      I3 => mod7_active_line(2),
      O => CHOICE13128
    );
  mod5_hdframe_n0075_5_77 : LUT4_L
    generic map(
      INIT => X"4000"
    )
    port map (
      I0 => mod0_tp_option(0),
      I1 => mod0_tp_option(1),
      I2 => mod7_color_v1h9_y(5),
      I3 => mod7_color_n0018,
      LO => CHOICE15220
    );
  mod7_n0047_3_192_SW0_G : LUT4
    generic map(
      INIT => X"EEEF"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(2),
      I3 => mod7_active_line(6),
      O => N8567
    );
  mod5_hdframe_n0070_9_217 : LUT4_D
    generic map(
      INIT => X"FF5D"
    )
    port map (
      I0 => mod5_hdframe_n0107,
      I1 => mod5_hdframe_n0114,
      I2 => mod5_hdframe_linecount(6),
      I3 => mod5_hdframe_n0112,
      LO => N8647,
      O => CHOICE15103
    );
  mod7_n0045_1_54 : LUT4
    generic map(
      INIT => X"F5F4"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(3),
      I2 => CHOICE12858,
      I3 => mod7_active_line(0),
      O => CHOICE12859
    );
  mod7_n0044_3_198_SW1 : LUT4
    generic map(
      INIT => X"F3F1"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_N217,
      I2 => CHOICE13120,
      I3 => mod7_active_line(2),
      O => N8445
    );
  mod7_n0046_3_39 : LUT4
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => mod7_N35,
      I1 => mod7_N61,
      I2 => mod7_active_line(5),
      I3 => mod7_N255,
      O => CHOICE13439
    );
  mod7_n0046_3_580 : LUT4
    generic map(
      INIT => X"2030"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_n0257,
      I2 => CHOICE13570,
      I3 => mod7_N255,
      O => CHOICE13571
    );
  mod7_n0046_2_71 : LUT3
    generic map(
      INIT => X"A8"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(4),
      O => CHOICE12970
    );
  mod7_n0047_3_50_SW1 : LUT4
    generic map(
      INIT => X"4FEE"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(9),
      I2 => mod7_active_line(2),
      I3 => mod7_active_line(4),
      O => N8341
    );
  mod7_n0034_1_147 : LUT4
    generic map(
      INIT => X"FFF4"
    )
    port map (
      I0 => mod7_tp_21_Q,
      I1 => mod7_N181,
      I2 => CHOICE14195,
      I3 => CHOICE14187,
      O => CHOICE14199
    );
  mod7_n0046_3_533_SW0 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => N8181,
      I1 => CHOICE13439,
      I2 => CHOICE13430,
      I3 => N8345,
      O => N8119
    );
  mod7_n0045_1_78 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod7_v_state(1),
      I1 => CHOICE12848,
      I2 => mod7_active_line(5),
      I3 => CHOICE12859,
      O => CHOICE12861
    );
  mod7_n0044_3_198 : LUT4
    generic map(
      INIT => X"FEBA"
    )
    port map (
      I0 => CHOICE13128,
      I1 => mod7_active_line(1),
      I2 => N8445,
      I3 => N8446,
      O => CHOICE13130
    );
  mod7_n0046_2_35 : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(7),
      I2 => mod7_N105,
      O => CHOICE12958
    );
  mod7_n0044_3_637_SW0 : LUT3
    generic map(
      INIT => X"DF"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(5),
      O => N8321
    );
  mod7_n0034_0_235_SW0 : LUT4_L
    generic map(
      INIT => X"FFA8"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => CHOICE14098,
      I2 => CHOICE14101,
      I3 => CHOICE14095,
      LO => N8125
    );
  mod7_n0044_3_1078 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => CHOICE13367,
      I1 => CHOICE13340,
      I2 => CHOICE13327,
      I3 => N8221,
      O => CHOICE13369
    );
  mod7_n0044_3_814 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => CHOICE13295,
      I1 => mod7_n0065,
      I2 => CHOICE13281,
      I3 => mod7_n0066,
      O => CHOICE13296
    );
  mod7_n0045_1_151_SW0_SW0 : LUT4
    generic map(
      INIT => X"BFFF"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(5),
      I2 => mod7_tp_17_Q,
      I3 => mod7_N251,
      O => N8360
    );
  mod7_counter_Ker012 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod7_counter_count(3),
      I1 => mod7_counter_count(8),
      I2 => mod7_counter_count(9),
      I3 => mod7_counter_count(10),
      O => CHOICE14026
    );
  mod7_n0046_2_80 : LUT4
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => CHOICE12965,
      I1 => CHOICE12971,
      I2 => mod7_active_line(9),
      I3 => mod7_active_line(4),
      O => CHOICE12972
    );
  mod7_Ker298_G : LUT3
    generic map(
      INIT => X"A8"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(2),
      O => N8569
    );
  mod7_n0044_3_494_SW0 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE13094,
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(2),
      I3 => N8347,
      O => N8225
    );
  mod7_n0046_3_972_SW1 : LUT3
    generic map(
      INIT => X"BA"
    )
    port map (
      I0 => N8185,
      I1 => mod7_active_line(5),
      I2 => N8494,
      O => N8476
    );
  mod7_n0044_3_637 : LUT4
    generic map(
      INIT => X"A2AA"
    )
    port map (
      I0 => mod7_N234,
      I1 => mod7_active_line(8),
      I2 => mod7_active_line(0),
      I3 => N8321,
      O => CHOICE13251
    );
  mod7_n0045_0_79 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(0),
      I2 => mod7_active_line(4),
      I3 => mod7_active_line(6),
      O => CHOICE13397
    );
  mod7_n0034_1_129 : LUT4
    generic map(
      INIT => X"2AA2"
    )
    port map (
      I0 => mod7_n0023,
      I1 => mod7_tp_19_Q,
      I2 => mod7_v_state(2),
      I3 => mod7_tp_21_Q,
      O => CHOICE14195
    );
  mod7_n0044_3_1092 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => CHOICE13273,
      I1 => CHOICE13369,
      O => CHOICE13370
    );
  mod7_n0046_3_351 : LUT4
    generic map(
      INIT => X"8F88"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_N255,
      I2 => mod7_active_line(1),
      I3 => mod7_active_line(8),
      O => CHOICE13515
    );
  mod7_Ker2449 : LUT4
    generic map(
      INIT => X"5755"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod0_pattern(0),
      I2 => mod0_pattern(1),
      I3 => N8488,
      O => CHOICE13419
    );
  mod7_n0045_1_151_SW0_SW1 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(5),
      I3 => N8488,
      O => N8361
    );
  mod7_n0034_7_21 : LUT4_L
    generic map(
      INIT => X"EEAE"
    )
    port map (
      I0 => mod7_n0024,
      I1 => mod7_n0025,
      I2 => mod7_N257,
      I3 => mod7_tp_21_Q,
      LO => CHOICE14222
    );
  mod5_hdframe_n0070_6_400 : LUT4_L
    generic map(
      INIT => X"FEEE"
    )
    port map (
      I0 => mod7_v_state(1),
      I1 => N8257,
      I2 => mod7_tp_0_Q,
      I3 => mod7_tp_1_Q,
      LO => CHOICE15004
    );
  mod7_n0047_3_501 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_N241,
      I1 => mod7_N300,
      I2 => mod7_tp_17_Q,
      I3 => mod7_N84,
      O => CHOICE13898
    );
  mod7_n0045_0_82 : LUT3
    generic map(
      INIT => X"BA"
    )
    port map (
      I0 => CHOICE13397,
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(1),
      O => CHOICE13398
    );
  mod5_hdframe_samplecount_2_rt_77 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(2),
      LO => mod5_hdframe_samplecount_2_rt
    );
  mod7_n0044_0_35_SW0 : LUT4
    generic map(
      INIT => X"3111"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod7_active_line(6),
      I2 => mod0_pattern(1),
      I3 => mod0_pattern(0),
      O => N8331
    );
  mod7_n0044_3_450_SW1 : LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(4),
      O => N8415
    );
  mod7_n0046_2_74 : LUT4
    generic map(
      INIT => X"F5F4"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(1),
      I2 => CHOICE12970,
      I3 => mod7_active_line(2),
      O => CHOICE12971
    );
  mod7_n0046_3_492 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => CHOICE13541,
      I1 => CHOICE13545,
      I2 => CHOICE13554,
      O => CHOICE13555
    );
  mod7_n0034_0_56 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod7_v_state(3),
      I1 => mod7_n0023,
      I2 => mod7_n0024,
      I3 => mod7_n0028,
      O => CHOICE14098
    );
  mod7_n0044_0_35 : LUT4
    generic map(
      INIT => X"F5F4"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(0),
      I2 => N8331,
      I3 => mod7_active_line(4),
      O => CHOICE13065
    );
  mod5_hdframe_n0070_4_224_SW0 : LUT4
    generic map(
      INIT => X"B800"
    )
    port map (
      I0 => mod5_hdframe_linecount(9),
      I1 => mod5_hdframe_n0115,
      I2 => CHOICE15301,
      I3 => mod5_hdframe_N871,
      O => N8061
    );
  mod7_n0044_3_957 : LUT3
    generic map(
      INIT => X"15"
    )
    port map (
      I0 => N8191,
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(2),
      O => CHOICE13340
    );
  mod7_h_state_FFd3_In69 : LUT4
    generic map(
      INIT => X"0E04"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => N8159,
      I2 => mod7_h_state_FFd4,
      I3 => mod7_h_state_FFd3,
      O => CHOICE13954
    );
  mod7_n0044_3_231 : LUT4
    generic map(
      INIT => X"2030"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(7),
      I2 => mod7_active_line(0),
      I3 => mod7_active_line(1),
      O => CHOICE13135
    );
  mod0_bitptr_3_rt_78 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod0_bitptr(3),
      O => mod0_bitptr_3_rt
    );
  mod5_hdframe_n0075_6_67 : LUT4_L
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE14775,
      I1 => mod7_color_N28,
      I2 => mod7_color_N102,
      I3 => mod7_color_N101,
      LO => CHOICE14778
    );
  mod7_n0044_3_800_G : LUT3
    generic map(
      INIT => X"BA"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(3),
      O => N8571
    );
  mod7_n0044_3_450 : LUT4
    generic map(
      INIT => X"EC22"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(6),
      I2 => N8415,
      I3 => mod7_active_line(0),
      O => CHOICE13201
    );
  mod7_n005245 : LUT2_D
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => CHOICE14005,
      I1 => CHOICE14018,
      LO => N8648,
      O => mod7_n0052
    );
  mod7_n0034_0_41 : LUT4
    generic map(
      INIT => X"F5F4"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_N118,
      I2 => CHOICE14091,
      I3 => CHOICE14872,
      O => CHOICE14095
    );
  mod7_n0046_3_158_SW0 : LUT4
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => CHOICE13465,
      I1 => CHOICE13451,
      I2 => mod7_active_line(1),
      I3 => mod7_active_line(3),
      O => N8181
    );
  mod7_n005239 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => CHOICE14017,
      I1 => mod7_counter_count(9),
      I2 => mod7_counter_count(2),
      I3 => mod7_counter_count(3),
      O => CHOICE14018
    );
  mod7_n0034_3_133 : LUT4_L
    generic map(
      INIT => X"8F88"
    )
    port map (
      I0 => CHOICE14459,
      I1 => mod7_N51,
      I2 => mod7_tp_19_Q,
      I3 => mod7_n0029,
      LO => CHOICE14363
    );
  mod7_n0044_3_456 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => CHOICE13189,
      I1 => CHOICE13192,
      I2 => CHOICE13201,
      O => CHOICE13202
    );
  mod7_n0046_3_461 : LUT4
    generic map(
      INIT => X"2030"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(4),
      I3 => mod7_active_line(7),
      O => CHOICE13545
    );
  mod7_n0044_3_177_SW0 : LUT4
    generic map(
      INIT => X"1110"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(8),
      I3 => mod7_active_line(0),
      O => N8319
    );
  mod5_hdframe_linecount_4_rt_79 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_linecount(4),
      O => mod5_hdframe_linecount_4_rt
    );
  mod7_n0045_1_151_SW0 : LUT4
    generic map(
      INIT => X"0819"
    )
    port map (
      I0 => mod7_active_line(0),
      I1 => mod7_active_line(4),
      I2 => N8361,
      I3 => N8360,
      O => N8223
    );
  mod7_n0044_0_53 : LUT4
    generic map(
      INIT => X"3233"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(7),
      I2 => CHOICE13065,
      I3 => mod7_active_line(5),
      O => CHOICE13068
    );
  mod7_n005234 : LUT4_L
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => mod7_counter_count(4),
      I1 => mod7_counter_count(5),
      I2 => mod7_counter_count(10),
      I3 => mod7_counter_count(0),
      LO => CHOICE14017
    );
  mod7_n0034_1_102 : LUT4_L
    generic map(
      INIT => X"CC40"
    )
    port map (
      I0 => mod7_N154,
      I1 => mod7_tp_21_Q,
      I2 => mod7_n0026,
      I3 => mod7_N225,
      LO => CHOICE14187
    );
  mod7_n0046_2_140_SW0 : LUT4
    generic map(
      INIT => X"BFFF"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => mod7_active_line(6),
      I2 => mod7_tp_17_Q,
      I3 => mod7_N302,
      O => N8333
    );
  mod5_hdframe_n0075_0_13 : LUT3
    generic map(
      INIT => X"A8"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod7_color_N117,
      I2 => CHOICE15028,
      O => CHOICE15030
    );
  mod7_n0046_3_575 : LUT4
    generic map(
      INIT => X"45EF"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(3),
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(8),
      O => CHOICE13570
    );
  mod7_n0045_1_151 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => N8223,
      I1 => mod7_active_line(3),
      I2 => mod7_active_line(8),
      I3 => mod7_active_line(7),
      O => CHOICE12879
    );
  mod5_hdframe_samplecount_1_rt_80 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(1),
      O => mod5_hdframe_samplecount_1_rt
    );
  mod7_n0044_3_242 : LUT4
    generic map(
      INIT => X"4F44"
    )
    port map (
      I0 => mod7_N75,
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(3),
      I3 => mod7_N263,
      O => CHOICE13140
    );
  mod5_hdframe_hd_framegenerator_n0083_10_lut3 : LUT3
    generic map(
      INIT => X"15"
    )
    port map (
      I0 => mod0_hd_format_tmp(3),
      I1 => mod0_hd_format_tmp(1),
      I2 => mod0_hd_format_tmp(2),
      O => N7987
    );
  mod7_n0034_5_43 : LUT4
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_h_state_FFd3,
      I2 => mod7_N237,
      I3 => mod7_n0026,
      O => CHOICE14396
    );
  mod7_n0044_0_63 : LUT4
    generic map(
      INIT => X"EEAE"
    )
    port map (
      I0 => CHOICE13068,
      I1 => mod7_active_line(7),
      I2 => mod7_active_line(2),
      I3 => mod7_active_line(1),
      O => CHOICE13069
    );
  mod5_hdframe_n0070_9_221_SW0 : LUT4
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => mod5_hdframe_n0094,
      I1 => mod5_hdframe_n0095,
      I2 => mod5_hdframe_n0132,
      I3 => N8647,
      O => N8038
    );
  mod7_n0044_3_469 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE13172,
      I1 => CHOICE13177,
      I2 => CHOICE13185,
      I3 => CHOICE13202,
      O => CHOICE13203
    );
  mod7_n0046_3_620_SW0 : LUT4
    generic map(
      INIT => X"FF5D"
    )
    port map (
      I0 => mod7_N234,
      I1 => mod7_active_line(9),
      I2 => mod7_active_line(3),
      I3 => mod7_N61,
      O => N8492
    );
  mod5_hdframe_n0070_6_482 : LUT4
    generic map(
      INIT => X"FF27"
    )
    port map (
      I0 => CHOICE14995,
      I1 => N8079,
      I2 => N8078,
      I3 => CHOICE14984,
      O => CHOICE15015
    );
  mod5_hdframe_n0070_6_134 : LUT4_D
    generic map(
      INIT => X"0F01"
    )
    port map (
      I0 => mod7_v_state(0),
      I1 => mod7_v_state(2),
      I2 => mod7_tp_1_Q,
      I3 => mod7_v_state(1),
      LO => N8649,
      O => CHOICE14944
    );
  mod5_hdframe_n0075_0_25 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod7_color_v0h5_cr(0),
      I1 => N8087,
      I2 => N8624,
      I3 => mod7_color_n0014,
      O => CHOICE15035
    );
  mod7_n0046_2_170_SW0 : LUT4
    generic map(
      INIT => X"FEEE"
    )
    port map (
      I0 => CHOICE12958,
      I1 => CHOICE12972,
      I2 => mod7_active_line(9),
      I3 => CHOICE12955,
      O => N8161
    );
  mod7_n0047_3_363_SW0 : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => mod7_active_line(8),
      I1 => N8291,
      I2 => N8417,
      O => N8217
    );
  mod7_n0044_3_705_SW0 : LUT4
    generic map(
      INIT => X"CF4F"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_N256,
      I2 => CHOICE13270,
      I3 => mod7_active_line(1),
      O => N8163
    );
  mod7_n0044_3_494 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE13203,
      I1 => CHOICE13168,
      I2 => CHOICE13130,
      I3 => N8225,
      O => CHOICE13205
    );
  mod7_n0046_3_794 : LUT4
    generic map(
      INIT => X"0F01"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(5),
      I2 => N8227,
      I3 => mod7_active_line(9),
      O => CHOICE13629
    );
  mod5_hdframe_n0070_0_0 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_luma_crc(10),
      O => CHOICE14658
    );
  mod7_n0046_3_505 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE13525,
      I1 => CHOICE13527,
      I2 => CHOICE13537,
      I3 => CHOICE13555,
      O => CHOICE13556
    );
  mod7_n0034_0_14 : LUT4
    generic map(
      INIT => X"44E4"
    )
    port map (
      I0 => mod7_v_state(2),
      I1 => mod7_n0025,
      I2 => N8588,
      I3 => mod7_tp_17_Q,
      O => CHOICE14088
    );
  mod7_n0046_3_794_SW0 : LUT4
    generic map(
      INIT => X"FBFF"
    )
    port map (
      I0 => mod7_N132,
      I1 => CHOICE13628,
      I2 => mod7_n0075,
      I3 => CHOICE13604,
      O => N8227
    );
  mod5_hdframe_n0070_8_59 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_n0177(1),
      I2 => mod5_hdframe_n0179(0),
      I3 => mod5_hdframe_n0171(0),
      O => CHOICE14572
    );
  mod7_n0034_6_31 : LUT4
    generic map(
      INIT => X"DC50"
    )
    port map (
      I0 => mod7_v_state(3),
      I1 => mod7_load_val(6),
      I2 => mod7_n0022,
      I3 => N8595,
      O => CHOICE14495
    );
  mod5_hdframe_n0075_7_124 : LUT4_L
    generic map(
      INIT => X"5444"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => CHOICE14718,
      I2 => mod5_hdframe_N881,
      I3 => CHOICE14728,
      LO => CHOICE14732
    );
  mod7_n0034_2_46_SW0 : LUT4_L
    generic map(
      INIT => X"1101"
    )
    port map (
      I0 => CHOICE14434,
      I1 => mod7_n0025,
      I2 => mod7_n0024,
      I3 => mod7_tp_21_Q,
      LO => N8129
    );
  mod5_hdframe_n0070_8_60 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_luma_crc(17),
      O => CHOICE14573
    );
  mod5_hdframe_samplecount_8_rt2_81 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(8),
      O => mod5_hdframe_samplecount_8_rt2
    );
  mod5_hdframe_samplecount_10_rt2_82 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(10),
      O => mod5_hdframe_samplecount_10_rt2
    );
  mod5_hdframe_n0075_2_28 : LUT4
    generic map(
      INIT => X"5455"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => mod7_color_N28,
      I2 => CHOICE15251,
      I3 => N8133,
      O => CHOICE15256
    );
  mod5_hdframe_n0075_9_216 : LUT4
    generic map(
      INIT => X"F8FC"
    )
    port map (
      I0 => mod7_color_N55,
      I1 => CHOICE14852,
      I2 => CHOICE14849,
      I3 => mod7_tp_0_Q,
      O => CHOICE14854
    );
  mod5_hdframe_n0075_9_246_SW0 : LUT4_L
    generic map(
      INIT => X"2A7F"
    )
    port map (
      I0 => mod7_v_state(1),
      I1 => mod0_pattern(0),
      I2 => mod7_h_state_FFd3_5,
      I3 => mod7_v_state(0),
      LO => N8383
    );
  mod5_hdframe_n0070_4_152 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE15313,
      I1 => CHOICE15322,
      I2 => CHOICE15311,
      I3 => mod7_color_N43,
      O => CHOICE15325
    );
  mod5_hdframe_n0070_6_333_SW0 : LUT4_L
    generic map(
      INIT => X"FEEE"
    )
    port map (
      I0 => CHOICE14973,
      I1 => mod5_hdframe_n0135,
      I2 => CHOICE14981,
      I3 => mod7_h_state_FFd2,
      LO => N8145
    );
  mod7_n0034_5_68 : LUT4
    generic map(
      INIT => X"F5F4"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => CHOICE14399,
      I2 => CHOICE14394,
      I3 => CHOICE14396,
      O => CHOICE14403
    );
  mod7_n0034_4_9 : LUT4
    generic map(
      INIT => X"A8AA"
    )
    port map (
      I0 => mod7_tp_21_Q,
      I1 => mod7_N292,
      I2 => mod7_N264,
      I3 => mod7_n0086,
      O => CHOICE14291
    );
  mod5_hdframe_n0070_0_135 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => N8091,
      I1 => mod7_color_N43,
      I2 => CHOICE14693,
      I3 => CHOICE14678,
      O => CHOICE14695
    );
  mod5_hdframe_n0070_3_17 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_luma_out(0),
      I2 => mod5_hdframe_luma_crc(0),
      I3 => mod5_hdframe_luma_crc(13),
      O => CHOICE15137
    );
  mod5_hdframe_n0075_9_345 : LUT4_L
    generic map(
      INIT => X"FFEC"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => CHOICE14828,
      I2 => CHOICE14875,
      I3 => CHOICE14827,
      LO => CHOICE14878
    );
  mod5_hdframe_n0070_8_65 : LUT4
    generic map(
      INIT => X"ECA0"
    )
    port map (
      I0 => mod7_color_v0h2_y_2_Q,
      I1 => mod7_tp_19_Q,
      I2 => mod7_color_n0014,
      I3 => mod7_color_n0037,
      O => CHOICE14576
    );
  mod7_n0034_7_93 : LUT4_L
    generic map(
      INIT => X"9F00"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_pattern(0),
      I2 => mod0_pattern(2),
      I3 => CHOICE14234,
      LO => CHOICE14236
    );
  mod7_n0034_6_47 : LUT4
    generic map(
      INIT => X"40C0"
    )
    port map (
      I0 => mod7_h_state_FFd4,
      I1 => N8371,
      I2 => mod7_h_state_FFd2,
      I3 => mod7_h_state_FFd3,
      O => CHOICE14500
    );
  mod5_hdframe_samplecount_10_rt1_83 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(10),
      O => mod5_hdframe_samplecount_10_rt1
    );
  mod5_hdframe_n0070_5_79 : LUT4
    generic map(
      INIT => X"FF04"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => mod7_color_N132,
      I2 => mod7_tp_1_Q,
      I3 => N8247,
      O => CHOICE15402
    );
  mod5_hdframe_n0070_3_78 : LUT4_L
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE15151,
      I1 => CHOICE15152,
      I2 => mod7_color_N40,
      I3 => CHOICE15150,
      LO => CHOICE15155
    );
  mod5_hdframe_n0070_5_79_SW0 : LUT4_L
    generic map(
      INIT => X"F080"
    )
    port map (
      I0 => mod7_color_v0h5_cr(0),
      I1 => mod7_color_N134,
      I2 => mod7_color_N121,
      I3 => mod7_color_N137,
      LO => N8247
    );
  mod5_hdframe_samplecount_3_rt_84 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(3),
      O => mod5_hdframe_samplecount_3_rt
    );
  mod7_h_state_FFd4_In43 : LUT4
    generic map(
      INIT => X"C080"
    )
    port map (
      I0 => CHOICE13976,
      I1 => CHOICE14018,
      I2 => CHOICE14005,
      I3 => CHOICE13984,
      O => CHOICE13986
    );
  mod5_hdframe_n0075_4_121 : LUT4_L
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE15479,
      I1 => mod5_hdframe_N881,
      I2 => CHOICE15485,
      I3 => CHOICE15490,
      LO => CHOICE15493
    );
  mod5_hdframe_n0070_6_305 : LUT4
    generic map(
      INIT => X"EAC0"
    )
    port map (
      I0 => mod7_tp_4_Q,
      I1 => CHOICE14979,
      I2 => mod7_color_N125,
      I3 => mod7_tp_1_Q,
      O => CHOICE14981
    );
  mod5_hdframe_n0070_9_5 : LUT4_L
    generic map(
      INIT => X"F8F0"
    )
    port map (
      I0 => mod7_color_v1h4_y(9),
      I1 => mod0_pattern(1),
      I2 => mod7_color_N26,
      I3 => mod7_color_N57,
      LO => CHOICE15061
    );
  mod5_hdframe_n0075_6_160 : LUT4_L
    generic map(
      INIT => X"3335"
    )
    port map (
      I0 => N8026,
      I1 => N8027,
      I2 => CHOICE14792,
      I3 => CHOICE14781,
      LO => CHOICE14797
    );
  mod5_hdframe_samplecount_7_rt_85 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(7),
      O => mod5_hdframe_samplecount_7_rt
    );
  mod7_n0034_4_42_SW0 : LUT4_L
    generic map(
      INIT => X"3111"
    )
    port map (
      I0 => mod7_n0024,
      I1 => mod7_n0025,
      I2 => mod7_tp_21_Q,
      I3 => mod7_N257,
      LO => N8151
    );
  mod7_n0034_6_99 : LUT4_L
    generic map(
      INIT => X"7770"
    )
    port map (
      I0 => sif_hd_format(2),
      I1 => sif_hd_format(1),
      I2 => CHOICE14506,
      I3 => CHOICE14502,
      LO => CHOICE14509
    );
  mod7_n0034_6_64 : LUT4
    generic map(
      INIT => X"FFEC"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => CHOICE14500,
      I2 => CHOICE14872,
      I3 => N8149,
      O => CHOICE14502
    );
  mod6_scram20_inst_Mxor_n0015_Result1 : LUT4_L
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_n0161(0),
      I1 => mod6_scram20_inst_S(12),
      I2 => mod6_scram20_inst_S(11),
      I3 => mod6_scram20_inst_S(10),
      LO => mod6_scram20_inst_n0148(0)
    );
  mod5_hdframe_n0075_5_0 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0115,
      I1 => mod5_hdframe_linecount(10),
      O => CHOICE15197
    );
  mod5_hdframe_n00991 : LUT2_D
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_n0101,
      I1 => mod5_hdframe_nor_cyo4,
      LO => N8650,
      O => mod5_hdframe_n0099
    );
  mod5_hdframe_n0075_9_223 : LUT4_L
    generic map(
      INIT => X"FFE0"
    )
    port map (
      I0 => CHOICE14843,
      I1 => mod7_tp_0_Q,
      I2 => mod7_tp_1_Q,
      I3 => CHOICE14854,
      LO => CHOICE14855
    );
  mod7_n0034_4_12 : LUT4
    generic map(
      INIT => X"FF80"
    )
    port map (
      I0 => mod7_N10,
      I1 => mod7_h_state_FFd2,
      I2 => mod7_h_state_FFd3,
      I3 => mod7_N186,
      O => CHOICE14293
    );
  mod7_color_Ker45_SW0 : LUT4_L
    generic map(
      INIT => X"FFA8"
    )
    port map (
      I0 => mod7_color_v0h5_cr(0),
      I1 => mod7_color_n0015,
      I2 => mod7_color_n0014,
      I3 => mod7_color_N98,
      LO => N8045
    );
  mod5_hdframe_n0075_9_144 : LUT4
    generic map(
      INIT => X"5755"
    )
    port map (
      I0 => N8400,
      I1 => mod7_v_state(2),
      I2 => mod7_v_state(1),
      I3 => mod7_v_state(0),
      O => CHOICE14842
    );
  mod5_hdframe_n0075_8_114_SW0 : LUT4
    generic map(
      INIT => X"F400"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => CHOICE15360,
      I2 => N8629,
      I3 => mod5_hdframe_N901,
      O => N8064
    );
  mod7_n0034_2_112_SW0_G : LUT2_L
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod7_h_state_FFd4,
      I1 => mod7_h_state_FFd2,
      LO => N8573
    );
  mod5_hdframe_n0075_4_93 : LUT4
    generic map(
      INIT => X"5545"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => CHOICE15486,
      I2 => N8041,
      I3 => mod7_color_N61,
      O => CHOICE15490
    );
  mod5_hdframe_n0070_7_148_SW0 : LUT4
    generic map(
      INIT => X"FF08"
    )
    port map (
      I0 => mod5_hdframe_luma_crc(16),
      I1 => mod5_hdframe_n0100,
      I2 => mod5_hdframe_n0116,
      I3 => N8640,
      O => N8035
    );
  mod5_hdframe_n0070_1_28 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => mod7_color_N80,
      I1 => CHOICE14743,
      I2 => CHOICE14747,
      I3 => N8610,
      O => CHOICE14749
    );
  mod7_color_Ker591 : LUT4_D
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => mod7_tp_0_Q,
      I1 => mod7_tp_4_Q,
      I2 => mod7_color_v1h9_y(5),
      I3 => mod7_color_N123,
      LO => N8651,
      O => mod7_color_N59
    );
  mod5_hdframe_n0070_4_121 : LUT4_L
    generic map(
      INIT => X"FFEC"
    )
    port map (
      I0 => CHOICE15316,
      I1 => CHOICE15320,
      I2 => mod7_color_n0039,
      I3 => mod7_color_N96,
      LO => CHOICE15322
    );
  mod5_hdframe_n0070_6_367 : LUT4_L
    generic map(
      INIT => X"FFF4"
    )
    port map (
      I0 => mod7_color_N19,
      I1 => mod7_color_N47,
      I2 => CHOICE14990,
      I3 => CHOICE14988,
      LO => CHOICE14994
    );
  mod5_hdframe_n0070_4_111 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => mod7_color_N120,
      I1 => N8652,
      I2 => mod7_color_n0013,
      I3 => mod7_color_v0h5_cr(0),
      O => CHOICE15320
    );
  mod7_color_Ker4016 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => mod7_color_N80,
      I1 => CHOICE15109,
      I2 => CHOICE15112,
      O => mod7_color_N40
    );
  mod5_hdframe_n0070_8_77 : LUT4_L
    generic map(
      INIT => X"C8C0"
    )
    port map (
      I0 => mod7_color_v1h9_y(5),
      I1 => mod7_color_yramp(8),
      I2 => mod7_color_N26,
      I3 => mod7_color_N57,
      LO => CHOICE14581
    );
  mod7_n0034_4_42 : LUT4
    generic map(
      INIT => X"3233"
    )
    port map (
      I0 => CHOICE14300,
      I1 => mod7_tp_17_Q,
      I2 => mod7_n0026,
      I3 => N8151,
      O => CHOICE14304
    );
  mod5_hdframe_n0070_7_92_SW0 : LUT4_L
    generic map(
      INIT => X"8880"
    )
    port map (
      I0 => mod7_color_v1h9_y(5),
      I1 => mod7_color_yramp(7),
      I2 => mod7_color_n0041,
      I3 => mod7_color_n0039,
      LO => N8147
    );
  mod5_hdframe_n0075_8_84 : LUT4_L
    generic map(
      INIT => X"FFE4"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => CHOICE14775,
      I2 => CHOICE15362,
      I3 => mod7_color_N29,
      LO => CHOICE15369
    );
  mod5_hdframe_n0075_4_163_SW1_SW0 : LUT4_L
    generic map(
      INIT => X"2882"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_chroma_crc(14),
      I2 => mod5_hdframe_n0190(0),
      I3 => mod5_hdframe_n0193(0),
      LO => N8500
    );
  mod5_hdframe_n0070_6_428 : LUT4_L
    generic map(
      INIT => X"FFDC"
    )
    port map (
      I0 => mod7_tp_1_Q,
      I1 => mod7_color_N135,
      I2 => mod7_color_N140,
      I3 => N8093,
      LO => CHOICE15010
    );
  mod5_hdframe_n0070_5_60 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_luma_crc(15),
      I2 => mod5_hdframe_n0177(0),
      I3 => mod5_hdframe_n0175(0),
      LO => CHOICE15392
    );
  mod7_n0034_6_83 : LUT4
    generic map(
      INIT => X"F5F4"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_N118,
      I2 => mod7_N93,
      I3 => CHOICE14872,
      O => CHOICE14506
    );
  mod7_Ker2814 : LUT4
    generic map(
      INIT => X"ECA0"
    )
    port map (
      I0 => mod7_v_state(3),
      I1 => mod7_N257,
      I2 => N8600,
      I3 => mod7_n0030,
      O => CHOICE14156
    );
  mod5_hdframe_n0070_3_68 : LUT4
    generic map(
      INIT => X"FF80"
    )
    port map (
      I0 => mod0_tp_option(2),
      I1 => mod7_color_v1h9_y(5),
      I2 => mod7_color_n0023,
      I3 => N8609,
      O => CHOICE15152
    );
  mod5_hdframe_n0075_4_79 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => CHOICE15482,
      I2 => mod7_color_N81,
      I3 => mod7_color_N24,
      O => CHOICE15485
    );
  mod7_n0034_2_38 : LUT4
    generic map(
      INIT => X"FEEE"
    )
    port map (
      I0 => mod7_N190,
      I1 => mod7_N120,
      I2 => mod7_n0028,
      I3 => mod7_tp_21_Q,
      O => CHOICE14440
    );
  mod5_hdframe_n0075_2_8 : LUT4_L
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod5_hdframe_samplecount(0),
      I1 => mod7_color_N81,
      I2 => mod7_color_N39,
      I3 => mod7_color_N24,
      LO => CHOICE15248
    );
  mod7_n0034_1_209 : LUT4_L
    generic map(
      INIT => X"EFEE"
    )
    port map (
      I0 => CHOICE14204,
      I1 => CHOICE14210,
      I2 => mod7_tp_17_Q,
      I3 => CHOICE14199,
      LO => CHOICE14212
    );
  mod5_hdframe_n0075_1_10 : LUT4
    generic map(
      INIT => X"FFF8"
    )
    port map (
      I0 => mod7_color_v1h2_cb_6_Q,
      I1 => mod7_color_n0018,
      I2 => N8626,
      I3 => mod7_color_N39,
      O => CHOICE14597
    );
  mod5_hdframe_n0070_6_350 : LUT4
    generic map(
      INIT => X"FFB0"
    )
    port map (
      I0 => mod0_tp_option(1),
      I1 => mod0_tp_option(0),
      I2 => mod7_tp_4_Q,
      I3 => mod7_color_N140,
      O => CHOICE14988
    );
  mod7_n0034_5_135 : LUT4
    generic map(
      INIT => X"88A8"
    )
    port map (
      I0 => mod7_load_val(5),
      I1 => CHOICE14406,
      I2 => CHOICE14411,
      I3 => mod7_tp_17_Q,
      O => CHOICE14415
    );
  mod5_hdframe_samplecount_5_rt_86 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(5),
      O => mod5_hdframe_samplecount_5_rt
    );
  mod7_n0047_3_290 : LUT4
    generic map(
      INIT => X"C405"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(3),
      I3 => mod7_active_line(4),
      O => CHOICE13838
    );
  mod7_n0034_7_10 : LUT4
    generic map(
      INIT => X"4F4C"
    )
    port map (
      I0 => mod7_v_state(2),
      I1 => mod7_N72,
      I2 => mod7_tp_19_Q,
      I3 => mod7_n0028,
      O => CHOICE14218
    );
  mod5_hdframe_n0070_7_84 : LUT4
    generic map(
      INIT => X"2820"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_pattern(2),
      I2 => mod0_pattern(0),
      I3 => mod7_color_yramp(7),
      O => CHOICE14643
    );
  mod5_hdframe_n0070_7_92 : LUT4
    generic map(
      INIT => X"FFA8"
    )
    port map (
      I0 => CHOICE14643,
      I1 => mod7_color_n0037,
      I2 => mod7_color_n0024,
      I3 => N8147,
      O => CHOICE14645
    );
  mod7_n0034_6_47_SW0 : LUT4_L
    generic map(
      INIT => X"FEEE"
    )
    port map (
      I0 => mod7_load_val(6),
      I1 => mod7_tp_17_Q,
      I2 => mod7_v_state(2),
      I3 => mod7_tp_19_Q,
      LO => N8371
    );
  mod5_hdframe_n0070_0_171 : LUT4_L
    generic map(
      INIT => X"5444"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => CHOICE14659,
      I2 => mod5_hdframe_N881,
      I3 => CHOICE14695,
      LO => CHOICE14699
    );
  mod5_hdframe_n0075_4_67 : LUT4_L
    generic map(
      INIT => X"FFA8"
    )
    port map (
      I0 => mod7_color_v0h2_y_0_Q,
      I1 => mod7_color_n0015,
      I2 => mod7_color_n0012,
      I3 => mod7_color_N128,
      LO => CHOICE15482
    );
  mod7_Ker1811 : LUT4
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_v_state(2),
      I2 => mod7_N72,
      I3 => mod7_n0028,
      O => mod7_N181
    );
  mod6_scram20_inst_Mxor_n0001_Result1 : LUT4_L
    generic map(
      INIT => X"6996"
    )
    port map (
      I0 => mod6_scram20_inst_S(6),
      I1 => mod6_scram20_inst_S(7),
      I2 => mod6_scram20_inst_S(8),
      I3 => mod6_scram20_inst_n0147(0),
      LO => mod6_scram20_inst_n0001
    );
  mod5_hdframe_n0070_1_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_luma_crc(10),
      O => CHOICE14737
    );
  mod7_n0034_5_144 : LUT4_L
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_n0024,
      I1 => mod7_tp_17_Q,
      I2 => mod7_v_state(3),
      I3 => mod7_load_val(5),
      LO => CHOICE14417
    );
  mod5_hdframe_n0070_3_57 : LUT4
    generic map(
      INIT => X"FEFC"
    )
    port map (
      I0 => CHOICE15145,
      I1 => mod7_color_N97,
      I2 => CHOICE15148,
      I3 => mod7_color_N58,
      O => CHOICE15150
    );
  mod5_hdframe_n0070_7_101 : LUT4
    generic map(
      INIT => X"FFEC"
    )
    port map (
      I0 => mod7_color_v0h5_cr(0),
      I1 => N1229,
      I2 => mod7_color_n0031,
      I3 => N8259,
      O => CHOICE14647
    );
  mod7_n0034_4_107_SW0 : LUT4_L
    generic map(
      INIT => X"8F88"
    )
    port map (
      I0 => mod7_v_state(3),
      I1 => mod7_N291,
      I2 => mod7_N154,
      I3 => mod7_n0030,
      LO => N8265
    );
  mod5_hdframe_n0075_2_2 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_chroma_crc(11),
      O => CHOICE15245
    );
  mod0_tp_option_21 : LUT3_L
    generic map(
      INIT => X"AC"
    )
    port map (
      I0 => MOSI_IBUF,
      I1 => mod0_tp_option(2),
      I2 => mod0_n0048,
      LO => N8076
    );
  mod5_hdframe_n0075_4_61 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_chroma_crc(13),
      O => CHOICE15479
    );
  mod7_Ker1991 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_N154,
      I1 => mod7_h_state_FFd4,
      I2 => mod7_h_state_FFd2,
      I3 => mod7_h_state_FFd3,
      O => mod7_N199
    );
  mod5_hdframe_n0075_4_60 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_chroma_crc(14),
      I2 => mod5_hdframe_n0190(0),
      I3 => mod5_hdframe_n0193(0),
      LO => CHOICE15478
    );
  mod5_hdframe_n0075_4_163_SW0 : LUT4
    generic map(
      INIT => X"B800"
    )
    port map (
      I0 => mod5_hdframe_linecount(9),
      I1 => mod5_hdframe_n0115,
      I2 => CHOICE15478,
      I3 => mod5_hdframe_N871,
      O => N8055
    );
  mod7_n0034_4_107 : LUT4
    generic map(
      INIT => X"EAAA"
    )
    port map (
      I0 => CHOICE14309,
      I1 => sif_hd_format(2),
      I2 => sif_hd_format(1),
      I3 => N8265,
      O => CHOICE14315
    );
  mod7_n0034_6_114 : LUT4_L
    generic map(
      INIT => X"C080"
    )
    port map (
      I0 => mod7_n0022,
      I1 => mod7_v_state(3),
      I2 => mod7_tp_21_Q,
      I3 => mod7_N291,
      LO => CHOICE14512
    );
  mod5_hdframe_n0070_2_20_SW0 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => mod7_color_v0h2_y_0_Q,
      I1 => mod7_color_n0015,
      I2 => mod7_color_n0013,
      I3 => mod7_color_n0012,
      O => N8015
    );
  mod5_hdframe_n0075_2_0 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0115,
      I1 => mod5_hdframe_linecount(7),
      O => CHOICE15243
    );
  mod5_hdframe_n0070_7_69 : LUT4_L
    generic map(
      INIT => X"FFEC"
    )
    port map (
      I0 => mod7_color_v1h4_y(9),
      I1 => mod7_color_N51,
      I2 => N8261,
      I3 => mod7_color_N100,
      LO => CHOICE14636
    );
  mod5_hdframe_n0070_4_87 : LUT4
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_color_v1h9_y(5),
      I1 => mod7_color_yramp(4),
      I2 => CHOICE15308,
      I3 => CHOICE15305,
      O => CHOICE15311
    );
  mod7_color_Ker1371 : LUT4_D
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_tp_4_Q,
      I1 => mod7_tp_5_Q,
      I2 => mod7_h_state_FFd4_4,
      I3 => mod7_h_state_FFd1_4,
      LO => N8652,
      O => mod7_color_N137
    );
  mod5_hdframe_Neq_stagelut11 : LUT4_L
    generic map(
      INIT => X"6006"
    )
    port map (
      I0 => mod5_hdframe_samplecount(2),
      I1 => mod5_tsl_2_Q,
      I2 => mod5_hdframe_samplecount(3),
      I3 => mod5_hdframe_n0086_3_Q,
      LO => mod5_hdframe_N182
    );
  mod5_hdframe_n0070_8_123 : LUT4_L
    generic map(
      INIT => X"5444"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => CHOICE14573,
      I2 => mod5_hdframe_N881,
      I3 => CHOICE14583,
      LO => CHOICE14587
    );
  mod7_color_Ker408 : LUT4_L
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_color_v0h2_y_2_Q,
      I1 => mod7_color_N121,
      I2 => mod7_color_N134,
      I3 => mod7_color_n0031,
      LO => CHOICE15112
    );
  mod5_hdframe_n0070_7_114 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => CHOICE14645,
      I1 => CHOICE14636,
      I2 => CHOICE14647,
      O => CHOICE14649
    );
  mod5_hdframe_n0070_9_15_SW1 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => mod7_color_v0h5_cr(0),
      I1 => mod7_h_state_FFd1,
      I2 => mod7_color_N122,
      I3 => mod7_tp_5_Q,
      O => N8068
    );
  mod7_n0034_5_161 : LUT4
    generic map(
      INIT => X"5510"
    )
    port map (
      I0 => mod7_tp_21_Q,
      I1 => mod7_tp_19_Q,
      I2 => mod7_N118,
      I3 => mod7_N216,
      O => CHOICE14424
    );
  mod5_hdframe_samplecount_8_rt3_87 : LUT1_L
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(8),
      LO => mod5_hdframe_samplecount_8_rt3
    );
  mod7_n0034_6_122 : LUT4
    generic map(
      INIT => X"5540"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_tp_21_Q,
      I2 => mod7_n0024,
      I3 => mod7_n0023,
      O => CHOICE14516
    );
  mod7_n0316_3_5 : LUT4
    generic map(
      INIT => X"FAD8"
    )
    port map (
      I0 => mod7_n0316(3),
      I1 => CHOICE12861,
      I2 => mod7_v_state(1),
      I3 => CHOICE12879,
      O => mod7_MUX_BLOCK_N7
    );
  mod5_hdframe_n0075_9_156 : LUT4
    generic map(
      INIT => X"FFD5"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_pattern(2),
      I2 => mod0_pattern(0),
      I3 => CHOICE14842,
      O => CHOICE14843
    );
  mod5_hdframe_n0070_1_7 : LUT4
    generic map(
      INIT => X"C8C0"
    )
    port map (
      I0 => mod7_color_v1h9_y(5),
      I1 => mod7_color_yramp(1),
      I2 => N8630,
      I3 => mod7_color_N58,
      O => CHOICE14740
    );
  mod7_n0034_4_130_SW0 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => mod7_h_state_FFd3,
      I1 => mod7_h_state_FFd4,
      I2 => mod7_h_state_FFd2,
      I3 => N8377,
      O => N8267
    );
  mod7_color_Ker42_SW0 : LUT4_D
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_color_v1h9_y(5),
      I1 => mod7_color_N135,
      I2 => mod7_color_N123,
      I3 => mod7_color_n0025,
      LO => N8653,
      O => N5872
    );
  mod5_hdframe_n0070_4_72 : LUT4_L
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => mod7_color_n0041,
      I1 => mod7_color_N01,
      I2 => mod7_color_n0024,
      I3 => mod7_color_n0037,
      LO => CHOICE15308
    );
  mod7_n0034_5_173 : LUT3_L
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => CHOICE14415,
      I1 => CHOICE14419,
      I2 => CHOICE14424,
      LO => CHOICE14426
    );
  mod5_hdframe_n0075_9_456 : LUT4_L
    generic map(
      INIT => X"6900"
    )
    port map (
      I0 => mod5_hdframe_n0190(1),
      I1 => mod5_hdframe_n0190(0),
      I2 => mod5_hdframe_n0191(0),
      I3 => mod5_hdframe_n0116,
      LO => CHOICE14898
    );
  mod5_hdframe_samplecount_6_rt_88 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(6),
      O => mod5_hdframe_samplecount_6_rt
    );
  mod5_hdframe_n0075_3_106_SW0 : LUT4
    generic map(
      INIT => X"B800"
    )
    port map (
      I0 => mod5_hdframe_linecount(8),
      I1 => mod5_hdframe_n0115,
      I2 => CHOICE15441,
      I3 => mod5_hdframe_N871,
      O => N8052
    );
  mod5_hdframe_n0075_5_201 : LUT4_L
    generic map(
      INIT => X"5510"
    )
    port map (
      I0 => mod5_hdframe_n0115,
      I1 => mod5_hdframe_n0116,
      I2 => CHOICE15234,
      I3 => CHOICE15212,
      LO => CHOICE15239
    );
  mod7_color_Ker1171 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => mod7_color_v1h4_y(9),
      I1 => mod0_tp_option(1),
      I2 => mod7_color_N121,
      I3 => mod7_color_N115,
      O => mod7_color_N117
    );
  mod7_n0034_4_130 : LUT4_L
    generic map(
      INIT => X"EAAA"
    )
    port map (
      I0 => N8267,
      I1 => mod7_N139,
      I2 => mod7_tp_21_Q,
      I3 => mod7_tp_19_Q,
      LO => CHOICE14324
    );
  mod5_hdframe_n0070_6_265 : LUT3
    generic map(
      INIT => X"0E"
    )
    port map (
      I0 => CHOICE14966,
      I1 => N8157,
      I2 => mod7_tp_0_Q,
      O => CHOICE14973
    );
  mod5_hdframe_n0075_1_0 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_chroma_crc(11),
      O => CHOICE14591
    );
  mod5_hdframe_n0070_4_96 : LUT4
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_color_v0h2_y_2_Q,
      I1 => mod7_color_N123,
      I2 => mod7_color_N140,
      I3 => mod7_color_n0015,
      O => CHOICE15313
    );
  mod7_n0034_6_136 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE14512,
      I1 => mod7_N257,
      I2 => mod7_n0030,
      I3 => CHOICE14516,
      O => CHOICE14519
    );
  mod7_color_Ker402 : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => mod7_color_N121,
      I1 => mod7_color_N137,
      I2 => N8639,
      O => CHOICE15109
    );
  mod5_hdframe_n0075_1_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_chroma_crc(10),
      O => CHOICE14592
    );
  mod5_hdframe_n0070_7_148_SW1 : LUT4
    generic map(
      INIT => X"FF54"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => CHOICE14631,
      I2 => N8655,
      I3 => CHOICE14630,
      O => N8036
    );
  mod5_hdframe_n0070_4_67 : LUT4
    generic map(
      INIT => X"B000"
    )
    port map (
      I0 => mod0_tp_option(1),
      I1 => mod0_tp_option(0),
      I2 => mod7_color_N115,
      I3 => mod7_color_N121,
      O => CHOICE15305
    );
  mod5_hdframe_hd_framegenerator_n0083_5_lut3 : LUT4
    generic map(
      INIT => X"1131"
    )
    port map (
      I0 => mod0_hd_format_tmp(2),
      I1 => mod0_hd_format_tmp(3),
      I2 => mod0_hd_format_tmp(0),
      I3 => mod0_hd_format_tmp(1),
      O => N7988
    );
  mod5_hdframe_n0070_3_40 : LUT4
    generic map(
      INIT => X"2820"
    )
    port map (
      I0 => mod0_pattern(1),
      I1 => mod0_pattern(2),
      I2 => mod0_pattern(0),
      I3 => mod7_color_yramp(3),
      O => CHOICE15145
    );
  mod7_color_Ker1211_1 : LUT4_D
    generic map(
      INIT => X"2212"
    )
    port map (
      I0 => mod7_h_state_FFd3_2,
      I1 => mod7_h_state_FFd2_2,
      I2 => mod7_h_state_FFd1_2,
      I3 => mod7_h_state_FFd4_2,
      LO => N8654,
      O => mod7_color_Ker1211
    );
  mod5_hdframe_n0070_1_12 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => mod7_color_N120,
      I1 => mod7_color_N137,
      I2 => mod7_color_n0041,
      I3 => mod7_tp_19_Q,
      O => CHOICE14743
    );
  mod7_n0034_4_147 : LUT4
    generic map(
      INIT => X"5510"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_tp_21_Q,
      I2 => mod7_N181,
      I3 => CHOICE14324,
      O => CHOICE14327
    );
  mod7_n0034_6_148 : LUT3_L
    generic map(
      INIT => X"54"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_n0030,
      I2 => mod7_n0026,
      LO => CHOICE14522
    );
  mod7_color_Ker224 : LUT4
    generic map(
      INIT => X"FF80"
    )
    port map (
      I0 => mod7_h_state_FFd1,
      I1 => mod7_tp_1_Q,
      I2 => N8628,
      I3 => mod7_color_N28,
      O => CHOICE15339
    );
  mod5_hdframe_n0070_4_61 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_luma_crc(13),
      O => CHOICE15302
    );
  mod5_hdframe_n0070_2_86 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE15169,
      I1 => mod5_hdframe_N881,
      I2 => mod7_color_N77,
      I3 => CHOICE15185,
      O => CHOICE15188
    );
  mod5_hdframe_n0075_7_60 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_chroma_crc(16),
      O => CHOICE14718
    );
  mod5_hdframe_n0070_4_60 : LUT4_L
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_luma_crc(14),
      I2 => mod5_hdframe_n0177(0),
      I3 => mod5_hdframe_n0179(0),
      LO => CHOICE15301
    );
  mod5_hdframe_n0070_9_100 : LUT4_L
    generic map(
      INIT => X"3237"
    )
    port map (
      I0 => mod7_color_N37,
      I1 => N8024,
      I2 => CHOICE15074,
      I3 => N8023,
      LO => CHOICE15081
    );
  mod5_hdframe_n0075_6_59 : LUT4
    generic map(
      INIT => X"8228"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => mod5_hdframe_chroma_crc(16),
      I2 => mod5_hdframe_n0192(0),
      I3 => mod5_hdframe_n0193(1),
      O => CHOICE14773
    );
  mod7_n0045_2_23 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE13924,
      I1 => mod7_v_state(2),
      I2 => CHOICE12713,
      I3 => CHOICE12759,
      O => CHOICE13926
    );
  mod7_color_Ker24_SW0 : LUT4
    generic map(
      INIT => X"ECA0"
    )
    port map (
      I0 => mod7_color_v0h2_y_0_Q,
      I1 => mod7_color_v1h2_cb_8_Q,
      I2 => N8616,
      I3 => mod7_color_n0018,
      O => N6921
    );
  mod5_hdframe_n0075_1_8 : LUT4
    generic map(
      INIT => X"FFEC"
    )
    port map (
      I0 => mod7_color_v0h2_y_2_Q,
      I1 => CHOICE14593,
      I2 => mod7_color_n0016,
      I3 => mod7_color_N91,
      O => CHOICE14596
    );
  mod5_hdframe_n0070_9_248 : LUT4_L
    generic map(
      INIT => X"CCCA"
    )
    port map (
      I0 => N8038,
      I1 => N8039,
      I2 => CHOICE15097,
      I3 => CHOICE15081,
      LO => mod5_hdframe_n0070(9)
    );
  mod5_hdframe_Ker881 : LUT3_D
    generic map(
      INIT => X"04"
    )
    port map (
      I0 => mod5_hdframe_n0135,
      I1 => mod5_hdframe_nor_cyo25,
      I2 => mod5_hdframe_n0100,
      LO => N8655,
      O => mod5_hdframe_N881
    );
  mod5_hdframe_n0070_6_374 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => CHOICE14994,
      O => CHOICE14995
    );
  mod5_hdframe_n0070_7_172 : LUT4_L
    generic map(
      INIT => X"AAAC"
    )
    port map (
      I0 => N8036,
      I1 => N8035,
      I2 => CHOICE14649,
      I3 => mod7_color_N21,
      LO => CHOICE14655
    );
  mod7_color_Ker25_SW1 : LUT4_L
    generic map(
      INIT => X"0007"
    )
    port map (
      I0 => N1258,
      I1 => mod7_color_n0018,
      I2 => mod7_color_N39,
      I3 => mod7_color_N51,
      LO => N8041
    );
  mod7_n0034_6_159 : LUT4
    generic map(
      INIT => X"5554"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_n0024,
      I2 => mod7_N199,
      I3 => CHOICE14522,
      O => CHOICE14526
    );
  mod5_hdframe_n0075_0_146 : LUT4_L
    generic map(
      INIT => X"5444"
    )
    port map (
      I0 => mod5_hdframe_n0116,
      I1 => CHOICE15025,
      I2 => mod5_hdframe_N881,
      I3 => CHOICE15051,
      LO => CHOICE15055
    );
  mod5_hdframe_n0075_4_83 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => mod7_color_N123,
      I1 => mod7_color_N140,
      I2 => mod7_color_v0h2_y_0_Q,
      O => CHOICE15486
    );
  mod7_color_Ker1251 : LUT4_L
    generic map(
      INIT => X"F100"
    )
    port map (
      I0 => mod7_v_state(1),
      I1 => mod7_v_state(0),
      I2 => mod7_v_state(2),
      I3 => mod7_tp_0_Q,
      LO => mod7_color_N125
    );
  mod5_hdframe_n0075_5_84 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => mod7_color_N107,
      I1 => mod7_color_N79,
      I2 => CHOICE15220,
      O => CHOICE15222
    );
  mod5_hdframe_linecount_10_rt_89 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_linecount(10),
      O => mod5_hdframe_linecount_10_rt
    );
  mod5_hdframe_n0075_5_159 : LUT4
    generic map(
      INIT => X"EEEA"
    )
    port map (
      I0 => CHOICE15213,
      I1 => mod5_hdframe_N881,
      I2 => CHOICE15224,
      I3 => CHOICE15231,
      O => CHOICE15234
    );
  mod5_hdframe_samplecount_4_rt_90 : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => mod5_hdframe_samplecount(4),
      O => mod5_hdframe_samplecount_4_rt
    );
  mod5_hdframe_n0070_9_15_SW0 : LUT4
    generic map(
      INIT => X"EEE0"
    )
    port map (
      I0 => mod7_color_v0h2_y_0_Q,
      I1 => mod7_color_v0h2_y_2_Q,
      I2 => mod7_color_N122,
      I3 => mod7_color_N121,
      O => N8067
    );
  mod5_hdframe_n0075_6_148_SW1 : LUT4
    generic map(
      INIT => X"F0F7"
    )
    port map (
      I0 => mod5_hdframe_chroma_crc(15),
      I1 => mod5_hdframe_n0100,
      I2 => mod5_hdframe_n0116,
      I3 => mod5_hdframe_N881,
      O => N8027
    );
  mod5_hdframe_n0070_2_52 : LUT4_L
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE15176,
      I1 => mod7_color_N40,
      I2 => CHOICE15183,
      I3 => N8015,
      LO => CHOICE15185
    );
  mod7_n0034_6_183_SW0 : LUT4
    generic map(
      INIT => X"4F44"
    )
    port map (
      I0 => mod7_v_state(3),
      I1 => mod7_n0024,
      I2 => N8604,
      I3 => mod7_tp_21_Q,
      O => N8233
    );
  mod5_hdframe_n0075_9_118 : LUT2
    generic map(
      INIT => X"D"
    )
    port map (
      I0 => mod5_hdframe_nor_cyo25,
      I1 => mod5_hdframe_n0135,
      O => CHOICE14828
    );
  mod7_n0034_3_11 : LUT4_L
    generic map(
      INIT => X"5510"
    )
    port map (
      I0 => mod7_tp_17_Q,
      I1 => mod7_N257,
      I2 => mod7_n0025,
      I3 => mod7_n0026,
      LO => CHOICE14335
    );
  mod7_h_state_FFd3_1_91 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd3_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd3_1
    );
  mod7_h_state_FFd1_1_92 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd1_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd1_1
    );
  mod7_h_state_FFd4_1_93 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd4_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd4_1
    );
  mod5_hdframe_n0070_6_469_SW0_SW0 : LUT4
    generic map(
      INIT => X"F1FF"
    )
    port map (
      I0 => mod7_v_state(1),
      I1 => mod7_v_state(0),
      I2 => mod7_v_state(2),
      I3 => CHOICE15010,
      O => N8498
    );
  mod7_h_state_FFd3_2_94 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd3_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd3_2
    );
  mod5_hdframe_n0075_0_109_G : LUT4_L
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => CHOICE15030,
      I1 => mod7_color_n0022,
      I2 => mod7_color_N59,
      I3 => CHOICE15035,
      LO => N8575
    );
  mod7_h_state_FFd3_3_95 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd3_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd3_3
    );
  mod7_h_state_FFd2_3_96 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd2_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd2_3
    );
  mod7_h_state_FFd4_3_97 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd4_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd4_3
    );
  mod7_h_state_FFd1_3_98 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd1_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd1_3
    );
  mod5_hdframe_n0075_8_114_SW1_SW0 : LUT4
    generic map(
      INIT => X"3233"
    )
    port map (
      I0 => mod5_hdframe_n0135,
      I1 => N8645,
      I2 => mod5_hdframe_n0100,
      I3 => mod5_hdframe_nor_cyo25,
      O => N8504
    );
  mod7_h_state_FFd4_4_99 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd4_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd4_4
    );
  mod7_h_state_FFd1_4_100 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd1_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd1_4
    );
  mod7_h_state_FFd3_4_101 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd3_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd3_4
    );
  mod7_h_state_FFd2_4_102 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd2_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd2_4
    );
  mod7_h_state_FFd4_5_103 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd4_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd4_5
    );
  mod7_h_state_FFd1_5_104 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd1_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd1_5
    );
  mod7_h_state_FFd3_5_105 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd3_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd3_5
    );
  mod7_h_state_FFd2_5_106 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => mod7_h_state_FFd2_In,
      CLR => mod7_h_state_FFd1_N0,
      C => txclka2,
      Q => mod7_h_state_FFd2_5
    );
  txclka2_BUFG : BUFG
    port map (
      I => txclka21,
      O => txclka2
    );
  mod5_hdframe_Ker53_rn_0111 : MUXF5
    port map (
      I0 => N8510,
      I1 => N8511,
      S => mod5_hdframe_n0133,
      O => mod5_hdframe_N531
    );
  mod5_hdframe_Ker53_rn_0111_F : LUT4_L
    generic map(
      INIT => X"BAAA"
    )
    port map (
      I0 => mod5_hdframe_N301,
      I1 => mod5_hdframe_n0132,
      I2 => mod5_hdframe_n0114,
      I3 => mod5_hdframe_linecount(2),
      LO => N8510
    );
  mod7_Ker2425 : MUXF5
    port map (
      I0 => N8512,
      I1 => N8513,
      S => mod7_active_line(7),
      O => CHOICE13413
    );
  mod7_Ker2425_F : LUT4
    generic map(
      INIT => X"81FF"
    )
    port map (
      I0 => mod0_pattern(0),
      I1 => mod0_pattern(2),
      I2 => mod0_pattern(1),
      I3 => mod7_active_line(3),
      O => N8512
    );
  mod7_n0046_2_148 : MUXF5
    port map (
      I0 => N8514,
      I1 => N8515,
      S => mod7_active_line(4),
      O => CHOICE12987
    );
  mod7_n0046_2_148_F : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_N231,
      I1 => mod0_pattern(1),
      I2 => mod0_pattern(0),
      I3 => mod0_pattern(2),
      O => N8514
    );
  mod7_n0044_1_94_SW0 : MUXF5
    port map (
      I0 => N8516,
      I1 => N8517,
      S => mod7_active_line(1),
      O => N8329
    );
  mod7_n0044_1_94_SW0_F : LUT4
    generic map(
      INIT => X"FBFF"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(8),
      I2 => mod7_active_line(4),
      I3 => CHOICE12793,
      O => N8516
    );
  mod7_Ker79 : MUXF5
    port map (
      I0 => N8518,
      I1 => N8519,
      S => mod7_tp_21_Q,
      O => mod7_N79
    );
  mod7_Ker79_F : LUT3_L
    generic map(
      INIT => X"2A"
    )
    port map (
      I0 => mod7_n0030,
      I1 => mod7_tp_19_Q,
      I2 => mod7_v_state(2),
      LO => N8518
    );
  mod7_h_state_FFd1_In_107 : MUXF5
    port map (
      I0 => N8520,
      I1 => N8521,
      S => mod7_h_state_FFd4,
      O => mod7_h_state_FFd1_In
    );
  mod7_h_state_FFd1_In_F : LUT3
    generic map(
      INIT => X"2A"
    )
    port map (
      I0 => mod7_h_state_FFd1,
      I1 => mod7_h_state_FFd3,
      I2 => mod5_hdframe_sav,
      O => N8520
    );
  mod7_n0034_5_112 : MUXF5
    port map (
      I0 => N8522,
      I1 => N8523,
      S => mod7_h_state_FFd3,
      O => CHOICE14411
    );
  mod7_n0034_5_112_F : LUT4_L
    generic map(
      INIT => X"A2AA"
    )
    port map (
      I0 => mod7_h_state_FFd2,
      I1 => mod7_tp_19_Q,
      I2 => mod7_tp_21_Q,
      I3 => mod7_v_state(2),
      LO => N8522
    );
  mod7_Ker22230 : MUXF5
    port map (
      I0 => N8524,
      I1 => N8525,
      S => mod7_active_line(8),
      O => CHOICE12742
    );
  mod7_Ker22230_F : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(3),
      O => N8524
    );
  mod7_n0046_3_692 : MUXF5
    port map (
      I0 => N8526,
      I1 => N8527,
      S => mod7_active_line(7),
      O => CHOICE13604
    );
  mod7_n0046_3_692_F : LUT4
    generic map(
      INIT => X"D555"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(1),
      I3 => mod7_active_line(2),
      O => N8526
    );
  mod5_hdframe_n0070_1_21 : MUXF5
    port map (
      I0 => N8528,
      I1 => N8529,
      S => mod7_color_N121,
      O => CHOICE14747
    );
  mod5_hdframe_n0070_1_21_F : LUT4_L
    generic map(
      INIT => X"C080"
    )
    port map (
      I0 => mod7_color_v0h2_y_0_Q,
      I1 => mod7_color_N134,
      I2 => mod7_color_N122,
      I3 => mod7_color_v0h2_y_2_Q,
      LO => N8528
    );
  mod5_hdframe_n0070_1_1171 : MUXF5
    port map (
      I0 => N8530,
      I1 => N8531,
      S => mod5_hdframe_n0116,
      O => mod5_hdframe_n0070(1)
    );
  mod5_hdframe_n0070_1_1171_F : LUT4_L
    generic map(
      INIT => X"BAAA"
    )
    port map (
      I0 => mod5_hdframe_N301,
      I1 => mod5_hdframe_n0115,
      I2 => mod5_hdframe_N871,
      I3 => CHOICE14753,
      LO => N8530
    );
  mod7_color_Ker511 : MUXF5
    port map (
      I0 => N8532,
      I1 => N8533,
      S => mod7_color_N121,
      O => mod7_color_N51
    );
  mod7_color_Ker511_F : LUT4_L
    generic map(
      INIT => X"C080"
    )
    port map (
      I0 => mod7_color_v0h2_y_0_Q,
      I1 => mod7_color_N134,
      I2 => mod7_color_N120,
      I3 => mod7_color_v0h2_y_2_Q,
      LO => N8532
    );
  mod5_hdframe_n0070_5_208_SW0 : MUXF5
    port map (
      I0 => N8534,
      I1 => N8535,
      S => mod5_hdframe_n0116,
      O => N8058
    );
  mod5_hdframe_n0070_5_208_SW0_F : LUT3_L
    generic map(
      INIT => X"DF"
    )
    port map (
      I0 => mod5_hdframe_n0100,
      I1 => mod5_hdframe_n0115,
      I2 => mod5_hdframe_luma_crc(14),
      LO => N8534
    );
  mod7_Ker8420 : MUXF5
    port map (
      I0 => N8536,
      I1 => N8537,
      S => mod7_active_line(1),
      O => CHOICE12384
    );
  mod7_Ker8420_F : LUT4
    generic map(
      INIT => X"F8B8"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(3),
      I2 => mod7_active_line(4),
      I3 => mod7_active_line(5),
      O => N8536
    );
  mod7_Ker3549 : MUXF5
    port map (
      I0 => N8538,
      I1 => N8539,
      S => mod7_active_line(3),
      O => CHOICE12403
    );
  mod7_Ker3549_F : LUT4
    generic map(
      INIT => X"CAC8"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(7),
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(4),
      O => N8538
    );
  mod7_n0046_3_776 : MUXF5
    port map (
      I0 => N8540,
      I1 => N8541,
      S => mod7_active_line(6),
      O => CHOICE13628
    );
  mod7_n0046_3_776_F : LUT4
    generic map(
      INIT => X"0809"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(8),
      I2 => mod7_active_line(7),
      I3 => mod7_active_line(5),
      O => N8540
    );
  mod5_hdframe_Ker51_rn_0111 : MUXF5
    port map (
      I0 => N8542,
      I1 => N8543,
      S => mod5_hdframe_n0133,
      O => mod5_hdframe_N511
    );
  mod5_hdframe_Ker51_rn_0111_F : LUT4_L
    generic map(
      INIT => X"BAAA"
    )
    port map (
      I0 => mod5_hdframe_N301,
      I1 => mod5_hdframe_n0132,
      I2 => mod5_hdframe_n0114,
      I3 => mod5_hdframe_linecount(0),
      LO => N8542
    );
  mod7_n0046_3_1147 : MUXF5
    port map (
      I0 => N8544,
      I1 => N8545,
      S => mod7_active_line(6),
      O => CHOICE13732
    );
  mod7_n0046_3_1147_F : LUT4
    generic map(
      INIT => X"2A2F"
    )
    port map (
      I0 => mod7_active_line(3),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(9),
      O => N8544
    );
  mod7_Ker229361_SW0 : MUXF5
    port map (
      I0 => N8546,
      I1 => N8547,
      S => mod7_active_line(0),
      O => N8209
    );
  mod7_Ker229361_SW0_F : LUT4
    generic map(
      INIT => X"F2FA"
    )
    port map (
      I0 => mod7_active_line(5),
      I1 => mod7_active_line(1),
      I2 => mod7_active_line(6),
      I3 => mod7_active_line(2),
      O => N8546
    );
  mod7_n0047_3_90 : MUXF5
    port map (
      I0 => N8548,
      I1 => N8549,
      S => mod7_n0079,
      O => CHOICE13792
    );
  mod7_n0047_3_90_F : LUT4
    generic map(
      INIT => X"BAAA"
    )
    port map (
      I0 => mod7_n0078,
      I1 => mod7_tp_19_Q,
      I2 => mod7_N241,
      I3 => mod7_N300,
      O => N8548
    );
  mod7_n0046_1_98 : MUXF5
    port map (
      I0 => N8550,
      I1 => N8551,
      S => mod7_active_line(5),
      O => CHOICE12908
    );
  mod7_n0046_1_98_F : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => mod7_N236,
      I1 => mod7_active_line(2),
      I2 => CHOICE12905,
      O => N8550
    );
  mod7_n0046_3_427 : MUXF5
    port map (
      I0 => N8552,
      I1 => N8553,
      S => mod7_active_line(4),
      O => CHOICE13537
    );
  mod7_n0046_3_427_F : LUT4
    generic map(
      INIT => X"08FA"
    )
    port map (
      I0 => mod7_active_line(9),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(5),
      I3 => mod7_active_line(3),
      O => N8552
    );
  mod7_n0044_3_327 : MUXF5
    port map (
      I0 => N8554,
      I1 => N8555,
      S => mod7_active_line(8),
      O => CHOICE13166
    );
  mod7_n0044_3_327_F : LUT4
    generic map(
      INIT => X"1110"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(4),
      I3 => mod7_active_line(0),
      O => N8554
    );
  mod7_n0047_2_104 : MUXF5
    port map (
      I0 => N8556,
      I1 => N8557,
      S => mod7_active_line(2),
      O => CHOICE13018
    );
  mod7_n0047_2_104_F : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_N232,
      I2 => mod7_tp_17_Q,
      I3 => mod7_active_line(1),
      O => N8556
    );
  mod5_hdframe_Ker52_rn_0111 : MUXF5
    port map (
      I0 => N8558,
      I1 => N8559,
      S => mod5_hdframe_n0133,
      O => mod5_hdframe_N521
    );
  mod5_hdframe_Ker52_rn_0111_F : LUT4_L
    generic map(
      INIT => X"BAAA"
    )
    port map (
      I0 => mod5_hdframe_N301,
      I1 => mod5_hdframe_n0132,
      I2 => mod5_hdframe_n0114,
      I3 => mod5_hdframe_linecount(1),
      LO => N8558
    );
  mod7_n0046_2_19 : MUXF5
    port map (
      I0 => N8560,
      I1 => N8561,
      S => mod7_active_line(7),
      O => CHOICE12955
    );
  mod7_n0046_2_19_F : LUT3
    generic map(
      INIT => X"F1"
    )
    port map (
      I0 => mod7_active_line(2),
      I1 => mod7_active_line(8),
      I2 => mod7_N4,
      O => N8560
    );
  mod7_n0047_1_18 : MUXF5
    port map (
      I0 => N8562,
      I1 => N8563,
      S => mod7_active_line(2),
      O => CHOICE13748
    );
  mod7_n0047_1_18_F : LUT4
    generic map(
      INIT => X"5060"
    )
    port map (
      I0 => mod0_pattern(2),
      I1 => mod0_pattern(1),
      I2 => mod7_active_line(6),
      I3 => mod0_pattern(0),
      O => N8562
    );
  mod7_n0046_3_310 : MUXF5
    port map (
      I0 => N8564,
      I1 => N8565,
      S => mod7_active_line(5),
      O => CHOICE13504
    );
  mod7_n0046_3_310_F : LUT3
    generic map(
      INIT => X"04"
    )
    port map (
      I0 => mod7_active_line(4),
      I1 => mod7_active_line(9),
      I2 => mod7_active_line(8),
      O => N8564
    );
  mod7_n0047_3_192_SW0 : MUXF5
    port map (
      I0 => N8566,
      I1 => N8567,
      S => mod7_active_line(5),
      O => N8183
    );
  mod7_n0047_3_192_SW0_F : LUT4
    generic map(
      INIT => X"AE8C"
    )
    port map (
      I0 => mod7_active_line(1),
      I1 => mod7_active_line(3),
      I2 => mod7_active_line(2),
      I3 => mod7_active_line(9),
      O => N8566
    );
  mod7_Ker298 : MUXF5
    port map (
      I0 => N8568,
      I1 => N8569,
      S => mod7_active_line(8),
      O => mod7_N298
    );
  mod7_Ker298_F : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => mod7_active_line(6),
      I1 => mod7_active_line(5),
      I2 => mod7_active_line(7),
      O => N8568
    );
  mod7_n0044_3_800 : MUXF5
    port map (
      I0 => N8570,
      I1 => N8571,
      S => mod7_active_line(0),
      O => CHOICE13295
    );
  mod7_n0044_3_800_F : LUT4
    generic map(
      INIT => X"BBBF"
    )
    port map (
      I0 => mod7_active_line(7),
      I1 => mod7_active_line(2),
      I2 => mod7_active_line(8),
      I3 => mod7_active_line(4),
      O => N8570
    );
  mod7_n0034_2_112_SW0 : MUXF5
    port map (
      I0 => N8572,
      I1 => N8573,
      S => mod7_tp_17_Q,
      O => N8153
    );
  mod7_n0034_2_112_SW0_F : LUT4_L
    generic map(
      INIT => X"AA80"
    )
    port map (
      I0 => mod7_tp_19_Q,
      I1 => mod7_n0025,
      I2 => mod7_v_state(2),
      I3 => CHOICE14872,
      LO => N8572
    );
  mod5_hdframe_n0075_0_109 : MUXF5
    port map (
      I0 => N8574,
      I1 => N8575,
      S => mod5_hdframe_samplecount(0),
      O => CHOICE15051
    );
  mod5_hdframe_n0075_0_109_F : LUT4_L
    generic map(
      INIT => X"FEEE"
    )
    port map (
      I0 => mod7_color_N61,
      I1 => mod7_color_N28,
      I2 => CHOICE15043,
      I3 => CHOICE15045,
      LO => N8574
    );
  txda_9_1_108 : FD
    port map (
      D => mod6_scram20_inst_dout(10),
      C => TXRIOA_USRCLK,
      Q => txda_9_1
    );
  txda_19_1_109 : FD
    port map (
      D => mod6_scram20_inst_dout(0),
      C => TXRIOA_USRCLK,
      Q => txda_19_1
    );
  mod0_sd_hd_1_110 : FDCE_1
    port map (
      D => MOSI_IBUF,
      CE => mod0_n0286,
      CLR => mod0_new_delay_N0,
      C => SCK_BUFGP,
      Q => mod0_sd_hd_1
    );

end Structure;

