-- Navn			: BB1_MAKE_H_COUNT
-- Dato			: 99-06-
-- Rev.nr.		: 0.1
-- Funktion		: Makes the new Hphase for the coarse Hphase counter.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_MAKE_H_COUNT IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_H_load_count			:	IN	BIT;
		pga1_bb1_Hcoarse				:	IN	INTEGER RANGE 0 TO 2047;
		pga1_bb1_linelatch				:	IN	BIT;


		pga1_bb1_H						:	OUT	INTEGER RANGE 0 TO 2047
--		pga1_bb1_H10_0					:	OUT STD_LOGIC_VECTOR(10 DOWNTO 0)
		); 
END BB1_MAKE_H_COUNT;

ARCHITECTURE BURST_DATA2H_COUNT OF BB1_MAKE_H_COUNT IS
	TYPE STATE_TYPE IS (s0_make_H_count);
	SIGNAL state: STATE_TYPE;
	SIGNAL H_count : INTEGER RANGE 0 TO 2047;
BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_make_H_count =>

					IF (pga1_bb1_H_load_count = '0' and pga1_bb1_linelatch = '0')
						THEN
							H_count <= H_count + 1;

					ELSIF 
						(pga1_bb1_H_load_count = '0' and pga1_bb1_linelatch = '1')
						THEN
							H_count <= 0;

					ELSIF (pga1_bb1_H_load_count = '1')
						THEN
							H_count <= pga1_bb1_Hcoarse;

					END IF;
			END CASE;
		END IF;
	END PROCESS;
	pga1_bb1_H <= H_count;--(10 DOWNTO 0);
END BURST_DATA2H_COUNT;





