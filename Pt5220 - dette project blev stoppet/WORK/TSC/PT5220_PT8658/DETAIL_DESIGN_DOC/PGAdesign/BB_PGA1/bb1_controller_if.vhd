-- Navn			: BB1_controller_if
-- Dato			: 99-05-
-- Rev.nr.		: 0.1
-- Funktion		: Controller writes data to the PGA and receives level.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_controller_if IS
	PORT(
--		clk									:	IN	STD_LOGIC;
--		bb1_pga1_rd							:	IN	STD_LOGIC;
--		bb1_pga1_wr							:	IN	STD_LOGIC;
		bb1_pga1_load_ltype_poff_hc10_8		:	IN	STD_LOGIC;
		bb1_pga1_load_hc7_0					:	IN	STD_LOGIC;
		bb1_pga1_load_b7_0					:	IN	STD_LOGIC;
		bb1_pga1_load_G_M_TCon_TCrdy		:	IN	STD_LOGIC;
		bb1_pga1_load_hf7_0					:	IN	STD_LOGIC;

		Reg_bb1_dio							:	IN	UNSIGNED(7 DOWNTO 0); 

		Reg_bb1_LineType					:	OUT	INTEGER RANGE 0 TO 15;
		Reg_bb1_PedOff						:	OUT	INTEGER RANGE 0 TO 1;
		Reg_bb1_HphaseCoarse				:	OUT	INTEGER RANGE 0 TO 2047;
		Reg_bb1_BurstPhase					:	OUT	INTEGER RANGE 0 TO 2047;
		Reg_bb1_HphaseFine					:	OUT	INTEGER RANGE 0 TO 127;
		Reg_bb1_System						:	OUT	INTEGER RANGE 0 TO 1;
		Reg_bb1_TC_on_off					:	OUT	INTEGER RANGE 0 TO 1;
		Reg_bb1_TC_rdy_off					:	OUT	INTEGER RANGE 0 TO 1
--		Reg_bb1_info						:	OUT	STD_LOGIC_VECTOR(7 DOWNTO 0)
		);
END BB1_controller_if;

ARCHITECTURE DATA2controller_if OF BB1_controller_if IS
	SIGNAL vec_H_hi :UNSIGNED(10 DOWNTO 0);
	SIGNAL vec_Burst_hi :UNSIGNED(10 DOWNTO 0);
	SIGNAL vec_H_low :UNSIGNED(7 DOWNTO 0);
	SIGNAL vec_Burst_low :UNSIGNED(7 DOWNTO 0);

BEGIN
	PROCESS (bb1_pga1_load_ltype_poff_hc10_8)

	BEGIN
		IF (bb1_pga1_load_ltype_poff_hc10_8'EVENT AND bb1_pga1_load_ltype_poff_hc10_8 = '0') THEN

						Reg_bb1_LineType <= CONV_INTEGER (Reg_bb1_dio(7 DOWNTO 4));
						Reg_bb1_PedOff <= CONV_INTEGER(Reg_bb1_dio(3));
						vec_H_hi(10) <= Reg_bb1_dio(2);
						vec_H_hi(9) <= Reg_bb1_dio(1);
						vec_H_hi(8) <= Reg_bb1_dio(0);

		END IF;
	END PROCESS;

	PROCESS (bb1_pga1_load_hc7_0)
	BEGIN
		IF (bb1_pga1_load_hc7_0'EVENT AND bb1_pga1_load_hc7_0 = '0') THEN

						vec_H_low(7) <= Reg_bb1_dio(7);
						vec_H_low(6) <= Reg_bb1_dio(6);
						vec_H_low(5) <= Reg_bb1_dio(5);
						vec_H_low(4) <= Reg_bb1_dio(4);
						vec_H_low(3) <= Reg_bb1_dio(3);
						vec_H_low(2) <= Reg_bb1_dio(2);
						vec_H_low(1) <= Reg_bb1_dio(1);
						vec_H_low(0) <= Reg_bb1_dio(0);
		END IF;
	END PROCESS;

	PROCESS (bb1_pga1_load_b7_0)
	BEGIN
		IF (bb1_pga1_load_b7_0'EVENT AND bb1_pga1_load_b7_0 = '0') THEN

						vec_Burst_low(7) <= Reg_bb1_dio(7);
						vec_Burst_low(6) <= Reg_bb1_dio(6);
						vec_Burst_low(5) <= Reg_bb1_dio(5);
						vec_Burst_low(4) <= Reg_bb1_dio(4);
						vec_Burst_low(3) <= Reg_bb1_dio(3);
						vec_Burst_low(2) <= Reg_bb1_dio(2);
						vec_Burst_low(1) <= Reg_bb1_dio(1);
						vec_Burst_low(0) <= Reg_bb1_dio(0);

		END IF;
	END PROCESS;

	PROCESS (bb1_pga1_load_G_M_TCon_TCrdy)
	BEGIN
		IF (bb1_pga1_load_G_M_TCon_TCrdy'EVENT AND bb1_pga1_load_G_M_TCon_TCrdy = '0') THEN
						vec_Burst_hi(10) <= Reg_bb1_dio(2);
						vec_Burst_hi(9) <= Reg_bb1_dio(1);
						vec_Burst_hi(8) <= Reg_bb1_dio(0);
						Reg_bb1_System <= CONV_INTEGER(Reg_bb1_dio(7));
						Reg_bb1_TC_on_off <= CONV_INTEGER(Reg_bb1_dio(6));
						Reg_bb1_TC_rdy_off <= CONV_INTEGER(Reg_bb1_dio(5));
		END IF;
	END PROCESS;

	PROCESS (bb1_pga1_load_hf7_0)
	BEGIN
		IF (bb1_pga1_load_hf7_0'EVENT AND bb1_pga1_load_hf7_0 = '0') THEN
						Reg_bb1_HphaseFine <= CONV_INTEGER(Reg_bb1_dio(7 DOWNTO 0));
		END IF;
	END PROCESS;
						vec_H_hi(7) <= vec_h_low(7);
						vec_H_hi(6) <= vec_h_low(6);
						vec_H_hi(5) <= vec_h_low(5);
						vec_H_hi(4) <= vec_h_low(4);
						vec_H_hi(3) <= vec_h_low(3);
						vec_H_hi(2) <= vec_h_low(2);
						vec_H_hi(1) <= vec_h_low(1);
						vec_H_hi(0) <= vec_h_low(0);
						vec_Burst_hi(7) <= vec_Burst_low(7);
						vec_Burst_hi(6) <= vec_Burst_low(6);
						vec_Burst_hi(5) <= vec_Burst_low(5);
						vec_Burst_hi(4) <= vec_Burst_low(4);
						vec_Burst_hi(3) <= vec_Burst_low(3);
						vec_Burst_hi(2) <= vec_Burst_low(2);
						vec_Burst_hi(1) <= vec_Burst_low(1);
						vec_Burst_hi(0) <= vec_Burst_low(0);

	Reg_bb1_HphaseCoarse <= CONV_INTEGER(vec_h_hi);
	Reg_bb1_BurstPhase	<= 	CONV_INTEGER(vec_Burst_hi);
	

END DATA2controller_if;

