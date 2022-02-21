-- Navn			: BB1_PLAY_BURST
-- Dato			: 99-05-
-- Rev.nr.		: 0.1
-- Funktion		: Setup burst-adr and Writes data from the burst-ram to the dac
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_PLAY_BURST IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_start_burst			:	IN	BIT;
--		pga1_bb1_burst_ram_data			:	IN	STD_LOGIC_VECTOR(8 DOWNTO 0); 
-- Data input er kun til test
		pga1_bb1_load_burst				:	OUT	BIT;
		pga1_bb1_burst_ram_adr			:	OUT	INTEGER RANGE 0 TO 127
		); 
END BB1_PLAY_BURST;

ARCHITECTURE BURST_DATA2DAC OF BB1_PLAY_BURST IS
	TYPE STATE_TYPE IS (s0_init_burst_adr,s1_get_burst_data);
	SIGNAL state: STATE_TYPE;
	SIGNAL adr_count : Integer RANGE 0 TO 127;
BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_init_burst_adr =>

					IF pga1_bb1_start_burst = '1' THEN
						state <= s1_get_burst_data;
						adr_count <= 0;
						pga1_bb1_load_burst <= '1';
					ELSE
						state <= s0_init_burst_adr;
						adr_count <= 0;
						pga1_bb1_load_burst <= '0';
					END IF;


				WHEN s1_get_burst_data =>

					IF pga1_bb1_start_burst = '1' THEN
						state <= s0_init_burst_adr;
						pga1_bb1_load_burst <= '0';
					ELSE
						state <= s1_get_burst_data;
						pga1_bb1_load_burst <= '1';
						adr_count <= adr_count + 1;
					END IF;

			END CASE;
		END IF;
	END PROCESS;
	pga1_bb1_burst_ram_adr <= adr_count;

END BURST_DATA2DAC;



