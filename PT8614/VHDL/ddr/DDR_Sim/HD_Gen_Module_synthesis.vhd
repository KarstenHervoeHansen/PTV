--------------------------------------------------------------------------------
-- Copyright (c) 1995-2005 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____
--  /   /\/   /
-- /___/  \  /    Vendor: Xilinx
-- \   \   \/     Version: I.27
--  \   \         Application: netgen
--  /   /         Filename: HD_Gen_Module_synthesis.vhd
-- /___/   /\     Timestamp: Mon Feb 26 16:52:47 2007
-- \   \  /  \ 
--  \___\/\___\
--             
-- Command	: -intstyle ise -ar Structure -tm HD_Gen_Module -w -dir netgen/synthesis -ofmt vhdl -sim HD_Gen_Module.ngc HD_Gen_Module_synthesis.vhd 
-- Device	: xc2vp20-6-ff896
-- Input file	: HD_Gen_Module.ngc
-- Output file	: C:\MyProject\DDR2interface_test\DDR2interface5\netgen\synthesis\HD_Gen_Module_synthesis.vhd
-- # of Entities	: 69
-- Design Name	: HD_Gen_Module
-- Xilinx	: C:\Xilinx81
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
--     Synthesis and Simulation Design Guide, Chapter 6
--             
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity system_controller is
  port (
    clk : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    ar_done : in STD_LOGIC := 'X'; 
    auto_ref_req : in STD_LOGIC := 'X'; 
    init_val : in STD_LOGIC := 'X'; 
    clk180 : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    user_cmd_ack : in STD_LOGIC := 'X'; 
    user_data_valid : in STD_LOGIC := 'X'; 
    TP1 : out STD_LOGIC; 
    TP2 : out STD_LOGIC; 
    TP3 : out STD_LOGIC; 
    burst_done : out STD_LOGIC; 
    user_output_data : in STD_LOGIC_VECTOR ( 31 downto 0 ); 
    user_input_address : out STD_LOGIC_VECTOR ( 22 downto 0 ); 
    TP_port : out STD_LOGIC_VECTOR ( 21 downto 0 ); 
    user_data_mask : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    user_input_data : out STD_LOGIC_VECTOR ( 31 downto 0 ); 
    user_command_register : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    user_config_register1 : out STD_LOGIC_VECTOR ( 14 downto 0 ); 
    user_config_register2 : out STD_LOGIC_VECTOR ( 12 downto 0 ); 
    user_bank_address : out STD_LOGIC_VECTOR ( 1 downto 0 ) 
  );
end system_controller;

architecture Structure of system_controller is
  signal NlwRenamedSignal_TP1 : STD_LOGIC; 
  signal NlwRenamedSignal_TP2 : STD_LOGIC; 
  signal next_burst_done : STD_LOGIC; 
  signal Q_n0043 : STD_LOGIC; 
  signal current_state_FFd7_1 : STD_LOGIC; 
  signal current_state_FFd9_2 : STD_LOGIC; 
  signal current_state_FFd10_3 : STD_LOGIC; 
  signal current_state_FFd2_4 : STD_LOGIC; 
  signal current_state_FFd4_5 : STD_LOGIC; 
  signal current_state_FFd5_6 : STD_LOGIC; 
  signal current_state_FFd3_7 : STD_LOGIC; 
  signal current_state_FFd6_8 : STD_LOGIC; 
  signal current_state_FFd8_9 : STD_LOGIC; 
  signal Q_n0058 : STD_LOGIC; 
  signal current_burst_done_10 : STD_LOGIC; 
  signal current_reg1_0_Q : STD_LOGIC; 
  signal current_reg1_7_Q : STD_LOGIC; 
  signal current_reg1_8_Q : STD_LOGIC; 
  signal current_reg1_11_Q : STD_LOGIC; 
  signal current_reg2_0_Q : STD_LOGIC; 
  signal current_reg2_1_Q : STD_LOGIC; 
  signal current_reg2_2_Q : STD_LOGIC; 
  signal current_reg2_7_Q : STD_LOGIC; 
  signal next_input_data_19_Q : STD_LOGIC; 
  signal next_input_data_15_Q : STD_LOGIC; 
  signal next_input_data_14_Q : STD_LOGIC; 
  signal next_input_data_12_Q : STD_LOGIC; 
  signal next_input_data_11_Q : STD_LOGIC; 
  signal next_input_data_10_Q : STD_LOGIC; 
  signal next_input_data_1_Q : STD_LOGIC; 
  signal next_input_data_0_Q : STD_LOGIC; 
  signal N1 : STD_LOGIC; 
  signal reset_INV : STD_LOGIC; 
  signal system_controller_Result_0_sel : STD_LOGIC; 
  signal current_state_FFd1_11 : STD_LOGIC; 
  signal current_state_FFd2_In_12 : STD_LOGIC; 
  signal current_state_FFd3_In_13 : STD_LOGIC; 
  signal current_state_FFd4_In : STD_LOGIC; 
  signal current_state_FFd5_In_14 : STD_LOGIC; 
  signal current_state_FFd6_In_15 : STD_LOGIC; 
  signal current_state_FFd7_In : STD_LOGIC; 
  signal current_state_FFd8_In_16 : STD_LOGIC; 
  signal current_state_FFd9_In_17 : STD_LOGIC; 
  signal current_state_FFd10_In_18 : STD_LOGIC; 
  signal N12 : STD_LOGIC; 
  signal N0 : STD_LOGIC; 
  signal N11 : STD_LOGIC; 
  signal N4 : STD_LOGIC; 
  signal N10 : STD_LOGIC; 
  signal N111 : STD_LOGIC; 
  signal N121 : STD_LOGIC; 
  signal N13 : STD_LOGIC; 
  signal N14 : STD_LOGIC; 
  signal N15 : STD_LOGIC; 
  signal N18 : STD_LOGIC; 
  signal N20 : STD_LOGIC; 
  signal N23 : STD_LOGIC; 
  signal N27 : STD_LOGIC; 
  signal N29 : STD_LOGIC; 
  signal N30 : STD_LOGIC; 
  signal N35 : STD_LOGIC; 
  signal N36 : STD_LOGIC; 
  signal N37 : STD_LOGIC; 
  signal N39 : STD_LOGIC; 
  signal N24 : STD_LOGIC; 
  signal N71 : STD_LOGIC; 
  signal N8 : STD_LOGIC; 
  signal N101 : STD_LOGIC; 
  signal N131 : STD_LOGIC; 
  signal N311 : STD_LOGIC; 
  signal N321 : STD_LOGIC; 
  signal N341 : STD_LOGIC; 
  signal N371 : STD_LOGIC; 
  signal N271 : STD_LOGIC; 
  signal N291 : STD_LOGIC; 
  signal N32 : STD_LOGIC; 
  signal N34 : STD_LOGIC; 
  signal N361 : STD_LOGIC; 
  signal N38 : STD_LOGIC; 
  signal N42 : STD_LOGIC; 
  signal N44 : STD_LOGIC; 
  signal N45 : STD_LOGIC; 
  signal N58 : STD_LOGIC; 
  signal N60 : STD_LOGIC; 
  signal N62 : STD_LOGIC; 
  signal N64 : STD_LOGIC; 
  signal N84 : STD_LOGIC; 
  signal N96 : STD_LOGIC; 
  signal N1011 : STD_LOGIC; 
  signal N103 : STD_LOGIC; 
  signal N105 : STD_LOGIC; 
  signal N112 : STD_LOGIC; 
  signal N163 : STD_LOGIC; 
  signal N197 : STD_LOGIC; 
  signal N211 : STD_LOGIC; 
  signal N219 : STD_LOGIC; 
  signal N220 : STD_LOGIC; 
  signal N222 : STD_LOGIC; 
  signal N223 : STD_LOGIC; 
  signal N225 : STD_LOGIC; 
  signal N229 : STD_LOGIC; 
  signal N230 : STD_LOGIC; 
  signal N232 : STD_LOGIC; 
  signal N233 : STD_LOGIC; 
  signal Ker4_map506 : STD_LOGIC; 
  signal Ker4_map511 : STD_LOGIC; 
  signal N254 : STD_LOGIC; 
  signal N260 : STD_LOGIC; 
  signal N274 : STD_LOGIC; 
  signal N283 : STD_LOGIC; 
  signal N14_map527 : STD_LOGIC; 
  signal N14_map533 : STD_LOGIC; 
  signal N14_map538 : STD_LOGIC; 
  signal N14_map539 : STD_LOGIC; 
  signal N14_map543 : STD_LOGIC; 
  signal N337 : STD_LOGIC; 
  signal N339 : STD_LOGIC; 
  signal N3411 : STD_LOGIC; 
  signal N345 : STD_LOGIC; 
  signal N358 : STD_LOGIC; 
  signal N360 : STD_LOGIC; 
  signal test_count_4_rt_19 : STD_LOGIC; 
  signal N362 : STD_LOGIC; 
  signal N367 : STD_LOGIC; 
  signal N369 : STD_LOGIC; 
  signal N3711 : STD_LOGIC; 
  signal N373 : STD_LOGIC; 
  signal N378 : STD_LOGIC; 
  signal N379 : STD_LOGIC; 
  signal N380 : STD_LOGIC; 
  signal N381 : STD_LOGIC; 
  signal N384 : STD_LOGIC; 
  signal N385 : STD_LOGIC; 
  signal NlwRenamedSig_OI_user_bank_address : STD_LOGIC_VECTOR ( 0 downto 0 ); 
  signal next_reg3 : STD_LOGIC_VECTOR ( 2 downto 1 ); 
  signal next_addr : STD_LOGIC_VECTOR ( 9 downto 2 ); 
  signal loop_count : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0056 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal col_addr : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0057 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal intern_reg0 : STD_LOGIC_VECTOR ( 31 downto 10 ); 
  signal intern_reg1 : STD_LOGIC_VECTOR ( 31 downto 10 ); 
  signal current_input_data : STD_LOGIC_VECTOR ( 31 downto 0 ); 
  signal current_reg3 : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal current_input_address : STD_LOGIC_VECTOR ( 9 downto 2 ); 
  signal test_count : STD_LOGIC_VECTOR ( 6 downto 0 ); 
  signal Result : STD_LOGIC_VECTOR ( 6 downto 1 ); 
begin
  NlwRenamedSignal_TP1 <= user_cmd_ack;
  NlwRenamedSignal_TP2 <= user_data_valid;
  TP1 <= NlwRenamedSignal_TP1;
  TP2 <= NlwRenamedSignal_TP2;
  TP3 <= NlwRenamedSig_OI_user_bank_address(0);
  user_input_address(22) <= NlwRenamedSig_OI_user_bank_address(0);
  user_input_address(21) <= NlwRenamedSig_OI_user_bank_address(0);
  user_input_address(20) <= NlwRenamedSig_OI_user_bank_address(0);
  user_input_address(19) <= NlwRenamedSig_OI_user_bank_address(0);
  user_input_address(18) <= NlwRenamedSig_OI_user_bank_address(0);
  user_input_address(17) <= NlwRenamedSig_OI_user_bank_address(0);
  user_input_address(16) <= NlwRenamedSig_OI_user_bank_address(0);
  user_input_address(15) <= NlwRenamedSig_OI_user_bank_address(0);
  user_input_address(14) <= NlwRenamedSig_OI_user_bank_address(0);
  user_input_address(13) <= NlwRenamedSig_OI_user_bank_address(0);
  user_input_address(12) <= NlwRenamedSig_OI_user_bank_address(0);
  user_input_address(11) <= NlwRenamedSig_OI_user_bank_address(0);
  user_input_address(10) <= NlwRenamedSig_OI_user_bank_address(0);
  user_input_address(1) <= NlwRenamedSig_OI_user_bank_address(0);
  user_input_address(0) <= NlwRenamedSig_OI_user_bank_address(0);
  user_data_mask(3) <= NlwRenamedSig_OI_user_bank_address(0);
  user_data_mask(2) <= NlwRenamedSig_OI_user_bank_address(0);
  user_data_mask(1) <= NlwRenamedSig_OI_user_bank_address(0);
  user_data_mask(0) <= NlwRenamedSig_OI_user_bank_address(0);
  user_command_register(3) <= NlwRenamedSig_OI_user_bank_address(0);
  user_command_register(0) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register1(14) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register1(13) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register1(11) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register1(10) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register1(9) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register1(8) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register1(7) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register1(6) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register1(3) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register1(2) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register1(0) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register2(12) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register2(11) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register2(10) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register2(6) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register2(5) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register2(4) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register2(3) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register2(1) <= NlwRenamedSig_OI_user_bank_address(0);
  user_config_register2(0) <= NlwRenamedSig_OI_user_bank_address(0);
  user_bank_address(1) <= NlwRenamedSig_OI_user_bank_address(0);
  user_bank_address(0) <= NlwRenamedSig_OI_user_bank_address(0);
  XST_GND : GND
    port map (
      G => NlwRenamedSig_OI_user_bank_address(0)
    );
  XST_VCC : VCC
    port map (
      P => N1
    );
  loop_count_0 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0056(0),
      CLR => reset_INV,
      C => clk90,
      Q => loop_count(0)
    );
  loop_count_1 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0056(1),
      CLR => reset_INV,
      C => clk90,
      Q => loop_count(1)
    );
  loop_count_2 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0056(2),
      CLR => reset_INV,
      C => clk90,
      Q => loop_count(2)
    );
  loop_count_3 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0056(3),
      CLR => reset_INV,
      C => clk90,
      Q => loop_count(3)
    );
  loop_count_4 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0056(4),
      CLR => reset_INV,
      C => clk90,
      Q => loop_count(4)
    );
  loop_count_5 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0056(5),
      CLR => reset_INV,
      C => clk90,
      Q => loop_count(5)
    );
  loop_count_6 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0056(6),
      CLR => reset_INV,
      C => clk90,
      Q => loop_count(6)
    );
  loop_count_7 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0056(7),
      CLR => reset_INV,
      C => clk90,
      Q => loop_count(7)
    );
  col_addr_0 : FDC
    port map (
      D => Q_n0057(0),
      CLR => reset_INV,
      C => clk90,
      Q => col_addr(0)
    );
  col_addr_1 : FDC
    port map (
      D => Q_n0057(1),
      CLR => reset_INV,
      C => clk90,
      Q => col_addr(1)
    );
  col_addr_2 : FDC
    port map (
      D => Q_n0057(2),
      CLR => reset_INV,
      C => clk90,
      Q => col_addr(2)
    );
  col_addr_3 : FDC
    port map (
      D => Q_n0057(3),
      CLR => reset_INV,
      C => clk90,
      Q => col_addr(3)
    );
  col_addr_4 : FDC
    port map (
      D => Q_n0057(4),
      CLR => reset_INV,
      C => clk90,
      Q => col_addr(4)
    );
  col_addr_5 : FDC
    port map (
      D => Q_n0057(5),
      CLR => reset_INV,
      C => clk90,
      Q => col_addr(5)
    );
  col_addr_6 : FDC
    port map (
      D => Q_n0057(6),
      CLR => reset_INV,
      C => clk90,
      Q => col_addr(6)
    );
  col_addr_7 : FDC
    port map (
      D => Q_n0057(7),
      CLR => reset_INV,
      C => clk90,
      Q => col_addr(7)
    );
  intern_reg0_10 : FDCE
    port map (
      D => user_output_data(10),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(10)
    );
  intern_reg0_11 : FDCE
    port map (
      D => user_output_data(11),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(11)
    );
  intern_reg0_12 : FDCE
    port map (
      D => user_output_data(12),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(12)
    );
  intern_reg0_13 : FDCE
    port map (
      D => user_output_data(13),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(13)
    );
  intern_reg0_14 : FDCE
    port map (
      D => user_output_data(14),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(14)
    );
  intern_reg0_15 : FDCE
    port map (
      D => user_output_data(15),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(15)
    );
  intern_reg0_16 : FDCE
    port map (
      D => user_output_data(16),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(16)
    );
  intern_reg0_17 : FDCE
    port map (
      D => user_output_data(17),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(17)
    );
  intern_reg0_18 : FDCE
    port map (
      D => user_output_data(18),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(18)
    );
  intern_reg0_19 : FDCE
    port map (
      D => user_output_data(19),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(19)
    );
  intern_reg0_20 : FDCE
    port map (
      D => user_output_data(20),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(20)
    );
  intern_reg0_21 : FDCE
    port map (
      D => user_output_data(21),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(21)
    );
  intern_reg0_22 : FDCE
    port map (
      D => user_output_data(22),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(22)
    );
  intern_reg0_23 : FDCE
    port map (
      D => user_output_data(23),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(23)
    );
  intern_reg0_24 : FDCE
    port map (
      D => user_output_data(24),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(24)
    );
  intern_reg0_25 : FDCE
    port map (
      D => user_output_data(25),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(25)
    );
  intern_reg0_26 : FDCE
    port map (
      D => user_output_data(26),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(26)
    );
  intern_reg0_27 : FDCE
    port map (
      D => user_output_data(27),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(27)
    );
  intern_reg0_28 : FDCE
    port map (
      D => user_output_data(28),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(28)
    );
  intern_reg0_29 : FDCE
    port map (
      D => user_output_data(29),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(29)
    );
  intern_reg0_30 : FDCE
    port map (
      D => user_output_data(30),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(30)
    );
  intern_reg0_31 : FDCE
    port map (
      D => user_output_data(31),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg0(31)
    );
  current_input_address_2 : FDC
    port map (
      D => next_addr(2),
      CLR => reset_INV,
      C => clk90,
      Q => current_input_address(2)
    );
  current_input_address_3 : FDC
    port map (
      D => next_addr(3),
      CLR => reset_INV,
      C => clk90,
      Q => current_input_address(3)
    );
  current_input_address_4 : FDC
    port map (
      D => next_addr(4),
      CLR => reset_INV,
      C => clk90,
      Q => current_input_address(4)
    );
  current_input_address_5 : FDC
    port map (
      D => next_addr(5),
      CLR => reset_INV,
      C => clk90,
      Q => current_input_address(5)
    );
  current_input_address_6 : FDC
    port map (
      D => next_addr(6),
      CLR => reset_INV,
      C => clk90,
      Q => current_input_address(6)
    );
  current_input_address_7 : FDC
    port map (
      D => next_addr(7),
      CLR => reset_INV,
      C => clk90,
      Q => current_input_address(7)
    );
  current_input_address_8 : FDC
    port map (
      D => next_addr(8),
      CLR => reset_INV,
      C => clk90,
      Q => current_input_address(8)
    );
  current_input_address_9 : FDC
    port map (
      D => next_addr(9),
      CLR => reset_INV,
      C => clk90,
      Q => current_input_address(9)
    );
  current_burst_done : FDC
    port map (
      D => next_burst_done,
      CLR => reset_INV,
      C => clk90,
      Q => current_burst_done_10
    );
  current_reg3_1 : FDC
    port map (
      D => next_reg3(1),
      CLR => reset_INV,
      C => clk90,
      Q => current_reg3(1)
    );
  current_reg3_0 : FDC
    port map (
      D => next_reg3(2),
      CLR => reset_INV,
      C => clk90,
      Q => current_reg3(0)
    );
  current_reg1_11 : FDC
    port map (
      D => N1,
      CLR => reset_INV,
      C => clk90,
      Q => current_reg1_11_Q
    );
  current_reg1_8 : FDC
    port map (
      D => N1,
      CLR => reset_INV,
      C => clk90,
      Q => current_reg1_8_Q
    );
  current_reg1_7 : FDC
    port map (
      D => N1,
      CLR => reset_INV,
      C => clk90,
      Q => current_reg1_7_Q
    );
  current_reg1_0 : FDC
    port map (
      D => N1,
      CLR => reset_INV,
      C => clk90,
      Q => current_reg1_0_Q
    );
  current_reg2_7 : FDC
    port map (
      D => N1,
      CLR => reset_INV,
      C => clk90,
      Q => current_reg2_7_Q
    );
  current_reg2_2 : FDC
    port map (
      D => N1,
      CLR => reset_INV,
      C => clk90,
      Q => current_reg2_2_Q
    );
  current_reg2_1 : FDC
    port map (
      D => N1,
      CLR => reset_INV,
      C => clk90,
      Q => current_reg2_1_Q
    );
  current_reg2_0 : FDC
    port map (
      D => N1,
      CLR => reset_INV,
      C => clk90,
      Q => current_reg2_0_Q
    );
  intern_reg1_10 : FDCE
    port map (
      D => intern_reg0(10),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(10)
    );
  intern_reg1_11 : FDCE
    port map (
      D => intern_reg0(11),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(11)
    );
  intern_reg1_12 : FDCE
    port map (
      D => intern_reg0(12),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(12)
    );
  intern_reg1_13 : FDCE
    port map (
      D => intern_reg0(13),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(13)
    );
  intern_reg1_14 : FDCE
    port map (
      D => intern_reg0(14),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(14)
    );
  intern_reg1_15 : FDCE
    port map (
      D => intern_reg0(15),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(15)
    );
  intern_reg1_16 : FDCE
    port map (
      D => intern_reg0(16),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(16)
    );
  intern_reg1_17 : FDCE
    port map (
      D => intern_reg0(17),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(17)
    );
  intern_reg1_18 : FDCE
    port map (
      D => intern_reg0(18),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(18)
    );
  intern_reg1_19 : FDCE
    port map (
      D => intern_reg0(19),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(19)
    );
  intern_reg1_20 : FDCE
    port map (
      D => intern_reg0(20),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(20)
    );
  intern_reg1_21 : FDCE
    port map (
      D => intern_reg0(21),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(21)
    );
  intern_reg1_22 : FDCE
    port map (
      D => intern_reg0(22),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(22)
    );
  intern_reg1_23 : FDCE
    port map (
      D => intern_reg0(23),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(23)
    );
  intern_reg1_24 : FDCE
    port map (
      D => intern_reg0(24),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(24)
    );
  intern_reg1_25 : FDCE
    port map (
      D => intern_reg0(25),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(25)
    );
  intern_reg1_26 : FDCE
    port map (
      D => intern_reg0(26),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(26)
    );
  intern_reg1_27 : FDCE
    port map (
      D => intern_reg0(27),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(27)
    );
  intern_reg1_28 : FDCE
    port map (
      D => intern_reg0(28),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(28)
    );
  intern_reg1_29 : FDCE
    port map (
      D => intern_reg0(29),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(29)
    );
  intern_reg1_30 : FDCE
    port map (
      D => intern_reg0(30),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(30)
    );
  intern_reg1_31 : FDCE
    port map (
      D => intern_reg0(31),
      CE => NlwRenamedSignal_TP2,
      CLR => reset_INV,
      C => clk90,
      Q => intern_reg1(31)
    );
  user_input_address_2 : FDC
    port map (
      D => current_input_address(2),
      CLR => reset_INV,
      C => clk,
      Q => user_input_address(2)
    );
  user_input_address_3 : FDC
    port map (
      D => current_input_address(3),
      CLR => reset_INV,
      C => clk,
      Q => user_input_address(3)
    );
  user_input_address_4 : FDC
    port map (
      D => current_input_address(4),
      CLR => reset_INV,
      C => clk,
      Q => user_input_address(4)
    );
  user_input_address_5 : FDC
    port map (
      D => current_input_address(5),
      CLR => reset_INV,
      C => clk,
      Q => user_input_address(5)
    );
  user_input_address_6 : FDC
    port map (
      D => current_input_address(6),
      CLR => reset_INV,
      C => clk,
      Q => user_input_address(6)
    );
  user_input_address_7 : FDC
    port map (
      D => current_input_address(7),
      CLR => reset_INV,
      C => clk,
      Q => user_input_address(7)
    );
  user_input_address_8 : FDC
    port map (
      D => current_input_address(8),
      CLR => reset_INV,
      C => clk,
      Q => user_input_address(8)
    );
  user_input_address_9 : FDC
    port map (
      D => current_input_address(9),
      CLR => reset_INV,
      C => clk,
      Q => user_input_address(9)
    );
  burst_done_0 : FDC
    port map (
      D => current_burst_done_10,
      CLR => reset_INV,
      C => clk,
      Q => burst_done
    );
  user_command_register_1 : FDC
    port map (
      D => current_reg3(1),
      CLR => reset_INV,
      C => clk,
      Q => user_command_register(1)
    );
  user_command_register_0 : FDC
    port map (
      D => current_reg3(0),
      CLR => reset_INV,
      C => clk,
      Q => user_command_register(2)
    );
  user_config_register1_11 : FDC
    port map (
      D => current_reg1_11_Q,
      CLR => reset_INV,
      C => clk,
      Q => user_config_register1(1)
    );
  user_config_register1_8 : FDC
    port map (
      D => current_reg1_8_Q,
      CLR => reset_INV,
      C => clk,
      Q => user_config_register1(4)
    );
  user_config_register1_7 : FDC
    port map (
      D => current_reg1_7_Q,
      CLR => reset_INV,
      C => clk,
      Q => user_config_register1(5)
    );
  user_config_register1_0 : FDC
    port map (
      D => current_reg1_0_Q,
      CLR => reset_INV,
      C => clk,
      Q => user_config_register1(12)
    );
  user_config_register2_7 : FDC
    port map (
      D => current_reg2_7_Q,
      CLR => reset_INV,
      C => clk,
      Q => user_config_register2(2)
    );
  user_config_register2_2 : FDC
    port map (
      D => current_reg2_2_Q,
      CLR => reset_INV,
      C => clk,
      Q => user_config_register2(7)
    );
  user_config_register2_1 : FDC
    port map (
      D => current_reg2_1_Q,
      CLR => reset_INV,
      C => clk,
      Q => user_config_register2(8)
    );
  user_config_register2_0 : FDC
    port map (
      D => current_reg2_0_Q,
      CLR => reset_INV,
      C => clk,
      Q => user_config_register2(9)
    );
  current_input_data_0 : FDC
    port map (
      D => next_input_data_0_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(0)
    );
  current_input_data_1 : FDC
    port map (
      D => next_input_data_1_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(1)
    );
  current_input_data_2 : FDC
    port map (
      D => next_input_data_19_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(2)
    );
  current_input_data_3 : FDC
    port map (
      D => next_input_data_12_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(3)
    );
  current_input_data_4 : FDC
    port map (
      D => next_input_data_12_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(4)
    );
  current_input_data_5 : FDC
    port map (
      D => next_input_data_19_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(5)
    );
  current_input_data_6 : FDC
    port map (
      D => next_input_data_1_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(6)
    );
  current_input_data_7 : FDC
    port map (
      D => next_input_data_0_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(7)
    );
  current_input_data_8 : FDC
    port map (
      D => next_input_data_0_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(8)
    );
  current_input_data_9 : FDC
    port map (
      D => next_input_data_14_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(9)
    );
  current_input_data_10 : FDC
    port map (
      D => next_input_data_10_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(10)
    );
  current_input_data_11 : FDC
    port map (
      D => next_input_data_11_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(11)
    );
  current_input_data_12 : FDC
    port map (
      D => next_input_data_12_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(12)
    );
  current_input_data_13 : FDC
    port map (
      D => next_input_data_10_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(13)
    );
  current_input_data_14 : FDC
    port map (
      D => next_input_data_14_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(14)
    );
  current_input_data_15 : FDC
    port map (
      D => next_input_data_15_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(15)
    );
  current_input_data_16 : FDC
    port map (
      D => next_input_data_14_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(16)
    );
  current_input_data_17 : FDC
    port map (
      D => next_input_data_15_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(17)
    );
  current_input_data_18 : FDC
    port map (
      D => next_input_data_12_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(18)
    );
  current_input_data_19 : FDC
    port map (
      D => next_input_data_19_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(19)
    );
  current_input_data_20 : FDC
    port map (
      D => next_input_data_10_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(20)
    );
  current_input_data_21 : FDC
    port map (
      D => next_input_data_11_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(21)
    );
  current_input_data_22 : FDC
    port map (
      D => next_input_data_0_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(22)
    );
  current_input_data_23 : FDC
    port map (
      D => next_input_data_1_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(23)
    );
  current_input_data_24 : FDC
    port map (
      D => next_input_data_14_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(24)
    );
  current_input_data_25 : FDC
    port map (
      D => next_input_data_15_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(25)
    );
  current_input_data_26 : FDC
    port map (
      D => next_input_data_11_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(26)
    );
  current_input_data_27 : FDC
    port map (
      D => next_input_data_19_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(27)
    );
  current_input_data_28 : FDC
    port map (
      D => next_input_data_10_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(28)
    );
  current_input_data_29 : FDC
    port map (
      D => next_input_data_11_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(29)
    );
  current_input_data_30 : FDC
    port map (
      D => next_input_data_15_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(30)
    );
  current_input_data_31 : FDC
    port map (
      D => next_input_data_1_Q,
      CLR => reset_INV,
      C => clk90,
      Q => current_input_data(31)
    );
  user_input_data_0 : FDCE
    port map (
      D => current_input_data(0),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(0)
    );
  user_input_data_1 : FDCE
    port map (
      D => current_input_data(1),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(1)
    );
  user_input_data_2 : FDCE
    port map (
      D => current_input_data(2),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(2)
    );
  user_input_data_3 : FDCE
    port map (
      D => current_input_data(3),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(3)
    );
  user_input_data_4 : FDCE
    port map (
      D => current_input_data(4),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(4)
    );
  user_input_data_5 : FDCE
    port map (
      D => current_input_data(5),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(5)
    );
  user_input_data_6 : FDCE
    port map (
      D => current_input_data(6),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(6)
    );
  user_input_data_7 : FDCE
    port map (
      D => current_input_data(7),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(7)
    );
  user_input_data_8 : FDCE
    port map (
      D => current_input_data(8),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(8)
    );
  user_input_data_9 : FDCE
    port map (
      D => current_input_data(9),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(9)
    );
  user_input_data_10 : FDCE
    port map (
      D => current_input_data(10),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(10)
    );
  user_input_data_11 : FDCE
    port map (
      D => current_input_data(11),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(11)
    );
  user_input_data_12 : FDCE
    port map (
      D => current_input_data(12),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(12)
    );
  user_input_data_13 : FDCE
    port map (
      D => current_input_data(13),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(13)
    );
  user_input_data_14 : FDCE
    port map (
      D => current_input_data(14),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(14)
    );
  user_input_data_15 : FDCE
    port map (
      D => current_input_data(15),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(15)
    );
  user_input_data_16 : FDCE
    port map (
      D => current_input_data(16),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(16)
    );
  user_input_data_17 : FDCE
    port map (
      D => current_input_data(17),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(17)
    );
  user_input_data_18 : FDCE
    port map (
      D => current_input_data(18),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(18)
    );
  user_input_data_19 : FDCE
    port map (
      D => current_input_data(19),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(19)
    );
  user_input_data_20 : FDCE
    port map (
      D => current_input_data(20),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(20)
    );
  user_input_data_21 : FDCE
    port map (
      D => current_input_data(21),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(21)
    );
  user_input_data_22 : FDCE
    port map (
      D => current_input_data(22),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(22)
    );
  user_input_data_23 : FDCE
    port map (
      D => current_input_data(23),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(23)
    );
  user_input_data_24 : FDCE
    port map (
      D => current_input_data(24),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(24)
    );
  user_input_data_25 : FDCE
    port map (
      D => current_input_data(25),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(25)
    );
  user_input_data_26 : FDCE
    port map (
      D => current_input_data(26),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(26)
    );
  user_input_data_27 : FDCE
    port map (
      D => current_input_data(27),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(27)
    );
  user_input_data_28 : FDCE
    port map (
      D => current_input_data(28),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(28)
    );
  user_input_data_29 : FDCE
    port map (
      D => current_input_data(29),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(29)
    );
  user_input_data_30 : FDCE
    port map (
      D => current_input_data(30),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(30)
    );
  user_input_data_31 : FDCE
    port map (
      D => current_input_data(31),
      CE => Q_n0058,
      CLR => reset_INV,
      C => clk90,
      Q => user_input_data(31)
    );
  test_count_0 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => system_controller_Result_0_sel,
      CLR => reset_INV,
      C => clk,
      Q => test_count(0)
    );
  test_count_1 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => Result(1),
      CLR => reset_INV,
      C => clk,
      Q => test_count(1)
    );
  test_count_2 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => Result(2),
      CLR => reset_INV,
      C => clk,
      Q => test_count(2)
    );
  test_count_3 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => Result(3),
      CLR => reset_INV,
      C => clk,
      Q => test_count(3)
    );
  test_count_4 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => Result(4),
      CLR => reset_INV,
      C => clk,
      Q => test_count(4)
    );
  test_count_5 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => Result(5),
      CLR => reset_INV,
      C => clk,
      Q => test_count(5)
    );
  test_count_6 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => Result(6),
      CLR => reset_INV,
      C => clk,
      Q => test_count(6)
    );
  current_state_FFd3 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state_FFd3_In_13,
      CLR => reset_INV,
      C => clk90,
      Q => current_state_FFd3_7
    );
  current_state_FFd1 : FDC
    generic map(
      INIT => '1'
    )
    port map (
      D => NlwRenamedSig_OI_user_bank_address(0),
      CLR => reset_INV,
      C => clk90,
      Q => current_state_FFd1_11
    );
  current_state_FFd2 : FDP
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state_FFd2_In_12,
      PRE => reset_INV,
      C => clk90,
      Q => current_state_FFd2_4
    );
  current_state_FFd4 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state_FFd4_In,
      CLR => reset_INV,
      C => clk90,
      Q => current_state_FFd4_5
    );
  current_state_FFd5 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state_FFd5_In_14,
      CLR => reset_INV,
      C => clk90,
      Q => current_state_FFd5_6
    );
  current_state_FFd8 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state_FFd8_In_16,
      CLR => reset_INV,
      C => clk90,
      Q => current_state_FFd8_9
    );
  current_state_FFd6 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state_FFd6_In_15,
      CLR => reset_INV,
      C => clk90,
      Q => current_state_FFd6_8
    );
  current_state_FFd7 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state_FFd7_In,
      CLR => reset_INV,
      C => clk90,
      Q => current_state_FFd7_1
    );
  current_state_FFd9 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state_FFd9_In_17,
      CLR => reset_INV,
      C => clk90,
      Q => current_state_FFd9_2
    );
  current_state_FFd10 : FDC
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state_FFd10_In_18,
      CLR => reset_INV,
      C => clk90,
      Q => current_state_FFd10_3
    );
  reset_INV1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => reset,
      O => reset_INV
    );
  system_controller_Result_1_xor11 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => test_count(0),
      I1 => test_count(1),
      O => Result(1)
    );
  system_controller_Result_2_xor11 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => test_count(0),
      I1 => test_count(1),
      I2 => test_count(2),
      O => Result(2)
    );
  system_controller_Result_3_xor11 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => test_count(0),
      I1 => test_count(3),
      I2 => test_count(1),
      I3 => test_count(2),
      O => Result(3)
    );
  current_state_FFd4_In1 : LUT4
    generic map(
      INIT => X"F444"
    )
    port map (
      I0 => NlwRenamedSignal_TP1,
      I1 => current_state_FFd4_5,
      I2 => init_val,
      I3 => current_state_FFd3_7,
      O => current_state_FFd4_In
    );
  system_controller_Result_4_xor11 : MUXF5
    port map (
      I0 => test_count_4_rt_19,
      I1 => N12,
      S => test_count(2),
      O => Result(4)
    );
  system_controller_Result_4_xor11_G : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => test_count(0),
      I1 => test_count(4),
      I2 => test_count(1),
      I3 => test_count(3),
      O => N12
    );
  Ker351 : LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      I0 => test_count(6),
      I1 => N11,
      O => N35
    );
  Ker111 : LUT3
    generic map(
      INIT => X"F4"
    )
    port map (
      I0 => N29,
      I1 => test_count(5),
      I2 => test_count(6),
      O => N111
    );
  TP_port_10_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(20),
      I1 => N111,
      I2 => intern_reg0(20),
      I3 => N35,
      O => TP_port(10)
    );
  TP_port_11_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(21),
      I1 => N111,
      I2 => intern_reg0(21),
      I3 => N35,
      O => TP_port(11)
    );
  TP_port_0_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(10),
      I1 => N111,
      I2 => intern_reg0(10),
      I3 => N35,
      O => TP_port(0)
    );
  TP_port_12_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(22),
      I1 => N111,
      I2 => intern_reg0(22),
      I3 => N35,
      O => TP_port(12)
    );
  TP_port_13_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(23),
      I1 => N111,
      I2 => intern_reg0(23),
      I3 => N35,
      O => TP_port(13)
    );
  TP_port_15_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(25),
      I1 => N111,
      I2 => intern_reg0(25),
      I3 => N35,
      O => TP_port(15)
    );
  TP_port_16_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(26),
      I1 => N111,
      I2 => intern_reg0(26),
      I3 => N35,
      O => TP_port(16)
    );
  TP_port_14_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(24),
      I1 => N111,
      I2 => intern_reg0(24),
      I3 => N35,
      O => TP_port(14)
    );
  TP_port_17_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(27),
      I1 => N111,
      I2 => intern_reg0(27),
      I3 => N35,
      O => TP_port(17)
    );
  TP_port_18_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(28),
      I1 => N111,
      I2 => intern_reg0(28),
      I3 => N35,
      O => TP_port(18)
    );
  TP_port_1_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(11),
      I1 => N111,
      I2 => intern_reg0(11),
      I3 => N35,
      O => TP_port(1)
    );
  TP_port_20_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(30),
      I1 => N111,
      I2 => intern_reg0(30),
      I3 => N35,
      O => TP_port(20)
    );
  TP_port_19_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(29),
      I1 => N111,
      I2 => intern_reg0(29),
      I3 => N35,
      O => TP_port(19)
    );
  TP_port_21_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(31),
      I1 => N111,
      I2 => intern_reg0(31),
      I3 => N35,
      O => TP_port(21)
    );
  TP_port_2_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(12),
      I1 => N111,
      I2 => intern_reg0(12),
      I3 => N35,
      O => TP_port(2)
    );
  TP_port_4_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(14),
      I1 => N111,
      I2 => intern_reg0(14),
      I3 => N35,
      O => TP_port(4)
    );
  TP_port_5_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(15),
      I1 => N111,
      I2 => intern_reg0(15),
      I3 => N35,
      O => TP_port(5)
    );
  TP_port_3_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(13),
      I1 => N111,
      I2 => intern_reg0(13),
      I3 => N35,
      O => TP_port(3)
    );
  TP_port_6_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(16),
      I1 => N111,
      I2 => intern_reg0(16),
      I3 => N35,
      O => TP_port(6)
    );
  TP_port_9_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(19),
      I1 => N111,
      I2 => intern_reg0(19),
      I3 => N35,
      O => TP_port(9)
    );
  TP_port_7_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(17),
      I1 => N111,
      I2 => intern_reg0(17),
      I3 => N35,
      O => TP_port(7)
    );
  TP_port_8_1 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => intern_reg1(18),
      I1 => N111,
      I2 => intern_reg0(18),
      I3 => N35,
      O => TP_port(8)
    );
  system_controller_Result_0_sel1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => test_count(0),
      O => system_controller_Result_0_sel
    );
  next_burst_done1 : LUT4
    generic map(
      INIT => X"0302"
    )
    port map (
      I0 => current_state_FFd6_8,
      I1 => N27,
      I2 => loop_count(1),
      I3 => current_state_FFd10_3,
      O => next_burst_done
    );
  system_controller_Result_6_xor11 : LUT4
    generic map(
      INIT => X"BA10"
    )
    port map (
      I0 => test_count(6),
      I1 => N29,
      I2 => test_count(5),
      I3 => N11,
      O => Result(6)
    );
  Ker29_SW0 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => test_count(1),
      I1 => test_count(0),
      O => N271
    );
  Ker29 : LUT4
    generic map(
      INIT => X"7FFF"
    )
    port map (
      I0 => test_count(4),
      I1 => test_count(3),
      I2 => test_count(2),
      I3 => N271,
      O => N29
    );
  Ker1_SW0 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => test_count(2),
      I1 => test_count(1),
      I2 => test_count(0),
      O => N291
    );
  Ker1 : LUT4
    generic map(
      INIT => X"7FFF"
    )
    port map (
      I0 => test_count(5),
      I1 => test_count(4),
      I2 => test_count(3),
      I3 => N291,
      O => N11
    );
  Ker201 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => loop_count(0),
      I1 => loop_count(1),
      O => N20
    );
  next_reg3_1_SW0 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => current_state_FFd8_9,
      I1 => current_state_FFd3_7,
      O => N32
    );
  next_reg3_1_Q : LUT4
    generic map(
      INIT => X"FFBA"
    )
    port map (
      I0 => N32,
      I1 => N27,
      I2 => current_state_FFd10_3,
      I3 => current_state_FFd9_2,
      O => next_reg3(1)
    );
  current_state_FFd2_In_SW0 : LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      I0 => loop_count(0),
      I1 => current_state_FFd2_4,
      O => N34
    );
  current_state_FFd2_In : LUT4
    generic map(
      INIT => X"FF02"
    )
    port map (
      I0 => N34,
      I1 => N27,
      I2 => loop_count(1),
      I3 => current_state_FFd1_11,
      O => current_state_FFd2_In_12
    );
  next_reg3_2_Q : LUT4
    generic map(
      INIT => X"FF54"
    )
    port map (
      I0 => N27,
      I1 => current_state_FFd10_3,
      I2 => current_state_FFd6_8,
      I3 => N361,
      O => next_reg3(2)
    );
  system_controller_Result_5_xor1_SW0 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => test_count(3),
      I1 => test_count(2),
      I2 => test_count(1),
      I3 => test_count(0),
      O => N38
    );
  system_controller_Result_5_xor1 : LUT4
    generic map(
      INIT => X"EA40"
    )
    port map (
      I0 => test_count(5),
      I1 => test_count(4),
      I2 => N38,
      I3 => N29,
      O => Result(5)
    );
  Ker151 : LUT3
    generic map(
      INIT => X"F4"
    )
    port map (
      I0 => loop_count(2),
      I1 => current_state_FFd5_6,
      I2 => current_state_FFd9_2,
      O => N15
    );
  Ker372 : LUT3
    generic map(
      INIT => X"7F"
    )
    port map (
      I0 => col_addr(1),
      I1 => col_addr(0),
      I2 => col_addr(2),
      O => N37
    );
  current_state_FFd8_In_SW0 : LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      I0 => NlwRenamedSignal_TP1,
      I1 => current_state_FFd8_9,
      O => N42
    );
  current_state_FFd8_In : LUT4
    generic map(
      INIT => X"FF8A"
    )
    port map (
      I0 => current_state_FFd7_1,
      I1 => loop_count(7),
      I2 => N71,
      I3 => N42,
      O => current_state_FFd8_In_16
    );
  current_state_FFd3_In_SW0 : LUT4
    generic map(
      INIT => X"F444"
    )
    port map (
      I0 => init_val,
      I1 => current_state_FFd3_7,
      I2 => current_state_FFd2_4,
      I3 => loop_count(0),
      O => N44
    );
  current_state_FFd3_In_SW1 : LUT3
    generic map(
      INIT => X"F4"
    )
    port map (
      I0 => init_val,
      I1 => current_state_FFd3_7,
      I2 => current_state_FFd2_4,
      O => N45
    );
  current_state_FFd3_In : LUT4
    generic map(
      INIT => X"ABA8"
    )
    port map (
      I0 => N45,
      I1 => N27,
      I2 => loop_count(1),
      I3 => N44,
      O => current_state_FFd3_In_13
    );
  Ker132 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => current_state_FFd5_6,
      I1 => current_state_FFd9_2,
      O => N13
    );
  Ker1311 : LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      I0 => loop_count(0),
      I1 => loop_count(1),
      O => N131
    );
  Ker81 : LUT3
    generic map(
      INIT => X"7F"
    )
    port map (
      I0 => loop_count(2),
      I1 => loop_count(1),
      I2 => loop_count(0),
      O => N8
    );
  next_input_data_0_1 : LUT4
    generic map(
      INIT => X"02AA"
    )
    port map (
      I0 => current_state_FFd5_6,
      I1 => N131,
      I2 => N39,
      I3 => N36,
      O => next_input_data_0_Q
    );
  next_input_data_10_1 : LUT4
    generic map(
      INIT => X"02AA"
    )
    port map (
      I0 => current_state_FFd5_6,
      I1 => N39,
      I2 => loop_count(0),
      I3 => N36,
      O => next_input_data_10_Q
    );
  Ker3211 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => current_state_FFd7_1,
      I1 => current_state_FFd10_3,
      I2 => current_state_FFd2_4,
      I3 => current_state_FFd6_8,
      O => N321
    );
  current_state_FFd7_In1 : LUT4
    generic map(
      INIT => X"FF20"
    )
    port map (
      I0 => N27,
      I1 => NlwRenamedSignal_TP1,
      I2 => current_state_FFd6_8,
      I3 => N371,
      O => current_state_FFd7_In
    );
  Ker30_SW0 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => col_addr(0),
      I1 => col_addr(4),
      O => N58
    );
  Ker30 : LUT4
    generic map(
      INIT => X"7FFF"
    )
    port map (
      I0 => col_addr(3),
      I1 => col_addr(2),
      I2 => col_addr(1),
      I3 => N58,
      O => N30
    );
  current_state_FFd6_In_SW0 : LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      I0 => NlwRenamedSignal_TP1,
      I1 => N27,
      O => N60
    );
  current_state_FFd6_In : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0043,
      I1 => current_state_FFd5_6,
      I2 => current_state_FFd6_8,
      I3 => N60,
      O => current_state_FFd6_In_15
    );
  current_state_FFd10_In_SW0 : LUT3
    generic map(
      INIT => X"FB"
    )
    port map (
      I0 => NlwRenamedSignal_TP2,
      I1 => N27,
      I2 => NlwRenamedSignal_TP1,
      O => N62
    );
  current_state_FFd10_In : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => Q_n0043,
      I1 => current_state_FFd9_2,
      I2 => current_state_FFd10_3,
      I3 => N62,
      O => current_state_FFd10_In_18
    );
  Q_n00580 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => current_state_FFd5_6,
      I1 => current_state_FFd4_5,
      O => N64
    );
  Q_n005836 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => NlwRenamedSignal_TP1,
      I1 => current_state_FFd9_2,
      I2 => current_state_FFd3_7,
      I3 => current_state_FFd8_9,
      O => N96
    );
  Q_n005837 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => N84,
      I1 => N96,
      O => Q_n0058
    );
  Q_n00431 : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => loop_count(0),
      I1 => loop_count(2),
      I2 => loop_count(1),
      I3 => N311,
      O => Q_n0043
    );
  current_state_FFd5_In_SW0 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => current_state_FFd4_5,
      I1 => NlwRenamedSignal_TP1,
      O => N1011
    );
  current_state_FFd5_In : LUT4
    generic map(
      INIT => X"FF2A"
    )
    port map (
      I0 => current_state_FFd5_6,
      I1 => N27,
      I2 => Q_n0043,
      I3 => N1011,
      O => current_state_FFd5_In_14
    );
  current_state_FFd9_In_SW0 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => current_state_FFd8_9,
      I1 => NlwRenamedSignal_TP1,
      O => N103
    );
  current_state_FFd9_In : LUT4
    generic map(
      INIT => X"FF2A"
    )
    port map (
      I0 => current_state_FFd9_2,
      I1 => N27,
      I2 => Q_n0043,
      I3 => N103,
      O => current_state_FFd9_In_17
    );
  Ker10_SW0 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => loop_count(2),
      I1 => loop_count(1),
      I2 => loop_count(0),
      O => N105
    );
  Ker10 : LUT4
    generic map(
      INIT => X"7FFF"
    )
    port map (
      I0 => loop_count(4),
      I1 => loop_count(5),
      I2 => loop_count(3),
      I3 => N105,
      O => N10
    );
  next_input_data_14_1 : LUT4
    generic map(
      INIT => X"AA08"
    )
    port map (
      I0 => current_state_FFd5_6,
      I1 => loop_count(0),
      I2 => N39,
      I3 => Q_n0043,
      O => next_input_data_14_Q
    );
  next_input_data_12_1 : LUT4
    generic map(
      INIT => X"AA08"
    )
    port map (
      I0 => current_state_FFd5_6,
      I1 => N20,
      I2 => N39,
      I3 => Q_n0043,
      O => next_input_data_12_Q
    );
  Q_n0056_3_1 : LUT4
    generic map(
      INIT => X"BA90"
    )
    port map (
      I0 => loop_count(3),
      I1 => N8,
      I2 => N18,
      I3 => N121,
      O => Q_n0056(3)
    );
  Q_n0056_6_1 : LUT4
    generic map(
      INIT => X"BA90"
    )
    port map (
      I0 => loop_count(6),
      I1 => N10,
      I2 => N18,
      I3 => N121,
      O => Q_n0056(6)
    );
  Ker40_SW0 : LUT4
    generic map(
      INIT => X"F8C8"
    )
    port map (
      I0 => current_state_FFd6_8,
      I1 => NlwRenamedSignal_TP1,
      I2 => current_state_FFd10_3,
      I3 => NlwRenamedSignal_TP2,
      O => N112
    );
  Ker241 : LUT3
    generic map(
      INIT => X"BF"
    )
    port map (
      I0 => N23,
      I1 => col_addr(4),
      I2 => col_addr(5),
      O => N24
    );
  Q_n0057_0_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => col_addr(0),
      I1 => N4,
      I2 => N14,
      O => Q_n0057(0)
    );
  Ker231 : LUT4
    generic map(
      INIT => X"7FFF"
    )
    port map (
      I0 => col_addr(2),
      I1 => col_addr(3),
      I2 => col_addr(0),
      I3 => col_addr(1),
      O => N23
    );
  Q_n0056_0_42 : LUT4
    generic map(
      INIT => X"DDD8"
    )
    port map (
      I0 => loop_count(0),
      I1 => N101,
      I2 => N18,
      I3 => N163,
      O => Q_n0056(0)
    );
  Ker391 : LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      I0 => loop_count(2),
      I1 => N311,
      O => N39
    );
  Q_n0057_3_Q : LUT4
    generic map(
      INIT => X"FAD8"
    )
    port map (
      I0 => col_addr(3),
      I1 => N14,
      I2 => N219,
      I3 => N220,
      O => Q_n0057(3)
    );
  Q_n0057_1_SW0 : LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      I0 => N4,
      I1 => col_addr(0),
      O => N222
    );
  Q_n0057_1_Q : LUT4
    generic map(
      INIT => X"AF27"
    )
    port map (
      I0 => col_addr(1),
      I1 => N223,
      I2 => N222,
      I3 => N14,
      O => Q_n0057(1)
    );
  Ker27_SW0 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => loop_count(5),
      I1 => loop_count(3),
      I2 => loop_count(2),
      O => N225
    );
  Ker27 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => loop_count(7),
      I1 => loop_count(4),
      I2 => loop_count(6),
      I3 => N225,
      O => N27
    );
  Q_n0057_6_SW0 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => col_addr(5),
      I1 => N30,
      I2 => N4,
      O => N229
    );
  Q_n0057_6_Q : LUT4
    generic map(
      INIT => X"FAD8"
    )
    port map (
      I0 => col_addr(6),
      I1 => N14,
      I2 => N229,
      I3 => N230,
      O => Q_n0057(6)
    );
  Q_n0057_2_SW0 : LUT3
    generic map(
      INIT => X"7F"
    )
    port map (
      I0 => col_addr(1),
      I1 => col_addr(0),
      I2 => N4,
      O => N232
    );
  Q_n0057_2_SW1 : LUT3
    generic map(
      INIT => X"D5"
    )
    port map (
      I0 => N15,
      I1 => col_addr(1),
      I2 => col_addr(0),
      O => N233
    );
  Q_n0057_2_Q : LUT4
    generic map(
      INIT => X"AF27"
    )
    port map (
      I0 => col_addr(2),
      I1 => N233,
      I2 => N232,
      I3 => N14,
      O => Q_n0057(2)
    );
  Ker45 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => current_state_FFd5_6,
      I1 => loop_count(2),
      I2 => N311,
      O => Ker4_map506
    );
  Ker417 : LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      I0 => loop_count(0),
      I1 => loop_count(1),
      O => Ker4_map511
    );
  Q_n0057_4_SW0 : LUT4
    generic map(
      INIT => X"AA08"
    )
    port map (
      I0 => N23,
      I1 => current_state_FFd5_6,
      I2 => loop_count(2),
      I3 => current_state_FFd9_2,
      O => N254
    );
  Q_n0057_4_Q : LUT4
    generic map(
      INIT => X"FAD8"
    )
    port map (
      I0 => col_addr(4),
      I1 => N14,
      I2 => N341,
      I3 => N254,
      O => Q_n0057(4)
    );
  Q_n0057_7_20 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => col_addr(6),
      I1 => col_addr(7),
      I2 => col_addr(5),
      O => N274
    );
  Q_n0057_7_29 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => N274,
      I1 => N30,
      I2 => N4,
      O => N283
    );
  Q_n0057_7_35 : LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      I0 => col_addr(7),
      I1 => N14,
      I2 => N283,
      I3 => N260,
      O => Q_n0057(7)
    );
  N145 : LUT3
    generic map(
      INIT => X"FB"
    )
    port map (
      I0 => loop_count(0),
      I1 => loop_count(1),
      I2 => N27,
      O => N14_map527
    );
  N1418 : LUT3
    generic map(
      INIT => X"01"
    )
    port map (
      I0 => current_state_FFd9_2,
      I1 => current_state_FFd4_5,
      I2 => current_state_FFd5_6,
      O => N14_map533
    );
  N1434 : LUT3
    generic map(
      INIT => X"AB"
    )
    port map (
      I0 => loop_count(0),
      I1 => loop_count(1),
      I2 => loop_count(2),
      O => N14_map538
    );
  N1435 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => loop_count(1),
      I1 => loop_count(2),
      O => N14_map539
    );
  N1452 : LUT4
    generic map(
      INIT => X"AA8A"
    )
    port map (
      I0 => current_state_FFd5_6,
      I1 => N14_map539,
      I2 => N311,
      I3 => N14_map538,
      O => N14_map543
    );
  N1459 : LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      I0 => N14_map533,
      I1 => current_state_FFd9_2,
      I2 => N14_map527,
      I3 => N14_map543,
      O => N14
    );
  Ker3411 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => col_addr(3),
      I1 => N37,
      I2 => N4,
      O => N341
    );
  Q_n0056_7_SW0 : LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      I0 => N10,
      I1 => loop_count(6),
      O => N337
    );
  Q_n0056_7_Q : LUT4
    generic map(
      INIT => X"EA60"
    )
    port map (
      I0 => loop_count(7),
      I1 => N337,
      I2 => N18,
      I3 => N121,
      O => Q_n0056(7)
    );
  Q_n0056_4_Q : LUT4
    generic map(
      INIT => X"EA60"
    )
    port map (
      I0 => loop_count(4),
      I1 => N339,
      I2 => N18,
      I3 => N121,
      O => Q_n0056(4)
    );
  Q_n0056_5_SW0 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => loop_count(4),
      I1 => N8,
      I2 => loop_count(3),
      O => N3411
    );
  Q_n0056_5_Q : LUT4
    generic map(
      INIT => X"EA60"
    )
    port map (
      I0 => loop_count(5),
      I1 => N3411,
      I2 => N18,
      I3 => N121,
      O => Q_n0056(5)
    );
  Ker3711 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => current_state_FFd7_1,
      I1 => loop_count(7),
      I2 => N71,
      O => N371
    );
  Ker181 : LUT4
    generic map(
      INIT => X"FF54"
    )
    port map (
      I0 => N27,
      I1 => current_state_FFd6_8,
      I2 => current_state_FFd10_3,
      I3 => N371,
      O => N18
    );
  Ker71_SW0 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => loop_count(3),
      I1 => loop_count(2),
      O => N345
    );
  Ker71 : LUT4
    generic map(
      INIT => X"777F"
    )
    port map (
      I0 => loop_count(5),
      I1 => loop_count(6),
      I2 => N345,
      I3 => loop_count(4),
      O => N71
    );
  next_addr_9_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => col_addr(7),
      I1 => N0,
      O => next_addr(9)
    );
  next_addr_7_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => col_addr(5),
      I1 => N0,
      O => next_addr(7)
    );
  next_addr_8_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => col_addr(6),
      I1 => N0,
      O => next_addr(8)
    );
  next_addr_4_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => col_addr(2),
      I1 => N0,
      O => next_addr(4)
    );
  next_addr_6_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => col_addr(4),
      I1 => N0,
      O => next_addr(6)
    );
  next_addr_3_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => col_addr(1),
      I1 => N0,
      O => next_addr(3)
    );
  next_addr_5_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => col_addr(3),
      I1 => N0,
      O => next_addr(5)
    );
  next_addr_2_1 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => col_addr(0),
      I1 => N0,
      O => next_addr(2)
    );
  Ker361 : LUT3
    generic map(
      INIT => X"EF"
    )
    port map (
      I0 => loop_count(1),
      I1 => loop_count(0),
      I2 => N311,
      O => N36
    );
  Ker311_SW0 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => loop_count(5),
      I1 => loop_count(3),
      O => N360
    );
  Ker311 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => loop_count(7),
      I1 => loop_count(4),
      I2 => loop_count(6),
      I3 => N360,
      O => N311
    );
  test_count_4_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => test_count(4),
      O => test_count_4_rt_19
    );
  Ker0_SW0_SW0 : LUT4
    generic map(
      INIT => X"BA8A"
    )
    port map (
      I0 => current_state_FFd8_9,
      I1 => loop_count(2),
      I2 => N311,
      I3 => N358,
      O => N362
    );
  Ker419 : LUT4
    generic map(
      INIT => X"AA08"
    )
    port map (
      I0 => Ker4_map511,
      I1 => current_state_FFd9_2,
      I2 => N27,
      I3 => Ker4_map506,
      O => N4
    );
  Q_n0057_7_4 : LUT3
    generic map(
      INIT => X"D0"
    )
    port map (
      I0 => col_addr(6),
      I1 => N24,
      I2 => N4,
      O => N260
    );
  Q_n0057_3_SW0 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => col_addr(1),
      I1 => col_addr(0),
      I2 => col_addr(2),
      I3 => N4,
      O => N219
    );
  Q_n0057_6_SW1 : LUT4
    generic map(
      INIT => X"F070"
    )
    port map (
      I0 => col_addr(5),
      I1 => col_addr(4),
      I2 => N4,
      I3 => N23,
      O => N230
    );
  Q_n0056_2_49 : LUT4
    generic map(
      INIT => X"FEFC"
    )
    port map (
      I0 => loop_count(2),
      I1 => N197,
      I2 => N211,
      I3 => N367,
      O => Q_n0056(2)
    );
  Q_n0056_2_44 : LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      I0 => loop_count(0),
      I1 => loop_count(2),
      I2 => N18,
      I3 => loop_count(1),
      O => N211
    );
  Q_n005821 : LUT4
    generic map(
      INIT => X"F302"
    )
    port map (
      I0 => current_state_FFd6_8,
      I1 => current_state_FFd10_3,
      I2 => N27,
      I3 => N64,
      O => N84
    );
  next_input_data_11_1 : LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      I0 => loop_count(2),
      I1 => current_state_FFd5_6,
      I2 => N311,
      I3 => N20,
      O => next_input_data_11_Q
    );
  next_input_data_1_1 : LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      I0 => loop_count(2),
      I1 => current_state_FFd5_6,
      I2 => N311,
      I3 => loop_count(0),
      O => next_input_data_1_Q
    );
  Ker121 : LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      I0 => N13,
      I1 => N27,
      I2 => N112,
      I3 => N321,
      O => N121
    );
  next_input_data_15_1 : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => loop_count(2),
      I1 => N311,
      I2 => N20,
      I3 => current_state_FFd5_6,
      O => next_input_data_15_Q
    );
  next_input_data_19_1 : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => loop_count(2),
      I1 => N311,
      I2 => loop_count(0),
      I3 => current_state_FFd5_6,
      O => next_input_data_19_Q
    );
  Ker0_SW1 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => current_state_FFd5_6,
      I1 => current_state_FFd9_2,
      I2 => current_state_FFd8_9,
      O => N358
    );
  next_reg3_2_SW0 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => current_state_FFd5_6,
      I1 => current_state_FFd9_2,
      I2 => current_state_FFd4_5,
      I3 => current_state_FFd8_9,
      O => N361
    );
  Q_n0057_1_SW1 : LUT4
    generic map(
      INIT => X"FF23"
    )
    port map (
      I0 => loop_count(2),
      I1 => current_state_FFd9_2,
      I2 => current_state_FFd5_6,
      I3 => col_addr(0),
      O => N223
    );
  Q_n0056_4_SW0 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => loop_count(2),
      I1 => loop_count(1),
      I2 => loop_count(0),
      I3 => loop_count(3),
      O => N339
    );
  Q_n0056_2_23 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => N13,
      I1 => loop_count(0),
      I2 => loop_count(1),
      I3 => N311,
      O => N197
    );
  Q_n0057_3_SW1 : LUT4
    generic map(
      INIT => X"4CCC"
    )
    port map (
      I0 => col_addr(1),
      I1 => N15,
      I2 => col_addr(0),
      I3 => col_addr(2),
      O => N220
    );
  Ker0_SW0 : LUT4
    generic map(
      INIT => X"FFAB"
    )
    port map (
      I0 => loop_count(1),
      I1 => current_state_FFd9_2,
      I2 => current_state_FFd5_6,
      I3 => loop_count(0),
      O => N369
    );
  Ker0 : LUT4
    generic map(
      INIT => X"FF20"
    )
    port map (
      I0 => loop_count(2),
      I1 => N369,
      I2 => N311,
      I3 => N362,
      O => N0
    );
  Q_n0056_0_16_SW0 : LUT4
    generic map(
      INIT => X"00E0"
    )
    port map (
      I0 => current_state_FFd9_2,
      I1 => current_state_FFd5_6,
      I2 => N311,
      I3 => loop_count(2),
      O => N3711
    );
  Q_n0056_0_16 : LUT4
    generic map(
      INIT => X"FF02"
    )
    port map (
      I0 => current_state_FFd2_4,
      I1 => N27,
      I2 => loop_count(1),
      I3 => N3711,
      O => N163
    );
  Q_n0056_2_14_SW0_SW0 : LUT4
    generic map(
      INIT => X"F444"
    )
    port map (
      I0 => N13,
      I1 => N321,
      I2 => N112,
      I3 => N27,
      O => N373
    );
  Q_n0056_2_14_SW0 : LUT4
    generic map(
      INIT => X"FEF0"
    )
    port map (
      I0 => current_state_FFd9_2,
      I1 => current_state_FFd5_6,
      I2 => N373,
      I3 => N36,
      O => N367
    );
  Q_n0056_1_30 : MUXF5
    port map (
      I0 => N378,
      I1 => N379,
      S => loop_count(1),
      O => Q_n0056(1)
    );
  Q_n0056_1_30_F : LUT4
    generic map(
      INIT => X"AA08"
    )
    port map (
      I0 => loop_count(0),
      I1 => N13,
      I2 => N39,
      I3 => N18,
      O => N378
    );
  Q_n0056_1_30_G : LUT4
    generic map(
      INIT => X"FF54"
    )
    port map (
      I0 => loop_count(0),
      I1 => N13,
      I2 => N18,
      I3 => N101,
      O => N379
    );
  Ker1011 : MUXF5
    port map (
      I0 => N380,
      I1 => N381,
      S => N13,
      O => N101
    );
  Ker1011_F : LUT3
    generic map(
      INIT => X"EA"
    )
    port map (
      I0 => N321,
      I1 => N112,
      I2 => N27,
      O => N380
    );
  Ker1011_G : LUT4
    generic map(
      INIT => X"EAFF"
    )
    port map (
      I0 => loop_count(2),
      I1 => N112,
      I2 => N27,
      I3 => N311,
      O => N381
    );
  Q_n0057_5_Q : MUXF5
    port map (
      I0 => N384,
      I1 => N385,
      S => col_addr(5),
      O => Q_n0057(5)
    );
  Q_n0057_5_F : LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      I0 => N37,
      I1 => col_addr(3),
      I2 => col_addr(4),
      I3 => N4,
      O => N384
    );
  Q_n0057_5_G : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => N4,
      I1 => N30,
      I2 => N14,
      O => N385
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity infrastructure is
  port (
    clk_int : in STD_LOGIC := 'X'; 
    sys_rst : in STD_LOGIC := 'X'; 
    rst_calib1 : in STD_LOGIC := 'X'; 
    delay_sel_val : in STD_LOGIC_VECTOR ( 4 downto 0 ); 
    delay_sel_val1_val : out STD_LOGIC_VECTOR ( 4 downto 0 ) 
  );
end infrastructure;

architecture Structure of infrastructure is
  signal Q_n0004 : STD_LOGIC; 
  signal rst_calib1_r1_20 : STD_LOGIC; 
  signal rst_calib1_r2_21 : STD_LOGIC; 
  signal delay_sel_val1_r : STD_LOGIC_VECTOR ( 4 downto 0 ); 
begin
  delay_sel_val1_r_0 : FDRE
    port map (
      D => delay_sel_val(0),
      R => sys_rst,
      CE => Q_n0004,
      C => clk_int,
      Q => delay_sel_val1_r(0)
    );
  delay_sel_val1_r_1 : FDRE
    port map (
      D => delay_sel_val(1),
      R => sys_rst,
      CE => Q_n0004,
      C => clk_int,
      Q => delay_sel_val1_r(1)
    );
  delay_sel_val1_r_2 : FDRE
    port map (
      D => delay_sel_val(2),
      R => sys_rst,
      CE => Q_n0004,
      C => clk_int,
      Q => delay_sel_val1_r(2)
    );
  delay_sel_val1_r_3 : FDRE
    port map (
      D => delay_sel_val(3),
      R => sys_rst,
      CE => Q_n0004,
      C => clk_int,
      Q => delay_sel_val1_r(3)
    );
  delay_sel_val1_r_4 : FDRE
    port map (
      D => delay_sel_val(4),
      R => sys_rst,
      CE => Q_n0004,
      C => clk_int,
      Q => delay_sel_val1_r(4)
    );
  rst_calib1_r1 : FDR
    port map (
      D => rst_calib1,
      R => sys_rst,
      C => clk_int,
      Q => rst_calib1_r1_20
    );
  rst_calib1_r2 : FDR
    port map (
      D => rst_calib1_r1_20,
      R => sys_rst,
      C => clk_int,
      Q => rst_calib1_r2_21
    );
  delay_sel_val1_4_1 : LUT4
    generic map(
      INIT => X"ABA8"
    )
    port map (
      I0 => delay_sel_val1_r(4),
      I1 => rst_calib1,
      I2 => rst_calib1_r2_21,
      I3 => delay_sel_val(4),
      O => delay_sel_val1_val(4)
    );
  delay_sel_val1_3_1 : LUT4
    generic map(
      INIT => X"ABA8"
    )
    port map (
      I0 => delay_sel_val1_r(3),
      I1 => rst_calib1,
      I2 => rst_calib1_r2_21,
      I3 => delay_sel_val(3),
      O => delay_sel_val1_val(3)
    );
  delay_sel_val1_2_1 : LUT4
    generic map(
      INIT => X"ABA8"
    )
    port map (
      I0 => delay_sel_val1_r(2),
      I1 => rst_calib1,
      I2 => rst_calib1_r2_21,
      I3 => delay_sel_val(2),
      O => delay_sel_val1_val(2)
    );
  delay_sel_val1_1_1 : LUT4
    generic map(
      INIT => X"ABA8"
    )
    port map (
      I0 => delay_sel_val1_r(1),
      I1 => rst_calib1,
      I2 => rst_calib1_r2_21,
      I3 => delay_sel_val(1),
      O => delay_sel_val1_val(1)
    );
  delay_sel_val1_0_1 : LUT4
    generic map(
      INIT => X"ABA8"
    )
    port map (
      I0 => delay_sel_val1_r(0),
      I1 => rst_calib1,
      I2 => rst_calib1_r2_21,
      I3 => delay_sel_val(0),
      O => delay_sel_val1_val(0)
    );
  Q_n00041 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => rst_calib1,
      I1 => rst_calib1_r2_21,
      O => Q_n0004
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity data_write_16bit is
  port (
    reset270_r : in STD_LOGIC := 'X'; 
    reset90_r : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_enable : in STD_LOGIC := 'X'; 
    write_en_val : out STD_LOGIC; 
    user_data_mask : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    user_input_data : in STD_LOGIC_VECTOR ( 31 downto 0 ); 
    write_data_falling : out STD_LOGIC_VECTOR ( 15 downto 0 ); 
    data_mask_f : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
    data_mask_r : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
    write_data_rising : out STD_LOGIC_VECTOR ( 15 downto 0 ) 
  );
end data_write_16bit;

architecture Structure of data_write_16bit is
  signal write_en_P1_23 : STD_LOGIC; 
  signal N2 : STD_LOGIC; 
  signal N3 : STD_LOGIC; 
  signal Mshreg_write_data3_2_24 : STD_LOGIC; 
  signal Mshreg_write_data3_0_25 : STD_LOGIC; 
  signal Mshreg_write_data3_1_26 : STD_LOGIC; 
  signal Mshreg_write_data3_3_27 : STD_LOGIC; 
  signal Mshreg_write_data3_4_28 : STD_LOGIC; 
  signal Mshreg_write_data3_5_29 : STD_LOGIC; 
  signal Mshreg_write_data3_6_30 : STD_LOGIC; 
  signal Mshreg_write_data3_7_31 : STD_LOGIC; 
  signal Mshreg_write_data3_8_32 : STD_LOGIC; 
  signal Mshreg_write_data3_11_33 : STD_LOGIC; 
  signal Mshreg_write_data3_9_34 : STD_LOGIC; 
  signal Mshreg_write_data3_10_35 : STD_LOGIC; 
  signal Mshreg_write_data3_12_36 : STD_LOGIC; 
  signal Mshreg_write_data3_13_37 : STD_LOGIC; 
  signal Mshreg_write_data3_14_38 : STD_LOGIC; 
  signal Mshreg_write_data3_15_39 : STD_LOGIC; 
  signal Mshreg_write_data3_16_40 : STD_LOGIC; 
  signal Mshreg_write_data3_17_41 : STD_LOGIC; 
  signal Mshreg_write_data3_20_42 : STD_LOGIC; 
  signal Mshreg_write_data3_18_43 : STD_LOGIC; 
  signal Mshreg_write_data3_19_44 : STD_LOGIC; 
  signal Mshreg_write_data3_21_45 : STD_LOGIC; 
  signal Mshreg_write_data3_22_46 : STD_LOGIC; 
  signal Mshreg_write_data3_23_47 : STD_LOGIC; 
  signal Mshreg_write_data3_24_48 : STD_LOGIC; 
  signal Mshreg_write_data3_25_49 : STD_LOGIC; 
  signal Mshreg_write_data3_26_50 : STD_LOGIC; 
  signal Mshreg_write_data3_29_51 : STD_LOGIC; 
  signal Mshreg_write_data3_27_52 : STD_LOGIC; 
  signal Mshreg_write_data3_28_53 : STD_LOGIC; 
  signal Mshreg_write_data3_30_54 : STD_LOGIC; 
  signal Mshreg_write_data3_31_55 : STD_LOGIC; 
  signal Mshreg_write_data_m3_0_56 : STD_LOGIC; 
  signal Mshreg_write_data_m3_1_57 : STD_LOGIC; 
  signal Mshreg_write_data_m3_2_58 : STD_LOGIC; 
  signal Mshreg_write_data_m3_3_59 : STD_LOGIC; 
  signal Mshreg_write_data_mask_0_60 : STD_LOGIC; 
  signal Mshreg_write_data_mask_1_61 : STD_LOGIC; 
  signal Mshreg_write_data_0_62 : STD_LOGIC; 
  signal Mshreg_write_data_1_63 : STD_LOGIC; 
  signal Mshreg_write_data_2_64 : STD_LOGIC; 
  signal Mshreg_write_data_3_65 : STD_LOGIC; 
  signal Mshreg_write_data_4_66 : STD_LOGIC; 
  signal Mshreg_write_data_5_67 : STD_LOGIC; 
  signal Mshreg_write_data_6_68 : STD_LOGIC; 
  signal Mshreg_write_data_9_69 : STD_LOGIC; 
  signal Mshreg_write_data_7_70 : STD_LOGIC; 
  signal Mshreg_write_data_8_71 : STD_LOGIC; 
  signal Mshreg_write_data_10_72 : STD_LOGIC; 
  signal Mshreg_write_data_11_73 : STD_LOGIC; 
  signal Mshreg_write_data_12_74 : STD_LOGIC; 
  signal Mshreg_write_data_13_75 : STD_LOGIC; 
  signal Mshreg_write_data_14_76 : STD_LOGIC; 
  signal Mshreg_write_data_15_77 : STD_LOGIC; 
  signal Mshreg_data_mask_r_0_78 : STD_LOGIC; 
  signal Mshreg_data_mask_r_1_79 : STD_LOGIC; 
  signal Mshreg_write_data_rising_0_80 : STD_LOGIC; 
  signal Mshreg_write_data_rising_1_81 : STD_LOGIC; 
  signal Mshreg_write_data_rising_2_82 : STD_LOGIC; 
  signal Mshreg_write_data_rising_3_83 : STD_LOGIC; 
  signal Mshreg_write_data_rising_4_84 : STD_LOGIC; 
  signal Mshreg_write_data_rising_7_85 : STD_LOGIC; 
  signal Mshreg_write_data_rising_5_86 : STD_LOGIC; 
  signal Mshreg_write_data_rising_6_87 : STD_LOGIC; 
  signal Mshreg_write_data_rising_8_88 : STD_LOGIC; 
  signal Mshreg_write_data_rising_9_89 : STD_LOGIC; 
  signal Mshreg_write_data_rising_10_90 : STD_LOGIC; 
  signal Mshreg_write_data_rising_11_91 : STD_LOGIC; 
  signal Mshreg_write_data_rising_12_92 : STD_LOGIC; 
  signal Mshreg_write_data_rising_13_93 : STD_LOGIC; 
  signal Mshreg_write_data_rising_14_94 : STD_LOGIC; 
  signal Mshreg_write_data_rising_15_95 : STD_LOGIC; 
  signal NLW_write_data_mo31_Q_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo30_Q_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo29_Q_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo28_Q_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo27_Q_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo26_Q_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo25_Q_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo24_Q_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo23_Q_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo22_Q_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo21_Q_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo20_Q_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo19_Q_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo18_Q_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo17_Q_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo16_Q_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mas3_Q_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mas2_Q_UNCONNECTED : STD_LOGIC; 
  signal write_data4 : STD_LOGIC_VECTOR ( 15 downto 0 ); 
  signal write_data3 : STD_LOGIC_VECTOR ( 31 downto 0 ); 
  signal write_data_m4 : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal write_data_m3 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  write_en_P1 : FDR
    port map (
      D => write_enable,
      R => reset90_r,
      C => clk90,
      Q => write_en_P1_23
    );
  write_en_val_22 : FDR_1
    port map (
      D => write_en_P1_23,
      R => reset90_r,
      C => clk90,
      Q => write_en_val
    );
  write_data_mo31 : FD
    port map (
      D => write_data3(31),
      C => clk90,
      Q => NLW_write_data_mo31_Q_UNCONNECTED
    );
  write_data_mo30 : FD
    port map (
      D => write_data3(30),
      C => clk90,
      Q => NLW_write_data_mo30_Q_UNCONNECTED
    );
  write_data_mo29 : FD
    port map (
      D => write_data3(29),
      C => clk90,
      Q => NLW_write_data_mo29_Q_UNCONNECTED
    );
  write_data_mo28 : FD
    port map (
      D => write_data3(28),
      C => clk90,
      Q => NLW_write_data_mo28_Q_UNCONNECTED
    );
  write_data_mo27 : FD
    port map (
      D => write_data3(27),
      C => clk90,
      Q => NLW_write_data_mo27_Q_UNCONNECTED
    );
  write_data_mo26 : FD
    port map (
      D => write_data3(26),
      C => clk90,
      Q => NLW_write_data_mo26_Q_UNCONNECTED
    );
  write_data_mo25 : FD
    port map (
      D => write_data3(25),
      C => clk90,
      Q => NLW_write_data_mo25_Q_UNCONNECTED
    );
  write_data_mo24 : FD
    port map (
      D => write_data3(24),
      C => clk90,
      Q => NLW_write_data_mo24_Q_UNCONNECTED
    );
  write_data_mo23 : FD
    port map (
      D => write_data3(23),
      C => clk90,
      Q => NLW_write_data_mo23_Q_UNCONNECTED
    );
  write_data_mo22 : FD
    port map (
      D => write_data3(22),
      C => clk90,
      Q => NLW_write_data_mo22_Q_UNCONNECTED
    );
  write_data_mo21 : FD
    port map (
      D => write_data3(21),
      C => clk90,
      Q => NLW_write_data_mo21_Q_UNCONNECTED
    );
  write_data_mo20 : FD
    port map (
      D => write_data3(20),
      C => clk90,
      Q => NLW_write_data_mo20_Q_UNCONNECTED
    );
  write_data_mo19 : FD
    port map (
      D => write_data3(19),
      C => clk90,
      Q => NLW_write_data_mo19_Q_UNCONNECTED
    );
  write_data_mo18 : FD
    port map (
      D => write_data3(18),
      C => clk90,
      Q => NLW_write_data_mo18_Q_UNCONNECTED
    );
  write_data_mo17 : FD
    port map (
      D => write_data3(17),
      C => clk90,
      Q => NLW_write_data_mo17_Q_UNCONNECTED
    );
  write_data_mo16 : FD
    port map (
      D => write_data3(16),
      C => clk90,
      Q => NLW_write_data_mo16_Q_UNCONNECTED
    );
  write_data_mo15 : FD
    port map (
      D => write_data3(15),
      C => clk90,
      Q => write_data4(15)
    );
  write_data_mo14 : FD
    port map (
      D => write_data3(14),
      C => clk90,
      Q => write_data4(14)
    );
  write_data_mo13 : FD
    port map (
      D => write_data3(13),
      C => clk90,
      Q => write_data4(13)
    );
  write_data_mo12 : FD
    port map (
      D => write_data3(12),
      C => clk90,
      Q => write_data4(12)
    );
  write_data_mo11 : FD
    port map (
      D => write_data3(11),
      C => clk90,
      Q => write_data4(11)
    );
  write_data_mo10 : FD
    port map (
      D => write_data3(10),
      C => clk90,
      Q => write_data4(10)
    );
  write_data_mo9 : FD
    port map (
      D => write_data3(9),
      C => clk90,
      Q => write_data4(9)
    );
  write_data_mo8 : FD
    port map (
      D => write_data3(8),
      C => clk90,
      Q => write_data4(8)
    );
  write_data_mo7 : FD
    port map (
      D => write_data3(7),
      C => clk90,
      Q => write_data4(7)
    );
  write_data_mo6 : FD
    port map (
      D => write_data3(6),
      C => clk90,
      Q => write_data4(6)
    );
  write_data_mo5 : FD
    port map (
      D => write_data3(5),
      C => clk90,
      Q => write_data4(5)
    );
  write_data_mo4 : FD
    port map (
      D => write_data3(4),
      C => clk90,
      Q => write_data4(4)
    );
  write_data_mo3 : FD
    port map (
      D => write_data3(3),
      C => clk90,
      Q => write_data4(3)
    );
  write_data_mo2 : FD
    port map (
      D => write_data3(2),
      C => clk90,
      Q => write_data4(2)
    );
  write_data_mo1 : FD
    port map (
      D => write_data3(1),
      C => clk90,
      Q => write_data4(1)
    );
  write_data_mo0 : FD
    port map (
      D => write_data3(0),
      C => clk90,
      Q => write_data4(0)
    );
  write_data_mas3 : FD
    port map (
      D => write_data_m3(3),
      C => clk90,
      Q => NLW_write_data_mas3_Q_UNCONNECTED
    );
  write_data_mas2 : FD
    port map (
      D => write_data_m3(2),
      C => clk90,
      Q => NLW_write_data_mas2_Q_UNCONNECTED
    );
  write_data_mas1 : FD
    port map (
      D => write_data_m3(1),
      C => clk90,
      Q => write_data_m4(1)
    );
  write_data_mas0 : FD
    port map (
      D => write_data_m3(0),
      C => clk90,
      Q => write_data_m4(0)
    );
  XST_VCC : VCC
    port map (
      P => N2
    );
  XST_GND : GND
    port map (
      G => N3
    );
  Mshreg_write_data3_2 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(2),
      Q => Mshreg_write_data3_2_24
    );
  write_data3_2 : FD
    port map (
      D => Mshreg_write_data3_2_24,
      C => clk90,
      Q => write_data3(2)
    );
  Mshreg_write_data3_0 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(0),
      Q => Mshreg_write_data3_0_25
    );
  write_data3_0 : FD
    port map (
      D => Mshreg_write_data3_0_25,
      C => clk90,
      Q => write_data3(0)
    );
  Mshreg_write_data3_1 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(1),
      Q => Mshreg_write_data3_1_26
    );
  write_data3_1 : FD
    port map (
      D => Mshreg_write_data3_1_26,
      C => clk90,
      Q => write_data3(1)
    );
  Mshreg_write_data3_3 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(3),
      Q => Mshreg_write_data3_3_27
    );
  write_data3_3 : FD
    port map (
      D => Mshreg_write_data3_3_27,
      C => clk90,
      Q => write_data3(3)
    );
  Mshreg_write_data3_4 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(4),
      Q => Mshreg_write_data3_4_28
    );
  write_data3_4 : FD
    port map (
      D => Mshreg_write_data3_4_28,
      C => clk90,
      Q => write_data3(4)
    );
  Mshreg_write_data3_5 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(5),
      Q => Mshreg_write_data3_5_29
    );
  write_data3_5 : FD
    port map (
      D => Mshreg_write_data3_5_29,
      C => clk90,
      Q => write_data3(5)
    );
  Mshreg_write_data3_6 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(6),
      Q => Mshreg_write_data3_6_30
    );
  write_data3_6 : FD
    port map (
      D => Mshreg_write_data3_6_30,
      C => clk90,
      Q => write_data3(6)
    );
  Mshreg_write_data3_7 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(7),
      Q => Mshreg_write_data3_7_31
    );
  write_data3_7 : FD
    port map (
      D => Mshreg_write_data3_7_31,
      C => clk90,
      Q => write_data3(7)
    );
  Mshreg_write_data3_8 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(8),
      Q => Mshreg_write_data3_8_32
    );
  write_data3_8 : FD
    port map (
      D => Mshreg_write_data3_8_32,
      C => clk90,
      Q => write_data3(8)
    );
  Mshreg_write_data3_11 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(11),
      Q => Mshreg_write_data3_11_33
    );
  write_data3_11 : FD
    port map (
      D => Mshreg_write_data3_11_33,
      C => clk90,
      Q => write_data3(11)
    );
  Mshreg_write_data3_9 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(9),
      Q => Mshreg_write_data3_9_34
    );
  write_data3_9 : FD
    port map (
      D => Mshreg_write_data3_9_34,
      C => clk90,
      Q => write_data3(9)
    );
  Mshreg_write_data3_10 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(10),
      Q => Mshreg_write_data3_10_35
    );
  write_data3_10 : FD
    port map (
      D => Mshreg_write_data3_10_35,
      C => clk90,
      Q => write_data3(10)
    );
  Mshreg_write_data3_12 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(12),
      Q => Mshreg_write_data3_12_36
    );
  write_data3_12 : FD
    port map (
      D => Mshreg_write_data3_12_36,
      C => clk90,
      Q => write_data3(12)
    );
  Mshreg_write_data3_13 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(13),
      Q => Mshreg_write_data3_13_37
    );
  write_data3_13 : FD
    port map (
      D => Mshreg_write_data3_13_37,
      C => clk90,
      Q => write_data3(13)
    );
  Mshreg_write_data3_14 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(14),
      Q => Mshreg_write_data3_14_38
    );
  write_data3_14 : FD
    port map (
      D => Mshreg_write_data3_14_38,
      C => clk90,
      Q => write_data3(14)
    );
  Mshreg_write_data3_15 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(15),
      Q => Mshreg_write_data3_15_39
    );
  write_data3_15 : FD
    port map (
      D => Mshreg_write_data3_15_39,
      C => clk90,
      Q => write_data3(15)
    );
  Mshreg_write_data3_16 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(16),
      Q => Mshreg_write_data3_16_40
    );
  write_data3_16 : FD
    port map (
      D => Mshreg_write_data3_16_40,
      C => clk90,
      Q => write_data3(16)
    );
  Mshreg_write_data3_17 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(17),
      Q => Mshreg_write_data3_17_41
    );
  write_data3_17 : FD
    port map (
      D => Mshreg_write_data3_17_41,
      C => clk90,
      Q => write_data3(17)
    );
  Mshreg_write_data3_20 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(20),
      Q => Mshreg_write_data3_20_42
    );
  write_data3_20 : FD
    port map (
      D => Mshreg_write_data3_20_42,
      C => clk90,
      Q => write_data3(20)
    );
  Mshreg_write_data3_18 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(18),
      Q => Mshreg_write_data3_18_43
    );
  write_data3_18 : FD
    port map (
      D => Mshreg_write_data3_18_43,
      C => clk90,
      Q => write_data3(18)
    );
  Mshreg_write_data3_19 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(19),
      Q => Mshreg_write_data3_19_44
    );
  write_data3_19 : FD
    port map (
      D => Mshreg_write_data3_19_44,
      C => clk90,
      Q => write_data3(19)
    );
  Mshreg_write_data3_21 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(21),
      Q => Mshreg_write_data3_21_45
    );
  write_data3_21 : FD
    port map (
      D => Mshreg_write_data3_21_45,
      C => clk90,
      Q => write_data3(21)
    );
  Mshreg_write_data3_22 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(22),
      Q => Mshreg_write_data3_22_46
    );
  write_data3_22 : FD
    port map (
      D => Mshreg_write_data3_22_46,
      C => clk90,
      Q => write_data3(22)
    );
  Mshreg_write_data3_23 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(23),
      Q => Mshreg_write_data3_23_47
    );
  write_data3_23 : FD
    port map (
      D => Mshreg_write_data3_23_47,
      C => clk90,
      Q => write_data3(23)
    );
  Mshreg_write_data3_24 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(24),
      Q => Mshreg_write_data3_24_48
    );
  write_data3_24 : FD
    port map (
      D => Mshreg_write_data3_24_48,
      C => clk90,
      Q => write_data3(24)
    );
  Mshreg_write_data3_25 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(25),
      Q => Mshreg_write_data3_25_49
    );
  write_data3_25 : FD
    port map (
      D => Mshreg_write_data3_25_49,
      C => clk90,
      Q => write_data3(25)
    );
  Mshreg_write_data3_26 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(26),
      Q => Mshreg_write_data3_26_50
    );
  write_data3_26 : FD
    port map (
      D => Mshreg_write_data3_26_50,
      C => clk90,
      Q => write_data3(26)
    );
  Mshreg_write_data3_29 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(29),
      Q => Mshreg_write_data3_29_51
    );
  write_data3_29 : FD
    port map (
      D => Mshreg_write_data3_29_51,
      C => clk90,
      Q => write_data3(29)
    );
  Mshreg_write_data3_27 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(27),
      Q => Mshreg_write_data3_27_52
    );
  write_data3_27 : FD
    port map (
      D => Mshreg_write_data3_27_52,
      C => clk90,
      Q => write_data3(27)
    );
  Mshreg_write_data3_28 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(28),
      Q => Mshreg_write_data3_28_53
    );
  write_data3_28 : FD
    port map (
      D => Mshreg_write_data3_28_53,
      C => clk90,
      Q => write_data3(28)
    );
  Mshreg_write_data3_30 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(30),
      Q => Mshreg_write_data3_30_54
    );
  write_data3_30 : FD
    port map (
      D => Mshreg_write_data3_30_54,
      C => clk90,
      Q => write_data3(30)
    );
  Mshreg_write_data3_31 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_input_data(31),
      Q => Mshreg_write_data3_31_55
    );
  write_data3_31 : FD
    port map (
      D => Mshreg_write_data3_31_55,
      C => clk90,
      Q => write_data3(31)
    );
  Mshreg_write_data_m3_0 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_data_mask(0),
      Q => Mshreg_write_data_m3_0_56
    );
  write_data_m3_0 : FD
    port map (
      D => Mshreg_write_data_m3_0_56,
      C => clk90,
      Q => write_data_m3(0)
    );
  Mshreg_write_data_m3_1 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_data_mask(1),
      Q => Mshreg_write_data_m3_1_57
    );
  write_data_m3_1 : FD
    port map (
      D => Mshreg_write_data_m3_1_57,
      C => clk90,
      Q => write_data_m3(1)
    );
  Mshreg_write_data_m3_2 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_data_mask(2),
      Q => Mshreg_write_data_m3_2_58
    );
  write_data_m3_2 : FD
    port map (
      D => Mshreg_write_data_m3_2_58,
      C => clk90,
      Q => write_data_m3(2)
    );
  Mshreg_write_data_m3_3 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N2,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => user_data_mask(3),
      Q => Mshreg_write_data_m3_3_59
    );
  write_data_m3_3 : FD
    port map (
      D => Mshreg_write_data_m3_3_59,
      C => clk90,
      Q => write_data_m3(3)
    );
  Mshreg_write_data_mask_0 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data_m4(0),
      Q => Mshreg_write_data_mask_0_60
    );
  write_data_mask_0 : FD
    port map (
      D => Mshreg_write_data_mask_0_60,
      C => clk90,
      Q => data_mask_f(0)
    );
  Mshreg_write_data_mask_1 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data_m4(1),
      Q => Mshreg_write_data_mask_1_61
    );
  write_data_mask_1 : FD
    port map (
      D => Mshreg_write_data_mask_1_61,
      C => clk90,
      Q => data_mask_f(1)
    );
  Mshreg_write_data_0 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data4(0),
      Q => Mshreg_write_data_0_62
    );
  write_data_0 : FD
    port map (
      D => Mshreg_write_data_0_62,
      C => clk90,
      Q => write_data_falling(0)
    );
  Mshreg_write_data_1 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data4(1),
      Q => Mshreg_write_data_1_63
    );
  write_data_1 : FD
    port map (
      D => Mshreg_write_data_1_63,
      C => clk90,
      Q => write_data_falling(1)
    );
  Mshreg_write_data_2 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data4(2),
      Q => Mshreg_write_data_2_64
    );
  write_data_2 : FD
    port map (
      D => Mshreg_write_data_2_64,
      C => clk90,
      Q => write_data_falling(2)
    );
  Mshreg_write_data_3 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data4(3),
      Q => Mshreg_write_data_3_65
    );
  write_data_3 : FD
    port map (
      D => Mshreg_write_data_3_65,
      C => clk90,
      Q => write_data_falling(3)
    );
  Mshreg_write_data_4 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data4(4),
      Q => Mshreg_write_data_4_66
    );
  write_data_4 : FD
    port map (
      D => Mshreg_write_data_4_66,
      C => clk90,
      Q => write_data_falling(4)
    );
  Mshreg_write_data_5 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data4(5),
      Q => Mshreg_write_data_5_67
    );
  write_data_5 : FD
    port map (
      D => Mshreg_write_data_5_67,
      C => clk90,
      Q => write_data_falling(5)
    );
  Mshreg_write_data_6 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data4(6),
      Q => Mshreg_write_data_6_68
    );
  write_data_6 : FD
    port map (
      D => Mshreg_write_data_6_68,
      C => clk90,
      Q => write_data_falling(6)
    );
  Mshreg_write_data_9 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data4(9),
      Q => Mshreg_write_data_9_69
    );
  write_data_9 : FD
    port map (
      D => Mshreg_write_data_9_69,
      C => clk90,
      Q => write_data_falling(9)
    );
  Mshreg_write_data_7 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data4(7),
      Q => Mshreg_write_data_7_70
    );
  write_data_7 : FD
    port map (
      D => Mshreg_write_data_7_70,
      C => clk90,
      Q => write_data_falling(7)
    );
  Mshreg_write_data_8 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data4(8),
      Q => Mshreg_write_data_8_71
    );
  write_data_8 : FD
    port map (
      D => Mshreg_write_data_8_71,
      C => clk90,
      Q => write_data_falling(8)
    );
  Mshreg_write_data_10 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data4(10),
      Q => Mshreg_write_data_10_72
    );
  write_data_10 : FD
    port map (
      D => Mshreg_write_data_10_72,
      C => clk90,
      Q => write_data_falling(10)
    );
  Mshreg_write_data_11 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data4(11),
      Q => Mshreg_write_data_11_73
    );
  write_data_11 : FD
    port map (
      D => Mshreg_write_data_11_73,
      C => clk90,
      Q => write_data_falling(11)
    );
  Mshreg_write_data_12 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data4(12),
      Q => Mshreg_write_data_12_74
    );
  write_data_12 : FD
    port map (
      D => Mshreg_write_data_12_74,
      C => clk90,
      Q => write_data_falling(12)
    );
  Mshreg_write_data_13 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data4(13),
      Q => Mshreg_write_data_13_75
    );
  write_data_13 : FD
    port map (
      D => Mshreg_write_data_13_75,
      C => clk90,
      Q => write_data_falling(13)
    );
  Mshreg_write_data_14 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data4(14),
      Q => Mshreg_write_data_14_76
    );
  write_data_14 : FD
    port map (
      D => Mshreg_write_data_14_76,
      C => clk90,
      Q => write_data_falling(14)
    );
  Mshreg_write_data_15 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N3,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk90,
      D => write_data4(15),
      Q => Mshreg_write_data_15_77
    );
  write_data_15 : FD
    port map (
      D => Mshreg_write_data_15_77,
      C => clk90,
      Q => write_data_falling(15)
    );
  Mshreg_data_mask_r_0 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data_m3(2),
      Q => Mshreg_data_mask_r_0_78
    );
  data_mask_r_0 : FD
    port map (
      D => Mshreg_data_mask_r_0_78,
      C => clk270,
      Q => data_mask_r(0)
    );
  Mshreg_data_mask_r_1 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data_m3(3),
      Q => Mshreg_data_mask_r_1_79
    );
  data_mask_r_1 : FD
    port map (
      D => Mshreg_data_mask_r_1_79,
      C => clk270,
      Q => data_mask_r(1)
    );
  Mshreg_write_data_rising_0 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data3(16),
      Q => Mshreg_write_data_rising_0_80
    );
  write_data_rising_0 : FD
    port map (
      D => Mshreg_write_data_rising_0_80,
      C => clk270,
      Q => write_data_rising(0)
    );
  Mshreg_write_data_rising_1 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data3(17),
      Q => Mshreg_write_data_rising_1_81
    );
  write_data_rising_1 : FD
    port map (
      D => Mshreg_write_data_rising_1_81,
      C => clk270,
      Q => write_data_rising(1)
    );
  Mshreg_write_data_rising_2 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data3(18),
      Q => Mshreg_write_data_rising_2_82
    );
  write_data_rising_2 : FD
    port map (
      D => Mshreg_write_data_rising_2_82,
      C => clk270,
      Q => write_data_rising(2)
    );
  Mshreg_write_data_rising_3 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data3(19),
      Q => Mshreg_write_data_rising_3_83
    );
  write_data_rising_3 : FD
    port map (
      D => Mshreg_write_data_rising_3_83,
      C => clk270,
      Q => write_data_rising(3)
    );
  Mshreg_write_data_rising_4 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data3(20),
      Q => Mshreg_write_data_rising_4_84
    );
  write_data_rising_4 : FD
    port map (
      D => Mshreg_write_data_rising_4_84,
      C => clk270,
      Q => write_data_rising(4)
    );
  Mshreg_write_data_rising_7 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data3(23),
      Q => Mshreg_write_data_rising_7_85
    );
  write_data_rising_7 : FD
    port map (
      D => Mshreg_write_data_rising_7_85,
      C => clk270,
      Q => write_data_rising(7)
    );
  Mshreg_write_data_rising_5 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data3(21),
      Q => Mshreg_write_data_rising_5_86
    );
  write_data_rising_5 : FD
    port map (
      D => Mshreg_write_data_rising_5_86,
      C => clk270,
      Q => write_data_rising(5)
    );
  Mshreg_write_data_rising_6 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data3(22),
      Q => Mshreg_write_data_rising_6_87
    );
  write_data_rising_6 : FD
    port map (
      D => Mshreg_write_data_rising_6_87,
      C => clk270,
      Q => write_data_rising(6)
    );
  Mshreg_write_data_rising_8 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data3(24),
      Q => Mshreg_write_data_rising_8_88
    );
  write_data_rising_8 : FD
    port map (
      D => Mshreg_write_data_rising_8_88,
      C => clk270,
      Q => write_data_rising(8)
    );
  Mshreg_write_data_rising_9 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data3(25),
      Q => Mshreg_write_data_rising_9_89
    );
  write_data_rising_9 : FD
    port map (
      D => Mshreg_write_data_rising_9_89,
      C => clk270,
      Q => write_data_rising(9)
    );
  Mshreg_write_data_rising_10 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data3(26),
      Q => Mshreg_write_data_rising_10_90
    );
  write_data_rising_10 : FD
    port map (
      D => Mshreg_write_data_rising_10_90,
      C => clk270,
      Q => write_data_rising(10)
    );
  Mshreg_write_data_rising_11 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data3(27),
      Q => Mshreg_write_data_rising_11_91
    );
  write_data_rising_11 : FD
    port map (
      D => Mshreg_write_data_rising_11_91,
      C => clk270,
      Q => write_data_rising(11)
    );
  Mshreg_write_data_rising_12 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data3(28),
      Q => Mshreg_write_data_rising_12_92
    );
  write_data_rising_12 : FD
    port map (
      D => Mshreg_write_data_rising_12_92,
      C => clk270,
      Q => write_data_rising(12)
    );
  Mshreg_write_data_rising_13 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data3(29),
      Q => Mshreg_write_data_rising_13_93
    );
  write_data_rising_13 : FD
    port map (
      D => Mshreg_write_data_rising_13_93,
      C => clk270,
      Q => write_data_rising(13)
    );
  Mshreg_write_data_rising_14 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data3(30),
      Q => Mshreg_write_data_rising_14_94
    );
  write_data_rising_14 : FD
    port map (
      D => Mshreg_write_data_rising_14_94,
      C => clk270,
      Q => write_data_rising(14)
    );
  Mshreg_write_data_rising_15 : SRL16
    generic map(
      INIT => X"0000"
    )
    port map (
      A0 => N2,
      A1 => N3,
      A2 => N3,
      A3 => N3,
      CLK => clk270,
      D => write_data3(31),
      Q => Mshreg_write_data_rising_15_95
    );
  write_data_rising_15 : FD
    port map (
      D => Mshreg_write_data_rising_15_95,
      C => clk270,
      Q => write_data_rising(15)
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity RAM_8D_INST_7 is
  port (
    WCLK : in STD_LOGIC := 'X'; 
    DPRA0 : in STD_LOGIC := 'X'; 
    DPRA1 : in STD_LOGIC := 'X'; 
    DPRA2 : in STD_LOGIC := 'X'; 
    DPRA3 : in STD_LOGIC := 'X'; 
    WE : in STD_LOGIC := 'X'; 
    A0 : in STD_LOGIC := 'X'; 
    A1 : in STD_LOGIC := 'X'; 
    A2 : in STD_LOGIC := 'X'; 
    A3 : in STD_LOGIC := 'X'; 
    D : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
    DPO : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
  );
end RAM_8D_INST_7;

architecture Structure of RAM_8D_INST_7 is
  signal NLW_B0_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B1_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B2_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B3_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B4_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B5_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B6_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B7_SPO_UNCONNECTED : STD_LOGIC; 
begin
  B0 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(0),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B0_SPO_UNCONNECTED,
      DPO => DPO(0)
    );
  B1 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(1),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B1_SPO_UNCONNECTED,
      DPO => DPO(1)
    );
  B2 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(2),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B2_SPO_UNCONNECTED,
      DPO => DPO(2)
    );
  B3 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(3),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B3_SPO_UNCONNECTED,
      DPO => DPO(3)
    );
  B4 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(4),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B4_SPO_UNCONNECTED,
      DPO => DPO(4)
    );
  B5 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(5),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B5_SPO_UNCONNECTED,
      DPO => DPO(5)
    );
  B6 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(6),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B6_SPO_UNCONNECTED,
      DPO => DPO(6)
    );
  B7 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(7),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B7_SPO_UNCONNECTED,
      DPO => DPO(7)
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity RAM_8D_INST_6 is
  port (
    WCLK : in STD_LOGIC := 'X'; 
    DPRA0 : in STD_LOGIC := 'X'; 
    DPRA1 : in STD_LOGIC := 'X'; 
    DPRA2 : in STD_LOGIC := 'X'; 
    DPRA3 : in STD_LOGIC := 'X'; 
    WE : in STD_LOGIC := 'X'; 
    A0 : in STD_LOGIC := 'X'; 
    A1 : in STD_LOGIC := 'X'; 
    A2 : in STD_LOGIC := 'X'; 
    A3 : in STD_LOGIC := 'X'; 
    D : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
    DPO : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
  );
end RAM_8D_INST_6;

architecture Structure of RAM_8D_INST_6 is
  signal NLW_B0_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B1_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B2_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B3_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B4_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B5_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B6_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B7_SPO_UNCONNECTED : STD_LOGIC; 
begin
  B0 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(0),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B0_SPO_UNCONNECTED,
      DPO => DPO(0)
    );
  B1 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(1),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B1_SPO_UNCONNECTED,
      DPO => DPO(1)
    );
  B2 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(2),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B2_SPO_UNCONNECTED,
      DPO => DPO(2)
    );
  B3 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(3),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B3_SPO_UNCONNECTED,
      DPO => DPO(3)
    );
  B4 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(4),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B4_SPO_UNCONNECTED,
      DPO => DPO(4)
    );
  B5 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(5),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B5_SPO_UNCONNECTED,
      DPO => DPO(5)
    );
  B6 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(6),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B6_SPO_UNCONNECTED,
      DPO => DPO(6)
    );
  B7 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(7),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B7_SPO_UNCONNECTED,
      DPO => DPO(7)
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity RAM_8D_INST_5 is
  port (
    WCLK : in STD_LOGIC := 'X'; 
    DPRA0 : in STD_LOGIC := 'X'; 
    DPRA1 : in STD_LOGIC := 'X'; 
    DPRA2 : in STD_LOGIC := 'X'; 
    DPRA3 : in STD_LOGIC := 'X'; 
    WE : in STD_LOGIC := 'X'; 
    A0 : in STD_LOGIC := 'X'; 
    A1 : in STD_LOGIC := 'X'; 
    A2 : in STD_LOGIC := 'X'; 
    A3 : in STD_LOGIC := 'X'; 
    D : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
    DPO : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
  );
end RAM_8D_INST_5;

architecture Structure of RAM_8D_INST_5 is
  signal NLW_B0_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B1_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B2_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B3_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B4_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B5_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B6_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B7_SPO_UNCONNECTED : STD_LOGIC; 
begin
  B0 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(0),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B0_SPO_UNCONNECTED,
      DPO => DPO(0)
    );
  B1 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(1),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B1_SPO_UNCONNECTED,
      DPO => DPO(1)
    );
  B2 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(2),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B2_SPO_UNCONNECTED,
      DPO => DPO(2)
    );
  B3 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(3),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B3_SPO_UNCONNECTED,
      DPO => DPO(3)
    );
  B4 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(4),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B4_SPO_UNCONNECTED,
      DPO => DPO(4)
    );
  B5 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(5),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B5_SPO_UNCONNECTED,
      DPO => DPO(5)
    );
  B6 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(6),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B6_SPO_UNCONNECTED,
      DPO => DPO(6)
    );
  B7 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(7),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B7_SPO_UNCONNECTED,
      DPO => DPO(7)
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity RAM_8D_INST_4 is
  port (
    WCLK : in STD_LOGIC := 'X'; 
    DPRA0 : in STD_LOGIC := 'X'; 
    DPRA1 : in STD_LOGIC := 'X'; 
    DPRA2 : in STD_LOGIC := 'X'; 
    DPRA3 : in STD_LOGIC := 'X'; 
    WE : in STD_LOGIC := 'X'; 
    A0 : in STD_LOGIC := 'X'; 
    A1 : in STD_LOGIC := 'X'; 
    A2 : in STD_LOGIC := 'X'; 
    A3 : in STD_LOGIC := 'X'; 
    D : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
    DPO : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
  );
end RAM_8D_INST_4;

architecture Structure of RAM_8D_INST_4 is
  signal NLW_B0_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B1_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B2_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B3_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B4_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B5_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B6_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B7_SPO_UNCONNECTED : STD_LOGIC; 
begin
  B0 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(0),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B0_SPO_UNCONNECTED,
      DPO => DPO(0)
    );
  B1 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(1),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B1_SPO_UNCONNECTED,
      DPO => DPO(1)
    );
  B2 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(2),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B2_SPO_UNCONNECTED,
      DPO => DPO(2)
    );
  B3 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(3),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B3_SPO_UNCONNECTED,
      DPO => DPO(3)
    );
  B4 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(4),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B4_SPO_UNCONNECTED,
      DPO => DPO(4)
    );
  B5 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(5),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B5_SPO_UNCONNECTED,
      DPO => DPO(5)
    );
  B6 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(6),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B6_SPO_UNCONNECTED,
      DPO => DPO(6)
    );
  B7 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(7),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B7_SPO_UNCONNECTED,
      DPO => DPO(7)
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity RAM_8D_INST_3 is
  port (
    WCLK : in STD_LOGIC := 'X'; 
    DPRA0 : in STD_LOGIC := 'X'; 
    DPRA1 : in STD_LOGIC := 'X'; 
    DPRA2 : in STD_LOGIC := 'X'; 
    DPRA3 : in STD_LOGIC := 'X'; 
    WE : in STD_LOGIC := 'X'; 
    A0 : in STD_LOGIC := 'X'; 
    A1 : in STD_LOGIC := 'X'; 
    A2 : in STD_LOGIC := 'X'; 
    A3 : in STD_LOGIC := 'X'; 
    D : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
    DPO : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
  );
end RAM_8D_INST_3;

architecture Structure of RAM_8D_INST_3 is
  signal NLW_B0_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B1_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B2_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B3_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B4_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B5_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B6_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B7_SPO_UNCONNECTED : STD_LOGIC; 
begin
  B0 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(0),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B0_SPO_UNCONNECTED,
      DPO => DPO(0)
    );
  B1 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(1),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B1_SPO_UNCONNECTED,
      DPO => DPO(1)
    );
  B2 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(2),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B2_SPO_UNCONNECTED,
      DPO => DPO(2)
    );
  B3 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(3),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B3_SPO_UNCONNECTED,
      DPO => DPO(3)
    );
  B4 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(4),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B4_SPO_UNCONNECTED,
      DPO => DPO(4)
    );
  B5 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(5),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B5_SPO_UNCONNECTED,
      DPO => DPO(5)
    );
  B6 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(6),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B6_SPO_UNCONNECTED,
      DPO => DPO(6)
    );
  B7 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(7),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B7_SPO_UNCONNECTED,
      DPO => DPO(7)
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity RAM_8D_INST_2 is
  port (
    WCLK : in STD_LOGIC := 'X'; 
    DPRA0 : in STD_LOGIC := 'X'; 
    DPRA1 : in STD_LOGIC := 'X'; 
    DPRA2 : in STD_LOGIC := 'X'; 
    DPRA3 : in STD_LOGIC := 'X'; 
    WE : in STD_LOGIC := 'X'; 
    A0 : in STD_LOGIC := 'X'; 
    A1 : in STD_LOGIC := 'X'; 
    A2 : in STD_LOGIC := 'X'; 
    A3 : in STD_LOGIC := 'X'; 
    D : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
    DPO : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
  );
end RAM_8D_INST_2;

architecture Structure of RAM_8D_INST_2 is
  signal NLW_B0_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B1_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B2_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B3_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B4_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B5_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B6_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B7_SPO_UNCONNECTED : STD_LOGIC; 
begin
  B0 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(0),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B0_SPO_UNCONNECTED,
      DPO => DPO(0)
    );
  B1 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(1),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B1_SPO_UNCONNECTED,
      DPO => DPO(1)
    );
  B2 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(2),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B2_SPO_UNCONNECTED,
      DPO => DPO(2)
    );
  B3 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(3),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B3_SPO_UNCONNECTED,
      DPO => DPO(3)
    );
  B4 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(4),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B4_SPO_UNCONNECTED,
      DPO => DPO(4)
    );
  B5 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(5),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B5_SPO_UNCONNECTED,
      DPO => DPO(5)
    );
  B6 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(6),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B6_SPO_UNCONNECTED,
      DPO => DPO(6)
    );
  B7 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(7),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B7_SPO_UNCONNECTED,
      DPO => DPO(7)
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity RAM_8D_INST_1 is
  port (
    WCLK : in STD_LOGIC := 'X'; 
    DPRA0 : in STD_LOGIC := 'X'; 
    DPRA1 : in STD_LOGIC := 'X'; 
    DPRA2 : in STD_LOGIC := 'X'; 
    DPRA3 : in STD_LOGIC := 'X'; 
    WE : in STD_LOGIC := 'X'; 
    A0 : in STD_LOGIC := 'X'; 
    A1 : in STD_LOGIC := 'X'; 
    A2 : in STD_LOGIC := 'X'; 
    A3 : in STD_LOGIC := 'X'; 
    D : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
    DPO : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
  );
end RAM_8D_INST_1;

architecture Structure of RAM_8D_INST_1 is
  signal NLW_B0_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B1_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B2_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B3_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B4_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B5_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B6_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B7_SPO_UNCONNECTED : STD_LOGIC; 
begin
  B0 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(0),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B0_SPO_UNCONNECTED,
      DPO => DPO(0)
    );
  B1 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(1),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B1_SPO_UNCONNECTED,
      DPO => DPO(1)
    );
  B2 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(2),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B2_SPO_UNCONNECTED,
      DPO => DPO(2)
    );
  B3 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(3),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B3_SPO_UNCONNECTED,
      DPO => DPO(3)
    );
  B4 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(4),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B4_SPO_UNCONNECTED,
      DPO => DPO(4)
    );
  B5 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(5),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B5_SPO_UNCONNECTED,
      DPO => DPO(5)
    );
  B6 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(6),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B6_SPO_UNCONNECTED,
      DPO => DPO(6)
    );
  B7 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(7),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B7_SPO_UNCONNECTED,
      DPO => DPO(7)
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity RAM_8D is
  port (
    WCLK : in STD_LOGIC := 'X'; 
    DPRA0 : in STD_LOGIC := 'X'; 
    DPRA1 : in STD_LOGIC := 'X'; 
    DPRA2 : in STD_LOGIC := 'X'; 
    DPRA3 : in STD_LOGIC := 'X'; 
    WE : in STD_LOGIC := 'X'; 
    A0 : in STD_LOGIC := 'X'; 
    A1 : in STD_LOGIC := 'X'; 
    A2 : in STD_LOGIC := 'X'; 
    A3 : in STD_LOGIC := 'X'; 
    D : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
    DPO : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
  );
end RAM_8D;

architecture Structure of RAM_8D is
  signal NLW_B0_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B1_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B2_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B3_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B4_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B5_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B6_SPO_UNCONNECTED : STD_LOGIC; 
  signal NLW_B7_SPO_UNCONNECTED : STD_LOGIC; 
begin
  B0 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(0),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B0_SPO_UNCONNECTED,
      DPO => DPO(0)
    );
  B1 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(1),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B1_SPO_UNCONNECTED,
      DPO => DPO(1)
    );
  B2 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(2),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B2_SPO_UNCONNECTED,
      DPO => DPO(2)
    );
  B3 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(3),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B3_SPO_UNCONNECTED,
      DPO => DPO(3)
    );
  B4 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(4),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B4_SPO_UNCONNECTED,
      DPO => DPO(4)
    );
  B5 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(5),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B5_SPO_UNCONNECTED,
      DPO => DPO(5)
    );
  B6 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(6),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B6_SPO_UNCONNECTED,
      DPO => DPO(6)
    );
  B7 : RAM16X1D
    port map (
      A0 => A0,
      A1 => A1,
      A2 => A2,
      A3 => A3,
      D => D(7),
      DPRA0 => DPRA0,
      DPRA1 => DPRA1,
      DPRA2 => DPRA2,
      DPRA3 => DPRA3,
      WCLK => WCLK,
      WE => WE,
      SPO => NLW_B7_SPO_UNCONNECTED,
      DPO => DPO(7)
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqbit_INST_15 is
  port (
    dq : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs_div_1 : in STD_LOGIC := 'X'; 
    dqs_div_2 : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    fbit_0 : out STD_LOGIC; 
    fbit_1 : out STD_LOGIC; 
    fbit_2 : out STD_LOGIC; 
    fbit_3 : out STD_LOGIC 
  );
end ddr2_dqbit_INST_15;

architecture Structure of ddr2_dqbit_INST_15 is
  signal dqsn : STD_LOGIC; 
  signal dqs_div2n : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : FDCE
    port map (
      D => dq,
      CE => dqs_div2n,
      CLR => reset,
      C => dqs1,
      Q => fbit(0)
    );
  fbit1 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqsn,
      Q => fbit(1)
    );
  fbit2 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqs1,
      Q => fbit(2)
    );
  fbit3 : FDCE
    port map (
      D => dq,
      CE => dqs_div_1,
      CLR => reset,
      C => dqsn,
      Q => fbit(3)
    );
  dqs_div2n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs_div_2,
      O => dqs_div2n
    );
  dqsn1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqsn
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqbit_INST_14 is
  port (
    dq : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs_div_1 : in STD_LOGIC := 'X'; 
    dqs_div_2 : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    fbit_0 : out STD_LOGIC; 
    fbit_1 : out STD_LOGIC; 
    fbit_2 : out STD_LOGIC; 
    fbit_3 : out STD_LOGIC 
  );
end ddr2_dqbit_INST_14;

architecture Structure of ddr2_dqbit_INST_14 is
  signal dqsn : STD_LOGIC; 
  signal dqs_div2n : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : FDCE
    port map (
      D => dq,
      CE => dqs_div2n,
      CLR => reset,
      C => dqs1,
      Q => fbit(0)
    );
  fbit1 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqsn,
      Q => fbit(1)
    );
  fbit2 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqs1,
      Q => fbit(2)
    );
  fbit3 : FDCE
    port map (
      D => dq,
      CE => dqs_div_1,
      CLR => reset,
      C => dqsn,
      Q => fbit(3)
    );
  dqs_div2n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs_div_2,
      O => dqs_div2n
    );
  dqsn1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqsn
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqbit_INST_13 is
  port (
    dq : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs_div_1 : in STD_LOGIC := 'X'; 
    dqs_div_2 : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    fbit_0 : out STD_LOGIC; 
    fbit_1 : out STD_LOGIC; 
    fbit_2 : out STD_LOGIC; 
    fbit_3 : out STD_LOGIC 
  );
end ddr2_dqbit_INST_13;

architecture Structure of ddr2_dqbit_INST_13 is
  signal dqsn : STD_LOGIC; 
  signal dqs_div2n : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : FDCE
    port map (
      D => dq,
      CE => dqs_div2n,
      CLR => reset,
      C => dqs1,
      Q => fbit(0)
    );
  fbit1 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqsn,
      Q => fbit(1)
    );
  fbit2 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqs1,
      Q => fbit(2)
    );
  fbit3 : FDCE
    port map (
      D => dq,
      CE => dqs_div_1,
      CLR => reset,
      C => dqsn,
      Q => fbit(3)
    );
  dqs_div2n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs_div_2,
      O => dqs_div2n
    );
  dqsn1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqsn
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqbit_INST_12 is
  port (
    dq : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs_div_1 : in STD_LOGIC := 'X'; 
    dqs_div_2 : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    fbit_0 : out STD_LOGIC; 
    fbit_1 : out STD_LOGIC; 
    fbit_2 : out STD_LOGIC; 
    fbit_3 : out STD_LOGIC 
  );
end ddr2_dqbit_INST_12;

architecture Structure of ddr2_dqbit_INST_12 is
  signal dqsn : STD_LOGIC; 
  signal dqs_div2n : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : FDCE
    port map (
      D => dq,
      CE => dqs_div2n,
      CLR => reset,
      C => dqs1,
      Q => fbit(0)
    );
  fbit1 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqsn,
      Q => fbit(1)
    );
  fbit2 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqs1,
      Q => fbit(2)
    );
  fbit3 : FDCE
    port map (
      D => dq,
      CE => dqs_div_1,
      CLR => reset,
      C => dqsn,
      Q => fbit(3)
    );
  dqs_div2n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs_div_2,
      O => dqs_div2n
    );
  dqsn1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqsn
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqbit_INST_11 is
  port (
    dq : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs_div_1 : in STD_LOGIC := 'X'; 
    dqs_div_2 : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    fbit_0 : out STD_LOGIC; 
    fbit_1 : out STD_LOGIC; 
    fbit_2 : out STD_LOGIC; 
    fbit_3 : out STD_LOGIC 
  );
end ddr2_dqbit_INST_11;

architecture Structure of ddr2_dqbit_INST_11 is
  signal dqsn : STD_LOGIC; 
  signal dqs_div2n : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : FDCE
    port map (
      D => dq,
      CE => dqs_div2n,
      CLR => reset,
      C => dqs1,
      Q => fbit(0)
    );
  fbit1 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqsn,
      Q => fbit(1)
    );
  fbit2 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqs1,
      Q => fbit(2)
    );
  fbit3 : FDCE
    port map (
      D => dq,
      CE => dqs_div_1,
      CLR => reset,
      C => dqsn,
      Q => fbit(3)
    );
  dqs_div2n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs_div_2,
      O => dqs_div2n
    );
  dqsn1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqsn
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqbit_INST_10 is
  port (
    dq : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs_div_1 : in STD_LOGIC := 'X'; 
    dqs_div_2 : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    fbit_0 : out STD_LOGIC; 
    fbit_1 : out STD_LOGIC; 
    fbit_2 : out STD_LOGIC; 
    fbit_3 : out STD_LOGIC 
  );
end ddr2_dqbit_INST_10;

architecture Structure of ddr2_dqbit_INST_10 is
  signal dqsn : STD_LOGIC; 
  signal dqs_div2n : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : FDCE
    port map (
      D => dq,
      CE => dqs_div2n,
      CLR => reset,
      C => dqs1,
      Q => fbit(0)
    );
  fbit1 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqsn,
      Q => fbit(1)
    );
  fbit2 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqs1,
      Q => fbit(2)
    );
  fbit3 : FDCE
    port map (
      D => dq,
      CE => dqs_div_1,
      CLR => reset,
      C => dqsn,
      Q => fbit(3)
    );
  dqs_div2n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs_div_2,
      O => dqs_div2n
    );
  dqsn1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqsn
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqbit_INST_9 is
  port (
    dq : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs_div_1 : in STD_LOGIC := 'X'; 
    dqs_div_2 : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    fbit_0 : out STD_LOGIC; 
    fbit_1 : out STD_LOGIC; 
    fbit_2 : out STD_LOGIC; 
    fbit_3 : out STD_LOGIC 
  );
end ddr2_dqbit_INST_9;

architecture Structure of ddr2_dqbit_INST_9 is
  signal dqsn : STD_LOGIC; 
  signal dqs_div2n : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : FDCE
    port map (
      D => dq,
      CE => dqs_div2n,
      CLR => reset,
      C => dqs1,
      Q => fbit(0)
    );
  fbit1 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqsn,
      Q => fbit(1)
    );
  fbit2 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqs1,
      Q => fbit(2)
    );
  fbit3 : FDCE
    port map (
      D => dq,
      CE => dqs_div_1,
      CLR => reset,
      C => dqsn,
      Q => fbit(3)
    );
  dqs_div2n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs_div_2,
      O => dqs_div2n
    );
  dqsn1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqsn
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqbit_INST_8 is
  port (
    dq : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs_div_1 : in STD_LOGIC := 'X'; 
    dqs_div_2 : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    fbit_0 : out STD_LOGIC; 
    fbit_1 : out STD_LOGIC; 
    fbit_2 : out STD_LOGIC; 
    fbit_3 : out STD_LOGIC 
  );
end ddr2_dqbit_INST_8;

architecture Structure of ddr2_dqbit_INST_8 is
  signal dqsn : STD_LOGIC; 
  signal dqs_div2n : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : FDCE
    port map (
      D => dq,
      CE => dqs_div2n,
      CLR => reset,
      C => dqs1,
      Q => fbit(0)
    );
  fbit1 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqsn,
      Q => fbit(1)
    );
  fbit2 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqs1,
      Q => fbit(2)
    );
  fbit3 : FDCE
    port map (
      D => dq,
      CE => dqs_div_1,
      CLR => reset,
      C => dqsn,
      Q => fbit(3)
    );
  dqs_div2n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs_div_2,
      O => dqs_div2n
    );
  dqsn1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqsn
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqbit_INST_7 is
  port (
    dq : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs_div_1 : in STD_LOGIC := 'X'; 
    dqs_div_2 : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    fbit_0 : out STD_LOGIC; 
    fbit_1 : out STD_LOGIC; 
    fbit_2 : out STD_LOGIC; 
    fbit_3 : out STD_LOGIC 
  );
end ddr2_dqbit_INST_7;

architecture Structure of ddr2_dqbit_INST_7 is
  signal dqsn : STD_LOGIC; 
  signal dqs_div2n : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : FDCE
    port map (
      D => dq,
      CE => dqs_div2n,
      CLR => reset,
      C => dqs1,
      Q => fbit(0)
    );
  fbit1 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqsn,
      Q => fbit(1)
    );
  fbit2 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqs1,
      Q => fbit(2)
    );
  fbit3 : FDCE
    port map (
      D => dq,
      CE => dqs_div_1,
      CLR => reset,
      C => dqsn,
      Q => fbit(3)
    );
  dqs_div2n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs_div_2,
      O => dqs_div2n
    );
  dqsn1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqsn
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqbit_INST_6 is
  port (
    dq : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs_div_1 : in STD_LOGIC := 'X'; 
    dqs_div_2 : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    fbit_0 : out STD_LOGIC; 
    fbit_1 : out STD_LOGIC; 
    fbit_2 : out STD_LOGIC; 
    fbit_3 : out STD_LOGIC 
  );
end ddr2_dqbit_INST_6;

architecture Structure of ddr2_dqbit_INST_6 is
  signal dqsn : STD_LOGIC; 
  signal dqs_div2n : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : FDCE
    port map (
      D => dq,
      CE => dqs_div2n,
      CLR => reset,
      C => dqs1,
      Q => fbit(0)
    );
  fbit1 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqsn,
      Q => fbit(1)
    );
  fbit2 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqs1,
      Q => fbit(2)
    );
  fbit3 : FDCE
    port map (
      D => dq,
      CE => dqs_div_1,
      CLR => reset,
      C => dqsn,
      Q => fbit(3)
    );
  dqs_div2n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs_div_2,
      O => dqs_div2n
    );
  dqsn1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqsn
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqbit_INST_5 is
  port (
    dq : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs_div_1 : in STD_LOGIC := 'X'; 
    dqs_div_2 : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    fbit_0 : out STD_LOGIC; 
    fbit_1 : out STD_LOGIC; 
    fbit_2 : out STD_LOGIC; 
    fbit_3 : out STD_LOGIC 
  );
end ddr2_dqbit_INST_5;

architecture Structure of ddr2_dqbit_INST_5 is
  signal dqsn : STD_LOGIC; 
  signal dqs_div2n : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : FDCE
    port map (
      D => dq,
      CE => dqs_div2n,
      CLR => reset,
      C => dqs1,
      Q => fbit(0)
    );
  fbit1 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqsn,
      Q => fbit(1)
    );
  fbit2 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqs1,
      Q => fbit(2)
    );
  fbit3 : FDCE
    port map (
      D => dq,
      CE => dqs_div_1,
      CLR => reset,
      C => dqsn,
      Q => fbit(3)
    );
  dqs_div2n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs_div_2,
      O => dqs_div2n
    );
  dqsn1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqsn
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqbit_INST_4 is
  port (
    dq : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs_div_1 : in STD_LOGIC := 'X'; 
    dqs_div_2 : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    fbit_0 : out STD_LOGIC; 
    fbit_1 : out STD_LOGIC; 
    fbit_2 : out STD_LOGIC; 
    fbit_3 : out STD_LOGIC 
  );
end ddr2_dqbit_INST_4;

architecture Structure of ddr2_dqbit_INST_4 is
  signal dqsn : STD_LOGIC; 
  signal dqs_div2n : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : FDCE
    port map (
      D => dq,
      CE => dqs_div2n,
      CLR => reset,
      C => dqs1,
      Q => fbit(0)
    );
  fbit1 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqsn,
      Q => fbit(1)
    );
  fbit2 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqs1,
      Q => fbit(2)
    );
  fbit3 : FDCE
    port map (
      D => dq,
      CE => dqs_div_1,
      CLR => reset,
      C => dqsn,
      Q => fbit(3)
    );
  dqs_div2n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs_div_2,
      O => dqs_div2n
    );
  dqsn1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqsn
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqbit_INST_3 is
  port (
    dq : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs_div_1 : in STD_LOGIC := 'X'; 
    dqs_div_2 : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    fbit_0 : out STD_LOGIC; 
    fbit_1 : out STD_LOGIC; 
    fbit_2 : out STD_LOGIC; 
    fbit_3 : out STD_LOGIC 
  );
end ddr2_dqbit_INST_3;

architecture Structure of ddr2_dqbit_INST_3 is
  signal dqsn : STD_LOGIC; 
  signal dqs_div2n : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : FDCE
    port map (
      D => dq,
      CE => dqs_div2n,
      CLR => reset,
      C => dqs1,
      Q => fbit(0)
    );
  fbit1 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqsn,
      Q => fbit(1)
    );
  fbit2 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqs1,
      Q => fbit(2)
    );
  fbit3 : FDCE
    port map (
      D => dq,
      CE => dqs_div_1,
      CLR => reset,
      C => dqsn,
      Q => fbit(3)
    );
  dqs_div2n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs_div_2,
      O => dqs_div2n
    );
  dqsn1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqsn
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqbit_INST_2 is
  port (
    dq : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs_div_1 : in STD_LOGIC := 'X'; 
    dqs_div_2 : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    fbit_0 : out STD_LOGIC; 
    fbit_1 : out STD_LOGIC; 
    fbit_2 : out STD_LOGIC; 
    fbit_3 : out STD_LOGIC 
  );
end ddr2_dqbit_INST_2;

architecture Structure of ddr2_dqbit_INST_2 is
  signal dqsn : STD_LOGIC; 
  signal dqs_div2n : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : FDCE
    port map (
      D => dq,
      CE => dqs_div2n,
      CLR => reset,
      C => dqs1,
      Q => fbit(0)
    );
  fbit1 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqsn,
      Q => fbit(1)
    );
  fbit2 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqs1,
      Q => fbit(2)
    );
  fbit3 : FDCE
    port map (
      D => dq,
      CE => dqs_div_1,
      CLR => reset,
      C => dqsn,
      Q => fbit(3)
    );
  dqs_div2n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs_div_2,
      O => dqs_div2n
    );
  dqsn1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqsn
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqbit_INST_1 is
  port (
    dq : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs_div_1 : in STD_LOGIC := 'X'; 
    dqs_div_2 : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    fbit_0 : out STD_LOGIC; 
    fbit_1 : out STD_LOGIC; 
    fbit_2 : out STD_LOGIC; 
    fbit_3 : out STD_LOGIC 
  );
end ddr2_dqbit_INST_1;

architecture Structure of ddr2_dqbit_INST_1 is
  signal dqsn : STD_LOGIC; 
  signal dqs_div2n : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : FDCE
    port map (
      D => dq,
      CE => dqs_div2n,
      CLR => reset,
      C => dqs1,
      Q => fbit(0)
    );
  fbit1 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqsn,
      Q => fbit(1)
    );
  fbit2 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqs1,
      Q => fbit(2)
    );
  fbit3 : FDCE
    port map (
      D => dq,
      CE => dqs_div_1,
      CLR => reset,
      C => dqsn,
      Q => fbit(3)
    );
  dqs_div2n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs_div_2,
      O => dqs_div2n
    );
  dqsn1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqsn
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqbit is
  port (
    dq : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs_div_1 : in STD_LOGIC := 'X'; 
    dqs_div_2 : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    fbit_0 : out STD_LOGIC; 
    fbit_1 : out STD_LOGIC; 
    fbit_2 : out STD_LOGIC; 
    fbit_3 : out STD_LOGIC 
  );
end ddr2_dqbit;

architecture Structure of ddr2_dqbit is
  signal dqsn : STD_LOGIC; 
  signal dqs_div2n : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : FDCE
    port map (
      D => dq,
      CE => dqs_div2n,
      CLR => reset,
      C => dqs1,
      Q => fbit(0)
    );
  fbit1 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqsn,
      Q => fbit(1)
    );
  fbit2 : FDCE
    port map (
      D => dq,
      CE => dqs_div_2,
      CLR => reset,
      C => dqs1,
      Q => fbit(2)
    );
  fbit3 : FDCE
    port map (
      D => dq,
      CE => dqs_div_1,
      CLR => reset,
      C => dqsn,
      Q => fbit(3)
    );
  dqs_div2n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs_div_2,
      O => dqs_div2n
    );
  dqsn1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqsn
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity data_read_16bit_rl is
  port (
    clk : in STD_LOGIC := 'X'; 
    reset270_r : in STD_LOGIC := 'X'; 
    read_valid_data_1 : in STD_LOGIC := 'X'; 
    read_valid_data_2 : in STD_LOGIC := 'X'; 
    reset90_r : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    next_state_val : out STD_LOGIC; 
    fifo_00_wr_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    dqs_div_col0 : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
    dqs_div_col1 : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
    dqs_delayed_col0 : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
    dqs_delayed_col1 : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
    dq : in STD_LOGIC_VECTOR ( 15 downto 0 ); 
    fifo_13_rd_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_12_rd_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_11_rd_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_10_rd_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_03_rd_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_02_rd_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_01_rd_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_00_rd_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    transfer_done_0 : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    transfer_done_1 : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_13_wr_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_12_wr_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_11_wr_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_10_wr_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_03_wr_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_02_wr_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_01_wr_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    user_output_data : out STD_LOGIC_VECTOR ( 31 downto 0 ) 
  );
end data_read_16bit_rl;

architecture Structure of data_read_16bit_rl is
  component ddr2_dqbit
    port (
      dq : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs_div_1 : in STD_LOGIC := 'X'; 
      dqs_div_2 : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      fbit_0 : out STD_LOGIC; 
      fbit_1 : out STD_LOGIC; 
      fbit_2 : out STD_LOGIC; 
      fbit_3 : out STD_LOGIC 
    );
  end component;
  component ddr2_dqbit_INST_1
    port (
      dq : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs_div_1 : in STD_LOGIC := 'X'; 
      dqs_div_2 : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      fbit_0 : out STD_LOGIC; 
      fbit_1 : out STD_LOGIC; 
      fbit_2 : out STD_LOGIC; 
      fbit_3 : out STD_LOGIC 
    );
  end component;
  component ddr2_dqbit_INST_2
    port (
      dq : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs_div_1 : in STD_LOGIC := 'X'; 
      dqs_div_2 : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      fbit_0 : out STD_LOGIC; 
      fbit_1 : out STD_LOGIC; 
      fbit_2 : out STD_LOGIC; 
      fbit_3 : out STD_LOGIC 
    );
  end component;
  component ddr2_dqbit_INST_3
    port (
      dq : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs_div_1 : in STD_LOGIC := 'X'; 
      dqs_div_2 : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      fbit_0 : out STD_LOGIC; 
      fbit_1 : out STD_LOGIC; 
      fbit_2 : out STD_LOGIC; 
      fbit_3 : out STD_LOGIC 
    );
  end component;
  component ddr2_dqbit_INST_4
    port (
      dq : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs_div_1 : in STD_LOGIC := 'X'; 
      dqs_div_2 : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      fbit_0 : out STD_LOGIC; 
      fbit_1 : out STD_LOGIC; 
      fbit_2 : out STD_LOGIC; 
      fbit_3 : out STD_LOGIC 
    );
  end component;
  component ddr2_dqbit_INST_5
    port (
      dq : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs_div_1 : in STD_LOGIC := 'X'; 
      dqs_div_2 : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      fbit_0 : out STD_LOGIC; 
      fbit_1 : out STD_LOGIC; 
      fbit_2 : out STD_LOGIC; 
      fbit_3 : out STD_LOGIC 
    );
  end component;
  component ddr2_dqbit_INST_6
    port (
      dq : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs_div_1 : in STD_LOGIC := 'X'; 
      dqs_div_2 : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      fbit_0 : out STD_LOGIC; 
      fbit_1 : out STD_LOGIC; 
      fbit_2 : out STD_LOGIC; 
      fbit_3 : out STD_LOGIC 
    );
  end component;
  component ddr2_dqbit_INST_7
    port (
      dq : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs_div_1 : in STD_LOGIC := 'X'; 
      dqs_div_2 : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      fbit_0 : out STD_LOGIC; 
      fbit_1 : out STD_LOGIC; 
      fbit_2 : out STD_LOGIC; 
      fbit_3 : out STD_LOGIC 
    );
  end component;
  component ddr2_dqbit_INST_8
    port (
      dq : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs_div_1 : in STD_LOGIC := 'X'; 
      dqs_div_2 : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      fbit_0 : out STD_LOGIC; 
      fbit_1 : out STD_LOGIC; 
      fbit_2 : out STD_LOGIC; 
      fbit_3 : out STD_LOGIC 
    );
  end component;
  component ddr2_dqbit_INST_9
    port (
      dq : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs_div_1 : in STD_LOGIC := 'X'; 
      dqs_div_2 : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      fbit_0 : out STD_LOGIC; 
      fbit_1 : out STD_LOGIC; 
      fbit_2 : out STD_LOGIC; 
      fbit_3 : out STD_LOGIC 
    );
  end component;
  component ddr2_dqbit_INST_10
    port (
      dq : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs_div_1 : in STD_LOGIC := 'X'; 
      dqs_div_2 : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      fbit_0 : out STD_LOGIC; 
      fbit_1 : out STD_LOGIC; 
      fbit_2 : out STD_LOGIC; 
      fbit_3 : out STD_LOGIC 
    );
  end component;
  component ddr2_dqbit_INST_11
    port (
      dq : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs_div_1 : in STD_LOGIC := 'X'; 
      dqs_div_2 : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      fbit_0 : out STD_LOGIC; 
      fbit_1 : out STD_LOGIC; 
      fbit_2 : out STD_LOGIC; 
      fbit_3 : out STD_LOGIC 
    );
  end component;
  component ddr2_dqbit_INST_12
    port (
      dq : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs_div_1 : in STD_LOGIC := 'X'; 
      dqs_div_2 : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      fbit_0 : out STD_LOGIC; 
      fbit_1 : out STD_LOGIC; 
      fbit_2 : out STD_LOGIC; 
      fbit_3 : out STD_LOGIC 
    );
  end component;
  component ddr2_dqbit_INST_13
    port (
      dq : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs_div_1 : in STD_LOGIC := 'X'; 
      dqs_div_2 : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      fbit_0 : out STD_LOGIC; 
      fbit_1 : out STD_LOGIC; 
      fbit_2 : out STD_LOGIC; 
      fbit_3 : out STD_LOGIC 
    );
  end component;
  component ddr2_dqbit_INST_14
    port (
      dq : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs_div_1 : in STD_LOGIC := 'X'; 
      dqs_div_2 : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      fbit_0 : out STD_LOGIC; 
      fbit_1 : out STD_LOGIC; 
      fbit_2 : out STD_LOGIC; 
      fbit_3 : out STD_LOGIC 
    );
  end component;
  component ddr2_dqbit_INST_15
    port (
      dq : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs_div_1 : in STD_LOGIC := 'X'; 
      dqs_div_2 : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      fbit_0 : out STD_LOGIC; 
      fbit_1 : out STD_LOGIC; 
      fbit_2 : out STD_LOGIC; 
      fbit_3 : out STD_LOGIC 
    );
  end component;
  component RAM_8D
    port (
      WCLK : in STD_LOGIC := 'X'; 
      DPRA0 : in STD_LOGIC := 'X'; 
      DPRA1 : in STD_LOGIC := 'X'; 
      DPRA2 : in STD_LOGIC := 'X'; 
      DPRA3 : in STD_LOGIC := 'X'; 
      WE : in STD_LOGIC := 'X'; 
      A0 : in STD_LOGIC := 'X'; 
      A1 : in STD_LOGIC := 'X'; 
      A2 : in STD_LOGIC := 'X'; 
      A3 : in STD_LOGIC := 'X'; 
      D : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
      DPO : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
    );
  end component;
  component RAM_8D_INST_1
    port (
      WCLK : in STD_LOGIC := 'X'; 
      DPRA0 : in STD_LOGIC := 'X'; 
      DPRA1 : in STD_LOGIC := 'X'; 
      DPRA2 : in STD_LOGIC := 'X'; 
      DPRA3 : in STD_LOGIC := 'X'; 
      WE : in STD_LOGIC := 'X'; 
      A0 : in STD_LOGIC := 'X'; 
      A1 : in STD_LOGIC := 'X'; 
      A2 : in STD_LOGIC := 'X'; 
      A3 : in STD_LOGIC := 'X'; 
      D : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
      DPO : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
    );
  end component;
  component RAM_8D_INST_2
    port (
      WCLK : in STD_LOGIC := 'X'; 
      DPRA0 : in STD_LOGIC := 'X'; 
      DPRA1 : in STD_LOGIC := 'X'; 
      DPRA2 : in STD_LOGIC := 'X'; 
      DPRA3 : in STD_LOGIC := 'X'; 
      WE : in STD_LOGIC := 'X'; 
      A0 : in STD_LOGIC := 'X'; 
      A1 : in STD_LOGIC := 'X'; 
      A2 : in STD_LOGIC := 'X'; 
      A3 : in STD_LOGIC := 'X'; 
      D : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
      DPO : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
    );
  end component;
  component RAM_8D_INST_3
    port (
      WCLK : in STD_LOGIC := 'X'; 
      DPRA0 : in STD_LOGIC := 'X'; 
      DPRA1 : in STD_LOGIC := 'X'; 
      DPRA2 : in STD_LOGIC := 'X'; 
      DPRA3 : in STD_LOGIC := 'X'; 
      WE : in STD_LOGIC := 'X'; 
      A0 : in STD_LOGIC := 'X'; 
      A1 : in STD_LOGIC := 'X'; 
      A2 : in STD_LOGIC := 'X'; 
      A3 : in STD_LOGIC := 'X'; 
      D : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
      DPO : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
    );
  end component;
  component RAM_8D_INST_4
    port (
      WCLK : in STD_LOGIC := 'X'; 
      DPRA0 : in STD_LOGIC := 'X'; 
      DPRA1 : in STD_LOGIC := 'X'; 
      DPRA2 : in STD_LOGIC := 'X'; 
      DPRA3 : in STD_LOGIC := 'X'; 
      WE : in STD_LOGIC := 'X'; 
      A0 : in STD_LOGIC := 'X'; 
      A1 : in STD_LOGIC := 'X'; 
      A2 : in STD_LOGIC := 'X'; 
      A3 : in STD_LOGIC := 'X'; 
      D : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
      DPO : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
    );
  end component;
  component RAM_8D_INST_5
    port (
      WCLK : in STD_LOGIC := 'X'; 
      DPRA0 : in STD_LOGIC := 'X'; 
      DPRA1 : in STD_LOGIC := 'X'; 
      DPRA2 : in STD_LOGIC := 'X'; 
      DPRA3 : in STD_LOGIC := 'X'; 
      WE : in STD_LOGIC := 'X'; 
      A0 : in STD_LOGIC := 'X'; 
      A1 : in STD_LOGIC := 'X'; 
      A2 : in STD_LOGIC := 'X'; 
      A3 : in STD_LOGIC := 'X'; 
      D : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
      DPO : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
    );
  end component;
  component RAM_8D_INST_6
    port (
      WCLK : in STD_LOGIC := 'X'; 
      DPRA0 : in STD_LOGIC := 'X'; 
      DPRA1 : in STD_LOGIC := 'X'; 
      DPRA2 : in STD_LOGIC := 'X'; 
      DPRA3 : in STD_LOGIC := 'X'; 
      WE : in STD_LOGIC := 'X'; 
      A0 : in STD_LOGIC := 'X'; 
      A1 : in STD_LOGIC := 'X'; 
      A2 : in STD_LOGIC := 'X'; 
      A3 : in STD_LOGIC := 'X'; 
      D : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
      DPO : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
    );
  end component;
  component RAM_8D_INST_7
    port (
      WCLK : in STD_LOGIC := 'X'; 
      DPRA0 : in STD_LOGIC := 'X'; 
      DPRA1 : in STD_LOGIC := 'X'; 
      DPRA2 : in STD_LOGIC := 'X'; 
      DPRA3 : in STD_LOGIC := 'X'; 
      WE : in STD_LOGIC := 'X'; 
      A0 : in STD_LOGIC := 'X'; 
      A1 : in STD_LOGIC := 'X'; 
      A2 : in STD_LOGIC := 'X'; 
      A3 : in STD_LOGIC := 'X'; 
      D : in STD_LOGIC_VECTOR ( 7 downto 0 ); 
      DPO : out STD_LOGIC_VECTOR ( 7 downto 0 ) 
    );
  end component;
  signal NlwRenamedSig_OI_next_state_val : STD_LOGIC; 
  signal Q_n0007 : STD_LOGIC; 
  signal N4 : STD_LOGIC; 
  signal NLW_ram_8d_dqs0_fbit1_DPO_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs0_fbit1_DPO_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs0_fbit1_DPO_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs0_fbit1_DPO_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs0_fbit1_DPO_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs0_fbit1_DPO_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs0_fbit1_DPO_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs0_fbit1_DPO_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs0_fbit3_DPO_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs0_fbit3_DPO_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs0_fbit3_DPO_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs0_fbit3_DPO_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs0_fbit3_DPO_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs0_fbit3_DPO_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs0_fbit3_DPO_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs0_fbit3_DPO_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs1_fbit1_DPO_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs1_fbit1_DPO_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs1_fbit3_DPO_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_ram_8d_dqs1_fbit3_DPO_0_UNCONNECTED : STD_LOGIC; 
  signal Q_n0004 : STD_LOGIC_VECTOR ( 31 downto 10 ); 
  signal fbit_0 : STD_LOGIC_VECTOR ( 15 downto 0 ); 
  signal fbit_1 : STD_LOGIC_VECTOR ( 15 downto 0 ); 
  signal fbit_2 : STD_LOGIC_VECTOR ( 15 downto 0 ); 
  signal fbit_3 : STD_LOGIC_VECTOR ( 15 downto 0 ); 
  signal fifo_10_data_out : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal fifo_00_data_out : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal fifo_11_data_out : STD_LOGIC_VECTOR ( 7 downto 2 ); 
  signal fifo_12_data_out : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal fifo_02_data_out : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal fifo_13_data_out : STD_LOGIC_VECTOR ( 7 downto 2 ); 
begin
  next_state_val <= NlwRenamedSig_OI_next_state_val;
  user_output_data_10 : FDRE
    port map (
      D => Q_n0004(10),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(10)
    );
  user_output_data_11 : FDRE
    port map (
      D => Q_n0004(11),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(11)
    );
  user_output_data_12 : FDRE
    port map (
      D => Q_n0004(12),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(12)
    );
  user_output_data_13 : FDRE
    port map (
      D => Q_n0004(13),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(13)
    );
  user_output_data_14 : FDRE
    port map (
      D => Q_n0004(14),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(14)
    );
  user_output_data_15 : FDRE
    port map (
      D => Q_n0004(15),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(15)
    );
  user_output_data_16 : FDRE
    port map (
      D => Q_n0004(16),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(16)
    );
  user_output_data_17 : FDRE
    port map (
      D => Q_n0004(17),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(17)
    );
  user_output_data_18 : FDRE
    port map (
      D => Q_n0004(18),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(18)
    );
  user_output_data_19 : FDRE
    port map (
      D => Q_n0004(19),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(19)
    );
  user_output_data_20 : FDRE
    port map (
      D => Q_n0004(20),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(20)
    );
  user_output_data_21 : FDRE
    port map (
      D => Q_n0004(21),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(21)
    );
  user_output_data_22 : FDRE
    port map (
      D => Q_n0004(22),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(22)
    );
  user_output_data_23 : FDRE
    port map (
      D => Q_n0004(23),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(23)
    );
  user_output_data_24 : FDRE
    port map (
      D => Q_n0004(24),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(24)
    );
  user_output_data_25 : FDRE
    port map (
      D => Q_n0004(25),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(25)
    );
  user_output_data_26 : FDRE
    port map (
      D => Q_n0004(26),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(26)
    );
  user_output_data_27 : FDRE
    port map (
      D => Q_n0004(27),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(27)
    );
  user_output_data_28 : FDRE
    port map (
      D => Q_n0004(28),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(28)
    );
  user_output_data_29 : FDRE
    port map (
      D => Q_n0004(29),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(29)
    );
  user_output_data_30 : FDRE
    port map (
      D => Q_n0004(30),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(30)
    );
  user_output_data_31 : FDRE
    port map (
      D => Q_n0004(31),
      R => reset90_r,
      CE => Q_n0007,
      C => clk90,
      Q => user_output_data(31)
    );
  next_state : FDR
    port map (
      D => N4,
      R => reset90_r,
      C => clk90,
      Q => NlwRenamedSig_OI_next_state_val
    );
  Q_n0004_31_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_10_data_out(7),
      I2 => fifo_12_data_out(7),
      O => Q_n0004(31)
    );
  Q_n0004_30_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_10_data_out(6),
      I2 => fifo_12_data_out(6),
      O => Q_n0004(30)
    );
  Q_n0004_29_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_10_data_out(5),
      I2 => fifo_12_data_out(5),
      O => Q_n0004(29)
    );
  Q_n0004_28_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_10_data_out(4),
      I2 => fifo_12_data_out(4),
      O => Q_n0004(28)
    );
  Q_n0004_27_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_10_data_out(3),
      I2 => fifo_12_data_out(3),
      O => Q_n0004(27)
    );
  Q_n0004_26_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_10_data_out(2),
      I2 => fifo_12_data_out(2),
      O => Q_n0004(26)
    );
  Q_n0004_25_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_10_data_out(1),
      I2 => fifo_12_data_out(1),
      O => Q_n0004(25)
    );
  Q_n0004_24_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_10_data_out(0),
      I2 => fifo_12_data_out(0),
      O => Q_n0004(24)
    );
  Q_n0004_23_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_00_data_out(7),
      I2 => fifo_02_data_out(7),
      O => Q_n0004(23)
    );
  Q_n0004_22_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_00_data_out(6),
      I2 => fifo_02_data_out(6),
      O => Q_n0004(22)
    );
  Q_n0004_21_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_00_data_out(5),
      I2 => fifo_02_data_out(5),
      O => Q_n0004(21)
    );
  Q_n0004_20_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_00_data_out(4),
      I2 => fifo_02_data_out(4),
      O => Q_n0004(20)
    );
  Q_n0004_19_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_00_data_out(3),
      I2 => fifo_02_data_out(3),
      O => Q_n0004(19)
    );
  Q_n0004_18_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_00_data_out(2),
      I2 => fifo_02_data_out(2),
      O => Q_n0004(18)
    );
  Q_n0004_17_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_00_data_out(1),
      I2 => fifo_02_data_out(1),
      O => Q_n0004(17)
    );
  Q_n0004_16_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_00_data_out(0),
      I2 => fifo_02_data_out(0),
      O => Q_n0004(16)
    );
  Q_n0004_15_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_11_data_out(7),
      I2 => fifo_13_data_out(7),
      O => Q_n0004(15)
    );
  Q_n0004_14_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_11_data_out(6),
      I2 => fifo_13_data_out(6),
      O => Q_n0004(14)
    );
  Q_n0004_13_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_11_data_out(5),
      I2 => fifo_13_data_out(5),
      O => Q_n0004(13)
    );
  Q_n0004_12_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_11_data_out(4),
      I2 => fifo_13_data_out(4),
      O => Q_n0004(12)
    );
  Q_n0004_11_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_11_data_out(3),
      I2 => fifo_13_data_out(3),
      O => Q_n0004(11)
    );
  Q_n0004_10_1 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => fifo_11_data_out(2),
      I2 => fifo_13_data_out(2),
      O => Q_n0004(10)
    );
  Q_n00071 : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => read_valid_data_1,
      I2 => read_valid_data_2,
      O => Q_n0007
    );
  ddr2_dqbit15 : ddr2_dqbit
    port map (
      dq => dq(15),
      reset => reset270_r,
      dqs => dqs_delayed_col0(1),
      dqs_div_1 => dqs_div_col0(1),
      dqs_div_2 => dqs_div_col1(1),
      dqs1 => dqs_delayed_col1(1),
      fbit_0 => fbit_0(15),
      fbit_1 => fbit_1(15),
      fbit_2 => fbit_2(15),
      fbit_3 => fbit_3(15)
    );
  ddr2_dqbit14 : ddr2_dqbit_INST_1
    port map (
      dq => dq(14),
      reset => reset270_r,
      dqs => dqs_delayed_col0(1),
      dqs_div_1 => dqs_div_col0(1),
      dqs_div_2 => dqs_div_col1(1),
      dqs1 => dqs_delayed_col1(1),
      fbit_0 => fbit_0(14),
      fbit_1 => fbit_1(14),
      fbit_2 => fbit_2(14),
      fbit_3 => fbit_3(14)
    );
  ddr2_dqbit13 : ddr2_dqbit_INST_2
    port map (
      dq => dq(13),
      reset => reset270_r,
      dqs => dqs_delayed_col0(1),
      dqs_div_1 => dqs_div_col0(1),
      dqs_div_2 => dqs_div_col1(1),
      dqs1 => dqs_delayed_col1(1),
      fbit_0 => fbit_0(13),
      fbit_1 => fbit_1(13),
      fbit_2 => fbit_2(13),
      fbit_3 => fbit_3(13)
    );
  ddr2_dqbit12 : ddr2_dqbit_INST_3
    port map (
      dq => dq(12),
      reset => reset270_r,
      dqs => dqs_delayed_col0(1),
      dqs_div_1 => dqs_div_col0(1),
      dqs_div_2 => dqs_div_col1(1),
      dqs1 => dqs_delayed_col1(1),
      fbit_0 => fbit_0(12),
      fbit_1 => fbit_1(12),
      fbit_2 => fbit_2(12),
      fbit_3 => fbit_3(12)
    );
  ddr2_dqbit11 : ddr2_dqbit_INST_4
    port map (
      dq => dq(11),
      reset => reset270_r,
      dqs => dqs_delayed_col0(1),
      dqs_div_1 => dqs_div_col0(1),
      dqs_div_2 => dqs_div_col1(1),
      dqs1 => dqs_delayed_col1(1),
      fbit_0 => fbit_0(11),
      fbit_1 => fbit_1(11),
      fbit_2 => fbit_2(11),
      fbit_3 => fbit_3(11)
    );
  ddr2_dqbit10 : ddr2_dqbit_INST_5
    port map (
      dq => dq(10),
      reset => reset270_r,
      dqs => dqs_delayed_col0(1),
      dqs_div_1 => dqs_div_col0(1),
      dqs_div_2 => dqs_div_col1(1),
      dqs1 => dqs_delayed_col1(1),
      fbit_0 => fbit_0(10),
      fbit_1 => fbit_1(10),
      fbit_2 => fbit_2(10),
      fbit_3 => fbit_3(10)
    );
  ddr2_dqbit9 : ddr2_dqbit_INST_6
    port map (
      dq => dq(9),
      reset => reset270_r,
      dqs => dqs_delayed_col0(1),
      dqs_div_1 => dqs_div_col0(1),
      dqs_div_2 => dqs_div_col1(1),
      dqs1 => dqs_delayed_col1(1),
      fbit_0 => fbit_0(9),
      fbit_1 => fbit_1(9),
      fbit_2 => fbit_2(9),
      fbit_3 => fbit_3(9)
    );
  ddr2_dqbit8 : ddr2_dqbit_INST_7
    port map (
      dq => dq(8),
      reset => reset270_r,
      dqs => dqs_delayed_col0(1),
      dqs_div_1 => dqs_div_col0(1),
      dqs_div_2 => dqs_div_col1(1),
      dqs1 => dqs_delayed_col1(1),
      fbit_0 => fbit_0(8),
      fbit_1 => fbit_1(8),
      fbit_2 => fbit_2(8),
      fbit_3 => fbit_3(8)
    );
  ddr2_dqbit7 : ddr2_dqbit_INST_8
    port map (
      dq => dq(7),
      reset => reset270_r,
      dqs => dqs_delayed_col0(0),
      dqs_div_1 => dqs_div_col0(0),
      dqs_div_2 => dqs_div_col1(0),
      dqs1 => dqs_delayed_col1(0),
      fbit_0 => fbit_0(7),
      fbit_1 => fbit_1(7),
      fbit_2 => fbit_2(7),
      fbit_3 => fbit_3(7)
    );
  ddr2_dqbit6 : ddr2_dqbit_INST_9
    port map (
      dq => dq(6),
      reset => reset270_r,
      dqs => dqs_delayed_col0(0),
      dqs_div_1 => dqs_div_col0(0),
      dqs_div_2 => dqs_div_col1(0),
      dqs1 => dqs_delayed_col1(0),
      fbit_0 => fbit_0(6),
      fbit_1 => fbit_1(6),
      fbit_2 => fbit_2(6),
      fbit_3 => fbit_3(6)
    );
  ddr2_dqbit5 : ddr2_dqbit_INST_10
    port map (
      dq => dq(5),
      reset => reset270_r,
      dqs => dqs_delayed_col0(0),
      dqs_div_1 => dqs_div_col0(0),
      dqs_div_2 => dqs_div_col1(0),
      dqs1 => dqs_delayed_col1(0),
      fbit_0 => fbit_0(5),
      fbit_1 => fbit_1(5),
      fbit_2 => fbit_2(5),
      fbit_3 => fbit_3(5)
    );
  ddr2_dqbit4 : ddr2_dqbit_INST_11
    port map (
      dq => dq(4),
      reset => reset270_r,
      dqs => dqs_delayed_col0(0),
      dqs_div_1 => dqs_div_col0(0),
      dqs_div_2 => dqs_div_col1(0),
      dqs1 => dqs_delayed_col1(0),
      fbit_0 => fbit_0(4),
      fbit_1 => fbit_1(4),
      fbit_2 => fbit_2(4),
      fbit_3 => fbit_3(4)
    );
  ddr2_dqbit3 : ddr2_dqbit_INST_12
    port map (
      dq => dq(3),
      reset => reset270_r,
      dqs => dqs_delayed_col0(0),
      dqs_div_1 => dqs_div_col0(0),
      dqs_div_2 => dqs_div_col1(0),
      dqs1 => dqs_delayed_col1(0),
      fbit_0 => fbit_0(3),
      fbit_1 => fbit_1(3),
      fbit_2 => fbit_2(3),
      fbit_3 => fbit_3(3)
    );
  ddr2_dqbit2 : ddr2_dqbit_INST_13
    port map (
      dq => dq(2),
      reset => reset270_r,
      dqs => dqs_delayed_col0(0),
      dqs_div_1 => dqs_div_col0(0),
      dqs_div_2 => dqs_div_col1(0),
      dqs1 => dqs_delayed_col1(0),
      fbit_0 => fbit_0(2),
      fbit_1 => fbit_1(2),
      fbit_2 => fbit_2(2),
      fbit_3 => fbit_3(2)
    );
  ddr2_dqbit1 : ddr2_dqbit_INST_14
    port map (
      dq => dq(1),
      reset => reset270_r,
      dqs => dqs_delayed_col0(0),
      dqs_div_1 => dqs_div_col0(0),
      dqs_div_2 => dqs_div_col1(0),
      dqs1 => dqs_delayed_col1(0),
      fbit_0 => fbit_0(1),
      fbit_1 => fbit_1(1),
      fbit_2 => fbit_2(1),
      fbit_3 => fbit_3(1)
    );
  ddr2_dqbit0 : ddr2_dqbit_INST_15
    port map (
      dq => dq(0),
      reset => reset270_r,
      dqs => dqs_delayed_col0(0),
      dqs_div_1 => dqs_div_col0(0),
      dqs_div_2 => dqs_div_col1(0),
      dqs1 => dqs_delayed_col1(0),
      fbit_0 => fbit_0(0),
      fbit_1 => fbit_1(0),
      fbit_2 => fbit_2(0),
      fbit_3 => fbit_3(0)
    );
  ram_8d_dqs1_fbit3 : RAM_8D
    port map (
      WCLK => clk90,
      DPRA0 => fifo_13_rd_addr(0),
      DPRA1 => fifo_13_rd_addr(1),
      DPRA2 => fifo_13_rd_addr(2),
      DPRA3 => fifo_13_rd_addr(3),
      WE => transfer_done_1(3),
      A0 => fifo_13_wr_addr(0),
      A1 => fifo_13_wr_addr(1),
      A2 => fifo_13_wr_addr(2),
      A3 => fifo_13_wr_addr(3),
      D(7) => fbit_3(15),
      D(6) => fbit_3(14),
      D(5) => fbit_3(13),
      D(4) => fbit_3(12),
      D(3) => fbit_3(11),
      D(2) => fbit_3(10),
      D(1) => fbit_3(9),
      D(0) => fbit_3(8),
      DPO(7) => fifo_13_data_out(7),
      DPO(6) => fifo_13_data_out(6),
      DPO(5) => fifo_13_data_out(5),
      DPO(4) => fifo_13_data_out(4),
      DPO(3) => fifo_13_data_out(3),
      DPO(2) => fifo_13_data_out(2),
      DPO(1) => NLW_ram_8d_dqs1_fbit3_DPO_1_UNCONNECTED,
      DPO(0) => NLW_ram_8d_dqs1_fbit3_DPO_0_UNCONNECTED
    );
  ram_8d_dqs1_fbit2 : RAM_8D_INST_1
    port map (
      WCLK => clk90,
      DPRA0 => fifo_12_rd_addr(0),
      DPRA1 => fifo_12_rd_addr(1),
      DPRA2 => fifo_12_rd_addr(2),
      DPRA3 => fifo_12_rd_addr(3),
      WE => transfer_done_1(2),
      A0 => fifo_12_wr_addr(0),
      A1 => fifo_12_wr_addr(1),
      A2 => fifo_12_wr_addr(2),
      A3 => fifo_12_wr_addr(3),
      D(7) => fbit_2(15),
      D(6) => fbit_2(14),
      D(5) => fbit_2(13),
      D(4) => fbit_2(12),
      D(3) => fbit_2(11),
      D(2) => fbit_2(10),
      D(1) => fbit_2(9),
      D(0) => fbit_2(8),
      DPO(7) => fifo_12_data_out(7),
      DPO(6) => fifo_12_data_out(6),
      DPO(5) => fifo_12_data_out(5),
      DPO(4) => fifo_12_data_out(4),
      DPO(3) => fifo_12_data_out(3),
      DPO(2) => fifo_12_data_out(2),
      DPO(1) => fifo_12_data_out(1),
      DPO(0) => fifo_12_data_out(0)
    );
  ram_8d_dqs1_fbit1 : RAM_8D_INST_2
    port map (
      WCLK => clk90,
      DPRA0 => fifo_11_rd_addr(0),
      DPRA1 => fifo_11_rd_addr(1),
      DPRA2 => fifo_11_rd_addr(2),
      DPRA3 => fifo_11_rd_addr(3),
      WE => transfer_done_1(1),
      A0 => fifo_11_wr_addr(0),
      A1 => fifo_11_wr_addr(1),
      A2 => fifo_11_wr_addr(2),
      A3 => fifo_11_wr_addr(3),
      D(7) => fbit_1(15),
      D(6) => fbit_1(14),
      D(5) => fbit_1(13),
      D(4) => fbit_1(12),
      D(3) => fbit_1(11),
      D(2) => fbit_1(10),
      D(1) => fbit_1(9),
      D(0) => fbit_1(8),
      DPO(7) => fifo_11_data_out(7),
      DPO(6) => fifo_11_data_out(6),
      DPO(5) => fifo_11_data_out(5),
      DPO(4) => fifo_11_data_out(4),
      DPO(3) => fifo_11_data_out(3),
      DPO(2) => fifo_11_data_out(2),
      DPO(1) => NLW_ram_8d_dqs1_fbit1_DPO_1_UNCONNECTED,
      DPO(0) => NLW_ram_8d_dqs1_fbit1_DPO_0_UNCONNECTED
    );
  ram_8d_dqs1_fbit0 : RAM_8D_INST_3
    port map (
      WCLK => clk90,
      DPRA0 => fifo_10_rd_addr(0),
      DPRA1 => fifo_10_rd_addr(1),
      DPRA2 => fifo_10_rd_addr(2),
      DPRA3 => fifo_10_rd_addr(3),
      WE => transfer_done_1(0),
      A0 => fifo_10_wr_addr(0),
      A1 => fifo_10_wr_addr(1),
      A2 => fifo_10_wr_addr(2),
      A3 => fifo_10_wr_addr(3),
      D(7) => fbit_0(15),
      D(6) => fbit_0(14),
      D(5) => fbit_0(13),
      D(4) => fbit_0(12),
      D(3) => fbit_0(11),
      D(2) => fbit_0(10),
      D(1) => fbit_0(9),
      D(0) => fbit_0(8),
      DPO(7) => fifo_10_data_out(7),
      DPO(6) => fifo_10_data_out(6),
      DPO(5) => fifo_10_data_out(5),
      DPO(4) => fifo_10_data_out(4),
      DPO(3) => fifo_10_data_out(3),
      DPO(2) => fifo_10_data_out(2),
      DPO(1) => fifo_10_data_out(1),
      DPO(0) => fifo_10_data_out(0)
    );
  ram_8d_dqs0_fbit3 : RAM_8D_INST_4
    port map (
      WCLK => clk90,
      DPRA0 => fifo_03_rd_addr(0),
      DPRA1 => fifo_03_rd_addr(1),
      DPRA2 => fifo_03_rd_addr(2),
      DPRA3 => fifo_03_rd_addr(3),
      WE => transfer_done_0(3),
      A0 => fifo_03_wr_addr(0),
      A1 => fifo_03_wr_addr(1),
      A2 => fifo_03_wr_addr(2),
      A3 => fifo_03_wr_addr(3),
      D(7) => fbit_3(7),
      D(6) => fbit_3(6),
      D(5) => fbit_3(5),
      D(4) => fbit_3(4),
      D(3) => fbit_3(3),
      D(2) => fbit_3(2),
      D(1) => fbit_3(1),
      D(0) => fbit_3(0),
      DPO(7) => NLW_ram_8d_dqs0_fbit3_DPO_7_UNCONNECTED,
      DPO(6) => NLW_ram_8d_dqs0_fbit3_DPO_6_UNCONNECTED,
      DPO(5) => NLW_ram_8d_dqs0_fbit3_DPO_5_UNCONNECTED,
      DPO(4) => NLW_ram_8d_dqs0_fbit3_DPO_4_UNCONNECTED,
      DPO(3) => NLW_ram_8d_dqs0_fbit3_DPO_3_UNCONNECTED,
      DPO(2) => NLW_ram_8d_dqs0_fbit3_DPO_2_UNCONNECTED,
      DPO(1) => NLW_ram_8d_dqs0_fbit3_DPO_1_UNCONNECTED,
      DPO(0) => NLW_ram_8d_dqs0_fbit3_DPO_0_UNCONNECTED
    );
  ram_8d_dqs0_fbit2 : RAM_8D_INST_5
    port map (
      WCLK => clk90,
      DPRA0 => fifo_02_rd_addr(0),
      DPRA1 => fifo_02_rd_addr(1),
      DPRA2 => fifo_02_rd_addr(2),
      DPRA3 => fifo_02_rd_addr(3),
      WE => transfer_done_0(2),
      A0 => fifo_02_wr_addr(0),
      A1 => fifo_02_wr_addr(1),
      A2 => fifo_02_wr_addr(2),
      A3 => fifo_02_wr_addr(3),
      D(7) => fbit_2(7),
      D(6) => fbit_2(6),
      D(5) => fbit_2(5),
      D(4) => fbit_2(4),
      D(3) => fbit_2(3),
      D(2) => fbit_2(2),
      D(1) => fbit_2(1),
      D(0) => fbit_2(0),
      DPO(7) => fifo_02_data_out(7),
      DPO(6) => fifo_02_data_out(6),
      DPO(5) => fifo_02_data_out(5),
      DPO(4) => fifo_02_data_out(4),
      DPO(3) => fifo_02_data_out(3),
      DPO(2) => fifo_02_data_out(2),
      DPO(1) => fifo_02_data_out(1),
      DPO(0) => fifo_02_data_out(0)
    );
  ram_8d_dqs0_fbit1 : RAM_8D_INST_6
    port map (
      WCLK => clk90,
      DPRA0 => fifo_01_rd_addr(0),
      DPRA1 => fifo_01_rd_addr(1),
      DPRA2 => fifo_01_rd_addr(2),
      DPRA3 => fifo_01_rd_addr(3),
      WE => transfer_done_0(1),
      A0 => fifo_01_wr_addr(0),
      A1 => fifo_01_wr_addr(1),
      A2 => fifo_01_wr_addr(2),
      A3 => fifo_01_wr_addr(3),
      D(7) => fbit_1(7),
      D(6) => fbit_1(6),
      D(5) => fbit_1(5),
      D(4) => fbit_1(4),
      D(3) => fbit_1(3),
      D(2) => fbit_1(2),
      D(1) => fbit_1(1),
      D(0) => fbit_1(0),
      DPO(7) => NLW_ram_8d_dqs0_fbit1_DPO_7_UNCONNECTED,
      DPO(6) => NLW_ram_8d_dqs0_fbit1_DPO_6_UNCONNECTED,
      DPO(5) => NLW_ram_8d_dqs0_fbit1_DPO_5_UNCONNECTED,
      DPO(4) => NLW_ram_8d_dqs0_fbit1_DPO_4_UNCONNECTED,
      DPO(3) => NLW_ram_8d_dqs0_fbit1_DPO_3_UNCONNECTED,
      DPO(2) => NLW_ram_8d_dqs0_fbit1_DPO_2_UNCONNECTED,
      DPO(1) => NLW_ram_8d_dqs0_fbit1_DPO_1_UNCONNECTED,
      DPO(0) => NLW_ram_8d_dqs0_fbit1_DPO_0_UNCONNECTED
    );
  ram_8d_dqs0_fbit0 : RAM_8D_INST_7
    port map (
      WCLK => clk90,
      DPRA0 => fifo_00_rd_addr(0),
      DPRA1 => fifo_00_rd_addr(1),
      DPRA2 => fifo_00_rd_addr(2),
      DPRA3 => fifo_00_rd_addr(3),
      WE => transfer_done_0(0),
      A0 => fifo_00_wr_addr(0),
      A1 => fifo_00_wr_addr(1),
      A2 => fifo_00_wr_addr(2),
      A3 => fifo_00_wr_addr(3),
      D(7) => fbit_0(7),
      D(6) => fbit_0(6),
      D(5) => fbit_0(5),
      D(4) => fbit_0(4),
      D(3) => fbit_0(3),
      D(2) => fbit_0(2),
      D(1) => fbit_0(1),
      D(0) => fbit_0(0),
      DPO(7) => fifo_00_data_out(7),
      DPO(6) => fifo_00_data_out(6),
      DPO(5) => fifo_00_data_out(5),
      DPO(4) => fifo_00_data_out(4),
      DPO(3) => fifo_00_data_out(3),
      DPO(2) => fifo_00_data_out(2),
      DPO(1) => fifo_00_data_out(1),
      DPO(0) => fifo_00_data_out(0)
    );
  Mrom_data_Mrom_n0005 : LUT3
    generic map(
      INIT => X"4E"
    )
    port map (
      I0 => NlwRenamedSig_OI_next_state_val,
      I1 => read_valid_data_1,
      I2 => read_valid_data_2,
      O => N4
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_transfer_done_INST_1 is
  port (
    reset270 : in STD_LOGIC := 'X'; 
    dqs_div : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    clk0 : in STD_LOGIC := 'X'; 
    clk180 : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    reset90 : in STD_LOGIC := 'X'; 
    reset180 : in STD_LOGIC := 'X'; 
    transfer_done0 : out STD_LOGIC; 
    transfer_done1 : out STD_LOGIC; 
    transfer_done2 : out STD_LOGIC; 
    transfer_done3 : out STD_LOGIC 
  );
end ddr2_transfer_done_INST_1;

architecture Structure of ddr2_transfer_done_INST_1 is
  signal transfer_done1_clk270 : STD_LOGIC; 
  signal transfer_done0_clk0 : STD_LOGIC; 
  signal transfer_done0_clk180 : STD_LOGIC; 
  signal transfer_done0_clk270 : STD_LOGIC; 
  signal sync_rst_clk270 : STD_LOGIC; 
  signal sync_rst_clk90 : STD_LOGIC; 
  signal sync_rst_xdone0_ck180 : STD_LOGIC; 
  signal transfer_done3_clk90 : STD_LOGIC; 
  signal transfer_done3_clk270 : STD_LOGIC; 
  signal transfer_done2_clk90 : STD_LOGIC; 
  signal sync_rst_xdone0_ck0 : STD_LOGIC; 
  signal transfer_done1_clk90 : STD_LOGIC; 
  signal transfer_done0_clk90 : STD_LOGIC; 
  signal transfer_done2_clk270 : STD_LOGIC; 
begin
  xdone0_clk0 : FDR
    port map (
      D => dqs_div,
      R => sync_rst_xdone0_ck0,
      C => clk0,
      Q => transfer_done0_clk0
    );
  xdone0_clk90 : FDR
    port map (
      D => transfer_done0_clk0,
      R => sync_rst_clk90,
      C => clk90,
      Q => transfer_done0_clk90
    );
  xdone0_clk180 : FDR
    port map (
      D => dqs_div,
      R => sync_rst_xdone0_ck180,
      C => clk180,
      Q => transfer_done0_clk180
    );
  xdone0_clk270 : FDR
    port map (
      D => transfer_done0_clk180,
      R => sync_rst_clk270,
      C => clk270,
      Q => transfer_done0_clk270
    );
  xdone1_clk90 : FD
    port map (
      D => transfer_done0_clk270,
      C => clk90,
      Q => transfer_done1_clk90
    );
  xdone1_clk270 : FD
    port map (
      D => transfer_done0_clk90,
      C => clk270,
      Q => transfer_done1_clk270
    );
  xdone2_clk90 : FD
    port map (
      D => transfer_done1_clk270,
      C => clk90,
      Q => transfer_done2_clk90
    );
  xdone2_clk270 : FD
    port map (
      D => transfer_done1_clk90,
      C => clk270,
      Q => transfer_done2_clk270
    );
  xdone3_clk90 : FD
    port map (
      D => transfer_done2_clk270,
      C => clk90,
      Q => transfer_done3_clk90
    );
  xdone3_clk270 : FD
    port map (
      D => transfer_done2_clk90,
      C => clk270,
      Q => transfer_done3_clk270
    );
  xdone0 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => transfer_done0_clk90,
      I1 => transfer_done0_clk270,
      O => transfer_done0
    );
  xdone1 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => transfer_done1_clk90,
      I1 => transfer_done1_clk270,
      O => transfer_done1
    );
  xdone2 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => transfer_done2_clk90,
      I1 => transfer_done2_clk270,
      O => transfer_done2
    );
  xdone3 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => transfer_done3_clk90,
      I1 => transfer_done3_clk270,
      O => transfer_done3
    );
  xdone0_rst90 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => reset90,
      I1 => transfer_done0_clk270,
      I2 => transfer_done0_clk90,
      O => sync_rst_clk90
    );
  xdone0_rst270 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => reset270,
      I1 => transfer_done0_clk270,
      I2 => transfer_done0_clk90,
      O => sync_rst_clk270
    );
  sync_rst_xdone0_ck1801 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => reset180,
      I1 => transfer_done0_clk180,
      O => sync_rst_xdone0_ck180
    );
  sync_rst_xdone0_ck01 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => reset,
      I1 => transfer_done0_clk0,
      O => sync_rst_xdone0_ck0
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_transfer_done is
  port (
    reset270 : in STD_LOGIC := 'X'; 
    dqs_div : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    clk0 : in STD_LOGIC := 'X'; 
    clk180 : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    reset90 : in STD_LOGIC := 'X'; 
    reset180 : in STD_LOGIC := 'X'; 
    transfer_done0 : out STD_LOGIC; 
    transfer_done1 : out STD_LOGIC; 
    transfer_done2 : out STD_LOGIC; 
    transfer_done3 : out STD_LOGIC 
  );
end ddr2_transfer_done;

architecture Structure of ddr2_transfer_done is
  signal transfer_done1_clk270 : STD_LOGIC; 
  signal transfer_done0_clk0 : STD_LOGIC; 
  signal transfer_done0_clk180 : STD_LOGIC; 
  signal transfer_done0_clk270 : STD_LOGIC; 
  signal sync_rst_clk270 : STD_LOGIC; 
  signal sync_rst_clk90 : STD_LOGIC; 
  signal sync_rst_xdone0_ck180 : STD_LOGIC; 
  signal transfer_done3_clk90 : STD_LOGIC; 
  signal transfer_done3_clk270 : STD_LOGIC; 
  signal transfer_done2_clk90 : STD_LOGIC; 
  signal sync_rst_xdone0_ck0 : STD_LOGIC; 
  signal transfer_done1_clk90 : STD_LOGIC; 
  signal transfer_done0_clk90 : STD_LOGIC; 
  signal transfer_done2_clk270 : STD_LOGIC; 
begin
  xdone0_clk0 : FDR
    port map (
      D => dqs_div,
      R => sync_rst_xdone0_ck0,
      C => clk0,
      Q => transfer_done0_clk0
    );
  xdone0_clk90 : FDR
    port map (
      D => transfer_done0_clk0,
      R => sync_rst_clk90,
      C => clk90,
      Q => transfer_done0_clk90
    );
  xdone0_clk180 : FDR
    port map (
      D => dqs_div,
      R => sync_rst_xdone0_ck180,
      C => clk180,
      Q => transfer_done0_clk180
    );
  xdone0_clk270 : FDR
    port map (
      D => transfer_done0_clk180,
      R => sync_rst_clk270,
      C => clk270,
      Q => transfer_done0_clk270
    );
  xdone1_clk90 : FD
    port map (
      D => transfer_done0_clk270,
      C => clk90,
      Q => transfer_done1_clk90
    );
  xdone1_clk270 : FD
    port map (
      D => transfer_done0_clk90,
      C => clk270,
      Q => transfer_done1_clk270
    );
  xdone2_clk90 : FD
    port map (
      D => transfer_done1_clk270,
      C => clk90,
      Q => transfer_done2_clk90
    );
  xdone2_clk270 : FD
    port map (
      D => transfer_done1_clk90,
      C => clk270,
      Q => transfer_done2_clk270
    );
  xdone3_clk90 : FD
    port map (
      D => transfer_done2_clk270,
      C => clk90,
      Q => transfer_done3_clk90
    );
  xdone3_clk270 : FD
    port map (
      D => transfer_done2_clk90,
      C => clk270,
      Q => transfer_done3_clk270
    );
  xdone0 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => transfer_done0_clk90,
      I1 => transfer_done0_clk270,
      O => transfer_done0
    );
  xdone1 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => transfer_done1_clk90,
      I1 => transfer_done1_clk270,
      O => transfer_done1
    );
  xdone2 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => transfer_done2_clk90,
      I1 => transfer_done2_clk270,
      O => transfer_done2
    );
  xdone3 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => transfer_done3_clk90,
      I1 => transfer_done3_clk270,
      O => transfer_done3
    );
  xdone0_rst90 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => reset90,
      I1 => transfer_done0_clk270,
      I2 => transfer_done0_clk90,
      O => sync_rst_clk90
    );
  xdone0_rst270 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => reset270,
      I1 => transfer_done0_clk270,
      I2 => transfer_done0_clk90,
      O => sync_rst_clk270
    );
  sync_rst_xdone0_ck1801 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => reset180,
      I1 => transfer_done0_clk180,
      O => sync_rst_xdone0_ck180
    );
  sync_rst_xdone0_ck01 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => reset,
      I1 => transfer_done0_clk0,
      O => sync_rst_xdone0_ck0
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqs_div_INST_1 is
  port (
    reset : in STD_LOGIC := 'X'; 
    rst_dqs_div_delayed : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    dqs_divn : out STD_LOGIC; 
    dqs_divp : out STD_LOGIC 
  );
end ddr2_dqs_div_INST_1;

architecture Structure of ddr2_dqs_div_INST_1 is
  signal NlwRenamedSig_OI_dqs_divp : STD_LOGIC; 
  signal dqs_n : STD_LOGIC; 
  signal dqs_div0n : STD_LOGIC; 
begin
  dqs_divp <= NlwRenamedSig_OI_dqs_divp;
  col1 : FDC
    port map (
      D => dqs_div0n,
      CLR => rst_dqs_div_delayed,
      C => dqs1,
      Q => NlwRenamedSig_OI_dqs_divp
    );
  col0 : FDC
    port map (
      D => NlwRenamedSig_OI_dqs_divp,
      CLR => reset,
      C => dqs_n,
      Q => dqs_divn
    );
  dqs_div0n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_dqs_divp,
      O => dqs_div0n
    );
  dqs_n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqs_n
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dqs_div is
  port (
    reset : in STD_LOGIC := 'X'; 
    rst_dqs_div_delayed : in STD_LOGIC := 'X'; 
    dqs : in STD_LOGIC := 'X'; 
    dqs1 : in STD_LOGIC := 'X'; 
    dqs_divn : out STD_LOGIC; 
    dqs_divp : out STD_LOGIC 
  );
end ddr2_dqs_div;

architecture Structure of ddr2_dqs_div is
  signal NlwRenamedSig_OI_dqs_divp : STD_LOGIC; 
  signal dqs_n : STD_LOGIC; 
  signal dqs_div0n : STD_LOGIC; 
begin
  dqs_divp <= NlwRenamedSig_OI_dqs_divp;
  col1 : FDC
    port map (
      D => dqs_div0n,
      CLR => rst_dqs_div_delayed,
      C => dqs1,
      Q => NlwRenamedSig_OI_dqs_divp
    );
  col0 : FDC
    port map (
      D => NlwRenamedSig_OI_dqs_divp,
      CLR => reset,
      C => dqs_n,
      Q => dqs_divn
    );
  dqs_div0n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_dqs_divp,
      O => dqs_div0n
    );
  dqs_n1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs,
      O => dqs_n
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity data_read_controller_16bit_rl is
  port (
    rst_dqs_div : in STD_LOGIC := 'X'; 
    clk : in STD_LOGIC := 'X'; 
    reset_r : in STD_LOGIC := 'X'; 
    reset270_r : in STD_LOGIC := 'X'; 
    reset90_r : in STD_LOGIC := 'X'; 
    clk180 : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    reset180_r : in STD_LOGIC := 'X'; 
    dqs_int_delay_in0 : in STD_LOGIC := 'X'; 
    dqs_int_delay_in1 : in STD_LOGIC := 'X'; 
    next_state : in STD_LOGIC := 'X'; 
    read_valid_data_1_val : out STD_LOGIC; 
    read_valid_data_2_val : out STD_LOGIC; 
    u_data_val : out STD_LOGIC; 
    delay_sel : in STD_LOGIC_VECTOR ( 4 downto 0 ); 
    fifo_01_rd_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_11_wr_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_11_rd_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    dqs_div_col1_val : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
    dqs_delayed_col1_val : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
    transfer_done_0_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_02_wr_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_02_rd_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_12_wr_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    transfer_done_1_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_12_rd_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_03_wr_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_00_wr_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_03_rd_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_00_rd_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_13_wr_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_10_wr_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_13_rd_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    fifo_10_rd_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    dqs_div_col0_val : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
    dqs_delayed_col0_val : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
    fifo_01_wr_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ) 
  );
end data_read_controller_16bit_rl;

architecture Structure of data_read_controller_16bit_rl is
  component ddr2_dqs_div
    port (
      reset : in STD_LOGIC := 'X'; 
      rst_dqs_div_delayed : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      dqs_divn : out STD_LOGIC; 
      dqs_divp : out STD_LOGIC 
    );
  end component;
  component ddr2_dqs_div_INST_1
    port (
      reset : in STD_LOGIC := 'X'; 
      rst_dqs_div_delayed : in STD_LOGIC := 'X'; 
      dqs : in STD_LOGIC := 'X'; 
      dqs1 : in STD_LOGIC := 'X'; 
      dqs_divn : out STD_LOGIC; 
      dqs_divp : out STD_LOGIC 
    );
  end component;
  component ddr2_transfer_done
    port (
      reset270 : in STD_LOGIC := 'X'; 
      dqs_div : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      clk0 : in STD_LOGIC := 'X'; 
      clk180 : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      reset90 : in STD_LOGIC := 'X'; 
      reset180 : in STD_LOGIC := 'X'; 
      transfer_done0 : out STD_LOGIC; 
      transfer_done1 : out STD_LOGIC; 
      transfer_done2 : out STD_LOGIC; 
      transfer_done3 : out STD_LOGIC 
    );
  end component;
  component ddr2_transfer_done_INST_1
    port (
      reset270 : in STD_LOGIC := 'X'; 
      dqs_div : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      clk0 : in STD_LOGIC := 'X'; 
      clk180 : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      reset90 : in STD_LOGIC := 'X'; 
      reset180 : in STD_LOGIC := 'X'; 
      transfer_done0 : out STD_LOGIC; 
      transfer_done1 : out STD_LOGIC; 
      transfer_done2 : out STD_LOGIC; 
      transfer_done3 : out STD_LOGIC 
    );
  end component;
  signal NlwRenamedSig_OI_read_valid_data_1_val : STD_LOGIC; 
  signal NlwRenamedSig_OI_read_valid_data_2_val : STD_LOGIC; 
  signal fifo_03_not_empty : STD_LOGIC; 
  signal fifo_01_not_empty_r_96 : STD_LOGIC; 
  signal rst_dqs_div_delayed : STD_LOGIC; 
  signal Q_n0008 : STD_LOGIC; 
  signal Q_n0018 : STD_LOGIC; 
  signal Q_n0019 : STD_LOGIC; 
  signal Q_n0046 : STD_LOGIC; 
  signal Q_n0047 : STD_LOGIC; 
  signal read_valid_data_1 : STD_LOGIC; 
  signal read_valid_data_2 : STD_LOGIC; 
  signal fifo_01_not_empty_r1_97 : STD_LOGIC; 
  signal rd_data_valid_reg_98 : STD_LOGIC; 
  signal fifo_03_not_empty_r_99 : STD_LOGIC; 
  signal fifo_01_not_empty : STD_LOGIC; 
  signal read_valid_data : STD_LOGIC; 
  signal rst_dqs_div_int : STD_LOGIC; 
  signal fifo_03_not_empty_r1_100 : STD_LOGIC; 
  signal reset90_r_INV : STD_LOGIC; 
  signal N28 : STD_LOGIC; 
  signal N51 : STD_LOGIC; 
  signal N77 : STD_LOGIC; 
  signal N100 : STD_LOGIC; 
  signal dqs_delay1_col1_delay5 : STD_LOGIC; 
  signal dqs_delay1_col1_delay4 : STD_LOGIC; 
  signal dqs_delay1_col1_delay3 : STD_LOGIC; 
  signal dqs_delay1_col1_delay2 : STD_LOGIC; 
  signal dqs_delay1_col1_delay1 : STD_LOGIC; 
  signal dqs_delay1_col1_high : STD_LOGIC; 
  signal dqs_delay1_col0_delay5 : STD_LOGIC; 
  signal dqs_delay1_col0_delay4 : STD_LOGIC; 
  signal dqs_delay1_col0_delay3 : STD_LOGIC; 
  signal dqs_delay1_col0_delay2 : STD_LOGIC; 
  signal dqs_delay1_col0_delay1 : STD_LOGIC; 
  signal dqs_delay1_col0_high : STD_LOGIC; 
  signal dqs_delay0_col1_delay5 : STD_LOGIC; 
  signal dqs_delay0_col1_delay4 : STD_LOGIC; 
  signal dqs_delay0_col1_delay3 : STD_LOGIC; 
  signal dqs_delay0_col1_delay2 : STD_LOGIC; 
  signal dqs_delay0_col1_delay1 : STD_LOGIC; 
  signal dqs_delay0_col1_high : STD_LOGIC; 
  signal dqs_delay0_col0_delay5 : STD_LOGIC; 
  signal dqs_delay0_col0_delay4 : STD_LOGIC; 
  signal dqs_delay0_col0_delay3 : STD_LOGIC; 
  signal dqs_delay0_col0_delay2 : STD_LOGIC; 
  signal dqs_delay0_col0_delay1 : STD_LOGIC; 
  signal dqs_delay0_col0_high : STD_LOGIC; 
  signal rst_dqs_div_delay0_delay5 : STD_LOGIC; 
  signal rst_dqs_div_delay0_delay4 : STD_LOGIC; 
  signal rst_dqs_div_delay0_delay3 : STD_LOGIC; 
  signal rst_dqs_div_delay0_delay2 : STD_LOGIC; 
  signal rst_dqs_div_delay0_delay1 : STD_LOGIC; 
  signal rst_dqs_div_delay0_high : STD_LOGIC; 
  signal NlwRenamedSig_OI_fifo_01_rd_addr_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal NlwRenamedSig_OI_fifo_11_wr_addr_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal NlwRenamedSig_OI_fifo_11_rd_addr_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal NlwRenamedSig_OI_dqs_div_col1_val : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal NlwRenamedSig_OI_dqs_delayed_col1_val : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal NlwRenamedSig_OI_transfer_done_0_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal NlwRenamedSig_OI_fifo_02_wr_addr_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal NlwRenamedSig_OI_fifo_02_rd_addr_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal NlwRenamedSig_OI_fifo_12_wr_addr_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal NlwRenamedSig_OI_transfer_done_1_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal NlwRenamedSig_OI_fifo_12_rd_addr_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal NlwRenamedSig_OI_fifo_03_wr_addr_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal NlwRenamedSig_OI_fifo_00_wr_addr_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal NlwRenamedSig_OI_fifo_03_rd_addr_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal NlwRenamedSig_OI_fifo_00_rd_addr_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal NlwRenamedSig_OI_fifo_13_wr_addr_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal NlwRenamedSig_OI_fifo_10_wr_addr_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal NlwRenamedSig_OI_fifo_13_rd_addr_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal NlwRenamedSig_OI_fifo_10_rd_addr_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal NlwRenamedSig_OI_dqs_delayed_col0_val : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal NlwRenamedSig_OI_fifo_01_wr_addr_val : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_11_rd_addr_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_12_rd_addr_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_13_rd_addr_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_00_wr_addr_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_01_wr_addr_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_02_wr_addr_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_03_wr_addr_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_10_wr_addr_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_11_wr_addr_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_12_wr_addr_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_13_wr_addr_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_00_rd_addr_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_01_rd_addr_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_02_rd_addr_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_03_rd_addr_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_10_rd_addr_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  read_valid_data_1_val <= NlwRenamedSig_OI_read_valid_data_1_val;
  read_valid_data_2_val <= NlwRenamedSig_OI_read_valid_data_2_val;
  fifo_01_rd_addr_val(3) <= NlwRenamedSig_OI_fifo_01_rd_addr_val(3);
  fifo_01_rd_addr_val(2) <= NlwRenamedSig_OI_fifo_01_rd_addr_val(2);
  fifo_01_rd_addr_val(1) <= NlwRenamedSig_OI_fifo_01_rd_addr_val(1);
  fifo_01_rd_addr_val(0) <= NlwRenamedSig_OI_fifo_01_rd_addr_val(0);
  fifo_11_wr_addr_val(3) <= NlwRenamedSig_OI_fifo_11_wr_addr_val(3);
  fifo_11_wr_addr_val(2) <= NlwRenamedSig_OI_fifo_11_wr_addr_val(2);
  fifo_11_wr_addr_val(1) <= NlwRenamedSig_OI_fifo_11_wr_addr_val(1);
  fifo_11_wr_addr_val(0) <= NlwRenamedSig_OI_fifo_11_wr_addr_val(0);
  fifo_11_rd_addr_val(3) <= NlwRenamedSig_OI_fifo_11_rd_addr_val(3);
  fifo_11_rd_addr_val(2) <= NlwRenamedSig_OI_fifo_11_rd_addr_val(2);
  fifo_11_rd_addr_val(1) <= NlwRenamedSig_OI_fifo_11_rd_addr_val(1);
  fifo_11_rd_addr_val(0) <= NlwRenamedSig_OI_fifo_11_rd_addr_val(0);
  dqs_div_col1_val(1) <= NlwRenamedSig_OI_dqs_div_col1_val(1);
  dqs_div_col1_val(0) <= NlwRenamedSig_OI_dqs_div_col1_val(0);
  dqs_delayed_col1_val(1) <= NlwRenamedSig_OI_dqs_delayed_col1_val(1);
  dqs_delayed_col1_val(0) <= NlwRenamedSig_OI_dqs_delayed_col1_val(0);
  transfer_done_0_val(3) <= NlwRenamedSig_OI_transfer_done_0_val(3);
  transfer_done_0_val(2) <= NlwRenamedSig_OI_transfer_done_0_val(2);
  transfer_done_0_val(1) <= NlwRenamedSig_OI_transfer_done_0_val(1);
  transfer_done_0_val(0) <= NlwRenamedSig_OI_transfer_done_0_val(0);
  fifo_02_wr_addr_val(3) <= NlwRenamedSig_OI_fifo_02_wr_addr_val(3);
  fifo_02_wr_addr_val(2) <= NlwRenamedSig_OI_fifo_02_wr_addr_val(2);
  fifo_02_wr_addr_val(1) <= NlwRenamedSig_OI_fifo_02_wr_addr_val(1);
  fifo_02_wr_addr_val(0) <= NlwRenamedSig_OI_fifo_02_wr_addr_val(0);
  fifo_02_rd_addr_val(3) <= NlwRenamedSig_OI_fifo_02_rd_addr_val(3);
  fifo_02_rd_addr_val(2) <= NlwRenamedSig_OI_fifo_02_rd_addr_val(2);
  fifo_02_rd_addr_val(1) <= NlwRenamedSig_OI_fifo_02_rd_addr_val(1);
  fifo_02_rd_addr_val(0) <= NlwRenamedSig_OI_fifo_02_rd_addr_val(0);
  fifo_12_wr_addr_val(3) <= NlwRenamedSig_OI_fifo_12_wr_addr_val(3);
  fifo_12_wr_addr_val(2) <= NlwRenamedSig_OI_fifo_12_wr_addr_val(2);
  fifo_12_wr_addr_val(1) <= NlwRenamedSig_OI_fifo_12_wr_addr_val(1);
  fifo_12_wr_addr_val(0) <= NlwRenamedSig_OI_fifo_12_wr_addr_val(0);
  transfer_done_1_val(3) <= NlwRenamedSig_OI_transfer_done_1_val(3);
  transfer_done_1_val(2) <= NlwRenamedSig_OI_transfer_done_1_val(2);
  transfer_done_1_val(1) <= NlwRenamedSig_OI_transfer_done_1_val(1);
  transfer_done_1_val(0) <= NlwRenamedSig_OI_transfer_done_1_val(0);
  fifo_12_rd_addr_val(3) <= NlwRenamedSig_OI_fifo_12_rd_addr_val(3);
  fifo_12_rd_addr_val(2) <= NlwRenamedSig_OI_fifo_12_rd_addr_val(2);
  fifo_12_rd_addr_val(1) <= NlwRenamedSig_OI_fifo_12_rd_addr_val(1);
  fifo_12_rd_addr_val(0) <= NlwRenamedSig_OI_fifo_12_rd_addr_val(0);
  fifo_03_wr_addr_val(3) <= NlwRenamedSig_OI_fifo_03_wr_addr_val(3);
  fifo_03_wr_addr_val(2) <= NlwRenamedSig_OI_fifo_03_wr_addr_val(2);
  fifo_03_wr_addr_val(1) <= NlwRenamedSig_OI_fifo_03_wr_addr_val(1);
  fifo_03_wr_addr_val(0) <= NlwRenamedSig_OI_fifo_03_wr_addr_val(0);
  fifo_00_wr_addr_val(3) <= NlwRenamedSig_OI_fifo_00_wr_addr_val(3);
  fifo_00_wr_addr_val(2) <= NlwRenamedSig_OI_fifo_00_wr_addr_val(2);
  fifo_00_wr_addr_val(1) <= NlwRenamedSig_OI_fifo_00_wr_addr_val(1);
  fifo_00_wr_addr_val(0) <= NlwRenamedSig_OI_fifo_00_wr_addr_val(0);
  fifo_03_rd_addr_val(3) <= NlwRenamedSig_OI_fifo_03_rd_addr_val(3);
  fifo_03_rd_addr_val(2) <= NlwRenamedSig_OI_fifo_03_rd_addr_val(2);
  fifo_03_rd_addr_val(1) <= NlwRenamedSig_OI_fifo_03_rd_addr_val(1);
  fifo_03_rd_addr_val(0) <= NlwRenamedSig_OI_fifo_03_rd_addr_val(0);
  fifo_00_rd_addr_val(3) <= NlwRenamedSig_OI_fifo_00_rd_addr_val(3);
  fifo_00_rd_addr_val(2) <= NlwRenamedSig_OI_fifo_00_rd_addr_val(2);
  fifo_00_rd_addr_val(1) <= NlwRenamedSig_OI_fifo_00_rd_addr_val(1);
  fifo_00_rd_addr_val(0) <= NlwRenamedSig_OI_fifo_00_rd_addr_val(0);
  fifo_13_wr_addr_val(3) <= NlwRenamedSig_OI_fifo_13_wr_addr_val(3);
  fifo_13_wr_addr_val(2) <= NlwRenamedSig_OI_fifo_13_wr_addr_val(2);
  fifo_13_wr_addr_val(1) <= NlwRenamedSig_OI_fifo_13_wr_addr_val(1);
  fifo_13_wr_addr_val(0) <= NlwRenamedSig_OI_fifo_13_wr_addr_val(0);
  fifo_10_wr_addr_val(3) <= NlwRenamedSig_OI_fifo_10_wr_addr_val(3);
  fifo_10_wr_addr_val(2) <= NlwRenamedSig_OI_fifo_10_wr_addr_val(2);
  fifo_10_wr_addr_val(1) <= NlwRenamedSig_OI_fifo_10_wr_addr_val(1);
  fifo_10_wr_addr_val(0) <= NlwRenamedSig_OI_fifo_10_wr_addr_val(0);
  fifo_13_rd_addr_val(3) <= NlwRenamedSig_OI_fifo_13_rd_addr_val(3);
  fifo_13_rd_addr_val(2) <= NlwRenamedSig_OI_fifo_13_rd_addr_val(2);
  fifo_13_rd_addr_val(1) <= NlwRenamedSig_OI_fifo_13_rd_addr_val(1);
  fifo_13_rd_addr_val(0) <= NlwRenamedSig_OI_fifo_13_rd_addr_val(0);
  fifo_10_rd_addr_val(3) <= NlwRenamedSig_OI_fifo_10_rd_addr_val(3);
  fifo_10_rd_addr_val(2) <= NlwRenamedSig_OI_fifo_10_rd_addr_val(2);
  fifo_10_rd_addr_val(1) <= NlwRenamedSig_OI_fifo_10_rd_addr_val(1);
  fifo_10_rd_addr_val(0) <= NlwRenamedSig_OI_fifo_10_rd_addr_val(0);
  dqs_delayed_col0_val(1) <= NlwRenamedSig_OI_dqs_delayed_col0_val(1);
  dqs_delayed_col0_val(0) <= NlwRenamedSig_OI_dqs_delayed_col0_val(0);
  fifo_01_wr_addr_val(3) <= NlwRenamedSig_OI_fifo_01_wr_addr_val(3);
  fifo_01_wr_addr_val(2) <= NlwRenamedSig_OI_fifo_01_wr_addr_val(2);
  fifo_01_wr_addr_val(1) <= NlwRenamedSig_OI_fifo_01_wr_addr_val(1);
  fifo_01_wr_addr_val(0) <= NlwRenamedSig_OI_fifo_01_wr_addr_val(0);
  rd_data_valid : FDR
    port map (
      D => Q_n0008,
      R => reset90_r,
      C => clk90,
      Q => u_data_val
    );
  fifo_03_not_empty_r : FDR
    port map (
      D => fifo_03_not_empty,
      R => reset90_r,
      C => clk90,
      Q => fifo_03_not_empty_r_99
    );
  rd_data_valid_reg : FDE
    port map (
      D => read_valid_data,
      CE => reset90_r_INV,
      C => clk90,
      Q => rd_data_valid_reg_98
    );
  rd_data_valid_1 : FDR
    port map (
      D => read_valid_data_1,
      R => reset90_r,
      C => clk90,
      Q => NlwRenamedSig_OI_read_valid_data_1_val
    );
  rd_data_valid_2 : FDR
    port map (
      D => read_valid_data_2,
      R => reset90_r,
      C => clk90,
      Q => NlwRenamedSig_OI_read_valid_data_2_val
    );
  fifo_01_not_empty_r : FDR
    port map (
      D => fifo_01_not_empty,
      R => reset90_r,
      C => clk90,
      Q => fifo_01_not_empty_r_96
    );
  fifo_03_not_empty_r1 : FDR
    port map (
      D => fifo_03_not_empty_r_99,
      R => reset90_r,
      C => clk90,
      Q => fifo_03_not_empty_r1_100
    );
  fifo_01_not_empty_r1 : FDR
    port map (
      D => fifo_01_not_empty_r_96,
      R => reset90_r,
      C => clk90,
      Q => fifo_01_not_empty_r1_97
    );
  fifo_11_rd_addr_0 : FDRE
    port map (
      D => fifo_11_rd_addr_n0000(0),
      R => reset90_r,
      CE => Q_n0046,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_11_rd_addr_val(0)
    );
  fifo_11_rd_addr_1 : FDRE
    port map (
      D => fifo_11_rd_addr_n0000(1),
      R => reset90_r,
      CE => Q_n0046,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_11_rd_addr_val(1)
    );
  fifo_11_rd_addr_2 : FDRE
    port map (
      D => fifo_11_rd_addr_n0000(2),
      R => reset90_r,
      CE => Q_n0046,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_11_rd_addr_val(2)
    );
  fifo_11_rd_addr_3 : FDRE
    port map (
      D => fifo_11_rd_addr_n0000(3),
      R => reset90_r,
      CE => Q_n0046,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_11_rd_addr_val(3)
    );
  fifo_12_rd_addr_0 : FDRE
    port map (
      D => fifo_12_rd_addr_n0000(0),
      R => reset90_r,
      CE => Q_n0047,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_12_rd_addr_val(0)
    );
  fifo_12_rd_addr_1 : FDRE
    port map (
      D => fifo_12_rd_addr_n0000(1),
      R => reset90_r,
      CE => Q_n0047,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_12_rd_addr_val(1)
    );
  fifo_12_rd_addr_2 : FDRE
    port map (
      D => fifo_12_rd_addr_n0000(2),
      R => reset90_r,
      CE => Q_n0047,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_12_rd_addr_val(2)
    );
  fifo_12_rd_addr_3 : FDRE
    port map (
      D => fifo_12_rd_addr_n0000(3),
      R => reset90_r,
      CE => Q_n0047,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_12_rd_addr_val(3)
    );
  fifo_13_rd_addr_0 : FDRE
    port map (
      D => fifo_13_rd_addr_n0000(0),
      R => reset90_r,
      CE => Q_n0047,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_13_rd_addr_val(0)
    );
  fifo_13_rd_addr_1 : FDRE
    port map (
      D => fifo_13_rd_addr_n0000(1),
      R => reset90_r,
      CE => Q_n0047,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_13_rd_addr_val(1)
    );
  fifo_13_rd_addr_2 : FDRE
    port map (
      D => fifo_13_rd_addr_n0000(2),
      R => reset90_r,
      CE => Q_n0047,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_13_rd_addr_val(2)
    );
  fifo_13_rd_addr_3 : FDRE
    port map (
      D => fifo_13_rd_addr_n0000(3),
      R => reset90_r,
      CE => Q_n0047,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_13_rd_addr_val(3)
    );
  fifo_00_wr_addr_0 : FDRE
    port map (
      D => fifo_00_wr_addr_n0000(0),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(0),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_00_wr_addr_val(0)
    );
  fifo_00_wr_addr_1 : FDRE
    port map (
      D => fifo_00_wr_addr_n0000(1),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(0),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_00_wr_addr_val(1)
    );
  fifo_00_wr_addr_2 : FDRE
    port map (
      D => fifo_00_wr_addr_n0000(2),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(0),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_00_wr_addr_val(2)
    );
  fifo_00_wr_addr_3 : FDRE
    port map (
      D => fifo_00_wr_addr_n0000(3),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(0),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_00_wr_addr_val(3)
    );
  fifo_01_wr_addr_0 : FDRE
    port map (
      D => fifo_01_wr_addr_n0000(0),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(1),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_01_wr_addr_val(0)
    );
  fifo_01_wr_addr_1 : FDRE
    port map (
      D => fifo_01_wr_addr_n0000(1),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(1),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_01_wr_addr_val(1)
    );
  fifo_01_wr_addr_2 : FDRE
    port map (
      D => fifo_01_wr_addr_n0000(2),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(1),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_01_wr_addr_val(2)
    );
  fifo_01_wr_addr_3 : FDRE
    port map (
      D => fifo_01_wr_addr_n0000(3),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(1),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_01_wr_addr_val(3)
    );
  fifo_02_wr_addr_0 : FDRE
    port map (
      D => fifo_02_wr_addr_n0000(0),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(2),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_02_wr_addr_val(0)
    );
  fifo_02_wr_addr_1 : FDRE
    port map (
      D => fifo_02_wr_addr_n0000(1),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(2),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_02_wr_addr_val(1)
    );
  fifo_02_wr_addr_2 : FDRE
    port map (
      D => fifo_02_wr_addr_n0000(2),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(2),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_02_wr_addr_val(2)
    );
  fifo_02_wr_addr_3 : FDRE
    port map (
      D => fifo_02_wr_addr_n0000(3),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(2),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_02_wr_addr_val(3)
    );
  fifo_03_wr_addr_0 : FDRE
    port map (
      D => fifo_03_wr_addr_n0000(0),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(3),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_03_wr_addr_val(0)
    );
  fifo_03_wr_addr_1 : FDRE
    port map (
      D => fifo_03_wr_addr_n0000(1),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(3),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_03_wr_addr_val(1)
    );
  fifo_03_wr_addr_2 : FDRE
    port map (
      D => fifo_03_wr_addr_n0000(2),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(3),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_03_wr_addr_val(2)
    );
  fifo_03_wr_addr_3 : FDRE
    port map (
      D => fifo_03_wr_addr_n0000(3),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(3),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_03_wr_addr_val(3)
    );
  fifo_10_wr_addr_0 : FDRE
    port map (
      D => fifo_10_wr_addr_n0000(0),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(0),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_10_wr_addr_val(0)
    );
  fifo_10_wr_addr_1 : FDRE
    port map (
      D => fifo_10_wr_addr_n0000(1),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(0),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_10_wr_addr_val(1)
    );
  fifo_10_wr_addr_2 : FDRE
    port map (
      D => fifo_10_wr_addr_n0000(2),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(0),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_10_wr_addr_val(2)
    );
  fifo_10_wr_addr_3 : FDRE
    port map (
      D => fifo_10_wr_addr_n0000(3),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(0),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_10_wr_addr_val(3)
    );
  fifo_11_wr_addr_0 : FDRE
    port map (
      D => fifo_11_wr_addr_n0000(0),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(1),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_11_wr_addr_val(0)
    );
  fifo_11_wr_addr_1 : FDRE
    port map (
      D => fifo_11_wr_addr_n0000(1),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(1),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_11_wr_addr_val(1)
    );
  fifo_11_wr_addr_2 : FDRE
    port map (
      D => fifo_11_wr_addr_n0000(2),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(1),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_11_wr_addr_val(2)
    );
  fifo_11_wr_addr_3 : FDRE
    port map (
      D => fifo_11_wr_addr_n0000(3),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(1),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_11_wr_addr_val(3)
    );
  fifo_12_wr_addr_0 : FDRE
    port map (
      D => fifo_12_wr_addr_n0000(0),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(2),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_12_wr_addr_val(0)
    );
  fifo_12_wr_addr_1 : FDRE
    port map (
      D => fifo_12_wr_addr_n0000(1),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(2),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_12_wr_addr_val(1)
    );
  fifo_12_wr_addr_2 : FDRE
    port map (
      D => fifo_12_wr_addr_n0000(2),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(2),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_12_wr_addr_val(2)
    );
  fifo_12_wr_addr_3 : FDRE
    port map (
      D => fifo_12_wr_addr_n0000(3),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(2),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_12_wr_addr_val(3)
    );
  fifo_13_wr_addr_0 : FDRE
    port map (
      D => fifo_13_wr_addr_n0000(0),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(3),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_13_wr_addr_val(0)
    );
  fifo_13_wr_addr_1 : FDRE
    port map (
      D => fifo_13_wr_addr_n0000(1),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(3),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_13_wr_addr_val(1)
    );
  fifo_13_wr_addr_2 : FDRE
    port map (
      D => fifo_13_wr_addr_n0000(2),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(3),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_13_wr_addr_val(2)
    );
  fifo_13_wr_addr_3 : FDRE
    port map (
      D => fifo_13_wr_addr_n0000(3),
      R => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(3),
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_13_wr_addr_val(3)
    );
  fifo_00_rd_addr_0 : FDRE
    port map (
      D => fifo_00_rd_addr_n0000(0),
      R => reset90_r,
      CE => Q_n0046,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_00_rd_addr_val(0)
    );
  fifo_00_rd_addr_1 : FDRE
    port map (
      D => fifo_00_rd_addr_n0000(1),
      R => reset90_r,
      CE => Q_n0046,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_00_rd_addr_val(1)
    );
  fifo_00_rd_addr_2 : FDRE
    port map (
      D => fifo_00_rd_addr_n0000(2),
      R => reset90_r,
      CE => Q_n0046,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_00_rd_addr_val(2)
    );
  fifo_00_rd_addr_3 : FDRE
    port map (
      D => fifo_00_rd_addr_n0000(3),
      R => reset90_r,
      CE => Q_n0046,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_00_rd_addr_val(3)
    );
  fifo_01_rd_addr_0 : FDRE
    port map (
      D => fifo_01_rd_addr_n0000(0),
      R => reset90_r,
      CE => Q_n0046,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_01_rd_addr_val(0)
    );
  fifo_01_rd_addr_1 : FDRE
    port map (
      D => fifo_01_rd_addr_n0000(1),
      R => reset90_r,
      CE => Q_n0046,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_01_rd_addr_val(1)
    );
  fifo_01_rd_addr_2 : FDRE
    port map (
      D => fifo_01_rd_addr_n0000(2),
      R => reset90_r,
      CE => Q_n0046,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_01_rd_addr_val(2)
    );
  fifo_01_rd_addr_3 : FDRE
    port map (
      D => fifo_01_rd_addr_n0000(3),
      R => reset90_r,
      CE => Q_n0046,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_01_rd_addr_val(3)
    );
  fifo_02_rd_addr_0 : FDRE
    port map (
      D => fifo_02_rd_addr_n0000(0),
      R => reset90_r,
      CE => Q_n0047,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_02_rd_addr_val(0)
    );
  fifo_02_rd_addr_1 : FDRE
    port map (
      D => fifo_02_rd_addr_n0000(1),
      R => reset90_r,
      CE => Q_n0047,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_02_rd_addr_val(1)
    );
  fifo_02_rd_addr_2 : FDRE
    port map (
      D => fifo_02_rd_addr_n0000(2),
      R => reset90_r,
      CE => Q_n0047,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_02_rd_addr_val(2)
    );
  fifo_02_rd_addr_3 : FDRE
    port map (
      D => fifo_02_rd_addr_n0000(3),
      R => reset90_r,
      CE => Q_n0047,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_02_rd_addr_val(3)
    );
  fifo_03_rd_addr_0 : FDRE
    port map (
      D => fifo_03_rd_addr_n0000(0),
      R => reset90_r,
      CE => Q_n0047,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_03_rd_addr_val(0)
    );
  fifo_03_rd_addr_1 : FDRE
    port map (
      D => fifo_03_rd_addr_n0000(1),
      R => reset90_r,
      CE => Q_n0047,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_03_rd_addr_val(1)
    );
  fifo_03_rd_addr_2 : FDRE
    port map (
      D => fifo_03_rd_addr_n0000(2),
      R => reset90_r,
      CE => Q_n0047,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_03_rd_addr_val(2)
    );
  fifo_03_rd_addr_3 : FDRE
    port map (
      D => fifo_03_rd_addr_n0000(3),
      R => reset90_r,
      CE => Q_n0047,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_03_rd_addr_val(3)
    );
  fifo_10_rd_addr_0 : FDRE
    port map (
      D => fifo_10_rd_addr_n0000(0),
      R => reset90_r,
      CE => Q_n0046,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_10_rd_addr_val(0)
    );
  fifo_10_rd_addr_1 : FDRE
    port map (
      D => fifo_10_rd_addr_n0000(1),
      R => reset90_r,
      CE => Q_n0046,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_10_rd_addr_val(1)
    );
  fifo_10_rd_addr_2 : FDRE
    port map (
      D => fifo_10_rd_addr_n0000(2),
      R => reset90_r,
      CE => Q_n0046,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_10_rd_addr_val(2)
    );
  fifo_10_rd_addr_3 : FDRE
    port map (
      D => fifo_10_rd_addr_n0000(3),
      R => reset90_r,
      CE => Q_n0046,
      C => clk90,
      Q => NlwRenamedSig_OI_fifo_10_rd_addr_val(3)
    );
  rst_dqs_div_int1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => rst_dqs_div,
      O => rst_dqs_div_int
    );
  Q_n00461 : LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      I0 => next_state,
      I1 => NlwRenamedSig_OI_read_valid_data_1_val,
      O => Q_n0046
    );
  Q_n00471 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => next_state,
      I1 => NlwRenamedSig_OI_read_valid_data_2_val,
      O => Q_n0047
    );
  reset90_r_INV1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => reset90_r,
      O => reset90_r_INV
    );
  fifo_11_rd_addr_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_11_rd_addr_val(0),
      O => fifo_11_rd_addr_n0000(0)
    );
  fifo_11_rd_addr_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_11_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_11_rd_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_11_rd_addr_val(1),
      I3 => NlwRenamedSig_OI_fifo_11_rd_addr_val(2),
      O => fifo_11_rd_addr_n0000(3)
    );
  fifo_11_rd_addr_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_11_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_11_rd_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_11_rd_addr_val(2),
      O => fifo_11_rd_addr_n0000(2)
    );
  fifo_11_rd_addr_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_11_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_11_rd_addr_val(1),
      O => fifo_11_rd_addr_n0000(1)
    );
  fifo_12_rd_addr_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_12_rd_addr_val(0),
      O => fifo_12_rd_addr_n0000(0)
    );
  fifo_12_rd_addr_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_12_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_12_rd_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_12_rd_addr_val(1),
      I3 => NlwRenamedSig_OI_fifo_12_rd_addr_val(2),
      O => fifo_12_rd_addr_n0000(3)
    );
  fifo_12_rd_addr_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_12_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_12_rd_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_12_rd_addr_val(2),
      O => fifo_12_rd_addr_n0000(2)
    );
  fifo_12_rd_addr_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_12_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_12_rd_addr_val(1),
      O => fifo_12_rd_addr_n0000(1)
    );
  fifo_13_rd_addr_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_13_rd_addr_val(0),
      O => fifo_13_rd_addr_n0000(0)
    );
  fifo_13_rd_addr_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_13_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_13_rd_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_13_rd_addr_val(1),
      I3 => NlwRenamedSig_OI_fifo_13_rd_addr_val(2),
      O => fifo_13_rd_addr_n0000(3)
    );
  fifo_13_rd_addr_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_13_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_13_rd_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_13_rd_addr_val(2),
      O => fifo_13_rd_addr_n0000(2)
    );
  fifo_13_rd_addr_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_13_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_13_rd_addr_val(1),
      O => fifo_13_rd_addr_n0000(1)
    );
  fifo_00_wr_addr_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_00_wr_addr_val(0),
      O => fifo_00_wr_addr_n0000(0)
    );
  fifo_00_wr_addr_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_00_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_00_wr_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_00_wr_addr_val(1),
      I3 => NlwRenamedSig_OI_fifo_00_wr_addr_val(2),
      O => fifo_00_wr_addr_n0000(3)
    );
  fifo_00_wr_addr_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_00_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_00_wr_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_00_wr_addr_val(2),
      O => fifo_00_wr_addr_n0000(2)
    );
  fifo_00_wr_addr_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_00_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_00_wr_addr_val(1),
      O => fifo_00_wr_addr_n0000(1)
    );
  fifo_01_wr_addr_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_01_wr_addr_val(0),
      O => fifo_01_wr_addr_n0000(0)
    );
  fifo_01_wr_addr_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_01_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_01_wr_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_01_wr_addr_val(1),
      I3 => NlwRenamedSig_OI_fifo_01_wr_addr_val(2),
      O => fifo_01_wr_addr_n0000(3)
    );
  fifo_01_wr_addr_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_01_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_01_wr_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_01_wr_addr_val(2),
      O => fifo_01_wr_addr_n0000(2)
    );
  fifo_01_wr_addr_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_01_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_01_wr_addr_val(1),
      O => fifo_01_wr_addr_n0000(1)
    );
  fifo_02_wr_addr_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_02_wr_addr_val(0),
      O => fifo_02_wr_addr_n0000(0)
    );
  fifo_02_wr_addr_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_02_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_02_wr_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_02_wr_addr_val(1),
      I3 => NlwRenamedSig_OI_fifo_02_wr_addr_val(2),
      O => fifo_02_wr_addr_n0000(3)
    );
  fifo_02_wr_addr_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_02_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_02_wr_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_02_wr_addr_val(2),
      O => fifo_02_wr_addr_n0000(2)
    );
  fifo_02_wr_addr_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_02_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_02_wr_addr_val(1),
      O => fifo_02_wr_addr_n0000(1)
    );
  fifo_03_wr_addr_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_03_wr_addr_val(0),
      O => fifo_03_wr_addr_n0000(0)
    );
  fifo_03_wr_addr_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_03_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_03_wr_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_03_wr_addr_val(1),
      I3 => NlwRenamedSig_OI_fifo_03_wr_addr_val(2),
      O => fifo_03_wr_addr_n0000(3)
    );
  fifo_03_wr_addr_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_03_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_03_wr_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_03_wr_addr_val(2),
      O => fifo_03_wr_addr_n0000(2)
    );
  fifo_03_wr_addr_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_03_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_03_wr_addr_val(1),
      O => fifo_03_wr_addr_n0000(1)
    );
  fifo_10_wr_addr_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_10_wr_addr_val(0),
      O => fifo_10_wr_addr_n0000(0)
    );
  fifo_10_wr_addr_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_10_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_10_wr_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_10_wr_addr_val(1),
      I3 => NlwRenamedSig_OI_fifo_10_wr_addr_val(2),
      O => fifo_10_wr_addr_n0000(3)
    );
  fifo_10_wr_addr_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_10_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_10_wr_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_10_wr_addr_val(2),
      O => fifo_10_wr_addr_n0000(2)
    );
  fifo_10_wr_addr_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_10_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_10_wr_addr_val(1),
      O => fifo_10_wr_addr_n0000(1)
    );
  fifo_11_wr_addr_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_11_wr_addr_val(0),
      O => fifo_11_wr_addr_n0000(0)
    );
  fifo_11_wr_addr_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_11_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_11_wr_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_11_wr_addr_val(1),
      I3 => NlwRenamedSig_OI_fifo_11_wr_addr_val(2),
      O => fifo_11_wr_addr_n0000(3)
    );
  fifo_11_wr_addr_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_11_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_11_wr_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_11_wr_addr_val(2),
      O => fifo_11_wr_addr_n0000(2)
    );
  fifo_11_wr_addr_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_11_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_11_wr_addr_val(1),
      O => fifo_11_wr_addr_n0000(1)
    );
  fifo_12_wr_addr_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_12_wr_addr_val(0),
      O => fifo_12_wr_addr_n0000(0)
    );
  fifo_12_wr_addr_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_12_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_12_wr_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_12_wr_addr_val(1),
      I3 => NlwRenamedSig_OI_fifo_12_wr_addr_val(2),
      O => fifo_12_wr_addr_n0000(3)
    );
  fifo_12_wr_addr_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_12_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_12_wr_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_12_wr_addr_val(2),
      O => fifo_12_wr_addr_n0000(2)
    );
  fifo_12_wr_addr_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_12_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_12_wr_addr_val(1),
      O => fifo_12_wr_addr_n0000(1)
    );
  fifo_13_wr_addr_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_13_wr_addr_val(0),
      O => fifo_13_wr_addr_n0000(0)
    );
  fifo_13_wr_addr_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_13_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_13_wr_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_13_wr_addr_val(1),
      I3 => NlwRenamedSig_OI_fifo_13_wr_addr_val(2),
      O => fifo_13_wr_addr_n0000(3)
    );
  fifo_13_wr_addr_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_13_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_13_wr_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_13_wr_addr_val(2),
      O => fifo_13_wr_addr_n0000(2)
    );
  fifo_13_wr_addr_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_13_wr_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_13_wr_addr_val(1),
      O => fifo_13_wr_addr_n0000(1)
    );
  fifo_00_rd_addr_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_00_rd_addr_val(0),
      O => fifo_00_rd_addr_n0000(0)
    );
  fifo_00_rd_addr_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_00_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_00_rd_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_00_rd_addr_val(1),
      I3 => NlwRenamedSig_OI_fifo_00_rd_addr_val(2),
      O => fifo_00_rd_addr_n0000(3)
    );
  fifo_00_rd_addr_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_00_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_00_rd_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_00_rd_addr_val(2),
      O => fifo_00_rd_addr_n0000(2)
    );
  fifo_00_rd_addr_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_00_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_00_rd_addr_val(1),
      O => fifo_00_rd_addr_n0000(1)
    );
  fifo_01_rd_addr_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_01_rd_addr_val(0),
      O => fifo_01_rd_addr_n0000(0)
    );
  fifo_01_rd_addr_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_01_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_01_rd_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_01_rd_addr_val(1),
      I3 => NlwRenamedSig_OI_fifo_01_rd_addr_val(2),
      O => fifo_01_rd_addr_n0000(3)
    );
  fifo_01_rd_addr_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_01_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_01_rd_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_01_rd_addr_val(2),
      O => fifo_01_rd_addr_n0000(2)
    );
  fifo_01_rd_addr_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_01_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_01_rd_addr_val(1),
      O => fifo_01_rd_addr_n0000(1)
    );
  fifo_02_rd_addr_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_02_rd_addr_val(0),
      O => fifo_02_rd_addr_n0000(0)
    );
  fifo_02_rd_addr_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_02_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_02_rd_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_02_rd_addr_val(1),
      I3 => NlwRenamedSig_OI_fifo_02_rd_addr_val(2),
      O => fifo_02_rd_addr_n0000(3)
    );
  fifo_02_rd_addr_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_02_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_02_rd_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_02_rd_addr_val(2),
      O => fifo_02_rd_addr_n0000(2)
    );
  fifo_02_rd_addr_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_02_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_02_rd_addr_val(1),
      O => fifo_02_rd_addr_n0000(1)
    );
  fifo_03_rd_addr_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_03_rd_addr_val(0),
      O => fifo_03_rd_addr_n0000(0)
    );
  fifo_03_rd_addr_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_03_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_03_rd_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_03_rd_addr_val(1),
      I3 => NlwRenamedSig_OI_fifo_03_rd_addr_val(2),
      O => fifo_03_rd_addr_n0000(3)
    );
  fifo_03_rd_addr_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_03_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_03_rd_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_03_rd_addr_val(2),
      O => fifo_03_rd_addr_n0000(2)
    );
  fifo_03_rd_addr_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_03_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_03_rd_addr_val(1),
      O => fifo_03_rd_addr_n0000(1)
    );
  fifo_10_rd_addr_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_10_rd_addr_val(0),
      O => fifo_10_rd_addr_n0000(0)
    );
  fifo_10_rd_addr_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_10_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_10_rd_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_10_rd_addr_val(1),
      I3 => NlwRenamedSig_OI_fifo_10_rd_addr_val(2),
      O => fifo_10_rd_addr_n0000(3)
    );
  fifo_10_rd_addr_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_10_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_10_rd_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_10_rd_addr_val(2),
      O => fifo_10_rd_addr_n0000(2)
    );
  fifo_10_rd_addr_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_10_rd_addr_val(0),
      I1 => NlwRenamedSig_OI_fifo_10_rd_addr_val(1),
      O => fifo_10_rd_addr_n0000(1)
    );
  ddr2_dqs_div1 : ddr2_dqs_div
    port map (
      reset => reset_r,
      rst_dqs_div_delayed => rst_dqs_div_delayed,
      dqs => NlwRenamedSig_OI_dqs_delayed_col0_val(1),
      dqs1 => NlwRenamedSig_OI_dqs_delayed_col1_val(1),
      dqs_divn => dqs_div_col0_val(1),
      dqs_divp => NlwRenamedSig_OI_dqs_div_col1_val(1)
    );
  ddr2_dqs_div0 : ddr2_dqs_div_INST_1
    port map (
      reset => reset_r,
      rst_dqs_div_delayed => rst_dqs_div_delayed,
      dqs => NlwRenamedSig_OI_dqs_delayed_col0_val(0),
      dqs1 => NlwRenamedSig_OI_dqs_delayed_col1_val(0),
      dqs_divn => dqs_div_col0_val(0),
      dqs_divp => NlwRenamedSig_OI_dqs_div_col1_val(0)
    );
  ddr2_transfer_done1 : ddr2_transfer_done
    port map (
      reset270 => reset270_r,
      dqs_div => NlwRenamedSig_OI_dqs_div_col1_val(1),
      reset => reset_r,
      clk0 => clk,
      clk180 => clk180,
      clk90 => clk90,
      clk270 => clk270,
      reset90 => reset90_r,
      reset180 => reset180_r,
      transfer_done0 => NlwRenamedSig_OI_transfer_done_1_val(0),
      transfer_done1 => NlwRenamedSig_OI_transfer_done_1_val(1),
      transfer_done2 => NlwRenamedSig_OI_transfer_done_1_val(2),
      transfer_done3 => NlwRenamedSig_OI_transfer_done_1_val(3)
    );
  ddr2_transfer_done0 : ddr2_transfer_done_INST_1
    port map (
      reset270 => reset270_r,
      dqs_div => NlwRenamedSig_OI_dqs_div_col1_val(0),
      reset => reset_r,
      clk0 => clk,
      clk180 => clk180,
      clk90 => clk90,
      clk270 => clk270,
      reset90 => reset90_r,
      reset180 => reset180_r,
      transfer_done0 => NlwRenamedSig_OI_transfer_done_0_val(0),
      transfer_done1 => NlwRenamedSig_OI_transfer_done_0_val(1),
      transfer_done2 => NlwRenamedSig_OI_transfer_done_0_val(2),
      transfer_done3 => NlwRenamedSig_OI_transfer_done_0_val(3)
    );
  Q_n00081 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => rd_data_valid_reg_98,
      I1 => read_valid_data,
      O => Q_n0008
    );
  read_valid_data1 : LUT4
    generic map(
      INIT => X"22F2"
    )
    port map (
      I0 => fifo_01_not_empty_r1_97,
      I1 => Q_n0018,
      I2 => fifo_03_not_empty_r1_100,
      I3 => Q_n0019,
      O => read_valid_data
    );
  Q_n0018426 : LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_00_rd_addr_val(1),
      I1 => NlwRenamedSig_OI_fifo_01_wr_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_00_rd_addr_val(0),
      I3 => NlwRenamedSig_OI_fifo_01_wr_addr_val(0),
      O => N28
    );
  Q_n0018453 : LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_00_rd_addr_val(3),
      I1 => NlwRenamedSig_OI_fifo_01_wr_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_00_rd_addr_val(2),
      I3 => NlwRenamedSig_OI_fifo_01_wr_addr_val(2),
      O => N51
    );
  Q_n0018454 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => N28,
      I1 => N51,
      O => Q_n0018
    );
  Q_n0019426 : LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_02_rd_addr_val(1),
      I1 => NlwRenamedSig_OI_fifo_03_wr_addr_val(1),
      I2 => NlwRenamedSig_OI_fifo_02_rd_addr_val(0),
      I3 => NlwRenamedSig_OI_fifo_03_wr_addr_val(0),
      O => N77
    );
  Q_n0019453 : LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => NlwRenamedSig_OI_fifo_02_rd_addr_val(3),
      I1 => NlwRenamedSig_OI_fifo_03_wr_addr_val(3),
      I2 => NlwRenamedSig_OI_fifo_02_rd_addr_val(2),
      I3 => NlwRenamedSig_OI_fifo_03_wr_addr_val(2),
      O => N100
    );
  Q_n0019454 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => N77,
      I1 => N100,
      O => Q_n0019
    );
  fifo_01_not_empty1 : LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      I0 => N28,
      I1 => N51,
      O => fifo_01_not_empty
    );
  fifo_03_not_empty1 : LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      I0 => N77,
      I1 => N100,
      O => fifo_03_not_empty
    );
  Q_n00201 : LUT3
    generic map(
      INIT => X"4C"
    )
    port map (
      I0 => N28,
      I1 => fifo_01_not_empty_r1_97,
      I2 => N51,
      O => read_valid_data_1
    );
  Q_n00211 : LUT3
    generic map(
      INIT => X"4C"
    )
    port map (
      I0 => N77,
      I1 => fifo_03_not_empty_r1_100,
      I2 => N100,
      O => read_valid_data_2
    );
  dqs_delay1_col1_six : LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      I0 => dqs_int_delay_in1,
      I1 => delay_sel(1),
      I2 => dqs_delay1_col1_delay2,
      I3 => dqs_delay1_col1_high,
      O => dqs_delay1_col1_delay3
    );
  dqs_delay1_col1_five : LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      I0 => dqs_delay1_col1_high,
      I1 => delay_sel(3),
      I2 => dqs_delay1_col1_delay4,
      I3 => dqs_int_delay_in1,
      O => dqs_delay1_col1_delay5
    );
  dqs_delay1_col1_four : LUT4
    generic map(
      INIT => X"FF00"
    )
    port map (
      I0 => dqs_delay1_col1_high,
      I1 => dqs_delay1_col1_high,
      I2 => dqs_delay1_col1_high,
      I3 => dqs_int_delay_in1,
      O => dqs_delay1_col1_delay1
    );
  dqs_delay1_col1_three : LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      I0 => dqs_int_delay_in1,
      I1 => delay_sel(0),
      I2 => dqs_delay1_col1_delay1,
      I3 => dqs_delay1_col1_high,
      O => dqs_delay1_col1_delay2
    );
  dqs_delay1_col1_two : LUT4
    generic map(
      INIT => X"EE22"
    )
    port map (
      I0 => dqs_int_delay_in1,
      I1 => delay_sel(2),
      I2 => dqs_delay1_col1_high,
      I3 => dqs_delay1_col1_delay3,
      O => dqs_delay1_col1_delay4
    );
  dqs_delay1_col1_one : LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      I0 => dqs_delay1_col1_high,
      I1 => delay_sel(4),
      I2 => dqs_delay1_col1_delay5,
      I3 => dqs_int_delay_in1,
      O => NlwRenamedSig_OI_dqs_delayed_col1_val(1)
    );
  dqs_delay1_col1_XST_VCC : VCC
    port map (
      P => dqs_delay1_col1_high
    );
  dqs_delay1_col0_six : LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      I0 => dqs_int_delay_in1,
      I1 => delay_sel(1),
      I2 => dqs_delay1_col0_delay2,
      I3 => dqs_delay1_col0_high,
      O => dqs_delay1_col0_delay3
    );
  dqs_delay1_col0_five : LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      I0 => dqs_delay1_col0_high,
      I1 => delay_sel(3),
      I2 => dqs_delay1_col0_delay4,
      I3 => dqs_int_delay_in1,
      O => dqs_delay1_col0_delay5
    );
  dqs_delay1_col0_four : LUT4
    generic map(
      INIT => X"FF00"
    )
    port map (
      I0 => dqs_delay1_col0_high,
      I1 => dqs_delay1_col0_high,
      I2 => dqs_delay1_col0_high,
      I3 => dqs_int_delay_in1,
      O => dqs_delay1_col0_delay1
    );
  dqs_delay1_col0_three : LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      I0 => dqs_int_delay_in1,
      I1 => delay_sel(0),
      I2 => dqs_delay1_col0_delay1,
      I3 => dqs_delay1_col0_high,
      O => dqs_delay1_col0_delay2
    );
  dqs_delay1_col0_two : LUT4
    generic map(
      INIT => X"EE22"
    )
    port map (
      I0 => dqs_int_delay_in1,
      I1 => delay_sel(2),
      I2 => dqs_delay1_col0_high,
      I3 => dqs_delay1_col0_delay3,
      O => dqs_delay1_col0_delay4
    );
  dqs_delay1_col0_one : LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      I0 => dqs_delay1_col0_high,
      I1 => delay_sel(4),
      I2 => dqs_delay1_col0_delay5,
      I3 => dqs_int_delay_in1,
      O => NlwRenamedSig_OI_dqs_delayed_col0_val(1)
    );
  dqs_delay1_col0_XST_VCC : VCC
    port map (
      P => dqs_delay1_col0_high
    );
  dqs_delay0_col1_six : LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      I0 => dqs_int_delay_in0,
      I1 => delay_sel(1),
      I2 => dqs_delay0_col1_delay2,
      I3 => dqs_delay0_col1_high,
      O => dqs_delay0_col1_delay3
    );
  dqs_delay0_col1_five : LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      I0 => dqs_delay0_col1_high,
      I1 => delay_sel(3),
      I2 => dqs_delay0_col1_delay4,
      I3 => dqs_int_delay_in0,
      O => dqs_delay0_col1_delay5
    );
  dqs_delay0_col1_four : LUT4
    generic map(
      INIT => X"FF00"
    )
    port map (
      I0 => dqs_delay0_col1_high,
      I1 => dqs_delay0_col1_high,
      I2 => dqs_delay0_col1_high,
      I3 => dqs_int_delay_in0,
      O => dqs_delay0_col1_delay1
    );
  dqs_delay0_col1_three : LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      I0 => dqs_int_delay_in0,
      I1 => delay_sel(0),
      I2 => dqs_delay0_col1_delay1,
      I3 => dqs_delay0_col1_high,
      O => dqs_delay0_col1_delay2
    );
  dqs_delay0_col1_two : LUT4
    generic map(
      INIT => X"EE22"
    )
    port map (
      I0 => dqs_int_delay_in0,
      I1 => delay_sel(2),
      I2 => dqs_delay0_col1_high,
      I3 => dqs_delay0_col1_delay3,
      O => dqs_delay0_col1_delay4
    );
  dqs_delay0_col1_one : LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      I0 => dqs_delay0_col1_high,
      I1 => delay_sel(4),
      I2 => dqs_delay0_col1_delay5,
      I3 => dqs_int_delay_in0,
      O => NlwRenamedSig_OI_dqs_delayed_col1_val(0)
    );
  dqs_delay0_col1_XST_VCC : VCC
    port map (
      P => dqs_delay0_col1_high
    );
  dqs_delay0_col0_six : LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      I0 => dqs_int_delay_in0,
      I1 => delay_sel(1),
      I2 => dqs_delay0_col0_delay2,
      I3 => dqs_delay0_col0_high,
      O => dqs_delay0_col0_delay3
    );
  dqs_delay0_col0_five : LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      I0 => dqs_delay0_col0_high,
      I1 => delay_sel(3),
      I2 => dqs_delay0_col0_delay4,
      I3 => dqs_int_delay_in0,
      O => dqs_delay0_col0_delay5
    );
  dqs_delay0_col0_four : LUT4
    generic map(
      INIT => X"FF00"
    )
    port map (
      I0 => dqs_delay0_col0_high,
      I1 => dqs_delay0_col0_high,
      I2 => dqs_delay0_col0_high,
      I3 => dqs_int_delay_in0,
      O => dqs_delay0_col0_delay1
    );
  dqs_delay0_col0_three : LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      I0 => dqs_int_delay_in0,
      I1 => delay_sel(0),
      I2 => dqs_delay0_col0_delay1,
      I3 => dqs_delay0_col0_high,
      O => dqs_delay0_col0_delay2
    );
  dqs_delay0_col0_two : LUT4
    generic map(
      INIT => X"EE22"
    )
    port map (
      I0 => dqs_int_delay_in0,
      I1 => delay_sel(2),
      I2 => dqs_delay0_col0_high,
      I3 => dqs_delay0_col0_delay3,
      O => dqs_delay0_col0_delay4
    );
  dqs_delay0_col0_one : LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      I0 => dqs_delay0_col0_high,
      I1 => delay_sel(4),
      I2 => dqs_delay0_col0_delay5,
      I3 => dqs_int_delay_in0,
      O => NlwRenamedSig_OI_dqs_delayed_col0_val(0)
    );
  dqs_delay0_col0_XST_VCC : VCC
    port map (
      P => dqs_delay0_col0_high
    );
  rst_dqs_div_delay0_six : LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      I0 => rst_dqs_div_int,
      I1 => delay_sel(1),
      I2 => rst_dqs_div_delay0_delay2,
      I3 => rst_dqs_div_delay0_high,
      O => rst_dqs_div_delay0_delay3
    );
  rst_dqs_div_delay0_five : LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      I0 => rst_dqs_div_delay0_high,
      I1 => delay_sel(3),
      I2 => rst_dqs_div_delay0_delay4,
      I3 => rst_dqs_div_int,
      O => rst_dqs_div_delay0_delay5
    );
  rst_dqs_div_delay0_four : LUT4
    generic map(
      INIT => X"FF00"
    )
    port map (
      I0 => rst_dqs_div_delay0_high,
      I1 => rst_dqs_div_delay0_high,
      I2 => rst_dqs_div_delay0_high,
      I3 => rst_dqs_div_int,
      O => rst_dqs_div_delay0_delay1
    );
  rst_dqs_div_delay0_three : LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      I0 => rst_dqs_div_int,
      I1 => delay_sel(0),
      I2 => rst_dqs_div_delay0_delay1,
      I3 => rst_dqs_div_delay0_high,
      O => rst_dqs_div_delay0_delay2
    );
  rst_dqs_div_delay0_two : LUT4
    generic map(
      INIT => X"EE22"
    )
    port map (
      I0 => rst_dqs_div_int,
      I1 => delay_sel(2),
      I2 => rst_dqs_div_delay0_high,
      I3 => rst_dqs_div_delay0_delay3,
      O => rst_dqs_div_delay0_delay4
    );
  rst_dqs_div_delay0_one : LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      I0 => rst_dqs_div_delay0_high,
      I1 => delay_sel(4),
      I2 => rst_dqs_div_delay0_delay5,
      I3 => rst_dqs_div_int,
      O => rst_dqs_div_delayed
    );
  rst_dqs_div_delay0_XST_VCC : VCC
    port map (
      P => rst_dqs_div_delay0_high
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity data_path_rst is
  port (
    clk : in STD_LOGIC := 'X'; 
    reset270 : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    clk180 : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    reset90 : in STD_LOGIC := 'X'; 
    reset180 : in STD_LOGIC := 'X'; 
    reset_r : out STD_LOGIC; 
    reset270_r : out STD_LOGIC; 
    reset90_r : out STD_LOGIC; 
    reset180_r : out STD_LOGIC 
  );
end data_path_rst;

architecture Structure of data_path_rst is
begin
  rst0_r : FD
    port map (
      D => reset,
      C => clk,
      Q => reset_r
    );
  rst90_r : FD
    port map (
      D => reset90,
      C => clk90,
      Q => reset90_r
    );
  rst180_r : FD
    port map (
      D => reset180,
      C => clk180,
      Q => reset180_r
    );
  rst270_r : FD
    port map (
      D => reset270,
      C => clk270,
      Q => reset270_r
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity data_path_16bit_rl is
  port (
    rst_dqs_div : in STD_LOGIC := 'X'; 
    clk : in STD_LOGIC := 'X'; 
    reset270 : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    clk180 : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    reset90 : in STD_LOGIC := 'X'; 
    dqs_int_delay_in0 : in STD_LOGIC := 'X'; 
    dqs_int_delay_in1 : in STD_LOGIC := 'X'; 
    write_enable : in STD_LOGIC := 'X'; 
    reset180 : in STD_LOGIC := 'X'; 
    reset270_r_val : out STD_LOGIC; 
    u_data_val : out STD_LOGIC; 
    write_en_val : out STD_LOGIC; 
    dq : in STD_LOGIC_VECTOR ( 15 downto 0 ); 
    delay_sel : in STD_LOGIC_VECTOR ( 4 downto 0 ); 
    user_data_mask : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    user_input_data : in STD_LOGIC_VECTOR ( 31 downto 0 ); 
    write_data_falling : out STD_LOGIC_VECTOR ( 15 downto 0 ); 
    data_mask_f : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
    data_mask_r : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
    write_data_rising : out STD_LOGIC_VECTOR ( 15 downto 0 ); 
    user_output_data : out STD_LOGIC_VECTOR ( 31 downto 0 ) 
  );
end data_path_16bit_rl;

architecture Structure of data_path_16bit_rl is
  component data_path_rst
    port (
      clk : in STD_LOGIC := 'X'; 
      reset270 : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      clk180 : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      reset90 : in STD_LOGIC := 'X'; 
      reset180 : in STD_LOGIC := 'X'; 
      reset_r : out STD_LOGIC; 
      reset270_r : out STD_LOGIC; 
      reset90_r : out STD_LOGIC; 
      reset180_r : out STD_LOGIC 
    );
  end component;
  component data_read_controller_16bit_rl
    port (
      rst_dqs_div : in STD_LOGIC := 'X'; 
      clk : in STD_LOGIC := 'X'; 
      reset_r : in STD_LOGIC := 'X'; 
      reset270_r : in STD_LOGIC := 'X'; 
      reset90_r : in STD_LOGIC := 'X'; 
      clk180 : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      reset180_r : in STD_LOGIC := 'X'; 
      dqs_int_delay_in0 : in STD_LOGIC := 'X'; 
      dqs_int_delay_in1 : in STD_LOGIC := 'X'; 
      next_state : in STD_LOGIC := 'X'; 
      read_valid_data_1_val : out STD_LOGIC; 
      read_valid_data_2_val : out STD_LOGIC; 
      u_data_val : out STD_LOGIC; 
      delay_sel : in STD_LOGIC_VECTOR ( 4 downto 0 ); 
      fifo_01_rd_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_11_wr_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_11_rd_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      dqs_div_col1_val : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
      dqs_delayed_col1_val : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
      transfer_done_0_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_02_wr_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_02_rd_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_12_wr_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      transfer_done_1_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_12_rd_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_03_wr_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_00_wr_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_03_rd_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_00_rd_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_13_wr_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_10_wr_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_13_rd_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_10_rd_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      dqs_div_col0_val : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
      dqs_delayed_col0_val : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
      fifo_01_wr_addr_val : out STD_LOGIC_VECTOR ( 3 downto 0 ) 
    );
  end component;
  component data_read_16bit_rl
    port (
      clk : in STD_LOGIC := 'X'; 
      reset270_r : in STD_LOGIC := 'X'; 
      read_valid_data_1 : in STD_LOGIC := 'X'; 
      read_valid_data_2 : in STD_LOGIC := 'X'; 
      reset90_r : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      next_state_val : out STD_LOGIC; 
      fifo_00_wr_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      dqs_div_col0 : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
      dqs_div_col1 : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
      dqs_delayed_col0 : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
      dqs_delayed_col1 : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
      dq : in STD_LOGIC_VECTOR ( 15 downto 0 ); 
      fifo_13_rd_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_12_rd_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_11_rd_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_10_rd_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_03_rd_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_02_rd_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_01_rd_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_00_rd_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      transfer_done_0 : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      transfer_done_1 : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_13_wr_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_12_wr_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_11_wr_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_10_wr_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_03_wr_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_02_wr_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      fifo_01_wr_addr : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      user_output_data : out STD_LOGIC_VECTOR ( 31 downto 0 ) 
    );
  end component;
  component data_write_16bit
    port (
      reset270_r : in STD_LOGIC := 'X'; 
      reset90_r : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_enable : in STD_LOGIC := 'X'; 
      write_en_val : out STD_LOGIC; 
      user_data_mask : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      user_input_data : in STD_LOGIC_VECTOR ( 31 downto 0 ); 
      write_data_falling : out STD_LOGIC_VECTOR ( 15 downto 0 ); 
      data_mask_f : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
      data_mask_r : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
      write_data_rising : out STD_LOGIC_VECTOR ( 15 downto 0 ) 
    );
  end component;
  signal NlwRenamedSig_OI_reset270_r_val : STD_LOGIC; 
  signal reset_r : STD_LOGIC; 
  signal read_valid_data_1 : STD_LOGIC; 
  signal read_valid_data_2 : STD_LOGIC; 
  signal reset90_r : STD_LOGIC; 
  signal reset180_r : STD_LOGIC; 
  signal next_state : STD_LOGIC; 
  signal transfer_done_0 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal transfer_done_1 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_00_wr_addr : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_01_wr_addr : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_02_wr_addr : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_03_wr_addr : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_10_wr_addr : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_11_wr_addr : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_12_wr_addr : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_13_wr_addr : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal dqs_delayed_col0 : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal dqs_delayed_col1 : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal dqs_div_col0 : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal dqs_div_col1 : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal fifo_00_rd_addr : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_01_rd_addr : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_02_rd_addr : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_03_rd_addr : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_10_rd_addr : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_11_rd_addr : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_12_rd_addr : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal fifo_13_rd_addr : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  reset270_r_val <= NlwRenamedSig_OI_reset270_r_val;
  data_path_rst0 : data_path_rst
    port map (
      clk => clk,
      reset270 => reset270,
      reset => reset,
      clk180 => clk180,
      clk90 => clk90,
      clk270 => clk270,
      reset90 => reset90,
      reset180 => reset180,
      reset_r => reset_r,
      reset270_r => NlwRenamedSig_OI_reset270_r_val,
      reset90_r => reset90_r,
      reset180_r => reset180_r
    );
  data_read_controller0 : data_read_controller_16bit_rl
    port map (
      rst_dqs_div => rst_dqs_div,
      clk => clk,
      reset_r => reset_r,
      reset270_r => NlwRenamedSig_OI_reset270_r_val,
      reset90_r => reset90_r,
      clk180 => clk180,
      clk90 => clk90,
      clk270 => clk270,
      reset180_r => reset180_r,
      dqs_int_delay_in0 => dqs_int_delay_in0,
      dqs_int_delay_in1 => dqs_int_delay_in1,
      next_state => next_state,
      read_valid_data_1_val => read_valid_data_1,
      read_valid_data_2_val => read_valid_data_2,
      u_data_val => u_data_val,
      delay_sel(4) => delay_sel(4),
      delay_sel(3) => delay_sel(3),
      delay_sel(2) => delay_sel(2),
      delay_sel(1) => delay_sel(1),
      delay_sel(0) => delay_sel(0),
      fifo_01_rd_addr_val(3) => fifo_01_rd_addr(3),
      fifo_01_rd_addr_val(2) => fifo_01_rd_addr(2),
      fifo_01_rd_addr_val(1) => fifo_01_rd_addr(1),
      fifo_01_rd_addr_val(0) => fifo_01_rd_addr(0),
      fifo_11_wr_addr_val(3) => fifo_11_wr_addr(3),
      fifo_11_wr_addr_val(2) => fifo_11_wr_addr(2),
      fifo_11_wr_addr_val(1) => fifo_11_wr_addr(1),
      fifo_11_wr_addr_val(0) => fifo_11_wr_addr(0),
      fifo_11_rd_addr_val(3) => fifo_11_rd_addr(3),
      fifo_11_rd_addr_val(2) => fifo_11_rd_addr(2),
      fifo_11_rd_addr_val(1) => fifo_11_rd_addr(1),
      fifo_11_rd_addr_val(0) => fifo_11_rd_addr(0),
      dqs_div_col1_val(1) => dqs_div_col1(1),
      dqs_div_col1_val(0) => dqs_div_col1(0),
      dqs_delayed_col1_val(1) => dqs_delayed_col1(1),
      dqs_delayed_col1_val(0) => dqs_delayed_col1(0),
      transfer_done_0_val(3) => transfer_done_0(3),
      transfer_done_0_val(2) => transfer_done_0(2),
      transfer_done_0_val(1) => transfer_done_0(1),
      transfer_done_0_val(0) => transfer_done_0(0),
      fifo_02_wr_addr_val(3) => fifo_02_wr_addr(3),
      fifo_02_wr_addr_val(2) => fifo_02_wr_addr(2),
      fifo_02_wr_addr_val(1) => fifo_02_wr_addr(1),
      fifo_02_wr_addr_val(0) => fifo_02_wr_addr(0),
      fifo_02_rd_addr_val(3) => fifo_02_rd_addr(3),
      fifo_02_rd_addr_val(2) => fifo_02_rd_addr(2),
      fifo_02_rd_addr_val(1) => fifo_02_rd_addr(1),
      fifo_02_rd_addr_val(0) => fifo_02_rd_addr(0),
      fifo_12_wr_addr_val(3) => fifo_12_wr_addr(3),
      fifo_12_wr_addr_val(2) => fifo_12_wr_addr(2),
      fifo_12_wr_addr_val(1) => fifo_12_wr_addr(1),
      fifo_12_wr_addr_val(0) => fifo_12_wr_addr(0),
      transfer_done_1_val(3) => transfer_done_1(3),
      transfer_done_1_val(2) => transfer_done_1(2),
      transfer_done_1_val(1) => transfer_done_1(1),
      transfer_done_1_val(0) => transfer_done_1(0),
      fifo_12_rd_addr_val(3) => fifo_12_rd_addr(3),
      fifo_12_rd_addr_val(2) => fifo_12_rd_addr(2),
      fifo_12_rd_addr_val(1) => fifo_12_rd_addr(1),
      fifo_12_rd_addr_val(0) => fifo_12_rd_addr(0),
      fifo_03_wr_addr_val(3) => fifo_03_wr_addr(3),
      fifo_03_wr_addr_val(2) => fifo_03_wr_addr(2),
      fifo_03_wr_addr_val(1) => fifo_03_wr_addr(1),
      fifo_03_wr_addr_val(0) => fifo_03_wr_addr(0),
      fifo_00_wr_addr_val(3) => fifo_00_wr_addr(3),
      fifo_00_wr_addr_val(2) => fifo_00_wr_addr(2),
      fifo_00_wr_addr_val(1) => fifo_00_wr_addr(1),
      fifo_00_wr_addr_val(0) => fifo_00_wr_addr(0),
      fifo_03_rd_addr_val(3) => fifo_03_rd_addr(3),
      fifo_03_rd_addr_val(2) => fifo_03_rd_addr(2),
      fifo_03_rd_addr_val(1) => fifo_03_rd_addr(1),
      fifo_03_rd_addr_val(0) => fifo_03_rd_addr(0),
      fifo_00_rd_addr_val(3) => fifo_00_rd_addr(3),
      fifo_00_rd_addr_val(2) => fifo_00_rd_addr(2),
      fifo_00_rd_addr_val(1) => fifo_00_rd_addr(1),
      fifo_00_rd_addr_val(0) => fifo_00_rd_addr(0),
      fifo_13_wr_addr_val(3) => fifo_13_wr_addr(3),
      fifo_13_wr_addr_val(2) => fifo_13_wr_addr(2),
      fifo_13_wr_addr_val(1) => fifo_13_wr_addr(1),
      fifo_13_wr_addr_val(0) => fifo_13_wr_addr(0),
      fifo_10_wr_addr_val(3) => fifo_10_wr_addr(3),
      fifo_10_wr_addr_val(2) => fifo_10_wr_addr(2),
      fifo_10_wr_addr_val(1) => fifo_10_wr_addr(1),
      fifo_10_wr_addr_val(0) => fifo_10_wr_addr(0),
      fifo_13_rd_addr_val(3) => fifo_13_rd_addr(3),
      fifo_13_rd_addr_val(2) => fifo_13_rd_addr(2),
      fifo_13_rd_addr_val(1) => fifo_13_rd_addr(1),
      fifo_13_rd_addr_val(0) => fifo_13_rd_addr(0),
      fifo_10_rd_addr_val(3) => fifo_10_rd_addr(3),
      fifo_10_rd_addr_val(2) => fifo_10_rd_addr(2),
      fifo_10_rd_addr_val(1) => fifo_10_rd_addr(1),
      fifo_10_rd_addr_val(0) => fifo_10_rd_addr(0),
      dqs_div_col0_val(1) => dqs_div_col0(1),
      dqs_div_col0_val(0) => dqs_div_col0(0),
      dqs_delayed_col0_val(1) => dqs_delayed_col0(1),
      dqs_delayed_col0_val(0) => dqs_delayed_col0(0),
      fifo_01_wr_addr_val(3) => fifo_01_wr_addr(3),
      fifo_01_wr_addr_val(2) => fifo_01_wr_addr(2),
      fifo_01_wr_addr_val(1) => fifo_01_wr_addr(1),
      fifo_01_wr_addr_val(0) => fifo_01_wr_addr(0)
    );
  data_read0 : data_read_16bit_rl
    port map (
      clk => clk,
      reset270_r => NlwRenamedSig_OI_reset270_r_val,
      read_valid_data_1 => read_valid_data_1,
      read_valid_data_2 => read_valid_data_2,
      reset90_r => reset90_r,
      clk90 => clk90,
      next_state_val => next_state,
      fifo_00_wr_addr(3) => fifo_00_wr_addr(3),
      fifo_00_wr_addr(2) => fifo_00_wr_addr(2),
      fifo_00_wr_addr(1) => fifo_00_wr_addr(1),
      fifo_00_wr_addr(0) => fifo_00_wr_addr(0),
      dqs_div_col0(1) => dqs_div_col0(1),
      dqs_div_col0(0) => dqs_div_col0(0),
      dqs_div_col1(1) => dqs_div_col1(1),
      dqs_div_col1(0) => dqs_div_col1(0),
      dqs_delayed_col0(1) => dqs_delayed_col0(1),
      dqs_delayed_col0(0) => dqs_delayed_col0(0),
      dqs_delayed_col1(1) => dqs_delayed_col1(1),
      dqs_delayed_col1(0) => dqs_delayed_col1(0),
      dq(15) => dq(15),
      dq(14) => dq(14),
      dq(13) => dq(13),
      dq(12) => dq(12),
      dq(11) => dq(11),
      dq(10) => dq(10),
      dq(9) => dq(9),
      dq(8) => dq(8),
      dq(7) => dq(7),
      dq(6) => dq(6),
      dq(5) => dq(5),
      dq(4) => dq(4),
      dq(3) => dq(3),
      dq(2) => dq(2),
      dq(1) => dq(1),
      dq(0) => dq(0),
      fifo_13_rd_addr(3) => fifo_13_rd_addr(3),
      fifo_13_rd_addr(2) => fifo_13_rd_addr(2),
      fifo_13_rd_addr(1) => fifo_13_rd_addr(1),
      fifo_13_rd_addr(0) => fifo_13_rd_addr(0),
      fifo_12_rd_addr(3) => fifo_12_rd_addr(3),
      fifo_12_rd_addr(2) => fifo_12_rd_addr(2),
      fifo_12_rd_addr(1) => fifo_12_rd_addr(1),
      fifo_12_rd_addr(0) => fifo_12_rd_addr(0),
      fifo_11_rd_addr(3) => fifo_11_rd_addr(3),
      fifo_11_rd_addr(2) => fifo_11_rd_addr(2),
      fifo_11_rd_addr(1) => fifo_11_rd_addr(1),
      fifo_11_rd_addr(0) => fifo_11_rd_addr(0),
      fifo_10_rd_addr(3) => fifo_10_rd_addr(3),
      fifo_10_rd_addr(2) => fifo_10_rd_addr(2),
      fifo_10_rd_addr(1) => fifo_10_rd_addr(1),
      fifo_10_rd_addr(0) => fifo_10_rd_addr(0),
      fifo_03_rd_addr(3) => fifo_03_rd_addr(3),
      fifo_03_rd_addr(2) => fifo_03_rd_addr(2),
      fifo_03_rd_addr(1) => fifo_03_rd_addr(1),
      fifo_03_rd_addr(0) => fifo_03_rd_addr(0),
      fifo_02_rd_addr(3) => fifo_02_rd_addr(3),
      fifo_02_rd_addr(2) => fifo_02_rd_addr(2),
      fifo_02_rd_addr(1) => fifo_02_rd_addr(1),
      fifo_02_rd_addr(0) => fifo_02_rd_addr(0),
      fifo_01_rd_addr(3) => fifo_01_rd_addr(3),
      fifo_01_rd_addr(2) => fifo_01_rd_addr(2),
      fifo_01_rd_addr(1) => fifo_01_rd_addr(1),
      fifo_01_rd_addr(0) => fifo_01_rd_addr(0),
      fifo_00_rd_addr(3) => fifo_00_rd_addr(3),
      fifo_00_rd_addr(2) => fifo_00_rd_addr(2),
      fifo_00_rd_addr(1) => fifo_00_rd_addr(1),
      fifo_00_rd_addr(0) => fifo_00_rd_addr(0),
      transfer_done_0(3) => transfer_done_0(3),
      transfer_done_0(2) => transfer_done_0(2),
      transfer_done_0(1) => transfer_done_0(1),
      transfer_done_0(0) => transfer_done_0(0),
      transfer_done_1(3) => transfer_done_1(3),
      transfer_done_1(2) => transfer_done_1(2),
      transfer_done_1(1) => transfer_done_1(1),
      transfer_done_1(0) => transfer_done_1(0),
      fifo_13_wr_addr(3) => fifo_13_wr_addr(3),
      fifo_13_wr_addr(2) => fifo_13_wr_addr(2),
      fifo_13_wr_addr(1) => fifo_13_wr_addr(1),
      fifo_13_wr_addr(0) => fifo_13_wr_addr(0),
      fifo_12_wr_addr(3) => fifo_12_wr_addr(3),
      fifo_12_wr_addr(2) => fifo_12_wr_addr(2),
      fifo_12_wr_addr(1) => fifo_12_wr_addr(1),
      fifo_12_wr_addr(0) => fifo_12_wr_addr(0),
      fifo_11_wr_addr(3) => fifo_11_wr_addr(3),
      fifo_11_wr_addr(2) => fifo_11_wr_addr(2),
      fifo_11_wr_addr(1) => fifo_11_wr_addr(1),
      fifo_11_wr_addr(0) => fifo_11_wr_addr(0),
      fifo_10_wr_addr(3) => fifo_10_wr_addr(3),
      fifo_10_wr_addr(2) => fifo_10_wr_addr(2),
      fifo_10_wr_addr(1) => fifo_10_wr_addr(1),
      fifo_10_wr_addr(0) => fifo_10_wr_addr(0),
      fifo_03_wr_addr(3) => fifo_03_wr_addr(3),
      fifo_03_wr_addr(2) => fifo_03_wr_addr(2),
      fifo_03_wr_addr(1) => fifo_03_wr_addr(1),
      fifo_03_wr_addr(0) => fifo_03_wr_addr(0),
      fifo_02_wr_addr(3) => fifo_02_wr_addr(3),
      fifo_02_wr_addr(2) => fifo_02_wr_addr(2),
      fifo_02_wr_addr(1) => fifo_02_wr_addr(1),
      fifo_02_wr_addr(0) => fifo_02_wr_addr(0),
      fifo_01_wr_addr(3) => fifo_01_wr_addr(3),
      fifo_01_wr_addr(2) => fifo_01_wr_addr(2),
      fifo_01_wr_addr(1) => fifo_01_wr_addr(1),
      fifo_01_wr_addr(0) => fifo_01_wr_addr(0),
      user_output_data(31) => user_output_data(31),
      user_output_data(30) => user_output_data(30),
      user_output_data(29) => user_output_data(29),
      user_output_data(28) => user_output_data(28),
      user_output_data(27) => user_output_data(27),
      user_output_data(26) => user_output_data(26),
      user_output_data(25) => user_output_data(25),
      user_output_data(24) => user_output_data(24),
      user_output_data(23) => user_output_data(23),
      user_output_data(22) => user_output_data(22),
      user_output_data(21) => user_output_data(21),
      user_output_data(20) => user_output_data(20),
      user_output_data(19) => user_output_data(19),
      user_output_data(18) => user_output_data(18),
      user_output_data(17) => user_output_data(17),
      user_output_data(16) => user_output_data(16),
      user_output_data(15) => user_output_data(15),
      user_output_data(14) => user_output_data(14),
      user_output_data(13) => user_output_data(13),
      user_output_data(12) => user_output_data(12),
      user_output_data(11) => user_output_data(11),
      user_output_data(10) => user_output_data(10),
      user_output_data(9) => user_output_data(9),
      user_output_data(8) => user_output_data(8),
      user_output_data(7) => user_output_data(7),
      user_output_data(6) => user_output_data(6),
      user_output_data(5) => user_output_data(5),
      user_output_data(4) => user_output_data(4),
      user_output_data(3) => user_output_data(3),
      user_output_data(2) => user_output_data(2),
      user_output_data(1) => user_output_data(1),
      user_output_data(0) => user_output_data(0)
    );
  data_write0 : data_write_16bit
    port map (
      reset270_r => NlwRenamedSig_OI_reset270_r_val,
      reset90_r => reset90_r,
      clk90 => clk90,
      clk270 => clk270,
      write_enable => write_enable,
      write_en_val => write_en_val,
      user_data_mask(3) => user_data_mask(3),
      user_data_mask(2) => user_data_mask(2),
      user_data_mask(1) => user_data_mask(1),
      user_data_mask(0) => user_data_mask(0),
      user_input_data(31) => user_input_data(31),
      user_input_data(30) => user_input_data(30),
      user_input_data(29) => user_input_data(29),
      user_input_data(28) => user_input_data(28),
      user_input_data(27) => user_input_data(27),
      user_input_data(26) => user_input_data(26),
      user_input_data(25) => user_input_data(25),
      user_input_data(24) => user_input_data(24),
      user_input_data(23) => user_input_data(23),
      user_input_data(22) => user_input_data(22),
      user_input_data(21) => user_input_data(21),
      user_input_data(20) => user_input_data(20),
      user_input_data(19) => user_input_data(19),
      user_input_data(18) => user_input_data(18),
      user_input_data(17) => user_input_data(17),
      user_input_data(16) => user_input_data(16),
      user_input_data(15) => user_input_data(15),
      user_input_data(14) => user_input_data(14),
      user_input_data(13) => user_input_data(13),
      user_input_data(12) => user_input_data(12),
      user_input_data(11) => user_input_data(11),
      user_input_data(10) => user_input_data(10),
      user_input_data(9) => user_input_data(9),
      user_input_data(8) => user_input_data(8),
      user_input_data(7) => user_input_data(7),
      user_input_data(6) => user_input_data(6),
      user_input_data(5) => user_input_data(5),
      user_input_data(4) => user_input_data(4),
      user_input_data(3) => user_input_data(3),
      user_input_data(2) => user_input_data(2),
      user_input_data(1) => user_input_data(1),
      user_input_data(0) => user_input_data(0),
      write_data_falling(15) => write_data_falling(15),
      write_data_falling(14) => write_data_falling(14),
      write_data_falling(13) => write_data_falling(13),
      write_data_falling(12) => write_data_falling(12),
      write_data_falling(11) => write_data_falling(11),
      write_data_falling(10) => write_data_falling(10),
      write_data_falling(9) => write_data_falling(9),
      write_data_falling(8) => write_data_falling(8),
      write_data_falling(7) => write_data_falling(7),
      write_data_falling(6) => write_data_falling(6),
      write_data_falling(5) => write_data_falling(5),
      write_data_falling(4) => write_data_falling(4),
      write_data_falling(3) => write_data_falling(3),
      write_data_falling(2) => write_data_falling(2),
      write_data_falling(1) => write_data_falling(1),
      write_data_falling(0) => write_data_falling(0),
      data_mask_f(1) => data_mask_f(1),
      data_mask_f(0) => data_mask_f(0),
      data_mask_r(1) => data_mask_r(1),
      data_mask_r(0) => data_mask_r(0),
      write_data_rising(15) => write_data_rising(15),
      write_data_rising(14) => write_data_rising(14),
      write_data_rising(13) => write_data_rising(13),
      write_data_rising(12) => write_data_rising(12),
      write_data_rising(11) => write_data_rising(11),
      write_data_rising(10) => write_data_rising(10),
      write_data_rising(9) => write_data_rising(9),
      write_data_rising(8) => write_data_rising(8),
      write_data_rising(7) => write_data_rising(7),
      write_data_rising(6) => write_data_rising(6),
      write_data_rising(5) => write_data_rising(5),
      write_data_rising(4) => write_data_rising(4),
      write_data_rising(3) => write_data_rising(3),
      write_data_rising(2) => write_data_rising(2),
      write_data_rising(1) => write_data_rising(1),
      write_data_rising(0) => write_data_rising(0)
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity controller_16bit_00 is
  port (
    clk : in STD_LOGIC := 'X'; 
    rst0 : in STD_LOGIC := 'X'; 
    wait_200us : in STD_LOGIC := 'X'; 
    clk180 : in STD_LOGIC := 'X'; 
    dip1 : in STD_LOGIC := 'X'; 
    dip3 : in STD_LOGIC := 'X'; 
    burst_done : in STD_LOGIC := 'X'; 
    rst180 : in STD_LOGIC := 'X'; 
    ddr_ODT_cntrl : out STD_LOGIC; 
    ddr_csb_cntrl : out STD_LOGIC; 
    cmd_ack : out STD_LOGIC; 
    auto_ref_req : out STD_LOGIC; 
    ar_done : out STD_LOGIC; 
    ddr_web_cntrl : out STD_LOGIC; 
    dqs_enable : out STD_LOGIC; 
    ddr_casb_cntrl : out STD_LOGIC; 
    rst_calib : out STD_LOGIC; 
    ddr_cke_cntrl : out STD_LOGIC; 
    init : out STD_LOGIC; 
    dqs_reset : out STD_LOGIC; 
    ddr_rasb_cntrl : out STD_LOGIC; 
    write_enable : out STD_LOGIC; 
    rst_dqs_div_int : out STD_LOGIC; 
    address : in STD_LOGIC_VECTOR ( 22 downto 0 ); 
    config_register1 : in STD_LOGIC_VECTOR ( 14 downto 0 ); 
    config_register2 : in STD_LOGIC_VECTOR ( 12 downto 0 ); 
    bank_address : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
    command_register : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    ddr_ba_cntrl : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
    ddr_address_cntrl : out STD_LOGIC_VECTOR ( 12 downto 0 ) 
  );
end controller_16bit_00;

architecture Structure of controller_16bit_00 is
  signal NlwRenamedSig_OI_auto_ref_req : STD_LOGIC; 
  signal NlwRenamedSig_OI_dqs_enable : STD_LOGIC; 
  signal NlwRenamedSig_OI_init : STD_LOGIC; 
  signal CONFLICT_value : STD_LOGIC; 
  signal DQS_enable1_103 : STD_LOGIC; 
  signal DQS_enable2_104 : STD_LOGIC; 
  signal AUTO_REF_detect_105 : STD_LOGIC; 
  signal CONFLICT_106 : STD_LOGIC; 
  signal Auto_Ref_issued_107 : STD_LOGIC; 
  signal ddr_ODT2_108 : STD_LOGIC; 
  signal low : STD_LOGIC; 
  signal Auto_Ref_issued_p : STD_LOGIC; 
  signal AUTO_REF_detect1_109 : STD_LOGIC; 
  signal Write_enable_out : STD_LOGIC; 
  signal write_cmd_in : STD_LOGIC; 
  signal GO_TO_ODT_ON_110 : STD_LOGIC; 
  signal read_cmd1_111 : STD_LOGIC; 
  signal read_cmd2_112 : STD_LOGIC; 
  signal read_cmd3_113 : STD_LOGIC; 
  signal read_cmd4_114 : STD_LOGIC; 
  signal read_cmd5_115 : STD_LOGIC; 
  signal GO_TO_ACTIVE_value : STD_LOGIC; 
  signal RFC_COUNT_reg_116 : STD_LOGIC; 
  signal read_cmd : STD_LOGIC; 
  signal DQS_reset_int_117 : STD_LOGIC; 
  signal DQS_reset1_clk0_118 : STD_LOGIC; 
  signal auto_ref_wait1_119 : STD_LOGIC; 
  signal auto_ref_wait2_120 : STD_LOGIC; 
  signal DQS_reset2_clk0_121 : STD_LOGIC; 
  signal DQS_enable_int_122 : STD_LOGIC; 
  signal BA_address_conflict_123 : STD_LOGIC; 
  signal accept_cmd_in : STD_LOGIC; 
  signal AR_Done_reg_124 : STD_LOGIC; 
  signal write_cmd1_125 : STD_LOGIC; 
  signal write_cmd2_126 : STD_LOGIC; 
  signal write_cmd3_127 : STD_LOGIC; 
  signal write_cmd4_128 : STD_LOGIC; 
  signal write_cmd5_129 : STD_LOGIC; 
  signal write_cmd6_130 : STD_LOGIC; 
  signal write_cmd7_131 : STD_LOGIC; 
  signal write_cmd8_132 : STD_LOGIC; 
  signal Q_n0031 : STD_LOGIC; 
  signal INIT_MEMORY_133 : STD_LOGIC; 
  signal DQS_reset_out : STD_LOGIC; 
  signal Q_n0200 : STD_LOGIC; 
  signal current_state_FFd5_134 : STD_LOGIC; 
  signal current_state_FFd7_135 : STD_LOGIC; 
  signal current_state_FFd12_136 : STD_LOGIC; 
  signal current_state_FFd6_137 : STD_LOGIC; 
  signal current_state_FFd13_138 : STD_LOGIC; 
  signal Q_n0213 : STD_LOGIC; 
  signal current_state_FFd9_139 : STD_LOGIC; 
  signal current_state_FFd11_140 : STD_LOGIC; 
  signal Q_n0215 : STD_LOGIC; 
  signal current_state_FFd8_141 : STD_LOGIC; 
  signal current_state_FFd4_142 : STD_LOGIC; 
  signal Q_n0217 : STD_LOGIC; 
  signal current_state_FFd3_143 : STD_LOGIC; 
  signal current_state_FFd14_144 : STD_LOGIC; 
  signal current_state_FFd10_145 : STD_LOGIC; 
  signal current_state_FFd2_146 : STD_LOGIC; 
  signal current_state_FFd1_147 : STD_LOGIC; 
  signal Q_n0181 : STD_LOGIC; 
  signal Q_n0235 : STD_LOGIC; 
  signal Q_n0241 : STD_LOGIC; 
  signal Q_n0242 : STD_LOGIC; 
  signal Q_n0238_148 : STD_LOGIC; 
  signal Q_n0243 : STD_LOGIC; 
  signal Q_n0244 : STD_LOGIC; 
  signal Q_n0245_149 : STD_LOGIC; 
  signal Q_n0250_150 : STD_LOGIC; 
  signal Q_n0247 : STD_LOGIC; 
  signal Q_n0248 : STD_LOGIC; 
  signal Q_n0322 : STD_LOGIC; 
  signal Q_n0282_151 : STD_LOGIC; 
  signal Q_n0283 : STD_LOGIC; 
  signal Q_n0342 : STD_LOGIC; 
  signal Q_n0337 : STD_LOGIC; 
  signal rst_dqs_div_r_152 : STD_LOGIC; 
  signal Q_n0380 : STD_LOGIC; 
  signal Q_n0381_153 : STD_LOGIC; 
  signal DQS_enable_out : STD_LOGIC; 
  signal Q_n0378 : STD_LOGIC; 
  signal Q_n0385 : STD_LOGIC; 
  signal rst0_r_154 : STD_LOGIC; 
  signal rst180_r_155 : STD_LOGIC; 
  signal INIT_PRE_COUNT_en : STD_LOGIC; 
  signal AUTO_REF_pulse_end_156 : STD_LOGIC; 
  signal wrburst_end_1_157 : STD_LOGIC; 
  signal GO_TO_ACTIVE_158 : STD_LOGIC; 
  signal wrburst_end_2_159 : STD_LOGIC; 
  signal wrburst_end_3_160 : STD_LOGIC; 
  signal GO_TO_ODT_ON_value : STD_LOGIC; 
  signal INIT_DONE_value : STD_LOGIC; 
  signal auto_ref_161 : STD_LOGIC; 
  signal AUTOREF_value : STD_LOGIC; 
  signal ack_o : STD_LOGIC; 
  signal initialize_memory : STD_LOGIC; 
  signal rdburst_end_1_162 : STD_LOGIC; 
  signal rdburst_end_2_163 : STD_LOGIC; 
  signal EMR_9_Q : STD_LOGIC; 
  signal EMR_8_Q : STD_LOGIC; 
  signal EMR_7_Q : STD_LOGIC; 
  signal EMR_2_Q : STD_LOGIC; 
  signal LMR_DLL_rst_10_Q : STD_LOGIC; 
  signal LMR_DLL_rst_8_Q : STD_LOGIC; 
  signal config_reg1_12_Q : STD_LOGIC; 
  signal config_reg1_5_Q : STD_LOGIC; 
  signal config_reg1_4_Q : STD_LOGIC; 
  signal config_reg1_1_Q : STD_LOGIC; 
  signal config_reg2_9_Q : STD_LOGIC; 
  signal config_reg2_8_Q : STD_LOGIC; 
  signal config_reg2_7_Q : STD_LOGIC; 
  signal config_reg2_2_Q : STD_LOGIC; 
  signal N5 : STD_LOGIC; 
  signal controller_16bit_00_n0254_0_cyo : STD_LOGIC; 
  signal controller_16bit_00_n0254_1_cyo : STD_LOGIC; 
  signal controller_16bit_00_n0254_2_cyo : STD_LOGIC; 
  signal controller_16bit_00_n0254_3_cyo : STD_LOGIC; 
  signal controller_16bit_00_n0254_4_cyo : STD_LOGIC; 
  signal controller_16bit_00_n0254_5_cyo : STD_LOGIC; 
  signal controller_16bit_00_n0254_6_cyo : STD_LOGIC; 
  signal controller_16bit_00_n0254_7_cyo : STD_LOGIC; 
  signal controller_16bit_00_n0254_8_cyo : STD_LOGIC; 
  signal controller_16bit_00_n0254_9_cyo : STD_LOGIC; 
  signal controller_16bit_00_n0254_10_cyo : STD_LOGIC; 
  signal current_state_FFd1_In : STD_LOGIC; 
  signal current_state_FFd2_In : STD_LOGIC; 
  signal current_state_FFd3_In : STD_LOGIC; 
  signal current_state_FFd4_In : STD_LOGIC; 
  signal current_state_FFd8_In_164 : STD_LOGIC; 
  signal current_state_FFd10_In : STD_LOGIC; 
  signal current_state_FFd14_In : STD_LOGIC; 
  signal dqs_div_rdburstcount_0_n0000 : STD_LOGIC; 
  signal WR_COUNT_0_n0000 : STD_LOGIC; 
  signal N10 : STD_LOGIC; 
  signal N13 : STD_LOGIC; 
  signal N15 : STD_LOGIC; 
  signal N19 : STD_LOGIC; 
  signal N29 : STD_LOGIC; 
  signal N48 : STD_LOGIC; 
  signal N53 : STD_LOGIC; 
  signal N55 : STD_LOGIC; 
  signal N57 : STD_LOGIC; 
  signal N58 : STD_LOGIC; 
  signal N0 : STD_LOGIC; 
  signal N1 : STD_LOGIC; 
  signal N6 : STD_LOGIC; 
  signal N7 : STD_LOGIC; 
  signal N12 : STD_LOGIC; 
  signal N171 : STD_LOGIC; 
  signal N191 : STD_LOGIC; 
  signal N25 : STD_LOGIC; 
  signal N26 : STD_LOGIC; 
  signal N32 : STD_LOGIC; 
  signal N35 : STD_LOGIC; 
  signal N501 : STD_LOGIC; 
  signal N01 : STD_LOGIC; 
  signal N241 : STD_LOGIC; 
  signal N351 : STD_LOGIC; 
  signal N59 : STD_LOGIC; 
  signal N63 : STD_LOGIC; 
  signal N65 : STD_LOGIC; 
  signal N69 : STD_LOGIC; 
  signal N72 : STD_LOGIC; 
  signal N73 : STD_LOGIC; 
  signal N11 : STD_LOGIC; 
  signal N21 : STD_LOGIC; 
  signal N23 : STD_LOGIC; 
  signal N36 : STD_LOGIC; 
  signal N49 : STD_LOGIC; 
  signal N54 : STD_LOGIC; 
  signal N56 : STD_LOGIC; 
  signal N60 : STD_LOGIC; 
  signal N691 : STD_LOGIC; 
  signal N71 : STD_LOGIC; 
  signal N731 : STD_LOGIC; 
  signal N75 : STD_LOGIC; 
  signal current_state_FFd10_In_map75 : STD_LOGIC; 
  signal current_state_FFd10_In_map79 : STD_LOGIC; 
  signal ddr_address1_2_map88 : STD_LOGIC; 
  signal N155 : STD_LOGIC; 
  signal N157 : STD_LOGIC; 
  signal N165 : STD_LOGIC; 
  signal N175 : STD_LOGIC; 
  signal N177 : STD_LOGIC; 
  signal N179 : STD_LOGIC; 
  signal RFC_COUNT_value_2_map254 : STD_LOGIC; 
  signal N239 : STD_LOGIC; 
  signal Ker36_map330 : STD_LOGIC; 
  signal Ker36_map335 : STD_LOGIC; 
  signal Ker36_map340 : STD_LOGIC; 
  signal ddr_address1_8_map343 : STD_LOGIC; 
  signal ddr_address1_8_map350 : STD_LOGIC; 
  signal ddr_address1_8_map352 : STD_LOGIC; 
  signal N317 : STD_LOGIC; 
  signal N319 : STD_LOGIC; 
  signal N339 : STD_LOGIC; 
  signal N343 : STD_LOGIC; 
  signal N346 : STD_LOGIC; 
  signal current_state_FFd13_In_map547 : STD_LOGIC; 
  signal current_state_FFd13_In_map562 : STD_LOGIC; 
  signal current_state_FFd13_In_map563 : STD_LOGIC; 
  signal current_state_FFd13_In_map568 : STD_LOGIC; 
  signal current_state_FFd13_In_map575 : STD_LOGIC; 
  signal N424 : STD_LOGIC; 
  signal N425 : STD_LOGIC; 
  signal current_state_FFd14_In_map624 : STD_LOGIC; 
  signal current_state_FFd14_In_map632 : STD_LOGIC; 
  signal current_state_FFd14_In_map640 : STD_LOGIC; 
  signal current_state_FFd14_In_map642 : STD_LOGIC; 
  signal current_state_FFd14_In_map645 : STD_LOGIC; 
  signal current_state_FFd14_In_map655 : STD_LOGIC; 
  signal current_state_FFd14_In_map663 : STD_LOGIC; 
  signal current_state_FFd14_In_map664 : STD_LOGIC; 
  signal current_state_FFd14_In_map666 : STD_LOGIC; 
  signal current_state_FFd14_In_map668 : STD_LOGIC; 
  signal N555 : STD_LOGIC; 
  signal N558 : STD_LOGIC; 
  signal N563 : STD_LOGIC; 
  signal N578 : STD_LOGIC; 
  signal AUTOREF_COUNT_1_rt_165 : STD_LOGIC; 
  signal AUTOREF_COUNT_2_rt_166 : STD_LOGIC; 
  signal AUTOREF_COUNT_3_rt_167 : STD_LOGIC; 
  signal AUTOREF_COUNT_4_rt_168 : STD_LOGIC; 
  signal AUTOREF_COUNT_5_rt_169 : STD_LOGIC; 
  signal AUTOREF_COUNT_6_rt_170 : STD_LOGIC; 
  signal AUTOREF_COUNT_7_rt_171 : STD_LOGIC; 
  signal AUTOREF_COUNT_8_rt_172 : STD_LOGIC; 
  signal AUTOREF_COUNT_9_rt_173 : STD_LOGIC; 
  signal AUTOREF_COUNT_10_rt_174 : STD_LOGIC; 
  signal AUTOREF_COUNT_11_rt_175 : STD_LOGIC; 
  signal DQS_reset1_clk0_rt_176 : STD_LOGIC; 
  signal current_state_FFd3_rt_177 : STD_LOGIC; 
  signal INIT_PRE_COUNT_4_rt_178 : STD_LOGIC; 
  signal N586 : STD_LOGIC; 
  signal N590 : STD_LOGIC; 
  signal N592 : STD_LOGIC; 
  signal N594 : STD_LOGIC; 
  signal N596 : STD_LOGIC; 
  signal N598 : STD_LOGIC; 
  signal N600 : STD_LOGIC; 
  signal N602 : STD_LOGIC; 
  signal N604 : STD_LOGIC; 
  signal N606 : STD_LOGIC; 
  signal N608 : STD_LOGIC; 
  signal N610 : STD_LOGIC; 
  signal N612 : STD_LOGIC; 
  signal N614 : STD_LOGIC; 
  signal N615 : STD_LOGIC; 
  signal N616 : STD_LOGIC; 
  signal N617 : STD_LOGIC; 
  signal N618 : STD_LOGIC; 
  signal N619 : STD_LOGIC; 
  signal N620 : STD_LOGIC; 
  signal N621 : STD_LOGIC; 
  signal Q_n0254 : STD_LOGIC_VECTOR ( 11 downto 1 ); 
  signal AUTOREF_CNT_val : STD_LOGIC_VECTOR ( 11 downto 0 ); 
  signal NlwRenamedSig_OI_high : STD_LOGIC_VECTOR ( 1 downto 1 ); 
  signal LMR_DLL_set : STD_LOGIC_VECTOR ( 10 downto 10 ); 
  signal address_reg : STD_LOGIC_VECTOR ( 9 downto 2 ); 
  signal CAS_COUNT : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal WR_COUNT : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal wrburst_end_cnt : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal burst_length : STD_LOGIC_VECTOR ( 1 downto 1 ); 
  signal RCDR_COUNT : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal RCDW_COUNT : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal ODT_COUNT : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal INIT_COUNT : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal MRD_COUNT : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal RP_COUNT : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal INIT_PRE_COUNT : STD_LOGIC_VECTOR ( 6 downto 0 ); 
  signal cas_latency : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal COLUMN_ADDRESS_reg : STD_LOGIC_VECTOR ( 9 downto 2 ); 
  signal dqs_div_cascount : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal AUTOREF_COUNT : STD_LOGIC_VECTOR ( 11 downto 0 ); 
  signal dqs_div_rdburstcount : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal RRD_COUNT : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal DLL_RST_COUNT : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal RFC_COUNT : STD_LOGIC_VECTOR ( 5 downto 0 ); 
  signal RCDR_COUNT_value : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal RFC_COUNT_value : STD_LOGIC_VECTOR ( 5 downto 0 ); 
  signal RRD_COUNT_value : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal MRD_COUNT_value : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal DLL_RST_COUNT_value : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal ODT_COUNT_value : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal RP_cnt_value : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal wrburst_end_cnt_value : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal RCDW_COUNT_value : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal cas_count_value : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal dqs_div_rdburstcount_n0000 : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal WR_COUNT_n0000 : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal dqs_div_cascount_n0001 : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal INIT_PRE_COUNT_n0000 : STD_LOGIC_VECTOR ( 6 downto 0 ); 
  signal INIT_COUNT_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  ddr_csb_cntrl <= NlwRenamedSig_OI_high(1);
  auto_ref_req <= NlwRenamedSig_OI_auto_ref_req;
  dqs_enable <= NlwRenamedSig_OI_dqs_enable;
  init <= NlwRenamedSig_OI_init;
  ddr_address_cntrl(12) <= NlwRenamedSig_OI_high(1);
  ddr_address_cntrl(11) <= NlwRenamedSig_OI_high(1);
  ddr_address_cntrl(0) <= NlwRenamedSig_OI_high(1);
  XST_GND : GND
    port map (
      G => NlwRenamedSig_OI_high(1)
    );
  XST_VCC : VCC
    port map (
      P => low
    );
  wrburst_end_1 : FDR
    port map (
      D => low,
      R => Q_n0245_149,
      C => clk180,
      Q => wrburst_end_1_157
    );
  rst0_r : FD
    port map (
      D => rst0,
      C => clk,
      Q => rst0_r_154
    );
  AR_Done_reg : FDR
    port map (
      D => low,
      R => Q_n0247,
      C => clk180,
      Q => AR_Done_reg_124
    );
  rst180_r : FD
    port map (
      D => rst180,
      C => clk180,
      Q => rst180_r_155
    );
  rdburst_end_1 : FDR
    port map (
      D => low,
      R => Q_n0250_150,
      C => clk180,
      Q => rdburst_end_1_162
    );
  BA_address_conflict : FDR
    port map (
      D => low,
      R => low,
      C => clk180,
      Q => BA_address_conflict_123
    );
  read_cmd1 : FDRE
    port map (
      D => read_cmd,
      R => rst180_r_155,
      CE => accept_cmd_in,
      C => clk180,
      Q => read_cmd1_111
    );
  write_cmd1 : FDRE
    port map (
      D => write_cmd_in,
      R => rst180_r_155,
      CE => accept_cmd_in,
      C => clk180,
      Q => write_cmd1_125
    );
  write_enable_101 : FDR
    port map (
      D => Write_enable_out,
      R => rst180_r_155,
      C => clk180,
      Q => write_enable
    );
  AUTO_REF_detect1 : FDR
    port map (
      D => AUTOREF_value,
      R => rst180_r_155,
      C => clk180,
      Q => AUTO_REF_detect1_109
    );
  auto_ref_wait1 : FDR
    port map (
      D => Q_n0031,
      R => rst180_r_155,
      C => clk180,
      Q => auto_ref_wait1_119
    );
  GO_TO_ACTIVE : FDR
    port map (
      D => GO_TO_ACTIVE_value,
      R => rst180_r_155,
      C => clk180,
      Q => GO_TO_ACTIVE_158
    );
  config_reg1_1 : FDR
    port map (
      D => config_register1(1),
      R => rst180_r_155,
      C => clk180,
      Q => config_reg1_1_Q
    );
  config_reg1_4 : FDR
    port map (
      D => config_register1(4),
      R => rst180_r_155,
      C => clk180,
      Q => config_reg1_4_Q
    );
  config_reg1_5 : FDR
    port map (
      D => config_register1(5),
      R => rst180_r_155,
      C => clk180,
      Q => config_reg1_5_Q
    );
  config_reg1_12 : FDR
    port map (
      D => config_register1(12),
      R => rst180_r_155,
      C => clk180,
      Q => config_reg1_12_Q
    );
  config_reg2_2 : FDR
    port map (
      D => config_register2(2),
      R => rst180_r_155,
      C => clk180,
      Q => config_reg2_2_Q
    );
  config_reg2_7 : FDR
    port map (
      D => config_register2(7),
      R => rst180_r_155,
      C => clk180,
      Q => config_reg2_7_Q
    );
  config_reg2_8 : FDR
    port map (
      D => config_register2(8),
      R => rst180_r_155,
      C => clk180,
      Q => config_reg2_8_Q
    );
  config_reg2_9 : FDR
    port map (
      D => config_register2(9),
      R => rst180_r_155,
      C => clk180,
      Q => config_reg2_9_Q
    );
  rst_dqs_div_r : FDRSE
    port map (
      D => NlwRenamedSig_OI_high(1),
      R => rst180_r_155,
      S => Q_n0241,
      CE => Q_n0242,
      C => clk180,
      Q => rst_dqs_div_r_152
    );
  INIT_MEMORY : FDRSE
    port map (
      D => NlwRenamedSig_OI_high(1),
      R => rst180_r_155,
      S => initialize_memory,
      CE => Q_n0244,
      C => clk180,
      Q => INIT_MEMORY_133
    );
  CONFLICT : FDR
    port map (
      D => CONFLICT_value,
      R => rst180_r_155,
      C => clk180,
      Q => CONFLICT_106
    );
  wrburst_end_2 : FDR
    port map (
      D => wrburst_end_1_157,
      R => rst180_r_155,
      C => clk180,
      Q => wrburst_end_2_159
    );
  DQS_enable_int : FDR
    port map (
      D => DQS_enable_out,
      R => rst180_r_155,
      C => clk180,
      Q => DQS_enable_int_122
    );
  address_reg_2 : FDR
    port map (
      D => address(2),
      R => rst180_r_155,
      C => clk180,
      Q => address_reg(2)
    );
  address_reg_3 : FDR
    port map (
      D => address(3),
      R => rst180_r_155,
      C => clk180,
      Q => address_reg(3)
    );
  address_reg_4 : FDR
    port map (
      D => address(4),
      R => rst180_r_155,
      C => clk180,
      Q => address_reg(4)
    );
  address_reg_5 : FDR
    port map (
      D => address(5),
      R => rst180_r_155,
      C => clk180,
      Q => address_reg(5)
    );
  address_reg_6 : FDR
    port map (
      D => address(6),
      R => rst180_r_155,
      C => clk180,
      Q => address_reg(6)
    );
  address_reg_7 : FDR
    port map (
      D => address(7),
      R => rst180_r_155,
      C => clk180,
      Q => address_reg(7)
    );
  address_reg_8 : FDR
    port map (
      D => address(8),
      R => rst180_r_155,
      C => clk180,
      Q => address_reg(8)
    );
  address_reg_9 : FDR
    port map (
      D => address(9),
      R => rst180_r_155,
      C => clk180,
      Q => address_reg(9)
    );
  GO_TO_ODT_ON : FDR
    port map (
      D => GO_TO_ODT_ON_value,
      R => rst180_r_155,
      C => clk180,
      Q => GO_TO_ODT_ON_110
    );
  rdburst_end_2 : FDR
    port map (
      D => rdburst_end_1_162,
      R => rst180_r_155,
      C => clk180,
      Q => rdburst_end_2_163
    );
  AUTOREF_COUNT_0 : FDR
    port map (
      D => AUTOREF_CNT_val(0),
      R => rst180_r_155,
      C => clk180,
      Q => AUTOREF_COUNT(0)
    );
  AUTOREF_COUNT_1 : FDR
    port map (
      D => AUTOREF_CNT_val(1),
      R => rst180_r_155,
      C => clk180,
      Q => AUTOREF_COUNT(1)
    );
  AUTOREF_COUNT_2 : FDR
    port map (
      D => AUTOREF_CNT_val(2),
      R => rst180_r_155,
      C => clk180,
      Q => AUTOREF_COUNT(2)
    );
  AUTOREF_COUNT_3 : FDR
    port map (
      D => AUTOREF_CNT_val(3),
      R => rst180_r_155,
      C => clk180,
      Q => AUTOREF_COUNT(3)
    );
  AUTOREF_COUNT_4 : FDR
    port map (
      D => AUTOREF_CNT_val(4),
      R => rst180_r_155,
      C => clk180,
      Q => AUTOREF_COUNT(4)
    );
  AUTOREF_COUNT_5 : FDR
    port map (
      D => AUTOREF_CNT_val(5),
      R => rst180_r_155,
      C => clk180,
      Q => AUTOREF_COUNT(5)
    );
  AUTOREF_COUNT_6 : FDR
    port map (
      D => AUTOREF_CNT_val(6),
      R => rst180_r_155,
      C => clk180,
      Q => AUTOREF_COUNT(6)
    );
  AUTOREF_COUNT_7 : FDR
    port map (
      D => AUTOREF_CNT_val(7),
      R => rst180_r_155,
      C => clk180,
      Q => AUTOREF_COUNT(7)
    );
  AUTOREF_COUNT_8 : FDR
    port map (
      D => AUTOREF_CNT_val(8),
      R => rst180_r_155,
      C => clk180,
      Q => AUTOREF_COUNT(8)
    );
  AUTOREF_COUNT_9 : FDR
    port map (
      D => AUTOREF_CNT_val(9),
      R => rst180_r_155,
      C => clk180,
      Q => AUTOREF_COUNT(9)
    );
  AUTOREF_COUNT_10 : FDR
    port map (
      D => AUTOREF_CNT_val(10),
      R => rst180_r_155,
      C => clk180,
      Q => AUTOREF_COUNT(10)
    );
  AUTOREF_COUNT_11 : FDR
    port map (
      D => AUTOREF_CNT_val(11),
      R => rst180_r_155,
      C => clk180,
      Q => AUTOREF_COUNT(11)
    );
  INIT_DONE : FDR
    port map (
      D => INIT_DONE_value,
      R => rst180_r_155,
      C => clk180,
      Q => NlwRenamedSig_OI_init
    );
  RFC_COUNT_reg : FDRSE
    port map (
      D => NlwRenamedSig_OI_high(1),
      R => rst180_r_155,
      S => AR_Done_reg_124,
      CE => Auto_Ref_issued_p,
      C => clk180,
      Q => RFC_COUNT_reg_116
    );
  read_cmd2 : FDR
    port map (
      D => read_cmd1_111,
      R => rst180_r_155,
      C => clk180,
      Q => read_cmd2_112
    );
  write_cmd2 : FDR
    port map (
      D => write_cmd1_125,
      R => rst180_r_155,
      C => clk180,
      Q => write_cmd2_126
    );
  auto_ref_wait2 : FDR
    port map (
      D => auto_ref_wait1_119,
      R => Q_n0235,
      C => clk180,
      Q => auto_ref_wait2_120
    );
  wrburst_end_3 : FDR
    port map (
      D => wrburst_end_2_159,
      R => rst180_r_155,
      C => clk180,
      Q => wrburst_end_3_160
    );
  cas_latency_0 : FDR
    port map (
      D => config_reg1_4_Q,
      R => rst180_r_155,
      C => clk180,
      Q => cas_latency(0)
    );
  cas_latency_1 : FDR
    port map (
      D => config_reg1_5_Q,
      R => rst180_r_155,
      C => clk180,
      Q => cas_latency(1)
    );
  burst_length_1 : FDR
    port map (
      D => config_reg1_1_Q,
      R => rst180_r_155,
      C => clk180,
      Q => burst_length(1)
    );
  DLL_RST_COUNT_0 : FDR
    port map (
      D => DLL_RST_COUNT_value(0),
      R => rst180_r_155,
      C => clk180,
      Q => DLL_RST_COUNT(0)
    );
  DLL_RST_COUNT_1 : FDR
    port map (
      D => DLL_RST_COUNT_value(1),
      R => rst180_r_155,
      C => clk180,
      Q => DLL_RST_COUNT(1)
    );
  DLL_RST_COUNT_2 : FDR
    port map (
      D => DLL_RST_COUNT_value(2),
      R => rst180_r_155,
      C => clk180,
      Q => DLL_RST_COUNT(2)
    );
  DLL_RST_COUNT_3 : FDR
    port map (
      D => DLL_RST_COUNT_value(3),
      R => rst180_r_155,
      C => clk180,
      Q => DLL_RST_COUNT(3)
    );
  DLL_RST_COUNT_4 : FDR
    port map (
      D => DLL_RST_COUNT_value(4),
      R => rst180_r_155,
      C => clk180,
      Q => DLL_RST_COUNT(4)
    );
  DLL_RST_COUNT_5 : FDR
    port map (
      D => DLL_RST_COUNT_value(5),
      R => rst180_r_155,
      C => clk180,
      Q => DLL_RST_COUNT(5)
    );
  DLL_RST_COUNT_6 : FDR
    port map (
      D => DLL_RST_COUNT_value(6),
      R => rst180_r_155,
      C => clk180,
      Q => DLL_RST_COUNT(6)
    );
  DLL_RST_COUNT_7 : FDR
    port map (
      D => DLL_RST_COUNT_value(7),
      R => rst180_r_155,
      C => clk180,
      Q => DLL_RST_COUNT(7)
    );
  EMR_2 : FDR
    port map (
      D => config_reg2_2_Q,
      R => rst180_r_155,
      C => clk180,
      Q => EMR_2_Q
    );
  EMR_7 : FDR
    port map (
      D => config_reg2_7_Q,
      R => rst180_r_155,
      C => clk180,
      Q => EMR_7_Q
    );
  EMR_8 : FDR
    port map (
      D => config_reg2_8_Q,
      R => rst180_r_155,
      C => clk180,
      Q => EMR_8_Q
    );
  EMR_9 : FDR
    port map (
      D => config_reg2_9_Q,
      R => rst180_r_155,
      C => clk180,
      Q => EMR_9_Q
    );
  DQS_enable1 : FDR
    port map (
      D => DQS_enable_int_122,
      R => rst0_r_154,
      C => clk,
      Q => DQS_enable1_103
    );
  LMR_DLL_set_10 : FDR
    port map (
      D => config_reg1_12_Q,
      R => rst180_r_155,
      C => clk180,
      Q => LMR_DLL_set(10)
    );
  wrburst_end_cnt_0 : FDR
    port map (
      D => wrburst_end_cnt_value(0),
      R => rst180_r_155,
      C => clk180,
      Q => wrburst_end_cnt(0)
    );
  wrburst_end_cnt_1 : FDR
    port map (
      D => wrburst_end_cnt_value(1),
      R => rst180_r_155,
      C => clk180,
      Q => wrburst_end_cnt(1)
    );
  AUTO_REF_detect : FDR
    port map (
      D => AUTO_REF_detect1_109,
      R => rst180_r_155,
      C => clk180,
      Q => AUTO_REF_detect_105
    );
  LMR_DLL_rst_8 : FDR
    port map (
      D => low,
      R => rst180_r_155,
      C => clk180,
      Q => LMR_DLL_rst_8_Q
    );
  LMR_DLL_rst_10 : FDR
    port map (
      D => config_reg1_12_Q,
      R => rst180_r_155,
      C => clk180,
      Q => LMR_DLL_rst_10_Q
    );
  COLUMN_ADDRESS_reg_2 : FDR
    port map (
      D => address_reg(2),
      R => rst180_r_155,
      C => clk180,
      Q => COLUMN_ADDRESS_reg(2)
    );
  COLUMN_ADDRESS_reg_3 : FDR
    port map (
      D => address_reg(3),
      R => rst180_r_155,
      C => clk180,
      Q => COLUMN_ADDRESS_reg(3)
    );
  COLUMN_ADDRESS_reg_4 : FDR
    port map (
      D => address_reg(4),
      R => rst180_r_155,
      C => clk180,
      Q => COLUMN_ADDRESS_reg(4)
    );
  COLUMN_ADDRESS_reg_5 : FDR
    port map (
      D => address_reg(5),
      R => rst180_r_155,
      C => clk180,
      Q => COLUMN_ADDRESS_reg(5)
    );
  COLUMN_ADDRESS_reg_6 : FDR
    port map (
      D => address_reg(6),
      R => rst180_r_155,
      C => clk180,
      Q => COLUMN_ADDRESS_reg(6)
    );
  COLUMN_ADDRESS_reg_7 : FDR
    port map (
      D => address_reg(7),
      R => rst180_r_155,
      C => clk180,
      Q => COLUMN_ADDRESS_reg(7)
    );
  COLUMN_ADDRESS_reg_8 : FDR
    port map (
      D => address_reg(8),
      R => rst180_r_155,
      C => clk180,
      Q => COLUMN_ADDRESS_reg(8)
    );
  COLUMN_ADDRESS_reg_9 : FDR
    port map (
      D => address_reg(9),
      R => rst180_r_155,
      C => clk180,
      Q => COLUMN_ADDRESS_reg(9)
    );
  read_cmd3 : FDR
    port map (
      D => read_cmd2_112,
      R => rst180_r_155,
      C => clk180,
      Q => read_cmd3_113
    );
  write_cmd3 : FDR
    port map (
      D => write_cmd2_126,
      R => rst180_r_155,
      C => clk180,
      Q => write_cmd3_127
    );
  auto_ref : FDR
    port map (
      D => auto_ref_wait2_120,
      R => Q_n0235,
      C => clk180,
      Q => auto_ref_161
    );
  AUTO_REF_pulse_end : FDR
    port map (
      D => AUTO_REF_detect_105,
      R => rst180_r_155,
      C => clk180,
      Q => AUTO_REF_pulse_end_156
    );
  DQS_enable2 : FDR
    port map (
      D => DQS_enable1_103,
      R => rst0_r_154,
      C => clk,
      Q => DQS_enable2_104
    );
  ODT_COUNT_0 : FDR
    port map (
      D => ODT_COUNT_value(0),
      R => rst180_r_155,
      C => clk180,
      Q => ODT_COUNT(0)
    );
  ODT_COUNT_1 : FDR
    port map (
      D => ODT_COUNT_value(1),
      R => rst180_r_155,
      C => clk180,
      Q => ODT_COUNT(1)
    );
  read_cmd4 : FDR
    port map (
      D => read_cmd3_113,
      R => rst180_r_155,
      C => clk180,
      Q => read_cmd4_114
    );
  write_cmd4 : FDR
    port map (
      D => write_cmd3_127,
      R => rst180_r_155,
      C => clk180,
      Q => write_cmd4_128
    );
  read_cmd5 : FDR
    port map (
      D => read_cmd4_114,
      R => rst180_r_155,
      C => clk180,
      Q => read_cmd5_115
    );
  write_cmd5 : FDR
    port map (
      D => write_cmd4_128,
      R => rst180_r_155,
      C => clk180,
      Q => write_cmd5_129
    );
  write_cmd6 : FDR
    port map (
      D => write_cmd5_129,
      R => rst180_r_155,
      C => clk180,
      Q => write_cmd6_130
    );
  Auto_Ref_issued : FDR
    generic map(
      INIT => '0'
    )
    port map (
      D => Auto_Ref_issued_p,
      R => rst180_r_155,
      C => clk180,
      Q => Auto_Ref_issued_107
    );
  DQS_reset_int : FDR
    generic map(
      INIT => '0'
    )
    port map (
      D => DQS_reset_out,
      R => rst180_r_155,
      C => clk180,
      Q => DQS_reset_int_117
    );
  write_cmd7 : FDR
    port map (
      D => write_cmd6_130,
      R => rst180_r_155,
      C => clk180,
      Q => write_cmd7_131
    );
  RCDR_COUNT_0 : FDR
    port map (
      D => RCDR_COUNT_value(0),
      R => rst180_r_155,
      C => clk180,
      Q => RCDR_COUNT(0)
    );
  RCDR_COUNT_1 : FDR
    port map (
      D => RCDR_COUNT_value(1),
      R => rst180_r_155,
      C => clk180,
      Q => RCDR_COUNT(1)
    );
  RCDR_COUNT_2 : FDR
    port map (
      D => RCDR_COUNT_value(2),
      R => rst180_r_155,
      C => clk180,
      Q => RCDR_COUNT(2)
    );
  RFC_COUNT_0 : FDR
    port map (
      D => RFC_COUNT_value(0),
      R => rst180_r_155,
      C => clk180,
      Q => RFC_COUNT(0)
    );
  RFC_COUNT_1 : FDR
    port map (
      D => RFC_COUNT_value(1),
      R => rst180_r_155,
      C => clk180,
      Q => RFC_COUNT(1)
    );
  RFC_COUNT_2 : FDR
    port map (
      D => RFC_COUNT_value(2),
      R => rst180_r_155,
      C => clk180,
      Q => RFC_COUNT(2)
    );
  RFC_COUNT_3 : FDR
    port map (
      D => RFC_COUNT_value(3),
      R => rst180_r_155,
      C => clk180,
      Q => RFC_COUNT(3)
    );
  RFC_COUNT_4 : FDR
    port map (
      D => RFC_COUNT_value(4),
      R => rst180_r_155,
      C => clk180,
      Q => RFC_COUNT(4)
    );
  RFC_COUNT_5 : FDR
    port map (
      D => RFC_COUNT_value(5),
      R => rst180_r_155,
      C => clk180,
      Q => RFC_COUNT(5)
    );
  write_cmd8 : FDR
    port map (
      D => write_cmd7_131,
      R => rst180_r_155,
      C => clk180,
      Q => write_cmd8_132
    );
  DQS_reset1_clk0 : FDR
    port map (
      D => DQS_reset_int_117,
      R => rst0_r_154,
      C => clk,
      Q => DQS_reset1_clk0_118
    );
  RRD_COUNT_0 : FDR
    port map (
      D => RRD_COUNT_value(0),
      R => rst180_r_155,
      C => clk180,
      Q => RRD_COUNT(0)
    );
  RRD_COUNT_1 : FDR
    port map (
      D => RRD_COUNT_value(1),
      R => rst180_r_155,
      C => clk180,
      Q => RRD_COUNT(1)
    );
  MRD_COUNT_0 : FDR
    port map (
      D => MRD_COUNT_value(0),
      R => rst180_r_155,
      C => clk180,
      Q => MRD_COUNT(0)
    );
  MRD_COUNT_1 : FDR
    port map (
      D => MRD_COUNT_value(1),
      R => rst180_r_155,
      C => clk180,
      Q => MRD_COUNT(1)
    );
  auto_ref_wait : FDRSE
    port map (
      D => NlwRenamedSig_OI_high(1),
      R => rst180_r_155,
      S => Q_n0243,
      CE => Auto_Ref_issued_107,
      C => clk180,
      Q => NlwRenamedSig_OI_auto_ref_req
    );
  RP_COUNT_0 : FDR
    port map (
      D => RP_cnt_value(0),
      R => rst180_r_155,
      C => clk180,
      Q => RP_COUNT(0)
    );
  RP_COUNT_1 : FDR
    port map (
      D => RP_cnt_value(1),
      R => rst180_r_155,
      C => clk180,
      Q => RP_COUNT(1)
    );
  RP_COUNT_2 : FDR
    port map (
      D => RP_cnt_value(2),
      R => rst180_r_155,
      C => clk180,
      Q => RP_COUNT(2)
    );
  RCDW_COUNT_0 : FDR
    port map (
      D => RCDW_COUNT_value(0),
      R => rst180_r_155,
      C => clk180,
      Q => RCDW_COUNT(0)
    );
  RCDW_COUNT_1 : FDR
    port map (
      D => RCDW_COUNT_value(1),
      R => rst180_r_155,
      C => clk180,
      Q => RCDW_COUNT(1)
    );
  RCDW_COUNT_2 : FDR
    port map (
      D => RCDW_COUNT_value(2),
      R => rst180_r_155,
      C => clk180,
      Q => RCDW_COUNT(2)
    );
  CAS_COUNT_0 : FDR
    port map (
      D => cas_count_value(0),
      R => rst180_r_155,
      C => clk180,
      Q => CAS_COUNT(0)
    );
  CAS_COUNT_1 : FDR
    port map (
      D => cas_count_value(1),
      R => rst180_r_155,
      C => clk180,
      Q => CAS_COUNT(1)
    );
  DQS_reset2_clk0 : FDR
    port map (
      D => DQS_reset1_clk0_118,
      R => rst0_r_154,
      C => clk,
      Q => DQS_reset2_clk0_121
    );
  ddr_ODT2 : FDR
    port map (
      D => write_cmd8_132,
      R => rst180_r_155,
      C => clk180,
      Q => ddr_ODT2_108
    );
  ddr_ODT_cntrl_102 : FDR
    port map (
      D => ddr_ODT2_108,
      R => rst180_r_155,
      C => clk180,
      Q => ddr_ODT_cntrl
    );
  ACK_REG_INST1 : FD
    port map (
      D => ack_o,
      C => clk180,
      Q => cmd_ack
    );
  rst_calib0 : FD
    port map (
      D => rst_dqs_div_r_152,
      C => clk180,
      Q => rst_calib
    );
  rst_iob_out : FD
    port map (
      D => rst_dqs_div_r_152,
      C => clk180,
      Q => rst_dqs_div_int
    );
  controller_16bit_00_n0254_0_lut : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => AUTOREF_COUNT(0),
      O => N5
    );
  controller_16bit_00_n0254_0_cy : MUXCY
    port map (
      CI => NlwRenamedSig_OI_high(1),
      DI => low,
      S => N5,
      O => controller_16bit_00_n0254_0_cyo
    );
  controller_16bit_00_n0254_1_cy : MUXCY
    port map (
      CI => controller_16bit_00_n0254_0_cyo,
      DI => NlwRenamedSig_OI_high(1),
      S => AUTOREF_COUNT_1_rt_165,
      O => controller_16bit_00_n0254_1_cyo
    );
  controller_16bit_00_n0254_1_xor : XORCY
    port map (
      CI => controller_16bit_00_n0254_0_cyo,
      LI => AUTOREF_COUNT_1_rt_165,
      O => Q_n0254(1)
    );
  controller_16bit_00_n0254_2_cy : MUXCY
    port map (
      CI => controller_16bit_00_n0254_1_cyo,
      DI => NlwRenamedSig_OI_high(1),
      S => AUTOREF_COUNT_2_rt_166,
      O => controller_16bit_00_n0254_2_cyo
    );
  controller_16bit_00_n0254_2_xor : XORCY
    port map (
      CI => controller_16bit_00_n0254_1_cyo,
      LI => AUTOREF_COUNT_2_rt_166,
      O => Q_n0254(2)
    );
  controller_16bit_00_n0254_3_cy : MUXCY
    port map (
      CI => controller_16bit_00_n0254_2_cyo,
      DI => NlwRenamedSig_OI_high(1),
      S => AUTOREF_COUNT_3_rt_167,
      O => controller_16bit_00_n0254_3_cyo
    );
  controller_16bit_00_n0254_3_xor : XORCY
    port map (
      CI => controller_16bit_00_n0254_2_cyo,
      LI => AUTOREF_COUNT_3_rt_167,
      O => Q_n0254(3)
    );
  controller_16bit_00_n0254_4_cy : MUXCY
    port map (
      CI => controller_16bit_00_n0254_3_cyo,
      DI => NlwRenamedSig_OI_high(1),
      S => AUTOREF_COUNT_4_rt_168,
      O => controller_16bit_00_n0254_4_cyo
    );
  controller_16bit_00_n0254_4_xor : XORCY
    port map (
      CI => controller_16bit_00_n0254_3_cyo,
      LI => AUTOREF_COUNT_4_rt_168,
      O => Q_n0254(4)
    );
  controller_16bit_00_n0254_5_cy : MUXCY
    port map (
      CI => controller_16bit_00_n0254_4_cyo,
      DI => NlwRenamedSig_OI_high(1),
      S => AUTOREF_COUNT_5_rt_169,
      O => controller_16bit_00_n0254_5_cyo
    );
  controller_16bit_00_n0254_5_xor : XORCY
    port map (
      CI => controller_16bit_00_n0254_4_cyo,
      LI => AUTOREF_COUNT_5_rt_169,
      O => Q_n0254(5)
    );
  controller_16bit_00_n0254_6_cy : MUXCY
    port map (
      CI => controller_16bit_00_n0254_5_cyo,
      DI => NlwRenamedSig_OI_high(1),
      S => AUTOREF_COUNT_6_rt_170,
      O => controller_16bit_00_n0254_6_cyo
    );
  controller_16bit_00_n0254_6_xor : XORCY
    port map (
      CI => controller_16bit_00_n0254_5_cyo,
      LI => AUTOREF_COUNT_6_rt_170,
      O => Q_n0254(6)
    );
  controller_16bit_00_n0254_7_cy : MUXCY
    port map (
      CI => controller_16bit_00_n0254_6_cyo,
      DI => NlwRenamedSig_OI_high(1),
      S => AUTOREF_COUNT_7_rt_171,
      O => controller_16bit_00_n0254_7_cyo
    );
  controller_16bit_00_n0254_7_xor : XORCY
    port map (
      CI => controller_16bit_00_n0254_6_cyo,
      LI => AUTOREF_COUNT_7_rt_171,
      O => Q_n0254(7)
    );
  controller_16bit_00_n0254_8_cy : MUXCY
    port map (
      CI => controller_16bit_00_n0254_7_cyo,
      DI => NlwRenamedSig_OI_high(1),
      S => AUTOREF_COUNT_8_rt_172,
      O => controller_16bit_00_n0254_8_cyo
    );
  controller_16bit_00_n0254_8_xor : XORCY
    port map (
      CI => controller_16bit_00_n0254_7_cyo,
      LI => AUTOREF_COUNT_8_rt_172,
      O => Q_n0254(8)
    );
  controller_16bit_00_n0254_9_cy : MUXCY
    port map (
      CI => controller_16bit_00_n0254_8_cyo,
      DI => NlwRenamedSig_OI_high(1),
      S => AUTOREF_COUNT_9_rt_173,
      O => controller_16bit_00_n0254_9_cyo
    );
  controller_16bit_00_n0254_9_xor : XORCY
    port map (
      CI => controller_16bit_00_n0254_8_cyo,
      LI => AUTOREF_COUNT_9_rt_173,
      O => Q_n0254(9)
    );
  controller_16bit_00_n0254_10_cy : MUXCY
    port map (
      CI => controller_16bit_00_n0254_9_cyo,
      DI => NlwRenamedSig_OI_high(1),
      S => AUTOREF_COUNT_10_rt_174,
      O => controller_16bit_00_n0254_10_cyo
    );
  controller_16bit_00_n0254_10_xor : XORCY
    port map (
      CI => controller_16bit_00_n0254_9_cyo,
      LI => AUTOREF_COUNT_10_rt_174,
      O => Q_n0254(10)
    );
  controller_16bit_00_n0254_11_xor : XORCY
    port map (
      CI => controller_16bit_00_n0254_10_cyo,
      LI => AUTOREF_COUNT_11_rt_175,
      O => Q_n0254(11)
    );
  current_state_FFd1 : FDR
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state_FFd1_In,
      R => rst180_r_155,
      C => clk180,
      Q => current_state_FFd1_147
    );
  current_state_FFd2 : FDR
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state_FFd2_In,
      R => rst180_r_155,
      C => clk180,
      Q => current_state_FFd2_146
    );
  current_state_FFd3 : FDR
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state_FFd3_In,
      R => rst180_r_155,
      C => clk180,
      Q => current_state_FFd3_143
    );
  current_state_FFd4 : FDR
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state_FFd4_In,
      R => rst180_r_155,
      C => clk180,
      Q => current_state_FFd4_142
    );
  current_state_FFd7 : FDR
    generic map(
      INIT => '0'
    )
    port map (
      D => DQS_reset_out,
      R => rst180_r_155,
      C => clk180,
      Q => current_state_FFd7_135
    );
  current_state_FFd5 : FDR
    generic map(
      INIT => '0'
    )
    port map (
      D => N73,
      R => rst180_r_155,
      C => clk180,
      Q => current_state_FFd5_134
    );
  current_state_FFd6 : FDR
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0217,
      R => rst180_r_155,
      C => clk180,
      Q => current_state_FFd6_137
    );
  current_state_FFd8 : FDR
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state_FFd8_In_164,
      R => rst180_r_155,
      C => clk180,
      Q => current_state_FFd8_141
    );
  current_state_FFd9 : FDR
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0200,
      R => rst180_r_155,
      C => clk180,
      Q => current_state_FFd9_139
    );
  current_state_FFd10 : FDR
    generic map(
      INIT => '0'
    )
    port map (
      D => current_state_FFd10_In,
      R => rst180_r_155,
      C => clk180,
      Q => current_state_FFd10_145
    );
  current_state_FFd11 : FDR
    generic map(
      INIT => '0'
    )
    port map (
      D => Auto_Ref_issued_p,
      R => rst180_r_155,
      C => clk180,
      Q => current_state_FFd11_140
    );
  current_state_FFd14 : FDS
    generic map(
      INIT => '1'
    )
    port map (
      D => current_state_FFd14_In,
      S => rst180_r_155,
      C => clk180,
      Q => current_state_FFd14_144
    );
  current_state_FFd12 : FDR
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0213,
      R => rst180_r_155,
      C => clk180,
      Q => current_state_FFd12_136
    );
  current_state_FFd13 : FDR
    generic map(
      INIT => '0'
    )
    port map (
      D => Q_n0215,
      R => rst180_r_155,
      C => clk180,
      Q => current_state_FFd13_138
    );
  dqs_div_rdburstcount_2 : FDRE
    port map (
      D => dqs_div_rdburstcount_n0000(2),
      R => dqs_div_rdburstcount_0_n0000,
      CE => Q_n0385,
      C => clk180,
      Q => dqs_div_rdburstcount(2)
    );
  dqs_div_rdburstcount_0 : FDRE
    port map (
      D => dqs_div_rdburstcount_n0000(0),
      R => dqs_div_rdburstcount_0_n0000,
      CE => Q_n0385,
      C => clk180,
      Q => dqs_div_rdburstcount(0)
    );
  dqs_div_rdburstcount_1 : FDRSE
    port map (
      D => dqs_div_rdburstcount_n0000(1),
      R => rst180_r_155,
      S => Q_n0248,
      CE => Q_n0385,
      C => clk180,
      Q => dqs_div_rdburstcount(1)
    );
  WR_COUNT_2 : FDRSE
    port map (
      D => WR_COUNT_n0000(2),
      R => rst180_r_155,
      S => NlwRenamedSig_OI_dqs_enable,
      CE => Q_n0380,
      C => clk180,
      Q => WR_COUNT(2)
    );
  WR_COUNT_0 : FDRE
    port map (
      D => WR_COUNT_n0000(0),
      R => WR_COUNT_0_n0000,
      CE => Q_n0380,
      C => clk180,
      Q => WR_COUNT(0)
    );
  WR_COUNT_1 : FDRE
    port map (
      D => WR_COUNT_n0000(1),
      R => WR_COUNT_0_n0000,
      CE => Q_n0380,
      C => clk180,
      Q => WR_COUNT(1)
    );
  dqs_div_cascount_2 : FDRE
    port map (
      D => dqs_div_cascount_n0001(2),
      R => rst180_r_155,
      CE => Q_n0378,
      C => clk180,
      Q => dqs_div_cascount(2)
    );
  dqs_div_cascount_0 : FDRE
    port map (
      D => dqs_div_cascount_n0001(0),
      R => rst180_r_155,
      CE => Q_n0378,
      C => clk180,
      Q => dqs_div_cascount(0)
    );
  dqs_div_cascount_1 : FDRSE
    port map (
      D => dqs_div_cascount_n0001(1),
      R => rst180_r_155,
      S => NlwRenamedSig_OI_high(1),
      CE => Q_n0378,
      C => clk180,
      Q => dqs_div_cascount(1)
    );
  INIT_PRE_COUNT_0 : FDSE
    port map (
      D => INIT_PRE_COUNT_n0000(0),
      S => rst180_r_155,
      CE => INIT_PRE_COUNT_en,
      C => clk180,
      Q => INIT_PRE_COUNT(0)
    );
  INIT_PRE_COUNT_1 : FDSE
    port map (
      D => INIT_PRE_COUNT_n0000(1),
      S => rst180_r_155,
      CE => INIT_PRE_COUNT_en,
      C => clk180,
      Q => INIT_PRE_COUNT(1)
    );
  INIT_PRE_COUNT_2 : FDRE
    port map (
      D => INIT_PRE_COUNT_n0000(2),
      R => rst180_r_155,
      CE => INIT_PRE_COUNT_en,
      C => clk180,
      Q => INIT_PRE_COUNT(2)
    );
  INIT_PRE_COUNT_3 : FDSE
    port map (
      D => INIT_PRE_COUNT_n0000(3),
      S => rst180_r_155,
      CE => INIT_PRE_COUNT_en,
      C => clk180,
      Q => INIT_PRE_COUNT(3)
    );
  INIT_PRE_COUNT_6 : FDSE
    port map (
      D => INIT_PRE_COUNT_n0000(6),
      S => rst180_r_155,
      CE => INIT_PRE_COUNT_en,
      C => clk180,
      Q => INIT_PRE_COUNT(6)
    );
  INIT_PRE_COUNT_4 : FDRE
    port map (
      D => INIT_PRE_COUNT_n0000(4),
      R => rst180_r_155,
      CE => INIT_PRE_COUNT_en,
      C => clk180,
      Q => INIT_PRE_COUNT(4)
    );
  INIT_PRE_COUNT_5 : FDSE
    port map (
      D => INIT_PRE_COUNT_n0000(5),
      S => rst180_r_155,
      CE => INIT_PRE_COUNT_en,
      C => clk180,
      Q => INIT_PRE_COUNT(5)
    );
  INIT_COUNT_0 : FDRE
    port map (
      D => INIT_COUNT_n0000(0),
      R => rst180_r_155,
      CE => Q_n0381_153,
      C => clk180,
      Q => INIT_COUNT(0)
    );
  INIT_COUNT_1 : FDRE
    port map (
      D => INIT_COUNT_n0000(1),
      R => rst180_r_155,
      CE => Q_n0381_153,
      C => clk180,
      Q => INIT_COUNT(1)
    );
  INIT_COUNT_2 : FDRE
    port map (
      D => INIT_COUNT_n0000(2),
      R => rst180_r_155,
      CE => Q_n0381_153,
      C => clk180,
      Q => INIT_COUNT(2)
    );
  INIT_COUNT_3 : FDRE
    port map (
      D => INIT_COUNT_n0000(3),
      R => rst180_r_155,
      CE => Q_n0381_153,
      C => clk180,
      Q => INIT_COUNT(3)
    );
  ddr_cke_cntrl1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => wait_200us,
      O => ddr_cke_cntrl
    );
  Q_n00311 : LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      I0 => Auto_Ref_issued_107,
      I1 => NlwRenamedSig_OI_auto_ref_req,
      O => Q_n0031
    );
  Q_n02351 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => Auto_Ref_issued_107,
      I1 => rst180_r_155,
      O => Q_n0235
    );
  Q_n02421 : LUT4
    generic map(
      INIT => X"FFFD"
    )
    port map (
      I0 => dqs_div_rdburstcount(0),
      I1 => dqs_div_cascount(2),
      I2 => dqs_div_rdburstcount(1),
      I3 => dqs_div_rdburstcount(2),
      O => N10
    );
  Q_n02422 : LUT3
    generic map(
      INIT => X"01"
    )
    port map (
      I0 => dqs_div_cascount(0),
      I1 => dqs_div_cascount(1),
      I2 => N10,
      O => Q_n0242
    );
  Q_n02431 : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => AUTO_REF_detect_105,
      I1 => AUTO_REF_pulse_end_156,
      I2 => NlwRenamedSig_OI_auto_ref_req,
      O => Q_n0243
    );
  Q_n02441 : LUT4
    generic map(
      INIT => X"FFFD"
    )
    port map (
      I0 => INIT_COUNT(3),
      I1 => INIT_COUNT(2),
      I2 => MRD_COUNT(0),
      I3 => MRD_COUNT(1),
      O => N13
    );
  Q_n02442 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => INIT_COUNT(1),
      I1 => N13,
      I2 => INIT_COUNT(0),
      O => Q_n0244
    );
  Q_n02471 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => RFC_COUNT(3),
      I1 => RFC_COUNT(4),
      I2 => RFC_COUNT(5),
      I3 => rst180_r_155,
      O => N15
    );
  Q_n02472 : LUT4
    generic map(
      INIT => X"FFFD"
    )
    port map (
      I0 => RFC_COUNT(1),
      I1 => RFC_COUNT(0),
      I2 => RFC_COUNT(2),
      I3 => N15,
      O => Q_n0247
    );
  Q_n02851 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => INIT_PRE_COUNT(3),
      I1 => INIT_PRE_COUNT(4),
      I2 => INIT_PRE_COUNT(5),
      I3 => INIT_PRE_COUNT(6),
      O => N19
    );
  Q_n02852 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => INIT_MEMORY_133,
      I1 => INIT_PRE_COUNT(1),
      I2 => INIT_PRE_COUNT(2),
      I3 => N19,
      O => INIT_PRE_COUNT_en
    );
  Q_n02921 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => RRD_COUNT(0),
      I1 => RRD_COUNT(1),
      I2 => BA_address_conflict_123,
      O => CONFLICT_value
    );
  Q_n02971 : LUT4
    generic map(
      INIT => X"0010"
    )
    port map (
      I0 => command_register(3),
      I1 => command_register(0),
      I2 => command_register(1),
      I3 => command_register(2),
      O => initialize_memory
    );
  Q_n03801 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => WR_COUNT(0),
      I1 => WR_COUNT(1),
      I2 => WR_COUNT(2),
      O => Q_n0380
    );
  Q_n03851 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => dqs_div_rdburstcount(0),
      I1 => dqs_div_rdburstcount(1),
      I2 => dqs_div_rdburstcount(2),
      O => Q_n0385
    );
  ODT_COUNT_value_0_1 : LUT3
    generic map(
      INIT => X"F4"
    )
    port map (
      I0 => ODT_COUNT(0),
      I1 => ODT_COUNT(1),
      I2 => GO_TO_ODT_ON_110,
      O => ODT_COUNT_value(0)
    );
  current_state_FFd3_In1 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => current_state_FFd4_142,
      I1 => wrburst_end_1_157,
      I2 => wrburst_end_2_159,
      I3 => wrburst_end_3_160,
      O => N29
    );
  current_state_FFd3_In2 : LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      I0 => current_state_FFd1_147,
      I1 => rdburst_end_1_162,
      I2 => N29,
      I3 => rdburst_end_2_163,
      O => current_state_FFd3_In
    );
  dqs_div_rdburstcount_Msub_n0000_n00081 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => dqs_div_rdburstcount(0),
      O => dqs_div_rdburstcount_n0000(0)
    );
  dqs_div_rdburstcount_Msub_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"A9"
    )
    port map (
      I0 => dqs_div_rdburstcount(2),
      I1 => dqs_div_rdburstcount(0),
      I2 => dqs_div_rdburstcount(1),
      O => dqs_div_rdburstcount_n0000(2)
    );
  dqs_div_rdburstcount_Msub_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => dqs_div_rdburstcount(0),
      I1 => dqs_div_rdburstcount(1),
      O => dqs_div_rdburstcount_n0000(1)
    );
  dqs_div_rdburstcount_n00001 : LUT4
    generic map(
      INIT => X"FF02"
    )
    port map (
      I0 => dqs_div_cascount(0),
      I1 => dqs_div_cascount(1),
      I2 => dqs_div_cascount(2),
      I3 => rst180_r_155,
      O => dqs_div_rdburstcount_0_n0000
    );
  WR_COUNT_Msub_n0000_n00081 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => WR_COUNT(0),
      O => WR_COUNT_n0000(0)
    );
  WR_COUNT_Msub_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"A9"
    )
    port map (
      I0 => WR_COUNT(2),
      I1 => WR_COUNT(0),
      I2 => WR_COUNT(1),
      O => WR_COUNT_n0000(2)
    );
  WR_COUNT_Msub_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => WR_COUNT(0),
      I1 => WR_COUNT(1),
      O => WR_COUNT_n0000(1)
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_0_xor11 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => INIT_PRE_COUNT(0),
      O => INIT_PRE_COUNT_n0000(0)
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_1_xor11 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => INIT_PRE_COUNT(0),
      I1 => INIT_PRE_COUNT(1),
      O => INIT_PRE_COUNT_n0000(1)
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_2_xor11 : LUT3
    generic map(
      INIT => X"A9"
    )
    port map (
      I0 => INIT_PRE_COUNT(2),
      I1 => INIT_PRE_COUNT(0),
      I2 => INIT_PRE_COUNT(1),
      O => INIT_PRE_COUNT_n0000(2)
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_3_xor11 : LUT4
    generic map(
      INIT => X"AAA9"
    )
    port map (
      I0 => INIT_PRE_COUNT(3),
      I1 => INIT_PRE_COUNT(0),
      I2 => INIT_PRE_COUNT(1),
      I3 => INIT_PRE_COUNT(2),
      O => INIT_PRE_COUNT_n0000(3)
    );
  INIT_COUNT_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => INIT_COUNT(0),
      O => INIT_COUNT_n0000(0)
    );
  INIT_COUNT_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => INIT_COUNT(0),
      I1 => INIT_COUNT(3),
      I2 => INIT_COUNT(1),
      I3 => INIT_COUNT(2),
      O => INIT_COUNT_n0000(3)
    );
  INIT_COUNT_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => INIT_COUNT(0),
      I1 => INIT_COUNT(1),
      I2 => INIT_COUNT(2),
      O => INIT_COUNT_n0000(2)
    );
  INIT_COUNT_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => INIT_COUNT(0),
      I1 => INIT_COUNT(1),
      O => INIT_COUNT_n0000(1)
    );
  ODT_COUNT_value_1_1 : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => ODT_COUNT(0),
      I1 => ODT_COUNT(1),
      I2 => GO_TO_ODT_ON_110,
      O => ODT_COUNT_value(1)
    );
  DLL_RST_COUNT_value_1_1 : LUT4
    generic map(
      INIT => X"EFFF"
    )
    port map (
      I0 => INIT_COUNT(3),
      I1 => INIT_COUNT(2),
      I2 => INIT_COUNT(1),
      I3 => INIT_MEMORY_133,
      O => N48
    );
  DLL_RST_COUNT_value_1_2 : LUT4
    generic map(
      INIT => X"9990"
    )
    port map (
      I0 => DLL_RST_COUNT(0),
      I1 => DLL_RST_COUNT(1),
      I2 => INIT_COUNT(0),
      I3 => N48,
      O => DLL_RST_COUNT_value(1)
    );
  dqs_reset1 : MUXF5
    port map (
      I0 => DQS_reset1_clk0_rt_176,
      I1 => N53,
      S => cas_latency(0),
      O => dqs_reset
    );
  dqs_reset1_G : LUT4
    generic map(
      INIT => X"EF40"
    )
    port map (
      I0 => NlwRenamedSig_OI_high(1),
      I1 => DQS_reset2_clk0_121,
      I2 => cas_latency(1),
      I3 => DQS_reset1_clk0_118,
      O => N53
    );
  Q_n03021 : MUXF5
    port map (
      I0 => NlwRenamedSig_OI_high(1),
      I1 => N55,
      S => cas_latency(0),
      O => ack_o
    );
  Q_n03021_G : LUT4
    generic map(
      INIT => X"2220"
    )
    port map (
      I0 => cas_latency(1),
      I1 => NlwRenamedSig_OI_high(1),
      I2 => read_cmd1_111,
      I3 => write_cmd4_128,
      O => N55
    );
  current_state_FFd2_In1 : MUXF5
    port map (
      I0 => current_state_FFd3_rt_177,
      I1 => N57,
      S => current_state_FFd2_146,
      O => current_state_FFd2_In
    );
  current_state_FFd2_In1_G : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => WR_COUNT(0),
      I1 => WR_COUNT(1),
      I2 => WR_COUNT(2),
      I3 => current_state_FFd3_143,
      O => N57
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_4_xor11 : MUXF5
    port map (
      I0 => N58,
      I1 => INIT_PRE_COUNT_4_rt_178,
      S => INIT_PRE_COUNT(1),
      O => INIT_PRE_COUNT_n0000(4)
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_4_xor11_F : LUT4
    generic map(
      INIT => X"AAA9"
    )
    port map (
      I0 => INIT_PRE_COUNT(4),
      I1 => INIT_PRE_COUNT(0),
      I2 => INIT_PRE_COUNT(2),
      I3 => INIT_PRE_COUNT(3),
      O => N58
    );
  ddr_ba1_1_1 : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => INIT_COUNT(3),
      I1 => INIT_COUNT(1),
      I2 => INIT_COUNT(2),
      I3 => current_state_FFd12_136,
      O => ddr_ba_cntrl(1)
    );
  Q_n02481 : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => dqs_div_cascount(0),
      I1 => dqs_div_cascount(2),
      I2 => dqs_div_cascount(1),
      O => Q_n0248
    );
  Ker631 : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => current_state_FFd1_147,
      I1 => rdburst_end_2_163,
      I2 => rdburst_end_1_162,
      O => N63
    );
  Ker731 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => N35,
      I1 => wrburst_end_cnt(1),
      I2 => wrburst_end_cnt(0),
      O => N73
    );
  ddr_address1_1_1 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => Q_n0337,
      I1 => current_state_FFd12_136,
      I2 => burst_length(1),
      O => ddr_address_cntrl(1)
    );
  Ker171 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => RFC_COUNT(5),
      I1 => RFC_COUNT(4),
      I2 => RFC_COUNT(3),
      I3 => RFC_COUNT(2),
      O => N171
    );
  current_state_FFd4_In1 : LUT4
    generic map(
      INIT => X"FF8A"
    )
    port map (
      I0 => N35,
      I1 => wrburst_end_cnt(1),
      I2 => wrburst_end_cnt(0),
      I3 => N7,
      O => current_state_FFd4_In
    );
  current_state_FFd1_In1 : LUT4
    generic map(
      INIT => X"FF8A"
    )
    port map (
      I0 => N63,
      I1 => CAS_COUNT(1),
      I2 => CAS_COUNT(0),
      I3 => current_state_FFd6_137,
      O => current_state_FFd1_In
    );
  Q_n02871 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => N35,
      I1 => wrburst_end_cnt(0),
      I2 => N7,
      I3 => wrburst_end_cnt(1),
      O => Write_enable_out
    );
  ddr_ba1_0_SW0 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => INIT_COUNT(3),
      I1 => INIT_COUNT(0),
      O => N11
    );
  ddr_ba1_0_Q : LUT4
    generic map(
      INIT => X"4020"
    )
    port map (
      I0 => INIT_COUNT(2),
      I1 => INIT_COUNT(1),
      I2 => current_state_FFd12_136,
      I3 => N11,
      O => ddr_ba_cntrl(0)
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_6_xor11 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => INIT_PRE_COUNT(6),
      I1 => N191,
      O => INIT_PRE_COUNT_n0000(6)
    );
  Q_n03011 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => wrburst_end_cnt(0),
      I1 => wrburst_end_cnt(1),
      I2 => DQS_reset_out,
      O => DQS_enable_out
    );
  wrburst_end_cnt_value_0_1 : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => wrburst_end_cnt(1),
      I1 => wrburst_end_cnt(0),
      I2 => DQS_reset_out,
      O => wrburst_end_cnt_value(0)
    );
  DLL_RST_COUNT_value_3_1 : LUT3
    generic map(
      INIT => X"F9"
    )
    port map (
      I0 => DLL_RST_COUNT(3),
      I1 => N241,
      I2 => N25,
      O => DLL_RST_COUNT_value(3)
    );
  Q_n02951 : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => N351,
      I1 => Q_n0322,
      I2 => INIT_COUNT(2),
      I3 => INIT_COUNT(1),
      O => INIT_DONE_value
    );
  DLL_RST_COUNT_value_6_1 : LUT4
    generic map(
      INIT => X"FF41"
    )
    port map (
      I0 => Q_n0322,
      I1 => DLL_RST_COUNT(6),
      I2 => N12,
      I3 => N25,
      O => DLL_RST_COUNT_value(6)
    );
  DLL_RST_COUNT_value_2_1 : LUT4
    generic map(
      INIT => X"4441"
    )
    port map (
      I0 => N25,
      I1 => DLL_RST_COUNT(2),
      I2 => DLL_RST_COUNT(0),
      I3 => DLL_RST_COUNT(1),
      O => DLL_RST_COUNT_value(2)
    );
  Ker25_SW0 : LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      I0 => INIT_COUNT(0),
      I1 => INIT_MEMORY_133,
      O => N21
    );
  Ker25 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => INIT_COUNT(1),
      I1 => INIT_COUNT(3),
      I2 => INIT_COUNT(2),
      I3 => N21,
      O => N25
    );
  Ker19_SW0 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => INIT_PRE_COUNT(1),
      I1 => INIT_PRE_COUNT(0),
      I2 => INIT_PRE_COUNT(2),
      O => N23
    );
  Ker19 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => INIT_PRE_COUNT(5),
      I1 => INIT_PRE_COUNT(4),
      I2 => INIT_PRE_COUNT(3),
      I3 => N23,
      O => N191
    );
  Q_n032211 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => DLL_RST_COUNT(0),
      I1 => DLL_RST_COUNT(1),
      I2 => DLL_RST_COUNT(3),
      I3 => DLL_RST_COUNT(2),
      O => N36
    );
  Q_n032224 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => DLL_RST_COUNT(7),
      I1 => DLL_RST_COUNT(6),
      I2 => DLL_RST_COUNT(5),
      I3 => DLL_RST_COUNT(4),
      O => N49
    );
  Q_n032225 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => N36,
      I1 => N49,
      O => Q_n0322
    );
  Q_n03421 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => current_state_FFd12_136,
      I1 => current_state_FFd13_138,
      O => Q_n0342
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_5_xor1_SW0 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => INIT_PRE_COUNT(3),
      I1 => INIT_PRE_COUNT(1),
      I2 => INIT_PRE_COUNT(0),
      I3 => INIT_PRE_COUNT(2),
      O => N54
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_5_xor1 : LUT4
    generic map(
      INIT => X"AF8F"
    )
    port map (
      I0 => INIT_PRE_COUNT(5),
      I1 => INIT_PRE_COUNT(4),
      I2 => N191,
      I3 => N54,
      O => INIT_PRE_COUNT_n0000(5)
    );
  current_state_FFd8_In_SW0 : LUT4
    generic map(
      INIT => X"EFFF"
    )
    port map (
      I0 => RCDR_COUNT(1),
      I1 => RCDR_COUNT(2),
      I2 => read_cmd5_115,
      I3 => RCDR_COUNT(0),
      O => N56
    );
  current_state_FFd8_In : LUT4
    generic map(
      INIT => X"FF80"
    )
    port map (
      I0 => N6,
      I1 => current_state_FFd8_141,
      I2 => N56,
      I3 => current_state_FFd9_139,
      O => current_state_FFd8_In_164
    );
  current_state_FFd7_In_SW0 : LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      I0 => RCDW_COUNT(1),
      I1 => RCDW_COUNT(0),
      O => N60
    );
  current_state_FFd7_In : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => RCDW_COUNT(2),
      I1 => current_state_FFd8_141,
      I2 => N60,
      I3 => write_cmd3_127,
      O => DQS_reset_out
    );
  cas_count_value_0_1 : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => CAS_COUNT(1),
      I1 => N26,
      I2 => CAS_COUNT(0),
      O => cas_count_value(0)
    );
  Ker2411 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => DLL_RST_COUNT(2),
      I1 => DLL_RST_COUNT(1),
      I2 => DLL_RST_COUNT(0),
      O => N241
    );
  cas_count_value_1_1 : LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      I0 => CAS_COUNT(0),
      I1 => CAS_COUNT(1),
      I2 => N26,
      I3 => N63,
      O => cas_count_value(1)
    );
  current_state_FFd6_In1 : LUT4
    generic map(
      INIT => X"FF20"
    )
    port map (
      I0 => N63,
      I1 => CAS_COUNT(1),
      I2 => CAS_COUNT(0),
      I3 => N26,
      O => Q_n0217
    );
  Ker61 : LUT4
    generic map(
      INIT => X"EFFF"
    )
    port map (
      I0 => RCDW_COUNT(2),
      I1 => RCDW_COUNT(1),
      I2 => write_cmd3_127,
      I3 => RCDW_COUNT(0),
      O => N6
    );
  Ker352 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => current_state_FFd4_142,
      I1 => wrburst_end_1_157,
      I2 => wrburst_end_3_160,
      I3 => wrburst_end_2_159,
      O => N35
    );
  ddr_address1_5_Q : LUT4
    generic map(
      INIT => X"FF80"
    )
    port map (
      I0 => cas_latency(1),
      I1 => Q_n0337,
      I2 => current_state_FFd12_136,
      I3 => N691,
      O => ddr_address_cntrl(5)
    );
  ddr_address1_4_Q : LUT4
    generic map(
      INIT => X"FF80"
    )
    port map (
      I0 => cas_latency(0),
      I1 => Q_n0337,
      I2 => current_state_FFd12_136,
      I3 => N71,
      O => ddr_address_cntrl(4)
    );
  wrburst_end_cnt_value_1_SW0 : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => burst_length(1),
      I1 => N35,
      I2 => wrburst_end_cnt(1),
      O => N731
    );
  wrburst_end_cnt_value_1_Q : LUT4
    generic map(
      INIT => X"EA40"
    )
    port map (
      I0 => DQS_reset_out,
      I1 => N731,
      I2 => wrburst_end_cnt(0),
      I3 => burst_length(1),
      O => wrburst_end_cnt_value(1)
    );
  Ker26_SW0 : LUT3
    generic map(
      INIT => X"BF"
    )
    port map (
      I0 => RCDR_COUNT(2),
      I1 => RCDR_COUNT(0),
      I2 => N6,
      O => N75
    );
  Ker26 : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => RCDR_COUNT(1),
      I1 => current_state_FFd8_141,
      I2 => N75,
      I3 => read_cmd5_115,
      O => N26
    );
  current_state_FFd10_In2 : LUT3
    generic map(
      INIT => X"C8"
    )
    port map (
      I0 => ODT_COUNT(0),
      I1 => current_state_FFd10_145,
      I2 => ODT_COUNT(1),
      O => current_state_FFd10_In_map75
    );
  current_state_FFd10_In10 : LUT4
    generic map(
      INIT => X"2220"
    )
    port map (
      I0 => current_state_FFd14_144,
      I1 => INIT_MEMORY_133,
      I2 => GO_TO_ODT_ON_110,
      I3 => CONFLICT_106,
      O => current_state_FFd10_In_map79
    );
  current_state_FFd10_In17 : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => N0,
      I1 => current_state_FFd10_In_map79,
      I2 => current_state_FFd10_In_map75,
      O => current_state_FFd10_In
    );
  DLL_RST_COUNT_value_4_Q : LUT4
    generic map(
      INIT => X"0401"
    )
    port map (
      I0 => Q_n0322,
      I1 => DLL_RST_COUNT(4),
      I2 => N25,
      I3 => N155,
      O => DLL_RST_COUNT_value(4)
    );
  DLL_RST_COUNT_value_5_SW0 : LUT4
    generic map(
      INIT => X"01FF"
    )
    port map (
      I0 => N241,
      I1 => DLL_RST_COUNT(4),
      I2 => DLL_RST_COUNT(3),
      I3 => DLL_RST_COUNT(5),
      O => N157
    );
  DLL_RST_COUNT_value_5_Q : LUT4
    generic map(
      INIT => X"0103"
    )
    port map (
      I0 => N12,
      I1 => Q_n0322,
      I2 => N25,
      I3 => N157,
      O => DLL_RST_COUNT_value(5)
    );
  dqs_div_cascount_n0001_0_1 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => Q_n0238_148,
      I1 => dqs_div_cascount(0),
      O => dqs_div_cascount_n0001(0)
    );
  dqs_div_cascount_n0001_1_1 : LUT3
    generic map(
      INIT => X"F9"
    )
    port map (
      I0 => dqs_div_cascount(0),
      I1 => dqs_div_cascount(1),
      I2 => Q_n0238_148,
      O => dqs_div_cascount_n0001(1)
    );
  Q_n03371 : LUT4
    generic map(
      INIT => X"DDD9"
    )
    port map (
      I0 => INIT_COUNT(1),
      I1 => INIT_COUNT(2),
      I2 => INIT_COUNT(3),
      I3 => INIT_COUNT(0),
      O => Q_n0337
    );
  dqs_div_cascount_n0001_2_1 : LUT4
    generic map(
      INIT => X"4441"
    )
    port map (
      I0 => Q_n0238_148,
      I1 => dqs_div_cascount(2),
      I2 => dqs_div_cascount(0),
      I3 => dqs_div_cascount(1),
      O => dqs_div_cascount_n0001(2)
    );
  Q_n03781 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => dqs_div_cascount(2),
      I1 => dqs_div_cascount(1),
      I2 => Q_n0238_148,
      I3 => dqs_div_cascount(0),
      O => Q_n0378
    );
  Q_n0238 : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => N7,
      I1 => cas_latency(1),
      I2 => N165,
      I3 => cas_latency(0),
      O => Q_n0238_148
    );
  Ker661 : LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      I0 => NlwRenamedSig_OI_auto_ref_req,
      I1 => Q_n0181,
      I2 => RFC_COUNT_reg_116,
      I3 => current_state_FFd14_144,
      O => accept_cmd_in
    );
  Q_n0282_SW0 : LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      I0 => INIT_COUNT(1),
      I1 => INIT_COUNT(0),
      O => N175
    );
  Q_n0282 : LUT4
    generic map(
      INIT => X"AB8A"
    )
    port map (
      I0 => current_state_FFd12_136,
      I1 => N175,
      I2 => INIT_COUNT(3),
      I3 => INIT_COUNT(2),
      O => Q_n0282_151
    );
  Ker12_SW0 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => DLL_RST_COUNT(0),
      I1 => DLL_RST_COUNT(3),
      I2 => DLL_RST_COUNT(2),
      O => N177
    );
  Ker12 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => DLL_RST_COUNT(5),
      I1 => DLL_RST_COUNT(4),
      I2 => DLL_RST_COUNT(1),
      I3 => N177,
      O => N12
    );
  RFC_COUNT_value_5_SW0 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => RFC_COUNT(4),
      I1 => RFC_COUNT(2),
      I2 => RFC_COUNT(1),
      I3 => RFC_COUNT(0),
      O => N179
    );
  RFC_COUNT_value_5_Q : LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      I0 => RFC_COUNT(5),
      I1 => RFC_COUNT(3),
      I2 => Auto_Ref_issued_p,
      I3 => N179,
      O => RFC_COUNT_value(5)
    );
  RFC_COUNT_value_2_5 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => RFC_COUNT(3),
      I1 => RFC_COUNT(4),
      I2 => RFC_COUNT(5),
      O => RFC_COUNT_value_2_map254
    );
  RFC_COUNT_value_4_SW0 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => RFC_COUNT(3),
      I1 => RFC_COUNT(2),
      I2 => RFC_COUNT(1),
      I3 => RFC_COUNT(0),
      O => N239
    );
  RFC_COUNT_value_4_Q : LUT4
    generic map(
      INIT => X"4140"
    )
    port map (
      I0 => Auto_Ref_issued_p,
      I1 => RFC_COUNT(4),
      I2 => N239,
      I3 => RFC_COUNT(5),
      O => RFC_COUNT_value(4)
    );
  Ker3611 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => AUTOREF_COUNT(3),
      I1 => AUTOREF_COUNT(4),
      I2 => AUTOREF_COUNT(6),
      I3 => AUTOREF_COUNT(5),
      O => Ker36_map330
    );
  Ker3620 : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => AUTOREF_COUNT(9),
      I1 => AUTOREF_COUNT(10),
      I2 => AUTOREF_COUNT(7),
      I3 => AUTOREF_COUNT(8),
      O => Ker36_map335
    );
  Ker3629 : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => AUTOREF_COUNT(11),
      I1 => AUTOREF_COUNT(1),
      I2 => AUTOREF_COUNT(0),
      I3 => AUTOREF_COUNT(2),
      O => Ker36_map340
    );
  Ker3640 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => Ker36_map330,
      I1 => Ker36_map335,
      I2 => Ker36_map340,
      O => AUTOREF_value
    );
  ddr_address1_8_22 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => Q_n0282_151,
      I1 => Q_n0337,
      I2 => LMR_DLL_rst_8_Q,
      O => ddr_address1_8_map352
    );
  ddr_address1_8_46 : LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      I0 => Q_n0342,
      I1 => ddr_address1_8_map350,
      I2 => ddr_address1_8_map343,
      I3 => ddr_address1_8_map352,
      O => ddr_address_cntrl(8)
    );
  Ker501 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => INIT_MEMORY_133,
      I1 => current_state_FFd14_144,
      O => N501
    );
  RFC_COUNT_value_0_1 : LUT4
    generic map(
      INIT => X"FF54"
    )
    port map (
      I0 => RFC_COUNT(0),
      I1 => N171,
      I2 => RFC_COUNT(1),
      I3 => Auto_Ref_issued_p,
      O => RFC_COUNT_value(0)
    );
  RP_cnt_value_2_1 : LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      I0 => RP_COUNT(2),
      I1 => RP_COUNT(0),
      I2 => Q_n0215,
      I3 => RP_COUNT(1),
      O => RP_cnt_value(2)
    );
  MRD_COUNT_value_0_1 : LUT4
    generic map(
      INIT => X"F444"
    )
    port map (
      I0 => MRD_COUNT(0),
      I1 => MRD_COUNT(1),
      I2 => N1,
      I3 => N501,
      O => MRD_COUNT_value(0)
    );
  RFC_COUNT_value_1_1 : LUT4
    generic map(
      INIT => X"EBEA"
    )
    port map (
      I0 => Auto_Ref_issued_p,
      I1 => RFC_COUNT(0),
      I2 => RFC_COUNT(1),
      I3 => N171,
      O => RFC_COUNT_value(1)
    );
  RP_cnt_value_1_1 : LUT4
    generic map(
      INIT => X"4140"
    )
    port map (
      I0 => Q_n0215,
      I1 => RP_COUNT(0),
      I2 => RP_COUNT(1),
      I3 => RP_COUNT(2),
      O => RP_cnt_value(1)
    );
  Q_n0250_SW0 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => burst_length(1),
      I1 => CAS_COUNT(0),
      O => N317
    );
  Q_n0250 : LUT4
    generic map(
      INIT => X"FF5D"
    )
    port map (
      I0 => burst_done,
      I1 => N317,
      I2 => CAS_COUNT(1),
      I3 => rst180_r_155,
      O => Q_n0250_150
    );
  Q_n0245_SW0 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => burst_length(1),
      I1 => wrburst_end_cnt(0),
      O => N319
    );
  Q_n0245 : LUT4
    generic map(
      INIT => X"FF5D"
    )
    port map (
      I0 => burst_done,
      I1 => N319,
      I2 => wrburst_end_cnt(1),
      I3 => rst180_r_155,
      O => Q_n0245_149
    );
  Ker71 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => current_state_FFd5_134,
      I1 => current_state_FFd7_135,
      O => N7
    );
  Ker591 : LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      I0 => MRD_COUNT(1),
      I1 => MRD_COUNT(0),
      O => N59
    );
  RRD_COUNT_value_1_1 : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => RRD_COUNT(0),
      I1 => RRD_COUNT(1),
      I2 => Q_n0200,
      O => RRD_COUNT_value(1)
    );
  RRD_COUNT_value_0_1 : LUT3
    generic map(
      INIT => X"04"
    )
    port map (
      I0 => RRD_COUNT(0),
      I1 => RRD_COUNT(1),
      I2 => Q_n0200,
      O => RRD_COUNT_value(0)
    );
  Ker01 : LUT3
    generic map(
      INIT => X"7F"
    )
    port map (
      I0 => auto_ref_161,
      I1 => RFC_COUNT_reg_116,
      I2 => Q_n0181,
      O => N0
    );
  RCDW_COUNT_value_0_1 : LUT4
    generic map(
      INIT => X"0302"
    )
    port map (
      I0 => RCDW_COUNT(2),
      I1 => RCDW_COUNT(0),
      I2 => Q_n0200,
      I3 => RCDW_COUNT(1),
      O => RCDW_COUNT_value(0)
    );
  RCDW_COUNT_value_2_1 : LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      I0 => RCDW_COUNT(2),
      I1 => RCDW_COUNT(0),
      I2 => Q_n0200,
      I3 => RCDW_COUNT(1),
      O => RCDW_COUNT_value(2)
    );
  RCDR_COUNT_value_0_1 : LUT4
    generic map(
      INIT => X"FF54"
    )
    port map (
      I0 => RCDR_COUNT(0),
      I1 => RCDR_COUNT(1),
      I2 => RCDR_COUNT(2),
      I3 => Q_n0200,
      O => RCDR_COUNT_value(0)
    );
  RCDR_COUNT_value_2_1 : LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      I0 => RCDR_COUNT(2),
      I1 => RCDR_COUNT(0),
      I2 => Q_n0200,
      I3 => RCDR_COUNT(1),
      O => RCDR_COUNT_value(2)
    );
  Ker651 : LUT4
    generic map(
      INIT => X"0010"
    )
    port map (
      I0 => CONFLICT_106,
      I1 => GO_TO_ODT_ON_110,
      I2 => N0,
      I3 => INIT_MEMORY_133,
      O => N65
    );
  RCDR_COUNT_value_1_1 : LUT4
    generic map(
      INIT => X"0C02"
    )
    port map (
      I0 => RCDR_COUNT(2),
      I1 => RCDR_COUNT(1),
      I2 => Q_n0200,
      I3 => RCDR_COUNT(0),
      O => RCDR_COUNT_value(1)
    );
  RCDW_COUNT_value_1_1 : LUT4
    generic map(
      INIT => X"0C02"
    )
    port map (
      I0 => RCDW_COUNT(2),
      I1 => RCDW_COUNT(1),
      I2 => Q_n0200,
      I3 => RCDW_COUNT(0),
      O => RCDW_COUNT_value(1)
    );
  Q_n0381 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => INIT_MEMORY_133,
      I1 => Q_n0215,
      I2 => Auto_Ref_issued_p,
      I3 => N339,
      O => Q_n0381_153
    );
  Ker69_SW1 : LUT3
    generic map(
      INIT => X"FB"
    )
    port map (
      I0 => INIT_COUNT(1),
      I1 => INIT_COUNT(0),
      I2 => INIT_COUNT(2),
      O => N343
    );
  current_state_FFd11_In_SW1 : LUT4
    generic map(
      INIT => X"DFFF"
    )
    port map (
      I0 => current_state_FFd14_144,
      I1 => INIT_COUNT(3),
      I2 => INIT_COUNT(2),
      I3 => INIT_COUNT(1),
      O => N346
    );
  current_state_FFd13_In36 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => INIT_PRE_COUNT(6),
      I1 => INIT_PRE_COUNT(3),
      I2 => INIT_COUNT(2),
      I3 => INIT_PRE_COUNT(2),
      O => current_state_FFd13_In_map562
    );
  current_state_FFd13_In100 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => current_state_FFd2_146,
      I1 => WR_COUNT(0),
      I2 => WR_COUNT(1),
      I3 => WR_COUNT(2),
      O => current_state_FFd13_In_map575
    );
  current_state_FFd13_In103 : LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      I0 => current_state_FFd13_In_map568,
      I1 => current_state_FFd13_In_map547,
      I2 => current_state_FFd13_In_map575,
      I3 => current_state_FFd13_In_map563,
      O => Q_n0215
    );
  ddr_address1_10_SW1 : LUT4
    generic map(
      INIT => X"8C80"
    )
    port map (
      I0 => LMR_DLL_rst_10_Q,
      I1 => Q_n0342,
      I2 => Q_n0282_151,
      I3 => LMR_DLL_set(10),
      O => N425
    );
  ddr_address1_10_Q : LUT4
    generic map(
      INIT => X"8C80"
    )
    port map (
      I0 => N425,
      I1 => Q_n0337,
      I2 => Q_n0283,
      I3 => N424,
      O => ddr_address_cntrl(10)
    );
  current_state_FFd14_In19 : LUT4
    generic map(
      INIT => X"FF89"
    )
    port map (
      I0 => INIT_COUNT(0),
      I1 => INIT_COUNT(1),
      I2 => RFC_COUNT_reg_116,
      I3 => INIT_COUNT(2),
      O => current_state_FFd14_In_map632
    );
  current_state_FFd14_In57 : LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      I0 => INIT_COUNT(3),
      I1 => current_state_FFd14_In_map632,
      I2 => N59,
      I3 => current_state_FFd14_In_map640,
      O => current_state_FFd14_In_map642
    );
  current_state_FFd14_In74 : LUT3
    generic map(
      INIT => X"08"
    )
    port map (
      I0 => INIT_COUNT(1),
      I1 => INIT_COUNT(2),
      I2 => N32,
      O => current_state_FFd14_In_map645
    );
  current_state_FFd14_In139 : LUT3
    generic map(
      INIT => X"01"
    )
    port map (
      I0 => INIT_COUNT(2),
      I1 => INIT_COUNT(3),
      I2 => INIT_COUNT(1),
      O => current_state_FFd14_In_map663
    );
  current_state_FFd14_In145 : LUT4
    generic map(
      INIT => X"7200"
    )
    port map (
      I0 => INIT_COUNT(0),
      I1 => Q_n0181,
      I2 => current_state_FFd14_In_map655,
      I3 => current_state_FFd14_In_map663,
      O => current_state_FFd14_In_map664
    );
  current_state_FFd14_In172 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => INIT_MEMORY_133,
      I1 => current_state_FFd14_In_map642,
      I2 => current_state_FFd14_In_map645,
      I3 => current_state_FFd14_In_map664,
      O => current_state_FFd14_In_map666
    );
  current_state_FFd14_In186 : LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      I0 => GO_TO_ACTIVE_158,
      I1 => N65,
      O => current_state_FFd14_In_map668
    );
  current_state_FFd14_In215 : LUT4
    generic map(
      INIT => X"EEEC"
    )
    port map (
      I0 => current_state_FFd14_144,
      I1 => current_state_FFd14_In_map624,
      I2 => current_state_FFd14_In_map668,
      I3 => current_state_FFd14_In_map666,
      O => current_state_FFd14_In
    );
  Ker3511 : LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      I0 => INIT_COUNT(0),
      I1 => INIT_COUNT(3),
      O => N351
    );
  Ker721 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => current_state_FFd12_136,
      I1 => current_state_FFd11_140,
      O => N72
    );
  Q_n01811 : LUT3
    generic map(
      INIT => X"01"
    )
    port map (
      I0 => RP_COUNT(2),
      I1 => RP_COUNT(0),
      I2 => RP_COUNT(1),
      O => Q_n0181
    );
  Ker321 : LUT3
    generic map(
      INIT => X"B8"
    )
    port map (
      I0 => RFC_COUNT_reg_116,
      I1 => INIT_COUNT(0),
      I2 => Q_n0181,
      O => N32
    );
  Q_n02835 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => N7,
      I1 => current_state_FFd14_144,
      I2 => current_state_FFd6_137,
      I3 => current_state_FFd1_147,
      O => N555
    );
  Q_n028313 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => current_state_FFd4_142,
      I1 => current_state_FFd3_143,
      I2 => current_state_FFd2_146,
      I3 => current_state_FFd9_139,
      O => N563
    );
  Q_n028341 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => N578,
      I1 => N555,
      I2 => N558,
      I3 => N563,
      O => Q_n0283
    );
  dqs_enable_i1 : LUT4
    generic map(
      INIT => X"ACCC"
    )
    port map (
      I0 => DQS_enable2_104,
      I1 => DQS_enable1_103,
      I2 => cas_latency(0),
      I3 => cas_latency(1),
      O => NlwRenamedSig_OI_dqs_enable
    );
  AUTOREF_COUNT_1_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => AUTOREF_COUNT(1),
      O => AUTOREF_COUNT_1_rt_165
    );
  AUTOREF_COUNT_2_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => AUTOREF_COUNT(2),
      O => AUTOREF_COUNT_2_rt_166
    );
  AUTOREF_COUNT_3_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => AUTOREF_COUNT(3),
      O => AUTOREF_COUNT_3_rt_167
    );
  AUTOREF_COUNT_4_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => AUTOREF_COUNT(4),
      O => AUTOREF_COUNT_4_rt_168
    );
  AUTOREF_COUNT_5_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => AUTOREF_COUNT(5),
      O => AUTOREF_COUNT_5_rt_169
    );
  AUTOREF_COUNT_6_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => AUTOREF_COUNT(6),
      O => AUTOREF_COUNT_6_rt_170
    );
  AUTOREF_COUNT_7_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => AUTOREF_COUNT(7),
      O => AUTOREF_COUNT_7_rt_171
    );
  AUTOREF_COUNT_8_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => AUTOREF_COUNT(8),
      O => AUTOREF_COUNT_8_rt_172
    );
  AUTOREF_COUNT_9_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => AUTOREF_COUNT(9),
      O => AUTOREF_COUNT_9_rt_173
    );
  AUTOREF_COUNT_10_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => AUTOREF_COUNT(10),
      O => AUTOREF_COUNT_10_rt_174
    );
  AUTOREF_COUNT_11_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => AUTOREF_COUNT(11),
      O => AUTOREF_COUNT_11_rt_175
    );
  DQS_reset1_clk0_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => DQS_reset1_clk0_118,
      O => DQS_reset1_clk0_rt_176
    );
  current_state_FFd3_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => current_state_FFd3_143,
      O => current_state_FFd3_rt_177
    );
  INIT_PRE_COUNT_4_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => INIT_PRE_COUNT(4),
      O => INIT_PRE_COUNT_4_rt_178
    );
  dqs_enable_i1_SW0 : LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      I0 => cas_latency(1),
      I1 => cas_latency(0),
      O => N586
    );
  WR_COUNT_n00001 : LUT4
    generic map(
      INIT => X"FBEA"
    )
    port map (
      I0 => rst180_r_155,
      I1 => N586,
      I2 => DQS_enable1_103,
      I3 => DQS_enable2_104,
      O => WR_COUNT_0_n0000
    );
  Q_n0238_SW0 : LUT4
    generic map(
      INIT => X"FFFD"
    )
    port map (
      I0 => current_state_FFd6_137,
      I1 => current_state_FFd13_138,
      I2 => current_state_FFd9_139,
      I3 => N72,
      O => N165
    );
  Q_n0381_SW0 : LUT4
    generic map(
      INIT => X"AA08"
    )
    port map (
      I0 => current_state_FFd14_144,
      I1 => N01,
      I2 => N59,
      I3 => N69,
      O => N339
    );
  Q_n02891 : LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      I0 => read_cmd1_111,
      I1 => command_register(1),
      I2 => command_register(2),
      I3 => accept_cmd_in,
      O => GO_TO_ACTIVE_value
    );
  Ker011 : LUT4
    generic map(
      INIT => X"1446"
    )
    port map (
      I0 => INIT_COUNT(2),
      I1 => INIT_COUNT(1),
      I2 => INIT_COUNT(0),
      I3 => INIT_COUNT(3),
      O => N01
    );
  Q_n03001 : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => command_register(1),
      I1 => command_register(2),
      I2 => write_cmd1_125,
      I3 => accept_cmd_in,
      O => GO_TO_ODT_ON_value
    );
  Q_n02911 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => command_register(1),
      I1 => command_register(2),
      I2 => accept_cmd_in,
      O => read_cmd
    );
  Q_n02981 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => command_register(2),
      I1 => command_register(1),
      I2 => accept_cmd_in,
      O => write_cmd_in
    );
  current_state_FFd9_In_SW0_SW0 : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => current_state_FFd10_145,
      I1 => ODT_COUNT(1),
      I2 => ODT_COUNT(0),
      O => N590
    );
  current_state_FFd9_In : LUT4
    generic map(
      INIT => X"FF80"
    )
    port map (
      I0 => GO_TO_ACTIVE_158,
      I1 => current_state_FFd14_144,
      I2 => N65,
      I3 => N590,
      O => Q_n0200
    );
  AUTOREF_CNT_val_11_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => NlwRenamedSig_OI_init,
      I1 => AUTOREF_value,
      I2 => Q_n0254(11),
      O => AUTOREF_CNT_val(11)
    );
  AUTOREF_CNT_val_10_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => NlwRenamedSig_OI_init,
      I1 => AUTOREF_value,
      I2 => Q_n0254(10),
      O => AUTOREF_CNT_val(10)
    );
  AUTOREF_CNT_val_9_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => NlwRenamedSig_OI_init,
      I1 => AUTOREF_value,
      I2 => Q_n0254(9),
      O => AUTOREF_CNT_val(9)
    );
  AUTOREF_CNT_val_8_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => NlwRenamedSig_OI_init,
      I1 => AUTOREF_value,
      I2 => Q_n0254(8),
      O => AUTOREF_CNT_val(8)
    );
  AUTOREF_CNT_val_7_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => NlwRenamedSig_OI_init,
      I1 => AUTOREF_value,
      I2 => Q_n0254(7),
      O => AUTOREF_CNT_val(7)
    );
  current_state_FFd14_In105_SW0 : LUT4
    generic map(
      INIT => X"FFFD"
    )
    port map (
      I0 => INIT_PRE_COUNT(0),
      I1 => INIT_PRE_COUNT(2),
      I2 => INIT_PRE_COUNT(1),
      I3 => INIT_PRE_COUNT(3),
      O => N592
    );
  current_state_FFd14_In105 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => INIT_PRE_COUNT(6),
      I1 => INIT_PRE_COUNT(5),
      I2 => INIT_PRE_COUNT(4),
      I3 => N592,
      O => current_state_FFd14_In_map655
    );
  Q_n02412 : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => dqs_div_cascount(2),
      I1 => burst_length(1),
      I2 => dqs_div_cascount(1),
      I3 => dqs_div_cascount(0),
      O => Q_n0241
    );
  RFC_COUNT_value_3_19_SW0 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => RFC_COUNT(2),
      I1 => RFC_COUNT(1),
      I2 => RFC_COUNT(0),
      O => N594
    );
  current_state_FFd13_In37_SW0 : LUT3
    generic map(
      INIT => X"FB"
    )
    port map (
      I0 => INIT_PRE_COUNT(1),
      I1 => INIT_PRE_COUNT(0),
      I2 => INIT_COUNT(0),
      O => N596
    );
  current_state_FFd13_In37 : LUT4
    generic map(
      INIT => X"0100"
    )
    port map (
      I0 => INIT_PRE_COUNT(5),
      I1 => INIT_PRE_COUNT(4),
      I2 => N596,
      I3 => current_state_FFd13_In_map562,
      O => current_state_FFd13_In_map563
    );
  RFC_COUNT_value_2_36_SW0 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => RFC_COUNT(1),
      I1 => RFC_COUNT(0),
      O => N598
    );
  RFC_COUNT_value_2_36 : LUT4
    generic map(
      INIT => X"4140"
    )
    port map (
      I0 => Auto_Ref_issued_p,
      I1 => N598,
      I2 => RFC_COUNT(2),
      I3 => RFC_COUNT_value_2_map254,
      O => RFC_COUNT_value(2)
    );
  current_state_FFd12_In1 : LUT4
    generic map(
      INIT => X"AA08"
    )
    port map (
      I0 => N501,
      I1 => N01,
      I2 => N59,
      I3 => N69,
      O => Q_n0213
    );
  current_state_FFd14_In46 : LUT4
    generic map(
      INIT => X"F666"
    )
    port map (
      I0 => INIT_COUNT(1),
      I1 => INIT_COUNT(2),
      I2 => INIT_COUNT(0),
      I3 => INIT_COUNT(3),
      O => current_state_FFd14_In_map640
    );
  current_state_FFd13_In4 : LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      I0 => MRD_COUNT(1),
      I1 => INIT_COUNT(2),
      I2 => MRD_COUNT(0),
      I3 => INIT_COUNT(0),
      O => current_state_FFd13_In_map547
    );
  current_state_FFd13_In73 : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => INIT_COUNT(1),
      I1 => INIT_MEMORY_133,
      I2 => INIT_COUNT(3),
      I3 => current_state_FFd14_144,
      O => current_state_FFd13_In_map568
    );
  Q_n02838 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => current_state_FFd12_136,
      I1 => current_state_FFd11_140,
      I2 => current_state_FFd10_145,
      I3 => current_state_FFd8_141,
      O => N558
    );
  Q_n028338 : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => INIT_COUNT(2),
      I1 => INIT_COUNT(0),
      I2 => INIT_COUNT(1),
      I3 => INIT_COUNT(3),
      O => N578
    );
  DLL_RST_COUNT_value_4_SW0 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => DLL_RST_COUNT(3),
      I1 => DLL_RST_COUNT(2),
      I2 => DLL_RST_COUNT(1),
      I3 => DLL_RST_COUNT(0),
      O => N155
    );
  current_state_FFd14_In0 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => current_state_FFd12_136,
      I1 => current_state_FFd11_140,
      I2 => current_state_FFd13_138,
      O => current_state_FFd14_In_map624
    );
  current_state_Out241 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => current_state_FFd5_134,
      I1 => current_state_FFd7_135,
      I2 => current_state_FFd13_138,
      I3 => current_state_FFd12_136,
      O => ddr_web_cntrl
    );
  Ker641 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => current_state_FFd13_138,
      I1 => current_state_FFd9_139,
      I2 => current_state_FFd12_136,
      I3 => current_state_FFd11_140,
      O => ddr_rasb_cntrl
    );
  AUTOREF_CNT_val_6_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => NlwRenamedSig_OI_init,
      I1 => AUTOREF_value,
      I2 => Q_n0254(6),
      O => AUTOREF_CNT_val(6)
    );
  AUTOREF_CNT_val_5_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => NlwRenamedSig_OI_init,
      I1 => AUTOREF_value,
      I2 => Q_n0254(5),
      O => AUTOREF_CNT_val(5)
    );
  AUTOREF_CNT_val_4_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => NlwRenamedSig_OI_init,
      I1 => AUTOREF_value,
      I2 => Q_n0254(4),
      O => AUTOREF_CNT_val(4)
    );
  AUTOREF_CNT_val_3_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => NlwRenamedSig_OI_init,
      I1 => AUTOREF_value,
      I2 => Q_n0254(3),
      O => AUTOREF_CNT_val(3)
    );
  AUTOREF_CNT_val_2_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => NlwRenamedSig_OI_init,
      I1 => AUTOREF_value,
      I2 => Q_n0254(2),
      O => AUTOREF_CNT_val(2)
    );
  AUTOREF_CNT_val_1_1 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => NlwRenamedSig_OI_init,
      I1 => AUTOREF_value,
      I2 => Q_n0254(1),
      O => AUTOREF_CNT_val(1)
    );
  ddr_address1_2_18 : LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      I0 => COLUMN_ADDRESS_reg(2),
      I1 => current_state_FFd6_137,
      I2 => ddr_address1_2_map88,
      I3 => N7,
      O => ddr_address_cntrl(2)
    );
  current_state_Out271 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => current_state_FFd12_136,
      I1 => current_state_FFd11_140,
      I2 => N7,
      I3 => current_state_FFd6_137,
      O => ddr_casb_cntrl
    );
  RP_cnt_value_0_1 : LUT4
    generic map(
      INIT => X"0302"
    )
    port map (
      I0 => RP_COUNT(2),
      I1 => Q_n0215,
      I2 => RP_COUNT(0),
      I3 => RP_COUNT(1),
      O => RP_cnt_value(0)
    );
  ddr_address1_10_SW0 : LUT4
    generic map(
      INIT => X"E2A2"
    )
    port map (
      I0 => current_state_FFd13_138,
      I1 => Q_n0282_151,
      I2 => LMR_DLL_rst_10_Q,
      I3 => current_state_FFd12_136,
      O => N424
    );
  DLL_RST_COUNT_value_0_1 : LUT4
    generic map(
      INIT => X"3111"
    )
    port map (
      I0 => DLL_RST_COUNT(0),
      I1 => N25,
      I2 => N36,
      I3 => N49,
      O => DLL_RST_COUNT_value(0)
    );
  ddr_address1_2_13_SW0 : LUT4
    generic map(
      INIT => X"BFBD"
    )
    port map (
      I0 => INIT_COUNT(2),
      I1 => INIT_COUNT(3),
      I2 => INIT_COUNT(1),
      I3 => INIT_COUNT(0),
      O => N600
    );
  ddr_address1_2_13 : LUT4
    generic map(
      INIT => X"2220"
    )
    port map (
      I0 => EMR_2_Q,
      I1 => N600,
      I2 => current_state_FFd12_136,
      I3 => current_state_FFd13_138,
      O => ddr_address1_2_map88
    );
  DLL_RST_COUNT_value_7_SW1 : LUT3
    generic map(
      INIT => X"36"
    )
    port map (
      I0 => N12,
      I1 => DLL_RST_COUNT(7),
      I2 => DLL_RST_COUNT(6),
      O => N602
    );
  DLL_RST_COUNT_value_7_Q : LUT4
    generic map(
      INIT => X"FF13"
    )
    port map (
      I0 => N36,
      I1 => N602,
      I2 => N49,
      I3 => N25,
      O => DLL_RST_COUNT_value(7)
    );
  ddr_address1_7_30 : LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      I0 => COLUMN_ADDRESS_reg(7),
      I1 => N7,
      I2 => N604,
      I3 => current_state_FFd6_137,
      O => ddr_address_cntrl(7)
    );
  ddr_address1_9_30 : LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      I0 => COLUMN_ADDRESS_reg(9),
      I1 => N7,
      I2 => N606,
      I3 => current_state_FFd6_137,
      O => ddr_address_cntrl(9)
    );
  ddr_address1_8_14_SW0 : LUT4
    generic map(
      INIT => X"BBDF"
    )
    port map (
      I0 => INIT_COUNT(2),
      I1 => INIT_COUNT(1),
      I2 => EMR_8_Q,
      I3 => INIT_COUNT(3),
      O => N608
    );
  ddr_address1_8_14 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => INIT_COUNT(0),
      I1 => N608,
      O => ddr_address1_8_map350
    );
  ddr_address1_5_SW0 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => COLUMN_ADDRESS_reg(5),
      I1 => current_state_FFd6_137,
      I2 => current_state_FFd5_134,
      I3 => current_state_FFd7_135,
      O => N691
    );
  ddr_address1_4_SW0 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => COLUMN_ADDRESS_reg(4),
      I1 => current_state_FFd6_137,
      I2 => current_state_FFd5_134,
      I3 => current_state_FFd7_135,
      O => N71
    );
  ddr_address1_8_0 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => COLUMN_ADDRESS_reg(8),
      I1 => current_state_FFd6_137,
      I2 => current_state_FFd5_134,
      I3 => current_state_FFd7_135,
      O => ddr_address1_8_map343
    );
  ddr_address1_6_1 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => COLUMN_ADDRESS_reg(6),
      I1 => current_state_FFd6_137,
      I2 => current_state_FFd5_134,
      I3 => current_state_FFd7_135,
      O => ddr_address_cntrl(6)
    );
  ddr_address1_3_1 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => COLUMN_ADDRESS_reg(3),
      I1 => current_state_FFd6_137,
      I2 => current_state_FFd5_134,
      I3 => current_state_FFd7_135,
      O => ddr_address_cntrl(3)
    );
  Ker11 : LUT4
    generic map(
      INIT => X"FF20"
    )
    port map (
      I0 => MRD_COUNT(0),
      I1 => MRD_COUNT(1),
      I2 => N01,
      I3 => N69,
      O => N1
    );
  AUTOREF_CNT_val_0_1 : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => NlwRenamedSig_OI_init,
      I1 => AUTOREF_value,
      I2 => AUTOREF_COUNT(0),
      O => AUTOREF_CNT_val(0)
    );
  ddr_address1_7_30_SW0_SW0 : LUT4
    generic map(
      INIT => X"BBDF"
    )
    port map (
      I0 => INIT_COUNT(2),
      I1 => INIT_COUNT(3),
      I2 => EMR_7_Q,
      I3 => INIT_COUNT(1),
      O => N610
    );
  ddr_address1_7_30_SW0 : LUT4
    generic map(
      INIT => X"0302"
    )
    port map (
      I0 => current_state_FFd12_136,
      I1 => INIT_COUNT(0),
      I2 => N610,
      I3 => current_state_FFd13_138,
      O => N604
    );
  ddr_address1_9_30_SW0_SW0 : LUT4
    generic map(
      INIT => X"BBDF"
    )
    port map (
      I0 => INIT_COUNT(2),
      I1 => INIT_COUNT(3),
      I2 => EMR_9_Q,
      I3 => INIT_COUNT(1),
      O => N612
    );
  ddr_address1_9_30_SW0 : LUT4
    generic map(
      INIT => X"0302"
    )
    port map (
      I0 => current_state_FFd12_136,
      I1 => INIT_COUNT(0),
      I2 => N612,
      I3 => current_state_FFd13_138,
      O => N606
    );
  MRD_COUNT_value_1_Q : MUXF5
    port map (
      I0 => N614,
      I1 => N615,
      S => N69,
      O => MRD_COUNT_value(1)
    );
  MRD_COUNT_value_1_F : LUT4
    generic map(
      INIT => X"A888"
    )
    port map (
      I0 => MRD_COUNT(0),
      I1 => MRD_COUNT(1),
      I2 => N501,
      I3 => N01,
      O => N614
    );
  MRD_COUNT_value_1_G : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => MRD_COUNT(1),
      I1 => MRD_COUNT(0),
      I2 => N501,
      O => N615
    );
  current_state_FFd11_In : MUXF5
    port map (
      I0 => N616,
      I1 => N617,
      S => INIT_MEMORY_133,
      O => Auto_Ref_issued_p
    );
  current_state_FFd11_In_F : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => Q_n0181,
      I1 => RFC_COUNT_reg_116,
      I2 => current_state_FFd14_144,
      I3 => auto_ref_161,
      O => N616
    );
  current_state_FFd11_In_G : LUT4
    generic map(
      INIT => X"0D08"
    )
    port map (
      I0 => INIT_COUNT(0),
      I1 => RFC_COUNT_reg_116,
      I2 => N346,
      I3 => Q_n0181,
      O => N617
    );
  Ker69 : MUXF5
    port map (
      I0 => N618,
      I1 => N619,
      S => INIT_COUNT(3),
      O => N69
    );
  Ker69_F : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => RP_COUNT(2),
      I1 => RP_COUNT(0),
      I2 => RP_COUNT(1),
      I3 => N343,
      O => N618
    );
  Ker69_G : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => RFC_COUNT_reg_116,
      I1 => INIT_COUNT(2),
      I2 => INIT_COUNT(1),
      I3 => INIT_COUNT(0),
      O => N619
    );
  RFC_COUNT_value_3_41 : MUXF5
    port map (
      I0 => N620,
      I1 => N621,
      S => RFC_COUNT(3),
      O => RFC_COUNT_value(3)
    );
  RFC_COUNT_value_3_41_F : LUT4
    generic map(
      INIT => X"0302"
    )
    port map (
      I0 => RFC_COUNT(4),
      I1 => N594,
      I2 => Auto_Ref_issued_p,
      I3 => RFC_COUNT(5),
      O => N620
    );
  RFC_COUNT_value_3_41_G : LUT4
    generic map(
      INIT => X"3332"
    )
    port map (
      I0 => RFC_COUNT(1),
      I1 => Auto_Ref_issued_p,
      I2 => RFC_COUNT(2),
      I3 => RFC_COUNT(0),
      O => N621
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity infrastructure_iobs_16bit is
  port (
    clk0 : in STD_LOGIC := 'X'; 
    clk180 : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    ddr2_clk0 : out STD_LOGIC; 
    ddr2_clk0b : out STD_LOGIC 
  );
end infrastructure_iobs_16bit;

architecture Structure of infrastructure_iobs_16bit is
  signal vcc_179 : STD_LOGIC; 
  signal ddr2_clk0_q : STD_LOGIC; 
  signal gnd_180 : STD_LOGIC; 
  signal ddr2_clk0b_q : STD_LOGIC; 
begin
  XST_VCC : VCC
    port map (
      P => vcc_179
    );
  XST_GND : GND
    port map (
      G => gnd_180
    );
  U1 : FDDRRSE
    port map (
      CE => vcc_179,
      R => gnd_180,
      S => gnd_180,
      C0 => clk0,
      C1 => clk180,
      D0 => vcc_179,
      D1 => gnd_180,
      Q => ddr2_clk0_q
    );
  U2 : FDDRRSE
    port map (
      CE => vcc_179,
      R => gnd_180,
      S => gnd_180,
      C0 => clk0,
      C1 => clk180,
      D0 => gnd_180,
      D1 => vcc_179,
      Q => ddr2_clk0b_q
    );
  r1 : OBUF
    port map (
      I => ddr2_clk0_q,
      O => ddr2_clk0
    );
  r2 : OBUF
    port map (
      I => ddr2_clk0b_q,
      O => ddr2_clk0b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity controller_iobs_16bit_00 is
  port (
    ddr_ODT_cntrl : in STD_LOGIC := 'X'; 
    ddr_csb_cntrl : in STD_LOGIC := 'X'; 
    ddr_web_cntrl : in STD_LOGIC := 'X'; 
    clk0 : in STD_LOGIC := 'X'; 
    clk180 : in STD_LOGIC := 'X'; 
    rst_dqs_div_in : in STD_LOGIC := 'X'; 
    ddr_casb_cntrl : in STD_LOGIC := 'X'; 
    ddr_cke_cntrl : in STD_LOGIC := 'X'; 
    ddr_rasb_cntrl : in STD_LOGIC := 'X'; 
    rst_dqs_div_int : in STD_LOGIC := 'X'; 
    rst_dqs_div : out STD_LOGIC; 
    ddr_cke : out STD_LOGIC; 
    ddr_csb : out STD_LOGIC; 
    ddr_rasb : out STD_LOGIC; 
    ddr_web : out STD_LOGIC; 
    ddr_casb : out STD_LOGIC; 
    ddr_ODT0 : out STD_LOGIC; 
    rst_dqs_div_out : out STD_LOGIC; 
    ddr_ba_cntrl : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
    ddr_address_cntrl : in STD_LOGIC_VECTOR ( 12 downto 0 ); 
    ddr_address : out STD_LOGIC_VECTOR ( 12 downto 0 ); 
    ddr_ba : out STD_LOGIC_VECTOR ( 1 downto 0 ) 
  );
end controller_iobs_16bit_00;

architecture Structure of controller_iobs_16bit_00 is
  signal ddr_web_q : STD_LOGIC; 
  signal ddr_cke_q : STD_LOGIC; 
  signal ddr_rasb_q : STD_LOGIC; 
  signal ddr_casb_q : STD_LOGIC; 
  signal ddr_address_reg : STD_LOGIC_VECTOR ( 12 downto 0 ); 
  signal ddr_ba_reg : STD_LOGIC_VECTOR ( 1 downto 0 ); 
begin
  iob_web : FD
    port map (
      D => ddr_web_cntrl,
      C => clk180,
      Q => ddr_web_q
    );
  iob_rasb : FD
    port map (
      D => ddr_rasb_cntrl,
      C => clk180,
      Q => ddr_rasb_q
    );
  iob_casb : FD
    port map (
      D => ddr_casb_cntrl,
      C => clk180,
      Q => ddr_casb_q
    );
  iob_cke : FD
    port map (
      D => ddr_cke_cntrl,
      C => clk180,
      Q => ddr_cke_q
    );
  iob_addr_0 : FD
    port map (
      D => ddr_address_cntrl(0),
      C => clk180,
      Q => ddr_address_reg(0)
    );
  iob_addr_1 : FD
    port map (
      D => ddr_address_cntrl(1),
      C => clk180,
      Q => ddr_address_reg(1)
    );
  iob_addr_2 : FD
    port map (
      D => ddr_address_cntrl(2),
      C => clk180,
      Q => ddr_address_reg(2)
    );
  iob_addr_3 : FD
    port map (
      D => ddr_address_cntrl(3),
      C => clk180,
      Q => ddr_address_reg(3)
    );
  iob_addr_4 : FD
    port map (
      D => ddr_address_cntrl(4),
      C => clk180,
      Q => ddr_address_reg(4)
    );
  iob_addr_5 : FD
    port map (
      D => ddr_address_cntrl(5),
      C => clk180,
      Q => ddr_address_reg(5)
    );
  iob_addr_6 : FD
    port map (
      D => ddr_address_cntrl(6),
      C => clk180,
      Q => ddr_address_reg(6)
    );
  iob_addr_7 : FD
    port map (
      D => ddr_address_cntrl(7),
      C => clk180,
      Q => ddr_address_reg(7)
    );
  iob_addr_8 : FD
    port map (
      D => ddr_address_cntrl(8),
      C => clk180,
      Q => ddr_address_reg(8)
    );
  iob_addr_9 : FD
    port map (
      D => ddr_address_cntrl(9),
      C => clk180,
      Q => ddr_address_reg(9)
    );
  iob_addr_10 : FD
    port map (
      D => ddr_address_cntrl(10),
      C => clk180,
      Q => ddr_address_reg(10)
    );
  iob_addr_11 : FD
    port map (
      D => ddr_address_cntrl(11),
      C => clk180,
      Q => ddr_address_reg(11)
    );
  iob_addr_12 : FD
    port map (
      D => ddr_address_cntrl(12),
      C => clk180,
      Q => ddr_address_reg(12)
    );
  iob_ba_0 : FD
    port map (
      D => ddr_ba_cntrl(0),
      C => clk180,
      Q => ddr_ba_reg(0)
    );
  iob_ba_1 : FD
    port map (
      D => ddr_ba_cntrl(1),
      C => clk180,
      Q => ddr_ba_reg(1)
    );
  r16 : OBUF
    port map (
      I => ddr_web_q,
      O => ddr_web
    );
  r17 : OBUF
    port map (
      I => ddr_rasb_q,
      O => ddr_rasb
    );
  r18 : OBUF
    port map (
      I => ddr_casb_q,
      O => ddr_casb
    );
  r19 : OBUF
    port map (
      I => ddr_csb_cntrl,
      O => ddr_csb
    );
  r20 : OBUF
    port map (
      I => ddr_cke_q,
      O => ddr_cke
    );
  r21 : OBUF
    port map (
      I => ddr_ODT_cntrl,
      O => ddr_ODT0
    );
  r12 : OBUF
    port map (
      I => ddr_address_reg(12),
      O => ddr_address(12)
    );
  r11 : OBUF
    port map (
      I => ddr_address_reg(11),
      O => ddr_address(11)
    );
  r10 : OBUF
    port map (
      I => ddr_address_reg(10),
      O => ddr_address(10)
    );
  r9 : OBUF
    port map (
      I => ddr_address_reg(9),
      O => ddr_address(9)
    );
  r8 : OBUF
    port map (
      I => ddr_address_reg(8),
      O => ddr_address(8)
    );
  r7 : OBUF
    port map (
      I => ddr_address_reg(7),
      O => ddr_address(7)
    );
  r6 : OBUF
    port map (
      I => ddr_address_reg(6),
      O => ddr_address(6)
    );
  r5 : OBUF
    port map (
      I => ddr_address_reg(5),
      O => ddr_address(5)
    );
  r4 : OBUF
    port map (
      I => ddr_address_reg(4),
      O => ddr_address(4)
    );
  r3 : OBUF
    port map (
      I => ddr_address_reg(3),
      O => ddr_address(3)
    );
  r2 : OBUF
    port map (
      I => ddr_address_reg(2),
      O => ddr_address(2)
    );
  r1 : OBUF
    port map (
      I => ddr_address_reg(1),
      O => ddr_address(1)
    );
  r0 : OBUF
    port map (
      I => ddr_address_reg(0),
      O => ddr_address(0)
    );
  r13 : OBUF
    port map (
      I => ddr_ba_reg(1),
      O => ddr_ba(1)
    );
  r00 : OBUF
    port map (
      I => ddr_ba_reg(0),
      O => ddr_ba(0)
    );
  rst_iob_inbuf : IBUF
    port map (
      I => rst_dqs_div_in,
      O => rst_dqs_div
    );
  rst_iob_outbuf : OBUF
    port map (
      I => rst_dqs_div_int,
      O => rst_dqs_div_out
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_dm_16bit is
  port (
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    mask_rising : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
    mask_falling : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
    ddr_dm : out STD_LOGIC_VECTOR ( 1 downto 0 ) 
  );
end ddr2_dm_16bit;

architecture Structure of ddr2_dm_16bit is
  signal vcc_181 : STD_LOGIC; 
  signal gnd_182 : STD_LOGIC; 
  signal mask_o : STD_LOGIC_VECTOR ( 1 downto 0 ); 
begin
  XST_VCC : VCC
    port map (
      P => vcc_181
    );
  XST_GND : GND
    port map (
      G => gnd_182
    );
  DDR_DM0_OUT : FDDRRSE
    port map (
      CE => vcc_181,
      R => gnd_182,
      S => gnd_182,
      C0 => clk270,
      C1 => clk90,
      D0 => mask_rising(0),
      D1 => mask_falling(0),
      Q => mask_o(0)
    );
  DDR_DM1_OUT : FDDRRSE
    port map (
      CE => vcc_181,
      R => gnd_182,
      S => gnd_182,
      C0 => clk270,
      C1 => clk90,
      D0 => mask_rising(1),
      D1 => mask_falling(1),
      Q => mask_o(1)
    );
  DM0_OBUF : OBUF
    port map (
      I => mask_o(0),
      O => ddr_dm(0)
    );
  DM1_OBUF : OBUF
    port map (
      I => mask_o(1),
      O => ddr_dm(1)
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dq_iob_INST_15 is
  port (
    reset : in STD_LOGIC := 'X'; 
    write_data_falling : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    write_data_rising : in STD_LOGIC := 'X'; 
    ddr_dq_inout : inout STD_LOGIC; 
    read_data_in : out STD_LOGIC 
  );
end ddr_dq_iob_INST_15;

architecture Structure of ddr_dq_iob_INST_15 is
  signal clock_en : STD_LOGIC; 
  signal GND_183 : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : GND
    port map (
      G => GND_183
    );
  XST_VCC : VCC
    port map (
      P => clock_en
    );
  DQ_T : FDCE
    port map (
      D => enable_b,
      CE => clock_en,
      CLR => reset,
      C => clk270,
      Q => ddr_en
    );
  DDR_OUT : FDDRRSE
    port map (
      CE => clock_en,
      R => GND_183,
      S => GND_183,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_OBUFT : OBUFT
    port map (
      I => ddr_dq_q,
      T => ddr_en,
      O => ddr_dq_inout
    );
  DQ_IBUF : IBUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => write_en_val,
      O => enable_b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dq_iob_INST_14 is
  port (
    reset : in STD_LOGIC := 'X'; 
    write_data_falling : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    write_data_rising : in STD_LOGIC := 'X'; 
    ddr_dq_inout : inout STD_LOGIC; 
    read_data_in : out STD_LOGIC 
  );
end ddr_dq_iob_INST_14;

architecture Structure of ddr_dq_iob_INST_14 is
  signal clock_en : STD_LOGIC; 
  signal GND_184 : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : GND
    port map (
      G => GND_184
    );
  XST_VCC : VCC
    port map (
      P => clock_en
    );
  DQ_T : FDCE
    port map (
      D => enable_b,
      CE => clock_en,
      CLR => reset,
      C => clk270,
      Q => ddr_en
    );
  DDR_OUT : FDDRRSE
    port map (
      CE => clock_en,
      R => GND_184,
      S => GND_184,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_OBUFT : OBUFT
    port map (
      I => ddr_dq_q,
      T => ddr_en,
      O => ddr_dq_inout
    );
  DQ_IBUF : IBUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => write_en_val,
      O => enable_b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dq_iob_INST_13 is
  port (
    reset : in STD_LOGIC := 'X'; 
    write_data_falling : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    write_data_rising : in STD_LOGIC := 'X'; 
    ddr_dq_inout : inout STD_LOGIC; 
    read_data_in : out STD_LOGIC 
  );
end ddr_dq_iob_INST_13;

architecture Structure of ddr_dq_iob_INST_13 is
  signal clock_en : STD_LOGIC; 
  signal GND_185 : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : GND
    port map (
      G => GND_185
    );
  XST_VCC : VCC
    port map (
      P => clock_en
    );
  DQ_T : FDCE
    port map (
      D => enable_b,
      CE => clock_en,
      CLR => reset,
      C => clk270,
      Q => ddr_en
    );
  DDR_OUT : FDDRRSE
    port map (
      CE => clock_en,
      R => GND_185,
      S => GND_185,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_OBUFT : OBUFT
    port map (
      I => ddr_dq_q,
      T => ddr_en,
      O => ddr_dq_inout
    );
  DQ_IBUF : IBUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => write_en_val,
      O => enable_b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dq_iob_INST_12 is
  port (
    reset : in STD_LOGIC := 'X'; 
    write_data_falling : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    write_data_rising : in STD_LOGIC := 'X'; 
    ddr_dq_inout : inout STD_LOGIC; 
    read_data_in : out STD_LOGIC 
  );
end ddr_dq_iob_INST_12;

architecture Structure of ddr_dq_iob_INST_12 is
  signal clock_en : STD_LOGIC; 
  signal GND_186 : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : GND
    port map (
      G => GND_186
    );
  XST_VCC : VCC
    port map (
      P => clock_en
    );
  DQ_T : FDCE
    port map (
      D => enable_b,
      CE => clock_en,
      CLR => reset,
      C => clk270,
      Q => ddr_en
    );
  DDR_OUT : FDDRRSE
    port map (
      CE => clock_en,
      R => GND_186,
      S => GND_186,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_OBUFT : OBUFT
    port map (
      I => ddr_dq_q,
      T => ddr_en,
      O => ddr_dq_inout
    );
  DQ_IBUF : IBUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => write_en_val,
      O => enable_b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dq_iob_INST_11 is
  port (
    reset : in STD_LOGIC := 'X'; 
    write_data_falling : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    write_data_rising : in STD_LOGIC := 'X'; 
    ddr_dq_inout : inout STD_LOGIC; 
    read_data_in : out STD_LOGIC 
  );
end ddr_dq_iob_INST_11;

architecture Structure of ddr_dq_iob_INST_11 is
  signal clock_en : STD_LOGIC; 
  signal GND_187 : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : GND
    port map (
      G => GND_187
    );
  XST_VCC : VCC
    port map (
      P => clock_en
    );
  DQ_T : FDCE
    port map (
      D => enable_b,
      CE => clock_en,
      CLR => reset,
      C => clk270,
      Q => ddr_en
    );
  DDR_OUT : FDDRRSE
    port map (
      CE => clock_en,
      R => GND_187,
      S => GND_187,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_OBUFT : OBUFT
    port map (
      I => ddr_dq_q,
      T => ddr_en,
      O => ddr_dq_inout
    );
  DQ_IBUF : IBUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => write_en_val,
      O => enable_b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dq_iob_INST_10 is
  port (
    reset : in STD_LOGIC := 'X'; 
    write_data_falling : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    write_data_rising : in STD_LOGIC := 'X'; 
    ddr_dq_inout : inout STD_LOGIC; 
    read_data_in : out STD_LOGIC 
  );
end ddr_dq_iob_INST_10;

architecture Structure of ddr_dq_iob_INST_10 is
  signal clock_en : STD_LOGIC; 
  signal GND_188 : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : GND
    port map (
      G => GND_188
    );
  XST_VCC : VCC
    port map (
      P => clock_en
    );
  DQ_T : FDCE
    port map (
      D => enable_b,
      CE => clock_en,
      CLR => reset,
      C => clk270,
      Q => ddr_en
    );
  DDR_OUT : FDDRRSE
    port map (
      CE => clock_en,
      R => GND_188,
      S => GND_188,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_OBUFT : OBUFT
    port map (
      I => ddr_dq_q,
      T => ddr_en,
      O => ddr_dq_inout
    );
  DQ_IBUF : IBUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => write_en_val,
      O => enable_b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dq_iob_INST_9 is
  port (
    reset : in STD_LOGIC := 'X'; 
    write_data_falling : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    write_data_rising : in STD_LOGIC := 'X'; 
    ddr_dq_inout : inout STD_LOGIC; 
    read_data_in : out STD_LOGIC 
  );
end ddr_dq_iob_INST_9;

architecture Structure of ddr_dq_iob_INST_9 is
  signal clock_en : STD_LOGIC; 
  signal GND_189 : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : GND
    port map (
      G => GND_189
    );
  XST_VCC : VCC
    port map (
      P => clock_en
    );
  DQ_T : FDCE
    port map (
      D => enable_b,
      CE => clock_en,
      CLR => reset,
      C => clk270,
      Q => ddr_en
    );
  DDR_OUT : FDDRRSE
    port map (
      CE => clock_en,
      R => GND_189,
      S => GND_189,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_OBUFT : OBUFT
    port map (
      I => ddr_dq_q,
      T => ddr_en,
      O => ddr_dq_inout
    );
  DQ_IBUF : IBUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => write_en_val,
      O => enable_b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dq_iob_INST_8 is
  port (
    reset : in STD_LOGIC := 'X'; 
    write_data_falling : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    write_data_rising : in STD_LOGIC := 'X'; 
    ddr_dq_inout : inout STD_LOGIC; 
    read_data_in : out STD_LOGIC 
  );
end ddr_dq_iob_INST_8;

architecture Structure of ddr_dq_iob_INST_8 is
  signal clock_en : STD_LOGIC; 
  signal GND_190 : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : GND
    port map (
      G => GND_190
    );
  XST_VCC : VCC
    port map (
      P => clock_en
    );
  DQ_T : FDCE
    port map (
      D => enable_b,
      CE => clock_en,
      CLR => reset,
      C => clk270,
      Q => ddr_en
    );
  DDR_OUT : FDDRRSE
    port map (
      CE => clock_en,
      R => GND_190,
      S => GND_190,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_OBUFT : OBUFT
    port map (
      I => ddr_dq_q,
      T => ddr_en,
      O => ddr_dq_inout
    );
  DQ_IBUF : IBUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => write_en_val,
      O => enable_b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dq_iob_INST_7 is
  port (
    reset : in STD_LOGIC := 'X'; 
    write_data_falling : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    write_data_rising : in STD_LOGIC := 'X'; 
    ddr_dq_inout : inout STD_LOGIC; 
    read_data_in : out STD_LOGIC 
  );
end ddr_dq_iob_INST_7;

architecture Structure of ddr_dq_iob_INST_7 is
  signal clock_en : STD_LOGIC; 
  signal GND_191 : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : GND
    port map (
      G => GND_191
    );
  XST_VCC : VCC
    port map (
      P => clock_en
    );
  DQ_T : FDCE
    port map (
      D => enable_b,
      CE => clock_en,
      CLR => reset,
      C => clk270,
      Q => ddr_en
    );
  DDR_OUT : FDDRRSE
    port map (
      CE => clock_en,
      R => GND_191,
      S => GND_191,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_OBUFT : OBUFT
    port map (
      I => ddr_dq_q,
      T => ddr_en,
      O => ddr_dq_inout
    );
  DQ_IBUF : IBUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => write_en_val,
      O => enable_b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dq_iob_INST_6 is
  port (
    reset : in STD_LOGIC := 'X'; 
    write_data_falling : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    write_data_rising : in STD_LOGIC := 'X'; 
    ddr_dq_inout : inout STD_LOGIC; 
    read_data_in : out STD_LOGIC 
  );
end ddr_dq_iob_INST_6;

architecture Structure of ddr_dq_iob_INST_6 is
  signal clock_en : STD_LOGIC; 
  signal GND_192 : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : GND
    port map (
      G => GND_192
    );
  XST_VCC : VCC
    port map (
      P => clock_en
    );
  DQ_T : FDCE
    port map (
      D => enable_b,
      CE => clock_en,
      CLR => reset,
      C => clk270,
      Q => ddr_en
    );
  DDR_OUT : FDDRRSE
    port map (
      CE => clock_en,
      R => GND_192,
      S => GND_192,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_OBUFT : OBUFT
    port map (
      I => ddr_dq_q,
      T => ddr_en,
      O => ddr_dq_inout
    );
  DQ_IBUF : IBUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => write_en_val,
      O => enable_b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dq_iob_INST_5 is
  port (
    reset : in STD_LOGIC := 'X'; 
    write_data_falling : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    write_data_rising : in STD_LOGIC := 'X'; 
    ddr_dq_inout : inout STD_LOGIC; 
    read_data_in : out STD_LOGIC 
  );
end ddr_dq_iob_INST_5;

architecture Structure of ddr_dq_iob_INST_5 is
  signal clock_en : STD_LOGIC; 
  signal GND_193 : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : GND
    port map (
      G => GND_193
    );
  XST_VCC : VCC
    port map (
      P => clock_en
    );
  DQ_T : FDCE
    port map (
      D => enable_b,
      CE => clock_en,
      CLR => reset,
      C => clk270,
      Q => ddr_en
    );
  DDR_OUT : FDDRRSE
    port map (
      CE => clock_en,
      R => GND_193,
      S => GND_193,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_OBUFT : OBUFT
    port map (
      I => ddr_dq_q,
      T => ddr_en,
      O => ddr_dq_inout
    );
  DQ_IBUF : IBUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => write_en_val,
      O => enable_b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dq_iob_INST_4 is
  port (
    reset : in STD_LOGIC := 'X'; 
    write_data_falling : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    write_data_rising : in STD_LOGIC := 'X'; 
    ddr_dq_inout : inout STD_LOGIC; 
    read_data_in : out STD_LOGIC 
  );
end ddr_dq_iob_INST_4;

architecture Structure of ddr_dq_iob_INST_4 is
  signal clock_en : STD_LOGIC; 
  signal GND_194 : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : GND
    port map (
      G => GND_194
    );
  XST_VCC : VCC
    port map (
      P => clock_en
    );
  DQ_T : FDCE
    port map (
      D => enable_b,
      CE => clock_en,
      CLR => reset,
      C => clk270,
      Q => ddr_en
    );
  DDR_OUT : FDDRRSE
    port map (
      CE => clock_en,
      R => GND_194,
      S => GND_194,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_OBUFT : OBUFT
    port map (
      I => ddr_dq_q,
      T => ddr_en,
      O => ddr_dq_inout
    );
  DQ_IBUF : IBUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => write_en_val,
      O => enable_b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dq_iob_INST_3 is
  port (
    reset : in STD_LOGIC := 'X'; 
    write_data_falling : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    write_data_rising : in STD_LOGIC := 'X'; 
    ddr_dq_inout : inout STD_LOGIC; 
    read_data_in : out STD_LOGIC 
  );
end ddr_dq_iob_INST_3;

architecture Structure of ddr_dq_iob_INST_3 is
  signal clock_en : STD_LOGIC; 
  signal GND_195 : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : GND
    port map (
      G => GND_195
    );
  XST_VCC : VCC
    port map (
      P => clock_en
    );
  DQ_T : FDCE
    port map (
      D => enable_b,
      CE => clock_en,
      CLR => reset,
      C => clk270,
      Q => ddr_en
    );
  DDR_OUT : FDDRRSE
    port map (
      CE => clock_en,
      R => GND_195,
      S => GND_195,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_OBUFT : OBUFT
    port map (
      I => ddr_dq_q,
      T => ddr_en,
      O => ddr_dq_inout
    );
  DQ_IBUF : IBUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => write_en_val,
      O => enable_b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dq_iob_INST_2 is
  port (
    reset : in STD_LOGIC := 'X'; 
    write_data_falling : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    write_data_rising : in STD_LOGIC := 'X'; 
    ddr_dq_inout : inout STD_LOGIC; 
    read_data_in : out STD_LOGIC 
  );
end ddr_dq_iob_INST_2;

architecture Structure of ddr_dq_iob_INST_2 is
  signal clock_en : STD_LOGIC; 
  signal GND_196 : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : GND
    port map (
      G => GND_196
    );
  XST_VCC : VCC
    port map (
      P => clock_en
    );
  DQ_T : FDCE
    port map (
      D => enable_b,
      CE => clock_en,
      CLR => reset,
      C => clk270,
      Q => ddr_en
    );
  DDR_OUT : FDDRRSE
    port map (
      CE => clock_en,
      R => GND_196,
      S => GND_196,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_OBUFT : OBUFT
    port map (
      I => ddr_dq_q,
      T => ddr_en,
      O => ddr_dq_inout
    );
  DQ_IBUF : IBUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => write_en_val,
      O => enable_b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dq_iob_INST_1 is
  port (
    reset : in STD_LOGIC := 'X'; 
    write_data_falling : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    write_data_rising : in STD_LOGIC := 'X'; 
    ddr_dq_inout : inout STD_LOGIC; 
    read_data_in : out STD_LOGIC 
  );
end ddr_dq_iob_INST_1;

architecture Structure of ddr_dq_iob_INST_1 is
  signal clock_en : STD_LOGIC; 
  signal GND_197 : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : GND
    port map (
      G => GND_197
    );
  XST_VCC : VCC
    port map (
      P => clock_en
    );
  DQ_T : FDCE
    port map (
      D => enable_b,
      CE => clock_en,
      CLR => reset,
      C => clk270,
      Q => ddr_en
    );
  DDR_OUT : FDDRRSE
    port map (
      CE => clock_en,
      R => GND_197,
      S => GND_197,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_OBUFT : OBUFT
    port map (
      I => ddr_dq_q,
      T => ddr_en,
      O => ddr_dq_inout
    );
  DQ_IBUF : IBUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => write_en_val,
      O => enable_b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dq_iob is
  port (
    reset : in STD_LOGIC := 'X'; 
    write_data_falling : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    write_data_rising : in STD_LOGIC := 'X'; 
    ddr_dq_inout : inout STD_LOGIC; 
    read_data_in : out STD_LOGIC 
  );
end ddr_dq_iob;

architecture Structure of ddr_dq_iob is
  signal clock_en : STD_LOGIC; 
  signal GND_198 : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : GND
    port map (
      G => GND_198
    );
  XST_VCC : VCC
    port map (
      P => clock_en
    );
  DQ_T : FDCE
    port map (
      D => enable_b,
      CE => clock_en,
      CLR => reset,
      C => clk270,
      Q => ddr_en
    );
  DDR_OUT : FDDRRSE
    port map (
      CE => clock_en,
      R => GND_198,
      S => GND_198,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_OBUFT : OBUFT
    port map (
      I => ddr_dq_q,
      T => ddr_en,
      O => ddr_dq_inout
    );
  DQ_IBUF : IBUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => write_en_val,
      O => enable_b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dqs_iob_INST_1 is
  port (
    clk : in STD_LOGIC := 'X'; 
    ddr_dqs_reset : in STD_LOGIC := 'X'; 
    ddr_dqs_enable : in STD_LOGIC := 'X'; 
    clk180 : in STD_LOGIC := 'X'; 
    ddr_dqs_n : inout STD_LOGIC; 
    ddr_dqs : inout STD_LOGIC; 
    dqs : out STD_LOGIC 
  );
end ddr_dqs_iob_INST_1;

architecture Structure of ddr_dqs_iob_INST_1 is
  signal data1 : STD_LOGIC; 
  signal vcc_199 : STD_LOGIC; 
  signal dqs_q : STD_LOGIC; 
  signal ddr_dqs_enable1 : STD_LOGIC; 
  signal ddr_dqs_enable_b : STD_LOGIC; 
  signal gnd_200 : STD_LOGIC; 
  signal NLW_U5_Q_UNCONNECTED : STD_LOGIC; 
begin
  XST_VCC : VCC
    port map (
      P => vcc_199
    );
  XST_GND : GND
    port map (
      G => gnd_200
    );
  U1 : FD
    port map (
      D => ddr_dqs_enable_b,
      C => clk,
      Q => ddr_dqs_enable1
    );
  U2 : FDDRRSE
    port map (
      CE => vcc_199,
      R => gnd_200,
      S => gnd_200,
      C0 => clk180,
      C1 => clk,
      D0 => gnd_200,
      D1 => data1,
      Q => dqs_q
    );
  U5 : FDDRRSE
    port map (
      CE => vcc_199,
      R => gnd_200,
      S => gnd_200,
      C0 => clk180,
      C1 => clk,
      D0 => data1,
      D1 => gnd_200,
      Q => NLW_U5_Q_UNCONNECTED
    );
  U7 : IOBUFDS_BLVDS_25
    port map (
      I => dqs_q,
      T => ddr_dqs_enable1,
      O => dqs,
      IOB => ddr_dqs_n,
      IO => ddr_dqs
    );
  ddr_dqs_enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => ddr_dqs_enable,
      O => ddr_dqs_enable_b
    );
  data11 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => ddr_dqs_reset,
      O => data1
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr_dqs_iob is
  port (
    clk : in STD_LOGIC := 'X'; 
    ddr_dqs_reset : in STD_LOGIC := 'X'; 
    ddr_dqs_enable : in STD_LOGIC := 'X'; 
    clk180 : in STD_LOGIC := 'X'; 
    ddr_dqs_n : inout STD_LOGIC; 
    ddr_dqs : inout STD_LOGIC; 
    dqs : out STD_LOGIC 
  );
end ddr_dqs_iob;

architecture Structure of ddr_dqs_iob is
  signal data1 : STD_LOGIC; 
  signal vcc_201 : STD_LOGIC; 
  signal dqs_q : STD_LOGIC; 
  signal ddr_dqs_enable1 : STD_LOGIC; 
  signal ddr_dqs_enable_b : STD_LOGIC; 
  signal gnd_202 : STD_LOGIC; 
  signal NLW_U5_Q_UNCONNECTED : STD_LOGIC; 
begin
  XST_VCC : VCC
    port map (
      P => vcc_201
    );
  XST_GND : GND
    port map (
      G => gnd_202
    );
  U1 : FD
    port map (
      D => ddr_dqs_enable_b,
      C => clk,
      Q => ddr_dqs_enable1
    );
  U2 : FDDRRSE
    port map (
      CE => vcc_201,
      R => gnd_202,
      S => gnd_202,
      C0 => clk180,
      C1 => clk,
      D0 => gnd_202,
      D1 => data1,
      Q => dqs_q
    );
  U5 : FDDRRSE
    port map (
      CE => vcc_201,
      R => gnd_202,
      S => gnd_202,
      C0 => clk180,
      C1 => clk,
      D0 => data1,
      D1 => gnd_202,
      Q => NLW_U5_Q_UNCONNECTED
    );
  U7 : IOBUFDS_BLVDS_25
    port map (
      I => dqs_q,
      T => ddr_dqs_enable1,
      O => dqs,
      IOB => ddr_dqs_n,
      IO => ddr_dqs
    );
  ddr_dqs_enable_b1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => ddr_dqs_enable,
      O => ddr_dqs_enable_b
    );
  data11 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => ddr_dqs_reset,
      O => data1
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity data_path_iobs_16bit is
  port (
    clk : in STD_LOGIC := 'X'; 
    reset270_r : in STD_LOGIC := 'X'; 
    dqs_enable : in STD_LOGIC := 'X'; 
    clk180 : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    dqs_reset : in STD_LOGIC := 'X'; 
    dqs_int_delay_in0 : out STD_LOGIC; 
    dqs_int_delay_in1 : out STD_LOGIC; 
    write_data_falling : in STD_LOGIC_VECTOR ( 15 downto 0 ); 
    data_mask_f : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
    data_mask_r : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
    write_data_rising : in STD_LOGIC_VECTOR ( 15 downto 0 ); 
    ddr_dq : inout STD_LOGIC_VECTOR ( 15 downto 0 ); 
    ddr_dqs_n : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
    ddr_dqs : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
    dq : out STD_LOGIC_VECTOR ( 15 downto 0 ); 
    ddr_dm : out STD_LOGIC_VECTOR ( 1 downto 0 ) 
  );
end data_path_iobs_16bit;

architecture Structure of data_path_iobs_16bit is
  component ddr_dqs_iob
    port (
      clk : in STD_LOGIC := 'X'; 
      ddr_dqs_reset : in STD_LOGIC := 'X'; 
      ddr_dqs_enable : in STD_LOGIC := 'X'; 
      clk180 : in STD_LOGIC := 'X'; 
      ddr_dqs_n : inout STD_LOGIC; 
      ddr_dqs : inout STD_LOGIC; 
      dqs : out STD_LOGIC 
    );
  end component;
  component ddr_dqs_iob_INST_1
    port (
      clk : in STD_LOGIC := 'X'; 
      ddr_dqs_reset : in STD_LOGIC := 'X'; 
      ddr_dqs_enable : in STD_LOGIC := 'X'; 
      clk180 : in STD_LOGIC := 'X'; 
      ddr_dqs_n : inout STD_LOGIC; 
      ddr_dqs : inout STD_LOGIC; 
      dqs : out STD_LOGIC 
    );
  end component;
  component ddr_dq_iob
    port (
      reset : in STD_LOGIC := 'X'; 
      write_data_falling : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      write_data_rising : in STD_LOGIC := 'X'; 
      ddr_dq_inout : inout STD_LOGIC; 
      read_data_in : out STD_LOGIC 
    );
  end component;
  component ddr_dq_iob_INST_1
    port (
      reset : in STD_LOGIC := 'X'; 
      write_data_falling : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      write_data_rising : in STD_LOGIC := 'X'; 
      ddr_dq_inout : inout STD_LOGIC; 
      read_data_in : out STD_LOGIC 
    );
  end component;
  component ddr_dq_iob_INST_2
    port (
      reset : in STD_LOGIC := 'X'; 
      write_data_falling : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      write_data_rising : in STD_LOGIC := 'X'; 
      ddr_dq_inout : inout STD_LOGIC; 
      read_data_in : out STD_LOGIC 
    );
  end component;
  component ddr_dq_iob_INST_3
    port (
      reset : in STD_LOGIC := 'X'; 
      write_data_falling : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      write_data_rising : in STD_LOGIC := 'X'; 
      ddr_dq_inout : inout STD_LOGIC; 
      read_data_in : out STD_LOGIC 
    );
  end component;
  component ddr_dq_iob_INST_4
    port (
      reset : in STD_LOGIC := 'X'; 
      write_data_falling : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      write_data_rising : in STD_LOGIC := 'X'; 
      ddr_dq_inout : inout STD_LOGIC; 
      read_data_in : out STD_LOGIC 
    );
  end component;
  component ddr_dq_iob_INST_5
    port (
      reset : in STD_LOGIC := 'X'; 
      write_data_falling : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      write_data_rising : in STD_LOGIC := 'X'; 
      ddr_dq_inout : inout STD_LOGIC; 
      read_data_in : out STD_LOGIC 
    );
  end component;
  component ddr_dq_iob_INST_6
    port (
      reset : in STD_LOGIC := 'X'; 
      write_data_falling : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      write_data_rising : in STD_LOGIC := 'X'; 
      ddr_dq_inout : inout STD_LOGIC; 
      read_data_in : out STD_LOGIC 
    );
  end component;
  component ddr_dq_iob_INST_7
    port (
      reset : in STD_LOGIC := 'X'; 
      write_data_falling : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      write_data_rising : in STD_LOGIC := 'X'; 
      ddr_dq_inout : inout STD_LOGIC; 
      read_data_in : out STD_LOGIC 
    );
  end component;
  component ddr_dq_iob_INST_8
    port (
      reset : in STD_LOGIC := 'X'; 
      write_data_falling : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      write_data_rising : in STD_LOGIC := 'X'; 
      ddr_dq_inout : inout STD_LOGIC; 
      read_data_in : out STD_LOGIC 
    );
  end component;
  component ddr_dq_iob_INST_9
    port (
      reset : in STD_LOGIC := 'X'; 
      write_data_falling : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      write_data_rising : in STD_LOGIC := 'X'; 
      ddr_dq_inout : inout STD_LOGIC; 
      read_data_in : out STD_LOGIC 
    );
  end component;
  component ddr_dq_iob_INST_10
    port (
      reset : in STD_LOGIC := 'X'; 
      write_data_falling : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      write_data_rising : in STD_LOGIC := 'X'; 
      ddr_dq_inout : inout STD_LOGIC; 
      read_data_in : out STD_LOGIC 
    );
  end component;
  component ddr_dq_iob_INST_11
    port (
      reset : in STD_LOGIC := 'X'; 
      write_data_falling : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      write_data_rising : in STD_LOGIC := 'X'; 
      ddr_dq_inout : inout STD_LOGIC; 
      read_data_in : out STD_LOGIC 
    );
  end component;
  component ddr_dq_iob_INST_12
    port (
      reset : in STD_LOGIC := 'X'; 
      write_data_falling : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      write_data_rising : in STD_LOGIC := 'X'; 
      ddr_dq_inout : inout STD_LOGIC; 
      read_data_in : out STD_LOGIC 
    );
  end component;
  component ddr_dq_iob_INST_13
    port (
      reset : in STD_LOGIC := 'X'; 
      write_data_falling : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      write_data_rising : in STD_LOGIC := 'X'; 
      ddr_dq_inout : inout STD_LOGIC; 
      read_data_in : out STD_LOGIC 
    );
  end component;
  component ddr_dq_iob_INST_14
    port (
      reset : in STD_LOGIC := 'X'; 
      write_data_falling : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      write_data_rising : in STD_LOGIC := 'X'; 
      ddr_dq_inout : inout STD_LOGIC; 
      read_data_in : out STD_LOGIC 
    );
  end component;
  component ddr_dq_iob_INST_15
    port (
      reset : in STD_LOGIC := 'X'; 
      write_data_falling : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      write_data_rising : in STD_LOGIC := 'X'; 
      ddr_dq_inout : inout STD_LOGIC; 
      read_data_in : out STD_LOGIC 
    );
  end component;
  component ddr2_dm_16bit
    port (
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      mask_rising : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
      mask_falling : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
      ddr_dm : out STD_LOGIC_VECTOR ( 1 downto 0 ) 
    );
  end component;
begin
  ddr_dqs_iob1 : ddr_dqs_iob
    port map (
      clk => clk,
      ddr_dqs_reset => dqs_reset,
      ddr_dqs_enable => dqs_enable,
      clk180 => clk180,
      ddr_dqs_n => ddr_dqs_n(1),
      ddr_dqs => ddr_dqs(1),
      dqs => dqs_int_delay_in1
    );
  ddr_dqs_iob0 : ddr_dqs_iob_INST_1
    port map (
      clk => clk,
      ddr_dqs_reset => dqs_reset,
      ddr_dqs_enable => dqs_enable,
      clk180 => clk180,
      ddr_dqs_n => ddr_dqs_n(0),
      ddr_dqs => ddr_dqs(0),
      dqs => dqs_int_delay_in0
    );
  ddr_dq_iob15 : ddr_dq_iob
    port map (
      reset => reset270_r,
      write_data_falling => write_data_falling(15),
      clk90 => clk90,
      clk270 => clk270,
      write_en_val => write_en_val,
      write_data_rising => write_data_rising(15),
      ddr_dq_inout => ddr_dq(15),
      read_data_in => dq(15)
    );
  ddr_dq_iob14 : ddr_dq_iob_INST_1
    port map (
      reset => reset270_r,
      write_data_falling => write_data_falling(14),
      clk90 => clk90,
      clk270 => clk270,
      write_en_val => write_en_val,
      write_data_rising => write_data_rising(14),
      ddr_dq_inout => ddr_dq(14),
      read_data_in => dq(14)
    );
  ddr_dq_iob13 : ddr_dq_iob_INST_2
    port map (
      reset => reset270_r,
      write_data_falling => write_data_falling(13),
      clk90 => clk90,
      clk270 => clk270,
      write_en_val => write_en_val,
      write_data_rising => write_data_rising(13),
      ddr_dq_inout => ddr_dq(13),
      read_data_in => dq(13)
    );
  ddr_dq_iob12 : ddr_dq_iob_INST_3
    port map (
      reset => reset270_r,
      write_data_falling => write_data_falling(12),
      clk90 => clk90,
      clk270 => clk270,
      write_en_val => write_en_val,
      write_data_rising => write_data_rising(12),
      ddr_dq_inout => ddr_dq(12),
      read_data_in => dq(12)
    );
  ddr_dq_iob11 : ddr_dq_iob_INST_4
    port map (
      reset => reset270_r,
      write_data_falling => write_data_falling(11),
      clk90 => clk90,
      clk270 => clk270,
      write_en_val => write_en_val,
      write_data_rising => write_data_rising(11),
      ddr_dq_inout => ddr_dq(11),
      read_data_in => dq(11)
    );
  ddr_dq_iob10 : ddr_dq_iob_INST_5
    port map (
      reset => reset270_r,
      write_data_falling => write_data_falling(10),
      clk90 => clk90,
      clk270 => clk270,
      write_en_val => write_en_val,
      write_data_rising => write_data_rising(10),
      ddr_dq_inout => ddr_dq(10),
      read_data_in => dq(10)
    );
  ddr_dq_iob9 : ddr_dq_iob_INST_6
    port map (
      reset => reset270_r,
      write_data_falling => write_data_falling(9),
      clk90 => clk90,
      clk270 => clk270,
      write_en_val => write_en_val,
      write_data_rising => write_data_rising(9),
      ddr_dq_inout => ddr_dq(9),
      read_data_in => dq(9)
    );
  ddr_dq_iob8 : ddr_dq_iob_INST_7
    port map (
      reset => reset270_r,
      write_data_falling => write_data_falling(8),
      clk90 => clk90,
      clk270 => clk270,
      write_en_val => write_en_val,
      write_data_rising => write_data_rising(8),
      ddr_dq_inout => ddr_dq(8),
      read_data_in => dq(8)
    );
  ddr_dq_iob7 : ddr_dq_iob_INST_8
    port map (
      reset => reset270_r,
      write_data_falling => write_data_falling(7),
      clk90 => clk90,
      clk270 => clk270,
      write_en_val => write_en_val,
      write_data_rising => write_data_rising(7),
      ddr_dq_inout => ddr_dq(7),
      read_data_in => dq(7)
    );
  ddr_dq_iob6 : ddr_dq_iob_INST_9
    port map (
      reset => reset270_r,
      write_data_falling => write_data_falling(6),
      clk90 => clk90,
      clk270 => clk270,
      write_en_val => write_en_val,
      write_data_rising => write_data_rising(6),
      ddr_dq_inout => ddr_dq(6),
      read_data_in => dq(6)
    );
  ddr_dq_iob5 : ddr_dq_iob_INST_10
    port map (
      reset => reset270_r,
      write_data_falling => write_data_falling(5),
      clk90 => clk90,
      clk270 => clk270,
      write_en_val => write_en_val,
      write_data_rising => write_data_rising(5),
      ddr_dq_inout => ddr_dq(5),
      read_data_in => dq(5)
    );
  ddr_dq_iob4 : ddr_dq_iob_INST_11
    port map (
      reset => reset270_r,
      write_data_falling => write_data_falling(4),
      clk90 => clk90,
      clk270 => clk270,
      write_en_val => write_en_val,
      write_data_rising => write_data_rising(4),
      ddr_dq_inout => ddr_dq(4),
      read_data_in => dq(4)
    );
  ddr_dq_iob3 : ddr_dq_iob_INST_12
    port map (
      reset => reset270_r,
      write_data_falling => write_data_falling(3),
      clk90 => clk90,
      clk270 => clk270,
      write_en_val => write_en_val,
      write_data_rising => write_data_rising(3),
      ddr_dq_inout => ddr_dq(3),
      read_data_in => dq(3)
    );
  ddr_dq_iob2 : ddr_dq_iob_INST_13
    port map (
      reset => reset270_r,
      write_data_falling => write_data_falling(2),
      clk90 => clk90,
      clk270 => clk270,
      write_en_val => write_en_val,
      write_data_rising => write_data_rising(2),
      ddr_dq_inout => ddr_dq(2),
      read_data_in => dq(2)
    );
  ddr_dq_iob1 : ddr_dq_iob_INST_14
    port map (
      reset => reset270_r,
      write_data_falling => write_data_falling(1),
      clk90 => clk90,
      clk270 => clk270,
      write_en_val => write_en_val,
      write_data_rising => write_data_rising(1),
      ddr_dq_inout => ddr_dq(1),
      read_data_in => dq(1)
    );
  ddr_dq_iob0 : ddr_dq_iob_INST_15
    port map (
      reset => reset270_r,
      write_data_falling => write_data_falling(0),
      clk90 => clk90,
      clk270 => clk270,
      write_en_val => write_en_val,
      write_data_rising => write_data_rising(0),
      ddr_dq_inout => ddr_dq(0),
      read_data_in => dq(0)
    );
  ddr2_dm0 : ddr2_dm_16bit
    port map (
      clk90 => clk90,
      clk270 => clk270,
      mask_rising(1) => data_mask_r(1),
      mask_rising(0) => data_mask_r(0),
      mask_falling(1) => data_mask_f(1),
      mask_falling(0) => data_mask_f(0),
      ddr_dm(1) => ddr_dm(1),
      ddr_dm(0) => ddr_dm(0)
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity iobs_16bit_00 is
  port (
    clk : in STD_LOGIC := 'X'; 
    ddr_ODT_cntrl : in STD_LOGIC := 'X'; 
    ddr_csb_cntrl : in STD_LOGIC := 'X'; 
    reset270_r : in STD_LOGIC := 'X'; 
    ddr_web_cntrl : in STD_LOGIC := 'X'; 
    dqs_enable : in STD_LOGIC := 'X'; 
    clk180 : in STD_LOGIC := 'X'; 
    rst_dqs_div_in : in STD_LOGIC := 'X'; 
    ddr_casb_cntrl : in STD_LOGIC := 'X'; 
    clk90 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    write_en_val : in STD_LOGIC := 'X'; 
    ddr_cke_cntrl : in STD_LOGIC := 'X'; 
    dqs_reset : in STD_LOGIC := 'X'; 
    ddr_rasb_cntrl : in STD_LOGIC := 'X'; 
    rst_dqs_div_int : in STD_LOGIC := 'X'; 
    rst_dqs_div : out STD_LOGIC; 
    ddr_cke : out STD_LOGIC; 
    ddr_csb : out STD_LOGIC; 
    ddr_rasb : out STD_LOGIC; 
    ddr_web : out STD_LOGIC; 
    ddr2_clk0 : out STD_LOGIC; 
    ddr_casb : out STD_LOGIC; 
    ddr2_clk0b : out STD_LOGIC; 
    ddr_ODT0 : out STD_LOGIC; 
    rst_dqs_div_out : out STD_LOGIC; 
    dqs_int_delay_in0 : out STD_LOGIC; 
    dqs_int_delay_in1 : out STD_LOGIC; 
    write_data_falling : in STD_LOGIC_VECTOR ( 15 downto 0 ); 
    ddr_ba_cntrl : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
    data_mask_f : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
    data_mask_r : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
    write_data_rising : in STD_LOGIC_VECTOR ( 15 downto 0 ); 
    ddr_address_cntrl : in STD_LOGIC_VECTOR ( 12 downto 0 ); 
    ddr_dq : inout STD_LOGIC_VECTOR ( 15 downto 0 ); 
    ddr_dqs_n : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
    ddr_dqs : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
    dq : out STD_LOGIC_VECTOR ( 15 downto 0 ); 
    ddr_address : out STD_LOGIC_VECTOR ( 12 downto 0 ); 
    ddr_ba : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
    ddr_dm : out STD_LOGIC_VECTOR ( 1 downto 0 ) 
  );
end iobs_16bit_00;

architecture Structure of iobs_16bit_00 is
  component data_path_iobs_16bit
    port (
      clk : in STD_LOGIC := 'X'; 
      reset270_r : in STD_LOGIC := 'X'; 
      dqs_enable : in STD_LOGIC := 'X'; 
      clk180 : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      dqs_reset : in STD_LOGIC := 'X'; 
      dqs_int_delay_in0 : out STD_LOGIC; 
      dqs_int_delay_in1 : out STD_LOGIC; 
      write_data_falling : in STD_LOGIC_VECTOR ( 15 downto 0 ); 
      data_mask_f : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
      data_mask_r : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
      write_data_rising : in STD_LOGIC_VECTOR ( 15 downto 0 ); 
      ddr_dq : inout STD_LOGIC_VECTOR ( 15 downto 0 ); 
      ddr_dqs_n : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
      ddr_dqs : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
      dq : out STD_LOGIC_VECTOR ( 15 downto 0 ); 
      ddr_dm : out STD_LOGIC_VECTOR ( 1 downto 0 ) 
    );
  end component;
  component controller_iobs_16bit_00
    port (
      ddr_ODT_cntrl : in STD_LOGIC := 'X'; 
      ddr_csb_cntrl : in STD_LOGIC := 'X'; 
      ddr_web_cntrl : in STD_LOGIC := 'X'; 
      clk0 : in STD_LOGIC := 'X'; 
      clk180 : in STD_LOGIC := 'X'; 
      rst_dqs_div_in : in STD_LOGIC := 'X'; 
      ddr_casb_cntrl : in STD_LOGIC := 'X'; 
      ddr_cke_cntrl : in STD_LOGIC := 'X'; 
      ddr_rasb_cntrl : in STD_LOGIC := 'X'; 
      rst_dqs_div_int : in STD_LOGIC := 'X'; 
      rst_dqs_div : out STD_LOGIC; 
      ddr_cke : out STD_LOGIC; 
      ddr_csb : out STD_LOGIC; 
      ddr_rasb : out STD_LOGIC; 
      ddr_web : out STD_LOGIC; 
      ddr_casb : out STD_LOGIC; 
      ddr_ODT0 : out STD_LOGIC; 
      rst_dqs_div_out : out STD_LOGIC; 
      ddr_ba_cntrl : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
      ddr_address_cntrl : in STD_LOGIC_VECTOR ( 12 downto 0 ); 
      ddr_address : out STD_LOGIC_VECTOR ( 12 downto 0 ); 
      ddr_ba : out STD_LOGIC_VECTOR ( 1 downto 0 ) 
    );
  end component;
  component infrastructure_iobs_16bit
    port (
      clk0 : in STD_LOGIC := 'X'; 
      clk180 : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      ddr2_clk0 : out STD_LOGIC; 
      ddr2_clk0b : out STD_LOGIC 
    );
  end component;
begin
  data_path_iobs0 : data_path_iobs_16bit
    port map (
      clk => clk,
      reset270_r => reset270_r,
      dqs_enable => dqs_enable,
      clk180 => clk180,
      clk90 => clk90,
      clk270 => clk270,
      write_en_val => write_en_val,
      dqs_reset => dqs_reset,
      dqs_int_delay_in0 => dqs_int_delay_in0,
      dqs_int_delay_in1 => dqs_int_delay_in1,
      write_data_falling(15) => write_data_falling(15),
      write_data_falling(14) => write_data_falling(14),
      write_data_falling(13) => write_data_falling(13),
      write_data_falling(12) => write_data_falling(12),
      write_data_falling(11) => write_data_falling(11),
      write_data_falling(10) => write_data_falling(10),
      write_data_falling(9) => write_data_falling(9),
      write_data_falling(8) => write_data_falling(8),
      write_data_falling(7) => write_data_falling(7),
      write_data_falling(6) => write_data_falling(6),
      write_data_falling(5) => write_data_falling(5),
      write_data_falling(4) => write_data_falling(4),
      write_data_falling(3) => write_data_falling(3),
      write_data_falling(2) => write_data_falling(2),
      write_data_falling(1) => write_data_falling(1),
      write_data_falling(0) => write_data_falling(0),
      data_mask_f(1) => data_mask_f(1),
      data_mask_f(0) => data_mask_f(0),
      data_mask_r(1) => data_mask_r(1),
      data_mask_r(0) => data_mask_r(0),
      write_data_rising(15) => write_data_rising(15),
      write_data_rising(14) => write_data_rising(14),
      write_data_rising(13) => write_data_rising(13),
      write_data_rising(12) => write_data_rising(12),
      write_data_rising(11) => write_data_rising(11),
      write_data_rising(10) => write_data_rising(10),
      write_data_rising(9) => write_data_rising(9),
      write_data_rising(8) => write_data_rising(8),
      write_data_rising(7) => write_data_rising(7),
      write_data_rising(6) => write_data_rising(6),
      write_data_rising(5) => write_data_rising(5),
      write_data_rising(4) => write_data_rising(4),
      write_data_rising(3) => write_data_rising(3),
      write_data_rising(2) => write_data_rising(2),
      write_data_rising(1) => write_data_rising(1),
      write_data_rising(0) => write_data_rising(0),
      ddr_dq(15) => ddr_dq(15),
      ddr_dq(14) => ddr_dq(14),
      ddr_dq(13) => ddr_dq(13),
      ddr_dq(12) => ddr_dq(12),
      ddr_dq(11) => ddr_dq(11),
      ddr_dq(10) => ddr_dq(10),
      ddr_dq(9) => ddr_dq(9),
      ddr_dq(8) => ddr_dq(8),
      ddr_dq(7) => ddr_dq(7),
      ddr_dq(6) => ddr_dq(6),
      ddr_dq(5) => ddr_dq(5),
      ddr_dq(4) => ddr_dq(4),
      ddr_dq(3) => ddr_dq(3),
      ddr_dq(2) => ddr_dq(2),
      ddr_dq(1) => ddr_dq(1),
      ddr_dq(0) => ddr_dq(0),
      ddr_dqs_n(1) => ddr_dqs_n(1),
      ddr_dqs_n(0) => ddr_dqs_n(0),
      ddr_dqs(1) => ddr_dqs(1),
      ddr_dqs(0) => ddr_dqs(0),
      dq(15) => dq(15),
      dq(14) => dq(14),
      dq(13) => dq(13),
      dq(12) => dq(12),
      dq(11) => dq(11),
      dq(10) => dq(10),
      dq(9) => dq(9),
      dq(8) => dq(8),
      dq(7) => dq(7),
      dq(6) => dq(6),
      dq(5) => dq(5),
      dq(4) => dq(4),
      dq(3) => dq(3),
      dq(2) => dq(2),
      dq(1) => dq(1),
      dq(0) => dq(0),
      ddr_dm(1) => ddr_dm(1),
      ddr_dm(0) => ddr_dm(0)
    );
  controller_iobs0 : controller_iobs_16bit_00
    port map (
      ddr_ODT_cntrl => ddr_ODT_cntrl,
      ddr_csb_cntrl => ddr_csb_cntrl,
      ddr_web_cntrl => ddr_web_cntrl,
      clk0 => clk,
      clk180 => clk180,
      rst_dqs_div_in => rst_dqs_div_in,
      ddr_casb_cntrl => ddr_casb_cntrl,
      ddr_cke_cntrl => ddr_cke_cntrl,
      ddr_rasb_cntrl => ddr_rasb_cntrl,
      rst_dqs_div_int => rst_dqs_div_int,
      rst_dqs_div => rst_dqs_div,
      ddr_cke => ddr_cke,
      ddr_csb => ddr_csb,
      ddr_rasb => ddr_rasb,
      ddr_web => ddr_web,
      ddr_casb => ddr_casb,
      ddr_ODT0 => ddr_ODT0,
      rst_dqs_div_out => rst_dqs_div_out,
      ddr_ba_cntrl(1) => ddr_ba_cntrl(1),
      ddr_ba_cntrl(0) => ddr_ba_cntrl(0),
      ddr_address_cntrl(12) => ddr_address_cntrl(12),
      ddr_address_cntrl(11) => ddr_address_cntrl(11),
      ddr_address_cntrl(10) => ddr_address_cntrl(10),
      ddr_address_cntrl(9) => ddr_address_cntrl(9),
      ddr_address_cntrl(8) => ddr_address_cntrl(8),
      ddr_address_cntrl(7) => ddr_address_cntrl(7),
      ddr_address_cntrl(6) => ddr_address_cntrl(6),
      ddr_address_cntrl(5) => ddr_address_cntrl(5),
      ddr_address_cntrl(4) => ddr_address_cntrl(4),
      ddr_address_cntrl(3) => ddr_address_cntrl(3),
      ddr_address_cntrl(2) => ddr_address_cntrl(2),
      ddr_address_cntrl(1) => ddr_address_cntrl(1),
      ddr_address_cntrl(0) => ddr_address_cntrl(0),
      ddr_address(12) => ddr_address(12),
      ddr_address(11) => ddr_address(11),
      ddr_address(10) => ddr_address(10),
      ddr_address(9) => ddr_address(9),
      ddr_address(8) => ddr_address(8),
      ddr_address(7) => ddr_address(7),
      ddr_address(6) => ddr_address(6),
      ddr_address(5) => ddr_address(5),
      ddr_address(4) => ddr_address(4),
      ddr_address(3) => ddr_address(3),
      ddr_address(2) => ddr_address(2),
      ddr_address(1) => ddr_address(1),
      ddr_address(0) => ddr_address(0),
      ddr_ba(1) => ddr_ba(1),
      ddr_ba(0) => ddr_ba(0)
    );
  infrastructure_iobs0 : infrastructure_iobs_16bit
    port map (
      clk0 => clk,
      clk180 => clk180,
      clk90 => clk90,
      clk270 => clk270,
      ddr2_clk0 => ddr2_clk0,
      ddr2_clk0b => ddr2_clk0b
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity ddr2_top_16bit_00 is
  port (
    clk_int : in STD_LOGIC := 'X'; 
    sys_rst : in STD_LOGIC := 'X'; 
    sys_rst90 : in STD_LOGIC := 'X'; 
    wait_200us : in STD_LOGIC := 'X'; 
    reset_in : in STD_LOGIC := 'X'; 
    clk90_int : in STD_LOGIC := 'X'; 
    sys_rst180 : in STD_LOGIC := 'X'; 
    clk180 : in STD_LOGIC := 'X'; 
    rst_dqs_div_in : in STD_LOGIC := 'X'; 
    sys_rst270 : in STD_LOGIC := 'X'; 
    clk270 : in STD_LOGIC := 'X'; 
    dip1 : in STD_LOGIC := 'X'; 
    dip3 : in STD_LOGIC := 'X'; 
    burst_done : in STD_LOGIC := 'X'; 
    ddr_cke : out STD_LOGIC; 
    ar_done : out STD_LOGIC; 
    auto_ref_req : out STD_LOGIC; 
    init_val : out STD_LOGIC; 
    ddr_csb : out STD_LOGIC; 
    ddr_rasb : out STD_LOGIC; 
    ddr_web : out STD_LOGIC; 
    ddr2_clk0 : out STD_LOGIC; 
    ddr_casb : out STD_LOGIC; 
    ddr2_clk0b : out STD_LOGIC; 
    ddr_ODT0 : out STD_LOGIC; 
    rst_dqs_div_out : out STD_LOGIC; 
    user_cmd_ack : out STD_LOGIC; 
    user_data_valid : out STD_LOGIC; 
    user_input_address : in STD_LOGIC_VECTOR ( 22 downto 0 ); 
    user_data_mask : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    user_input_data : in STD_LOGIC_VECTOR ( 31 downto 0 ); 
    user_command_register : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    delay_sel_val : in STD_LOGIC_VECTOR ( 4 downto 0 ); 
    user_config_register1 : in STD_LOGIC_VECTOR ( 14 downto 0 ); 
    user_config_register2 : in STD_LOGIC_VECTOR ( 12 downto 0 ); 
    user_bank_address : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
    ddr_dq : inout STD_LOGIC_VECTOR ( 15 downto 0 ); 
    ddr_dqs_n : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
    ddr_dqs : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
    ddr_address : out STD_LOGIC_VECTOR ( 12 downto 0 ); 
    ddr_ba : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
    user_output_data : out STD_LOGIC_VECTOR ( 31 downto 0 ); 
    ddr_dm : out STD_LOGIC_VECTOR ( 1 downto 0 ) 
  );
end ddr2_top_16bit_00;

architecture Structure of ddr2_top_16bit_00 is
  component iobs_16bit_00
    port (
      clk : in STD_LOGIC := 'X'; 
      ddr_ODT_cntrl : in STD_LOGIC := 'X'; 
      ddr_csb_cntrl : in STD_LOGIC := 'X'; 
      reset270_r : in STD_LOGIC := 'X'; 
      ddr_web_cntrl : in STD_LOGIC := 'X'; 
      dqs_enable : in STD_LOGIC := 'X'; 
      clk180 : in STD_LOGIC := 'X'; 
      rst_dqs_div_in : in STD_LOGIC := 'X'; 
      ddr_casb_cntrl : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      write_en_val : in STD_LOGIC := 'X'; 
      ddr_cke_cntrl : in STD_LOGIC := 'X'; 
      dqs_reset : in STD_LOGIC := 'X'; 
      ddr_rasb_cntrl : in STD_LOGIC := 'X'; 
      rst_dqs_div_int : in STD_LOGIC := 'X'; 
      rst_dqs_div : out STD_LOGIC; 
      ddr_cke : out STD_LOGIC; 
      ddr_csb : out STD_LOGIC; 
      ddr_rasb : out STD_LOGIC; 
      ddr_web : out STD_LOGIC; 
      ddr2_clk0 : out STD_LOGIC; 
      ddr_casb : out STD_LOGIC; 
      ddr2_clk0b : out STD_LOGIC; 
      ddr_ODT0 : out STD_LOGIC; 
      rst_dqs_div_out : out STD_LOGIC; 
      dqs_int_delay_in0 : out STD_LOGIC; 
      dqs_int_delay_in1 : out STD_LOGIC; 
      write_data_falling : in STD_LOGIC_VECTOR ( 15 downto 0 ); 
      ddr_ba_cntrl : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
      data_mask_f : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
      data_mask_r : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
      write_data_rising : in STD_LOGIC_VECTOR ( 15 downto 0 ); 
      ddr_address_cntrl : in STD_LOGIC_VECTOR ( 12 downto 0 ); 
      ddr_dq : inout STD_LOGIC_VECTOR ( 15 downto 0 ); 
      ddr_dqs_n : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
      ddr_dqs : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
      dq : out STD_LOGIC_VECTOR ( 15 downto 0 ); 
      ddr_address : out STD_LOGIC_VECTOR ( 12 downto 0 ); 
      ddr_ba : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
      ddr_dm : out STD_LOGIC_VECTOR ( 1 downto 0 ) 
    );
  end component;
  component controller_16bit_00
    port (
      clk : in STD_LOGIC := 'X'; 
      rst0 : in STD_LOGIC := 'X'; 
      wait_200us : in STD_LOGIC := 'X'; 
      clk180 : in STD_LOGIC := 'X'; 
      dip1 : in STD_LOGIC := 'X'; 
      dip3 : in STD_LOGIC := 'X'; 
      burst_done : in STD_LOGIC := 'X'; 
      rst180 : in STD_LOGIC := 'X'; 
      ddr_ODT_cntrl : out STD_LOGIC; 
      ddr_csb_cntrl : out STD_LOGIC; 
      cmd_ack : out STD_LOGIC; 
      auto_ref_req : out STD_LOGIC; 
      ar_done : out STD_LOGIC; 
      ddr_web_cntrl : out STD_LOGIC; 
      dqs_enable : out STD_LOGIC; 
      ddr_casb_cntrl : out STD_LOGIC; 
      rst_calib : out STD_LOGIC; 
      ddr_cke_cntrl : out STD_LOGIC; 
      init : out STD_LOGIC; 
      dqs_reset : out STD_LOGIC; 
      ddr_rasb_cntrl : out STD_LOGIC; 
      write_enable : out STD_LOGIC; 
      rst_dqs_div_int : out STD_LOGIC; 
      address : in STD_LOGIC_VECTOR ( 22 downto 0 ); 
      config_register1 : in STD_LOGIC_VECTOR ( 14 downto 0 ); 
      config_register2 : in STD_LOGIC_VECTOR ( 12 downto 0 ); 
      bank_address : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
      command_register : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      ddr_ba_cntrl : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
      ddr_address_cntrl : out STD_LOGIC_VECTOR ( 12 downto 0 ) 
    );
  end component;
  component data_path_16bit_rl
    port (
      rst_dqs_div : in STD_LOGIC := 'X'; 
      clk : in STD_LOGIC := 'X'; 
      reset270 : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      clk180 : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      reset90 : in STD_LOGIC := 'X'; 
      dqs_int_delay_in0 : in STD_LOGIC := 'X'; 
      dqs_int_delay_in1 : in STD_LOGIC := 'X'; 
      write_enable : in STD_LOGIC := 'X'; 
      reset180 : in STD_LOGIC := 'X'; 
      reset270_r_val : out STD_LOGIC; 
      u_data_val : out STD_LOGIC; 
      write_en_val : out STD_LOGIC; 
      dq : in STD_LOGIC_VECTOR ( 15 downto 0 ); 
      delay_sel : in STD_LOGIC_VECTOR ( 4 downto 0 ); 
      user_data_mask : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      user_input_data : in STD_LOGIC_VECTOR ( 31 downto 0 ); 
      write_data_falling : out STD_LOGIC_VECTOR ( 15 downto 0 ); 
      data_mask_f : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
      data_mask_r : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
      write_data_rising : out STD_LOGIC_VECTOR ( 15 downto 0 ); 
      user_output_data : out STD_LOGIC_VECTOR ( 31 downto 0 ) 
    );
  end component;
  component infrastructure
    port (
      clk_int : in STD_LOGIC := 'X'; 
      sys_rst : in STD_LOGIC := 'X'; 
      rst_calib1 : in STD_LOGIC := 'X'; 
      delay_sel_val : in STD_LOGIC_VECTOR ( 4 downto 0 ); 
      delay_sel_val1_val : out STD_LOGIC_VECTOR ( 4 downto 0 ) 
    );
  end component;
  signal ddr_ODT_cntrl : STD_LOGIC; 
  signal ddr_csb_cntrl : STD_LOGIC; 
  signal reset270_r : STD_LOGIC; 
  signal ddr_web_cntrl : STD_LOGIC; 
  signal dqs_div_rst : STD_LOGIC; 
  signal dqs_enable : STD_LOGIC; 
  signal ddr_casb_cntrl : STD_LOGIC; 
  signal write_en_val : STD_LOGIC; 
  signal rst_calib : STD_LOGIC; 
  signal ddr_cke_cntrl : STD_LOGIC; 
  signal dqs_reset : STD_LOGIC; 
  signal ddr_rasb_cntrl : STD_LOGIC; 
  signal dqs_int_delay_in0 : STD_LOGIC; 
  signal dqs_int_delay_in1 : STD_LOGIC; 
  signal write_enable : STD_LOGIC; 
  signal rst_dqs_div_int : STD_LOGIC; 
  signal NLW_controller0_dip1_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_dip3_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_address_22_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_address_21_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_address_20_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_address_19_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_address_18_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_address_17_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_address_16_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_address_15_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_address_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_address_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_address_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_address_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_address_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_address_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_address_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_config_register1_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_config_register1_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_config_register1_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_config_register1_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_config_register1_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_config_register1_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_config_register1_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_config_register2_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_config_register2_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_config_register2_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_config_register2_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_config_register2_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_config_register2_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_config_register2_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_config_register2_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_config_register2_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_bank_address_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_controller0_bank_address_0_UNCONNECTED : STD_LOGIC; 
  signal ddr_ba_cntrl : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal ddr_address_cntrl : STD_LOGIC_VECTOR ( 12 downto 0 ); 
  signal delay_sel : STD_LOGIC_VECTOR ( 4 downto 0 ); 
  signal dq : STD_LOGIC_VECTOR ( 15 downto 0 ); 
  signal data_mask_f : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal data_mask_r : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal write_data_falling : STD_LOGIC_VECTOR ( 15 downto 0 ); 
  signal write_data_rising : STD_LOGIC_VECTOR ( 15 downto 0 ); 
begin
  iobs0 : iobs_16bit_00
    port map (
      clk => clk_int,
      ddr_ODT_cntrl => ddr_ODT_cntrl,
      ddr_csb_cntrl => ddr_csb_cntrl,
      reset270_r => reset270_r,
      ddr_web_cntrl => ddr_web_cntrl,
      dqs_enable => dqs_enable,
      clk180 => clk180,
      rst_dqs_div_in => rst_dqs_div_in,
      ddr_casb_cntrl => ddr_casb_cntrl,
      clk90 => clk90_int,
      clk270 => clk270,
      write_en_val => write_en_val,
      ddr_cke_cntrl => ddr_cke_cntrl,
      dqs_reset => dqs_reset,
      ddr_rasb_cntrl => ddr_rasb_cntrl,
      rst_dqs_div_int => rst_dqs_div_int,
      rst_dqs_div => dqs_div_rst,
      ddr_cke => ddr_cke,
      ddr_csb => ddr_csb,
      ddr_rasb => ddr_rasb,
      ddr_web => ddr_web,
      ddr2_clk0 => ddr2_clk0,
      ddr_casb => ddr_casb,
      ddr2_clk0b => ddr2_clk0b,
      ddr_ODT0 => ddr_ODT0,
      rst_dqs_div_out => rst_dqs_div_out,
      dqs_int_delay_in0 => dqs_int_delay_in0,
      dqs_int_delay_in1 => dqs_int_delay_in1,
      write_data_falling(15) => write_data_falling(15),
      write_data_falling(14) => write_data_falling(14),
      write_data_falling(13) => write_data_falling(13),
      write_data_falling(12) => write_data_falling(12),
      write_data_falling(11) => write_data_falling(11),
      write_data_falling(10) => write_data_falling(10),
      write_data_falling(9) => write_data_falling(9),
      write_data_falling(8) => write_data_falling(8),
      write_data_falling(7) => write_data_falling(7),
      write_data_falling(6) => write_data_falling(6),
      write_data_falling(5) => write_data_falling(5),
      write_data_falling(4) => write_data_falling(4),
      write_data_falling(3) => write_data_falling(3),
      write_data_falling(2) => write_data_falling(2),
      write_data_falling(1) => write_data_falling(1),
      write_data_falling(0) => write_data_falling(0),
      ddr_ba_cntrl(1) => ddr_ba_cntrl(1),
      ddr_ba_cntrl(0) => ddr_ba_cntrl(0),
      data_mask_f(1) => data_mask_f(1),
      data_mask_f(0) => data_mask_f(0),
      data_mask_r(1) => data_mask_r(1),
      data_mask_r(0) => data_mask_r(0),
      write_data_rising(15) => write_data_rising(15),
      write_data_rising(14) => write_data_rising(14),
      write_data_rising(13) => write_data_rising(13),
      write_data_rising(12) => write_data_rising(12),
      write_data_rising(11) => write_data_rising(11),
      write_data_rising(10) => write_data_rising(10),
      write_data_rising(9) => write_data_rising(9),
      write_data_rising(8) => write_data_rising(8),
      write_data_rising(7) => write_data_rising(7),
      write_data_rising(6) => write_data_rising(6),
      write_data_rising(5) => write_data_rising(5),
      write_data_rising(4) => write_data_rising(4),
      write_data_rising(3) => write_data_rising(3),
      write_data_rising(2) => write_data_rising(2),
      write_data_rising(1) => write_data_rising(1),
      write_data_rising(0) => write_data_rising(0),
      ddr_address_cntrl(12) => ddr_address_cntrl(12),
      ddr_address_cntrl(11) => ddr_address_cntrl(11),
      ddr_address_cntrl(10) => ddr_address_cntrl(10),
      ddr_address_cntrl(9) => ddr_address_cntrl(9),
      ddr_address_cntrl(8) => ddr_address_cntrl(8),
      ddr_address_cntrl(7) => ddr_address_cntrl(7),
      ddr_address_cntrl(6) => ddr_address_cntrl(6),
      ddr_address_cntrl(5) => ddr_address_cntrl(5),
      ddr_address_cntrl(4) => ddr_address_cntrl(4),
      ddr_address_cntrl(3) => ddr_address_cntrl(3),
      ddr_address_cntrl(2) => ddr_address_cntrl(2),
      ddr_address_cntrl(1) => ddr_address_cntrl(1),
      ddr_address_cntrl(0) => ddr_address_cntrl(0),
      ddr_dq(15) => ddr_dq(15),
      ddr_dq(14) => ddr_dq(14),
      ddr_dq(13) => ddr_dq(13),
      ddr_dq(12) => ddr_dq(12),
      ddr_dq(11) => ddr_dq(11),
      ddr_dq(10) => ddr_dq(10),
      ddr_dq(9) => ddr_dq(9),
      ddr_dq(8) => ddr_dq(8),
      ddr_dq(7) => ddr_dq(7),
      ddr_dq(6) => ddr_dq(6),
      ddr_dq(5) => ddr_dq(5),
      ddr_dq(4) => ddr_dq(4),
      ddr_dq(3) => ddr_dq(3),
      ddr_dq(2) => ddr_dq(2),
      ddr_dq(1) => ddr_dq(1),
      ddr_dq(0) => ddr_dq(0),
      ddr_dqs_n(1) => ddr_dqs_n(1),
      ddr_dqs_n(0) => ddr_dqs_n(0),
      ddr_dqs(1) => ddr_dqs(1),
      ddr_dqs(0) => ddr_dqs(0),
      dq(15) => dq(15),
      dq(14) => dq(14),
      dq(13) => dq(13),
      dq(12) => dq(12),
      dq(11) => dq(11),
      dq(10) => dq(10),
      dq(9) => dq(9),
      dq(8) => dq(8),
      dq(7) => dq(7),
      dq(6) => dq(6),
      dq(5) => dq(5),
      dq(4) => dq(4),
      dq(3) => dq(3),
      dq(2) => dq(2),
      dq(1) => dq(1),
      dq(0) => dq(0),
      ddr_address(12) => ddr_address(12),
      ddr_address(11) => ddr_address(11),
      ddr_address(10) => ddr_address(10),
      ddr_address(9) => ddr_address(9),
      ddr_address(8) => ddr_address(8),
      ddr_address(7) => ddr_address(7),
      ddr_address(6) => ddr_address(6),
      ddr_address(5) => ddr_address(5),
      ddr_address(4) => ddr_address(4),
      ddr_address(3) => ddr_address(3),
      ddr_address(2) => ddr_address(2),
      ddr_address(1) => ddr_address(1),
      ddr_address(0) => ddr_address(0),
      ddr_ba(1) => ddr_ba(1),
      ddr_ba(0) => ddr_ba(0),
      ddr_dm(1) => ddr_dm(1),
      ddr_dm(0) => ddr_dm(0)
    );
  controller0 : controller_16bit_00
    port map (
      clk => clk_int,
      rst0 => sys_rst,
      wait_200us => wait_200us,
      clk180 => clk180,
      dip1 => NLW_controller0_dip1_UNCONNECTED,
      dip3 => NLW_controller0_dip3_UNCONNECTED,
      burst_done => burst_done,
      rst180 => sys_rst180,
      ddr_ODT_cntrl => ddr_ODT_cntrl,
      ddr_csb_cntrl => ddr_csb_cntrl,
      cmd_ack => user_cmd_ack,
      auto_ref_req => auto_ref_req,
      ar_done => ar_done,
      ddr_web_cntrl => ddr_web_cntrl,
      dqs_enable => dqs_enable,
      ddr_casb_cntrl => ddr_casb_cntrl,
      rst_calib => rst_calib,
      ddr_cke_cntrl => ddr_cke_cntrl,
      init => init_val,
      dqs_reset => dqs_reset,
      ddr_rasb_cntrl => ddr_rasb_cntrl,
      write_enable => write_enable,
      rst_dqs_div_int => rst_dqs_div_int,
      address(22) => NLW_controller0_address_22_UNCONNECTED,
      address(21) => NLW_controller0_address_21_UNCONNECTED,
      address(20) => NLW_controller0_address_20_UNCONNECTED,
      address(19) => NLW_controller0_address_19_UNCONNECTED,
      address(18) => NLW_controller0_address_18_UNCONNECTED,
      address(17) => NLW_controller0_address_17_UNCONNECTED,
      address(16) => NLW_controller0_address_16_UNCONNECTED,
      address(15) => NLW_controller0_address_15_UNCONNECTED,
      address(14) => NLW_controller0_address_14_UNCONNECTED,
      address(13) => NLW_controller0_address_13_UNCONNECTED,
      address(12) => NLW_controller0_address_12_UNCONNECTED,
      address(11) => NLW_controller0_address_11_UNCONNECTED,
      address(10) => NLW_controller0_address_10_UNCONNECTED,
      address(9) => user_input_address(9),
      address(8) => user_input_address(8),
      address(7) => user_input_address(7),
      address(6) => user_input_address(6),
      address(5) => user_input_address(5),
      address(4) => user_input_address(4),
      address(3) => user_input_address(3),
      address(2) => user_input_address(2),
      address(1) => NLW_controller0_address_1_UNCONNECTED,
      address(0) => NLW_controller0_address_0_UNCONNECTED,
      config_register1(14) => NLW_controller0_config_register1_14_UNCONNECTED,
      config_register1(13) => NLW_controller0_config_register1_13_UNCONNECTED,
      config_register1(12) => user_config_register1(12),
      config_register1(11) => NLW_controller0_config_register1_11_UNCONNECTED,
      config_register1(10) => NLW_controller0_config_register1_10_UNCONNECTED,
      config_register1(9) => user_config_register1(9),
      config_register1(8) => user_config_register1(8),
      config_register1(7) => user_config_register1(7),
      config_register1(6) => NLW_controller0_config_register1_6_UNCONNECTED,
      config_register1(5) => user_config_register1(5),
      config_register1(4) => user_config_register1(4),
      config_register1(3) => user_config_register1(3),
      config_register1(2) => NLW_controller0_config_register1_2_UNCONNECTED,
      config_register1(1) => user_config_register1(1),
      config_register1(0) => NLW_controller0_config_register1_0_UNCONNECTED,
      config_register2(12) => NLW_controller0_config_register2_12_UNCONNECTED,
      config_register2(11) => NLW_controller0_config_register2_11_UNCONNECTED,
      config_register2(10) => NLW_controller0_config_register2_10_UNCONNECTED,
      config_register2(9) => user_config_register2(9),
      config_register2(8) => user_config_register2(8),
      config_register2(7) => user_config_register2(7),
      config_register2(6) => NLW_controller0_config_register2_6_UNCONNECTED,
      config_register2(5) => NLW_controller0_config_register2_5_UNCONNECTED,
      config_register2(4) => NLW_controller0_config_register2_4_UNCONNECTED,
      config_register2(3) => NLW_controller0_config_register2_3_UNCONNECTED,
      config_register2(2) => user_config_register2(2),
      config_register2(1) => NLW_controller0_config_register2_1_UNCONNECTED,
      config_register2(0) => NLW_controller0_config_register2_0_UNCONNECTED,
      bank_address(1) => NLW_controller0_bank_address_1_UNCONNECTED,
      bank_address(0) => NLW_controller0_bank_address_0_UNCONNECTED,
      command_register(3) => user_command_register(3),
      command_register(2) => user_command_register(2),
      command_register(1) => user_command_register(1),
      command_register(0) => user_command_register(0),
      ddr_ba_cntrl(1) => ddr_ba_cntrl(1),
      ddr_ba_cntrl(0) => ddr_ba_cntrl(0),
      ddr_address_cntrl(12) => ddr_address_cntrl(12),
      ddr_address_cntrl(11) => ddr_address_cntrl(11),
      ddr_address_cntrl(10) => ddr_address_cntrl(10),
      ddr_address_cntrl(9) => ddr_address_cntrl(9),
      ddr_address_cntrl(8) => ddr_address_cntrl(8),
      ddr_address_cntrl(7) => ddr_address_cntrl(7),
      ddr_address_cntrl(6) => ddr_address_cntrl(6),
      ddr_address_cntrl(5) => ddr_address_cntrl(5),
      ddr_address_cntrl(4) => ddr_address_cntrl(4),
      ddr_address_cntrl(3) => ddr_address_cntrl(3),
      ddr_address_cntrl(2) => ddr_address_cntrl(2),
      ddr_address_cntrl(1) => ddr_address_cntrl(1),
      ddr_address_cntrl(0) => ddr_address_cntrl(0)
    );
  data_path0 : data_path_16bit_rl
    port map (
      rst_dqs_div => dqs_div_rst,
      clk => clk_int,
      reset270 => sys_rst270,
      reset => sys_rst,
      clk180 => clk180,
      clk90 => clk90_int,
      clk270 => clk270,
      reset90 => sys_rst90,
      dqs_int_delay_in0 => dqs_int_delay_in0,
      dqs_int_delay_in1 => dqs_int_delay_in1,
      write_enable => write_enable,
      reset180 => sys_rst180,
      reset270_r_val => reset270_r,
      u_data_val => user_data_valid,
      write_en_val => write_en_val,
      dq(15) => dq(15),
      dq(14) => dq(14),
      dq(13) => dq(13),
      dq(12) => dq(12),
      dq(11) => dq(11),
      dq(10) => dq(10),
      dq(9) => dq(9),
      dq(8) => dq(8),
      dq(7) => dq(7),
      dq(6) => dq(6),
      dq(5) => dq(5),
      dq(4) => dq(4),
      dq(3) => dq(3),
      dq(2) => dq(2),
      dq(1) => dq(1),
      dq(0) => dq(0),
      delay_sel(4) => delay_sel(4),
      delay_sel(3) => delay_sel(3),
      delay_sel(2) => delay_sel(2),
      delay_sel(1) => delay_sel(1),
      delay_sel(0) => delay_sel(0),
      user_data_mask(3) => user_data_mask(3),
      user_data_mask(2) => user_data_mask(2),
      user_data_mask(1) => user_data_mask(1),
      user_data_mask(0) => user_data_mask(0),
      user_input_data(31) => user_input_data(31),
      user_input_data(30) => user_input_data(30),
      user_input_data(29) => user_input_data(29),
      user_input_data(28) => user_input_data(28),
      user_input_data(27) => user_input_data(27),
      user_input_data(26) => user_input_data(26),
      user_input_data(25) => user_input_data(25),
      user_input_data(24) => user_input_data(24),
      user_input_data(23) => user_input_data(23),
      user_input_data(22) => user_input_data(22),
      user_input_data(21) => user_input_data(21),
      user_input_data(20) => user_input_data(20),
      user_input_data(19) => user_input_data(19),
      user_input_data(18) => user_input_data(18),
      user_input_data(17) => user_input_data(17),
      user_input_data(16) => user_input_data(16),
      user_input_data(15) => user_input_data(15),
      user_input_data(14) => user_input_data(14),
      user_input_data(13) => user_input_data(13),
      user_input_data(12) => user_input_data(12),
      user_input_data(11) => user_input_data(11),
      user_input_data(10) => user_input_data(10),
      user_input_data(9) => user_input_data(9),
      user_input_data(8) => user_input_data(8),
      user_input_data(7) => user_input_data(7),
      user_input_data(6) => user_input_data(6),
      user_input_data(5) => user_input_data(5),
      user_input_data(4) => user_input_data(4),
      user_input_data(3) => user_input_data(3),
      user_input_data(2) => user_input_data(2),
      user_input_data(1) => user_input_data(1),
      user_input_data(0) => user_input_data(0),
      write_data_falling(15) => write_data_falling(15),
      write_data_falling(14) => write_data_falling(14),
      write_data_falling(13) => write_data_falling(13),
      write_data_falling(12) => write_data_falling(12),
      write_data_falling(11) => write_data_falling(11),
      write_data_falling(10) => write_data_falling(10),
      write_data_falling(9) => write_data_falling(9),
      write_data_falling(8) => write_data_falling(8),
      write_data_falling(7) => write_data_falling(7),
      write_data_falling(6) => write_data_falling(6),
      write_data_falling(5) => write_data_falling(5),
      write_data_falling(4) => write_data_falling(4),
      write_data_falling(3) => write_data_falling(3),
      write_data_falling(2) => write_data_falling(2),
      write_data_falling(1) => write_data_falling(1),
      write_data_falling(0) => write_data_falling(0),
      data_mask_f(1) => data_mask_f(1),
      data_mask_f(0) => data_mask_f(0),
      data_mask_r(1) => data_mask_r(1),
      data_mask_r(0) => data_mask_r(0),
      write_data_rising(15) => write_data_rising(15),
      write_data_rising(14) => write_data_rising(14),
      write_data_rising(13) => write_data_rising(13),
      write_data_rising(12) => write_data_rising(12),
      write_data_rising(11) => write_data_rising(11),
      write_data_rising(10) => write_data_rising(10),
      write_data_rising(9) => write_data_rising(9),
      write_data_rising(8) => write_data_rising(8),
      write_data_rising(7) => write_data_rising(7),
      write_data_rising(6) => write_data_rising(6),
      write_data_rising(5) => write_data_rising(5),
      write_data_rising(4) => write_data_rising(4),
      write_data_rising(3) => write_data_rising(3),
      write_data_rising(2) => write_data_rising(2),
      write_data_rising(1) => write_data_rising(1),
      write_data_rising(0) => write_data_rising(0),
      user_output_data(31) => user_output_data(31),
      user_output_data(30) => user_output_data(30),
      user_output_data(29) => user_output_data(29),
      user_output_data(28) => user_output_data(28),
      user_output_data(27) => user_output_data(27),
      user_output_data(26) => user_output_data(26),
      user_output_data(25) => user_output_data(25),
      user_output_data(24) => user_output_data(24),
      user_output_data(23) => user_output_data(23),
      user_output_data(22) => user_output_data(22),
      user_output_data(21) => user_output_data(21),
      user_output_data(20) => user_output_data(20),
      user_output_data(19) => user_output_data(19),
      user_output_data(18) => user_output_data(18),
      user_output_data(17) => user_output_data(17),
      user_output_data(16) => user_output_data(16),
      user_output_data(15) => user_output_data(15),
      user_output_data(14) => user_output_data(14),
      user_output_data(13) => user_output_data(13),
      user_output_data(12) => user_output_data(12),
      user_output_data(11) => user_output_data(11),
      user_output_data(10) => user_output_data(10),
      user_output_data(9) => user_output_data(9),
      user_output_data(8) => user_output_data(8),
      user_output_data(7) => user_output_data(7),
      user_output_data(6) => user_output_data(6),
      user_output_data(5) => user_output_data(5),
      user_output_data(4) => user_output_data(4),
      user_output_data(3) => user_output_data(3),
      user_output_data(2) => user_output_data(2),
      user_output_data(1) => user_output_data(1),
      user_output_data(0) => user_output_data(0)
    );
  infrastructure0 : infrastructure
    port map (
      clk_int => clk_int,
      sys_rst => sys_rst,
      rst_calib1 => rst_calib,
      delay_sel_val(4) => delay_sel_val(4),
      delay_sel_val(3) => delay_sel_val(3),
      delay_sel_val(2) => delay_sel_val(2),
      delay_sel_val(1) => delay_sel_val(1),
      delay_sel_val(0) => delay_sel_val(0),
      delay_sel_val1_val(4) => delay_sel(4),
      delay_sel_val1_val(3) => delay_sel(3),
      delay_sel_val1_val(2) => delay_sel(2),
      delay_sel_val1_val(1) => delay_sel(1),
      delay_sel_val1_val(0) => delay_sel(0)
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity cal_div2f is
  port (
    reset : in STD_LOGIC := 'X'; 
    iclk : in STD_LOGIC := 'X'; 
    oclk : out STD_LOGIC 
  );
end cal_div2f;

architecture Structure of cal_div2f is
  signal poclk_204 : STD_LOGIC; 
  signal Q_n0003 : STD_LOGIC; 
begin
  poclk : FDR_1
    port map (
      D => Q_n0003,
      R => reset,
      C => iclk,
      Q => poclk_204
    );
  oclk_203 : FDR_1
    port map (
      D => poclk_204,
      R => reset,
      C => iclk,
      Q => oclk
    );
  Q_n00031 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => poclk_204,
      O => Q_n0003
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity cal_div2 is
  port (
    reset : in STD_LOGIC := 'X'; 
    iclk : in STD_LOGIC := 'X'; 
    oclk : out STD_LOGIC 
  );
end cal_div2;

architecture Structure of cal_div2 is
  signal poclk_206 : STD_LOGIC; 
  signal Q_n0003 : STD_LOGIC; 
begin
  poclk : FDR
    port map (
      D => Q_n0003,
      R => reset,
      C => iclk,
      Q => poclk_206
    );
  oclk_205 : FDR
    port map (
      D => poclk_206,
      R => reset,
      C => iclk,
      Q => oclk
    );
  Q_n00031 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => poclk_206,
      O => Q_n0003
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity cal_ctl is
  port (
    clk : in STD_LOGIC := 'X'; 
    hxSamp1 : in STD_LOGIC := 'X'; 
    dcmlocked : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    phSamp1 : in STD_LOGIC := 'X'; 
    psDone : in STD_LOGIC := 'X'; 
    okToSelTap : in STD_LOGIC := 'X'; 
    locReset : out STD_LOGIC; 
    psInc : out STD_LOGIC; 
    psEn : out STD_LOGIC; 
    selTap : out STD_LOGIC_VECTOR ( 4 downto 0 ); 
    tapForDqs : out STD_LOGIC_VECTOR ( 4 downto 0 ) 
  );
end cal_ctl;

architecture Structure of cal_ctl is
  signal NlwRenamedSig_OI_psInc : STD_LOGIC; 
  signal NlwRenamedSig_OI_psEn : STD_LOGIC; 
  signal prevSamp_208 : STD_LOGIC; 
  signal resetDcm_209 : STD_LOGIC; 
  signal okSelCnt_210 : STD_LOGIC; 
  signal wait3Cycle_211 : STD_LOGIC; 
  signal wait4Cycle_212 : STD_LOGIC; 
  signal wait5Cycle_213 : STD_LOGIC; 
  signal psDoneReg_214 : STD_LOGIC; 
  signal rstate_215 : STD_LOGIC; 
  signal Q_n0051 : STD_LOGIC; 
  signal Q_n0103 : STD_LOGIC; 
  signal Q_n0060 : STD_LOGIC; 
  signal Q_n0113 : STD_LOGIC; 
  signal Q_n0070 : STD_LOGIC; 
  signal Q_n0067_216 : STD_LOGIC; 
  signal Q_n0072 : STD_LOGIC; 
  signal Q_n0122 : STD_LOGIC; 
  signal Q_n0125 : STD_LOGIC; 
  signal Q_n0076_217 : STD_LOGIC; 
  signal Q_n0081_218 : STD_LOGIC; 
  signal Q_n0126 : STD_LOGIC; 
  signal Q_n0131 : STD_LOGIC; 
  signal Q_n0132 : STD_LOGIC; 
  signal waitOneCycle_219 : STD_LOGIC; 
  signal Q_n0083 : STD_LOGIC; 
  signal Q_n0090 : STD_LOGIC; 
  signal Q_n0091 : STD_LOGIC; 
  signal Q_n0088 : STD_LOGIC; 
  signal waitTwoCycle_220 : STD_LOGIC; 
  signal Q_n0200 : STD_LOGIC; 
  signal Q_n0201 : STD_LOGIC; 
  signal Q_n0202 : STD_LOGIC; 
  signal Q_n0203 : STD_LOGIC; 
  signal Q_n0204_221 : STD_LOGIC; 
  signal Q_n0210 : STD_LOGIC; 
  signal Q_n0205_222 : STD_LOGIC; 
  signal Q_n0211_223 : STD_LOGIC; 
  signal Q_n0206 : STD_LOGIC; 
  signal Q_n0207 : STD_LOGIC; 
  signal Q_n0213 : STD_LOGIC; 
  signal Q_n0208_224 : STD_LOGIC; 
  signal Q_n0214 : STD_LOGIC; 
  signal Q_n0209 : STD_LOGIC; 
  signal Q_n0215 : STD_LOGIC; 
  signal Q_n0216 : STD_LOGIC; 
  signal Q_n0217 : STD_LOGIC; 
  signal Q_n0194 : STD_LOGIC; 
  signal Q_n0195 : STD_LOGIC; 
  signal Q_n0196 : STD_LOGIC; 
  signal Q_n0198 : STD_LOGIC; 
  signal N1 : STD_LOGIC; 
  signal N8 : STD_LOGIC; 
  signal cal_ctl_n0068_0_cyo : STD_LOGIC; 
  signal N9 : STD_LOGIC; 
  signal cal_ctl_n0068_1_cyo : STD_LOGIC; 
  signal N10 : STD_LOGIC; 
  signal cal_ctl_n0068_2_cyo : STD_LOGIC; 
  signal N11 : STD_LOGIC; 
  signal cal_ctl_n0068_3_cyo : STD_LOGIC; 
  signal N12 : STD_LOGIC; 
  signal cal_ctl_n0068_4_cyo : STD_LOGIC; 
  signal N13 : STD_LOGIC; 
  signal cal_ctl_n0068_5_cyo : STD_LOGIC; 
  signal N14 : STD_LOGIC; 
  signal cal_ctl_n0068_6_cyo : STD_LOGIC; 
  signal N15 : STD_LOGIC; 
  signal N16 : STD_LOGIC; 
  signal cal_ctl_n0089_0_cyo : STD_LOGIC; 
  signal N17 : STD_LOGIC; 
  signal cal_ctl_n0089_1_cyo : STD_LOGIC; 
  signal N18 : STD_LOGIC; 
  signal cal_ctl_n0089_2_cyo : STD_LOGIC; 
  signal N19 : STD_LOGIC; 
  signal cal_ctl_n0089_3_cyo : STD_LOGIC; 
  signal N20 : STD_LOGIC; 
  signal cal_ctl_n0089_4_cyo : STD_LOGIC; 
  signal N21 : STD_LOGIC; 
  signal cal_ctl_n0089_5_cyo : STD_LOGIC; 
  signal N22 : STD_LOGIC; 
  signal cal_ctl_n0089_6_cyo : STD_LOGIC; 
  signal N23 : STD_LOGIC; 
  signal cal_ctl_n0095_3_cyo : STD_LOGIC; 
  signal cal_ctl_n0095_5_cyo : STD_LOGIC; 
  signal cal_ctl_n0096_3_cyo : STD_LOGIC; 
  signal cal_ctl_n0096_5_cyo : STD_LOGIC; 
  signal cal_ctl_n0097_3_cyo : STD_LOGIC; 
  signal cal_ctl_n0097_5_cyo : STD_LOGIC; 
  signal cal_ctl_Result_4_cyo : STD_LOGIC; 
  signal state_FFd1_225 : STD_LOGIC; 
  signal state_FFd2_226 : STD_LOGIC; 
  signal state_FFd3_227 : STD_LOGIC; 
  signal state_FFd4_228 : STD_LOGIC; 
  signal state_FFd1_In : STD_LOGIC; 
  signal state_FFd2_In : STD_LOGIC; 
  signal state_FFd3_In : STD_LOGIC; 
  signal state_FFd4_In : STD_LOGIC; 
  signal Madd_n0079_n0001 : STD_LOGIC; 
  signal Madd_n0079_n0002 : STD_LOGIC; 
  signal lPtr_n0000 : STD_LOGIC; 
  signal selCnt_Sclr_INV : STD_LOGIC; 
  signal N41 : STD_LOGIC; 
  signal N45 : STD_LOGIC; 
  signal N47 : STD_LOGIC; 
  signal N49 : STD_LOGIC; 
  signal N51 : STD_LOGIC; 
  signal N59 : STD_LOGIC; 
  signal N61 : STD_LOGIC; 
  signal N64 : STD_LOGIC; 
  signal N66 : STD_LOGIC; 
  signal N79 : STD_LOGIC; 
  signal N91 : STD_LOGIC; 
  signal N101 : STD_LOGIC; 
  signal N131 : STD_LOGIC; 
  signal N161 : STD_LOGIC; 
  signal N211 : STD_LOGIC; 
  signal N221 : STD_LOGIC; 
  signal N311 : STD_LOGIC; 
  signal N32 : STD_LOGIC; 
  signal N35 : STD_LOGIC; 
  signal N38 : STD_LOGIC; 
  signal N40 : STD_LOGIC; 
  signal N411 : STD_LOGIC; 
  signal N421 : STD_LOGIC; 
  signal N43 : STD_LOGIC; 
  signal N171 : STD_LOGIC; 
  signal N24 : STD_LOGIC; 
  signal N27 : STD_LOGIC; 
  signal N29 : STD_LOGIC; 
  signal N491 : STD_LOGIC; 
  signal N58 : STD_LOGIC; 
  signal N591 : STD_LOGIC; 
  signal N71 : STD_LOGIC; 
  signal N72 : STD_LOGIC; 
  signal state_FFd4_In_map93 : STD_LOGIC; 
  signal state_FFd4_In_map100 : STD_LOGIC; 
  signal state_FFd4_In_map102 : STD_LOGIC; 
  signal N134 : STD_LOGIC; 
  signal N138 : STD_LOGIC; 
  signal N156 : STD_LOGIC; 
  signal N168 : STD_LOGIC; 
  signal N169 : STD_LOGIC; 
  signal state_FFd3_In_MUXF5 : STD_LOGIC; 
  signal N170 : STD_LOGIC; 
  signal N1711 : STD_LOGIC; 
  signal state_FFd3_In_MUXF51 : STD_LOGIC; 
  signal N177 : STD_LOGIC; 
  signal N194 : STD_LOGIC; 
  signal N258 : STD_LOGIC; 
  signal N277 : STD_LOGIC; 
  signal N280 : STD_LOGIC; 
  signal N296 : STD_LOGIC; 
  signal N299 : STD_LOGIC; 
  signal N306 : STD_LOGIC; 
  signal N307 : STD_LOGIC; 
  signal N316 : STD_LOGIC; 
  signal state_FFd2_In_map230 : STD_LOGIC; 
  signal state_FFd2_In_map248 : STD_LOGIC; 
  signal N405 : STD_LOGIC; 
  signal N417 : STD_LOGIC; 
  signal N423 : STD_LOGIC; 
  signal N431 : STD_LOGIC; 
  signal N440 : STD_LOGIC; 
  signal N447 : STD_LOGIC; 
  signal N460 : STD_LOGIC; 
  signal N517 : STD_LOGIC; 
  signal N549 : STD_LOGIC; 
  signal N589 : STD_LOGIC; 
  signal N616 : STD_LOGIC; 
  signal N646 : STD_LOGIC; 
  signal N673 : STD_LOGIC; 
  signal N708 : STD_LOGIC; 
  signal N735 : STD_LOGIC; 
  signal N765 : STD_LOGIC; 
  signal N792 : STD_LOGIC; 
  signal decNegSh_4_rt_229 : STD_LOGIC; 
  signal N805 : STD_LOGIC; 
  signal N807 : STD_LOGIC; 
  signal N813 : STD_LOGIC; 
  signal N815 : STD_LOGIC; 
  signal N817 : STD_LOGIC; 
  signal N819 : STD_LOGIC; 
  signal N821 : STD_LOGIC; 
  signal N823 : STD_LOGIC; 
  signal N826 : STD_LOGIC; 
  signal N827 : STD_LOGIC; 
  signal N828 : STD_LOGIC; 
  signal N829 : STD_LOGIC; 
  signal N830 : STD_LOGIC; 
  signal N831 : STD_LOGIC; 
  signal N832 : STD_LOGIC; 
  signal N833 : STD_LOGIC; 
  signal N834 : STD_LOGIC; 
  signal N835 : STD_LOGIC; 
  signal N836 : STD_LOGIC; 
  signal N837 : STD_LOGIC; 
  signal N838 : STD_LOGIC; 
  signal N839 : STD_LOGIC; 
  signal NlwRenamedSig_OI_selTap : STD_LOGIC_VECTOR ( 4 downto 0 ); 
  signal zoShft : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal ozShft : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0057 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0058 : STD_LOGIC_VECTOR ( 4 downto 0 ); 
  signal inTapForDqs : STD_LOGIC_VECTOR ( 4 downto 0 ); 
  signal Q_n0059 : STD_LOGIC_VECTOR ( 4 downto 0 ); 
  signal posPhShft : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal negPhShft : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0073 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0080 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0084 : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal Q_n0092 : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal midPt : STD_LOGIC_VECTOR ( 3 downto 1 ); 
  signal selCnt : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal d4Shft : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal d0Shft : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal newTap : STD_LOGIC_VECTOR ( 4 downto 0 ); 
  signal decNegSh : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal d5Shft : STD_LOGIC_VECTOR ( 7 downto 4 ); 
  signal Q_n0068 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal d1Shft : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal suShft : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0074 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal d2Shft : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0079 : STD_LOGIC_VECTOR ( 2 downto 1 ); 
  signal decPosSh : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal uPtr : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal Q_n0085 : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal d3Shft : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal Q_n0089 : STD_LOGIC_VECTOR ( 7 downto 1 ); 
  signal lPtr : STD_LOGIC_VECTOR ( 2 downto 0 ); 
  signal Result : STD_LOGIC_VECTOR ( 7 downto 0 ); 
  signal selCnt_n0000 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  psInc <= NlwRenamedSig_OI_psInc;
  psEn <= NlwRenamedSig_OI_psEn;
  selTap(4) <= NlwRenamedSig_OI_selTap(4);
  selTap(3) <= NlwRenamedSig_OI_selTap(3);
  selTap(2) <= NlwRenamedSig_OI_selTap(2);
  selTap(1) <= NlwRenamedSig_OI_selTap(1);
  selTap(0) <= NlwRenamedSig_OI_selTap(0);
  XST_GND : GND
    port map (
      G => uPtr(3)
    );
  XST_VCC : VCC
    port map (
      P => N1
    );
  d4Shft_0 : FDRE
    port map (
      D => Q_n0057(0),
      R => reset,
      CE => Q_n0195,
      C => clk,
      Q => d4Shft(0)
    );
  d4Shft_1 : FDRE
    port map (
      D => Q_n0057(1),
      R => reset,
      CE => Q_n0195,
      C => clk,
      Q => d4Shft(1)
    );
  d4Shft_2 : FDRE
    port map (
      D => Q_n0057(2),
      R => reset,
      CE => Q_n0195,
      C => clk,
      Q => d4Shft(2)
    );
  d4Shft_3 : FDRE
    port map (
      D => Q_n0057(3),
      R => reset,
      CE => Q_n0195,
      C => clk,
      Q => d4Shft(3)
    );
  d4Shft_4 : FDRE
    port map (
      D => Q_n0057(4),
      R => reset,
      CE => Q_n0195,
      C => clk,
      Q => d4Shft(4)
    );
  d4Shft_5 : FDRE
    port map (
      D => Q_n0057(5),
      R => reset,
      CE => Q_n0195,
      C => clk,
      Q => d4Shft(5)
    );
  d4Shft_6 : FDRE
    port map (
      D => Q_n0057(6),
      R => reset,
      CE => Q_n0195,
      C => clk,
      Q => d4Shft(6)
    );
  d4Shft_7 : FDRE
    port map (
      D => Q_n0057(7),
      R => reset,
      CE => Q_n0195,
      C => clk,
      Q => d4Shft(7)
    );
  selTap_0 : FDRE
    port map (
      D => Q_n0058(0),
      R => reset,
      CE => Q_n0194,
      C => clk,
      Q => NlwRenamedSig_OI_selTap(0)
    );
  selTap_1 : FDRE
    port map (
      D => Q_n0058(1),
      R => reset,
      CE => Q_n0194,
      C => clk,
      Q => NlwRenamedSig_OI_selTap(1)
    );
  selTap_2 : FDRE
    port map (
      D => Q_n0058(2),
      R => reset,
      CE => Q_n0194,
      C => clk,
      Q => NlwRenamedSig_OI_selTap(2)
    );
  selTap_3 : FDRE
    port map (
      D => Q_n0058(3),
      R => reset,
      CE => Q_n0194,
      C => clk,
      Q => NlwRenamedSig_OI_selTap(3)
    );
  selTap_4 : FDRE
    port map (
      D => Q_n0058(4),
      R => reset,
      CE => Q_n0194,
      C => clk,
      Q => NlwRenamedSig_OI_selTap(4)
    );
  d0Shft_0 : FDRE
    port map (
      D => Q_n0057(0),
      R => reset,
      CE => Q_n0196,
      C => clk,
      Q => d0Shft(0)
    );
  d0Shft_1 : FDRE
    port map (
      D => Q_n0057(1),
      R => reset,
      CE => Q_n0196,
      C => clk,
      Q => d0Shft(1)
    );
  d0Shft_2 : FDRE
    port map (
      D => Q_n0057(2),
      R => reset,
      CE => Q_n0196,
      C => clk,
      Q => d0Shft(2)
    );
  d0Shft_3 : FDRE
    port map (
      D => Q_n0057(3),
      R => reset,
      CE => Q_n0196,
      C => clk,
      Q => d0Shft(3)
    );
  d0Shft_4 : FDRE
    port map (
      D => Q_n0057(4),
      R => reset,
      CE => Q_n0196,
      C => clk,
      Q => d0Shft(4)
    );
  d0Shft_5 : FDRE
    port map (
      D => Q_n0057(5),
      R => reset,
      CE => Q_n0196,
      C => clk,
      Q => d0Shft(5)
    );
  d0Shft_6 : FDRE
    port map (
      D => Q_n0057(6),
      R => reset,
      CE => Q_n0196,
      C => clk,
      Q => d0Shft(6)
    );
  d0Shft_7 : FDRE
    port map (
      D => Q_n0057(7),
      R => reset,
      CE => Q_n0196,
      C => clk,
      Q => d0Shft(7)
    );
  d5Shft_4 : FDRE
    port map (
      D => Q_n0057(4),
      R => reset,
      CE => Q_n0201,
      C => clk,
      Q => d5Shft(4)
    );
  d5Shft_5 : FDRE
    port map (
      D => Q_n0057(5),
      R => reset,
      CE => Q_n0201,
      C => clk,
      Q => d5Shft(5)
    );
  d5Shft_6 : FDRE
    port map (
      D => Q_n0057(6),
      R => reset,
      CE => Q_n0201,
      C => clk,
      Q => d5Shft(6)
    );
  d5Shft_7 : FDRE
    port map (
      D => Q_n0057(7),
      R => reset,
      CE => Q_n0201,
      C => clk,
      Q => d5Shft(7)
    );
  d1Shft_0 : FDRE
    port map (
      D => Q_n0057(0),
      R => reset,
      CE => Q_n0203,
      C => clk,
      Q => d1Shft(0)
    );
  d1Shft_1 : FDRE
    port map (
      D => Q_n0057(1),
      R => reset,
      CE => Q_n0203,
      C => clk,
      Q => d1Shft(1)
    );
  d1Shft_2 : FDRE
    port map (
      D => Q_n0057(2),
      R => reset,
      CE => Q_n0203,
      C => clk,
      Q => d1Shft(2)
    );
  d1Shft_3 : FDRE
    port map (
      D => Q_n0057(3),
      R => reset,
      CE => Q_n0203,
      C => clk,
      Q => d1Shft(3)
    );
  d1Shft_4 : FDRE
    port map (
      D => Q_n0057(4),
      R => reset,
      CE => Q_n0203,
      C => clk,
      Q => d1Shft(4)
    );
  d1Shft_5 : FDRE
    port map (
      D => Q_n0057(5),
      R => reset,
      CE => Q_n0203,
      C => clk,
      Q => d1Shft(5)
    );
  d1Shft_6 : FDRE
    port map (
      D => Q_n0057(6),
      R => reset,
      CE => Q_n0203,
      C => clk,
      Q => d1Shft(6)
    );
  d1Shft_7 : FDRE
    port map (
      D => Q_n0057(7),
      R => reset,
      CE => Q_n0203,
      C => clk,
      Q => d1Shft(7)
    );
  resetDcm : FDRE
    port map (
      D => Q_n0072,
      R => reset,
      CE => Q_n0205_222,
      C => clk,
      Q => resetDcm_209
    );
  suShft_0 : FDRE
    port map (
      D => Q_n0073(0),
      R => reset,
      CE => Q_n0206,
      C => clk,
      Q => suShft(0)
    );
  suShft_1 : FDRE
    port map (
      D => Q_n0073(1),
      R => reset,
      CE => Q_n0206,
      C => clk,
      Q => suShft(1)
    );
  suShft_2 : FDRE
    port map (
      D => Q_n0073(2),
      R => reset,
      CE => Q_n0206,
      C => clk,
      Q => suShft(2)
    );
  suShft_3 : FDRE
    port map (
      D => Q_n0073(3),
      R => reset,
      CE => Q_n0206,
      C => clk,
      Q => suShft(3)
    );
  suShft_4 : FDRE
    port map (
      D => Q_n0073(4),
      R => reset,
      CE => Q_n0206,
      C => clk,
      Q => suShft(4)
    );
  suShft_5 : FDRE
    port map (
      D => Q_n0073(5),
      R => reset,
      CE => Q_n0206,
      C => clk,
      Q => suShft(5)
    );
  suShft_6 : FDRE
    port map (
      D => Q_n0073(6),
      R => reset,
      CE => Q_n0206,
      C => clk,
      Q => suShft(6)
    );
  suShft_7 : FDRE
    port map (
      D => Q_n0073(7),
      R => reset,
      CE => Q_n0206,
      C => clk,
      Q => suShft(7)
    );
  rstate : FDSE
    port map (
      D => Q_n0076_217,
      S => reset,
      CE => Q_n0208_224,
      C => clk,
      Q => rstate_215
    );
  d2Shft_0 : FDRE
    port map (
      D => Q_n0057(0),
      R => reset,
      CE => Q_n0209,
      C => clk,
      Q => d2Shft(0)
    );
  d2Shft_1 : FDRE
    port map (
      D => Q_n0057(1),
      R => reset,
      CE => Q_n0209,
      C => clk,
      Q => d2Shft(1)
    );
  d2Shft_2 : FDRE
    port map (
      D => Q_n0057(2),
      R => reset,
      CE => Q_n0209,
      C => clk,
      Q => d2Shft(2)
    );
  d2Shft_3 : FDRE
    port map (
      D => Q_n0057(3),
      R => reset,
      CE => Q_n0209,
      C => clk,
      Q => d2Shft(3)
    );
  d2Shft_4 : FDRE
    port map (
      D => Q_n0057(4),
      R => reset,
      CE => Q_n0209,
      C => clk,
      Q => d2Shft(4)
    );
  d2Shft_5 : FDRE
    port map (
      D => Q_n0057(5),
      R => reset,
      CE => Q_n0209,
      C => clk,
      Q => d2Shft(5)
    );
  d2Shft_6 : FDRE
    port map (
      D => Q_n0057(6),
      R => reset,
      CE => Q_n0209,
      C => clk,
      Q => d2Shft(6)
    );
  d2Shft_7 : FDRE
    port map (
      D => Q_n0057(7),
      R => reset,
      CE => Q_n0209,
      C => clk,
      Q => d2Shft(7)
    );
  decPosSh_0 : FDRE
    port map (
      D => Q_n0080(0),
      R => reset,
      CE => Q_n0211_223,
      C => clk,
      Q => decPosSh(0)
    );
  decPosSh_1 : FDRE
    port map (
      D => Q_n0080(1),
      R => reset,
      CE => Q_n0211_223,
      C => clk,
      Q => decPosSh(1)
    );
  decPosSh_2 : FDRE
    port map (
      D => Q_n0080(2),
      R => reset,
      CE => Q_n0211_223,
      C => clk,
      Q => decPosSh(2)
    );
  decPosSh_3 : FDRE
    port map (
      D => Q_n0080(3),
      R => reset,
      CE => Q_n0211_223,
      C => clk,
      Q => decPosSh(3)
    );
  decPosSh_4 : FDRE
    port map (
      D => Q_n0080(4),
      R => reset,
      CE => Q_n0211_223,
      C => clk,
      Q => decPosSh(4)
    );
  decPosSh_5 : FDRE
    port map (
      D => Q_n0080(5),
      R => reset,
      CE => Q_n0211_223,
      C => clk,
      Q => decPosSh(5)
    );
  decPosSh_6 : FDRE
    port map (
      D => Q_n0080(6),
      R => reset,
      CE => Q_n0211_223,
      C => clk,
      Q => decPosSh(6)
    );
  decPosSh_7 : FDRE
    port map (
      D => Q_n0080(7),
      R => reset,
      CE => Q_n0211_223,
      C => clk,
      Q => decPosSh(7)
    );
  okSelCnt : FDR
    port map (
      D => N1,
      R => Q_n0081_218,
      C => clk,
      Q => okSelCnt_210
    );
  waitOneCycle : FDSE
    port map (
      D => Q_n0083,
      S => reset,
      CE => Q_n0213,
      C => clk,
      Q => waitOneCycle_219
    );
  d3Shft_0 : FDRE
    port map (
      D => Q_n0057(0),
      R => reset,
      CE => Q_n0216,
      C => clk,
      Q => d3Shft(0)
    );
  d3Shft_1 : FDRE
    port map (
      D => Q_n0057(1),
      R => reset,
      CE => Q_n0216,
      C => clk,
      Q => d3Shft(1)
    );
  d3Shft_2 : FDRE
    port map (
      D => Q_n0057(2),
      R => reset,
      CE => Q_n0216,
      C => clk,
      Q => d3Shft(2)
    );
  d3Shft_3 : FDRE
    port map (
      D => Q_n0057(3),
      R => reset,
      CE => Q_n0216,
      C => clk,
      Q => d3Shft(3)
    );
  d3Shft_4 : FDRE
    port map (
      D => Q_n0057(4),
      R => reset,
      CE => Q_n0216,
      C => clk,
      Q => d3Shft(4)
    );
  d3Shft_5 : FDRE
    port map (
      D => Q_n0057(5),
      R => reset,
      CE => Q_n0216,
      C => clk,
      Q => d3Shft(5)
    );
  d3Shft_6 : FDRE
    port map (
      D => Q_n0057(6),
      R => reset,
      CE => Q_n0216,
      C => clk,
      Q => d3Shft(6)
    );
  d3Shft_7 : FDRE
    port map (
      D => Q_n0057(7),
      R => reset,
      CE => Q_n0216,
      C => clk,
      Q => d3Shft(7)
    );
  psDoneReg : FDR
    port map (
      D => psDone,
      R => reset,
      C => clk,
      Q => psDoneReg_214
    );
  waitTwoCycle : FDRE
    port map (
      D => waitOneCycle_219,
      R => reset,
      CE => Q_n0198,
      C => clk,
      Q => waitTwoCycle_220
    );
  wait3Cycle : FDRE
    port map (
      D => waitTwoCycle_220,
      R => reset,
      CE => Q_n0210,
      C => clk,
      Q => wait3Cycle_211
    );
  psEn_207 : FDRE
    port map (
      D => Q_n0088,
      R => reset,
      CE => Q_n0208_224,
      C => clk,
      Q => NlwRenamedSig_OI_psEn
    );
  psinc_val : FDRE
    port map (
      D => Q_n0070,
      R => reset,
      CE => Q_n0204_221,
      C => clk,
      Q => NlwRenamedSig_OI_psInc
    );
  wait4Cycle : FDRE
    port map (
      D => wait3Cycle_211,
      R => reset,
      CE => Q_n0215,
      C => clk,
      Q => wait4Cycle_212
    );
  wait5Cycle : FDRE
    port map (
      D => wait4Cycle_212,
      R => reset,
      CE => Q_n0217,
      C => clk,
      Q => wait5Cycle_213
    );
  negPhShft_0 : FDRE
    port map (
      D => Q_n0074(0),
      R => reset,
      CE => Q_n0207,
      C => clk,
      Q => negPhShft(0)
    );
  negPhShft_1 : FDRE
    port map (
      D => Q_n0074(1),
      R => reset,
      CE => Q_n0207,
      C => clk,
      Q => negPhShft(1)
    );
  negPhShft_2 : FDRE
    port map (
      D => Q_n0074(2),
      R => reset,
      CE => Q_n0207,
      C => clk,
      Q => negPhShft(2)
    );
  negPhShft_3 : FDRE
    port map (
      D => Q_n0074(3),
      R => reset,
      CE => Q_n0207,
      C => clk,
      Q => negPhShft(3)
    );
  negPhShft_4 : FDRE
    port map (
      D => Q_n0074(4),
      R => reset,
      CE => Q_n0207,
      C => clk,
      Q => negPhShft(4)
    );
  negPhShft_5 : FDRE
    port map (
      D => Q_n0074(5),
      R => reset,
      CE => Q_n0207,
      C => clk,
      Q => negPhShft(5)
    );
  negPhShft_6 : FDRE
    port map (
      D => Q_n0074(6),
      R => reset,
      CE => Q_n0207,
      C => clk,
      Q => negPhShft(6)
    );
  negPhShft_7 : FDRE
    port map (
      D => Q_n0074(7),
      R => reset,
      CE => Q_n0207,
      C => clk,
      Q => negPhShft(7)
    );
  posPhShft_0 : FDRE
    port map (
      D => Q_n0085(0),
      R => reset,
      CE => Q_n0214,
      C => clk,
      Q => posPhShft(0)
    );
  posPhShft_1 : FDRE
    port map (
      D => Q_n0085(1),
      R => reset,
      CE => Q_n0214,
      C => clk,
      Q => posPhShft(1)
    );
  posPhShft_2 : FDRE
    port map (
      D => Q_n0085(2),
      R => reset,
      CE => Q_n0214,
      C => clk,
      Q => posPhShft(2)
    );
  posPhShft_3 : FDRE
    port map (
      D => Q_n0085(3),
      R => reset,
      CE => Q_n0214,
      C => clk,
      Q => posPhShft(3)
    );
  posPhShft_4 : FDRE
    port map (
      D => Q_n0085(4),
      R => reset,
      CE => Q_n0214,
      C => clk,
      Q => posPhShft(4)
    );
  posPhShft_5 : FDRE
    port map (
      D => Q_n0085(5),
      R => reset,
      CE => Q_n0214,
      C => clk,
      Q => posPhShft(5)
    );
  posPhShft_6 : FDRE
    port map (
      D => Q_n0085(6),
      R => reset,
      CE => Q_n0214,
      C => clk,
      Q => posPhShft(6)
    );
  posPhShft_7 : FDRE
    port map (
      D => Q_n0085(7),
      R => reset,
      CE => Q_n0214,
      C => clk,
      Q => posPhShft(7)
    );
  prevSamp : FDRE
    port map (
      D => Q_n0067_216,
      R => reset,
      CE => Q_n0202,
      C => clk,
      Q => prevSamp_208
    );
  zoShft_0 : FDR
    port map (
      D => Q_n0068(0),
      R => reset,
      C => clk,
      Q => zoShft(0)
    );
  zoShft_1 : FDR
    port map (
      D => Q_n0068(1),
      R => reset,
      C => clk,
      Q => zoShft(1)
    );
  zoShft_2 : FDR
    port map (
      D => Q_n0068(2),
      R => reset,
      C => clk,
      Q => zoShft(2)
    );
  zoShft_3 : FDR
    port map (
      D => Q_n0068(3),
      R => reset,
      C => clk,
      Q => zoShft(3)
    );
  zoShft_4 : FDR
    port map (
      D => Q_n0068(4),
      R => reset,
      C => clk,
      Q => zoShft(4)
    );
  zoShft_5 : FDR
    port map (
      D => Q_n0068(5),
      R => reset,
      C => clk,
      Q => zoShft(5)
    );
  zoShft_6 : FDR
    port map (
      D => Q_n0068(6),
      R => reset,
      C => clk,
      Q => zoShft(6)
    );
  zoShft_7 : FDR
    port map (
      D => Q_n0068(7),
      R => reset,
      C => clk,
      Q => zoShft(7)
    );
  ozShft_0 : FDR
    port map (
      D => N16,
      R => reset,
      C => clk,
      Q => ozShft(0)
    );
  ozShft_1 : FDR
    port map (
      D => Q_n0089(1),
      R => reset,
      C => clk,
      Q => ozShft(1)
    );
  ozShft_2 : FDR
    port map (
      D => Q_n0089(2),
      R => reset,
      C => clk,
      Q => ozShft(2)
    );
  ozShft_3 : FDR
    port map (
      D => Q_n0089(3),
      R => reset,
      C => clk,
      Q => ozShft(3)
    );
  ozShft_4 : FDR
    port map (
      D => Q_n0089(4),
      R => reset,
      C => clk,
      Q => ozShft(4)
    );
  ozShft_5 : FDR
    port map (
      D => Q_n0089(5),
      R => reset,
      C => clk,
      Q => ozShft(5)
    );
  ozShft_6 : FDR
    port map (
      D => Q_n0089(6),
      R => reset,
      C => clk,
      Q => ozShft(6)
    );
  ozShft_7 : FDR
    port map (
      D => Q_n0089(7),
      R => reset,
      C => clk,
      Q => ozShft(7)
    );
  uPtr_0 : FDS
    port map (
      D => Q_n0084(0),
      S => reset,
      C => clk,
      Q => uPtr(0)
    );
  uPtr_1 : FDR
    port map (
      D => Q_n0084(1),
      R => reset,
      C => clk,
      Q => uPtr(1)
    );
  uPtr_2 : FDS
    port map (
      D => Q_n0084(2),
      S => reset,
      C => clk,
      Q => uPtr(2)
    );
  midPt_1 : FDS
    port map (
      D => Q_n0079(1),
      S => reset,
      C => clk,
      Q => midPt(1)
    );
  midPt_2 : FDR
    port map (
      D => Q_n0079(2),
      R => reset,
      C => clk,
      Q => midPt(2)
    );
  midPt_3 : FDR
    port map (
      D => Madd_n0079_n0002,
      R => reset,
      C => clk,
      Q => midPt(3)
    );
  newTap_2 : FDR
    port map (
      D => inTapForDqs(2),
      R => reset,
      C => clk,
      Q => newTap(2)
    );
  newTap_0 : FDR
    port map (
      D => inTapForDqs(0),
      R => reset,
      C => clk,
      Q => newTap(0)
    );
  newTap_1 : FDR
    port map (
      D => inTapForDqs(1),
      R => reset,
      C => clk,
      Q => newTap(1)
    );
  newTap_3 : FDS
    port map (
      D => inTapForDqs(3),
      S => reset,
      C => clk,
      Q => newTap(3)
    );
  newTap_4 : FDS
    port map (
      D => inTapForDqs(4),
      S => reset,
      C => clk,
      Q => newTap(4)
    );
  inTapForDqs_2 : FDR
    port map (
      D => Q_n0059(2),
      R => reset,
      C => clk,
      Q => inTapForDqs(2)
    );
  inTapForDqs_0 : FDR
    port map (
      D => Q_n0059(0),
      R => reset,
      C => clk,
      Q => inTapForDqs(0)
    );
  inTapForDqs_1 : FDR
    port map (
      D => Q_n0059(1),
      R => reset,
      C => clk,
      Q => inTapForDqs(1)
    );
  inTapForDqs_3 : FDS
    port map (
      D => Q_n0059(3),
      S => reset,
      C => clk,
      Q => inTapForDqs(3)
    );
  inTapForDqs_4 : FDS
    port map (
      D => Q_n0059(4),
      S => reset,
      C => clk,
      Q => inTapForDqs(4)
    );
  tapForDqs_2 : FDRE
    port map (
      D => newTap(2),
      R => reset,
      CE => okSelCnt_210,
      C => clk,
      Q => tapForDqs(2)
    );
  tapForDqs_0 : FDRE
    port map (
      D => newTap(0),
      R => reset,
      CE => okSelCnt_210,
      C => clk,
      Q => tapForDqs(0)
    );
  tapForDqs_1 : FDRE
    port map (
      D => newTap(1),
      R => reset,
      CE => okSelCnt_210,
      C => clk,
      Q => tapForDqs(1)
    );
  tapForDqs_3 : FDSE
    port map (
      D => newTap(3),
      S => reset,
      CE => okSelCnt_210,
      C => clk,
      Q => tapForDqs(3)
    );
  tapForDqs_4 : FDSE
    port map (
      D => newTap(4),
      S => reset,
      CE => okSelCnt_210,
      C => clk,
      Q => tapForDqs(4)
    );
  decNegSh_0 : FDRE
    port map (
      D => Result(0),
      R => Q_n0060,
      CE => N101,
      C => clk,
      Q => decNegSh(0)
    );
  decNegSh_1 : FDRE
    port map (
      D => Result(1),
      R => Q_n0060,
      CE => N101,
      C => clk,
      Q => decNegSh(1)
    );
  decNegSh_2 : FDRE
    port map (
      D => Result(2),
      R => Q_n0060,
      CE => N101,
      C => clk,
      Q => decNegSh(2)
    );
  decNegSh_3 : FDRE
    port map (
      D => Result(3),
      R => Q_n0060,
      CE => N101,
      C => clk,
      Q => decNegSh(3)
    );
  decNegSh_4 : FDRE
    port map (
      D => Result(4),
      R => Q_n0060,
      CE => N101,
      C => clk,
      Q => decNegSh(4)
    );
  decNegSh_5 : FDRE
    port map (
      D => Result(5),
      R => Q_n0060,
      CE => N101,
      C => clk,
      Q => decNegSh(5)
    );
  decNegSh_6 : FDRE
    port map (
      D => Result(6),
      R => Q_n0060,
      CE => N101,
      C => clk,
      Q => decNegSh(6)
    );
  decNegSh_7 : FDRE
    port map (
      D => Result(7),
      R => Q_n0060,
      CE => N101,
      C => clk,
      Q => decNegSh(7)
    );
  cal_ctl_n0068_0_lut : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => suShft(0),
      I1 => posPhShft(0),
      O => N8
    );
  cal_ctl_n0068_0_cy : MUXCY
    port map (
      CI => N1,
      DI => suShft(0),
      S => N8,
      O => cal_ctl_n0068_0_cyo
    );
  cal_ctl_n0068_0_xor : XORCY
    port map (
      CI => N1,
      LI => N8,
      O => Q_n0068(0)
    );
  cal_ctl_n0068_1_lut : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => suShft(1),
      I1 => posPhShft(1),
      O => N9
    );
  cal_ctl_n0068_1_cy : MUXCY
    port map (
      CI => cal_ctl_n0068_0_cyo,
      DI => suShft(1),
      S => N9,
      O => cal_ctl_n0068_1_cyo
    );
  cal_ctl_n0068_1_xor : XORCY
    port map (
      CI => cal_ctl_n0068_0_cyo,
      LI => N9,
      O => Q_n0068(1)
    );
  cal_ctl_n0068_2_lut : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => suShft(2),
      I1 => posPhShft(2),
      O => N10
    );
  cal_ctl_n0068_2_cy : MUXCY
    port map (
      CI => cal_ctl_n0068_1_cyo,
      DI => suShft(2),
      S => N10,
      O => cal_ctl_n0068_2_cyo
    );
  cal_ctl_n0068_2_xor : XORCY
    port map (
      CI => cal_ctl_n0068_1_cyo,
      LI => N10,
      O => Q_n0068(2)
    );
  cal_ctl_n0068_3_lut : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => suShft(3),
      I1 => posPhShft(3),
      O => N11
    );
  cal_ctl_n0068_3_cy : MUXCY
    port map (
      CI => cal_ctl_n0068_2_cyo,
      DI => suShft(3),
      S => N11,
      O => cal_ctl_n0068_3_cyo
    );
  cal_ctl_n0068_3_xor : XORCY
    port map (
      CI => cal_ctl_n0068_2_cyo,
      LI => N11,
      O => Q_n0068(3)
    );
  cal_ctl_n0068_4_lut : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => suShft(4),
      I1 => posPhShft(4),
      O => N12
    );
  cal_ctl_n0068_4_cy : MUXCY
    port map (
      CI => cal_ctl_n0068_3_cyo,
      DI => suShft(4),
      S => N12,
      O => cal_ctl_n0068_4_cyo
    );
  cal_ctl_n0068_4_xor : XORCY
    port map (
      CI => cal_ctl_n0068_3_cyo,
      LI => N12,
      O => Q_n0068(4)
    );
  cal_ctl_n0068_5_lut : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => suShft(5),
      I1 => posPhShft(5),
      O => N13
    );
  cal_ctl_n0068_5_cy : MUXCY
    port map (
      CI => cal_ctl_n0068_4_cyo,
      DI => suShft(5),
      S => N13,
      O => cal_ctl_n0068_5_cyo
    );
  cal_ctl_n0068_5_xor : XORCY
    port map (
      CI => cal_ctl_n0068_4_cyo,
      LI => N13,
      O => Q_n0068(5)
    );
  cal_ctl_n0068_6_lut : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => suShft(6),
      I1 => posPhShft(6),
      O => N14
    );
  cal_ctl_n0068_6_cy : MUXCY
    port map (
      CI => cal_ctl_n0068_5_cyo,
      DI => suShft(6),
      S => N14,
      O => cal_ctl_n0068_6_cyo
    );
  cal_ctl_n0068_6_xor : XORCY
    port map (
      CI => cal_ctl_n0068_5_cyo,
      LI => N14,
      O => Q_n0068(6)
    );
  cal_ctl_n0068_7_lut : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => suShft(7),
      I1 => posPhShft(7),
      O => N15
    );
  cal_ctl_n0068_7_xor : XORCY
    port map (
      CI => cal_ctl_n0068_6_cyo,
      LI => N15,
      O => Q_n0068(7)
    );
  cal_ctl_n0089_0_lut : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => negPhShft(0),
      I1 => suShft(0),
      O => N16
    );
  cal_ctl_n0089_0_cy : MUXCY
    port map (
      CI => uPtr(3),
      DI => negPhShft(0),
      S => N16,
      O => cal_ctl_n0089_0_cyo
    );
  cal_ctl_n0089_1_lut : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => negPhShft(1),
      I1 => suShft(1),
      O => N17
    );
  cal_ctl_n0089_1_cy : MUXCY
    port map (
      CI => cal_ctl_n0089_0_cyo,
      DI => negPhShft(1),
      S => N17,
      O => cal_ctl_n0089_1_cyo
    );
  cal_ctl_n0089_1_xor : XORCY
    port map (
      CI => cal_ctl_n0089_0_cyo,
      LI => N17,
      O => Q_n0089(1)
    );
  cal_ctl_n0089_2_lut : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => negPhShft(2),
      I1 => suShft(2),
      O => N18
    );
  cal_ctl_n0089_2_cy : MUXCY
    port map (
      CI => cal_ctl_n0089_1_cyo,
      DI => negPhShft(2),
      S => N18,
      O => cal_ctl_n0089_2_cyo
    );
  cal_ctl_n0089_2_xor : XORCY
    port map (
      CI => cal_ctl_n0089_1_cyo,
      LI => N18,
      O => Q_n0089(2)
    );
  cal_ctl_n0089_3_lut : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => negPhShft(3),
      I1 => suShft(3),
      O => N19
    );
  cal_ctl_n0089_3_cy : MUXCY
    port map (
      CI => cal_ctl_n0089_2_cyo,
      DI => negPhShft(3),
      S => N19,
      O => cal_ctl_n0089_3_cyo
    );
  cal_ctl_n0089_3_xor : XORCY
    port map (
      CI => cal_ctl_n0089_2_cyo,
      LI => N19,
      O => Q_n0089(3)
    );
  cal_ctl_n0089_4_lut : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => negPhShft(4),
      I1 => suShft(4),
      O => N20
    );
  cal_ctl_n0089_4_cy : MUXCY
    port map (
      CI => cal_ctl_n0089_3_cyo,
      DI => negPhShft(4),
      S => N20,
      O => cal_ctl_n0089_4_cyo
    );
  cal_ctl_n0089_4_xor : XORCY
    port map (
      CI => cal_ctl_n0089_3_cyo,
      LI => N20,
      O => Q_n0089(4)
    );
  cal_ctl_n0089_5_lut : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => negPhShft(5),
      I1 => suShft(5),
      O => N21
    );
  cal_ctl_n0089_5_cy : MUXCY
    port map (
      CI => cal_ctl_n0089_4_cyo,
      DI => negPhShft(5),
      S => N21,
      O => cal_ctl_n0089_5_cyo
    );
  cal_ctl_n0089_5_xor : XORCY
    port map (
      CI => cal_ctl_n0089_4_cyo,
      LI => N21,
      O => Q_n0089(5)
    );
  cal_ctl_n0089_6_lut : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => negPhShft(6),
      I1 => suShft(6),
      O => N22
    );
  cal_ctl_n0089_6_cy : MUXCY
    port map (
      CI => cal_ctl_n0089_5_cyo,
      DI => negPhShft(6),
      S => N22,
      O => cal_ctl_n0089_6_cyo
    );
  cal_ctl_n0089_6_xor : XORCY
    port map (
      CI => cal_ctl_n0089_5_cyo,
      LI => N22,
      O => Q_n0089(6)
    );
  cal_ctl_n0089_7_lut : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => negPhShft(7),
      I1 => suShft(7),
      O => N23
    );
  cal_ctl_n0089_7_xor : XORCY
    port map (
      CI => cal_ctl_n0089_6_cyo,
      LI => N23,
      O => Q_n0089(7)
    );
  state_FFd1 : FDRE
    port map (
      D => state_FFd1_In,
      R => reset,
      CE => Q_n0194,
      C => clk,
      Q => state_FFd1_225
    );
  state_FFd2 : FDRE
    port map (
      D => state_FFd2_In,
      R => reset,
      CE => Q_n0194,
      C => clk,
      Q => state_FFd2_226
    );
  state_FFd3 : FDRE
    port map (
      D => state_FFd3_In,
      R => reset,
      CE => Q_n0194,
      C => clk,
      Q => state_FFd3_227
    );
  state_FFd4 : FDRE
    port map (
      D => state_FFd4_In,
      R => reset,
      CE => Q_n0194,
      C => clk,
      Q => state_FFd4_228
    );
  lPtr_0 : FDRS
    port map (
      D => Q_n0092(0),
      R => Q_n0090,
      S => Q_n0091,
      C => clk,
      Q => lPtr(0)
    );
  lPtr_1 : FDR
    port map (
      D => Q_n0092(1),
      R => lPtr_n0000,
      C => clk,
      Q => lPtr(1)
    );
  lPtr_2 : FDR
    port map (
      D => Q_n0092(2),
      R => lPtr_n0000,
      C => clk,
      Q => lPtr(2)
    );
  selCnt_0 : FDRE
    port map (
      D => selCnt_n0000(0),
      R => selCnt_Sclr_INV,
      CE => Q_n0200,
      C => clk,
      Q => selCnt(0)
    );
  selCnt_1 : FDRE
    port map (
      D => selCnt_n0000(1),
      R => selCnt_Sclr_INV,
      CE => Q_n0200,
      C => clk,
      Q => selCnt(1)
    );
  selCnt_2 : FDRE
    port map (
      D => selCnt_n0000(2),
      R => selCnt_Sclr_INV,
      CE => Q_n0200,
      C => clk,
      Q => selCnt(2)
    );
  selCnt_3 : FDRE
    port map (
      D => selCnt_n0000(3),
      R => selCnt_Sclr_INV,
      CE => Q_n0200,
      C => clk,
      Q => selCnt(3)
    );
  Q_n0057_7_1 : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => hxSamp1,
      I1 => ozShft(7),
      I2 => prevSamp_208,
      I3 => zoShft(7),
      O => Q_n0057(7)
    );
  Q_n0057_6_1 : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => hxSamp1,
      I1 => ozShft(6),
      I2 => prevSamp_208,
      I3 => zoShft(6),
      O => Q_n0057(6)
    );
  Q_n0057_5_1 : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => hxSamp1,
      I1 => ozShft(5),
      I2 => prevSamp_208,
      I3 => zoShft(5),
      O => Q_n0057(5)
    );
  Q_n0057_4_1 : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => hxSamp1,
      I1 => ozShft(4),
      I2 => prevSamp_208,
      I3 => zoShft(4),
      O => Q_n0057(4)
    );
  Q_n0057_3_1 : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => hxSamp1,
      I1 => ozShft(3),
      I2 => prevSamp_208,
      I3 => zoShft(3),
      O => Q_n0057(3)
    );
  Q_n0057_2_1 : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => hxSamp1,
      I1 => ozShft(2),
      I2 => prevSamp_208,
      I3 => zoShft(2),
      O => Q_n0057(2)
    );
  Q_n0057_1_1 : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => hxSamp1,
      I1 => ozShft(1),
      I2 => prevSamp_208,
      I3 => zoShft(1),
      O => Q_n0057(1)
    );
  Q_n0057_0_1 : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => hxSamp1,
      I1 => ozShft(0),
      I2 => prevSamp_208,
      I3 => zoShft(0),
      O => Q_n0057(0)
    );
  Q_n0073_7_1 : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => phSamp1,
      I1 => negPhShft(7),
      I2 => prevSamp_208,
      I3 => posPhShft(7),
      O => Q_n0073(7)
    );
  Q_n0073_6_1 : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => phSamp1,
      I1 => negPhShft(6),
      I2 => prevSamp_208,
      I3 => posPhShft(6),
      O => Q_n0073(6)
    );
  Q_n0073_5_1 : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => phSamp1,
      I1 => negPhShft(5),
      I2 => prevSamp_208,
      I3 => posPhShft(5),
      O => Q_n0073(5)
    );
  Q_n0073_4_1 : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => phSamp1,
      I1 => negPhShft(4),
      I2 => prevSamp_208,
      I3 => posPhShft(4),
      O => Q_n0073(4)
    );
  Q_n0073_3_1 : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => phSamp1,
      I1 => negPhShft(3),
      I2 => prevSamp_208,
      I3 => posPhShft(3),
      O => Q_n0073(3)
    );
  Q_n0073_2_1 : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => phSamp1,
      I1 => negPhShft(2),
      I2 => prevSamp_208,
      I3 => posPhShft(2),
      O => Q_n0073(2)
    );
  Q_n0073_1_1 : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => phSamp1,
      I1 => negPhShft(1),
      I2 => prevSamp_208,
      I3 => posPhShft(1),
      O => Q_n0073(1)
    );
  Q_n0073_0_1 : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => phSamp1,
      I1 => negPhShft(0),
      I2 => prevSamp_208,
      I3 => posPhShft(0),
      O => Q_n0073(0)
    );
  Q_n01941 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => wait4Cycle_212,
      I1 => wait5Cycle_213,
      I2 => waitOneCycle_219,
      I3 => waitTwoCycle_220,
      O => N41
    );
  Q_n01942 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => dcmlocked,
      I1 => resetDcm_209,
      I2 => wait3Cycle_211,
      I3 => N41,
      O => Q_n0194
    );
  Q_n01981 : LUT4
    generic map(
      INIT => X"2220"
    )
    port map (
      I0 => dcmlocked,
      I1 => resetDcm_209,
      I2 => waitOneCycle_219,
      I3 => waitTwoCycle_220,
      O => Q_n0198
    );
  Q_n02001 : LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      I0 => reset,
      I1 => wait4Cycle_212,
      O => Q_n0200
    );
  Q_n02021 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => wait3Cycle_211,
      I1 => wait4Cycle_212,
      I2 => waitOneCycle_219,
      I3 => waitTwoCycle_220,
      O => N45
    );
  Q_n02022 : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => dcmlocked,
      I1 => resetDcm_209,
      I2 => N45,
      O => Q_n0202
    );
  Q_n02101 : LUT4
    generic map(
      INIT => X"FFAB"
    )
    port map (
      I0 => waitOneCycle_219,
      I1 => wait3Cycle_211,
      I2 => waitTwoCycle_220,
      I3 => resetDcm_209,
      O => N47
    );
  Q_n02102 : LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      I0 => N47,
      I1 => dcmlocked,
      O => Q_n0210
    );
  Q_n02151 : LUT4
    generic map(
      INIT => X"FFAB"
    )
    port map (
      I0 => waitTwoCycle_220,
      I1 => wait3Cycle_211,
      I2 => wait4Cycle_212,
      I3 => waitOneCycle_219,
      O => N49
    );
  Q_n02152 : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => dcmlocked,
      I1 => resetDcm_209,
      I2 => N49,
      O => Q_n0215
    );
  Q_n02171 : LUT4
    generic map(
      INIT => X"FFAB"
    )
    port map (
      I0 => waitTwoCycle_220,
      I1 => wait4Cycle_212,
      I2 => wait5Cycle_213,
      I3 => waitOneCycle_219,
      O => N51
    );
  Q_n02172 : LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      I0 => dcmlocked,
      I1 => resetDcm_209,
      I2 => wait3Cycle_211,
      I3 => N51,
      O => Q_n0217
    );
  cal_ctl_Result_0_xor11 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => decNegSh(0),
      O => Result(0)
    );
  cal_ctl_Result_1_xor11 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => decNegSh(0),
      I1 => decNegSh(1),
      O => Result(1)
    );
  cal_ctl_Result_2_xor11 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => decNegSh(0),
      I1 => decNegSh(1),
      I2 => decNegSh(2),
      O => Result(2)
    );
  cal_ctl_Result_3_xor11 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => decNegSh(0),
      I1 => decNegSh(3),
      I2 => decNegSh(1),
      I3 => decNegSh(2),
      O => Result(3)
    );
  Q_n0074_0_1 : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => negPhShft(0),
      I1 => psDoneReg_214,
      I2 => NlwRenamedSig_OI_psInc,
      I3 => rstate_215,
      O => Q_n0074(0)
    );
  Q_n0074_1_1 : LUT3
    generic map(
      INIT => X"BF"
    )
    port map (
      I0 => NlwRenamedSig_OI_psInc,
      I1 => psDoneReg_214,
      I2 => rstate_215,
      O => N59
    );
  Q_n0074_1_2 : LUT3
    generic map(
      INIT => X"06"
    )
    port map (
      I0 => negPhShft(0),
      I1 => negPhShft(1),
      I2 => N59,
      O => Q_n0074(1)
    );
  Q_n0074_2_1 : LUT3
    generic map(
      INIT => X"BF"
    )
    port map (
      I0 => NlwRenamedSig_OI_psInc,
      I1 => psDoneReg_214,
      I2 => rstate_215,
      O => N61
    );
  Q_n0074_2_2 : LUT4
    generic map(
      INIT => X"060C"
    )
    port map (
      I0 => negPhShft(1),
      I1 => negPhShft(2),
      I2 => N61,
      I3 => negPhShft(0),
      O => Q_n0074(2)
    );
  Q_n0085_0_1 : LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      I0 => posPhShft(0),
      I1 => psDoneReg_214,
      I2 => NlwRenamedSig_OI_psInc,
      I3 => rstate_215,
      O => Q_n0085(0)
    );
  Q_n0085_1_1 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => psDoneReg_214,
      I1 => NlwRenamedSig_OI_psInc,
      I2 => rstate_215,
      O => N64
    );
  Q_n0085_1_2 : LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      I0 => posPhShft(0),
      I1 => posPhShft(1),
      I2 => N64,
      O => Q_n0085(1)
    );
  Q_n0085_2_1 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => psDoneReg_214,
      I1 => NlwRenamedSig_OI_psInc,
      I2 => rstate_215,
      O => N66
    );
  Q_n0085_2_2 : LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      I0 => posPhShft(0),
      I1 => posPhShft(2),
      I2 => N66,
      I3 => posPhShft(1),
      O => Q_n0085(2)
    );
  Madd_n0079_LPM_ADD_SUB_2_0_xo_1_1 : LUT4
    generic map(
      INIT => X"965A"
    )
    port map (
      I0 => lPtr(1),
      I1 => lPtr(0),
      I2 => uPtr(1),
      I3 => uPtr(0),
      O => Q_n0079(1)
    );
  selCnt_Madd_n0000_n00091 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => selCnt(0),
      O => selCnt_n0000(0)
    );
  selCnt_Madd_n0000_Mxor_Result_3_Result1 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => selCnt(0),
      I1 => selCnt(3),
      I2 => selCnt(1),
      I3 => selCnt(2),
      O => selCnt_n0000(3)
    );
  selCnt_Madd_n0000_Mxor_Result_2_Result1 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => selCnt(0),
      I1 => selCnt(1),
      I2 => selCnt(2),
      O => selCnt_n0000(2)
    );
  selCnt_Madd_n0000_Mxor_Result_1_Result1 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => selCnt(0),
      I1 => selCnt(1),
      O => selCnt_n0000(1)
    );
  Q_n0059_4_1 : LUT4
    generic map(
      INIT => X"BBFC"
    )
    port map (
      I0 => inTapForDqs(4),
      I1 => midPt(2),
      I2 => midPt(1),
      I3 => midPt(3),
      O => Q_n0059(4)
    );
  Q_n0059_3_1 : LUT3
    generic map(
      INIT => X"E6"
    )
    port map (
      I0 => midPt(3),
      I1 => midPt(2),
      I2 => inTapForDqs(3),
      O => Q_n0059(3)
    );
  Q_n0059_2_1 : LUT4
    generic map(
      INIT => X"BC8C"
    )
    port map (
      I0 => inTapForDqs(2),
      I1 => midPt(3),
      I2 => midPt(2),
      I3 => midPt(1),
      O => Q_n0059(2)
    );
  Q_n0059_1_1 : LUT3
    generic map(
      INIT => X"A2"
    )
    port map (
      I0 => midPt(3),
      I1 => midPt(2),
      I2 => inTapForDqs(1),
      O => Q_n0059(1)
    );
  Q_n0059_0_1 : LUT4
    generic map(
      INIT => X"8C80"
    )
    port map (
      I0 => inTapForDqs(0),
      I1 => midPt(3),
      I2 => midPt(2),
      I3 => midPt(1),
      O => Q_n0059(0)
    );
  cal_ctl_Result_4_xor11 : MUXF5
    port map (
      I0 => decNegSh_4_rt_229,
      I1 => N79,
      S => decNegSh(2),
      O => Result(4)
    );
  cal_ctl_Result_4_xor11_G : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => decNegSh(0),
      I1 => decNegSh(4),
      I2 => decNegSh(1),
      I3 => decNegSh(3),
      O => N79
    );
  Q_n0084_2_1 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => Q_n0125,
      I1 => Q_n0126,
      O => Q_n0084(2)
    );
  Ker421 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => rstate_215,
      I1 => psDoneReg_214,
      I2 => NlwRenamedSig_OI_psInc,
      O => N421
    );
  Ker431 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => psDoneReg_214,
      I1 => NlwRenamedSig_OI_psInc,
      I2 => rstate_215,
      O => N43
    );
  Madd_n0079_n00021 : LUT3
    generic map(
      INIT => X"E8"
    )
    port map (
      I0 => Madd_n0079_n0001,
      I1 => uPtr(2),
      I2 => lPtr(2),
      O => Madd_n0079_n0002
    );
  Q_n0085_4_1 : LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      I0 => posPhShft(4),
      I1 => cal_ctl_n0097_3_cyo,
      I2 => N421,
      O => Q_n0085(4)
    );
  Q_n0074_4_1 : LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      I0 => negPhShft(4),
      I1 => cal_ctl_n0095_3_cyo,
      I2 => N43,
      O => Q_n0074(4)
    );
  Madd_n0079_LPM_ADD_SUB_2_01_xo_1_1 : LUT3
    generic map(
      INIT => X"96"
    )
    port map (
      I0 => lPtr(2),
      I1 => Madd_n0079_n0001,
      I2 => uPtr(2),
      O => Q_n0079(2)
    );
  Q_n01251 : LUT4
    generic map(
      INIT => X"0133"
    )
    port map (
      I0 => d5Shft(5),
      I1 => d5Shft(7),
      I2 => d5Shft(4),
      I3 => d5Shft(6),
      O => Q_n0125
    );
  Q_n01261 : LUT4
    generic map(
      INIT => X"0133"
    )
    port map (
      I0 => d4Shft(5),
      I1 => d4Shft(7),
      I2 => d4Shft(4),
      I3 => d4Shft(6),
      O => Q_n0126
    );
  Q_n0074_5_1 : LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      I0 => cal_ctl_n0095_3_cyo,
      I1 => negPhShft(5),
      I2 => N43,
      I3 => negPhShft(4),
      O => Q_n0074(5)
    );
  Madd_n0079_n00011 : LUT4
    generic map(
      INIT => X"F880"
    )
    port map (
      I0 => lPtr(0),
      I1 => uPtr(0),
      I2 => uPtr(1),
      I3 => lPtr(1),
      O => Madd_n0079_n0001
    );
  Q_n0085_5_1 : LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      I0 => cal_ctl_n0097_3_cyo,
      I1 => posPhShft(5),
      I2 => N421,
      I3 => posPhShft(4),
      O => Q_n0085(5)
    );
  Ker401 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => psDoneReg_214,
      I1 => dcmlocked,
      I2 => rstate_215,
      O => N40
    );
  Q_n02131 : LUT4
    generic map(
      INIT => X"2220"
    )
    port map (
      I0 => dcmlocked,
      I1 => resetDcm_209,
      I2 => N38,
      I3 => waitOneCycle_219,
      O => Q_n0213
    );
  cal_ctl_Result_5_xor11 : LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      I0 => decNegSh(5),
      I1 => cal_ctl_Result_4_cyo,
      O => Result(5)
    );
  cal_ctl_Result_6_xor11 : LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      I0 => decNegSh(5),
      I1 => cal_ctl_Result_4_cyo,
      I2 => decNegSh(6),
      O => Result(6)
    );
  cal_ctl_Result_7_xor11 : LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      I0 => decNegSh(5),
      I1 => decNegSh(7),
      I2 => cal_ctl_Result_4_cyo,
      I3 => decNegSh(6),
      O => Result(7)
    );
  cal_ctl_Result_4_cy1_SW0 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => decNegSh(1),
      I1 => decNegSh(0),
      O => N171
    );
  cal_ctl_Result_4_cy1 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => decNegSh(4),
      I1 => decNegSh(3),
      I2 => decNegSh(2),
      I3 => N171,
      O => cal_ctl_Result_4_cyo
    );
  Ker131 : LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      I0 => state_FFd2_226,
      I1 => state_FFd4_228,
      O => N131
    );
  Q_n0084_1_SW0 : LUT4
    generic map(
      INIT => X"FFFD"
    )
    port map (
      I0 => N211,
      I1 => d3Shft(7),
      I2 => Q_n0126,
      I3 => Q_n0125,
      O => N24
    );
  Q_n0058_0_SW0 : LUT4
    generic map(
      INIT => X"FF5D"
    )
    port map (
      I0 => state_FFd1_225,
      I1 => state_FFd2_226,
      I2 => NlwRenamedSig_OI_selTap(0),
      I3 => state_FFd3_227,
      O => N27
    );
  Q_n0058_0_Q : LUT4
    generic map(
      INIT => X"A2AE"
    )
    port map (
      I0 => NlwRenamedSig_OI_selTap(0),
      I1 => state_FFd4_228,
      I2 => N32,
      I3 => N27,
      O => Q_n0058(0)
    );
  Q_n0058_1_SW0 : LUT4
    generic map(
      INIT => X"AAC4"
    )
    port map (
      I0 => state_FFd2_226,
      I1 => state_FFd1_225,
      I2 => NlwRenamedSig_OI_selTap(1),
      I3 => state_FFd3_227,
      O => N29
    );
  Q_n0058_1_Q : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => state_FFd4_228,
      I1 => NlwRenamedSig_OI_selTap(1),
      I2 => N32,
      I3 => N29,
      O => Q_n0058(1)
    );
  Q_n0092_2_1 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => Q_n0131,
      I1 => Q_n0132,
      O => Q_n0092(2)
    );
  Q_n0092_1_1 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => Q_n0131,
      I1 => Q_n0132,
      O => Q_n0092(1)
    );
  Q_n0085_6_1 : LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      I0 => posPhShft(6),
      I1 => cal_ctl_n0097_5_cyo,
      I2 => N421,
      O => Q_n0085(6)
    );
  Q_n0074_6_1 : LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      I0 => negPhShft(6),
      I1 => cal_ctl_n0095_5_cyo,
      I2 => N43,
      O => Q_n0074(6)
    );
  Q_n00831 : LUT4
    generic map(
      INIT => X"5501"
    )
    port map (
      I0 => waitOneCycle_219,
      I1 => N311,
      I2 => N32,
      I3 => N35,
      O => Q_n0083
    );
  Q_n00721 : LUT4
    generic map(
      INIT => X"5501"
    )
    port map (
      I0 => resetDcm_209,
      I1 => N311,
      I2 => N32,
      I3 => N35,
      O => Q_n0072
    );
  Q_n0085_7_1 : LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      I0 => cal_ctl_n0097_5_cyo,
      I1 => posPhShft(7),
      I2 => N421,
      I3 => posPhShft(6),
      O => Q_n0085(7)
    );
  Q_n0074_7_1 : LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      I0 => cal_ctl_n0095_5_cyo,
      I1 => negPhShft(7),
      I2 => N43,
      I3 => negPhShft(6),
      O => Q_n0074(7)
    );
  Ker161 : LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      I0 => state_FFd1_225,
      I1 => state_FFd4_228,
      O => N161
    );
  Ker2111 : LUT3
    generic map(
      INIT => X"1F"
    )
    port map (
      I0 => d3Shft(5),
      I1 => d3Shft(4),
      I2 => d3Shft(6),
      O => N211
    );
  cal_ctl_n0096_5_cy11 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => cal_ctl_n0096_3_cyo,
      I1 => decPosSh(4),
      I2 => decPosSh(5),
      O => cal_ctl_n0096_5_cyo
    );
  cal_ctl_n0097_5_cy11 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => cal_ctl_n0097_3_cyo,
      I1 => posPhShft(4),
      I2 => posPhShft(5),
      O => cal_ctl_n0097_5_cyo
    );
  cal_ctl_n0095_5_cy11 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => cal_ctl_n0095_3_cyo,
      I1 => negPhShft(4),
      I2 => negPhShft(5),
      O => cal_ctl_n0095_5_cyo
    );
  cal_ctl_n0095_3_cy11 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => negPhShft(2),
      I1 => negPhShft(3),
      I2 => negPhShft(0),
      I3 => negPhShft(1),
      O => cal_ctl_n0095_3_cyo
    );
  cal_ctl_n0096_3_cy11 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => decPosSh(2),
      I1 => decPosSh(3),
      I2 => decPosSh(0),
      I3 => decPosSh(1),
      O => cal_ctl_n0096_3_cyo
    );
  cal_ctl_n0097_3_cy11 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => posPhShft(2),
      I1 => posPhShft(3),
      I2 => posPhShft(0),
      I3 => posPhShft(1),
      O => cal_ctl_n0097_3_cyo
    );
  Q_n0080_3_SW0 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => decPosSh(1),
      I1 => decPosSh(0),
      O => N491
    );
  Q_n0080_3_Q : LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      I0 => decPosSh(2),
      I1 => decPosSh(3),
      I2 => Q_n0103,
      I3 => N491,
      O => Q_n0080(3)
    );
  Q_n013215 : LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      I0 => d3Shft(3),
      I1 => d3Shft(1),
      I2 => d3Shft(0),
      I3 => d3Shft(2),
      O => N58
    );
  Q_n013216 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => d3Shft(4),
      I1 => d3Shft(5),
      O => N591
    );
  Q_n013115 : LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      I0 => d2Shft(3),
      I1 => d2Shft(1),
      I2 => d2Shft(0),
      I3 => d2Shft(2),
      O => N71
    );
  Q_n013116 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => d2Shft(4),
      I1 => d2Shft(5),
      O => N72
    );
  state_FFd4_In7 : LUT4
    generic map(
      INIT => X"2220"
    )
    port map (
      I0 => psDoneReg_214,
      I1 => state_FFd4_228,
      I2 => state_FFd2_226,
      I3 => state_FFd1_225,
      O => state_FFd4_In_map93
    );
  state_FFd4_In24 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => state_FFd2_226,
      I1 => state_FFd3_227,
      O => state_FFd4_In_map100
    );
  state_FFd4_In33 : LUT4
    generic map(
      INIT => X"F666"
    )
    port map (
      I0 => prevSamp_208,
      I1 => hxSamp1,
      I2 => state_FFd1_225,
      I3 => state_FFd4_In_map100,
      O => state_FFd4_In_map102
    );
  state_FFd4_In46 : LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      I0 => N35,
      I1 => state_FFd4_228,
      I2 => state_FFd4_In_map102,
      I3 => state_FFd4_In_map93,
      O => state_FFd4_In
    );
  Q_n0092_0_28 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => d4Shft(7),
      I1 => d4Shft(6),
      O => N138
    );
  Q_n0092_0_56 : LUT4
    generic map(
      INIT => X"0F08"
    )
    port map (
      I0 => N134,
      I1 => N138,
      I2 => Q_n0131,
      I3 => Q_n0132,
      O => Q_n0092(0)
    );
  Ker45_SW0 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => wait5Cycle_213,
      I1 => wait4Cycle_212,
      I2 => wait3Cycle_211,
      I3 => waitOneCycle_219,
      O => N156
    );
  Q_n0080_0_1 : LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      I0 => decPosSh(0),
      I1 => Q_n0103,
      O => Q_n0080(0)
    );
  Ker221 : LUT3
    generic map(
      INIT => X"1F"
    )
    port map (
      I0 => d2Shft(5),
      I1 => d2Shft(4),
      I2 => d2Shft(6),
      O => N221
    );
  Q_n0080_4_1 : LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      I0 => decPosSh(4),
      I1 => cal_ctl_n0096_3_cyo,
      I2 => Q_n0103,
      O => Q_n0080(4)
    );
  Q_n0080_6_1 : LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      I0 => decPosSh(6),
      I1 => cal_ctl_n0096_5_cyo,
      I2 => Q_n0103,
      O => Q_n0080(6)
    );
  Q_n0080_1_1 : LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      I0 => decPosSh(0),
      I1 => decPosSh(1),
      I2 => Q_n0103,
      O => Q_n0080(1)
    );
  Q_n0080_2_1 : LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      I0 => decPosSh(0),
      I1 => decPosSh(2),
      I2 => Q_n0103,
      I3 => decPosSh(1),
      O => Q_n0080(2)
    );
  Q_n0080_7_1 : LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      I0 => cal_ctl_n0096_5_cyo,
      I1 => decPosSh(7),
      I2 => Q_n0103,
      I3 => decPosSh(6),
      O => Q_n0080(7)
    );
  Q_n0080_5_1 : LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      I0 => cal_ctl_n0096_3_cyo,
      I1 => decPosSh(5),
      I2 => Q_n0103,
      I3 => decPosSh(4),
      O => Q_n0080(5)
    );
  state_FFd3_In1 : LUT4
    generic map(
      INIT => X"E2A2"
    )
    port map (
      I0 => N32,
      I1 => state_FFd1_225,
      I2 => state_FFd2_226,
      I3 => psDoneReg_214,
      O => N168
    );
  state_FFd3_In2 : LUT4
    generic map(
      INIT => X"D515"
    )
    port map (
      I0 => N32,
      I1 => state_FFd1_225,
      I2 => state_FFd2_226,
      I3 => psDoneReg_214,
      O => N169
    );
  state_FFd3_In_rn_0 : MUXF5
    port map (
      I0 => N169,
      I1 => N168,
      S => state_FFd3_227,
      O => state_FFd3_In_MUXF5
    );
  state_FFd3_In3 : LUT3
    generic map(
      INIT => X"FB"
    )
    port map (
      I0 => state_FFd1_225,
      I1 => psDoneReg_214,
      I2 => state_FFd2_226,
      O => N170
    );
  state_FFd3_In_rn_1 : MUXF5
    port map (
      I0 => N1711,
      I1 => N170,
      S => state_FFd3_227,
      O => state_FFd3_In_MUXF51
    );
  state_FFd3_In_rn_2 : MUXF6
    port map (
      I0 => state_FFd3_In_MUXF51,
      I1 => state_FFd3_In_MUXF5,
      S => state_FFd4_228,
      O => state_FFd3_In
    );
  Q_n008825 : LUT4
    generic map(
      INIT => X"0F09"
    )
    port map (
      I0 => phSamp1,
      I1 => prevSamp_208,
      I2 => Q_n0051,
      I3 => NlwRenamedSig_OI_psEn,
      O => N194
    );
  Q_n0084_0_21 : LUT4
    generic map(
      INIT => X"0133"
    )
    port map (
      I0 => d1Shft(5),
      I1 => d1Shft(7),
      I2 => d1Shft(4),
      I3 => d1Shft(6),
      O => N277
    );
  Q_n0084_0_24 : LUT3
    generic map(
      INIT => X"A2"
    )
    port map (
      I0 => N277,
      I1 => N221,
      I2 => d2Shft(7),
      O => N280
    );
  Q_n0084_0_61 : LUT4
    generic map(
      INIT => X"FF54"
    )
    port map (
      I0 => Q_n0126,
      I1 => N280,
      I2 => N258,
      I3 => Q_n0125,
      O => Q_n0084(0)
    );
  Q_n0211_SW0 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => resetDcm_209,
      I1 => dcmlocked,
      O => N296
    );
  Q_n0211 : LUT4
    generic map(
      INIT => X"C040"
    )
    port map (
      I0 => Q_n0113,
      I1 => rstate_215,
      I2 => N296,
      I3 => Q_n0103,
      O => Q_n0211_223
    );
  lPtr_n00001 : LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      I0 => Q_n0090,
      I1 => Q_n0091,
      O => lPtr_n0000
    );
  Q_n0081_SW0 : LUT3
    generic map(
      INIT => X"FB"
    )
    port map (
      I0 => selCnt(1),
      I1 => selCnt(2),
      I2 => reset,
      O => N299
    );
  Q_n0081 : LUT4
    generic map(
      INIT => X"EFFF"
    )
    port map (
      I0 => selCnt(0),
      I1 => N299,
      I2 => Q_n0122,
      I3 => selCnt(3),
      O => Q_n0081_218
    );
  Q_n009115 : LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      I0 => d1Shft(2),
      I1 => d1Shft(1),
      I2 => d1Shft(0),
      I3 => d1Shft(3),
      O => N306
    );
  Q_n009116 : LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      I0 => d1Shft(5),
      I1 => d1Shft(4),
      O => N307
    );
  Q_n00902 : LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      I0 => reset,
      I1 => d0Shft(6),
      I2 => d0Shft(7),
      O => N316
    );
  N321 : LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      I0 => prevSamp_208,
      I1 => hxSamp1,
      O => N32
    );
  state_Out171 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => state_FFd3_227,
      I1 => state_FFd2_226,
      I2 => state_FFd1_225,
      I3 => state_FFd4_228,
      O => Q_n0051
    );
  Ker3111 : LUT4
    generic map(
      INIT => X"20FF"
    )
    port map (
      I0 => state_FFd1_225,
      I1 => state_FFd3_227,
      I2 => state_FFd2_226,
      I3 => state_FFd4_228,
      O => N311
    );
  state_FFd2_In14 : LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      I0 => state_FFd3_227,
      I1 => state_FFd2_226,
      O => state_FFd2_In_map230
    );
  state_FFd2_In88 : LUT4
    generic map(
      INIT => X"A326"
    )
    port map (
      I0 => N32,
      I1 => state_FFd2_226,
      I2 => state_FFd1_225,
      I3 => state_FFd3_227,
      O => state_FFd2_In_map248
    );
  Q_n0076_SW0 : LUT4
    generic map(
      INIT => X"FF14"
    )
    port map (
      I0 => N311,
      I1 => hxSamp1,
      I2 => prevSamp_208,
      I3 => N35,
      O => N405
    );
  Q_n0076 : LUT4
    generic map(
      INIT => X"7362"
    )
    port map (
      I0 => rstate_215,
      I1 => resetDcm_209,
      I2 => psDoneReg_214,
      I3 => N405,
      O => Q_n0076_217
    );
  Q_n007010 : LUT3
    generic map(
      INIT => X"5D"
    )
    port map (
      I0 => prevSamp_208,
      I1 => state_FFd4_228,
      I2 => hxSamp1,
      O => N417
    );
  Q_n007016 : LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      I0 => state_FFd2_226,
      I1 => state_FFd3_227,
      I2 => state_FFd1_225,
      O => N423
    );
  Q_n007028 : LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      I0 => state_FFd3_227,
      I1 => phSamp1,
      O => N431
    );
  Q_n007037 : LUT4
    generic map(
      INIT => X"3332"
    )
    port map (
      I0 => state_FFd2_226,
      I1 => state_FFd4_228,
      I2 => N431,
      I3 => state_FFd1_225,
      O => N440
    );
  Q_n007057 : LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      I0 => NlwRenamedSig_OI_psInc,
      I1 => N417,
      I2 => N423,
      I3 => N440,
      O => N447
    );
  Q_n007083 : LUT4
    generic map(
      INIT => X"111F"
    )
    port map (
      I0 => Q_n0051,
      I1 => phSamp1,
      I2 => hxSamp1,
      I3 => N311,
      O => N460
    );
  Ker381 : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => wait5Cycle_213,
      I1 => wait4Cycle_212,
      I2 => waitTwoCycle_220,
      I3 => wait3Cycle_211,
      O => N38
    );
  Ker91 : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => resetDcm_209,
      I1 => dcmlocked,
      I2 => waitOneCycle_219,
      I3 => N38,
      O => N91
    );
  Q_n01961 : LUT4
    generic map(
      INIT => X"0010"
    )
    port map (
      I0 => state_FFd3_227,
      I1 => state_FFd1_225,
      I2 => N411,
      I3 => N131,
      O => Q_n0196
    );
  Q_n02031 : LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      I0 => state_FFd3_227,
      I1 => state_FFd1_225,
      I2 => N411,
      I3 => N131,
      O => Q_n0203
    );
  Q_n02011 : LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      I0 => N131,
      I1 => state_FFd3_227,
      I2 => N411,
      I3 => state_FFd1_225,
      O => Q_n0201
    );
  Q_n02091 : LUT4
    generic map(
      INIT => X"0040"
    )
    port map (
      I0 => state_FFd3_227,
      I1 => state_FFd2_226,
      I2 => N411,
      I3 => N161,
      O => Q_n0209
    );
  Q_n02161 : LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      I0 => state_FFd3_227,
      I1 => state_FFd2_226,
      I2 => N411,
      I3 => N161,
      O => Q_n0216
    );
  Q_n01951 : LUT4
    generic map(
      INIT => X"0040"
    )
    port map (
      I0 => state_FFd3_227,
      I1 => state_FFd1_225,
      I2 => N411,
      I3 => N131,
      O => Q_n0195
    );
  selCnt_Sclr_INV1 : LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => Q_n0122,
      I1 => reset,
      O => selCnt_Sclr_INV
    );
  Q_n0122526 : LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => inTapForDqs(1),
      I1 => newTap(1),
      I2 => inTapForDqs(4),
      I3 => newTap(4),
      O => N517
    );
  Q_n0122562 : LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      I0 => inTapForDqs(2),
      I1 => newTap(2),
      I2 => inTapForDqs(3),
      I3 => newTap(3),
      O => N549
    );
  Q_n02071 : LUT4
    generic map(
      INIT => X"22F2"
    )
    port map (
      I0 => N40,
      I1 => NlwRenamedSig_OI_psInc,
      I2 => N101,
      I3 => Q_n0113,
      O => Q_n0207
    );
  Q_n02141 : LUT4
    generic map(
      INIT => X"88F8"
    )
    port map (
      I0 => NlwRenamedSig_OI_psInc,
      I1 => N40,
      I2 => N101,
      I3 => Q_n0113,
      O => Q_n0214
    );
  Q_n011332 : LUT4
    generic map(
      INIT => X"6FF6"
    )
    port map (
      I0 => negPhShft(2),
      I1 => decNegSh(2),
      I2 => negPhShft(3),
      I3 => decNegSh(3),
      O => N589
    );
  Q_n011365 : LUT4
    generic map(
      INIT => X"6FF6"
    )
    port map (
      I0 => negPhShft(4),
      I1 => decNegSh(4),
      I2 => negPhShft(5),
      I3 => decNegSh(5),
      O => N616
    );
  Q_n0113113 : LUT4
    generic map(
      INIT => X"6FF6"
    )
    port map (
      I0 => negPhShft(6),
      I1 => decNegSh(6),
      I2 => negPhShft(7),
      I3 => decNegSh(7),
      O => N646
    );
  Q_n0113146 : LUT4
    generic map(
      INIT => X"6FF6"
    )
    port map (
      I0 => negPhShft(0),
      I1 => decNegSh(0),
      I2 => negPhShft(1),
      I3 => decNegSh(1),
      O => N673
    );
  Q_n0113164 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => N589,
      I1 => N616,
      I2 => N646,
      I3 => N673,
      O => Q_n0113
    );
  Q_n00601 : LUT3
    generic map(
      INIT => X"BA"
    )
    port map (
      I0 => reset,
      I1 => Q_n0113,
      I2 => N101,
      O => Q_n0060
    );
  Ker101 : LUT4
    generic map(
      INIT => X"0800"
    )
    port map (
      I0 => dcmlocked,
      I1 => resetDcm_209,
      I2 => Q_n0103,
      I3 => rstate_215,
      O => N101
    );
  Q_n010332 : LUT4
    generic map(
      INIT => X"7DBE"
    )
    port map (
      I0 => decPosSh(3),
      I1 => decPosSh(2),
      I2 => posPhShft(2),
      I3 => posPhShft(3),
      O => N708
    );
  Q_n010365 : LUT4
    generic map(
      INIT => X"7DBE"
    )
    port map (
      I0 => decPosSh(5),
      I1 => decPosSh(4),
      I2 => posPhShft(4),
      I3 => posPhShft(5),
      O => N735
    );
  Q_n0103113 : LUT4
    generic map(
      INIT => X"7DBE"
    )
    port map (
      I0 => decPosSh(7),
      I1 => decPosSh(6),
      I2 => posPhShft(6),
      I3 => posPhShft(7),
      O => N765
    );
  Q_n0103146 : LUT4
    generic map(
      INIT => X"7DBE"
    )
    port map (
      I0 => decPosSh(1),
      I1 => decPosSh(0),
      I2 => posPhShft(0),
      I3 => posPhShft(1),
      O => N792
    );
  Q_n0103164 : LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      I0 => N708,
      I1 => N735,
      I2 => N765,
      I3 => N792,
      O => Q_n0103
    );
  decNegSh_4_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => decNegSh(4),
      O => decNegSh_4_rt_229
    );
  Q_n0122578 : LUT4
    generic map(
      INIT => X"9000"
    )
    port map (
      I0 => inTapForDqs(0),
      I1 => newTap(0),
      I2 => N517,
      I3 => N549,
      O => Q_n0122
    );
  Q_n0091113 : LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      I0 => d1Shft(6),
      I1 => N306,
      I2 => N307,
      I3 => d1Shft(7),
      O => Q_n0091
    );
  Q_n008854 : LUT4
    generic map(
      INIT => X"FE54"
    )
    port map (
      I0 => resetDcm_209,
      I1 => N194,
      I2 => N177,
      I3 => rstate_215,
      O => Q_n0088
    );
  Q_n0132113 : LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      I0 => d3Shft(7),
      I1 => N58,
      I2 => N591,
      I3 => d3Shft(6),
      O => Q_n0132
    );
  Q_n0131113 : LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      I0 => d2Shft(7),
      I1 => N71,
      I2 => N72,
      I3 => d2Shft(6),
      O => Q_n0131
    );
  Q_n009015_SW0 : LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      I0 => d0Shft(3),
      I1 => d0Shft(1),
      I2 => d0Shft(0),
      I3 => d0Shft(2),
      O => N805
    );
  Q_n009015 : LUT4
    generic map(
      INIT => X"FF80"
    )
    port map (
      I0 => d0Shft(5),
      I1 => d0Shft(4),
      I2 => N805,
      I3 => N316,
      O => Q_n0090
    );
  Q_n0092_0_20_SW0 : LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      I0 => d4Shft(1),
      I1 => d4Shft(0),
      I2 => d4Shft(2),
      O => N807
    );
  Q_n0092_0_20 : LUT4
    generic map(
      INIT => X"57FF"
    )
    port map (
      I0 => d4Shft(5),
      I1 => N807,
      I2 => d4Shft(3),
      I3 => d4Shft(4),
      O => N134
    );
  state_FFd3_In4 : LUT4
    generic map(
      INIT => X"0401"
    )
    port map (
      I0 => state_FFd1_225,
      I1 => phSamp1,
      I2 => state_FFd2_226,
      I3 => prevSamp_208,
      O => N1711
    );
  Q_n0084_0_2 : LUT4
    generic map(
      INIT => X"0133"
    )
    port map (
      I0 => d3Shft(5),
      I1 => d3Shft(7),
      I2 => d3Shft(4),
      I3 => d3Shft(6),
      O => N258
    );
  Ker4111 : LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      I0 => prevSamp_208,
      I1 => hxSamp1,
      I2 => N91,
      O => N411
    );
  Ker351 : LUT3
    generic map(
      INIT => X"06"
    )
    port map (
      I0 => prevSamp_208,
      I1 => phSamp1,
      I2 => Q_n0051,
      O => N35
    );
  Q_n00885 : LUT4
    generic map(
      INIT => X"0F09"
    )
    port map (
      I0 => hxSamp1,
      I1 => prevSamp_208,
      I2 => N311,
      I3 => NlwRenamedSig_OI_psEn,
      O => N177
    );
  Q_n02061 : LUT4
    generic map(
      INIT => X"0440"
    )
    port map (
      I0 => Q_n0051,
      I1 => N91,
      I2 => prevSamp_208,
      I3 => phSamp1,
      O => Q_n0206
    );
  Q_n0084_1_SW2 : LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      I0 => d2Shft(6),
      I1 => d2Shft(5),
      I2 => d2Shft(7),
      I3 => d2Shft(4),
      O => N813
    );
  Q_n0084_1_Q : LUT4
    generic map(
      INIT => X"4447"
    )
    port map (
      I0 => N24,
      I1 => N813,
      I2 => Q_n0126,
      I3 => Q_n0125,
      O => Q_n0084(1)
    );
  Q_n0085_3_SW1 : LUT4
    generic map(
      INIT => X"9555"
    )
    port map (
      I0 => posPhShft(3),
      I1 => posPhShft(2),
      I2 => posPhShft(1),
      I3 => posPhShft(0),
      O => N815
    );
  Q_n0085_3_Q : LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      I0 => N815,
      I1 => rstate_215,
      I2 => NlwRenamedSig_OI_psInc,
      I3 => psDoneReg_214,
      O => Q_n0085(3)
    );
  Q_n0074_3_SW1 : LUT4
    generic map(
      INIT => X"9555"
    )
    port map (
      I0 => negPhShft(3),
      I1 => negPhShft(0),
      I2 => negPhShft(2),
      I3 => negPhShft(1),
      O => N817
    );
  Q_n0074_3_Q : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => NlwRenamedSig_OI_psInc,
      I1 => psDoneReg_214,
      I2 => N817,
      I3 => rstate_215,
      O => Q_n0074(3)
    );
  Q_n0058_2_SW1 : LUT4
    generic map(
      INIT => X"AEA6"
    )
    port map (
      I0 => state_FFd2_226,
      I1 => state_FFd1_225,
      I2 => state_FFd3_227,
      I3 => NlwRenamedSig_OI_selTap(2),
      O => N819
    );
  Q_n0058_2_Q : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => state_FFd4_228,
      I1 => NlwRenamedSig_OI_selTap(2),
      I2 => N32,
      I3 => N819,
      O => Q_n0058(2)
    );
  Q_n0058_3_66_SW0 : LUT4
    generic map(
      INIT => X"F6B6"
    )
    port map (
      I0 => state_FFd3_227,
      I1 => state_FFd1_225,
      I2 => state_FFd2_226,
      I3 => NlwRenamedSig_OI_selTap(3),
      O => N821
    );
  Q_n0058_3_66 : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => state_FFd4_228,
      I1 => NlwRenamedSig_OI_selTap(3),
      I2 => N32,
      I3 => N821,
      O => Q_n0058(3)
    );
  Q_n0058_4_SW0 : LUT4
    generic map(
      INIT => X"DDF7"
    )
    port map (
      I0 => state_FFd1_225,
      I1 => state_FFd2_226,
      I2 => NlwRenamedSig_OI_selTap(4),
      I3 => state_FFd3_227,
      O => N823
    );
  Q_n0058_4_Q : LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      I0 => state_FFd4_228,
      I1 => NlwRenamedSig_OI_selTap(4),
      I2 => N32,
      I3 => N823,
      O => Q_n0058(4)
    );
  Q_n0067 : MUXF5
    port map (
      I0 => N826,
      I1 => N827,
      S => Q_n0051,
      O => Q_n0067_216
    );
  Q_n0067_F : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => wait5Cycle_213,
      I1 => prevSamp_208,
      I2 => phSamp1,
      O => N826
    );
  Q_n0067_G : LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      I0 => wait5Cycle_213,
      I1 => prevSamp_208,
      I2 => hxSamp1,
      O => N827
    );
  Q_n0205 : MUXF5
    port map (
      I0 => N828,
      I1 => N829,
      S => resetDcm_209,
      O => Q_n0205_222
    );
  Q_n0205_F : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => dcmlocked,
      I1 => waitTwoCycle_220,
      I2 => N156,
      O => N828
    );
  Q_n0205_G : LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      I0 => Q_n0113,
      I1 => rstate_215,
      I2 => Q_n0103,
      I3 => dcmlocked,
      O => N829
    );
  Q_n0070131 : MUXF5
    port map (
      I0 => N830,
      I1 => N831,
      S => resetDcm_209,
      O => Q_n0070
    );
  Q_n0070131_F : LUT3
    generic map(
      INIT => X"F4"
    )
    port map (
      I0 => prevSamp_208,
      I1 => N460,
      I2 => N447,
      O => N830
    );
  Q_n0070131_G : LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      I0 => N708,
      I1 => N735,
      I2 => N765,
      I3 => N792,
      O => N831
    );
  Q_n0208 : MUXF5
    port map (
      I0 => N832,
      I1 => N833,
      S => resetDcm_209,
      O => Q_n0208_224
    );
  Q_n0208_F : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => dcmlocked,
      I1 => waitTwoCycle_220,
      I2 => N156,
      O => N832
    );
  Q_n0208_G : LUT4
    generic map(
      INIT => X"AA8A"
    )
    port map (
      I0 => dcmlocked,
      I1 => Q_n0113,
      I2 => rstate_215,
      I3 => Q_n0103,
      O => N833
    );
  Q_n0204 : MUXF5
    port map (
      I0 => N834,
      I1 => N835,
      S => resetDcm_209,
      O => Q_n0204_221
    );
  Q_n0204_F : LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      I0 => dcmlocked,
      I1 => waitTwoCycle_220,
      I2 => N156,
      O => N834
    );
  Q_n0204_G : LUT4
    generic map(
      INIT => X"C080"
    )
    port map (
      I0 => Q_n0103,
      I1 => rstate_215,
      I2 => dcmlocked,
      I3 => Q_n0113,
      O => N835
    );
  state_FFd2_In105 : MUXF5
    port map (
      I0 => N836,
      I1 => N837,
      S => psDoneReg_214,
      O => state_FFd2_In
    );
  state_FFd2_In105_F : LUT4
    generic map(
      INIT => X"FA8A"
    )
    port map (
      I0 => state_FFd2_226,
      I1 => state_FFd1_225,
      I2 => state_FFd4_228,
      I3 => state_FFd2_In_map248,
      O => N836
    );
  state_FFd2_In105_G : LUT4
    generic map(
      INIT => X"EA40"
    )
    port map (
      I0 => state_FFd4_228,
      I1 => state_FFd1_225,
      I2 => state_FFd2_In_map230,
      I3 => state_FFd2_In_map248,
      O => N837
    );
  state_FFd1_In_rn_0111 : MUXF5
    port map (
      I0 => N838,
      I1 => N839,
      S => state_FFd4_228,
      O => state_FFd1_In
    );
  state_FFd1_In_rn_0111_F : LUT3
    generic map(
      INIT => X"A2"
    )
    port map (
      I0 => state_FFd1_225,
      I1 => psDoneReg_214,
      I2 => state_FFd2_226,
      O => N838
    );
  state_FFd1_In_rn_0111_G : LUT4
    generic map(
      INIT => X"F8EA"
    )
    port map (
      I0 => state_FFd1_225,
      I1 => N32,
      I2 => state_FFd2_226,
      I3 => state_FFd3_227,
      O => N839
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity cal_reg_INST_1 is
  port (
    clk : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dInp : in STD_LOGIC := 'X'; 
    iReg : out STD_LOGIC; 
    dReg : out STD_LOGIC 
  );
end cal_reg_INST_1;

architecture Structure of cal_reg_INST_1 is
  signal NlwRenamedSig_OI_iReg : STD_LOGIC; 
begin
  iReg <= NlwRenamedSig_OI_iReg;
  ireg_FD : FD
    port map (
      D => dInp,
      C => clk,
      Q => NlwRenamedSig_OI_iReg
    );
  dreg_FD : FD
    port map (
      D => NlwRenamedSig_OI_iReg,
      C => clk,
      Q => dReg
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity cal_reg is
  port (
    clk : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    dInp : in STD_LOGIC := 'X'; 
    iReg : out STD_LOGIC; 
    dReg : out STD_LOGIC 
  );
end cal_reg;

architecture Structure of cal_reg is
  signal NlwRenamedSig_OI_iReg : STD_LOGIC; 
begin
  iReg <= NlwRenamedSig_OI_iReg;
  ireg_FD : FD
    port map (
      D => dInp,
      C => clk,
      Q => NlwRenamedSig_OI_iReg
    );
  dreg_FD : FD
    port map (
      D => NlwRenamedSig_OI_iReg,
      C => clk,
      Q => dReg
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity cal_top is
  port (
    clk : in STD_LOGIC := 'X'; 
    reset : in STD_LOGIC := 'X'; 
    clk0 : in STD_LOGIC := 'X'; 
    clk0dcmlock : in STD_LOGIC := 'X'; 
    okToSelTap : in STD_LOGIC := 'X'; 
    tapForDqs : out STD_LOGIC_VECTOR ( 4 downto 0 ) 
  );
end cal_top;

architecture Structure of cal_top is
  component cal_reg
    port (
      clk : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dInp : in STD_LOGIC := 'X'; 
      iReg : out STD_LOGIC; 
      dReg : out STD_LOGIC 
    );
  end component;
  component cal_reg_INST_1
    port (
      clk : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      dInp : in STD_LOGIC := 'X'; 
      iReg : out STD_LOGIC; 
      dReg : out STD_LOGIC 
    );
  end component;
  component cal_ctl
    port (
      clk : in STD_LOGIC := 'X'; 
      hxSamp1 : in STD_LOGIC := 'X'; 
      dcmlocked : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      phSamp1 : in STD_LOGIC := 'X'; 
      psDone : in STD_LOGIC := 'X'; 
      okToSelTap : in STD_LOGIC := 'X'; 
      locReset : out STD_LOGIC; 
      psInc : out STD_LOGIC; 
      psEn : out STD_LOGIC; 
      selTap : out STD_LOGIC_VECTOR ( 4 downto 0 ); 
      tapForDqs : out STD_LOGIC_VECTOR ( 4 downto 0 ) 
    );
  end component;
  component cal_div2
    port (
      reset : in STD_LOGIC := 'X'; 
      iclk : in STD_LOGIC := 'X'; 
      oclk : out STD_LOGIC 
    );
  end component;
  component cal_div2f
    port (
      reset : in STD_LOGIC := 'X'; 
      iclk : in STD_LOGIC := 'X'; 
      oclk : out STD_LOGIC 
    );
  end component;
  signal fpga_rst_230 : STD_LOGIC; 
  signal suClkDiv2 : STD_LOGIC; 
  signal hxSamp1 : STD_LOGIC; 
  signal dcmlocked : STD_LOGIC; 
  signal suPhClkDiv2 : STD_LOGIC; 
  signal hexClk : STD_LOGIC; 
  signal phShftClkDcm : STD_LOGIC; 
  signal Q_n0011 : STD_LOGIC; 
  signal divRst : STD_LOGIC; 
  signal gnd_231 : STD_LOGIC; 
  signal reset_not : STD_LOGIC; 
  signal psInc : STD_LOGIC; 
  signal clkDiv2 : STD_LOGIC; 
  signal phShftClk : STD_LOGIC; 
  signal phSamp1 : STD_LOGIC; 
  signal psEn : STD_LOGIC; 
  signal psDone : STD_LOGIC; 
  signal phClkDiv2 : STD_LOGIC; 
  signal N1 : STD_LOGIC; 
  signal cal_clkd2_n0003 : STD_LOGIC; 
  signal cal_clkd2_poclk_232 : STD_LOGIC; 
  signal cal_phClkd2_n0003 : STD_LOGIC; 
  signal cal_phClkd2_poclk_233 : STD_LOGIC; 
  signal ckt_to_cal_delay5 : STD_LOGIC; 
  signal ckt_to_cal_delay4 : STD_LOGIC; 
  signal ckt_to_cal_delay3 : STD_LOGIC; 
  signal ckt_to_cal_delay2 : STD_LOGIC; 
  signal ckt_to_cal_delay1 : STD_LOGIC; 
  signal ckt_to_cal_high : STD_LOGIC; 
  signal NLW_cal_ctl0_locReset_UNCONNECTED : STD_LOGIC; 
  signal NLW_hxSampReg0_iReg_UNCONNECTED : STD_LOGIC; 
  signal NLW_phSampReg0_iReg_UNCONNECTED : STD_LOGIC; 
  signal NLW_cal_dcm_STATUS_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_cal_dcm_STATUS_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_cal_dcm_STATUS_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_cal_dcm_STATUS_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_cal_dcm_STATUS_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_cal_dcm_STATUS_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_cal_dcm_STATUS_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_cal_dcm_STATUS_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_cal_dcm_CLK90_UNCONNECTED : STD_LOGIC; 
  signal NLW_cal_dcm_CLK180_UNCONNECTED : STD_LOGIC; 
  signal NLW_cal_dcm_CLK270_UNCONNECTED : STD_LOGIC; 
  signal NLW_cal_dcm_CLK2X_UNCONNECTED : STD_LOGIC; 
  signal NLW_cal_dcm_CLK2X180_UNCONNECTED : STD_LOGIC; 
  signal NLW_cal_dcm_CLKDV_UNCONNECTED : STD_LOGIC; 
  signal NLW_cal_dcm_CLKFX_UNCONNECTED : STD_LOGIC; 
  signal NLW_cal_dcm_CLKFX180_UNCONNECTED : STD_LOGIC; 
  signal selTap : STD_LOGIC_VECTOR ( 4 downto 0 ); 
begin
  XST_GND : GND
    port map (
      G => gnd_231
    );
  XST_VCC : VCC
    port map (
      P => N1
    );
  fpga_rst : FDR
    port map (
      D => N1,
      R => Q_n0011,
      C => clk0,
      Q => fpga_rst_230
    );
  cal_dcm : DCM
    generic map(
      CLKOUT_PHASE_SHIFT => "VARIABLE",
      DLL_FREQUENCY_MODE => "LOW",
      DUTY_CYCLE_CORRECTION => TRUE,
      PHASE_SHIFT => 128
    )
    port map (
      RST => reset_not,
      CLKIN => clk,
      CLKFB => phShftClk,
      PSINCDEC => psInc,
      PSEN => psEn,
      PSCLK => clk0,
      DSSEN => gnd_231,
      CLK0 => phShftClkDcm,
      CLK90 => NLW_cal_dcm_CLK90_UNCONNECTED,
      CLK180 => NLW_cal_dcm_CLK180_UNCONNECTED,
      CLK270 => NLW_cal_dcm_CLK270_UNCONNECTED,
      CLK2X => NLW_cal_dcm_CLK2X_UNCONNECTED,
      CLK2X180 => NLW_cal_dcm_CLK2X180_UNCONNECTED,
      CLKDV => NLW_cal_dcm_CLKDV_UNCONNECTED,
      CLKFX => NLW_cal_dcm_CLKFX_UNCONNECTED,
      CLKFX180 => NLW_cal_dcm_CLKFX180_UNCONNECTED,
      LOCKED => dcmlocked,
      PSDONE => psDone,
      STATUS(7) => NLW_cal_dcm_STATUS_7_UNCONNECTED,
      STATUS(6) => NLW_cal_dcm_STATUS_6_UNCONNECTED,
      STATUS(5) => NLW_cal_dcm_STATUS_5_UNCONNECTED,
      STATUS(4) => NLW_cal_dcm_STATUS_4_UNCONNECTED,
      STATUS(3) => NLW_cal_dcm_STATUS_3_UNCONNECTED,
      STATUS(2) => NLW_cal_dcm_STATUS_2_UNCONNECTED,
      STATUS(1) => NLW_cal_dcm_STATUS_1_UNCONNECTED,
      STATUS(0) => NLW_cal_dcm_STATUS_0_UNCONNECTED
    );
  phclk_bufg : BUFG
    port map (
      I => phShftClkDcm,
      O => phShftClk
    );
  phSampReg0 : cal_reg
    port map (
      clk => suPhClkDiv2,
      reset => fpga_rst_230,
      dInp => suClkDiv2,
      iReg => NLW_phSampReg0_iReg_UNCONNECTED,
      dReg => phSamp1
    );
  hxSampReg0 : cal_reg_INST_1
    port map (
      clk => hexClk,
      reset => fpga_rst_230,
      dInp => clkDiv2,
      iReg => NLW_hxSampReg0_iReg_UNCONNECTED,
      dReg => hxSamp1
    );
  reset_not1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => reset,
      O => reset_not
    );
  divRst1 : LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      I0 => clk0dcmlock,
      I1 => dcmlocked,
      O => divRst
    );
  Q_n00111 : LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      I0 => clk0dcmlock,
      I1 => dcmlocked,
      I2 => reset,
      O => Q_n0011
    );
  cal_ctl0 : cal_ctl
    port map (
      clk => clk0,
      hxSamp1 => hxSamp1,
      dcmlocked => dcmlocked,
      reset => fpga_rst_230,
      phSamp1 => phSamp1,
      psDone => psDone,
      okToSelTap => N1,
      locReset => NLW_cal_ctl0_locReset_UNCONNECTED,
      psInc => psInc,
      psEn => psEn,
      selTap(4) => selTap(4),
      selTap(3) => selTap(3),
      selTap(2) => selTap(2),
      selTap(1) => selTap(1),
      selTap(0) => selTap(0),
      tapForDqs(4) => tapForDqs(4),
      tapForDqs(3) => tapForDqs(3),
      tapForDqs(2) => tapForDqs(2),
      tapForDqs(1) => tapForDqs(1),
      tapForDqs(0) => tapForDqs(0)
    );
  cal_suClkd2 : cal_div2
    port map (
      reset => divRst,
      iclk => clk0,
      oclk => suClkDiv2
    );
  cal_suPhClkd2 : cal_div2f
    port map (
      reset => divRst,
      iclk => phShftClk,
      oclk => suPhClkDiv2
    );
  cal_clkd2_n00031 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => cal_clkd2_poclk_232,
      O => cal_clkd2_n0003
    );
  cal_clkd2_oclk : FDR
    port map (
      D => cal_clkd2_poclk_232,
      R => divRst,
      C => clk0,
      Q => clkDiv2
    );
  cal_clkd2_poclk : FDR
    port map (
      D => cal_clkd2_n0003,
      R => divRst,
      C => clk0,
      Q => cal_clkd2_poclk_232
    );
  cal_phClkd2_n00031 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => cal_phClkd2_poclk_233,
      O => cal_phClkd2_n0003
    );
  cal_phClkd2_oclk : FDR_1
    port map (
      D => cal_phClkd2_poclk_233,
      R => divRst,
      C => phShftClk,
      Q => phClkDiv2
    );
  cal_phClkd2_poclk : FDR_1
    port map (
      D => cal_phClkd2_n0003,
      R => divRst,
      C => phShftClk,
      Q => cal_phClkd2_poclk_233
    );
  ckt_to_cal_six : LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      I0 => phClkDiv2,
      I1 => selTap(1),
      I2 => ckt_to_cal_delay2,
      I3 => ckt_to_cal_high,
      O => ckt_to_cal_delay3
    );
  ckt_to_cal_five : LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      I0 => ckt_to_cal_high,
      I1 => selTap(3),
      I2 => ckt_to_cal_delay4,
      I3 => phClkDiv2,
      O => ckt_to_cal_delay5
    );
  ckt_to_cal_four : LUT4
    generic map(
      INIT => X"FF00"
    )
    port map (
      I0 => ckt_to_cal_high,
      I1 => ckt_to_cal_high,
      I2 => ckt_to_cal_high,
      I3 => phClkDiv2,
      O => ckt_to_cal_delay1
    );
  ckt_to_cal_three : LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      I0 => phClkDiv2,
      I1 => selTap(0),
      I2 => ckt_to_cal_delay1,
      I3 => ckt_to_cal_high,
      O => ckt_to_cal_delay2
    );
  ckt_to_cal_two : LUT4
    generic map(
      INIT => X"EE22"
    )
    port map (
      I0 => phClkDiv2,
      I1 => selTap(2),
      I2 => ckt_to_cal_high,
      I3 => ckt_to_cal_delay3,
      O => ckt_to_cal_delay4
    );
  ckt_to_cal_one : LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      I0 => ckt_to_cal_high,
      I1 => selTap(4),
      I2 => ckt_to_cal_delay5,
      I3 => phClkDiv2,
      O => hexClk
    );
  ckt_to_cal_XST_VCC : VCC
    port map (
      P => ckt_to_cal_high
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity infrastructure_top is
  port (
    sys_clk_ibuf : in STD_LOGIC := 'X'; 
    reset_in : in STD_LOGIC := 'X'; 
    sys_rst180_val : out STD_LOGIC; 
    sys_rst_val : out STD_LOGIC; 
    sys_rst90_val : out STD_LOGIC; 
    wait_200us : out STD_LOGIC; 
    clk90_int_val : out STD_LOGIC; 
    clk_int_val : out STD_LOGIC; 
    sys_rst270_val : out STD_LOGIC; 
    delay_sel_val1_val : out STD_LOGIC_VECTOR ( 4 downto 0 ) 
  );
end infrastructure_top;

architecture Structure of infrastructure_top is
  component cal_top
    port (
      clk : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      clk0 : in STD_LOGIC := 'X'; 
      clk0dcmlock : in STD_LOGIC := 'X'; 
      okToSelTap : in STD_LOGIC := 'X'; 
      tapForDqs : out STD_LOGIC_VECTOR ( 4 downto 0 ) 
    );
  end component;
  component dcmx3y0_2vp20
    port (
      clock1_in : in STD_LOGIC := 'X'; 
      clock2_in : in STD_LOGIC := 'X'; 
      clock2_out : out STD_LOGIC; 
      clock1_out : out STD_LOGIC 
    );
  end component;
  signal NlwRenamedSig_OI_wait_200us : STD_LOGIC; 
  signal NlwRenamedSig_OI_clk90_int_val : STD_LOGIC; 
  signal NlwRenamedSig_OI_clk_int_val : STD_LOGIC; 
  signal sys_rst270_1_234 : STD_LOGIC; 
  signal wait_clk90_235 : STD_LOGIC; 
  signal dcm_lock : STD_LOGIC; 
  signal sys_rst_1_236 : STD_LOGIC; 
  signal vcc_237 : STD_LOGIC; 
  signal sys_rst270_o_238 : STD_LOGIC; 
  signal sys_rst90_1_239 : STD_LOGIC; 
  signal Q_n0013 : STD_LOGIC; 
  signal Q_n0020 : STD_LOGIC; 
  signal Q_n0015 : STD_LOGIC; 
  signal Q_n0021 : STD_LOGIC; 
  signal Q_n0016 : STD_LOGIC; 
  signal Q_n0022 : STD_LOGIC; 
  signal Q_n0017 : STD_LOGIC; 
  signal sys_rst_o_240 : STD_LOGIC; 
  signal sys_rst90_o_241 : STD_LOGIC; 
  signal user_rst : STD_LOGIC; 
  signal sys_rst180_1_242 : STD_LOGIC; 
  signal sys_rst180_o_243 : STD_LOGIC; 
  signal N1 : STD_LOGIC; 
  signal infrastructure_top_Result_0_cyo : STD_LOGIC; 
  signal infrastructure_top_Result_1_cyo : STD_LOGIC; 
  signal infrastructure_top_Result_2_cyo : STD_LOGIC; 
  signal infrastructure_top_Result_3_cyo : STD_LOGIC; 
  signal infrastructure_top_Result_4_cyo : STD_LOGIC; 
  signal infrastructure_top_Result_5_cyo : STD_LOGIC; 
  signal infrastructure_top_Result_6_cyo : STD_LOGIC; 
  signal infrastructure_top_Result_7_cyo : STD_LOGIC; 
  signal infrastructure_top_Result_8_cyo : STD_LOGIC; 
  signal infrastructure_top_Result_9_cyo : STD_LOGIC; 
  signal infrastructure_top_Result_10_cyo : STD_LOGIC; 
  signal infrastructure_top_Result_11_cyo : STD_LOGIC; 
  signal infrastructure_top_Result_12_cyo : STD_LOGIC; 
  signal infrastructure_top_Result_13_cyo : STD_LOGIC; 
  signal infrastructure_top_Result_14_cyo : STD_LOGIC; 
  signal Counter200_1_rt_244 : STD_LOGIC; 
  signal Counter200_2_rt_245 : STD_LOGIC; 
  signal Counter200_3_rt_246 : STD_LOGIC; 
  signal Counter200_4_rt_247 : STD_LOGIC; 
  signal Counter200_5_rt_248 : STD_LOGIC; 
  signal Counter200_6_rt_249 : STD_LOGIC; 
  signal Counter200_7_rt_250 : STD_LOGIC; 
  signal Counter200_8_rt_251 : STD_LOGIC; 
  signal Counter200_9_rt_252 : STD_LOGIC; 
  signal Counter200_10_rt_253 : STD_LOGIC; 
  signal Counter200_11_rt_254 : STD_LOGIC; 
  signal Counter200_12_rt_255 : STD_LOGIC; 
  signal Counter200_13_rt_256 : STD_LOGIC; 
  signal Counter200_14_rt_257 : STD_LOGIC; 
  signal Counter200_15_rt_258 : STD_LOGIC; 
  signal clk_dcm0_clk90dcm : STD_LOGIC; 
  signal clk_dcm0_clk0fb : STD_LOGIC; 
  signal clk_dcm0_clk90d2inv : STD_LOGIC; 
  signal clk_dcm0_clk0dcm : STD_LOGIC; 
  signal clk_dcm0_clk0_o : STD_LOGIC; 
  signal clk_dcm0_gnd : STD_LOGIC; 
  signal clk_dcm0_clk_half_buf : STD_LOGIC; 
  signal clk_dcm0_rst_delay5_259 : STD_LOGIC; 
  signal clk_dcm0_rst_delay4_260 : STD_LOGIC; 
  signal clk_dcm0_rst_delay3_261 : STD_LOGIC; 
  signal clk_dcm0_rst_delay2_262 : STD_LOGIC; 
  signal clk_dcm0_rst_delay1_263 : STD_LOGIC; 
  signal clk_dcm0_clk0d2inv : STD_LOGIC; 
  signal clk_dcm0_clk_half : STD_LOGIC; 
  signal clk_dcm0_clk0fb_buf : STD_LOGIC; 
  signal clk_dcm0_clk0_o_buf : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST_DV_STATUS_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST_DV_STATUS_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST_DV_STATUS_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST_DV_STATUS_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST_DV_STATUS_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST_DV_STATUS_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST_DV_STATUS_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST_DV_STATUS_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST_DV_CLK90_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST_DV_CLK180_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST_DV_CLK270_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST_DV_CLK2X_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST_DV_CLK2X180_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST_DV_CLKFX_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST_DV_CLKFX180_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST_DV_LOCKED_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST_DV_PSDONE_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST1_STATUS_7_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST1_STATUS_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST1_STATUS_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST1_STATUS_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST1_STATUS_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST1_STATUS_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST1_STATUS_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST1_STATUS_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST1_CLK270_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST1_CLK2X_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST1_CLK2X180_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST1_CLKDV_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST1_CLKFX_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST1_CLKFX180_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk_dcm0_DCM_INST1_PSDONE_UNCONNECTED : STD_LOGIC; 
  signal Counter200 : STD_LOGIC_VECTOR ( 15 downto 0 ); 
  signal Result : STD_LOGIC_VECTOR ( 15 downto 0 ); 
begin
  wait_200us <= NlwRenamedSig_OI_wait_200us;
  clk90_int_val <= NlwRenamedSig_OI_clk90_int_val;
  clk_int_val <= NlwRenamedSig_OI_clk_int_val;
  XST_VCC : VCC
    port map (
      P => vcc_237
    );
  XST_GND : GND
    port map (
      G => N1
    );
  sys_rst90_o : FDR
    port map (
      D => vcc_237,
      R => Q_n0020,
      C => NlwRenamedSig_OI_clk90_int_val,
      Q => sys_rst90_o_241
    );
  sys_rst180_o : FDR_1
    port map (
      D => vcc_237,
      R => Q_n0021,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => sys_rst180_o_243
    );
  wait_200us_i : FDSE
    port map (
      D => N1,
      S => Q_n0013,
      CE => Q_n0015,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => NlwRenamedSig_OI_wait_200us
    );
  sys_rst270_o : FDR_1
    port map (
      D => vcc_237,
      R => Q_n0020,
      C => NlwRenamedSig_OI_clk90_int_val,
      Q => sys_rst270_o_238
    );
  sys_rst_o : FDR
    port map (
      D => vcc_237,
      R => Q_n0021,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => sys_rst_o_240
    );
  sys_rst270_1 : FDS_1
    generic map(
      INIT => '1'
    )
    port map (
      D => sys_rst270_o_238,
      S => Q_n0016,
      C => NlwRenamedSig_OI_clk90_int_val,
      Q => sys_rst270_1_234
    );
  sys_rst_1 : FDS
    generic map(
      INIT => '1'
    )
    port map (
      D => sys_rst_o_240,
      S => Q_n0017,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => sys_rst_1_236
    );
  wait_clk90 : FDS
    port map (
      D => NlwRenamedSig_OI_wait_200us,
      S => Q_n0013,
      C => NlwRenamedSig_OI_clk90_int_val,
      Q => wait_clk90_235
    );
  sys_rst90_1 : FDS
    generic map(
      INIT => '1'
    )
    port map (
      D => sys_rst90_o_241,
      S => Q_n0016,
      C => NlwRenamedSig_OI_clk90_int_val,
      Q => sys_rst90_1_239
    );
  sys_rst180_1 : FDS_1
    generic map(
      INIT => '1'
    )
    port map (
      D => sys_rst180_o_243,
      S => Q_n0017,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => sys_rst180_1_242
    );
  sys_rst180 : FDS_1
    port map (
      D => sys_rst180_1_242,
      S => Q_n0017,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => sys_rst180_val
    );
  sys_rst270 : FDS_1
    port map (
      D => sys_rst270_1_234,
      S => Q_n0016,
      C => NlwRenamedSig_OI_clk90_int_val,
      Q => sys_rst270_val
    );
  sys_rst90 : FDS
    port map (
      D => sys_rst90_1_239,
      S => Q_n0016,
      C => NlwRenamedSig_OI_clk90_int_val,
      Q => sys_rst90_val
    );
  sys_rst : FDS
    port map (
      D => sys_rst_1_236,
      S => Q_n0017,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => sys_rst_val
    );
  Counter200_0 : FDRE
    port map (
      D => Result(0),
      R => Q_n0013,
      CE => Q_n0022,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => Counter200(0)
    );
  Counter200_1 : FDRE
    port map (
      D => Result(1),
      R => Q_n0013,
      CE => Q_n0022,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => Counter200(1)
    );
  Counter200_2 : FDRE
    port map (
      D => Result(2),
      R => Q_n0013,
      CE => Q_n0022,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => Counter200(2)
    );
  Counter200_3 : FDRE
    port map (
      D => Result(3),
      R => Q_n0013,
      CE => Q_n0022,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => Counter200(3)
    );
  Counter200_4 : FDRE
    port map (
      D => Result(4),
      R => Q_n0013,
      CE => Q_n0022,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => Counter200(4)
    );
  Counter200_5 : FDRE
    port map (
      D => Result(5),
      R => Q_n0013,
      CE => Q_n0022,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => Counter200(5)
    );
  Counter200_6 : FDRE
    port map (
      D => Result(6),
      R => Q_n0013,
      CE => Q_n0022,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => Counter200(6)
    );
  Counter200_7 : FDRE
    port map (
      D => Result(7),
      R => Q_n0013,
      CE => Q_n0022,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => Counter200(7)
    );
  Counter200_8 : FDRE
    port map (
      D => Result(8),
      R => Q_n0013,
      CE => Q_n0022,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => Counter200(8)
    );
  Counter200_9 : FDRE
    port map (
      D => Result(9),
      R => Q_n0013,
      CE => Q_n0022,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => Counter200(9)
    );
  Counter200_10 : FDRE
    port map (
      D => Result(10),
      R => Q_n0013,
      CE => Q_n0022,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => Counter200(10)
    );
  Counter200_11 : FDRE
    port map (
      D => Result(11),
      R => Q_n0013,
      CE => Q_n0022,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => Counter200(11)
    );
  Counter200_12 : FDRE
    port map (
      D => Result(12),
      R => Q_n0013,
      CE => Q_n0022,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => Counter200(12)
    );
  Counter200_13 : FDRE
    port map (
      D => Result(13),
      R => Q_n0013,
      CE => Q_n0022,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => Counter200(13)
    );
  Counter200_14 : FDRE
    port map (
      D => Result(14),
      R => Q_n0013,
      CE => Q_n0022,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => Counter200(14)
    );
  Counter200_15 : FDRE
    port map (
      D => Result(15),
      R => Q_n0013,
      CE => Q_n0022,
      C => NlwRenamedSig_OI_clk_int_val,
      Q => Counter200(15)
    );
  infrastructure_top_Result_0_lut : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => Counter200(0),
      O => Result(0)
    );
  infrastructure_top_Result_0_cy : MUXCY
    port map (
      CI => N1,
      DI => vcc_237,
      S => Result(0),
      O => infrastructure_top_Result_0_cyo
    );
  infrastructure_top_Result_1_cy : MUXCY
    port map (
      CI => infrastructure_top_Result_0_cyo,
      DI => N1,
      S => Counter200_1_rt_244,
      O => infrastructure_top_Result_1_cyo
    );
  infrastructure_top_Result_1_xor : XORCY
    port map (
      CI => infrastructure_top_Result_0_cyo,
      LI => Counter200_1_rt_244,
      O => Result(1)
    );
  infrastructure_top_Result_2_cy : MUXCY
    port map (
      CI => infrastructure_top_Result_1_cyo,
      DI => N1,
      S => Counter200_2_rt_245,
      O => infrastructure_top_Result_2_cyo
    );
  infrastructure_top_Result_2_xor : XORCY
    port map (
      CI => infrastructure_top_Result_1_cyo,
      LI => Counter200_2_rt_245,
      O => Result(2)
    );
  infrastructure_top_Result_3_cy : MUXCY
    port map (
      CI => infrastructure_top_Result_2_cyo,
      DI => N1,
      S => Counter200_3_rt_246,
      O => infrastructure_top_Result_3_cyo
    );
  infrastructure_top_Result_3_xor : XORCY
    port map (
      CI => infrastructure_top_Result_2_cyo,
      LI => Counter200_3_rt_246,
      O => Result(3)
    );
  infrastructure_top_Result_4_cy : MUXCY
    port map (
      CI => infrastructure_top_Result_3_cyo,
      DI => N1,
      S => Counter200_4_rt_247,
      O => infrastructure_top_Result_4_cyo
    );
  infrastructure_top_Result_4_xor : XORCY
    port map (
      CI => infrastructure_top_Result_3_cyo,
      LI => Counter200_4_rt_247,
      O => Result(4)
    );
  infrastructure_top_Result_5_cy : MUXCY
    port map (
      CI => infrastructure_top_Result_4_cyo,
      DI => N1,
      S => Counter200_5_rt_248,
      O => infrastructure_top_Result_5_cyo
    );
  infrastructure_top_Result_5_xor : XORCY
    port map (
      CI => infrastructure_top_Result_4_cyo,
      LI => Counter200_5_rt_248,
      O => Result(5)
    );
  infrastructure_top_Result_6_cy : MUXCY
    port map (
      CI => infrastructure_top_Result_5_cyo,
      DI => N1,
      S => Counter200_6_rt_249,
      O => infrastructure_top_Result_6_cyo
    );
  infrastructure_top_Result_6_xor : XORCY
    port map (
      CI => infrastructure_top_Result_5_cyo,
      LI => Counter200_6_rt_249,
      O => Result(6)
    );
  infrastructure_top_Result_7_cy : MUXCY
    port map (
      CI => infrastructure_top_Result_6_cyo,
      DI => N1,
      S => Counter200_7_rt_250,
      O => infrastructure_top_Result_7_cyo
    );
  infrastructure_top_Result_7_xor : XORCY
    port map (
      CI => infrastructure_top_Result_6_cyo,
      LI => Counter200_7_rt_250,
      O => Result(7)
    );
  infrastructure_top_Result_8_cy : MUXCY
    port map (
      CI => infrastructure_top_Result_7_cyo,
      DI => N1,
      S => Counter200_8_rt_251,
      O => infrastructure_top_Result_8_cyo
    );
  infrastructure_top_Result_8_xor : XORCY
    port map (
      CI => infrastructure_top_Result_7_cyo,
      LI => Counter200_8_rt_251,
      O => Result(8)
    );
  infrastructure_top_Result_9_cy : MUXCY
    port map (
      CI => infrastructure_top_Result_8_cyo,
      DI => N1,
      S => Counter200_9_rt_252,
      O => infrastructure_top_Result_9_cyo
    );
  infrastructure_top_Result_9_xor : XORCY
    port map (
      CI => infrastructure_top_Result_8_cyo,
      LI => Counter200_9_rt_252,
      O => Result(9)
    );
  infrastructure_top_Result_10_cy : MUXCY
    port map (
      CI => infrastructure_top_Result_9_cyo,
      DI => N1,
      S => Counter200_10_rt_253,
      O => infrastructure_top_Result_10_cyo
    );
  infrastructure_top_Result_10_xor : XORCY
    port map (
      CI => infrastructure_top_Result_9_cyo,
      LI => Counter200_10_rt_253,
      O => Result(10)
    );
  infrastructure_top_Result_11_cy : MUXCY
    port map (
      CI => infrastructure_top_Result_10_cyo,
      DI => N1,
      S => Counter200_11_rt_254,
      O => infrastructure_top_Result_11_cyo
    );
  infrastructure_top_Result_11_xor : XORCY
    port map (
      CI => infrastructure_top_Result_10_cyo,
      LI => Counter200_11_rt_254,
      O => Result(11)
    );
  infrastructure_top_Result_12_cy : MUXCY
    port map (
      CI => infrastructure_top_Result_11_cyo,
      DI => N1,
      S => Counter200_12_rt_255,
      O => infrastructure_top_Result_12_cyo
    );
  infrastructure_top_Result_12_xor : XORCY
    port map (
      CI => infrastructure_top_Result_11_cyo,
      LI => Counter200_12_rt_255,
      O => Result(12)
    );
  infrastructure_top_Result_13_cy : MUXCY
    port map (
      CI => infrastructure_top_Result_12_cyo,
      DI => N1,
      S => Counter200_13_rt_256,
      O => infrastructure_top_Result_13_cyo
    );
  infrastructure_top_Result_13_xor : XORCY
    port map (
      CI => infrastructure_top_Result_12_cyo,
      LI => Counter200_13_rt_256,
      O => Result(13)
    );
  infrastructure_top_Result_14_cy : MUXCY
    port map (
      CI => infrastructure_top_Result_13_cyo,
      DI => N1,
      S => Counter200_14_rt_257,
      O => infrastructure_top_Result_14_cyo
    );
  infrastructure_top_Result_14_xor : XORCY
    port map (
      CI => infrastructure_top_Result_13_cyo,
      LI => Counter200_14_rt_257,
      O => Result(14)
    );
  infrastructure_top_Result_15_xor : XORCY
    port map (
      CI => infrastructure_top_Result_14_cyo,
      LI => Counter200_15_rt_258,
      O => Result(15)
    );
  user_rst1 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => reset_in,
      O => user_rst
    );
  Q_n00131 : LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      I0 => dcm_lock,
      I1 => reset_in,
      O => Q_n0013
    );
  Q_n00151 : LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      I0 => Counter200(13),
      I1 => Counter200(14),
      I2 => Counter200(15),
      I3 => NlwRenamedSig_OI_wait_200us,
      O => Q_n0015
    );
  Q_n00201 : LUT3
    generic map(
      INIT => X"08"
    )
    port map (
      I0 => dcm_lock,
      I1 => reset_in,
      I2 => wait_clk90_235,
      O => Q_n0020
    );
  Q_n00211 : LUT3
    generic map(
      INIT => X"08"
    )
    port map (
      I0 => dcm_lock,
      I1 => reset_in,
      I2 => NlwRenamedSig_OI_wait_200us,
      O => Q_n0021
    );
  Q_n00221 : LUT4
    generic map(
      INIT => X"4CCC"
    )
    port map (
      I0 => Counter200(13),
      I1 => NlwRenamedSig_OI_wait_200us,
      I2 => Counter200(14),
      I3 => Counter200(15),
      O => Q_n0022
    );
  cal_top0 : cal_top
    port map (
      clk => sys_clk_ibuf,
      reset => reset_in,
      clk0 => NlwRenamedSig_OI_clk_int_val,
      clk0dcmlock => dcm_lock,
      okToSelTap => vcc_237,
      tapForDqs(4) => delay_sel_val1_val(4),
      tapForDqs(3) => delay_sel_val1_val(3),
      tapForDqs(2) => delay_sel_val1_val(2),
      tapForDqs(1) => delay_sel_val1_val(1),
      tapForDqs(0) => delay_sel_val1_val(0)
    );
  Q_n00171 : LUT3
    generic map(
      INIT => X"F7"
    )
    port map (
      I0 => dcm_lock,
      I1 => reset_in,
      I2 => NlwRenamedSig_OI_wait_200us,
      O => Q_n0017
    );
  Q_n00161 : LUT3
    generic map(
      INIT => X"F7"
    )
    port map (
      I0 => dcm_lock,
      I1 => reset_in,
      I2 => wait_clk90_235,
      O => Q_n0016
    );
  Counter200_1_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Counter200(1),
      O => Counter200_1_rt_244
    );
  Counter200_2_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Counter200(2),
      O => Counter200_2_rt_245
    );
  Counter200_3_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Counter200(3),
      O => Counter200_3_rt_246
    );
  Counter200_4_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Counter200(4),
      O => Counter200_4_rt_247
    );
  Counter200_5_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Counter200(5),
      O => Counter200_5_rt_248
    );
  Counter200_6_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Counter200(6),
      O => Counter200_6_rt_249
    );
  Counter200_7_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Counter200(7),
      O => Counter200_7_rt_250
    );
  Counter200_8_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Counter200(8),
      O => Counter200_8_rt_251
    );
  Counter200_9_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Counter200(9),
      O => Counter200_9_rt_252
    );
  Counter200_10_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Counter200(10),
      O => Counter200_10_rt_253
    );
  Counter200_11_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Counter200(11),
      O => Counter200_11_rt_254
    );
  Counter200_12_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Counter200(12),
      O => Counter200_12_rt_255
    );
  Counter200_13_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Counter200(13),
      O => Counter200_13_rt_256
    );
  Counter200_14_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Counter200(14),
      O => Counter200_14_rt_257
    );
  Counter200_15_rt : LUT1
    generic map(
      INIT => X"2"
    )
    port map (
      I0 => Counter200(15),
      O => Counter200_15_rt_258
    );
  clk_dcm0_BUFG_CLK90 : BUFG
    port map (
      I => clk_dcm0_clk90d2inv,
      O => NlwRenamedSig_OI_clk90_int_val
    );
  clk_dcm0_BUFG_CLK0 : BUFG
    port map (
      I => clk_dcm0_clk0d2inv,
      O => NlwRenamedSig_OI_clk_int_val
    );
  clk_dcm0_BUFG_CLKFB : BUFG
    port map (
      I => clk_dcm0_clk0fb,
      O => clk_dcm0_clk0fb_buf
    );
  clk_dcm0_BUFG_CLKDV : BUFG
    port map (
      I => clk_dcm0_clk_half,
      O => clk_dcm0_clk_half_buf
    );
  clk_dcm0_BUFG_CLKDV2 : BUFG
    port map (
      I => clk_dcm0_clk0_o,
      O => clk_dcm0_clk0_o_buf
    );
  clk_dcm0_DCM_INST_DV : DCM
    port map (
      RST => user_rst,
      CLKIN => sys_clk_ibuf,
      CLKFB => clk_dcm0_clk0_o_buf,
      PSINCDEC => clk_dcm0_gnd,
      PSEN => clk_dcm0_gnd,
      PSCLK => clk_dcm0_gnd,
      DSSEN => clk_dcm0_gnd,
      CLK0 => clk_dcm0_clk0_o,
      CLK90 => NLW_clk_dcm0_DCM_INST_DV_CLK90_UNCONNECTED,
      CLK180 => NLW_clk_dcm0_DCM_INST_DV_CLK180_UNCONNECTED,
      CLK270 => NLW_clk_dcm0_DCM_INST_DV_CLK270_UNCONNECTED,
      CLK2X => NLW_clk_dcm0_DCM_INST_DV_CLK2X_UNCONNECTED,
      CLK2X180 => NLW_clk_dcm0_DCM_INST_DV_CLK2X180_UNCONNECTED,
      CLKDV => clk_dcm0_clk_half,
      CLKFX => NLW_clk_dcm0_DCM_INST_DV_CLKFX_UNCONNECTED,
      CLKFX180 => NLW_clk_dcm0_DCM_INST_DV_CLKFX180_UNCONNECTED,
      LOCKED => NLW_clk_dcm0_DCM_INST_DV_LOCKED_UNCONNECTED,
      PSDONE => NLW_clk_dcm0_DCM_INST_DV_PSDONE_UNCONNECTED,
      STATUS(7) => NLW_clk_dcm0_DCM_INST_DV_STATUS_7_UNCONNECTED,
      STATUS(6) => NLW_clk_dcm0_DCM_INST_DV_STATUS_6_UNCONNECTED,
      STATUS(5) => NLW_clk_dcm0_DCM_INST_DV_STATUS_5_UNCONNECTED,
      STATUS(4) => NLW_clk_dcm0_DCM_INST_DV_STATUS_4_UNCONNECTED,
      STATUS(3) => NLW_clk_dcm0_DCM_INST_DV_STATUS_3_UNCONNECTED,
      STATUS(2) => NLW_clk_dcm0_DCM_INST_DV_STATUS_2_UNCONNECTED,
      STATUS(1) => NLW_clk_dcm0_DCM_INST_DV_STATUS_1_UNCONNECTED,
      STATUS(0) => NLW_clk_dcm0_DCM_INST_DV_STATUS_0_UNCONNECTED
    );
  clk_dcm0_DCM_INST1 : DCM
    generic map(
      DLL_FREQUENCY_MODE => "LOW",
      DUTY_CYCLE_CORRECTION => TRUE
    )
    port map (
      RST => clk_dcm0_rst_delay5_259,
      CLKIN => clk_dcm0_clk_half_buf,
      CLKFB => clk_dcm0_clk0fb_buf,
      PSINCDEC => clk_dcm0_gnd,
      PSEN => clk_dcm0_gnd,
      PSCLK => clk_dcm0_gnd,
      DSSEN => clk_dcm0_gnd,
      CLK0 => clk_dcm0_clk0fb,
      CLK90 => clk_dcm0_clk0dcm,
      CLK180 => clk_dcm0_clk90dcm,
      CLK270 => NLW_clk_dcm0_DCM_INST1_CLK270_UNCONNECTED,
      CLK2X => NLW_clk_dcm0_DCM_INST1_CLK2X_UNCONNECTED,
      CLK2X180 => NLW_clk_dcm0_DCM_INST1_CLK2X180_UNCONNECTED,
      CLKDV => NLW_clk_dcm0_DCM_INST1_CLKDV_UNCONNECTED,
      CLKFX => NLW_clk_dcm0_DCM_INST1_CLKFX_UNCONNECTED,
      CLKFX180 => NLW_clk_dcm0_DCM_INST1_CLKFX180_UNCONNECTED,
      LOCKED => dcm_lock,
      PSDONE => NLW_clk_dcm0_DCM_INST1_PSDONE_UNCONNECTED,
      STATUS(7) => NLW_clk_dcm0_DCM_INST1_STATUS_7_UNCONNECTED,
      STATUS(6) => NLW_clk_dcm0_DCM_INST1_STATUS_6_UNCONNECTED,
      STATUS(5) => NLW_clk_dcm0_DCM_INST1_STATUS_5_UNCONNECTED,
      STATUS(4) => NLW_clk_dcm0_DCM_INST1_STATUS_4_UNCONNECTED,
      STATUS(3) => NLW_clk_dcm0_DCM_INST1_STATUS_3_UNCONNECTED,
      STATUS(2) => NLW_clk_dcm0_DCM_INST1_STATUS_2_UNCONNECTED,
      STATUS(1) => NLW_clk_dcm0_DCM_INST1_STATUS_1_UNCONNECTED,
      STATUS(0) => NLW_clk_dcm0_DCM_INST1_STATUS_0_UNCONNECTED
    );
  clk_dcm0_rst_delay5 : FDP
    port map (
      D => clk_dcm0_rst_delay4_260,
      PRE => user_rst,
      C => clk_dcm0_clk_half,
      Q => clk_dcm0_rst_delay5_259
    );
  clk_dcm0_rst_delay4 : FDP
    port map (
      D => clk_dcm0_rst_delay3_261,
      PRE => user_rst,
      C => clk_dcm0_clk_half,
      Q => clk_dcm0_rst_delay4_260
    );
  clk_dcm0_rst_delay3 : FDP
    port map (
      D => clk_dcm0_rst_delay2_262,
      PRE => user_rst,
      C => clk_dcm0_clk_half,
      Q => clk_dcm0_rst_delay3_261
    );
  clk_dcm0_rst_delay2 : FDP
    port map (
      D => clk_dcm0_rst_delay1_263,
      PRE => user_rst,
      C => clk_dcm0_clk_half,
      Q => clk_dcm0_rst_delay2_262
    );
  clk_dcm0_rst_delay1 : FDP
    port map (
      D => clk_dcm0_gnd,
      PRE => user_rst,
      C => clk_dcm0_clk_half,
      Q => clk_dcm0_rst_delay1_263
    );
  clk_dcm0_DCD0 : dcmx3y0_2vp20
    port map (
      clock1_in => clk_dcm0_clk0dcm,
      clock2_in => clk_dcm0_clk90dcm,
      clock2_out => clk_dcm0_clk90d2inv,
      clock1_out => clk_dcm0_clk0d2inv
    );
  clk_dcm0_XST_GND : GND
    port map (
      G => clk_dcm0_gnd
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity mem_interface_top is
  port (
    cntrl0_rst_dqs_div_in : in STD_LOGIC := 'X'; 
    cntrl0_burst_done : in STD_LOGIC := 'X'; 
    reset_in : in STD_LOGIC := 'X'; 
    dip1 : in STD_LOGIC := 'X'; 
    dip2 : in STD_LOGIC := 'X'; 
    dip3 : in STD_LOGIC := 'X'; 
    SYS_CLK : in STD_LOGIC := 'X'; 
    sys_rst270_1 : out STD_LOGIC; 
    cntrl0_ddr2_clk0b : out STD_LOGIC; 
    cntrl0_ddr2_cke : out STD_LOGIC; 
    sys_rst_1 : out STD_LOGIC; 
    clk180_1 : out STD_LOGIC; 
    sys_rst90_1 : out STD_LOGIC; 
    cntrl0_rst_dqs_div_out : out STD_LOGIC; 
    cntrl0_init_val : out STD_LOGIC; 
    cntrl0_ddr2_csb : out STD_LOGIC; 
    cntrl0_user_data_valid : out STD_LOGIC; 
    clk_int_1 : out STD_LOGIC; 
    cntrl0_ar_done : out STD_LOGIC; 
    cntrl0_auto_ref_req : out STD_LOGIC; 
    sys_rst180_1 : out STD_LOGIC; 
    cntrl0_ddr2_rasb : out STD_LOGIC; 
    cntrl0_ddr2_web : out STD_LOGIC; 
    cntrl0_user_cmd_ack : out STD_LOGIC; 
    clk90_int_1 : out STD_LOGIC; 
    clk270_1 : out STD_LOGIC; 
    cntrl0_ddr2_casb : out STD_LOGIC; 
    cntrl0_ddr2_ODT0 : out STD_LOGIC; 
    cntrl0_ddr2_clk0 : out STD_LOGIC; 
    cntrl0_user_data_mask : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    cntrl0_user_bank_address : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
    cntrl0_user_command_register : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
    cntrl0_user_input_address : in STD_LOGIC_VECTOR ( 22 downto 0 ); 
    cntrl0_user_config_register1 : in STD_LOGIC_VECTOR ( 14 downto 0 ); 
    cntrl0_user_config_register2 : in STD_LOGIC_VECTOR ( 12 downto 0 ); 
    cntrl0_user_input_data : in STD_LOGIC_VECTOR ( 31 downto 0 ); 
    cntrl0_ddr2_dqs_n : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
    cntrl0_ddr2_dqs : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
    cntrl0_ddr2_dq : inout STD_LOGIC_VECTOR ( 15 downto 0 ); 
    cntrl0_ddr2_address : out STD_LOGIC_VECTOR ( 12 downto 0 ); 
    cntrl0_ddr2_ba : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
    cntrl0_ddr2_dm : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
    cntrl0_user_output_data : out STD_LOGIC_VECTOR ( 31 downto 0 ) 
  );
end mem_interface_top;

architecture Structure of mem_interface_top is
  component infrastructure_top
    port (
      sys_clk_ibuf : in STD_LOGIC := 'X'; 
      reset_in : in STD_LOGIC := 'X'; 
      sys_rst180_val : out STD_LOGIC; 
      sys_rst_val : out STD_LOGIC; 
      sys_rst90_val : out STD_LOGIC; 
      wait_200us : out STD_LOGIC; 
      clk90_int_val : out STD_LOGIC; 
      clk_int_val : out STD_LOGIC; 
      sys_rst270_val : out STD_LOGIC; 
      delay_sel_val1_val : out STD_LOGIC_VECTOR ( 4 downto 0 ) 
    );
  end component;
  component ddr2_top_16bit_00
    port (
      clk_int : in STD_LOGIC := 'X'; 
      sys_rst : in STD_LOGIC := 'X'; 
      sys_rst90 : in STD_LOGIC := 'X'; 
      wait_200us : in STD_LOGIC := 'X'; 
      reset_in : in STD_LOGIC := 'X'; 
      clk90_int : in STD_LOGIC := 'X'; 
      sys_rst180 : in STD_LOGIC := 'X'; 
      clk180 : in STD_LOGIC := 'X'; 
      rst_dqs_div_in : in STD_LOGIC := 'X'; 
      sys_rst270 : in STD_LOGIC := 'X'; 
      clk270 : in STD_LOGIC := 'X'; 
      dip1 : in STD_LOGIC := 'X'; 
      dip3 : in STD_LOGIC := 'X'; 
      burst_done : in STD_LOGIC := 'X'; 
      ddr_cke : out STD_LOGIC; 
      ar_done : out STD_LOGIC; 
      auto_ref_req : out STD_LOGIC; 
      init_val : out STD_LOGIC; 
      ddr_csb : out STD_LOGIC; 
      ddr_rasb : out STD_LOGIC; 
      ddr_web : out STD_LOGIC; 
      ddr2_clk0 : out STD_LOGIC; 
      ddr_casb : out STD_LOGIC; 
      ddr2_clk0b : out STD_LOGIC; 
      ddr_ODT0 : out STD_LOGIC; 
      rst_dqs_div_out : out STD_LOGIC; 
      user_cmd_ack : out STD_LOGIC; 
      user_data_valid : out STD_LOGIC; 
      user_input_address : in STD_LOGIC_VECTOR ( 22 downto 0 ); 
      user_data_mask : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      user_input_data : in STD_LOGIC_VECTOR ( 31 downto 0 ); 
      user_command_register : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      delay_sel_val : in STD_LOGIC_VECTOR ( 4 downto 0 ); 
      user_config_register1 : in STD_LOGIC_VECTOR ( 14 downto 0 ); 
      user_config_register2 : in STD_LOGIC_VECTOR ( 12 downto 0 ); 
      user_bank_address : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
      ddr_dq : inout STD_LOGIC_VECTOR ( 15 downto 0 ); 
      ddr_dqs_n : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
      ddr_dqs : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
      ddr_address : out STD_LOGIC_VECTOR ( 12 downto 0 ); 
      ddr_ba : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
      user_output_data : out STD_LOGIC_VECTOR ( 31 downto 0 ); 
      ddr_dm : out STD_LOGIC_VECTOR ( 1 downto 0 ) 
    );
  end component;
  signal NlwRenamedSig_OI_sys_rst270_1 : STD_LOGIC; 
  signal NlwRenamedSig_OI_sys_rst_1 : STD_LOGIC; 
  signal NlwRenamedSig_OI_clk180_1 : STD_LOGIC; 
  signal NlwRenamedSig_OI_sys_rst90_1 : STD_LOGIC; 
  signal NlwRenamedSig_OI_clk_int_1 : STD_LOGIC; 
  signal NlwRenamedSig_OI_sys_rst180_1 : STD_LOGIC; 
  signal NlwRenamedSig_OI_clk90_int_1 : STD_LOGIC; 
  signal NlwRenamedSig_OI_clk270_1 : STD_LOGIC; 
  signal wait_200us : STD_LOGIC; 
  signal NLW_ddr2_top0_dip1_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_dip3_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_input_address_22_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_input_address_21_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_input_address_20_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_input_address_19_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_input_address_18_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_input_address_17_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_input_address_16_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_input_address_15_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_input_address_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_input_address_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_input_address_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_input_address_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_input_address_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_input_address_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_input_address_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_config_register1_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_config_register1_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_config_register1_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_config_register1_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_config_register1_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_config_register1_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_config_register1_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_config_register2_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_config_register2_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_config_register2_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_config_register2_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_config_register2_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_config_register2_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_config_register2_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_config_register2_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_config_register2_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_bank_address_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_ddr2_top0_user_bank_address_0_UNCONNECTED : STD_LOGIC; 
  signal delay_sel : STD_LOGIC_VECTOR ( 4 downto 0 ); 
begin
  sys_rst270_1 <= NlwRenamedSig_OI_sys_rst270_1;
  sys_rst_1 <= NlwRenamedSig_OI_sys_rst_1;
  clk180_1 <= NlwRenamedSig_OI_clk180_1;
  sys_rst90_1 <= NlwRenamedSig_OI_sys_rst90_1;
  clk_int_1 <= NlwRenamedSig_OI_clk_int_1;
  sys_rst180_1 <= NlwRenamedSig_OI_sys_rst180_1;
  clk90_int_1 <= NlwRenamedSig_OI_clk90_int_1;
  clk270_1 <= NlwRenamedSig_OI_clk270_1;
  clk180_11 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_clk_int_1,
      O => NlwRenamedSig_OI_clk180_1
    );
  clk270_11 : LUT1
    generic map(
      INIT => X"1"
    )
    port map (
      I0 => NlwRenamedSig_OI_clk90_int_1,
      O => NlwRenamedSig_OI_clk270_1
    );
  infrastructure_top0 : infrastructure_top
    port map (
      sys_clk_ibuf => SYS_CLK,
      reset_in => reset_in,
      sys_rst180_val => NlwRenamedSig_OI_sys_rst180_1,
      sys_rst_val => NlwRenamedSig_OI_sys_rst_1,
      sys_rst90_val => NlwRenamedSig_OI_sys_rst90_1,
      wait_200us => wait_200us,
      clk90_int_val => NlwRenamedSig_OI_clk90_int_1,
      clk_int_val => NlwRenamedSig_OI_clk_int_1,
      sys_rst270_val => NlwRenamedSig_OI_sys_rst270_1,
      delay_sel_val1_val(4) => delay_sel(4),
      delay_sel_val1_val(3) => delay_sel(3),
      delay_sel_val1_val(2) => delay_sel(2),
      delay_sel_val1_val(1) => delay_sel(1),
      delay_sel_val1_val(0) => delay_sel(0)
    );
  ddr2_top0 : ddr2_top_16bit_00
    port map (
      clk_int => NlwRenamedSig_OI_clk_int_1,
      sys_rst => NlwRenamedSig_OI_sys_rst_1,
      sys_rst90 => NlwRenamedSig_OI_sys_rst90_1,
      wait_200us => wait_200us,
      reset_in => reset_in,
      clk90_int => NlwRenamedSig_OI_clk90_int_1,
      sys_rst180 => NlwRenamedSig_OI_sys_rst180_1,
      clk180 => NlwRenamedSig_OI_clk180_1,
      rst_dqs_div_in => cntrl0_rst_dqs_div_in,
      sys_rst270 => NlwRenamedSig_OI_sys_rst270_1,
      clk270 => NlwRenamedSig_OI_clk270_1,
      dip1 => NLW_ddr2_top0_dip1_UNCONNECTED,
      dip3 => NLW_ddr2_top0_dip3_UNCONNECTED,
      burst_done => cntrl0_burst_done,
      ddr_cke => cntrl0_ddr2_cke,
      ar_done => cntrl0_ar_done,
      auto_ref_req => cntrl0_auto_ref_req,
      init_val => cntrl0_init_val,
      ddr_csb => cntrl0_ddr2_csb,
      ddr_rasb => cntrl0_ddr2_rasb,
      ddr_web => cntrl0_ddr2_web,
      ddr2_clk0 => cntrl0_ddr2_clk0,
      ddr_casb => cntrl0_ddr2_casb,
      ddr2_clk0b => cntrl0_ddr2_clk0b,
      ddr_ODT0 => cntrl0_ddr2_ODT0,
      rst_dqs_div_out => cntrl0_rst_dqs_div_out,
      user_cmd_ack => cntrl0_user_cmd_ack,
      user_data_valid => cntrl0_user_data_valid,
      user_input_address(22) => NLW_ddr2_top0_user_input_address_22_UNCONNECTED,
      user_input_address(21) => NLW_ddr2_top0_user_input_address_21_UNCONNECTED,
      user_input_address(20) => NLW_ddr2_top0_user_input_address_20_UNCONNECTED,
      user_input_address(19) => NLW_ddr2_top0_user_input_address_19_UNCONNECTED,
      user_input_address(18) => NLW_ddr2_top0_user_input_address_18_UNCONNECTED,
      user_input_address(17) => NLW_ddr2_top0_user_input_address_17_UNCONNECTED,
      user_input_address(16) => NLW_ddr2_top0_user_input_address_16_UNCONNECTED,
      user_input_address(15) => NLW_ddr2_top0_user_input_address_15_UNCONNECTED,
      user_input_address(14) => NLW_ddr2_top0_user_input_address_14_UNCONNECTED,
      user_input_address(13) => NLW_ddr2_top0_user_input_address_13_UNCONNECTED,
      user_input_address(12) => NLW_ddr2_top0_user_input_address_12_UNCONNECTED,
      user_input_address(11) => NLW_ddr2_top0_user_input_address_11_UNCONNECTED,
      user_input_address(10) => NLW_ddr2_top0_user_input_address_10_UNCONNECTED,
      user_input_address(9) => cntrl0_user_input_address(9),
      user_input_address(8) => cntrl0_user_input_address(8),
      user_input_address(7) => cntrl0_user_input_address(7),
      user_input_address(6) => cntrl0_user_input_address(6),
      user_input_address(5) => cntrl0_user_input_address(5),
      user_input_address(4) => cntrl0_user_input_address(4),
      user_input_address(3) => cntrl0_user_input_address(3),
      user_input_address(2) => cntrl0_user_input_address(2),
      user_input_address(1) => NLW_ddr2_top0_user_input_address_1_UNCONNECTED,
      user_input_address(0) => NLW_ddr2_top0_user_input_address_0_UNCONNECTED,
      user_data_mask(3) => cntrl0_user_data_mask(3),
      user_data_mask(2) => cntrl0_user_data_mask(2),
      user_data_mask(1) => cntrl0_user_data_mask(1),
      user_data_mask(0) => cntrl0_user_data_mask(0),
      user_input_data(31) => cntrl0_user_input_data(31),
      user_input_data(30) => cntrl0_user_input_data(30),
      user_input_data(29) => cntrl0_user_input_data(29),
      user_input_data(28) => cntrl0_user_input_data(28),
      user_input_data(27) => cntrl0_user_input_data(27),
      user_input_data(26) => cntrl0_user_input_data(26),
      user_input_data(25) => cntrl0_user_input_data(25),
      user_input_data(24) => cntrl0_user_input_data(24),
      user_input_data(23) => cntrl0_user_input_data(23),
      user_input_data(22) => cntrl0_user_input_data(22),
      user_input_data(21) => cntrl0_user_input_data(21),
      user_input_data(20) => cntrl0_user_input_data(20),
      user_input_data(19) => cntrl0_user_input_data(19),
      user_input_data(18) => cntrl0_user_input_data(18),
      user_input_data(17) => cntrl0_user_input_data(17),
      user_input_data(16) => cntrl0_user_input_data(16),
      user_input_data(15) => cntrl0_user_input_data(15),
      user_input_data(14) => cntrl0_user_input_data(14),
      user_input_data(13) => cntrl0_user_input_data(13),
      user_input_data(12) => cntrl0_user_input_data(12),
      user_input_data(11) => cntrl0_user_input_data(11),
      user_input_data(10) => cntrl0_user_input_data(10),
      user_input_data(9) => cntrl0_user_input_data(9),
      user_input_data(8) => cntrl0_user_input_data(8),
      user_input_data(7) => cntrl0_user_input_data(7),
      user_input_data(6) => cntrl0_user_input_data(6),
      user_input_data(5) => cntrl0_user_input_data(5),
      user_input_data(4) => cntrl0_user_input_data(4),
      user_input_data(3) => cntrl0_user_input_data(3),
      user_input_data(2) => cntrl0_user_input_data(2),
      user_input_data(1) => cntrl0_user_input_data(1),
      user_input_data(0) => cntrl0_user_input_data(0),
      user_command_register(3) => cntrl0_user_command_register(3),
      user_command_register(2) => cntrl0_user_command_register(2),
      user_command_register(1) => cntrl0_user_command_register(1),
      user_command_register(0) => cntrl0_user_command_register(0),
      delay_sel_val(4) => delay_sel(4),
      delay_sel_val(3) => delay_sel(3),
      delay_sel_val(2) => delay_sel(2),
      delay_sel_val(1) => delay_sel(1),
      delay_sel_val(0) => delay_sel(0),
      user_config_register1(14) => NLW_ddr2_top0_user_config_register1_14_UNCONNECTED,
      user_config_register1(13) => NLW_ddr2_top0_user_config_register1_13_UNCONNECTED,
      user_config_register1(12) => cntrl0_user_config_register1(12),
      user_config_register1(11) => NLW_ddr2_top0_user_config_register1_11_UNCONNECTED,
      user_config_register1(10) => NLW_ddr2_top0_user_config_register1_10_UNCONNECTED,
      user_config_register1(9) => cntrl0_user_config_register1(9),
      user_config_register1(8) => cntrl0_user_config_register1(8),
      user_config_register1(7) => cntrl0_user_config_register1(7),
      user_config_register1(6) => NLW_ddr2_top0_user_config_register1_6_UNCONNECTED,
      user_config_register1(5) => cntrl0_user_config_register1(5),
      user_config_register1(4) => cntrl0_user_config_register1(4),
      user_config_register1(3) => cntrl0_user_config_register1(3),
      user_config_register1(2) => NLW_ddr2_top0_user_config_register1_2_UNCONNECTED,
      user_config_register1(1) => cntrl0_user_config_register1(1),
      user_config_register1(0) => NLW_ddr2_top0_user_config_register1_0_UNCONNECTED,
      user_config_register2(12) => NLW_ddr2_top0_user_config_register2_12_UNCONNECTED,
      user_config_register2(11) => NLW_ddr2_top0_user_config_register2_11_UNCONNECTED,
      user_config_register2(10) => NLW_ddr2_top0_user_config_register2_10_UNCONNECTED,
      user_config_register2(9) => cntrl0_user_config_register2(9),
      user_config_register2(8) => cntrl0_user_config_register2(8),
      user_config_register2(7) => cntrl0_user_config_register2(7),
      user_config_register2(6) => NLW_ddr2_top0_user_config_register2_6_UNCONNECTED,
      user_config_register2(5) => NLW_ddr2_top0_user_config_register2_5_UNCONNECTED,
      user_config_register2(4) => NLW_ddr2_top0_user_config_register2_4_UNCONNECTED,
      user_config_register2(3) => NLW_ddr2_top0_user_config_register2_3_UNCONNECTED,
      user_config_register2(2) => cntrl0_user_config_register2(2),
      user_config_register2(1) => NLW_ddr2_top0_user_config_register2_1_UNCONNECTED,
      user_config_register2(0) => NLW_ddr2_top0_user_config_register2_0_UNCONNECTED,
      user_bank_address(1) => NLW_ddr2_top0_user_bank_address_1_UNCONNECTED,
      user_bank_address(0) => NLW_ddr2_top0_user_bank_address_0_UNCONNECTED,
      ddr_dq(15) => cntrl0_ddr2_dq(15),
      ddr_dq(14) => cntrl0_ddr2_dq(14),
      ddr_dq(13) => cntrl0_ddr2_dq(13),
      ddr_dq(12) => cntrl0_ddr2_dq(12),
      ddr_dq(11) => cntrl0_ddr2_dq(11),
      ddr_dq(10) => cntrl0_ddr2_dq(10),
      ddr_dq(9) => cntrl0_ddr2_dq(9),
      ddr_dq(8) => cntrl0_ddr2_dq(8),
      ddr_dq(7) => cntrl0_ddr2_dq(7),
      ddr_dq(6) => cntrl0_ddr2_dq(6),
      ddr_dq(5) => cntrl0_ddr2_dq(5),
      ddr_dq(4) => cntrl0_ddr2_dq(4),
      ddr_dq(3) => cntrl0_ddr2_dq(3),
      ddr_dq(2) => cntrl0_ddr2_dq(2),
      ddr_dq(1) => cntrl0_ddr2_dq(1),
      ddr_dq(0) => cntrl0_ddr2_dq(0),
      ddr_dqs_n(1) => cntrl0_ddr2_dqs_n(1),
      ddr_dqs_n(0) => cntrl0_ddr2_dqs_n(0),
      ddr_dqs(1) => cntrl0_ddr2_dqs(1),
      ddr_dqs(0) => cntrl0_ddr2_dqs(0),
      ddr_address(12) => cntrl0_ddr2_address(12),
      ddr_address(11) => cntrl0_ddr2_address(11),
      ddr_address(10) => cntrl0_ddr2_address(10),
      ddr_address(9) => cntrl0_ddr2_address(9),
      ddr_address(8) => cntrl0_ddr2_address(8),
      ddr_address(7) => cntrl0_ddr2_address(7),
      ddr_address(6) => cntrl0_ddr2_address(6),
      ddr_address(5) => cntrl0_ddr2_address(5),
      ddr_address(4) => cntrl0_ddr2_address(4),
      ddr_address(3) => cntrl0_ddr2_address(3),
      ddr_address(2) => cntrl0_ddr2_address(2),
      ddr_address(1) => cntrl0_ddr2_address(1),
      ddr_address(0) => cntrl0_ddr2_address(0),
      ddr_ba(1) => cntrl0_ddr2_ba(1),
      ddr_ba(0) => cntrl0_ddr2_ba(0),
      user_output_data(31) => cntrl0_user_output_data(31),
      user_output_data(30) => cntrl0_user_output_data(30),
      user_output_data(29) => cntrl0_user_output_data(29),
      user_output_data(28) => cntrl0_user_output_data(28),
      user_output_data(27) => cntrl0_user_output_data(27),
      user_output_data(26) => cntrl0_user_output_data(26),
      user_output_data(25) => cntrl0_user_output_data(25),
      user_output_data(24) => cntrl0_user_output_data(24),
      user_output_data(23) => cntrl0_user_output_data(23),
      user_output_data(22) => cntrl0_user_output_data(22),
      user_output_data(21) => cntrl0_user_output_data(21),
      user_output_data(20) => cntrl0_user_output_data(20),
      user_output_data(19) => cntrl0_user_output_data(19),
      user_output_data(18) => cntrl0_user_output_data(18),
      user_output_data(17) => cntrl0_user_output_data(17),
      user_output_data(16) => cntrl0_user_output_data(16),
      user_output_data(15) => cntrl0_user_output_data(15),
      user_output_data(14) => cntrl0_user_output_data(14),
      user_output_data(13) => cntrl0_user_output_data(13),
      user_output_data(12) => cntrl0_user_output_data(12),
      user_output_data(11) => cntrl0_user_output_data(11),
      user_output_data(10) => cntrl0_user_output_data(10),
      user_output_data(9) => cntrl0_user_output_data(9),
      user_output_data(8) => cntrl0_user_output_data(8),
      user_output_data(7) => cntrl0_user_output_data(7),
      user_output_data(6) => cntrl0_user_output_data(6),
      user_output_data(5) => cntrl0_user_output_data(5),
      user_output_data(4) => cntrl0_user_output_data(4),
      user_output_data(3) => cntrl0_user_output_data(3),
      user_output_data(2) => cntrl0_user_output_data(2),
      user_output_data(1) => cntrl0_user_output_data(1),
      user_output_data(0) => cntrl0_user_output_data(0),
      ddr_dm(1) => cntrl0_ddr2_dm(1),
      ddr_dm(0) => cntrl0_ddr2_dm(0)
    );

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity HD_Gen_Module is
  port (
    mreset_i : in STD_LOGIC := 'X'; 
    brefclk2_n_i : in STD_LOGIC := 'X'; 
    clk_27_i : in STD_LOGIC := 'X'; 
    brefclk2_p_i : in STD_LOGIC := 'X'; 
    brefclk_n_i : in STD_LOGIC := 'X'; 
    brefclk_p_i : in STD_LOGIC := 'X'; 
    pc_ibf_i : in STD_LOGIC := 'X'; 
    pc_zstb_o : out STD_LOGIC; 
    ddr2_cke : out STD_LOGIC; 
    ddr2_rasb : out STD_LOGIC; 
    ddr2_csb : out STD_LOGIC; 
    vcxo_control_1_o : out STD_LOGIC; 
    ddr2_casb : out STD_LOGIC; 
    vcxo_control_2_o : out STD_LOGIC; 
    ddr2_web : out STD_LOGIC; 
    ddr2_ODT0 : out STD_LOGIC; 
    ddr2_clk0 : out STD_LOGIC; 
    ddr2_clk0b : out STD_LOGIC; 
    ddr2_dqs_n : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
    ddr2_dqs : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
    ddr2_dq : inout STD_LOGIC_VECTOR ( 15 downto 0 ); 
    led_o : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    pc_data_o : out STD_LOGIC_VECTOR ( 21 downto 0 ); 
    ddr2_ba : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
    ddr2_dm : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
    ddr2_address : out STD_LOGIC_VECTOR ( 12 downto 0 ) 
  );
end HD_Gen_Module;

architecture Structure of HD_Gen_Module is
  component mem_interface_top
    port (
      cntrl0_rst_dqs_div_in : in STD_LOGIC := 'X'; 
      cntrl0_burst_done : in STD_LOGIC := 'X'; 
      reset_in : in STD_LOGIC := 'X'; 
      dip1 : in STD_LOGIC := 'X'; 
      dip2 : in STD_LOGIC := 'X'; 
      dip3 : in STD_LOGIC := 'X'; 
      SYS_CLK : in STD_LOGIC := 'X'; 
      sys_rst270_1 : out STD_LOGIC; 
      cntrl0_ddr2_clk0b : out STD_LOGIC; 
      cntrl0_ddr2_cke : out STD_LOGIC; 
      sys_rst_1 : out STD_LOGIC; 
      clk180_1 : out STD_LOGIC; 
      sys_rst90_1 : out STD_LOGIC; 
      cntrl0_rst_dqs_div_out : out STD_LOGIC; 
      cntrl0_init_val : out STD_LOGIC; 
      cntrl0_ddr2_csb : out STD_LOGIC; 
      cntrl0_user_data_valid : out STD_LOGIC; 
      clk_int_1 : out STD_LOGIC; 
      cntrl0_ar_done : out STD_LOGIC; 
      cntrl0_auto_ref_req : out STD_LOGIC; 
      sys_rst180_1 : out STD_LOGIC; 
      cntrl0_ddr2_rasb : out STD_LOGIC; 
      cntrl0_ddr2_web : out STD_LOGIC; 
      cntrl0_user_cmd_ack : out STD_LOGIC; 
      clk90_int_1 : out STD_LOGIC; 
      clk270_1 : out STD_LOGIC; 
      cntrl0_ddr2_casb : out STD_LOGIC; 
      cntrl0_ddr2_ODT0 : out STD_LOGIC; 
      cntrl0_ddr2_clk0 : out STD_LOGIC; 
      cntrl0_user_data_mask : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      cntrl0_user_bank_address : in STD_LOGIC_VECTOR ( 1 downto 0 ); 
      cntrl0_user_command_register : in STD_LOGIC_VECTOR ( 3 downto 0 ); 
      cntrl0_user_input_address : in STD_LOGIC_VECTOR ( 22 downto 0 ); 
      cntrl0_user_config_register1 : in STD_LOGIC_VECTOR ( 14 downto 0 ); 
      cntrl0_user_config_register2 : in STD_LOGIC_VECTOR ( 12 downto 0 ); 
      cntrl0_user_input_data : in STD_LOGIC_VECTOR ( 31 downto 0 ); 
      cntrl0_ddr2_dqs_n : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
      cntrl0_ddr2_dqs : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
      cntrl0_ddr2_dq : inout STD_LOGIC_VECTOR ( 15 downto 0 ); 
      cntrl0_ddr2_address : out STD_LOGIC_VECTOR ( 12 downto 0 ); 
      cntrl0_ddr2_ba : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
      cntrl0_ddr2_dm : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
      cntrl0_user_output_data : out STD_LOGIC_VECTOR ( 31 downto 0 ) 
    );
  end component;
  component system_controller
    port (
      clk : in STD_LOGIC := 'X'; 
      reset : in STD_LOGIC := 'X'; 
      ar_done : in STD_LOGIC := 'X'; 
      auto_ref_req : in STD_LOGIC := 'X'; 
      init_val : in STD_LOGIC := 'X'; 
      clk180 : in STD_LOGIC := 'X'; 
      clk90 : in STD_LOGIC := 'X'; 
      user_cmd_ack : in STD_LOGIC := 'X'; 
      user_data_valid : in STD_LOGIC := 'X'; 
      TP1 : out STD_LOGIC; 
      TP2 : out STD_LOGIC; 
      TP3 : out STD_LOGIC; 
      burst_done : out STD_LOGIC; 
      user_output_data : in STD_LOGIC_VECTOR ( 31 downto 0 ); 
      user_input_address : out STD_LOGIC_VECTOR ( 22 downto 0 ); 
      TP_port : out STD_LOGIC_VECTOR ( 21 downto 0 ); 
      user_data_mask : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      user_input_data : out STD_LOGIC_VECTOR ( 31 downto 0 ); 
      user_command_register : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
      user_config_register1 : out STD_LOGIC_VECTOR ( 14 downto 0 ); 
      user_config_register2 : out STD_LOGIC_VECTOR ( 12 downto 0 ); 
      user_bank_address : out STD_LOGIC_VECTOR ( 1 downto 0 ) 
    );
  end component;
  signal mreset_i_IBUF_264 : STD_LOGIC; 
  signal rst_dqs_div : STD_LOGIC; 
  signal brefclk2 : STD_LOGIC; 
  signal new_clk : STD_LOGIC; 
  signal ar_done : STD_LOGIC; 
  signal auto_ref_req : STD_LOGIC; 
  signal init_val : STD_LOGIC; 
  signal clk1 : STD_LOGIC; 
  signal clk180 : STD_LOGIC; 
  signal clk90 : STD_LOGIC; 
  signal brefclk : STD_LOGIC; 
  signal user_cmd_ack : STD_LOGIC; 
  signal burst_done : STD_LOGIC; 
  signal user_data_valid : STD_LOGIC; 
  signal led_o_2_OBUF_265 : STD_LOGIC; 
  signal led_o_1_OBUF_266 : STD_LOGIC; 
  signal led_o_0_OBUF_267 : STD_LOGIC; 
  signal pc_data_o_21_OBUF_268 : STD_LOGIC; 
  signal pc_data_o_20_OBUF_269 : STD_LOGIC; 
  signal pc_data_o_19_OBUF_270 : STD_LOGIC; 
  signal pc_data_o_18_OBUF_271 : STD_LOGIC; 
  signal pc_data_o_17_OBUF_272 : STD_LOGIC; 
  signal pc_data_o_16_OBUF_273 : STD_LOGIC; 
  signal pc_data_o_15_OBUF_274 : STD_LOGIC; 
  signal pc_data_o_14_OBUF_275 : STD_LOGIC; 
  signal pc_data_o_13_OBUF_276 : STD_LOGIC; 
  signal pc_data_o_12_OBUF_277 : STD_LOGIC; 
  signal pc_data_o_11_OBUF_278 : STD_LOGIC; 
  signal pc_data_o_10_OBUF_279 : STD_LOGIC; 
  signal pc_data_o_9_OBUF_280 : STD_LOGIC; 
  signal pc_data_o_8_OBUF_281 : STD_LOGIC; 
  signal pc_data_o_7_OBUF_282 : STD_LOGIC; 
  signal pc_data_o_6_OBUF_283 : STD_LOGIC; 
  signal pc_data_o_5_OBUF_284 : STD_LOGIC; 
  signal pc_data_o_4_OBUF_285 : STD_LOGIC; 
  signal pc_data_o_3_OBUF_286 : STD_LOGIC; 
  signal pc_data_o_2_OBUF_287 : STD_LOGIC; 
  signal pc_data_o_1_OBUF_288 : STD_LOGIC; 
  signal pc_data_o_0_OBUF_289 : STD_LOGIC; 
  signal user_config_register1_12_Q : STD_LOGIC; 
  signal user_config_register1_9_Q : STD_LOGIC; 
  signal user_config_register1_8_Q : STD_LOGIC; 
  signal user_config_register1_7_Q : STD_LOGIC; 
  signal user_config_register1_5_Q : STD_LOGIC; 
  signal user_config_register1_4_Q : STD_LOGIC; 
  signal user_config_register1_3_Q : STD_LOGIC; 
  signal user_config_register1_1_Q : STD_LOGIC; 
  signal user_config_register2_9_Q : STD_LOGIC; 
  signal user_config_register2_8_Q : STD_LOGIC; 
  signal user_config_register2_7_Q : STD_LOGIC; 
  signal user_config_register2_2_Q : STD_LOGIC; 
  signal NLW_system_controller_inst_user_input_address_22_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_input_address_21_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_input_address_20_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_input_address_19_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_input_address_18_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_input_address_17_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_input_address_16_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_input_address_15_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_input_address_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_input_address_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_input_address_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_input_address_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_input_address_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_input_address_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_input_address_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_config_register1_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_config_register1_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_config_register1_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_config_register1_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_config_register1_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_config_register1_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_config_register1_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_config_register2_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_config_register2_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_config_register2_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_config_register2_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_config_register2_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_config_register2_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_config_register2_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_config_register2_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_config_register2_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_bank_address_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_system_controller_inst_user_bank_address_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_dip1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_dip2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_dip3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_bank_address_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_bank_address_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_input_address_22_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_input_address_21_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_input_address_20_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_input_address_19_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_input_address_18_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_input_address_17_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_input_address_16_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_input_address_15_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_input_address_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_input_address_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_input_address_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_input_address_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_input_address_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_input_address_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_input_address_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_config_register1_14_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_config_register1_13_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_config_register1_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_config_register1_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_config_register1_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_config_register1_2_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_config_register1_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_config_register2_12_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_config_register2_11_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_config_register2_10_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_config_register2_6_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_config_register2_5_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_config_register2_4_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_config_register2_3_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_config_register2_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_cntrl0_user_config_register2_0_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_sys_rst270_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_sys_rst_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_sys_rst90_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_sys_rst180_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_mem_interface_top_inst_clk270_1_UNCONNECTED : STD_LOGIC; 
  signal NLW_clk2_for_logic_O_UNCONNECTED : STD_LOGIC; 
  signal user_input_address : STD_LOGIC_VECTOR ( 9 downto 2 ); 
  signal user_input_data : STD_LOGIC_VECTOR ( 31 downto 0 ); 
  signal user_data_mask : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal user_command_register : STD_LOGIC_VECTOR ( 3 downto 0 ); 
  signal user_output_data : STD_LOGIC_VECTOR ( 31 downto 0 ); 
begin
  diff_buf_brefclk : IBUFGDS
    generic map(
      DIFF_TERM => FALSE
    )
    port map (
      I => brefclk_p_i,
      IB => brefclk_n_i,
      O => brefclk
    );
  diff_buf_brefclk2 : IBUFGDS
    generic map(
      DIFF_TERM => FALSE
    )
    port map (
      I => brefclk2_p_i,
      IB => brefclk2_n_i,
      O => brefclk2
    );
  clk1_for_logic : BUFG
    port map (
      I => brefclk,
      O => clk1
    );
  clk2_for_logic : BUFG
    port map (
      I => brefclk2,
      O => NLW_clk2_for_logic_O_UNCONNECTED
    );
  mem_interface_top_inst : mem_interface_top
    port map (
      cntrl0_rst_dqs_div_in => rst_dqs_div,
      cntrl0_burst_done => burst_done,
      reset_in => mreset_i_IBUF_264,
      dip1 => NLW_mem_interface_top_inst_dip1_UNCONNECTED,
      dip2 => NLW_mem_interface_top_inst_dip2_UNCONNECTED,
      dip3 => NLW_mem_interface_top_inst_dip3_UNCONNECTED,
      SYS_CLK => clk1,
      sys_rst270_1 => NLW_mem_interface_top_inst_sys_rst270_1_UNCONNECTED,
      cntrl0_ddr2_clk0b => ddr2_clk0b,
      cntrl0_ddr2_cke => ddr2_cke,
      sys_rst_1 => NLW_mem_interface_top_inst_sys_rst_1_UNCONNECTED,
      clk180_1 => clk180,
      sys_rst90_1 => NLW_mem_interface_top_inst_sys_rst90_1_UNCONNECTED,
      cntrl0_rst_dqs_div_out => rst_dqs_div,
      cntrl0_init_val => init_val,
      cntrl0_ddr2_csb => ddr2_csb,
      cntrl0_user_data_valid => user_data_valid,
      clk_int_1 => new_clk,
      cntrl0_ar_done => ar_done,
      cntrl0_auto_ref_req => auto_ref_req,
      sys_rst180_1 => NLW_mem_interface_top_inst_sys_rst180_1_UNCONNECTED,
      cntrl0_ddr2_rasb => ddr2_rasb,
      cntrl0_ddr2_web => ddr2_web,
      cntrl0_user_cmd_ack => user_cmd_ack,
      clk90_int_1 => clk90,
      clk270_1 => NLW_mem_interface_top_inst_clk270_1_UNCONNECTED,
      cntrl0_ddr2_casb => ddr2_casb,
      cntrl0_ddr2_ODT0 => ddr2_ODT0,
      cntrl0_ddr2_clk0 => ddr2_clk0,
      cntrl0_user_data_mask(3) => user_data_mask(3),
      cntrl0_user_data_mask(2) => user_data_mask(2),
      cntrl0_user_data_mask(1) => user_data_mask(1),
      cntrl0_user_data_mask(0) => user_data_mask(0),
      cntrl0_user_bank_address(1) => NLW_mem_interface_top_inst_cntrl0_user_bank_address_1_UNCONNECTED,
      cntrl0_user_bank_address(0) => NLW_mem_interface_top_inst_cntrl0_user_bank_address_0_UNCONNECTED,
      cntrl0_user_command_register(3) => user_command_register(3),
      cntrl0_user_command_register(2) => user_command_register(2),
      cntrl0_user_command_register(1) => user_command_register(1),
      cntrl0_user_command_register(0) => user_command_register(0),
      cntrl0_user_input_address(22) => NLW_mem_interface_top_inst_cntrl0_user_input_address_22_UNCONNECTED,
      cntrl0_user_input_address(21) => NLW_mem_interface_top_inst_cntrl0_user_input_address_21_UNCONNECTED,
      cntrl0_user_input_address(20) => NLW_mem_interface_top_inst_cntrl0_user_input_address_20_UNCONNECTED,
      cntrl0_user_input_address(19) => NLW_mem_interface_top_inst_cntrl0_user_input_address_19_UNCONNECTED,
      cntrl0_user_input_address(18) => NLW_mem_interface_top_inst_cntrl0_user_input_address_18_UNCONNECTED,
      cntrl0_user_input_address(17) => NLW_mem_interface_top_inst_cntrl0_user_input_address_17_UNCONNECTED,
      cntrl0_user_input_address(16) => NLW_mem_interface_top_inst_cntrl0_user_input_address_16_UNCONNECTED,
      cntrl0_user_input_address(15) => NLW_mem_interface_top_inst_cntrl0_user_input_address_15_UNCONNECTED,
      cntrl0_user_input_address(14) => NLW_mem_interface_top_inst_cntrl0_user_input_address_14_UNCONNECTED,
      cntrl0_user_input_address(13) => NLW_mem_interface_top_inst_cntrl0_user_input_address_13_UNCONNECTED,
      cntrl0_user_input_address(12) => NLW_mem_interface_top_inst_cntrl0_user_input_address_12_UNCONNECTED,
      cntrl0_user_input_address(11) => NLW_mem_interface_top_inst_cntrl0_user_input_address_11_UNCONNECTED,
      cntrl0_user_input_address(10) => NLW_mem_interface_top_inst_cntrl0_user_input_address_10_UNCONNECTED,
      cntrl0_user_input_address(9) => user_input_address(9),
      cntrl0_user_input_address(8) => user_input_address(8),
      cntrl0_user_input_address(7) => user_input_address(7),
      cntrl0_user_input_address(6) => user_input_address(6),
      cntrl0_user_input_address(5) => user_input_address(5),
      cntrl0_user_input_address(4) => user_input_address(4),
      cntrl0_user_input_address(3) => user_input_address(3),
      cntrl0_user_input_address(2) => user_input_address(2),
      cntrl0_user_input_address(1) => NLW_mem_interface_top_inst_cntrl0_user_input_address_1_UNCONNECTED,
      cntrl0_user_input_address(0) => NLW_mem_interface_top_inst_cntrl0_user_input_address_0_UNCONNECTED,
      cntrl0_user_config_register1(14) => NLW_mem_interface_top_inst_cntrl0_user_config_register1_14_UNCONNECTED,
      cntrl0_user_config_register1(13) => NLW_mem_interface_top_inst_cntrl0_user_config_register1_13_UNCONNECTED,
      cntrl0_user_config_register1(12) => user_config_register1_12_Q,
      cntrl0_user_config_register1(11) => NLW_mem_interface_top_inst_cntrl0_user_config_register1_11_UNCONNECTED,
      cntrl0_user_config_register1(10) => NLW_mem_interface_top_inst_cntrl0_user_config_register1_10_UNCONNECTED,
      cntrl0_user_config_register1(9) => user_config_register1_9_Q,
      cntrl0_user_config_register1(8) => user_config_register1_8_Q,
      cntrl0_user_config_register1(7) => user_config_register1_7_Q,
      cntrl0_user_config_register1(6) => NLW_mem_interface_top_inst_cntrl0_user_config_register1_6_UNCONNECTED,
      cntrl0_user_config_register1(5) => user_config_register1_5_Q,
      cntrl0_user_config_register1(4) => user_config_register1_4_Q,
      cntrl0_user_config_register1(3) => user_config_register1_3_Q,
      cntrl0_user_config_register1(2) => NLW_mem_interface_top_inst_cntrl0_user_config_register1_2_UNCONNECTED,
      cntrl0_user_config_register1(1) => user_config_register1_1_Q,
      cntrl0_user_config_register1(0) => NLW_mem_interface_top_inst_cntrl0_user_config_register1_0_UNCONNECTED,
      cntrl0_user_config_register2(12) => NLW_mem_interface_top_inst_cntrl0_user_config_register2_12_UNCONNECTED,
      cntrl0_user_config_register2(11) => NLW_mem_interface_top_inst_cntrl0_user_config_register2_11_UNCONNECTED,
      cntrl0_user_config_register2(10) => NLW_mem_interface_top_inst_cntrl0_user_config_register2_10_UNCONNECTED,
      cntrl0_user_config_register2(9) => user_config_register2_9_Q,
      cntrl0_user_config_register2(8) => user_config_register2_8_Q,
      cntrl0_user_config_register2(7) => user_config_register2_7_Q,
      cntrl0_user_config_register2(6) => NLW_mem_interface_top_inst_cntrl0_user_config_register2_6_UNCONNECTED,
      cntrl0_user_config_register2(5) => NLW_mem_interface_top_inst_cntrl0_user_config_register2_5_UNCONNECTED,
      cntrl0_user_config_register2(4) => NLW_mem_interface_top_inst_cntrl0_user_config_register2_4_UNCONNECTED,
      cntrl0_user_config_register2(3) => NLW_mem_interface_top_inst_cntrl0_user_config_register2_3_UNCONNECTED,
      cntrl0_user_config_register2(2) => user_config_register2_2_Q,
      cntrl0_user_config_register2(1) => NLW_mem_interface_top_inst_cntrl0_user_config_register2_1_UNCONNECTED,
      cntrl0_user_config_register2(0) => NLW_mem_interface_top_inst_cntrl0_user_config_register2_0_UNCONNECTED,
      cntrl0_user_input_data(31) => user_input_data(31),
      cntrl0_user_input_data(30) => user_input_data(30),
      cntrl0_user_input_data(29) => user_input_data(29),
      cntrl0_user_input_data(28) => user_input_data(28),
      cntrl0_user_input_data(27) => user_input_data(27),
      cntrl0_user_input_data(26) => user_input_data(26),
      cntrl0_user_input_data(25) => user_input_data(25),
      cntrl0_user_input_data(24) => user_input_data(24),
      cntrl0_user_input_data(23) => user_input_data(23),
      cntrl0_user_input_data(22) => user_input_data(22),
      cntrl0_user_input_data(21) => user_input_data(21),
      cntrl0_user_input_data(20) => user_input_data(20),
      cntrl0_user_input_data(19) => user_input_data(19),
      cntrl0_user_input_data(18) => user_input_data(18),
      cntrl0_user_input_data(17) => user_input_data(17),
      cntrl0_user_input_data(16) => user_input_data(16),
      cntrl0_user_input_data(15) => user_input_data(15),
      cntrl0_user_input_data(14) => user_input_data(14),
      cntrl0_user_input_data(13) => user_input_data(13),
      cntrl0_user_input_data(12) => user_input_data(12),
      cntrl0_user_input_data(11) => user_input_data(11),
      cntrl0_user_input_data(10) => user_input_data(10),
      cntrl0_user_input_data(9) => user_input_data(9),
      cntrl0_user_input_data(8) => user_input_data(8),
      cntrl0_user_input_data(7) => user_input_data(7),
      cntrl0_user_input_data(6) => user_input_data(6),
      cntrl0_user_input_data(5) => user_input_data(5),
      cntrl0_user_input_data(4) => user_input_data(4),
      cntrl0_user_input_data(3) => user_input_data(3),
      cntrl0_user_input_data(2) => user_input_data(2),
      cntrl0_user_input_data(1) => user_input_data(1),
      cntrl0_user_input_data(0) => user_input_data(0),
      cntrl0_ddr2_dqs_n(1) => ddr2_dqs_n(1),
      cntrl0_ddr2_dqs_n(0) => ddr2_dqs_n(0),
      cntrl0_ddr2_dqs(1) => ddr2_dqs(1),
      cntrl0_ddr2_dqs(0) => ddr2_dqs(0),
      cntrl0_ddr2_dq(15) => ddr2_dq(15),
      cntrl0_ddr2_dq(14) => ddr2_dq(14),
      cntrl0_ddr2_dq(13) => ddr2_dq(13),
      cntrl0_ddr2_dq(12) => ddr2_dq(12),
      cntrl0_ddr2_dq(11) => ddr2_dq(11),
      cntrl0_ddr2_dq(10) => ddr2_dq(10),
      cntrl0_ddr2_dq(9) => ddr2_dq(9),
      cntrl0_ddr2_dq(8) => ddr2_dq(8),
      cntrl0_ddr2_dq(7) => ddr2_dq(7),
      cntrl0_ddr2_dq(6) => ddr2_dq(6),
      cntrl0_ddr2_dq(5) => ddr2_dq(5),
      cntrl0_ddr2_dq(4) => ddr2_dq(4),
      cntrl0_ddr2_dq(3) => ddr2_dq(3),
      cntrl0_ddr2_dq(2) => ddr2_dq(2),
      cntrl0_ddr2_dq(1) => ddr2_dq(1),
      cntrl0_ddr2_dq(0) => ddr2_dq(0),
      cntrl0_ddr2_address(12) => ddr2_address(12),
      cntrl0_ddr2_address(11) => ddr2_address(11),
      cntrl0_ddr2_address(10) => ddr2_address(10),
      cntrl0_ddr2_address(9) => ddr2_address(9),
      cntrl0_ddr2_address(8) => ddr2_address(8),
      cntrl0_ddr2_address(7) => ddr2_address(7),
      cntrl0_ddr2_address(6) => ddr2_address(6),
      cntrl0_ddr2_address(5) => ddr2_address(5),
      cntrl0_ddr2_address(4) => ddr2_address(4),
      cntrl0_ddr2_address(3) => ddr2_address(3),
      cntrl0_ddr2_address(2) => ddr2_address(2),
      cntrl0_ddr2_address(1) => ddr2_address(1),
      cntrl0_ddr2_address(0) => ddr2_address(0),
      cntrl0_ddr2_ba(1) => ddr2_ba(1),
      cntrl0_ddr2_ba(0) => ddr2_ba(0),
      cntrl0_ddr2_dm(1) => ddr2_dm(1),
      cntrl0_ddr2_dm(0) => ddr2_dm(0),
      cntrl0_user_output_data(31) => user_output_data(31),
      cntrl0_user_output_data(30) => user_output_data(30),
      cntrl0_user_output_data(29) => user_output_data(29),
      cntrl0_user_output_data(28) => user_output_data(28),
      cntrl0_user_output_data(27) => user_output_data(27),
      cntrl0_user_output_data(26) => user_output_data(26),
      cntrl0_user_output_data(25) => user_output_data(25),
      cntrl0_user_output_data(24) => user_output_data(24),
      cntrl0_user_output_data(23) => user_output_data(23),
      cntrl0_user_output_data(22) => user_output_data(22),
      cntrl0_user_output_data(21) => user_output_data(21),
      cntrl0_user_output_data(20) => user_output_data(20),
      cntrl0_user_output_data(19) => user_output_data(19),
      cntrl0_user_output_data(18) => user_output_data(18),
      cntrl0_user_output_data(17) => user_output_data(17),
      cntrl0_user_output_data(16) => user_output_data(16),
      cntrl0_user_output_data(15) => user_output_data(15),
      cntrl0_user_output_data(14) => user_output_data(14),
      cntrl0_user_output_data(13) => user_output_data(13),
      cntrl0_user_output_data(12) => user_output_data(12),
      cntrl0_user_output_data(11) => user_output_data(11),
      cntrl0_user_output_data(10) => user_output_data(10),
      cntrl0_user_output_data(9) => user_output_data(9),
      cntrl0_user_output_data(8) => user_output_data(8),
      cntrl0_user_output_data(7) => user_output_data(7),
      cntrl0_user_output_data(6) => user_output_data(6),
      cntrl0_user_output_data(5) => user_output_data(5),
      cntrl0_user_output_data(4) => user_output_data(4),
      cntrl0_user_output_data(3) => user_output_data(3),
      cntrl0_user_output_data(2) => user_output_data(2),
      cntrl0_user_output_data(1) => user_output_data(1),
      cntrl0_user_output_data(0) => user_output_data(0)
    );
  system_controller_inst : system_controller
    port map (
      clk => new_clk,
      reset => mreset_i_IBUF_264,
      ar_done => ar_done,
      auto_ref_req => auto_ref_req,
      init_val => init_val,
      clk180 => clk180,
      clk90 => clk90,
      user_cmd_ack => user_cmd_ack,
      user_data_valid => user_data_valid,
      TP1 => led_o_0_OBUF_267,
      TP2 => led_o_1_OBUF_266,
      TP3 => led_o_2_OBUF_265,
      burst_done => burst_done,
      user_output_data(31) => user_output_data(31),
      user_output_data(30) => user_output_data(30),
      user_output_data(29) => user_output_data(29),
      user_output_data(28) => user_output_data(28),
      user_output_data(27) => user_output_data(27),
      user_output_data(26) => user_output_data(26),
      user_output_data(25) => user_output_data(25),
      user_output_data(24) => user_output_data(24),
      user_output_data(23) => user_output_data(23),
      user_output_data(22) => user_output_data(22),
      user_output_data(21) => user_output_data(21),
      user_output_data(20) => user_output_data(20),
      user_output_data(19) => user_output_data(19),
      user_output_data(18) => user_output_data(18),
      user_output_data(17) => user_output_data(17),
      user_output_data(16) => user_output_data(16),
      user_output_data(15) => user_output_data(15),
      user_output_data(14) => user_output_data(14),
      user_output_data(13) => user_output_data(13),
      user_output_data(12) => user_output_data(12),
      user_output_data(11) => user_output_data(11),
      user_output_data(10) => user_output_data(10),
      user_output_data(9) => user_output_data(9),
      user_output_data(8) => user_output_data(8),
      user_output_data(7) => user_output_data(7),
      user_output_data(6) => user_output_data(6),
      user_output_data(5) => user_output_data(5),
      user_output_data(4) => user_output_data(4),
      user_output_data(3) => user_output_data(3),
      user_output_data(2) => user_output_data(2),
      user_output_data(1) => user_output_data(1),
      user_output_data(0) => user_output_data(0),
      user_input_address(22) => NLW_system_controller_inst_user_input_address_22_UNCONNECTED,
      user_input_address(21) => NLW_system_controller_inst_user_input_address_21_UNCONNECTED,
      user_input_address(20) => NLW_system_controller_inst_user_input_address_20_UNCONNECTED,
      user_input_address(19) => NLW_system_controller_inst_user_input_address_19_UNCONNECTED,
      user_input_address(18) => NLW_system_controller_inst_user_input_address_18_UNCONNECTED,
      user_input_address(17) => NLW_system_controller_inst_user_input_address_17_UNCONNECTED,
      user_input_address(16) => NLW_system_controller_inst_user_input_address_16_UNCONNECTED,
      user_input_address(15) => NLW_system_controller_inst_user_input_address_15_UNCONNECTED,
      user_input_address(14) => NLW_system_controller_inst_user_input_address_14_UNCONNECTED,
      user_input_address(13) => NLW_system_controller_inst_user_input_address_13_UNCONNECTED,
      user_input_address(12) => NLW_system_controller_inst_user_input_address_12_UNCONNECTED,
      user_input_address(11) => NLW_system_controller_inst_user_input_address_11_UNCONNECTED,
      user_input_address(10) => NLW_system_controller_inst_user_input_address_10_UNCONNECTED,
      user_input_address(9) => user_input_address(9),
      user_input_address(8) => user_input_address(8),
      user_input_address(7) => user_input_address(7),
      user_input_address(6) => user_input_address(6),
      user_input_address(5) => user_input_address(5),
      user_input_address(4) => user_input_address(4),
      user_input_address(3) => user_input_address(3),
      user_input_address(2) => user_input_address(2),
      user_input_address(1) => NLW_system_controller_inst_user_input_address_1_UNCONNECTED,
      user_input_address(0) => NLW_system_controller_inst_user_input_address_0_UNCONNECTED,
      TP_port(21) => pc_data_o_21_OBUF_268,
      TP_port(20) => pc_data_o_20_OBUF_269,
      TP_port(19) => pc_data_o_19_OBUF_270,
      TP_port(18) => pc_data_o_18_OBUF_271,
      TP_port(17) => pc_data_o_17_OBUF_272,
      TP_port(16) => pc_data_o_16_OBUF_273,
      TP_port(15) => pc_data_o_15_OBUF_274,
      TP_port(14) => pc_data_o_14_OBUF_275,
      TP_port(13) => pc_data_o_13_OBUF_276,
      TP_port(12) => pc_data_o_12_OBUF_277,
      TP_port(11) => pc_data_o_11_OBUF_278,
      TP_port(10) => pc_data_o_10_OBUF_279,
      TP_port(9) => pc_data_o_9_OBUF_280,
      TP_port(8) => pc_data_o_8_OBUF_281,
      TP_port(7) => pc_data_o_7_OBUF_282,
      TP_port(6) => pc_data_o_6_OBUF_283,
      TP_port(5) => pc_data_o_5_OBUF_284,
      TP_port(4) => pc_data_o_4_OBUF_285,
      TP_port(3) => pc_data_o_3_OBUF_286,
      TP_port(2) => pc_data_o_2_OBUF_287,
      TP_port(1) => pc_data_o_1_OBUF_288,
      TP_port(0) => pc_data_o_0_OBUF_289,
      user_data_mask(3) => user_data_mask(3),
      user_data_mask(2) => user_data_mask(2),
      user_data_mask(1) => user_data_mask(1),
      user_data_mask(0) => user_data_mask(0),
      user_input_data(31) => user_input_data(31),
      user_input_data(30) => user_input_data(30),
      user_input_data(29) => user_input_data(29),
      user_input_data(28) => user_input_data(28),
      user_input_data(27) => user_input_data(27),
      user_input_data(26) => user_input_data(26),
      user_input_data(25) => user_input_data(25),
      user_input_data(24) => user_input_data(24),
      user_input_data(23) => user_input_data(23),
      user_input_data(22) => user_input_data(22),
      user_input_data(21) => user_input_data(21),
      user_input_data(20) => user_input_data(20),
      user_input_data(19) => user_input_data(19),
      user_input_data(18) => user_input_data(18),
      user_input_data(17) => user_input_data(17),
      user_input_data(16) => user_input_data(16),
      user_input_data(15) => user_input_data(15),
      user_input_data(14) => user_input_data(14),
      user_input_data(13) => user_input_data(13),
      user_input_data(12) => user_input_data(12),
      user_input_data(11) => user_input_data(11),
      user_input_data(10) => user_input_data(10),
      user_input_data(9) => user_input_data(9),
      user_input_data(8) => user_input_data(8),
      user_input_data(7) => user_input_data(7),
      user_input_data(6) => user_input_data(6),
      user_input_data(5) => user_input_data(5),
      user_input_data(4) => user_input_data(4),
      user_input_data(3) => user_input_data(3),
      user_input_data(2) => user_input_data(2),
      user_input_data(1) => user_input_data(1),
      user_input_data(0) => user_input_data(0),
      user_command_register(3) => user_command_register(3),
      user_command_register(2) => user_command_register(2),
      user_command_register(1) => user_command_register(1),
      user_command_register(0) => user_command_register(0),
      user_config_register1(14) => NLW_system_controller_inst_user_config_register1_14_UNCONNECTED,
      user_config_register1(13) => NLW_system_controller_inst_user_config_register1_13_UNCONNECTED,
      user_config_register1(12) => user_config_register1_12_Q,
      user_config_register1(11) => NLW_system_controller_inst_user_config_register1_11_UNCONNECTED,
      user_config_register1(10) => NLW_system_controller_inst_user_config_register1_10_UNCONNECTED,
      user_config_register1(9) => user_config_register1_9_Q,
      user_config_register1(8) => user_config_register1_8_Q,
      user_config_register1(7) => user_config_register1_7_Q,
      user_config_register1(6) => NLW_system_controller_inst_user_config_register1_6_UNCONNECTED,
      user_config_register1(5) => user_config_register1_5_Q,
      user_config_register1(4) => user_config_register1_4_Q,
      user_config_register1(3) => user_config_register1_3_Q,
      user_config_register1(2) => NLW_system_controller_inst_user_config_register1_2_UNCONNECTED,
      user_config_register1(1) => user_config_register1_1_Q,
      user_config_register1(0) => NLW_system_controller_inst_user_config_register1_0_UNCONNECTED,
      user_config_register2(12) => NLW_system_controller_inst_user_config_register2_12_UNCONNECTED,
      user_config_register2(11) => NLW_system_controller_inst_user_config_register2_11_UNCONNECTED,
      user_config_register2(10) => NLW_system_controller_inst_user_config_register2_10_UNCONNECTED,
      user_config_register2(9) => user_config_register2_9_Q,
      user_config_register2(8) => user_config_register2_8_Q,
      user_config_register2(7) => user_config_register2_7_Q,
      user_config_register2(6) => NLW_system_controller_inst_user_config_register2_6_UNCONNECTED,
      user_config_register2(5) => NLW_system_controller_inst_user_config_register2_5_UNCONNECTED,
      user_config_register2(4) => NLW_system_controller_inst_user_config_register2_4_UNCONNECTED,
      user_config_register2(3) => NLW_system_controller_inst_user_config_register2_3_UNCONNECTED,
      user_config_register2(2) => user_config_register2_2_Q,
      user_config_register2(1) => NLW_system_controller_inst_user_config_register2_1_UNCONNECTED,
      user_config_register2(0) => NLW_system_controller_inst_user_config_register2_0_UNCONNECTED,
      user_bank_address(1) => NLW_system_controller_inst_user_bank_address_1_UNCONNECTED,
      user_bank_address(0) => NLW_system_controller_inst_user_bank_address_0_UNCONNECTED
    );
  mreset_i_IBUF : IBUF
    port map (
      I => mreset_i,
      O => mreset_i_IBUF_264
    );
  led_o_2_OBUF : OBUF
    port map (
      I => led_o_2_OBUF_265,
      O => led_o(2)
    );
  led_o_1_OBUF : OBUF
    port map (
      I => led_o_1_OBUF_266,
      O => led_o(1)
    );
  led_o_0_OBUF : OBUF
    port map (
      I => led_o_0_OBUF_267,
      O => led_o(0)
    );
  pc_data_o_21_OBUF : OBUF
    port map (
      I => pc_data_o_21_OBUF_268,
      O => pc_data_o(21)
    );
  pc_data_o_20_OBUF : OBUF
    port map (
      I => pc_data_o_20_OBUF_269,
      O => pc_data_o(20)
    );
  pc_data_o_19_OBUF : OBUF
    port map (
      I => pc_data_o_19_OBUF_270,
      O => pc_data_o(19)
    );
  pc_data_o_18_OBUF : OBUF
    port map (
      I => pc_data_o_18_OBUF_271,
      O => pc_data_o(18)
    );
  pc_data_o_17_OBUF : OBUF
    port map (
      I => pc_data_o_17_OBUF_272,
      O => pc_data_o(17)
    );
  pc_data_o_16_OBUF : OBUF
    port map (
      I => pc_data_o_16_OBUF_273,
      O => pc_data_o(16)
    );
  pc_data_o_15_OBUF : OBUF
    port map (
      I => pc_data_o_15_OBUF_274,
      O => pc_data_o(15)
    );
  pc_data_o_14_OBUF : OBUF
    port map (
      I => pc_data_o_14_OBUF_275,
      O => pc_data_o(14)
    );
  pc_data_o_13_OBUF : OBUF
    port map (
      I => pc_data_o_13_OBUF_276,
      O => pc_data_o(13)
    );
  pc_data_o_12_OBUF : OBUF
    port map (
      I => pc_data_o_12_OBUF_277,
      O => pc_data_o(12)
    );
  pc_data_o_11_OBUF : OBUF
    port map (
      I => pc_data_o_11_OBUF_278,
      O => pc_data_o(11)
    );
  pc_data_o_10_OBUF : OBUF
    port map (
      I => pc_data_o_10_OBUF_279,
      O => pc_data_o(10)
    );
  pc_data_o_9_OBUF : OBUF
    port map (
      I => pc_data_o_9_OBUF_280,
      O => pc_data_o(9)
    );
  pc_data_o_8_OBUF : OBUF
    port map (
      I => pc_data_o_8_OBUF_281,
      O => pc_data_o(8)
    );
  pc_data_o_7_OBUF : OBUF
    port map (
      I => pc_data_o_7_OBUF_282,
      O => pc_data_o(7)
    );
  pc_data_o_6_OBUF : OBUF
    port map (
      I => pc_data_o_6_OBUF_283,
      O => pc_data_o(6)
    );
  pc_data_o_5_OBUF : OBUF
    port map (
      I => pc_data_o_5_OBUF_284,
      O => pc_data_o(5)
    );
  pc_data_o_4_OBUF : OBUF
    port map (
      I => pc_data_o_4_OBUF_285,
      O => pc_data_o(4)
    );
  pc_data_o_3_OBUF : OBUF
    port map (
      I => pc_data_o_3_OBUF_286,
      O => pc_data_o(3)
    );
  pc_data_o_2_OBUF : OBUF
    port map (
      I => pc_data_o_2_OBUF_287,
      O => pc_data_o(2)
    );
  pc_data_o_1_OBUF : OBUF
    port map (
      I => pc_data_o_1_OBUF_288,
      O => pc_data_o(1)
    );
  pc_data_o_0_OBUF : OBUF
    port map (
      I => pc_data_o_0_OBUF_289,
      O => pc_data_o(0)
    );

end Structure;

