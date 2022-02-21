library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity glitch_remover is
   port (
      clk_i             : in std_logic;
      signal_i          : in std_logic;
      clean_signal_o    : out std_logic
   );
end glitch_remover;


architecture behavioral of glitch_remover is


signal delayed_signals : std_logic_vector(3 downto 0);


begin


---------------------------------------------------------------
-- Delaying input signals :
---------------------------------------------------------------
signal_delaying : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      delayed_signals(0) <= signal_i;
      delayed_signals(3 downto 1) <= delayed_signals(2 downto 0);
   end if;
end process;


clean_signal_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      case delayed_signals is
         when "0000" =>
            clean_signal_o <= '0';
         when "1111" =>
            clean_signal_o <= '1';
         when others =>
            null;
      end case;
   end if;
end process;


end behavioral;