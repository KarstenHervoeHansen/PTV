library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity Tri_level_timer is
   Port (
        mreset :            in std_logic;       -- reset til sync_statemachine
        tsg_reset :         in std_logic;       -- start generator puls - aktiv lav
        f148 :              in std_logic;       -- multiplexed 148 MHz clock
        scan_method :       in std_logic;       -- ( 1=interlaced, 0=progressiv )
        div10_11 :          in std_logic;       -- divider f148 med 10 hvis lav, 11 hvis høj
        sync_mode_lsb :     in std_logic;
        spl_div :           in std_logic_vector(9 downto 0);  -- total samples pr. line divideret med 10 (prog) eller 11 (interlaced)
        lines_pr_frame :    in integer range 0 to 2047;
        framerate_x2 :      in std_logic;
        tsg_lvl :           in std_logic;
        tsg_ok :            out std_logic;
        last_count :        out std_logic;  -- indikerer samples ved linieskift
        last_lineframe :    out std_logic;
        tsg_out :           out std_logic_vector(3 downto 0);
        --debug :
        divclk :            out std_logic;
        spldivdowncount :   out integer range 0 to 1023;
        lineclock   :       out std_logic;
        linie :             out integer range 0 to 2047;
        frameclock :        out std_logic;
        frame_db :          out integer range 0 to 3;
        linebegin :         out std_logic;
        linemid :           out std_logic;
        noofpulses :        out std_logic;
        pulsetype :         out std_logic;
        spldivcnt_active :  out std_logic;
        divsig :            out integer range 0 to 15;
        clksync :           out std_logic;
        smclk :             out std_logic;
        lvlsample_db :      inout std_logic;
        statecounter :      out integer range 0 to 63;
        state_db :          out integer range 0 to 7;
        f74_db :            out std_logic;
        sync_sim :          out std_logic;
        sync_t_sim :        out std_logic
        );
end Tri_level_timer;


architecture Behavioral of Tri_level_timer is


component sync_statemachine
    Port (  
        mreset :            in std_logic;
        tsg_reset :         in std_logic;
        f148 :              in std_logic;
        f148_div :          in std_logic;
        div10_11 :          in std_logic;
        spldiv_downcount :  in std_logic_vector(9 downto 0);
        line_begin :        in std_logic;
        line_mid :          in std_logic;
        pulse_type :        in std_logic;
        no_of_pulses :      in std_logic;
        tsg_lvl :           in std_logic;
        tsg_ok :            out std_logic;
        tsg_out :           out std_logic_vector(3 downto 0);
        -- debug :
        statecounter :      out integer range 0 to 63;
        state_db :          out integer range 0 to 7;
        clksync :           out std_logic;
        smclk :             out std_logic;
        lvlsample_db :      inout std_logic;
        f74_db :            out std_logic;
        sync_sim :          out std_logic;
        sync_t_sim :        out std_logic
        );
end component;


signal no_of_pulses :       std_logic;
signal pulse_type :         std_logic;
signal div :                integer range 0 to 15;
signal divstart_value :     integer range 0 to 15;
signal f148_div :           std_logic;
signal spldiv_downcount :   std_logic_vector(9 downto 0);
signal line_clock :         std_logic;
signal line :               integer range 0 to 2047;
signal frame_clock :        std_logic;
signal frame :              std_logic_vector(1 downto 0);
signal line_begin :         std_logic;  -- hjaelpesignal 3
signal line_mid :           std_logic;  -- hjaelpesignal 4
signal line_cond :          std_logic;
signal frame_cond :         std_logic;


begin


--------------------------------------------------
-- debug :
linebegin <= line_begin;
linemid <= line_mid;
lineclock <= line_clock;
noofpulses <= no_of_pulses;
pulsetype <= Pulse_type;
spldivcnt_active <= spldiv_downcount(0);
divsig <= div;
divclk <= f148_div;
spldivdowncount <= conv_integer(spldiv_downcount);
linie <= line;
frameclock <= frame_clock;
frame_db <= conv_integer(frame);
--------------------------------------------------



statemachine : sync_statemachine port map ( 
    mreset              => mreset,
    tsg_reset           => tsg_reset,
    f148                => f148,
    f148_div            => f148_div,
    div10_11            => div10_11,
    spldiv_downcount    => spldiv_downcount,
    line_begin          => line_begin,
    line_mid            => line_mid,
    pulse_type          => pulse_type,
    no_of_pulses        => no_of_pulses,
    tsg_lvl             => tsg_lvl,
    tsg_ok              => tsg_ok,
    tsg_out             => tsg_out,
    -- debug :
    statecounter        => statecounter,
    state_db            => state_db,
    clksync             => clksync,
    smclk               => smclk,
    lvlsample_db        => lvlsample_db,
    f74_db              => f74_db,
    sync_sim            => sync_sim,
    sync_t_sim          => sync_t_sim
    );


make_divstart_value : with div10_11 select
    divstart_value <= 0 when '1',
                      1 when others;

make_f148_div : process( tsg_reset, f148, div10_11, divstart_value )
-- taeller 10 eller 11 samples ad gangen, da logikken til hjaelpesignalerne 
-- no_of_pulses, Pulse_type og sync_statemaskinen
-- bliver reduceret, ved at benytte dette clocksignal.
-- ( initialiseringen af div, bestemmer, om der skal deles med 10 eller 11 samples )
-- ( 10 samples for progressive formater og 11 samples for interlaced formater )
begin
    if tsg_reset = '0' then
        f148_div <= '0';
        div <= divstart_value;
    elsif f148'event and f148 = '0' then 
        case div is
            when 5 =>
                div <= div + 1;
                f148_div <= '1';
            when 10 =>
                div <= divstart_value;
                f148_div <= '0';
            when others =>
                div <= div + 1;
        end case;
    end if;
end process;


make_downcount : process(tsg_reset, f148_div, spl_div) 
-- spldiv_downcount er en taeller, der taeller samples x 10 (prog) eller samples x 11 (interlaced)
-- taelleren taeller ned fra spl_div, da logikken til line_begin hermed bliver meget simpel
begin
    if tsg_reset = '0' then
        spldiv_downcount <= spl_div;
        line_clock <= '0';
    elsif f148_div'event and f148_div = '0' then
        case spldiv_downcount is
            when "0011011010" =>
                if framerate_x2 = '1' then
                    line_clock <= '1';
                else
                    null;
                end if;
                spldiv_downcount <= spldiv_downcount - 1;
            when "0010000000" =>
                line_clock <= '1';              -- line_clk high for dubbel framerate formater
                spldiv_downcount <= spldiv_downcount - 1;
            when "0000000000" =>
                spldiv_downcount <= spl_div;
                line_clock <= '0';
            when others =>
                spldiv_downcount <= spldiv_downcount - 1;
        end case;
    end if;
end process;


make_line_counter : process( tsg_reset, line_clock )
begin
    if tsg_reset = '0' then
        line <= 0;
        frame_clock <= '1';
    elsif line_clock'event and line_clock = '0' then
        if line /= lines_pr_frame then
            line <= line + 1;
            if line = 512 then
                frame_clock <= '0';
            end if;
        else
		    line <= 0;
            frame_clock <= '1';
		end if;
    end if;
end process;


frame_counter : process( tsg_reset, frame_clock )
begin
    if tsg_reset = '0' then
        frame <= "00";
    elsif frame_clock'event and frame_clock = '1' then
        if frame /= "11" then
            frame <= frame + 1;
        else
            frame <= "00";
        end if;
    end if;
end process;


make_line_begin_and_line_mid : process(tsg_reset, f148_div, spldiv_downcount) 
-- line_begin = hjaelpesignal 3: identificerer tidspunktet, hvor foerste puls startes (sker paa alle linjer)
-- line_mid = hjaelpesignal 4: identificerer tidspunktet, hvor midterste puls paa en linje evt. skal startes
begin
    if tsg_reset = '0' then
        line_begin <= '0';
        line_mid <= '0';
    elsif f148_div'event and f148_div = '1' then

        -- line_begin :
        if spldiv_downcount = "0000001010" then
            line_begin <= '1';
        else
            line_begin <= '0';
        end if;

        -- line_mid :
        -- 1920x1080/60/I, 1920x1080/59.94/I, 1920x1035/60/I og 1920x1035/59.94/I :
        if spl_div(4) = '0' and spldiv_downcount = "0011010010" then
            line_mid <= '1';
        -- 1920x1080/50/I :
        elsif spl_div(4) = '1' and spldiv_downcount = "0011111010" then
            line_mid <= '1';
        else
            line_mid <= '0';
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
                if scan_method = '1' and line_clock = '1' then
                    Pulse_type <= '0';
                end if;
            when 567 =>
                if scan_method = '1' and line_clock = '1' then
                    Pulse_type <= '1';
                end if;
            when others =>
                null;
        end case;            
    end if;
end process;


make_last_count : process( tsg_reset, f148_div, spldiv_downcount )
-- last_count er hoej omkring liniesktift.
-- ( benyttes i genlocktiming komponent )
begin
    if tsg_reset = '0' then
        last_count <= '0';
    elsif f148_div'event and f148_div = '1' then
        case spldiv_downcount is
            when "0000000000" =>
                last_count <= '1';
            when others =>
                last_count <= '0';
        end case;
    end if;
end process;


make_line_cond : process( tsg_reset, line_clock, line )
-- line_cond = i slutningen af sidste linie og starten af foerste
-- benyttes til genlocktiming
begin
    if tsg_reset = '0' then
        line_cond <= '0';
    elsif line_clock'event and line_clock = '1' then
        if line = lines_pr_frame then
            line_cond <= '1';
        else
            line_cond <= '0';
        end if;
    end if;
end process;


make_frame_cond : process ( tsg_reset, frame_clock, frame, sync_mode_lsb )
-- frame_cond skal gaa hoej naar frame = 1 og 3 intern_sync = f4m ( sync_mode(0) = 0 )
-- og kun hoej naar frame = 3 OG intern_sync = f8g
-- benyttes til genlocktiming
begin
    if tsg_reset = '0' then
        frame_cond <= '0';
    elsif frame_clock'event and frame_clock = '0' then
        case frame is
            when "01" =>
                frame_cond <= sync_mode_lsb;    -- 1 hvis internsync = f4_m
            when "11" =>
                frame_cond <= '1';
            when others =>
                frame_cond <= '0';
        end case;
    end if;
end process;


-- benyttes til genlocktiming :
last_lineframe <= line_cond and frame_cond;


end Behavioral;