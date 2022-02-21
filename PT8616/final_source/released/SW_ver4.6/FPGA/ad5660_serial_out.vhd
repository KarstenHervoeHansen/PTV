library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity ad5660_serial_out is
   port (
      clk_148_i                  : in std_logic;                        
      data_i                     : in std_logic_vector(15 downto 0);
      new_data_i                 : in std_logic;                        
      sclk_o                     : out std_logic;
      dout_o                     : out std_logic;
      zsync_o                    : out std_logic
   );
end ad5660_serial_out;


architecture Behavioral of ad5660_serial_out is

signal bit_count                 : std_logic_vector(7 downto 0);
signal bit_tick                  : std_logic;
signal shift_reg                 : std_logic_vector(23 downto 0);
signal zsync                     : std_logic;


begin


-------------------------------------------------------------
-- Make bit counter
-------------------------------------------------------------
bit_counter : process (clk_148_i) begin
   if clk_148_i'event and clk_148_i = '1' then
      if new_data_i = '1' then
         bit_count <= "11000111";  -- 24 & 8
      else
         if bit_count = 0 then
            bit_count <= (others => '0');
         else
            bit_count <= bit_count - 1;
         end if;
      end if;
   end if;
end process;

sclk_o <= bit_count(2);

make_bit_tick : process (clk_148_i) begin
   if clk_148_i'event and clk_148_i = '1' then
      if new_data_i = '1' then
         bit_tick <= '0';
      else
         if bit_count(2 downto 0) = "001" then
            bit_tick <= '1';
         else
            bit_tick <= '0';
         end if;
      end if;
   end if;
end process;

make_shift_reg : process (clk_148_i) begin
   if clk_148_i'event and clk_148_i = '1' then
      if new_data_i = '1' then
         shift_reg <= "00000000" & data_i;
      else
         if bit_tick = '1' and zsync = '0' then
            shift_reg(23 downto 0) <= shift_reg(22 downto 0) & '0';
         end if;
      end if;
   end if;
end process;

dout_o <= shift_reg(23);

make_zsync : process (clk_148_i) begin
   if clk_148_i'event and clk_148_i = '1' then
      if new_data_i = '1' then
         zsync <= '1';
      else
         if bit_tick = '1' then
            zsync <= '0';
         end if;
      end if;
   end if;
end process;

zsync_o <= zsync;

end Behavioral;