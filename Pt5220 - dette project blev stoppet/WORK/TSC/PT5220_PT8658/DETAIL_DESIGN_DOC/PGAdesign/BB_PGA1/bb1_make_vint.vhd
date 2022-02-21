-- Navn			: BB1_MAKE_VINT
-- Dato			: 99-06-
-- Rev.nr.		: 0.1
-- Funktion		: Makes the loadpulse for the coarse Hphase counter.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_MAKE_VINT IS
	PORT(
		clk								:	IN	STD_LOGIC;
--		pga1_bb1_H						:	IN	INTEGER RANGE 0 TO 2047;
		pga1_f8_g						:	IN	STD_LOGIC;
		pga1_f4_m						:	IN	STD_LOGIC;
		pga1_G_M						:	IN	STD_LOGIC;
		pga1_bb1_fieldlatch				:	IN	STD_LOGIC;


		pga1_bb1_make_vint				:	INOUT	STD_LOGIC
		);
END BB1_MAKE_VINT;

ARCHITECTURE VINT OF BB1_MAKE_VINT IS
	TYPE STATE_TYPE IS (s0_make_vint_pulse);
	SIGNAL state: STATE_TYPE;
BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_make_vint_pulse =>
					IF (pga1_bb1_fieldlatch = '1' AND pga1_f8_g = '1' AND pga1_G_M = '1') 
						OR ((pga1_bb1_fieldlatch = '1') AND (pga1_f4_m = '1') AND (pga1_G_M = '0'))
						OR (pga1_bb1_fieldlatch = '0' AND pga1_bb1_make_vint = '1')
					THEN
						pga1_bb1_make_vint <= '1';
					ELSE
						pga1_bb1_make_vint <= '0';
					END IF;
					state <= s0_make_vint_pulse;
			END CASE;
		END IF;
	END PROCESS;

END VINT;

