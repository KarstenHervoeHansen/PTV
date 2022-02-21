library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity genlock_timing is
    port ( 
        mreset :            in std_logic;   -- master reset
        f27m :              in std_logic;   -- 27 MHz master clock
        f148 :              in std_logic;   -- 148 MHz mux'ed clock
        f4m :               in std_logic;   -- sync reference for 625 line compliant formats
        f8g :               in std_logic;   -- sync reference for 525 line compliant formats
        scan_method :       in std_logic;   -- progressiv=0, interlaced=1
        sync_mode :         in std_logic_vector(1 downto 0);    -- 0X=freerunning, 10=genlocked to f4m, 11= genlocked to f8g
        phasedelay :        in std_logic_vector(25 downto 0);   -- delay from sync reference
        last_count :        in std_logic;   -- indikerer samples ved linieskift
        last_lineframe :    in std_logic;       -- indikerer linieskift ved frame end
        greset :            inout std_logic;    -- reset til trileveltimer
        -- debug :
        internsync :        out std_logic;
  --      startcounter :      out std_logic;
    --    delaycount :        out integer range 0 to 67108863;
      --  dlycnt_active :     out std_logic;
        syncdelayed :       out std_logic;
        windowcond :        out std_logic;
        windowcount :       out std_logic_vector(3 downto 0);
        genlockwindow :     out std_logic
        );
end genlock_timing;

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
        phasedelay :    in std_logic_vector(25 downto 0);
        f148 :          in std_logic;
        sync_delayed :  out std_logic
        -- debug :
--        dlycnt_active : out std_logic;
  --      delaycount :    out integer range 0 to 67108863
        );
end component;

type state_define is ( wait_for_sync, startcounter_state, wait_for_sync_low );
signal state :              state_define;

signal temp_sync :          std_logic;  -- f8g eller f4m
signal intern_sync :        std_logic;  -- temp_sync synkron med f148
signal sync_delayed :       std_logic;  -- intern_sync + phasedelay
signal start_counter :      std_logic;  -- start til phasedelaycounter        
signal wincount :           std_logic_vector(3 downto 0); -- bruges til at generere genlock-window
signal window_cond :        std_logic;  -- 
signal genlock_window :     std_logic;
signal phasedelay_cond :    std_logic;

begin

-- debug : ----------------------
internsync <= intern_sync;
--startcounter <= start_counter;
syncdelayed <= sync_delayed;
windowcond <= window_cond;
windowcount <= wincount;
genlockwindow <= genlock_window;
---------------------------------

delay_cnt : phasedelay_count Port map(
    start_count     => start_counter,
    phasedelay      => phasedelay,
    f148            => f148,
    sync_delayed    => sync_delayed
    -- debug :
--    dlycnt_active   => dlycnt_active,
  --  delaycount      => delaycount
    );
        

--------------------------------------------------------------------------------
-- mainframesync f4m eller f8g laeses en halv f27-cycle efter flanke (temp_sync)
-- temp_sync laeses paa foerste hoejtgaaende f148-flanke herefter :
--              ____             ___________             _______
-- f27m             |___________|           |___________| 
--                               _______________________________
-- f4m/f8g      ________________|
--                                           ____________________
-- temp_sync    ____________________________|
--                                              _______________
-- intern_sync  _______________________________|
--                  _   _   _   _   _   _   _   _   _   _
-- f148           _| |_| |_| |_| |_| |_| |_| |_| |_| |_|


get_mainframesync : process( mreset, f27m )
-- laeser f4m eller f8g fra mainframe bussen
-- en halv 27MHz-clockcycle ( 18.52 ) ns efter denne saettes
begin
    if mreset = '0' then
        temp_sync <= '0';
    elsif f27m'event and f27m = '0' then
        if sync_mode(0) = '0' then
            temp_sync <= f4m;
        else
            temp_sync <= f8g;
        end  if;
    end if;
end process;


synchronize_to_trilevelgenerator : process( f148 )
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


-- last_lineframe er hoej i sidste linie af sidste frame, hvor f4m el f8g
-- ( repraesenteret ved sync_delayed ) forventes laest igen
window_cond <= last_count and last_lineframe;


genlock_window <= not window_cond;


make_greset : process( mreset, sync_delayed )
begin
    if mreset = '0' then
        greset <= '0';
    elsif sync_delayed'event and sync_delayed = '1' then
        if greset = '0' and sync_mode(1) = '1' then
            -- der sync'es op til f4m eller f8g :
            greset <= '1';
        elsif greset = '1' and genlock_window = '0' then
            -- ingen reset ( sync = OK )
            null;
        else
            -- reset TSG :
            greset <= '0';
            -- debug :
--            null;       -- null her goer at der kun laases op 1 gang.
        end if;
    end if;
end process;


end Behavioral;


-- old code :

--make_wincount : process( window_cond, f148, scan_method)
-- wincount er en counter, der taeller f148 cycles i den spldiv_downcount
-- hvor sync_delayed forventes laest.
-- ved progressiv scanning taelles fra 1, for at genlock_window falder samme
-- sted i forhold til slutningen af den paagaeldende frame.
--begin
  --  if window_cond = '0' then
    --    if scan_method = '1' then
      --      wincount <= "0000";    -- interlaced scan
        --else
          --  wincount <= "0001";    -- interlaced scan
--        end if;            
  --  elsif f148'event and f148 = '1' then
    --    wincount(0) <= not wincount(0);
      --  if wincount(0) = '1' then
        --    wincount(1) <= not wincount(1);
--        end if;
  --      if wincount(1 downto 0) = "11" then
    --        wincount(2) <= not wincount(2);
      --  end if;
        --if wincount(2 downto 0) = "111" then
          --  wincount(3) <= not wincount(3);
--        end if;
  --  end if;
--end process;

--make_genlockwindow : process( window_cond, f148 )
-- genlock_window er et 3 samples bredt vindue
-- hvis sync fra mainframe bussen ikke falder inden for dette vindue 
-- bliver generatoren resat og der ventes paa en ny f4m- eller f8g-puls
--begin
  --  if window_cond = '0' then
    --    genlock_window <= '1';
--    elsif f148'event and f148 = '0' then
  --      case wincount is
    --        when "0010" =>
      --          genlock_window <= '0';
        --    when "1101" =>
          --      genlock_window <= '1';
            --when others =>
--                null;
  --      end case;
    --end if;
--end process;

