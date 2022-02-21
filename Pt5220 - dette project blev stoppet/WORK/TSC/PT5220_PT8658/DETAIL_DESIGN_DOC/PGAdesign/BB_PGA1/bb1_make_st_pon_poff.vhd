-- Navn			: BB1_MAKE_ST_PON_POFF
-- Dato			: 99-06-
-- Rev.nr.		: 0.1
-- Funktion		: Makes the loadpulse for the coarse Hphase counter.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_MAKE_ST_PON_POFF IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_poff					:	IN	STD_LOGIC;
		pga1_bb1_H						:	IN	INTEGER RANGE 0 TO 2047;
		pga1_bb1_p1						:	IN	STD_LOGIC;
		pga1_bb1_p2						:	IN	STD_LOGIC;
		pga1_bb1_p3						:	IN	STD_LOGIC;
		pga1_bb1_p4						:	IN	STD_LOGIC;
		pga1_bb1_G_M					:	IN	STD_LOGIC;

		pga1_bb1_start_pup				:	OUT	BIT;
		pga1_bb1_start_pdown			:	OUT	BIT
		); 
END BB1_MAKE_ST_PON_POFF;

ARCHITECTURE PON_OFF_START OF BB1_MAKE_ST_PON_POFF IS
	TYPE STATE_TYPE IS (s0_start_pon_poff);
	SIGNAL state: STATE_TYPE;
BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_start_pon_poff =>
-- H10_0 = 254 ~ 9,40us
-- H10_0 =1112 ~ 41,178us (Tektronik bruger 32,278us som er start af tol.)
-- som er '+H'.
					IF ((pga1_bb1_p1 = '1' and pga1_bb1_poff = '0'
						and pga1_bb1_H = 254 and pga1_bb1_G_M ='0')
						or (pga1_bb1_p3 = '1' and pga1_bb1_poff = '0'
						and pga1_bb1_H = 1112 and pga1_bb1_G_M ='0'))
					THEN
						pga1_bb1_start_pup <= '1';
					ELSE
						pga1_bb1_start_pup <= '0';
					END IF;

-- H10_0 = 818 ~ 30,278us
-- H10_0 =1112 ~ 62,056us
					IF ((pga1_bb1_p2 = '1' and pga1_bb1_poff = '0'
						and pga1_bb1_H = 818 and pga1_bb1_G_M ='0')
						or (pga1_bb1_p4 = '1' and pga1_bb1_poff = '0'
						and pga1_bb1_H = 1676 and pga1_bb1_G_M ='0'))
					THEN
						pga1_bb1_start_pdown <= '1';
					ELSE
						pga1_bb1_start_pdown <= '0';
					END IF;

					state <= s0_start_pon_poff;
			END CASE;
		END IF;
	END PROCESS;

END PON_OFF_START;





