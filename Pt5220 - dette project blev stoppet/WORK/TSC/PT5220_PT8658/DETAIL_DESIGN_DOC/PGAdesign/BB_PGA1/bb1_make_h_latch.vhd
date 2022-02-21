-- Navn			: BB1_MAKE_H_LATCH
-- Dato			: 99-06-
-- Rev.nr.		: 0.1
-- Funktion		: Makes the Hlatch pulse for the coarse Hphase counter.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_MAKE_H_LATCH IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_G_M					:	IN	STD_LOGIC;
		pga1_bb1_Hcoarse				:	IN	INTEGER RANGE 0 TO 2047;

		pga1_bb1_fieldlatch				:	OUT	BIT;
		pga1_bb1_linelatch				:	OUT	BIT

		); 
END BB1_MAKE_H_LATCH;

ARCHITECTURE BURST_DATA2H_LATCH OF BB1_MAKE_H_LATCH IS
	TYPE STATE_TYPE IS (s0_make_H_loadpulse);
	SIGNAL state: STATE_TYPE;
BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_make_H_loadpulse =>

					IF (pga1_bb1_Hcoarse = 1727 and pga1_bb1_G_M = '1')
						or (pga1_bb1_Hcoarse = 1715 and pga1_bb1_G_M ='0')
					THEN
						pga1_bb1_linelatch <= '1';
					ELSE
						pga1_bb1_linelatch <= '0';
					END IF;

					IF (pga1_bb1_Hcoarse = 1724 and pga1_bb1_G_M = '1')
						or (pga1_bb1_Hcoarse = 1712 and pga1_bb1_G_M ='0')
					THEN
						pga1_bb1_fieldlatch <= '1';
					ELSE
						pga1_bb1_fieldlatch <= '0';
					END IF;

			END CASE;
		END IF;
	END PROCESS;

END BURST_DATA2H_LATCH;





