library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-------------------------------------------------------------------------------------
-- v1:      Original kode til Prototype lavet af PEH + studerende Jesper Christoffersen 
--          for DK-Audio, som afgangsprojekt november 2002 til marts 2003
-- v2:      19/6 2003 - PS      tilfoejet clamping- og enablesignaler til rekonstruktionsfiltre
-- v3:      23/5 2003 - PS      nyt hieraki => toplevel = Tri_level_sync_generator.vhd
-------------------------------------------------------------------------------------

entity Tri_Level_Sync_Generator is
   Port ( 

        reset :                 in std_logic;       -- Master Reset fra mainboard
        f27 :                   in std_logic;       -- 27 MHz masterclock fra mainboard
        f74 :                   in std_logic;       -- output clock fra clock synthesizer - input til multiplier
        f148 :                  in std_logic;       -- Output fra clock multiplier ( 148 MHz )
        dac_clk :               out std_logic;      -- clocksignal til konverteren ( f148 inverteret i gammel kode, f74 i denne )

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

		d2 :                    in std_logic_vector(9 downto 0);
		
		d1 :                    in std_logic_vector(9 downto 0);
		
          
--------- test pins ----------------------------------------------

        frame :                 out std_logic;      -- frame clock (til test formaal)
        line_freq :             out std_logic       -- line clock  (til test formaal)
          
        );
end Tri_Level_Sync_Generator;

architecture Behavioral of Tri_Level_Sync_Generator is

component Tri_level_timer
    Port ( 
        tsg_start :         in std_logic;       -- start generator pulse - aktiv lav
        f148 :              in std_logic;       -- Output fra clock multiplier ( 148 MHz )
        f148_div :          inout std_logic;    -- f148 divideret med 10 (progressiv) eller 11 (interlaced)

        scan_method :       in std_logic;                 -- ( 0=interlaced, 1=progressiv )
        cnt_setup :         in integer range 0 to 1023;   -- vaerdi til valg af Video standard        
        samples_pr_line :   in integer range 0 to 8191;   -- * 2 pga oversampling
        lines_pr_frame :    in integer range 0 to 2047;                  
            
        -- til konverter :
        sync :              out std_logic;      -- styresignal til konverteren
        sync_t :            out std_logic;      -- styresignal til konverteren
        blank :             out std_logic;      -- styresignal til konverteren (altid nul)

        -- til rekonstruktionsfiltre :        
        sync_in_1 :         out std_logic;      -- aktiverer clamping paa filter output
        sync_in_2 :         out std_logic;      -- aktiverer clamping paa filter output
        disable_1 :         inout std_logic;    -- filter 1 disable ( aktiv hoej )
        disable_2 :         out std_logic;      -- filter 2 disable ( aktiv hoej )

        -- test pins :
        frame :             out std_logic;                  -- frame clock (til test formaal)
        line_freq :         out std_logic;                  -- line clock  (til test formaal)
        ref_pixel :         inout integer range 0 to 8191;  -- for debug
        ref_line :         out integer range 0 to 2047      -- for debug
         
        );
end component;

component genlock
    Port(
        reset :                 in std_logic;       -- Master Reset fra mainboard
        f27 :                   in std_logic;       -- 27 MHz masterclock fra mainboard
        f148 :                  in std_logic;       -- Output fra clock multiplier ( 148 MHz )

        f4m :                   in std_logic;       -- 1/8 x Vertikal sync  ( PAL format )
        fhm :                   in std_logic;       -- Horisontal sync      ( PAL - 525 linier pr frame )
        f8g :                   in std_logic;       -- 1/4 x Vertikal sync  ( NTSC format )
        fhg :                   in std_logic;       -- Horisontal sync      ( NTSC - 625 linier pr frame )

        run_mode :              in std_logic;       -- 0=freerunning eller 1=genlocked
        samples_pr_line :       in integer range 0 to 8191;
        lines_pr_frame :        in integer range 0 to 4095;

        tsg_start :             inout std_logic     -- reset til trileveltimer
        );
end component;

component dac
    Port (
        reset :         in std_logic;
        f74 :           in std_logic;
        f148 :          in std_logic;
        sync :          in std_logic;
        sync_t :        in std_logic;
        d3 :            inout std_logic_vector(9 downto 0);
        ti_dac :        in std_logic
        );
end component;


--------------------------------------------
-- video format registre, der skal tilskrives af CPU :

--signal ref_pixel :          integer range 0 to 8191;   -- for debug
--signal ref_line :           integer range 0 to 2047;   -- for debug

signal cnt_setup :          integer range 0 to 1023;    -- 10 bit
signal samples_pr_line :    integer range 0 to 8191;    -- 13 bit
signal lines_pr_frame :     integer range 0 to 2047;    -- 11 bit
signal scan_method :        std_logic;                  -- 1 bit ( 0=interlaced, 1=progressiv )  

signal tsg_start :          std_logic;
signal master_slave :       std_logic;
signal run_mode :           std_logic;
signal field_offset :       std_logic;
signal f148_div :           std_logic;
signal line_offset :        integer range 0 to 4095;
signal sample_offset :      integer range 0 to 8191;

signal f148_trileveltimer : std_logic;		-- masterclock til TrilevelSyncGenerator
signal Texas_converter :    std_logic := '1';      -- hvis high kompileres den hjemmelavede DAconverter ikke op.

begin

TriLevelTimer : Tri_level_timer port map (
        tsg_start       => tsg_start,
        f148            => f148_trileveltimer,
        f148_div        => f148_div,
        scan_method     => scan_method,
        cnt_setup       => cnt_setup,
        samples_pr_line => samples_pr_line,
        lines_pr_frame  => lines_pr_frame,
        blank           => blank,
        sync            => sync,
        sync_t          => sync_t,
        sync_in_1       => sync_in_1,
        sync_in_2       => sync_in_2,
        disable_1       => disable_1,
        disable_2       => disable_2,
        frame           => frame,
        line_freq       => line_freq,
        ref_pixel      => open,
        ref_line       => open
        );

Genlock_pulses : genlock port map (
        reset           => reset,
        f27             => f27,
        f148            => f148_trileveltimer,
        f4m             => f4m,
        fhm             => fhm,
        f8g             => f8g,
        fhg             => fhg,
        run_mode        => run_mode,
        tsg_start       => tsg_start,
        samples_pr_line => samples_pr_line,
        lines_pr_frame  => lines_pr_frame
        );

da_converter : dac port map (
        reset       => reset,
        f74         => f74,
        f148        => f148,
        sync        => sync,
        sync_t      => sync_t,
        d3          => d3,
        ti_dac      => texas_converter
        );

-------------------------------
-- clock til THS8135 :
dac_clk <= f74;
-------------------------------
-- clock til Trileveltimer :
-- trileveltimer koerer paa f148's nedadgaaende flanke, saa sync og sync_t ikke
-- aendres, samtidig med f27's hoejtgaaende flanke
f148_trileveltimer <= not f148;
-------------------------------
-- setup clock synthesizer :
clk_generator_mode <= "0010";
--IC5_S0 <= '0'; -- saetter IC5 
--IC5_S1 <= '1'; -- til at lave 
--IC5_S2 <= '0'; -- 74,25 MHz
--IC5_S3 <= '0';
-------------------------------
-- setup clock multiplier :
multiplier_mode <= "1111";
--IC6_S0 <= '1'; -- saetter IC6
--IC6_S1 <= '1'; -- til at gange
--IC6_S2 <= '1'; -- med faktor 2
--IC6_S3 <= '1';
-------------------------------

-- 1920x1080/I :
cnt_setup <= 173;                 -- = 2*b - 88 - 1
scan_method <= '0';               -- 0=interlaced scan, 1=progresive scan
samples_pr_line <= 4399;          -- ( gange 2 )
lines_pr_frame <= 1124;
run_mode <= '1';                    

-- 1280x720/P :
--cnt_setup <= 157;
--scan_method <= '1';                 -- 0=interlaced scan, 1=progresive scan
--samples_pr_line <= 3299;        
--lines_pr_frame <= 749;
--run_mode <= '1';

--debug/simulation :
--cnt_setup <= 3;                 -- = 2*b - 88 - 1
--scan_method <= '0';             -- 0=interlaced scan, 1=progresive scan
--samples_pr_line <= 43;          -- ( gange 2 )
--lines_pr_frame <= 44;



end Behavioral;
