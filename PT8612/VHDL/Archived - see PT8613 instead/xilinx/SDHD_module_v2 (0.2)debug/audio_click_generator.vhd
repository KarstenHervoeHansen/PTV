library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity audio_click_generator is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      timing_i             : in std_logic_vector(11 downto 0);
      timing_modulation_i  : in std_logic;
      
      -- syncronization inputs :
      max_open_pulse_i     : in std_logic;
     
      -- audio pulse:
      audio_click_o        : out std_logic
   );
end audio_click_generator;


architecture behavioral of audio_click_generator is

signal max_open_delayed_1     : std_logic;
signal max_open               : std_logic;
signal tick_1ms_count         : std_logic_vector(17 downto 0);
signal tick_1ms               : std_logic;
signal click_timing_count     : std_logic_vector(11 downto 0);
signal timing_modul_plus      : std_logic;

constant modulation_amplitude : integer := 50;

begin


---------------------------------------------------------------
-- max_open_generation (on rising edge of max_open_pulse_i):
---------------------------------------------------------------
max_open_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      max_open_delayed_1   <= '1';
      max_open             <= '0';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         max_open_delayed_1   <= '1';
         max_open             <= '0';
      else
         max_open_delayed_1 <= max_open_pulse_i;
         if max_open_delayed_1 = '0' and max_open_pulse_i = '1' then  -- Rising
            max_open <= '1';
         else
            max_open <= '0';
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- generate 1ms tick.
---------------------------------------------------------------
tick_1ms_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      tick_1ms_count <= conv_std_logic_vector(148500-1,18);
      tick_1ms <= '0';
   elsif clk_i'event and clk_i = '1' then
      if max_open = '1' then
         tick_1ms_count <= conv_std_logic_vector(148500-1,18);
         tick_1ms <= '1';
      else
         if tick_1ms_count = conv_std_logic_vector(0,18) then
            tick_1ms_count <= conv_std_logic_vector(148500-1,18);
            tick_1ms <= '1';        
         else
            tick_1ms_count <= tick_1ms_count - 1;
            tick_1ms <= '0';        
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- generate click_timing_count
---------------------------------------------------------------
click_timing_count_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      click_timing_count   <= conv_std_logic_vector(2**11-1000,12);
      timing_modul_plus    <= '1';
   elsif clk_i'event and clk_i = '1' then
      if max_open = '1' then
         if timing_modulation_i = '0' then
            click_timing_count   <= conv_std_logic_vector(2**11-1000,12);
         elsif timing_modul_plus = '1' then
            click_timing_count   <= conv_std_logic_vector(2**11-1000-modulation_amplitude,12);
         else
            click_timing_count   <= conv_std_logic_vector(2**11-1000+modulation_amplitude,12);
         end if;
         timing_modul_plus    <= not timing_modul_plus;
      elsif tick_1ms = '1' then
         click_timing_count <= click_timing_count + 1;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- generate audio click.
---------------------------------------------------------------
audio_click_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      audio_click_o <= '0';
   elsif clk_i'event and clk_i = '1' then
      if click_timing_count = timing_i and tick_1ms = '1' then
         audio_click_o <= '1';
      elsif tick_1ms_count <= conv_std_logic_vector(30000,18) then
         audio_click_o <= '0';
      end if;
   end if;
end process;


end behavioral;