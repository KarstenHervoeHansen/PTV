--------------------------------------------------------------------------------
-- Copyright (c) 1995-2005 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____
--  /   /\/   /
-- /___/  \  /    Vendor: Xilinx
-- \   \   \/     Version: I.27
--  \   \         Application: netgen
--  /   /         Filename: HD_Gen_Module_translate.vhd
-- /___/   /\     Timestamp: Tue Feb 27 09:27:22 2007
-- \   \  /  \ 
--  \___\/\___\
--             
-- Command	: -intstyle ise -rpw 100 -tpw 0 -ar Structure -tm HD_Gen_Module -w -dir netgen/translate -ofmt vhdl -sim HD_Gen_Module.ngd HD_Gen_Module_translate.vhd 
-- Device	: 2vp20ff896-6
-- Input file	: HD_Gen_Module.ngd
-- Output file	: C:\MyProject\DDR2interface_test\DDR2interface5\netgen\translate\HD_Gen_Module_translate.vhd
-- # of Entities	: 70
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
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal VCC : STD_LOGIC; 
  signal GND : STD_LOGIC; 
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
  XST_GND : X_ZERO
    port map (
      O => NlwRenamedSig_OI_user_bank_address(0)
    );
  XST_VCC : X_ONE
    port map (
      O => N1
    );
  loop_count_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0056(0),
      RST => reset_INV,
      CLK => clk90,
      O => loop_count(0),
      CE => VCC,
      SET => GND
    );
  loop_count_1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0056(1),
      RST => reset_INV,
      CLK => clk90,
      O => loop_count(1),
      CE => VCC,
      SET => GND
    );
  loop_count_2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0056(2),
      RST => reset_INV,
      CLK => clk90,
      O => loop_count(2),
      CE => VCC,
      SET => GND
    );
  loop_count_3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0056(3),
      RST => reset_INV,
      CLK => clk90,
      O => loop_count(3),
      CE => VCC,
      SET => GND
    );
  loop_count_4 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0056(4),
      RST => reset_INV,
      CLK => clk90,
      O => loop_count(4),
      CE => VCC,
      SET => GND
    );
  loop_count_5 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0056(5),
      RST => reset_INV,
      CLK => clk90,
      O => loop_count(5),
      CE => VCC,
      SET => GND
    );
  loop_count_6 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0056(6),
      RST => reset_INV,
      CLK => clk90,
      O => loop_count(6),
      CE => VCC,
      SET => GND
    );
  loop_count_7 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0056(7),
      RST => reset_INV,
      CLK => clk90,
      O => loop_count(7),
      CE => VCC,
      SET => GND
    );
  col_addr_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(0),
      RST => reset_INV,
      CLK => clk90,
      O => col_addr(0),
      CE => VCC,
      SET => GND
    );
  col_addr_1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(1),
      RST => reset_INV,
      CLK => clk90,
      O => col_addr(1),
      CE => VCC,
      SET => GND
    );
  col_addr_2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(2),
      RST => reset_INV,
      CLK => clk90,
      O => col_addr(2),
      CE => VCC,
      SET => GND
    );
  col_addr_3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(3),
      RST => reset_INV,
      CLK => clk90,
      O => col_addr(3),
      CE => VCC,
      SET => GND
    );
  col_addr_4 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(4),
      RST => reset_INV,
      CLK => clk90,
      O => col_addr(4),
      CE => VCC,
      SET => GND
    );
  col_addr_5 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(5),
      RST => reset_INV,
      CLK => clk90,
      O => col_addr(5),
      CE => VCC,
      SET => GND
    );
  col_addr_6 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(6),
      RST => reset_INV,
      CLK => clk90,
      O => col_addr(6),
      CE => VCC,
      SET => GND
    );
  col_addr_7 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(7),
      RST => reset_INV,
      CLK => clk90,
      O => col_addr(7),
      CE => VCC,
      SET => GND
    );
  intern_reg0_10 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(10),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(10),
      SET => GND
    );
  intern_reg0_11 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(11),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(11),
      SET => GND
    );
  intern_reg0_12 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(12),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(12),
      SET => GND
    );
  intern_reg0_13 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(13),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(13),
      SET => GND
    );
  intern_reg0_14 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(14),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(14),
      SET => GND
    );
  intern_reg0_15 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(15),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(15),
      SET => GND
    );
  intern_reg0_16 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(16),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(16),
      SET => GND
    );
  intern_reg0_17 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(17),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(17),
      SET => GND
    );
  intern_reg0_18 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(18),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(18),
      SET => GND
    );
  intern_reg0_19 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(19),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(19),
      SET => GND
    );
  intern_reg0_20 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(20),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(20),
      SET => GND
    );
  intern_reg0_21 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(21),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(21),
      SET => GND
    );
  intern_reg0_22 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(22),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(22),
      SET => GND
    );
  intern_reg0_23 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(23),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(23),
      SET => GND
    );
  intern_reg0_24 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(24),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(24),
      SET => GND
    );
  intern_reg0_25 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(25),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(25),
      SET => GND
    );
  intern_reg0_26 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(26),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(26),
      SET => GND
    );
  intern_reg0_27 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(27),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(27),
      SET => GND
    );
  intern_reg0_28 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(28),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(28),
      SET => GND
    );
  intern_reg0_29 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(29),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(29),
      SET => GND
    );
  intern_reg0_30 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(30),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(30),
      SET => GND
    );
  intern_reg0_31 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => user_output_data(31),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg0(31),
      SET => GND
    );
  current_input_address_2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_addr(2),
      RST => reset_INV,
      CLK => clk90,
      O => current_input_address(2),
      CE => VCC,
      SET => GND
    );
  current_input_address_3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_addr(3),
      RST => reset_INV,
      CLK => clk90,
      O => current_input_address(3),
      CE => VCC,
      SET => GND
    );
  current_input_address_4 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_addr(4),
      RST => reset_INV,
      CLK => clk90,
      O => current_input_address(4),
      CE => VCC,
      SET => GND
    );
  current_input_address_5 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_addr(5),
      RST => reset_INV,
      CLK => clk90,
      O => current_input_address(5),
      CE => VCC,
      SET => GND
    );
  current_input_address_6 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_addr(6),
      RST => reset_INV,
      CLK => clk90,
      O => current_input_address(6),
      CE => VCC,
      SET => GND
    );
  current_input_address_7 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_addr(7),
      RST => reset_INV,
      CLK => clk90,
      O => current_input_address(7),
      CE => VCC,
      SET => GND
    );
  current_input_address_8 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_addr(8),
      RST => reset_INV,
      CLK => clk90,
      O => current_input_address(8),
      CE => VCC,
      SET => GND
    );
  current_input_address_9 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_addr(9),
      RST => reset_INV,
      CLK => clk90,
      O => current_input_address(9),
      CE => VCC,
      SET => GND
    );
  current_burst_done : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_burst_done,
      RST => reset_INV,
      CLK => clk90,
      O => current_burst_done_10,
      CE => VCC,
      SET => GND
    );
  current_reg3_1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_reg3(1),
      RST => reset_INV,
      CLK => clk90,
      O => current_reg3(1),
      CE => VCC,
      SET => GND
    );
  current_reg3_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_reg3(2),
      RST => reset_INV,
      CLK => clk90,
      O => current_reg3(0),
      CE => VCC,
      SET => GND
    );
  current_reg1_11 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => N1,
      RST => reset_INV,
      CLK => clk90,
      O => current_reg1_11_Q,
      CE => VCC,
      SET => GND
    );
  current_reg1_8 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => N1,
      RST => reset_INV,
      CLK => clk90,
      O => current_reg1_8_Q,
      CE => VCC,
      SET => GND
    );
  current_reg1_7 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => N1,
      RST => reset_INV,
      CLK => clk90,
      O => current_reg1_7_Q,
      CE => VCC,
      SET => GND
    );
  current_reg1_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => N1,
      RST => reset_INV,
      CLK => clk90,
      O => current_reg1_0_Q,
      CE => VCC,
      SET => GND
    );
  current_reg2_7 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => N1,
      RST => reset_INV,
      CLK => clk90,
      O => current_reg2_7_Q,
      CE => VCC,
      SET => GND
    );
  current_reg2_2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => N1,
      RST => reset_INV,
      CLK => clk90,
      O => current_reg2_2_Q,
      CE => VCC,
      SET => GND
    );
  current_reg2_1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => N1,
      RST => reset_INV,
      CLK => clk90,
      O => current_reg2_1_Q,
      CE => VCC,
      SET => GND
    );
  current_reg2_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => N1,
      RST => reset_INV,
      CLK => clk90,
      O => current_reg2_0_Q,
      CE => VCC,
      SET => GND
    );
  intern_reg1_10 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(10),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(10),
      SET => GND
    );
  intern_reg1_11 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(11),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(11),
      SET => GND
    );
  intern_reg1_12 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(12),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(12),
      SET => GND
    );
  intern_reg1_13 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(13),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(13),
      SET => GND
    );
  intern_reg1_14 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(14),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(14),
      SET => GND
    );
  intern_reg1_15 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(15),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(15),
      SET => GND
    );
  intern_reg1_16 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(16),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(16),
      SET => GND
    );
  intern_reg1_17 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(17),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(17),
      SET => GND
    );
  intern_reg1_18 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(18),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(18),
      SET => GND
    );
  intern_reg1_19 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(19),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(19),
      SET => GND
    );
  intern_reg1_20 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(20),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(20),
      SET => GND
    );
  intern_reg1_21 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(21),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(21),
      SET => GND
    );
  intern_reg1_22 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(22),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(22),
      SET => GND
    );
  intern_reg1_23 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(23),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(23),
      SET => GND
    );
  intern_reg1_24 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(24),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(24),
      SET => GND
    );
  intern_reg1_25 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(25),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(25),
      SET => GND
    );
  intern_reg1_26 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(26),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(26),
      SET => GND
    );
  intern_reg1_27 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(27),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(27),
      SET => GND
    );
  intern_reg1_28 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(28),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(28),
      SET => GND
    );
  intern_reg1_29 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(29),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(29),
      SET => GND
    );
  intern_reg1_30 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(30),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(30),
      SET => GND
    );
  intern_reg1_31 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => intern_reg0(31),
      CE => NlwRenamedSignal_TP2,
      RST => reset_INV,
      CLK => clk90,
      O => intern_reg1(31),
      SET => GND
    );
  user_input_address_2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_address(2),
      RST => reset_INV,
      CLK => clk,
      O => user_input_address(2),
      CE => VCC,
      SET => GND
    );
  user_input_address_3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_address(3),
      RST => reset_INV,
      CLK => clk,
      O => user_input_address(3),
      CE => VCC,
      SET => GND
    );
  user_input_address_4 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_address(4),
      RST => reset_INV,
      CLK => clk,
      O => user_input_address(4),
      CE => VCC,
      SET => GND
    );
  user_input_address_5 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_address(5),
      RST => reset_INV,
      CLK => clk,
      O => user_input_address(5),
      CE => VCC,
      SET => GND
    );
  user_input_address_6 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_address(6),
      RST => reset_INV,
      CLK => clk,
      O => user_input_address(6),
      CE => VCC,
      SET => GND
    );
  user_input_address_7 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_address(7),
      RST => reset_INV,
      CLK => clk,
      O => user_input_address(7),
      CE => VCC,
      SET => GND
    );
  user_input_address_8 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_address(8),
      RST => reset_INV,
      CLK => clk,
      O => user_input_address(8),
      CE => VCC,
      SET => GND
    );
  user_input_address_9 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_address(9),
      RST => reset_INV,
      CLK => clk,
      O => user_input_address(9),
      CE => VCC,
      SET => GND
    );
  burst_done_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_burst_done_10,
      RST => reset_INV,
      CLK => clk,
      O => burst_done,
      CE => VCC,
      SET => GND
    );
  user_command_register_1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_reg3(1),
      RST => reset_INV,
      CLK => clk,
      O => user_command_register(1),
      CE => VCC,
      SET => GND
    );
  user_command_register_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_reg3(0),
      RST => reset_INV,
      CLK => clk,
      O => user_command_register(2),
      CE => VCC,
      SET => GND
    );
  user_config_register1_11 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_reg1_11_Q,
      RST => reset_INV,
      CLK => clk,
      O => user_config_register1(1),
      CE => VCC,
      SET => GND
    );
  user_config_register1_8 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_reg1_8_Q,
      RST => reset_INV,
      CLK => clk,
      O => user_config_register1(4),
      CE => VCC,
      SET => GND
    );
  user_config_register1_7 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_reg1_7_Q,
      RST => reset_INV,
      CLK => clk,
      O => user_config_register1(5),
      CE => VCC,
      SET => GND
    );
  user_config_register1_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_reg1_0_Q,
      RST => reset_INV,
      CLK => clk,
      O => user_config_register1(12),
      CE => VCC,
      SET => GND
    );
  user_config_register2_7 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_reg2_7_Q,
      RST => reset_INV,
      CLK => clk,
      O => user_config_register2(2),
      CE => VCC,
      SET => GND
    );
  user_config_register2_2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_reg2_2_Q,
      RST => reset_INV,
      CLK => clk,
      O => user_config_register2(7),
      CE => VCC,
      SET => GND
    );
  user_config_register2_1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_reg2_1_Q,
      RST => reset_INV,
      CLK => clk,
      O => user_config_register2(8),
      CE => VCC,
      SET => GND
    );
  user_config_register2_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_reg2_0_Q,
      RST => reset_INV,
      CLK => clk,
      O => user_config_register2(9),
      CE => VCC,
      SET => GND
    );
  current_input_data_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_0_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(0),
      CE => VCC,
      SET => GND
    );
  current_input_data_1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_1_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(1),
      CE => VCC,
      SET => GND
    );
  current_input_data_2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_19_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(2),
      CE => VCC,
      SET => GND
    );
  current_input_data_3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_12_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(3),
      CE => VCC,
      SET => GND
    );
  current_input_data_4 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_12_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(4),
      CE => VCC,
      SET => GND
    );
  current_input_data_5 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_19_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(5),
      CE => VCC,
      SET => GND
    );
  current_input_data_6 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_1_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(6),
      CE => VCC,
      SET => GND
    );
  current_input_data_7 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_0_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(7),
      CE => VCC,
      SET => GND
    );
  current_input_data_8 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_0_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(8),
      CE => VCC,
      SET => GND
    );
  current_input_data_9 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_14_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(9),
      CE => VCC,
      SET => GND
    );
  current_input_data_10 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_10_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(10),
      CE => VCC,
      SET => GND
    );
  current_input_data_11 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_11_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(11),
      CE => VCC,
      SET => GND
    );
  current_input_data_12 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_12_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(12),
      CE => VCC,
      SET => GND
    );
  current_input_data_13 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_10_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(13),
      CE => VCC,
      SET => GND
    );
  current_input_data_14 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_14_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(14),
      CE => VCC,
      SET => GND
    );
  current_input_data_15 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_15_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(15),
      CE => VCC,
      SET => GND
    );
  current_input_data_16 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_14_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(16),
      CE => VCC,
      SET => GND
    );
  current_input_data_17 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_15_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(17),
      CE => VCC,
      SET => GND
    );
  current_input_data_18 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_12_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(18),
      CE => VCC,
      SET => GND
    );
  current_input_data_19 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_19_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(19),
      CE => VCC,
      SET => GND
    );
  current_input_data_20 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_10_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(20),
      CE => VCC,
      SET => GND
    );
  current_input_data_21 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_11_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(21),
      CE => VCC,
      SET => GND
    );
  current_input_data_22 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_0_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(22),
      CE => VCC,
      SET => GND
    );
  current_input_data_23 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_1_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(23),
      CE => VCC,
      SET => GND
    );
  current_input_data_24 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_14_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(24),
      CE => VCC,
      SET => GND
    );
  current_input_data_25 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_15_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(25),
      CE => VCC,
      SET => GND
    );
  current_input_data_26 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_11_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(26),
      CE => VCC,
      SET => GND
    );
  current_input_data_27 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_19_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(27),
      CE => VCC,
      SET => GND
    );
  current_input_data_28 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_10_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(28),
      CE => VCC,
      SET => GND
    );
  current_input_data_29 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_11_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(29),
      CE => VCC,
      SET => GND
    );
  current_input_data_30 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_15_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(30),
      CE => VCC,
      SET => GND
    );
  current_input_data_31 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => next_input_data_1_Q,
      RST => reset_INV,
      CLK => clk90,
      O => current_input_data(31),
      CE => VCC,
      SET => GND
    );
  user_input_data_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(0),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(0),
      SET => GND
    );
  user_input_data_1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(1),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(1),
      SET => GND
    );
  user_input_data_2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(2),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(2),
      SET => GND
    );
  user_input_data_3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(3),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(3),
      SET => GND
    );
  user_input_data_4 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(4),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(4),
      SET => GND
    );
  user_input_data_5 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(5),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(5),
      SET => GND
    );
  user_input_data_6 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(6),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(6),
      SET => GND
    );
  user_input_data_7 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(7),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(7),
      SET => GND
    );
  user_input_data_8 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(8),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(8),
      SET => GND
    );
  user_input_data_9 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(9),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(9),
      SET => GND
    );
  user_input_data_10 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(10),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(10),
      SET => GND
    );
  user_input_data_11 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(11),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(11),
      SET => GND
    );
  user_input_data_12 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(12),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(12),
      SET => GND
    );
  user_input_data_13 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(13),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(13),
      SET => GND
    );
  user_input_data_14 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(14),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(14),
      SET => GND
    );
  user_input_data_15 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(15),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(15),
      SET => GND
    );
  user_input_data_16 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(16),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(16),
      SET => GND
    );
  user_input_data_17 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(17),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(17),
      SET => GND
    );
  user_input_data_18 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(18),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(18),
      SET => GND
    );
  user_input_data_19 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(19),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(19),
      SET => GND
    );
  user_input_data_20 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(20),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(20),
      SET => GND
    );
  user_input_data_21 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(21),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(21),
      SET => GND
    );
  user_input_data_22 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(22),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(22),
      SET => GND
    );
  user_input_data_23 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(23),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(23),
      SET => GND
    );
  user_input_data_24 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(24),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(24),
      SET => GND
    );
  user_input_data_25 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(25),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(25),
      SET => GND
    );
  user_input_data_26 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(26),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(26),
      SET => GND
    );
  user_input_data_27 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(27),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(27),
      SET => GND
    );
  user_input_data_28 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(28),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(28),
      SET => GND
    );
  user_input_data_29 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(29),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(29),
      SET => GND
    );
  user_input_data_30 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(30),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(30),
      SET => GND
    );
  user_input_data_31 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_input_data(31),
      CE => Q_n0058,
      RST => reset_INV,
      CLK => clk90,
      O => user_input_data(31),
      SET => GND
    );
  test_count_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => system_controller_Result_0_sel,
      RST => reset_INV,
      CLK => clk,
      O => test_count(0),
      CE => VCC,
      SET => GND
    );
  test_count_1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(1),
      RST => reset_INV,
      CLK => clk,
      O => test_count(1),
      CE => VCC,
      SET => GND
    );
  test_count_2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(2),
      RST => reset_INV,
      CLK => clk,
      O => test_count(2),
      CE => VCC,
      SET => GND
    );
  test_count_3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(3),
      RST => reset_INV,
      CLK => clk,
      O => test_count(3),
      CE => VCC,
      SET => GND
    );
  test_count_4 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(4),
      RST => reset_INV,
      CLK => clk,
      O => test_count(4),
      CE => VCC,
      SET => GND
    );
  test_count_5 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(5),
      RST => reset_INV,
      CLK => clk,
      O => test_count(5),
      CE => VCC,
      SET => GND
    );
  test_count_6 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(6),
      RST => reset_INV,
      CLK => clk,
      O => test_count(6),
      CE => VCC,
      SET => GND
    );
  current_state_FFd3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_state_FFd3_In_13,
      RST => reset_INV,
      CLK => clk90,
      O => current_state_FFd3_7,
      CE => VCC,
      SET => GND
    );
  current_state_FFd1 : X_FF
    generic map(
      INIT => '1'
    )
    port map (
      I => NlwRenamedSig_OI_user_bank_address(0),
      RST => reset_INV,
      CLK => clk90,
      O => current_state_FFd1_11,
      CE => VCC,
      SET => GND
    );
  current_state_FFd2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_state_FFd2_In_12,
      SET => reset_INV,
      CLK => clk90,
      O => current_state_FFd2_4,
      CE => VCC,
      RST => GND
    );
  current_state_FFd4 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_state_FFd4_In,
      RST => reset_INV,
      CLK => clk90,
      O => current_state_FFd4_5,
      CE => VCC,
      SET => GND
    );
  current_state_FFd5 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_state_FFd5_In_14,
      RST => reset_INV,
      CLK => clk90,
      O => current_state_FFd5_6,
      CE => VCC,
      SET => GND
    );
  current_state_FFd8 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_state_FFd8_In_16,
      RST => reset_INV,
      CLK => clk90,
      O => current_state_FFd8_9,
      CE => VCC,
      SET => GND
    );
  current_state_FFd6 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_state_FFd6_In_15,
      RST => reset_INV,
      CLK => clk90,
      O => current_state_FFd6_8,
      CE => VCC,
      SET => GND
    );
  current_state_FFd7 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_state_FFd7_In,
      RST => reset_INV,
      CLK => clk90,
      O => current_state_FFd7_1,
      CE => VCC,
      SET => GND
    );
  current_state_FFd9 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_state_FFd9_In_17,
      RST => reset_INV,
      CLK => clk90,
      O => current_state_FFd9_2,
      CE => VCC,
      SET => GND
    );
  current_state_FFd10 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_state_FFd10_In_18,
      RST => reset_INV,
      CLK => clk90,
      O => current_state_FFd10_3,
      CE => VCC,
      SET => GND
    );
  reset_INV1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => reset,
      O => reset_INV,
      ADR1 => GND
    );
  system_controller_Result_1_xor11 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => test_count(0),
      ADR1 => test_count(1),
      O => Result(1)
    );
  system_controller_Result_2_xor11 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => test_count(0),
      ADR1 => test_count(1),
      ADR2 => test_count(2),
      O => Result(2)
    );
  system_controller_Result_3_xor11 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => test_count(0),
      ADR1 => test_count(3),
      ADR2 => test_count(1),
      ADR3 => test_count(2),
      O => Result(3)
    );
  current_state_FFd4_In1 : X_LUT4
    generic map(
      INIT => X"F444"
    )
    port map (
      ADR0 => NlwRenamedSignal_TP1,
      ADR1 => current_state_FFd4_5,
      ADR2 => init_val,
      ADR3 => current_state_FFd3_7,
      O => current_state_FFd4_In
    );
  system_controller_Result_4_xor11 : X_MUX2
    port map (
      IA => test_count_4_rt_19,
      IB => N12,
      SEL => test_count(2),
      O => Result(4)
    );
  system_controller_Result_4_xor11_G : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => test_count(0),
      ADR1 => test_count(4),
      ADR2 => test_count(1),
      ADR3 => test_count(3),
      O => N12
    );
  Ker351 : X_LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      ADR0 => test_count(6),
      ADR1 => N11,
      O => N35
    );
  Ker111 : X_LUT3
    generic map(
      INIT => X"F4"
    )
    port map (
      ADR0 => N29,
      ADR1 => test_count(5),
      ADR2 => test_count(6),
      O => N111
    );
  TP_port_10_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(20),
      ADR1 => N111,
      ADR2 => intern_reg0(20),
      ADR3 => N35,
      O => TP_port(10)
    );
  TP_port_11_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(21),
      ADR1 => N111,
      ADR2 => intern_reg0(21),
      ADR3 => N35,
      O => TP_port(11)
    );
  TP_port_0_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(10),
      ADR1 => N111,
      ADR2 => intern_reg0(10),
      ADR3 => N35,
      O => TP_port(0)
    );
  TP_port_12_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(22),
      ADR1 => N111,
      ADR2 => intern_reg0(22),
      ADR3 => N35,
      O => TP_port(12)
    );
  TP_port_13_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(23),
      ADR1 => N111,
      ADR2 => intern_reg0(23),
      ADR3 => N35,
      O => TP_port(13)
    );
  TP_port_15_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(25),
      ADR1 => N111,
      ADR2 => intern_reg0(25),
      ADR3 => N35,
      O => TP_port(15)
    );
  TP_port_16_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(26),
      ADR1 => N111,
      ADR2 => intern_reg0(26),
      ADR3 => N35,
      O => TP_port(16)
    );
  TP_port_14_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(24),
      ADR1 => N111,
      ADR2 => intern_reg0(24),
      ADR3 => N35,
      O => TP_port(14)
    );
  TP_port_17_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(27),
      ADR1 => N111,
      ADR2 => intern_reg0(27),
      ADR3 => N35,
      O => TP_port(17)
    );
  TP_port_18_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(28),
      ADR1 => N111,
      ADR2 => intern_reg0(28),
      ADR3 => N35,
      O => TP_port(18)
    );
  TP_port_1_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(11),
      ADR1 => N111,
      ADR2 => intern_reg0(11),
      ADR3 => N35,
      O => TP_port(1)
    );
  TP_port_20_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(30),
      ADR1 => N111,
      ADR2 => intern_reg0(30),
      ADR3 => N35,
      O => TP_port(20)
    );
  TP_port_19_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(29),
      ADR1 => N111,
      ADR2 => intern_reg0(29),
      ADR3 => N35,
      O => TP_port(19)
    );
  TP_port_21_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(31),
      ADR1 => N111,
      ADR2 => intern_reg0(31),
      ADR3 => N35,
      O => TP_port(21)
    );
  TP_port_2_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(12),
      ADR1 => N111,
      ADR2 => intern_reg0(12),
      ADR3 => N35,
      O => TP_port(2)
    );
  TP_port_4_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(14),
      ADR1 => N111,
      ADR2 => intern_reg0(14),
      ADR3 => N35,
      O => TP_port(4)
    );
  TP_port_5_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(15),
      ADR1 => N111,
      ADR2 => intern_reg0(15),
      ADR3 => N35,
      O => TP_port(5)
    );
  TP_port_3_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(13),
      ADR1 => N111,
      ADR2 => intern_reg0(13),
      ADR3 => N35,
      O => TP_port(3)
    );
  TP_port_6_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(16),
      ADR1 => N111,
      ADR2 => intern_reg0(16),
      ADR3 => N35,
      O => TP_port(6)
    );
  TP_port_9_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(19),
      ADR1 => N111,
      ADR2 => intern_reg0(19),
      ADR3 => N35,
      O => TP_port(9)
    );
  TP_port_7_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(17),
      ADR1 => N111,
      ADR2 => intern_reg0(17),
      ADR3 => N35,
      O => TP_port(7)
    );
  TP_port_8_1 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => intern_reg1(18),
      ADR1 => N111,
      ADR2 => intern_reg0(18),
      ADR3 => N35,
      O => TP_port(8)
    );
  system_controller_Result_0_sel1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => test_count(0),
      O => system_controller_Result_0_sel,
      ADR1 => GND
    );
  next_burst_done1 : X_LUT4
    generic map(
      INIT => X"0302"
    )
    port map (
      ADR0 => current_state_FFd6_8,
      ADR1 => N27,
      ADR2 => loop_count(1),
      ADR3 => current_state_FFd10_3,
      O => next_burst_done
    );
  system_controller_Result_6_xor11 : X_LUT4
    generic map(
      INIT => X"BA10"
    )
    port map (
      ADR0 => test_count(6),
      ADR1 => N29,
      ADR2 => test_count(5),
      ADR3 => N11,
      O => Result(6)
    );
  Ker29_SW0 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => test_count(1),
      ADR1 => test_count(0),
      O => N271
    );
  Ker29 : X_LUT4
    generic map(
      INIT => X"7FFF"
    )
    port map (
      ADR0 => test_count(4),
      ADR1 => test_count(3),
      ADR2 => test_count(2),
      ADR3 => N271,
      O => N29
    );
  Ker1_SW0 : X_LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      ADR0 => test_count(2),
      ADR1 => test_count(1),
      ADR2 => test_count(0),
      O => N291
    );
  Ker1 : X_LUT4
    generic map(
      INIT => X"7FFF"
    )
    port map (
      ADR0 => test_count(5),
      ADR1 => test_count(4),
      ADR2 => test_count(3),
      ADR3 => N291,
      O => N11
    );
  Ker201 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => loop_count(0),
      ADR1 => loop_count(1),
      O => N20
    );
  next_reg3_1_SW0 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => current_state_FFd8_9,
      ADR1 => current_state_FFd3_7,
      O => N32
    );
  next_reg3_1_Q : X_LUT4
    generic map(
      INIT => X"FFBA"
    )
    port map (
      ADR0 => N32,
      ADR1 => N27,
      ADR2 => current_state_FFd10_3,
      ADR3 => current_state_FFd9_2,
      O => next_reg3(1)
    );
  current_state_FFd2_In_SW0 : X_LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      ADR0 => loop_count(0),
      ADR1 => current_state_FFd2_4,
      O => N34
    );
  current_state_FFd2_In : X_LUT4
    generic map(
      INIT => X"FF02"
    )
    port map (
      ADR0 => N34,
      ADR1 => N27,
      ADR2 => loop_count(1),
      ADR3 => current_state_FFd1_11,
      O => current_state_FFd2_In_12
    );
  next_reg3_2_Q : X_LUT4
    generic map(
      INIT => X"FF54"
    )
    port map (
      ADR0 => N27,
      ADR1 => current_state_FFd10_3,
      ADR2 => current_state_FFd6_8,
      ADR3 => N361,
      O => next_reg3(2)
    );
  system_controller_Result_5_xor1_SW0 : X_LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      ADR0 => test_count(3),
      ADR1 => test_count(2),
      ADR2 => test_count(1),
      ADR3 => test_count(0),
      O => N38
    );
  system_controller_Result_5_xor1 : X_LUT4
    generic map(
      INIT => X"EA40"
    )
    port map (
      ADR0 => test_count(5),
      ADR1 => test_count(4),
      ADR2 => N38,
      ADR3 => N29,
      O => Result(5)
    );
  Ker151 : X_LUT3
    generic map(
      INIT => X"F4"
    )
    port map (
      ADR0 => loop_count(2),
      ADR1 => current_state_FFd5_6,
      ADR2 => current_state_FFd9_2,
      O => N15
    );
  Ker372 : X_LUT3
    generic map(
      INIT => X"7F"
    )
    port map (
      ADR0 => col_addr(1),
      ADR1 => col_addr(0),
      ADR2 => col_addr(2),
      O => N37
    );
  current_state_FFd8_In_SW0 : X_LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      ADR0 => NlwRenamedSignal_TP1,
      ADR1 => current_state_FFd8_9,
      O => N42
    );
  current_state_FFd8_In : X_LUT4
    generic map(
      INIT => X"FF8A"
    )
    port map (
      ADR0 => current_state_FFd7_1,
      ADR1 => loop_count(7),
      ADR2 => N71,
      ADR3 => N42,
      O => current_state_FFd8_In_16
    );
  current_state_FFd3_In_SW0 : X_LUT4
    generic map(
      INIT => X"F444"
    )
    port map (
      ADR0 => init_val,
      ADR1 => current_state_FFd3_7,
      ADR2 => current_state_FFd2_4,
      ADR3 => loop_count(0),
      O => N44
    );
  current_state_FFd3_In_SW1 : X_LUT3
    generic map(
      INIT => X"F4"
    )
    port map (
      ADR0 => init_val,
      ADR1 => current_state_FFd3_7,
      ADR2 => current_state_FFd2_4,
      O => N45
    );
  current_state_FFd3_In : X_LUT4
    generic map(
      INIT => X"ABA8"
    )
    port map (
      ADR0 => N45,
      ADR1 => N27,
      ADR2 => loop_count(1),
      ADR3 => N44,
      O => current_state_FFd3_In_13
    );
  Ker132 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => current_state_FFd5_6,
      ADR1 => current_state_FFd9_2,
      O => N13
    );
  Ker1311 : X_LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      ADR0 => loop_count(0),
      ADR1 => loop_count(1),
      O => N131
    );
  Ker81 : X_LUT3
    generic map(
      INIT => X"7F"
    )
    port map (
      ADR0 => loop_count(2),
      ADR1 => loop_count(1),
      ADR2 => loop_count(0),
      O => N8
    );
  next_input_data_0_1 : X_LUT4
    generic map(
      INIT => X"02AA"
    )
    port map (
      ADR0 => current_state_FFd5_6,
      ADR1 => N131,
      ADR2 => N39,
      ADR3 => N36,
      O => next_input_data_0_Q
    );
  next_input_data_10_1 : X_LUT4
    generic map(
      INIT => X"02AA"
    )
    port map (
      ADR0 => current_state_FFd5_6,
      ADR1 => N39,
      ADR2 => loop_count(0),
      ADR3 => N36,
      O => next_input_data_10_Q
    );
  Ker3211 : X_LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      ADR0 => current_state_FFd7_1,
      ADR1 => current_state_FFd10_3,
      ADR2 => current_state_FFd2_4,
      ADR3 => current_state_FFd6_8,
      O => N321
    );
  current_state_FFd7_In1 : X_LUT4
    generic map(
      INIT => X"FF20"
    )
    port map (
      ADR0 => N27,
      ADR1 => NlwRenamedSignal_TP1,
      ADR2 => current_state_FFd6_8,
      ADR3 => N371,
      O => current_state_FFd7_In
    );
  Ker30_SW0 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => col_addr(0),
      ADR1 => col_addr(4),
      O => N58
    );
  Ker30 : X_LUT4
    generic map(
      INIT => X"7FFF"
    )
    port map (
      ADR0 => col_addr(3),
      ADR1 => col_addr(2),
      ADR2 => col_addr(1),
      ADR3 => N58,
      O => N30
    );
  current_state_FFd6_In_SW0 : X_LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      ADR0 => NlwRenamedSignal_TP1,
      ADR1 => N27,
      O => N60
    );
  current_state_FFd6_In : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => Q_n0043,
      ADR1 => current_state_FFd5_6,
      ADR2 => current_state_FFd6_8,
      ADR3 => N60,
      O => current_state_FFd6_In_15
    );
  current_state_FFd10_In_SW0 : X_LUT3
    generic map(
      INIT => X"FB"
    )
    port map (
      ADR0 => NlwRenamedSignal_TP2,
      ADR1 => N27,
      ADR2 => NlwRenamedSignal_TP1,
      O => N62
    );
  current_state_FFd10_In : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => Q_n0043,
      ADR1 => current_state_FFd9_2,
      ADR2 => current_state_FFd10_3,
      ADR3 => N62,
      O => current_state_FFd10_In_18
    );
  Q_n00580 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => current_state_FFd5_6,
      ADR1 => current_state_FFd4_5,
      O => N64
    );
  Q_n005836 : X_LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      ADR0 => NlwRenamedSignal_TP1,
      ADR1 => current_state_FFd9_2,
      ADR2 => current_state_FFd3_7,
      ADR3 => current_state_FFd8_9,
      O => N96
    );
  Q_n005837 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => N84,
      ADR1 => N96,
      O => Q_n0058
    );
  Q_n00431 : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => loop_count(0),
      ADR1 => loop_count(2),
      ADR2 => loop_count(1),
      ADR3 => N311,
      O => Q_n0043
    );
  current_state_FFd5_In_SW0 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => current_state_FFd4_5,
      ADR1 => NlwRenamedSignal_TP1,
      O => N1011
    );
  current_state_FFd5_In : X_LUT4
    generic map(
      INIT => X"FF2A"
    )
    port map (
      ADR0 => current_state_FFd5_6,
      ADR1 => N27,
      ADR2 => Q_n0043,
      ADR3 => N1011,
      O => current_state_FFd5_In_14
    );
  current_state_FFd9_In_SW0 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => current_state_FFd8_9,
      ADR1 => NlwRenamedSignal_TP1,
      O => N103
    );
  current_state_FFd9_In : X_LUT4
    generic map(
      INIT => X"FF2A"
    )
    port map (
      ADR0 => current_state_FFd9_2,
      ADR1 => N27,
      ADR2 => Q_n0043,
      ADR3 => N103,
      O => current_state_FFd9_In_17
    );
  Ker10_SW0 : X_LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      ADR0 => loop_count(2),
      ADR1 => loop_count(1),
      ADR2 => loop_count(0),
      O => N105
    );
  Ker10 : X_LUT4
    generic map(
      INIT => X"7FFF"
    )
    port map (
      ADR0 => loop_count(4),
      ADR1 => loop_count(5),
      ADR2 => loop_count(3),
      ADR3 => N105,
      O => N10
    );
  next_input_data_14_1 : X_LUT4
    generic map(
      INIT => X"AA08"
    )
    port map (
      ADR0 => current_state_FFd5_6,
      ADR1 => loop_count(0),
      ADR2 => N39,
      ADR3 => Q_n0043,
      O => next_input_data_14_Q
    );
  next_input_data_12_1 : X_LUT4
    generic map(
      INIT => X"AA08"
    )
    port map (
      ADR0 => current_state_FFd5_6,
      ADR1 => N20,
      ADR2 => N39,
      ADR3 => Q_n0043,
      O => next_input_data_12_Q
    );
  Q_n0056_3_1 : X_LUT4
    generic map(
      INIT => X"BA90"
    )
    port map (
      ADR0 => loop_count(3),
      ADR1 => N8,
      ADR2 => N18,
      ADR3 => N121,
      O => Q_n0056(3)
    );
  Q_n0056_6_1 : X_LUT4
    generic map(
      INIT => X"BA90"
    )
    port map (
      ADR0 => loop_count(6),
      ADR1 => N10,
      ADR2 => N18,
      ADR3 => N121,
      O => Q_n0056(6)
    );
  Ker40_SW0 : X_LUT4
    generic map(
      INIT => X"F8C8"
    )
    port map (
      ADR0 => current_state_FFd6_8,
      ADR1 => NlwRenamedSignal_TP1,
      ADR2 => current_state_FFd10_3,
      ADR3 => NlwRenamedSignal_TP2,
      O => N112
    );
  Ker241 : X_LUT3
    generic map(
      INIT => X"BF"
    )
    port map (
      ADR0 => N23,
      ADR1 => col_addr(4),
      ADR2 => col_addr(5),
      O => N24
    );
  Q_n0057_0_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => col_addr(0),
      ADR1 => N4,
      ADR2 => N14,
      O => Q_n0057(0)
    );
  Ker231 : X_LUT4
    generic map(
      INIT => X"7FFF"
    )
    port map (
      ADR0 => col_addr(2),
      ADR1 => col_addr(3),
      ADR2 => col_addr(0),
      ADR3 => col_addr(1),
      O => N23
    );
  Q_n0056_0_42 : X_LUT4
    generic map(
      INIT => X"DDD8"
    )
    port map (
      ADR0 => loop_count(0),
      ADR1 => N101,
      ADR2 => N18,
      ADR3 => N163,
      O => Q_n0056(0)
    );
  Ker391 : X_LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      ADR0 => loop_count(2),
      ADR1 => N311,
      O => N39
    );
  Q_n0057_3_Q : X_LUT4
    generic map(
      INIT => X"FAD8"
    )
    port map (
      ADR0 => col_addr(3),
      ADR1 => N14,
      ADR2 => N219,
      ADR3 => N220,
      O => Q_n0057(3)
    );
  Q_n0057_1_SW0 : X_LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      ADR0 => N4,
      ADR1 => col_addr(0),
      O => N222
    );
  Q_n0057_1_Q : X_LUT4
    generic map(
      INIT => X"AF27"
    )
    port map (
      ADR0 => col_addr(1),
      ADR1 => N223,
      ADR2 => N222,
      ADR3 => N14,
      O => Q_n0057(1)
    );
  Ker27_SW0 : X_LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      ADR0 => loop_count(5),
      ADR1 => loop_count(3),
      ADR2 => loop_count(2),
      O => N225
    );
  Ker27 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => loop_count(7),
      ADR1 => loop_count(4),
      ADR2 => loop_count(6),
      ADR3 => N225,
      O => N27
    );
  Q_n0057_6_SW0 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => col_addr(5),
      ADR1 => N30,
      ADR2 => N4,
      O => N229
    );
  Q_n0057_6_Q : X_LUT4
    generic map(
      INIT => X"FAD8"
    )
    port map (
      ADR0 => col_addr(6),
      ADR1 => N14,
      ADR2 => N229,
      ADR3 => N230,
      O => Q_n0057(6)
    );
  Q_n0057_2_SW0 : X_LUT3
    generic map(
      INIT => X"7F"
    )
    port map (
      ADR0 => col_addr(1),
      ADR1 => col_addr(0),
      ADR2 => N4,
      O => N232
    );
  Q_n0057_2_SW1 : X_LUT3
    generic map(
      INIT => X"D5"
    )
    port map (
      ADR0 => N15,
      ADR1 => col_addr(1),
      ADR2 => col_addr(0),
      O => N233
    );
  Q_n0057_2_Q : X_LUT4
    generic map(
      INIT => X"AF27"
    )
    port map (
      ADR0 => col_addr(2),
      ADR1 => N233,
      ADR2 => N232,
      ADR3 => N14,
      O => Q_n0057(2)
    );
  Ker45 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => current_state_FFd5_6,
      ADR1 => loop_count(2),
      ADR2 => N311,
      O => Ker4_map506
    );
  Ker417 : X_LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      ADR0 => loop_count(0),
      ADR1 => loop_count(1),
      O => Ker4_map511
    );
  Q_n0057_4_SW0 : X_LUT4
    generic map(
      INIT => X"AA08"
    )
    port map (
      ADR0 => N23,
      ADR1 => current_state_FFd5_6,
      ADR2 => loop_count(2),
      ADR3 => current_state_FFd9_2,
      O => N254
    );
  Q_n0057_4_Q : X_LUT4
    generic map(
      INIT => X"FAD8"
    )
    port map (
      ADR0 => col_addr(4),
      ADR1 => N14,
      ADR2 => N341,
      ADR3 => N254,
      O => Q_n0057(4)
    );
  Q_n0057_7_20 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => col_addr(6),
      ADR1 => col_addr(7),
      ADR2 => col_addr(5),
      O => N274
    );
  Q_n0057_7_29 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => N274,
      ADR1 => N30,
      ADR2 => N4,
      O => N283
    );
  Q_n0057_7_35 : X_LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      ADR0 => col_addr(7),
      ADR1 => N14,
      ADR2 => N283,
      ADR3 => N260,
      O => Q_n0057(7)
    );
  N145 : X_LUT3
    generic map(
      INIT => X"FB"
    )
    port map (
      ADR0 => loop_count(0),
      ADR1 => loop_count(1),
      ADR2 => N27,
      O => N14_map527
    );
  N1418 : X_LUT3
    generic map(
      INIT => X"01"
    )
    port map (
      ADR0 => current_state_FFd9_2,
      ADR1 => current_state_FFd4_5,
      ADR2 => current_state_FFd5_6,
      O => N14_map533
    );
  N1434 : X_LUT3
    generic map(
      INIT => X"AB"
    )
    port map (
      ADR0 => loop_count(0),
      ADR1 => loop_count(1),
      ADR2 => loop_count(2),
      O => N14_map538
    );
  N1435 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => loop_count(1),
      ADR1 => loop_count(2),
      O => N14_map539
    );
  N1452 : X_LUT4
    generic map(
      INIT => X"AA8A"
    )
    port map (
      ADR0 => current_state_FFd5_6,
      ADR1 => N14_map539,
      ADR2 => N311,
      ADR3 => N14_map538,
      O => N14_map543
    );
  N1459 : X_LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      ADR0 => N14_map533,
      ADR1 => current_state_FFd9_2,
      ADR2 => N14_map527,
      ADR3 => N14_map543,
      O => N14
    );
  Ker3411 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => col_addr(3),
      ADR1 => N37,
      ADR2 => N4,
      O => N341
    );
  Q_n0056_7_SW0 : X_LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      ADR0 => N10,
      ADR1 => loop_count(6),
      O => N337
    );
  Q_n0056_7_Q : X_LUT4
    generic map(
      INIT => X"EA60"
    )
    port map (
      ADR0 => loop_count(7),
      ADR1 => N337,
      ADR2 => N18,
      ADR3 => N121,
      O => Q_n0056(7)
    );
  Q_n0056_4_Q : X_LUT4
    generic map(
      INIT => X"EA60"
    )
    port map (
      ADR0 => loop_count(4),
      ADR1 => N339,
      ADR2 => N18,
      ADR3 => N121,
      O => Q_n0056(4)
    );
  Q_n0056_5_SW0 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => loop_count(4),
      ADR1 => N8,
      ADR2 => loop_count(3),
      O => N3411
    );
  Q_n0056_5_Q : X_LUT4
    generic map(
      INIT => X"EA60"
    )
    port map (
      ADR0 => loop_count(5),
      ADR1 => N3411,
      ADR2 => N18,
      ADR3 => N121,
      O => Q_n0056(5)
    );
  Ker3711 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => current_state_FFd7_1,
      ADR1 => loop_count(7),
      ADR2 => N71,
      O => N371
    );
  Ker181 : X_LUT4
    generic map(
      INIT => X"FF54"
    )
    port map (
      ADR0 => N27,
      ADR1 => current_state_FFd6_8,
      ADR2 => current_state_FFd10_3,
      ADR3 => N371,
      O => N18
    );
  Ker71_SW0 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => loop_count(3),
      ADR1 => loop_count(2),
      O => N345
    );
  Ker71 : X_LUT4
    generic map(
      INIT => X"777F"
    )
    port map (
      ADR0 => loop_count(5),
      ADR1 => loop_count(6),
      ADR2 => N345,
      ADR3 => loop_count(4),
      O => N71
    );
  next_addr_9_1 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => col_addr(7),
      ADR1 => N0,
      O => next_addr(9)
    );
  next_addr_7_1 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => col_addr(5),
      ADR1 => N0,
      O => next_addr(7)
    );
  next_addr_8_1 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => col_addr(6),
      ADR1 => N0,
      O => next_addr(8)
    );
  next_addr_4_1 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => col_addr(2),
      ADR1 => N0,
      O => next_addr(4)
    );
  next_addr_6_1 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => col_addr(4),
      ADR1 => N0,
      O => next_addr(6)
    );
  next_addr_3_1 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => col_addr(1),
      ADR1 => N0,
      O => next_addr(3)
    );
  next_addr_5_1 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => col_addr(3),
      ADR1 => N0,
      O => next_addr(5)
    );
  next_addr_2_1 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => col_addr(0),
      ADR1 => N0,
      O => next_addr(2)
    );
  Ker361 : X_LUT3
    generic map(
      INIT => X"EF"
    )
    port map (
      ADR0 => loop_count(1),
      ADR1 => loop_count(0),
      ADR2 => N311,
      O => N36
    );
  Ker311_SW0 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => loop_count(5),
      ADR1 => loop_count(3),
      O => N360
    );
  Ker311 : X_LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      ADR0 => loop_count(7),
      ADR1 => loop_count(4),
      ADR2 => loop_count(6),
      ADR3 => N360,
      O => N311
    );
  test_count_4_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => test_count(4),
      O => test_count_4_rt_19,
      ADR1 => GND
    );
  Ker0_SW0_SW0 : X_LUT4
    generic map(
      INIT => X"BA8A"
    )
    port map (
      ADR0 => current_state_FFd8_9,
      ADR1 => loop_count(2),
      ADR2 => N311,
      ADR3 => N358,
      O => N362
    );
  Ker419 : X_LUT4
    generic map(
      INIT => X"AA08"
    )
    port map (
      ADR0 => Ker4_map511,
      ADR1 => current_state_FFd9_2,
      ADR2 => N27,
      ADR3 => Ker4_map506,
      O => N4
    );
  Q_n0057_7_4 : X_LUT3
    generic map(
      INIT => X"D0"
    )
    port map (
      ADR0 => col_addr(6),
      ADR1 => N24,
      ADR2 => N4,
      O => N260
    );
  Q_n0057_3_SW0 : X_LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      ADR0 => col_addr(1),
      ADR1 => col_addr(0),
      ADR2 => col_addr(2),
      ADR3 => N4,
      O => N219
    );
  Q_n0057_6_SW1 : X_LUT4
    generic map(
      INIT => X"F070"
    )
    port map (
      ADR0 => col_addr(5),
      ADR1 => col_addr(4),
      ADR2 => N4,
      ADR3 => N23,
      O => N230
    );
  Q_n0056_2_49 : X_LUT4
    generic map(
      INIT => X"FEFC"
    )
    port map (
      ADR0 => loop_count(2),
      ADR1 => N197,
      ADR2 => N211,
      ADR3 => N367,
      O => Q_n0056(2)
    );
  Q_n0056_2_44 : X_LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      ADR0 => loop_count(0),
      ADR1 => loop_count(2),
      ADR2 => N18,
      ADR3 => loop_count(1),
      O => N211
    );
  Q_n005821 : X_LUT4
    generic map(
      INIT => X"F302"
    )
    port map (
      ADR0 => current_state_FFd6_8,
      ADR1 => current_state_FFd10_3,
      ADR2 => N27,
      ADR3 => N64,
      O => N84
    );
  next_input_data_11_1 : X_LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      ADR0 => loop_count(2),
      ADR1 => current_state_FFd5_6,
      ADR2 => N311,
      ADR3 => N20,
      O => next_input_data_11_Q
    );
  next_input_data_1_1 : X_LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      ADR0 => loop_count(2),
      ADR1 => current_state_FFd5_6,
      ADR2 => N311,
      ADR3 => loop_count(0),
      O => next_input_data_1_Q
    );
  Ker121 : X_LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      ADR0 => N13,
      ADR1 => N27,
      ADR2 => N112,
      ADR3 => N321,
      O => N121
    );
  next_input_data_15_1 : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => loop_count(2),
      ADR1 => N311,
      ADR2 => N20,
      ADR3 => current_state_FFd5_6,
      O => next_input_data_15_Q
    );
  next_input_data_19_1 : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => loop_count(2),
      ADR1 => N311,
      ADR2 => loop_count(0),
      ADR3 => current_state_FFd5_6,
      O => next_input_data_19_Q
    );
  Ker0_SW1 : X_LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      ADR0 => current_state_FFd5_6,
      ADR1 => current_state_FFd9_2,
      ADR2 => current_state_FFd8_9,
      O => N358
    );
  next_reg3_2_SW0 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => current_state_FFd5_6,
      ADR1 => current_state_FFd9_2,
      ADR2 => current_state_FFd4_5,
      ADR3 => current_state_FFd8_9,
      O => N361
    );
  Q_n0057_1_SW1 : X_LUT4
    generic map(
      INIT => X"FF23"
    )
    port map (
      ADR0 => loop_count(2),
      ADR1 => current_state_FFd9_2,
      ADR2 => current_state_FFd5_6,
      ADR3 => col_addr(0),
      O => N223
    );
  Q_n0056_4_SW0 : X_LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      ADR0 => loop_count(2),
      ADR1 => loop_count(1),
      ADR2 => loop_count(0),
      ADR3 => loop_count(3),
      O => N339
    );
  Q_n0056_2_23 : X_LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      ADR0 => N13,
      ADR1 => loop_count(0),
      ADR2 => loop_count(1),
      ADR3 => N311,
      O => N197
    );
  Q_n0057_3_SW1 : X_LUT4
    generic map(
      INIT => X"4CCC"
    )
    port map (
      ADR0 => col_addr(1),
      ADR1 => N15,
      ADR2 => col_addr(0),
      ADR3 => col_addr(2),
      O => N220
    );
  Ker0_SW0 : X_LUT4
    generic map(
      INIT => X"FFAB"
    )
    port map (
      ADR0 => loop_count(1),
      ADR1 => current_state_FFd9_2,
      ADR2 => current_state_FFd5_6,
      ADR3 => loop_count(0),
      O => N369
    );
  Ker0 : X_LUT4
    generic map(
      INIT => X"FF20"
    )
    port map (
      ADR0 => loop_count(2),
      ADR1 => N369,
      ADR2 => N311,
      ADR3 => N362,
      O => N0
    );
  Q_n0056_0_16_SW0 : X_LUT4
    generic map(
      INIT => X"00E0"
    )
    port map (
      ADR0 => current_state_FFd9_2,
      ADR1 => current_state_FFd5_6,
      ADR2 => N311,
      ADR3 => loop_count(2),
      O => N3711
    );
  Q_n0056_0_16 : X_LUT4
    generic map(
      INIT => X"FF02"
    )
    port map (
      ADR0 => current_state_FFd2_4,
      ADR1 => N27,
      ADR2 => loop_count(1),
      ADR3 => N3711,
      O => N163
    );
  Q_n0056_2_14_SW0_SW0 : X_LUT4
    generic map(
      INIT => X"F444"
    )
    port map (
      ADR0 => N13,
      ADR1 => N321,
      ADR2 => N112,
      ADR3 => N27,
      O => N373
    );
  Q_n0056_2_14_SW0 : X_LUT4
    generic map(
      INIT => X"FEF0"
    )
    port map (
      ADR0 => current_state_FFd9_2,
      ADR1 => current_state_FFd5_6,
      ADR2 => N373,
      ADR3 => N36,
      O => N367
    );
  Q_n0056_1_30 : X_MUX2
    port map (
      IA => N378,
      IB => N379,
      SEL => loop_count(1),
      O => Q_n0056(1)
    );
  Q_n0056_1_30_F : X_LUT4
    generic map(
      INIT => X"AA08"
    )
    port map (
      ADR0 => loop_count(0),
      ADR1 => N13,
      ADR2 => N39,
      ADR3 => N18,
      O => N378
    );
  Q_n0056_1_30_G : X_LUT4
    generic map(
      INIT => X"FF54"
    )
    port map (
      ADR0 => loop_count(0),
      ADR1 => N13,
      ADR2 => N18,
      ADR3 => N101,
      O => N379
    );
  Ker1011 : X_MUX2
    port map (
      IA => N380,
      IB => N381,
      SEL => N13,
      O => N101
    );
  Ker1011_F : X_LUT3
    generic map(
      INIT => X"EA"
    )
    port map (
      ADR0 => N321,
      ADR1 => N112,
      ADR2 => N27,
      O => N380
    );
  Ker1011_G : X_LUT4
    generic map(
      INIT => X"EAFF"
    )
    port map (
      ADR0 => loop_count(2),
      ADR1 => N112,
      ADR2 => N27,
      ADR3 => N311,
      O => N381
    );
  Q_n0057_5_Q : X_MUX2
    port map (
      IA => N384,
      IB => N385,
      SEL => col_addr(5),
      O => Q_n0057(5)
    );
  Q_n0057_5_F : X_LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      ADR0 => N37,
      ADR1 => col_addr(3),
      ADR2 => col_addr(4),
      ADR3 => N4,
      O => N384
    );
  Q_n0057_5_G : X_LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      ADR0 => N4,
      ADR1 => N30,
      ADR2 => N14,
      O => N385
    );
  NlwBlock_system_controller_inst_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlock_system_controller_inst_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal VCC : STD_LOGIC; 
  signal delay_sel_val1_r : STD_LOGIC_VECTOR ( 4 downto 0 ); 
begin
  delay_sel_val1_r_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => delay_sel_val(0),
      SRST => sys_rst,
      CE => Q_n0004,
      CLK => clk_int,
      O => delay_sel_val1_r(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  delay_sel_val1_r_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => delay_sel_val(1),
      SRST => sys_rst,
      CE => Q_n0004,
      CLK => clk_int,
      O => delay_sel_val1_r(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  delay_sel_val1_r_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => delay_sel_val(2),
      SRST => sys_rst,
      CE => Q_n0004,
      CLK => clk_int,
      O => delay_sel_val1_r(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  delay_sel_val1_r_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => delay_sel_val(3),
      SRST => sys_rst,
      CE => Q_n0004,
      CLK => clk_int,
      O => delay_sel_val1_r(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  delay_sel_val1_r_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => delay_sel_val(4),
      SRST => sys_rst,
      CE => Q_n0004,
      CLK => clk_int,
      O => delay_sel_val1_r(4),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  rst_calib1_r1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => rst_calib1,
      SRST => sys_rst,
      CLK => clk_int,
      O => rst_calib1_r1_20,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  rst_calib1_r2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => rst_calib1_r1_20,
      SRST => sys_rst,
      CLK => clk_int,
      O => rst_calib1_r2_21,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  delay_sel_val1_4_1 : X_LUT4
    generic map(
      INIT => X"ABA8"
    )
    port map (
      ADR0 => delay_sel_val1_r(4),
      ADR1 => rst_calib1,
      ADR2 => rst_calib1_r2_21,
      ADR3 => delay_sel_val(4),
      O => delay_sel_val1_val(4)
    );
  delay_sel_val1_3_1 : X_LUT4
    generic map(
      INIT => X"ABA8"
    )
    port map (
      ADR0 => delay_sel_val1_r(3),
      ADR1 => rst_calib1,
      ADR2 => rst_calib1_r2_21,
      ADR3 => delay_sel_val(3),
      O => delay_sel_val1_val(3)
    );
  delay_sel_val1_2_1 : X_LUT4
    generic map(
      INIT => X"ABA8"
    )
    port map (
      ADR0 => delay_sel_val1_r(2),
      ADR1 => rst_calib1,
      ADR2 => rst_calib1_r2_21,
      ADR3 => delay_sel_val(2),
      O => delay_sel_val1_val(2)
    );
  delay_sel_val1_1_1 : X_LUT4
    generic map(
      INIT => X"ABA8"
    )
    port map (
      ADR0 => delay_sel_val1_r(1),
      ADR1 => rst_calib1,
      ADR2 => rst_calib1_r2_21,
      ADR3 => delay_sel_val(1),
      O => delay_sel_val1_val(1)
    );
  delay_sel_val1_0_1 : X_LUT4
    generic map(
      INIT => X"ABA8"
    )
    port map (
      ADR0 => delay_sel_val1_r(0),
      ADR1 => rst_calib1,
      ADR2 => rst_calib1_r2_21,
      ADR3 => delay_sel_val(0),
      O => delay_sel_val1_val(0)
    );
  Q_n00041 : X_LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      ADR0 => rst_calib1,
      ADR1 => rst_calib1_r2_21,
      O => Q_n0004
    );
  NlwBlock_infrastructure0_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlock_infrastructure0_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal Mshreg_write_data3_2 : STD_LOGIC; 
  signal Mshreg_write_data3_0 : STD_LOGIC; 
  signal Mshreg_write_data3_1 : STD_LOGIC; 
  signal Mshreg_write_data3_3 : STD_LOGIC; 
  signal Mshreg_write_data3_4 : STD_LOGIC; 
  signal Mshreg_write_data3_5 : STD_LOGIC; 
  signal Mshreg_write_data3_6 : STD_LOGIC; 
  signal Mshreg_write_data3_7 : STD_LOGIC; 
  signal Mshreg_write_data3_8 : STD_LOGIC; 
  signal Mshreg_write_data3_11 : STD_LOGIC; 
  signal Mshreg_write_data3_9 : STD_LOGIC; 
  signal Mshreg_write_data3_10 : STD_LOGIC; 
  signal Mshreg_write_data3_12 : STD_LOGIC; 
  signal Mshreg_write_data3_13 : STD_LOGIC; 
  signal Mshreg_write_data3_14 : STD_LOGIC; 
  signal Mshreg_write_data3_15 : STD_LOGIC; 
  signal Mshreg_write_data3_16 : STD_LOGIC; 
  signal Mshreg_write_data3_17 : STD_LOGIC; 
  signal Mshreg_write_data3_20 : STD_LOGIC; 
  signal Mshreg_write_data3_18 : STD_LOGIC; 
  signal Mshreg_write_data3_19 : STD_LOGIC; 
  signal Mshreg_write_data3_21 : STD_LOGIC; 
  signal Mshreg_write_data3_22 : STD_LOGIC; 
  signal Mshreg_write_data3_23 : STD_LOGIC; 
  signal Mshreg_write_data3_24 : STD_LOGIC; 
  signal Mshreg_write_data3_25 : STD_LOGIC; 
  signal Mshreg_write_data3_26 : STD_LOGIC; 
  signal Mshreg_write_data3_29 : STD_LOGIC; 
  signal Mshreg_write_data3_27 : STD_LOGIC; 
  signal Mshreg_write_data3_28 : STD_LOGIC; 
  signal Mshreg_write_data3_30 : STD_LOGIC; 
  signal Mshreg_write_data3_31 : STD_LOGIC; 
  signal Mshreg_write_data_m3_0 : STD_LOGIC; 
  signal Mshreg_write_data_m3_1 : STD_LOGIC; 
  signal Mshreg_write_data_m3_2 : STD_LOGIC; 
  signal Mshreg_write_data_m3_3 : STD_LOGIC; 
  signal Mshreg_write_data_mask_0 : STD_LOGIC; 
  signal Mshreg_write_data_mask_1 : STD_LOGIC; 
  signal Mshreg_write_data_0 : STD_LOGIC; 
  signal Mshreg_write_data_1 : STD_LOGIC; 
  signal Mshreg_write_data_2 : STD_LOGIC; 
  signal Mshreg_write_data_3 : STD_LOGIC; 
  signal Mshreg_write_data_4 : STD_LOGIC; 
  signal Mshreg_write_data_5 : STD_LOGIC; 
  signal Mshreg_write_data_6 : STD_LOGIC; 
  signal Mshreg_write_data_9 : STD_LOGIC; 
  signal Mshreg_write_data_7 : STD_LOGIC; 
  signal Mshreg_write_data_8 : STD_LOGIC; 
  signal Mshreg_write_data_10 : STD_LOGIC; 
  signal Mshreg_write_data_11 : STD_LOGIC; 
  signal Mshreg_write_data_12 : STD_LOGIC; 
  signal Mshreg_write_data_13 : STD_LOGIC; 
  signal Mshreg_write_data_14 : STD_LOGIC; 
  signal Mshreg_write_data_15 : STD_LOGIC; 
  signal Mshreg_data_mask_r_0 : STD_LOGIC; 
  signal Mshreg_data_mask_r_1 : STD_LOGIC; 
  signal Mshreg_write_data_rising_0 : STD_LOGIC; 
  signal Mshreg_write_data_rising_1 : STD_LOGIC; 
  signal Mshreg_write_data_rising_2 : STD_LOGIC; 
  signal Mshreg_write_data_rising_3 : STD_LOGIC; 
  signal Mshreg_write_data_rising_4 : STD_LOGIC; 
  signal Mshreg_write_data_rising_7 : STD_LOGIC; 
  signal Mshreg_write_data_rising_5 : STD_LOGIC; 
  signal Mshreg_write_data_rising_6 : STD_LOGIC; 
  signal Mshreg_write_data_rising_8 : STD_LOGIC; 
  signal Mshreg_write_data_rising_9 : STD_LOGIC; 
  signal Mshreg_write_data_rising_10 : STD_LOGIC; 
  signal Mshreg_write_data_rising_11 : STD_LOGIC; 
  signal Mshreg_write_data_rising_12 : STD_LOGIC; 
  signal Mshreg_write_data_rising_13 : STD_LOGIC; 
  signal Mshreg_write_data_rising_14 : STD_LOGIC; 
  signal Mshreg_write_data_rising_15 : STD_LOGIC; 
  signal Mshreg_write_data3_2_CE : STD_LOGIC; 
  signal Mshreg_write_data3_0_CE : STD_LOGIC; 
  signal Mshreg_write_data3_1_CE : STD_LOGIC; 
  signal Mshreg_write_data3_3_CE : STD_LOGIC; 
  signal Mshreg_write_data3_4_CE : STD_LOGIC; 
  signal Mshreg_write_data3_5_CE : STD_LOGIC; 
  signal Mshreg_write_data3_6_CE : STD_LOGIC; 
  signal Mshreg_write_data3_7_CE : STD_LOGIC; 
  signal Mshreg_write_data3_8_CE : STD_LOGIC; 
  signal Mshreg_write_data3_11_CE : STD_LOGIC; 
  signal Mshreg_write_data3_9_CE : STD_LOGIC; 
  signal Mshreg_write_data3_10_CE : STD_LOGIC; 
  signal Mshreg_write_data3_12_CE : STD_LOGIC; 
  signal Mshreg_write_data3_13_CE : STD_LOGIC; 
  signal Mshreg_write_data3_14_CE : STD_LOGIC; 
  signal Mshreg_write_data3_15_CE : STD_LOGIC; 
  signal Mshreg_write_data3_16_CE : STD_LOGIC; 
  signal Mshreg_write_data3_17_CE : STD_LOGIC; 
  signal Mshreg_write_data3_20_CE : STD_LOGIC; 
  signal Mshreg_write_data3_18_CE : STD_LOGIC; 
  signal Mshreg_write_data3_19_CE : STD_LOGIC; 
  signal Mshreg_write_data3_21_CE : STD_LOGIC; 
  signal Mshreg_write_data3_22_CE : STD_LOGIC; 
  signal Mshreg_write_data3_23_CE : STD_LOGIC; 
  signal Mshreg_write_data3_24_CE : STD_LOGIC; 
  signal Mshreg_write_data3_25_CE : STD_LOGIC; 
  signal Mshreg_write_data3_26_CE : STD_LOGIC; 
  signal Mshreg_write_data3_29_CE : STD_LOGIC; 
  signal Mshreg_write_data3_27_CE : STD_LOGIC; 
  signal Mshreg_write_data3_28_CE : STD_LOGIC; 
  signal Mshreg_write_data3_30_CE : STD_LOGIC; 
  signal Mshreg_write_data3_31_CE : STD_LOGIC; 
  signal Mshreg_write_data_m3_0_CE : STD_LOGIC; 
  signal Mshreg_write_data_m3_1_CE : STD_LOGIC; 
  signal Mshreg_write_data_m3_2_CE : STD_LOGIC; 
  signal Mshreg_write_data_m3_3_CE : STD_LOGIC; 
  signal Mshreg_write_data_mask_0_CE : STD_LOGIC; 
  signal Mshreg_write_data_mask_1_CE : STD_LOGIC; 
  signal Mshreg_write_data_0_CE : STD_LOGIC; 
  signal Mshreg_write_data_1_CE : STD_LOGIC; 
  signal Mshreg_write_data_2_CE : STD_LOGIC; 
  signal Mshreg_write_data_3_CE : STD_LOGIC; 
  signal Mshreg_write_data_4_CE : STD_LOGIC; 
  signal Mshreg_write_data_5_CE : STD_LOGIC; 
  signal Mshreg_write_data_6_CE : STD_LOGIC; 
  signal Mshreg_write_data_9_CE : STD_LOGIC; 
  signal Mshreg_write_data_7_CE : STD_LOGIC; 
  signal Mshreg_write_data_8_CE : STD_LOGIC; 
  signal Mshreg_write_data_10_CE : STD_LOGIC; 
  signal Mshreg_write_data_11_CE : STD_LOGIC; 
  signal Mshreg_write_data_12_CE : STD_LOGIC; 
  signal Mshreg_write_data_13_CE : STD_LOGIC; 
  signal Mshreg_write_data_14_CE : STD_LOGIC; 
  signal Mshreg_write_data_15_CE : STD_LOGIC; 
  signal Mshreg_data_mask_r_0_CE : STD_LOGIC; 
  signal Mshreg_data_mask_r_1_CE : STD_LOGIC; 
  signal Mshreg_write_data_rising_0_CE : STD_LOGIC; 
  signal Mshreg_write_data_rising_1_CE : STD_LOGIC; 
  signal Mshreg_write_data_rising_2_CE : STD_LOGIC; 
  signal Mshreg_write_data_rising_3_CE : STD_LOGIC; 
  signal Mshreg_write_data_rising_4_CE : STD_LOGIC; 
  signal Mshreg_write_data_rising_7_CE : STD_LOGIC; 
  signal Mshreg_write_data_rising_5_CE : STD_LOGIC; 
  signal Mshreg_write_data_rising_6_CE : STD_LOGIC; 
  signal Mshreg_write_data_rising_8_CE : STD_LOGIC; 
  signal Mshreg_write_data_rising_9_CE : STD_LOGIC; 
  signal Mshreg_write_data_rising_10_CE : STD_LOGIC; 
  signal Mshreg_write_data_rising_11_CE : STD_LOGIC; 
  signal Mshreg_write_data_rising_12_CE : STD_LOGIC; 
  signal Mshreg_write_data_rising_13_CE : STD_LOGIC; 
  signal Mshreg_write_data_rising_14_CE : STD_LOGIC; 
  signal Mshreg_write_data_rising_15_CE : STD_LOGIC; 
  signal VCC : STD_LOGIC; 
  signal GND : STD_LOGIC; 
  signal NlwInverterSignal_mem_interface_top_inst_ddr2_top0_data_path0_data_write0_write_en_val_C : STD_LOGIC; 
  signal NLW_write_data_mo31_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo30_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo29_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo28_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo27_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo26_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo25_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo24_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo23_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo22_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo21_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo20_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo19_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo18_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo17_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mo16_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mas3_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_write_data_mas2_O_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_2_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_0_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_1_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_3_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_4_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_5_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_6_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_7_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_8_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_11_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_9_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_10_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_12_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_13_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_14_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_15_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_16_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_17_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_20_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_18_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_19_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_21_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_22_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_23_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_24_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_25_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_26_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_29_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_27_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_28_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_30_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data3_31_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_m3_0_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_m3_1_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_m3_2_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_m3_3_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_mask_0_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_mask_1_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_0_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_1_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_2_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_3_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_4_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_5_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_6_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_9_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_7_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_8_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_10_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_11_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_12_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_13_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_14_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_15_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_data_mask_r_0_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_data_mask_r_1_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_rising_0_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_rising_1_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_rising_2_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_rising_3_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_rising_4_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_rising_7_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_rising_5_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_rising_6_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_rising_8_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_rising_9_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_rising_10_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_rising_11_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_rising_12_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_rising_13_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_rising_14_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal NLW_Mshreg_write_data_rising_15_SRL16E_Q15_UNCONNECTED : STD_LOGIC; 
  signal write_data4 : STD_LOGIC_VECTOR ( 15 downto 0 ); 
  signal write_data3 : STD_LOGIC_VECTOR ( 31 downto 0 ); 
  signal write_data_m4 : STD_LOGIC_VECTOR ( 1 downto 0 ); 
  signal write_data_m3 : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  write_en_P1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_enable,
      SRST => reset90_r,
      CLK => clk90,
      O => write_en_P1_23,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  write_en_val_22 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_en_P1_23,
      SRST => reset90_r,
      CLK => NlwInverterSignal_mem_interface_top_inst_ddr2_top0_data_path0_data_write0_write_en_val_C,
      O => write_en_val,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  write_data_mo31 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(31),
      CLK => clk90,
      O => NLW_write_data_mo31_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo30 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(30),
      CLK => clk90,
      O => NLW_write_data_mo30_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo29 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(29),
      CLK => clk90,
      O => NLW_write_data_mo29_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo28 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(28),
      CLK => clk90,
      O => NLW_write_data_mo28_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo27 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(27),
      CLK => clk90,
      O => NLW_write_data_mo27_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo26 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(26),
      CLK => clk90,
      O => NLW_write_data_mo26_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo25 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(25),
      CLK => clk90,
      O => NLW_write_data_mo25_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo24 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(24),
      CLK => clk90,
      O => NLW_write_data_mo24_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo23 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(23),
      CLK => clk90,
      O => NLW_write_data_mo23_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo22 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(22),
      CLK => clk90,
      O => NLW_write_data_mo22_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo21 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(21),
      CLK => clk90,
      O => NLW_write_data_mo21_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo20 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(20),
      CLK => clk90,
      O => NLW_write_data_mo20_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo19 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(19),
      CLK => clk90,
      O => NLW_write_data_mo19_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo18 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(18),
      CLK => clk90,
      O => NLW_write_data_mo18_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo17 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(17),
      CLK => clk90,
      O => NLW_write_data_mo17_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo16 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(16),
      CLK => clk90,
      O => NLW_write_data_mo16_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo15 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(15),
      CLK => clk90,
      O => write_data4(15),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo14 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(14),
      CLK => clk90,
      O => write_data4(14),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo13 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(13),
      CLK => clk90,
      O => write_data4(13),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo12 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(12),
      CLK => clk90,
      O => write_data4(12),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo11 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(11),
      CLK => clk90,
      O => write_data4(11),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo10 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(10),
      CLK => clk90,
      O => write_data4(10),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo9 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(9),
      CLK => clk90,
      O => write_data4(9),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo8 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(8),
      CLK => clk90,
      O => write_data4(8),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo7 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(7),
      CLK => clk90,
      O => write_data4(7),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo6 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(6),
      CLK => clk90,
      O => write_data4(6),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo5 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(5),
      CLK => clk90,
      O => write_data4(5),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo4 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(4),
      CLK => clk90,
      O => write_data4(4),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(3),
      CLK => clk90,
      O => write_data4(3),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(2),
      CLK => clk90,
      O => write_data4(2),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(1),
      CLK => clk90,
      O => write_data4(1),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mo0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data3(0),
      CLK => clk90,
      O => write_data4(0),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mas3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data_m3(3),
      CLK => clk90,
      O => NLW_write_data_mas3_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mas2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data_m3(2),
      CLK => clk90,
      O => NLW_write_data_mas2_O_UNCONNECTED,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mas1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data_m3(1),
      CLK => clk90,
      O => write_data_m4(1),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mas0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_data_m3(0),
      CLK => clk90,
      O => write_data_m4(0),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  XST_VCC : X_ONE
    port map (
      O => N2
    );
  XST_GND : X_ZERO
    port map (
      O => N3
    );
  write_data3_2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_2,
      CLK => clk90,
      O => write_data3(2),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_0,
      CLK => clk90,
      O => write_data3(0),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_1,
      CLK => clk90,
      O => write_data3(1),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_3,
      CLK => clk90,
      O => write_data3(3),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_4 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_4,
      CLK => clk90,
      O => write_data3(4),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_5 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_5,
      CLK => clk90,
      O => write_data3(5),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_6 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_6,
      CLK => clk90,
      O => write_data3(6),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_7 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_7,
      CLK => clk90,
      O => write_data3(7),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_8 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_8,
      CLK => clk90,
      O => write_data3(8),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_11 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_11,
      CLK => clk90,
      O => write_data3(11),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_9 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_9,
      CLK => clk90,
      O => write_data3(9),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_10 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_10,
      CLK => clk90,
      O => write_data3(10),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_12 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_12,
      CLK => clk90,
      O => write_data3(12),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_13 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_13,
      CLK => clk90,
      O => write_data3(13),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_14 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_14,
      CLK => clk90,
      O => write_data3(14),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_15 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_15,
      CLK => clk90,
      O => write_data3(15),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_16 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_16,
      CLK => clk90,
      O => write_data3(16),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_17 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_17,
      CLK => clk90,
      O => write_data3(17),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_20 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_20,
      CLK => clk90,
      O => write_data3(20),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_18 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_18,
      CLK => clk90,
      O => write_data3(18),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_19 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_19,
      CLK => clk90,
      O => write_data3(19),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_21 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_21,
      CLK => clk90,
      O => write_data3(21),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_22 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_22,
      CLK => clk90,
      O => write_data3(22),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_23 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_23,
      CLK => clk90,
      O => write_data3(23),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_24 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_24,
      CLK => clk90,
      O => write_data3(24),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_25 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_25,
      CLK => clk90,
      O => write_data3(25),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_26 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_26,
      CLK => clk90,
      O => write_data3(26),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_29 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_29,
      CLK => clk90,
      O => write_data3(29),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_27 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_27,
      CLK => clk90,
      O => write_data3(27),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_28 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_28,
      CLK => clk90,
      O => write_data3(28),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_30 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_30,
      CLK => clk90,
      O => write_data3(30),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data3_31 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data3_31,
      CLK => clk90,
      O => write_data3(31),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_m3_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_m3_0,
      CLK => clk90,
      O => write_data_m3(0),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_m3_1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_m3_1,
      CLK => clk90,
      O => write_data_m3(1),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_m3_2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_m3_2,
      CLK => clk90,
      O => write_data_m3(2),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_m3_3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_m3_3,
      CLK => clk90,
      O => write_data_m3(3),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mask_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_mask_0,
      CLK => clk90,
      O => data_mask_f(0),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_mask_1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_mask_1,
      CLK => clk90,
      O => data_mask_f(1),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_0,
      CLK => clk90,
      O => write_data_falling(0),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_1,
      CLK => clk90,
      O => write_data_falling(1),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_2,
      CLK => clk90,
      O => write_data_falling(2),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_3,
      CLK => clk90,
      O => write_data_falling(3),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_4 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_4,
      CLK => clk90,
      O => write_data_falling(4),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_5 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_5,
      CLK => clk90,
      O => write_data_falling(5),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_6 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_6,
      CLK => clk90,
      O => write_data_falling(6),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_9 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_9,
      CLK => clk90,
      O => write_data_falling(9),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_7 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_7,
      CLK => clk90,
      O => write_data_falling(7),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_8 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_8,
      CLK => clk90,
      O => write_data_falling(8),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_10 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_10,
      CLK => clk90,
      O => write_data_falling(10),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_11 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_11,
      CLK => clk90,
      O => write_data_falling(11),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_12 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_12,
      CLK => clk90,
      O => write_data_falling(12),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_13 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_13,
      CLK => clk90,
      O => write_data_falling(13),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_14 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_14,
      CLK => clk90,
      O => write_data_falling(14),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_15 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_15,
      CLK => clk90,
      O => write_data_falling(15),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  data_mask_r_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_data_mask_r_0,
      CLK => clk270,
      O => data_mask_r(0),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  data_mask_r_1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_data_mask_r_1,
      CLK => clk270,
      O => data_mask_r(1),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_rising_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_rising_0,
      CLK => clk270,
      O => write_data_rising(0),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_rising_1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_rising_1,
      CLK => clk270,
      O => write_data_rising(1),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_rising_2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_rising_2,
      CLK => clk270,
      O => write_data_rising(2),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_rising_3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_rising_3,
      CLK => clk270,
      O => write_data_rising(3),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_rising_4 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_rising_4,
      CLK => clk270,
      O => write_data_rising(4),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_rising_7 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_rising_7,
      CLK => clk270,
      O => write_data_rising(7),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_rising_5 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_rising_5,
      CLK => clk270,
      O => write_data_rising(5),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_rising_6 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_rising_6,
      CLK => clk270,
      O => write_data_rising(6),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_rising_8 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_rising_8,
      CLK => clk270,
      O => write_data_rising(8),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_rising_9 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_rising_9,
      CLK => clk270,
      O => write_data_rising(9),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_rising_10 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_rising_10,
      CLK => clk270,
      O => write_data_rising(10),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_rising_11 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_rising_11,
      CLK => clk270,
      O => write_data_rising(11),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_rising_12 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_rising_12,
      CLK => clk270,
      O => write_data_rising(12),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_rising_13 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_rising_13,
      CLK => clk270,
      O => write_data_rising(13),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_rising_14 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_rising_14,
      CLK => clk270,
      O => write_data_rising(14),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  write_data_rising_15 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => Mshreg_write_data_rising_15,
      CLK => clk270,
      O => write_data_rising(15),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  Mshreg_write_data3_2_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_2_CE
    );
  Mshreg_write_data3_2_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(2),
      CE => Mshreg_write_data3_2_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_2,
      Q15 => NLW_Mshreg_write_data3_2_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_0_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_0_CE
    );
  Mshreg_write_data3_0_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(0),
      CE => Mshreg_write_data3_0_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_0,
      Q15 => NLW_Mshreg_write_data3_0_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_1_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_1_CE
    );
  Mshreg_write_data3_1_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(1),
      CE => Mshreg_write_data3_1_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_1,
      Q15 => NLW_Mshreg_write_data3_1_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_3_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_3_CE
    );
  Mshreg_write_data3_3_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(3),
      CE => Mshreg_write_data3_3_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_3,
      Q15 => NLW_Mshreg_write_data3_3_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_4_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_4_CE
    );
  Mshreg_write_data3_4_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(4),
      CE => Mshreg_write_data3_4_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_4,
      Q15 => NLW_Mshreg_write_data3_4_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_5_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_5_CE
    );
  Mshreg_write_data3_5_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(5),
      CE => Mshreg_write_data3_5_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_5,
      Q15 => NLW_Mshreg_write_data3_5_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_6_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_6_CE
    );
  Mshreg_write_data3_6_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(6),
      CE => Mshreg_write_data3_6_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_6,
      Q15 => NLW_Mshreg_write_data3_6_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_7_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_7_CE
    );
  Mshreg_write_data3_7_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(7),
      CE => Mshreg_write_data3_7_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_7,
      Q15 => NLW_Mshreg_write_data3_7_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_8_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_8_CE
    );
  Mshreg_write_data3_8_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(8),
      CE => Mshreg_write_data3_8_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_8,
      Q15 => NLW_Mshreg_write_data3_8_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_11_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_11_CE
    );
  Mshreg_write_data3_11_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(11),
      CE => Mshreg_write_data3_11_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_11,
      Q15 => NLW_Mshreg_write_data3_11_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_9_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_9_CE
    );
  Mshreg_write_data3_9_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(9),
      CE => Mshreg_write_data3_9_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_9,
      Q15 => NLW_Mshreg_write_data3_9_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_10_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_10_CE
    );
  Mshreg_write_data3_10_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(10),
      CE => Mshreg_write_data3_10_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_10,
      Q15 => NLW_Mshreg_write_data3_10_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_12_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_12_CE
    );
  Mshreg_write_data3_12_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(12),
      CE => Mshreg_write_data3_12_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_12,
      Q15 => NLW_Mshreg_write_data3_12_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_13_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_13_CE
    );
  Mshreg_write_data3_13_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(13),
      CE => Mshreg_write_data3_13_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_13,
      Q15 => NLW_Mshreg_write_data3_13_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_14_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_14_CE
    );
  Mshreg_write_data3_14_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(14),
      CE => Mshreg_write_data3_14_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_14,
      Q15 => NLW_Mshreg_write_data3_14_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_15_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_15_CE
    );
  Mshreg_write_data3_15_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(15),
      CE => Mshreg_write_data3_15_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_15,
      Q15 => NLW_Mshreg_write_data3_15_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_16_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_16_CE
    );
  Mshreg_write_data3_16_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(16),
      CE => Mshreg_write_data3_16_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_16,
      Q15 => NLW_Mshreg_write_data3_16_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_17_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_17_CE
    );
  Mshreg_write_data3_17_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(17),
      CE => Mshreg_write_data3_17_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_17,
      Q15 => NLW_Mshreg_write_data3_17_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_20_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_20_CE
    );
  Mshreg_write_data3_20_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(20),
      CE => Mshreg_write_data3_20_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_20,
      Q15 => NLW_Mshreg_write_data3_20_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_18_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_18_CE
    );
  Mshreg_write_data3_18_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(18),
      CE => Mshreg_write_data3_18_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_18,
      Q15 => NLW_Mshreg_write_data3_18_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_19_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_19_CE
    );
  Mshreg_write_data3_19_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(19),
      CE => Mshreg_write_data3_19_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_19,
      Q15 => NLW_Mshreg_write_data3_19_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_21_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_21_CE
    );
  Mshreg_write_data3_21_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(21),
      CE => Mshreg_write_data3_21_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_21,
      Q15 => NLW_Mshreg_write_data3_21_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_22_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_22_CE
    );
  Mshreg_write_data3_22_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(22),
      CE => Mshreg_write_data3_22_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_22,
      Q15 => NLW_Mshreg_write_data3_22_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_23_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_23_CE
    );
  Mshreg_write_data3_23_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(23),
      CE => Mshreg_write_data3_23_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_23,
      Q15 => NLW_Mshreg_write_data3_23_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_24_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_24_CE
    );
  Mshreg_write_data3_24_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(24),
      CE => Mshreg_write_data3_24_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_24,
      Q15 => NLW_Mshreg_write_data3_24_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_25_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_25_CE
    );
  Mshreg_write_data3_25_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(25),
      CE => Mshreg_write_data3_25_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_25,
      Q15 => NLW_Mshreg_write_data3_25_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_26_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_26_CE
    );
  Mshreg_write_data3_26_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(26),
      CE => Mshreg_write_data3_26_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_26,
      Q15 => NLW_Mshreg_write_data3_26_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_29_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_29_CE
    );
  Mshreg_write_data3_29_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(29),
      CE => Mshreg_write_data3_29_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_29,
      Q15 => NLW_Mshreg_write_data3_29_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_27_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_27_CE
    );
  Mshreg_write_data3_27_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(27),
      CE => Mshreg_write_data3_27_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_27,
      Q15 => NLW_Mshreg_write_data3_27_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_28_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_28_CE
    );
  Mshreg_write_data3_28_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(28),
      CE => Mshreg_write_data3_28_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_28,
      Q15 => NLW_Mshreg_write_data3_28_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_30_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_30_CE
    );
  Mshreg_write_data3_30_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(30),
      CE => Mshreg_write_data3_30_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_30,
      Q15 => NLW_Mshreg_write_data3_30_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data3_31_VCC : X_ONE
    port map (
      O => Mshreg_write_data3_31_CE
    );
  Mshreg_write_data3_31_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_input_data(31),
      CE => Mshreg_write_data3_31_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data3_31,
      Q15 => NLW_Mshreg_write_data3_31_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_m3_0_VCC : X_ONE
    port map (
      O => Mshreg_write_data_m3_0_CE
    );
  Mshreg_write_data_m3_0_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_data_mask(0),
      CE => Mshreg_write_data_m3_0_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data_m3_0,
      Q15 => NLW_Mshreg_write_data_m3_0_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_m3_1_VCC : X_ONE
    port map (
      O => Mshreg_write_data_m3_1_CE
    );
  Mshreg_write_data_m3_1_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_data_mask(1),
      CE => Mshreg_write_data_m3_1_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data_m3_1,
      Q15 => NLW_Mshreg_write_data_m3_1_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_m3_2_VCC : X_ONE
    port map (
      O => Mshreg_write_data_m3_2_CE
    );
  Mshreg_write_data_m3_2_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_data_mask(2),
      CE => Mshreg_write_data_m3_2_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data_m3_2,
      Q15 => NLW_Mshreg_write_data_m3_2_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_m3_3_VCC : X_ONE
    port map (
      O => Mshreg_write_data_m3_3_CE
    );
  Mshreg_write_data_m3_3_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => user_data_mask(3),
      CE => Mshreg_write_data_m3_3_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N2,
      A0 => N3,
      Q => Mshreg_write_data_m3_3,
      Q15 => NLW_Mshreg_write_data_m3_3_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_mask_0_VCC : X_ONE
    port map (
      O => Mshreg_write_data_mask_0_CE
    );
  Mshreg_write_data_mask_0_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data_m4(0),
      CE => Mshreg_write_data_mask_0_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_mask_0,
      Q15 => NLW_Mshreg_write_data_mask_0_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_mask_1_VCC : X_ONE
    port map (
      O => Mshreg_write_data_mask_1_CE
    );
  Mshreg_write_data_mask_1_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data_m4(1),
      CE => Mshreg_write_data_mask_1_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_mask_1,
      Q15 => NLW_Mshreg_write_data_mask_1_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_0_VCC : X_ONE
    port map (
      O => Mshreg_write_data_0_CE
    );
  Mshreg_write_data_0_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data4(0),
      CE => Mshreg_write_data_0_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_0,
      Q15 => NLW_Mshreg_write_data_0_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_1_VCC : X_ONE
    port map (
      O => Mshreg_write_data_1_CE
    );
  Mshreg_write_data_1_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data4(1),
      CE => Mshreg_write_data_1_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_1,
      Q15 => NLW_Mshreg_write_data_1_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_2_VCC : X_ONE
    port map (
      O => Mshreg_write_data_2_CE
    );
  Mshreg_write_data_2_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data4(2),
      CE => Mshreg_write_data_2_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_2,
      Q15 => NLW_Mshreg_write_data_2_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_3_VCC : X_ONE
    port map (
      O => Mshreg_write_data_3_CE
    );
  Mshreg_write_data_3_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data4(3),
      CE => Mshreg_write_data_3_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_3,
      Q15 => NLW_Mshreg_write_data_3_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_4_VCC : X_ONE
    port map (
      O => Mshreg_write_data_4_CE
    );
  Mshreg_write_data_4_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data4(4),
      CE => Mshreg_write_data_4_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_4,
      Q15 => NLW_Mshreg_write_data_4_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_5_VCC : X_ONE
    port map (
      O => Mshreg_write_data_5_CE
    );
  Mshreg_write_data_5_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data4(5),
      CE => Mshreg_write_data_5_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_5,
      Q15 => NLW_Mshreg_write_data_5_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_6_VCC : X_ONE
    port map (
      O => Mshreg_write_data_6_CE
    );
  Mshreg_write_data_6_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data4(6),
      CE => Mshreg_write_data_6_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_6,
      Q15 => NLW_Mshreg_write_data_6_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_9_VCC : X_ONE
    port map (
      O => Mshreg_write_data_9_CE
    );
  Mshreg_write_data_9_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data4(9),
      CE => Mshreg_write_data_9_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_9,
      Q15 => NLW_Mshreg_write_data_9_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_7_VCC : X_ONE
    port map (
      O => Mshreg_write_data_7_CE
    );
  Mshreg_write_data_7_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data4(7),
      CE => Mshreg_write_data_7_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_7,
      Q15 => NLW_Mshreg_write_data_7_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_8_VCC : X_ONE
    port map (
      O => Mshreg_write_data_8_CE
    );
  Mshreg_write_data_8_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data4(8),
      CE => Mshreg_write_data_8_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_8,
      Q15 => NLW_Mshreg_write_data_8_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_10_VCC : X_ONE
    port map (
      O => Mshreg_write_data_10_CE
    );
  Mshreg_write_data_10_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data4(10),
      CE => Mshreg_write_data_10_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_10,
      Q15 => NLW_Mshreg_write_data_10_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_11_VCC : X_ONE
    port map (
      O => Mshreg_write_data_11_CE
    );
  Mshreg_write_data_11_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data4(11),
      CE => Mshreg_write_data_11_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_11,
      Q15 => NLW_Mshreg_write_data_11_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_12_VCC : X_ONE
    port map (
      O => Mshreg_write_data_12_CE
    );
  Mshreg_write_data_12_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data4(12),
      CE => Mshreg_write_data_12_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_12,
      Q15 => NLW_Mshreg_write_data_12_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_13_VCC : X_ONE
    port map (
      O => Mshreg_write_data_13_CE
    );
  Mshreg_write_data_13_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data4(13),
      CE => Mshreg_write_data_13_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_13,
      Q15 => NLW_Mshreg_write_data_13_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_14_VCC : X_ONE
    port map (
      O => Mshreg_write_data_14_CE
    );
  Mshreg_write_data_14_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data4(14),
      CE => Mshreg_write_data_14_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_14,
      Q15 => NLW_Mshreg_write_data_14_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_15_VCC : X_ONE
    port map (
      O => Mshreg_write_data_15_CE
    );
  Mshreg_write_data_15_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data4(15),
      CE => Mshreg_write_data_15_CE,
      CLK => clk90,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N3,
      Q => Mshreg_write_data_15,
      Q15 => NLW_Mshreg_write_data_15_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_data_mask_r_0_VCC : X_ONE
    port map (
      O => Mshreg_data_mask_r_0_CE
    );
  Mshreg_data_mask_r_0_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data_m3(2),
      CE => Mshreg_data_mask_r_0_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_data_mask_r_0,
      Q15 => NLW_Mshreg_data_mask_r_0_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_data_mask_r_1_VCC : X_ONE
    port map (
      O => Mshreg_data_mask_r_1_CE
    );
  Mshreg_data_mask_r_1_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data_m3(3),
      CE => Mshreg_data_mask_r_1_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_data_mask_r_1,
      Q15 => NLW_Mshreg_data_mask_r_1_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_rising_0_VCC : X_ONE
    port map (
      O => Mshreg_write_data_rising_0_CE
    );
  Mshreg_write_data_rising_0_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data3(16),
      CE => Mshreg_write_data_rising_0_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_write_data_rising_0,
      Q15 => NLW_Mshreg_write_data_rising_0_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_rising_1_VCC : X_ONE
    port map (
      O => Mshreg_write_data_rising_1_CE
    );
  Mshreg_write_data_rising_1_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data3(17),
      CE => Mshreg_write_data_rising_1_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_write_data_rising_1,
      Q15 => NLW_Mshreg_write_data_rising_1_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_rising_2_VCC : X_ONE
    port map (
      O => Mshreg_write_data_rising_2_CE
    );
  Mshreg_write_data_rising_2_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data3(18),
      CE => Mshreg_write_data_rising_2_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_write_data_rising_2,
      Q15 => NLW_Mshreg_write_data_rising_2_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_rising_3_VCC : X_ONE
    port map (
      O => Mshreg_write_data_rising_3_CE
    );
  Mshreg_write_data_rising_3_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data3(19),
      CE => Mshreg_write_data_rising_3_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_write_data_rising_3,
      Q15 => NLW_Mshreg_write_data_rising_3_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_rising_4_VCC : X_ONE
    port map (
      O => Mshreg_write_data_rising_4_CE
    );
  Mshreg_write_data_rising_4_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data3(20),
      CE => Mshreg_write_data_rising_4_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_write_data_rising_4,
      Q15 => NLW_Mshreg_write_data_rising_4_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_rising_7_VCC : X_ONE
    port map (
      O => Mshreg_write_data_rising_7_CE
    );
  Mshreg_write_data_rising_7_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data3(23),
      CE => Mshreg_write_data_rising_7_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_write_data_rising_7,
      Q15 => NLW_Mshreg_write_data_rising_7_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_rising_5_VCC : X_ONE
    port map (
      O => Mshreg_write_data_rising_5_CE
    );
  Mshreg_write_data_rising_5_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data3(21),
      CE => Mshreg_write_data_rising_5_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_write_data_rising_5,
      Q15 => NLW_Mshreg_write_data_rising_5_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_rising_6_VCC : X_ONE
    port map (
      O => Mshreg_write_data_rising_6_CE
    );
  Mshreg_write_data_rising_6_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data3(22),
      CE => Mshreg_write_data_rising_6_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_write_data_rising_6,
      Q15 => NLW_Mshreg_write_data_rising_6_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_rising_8_VCC : X_ONE
    port map (
      O => Mshreg_write_data_rising_8_CE
    );
  Mshreg_write_data_rising_8_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data3(24),
      CE => Mshreg_write_data_rising_8_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_write_data_rising_8,
      Q15 => NLW_Mshreg_write_data_rising_8_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_rising_9_VCC : X_ONE
    port map (
      O => Mshreg_write_data_rising_9_CE
    );
  Mshreg_write_data_rising_9_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data3(25),
      CE => Mshreg_write_data_rising_9_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_write_data_rising_9,
      Q15 => NLW_Mshreg_write_data_rising_9_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_rising_10_VCC : X_ONE
    port map (
      O => Mshreg_write_data_rising_10_CE
    );
  Mshreg_write_data_rising_10_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data3(26),
      CE => Mshreg_write_data_rising_10_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_write_data_rising_10,
      Q15 => NLW_Mshreg_write_data_rising_10_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_rising_11_VCC : X_ONE
    port map (
      O => Mshreg_write_data_rising_11_CE
    );
  Mshreg_write_data_rising_11_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data3(27),
      CE => Mshreg_write_data_rising_11_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_write_data_rising_11,
      Q15 => NLW_Mshreg_write_data_rising_11_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_rising_12_VCC : X_ONE
    port map (
      O => Mshreg_write_data_rising_12_CE
    );
  Mshreg_write_data_rising_12_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data3(28),
      CE => Mshreg_write_data_rising_12_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_write_data_rising_12,
      Q15 => NLW_Mshreg_write_data_rising_12_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_rising_13_VCC : X_ONE
    port map (
      O => Mshreg_write_data_rising_13_CE
    );
  Mshreg_write_data_rising_13_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data3(29),
      CE => Mshreg_write_data_rising_13_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_write_data_rising_13,
      Q15 => NLW_Mshreg_write_data_rising_13_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_rising_14_VCC : X_ONE
    port map (
      O => Mshreg_write_data_rising_14_CE
    );
  Mshreg_write_data_rising_14_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data3(30),
      CE => Mshreg_write_data_rising_14_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_write_data_rising_14,
      Q15 => NLW_Mshreg_write_data_rising_14_SRL16E_Q15_UNCONNECTED
    );
  Mshreg_write_data_rising_15_VCC : X_ONE
    port map (
      O => Mshreg_write_data_rising_15_CE
    );
  Mshreg_write_data_rising_15_SRL16E : X_SRLC16E
    generic map(
      INIT => X"0000"
    )
    port map (
      D => write_data3(31),
      CE => Mshreg_write_data_rising_15_CE,
      CLK => clk270,
      A3 => N3,
      A2 => N3,
      A1 => N3,
      A0 => N2,
      Q => Mshreg_write_data_rising_15,
      Q15 => NLW_Mshreg_write_data_rising_15_SRL16E_Q15_UNCONNECTED
    );
  NlwBlock_data_write0_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlock_data_write0_GND : X_ZERO
    port map (
      O => GND
    );
  NlwInverterBlock_mem_interface_top_inst_ddr2_top0_data_path0_data_write0_write_en_val_C : X_INV
    port map (
      I => clk90,
      O => NlwInverterSignal_mem_interface_top_inst_ddr2_top0_data_path0_data_write0_write_en_val_C
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal B0_SPO : STD_LOGIC; 
  signal B1_SPO : STD_LOGIC; 
  signal B2_SPO : STD_LOGIC; 
  signal B3_SPO : STD_LOGIC; 
  signal B4_SPO : STD_LOGIC; 
  signal B5_SPO : STD_LOGIC; 
  signal B6_SPO : STD_LOGIC; 
  signal B7_SPO : STD_LOGIC; 
begin
  B0_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(0),
      CLK => WCLK,
      WE => WE,
      O => DPO(0)
    );
  B0_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(0),
      CLK => WCLK,
      WE => WE,
      O => B0_SPO
    );
  B1_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(1),
      CLK => WCLK,
      WE => WE,
      O => DPO(1)
    );
  B1_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(1),
      CLK => WCLK,
      WE => WE,
      O => B1_SPO
    );
  B2_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(2),
      CLK => WCLK,
      WE => WE,
      O => DPO(2)
    );
  B2_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(2),
      CLK => WCLK,
      WE => WE,
      O => B2_SPO
    );
  B3_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(3),
      CLK => WCLK,
      WE => WE,
      O => DPO(3)
    );
  B3_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(3),
      CLK => WCLK,
      WE => WE,
      O => B3_SPO
    );
  B4_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(4),
      CLK => WCLK,
      WE => WE,
      O => DPO(4)
    );
  B4_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(4),
      CLK => WCLK,
      WE => WE,
      O => B4_SPO
    );
  B5_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(5),
      CLK => WCLK,
      WE => WE,
      O => DPO(5)
    );
  B5_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(5),
      CLK => WCLK,
      WE => WE,
      O => B5_SPO
    );
  B6_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(6),
      CLK => WCLK,
      WE => WE,
      O => DPO(6)
    );
  B6_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(6),
      CLK => WCLK,
      WE => WE,
      O => B6_SPO
    );
  B7_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(7),
      CLK => WCLK,
      WE => WE,
      O => DPO(7)
    );
  B7_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(7),
      CLK => WCLK,
      WE => WE,
      O => B7_SPO
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal B0_SPO : STD_LOGIC; 
  signal B1_SPO : STD_LOGIC; 
  signal B2_SPO : STD_LOGIC; 
  signal B3_SPO : STD_LOGIC; 
  signal B4_SPO : STD_LOGIC; 
  signal B5_SPO : STD_LOGIC; 
  signal B6_SPO : STD_LOGIC; 
  signal B7_SPO : STD_LOGIC; 
begin
  B0_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(0),
      CLK => WCLK,
      WE => WE,
      O => DPO(0)
    );
  B0_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(0),
      CLK => WCLK,
      WE => WE,
      O => B0_SPO
    );
  B1_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(1),
      CLK => WCLK,
      WE => WE,
      O => DPO(1)
    );
  B1_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(1),
      CLK => WCLK,
      WE => WE,
      O => B1_SPO
    );
  B2_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(2),
      CLK => WCLK,
      WE => WE,
      O => DPO(2)
    );
  B2_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(2),
      CLK => WCLK,
      WE => WE,
      O => B2_SPO
    );
  B3_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(3),
      CLK => WCLK,
      WE => WE,
      O => DPO(3)
    );
  B3_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(3),
      CLK => WCLK,
      WE => WE,
      O => B3_SPO
    );
  B4_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(4),
      CLK => WCLK,
      WE => WE,
      O => DPO(4)
    );
  B4_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(4),
      CLK => WCLK,
      WE => WE,
      O => B4_SPO
    );
  B5_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(5),
      CLK => WCLK,
      WE => WE,
      O => DPO(5)
    );
  B5_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(5),
      CLK => WCLK,
      WE => WE,
      O => B5_SPO
    );
  B6_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(6),
      CLK => WCLK,
      WE => WE,
      O => DPO(6)
    );
  B6_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(6),
      CLK => WCLK,
      WE => WE,
      O => B6_SPO
    );
  B7_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(7),
      CLK => WCLK,
      WE => WE,
      O => DPO(7)
    );
  B7_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(7),
      CLK => WCLK,
      WE => WE,
      O => B7_SPO
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal B0_SPO : STD_LOGIC; 
  signal B1_SPO : STD_LOGIC; 
  signal B2_SPO : STD_LOGIC; 
  signal B3_SPO : STD_LOGIC; 
  signal B4_SPO : STD_LOGIC; 
  signal B5_SPO : STD_LOGIC; 
  signal B6_SPO : STD_LOGIC; 
  signal B7_SPO : STD_LOGIC; 
begin
  B0_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(0),
      CLK => WCLK,
      WE => WE,
      O => DPO(0)
    );
  B0_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(0),
      CLK => WCLK,
      WE => WE,
      O => B0_SPO
    );
  B1_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(1),
      CLK => WCLK,
      WE => WE,
      O => DPO(1)
    );
  B1_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(1),
      CLK => WCLK,
      WE => WE,
      O => B1_SPO
    );
  B2_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(2),
      CLK => WCLK,
      WE => WE,
      O => DPO(2)
    );
  B2_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(2),
      CLK => WCLK,
      WE => WE,
      O => B2_SPO
    );
  B3_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(3),
      CLK => WCLK,
      WE => WE,
      O => DPO(3)
    );
  B3_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(3),
      CLK => WCLK,
      WE => WE,
      O => B3_SPO
    );
  B4_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(4),
      CLK => WCLK,
      WE => WE,
      O => DPO(4)
    );
  B4_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(4),
      CLK => WCLK,
      WE => WE,
      O => B4_SPO
    );
  B5_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(5),
      CLK => WCLK,
      WE => WE,
      O => DPO(5)
    );
  B5_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(5),
      CLK => WCLK,
      WE => WE,
      O => B5_SPO
    );
  B6_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(6),
      CLK => WCLK,
      WE => WE,
      O => DPO(6)
    );
  B6_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(6),
      CLK => WCLK,
      WE => WE,
      O => B6_SPO
    );
  B7_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(7),
      CLK => WCLK,
      WE => WE,
      O => DPO(7)
    );
  B7_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(7),
      CLK => WCLK,
      WE => WE,
      O => B7_SPO
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal B0_SPO : STD_LOGIC; 
  signal B1_SPO : STD_LOGIC; 
  signal B2_SPO : STD_LOGIC; 
  signal B3_SPO : STD_LOGIC; 
  signal B4_SPO : STD_LOGIC; 
  signal B5_SPO : STD_LOGIC; 
  signal B6_SPO : STD_LOGIC; 
  signal B7_SPO : STD_LOGIC; 
begin
  B0_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(0),
      CLK => WCLK,
      WE => WE,
      O => DPO(0)
    );
  B0_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(0),
      CLK => WCLK,
      WE => WE,
      O => B0_SPO
    );
  B1_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(1),
      CLK => WCLK,
      WE => WE,
      O => DPO(1)
    );
  B1_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(1),
      CLK => WCLK,
      WE => WE,
      O => B1_SPO
    );
  B2_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(2),
      CLK => WCLK,
      WE => WE,
      O => DPO(2)
    );
  B2_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(2),
      CLK => WCLK,
      WE => WE,
      O => B2_SPO
    );
  B3_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(3),
      CLK => WCLK,
      WE => WE,
      O => DPO(3)
    );
  B3_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(3),
      CLK => WCLK,
      WE => WE,
      O => B3_SPO
    );
  B4_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(4),
      CLK => WCLK,
      WE => WE,
      O => DPO(4)
    );
  B4_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(4),
      CLK => WCLK,
      WE => WE,
      O => B4_SPO
    );
  B5_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(5),
      CLK => WCLK,
      WE => WE,
      O => DPO(5)
    );
  B5_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(5),
      CLK => WCLK,
      WE => WE,
      O => B5_SPO
    );
  B6_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(6),
      CLK => WCLK,
      WE => WE,
      O => DPO(6)
    );
  B6_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(6),
      CLK => WCLK,
      WE => WE,
      O => B6_SPO
    );
  B7_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(7),
      CLK => WCLK,
      WE => WE,
      O => DPO(7)
    );
  B7_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(7),
      CLK => WCLK,
      WE => WE,
      O => B7_SPO
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal B0_SPO : STD_LOGIC; 
  signal B1_SPO : STD_LOGIC; 
  signal B2_SPO : STD_LOGIC; 
  signal B3_SPO : STD_LOGIC; 
  signal B4_SPO : STD_LOGIC; 
  signal B5_SPO : STD_LOGIC; 
  signal B6_SPO : STD_LOGIC; 
  signal B7_SPO : STD_LOGIC; 
begin
  B0_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(0),
      CLK => WCLK,
      WE => WE,
      O => DPO(0)
    );
  B0_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(0),
      CLK => WCLK,
      WE => WE,
      O => B0_SPO
    );
  B1_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(1),
      CLK => WCLK,
      WE => WE,
      O => DPO(1)
    );
  B1_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(1),
      CLK => WCLK,
      WE => WE,
      O => B1_SPO
    );
  B2_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(2),
      CLK => WCLK,
      WE => WE,
      O => DPO(2)
    );
  B2_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(2),
      CLK => WCLK,
      WE => WE,
      O => B2_SPO
    );
  B3_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(3),
      CLK => WCLK,
      WE => WE,
      O => DPO(3)
    );
  B3_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(3),
      CLK => WCLK,
      WE => WE,
      O => B3_SPO
    );
  B4_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(4),
      CLK => WCLK,
      WE => WE,
      O => DPO(4)
    );
  B4_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(4),
      CLK => WCLK,
      WE => WE,
      O => B4_SPO
    );
  B5_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(5),
      CLK => WCLK,
      WE => WE,
      O => DPO(5)
    );
  B5_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(5),
      CLK => WCLK,
      WE => WE,
      O => B5_SPO
    );
  B6_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(6),
      CLK => WCLK,
      WE => WE,
      O => DPO(6)
    );
  B6_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(6),
      CLK => WCLK,
      WE => WE,
      O => B6_SPO
    );
  B7_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(7),
      CLK => WCLK,
      WE => WE,
      O => DPO(7)
    );
  B7_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(7),
      CLK => WCLK,
      WE => WE,
      O => B7_SPO
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal B0_SPO : STD_LOGIC; 
  signal B1_SPO : STD_LOGIC; 
  signal B2_SPO : STD_LOGIC; 
  signal B3_SPO : STD_LOGIC; 
  signal B4_SPO : STD_LOGIC; 
  signal B5_SPO : STD_LOGIC; 
  signal B6_SPO : STD_LOGIC; 
  signal B7_SPO : STD_LOGIC; 
begin
  B0_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(0),
      CLK => WCLK,
      WE => WE,
      O => DPO(0)
    );
  B0_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(0),
      CLK => WCLK,
      WE => WE,
      O => B0_SPO
    );
  B1_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(1),
      CLK => WCLK,
      WE => WE,
      O => DPO(1)
    );
  B1_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(1),
      CLK => WCLK,
      WE => WE,
      O => B1_SPO
    );
  B2_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(2),
      CLK => WCLK,
      WE => WE,
      O => DPO(2)
    );
  B2_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(2),
      CLK => WCLK,
      WE => WE,
      O => B2_SPO
    );
  B3_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(3),
      CLK => WCLK,
      WE => WE,
      O => DPO(3)
    );
  B3_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(3),
      CLK => WCLK,
      WE => WE,
      O => B3_SPO
    );
  B4_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(4),
      CLK => WCLK,
      WE => WE,
      O => DPO(4)
    );
  B4_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(4),
      CLK => WCLK,
      WE => WE,
      O => B4_SPO
    );
  B5_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(5),
      CLK => WCLK,
      WE => WE,
      O => DPO(5)
    );
  B5_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(5),
      CLK => WCLK,
      WE => WE,
      O => B5_SPO
    );
  B6_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(6),
      CLK => WCLK,
      WE => WE,
      O => DPO(6)
    );
  B6_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(6),
      CLK => WCLK,
      WE => WE,
      O => B6_SPO
    );
  B7_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(7),
      CLK => WCLK,
      WE => WE,
      O => DPO(7)
    );
  B7_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(7),
      CLK => WCLK,
      WE => WE,
      O => B7_SPO
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal B0_SPO : STD_LOGIC; 
  signal B1_SPO : STD_LOGIC; 
  signal B2_SPO : STD_LOGIC; 
  signal B3_SPO : STD_LOGIC; 
  signal B4_SPO : STD_LOGIC; 
  signal B5_SPO : STD_LOGIC; 
  signal B6_SPO : STD_LOGIC; 
  signal B7_SPO : STD_LOGIC; 
begin
  B0_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(0),
      CLK => WCLK,
      WE => WE,
      O => DPO(0)
    );
  B0_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(0),
      CLK => WCLK,
      WE => WE,
      O => B0_SPO
    );
  B1_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(1),
      CLK => WCLK,
      WE => WE,
      O => DPO(1)
    );
  B1_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(1),
      CLK => WCLK,
      WE => WE,
      O => B1_SPO
    );
  B2_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(2),
      CLK => WCLK,
      WE => WE,
      O => DPO(2)
    );
  B2_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(2),
      CLK => WCLK,
      WE => WE,
      O => B2_SPO
    );
  B3_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(3),
      CLK => WCLK,
      WE => WE,
      O => DPO(3)
    );
  B3_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(3),
      CLK => WCLK,
      WE => WE,
      O => B3_SPO
    );
  B4_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(4),
      CLK => WCLK,
      WE => WE,
      O => DPO(4)
    );
  B4_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(4),
      CLK => WCLK,
      WE => WE,
      O => B4_SPO
    );
  B5_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(5),
      CLK => WCLK,
      WE => WE,
      O => DPO(5)
    );
  B5_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(5),
      CLK => WCLK,
      WE => WE,
      O => B5_SPO
    );
  B6_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(6),
      CLK => WCLK,
      WE => WE,
      O => DPO(6)
    );
  B6_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(6),
      CLK => WCLK,
      WE => WE,
      O => B6_SPO
    );
  B7_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(7),
      CLK => WCLK,
      WE => WE,
      O => DPO(7)
    );
  B7_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(7),
      CLK => WCLK,
      WE => WE,
      O => B7_SPO
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal B0_SPO : STD_LOGIC; 
  signal B1_SPO : STD_LOGIC; 
  signal B2_SPO : STD_LOGIC; 
  signal B3_SPO : STD_LOGIC; 
  signal B4_SPO : STD_LOGIC; 
  signal B5_SPO : STD_LOGIC; 
  signal B6_SPO : STD_LOGIC; 
  signal B7_SPO : STD_LOGIC; 
begin
  B0_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(0),
      CLK => WCLK,
      WE => WE,
      O => DPO(0)
    );
  B0_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(0),
      CLK => WCLK,
      WE => WE,
      O => B0_SPO
    );
  B1_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(1),
      CLK => WCLK,
      WE => WE,
      O => DPO(1)
    );
  B1_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(1),
      CLK => WCLK,
      WE => WE,
      O => B1_SPO
    );
  B2_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(2),
      CLK => WCLK,
      WE => WE,
      O => DPO(2)
    );
  B2_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(2),
      CLK => WCLK,
      WE => WE,
      O => B2_SPO
    );
  B3_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(3),
      CLK => WCLK,
      WE => WE,
      O => DPO(3)
    );
  B3_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(3),
      CLK => WCLK,
      WE => WE,
      O => B3_SPO
    );
  B4_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(4),
      CLK => WCLK,
      WE => WE,
      O => DPO(4)
    );
  B4_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(4),
      CLK => WCLK,
      WE => WE,
      O => B4_SPO
    );
  B5_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(5),
      CLK => WCLK,
      WE => WE,
      O => DPO(5)
    );
  B5_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(5),
      CLK => WCLK,
      WE => WE,
      O => B5_SPO
    );
  B6_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(6),
      CLK => WCLK,
      WE => WE,
      O => DPO(6)
    );
  B6_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(6),
      CLK => WCLK,
      WE => WE,
      O => B6_SPO
    );
  B7_X_RAMD16 : X_RAMD16
    generic map(
      INIT => X"0000"
    )
    port map (
      RADR0 => DPRA0,
      RADR1 => DPRA1,
      RADR2 => DPRA2,
      RADR3 => DPRA3,
      WADR0 => A0,
      WADR1 => A1,
      WADR2 => A2,
      WADR3 => A3,
      I => D(7),
      CLK => WCLK,
      WE => WE,
      O => DPO(7)
    );
  B7_X_RAMS16 : X_RAMS16
    generic map(
      INIT => X"0000"
    )
    port map (
      ADR0 => A0,
      ADR1 => A1,
      ADR2 => A2,
      ADR3 => A3,
      I => D(7),
      CLK => WCLK,
      WE => WE,
      O => B7_SPO
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div2n,
      RST => reset,
      CLK => dqs1,
      O => fbit(0),
      SET => GND
    );
  fbit1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqsn,
      O => fbit(1),
      SET => GND
    );
  fbit2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqs1,
      O => fbit(2),
      SET => GND
    );
  fbit3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_1,
      RST => reset,
      CLK => dqsn,
      O => fbit(3),
      SET => GND
    );
  dqs_div2n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs_div_2,
      O => dqs_div2n,
      ADR1 => GND
    );
  dqsn1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqsn,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqbit0_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div2n,
      RST => reset,
      CLK => dqs1,
      O => fbit(0),
      SET => GND
    );
  fbit1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqsn,
      O => fbit(1),
      SET => GND
    );
  fbit2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqs1,
      O => fbit(2),
      SET => GND
    );
  fbit3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_1,
      RST => reset,
      CLK => dqsn,
      O => fbit(3),
      SET => GND
    );
  dqs_div2n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs_div_2,
      O => dqs_div2n,
      ADR1 => GND
    );
  dqsn1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqsn,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqbit1_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div2n,
      RST => reset,
      CLK => dqs1,
      O => fbit(0),
      SET => GND
    );
  fbit1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqsn,
      O => fbit(1),
      SET => GND
    );
  fbit2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqs1,
      O => fbit(2),
      SET => GND
    );
  fbit3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_1,
      RST => reset,
      CLK => dqsn,
      O => fbit(3),
      SET => GND
    );
  dqs_div2n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs_div_2,
      O => dqs_div2n,
      ADR1 => GND
    );
  dqsn1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqsn,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqbit2_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div2n,
      RST => reset,
      CLK => dqs1,
      O => fbit(0),
      SET => GND
    );
  fbit1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqsn,
      O => fbit(1),
      SET => GND
    );
  fbit2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqs1,
      O => fbit(2),
      SET => GND
    );
  fbit3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_1,
      RST => reset,
      CLK => dqsn,
      O => fbit(3),
      SET => GND
    );
  dqs_div2n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs_div_2,
      O => dqs_div2n,
      ADR1 => GND
    );
  dqsn1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqsn,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqbit3_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div2n,
      RST => reset,
      CLK => dqs1,
      O => fbit(0),
      SET => GND
    );
  fbit1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqsn,
      O => fbit(1),
      SET => GND
    );
  fbit2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqs1,
      O => fbit(2),
      SET => GND
    );
  fbit3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_1,
      RST => reset,
      CLK => dqsn,
      O => fbit(3),
      SET => GND
    );
  dqs_div2n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs_div_2,
      O => dqs_div2n,
      ADR1 => GND
    );
  dqsn1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqsn,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqbit4_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div2n,
      RST => reset,
      CLK => dqs1,
      O => fbit(0),
      SET => GND
    );
  fbit1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqsn,
      O => fbit(1),
      SET => GND
    );
  fbit2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqs1,
      O => fbit(2),
      SET => GND
    );
  fbit3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_1,
      RST => reset,
      CLK => dqsn,
      O => fbit(3),
      SET => GND
    );
  dqs_div2n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs_div_2,
      O => dqs_div2n,
      ADR1 => GND
    );
  dqsn1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqsn,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqbit5_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div2n,
      RST => reset,
      CLK => dqs1,
      O => fbit(0),
      SET => GND
    );
  fbit1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqsn,
      O => fbit(1),
      SET => GND
    );
  fbit2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqs1,
      O => fbit(2),
      SET => GND
    );
  fbit3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_1,
      RST => reset,
      CLK => dqsn,
      O => fbit(3),
      SET => GND
    );
  dqs_div2n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs_div_2,
      O => dqs_div2n,
      ADR1 => GND
    );
  dqsn1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqsn,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqbit6_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div2n,
      RST => reset,
      CLK => dqs1,
      O => fbit(0),
      SET => GND
    );
  fbit1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqsn,
      O => fbit(1),
      SET => GND
    );
  fbit2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqs1,
      O => fbit(2),
      SET => GND
    );
  fbit3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_1,
      RST => reset,
      CLK => dqsn,
      O => fbit(3),
      SET => GND
    );
  dqs_div2n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs_div_2,
      O => dqs_div2n,
      ADR1 => GND
    );
  dqsn1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqsn,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqbit7_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div2n,
      RST => reset,
      CLK => dqs1,
      O => fbit(0),
      SET => GND
    );
  fbit1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqsn,
      O => fbit(1),
      SET => GND
    );
  fbit2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqs1,
      O => fbit(2),
      SET => GND
    );
  fbit3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_1,
      RST => reset,
      CLK => dqsn,
      O => fbit(3),
      SET => GND
    );
  dqs_div2n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs_div_2,
      O => dqs_div2n,
      ADR1 => GND
    );
  dqsn1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqsn,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqbit8_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div2n,
      RST => reset,
      CLK => dqs1,
      O => fbit(0),
      SET => GND
    );
  fbit1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqsn,
      O => fbit(1),
      SET => GND
    );
  fbit2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqs1,
      O => fbit(2),
      SET => GND
    );
  fbit3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_1,
      RST => reset,
      CLK => dqsn,
      O => fbit(3),
      SET => GND
    );
  dqs_div2n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs_div_2,
      O => dqs_div2n,
      ADR1 => GND
    );
  dqsn1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqsn,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqbit9_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div2n,
      RST => reset,
      CLK => dqs1,
      O => fbit(0),
      SET => GND
    );
  fbit1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqsn,
      O => fbit(1),
      SET => GND
    );
  fbit2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqs1,
      O => fbit(2),
      SET => GND
    );
  fbit3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_1,
      RST => reset,
      CLK => dqsn,
      O => fbit(3),
      SET => GND
    );
  dqs_div2n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs_div_2,
      O => dqs_div2n,
      ADR1 => GND
    );
  dqsn1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqsn,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqbit10_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div2n,
      RST => reset,
      CLK => dqs1,
      O => fbit(0),
      SET => GND
    );
  fbit1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqsn,
      O => fbit(1),
      SET => GND
    );
  fbit2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqs1,
      O => fbit(2),
      SET => GND
    );
  fbit3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_1,
      RST => reset,
      CLK => dqsn,
      O => fbit(3),
      SET => GND
    );
  dqs_div2n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs_div_2,
      O => dqs_div2n,
      ADR1 => GND
    );
  dqsn1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqsn,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqbit11_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div2n,
      RST => reset,
      CLK => dqs1,
      O => fbit(0),
      SET => GND
    );
  fbit1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqsn,
      O => fbit(1),
      SET => GND
    );
  fbit2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqs1,
      O => fbit(2),
      SET => GND
    );
  fbit3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_1,
      RST => reset,
      CLK => dqsn,
      O => fbit(3),
      SET => GND
    );
  dqs_div2n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs_div_2,
      O => dqs_div2n,
      ADR1 => GND
    );
  dqsn1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqsn,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqbit12_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div2n,
      RST => reset,
      CLK => dqs1,
      O => fbit(0),
      SET => GND
    );
  fbit1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqsn,
      O => fbit(1),
      SET => GND
    );
  fbit2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqs1,
      O => fbit(2),
      SET => GND
    );
  fbit3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_1,
      RST => reset,
      CLK => dqsn,
      O => fbit(3),
      SET => GND
    );
  dqs_div2n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs_div_2,
      O => dqs_div2n,
      ADR1 => GND
    );
  dqsn1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqsn,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqbit13_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div2n,
      RST => reset,
      CLK => dqs1,
      O => fbit(0),
      SET => GND
    );
  fbit1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqsn,
      O => fbit(1),
      SET => GND
    );
  fbit2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqs1,
      O => fbit(2),
      SET => GND
    );
  fbit3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_1,
      RST => reset,
      CLK => dqsn,
      O => fbit(3),
      SET => GND
    );
  dqs_div2n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs_div_2,
      O => dqs_div2n,
      ADR1 => GND
    );
  dqsn1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqsn,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqbit14_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal fbit : STD_LOGIC_VECTOR ( 3 downto 0 ); 
begin
  fbit_0 <= fbit(0);
  fbit_1 <= fbit(1);
  fbit_2 <= fbit(2);
  fbit_3 <= fbit(3);
  fbit0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div2n,
      RST => reset,
      CLK => dqs1,
      O => fbit(0),
      SET => GND
    );
  fbit1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqsn,
      O => fbit(1),
      SET => GND
    );
  fbit2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_2,
      RST => reset,
      CLK => dqs1,
      O => fbit(2),
      SET => GND
    );
  fbit3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dq,
      CE => dqs_div_1,
      RST => reset,
      CLK => dqsn,
      O => fbit(3),
      SET => GND
    );
  dqs_div2n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs_div_2,
      O => dqs_div2n,
      ADR1 => GND
    );
  dqsn1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqsn,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqbit15_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal VCC : STD_LOGIC; 
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
  user_output_data_10 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(10),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(10),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_11 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(11),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(11),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_12 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(12),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(12),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_13 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(13),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(13),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_14 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(14),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(14),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_15 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(15),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(15),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_16 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(16),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(16),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_17 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(17),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(17),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_18 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(18),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(18),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_19 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(19),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(19),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_20 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(20),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(20),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_21 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(21),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(21),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_22 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(22),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(22),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_23 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(23),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(23),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_24 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(24),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(24),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_25 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(25),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(25),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_26 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(26),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(26),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_27 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(27),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(27),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_28 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(28),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(28),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_29 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(29),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(29),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_30 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(30),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(30),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  user_output_data_31 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0004(31),
      SRST => reset90_r,
      CE => Q_n0007,
      CLK => clk90,
      O => user_output_data(31),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  next_state : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => N4,
      SRST => reset90_r,
      CLK => clk90,
      O => NlwRenamedSig_OI_next_state_val,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Q_n0004_31_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_10_data_out(7),
      ADR2 => fifo_12_data_out(7),
      O => Q_n0004(31)
    );
  Q_n0004_30_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_10_data_out(6),
      ADR2 => fifo_12_data_out(6),
      O => Q_n0004(30)
    );
  Q_n0004_29_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_10_data_out(5),
      ADR2 => fifo_12_data_out(5),
      O => Q_n0004(29)
    );
  Q_n0004_28_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_10_data_out(4),
      ADR2 => fifo_12_data_out(4),
      O => Q_n0004(28)
    );
  Q_n0004_27_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_10_data_out(3),
      ADR2 => fifo_12_data_out(3),
      O => Q_n0004(27)
    );
  Q_n0004_26_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_10_data_out(2),
      ADR2 => fifo_12_data_out(2),
      O => Q_n0004(26)
    );
  Q_n0004_25_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_10_data_out(1),
      ADR2 => fifo_12_data_out(1),
      O => Q_n0004(25)
    );
  Q_n0004_24_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_10_data_out(0),
      ADR2 => fifo_12_data_out(0),
      O => Q_n0004(24)
    );
  Q_n0004_23_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_00_data_out(7),
      ADR2 => fifo_02_data_out(7),
      O => Q_n0004(23)
    );
  Q_n0004_22_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_00_data_out(6),
      ADR2 => fifo_02_data_out(6),
      O => Q_n0004(22)
    );
  Q_n0004_21_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_00_data_out(5),
      ADR2 => fifo_02_data_out(5),
      O => Q_n0004(21)
    );
  Q_n0004_20_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_00_data_out(4),
      ADR2 => fifo_02_data_out(4),
      O => Q_n0004(20)
    );
  Q_n0004_19_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_00_data_out(3),
      ADR2 => fifo_02_data_out(3),
      O => Q_n0004(19)
    );
  Q_n0004_18_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_00_data_out(2),
      ADR2 => fifo_02_data_out(2),
      O => Q_n0004(18)
    );
  Q_n0004_17_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_00_data_out(1),
      ADR2 => fifo_02_data_out(1),
      O => Q_n0004(17)
    );
  Q_n0004_16_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_00_data_out(0),
      ADR2 => fifo_02_data_out(0),
      O => Q_n0004(16)
    );
  Q_n0004_15_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_11_data_out(7),
      ADR2 => fifo_13_data_out(7),
      O => Q_n0004(15)
    );
  Q_n0004_14_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_11_data_out(6),
      ADR2 => fifo_13_data_out(6),
      O => Q_n0004(14)
    );
  Q_n0004_13_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_11_data_out(5),
      ADR2 => fifo_13_data_out(5),
      O => Q_n0004(13)
    );
  Q_n0004_12_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_11_data_out(4),
      ADR2 => fifo_13_data_out(4),
      O => Q_n0004(12)
    );
  Q_n0004_11_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_11_data_out(3),
      ADR2 => fifo_13_data_out(3),
      O => Q_n0004(11)
    );
  Q_n0004_10_1 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => fifo_11_data_out(2),
      ADR2 => fifo_13_data_out(2),
      O => Q_n0004(10)
    );
  Q_n00071 : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => read_valid_data_1,
      ADR2 => read_valid_data_2,
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
  Mrom_data_Mrom_n0005 : X_LUT3
    generic map(
      INIT => X"4E"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_next_state_val,
      ADR1 => read_valid_data_1,
      ADR2 => read_valid_data_2,
      O => N4
    );
  NlwBlock_data_read0_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlock_data_read0_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal VCC : STD_LOGIC; 
  signal GND : STD_LOGIC; 
begin
  xdone0_clk0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => dqs_div,
      SRST => sync_rst_xdone0_ck0,
      CLK => clk0,
      O => transfer_done0_clk0,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  xdone0_clk90 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => transfer_done0_clk0,
      SRST => sync_rst_clk90,
      CLK => clk90,
      O => transfer_done0_clk90,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  xdone0_clk180 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => dqs_div,
      SRST => sync_rst_xdone0_ck180,
      CLK => clk180,
      O => transfer_done0_clk180,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  xdone0_clk270 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => transfer_done0_clk180,
      SRST => sync_rst_clk270,
      CLK => clk270,
      O => transfer_done0_clk270,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  xdone1_clk90 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => transfer_done0_clk270,
      CLK => clk90,
      O => transfer_done1_clk90,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  xdone1_clk270 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => transfer_done0_clk90,
      CLK => clk270,
      O => transfer_done1_clk270,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  xdone2_clk90 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => transfer_done1_clk270,
      CLK => clk90,
      O => transfer_done2_clk90,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  xdone2_clk270 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => transfer_done1_clk90,
      CLK => clk270,
      O => transfer_done2_clk270,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  xdone3_clk90 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => transfer_done2_clk270,
      CLK => clk90,
      O => transfer_done3_clk90,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  xdone3_clk270 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => transfer_done2_clk90,
      CLK => clk270,
      O => transfer_done3_clk270,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  xdone0 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => transfer_done0_clk90,
      ADR1 => transfer_done0_clk270,
      O => transfer_done0
    );
  xdone1 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => transfer_done1_clk90,
      ADR1 => transfer_done1_clk270,
      O => transfer_done1
    );
  xdone2 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => transfer_done2_clk90,
      ADR1 => transfer_done2_clk270,
      O => transfer_done2
    );
  xdone3 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => transfer_done3_clk90,
      ADR1 => transfer_done3_clk270,
      O => transfer_done3
    );
  xdone0_rst90 : X_LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      ADR0 => reset90,
      ADR1 => transfer_done0_clk270,
      ADR2 => transfer_done0_clk90,
      O => sync_rst_clk90
    );
  xdone0_rst270 : X_LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      ADR0 => reset270,
      ADR1 => transfer_done0_clk270,
      ADR2 => transfer_done0_clk90,
      O => sync_rst_clk270
    );
  sync_rst_xdone0_ck1801 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => reset180,
      ADR1 => transfer_done0_clk180,
      O => sync_rst_xdone0_ck180
    );
  sync_rst_xdone0_ck01 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => reset,
      ADR1 => transfer_done0_clk0,
      O => sync_rst_xdone0_ck0
    );
  NlwBlock_ddr2_transfer_done0_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlock_ddr2_transfer_done0_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal VCC : STD_LOGIC; 
  signal GND : STD_LOGIC; 
begin
  xdone0_clk0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => dqs_div,
      SRST => sync_rst_xdone0_ck0,
      CLK => clk0,
      O => transfer_done0_clk0,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  xdone0_clk90 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => transfer_done0_clk0,
      SRST => sync_rst_clk90,
      CLK => clk90,
      O => transfer_done0_clk90,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  xdone0_clk180 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => dqs_div,
      SRST => sync_rst_xdone0_ck180,
      CLK => clk180,
      O => transfer_done0_clk180,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  xdone0_clk270 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => transfer_done0_clk180,
      SRST => sync_rst_clk270,
      CLK => clk270,
      O => transfer_done0_clk270,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  xdone1_clk90 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => transfer_done0_clk270,
      CLK => clk90,
      O => transfer_done1_clk90,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  xdone1_clk270 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => transfer_done0_clk90,
      CLK => clk270,
      O => transfer_done1_clk270,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  xdone2_clk90 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => transfer_done1_clk270,
      CLK => clk90,
      O => transfer_done2_clk90,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  xdone2_clk270 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => transfer_done1_clk90,
      CLK => clk270,
      O => transfer_done2_clk270,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  xdone3_clk90 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => transfer_done2_clk270,
      CLK => clk90,
      O => transfer_done3_clk90,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  xdone3_clk270 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => transfer_done2_clk90,
      CLK => clk270,
      O => transfer_done3_clk270,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  xdone0 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => transfer_done0_clk90,
      ADR1 => transfer_done0_clk270,
      O => transfer_done0
    );
  xdone1 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => transfer_done1_clk90,
      ADR1 => transfer_done1_clk270,
      O => transfer_done1
    );
  xdone2 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => transfer_done2_clk90,
      ADR1 => transfer_done2_clk270,
      O => transfer_done2
    );
  xdone3 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => transfer_done3_clk90,
      ADR1 => transfer_done3_clk270,
      O => transfer_done3
    );
  xdone0_rst90 : X_LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      ADR0 => reset90,
      ADR1 => transfer_done0_clk270,
      ADR2 => transfer_done0_clk90,
      O => sync_rst_clk90
    );
  xdone0_rst270 : X_LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      ADR0 => reset270,
      ADR1 => transfer_done0_clk270,
      ADR2 => transfer_done0_clk90,
      O => sync_rst_clk270
    );
  sync_rst_xdone0_ck1801 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => reset180,
      ADR1 => transfer_done0_clk180,
      O => sync_rst_xdone0_ck180
    );
  sync_rst_xdone0_ck01 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => reset,
      ADR1 => transfer_done0_clk0,
      O => sync_rst_xdone0_ck0
    );
  NlwBlock_ddr2_transfer_done1_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlock_ddr2_transfer_done1_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal VCC : STD_LOGIC; 
  signal GND : STD_LOGIC; 
begin
  dqs_divp <= NlwRenamedSig_OI_dqs_divp;
  col1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dqs_div0n,
      RST => rst_dqs_div_delayed,
      CLK => dqs1,
      O => NlwRenamedSig_OI_dqs_divp,
      CE => VCC,
      SET => GND
    );
  col0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => NlwRenamedSig_OI_dqs_divp,
      RST => reset,
      CLK => dqs_n,
      O => dqs_divn,
      CE => VCC,
      SET => GND
    );
  dqs_div0n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_dqs_divp,
      O => dqs_div0n,
      ADR1 => GND
    );
  dqs_n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqs_n,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqs_div0_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlock_ddr2_dqs_div0_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal VCC : STD_LOGIC; 
  signal GND : STD_LOGIC; 
begin
  dqs_divp <= NlwRenamedSig_OI_dqs_divp;
  col1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dqs_div0n,
      RST => rst_dqs_div_delayed,
      CLK => dqs1,
      O => NlwRenamedSig_OI_dqs_divp,
      CE => VCC,
      SET => GND
    );
  col0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => NlwRenamedSig_OI_dqs_divp,
      RST => reset,
      CLK => dqs_n,
      O => dqs_divn,
      CE => VCC,
      SET => GND
    );
  dqs_div0n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_dqs_divp,
      O => dqs_div0n,
      ADR1 => GND
    );
  dqs_n1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs,
      O => dqs_n,
      ADR1 => GND
    );
  NlwBlock_ddr2_dqs_div1_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlock_ddr2_dqs_div1_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal fifo_01_not_empty_r_24 : STD_LOGIC; 
  signal rst_dqs_div_delayed : STD_LOGIC; 
  signal Q_n0008 : STD_LOGIC; 
  signal Q_n0018 : STD_LOGIC; 
  signal Q_n0019 : STD_LOGIC; 
  signal Q_n0046 : STD_LOGIC; 
  signal Q_n0047 : STD_LOGIC; 
  signal read_valid_data_1 : STD_LOGIC; 
  signal read_valid_data_2 : STD_LOGIC; 
  signal fifo_01_not_empty_r1_25 : STD_LOGIC; 
  signal rd_data_valid_reg_26 : STD_LOGIC; 
  signal fifo_03_not_empty_r_27 : STD_LOGIC; 
  signal fifo_01_not_empty : STD_LOGIC; 
  signal read_valid_data : STD_LOGIC; 
  signal rst_dqs_div_int : STD_LOGIC; 
  signal fifo_03_not_empty_r1_28 : STD_LOGIC; 
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
  signal VCC : STD_LOGIC; 
  signal GND : STD_LOGIC; 
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
  rd_data_valid : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0008,
      SRST => reset90_r,
      CLK => clk90,
      O => u_data_val,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_03_not_empty_r : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_03_not_empty,
      SRST => reset90_r,
      CLK => clk90,
      O => fifo_03_not_empty_r_27,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  rd_data_valid_reg : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => read_valid_data,
      CE => reset90_r_INV,
      CLK => clk90,
      O => rd_data_valid_reg_26,
      SET => GND,
      RST => GND
    );
  rd_data_valid_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => read_valid_data_1,
      SRST => reset90_r,
      CLK => clk90,
      O => NlwRenamedSig_OI_read_valid_data_1_val,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  rd_data_valid_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => read_valid_data_2,
      SRST => reset90_r,
      CLK => clk90,
      O => NlwRenamedSig_OI_read_valid_data_2_val,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_01_not_empty_r : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_01_not_empty,
      SRST => reset90_r,
      CLK => clk90,
      O => fifo_01_not_empty_r_24,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_03_not_empty_r1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_03_not_empty_r_27,
      SRST => reset90_r,
      CLK => clk90,
      O => fifo_03_not_empty_r1_28,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_01_not_empty_r1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_01_not_empty_r_24,
      SRST => reset90_r,
      CLK => clk90,
      O => fifo_01_not_empty_r1_25,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_11_rd_addr_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_11_rd_addr_n0000(0),
      SRST => reset90_r,
      CE => Q_n0046,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_11_rd_addr_val(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_11_rd_addr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_11_rd_addr_n0000(1),
      SRST => reset90_r,
      CE => Q_n0046,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_11_rd_addr_val(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_11_rd_addr_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_11_rd_addr_n0000(2),
      SRST => reset90_r,
      CE => Q_n0046,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_11_rd_addr_val(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_11_rd_addr_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_11_rd_addr_n0000(3),
      SRST => reset90_r,
      CE => Q_n0046,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_11_rd_addr_val(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_12_rd_addr_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_12_rd_addr_n0000(0),
      SRST => reset90_r,
      CE => Q_n0047,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_12_rd_addr_val(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_12_rd_addr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_12_rd_addr_n0000(1),
      SRST => reset90_r,
      CE => Q_n0047,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_12_rd_addr_val(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_12_rd_addr_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_12_rd_addr_n0000(2),
      SRST => reset90_r,
      CE => Q_n0047,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_12_rd_addr_val(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_12_rd_addr_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_12_rd_addr_n0000(3),
      SRST => reset90_r,
      CE => Q_n0047,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_12_rd_addr_val(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_13_rd_addr_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_13_rd_addr_n0000(0),
      SRST => reset90_r,
      CE => Q_n0047,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_13_rd_addr_val(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_13_rd_addr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_13_rd_addr_n0000(1),
      SRST => reset90_r,
      CE => Q_n0047,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_13_rd_addr_val(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_13_rd_addr_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_13_rd_addr_n0000(2),
      SRST => reset90_r,
      CE => Q_n0047,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_13_rd_addr_val(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_13_rd_addr_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_13_rd_addr_n0000(3),
      SRST => reset90_r,
      CE => Q_n0047,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_13_rd_addr_val(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_00_wr_addr_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_00_wr_addr_n0000(0),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(0),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_00_wr_addr_val(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_00_wr_addr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_00_wr_addr_n0000(1),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(0),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_00_wr_addr_val(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_00_wr_addr_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_00_wr_addr_n0000(2),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(0),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_00_wr_addr_val(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_00_wr_addr_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_00_wr_addr_n0000(3),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(0),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_00_wr_addr_val(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_01_wr_addr_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_01_wr_addr_n0000(0),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(1),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_01_wr_addr_val(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_01_wr_addr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_01_wr_addr_n0000(1),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(1),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_01_wr_addr_val(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_01_wr_addr_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_01_wr_addr_n0000(2),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(1),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_01_wr_addr_val(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_01_wr_addr_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_01_wr_addr_n0000(3),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(1),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_01_wr_addr_val(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_02_wr_addr_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_02_wr_addr_n0000(0),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(2),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_02_wr_addr_val(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_02_wr_addr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_02_wr_addr_n0000(1),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(2),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_02_wr_addr_val(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_02_wr_addr_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_02_wr_addr_n0000(2),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(2),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_02_wr_addr_val(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_02_wr_addr_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_02_wr_addr_n0000(3),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(2),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_02_wr_addr_val(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_03_wr_addr_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_03_wr_addr_n0000(0),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(3),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_03_wr_addr_val(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_03_wr_addr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_03_wr_addr_n0000(1),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(3),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_03_wr_addr_val(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_03_wr_addr_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_03_wr_addr_n0000(2),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(3),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_03_wr_addr_val(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_03_wr_addr_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_03_wr_addr_n0000(3),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_0_val(3),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_03_wr_addr_val(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_10_wr_addr_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_10_wr_addr_n0000(0),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(0),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_10_wr_addr_val(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_10_wr_addr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_10_wr_addr_n0000(1),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(0),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_10_wr_addr_val(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_10_wr_addr_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_10_wr_addr_n0000(2),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(0),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_10_wr_addr_val(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_10_wr_addr_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_10_wr_addr_n0000(3),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(0),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_10_wr_addr_val(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_11_wr_addr_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_11_wr_addr_n0000(0),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(1),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_11_wr_addr_val(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_11_wr_addr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_11_wr_addr_n0000(1),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(1),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_11_wr_addr_val(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_11_wr_addr_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_11_wr_addr_n0000(2),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(1),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_11_wr_addr_val(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_11_wr_addr_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_11_wr_addr_n0000(3),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(1),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_11_wr_addr_val(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_12_wr_addr_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_12_wr_addr_n0000(0),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(2),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_12_wr_addr_val(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_12_wr_addr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_12_wr_addr_n0000(1),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(2),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_12_wr_addr_val(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_12_wr_addr_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_12_wr_addr_n0000(2),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(2),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_12_wr_addr_val(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_12_wr_addr_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_12_wr_addr_n0000(3),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(2),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_12_wr_addr_val(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_13_wr_addr_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_13_wr_addr_n0000(0),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(3),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_13_wr_addr_val(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_13_wr_addr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_13_wr_addr_n0000(1),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(3),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_13_wr_addr_val(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_13_wr_addr_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_13_wr_addr_n0000(2),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(3),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_13_wr_addr_val(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_13_wr_addr_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_13_wr_addr_n0000(3),
      SRST => reset90_r,
      CE => NlwRenamedSig_OI_transfer_done_1_val(3),
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_13_wr_addr_val(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_00_rd_addr_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_00_rd_addr_n0000(0),
      SRST => reset90_r,
      CE => Q_n0046,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_00_rd_addr_val(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_00_rd_addr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_00_rd_addr_n0000(1),
      SRST => reset90_r,
      CE => Q_n0046,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_00_rd_addr_val(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_00_rd_addr_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_00_rd_addr_n0000(2),
      SRST => reset90_r,
      CE => Q_n0046,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_00_rd_addr_val(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_00_rd_addr_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_00_rd_addr_n0000(3),
      SRST => reset90_r,
      CE => Q_n0046,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_00_rd_addr_val(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_01_rd_addr_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_01_rd_addr_n0000(0),
      SRST => reset90_r,
      CE => Q_n0046,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_01_rd_addr_val(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_01_rd_addr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_01_rd_addr_n0000(1),
      SRST => reset90_r,
      CE => Q_n0046,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_01_rd_addr_val(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_01_rd_addr_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_01_rd_addr_n0000(2),
      SRST => reset90_r,
      CE => Q_n0046,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_01_rd_addr_val(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_01_rd_addr_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_01_rd_addr_n0000(3),
      SRST => reset90_r,
      CE => Q_n0046,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_01_rd_addr_val(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_02_rd_addr_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_02_rd_addr_n0000(0),
      SRST => reset90_r,
      CE => Q_n0047,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_02_rd_addr_val(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_02_rd_addr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_02_rd_addr_n0000(1),
      SRST => reset90_r,
      CE => Q_n0047,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_02_rd_addr_val(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_02_rd_addr_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_02_rd_addr_n0000(2),
      SRST => reset90_r,
      CE => Q_n0047,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_02_rd_addr_val(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_02_rd_addr_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_02_rd_addr_n0000(3),
      SRST => reset90_r,
      CE => Q_n0047,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_02_rd_addr_val(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_03_rd_addr_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_03_rd_addr_n0000(0),
      SRST => reset90_r,
      CE => Q_n0047,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_03_rd_addr_val(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_03_rd_addr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_03_rd_addr_n0000(1),
      SRST => reset90_r,
      CE => Q_n0047,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_03_rd_addr_val(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_03_rd_addr_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_03_rd_addr_n0000(2),
      SRST => reset90_r,
      CE => Q_n0047,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_03_rd_addr_val(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_03_rd_addr_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_03_rd_addr_n0000(3),
      SRST => reset90_r,
      CE => Q_n0047,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_03_rd_addr_val(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_10_rd_addr_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_10_rd_addr_n0000(0),
      SRST => reset90_r,
      CE => Q_n0046,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_10_rd_addr_val(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_10_rd_addr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_10_rd_addr_n0000(1),
      SRST => reset90_r,
      CE => Q_n0046,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_10_rd_addr_val(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_10_rd_addr_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_10_rd_addr_n0000(2),
      SRST => reset90_r,
      CE => Q_n0046,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_10_rd_addr_val(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  fifo_10_rd_addr_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => fifo_10_rd_addr_n0000(3),
      SRST => reset90_r,
      CE => Q_n0046,
      CLK => clk90,
      O => NlwRenamedSig_OI_fifo_10_rd_addr_val(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  rst_dqs_div_int1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => rst_dqs_div,
      O => rst_dqs_div_int,
      ADR1 => GND
    );
  Q_n00461 : X_LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      ADR0 => next_state,
      ADR1 => NlwRenamedSig_OI_read_valid_data_1_val,
      O => Q_n0046
    );
  Q_n00471 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => next_state,
      ADR1 => NlwRenamedSig_OI_read_valid_data_2_val,
      O => Q_n0047
    );
  reset90_r_INV1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => reset90_r,
      O => reset90_r_INV,
      ADR1 => GND
    );
  fifo_11_rd_addr_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_11_rd_addr_val(0),
      O => fifo_11_rd_addr_n0000(0),
      ADR1 => GND
    );
  fifo_11_rd_addr_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_11_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_11_rd_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_11_rd_addr_val(1),
      ADR3 => NlwRenamedSig_OI_fifo_11_rd_addr_val(2),
      O => fifo_11_rd_addr_n0000(3)
    );
  fifo_11_rd_addr_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_11_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_11_rd_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_11_rd_addr_val(2),
      O => fifo_11_rd_addr_n0000(2)
    );
  fifo_11_rd_addr_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_11_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_11_rd_addr_val(1),
      O => fifo_11_rd_addr_n0000(1)
    );
  fifo_12_rd_addr_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_12_rd_addr_val(0),
      O => fifo_12_rd_addr_n0000(0),
      ADR1 => GND
    );
  fifo_12_rd_addr_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_12_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_12_rd_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_12_rd_addr_val(1),
      ADR3 => NlwRenamedSig_OI_fifo_12_rd_addr_val(2),
      O => fifo_12_rd_addr_n0000(3)
    );
  fifo_12_rd_addr_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_12_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_12_rd_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_12_rd_addr_val(2),
      O => fifo_12_rd_addr_n0000(2)
    );
  fifo_12_rd_addr_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_12_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_12_rd_addr_val(1),
      O => fifo_12_rd_addr_n0000(1)
    );
  fifo_13_rd_addr_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_13_rd_addr_val(0),
      O => fifo_13_rd_addr_n0000(0),
      ADR1 => GND
    );
  fifo_13_rd_addr_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_13_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_13_rd_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_13_rd_addr_val(1),
      ADR3 => NlwRenamedSig_OI_fifo_13_rd_addr_val(2),
      O => fifo_13_rd_addr_n0000(3)
    );
  fifo_13_rd_addr_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_13_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_13_rd_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_13_rd_addr_val(2),
      O => fifo_13_rd_addr_n0000(2)
    );
  fifo_13_rd_addr_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_13_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_13_rd_addr_val(1),
      O => fifo_13_rd_addr_n0000(1)
    );
  fifo_00_wr_addr_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_00_wr_addr_val(0),
      O => fifo_00_wr_addr_n0000(0),
      ADR1 => GND
    );
  fifo_00_wr_addr_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_00_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_00_wr_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_00_wr_addr_val(1),
      ADR3 => NlwRenamedSig_OI_fifo_00_wr_addr_val(2),
      O => fifo_00_wr_addr_n0000(3)
    );
  fifo_00_wr_addr_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_00_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_00_wr_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_00_wr_addr_val(2),
      O => fifo_00_wr_addr_n0000(2)
    );
  fifo_00_wr_addr_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_00_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_00_wr_addr_val(1),
      O => fifo_00_wr_addr_n0000(1)
    );
  fifo_01_wr_addr_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_01_wr_addr_val(0),
      O => fifo_01_wr_addr_n0000(0),
      ADR1 => GND
    );
  fifo_01_wr_addr_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_01_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_01_wr_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_01_wr_addr_val(1),
      ADR3 => NlwRenamedSig_OI_fifo_01_wr_addr_val(2),
      O => fifo_01_wr_addr_n0000(3)
    );
  fifo_01_wr_addr_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_01_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_01_wr_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_01_wr_addr_val(2),
      O => fifo_01_wr_addr_n0000(2)
    );
  fifo_01_wr_addr_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_01_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_01_wr_addr_val(1),
      O => fifo_01_wr_addr_n0000(1)
    );
  fifo_02_wr_addr_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_02_wr_addr_val(0),
      O => fifo_02_wr_addr_n0000(0),
      ADR1 => GND
    );
  fifo_02_wr_addr_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_02_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_02_wr_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_02_wr_addr_val(1),
      ADR3 => NlwRenamedSig_OI_fifo_02_wr_addr_val(2),
      O => fifo_02_wr_addr_n0000(3)
    );
  fifo_02_wr_addr_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_02_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_02_wr_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_02_wr_addr_val(2),
      O => fifo_02_wr_addr_n0000(2)
    );
  fifo_02_wr_addr_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_02_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_02_wr_addr_val(1),
      O => fifo_02_wr_addr_n0000(1)
    );
  fifo_03_wr_addr_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_03_wr_addr_val(0),
      O => fifo_03_wr_addr_n0000(0),
      ADR1 => GND
    );
  fifo_03_wr_addr_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_03_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_03_wr_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_03_wr_addr_val(1),
      ADR3 => NlwRenamedSig_OI_fifo_03_wr_addr_val(2),
      O => fifo_03_wr_addr_n0000(3)
    );
  fifo_03_wr_addr_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_03_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_03_wr_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_03_wr_addr_val(2),
      O => fifo_03_wr_addr_n0000(2)
    );
  fifo_03_wr_addr_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_03_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_03_wr_addr_val(1),
      O => fifo_03_wr_addr_n0000(1)
    );
  fifo_10_wr_addr_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_10_wr_addr_val(0),
      O => fifo_10_wr_addr_n0000(0),
      ADR1 => GND
    );
  fifo_10_wr_addr_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_10_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_10_wr_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_10_wr_addr_val(1),
      ADR3 => NlwRenamedSig_OI_fifo_10_wr_addr_val(2),
      O => fifo_10_wr_addr_n0000(3)
    );
  fifo_10_wr_addr_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_10_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_10_wr_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_10_wr_addr_val(2),
      O => fifo_10_wr_addr_n0000(2)
    );
  fifo_10_wr_addr_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_10_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_10_wr_addr_val(1),
      O => fifo_10_wr_addr_n0000(1)
    );
  fifo_11_wr_addr_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_11_wr_addr_val(0),
      O => fifo_11_wr_addr_n0000(0),
      ADR1 => GND
    );
  fifo_11_wr_addr_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_11_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_11_wr_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_11_wr_addr_val(1),
      ADR3 => NlwRenamedSig_OI_fifo_11_wr_addr_val(2),
      O => fifo_11_wr_addr_n0000(3)
    );
  fifo_11_wr_addr_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_11_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_11_wr_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_11_wr_addr_val(2),
      O => fifo_11_wr_addr_n0000(2)
    );
  fifo_11_wr_addr_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_11_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_11_wr_addr_val(1),
      O => fifo_11_wr_addr_n0000(1)
    );
  fifo_12_wr_addr_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_12_wr_addr_val(0),
      O => fifo_12_wr_addr_n0000(0),
      ADR1 => GND
    );
  fifo_12_wr_addr_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_12_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_12_wr_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_12_wr_addr_val(1),
      ADR3 => NlwRenamedSig_OI_fifo_12_wr_addr_val(2),
      O => fifo_12_wr_addr_n0000(3)
    );
  fifo_12_wr_addr_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_12_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_12_wr_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_12_wr_addr_val(2),
      O => fifo_12_wr_addr_n0000(2)
    );
  fifo_12_wr_addr_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_12_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_12_wr_addr_val(1),
      O => fifo_12_wr_addr_n0000(1)
    );
  fifo_13_wr_addr_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_13_wr_addr_val(0),
      O => fifo_13_wr_addr_n0000(0),
      ADR1 => GND
    );
  fifo_13_wr_addr_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_13_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_13_wr_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_13_wr_addr_val(1),
      ADR3 => NlwRenamedSig_OI_fifo_13_wr_addr_val(2),
      O => fifo_13_wr_addr_n0000(3)
    );
  fifo_13_wr_addr_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_13_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_13_wr_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_13_wr_addr_val(2),
      O => fifo_13_wr_addr_n0000(2)
    );
  fifo_13_wr_addr_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_13_wr_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_13_wr_addr_val(1),
      O => fifo_13_wr_addr_n0000(1)
    );
  fifo_00_rd_addr_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_00_rd_addr_val(0),
      O => fifo_00_rd_addr_n0000(0),
      ADR1 => GND
    );
  fifo_00_rd_addr_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_00_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_00_rd_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_00_rd_addr_val(1),
      ADR3 => NlwRenamedSig_OI_fifo_00_rd_addr_val(2),
      O => fifo_00_rd_addr_n0000(3)
    );
  fifo_00_rd_addr_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_00_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_00_rd_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_00_rd_addr_val(2),
      O => fifo_00_rd_addr_n0000(2)
    );
  fifo_00_rd_addr_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_00_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_00_rd_addr_val(1),
      O => fifo_00_rd_addr_n0000(1)
    );
  fifo_01_rd_addr_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_01_rd_addr_val(0),
      O => fifo_01_rd_addr_n0000(0),
      ADR1 => GND
    );
  fifo_01_rd_addr_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_01_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_01_rd_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_01_rd_addr_val(1),
      ADR3 => NlwRenamedSig_OI_fifo_01_rd_addr_val(2),
      O => fifo_01_rd_addr_n0000(3)
    );
  fifo_01_rd_addr_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_01_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_01_rd_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_01_rd_addr_val(2),
      O => fifo_01_rd_addr_n0000(2)
    );
  fifo_01_rd_addr_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_01_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_01_rd_addr_val(1),
      O => fifo_01_rd_addr_n0000(1)
    );
  fifo_02_rd_addr_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_02_rd_addr_val(0),
      O => fifo_02_rd_addr_n0000(0),
      ADR1 => GND
    );
  fifo_02_rd_addr_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_02_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_02_rd_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_02_rd_addr_val(1),
      ADR3 => NlwRenamedSig_OI_fifo_02_rd_addr_val(2),
      O => fifo_02_rd_addr_n0000(3)
    );
  fifo_02_rd_addr_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_02_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_02_rd_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_02_rd_addr_val(2),
      O => fifo_02_rd_addr_n0000(2)
    );
  fifo_02_rd_addr_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_02_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_02_rd_addr_val(1),
      O => fifo_02_rd_addr_n0000(1)
    );
  fifo_03_rd_addr_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_03_rd_addr_val(0),
      O => fifo_03_rd_addr_n0000(0),
      ADR1 => GND
    );
  fifo_03_rd_addr_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_03_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_03_rd_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_03_rd_addr_val(1),
      ADR3 => NlwRenamedSig_OI_fifo_03_rd_addr_val(2),
      O => fifo_03_rd_addr_n0000(3)
    );
  fifo_03_rd_addr_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_03_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_03_rd_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_03_rd_addr_val(2),
      O => fifo_03_rd_addr_n0000(2)
    );
  fifo_03_rd_addr_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_03_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_03_rd_addr_val(1),
      O => fifo_03_rd_addr_n0000(1)
    );
  fifo_10_rd_addr_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_10_rd_addr_val(0),
      O => fifo_10_rd_addr_n0000(0),
      ADR1 => GND
    );
  fifo_10_rd_addr_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_10_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_10_rd_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_10_rd_addr_val(1),
      ADR3 => NlwRenamedSig_OI_fifo_10_rd_addr_val(2),
      O => fifo_10_rd_addr_n0000(3)
    );
  fifo_10_rd_addr_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_10_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_10_rd_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_10_rd_addr_val(2),
      O => fifo_10_rd_addr_n0000(2)
    );
  fifo_10_rd_addr_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_10_rd_addr_val(0),
      ADR1 => NlwRenamedSig_OI_fifo_10_rd_addr_val(1),
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
  Q_n00081 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => rd_data_valid_reg_26,
      ADR1 => read_valid_data,
      O => Q_n0008
    );
  read_valid_data1 : X_LUT4
    generic map(
      INIT => X"22F2"
    )
    port map (
      ADR0 => fifo_01_not_empty_r1_25,
      ADR1 => Q_n0018,
      ADR2 => fifo_03_not_empty_r1_28,
      ADR3 => Q_n0019,
      O => read_valid_data
    );
  Q_n0018426 : X_LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_00_rd_addr_val(1),
      ADR1 => NlwRenamedSig_OI_fifo_01_wr_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_00_rd_addr_val(0),
      ADR3 => NlwRenamedSig_OI_fifo_01_wr_addr_val(0),
      O => N28
    );
  Q_n0018453 : X_LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_00_rd_addr_val(3),
      ADR1 => NlwRenamedSig_OI_fifo_01_wr_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_00_rd_addr_val(2),
      ADR3 => NlwRenamedSig_OI_fifo_01_wr_addr_val(2),
      O => N51
    );
  Q_n0018454 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => N28,
      ADR1 => N51,
      O => Q_n0018
    );
  Q_n0019426 : X_LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_02_rd_addr_val(1),
      ADR1 => NlwRenamedSig_OI_fifo_03_wr_addr_val(1),
      ADR2 => NlwRenamedSig_OI_fifo_02_rd_addr_val(0),
      ADR3 => NlwRenamedSig_OI_fifo_03_wr_addr_val(0),
      O => N77
    );
  Q_n0019453 : X_LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_fifo_02_rd_addr_val(3),
      ADR1 => NlwRenamedSig_OI_fifo_03_wr_addr_val(3),
      ADR2 => NlwRenamedSig_OI_fifo_02_rd_addr_val(2),
      ADR3 => NlwRenamedSig_OI_fifo_03_wr_addr_val(2),
      O => N100
    );
  Q_n0019454 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => N77,
      ADR1 => N100,
      O => Q_n0019
    );
  fifo_01_not_empty1 : X_LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      ADR0 => N28,
      ADR1 => N51,
      O => fifo_01_not_empty
    );
  fifo_03_not_empty1 : X_LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      ADR0 => N77,
      ADR1 => N100,
      O => fifo_03_not_empty
    );
  Q_n00201 : X_LUT3
    generic map(
      INIT => X"4C"
    )
    port map (
      ADR0 => N28,
      ADR1 => fifo_01_not_empty_r1_25,
      ADR2 => N51,
      O => read_valid_data_1
    );
  Q_n00211 : X_LUT3
    generic map(
      INIT => X"4C"
    )
    port map (
      ADR0 => N77,
      ADR1 => fifo_03_not_empty_r1_28,
      ADR2 => N100,
      O => read_valid_data_2
    );
  dqs_delay1_col1_six : X_LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      ADR0 => dqs_int_delay_in1,
      ADR1 => delay_sel(1),
      ADR2 => dqs_delay1_col1_delay2,
      ADR3 => dqs_delay1_col1_high,
      O => dqs_delay1_col1_delay3
    );
  dqs_delay1_col1_five : X_LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      ADR0 => dqs_delay1_col1_high,
      ADR1 => delay_sel(3),
      ADR2 => dqs_delay1_col1_delay4,
      ADR3 => dqs_int_delay_in1,
      O => dqs_delay1_col1_delay5
    );
  dqs_delay1_col1_four : X_LUT4
    generic map(
      INIT => X"FF00"
    )
    port map (
      ADR0 => dqs_delay1_col1_high,
      ADR1 => dqs_delay1_col1_high,
      ADR2 => dqs_delay1_col1_high,
      ADR3 => dqs_int_delay_in1,
      O => dqs_delay1_col1_delay1
    );
  dqs_delay1_col1_three : X_LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      ADR0 => dqs_int_delay_in1,
      ADR1 => delay_sel(0),
      ADR2 => dqs_delay1_col1_delay1,
      ADR3 => dqs_delay1_col1_high,
      O => dqs_delay1_col1_delay2
    );
  dqs_delay1_col1_two : X_LUT4
    generic map(
      INIT => X"EE22"
    )
    port map (
      ADR0 => dqs_int_delay_in1,
      ADR1 => delay_sel(2),
      ADR2 => dqs_delay1_col1_high,
      ADR3 => dqs_delay1_col1_delay3,
      O => dqs_delay1_col1_delay4
    );
  dqs_delay1_col1_one : X_LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      ADR0 => dqs_delay1_col1_high,
      ADR1 => delay_sel(4),
      ADR2 => dqs_delay1_col1_delay5,
      ADR3 => dqs_int_delay_in1,
      O => NlwRenamedSig_OI_dqs_delayed_col1_val(1)
    );
  dqs_delay1_col1_XST_VCC : X_ONE
    port map (
      O => dqs_delay1_col1_high
    );
  dqs_delay1_col0_six : X_LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      ADR0 => dqs_int_delay_in1,
      ADR1 => delay_sel(1),
      ADR2 => dqs_delay1_col0_delay2,
      ADR3 => dqs_delay1_col0_high,
      O => dqs_delay1_col0_delay3
    );
  dqs_delay1_col0_five : X_LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      ADR0 => dqs_delay1_col0_high,
      ADR1 => delay_sel(3),
      ADR2 => dqs_delay1_col0_delay4,
      ADR3 => dqs_int_delay_in1,
      O => dqs_delay1_col0_delay5
    );
  dqs_delay1_col0_four : X_LUT4
    generic map(
      INIT => X"FF00"
    )
    port map (
      ADR0 => dqs_delay1_col0_high,
      ADR1 => dqs_delay1_col0_high,
      ADR2 => dqs_delay1_col0_high,
      ADR3 => dqs_int_delay_in1,
      O => dqs_delay1_col0_delay1
    );
  dqs_delay1_col0_three : X_LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      ADR0 => dqs_int_delay_in1,
      ADR1 => delay_sel(0),
      ADR2 => dqs_delay1_col0_delay1,
      ADR3 => dqs_delay1_col0_high,
      O => dqs_delay1_col0_delay2
    );
  dqs_delay1_col0_two : X_LUT4
    generic map(
      INIT => X"EE22"
    )
    port map (
      ADR0 => dqs_int_delay_in1,
      ADR1 => delay_sel(2),
      ADR2 => dqs_delay1_col0_high,
      ADR3 => dqs_delay1_col0_delay3,
      O => dqs_delay1_col0_delay4
    );
  dqs_delay1_col0_one : X_LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      ADR0 => dqs_delay1_col0_high,
      ADR1 => delay_sel(4),
      ADR2 => dqs_delay1_col0_delay5,
      ADR3 => dqs_int_delay_in1,
      O => NlwRenamedSig_OI_dqs_delayed_col0_val(1)
    );
  dqs_delay1_col0_XST_VCC : X_ONE
    port map (
      O => dqs_delay1_col0_high
    );
  dqs_delay0_col1_six : X_LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      ADR0 => dqs_int_delay_in0,
      ADR1 => delay_sel(1),
      ADR2 => dqs_delay0_col1_delay2,
      ADR3 => dqs_delay0_col1_high,
      O => dqs_delay0_col1_delay3
    );
  dqs_delay0_col1_five : X_LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      ADR0 => dqs_delay0_col1_high,
      ADR1 => delay_sel(3),
      ADR2 => dqs_delay0_col1_delay4,
      ADR3 => dqs_int_delay_in0,
      O => dqs_delay0_col1_delay5
    );
  dqs_delay0_col1_four : X_LUT4
    generic map(
      INIT => X"FF00"
    )
    port map (
      ADR0 => dqs_delay0_col1_high,
      ADR1 => dqs_delay0_col1_high,
      ADR2 => dqs_delay0_col1_high,
      ADR3 => dqs_int_delay_in0,
      O => dqs_delay0_col1_delay1
    );
  dqs_delay0_col1_three : X_LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      ADR0 => dqs_int_delay_in0,
      ADR1 => delay_sel(0),
      ADR2 => dqs_delay0_col1_delay1,
      ADR3 => dqs_delay0_col1_high,
      O => dqs_delay0_col1_delay2
    );
  dqs_delay0_col1_two : X_LUT4
    generic map(
      INIT => X"EE22"
    )
    port map (
      ADR0 => dqs_int_delay_in0,
      ADR1 => delay_sel(2),
      ADR2 => dqs_delay0_col1_high,
      ADR3 => dqs_delay0_col1_delay3,
      O => dqs_delay0_col1_delay4
    );
  dqs_delay0_col1_one : X_LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      ADR0 => dqs_delay0_col1_high,
      ADR1 => delay_sel(4),
      ADR2 => dqs_delay0_col1_delay5,
      ADR3 => dqs_int_delay_in0,
      O => NlwRenamedSig_OI_dqs_delayed_col1_val(0)
    );
  dqs_delay0_col1_XST_VCC : X_ONE
    port map (
      O => dqs_delay0_col1_high
    );
  dqs_delay0_col0_six : X_LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      ADR0 => dqs_int_delay_in0,
      ADR1 => delay_sel(1),
      ADR2 => dqs_delay0_col0_delay2,
      ADR3 => dqs_delay0_col0_high,
      O => dqs_delay0_col0_delay3
    );
  dqs_delay0_col0_five : X_LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      ADR0 => dqs_delay0_col0_high,
      ADR1 => delay_sel(3),
      ADR2 => dqs_delay0_col0_delay4,
      ADR3 => dqs_int_delay_in0,
      O => dqs_delay0_col0_delay5
    );
  dqs_delay0_col0_four : X_LUT4
    generic map(
      INIT => X"FF00"
    )
    port map (
      ADR0 => dqs_delay0_col0_high,
      ADR1 => dqs_delay0_col0_high,
      ADR2 => dqs_delay0_col0_high,
      ADR3 => dqs_int_delay_in0,
      O => dqs_delay0_col0_delay1
    );
  dqs_delay0_col0_three : X_LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      ADR0 => dqs_int_delay_in0,
      ADR1 => delay_sel(0),
      ADR2 => dqs_delay0_col0_delay1,
      ADR3 => dqs_delay0_col0_high,
      O => dqs_delay0_col0_delay2
    );
  dqs_delay0_col0_two : X_LUT4
    generic map(
      INIT => X"EE22"
    )
    port map (
      ADR0 => dqs_int_delay_in0,
      ADR1 => delay_sel(2),
      ADR2 => dqs_delay0_col0_high,
      ADR3 => dqs_delay0_col0_delay3,
      O => dqs_delay0_col0_delay4
    );
  dqs_delay0_col0_one : X_LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      ADR0 => dqs_delay0_col0_high,
      ADR1 => delay_sel(4),
      ADR2 => dqs_delay0_col0_delay5,
      ADR3 => dqs_int_delay_in0,
      O => NlwRenamedSig_OI_dqs_delayed_col0_val(0)
    );
  dqs_delay0_col0_XST_VCC : X_ONE
    port map (
      O => dqs_delay0_col0_high
    );
  rst_dqs_div_delay0_six : X_LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      ADR0 => rst_dqs_div_int,
      ADR1 => delay_sel(1),
      ADR2 => rst_dqs_div_delay0_delay2,
      ADR3 => rst_dqs_div_delay0_high,
      O => rst_dqs_div_delay0_delay3
    );
  rst_dqs_div_delay0_five : X_LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      ADR0 => rst_dqs_div_delay0_high,
      ADR1 => delay_sel(3),
      ADR2 => rst_dqs_div_delay0_delay4,
      ADR3 => rst_dqs_div_int,
      O => rst_dqs_div_delay0_delay5
    );
  rst_dqs_div_delay0_four : X_LUT4
    generic map(
      INIT => X"FF00"
    )
    port map (
      ADR0 => rst_dqs_div_delay0_high,
      ADR1 => rst_dqs_div_delay0_high,
      ADR2 => rst_dqs_div_delay0_high,
      ADR3 => rst_dqs_div_int,
      O => rst_dqs_div_delay0_delay1
    );
  rst_dqs_div_delay0_three : X_LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      ADR0 => rst_dqs_div_int,
      ADR1 => delay_sel(0),
      ADR2 => rst_dqs_div_delay0_delay1,
      ADR3 => rst_dqs_div_delay0_high,
      O => rst_dqs_div_delay0_delay2
    );
  rst_dqs_div_delay0_two : X_LUT4
    generic map(
      INIT => X"EE22"
    )
    port map (
      ADR0 => rst_dqs_div_int,
      ADR1 => delay_sel(2),
      ADR2 => rst_dqs_div_delay0_high,
      ADR3 => rst_dqs_div_delay0_delay3,
      O => rst_dqs_div_delay0_delay4
    );
  rst_dqs_div_delay0_one : X_LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      ADR0 => rst_dqs_div_delay0_high,
      ADR1 => delay_sel(4),
      ADR2 => rst_dqs_div_delay0_delay5,
      ADR3 => rst_dqs_div_int,
      O => rst_dqs_div_delayed
    );
  rst_dqs_div_delay0_XST_VCC : X_ONE
    port map (
      O => rst_dqs_div_delay0_high
    );
  NlwBlock_data_read_controller0_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlock_data_read_controller0_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal VCC : STD_LOGIC; 
  signal GND : STD_LOGIC; 
begin
  rst0_r : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => reset,
      CLK => clk,
      O => reset_r,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  rst90_r : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => reset90,
      CLK => clk90,
      O => reset90_r,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  rst180_r : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => reset180,
      CLK => clk180,
      O => reset180_r,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  rst270_r : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => reset270,
      CLK => clk270,
      O => reset270_r,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  NlwBlock_data_path_rst0_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlock_data_path_rst0_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal DQS_enable1_31 : STD_LOGIC; 
  signal DQS_enable2_32 : STD_LOGIC; 
  signal AUTO_REF_detect_33 : STD_LOGIC; 
  signal CONFLICT_34 : STD_LOGIC; 
  signal Auto_Ref_issued_35 : STD_LOGIC; 
  signal ddr_ODT2_36 : STD_LOGIC; 
  signal low : STD_LOGIC; 
  signal Auto_Ref_issued_p : STD_LOGIC; 
  signal AUTO_REF_detect1_37 : STD_LOGIC; 
  signal Write_enable_out : STD_LOGIC; 
  signal write_cmd_in : STD_LOGIC; 
  signal GO_TO_ODT_ON_38 : STD_LOGIC; 
  signal read_cmd1_39 : STD_LOGIC; 
  signal read_cmd2_40 : STD_LOGIC; 
  signal read_cmd3_41 : STD_LOGIC; 
  signal read_cmd4_42 : STD_LOGIC; 
  signal read_cmd5_43 : STD_LOGIC; 
  signal GO_TO_ACTIVE_value : STD_LOGIC; 
  signal RFC_COUNT_reg_44 : STD_LOGIC; 
  signal read_cmd : STD_LOGIC; 
  signal DQS_reset_int_45 : STD_LOGIC; 
  signal DQS_reset1_clk0_46 : STD_LOGIC; 
  signal auto_ref_wait1_47 : STD_LOGIC; 
  signal auto_ref_wait2_48 : STD_LOGIC; 
  signal DQS_reset2_clk0_49 : STD_LOGIC; 
  signal DQS_enable_int_50 : STD_LOGIC; 
  signal BA_address_conflict_51 : STD_LOGIC; 
  signal accept_cmd_in : STD_LOGIC; 
  signal AR_Done_reg_52 : STD_LOGIC; 
  signal write_cmd1_53 : STD_LOGIC; 
  signal write_cmd2_54 : STD_LOGIC; 
  signal write_cmd3_55 : STD_LOGIC; 
  signal write_cmd4_56 : STD_LOGIC; 
  signal write_cmd5_57 : STD_LOGIC; 
  signal write_cmd6_58 : STD_LOGIC; 
  signal write_cmd7_59 : STD_LOGIC; 
  signal write_cmd8_60 : STD_LOGIC; 
  signal Q_n0031 : STD_LOGIC; 
  signal INIT_MEMORY_61 : STD_LOGIC; 
  signal DQS_reset_out : STD_LOGIC; 
  signal Q_n0200 : STD_LOGIC; 
  signal current_state_FFd5_62 : STD_LOGIC; 
  signal current_state_FFd7_63 : STD_LOGIC; 
  signal current_state_FFd12_64 : STD_LOGIC; 
  signal current_state_FFd6_65 : STD_LOGIC; 
  signal current_state_FFd13_66 : STD_LOGIC; 
  signal Q_n0213 : STD_LOGIC; 
  signal current_state_FFd9_67 : STD_LOGIC; 
  signal current_state_FFd11_68 : STD_LOGIC; 
  signal Q_n0215 : STD_LOGIC; 
  signal current_state_FFd8_69 : STD_LOGIC; 
  signal current_state_FFd4_70 : STD_LOGIC; 
  signal Q_n0217 : STD_LOGIC; 
  signal current_state_FFd3_71 : STD_LOGIC; 
  signal current_state_FFd14_72 : STD_LOGIC; 
  signal current_state_FFd10_73 : STD_LOGIC; 
  signal current_state_FFd2_74 : STD_LOGIC; 
  signal current_state_FFd1_75 : STD_LOGIC; 
  signal Q_n0181 : STD_LOGIC; 
  signal Q_n0235 : STD_LOGIC; 
  signal Q_n0241 : STD_LOGIC; 
  signal Q_n0242 : STD_LOGIC; 
  signal Q_n0238_76 : STD_LOGIC; 
  signal Q_n0243 : STD_LOGIC; 
  signal Q_n0244 : STD_LOGIC; 
  signal Q_n0245_77 : STD_LOGIC; 
  signal Q_n0250_78 : STD_LOGIC; 
  signal Q_n0247 : STD_LOGIC; 
  signal Q_n0248 : STD_LOGIC; 
  signal Q_n0322 : STD_LOGIC; 
  signal Q_n0282_79 : STD_LOGIC; 
  signal Q_n0283 : STD_LOGIC; 
  signal Q_n0342 : STD_LOGIC; 
  signal Q_n0337 : STD_LOGIC; 
  signal rst_dqs_div_r_80 : STD_LOGIC; 
  signal Q_n0380 : STD_LOGIC; 
  signal Q_n0381_81 : STD_LOGIC; 
  signal DQS_enable_out : STD_LOGIC; 
  signal Q_n0378 : STD_LOGIC; 
  signal Q_n0385 : STD_LOGIC; 
  signal rst0_r_82 : STD_LOGIC; 
  signal rst180_r_83 : STD_LOGIC; 
  signal INIT_PRE_COUNT_en : STD_LOGIC; 
  signal AUTO_REF_pulse_end_84 : STD_LOGIC; 
  signal wrburst_end_1_85 : STD_LOGIC; 
  signal GO_TO_ACTIVE_86 : STD_LOGIC; 
  signal wrburst_end_2_87 : STD_LOGIC; 
  signal wrburst_end_3_88 : STD_LOGIC; 
  signal GO_TO_ODT_ON_value : STD_LOGIC; 
  signal INIT_DONE_value : STD_LOGIC; 
  signal auto_ref_89 : STD_LOGIC; 
  signal AUTOREF_value : STD_LOGIC; 
  signal ack_o : STD_LOGIC; 
  signal initialize_memory : STD_LOGIC; 
  signal rdburst_end_1_90 : STD_LOGIC; 
  signal rdburst_end_2_91 : STD_LOGIC; 
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
  signal current_state_FFd8_In_92 : STD_LOGIC; 
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
  signal AUTOREF_COUNT_1_rt_93 : STD_LOGIC; 
  signal AUTOREF_COUNT_2_rt_94 : STD_LOGIC; 
  signal AUTOREF_COUNT_3_rt_95 : STD_LOGIC; 
  signal AUTOREF_COUNT_4_rt_96 : STD_LOGIC; 
  signal AUTOREF_COUNT_5_rt_97 : STD_LOGIC; 
  signal AUTOREF_COUNT_6_rt_98 : STD_LOGIC; 
  signal AUTOREF_COUNT_7_rt_99 : STD_LOGIC; 
  signal AUTOREF_COUNT_8_rt_100 : STD_LOGIC; 
  signal AUTOREF_COUNT_9_rt_101 : STD_LOGIC; 
  signal AUTOREF_COUNT_10_rt_102 : STD_LOGIC; 
  signal AUTOREF_COUNT_11_rt_103 : STD_LOGIC; 
  signal DQS_reset1_clk0_rt_104 : STD_LOGIC; 
  signal current_state_FFd3_rt_105 : STD_LOGIC; 
  signal INIT_PRE_COUNT_4_rt_106 : STD_LOGIC; 
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
  signal VCC : STD_LOGIC; 
  signal GND : STD_LOGIC; 
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
  XST_GND : X_ZERO
    port map (
      O => NlwRenamedSig_OI_high(1)
    );
  XST_VCC : X_ONE
    port map (
      O => low
    );
  wrburst_end_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => low,
      SRST => Q_n0245_77,
      CLK => clk180,
      O => wrburst_end_1_85,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  rst0_r : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => rst0,
      CLK => clk,
      O => rst0_r_82,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  AR_Done_reg : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => low,
      SRST => Q_n0247,
      CLK => clk180,
      O => AR_Done_reg_52,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  rst180_r : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => rst180,
      CLK => clk180,
      O => rst180_r_83,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  rdburst_end_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => low,
      SRST => Q_n0250_78,
      CLK => clk180,
      O => rdburst_end_1_90,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  BA_address_conflict : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => low,
      SRST => low,
      CLK => clk180,
      O => BA_address_conflict_51,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  read_cmd1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => read_cmd,
      SRST => rst180_r_83,
      CE => accept_cmd_in,
      CLK => clk180,
      O => read_cmd1_39,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  write_cmd1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_cmd_in,
      SRST => rst180_r_83,
      CE => accept_cmd_in,
      CLK => clk180,
      O => write_cmd1_53,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  write_enable_29 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Write_enable_out,
      SRST => rst180_r_83,
      CLK => clk180,
      O => write_enable,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  AUTO_REF_detect1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => AUTOREF_value,
      SRST => rst180_r_83,
      CLK => clk180,
      O => AUTO_REF_detect1_37,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  auto_ref_wait1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0031,
      SRST => rst180_r_83,
      CLK => clk180,
      O => auto_ref_wait1_47,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  GO_TO_ACTIVE : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => GO_TO_ACTIVE_value,
      SRST => rst180_r_83,
      CLK => clk180,
      O => GO_TO_ACTIVE_86,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  config_reg1_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => config_register1(1),
      SRST => rst180_r_83,
      CLK => clk180,
      O => config_reg1_1_Q,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  config_reg1_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => config_register1(4),
      SRST => rst180_r_83,
      CLK => clk180,
      O => config_reg1_4_Q,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  config_reg1_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => config_register1(5),
      SRST => rst180_r_83,
      CLK => clk180,
      O => config_reg1_5_Q,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  config_reg1_12 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => config_register1(12),
      SRST => rst180_r_83,
      CLK => clk180,
      O => config_reg1_12_Q,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  config_reg2_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => config_register2(2),
      SRST => rst180_r_83,
      CLK => clk180,
      O => config_reg2_2_Q,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  config_reg2_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => config_register2(7),
      SRST => rst180_r_83,
      CLK => clk180,
      O => config_reg2_7_Q,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  config_reg2_8 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => config_register2(8),
      SRST => rst180_r_83,
      CLK => clk180,
      O => config_reg2_8_Q,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  config_reg2_9 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => config_register2(9),
      SRST => rst180_r_83,
      CLK => clk180,
      O => config_reg2_9_Q,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  rst_dqs_div_r : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => NlwRenamedSig_OI_high(1),
      SRST => rst180_r_83,
      SSET => Q_n0241,
      CE => Q_n0242,
      CLK => clk180,
      O => rst_dqs_div_r_80,
      SET => GND,
      RST => GND
    );
  INIT_MEMORY : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => NlwRenamedSig_OI_high(1),
      SRST => rst180_r_83,
      SSET => initialize_memory,
      CE => Q_n0244,
      CLK => clk180,
      O => INIT_MEMORY_61,
      SET => GND,
      RST => GND
    );
  CONFLICT : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => CONFLICT_value,
      SRST => rst180_r_83,
      CLK => clk180,
      O => CONFLICT_34,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  wrburst_end_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => wrburst_end_1_85,
      SRST => rst180_r_83,
      CLK => clk180,
      O => wrburst_end_2_87,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  DQS_enable_int : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => DQS_enable_out,
      SRST => rst180_r_83,
      CLK => clk180,
      O => DQS_enable_int_50,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  address_reg_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => address(2),
      SRST => rst180_r_83,
      CLK => clk180,
      O => address_reg(2),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  address_reg_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => address(3),
      SRST => rst180_r_83,
      CLK => clk180,
      O => address_reg(3),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  address_reg_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => address(4),
      SRST => rst180_r_83,
      CLK => clk180,
      O => address_reg(4),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  address_reg_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => address(5),
      SRST => rst180_r_83,
      CLK => clk180,
      O => address_reg(5),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  address_reg_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => address(6),
      SRST => rst180_r_83,
      CLK => clk180,
      O => address_reg(6),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  address_reg_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => address(7),
      SRST => rst180_r_83,
      CLK => clk180,
      O => address_reg(7),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  address_reg_8 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => address(8),
      SRST => rst180_r_83,
      CLK => clk180,
      O => address_reg(8),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  address_reg_9 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => address(9),
      SRST => rst180_r_83,
      CLK => clk180,
      O => address_reg(9),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  GO_TO_ODT_ON : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => GO_TO_ODT_ON_value,
      SRST => rst180_r_83,
      CLK => clk180,
      O => GO_TO_ODT_ON_38,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  rdburst_end_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => rdburst_end_1_90,
      SRST => rst180_r_83,
      CLK => clk180,
      O => rdburst_end_2_91,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  AUTOREF_COUNT_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => AUTOREF_CNT_val(0),
      SRST => rst180_r_83,
      CLK => clk180,
      O => AUTOREF_COUNT(0),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  AUTOREF_COUNT_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => AUTOREF_CNT_val(1),
      SRST => rst180_r_83,
      CLK => clk180,
      O => AUTOREF_COUNT(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  AUTOREF_COUNT_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => AUTOREF_CNT_val(2),
      SRST => rst180_r_83,
      CLK => clk180,
      O => AUTOREF_COUNT(2),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  AUTOREF_COUNT_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => AUTOREF_CNT_val(3),
      SRST => rst180_r_83,
      CLK => clk180,
      O => AUTOREF_COUNT(3),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  AUTOREF_COUNT_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => AUTOREF_CNT_val(4),
      SRST => rst180_r_83,
      CLK => clk180,
      O => AUTOREF_COUNT(4),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  AUTOREF_COUNT_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => AUTOREF_CNT_val(5),
      SRST => rst180_r_83,
      CLK => clk180,
      O => AUTOREF_COUNT(5),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  AUTOREF_COUNT_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => AUTOREF_CNT_val(6),
      SRST => rst180_r_83,
      CLK => clk180,
      O => AUTOREF_COUNT(6),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  AUTOREF_COUNT_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => AUTOREF_CNT_val(7),
      SRST => rst180_r_83,
      CLK => clk180,
      O => AUTOREF_COUNT(7),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  AUTOREF_COUNT_8 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => AUTOREF_CNT_val(8),
      SRST => rst180_r_83,
      CLK => clk180,
      O => AUTOREF_COUNT(8),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  AUTOREF_COUNT_9 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => AUTOREF_CNT_val(9),
      SRST => rst180_r_83,
      CLK => clk180,
      O => AUTOREF_COUNT(9),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  AUTOREF_COUNT_10 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => AUTOREF_CNT_val(10),
      SRST => rst180_r_83,
      CLK => clk180,
      O => AUTOREF_COUNT(10),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  AUTOREF_COUNT_11 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => AUTOREF_CNT_val(11),
      SRST => rst180_r_83,
      CLK => clk180,
      O => AUTOREF_COUNT(11),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  INIT_DONE : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => INIT_DONE_value,
      SRST => rst180_r_83,
      CLK => clk180,
      O => NlwRenamedSig_OI_init,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  RFC_COUNT_reg : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => NlwRenamedSig_OI_high(1),
      SRST => rst180_r_83,
      SSET => AR_Done_reg_52,
      CE => Auto_Ref_issued_p,
      CLK => clk180,
      O => RFC_COUNT_reg_44,
      SET => GND,
      RST => GND
    );
  read_cmd2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => read_cmd1_39,
      SRST => rst180_r_83,
      CLK => clk180,
      O => read_cmd2_40,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  write_cmd2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_cmd1_53,
      SRST => rst180_r_83,
      CLK => clk180,
      O => write_cmd2_54,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  auto_ref_wait2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => auto_ref_wait1_47,
      SRST => Q_n0235,
      CLK => clk180,
      O => auto_ref_wait2_48,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  wrburst_end_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => wrburst_end_2_87,
      SRST => rst180_r_83,
      CLK => clk180,
      O => wrburst_end_3_88,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  cas_latency_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => config_reg1_4_Q,
      SRST => rst180_r_83,
      CLK => clk180,
      O => cas_latency(0),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  cas_latency_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => config_reg1_5_Q,
      SRST => rst180_r_83,
      CLK => clk180,
      O => cas_latency(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  burst_length_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => config_reg1_1_Q,
      SRST => rst180_r_83,
      CLK => clk180,
      O => burst_length(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  DLL_RST_COUNT_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => DLL_RST_COUNT_value(0),
      SRST => rst180_r_83,
      CLK => clk180,
      O => DLL_RST_COUNT(0),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  DLL_RST_COUNT_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => DLL_RST_COUNT_value(1),
      SRST => rst180_r_83,
      CLK => clk180,
      O => DLL_RST_COUNT(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  DLL_RST_COUNT_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => DLL_RST_COUNT_value(2),
      SRST => rst180_r_83,
      CLK => clk180,
      O => DLL_RST_COUNT(2),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  DLL_RST_COUNT_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => DLL_RST_COUNT_value(3),
      SRST => rst180_r_83,
      CLK => clk180,
      O => DLL_RST_COUNT(3),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  DLL_RST_COUNT_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => DLL_RST_COUNT_value(4),
      SRST => rst180_r_83,
      CLK => clk180,
      O => DLL_RST_COUNT(4),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  DLL_RST_COUNT_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => DLL_RST_COUNT_value(5),
      SRST => rst180_r_83,
      CLK => clk180,
      O => DLL_RST_COUNT(5),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  DLL_RST_COUNT_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => DLL_RST_COUNT_value(6),
      SRST => rst180_r_83,
      CLK => clk180,
      O => DLL_RST_COUNT(6),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  DLL_RST_COUNT_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => DLL_RST_COUNT_value(7),
      SRST => rst180_r_83,
      CLK => clk180,
      O => DLL_RST_COUNT(7),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  EMR_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => config_reg2_2_Q,
      SRST => rst180_r_83,
      CLK => clk180,
      O => EMR_2_Q,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  EMR_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => config_reg2_7_Q,
      SRST => rst180_r_83,
      CLK => clk180,
      O => EMR_7_Q,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  EMR_8 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => config_reg2_8_Q,
      SRST => rst180_r_83,
      CLK => clk180,
      O => EMR_8_Q,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  EMR_9 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => config_reg2_9_Q,
      SRST => rst180_r_83,
      CLK => clk180,
      O => EMR_9_Q,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  DQS_enable1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => DQS_enable_int_50,
      SRST => rst0_r_82,
      CLK => clk,
      O => DQS_enable1_31,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  LMR_DLL_set_10 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => config_reg1_12_Q,
      SRST => rst180_r_83,
      CLK => clk180,
      O => LMR_DLL_set(10),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  wrburst_end_cnt_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => wrburst_end_cnt_value(0),
      SRST => rst180_r_83,
      CLK => clk180,
      O => wrburst_end_cnt(0),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  wrburst_end_cnt_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => wrburst_end_cnt_value(1),
      SRST => rst180_r_83,
      CLK => clk180,
      O => wrburst_end_cnt(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  AUTO_REF_detect : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => AUTO_REF_detect1_37,
      SRST => rst180_r_83,
      CLK => clk180,
      O => AUTO_REF_detect_33,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  LMR_DLL_rst_8 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => low,
      SRST => rst180_r_83,
      CLK => clk180,
      O => LMR_DLL_rst_8_Q,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  LMR_DLL_rst_10 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => config_reg1_12_Q,
      SRST => rst180_r_83,
      CLK => clk180,
      O => LMR_DLL_rst_10_Q,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  COLUMN_ADDRESS_reg_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => address_reg(2),
      SRST => rst180_r_83,
      CLK => clk180,
      O => COLUMN_ADDRESS_reg(2),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  COLUMN_ADDRESS_reg_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => address_reg(3),
      SRST => rst180_r_83,
      CLK => clk180,
      O => COLUMN_ADDRESS_reg(3),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  COLUMN_ADDRESS_reg_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => address_reg(4),
      SRST => rst180_r_83,
      CLK => clk180,
      O => COLUMN_ADDRESS_reg(4),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  COLUMN_ADDRESS_reg_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => address_reg(5),
      SRST => rst180_r_83,
      CLK => clk180,
      O => COLUMN_ADDRESS_reg(5),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  COLUMN_ADDRESS_reg_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => address_reg(6),
      SRST => rst180_r_83,
      CLK => clk180,
      O => COLUMN_ADDRESS_reg(6),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  COLUMN_ADDRESS_reg_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => address_reg(7),
      SRST => rst180_r_83,
      CLK => clk180,
      O => COLUMN_ADDRESS_reg(7),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  COLUMN_ADDRESS_reg_8 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => address_reg(8),
      SRST => rst180_r_83,
      CLK => clk180,
      O => COLUMN_ADDRESS_reg(8),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  COLUMN_ADDRESS_reg_9 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => address_reg(9),
      SRST => rst180_r_83,
      CLK => clk180,
      O => COLUMN_ADDRESS_reg(9),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  read_cmd3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => read_cmd2_40,
      SRST => rst180_r_83,
      CLK => clk180,
      O => read_cmd3_41,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  write_cmd3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_cmd2_54,
      SRST => rst180_r_83,
      CLK => clk180,
      O => write_cmd3_55,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  auto_ref : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => auto_ref_wait2_48,
      SRST => Q_n0235,
      CLK => clk180,
      O => auto_ref_89,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  AUTO_REF_pulse_end : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => AUTO_REF_detect_33,
      SRST => rst180_r_83,
      CLK => clk180,
      O => AUTO_REF_pulse_end_84,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  DQS_enable2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => DQS_enable1_31,
      SRST => rst0_r_82,
      CLK => clk,
      O => DQS_enable2_32,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  ODT_COUNT_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => ODT_COUNT_value(0),
      SRST => rst180_r_83,
      CLK => clk180,
      O => ODT_COUNT(0),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  ODT_COUNT_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => ODT_COUNT_value(1),
      SRST => rst180_r_83,
      CLK => clk180,
      O => ODT_COUNT(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  read_cmd4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => read_cmd3_41,
      SRST => rst180_r_83,
      CLK => clk180,
      O => read_cmd4_42,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  write_cmd4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_cmd3_55,
      SRST => rst180_r_83,
      CLK => clk180,
      O => write_cmd4_56,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  read_cmd5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => read_cmd4_42,
      SRST => rst180_r_83,
      CLK => clk180,
      O => read_cmd5_43,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  write_cmd5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_cmd4_56,
      SRST => rst180_r_83,
      CLK => clk180,
      O => write_cmd5_57,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  write_cmd6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_cmd5_57,
      SRST => rst180_r_83,
      CLK => clk180,
      O => write_cmd6_58,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Auto_Ref_issued : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Auto_Ref_issued_p,
      SRST => rst180_r_83,
      CLK => clk180,
      O => Auto_Ref_issued_35,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  DQS_reset_int : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => DQS_reset_out,
      SRST => rst180_r_83,
      CLK => clk180,
      O => DQS_reset_int_45,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  write_cmd7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_cmd6_58,
      SRST => rst180_r_83,
      CLK => clk180,
      O => write_cmd7_59,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  RCDR_COUNT_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => RCDR_COUNT_value(0),
      SRST => rst180_r_83,
      CLK => clk180,
      O => RCDR_COUNT(0),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  RCDR_COUNT_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => RCDR_COUNT_value(1),
      SRST => rst180_r_83,
      CLK => clk180,
      O => RCDR_COUNT(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  RCDR_COUNT_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => RCDR_COUNT_value(2),
      SRST => rst180_r_83,
      CLK => clk180,
      O => RCDR_COUNT(2),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  RFC_COUNT_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => RFC_COUNT_value(0),
      SRST => rst180_r_83,
      CLK => clk180,
      O => RFC_COUNT(0),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  RFC_COUNT_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => RFC_COUNT_value(1),
      SRST => rst180_r_83,
      CLK => clk180,
      O => RFC_COUNT(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  RFC_COUNT_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => RFC_COUNT_value(2),
      SRST => rst180_r_83,
      CLK => clk180,
      O => RFC_COUNT(2),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  RFC_COUNT_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => RFC_COUNT_value(3),
      SRST => rst180_r_83,
      CLK => clk180,
      O => RFC_COUNT(3),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  RFC_COUNT_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => RFC_COUNT_value(4),
      SRST => rst180_r_83,
      CLK => clk180,
      O => RFC_COUNT(4),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  RFC_COUNT_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => RFC_COUNT_value(5),
      SRST => rst180_r_83,
      CLK => clk180,
      O => RFC_COUNT(5),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  write_cmd8 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_cmd7_59,
      SRST => rst180_r_83,
      CLK => clk180,
      O => write_cmd8_60,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  DQS_reset1_clk0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => DQS_reset_int_45,
      SRST => rst0_r_82,
      CLK => clk,
      O => DQS_reset1_clk0_46,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  RRD_COUNT_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => RRD_COUNT_value(0),
      SRST => rst180_r_83,
      CLK => clk180,
      O => RRD_COUNT(0),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  RRD_COUNT_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => RRD_COUNT_value(1),
      SRST => rst180_r_83,
      CLK => clk180,
      O => RRD_COUNT(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  MRD_COUNT_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => MRD_COUNT_value(0),
      SRST => rst180_r_83,
      CLK => clk180,
      O => MRD_COUNT(0),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  MRD_COUNT_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => MRD_COUNT_value(1),
      SRST => rst180_r_83,
      CLK => clk180,
      O => MRD_COUNT(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  auto_ref_wait : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => NlwRenamedSig_OI_high(1),
      SRST => rst180_r_83,
      SSET => Q_n0243,
      CE => Auto_Ref_issued_35,
      CLK => clk180,
      O => NlwRenamedSig_OI_auto_ref_req,
      SET => GND,
      RST => GND
    );
  RP_COUNT_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => RP_cnt_value(0),
      SRST => rst180_r_83,
      CLK => clk180,
      O => RP_COUNT(0),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  RP_COUNT_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => RP_cnt_value(1),
      SRST => rst180_r_83,
      CLK => clk180,
      O => RP_COUNT(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  RP_COUNT_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => RP_cnt_value(2),
      SRST => rst180_r_83,
      CLK => clk180,
      O => RP_COUNT(2),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  RCDW_COUNT_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => RCDW_COUNT_value(0),
      SRST => rst180_r_83,
      CLK => clk180,
      O => RCDW_COUNT(0),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  RCDW_COUNT_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => RCDW_COUNT_value(1),
      SRST => rst180_r_83,
      CLK => clk180,
      O => RCDW_COUNT(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  RCDW_COUNT_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => RCDW_COUNT_value(2),
      SRST => rst180_r_83,
      CLK => clk180,
      O => RCDW_COUNT(2),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  CAS_COUNT_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => cas_count_value(0),
      SRST => rst180_r_83,
      CLK => clk180,
      O => CAS_COUNT(0),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  CAS_COUNT_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => cas_count_value(1),
      SRST => rst180_r_83,
      CLK => clk180,
      O => CAS_COUNT(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  DQS_reset2_clk0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => DQS_reset1_clk0_46,
      SRST => rst0_r_82,
      CLK => clk,
      O => DQS_reset2_clk0_49,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  ddr_ODT2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => write_cmd8_60,
      SRST => rst180_r_83,
      CLK => clk180,
      O => ddr_ODT2_36,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  ddr_ODT_cntrl_30 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_ODT2_36,
      SRST => rst180_r_83,
      CLK => clk180,
      O => ddr_ODT_cntrl,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  ACK_REG_INST1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ack_o,
      CLK => clk180,
      O => cmd_ack,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  rst_calib0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => rst_dqs_div_r_80,
      CLK => clk180,
      O => rst_calib,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  rst_iob_out : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => rst_dqs_div_r_80,
      CLK => clk180,
      O => rst_dqs_div_int,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  controller_16bit_00_n0254_0_lut : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => AUTOREF_COUNT(0),
      O => N5,
      ADR1 => GND
    );
  controller_16bit_00_n0254_0_cy : X_MUX2
    port map (
      IB => NlwRenamedSig_OI_high(1),
      IA => low,
      SEL => N5,
      O => controller_16bit_00_n0254_0_cyo
    );
  controller_16bit_00_n0254_1_cy : X_MUX2
    port map (
      IB => controller_16bit_00_n0254_0_cyo,
      IA => NlwRenamedSig_OI_high(1),
      SEL => AUTOREF_COUNT_1_rt_93,
      O => controller_16bit_00_n0254_1_cyo
    );
  controller_16bit_00_n0254_1_xor : X_XOR2
    port map (
      I0 => controller_16bit_00_n0254_0_cyo,
      I1 => AUTOREF_COUNT_1_rt_93,
      O => Q_n0254(1)
    );
  controller_16bit_00_n0254_2_cy : X_MUX2
    port map (
      IB => controller_16bit_00_n0254_1_cyo,
      IA => NlwRenamedSig_OI_high(1),
      SEL => AUTOREF_COUNT_2_rt_94,
      O => controller_16bit_00_n0254_2_cyo
    );
  controller_16bit_00_n0254_2_xor : X_XOR2
    port map (
      I0 => controller_16bit_00_n0254_1_cyo,
      I1 => AUTOREF_COUNT_2_rt_94,
      O => Q_n0254(2)
    );
  controller_16bit_00_n0254_3_cy : X_MUX2
    port map (
      IB => controller_16bit_00_n0254_2_cyo,
      IA => NlwRenamedSig_OI_high(1),
      SEL => AUTOREF_COUNT_3_rt_95,
      O => controller_16bit_00_n0254_3_cyo
    );
  controller_16bit_00_n0254_3_xor : X_XOR2
    port map (
      I0 => controller_16bit_00_n0254_2_cyo,
      I1 => AUTOREF_COUNT_3_rt_95,
      O => Q_n0254(3)
    );
  controller_16bit_00_n0254_4_cy : X_MUX2
    port map (
      IB => controller_16bit_00_n0254_3_cyo,
      IA => NlwRenamedSig_OI_high(1),
      SEL => AUTOREF_COUNT_4_rt_96,
      O => controller_16bit_00_n0254_4_cyo
    );
  controller_16bit_00_n0254_4_xor : X_XOR2
    port map (
      I0 => controller_16bit_00_n0254_3_cyo,
      I1 => AUTOREF_COUNT_4_rt_96,
      O => Q_n0254(4)
    );
  controller_16bit_00_n0254_5_cy : X_MUX2
    port map (
      IB => controller_16bit_00_n0254_4_cyo,
      IA => NlwRenamedSig_OI_high(1),
      SEL => AUTOREF_COUNT_5_rt_97,
      O => controller_16bit_00_n0254_5_cyo
    );
  controller_16bit_00_n0254_5_xor : X_XOR2
    port map (
      I0 => controller_16bit_00_n0254_4_cyo,
      I1 => AUTOREF_COUNT_5_rt_97,
      O => Q_n0254(5)
    );
  controller_16bit_00_n0254_6_cy : X_MUX2
    port map (
      IB => controller_16bit_00_n0254_5_cyo,
      IA => NlwRenamedSig_OI_high(1),
      SEL => AUTOREF_COUNT_6_rt_98,
      O => controller_16bit_00_n0254_6_cyo
    );
  controller_16bit_00_n0254_6_xor : X_XOR2
    port map (
      I0 => controller_16bit_00_n0254_5_cyo,
      I1 => AUTOREF_COUNT_6_rt_98,
      O => Q_n0254(6)
    );
  controller_16bit_00_n0254_7_cy : X_MUX2
    port map (
      IB => controller_16bit_00_n0254_6_cyo,
      IA => NlwRenamedSig_OI_high(1),
      SEL => AUTOREF_COUNT_7_rt_99,
      O => controller_16bit_00_n0254_7_cyo
    );
  controller_16bit_00_n0254_7_xor : X_XOR2
    port map (
      I0 => controller_16bit_00_n0254_6_cyo,
      I1 => AUTOREF_COUNT_7_rt_99,
      O => Q_n0254(7)
    );
  controller_16bit_00_n0254_8_cy : X_MUX2
    port map (
      IB => controller_16bit_00_n0254_7_cyo,
      IA => NlwRenamedSig_OI_high(1),
      SEL => AUTOREF_COUNT_8_rt_100,
      O => controller_16bit_00_n0254_8_cyo
    );
  controller_16bit_00_n0254_8_xor : X_XOR2
    port map (
      I0 => controller_16bit_00_n0254_7_cyo,
      I1 => AUTOREF_COUNT_8_rt_100,
      O => Q_n0254(8)
    );
  controller_16bit_00_n0254_9_cy : X_MUX2
    port map (
      IB => controller_16bit_00_n0254_8_cyo,
      IA => NlwRenamedSig_OI_high(1),
      SEL => AUTOREF_COUNT_9_rt_101,
      O => controller_16bit_00_n0254_9_cyo
    );
  controller_16bit_00_n0254_9_xor : X_XOR2
    port map (
      I0 => controller_16bit_00_n0254_8_cyo,
      I1 => AUTOREF_COUNT_9_rt_101,
      O => Q_n0254(9)
    );
  controller_16bit_00_n0254_10_cy : X_MUX2
    port map (
      IB => controller_16bit_00_n0254_9_cyo,
      IA => NlwRenamedSig_OI_high(1),
      SEL => AUTOREF_COUNT_10_rt_102,
      O => controller_16bit_00_n0254_10_cyo
    );
  controller_16bit_00_n0254_10_xor : X_XOR2
    port map (
      I0 => controller_16bit_00_n0254_9_cyo,
      I1 => AUTOREF_COUNT_10_rt_102,
      O => Q_n0254(10)
    );
  controller_16bit_00_n0254_11_xor : X_XOR2
    port map (
      I0 => controller_16bit_00_n0254_10_cyo,
      I1 => AUTOREF_COUNT_11_rt_103,
      O => Q_n0254(11)
    );
  current_state_FFd1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_state_FFd1_In,
      SRST => rst180_r_83,
      CLK => clk180,
      O => current_state_FFd1_75,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  current_state_FFd2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_state_FFd2_In,
      SRST => rst180_r_83,
      CLK => clk180,
      O => current_state_FFd2_74,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  current_state_FFd3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_state_FFd3_In,
      SRST => rst180_r_83,
      CLK => clk180,
      O => current_state_FFd3_71,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  current_state_FFd4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_state_FFd4_In,
      SRST => rst180_r_83,
      CLK => clk180,
      O => current_state_FFd4_70,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  current_state_FFd7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => DQS_reset_out,
      SRST => rst180_r_83,
      CLK => clk180,
      O => current_state_FFd7_63,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  current_state_FFd5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => N73,
      SRST => rst180_r_83,
      CLK => clk180,
      O => current_state_FFd5_62,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  current_state_FFd6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0217,
      SRST => rst180_r_83,
      CLK => clk180,
      O => current_state_FFd6_65,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  current_state_FFd8 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_state_FFd8_In_92,
      SRST => rst180_r_83,
      CLK => clk180,
      O => current_state_FFd8_69,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  current_state_FFd9 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0200,
      SRST => rst180_r_83,
      CLK => clk180,
      O => current_state_FFd9_67,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  current_state_FFd10 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => current_state_FFd10_In,
      SRST => rst180_r_83,
      CLK => clk180,
      O => current_state_FFd10_73,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  current_state_FFd11 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Auto_Ref_issued_p,
      SRST => rst180_r_83,
      CLK => clk180,
      O => current_state_FFd11_68,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  current_state_FFd14 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => current_state_FFd14_In,
      SSET => rst180_r_83,
      CLK => clk180,
      O => current_state_FFd14_72,
      CE => VCC,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  current_state_FFd12 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0213,
      SRST => rst180_r_83,
      CLK => clk180,
      O => current_state_FFd12_64,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  current_state_FFd13 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0215,
      SRST => rst180_r_83,
      CLK => clk180,
      O => current_state_FFd13_66,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  dqs_div_rdburstcount_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => dqs_div_rdburstcount_n0000(2),
      SRST => dqs_div_rdburstcount_0_n0000,
      CE => Q_n0385,
      CLK => clk180,
      O => dqs_div_rdburstcount(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  dqs_div_rdburstcount_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => dqs_div_rdburstcount_n0000(0),
      SRST => dqs_div_rdburstcount_0_n0000,
      CE => Q_n0385,
      CLK => clk180,
      O => dqs_div_rdburstcount(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  dqs_div_rdburstcount_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => dqs_div_rdburstcount_n0000(1),
      SRST => rst180_r_83,
      SSET => Q_n0248,
      CE => Q_n0385,
      CLK => clk180,
      O => dqs_div_rdburstcount(1),
      SET => GND,
      RST => GND
    );
  WR_COUNT_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => WR_COUNT_n0000(2),
      SRST => rst180_r_83,
      SSET => NlwRenamedSig_OI_dqs_enable,
      CE => Q_n0380,
      CLK => clk180,
      O => WR_COUNT(2),
      SET => GND,
      RST => GND
    );
  WR_COUNT_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => WR_COUNT_n0000(0),
      SRST => WR_COUNT_0_n0000,
      CE => Q_n0380,
      CLK => clk180,
      O => WR_COUNT(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  WR_COUNT_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => WR_COUNT_n0000(1),
      SRST => WR_COUNT_0_n0000,
      CE => Q_n0380,
      CLK => clk180,
      O => WR_COUNT(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  dqs_div_cascount_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => dqs_div_cascount_n0001(2),
      SRST => rst180_r_83,
      CE => Q_n0378,
      CLK => clk180,
      O => dqs_div_cascount(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  dqs_div_cascount_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => dqs_div_cascount_n0001(0),
      SRST => rst180_r_83,
      CE => Q_n0378,
      CLK => clk180,
      O => dqs_div_cascount(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  dqs_div_cascount_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => dqs_div_cascount_n0001(1),
      SRST => rst180_r_83,
      SSET => NlwRenamedSig_OI_high(1),
      CE => Q_n0378,
      CLK => clk180,
      O => dqs_div_cascount(1),
      SET => GND,
      RST => GND
    );
  INIT_PRE_COUNT_0 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => INIT_PRE_COUNT_n0000(0),
      SSET => rst180_r_83,
      CE => INIT_PRE_COUNT_en,
      CLK => clk180,
      O => INIT_PRE_COUNT(0),
      SET => GND,
      RST => GND,
      SRST => GND
    );
  INIT_PRE_COUNT_1 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => INIT_PRE_COUNT_n0000(1),
      SSET => rst180_r_83,
      CE => INIT_PRE_COUNT_en,
      CLK => clk180,
      O => INIT_PRE_COUNT(1),
      SET => GND,
      RST => GND,
      SRST => GND
    );
  INIT_PRE_COUNT_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => INIT_PRE_COUNT_n0000(2),
      SRST => rst180_r_83,
      CE => INIT_PRE_COUNT_en,
      CLK => clk180,
      O => INIT_PRE_COUNT(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  INIT_PRE_COUNT_3 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => INIT_PRE_COUNT_n0000(3),
      SSET => rst180_r_83,
      CE => INIT_PRE_COUNT_en,
      CLK => clk180,
      O => INIT_PRE_COUNT(3),
      SET => GND,
      RST => GND,
      SRST => GND
    );
  INIT_PRE_COUNT_6 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => INIT_PRE_COUNT_n0000(6),
      SSET => rst180_r_83,
      CE => INIT_PRE_COUNT_en,
      CLK => clk180,
      O => INIT_PRE_COUNT(6),
      SET => GND,
      RST => GND,
      SRST => GND
    );
  INIT_PRE_COUNT_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => INIT_PRE_COUNT_n0000(4),
      SRST => rst180_r_83,
      CE => INIT_PRE_COUNT_en,
      CLK => clk180,
      O => INIT_PRE_COUNT(4),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  INIT_PRE_COUNT_5 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => INIT_PRE_COUNT_n0000(5),
      SSET => rst180_r_83,
      CE => INIT_PRE_COUNT_en,
      CLK => clk180,
      O => INIT_PRE_COUNT(5),
      SET => GND,
      RST => GND,
      SRST => GND
    );
  INIT_COUNT_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => INIT_COUNT_n0000(0),
      SRST => rst180_r_83,
      CE => Q_n0381_81,
      CLK => clk180,
      O => INIT_COUNT(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  INIT_COUNT_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => INIT_COUNT_n0000(1),
      SRST => rst180_r_83,
      CE => Q_n0381_81,
      CLK => clk180,
      O => INIT_COUNT(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  INIT_COUNT_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => INIT_COUNT_n0000(2),
      SRST => rst180_r_83,
      CE => Q_n0381_81,
      CLK => clk180,
      O => INIT_COUNT(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  INIT_COUNT_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => INIT_COUNT_n0000(3),
      SRST => rst180_r_83,
      CE => Q_n0381_81,
      CLK => clk180,
      O => INIT_COUNT(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  ddr_cke_cntrl1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => wait_200us,
      O => ddr_cke_cntrl,
      ADR1 => GND
    );
  Q_n00311 : X_LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      ADR0 => Auto_Ref_issued_35,
      ADR1 => NlwRenamedSig_OI_auto_ref_req,
      O => Q_n0031
    );
  Q_n02351 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => Auto_Ref_issued_35,
      ADR1 => rst180_r_83,
      O => Q_n0235
    );
  Q_n02421 : X_LUT4
    generic map(
      INIT => X"FFFD"
    )
    port map (
      ADR0 => dqs_div_rdburstcount(0),
      ADR1 => dqs_div_cascount(2),
      ADR2 => dqs_div_rdburstcount(1),
      ADR3 => dqs_div_rdburstcount(2),
      O => N10
    );
  Q_n02422 : X_LUT3
    generic map(
      INIT => X"01"
    )
    port map (
      ADR0 => dqs_div_cascount(0),
      ADR1 => dqs_div_cascount(1),
      ADR2 => N10,
      O => Q_n0242
    );
  Q_n02431 : X_LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      ADR0 => AUTO_REF_detect_33,
      ADR1 => AUTO_REF_pulse_end_84,
      ADR2 => NlwRenamedSig_OI_auto_ref_req,
      O => Q_n0243
    );
  Q_n02441 : X_LUT4
    generic map(
      INIT => X"FFFD"
    )
    port map (
      ADR0 => INIT_COUNT(3),
      ADR1 => INIT_COUNT(2),
      ADR2 => MRD_COUNT(0),
      ADR3 => MRD_COUNT(1),
      O => N13
    );
  Q_n02442 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => INIT_COUNT(1),
      ADR1 => N13,
      ADR2 => INIT_COUNT(0),
      O => Q_n0244
    );
  Q_n02471 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => RFC_COUNT(3),
      ADR1 => RFC_COUNT(4),
      ADR2 => RFC_COUNT(5),
      ADR3 => rst180_r_83,
      O => N15
    );
  Q_n02472 : X_LUT4
    generic map(
      INIT => X"FFFD"
    )
    port map (
      ADR0 => RFC_COUNT(1),
      ADR1 => RFC_COUNT(0),
      ADR2 => RFC_COUNT(2),
      ADR3 => N15,
      O => Q_n0247
    );
  Q_n02851 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => INIT_PRE_COUNT(3),
      ADR1 => INIT_PRE_COUNT(4),
      ADR2 => INIT_PRE_COUNT(5),
      ADR3 => INIT_PRE_COUNT(6),
      O => N19
    );
  Q_n02852 : X_LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      ADR0 => INIT_MEMORY_61,
      ADR1 => INIT_PRE_COUNT(1),
      ADR2 => INIT_PRE_COUNT(2),
      ADR3 => N19,
      O => INIT_PRE_COUNT_en
    );
  Q_n02921 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => RRD_COUNT(0),
      ADR1 => RRD_COUNT(1),
      ADR2 => BA_address_conflict_51,
      O => CONFLICT_value
    );
  Q_n02971 : X_LUT4
    generic map(
      INIT => X"0010"
    )
    port map (
      ADR0 => command_register(3),
      ADR1 => command_register(0),
      ADR2 => command_register(1),
      ADR3 => command_register(2),
      O => initialize_memory
    );
  Q_n03801 : X_LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      ADR0 => WR_COUNT(0),
      ADR1 => WR_COUNT(1),
      ADR2 => WR_COUNT(2),
      O => Q_n0380
    );
  Q_n03851 : X_LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      ADR0 => dqs_div_rdburstcount(0),
      ADR1 => dqs_div_rdburstcount(1),
      ADR2 => dqs_div_rdburstcount(2),
      O => Q_n0385
    );
  ODT_COUNT_value_0_1 : X_LUT3
    generic map(
      INIT => X"F4"
    )
    port map (
      ADR0 => ODT_COUNT(0),
      ADR1 => ODT_COUNT(1),
      ADR2 => GO_TO_ODT_ON_38,
      O => ODT_COUNT_value(0)
    );
  current_state_FFd3_In1 : X_LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      ADR0 => current_state_FFd4_70,
      ADR1 => wrburst_end_1_85,
      ADR2 => wrburst_end_2_87,
      ADR3 => wrburst_end_3_88,
      O => N29
    );
  current_state_FFd3_In2 : X_LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      ADR0 => current_state_FFd1_75,
      ADR1 => rdburst_end_1_90,
      ADR2 => N29,
      ADR3 => rdburst_end_2_91,
      O => current_state_FFd3_In
    );
  dqs_div_rdburstcount_Msub_n0000_n00081 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => dqs_div_rdburstcount(0),
      O => dqs_div_rdburstcount_n0000(0),
      ADR1 => GND
    );
  dqs_div_rdburstcount_Msub_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"A9"
    )
    port map (
      ADR0 => dqs_div_rdburstcount(2),
      ADR1 => dqs_div_rdburstcount(0),
      ADR2 => dqs_div_rdburstcount(1),
      O => dqs_div_rdburstcount_n0000(2)
    );
  dqs_div_rdburstcount_Msub_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      ADR0 => dqs_div_rdburstcount(0),
      ADR1 => dqs_div_rdburstcount(1),
      O => dqs_div_rdburstcount_n0000(1)
    );
  dqs_div_rdburstcount_n00001 : X_LUT4
    generic map(
      INIT => X"FF02"
    )
    port map (
      ADR0 => dqs_div_cascount(0),
      ADR1 => dqs_div_cascount(1),
      ADR2 => dqs_div_cascount(2),
      ADR3 => rst180_r_83,
      O => dqs_div_rdburstcount_0_n0000
    );
  WR_COUNT_Msub_n0000_n00081 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => WR_COUNT(0),
      O => WR_COUNT_n0000(0),
      ADR1 => GND
    );
  WR_COUNT_Msub_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"A9"
    )
    port map (
      ADR0 => WR_COUNT(2),
      ADR1 => WR_COUNT(0),
      ADR2 => WR_COUNT(1),
      O => WR_COUNT_n0000(2)
    );
  WR_COUNT_Msub_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      ADR0 => WR_COUNT(0),
      ADR1 => WR_COUNT(1),
      O => WR_COUNT_n0000(1)
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_0_xor11 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => INIT_PRE_COUNT(0),
      O => INIT_PRE_COUNT_n0000(0),
      ADR1 => GND
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_1_xor11 : X_LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      ADR0 => INIT_PRE_COUNT(0),
      ADR1 => INIT_PRE_COUNT(1),
      O => INIT_PRE_COUNT_n0000(1)
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_2_xor11 : X_LUT3
    generic map(
      INIT => X"A9"
    )
    port map (
      ADR0 => INIT_PRE_COUNT(2),
      ADR1 => INIT_PRE_COUNT(0),
      ADR2 => INIT_PRE_COUNT(1),
      O => INIT_PRE_COUNT_n0000(2)
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_3_xor11 : X_LUT4
    generic map(
      INIT => X"AAA9"
    )
    port map (
      ADR0 => INIT_PRE_COUNT(3),
      ADR1 => INIT_PRE_COUNT(0),
      ADR2 => INIT_PRE_COUNT(1),
      ADR3 => INIT_PRE_COUNT(2),
      O => INIT_PRE_COUNT_n0000(3)
    );
  INIT_COUNT_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => INIT_COUNT(0),
      O => INIT_COUNT_n0000(0),
      ADR1 => GND
    );
  INIT_COUNT_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => INIT_COUNT(0),
      ADR1 => INIT_COUNT(3),
      ADR2 => INIT_COUNT(1),
      ADR3 => INIT_COUNT(2),
      O => INIT_COUNT_n0000(3)
    );
  INIT_COUNT_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => INIT_COUNT(0),
      ADR1 => INIT_COUNT(1),
      ADR2 => INIT_COUNT(2),
      O => INIT_COUNT_n0000(2)
    );
  INIT_COUNT_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => INIT_COUNT(0),
      ADR1 => INIT_COUNT(1),
      O => INIT_COUNT_n0000(1)
    );
  ODT_COUNT_value_1_1 : X_LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      ADR0 => ODT_COUNT(0),
      ADR1 => ODT_COUNT(1),
      ADR2 => GO_TO_ODT_ON_38,
      O => ODT_COUNT_value(1)
    );
  DLL_RST_COUNT_value_1_1 : X_LUT4
    generic map(
      INIT => X"EFFF"
    )
    port map (
      ADR0 => INIT_COUNT(3),
      ADR1 => INIT_COUNT(2),
      ADR2 => INIT_COUNT(1),
      ADR3 => INIT_MEMORY_61,
      O => N48
    );
  DLL_RST_COUNT_value_1_2 : X_LUT4
    generic map(
      INIT => X"9990"
    )
    port map (
      ADR0 => DLL_RST_COUNT(0),
      ADR1 => DLL_RST_COUNT(1),
      ADR2 => INIT_COUNT(0),
      ADR3 => N48,
      O => DLL_RST_COUNT_value(1)
    );
  dqs_reset1 : X_MUX2
    port map (
      IA => DQS_reset1_clk0_rt_104,
      IB => N53,
      SEL => cas_latency(0),
      O => dqs_reset
    );
  dqs_reset1_G : X_LUT4
    generic map(
      INIT => X"EF40"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_high(1),
      ADR1 => DQS_reset2_clk0_49,
      ADR2 => cas_latency(1),
      ADR3 => DQS_reset1_clk0_46,
      O => N53
    );
  Q_n03021 : X_MUX2
    port map (
      IA => NlwRenamedSig_OI_high(1),
      IB => N55,
      SEL => cas_latency(0),
      O => ack_o
    );
  Q_n03021_G : X_LUT4
    generic map(
      INIT => X"2220"
    )
    port map (
      ADR0 => cas_latency(1),
      ADR1 => NlwRenamedSig_OI_high(1),
      ADR2 => read_cmd1_39,
      ADR3 => write_cmd4_56,
      O => N55
    );
  current_state_FFd2_In1 : X_MUX2
    port map (
      IA => current_state_FFd3_rt_105,
      IB => N57,
      SEL => current_state_FFd2_74,
      O => current_state_FFd2_In
    );
  current_state_FFd2_In1_G : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => WR_COUNT(0),
      ADR1 => WR_COUNT(1),
      ADR2 => WR_COUNT(2),
      ADR3 => current_state_FFd3_71,
      O => N57
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_4_xor11 : X_MUX2
    port map (
      IA => N58,
      IB => INIT_PRE_COUNT_4_rt_106,
      SEL => INIT_PRE_COUNT(1),
      O => INIT_PRE_COUNT_n0000(4)
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_4_xor11_F : X_LUT4
    generic map(
      INIT => X"AAA9"
    )
    port map (
      ADR0 => INIT_PRE_COUNT(4),
      ADR1 => INIT_PRE_COUNT(0),
      ADR2 => INIT_PRE_COUNT(2),
      ADR3 => INIT_PRE_COUNT(3),
      O => N58
    );
  ddr_ba1_1_1 : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => INIT_COUNT(3),
      ADR1 => INIT_COUNT(1),
      ADR2 => INIT_COUNT(2),
      ADR3 => current_state_FFd12_64,
      O => ddr_ba_cntrl(1)
    );
  Q_n02481 : X_LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      ADR0 => dqs_div_cascount(0),
      ADR1 => dqs_div_cascount(2),
      ADR2 => dqs_div_cascount(1),
      O => Q_n0248
    );
  Ker631 : X_LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      ADR0 => current_state_FFd1_75,
      ADR1 => rdburst_end_2_91,
      ADR2 => rdburst_end_1_90,
      O => N63
    );
  Ker731 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => N35,
      ADR1 => wrburst_end_cnt(1),
      ADR2 => wrburst_end_cnt(0),
      O => N73
    );
  ddr_address1_1_1 : X_LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      ADR0 => Q_n0337,
      ADR1 => current_state_FFd12_64,
      ADR2 => burst_length(1),
      O => ddr_address_cntrl(1)
    );
  Ker171 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => RFC_COUNT(5),
      ADR1 => RFC_COUNT(4),
      ADR2 => RFC_COUNT(3),
      ADR3 => RFC_COUNT(2),
      O => N171
    );
  current_state_FFd4_In1 : X_LUT4
    generic map(
      INIT => X"FF8A"
    )
    port map (
      ADR0 => N35,
      ADR1 => wrburst_end_cnt(1),
      ADR2 => wrburst_end_cnt(0),
      ADR3 => N7,
      O => current_state_FFd4_In
    );
  current_state_FFd1_In1 : X_LUT4
    generic map(
      INIT => X"FF8A"
    )
    port map (
      ADR0 => N63,
      ADR1 => CAS_COUNT(1),
      ADR2 => CAS_COUNT(0),
      ADR3 => current_state_FFd6_65,
      O => current_state_FFd1_In
    );
  Q_n02871 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => N35,
      ADR1 => wrburst_end_cnt(0),
      ADR2 => N7,
      ADR3 => wrburst_end_cnt(1),
      O => Write_enable_out
    );
  ddr_ba1_0_SW0 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => INIT_COUNT(3),
      ADR1 => INIT_COUNT(0),
      O => N11
    );
  ddr_ba1_0_Q : X_LUT4
    generic map(
      INIT => X"4020"
    )
    port map (
      ADR0 => INIT_COUNT(2),
      ADR1 => INIT_COUNT(1),
      ADR2 => current_state_FFd12_64,
      ADR3 => N11,
      O => ddr_ba_cntrl(0)
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_6_xor11 : X_LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      ADR0 => INIT_PRE_COUNT(6),
      ADR1 => N191,
      O => INIT_PRE_COUNT_n0000(6)
    );
  Q_n03011 : X_LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      ADR0 => wrburst_end_cnt(0),
      ADR1 => wrburst_end_cnt(1),
      ADR2 => DQS_reset_out,
      O => DQS_enable_out
    );
  wrburst_end_cnt_value_0_1 : X_LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      ADR0 => wrburst_end_cnt(1),
      ADR1 => wrburst_end_cnt(0),
      ADR2 => DQS_reset_out,
      O => wrburst_end_cnt_value(0)
    );
  DLL_RST_COUNT_value_3_1 : X_LUT3
    generic map(
      INIT => X"F9"
    )
    port map (
      ADR0 => DLL_RST_COUNT(3),
      ADR1 => N241,
      ADR2 => N25,
      O => DLL_RST_COUNT_value(3)
    );
  Q_n02951 : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => N351,
      ADR1 => Q_n0322,
      ADR2 => INIT_COUNT(2),
      ADR3 => INIT_COUNT(1),
      O => INIT_DONE_value
    );
  DLL_RST_COUNT_value_6_1 : X_LUT4
    generic map(
      INIT => X"FF41"
    )
    port map (
      ADR0 => Q_n0322,
      ADR1 => DLL_RST_COUNT(6),
      ADR2 => N12,
      ADR3 => N25,
      O => DLL_RST_COUNT_value(6)
    );
  DLL_RST_COUNT_value_2_1 : X_LUT4
    generic map(
      INIT => X"4441"
    )
    port map (
      ADR0 => N25,
      ADR1 => DLL_RST_COUNT(2),
      ADR2 => DLL_RST_COUNT(0),
      ADR3 => DLL_RST_COUNT(1),
      O => DLL_RST_COUNT_value(2)
    );
  Ker25_SW0 : X_LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      ADR0 => INIT_COUNT(0),
      ADR1 => INIT_MEMORY_61,
      O => N21
    );
  Ker25 : X_LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      ADR0 => INIT_COUNT(1),
      ADR1 => INIT_COUNT(3),
      ADR2 => INIT_COUNT(2),
      ADR3 => N21,
      O => N25
    );
  Ker19_SW0 : X_LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      ADR0 => INIT_PRE_COUNT(1),
      ADR1 => INIT_PRE_COUNT(0),
      ADR2 => INIT_PRE_COUNT(2),
      O => N23
    );
  Ker19 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => INIT_PRE_COUNT(5),
      ADR1 => INIT_PRE_COUNT(4),
      ADR2 => INIT_PRE_COUNT(3),
      ADR3 => N23,
      O => N191
    );
  Q_n032211 : X_LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      ADR0 => DLL_RST_COUNT(0),
      ADR1 => DLL_RST_COUNT(1),
      ADR2 => DLL_RST_COUNT(3),
      ADR3 => DLL_RST_COUNT(2),
      O => N36
    );
  Q_n032224 : X_LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      ADR0 => DLL_RST_COUNT(7),
      ADR1 => DLL_RST_COUNT(6),
      ADR2 => DLL_RST_COUNT(5),
      ADR3 => DLL_RST_COUNT(4),
      O => N49
    );
  Q_n032225 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => N36,
      ADR1 => N49,
      O => Q_n0322
    );
  Q_n03421 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => current_state_FFd12_64,
      ADR1 => current_state_FFd13_66,
      O => Q_n0342
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_5_xor1_SW0 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => INIT_PRE_COUNT(3),
      ADR1 => INIT_PRE_COUNT(1),
      ADR2 => INIT_PRE_COUNT(0),
      ADR3 => INIT_PRE_COUNT(2),
      O => N54
    );
  INIT_PRE_COUNT_LPM_COUNTER_4_n0000_5_xor1 : X_LUT4
    generic map(
      INIT => X"AF8F"
    )
    port map (
      ADR0 => INIT_PRE_COUNT(5),
      ADR1 => INIT_PRE_COUNT(4),
      ADR2 => N191,
      ADR3 => N54,
      O => INIT_PRE_COUNT_n0000(5)
    );
  current_state_FFd8_In_SW0 : X_LUT4
    generic map(
      INIT => X"EFFF"
    )
    port map (
      ADR0 => RCDR_COUNT(1),
      ADR1 => RCDR_COUNT(2),
      ADR2 => read_cmd5_43,
      ADR3 => RCDR_COUNT(0),
      O => N56
    );
  current_state_FFd8_In : X_LUT4
    generic map(
      INIT => X"FF80"
    )
    port map (
      ADR0 => N6,
      ADR1 => current_state_FFd8_69,
      ADR2 => N56,
      ADR3 => current_state_FFd9_67,
      O => current_state_FFd8_In_92
    );
  current_state_FFd7_In_SW0 : X_LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      ADR0 => RCDW_COUNT(1),
      ADR1 => RCDW_COUNT(0),
      O => N60
    );
  current_state_FFd7_In : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => RCDW_COUNT(2),
      ADR1 => current_state_FFd8_69,
      ADR2 => N60,
      ADR3 => write_cmd3_55,
      O => DQS_reset_out
    );
  cas_count_value_0_1 : X_LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      ADR0 => CAS_COUNT(1),
      ADR1 => N26,
      ADR2 => CAS_COUNT(0),
      O => cas_count_value(0)
    );
  Ker2411 : X_LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      ADR0 => DLL_RST_COUNT(2),
      ADR1 => DLL_RST_COUNT(1),
      ADR2 => DLL_RST_COUNT(0),
      O => N241
    );
  cas_count_value_1_1 : X_LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      ADR0 => CAS_COUNT(0),
      ADR1 => CAS_COUNT(1),
      ADR2 => N26,
      ADR3 => N63,
      O => cas_count_value(1)
    );
  current_state_FFd6_In1 : X_LUT4
    generic map(
      INIT => X"FF20"
    )
    port map (
      ADR0 => N63,
      ADR1 => CAS_COUNT(1),
      ADR2 => CAS_COUNT(0),
      ADR3 => N26,
      O => Q_n0217
    );
  Ker61 : X_LUT4
    generic map(
      INIT => X"EFFF"
    )
    port map (
      ADR0 => RCDW_COUNT(2),
      ADR1 => RCDW_COUNT(1),
      ADR2 => write_cmd3_55,
      ADR3 => RCDW_COUNT(0),
      O => N6
    );
  Ker352 : X_LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      ADR0 => current_state_FFd4_70,
      ADR1 => wrburst_end_1_85,
      ADR2 => wrburst_end_3_88,
      ADR3 => wrburst_end_2_87,
      O => N35
    );
  ddr_address1_5_Q : X_LUT4
    generic map(
      INIT => X"FF80"
    )
    port map (
      ADR0 => cas_latency(1),
      ADR1 => Q_n0337,
      ADR2 => current_state_FFd12_64,
      ADR3 => N691,
      O => ddr_address_cntrl(5)
    );
  ddr_address1_4_Q : X_LUT4
    generic map(
      INIT => X"FF80"
    )
    port map (
      ADR0 => cas_latency(0),
      ADR1 => Q_n0337,
      ADR2 => current_state_FFd12_64,
      ADR3 => N71,
      O => ddr_address_cntrl(4)
    );
  wrburst_end_cnt_value_1_SW0 : X_LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      ADR0 => burst_length(1),
      ADR1 => N35,
      ADR2 => wrburst_end_cnt(1),
      O => N731
    );
  wrburst_end_cnt_value_1_Q : X_LUT4
    generic map(
      INIT => X"EA40"
    )
    port map (
      ADR0 => DQS_reset_out,
      ADR1 => N731,
      ADR2 => wrburst_end_cnt(0),
      ADR3 => burst_length(1),
      O => wrburst_end_cnt_value(1)
    );
  Ker26_SW0 : X_LUT3
    generic map(
      INIT => X"BF"
    )
    port map (
      ADR0 => RCDR_COUNT(2),
      ADR1 => RCDR_COUNT(0),
      ADR2 => N6,
      O => N75
    );
  Ker26 : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => RCDR_COUNT(1),
      ADR1 => current_state_FFd8_69,
      ADR2 => N75,
      ADR3 => read_cmd5_43,
      O => N26
    );
  current_state_FFd10_In2 : X_LUT3
    generic map(
      INIT => X"C8"
    )
    port map (
      ADR0 => ODT_COUNT(0),
      ADR1 => current_state_FFd10_73,
      ADR2 => ODT_COUNT(1),
      O => current_state_FFd10_In_map75
    );
  current_state_FFd10_In10 : X_LUT4
    generic map(
      INIT => X"2220"
    )
    port map (
      ADR0 => current_state_FFd14_72,
      ADR1 => INIT_MEMORY_61,
      ADR2 => GO_TO_ODT_ON_38,
      ADR3 => CONFLICT_34,
      O => current_state_FFd10_In_map79
    );
  current_state_FFd10_In17 : X_LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      ADR0 => N0,
      ADR1 => current_state_FFd10_In_map79,
      ADR2 => current_state_FFd10_In_map75,
      O => current_state_FFd10_In
    );
  DLL_RST_COUNT_value_4_Q : X_LUT4
    generic map(
      INIT => X"0401"
    )
    port map (
      ADR0 => Q_n0322,
      ADR1 => DLL_RST_COUNT(4),
      ADR2 => N25,
      ADR3 => N155,
      O => DLL_RST_COUNT_value(4)
    );
  DLL_RST_COUNT_value_5_SW0 : X_LUT4
    generic map(
      INIT => X"01FF"
    )
    port map (
      ADR0 => N241,
      ADR1 => DLL_RST_COUNT(4),
      ADR2 => DLL_RST_COUNT(3),
      ADR3 => DLL_RST_COUNT(5),
      O => N157
    );
  DLL_RST_COUNT_value_5_Q : X_LUT4
    generic map(
      INIT => X"0103"
    )
    port map (
      ADR0 => N12,
      ADR1 => Q_n0322,
      ADR2 => N25,
      ADR3 => N157,
      O => DLL_RST_COUNT_value(5)
    );
  dqs_div_cascount_n0001_0_1 : X_LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      ADR0 => Q_n0238_76,
      ADR1 => dqs_div_cascount(0),
      O => dqs_div_cascount_n0001(0)
    );
  dqs_div_cascount_n0001_1_1 : X_LUT3
    generic map(
      INIT => X"F9"
    )
    port map (
      ADR0 => dqs_div_cascount(0),
      ADR1 => dqs_div_cascount(1),
      ADR2 => Q_n0238_76,
      O => dqs_div_cascount_n0001(1)
    );
  Q_n03371 : X_LUT4
    generic map(
      INIT => X"DDD9"
    )
    port map (
      ADR0 => INIT_COUNT(1),
      ADR1 => INIT_COUNT(2),
      ADR2 => INIT_COUNT(3),
      ADR3 => INIT_COUNT(0),
      O => Q_n0337
    );
  dqs_div_cascount_n0001_2_1 : X_LUT4
    generic map(
      INIT => X"4441"
    )
    port map (
      ADR0 => Q_n0238_76,
      ADR1 => dqs_div_cascount(2),
      ADR2 => dqs_div_cascount(0),
      ADR3 => dqs_div_cascount(1),
      O => dqs_div_cascount_n0001(2)
    );
  Q_n03781 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => dqs_div_cascount(2),
      ADR1 => dqs_div_cascount(1),
      ADR2 => Q_n0238_76,
      ADR3 => dqs_div_cascount(0),
      O => Q_n0378
    );
  Q_n0238 : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => N7,
      ADR1 => cas_latency(1),
      ADR2 => N165,
      ADR3 => cas_latency(0),
      O => Q_n0238_76
    );
  Ker661 : X_LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_auto_ref_req,
      ADR1 => Q_n0181,
      ADR2 => RFC_COUNT_reg_44,
      ADR3 => current_state_FFd14_72,
      O => accept_cmd_in
    );
  Q_n0282_SW0 : X_LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      ADR0 => INIT_COUNT(1),
      ADR1 => INIT_COUNT(0),
      O => N175
    );
  Q_n0282 : X_LUT4
    generic map(
      INIT => X"AB8A"
    )
    port map (
      ADR0 => current_state_FFd12_64,
      ADR1 => N175,
      ADR2 => INIT_COUNT(3),
      ADR3 => INIT_COUNT(2),
      O => Q_n0282_79
    );
  Ker12_SW0 : X_LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      ADR0 => DLL_RST_COUNT(0),
      ADR1 => DLL_RST_COUNT(3),
      ADR2 => DLL_RST_COUNT(2),
      O => N177
    );
  Ker12 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => DLL_RST_COUNT(5),
      ADR1 => DLL_RST_COUNT(4),
      ADR2 => DLL_RST_COUNT(1),
      ADR3 => N177,
      O => N12
    );
  RFC_COUNT_value_5_SW0 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => RFC_COUNT(4),
      ADR1 => RFC_COUNT(2),
      ADR2 => RFC_COUNT(1),
      ADR3 => RFC_COUNT(0),
      O => N179
    );
  RFC_COUNT_value_5_Q : X_LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      ADR0 => RFC_COUNT(5),
      ADR1 => RFC_COUNT(3),
      ADR2 => Auto_Ref_issued_p,
      ADR3 => N179,
      O => RFC_COUNT_value(5)
    );
  RFC_COUNT_value_2_5 : X_LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      ADR0 => RFC_COUNT(3),
      ADR1 => RFC_COUNT(4),
      ADR2 => RFC_COUNT(5),
      O => RFC_COUNT_value_2_map254
    );
  RFC_COUNT_value_4_SW0 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => RFC_COUNT(3),
      ADR1 => RFC_COUNT(2),
      ADR2 => RFC_COUNT(1),
      ADR3 => RFC_COUNT(0),
      O => N239
    );
  RFC_COUNT_value_4_Q : X_LUT4
    generic map(
      INIT => X"4140"
    )
    port map (
      ADR0 => Auto_Ref_issued_p,
      ADR1 => RFC_COUNT(4),
      ADR2 => N239,
      ADR3 => RFC_COUNT(5),
      O => RFC_COUNT_value(4)
    );
  Ker3611 : X_LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      ADR0 => AUTOREF_COUNT(3),
      ADR1 => AUTOREF_COUNT(4),
      ADR2 => AUTOREF_COUNT(6),
      ADR3 => AUTOREF_COUNT(5),
      O => Ker36_map330
    );
  Ker3620 : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => AUTOREF_COUNT(9),
      ADR1 => AUTOREF_COUNT(10),
      ADR2 => AUTOREF_COUNT(7),
      ADR3 => AUTOREF_COUNT(8),
      O => Ker36_map335
    );
  Ker3629 : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => AUTOREF_COUNT(11),
      ADR1 => AUTOREF_COUNT(1),
      ADR2 => AUTOREF_COUNT(0),
      ADR3 => AUTOREF_COUNT(2),
      O => Ker36_map340
    );
  Ker3640 : X_LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      ADR0 => Ker36_map330,
      ADR1 => Ker36_map335,
      ADR2 => Ker36_map340,
      O => AUTOREF_value
    );
  ddr_address1_8_22 : X_LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      ADR0 => Q_n0282_79,
      ADR1 => Q_n0337,
      ADR2 => LMR_DLL_rst_8_Q,
      O => ddr_address1_8_map352
    );
  ddr_address1_8_46 : X_LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      ADR0 => Q_n0342,
      ADR1 => ddr_address1_8_map350,
      ADR2 => ddr_address1_8_map343,
      ADR3 => ddr_address1_8_map352,
      O => ddr_address_cntrl(8)
    );
  Ker501 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => INIT_MEMORY_61,
      ADR1 => current_state_FFd14_72,
      O => N501
    );
  RFC_COUNT_value_0_1 : X_LUT4
    generic map(
      INIT => X"FF54"
    )
    port map (
      ADR0 => RFC_COUNT(0),
      ADR1 => N171,
      ADR2 => RFC_COUNT(1),
      ADR3 => Auto_Ref_issued_p,
      O => RFC_COUNT_value(0)
    );
  RP_cnt_value_2_1 : X_LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      ADR0 => RP_COUNT(2),
      ADR1 => RP_COUNT(0),
      ADR2 => Q_n0215,
      ADR3 => RP_COUNT(1),
      O => RP_cnt_value(2)
    );
  MRD_COUNT_value_0_1 : X_LUT4
    generic map(
      INIT => X"F444"
    )
    port map (
      ADR0 => MRD_COUNT(0),
      ADR1 => MRD_COUNT(1),
      ADR2 => N1,
      ADR3 => N501,
      O => MRD_COUNT_value(0)
    );
  RFC_COUNT_value_1_1 : X_LUT4
    generic map(
      INIT => X"EBEA"
    )
    port map (
      ADR0 => Auto_Ref_issued_p,
      ADR1 => RFC_COUNT(0),
      ADR2 => RFC_COUNT(1),
      ADR3 => N171,
      O => RFC_COUNT_value(1)
    );
  RP_cnt_value_1_1 : X_LUT4
    generic map(
      INIT => X"4140"
    )
    port map (
      ADR0 => Q_n0215,
      ADR1 => RP_COUNT(0),
      ADR2 => RP_COUNT(1),
      ADR3 => RP_COUNT(2),
      O => RP_cnt_value(1)
    );
  Q_n0250_SW0 : X_LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      ADR0 => burst_length(1),
      ADR1 => CAS_COUNT(0),
      O => N317
    );
  Q_n0250 : X_LUT4
    generic map(
      INIT => X"FF5D"
    )
    port map (
      ADR0 => burst_done,
      ADR1 => N317,
      ADR2 => CAS_COUNT(1),
      ADR3 => rst180_r_83,
      O => Q_n0250_78
    );
  Q_n0245_SW0 : X_LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      ADR0 => burst_length(1),
      ADR1 => wrburst_end_cnt(0),
      O => N319
    );
  Q_n0245 : X_LUT4
    generic map(
      INIT => X"FF5D"
    )
    port map (
      ADR0 => burst_done,
      ADR1 => N319,
      ADR2 => wrburst_end_cnt(1),
      ADR3 => rst180_r_83,
      O => Q_n0245_77
    );
  Ker71 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => current_state_FFd5_62,
      ADR1 => current_state_FFd7_63,
      O => N7
    );
  Ker591 : X_LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      ADR0 => MRD_COUNT(1),
      ADR1 => MRD_COUNT(0),
      O => N59
    );
  RRD_COUNT_value_1_1 : X_LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      ADR0 => RRD_COUNT(0),
      ADR1 => RRD_COUNT(1),
      ADR2 => Q_n0200,
      O => RRD_COUNT_value(1)
    );
  RRD_COUNT_value_0_1 : X_LUT3
    generic map(
      INIT => X"04"
    )
    port map (
      ADR0 => RRD_COUNT(0),
      ADR1 => RRD_COUNT(1),
      ADR2 => Q_n0200,
      O => RRD_COUNT_value(0)
    );
  Ker01 : X_LUT3
    generic map(
      INIT => X"7F"
    )
    port map (
      ADR0 => auto_ref_89,
      ADR1 => RFC_COUNT_reg_44,
      ADR2 => Q_n0181,
      O => N0
    );
  RCDW_COUNT_value_0_1 : X_LUT4
    generic map(
      INIT => X"0302"
    )
    port map (
      ADR0 => RCDW_COUNT(2),
      ADR1 => RCDW_COUNT(0),
      ADR2 => Q_n0200,
      ADR3 => RCDW_COUNT(1),
      O => RCDW_COUNT_value(0)
    );
  RCDW_COUNT_value_2_1 : X_LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      ADR0 => RCDW_COUNT(2),
      ADR1 => RCDW_COUNT(0),
      ADR2 => Q_n0200,
      ADR3 => RCDW_COUNT(1),
      O => RCDW_COUNT_value(2)
    );
  RCDR_COUNT_value_0_1 : X_LUT4
    generic map(
      INIT => X"FF54"
    )
    port map (
      ADR0 => RCDR_COUNT(0),
      ADR1 => RCDR_COUNT(1),
      ADR2 => RCDR_COUNT(2),
      ADR3 => Q_n0200,
      O => RCDR_COUNT_value(0)
    );
  RCDR_COUNT_value_2_1 : X_LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      ADR0 => RCDR_COUNT(2),
      ADR1 => RCDR_COUNT(0),
      ADR2 => Q_n0200,
      ADR3 => RCDR_COUNT(1),
      O => RCDR_COUNT_value(2)
    );
  Ker651 : X_LUT4
    generic map(
      INIT => X"0010"
    )
    port map (
      ADR0 => CONFLICT_34,
      ADR1 => GO_TO_ODT_ON_38,
      ADR2 => N0,
      ADR3 => INIT_MEMORY_61,
      O => N65
    );
  RCDR_COUNT_value_1_1 : X_LUT4
    generic map(
      INIT => X"0C02"
    )
    port map (
      ADR0 => RCDR_COUNT(2),
      ADR1 => RCDR_COUNT(1),
      ADR2 => Q_n0200,
      ADR3 => RCDR_COUNT(0),
      O => RCDR_COUNT_value(1)
    );
  RCDW_COUNT_value_1_1 : X_LUT4
    generic map(
      INIT => X"0C02"
    )
    port map (
      ADR0 => RCDW_COUNT(2),
      ADR1 => RCDW_COUNT(1),
      ADR2 => Q_n0200,
      ADR3 => RCDW_COUNT(0),
      O => RCDW_COUNT_value(1)
    );
  Q_n0381 : X_LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      ADR0 => INIT_MEMORY_61,
      ADR1 => Q_n0215,
      ADR2 => Auto_Ref_issued_p,
      ADR3 => N339,
      O => Q_n0381_81
    );
  Ker69_SW1 : X_LUT3
    generic map(
      INIT => X"FB"
    )
    port map (
      ADR0 => INIT_COUNT(1),
      ADR1 => INIT_COUNT(0),
      ADR2 => INIT_COUNT(2),
      O => N343
    );
  current_state_FFd11_In_SW1 : X_LUT4
    generic map(
      INIT => X"DFFF"
    )
    port map (
      ADR0 => current_state_FFd14_72,
      ADR1 => INIT_COUNT(3),
      ADR2 => INIT_COUNT(2),
      ADR3 => INIT_COUNT(1),
      O => N346
    );
  current_state_FFd13_In36 : X_LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      ADR0 => INIT_PRE_COUNT(6),
      ADR1 => INIT_PRE_COUNT(3),
      ADR2 => INIT_COUNT(2),
      ADR3 => INIT_PRE_COUNT(2),
      O => current_state_FFd13_In_map562
    );
  current_state_FFd13_In100 : X_LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      ADR0 => current_state_FFd2_74,
      ADR1 => WR_COUNT(0),
      ADR2 => WR_COUNT(1),
      ADR3 => WR_COUNT(2),
      O => current_state_FFd13_In_map575
    );
  current_state_FFd13_In103 : X_LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      ADR0 => current_state_FFd13_In_map568,
      ADR1 => current_state_FFd13_In_map547,
      ADR2 => current_state_FFd13_In_map575,
      ADR3 => current_state_FFd13_In_map563,
      O => Q_n0215
    );
  ddr_address1_10_SW1 : X_LUT4
    generic map(
      INIT => X"8C80"
    )
    port map (
      ADR0 => LMR_DLL_rst_10_Q,
      ADR1 => Q_n0342,
      ADR2 => Q_n0282_79,
      ADR3 => LMR_DLL_set(10),
      O => N425
    );
  ddr_address1_10_Q : X_LUT4
    generic map(
      INIT => X"8C80"
    )
    port map (
      ADR0 => N425,
      ADR1 => Q_n0337,
      ADR2 => Q_n0283,
      ADR3 => N424,
      O => ddr_address_cntrl(10)
    );
  current_state_FFd14_In19 : X_LUT4
    generic map(
      INIT => X"FF89"
    )
    port map (
      ADR0 => INIT_COUNT(0),
      ADR1 => INIT_COUNT(1),
      ADR2 => RFC_COUNT_reg_44,
      ADR3 => INIT_COUNT(2),
      O => current_state_FFd14_In_map632
    );
  current_state_FFd14_In57 : X_LUT4
    generic map(
      INIT => X"F888"
    )
    port map (
      ADR0 => INIT_COUNT(3),
      ADR1 => current_state_FFd14_In_map632,
      ADR2 => N59,
      ADR3 => current_state_FFd14_In_map640,
      O => current_state_FFd14_In_map642
    );
  current_state_FFd14_In74 : X_LUT3
    generic map(
      INIT => X"08"
    )
    port map (
      ADR0 => INIT_COUNT(1),
      ADR1 => INIT_COUNT(2),
      ADR2 => N32,
      O => current_state_FFd14_In_map645
    );
  current_state_FFd14_In139 : X_LUT3
    generic map(
      INIT => X"01"
    )
    port map (
      ADR0 => INIT_COUNT(2),
      ADR1 => INIT_COUNT(3),
      ADR2 => INIT_COUNT(1),
      O => current_state_FFd14_In_map663
    );
  current_state_FFd14_In145 : X_LUT4
    generic map(
      INIT => X"7200"
    )
    port map (
      ADR0 => INIT_COUNT(0),
      ADR1 => Q_n0181,
      ADR2 => current_state_FFd14_In_map655,
      ADR3 => current_state_FFd14_In_map663,
      O => current_state_FFd14_In_map664
    );
  current_state_FFd14_In172 : X_LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      ADR0 => INIT_MEMORY_61,
      ADR1 => current_state_FFd14_In_map642,
      ADR2 => current_state_FFd14_In_map645,
      ADR3 => current_state_FFd14_In_map664,
      O => current_state_FFd14_In_map666
    );
  current_state_FFd14_In186 : X_LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      ADR0 => GO_TO_ACTIVE_86,
      ADR1 => N65,
      O => current_state_FFd14_In_map668
    );
  current_state_FFd14_In215 : X_LUT4
    generic map(
      INIT => X"EEEC"
    )
    port map (
      ADR0 => current_state_FFd14_72,
      ADR1 => current_state_FFd14_In_map624,
      ADR2 => current_state_FFd14_In_map668,
      ADR3 => current_state_FFd14_In_map666,
      O => current_state_FFd14_In
    );
  Ker3511 : X_LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      ADR0 => INIT_COUNT(0),
      ADR1 => INIT_COUNT(3),
      O => N351
    );
  Ker721 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => current_state_FFd12_64,
      ADR1 => current_state_FFd11_68,
      O => N72
    );
  Q_n01811 : X_LUT3
    generic map(
      INIT => X"01"
    )
    port map (
      ADR0 => RP_COUNT(2),
      ADR1 => RP_COUNT(0),
      ADR2 => RP_COUNT(1),
      O => Q_n0181
    );
  Ker321 : X_LUT3
    generic map(
      INIT => X"B8"
    )
    port map (
      ADR0 => RFC_COUNT_reg_44,
      ADR1 => INIT_COUNT(0),
      ADR2 => Q_n0181,
      O => N32
    );
  Q_n02835 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => N7,
      ADR1 => current_state_FFd14_72,
      ADR2 => current_state_FFd6_65,
      ADR3 => current_state_FFd1_75,
      O => N555
    );
  Q_n028313 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => current_state_FFd4_70,
      ADR1 => current_state_FFd3_71,
      ADR2 => current_state_FFd2_74,
      ADR3 => current_state_FFd9_67,
      O => N563
    );
  Q_n028341 : X_LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      ADR0 => N578,
      ADR1 => N555,
      ADR2 => N558,
      ADR3 => N563,
      O => Q_n0283
    );
  dqs_enable_i1 : X_LUT4
    generic map(
      INIT => X"ACCC"
    )
    port map (
      ADR0 => DQS_enable2_32,
      ADR1 => DQS_enable1_31,
      ADR2 => cas_latency(0),
      ADR3 => cas_latency(1),
      O => NlwRenamedSig_OI_dqs_enable
    );
  AUTOREF_COUNT_1_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => AUTOREF_COUNT(1),
      O => AUTOREF_COUNT_1_rt_93,
      ADR1 => GND
    );
  AUTOREF_COUNT_2_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => AUTOREF_COUNT(2),
      O => AUTOREF_COUNT_2_rt_94,
      ADR1 => GND
    );
  AUTOREF_COUNT_3_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => AUTOREF_COUNT(3),
      O => AUTOREF_COUNT_3_rt_95,
      ADR1 => GND
    );
  AUTOREF_COUNT_4_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => AUTOREF_COUNT(4),
      O => AUTOREF_COUNT_4_rt_96,
      ADR1 => GND
    );
  AUTOREF_COUNT_5_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => AUTOREF_COUNT(5),
      O => AUTOREF_COUNT_5_rt_97,
      ADR1 => GND
    );
  AUTOREF_COUNT_6_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => AUTOREF_COUNT(6),
      O => AUTOREF_COUNT_6_rt_98,
      ADR1 => GND
    );
  AUTOREF_COUNT_7_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => AUTOREF_COUNT(7),
      O => AUTOREF_COUNT_7_rt_99,
      ADR1 => GND
    );
  AUTOREF_COUNT_8_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => AUTOREF_COUNT(8),
      O => AUTOREF_COUNT_8_rt_100,
      ADR1 => GND
    );
  AUTOREF_COUNT_9_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => AUTOREF_COUNT(9),
      O => AUTOREF_COUNT_9_rt_101,
      ADR1 => GND
    );
  AUTOREF_COUNT_10_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => AUTOREF_COUNT(10),
      O => AUTOREF_COUNT_10_rt_102,
      ADR1 => GND
    );
  AUTOREF_COUNT_11_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => AUTOREF_COUNT(11),
      O => AUTOREF_COUNT_11_rt_103,
      ADR1 => GND
    );
  DQS_reset1_clk0_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => DQS_reset1_clk0_46,
      O => DQS_reset1_clk0_rt_104,
      ADR1 => GND
    );
  current_state_FFd3_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => current_state_FFd3_71,
      O => current_state_FFd3_rt_105,
      ADR1 => GND
    );
  INIT_PRE_COUNT_4_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => INIT_PRE_COUNT(4),
      O => INIT_PRE_COUNT_4_rt_106,
      ADR1 => GND
    );
  dqs_enable_i1_SW0 : X_LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      ADR0 => cas_latency(1),
      ADR1 => cas_latency(0),
      O => N586
    );
  WR_COUNT_n00001 : X_LUT4
    generic map(
      INIT => X"FBEA"
    )
    port map (
      ADR0 => rst180_r_83,
      ADR1 => N586,
      ADR2 => DQS_enable1_31,
      ADR3 => DQS_enable2_32,
      O => WR_COUNT_0_n0000
    );
  Q_n0238_SW0 : X_LUT4
    generic map(
      INIT => X"FFFD"
    )
    port map (
      ADR0 => current_state_FFd6_65,
      ADR1 => current_state_FFd13_66,
      ADR2 => current_state_FFd9_67,
      ADR3 => N72,
      O => N165
    );
  Q_n0381_SW0 : X_LUT4
    generic map(
      INIT => X"AA08"
    )
    port map (
      ADR0 => current_state_FFd14_72,
      ADR1 => N01,
      ADR2 => N59,
      ADR3 => N69,
      O => N339
    );
  Q_n02891 : X_LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      ADR0 => read_cmd1_39,
      ADR1 => command_register(1),
      ADR2 => command_register(2),
      ADR3 => accept_cmd_in,
      O => GO_TO_ACTIVE_value
    );
  Ker011 : X_LUT4
    generic map(
      INIT => X"1446"
    )
    port map (
      ADR0 => INIT_COUNT(2),
      ADR1 => INIT_COUNT(1),
      ADR2 => INIT_COUNT(0),
      ADR3 => INIT_COUNT(3),
      O => N01
    );
  Q_n03001 : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => command_register(1),
      ADR1 => command_register(2),
      ADR2 => write_cmd1_53,
      ADR3 => accept_cmd_in,
      O => GO_TO_ODT_ON_value
    );
  Q_n02911 : X_LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      ADR0 => command_register(1),
      ADR1 => command_register(2),
      ADR2 => accept_cmd_in,
      O => read_cmd
    );
  Q_n02981 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => command_register(2),
      ADR1 => command_register(1),
      ADR2 => accept_cmd_in,
      O => write_cmd_in
    );
  current_state_FFd9_In_SW0_SW0 : X_LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      ADR0 => current_state_FFd10_73,
      ADR1 => ODT_COUNT(1),
      ADR2 => ODT_COUNT(0),
      O => N590
    );
  current_state_FFd9_In : X_LUT4
    generic map(
      INIT => X"FF80"
    )
    port map (
      ADR0 => GO_TO_ACTIVE_86,
      ADR1 => current_state_FFd14_72,
      ADR2 => N65,
      ADR3 => N590,
      O => Q_n0200
    );
  AUTOREF_CNT_val_11_1 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_init,
      ADR1 => AUTOREF_value,
      ADR2 => Q_n0254(11),
      O => AUTOREF_CNT_val(11)
    );
  AUTOREF_CNT_val_10_1 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_init,
      ADR1 => AUTOREF_value,
      ADR2 => Q_n0254(10),
      O => AUTOREF_CNT_val(10)
    );
  AUTOREF_CNT_val_9_1 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_init,
      ADR1 => AUTOREF_value,
      ADR2 => Q_n0254(9),
      O => AUTOREF_CNT_val(9)
    );
  AUTOREF_CNT_val_8_1 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_init,
      ADR1 => AUTOREF_value,
      ADR2 => Q_n0254(8),
      O => AUTOREF_CNT_val(8)
    );
  AUTOREF_CNT_val_7_1 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_init,
      ADR1 => AUTOREF_value,
      ADR2 => Q_n0254(7),
      O => AUTOREF_CNT_val(7)
    );
  current_state_FFd14_In105_SW0 : X_LUT4
    generic map(
      INIT => X"FFFD"
    )
    port map (
      ADR0 => INIT_PRE_COUNT(0),
      ADR1 => INIT_PRE_COUNT(2),
      ADR2 => INIT_PRE_COUNT(1),
      ADR3 => INIT_PRE_COUNT(3),
      O => N592
    );
  current_state_FFd14_In105 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => INIT_PRE_COUNT(6),
      ADR1 => INIT_PRE_COUNT(5),
      ADR2 => INIT_PRE_COUNT(4),
      ADR3 => N592,
      O => current_state_FFd14_In_map655
    );
  Q_n02412 : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => dqs_div_cascount(2),
      ADR1 => burst_length(1),
      ADR2 => dqs_div_cascount(1),
      ADR3 => dqs_div_cascount(0),
      O => Q_n0241
    );
  RFC_COUNT_value_3_19_SW0 : X_LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      ADR0 => RFC_COUNT(2),
      ADR1 => RFC_COUNT(1),
      ADR2 => RFC_COUNT(0),
      O => N594
    );
  current_state_FFd13_In37_SW0 : X_LUT3
    generic map(
      INIT => X"FB"
    )
    port map (
      ADR0 => INIT_PRE_COUNT(1),
      ADR1 => INIT_PRE_COUNT(0),
      ADR2 => INIT_COUNT(0),
      O => N596
    );
  current_state_FFd13_In37 : X_LUT4
    generic map(
      INIT => X"0100"
    )
    port map (
      ADR0 => INIT_PRE_COUNT(5),
      ADR1 => INIT_PRE_COUNT(4),
      ADR2 => N596,
      ADR3 => current_state_FFd13_In_map562,
      O => current_state_FFd13_In_map563
    );
  RFC_COUNT_value_2_36_SW0 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => RFC_COUNT(1),
      ADR1 => RFC_COUNT(0),
      O => N598
    );
  RFC_COUNT_value_2_36 : X_LUT4
    generic map(
      INIT => X"4140"
    )
    port map (
      ADR0 => Auto_Ref_issued_p,
      ADR1 => N598,
      ADR2 => RFC_COUNT(2),
      ADR3 => RFC_COUNT_value_2_map254,
      O => RFC_COUNT_value(2)
    );
  current_state_FFd12_In1 : X_LUT4
    generic map(
      INIT => X"AA08"
    )
    port map (
      ADR0 => N501,
      ADR1 => N01,
      ADR2 => N59,
      ADR3 => N69,
      O => Q_n0213
    );
  current_state_FFd14_In46 : X_LUT4
    generic map(
      INIT => X"F666"
    )
    port map (
      ADR0 => INIT_COUNT(1),
      ADR1 => INIT_COUNT(2),
      ADR2 => INIT_COUNT(0),
      ADR3 => INIT_COUNT(3),
      O => current_state_FFd14_In_map640
    );
  current_state_FFd13_In4 : X_LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      ADR0 => MRD_COUNT(1),
      ADR1 => INIT_COUNT(2),
      ADR2 => MRD_COUNT(0),
      ADR3 => INIT_COUNT(0),
      O => current_state_FFd13_In_map547
    );
  current_state_FFd13_In73 : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => INIT_COUNT(1),
      ADR1 => INIT_MEMORY_61,
      ADR2 => INIT_COUNT(3),
      ADR3 => current_state_FFd14_72,
      O => current_state_FFd13_In_map568
    );
  Q_n02838 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => current_state_FFd12_64,
      ADR1 => current_state_FFd11_68,
      ADR2 => current_state_FFd10_73,
      ADR3 => current_state_FFd8_69,
      O => N558
    );
  Q_n028338 : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => INIT_COUNT(2),
      ADR1 => INIT_COUNT(0),
      ADR2 => INIT_COUNT(1),
      ADR3 => INIT_COUNT(3),
      O => N578
    );
  DLL_RST_COUNT_value_4_SW0 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => DLL_RST_COUNT(3),
      ADR1 => DLL_RST_COUNT(2),
      ADR2 => DLL_RST_COUNT(1),
      ADR3 => DLL_RST_COUNT(0),
      O => N155
    );
  current_state_FFd14_In0 : X_LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      ADR0 => current_state_FFd12_64,
      ADR1 => current_state_FFd11_68,
      ADR2 => current_state_FFd13_66,
      O => current_state_FFd14_In_map624
    );
  current_state_Out241 : X_LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      ADR0 => current_state_FFd5_62,
      ADR1 => current_state_FFd7_63,
      ADR2 => current_state_FFd13_66,
      ADR3 => current_state_FFd12_64,
      O => ddr_web_cntrl
    );
  Ker641 : X_LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      ADR0 => current_state_FFd13_66,
      ADR1 => current_state_FFd9_67,
      ADR2 => current_state_FFd12_64,
      ADR3 => current_state_FFd11_68,
      O => ddr_rasb_cntrl
    );
  AUTOREF_CNT_val_6_1 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_init,
      ADR1 => AUTOREF_value,
      ADR2 => Q_n0254(6),
      O => AUTOREF_CNT_val(6)
    );
  AUTOREF_CNT_val_5_1 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_init,
      ADR1 => AUTOREF_value,
      ADR2 => Q_n0254(5),
      O => AUTOREF_CNT_val(5)
    );
  AUTOREF_CNT_val_4_1 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_init,
      ADR1 => AUTOREF_value,
      ADR2 => Q_n0254(4),
      O => AUTOREF_CNT_val(4)
    );
  AUTOREF_CNT_val_3_1 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_init,
      ADR1 => AUTOREF_value,
      ADR2 => Q_n0254(3),
      O => AUTOREF_CNT_val(3)
    );
  AUTOREF_CNT_val_2_1 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_init,
      ADR1 => AUTOREF_value,
      ADR2 => Q_n0254(2),
      O => AUTOREF_CNT_val(2)
    );
  AUTOREF_CNT_val_1_1 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_init,
      ADR1 => AUTOREF_value,
      ADR2 => Q_n0254(1),
      O => AUTOREF_CNT_val(1)
    );
  ddr_address1_2_18 : X_LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      ADR0 => COLUMN_ADDRESS_reg(2),
      ADR1 => current_state_FFd6_65,
      ADR2 => ddr_address1_2_map88,
      ADR3 => N7,
      O => ddr_address_cntrl(2)
    );
  current_state_Out271 : X_LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      ADR0 => current_state_FFd12_64,
      ADR1 => current_state_FFd11_68,
      ADR2 => N7,
      ADR3 => current_state_FFd6_65,
      O => ddr_casb_cntrl
    );
  RP_cnt_value_0_1 : X_LUT4
    generic map(
      INIT => X"0302"
    )
    port map (
      ADR0 => RP_COUNT(2),
      ADR1 => Q_n0215,
      ADR2 => RP_COUNT(0),
      ADR3 => RP_COUNT(1),
      O => RP_cnt_value(0)
    );
  ddr_address1_10_SW0 : X_LUT4
    generic map(
      INIT => X"E2A2"
    )
    port map (
      ADR0 => current_state_FFd13_66,
      ADR1 => Q_n0282_79,
      ADR2 => LMR_DLL_rst_10_Q,
      ADR3 => current_state_FFd12_64,
      O => N424
    );
  DLL_RST_COUNT_value_0_1 : X_LUT4
    generic map(
      INIT => X"3111"
    )
    port map (
      ADR0 => DLL_RST_COUNT(0),
      ADR1 => N25,
      ADR2 => N36,
      ADR3 => N49,
      O => DLL_RST_COUNT_value(0)
    );
  ddr_address1_2_13_SW0 : X_LUT4
    generic map(
      INIT => X"BFBD"
    )
    port map (
      ADR0 => INIT_COUNT(2),
      ADR1 => INIT_COUNT(3),
      ADR2 => INIT_COUNT(1),
      ADR3 => INIT_COUNT(0),
      O => N600
    );
  ddr_address1_2_13 : X_LUT4
    generic map(
      INIT => X"2220"
    )
    port map (
      ADR0 => EMR_2_Q,
      ADR1 => N600,
      ADR2 => current_state_FFd12_64,
      ADR3 => current_state_FFd13_66,
      O => ddr_address1_2_map88
    );
  DLL_RST_COUNT_value_7_SW1 : X_LUT3
    generic map(
      INIT => X"36"
    )
    port map (
      ADR0 => N12,
      ADR1 => DLL_RST_COUNT(7),
      ADR2 => DLL_RST_COUNT(6),
      O => N602
    );
  DLL_RST_COUNT_value_7_Q : X_LUT4
    generic map(
      INIT => X"FF13"
    )
    port map (
      ADR0 => N36,
      ADR1 => N602,
      ADR2 => N49,
      ADR3 => N25,
      O => DLL_RST_COUNT_value(7)
    );
  ddr_address1_7_30 : X_LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      ADR0 => COLUMN_ADDRESS_reg(7),
      ADR1 => N7,
      ADR2 => N604,
      ADR3 => current_state_FFd6_65,
      O => ddr_address_cntrl(7)
    );
  ddr_address1_9_30 : X_LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      ADR0 => COLUMN_ADDRESS_reg(9),
      ADR1 => N7,
      ADR2 => N606,
      ADR3 => current_state_FFd6_65,
      O => ddr_address_cntrl(9)
    );
  ddr_address1_8_14_SW0 : X_LUT4
    generic map(
      INIT => X"BBDF"
    )
    port map (
      ADR0 => INIT_COUNT(2),
      ADR1 => INIT_COUNT(1),
      ADR2 => EMR_8_Q,
      ADR3 => INIT_COUNT(3),
      O => N608
    );
  ddr_address1_8_14 : X_LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      ADR0 => INIT_COUNT(0),
      ADR1 => N608,
      O => ddr_address1_8_map350
    );
  ddr_address1_5_SW0 : X_LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      ADR0 => COLUMN_ADDRESS_reg(5),
      ADR1 => current_state_FFd6_65,
      ADR2 => current_state_FFd5_62,
      ADR3 => current_state_FFd7_63,
      O => N691
    );
  ddr_address1_4_SW0 : X_LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      ADR0 => COLUMN_ADDRESS_reg(4),
      ADR1 => current_state_FFd6_65,
      ADR2 => current_state_FFd5_62,
      ADR3 => current_state_FFd7_63,
      O => N71
    );
  ddr_address1_8_0 : X_LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      ADR0 => COLUMN_ADDRESS_reg(8),
      ADR1 => current_state_FFd6_65,
      ADR2 => current_state_FFd5_62,
      ADR3 => current_state_FFd7_63,
      O => ddr_address1_8_map343
    );
  ddr_address1_6_1 : X_LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      ADR0 => COLUMN_ADDRESS_reg(6),
      ADR1 => current_state_FFd6_65,
      ADR2 => current_state_FFd5_62,
      ADR3 => current_state_FFd7_63,
      O => ddr_address_cntrl(6)
    );
  ddr_address1_3_1 : X_LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      ADR0 => COLUMN_ADDRESS_reg(3),
      ADR1 => current_state_FFd6_65,
      ADR2 => current_state_FFd5_62,
      ADR3 => current_state_FFd7_63,
      O => ddr_address_cntrl(3)
    );
  Ker11 : X_LUT4
    generic map(
      INIT => X"FF20"
    )
    port map (
      ADR0 => MRD_COUNT(0),
      ADR1 => MRD_COUNT(1),
      ADR2 => N01,
      ADR3 => N69,
      O => N1
    );
  AUTOREF_CNT_val_0_1 : X_LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_init,
      ADR1 => AUTOREF_value,
      ADR2 => AUTOREF_COUNT(0),
      O => AUTOREF_CNT_val(0)
    );
  ddr_address1_7_30_SW0_SW0 : X_LUT4
    generic map(
      INIT => X"BBDF"
    )
    port map (
      ADR0 => INIT_COUNT(2),
      ADR1 => INIT_COUNT(3),
      ADR2 => EMR_7_Q,
      ADR3 => INIT_COUNT(1),
      O => N610
    );
  ddr_address1_7_30_SW0 : X_LUT4
    generic map(
      INIT => X"0302"
    )
    port map (
      ADR0 => current_state_FFd12_64,
      ADR1 => INIT_COUNT(0),
      ADR2 => N610,
      ADR3 => current_state_FFd13_66,
      O => N604
    );
  ddr_address1_9_30_SW0_SW0 : X_LUT4
    generic map(
      INIT => X"BBDF"
    )
    port map (
      ADR0 => INIT_COUNT(2),
      ADR1 => INIT_COUNT(3),
      ADR2 => EMR_9_Q,
      ADR3 => INIT_COUNT(1),
      O => N612
    );
  ddr_address1_9_30_SW0 : X_LUT4
    generic map(
      INIT => X"0302"
    )
    port map (
      ADR0 => current_state_FFd12_64,
      ADR1 => INIT_COUNT(0),
      ADR2 => N612,
      ADR3 => current_state_FFd13_66,
      O => N606
    );
  MRD_COUNT_value_1_Q : X_MUX2
    port map (
      IA => N614,
      IB => N615,
      SEL => N69,
      O => MRD_COUNT_value(1)
    );
  MRD_COUNT_value_1_F : X_LUT4
    generic map(
      INIT => X"A888"
    )
    port map (
      ADR0 => MRD_COUNT(0),
      ADR1 => MRD_COUNT(1),
      ADR2 => N501,
      ADR3 => N01,
      O => N614
    );
  MRD_COUNT_value_1_G : X_LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      ADR0 => MRD_COUNT(1),
      ADR1 => MRD_COUNT(0),
      ADR2 => N501,
      O => N615
    );
  current_state_FFd11_In : X_MUX2
    port map (
      IA => N616,
      IB => N617,
      SEL => INIT_MEMORY_61,
      O => Auto_Ref_issued_p
    );
  current_state_FFd11_In_F : X_LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      ADR0 => Q_n0181,
      ADR1 => RFC_COUNT_reg_44,
      ADR2 => current_state_FFd14_72,
      ADR3 => auto_ref_89,
      O => N616
    );
  current_state_FFd11_In_G : X_LUT4
    generic map(
      INIT => X"0D08"
    )
    port map (
      ADR0 => INIT_COUNT(0),
      ADR1 => RFC_COUNT_reg_44,
      ADR2 => N346,
      ADR3 => Q_n0181,
      O => N617
    );
  Ker69 : X_MUX2
    port map (
      IA => N618,
      IB => N619,
      SEL => INIT_COUNT(3),
      O => N69
    );
  Ker69_F : X_LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      ADR0 => RP_COUNT(2),
      ADR1 => RP_COUNT(0),
      ADR2 => RP_COUNT(1),
      ADR3 => N343,
      O => N618
    );
  Ker69_G : X_LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      ADR0 => RFC_COUNT_reg_44,
      ADR1 => INIT_COUNT(2),
      ADR2 => INIT_COUNT(1),
      ADR3 => INIT_COUNT(0),
      O => N619
    );
  RFC_COUNT_value_3_41 : X_MUX2
    port map (
      IA => N620,
      IB => N621,
      SEL => RFC_COUNT(3),
      O => RFC_COUNT_value(3)
    );
  RFC_COUNT_value_3_41_F : X_LUT4
    generic map(
      INIT => X"0302"
    )
    port map (
      ADR0 => RFC_COUNT(4),
      ADR1 => N594,
      ADR2 => Auto_Ref_issued_p,
      ADR3 => RFC_COUNT(5),
      O => N620
    );
  RFC_COUNT_value_3_41_G : X_LUT4
    generic map(
      INIT => X"3332"
    )
    port map (
      ADR0 => RFC_COUNT(1),
      ADR1 => Auto_Ref_issued_p,
      ADR2 => RFC_COUNT(2),
      ADR3 => RFC_COUNT(0),
      O => N621
    );
  NlwBlock_controller0_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlock_controller0_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal vcc : STD_LOGIC; 
  signal ddr2_clk0_q : STD_LOGIC; 
  signal gnd : STD_LOGIC; 
  signal ddr2_clk0b_q : STD_LOGIC; 
begin
  XST_VCC : X_ONE
    port map (
      O => vcc
    );
  XST_GND : X_ZERO
    port map (
      O => gnd
    );
  U1 : X_FDDRRSE
    port map (
      CE => vcc,
      R => gnd,
      S => gnd,
      C0 => clk0,
      C1 => clk180,
      D0 => vcc,
      D1 => gnd,
      Q => ddr2_clk0_q
    );
  U2 : X_FDDRRSE
    port map (
      CE => vcc,
      R => gnd,
      S => gnd,
      C0 => clk0,
      C1 => clk180,
      D0 => gnd,
      D1 => vcc,
      Q => ddr2_clk0b_q
    );
  r1 : X_OBUF
    port map (
      I => ddr2_clk0_q,
      O => ddr2_clk0
    );
  r2 : X_OBUF
    port map (
      I => ddr2_clk0b_q,
      O => ddr2_clk0b
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal NlwRenamedSig_IO_rst_dqs_div_in : STD_LOGIC; 
  signal ddr_web_q : STD_LOGIC; 
  signal ddr_cke_q : STD_LOGIC; 
  signal ddr_rasb_q : STD_LOGIC; 
  signal ddr_casb_q : STD_LOGIC; 
  signal VCC : STD_LOGIC; 
  signal GND : STD_LOGIC; 
  signal ddr_address_reg : STD_LOGIC_VECTOR ( 12 downto 0 ); 
  signal ddr_ba_reg : STD_LOGIC_VECTOR ( 1 downto 0 ); 
begin
  NlwRenamedSig_IO_rst_dqs_div_in <= rst_dqs_div_in;
  iob_web : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_web_cntrl,
      CLK => clk180,
      O => ddr_web_q,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_rasb : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_rasb_cntrl,
      CLK => clk180,
      O => ddr_rasb_q,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_casb : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_casb_cntrl,
      CLK => clk180,
      O => ddr_casb_q,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_cke : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_cke_cntrl,
      CLK => clk180,
      O => ddr_cke_q,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_addr_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_address_cntrl(0),
      CLK => clk180,
      O => ddr_address_reg(0),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_addr_1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_address_cntrl(1),
      CLK => clk180,
      O => ddr_address_reg(1),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_addr_2 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_address_cntrl(2),
      CLK => clk180,
      O => ddr_address_reg(2),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_addr_3 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_address_cntrl(3),
      CLK => clk180,
      O => ddr_address_reg(3),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_addr_4 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_address_cntrl(4),
      CLK => clk180,
      O => ddr_address_reg(4),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_addr_5 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_address_cntrl(5),
      CLK => clk180,
      O => ddr_address_reg(5),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_addr_6 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_address_cntrl(6),
      CLK => clk180,
      O => ddr_address_reg(6),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_addr_7 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_address_cntrl(7),
      CLK => clk180,
      O => ddr_address_reg(7),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_addr_8 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_address_cntrl(8),
      CLK => clk180,
      O => ddr_address_reg(8),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_addr_9 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_address_cntrl(9),
      CLK => clk180,
      O => ddr_address_reg(9),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_addr_10 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_address_cntrl(10),
      CLK => clk180,
      O => ddr_address_reg(10),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_addr_11 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_address_cntrl(11),
      CLK => clk180,
      O => ddr_address_reg(11),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_addr_12 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_address_cntrl(12),
      CLK => clk180,
      O => ddr_address_reg(12),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_ba_0 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_ba_cntrl(0),
      CLK => clk180,
      O => ddr_ba_reg(0),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  iob_ba_1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_ba_cntrl(1),
      CLK => clk180,
      O => ddr_ba_reg(1),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  rst_iob_inbuf : X_BUF
    port map (
      I => NlwRenamedSig_IO_rst_dqs_div_in,
      O => rst_dqs_div
    );
  r20 : X_OBUF
    port map (
      I => ddr_cke_q,
      O => ddr_cke
    );
  r17 : X_OBUF
    port map (
      I => ddr_rasb_q,
      O => ddr_rasb
    );
  r19 : X_OBUF
    port map (
      I => ddr_csb_cntrl,
      O => ddr_csb
    );
  r18 : X_OBUF
    port map (
      I => ddr_casb_q,
      O => ddr_casb
    );
  r16 : X_OBUF
    port map (
      I => ddr_web_q,
      O => ddr_web
    );
  r21 : X_OBUF
    port map (
      I => ddr_ODT_cntrl,
      O => ddr_ODT0
    );
  rst_iob_outbuf : X_OBUF
    port map (
      I => rst_dqs_div_int,
      O => rst_dqs_div_out
    );
  r13 : X_OBUF
    port map (
      I => ddr_ba_reg(1),
      O => ddr_ba(1)
    );
  r00 : X_OBUF
    port map (
      I => ddr_ba_reg(0),
      O => ddr_ba(0)
    );
  r12 : X_OBUF
    port map (
      I => ddr_address_reg(12),
      O => ddr_address(12)
    );
  r11 : X_OBUF
    port map (
      I => ddr_address_reg(11),
      O => ddr_address(11)
    );
  r10 : X_OBUF
    port map (
      I => ddr_address_reg(10),
      O => ddr_address(10)
    );
  r9 : X_OBUF
    port map (
      I => ddr_address_reg(9),
      O => ddr_address(9)
    );
  r8 : X_OBUF
    port map (
      I => ddr_address_reg(8),
      O => ddr_address(8)
    );
  r7 : X_OBUF
    port map (
      I => ddr_address_reg(7),
      O => ddr_address(7)
    );
  r6 : X_OBUF
    port map (
      I => ddr_address_reg(6),
      O => ddr_address(6)
    );
  r5 : X_OBUF
    port map (
      I => ddr_address_reg(5),
      O => ddr_address(5)
    );
  r4 : X_OBUF
    port map (
      I => ddr_address_reg(4),
      O => ddr_address(4)
    );
  r3 : X_OBUF
    port map (
      I => ddr_address_reg(3),
      O => ddr_address(3)
    );
  r2 : X_OBUF
    port map (
      I => ddr_address_reg(2),
      O => ddr_address(2)
    );
  r1 : X_OBUF
    port map (
      I => ddr_address_reg(1),
      O => ddr_address(1)
    );
  r0 : X_OBUF
    port map (
      I => ddr_address_reg(0),
      O => ddr_address(0)
    );
  NlwBlock_controller_iobs0_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlock_controller_iobs0_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal vcc : STD_LOGIC; 
  signal gnd : STD_LOGIC; 
  signal mask_o : STD_LOGIC_VECTOR ( 1 downto 0 ); 
begin
  XST_VCC : X_ONE
    port map (
      O => vcc
    );
  XST_GND : X_ZERO
    port map (
      O => gnd
    );
  DDR_DM0_OUT : X_FDDRRSE
    port map (
      CE => vcc,
      R => gnd,
      S => gnd,
      C0 => clk270,
      C1 => clk90,
      D0 => mask_rising(0),
      D1 => mask_falling(0),
      Q => mask_o(0)
    );
  DDR_DM1_OUT : X_FDDRRSE
    port map (
      CE => vcc,
      R => gnd,
      S => gnd,
      C0 => clk270,
      C1 => clk90,
      D0 => mask_rising(1),
      D1 => mask_falling(1),
      Q => mask_o(1)
    );
  DM1_OBUF : X_OBUF
    port map (
      I => mask_o(1),
      O => ddr_dm(1)
    );
  DM0_OBUF : X_OBUF
    port map (
      I => mask_o(0),
      O => ddr_dm(0)
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : X_ZERO
    port map (
      O => GND
    );
  XST_VCC : X_ONE
    port map (
      O => clock_en
    );
  DQ_T : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => enable_b,
      CE => clock_en,
      RST => reset,
      CLK => clk270,
      O => ddr_en,
      SET => GND
    );
  DDR_OUT : X_FDDRRSE
    port map (
      CE => clock_en,
      R => GND,
      S => GND,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_IBUF : X_BUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => write_en_val,
      O => enable_b,
      ADR1 => GND
    );
  DQ_OBUFT : X_OBUFT
    port map (
      I => ddr_dq_q,
      CTL => ddr_en,
      O => ddr_dq_inout
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : X_ZERO
    port map (
      O => GND
    );
  XST_VCC : X_ONE
    port map (
      O => clock_en
    );
  DQ_T : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => enable_b,
      CE => clock_en,
      RST => reset,
      CLK => clk270,
      O => ddr_en,
      SET => GND
    );
  DDR_OUT : X_FDDRRSE
    port map (
      CE => clock_en,
      R => GND,
      S => GND,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_IBUF : X_BUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => write_en_val,
      O => enable_b,
      ADR1 => GND
    );
  DQ_OBUFT : X_OBUFT
    port map (
      I => ddr_dq_q,
      CTL => ddr_en,
      O => ddr_dq_inout
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : X_ZERO
    port map (
      O => GND
    );
  XST_VCC : X_ONE
    port map (
      O => clock_en
    );
  DQ_T : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => enable_b,
      CE => clock_en,
      RST => reset,
      CLK => clk270,
      O => ddr_en,
      SET => GND
    );
  DDR_OUT : X_FDDRRSE
    port map (
      CE => clock_en,
      R => GND,
      S => GND,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_IBUF : X_BUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => write_en_val,
      O => enable_b,
      ADR1 => GND
    );
  DQ_OBUFT : X_OBUFT
    port map (
      I => ddr_dq_q,
      CTL => ddr_en,
      O => ddr_dq_inout
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : X_ZERO
    port map (
      O => GND
    );
  XST_VCC : X_ONE
    port map (
      O => clock_en
    );
  DQ_T : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => enable_b,
      CE => clock_en,
      RST => reset,
      CLK => clk270,
      O => ddr_en,
      SET => GND
    );
  DDR_OUT : X_FDDRRSE
    port map (
      CE => clock_en,
      R => GND,
      S => GND,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_IBUF : X_BUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => write_en_val,
      O => enable_b,
      ADR1 => GND
    );
  DQ_OBUFT : X_OBUFT
    port map (
      I => ddr_dq_q,
      CTL => ddr_en,
      O => ddr_dq_inout
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : X_ZERO
    port map (
      O => GND
    );
  XST_VCC : X_ONE
    port map (
      O => clock_en
    );
  DQ_T : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => enable_b,
      CE => clock_en,
      RST => reset,
      CLK => clk270,
      O => ddr_en,
      SET => GND
    );
  DDR_OUT : X_FDDRRSE
    port map (
      CE => clock_en,
      R => GND,
      S => GND,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_IBUF : X_BUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => write_en_val,
      O => enable_b,
      ADR1 => GND
    );
  DQ_OBUFT : X_OBUFT
    port map (
      I => ddr_dq_q,
      CTL => ddr_en,
      O => ddr_dq_inout
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : X_ZERO
    port map (
      O => GND
    );
  XST_VCC : X_ONE
    port map (
      O => clock_en
    );
  DQ_T : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => enable_b,
      CE => clock_en,
      RST => reset,
      CLK => clk270,
      O => ddr_en,
      SET => GND
    );
  DDR_OUT : X_FDDRRSE
    port map (
      CE => clock_en,
      R => GND,
      S => GND,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_IBUF : X_BUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => write_en_val,
      O => enable_b,
      ADR1 => GND
    );
  DQ_OBUFT : X_OBUFT
    port map (
      I => ddr_dq_q,
      CTL => ddr_en,
      O => ddr_dq_inout
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : X_ZERO
    port map (
      O => GND
    );
  XST_VCC : X_ONE
    port map (
      O => clock_en
    );
  DQ_T : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => enable_b,
      CE => clock_en,
      RST => reset,
      CLK => clk270,
      O => ddr_en,
      SET => GND
    );
  DDR_OUT : X_FDDRRSE
    port map (
      CE => clock_en,
      R => GND,
      S => GND,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_IBUF : X_BUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => write_en_val,
      O => enable_b,
      ADR1 => GND
    );
  DQ_OBUFT : X_OBUFT
    port map (
      I => ddr_dq_q,
      CTL => ddr_en,
      O => ddr_dq_inout
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : X_ZERO
    port map (
      O => GND
    );
  XST_VCC : X_ONE
    port map (
      O => clock_en
    );
  DQ_T : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => enable_b,
      CE => clock_en,
      RST => reset,
      CLK => clk270,
      O => ddr_en,
      SET => GND
    );
  DDR_OUT : X_FDDRRSE
    port map (
      CE => clock_en,
      R => GND,
      S => GND,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_IBUF : X_BUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => write_en_val,
      O => enable_b,
      ADR1 => GND
    );
  DQ_OBUFT : X_OBUFT
    port map (
      I => ddr_dq_q,
      CTL => ddr_en,
      O => ddr_dq_inout
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : X_ZERO
    port map (
      O => GND
    );
  XST_VCC : X_ONE
    port map (
      O => clock_en
    );
  DQ_T : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => enable_b,
      CE => clock_en,
      RST => reset,
      CLK => clk270,
      O => ddr_en,
      SET => GND
    );
  DDR_OUT : X_FDDRRSE
    port map (
      CE => clock_en,
      R => GND,
      S => GND,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_IBUF : X_BUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => write_en_val,
      O => enable_b,
      ADR1 => GND
    );
  DQ_OBUFT : X_OBUFT
    port map (
      I => ddr_dq_q,
      CTL => ddr_en,
      O => ddr_dq_inout
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : X_ZERO
    port map (
      O => GND
    );
  XST_VCC : X_ONE
    port map (
      O => clock_en
    );
  DQ_T : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => enable_b,
      CE => clock_en,
      RST => reset,
      CLK => clk270,
      O => ddr_en,
      SET => GND
    );
  DDR_OUT : X_FDDRRSE
    port map (
      CE => clock_en,
      R => GND,
      S => GND,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_IBUF : X_BUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => write_en_val,
      O => enable_b,
      ADR1 => GND
    );
  DQ_OBUFT : X_OBUFT
    port map (
      I => ddr_dq_q,
      CTL => ddr_en,
      O => ddr_dq_inout
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : X_ZERO
    port map (
      O => GND
    );
  XST_VCC : X_ONE
    port map (
      O => clock_en
    );
  DQ_T : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => enable_b,
      CE => clock_en,
      RST => reset,
      CLK => clk270,
      O => ddr_en,
      SET => GND
    );
  DDR_OUT : X_FDDRRSE
    port map (
      CE => clock_en,
      R => GND,
      S => GND,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_IBUF : X_BUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => write_en_val,
      O => enable_b,
      ADR1 => GND
    );
  DQ_OBUFT : X_OBUFT
    port map (
      I => ddr_dq_q,
      CTL => ddr_en,
      O => ddr_dq_inout
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : X_ZERO
    port map (
      O => GND
    );
  XST_VCC : X_ONE
    port map (
      O => clock_en
    );
  DQ_T : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => enable_b,
      CE => clock_en,
      RST => reset,
      CLK => clk270,
      O => ddr_en,
      SET => GND
    );
  DDR_OUT : X_FDDRRSE
    port map (
      CE => clock_en,
      R => GND,
      S => GND,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_IBUF : X_BUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => write_en_val,
      O => enable_b,
      ADR1 => GND
    );
  DQ_OBUFT : X_OBUFT
    port map (
      I => ddr_dq_q,
      CTL => ddr_en,
      O => ddr_dq_inout
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : X_ZERO
    port map (
      O => GND
    );
  XST_VCC : X_ONE
    port map (
      O => clock_en
    );
  DQ_T : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => enable_b,
      CE => clock_en,
      RST => reset,
      CLK => clk270,
      O => ddr_en,
      SET => GND
    );
  DDR_OUT : X_FDDRRSE
    port map (
      CE => clock_en,
      R => GND,
      S => GND,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_IBUF : X_BUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => write_en_val,
      O => enable_b,
      ADR1 => GND
    );
  DQ_OBUFT : X_OBUFT
    port map (
      I => ddr_dq_q,
      CTL => ddr_en,
      O => ddr_dq_inout
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : X_ZERO
    port map (
      O => GND
    );
  XST_VCC : X_ONE
    port map (
      O => clock_en
    );
  DQ_T : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => enable_b,
      CE => clock_en,
      RST => reset,
      CLK => clk270,
      O => ddr_en,
      SET => GND
    );
  DDR_OUT : X_FDDRRSE
    port map (
      CE => clock_en,
      R => GND,
      S => GND,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_IBUF : X_BUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => write_en_val,
      O => enable_b,
      ADR1 => GND
    );
  DQ_OBUFT : X_OBUFT
    port map (
      I => ddr_dq_q,
      CTL => ddr_en,
      O => ddr_dq_inout
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : X_ZERO
    port map (
      O => GND
    );
  XST_VCC : X_ONE
    port map (
      O => clock_en
    );
  DQ_T : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => enable_b,
      CE => clock_en,
      RST => reset,
      CLK => clk270,
      O => ddr_en,
      SET => GND
    );
  DDR_OUT : X_FDDRRSE
    port map (
      CE => clock_en,
      R => GND,
      S => GND,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_IBUF : X_BUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => write_en_val,
      O => enable_b,
      ADR1 => GND
    );
  DQ_OBUFT : X_OBUFT
    port map (
      I => ddr_dq_q,
      CTL => ddr_en,
      O => ddr_dq_inout
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
  signal enable_b : STD_LOGIC; 
  signal ddr_en : STD_LOGIC; 
  signal ddr_dq_q : STD_LOGIC; 
begin
  XST_GND : X_ZERO
    port map (
      O => GND
    );
  XST_VCC : X_ONE
    port map (
      O => clock_en
    );
  DQ_T : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => enable_b,
      CE => clock_en,
      RST => reset,
      CLK => clk270,
      O => ddr_en,
      SET => GND
    );
  DDR_OUT : X_FDDRRSE
    port map (
      CE => clock_en,
      R => GND,
      S => GND,
      C0 => clk270,
      C1 => clk90,
      D0 => write_data_rising,
      D1 => write_data_falling,
      Q => ddr_dq_q
    );
  DQ_IBUF : X_BUF
    port map (
      I => ddr_dq_inout,
      O => read_data_in
    );
  enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => write_en_val,
      O => enable_b,
      ADR1 => GND
    );
  DQ_OBUFT : X_OBUFT
    port map (
      I => ddr_dq_q,
      CTL => ddr_en,
      O => ddr_dq_inout
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal vcc : STD_LOGIC; 
  signal dqs_q : STD_LOGIC; 
  signal ddr_dqs_enable1 : STD_LOGIC; 
  signal ddr_dqs_enable_b : STD_LOGIC; 
  signal gnd : STD_LOGIC; 
  signal VCC_110 : STD_LOGIC; 
  signal GND_111 : STD_LOGIC; 
  signal NLW_U5_Q_UNCONNECTED : STD_LOGIC; 
begin
  XST_VCC : X_ONE
    port map (
      O => vcc
    );
  XST_GND : X_ZERO
    port map (
      O => gnd
    );
  U1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_dqs_enable_b,
      CLK => clk,
      O => ddr_dqs_enable1,
      CE => VCC_110,
      SET => GND_111,
      RST => GND_111
    );
  U2 : X_FDDRRSE
    port map (
      CE => vcc,
      R => gnd,
      S => gnd,
      C0 => clk180,
      C1 => clk,
      D0 => gnd,
      D1 => data1,
      Q => dqs_q
    );
  U5 : X_FDDRRSE
    port map (
      CE => vcc,
      R => gnd,
      S => gnd,
      C0 => clk180,
      C1 => clk,
      D0 => data1,
      D1 => gnd,
      Q => NLW_U5_Q_UNCONNECTED
    );
  ddr_dqs_enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => ddr_dqs_enable,
      O => ddr_dqs_enable_b,
      ADR1 => GND_111
    );
  data11 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => ddr_dqs_reset,
      O => data1,
      ADR1 => GND_111
    );
  U7_OBUFTDS : X_OBUFTDS
    port map (
      T => ddr_dqs_enable1,
      I => dqs_q,
      O => ddr_dqs,
      OB => ddr_dqs_n
    );
  U7_IBUFDS : X_IBUFDS
    port map (
      I => ddr_dqs,
      IB => ddr_dqs_n,
      O => dqs
    );
  NlwBlock_ddr_dqs_iob0_VCC : X_ONE
    port map (
      O => VCC_110
    );
  NlwBlock_ddr_dqs_iob0_GND : X_ZERO
    port map (
      O => GND_111
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal vcc : STD_LOGIC; 
  signal dqs_q : STD_LOGIC; 
  signal ddr_dqs_enable1 : STD_LOGIC; 
  signal ddr_dqs_enable_b : STD_LOGIC; 
  signal gnd : STD_LOGIC; 
  signal VCC_112 : STD_LOGIC; 
  signal GND_113 : STD_LOGIC; 
  signal NLW_U5_Q_UNCONNECTED : STD_LOGIC; 
begin
  XST_VCC : X_ONE
    port map (
      O => vcc
    );
  XST_GND : X_ZERO
    port map (
      O => gnd
    );
  U1 : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => ddr_dqs_enable_b,
      CLK => clk,
      O => ddr_dqs_enable1,
      CE => VCC_112,
      SET => GND_113,
      RST => GND_113
    );
  U2 : X_FDDRRSE
    port map (
      CE => vcc,
      R => gnd,
      S => gnd,
      C0 => clk180,
      C1 => clk,
      D0 => gnd,
      D1 => data1,
      Q => dqs_q
    );
  U5 : X_FDDRRSE
    port map (
      CE => vcc,
      R => gnd,
      S => gnd,
      C0 => clk180,
      C1 => clk,
      D0 => data1,
      D1 => gnd,
      Q => NLW_U5_Q_UNCONNECTED
    );
  ddr_dqs_enable_b1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => ddr_dqs_enable,
      O => ddr_dqs_enable_b,
      ADR1 => GND_113
    );
  data11 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => ddr_dqs_reset,
      O => data1,
      ADR1 => GND_113
    );
  U7_OBUFTDS : X_OBUFTDS
    port map (
      T => ddr_dqs_enable1,
      I => dqs_q,
      O => ddr_dqs,
      OB => ddr_dqs_n
    );
  U7_IBUFDS : X_IBUFDS
    port map (
      I => ddr_dqs,
      IB => ddr_dqs_n,
      O => dqs
    );
  NlwBlock_ddr_dqs_iob1_VCC : X_ONE
    port map (
      O => VCC_112
    );
  NlwBlock_ddr_dqs_iob1_GND : X_ZERO
    port map (
      O => GND_113
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

entity dcmx3y0_2vp20 is
  port (
    clock1_in : in STD_LOGIC := 'X'; 
    clock2_in : in STD_LOGIC := 'X'; 
    clock2_out : out STD_LOGIC; 
    clock1_out : out STD_LOGIC 
  );
end dcmx3y0_2vp20;

architecture Structure of dcmx3y0_2vp20 is
  signal VCC_114 : STD_LOGIC; 
  signal clkd1inv_1 : STD_LOGIC; 
  signal clkd2buf_1 : STD_LOGIC; 
  signal clkd1inv_2 : STD_LOGIC; 
  signal clkd2buf_2 : STD_LOGIC; 
begin
  VCC : X_ONE
    port map (
      O => VCC_114
    );
  INV1_1 : X_LUT4
    generic map(
      LOC => "SLICE_X87Y1",
      INIT => X"0f0f"
    )
    port map (
      ADR0 => VCC_114,
      ADR1 => VCC_114,
      ADR2 => clock1_in,
      ADR3 => VCC_114,
      O => clkd1inv_1
    );
  BUF2_1 : X_LUT4
    generic map(
      LOC => "SLICE_X44Y1",
      INIT => X"f0f0"
    )
    port map (
      ADR0 => VCC_114,
      ADR1 => VCC_114,
      ADR2 => clkd1inv_1,
      ADR3 => VCC_114,
      O => clkd2buf_1
    );
  INV2_1 : X_LUT4
    generic map(
      LOC => "SLICE_X44Y1",
      INIT => X"0f0f"
    )
    port map (
      ADR0 => VCC_114,
      ADR1 => VCC_114,
      ADR2 => clkd2buf_1,
      ADR3 => VCC_114,
      O => clock1_out
    );
  INV1_2 : X_LUT4
    generic map(
      LOC => "SLICE_X86Y0",
      INIT => X"00ff"
    )
    port map (
      ADR0 => VCC_114,
      ADR1 => VCC_114,
      ADR2 => VCC_114,
      ADR3 => clock2_in,
      O => clkd1inv_2
    );
  BUF2_2 : X_LUT4
    generic map(
      LOC => "SLICE_X45Y0",
      INIT => X"f0f0"
    )
    port map (
      ADR0 => VCC_114,
      ADR1 => VCC_114,
      ADR2 => clkd1inv_2,
      ADR3 => VCC_114,
      O => clkd2buf_2
    );
  INV2_2 : X_LUT4
    generic map(
      LOC => "SLICE_X45Y0",
      INIT => X"0f0f"
    )
    port map (
      ADR0 => VCC_114,
      ADR1 => VCC_114,
      ADR2 => clkd2buf_2,
      ADR3 => VCC_114,
      O => clock2_out
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

entity cal_div2f is
  port (
    reset : in STD_LOGIC := 'X'; 
    iclk : in STD_LOGIC := 'X'; 
    oclk : out STD_LOGIC 
  );
end cal_div2f;

architecture Structure of cal_div2f is
  signal poclk_116 : STD_LOGIC; 
  signal Q_n0003 : STD_LOGIC; 
  signal NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_cal_top0_cal_suPhClkd2_poclk_C : STD_LOGIC; 
  signal VCC : STD_LOGIC; 
  signal GND : STD_LOGIC; 
  signal NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_cal_top0_cal_suPhClkd2_oclk_C : STD_LOGIC; 
begin
  poclk : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0003,
      SRST => reset,
      CLK => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_cal_top0_cal_suPhClkd2_poclk_C,
      O => poclk_116,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  oclk_115 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => poclk_116,
      SRST => reset,
      CLK => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_cal_top0_cal_suPhClkd2_oclk_C,
      O => oclk,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Q_n00031 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => poclk_116,
      O => Q_n0003,
      ADR1 => GND
    );
  NlwInverterBlock_mem_interface_top_inst_infrastructure_top0_cal_top0_cal_suPhClkd2_poclk_C : X_INV
    port map (
      I => iclk,
      O => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_cal_top0_cal_suPhClkd2_poclk_C
    );
  NlwBlock_cal_suPhClkd2_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlock_cal_suPhClkd2_GND : X_ZERO
    port map (
      O => GND
    );
  NlwInverterBlock_mem_interface_top_inst_infrastructure_top0_cal_top0_cal_suPhClkd2_oclk_C : X_INV
    port map (
      I => iclk,
      O => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_cal_top0_cal_suPhClkd2_oclk_C
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

entity cal_div2 is
  port (
    reset : in STD_LOGIC := 'X'; 
    iclk : in STD_LOGIC := 'X'; 
    oclk : out STD_LOGIC 
  );
end cal_div2;

architecture Structure of cal_div2 is
  signal poclk_118 : STD_LOGIC; 
  signal Q_n0003 : STD_LOGIC; 
  signal VCC : STD_LOGIC; 
  signal GND : STD_LOGIC; 
begin
  poclk : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0003,
      SRST => reset,
      CLK => iclk,
      O => poclk_118,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  oclk_117 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => poclk_118,
      SRST => reset,
      CLK => iclk,
      O => oclk,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Q_n00031 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => poclk_118,
      O => Q_n0003,
      ADR1 => GND
    );
  NlwBlock_cal_suClkd2_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlock_cal_suClkd2_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal prevSamp_120 : STD_LOGIC; 
  signal resetDcm_121 : STD_LOGIC; 
  signal okSelCnt_122 : STD_LOGIC; 
  signal wait3Cycle_123 : STD_LOGIC; 
  signal wait4Cycle_124 : STD_LOGIC; 
  signal wait5Cycle_125 : STD_LOGIC; 
  signal psDoneReg_126 : STD_LOGIC; 
  signal rstate_127 : STD_LOGIC; 
  signal Q_n0051 : STD_LOGIC; 
  signal Q_n0103 : STD_LOGIC; 
  signal Q_n0060 : STD_LOGIC; 
  signal Q_n0113 : STD_LOGIC; 
  signal Q_n0070 : STD_LOGIC; 
  signal Q_n0067_128 : STD_LOGIC; 
  signal Q_n0072 : STD_LOGIC; 
  signal Q_n0122 : STD_LOGIC; 
  signal Q_n0125 : STD_LOGIC; 
  signal Q_n0076_129 : STD_LOGIC; 
  signal Q_n0081_130 : STD_LOGIC; 
  signal Q_n0126 : STD_LOGIC; 
  signal Q_n0131 : STD_LOGIC; 
  signal Q_n0132 : STD_LOGIC; 
  signal waitOneCycle_131 : STD_LOGIC; 
  signal Q_n0083 : STD_LOGIC; 
  signal Q_n0090 : STD_LOGIC; 
  signal Q_n0091 : STD_LOGIC; 
  signal Q_n0088 : STD_LOGIC; 
  signal waitTwoCycle_132 : STD_LOGIC; 
  signal Q_n0200 : STD_LOGIC; 
  signal Q_n0201 : STD_LOGIC; 
  signal Q_n0202 : STD_LOGIC; 
  signal Q_n0203 : STD_LOGIC; 
  signal Q_n0204_133 : STD_LOGIC; 
  signal Q_n0210 : STD_LOGIC; 
  signal Q_n0205_134 : STD_LOGIC; 
  signal Q_n0211_135 : STD_LOGIC; 
  signal Q_n0206 : STD_LOGIC; 
  signal Q_n0207 : STD_LOGIC; 
  signal Q_n0213 : STD_LOGIC; 
  signal Q_n0208_136 : STD_LOGIC; 
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
  signal state_FFd1_137 : STD_LOGIC; 
  signal state_FFd2_138 : STD_LOGIC; 
  signal state_FFd3_139 : STD_LOGIC; 
  signal state_FFd4_140 : STD_LOGIC; 
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
  signal decNegSh_4_rt_141 : STD_LOGIC; 
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
  signal GND : STD_LOGIC; 
  signal VCC : STD_LOGIC; 
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
  XST_GND : X_ZERO
    port map (
      O => uPtr(3)
    );
  XST_VCC : X_ONE
    port map (
      O => N1
    );
  d4Shft_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(0),
      SRST => reset,
      CE => Q_n0195,
      CLK => clk,
      O => d4Shft(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d4Shft_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(1),
      SRST => reset,
      CE => Q_n0195,
      CLK => clk,
      O => d4Shft(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d4Shft_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(2),
      SRST => reset,
      CE => Q_n0195,
      CLK => clk,
      O => d4Shft(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d4Shft_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(3),
      SRST => reset,
      CE => Q_n0195,
      CLK => clk,
      O => d4Shft(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d4Shft_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(4),
      SRST => reset,
      CE => Q_n0195,
      CLK => clk,
      O => d4Shft(4),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d4Shft_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(5),
      SRST => reset,
      CE => Q_n0195,
      CLK => clk,
      O => d4Shft(5),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d4Shft_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(6),
      SRST => reset,
      CE => Q_n0195,
      CLK => clk,
      O => d4Shft(6),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d4Shft_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(7),
      SRST => reset,
      CE => Q_n0195,
      CLK => clk,
      O => d4Shft(7),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  selTap_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0058(0),
      SRST => reset,
      CE => Q_n0194,
      CLK => clk,
      O => NlwRenamedSig_OI_selTap(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  selTap_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0058(1),
      SRST => reset,
      CE => Q_n0194,
      CLK => clk,
      O => NlwRenamedSig_OI_selTap(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  selTap_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0058(2),
      SRST => reset,
      CE => Q_n0194,
      CLK => clk,
      O => NlwRenamedSig_OI_selTap(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  selTap_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0058(3),
      SRST => reset,
      CE => Q_n0194,
      CLK => clk,
      O => NlwRenamedSig_OI_selTap(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  selTap_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0058(4),
      SRST => reset,
      CE => Q_n0194,
      CLK => clk,
      O => NlwRenamedSig_OI_selTap(4),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d0Shft_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(0),
      SRST => reset,
      CE => Q_n0196,
      CLK => clk,
      O => d0Shft(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d0Shft_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(1),
      SRST => reset,
      CE => Q_n0196,
      CLK => clk,
      O => d0Shft(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d0Shft_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(2),
      SRST => reset,
      CE => Q_n0196,
      CLK => clk,
      O => d0Shft(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d0Shft_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(3),
      SRST => reset,
      CE => Q_n0196,
      CLK => clk,
      O => d0Shft(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d0Shft_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(4),
      SRST => reset,
      CE => Q_n0196,
      CLK => clk,
      O => d0Shft(4),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d0Shft_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(5),
      SRST => reset,
      CE => Q_n0196,
      CLK => clk,
      O => d0Shft(5),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d0Shft_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(6),
      SRST => reset,
      CE => Q_n0196,
      CLK => clk,
      O => d0Shft(6),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d0Shft_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(7),
      SRST => reset,
      CE => Q_n0196,
      CLK => clk,
      O => d0Shft(7),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d5Shft_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(4),
      SRST => reset,
      CE => Q_n0201,
      CLK => clk,
      O => d5Shft(4),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d5Shft_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(5),
      SRST => reset,
      CE => Q_n0201,
      CLK => clk,
      O => d5Shft(5),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d5Shft_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(6),
      SRST => reset,
      CE => Q_n0201,
      CLK => clk,
      O => d5Shft(6),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d5Shft_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(7),
      SRST => reset,
      CE => Q_n0201,
      CLK => clk,
      O => d5Shft(7),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d1Shft_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(0),
      SRST => reset,
      CE => Q_n0203,
      CLK => clk,
      O => d1Shft(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d1Shft_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(1),
      SRST => reset,
      CE => Q_n0203,
      CLK => clk,
      O => d1Shft(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d1Shft_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(2),
      SRST => reset,
      CE => Q_n0203,
      CLK => clk,
      O => d1Shft(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d1Shft_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(3),
      SRST => reset,
      CE => Q_n0203,
      CLK => clk,
      O => d1Shft(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d1Shft_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(4),
      SRST => reset,
      CE => Q_n0203,
      CLK => clk,
      O => d1Shft(4),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d1Shft_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(5),
      SRST => reset,
      CE => Q_n0203,
      CLK => clk,
      O => d1Shft(5),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d1Shft_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(6),
      SRST => reset,
      CE => Q_n0203,
      CLK => clk,
      O => d1Shft(6),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d1Shft_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(7),
      SRST => reset,
      CE => Q_n0203,
      CLK => clk,
      O => d1Shft(7),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  resetDcm : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0072,
      SRST => reset,
      CE => Q_n0205_134,
      CLK => clk,
      O => resetDcm_121,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  suShft_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0073(0),
      SRST => reset,
      CE => Q_n0206,
      CLK => clk,
      O => suShft(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  suShft_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0073(1),
      SRST => reset,
      CE => Q_n0206,
      CLK => clk,
      O => suShft(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  suShft_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0073(2),
      SRST => reset,
      CE => Q_n0206,
      CLK => clk,
      O => suShft(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  suShft_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0073(3),
      SRST => reset,
      CE => Q_n0206,
      CLK => clk,
      O => suShft(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  suShft_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0073(4),
      SRST => reset,
      CE => Q_n0206,
      CLK => clk,
      O => suShft(4),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  suShft_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0073(5),
      SRST => reset,
      CE => Q_n0206,
      CLK => clk,
      O => suShft(5),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  suShft_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0073(6),
      SRST => reset,
      CE => Q_n0206,
      CLK => clk,
      O => suShft(6),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  suShft_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0073(7),
      SRST => reset,
      CE => Q_n0206,
      CLK => clk,
      O => suShft(7),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  rstate : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => Q_n0076_129,
      SSET => reset,
      CE => Q_n0208_136,
      CLK => clk,
      O => rstate_127,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  d2Shft_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(0),
      SRST => reset,
      CE => Q_n0209,
      CLK => clk,
      O => d2Shft(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d2Shft_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(1),
      SRST => reset,
      CE => Q_n0209,
      CLK => clk,
      O => d2Shft(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d2Shft_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(2),
      SRST => reset,
      CE => Q_n0209,
      CLK => clk,
      O => d2Shft(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d2Shft_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(3),
      SRST => reset,
      CE => Q_n0209,
      CLK => clk,
      O => d2Shft(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d2Shft_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(4),
      SRST => reset,
      CE => Q_n0209,
      CLK => clk,
      O => d2Shft(4),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d2Shft_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(5),
      SRST => reset,
      CE => Q_n0209,
      CLK => clk,
      O => d2Shft(5),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d2Shft_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(6),
      SRST => reset,
      CE => Q_n0209,
      CLK => clk,
      O => d2Shft(6),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d2Shft_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(7),
      SRST => reset,
      CE => Q_n0209,
      CLK => clk,
      O => d2Shft(7),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  decPosSh_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0080(0),
      SRST => reset,
      CE => Q_n0211_135,
      CLK => clk,
      O => decPosSh(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  decPosSh_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0080(1),
      SRST => reset,
      CE => Q_n0211_135,
      CLK => clk,
      O => decPosSh(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  decPosSh_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0080(2),
      SRST => reset,
      CE => Q_n0211_135,
      CLK => clk,
      O => decPosSh(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  decPosSh_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0080(3),
      SRST => reset,
      CE => Q_n0211_135,
      CLK => clk,
      O => decPosSh(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  decPosSh_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0080(4),
      SRST => reset,
      CE => Q_n0211_135,
      CLK => clk,
      O => decPosSh(4),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  decPosSh_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0080(5),
      SRST => reset,
      CE => Q_n0211_135,
      CLK => clk,
      O => decPosSh(5),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  decPosSh_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0080(6),
      SRST => reset,
      CE => Q_n0211_135,
      CLK => clk,
      O => decPosSh(6),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  decPosSh_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0080(7),
      SRST => reset,
      CE => Q_n0211_135,
      CLK => clk,
      O => decPosSh(7),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  okSelCnt : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => N1,
      SRST => Q_n0081_130,
      CLK => clk,
      O => okSelCnt_122,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  waitOneCycle : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => Q_n0083,
      SSET => reset,
      CE => Q_n0213,
      CLK => clk,
      O => waitOneCycle_131,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  d3Shft_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(0),
      SRST => reset,
      CE => Q_n0216,
      CLK => clk,
      O => d3Shft(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d3Shft_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(1),
      SRST => reset,
      CE => Q_n0216,
      CLK => clk,
      O => d3Shft(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d3Shft_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(2),
      SRST => reset,
      CE => Q_n0216,
      CLK => clk,
      O => d3Shft(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d3Shft_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(3),
      SRST => reset,
      CE => Q_n0216,
      CLK => clk,
      O => d3Shft(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d3Shft_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(4),
      SRST => reset,
      CE => Q_n0216,
      CLK => clk,
      O => d3Shft(4),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d3Shft_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(5),
      SRST => reset,
      CE => Q_n0216,
      CLK => clk,
      O => d3Shft(5),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d3Shft_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(6),
      SRST => reset,
      CE => Q_n0216,
      CLK => clk,
      O => d3Shft(6),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  d3Shft_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0057(7),
      SRST => reset,
      CE => Q_n0216,
      CLK => clk,
      O => d3Shft(7),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  psDoneReg : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => psDone,
      SRST => reset,
      CLK => clk,
      O => psDoneReg_126,
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  waitTwoCycle : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => waitOneCycle_131,
      SRST => reset,
      CE => Q_n0198,
      CLK => clk,
      O => waitTwoCycle_132,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  wait3Cycle : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => waitTwoCycle_132,
      SRST => reset,
      CE => Q_n0210,
      CLK => clk,
      O => wait3Cycle_123,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  psEn_119 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0088,
      SRST => reset,
      CE => Q_n0208_136,
      CLK => clk,
      O => NlwRenamedSig_OI_psEn,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  psinc_val : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0070,
      SRST => reset,
      CE => Q_n0204_133,
      CLK => clk,
      O => NlwRenamedSig_OI_psInc,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  wait4Cycle : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => wait3Cycle_123,
      SRST => reset,
      CE => Q_n0215,
      CLK => clk,
      O => wait4Cycle_124,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  wait5Cycle : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => wait4Cycle_124,
      SRST => reset,
      CE => Q_n0217,
      CLK => clk,
      O => wait5Cycle_125,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  negPhShft_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0074(0),
      SRST => reset,
      CE => Q_n0207,
      CLK => clk,
      O => negPhShft(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  negPhShft_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0074(1),
      SRST => reset,
      CE => Q_n0207,
      CLK => clk,
      O => negPhShft(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  negPhShft_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0074(2),
      SRST => reset,
      CE => Q_n0207,
      CLK => clk,
      O => negPhShft(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  negPhShft_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0074(3),
      SRST => reset,
      CE => Q_n0207,
      CLK => clk,
      O => negPhShft(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  negPhShft_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0074(4),
      SRST => reset,
      CE => Q_n0207,
      CLK => clk,
      O => negPhShft(4),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  negPhShft_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0074(5),
      SRST => reset,
      CE => Q_n0207,
      CLK => clk,
      O => negPhShft(5),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  negPhShft_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0074(6),
      SRST => reset,
      CE => Q_n0207,
      CLK => clk,
      O => negPhShft(6),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  negPhShft_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0074(7),
      SRST => reset,
      CE => Q_n0207,
      CLK => clk,
      O => negPhShft(7),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  posPhShft_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0085(0),
      SRST => reset,
      CE => Q_n0214,
      CLK => clk,
      O => posPhShft(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  posPhShft_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0085(1),
      SRST => reset,
      CE => Q_n0214,
      CLK => clk,
      O => posPhShft(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  posPhShft_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0085(2),
      SRST => reset,
      CE => Q_n0214,
      CLK => clk,
      O => posPhShft(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  posPhShft_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0085(3),
      SRST => reset,
      CE => Q_n0214,
      CLK => clk,
      O => posPhShft(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  posPhShft_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0085(4),
      SRST => reset,
      CE => Q_n0214,
      CLK => clk,
      O => posPhShft(4),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  posPhShft_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0085(5),
      SRST => reset,
      CE => Q_n0214,
      CLK => clk,
      O => posPhShft(5),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  posPhShft_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0085(6),
      SRST => reset,
      CE => Q_n0214,
      CLK => clk,
      O => posPhShft(6),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  posPhShft_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0085(7),
      SRST => reset,
      CE => Q_n0214,
      CLK => clk,
      O => posPhShft(7),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  prevSamp : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0067_128,
      SRST => reset,
      CE => Q_n0202,
      CLK => clk,
      O => prevSamp_120,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  zoShft_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0068(0),
      SRST => reset,
      CLK => clk,
      O => zoShft(0),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  zoShft_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0068(1),
      SRST => reset,
      CLK => clk,
      O => zoShft(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  zoShft_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0068(2),
      SRST => reset,
      CLK => clk,
      O => zoShft(2),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  zoShft_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0068(3),
      SRST => reset,
      CLK => clk,
      O => zoShft(3),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  zoShft_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0068(4),
      SRST => reset,
      CLK => clk,
      O => zoShft(4),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  zoShft_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0068(5),
      SRST => reset,
      CLK => clk,
      O => zoShft(5),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  zoShft_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0068(6),
      SRST => reset,
      CLK => clk,
      O => zoShft(6),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  zoShft_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0068(7),
      SRST => reset,
      CLK => clk,
      O => zoShft(7),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  ozShft_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => N16,
      SRST => reset,
      CLK => clk,
      O => ozShft(0),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  ozShft_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0089(1),
      SRST => reset,
      CLK => clk,
      O => ozShft(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  ozShft_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0089(2),
      SRST => reset,
      CLK => clk,
      O => ozShft(2),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  ozShft_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0089(3),
      SRST => reset,
      CLK => clk,
      O => ozShft(3),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  ozShft_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0089(4),
      SRST => reset,
      CLK => clk,
      O => ozShft(4),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  ozShft_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0089(5),
      SRST => reset,
      CLK => clk,
      O => ozShft(5),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  ozShft_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0089(6),
      SRST => reset,
      CLK => clk,
      O => ozShft(6),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  ozShft_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0089(7),
      SRST => reset,
      CLK => clk,
      O => ozShft(7),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  uPtr_0 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => Q_n0084(0),
      SSET => reset,
      CLK => clk,
      O => uPtr(0),
      CE => VCC,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  uPtr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0084(1),
      SRST => reset,
      CLK => clk,
      O => uPtr(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  uPtr_2 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => Q_n0084(2),
      SSET => reset,
      CLK => clk,
      O => uPtr(2),
      CE => VCC,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  midPt_1 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => Q_n0079(1),
      SSET => reset,
      CLK => clk,
      O => midPt(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  midPt_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0079(2),
      SRST => reset,
      CLK => clk,
      O => midPt(2),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  midPt_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Madd_n0079_n0002,
      SRST => reset,
      CLK => clk,
      O => midPt(3),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  newTap_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => inTapForDqs(2),
      SRST => reset,
      CLK => clk,
      O => newTap(2),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  newTap_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => inTapForDqs(0),
      SRST => reset,
      CLK => clk,
      O => newTap(0),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  newTap_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => inTapForDqs(1),
      SRST => reset,
      CLK => clk,
      O => newTap(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  newTap_3 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => inTapForDqs(3),
      SSET => reset,
      CLK => clk,
      O => newTap(3),
      CE => VCC,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  newTap_4 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => inTapForDqs(4),
      SSET => reset,
      CLK => clk,
      O => newTap(4),
      CE => VCC,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  inTapForDqs_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0059(2),
      SRST => reset,
      CLK => clk,
      O => inTapForDqs(2),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  inTapForDqs_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0059(0),
      SRST => reset,
      CLK => clk,
      O => inTapForDqs(0),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  inTapForDqs_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0059(1),
      SRST => reset,
      CLK => clk,
      O => inTapForDqs(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  inTapForDqs_3 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => Q_n0059(3),
      SSET => reset,
      CLK => clk,
      O => inTapForDqs(3),
      CE => VCC,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  inTapForDqs_4 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => Q_n0059(4),
      SSET => reset,
      CLK => clk,
      O => inTapForDqs(4),
      CE => VCC,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  tapForDqs_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => newTap(2),
      SRST => reset,
      CE => okSelCnt_122,
      CLK => clk,
      O => tapForDqs(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  tapForDqs_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => newTap(0),
      SRST => reset,
      CE => okSelCnt_122,
      CLK => clk,
      O => tapForDqs(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  tapForDqs_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => newTap(1),
      SRST => reset,
      CE => okSelCnt_122,
      CLK => clk,
      O => tapForDqs(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  tapForDqs_3 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => newTap(3),
      SSET => reset,
      CE => okSelCnt_122,
      CLK => clk,
      O => tapForDqs(3),
      SET => GND,
      RST => GND,
      SRST => GND
    );
  tapForDqs_4 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => newTap(4),
      SSET => reset,
      CE => okSelCnt_122,
      CLK => clk,
      O => tapForDqs(4),
      SET => GND,
      RST => GND,
      SRST => GND
    );
  decNegSh_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(0),
      SRST => Q_n0060,
      CE => N101,
      CLK => clk,
      O => decNegSh(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  decNegSh_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(1),
      SRST => Q_n0060,
      CE => N101,
      CLK => clk,
      O => decNegSh(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  decNegSh_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(2),
      SRST => Q_n0060,
      CE => N101,
      CLK => clk,
      O => decNegSh(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  decNegSh_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(3),
      SRST => Q_n0060,
      CE => N101,
      CLK => clk,
      O => decNegSh(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  decNegSh_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(4),
      SRST => Q_n0060,
      CE => N101,
      CLK => clk,
      O => decNegSh(4),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  decNegSh_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(5),
      SRST => Q_n0060,
      CE => N101,
      CLK => clk,
      O => decNegSh(5),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  decNegSh_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(6),
      SRST => Q_n0060,
      CE => N101,
      CLK => clk,
      O => decNegSh(6),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  decNegSh_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(7),
      SRST => Q_n0060,
      CE => N101,
      CLK => clk,
      O => decNegSh(7),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  cal_ctl_n0068_0_lut : X_LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      ADR0 => suShft(0),
      ADR1 => posPhShft(0),
      O => N8
    );
  cal_ctl_n0068_0_cy : X_MUX2
    port map (
      IB => N1,
      IA => suShft(0),
      SEL => N8,
      O => cal_ctl_n0068_0_cyo
    );
  cal_ctl_n0068_0_xor : X_XOR2
    port map (
      I0 => N1,
      I1 => N8,
      O => Q_n0068(0)
    );
  cal_ctl_n0068_1_lut : X_LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      ADR0 => suShft(1),
      ADR1 => posPhShft(1),
      O => N9
    );
  cal_ctl_n0068_1_cy : X_MUX2
    port map (
      IB => cal_ctl_n0068_0_cyo,
      IA => suShft(1),
      SEL => N9,
      O => cal_ctl_n0068_1_cyo
    );
  cal_ctl_n0068_1_xor : X_XOR2
    port map (
      I0 => cal_ctl_n0068_0_cyo,
      I1 => N9,
      O => Q_n0068(1)
    );
  cal_ctl_n0068_2_lut : X_LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      ADR0 => suShft(2),
      ADR1 => posPhShft(2),
      O => N10
    );
  cal_ctl_n0068_2_cy : X_MUX2
    port map (
      IB => cal_ctl_n0068_1_cyo,
      IA => suShft(2),
      SEL => N10,
      O => cal_ctl_n0068_2_cyo
    );
  cal_ctl_n0068_2_xor : X_XOR2
    port map (
      I0 => cal_ctl_n0068_1_cyo,
      I1 => N10,
      O => Q_n0068(2)
    );
  cal_ctl_n0068_3_lut : X_LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      ADR0 => suShft(3),
      ADR1 => posPhShft(3),
      O => N11
    );
  cal_ctl_n0068_3_cy : X_MUX2
    port map (
      IB => cal_ctl_n0068_2_cyo,
      IA => suShft(3),
      SEL => N11,
      O => cal_ctl_n0068_3_cyo
    );
  cal_ctl_n0068_3_xor : X_XOR2
    port map (
      I0 => cal_ctl_n0068_2_cyo,
      I1 => N11,
      O => Q_n0068(3)
    );
  cal_ctl_n0068_4_lut : X_LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      ADR0 => suShft(4),
      ADR1 => posPhShft(4),
      O => N12
    );
  cal_ctl_n0068_4_cy : X_MUX2
    port map (
      IB => cal_ctl_n0068_3_cyo,
      IA => suShft(4),
      SEL => N12,
      O => cal_ctl_n0068_4_cyo
    );
  cal_ctl_n0068_4_xor : X_XOR2
    port map (
      I0 => cal_ctl_n0068_3_cyo,
      I1 => N12,
      O => Q_n0068(4)
    );
  cal_ctl_n0068_5_lut : X_LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      ADR0 => suShft(5),
      ADR1 => posPhShft(5),
      O => N13
    );
  cal_ctl_n0068_5_cy : X_MUX2
    port map (
      IB => cal_ctl_n0068_4_cyo,
      IA => suShft(5),
      SEL => N13,
      O => cal_ctl_n0068_5_cyo
    );
  cal_ctl_n0068_5_xor : X_XOR2
    port map (
      I0 => cal_ctl_n0068_4_cyo,
      I1 => N13,
      O => Q_n0068(5)
    );
  cal_ctl_n0068_6_lut : X_LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      ADR0 => suShft(6),
      ADR1 => posPhShft(6),
      O => N14
    );
  cal_ctl_n0068_6_cy : X_MUX2
    port map (
      IB => cal_ctl_n0068_5_cyo,
      IA => suShft(6),
      SEL => N14,
      O => cal_ctl_n0068_6_cyo
    );
  cal_ctl_n0068_6_xor : X_XOR2
    port map (
      I0 => cal_ctl_n0068_5_cyo,
      I1 => N14,
      O => Q_n0068(6)
    );
  cal_ctl_n0068_7_lut : X_LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      ADR0 => suShft(7),
      ADR1 => posPhShft(7),
      O => N15
    );
  cal_ctl_n0068_7_xor : X_XOR2
    port map (
      I0 => cal_ctl_n0068_6_cyo,
      I1 => N15,
      O => Q_n0068(7)
    );
  cal_ctl_n0089_0_lut : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => negPhShft(0),
      ADR1 => suShft(0),
      O => N16
    );
  cal_ctl_n0089_0_cy : X_MUX2
    port map (
      IB => uPtr(3),
      IA => negPhShft(0),
      SEL => N16,
      O => cal_ctl_n0089_0_cyo
    );
  cal_ctl_n0089_1_lut : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => negPhShft(1),
      ADR1 => suShft(1),
      O => N17
    );
  cal_ctl_n0089_1_cy : X_MUX2
    port map (
      IB => cal_ctl_n0089_0_cyo,
      IA => negPhShft(1),
      SEL => N17,
      O => cal_ctl_n0089_1_cyo
    );
  cal_ctl_n0089_1_xor : X_XOR2
    port map (
      I0 => cal_ctl_n0089_0_cyo,
      I1 => N17,
      O => Q_n0089(1)
    );
  cal_ctl_n0089_2_lut : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => negPhShft(2),
      ADR1 => suShft(2),
      O => N18
    );
  cal_ctl_n0089_2_cy : X_MUX2
    port map (
      IB => cal_ctl_n0089_1_cyo,
      IA => negPhShft(2),
      SEL => N18,
      O => cal_ctl_n0089_2_cyo
    );
  cal_ctl_n0089_2_xor : X_XOR2
    port map (
      I0 => cal_ctl_n0089_1_cyo,
      I1 => N18,
      O => Q_n0089(2)
    );
  cal_ctl_n0089_3_lut : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => negPhShft(3),
      ADR1 => suShft(3),
      O => N19
    );
  cal_ctl_n0089_3_cy : X_MUX2
    port map (
      IB => cal_ctl_n0089_2_cyo,
      IA => negPhShft(3),
      SEL => N19,
      O => cal_ctl_n0089_3_cyo
    );
  cal_ctl_n0089_3_xor : X_XOR2
    port map (
      I0 => cal_ctl_n0089_2_cyo,
      I1 => N19,
      O => Q_n0089(3)
    );
  cal_ctl_n0089_4_lut : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => negPhShft(4),
      ADR1 => suShft(4),
      O => N20
    );
  cal_ctl_n0089_4_cy : X_MUX2
    port map (
      IB => cal_ctl_n0089_3_cyo,
      IA => negPhShft(4),
      SEL => N20,
      O => cal_ctl_n0089_4_cyo
    );
  cal_ctl_n0089_4_xor : X_XOR2
    port map (
      I0 => cal_ctl_n0089_3_cyo,
      I1 => N20,
      O => Q_n0089(4)
    );
  cal_ctl_n0089_5_lut : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => negPhShft(5),
      ADR1 => suShft(5),
      O => N21
    );
  cal_ctl_n0089_5_cy : X_MUX2
    port map (
      IB => cal_ctl_n0089_4_cyo,
      IA => negPhShft(5),
      SEL => N21,
      O => cal_ctl_n0089_5_cyo
    );
  cal_ctl_n0089_5_xor : X_XOR2
    port map (
      I0 => cal_ctl_n0089_4_cyo,
      I1 => N21,
      O => Q_n0089(5)
    );
  cal_ctl_n0089_6_lut : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => negPhShft(6),
      ADR1 => suShft(6),
      O => N22
    );
  cal_ctl_n0089_6_cy : X_MUX2
    port map (
      IB => cal_ctl_n0089_5_cyo,
      IA => negPhShft(6),
      SEL => N22,
      O => cal_ctl_n0089_6_cyo
    );
  cal_ctl_n0089_6_xor : X_XOR2
    port map (
      I0 => cal_ctl_n0089_5_cyo,
      I1 => N22,
      O => Q_n0089(6)
    );
  cal_ctl_n0089_7_lut : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => negPhShft(7),
      ADR1 => suShft(7),
      O => N23
    );
  cal_ctl_n0089_7_xor : X_XOR2
    port map (
      I0 => cal_ctl_n0089_6_cyo,
      I1 => N23,
      O => Q_n0089(7)
    );
  state_FFd1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => state_FFd1_In,
      SRST => reset,
      CE => Q_n0194,
      CLK => clk,
      O => state_FFd1_137,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  state_FFd2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => state_FFd2_In,
      SRST => reset,
      CE => Q_n0194,
      CLK => clk,
      O => state_FFd2_138,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  state_FFd3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => state_FFd3_In,
      SRST => reset,
      CE => Q_n0194,
      CLK => clk,
      O => state_FFd3_139,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  state_FFd4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => state_FFd4_In,
      SRST => reset,
      CE => Q_n0194,
      CLK => clk,
      O => state_FFd4_140,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  lPtr_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0092(0),
      SRST => Q_n0090,
      SSET => Q_n0091,
      CLK => clk,
      O => lPtr(0),
      CE => VCC,
      SET => GND,
      RST => GND
    );
  lPtr_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0092(1),
      SRST => lPtr_n0000,
      CLK => clk,
      O => lPtr(1),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  lPtr_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Q_n0092(2),
      SRST => lPtr_n0000,
      CLK => clk,
      O => lPtr(2),
      CE => VCC,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  selCnt_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => selCnt_n0000(0),
      SRST => selCnt_Sclr_INV,
      CE => Q_n0200,
      CLK => clk,
      O => selCnt(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  selCnt_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => selCnt_n0000(1),
      SRST => selCnt_Sclr_INV,
      CE => Q_n0200,
      CLK => clk,
      O => selCnt(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  selCnt_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => selCnt_n0000(2),
      SRST => selCnt_Sclr_INV,
      CE => Q_n0200,
      CLK => clk,
      O => selCnt(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  selCnt_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => selCnt_n0000(3),
      SRST => selCnt_Sclr_INV,
      CE => Q_n0200,
      CLK => clk,
      O => selCnt(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Q_n0057_7_1 : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => hxSamp1,
      ADR1 => ozShft(7),
      ADR2 => prevSamp_120,
      ADR3 => zoShft(7),
      O => Q_n0057(7)
    );
  Q_n0057_6_1 : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => hxSamp1,
      ADR1 => ozShft(6),
      ADR2 => prevSamp_120,
      ADR3 => zoShft(6),
      O => Q_n0057(6)
    );
  Q_n0057_5_1 : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => hxSamp1,
      ADR1 => ozShft(5),
      ADR2 => prevSamp_120,
      ADR3 => zoShft(5),
      O => Q_n0057(5)
    );
  Q_n0057_4_1 : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => hxSamp1,
      ADR1 => ozShft(4),
      ADR2 => prevSamp_120,
      ADR3 => zoShft(4),
      O => Q_n0057(4)
    );
  Q_n0057_3_1 : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => hxSamp1,
      ADR1 => ozShft(3),
      ADR2 => prevSamp_120,
      ADR3 => zoShft(3),
      O => Q_n0057(3)
    );
  Q_n0057_2_1 : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => hxSamp1,
      ADR1 => ozShft(2),
      ADR2 => prevSamp_120,
      ADR3 => zoShft(2),
      O => Q_n0057(2)
    );
  Q_n0057_1_1 : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => hxSamp1,
      ADR1 => ozShft(1),
      ADR2 => prevSamp_120,
      ADR3 => zoShft(1),
      O => Q_n0057(1)
    );
  Q_n0057_0_1 : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => hxSamp1,
      ADR1 => ozShft(0),
      ADR2 => prevSamp_120,
      ADR3 => zoShft(0),
      O => Q_n0057(0)
    );
  Q_n0073_7_1 : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => phSamp1,
      ADR1 => negPhShft(7),
      ADR2 => prevSamp_120,
      ADR3 => posPhShft(7),
      O => Q_n0073(7)
    );
  Q_n0073_6_1 : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => phSamp1,
      ADR1 => negPhShft(6),
      ADR2 => prevSamp_120,
      ADR3 => posPhShft(6),
      O => Q_n0073(6)
    );
  Q_n0073_5_1 : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => phSamp1,
      ADR1 => negPhShft(5),
      ADR2 => prevSamp_120,
      ADR3 => posPhShft(5),
      O => Q_n0073(5)
    );
  Q_n0073_4_1 : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => phSamp1,
      ADR1 => negPhShft(4),
      ADR2 => prevSamp_120,
      ADR3 => posPhShft(4),
      O => Q_n0073(4)
    );
  Q_n0073_3_1 : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => phSamp1,
      ADR1 => negPhShft(3),
      ADR2 => prevSamp_120,
      ADR3 => posPhShft(3),
      O => Q_n0073(3)
    );
  Q_n0073_2_1 : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => phSamp1,
      ADR1 => negPhShft(2),
      ADR2 => prevSamp_120,
      ADR3 => posPhShft(2),
      O => Q_n0073(2)
    );
  Q_n0073_1_1 : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => phSamp1,
      ADR1 => negPhShft(1),
      ADR2 => prevSamp_120,
      ADR3 => posPhShft(1),
      O => Q_n0073(1)
    );
  Q_n0073_0_1 : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => phSamp1,
      ADR1 => negPhShft(0),
      ADR2 => prevSamp_120,
      ADR3 => posPhShft(0),
      O => Q_n0073(0)
    );
  Q_n01941 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => wait4Cycle_124,
      ADR1 => wait5Cycle_125,
      ADR2 => waitOneCycle_131,
      ADR3 => waitTwoCycle_132,
      O => N41
    );
  Q_n01942 : X_LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      ADR0 => dcmlocked,
      ADR1 => resetDcm_121,
      ADR2 => wait3Cycle_123,
      ADR3 => N41,
      O => Q_n0194
    );
  Q_n01981 : X_LUT4
    generic map(
      INIT => X"2220"
    )
    port map (
      ADR0 => dcmlocked,
      ADR1 => resetDcm_121,
      ADR2 => waitOneCycle_131,
      ADR3 => waitTwoCycle_132,
      O => Q_n0198
    );
  Q_n02001 : X_LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      ADR0 => reset,
      ADR1 => wait4Cycle_124,
      O => Q_n0200
    );
  Q_n02021 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => wait3Cycle_123,
      ADR1 => wait4Cycle_124,
      ADR2 => waitOneCycle_131,
      ADR3 => waitTwoCycle_132,
      O => N45
    );
  Q_n02022 : X_LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      ADR0 => dcmlocked,
      ADR1 => resetDcm_121,
      ADR2 => N45,
      O => Q_n0202
    );
  Q_n02101 : X_LUT4
    generic map(
      INIT => X"FFAB"
    )
    port map (
      ADR0 => waitOneCycle_131,
      ADR1 => wait3Cycle_123,
      ADR2 => waitTwoCycle_132,
      ADR3 => resetDcm_121,
      O => N47
    );
  Q_n02102 : X_LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      ADR0 => N47,
      ADR1 => dcmlocked,
      O => Q_n0210
    );
  Q_n02151 : X_LUT4
    generic map(
      INIT => X"FFAB"
    )
    port map (
      ADR0 => waitTwoCycle_132,
      ADR1 => wait3Cycle_123,
      ADR2 => wait4Cycle_124,
      ADR3 => waitOneCycle_131,
      O => N49
    );
  Q_n02152 : X_LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      ADR0 => dcmlocked,
      ADR1 => resetDcm_121,
      ADR2 => N49,
      O => Q_n0215
    );
  Q_n02171 : X_LUT4
    generic map(
      INIT => X"FFAB"
    )
    port map (
      ADR0 => waitTwoCycle_132,
      ADR1 => wait4Cycle_124,
      ADR2 => wait5Cycle_125,
      ADR3 => waitOneCycle_131,
      O => N51
    );
  Q_n02172 : X_LUT4
    generic map(
      INIT => X"0002"
    )
    port map (
      ADR0 => dcmlocked,
      ADR1 => resetDcm_121,
      ADR2 => wait3Cycle_123,
      ADR3 => N51,
      O => Q_n0217
    );
  cal_ctl_Result_0_xor11 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => decNegSh(0),
      O => Result(0),
      ADR1 => GND
    );
  cal_ctl_Result_1_xor11 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => decNegSh(0),
      ADR1 => decNegSh(1),
      O => Result(1)
    );
  cal_ctl_Result_2_xor11 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => decNegSh(0),
      ADR1 => decNegSh(1),
      ADR2 => decNegSh(2),
      O => Result(2)
    );
  cal_ctl_Result_3_xor11 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => decNegSh(0),
      ADR1 => decNegSh(3),
      ADR2 => decNegSh(1),
      ADR3 => decNegSh(2),
      O => Result(3)
    );
  Q_n0074_0_1 : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => negPhShft(0),
      ADR1 => psDoneReg_126,
      ADR2 => NlwRenamedSig_OI_psInc,
      ADR3 => rstate_127,
      O => Q_n0074(0)
    );
  Q_n0074_1_1 : X_LUT3
    generic map(
      INIT => X"BF"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_psInc,
      ADR1 => psDoneReg_126,
      ADR2 => rstate_127,
      O => N59
    );
  Q_n0074_1_2 : X_LUT3
    generic map(
      INIT => X"06"
    )
    port map (
      ADR0 => negPhShft(0),
      ADR1 => negPhShft(1),
      ADR2 => N59,
      O => Q_n0074(1)
    );
  Q_n0074_2_1 : X_LUT3
    generic map(
      INIT => X"BF"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_psInc,
      ADR1 => psDoneReg_126,
      ADR2 => rstate_127,
      O => N61
    );
  Q_n0074_2_2 : X_LUT4
    generic map(
      INIT => X"060C"
    )
    port map (
      ADR0 => negPhShft(1),
      ADR1 => negPhShft(2),
      ADR2 => N61,
      ADR3 => negPhShft(0),
      O => Q_n0074(2)
    );
  Q_n0085_0_1 : X_LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      ADR0 => posPhShft(0),
      ADR1 => psDoneReg_126,
      ADR2 => NlwRenamedSig_OI_psInc,
      ADR3 => rstate_127,
      O => Q_n0085(0)
    );
  Q_n0085_1_1 : X_LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      ADR0 => psDoneReg_126,
      ADR1 => NlwRenamedSig_OI_psInc,
      ADR2 => rstate_127,
      O => N64
    );
  Q_n0085_1_2 : X_LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      ADR0 => posPhShft(0),
      ADR1 => posPhShft(1),
      ADR2 => N64,
      O => Q_n0085(1)
    );
  Q_n0085_2_1 : X_LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      ADR0 => psDoneReg_126,
      ADR1 => NlwRenamedSig_OI_psInc,
      ADR2 => rstate_127,
      O => N66
    );
  Q_n0085_2_2 : X_LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      ADR0 => posPhShft(0),
      ADR1 => posPhShft(2),
      ADR2 => N66,
      ADR3 => posPhShft(1),
      O => Q_n0085(2)
    );
  Madd_n0079_LPM_ADD_SUB_2_0_xo_1_1 : X_LUT4
    generic map(
      INIT => X"965A"
    )
    port map (
      ADR0 => lPtr(1),
      ADR1 => lPtr(0),
      ADR2 => uPtr(1),
      ADR3 => uPtr(0),
      O => Q_n0079(1)
    );
  selCnt_Madd_n0000_n00091 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => selCnt(0),
      O => selCnt_n0000(0),
      ADR1 => GND
    );
  selCnt_Madd_n0000_Mxor_Result_3_Result1 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => selCnt(0),
      ADR1 => selCnt(3),
      ADR2 => selCnt(1),
      ADR3 => selCnt(2),
      O => selCnt_n0000(3)
    );
  selCnt_Madd_n0000_Mxor_Result_2_Result1 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => selCnt(0),
      ADR1 => selCnt(1),
      ADR2 => selCnt(2),
      O => selCnt_n0000(2)
    );
  selCnt_Madd_n0000_Mxor_Result_1_Result1 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => selCnt(0),
      ADR1 => selCnt(1),
      O => selCnt_n0000(1)
    );
  Q_n0059_4_1 : X_LUT4
    generic map(
      INIT => X"BBFC"
    )
    port map (
      ADR0 => inTapForDqs(4),
      ADR1 => midPt(2),
      ADR2 => midPt(1),
      ADR3 => midPt(3),
      O => Q_n0059(4)
    );
  Q_n0059_3_1 : X_LUT3
    generic map(
      INIT => X"E6"
    )
    port map (
      ADR0 => midPt(3),
      ADR1 => midPt(2),
      ADR2 => inTapForDqs(3),
      O => Q_n0059(3)
    );
  Q_n0059_2_1 : X_LUT4
    generic map(
      INIT => X"BC8C"
    )
    port map (
      ADR0 => inTapForDqs(2),
      ADR1 => midPt(3),
      ADR2 => midPt(2),
      ADR3 => midPt(1),
      O => Q_n0059(2)
    );
  Q_n0059_1_1 : X_LUT3
    generic map(
      INIT => X"A2"
    )
    port map (
      ADR0 => midPt(3),
      ADR1 => midPt(2),
      ADR2 => inTapForDqs(1),
      O => Q_n0059(1)
    );
  Q_n0059_0_1 : X_LUT4
    generic map(
      INIT => X"8C80"
    )
    port map (
      ADR0 => inTapForDqs(0),
      ADR1 => midPt(3),
      ADR2 => midPt(2),
      ADR3 => midPt(1),
      O => Q_n0059(0)
    );
  cal_ctl_Result_4_xor11 : X_MUX2
    port map (
      IA => decNegSh_4_rt_141,
      IB => N79,
      SEL => decNegSh(2),
      O => Result(4)
    );
  cal_ctl_Result_4_xor11_G : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => decNegSh(0),
      ADR1 => decNegSh(4),
      ADR2 => decNegSh(1),
      ADR3 => decNegSh(3),
      O => N79
    );
  Q_n0084_2_1 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => Q_n0125,
      ADR1 => Q_n0126,
      O => Q_n0084(2)
    );
  Ker421 : X_LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      ADR0 => rstate_127,
      ADR1 => psDoneReg_126,
      ADR2 => NlwRenamedSig_OI_psInc,
      O => N421
    );
  Ker431 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => psDoneReg_126,
      ADR1 => NlwRenamedSig_OI_psInc,
      ADR2 => rstate_127,
      O => N43
    );
  Madd_n0079_n00021 : X_LUT3
    generic map(
      INIT => X"E8"
    )
    port map (
      ADR0 => Madd_n0079_n0001,
      ADR1 => uPtr(2),
      ADR2 => lPtr(2),
      O => Madd_n0079_n0002
    );
  Q_n0085_4_1 : X_LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      ADR0 => posPhShft(4),
      ADR1 => cal_ctl_n0097_3_cyo,
      ADR2 => N421,
      O => Q_n0085(4)
    );
  Q_n0074_4_1 : X_LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      ADR0 => negPhShft(4),
      ADR1 => cal_ctl_n0095_3_cyo,
      ADR2 => N43,
      O => Q_n0074(4)
    );
  Madd_n0079_LPM_ADD_SUB_2_01_xo_1_1 : X_LUT3
    generic map(
      INIT => X"96"
    )
    port map (
      ADR0 => lPtr(2),
      ADR1 => Madd_n0079_n0001,
      ADR2 => uPtr(2),
      O => Q_n0079(2)
    );
  Q_n01251 : X_LUT4
    generic map(
      INIT => X"0133"
    )
    port map (
      ADR0 => d5Shft(5),
      ADR1 => d5Shft(7),
      ADR2 => d5Shft(4),
      ADR3 => d5Shft(6),
      O => Q_n0125
    );
  Q_n01261 : X_LUT4
    generic map(
      INIT => X"0133"
    )
    port map (
      ADR0 => d4Shft(5),
      ADR1 => d4Shft(7),
      ADR2 => d4Shft(4),
      ADR3 => d4Shft(6),
      O => Q_n0126
    );
  Q_n0074_5_1 : X_LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      ADR0 => cal_ctl_n0095_3_cyo,
      ADR1 => negPhShft(5),
      ADR2 => N43,
      ADR3 => negPhShft(4),
      O => Q_n0074(5)
    );
  Madd_n0079_n00011 : X_LUT4
    generic map(
      INIT => X"F880"
    )
    port map (
      ADR0 => lPtr(0),
      ADR1 => uPtr(0),
      ADR2 => uPtr(1),
      ADR3 => lPtr(1),
      O => Madd_n0079_n0001
    );
  Q_n0085_5_1 : X_LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      ADR0 => cal_ctl_n0097_3_cyo,
      ADR1 => posPhShft(5),
      ADR2 => N421,
      ADR3 => posPhShft(4),
      O => Q_n0085(5)
    );
  Ker401 : X_LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      ADR0 => psDoneReg_126,
      ADR1 => dcmlocked,
      ADR2 => rstate_127,
      O => N40
    );
  Q_n02131 : X_LUT4
    generic map(
      INIT => X"2220"
    )
    port map (
      ADR0 => dcmlocked,
      ADR1 => resetDcm_121,
      ADR2 => N38,
      ADR3 => waitOneCycle_131,
      O => Q_n0213
    );
  cal_ctl_Result_5_xor11 : X_LUT2
    generic map(
      INIT => X"6"
    )
    port map (
      ADR0 => decNegSh(5),
      ADR1 => cal_ctl_Result_4_cyo,
      O => Result(5)
    );
  cal_ctl_Result_6_xor11 : X_LUT3
    generic map(
      INIT => X"78"
    )
    port map (
      ADR0 => decNegSh(5),
      ADR1 => cal_ctl_Result_4_cyo,
      ADR2 => decNegSh(6),
      O => Result(6)
    );
  cal_ctl_Result_7_xor11 : X_LUT4
    generic map(
      INIT => X"6CCC"
    )
    port map (
      ADR0 => decNegSh(5),
      ADR1 => decNegSh(7),
      ADR2 => cal_ctl_Result_4_cyo,
      ADR3 => decNegSh(6),
      O => Result(7)
    );
  cal_ctl_Result_4_cy1_SW0 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => decNegSh(1),
      ADR1 => decNegSh(0),
      O => N171
    );
  cal_ctl_Result_4_cy1 : X_LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      ADR0 => decNegSh(4),
      ADR1 => decNegSh(3),
      ADR2 => decNegSh(2),
      ADR3 => N171,
      O => cal_ctl_Result_4_cyo
    );
  Ker131 : X_LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      ADR0 => state_FFd2_138,
      ADR1 => state_FFd4_140,
      O => N131
    );
  Q_n0084_1_SW0 : X_LUT4
    generic map(
      INIT => X"FFFD"
    )
    port map (
      ADR0 => N211,
      ADR1 => d3Shft(7),
      ADR2 => Q_n0126,
      ADR3 => Q_n0125,
      O => N24
    );
  Q_n0058_0_SW0 : X_LUT4
    generic map(
      INIT => X"FF5D"
    )
    port map (
      ADR0 => state_FFd1_137,
      ADR1 => state_FFd2_138,
      ADR2 => NlwRenamedSig_OI_selTap(0),
      ADR3 => state_FFd3_139,
      O => N27
    );
  Q_n0058_0_Q : X_LUT4
    generic map(
      INIT => X"A2AE"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_selTap(0),
      ADR1 => state_FFd4_140,
      ADR2 => N32,
      ADR3 => N27,
      O => Q_n0058(0)
    );
  Q_n0058_1_SW0 : X_LUT4
    generic map(
      INIT => X"AAC4"
    )
    port map (
      ADR0 => state_FFd2_138,
      ADR1 => state_FFd1_137,
      ADR2 => NlwRenamedSig_OI_selTap(1),
      ADR3 => state_FFd3_139,
      O => N29
    );
  Q_n0058_1_Q : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => state_FFd4_140,
      ADR1 => NlwRenamedSig_OI_selTap(1),
      ADR2 => N32,
      ADR3 => N29,
      O => Q_n0058(1)
    );
  Q_n0092_2_1 : X_LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      ADR0 => Q_n0131,
      ADR1 => Q_n0132,
      O => Q_n0092(2)
    );
  Q_n0092_1_1 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => Q_n0131,
      ADR1 => Q_n0132,
      O => Q_n0092(1)
    );
  Q_n0085_6_1 : X_LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      ADR0 => posPhShft(6),
      ADR1 => cal_ctl_n0097_5_cyo,
      ADR2 => N421,
      O => Q_n0085(6)
    );
  Q_n0074_6_1 : X_LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      ADR0 => negPhShft(6),
      ADR1 => cal_ctl_n0095_5_cyo,
      ADR2 => N43,
      O => Q_n0074(6)
    );
  Q_n00831 : X_LUT4
    generic map(
      INIT => X"5501"
    )
    port map (
      ADR0 => waitOneCycle_131,
      ADR1 => N311,
      ADR2 => N32,
      ADR3 => N35,
      O => Q_n0083
    );
  Q_n00721 : X_LUT4
    generic map(
      INIT => X"5501"
    )
    port map (
      ADR0 => resetDcm_121,
      ADR1 => N311,
      ADR2 => N32,
      ADR3 => N35,
      O => Q_n0072
    );
  Q_n0085_7_1 : X_LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      ADR0 => cal_ctl_n0097_5_cyo,
      ADR1 => posPhShft(7),
      ADR2 => N421,
      ADR3 => posPhShft(6),
      O => Q_n0085(7)
    );
  Q_n0074_7_1 : X_LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      ADR0 => cal_ctl_n0095_5_cyo,
      ADR1 => negPhShft(7),
      ADR2 => N43,
      ADR3 => negPhShft(6),
      O => Q_n0074(7)
    );
  Ker161 : X_LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      ADR0 => state_FFd1_137,
      ADR1 => state_FFd4_140,
      O => N161
    );
  Ker2111 : X_LUT3
    generic map(
      INIT => X"1F"
    )
    port map (
      ADR0 => d3Shft(5),
      ADR1 => d3Shft(4),
      ADR2 => d3Shft(6),
      O => N211
    );
  cal_ctl_n0096_5_cy11 : X_LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      ADR0 => cal_ctl_n0096_3_cyo,
      ADR1 => decPosSh(4),
      ADR2 => decPosSh(5),
      O => cal_ctl_n0096_5_cyo
    );
  cal_ctl_n0097_5_cy11 : X_LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      ADR0 => cal_ctl_n0097_3_cyo,
      ADR1 => posPhShft(4),
      ADR2 => posPhShft(5),
      O => cal_ctl_n0097_5_cyo
    );
  cal_ctl_n0095_5_cy11 : X_LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      ADR0 => cal_ctl_n0095_3_cyo,
      ADR1 => negPhShft(4),
      ADR2 => negPhShft(5),
      O => cal_ctl_n0095_5_cyo
    );
  cal_ctl_n0095_3_cy11 : X_LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      ADR0 => negPhShft(2),
      ADR1 => negPhShft(3),
      ADR2 => negPhShft(0),
      ADR3 => negPhShft(1),
      O => cal_ctl_n0095_3_cyo
    );
  cal_ctl_n0096_3_cy11 : X_LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      ADR0 => decPosSh(2),
      ADR1 => decPosSh(3),
      ADR2 => decPosSh(0),
      ADR3 => decPosSh(1),
      O => cal_ctl_n0096_3_cyo
    );
  cal_ctl_n0097_3_cy11 : X_LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      ADR0 => posPhShft(2),
      ADR1 => posPhShft(3),
      ADR2 => posPhShft(0),
      ADR3 => posPhShft(1),
      O => cal_ctl_n0097_3_cyo
    );
  Q_n0080_3_SW0 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => decPosSh(1),
      ADR1 => decPosSh(0),
      O => N491
    );
  Q_n0080_3_Q : X_LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      ADR0 => decPosSh(2),
      ADR1 => decPosSh(3),
      ADR2 => Q_n0103,
      ADR3 => N491,
      O => Q_n0080(3)
    );
  Q_n013215 : X_LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      ADR0 => d3Shft(3),
      ADR1 => d3Shft(1),
      ADR2 => d3Shft(0),
      ADR3 => d3Shft(2),
      O => N58
    );
  Q_n013216 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => d3Shft(4),
      ADR1 => d3Shft(5),
      O => N591
    );
  Q_n013115 : X_LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      ADR0 => d2Shft(3),
      ADR1 => d2Shft(1),
      ADR2 => d2Shft(0),
      ADR3 => d2Shft(2),
      O => N71
    );
  Q_n013116 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => d2Shft(4),
      ADR1 => d2Shft(5),
      O => N72
    );
  state_FFd4_In7 : X_LUT4
    generic map(
      INIT => X"2220"
    )
    port map (
      ADR0 => psDoneReg_126,
      ADR1 => state_FFd4_140,
      ADR2 => state_FFd2_138,
      ADR3 => state_FFd1_137,
      O => state_FFd4_In_map93
    );
  state_FFd4_In24 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => state_FFd2_138,
      ADR1 => state_FFd3_139,
      O => state_FFd4_In_map100
    );
  state_FFd4_In33 : X_LUT4
    generic map(
      INIT => X"F666"
    )
    port map (
      ADR0 => prevSamp_120,
      ADR1 => hxSamp1,
      ADR2 => state_FFd1_137,
      ADR3 => state_FFd4_In_map100,
      O => state_FFd4_In_map102
    );
  state_FFd4_In46 : X_LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      ADR0 => N35,
      ADR1 => state_FFd4_140,
      ADR2 => state_FFd4_In_map102,
      ADR3 => state_FFd4_In_map93,
      O => state_FFd4_In
    );
  Q_n0092_0_28 : X_LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      ADR0 => d4Shft(7),
      ADR1 => d4Shft(6),
      O => N138
    );
  Q_n0092_0_56 : X_LUT4
    generic map(
      INIT => X"0F08"
    )
    port map (
      ADR0 => N134,
      ADR1 => N138,
      ADR2 => Q_n0131,
      ADR3 => Q_n0132,
      O => Q_n0092(0)
    );
  Ker45_SW0 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => wait5Cycle_125,
      ADR1 => wait4Cycle_124,
      ADR2 => wait3Cycle_123,
      ADR3 => waitOneCycle_131,
      O => N156
    );
  Q_n0080_0_1 : X_LUT2
    generic map(
      INIT => X"4"
    )
    port map (
      ADR0 => decPosSh(0),
      ADR1 => Q_n0103,
      O => Q_n0080(0)
    );
  Ker221 : X_LUT3
    generic map(
      INIT => X"1F"
    )
    port map (
      ADR0 => d2Shft(5),
      ADR1 => d2Shft(4),
      ADR2 => d2Shft(6),
      O => N221
    );
  Q_n0080_4_1 : X_LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      ADR0 => decPosSh(4),
      ADR1 => cal_ctl_n0096_3_cyo,
      ADR2 => Q_n0103,
      O => Q_n0080(4)
    );
  Q_n0080_6_1 : X_LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      ADR0 => decPosSh(6),
      ADR1 => cal_ctl_n0096_5_cyo,
      ADR2 => Q_n0103,
      O => Q_n0080(6)
    );
  Q_n0080_1_1 : X_LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      ADR0 => decPosSh(0),
      ADR1 => decPosSh(1),
      ADR2 => Q_n0103,
      O => Q_n0080(1)
    );
  Q_n0080_2_1 : X_LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      ADR0 => decPosSh(0),
      ADR1 => decPosSh(2),
      ADR2 => Q_n0103,
      ADR3 => decPosSh(1),
      O => Q_n0080(2)
    );
  Q_n0080_7_1 : X_LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      ADR0 => cal_ctl_n0096_5_cyo,
      ADR1 => decPosSh(7),
      ADR2 => Q_n0103,
      ADR3 => decPosSh(6),
      O => Q_n0080(7)
    );
  Q_n0080_5_1 : X_LUT4
    generic map(
      INIT => X"60C0"
    )
    port map (
      ADR0 => cal_ctl_n0096_3_cyo,
      ADR1 => decPosSh(5),
      ADR2 => Q_n0103,
      ADR3 => decPosSh(4),
      O => Q_n0080(5)
    );
  state_FFd3_In1 : X_LUT4
    generic map(
      INIT => X"E2A2"
    )
    port map (
      ADR0 => N32,
      ADR1 => state_FFd1_137,
      ADR2 => state_FFd2_138,
      ADR3 => psDoneReg_126,
      O => N168
    );
  state_FFd3_In2 : X_LUT4
    generic map(
      INIT => X"D515"
    )
    port map (
      ADR0 => N32,
      ADR1 => state_FFd1_137,
      ADR2 => state_FFd2_138,
      ADR3 => psDoneReg_126,
      O => N169
    );
  state_FFd3_In_rn_0 : X_MUX2
    port map (
      IA => N169,
      IB => N168,
      SEL => state_FFd3_139,
      O => state_FFd3_In_MUXF5
    );
  state_FFd3_In3 : X_LUT3
    generic map(
      INIT => X"FB"
    )
    port map (
      ADR0 => state_FFd1_137,
      ADR1 => psDoneReg_126,
      ADR2 => state_FFd2_138,
      O => N170
    );
  state_FFd3_In_rn_1 : X_MUX2
    port map (
      IA => N1711,
      IB => N170,
      SEL => state_FFd3_139,
      O => state_FFd3_In_MUXF51
    );
  state_FFd3_In_rn_2 : X_MUX2
    port map (
      IA => state_FFd3_In_MUXF51,
      IB => state_FFd3_In_MUXF5,
      SEL => state_FFd4_140,
      O => state_FFd3_In
    );
  Q_n008825 : X_LUT4
    generic map(
      INIT => X"0F09"
    )
    port map (
      ADR0 => phSamp1,
      ADR1 => prevSamp_120,
      ADR2 => Q_n0051,
      ADR3 => NlwRenamedSig_OI_psEn,
      O => N194
    );
  Q_n0084_0_21 : X_LUT4
    generic map(
      INIT => X"0133"
    )
    port map (
      ADR0 => d1Shft(5),
      ADR1 => d1Shft(7),
      ADR2 => d1Shft(4),
      ADR3 => d1Shft(6),
      O => N277
    );
  Q_n0084_0_24 : X_LUT3
    generic map(
      INIT => X"A2"
    )
    port map (
      ADR0 => N277,
      ADR1 => N221,
      ADR2 => d2Shft(7),
      O => N280
    );
  Q_n0084_0_61 : X_LUT4
    generic map(
      INIT => X"FF54"
    )
    port map (
      ADR0 => Q_n0126,
      ADR1 => N280,
      ADR2 => N258,
      ADR3 => Q_n0125,
      O => Q_n0084(0)
    );
  Q_n0211_SW0 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => resetDcm_121,
      ADR1 => dcmlocked,
      O => N296
    );
  Q_n0211 : X_LUT4
    generic map(
      INIT => X"C040"
    )
    port map (
      ADR0 => Q_n0113,
      ADR1 => rstate_127,
      ADR2 => N296,
      ADR3 => Q_n0103,
      O => Q_n0211_135
    );
  lPtr_n00001 : X_LUT2
    generic map(
      INIT => X"E"
    )
    port map (
      ADR0 => Q_n0090,
      ADR1 => Q_n0091,
      O => lPtr_n0000
    );
  Q_n0081_SW0 : X_LUT3
    generic map(
      INIT => X"FB"
    )
    port map (
      ADR0 => selCnt(1),
      ADR1 => selCnt(2),
      ADR2 => reset,
      O => N299
    );
  Q_n0081 : X_LUT4
    generic map(
      INIT => X"EFFF"
    )
    port map (
      ADR0 => selCnt(0),
      ADR1 => N299,
      ADR2 => Q_n0122,
      ADR3 => selCnt(3),
      O => Q_n0081_130
    );
  Q_n009115 : X_LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      ADR0 => d1Shft(2),
      ADR1 => d1Shft(1),
      ADR2 => d1Shft(0),
      ADR3 => d1Shft(3),
      O => N306
    );
  Q_n009116 : X_LUT2
    generic map(
      INIT => X"8"
    )
    port map (
      ADR0 => d1Shft(5),
      ADR1 => d1Shft(4),
      O => N307
    );
  Q_n00902 : X_LUT3
    generic map(
      INIT => X"FE"
    )
    port map (
      ADR0 => reset,
      ADR1 => d0Shft(6),
      ADR2 => d0Shft(7),
      O => N316
    );
  N321 : X_LUT2
    generic map(
      INIT => X"9"
    )
    port map (
      ADR0 => prevSamp_120,
      ADR1 => hxSamp1,
      O => N32
    );
  state_Out171 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => state_FFd3_139,
      ADR1 => state_FFd2_138,
      ADR2 => state_FFd1_137,
      ADR3 => state_FFd4_140,
      O => Q_n0051
    );
  Ker3111 : X_LUT4
    generic map(
      INIT => X"20FF"
    )
    port map (
      ADR0 => state_FFd1_137,
      ADR1 => state_FFd3_139,
      ADR2 => state_FFd2_138,
      ADR3 => state_FFd4_140,
      O => N311
    );
  state_FFd2_In14 : X_LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      ADR0 => state_FFd3_139,
      ADR1 => state_FFd2_138,
      O => state_FFd2_In_map230
    );
  state_FFd2_In88 : X_LUT4
    generic map(
      INIT => X"A326"
    )
    port map (
      ADR0 => N32,
      ADR1 => state_FFd2_138,
      ADR2 => state_FFd1_137,
      ADR3 => state_FFd3_139,
      O => state_FFd2_In_map248
    );
  Q_n0076_SW0 : X_LUT4
    generic map(
      INIT => X"FF14"
    )
    port map (
      ADR0 => N311,
      ADR1 => hxSamp1,
      ADR2 => prevSamp_120,
      ADR3 => N35,
      O => N405
    );
  Q_n0076 : X_LUT4
    generic map(
      INIT => X"7362"
    )
    port map (
      ADR0 => rstate_127,
      ADR1 => resetDcm_121,
      ADR2 => psDoneReg_126,
      ADR3 => N405,
      O => Q_n0076_129
    );
  Q_n007010 : X_LUT3
    generic map(
      INIT => X"5D"
    )
    port map (
      ADR0 => prevSamp_120,
      ADR1 => state_FFd4_140,
      ADR2 => hxSamp1,
      O => N417
    );
  Q_n007016 : X_LUT3
    generic map(
      INIT => X"20"
    )
    port map (
      ADR0 => state_FFd2_138,
      ADR1 => state_FFd3_139,
      ADR2 => state_FFd1_137,
      O => N423
    );
  Q_n007028 : X_LUT2
    generic map(
      INIT => X"B"
    )
    port map (
      ADR0 => state_FFd3_139,
      ADR1 => phSamp1,
      O => N431
    );
  Q_n007037 : X_LUT4
    generic map(
      INIT => X"3332"
    )
    port map (
      ADR0 => state_FFd2_138,
      ADR1 => state_FFd4_140,
      ADR2 => N431,
      ADR3 => state_FFd1_137,
      O => N440
    );
  Q_n007057 : X_LUT4
    generic map(
      INIT => X"AAA8"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_psInc,
      ADR1 => N417,
      ADR2 => N423,
      ADR3 => N440,
      O => N447
    );
  Q_n007083 : X_LUT4
    generic map(
      INIT => X"111F"
    )
    port map (
      ADR0 => Q_n0051,
      ADR1 => phSamp1,
      ADR2 => hxSamp1,
      ADR3 => N311,
      O => N460
    );
  Ker381 : X_LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      ADR0 => wait5Cycle_125,
      ADR1 => wait4Cycle_124,
      ADR2 => waitTwoCycle_132,
      ADR3 => wait3Cycle_123,
      O => N38
    );
  Ker91 : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => resetDcm_121,
      ADR1 => dcmlocked,
      ADR2 => waitOneCycle_131,
      ADR3 => N38,
      O => N91
    );
  Q_n01961 : X_LUT4
    generic map(
      INIT => X"0010"
    )
    port map (
      ADR0 => state_FFd3_139,
      ADR1 => state_FFd1_137,
      ADR2 => N411,
      ADR3 => N131,
      O => Q_n0196
    );
  Q_n02031 : X_LUT4
    generic map(
      INIT => X"0020"
    )
    port map (
      ADR0 => state_FFd3_139,
      ADR1 => state_FFd1_137,
      ADR2 => N411,
      ADR3 => N131,
      O => Q_n0203
    );
  Q_n02011 : X_LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      ADR0 => N131,
      ADR1 => state_FFd3_139,
      ADR2 => N411,
      ADR3 => state_FFd1_137,
      O => Q_n0201
    );
  Q_n02091 : X_LUT4
    generic map(
      INIT => X"0040"
    )
    port map (
      ADR0 => state_FFd3_139,
      ADR1 => state_FFd2_138,
      ADR2 => N411,
      ADR3 => N161,
      O => Q_n0209
    );
  Q_n02161 : X_LUT4
    generic map(
      INIT => X"0080"
    )
    port map (
      ADR0 => state_FFd3_139,
      ADR1 => state_FFd2_138,
      ADR2 => N411,
      ADR3 => N161,
      O => Q_n0216
    );
  Q_n01951 : X_LUT4
    generic map(
      INIT => X"0040"
    )
    port map (
      ADR0 => state_FFd3_139,
      ADR1 => state_FFd1_137,
      ADR2 => N411,
      ADR3 => N131,
      O => Q_n0195
    );
  selCnt_Sclr_INV1 : X_LUT2
    generic map(
      INIT => X"1"
    )
    port map (
      ADR0 => Q_n0122,
      ADR1 => reset,
      O => selCnt_Sclr_INV
    );
  Q_n0122526 : X_LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      ADR0 => inTapForDqs(1),
      ADR1 => newTap(1),
      ADR2 => inTapForDqs(4),
      ADR3 => newTap(4),
      O => N517
    );
  Q_n0122562 : X_LUT4
    generic map(
      INIT => X"9009"
    )
    port map (
      ADR0 => inTapForDqs(2),
      ADR1 => newTap(2),
      ADR2 => inTapForDqs(3),
      ADR3 => newTap(3),
      O => N549
    );
  Q_n02071 : X_LUT4
    generic map(
      INIT => X"22F2"
    )
    port map (
      ADR0 => N40,
      ADR1 => NlwRenamedSig_OI_psInc,
      ADR2 => N101,
      ADR3 => Q_n0113,
      O => Q_n0207
    );
  Q_n02141 : X_LUT4
    generic map(
      INIT => X"88F8"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_psInc,
      ADR1 => N40,
      ADR2 => N101,
      ADR3 => Q_n0113,
      O => Q_n0214
    );
  Q_n011332 : X_LUT4
    generic map(
      INIT => X"6FF6"
    )
    port map (
      ADR0 => negPhShft(2),
      ADR1 => decNegSh(2),
      ADR2 => negPhShft(3),
      ADR3 => decNegSh(3),
      O => N589
    );
  Q_n011365 : X_LUT4
    generic map(
      INIT => X"6FF6"
    )
    port map (
      ADR0 => negPhShft(4),
      ADR1 => decNegSh(4),
      ADR2 => negPhShft(5),
      ADR3 => decNegSh(5),
      O => N616
    );
  Q_n0113113 : X_LUT4
    generic map(
      INIT => X"6FF6"
    )
    port map (
      ADR0 => negPhShft(6),
      ADR1 => decNegSh(6),
      ADR2 => negPhShft(7),
      ADR3 => decNegSh(7),
      O => N646
    );
  Q_n0113146 : X_LUT4
    generic map(
      INIT => X"6FF6"
    )
    port map (
      ADR0 => negPhShft(0),
      ADR1 => decNegSh(0),
      ADR2 => negPhShft(1),
      ADR3 => decNegSh(1),
      O => N673
    );
  Q_n0113164 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => N589,
      ADR1 => N616,
      ADR2 => N646,
      ADR3 => N673,
      O => Q_n0113
    );
  Q_n00601 : X_LUT3
    generic map(
      INIT => X"BA"
    )
    port map (
      ADR0 => reset,
      ADR1 => Q_n0113,
      ADR2 => N101,
      O => Q_n0060
    );
  Ker101 : X_LUT4
    generic map(
      INIT => X"0800"
    )
    port map (
      ADR0 => dcmlocked,
      ADR1 => resetDcm_121,
      ADR2 => Q_n0103,
      ADR3 => rstate_127,
      O => N101
    );
  Q_n010332 : X_LUT4
    generic map(
      INIT => X"7DBE"
    )
    port map (
      ADR0 => decPosSh(3),
      ADR1 => decPosSh(2),
      ADR2 => posPhShft(2),
      ADR3 => posPhShft(3),
      O => N708
    );
  Q_n010365 : X_LUT4
    generic map(
      INIT => X"7DBE"
    )
    port map (
      ADR0 => decPosSh(5),
      ADR1 => decPosSh(4),
      ADR2 => posPhShft(4),
      ADR3 => posPhShft(5),
      O => N735
    );
  Q_n0103113 : X_LUT4
    generic map(
      INIT => X"7DBE"
    )
    port map (
      ADR0 => decPosSh(7),
      ADR1 => decPosSh(6),
      ADR2 => posPhShft(6),
      ADR3 => posPhShft(7),
      O => N765
    );
  Q_n0103146 : X_LUT4
    generic map(
      INIT => X"7DBE"
    )
    port map (
      ADR0 => decPosSh(1),
      ADR1 => decPosSh(0),
      ADR2 => posPhShft(0),
      ADR3 => posPhShft(1),
      O => N792
    );
  Q_n0103164 : X_LUT4
    generic map(
      INIT => X"FFFE"
    )
    port map (
      ADR0 => N708,
      ADR1 => N735,
      ADR2 => N765,
      ADR3 => N792,
      O => Q_n0103
    );
  decNegSh_4_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => decNegSh(4),
      O => decNegSh_4_rt_141,
      ADR1 => GND
    );
  Q_n0122578 : X_LUT4
    generic map(
      INIT => X"9000"
    )
    port map (
      ADR0 => inTapForDqs(0),
      ADR1 => newTap(0),
      ADR2 => N517,
      ADR3 => N549,
      O => Q_n0122
    );
  Q_n0091113 : X_LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      ADR0 => d1Shft(6),
      ADR1 => N306,
      ADR2 => N307,
      ADR3 => d1Shft(7),
      O => Q_n0091
    );
  Q_n008854 : X_LUT4
    generic map(
      INIT => X"FE54"
    )
    port map (
      ADR0 => resetDcm_121,
      ADR1 => N194,
      ADR2 => N177,
      ADR3 => rstate_127,
      O => Q_n0088
    );
  Q_n0132113 : X_LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      ADR0 => d3Shft(7),
      ADR1 => N58,
      ADR2 => N591,
      ADR3 => d3Shft(6),
      O => Q_n0132
    );
  Q_n0131113 : X_LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      ADR0 => d2Shft(7),
      ADR1 => N71,
      ADR2 => N72,
      ADR3 => d2Shft(6),
      O => Q_n0131
    );
  Q_n009015_SW0 : X_LUT4
    generic map(
      INIT => X"FFEA"
    )
    port map (
      ADR0 => d0Shft(3),
      ADR1 => d0Shft(1),
      ADR2 => d0Shft(0),
      ADR3 => d0Shft(2),
      O => N805
    );
  Q_n009015 : X_LUT4
    generic map(
      INIT => X"FF80"
    )
    port map (
      ADR0 => d0Shft(5),
      ADR1 => d0Shft(4),
      ADR2 => N805,
      ADR3 => N316,
      O => Q_n0090
    );
  Q_n0092_0_20_SW0 : X_LUT3
    generic map(
      INIT => X"F8"
    )
    port map (
      ADR0 => d4Shft(1),
      ADR1 => d4Shft(0),
      ADR2 => d4Shft(2),
      O => N807
    );
  Q_n0092_0_20 : X_LUT4
    generic map(
      INIT => X"57FF"
    )
    port map (
      ADR0 => d4Shft(5),
      ADR1 => N807,
      ADR2 => d4Shft(3),
      ADR3 => d4Shft(4),
      O => N134
    );
  state_FFd3_In4 : X_LUT4
    generic map(
      INIT => X"0401"
    )
    port map (
      ADR0 => state_FFd1_137,
      ADR1 => phSamp1,
      ADR2 => state_FFd2_138,
      ADR3 => prevSamp_120,
      O => N1711
    );
  Q_n0084_0_2 : X_LUT4
    generic map(
      INIT => X"0133"
    )
    port map (
      ADR0 => d3Shft(5),
      ADR1 => d3Shft(7),
      ADR2 => d3Shft(4),
      ADR3 => d3Shft(6),
      O => N258
    );
  Ker4111 : X_LUT3
    generic map(
      INIT => X"60"
    )
    port map (
      ADR0 => prevSamp_120,
      ADR1 => hxSamp1,
      ADR2 => N91,
      O => N411
    );
  Ker351 : X_LUT3
    generic map(
      INIT => X"06"
    )
    port map (
      ADR0 => prevSamp_120,
      ADR1 => phSamp1,
      ADR2 => Q_n0051,
      O => N35
    );
  Q_n00885 : X_LUT4
    generic map(
      INIT => X"0F09"
    )
    port map (
      ADR0 => hxSamp1,
      ADR1 => prevSamp_120,
      ADR2 => N311,
      ADR3 => NlwRenamedSig_OI_psEn,
      O => N177
    );
  Q_n02061 : X_LUT4
    generic map(
      INIT => X"0440"
    )
    port map (
      ADR0 => Q_n0051,
      ADR1 => N91,
      ADR2 => prevSamp_120,
      ADR3 => phSamp1,
      O => Q_n0206
    );
  Q_n0084_1_SW2 : X_LUT4
    generic map(
      INIT => X"FAF8"
    )
    port map (
      ADR0 => d2Shft(6),
      ADR1 => d2Shft(5),
      ADR2 => d2Shft(7),
      ADR3 => d2Shft(4),
      O => N813
    );
  Q_n0084_1_Q : X_LUT4
    generic map(
      INIT => X"4447"
    )
    port map (
      ADR0 => N24,
      ADR1 => N813,
      ADR2 => Q_n0126,
      ADR3 => Q_n0125,
      O => Q_n0084(1)
    );
  Q_n0085_3_SW1 : X_LUT4
    generic map(
      INIT => X"9555"
    )
    port map (
      ADR0 => posPhShft(3),
      ADR1 => posPhShft(2),
      ADR2 => posPhShft(1),
      ADR3 => posPhShft(0),
      O => N815
    );
  Q_n0085_3_Q : X_LUT4
    generic map(
      INIT => X"4000"
    )
    port map (
      ADR0 => N815,
      ADR1 => rstate_127,
      ADR2 => NlwRenamedSig_OI_psInc,
      ADR3 => psDoneReg_126,
      O => Q_n0085(3)
    );
  Q_n0074_3_SW1 : X_LUT4
    generic map(
      INIT => X"9555"
    )
    port map (
      ADR0 => negPhShft(3),
      ADR1 => negPhShft(0),
      ADR2 => negPhShft(2),
      ADR3 => negPhShft(1),
      O => N817
    );
  Q_n0074_3_Q : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_psInc,
      ADR1 => psDoneReg_126,
      ADR2 => N817,
      ADR3 => rstate_127,
      O => Q_n0074(3)
    );
  Q_n0058_2_SW1 : X_LUT4
    generic map(
      INIT => X"AEA6"
    )
    port map (
      ADR0 => state_FFd2_138,
      ADR1 => state_FFd1_137,
      ADR2 => state_FFd3_139,
      ADR3 => NlwRenamedSig_OI_selTap(2),
      O => N819
    );
  Q_n0058_2_Q : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => state_FFd4_140,
      ADR1 => NlwRenamedSig_OI_selTap(2),
      ADR2 => N32,
      ADR3 => N819,
      O => Q_n0058(2)
    );
  Q_n0058_3_66_SW0 : X_LUT4
    generic map(
      INIT => X"F6B6"
    )
    port map (
      ADR0 => state_FFd3_139,
      ADR1 => state_FFd1_137,
      ADR2 => state_FFd2_138,
      ADR3 => NlwRenamedSig_OI_selTap(3),
      O => N821
    );
  Q_n0058_3_66 : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => state_FFd4_140,
      ADR1 => NlwRenamedSig_OI_selTap(3),
      ADR2 => N32,
      ADR3 => N821,
      O => Q_n0058(3)
    );
  Q_n0058_4_SW0 : X_LUT4
    generic map(
      INIT => X"DDF7"
    )
    port map (
      ADR0 => state_FFd1_137,
      ADR1 => state_FFd2_138,
      ADR2 => NlwRenamedSig_OI_selTap(4),
      ADR3 => state_FFd3_139,
      O => N823
    );
  Q_n0058_4_Q : X_LUT4
    generic map(
      INIT => X"CEC4"
    )
    port map (
      ADR0 => state_FFd4_140,
      ADR1 => NlwRenamedSig_OI_selTap(4),
      ADR2 => N32,
      ADR3 => N823,
      O => Q_n0058(4)
    );
  Q_n0067 : X_MUX2
    port map (
      IA => N826,
      IB => N827,
      SEL => Q_n0051,
      O => Q_n0067_128
    );
  Q_n0067_F : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => wait5Cycle_125,
      ADR1 => prevSamp_120,
      ADR2 => phSamp1,
      O => N826
    );
  Q_n0067_G : X_LUT3
    generic map(
      INIT => X"E4"
    )
    port map (
      ADR0 => wait5Cycle_125,
      ADR1 => prevSamp_120,
      ADR2 => hxSamp1,
      O => N827
    );
  Q_n0205 : X_MUX2
    port map (
      IA => N828,
      IB => N829,
      SEL => resetDcm_121,
      O => Q_n0205_134
    );
  Q_n0205_F : X_LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      ADR0 => dcmlocked,
      ADR1 => waitTwoCycle_132,
      ADR2 => N156,
      O => N828
    );
  Q_n0205_G : X_LUT4
    generic map(
      INIT => X"0400"
    )
    port map (
      ADR0 => Q_n0113,
      ADR1 => rstate_127,
      ADR2 => Q_n0103,
      ADR3 => dcmlocked,
      O => N829
    );
  Q_n0070131 : X_MUX2
    port map (
      IA => N830,
      IB => N831,
      SEL => resetDcm_121,
      O => Q_n0070
    );
  Q_n0070131_F : X_LUT3
    generic map(
      INIT => X"F4"
    )
    port map (
      ADR0 => prevSamp_120,
      ADR1 => N460,
      ADR2 => N447,
      O => N830
    );
  Q_n0070131_G : X_LUT4
    generic map(
      INIT => X"0001"
    )
    port map (
      ADR0 => N708,
      ADR1 => N735,
      ADR2 => N765,
      ADR3 => N792,
      O => N831
    );
  Q_n0208 : X_MUX2
    port map (
      IA => N832,
      IB => N833,
      SEL => resetDcm_121,
      O => Q_n0208_136
    );
  Q_n0208_F : X_LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      ADR0 => dcmlocked,
      ADR1 => waitTwoCycle_132,
      ADR2 => N156,
      O => N832
    );
  Q_n0208_G : X_LUT4
    generic map(
      INIT => X"AA8A"
    )
    port map (
      ADR0 => dcmlocked,
      ADR1 => Q_n0113,
      ADR2 => rstate_127,
      ADR3 => Q_n0103,
      O => N833
    );
  Q_n0204 : X_MUX2
    port map (
      IA => N834,
      IB => N835,
      SEL => resetDcm_121,
      O => Q_n0204_133
    );
  Q_n0204_F : X_LUT3
    generic map(
      INIT => X"02"
    )
    port map (
      ADR0 => dcmlocked,
      ADR1 => waitTwoCycle_132,
      ADR2 => N156,
      O => N834
    );
  Q_n0204_G : X_LUT4
    generic map(
      INIT => X"C080"
    )
    port map (
      ADR0 => Q_n0103,
      ADR1 => rstate_127,
      ADR2 => dcmlocked,
      ADR3 => Q_n0113,
      O => N835
    );
  state_FFd2_In105 : X_MUX2
    port map (
      IA => N836,
      IB => N837,
      SEL => psDoneReg_126,
      O => state_FFd2_In
    );
  state_FFd2_In105_F : X_LUT4
    generic map(
      INIT => X"FA8A"
    )
    port map (
      ADR0 => state_FFd2_138,
      ADR1 => state_FFd1_137,
      ADR2 => state_FFd4_140,
      ADR3 => state_FFd2_In_map248,
      O => N836
    );
  state_FFd2_In105_G : X_LUT4
    generic map(
      INIT => X"EA40"
    )
    port map (
      ADR0 => state_FFd4_140,
      ADR1 => state_FFd1_137,
      ADR2 => state_FFd2_In_map230,
      ADR3 => state_FFd2_In_map248,
      O => N837
    );
  state_FFd1_In_rn_0111 : X_MUX2
    port map (
      IA => N838,
      IB => N839,
      SEL => state_FFd4_140,
      O => state_FFd1_In
    );
  state_FFd1_In_rn_0111_F : X_LUT3
    generic map(
      INIT => X"A2"
    )
    port map (
      ADR0 => state_FFd1_137,
      ADR1 => psDoneReg_126,
      ADR2 => state_FFd2_138,
      O => N838
    );
  state_FFd1_In_rn_0111_G : X_LUT4
    generic map(
      INIT => X"F8EA"
    )
    port map (
      ADR0 => state_FFd1_137,
      ADR1 => N32,
      ADR2 => state_FFd2_138,
      ADR3 => state_FFd3_139,
      O => N839
    );
  NlwBlock_cal_ctl0_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlock_cal_ctl0_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal VCC : STD_LOGIC; 
  signal GND : STD_LOGIC; 
begin
  iReg <= NlwRenamedSig_OI_iReg;
  ireg_FD : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dInp,
      CLK => clk,
      O => NlwRenamedSig_OI_iReg,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  dreg_FD : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => NlwRenamedSig_OI_iReg,
      CLK => clk,
      O => dReg,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  NlwBlock_hxSampReg0_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlock_hxSampReg0_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal VCC : STD_LOGIC; 
  signal GND : STD_LOGIC; 
begin
  iReg <= NlwRenamedSig_OI_iReg;
  ireg_FD : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => dInp,
      CLK => clk,
      O => NlwRenamedSig_OI_iReg,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  dreg_FD : X_FF
    generic map(
      INIT => '0'
    )
    port map (
      I => NlwRenamedSig_OI_iReg,
      CLK => clk,
      O => dReg,
      CE => VCC,
      SET => GND,
      RST => GND
    );
  NlwBlock_phSampReg0_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlock_phSampReg0_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal fpga_rst_142 : STD_LOGIC; 
  signal suClkDiv2 : STD_LOGIC; 
  signal hxSamp1 : STD_LOGIC; 
  signal dcmlocked : STD_LOGIC; 
  signal suPhClkDiv2 : STD_LOGIC; 
  signal hexClk : STD_LOGIC; 
  signal phShftClkDcm : STD_LOGIC; 
  signal Q_n0011 : STD_LOGIC; 
  signal divRst : STD_LOGIC; 
  signal gnd : STD_LOGIC; 
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
  signal cal_clkd2_poclk_143 : STD_LOGIC; 
  signal cal_phClkd2_n0003 : STD_LOGIC; 
  signal cal_phClkd2_poclk_144 : STD_LOGIC; 
  signal ckt_to_cal_delay5 : STD_LOGIC; 
  signal ckt_to_cal_delay4 : STD_LOGIC; 
  signal ckt_to_cal_delay3 : STD_LOGIC; 
  signal ckt_to_cal_delay2 : STD_LOGIC; 
  signal ckt_to_cal_delay1 : STD_LOGIC; 
  signal ckt_to_cal_high : STD_LOGIC; 
  signal NLW_cal_ctl0_locReset_UNCONNECTED : STD_LOGIC; 
  signal NLW_hxSampReg0_iReg_UNCONNECTED : STD_LOGIC; 
  signal NLW_phSampReg0_iReg_UNCONNECTED : STD_LOGIC; 
  signal VCC : STD_LOGIC; 
  signal GND_145 : STD_LOGIC; 
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
  signal NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_cal_top0_cal_phClkd2_oclk_C : STD_LOGIC; 
  signal NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_cal_top0_cal_phClkd2_poclk_C : STD_LOGIC; 
  signal selTap : STD_LOGIC_VECTOR ( 4 downto 0 ); 
begin
  XST_GND : X_ZERO
    port map (
      O => gnd
    );
  XST_VCC : X_ONE
    port map (
      O => N1
    );
  fpga_rst : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => N1,
      SRST => Q_n0011,
      CLK => clk0,
      O => fpga_rst_142,
      CE => VCC,
      SET => GND_145,
      RST => GND_145,
      SSET => GND_145
    );
  cal_dcm : X_DCM
    generic map(
      CLKOUT_PHASE_SHIFT => "VARIABLE",
      DLL_FREQUENCY_MODE => "LOW",
      DUTY_CYCLE_CORRECTION => TRUE,
      PHASE_SHIFT => 128,
      LOC => "DCM_X2Y0"
    )
    port map (
      RST => reset_not,
      CLKIN => clk,
      CLKFB => phShftClk,
      PSINCDEC => psInc,
      PSEN => psEn,
      PSCLK => clk0,
      DSSEN => gnd,
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
  phclk_bufg : X_CKBUF
    port map (
      I => phShftClkDcm,
      O => phShftClk
    );
  phSampReg0 : cal_reg
    port map (
      clk => suPhClkDiv2,
      reset => fpga_rst_142,
      dInp => suClkDiv2,
      iReg => NLW_phSampReg0_iReg_UNCONNECTED,
      dReg => phSamp1
    );
  hxSampReg0 : cal_reg_INST_1
    port map (
      clk => hexClk,
      reset => fpga_rst_142,
      dInp => clkDiv2,
      iReg => NLW_hxSampReg0_iReg_UNCONNECTED,
      dReg => hxSamp1
    );
  reset_not1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => reset,
      O => reset_not,
      ADR1 => GND_145
    );
  divRst1 : X_LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      ADR0 => clk0dcmlock,
      ADR1 => dcmlocked,
      O => divRst
    );
  Q_n00111 : X_LUT3
    generic map(
      INIT => X"80"
    )
    port map (
      ADR0 => clk0dcmlock,
      ADR1 => dcmlocked,
      ADR2 => reset,
      O => Q_n0011
    );
  cal_ctl0 : cal_ctl
    port map (
      clk => clk0,
      hxSamp1 => hxSamp1,
      dcmlocked => dcmlocked,
      reset => fpga_rst_142,
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
  cal_clkd2_n00031 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => cal_clkd2_poclk_143,
      O => cal_clkd2_n0003,
      ADR1 => GND_145
    );
  cal_clkd2_oclk : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => cal_clkd2_poclk_143,
      SRST => divRst,
      CLK => clk0,
      O => clkDiv2,
      CE => VCC,
      SET => GND_145,
      RST => GND_145,
      SSET => GND_145
    );
  cal_clkd2_poclk : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => cal_clkd2_n0003,
      SRST => divRst,
      CLK => clk0,
      O => cal_clkd2_poclk_143,
      CE => VCC,
      SET => GND_145,
      RST => GND_145,
      SSET => GND_145
    );
  cal_phClkd2_n00031 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => cal_phClkd2_poclk_144,
      O => cal_phClkd2_n0003,
      ADR1 => GND_145
    );
  cal_phClkd2_oclk : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => cal_phClkd2_poclk_144,
      SRST => divRst,
      CLK => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_cal_top0_cal_phClkd2_oclk_C,
      O => phClkDiv2,
      CE => VCC,
      SET => GND_145,
      RST => GND_145,
      SSET => GND_145
    );
  cal_phClkd2_poclk : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => cal_phClkd2_n0003,
      SRST => divRst,
      CLK => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_cal_top0_cal_phClkd2_poclk_C,
      O => cal_phClkd2_poclk_144,
      CE => VCC,
      SET => GND_145,
      RST => GND_145,
      SSET => GND_145
    );
  ckt_to_cal_six : X_LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      ADR0 => phClkDiv2,
      ADR1 => selTap(1),
      ADR2 => ckt_to_cal_delay2,
      ADR3 => ckt_to_cal_high,
      O => ckt_to_cal_delay3
    );
  ckt_to_cal_five : X_LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      ADR0 => ckt_to_cal_high,
      ADR1 => selTap(3),
      ADR2 => ckt_to_cal_delay4,
      ADR3 => phClkDiv2,
      O => ckt_to_cal_delay5
    );
  ckt_to_cal_four : X_LUT4
    generic map(
      INIT => X"FF00"
    )
    port map (
      ADR0 => ckt_to_cal_high,
      ADR1 => ckt_to_cal_high,
      ADR2 => ckt_to_cal_high,
      ADR3 => phClkDiv2,
      O => ckt_to_cal_delay1
    );
  ckt_to_cal_three : X_LUT4
    generic map(
      INIT => X"E2E2"
    )
    port map (
      ADR0 => phClkDiv2,
      ADR1 => selTap(0),
      ADR2 => ckt_to_cal_delay1,
      ADR3 => ckt_to_cal_high,
      O => ckt_to_cal_delay2
    );
  ckt_to_cal_two : X_LUT4
    generic map(
      INIT => X"EE22"
    )
    port map (
      ADR0 => phClkDiv2,
      ADR1 => selTap(2),
      ADR2 => ckt_to_cal_high,
      ADR3 => ckt_to_cal_delay3,
      O => ckt_to_cal_delay4
    );
  ckt_to_cal_one : X_LUT4
    generic map(
      INIT => X"F3C0"
    )
    port map (
      ADR0 => ckt_to_cal_high,
      ADR1 => selTap(4),
      ADR2 => ckt_to_cal_delay5,
      ADR3 => phClkDiv2,
      O => hexClk
    );
  ckt_to_cal_XST_VCC : X_ONE
    port map (
      O => ckt_to_cal_high
    );
  NlwBlock_cal_top0_VCC : X_ONE
    port map (
      O => VCC
    );
  NlwBlock_cal_top0_GND : X_ZERO
    port map (
      O => GND_145
    );
  NlwInverterBlock_mem_interface_top_inst_infrastructure_top0_cal_top0_cal_phClkd2_oclk_C : X_INV
    port map (
      I => phShftClk,
      O => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_cal_top0_cal_phClkd2_oclk_C
    );
  NlwInverterBlock_mem_interface_top_inst_infrastructure_top0_cal_top0_cal_phClkd2_poclk_C : X_INV
    port map (
      I => phShftClk,
      O => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_cal_top0_cal_phClkd2_poclk_C
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal sys_rst270_1_146 : STD_LOGIC; 
  signal wait_clk90_147 : STD_LOGIC; 
  signal dcm_lock : STD_LOGIC; 
  signal sys_rst_1_148 : STD_LOGIC; 
  signal vcc : STD_LOGIC; 
  signal sys_rst270_o_149 : STD_LOGIC; 
  signal sys_rst90_1_150 : STD_LOGIC; 
  signal Q_n0013 : STD_LOGIC; 
  signal Q_n0020 : STD_LOGIC; 
  signal Q_n0015 : STD_LOGIC; 
  signal Q_n0021 : STD_LOGIC; 
  signal Q_n0016 : STD_LOGIC; 
  signal Q_n0022 : STD_LOGIC; 
  signal Q_n0017 : STD_LOGIC; 
  signal sys_rst_o_151 : STD_LOGIC; 
  signal sys_rst90_o_152 : STD_LOGIC; 
  signal user_rst : STD_LOGIC; 
  signal sys_rst180_1_153 : STD_LOGIC; 
  signal sys_rst180_o_154 : STD_LOGIC; 
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
  signal Counter200_1_rt_155 : STD_LOGIC; 
  signal Counter200_2_rt_156 : STD_LOGIC; 
  signal Counter200_3_rt_157 : STD_LOGIC; 
  signal Counter200_4_rt_158 : STD_LOGIC; 
  signal Counter200_5_rt_159 : STD_LOGIC; 
  signal Counter200_6_rt_160 : STD_LOGIC; 
  signal Counter200_7_rt_161 : STD_LOGIC; 
  signal Counter200_8_rt_162 : STD_LOGIC; 
  signal Counter200_9_rt_163 : STD_LOGIC; 
  signal Counter200_10_rt_164 : STD_LOGIC; 
  signal Counter200_11_rt_165 : STD_LOGIC; 
  signal Counter200_12_rt_166 : STD_LOGIC; 
  signal Counter200_13_rt_167 : STD_LOGIC; 
  signal Counter200_14_rt_168 : STD_LOGIC; 
  signal Counter200_15_rt_169 : STD_LOGIC; 
  signal clk_dcm0_clk90dcm : STD_LOGIC; 
  signal clk_dcm0_clk0fb : STD_LOGIC; 
  signal clk_dcm0_clk90d2inv : STD_LOGIC; 
  signal clk_dcm0_clk0dcm : STD_LOGIC; 
  signal clk_dcm0_clk0_o : STD_LOGIC; 
  signal clk_dcm0_gnd : STD_LOGIC; 
  signal clk_dcm0_clk_half_buf : STD_LOGIC; 
  signal clk_dcm0_rst_delay5_170 : STD_LOGIC; 
  signal clk_dcm0_rst_delay4_171 : STD_LOGIC; 
  signal clk_dcm0_rst_delay3_172 : STD_LOGIC; 
  signal clk_dcm0_rst_delay2_173 : STD_LOGIC; 
  signal clk_dcm0_rst_delay1_174 : STD_LOGIC; 
  signal clk_dcm0_clk0d2inv : STD_LOGIC; 
  signal clk_dcm0_clk_half : STD_LOGIC; 
  signal clk_dcm0_clk0fb_buf : STD_LOGIC; 
  signal clk_dcm0_clk0_o_buf : STD_LOGIC; 
  signal VCC_175 : STD_LOGIC; 
  signal GND : STD_LOGIC; 
  signal NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst180_o_C : STD_LOGIC; 
  signal NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst270_o_C : STD_LOGIC; 
  signal NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst270_1_C : STD_LOGIC; 
  signal NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst180_1_C : STD_LOGIC; 
  signal NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst180_C : STD_LOGIC; 
  signal NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst270_C : STD_LOGIC; 
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
  XST_VCC : X_ONE
    port map (
      O => vcc
    );
  XST_GND : X_ZERO
    port map (
      O => N1
    );
  sys_rst90_o : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => vcc,
      SRST => Q_n0020,
      CLK => NlwRenamedSig_OI_clk90_int_val,
      O => sys_rst90_o_152,
      CE => VCC_175,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  sys_rst180_o : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => vcc,
      SRST => Q_n0021,
      CLK => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst180_o_C,
      O => sys_rst180_o_154,
      CE => VCC_175,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  wait_200us_i : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => N1,
      SSET => Q_n0013,
      CE => Q_n0015,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => NlwRenamedSig_OI_wait_200us,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  sys_rst270_o : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => vcc,
      SRST => Q_n0020,
      CLK => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst270_o_C,
      O => sys_rst270_o_149,
      CE => VCC_175,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  sys_rst_o : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => vcc,
      SRST => Q_n0021,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => sys_rst_o_151,
      CE => VCC_175,
      SET => GND,
      RST => GND,
      SSET => GND
    );
  sys_rst270_1 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => sys_rst270_o_149,
      SSET => Q_n0016,
      CLK => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst270_1_C,
      O => sys_rst270_1_146,
      CE => VCC_175,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  sys_rst_1 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => sys_rst_o_151,
      SSET => Q_n0017,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => sys_rst_1_148,
      CE => VCC_175,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  wait_clk90 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => NlwRenamedSig_OI_wait_200us,
      SSET => Q_n0013,
      CLK => NlwRenamedSig_OI_clk90_int_val,
      O => wait_clk90_147,
      CE => VCC_175,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  sys_rst90_1 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => sys_rst90_o_152,
      SSET => Q_n0016,
      CLK => NlwRenamedSig_OI_clk90_int_val,
      O => sys_rst90_1_150,
      CE => VCC_175,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  sys_rst180_1 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => sys_rst180_o_154,
      SSET => Q_n0017,
      CLK => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst180_1_C,
      O => sys_rst180_1_153,
      CE => VCC_175,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  sys_rst180 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => sys_rst180_1_153,
      SSET => Q_n0017,
      CLK => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst180_C,
      O => sys_rst180_val,
      CE => VCC_175,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  sys_rst270 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => sys_rst270_1_146,
      SSET => Q_n0016,
      CLK => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst270_C,
      O => sys_rst270_val,
      CE => VCC_175,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  sys_rst90 : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => sys_rst90_1_150,
      SSET => Q_n0016,
      CLK => NlwRenamedSig_OI_clk90_int_val,
      O => sys_rst90_val,
      CE => VCC_175,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  sys_rst : X_SFF
    generic map(
      INIT => '1'
    )
    port map (
      I => sys_rst_1_148,
      SSET => Q_n0017,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => sys_rst_val,
      CE => VCC_175,
      SET => GND,
      RST => GND,
      SRST => GND
    );
  Counter200_0 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(0),
      SRST => Q_n0013,
      CE => Q_n0022,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => Counter200(0),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Counter200_1 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(1),
      SRST => Q_n0013,
      CE => Q_n0022,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => Counter200(1),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Counter200_2 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(2),
      SRST => Q_n0013,
      CE => Q_n0022,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => Counter200(2),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Counter200_3 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(3),
      SRST => Q_n0013,
      CE => Q_n0022,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => Counter200(3),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Counter200_4 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(4),
      SRST => Q_n0013,
      CE => Q_n0022,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => Counter200(4),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Counter200_5 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(5),
      SRST => Q_n0013,
      CE => Q_n0022,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => Counter200(5),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Counter200_6 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(6),
      SRST => Q_n0013,
      CE => Q_n0022,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => Counter200(6),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Counter200_7 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(7),
      SRST => Q_n0013,
      CE => Q_n0022,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => Counter200(7),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Counter200_8 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(8),
      SRST => Q_n0013,
      CE => Q_n0022,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => Counter200(8),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Counter200_9 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(9),
      SRST => Q_n0013,
      CE => Q_n0022,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => Counter200(9),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Counter200_10 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(10),
      SRST => Q_n0013,
      CE => Q_n0022,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => Counter200(10),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Counter200_11 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(11),
      SRST => Q_n0013,
      CE => Q_n0022,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => Counter200(11),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Counter200_12 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(12),
      SRST => Q_n0013,
      CE => Q_n0022,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => Counter200(12),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Counter200_13 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(13),
      SRST => Q_n0013,
      CE => Q_n0022,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => Counter200(13),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Counter200_14 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(14),
      SRST => Q_n0013,
      CE => Q_n0022,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => Counter200(14),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  Counter200_15 : X_SFF
    generic map(
      INIT => '0'
    )
    port map (
      I => Result(15),
      SRST => Q_n0013,
      CE => Q_n0022,
      CLK => NlwRenamedSig_OI_clk_int_val,
      O => Counter200(15),
      SET => GND,
      RST => GND,
      SSET => GND
    );
  infrastructure_top_Result_0_lut : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => Counter200(0),
      O => Result(0),
      ADR1 => GND
    );
  infrastructure_top_Result_0_cy : X_MUX2
    port map (
      IB => N1,
      IA => vcc,
      SEL => Result(0),
      O => infrastructure_top_Result_0_cyo
    );
  infrastructure_top_Result_1_cy : X_MUX2
    port map (
      IB => infrastructure_top_Result_0_cyo,
      IA => N1,
      SEL => Counter200_1_rt_155,
      O => infrastructure_top_Result_1_cyo
    );
  infrastructure_top_Result_1_xor : X_XOR2
    port map (
      I0 => infrastructure_top_Result_0_cyo,
      I1 => Counter200_1_rt_155,
      O => Result(1)
    );
  infrastructure_top_Result_2_cy : X_MUX2
    port map (
      IB => infrastructure_top_Result_1_cyo,
      IA => N1,
      SEL => Counter200_2_rt_156,
      O => infrastructure_top_Result_2_cyo
    );
  infrastructure_top_Result_2_xor : X_XOR2
    port map (
      I0 => infrastructure_top_Result_1_cyo,
      I1 => Counter200_2_rt_156,
      O => Result(2)
    );
  infrastructure_top_Result_3_cy : X_MUX2
    port map (
      IB => infrastructure_top_Result_2_cyo,
      IA => N1,
      SEL => Counter200_3_rt_157,
      O => infrastructure_top_Result_3_cyo
    );
  infrastructure_top_Result_3_xor : X_XOR2
    port map (
      I0 => infrastructure_top_Result_2_cyo,
      I1 => Counter200_3_rt_157,
      O => Result(3)
    );
  infrastructure_top_Result_4_cy : X_MUX2
    port map (
      IB => infrastructure_top_Result_3_cyo,
      IA => N1,
      SEL => Counter200_4_rt_158,
      O => infrastructure_top_Result_4_cyo
    );
  infrastructure_top_Result_4_xor : X_XOR2
    port map (
      I0 => infrastructure_top_Result_3_cyo,
      I1 => Counter200_4_rt_158,
      O => Result(4)
    );
  infrastructure_top_Result_5_cy : X_MUX2
    port map (
      IB => infrastructure_top_Result_4_cyo,
      IA => N1,
      SEL => Counter200_5_rt_159,
      O => infrastructure_top_Result_5_cyo
    );
  infrastructure_top_Result_5_xor : X_XOR2
    port map (
      I0 => infrastructure_top_Result_4_cyo,
      I1 => Counter200_5_rt_159,
      O => Result(5)
    );
  infrastructure_top_Result_6_cy : X_MUX2
    port map (
      IB => infrastructure_top_Result_5_cyo,
      IA => N1,
      SEL => Counter200_6_rt_160,
      O => infrastructure_top_Result_6_cyo
    );
  infrastructure_top_Result_6_xor : X_XOR2
    port map (
      I0 => infrastructure_top_Result_5_cyo,
      I1 => Counter200_6_rt_160,
      O => Result(6)
    );
  infrastructure_top_Result_7_cy : X_MUX2
    port map (
      IB => infrastructure_top_Result_6_cyo,
      IA => N1,
      SEL => Counter200_7_rt_161,
      O => infrastructure_top_Result_7_cyo
    );
  infrastructure_top_Result_7_xor : X_XOR2
    port map (
      I0 => infrastructure_top_Result_6_cyo,
      I1 => Counter200_7_rt_161,
      O => Result(7)
    );
  infrastructure_top_Result_8_cy : X_MUX2
    port map (
      IB => infrastructure_top_Result_7_cyo,
      IA => N1,
      SEL => Counter200_8_rt_162,
      O => infrastructure_top_Result_8_cyo
    );
  infrastructure_top_Result_8_xor : X_XOR2
    port map (
      I0 => infrastructure_top_Result_7_cyo,
      I1 => Counter200_8_rt_162,
      O => Result(8)
    );
  infrastructure_top_Result_9_cy : X_MUX2
    port map (
      IB => infrastructure_top_Result_8_cyo,
      IA => N1,
      SEL => Counter200_9_rt_163,
      O => infrastructure_top_Result_9_cyo
    );
  infrastructure_top_Result_9_xor : X_XOR2
    port map (
      I0 => infrastructure_top_Result_8_cyo,
      I1 => Counter200_9_rt_163,
      O => Result(9)
    );
  infrastructure_top_Result_10_cy : X_MUX2
    port map (
      IB => infrastructure_top_Result_9_cyo,
      IA => N1,
      SEL => Counter200_10_rt_164,
      O => infrastructure_top_Result_10_cyo
    );
  infrastructure_top_Result_10_xor : X_XOR2
    port map (
      I0 => infrastructure_top_Result_9_cyo,
      I1 => Counter200_10_rt_164,
      O => Result(10)
    );
  infrastructure_top_Result_11_cy : X_MUX2
    port map (
      IB => infrastructure_top_Result_10_cyo,
      IA => N1,
      SEL => Counter200_11_rt_165,
      O => infrastructure_top_Result_11_cyo
    );
  infrastructure_top_Result_11_xor : X_XOR2
    port map (
      I0 => infrastructure_top_Result_10_cyo,
      I1 => Counter200_11_rt_165,
      O => Result(11)
    );
  infrastructure_top_Result_12_cy : X_MUX2
    port map (
      IB => infrastructure_top_Result_11_cyo,
      IA => N1,
      SEL => Counter200_12_rt_166,
      O => infrastructure_top_Result_12_cyo
    );
  infrastructure_top_Result_12_xor : X_XOR2
    port map (
      I0 => infrastructure_top_Result_11_cyo,
      I1 => Counter200_12_rt_166,
      O => Result(12)
    );
  infrastructure_top_Result_13_cy : X_MUX2
    port map (
      IB => infrastructure_top_Result_12_cyo,
      IA => N1,
      SEL => Counter200_13_rt_167,
      O => infrastructure_top_Result_13_cyo
    );
  infrastructure_top_Result_13_xor : X_XOR2
    port map (
      I0 => infrastructure_top_Result_12_cyo,
      I1 => Counter200_13_rt_167,
      O => Result(13)
    );
  infrastructure_top_Result_14_cy : X_MUX2
    port map (
      IB => infrastructure_top_Result_13_cyo,
      IA => N1,
      SEL => Counter200_14_rt_168,
      O => infrastructure_top_Result_14_cyo
    );
  infrastructure_top_Result_14_xor : X_XOR2
    port map (
      I0 => infrastructure_top_Result_13_cyo,
      I1 => Counter200_14_rt_168,
      O => Result(14)
    );
  infrastructure_top_Result_15_xor : X_XOR2
    port map (
      I0 => infrastructure_top_Result_14_cyo,
      I1 => Counter200_15_rt_169,
      O => Result(15)
    );
  user_rst1 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => reset_in,
      O => user_rst,
      ADR1 => GND
    );
  Q_n00131 : X_LUT2
    generic map(
      INIT => X"7"
    )
    port map (
      ADR0 => dcm_lock,
      ADR1 => reset_in,
      O => Q_n0013
    );
  Q_n00151 : X_LUT4
    generic map(
      INIT => X"8000"
    )
    port map (
      ADR0 => Counter200(13),
      ADR1 => Counter200(14),
      ADR2 => Counter200(15),
      ADR3 => NlwRenamedSig_OI_wait_200us,
      O => Q_n0015
    );
  Q_n00201 : X_LUT3
    generic map(
      INIT => X"08"
    )
    port map (
      ADR0 => dcm_lock,
      ADR1 => reset_in,
      ADR2 => wait_clk90_147,
      O => Q_n0020
    );
  Q_n00211 : X_LUT3
    generic map(
      INIT => X"08"
    )
    port map (
      ADR0 => dcm_lock,
      ADR1 => reset_in,
      ADR2 => NlwRenamedSig_OI_wait_200us,
      O => Q_n0021
    );
  Q_n00221 : X_LUT4
    generic map(
      INIT => X"4CCC"
    )
    port map (
      ADR0 => Counter200(13),
      ADR1 => NlwRenamedSig_OI_wait_200us,
      ADR2 => Counter200(14),
      ADR3 => Counter200(15),
      O => Q_n0022
    );
  cal_top0 : cal_top
    port map (
      clk => sys_clk_ibuf,
      reset => reset_in,
      clk0 => NlwRenamedSig_OI_clk_int_val,
      clk0dcmlock => dcm_lock,
      okToSelTap => vcc,
      tapForDqs(4) => delay_sel_val1_val(4),
      tapForDqs(3) => delay_sel_val1_val(3),
      tapForDqs(2) => delay_sel_val1_val(2),
      tapForDqs(1) => delay_sel_val1_val(1),
      tapForDqs(0) => delay_sel_val1_val(0)
    );
  Q_n00171 : X_LUT3
    generic map(
      INIT => X"F7"
    )
    port map (
      ADR0 => dcm_lock,
      ADR1 => reset_in,
      ADR2 => NlwRenamedSig_OI_wait_200us,
      O => Q_n0017
    );
  Q_n00161 : X_LUT3
    generic map(
      INIT => X"F7"
    )
    port map (
      ADR0 => dcm_lock,
      ADR1 => reset_in,
      ADR2 => wait_clk90_147,
      O => Q_n0016
    );
  Counter200_1_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => Counter200(1),
      O => Counter200_1_rt_155,
      ADR1 => GND
    );
  Counter200_2_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => Counter200(2),
      O => Counter200_2_rt_156,
      ADR1 => GND
    );
  Counter200_3_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => Counter200(3),
      O => Counter200_3_rt_157,
      ADR1 => GND
    );
  Counter200_4_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => Counter200(4),
      O => Counter200_4_rt_158,
      ADR1 => GND
    );
  Counter200_5_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => Counter200(5),
      O => Counter200_5_rt_159,
      ADR1 => GND
    );
  Counter200_6_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => Counter200(6),
      O => Counter200_6_rt_160,
      ADR1 => GND
    );
  Counter200_7_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => Counter200(7),
      O => Counter200_7_rt_161,
      ADR1 => GND
    );
  Counter200_8_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => Counter200(8),
      O => Counter200_8_rt_162,
      ADR1 => GND
    );
  Counter200_9_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => Counter200(9),
      O => Counter200_9_rt_163,
      ADR1 => GND
    );
  Counter200_10_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => Counter200(10),
      O => Counter200_10_rt_164,
      ADR1 => GND
    );
  Counter200_11_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => Counter200(11),
      O => Counter200_11_rt_165,
      ADR1 => GND
    );
  Counter200_12_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => Counter200(12),
      O => Counter200_12_rt_166,
      ADR1 => GND
    );
  Counter200_13_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => Counter200(13),
      O => Counter200_13_rt_167,
      ADR1 => GND
    );
  Counter200_14_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => Counter200(14),
      O => Counter200_14_rt_168,
      ADR1 => GND
    );
  Counter200_15_rt : X_LUT2
    generic map(
      INIT => X"A"
    )
    port map (
      ADR0 => Counter200(15),
      O => Counter200_15_rt_169,
      ADR1 => GND
    );
  clk_dcm0_BUFG_CLK90 : X_CKBUF
    generic map(
      LOC => "BUFGMUX7S"
    )
    port map (
      I => clk_dcm0_clk90d2inv,
      O => NlwRenamedSig_OI_clk90_int_val
    );
  clk_dcm0_BUFG_CLK0 : X_CKBUF
    generic map(
      LOC => "BUFGMUX6P"
    )
    port map (
      I => clk_dcm0_clk0d2inv,
      O => NlwRenamedSig_OI_clk_int_val
    );
  clk_dcm0_BUFG_CLKFB : X_CKBUF
    port map (
      I => clk_dcm0_clk0fb,
      O => clk_dcm0_clk0fb_buf
    );
  clk_dcm0_BUFG_CLKDV : X_CKBUF
    port map (
      I => clk_dcm0_clk_half,
      O => clk_dcm0_clk_half_buf
    );
  clk_dcm0_BUFG_CLKDV2 : X_CKBUF
    port map (
      I => clk_dcm0_clk0_o,
      O => clk_dcm0_clk0_o_buf
    );
  clk_dcm0_DCM_INST_DV : X_DCM
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
  clk_dcm0_DCM_INST1 : X_DCM
    generic map(
      DLL_FREQUENCY_MODE => "LOW",
      DUTY_CYCLE_CORRECTION => TRUE,
      LOC => "DCM_X3Y0"
    )
    port map (
      RST => clk_dcm0_rst_delay5_170,
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
  clk_dcm0_rst_delay5 : X_FF
    generic map(
      INIT => '1'
    )
    port map (
      I => clk_dcm0_rst_delay4_171,
      SET => user_rst,
      CLK => clk_dcm0_clk_half,
      O => clk_dcm0_rst_delay5_170,
      CE => VCC_175,
      RST => GND
    );
  clk_dcm0_rst_delay4 : X_FF
    generic map(
      INIT => '1'
    )
    port map (
      I => clk_dcm0_rst_delay3_172,
      SET => user_rst,
      CLK => clk_dcm0_clk_half,
      O => clk_dcm0_rst_delay4_171,
      CE => VCC_175,
      RST => GND
    );
  clk_dcm0_rst_delay3 : X_FF
    generic map(
      INIT => '1'
    )
    port map (
      I => clk_dcm0_rst_delay2_173,
      SET => user_rst,
      CLK => clk_dcm0_clk_half,
      O => clk_dcm0_rst_delay3_172,
      CE => VCC_175,
      RST => GND
    );
  clk_dcm0_rst_delay2 : X_FF
    generic map(
      INIT => '1'
    )
    port map (
      I => clk_dcm0_rst_delay1_174,
      SET => user_rst,
      CLK => clk_dcm0_clk_half,
      O => clk_dcm0_rst_delay2_173,
      CE => VCC_175,
      RST => GND
    );
  clk_dcm0_rst_delay1 : X_FF
    generic map(
      INIT => '1'
    )
    port map (
      I => clk_dcm0_gnd,
      SET => user_rst,
      CLK => clk_dcm0_clk_half,
      O => clk_dcm0_rst_delay1_174,
      CE => VCC_175,
      RST => GND
    );
  clk_dcm0_DCD0 : dcmx3y0_2vp20
    port map (
      clock1_in => clk_dcm0_clk0dcm,
      clock2_in => clk_dcm0_clk90dcm,
      clock2_out => clk_dcm0_clk90d2inv,
      clock1_out => clk_dcm0_clk0d2inv
    );
  clk_dcm0_XST_GND : X_ZERO
    port map (
      O => clk_dcm0_gnd
    );
  NlwBlock_infrastructure_top0_VCC : X_ONE
    port map (
      O => VCC_175
    );
  NlwBlock_infrastructure_top0_GND : X_ZERO
    port map (
      O => GND
    );
  NlwInverterBlock_mem_interface_top_inst_infrastructure_top0_sys_rst180_o_C : X_INV
    port map (
      I => NlwRenamedSig_OI_clk_int_val,
      O => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst180_o_C
    );
  NlwInverterBlock_mem_interface_top_inst_infrastructure_top0_sys_rst270_o_C : X_INV
    port map (
      I => NlwRenamedSig_OI_clk90_int_val,
      O => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst270_o_C
    );
  NlwInverterBlock_mem_interface_top_inst_infrastructure_top0_sys_rst270_1_C : X_INV
    port map (
      I => NlwRenamedSig_OI_clk90_int_val,
      O => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst270_1_C
    );
  NlwInverterBlock_mem_interface_top_inst_infrastructure_top0_sys_rst180_1_C : X_INV
    port map (
      I => NlwRenamedSig_OI_clk_int_val,
      O => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst180_1_C
    );
  NlwInverterBlock_mem_interface_top_inst_infrastructure_top0_sys_rst180_C : X_INV
    port map (
      I => NlwRenamedSig_OI_clk_int_val,
      O => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst180_C
    );
  NlwInverterBlock_mem_interface_top_inst_infrastructure_top0_sys_rst270_C : X_INV
    port map (
      I => NlwRenamedSig_OI_clk90_int_val,
      O => NlwInverterSignal_mem_interface_top_inst_infrastructure_top0_sys_rst270_C
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
  signal GND : STD_LOGIC; 
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
  clk180_11 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_clk_int_1,
      O => NlwRenamedSig_OI_clk180_1,
      ADR1 => GND
    );
  clk270_11 : X_LUT2
    generic map(
      INIT => X"5"
    )
    port map (
      ADR0 => NlwRenamedSig_OI_clk90_int_1,
      O => NlwRenamedSig_OI_clk270_1,
      ADR1 => GND
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
  NlwBlock_mem_interface_top_inst_GND : X_ZERO
    port map (
      O => GND
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library SIMPRIM;
use SIMPRIM.VCOMPONENTS.ALL;
use SIMPRIM.VPACKAGE.ALL;

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
    rst_dqs_div : in STD_LOGIC := 'X'; 
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
  signal mreset_i_IBUF_181 : STD_LOGIC; 
  signal NlwRenamedSig_IO_rst_dqs_div : STD_LOGIC; 
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
  signal led_o_2_OBUF_182 : STD_LOGIC; 
  signal led_o_1_OBUF_183 : STD_LOGIC; 
  signal led_o_0_OBUF_184 : STD_LOGIC; 
  signal pc_data_o_21_OBUF_185 : STD_LOGIC; 
  signal pc_data_o_20_OBUF_186 : STD_LOGIC; 
  signal pc_data_o_19_OBUF_187 : STD_LOGIC; 
  signal pc_data_o_18_OBUF_188 : STD_LOGIC; 
  signal pc_data_o_17_OBUF_189 : STD_LOGIC; 
  signal pc_data_o_16_OBUF_190 : STD_LOGIC; 
  signal pc_data_o_15_OBUF_191 : STD_LOGIC; 
  signal pc_data_o_14_OBUF_192 : STD_LOGIC; 
  signal pc_data_o_13_OBUF_193 : STD_LOGIC; 
  signal pc_data_o_12_OBUF_194 : STD_LOGIC; 
  signal pc_data_o_11_OBUF_195 : STD_LOGIC; 
  signal pc_data_o_10_OBUF_196 : STD_LOGIC; 
  signal pc_data_o_9_OBUF_197 : STD_LOGIC; 
  signal pc_data_o_8_OBUF_198 : STD_LOGIC; 
  signal pc_data_o_7_OBUF_199 : STD_LOGIC; 
  signal pc_data_o_6_OBUF_200 : STD_LOGIC; 
  signal pc_data_o_5_OBUF_201 : STD_LOGIC; 
  signal pc_data_o_4_OBUF_202 : STD_LOGIC; 
  signal pc_data_o_3_OBUF_203 : STD_LOGIC; 
  signal pc_data_o_2_OBUF_204 : STD_LOGIC; 
  signal pc_data_o_1_OBUF_205 : STD_LOGIC; 
  signal pc_data_o_0_OBUF_206 : STD_LOGIC; 
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
  NlwRenamedSig_IO_rst_dqs_div <= rst_dqs_div;
  diff_buf_brefclk : X_IBUFDS
    generic map(
      DIFF_TERM => FALSE
    )
    port map (
      I => brefclk_p_i,
      IB => brefclk_n_i,
      O => brefclk
    );
  diff_buf_brefclk2 : X_IBUFDS
    generic map(
      DIFF_TERM => FALSE
    )
    port map (
      I => brefclk2_p_i,
      IB => brefclk2_n_i,
      O => brefclk2
    );
  clk1_for_logic : X_CKBUF
    port map (
      I => brefclk,
      O => clk1
    );
  clk2_for_logic : X_CKBUF
    port map (
      I => brefclk2,
      O => NLW_clk2_for_logic_O_UNCONNECTED
    );
  mem_interface_top_inst : mem_interface_top
    port map (
      cntrl0_rst_dqs_div_in => NlwRenamedSig_IO_rst_dqs_div,
      cntrl0_burst_done => burst_done,
      reset_in => mreset_i_IBUF_181,
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
      cntrl0_rst_dqs_div_out => NlwRenamedSig_IO_rst_dqs_div,
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
      reset => mreset_i_IBUF_181,
      ar_done => ar_done,
      auto_ref_req => auto_ref_req,
      init_val => init_val,
      clk180 => clk180,
      clk90 => clk90,
      user_cmd_ack => user_cmd_ack,
      user_data_valid => user_data_valid,
      TP1 => led_o_0_OBUF_184,
      TP2 => led_o_1_OBUF_183,
      TP3 => led_o_2_OBUF_182,
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
      TP_port(21) => pc_data_o_21_OBUF_185,
      TP_port(20) => pc_data_o_20_OBUF_186,
      TP_port(19) => pc_data_o_19_OBUF_187,
      TP_port(18) => pc_data_o_18_OBUF_188,
      TP_port(17) => pc_data_o_17_OBUF_189,
      TP_port(16) => pc_data_o_16_OBUF_190,
      TP_port(15) => pc_data_o_15_OBUF_191,
      TP_port(14) => pc_data_o_14_OBUF_192,
      TP_port(13) => pc_data_o_13_OBUF_193,
      TP_port(12) => pc_data_o_12_OBUF_194,
      TP_port(11) => pc_data_o_11_OBUF_195,
      TP_port(10) => pc_data_o_10_OBUF_196,
      TP_port(9) => pc_data_o_9_OBUF_197,
      TP_port(8) => pc_data_o_8_OBUF_198,
      TP_port(7) => pc_data_o_7_OBUF_199,
      TP_port(6) => pc_data_o_6_OBUF_200,
      TP_port(5) => pc_data_o_5_OBUF_201,
      TP_port(4) => pc_data_o_4_OBUF_202,
      TP_port(3) => pc_data_o_3_OBUF_203,
      TP_port(2) => pc_data_o_2_OBUF_204,
      TP_port(1) => pc_data_o_1_OBUF_205,
      TP_port(0) => pc_data_o_0_OBUF_206,
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
  mreset_i_IBUF : X_BUF
    port map (
      I => mreset_i,
      O => mreset_i_IBUF_181
    );
  led_o_2_OBUF : X_OBUF
    port map (
      I => led_o_2_OBUF_182,
      O => led_o(2)
    );
  led_o_1_OBUF : X_OBUF
    port map (
      I => led_o_1_OBUF_183,
      O => led_o(1)
    );
  led_o_0_OBUF : X_OBUF
    port map (
      I => led_o_0_OBUF_184,
      O => led_o(0)
    );
  pc_data_o_21_OBUF : X_OBUF
    port map (
      I => pc_data_o_21_OBUF_185,
      O => pc_data_o(21)
    );
  pc_data_o_20_OBUF : X_OBUF
    port map (
      I => pc_data_o_20_OBUF_186,
      O => pc_data_o(20)
    );
  pc_data_o_19_OBUF : X_OBUF
    port map (
      I => pc_data_o_19_OBUF_187,
      O => pc_data_o(19)
    );
  pc_data_o_18_OBUF : X_OBUF
    port map (
      I => pc_data_o_18_OBUF_188,
      O => pc_data_o(18)
    );
  pc_data_o_17_OBUF : X_OBUF
    port map (
      I => pc_data_o_17_OBUF_189,
      O => pc_data_o(17)
    );
  pc_data_o_16_OBUF : X_OBUF
    port map (
      I => pc_data_o_16_OBUF_190,
      O => pc_data_o(16)
    );
  pc_data_o_15_OBUF : X_OBUF
    port map (
      I => pc_data_o_15_OBUF_191,
      O => pc_data_o(15)
    );
  pc_data_o_14_OBUF : X_OBUF
    port map (
      I => pc_data_o_14_OBUF_192,
      O => pc_data_o(14)
    );
  pc_data_o_13_OBUF : X_OBUF
    port map (
      I => pc_data_o_13_OBUF_193,
      O => pc_data_o(13)
    );
  pc_data_o_12_OBUF : X_OBUF
    port map (
      I => pc_data_o_12_OBUF_194,
      O => pc_data_o(12)
    );
  pc_data_o_11_OBUF : X_OBUF
    port map (
      I => pc_data_o_11_OBUF_195,
      O => pc_data_o(11)
    );
  pc_data_o_10_OBUF : X_OBUF
    port map (
      I => pc_data_o_10_OBUF_196,
      O => pc_data_o(10)
    );
  pc_data_o_9_OBUF : X_OBUF
    port map (
      I => pc_data_o_9_OBUF_197,
      O => pc_data_o(9)
    );
  pc_data_o_8_OBUF : X_OBUF
    port map (
      I => pc_data_o_8_OBUF_198,
      O => pc_data_o(8)
    );
  pc_data_o_7_OBUF : X_OBUF
    port map (
      I => pc_data_o_7_OBUF_199,
      O => pc_data_o(7)
    );
  pc_data_o_6_OBUF : X_OBUF
    port map (
      I => pc_data_o_6_OBUF_200,
      O => pc_data_o(6)
    );
  pc_data_o_5_OBUF : X_OBUF
    port map (
      I => pc_data_o_5_OBUF_201,
      O => pc_data_o(5)
    );
  pc_data_o_4_OBUF : X_OBUF
    port map (
      I => pc_data_o_4_OBUF_202,
      O => pc_data_o(4)
    );
  pc_data_o_3_OBUF : X_OBUF
    port map (
      I => pc_data_o_3_OBUF_203,
      O => pc_data_o(3)
    );
  pc_data_o_2_OBUF : X_OBUF
    port map (
      I => pc_data_o_2_OBUF_204,
      O => pc_data_o(2)
    );
  pc_data_o_1_OBUF : X_OBUF
    port map (
      I => pc_data_o_1_OBUF_205,
      O => pc_data_o(1)
    );
  pc_data_o_0_OBUF : X_OBUF
    port map (
      I => pc_data_o_0_OBUF_206,
      O => pc_data_o(0)
    );
  NlwBlockROC : X_ROC
    generic map (ROC_WIDTH => 100 ns)
    port map (O => GSR);
  NlwBlockTOC : X_TOC
    port map (O => GTS);

end Structure;

