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
-- 2007-05-11: digital PLL w. external DAC                                    THH
------------------------------------------------------------------------------------------------

entity HD_Gen_Module is
   Port (
      --------- from Mainframe -----------------------------------------------------------------
      mreset_i          : in std_logic;      -- Master Reset fra mainboard
      f4m_i             : in std_logic;      -- Genlock syncronization for NTSC (4 field period)
      f8g_i             : in std_logic;      -- Genlock syncronization for PAL (8 field period)

      --------- clocks -------------------------------------------------------------------------
      clk_27_i          : in std_logic;
      brefclk_p_i       : in std_logic;
      brefclk_n_i       : in std_logic;
      brefclk2_p_i      : in std_logic;
      brefclk2_n_i      : in std_logic;

      --------- from Controller ----------------------------------------------------------------
      sck_i             : in std_logic;      -- serial interface clock
      mosi_i            : in std_logic;      -- Master Out Serial data In
      miso_o            : out std_logic;     -- master in serial data out
      cs1_i             : in std_logic;      -- chip select channel 1 (nss)

      --------- to Controller ------------------------------------------------------------------
      ok1_o             : out std_logic;     -- generator 1&2 output levels OK - respons til mainframe
      ok2_o             : out std_logic;     -- generator 3&4 output levels OK - respons til mainframe

      --------- to PLL's -----------------------------------------------------------------------
      pll1_s_o          : out std_logic_vector(3 downto 0);
      pll2_s_o          : out std_logic_vector(3 downto 0);
      
      --------- vcxo : --------------------------------------------------------------------------
      dac_sclk_1_o      : out std_logic;
      dac_dout_1_o      : out std_logic;     -- serial data to external vcxo DAC
      dac_zsync_1_o     : out std_logic;
      dac_sclk_2_o      : out std_logic;
      dac_dout_2_o      : out std_logic;     -- serial data to external vcxo DAC
      dac_zsync_2_o     : out std_logic;
      
      --------- HD Outputs ---------------------------------------------------------------------
      sd_zhd_1_o        : out std_logic;
      SDI_p_1_o         : out std_logic;
      SDI_n_1_o         : out std_logic;
                        
      sd_zhd_2_o        : out std_logic;
      SDI_p_2_o         : out std_logic;
      SDI_n_2_o         : out std_logic;
                        
      sd_zhd_3_o        : out std_logic;
      SDI_p_3_o         : out std_logic;
      SDI_n_3_o         : out std_logic;
                        
      sd_zhd_4_o        : out std_logic;
      SDI_p_4_o         : out std_logic;
      SDI_n_4_o         : out std_logic;

      --------- LED's --------------------------------------------------------------------------
      led_o             : out std_logic_vector(3 downto 0);

      --------- Debug I/O ---------------------------------------------------------------------
--      pc_ibf_i        : in std_logic;
--      pc_data_o       : out std_logic_vector(9 downto 0);
--      pc_zstb_o       : out std_logic
      red_o             : out std_logic_vector(5 downto 0);
      green_o           : out std_logic_vector(5 downto 0);
      blue_o            : out std_logic_vector(5 downto 0)
   );
end HD_Gen_Module;


architecture Behavioral of HD_Gen_Module is

signal reset                        : std_logic_vector(3 downto 0);
signal f4m_1485_grm                 : std_logic;
signal f4m_1484_grm                 : std_logic;
signal f8g_grm                      : std_logic;
signal led_status                   : std_logic_vector(3 downto 0);
signal led                          : std_logic_vector(3 downto 0);
signal sync_2398                    : std_logic;
signal sync_24                      : std_logic;
signal sync_30                      : std_logic;
signal f4m_1484_clean               : std_logic;
signal f4m_1484_genlock_ok          : std_logic;
signal f4m_1484_genlock_resync      : std_logic;
signal f4m_1485_clean               : std_logic;
signal f4m_1485_genlock_ok          : std_logic;
signal f4m_1485_genlock_resync      : std_logic;
signal f8g_clean                    : std_logic;
signal f8g_genlock_ok               : std_logic;
signal f8g_genlock_resync           : std_logic;
signal genlock_resync               : std_logic;
signal clk1                         : std_logic;
signal clk2                         : std_logic;
signal clk_27                       : std_logic;
signal brefclk                      : std_logic;
signal brefclk2                     : std_logic;
signal refclk                       : std_logic;
signal refclk2                      : std_logic;
signal tick_10ms                    : std_logic;
                                    
signal secm_tick_1484               : std_logic;
signal sec_tick_1485                : std_logic;
signal secm_tick_1485               : std_logic;
signal frame_count_25_command       : std_logic_vector(7 downto 0);
signal frame_count_30_command       : std_logic_vector(7 downto 0);
signal frame_count_25_actual        : std_logic_vector(7 downto 0);
signal frame_count_30_actual        : std_logic_vector(7 downto 0);
signal new_frame_count_25_command   : std_logic;
signal new_frame_count_30_command   : std_logic;
signal set_frame_count_25_command   : std_logic;
signal set_frame_count_30_command   : std_logic;
                                    
signal f4m_1484                     : std_logic;
signal f4m_1485                     : std_logic;
signal f8g                          : std_logic;
signal f2398                        : std_logic;
signal f24                          : std_logic;
signal f30                          : std_logic;
                                    
signal rio_fifo_err_ch1             : std_logic    := '0';
signal rio_fifo_err_ch2             : std_logic    := '0';
signal error_ch1                    : std_logic    := '0';
signal error_ch2                    : std_logic    := '0';
signal error_ch3                    : std_logic    := '0';
signal error_ch4                    : std_logic    := '0';
signal module_error                 : std_logic;
signal module_warning               : std_logic;
signal module_error_vcxo            : std_logic;
signal module_error_ics             : std_logic;
signal module_error_to_main         : std_logic;
signal miso_module                  : std_logic;
signal miso_ch1                     : std_logic    := '1';
signal miso_ch2                     : std_logic    := '1';
signal miso_ch3                     : std_logic    := '1';
signal miso_ch4                     : std_logic    := '1';
signal sd_zhd_1                     : std_logic    := '0';
signal sd_zhd_2                     : std_logic    := '0';
signal sd_zhd_3                     : std_logic    := '0';
signal sd_zhd_4                     : std_logic    := '0';
signal warning_clock                : std_logic_vector(7 downto 0);
signal error_clock                  : std_logic_vector(7 downto 0);

signal dac_sclk_1                   : std_logic;
signal dac_dout_1                   : std_logic;
signal dac_zsync_1                  : std_logic;
signal dac_sclk_2                   : std_logic;
signal dac_dout_2                   : std_logic;
signal dac_zsync_2                  : std_logic;

                                    
-- MGT:                             
signal ch_1_mgt_fifo_err            : std_logic;
signal ch_2_mgt_fifo_err            : std_logic;
signal ch_3_mgt_fifo_err            : std_logic;
signal ch_4_mgt_fifo_err            : std_logic;
signal mgt_1_power_down             : std_logic;
signal mgt_2_power_down             : std_logic;
signal mgt_3_power_down             : std_logic;
signal mgt_4_power_down             : std_logic;
signal mgt_1_sync_reset             : std_logic;
signal mgt_2_sync_reset             : std_logic;
signal mgt_3_sync_reset             : std_logic;
signal mgt_4_sync_reset             : std_logic;
signal mgt_1_use_clk2               : std_logic;
signal mgt_2_use_clk2               : std_logic;
signal mgt_3_use_clk2               : std_logic;
signal mgt_4_use_clk2               : std_logic;
signal mgt_1_par_data               : std_logic_vector(19 downto 0);
signal mgt_2_par_data               : std_logic_vector(19 downto 0);
signal mgt_3_par_data               : std_logic_vector(19 downto 0);
signal mgt_4_par_data               : std_logic_vector(19 downto 0);
signal mgt_1_data_clk               : std_logic;
signal mgt_2_data_clk               : std_logic;
signal mgt_3_data_clk               : std_logic;
signal mgt_4_data_clk               : std_logic;
signal mgt_1_fifo_err               : std_logic;
signal mgt_2_fifo_err               : std_logic;
signal mgt_3_fifo_err               : std_logic;
signal mgt_4_fifo_err               : std_logic;
signal ch_1_mgt_power_down          : std_logic;
signal ch_2_mgt_power_down          : std_logic;
signal ch_3_mgt_power_down          : std_logic;
signal ch_4_mgt_power_down          : std_logic;
signal ch_1_mgt_sync_reset          : std_logic;
signal ch_2_mgt_sync_reset          : std_logic;
signal ch_3_mgt_sync_reset          : std_logic;
signal ch_4_mgt_sync_reset          : std_logic;
signal ch_1_mgt_use_clk2            : std_logic;
signal ch_2_mgt_use_clk2            : std_logic;
signal ch_3_mgt_use_clk2            : std_logic;
signal ch_4_mgt_use_clk2            : std_logic;
signal ch_1_mgt_par_data_a          : std_logic_vector(19 downto 0);
signal ch_1_mgt_par_data_b          : std_logic_vector(19 downto 0);
signal ch_2_mgt_par_data_a          : std_logic_vector(19 downto 0);
signal ch_2_mgt_par_data_b          : std_logic_vector(19 downto 0);
signal ch_3_mgt_par_data_a          : std_logic_vector(19 downto 0);
signal ch_4_mgt_par_data_a          : std_logic_vector(19 downto 0);
signal ch_1_mgt_data_clk            : std_logic;
signal ch_2_mgt_data_clk            : std_logic;
signal ch_3_mgt_data_clk            : std_logic;
signal ch_4_mgt_data_clk            : std_logic;
                                    
signal debug                        : std_logic_vector(3 downto 0);
signal clock_debug                  : std_logic_vector(9 downto 0);
                                    
--Constants:                        
--constant f8g_count1                 : integer := 3375;  -- used to genlock to f8g sync
--constant f8g_count2                 : integer := 7040;  -- 3375*7040 = 23760000
--constant f8g_bits1                  : integer := 12;    -- size of counter 1
--constant f8g_bits2                  : integer := 13;    -- size of counter 2
--                                    
--constant f4m_1484_count1            : integer := 2750;  -- used to genlock to f4m sync (on 1484 clk)
--constant f4m_1484_count2            : integer := 3600;  -- 2750*3600 = 9900000
--constant f4m_1484_bits1             : integer := 12;    -- size of counter 1
--constant f4m_1484_bits2             : integer := 12;    -- size of counter 2
--                                    
--constant f4m_1485_count1            : integer := 3146;  -- used to genlock to f4m sync (on 1485 clk)
--constant f4m_1485_count2            : integer := 3150;  -- 3146*3150 = 9909900
--constant f4m_1485_bits1             : integer := 12;    -- size of counter 1
--constant f4m_1485_bits2             : integer := 12;    -- size of counter 2
--                                    
--constant sync2398_count1            : integer := 2250;  -- used to generate 23,98Hz sync
--constant sync2398_count2            : integer := 2750;  -- 2250*2750 = 6187500
--constant sync2398_bits1             : integer := 12;    -- size of counter 1
--constant sync2398_bits2             : integer := 12;    -- size of counter 2
--                                    
--constant sync24_count1              : integer := 2250;  -- used to generate 24Hz sync
--constant sync24_count2              : integer := 2750;  -- 2250*2750 = 6187500
--constant sync24_bits1               : integer := 12;    -- size of counter 1
--constant sync24_bits2               : integer := 12;    -- size of counter 2
--                                    
--constant sync30_count1              : integer := 1375;  -- used to generate 30Hz sync
--constant sync30_count2              : integer := 3600;  -- 1375*3600 = 4950000
--constant sync30_bits1               : integer := 12;    -- size of counter 1
--constant sync30_bits2               : integer := 12;    -- size of counter 2
                                    
constant use_vcxo                   : std_logic := '0'; -- OBS! : Comment TS_genlock_F2F in ucf file if use_vcxo = 1
constant pt8612_zpt8613             : std_logic := '1';
constant output_debug_signals       : std_logic := '1';

component IBUFGDS
   generic(IOSTANDARD: string);
   port( O: out std_ulogic;
         I: in std_ulogic;
         IB:in std_ulogic);
end component;


component HD_Gen_Channel
   generic (
      channel_id              : integer;
      pc_enable_mode          : std_logic;
      bypass_scrambler        : std_logic;
      timing_dummy_data       : std_logic;
      use_zero_timing         : std_logic;
      use_8612_z8613_mapping  : std_logic;
      use_bit_toggle          : std_logic
   );
   Port (
--------- to/from Module ---------------------------------------------------------------------
      reset_i           : in std_logic;
      reset_delayed_i   : in std_logic;
      f1485_i           : in std_logic;
      clk_27_i          : in std_logic;
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
      cs_i              : in std_logic;      -- chip select
      sck_i             : in std_logic;      -- serial clock
      mosi_i            : in std_logic;      -- master out serial data in
      miso_o            : out std_logic;     -- master in serial data out

--------- Outputs -------------------------------------------------------------------------
      sd_zhd_o          : out std_logic;

--------- MGT interface -------------------------------------------------------------------------
      mgt_fifo_err_i    : in std_logic;
      mgt_power_down_o  : out std_logic;
      mgt_sync_reset_o  : out std_logic;
      mgt_use_clk2_o    : out std_logic;
      mgt_par_data_a_o  : out std_logic_vector(19 downto 0);
      mgt_par_data_b_o  : out std_logic_vector(19 downto 0);
      mgt_data_clk_o    : out std_logic;

--------- Monitoring I/O ---------------------------------------------------------------------
      video_reset_o     : out std_logic;
      error_o           : out std_logic;

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


component master_reset_delay
   generic (
      sticky_led              : std_logic_vector(3 downto 0)         -- When high the led remains on until led_off is asserted
   );
   port (
      zreset_i                : in std_logic;                        -- Master reset
      clk_27_i                : in std_logic;
      led_i                   : in std_logic_vector(3 downto 0);
      led_off_i               : in std_logic;
      led_o                   : out std_logic_vector(3 downto 0);
      tick_10ms_o             : out std_logic;
      reset_o                 : out std_logic_vector(3 downto 0)
   );
end component;


component clock_genlock_controller
   generic (
      use_vcxo_zics           : std_logic   
   );
   port (
      clk_27_i                : in std_logic;
      clk_1484_i              : in std_logic;
      clk_1485_i              : in std_logic;
      f8g_i                   : in std_logic;      -- 160ms sync pulse input
      f4m_i                   : in std_logic;      -- (1.001/15)s sync pulse input
      vcxo_control_1_o        : out std_logic;     -- control voltage to loopfilter if no external dac is used
      dac_sclk_1_o            : out std_logic;
      dac_dout_1_o            : out std_logic;     -- serial data to external DAC
      dac_zsync_1_o           : out std_logic;
      vcxo_control_2_o        : out std_logic;     -- control voltage to loopfilter if no external dac is used
      dac_sclk_2_o            : out std_logic;
      dac_dout_2_o            : out std_logic;     -- serial data to external DAC
      dac_zsync_2_o           : out std_logic;
      secm_tick_1484_o        : out std_logic;     -- 1.001s tick output in 1484 clk domain
      sec_tick_1485_o         : out std_logic;     -- 1s tick output
      secm_tick_1485_o        : out std_logic;     -- 1.001s tick output in 1485 clk domain
      frame_count_25_i        : in std_logic_vector(7 downto 0);
      new_frame_count_25_i    : in std_logic;
      set_frame_count_25_i    : in std_logic;
      frame_count_25_o        : out std_logic_vector(7 downto 0);
      frame_count_30_i        : in std_logic_vector(7 downto 0);
      new_frame_count_30_i    : in std_logic;
      set_frame_count_30_i    : in std_logic;
      frame_count_30_o        : out std_logic_vector(7 downto 0);
      warning_o               : out std_logic_vector(7 downto 0);
      error_o                 : out std_logic_vector(7 downto 0);
      debug_o                 : out std_logic_vector(9 downto 0)
   );
end component;


component error_controller
   port (
      clk_i                : in std_logic;                        -- 27MHz clock
      parameter_change_i   : in std_logic;                        -- connect to cs_i
      error_i              : in std_logic;
      error_o              : out std_logic;
      tick_10ms_o          : out std_logic
   );
end component;


component serial_interface
   generic (
      channel_id              : integer;
      use_8612_z8613_mapping  : std_logic
   );
   Port (
      clk_i                   : in  std_logic;        -- clock
      reset_i                 : in  std_logic := '0'; -- sync_reset
      cs_i                    : in  std_logic;        -- chip select
      sck_i                   : in  std_logic;        -- serial clock
      mosi_i                  : in  std_logic;        -- master out serial data in
      miso_o                  : out std_logic;        -- master in serial data out (high when inactive)
      status_channel_i        : in std_logic_vector(7 downto 0) := (others => '1');
      warning_clock_i         : in std_logic_vector(7 downto 0) := (others => '1');
      error_clock_i           : in std_logic_vector(7 downto 0) := (others => '1');
      frame_count_25_o        : out std_logic_vector(7 downto 0);
      new_frame_count_25_o    : out std_logic;
      set_frame_count_25_o    : out std_logic;
      frame_count_25_i        : in std_logic_vector(7 downto 0) := (others => '1');
      frame_count_30_o        : out std_logic_vector(7 downto 0);
      new_frame_count_30_o    : out std_logic;
      set_frame_count_30_o    : out std_logic;
      frame_count_30_i        : in std_logic_vector(7 downto 0) := (others => '1');
      system_o                : out std_logic_vector(11 downto 0);
      pattern_o               : out std_logic_vector(15 downto 0);
      timing_o                : out std_logic_vector(23 downto 0);
      click_timing_o          : out std_logic_vector(11 downto 0);
      audio_level_index_o     : out std_logic_vector(7 downto 0);
      audio_signal_index_o    : out std_logic_vector(7 downto 0);
      bmp_v_position_o        : out std_logic_vector(15 downto 0);
      bmp_h_position_o        : out std_logic_vector(15 downto 0);
      bmp_h_size_o            : out std_logic_vector(7 downto 0);
      bmp_v_size_o            : out std_logic_vector(7 downto 0);
      bmp_scale_o             : out std_logic_vector(7 downto 0);
      bmp_back_text_color_o   : out std_logic_vector(7 downto 0);
      bmp_movement_o          : out std_logic_vector(7 downto 0);
      bmp_on_zoff_o           : out std_logic;
      text_ram_clk_i          : in  std_logic                     := '0';              -- clock for reading text ram
      text_ram_addr_i         : in  std_logic_vector(8 downto 0)  := (others => '0');
      text_ram_en_i           : in  std_logic                     := '0';              -- enable signal for reading text ram
      text_ram_data_o         : out std_logic_vector( 7 downto 0)
   );
end component;

component rocket_io_top
   port(
      brefclk_i         : in std_logic;
      brefclk2_i        : in std_logic;
      use_clk2_i        : in std_logic;
      sync_reset_i      : in std_logic;
      power_down_i      : in std_logic;
      data_clk_i        : in std_logic;
      data_i            : in std_logic_vector (19 downto 0);
      fifo_error_o      : out std_logic;
      sdi_p_o           : out std_logic;     -- serial video outputs:
      sdi_n_o           : out std_logic
   );
end component;

--component glitch_remover
--   port (
--      clk_i             : in std_logic;
--      signal_i          : in std_logic;
--      clean_signal_o    : out std_logic
--   );
--end component;


--component period_dual_count
--   generic(
--      count_val1        : integer;
--      count_val2        : integer;
--      tick_val1         : integer := 2;
--      tick_val2         : integer := 1;
--      bits1             : integer;
--      bits2             : integer
--   );
--   Port(
--      mreset_i          : in std_logic;
--      clk_i             : in std_logic;
--      sync_reset_i      : in std_logic;
--      sync_o            : out std_logic
--   );
--end component;


--component sync_genlock_regen
--   generic (
--      count_val1        : integer;        -- the period of sync is count_val1*count_val2
--      count_val2        : integer;
--      bits1             : integer;        -- number of bits in counters
--      bits2             : integer
--   );
--   port (
--      mreset_i          : in std_logic;
--      clk_i             : in std_logic;
--      sync_i            : in std_logic;
--      sync_clean_o      : out std_logic;
--      genlock_ok_o      : out std_logic
--   );
--end component;


begin


--------------------------------------------------------------
-- Pll control and genlock regeneration :
--------------------------------------------------------------
clock_and_genlock_control : clock_genlock_controller
   generic map (
      use_vcxo_zics        => use_vcxo
   )                       
   port map (              
      clk_27_i             => clk_27,
      clk_1484_i           => clk1,
      clk_1485_i           => clk2,
      f8g_i                => f8g_i,            -- 160ms sync pulse input
      f4m_i                => f4m_i,            -- (1.001/15)s sync pulse input
--      vcxo_control_1_o     => vcxo_control_1_o,
      dac_sclk_1_o         => dac_sclk_1,
      dac_dout_1_o         => dac_dout_1,
      dac_zsync_1_o        => dac_zsync_1,
--      vcxo_control_2_o     => vcxo_control_2_o,
      dac_sclk_2_o         => dac_sclk_2,
      dac_dout_2_o         => dac_dout_2,
      dac_zsync_2_o        => dac_zsync_2,
      secm_tick_1484_o     => secm_tick_1484,   -- 1.001s tick output in 1484 clk domain
      sec_tick_1485_o      => sec_tick_1485,    -- 1s tick output
      secm_tick_1485_o     => secm_tick_1485,   -- 1.001s tick output in 1485 clk domain
      frame_count_25_i     => frame_count_25_command,    
      new_frame_count_25_i => new_frame_count_25_command,
      set_frame_count_25_i => set_frame_count_25_command,
      frame_count_25_o     => frame_count_25_actual,     
      frame_count_30_i     => frame_count_30_command,     
      new_frame_count_30_i => new_frame_count_30_command,
      set_frame_count_30_i => set_frame_count_30_command,
      frame_count_30_o     => frame_count_30_actual,     
      warning_o            => warning_clock,
      error_o              => error_clock,
      debug_o              => clock_debug
   );


-- mapping of timing signals:   
f4m_1484       <= secm_tick_1484;
f4m_1485       <= secm_tick_1485;
f8g            <= sec_tick_1485;
f2398          <= secm_tick_1484;
f24            <= sec_tick_1485;
f30            <= sec_tick_1485;


--------------------------------------------------------------
-- Master reset delay, 10ms tick generation and led control :
--------------------------------------------------------------
master_reset_delaying : master_reset_delay
generic map (
   sticky_led        => "0011"
)
port map(
   zreset_i          => mreset_i,
   clk_27_i          => clk_27,
   led_i             => led_status,
   led_off_i         => not cs1_i,
   led_o             => led,
   tick_10ms_o       => tick_10ms,
   reset_o           => reset
);

led_o <= led;


-------------------------------------------------------------
-- Status :
-------------------------------------------------------------

module_error   <= error_ch1 or error_ch2 or error_ch3 or error_ch4 or error_clock(7) or error_clock(6) or error_clock(5) or error_clock(4) or error_clock(3) or error_clock(2) or error_clock(1) or error_clock(0);

module_warning <= warning_clock(7) or warning_clock(6) or warning_clock(5) or warning_clock(4) or warning_clock(3) or warning_clock(2) or warning_clock(1) or warning_clock(0);


led_status(0) <= module_error;   -- error signal to mainframe before error controller

led_status(1) <= module_warning; -- or-ing of all warnings

-- PLL warnings:
--               pll 1484 warning or pll 1485 warning
led_status(2) <= warning_clock(1) or warning_clock(0);

-- Genlock warnings:
--                f4m 27 to 148   or  f8g 27 to 148   or      f4m         or f4m frame count  or      f8g         or f8g frame count 
led_status(3) <= warning_clock(7) or warning_clock(6) or warning_clock(5) or warning_clock(4) or warning_clock(3) or warning_clock(2);


-------------------------------------------------------------
-- Status to mainframe (Changeover):
-------------------------------------------------------------

module_error_controller : error_controller
port map(
   clk_i                => clk_27,
   parameter_change_i   => not cs1_i,
   error_i              => module_error,
   error_o              => module_error_to_main,
   tick_10ms_o          => open
);

ok1_o <= '1'; --not module_error_to_main;


-------------------------------------------------------------
-- Serial interface to report status:
-------------------------------------------------------------
serial_interfacing : serial_interface
generic map (
   channel_id               => 0,
   use_8612_z8613_mapping   => '1'
)
port map (
   clk_i                    => clk_27,--clk2,
   reset_i                  => '0',
   cs_i                     => cs1_i,
   sck_i                    => sck_i ,
   mosi_i                   => mosi_i,
   miso_o                   => miso_module,
   status_channel_i         => "11111111",
   warning_clock_i          => warning_clock,
   error_clock_i            => error_clock,
   frame_count_25_o         => frame_count_25_command,
   new_frame_count_25_o     => new_frame_count_25_command,
   set_frame_count_25_o     => set_frame_count_25_command,
   frame_count_25_i         => frame_count_25_actual,
   frame_count_30_o         => frame_count_30_command,
   new_frame_count_30_o     => new_frame_count_30_command,
   set_frame_count_30_o     => set_frame_count_30_command,
   frame_count_30_i         => frame_count_30_actual
);

miso_o <= miso_module and miso_ch1 and miso_ch2 and miso_ch3 and miso_ch4;


--------------------------------------------------------------
-- Differential clock inputs:
--------------------------------------------------------------
diff_buf_brefclk : IBUFGDS
generic map (IOSTANDARD => "LVDS_25_DT") -- generic map (IOSTANDARD => "LVDS_25")
port map (
   O  => brefclk,
   I  => brefclk_p_i,
   IB => brefclk_n_i
);


diff_buf_brefclk2 : IBUFGDS
generic map (IOSTANDARD => "LVDS_25_DT")  -- generic map (IOSTANDARD => "LVDS_25")
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


clk27_for_logic : BUFG
port map (
   I => clk_27_i,
   O => clk_27
);


--------------------------------------------
-- Generator channel 1
--------------------------------------------
HD_Gen_Channel_1 : HD_Gen_channel
   generic map (
      channel_id              => 1,
      pc_enable_mode          => '0',
      bypass_scrambler        => '0',
      timing_dummy_data       => '0',
      use_zero_timing         => '1',
      use_8612_z8613_mapping  => '1',  -- don't change!
      use_bit_toggle          => '0'
   )
   port map (
      reset_i           => reset(0),
      reset_delayed_i   => reset(3),
      f1485_i           => clk2,
      clk_27_i          => clk_27,
      brefclk_i         => brefclk,
      brefclk2_i        => brefclk2,
      tick_10ms_i       => tick_10ms,
      f4m_1484_i        => f4m_1484,
      f4m_1485_i        => f4m_1485,
      f8g_i             => f8g,
      f2398_i           => f2398,
      f24_i             => f24,
      f30_i             => f30,
      cs_i              => cs1_i,
      sck_i             => sck_i,
      mosi_i            => mosi_i,
      miso_o            => miso_ch1,
      sd_zhd_o          => sd_zhd_1,
      mgt_fifo_err_i    => ch_1_mgt_fifo_err,
      mgt_power_down_o  => ch_1_mgt_power_down,
      mgt_sync_reset_o  => ch_1_mgt_sync_reset,
      mgt_use_clk2_o    => ch_1_mgt_use_clk2,
      mgt_par_data_a_o  => ch_1_mgt_par_data_a,
      mgt_par_data_b_o  => ch_1_mgt_par_data_b,
      mgt_data_clk_o    => ch_1_mgt_data_clk,
      video_reset_o     => open,
      error_o           => error_ch1
   );

--------------------------------------------
-- Generator channel 2
--------------------------------------------
HD_Gen_Channel_2 : HD_Gen_channel
   generic map (
      channel_id              => 2,
      pc_enable_mode          => '0',
      bypass_scrambler        => '0',
      timing_dummy_data       => '0',
      use_zero_timing         => '1',
      use_8612_z8613_mapping  => '1',  -- don't change!
      use_bit_toggle          => '0'
   )
   port map (
      reset_i           => reset(0),
      reset_delayed_i   => reset(3),
      f1485_i           => clk2,
      clk_27_i          => clk_27,
      brefclk_i         => brefclk,
      brefclk2_i        => brefclk2,
      tick_10ms_i       => tick_10ms,
      f4m_1484_i        => f4m_1484,
      f4m_1485_i        => f4m_1485,
      f8g_i             => f8g,
      f2398_i           => f2398,
      f24_i             => f24,
      f30_i             => f30,
      cs_i              => cs1_i,
      sck_i             => sck_i,
      mosi_i            => mosi_i,
      miso_o            => miso_ch2,
      sd_zhd_o          => sd_zhd_2,
      mgt_fifo_err_i    => ch_2_mgt_fifo_err,
      mgt_power_down_o  => ch_2_mgt_power_down,
      mgt_sync_reset_o  => ch_2_mgt_sync_reset,
      mgt_use_clk2_o    => ch_2_mgt_use_clk2,
      mgt_par_data_a_o  => ch_2_mgt_par_data_a,
      mgt_par_data_b_o  => ch_2_mgt_par_data_b,
      mgt_data_clk_o    => ch_2_mgt_data_clk,
      video_reset_o     => open,
      error_o           => error_ch2
   );

--------------------------------------------
-- Generator channel 3
--------------------------------------------
HD_Gen_Channel_3 : HD_Gen_channel
   generic map (
      channel_id              => 3,
      pc_enable_mode          => '0',
      bypass_scrambler        => '0',
      timing_dummy_data       => '0',
      use_zero_timing         => '1',
      use_8612_z8613_mapping  => '1',  -- don't change!
      use_bit_toggle          => '0'
   )
   port map (
      reset_i           => reset(0),
      reset_delayed_i   => reset(3),
      f1485_i           => clk2,
      clk_27_i          => clk_27,
      brefclk_i         => brefclk,
      brefclk2_i        => brefclk2,
      tick_10ms_i       => tick_10ms,
      f4m_1484_i        => f4m_1484,
      f4m_1485_i        => f4m_1485,
      f8g_i             => f8g,
      f2398_i           => f2398,
      f24_i             => f24,
      f30_i             => f30,
      cs_i              => cs1_i,
      sck_i             => sck_i,
      mosi_i            => mosi_i,
      miso_o            => miso_ch3,
      sd_zhd_o          => sd_zhd_3,
      mgt_fifo_err_i    => ch_3_mgt_fifo_err,
      mgt_power_down_o  => ch_3_mgt_power_down,
      mgt_sync_reset_o  => ch_3_mgt_sync_reset,
      mgt_use_clk2_o    => ch_3_mgt_use_clk2,
      mgt_par_data_a_o  => ch_3_mgt_par_data_a,
      mgt_par_data_b_o  => open,
      mgt_data_clk_o    => ch_3_mgt_data_clk,
      video_reset_o     => open,
      error_o           => error_ch3
   );

--------------------------------------------
-- Generator channel 4
--------------------------------------------
HD_Gen_Channel_4 : HD_Gen_channel
   generic map (
      channel_id              => 4,
      pc_enable_mode          => '0',
      bypass_scrambler        => '1',
      timing_dummy_data       => '1',
      use_zero_timing         => '1',
      use_8612_z8613_mapping  => '1',  -- don't change!
      use_bit_toggle          => '0'
   )
   port map (
      reset_i           => reset(0),
      reset_delayed_i   => reset(3),
      f1485_i           => clk2,
      clk_27_i          => clk_27,
      brefclk_i         => brefclk,
      brefclk2_i        => brefclk2,
      tick_10ms_i       => tick_10ms,
      f4m_1484_i        => f4m_1484,
      f4m_1485_i        => f4m_1485,
      f8g_i             => f8g,
      f2398_i           => f2398,
      f24_i             => f24,
      f30_i             => f30,
      cs_i              => cs1_i,
      sck_i             => sck_i,
      mosi_i            => mosi_i,
      miso_o            => miso_ch4,
      sd_zhd_o          => sd_zhd_4,
      mgt_fifo_err_i    => ch_4_mgt_fifo_err,
      mgt_power_down_o  => ch_4_mgt_power_down,
      mgt_sync_reset_o  => ch_4_mgt_sync_reset,
      mgt_use_clk2_o    => ch_4_mgt_use_clk2,
      mgt_par_data_a_o  => ch_4_mgt_par_data_a,
      mgt_par_data_b_o  => open,
      mgt_data_clk_o    => ch_4_mgt_data_clk,
      video_reset_o     => open,
      error_o           => error_ch4
   );


---------------------------------------------------------------------------
-- Select between PT8612 and PT8613 implementation:
---------------------------------------------------------------------------
pt8612_or_pt8613_selecting : process
begin
   if pt8612_zpt8613 = '1' then  -- PT8612:
      ch_1_mgt_fifo_err    <= mgt_1_fifo_err;
      ch_2_mgt_fifo_err    <= mgt_2_fifo_err;
      ch_3_mgt_fifo_err    <= mgt_3_fifo_err;
      ch_4_mgt_fifo_err    <= mgt_4_fifo_err;
      mgt_1_power_down     <= ch_1_mgt_power_down;
      mgt_2_power_down     <= ch_2_mgt_power_down;
      mgt_3_power_down     <= ch_3_mgt_power_down;
      mgt_4_power_down     <= ch_4_mgt_power_down;
      mgt_1_sync_reset     <= ch_1_mgt_sync_reset;
      mgt_2_sync_reset     <= ch_2_mgt_sync_reset;
      mgt_3_sync_reset     <= ch_3_mgt_sync_reset;
      mgt_4_sync_reset     <= ch_4_mgt_sync_reset;
      mgt_1_use_clk2       <= ch_1_mgt_use_clk2;
      mgt_2_use_clk2       <= ch_2_mgt_use_clk2;
      mgt_3_use_clk2       <= ch_3_mgt_use_clk2;
      mgt_4_use_clk2       <= ch_4_mgt_use_clk2;
      mgt_1_par_data       <= ch_1_mgt_par_data_a;
      mgt_2_par_data       <= ch_2_mgt_par_data_a;
      mgt_3_par_data       <= ch_3_mgt_par_data_a;
      mgt_4_par_data       <= ch_4_mgt_par_data_a;
      mgt_1_data_clk       <= ch_1_mgt_data_clk;
      mgt_2_data_clk       <= ch_2_mgt_data_clk;
      mgt_3_data_clk       <= ch_3_mgt_data_clk;
      mgt_4_data_clk       <= ch_4_mgt_data_clk;
      sd_zhd_1_o           <= sd_zhd_1;
      sd_zhd_2_o           <= sd_zhd_2;
      sd_zhd_3_o           <= sd_zhd_3;
      sd_zhd_4_o           <= sd_zhd_4;
   else                          -- PT8613:
      ch_1_mgt_fifo_err    <= mgt_1_fifo_err or mgt_2_fifo_err;
      ch_2_mgt_fifo_err    <= mgt_3_fifo_err or mgt_4_fifo_err;
      mgt_1_power_down     <= ch_1_mgt_power_down;
      mgt_2_power_down     <= ch_1_mgt_power_down;
      mgt_3_power_down     <= ch_2_mgt_power_down;
      mgt_4_power_down     <= ch_2_mgt_power_down;
      mgt_1_sync_reset     <= ch_1_mgt_sync_reset;
      mgt_2_sync_reset     <= ch_1_mgt_sync_reset;
      mgt_3_sync_reset     <= ch_2_mgt_sync_reset;
      mgt_4_sync_reset     <= ch_2_mgt_sync_reset;
      mgt_1_use_clk2       <= ch_1_mgt_use_clk2;
      mgt_2_use_clk2       <= ch_1_mgt_use_clk2;
      mgt_3_use_clk2       <= ch_2_mgt_use_clk2;
      mgt_4_use_clk2       <= ch_2_mgt_use_clk2;
      mgt_1_par_data       <= ch_1_mgt_par_data_a;
      mgt_2_par_data       <= ch_1_mgt_par_data_b;
      mgt_3_par_data       <= ch_2_mgt_par_data_a;
      mgt_4_par_data       <= ch_2_mgt_par_data_b;
      mgt_1_data_clk       <= ch_1_mgt_data_clk;
      mgt_2_data_clk       <= ch_1_mgt_data_clk;
      mgt_3_data_clk       <= ch_2_mgt_data_clk;
      mgt_4_data_clk       <= ch_2_mgt_data_clk;
      sd_zhd_1_o           <= sd_zhd_1;
      sd_zhd_2_o           <= sd_zhd_1;
      sd_zhd_3_o           <= sd_zhd_2;
      sd_zhd_4_o           <= sd_zhd_2;
   end if;
end process;


-----------------------------------------------------
-- MGT serializers (Rocket IO) :
-----------------------------------------------------
rocket_io_inst_1 :  rocket_io_top
	port map(
      brefclk_i            => brefclk,
      brefclk2_i           => brefclk2,
      use_clk2_i           => mgt_1_use_clk2,
      sync_reset_i         => mgt_1_sync_reset,
      power_down_i         => mgt_1_power_down,
      data_clk_i           => mgt_1_data_clk,
      data_i               => mgt_1_par_data,
      fifo_error_o         => mgt_1_fifo_err,
      sdi_p_o              => SDI_p_1_o,
      sdi_n_o              => sdi_n_1_o
   );

rocket_io_inst_2 :  rocket_io_top
	port map(
      brefclk_i            => brefclk,
      brefclk2_i           => brefclk2,
      use_clk2_i           => mgt_2_use_clk2,
      sync_reset_i         => mgt_2_sync_reset,
      power_down_i         => mgt_2_power_down,
      data_clk_i           => mgt_2_data_clk,
      data_i               => mgt_2_par_data,
      fifo_error_o         => mgt_2_fifo_err,
      sdi_p_o              => sdi_p_2_o,
      sdi_n_o              => sdi_n_2_o
   );

rocket_io_inst_3 :  rocket_io_top
	port map(
      brefclk_i            => brefclk,
      brefclk2_i           => brefclk2,
      use_clk2_i           => mgt_3_use_clk2,
      sync_reset_i         => mgt_3_sync_reset,
      power_down_i         => mgt_3_power_down,
      data_clk_i           => mgt_3_data_clk,
      data_i               => mgt_3_par_data,
      fifo_error_o         => mgt_3_fifo_err,
      sdi_p_o              => sdi_p_3_o,
      sdi_n_o              => sdi_n_3_o
   );

rocket_io_inst_4 :  rocket_io_top
	port map(
      brefclk_i            => brefclk,
      brefclk2_i           => brefclk2,
      use_clk2_i           => mgt_4_use_clk2,
      sync_reset_i         => mgt_4_sync_reset,
      power_down_i         => mgt_4_power_down,
      data_clk_i           => mgt_4_data_clk,
      data_i               => mgt_4_par_data,
      fifo_error_o         => mgt_4_fifo_err,
      sdi_p_o              => sdi_p_4_o,
      sdi_n_o              => sdi_n_4_o
   );


-------------------------------------------------------------
-- Mapping of output signals:
-------------------------------------------------------------
ok2_o <= '1';

-------------------------------------------------------------
-- Signals for debug:
-------------------------------------------------------------
debug_outputs_selecting : process
begin
   if output_debug_signals = '1' then
      -- VCXO dacs:
      blue_o(0)   <= dac_sclk_1;
      blue_o(1)   <= dac_dout_1;
      blue_o(2)   <= dac_zsync_1;
      blue_o(3)   <= dac_sclk_2;
      blue_o(4)   <= dac_dout_2;
      blue_o(5)   <= dac_zsync_2;
      
      -- Sec ticks:
      green_o(0)  <= clock_debug(0);
      green_o(1)  <= clock_debug(1);
      green_o(2)  <= clock_debug(2);
      green_o(3)  <= clock_debug(3);
      green_o(4)  <= clock_debug(4);
      green_o(5)  <= clock_debug(5);
      
      -- PLL divided clocks:
      red_o(0)    <= clock_debug(6);
      red_o(1)    <= clock_debug(7);
      red_o(2)    <= clock_debug(8);
      red_o(3)    <= clock_debug(9);
      red_o(4)    <= warning_clock(0);
      red_o(5)    <= warning_clock(1);
   else
      -- VCXO dacs:
      blue_o      <= (others => 'Z');
      -- Sec ticks:
      green_o     <= (others => 'Z');
      -- PLL divided clocks:
      red_o       <= (others => 'Z');
   end if;
end process;


----------------------------------------------------------------
---- Select if vcxo's or ICS clock circuits are used :
----------------------------------------------------------------
select_vcxo_ics_implementation : process
begin
   if use_vcxo = '1' then
      pll1_s_o       <= (others => 'Z');
      pll2_s_o       <= (others => 'Z');
      dac_sclk_1_o   <= dac_sclk_1;
      dac_dout_1_o   <= dac_dout_1;
      dac_zsync_1_o  <= dac_zsync_1;
      dac_sclk_2_o   <= dac_sclk_2;
      dac_dout_2_o   <= dac_dout_2;   
      dac_zsync_2_o  <= dac_zsync_2;
   else
      pll1_s_o <= "0110";
      pll2_s_o <= "0111";
      dac_sclk_1_o   <= 'Z';
      dac_dout_1_o   <= 'Z';
      dac_zsync_1_o  <= 'Z';
      dac_sclk_2_o   <= 'Z';
      dac_dout_2_o   <= 'Z'; 
      dac_zsync_2_o  <= 'Z';
   end if;
end process;

   
----------------------------------------------------------------
---- Remove glitches on input signals:
----------------------------------------------------------------
--glitch_removing_f4m_1485 : glitch_remover
--   port map(
--      clk_i             => clk2,
--      signal_i          => f4m_i,
--      clean_signal_o    => f4m_1485_grm
--   );
--
--glitch_removing_f4m_1484 : glitch_remover
--   port map(
--      clk_i             => clk1,
--      signal_i          => f4m_i,
--      clean_signal_o    => f4m_1484_grm
--   );
--
--glitch_removing_f8g : glitch_remover
--   port map(
--      clk_i             => clk2,
--      signal_i          => f8g_i,
--      clean_signal_o    => f8g_grm
--   );
--
--
------------------------------------------------------------------
------ Generate free running sync pulses with a frequency of 23.98 Hz
------------------------------------------------------------------
--sync2398_generation : period_dual_count
--generic map (
--   count_val1     => sync2398_count1,
--   count_val2     => sync2398_count2,
--   bits1          => sync2398_bits1,
--   bits2          => sync2398_bits2
--)
--port map (
--   mreset_i       => not reset(0),
--   clk_i          => clk1,      -- 148.4
--   sync_reset_i   => '0',
--   sync_o         => sync_2398
--);
--
--
----------------------------------------------------------------
---- Generate free running sync pulses with a frequency of 24 Hz
----------------------------------------------------------------
--sync24_generation : period_dual_count
--generic map (
--   count_val1     => sync24_count1,
--   count_val2     => sync24_count2,
--   bits1          => sync24_bits1,
--   bits2          => sync24_bits2
--)
--port map (
--   mreset_i       => not reset(0),
--   clk_i          => clk2,      -- 148.5
--   sync_reset_i   => '0',
--   sync_o         => sync_24
--);
--
--
----------------------------------------------------------------
---- Generate free running sync pulses with a frequency of 30 Hz
----------------------------------------------------------------
--sync30_generation : period_dual_count
--generic map (
--   count_val1     => sync30_count1,
--   count_val2     => sync30_count2,
--   bits1          => sync30_bits1,
--   bits2          => sync30_bits2
--)
--port map (
--   mreset_i       => not reset(0),
--   clk_i          => clk2,      -- 148.5
--   sync_reset_i   => '0',
--   sync_o         => sync_30
--);
--
--
----------------------------------------------------------------
---- Genlock to the mainframe f4m signal
---- for 30/1.001 and 60/1.001 HD systems
----------------------------------------------------------------
--f4m_genlock_1484_regen : sync_genlock_regen
--   generic map (
--      count_val1     => f4m_1484_count1,
--      count_val2     => f4m_1484_count2,
--      bits1          => f4m_1484_bits1,
--      bits2          => f4m_1484_bits2
--   )
--   port map (
--      mreset_i       => not reset(1),
--      clk_i          => clk1,  --148.4
--      sync_i         => f4m_1484_grm,
--      sync_clean_o   => f4m_1484_clean,
--      genlock_ok_o   => f4m_1484_genlock_ok
--   );
--
--
----------------------------------------------------------------
---- Genlock to the mainframe f4m signal
---- for SD 525 (NTSC) system
----------------------------------------------------------------
--f4m_genlock_1485_regen : sync_genlock_regen
--   generic map (
--      count_val1     => f4m_1485_count1,
--      count_val2     => f4m_1485_count2,
--      bits1          => f4m_1485_bits1,
--      bits2          => f4m_1485_bits2
--   )
--   port map (
--      mreset_i       => not reset(1),
--      clk_i          => clk2,     --148.5
--      sync_i         => f4m_1485_grm,
--      sync_clean_o   => f4m_1485_clean,
--      genlock_ok_o   => f4m_1485_genlock_ok
--   );
--
--
----------------------------------------------------------------
---- Genlock to the mainframe f8g signal for 25 and 50 Hz systems
----------------------------------------------------------------
--f8g_genlock_regen : sync_genlock_regen
--   generic map (
--      count_val1     => f8g_count1,
--      count_val2     => f8g_count2,
--      bits1          => f8g_bits1,
--      bits2          => f8g_bits2
--   )
--   port map (
--      mreset_i       => not reset(1),
--      clk_i          => clk2,        -- 148.5
--      sync_i         => f8g_grm,
--      sync_clean_o   => f8g_clean,
--      genlock_ok_o   => f8g_genlock_ok
--   );


--debug <= "0000";


end Behavioral;