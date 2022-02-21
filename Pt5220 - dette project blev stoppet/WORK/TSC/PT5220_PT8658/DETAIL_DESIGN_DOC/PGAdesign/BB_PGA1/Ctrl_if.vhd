-- megafunction wizard: %LPM_SHIFTREG%
-- GENERATION: STANDARD
-- VERSION: WM1.0
-- MODULE: lpm_shiftreg 

-- ============================================================
-- File Name: Ctrl_if.vhd
-- Megafunction Name(s):
-- 			lpm_shiftreg
-- ============================================================
-- ************************************************************
-- THIS IS A WIZARD GENERATED FILE. DO NOT EDIT THIS FILE!
-- ************************************************************


--	Copyright (C) 1988-1998 Altera Corporation

--	Any megafunction design, and related net list (encrypted or decrypted),
--	support information, device programming or simulation file, and any other
--	associated documentation or information provided by Altera or a partner
--	under Altera's Megafunction Partnership Program may be used only to
--	program PLD devices (but not masked PLD devices) from Altera.  Any other
--	use of such megafunction design, net list, support information, device
--	programming or simulation file, or any other related documentation or
--	information is prohibited for any other purpose, including, but not
--	limited to modification, reverse engineering, de-compiling, or use with
--	any other silicon devices, unless such use is explicitly licensed under
--	a separate agreement with Altera or a megafunction partner.  Title to
--	the intellectual property, including patents, copyrights, trademarks,
--	trade secrets, or maskworks, embodied in any such megafunction design,
--	net list, support information, device programming or simulation file, or
--	any other related documentation or information provided by Altera or a
--	megafunction partner, remains with Altera, the megafunction partner, or
--	their respective licensors.  No other licenses, including any licenses
--	needed under any third party's intellectual property, are provided herein.

LIBRARY ieee;
USE ieee.std_logic_1164.all;
LIBRARY lpm;
USE lpm.lpm_components.all;

ENTITY Ctrl_if IS
	PORT
	(
		clock		: IN STD_LOGIC ;
		q		: OUT STD_LOGIC_VECTOR (7 DOWNTO 0);
		shiftout		: OUT STD_LOGIC 
	);
END Ctrl_if;


ARCHITECTURE SYN OF Ctrl_if IS

	SIGNAL sub_wire0	: STD_LOGIC_VECTOR (7 DOWNTO 0);
	SIGNAL sub_wire1	: STD_LOGIC ;



	COMPONENT lpm_shiftreg
	GENERIC (
		LPM_WIDTH		: POSITIVE;
		LPM_DIRECTION		: STRING
	);
	PORT (
			clock	: IN STD_LOGIC ;
			q	: OUT STD_LOGIC_VECTOR (7 DOWNTO 0);
			shiftout	: OUT STD_LOGIC 
	);
	END COMPONENT;

BEGIN
	q    <= sub_wire0(7 DOWNTO 0);
	shiftout    <= sub_wire1;

	lpm_shiftreg_component : lpm_shiftreg
	GENERIC MAP (
		LPM_WIDTH => 8,
		LPM_DIRECTION => "LEFT"
	)
	PORT MAP (
		clock => clock,
		q => sub_wire0,
		shiftout => sub_wire1
	);



END SYN;

-- ============================================================
-- CNX file retrieval info
-- ============================================================
-- Retrieval info: PRIVATE: nBit NUMERIC "8"
-- Retrieval info: PRIVATE: LeftShift NUMERIC "1"
-- Retrieval info: PRIVATE: Q_OUT NUMERIC "0"
-- Retrieval info: PRIVATE: SerialShiftOutput NUMERIC "1"
-- Retrieval info: PRIVATE: CLK_EN NUMERIC "0"
-- Retrieval info: PRIVATE: SerialShiftInput NUMERIC "0"
-- Retrieval info: PRIVATE: SCLR NUMERIC "0"
-- Retrieval info: PRIVATE: SLOAD NUMERIC "0"
-- Retrieval info: PRIVATE: SSET NUMERIC "0"
-- Retrieval info: PRIVATE: SSET_ALL1 NUMERIC "1"
-- Retrieval info: PRIVATE: SSETV NUMERIC "0"
-- Retrieval info: PRIVATE: ACLR NUMERIC "0"
-- Retrieval info: PRIVATE: ALOAD NUMERIC "0"
-- Retrieval info: PRIVATE: ASET NUMERIC "0"
-- Retrieval info: PRIVATE: ASET_ALL1 NUMERIC "1"
-- Retrieval info: PRIVATE: ASETV NUMERIC "0"
-- Retrieval info: CONSTANT: LPM_WIDTH NUMERIC "8"
-- Retrieval info: CONSTANT: LPM_DIRECTION STRING "LEFT"
-- Retrieval info: USED_PORT: clock 0 0 0 0 INPUT NODEFVAL clock
-- Retrieval info: USED_PORT: q 0 0 8 0 OUTPUT NODEFVAL q[7..0]
-- Retrieval info: USED_PORT: shiftout 0 0 0 0 OUTPUT NODEFVAL shiftout
-- Retrieval info: CONNECT: @clock 0 0 0 0 clock 0 0 0 0
-- Retrieval info: CONNECT: q 0 0 8 0 @q 0 0 8 0
-- Retrieval info: CONNECT: shiftout 0 0 0 0 @shiftout 0 0 0 0
