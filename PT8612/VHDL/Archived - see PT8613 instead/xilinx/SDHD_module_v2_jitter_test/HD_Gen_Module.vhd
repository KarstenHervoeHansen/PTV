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
      mreset_i       : in std_logic;      -- Master Reset fra mainboard
      f4m_i          : in std_logic;      -- Genlock syncronization for NTSC (4 field period)
      f8g_i          : in std_logic;      -- Genlock syncronization for PAL (8 field period)

      --------- clocks -------------------------------------------------------------------------
   	brefclk_p_i    : in std_logic;
   	brefclk_n_i    : in std_logic;
   	brefclk2_p_i   : in std_logic;
   	brefclk2_n_i   : in std_logic;

   	refclk_p_i     : in std_logic;
      refclk_n_i     : in std_logic;
      refclk2_p_i    : in std_logic;
      refclk2_n_i    : in std_logic;
   	
      --------- from Controller ----------------------------------------------------------------
      sck_i          : in std_logic;      -- serial interface clock
      mosi_i         : in std_logic;      -- Master Out Serial data In
      cs1_i          : in std_logic;      -- chip select channel 1 (nss)
      cs2_i          : in std_logic;      -- chip select channel 2 (port 2 bit 5)
      cs3_i          : in std_logic;      -- chip select channel 3 (port 1 bit 7)
      cs4_i          : in std_logic;      -- chip select channel 4 (port 1 bit 6)

      --------- to Controller ------------------------------------------------------------------
      ok1_o          : out std_logic;     -- generator 1&2 output levels OK - respons til mainframe
      ok2_o          : out std_logic;     -- generator 3&4 output levels OK - respons til mainframe

      --------- to PLL's -----------------------------------------------------------------------
	   pll1_S0_o      : out std_logic;
	   pll1_S1_o      : out std_logic;
	   pll1_S2_o      : out std_logic;
	   pll1_S3_o      : out std_logic;
	   pll2_S0_o      : out std_logic;
	   pll2_S1_o      : out std_logic;
	   pll2_S2_o      : out std_logic;
	   pll2_S3_o      : out std_logic;

      --------- HD Outputs ---------------------------------------------------------------------
      sd_zhd_1_o     : out std_logic;
      SDI_p_1_o      : out std_logic;
      SDI_n_1_o      : out std_logic;

      sd_zhd_2_o     : out std_logic;
      SDI_p_2_o      : out std_logic;
      SDI_n_2_o      : out std_logic;
      
      sd_zhd_3_o     : out std_logic;
      SDI_p_3_o      : out std_logic;
      SDI_n_3_o      : out std_logic;
                                    
      sd_zhd_4_o     : out std_logic;
      SDI_p_4_o      : out std_logic;
      SDI_n_4_o      : out std_logic;
      
      --------- LED's --------------------------------------------------------------------------
      led1_o         : out std_logic;
      led2_o         : out std_logic;
      led3_o         : out std_logic;
      led4_o         : out std_logic;

      --------- Debug --------------------------------------------------------------------------
      debug_o        : out std_logic_vector(9 downto 0)
   );
end HD_Gen_Module;


architecture Behavioral of HD_Gen_Module is

signal mreset                    : std_logic;
signal f4m_1485                  : std_logic;
signal f4m_1484                  : std_logic;
signal f8g                       : std_logic;
signal sck                       : std_logic;
signal mosi                      : std_logic;
signal cs1                       : std_logic;
signal cs2                       : std_logic;
signal cs3                       : std_logic;
signal cs4                       : std_logic;

signal sync_2398                 : std_logic;
signal sync_24                   : std_logic;
signal sync_30                   : std_logic;
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
signal clk1                      : std_logic;
signal clk2                      : std_logic;
signal brefclk                   : std_logic;
signal brefclk2                  : std_logic;
signal refclk                    : std_logic;
signal refclk2                   : std_logic;
signal tick_10ms                 : std_logic;

signal timing_change_ch1         : std_logic    := '0';
signal timing_change_ch2         : std_logic    := '0';
signal timing_change_ch3         : std_logic    := '0';
signal timing_change_ch4         : std_logic    := '0';
signal rio_fifo_err_ch1          : std_logic    := '0';
signal rio_fifo_err_ch2          : std_logic    := '0';
signal rio_fifo_err_ch3          : std_logic    := '0';
signal rio_fifo_err_ch4          : std_logic    := '0';

signal sd_zhd_1                  : std_logic    := '0';
signal sd_zhd_2                  : std_logic    := '0';
signal sd_zhd_3                  : std_logic    := '0';
signal sd_zhd_4                  : std_logic    := '0';

signal timing_change             : std_logic;
signal rio_fifo_error            : std_logic;
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


component glitch_remover
   port (
      clk_i            : in std_logic;
      signal_i         : in std_logic;
      clean_signal_o   : out std_logic
   );
end component;


component IBUFGDS
   generic(IOSTANDARD: string);
	port( O: out std_ulogic;
	 		I: in std_ulogic;
			IB:in std_ulogic);
end component;


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
--------- to/from Module ---------------------------------------------------------------------
      mreset_i          : in std_logic;      -- Master Reset fra mainboard
      reset_delayed_i   : in std_logic;
      f1484_i           : in std_logic;
      f1485_i           : in std_logic;
      brefclk_i         : in std_logic;
      brefclk2_i        : in std_logic;
      tick_10ms_i       : in std_logic;
      
--------- Sync signals -----------------------------------------------------------------------


--------- fra CPU ----------------------------------------------------------------------------
      sck_i             : in std_logic;      -- serial interface clock
      mosi_i            : in std_logic;      -- Master Out Serial data In
      cs_i              : in std_logic;      -- chip select

--------- HD Outputs -------------------------------------------------------------------------
      sd_zhd_o          : out std_logic;
      sdi_p_o           : out std_logic;
      sdi_n_o           : out std_logic;

--------- Monitoring I/O ---------------------------------------------------------------------
      timing_change_o   : out std_logic;
      rio_fifo_err_o    : out std_logic;    

--------- Monitoring I/O ---------------------------------------------------------------------
      debug_o           : out std_logic_vector(9 downto 0)
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
-- Remove glitches on input signals:
--------------------------------------------------------------
glitch_removing_mreset : glitch_remover
   port map(
      clk_i             => clk2,                               
      signal_i          => mreset_i,
      clean_signal_o    => mreset
   );

glitch_removing_f4m_1485 : glitch_remover
   port map(
      clk_i             => clk2,                               
      signal_i          => f4m_i,
      clean_signal_o    => f4m_1485
   );

glitch_removing_f4m_1484 : glitch_remover
   port map(
      clk_i             => clk1,                               
      signal_i          => f4m_i,
      clean_signal_o    => f4m_1484
   );

glitch_removing_f8g : glitch_remover
   port map(
      clk_i             => clk2,                               
      signal_i          => f8g_i,
      clean_signal_o    => f8g
   );
   
glitch_removing_sck : glitch_remover
   port map(
      clk_i             => clk2,                               
      signal_i          => sck_i,
      clean_signal_o    => sck
   );

glitch_removing_mosi : glitch_remover
   port map(
      clk_i             => clk2,                               
      signal_i          => mosi_i,
      clean_signal_o    => mosi
   );

glitch_removing_cs1 : glitch_remover
   port map(
      clk_i             => clk2,                               
      signal_i          => cs1_i,
      clean_signal_o    => cs1
   );

glitch_removing_cs2 : glitch_remover
   port map(
      clk_i             => clk2,                               
      signal_i          => cs2_i,
      clean_signal_o    => cs2
   );

glitch_removing_cs3 : glitch_remover
   port map(
      clk_i             => clk2,                               
      signal_i          => cs3_i,
      clean_signal_o    => cs3
   );

glitch_removing_cs4 : glitch_remover
   port map(
      clk_i             => clk2,                               
      signal_i          => cs4_i,
      clean_signal_o    => cs4
   );


--------------------------------------------------------------
-- Master reset delay and 10ms tick generation:
--------------------------------------------------------------
master_reset_delaying : master_reset_delay
port map(
   mreset_i          => mreset,
   clk_i             => clk2,       --148.5
   tick_10ms_o       => tick_10ms,
   reset_delayed_o   => reset_delayed
);


--------------------------------------------------------------
-- Differntial clock inputs:
--------------------------------------------------------------
diff_buf_brefclk : IBUFGDS
generic map (IOSTANDARD => "LVDS_25_DCI")
port map (
	O  => brefclk,
	I  => brefclk_p_i,
	IB => brefclk_n_i
);


diff_buf_brefclk2 : IBUFGDS
generic map (IOSTANDARD => "LVDS_25_DCI")
port map (
	O  => brefclk2,
	I  => brefclk2_p_i,
	IB => brefclk2_n_i
);


--diff_buf_refclk : IBUFGDS
--generic map (IOSTANDARD => "LVDS_25")
--port map (
--	O  => refclk,
--	I  => refclk_p_i,
--	IB => refclk_n_i
--);
--
--
--diff_buf_refclk2 : IBUFGDS
--generic map (IOSTANDARD => "LVDS_25")
--port map (
--	O  => refclk2,
--	I  => refclk2_p_i,
--	IB => refclk2_n_i
--);


--------------------------------------------------------------
-- Use BUFGMUX (BUFG) for clock distribution:
--------------------------------------------------------------
--clk1_for_logic : bufgmux
--   port map(
--      I0 => refclk2,
--      I1 => refclk,
--      S  => '1',
--      O  => clk1
--   );


--clk2_for_logic : bufgmux
--   port map(
--      I0 => refclk2,
--      I1 => refclk,
--      S  => '0',
--      O  => clk2
--   );


clk1_for_logic : BUFG
port map(
	I => brefclk,
	O => clk1
);


clk2_for_logic : BUFG
port map(
	I => brefclk2,
	O => clk2
);


--------------------------------------------
-- Generator channel 1
--------------------------------------------
HD_Gen_Channel_1 : HD_Gen_channel
   Port map (
      mreset_i          => mreset,
      reset_delayed_i   => reset_delayed,
      f1484_i           => refclk,
      f1485_i           => clk2,
      brefclk_i         => brefclk,
      brefclk2_i        => brefclk2,
      tick_10ms_i       => tick_10ms,
      sck_i             => sck,
      mosi_i            => mosi,
      cs_i              => cs1,
      sd_zhd_o          => sd_zhd_1,
      SDI_p_o           => SDI_p_1_o,
      SDI_n_o           => SDI_n_1_o,
      rio_fifo_err_o    => rio_fifo_err_ch1
   );

   
--------------------------------------------
-- Generator channel 2
--------------------------------------------
HD_Gen_Channel_2 : HD_Gen_channel
   Port map (
      mreset_i          => mreset,
      reset_delayed_i   => reset_delayed,
      f1484_i           => refclk,
      f1485_i           => clk2,
      brefclk_i         => brefclk,
      brefclk2_i        => brefclk2,
      tick_10ms_i       => tick_10ms,
      sck_i             => sck,
      mosi_i            => mosi,
      cs_i              => cs2,
      sd_zhd_o          => sd_zhd_2,
      SDI_p_o           => SDI_p_2_o,
      SDI_n_o           => SDI_n_2_o,
      rio_fifo_err_o    => rio_fifo_err_ch2
   );


----------------------------------------------
---- Generator channel 3
----------------------------------------------
HD_Gen_Channel_3 : HD_Gen_channel
   Port map (
      mreset_i          => mreset,
      reset_delayed_i   => reset_delayed,
      f1484_i           => refclk,
      f1485_i           => clk2,
      brefclk_i         => brefclk,
      brefclk2_i        => brefclk2,
      tick_10ms_i       => tick_10ms,
      sck_i             => sck,
      mosi_i            => mosi,
      cs_i              => cs3,
      sd_zhd_o          => sd_zhd_3,
      SDI_p_o           => SDI_p_3_o,
      SDI_n_o           => SDI_n_3_o,
      rio_fifo_err_o    => rio_fifo_err_ch3
   );
--
--   
----------------------------------------------
---- Generator channel 4
----------------------------------------------
HD_Gen_Channel_4 : HD_Gen_channel
   Port map (
      mreset_i          => mreset,
      reset_delayed_i   => reset_delayed,
      f1484_i           => refclk,
      f1485_i           => clk2,
      brefclk_i         => brefclk,
      brefclk2_i        => brefclk2,
      tick_10ms_i       => tick_10ms,
      sck_i             => sck,
      mosi_i            => mosi,
      cs_i              => cs4,
      sd_zhd_o          => sd_zhd_4,
      SDI_p_o           => SDI_p_4_o,
      SDI_n_o           => SDI_n_4_o,
      rio_fifo_err_o    => rio_fifo_err_ch4
   );
         

-------------------------------------------------------------
-- Monitors the timing changes and genlock:
-------------------------------------------------------------
rio_fifo_error    <= rio_fifo_err_ch1 or rio_fifo_err_ch2 or rio_fifo_err_ch3 or rio_fifo_err_ch4;


-------------------------------------------------------------
-- Led latches:
-------------------------------------------------------------
led_latch1: led_latch
   port map (
      led_i       => '0',
      clk_i       => clk2,    -- 148.5
      tick_10ms_i => tick_10ms,
      led_o       => led1_o
   );

led_latch2: led_latch
   port map (
      led_i       => '0',
      clk_i       => clk2,    -- 148.5
      tick_10ms_i => tick_10ms,
      led_o       => led2_o
   );

led_latch3: led_latch
   port map (
      led_i       => rio_fifo_error,
      clk_i       => clk2,    -- 148.5
      tick_10ms_i => tick_10ms,
      led_o       => led3_o
   );

led_latch4: led_latch
   port map (
      led_i       => '0',
      clk_i       => clk2,    -- 148.5
      tick_10ms_i => tick_10ms,
      led_o       => led4_o
   );

-------------------------------------------------------------
-- Mapping of output signals:
-------------------------------------------------------------

--PLL configuration:
pll1_S0_o <= '1';    --brefclk2 148.5
pll1_S1_o <= '1';
pll1_S2_o <= '1';
pll1_S3_o <= '0';

pll2_S0_o <= '0';    --brefclk 148.35
pll2_S1_o <= '1';
pll2_S2_o <= '1';
pll2_S3_o <= '0';

sd_zhd_1_o <= sd_zhd_1;
sd_zhd_2_o <= sd_zhd_2;
sd_zhd_3_o <= sd_zhd_3;
sd_zhd_4_o <= sd_zhd_4;

ok1_o <= '1';
ok2_o <= '1';

--led1_o <= debug(0);
--led2_o <= debug(1);
--led3_o <= debug(2);
--led4_o <= debug(2);

debug_o  <= (others => '0'); --debug;

end Behavioral;