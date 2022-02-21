library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
library UNISIM;
use UNISIM.VComponents.all;

-----------------------------------------------------------------------------------------------
-- v1:      Original kode til Prototype lavet af PEH + studerende Jesper Christoffersen
--          for DK-Audio, som afgangsprojekt november 2002 til marts 2003
-- v2:      19/6 2003 - PS      tilfoejet clamping- og enablesignaler til rekonstruktionsfiltre
-- v3:      23/5 2003 - PS      nyt hieraki => toplevel = Tri_level_sync_generator.vhd
-- v301 :   21/8 2003 - PS      ny komponent sync_statemachine og state_timer
-- v4 :     8/9  2003 - PS      nyt printudlaeg m 100pin CPLD => ny pin-konfiguration
--                                                               + opsplitning af software
-- v5:      23/12 2003 - PS     nyt printudlæg m Spartan3 FPGA
-- v501:    21/04 2004 - ps     ny sync_statemachine ( pulsetype 1 og 2 )
-- v502:    26/01 2005 - PS     ny komponent freerunning_reference, der genererer intern
--                              vertikal referencesignal til 23.98, 24, 30 & 60Hz formater
-- v503:    10/02 2005 - PS     freerunning_reference komponenten, benytter f148 istedet for
--                              f27 pga. crosstalk fra f148, der forstyrrer tællerne i kom-
--                              ponenten.
-- v503 no27: 23/05 05 - PS     27 MHz clocken er fjernet fra designet, som forsøg på at
--                              ordne crosstalkproblem på GCK inputs.
--                              NB: delay=0 svarer nu til, at outputtet ligger ca. 3 148MHz
--                              samples før
-- v503 genlock monitor
--          01-06-2005 - thh    Only genlock cirquit implemented
-----------------------------------------------------------------------------------------------

entity Tri_Level_Module is
   Port (
      --------- from Mainframe -------------------------------------------------------------------
      mreset_i    : in std_logic;      -- Master Reset fra mainboard
      f1484_i     : in std_logic;      -- 148.35 MHz clock
      f1485_i     : in std_logic;      -- 148.5 MHz clock
      f4m_i       : in std_logic;      -- Vertikal sync reference - 625 lines SD format
      f8g_i       : in std_logic;      -- Vertikal sync reference - 525 lines SD format

      --------- from Controller -------------------------------------------------------------------------
      sck_i       : in std_logic;      -- serial interface clock
      mosi_i      : in std_logic;      -- Master Out Serial data In
      cs1_i       : in std_logic;      -- chip select tsg1 (nss)
      cs2_i       : in std_logic;      -- chip select tsg2 (port 2 bit 5)
      cs3_i       : in std_logic;      -- chip select tsg3 (port 1 bit 7)
      cs4_i       : in std_logic;      -- chip select tsg4 (port 1 bit 6)

      --------- to Controller -------------------------------------------------------------------------
      res1_o      : out std_logic;     -- generator 1&2 output levels OK - respons til mainframe
      res2_o      : out std_logic;     -- generator 3&4 output levels OK - respons til mainframe

      --------- TSG outputs --------------------------------------------------------------------------
      tsg1_o      : out std_logic_vector(3 downto 0);   -- 4 bit til DA converter
      tsg2_o      : out std_logic_vector(3 downto 0);
      tsg3_o      : out std_logic_vector(3 downto 0);
      tsg4_o      : out std_logic_vector(3 downto 0);

      --------- til rekonstruktions filtre ----------------------------------------------
      clmp1_o     : out std_logic;      -- til clamping puls - benyttes ikke
      clmp2_o     : out std_logic;
      clmp3_o     : out std_logic;
      clmp4_o     : out std_logic;

      --------- Output level check -------------------------------------------------------------------
      tsg1_lvl_i  : in std_logic;        -- signals from comparators
      tsg2_lvl_i  : in std_logic;
      tsg3_lvl_i  : in std_logic;
      tsg4_lvl_i  : in std_logic;
      
      --------------------------------------------------------------------------------
      p0_i        : in std_logic_vector(7 downto 2); -- port 0 [7:2]
      p1_i        : in std_logic_vector(5 downto 0); -- port 1 [5:0]

      --------- testpoints -------------------------------------------------------
      led1_o      : out std_logic;     -- forbundet til en noconnect på xc3s50
      led2_o      : out std_logic;     -- forbundet til en noconnect på xc3s50
      led3_o      : out std_logic

--      ext0_o      : out std_logic_vector(3 downto 0);     -- j5 pin 1
--      ext1_o      : out std_logic_vector(3 downto 0);     -- j5 pin 2
--      ext2_o      : out std_logic_vector(3 downto 0);     -- j5 pin 3
--      ext3_o      : out std_logic_vector(3 downto 0)      -- j5 pin 4
   );

end Tri_Level_Module;


architecture Behavioral of Tri_Level_Module is


signal tsg1_ok,tsg2_ok,tsg3_ok,tsg4_ok    : std_logic;  -- low pulse when level error
signal sync_2398,sync_24,sync_30          : std_logic;

signal f4m_clean                 : std_logic;
signal f4m_genlock_ok            : std_logic;
signal f4m_genlock_resync        : std_logic;
signal f8g_clean                 : std_logic;
signal f8g_genlock_ok            : std_logic;
signal f8g_genlock_resync        : std_logic;
signal genlock_resync            : std_logic;
signal reset_delayed             : std_logic;
signal f1484                     : std_logic;
signal f1485a                    : std_logic;
signal f1485b                    : std_logic;
signal f1485c                    : std_logic;
signal tick_10ms	 	      		: std_logic;

signal timing_change_ch1         : std_logic;
signal timing_change_ch2         : std_logic;
signal timing_change_ch3         : std_logic;
signal timing_change_ch4         : std_logic;
signal timing_change             : std_logic;
signal output_level_ok           : std_logic;
signal all_genlock_ok            : std_logic;

signal clk1                      : std_logic;
signal clk2                      : std_logic;
signal clk3                      : std_logic;
signal clk4                      : std_logic;
signal clk_select_1              : std_logic;
signal clk_select_2              : std_logic;
signal clk_select_3              : std_logic;
signal clk_select_4              : std_logic;

attribute clock_signal           : string;
attribute clock_signal of clk1   : signal is "yes";
attribute clock_signal of clk2   : signal is "yes";
attribute clock_signal of clk3   : signal is "yes";
attribute clock_signal of clk4   : signal is "yes";



-- debug:
--signal sync_mode                 : std_logic_vector(1 downto 0);
--signal ch1_genlock_sync          : std_logic;
--signal ch1_frame_start           : std_logic;
--signal ch1_genlock_sync_tog      : std_logic;
--signal ch1_frame_start_tog       : std_logic;
--signal f8g_clean_tog             : std_logic;
--signal f8g_sync_reset            : std_logic;
--signal f4m_clean_tog             : std_logic;
--signal f4m_sync_reset            : std_logic;


--signal genlock_resync_latch      : std_logic;
--signal spl_div                   : std_logic_vector(9 downto 0);

--signal debug1                    : std_logic_vector(15 downto 0) := (others => '0');
--signal debug2                    : std_logic_vector(15 downto 0) := (others => '0');
--signal debug3                    : std_logic_vector(15 downto 0) := (others => '0');
--signal debug4                    : std_logic_vector(15 downto 0) := (others => '0');

--Constants:
constant f8g_count1              : integer := 3375;        -- used to genlock to f8g sync
constant f8g_count2              : integer := 7040;        -- 3375*7040 = 23760000
constant f8g_bits1               : integer := 12;          -- size of counter 1
constant f8g_bits2               : integer := 13;          -- size of counter 2

constant f4m_count1              : integer := 2750;        -- used to genlock to f4m sync
constant f4m_count2              : integer := 3600;        -- 2750*3600 = 9900000
constant f4m_bits1               : integer := 12;          -- size of counter 1
constant f4m_bits2               : integer := 12;          -- size of counter 2

constant sync2398_count1         : integer := 2250;        -- used to generate 23,98Hz sync
constant sync2398_count2         : integer := 2750;        -- 2250*2750 = 6187500
constant sync2398_bits1          : integer := 12;          -- size of counter 1
constant sync2398_bits2          : integer := 12;          -- size of counter 2

constant sync24_count1           : integer := 2250;        -- used to generate 24Hz sync
constant sync24_count2           : integer := 2750;        -- 2250*2750 = 6187500
constant sync24_bits1            : integer := 12;          -- size of counter 1
constant sync24_bits2            : integer := 12;          -- size of counter 2

constant sync30_count1           : integer := 1375;        -- used to generate 30Hz sync
constant sync30_count2           : integer := 3600;        -- 1375*3600 = 4950000
constant sync30_bits1            : integer := 12;          -- size of counter 1
constant sync30_bits2            : integer := 12;          -- size of counter 2


component master_reset_delay
   port (
	   mreset_i             		: in std_logic;           -- Master reset
	   clk_i                		: in std_logic;           -- sample clock
	   tick_10ms_o                : out std_logic;
	   reset_delayed_o				: out std_logic
   );
end component;


component period_dual_count
   generic(
      count_val1  : integer;
      count_val2  : integer;
      bits1       : integer;
      bits2       : integer
   );
   Port(
      mreset_i    : in std_logic;
      clk_i       : in std_logic;
      sync_reset_i: in std_logic;
      sync_o      : out std_logic
   );
end component;


component sync_genlock_regen
	generic (
   	count_val1         : integer;                             -- the period of sync is count_val1*count_val2
      count_val2         : integer;
      bits1              : integer;                             -- number of bits in counters
      bits2              : integer
	);
  	port (
   	mreset_i           : in std_logic;
   	clk_i              : in std_logic;
   	sync_i             : in std_logic;
   	sync_clean_o       : out std_logic;
		genlock_ok_o       : out std_logic
		--for debug:
--		sync_reset_o       : out std_logic
   );
end component;


component tri_level_channel
   Port (
      mreset_i          : in std_logic;      -- Master Reset fra mainboard
      reset_delayed_i   : in std_logic;
      f1485_i           : in std_logic;
      tick_10ms_i       : in std_logic;
      clk_i             : in std_logic;      -- 148.35 or 148.5 MHz clock
      f4m_i             : in std_logic;      -- Vertikal sync reference - 625 lines SD format
      f8g_i             : in std_logic;      -- Vertikal sync reference - 525 lines SD format
      f2398_i           : in std_logic;
      f24_i             : in std_logic;
      f30_i             : in std_logic;
      sck_i             : in std_logic;      -- serial interface clock
      mosi_i            : in std_logic;      -- Master Out Serial data In
      cs_i              : in std_logic;      -- chip select
      tsg_level_i       : in std_logic;      -- Monitoring of the output signal
      use_f1484_o       : out std_logic;     -- selecting clock domain
      use_f1485_o       : out std_logic;     -- selecting clock domain
      DAC_o             : out std_logic_vector(3 downto 0);   -- 4 bit til DA converter
      clmp_o            : out std_logic;     -- til clamping puls - benyttes ikke
      tsg_ok_o          : out std_logic;     -- tsg status
      timing_change_o   : out std_logic
      -- for debug:
--      debug_o           : out std_logic_vector(15 downto 0)
   );
end component;


component BUFG
   Port (
      I : in  std_logic;
      O : out std_logic
   );
end component;


component bufgmux
   Port (
      I0 : in std_logic;
      I1 : in std_logic;
      S  : in std_logic;
      O  : out std_logic
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
   clk_i             => f1485a,
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

f1485a_i_distribution : BUFG
port map(
   I => f1485_i,
   O => f1485a
);

f1485b_i_distribution : BUFG
port map(
   I => f1485_i,
   O => f1485b
);

f1485c_i_distribution : BUFG
port map(
   I => f1485_i,
   O => f1485c
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
   clk_i        => f1485a,
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
   clk_i        => f1485a,
   sync_reset_i => '0',
   sync_o       => sync_30
);


--------------------------------------------------------------
-- Genlock to the mainframe f4m signal for NTSC
--------------------------------------------------------------
f4m_genlock_regen : sync_genlock_regen
   generic map (
      count_val1       => f4m_count1,
      count_val2       => f4m_count2,
      bits1            => f4m_bits1,
      bits2            => f4m_bits2
   )
   port map (
      mreset_i         => reset_delayed,
      clk_i            => f1484,
      sync_i           => f4m_i,
      sync_clean_o     => f4m_clean,
      genlock_ok_o     => f4m_genlock_ok
      --for debug:
--      sync_reset_o     => f4m_sync_reset
   );


--------------------------------------------------------------
-- Genlock to the mainframe f8g signal for PAL
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
      clk_i            => f1485a,
      sync_i           => f8g_i,
      sync_clean_o     => f8g_clean,
      genlock_ok_o     => f8g_genlock_ok
      --for debug:
--      sync_reset_o     => f8g_sync_reset
   );


--------------------------------------------
-- Generator channel 1
--------------------------------------------

-- Select clock domain for Channel 1:
clock_select_ch_1 : bufgmux
   port map(
      I0 => f1485_i,
      I1 => f1484_i,
      S  => clk_select_1,
      O  => clk1
   );

Tri_Level_Channel_1 : tri_level_channel
   Port map (
      mreset_i          => mreset_i,
      reset_delayed_i   => reset_delayed,
      f1485_i           => f1485b,
      tick_10ms_i       => tick_10ms,
      clk_i             => clk1,
      f4m_i             => f4m_clean,
      f8g_i             => f8g_clean,
      f2398_i           => sync_2398,
      f24_i             => sync_24,
      f30_i             => sync_30,
      sck_i             => sck_i,
      mosi_i            => mosi_i,
      cs_i              => cs1_i,
      tsg_level_i       => tsg1_lvl_i,

      use_f1484_o       => clk_select_1,
      tsg_ok_o          => tsg1_ok,
      DAC_o             => tsg1_o,
      clmp_o            => clmp1_o,
      timing_change_o   => timing_change_ch1

      --debug:
--      debug_o           => debug1
   );

   
--------------------------------------------
-- Generator channel 2
--------------------------------------------

-- Select clock domain for Channel 2:
clock_select_ch_2 : bufgmux
   port map(
      I0 => f1485_i,
      I1 => f1484_i,
      S  => clk_select_2,
      O  => clk2
   );

Tri_Level_Channel_2 : tri_level_channel
   Port map (
      mreset_i          => mreset_i,
      reset_delayed_i   => reset_delayed,
      f1485_i           => f1485b,
      tick_10ms_i       => tick_10ms,
      clk_i             => clk2,
      f4m_i             => f4m_clean,
      f8g_i             => f8g_clean,
      f2398_i           => sync_2398,
      f24_i             => sync_24,
      f30_i             => sync_30,
      sck_i             => sck_i,
      mosi_i            => mosi_i,
      cs_i              => cs2_i,
      tsg_level_i       => tsg2_lvl_i,

      use_f1484_o       => clk_select_2,
      tsg_ok_o          => tsg2_ok,
      DAC_o             => tsg2_o,
      clmp_o            => clmp2_o,
      timing_change_o   => timing_change_ch2
      --debug:
--      debug_o           => debug2
   );
  


--------------------------------------------
-- Generator channel 3
--------------------------------------------

-- Select clock domain for Channel 3:
clock_select_ch_3 : bufgmux
   port map(
      I0 => f1484_i,
      I1 => f1485_i,
      S  => clk_select_3,
      O  => clk3
   );

Tri_Level_Channel_3 : tri_level_channel
   Port map (
      mreset_i          => mreset_i,
      reset_delayed_i   => reset_delayed,
      f1485_i           => f1485c,
      tick_10ms_i       => tick_10ms,
      clk_i             => clk3,
      f4m_i             => f4m_clean,
      f8g_i             => f8g_clean,
      f2398_i           => sync_2398,
      f24_i             => sync_24,
      f30_i             => sync_30,
      sck_i             => sck_i,
      mosi_i            => mosi_i,
      cs_i              => cs3_i,
      tsg_level_i       => tsg3_lvl_i,

      use_f1485_o       => clk_select_3,
      tsg_ok_o          => tsg3_ok,
      DAC_o             => tsg3_o,
      clmp_o            => clmp3_o,
      timing_change_o   => timing_change_ch3
      --debug:
--      debug_o           => debug3
   );


--------------------------------------------
-- Generator channel 4
--------------------------------------------

-- Select clock domain for Channel 4:
clock_select_ch_4 : bufgmux
   port map(
      I0 => f1484_i,
      I1 => f1485_i,
      S  => clk_select_4,
      O  => clk4
   );

Tri_Level_Channel_4 : tri_level_channel
   Port map (
      mreset_i          => mreset_i,
      reset_delayed_i   => reset_delayed,
      f1485_i           => f1485c,
      tick_10ms_i       => tick_10ms,
      clk_i             => clk4,
      f4m_i             => f4m_clean,
      f8g_i             => f8g_clean,
      f2398_i           => sync_2398,
      f24_i             => sync_24,
      f30_i             => sync_30,
      sck_i             => sck_i,
      mosi_i            => mosi_i,
      cs_i              => cs4_i,
      tsg_level_i       => tsg4_lvl_i,

      use_f1485_o       => clk_select_4,
      tsg_ok_o          => tsg4_ok,
      DAC_o             => tsg4_o,
      clmp_o            => clmp4_o,
      timing_change_o   => timing_change_ch4
      --debug:
--      debug_o           => debug4
   );


-------------------------------------------------------------
-- Monitors the signal level and timing changes:
-------------------------------------------------------------
output_level_ok   <= tsg1_ok and tsg2_ok and tsg3_ok and tsg4_ok;
timing_change     <= timing_change_ch1 or timing_change_ch2 or timing_change_ch3 or timing_change_ch4;
all_genlock_ok    <= f4m_genlock_ok and f8g_genlock_ok;

res1_o <= output_level_ok;
res2_o <= output_level_ok;


-------------------------------------------------------------
-- Led latches:
-------------------------------------------------------------
led_latch1: led_latch
   port map (
	   led_i       => timing_change,
	   clk_i       => f1485a,
	   tick_10ms_i => tick_10ms,
	   led_o       => led1_o
   );

led_latch2: led_latch
   port map (
	   led_i       => not all_genlock_ok,
	   clk_i       => f1485a,
	   tick_10ms_i => tick_10ms,
	   led_o       => led2_o
   );

led_latch3: led_latch
   port map (
	   led_i       => not output_level_ok,
	   clk_i       => f1485a,
	   tick_10ms_i => tick_10ms,
	   led_o       => led3_o
   );


-------------------------------------------------------------
-- Debug:
-------------------------------------------------------------

--     res1_o      <= '1';        -- 1 = level ok
--     res2_o      <= '1';        -- 1 = level ok

--      tsg3_o      <= (others => '0');
--      tsg4_o      <= (others => '0');

--      clmp3_o     <= '0';
--      clmp4_o     <= '0';

--      ext3_o(0)   <= debug1(0);    -- 1
--      ext3_o(1)   <= debug1(1);    -- 2
--      ext3_o(2)   <= debug1(2);    -- 3
--      ext3_o(3)   <= debug1(3);    -- 4
--
--      ext2_o(0)   <= debug1(4);    -- 6
--      ext2_o(1)   <= debug1(5);    -- 7
--      ext2_o(2)   <= debug1(6);    -- 8
--      ext2_o(3)   <= debug1(7);    -- 9
--
--      ext1_o(0)   <= debug1(8);    -- 12
--      ext1_o(1)   <= debug1(9);    -- 13
--      ext1_o(2)   <= debug1(10);   -- 14
--      ext1_o(3)   <= debug1(11);   -- 15
--
--      ext0_o(0)   <= debug1(12);   -- 17
--      ext0_o(1)   <= debug1(13);   -- 18
--      ext0_o(2)   <= debug1(14);   -- 19
--      ext0_o(3)   <= debug1(15);   -- 20
--
--      led1_o      <= timing_change;--debug(0);
--      led2_o      <= debug(1);

end Behavioral;