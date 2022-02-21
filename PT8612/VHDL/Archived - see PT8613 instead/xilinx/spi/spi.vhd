--///**********************************************************************
--// File:    spi.vhd
--//***********************************************************************
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;

entity spi_test is
   Port
      (
      --//------- LED's --------------------------------------------------------------------------
      clk_i         : in  std_logic;      

      led1_o        : out std_logic;
      led2_o        : out std_logic;
      led3_o        : out std_logic;
      led4_o        : out std_logic
      );
end spi_test;


architecture Behavioral of spi_test is

signal count    : std_logic_vector(15 downto 0);


begin

led1_o <= count(12);
led2_o <= count(13);
led3_o <= count(14);
led4_o <= count(15);

counting : process( clk_i)
begin
if clk_i'event and clk_i = '1' then
   count <= count + 1;
end if;
end process;



end Behavioral;