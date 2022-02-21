-- Navn			: BB1_MAKE_ST_SYNC_UP_DOWN
-- Dato			: 99-06-
-- Rev.nr.		: 0.1
-- Funktion		: Makes the start signals for the sync and signals with the
--				  same amplitude (e.g. fieldpulses).
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_MAKE_ST_SYNC_UP_DOWN IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_vx						:	IN	STD_LOGIC;
		pga1_bb1_ux						:	IN	STD_LOGIC;
		pga1_bb1_sx						:	IN	STD_LOGIC;
		pga1_bb1_xv						:	IN	STD_LOGIC;
		pga1_bb1_xu						:	IN	STD_LOGIC;
		pga1_bb1_G_M					:	IN	STD_LOGIC;
		pga1_bb1_H						:	IN	INTEGER RANGE 0 TO 2047;

		pga1_bb1_start_up			:	OUT	STD_LOGIC;
		pga1_bb1_start_down			:	OUT	STD_LOGIC
		); 
END BB1_MAKE_ST_SYNC_UP_DOWN;

ARCHITECTURE START_SYNC_UP_DOWN OF BB1_MAKE_ST_SYNC_UP_DOWN IS
	TYPE STATE_TYPE IS (s0_start_sync_up_down);
	SIGNAL state: STATE_TYPE;
BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_start_sync_up_down =>
--H10_0 = 0 ~ 0,000us
--H10_0 = 858 ~ 31,778us
--H10_0 = 864 ~ 32,000us

					IF ((pga1_bb1_vx = '1' and pga1_bb1_H = 0)
						or (pga1_bb1_ux = '1' and pga1_bb1_H = 0)
						or (pga1_bb1_sx = '1' and pga1_bb1_H = 0)
						or (pga1_bb1_xv = '1' and pga1_bb1_H = 864 and
							pga1_bb1_G_M = '1')
						or (pga1_bb1_xv = '1' and pga1_bb1_H = 858 and
							pga1_bb1_G_M = '0')
						or (pga1_bb1_xu = '1' and pga1_bb1_H = 864 and
							pga1_bb1_G_M = '1')
						or (pga1_bb1_xu = '1' and pga1_bb1_H = 858 and
							pga1_bb1_G_M = '0'))
					THEN
						pga1_bb1_start_down <= '1';
					ELSE
						pga1_bb1_start_down <= '0';
					END IF;
--H10_0 = 63 ~ 2,350us
--H10_0 = 127 ~ 4,700us
--H10_0 = 731 ~ 27,030us
--H10_0 = 737 ~ 27,300us
--H10_0 = 921 ~ 34,128us
--H10_0 = 927 ~ 34,350us
--H10_0 = 1589 ~ 58,856us
--H10_0 = 1601 ~ 59,296us

					IF ((pga1_bb1_vx = '1' and pga1_bb1_H = 737 and
							pga1_bb1_G_M = '1')
						or (pga1_bb1_vx = '1' and pga1_bb1_H = 731 and
							pga1_bb1_G_M = '0')
						or (pga1_bb1_ux = '1' and pga1_bb1_H = 63)
						or (pga1_bb1_sx = '1' and pga1_bb1_H = 127)
						or (pga1_bb1_xv = '1' and pga1_bb1_H = 1601 and
							pga1_bb1_G_M = '1')
						or (pga1_bb1_xv = '1' and pga1_bb1_H = 1589 and
							pga1_bb1_G_M = '0')
						or (pga1_bb1_xu = '1'  and pga1_bb1_H = 927 and
							pga1_bb1_G_M = '1')
						or (pga1_bb1_xu = '1' and pga1_bb1_H = 921 and
							pga1_bb1_G_M = '0'))
					THEN
						pga1_bb1_start_up <= '1';
					ELSE
						pga1_bb1_start_up <= '0';
					END IF;
					state <= s0_start_sync_up_down;
			END CASE;
		END IF;
	END PROCESS;

END START_SYNC_UP_DOWN;





