library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity window_generator is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      -- system parameters:
      pattern_i            : in std_logic_vector(15 downto 0);
      
      -- frame control signal:
      v_grid_state_i       : in std_logic_vector(3 downto 0);
      h_grid_state_i       : in std_logic_vector(3 downto 0);
      
      -- color parameter outputs:
      color_presence_o     : out std_logic_vector(2 downto 0);
      color_level_o        : out std_logic_vector(4 downto 0)
   );
end window_generator;


architecture behavioral of window_generator is

signal color_level         : std_logic_vector(4 downto 0);
signal window_active_v     : std_logic;
signal window_active_h     : std_logic;
signal full_zwindow        : std_logic;

begin


--------------------------------------------
-- Mapping of color_presence_o :
--------------------------------------------
color_presence_o <= "111";


--------------------------------------------
-- Indicate active window horizontal :
--------------------------------------------
window_active_h_generation : process (clk_i, zreset_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         case h_grid_state_i is
         when "0101" => window_active_h <= '1';
         when "0110" => window_active_h <= '1';
         when "0111" => window_active_h <= '1';
         when "1000" => window_active_h <= '1';
         when "1001" => window_active_h <= '1';
         when "1010" => window_active_h <= '1';
         when others => window_active_h <= '0';
         end case;
      end if;
   end if;
end process;


--------------------------------------------
-- Indicate active window vertical :
--------------------------------------------
with v_grid_state_i select
   window_active_v <= '1' when "0010",
                      '1' when "0011",
                      '1' when "0100",
                      '1' when "0101",
                      '1' when "0110",
                      '0' when others;
             
                      
--------------------------------------------
-- Indicate active window vertical :
--------------------------------------------
with pattern_i(15 downto 8) select
   full_zwindow <= '0' when X"02",
                   '1' when X"03",
                   'X' when others;
                      
                      
--------------------------------------------
-- Generate color_level_o :
--------------------------------------------
color_level_generation : process (clk_i, zreset_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if full_zwindow = '1' or (window_active_v = '1' and window_active_h = '1') then
            color_level_o <= pattern_i(4 downto 0);            
         else
            color_level_o <= "00001";
         end if;
      end if;
   end if;
end process;


--color_level_o <= pattern_i(4 downto 0);
                    

end behavioral;