library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity serial_interface is
   Port(
      mreset_i        : in std_logic;                          -- master reset
      clk_i           : in std_logic;                          -- clock
      cs_i            : in std_logic;                          -- chip select
      sck_i           : in std_logic;                          -- serial clock
      mosi_i          : in std_logic;                          -- master out serial data in
      system_o        : out std_logic_vector(7 downto 0);
      pattern_o       : out std_logic_vector(15 downto 0);
      timing_o        : out std_logic_vector(23 downto 0)
   );
end serial_interface;


architecture Behavioral of serial_interface is


signal cs                     : std_logic;
signal sck                    : std_logic;
signal sck_delayed1           : std_logic;
signal mosi                   : std_logic;
signal sck_tick               : std_logic;
signal bitptr                 : integer range 0 to 63;
signal system                 : std_logic_vector(7 downto 0);
signal pattern                : std_logic_vector(15 downto 0);
signal timing                 : std_logic_vector(23 downto 0);

constant default_timing       : std_logic_vector(23 downto 0) := (others => '0');
constant default_system       : std_logic_vector(7 downto 0) := X"A0";--(others => '1');   --off
constant default_pattern      : std_logic_vector(15 downto 0) := (others => '1');   --black

begin


----------------------------------------------------------------
-- Reclocking of signals from serial interface to clk_i domain:
----------------------------------------------------------------
signal_reclocking: process (mreset_i, clk_i) begin
   if mreset_i = '0' then
      cs             <= '1';
      sck            <= '0';
      sck_delayed1   <= '0';
      mosi           <= '0';
   elsif clk_i'event and clk_i ='1' then
      cs             <= cs_i;
      sck            <= sck_i;
      sck_delayed1   <= sck;
      mosi           <= mosi_i;
   end if;
end process;


----------------------------------------------------------------
-- Make tick at sck falling edge:
----------------------------------------------------------------
sck_ticking: process (mreset_i, clk_i) begin
   if mreset_i = '0' then
      sck_tick <= '0';
   elsif clk_i'event and clk_i ='1' then
      if sck = '0' and sck_delayed1 = '1' and cs = '0' then
         sck_tick <= '1';
      else
         sck_tick <= '0';
      end if;
   end if;
end process;


----------------------------------------------------------------
-- Make pointer to clock in data at sck faling edge:
----------------------------------------------------------------
make_bitptr : process(mreset_i, clk_i) begin
   if mreset_i = '0' then
      bitptr <= 1;
   elsif clk_i'event and clk_i = '1' then
      if cs = '1' then
         bitptr <= 1;
      elsif sck_tick = '1' then
         bitptr <= bitptr + 1;
      end if;
   end if;
end process;


----------------------------------------------------------------
-- Clock in serial data:
----------------------------------------------------------------
get_serial_data : process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      timing <= default_timing;
      system <= default_system;
      pattern <= default_pattern;
   elsif clk_i'event and clk_i = '1' then
      if sck_tick = '1' then
         case bitptr is
            when 9  => timing(23)   <= mosi;
            when 10 => timing(22)   <= mosi;
            when 11 => timing(21)   <= mosi;
            when 12 => timing(20)   <= mosi;
            when 13 => timing(19)   <= mosi;
            when 14 => timing(18)   <= mosi;
            when 15 => timing(17)   <= mosi;
            when 16 => timing(16)   <= mosi;
            when 17 => timing(15)   <= mosi;
            when 18 => timing(14)   <= mosi;
            when 19 => timing(13)   <= mosi;
            when 20 => timing(12)   <= mosi;
            when 21 => timing(11)   <= mosi;
            when 22 => timing(10)   <= mosi;
            when 23 => timing(9)    <= mosi;
            when 24 => timing(8)    <= mosi;
            when 25 => timing(7)    <= mosi;
            when 26 => timing(6)    <= mosi;
            when 27 => timing(5)    <= mosi;
            when 28 => timing(4)    <= mosi;
            when 29 => timing(3)    <= mosi;
            when 30 => timing(2)    <= mosi;
            when 31 => timing(1)    <= mosi;
            when 32 => timing(0)    <= mosi;
            when 33 => system(7)    <= mosi;    --zSD_HD
            when 34 => system(6)    <= mosi;    --zPAL_NTSC
            when 35 => system(5)    <= mosi;    --Interlaced
            when 36 => system(4)    <= mosi;    --z7425_7417
            when 37 => system(3)    <= mosi;    --HD_format(3)
            when 38 => system(2)    <= mosi;    --HD_format(2)
            when 39 => system(1)    <= mosi;    --HD_format(1)
            when 40 => system(0)    <= mosi;    --HD_format(0)
            when 41 => pattern(15)  <= mosi;
            when 42 => pattern(14)  <= mosi;
            when 43 => pattern(13)  <= mosi;
            when 44 => pattern(12)  <= mosi;
            when 45 => pattern(11)  <= mosi;
            when 46 => pattern(10)  <= mosi;
            when 47 => pattern(9)   <= mosi;
            when 48 => pattern(8)   <= mosi;
            when 49 => pattern(7)   <= mosi;    --TP_option(7)
            when 50 => pattern(6)   <= mosi;
            when 51 => pattern(5)   <= mosi;
            when 52 => pattern(4)   <= mosi;
            when 53 => pattern(3)   <= mosi;
            when 54 => pattern(2)   <= mosi;
            when 55 => pattern(1)   <= mosi;
            when 56 => pattern(0)   <= mosi;    --TP_option(0)
            when others => null;
         end case;
      end if;
   end if;
end process;


------------------------------------------------------------
-- Output reclocking:
------------------------------------------------------------
output_reclocking: process (mreset_i, clk_i) begin
   if mreset_i = '0' then
      timing_o <= default_timing;
      system_o <= default_system;
      pattern_o <= default_pattern;
   elsif clk_i'event and clk_i ='1' then
      if cs = '1' then
         timing_o <= timing;
         system_o <= system;
         pattern_o <= pattern;
      end if;
   end if;
end process;


end Behavioral;