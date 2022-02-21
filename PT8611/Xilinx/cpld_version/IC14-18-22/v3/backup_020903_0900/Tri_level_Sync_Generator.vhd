library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-------------------------------------------------------------------------------------
-- v1:      Original kode til Prototype lavet af PEH + studerende Jesper Christoffersen 
--          for DK-Audio, som afgangsprojekt november 2002 til marts 2003
-- v2:      19/6 2003 - PS      tilfoejet clamping- og enablesignaler til rekonstruktionsfiltre
-- v3:      23/5 2003 - PS      nyt hieraki => toplevel = Tri_level_sync_generator.vhd
-- v301 :   21/8 2003 - PS      ny komponent sync_statemachine og state_timer
-------------------------------------------------------------------------------------

entity Tri_Level_Sync_Generator is
   Port ( 

        reset :                 in std_logic;       -- Master Reset fra mainboard
        f27m :                  in std_logic;       -- 27 MHz masterclock fra mainboard
        f74m :                  in std_logic;       -- output clock fra clock synthesizer - input til multiplier
        f148m :                 in std_logic;       -- 148 MHz master clock fra clock multiplier
        dac_clk :               out std_logic;      -- clocksignal til konverteren ( f148 inverteret i gammel kode, f74m i denne )

        tx :                    out std_logic;      -- serial port transmit
        rx :                    in std_logic;       -- serial port receive            

        f4m :                   in std_logic;       -- 1/8 x Vertikal sync  ( PAL format )
        fhm :                   in std_logic;       -- Horisontal sync      ( PAL - 525 linier pr frame )
        f8g :                   in std_logic;       -- 1/4 x Vertikal sync  ( NTSC format )
        fhg :                   in std_logic;       -- Horisontal sync      ( NTSC - 625 linier pr frame )

        bby :                   in std_logic;       -- not used
        bbx :                   in std_logic;       -- not used
        
--------- Video clock generator og mulitiplier --------------------------

        clk_generator_mode  :   out std_logic_vector(3 downto 0);       -- mode pins
        clk_generator_ref :     in std_logic;                           -- Reference clock output
        multiplier_mode  :      out std_logic_vector(3 downto 0);       -- mode pins
        multiplier_oe1 :        in std_logic;                           -- Output buffer 1 enable
        multiplier_oe2 :        in std_logic;                           -- Output buffer 2 enable
          
--------- styresignaler til reconstruction filter --------------------------

        sync_in_1 :             out std_logic;      -- aktiverer clamping paa filter output
        sync_in_2 :             out std_logic;      -- aktiverer clamping paa filter output
        disable_1 :             inout std_logic;    -- filter 1 disable ( aktiv hoej )
        disable_2 :             out std_logic;      -- filter 2 disable ( aktiv hoej )

--------- Video DAC ----------------------------------------------

        m1 :                    in std_logic;       -- mode1 pin
        m2 :                    in std_logic;       -- mode2 pin

        blank :                 out std_logic;      -- Sync styresignal til konverteren (altid nul)
        sync :                  inout std_logic;      -- Sync styresignal til konverteren
        sync_t :                inout std_logic;      -- Sync styresignal til konverteren
          
--------- DIP switches ----------------------------------------------
          
        d3 :                    inout std_logic_vector(9 downto 0);
		d2 :                    inout std_logic_vector(9 downto 0);
		d1 :                    in std_logic_vector(9 downto 0)

        );
end Tri_Level_Sync_Generator;

architecture Behavioral of Tri_Level_Sync_Generator is

component Tri_level_timer
    Port ( 
        tsg_start :         in std_logic;       -- start generator puls - aktiv lav
        f148 :              in std_logic;       -- Output fra clock multiplier ( 148 MHz )
        f148_div :          inout std_logic;    -- f148 divideret med 10 (progressiv) eller 11 (interlaced)

        scan_method :       in std_logic;                 -- ( 0=interlaced, 1=progressiv )
        samples_pr_line :   in integer range 0 to 8191;                  
        lines_pr_frame :    in integer range 0 to 2047;                  
            
        sample :            inout integer range 0 to 8191;
        line :              inout integer range 0 to 2047;
        line_freq :         out std_logic;                  -- line clock  (til test formaal)
        frame_freq :        inout std_logic;

        sync :              out std_logic; -- styresignal til konverteren
        sync_t :            out std_logic; -- styresignal til konverteren

--------- debug ---------------------------------------------------------------

        mid_pulse :         out std_logic;
        begin_pulse :       out std_logic;
        noof_pulses :       out std_logic;
        puls_type :         out std_logic
        );
end component;

component genlock
    Port(
        reset :                 in std_logic;       -- Master Reset fra mainboard
        f27m :                  in std_logic;       -- 27 MHz masterclock fra mainboard
        f148 :                  in std_logic;       -- Output fra clock multiplier ( 148 MHz )

        f4m :                   in std_logic;       -- 1/8 x Vertikal sync  ( PAL format )
        fhm :                   in std_logic;       -- Horisontal sync      ( PAL - 525 linier pr frame )
        f8g :                   in std_logic;       -- 1/4 x Vertikal sync  ( NTSC format )
        fhg :                   in std_logic;       -- Horisontal sync      ( NTSC - 625 linier pr frame )

        run_mode :              in std_logic;       -- 0=freerunning eller 1=genlocked
        frame_freq :            in std_logic;
        samples_pr_line :       in integer range 0 to 8191;
        lines_pr_frame :        in integer range 0 to 4095;
        sample_count :          in integer range 0 to 8191;
        line_count :            in integer range 0 to 4095;

        genlock_reset :         inout std_logic;    -- reset til trileveltimer

        -- debug :
        debugsig :              inout std_logic;
        pulsecond :             out std_logic;
        samplecond :            out std_logic;
        linecond :              out std_logic;
        framecond :             out std_logic;
        syncok :                out std_logic;
        syncpulse :             out std_logic;
        f4mint :                out std_logic
        );
end component;

component dac
    Port (
        reset :         in std_logic;
        f74 :           in std_logic;
        sync :          in std_logic;
        sync_t :        in std_logic;
        d3 :            inout std_logic_vector(7 downto 0)
        );
end component;


--------------------------------------------
-- video format registre, der skal tilskrives af CPU :

--signal ref_pixel :          integer range 0 to 8191;   -- for debug
--signal ref_line :           integer range 0 to 2047;   -- for debug

signal samples_pr_line :    integer range 0 to 8191;    -- 13 bit
signal lines_pr_frame :     integer range 0 to 2047;    -- 11 bit
signal sample_count :       integer range 0 to 8191;
signal line_count :         integer range 0 to 2047;
signal line_offset :        integer range 0 to 4095;
signal sample_offset :      integer range 0 to 8191;

signal frame_freq :         std_logic := '0';
signal field_offset :       std_logic;              -- offset fra genlock signal
signal scan_method :        std_logic;              -- 1 bit ( 0=interlaced, 1=progressiv )  
signal tsg_start :          std_logic;              -- reset til trileveltimer
signal genlock_reset :      std_logic;              -- reset fra genlock komponent
signal master_slave :       std_logic;
signal run_mode :           std_logic;              -- genlocked eller freerunning
signal f74i :               std_logic := '0';       -- intern genereret 74 MHz
signal f148t :              std_logic;		        -- masterclock til TrilevelSyncGenerator

-- debug signals :

signal f148_div :           std_logic;
--signal mid_pulse :          std_logic;
--signal begin_pulse :        std_logic;
--signal noof_pulses :        std_logic;
--signal puls_type :          std_logic;
signal syncok :             std_logic;
signal syncpulse :          std_logic;
signal f4mint :             std_logic;
signal line_freq :          std_logic;
signal debugsig :           std_logic;
signal pulsecond :          std_logic;
signal framecond :          std_logic;
signal linecond  :          std_logic;
signal samplecond :         std_logic;

begin

TriLevelTimer : Tri_level_timer port map (
        tsg_start       => tsg_start,
        f148            => f148t,
        f148_div        => f148_div,
        scan_method     => scan_method,
        samples_pr_line => samples_pr_line,
        lines_pr_frame  => lines_pr_frame,
        sample          => sample_count,
        line            => line_count,
        sync            => sync,
        sync_t          => sync_t,
        -- debug signals :
        frame_freq      => frame_freq,
        line_freq       => line_freq,
        mid_pulse       => open,--mid_pulse,
        begin_pulse     => open,--begin_pulse,
        noof_pulses     => open,--noof_pulses,
        puls_type       => open --puls_type
        );

Genlock_pulses : genlock port map (
        reset           => reset,
        f27m            => f27m,
        f148            => f148t,
        f4m             => f4m,
        fhm             => fhm,
        f8g             => f8g,
        fhg             => fhg,
        run_mode        => run_mode,
        frame_freq      => frame_freq,
        genlock_reset   => genlock_reset,
        samples_pr_line => samples_pr_line,
        lines_pr_frame  => lines_pr_frame,
        sample_count    => sample_count,
        line_count      => line_count,
        -- debug :
        debugsig        => debugsig,
        pulsecond       => pulsecond,
        samplecond      => samplecond,
        linecond        => linecond,
        framecond       => framecond,
        syncpulse       => syncpulse,
        syncok          => syncok,
        f4mint          => f4mint
        );

da_converter : dac port map (
        reset           => reset,
        f74             => f74i,
        sync            => sync,
        sync_t          => sync_t,
        d3(7 downto 0)  => d3(9 downto 2)
        );

--------------------------------------------------------------
-- test pins :
d2(9) <= syncok;
--d2(8) <= syncpulse;
d2(7) <= f148t;
--d2(6) <= pulsecond;
d2(5) <= f4mint;
--d2(4) <= frame_freq;
d2(3) <= samplecond;
d2(2) <= linecond;
d2(1) <= framecond;
d2(0) <= pulsecond;

--d3(1) <= syncok;
--d3(0) <= frame_freq;

--------------------------------------------------------------
-- reset til trileveltimer :
with run_mode select 
    tsg_start <= genlock_reset when '1',
                 reset when others;

--------------------------------------------------------------
-- clock til Trileveltimer ( f148t ) :
-- trileveltimer koerer paa f148's nedadgaaende flanke, saa sync og sync_t ikke
-- aendres, samtidig med f74m's hoejtgaaende flanke
f148t <= not f148m;

--------------------------------------------------------------
-- f74i = intern neddeling af f148m
make_f74i : process( f148m )
begin
    if f148m'event and f148m = '1' then
        f74i <= not f74i;
    end if;
end process;

--------------------------------------------------------------
-- setup videokonverter THS8135 :
dac_clk <= f74i;
blank <= '0';

--------------------------------------------------------------
-- setup clock synthesizer :
clk_generator_mode <= "0011";
-- mode "0010" = 74,25 MHz
-- mode "0011" = 74,176 MHz

--------------------------------------------------------------
-- setup clock multiplier :
multiplier_mode <= "1111";
-- mode "0011" = input gange 2

--------------------------------------------------------------
-- setup filter :
sync_in_1   <= '0';
sync_in_2   <= '0';
Disable_1 <= '0';
Disable_2 <= '0';

-------------------------------

-- 1920x1080/I :
scan_method <= '0';               -- 0=interlaced scan, 1=progresive scan
samples_pr_line <= 4399;          -- ( gange 2 )
lines_pr_frame <= 1124;
run_mode <= '1';                    

-- 1280x720/P :
--scan_method <= '1';                 -- 0=interlaced scan, 1=progresive scan
--samples_pr_line <= 3299;        
--lines_pr_frame <= 749;
--run_mode <= '1';

--debug/simulation :
--scan_method <= '1';             -- 0=interlaced scan, 1=progresive scan
--samples_pr_line <= 43;          -- ( gange 2 )
--lines_pr_frame <= 44;

end Behavioral;
