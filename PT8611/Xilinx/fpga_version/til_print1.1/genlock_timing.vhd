library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity genlock_timing is
    port ( 
        mreset :            in std_logic;   -- master reset
        f27 :               in std_logic;   -- 27 MHz master clock
        f148 :              in std_logic;   -- 148 MHz mux'ed clock
        f4m :               in std_logic;   -- sync reference for 625 line compliant formats
        f8g :               in std_logic;   -- sync reference for 525 line compliant formats
        f4_free :           in std_logic;   -- sync reference ved freerunning formater
        sync_mode :         in std_logic_vector(1 downto 0);    -- 0X=freerunning, 10=genlocked to f4m, 11= genlocked to f8g
        phasedelay :        in std_logic_vector(23 downto 0);   -- delay from sync reference
        cs :                in std_logic;
        last_count :        in std_logic;   -- indikerer samples ved linieskift
        last_lineframe :    in std_logic;       -- indikerer linieskift ved frame end
        new_delay :         in std_logic;
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
end genlock_timing;

-------------------------------------------------------------------------
--
--  Timing fra main_board :
--
--  f4m, f8g, fhm, fhm clockes paa hoejtgaaende f27-flanke
--
--  Tf8g                  = 64    us      positiv puls
--  Tf4m                  = 63,55 us      positiv puls
--  Tfhm og Tfhg = Tf27   = 37    ns      positiv puls
--
--                _   _   _   _
--  f27         _| |_| |_| |_| |_
--                    _____________          __________
--  f4m, f8g     ____|                ...              |___________
--                    ___                               ___
--  fhm, fhg     ____|   |_________   ...    __________|   |_______
--
-------------------------------------------------------------------------

architecture Behavioral of genlock_timing is

--attribute BUFG: string;
--attribute BUFG of f148: signal is "CLK";

component phasedelay_count
    Port(
        start_count :   in std_logic;
        phasedelay :    in std_logic_vector(23 downto 0);
        f148 :          in std_logic;
        sync_delayed :  out std_logic;
        -- debug :
        dlycnt_active : out std_logic;
        delaycount :    out integer range 0 to 16777215
        );
end component;

type state_define is ( wait_for_sync, startcounter_state, wait_for_sync_low );
signal state :              state_define;

signal temp_sync :          std_logic;  -- f8g eller f4m
signal intern_sync :        std_logic;  -- temp_sync synkron med f148
signal sync_delayed :       std_logic;  -- intern_sync + phasedelay
signal start_counter :      std_logic;  -- start til phasedelaycounter        
signal delay :              std_logic_vector(23 downto 0);
signal greset_unmask :      std_logic;  -- indikerer nyt phasedelay
signal genlock_window :     std_logic;
signal genlock_ok :         std_logic;

begin

-- debug : ----------------------
internsync <= intern_sync;
startcounter <= start_counter;
genlockok <= genlock_ok;
delay_cmp <= delay;
syncdelayed <= sync_delayed;
gresetunmask <= greset_unmask;
genlockwindow <= genlock_window;
---------------------------------

delay_cnt : phasedelay_count Port map(
    start_count     => start_counter,
    phasedelay      => phasedelay,
    f148            => f148,
    sync_delayed    => sync_delayed,
    -- debug :
    dlycnt_active   => dlycnt_active,
    delaycount      => delaycount
    );

--------------------------------------------------------------------------------
-- mainframesync f4m, f8g eller f4_free læses en halv f27-cycle efter flanke 
-- temp_sync laeses paa foerste hoejtgaaende f148-flanke herefter :
--              ____             ___________             _______
-- f27              |___________|           |___________| 
--                               _______________________________
-- f4m/f8g/f4_free  ____________|
--                                           ____________________
-- temp_sync    ____________________________|
--                                              _______________
-- intern_sync  _______________________________|
--                  _   _   _   _   _   _   _   _   _   _
-- f148           _| |_| |_| |_| |_| |_| |_| |_| |_| |_|

get_mainframesync : process( mreset, f27 )
-- laeser f4m eller f8g fra mainframe bussen
-- en halv 27MHz-clockcycle ( 18.52 ) ns efter denne saettes
begin
    if mreset = '0' then
        temp_sync <= '0';
    elsif f27'event and f27 = '0' then
        if sync_mode = "11" then
            temp_sync <= f4m;
        elsif sync_mode = "10" then
            temp_sync <= f8g;
        else
            temp_sync <= f4_free;
        end if;
    end if;
end process;

synchronize_to_148MHz : process( f148 )
-- temp_sync synkroniseres til 148 MHz clocken
begin
    if f148'event and f148 = '1' then
        intern_sync <= temp_sync;
    end if;
end process;

make_phasedelay_counter_start : process( mreset, f148, intern_sync )
-- start_counter er signal fra en oneshot-FF genereret af intern_sync
-- 1+1/2 f148 cycle efter intern_sync gaar hoej
begin
    if mreset = '0' then 
        start_counter <= '0';
        state <= wait_for_sync;
    elsif f148'event and f148 = '0' then
        case state is
            when wait_for_sync =>
                if intern_sync = '1' then
                    state <= startcounter_state;
                end if;
            when startcounter_state =>
                state <= wait_for_sync_low;
                start_counter <= '1';
            when wait_for_sync_low =>
                start_counter <= '0';
                if intern_sync = '0' then
                    state <= wait_for_sync;
                end if;
        end case;
    end if;
end process;

make_greset_unmask : process( new_delay, start_counter, phasedelay )
-- ved sync fra mainframe ( hoejtgaaende flanke paa start_counter ),
-- tjekker denne proces, om der er blevet skrevet ny vaerdi i phasedelay
-- greset_unmask ( active high ), tillader sync_delayed at resette generatoren
begin
    if new_delay = '1' then
        greset_unmask <= '0';
        delay(23) <= not phasedelay(23); -- ved naeste start_counter, er phasedelay /= delay
    elsif start_counter'event and start_counter = '1' then
        if ( delay /= phasedelay and cs = '1' ) or genlock_ok = '0' then
            -- der tjekkes paa cs=1, for ikke at komme til at saette greset_unmask,
            -- mens der skrives til videoregistrene
            greset_unmask <= '1';   -- greset 'unmaskes'
            delay <= phasedelay;    -- ved naeste start_counter, er delay = phasedelay
        else
            greset_unmask <= '0';   -- greset maskeres igen ved naeste start_counter
        end if;
    end if;
end process;

-- last_lineframe er hoej i sidste linie af sidste frame, hvor f4m, f8g  el freerun_sync
-- ( repraesenteret ved sync_delayed ) forventes laest igen 
-- og last_count er høj 10 eller 11 f148 cycles omkring hvert linieskift.
genlock_window <= not ( last_count and last_lineframe );

check_genlock : process( mreset, sync_delayed )
-- denne proces checker, om generatoren er synkron med sync_delayed
begin
    if mreset = '0' then
        genlock_ok <= '0';
    elsif sync_delayed'event and sync_delayed = '1' then
        if genlock_ok = '0' then
            genlock_ok <= '1';  -- der sync'es op til f4m eller f8g eller f4_free :
        elsif genlock_ok = '1' and genlock_window = '1' then
            genlock_ok <= '0';  -- genererer reset til trilevel timeren
        else
            null;               -- ingen reset ( genlock er OK )
        end if;
    end if;
end process;

make_genlock_reset : genlock_reset <= not ( greset_unmask and sync_delayed );

end Behavioral;
