-- Navn			: BB12_LOAD_FLASH_2_RAM
-- Dato			: 99-06-
-- Rev.nr.		: 0.1
-- Funktion		: Makes the loadpulse for the coarse Hphase counter.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;

ENTITY BB12_LOAD_FLASH_2_RAM IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb12_ramload_bb1			:	IN	STD_LOGIC;

		pga1_bb12_WE_bb1				:	OUT	STD_LOGIC; --active low
		tristate_bb1_flash_2_ram		:	OUT	STD_LOGIC;
		pga1_bb12_burst_sampleadr_bb1	:	OUT	INTEGER RANGE 0 TO 127;
		pga1_bb12_burst_ramadr_bb1		:	OUT	INTEGER RANGE 0 TO 127


		); 
END BB12_LOAD_FLASH_2_RAM;

ARCHITECTURE LOAD_FLASH_2_RAM OF BB12_LOAD_FLASH_2_RAM IS
	TYPE STATE_TYPE IS (init,s0_burst_ramload_init,s1_burst_ramload_count,s2_burst_ramload_delay,
s3_burst_ramload_delay,s4_burst_ramload_test);
	SIGNAL state: STATE_TYPE;
	SIGNAL flashburst_samplecount : INTEGER RANGE 0 TO 81;
	SIGNAL ramadr_count : INTEGER RANGE 0 TO 127;

BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN init =>
						state <= s0_burst_ramload_init;

				WHEN s0_burst_ramload_init =>

					IF pga1_bb12_ramload_bb1 = '1' THEN
						tristate_bb1_flash_2_ram <= '1';
						state <= s0_burst_ramload_init;

					ELSE
						tristate_bb1_flash_2_ram <= '0';
						state <= s2_burst_ramload_delay;
					END IF;
						ramadr_count <= 127;
						pga1_bb12_WE_bb1 <= '1';
						flashburst_samplecount <= 0;

				WHEN s1_burst_ramload_count =>
					flashburst_samplecount <= flashburst_samplecount +1;
					pga1_bb12_WE_bb1 <= '0';
					state <= s2_burst_ramload_delay;

				WHEN s2_burst_ramload_delay =>
					pga1_bb12_WE_bb1 <= '1';
					state <= s3_burst_ramload_delay;

				WHEN s3_burst_ramload_delay =>
					pga1_bb12_WE_bb1 <= '0';
					state <= s4_burst_ramload_test;

				WHEN s4_burst_ramload_test =>
					IF flashburst_samplecount = 81 THEN
						state <= s0_burst_ramload_init;
					ELSE 
						state <= s1_burst_ramload_count;
					END IF;
						ramadr_count <= ramadr_count + 1;

			END CASE;
		END IF;
	END PROCESS;

pga1_bb12_burst_sampleadr_bb1 <= flashburst_samplecount;
pga1_bb12_burst_ramadr_bb1 <= ramadr_count;

END LOAD_FLASH_2_RAM;

