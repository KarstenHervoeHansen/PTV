library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity altddio_in is
    Port ( datain : in std_logic_vector(21 downto 0);
           inclock : in std_logic;
           dataout_h : out std_logic_vector(21 downto 0);
           dataout_l : out std_logic_vector(21 downto 0));
end altddio_in;

architecture Behavioral of altddio_in is

begin

altddio_h: process(inclock)
begin
	if inclock'event and inclock = '1' then
			dataout_h <= datain;
	end if;
end process;

altddio_l: process(inclock)
begin		
	if inclock'event and inclock = '0' then
			dataout_l <= datain;
	end if;
end process;
end Behavioral;
