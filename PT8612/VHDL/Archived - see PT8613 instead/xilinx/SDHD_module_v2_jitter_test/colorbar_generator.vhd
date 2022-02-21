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
      asl_i                : in std_logic_vector(10 downto 0);
      colorbar_type_i      : in std_logic_vector(1 downto 0);    -- "00": 100/0/100/0, "01": 100/0/75/0, "10": 75/0/75/0
      
      -- frame control signals:
      sav_tick_advanced_i  : in std_logic;

      -- color parameter outputs:
      color_system_o       : out std_logic_vector(1 downto 0);
      color_index_o        : out std_logic_vector(3 downto 0)
   );
end colorbar_generator;


architecture behavioral of colorbar_generator is


type color_state_type is (c_start, white, yellow, cyan, green, magenta, red, blue, black, c_end);
signal color_state         : color_state_type;

signal state_count         : std_logic_vector(10 downto 0);

signal start_count         : std_logic_vector(10 downto 0);
signal bar_count           : std_logic_vector(10 downto 0);
signal white_count           : std_logic_vector(10 downto 0);

signal start_color_index   : std_logic_vector(3 downto 0);
signal end_color_index     : std_logic_vector(3 downto 0);


constant sav_offset        : integer := 7;  -- horizontal position adjustment


begin


---------------------------------------------------------------
-- count selection and start/end colors :
---------------------------------------------------------------
state_count_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      start_count       <= (others => '0');
      bar_count         <= (others => '0');
      start_color_index <= (others => '0');
      end_color_index   <= (others => '0');
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         start_count       <= (others => '0');
         bar_count         <= (others => '0');
         start_color_index <= (others => '0');
         end_color_index   <= (others => '0');
      elsif en_i = '1' then
         case asl_i is
            when conv_std_logic_vector(720,11) =>   -- (525 and 625 lines)
               start_count       <= conv_std_logic_vector(16+sav_offset,11);     --16
               white_count       <= conv_std_logic_vector(86,11);                --86
               bar_count         <= conv_std_logic_vector(86,11);                --86
               start_color_index <= "0000";  -- Black
               end_color_index   <= "0000";  -- Black
            when conv_std_logic_vector(1280,11) =>  -- (720 lines)
               start_count       <= conv_std_logic_vector(2+sav_offset,11);
               white_count       <= conv_std_logic_vector(158,11);
               bar_count         <= conv_std_logic_vector(160,11);
               start_color_index <= "111" & colorbar_type_i(1);  -- 100/75% White
               end_color_index   <= "0000";  -- Black
            when others =>                   -- (1080 lines)
               start_count       <= conv_std_logic_vector(2+sav_offset,11);
               white_count       <= conv_std_logic_vector(238,11);
               bar_count         <= conv_std_logic_vector(240,11);
               start_color_index <= "111" & colorbar_type_i(1);  -- 100/75% White
               end_color_index   <= "0000";  -- Black
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
-- Generate color index :
---------------------------------------------------------------
color_index_generation : process(zreset_i, clk_i)
begin
   if zreset_i = '0' then
      color_index_o <= "0000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         color_index_o <= "0000";
      elsif en_i = '1' then
         case color_state is
            when c_start =>
               color_index_o <= start_color_index;
            when white =>
               color_index_o <= "111" & colorbar_type_i(1);
            when yellow =>
               color_index_o <= "110" & colorbar_type_i(0);
            when cyan =>
               color_index_o <= "011" & colorbar_type_i(0);
            when green =>
               color_index_o <= "010" & colorbar_type_i(0);
            when magenta =>
               color_index_o <= "101" & colorbar_type_i(0);
            when red =>
               color_index_o <= "100" & colorbar_type_i(0);
            when blue =>
               color_index_o <= "001" & colorbar_type_i(0);
            when black =>
               color_index_o <= "000" & colorbar_type_i(0);
            when others =>    -- c_end
               color_index_o <= end_color_index;
         end case;
      end if;
   end if;
end process;


end behavioral;