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
      system_o        : out std_logic_vector(14 downto 0);
      timing_o        : out std_logic_vector(23 downto 0)
      --debug:
--      debug_o         : out std_logic_vector(15 downto 0) := (others => '0')

   );
end serial_interface;


architecture Behavioral of serial_interface is


signal cs                     : std_logic;
signal sck                    : std_logic;
signal sck_delayed1           : std_logic;
signal mosi                   : std_logic;
signal sck_tick               : std_logic;
signal bitptr                 : integer range 0 to 63;
signal system                 : std_logic_vector(14 downto 0);
signal timing                 : std_logic_vector(23 downto 0);


constant default_timing       : std_logic_vector(23 downto 0) := (others => '0');
constant default_system       : std_logic_vector(14 downto 0) := (others => '0');   -- Mute


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
            when 34 => system(14)   <= mosi;    --sysclk_sel
            when 35 => system(13)   <= mosi;    --sync_mode(1)
            when 36 => system(12)   <= mosi;    --sync_mode(0)
            when 39 => system(11)   <= mosi;    --spl_div(9)
            when 40 => system(10)   <= mosi;    --spl_div(8)
            when 41 => system(9)    <= mosi;    --spl_div(7)
            when 42 => system(8)    <= mosi;    --spl_div(6)
            when 43 => system(7)    <= mosi;    --spl_div(5)
            when 44 => system(6)    <= mosi;    --spl_div(4)
            when 45 => system(5)    <= mosi;    --spl_div(3)
            when 46 => system(4)    <= mosi;    --spl_div(2)
            when 47 => system(3)    <= mosi;    --spl_div(1)
            when 48 => system(2)    <= mosi;    --spl_div(0)
            when 52 => system(1)    <= mosi;    --scan_method
            when 54 => system(0)    <= mosi;    --lpf_msb indicates 750 or 1125 lines
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
      timing_o <= (others => '0');
      system_o <= (others => '0');
   elsif clk_i'event and clk_i ='1' then
      if cs = '1' then
         timing_o <= timing;
         system_o <= system;
      end if;
   end if;
end process;


------------------------------------------------------------
-- Debug:
------------------------------------------------------------
--debug_o(0)  <= sck_tick;
--
--debug_o(2)  <= cs;
--debug_o(3)  <= system(14);
--
--debug_monitoring : process(mreset_i, clk_i) begin
--   if mreset_i = '0' then
--      debug_o(1)  <= '1';
--   elsif clk_i'event and clk_i = '1' then
--      if bitptr = 0 then
--         debug_o(1)  <= '1';
--      else
--         debug_o(1)  <= '0';
--      end if;
--   end if;
--end process;


end Behavioral;