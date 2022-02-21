library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity colorbar_generator is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      -- system parameters:
      system_i             : in std_logic_vector(7 downto 0);
      pattern_i            : in std_logic_vector(15 downto 0);    -- "00": 100/0/100/0, "01": 100/0/75/0, "10": 75/0/75/0
      
      -- frame control signals:
      sav_tick_advanced_i  : in std_logic;

      -- color parameter outputs:
      color_presence_o     : out std_logic_vector(2 downto 0);
      red_color_presence_o : out std_logic_vector(2 downto 0);
      color_level_o        : out std_logic_vector(4 downto 0)
   );
end colorbar_generator;


architecture behavioral of colorbar_generator is


type color_state_type is (c_start, white, yellow, cyan, green, magenta, red, blue, black, c_end);
signal color_state               : color_state_type;
                                 
signal state_count               : std_logic_vector(10 downto 0);
                                 
signal start_count               : std_logic_vector(10 downto 0);
signal bar_count                 : std_logic_vector(10 downto 0);
signal white_count               : std_logic_vector(10 downto 0);
                                 
signal start_color_presence      : std_logic_vector(2 downto 0);
signal start_color_level         : std_logic_vector(4 downto 0);
signal color_color_level         : std_logic_vector(4 downto 0);

signal red_start_color_presence  : std_logic_vector(2 downto 0);
signal red_end_color_presence    : std_logic_vector(2 downto 0);

signal end_color_presence        : std_logic_vector(2 downto 0);
signal end_color_level           : std_logic_vector(4 downto 0);
                                 
signal colorbar_type             : std_logic_vector(1 downto 0);

signal spl_index                 : std_logic_vector(1 downto 0);

                                 
constant sav_offset              : integer := 6;  -- horizontal position adjustment


begin


----------------------------------------------
---- Generate active colorbar :
----------------------------------------------
--colorbar_type_generation : process (clk_i, zreset_i) begin
--   if zreset_i = '0' then
--      colorbar_type <= (others => '0');
--   elsif clk_i'event and clk_i = '1' then
--      if sync_reset_i = '1' then
--         colorbar_type <= (others => '0');
--      elsif en_i = '1' then
--         case pattern_i(1 downto 0) is
--            when "00" =>
--               colorbar_type <= "00";     -- 100/0/100/0
--            when "01" =>
--               colorbar_type <= "01";     -- 100/0/75/0
--            when others =>
--               colorbar_type <= "11";     -- 75/0/75/0
--         end case;
--      end if;
--   end if;
--end process;


---------------------------------------------------------------
-- Horizontal state counter :
---------------------------------------------------------------
horz_sm : process(zreset_i, clk_i)
begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
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
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         case color_state is
            when c_start =>
               color_presence_o     <= start_color_presence;
               red_color_presence_o <= red_start_color_presence;
            when white =>
               color_presence_o     <= "111";
               red_color_presence_o <= "100";
            when yellow =>
               color_presence_o     <= "110";
               red_color_presence_o <= "100";
            when cyan =>
               color_presence_o     <= "011";
               red_color_presence_o <= "100";
            when green =>
               color_presence_o     <= "010";
               red_color_presence_o <= "100";
            when magenta =>
               color_presence_o     <= "101";
               red_color_presence_o <= "100";
            when red =>
               color_presence_o     <= "100";
               red_color_presence_o <= "100";
            when blue =>
               color_presence_o     <= "001";
               red_color_presence_o <= "100";
            when black =>
               color_presence_o     <= "000";
               red_color_presence_o <= "100";
            when others =>    -- c_end
               color_presence_o     <= end_color_presence;
               red_color_presence_o <= red_end_color_presence;
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate color level :
---------------------------------------------------------------
color_level_generation : process(zreset_i, clk_i)
begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         case color_state is
            when c_start =>
               color_level_o <= start_color_level;
            when white =>
               color_level_o <= start_color_level;
            when c_end =>
               color_level_o <= end_color_level;
            when others =>  -- colors and black
               color_level_o <= color_color_level;
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Parameters :
---------------------------------------------------------------
end_color_presence         <= "000";      --black
end_color_level            <= "XXXXX";    --don't care

with pattern_i(1 downto 0) select
   start_color_level       <= "10000"     when "10",     -- 75/0/75/0  
                              "10101"     when others;   -- 100/0/X/0               

with pattern_i(1 downto 0) select
   color_color_level       <= "10101"     when "00",     -- 100/0/100/0
                              "10000"     when others;   -- X/0/75/0               
                             

---------------------------------------------------------------
-- samples per line dependent on system :
-- 720: "00", 1280: "01", 1920: "10"
---------------------------------------------------------------
with system_i select
   spl_index <=      "10" when X"A0",   -- 1080i/60
                     "10" when X"B0",   -- 1080i/59.94
                     "10" when X"82",   -- 1080p/30
                     "10" when X"92",   -- 1080p/29.97
                     "10" when X"83",   -- 1080p/25
                     "10" when X"84",   -- 1080p/24
                     "10" when X"94",   -- 1080p/23.98
                     "01" when X"86",   -- 720p/60
                     "01" when X"96",   -- 720p/59.94
                     "01" when X"87",   -- 720p/50
                     "01" when X"88",   -- 720p/30
                     "01" when X"98",   -- 720p/29.97
                     "01" when X"89",   -- 720p/25
                     "01" when X"8A",   -- 720p/24
                     "01" when X"9A",   -- 720p/23.98
                     "00" when X"60",   -- NTSC 525i/59.94
                     "00" when X"20",   -- PAL 625i/50
                     "10" when others;  -- 1080i/50

                     
---------------------------------------------------------------
-- Parameters dependent on spl_index :
---------------------------------------------------------------
with spl_index select
   start_count             <= conv_std_logic_vector(16+sav_offset,11)   when "00",     -- 720
                              conv_std_logic_vector(2+sav_offset,11)    when "01",     -- 1280
                              conv_std_logic_vector(2+sav_offset,11)    when others;   -- 1920

with spl_index select
   white_count             <= conv_std_logic_vector(86,11)              when "00",     -- 720
                              conv_std_logic_vector(158,11)             when "01",     -- 1280
                              conv_std_logic_vector(238,11)             when others;   -- 1920
                              
with spl_index select
   bar_count               <= conv_std_logic_vector(86,11)              when "00",     -- 720
                              conv_std_logic_vector(160,11)             when "01",     -- 1280
                              conv_std_logic_vector(240,11)             when others;   -- 1920
                                         
with spl_index select
   start_color_presence    <= "000"                                     when "00",     -- 720  (black)
                              "111"                                     when "01",     -- 1280 (white)
                              "111"                                     when others;   -- 1920 (white)

with spl_index select
   red_start_color_presence   <= "000"                                  when "00",     -- 720  (black)
                                 "100"                                  when "01",     -- 1280 (red)
                                 "100"                                  when others;   -- 1920 (red)
                              
red_end_color_presence <= red_start_color_presence;


---------------------------------------------------------------
-- count selection and start/end colors :
---------------------------------------------------------------
--state_count_generation : process (clk_i, zreset_i) begin
--   if clk_i'event and clk_i = '1' then
--      if en_i = '1' then
--         case asl_i is
--            when conv_std_logic_vector(720,11) =>   -- (525 and 625 lines)
--               --start_count                <= conv_std_logic_vector(16+sav_offset,11);     --16
----               white_count                <= conv_std_logic_vector(86,11);                --86
----               bar_count                  <= conv_std_logic_vector(86,11);                --86
----               start_color_presence       <= "000";   --black
----               start_color_level          <= "XXXXX";
----               end_color_presence         <= "000";   --black
----               end_color_level            <= "XXXXX";         
--               red_start_color_presence   <= "000";   --black
--               red_end_color_presence     <= "000";   --black
--            when conv_std_logic_vector(1280,11) =>  -- (720 lines)
--               --start_count                <= conv_std_logic_vector(2+sav_offset,11);
--  --             white_count                <= conv_std_logic_vector(158,11);
--  --             bar_count                  <= conv_std_logic_vector(160,11);
--  --             start_color_presence       <= "111";   --white
--    --           if colorbar_type(1) = '1' then --white: 75%
--  --                start_color_level <= "10000";                  
--      --         else                             --white: 100%
--        --          start_color_level <= "10110";                  
--          --     end if;
--  --             end_color_presence         <= "000";   --black
--  --             end_color_level            <= "XXXXX";         
--               red_start_color_presence   <= "100";   --black
--               red_end_color_presence     <= "100";   --black
--            when others =>                   -- (1080 lines)
--               --start_count                <= conv_std_logic_vector(2+sav_offset,11);
--    --           white_count                <= conv_std_logic_vector(238,11);
--    --           bar_count                  <= conv_std_logic_vector(240,11);
--    --           start_color_presence <= "111";   --white
--  --             if colorbar_type(1) = '1' then   --white: 75%
--    --              start_color_level <= "10000";                  
--      --         else                             --white: 100%
--        --          start_color_level <= "10110";                  
--          --     end if;
--    --           end_color_presence         <= "000";   --black
--    --           end_color_level            <= "XXXXX";         
--               red_start_color_presence   <= "100";   --black
--               red_end_color_presence     <= "100";   --black
--         end case;
--      end if;
--   end if;
--end process;


end Behavioral;