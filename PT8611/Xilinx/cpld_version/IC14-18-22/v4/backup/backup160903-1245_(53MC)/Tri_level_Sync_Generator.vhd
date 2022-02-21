library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-----------------------------------------------------------------------------------------------
-- v1:      Original kode til Prototype lavet af PEH + studerende Jesper Christoffersen 
--          for DK-Audio, som afgangsprojekt november 2002 til marts 2003
-- v2:      19/6 2003 - PS      tilfoejet clamping- og enablesignaler til rekonstruktionsfiltre
-- v3:      23/5 2003 - PS      nyt hieraki => toplevel = Tri_level_sync_generator.vhd
-- v301 :   21/8 2003 - PS      ny komponent sync_statemachine og state_timer
-- v4 :     8/9  2003 - PS      nyt printudlaeg m 100pin CPLD => ny pin-konfiguration
-----------------------------------------------------------------------------------------------

entity Tri_Level_Sync_Generator is
   Port ( 
--------- fra Mainframe -----------------------------------------------------------------------
        mreset :                in std_logic;       -- Master Reset fra mainboard
        f27m :                  in std_logic;       -- 27 MHz masterclock fra mainboard
        f74m :                  in std_logic;       -- output clock fra clock synthesizer - input til multiplier
        f148m :                 in std_logic;       -- 148 MHz master clock fra clock multiplier

        f4m :                   in std_logic;       -- 1/8 x Vertikal sync  ( NTSC format )
        fhm :                   in std_logic;       -- Horisontal sync      ( NTSC - 525 linier pr frame )
        f8g :                   in std_logic;       -- 1/4 x Vertikal sync  ( PAL format )
        fhg :                   in std_logic;       -- Horisontal sync      ( PAL - 625 linier pr frame )

--------- til/fra MasterCPLD ------------------------------------------------------------------
        gpbus :                 in std_logic_vector(7 downto 0);
        
--------- til/fra CPU -------------------------------------------------------------------------
        cs :                    in std_logic;
        sck :                   in std_logic;
        mosi :                  in std_logic;
        miso :                  in std_logic;       -- (output not used yet)

--------- til rekonstruktions filter -----------------------------------------------------------
        sync_in :               out std_logic;      -- styrer clamping paa filter output

--------- til DA-konverter --------------------------------------------------------------------
        dac_clk :               out std_logic;      -- f74i - internt genereret af f148m
        dac_mode :              in std_logic_vector(1 downto 0); -- pin m2 og m1 pa DAC
        blank :                 out std_logic;        -- altid lav
        sync :                  inout std_logic;      -- Sync styresignal til konverteren
        sync_t :                inout std_logic;      -- Sync styresignal til konverteren

        rcr :                   in std_logic_vector(9 downto 0);   -- output for debug (not used yet)
        gy :                    in std_logic_vector(9 downto 0);    -- not used yet
        bcb_h :                 in std_logic_vector(5 downto 0); -- inout for debug
        bcb_l :                 out std_logic_vector(3 downto 0)
        );
end Tri_Level_Sync_Generator;


architecture Behavioral of Tri_Level_Sync_Generator is


component frame_timer
    Port(
        tsg_reset :         in std_logic;       -- start generator puls - aktiv lav
        f148 :              in std_logic;       -- Output fra clock multiplier ( 148 MHz )
        samples_pr_line :   in integer range 0 to 8191;
        lines_pr_frame :    in integer range 0 to 2047;

        sample :            inout integer range 0 to 8191;
        line :              inout integer range 0 to 2047;
        frame :             inout std_logic_vector(1 downto 0)
        );
end component;

component Tri_level_timer
    Port ( 
        tsg_reset :         in std_logic;   -- start generator puls - aktiv lav
        f148 :              in std_logic;   -- Output fra clock multiplier ( 148 MHz )
        scan_method :       in std_logic;   -- ( 0=interlaced, 1=progressiv )
        samples_pr_line :   in integer range 0 to 8191;
        lines_pr_frame :    in integer range 0 to 2047;
        sample :            in integer range 0 to 8191;
        line :              in integer range 0 to 2047;

        sync :              out std_logic;  -- styresignal til konverteren
        sync_t :            out std_logic   -- styresignal til konverteren
        );
end component;

component genlock
    Port(
        mreset :                in std_logic;       -- Master Reset fra mainboard
        f27m :                  in std_logic;       -- 27 MHz masterclock fra mainboard
        f148 :                  in std_logic;       -- Output fra clock multiplier ( 148 MHz )
        f4m :                   in std_logic;       -- 4 x field rate
        f8g :                   in std_logic;       -- 8 x field rate

        sync_mode :             in std_logic_vector(1 downto 0); -- 0X=freerunning, 10=genlocked to f4m, 11= genlocked to f8g
        phasedelay_reg :        in std_logic_vector(25 downto 0);
        samples_pr_line :       in integer range 0 to 8191;
        lines_pr_frame :        in integer range 0 to 4095;
        sample :                in integer range 0 to 8191;
        line :                  in integer range 0 to 4095;
        frame :                 in std_logic_vector(1 downto 0);

        greset :                inout std_logic;    -- reset til trileveltimer

        -- debug :
        internsync :            out std_logic;
        startcounter :          out std_logic;
        syncdelayed :           out std_logic;
        syncok :                out std_logic
        );
end component;

component dac
    Port(
        tsg_reset :     in std_logic;
        f74 :           in std_logic;
        sync :          in std_logic;
        sync_t :        in std_logic;
        analoglevel :   out std_logic_vector(3 downto 0)
        );
end component;

--component serial_interface
  --  Port(
    --    cs :                in std_logic;
      --  sck :               in std_logic;
        --mosi :              in std_logic;
--        miso :              in std_logic;       -- (output not used yet)
--
  --      scan_method :       out std_logic;   -- ( 0=interlaced, 1=progressiv )
    --    sync_mode :         out std_logic_vector(1 downto 0); -- 0X=freerunning, 10=genlocked to f4m, 11= genlocked to f8g
      --  phasedelay_reg :    out std_logic_vector(25 downto 0);
        --samples_pr_line :   out integer range 0 to 8191;
--        lines_pr_frame :    out integer range 0 to 4095;
  --      );

----------------------------------------------------------------------------------------
-- video format registre, der skal tilskrives af CPU :
signal samples_pr_line_reg :    std_logic_vector(13 downto 0);  -- 14 bit
signal lines_pr_frame_reg :     std_logic_vector(10 downto 0);  -- 12 bit
signal phasedelay_reg :         std_logic_vector(25 downto 0);  -- 26 bit   ( delay i samples fra f4m, f8g )
signal sync_mode_reg :           std_logic_vector(1 downto 0);   -- 2 bit    ( 0X=freerunning, 10=genlocked to f4m, 11=genlocked to f8g )
signal scan_method_reg :        std_logic;                      -- 1 bit    ( 0=interlaced, 1=progressiv )  

-------------------------------------------------------------------------
signal samples_pr_line_int :    integer range 0 to 8191;        -- 14 bit
signal lines_pr_frame_int :     integer range 0 to 2047;        -- 12 bit
signal sample :             integer range 0 to 8191;
signal line :               integer range 0 to 2047;
signal frame :              std_logic_vector(1 downto 0);
signal frame_freq :         std_logic;
signal tsg_reset :          std_logic;              -- reset til trileveltimer
signal greset :             std_logic;              -- reset fra genlock komponent
signal f74i :               std_logic;              -- intern genereret 74 MHz
signal f148t :              std_logic;		        -- masterclock til TrilevelSyncGenerator

-- debug signals : ---------------
signal internsync :     std_logic;
signal startcounter :   std_logic;
signal syncdelayed :    std_logic;
signal syncok :         std_logic;
----------------------------------

begin

--bcb_h(5) <= syncok;

FrameTimer : frame_timer port map (
        tsg_reset       => tsg_reset,
        f148            => f148t,
        samples_pr_line => samples_pr_line_int,
        lines_pr_frame  => lines_pr_frame_int,
        sample          => sample,
        line            => line,
        frame           => frame
        );


TriLevelTimer : Tri_level_timer port map (
        tsg_reset       => tsg_reset,
        f148            => f148t,
        scan_method     => scan_method_reg,
        samples_pr_line => samples_pr_line_int,
        lines_pr_frame  => lines_pr_frame_int,
        sample          => sample,
        line            => line,
        sync            => sync,
        sync_t          => sync_t
        );

Genlock_pulses : genlock port map (
        mreset          => mreset,
        f27m            => f27m,
        f148            => f148t,
        f4m             => f4m,
        f8g             => f8g,
        sync_mode       => sync_mode_reg,
        phasedelay_reg  => phasedelay_reg,
        greset          => greset,
        samples_pr_line => samples_pr_line_int,
        lines_pr_frame  => lines_pr_frame_int,
        sample          => sample,
        line            => line,
        frame           => frame,
        -- debug :
        internsync      => internsync,
        startcounter    => startcounter,
        syncdelayed     => syncdelayed,
        syncok          => syncok
        );

da_converter : dac port map (
        tsg_reset               => tsg_reset,
        f74                     => f74i,
        sync                    => sync,
        sync_t                  => sync_t,
        analoglevel(3 downto 0) => BCb_L(3 downto 0)
        );


--------------------------------------------------------------
-- konvertering af std_logic vektorer til integer :
samples_pr_line_int <= conv_integer(samples_pr_line_reg);
lines_pr_frame_int  <= conv_integer(lines_pr_frame_reg);
--------------------------------------------------------------
-- reset til trileveltimer :
with sync_mode_reg(1) select 
    tsg_reset <= greset when '1',
                 mreset when others;
--------------------------------------------------------------
-- reset video registre :
reset_registers : process( tsg_reset )
begin
    if tsg_reset = '0' then
        phasedelay_reg <= "00100101110000111111001100";
    end if;
end process;
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
-- setup filter :
sync_in   <= '0';   -- clamping inactive
-------------------------------
-- 1920x1080/I :
scan_method_reg <= '0';             -- 0=interlaced scan, 1=progresive scan
samples_pr_line_reg <= "01000100101111";      -- 4399
lines_pr_frame_reg  <= "10001100100";      --1124
-- sync_mode_reg <= "10";                    
-- phasedelay_reg <= "00000000000000000000001111";
-- phasedelay_reg <= "00100101110000111111001100";

-- 1280x720/P :
--scan_method <= '1';                 -- 0=interlaced scan, 1=progresive scan
--samples_pr_line <= 3299;        
--lines_pr_frame <= 749;
--sync_mode <= '1';

end Behavioral;
