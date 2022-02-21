library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity noise_generator is
   generic (
      seed                    : integer        -- 0 to 24
   );
   port (
      clk_148_i               : in std_logic;
      en_i                    : in std_logic;
      sync_reset_i            : in std_logic;
      random_word_o           : out std_logic_vector(9 downto 0)
   );
end noise_generator;


architecture Behavioral of noise_generator is

constant register_length : integer := 250;
constant register_outlet : integer := 103;

type array_type is array(0 to register_length-1) of std_logic_vector(9 downto 0);
signal register_data          : array_type;


begin


random_generation : process (clk_148_i) begin
   if clk_148_i'event and clk_148_i = '1' then
      if sync_reset_i = '1' then
         for i in 0 to 9 loop
            register_data(i+seed*10) (i) <= '1';
         end loop;
      else
         if en_i = '1' then
            for i in 1 to register_length-1 loop
               register_data(i) <= register_data(i-1);
               register_data(0) <= register_data(register_length-1) xor register_data(register_outlet-1);
            end loop;
         end if;
      end if;
   end if;
end process;

random_word_o <= register_data(0);

end Behavioral;