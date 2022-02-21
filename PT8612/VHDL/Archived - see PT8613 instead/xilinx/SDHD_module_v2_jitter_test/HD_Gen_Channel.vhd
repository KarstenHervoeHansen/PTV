library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;

----------------------------------------------------------------------------------------------
-- 2005-09-26: Created                    Thomas Holm Hansen
--
----------------------------------------------------------------------------------------------


entity HD_Gen_Channel is
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
end HD_Gen_Channel;


architecture Behavioral of HD_Gen_Channel is

signal use_f1484        : std_logic;
signal var_clk          : std_logic;
signal genlock_sync     : std_logic;
signal system           : std_logic_vector(7 downto 0);
signal pattern          : std_logic_vector(15 downto 0);
signal timing           : std_logic_vector(23 downto 0);
signal frame_start      : std_logic;
signal mute             : std_logic;
signal reset_channel    : std_logic;
signal MGT_power_down   : std_logic;

signal f4m_1484         : std_logic;
signal f4m_1485         : std_logic;
signal f8g              : std_logic;
signal f2398            : std_logic;
signal f24              : std_logic;
signal f30              : std_logic;

signal tsg_level        : std_logic;
signal sampled_tsg_lvl  : std_logic;
signal lvl_count        : std_logic_vector(2 downto 0);
signal spl_is_zero      : std_logic;
signal debug            : std_logic_vector(9 downto 0);


component bufgmux
   Port (
      I0 : in std_logic;
      I1 : in std_logic;
      S  : in std_logic;
      O  : out std_logic
   );
end component;


component channel_controller
   port (
      mreset_i                : in std_logic;
      reset_delayed_i         : in std_logic;
      clk_i                   : in std_logic;
      tick_10ms_i             : in std_logic;
      var_clk_i               : in std_logic;
      cs_i                    : in std_logic;
      sck_i                   : in std_logic;
      mosi_i                  : in std_logic;
      mute_o                  : out std_logic;
      use_f1484_o             : out std_logic;
      system_o                : out std_logic_vector(7 downto 0);
      pattern_o               : out std_logic_vector(15 downto 0);
      timing_o                : out std_logic_vector(23 downto 0);
      reset_channel_o         : out std_logic;
      MGT_power_down_o        : out std_logic
   );
end component;


component video_generator
   port(
      clk_i              : in std_logic;
      brefclk_i          : in std_logic;
      brefclk2_i         : in std_logic;
      use_clk2_i         : in std_logic;
      
      zreset_i           : in std_logic;
      mute_i             : in std_logic;
      MGT_power_down_i   : in std_logic;
      
      -- video parameters:
      system_i           : in std_logic_vector(7 downto 0);
      pattern_i          : in std_logic_vector(15 downto 0);
      
      use_f1484_o        : out std_logic;
      -- serial video output:
      sd_zhd_o           : out std_logic;
      sdi_p_o            : out std_logic;
      sdi_n_o            : out std_logic;
      
      -- timing status:
      timing_change_o    : out std_logic;
      rio_fifo_err_o     : out std_logic;
      
      -- debug:
      debug_o           : out std_logic_vector(9 downto 0)
   );
end component;


component frame_sync_delay
   port (
	   mreset_i       : in std_logic;                     -- Master reset
	   clk_i         	: in std_logic;                     -- sample clock
	   sync_i         : in std_logic;                     
      timing_i       : in std_logic_vector(23 downto 0); -- delay from sync reference
	                                                      -- Input parameters must always be valid
	   frame_sync_o	: out std_logic
	);
end component;


begin


--------------------------------------------
-- Select video clock:
--------------------------------------------
clock_selecting : bufgmux
   port map(
      I0 => brefclk_i,  --f1484_i,
      I1 => brefclk2_i, --f1485_i,
      S  => not use_f1484,
      O  => var_clk
   );

   
--------------------------------------------
-- Controller:
--------------------------------------------
system_controller : channel_controller
   port map (
      mreset_i          => mreset_i,
      reset_delayed_i   => reset_delayed_i,
      clk_i             => f1485_i,
      tick_10ms_i       => tick_10ms_i,
      var_clk_i         => var_clk,
      cs_i              => cs_i,
      sck_i             => sck_i,
      mosi_i            => mosi_i,
      mute_o            => mute,
      system_o          => system,
      pattern_o         => pattern,
      timing_o          => timing,
      reset_channel_o   => reset_channel,
      MGT_power_down_o  => MGT_power_down
   );

   
video_generator_instance : video_generator
   port map (
      clk_i                  => var_clk,       
      brefclk_i              => brefclk_i,      --148.4
      brefclk2_i             => brefclk2_i,     --148.5
      use_clk2_i             => not use_f1484,
      zreset_i               => reset_channel, 
      mute_i                 => mute,          
      MGT_power_down_i       => MGT_power_down,
      -- video parameters   
      system_i               => system,             
      pattern_i              => pattern,
      
      use_f1484_o            => use_f1484,
      -- serial video out   
      sd_zhd_o               => sd_zhd_o,
      sdi_p_o                => sdi_p_o,
      sdi_n_o                => sdi_n_o,
      -- timing status:     
      rio_fifo_err_o         => rio_fifo_err_o
   );


end Behavioral;