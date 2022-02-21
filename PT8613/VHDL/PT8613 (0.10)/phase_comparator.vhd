library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity phase_comparator is
   port (
      clk_ref_i            : in std_logic;
      clk_vco_i            : in std_logic;
      diff_pulse_o         : inout std_logic
   );
end phase_comparator;


architecture behavioral of phase_comparator is

signal vcxo_faster         : std_logic;
signal vcxo_slower         : std_logic;


begin

--------------------------------------------------------------------
-- Phase comparator:
--------------------------------------------------------------------
phase_comparison : process (clk_ref_i, clk_vco_i)
begin
   if clk_vco_i = '0' then
      if clk_ref_i'event and clk_ref_i = '1' then
            vcxo_faster <= '1';
      end if;
   else
      vcxo_faster <= '0';
   end if;
   if clk_ref_i = '0' then
      if clk_vco_i'event and clk_vco_i = '1' then
            vcxo_slower <= '1';
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
      vcxo_control_o <= '1';
   elsif vcxo_slower = '1' then
      vcxo_control_o <= '0';
   else
      vcxo_control_o <= 'Z';
   end if;
end process;


end behavioral;