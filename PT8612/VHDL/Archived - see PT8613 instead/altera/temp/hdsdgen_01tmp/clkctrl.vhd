-- megafunction wizard: %ALTCLKCTRL%CBX%
-- GENERATION: STANDARD
-- VERSION: WM1.0
-- MODULE: altclkctrl 

-- ============================================================
-- File Name: clkctrl.vhd
-- Megafunction Name(s):
-- 			altclkctrl
-- ============================================================
-- ************************************************************
-- THIS IS A WIZARD-GENERATED FILE. DO NOT EDIT THIS FILE!
--
-- 4.1 Build 208 09/10/2004 SP 2 SJ Web Edition
-- ************************************************************


--Copyright (C) 1991-2004 Altera Corporation
--Any  megafunction  design,  and related netlist (encrypted  or  decrypted),
--support information,  device programming or simulation file,  and any other
--associated  documentation or information  provided by  Altera  or a partner
--under  Altera's   Megafunction   Partnership   Program  may  be  used  only
--to program  PLD  devices (but not masked  PLD  devices) from  Altera.   Any
--other  use  of such  megafunction  design,  netlist,  support  information,
--device programming or simulation file,  or any other  related documentation
--or information  is prohibited  for  any  other purpose,  including, but not
--limited to  modification,  reverse engineering,  de-compiling, or use  with
--any other  silicon devices,  unless such use is  explicitly  licensed under
--a separate agreement with  Altera  or a megafunction partner.  Title to the
--intellectual property,  including patents,  copyrights,  trademarks,  trade
--secrets,  or maskworks,  embodied in any such megafunction design, netlist,
--support  information,  device programming or simulation file,  or any other
--related documentation or information provided by  Altera  or a megafunction
--partner, remains with Altera, the megafunction partner, or their respective
--licensors. No other licenses, including any licenses needed under any third
--party's intellectual property, are provided herein.


--altclkctrl clock_type="Global Clock" DEVICE_FAMILY="CYCLONE II" ena_register_mode="falling edge" clkselect ena inclk outclk
--VERSION_BEGIN 4.1 cbx_altclkbuf 2004:04:26:13:40:04:SJ cbx_mgl 2004:06:17:17:30:06:SJ cbx_stratixii 2004:05:18:11:28:28:SJ  VERSION_END

 LIBRARY cycloneii;
 USE cycloneii.all;

--synthesis_resources = stratixii_clkctrl 1 
 LIBRARY ieee;
 USE ieee.std_logic_1164.all;

 ENTITY  clkctrl_altclkctrl_cdb IS 
	 PORT 
	 ( 
		 clkselect	:	IN  STD_LOGIC_VECTOR (1 DOWNTO 0) := (OTHERS => '0');
		 ena	:	IN  STD_LOGIC := '1';
		 inclk	:	IN  STD_LOGIC_VECTOR (3 DOWNTO 0) := (OTHERS => '0');
		 outclk	:	OUT  STD_LOGIC
	 ); 
 END clkctrl_altclkctrl_cdb;

 ARCHITECTURE RTL OF clkctrl_altclkctrl_cdb IS

	 ATTRIBUTE synthesis_clearbox : boolean;
	 ATTRIBUTE synthesis_clearbox OF RTL : ARCHITECTURE IS true;
	 SIGNAL  wire_clkctrl1_outclk	:	STD_LOGIC;
	 COMPONENT  cycloneii_clkctrl
	 GENERIC 
	 (
		clock_type	:	STRING;
		ena_register_mode	:	STRING := "falling edge";
		lpm_type	:	STRING := "cycloneii_clkctrl"
	 );
	 PORT
	 ( 
		clkselect	:	IN STD_LOGIC_VECTOR(1 DOWNTO 0);
		ena	:	IN STD_LOGIC;
		inclk	:	IN STD_LOGIC_VECTOR(3 DOWNTO 0);
		outclk	:	OUT STD_LOGIC
	 ); 
	 END COMPONENT;
 BEGIN

	outclk <= wire_clkctrl1_outclk;
	clkctrl1 :  cycloneii_clkctrl
	  GENERIC MAP (
		clock_type => "Global Clock",
		ena_register_mode => "falling edge"
	  )
	  PORT MAP ( 
		clkselect => clkselect,
		ena => ena,
		inclk => inclk,
		outclk => wire_clkctrl1_outclk
	  );

 END RTL; --clkctrl_altclkctrl_cdb
--VALID FILE


LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY clkctrl IS
	PORT
	(
		ena		: IN STD_LOGIC  := '1';
		clkselect		: IN STD_LOGIC  := '0';
		inclk1x		: IN STD_LOGIC ;
		inclk0x		: IN STD_LOGIC ;
		outclk		: OUT STD_LOGIC 
	);
END clkctrl;


ARCHITECTURE RTL OF clkctrl IS

	ATTRIBUTE synthesis_clearbox: boolean;
	ATTRIBUTE synthesis_clearbox OF RTL: ARCHITECTURE IS TRUE;
	SIGNAL sub_wire0	: STD_LOGIC ;
	SIGNAL sub_wire1	: STD_LOGIC ;
	SIGNAL sub_wire2	: STD_LOGIC_VECTOR (3 DOWNTO 0);
	SIGNAL sub_wire3	: STD_LOGIC ;
	SIGNAL sub_wire4_bv	: BIT_VECTOR (1 DOWNTO 0);
	SIGNAL sub_wire4	: STD_LOGIC_VECTOR (1 DOWNTO 0);
	SIGNAL sub_wire5	: STD_LOGIC ;
	SIGNAL sub_wire6	: STD_LOGIC_VECTOR (1 DOWNTO 0);
	SIGNAL sub_wire7_bv	: BIT_VECTOR (0 DOWNTO 0);
	SIGNAL sub_wire7	: STD_LOGIC_VECTOR (0 DOWNTO 0);



	COMPONENT clkctrl_altclkctrl_cdb
	PORT (
			ena	: IN STD_LOGIC ;
			outclk	: OUT STD_LOGIC ;
			inclk	: IN STD_LOGIC_VECTOR (3 DOWNTO 0);
			clkselect	: IN STD_LOGIC_VECTOR (1 DOWNTO 0)
	);
	END COMPONENT;

BEGIN
	sub_wire4_bv(1 DOWNTO 0) <= "00";
	sub_wire4    <= To_stdlogicvector(sub_wire4_bv);
	sub_wire7_bv(0 DOWNTO 0) <= "0";
	sub_wire7    <= To_stdlogicvector(sub_wire7_bv);
	sub_wire3    <= inclk1x;
	outclk    <= sub_wire0;
	sub_wire1    <= inclk0x;
	sub_wire2    <= sub_wire4(1 DOWNTO 0) & sub_wire3 & sub_wire1;
	sub_wire5    <= clkselect;
	sub_wire6    <= sub_wire7(0 DOWNTO 0) & sub_wire5;

	clkctrl_altclkctrl_cdb_component : clkctrl_altclkctrl_cdb
	PORT MAP (
		ena => ena,
		inclk => sub_wire2,
		clkselect => sub_wire6,
		outclk => sub_wire0
	);



END RTL;

-- ============================================================
-- CNX file retrieval info
-- ============================================================
-- Retrieval info: PRIVATE: clock_inputs NUMERIC "2"
-- Retrieval info: PRIVATE: INTENDED_DEVICE_FAMILY STRING "Cyclone II"
-- Retrieval info: CONSTANT: ENA_REGISTER_MODE STRING "falling edge"
-- Retrieval info: CONSTANT: INTENDED_DEVICE_FAMILY STRING "Cyclone II"
-- Retrieval info: CONSTANT: clock_type STRING "Global Clock"
-- Retrieval info: USED_PORT: ena 0 0 0 0 INPUT VCC "ena"
-- Retrieval info: USED_PORT: outclk 0 0 0 0 OUTPUT NODEFVAL "outclk"
-- Retrieval info: USED_PORT: clkselect 0 0 0 0 INPUT GND "clkselect"
-- Retrieval info: USED_PORT: inclk1x 0 0 0 0 INPUT NODEFVAL "inclk1x"
-- Retrieval info: USED_PORT: inclk0x 0 0 0 0 INPUT NODEFVAL "inclk0x"
-- Retrieval info: CONNECT: @ena 0 0 0 0 ena 0 0 0 0
-- Retrieval info: CONNECT: @clkselect 0 0 1 1 GND 0 0 0 0
-- Retrieval info: CONNECT: @inclk 0 0 1 1 inclk1x 0 0 0 0
-- Retrieval info: CONNECT: @inclk 0 0 1 0 inclk0x 0 0 0 0
-- Retrieval info: CONNECT: @clkselect 0 0 1 0 clkselect 0 0 0 0
-- Retrieval info: CONNECT: @inclk 0 0 2 2 GND 0 0 2 0
-- Retrieval info: CONNECT: outclk 0 0 0 0 @outclk 0 0 0 0
-- Retrieval info: GEN_FILE: TYPE_NORMAL clkctrl.vhd TRUE FALSE
-- Retrieval info: GEN_FILE: TYPE_NORMAL clkctrl.inc FALSE FALSE
-- Retrieval info: GEN_FILE: TYPE_NORMAL clkctrl.cmp FALSE FALSE
-- Retrieval info: GEN_FILE: TYPE_NORMAL clkctrl.bsf FALSE FALSE
-- Retrieval info: GEN_FILE: TYPE_NORMAL clkctrl_inst.vhd FALSE FALSE
