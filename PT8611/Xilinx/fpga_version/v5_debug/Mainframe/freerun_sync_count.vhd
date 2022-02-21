library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
library UNISIM;
use UNISIM.VComponents.all;

entity freerunning_sync_count is
   generic(
      sync_period : integer;
   );
   Port(
      mreset_i : in std_logic;
      clk_i    : in std_logic;
      sync_o   : out std_logic;
   );
end freerunning_sync_count;

architecture Behavioral of freerunning_sync_count is
signal sync_cnt : std_logic_vector(22 downto 0);

begin

--------------------------------------------------------------
-- Generate sync pulses with a period of sync_period
--------------------------------------------------------------
sync_counting : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                          
   sync_cnt <= (others=>'0');
   sync_o <= '0';
elsif clk_i'event and clk_i = '1' then
   if sync_cnt = conv_std_logic_vector(0,23) then
      sync_cnt <= conv_std_logic_vector(sync_period-1,23);
      sync_o <= '1';
   else
      sync_cnt = sync_cnt - 1;
      sync_o <= '0';
   end if;
end if;    
end process;

end Behavioral;