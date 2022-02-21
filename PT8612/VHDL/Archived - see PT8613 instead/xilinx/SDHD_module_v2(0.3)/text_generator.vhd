library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity text_generator is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      -- input parameters:
      system_i             : in std_logic_vector(7 downto 0);
      v_pos_i              : in std_logic_vector(3 downto 0);
      h_pos_i              : in std_logic_vector(3 downto 0);
      bmp_heigth_i         : in std_logic_vector(10 downto 0);    -- must be >= 1
      bmp_width_i          : in std_logic_vector(10 downto 0);    -- must be >= 1
      text_on_i            : in std_logic;
      text_color_presence_i: in std_logic_vector(2 downto 0);
      back_color_presence_i: in std_logic_vector(2 downto 0);
      text_size_i          : in std_logic_vector(1 downto 0);
      movement_i           : in std_logic_vector(1 downto 0);     -- "00": no movement, "01": vertical, "10": horizontal

      -- frame control signals:
      av_line_i            : in std_logic_vector(10 downto 0);
      av_sample_i          : in std_logic_vector(10 downto 0);
      fvh_i                : in std_logic_vector(2 downto 0);
      v_grid_state_i       : in std_logic_vector(3 downto 0);
      h_grid_state_i       : in std_logic_vector(3 downto 0);
      sav_advanced_i       : in std_logic;

      -- interface to text RAM:
      ram_data_i           : in std_logic_vector(7 downto 0);
      ram_addr_o           : out std_logic_vector(8 downto 0);
      ram_enable_o         : out std_logic;

      -- insert text control:
      insert_text_o        : out std_logic;
      color_presence_o     : out std_logic_vector(2 downto 0)
   );
end text_generator;


architecture behavioral of text_generator is

signal interlaced                      : std_logic;
                                       
signal bmp_bit_count                   : std_logic_vector(11 downto 0);
signal bmp_bit_count_store             : std_logic_vector(11 downto 0);
signal bit_pointer                     : std_logic_vector(2 downto 0);
signal interlaced_and_text_size_index  : std_logic_vector(2 downto 0);

signal pix_repeat_h_count              : std_logic_vector(4 downto 0);
signal pix_repeat_v_count              : std_logic_vector(4 downto 0);
signal pix_repeat_minus_1_h            : std_logic_vector(4 downto 0);
signal pix_repeat_minus_1_v            : std_logic_vector(4 downto 0);
signal interlaced_spl_size_index       : std_logic_vector(4 downto 0);
signal spl_size_index                  : std_logic_vector(3 downto 0);

signal v_delayed_1                     : std_logic;
signal field_tick                      : std_logic;
signal first_text_sample               : std_logic_vector(2 downto 0);
signal first_text_sample_passed        : std_logic;
signal first_text_line_passed          : std_logic;
signal first_text_line                 : std_logic_vector(2 downto 0);
signal bmp_h_count_was_one             : std_logic;
signal h_pix_tick                      : std_logic;
signal line_tick                       : std_logic;
signal pix_on                          : std_logic;
signal insert_text                     : std_logic_vector(5 downto 0);
                                       
signal bmp_v_count                     : std_logic_vector(10 downto 0);
signal bmp_h_count                     : std_logic_vector(10 downto 0);
signal pix_repeat_h_count_was_zero     : std_logic_vector(1 downto 0);
                                       
signal v_count_up_zdown                : std_logic;
signal h_count_up_zdown                : std_logic;
signal step_size                       : std_logic_vector(10 downto 0);
signal v_move_count                    : std_logic_vector(10 downto 0);
signal h_move_count                    : std_logic_vector(10 downto 0);
                                       
signal in_bmp_v                        : std_logic;
signal in_bmp_h                        : std_logic;
signal insert_text_h                   : std_logic;
signal last_pix_repeat_v               : std_logic;
signal bmp_speeding                    : std_logic;
signal h_count_up_zdown_next           : std_logic;
signal h_check_borders                 : std_logic;
signal text_on                         : std_logic;

signal spl_index                       : std_logic_vector(1 downto 0);


begin


--------------------------------------------
-- parameters :
--------------------------------------------
with system_i select
   interlaced <=           '1'   when X"A0",   -- 1080i/60
                           '1'   when X"B0",   -- 1080i/59.94
                           '0'   when X"82",   -- 1080p/30
                           '0'   when X"92",   -- 1080p/29.97
                           '0'   when X"83",   -- 1080p/25
                           '0'   when X"84",   -- 1080p/24
                           '0'   when X"94",   -- 1080p/23.98
                           '0'   when X"86",   -- 720p/60
                           '0'   when X"96",   -- 720p/59.94
                           '0'   when X"87",   -- 720p/50
                           '0'   when X"88",   -- 720p/30
                           '0'   when X"98",   -- 720p/29.97
                           '0'   when X"89",   -- 720p/25
                           '0'   when X"8A",   -- 720p/24
                           '0'   when X"9A",   -- 720p/23.98
                           '1'   when X"60",   -- NTSC 525i/59.94
                           '1'   when X"20",   -- PAL 625i/50
                           '1'   when others;  -- 1080i/50


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
-- vertical pixel repetition:
---------------------------------------------------------------
interlaced_spl_size_index <= interlaced & spl_index & text_size_i;

with interlaced_spl_size_index select
   pix_repeat_minus_1_v <= "00000" when "10000",      --  2/2-1 when i,  720, 2 x 2
                           "00001" when "10001",      --  4/2-1 when i,  720, 4 x 4
                           "00010" when "10010",      --  6/2-1 when i,  720, 6 x 6
                           "00011" when "10011",      --  8/2-1 when i,  720, 8 x 8
                                                                   
                           "00011" when "00100",      --    4-1 when p, 1280, 4 x 4
                           "00111" when "00101",      --    8-1 when p, 1280, 8 x 8
                           "01001" when "00110",      --   10-1 when p, 1280, 10 x 10
                           "01101" when "00111",      --   14-1 when p, 1280, 14 x 14
                                                                   
                           "00101" when "01000",      --    6-1 when p, 1920, 6 x 6  
                           "01001" when "01001",      --   10-1 when p, 1920, 10 x 10  
                           "01111" when "01010",      --   16-1 when p, 1920, 16 x 16
                           "10101" when "01011",      --   22-1 when p, 1920, 22 x 22
                                                         
                           "00100" when "11001",      -- 10/2-1 when i, 1920, 10 x 10  
                           "00111" when "11010",      -- 16/2-1 when i, 1920, 16 x 16
                           "01010" when "11011",      -- 22/2-1 when i, 1920, 22 x 22
                           "00010" when others;       --  6/2-1 when i, 1920, 6 x 6  


---------------------------------------------------------------
-- horizontal pixel repetition:
---------------------------------------------------------------
spl_size_index <= spl_index & text_size_i;

with spl_size_index select
   pix_repeat_minus_1_h <= "00001" when "0000",      --    2-1 when  720, 2 x 2
                           "00011" when "0001",      --    4-1 when  720, 4 x 4
                           "00101" when "0010",      --    6-1 when  720, 6 x 6
                           "00111" when "0011",      --    8-1 when  720, 8 x 8
                                                                  
                           "00011" when "0100",      --    4-1 when 1280, 4 x 4
                           "00111" when "0101",      --    8-1 when 1280, 8 x 8
                           "01001" when "0110",      --   10-1 when 1280, 10 x 10
                           "01101" when "0111",      --   14-1 when 1280, 14 x 14
                                                                  
                           "01001" when "1001",      --   10-1 when 1920, 10 x 10  
                           "01111" when "1010",      --   16-1 when 1920, 16 x 16
                           "10101" when "1011",      --   22-1 when 1920, 22 x 22
                           "00101" when others;      --    6-1 when 1920, 6 x 6  

                                                      
with system_i select
   step_size            <= conv_std_logic_vector(6,11)      when X"A0",    -- 1080i/60
                           conv_std_logic_vector(6,11)      when X"B0",    -- 1080i/59.94
                           conv_std_logic_vector(10,11)     when X"82",    -- 1080p/30
                           conv_std_logic_vector(10,11)     when X"92",    -- 1080p/29.97
                           conv_std_logic_vector(12,11)     when X"83",    -- 1080p/25
                           conv_std_logic_vector(14,11)     when X"84",    -- 1080p/24
                           conv_std_logic_vector(14,11)     when X"94",    -- 1080p/23.98
                           conv_std_logic_vector(4,11)      when X"86",    -- 720p/60
                           conv_std_logic_vector(4,11)      when X"96",    -- 720p/59.94
                           conv_std_logic_vector(4,11)      when X"87",    -- 720p/50
                           conv_std_logic_vector(8,11)      when X"88",    -- 720p/30
                           conv_std_logic_vector(8,11)      when X"98",    -- 720p/29.97
                           conv_std_logic_vector(8,11)      when X"89",    -- 720p/25
                           conv_std_logic_vector(10,11)     when X"8A",    -- 720p/24
                           conv_std_logic_vector(10,11)     when X"9A",    -- 720p/23.98
                           conv_std_logic_vector(2,11)      when X"60",    -- 525i/59.94
                           conv_std_logic_vector(2,11)      when X"20",    -- 625i/50
                           conv_std_logic_vector(6,11)      when others;   -- 1080i/50


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
         if v_delayed_1 = '0' and fvh_i(1) = '1' then  -- Rising v
            field_tick <= '1';
         else
            field_tick <= '0';
         end if;
      end if;
   end if;
end process;
                           
                           
--------------------------------------------
-- vertical direction state machine :
--------------------------------------------
v_move_direction_sm : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if v_count_up_zdown = '1' then
            if v_grid_state_i = "1000" and (insert_text(0) = '1' or first_text_line(0) = '1') then
               v_count_up_zdown <= '0';
            end if;
         else
            if v_grid_state_i = "0000" and (insert_text(0) = '1' or first_text_line(0) = '1') then
               v_count_up_zdown <= '1';
            end if;
         end if;
      end if;
   end if;
end process;


--------------------------------------------
-- vertical move count :
--------------------------------------------
v_move_count_geneation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         v_move_count <= conv_std_logic_vector(128,11);
      else
         if en_i = '1' then
            if movement_i(0) = '1' then   -- vertical movement
               if field_tick = '1' then
                  if v_count_up_zdown = '1' then   -- increase
                     v_move_count <= v_move_count + step_size;
                  else                             -- decrease
                     v_move_count <= v_move_count - step_size;
                  end if;
               end if;
            else
               v_move_count <= conv_std_logic_vector(128,11);
            end if;
         end if;
      end if;
   end if;
end process;


--------------------------------------------
-- horizontal direction state machine :
--------------------------------------------
h_move_direction_sm : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if h_check_borders = '1' then
            if h_count_up_zdown = '0' then   -- counting down:
               if h_grid_state_i = "0000" and first_text_sample(0) = '1' then
                  h_count_up_zdown_next  <= '1';
               end if;
            else                             -- counting up:
               if h_grid_state_i = "0000" and first_text_sample(0) = '1' then
                  h_count_up_zdown_next <= '1';
                  h_check_borders       <= '0';
               elsif h_grid_state_i = "1111" and in_bmp_h = '1' then
                  h_count_up_zdown_next <= '0';
               end if;
            end if;
         end if;
         if field_tick = '1' then
            h_count_up_zdown <= h_count_up_zdown_next;
            h_check_borders  <= '1';
         end if;
      end if;
   end if;
end process;


--------------------------------------------
-- horizontal move count :
--------------------------------------------
h_move_count_geneation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         h_move_count <= conv_std_logic_vector(128,11);
      else
         if en_i = '1' then
            if movement_i(1) = '1' then   -- horizontal movement
               if field_tick = '1' then
                  if h_count_up_zdown = '1' then   -- increase
                     h_move_count <= h_move_count + step_size;
                  else                             -- decrease
                     h_move_count <= h_move_count - step_size;
                  end if;
               end if;
            else
               h_move_count <= conv_std_logic_vector(128,11);
            end if;
         end if;
      end if;
   end if;
end process;


--------------------------------------------
-- indicate first text sample :
--------------------------------------------
first_text_sample_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if fvh_i(0) = '1' then
            first_text_sample(0)  <= '0';
            first_text_sample_passed        <= '0';
         elsif first_text_sample_passed = '0' then
            if movement_i(1) = '0' then   -- no horizontal movement
               if h_grid_state_i = h_pos_i then
                  first_text_sample(0)  <= '1';
                  first_text_sample_passed      <= '1';
               else
                  first_text_sample(0)  <= '0';
               end if;
            else                          -- horizontal movement
               if av_sample_i = h_move_count then
                  first_text_sample(0)  <= '1';
                  first_text_sample_passed      <= '1';
               else
                  first_text_sample(0)  <= '0';
               end if;
            end if;            
         else
            first_text_sample(0)  <= '0';
         end if;
         first_text_sample(2 downto 1) <= first_text_sample(1 downto 0);
      end if;
   end if;
end process;


--------------------------------------------
-- count pixel repetition samples :
--------------------------------------------
pixel_repeat_h_counting : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if first_text_sample(0) = '1' then
            pix_repeat_h_count <= pix_repeat_minus_1_h;
         else
            if pix_repeat_h_count = "00000" then
               pix_repeat_h_count <= pix_repeat_minus_1_h;
            else
               pix_repeat_h_count <= pix_repeat_h_count - 1;
            end if;
         end if;
      end if;
   end if;
end process;


pix_repeat_h_count_was_zero_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if pix_repeat_h_count = "00000" or bmp_speeding = '1' then
            pix_repeat_h_count_was_zero(0) <= '1';
         else
            pix_repeat_h_count_was_zero(0) <= '0';
         end if;
         pix_repeat_h_count_was_zero(1) <= pix_repeat_h_count_was_zero(0);
      end if;
   end if;
end process;


bmp_speeding_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then 
         if fvh_i(0) = '1' then
            bmp_speeding <= '1';
         elsif first_text_sample(0) = '1' then
            bmp_speeding <= '0';
         end if;
      end if;
   end if;
end process;



--------------------------------------------
-- Count BMP width :
--------------------------------------------
bmp_width_counting : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if first_text_sample(1) = '1' then
            bmp_h_count <= bmp_width_i;
         elsif pix_repeat_h_count_was_zero(0) = '1' then
            bmp_h_count <= bmp_h_count - 1;
         end if;
      end if;
   end if;
end process;


bmp_h_count_was_one_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if bmp_h_count = conv_std_logic_vector(1,11) then
            bmp_h_count_was_one <= '1';
         else
            bmp_h_count_was_one <= '0';
         end if;
      end if;
   end if;
end process;


in_bmp_h_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if first_text_sample(2) = '1' then
            in_bmp_h <= '1';
         elsif pix_repeat_h_count_was_zero(1) = '1' and bmp_h_count_was_one = '1' then
            in_bmp_h <= '0';
         end if;
      end if;
   end if;
end process;


--------------------------------------------
-- indicate first text line :
--------------------------------------------

line_tick <= first_text_sample(0);

first_text_line_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if line_tick = '1' then
            if fvh_i(1) = '1' then
               first_text_line(0)      <= '0';
               first_text_line_passed  <= '0';
            elsif first_text_line_passed = '0' then
               if movement_i(0) = '0' then   -- no vertical movement
                  if v_grid_state_i = v_pos_i then
                     first_text_line(0)      <= '1';
                     first_text_line_passed  <= '1';
                  else
                     first_text_line(0) <= '0';
                  end if;
               else                          -- vertical movement
                  if av_line_i(10 downto 1) = v_move_count(10 downto 1) then
                     first_text_line(0)      <= '1';
                     first_text_line_passed  <= '1';
                  else
                     first_text_line(0) <= '0';
                  end if;
               end if;            
            else
               first_text_line(0) <= '0';
            end if;
            first_text_line(2 downto 1) <= first_text_line(1 downto 0);
         end if;
      end if;
   end if;
end process;


--------------------------------------------
-- count pixel repetition lines :
--------------------------------------------
pixel_repeat_v_counting : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if line_tick = '1' then
            if first_text_line(0) = '1' then
               pix_repeat_v_count <= pix_repeat_minus_1_v;
            else
               if pix_repeat_v_count = "00000" then
                  pix_repeat_v_count <= pix_repeat_minus_1_v;
               else
                  pix_repeat_v_count <= pix_repeat_v_count - 1;
               end if;
            end if;
         end if;
      end if;
   end if;
end process;


last_pix_repeat_v_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if line_tick = '1' then
            if pix_repeat_v_count = "00000" then
               last_pix_repeat_v <= '1';
            else
               last_pix_repeat_v <= '0';
            end if;
         end if;
      end if;
   end if;
end process;


--------------------------------------------
-- Count BMP heigth :
--------------------------------------------
bmp_heigth_counting : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if line_tick = '1' then
            if first_text_line(1) = '1' then
               bmp_v_count <= bmp_heigth_i;
            elsif last_pix_repeat_v = '1' then
               bmp_v_count <= bmp_v_count - 1;
            end if;
         end if;
      end if;
   end if;
end process;


in_bmp_v_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if line_tick = '1' then
            if fvh_i(1) = '1' then
               in_bmp_v <= '0';
            elsif first_text_line(1) = '1' then
               in_bmp_v <= '1';
            elsif last_pix_repeat_v = '1' and bmp_v_count = conv_std_logic_vector(1,11) then
               in_bmp_v <= '0';
            end if;
         end if;
      end if;
   end if;
end process;


--------------------------------------------
-- BMP pixel tick generation :
--------------------------------------------
h_pix_tick_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if in_bmp_h = '1' and in_bmp_v = '1' then
            h_pix_tick <= pix_repeat_h_count_was_zero(1);
         else
            h_pix_tick <= '0';
         end if;
      end if;
   end if;
end process;


--------------------------------------------
-- Insert text generation :
--------------------------------------------
insert_text_h_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if fvh_i(0) = '1' then
            insert_text_h <= '0';
         elsif first_text_sample(2) = '1' then
            insert_text_h <= '1';
         elsif pix_repeat_h_count_was_zero(1) = '1' and bmp_h_count_was_one = '1' then
            insert_text_h <= '0';
         end if;
      end if;
   end if;
end process;


insert_text_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if insert_text_h = '1' and in_bmp_v = '1' and text_on = '1' then
            insert_text(0) <= '1';
         else
            insert_text(0) <= '0';
         end if;
         insert_text(5 downto 1) <= insert_text(4 downto 0);
      end if;
   end if;
end process;

insert_text_o <= insert_text(2);


--------------------------------------------
-- Set text off in heigth or width is zero :
--------------------------------------------
heigth_width_checking : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if (bmp_heigth_i = 0) or (bmp_width_i = 0) then
            text_on <= '0';
         else
            text_on <= text_on_i;
         end if;
      end if;
   end if;
end process;


--------------------------------------------
-- Count BMP bit :
--------------------------------------------
bmp_bit_counting : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if fvh_i(1) = '1' then
            bmp_bit_count        <= (others => '0'); --text_start_address & "000";
         elsif line_tick = '1' and last_pix_repeat_v = '0' then
            bmp_bit_count <= bmp_bit_count_store;
         elsif h_pix_tick = '1' then
            bmp_bit_count <= bmp_bit_count + 1;
         end if;
      end if;
   end if;
end process;


bmp_bit_counting_storing : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if line_tick = '1' then
            if last_pix_repeat_v = '1' then
               bmp_bit_count_store  <= bmp_bit_count;            
            end if;
         end if;
      end if;
   end if;
end process;


ram_addr_o     <= bmp_bit_count(11 downto 3);
ram_enable_o   <= en_i;


--------------------------------------------
-- Pick bit in byte :
--------------------------------------------
bit_picking : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         bit_pointer <= bmp_bit_count(2 downto 0);    -- delay to match delay in RAM
         case bit_pointer is
         when "111"  => pix_on <= ram_data_i(0);
         when "110"  => pix_on <= ram_data_i(1);
         when "101"  => pix_on <= ram_data_i(2);
         when "100"  => pix_on <= ram_data_i(3);
         when "011"  => pix_on <= ram_data_i(4);
         when "010"  => pix_on <= ram_data_i(5);
         when "001"  => pix_on <= ram_data_i(6);
         when others => pix_on <= ram_data_i(7);
         end case;
      end if;
   end if;
end process;


--------------------------------------------
-- Color presence generation :
--------------------------------------------
color_presence_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if pix_on = '1' then
            color_presence_o <= text_color_presence_i;
         else
            color_presence_o <= back_color_presence_i;
         end if;
      end if;
   end if;
end process;


end behavioral;