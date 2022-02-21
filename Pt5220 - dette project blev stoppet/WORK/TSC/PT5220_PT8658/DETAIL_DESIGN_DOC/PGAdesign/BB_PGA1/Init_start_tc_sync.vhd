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
--		clk								:	IN	STD_LOGIC;
		wr								:	IN	STD_LOGIC;
--		rd								:	IN	STD_LOGIC;
		load_sync						:	IN	STD_LOGIC_VECTOR(3 DOWNTO 0);
--		load_burst						:	IN	BIT;
--		load_timecode					:	IN	BIT;
--		load_video						:	IN	BIT;
--		load_black						:	IN	BIT;
		Reg_bb1_din						:	IN	STD_LOGIC_VECTOR(7 DOWNTO 0); 

		Reg_bb1_LineType				:	OUT	STD_LOGIC_VECTOR(3 DOWNTO 0);
		Reg_bb1_PedOff					:	OUT	STD_LOGIC;
		Reg_bb1_HphaseCoarse			:	OUT	STD_LOGIC_VECTOR(10 DOWNTO 0);
		Reg_bb1_BurstPhase				:	OUT	STD_LOGIC_VECTOR(10 DOWNTO 0);
		Reg_bb1_System					:	OUT	STD_LOGIC;
		Reg_bb1_TC_on_off				:	OUT	STD_LOGIC;
		Reg_bb1_TC_rdy_off				:	OUT	STD_LOGIC;
		Reg_bb1_HphaseFine				:	OUT	STD_LOGIC_VECTOR(7 DOWNTO 0);
		Reg_bb1_Timercode2_9			:	OUT	STD_LOGIC_VECTOR(7 DOWNTO 0);
		Reg_bb1_Timercode12_19			:	OUT	STD_LOGIC_VECTOR(7 DOWNTO 0);
		Reg_bb1_Timercode22_29			:	OUT	STD_LOGIC_VECTOR(7 DOWNTO 0);
		Reg_bb1_Timercode32_39			:	OUT	STD_LOGIC_VECTOR(7 DOWNTO 0);
		Reg_bb1_Timercode42_49			:	OUT	STD_LOGIC_VECTOR(7 DOWNTO 0);
		Reg_bb1_Timercode52_59			:	OUT	STD_LOGIC_VECTOR(7 DOWNTO 0);
		Reg_bb1_Timercode62_69			:	OUT	STD_LOGIC_VECTOR(7 DOWNTO 0);
		Reg_bb1_Timecode72_79			:	OUT	STD_LOGIC_VECTOR(7 DOWNTO 0)
--		Reg_bb1_info					:	OUT	STD_LOGIC_VECTOR(7 DOWNTO 0)
		);
END BB1_controller_if;

ARCHITECTURE DATA2controller_if OF BB1_controller_if IS
	TYPE STATE_TYPE IS (s0_load_pga);
	SIGNAL state: STATE_TYPE;
BEGIN
--	PROCESS (clk)
	PROCESS (wr)
	BEGIN
--		IF (clk'EVENT AND clk = '1') THEN
		IF (wr'EVENT AND wr = '0') THEN
--			CASE state IS
--				WHEN s0_load_pga =>

					IF load_sync = "0000" THEN
						Reg_bb1_LineType(3 DOWNTO 0) <= Reg_bb1_din(7 DOWNTO 4);
						Reg_bb1_PedOff <= Reg_bb1_din(3);
						Reg_bb1_HphaseCoarse(10 DOWNTO 8) <= Reg_bb1_din(2 DOWNTO 0);

					ELSIF load_sync = "0001" THEN
						Reg_bb1_HphaseCoarse(7 DOWNTO 0) <= Reg_bb1_din(7 DOWNTO 0);

					ELSIF load_sync = "0010" THEN
						Reg_bb1_BurstPhase(7 DOWNTO 0) <= Reg_bb1_din(7 DOWNTO 0);

					ELSIF load_sync = "0011" THEN
						Reg_bb1_BurstPhase(10 DOWNTO 8) <= Reg_bb1_din(3 DOWNTO 0);
						Reg_bb1_System <= Reg_bb1_din(7);
						Reg_bb1_TC_on_off <= Reg_bb1_din(6);
						Reg_bb1_TC_rdy_off <= Reg_bb1_din(5);

					ELSIF load_sync = "0100" THEN
						Reg_bb1_HphaseFine(7 DOWNTO 0) <= Reg_bb1_din(7 DOWNTO 0);

					ELSIF load_sync = "0110" THEN
						Reg_bb1_HphaseFine(7 DOWNTO 0) <= Reg_bb1_din(7 DOWNTO 0);

					ELSIF load_sync = "0111" THEN
						Reg_bb1_HphaseFine(7 DOWNTO 0) <= Reg_bb1_din(7 DOWNTO 0);

					ELSIF load_sync = "1000" THEN
						Reg_bb1_Timercode2_9(7 DOWNTO 0) <= Reg_bb1_din(7 DOWNTO 0);

					ELSIF load_sync = "1001" THEN
						Reg_bb1_Timercode12_19(7 DOWNTO 0) <= Reg_bb1_din(7 DOWNTO 0);

					ELSIF load_sync = "1010" THEN
						Reg_bb1_Timercode22_29(7 DOWNTO 0) <= Reg_bb1_din(7 DOWNTO 0);

					ELSIF load_sync = "1011" THEN
						Reg_bb1_Timercode32_39(7 DOWNTO 0) <= Reg_bb1_din(7 DOWNTO 0);

					ELSIF load_sync = "1100" THEN
						Reg_bb1_Timercode42_49(7 DOWNTO 0) <= Reg_bb1_din(7 DOWNTO 0);

					ELSIF load_sync = "1101" THEN
						Reg_bb1_Timercode52_59(7 DOWNTO 0) <= Reg_bb1_din(7 DOWNTO 0);

					ELSIF load_sync = "1110" THEN
						Reg_bb1_Timercode62_69(7 DOWNTO 0) <= Reg_bb1_din(7 DOWNTO 0);

					ELSIF load_sync = "1111" THEN
						Reg_bb1_Timecode72_79(7 DOWNTO 0) <= Reg_bb1_din(7 DOWNTO 0);

					END IF;
--			END CASE;
		END IF;
	END PROCESS;
--	PROCESS (rd)
--	BEGIN
--		IF (rd'EVENT AND rd = '0') THEN
--			IF load_sync = "0101" THEN
--				Reg_bb1_info(7 DOWNTO 0) <= Reg_bb1_din(7 DOWNTO 0);
--			ELSIF load_sync = "0111" THEN
--				Reg_bb1_info(7 DOWNTO 0) <= Reg_bb1_din(7 DOWNTO 0);
--			END IF;
--	END PROCESS;

END DATA2controller_if;

