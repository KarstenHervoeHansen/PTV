library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity genlock is
    port ( 
        mreset :                in std_logic;       -- Master Reset fra mainboard
        f27m :                  in std_logic;       -- 27 MHz masterclock fra mainboard
        f148 :                  in std_logic;       -- Output fra clock multiplier ( 148 MHz )

        f4m :                   in std_logic;       -- 4 x field rate       ( NTSC format )
        fhm :                   in std_logic;       -- Horisontal sync      ( NTSC - 625 linier pr frame )
        f8g :                   in std_logic;       -- 8 x field rate       ( PAL format )
        fhg :                   in std_logic;       -- Horisontal sync      ( PAL - 525 linier pr frame )

        run_mode :              in std_logic;       -- 0=freerunning eller 1=genlocked
        frame_freq :            in std_logic;

        samples_pr_line :       in integer range 0 to 8191;
        lines_pr_frame :        in integer range 0 to 4095;
        sample_count :          in integer range 0 to 8191;
        line_count :            in integer range 0 to 4095;

        phasedelay_reg :        in std_logic_vector(25 downto 0);
        greset :                inout std_logic;    -- reset til trileveltimer

        -- debug :
        syncok :                out std_logic
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

component phasedelay
    Port(
        start_count :       in std_logic;
        phasedelay_reg :    in std_logic_vector(25 downto 0);
        f148 :              in std_logic;
        f4m_delayed :       out std_logic
        );
end component;

type state_define is ( wait_for_sync, startcounter_state, wait_for_sync_low );
signal state :              state_define;

signal f4m_tmp :            std_logic;              -- intern 'kopi' af f4m clocket paa f27m
signal f4m_int :            std_logic;              -- f4m clocket paa f148
signal f4m_delayed :        std_logic;              -- f4m + phasedelay
signal sync_ok :            std_logic;              -- hoej hvis OK
signal frame_count :        integer range 0 to 7;
signal start_counter :      std_logic;

signal line_cond :              std_logic;
signal sample_cond :            std_logic;
signal frame_cond :             std_logic;
signal genlock_window :         std_logic;
signal window_count :           std_logic_vector(1 downto 0);
signal phasedelay_cond :        std_logic;

begin

-- debug : -----------
syncok <= sync_ok;
----------------------

phasedelay_counter : phasedelay Port map(
    start_count     => start_counter,
    phasedelay_reg  => phasedelay_reg,
    f148            => f148,
    f4m_delayed     => f4m_delayed
    );
        
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


get_f4m : process( mreset, f27m )
-- laeser f4m en halv 27MHz-clockcycle ( 18.52 ) ns efter denne saettes
begin
    if mreset = '0' then
        f4m_tmp <= '0';
    elsif f27m'event and f27m = '0' then
        f4m_tmp <= f4m;
    end if;
end process;


synchronize_NTSC_to_mainframe : process( f148 )
-- f4m synkroniseres til 148 MHz clocken
begin
    if f148'event and f148 = '1' then
        f4m_int <= f4m_tmp;
    end if;
end process;


frame_counter : process( greset, frame_freq )
begin
    if greset = '0' then
        frame_count <= 0;
    elsif frame_freq'event and frame_freq = '1' then
        if frame_count /= 1 then
            frame_count <= frame_count + 1;
        else            
            frame_count <= 0;            
        end if;
    end if;
end process;


-- frame_cond = 1 ved sidste frame inden f4m forventes
frame_cond <=   '1' when ( frame_count = 1 ) else
                '0';

-- line_cond = 1 ved sidste linie
line_cond  <=   '1' when ( line_count = lines_pr_frame ) else
                '0';

-- sample_cond = 1 ved sample = samples_pr_line - 3
sample_cond <=  '1' when ( sample_count = samples_pr_line - 3 ) else
                '0';

-- phasedelay_cond clockes paa 1ste hoejtgaaende f148 
-- ( er kun stabil paa denne flanke )
phasedelay_cond <= line_cond and frame_cond and sample_cond;

-- genlock_window angiver et tre f148-cycles langt vindue
-- ( f4m forventes laest inden for dette tidsrum )
make_genlock_window : process( phasedelay_cond, f148 )
begin
    if phasedelay_cond = '1' then
        window_count <= "00";
    elsif f148'event and f148 = '1' then
        case window_count is
            when "00" =>
                window_count <= "01";
                genlock_window <= '0';
            when "01" =>
                window_count <= "10";
                genlock_window <= '0';
            when "10" =>
                window_count <= "11";
                genlock_window <= '0';
            when others =>
                window_count <= "11";
                genlock_window <= '1';
        end case;
    end if;
end process;


make_phasedelay_counter_start : process( mreset, f148 )
begin
    if mreset = '0' then 
        start_counter <= '0';
        state <= wait_for_sync;
    elsif f148'event and f148 = '0' then
        case state is
            when wait_for_sync =>
                if f4m_int = '1' then
                    state <= startcounter_state;
                end if;

            when startcounter_state =>
                state <= wait_for_sync_low;
                start_counter <= '1';

            when wait_for_sync_low =>
                start_counter <= '0';
                if f4m_int = '0' then
                    state <= wait_for_sync;
                end if;
        end case;
    end if;
end process;


-- hvis f4m samples inden for genlock_window
-- bliver sync_ok sat lig 1 (OK)
-- f4m_delayed er 1 f148-cycle lang
make_syncok : process( mreset, f4m_delayed )
begin
    if mreset = '0' then
        sync_ok <= '0';
    elsif f4m_delayed'event and f4m_delayed = '1' then
        if sync_ok = '0' then
            sync_ok <= '1';
        elsif sync_ok = '1' and genlock_window = '0' then
            null;
        else
            sync_ok <= '0';
        end if;
    end if;
end process;

greset <= sync_ok;

end Behavioral;
