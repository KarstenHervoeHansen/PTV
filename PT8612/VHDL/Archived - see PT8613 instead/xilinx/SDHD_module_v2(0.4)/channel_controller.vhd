library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;


entity channel_controller is
   port (
      mreset_i                : in std_logic;
      reset_delayed_i         : in std_logic;
      clk_i                   : in std_logic;
      tick_10ms_i             : in std_logic;
      var_clk_i               : in std_logic;
      --// Signal to/fro RAM in serial SPI interface
      ch_ram_data_i           : in  std_logic_vector(7 downto 0);  --// data to read bus
      ch_ram_ready_i          : in  std_logic;                     --// data ready signal
      ch_ram_addr_o           : out std_logic_vector(4 downto 0);  --// address buss

      mute_o                  : out std_logic;
      use_f1484_o             : out std_logic;
      use_f1485_o             : out std_logic;
      system_o                : out std_logic_vector(7 downto 0);
      pattern_o               : out std_logic_vector(15 downto 0);
      timing_o                : out std_logic_vector(23 downto 0);
      click_timing_o          : out std_logic_vector(11 downto 0);
      audio_level_index_o     : out std_logic_vector(7 downto 0);
      audio_signal_index_o    : out std_logic_vector(7 downto 0);
      -- text:
      bmp_v_position_o        : out std_logic_vector(15 downto 0);
      bmp_h_position_o        : out std_logic_vector(15 downto 0);
      bmp_h_size_o            : out std_logic_vector(7 downto 0);
      bmp_v_size_o            : out std_logic_vector(7 downto 0);
      bmp_scale_o             : out std_logic_vector(7 downto 0);
      bmp_back_text_color_o   : out std_logic_vector(7 downto 0);
      bmp_movement_o          : out std_logic_vector(7 downto 0);
      bmp_on_zoff_o           : out std_logic;

      reset_channel_o         : out std_logic;
      MGT_power_down_o        : out std_logic;
      debugger_o              : out std_logic_vector(9 downto 0)
   );
end channel_controller;


architecture Behavioral of channel_controller is


type statetype is (reset, unreseting, powering, unmuting, unblacking, running, blacking, muting, downpowering);
signal state                  : statetype;
signal state_count            : std_logic_vector(7 downto 0);
signal allowed_count          : std_logic_vector(3 downto 0);
signal write_allowed          : std_logic;
signal read_allowed           : std_logic;
signal read_allowed_var_clk   : std_logic;
signal param_valid            : std_logic;
signal mute                   : std_logic;
signal mute_var_clk           : std_logic;
signal actual_system          : std_logic_vector(7 downto 0);
signal actual_system_var_clk  : std_logic_vector(7 downto 0);

signal command_system         : std_logic_vector(7 downto 0);
signal command_pattern        : std_logic_vector(15 downto 0);
signal command_timing         : std_logic_vector(23 downto 0);
signal command_aclick         : std_logic_vector(15 downto 0);
signal command_edh_0          : std_logic_vector( 7 downto 0);
signal command_agroup         : std_logic_vector( 7 downto 0);
signal command_asr            : std_logic_vector( 7 downto 0);
signal command_pos            : std_logic_vector(15 downto 0);
signal command_sca            : std_logic_vector( 7 downto 0);
signal command_col            : std_logic_vector( 7 downto 0);
signal command_siz            : std_logic_vector( 15 downto 0);
signal command_mov            : std_logic_vector( 7 downto 0);
signal command_xmin           : std_logic_vector( 7 downto 0);
signal command_xmax           : std_logic_vector( 7 downto 0);
signal command_ymin           : std_logic_vector( 7 downto 0);
signal command_ymax           : std_logic_vector( 7 downto 0);
signal command_tms            : std_logic_vector( 7 downto 0);

signal ram_ready              : std_logic; --// JK (for debugging)

signal actual_pattern         : std_logic_vector(15 downto 0);
signal actual_pattern_var_clk : std_logic_vector(15 downto 0);
signal actual_timing          : std_logic_vector(23 downto 0);
signal actual_timing_var_clk  : std_logic_vector(23 downto 0);
signal new_system             : std_logic;
signal command_system_is_off  : std_logic;
signal MGT_power_down         : std_logic;
signal reset_channel          : std_logic;

constant black_pattern        : std_logic_vector(15 downto 0) := X"0900";
constant off_system           : std_logic_vector(7 downto 0) := "00001111";

attribute keep : string;
attribute keep of system_o: signal is "true";

component settings_buffer
   Port (
      sb_mreset_i             : in  std_logic;   --// master reset
      --// interface to Channel RAM via Serial Interface:
      rd_add_o                : out std_logic_vector( 4 downto 0); --// address bus
      rd_dat_i                : in  std_logic_vector( 7 downto 0); --// data to read bus
      ready_i                 : in  std_logic;                     --// data ready signal
      --// output section:
      sb_clk_i                : in  std_logic;
      sys_o                   : out std_logic_vector( 7 downto 0);
      pat_o                   : out std_logic_vector(15 downto 0);
      del_o                   : out std_logic_vector(23 downto 0);
      aclick_o                : out std_logic_vector(15 downto 0);
      asignal_o               : out std_logic_vector( 7 downto 0);
      alevel_o                : out std_logic_vector( 7 downto 0);
      agroup_o                : out std_logic_vector( 7 downto 0);
      asr_o                   : out std_logic_vector( 7 downto 0);
      bmp_v_position_o        : out std_logic_vector(15 downto 0);
      bmp_h_position_o        : out std_logic_vector(15 downto 0);
      bmp_h_size_o            : out std_logic_vector(7 downto 0);
      bmp_v_size_o            : out std_logic_vector(7 downto 0);
      bmp_scale_o             : out std_logic_vector(7 downto 0);
      bmp_back_text_color_o   : out std_logic_vector(7 downto 0);
      bmp_movement_o          : out std_logic_vector(7 downto 0);
      bmp_on_zoff_o           : out std_logic
   );
end component;


begin


--// Signals to be output via debugger vector into leds
ram_ready     <= ch_ram_ready_i;
debugger_o(0) <= command_system(0);    --// Led2
debugger_o(3) <= ram_ready;            --// Led5

-------------------------------------------------------
-- Settings buffering:
-------------------------------------------------------
settings_buffering : settings_buffer
   Port map
   (
      sb_mreset_i             => mreset_i,
      rd_add_o                => ch_ram_addr_o,
      rd_dat_i                => ch_ram_data_i,
      ready_i                 => ram_ready, --//ch_ram_ready_i,
      sb_clk_i                => clk_i,
      sys_o                   => command_system,
      pat_o                   => command_pattern,
      del_o                   => command_timing,
      aclick_o                => command_aclick,
      asignal_o               => audio_signal_index_o,
      alevel_o                => audio_level_index_o,
      agroup_o                => command_agroup,
      asr_o                   => command_asr,
      bmp_v_position_o        => bmp_v_position_o,     
      bmp_h_position_o        => bmp_h_position_o,     
      bmp_h_size_o            => bmp_h_size_o,         
      bmp_v_size_o            => bmp_v_size_o,         
      bmp_scale_o             => bmp_scale_o,          
      bmp_back_text_color_o   => bmp_back_text_color_o,
      bmp_movement_o          => bmp_movement_o,       
      bmp_on_zoff_o           => bmp_on_zoff_o
   );


-------------------------------------------------------
-- Generate seperated read and write allowed pulses of
-- width 2. They are used to exchange the data across
-- the two clock domains clk_i and var_clk_i.
-------------------------------------------------------
allowed_generation : process (reset_delayed_i, clk_i) begin
   if reset_delayed_i = '0' then
      write_allowed <= '0';
      read_allowed <= '0';
      allowed_count <= conv_std_logic_vector(0,4);
   elsif clk_i'event and clk_i ='1' then
      allowed_count <= allowed_count + 1;
      if allowed_count(3 downto 1) = "000" then
         write_allowed <= '1';
      else
         write_allowed <= '0';
      end if;
      if allowed_count(3 downto 1) = "100" then
         read_allowed <= '1';
      else
         read_allowed <= '0';
      end if;
   end if;
end process;


-----------------------------------------------------
-- Generate new_system signal that indicates if
-- actual_system /= command_system:
-------------------------------------------------------
new_system_generation : process (mreset_i, clk_i) begin
   if mreset_i = '0' then
      new_system <= '0';
   elsif clk_i'event and clk_i ='1' then
      if actual_system /= command_system then
         new_system <= '1';
      else
         new_system <= '0';
      end if;
   end if;
end process;


-----------------------------------------------------
-- Generate command_system_is_off signal that indicates if
-- command_system = off_system :
-------------------------------------------------------
command_system_is_off_generation : process (mreset_i, clk_i) begin
   if mreset_i = '0' then
      command_system_is_off <= '1';
   elsif clk_i'event and clk_i ='1' then
      if command_system = off_system then
         command_system_is_off <= '1';
      else
         command_system_is_off <= '0';
      end if;
   end if;
end process;


-------------------------------------------------------
-- state machine for controlling of mute, reset, and
-- actual system, pattern and timing parameters
-------------------------------------------------------
controller_sm : process (reset_delayed_i, clk_i) begin
   if reset_delayed_i = '0' then
      state <= reset;
      state_count <= conv_std_logic_vector(6,8);
      reset_channel <= '0';
      MGT_power_down <= '1';
      mute <= '1';
      actual_timing <= command_timing;
      actual_pattern <= black_pattern;
      actual_system <= command_system;
   elsif clk_i'event and clk_i ='1' then
      case state is
         when running =>
            if write_allowed = '1' then
               if new_system = '1' then         --actual_system /= command_system
                  state_count <= conv_std_logic_vector(6,8);
                  state <= blacking;
                  actual_pattern <= black_pattern;
               else
                  actual_timing  <= command_timing;
                  actual_pattern <= command_pattern;
               end if;
            end if;
         when blacking =>
            if tick_10ms_i = '1' then
               if state_count = conv_std_logic_vector(0,8) then
                  state <= muting;
                  state_count <= conv_std_logic_vector(6,8);
                  mute <= '1';
               else
                  state_count <= state_count - 1;
               end if;
            end if;
         when muting =>
            if tick_10ms_i = '1' then
               if state_count = conv_std_logic_vector(0,8) then
                  state <= downpowering;
                  state_count <= conv_std_logic_vector(1,8);
                  MGT_power_down <= '1';
               else
                  state_count <= state_count - 1;
               end if;
            end if;
         when downpowering =>
            if tick_10ms_i = '1' then
               if state_count = conv_std_logic_vector(0,8) then
                  state <= reset;
                  reset_channel <= '0';
               else
                  state_count <= state_count - 1;
               end if;
            end if;
         when reset =>
            if write_allowed = '1' then
               if command_system_is_off = '0' then    --command_system /= off_system
                  actual_timing <= command_timing;
                  actual_pattern <= black_pattern;
                  actual_system <= command_system;
                  state <= unreseting;
                  reset_channel <= '1';
                  state_count <= conv_std_logic_vector(1,8);
               end if;
            end if;
         when unreseting =>
            if tick_10ms_i = '1' then
               if state_count = conv_std_logic_vector(0,8) then
                  state <= powering;
                  state_count <= conv_std_logic_vector(12,8);
                  MGT_power_down <= '0';
               else
                  state_count <= state_count - 1;
               end if;
            end if;
         when powering =>
            if tick_10ms_i = '1' then
               if state_count = conv_std_logic_vector(0,8) then
                  state <= unmuting;
                  state_count <= conv_std_logic_vector(12,8);
                  mute <= '0';
               else
                  state_count <= state_count - 1;
               end if;
            end if;
         when unmuting =>
            if tick_10ms_i = '1' then
               if state_count = conv_std_logic_vector(0,8) then
                  state <= running;
               else
                  state_count <= state_count - 1;
               end if;
            end if;
         when others =>
            null;
      end case;
   end if;
end process;


------------------------------------------------------------
-- Signal reclocking to var_clk_i clock domain:
------------------------------------------------------------
var_clk_reclocking: process (reset_delayed_i, var_clk_i) begin
   if reset_delayed_i = '0' then
      reset_channel_o <= '0';
      read_allowed_var_clk <= '0';
      actual_timing_var_clk <= actual_timing;
      actual_pattern_var_clk <= actual_pattern;
      actual_system_var_clk <= actual_system;
      mute_var_clk <= mute;
   elsif var_clk_i'event and var_clk_i ='1' then
      MGT_power_down_o <= MGT_power_down;
      reset_channel_o <= reset_channel;
      read_allowed_var_clk <= read_allowed;
      actual_timing_var_clk <= actual_timing;
      actual_pattern_var_clk <= actual_pattern;
      actual_system_var_clk <= actual_system;
      mute_var_clk <= mute;
   end if;
end process;


------------------------------------------------------------
-- Update timing, pattern and mute when read_allowed_var_clk = '1':
------------------------------------------------------------
update_timing_mute: process (reset_delayed_i, var_clk_i) begin
   if reset_delayed_i = '0' then
      timing_o <= actual_timing_var_clk;
      pattern_o <= actual_pattern_var_clk;
      mute_o <= '1';
   elsif var_clk_i'event and var_clk_i ='1' then
      if read_allowed_var_clk = '1' then
         timing_o <= actual_timing_var_clk;
         pattern_o <= actual_pattern_var_clk;
         mute_o <= mute_var_clk;
      end if;
   end if;
end process;


------------------------------------------------------------
-- Mapping of system parameters:
------------------------------------------------------------

click_timing_o <= command_aclick(11 downto 0);

system_o    <= actual_system_var_clk;
use_f1484_o <= actual_system(4);
use_f1485_o <= not actual_system(4);

end Behavioral;