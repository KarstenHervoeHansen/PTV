-- Navn			: BB1_MAKE_LOAD_H_COUNT
-- Dato			: 99-05-
-- Rev.nr.		: 0.1
-- Funktion		: Makes the loadpulse for the coarse Hphase counter.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_MAKE_LOAD_H_COUNT IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_FH_G					:	IN	STD_LOGIC;
		pga1_bb1_FH_M					:	IN	STD_LOGIC;
		pga1_bb1_G_M					:	IN	STD_LOGIC;

		pga1_bb1_load_H_count			:	OUT	BIT
		); 
END BB1_MAKE_LOAD_H_COUNT;

ARCHITECTURE BURST_DATA2H_COUNT OF BB1_MAKE_LOAD_H_COUNT IS
	TYPE STATE_TYPE IS (s0_make_H_loadpulse);
	SIGNAL state: STATE_TYPE;
BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_make_H_loadpulse =>

					IF (pga1_bb1_FH_G = '1' and pga1_bb1_G_M = '1')
						or (pga1_bb1_FH_M = '1' and pga1_bb1_G_M ='0')
					THEN
						pga1_bb1_load_H_count <= '1';
					ELSE
						pga1_bb1_load_H_count <= '0';
					END IF;
			END CASE;
		END IF;
	END PROCESS;

END BURST_DATA2H_COUNT;





