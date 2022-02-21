library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
library UNISIM;
use UNISIM.VComponents.all;

entity syncgenerator is
    Port (
        -- fra mainframe : ------------------------------------
        mreset :        in std_logic;       -- Master Reset fra mainboard
        f27 :           in std_logic;       -- 27 MHz clock fra mainboard
        f148 :          in std_logic;       -- 148 MHz clock
        f4m :           in std_logic;       -- 1/4 x Vertikal sync - 625 lines format
        f8g :           in std_logic;       -- 1/8 x Vertikal sync - 525 lines format
        -- til/fra cpu : --------------------------------------
        sck :           in std_logic;
        mosi :          in std_logic;
        cs :            in std_logic;
        -- ouput check til changeover unit : ------------------
        tsg_lvl :       in std_logic;
        tsg_ok :        out std_logic;
        -- cyctem clock select : ------------------------------
        sysclk_sel :    out std_logic;
        -- Generator output : ---------------------------------
        tsg_out :       out std_logic_vector(3 downto 0);
        -- testpoints : ---------------------------------------
        tp :            out std_logic_vector(7 downto 0)
        );
end syncgenerator;


architecture Behavioral of syncgenerator is


component genlock_timing
    Port(
        mreset :            in std_logic;   -- master reset
        f27 :               in std_logic;   -- 27 MHz master clock
        f148 :              in std_logic;   -- 148 MHz mux'ed clock
        f4m :               in std_logic;   -- sync reference for 625 line compliant formats
        f8g :               in std_logic;   -- sync reference for 525 line compliant formats
        sync_mode :         in std_logic_vector(1 downto 0);    -- 0X=freerunning, 10=genlocked to f4m, 11= genlocked to f8g
        phasedelay :        in std_logic_vector(23 downto 0);   -- delay from sync reference
        cs :                in std_logic;
        last_count :        in std_logic;   -- indikerer samples ved linieskift
        last_lineframe :    in std_logic;   -- indikerer linieskift ved frame end
        new_delay :         in std_logic;   -- indikerer nyt phasedelay
        genlock_reset :     out std_logic;    -- reset til trileveltimer
        -- debug :
        internsync :        out std_logic;
        startcounter :      out std_logic;
        delaycount :        out integer range 0 to 16777215;
        dlycnt_active :     out std_logic;
        syncdelayed :       out std_logic;
        gresetunmask :      out std_logic;

        genlockok :         out std_logic;
        delay_cmp :         out std_logic_vector(23 downto 0);
        genlockwindow :     out std_logic
        );
end component;

component serial_interface
    Port( 
        mreset :        in std_logic;
        cs :            in std_logic;
        sck :           in std_logic;
        mosi :          in std_logic;
        spl_div :       inout std_logic_vector(9 downto 0);
        lpf :           out integer range 0 to 2047;
        div10_11 :      out std_logic;
        framerate_x2 :  out std_logic;
        sync_mode :     out std_logic_vector(1 downto 0);
        scan_method :   inout std_logic;
        sysclk_sel :    out std_logic;
        phasedelay :    out std_logic_vector(23 downto 0);
        freerun_reset : out std_logic;
        new_delay :     out std_logic;
        -- debug :
        lpfbit :        out std_logic
        );
end component;

component Tri_level_timer
    Port ( 
        mreset :            in std_logic;       -- reset til sync_statemachine
        tsg_reset :         in std_logic;       -- start generator puls - aktiv lav
        f148 :              in std_logic;       -- multiplexed 148 MHz clock
        scan_method :       in std_logic;       -- ( 1=interlaced, 0=progressiv )
        div10_11 :          in std_logic;
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
--        lvlsample_db :      inout std_logic;
        statecounter :      out integer range 0 to 63;
        state_db :          out integer range 0 to 7;
        f74_db :            out std_logic;
        sync_sim :          out std_logic;
        sync_t_sim :        out std_logic
        );
end component;


-------------------------------------------------------------------------
signal framerate_x2 :       std_logic;
signal spl_div :            std_logic_vector(9 downto 0);   -- total samples pr. line divideret med 10 (prog) eller 11 (interlaced)
signal lines_pr_frame :     integer range 0 to 2047;
signal div10_11 :           std_logic;
signal phasedelay :         std_logic_vector(23 downto 0);  -- 26 bit   ( delay i samples fra f4m, f8g )
signal sync_mode :          std_logic_vector(1 downto 0);   -- 2 bit    ( 0X=freerunning, 10=genlocked to f4m, 11=genlocked to f8g )
signal scan_method :        std_logic;
-------------------------------------------------------------------------
signal last_count :         std_logic;  -- indikerer end of line
signal last_lineframe :     std_logic;  -- fra tlg cpld
signal sw_reset :           std_logic;  -- freerun_reset or genlock_reset
signal freerun_reset :      std_logic;  -- indikerer nyt format i video registre
signal genlock_reset :      std_logic;  -- reset fra genlock komponent
signal tsg_reset :          std_logic;  -- reset til trileveltimer
signal mute :               std_logic;  -- mute'r output af tsg - active low
signal new_delay :          std_logic;  -- indikerer nyt phasedelay
-- debug signals : ---------------------------------
signal divclk :             std_logic;
signal spldivdowncount :    integer range 0 to 1023;
signal linie :              integer range 0 to 2047;
signal lpfbit :             std_logic;
signal frame_db :           integer range 0 to 3;
signal internsync :         std_logic;
signal startcounter :       std_logic;
signal delay_cmp :          std_logic_vector(23 downto 0);
signal genlockok :          std_logic;
signal delaycount :         integer range 0 to 16777215;
signal dlycnt_active :      std_logic;
signal syncdelayed :        std_logic;
signal gresetunmask :       std_logic;
signal genlockwindow :      std_logic;
signal lineclock :          std_logic;
signal frameclock :         std_logic;
signal linebegin :          std_logic;
signal linemid :            std_logic;
signal noofpulses :         std_logic;
signal pulsetype :          std_logic;
--signal lines_pr_frame :     integer range 0 to 2047;
signal spldivcnt_active :   std_logic;
signal divsig :             integer range 0 to 15;
signal clksync :            std_logic;
signal smclk :              std_logic;
signal lvlsample_db :       std_logic;
signal statecounter :       integer range 0 to 63;
signal state_db :           integer range 0 to 7;
signal f74_db :             std_logic;
signal sync :               std_logic;
signal sync_t :             std_logic;


begin


Genlock : genlock_timing port map (
        mreset              => mreset,
        f27                 => f27,
        f148                => f148,
        f4m                 => f4m,
        f8g                 => f8g,
        sync_mode           => sync_mode,
        phasedelay          => phasedelay,
        cs                  => cs,
        last_count          => last_count,
        last_lineframe      => last_lineframe,
        new_delay           => new_delay,
        genlock_reset       => genlock_reset,
        -- debug signals :
        internsync          => internsync,
        startcounter        => startcounter,
        delaycount          => delaycount,
        dlycnt_active       => dlycnt_active,
        genlockok           => genlockok,
        delay_cmp           => delay_cmp,
        syncdelayed         => syncdelayed,
        gresetunmask        => gresetunmask,
        genlockwindow       => genlockwindow
        );

sif : serial_interface port map (
        mreset          => mreset,
        cs              => cs,
        sck             => sck,
        mosi            => mosi,
        spl_div         => spl_div,
        lpf             => lines_pr_frame,
        div10_11        => div10_11,
        framerate_x2    => framerate_x2,
        sync_mode       => sync_mode,
        scan_method     => scan_method,
        sysclk_sel      => sysclk_sel,
        phasedelay      => phasedelay,
        freerun_reset   => freerun_reset,
        new_delay       => new_delay,
        -- debug :
        lpfbit          => lpfbit
        );

TLTimer : Tri_level_timer port map (
        mreset              => mreset,
        tsg_reset           => tsg_reset,
        f148                => f148,
        scan_method         => scan_method,
        div10_11            => div10_11,
        sync_mode_lsb       => sync_mode(0),
        spl_div             => spl_div,
        lines_pr_frame      => lines_pr_frame,
        framerate_x2        => framerate_x2,
        tsg_lvl             => tsg_lvl,
        tsg_ok              => tsg_ok,
        last_count          => last_count,
        last_lineframe      => last_lineframe,
        tsg_out             => tsg_out,
        -- debug signals :
        divclk              => divclk,
        spldivdowncount     => spldivdowncount,
        lineclock           => lineclock,
        linie               => linie,
        frameclock          => frameclock,
        frame_db            => frame_db,
        linebegin           => linebegin,
        linemid             => linemid,
        noofpulses          => noofpulses,
        pulsetype           => pulsetype,
        spldivcnt_active    => spldivcnt_active,
        divsig              => divsig,
        clksync             => clksync,
        smclk               => smclk,
--        lvlsample_db        => lvlsample_db,
        statecounter        => statecounter,
        state_db            => state_db,
        f74_db              => f74_db,
        sync_sim            => sync,
        sync_t_sim          => sync_t
        );

--------------------------------------------------------------
-- reset til trileveltimer (tsg_reset) :
-- genlock_reset = reset puls synkroniseret til f4m eller f8g plus phasedelay
-- freerun_reset = chip select ( reset, naar der skrives til videoregistrene )
-- mreset = reset fra mainframe
make_sw_reset : process( sync_mode(1), genlock_reset, freerun_reset )
begin
    if sync_mode(1) = '1' then
        sw_reset <= genlock_reset;
    else
        sw_reset <= freerun_reset;
    end if;
end process;

make_mute_signal : with spl_div select
    mute <= '0' when "0000000000",
            '1' when others;

make_tsg_reset : tsg_reset <= mreset and sw_reset and mute;

-------------------------------------------------------------
-- set testpoints :
with sync_mode select
    tp(7) <= f4m when "11",
             f8g when "10",
             frameclock when others;
tp(6) <= startcounter;
tp(5) <= syncdelayed;
tp(4) <= sw_reset;
tp(3) <= tsg_reset;
tp(2) <= genlockwindow;
tp(1) <= gresetunmask;
tp(0) <= genlockok;

end Behavioral;