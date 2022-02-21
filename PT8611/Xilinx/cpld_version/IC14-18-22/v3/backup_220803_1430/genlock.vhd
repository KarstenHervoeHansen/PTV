library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity genlock is
    Port ( 
        reset :                 in std_logic;       -- Master Reset fra mainboard
        f27m :                  in std_logic;       -- 27 MHz masterclock fra mainboard
        f148 :                  in std_logic;       -- Output fra clock multiplier ( 148 MHz )

        f4m :                   in std_logic;       -- 1/8 x Vertikal sync  ( PAL format )
        fhm :                   in std_logic;       -- Horisontal sync      ( PAL - 525 linier pr frame )
        f8g :                   in std_logic;       -- 1/4 x Vertikal sync  ( NTSC format )
        fhg :                   in std_logic;       -- Horisontal sync      ( NTSC - 625 linier pr frame )

        run_mode :              in std_logic;       -- 0=freerunning eller 1=genlocked
        samples_pr_line :       in integer range 0 to 8191;
        lines_pr_frame :        in integer range 0 to 4095;
        sample_count :          in integer range 0 to 8191;
        line_count :            in integer range 0 to 4095;

        tsg_start :             inout std_logic     -- reset til trileveltimer
        );
end genlock;

-------------------------------------------------------------------------
--
--  Timing fra main_frame :
--
--  f4m, f8g, fhm, fhm clockes paa hoejtgaaende f27m-flanke
--
--  Tf8g                 = 64    us      positiv puls
--  Tf4m                 = 63,55 us      positiv puls
--  Tfhm og Tfhg = Tf27   = 37    ns      positiv puls
--
--               _   _   _   _
--  f27m        _| |_| |_| |_| |_
--                   _____________          __________
--  f4m, f8g    ____|                ...              |___________
--                   ___                               ___
--  fhm, fhg    ____|   |_________   ...    __________|   |_______
--
-------------------------------------------------------------------------


architecture Behavioral of genlock is

component check_genlock
    Port(
        reset :             in std_logic;
        f148 :              in std_logic;
        f4m_int :           in std_logic;
        sync_pulse :        in std_logic;
        sync_ok :           out std_logic
        );
end component;


type state_define is ( wait_for_sync, pulse, wait_for_counter, wait_for_sync_low );
signal state :              state_define;

signal f4m_tmp :            std_logic;              -- intern 'kopi' af f4m clocket paa f27m
signal f4m_int :            std_logic;              -- f4m clocket paa f148
signal sync_ok :            std_logic;              -- hoej hvis OK
signal sync_pulse :         std_logic;              -- sync til TrilevelSyncGeneratorer
signal last_sample :        std_logic;              -- internt synkroniseringssignal
signal new_line :           std_logic;              -- hoejtgaaende flanke indikerer ny linie
signal new_field :          std_logic;              -- hoejtgaaende flanke indikerer ny frame/field
--signal sample_count :       integer range 0 to 8191;    
--signal line_count :         integer range 0 to 4095;
signal field_count :        integer range 0 to 7;

-- signal samples_pr_line :    integer range 0 to 8191 := 10;     -- debug
-- signal lines_pr_frame :     integer range 0 to 4095 := 8;      -- debug
-- signal run_mode :           std_logic := '1';                  -- debug

begin

genlock_check : check_genlock port map (
    reset       => reset,
    f148        => f148, 
    f4m_int     => f4m_int,
    sync_pulse  => sync_pulse,
    sync_ok     => sync_ok
    );

--------------------------------------------------------------------
-- f4m laeses en halv f27-cycle efter f4m gaar hoej (f4m_tmp)
-- f4m_tmp laeses paa foerste hoejtgaaende f148-flanke herefter :
--
--              ____             ___________             _______
-- f27m              |___________|           |___________| 
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
        sync_pulse <= '1';
        tsg_start <= '1';
    elsif ( f148'event and f148 = '1' ) and ( run_mode = '1' ) then
        case state is

            when wait_for_sync =>
                if f4m_int = '1' then
                    state <= pulse;                    
                    sync_pulse <= '0';
                    tsg_start <= '0';   -- tsg_start er reset til Tri_Level_Timer (forekommer kun ved f4m)
                end if;

            when pulse =>
                state <= wait_for_counter;
                sync_pulse <= '1';
                tsg_start <= '1';

            when wait_for_counter =>
                if sync_ok = '1' then  
                    if ( field_count = 3 ) and ( sample_count = samples_pr_line ) and ( line_count = lines_pr_frame ) then
                        state <= pulse;
                        sync_pulse <= '0';
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

--sample_counter : process(tsg_start, f148)
--begin
--    if tsg_start = '0' then
--        sample_count <= 0;
 --       new_line <= '0';
   -- elsif (f148'event and f148 = '1') then
     --   if sample_count = ( samples_pr_line ) then
       --     sample_count <= 0;
         --   new_line <= '1';
--        else
--	        sample_count <= (sample_count + 1);
  --          if (sample_count >= 1024) and (sample_count <= 2047) then
--  --          if (sample_count >= 4) and (sample_count <= 7) then -- debug
      --          new_line <= '0';
        --    end if;
--		end if;
--	end if;		
--end process;

--line_counter : process( tsg_start, new_line )
--begin
--    if tsg_start = '0' then
  --      line_count <= 0;
    --    new_field <= '0';
--    elsif new_line'event and new_line = '1' then
  --      if line_count /= (lines_pr_frame ) then
	--	    line_count <= line_count + 1;
      --      if (line_count >= 256) and (line_count <= 511) then
--      --      if ( line_count >= 4 ) and  ( line_count <= 7 ) then  -- debug
          --      new_field <= '0';
            --end if;
--        else
--		    line_count <= 0;
  --          new_field <= '1';       -- indikerer nyt field
	--	end if;
--	end if;
--end process;    

-- NB : husk at tage hoejde for baade interlaced og progressiv
field_frame_counter : process( tsg_start, new_field )
begin
    if tsg_start = '0' then
        field_count <= 0;
    elsif new_field'event and new_field = '1' then
        if field_count /= 3 then
            field_count <= field_count + 1;
        else
            field_count <= 0;            
        end if;
    end if;
end process;



end Behavioral;
