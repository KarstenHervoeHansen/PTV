-- Navn			: BB1_LEVEL_IN
-- Dato			: 99-05-
-- Rev.nr.		: 0.1
-- Funktion		: Controller writes data to the PGA and receives level.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_LEVEL_IN IS
	PORT(
		clk									:	IN  STD_LOGIC;
		bb1_pga1_levelbb1					:	IN	STD_LOGIC;

		bb1_pga1_level0						:	OUT	STD_LOGIC
		);
END BB1_LEVEL_IN;

ARCHITECTURE LEVEL_IN OF BB1_LEVEL_IN IS

BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			bb1_pga1_level0 <= bb1_pga1_levelbb1;
		END IF;
	END PROCESS;
END LEVEL_IN;
