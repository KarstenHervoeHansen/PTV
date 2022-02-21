library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--------------------------------------------------------------------------
-- v1 :     Original kode til Prototype lavet af studerende Jesper Christoffersen 
--          for DK-Audio, som afgangsprojekt november 2002 til marts 2003
-- v2:      19/6 2003 - PS      tilfoejet clamping- og enablesignaler til rekonstruktionsfiltre
-- v3:      23/5 2003 - PS      nyt hieraki => toplevel = Tri_level_sync_generator.vhd
--------------------------------------------------------------------------

entity Tri_level_timer is
   Port ( 
        tsg_start :         in std_logic;       -- start generator puls - aktiv lav
        f148 :              in std_logic;       -- Output fra clock multiplier ( 148 MHz )
        f148_div :          inout std_logic;    -- f148 divideret med 10 (progressiv) eller 11 (interlaced)

        scan_method :       in std_logic;                 -- ( 0=interlaced, 1=progressiv )
        cnt_setup :         in integer range 0 to 1023;
        samples_pr_line :   in integer range 0 to 8191;                  
        lines_pr_frame :    in integer range 0 to 2047;                  
            
        sample :            inout integer range 0 to 8191;
        line :              inout integer range 0 to 2047;
        line_freq :         out std_logic;                  -- line clock  (til test formaal)
        frame_freq :        out std_logic;

        sync :              out std_logic; -- styresignal til konverteren
        sync_t :            out std_logic; -- styresignal til konverteren
        blank :             out std_logic; -- styresignal til konverteren (altid nul)

--------- rekonstruktionsfiltre -------------------------------------------

        sync_in_1 :         out std_logic;      -- aktiverer clamping paa filter output
        sync_in_2 :         out std_logic;      -- aktiverer clamping paa filter output
        disable_1 :         inout std_logic;    -- filter 1 disable ( aktiv hoej )
        disable_2 :         out std_logic       -- filter 2 disable ( aktiv hoej )

        );
end Tri_level_timer;


architecture Behavioral of Tri_level_timer is

component Tilstandsmaskine
    Port (  tsg_start :     in std_logic;
            f148 :          in std_logic;
            command :       in integer range 0 to 2;

            scan_method :   in std_logic;   -- ( 0=interlaced, 1=progressiv )
            cnt_setup :     in integer range 0 to 1023; -- signal til 

            sync :          out std_logic;  -- styresignal til konverteren
            sync_t :        out std_logic;  -- styresignal til konverteren
            blank :         out std_logic;  -- styresignal til konverteren

            state :         out integer range 0 to 15 -- for debug only

         );
end component;


signal command :            integer range 0 to 2;           -- starter statemaskinen og bestemmer pulstype
signal last_sample :        std_logic;
signal new_line :           std_logic               := '0'; -- hoejtgaaende flanke signalerer ny linie
signal div :                integer range 0 to 15;          
signal no_of_pulses :       std_logic;            -- hjaelpesignal 1
signal pulse_type :         integer range 1 to 2; -- hjaelpesignal 2
signal line_begin_pulse :   std_logic;            -- hjaelpesignal 3
signal line_mid_pulse :     std_logic;            -- hjaelpesignal 4

signal filter_1_valgt :     std_logic := '1'; -- lav for outputfilter 1 aktiv, hoej for outputfilter 2

signal state :              integer range 0 to 15;          -- for debug

begin

statemachine : Tilstandsmaskine port map ( 
    tsg_start   => tsg_start,
    f148        => f148,
    command     => command,
    sync        => sync,
    sync_t      => sync_t,
    blank       => blank,
    scan_method => scan_method,
    cnt_setup   => cnt_setup, 
    state       => state -- for debug only
    );

line_freq <= new_line;          -- line clock til testpin line_freq

--Sync_In_1 <= not no_of_pulses;  -- Ved field sync og frame sync clampes filtrenes udgange for
--Sync_In_2 <= not no_of_pulses;  -- at modvirke DC paa Syncsignalet
                                -- Denne metode er kun brugbar ved interlaced video, da der
                                -- ved progressiv scanning kun er een tri-level-puls pr linie
                                -- Sync_in signalet kan derfor ikke foelge no_of_pulses

Sync_In_1 <= '0';  -- debug
Sync_In_2 <= '0';  -- debug


--vaelg_filter :  Disable_1 <= filter_1_valgt;
--                Disable_2 <= not Disable_1;
vaelg_filter :  Disable_1 <= '0';   -- debug
                Disable_2 <= '0';   -- debug


make_sample_counter : process(tsg_start,f148)
begin
--    if tsg_start = '0' then
--        sample <= 0;          
--        new_line     <= '1'; -- det er dette signal der driver linjetaelleren
--    elsif (f148'event and f148 = '1') then
    if tsg_start = '0' then
        sample <= 0;
        new_line <= '1';
    elsif f148'event and f148 = '1' then
		if sample /= samples_pr_line then
            sample <= (sample + 1);
            if (sample >= 1024) and (sample <= 2047) then
--            if (sample >= 4) and (sample <= 7) then -- debug
		       new_line <= '0'; -- signalet new_line saettes lav naar sample overstiger vaerdien 1024
            end if;
		else  
		    new_line <= '1'; -- signalet new_line saettes hoejt naar sample har vaerdien samples_pr_line
		    sample <= 0; -- taelleren starter forfra
		end if;
	end if;		
end process;

make_line_counter : process(tsg_start,new_line)
begin
    if tsg_start = '0' then
        line <= 0;
        frame_freq <= '1';
    elsif new_line'event and new_line = '1' then
        if line /= lines_pr_frame then
		    line <= line + 1;
            if (line >= 256) and (line <= 511) then
--            if (line >= 4) and (line <= 7) then -- debug
                frame_freq <= '0';
            end if;
        else
		    line <= 0;
            frame_freq <= '1';       -- indikerer ny frame
		end if;
	end if;
end process;    

make_f148_div : process(tsg_start,f148) 
-- taeller 10 eller 11 samples ad gangen, da logikken til hjaelpesignalerne no_of_pulses, Pulse_type
-- og command bliver reduceret kraftigt, ved at benytte dette neddelte clocksignal
begin
    if tsg_start = '0' then
        last_sample <= '0';
    elsif f148'event and f148 = '1' then 
--       if sample = 4398 then   -- sikrer at 'sample' og 'f148_div' er synkrone ( original )
        if sample = ( samples_pr_line - 1 ) then   -- sikrer at 'sample' og 'f148_div' er synkrone
            last_sample <= '1';
        else
            last_sample <= '0';
        end if;
    end if;

    if tsg_start = '0' then
        div <= 0;
        f148_div <= '0';
    elsif f148'event and f148 = '1' then 
        if scan_method = '0' then
            if (div /= 10) and (last_sample = '0') then
                div <= div + 1;
                if div = 5 then
                    f148_div <= '1';
                end if;
            else
                div <= 0;
                f148_div <= '0';
            end if;
        else -- ( scan_method = 1 )
            if (div /= 9) and (last_sample = '0') then
                div <= div + 1;
                if div = 5 then
                    f148_div <= '1';
                end if;
            else
                div <= 0;
                f148_div <= '0';
            end if;
        end if;
    end if;
end process;
      

make_state_command : process(tsg_start, f148_div) -- starter statemaskinen
begin
    if tsg_start = '0' then
        command <= 0;
    elsif f148_div'event and f148_div = '0' then
        if line_begin_pulse = '1' then -- laver synkpuls i starten af en linje
            command <= Pulse_type;
        elsif (line_mid_pulse = '1') and (no_of_pulses = '1') then
            command <= Pulse_type; -- laver synkpuls midt paa en linje (field synk)
        else
            command <= 0;
        end if;
    end if;
end process;


make_no_of_pulses : process(tsg_start, f148_div) -- hjaelpesignal 1: fortaeller om der er 1 eller 2 pulser/linje
begin
    if tsg_start = '0' then
        no_of_pulses <= '1';
    elsif f148_div'event and f148_div = '0' then
        if scan_method = '0' then          -- ( interlaced scan )
            case line is
                when 0 =>
                    no_of_pulses <= '1'; -- 2 pulser/linje
                when 6 =>
                    no_of_pulses <= '0'; -- 1 puls/linje
                when 562 =>
--                when 22 => -- debug
                    no_of_pulses <= '1'; -- 2 pulser/linje
                when 568 =>
--                when 29 => -- debug
                    no_of_pulses <= '0'; -- 1 puls/linje
                when others =>
                    null;
            end case;
        else                             -- ( progressiv scan )
            no_of_pulses <= '0';         -- ( ved progressiv scanning er der kun een puls pr. linie )
        end if;
    end if;
end process;


-- fejl i original kode mht, paa hvilke linier, der skal laves hvilke pulsetyper
-- => ny make_Pulse_type
make_Pulse_type : process(tsg_start, f148_div) -- hjaelpesignal 2: fortaeller hvilken pulstype der skal laves
begin
    if tsg_start = '0' then
        Pulse_type <= 1;
    elsif f148_div'event and f148_div = '0' then
        if line = ( lines_pr_frame - 1 ) and ( sample >= 2048 ) and ( sample <= 4095 ) then
           Pulse_type <= 1;    -- skifter til pulstype 1 paa foerste linie
        elsif line = 4 and ( sample >= 2048 ) and ( sample <= 4095 ) then
            Pulse_type <= 2;    -- skifter til pulstype 2 paa femte linie
        elsif line = 562 and scan_method = '0' then
            Pulse_type <= 1;
        elsif line = 567 and scan_method = '0' then
            Pulse_type <= 2;

-- debug :
--        if line = ( lines_pr_frame - 1 ) and ( sample >= 32 ) then 
  --          Pulse_type <= 1;    -- skifter til pulstype 1 paa foerste linie
    --    elsif line = 4 and ( sample >= 32 ) then
      --      Pulse_type <= 2;    -- skifter til pulstype 2 paa femte linie
        --elsif line = 22 and scan_method = '0' then
          --  Pulse_type <= 1;
--        elsif line = 28 and scan_method = '0' then
  --          Pulse_type <= 2;

        end if;            
    end if;
end process;


make_line_begin_pulse : process(tsg_start, f148) -- hjaelpesignal 3: identificerer tidspunktet, hvor foerste puls startes (sker paa alle linjer)
begin
    if tsg_start = '0' then
        line_begin_pulse <= '0';
    elsif f148'event and f148 = '1' then
        -- NB: tjek op paa sample conditions (progressiv/interlaced)
        if ( scan_method = '0' ) and ( sample = ( samples_pr_line - 176 )) then
            line_begin_pulse <= '1';
        elsif ( scan_method = '0' ) and ( sample = ( samples_pr_line - 154 )) then
            line_begin_pulse <= '0';
        elsif ( scan_method = '1' ) and ( sample = ( samples_pr_line - 161 )) then
            line_begin_pulse <= '1';
        elsif ( scan_method = '1' ) and ( sample = ( samples_pr_line - 141 )) then
            line_begin_pulse <= '0';
        end if;

-- debug :
--        if ( scan_method = '0' ) and ( sample = ( samples_pr_line - 6 )) then
  --          line_begin_pulse <= '1';
    --    elsif ( scan_method = '0' ) and ( sample = samples_pr_line ) then
      --      line_begin_pulse <= '0';
        --elsif ( scan_method = '1' ) and ( sample = ( samples_pr_line - 6 )) then
          --  line_begin_pulse <= '1';
--        elsif ( scan_method = '1' ) and ( sample = samples_pr_line ) then
  --          line_begin_pulse <= '0';
    --    end if;
    end if;
end process;


make_line_mid_pulse : process(tsg_start, f148) -- hjaelpesignal 4: identificerer tidspunktet, hvor midterste puls paa en linje evt. skal startes
begin
    if tsg_start = '0' then
        line_mid_pulse <= '0';
    elsif f148'event and f148 = '1' then
        if scan_method = '0' then
            case sample is
                when 2023 => -- 2*(g-2*a)-1 = 2*(1100-88)-1 = 2023
                    line_mid_pulse <= '1';
                when 2045 => -- signalet holdes i 11 clock cycles pga. den neddelte clock i "make_state_command" processen
                    line_mid_pulse <= '0';
                when others =>
                    null;

--                when 21 => -- 2*(g-2*a)-1 = 2*(1100-88)-1 = 2023
  --                  line_mid_pulse <= '1';
    --            when 32 => -- signalet holdes i 11 clock cycles pga. den neddelte clock i "make_state_command" processen
      --              line_mid_pulse <= '0';
        --        when others =>
          --          null;
            end case;
        end if;
    end if;
end process;

end Behavioral;
