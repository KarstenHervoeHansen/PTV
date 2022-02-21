library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity toplvl is
    port (
		clk :       in std_logic;   
        op1 :    	in std_logic_vector(9 downto 0);
        op2 :      	in std_logic_vector(9 downto 0);
		res10 :    out std_logic_vector(9 downto 0);
		res :    out std_logic_vector(19 downto 0)
    );
end toplvl;

architecture Behavioral of toplvl is

signal op1_new : std_logic_vector(9 downto 0);

component mult IS
	PORT
	(
		dataa		: IN STD_LOGIC_VECTOR (9 DOWNTO 0);
		datab		: IN STD_LOGIC_VECTOR (9 DOWNTO 0);
		result		: OUT STD_LOGIC_VECTOR (19 DOWNTO 0)
	);
END component;

component mult10 IS
	PORT
	(
		dataa		: IN STD_LOGIC_VECTOR (9 DOWNTO 0);
		datab		: IN STD_LOGIC_VECTOR (9 DOWNTO 0);
		result		: OUT STD_LOGIC_VECTOR (9 DOWNTO 0)
	);
END component;

begin

u1 : mult port map(
	dataa => op1_new,
	datab => op2,
	result => res
	);

u2 : mult10 port map(
	dataa => op1_new,
	datab => op2,
	result => res10
	);

process( clk ) 
begin
	if clk'event and clk = '1' then
		op1_new <= op1;
	end if;
end process;

end Behavioral;
