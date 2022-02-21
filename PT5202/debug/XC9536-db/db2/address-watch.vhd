library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity addresswatch is
    Port (
       add :        in      std_logic_vector(17 downto 0);
       psen :       in      std_logic;
       reset :      in      std_logic;
       flag3 :      out     std_logic;
       flag2 :      out     std_logic;
       flag1 :      out     std_logic;
       flag0 :      out     std_logic
       );
			
end addresswatch;

architecture behavioral of addresswatch is

type poss_states is (first,second,third);
signal state : poss_states;

begin

statemach : process(psen,reset)
begin
    if reset = '0' then
        state <= first;
    elsif psen'event and psen = '1' then
        case state is
            when first =>
                if add(15 downto 0) = "0000011011110011" then
                    state <= second;
                else
                    state <= first;
                end if;
            when second =>
                if add(15 downto 0) = "0000011011110111" then
                    state <= third;
                else
                    state <= first;
                end if;
            when others =>
                state <= first;
        end case;
     end if;                   
end process;

with state select
    -- flag0 signalerer waiting for T-11
    flag0 <= '0' when first,
    '1' when others;

with state select
    -- flag1 signalerer waiting for T-10
    flag1 <= '0' when second,
    '1' when others;

with state select
    -- flag2 signalerer at sekvensen T-11 og T-10 har efterfulgt hinanden
    flag2 <= '0' when third,
    '1' when others;

setflag3 : process(psen,add)
begin
    if psen'event and psen = '1' then
        if add(15 downto 0) = "1111111111111111" then
            flag3 <= '0';
        else
            flag3 <= '1';
        end if;
    end if;
end process;

end behavioral;
