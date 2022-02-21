library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity sample_counter is
    Port( 
        tsg_reset :         in std_logic;       -- start generator puls - aktiv lav
        f148 :              in std_logic;       -- Output fra clock multiplier ( 148 MHz )
        samples_pr_line :   in integer range 0 to 16383;
        sample :            inout integer range 0 to 16383;
        new_line :          out std_logic
        );
end sample_counter;

architecture Behavioral of sample_counter is

begin

make_sample_counter : process(tsg_reset,f148)
begin
    if tsg_reset = '0' then
        sample <= 0;
        new_line <= '1';
    elsif f148'event and f148 = '1' then

		if sample /= samples_pr_line then
            sample <= (sample + 1);
            if sample = 1024 then
                new_line <= '0';
            end if;
		else  
		    new_line <= '1';
		    sample <= 0;
		end if;

	end if;		
end process;

end Behavioral;
