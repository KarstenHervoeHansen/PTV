library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity phase_comparator is
   port (
      clk_ref_i            : in std_logic;
      clk_vco_i            : in std_logic;
      faster_o             : inout std_logic;
      slower_o             : inout std_logic
   );
end phase_comparator;


architecture behavioral of phase_comparator is

signal vcxo_faster            : std_logic;
signal vcxo_slower            : std_logic;
signal pulses_are_zero        : std_logic;
signal pulses_are_one         : std_logic;
signal z_reset_pulses         : std_logic;
signal z_reset_at_ref_falling : std_logic;

begin

--------------------------------------------------------------------
-- Phase comparator:
--------------------------------------------------------------------
--phase_comparison : process (clk_ref_i, clk_vco_i)
--begin
--
--   if clk_vco_i = '1' then
--      vcxo_faster <= '0';
--   else
--      if clk_ref_i'event and clk_ref_i = '1' then
--            vcxo_faster <= '1';
--      end if;
--   end if;
--
--   if clk_ref_i = '1' then
--      vcxo_slower <= '0';
--   else
--      if clk_vco_i'event and clk_vco_i = '1' then
--            vcxo_slower <= '1';
--      end if;
--   end if;
--
--end process;


--------------------------------------------------------------------
-- Phase comparator:
--------------------------------------------------------------------
phase_comparison_faster : process (clk_ref_i, z_reset_pulses)
begin
   if z_reset_pulses = '0' then
      vcxo_faster <= '0';
   else
      if clk_ref_i'event and clk_ref_i = '1' then
            vcxo_faster <= '1';
      end if;
   end if;
end process;

phase_comparison_slower : process (clk_vco_i, z_reset_pulses)
begin
   if z_reset_pulses = '0' then
      vcxo_slower <= '0';
   else
      if clk_vco_i'event and clk_vco_i = '1' then
            vcxo_slower <= '1';
      end if;
   end if;
end process;

reset_at_falling_ref_generation : process (clk_ref_i, pulses_are_zero)
begin
   if pulses_are_zero = '1' then
      z_reset_at_ref_falling <= '1';
   else
      if clk_ref_i'event and clk_ref_i = '0' then
         z_reset_at_ref_falling <= '0';
      end if;
   end if;
end process;

pulses_are_zero   <= (not vcxo_slower) and (not vcxo_faster);
pulses_are_one    <= vcxo_slower and vcxo_faster;
z_reset_pulses    <= (not pulses_are_one); -- and z_reset_at_ref_falling;


--------------------------------------------------------------------
-- VCXO control voltage:
--------------------------------------------------------------------
--vcxo_control_generation : process (vcxo_faster, vcxo_slower)
--begin
--   if vcxo_faster = '1' then
--      diff_pulse_o <= '1';
--   elsif vcxo_slower = '1' then
--      diff_pulse_o <= '0';
--   else
--      diff_pulse_o <= 'Z';
--   end if;
--end process;


faster_generation : process (vcxo_faster)
begin
   if vcxo_faster = '1' then
      faster_o <= '1';
   else
      faster_o <= 'Z';
   end if;
end process;


slower_generation : process (vcxo_slower)
begin
   if vcxo_slower = '1' then
      slower_o <= '0';
   else
      slower_o <= 'Z';
   end if;
end process;


end behavioral;