-- Navn			: BB12_FLASH_BURST_RAM_ON
-- Dato			: 99-06-
-- Rev.nr.		: 0.1
-- Funktion		: Makes the loadpulse for the coarse Hphase counter.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;

ENTITY BB12_FLASH_BURST_RAM_ON IS
	PORT(
		clk							:	IN	STD_LOGIC;
		pga1_bb12_start_ramload		:	IN	STD_LOGIC;
		pga1_bb12_stopload			:	OUT	STD_LOGIC
		); 
END BB12_FLASH_BURST_RAM_ON;

ARCHITECTURE FLASH_BURST_RAM_ON OF BB12_FLASH_BURST_RAM_ON IS
	TYPE STATE_TYPE IS (s0_burst_ramload,s1_burst_ramload_count,s2_burst_ramload_test);
	SIGNAL state: STATE_TYPE;
	SIGNAL burst_count : INTEGER RANGE 0 TO 162; 

BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_burst_ramload =>

					IF (pga1_bb12_start_ramload = '1') THEN
						burst_count <= 0;
						pga1_bb12_stopload <= '0';
						state <= s1_burst_ramload_count;
					ELSE
						pga1_bb12_stopload <= '1';
						state <= s0_burst_ramload;
					END IF;

				WHEN s1_burst_ramload_count =>
					burst_count <= burst_count + 1;
					state <= s2_burst_ramload_test;

				WHEN s2_burst_ramload_test =>
					IF burst_count = 162 THEN
						pga1_bb12_stopload <= '1';
						state <= s0_burst_ramload;
					ELSE 
						state <= s1_burst_ramload_count;
					END IF;
			END CASE;
		END IF;
	END PROCESS;

END FLASH_BURST_RAM_ON;

