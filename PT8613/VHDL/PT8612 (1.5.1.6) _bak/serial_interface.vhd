-------------------------------------------------------
-- File:     serial_interface.vhd
-- Project:  PT8612 HD-SDI Test Pattern Generator
-- Date:     16.08.2006
-- Author:   THH
-------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity serial_interface is
   generic (
      channel_id              : integer
   );
   Port (
      clk_i                   : in  std_logic;        -- clock
      reset_i                 : in  std_logic := '0'; -- sync_reset
      cs_i                    : in  std_logic;        -- chip select
      sck_i                   : in  std_logic;        -- serial clock
      mosi_i                  : in  std_logic;        -- master out serial data in
      miso_o                  : out std_logic;        -- master in serial data out (high when inactive)
      status_channel_i        : in std_logic_vector(7 downto 0) := (others => '1');
      warning_clock_i         : in std_logic_vector(7 downto 0) := (others => '1');
      error_clock_i           : in std_logic_vector(7 downto 0) := (others => '1');
      frame_count_25_o        : out std_logic_vector(7 downto 0);
      new_frame_count_25_o    : out std_logic;
      set_frame_count_25_o    : out std_logic;
      frame_count_25_i        : in std_logic_vector(7 downto 0) := (others => '1');
      frame_count_30_o        : out std_logic_vector(7 downto 0);
      new_frame_count_30_o    : out std_logic;
      set_frame_count_30_o    : out std_logic;
      frame_count_30_i        : in std_logic_vector(7 downto 0) := (others => '1');
      system_o                : out std_logic_vector(11 downto 0);
      pattern_o               : out std_logic_vector(15 downto 0);
      timing_o                : out std_logic_vector(23 downto 0);
      click_timing_o          : out std_logic_vector(11 downto 0);
      audio_level_index_o     : out std_logic_vector(7 downto 0);
      audio_signal_index_o    : out std_logic_vector(7 downto 0);
      bmp_v_position_o        : out std_logic_vector(15 downto 0);
      bmp_h_position_o        : out std_logic_vector(15 downto 0);
      bmp_h_size_o            : out std_logic_vector(7 downto 0);
      bmp_v_size_o            : out std_logic_vector(7 downto 0);
      bmp_scale_o             : out std_logic_vector(7 downto 0);
      bmp_back_text_color_o   : out std_logic_vector(7 downto 0);
      bmp_movement_o          : out std_logic_vector(7 downto 0);
      bmp_on_zoff_o           : out std_logic;
      channel_error_mask_o    : out std_logic_vector(7 downto 0) := (others => '0');  -- Ignore errors
      clock_error_mask_o      : out std_logic_vector(7 downto 0) := (others => '0');  -- Ignore errors
      text_ram_clk_i          : in  std_logic                    := '0';              -- clock for reading text ram
      text_ram_addr_i         : in  std_logic_vector(8 downto 0) := (others => '0');
      text_ram_en_i           : in  std_logic                    := '0';              -- enable signal for reading text ram
      text_ram_data_o         : out std_logic_vector( 7 downto 0)
   );
end serial_interface;

-- Example of transmitting and recieving one byte:
--         -----                                                                              -----
--    cs:       |                                                                            |
--               ----------------------------------------------------------------------------
--                      ---     ---     ---     ---     ---     ---     ---     ---
--   sck:              |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
--         ------------     ---     ---     ---     ---     ---     ---     ---     ---------------
--                       ------- ------- ------- ------- ------- ------- ------- -------
--  mosi:               X  msb  X  b6   X  b5   X  b4   X  b3   X  b2   X  b1   X  lsb  X
--         ------------- ------- ------- ------- ------- ------- ------- ------- ------- ----------
--                       ------- ------- ------- ------- ------- ------- ------- -------
--  miso:               X  msb  X  b6   X  b5   X  b4   X  b3   X  b2   X  b1   X  lsb  X
--         ------------- ------- ------- ------- ------- ------- ------- ------- ------- ----------


architecture Behavioral of serial_interface is

signal cs                           : std_logic;
signal cs_delayed                   : std_logic;
signal sck                          : std_logic;
signal sck_delayed                  : std_logic;
signal mosi                         : std_logic;
signal sck_rising_tick              : std_logic;
signal sck_falling_tick             : std_logic;
signal sck_falling_tick_advanced    : std_logic;
signal byte_done                    : std_logic;
signal byte_begin                   : std_logic;
signal mosi_del_line                : std_logic_vector(23 downto 0);
signal miso_register                : std_logic_vector(7 downto 0);

signal bit_count                    : std_logic_vector(2 downto 0);
signal adr_count                    : std_logic_vector(15 downto 0);
signal text_ram_addr                : std_logic_vector(8 downto 0);
signal channel_sel                  : std_logic_vector(3 downto 0);  --// select channel RAM
signal channel_selected             : std_logic;
signal channel_id_bits              : std_logic_vector(3 downto 0);
signal text_ram_write               : std_logic;
signal click_timing                 : std_logic_vector(11 downto 0);

signal status_channel_sticky        : std_logic_vector(7 downto 0);
signal warning_clock_sticky         : std_logic_vector(7 downto 0);
signal error_clock_sticky           : std_logic_vector(7 downto 0);
signal load_with_channel            : std_logic;
signal load_with_warning_clock      : std_logic;
signal load_with_error_clock        : std_logic;
signal load_with_non_sticky         : std_logic;
signal set_frame_count              : std_logic_vector(1 downto 0);

type state_type is (header, data);
signal state                        : state_type;


component text_ram
   port
      (
      --// input (SPI) part (write only)
      clka:  IN  std_logic;
      addra: IN  std_logic_VECTOR( 8 downto 0);
      dina:  IN  std_logic_VECTOR( 7 downto 0);
      wea:   IN  std_logic;                      --// write enable for part A (active hi)
      ena:   IN  std_logic;

      --// output (FPGA) (read only)
      clkb:  IN  std_logic;
      addrb: IN  std_logic_VECTOR( 8 downto 0);
      doutb: OUT std_logic_VECTOR( 7 downto 0);
      enb:   IN  std_logic
      );
end component;

component glitch_remover
   port (
      clk_i            : in std_logic;
      signal_i         : in std_logic;
      clean_signal_o   : out std_logic
   );
end component;


begin


-------------------------------------------------------
-- Remove glitches on input signals:
-------------------------------------------------------
glitch_removing_sck : glitch_remover
   port map(
      clk_i             => clk_i,
      signal_i          => sck_i,
      clean_signal_o    => sck
   );

glitch_removing_mosi : glitch_remover
   port map(
      clk_i             => clk_i,
      signal_i          => mosi_i,
      clean_signal_o    => mosi
   );

glitch_removing_cs : glitch_remover
   port map(
      clk_i             => clk_i,
      signal_i          => cs_i,
      clean_signal_o    => cs
   );

text_ram_instance : text_ram
   port map (
      clka   => clk_i,
      addra  => text_ram_addr,
      dina   => mosi_del_line(8 downto 1),   -- due to timing of text_ram_write use (8 downto 1) instead of (7 downto 0)
      ena    => channel_selected,
      wea    => text_ram_write,
      clkb   => text_ram_clk_i,
      addrb  => text_ram_addr_i,
      doutb  => text_ram_data_o,
      enb    => text_ram_en_i
   );

indicate_when_channel_is_selected : process(channel_sel, channel_id_bits) begin
   if channel_sel = channel_id_bits then
      channel_selected <= '1';
   else
      channel_selected <= '0';
   end if;
end process;

--------------------------------------------------------------
-- Make tick at spi_sck rising and falling edge:
--------------------------------------------------------------
sck_rising_falling_ticking: process(clk_i) begin
   if clk_i'event and clk_i ='1' then
      sck_delayed <= sck;
      if cs = '1' then
         sck_rising_tick   <= '0';
         sck_falling_tick  <= '0';
      else
         if sck = '1' and sck_delayed = '0' then   -- rising edge :
            sck_rising_tick   <= '1';
         else
            sck_rising_tick   <= '0';
         end if;
         if sck = '0' and sck_delayed = '1' then   -- falling edge :
            sck_falling_tick  <= '1';
         else
            sck_falling_tick  <= '0';
         end if;
      end if;
   end if;
end process;


-------------------------------------------------------------
-- Count bits:
-------------------------------------------------------------
make_bit_counter : process(clk_i) begin
   if clk_i'event and clk_i = '1' then
      if cs = '1' then
         bit_count   <= "000";
         byte_done   <= '0';
         byte_begin  <= '1';
      elsif sck_falling_tick = '1' then
         bit_count <= bit_count + 1;
         if bit_count = "110" then
            byte_done <= '1';
         else
            byte_done <= '0';
         end if;
         if bit_count = "111" then
            byte_begin <= '1';
         else
            byte_begin <= '0';
         end if;
      end if;
   end if;
end process;


-------------------------------------------------------------
-- Delayline of mosi :
-------------------------------------------------------------
mosi_del_line(0) <= mosi;
mosi_del_line_update : process(clk_i) begin
   if clk_i'event and clk_i = '1' then
      if sck_falling_tick = '1' then
         mosi_del_line(23 downto 1) <= mosi_del_line(22 downto 0);
      end if;
   end if;
end process;


-------------------------------------------------------
-- state machine for recieving serial data :
-------------------------------------------------------
serial_data_sm : process (clk_i) begin
   if clk_i'event and clk_i ='1' then
      if cs = '1' then
         state       <= header;
         adr_count   <= (others => '0');
      elsif sck_falling_tick = '1' then
         if byte_done = '1' then
            case state is
            when header =>
               if adr_count = conv_std_logic_vector(2,16) then
                  channel_sel <= mosi_del_line(19 downto 16);
                  adr_count   <= mosi_del_line(15 downto 0);
                  state       <= data;
               else
                  adr_count   <= adr_count + 1;
               end if;
            when data =>
               if adr_count /= X"FFFF" then
                  adr_count <= adr_count + 1;
               end if;
            when others =>
               null;
            end case;
         end if;
      end if;
   end if;
end process;


--------------------------------------------------------------
-- channel_id_bits:
--------------------------------------------------------------
with channel_id select
   channel_id_bits   <= "0001"   when 1,
                        "0010"   when 2,
                        "0100"   when 3,
                        "1000"   when 4,
                        "0000"   when others;

--------------------------------------------------------------
-- Map serial data to output parameters :
--------------------------------------------------------------
parameter_register_update: process(clk_i) begin
   if clk_i'event and clk_i = '1' then
      if reset_i = '1' then
         system_o(11 downto 4) <= "00001111";
      else
         if state = data then
            if channel_sel = channel_id_bits then
               if sck_falling_tick = '1' then
                  if byte_done = '1' then
                     case adr_count is
                     when "0000000000000000" => system_o(11 downto 4)   <= mosi_del_line( 7 downto 0);
                     when "0000000000000001" => pattern_o(15 downto 8)  <= mosi_del_line( 7 downto 0);
                     when "0000000000000010" => pattern_o(7 downto 0)   <= mosi_del_line( 7 downto 0);
                     when "0000000000000101" => timing_o                <= mosi_del_line(23 downto 0);
                     when "0000000000000110" => system_o(3 downto 0)    <= mosi_del_line( 3 downto 0);
                     when "0000000000001000" => click_timing_o          <= mosi_del_line(11 downto 0);
                     when "0000000000001001" => audio_signal_index_o    <= mosi_del_line( 7 downto 0);
                     when "0000000000001010" => audio_level_index_o     <= mosi_del_line( 7 downto 0);
                     when "0000000000001100" => bmp_h_position_o        <= mosi_del_line(15 downto 0);
                     when "0000000000001110" => bmp_v_position_o        <= mosi_del_line(15 downto 0);
                     when "0000000000001111" => bmp_scale_o             <= mosi_del_line( 7 downto 0);
                     when "0000000000010000" => bmp_back_text_color_o   <= mosi_del_line( 7 downto 0);
                     when "0000000000010001" => bmp_h_size_o            <= mosi_del_line( 7 downto 0);
                     when "0000000000010010" => bmp_v_size_o            <= mosi_del_line( 7 downto 0);
                     when "0000000000010011" => bmp_movement_o          <= mosi_del_line( 7 downto 0);
                     when "0000000000010100" => bmp_on_zoff_o           <= mosi_del_line( 0);
                     when "0000000010001000" => clock_error_mask_o      <= mosi_del_line( 7 downto 0);
                     when "0000000010001001" => channel_error_mask_o    <= mosi_del_line( 7 downto 0);
                     when "0000000010010010" => frame_count_25_o        <= mosi_del_line( 7 downto 0);
                                                new_frame_count_25_o    <= '1';
                     when "0000000010010011" => frame_count_30_o        <= mosi_del_line( 7 downto 0);
                                                new_frame_count_30_o    <= '1';
                     when "0000000010010100" => set_frame_count         <= mosi_del_line( 1 downto 0);
                     when others             => null;
                     end case;
                  end if;
               end if;
            end if;
         else     -- state = header
            new_frame_count_25_o <= '0';
            new_frame_count_30_o <= '0';
            set_frame_count      <= "00";
         end if;
      end if;
   end if;
end process;

set_frame_count_25_o <= set_frame_count(0);
set_frame_count_30_o <= set_frame_count(1);

--------------------------------------------------------------
-- load register and output serially:
--------------------------------------------------------------
load_miso_register: process(channel_sel, channel_id_bits, state, sck_rising_tick, byte_begin, adr_count) begin
   if channel_sel = channel_id_bits and state = data and sck_rising_tick = '1' and byte_begin = '1' then
      case adr_count is
      when "0000000010000000" => -- 80h
         load_with_channel       <= '1';
         load_with_warning_clock <= '0';
         load_with_error_clock   <= '0';
         load_with_non_sticky    <= '0';
      when "0000000010000001" => -- 81h
         load_with_channel       <= '0';
         load_with_warning_clock <= '1';
         load_with_error_clock   <= '0';
         load_with_non_sticky    <= '0';
      when "0000000010000010" => -- 82h
         load_with_channel       <= '0';
         load_with_warning_clock <= '0';
         load_with_error_clock   <= '1';
         load_with_non_sticky    <= '0';
      when others             =>
         load_with_channel       <= '0';
         load_with_warning_clock <= '0';
         load_with_error_clock   <= '0';
         load_with_non_sticky    <= '1';
      end case;
   else
      load_with_channel          <= '0';
      load_with_warning_clock    <= '0';
      load_with_error_clock      <= '0';
      load_with_non_sticky       <= '0';
   end if;
end process;

sticky_registers: process(clk_i) begin
   if clk_i'event and clk_i = '1' then
      if load_with_channel = '1' then
         status_channel_sticky   <= status_channel_i;
      else
         status_channel_sticky   <= status_channel_sticky or status_channel_i;
      end if;
      if load_with_error_clock = '1' then
         error_clock_sticky      <= error_clock_i;
      else
         error_clock_sticky      <= error_clock_sticky or error_clock_i;
      end if;
      if load_with_warning_clock = '1' then
         warning_clock_sticky    <= warning_clock_i;
      else
         warning_clock_sticky    <= warning_clock_sticky or warning_clock_i;
      end if;
   end if;
end process;

register_serializing: process(clk_i) begin
   if clk_i'event and clk_i = '1' then
      if channel_sel = channel_id_bits and state = data then
         if load_with_channel = '1' then
            miso_register     <= status_channel_sticky;
         elsif load_with_warning_clock = '1' then
            miso_register     <= warning_clock_sticky;
         elsif load_with_error_clock = '1' then
            miso_register     <= error_clock_sticky;
         elsif load_with_non_sticky = '1' then
            case adr_count is
            when "0000000010010000" => -- 90h
               miso_register     <= frame_count_25_i;
            when "0000000010010001" => -- 91h
               miso_register     <= frame_count_30_i;
            when others             =>
               miso_register     <= (others => '1');
            end case;
         elsif sck_rising_tick = '1' and byte_begin = '0' then
            miso_register     <= miso_register(6 downto 0) & '1';  -- output msb first
         end if;
      else
         miso_register <= (others => '1');
      end if;
   end if;
end process;

miso_o   <= miso_register(7);


-------------------------------------------------------------
-- Generate ram write for text ram :
-------------------------------------------------------------
ram_write_generation: process(clk_i) begin
   if clk_i'event and clk_i = '1' then
      if channel_sel = channel_id_bits and sck_falling_tick = '1' and byte_done = '1' and state = data and adr_count(15 downto 9) = "0001000" then
         text_ram_write <= '1';
      else
         text_ram_write <= '0';
      end if;
      text_ram_addr <= adr_count(8 downto 0);
   end if;
end process;


end Behavioral;