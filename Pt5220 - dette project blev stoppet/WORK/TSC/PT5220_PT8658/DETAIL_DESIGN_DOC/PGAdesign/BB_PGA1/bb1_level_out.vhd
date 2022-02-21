-- Navn			: BB1_LEVEL_OUT
-- Dato			: 99-05-
-- Rev.nr.		: 0.1
-- Funktion		: Controller writes data to the PGA and receives level.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_LEVEL_OUT IS
	PORT(
---		clk									:	IN  STD_LOGIC;
---		bb1_pga1_levelbb1					:	IN	STD_LOGIC;
---		bb1_pga1_levelbb2					:	IN	STD_LOGIC;
		bb1_pga1_level0						:	IN	STD_LOGIC;
		bb1_pga1_level1						:	IN	STD_LOGIC;
		bb1_pga1_load_level					:	IN	STD_LOGIC;
		Reg_bb1_dio						:	INOUT UNSIGNED(7 DOWNTO 0)
--		Reg_bb1_dio7						:	INOUT	STD_LOGIC;
--		Reg_bb1_dio6						:	INOUT	STD_LOGIC;
--		Reg_bb1_dio5						:	INOUT	STD_LOGIC;
--		Reg_bb1_dio4						:	INOUT	STD_LOGIC;
--		Reg_bb1_dio3						:	INOUT	STD_LOGIC;
--		Reg_bb1_dio2						:	INOUT	STD_LOGIC;
--		Reg_bb1_dio1						:	INOUT	STD_LOGIC;
--		Reg_bb1_dio0						:	INOUT	STD_LOGIC

		);
END BB1_LEVEL_OUT;

ARCHITECTURE LEVEL_OUT OF BB1_LEVEL_OUT IS

BEGIN
	PROCESS
	BEGIN
		IF bb1_pga1_load_level = '1' THEN
---			IF bb1_pga1_load_level = '0' THEN
---			Reg_bb1_dio(7 DOWNTO 2) <= "000000";
---			Reg_bb1_dio(1) <= bb1_pga1_level1;
---			Reg_bb1_dio(0) <= bb1_pga1_level0;
			Reg_bb1_dio(7) <= 'Z';  -- 'Z' shall be capital letter
			Reg_bb1_dio(6) <= 'Z';
			Reg_bb1_dio(5) <= 'Z';
			Reg_bb1_dio(4) <= 'Z';
			Reg_bb1_dio(3) <= 'Z';
			Reg_bb1_dio(2) <= 'Z';
			Reg_bb1_dio(1) <= 'Z';
			Reg_bb1_dio(0) <= 'Z';
		ELSE
			Reg_bb1_dio(1) <= bb1_pga1_level1;
			Reg_bb1_dio(0) <= bb1_pga1_level0;
---				Reg_bb1_dio(7 DOWNTO 0) <= "zzzzzzzz"; 
---			END IF;
		END IF;
	END PROCESS;
END LEVEL_OUT;




