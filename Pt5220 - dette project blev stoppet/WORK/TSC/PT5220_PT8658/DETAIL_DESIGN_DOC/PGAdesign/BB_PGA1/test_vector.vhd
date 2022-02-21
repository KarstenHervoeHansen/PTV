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




ENTITY test_vector IS
	PORT(
		clk	:	IN	STD_LOGIC;

--		bvi	:	INOUT	INTEGER RANGE 0 to 15
		bvi	:	IN	UNSIGNED(3 DOWNTO 0);
		bvo	:	OUT INTEGER
		); 
END test_vector;

ARCHITECTURE testvec OF test_vector IS
BEGIN
	PROCESS (clk)
--VARIABLE a : bit_vector(3 DOWNTO 0);
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
		bvo <= CONV_INTEGER (bvi(1 DOWNTO 0) + 1);

		END IF;

	END PROCESS;
END testvec;

