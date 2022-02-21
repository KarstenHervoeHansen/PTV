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
      cs_i                    : in std_logic;
      sck_i                   : in std_logic;
      mosi_i                  : in std_logic;
      mute_o                  : out std_logic;
      use_f1484_o             : out std_logic;
      use_f1485_o             : out std_logic;
      system_o                : out std_logic_vector(7 downto 0);
      pattern_o               : out std_logic_vector(15 downto 0);
      timing_o                : out std_logic_vector(23 downto 0);
      reset_channel_o         : out std_logic;
      MGT_power_down_o        : out std_logic
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
signal actual_pattern         : std_logic_vector(15 downto 0);
signal actual_pattern_var_clk : std_logic_vector(15 downto 0);
signal command_pattern        : std_logic_vector(15 downto 0);
signal command_timing         : std_logic_vector(23 downto 0);
signal actual_timing          : std_logic_vector(23 downto 0);
signal actual_timing_var_clk  : std_logic_vector(23 downto 0);
signal new_system             : std_logic;
signal MGT_power_down         : std_logic;
signal reset_channel          : std_logic;

constant black_pattern        : std_logic_vector(15 downto 0) := (others => '0');
constant off_system           : std_logic_vector(7 downto 0) := "00001111";


component serial_interface
   Port(
      mreset_i                : in std_logic;                          -- master reset
      clk_i                   : in std_logic;                          -- clock
      cs_i                    : in std_logic;                          -- chip select
      sck_i                   : in std_logic;                          -- serial clock
      mosi_i                  : in std_logic;                          -- master out serial data in
      system_o                : out std_logic_vector(7 downto 0);
      pattern_o               : out std_logic_vector(15 downto 0);
      timing_o                : out std_logic_vector(23 downto 0)
   );
end component;


begin


-------------------------------------------------------
-- Serial interface
-------------------------------------------------------
serial_interfacing : serial_interface
   Port map(
      mreset_i          => mreset_i,
      clk_i             => clk_i,
      cs_i              => cs_i,
      sck_i             => sck_i,
      mosi_i            => mosi_i,
      system_o          => command_system,
      pattern_o         => command_pattern,
      timing_o          => command_timing
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


-------------------------------------------------------
---- Generate new_system signal that indicates if
---- actual_system /= command_system:
---------------------------------------------------------
--new_system_generation : process (mreset_i, clk_i) begin
--   if mreset_i = '0' then
--      new_system <= '0';
--   elsif clk_i'event and clk_i ='1' then
--      if actual_system /= command_system then
--         new_system <= '1';
--      else
--         new_system <= '0';
--      end if;
--   end if;
--end process;


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
               if actual_system /= command_system then      --new_system = '1'
                  state_count <= conv_std_logic_vector(6,8);
                  state <= blacking;
                  actual_pattern <= black_pattern;
               else
                  actual_timing <= command_timing;
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
               if command_system /= off_system then
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
system_o    <= actual_system_var_clk;
use_f1484_o <= actual_system(4);
use_f1485_o <= not actual_system(4);

end Behavioral;