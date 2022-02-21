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
--                                                               + opsplitning af software
-----------------------------------------------------------------------------------------------

entity Tri_Level_Sync_Generator is
   Port ( 
--------- fra Mainframe -----------------------------------------------------------------------
        mreset :    in std_logic;       -- Master Reset fra mainboard
        f27m :      in std_logic;       -- 27 MHz masterclock fra mainboard
        f14835 :    in std_logic;       -- pin 24 alternativ indgang for 148.35 MHz clock
        f1485 :     in std_logic;       -- pin 25 alternativ indgang for 148.5 MHz clock
--        f1485m :    in std_logic;       -- 148.5 MHz master clock fra ic4
--        f14835m :   in std_logic;       -- 148.5/1.001 MHz master clock fra ic2
        f4m :       in std_logic;       -- 1/4 x Vertikal sync - 625 lines SD format
--        fhm :       in std_logic;       -- Horisontal sync
        f8g :       in std_logic;       -- 1/8 x Vertikal sync - 525 lines SD format
--        fhg :       in std_logic;       -- Horisontal sync

--------- til/fra MasterCPLD ------------------------------------------------------------------
        gpbus7 :    out std_logic;
        gpbus6 :    out std_logic;
        gpbus5 :    in std_logic;
        gpbus4 :    in std_logic;
        gpbus3 :    out std_logic;
        gpbus2 :    in std_logic;
        gpbus1 :    out std_logic;
        gpbus0 :    in std_logic;
        
--------- til/fra CPU -------------------------------------------------------------------------
        cs :        in std_logic;
        sck :       in std_logic;
        mosi :      in std_logic;
--        miso :      in std_logic;       -- not used

--------- til rekonstruktions filter -----------------------------------------------------------
        sync_in :   out std_logic;      -- styrer clamping paa filter output

--------- til THS8135 --------------------------------------------------------------------------
--        dac_clk :   out std_logic;      -- f74i - internt genereret af f1485m eller f14835m
  --      dac_mode :  in std_logic_vector(1 downto 0); -- pin m2 og m1 pa DAC
    --    blank :     out std_logic;        -- altid lav
--        rcr :       in std_logic_vector(9 downto 0);   -- output for debug (not used yet)
        gy :        in std_logic_vector(9 downto 0);    -- not used yet
    --    bcb_h :     in std_logic_vector(5 downto 0); -- inout for debug
        bcb_l :     out std_logic_vector(3 downto 0);   -- output til "hjemmelavet" DA-konverter
        sync_tp :   inout std_logic;      -- Sync styresignal til konverteren / test pin
        sync_t_tp : inout std_logic       -- Sync styresignal til konverteren / test pin
        );
end Tri_Level_Sync_Generator;


architecture Behavioral of Tri_Level_Sync_Generator is


component da_converter
    Port(
        tsg_reset :         in std_logic;
        f148 :              in std_logic;
        sync :              in std_logic;
        sync_t :            in std_logic;
        analoglevel :       out std_logic_vector(3 downto 0)
        );
end component;

component genlock_timing
    Port(
        mreset :            in std_logic;
        f27m :              in std_logic;
        f148 :              in std_logic;
        f4m :               in std_logic;
        f8g :               in std_logic;
        scan_method :       in std_logic;
        sync_mode :         in std_logic_vector(1 downto 0);
        phasedelay :        in std_logic_vector(25 downto 0);
        last_count :        in std_logic;
        last_lineframe :    in std_logic;
        greset :            inout std_logic;
        -- debug :
--        internsync :        out std_logic;
  --      startcounter :      out std_logic;
    --    delaycount :        out integer range 0 to 67108863;
      --  dlycnt_active :     out std_logic;
        syncdelayed :       out std_logic;
--        lastcount :         out std_logic;
  --      windowcount :       out std_logic_vector(3 downto 0);
        genlockwindow :     out std_logic
        );
end component;

component serial_interface
    Port( 
        mreset :        in std_logic;
        cs :            in std_logic;
        sck :           in std_logic;
        mosi :          in std_logic;
        spl_div :       out std_logic_vector(9 downto 0);
        sync_mode :     out std_logic_vector(1 downto 0);
        system_clk :    out std_logic;
        phasedelay :    out std_logic_vector(25 downto 0)
        );
end component;

component Tri_level_timer
    Port ( 
        tsg_reset :         in std_logic;
        f148 :              in std_logic;
        scan_method :       in std_logic;
        spl_div :           in std_logic_vector(9 downto 0);
        no_of_pulses :      in std_logic;
        pulse_type :        in std_logic;
        f148_div :          inout std_logic;
        spldiv_downcount :  inout std_logic_vector(9 downto 0);
        last_count :        out std_logic;
        line_clk :          out std_logic;
        sync :              out std_logic;
        sync_t :            out std_logic
        --debug :
--        linebegin :         out std_logic;
  --      linemid :           out std_logic;
    --    spldivcnt_active :  out std_logic;
      --  divsig :            out integer range 0 to 15;
        --clksync :           out std_logic;
--        smclk :             out std_logic
        );
end component;


--------------------------------------------------------------
-- force global clocks :
attribute BUFG: string;
attribute BUFG of f27m: signal is "CLK";
-------------------------------------------------------------------------
signal spl_div :            std_logic_vector(9 downto 0);   -- total samples pr. line divideret med 10 (prog) eller 11 (interlaced)
signal phasedelay :         std_logic_vector(25 downto 0);  -- 26 bit   ( delay i samples fra f4m, f8g )
signal sync_mode :          std_logic_vector(1 downto 0);   -- 2 bit    ( 0X=freerunning, 10=genlocked to f4m, 11=genlocked to f8g )
signal system_clk :         std_logic;
signal scan_method :        std_logic;
-------------------------------------------------------------------------
signal spldiv_downcount :   std_logic_vector(9 downto 0);
signal last_count :         std_logic;  -- indikerer end of line
signal sync :               std_logic;  -- styresignal til konverter
signal sync_t :             std_logic;  -- styresignal til konverter
signal tsg_reset :          std_logic;  -- reset til trileveltimer
signal greset :             std_logic;  -- reset fra genlock komponent
-------------------------------------------------------------------------
signal f148 :               std_logic;
signal f148_div :           std_logic;  -- til tlg cpld
signal no_of_pulses :       std_logic;  -- fra tlg cpld
signal pulse_type :         std_logic;  -- fra tlg cpld
signal line_clk :           std_logic;  -- til tlg cpld (aktiv lav pga tPLH opencollector levelkonverter)
signal last_lineframe :     std_logic;  -- fra tlg cpld

-- debug signals : ---------------------------------
--signal line :           integer range 0 to 2047;
--signal internsync :     std_logic;
--signal startcounter :   std_logic;
--signal delaycount :     integer range 0 to 67108863;
--signal dlycnt_active :  std_logic;
signal syncdelayed :    std_logic;
--signal windowcount :    std_logic_vector(3 downto 0);
signal genlockwindow :  std_logic;
--signal linebegin :      std_logic;
--signal linemid :        std_logic;
--signal mcpld_gpbus :    std_logic_vector(7 downto 0);
--signal lines_pr_frame : integer range 0 to 2047;
--signal spldivcnt_active :   std_logic;
--signal divsig :         integer range 0 to 15;
--signal clksync :        std_logic;
--signal smclk :          std_logic;
-----------------------------------------------------

begin

dac : da_converter port map (
        tsg_reset               => tsg_reset,
        f148                    => f148,
        sync                    => sync,
        sync_t                  => sync_t,
        analoglevel(3 downto 0) => bcb_l(3 downto 0)
        );

Genlock : genlock_timing port map (
        mreset              => mreset,
        f27m                => f27m,
        f148                => f148,
        f4m                 => f4m,
        f8g                 => f8g,
        scan_method         => scan_method,
        sync_mode           => sync_mode,
        phasedelay          => phasedelay,
        greset              => greset,
        last_count          => last_count,
        last_lineframe      => last_lineframe,
        -- debug :
--        internsync          => open,
  --      startcounter        => open,
    --    delaycount          => open,
      --  dlycnt_active       => open,
        syncdelayed         => syncdelayed,
--        lastcount           => open,
  --      windowcount         => open,
        genlockwindow       => genlockwindow
        );

sif : serial_interface port map (
        mreset          => mreset,
        cs              => cs,
        sck             => sck,
        mosi            => mosi,
        spl_div         => spl_div,
        sync_mode       => sync_mode,
        system_clk      => system_clk,
        phasedelay      => phasedelay
        );

TLTimer : Tri_level_timer port map (
        tsg_reset           => tsg_reset,
        f148                => f148,
        scan_method         => scan_method,
        spl_div             => spl_div,
        no_of_pulses        => no_of_pulses,
        pulse_type          => pulse_type,
        f148_div            => f148_div,
        spldiv_downcount    => spldiv_downcount,
        last_count          => last_count,
        sync                => sync,
        sync_t              => sync_t,
        line_clk            => line_clk
        -- debug :
--        linemid             => open,
  --      linebegin           => open,
    --    spldivcnt_active    => open,
      --  divsig              => open,
        --clksync             => open,
--        smclk               => open
        );


-------------------------------------------------------------
-- set system clock :
clock_mux : process (system_clk, f14835, f1485) 
begin 
    if (system_clk = '0') then 
        f148 <= f1485; 
    else 
        f148 <= f14835; 
    end if; 
end process;

--------------------------------------------------------------
-- reset til trileveltimer (tsg_reset) :
-- greset = reset puls synkroniseret til f4m eller f8g plus phasedelay
-- mreset = reset fra mainframe
-- cs = 0, betyder at der skrives til videoformat registre
with sync_mode(1) select 
    tsg_reset <= (greset and cs) when '1',
                 (mreset and cs) when others;

--------------------------------------------------------------
-- setup filter :
sync_in   <= '0';

--------------------------------------------------------------
-- setup general purpose bus :
gpbus7          <= tsg_reset;       -- (out)
gpbus6          <= f148_div;        -- (out)
no_of_pulses    <= gpbus5;          -- (in)  til trileveltimer komponent
pulse_type      <= gpbus4;          -- (in)  til trileveltimer komponent
gpbus3          <= line_clk;        -- (out)
last_lineframe  <= gpbus2;          -- (in)  til genlock komponent
gpbus1          <= sync_mode(0);    -- (out)
scan_method     <= gpbus0;

--------------------------------------------------------------
-- setup videokonverter THS8135 :
--dac_clk <= f74i;
--blank <= '0';

--------------------------------------------------------------
-- testpins :
sync_tp <= genlockwindow; -- testpin
--sync_tp <= tsg_reset; -- testpin
sync_t_tp <= syncdelayed; -- testpin
--sync_t_tp <= tsg_reset; -- testpin
--sync_t_tp <= f148_div; -- testpin

------------------------------------------------
-- debug format 1920x1080/60/I :
--system_clk <= '1';
--sync_mode <= "10";
--phasedelay <= "00000000000000000000001111";
--spl_div <= "0110001111";    -- ( 4400/11 - 1 ) = 399
--phasedelay <= "00100101110000111111001100"; -- ( 4400x1125x2-20 )
--scan_method <= '1';
--lines_pr_frame <= 1124;

end Behavioral;
