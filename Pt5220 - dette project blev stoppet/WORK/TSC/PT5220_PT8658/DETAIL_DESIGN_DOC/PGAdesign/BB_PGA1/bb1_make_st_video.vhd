-- Navn			: BB1_MAKE_ST_VIDEO
-- Dato			: 99-06-
-- Rev.nr.		: 0.1
-- Funktion		: Makes the start video signal.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_MAKE_ST_VIDEO IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_poff					:	IN	STD_LOGIC;
		pga1_bb1_G_M					:	IN	STD_LOGIC;
		pga1_bb1_H						:	IN	INTEGER RANGE 0 TO 2047;
		pga1_bb1_plin7					:	IN	STD_LOGIC;
		pga1_bb1_plin7out				:	IN	STD_LOGIC;

		pga1_bb1_start_lin7				:	OUT	STD_LOGIC
		);
END BB1_MAKE_ST_VIDEO;

ARCHITECTURE START_VIDEO OF BB1_MAKE_ST_VIDEO IS

	TYPE STATE_TYPE IS (s0_make_lin7_pulse);
	SIGNAL state: STATE_TYPE;
BEGIN
	PROCESS (clk)
--	VARIABLE outlin7: STD_LOGIC;
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_make_lin7_pulse =>

--Inputs				|	Output
--PRN	CLRN	CLK	T	|	Q
--	L	H		X	X	|	H
--	H	L		X	X	|	L
--	L	L		X	X	|	Illegal
--	H	H		^	L	|	Qo*
--	H	H		^	H	|	Toggle
--	H	H		L	X	|	Qo*

--outlin7 := (pga1_bb1_H = 335 and pga1_bb1_G_M = '1'
--						and outlin7 = '0' and pga1_bb1_plin7 ='0'
--						and pga1_bb1_poff = '1') or
--						(pga1_bb1_H = 800 and outlin7 = '1')

					IF (pga1_bb1_H = 335 and pga1_bb1_G_M = '1'
						and pga1_bb1_plin7out = '0' and pga1_bb1_plin7 ='1'
						and pga1_bb1_poff = '1') or
						(pga1_bb1_H = 800 and pga1_bb1_plin7out = '1')

					THEN
						pga1_bb1_start_lin7 <= not(pga1_bb1_plin7out);
--						outlin7 := not(pga1_bb1_plin7out);
--						state <= s1;
					ELSE
--						outlin7 := outlin7;
						pga1_bb1_start_lin7 <= (pga1_bb1_plin7out);
--					state <= s0_make_lin7_pulse;
					END IF;
					state <= s0_make_lin7_pulse;
--			WHEN s1 =>
--				outlin7 := not(pga1_bb1_plin7out);
--				state <= s0_make_lin7_pulse;
			END CASE;
		END IF;
--		pga1_bb1_start_lin7 <= outlin7;
	END PROCESS;

END START_VIDEO;

