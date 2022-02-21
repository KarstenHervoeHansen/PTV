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
      spl_div_o               : out std_logic_vector(9 downto 0);
      lpf_o                   : out std_logic_vector(10 downto 0);
      div10_11_o              : out std_logic;
      framerate_x2_o          : out std_logic;
      sync_mode_o             : out std_logic_vector(1 downto 0);
      fr_ref_sel_o            : out std_logic_vector(1 downto 0);
      scan_method_o           : out std_logic;
      timing_o                : out std_logic_vector(23 downto 0);
      reset_channel_o         : out std_logic;
      spl_is_zero_o           : out std_logic
      --debug:
--      debug_o                 : out std_logic_vector(15 downto 0) := (others => '0')
   );
end channel_controller;


architecture Behavioral of channel_controller is


type statetype is (muted, unmuting, running, muting);
signal state                     : statetype;
signal state_count               : std_logic_vector(7 downto 0);
signal allowed_count             : std_logic_vector(3 downto 0);
signal write_allowed             : std_logic;
signal read_allowed              : std_logic;
signal read_allowed_var_clk      : std_logic;
signal param_valid               : std_logic;
signal mute                      : std_logic;
signal mute_var_clk              : std_logic;
signal actual_system             : std_logic_vector(14 downto 0);
signal actual_system_var_clk     : std_logic_vector(14 downto 0);
signal command_system            : std_logic_vector(14 downto 0);
signal command_timing            : std_logic_vector(23 downto 0);
--signal command_valid             : std_logic;

signal actual_timing             : std_logic_vector(23 downto 0);
signal actual_timing_var_clk     : std_logic_vector(23 downto 0);
signal new_system                : std_logic;
signal reset_channel             : std_logic;

--debug:
--signal debug                     : std_logic_vector(15 downto 0) := (others => '0');


component serial_interface
   Port(
      mreset_i                : in std_logic;                          -- master reset
      clk_i                   : in std_logic;                          -- clock
      cs_i                    : in std_logic;                          -- chip select
      sck_i                   : in std_logic;                          -- serial clock
      mosi_i                  : in std_logic;                          -- master out serial data in
      system_o                : out std_logic_vector(14 downto 0);
      timing_o                : out std_logic_vector(23 downto 0)
      --debug:
--      debug_o                 : out std_logic_vector(15 downto 0) := (others => '0')
   );
end component;


begin

-------------------------------------------------------
-- Serial interface. Output parameters are only valid
-- if param_valid_o = '1'
-------------------------------------------------------
serial_interfacing : serial_interface
   Port map(
      mreset_i          => mreset_i,
      clk_i             => clk_i,
      cs_i              => cs_i,
      sck_i             => sck_i,
      mosi_i            => mosi_i,
      system_o          => command_system,
      timing_o          => command_timing
      --debug:
--      debug_o           => debug
   );

--command_system_vector   <= spl_div & lpf & div10_11 & framerate_x2 & sync_mode &
--                              fr_ref_sel & scan_method & use_f1484;


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
-- Generate new_system signal that indicates if
-- actual_system /= command_system:
-------------------------------------------------------
new_system_generation : process (mreset_i, clk_i) begin
   if mreset_i = '0' then
      new_system <= '0';
   elsif clk_i'event and clk_i ='1' then
      if actual_system /= command_system then      --command_valid = '1' and
         new_system <= '1';
      else   
         new_system <= '0';
      end if;
   end if;
end process;


-------------------------------------------------------
-- state machine for controlling of mute, reset, and
-- actual system and timing parameters
-------------------------------------------------------
controller_sm : process (reset_delayed_i, clk_i) begin
   if reset_delayed_i = '0' then
      state <= muted;
      state_count <= conv_std_logic_vector(6,8);         --6
      reset_channel <= '0';
      mute <= '1';
      actual_timing <= command_timing;
      actual_system <= command_system;
      spl_is_zero_o <= '0';
   elsif clk_i'event and clk_i ='1' then
      case state is
         when running =>
            if new_system = '1' then
               state_count <= conv_std_logic_vector(6,8);
               state <= muting;
               mute <= '1';
            elsif write_allowed = '1' then      -- and command_valid = '1'
               actual_timing <= command_timing;
               mute <= '0';
            end if;
         when muting =>
            if tick_10ms_i = '1' then
               if state_count = conv_std_logic_vector(0,8) then
                  state <= muted;
                  reset_channel <= '0';
               else
                  state_count <= state_count - 1;
               end if;
            end if;
         when muted =>
            if write_allowed = '1' then         --command_valid = '1' and
               if command_system(11 downto 2) /= conv_std_logic_vector(0,10) then   --spl_div /= 000...
                  actual_timing <= command_timing;
                  actual_system <= command_system;
                  state <= unmuting;
                  state_count <= conv_std_logic_vector(1,8);
                  spl_is_zero_o <= '0';
               else
                  spl_is_zero_o <= '1';
               end if;
            end if;
         when unmuting =>
            if tick_10ms_i = '1' then
               if state_count = conv_std_logic_vector(0,8) then
                  state <= running;
                  reset_channel <= '1';
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
      actual_system_var_clk <= actual_system;
      mute_var_clk <= mute;
   elsif var_clk_i'event and var_clk_i ='1' then
      reset_channel_o <= reset_channel;
      read_allowed_var_clk <= read_allowed;
      actual_timing_var_clk <= actual_timing;
      actual_system_var_clk <= actual_system;
      mute_var_clk <= mute;
   end if;
end process;


------------------------------------------------------------
-- Update timing and mute when read_allowed_var_clk = '1':
------------------------------------------------------------
update_timing_mute: process (reset_delayed_i, var_clk_i) begin
   if reset_delayed_i = '0' then
      timing_o <= actual_timing_var_clk;
      mute_o <= '1';
   elsif var_clk_i'event and var_clk_i ='1' then
      if read_allowed_var_clk = '1' then
         timing_o <= actual_timing_var_clk;
         mute_o <= mute_var_clk;
      end if;
   end if;
end process;


------------------------------------------------------------
-- Mapping of output parameters:
------------------------------------------------------------
use_f1484_o          <= actual_system_var_clk(14);
use_f1485_o          <= not actual_system_var_clk(14);
spl_div_o            <= actual_system_var_clk(11 downto 2);
div10_11_o           <= actual_system_var_clk(0);
sync_mode_o          <= actual_system_var_clk(13 downto 12);
fr_ref_sel_o         <= actual_system_var_clk(7) & actual_system_var_clk(14);
scan_method_o        <= actual_system_var_clk(1);


lpf_generation: process (mreset_i, var_clk_i) begin
   if mreset_i = '0' then
      lpf_o <= conv_std_logic_vector(0,11);
   elsif var_clk_i'event and var_clk_i ='1' then
      if actual_system_var_clk(0) = '1' then
         lpf_o <= conv_std_logic_vector(1125,11);
      else
         lpf_o <= conv_std_logic_vector(750,11);
      end if;
   end if;
end process;


framerate_x2_generation: process (mreset_i, var_clk_i) begin
   if mreset_i = '0' then
      framerate_x2_o <= '0';
   elsif var_clk_i'event and var_clk_i ='1' then
      if actual_system_var_clk(11 downto 2) = "0011000111" or actual_system_var_clk(11 downto 2) = "0011101111" then
         framerate_x2_o <= '1';
      else
         framerate_x2_o <= '0';
      end if;
   end if;
end process;


-------------------------------------------------------
-- Debug:
-------------------------------------------------------

--debug_state_monitor : process (mreset_i, clk_i) begin
--   if mreset_i = '0' then
--      debug_o(0) <= '0';
--      debug_o(1) <= '0';
--      debug_o(2) <= '0';
--   elsif clk_i'event and clk_i ='1' then
--      case state is
--         when running =>
--            debug_o(0) <= '0';
--            debug_o(1) <= '0';
--            debug_o(2) <= '0';
--         when muting =>
--            debug_o(0) <= '1';
--            debug_o(1) <= '0';
--            debug_o(2) <= '0';
--         when muted =>
--            debug_o(0) <= '0';
--            debug_o(1) <= '1';
--            debug_o(2) <= '0';
--         when unmuting =>
--            debug_o(0) <= '1';
--            debug_o(1) <= '1';
--            debug_o(2) <= '0';
--         when others =>
--            debug_o(0) <= '0';
--            debug_o(1) <= '0';
--            debug_o(2) <= '1';
--      end case;
--   end if;
--end process;

--debug_o(15 downto 4) <= command_system(14 downto 3);
--debug_o(3 downto 0) <= debug(3 downto 0);


end Behavioral;