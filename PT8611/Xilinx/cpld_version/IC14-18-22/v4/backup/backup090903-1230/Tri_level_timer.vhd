library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-----------------------------------------------------------------------------------------------
-- v1 :     Original kode til Prototype lavet af studerende Jesper Christoffersen 
--          for DK-Audio, som afgangsprojekt november 2002 til marts 2003
-- v2:      19/6 2003 - PS      tilfoejet clamping- og enablesignaler til rekonstruktionsfiltre
-- v3:      23/5 2003 - PS      nyt hieraki => toplevel = Tri_level_sync_generator.vhd
-----------------------------------------------------------------------------------------------

entity Tri_level_timer is
   Port ( 
        tsg_reset :         in std_logic;       -- start generator puls - aktiv lav
        f148 :              in std_logic;       -- Output fra clock multiplier ( 148 MHz )
        scan_method :       in std_logic;                 -- ( 0=interlaced, 1=progressiv )
        samples_pr_line :   in integer range 0 to 8191;                  
        lines_pr_frame :    in integer range 0 to 2047;                  
            
        sample :            inout integer range 0 to 8191;
        line :              inout integer range 0 to 2047;
        frame_freq :        inout std_logic;

        sync :              out std_logic; -- styresignal til konverteren
        sync_t :            out std_logic  -- styresignal til konverteren

        );
end Tri_level_timer;


architecture Behavioral of Tri_level_timer is

component sync_statemachine
    Port (  
        tsg_reset :     in std_logic;
        f148_div :      in std_logic;

        line_begin :    in std_logic;
        line_mid :      in std_logic;
        pulse_type :    in std_logic;
        no_of_pulses :  in std_logic;
    
        sync :          out std_logic;  -- styresignal til konverteren
        sync_t :        out std_logic   -- styresignal til konverteren
        );
end component;

signal f148_div :           std_logic;
signal last_sample :        std_logic;
signal new_line :           std_logic; -- hoejtgaaende flanke signalerer ny linie
signal div :                integer range 0 to 127;          
signal no_of_pulses :       std_logic;            -- hjaelpesignal 1
signal pulse_type :         std_logic;            -- hjaelpesignal 2
signal line_begin :         std_logic;            -- hjaelpesignal 3
signal line_mid :           std_logic;            -- hjaelpesignal 4

begin

statemachine : sync_statemachine port map ( 
    tsg_reset           => tsg_reset,
    f148_div            => f148_div,
    line_begin          => line_begin,
    line_mid            => line_mid,
    pulse_type          => pulse_type,
    no_of_pulses        => no_of_pulses,
    sync                => sync,
    sync_t              => sync_t
    );

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
        frame_freq <= '1';
    elsif new_line'event and new_line = '1' then
        if line /= lines_pr_frame then
		    line <= line + 1;
            if (line >= 256) and (line <= 511) then
                frame_freq <= '0';
            end if;
        else
		    line <= 0;
            frame_freq <= '1';       -- indikerer ny frame
		end if;
	end if;
end process;    


make_f148_div : process(tsg_reset,f148) 
-- taeller 80 eller 88 samples ad gangen, da logikken til hjaelpesignalerne 
-- no_of_pulses, Pulse_type og sync_statemaskinen
-- bliver reduceret kraftigt, ved at benytte dette clocksignal
begin
    if tsg_reset = '0' then
        last_sample <= '0';
    elsif f148'event and f148 = '1' then 
        if sample = ( samples_pr_line - 1 ) then   -- sikrer at 'sample' og 'f148_div' er synkrone
            last_sample <= '1';
        else
            last_sample <= '0';
        end if;
    end if;

    if tsg_reset = '0' then
        div <= 0;
        f148_div <= '0';
    elsif f148'event and f148 = '1' then 
        if scan_method = '0' then -- ( scan_method = interlaced )
            if (div /= 87) and (last_sample = '0') then
                div <= div + 1;
                if div = 43 then
                    f148_div <= '1';
                end if;
            else
                div <= 0;
                f148_div <= '0';
            end if;
        else -- ( scan_method = progressiv )
            if (div /= 79) and (last_sample = '0') then
                div <= div + 1;
                if div = 39 then
                    f148_div <= '1';
                end if;
            else
                div <= 0;
                f148_div <= '0';
            end if;
        end if;
    end if;
end process;
      

make_no_of_pulses : process(tsg_reset, f148_div) -- hjaelpesignal 1: fortaeller om der er 1 eller 2 pulser/linje
begin
    if tsg_reset = '0' then
        no_of_pulses <= '1';
    elsif f148_div'event and f148_div = '0' then
        if scan_method = '0' then          -- ( interlaced scan )

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

        else                             -- ( progressiv scan )
            no_of_pulses <= '0';         -- ( kun een puls pr. linie )
        end if;
    end if;
end process;


make_Pulse_type : process(tsg_reset, f148_div) -- hjaelpesignal 2: fortaeller hvilken pulstype der skal laves
begin
    if tsg_reset = '0' then
        Pulse_type <= '0';
    elsif f148_div'event and f148_div = '0' then

        if line = 0 then
            Pulse_type <= '0';    -- skifter til pulstype 1 paa foerste linie
        elsif line = 5 then
            Pulse_type <= '1';    -- skifter til pulstype 2 paa femte linie
        elsif line = 562 and scan_method = '0' then
            Pulse_type <= '0';
        elsif line = 567 and scan_method = '0' then
            Pulse_type <= '1';

        end if;            
    end if;
end process;


make_line_begin : process(tsg_reset, f148) -- hjaelpesignal 3: identificerer tidspunktet, hvor foerste puls startes (sker paa alle linjer)
begin
    if tsg_reset = '0' then
        line_begin <= '0';
    elsif f148'event and f148 = '1' then

        if ( scan_method = '0' ) and ( sample = ( samples_pr_line - 181 )) then
            line_begin <= '1';
        elsif ( scan_method = '0' ) and ( sample = ( samples_pr_line - 170 )) then
            line_begin <= '0';
        elsif ( scan_method = '1' ) and ( sample = ( samples_pr_line - 166 )) then
            line_begin <= '1';
        elsif ( scan_method = '1' ) and ( sample = ( samples_pr_line - 155 )) then
            line_begin <= '0';
        end if;

    end if;
end process;


make_line_mid : process(tsg_reset, f148) -- hjaelpesignal 4: identificerer tidspunktet, hvor midterste puls paa en linje evt. skal startes
begin
    if tsg_reset = '0' then
        line_mid <= '0';
    elsif f148'event and f148 = '1' then
        if scan_method = '0' then

            case sample is
                when 2018 =>
                    line_mid <= '1';
                when 2029 =>
                    line_mid <= '0';
                when others =>
                    null;
            end case;

        end if;
    end if;
end process;

end Behavioral;
