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
      f4m_1484_i        : in std_logic;      -- sync reference - HD systems
      f4m_1485_i        : in std_logic;      -- sync reference - SD NTSC
      f8g_i             : in std_logic;      -- sync reference
      f2398_i           : in std_logic;
      f24_i             : in std_logic;
      f30_i             : in std_logic;

--------- fra CPU ----------------------------------------------------------------------------
      ch_ram_data_i     : in  std_logic_vector( 7 downto 0); 
      ch_ram_ready_i    : in  std_logic;                     
      ch_ram_addr_o     : out std_logic_vector( 8 downto 0); 

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


signal use_f1484           : std_logic;
signal var_clk             : std_logic;
signal genlock_sync        : std_logic;
signal system              : std_logic_vector(7 downto 0);
signal pattern             : std_logic_vector(15 downto 0);
signal timing              : std_logic_vector(23 downto 0);
                           
signal frame_start         : std_logic;
signal mute                : std_logic;
signal reset_channel       : std_logic;
signal MGT_power_down      : std_logic;
                           
signal f4m_1484            : std_logic;
signal f4m_1485            : std_logic;
signal f8g                 : std_logic;
signal f2398               : std_logic;
signal f24                 : std_logic;
signal f30                 : std_logic;
                           
signal tsg_level           : std_logic;
signal sampled_tsg_lvl     : std_logic;
signal lvl_count           : std_logic_vector(2 downto 0);
signal spl_is_zero         : std_logic;
signal debug               : std_logic_vector(9 downto 0);
                           
signal click_timing        : std_logic_vector(11 downto 0);
signal click_modulation    : std_logic;
signal debug_pattern       : std_logic_vector(15 downto 0);

signal audio_level_index   : std_logic_vector(7 downto 0);
signal audio_signal_index  : std_logic_vector(7 downto 0);


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
      --// Signal to/fro RAM in serial SPI interface
      ch_ram_data_i           : in  std_logic_vector( 7 downto 0); --// data to read bus
      ch_ram_ready_i          : in  std_logic;                     --// data ready signal
      ch_ram_addr_o           : out std_logic_vector( 8 downto 0); --// address buss

      mute_o                  : out std_logic;
      use_f1484_o             : out std_logic;
      use_f1485_o             : out std_logic;
      system_o                : out std_logic_vector(7 downto 0);
      pattern_o               : out std_logic_vector(15 downto 0);
      timing_o                : out std_logic_vector(23 downto 0);
      click_timing_o          : out std_logic_vector(11 downto 0);
      audio_level_index_o     : out std_logic_vector(7 downto 0);
      audio_signal_index_o    : out std_logic_vector(7 downto 0);
      
      reset_channel_o         : out std_logic;
      MGT_power_down_o        : out std_logic;
      debugger_o              : out std_logic_vector(9 downto 0)
   );
end component;


component video_generator
   port(
   clk_i                : in std_logic;
   brefclk_i            : in std_logic;
   brefclk2_i           : in std_logic;
   use_clk2_i           : in std_logic;
                        
   zreset_i             : in std_logic;
   mute_i               : in std_logic;
   frame_start_i        : in std_logic;
   MGT_power_down_i     : in std_logic;

   -- video parameters:
   system_i             : in std_logic_vector(7 downto 0);
   pattern_i            : in std_logic_vector(15 downto 0);

   -- audio parameters:
   audio_level_index_i  : in std_logic_vector(7 downto 0);
   audio_signal_index_i : in std_logic_vector(7 downto 0);
   click_timing_i       : in std_logic_vector(11 downto 0);
   click_modulation_i   : in std_logic;

   -- serial video output:
   sd_zhd_o             : out std_logic;
   sdi_p_o              : out std_logic;
   sdi_n_o              : out std_logic;
                        
   -- timing status:    
   timing_change_o      : out std_logic;
   rio_fifo_err_o       : out std_logic;
                        
   -- debug:            
   debug_o              : out std_logic_vector(9 downto 0)
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
      mreset_i                      => mreset_i,
      reset_delayed_i               => reset_delayed_i,
      clk_i                         => f1485_i,
      tick_10ms_i                   => tick_10ms_i,
      var_clk_i                     => var_clk,
      ch_ram_addr_o                 => ch_ram_addr_o,
      ch_ram_data_i                 => ch_ram_data_i,
      ch_ram_ready_i                => ch_ram_ready_i,
      mute_o                        => mute,
      use_f1484_o                   => use_f1484,
      system_o                      => system,
      pattern_o                     => pattern,
      timing_o                      => timing,
      click_timing_o                => click_timing,
      audio_level_index_o           => audio_level_index,
      audio_signal_index_o          => audio_signal_index,
      reset_channel_o               => reset_channel,
      MGT_power_down_o              => MGT_power_down
   );

   
--------------------------------------------
-- remapping of parameters for debug:
--------------------------------------------   
click_modulation <= '0';  


--param_remapping : process (var_clk) begin
--   if var_clk'event and var_clk = '1' then
--      case pattern is
--      when X"0000" =>   debug_pattern <= X"0000";  
--      when X"0001" =>   debug_pattern <= X"0001";  
--      when X"0002" =>   debug_pattern <= X"0002";  
--
--      when X"0100" =>   debug_pattern <= X"0100";  
--      when X"0101" =>   debug_pattern <= X"0101";  
--      when X"0102" =>   debug_pattern <= X"0102";  
--      
--      when X"0200" =>   debug_pattern <= X"0216";  
--      when X"0201" =>   debug_pattern <= X"0400";  
--      when X"0202" =>   debug_pattern <= X"0500";  
--      when X"0001" =>   debug_pattern <= X"0700";  
--      when X"0002" =>   debug_pattern <= X"0800";  
--      when others  =>   debug_pattern <= X"0000";  
--      end case;
--   end if;
--end process;

  
debug_pattern <= pattern;


video_generator_instance : video_generator
   port map (
      clk_i                  => var_clk,
      brefclk_i              => brefclk_i,      --148.4
      brefclk2_i             => brefclk2_i,     --148.5
      use_clk2_i             => not use_f1484,
      zreset_i               => reset_channel,
      mute_i                 => mute,
      frame_start_i          => frame_start,
      MGT_power_down_i       => MGT_power_down,
      
      -- video parameters:
      system_i               => system,
      pattern_i              => debug_pattern,--pattern,
      
      -- audio parameters:
      click_timing_i         => click_timing, 
      click_modulation_i     => click_modulation,
      audio_level_index_i    => audio_level_index,  
      audio_signal_index_i   => audio_signal_index,

            -- serial video out:
      sd_zhd_o               => sd_zhd_o,
      sdi_p_o                => sdi_p_o,
      sdi_n_o                => sdi_n_o,
      
      -- timing status:
      timing_change_o        => timing_change_o,
      rio_fifo_err_o         => rio_fifo_err_o,
      debug_o                => debug
   );


frame_sync_delaying : frame_sync_delay
   port map (
      mreset_i          => reset_channel,
      clk_i             => var_clk,
      sync_i            => genlock_sync,
      timing_i          => timing,
      frame_sync_o      => frame_start
   );


-------------------------------------------------------------
-- Select genlock sync source:
-------------------------------------------------------------
sync_reclocking : process(reset_channel, var_clk)
begin
   if reset_channel = '0' then
      f4m_1484 <= '0';
      f4m_1485 <= '0';
      f8g      <= '0';
      f2398    <= '0';
      f24      <= '0';
      f30      <= '0';
   elsif var_clk'event and var_clk = '1' then
      f4m_1484 <= f4m_1484_i;
      f4m_1485 <= f4m_1485_i;
      f8g      <= f8g_i;
      f2398    <= f2398_i;
      f24      <= f24_i;
      f30      <= f30_i;
   end if;
end process;


sync_selecting : process(reset_channel, var_clk)
begin
   if reset_channel = '0' then
      genlock_sync <= '0';
   elsif var_clk'event and var_clk = '1' then
      case system is
         when X"A0" => genlock_sync <= f30;
         when X"B0" => genlock_sync <= f4m_1484;
         when X"A1" => genlock_sync <= f8g;
         when X"82" => genlock_sync <= f30;
         when X"92" => genlock_sync <= f4m_1484;
         when X"83" => genlock_sync <= f8g;
         when X"84" => genlock_sync <= f24;
         when X"94" => genlock_sync <= f2398;
         when X"A5" => genlock_sync <= f30;
         when X"B5" => genlock_sync <= f4m_1484;
         when X"86" => genlock_sync <= f30;
         when X"96" => genlock_sync <= f4m_1484;
         when X"87" => genlock_sync <= f8g;
         when X"88" => genlock_sync <= f30;
         when X"98" => genlock_sync <= f4m_1484;
         when X"89" => genlock_sync <= f8g;
         when X"8A" => genlock_sync <= f24;
         when X"9A" => genlock_sync <= f2398;
         when X"60" => genlock_sync <= f4m_1485;
         when X"20" => genlock_sync <= f8g;
         when X"0F" => genlock_sync <= '0';
         when others => null;
      end case;
   end if;
end process;


-------------------------------------------------------------
-- Debug:
-------------------------------------------------------------
debug_o <= debug;

end Behavioral;