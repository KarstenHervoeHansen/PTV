library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity pluge_generator is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;
      
      -- frame control signals:
      h_state_i            : in std_logic_vector(2 downto 0);
      v_state_i            : in std_logic_vector(3 downto 0);
      
      -- color parameter outputs:
      color_presence_o     : out std_logic_vector(2 downto 0);
      color_level_o        : out std_logic_vector(4 downto 0)
   );
end pluge_generator;


architecture behavioral of pluge_generator is


signal grey_level          : std_logic_vector(4 downto 0);

constant grey_1_level      : std_logic_vector(4 downto 0) := "11001";
constant grey_2_level      : std_logic_vector(4 downto 0) := "11010";
constant grey_3_level      : std_logic_vector(4 downto 0) := "11011";
constant white_level       : std_logic_vector(4 downto 0) := "10101";


begin


---------------------------------------------------------------
-- Generate color presence :
---------------------------------------------------------------
color_presence_o <= "111";


---------------------------------------------------------------
-- Generate color level :
---------------------------------------------------------------
color_level_generation : process(zreset_i, clk_i)
begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         case h_state_i is
         when "000" =>  color_level_o <= "00001";  --  0%
         when "001" =>  color_level_o <= "10111";  -- -2%
         when "010" =>  color_level_o <= "00001";  --  0%
         when "011" =>  color_level_o <= "11000";  --  2%
         when "100" =>  color_level_o <= "00001";  --  0%
         when "111" =>  color_level_o <= "00001";  --  0%
         when others => color_level_o <= grey_level;  --  different grey levels
         end case;
      end if;
   end if;
end process;


with v_state_i select
   grey_level  <= white_level  when "0000",
                  white_level  when "0001",
                  grey_1_level when "0010",
                  grey_1_level when "0011",
                  grey_2_level when "0100",
                  grey_2_level when "0101",
                  grey_3_level when others;

end Behavioral;