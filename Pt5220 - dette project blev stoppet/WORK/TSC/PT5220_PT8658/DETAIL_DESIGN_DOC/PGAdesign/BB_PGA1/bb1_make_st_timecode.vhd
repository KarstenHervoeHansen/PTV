-- Navn			: BB1_MAKE_ST_TIMECODE
-- Dato			: 99-06-
-- Rev.nr.		: 0.1
-- Funktion		: Makes the startpulse for the timecode module.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_MAKE_ST_TIMECODE IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_H						:	IN	INTEGER RANGE 0 TO 2047;
		pga1_bb1_tc_on_off				:	IN	STD_LOGIC;
		pga1_bb1_G_M					:	IN	STD_LOGIC;

		pga1_bb1_end_timecode			:	OUT	STD_LOGIC;
		pga1_bb1_start_timecode			:	OUT	STD_LOGIC

		); 
END BB1_MAKE_ST_TIMECODE;

ARCHITECTURE START_TIMECODE OF BB1_MAKE_ST_TIMECODE IS
	TYPE STATE_TYPE IS (s0_start_timecode);
	SIGNAL state: STATE_TYPE;
BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_start_timecode =>
-- H10_0 = 303 ~ 11,20us
-- H10_0 = 270 ~ 10us
-- tc_on_off is active in line 10 and 12 in each field.
					IF ((pga1_bb1_tc_on_off = '1' and pga1_bb1_G_M = '1'
						and pga1_bb1_H = 303 )
						or (pga1_bb1_tc_on_off = '1' and pga1_bb1_G_M = '0'
						and pga1_bb1_H = 270 ))
					THEN
						pga1_bb1_start_timecode <= '1';
					ELSIF ((pga1_bb1_tc_on_off = '1' and pga1_bb1_G_M = '1'
						and pga1_bb1_H = 1341+303 )
						or (pga1_bb1_tc_on_off = '1' and pga1_bb1_G_M = '0'
						and pga1_bb1_H = 1358+270 ))
					THEN
						pga1_bb1_end_timecode <= '1';
					ELSE
						pga1_bb1_end_timecode <= '0';
						pga1_bb1_start_timecode <= '0';
					END IF;

					state <= s0_start_timecode;
			END CASE;
		END IF;
	END PROCESS;

END START_TIMECODE;





