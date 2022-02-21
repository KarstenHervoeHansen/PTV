-- Navn			: BB12_MAKE_BURSTSTOP
-- Dato			: 99-06-
-- Rev.nr.		: 0.1
-- Funktion		: Makes a stoppulse when the burst has reach the end.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB12_MAKE_BURSTSTOP IS
	PORT(
		clk							:	IN	STD_LOGIC;
		pga1_bb12_start				:	IN	STD_LOGIC;
		pga1_bb12_stopload			:	IN	STD_LOGIC;

		pga1_bb12_burststop			:	OUT	STD_LOGIC
		); 
END BB12_MAKE_BURSTSTOP;

ARCHITECTURE BURSTSTOP OF BB12_MAKE_BURSTSTOP IS
	TYPE STATE_TYPE IS (s0_burststop,s1_burststop_count,s2_burststop_test);
	SIGNAL state: STATE_TYPE;
	SIGNAL burst_count : INTEGER RANGE 0 TO 41; 

BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_burststop =>

					IF (pga1_bb12_start = '1') THEN
						burst_count <= 0;
						state <= s1_burststop_count;
					ELSE
						state <= s0_burststop;
					END IF;
						pga1_bb12_burststop <= '0';

				WHEN s1_burststop_count =>
					burst_count <= burst_count + 1;
					state <= s2_burststop_test;

				WHEN s2_burststop_test =>
					IF pga1_bb12_stopload = '0' THEN
						state <= s0_burststop;
					ELSIF burst_count = 41 THEN
						pga1_bb12_burststop <= '1';
						state <= s2_burststop_test;
					ELSE 
						state <= s1_burststop_count;
					END IF;
			END CASE;
		END IF;
	END PROCESS;

END BURSTSTOP;





