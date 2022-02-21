-- Navn			: SHIFT_REG_45
-- Dato			: 99-05-
-- Rev.nr.		: 0.1
-- Funktion		: Default 45bit shift register with aclr and clk.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

ENTITY SHIFT_REG_45 IS 
PORT
(
	clk			: IN bit; 
	d_in		: IN bit;
	aclr		: IN bit;
	d_out		: OUT bit
); 
END SHIFT_REG_45; 

ARCHITECTURE SR OF SHIFT_REG_45 IS 

SIGNAL reg : bit_vector (44 DOWNTO 0) := "000000000000000000000000000000000000000000000"; 

BEGIN
PROCESS (clk) 
BEGIN 
	IF aclr = '1' THEN
		reg(44 DOWNTO 0) <= "000000000000000000000000000000000000000000000";
	ELSIF (clk = '1') AND (clk'event) THEN 
	d_out <= reg(44); 
	FOR i IN 44 DOWNTO 1 LOOP 
		reg(i) <= reg(i-1); 
	END LOOP;
	reg(0) <= d_in; 
	END IF; 
END PROCESS; 
END SR;

