library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;

------------------------------------------------------------------------------------------------
-- Toplevel for HD generator module PT8612
-- 2005-09-26: Created                    Thomas Holm Hansen
--
------------------------------------------------------------------------------------------------

entity HD_Gen_Module is
   Port (
      --------- from Mainframe -----------------------------------------------------------------
      mreset_i    : in std_logic;      -- Master Reset fra mainboard
      f1484_i     : in std_logic;      -- 148.35 MHz clock
      f1485_i     : in std_logic;      -- 148.5 MHz clock
      f4m_i       : in std_logic;      -- Genlock syncronization for NTSC (4 field period)
      f8g_i       : in std_logic;      -- Genlock syncronization for PAL (8 field period)

      --------- from Controller ----------------------------------------------------------------
      sck_i       : in std_logic;      -- serial interface clock
      mosi_i      : in std_logic;      -- Master Out Serial data In
      cs1_i       : in std_logic;      -- chip select channel 1 (nss)
      cs2_i       : in std_logic;      -- chip select channel 2 (port 2 bit 5)
      cs3_i       : in std_logic;      -- chip select channel 3 (port 1 bit 7)
      cs4_i       : in std_logic;      -- chip select channel 4 (port 1 bit 6)

      --------- to Controller ------------------------------------------------------------------
      res1_o      : out std_logic;     -- generator 1&2 output levels OK - respons til mainframe
      res2_o      : out std_logic;     -- generator 3&4 output levels OK - respons til mainframe

      --------- HD Outputs ---------------------------------------------------------------------
      --SDI_p_1_o   : out std_logic;
      --SDI_n_1_o   : out std_logic;
      
      --------- LED's --------------------------------------------------------------------------
      led1_o      : out std_logic;
      led2_o      : out std_logic;
      led3_o      : out std_logic;
      led4_o      : out std_logic;

      --------- Debug --------------------------------------------------------------------------
      debug_o     : out std_logic_vector(9 downto 0)
   );
end HD_Gen_Module;


architecture Behavioral of HD_Gen_Module is


signal sync_2398,sync_24,sync_30 : std_logic;

signal f4m_1484_clean            : std_logic;
signal f4m_1484_genlock_ok       : std_logic;
signal f4m_1484_genlock_resync   : std_logic;
signal f4m_1485_clean            : std_logic;
signal f4m_1485_genlock_ok       : std_logic;
signal f4m_1485_genlock_resync   : std_logic;
signal f8g_clean                 : std_logic;
signal f8g_genlock_ok            : std_logic;
signal f8g_genlock_resync        : std_logic;
signal genlock_resync            : std_logic;
signal reset_delayed             : std_logic;
signal f1484                     : std_logic;
signal f1485                     : std_logic;
signal tick_10ms                 : std_logic;

signal timing_change_ch1         : std_logic;
--signal timing_change_ch2         : std_logic;
--signal timing_change_ch3         : std_logic;
--signal timing_change_ch4         : std_logic;
signal timing_change             : std_logic;
signal all_genlock_ok            : std_logic;
signal debug							: std_logic_vector(9 downto 0);


--Constants:
constant f8g_count1              : integer := 3375;  -- used to genlock to f8g sync
constant f8g_count2              : integer := 7040;  -- 3375*7040 = 23760000
constant f8g_bits1               : integer := 12;    -- size of counter 1
constant f8g_bits2               : integer := 13;    -- size of counter 2

constant f4m_1484_count1         : integer := 2750;  -- used to genlock to f4m sync (on 1484 clk)
constant f4m_1484_count2         : integer := 3600;  -- 2750*3600 = 9900000
constant f4m_1484_bits1          : integer := 12;    -- size of counter 1
constant f4m_1484_bits2          : integer := 12;    -- size of counter 2

constant f4m_1485_count1         : integer := 3146;  -- used to genlock to f4m sync (on 1485 clk)
constant f4m_1485_count2         : integer := 3150;  -- 3146*3150 = 9909900
constant f4m_1485_bits1          : integer := 12;    -- size of counter 1
constant f4m_1485_bits2          : integer := 12;    -- size of counter 2

constant sync2398_count1         : integer := 2250;  -- used to generate 23,98Hz sync
constant sync2398_count2         : integer := 2750;  -- 2250*2750 = 6187500
constant sync2398_bits1          : integer := 12;    -- size of counter 1
constant sync2398_bits2          : integer := 12;    -- size of counter 2

constant sync24_count1           : integer := 2250;  -- used to generate 24Hz sync
constant sync24_count2           : integer := 2750;  -- 2250*2750 = 6187500
constant sync24_bits1            : integer := 12;    -- size of counter 1
constant sync24_bits2            : integer := 12;    -- size of counter 2

constant sync30_count1           : integer := 1375;  -- used to generate 30Hz sync
constant sync30_count2           : integer := 3600;  -- 1375*3600 = 4950000
constant sync30_bits1            : integer := 12;    -- size of counter 1
constant sync30_bits2            : integer := 12;    -- size of counter 2


component master_reset_delay
   port (
      mreset_i             : in std_logic;           -- Master reset
      clk_i                : in std_logic;           -- sample clock
      tick_10ms_o          : out std_logic;
      reset_delayed_o      : out std_logic
   );
end component;


component period_dual_count
   generic(
      count_val1           : integer;
      count_val2           : integer;
      bits1                : integer;
      bits2                : integer
   );
   Port(
      mreset_i             : in std_logic;
      clk_i                : in std_logic;
      sync_reset_i         : in std_logic;
      sync_o               : out std_logic
   );
end component;


component sync_genlock_regen
   generic (
      count_val1           : integer;        -- the period of sync is count_val1*count_val2
      count_val2           : integer;
      bits1                : integer;        -- number of bits in counters
      bits2                : integer
   );                      
   port (                  
      mreset_i             : in std_logic;
      clk_i                : in std_logic;
      sync_i               : in std_logic;
      sync_clean_o         : out std_logic;
      genlock_ok_o         : out std_logic
   );
end component;


component HD_Gen_Channel
   Port (
      mreset_i          : in std_logic;      -- Master Reset fra mainboard
      reset_delayed_i   : in std_logic;
      f1484_i           : in std_logic;
      f1485_i           : in std_logic;
      tick_10ms_i       : in std_logic;
      f4m_i             : in std_logic;
      f8g_i             : in std_logic;
      f2398_i           : in std_logic;
      f24_i             : in std_logic;
      f30_i             : in std_logic;
      sck_i             : in std_logic;      -- serial interface clock
      mosi_i            : in std_logic;      -- Master Out Serial data In
      cs_i              : in std_logic;      -- chip select
      SDI_p_o           : out std_logic;
      SDI_n_o           : out std_logic;
      timing_change_o   : out std_logic;
      debug_o           : out std_logic_vector(9 downto 0)
   );
end component;


component BUFG
   Port (
      I : in  std_logic;
      O : out std_logic
   );
end component;


component led_latch
   port (
      led_i          : in std_logic;
      clk_i          : in std_logic;
      tick_10ms_i    : in std_logic;
      led_o          : out std_logic
   );
end component;


begin


--------------------------------------------------------------
-- Master reset delay and 10ms tick generation:
--------------------------------------------------------------
master_reset_delaying : master_reset_delay
port map(
   mreset_i          => mreset_i,
   clk_i             => f1485,
   tick_10ms_o       => tick_10ms,
   reset_delayed_o   => reset_delayed
);


--------------------------------------------------------------
-- Use BUFG for clock distribution:
--------------------------------------------------------------
f1484_i_distribution : BUFG
port map(
   I => f1484_i,
   O => f1484
);

f1485_i_distribution : BUFG
port map(
   I => f1485_i,
   O => f1485
);


--------------------------------------------------------------
-- Generate free running sync pulses with a frequency of 23.98 Hz
--------------------------------------------------------------
sync2398_generation : period_dual_count
generic map (
   count_val1 => sync2398_count1,
   count_val2 => sync2398_count2,
   bits1      => sync2398_bits1,
   bits2      => sync2398_bits2
)
port map (
   mreset_i     => mreset_i,
   clk_i        => f1484,
   sync_reset_i => '0',
   sync_o       => sync_2398
);


--------------------------------------------------------------
-- Generate free running sync pulses with a frequency of 24 Hz
--------------------------------------------------------------
sync24_generation : period_dual_count
generic map (
   count_val1 => sync24_count1,
   count_val2 => sync24_count2,
   bits1      => sync24_bits1,
   bits2      => sync24_bits2
)
port map (
   mreset_i     => mreset_i,
   clk_i        => f1485,
   sync_reset_i => '0',
   sync_o       => sync_24
);


--------------------------------------------------------------
-- Generate free running sync pulses with a frequency of 30 Hz
--------------------------------------------------------------
sync30_generation : period_dual_count
generic map (
   count_val1 => sync30_count1,
   count_val2 => sync30_count2,
   bits1      => sync30_bits1,
   bits2      => sync30_bits2
)
port map (
   mreset_i     => mreset_i,
   clk_i        => f1485,
   sync_reset_i => '0',
   sync_o       => sync_30
);


--------------------------------------------------------------
-- Genlock to the mainframe f4m signal
-- for 30/1.001 and 60/1.001 HD systems
--------------------------------------------------------------
f4m_genlock_1484_regen : sync_genlock_regen
   generic map (
      count_val1       => f4m_1484_count1,
      count_val2       => f4m_1484_count2,
      bits1            => f4m_1484_bits1,
      bits2            => f4m_1484_bits2
   )
   port map (
      mreset_i         => reset_delayed,
      clk_i            => f1484,
      sync_i           => f4m_i,
      sync_clean_o     => f4m_1484_clean,
      genlock_ok_o     => f4m_1484_genlock_ok
   );

   
--------------------------------------------------------------
-- Genlock to the mainframe f4m signal
-- for SD 525 (NTSC) system
--------------------------------------------------------------
f4m_genlock_1485_regen : sync_genlock_regen
   generic map (
      count_val1       => f4m_1485_count1,
      count_val2       => f4m_1485_count2,
      bits1            => f4m_1485_bits1,
      bits2            => f4m_1485_bits2
   )
   port map (
      mreset_i         => reset_delayed,
      clk_i            => f1484,
      sync_i           => f4m_i,
      sync_clean_o     => f4m_1485_clean,
      genlock_ok_o     => f4m_1485_genlock_ok
   );   
   

--------------------------------------------------------------
-- Genlock to the mainframe f8g signal for 25 and 50 Hz systems
--------------------------------------------------------------
f8g_genlock_regen : sync_genlock_regen
   generic map (
      count_val1       => f8g_count1,
      count_val2       => f8g_count2,
      bits1            => f8g_bits1,
      bits2            => f8g_bits2
   )
   port map (
      mreset_i         => reset_delayed,
      clk_i            => f1485,
      sync_i           => f8g_i,
      sync_clean_o     => f8g_clean,
      genlock_ok_o     => f8g_genlock_ok
   );


--------------------------------------------
-- Generator channel 1
--------------------------------------------
HD_Gen_Channel_1 : HD_Gen_channel
   Port map (
      mreset_i          => mreset_i,
      reset_delayed_i   => reset_delayed,
      f1484_i           => f1484,
      f1485_i           => f1485,
      tick_10ms_i       => tick_10ms,
      f4m_1484_i        => f4m_1484_clean,
      f4m_1485_i        => f4m_1485_clean,
      f8g_i             => f8g_clean,
      f2398_i           => sync_2398,
      f24_i             => sync_24,
      f30_i             => sync_30,
      sck_i             => sck_i,
      mosi_i            => mosi_i,
      cs_i              => cs1_i,
      --SDI_p_o           => SDI_p_1_o,
      --SDI_n_o           => SDI_n_1_o,
      timing_change_o   => timing_change_ch1,
      debug_o           => debug
   );


-------------------------------------------------------------
-- Monitors the timing changes and genlock:
-------------------------------------------------------------
timing_change     <= timing_change_ch1;  -- or timing_change_ch2 or timing_change_ch3 or timing_change_ch4;
all_genlock_ok    <= f4m_1484_genlock_ok and f4m_1485_genlock_ok and f8g_genlock_ok;


-------------------------------------------------------------
-- Led latches:
-------------------------------------------------------------
--led_latch1: led_latch
--   port map (
--      led_i       => timing_change,
--      clk_i       => f1485,
--      tick_10ms_i => tick_10ms,
--      led_o       => led1_o
--   );
--
--led_latch2: led_latch
--   port map (
--      led_i       => not all_genlock_ok,
--      clk_i       => f1485,
--      tick_10ms_i => tick_10ms,
--      led_o       => led2_o
--   );
--
--led_latch3: led_latch
--   port map (
--      led_i       => '1',
--      clk_i       => f1485,
--      tick_10ms_i => tick_10ms,
--      led_o       => led3_o
--   );
--
--led_latch4: led_latch
--   port map (
--      led_i       => '1',
--      clk_i       => f1485,
--      tick_10ms_i => tick_10ms,
--      led_o       => led4_o
--   );   
   
-------------------------------------------------------------
-- Mapping of unused signals:
-------------------------------------------------------------

res1_o <= '1';
res2_o <= '1';
   
led1_o <= debug(0);
led2_o <= debug(1);
led3_o <= debug(2);
led4_o <= debug(3);

debug_o	<= debug;

end Behavioral;