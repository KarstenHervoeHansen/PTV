library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity colorbar_generator is
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;

      -- system parameters:
      pattern_i            : in std_logic_vector(15 downto 0);    -- "00": 100/0/100/0, "01": 100/0/75/0, "10": 75/0/75/0
      
      -- frame control signals:
      h_grid_state_i       : in std_logic_vector(3 downto 0);


      -- color parameter outputs:
      color_presence_o     : out std_logic_vector(2 downto 0);
      color_level_o        : out std_logic_vector(4 downto 0)
   );
end colorbar_generator;


architecture behavioral of colorbar_generator is


signal start_color_level         : std_logic_vector(4 downto 0);
signal color_color_level         : std_logic_vector(4 downto 0);

begin


---------------------------------------------------------------
-- Generate color presence :
---------------------------------------------------------------
color_presence_generation : process(clk_i)
begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         case h_grid_state_i(3 downto 1) is
            when "000"  => color_presence_o  <= "111";   -- white
            when "001"  => color_presence_o  <= "110";   -- yellow
            when "010"  => color_presence_o  <= "011";   -- cyan
            when "011"  => color_presence_o  <= "010";   -- green
            when "100"  => color_presence_o  <= "101";   -- magenta
            when "101"  => color_presence_o  <= "100";   -- red
            when "110"  => color_presence_o  <= "001";   -- blue
            when others => color_presence_o  <= "000";   -- black
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate color level :
---------------------------------------------------------------
color_level_generation : process(clk_i)
begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         case h_grid_state_i(3 downto 1) is
            when "000"  => color_level_o  <= start_color_level;
            when others => color_level_o  <= color_color_level;
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Parameters :
---------------------------------------------------------------

with pattern_i(1 downto 0) select
   start_color_level       <= "10000"     when "10",     -- 75/0/75/0  
                              "10101"     when others;   -- 100/0/X/0               

with pattern_i(1 downto 0) select
   color_color_level       <= "10101"     when "00",     -- 100/0/100/0
                              "10000"     when others;   -- X/0/75/0               
                             

end Behavioral;