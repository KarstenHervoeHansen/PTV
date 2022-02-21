library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity Tri_level_timer is
   Port ( 
        tsg_reset :         in std_logic;       -- start generator puls - aktiv lav
        f148 :              in std_logic;       -- multiplexed 148 MHz clock
        scan_method :       in std_logic;       -- ( 1=interlaced, 0=progressiv )
        spl_div :           in std_logic_vector(9 downto 0);  -- total samples pr. line divideret med 10 (prog) eller 11 (interlaced)
        no_of_pulses :      in std_logic;       -- antal pulser pr linie
        pulse_type :        in std_logic;       -- pulsetype 1 eller 2
        f148_div :          inout std_logic;    -- f148 divideret med 10 eller 11
        spldiv_downcount :  inout std_logic_vector(9 downto 0);
        last_count :        out std_logic;  -- indikerer samples ved linieskift
        line_clk :          out std_logic;  -- 
        sync :              out std_logic;  -- styresignal til konverter
        sync_t :            out std_logic   -- styresignal til konverter
        --debug :
--        linebegin :         out std_logic;
  --      linemid :           out std_logic;
    --    spldivcnt_active :  out std_logic;
      --  divsig :            out integer range 0 to 15;
        --clksync :           out std_logic;
--        smclk :             out std_logic
        );
end Tri_level_timer;


architecture Behavioral of Tri_level_timer is

component sync_statemachine
    Port (  
        tsg_reset :         in std_logic;
        f148_div :          in std_logic;
        spldiv_downcount :  in std_logic_vector(9 downto 0);
        line_begin :        in std_logic;
        line_mid :          in std_logic;
        pulse_type :        in std_logic;
        no_of_pulses :      in std_logic;
        sync :              out std_logic;  -- styresignal til konverteren
        sync_t :            out std_logic  -- styresignal til konverteren
        -- debug :
--        clksync :           out std_logic;
  --      smclk :             out std_logic
        );
end component;

signal div :        integer range 0 to 15;
signal line_begin : std_logic;  -- hjaelpesignal 3
signal line_mid :   std_logic;  -- hjaelpesignal 4

begin

-- debug :
--linebegin <= line_begin;
--linemid <= line_mid;
--spldivcnt_active <= spldiv_downcount(0);
--divsig <= div;


statemachine : sync_statemachine port map ( 
    tsg_reset           => tsg_reset,
    f148_div            => f148_div,
    spldiv_downcount    => spldiv_downcount,
    line_begin          => line_begin,
    line_mid            => line_mid,
    pulse_type          => pulse_type,
    no_of_pulses        => no_of_pulses,
    sync                => sync,
    sync_t              => sync_t
    -- debug :
--    clksync             => clksync,
  --  smclk               => smclk
    );


make_f148_div : process( tsg_reset, f148, scan_method )
-- taeller 10 eller 11 samples ad gangen, da logikken til hjaelpesignalerne 
-- no_of_pulses, Pulse_type og sync_statemaskinen
-- bliver reduceret, ved at benytte dette clocksignal.
-- ( initialiseringen af div, bestemmer, om der skal deles med 10 eller 11 samples )
begin
    if tsg_reset = '0' then
        f148_div <= '0';
        if scan_method = '1' then 
            div <= 0; -- interlaced scan
        else
            div <= 1; -- progressiv scan
        end if;
    elsif f148'event and f148 = '0' then 
        if (div /= 10) then
            div <= div + 1;
            if div = 5 then
                f148_div <= '1';
            end if;
        else
            if scan_method = '1' then 
                div <= 0; -- interlaced scan
            else
                div <= 1; -- progressiv scan
            end if;
            f148_div <= '0';
        end if;
    end if;
end process;


make_downcount : process(tsg_reset, f148_div, spl_div) 
-- spldiv_downcount er en taeller, der taeller samples x 10 (prog) eller samples x 11 (interlaced)
-- taelleren taeller ned fra spl_div, da logikken til line_begin hermed bliver meget simpel
begin
    if tsg_reset = '0' then
        spldiv_downcount <= spl_div;
        line_clk <= '0';
    elsif f148_div'event and f148_div = '0' then
        if spldiv_downcount /= "0000000000" then
            spldiv_downcount <= spldiv_downcount - 1;
            if spldiv_downcount = "0011011000" then
                line_clk <= '1';
            end if;
        else
            spldiv_downcount <= spl_div;
            line_clk <= '0';
        end if;
    end if;
end process;


make_last_count : process( tsg_reset, f148_div, spldiv_downcount )
-- last_count er hoej omkring liniesktift.
-- benyttes til genlocktiming

------------------------------------------------------------+
-- NB : lav genlockwindow ved at and'e div paa plast_count  |
--      og flyt koden til genlock_timing modulet            |
------------------------------------------------------------+

begin
    if tsg_reset = '0' then
        last_count <= '0';
    elsif f148_div'event and f148_div = '1' then
        case spldiv_downcount is
--            when "0000000001" =>
  --              last_count <= '1';
            when "0000000000" =>
                last_count <= '1';
            when others =>
                last_count <= '0';
        end case;
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
        if spldiv_downcount = "0000010000" then
            line_begin <= '1';
        else
            line_begin <= '0';
        end if;

        -- line_mid :
        -- 1920x1080/60/I, 1920x1080/59.94/I, 1920x1035/60/I og 1920x1035/59.94/I :
        if spl_div(4) = '0' and spldiv_downcount = "0011011000" then
            line_mid <= '1';
        -- 1920x1080/50/I :
        elsif spl_div(4) = '1' and spldiv_downcount = "0100000000" then
            line_mid <= '1';
        else
            line_mid <= '0';
        end if;

        -- line_mid ( old ) :
--        if spldiv_downcount = "0011011000" then
  --          line_mid <= '1';
    --    else
      --      line_mid <= '0';
        --end if;

    end if;
end process;


end Behavioral;
