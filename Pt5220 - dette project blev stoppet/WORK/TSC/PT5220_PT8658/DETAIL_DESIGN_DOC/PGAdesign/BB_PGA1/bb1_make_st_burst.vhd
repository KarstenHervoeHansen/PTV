-- Navn			: BB1_MAKE_ST_BURST
-- Dato			: 99-06-
-- Rev.nr.		: 0.1
-- Funktion		: Makes the loadpulse for the coarse Hphase counter.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_MAKE_ST_BURST IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_line_type				:	IN	STD_LOGIC_VECTOR(3 downto 0);
		pga1_bb1_H						:	IN	INTEGER RANGE 0 TO 2047;
		pga1_bb1_G_M					:	IN	STD_LOGIC;

		pga1_bb1_start_burst			:	OUT	STD_LOGIC
		); 
END BB1_MAKE_ST_BURST;

ARCHITECTURE START_BURST OF BB1_MAKE_ST_BURST IS
	TYPE STATE_TYPE IS (s0_start_burst);
	SIGNAL state: STATE_TYPE;
BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_start_burst =>

					IF (pga1_bb1_line_type(3) = '1') and (pga1_bb1_line_type(1) = '1')
						and (pga1_bb1_H = 151) and (pga1_bb1_G_M ='1')
					THEN
						pga1_bb1_start_burst <= '1';
					ELSIF (pga1_bb1_line_type(3) = '1' and pga1_bb1_line_type(1) = '1'
						and pga1_bb1_H = 143 and pga1_bb1_G_M ='0')
					THEN
						pga1_bb1_start_burst <= '1';
					ELSE
						pga1_bb1_start_burst <= '0';
					END IF;
					state <= s0_start_burst;
			END CASE;
		END IF;
	END PROCESS;

END START_BURST;





