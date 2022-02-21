library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity trilevelgenerator_3 is
    Port( 
        tsg_reset :         in std_logic;
        f148_div :          in std_logic;
        no_of_pulses :      out std_logic;
        pulse_type :        out std_logic;
        line_clk :          in std_logic;
        last_lineframe :    out std_logic;
        sync_mode_lsb :     in std_logic;
        lines_pr_frame :    in integer range 0 to 2047;
        scan_method :       in std_logic
        );
end trilevelgenerator_3;

architecture Behavioral of trilevelgenerator_3 is

signal line :           integer range 0 to 2047;
signal half_line :      std_logic;
signal new_frame :      std_logic;
signal frame :          std_logic_vector(1 downto 0);
signal line_cond :      std_logic;
signal temp_cond :      std_logic;
signal frame_cond :     std_logic;


begin

make_line_counter : process(tsg_reset,line_clk)
begin
    if tsg_reset = '0' then
        line <= 0;
        new_frame <= '1';
    elsif line_clk'event and line_clk = '0' then
        if line /= lines_pr_frame then
		    line <= line + 1;
            if line = 512 then
                new_frame <= '0';
            end if;
        else
		    line <= 0;
            new_frame <= '1';
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


make_no_of_pulses : process(tsg_reset, f148_div) -- hjaelpesignal 1: fortaeller om der er 1 eller 2 pulser/linje
begin
    if tsg_reset = '0' then
        no_of_pulses <= '1';
    elsif f148_div'event and f148_div = '0' then
        if scan_method = '1' then        
            -- interlaced scan :
            -- to pulser pr linie v. frame- og field sync
            case line is
                when 0 =>
                    no_of_pulses <= '1'; -- 2 pulser/linje
                when 6 =>
                    no_of_pulses <= '0'; -- 1 puls/linje
                when 562 =>
                        no_of_pulses <= '1'; -- 2 pulser/linje
                when 568 =>
                        no_of_pulses <= '0'; -- 1 puls/linje
                when others =>
                    null;
            end case;
        else                             -- progressiv scan :
            no_of_pulses <= '0';         -- kun een puls pr. linie
        end if;                          -- paa alle linier
    end if;
end process;


make_Pulse_type : process(tsg_reset, f148_div) 
-- hjaelpesignal 2: fortaeller hvilken pulstype der skal laves
begin
    if tsg_reset = '0' then
        Pulse_type <= '0';
    elsif f148_div'event and f148_div = '0' then
        case line is
            when 0 =>
                Pulse_type <= '0';    -- skifter til pulstype 1 paa foerste linie
            when 5 =>
                Pulse_type <= '1';    -- skifter til pulstype 2 paa femte linie
            when 562 =>
                -- field sync v. interlaced scanning :
                if scan_method = '1' and line_clk = '1' then
                    Pulse_type <= '0';
                end if;
            when 567 =>
                if scan_method = '1' and line_clk = '1' then
                    Pulse_type <= '1';
                end if;
            when others =>
                null;
        end case;            
    end if;
end process;


----------------------------------------------------------------------------------------
-- make last_lineframe :

-- frame_cond skal gaa hoej naar sync_mode(0)=0 (internsync=f4m) OG frame = 1 og 3
-- og naar sync_mode(0)=1 (internsync=f8g), kun hoej naar frame = 3 :
frame_cond <= (not frame(1) and frame(0) and not sync_mode_lsb) or (frame(1) and frame(0));

-- line_cond = 1 ved sidste linie
line_cond  <=   '1' when ( line = lines_pr_frame ) else
                '0';

last_lineframe <= line_cond and frame_cond;


end Behavioral;
