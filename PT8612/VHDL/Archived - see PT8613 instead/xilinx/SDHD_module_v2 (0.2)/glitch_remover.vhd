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
      delayed_signals(1) <= delayed_signals(0);
      delayed_signals(2) <= delayed_signals(1);
   end if;
end process;


clean_signal_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      case delayed_signals is
         when "1000" =>
            delayed_signals(3) <= '0';
         when "0111" =>
            delayed_signals(3) <= '1';
         when others =>
            null;
      end case;
   end if;
end process;


clean_signal_o <= delayed_signals(3);


end behavioral;