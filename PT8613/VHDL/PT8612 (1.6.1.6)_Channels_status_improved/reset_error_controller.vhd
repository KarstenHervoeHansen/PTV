library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity reset_error_controller is
   generic (
      sticky_led           : std_logic_vector(3 downto 0)         -- When high the led remains on until parameter_change is asserted
   );
   port (
      clk_i                : in std_logic;                        -- 27MHz clock
      zreset_i             : in std_logic;                        -- Master reset
      parameter_change_i   : in std_logic;                        -- connect to cs_i
      channel_error_i      : in std_logic_vector(3 downto 0);
      clock_error_i        : in std_logic_vector(7 downto 0);
      clock_warning_i      : in std_logic_vector(7 downto 0);
      channel_error_mask_i : in std_logic_vector(3 downto 0);
      clock_error_mask_i   : in std_logic_vector(7 downto 0);
      reset_o              : out std_logic_vector(3 downto 0);
      error_o              : out std_logic;
      led_o                : out std_logic_vector(3 downto 0);
      tick_10ms_o          : out std_logic
   );
end reset_error_controller;


architecture Behavioral of reset_error_controller is

type reset_statetype is (master_reset, led1, led2, led3, led4, waiting, running);
signal reset_state               : reset_statetype;
type error_statetype is (no_errors, wait_for_tick, wait1, wait2, observe1, observe2, observe3, observe4, observe5, report_error);
signal error_state               : error_statetype;

signal reset_state_count         : std_logic_vector(5 downto 0);
signal parameter_change          : std_logic;
signal reset                     : std_logic;
signal led_status                : std_logic_vector(3 downto 0);
signal led_on                    : std_logic_vector(3 downto 0);
signal led_count                 : std_logic_vector(3 downto 0);

signal tick_10ms_count           : std_logic_vector(18 downto 0);
signal tick_10ms                 : std_logic;
signal tick_2s_count             : std_logic_vector(7 downto 0);
signal tick_2s                   : std_logic;

signal masked_error              : std_logic;


component glitch_remover
   port (
      clk_i            : in std_logic;
      signal_i         : in std_logic;
      clean_signal_o   : out std_logic
   );
end component;


begin

--------------------------------------------------------------
-- Invert zreset_i and remove glitches :
--------------------------------------------------------------
glitch_removing_reset : glitch_remover
   port map(
      clk_i             => clk_i,
      signal_i          => not zreset_i,
      clean_signal_o    => reset
   );

glitch_removing_parameter_change : glitch_remover
   port map(
      clk_i             => clk_i,
      signal_i          => parameter_change_i,
      clean_signal_o    => parameter_change
   );


-------------------------------------------------------------
-- Generates timing ticks :
-------------------------------------------------------------
tick_10ms_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if tick_10ms_count = 0 then
         tick_10ms_count <= conv_std_logic_vector(270000-1,19);
         tick_10ms <= '1';
      else
         tick_10ms_count <= tick_10ms_count-1;
         tick_10ms <= '0';
      end if;
   end if;
end process;

tick_2s_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if tick_10ms = '1' then
         if tick_2s_count = 0 then
            tick_2s_count <= conv_std_logic_vector(199,8);
            tick_2s <= '1';
         else
            tick_2s_count <= tick_2s_count-1;
            tick_2s <= '0';
         end if;
      else
         tick_2s <= '0';
      end if;
   end if;
end process;

tick_10ms_o <= tick_10ms;


--------------------------------------------------------------
-- Delays the reset signal
--------------------------------------------------------------
reset_delaying_sm : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if reset = '1' then
         reset_state <= master_reset;
      else
         if tick_10ms = '1' then
            case reset_state is
            when master_reset =>
               if reset = '0' then
                  reset_state       <= led1;
                  reset_state_count <= (others => '1');
               end if;
            when led1 =>
               if reset_state_count = 0 then
                  reset_state       <= led2;
               end if;
               reset_state_count    <= reset_state_count - 1;
            when led2 =>
               if reset_state_count = 0 then
                  reset_state       <= led3;
               end if;
               reset_state_count    <= reset_state_count - 1;
            when led3 =>
               if reset_state_count = 0 then
                  reset_state       <= led4;
               end if;
               reset_state_count    <= reset_state_count - 1;
            when led4 =>
               if reset_state_count = 0 then
                  reset_state       <= waiting;
               end if;
               reset_state_count    <= reset_state_count - 1;
            when waiting =>
               if reset_state_count = 0 then
                  reset_state       <= running;
               end if;
               reset_state_count    <= reset_state_count - 1;
            when others =>    -- running
               null;
            end case;
         end if;
      end if;
   end if;
end process;


reset_sm_mapping : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      case reset_state is
      when master_reset =>
         led_o    <= "1111";
         reset_o(3 downto 1)  <= "111";
      when led1 =>
         led_o    <= "0001";
         reset_o(3 downto 1)  <= "110";
      when led2 =>
         led_o    <= "0010";
         reset_o(3 downto 1)  <= "110";
      when led3 =>
         led_o    <= "0100";
         reset_o(3 downto 1)  <= "100";
      when led4 =>
         led_o    <= "1000";
         reset_o(3 downto 1)  <= "100";
      when waiting =>
         led_o    <= "0000";
         reset_o(3 downto 1)  <= "100";
      when others =>    -- running
         led_o    <= led_on;
         reset_o(3 downto 1)  <= "000";
      end case;
   end if;
end process;

reset_o(0) <= reset;


--------------------------------------------------------------
-- Error masking :
--------------------------------------------------------------
masked_error <=   (clock_error_i(7)    and clock_error_mask_i(7))   or
                  (clock_error_i(6)    and clock_error_mask_i(6))   or
                  (clock_error_i(5)    and clock_error_mask_i(5))   or
                  (clock_error_i(4)    and clock_error_mask_i(4))   or
                  (clock_error_i(3)    and clock_error_mask_i(3))   or
                  (clock_error_i(2)    and clock_error_mask_i(2))   or
                  (clock_error_i(1)    and clock_error_mask_i(1))   or
                  (clock_error_i(0)    and clock_error_mask_i(0))   or
                  (channel_error_i(3)  and channel_error_mask_i(3)) or
                  (channel_error_i(2)  and channel_error_mask_i(2)) or
                  (channel_error_i(1)  and channel_error_mask_i(1)) or
                  (channel_error_i(0)  and channel_error_mask_i(0));


--------------------------------------------------------------
-- error sm :
--------------------------------------------------------------
error_sm : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if parameter_change = '1' then
         error_state    <= no_errors;
         error_o  <= '0';
      else
         case error_state is
         when no_errors =>
            if masked_error = '1' then
               error_state <= wait_for_tick;
            end if;
            error_o  <= '0';
         when wait_for_tick =>
            if tick_2s = '1' then
               error_state <= wait1;
            end if;
            error_o  <= '0';
         when wait1 =>
            if tick_2s = '1' then
               error_state <= wait2;
            end if;
            error_o  <= '0';
         when wait2 =>
            if tick_2s = '1' then
               error_state <= observe1;
            end if;
            error_o  <= '0';
         when observe1 =>
            if masked_error = '1' then
               error_state <= report_error;
            elsif tick_2s = '1' then
               error_state <= observe2;
            end if;
         when observe2 =>
            if masked_error = '1' then
               error_state <= report_error;
            elsif tick_2s = '1' then
               error_state <= observe3;
            end if;
         when observe3 =>
            if masked_error = '1' then
               error_state <= report_error;
            elsif tick_2s = '1' then
               error_state <= observe4;
            end if;
            error_o  <= '0';
         when observe4 =>
            if masked_error = '1' then
               error_state <= report_error;
            elsif tick_2s = '1' then
               error_state <= observe5;
            end if;
            error_o  <= '0';
         when observe5 =>
            if masked_error = '1' then
               error_state <= report_error;
            elsif tick_2s = '1' then
               error_state <= no_errors;
            end if;
            error_o  <= '0';
         when others => -- report_error
            error_state    <= observe1;
            error_o  <= '1';
         end case;
      end if;
   end if;
end process;


--------------------------------------------------------------
-- Status on LED's:
--------------------------------------------------------------
-- Error indication:
led_status(0) <=  clock_error_i(7)     or -- f4m 27 to 148 period
                  clock_error_i(6)     or -- f8g 27 to 148 period
                  clock_error_i(5)     or -- f4m period
                  clock_error_i(4)     or -- f4m frame count
                  clock_error_i(3)     or -- f8g period
                  clock_error_i(2)     or -- f8g frame count
                  clock_error_i(1)     or -- pll 1484 warning
                  clock_error_i(0)     or -- pll 1485 warning
                  channel_error_i(3)   or -- channel 4
                  channel_error_i(2)   or -- channel 3
                  channel_error_i(1)   or -- channel 2
                  channel_error_i(0);     -- channel 1

-- Warning indication:
led_status(1) <=  clock_warning_i(7)   or -- f4m 27 to 148 period
                  clock_warning_i(6)   or -- f8g 27 to 148 period
                  clock_warning_i(5)   or -- f4m period
                  clock_warning_i(4)   or -- f4m frame count
                  clock_warning_i(3)   or -- f8g period
                  clock_warning_i(2)   or -- f8g frame count
                  clock_warning_i(1)   or -- pll 1484 warning
                  clock_warning_i(0);     -- pll 1485 warning

-- PLL warning indication:
led_status(2) <=  clock_warning_i(1)   or -- pll 1484 warning
                  clock_warning_i(0);     -- pll 1485 warning

-- Genlock warning indication:
led_status(3) <=  clock_warning_i(7)   or -- f4m 27 to 148 period
                  clock_warning_i(6)   or -- f8g 27 to 148 period
                  clock_warning_i(5)   or -- f4m period
                  clock_warning_i(4)   or -- f4m frame count
                  clock_warning_i(3)   or -- f8g period
                  clock_warning_i(2);     -- f8g frame count


--------------------------------------------------------------
-- Latch the led signal:
--------------------------------------------------------------
led_latching : process (clk_i) begin
   for i in 0 to 3 loop
   	if clk_i'event and clk_i = '1' then
   	   if led_status(i) = '1' then
   		   led_on(i)      <= '1';
   		   led_count(i)   <= '1';
   	   elsif sticky_led(i) = '1' then
   	      if parameter_change = '1' then
      	      led_on(i)   <= '0';
   	      end if;
   	   elsif tick_10ms = '1' then
   	      if led_count(i) = '0' then
   	         led_on(i)      <= '0';
   	      else
   	         led_count(i)   <= '0';
   	      end if;
   	   end if;
   	end if;
   end loop;
end process;


end Behavioral;