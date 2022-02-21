library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity state_counter is
    Port ( 
        clk :       in std_logic;
        load :      in std_logic;
        load_val :  in std_logic_vector(10 downto 0);
        count :     inout std_logic_vector(10 downto 0)
        );
end state_counter;

architecture Behavioral of state_counter is

begin

make_count : process( clk, load )
begin
    if load = '1' then
        count <= load_val;
    elsif ( clk'event and clk = '1' ) then
        if count /= "00000000000" then
            count <= count - 1;
        else
            count <= load_val;
        end if;
    end if;
end process;


end Behavioral;
