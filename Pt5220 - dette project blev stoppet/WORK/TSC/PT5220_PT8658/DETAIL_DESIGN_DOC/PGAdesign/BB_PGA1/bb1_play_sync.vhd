-- Navn			: BB1_PLAY_SYNC
-- Dato			: 99-05-
-- Rev.nr.		: 0.1
-- Funktion		: Setup sync-adr and Writes data from the sync-ram to the dac
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_PLAY_SYNC IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_start_syncdown			:	IN	BIT;
		pga1_bb1_start_syncup			:	IN	BIT;
--		pga1_bb1_sync_ram_data			:	IN	STD_LOGIC_VECTOR(3 DOWNTO 0); 
-- Data input er kun til test
		pga1_bb1_load_sync				:	OUT	BIT;
		pga1_bb1_sync_ram_adr			:	OUT	INTEGER RANGE 0 TO 15
		); 
END BB1_PLAY_SYNC;

ARCHITECTURE SYNC_DATA2DAC OF BB1_PLAY_SYNC IS
	TYPE STATE_TYPE IS (s0_init_sync_adr,s1_get_sync_u_data,s2_get_sync_d_data);
	SIGNAL state: STATE_TYPE;
	SIGNAL adr_count : Integer RANGE 0 TO 15;
BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_init_sync_adr =>

					IF pga1_bb1_start_syncup = '1' THEN
						pga1_bb1_load_sync <= '1';
						state <= s1_get_sync_u_data;
						adr_count <= 0; -- data op
					ELSIF pga1_bb1_start_syncdown = '1' THEN
						pga1_bb1_load_sync <= '1';
						state <= s2_get_sync_d_data;
						adr_count <= 9; -- data ned
					ELSE
						pga1_bb1_load_sync <= '0';
						state <= s0_init_sync_adr;
					END IF;


				WHEN s1_get_sync_u_data =>

					IF pga1_bb1_start_syncup = '0' THEN
						state <= s1_get_sync_u_data;
						pga1_bb1_load_sync <= '1';
					ELSE
						state <= s0_init_sync_adr;
						pga1_bb1_load_sync <= '0';
					END IF;
						adr_count <= adr_count + 1;

				WHEN s2_get_sync_d_data =>

					IF pga1_bb1_start_syncdown = '0' THEN
						state <= s2_get_sync_d_data;
						pga1_bb1_load_sync <= '1';
					ELSE
						state <= s0_init_sync_adr;
						pga1_bb1_load_sync <= '0';
					END IF;
						adr_count <= adr_count - 1;

				WHEN OTHERS =>
						state <= s0_init_sync_adr;

			END CASE;
		END IF;
	END PROCESS;
	pga1_bb1_sync_ram_adr <= adr_count;

END SYNC_DATA2DAC;



