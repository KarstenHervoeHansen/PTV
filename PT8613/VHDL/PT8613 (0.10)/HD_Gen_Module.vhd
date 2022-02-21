library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;

------------------------------------------------------------------------------------------------
-- Toplevel for HD generator module PT8612
-- 2005-09-26: Created                                                        Thomas Holm Hansen
-- 2006-02-20: crc_registering process changed                                THH
-- 2006-07-01: changed to dual-link                                           THH
------------------------------------------------------------------------------------------------

entity HD_Gen_Module is
   Port (
      --------- from Mainframe -----------------------------------------------------------------
      mreset_i       : in std_logic;      -- Master Reset fra mainboard
      f4m_i          : in std_logic;      -- Genlock syncronization for NTSC (4 field period)
      f8g_i          : in std_logic;      -- Genlock syncronization for PAL (8 field period)

      --------- clocks -------------------------------------------------------------------------
      clk_27_i       : in std_logic;
      brefclk_p_i    : in std_logic;
   	brefclk_n_i    : in std_logic;
   	brefclk2_p_i   : in std_logic;
   	brefclk2_n_i   : in std_logic;

      --------- from Controller ----------------------------------------------------------------
      sck_i          : in std_logic;      -- serial interface clock
      mosi_i         : in std_logic;      -- Master Out Serial data In
      cs1_i          : in std_logic;      -- chip select channel 1 (nss)

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

	   --------- vcxo : --------------------------------------------------------------------------
      vcxo_control_1_o  : inout std_logic;
      vcxo_control_2_o  : inout std_logic;

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
      led_o          : out std_logic_vector(3 downto 0);

      --------- Debug I/O ---------------------------------------------------------------------
      pc_ibf_i       : in std_logic;
      pc_data_o      : out std_logic_vector(9 downto 0);
      pc_zstb_o      : out std_logic
   );
end HD_Gen_Module;


architecture Behavioral of HD_Gen_Module is

signal reset                     : std_logic_vector(3 downto 0);
signal f4m_1485                  : std_logic;
signal f4m_1484                  : std_logic;
signal f8g                       : std_logic;
signal led_status                : std_logic_vector(3 downto 0);
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
signal clk1                      : std_logic;
signal clk2                      : std_logic;
signal brefclk                   : std_logic;
signal brefclk2                  : std_logic;
signal refclk                    : std_logic;
signal refclk2                   : std_logic;
signal tick_10ms                 : std_logic;

signal video_reset_ch1           : std_logic    := '0';
signal video_reset_ch2           : std_logic    := '0';
signal video_reset               : std_logic;

signal rio_fifo_err_ch1          : std_logic    := '0';
signal rio_fifo_err_ch2          : std_logic    := '0';

signal sd_zhd_1                  : std_logic    := '0';
signal sd_zhd_2                  : std_logic    := '0';
signal sd_zhd_3                  : std_logic    := '0';
signal sd_zhd_4                  : std_logic    := '0';

signal timing_change             : std_logic;
signal rio_fifo_error            : std_logic;
signal all_genlock_ok            : std_logic;
signal debug							: std_logic_vector(3 downto 0);

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

--constant channel_id_1            : integer := 0;
--constant channel_id_2            : integer := 1;


component pll_controller
   generic (
      ref_div_minus_1   : integer;
      vco_div_minus_1   : integer
   );
   port (
      clk_ref_i         : in std_logic;
      clk_vco_i         : in std_logic;
      vcxo_control_o    : inout std_logic
   );
end component;

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
   generic (
      channel_id        : integer;
      pc_enable_mode    : std_logic
   );
   Port (
--------- to/from Module ---------------------------------------------------------------------
      reset_i           : in std_logic;      
      reset_delayed_i   : in std_logic;      
      f1485_i           : in std_logic;
      brefclk_i         : in std_logic;
      brefclk2_i        : in std_logic;
      tick_10ms_i       : in std_logic;

--------- Sync signals -----------------------------------------------------------------------
      f4m_1484_i        : in std_logic;      -- sync reference - HD systems
      f4m_1485_i        : in std_logic;      -- sync reference - SD NTSC
      f8g_i             : in std_logic;      -- sync reference
      f2398_i           : in std_logic;
      f24_i             : in std_logic;
      f30_i             : in std_logic;

--------- fra CPU ----------------------------------------------------------------------------
      cs_i              : in  std_logic;     -- chip select
      sck_i             : in  std_logic;     -- serial clock
      mosi_i            : in  std_logic;     -- master out serial data in

--------- HD Outputs -------------------------------------------------------------------------
      sd_zhd_o          : out std_logic;
      sdi_p_a_o         : out std_logic;
      sdi_n_a_o         : out std_logic;
      sdi_p_b_o         : out std_logic;
      sdi_n_b_o         : out std_logic;
      
--------- Monitoring I/O ---------------------------------------------------------------------
      video_reset_o     : out std_logic;
      rio_fifo_err_o    : out std_logic;

--------- Debug I/O ---------------------------------------------------------------------
      pc_ibf_i          : in std_logic := '0';
      pc_data_o         : out std_logic_vector(9 downto 0);
      pc_zstb_o         : out std_logic;
      debug_o           : out std_logic_vector(3 downto 0)
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


component master_reset_delay
   port (
      zreset_i                   : in std_logic;                        -- Master reset
      clk_i                      : in std_logic;                        -- sample clock
      led_i                      : in std_logic_vector(3 downto 0);
      led_o                      : out std_logic_vector(3 downto 0);
      tick_10ms_o                : out std_logic;
      reset_o                    : out std_logic_vector(3 downto 0)
   );
end component;


begin

pll_controlling_1484 : pll_controller
generic map (
   ref_div_minus_1   => 90,      -- period = 91
   vco_div_minus_1   => 499      -- period = 500
)
port map (
   clk_ref_i         => clk_27_i,
   clk_vco_i         => brefclk,
   vcxo_control_o    => vcxo_control_1_o
);


pll_controlling_1485 : pll_controller
generic map (
   ref_div_minus_1   => 89,     -- period = 90
   vco_div_minus_1   => 494     -- period = 495
)
port map (
   clk_ref_i         => clk_27_i,
   clk_vco_i         => brefclk2,
   vcxo_control_o    => vcxo_control_2_o
);


--------------------------------------------------------------
-- Remove glitches on input signals:
--------------------------------------------------------------
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
   

--------------------------------------------------------------
-- Master reset delay, 10ms tick generation and led control :
--------------------------------------------------------------
master_reset_delaying : master_reset_delay
port map(
   zreset_i          => mreset_i,
   clk_i             => clk2,       --148.5
   led_i             => led_status,
   led_o             => led_o,
   tick_10ms_o       => tick_10ms,
   reset_o           => reset
);


-------------------------------------------------------------
-- Monitors the timing changes and genlock:
-------------------------------------------------------------
video_reset       <= video_reset_ch1 or video_reset_ch2;
all_genlock_ok    <= f4m_1484_genlock_ok and f4m_1485_genlock_ok and f8g_genlock_ok;
rio_fifo_error    <= rio_fifo_err_ch1 or rio_fifo_err_ch2;

led_status        <= not all_genlock_ok & rio_fifo_error & video_reset & debug(0);

--------------------------------------------------------------
-- Differntial clock inputs:
--------------------------------------------------------------
diff_buf_brefclk : IBUFGDS
generic map (IOSTANDARD => "LVDS_25_DT") -- generic map (IOSTANDARD => "LVDS_25")
port map (
	O  => brefclk,
	I  => brefclk_p_i,
	IB => brefclk_n_i
);


diff_buf_brefclk2 : IBUFGDS
generic map (IOSTANDARD => "LVDS_25_DT")  --generic map (IOSTANDARD => "LVDS_25")
port map (
	O  => brefclk2,
	I  => brefclk2_p_i,
	IB => brefclk2_n_i
);


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
   mreset_i     => not reset(0),
   clk_i        => clk1,      -- 148.4
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
   mreset_i     => not reset(0),
   clk_i        => clk2,      -- 148.5
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
   mreset_i     => not reset(0),
   clk_i        => clk2,      -- 148.5
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
      mreset_i         => not reset(1),
      clk_i            => clk1,  --148.4
      sync_i           => f4m_1484,
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
      mreset_i         => not reset(1),
      clk_i            => clk2,     --148.5 
      sync_i           => f4m_1485,
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
      mreset_i         => not reset(1),
      clk_i            => clk2,        -- 148.5
      sync_i           => f8g,
      sync_clean_o     => f8g_clean,
      genlock_ok_o     => f8g_genlock_ok
   );


--------------------------------------------
-- Generator channel 1
--------------------------------------------
HD_Gen_Channel_1 : HD_Gen_channel
   generic map (
      channel_id        => 0,
      pc_enable_mode    => '0'
   )
   port map (
      reset_i           => reset(0),
      reset_delayed_i   => reset(3),
      f1485_i           => clk2,
      brefclk_i         => brefclk,
      brefclk2_i        => brefclk2,
      tick_10ms_i       => tick_10ms,
      f4m_1484_i        => f4m_1484_clean,
      f4m_1485_i        => f4m_1485_clean,
      f8g_i             => f8g_clean,
      f2398_i           => sync_2398,
      f24_i             => sync_24,
      f30_i             => sync_30,
      cs_i              => cs1_i,  
      sck_i             => sck_i, 
      mosi_i            => mosi_i,  
      sd_zhd_o          => sd_zhd_1,
      SDI_p_a_o         => SDI_p_1_o,
      SDI_n_a_o         => SDI_n_1_o,
      SDI_p_b_o         => SDI_p_2_o,
      SDI_n_b_o         => SDI_n_2_o,
      video_reset_o     => video_reset_ch1,
      rio_fifo_err_o    => rio_fifo_err_ch1,
      pc_ibf_i          => pc_ibf_i, 
      pc_data_o         => pc_data_o,
      pc_zstb_o         => pc_zstb_o
   );

   
--------------------------------------------
-- Generator channel 2
--------------------------------------------
HD_Gen_Channel_2 : HD_Gen_channel
   generic map (
      channel_id        => 1,
      pc_enable_mode    => '0'
   )
   Port map (
      reset_i           => reset(0),
      reset_delayed_i   => reset(3),
      f1485_i           => clk2,
      brefclk_i         => brefclk,
      brefclk2_i        => brefclk2,
      tick_10ms_i       => tick_10ms,
      f4m_1484_i        => f4m_1484_clean,
      f4m_1485_i        => f4m_1485_clean,
      f8g_i             => f8g_clean,
      f2398_i           => sync_2398,
      f24_i             => sync_24,
      f30_i             => sync_30,
      cs_i              => cs1_i,  
      sck_i             => sck_i,  
      mosi_i            => mosi_i, 
      sd_zhd_o          => sd_zhd_2,
      SDI_p_a_o         => SDI_p_3_o,
      SDI_n_a_o         => SDI_n_3_o,
      SDI_p_b_o         => SDI_p_4_o,
      SDI_n_b_o         => SDI_n_4_o,
      video_reset_o     => video_reset_ch2,
      rio_fifo_err_o    => rio_fifo_err_ch2,
      debug_o           => debug
   );
         

-------------------------------------------------------------
-- Mapping of output signals:
-------------------------------------------------------------

--PLL configuration:
pll1_S0_o <= '0';    --brefclk2 148.5
pll1_S1_o <= '1';
pll1_S2_o <= '1';
pll1_S3_o <= '0';

pll2_S0_o <= '1';    --brefclk 148.35
pll2_S1_o <= '1';
pll2_S2_o <= '1';
pll2_S3_o <= '0';

sd_zhd_1_o <= sd_zhd_1;
sd_zhd_2_o <= sd_zhd_1;
sd_zhd_3_o <= sd_zhd_2;
sd_zhd_4_o <= sd_zhd_2;

ok1_o <= '1';
ok2_o <= '1';


end Behavioral;