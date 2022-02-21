library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity pll_controller is
   generic (
      ref_div_minus_1      : integer;
      vco_div_minus_1      : integer
   );
   port (
      clk_ref_i            : in std_logic;
      clk_vco_i            : in std_logic;
      vcxo_control_o       : inout std_logic
   );
end pll_controller;


architecture behavioral of pll_controller is

signal f27_count           : std_logic_vector(7 downto 0);
signal f27_divided         : std_logic;
signal f148_count          : std_logic_vector(9 downto 0);
signal f148_divided        : std_logic;
signal vcxo_faster         : std_logic;
signal vcxo_slower         : std_logic;
signal vcxo_control        : std_logic;

begin


--------------------------------------------------------------------
-- Frequency dividers :
--------------------------------------------------------------------
f27_frequency_divider : process (clk_ref_i)
begin
   if clk_ref_i'event and clk_ref_i = '1' then
      if f27_count = 0 then
         f27_count      <= conv_std_logic_vector(ref_div_minus_1,8);    -- period = 182
         f27_divided    <= not f27_divided;
      else
         f27_count      <= f27_count - 1;
      end if;
   end if;
end process;

f148_frequency_divider : process (clk_vco_i)
begin
   if clk_vco_i'event and clk_vco_i = '1' then
      if f148_count = 0 then
         f148_count    <= conv_std_logic_vector(vco_div_minus_1,10);     -- period = 1001 for 148.5Mhz
         f148_divided  <= not f148_divided;
      else
         f148_count    <= f148_count - 1;
      end if;
   end if;
end process;


--------------------------------------------------------------------
-- Phase comparator:
--------------------------------------------------------------------
phase_comparison : process (f27_divided, f148_divided)
begin
   if f148_divided = '0' then
      if f27_divided'event and f27_divided = '1' then
--         if f148_divided = '0' then
            vcxo_faster <= '1';
--         end if;
      end if;
   else
      vcxo_faster <= '0';
   end if;
   if f27_divided = '0' then
      if f148_divided'event and f148_divided = '1' then
--         if f27_divided = '0' then
            vcxo_slower <= '1';
--         end if;
      end if;
   else
      vcxo_slower <= '0';
   end if;
end process;


--------------------------------------------------------------------
-- VCXO control voltage:
--------------------------------------------------------------------
vcxo_control_generation : process (vcxo_faster, vcxo_slower)
begin
   if vcxo_faster = '1' then
      vcxo_control <= '1';
   elsif vcxo_slower = '1' then
      vcxo_control <= '0';
   else
      vcxo_control <= 'Z';
   end if;
end process;


vcxo_control_o <= vcxo_control;


end behavioral;