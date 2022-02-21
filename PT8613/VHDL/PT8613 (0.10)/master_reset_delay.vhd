library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity master_reset_delay is
   port (
      zreset_i                   : in std_logic;                        -- Master reset
      clk_i                      : in std_logic;                        -- sample clock
      led_i                      : in std_logic_vector(3 downto 0);
      led_o                      : out std_logic_vector(3 downto 0);
      tick_10ms_o                : out std_logic;
      reset_o                    : out std_logic_vector(3 downto 0)
   );
end master_reset_delay;


architecture Behavioral of master_reset_delay is

type statetype is (master_reset, led1, led2, led3, led4, waiting, running);
signal state                     : statetype;

signal tick_10ms_count           : std_logic_vector(20 downto 0);
signal state_count               : std_logic_vector(5 downto 0);
signal tick_10ms                 : std_logic;

signal reset                     : std_logic;
signal led_on                    : std_logic_vector(3 downto 0);
signal led_count                 : std_logic_vector(3 downto 0);


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
   

-------------------------------------------------------------
-- Generates 10ms tick pulses
-------------------------------------------------------------
tick_10ms_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if reset = '1' then
         tick_10ms_count <= conv_std_logic_vector(1485000,21);
         tick_10ms <= '0';
      else
         if tick_10ms_count = conv_std_logic_vector(1,21) then
            tick_10ms_count <= conv_std_logic_vector(1485000,21);
            tick_10ms <= '1';
         else
            tick_10ms_count <= tick_10ms_count-1;
            tick_10ms <= '0';
         end if;
      end if;
   end if;
end process;

tick_10ms_o <= tick_10ms;


--------------------------------------------------------------
-- Latch the led signal:
--------------------------------------------------------------
led_latching : process (led_i, clk_i) begin
   for i in 0 to 3 loop
   	if led_i(i) = '1' then
   		led_on(i)      <= '1';
   		led_count(i)   <= '1';
   	elsif clk_i'event and clk_i = '1' then
   	   if tick_10ms = '1' then
   	      if led_count(i) = '0' then
   	         led_on(i)      <= '0';
   	      else
   	         led_count(i)   <= '0';
   	      end if;
   	   end if;
   	end if;
   end loop;
end process;
   

--------------------------------------------------------------
-- Delays the reset signal
--------------------------------------------------------------
reset_delaying_sm : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if reset = '1' then
         state <= master_reset;
      else
         if tick_10ms = '1' then
            case state is
            when master_reset =>
               if reset = '0' then
                  state       <= led1;
                  state_count <= (others => '1');
               end if;
            when led1 =>
               if state_count = 0 then
                  state       <= led2;
               end if;
               state_count    <= state_count - 1;
            when led2 =>
               if state_count = 0 then
                  state       <= led3;
               end if;
               state_count    <= state_count - 1;
            when led3 =>
               if state_count = 0 then
                  state       <= led4;
               end if;
               state_count    <= state_count - 1;
            when led4 =>
               if state_count = 0 then
                  state       <= waiting;
               end if;
               state_count    <= state_count - 1;
            when waiting =>
               if state_count = 0 then
                  state       <= running;
               end if;
               state_count    <= state_count - 1;
            when others =>    -- running
               null;
            end case;
         end if;
      end if;
   end if;
end process;


reset_sm_mapping : process (state, led_on) begin
   if clk_i'event and clk_i = '1' then
      case state is
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

end Behavioral;