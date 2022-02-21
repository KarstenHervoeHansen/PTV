-- Navn			: BB1_LOAD_BURST
-- Dato			: 99-06-
-- Rev.nr.		: 0.1
-- Funktion		: Write data from the flash-prom to the burst-ram
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_LOAD_BURST IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_load_burst				:	IN	BIT;
		Reg_bb1_HphaseFine				:	IN	STD_LOGIC_VECTOR(7 DOWNTO 0);
		Reg_bb1_BurstPhase				:	IN	STD_LOGIC_VECTOR(10 DOWNTO 0);
		pga1_bb1_Reg_FlashData			:	IN	INTEGER RANGE 0 TO 511;

		pga1_bb1_burst_wr				:	OUT STD_LOGIC;
		pga1_bb1_BurstDataOut			:	OUT	INTEGER RANGE 0 TO 511;
		pga1_bb1_BurstAdrOutPh			:	OUT	INTEGER RANGE 0 TO 2047; -- 8-18bit
		pga1_bb1_BurstAdrOutFine		:	OUT	INTEGER RANGE 0 TO 127; -- 0-7bit
		pga1_bb1_FlashAdrOut			:	OUT INTEGER RANGE 0 TO 127
		); 
END BB1_LOAD_BURST;

ARCHITECTURE DATA2BURST OF BB1_LOAD_BURST IS
	TYPE STATE_TYPE IS (s0_load_burst,s1_get_flash,s2_load_ram);
	SIGNAL state: STATE_TYPE;
	SIGNAL burst_data : Integer RANGE 0 TO 511;

	SIGNAL burst_adr_count : Integer RANGE 0 TO 81;
	SIGNAL flash_adr_count : Integer RANGE 0 TO 81;
BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_load_burst =>

					IF pga1_bb1_load_burst = '1' THEN
				    	burst_adr_count <= 127;
						flash_adr_count <= 127;
						state <= s0_load_burst;
					ELSE
				    	burst_adr_count <= burst_adr_count + 1;
						flash_adr_count <= flash_adr_count + 1;
						pga1_bb1_burst_wr <= '0';
						state <= s1_get_flash;
					END IF;

				WHEN s1_get_flash =>
						burst_data <= pga1_bb1_Reg_FlashData;
						pga1_bb1_burst_wr <= '1';
						state <= s2_load_ram;

				WHEN s2_load_ram =>
						pga1_bb1_BurstDataOut <= burst_data;
						pga1_bb1_burst_wr <= '1';
						state <= s0_load_burst;

    			WHEN OTHERS => state <= s0_load_burst;

			END CASE;
		END IF;
	pga1_bb1_BurstAdrOutFine <= burst_adr_count;
	pga1_bb1_BurstAdrOutPh <= Reg_bb1_BurstPhase;
	pga1_bb1_FlashAdrOut <= flash_adr_count;

	END PROCESS;

END DATA2BURST;

