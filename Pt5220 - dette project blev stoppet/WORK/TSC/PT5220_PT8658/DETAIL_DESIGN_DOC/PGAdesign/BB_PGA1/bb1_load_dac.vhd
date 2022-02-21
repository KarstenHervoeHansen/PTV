-- Navn			: BB1_LOAD_DAC
-- Dato			: 99-05-
-- Rev.nr.		: 0.1
-- Funktion		: Write data to the dac
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_LOAD_DAC IS
	PORT(
		clk								:	IN	STD_LOGIC;
		load_sync						:	IN	BIT;
		load_burst						:	IN	BIT;
		load_timecode					:	IN	BIT;
		load_video						:	IN	BIT;
		load_pedistal					:	IN	BIT;
		burst_reg						:	IN	STD_LOGIC_VECTOR(8 DOWNTO 0); 
		sync_timecode_reg				:	IN	STD_LOGIC_VECTOR(7 DOWNTO 0); 
		video_reg						:	IN	STD_LOGIC_VECTOR(8 DOWNTO 0); 
--		black_reg						:	IN	STD_LOGIC_VECTOR(7 DOWNTO 0); 
		pedistal_reg					:	IN	STD_LOGIC_VECTOR(3 DOWNTO 0);


		Dacdataout						:	OUT	STD_LOGIC_VECTOR(11 DOWNTO 0)
		); 
END BB1_LOAD_DAC;

ARCHITECTURE DATA2DAC OF BB1_LOAD_DAC IS
	TYPE STATE_TYPE IS (s0_load_dac);
	SIGNAL state: STATE_TYPE;
BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_load_dac =>

					IF load_sync = '1' THEN
				    	Dacdataout(11 DOWNTO 8) <= "0000";
				    	Dacdataout(7 DOWNTO 0) <=  sync_timecode_reg(7 DOWNTO 0) ;


					ELSIF load_burst = '1' THEN
				    	Dacdataout(11 DOWNTO 10) <= "00";
    					Dacdataout(9 DOWNTO 1) <= burst_reg(8 DOWNTO 0) ;
				    	Dacdataout(0) <= '0';

					ELSIF load_timecode = '1' THEN
				    	Dacdataout(11) <= '0';
	   					Dacdataout(10 DOWNTO 3) <= sync_timecode_reg(7 DOWNTO 0) ;
				    	Dacdataout(2 DOWNTO 0) <= "000";

					ELSIF load_video = '1' THEN
    					Dacdataout(11 DOWNTO 3) <= video_reg(8 DOWNTO 0) ;
				    	Dacdataout(2 DOWNTO 0) <= "000";

--					ELSIF load_black = '1' THEN
--				    	Dacdataout(11 DOWNTO 8) <= "0000";
--   					Dacdataout(7 DOWNTO 0) <= black_reg(7 DOWNTO 0) ;

					ELSIF load_pedistal = '1' THEN
				    	Dacdataout(11 DOWNTO 4) <= "00001111"; -- load 300mV
    					Dacdataout(3 DOWNTO 0) <= pedistal_reg(3 DOWNTO 0) ;
					ELSE
				    	Dacdataout(11 DOWNTO 0) <= "010101010101"; -- load 300mV
					END IF;

--    			WHEN OTHERS => state <= s0_load_dac;

			END CASE;
		END IF;
	END PROCESS;

END DATA2DAC;

