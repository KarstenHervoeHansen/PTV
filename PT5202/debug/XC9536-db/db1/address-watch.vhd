library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity addresswatch is
    Port (
       add :        in      std_logic_vector(17 downto 0);
       psen :       in      std_logic;
       flag2 :      out     std_logic;
       flag1 :      out     std_logic;
       flag0 :      out     std_logic
       );
			
end addresswatch;

architecture behavioral of addresswatch is

begin

setflags : process(psen,add)
begin
  if psen'event and psen = '1' then
    flag2 <= '1';
    flag1 <= '1';
    flag0 <= '1';
    case add(15 downto 0) is
        when "1111111111111111" =>
            flag2 <= '0';
        when "0000011011110111" =>
            flag1 <= '0';
        when "0000000111011111" =>
            flag0 <= '0';
        when others =>
            null;
    end case;
  end if;
end process;

end behavioral;
