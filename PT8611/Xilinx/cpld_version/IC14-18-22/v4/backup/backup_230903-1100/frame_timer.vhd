library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity frame_timer is
    Port( 
        tsg_reset :         in std_logic;       -- start generator puls - aktiv lav
        f148 :              in std_logic;       -- Output fra clock multiplier ( 148 MHz )
        samples_pr_line :   in integer range 0 to 16383;
        lines_pr_frame :    in integer range 0 to 2047;

        sample :            inout integer range 0 to 16383;
        line :              inout integer range 0 to 2047;
        frame :             inout std_logic_vector(1 downto 0)
        );
end frame_timer;

architecture Behavioral of frame_timer is

signal new_line :           std_logic; -- hoejtgaaende flanke signalerer ny linie
signal new_frame :          std_logic; -- hoejtgaaende flanke signalerer ny linie

begin

make_sample_counter : process(tsg_reset,f148)
begin
    if tsg_reset = '0' then
--        sample <= 0;
        sample <= 0;    -- debug
        new_line <= '1';
    elsif f148'event and f148 = '1' then

		if sample /= samples_pr_line then
            sample <= (sample + 1);
            if (sample >= 1024) and (sample <= 2047) then
		       new_line <= '0';
            end if;
		else  
		    new_line <= '1';
		    sample <= 0;
		end if;

	end if;		
end process;

make_line_counter : process(tsg_reset,new_line)
begin
    if tsg_reset = '0' then
        line <= 0;
        new_frame <= '1';
    elsif new_line'event and new_line = '1' then
        if line /= lines_pr_frame then
--            if line = 9 then
  --              line <= 561;
    --            new_frame <= '0';
      --      elsif line = 568 then
        --        line <= 1122;
          --  else
            --    line <= line + 1;
--            end if;
		    line <= line + 1;
            if (line >= 256) and (line <= 511) then
                new_frame <= '0';
            end if;
        else
		    line <= 0;
            new_frame <= '1';       -- indikerer ny frame til genlock komponent
		end if;
	end if;
end process;    

frame_counter : process( tsg_reset, new_frame )
begin
    if tsg_reset = '0' then
        frame <= "00";
    elsif new_frame'event and new_frame = '1' then
        if frame /= "11" then
            frame <= frame + 1;
        else
            frame <= "00";
        end if;
    end if;
end process;


end Behavioral;
