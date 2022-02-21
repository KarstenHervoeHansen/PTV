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
      clk_i                   : in  std_logic;     -- clock
      reset_i                 : in  std_logic;     -- sync_reset
      cs_i                    : in  std_logic;     -- chip select
      sck_i                   : in  std_logic;     -- serial clock
      mosi_i                  : in  std_logic;     -- master out serial data in

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

      text_ram_clk_i          : in  std_logic;     -- clock for reading text ram
      text_ram_addr_i         : in  std_logic_vector( 8 downto 0);
      text_ram_en_i           : in  std_logic;     -- enable signal for reading text ram
      text_ram_data_o         : out std_logic_vector( 7 downto 0)
   );
end serial_interface;


architecture Behavioral of serial_interface is

signal cs                     : std_logic;
signal cs_delayed             : std_logic;
signal sck                    : std_logic;
signal sck_delayed            : std_logic;
signal mosi                   : std_logic;
signal sck_tick               : std_logic;
signal sck_tick_advanced      : std_logic;
signal byte_done              : std_logic;
signal mosi_del_line          : std_logic_vector(23 downto 0);
signal bit_count              : std_logic_vector(2 downto 0);
signal adr_count              : std_logic_vector(15 downto 0);
signal text_ram_addr          : std_logic_vector(8 downto 0);
signal channel_sel            : std_logic_vector(3 downto 0);  --// select channel RAM
signal text_ram_write         : std_logic;
signal click_timing           : std_logic_vector(11 downto 0);


type state_type is (header, data);
signal state                  : state_type;


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
      ena    => channel_sel(channel_id),
      wea    => text_ram_write,
      clkb   => text_ram_clk_i,
      addrb  => text_ram_addr_i,
      doutb  => text_ram_data_o,
      enb    => text_ram_en_i
   );


--------------------------------------------------------------
-- Make tick at spi_sck falling edge:
--------------------------------------------------------------
sck_ticking: process(clk_i) begin
   if clk_i'event and clk_i ='1' then
      if cs = '1' then
         sck_delayed       <= '0';
         sck_tick          <= '0';
         sck_tick_advanced <= '0';
      else
         if sck = '0' and sck_delayed = '1' then   -- falling edge :
            sck_tick_advanced <= '1';
         else
            sck_tick_advanced <= '0';
         end if;
         sck_delayed <= sck;
         sck_tick    <= sck_tick_advanced;
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
      elsif sck_tick = '1' then
         if bit_count = "111" then
            bit_count <= "000";
         else
            bit_count <= bit_count + 1;
         end if;
         if bit_count = "110" then
            byte_done <= '1';
         else
            byte_done <= '0';
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
      if sck_tick = '1' then
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
      elsif sck_tick = '1' then
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
-- Map serial data to output parameters :
--------------------------------------------------------------
parameter_register_update: process(clk_i) begin
   if clk_i'event and clk_i = '1' then
      if reset_i = '1' then
         system_o(11 downto 4) <= "00001111";
      else
         if channel_sel(channel_id) = '1' then
            if sck_tick = '1' then
               if byte_done = '1' then
                  if state = data then
                     case adr_count is
                     when "0000000000000000" => system_o(11 downto 4)   <= mosi_del_line( 7 downto 0);
                     when "0000000000000001" => system_o(3 downto 0)    <= mosi_del_line( 3 downto 0);
                     when "0000000000000010" => pattern_o(15 downto 8)  <= mosi_del_line( 7 downto 0);
                     when "0000000000000011" => pattern_o(7 downto 0)   <= mosi_del_line( 7 downto 0);
                     when "0000000000000110" => timing_o                <= mosi_del_line(23 downto 0);
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
                     when others             => null;
                     end case;
                  end if;
               end if;
            end if;
         end if;
      end if;
   end if;
end process;


--parameter_register_update: process(clk_i) begin
--   if clk_i'event and clk_i = '1' then
--      if channel_sel(channel_id) = '1' then
--         if sck_tick = '1' then
--            if byte_done = '1' then
--               if state = data then
--                  case adr_count is
--                  when "0000000000000000" => system_o(11 downto 4)   <= mosi_del_line( 7 downto 0);
--                  when "0000000000000001" => pattern_o(15 downto 8)  <= mosi_del_line( 7 downto 0);
--                  when "0000000000000010" => pattern_o(7 downto 0)   <= mosi_del_line( 7 downto 0);
--                  when "0000000000000101" => timing_o                <= mosi_del_line(23 downto 0);
--                  when "0000000000001000" => click_timing            <= mosi_del_line(11 downto 0);
--                  when "0000000000001001" => audio_signal_index_o    <= mosi_del_line( 7 downto 0);
--                  when "0000000000001010" => audio_level_index_o     <= mosi_del_line( 7 downto 0);
--                  when "0000000000001100" => bmp_h_position_o        <= mosi_del_line(15 downto 0);
--                  when "0000000000001110" => bmp_v_position_o        <= mosi_del_line(15 downto 0);
--                  when "0000000000001111" => bmp_scale_o             <= mosi_del_line( 7 downto 0);
--                  when "0000000000010000" => bmp_back_text_color_o   <= mosi_del_line( 7 downto 0);
--                  when "0000000000010001" => bmp_h_size_o            <= mosi_del_line( 7 downto 0);
--                  when "0000000000010010" => bmp_v_size_o            <= mosi_del_line( 7 downto 0);
--                  when "0000000000010011" => bmp_movement_o          <= mosi_del_line( 7 downto 0);
--                  when "0000000000010100" => bmp_on_zoff_o           <= mosi_del_line( 0);
--                  when others             => null;
--                  end case;
--               end if;
--            end if;
--         end if;
--      end if;
--   end if;
--end process;
--
--system_o(3 downto 0) <= click_timing(3 downto 0);
--click_timing_o       <= click_timing;


-------------------------------------------------------------
-- Generate ram write for text ram :
-------------------------------------------------------------
ram_write_generation: process(clk_i) begin
   if clk_i'event and clk_i = '1' then
      if channel_sel(channel_id) = '1' and sck_tick = '1' and byte_done = '1' and state = data and adr_count(15 downto 9) = "0001000" then
         text_ram_write <= '1';
      else
         text_ram_write <= '0';
      end if;
      text_ram_addr <= adr_count(8 downto 0);
   end if;
end process;


end Behavioral;