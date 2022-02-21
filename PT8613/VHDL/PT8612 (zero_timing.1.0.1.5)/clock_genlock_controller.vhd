library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity clock_genlock_controller is
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
end clock_genlock_controller;


architecture Behavioral of clock_genlock_controller is

signal f1484_pll_ref                : std_logic;
signal vcxo_1484_dec_falling_tick   : std_logic;
signal secm_27                      : std_logic;
signal f1485_pll_ref                : std_logic;
signal f1484_pll_fb                 : std_logic;
signal f1485_pll_fb                 : std_logic;
signal vcxo_1485_dec_falling_tick   : std_logic;
signal sec_27                       : std_logic;
signal secm_tick_27                 : std_logic;
signal pll_1484_locked              : std_logic;
signal pll_1485_locked              : std_logic;
signal m_trans_pulse                : std_logic;
signal m_trans_count                : std_logic_vector(6 downto 0);
signal genlock_f4m_warning          : std_logic_vector(1 downto 0);
signal genlock_f4m_error            : std_logic_vector(1 downto 0);
signal genlock_f8g_warning          : std_logic_vector(1 downto 0);
signal genlock_f8g_error            : std_logic_vector(1 downto 0);
signal secm_27_to_1484_warning      : std_logic;
signal sec_27_to_1485_warning       : std_logic;
signal secm_27_to_1485_warning      : std_logic;
signal secm_27_to_1484_error        : std_logic;
signal sec_27_to_1485_error         : std_logic;
signal secm_27_to_1485_error        : std_logic;
signal pll_1484_error               : std_logic;
signal pll_1484_warning             : std_logic;
signal pll_1485_error               : std_logic;
signal pll_1485_warning             : std_logic;
signal secm_pulse_delayed           : std_logic_vector(1 downto 0);
                                    
signal ics_secm_tick_1484           : std_logic;
signal ics_sec_tick_1485            : std_logic;
signal ics_secm_tick_1485           : std_logic;
signal vcxo_secm_tick_1484          : std_logic;
signal vcxo_sec_tick_1485           : std_logic;
signal vcxo_secm_tick_1485          : std_logic;


component genlock_counter
   generic (
      pll_per                 : integer;           -- decimation of clk_i to pll, (91)
      half_frame_per          : integer;           -- count on pll tick           (9900)
      sync_per                : integer;           -- count on frame tick         (2)
      fps_25_z30              : std_logic;         -- 1 : 25, 0 : 30 frames per ~second
      ref_step_debug          : std_logic          -- when high introducese step of pll_ref_o to test pll step response
   );
   port (
      clk_i                   : in std_logic;      -- clock
      sync_i                  : in std_logic;      -- sync input
      trans_tick_i            : in std_logic;      -- tick that needs to be transferred to 148.5MHz domain
      trans_pulse_o           : out std_logic;     -- indicates that a pulse is present that needs to be transferred
      trans_count_o           : out std_logic_vector(6 downto 0);  -- position of pulse that needs to be transferred
      pll_ref_o               : out std_logic;     -- signal to phase comparator
      sec_pulse_o             : out std_logic;     -- second pulse output (duration = pll period)
      sec_tick_advanced_o     : out std_logic;     -- second tick at falling edge of pll_ref_o (advanced pll_count_at_falling+1)
      frame_count_i           : in std_logic_vector(7 downto 0); -- frame number in a second period
      new_frame_count_i       : in std_logic;      -- new frame count value set from serial interface
      set_frame_count_i       : in std_logic;      -- when asserted the frame counter value is set to match the temporary frame counter
      frame_count_o           : out std_logic_vector(7 downto 0); -- frame number in a second period
      warning_o               : out std_logic_vector(1 downto 0); -- a single or more sync period fails & a single or more failing frame count set command
      error_o                 : out std_logic_vector(1 downto 0)  -- repetitive failing sync periods & repetitive failing frame count set commands
   );
end component;


component genlock_regen_148
   generic (
      tick_per                : integer;           -- Period on 10 (148.5/1.001) or 11 (148.5) used to make 27MHz tick on the 148.5MHz clock
      pll_per_div_tick_per    : integer;           -- decimation of clk_148_i to pll, 148.5 : 495/11 (45) or 148.5/1.001 : 500/10 (50)
      tick_count_at_tick      : integer;
      pll_count_at_tick       : integer;           -- Value of pll_count where the sec_tick_o is placed
      tick_count_dec_falling  : integer;           -- Value of tick_count when tick_count_dec_falling occurs
      pll_count_dec_falling   : integer            -- Value of pll_count when tick_count_dec_falling occurs
   );
   port (
      clk_148_i               : in std_logic;
      sec_pulse_i             : in std_logic;      -- sync pulse input from genlock_counter
      trans_pulse_i           : in std_logic;      -- indicates that a pulse from 27MHz domain is present
      trans_count_i           : in std_logic_vector(6 downto 0);  -- position of pulse that needs to be transferred
      vcxo_dec_falling_tick_i : in std_logic;      -- falling edge of vcxo divided
      sec_tick_o              : out std_logic;     -- second (or 1.001s) tick output
      trans_tick_o            : out std_logic      -- transferred tick
   );
end component;


component genlock_regen_27_to_148
   generic (
      count_val1           : integer;              -- the period of sync is count_val1*count_val2
      count_val2           : integer;
      bits1                : integer;              -- number of bits in counters
      bits2                : integer
   );
   port (
      mreset_i             : in std_logic;         -- Master reset
      clk_i                : in std_logic;         -- sample clock
      sync_i               : in std_logic;         -- sync input
      sync_clean_o         : out std_logic;        -- reconstructed sync output
      warning_o            : out std_logic;
      error_o              : out std_logic
   );
end component;


component digital_pll is
   generic (
      vcxo_div_per         : integer         -- decimation of clk_i to pll, (500)
   );
   port (
      clk_vcxo_i              : in std_logic;
      ref_decimated_i         : in std_logic;
      vcxo_dec_falling_tick_o : out std_logic;  -- tick at falling edge of decimated vcxo to phase comparator 
      pdm_o                   : out std_logic;  -- pulse density modulated signal
      dac_sclk_o              : out std_logic;
      dac_dout_o              : out std_logic;
      dac_zsync_o             : out std_logic;
      clk_vcxo_decimated_o    : out std_logic; 
      lock_warning_o          : out std_logic;
      lock_error_o            : out std_logic
   );
end component;


begin


------------------------------------------------------------------------
-- f4m genlock, 148.4MHz pll and 1.001s tick in 148.4MHz clock domain:
------------------------------------------------------------------------
f4m_genlock : genlock_counter
   generic map(
      pll_per                 => 91,                  -- decimation of clk_i to pll, (91)
      half_frame_per          => 4950,                -- count on pll tick           (4950)
      sync_per                => 2,                   -- count on frame tick         (2)
      fps_25_z30              => '0',                 -- count on frame tick         (30)
      ref_step_debug          => '0'                  -- when high introduce step of pll_ref_o to test pll step response
   )
   port map(
      clk_i                   => clk_27_i,
      sync_i                  => f4m_i,
      trans_tick_i            => '0',
      trans_pulse_o           => open,
      trans_count_o           => open,
      pll_ref_o               => f1484_pll_ref,
      sec_pulse_o             => secm_27,
      sec_tick_advanced_o     => secm_tick_27,
      frame_count_i           => frame_count_30_i,
      new_frame_count_i       => new_frame_count_30_i,
      set_frame_count_i       => set_frame_count_30_i,
      frame_count_o           => frame_count_30_o,
      warning_o               => genlock_f4m_warning,
      error_o                 => genlock_f4m_error
   );

secm_tick_regeneration : genlock_regen_148
   generic map(
      tick_per                => 10,                  -- decimation factor is 10*50 = 500
      pll_per_div_tick_per    => 50,
      tick_count_at_tick      => 3,    --2, --8,
      pll_count_at_tick       => 29,   --49
      tick_count_dec_falling  => 0, --9,    --tick_per-1
      pll_count_dec_falling   => 50 --49    --pll_per_div_tick_per-1
   )
   port map(
      clk_148_i               => clk_1484_i,
      sec_pulse_i             => secm_27,             -- sync pulse input from genlock_counter
      trans_pulse_i           => '0',
      trans_count_i           => (others => '0'),
      vcxo_dec_falling_tick_i => vcxo_1484_dec_falling_tick,
      sec_tick_o              => vcxo_secm_tick_1484, -- 1.001 sec tick output
      trans_tick_o            => open                 -- 1.001 sec tick output
   );

digital_pll_f1484 : digital_pll
   generic map(
      vcxo_div_per            => 500
   )
   port map(
      clk_vcxo_i              => clk_1484_i,
      ref_decimated_i         => f1484_pll_ref,
      vcxo_dec_falling_tick_o => vcxo_1484_dec_falling_tick,
      pdm_o                   => vcxo_control_1_o,    -- control voltage to loopfilter,
      dac_sclk_o              => dac_sclk_1_o,
      dac_dout_o              => dac_dout_1_o,
      dac_zsync_o             => dac_zsync_1_o,
      clk_vcxo_decimated_o    => f1484_pll_fb,        -- for debug
      lock_warning_o          => pll_1484_warning,
      lock_error_o            => pll_1484_error
   );

--------------------------------------------------------------
-- When the clock is generated using ics plls the 27mhz sec
-- pulse is transferred to th 148.x mhz domains using
-- old-style genlock:
--------------------------------------------------------------
secm_transfer_to_1484 : genlock_regen_27_to_148
   generic map (
      count_val1     => 13500,
      count_val2     => 11000,
      bits1          => 14,
      bits2          => 14
   )
   port map (
      mreset_i       => '1',
      clk_i          => clk_1484_i,
      sync_i         => secm_27,
      sync_clean_o   => ics_secm_tick_1484,
      warning_o      => secm_27_to_1484_warning,
      error_o        => secm_27_to_1484_error
   );

------------------------------------------------------------------------
-- f8g genlock, 148.5MHz pll and 1s and 1.001s tick in 148.5MHz clock domain:
------------------------------------------------------------------------
f8g_genlock : genlock_counter
   generic map(
      pll_per                 => 90,                  -- decimation of clk_i to pll, (91)
      half_frame_per          => 6000,                -- count on pll tick           (9900)
      sync_per                => 4,                   -- count on frame tick         (2)
      fps_25_z30              => '1',                 -- count on frame tick         (30)
      ref_step_debug          => '0'                  -- when high introducese step of pll_ref_o to test pll step response
   )
   port map(
      clk_i                   => clk_27_i,
      sync_i                  => f8g_i,
      trans_tick_i            => secm_tick_27,
      trans_pulse_o           => m_trans_pulse,
      trans_count_o           => m_trans_count,
      pll_ref_o               => f1485_pll_ref,
      sec_pulse_o             => sec_27,
      frame_count_i           => frame_count_25_i,
      new_frame_count_i       => new_frame_count_25_i,
      set_frame_count_i       => set_frame_count_25_i,
      frame_count_o           => frame_count_25_o,
      warning_o               => genlock_f8g_warning,
      error_o                 => genlock_f8g_error
   );

sec_tick_regeneration : genlock_regen_148
   generic map(
      tick_per                => 11,                  -- decimation factor is 11*45 = 495
      pll_per_div_tick_per    => 45,
      tick_count_at_tick      => 5,    --3,  --0,
      pll_count_at_tick       => 22,   --26, --0
      tick_count_dec_falling  => 1, --0,    --tick_per-1
      pll_count_dec_falling   => 41    --44 --pll_per_div_tick_per-1
   )
   port map(
      clk_148_i               => clk_1485_i,
      sec_pulse_i             => sec_27,              -- sync pulse input from genlock_counter
      trans_pulse_i           => m_trans_pulse,
      trans_count_i           => m_trans_count,
      vcxo_dec_falling_tick_i => vcxo_1485_dec_falling_tick,
      sec_tick_o              => vcxo_sec_tick_1485,  -- 1 sec tick output
      trans_tick_o            => vcxo_secm_tick_1485  -- 1.001 sec tick output
   );

digital_pll_f1485 : digital_pll
   generic map(
      vcxo_div_per            => 495
   )
   port map(
      clk_vcxo_i              => clk_1485_i,
      ref_decimated_i         => f1485_pll_ref,
      vcxo_dec_falling_tick_o => vcxo_1485_dec_falling_tick,
      pdm_o                   => vcxo_control_2_o,  -- control voltage to loopfilter,
      dac_sclk_o              => dac_sclk_2_o,
      dac_dout_o              => dac_dout_2_o,
      dac_zsync_o             => dac_zsync_2_o,
      clk_vcxo_decimated_o    => f1485_pll_fb,      -- for debug
      lock_warning_o          => pll_1485_warning,
      lock_error_o            => pll_1485_error
   );

--------------------------------------------------------------
-- When the clock is generated using ics plls the 27mhz sec
-- pulse is transferred to th 148.x mhz domains using
-- old-style genlock:
--------------------------------------------------------------
sec_transfer_to_1485 : genlock_regen_27_to_148
   generic map (
      count_val1     => 13500,
      count_val2     => 11000,
      bits1          => 14,
      bits2          => 14
   )
   port map (
      mreset_i       => '1',
      clk_i          => clk_1485_i,
      sync_i         => sec_27,
      sync_clean_o   => ics_sec_tick_1485,
      warning_o      => sec_27_to_1485_warning,
      error_o        => sec_27_to_1485_error
   );

secm_transfer_to_1485 : genlock_regen_27_to_148
   generic map (
      count_val1     => 13500,
      count_val2     => 11011,
      bits1          => 14,
      bits2          => 14
   )
   port map (
      mreset_i       => '1',
      clk_i          => clk_1485_i,
      sync_i         => secm_27,
      sync_clean_o   => ics_secm_tick_1485,
      warning_o      => secm_27_to_1485_warning,
      error_o        => secm_27_to_1485_error
   );


----------------------------------------------------------------
-- Select between vcxo and ics implementation:
----------------------------------------------------------------
vcxo_ics_selecting : process
begin
   if use_vcxo_zics = '1' then   -- VCXO:
      warning_o         <= "00" & genlock_f4m_warning & genlock_f8g_warning & pll_1484_warning & pll_1485_warning;
      error_o           <= "00" & genlock_f4m_error & genlock_f8g_error & pll_1484_error & pll_1485_error;
      secm_tick_1484_o  <= vcxo_secm_tick_1484;
      sec_tick_1485_o   <= vcxo_sec_tick_1485;
      secm_tick_1485_o  <= vcxo_secm_tick_1485;
   else                          -- ICS:
      warning_o         <= (secm_27_to_1484_warning or secm_27_to_1485_warning) & sec_27_to_1485_warning & genlock_f4m_warning & genlock_f8g_warning & pll_1484_warning & pll_1485_warning;
      error_o           <= (secm_27_to_1484_error or secm_27_to_1485_error) & sec_27_to_1485_error & genlock_f4m_error & genlock_f8g_error & "00";
      secm_tick_1484_o  <= ics_secm_tick_1484;
      sec_tick_1485_o   <= ics_sec_tick_1485; 
      secm_tick_1485_o  <= ics_secm_tick_1485;
   end if;
end process;   


----------------------------------------------------------------
-- Debug:
----------------------------------------------------------------
debug_o(0) <= vcxo_secm_tick_1484;
debug_o(1) <= vcxo_secm_tick_1485;
debug_o(2) <= vcxo_sec_tick_1485;
debug_o(3) <= ics_secm_tick_1484;
debug_o(4) <= ics_secm_tick_1485;
debug_o(5) <= ics_sec_tick_1485;
debug_o(6) <= f1484_pll_ref;
debug_o(7) <= f1484_pll_fb;
debug_o(8) <= f1485_pll_ref;
debug_o(9) <= f1485_pll_fb;


end Behavioral;