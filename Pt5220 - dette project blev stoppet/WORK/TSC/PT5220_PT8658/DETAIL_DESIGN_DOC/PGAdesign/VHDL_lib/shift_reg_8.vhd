entity SHIFT_REG_8 is 
port
(
	d_in		: in bit;
	aclr		: in bit;
	clk			: in bit; 
	d_out		: out bit
); 
end SHIFT_REG_8; 

architecture SR of SHIFT_REG_8 is 

signal reg : bit_vector (7 downto 0) := "00000000"; 

begin 
process (clk) 
begin 
	if aclr = '1' then
		reg(7 downto 0) <= "00000000";
	elsif (clk = '1') and (clk'event) then 
	d_out <= reg(7); 
	for i in 7 downto 1 loop 
		reg(i) <= reg(i-1); 
	end loop; 
	reg(0) <= d_in; 
	end if; 
end process; 
end SR;

