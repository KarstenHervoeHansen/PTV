library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity genlock is
    Port ( 
        reset :                 in std_logic;       -- Master Reset fra mainboard
        f27m :                  in std_logic;       -- 27 MHz masterclock fra mainboard
        f148 :                  in std_logic;       -- Output fra clock multiplier ( 148 MHz )

        f4m :                   in std_logic;       -- 4 x field rate       ( PAL format )
        fhm :                   in std_logic;       -- Horisontal sync      ( PAL - 525 linier pr frame )
        f8g :                   in std_logic;       -- 8 x field rate       ( NTSC format )
        fhg :                   in std_logic;       -- Horisontal sync      ( NTSC - 625 linier pr frame )

        run_mode :              in std_logic;       -- 0=freerunning eller 1=genlocked
        frame_freq :            in std_logic;

        samples_pr_line :       in integer range 0 to 8191;
        lines_pr_frame :        in integer range 0 to 4095;
        sample_count :          in integer range 0 to 8191;
        line_count :            in integer range 0 to 4095;

        tsg_start :             inout std_logic;    -- reset til trileveltimer

        -- debug :
        debugtoggler :          inout std_logic;
        syncok :                out std_logic;
        syncpulse :             out std_logic;
        f4mint :                out std_logic
        );
end genlock;

-------------------------------------------------------------------------
--
--  Timing fra main_board :
--
--  f4m, f8g, fhm, fhm clockes paa hoejtgaaende f27m-flanke
--
--  Tf8g                  = 64    us      positiv puls
--  Tf4m                  = 63,55 us      positiv puls
--  Tfhm og Tfhg = Tf27   = 37    ns      positiv puls
--
--                _   _   _   _
--  f27m        _| |_| |_| |_| |_
--                   _____________          __________
--  f4m, f8g    ____|                ...              |___________
--                   ___                               ___
--  fhm, fhg    ____|   |_________   ...    __________|   |_______
--
-------------------------------------------------------------------------

architecture Behavioral of genlock is

type state_define is ( wait_for_sync, wait_for_counter, pulse, wait1, wait2, wait_for_sync_low );
signal state :              state_define;

signal f4m_tmp :            std_logic;              -- intern 'kopi' af f4m clocket paa f27m
signal f4m_int :            std_logic;              -- f4m clocket paa f148
signal sync_ok :            std_logic;              -- hoej hvis OK
signal sync_pulse :         std_logic;              -- sync til TrilevelSyncGeneratorer
signal frame_count :        integer range 0 to 7;

--signal last_sample :        std_logic;              -- internt synkroniseringssignal
--signal new_line :           std_logic;              -- hoejtgaaende flanke indikerer ny linie
--signal new_field :          std_logic;              -- hoejtgaaende flanke indikerer ny frame/field
--signal sample_count :       integer range 0 to 8191;    
--signal line_count :         integer range 0 to 4095;

-- signal samples_pr_line :    integer range 0 to 8191 := 10;     -- debug
-- signal lines_pr_frame :     integer range 0 to 4095 := 8;      -- debug
-- signal run_mode :           std_logic := '1';                  -- debug


begin

-- debug :
syncok <= sync_ok;
syncpulse <= sync_pulse;
f4mint <= f4m_int;
        
--------------------------------------------------------------------
-- f4m laeses en halv f27-cycle efter f4m gaar hoej (f4m_tmp)
-- f4m_tmp laeses paa foerste hoejtgaaende f148-flanke herefter :
--
--              ____             ___________             _______
-- f27m             |___________|           |___________| 
--                               _______________________________
-- f4m          ________________|
--                                           ____________________
-- f4m_tmp      ____________________________|
--                                              _______________
-- f4m_int      _______________________________|
--                  _   _   _   _   _   _   _   _   _   _
-- f148           _| |_| |_| |_| |_| |_| |_| |_| |_| |_|
--              _______________________________     __________
-- sync_pulse                                  |___|


get_f4m : process( reset, f27m )
-- laeser f4m en halv 27MHz-clockcycle ( 18.52 ) ns efter denne saettes
begin
    if reset = '0' then
        f4m_tmp <= '0';
    elsif f27m'event and f27m = '0' then
        f4m_tmp <= f4m;
    end if;
end process;


synchronize_PAL_to_mainframe : process( f148 )
-- synkroniserer f4m til den interne 148 MHz clock
begin
    if f148'event and f148 = '1' then
        f4m_int <= f4m_tmp;
    end if;
end process;


sync_statemachine : process( reset, f4m_int, f148 )
-- 4 state statemaskine : 
--
-- state wait_for_sync - vent paa f4m_int
--
-- state pulse - generer syncpuls og gaa til wait_for_counter hvis sync_ok 
-- og wait_for_sync_low hvis /sync_ok
--
-- state wait_for_counter - gaa til pulse hvis counter = 0
--
-- state wait_for_sync_low - vent paa /f4m, gaa til state 1 naar f4m = 0
begin
    if reset = '0' then
        state <= wait_for_sync;
        sync_pulse <= '0';
        tsg_start <= '1';
        debugtoggler <= '0';
    elsif ( f148'event and f148 = '0' ) and ( run_mode = '1' ) then
        case state is

            when wait_for_sync =>
                if f4m_int = '1' then
                    state <= pulse;                    
                    sync_pulse <= '0';
                    tsg_start <= '0';
                end if;

            when pulse =>
                state <= wait1;
                tsg_start <= '1';

            when wait1 =>
                state <= wait2;

            when wait2 =>
                state <= wait_for_counter;
                sync_pulse <= '1';

            when wait_for_counter =>
                if sync_ok = '1' then  
--                    if ( frame_count = 1 ) and ( line_count = lines_pr_frame ) and ( sample_count = samples_pr_line - 2 ) then
                    if  sample_count = (samples_pr_line - 2) then   -- debug
                        state <= pulse;
                        sync_pulse <= '0';
                        debugtoggler <= not debugtoggler;
                    else
                        null;
                    end if;
                else
                    state <= wait_for_sync_low;
                end if;

            when wait_for_sync_low =>
                if f4m_int = '0' then
                    state <= wait_for_sync;
                else
                    null;
                end if;

        end case;
    end if;
end process;


frame_counter : process( tsg_start, frame_freq )
begin
    if tsg_start = '0' then
        frame_count <= 0;
    elsif frame_freq'event and frame_freq = '1' then
        if frame_count /= 1 then
            frame_count <= frame_count + 1;
        else
            frame_count <= 0;            
        end if;
    end if;
end process;


check_genlock : process( tsg_start, f4m_int )
begin
    if tsg_start = '0' then
        sync_ok <= '1';
    elsif f4m_int'event and f4m_int = '1' then
        if sync_pulse = '0' then
            sync_ok <= '1';
        else
            sync_ok <= '0';
        end if;        
    end if;
end process;

end Behavioral;
