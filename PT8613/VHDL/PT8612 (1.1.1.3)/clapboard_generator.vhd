library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity clapboard_generator is
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      -- system parameters:
      system_i             : in std_logic_vector(7 downto 0);

      -- frame control signals:
      sav_tick_advanced_i  : in std_logic;
      fvh_i                : in std_logic_vector(2 downto 0);

      -- Audio syncronization :
      max_open_field_o     : out std_logic;

      -- Colors:
      color_presence_o     : out std_logic_vector(2 downto 0);
      color_level_o        : out std_logic_vector(4 downto 0)
   );
end clapboard_generator;


architecture behavioral of clapboard_generator is

type cb_state_type is (red1, black1, red_white, black2, red2);
signal cb_state                        : cb_state_type;

signal v_delayed_1                     : std_logic;
signal field_tick                      : std_logic;
signal count_up_zdown                  : std_logic;

signal period_div_2                    : std_logic_vector(5 downto 0);
signal field_count                     : std_logic_vector(5 downto 0);
signal step_size                       : std_logic_vector(10 downto 0);

signal state_count                     : std_logic_vector(10 downto 0);
signal width_white                     : std_logic_vector(10 downto 0);
signal width_black                     : std_logic_vector(10 downto 0);
signal width_black_start               : std_logic_vector(10 downto 0);

signal width_red                       : std_logic_vector(10 downto 0);
signal width_black_check               : std_logic_vector(10 downto 0);
signal width_red_white                 : std_logic_vector(10 downto 0);
signal width_black_tmp                 : std_logic_vector(10 downto 0);
signal width_red_white_tmp             : std_logic_vector(10 downto 0);
signal bars_hit                        : std_logic;
signal progressive_picture             : std_logic;
signal progressive_transport           : std_logic;
signal progressive_picture_transport   : std_logic_vector(1 downto 0);

signal spl_index                       : std_logic_vector(1 downto 0);

constant sav_offset                    : integer := 6;  -- horizontal position adjustment


begin


---------------------------------------------------------------
-- Horizontal state counter :
---------------------------------------------------------------
horz_sm : process(clk_i)
begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if sav_tick_advanced_i = '1' then
            state_count <= width_red;
            cb_state    <= red1;
         else
            case cb_state is
               when red1 =>
                  if state_count = "00000000001" then
                     state_count <= width_black_check;
                     cb_state    <= black1;
                  else
                     state_count <= state_count - 1;
                  end if;
               when black1 =>
                  if state_count = "00000000001" then
                     state_count <= width_red_white;
                     cb_state    <= red_white;
                  else
                     state_count <= state_count - 1;
                  end if;
               when red_white =>
                  if state_count = "00000000001" then
                     state_count <= width_black_check;
                     cb_state    <= black2;
                  else
                     state_count <= state_count - 1;
                  end if;
               when black2 =>
                  if state_count = "00000000001" then
                     state_count <= width_red;
                     cb_state    <= red2;
                  else
                     state_count <= state_count - 1;
                  end if;
               when others => -- red2
                  cb_state       <= red2;
                  state_count <= (others => 'X');
            end case;
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate color presence :
---------------------------------------------------------------
color_presence_level : process(clk_i)
begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         case cb_state is
            when red1 =>
               color_presence_o  <= "100";
               color_level_o     <= "10000";        --75%
            when black1 =>
               if bars_hit = '0' then
                  color_presence_o  <= "000";
                  color_level_o     <= "XXXXX";
               else
                  color_presence_o  <= "111";
                  color_level_o     <= "10101";     --100%
               end if;
            when red_white =>
               if bars_hit = '0' then
                  color_presence_o  <= "100";
                  color_level_o     <= "10000";
               else
                  color_presence_o  <= "111";
                  color_level_o     <= "10101";     --100%
               end if;
            when black2 =>
               if bars_hit = '0' then
                  color_presence_o  <= "000";
                  color_level_o     <= "XXXXX";
               else
                  color_presence_o  <= "111";
                  color_level_o     <= "10101";     --100%
               end if;
            when others => -- red2
               color_presence_o  <= "100";
               color_level_o     <= "10000";        --75%
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- field tick generation (on rising edge of v):
---------------------------------------------------------------
field_tick_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         v_delayed_1       <= '1';
         field_tick        <= '0';
      elsif en_i = '1' then
         v_delayed_1 <= fvh_i(1);
         case progressive_picture_transport is
         when "00"   => -- interlaced picture, interlaced transport (i systems):
            if v_delayed_1 = '0' and fvh_i(1) = '1' then  -- Rising v
               field_tick <= '1';
            else
               field_tick <= '0';
            end if;
         when "01"   => -- interlaced picture, progressive transport (never happens):
            field_tick <= 'X';
         when "10"   => -- progressive picture, interlaced transport (sF systems):
            if v_delayed_1 = '0' and fvh_i(1) = '1' and fvh_i(2) = '1' then  -- Rising v, field 2
               field_tick <= '1';
            else
               field_tick <= '0';
            end if;
         when others => -- progressive picture, progressive transport (p systems):
            if v_delayed_1 = '0' and fvh_i(1) = '1' then  -- Rising v
               field_tick <= '1';
            else
               field_tick <= '0';
            end if;
         end case;
      end if;
   end if;
end process;

progressive_picture_transport <= progressive_picture & progressive_transport;

---------------------------------------------------------------
-- circular field counter with a period equal
-- to half of clapboard sequence (approx 1 s).
-- Also generate count_up_zdown signal:
---------------------------------------------------------------
field_counter : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         field_count <= period_div_2;
         count_up_zdown <= '1';
      elsif en_i = '1' then
         if field_tick = '1' then
            if field_count = conv_std_logic_vector(1,6) then
               field_count <= period_div_2;
               count_up_zdown <= not count_up_zdown;
            else
               field_count <= field_count - 1;
            end if;
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate max_open_field_o:
---------------------------------------------------------------
max_open_field_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         max_open_field_o <= '0';
      elsif en_i = '1' then
         if field_tick = '1' then
            if field_count = conv_std_logic_vector(1,6) and count_up_zdown = '1' then  --if field_count = period_div_2 and count_up_zdown = '0' then
               max_open_field_o <= '1';
            else
               max_open_field_o <= '0';
            end if;
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate width of red/white and black bars by counting up and down
-- with increments/decrements of step_size.
-- Black bars are decreasing on count_up_zdown = '1'
---------------------------------------------------------------
bar_width_counter : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         width_red_white_tmp  <= (others => '0');
         width_black_tmp      <= width_black_start;
      elsif en_i = '1' then
         if field_tick = '1' then
            if field_count = conv_std_logic_vector(1,6) and count_up_zdown = '0' then
               width_red_white_tmp  <= (others => '0');
               width_black_tmp      <= width_black_start;
            else
               if count_up_zdown = '1' then
                  width_red_white_tmp  <= width_red_white_tmp + (step_size(9 downto 0) & '0');
                  width_black_tmp      <= width_black_tmp - step_size;
               else
                  width_red_white_tmp  <= width_red_white_tmp - (step_size(9 downto 0) & '0');
                  width_black_tmp      <= width_black_tmp + step_size;
               end if;
            end if;
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Exception when bars hits. And mapping to outputs
-- Widths are changed and color on center bar is changed to white
---------------------------------------------------------------
bar_width_exception : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if width_red_white_tmp = 0 then  -- bars hit
            width_black_check    <= width_black;
            width_red_white      <= width_white;
            bars_hit             <= '1';
         else
            width_black_check    <= width_black_tmp;
            width_red_white      <= width_red_white_tmp;
            bars_hit             <= '0';
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Parameters dependent on system :
-- (See seperate spreadsheet)
---------------------------------------------------------------

with system_i select
   progressive_picture     <= '0' when X"A0",   -- 1080i/30
                              '0' when X"B0",   -- 1080i/29.97
                              '1' when X"82",   -- 1080p/30
                              '1' when X"92",   -- 1080p/29.97
                              '1' when X"83",   -- 1080p/25
                              '1' when X"84",   -- 1080p/24
                              '1' when X"94",   -- 1080p/23.98
                              '1' when X"A2",   -- 1080sF/30
                              '1' when X"B2",   -- 1080sF/29.97
                              '1' when X"A3",   -- 1080sF/25
                              '1' when X"A4",   -- 1080sF/24
                              '1' when X"B4",   -- 1080sF/23.98
                              '1' when X"86",   -- 720p/60
                              '1' when X"96",   -- 720p/59.94
                              '1' when X"87",   -- 720p/50
                              '1' when X"88",   -- 720p/30
                              '1' when X"98",   -- 720p/29.97
                              '1' when X"89",   -- 720p/25
                              '1' when X"8A",   -- 720p/24
                              '1' when X"9A",   -- 720p/23.98
                              '0' when X"60",   -- NTSC 525i/29.97
                              '0' when X"20",   -- PAL 625i/25
                              '0' when others;  -- 1080i/25

with system_i select
   progressive_transport   <= '0' when X"A0",   -- 1080i/30
                              '0' when X"B0",   -- 1080i/29.97
                              '1' when X"82",   -- 1080p/30
                              '1' when X"92",   -- 1080p/29.97
                              '1' when X"83",   -- 1080p/25
                              '1' when X"84",   -- 1080p/24
                              '1' when X"94",   -- 1080p/23.98
                              '0' when X"A2",   -- 1080sF/30
                              '0' when X"B2",   -- 1080sF/29.97
                              '0' when X"A3",   -- 1080sF/25
                              '0' when X"A4",   -- 1080sF/24
                              '0' when X"B4",   -- 1080sF/23.98
                              '1' when X"86",   -- 720p/60
                              '1' when X"96",   -- 720p/59.94
                              '1' when X"87",   -- 720p/50
                              '1' when X"88",   -- 720p/30
                              '1' when X"98",   -- 720p/29.97
                              '1' when X"89",   -- 720p/25
                              '1' when X"8A",   -- 720p/24
                              '1' when X"9A",   -- 720p/23.98
                              '0' when X"60",   -- NTSC 525i/29.97
                              '0' when X"20",   -- PAL 625i/25
                              '0' when others;  -- 1080i/25


---------------------------------------------------------------
-- samples per line dependent on system :
-- 720: "00", 1280: "01", 1920: "10"
---------------------------------------------------------------
with system_i select
   spl_index <=      "10" when X"A0",   -- 1080i/30
                     "10" when X"B0",   -- 1080i/29.97
                     "10" when X"82",   -- 1080p/30
                     "10" when X"92",   -- 1080p/29.97
                     "10" when X"83",   -- 1080p/25
                     "10" when X"84",   -- 1080p/24
                     "10" when X"94",   -- 1080p/23.98
                     "10" when X"A2",   -- 1080sF/30
                     "10" when X"B2",   -- 1080sF/29.97
                     "10" when X"A3",   -- 1080sF/25
                     "10" when X"A4",   -- 1080sF/24
                     "10" when X"B4",   -- 1080sF/23.98
                     "01" when X"86",   -- 720p/60
                     "01" when X"96",   -- 720p/59.94
                     "01" when X"87",   -- 720p/50
                     "01" when X"88",   -- 720p/30
                     "01" when X"98",   -- 720p/29.97
                     "01" when X"89",   -- 720p/25
                     "01" when X"8A",   -- 720p/24
                     "01" when X"9A",   -- 720p/23.98
                     "00" when X"60",   -- NTSC 525i/29.97
                     "00" when X"20",   -- PAL 625i/25
                     "10" when others;  -- 1080i/25   X"A1"


---------------------------------------------------------------
-- Parameters dependent on spl_index :
---------------------------------------------------------------
with spl_index select
   width_red            <= conv_std_logic_vector(9+16+sav_offset,11) when "00",     -- 720
                           conv_std_logic_vector(16+2+sav_offset,11) when "01",     -- 1280
                           conv_std_logic_vector(24+2+sav_offset,11) when others;   -- 1920

with spl_index select
   width_white          <= conv_std_logic_vector(2,11)               when "00",     -- 720
                           conv_std_logic_vector(4,11)               when "01",     -- 1280
                           conv_std_logic_vector(6,11)               when others;   -- 1920

with spl_index select
   width_black_start    <= conv_std_logic_vector(335,11)             when "00",     -- 720
                           conv_std_logic_vector(622,11)             when "01",     -- 1280
                           conv_std_logic_vector(934,11)             when others;   -- 1920

with spl_index select
   width_black          <= conv_std_logic_vector(334,11)             when "00",     -- 720
                           conv_std_logic_vector(620,11)             when "01",     -- 1280
                           conv_std_logic_vector(931,11)             when others;   -- 1920

with system_i select
   period_div_2         <= conv_std_logic_vector(60,6)               when X"A0",    -- 1080i/30
                           conv_std_logic_vector(60,6)               when X"B0",    -- 1080i/29.97
                           conv_std_logic_vector(30,6)               when X"82",    -- 1080p/30
                           conv_std_logic_vector(30,6)               when X"92",    -- 1080p/29.97
                           conv_std_logic_vector(25,6)               when X"83",    -- 1080p/25
                           conv_std_logic_vector(24,6)               when X"84",    -- 1080p/24
                           conv_std_logic_vector(24,6)               when X"94",    -- 1080p/23.98
                           conv_std_logic_vector(30,6)               when X"A2",    -- 1080sF/30
                           conv_std_logic_vector(30,6)               when X"B2",    -- 1080sF/29.97
                           conv_std_logic_vector(25,6)               when X"A3",    -- 1080sF/25
                           conv_std_logic_vector(24,6)               when X"A4",    -- 1080sF/24
                           conv_std_logic_vector(24,6)               when X"B4",    -- 1080sF/23.98
                           conv_std_logic_vector(60,6)               when X"86",    -- 720p/60
                           conv_std_logic_vector(60,6)               when X"96",    -- 720p/59.94
                           conv_std_logic_vector(50,6)               when X"87",    -- 720p/50
                           conv_std_logic_vector(30,6)               when X"88",    -- 720p/30
                           conv_std_logic_vector(30,6)               when X"98",    -- 720p/29.97
                           conv_std_logic_vector(25,6)               when X"89",    -- 720p/25
                           conv_std_logic_vector(24,6)               when X"8A",    -- 720p/24
                           conv_std_logic_vector(24,6)               when X"9A",    -- 720p/23.98
                           conv_std_logic_vector(60,6)               when X"60",    -- 525i/29.97
                           conv_std_logic_vector(50,6)               when X"20",    -- 625i/25
                           conv_std_logic_vector(50,6)               when others;   -- 1080i/25

with system_i select
   step_size            <= conv_std_logic_vector(15,11)              when X"A0",    -- 1080i/60
                           conv_std_logic_vector(15,11)              when X"B0",    -- 1080i/59.94
                           conv_std_logic_vector(30,11)              when X"82",    -- 1080p/30
                           conv_std_logic_vector(30,11)              when X"92",    -- 1080p/29.97
                           conv_std_logic_vector(36,11)              when X"83",    -- 1080p/25
                           conv_std_logic_vector(37,11)              when X"84",    -- 1080p/24
                           conv_std_logic_vector(37,11)              when X"94",    -- 1080p/23.98
                           conv_std_logic_vector(30,11)              when X"A2",    -- 1080sF/30
                           conv_std_logic_vector(30,11)              when X"B2",    -- 1080sF/29.97
                           conv_std_logic_vector(36,11)              when X"A3",    -- 1080sF/25
                           conv_std_logic_vector(37,11)              when X"A4",    -- 1080sF/24
                           conv_std_logic_vector(37,11)              when X"B4",    -- 1080sF/23.98
                           conv_std_logic_vector(10,11)              when X"86",    -- 720p/60
                           conv_std_logic_vector(10,11)              when X"96",    -- 720p/59.94
                           conv_std_logic_vector(12,11)              when X"87",    -- 720p/50
                           conv_std_logic_vector(20,11)              when X"88",    -- 720p/30
                           conv_std_logic_vector(20,11)              when X"98",    -- 720p/29.97
                           conv_std_logic_vector(24,11)              when X"89",    -- 720p/25
                           conv_std_logic_vector(25,11)              when X"8A",    -- 720p/24
                           conv_std_logic_vector(25,11)              when X"9A",    -- 720p/23.98
                           conv_std_logic_vector(5,11)               when X"60",    -- 525i/59.94
                           conv_std_logic_vector(6,11)               when X"20",    -- 625i/50
                           conv_std_logic_vector(18,11)              when others;   -- 1080i/50


end behavioral;