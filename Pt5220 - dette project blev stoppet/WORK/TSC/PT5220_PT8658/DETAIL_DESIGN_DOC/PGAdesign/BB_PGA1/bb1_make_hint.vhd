-- Navn			: BB1_MAKE_HINT
-- Dato			: 99-06-
-- Rev.nr.		: 0.1
-- Funktion		: Makes the loadpulse for the coarse Hphase counter.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_MAKE_HINT IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_H						:	IN	INTEGER RANGE 0 TO 2047;

		pga1_bb1_make_hint				:	INOUT	STD_LOGIC
		);
END BB1_MAKE_HINT;

ARCHITECTURE HINT OF BB1_MAKE_HINT IS
	TYPE STATE_TYPE IS (s0_make_hint_pulse);
	SIGNAL state: STATE_TYPE;
BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_make_hint_pulse =>

--Inputs	|	Output
--	CLK	T	|	Q
--	X	X	|	H
--	X	X	|	L
--	X	X	|	Illegal
--	^	L	|	Qo*
--	^	H	|	Toggle
--	L	X	|	Qo*

					IF (pga1_bb1_H = 0 and pga1_bb1_make_hint = '1')
						or
						(pga1_bb1_H = 38 and pga1_bb1_make_hint = '0')

					THEN
						pga1_bb1_make_hint <= not(pga1_bb1_make_hint);
					ELSE
						pga1_bb1_make_hint <= (pga1_bb1_make_hint);
					END IF;
					state <= s0_make_hint_pulse;
			END CASE;
		END IF;
	END PROCESS;

END HINT;

