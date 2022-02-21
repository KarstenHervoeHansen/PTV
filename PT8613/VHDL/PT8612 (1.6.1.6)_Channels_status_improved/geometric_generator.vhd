library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity geometric_generator is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      -- system parameters:
      asl_i                : in std_logic_vector(10 downto 0);
      pattern_i            : in std_logic_vector(15 downto 0);    -- "00": 100/0/100/0, "01": 100/0/75/0, "10": 75/0/75/0
      
      -- frame control signals:
      sav_tick_advanced_i  : in std_logic;

      -- color parameter outputs:
      color_presence_o     : out std_logic_vector(2 downto 0);
      color_level_o        : out std_logic_vector(4 downto 0)
   );
end geometric_generator;


architecture behavioral of geometric_generator is


type color_state_type is (c_start, white, yellow, cyan, green, magenta, red, blue, black, c_end);
signal color_state            : color_state_type;
                              
signal state_count            : std_logic_vector(10 downto 0);
                              
signal start_count            : std_logic_vector(10 downto 0);
signal bar_count              : std_logic_vector(10 downto 0);
signal white_count            : std_logic_vector(10 downto 0);
                              
signal start_color_presence   : std_logic_vector(2 downto 0);
signal start_color_level      : std_logic_vector(4 downto 0);

signal end_color_presence     : std_logic_vector(2 downto 0);
signal end_color_level        : std_logic_vector(4 downto 0);

signal colorbar_type          : std_logic_vector(1 downto 0);
                              
constant sav_offset           : integer := 6;  -- horizontal position adjustment


begin


--------------------------------------------
-- Generate active colorbar :
--------------------------------------------
colorbar_type_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      colorbar_type <= (others => '0');
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         colorbar_type <= (others => '0');
      elsif en_i = '1' then
         case pattern_i(1 downto 0) is
            when "00" =>
               colorbar_type <= "00";     -- 100/0/100/0
            when "01" =>
               colorbar_type <= "01";     -- 100/0/75/0
            when others =>
               colorbar_type <= "11";     -- 75/0/75/0
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- count selection and start/end colors :
---------------------------------------------------------------
state_count_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      start_count          <= (others => '0');
      bar_count            <= (others => '0');
      start_color_presence <= (others => '0');
      start_color_level    <= (others => '0');
      end_color_presence   <= (others => '0');
      end_color_level      <= (others => '0');
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         start_count          <= (others => '0');
         bar_count            <= (others => '0');
         start_color_presence <= (others => '0');
         start_color_level    <= (others => '0');
         end_color_presence   <= (others => '0');
         end_color_level      <= (others => '0');
      elsif en_i = '1' then
         case asl_i is
            when conv_std_logic_vector(720,11) =>   -- (525 and 625 lines)
               start_count          <= conv_std_logic_vector(16+sav_offset,11);     --16
               white_count          <= conv_std_logic_vector(86,11);                --86
               bar_count            <= conv_std_logic_vector(86,11);                --86
               start_color_presence <= "000";   --black
               start_color_level    <= "XXXXX";
               end_color_presence   <= "000";   --black
               end_color_level      <= "XXXXX";         
            when conv_std_logic_vector(1280,11) =>  -- (720 lines)
               start_count       <= conv_std_logic_vector(2+sav_offset,11);
               white_count       <= conv_std_logic_vector(158,11);
               bar_count         <= conv_std_logic_vector(160,11);
               start_color_presence <= "111";   --white
               if colorbar_type(1) = '1' then --white: 75%
                  start_color_level <= "10100";                  
               else                             --white: 100%
                  start_color_level <= "11001";                  
               end if;
               end_color_presence   <= "000";   --black
               end_color_level      <= "XXXXX";         
            when others =>                   -- (1080 lines)
               start_count       <= conv_std_logic_vector(2+sav_offset,11);
               white_count       <= conv_std_logic_vector(238,11);
               bar_count         <= conv_std_logic_vector(240,11);
               start_color_presence <= "111";   --white
               if colorbar_type(1) = '1' then   --white: 75%
                  start_color_level <= "10100";                  
               else                             --white: 100%
                  start_color_level <= "11001";                  
               end if;
               end_color_presence   <= "000";   --black
               end_color_level      <= "XXXXX";         
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Horizontal state counter :
---------------------------------------------------------------
horz_sm : process(zreset_i, clk_i)
begin
   if zreset_i = '0' then
      color_state <= c_end;
      state_count <= (others => '0');
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         color_state <= c_end;
         state_count <= (others => '0');
      elsif en_i = '1' then
         if sav_tick_advanced_i = '1' then
            state_count <= start_count;
            color_state <= c_start;
         else
            case color_state is
               when c_start =>
                  if state_count = "00000000001" then
                     state_count <= white_count;
                     color_state <= white;
                  else
                     state_count <= state_count - 1;
                  end if;
               when white =>
                  if state_count = "00000000001" then
                     state_count <= bar_count;
                     color_state <= yellow;
                  else
                     state_count <= state_count - 1;
                  end if;
               when yellow =>
                  if state_count = "00000000001" then
                     state_count <= bar_count;
                     color_state <= cyan;
                  else
                     state_count <= state_count - 1;
                  end if;
               when cyan =>
                  if state_count = "00000000001" then
                     state_count <= bar_count;
                     color_state <= green;
                  else
                     state_count <= state_count - 1;
                  end if;
               when green =>
                  if state_count = "00000000001" then
                     state_count <= bar_count;
                     color_state <= magenta;
                  else
                     state_count <= state_count - 1;
                  end if;
               when magenta =>
                  if state_count = "00000000001" then
                     state_count <= bar_count;
                     color_state <= red;
                  else
                     state_count <= state_count - 1;
                  end if;
               when red =>
                  if state_count = "00000000001" then
                     state_count <= bar_count;
                     color_state <= blue;
                  else
                     state_count <= state_count - 1;
                  end if;
               when blue =>
                  if state_count = "00000000001" then
                     state_count <= bar_count;
                     color_state <= black;
                  else
                     state_count <= state_count - 1;
                  end if;
               when black =>
                  if state_count = "00000000001" then
                     state_count <= (others => 'X');
                     color_state <= c_end;
                  else
                     state_count <= state_count - 1;
                  end if;
               when others =>    -- c_end
                  color_state <= c_end;
                  state_count <= "XXXXXXXXXXX";
            end case;
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate color presence :
---------------------------------------------------------------
color_presence_generation : process(zreset_i, clk_i)
begin
   if zreset_i = '0' then
      color_presence_o <= "000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         color_presence_o <= "000";
      elsif en_i = '1' then
         case color_state is
            when c_start =>
               color_presence_o <= start_color_presence;
            when white =>
               color_presence_o <= "111";
            when yellow =>
               color_presence_o <= "110";
            when cyan =>
               color_presence_o <= "011";
            when green =>
               color_presence_o <= "010";
            when magenta =>
               color_presence_o <= "101";
            when red =>
               color_presence_o <= "100";
            when blue =>
               color_presence_o <= "001";
            when black =>
               color_presence_o <= "000";
            when others =>    -- c_end
               color_presence_o <= end_color_presence;
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate color level :
---------------------------------------------------------------
color_level_generation : process(zreset_i, clk_i)
begin
   if zreset_i = '0' then
      color_level_o <= "XXXXX";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         color_level_o <= "XXXXX";
      elsif en_i = '1' then
         case color_state is
            when c_start =>
               color_level_o <= start_color_level;
            when white =>
               if colorbar_type(1) = '1' then   --white: 75%
                  color_level_o <= "10100";
               else                             --white: 100%
                  color_level_o <= "11001";
               end if;
            when c_end =>
               color_level_o <= end_color_level;
            when others =>
               if colorbar_type(0) = '1' then   --colors: 75%
                  color_level_o <= "10100";
               else                             --colors: 100%
                  color_level_o <= "11001";
               end if;
         end case;
      end if;
   end if;
end process;


end behavioral;