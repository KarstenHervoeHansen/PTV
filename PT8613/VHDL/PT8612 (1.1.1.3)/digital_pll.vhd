library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_arith.all;
USE ieee.std_logic_unsigned.all;

--------------------------------------------------------------------
-- Digital PLL :
-- See simulink file: pt8612_dpll_fpga.mld for documentation.
--------------------------------------------------------------------
entity digital_pll is
   generic (
      vcxo_div_per            : integer         -- decimation of clk_i to pll, (495/500)
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
end digital_pll;

architecture behavioral of digital_pll is

-- phase detector:
signal vcxo_dec_falling_tick        : std_logic;
signal clk_vcxo_decimated           : std_logic;
signal phase_diff                   : std_logic_vector(9 downto 0);

-- dac:
signal dac_enable                   : std_logic;
signal sd_dac_enable                : std_logic;
signal sd_dac_input                 : std_logic_vector(15 downto 0);
signal dac_input                    : std_logic_vector(15 downto 0);
signal load_sd_dac_data_temp        : std_logic;

--loop filter:
signal lf_enable                    : std_logic;
signal lf_enable_count              : std_logic_vector(1 downto 0);
signal lf_output                    : std_logic_vector(15 downto 0);


component ad5660_serial_out
   port (
      clk_148_i                  : in std_logic;
      data_i                     : in std_logic_vector(15 downto 0);
      new_data_i                 : in std_logic;
      sclk_o                     : out std_logic;
      dout_o                     : out std_logic;
      zsync_o                    : out std_logic
   );
end component;

component digital_pll_loopfilter
   port (
      clk_i                   : in std_logic;
      sample_tick_i           : in std_logic;
      phase_i                 : in std_logic_vector(9 downto 0);
      lf_out_o                : out std_logic_vector(15 downto 0)
   );
end component;

component digital_pll_sd_dac
   port (
      clk_i                   : in std_logic;
      data_i                  : in std_logic_vector(15 downto 0);
      enable_o                : out std_logic;
      pdm_o                   : out std_logic
   );
end component;

component digital_pll_phase_detector
   generic (
      vcxo_div_per            : integer         -- decimation of clk_i to pll, (495/500)
   );
   port (
      clk_vcxo_i              : in std_logic;
      ref_decimated_i         : in std_logic;
      vcxo_dec_falling_tick_o : out std_logic;  -- tick at falling edge of decimated vcxo to phase comparator
      clk_vcxo_decimated_o    : out std_logic;
      phase_diff_o            : out std_logic_vector(9 downto 0);
      lock_warning_o          : out std_logic;
      lock_error_o            : out std_logic
   );
end component;

begin

--                           --   --   --   --   --   --   --   --   --   --   --          --   --   --   --   --   --   --   --   --   --   -- 
-- clk_vcxo_i:              |  | |  | |  | |  | |  | |  | |  | |  | |  | |  | |  |        |  | |  | |  | |  | |  | |  | |  | |  | |  | |  | |  |
--                         -    -    -    -    -    -    -    -    -    -    -           -    -    -    -    -    -    -    -    -    -    -    
--                                       -----------------------------------------       --------------------------------------
-- ref_decimated_i:                     |                                                                                      |
--                         -------------                                                                                        --------------------------------
--                                               ----                                    
-- clk_ref_dec_rising:                          |    |                                   
--                         ---------------------      ----------------------------       ---------------------------------------------------------
--                         - ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---       - ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
-- phase_count:             X  7 X  6 X  5 X  4 X  3 X  2 X  1 X  0 X -1 X -2 X -3        X-247X-248X-249X-250X 249X 248X 247X 246X 245X 244X
--                         - ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---       - ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
--                         -------------------------- ----------------------------       ----------------------------------------------------------------
-- phase_diff:                                       X   7  (3*2-1)                           7                                                    
--                         -------------------------- ----------------------------       ----------------------------------------------------------------
--                                                                   -------------       ---------------------
-- clk_vcxo_decimated_o:                                            |                                         |
--                         -----------------------------------------                                           -------------------------------------
--                                                                                                        ----
-- period_end_delayed(1):                                                                                |    |
--                         -------------------------------------------------------       ----------------      --------------------------------------
--                         -------------------------------------------------------       --------------------- --------------------------------------------------
-- lf_int_reg:                                                                                                X                                                  
--                         -------------------------------------------------------       --------------------- --------------------------------------------------
--                         -------------------------- ----------------------------       --------------------- -------------------------------------------
-- lf_output:                                        X                                                        X       
--                         -------------------------- ----------------------------       --------------------- -------------------------------------------


--------------------------------------------------------------------
-- Phase detector :
--------------------------------------------------------------------
phase_detector : digital_pll_phase_detector
   generic map(
      vcxo_div_per            => vcxo_div_per
   )
   port map(
      clk_vcxo_i              => clk_vcxo_i,
      ref_decimated_i         => ref_decimated_i,        
      vcxo_dec_falling_tick_o => vcxo_dec_falling_tick,
      clk_vcxo_decimated_o    => clk_vcxo_decimated,
      phase_diff_o            => phase_diff,
      lock_warning_o          => lock_warning_o,
      lock_error_o            => lock_error_o  
   );

clk_vcxo_decimated_o    <= clk_vcxo_decimated;   
vcxo_dec_falling_tick_o <= vcxo_dec_falling_tick;

--------------------------------------------------------------------
-- generate lf_enable and dac_enable to slow down update rate of loopfilter and DAC:
--------------------------------------------------------------------
lf_enable_generation: process (clk_vcxo_i)
begin
   if clk_vcxo_i'event and clk_vcxo_i = '1' then
      if vcxo_dec_falling_tick = '1' then
         lf_enable_count <= lf_enable_count + 1;
      end if;
      if vcxo_dec_falling_tick = '1' and lf_enable_count = 0 then
         lf_enable <= '1';
      else
         lf_enable <= '0';
      end if;
      if vcxo_dec_falling_tick = '1' and lf_enable_count = 0 then
         dac_enable <= '1';
      else
         dac_enable <= '0';
      end if;
   end if;
end process;

--------------------------------------------------------------------
-- Loop filter :
--------------------------------------------------------------------
loopfilter : digital_pll_loopfilter
   port map (
      clk_i          => clk_vcxo_i,
      sample_tick_i  => lf_enable,
      phase_i        => phase_diff,
      lf_out_o       => lf_output
   );

--------------------------------------------------------------------
-- Serial output to external DAC (AD5660) :
--------------------------------------------------------------------
dac_input <= '0' & lf_output(15 downto 1);   -- lf_output,

serial_data_to_dac : ad5660_serial_out
   port map (
      clk_148_i   => clk_vcxo_i,
      data_i      => dac_input,
      new_data_i  => dac_enable,
      sclk_o      => dac_sclk_o,
      dout_o      => dac_dout_o,
      zsync_o     => dac_zsync_o
   );

--------------------------------------------------------------------
-- Single bit DAC :
-- 16-bit resolution 1st order single bit modulator with return to zero pulses
--------------------------------------------------------------------
dac_input_register : process (clk_vcxo_i)
begin
   if clk_vcxo_i'event and clk_vcxo_i = '1' then
      if vcxo_dec_falling_tick = '1' then
         load_sd_dac_data_temp <= '1';
      elsif sd_dac_enable = '1' and load_sd_dac_data_temp = '1' then
         load_sd_dac_data_temp <= '0';
         sd_dac_input <= lf_output;
      end if;
   end if;
end process;
   
single_bit_dac : digital_pll_sd_dac
   port map (
      clk_i       => clk_vcxo_i,
      data_i      => sd_dac_input,
      enable_o    => sd_dac_enable,
      pdm_o       => pdm_o
   );

end behavioral;