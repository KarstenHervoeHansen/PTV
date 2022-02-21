library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity line_controller is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      -- parameters:
      system_i             : in std_logic_vector(7 downto 0);
      pattern_i            : in std_logic_vector(15 downto 0);
--      interlaced_i         : in std_logic;

      -- frame control signals:
--      last_av_sample_i     : in std_logic;
      av_line_i            : in std_logic_vector(10 downto 0);
      fvh_i                : in std_logic_vector(2 downto 0);
      insert_text_i        : in std_logic;

      -- vertical state signals:
      lower_zupper_o       : out std_logic;

      -- Mux selection signals:
      index_mux_select_o   : out std_logic_vector(2 downto 0);
      unfilt_mux_select_o  : out std_logic;
      av_mux_select_o      : out std_logic
   );
end line_controller;


architecture behavioral of line_controller is


signal field_start               : std_logic;
signal frame_count               : std_logic;
signal interlaced                : std_logic;
signal hd_zsd_temp               : std_logic;
signal state_count               : std_logic_vector(9 downto 0);
--signal line_grid_count           : std_logic_vector(6 downto 0);
signal grid_size_minus_1         : std_logic_vector(6 downto 0);
signal half_field_count          : std_logic_vector(10 downto 0);
signal index_mux_select          : std_logic_vector(2 downto 0);

signal index_mux_select_combi    : std_logic_vector(2 downto 0);
signal unfilt_mux_select_combi   : std_logic;
signal index_mux_select_window   : std_logic_vector(2 downto 0);
signal window_active             : std_logic;
signal unfilt_mux_select         : std_logic;
signal av_mux_select             : std_logic;
signal lower_zupper              : std_logic;


signal red1_start                : std_logic_vector(10 downto 0);
signal clapper_start             : std_logic_vector(10 downto 0);
signal red2_start                : std_logic_vector(10 downto 0);

signal lpf_index                 : std_logic_vector(2 downto 0);


type combi_state_type is (colorbar, lumaramp, red1, clapperboard, red2);
signal combi_state         : combi_state_type;


begin


---------------------------------------------------------------
-- Combi state machine :
---------------------------------------------------------------
combi_state_machine : process (clk_i, zreset_i) begin
   if clk_i'event and clk_i = '1' then
      if lower_zupper = '0' then
         combi_state <= colorbar;
      else
         case combi_state is
         when colorbar =>
            if lower_zupper = '1' then
               combi_state <= lumaramp;
            end if;
         when lumaramp =>
            if av_line_i(10 downto 1) = red1_start(10 downto 1) then
               combi_state <= red1;
            end if;
         when red1 =>
            if av_line_i(10 downto 1) = clapper_start(10 downto 1) then
               combi_state <= clapperboard;
            end if;
         when clapperboard =>
            if av_line_i(10 downto 1) = red2_start(10 downto 1) then
               combi_state <= red2;
            end if;
         when others => -- red2
            combi_state <= red2;
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- combi index generation from state :
---------------------------------------------------------------
combi_index_generation : process (clk_i, zreset_i) begin
   if clk_i'event and clk_i = '1' then
      case combi_state is
      when colorbar =>
         index_mux_select_combi  <= "001";
         unfilt_mux_select_combi <= '0';
      when lumaramp =>
         index_mux_select_combi  <= "XXX";
         unfilt_mux_select_combi <= '1';
      when clapperboard =>
         index_mux_select_combi  <= "011";
         unfilt_mux_select_combi <= '0';
      when others => -- red1 and red2 :
         index_mux_select_combi  <= "010";
         unfilt_mux_select_combi <= '0';
      end case;
   end if;
end process;


---------------------------------------------------------------
-- select mux states :
---------------------------------------------------------------
mux_state_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      index_mux_select   <= "000";
      unfilt_mux_select  <= '0';
      av_mux_select      <= '0';
   elsif clk_i'event and clk_i = '1' then
      case pattern_i(15 downto 8) is
      when X"00" =>  -- Colorbar :
         index_mux_select   <= "001";
         unfilt_mux_select  <= '0';
         av_mux_select      <= '0';
      when X"05" =>  -- Check field :
         index_mux_select   <= "XXX";
         unfilt_mux_select  <= 'X';
         av_mux_select      <= '1';
      when X"01" =>  -- Combi :                    -- change dependent on line number
         index_mux_select   <= index_mux_select_combi;
         unfilt_mux_select  <= unfilt_mux_select_combi;
         av_mux_select      <= '0';
      when X"06" =>  -- Pluge :                    -- change dependent on line number
         index_mux_select   <= "101";
         unfilt_mux_select  <= '0';
         av_mux_select      <= '0';
      when X"07" =>  -- Luma Ramp :
         index_mux_select   <= "XXX";
         unfilt_mux_select  <= '1';
         av_mux_select      <= '0';
      when X"08" =>  -- Clapperboard :
         index_mux_select   <= "011";
         unfilt_mux_select  <= '0';
         av_mux_select      <= '0';
      when X"03" =>  -- Grey :
         index_mux_select   <= "100";
         unfilt_mux_select  <= '0';
         av_mux_select      <= '0';
      when X"02" =>  -- Grey window :
         index_mux_select   <= "100";
         unfilt_mux_select  <= '0';
         av_mux_select      <= '0';
      when X"04" =>  -- Crosshatch :
         index_mux_select   <= "111";
         unfilt_mux_select  <= '0';
         av_mux_select      <= '0';
      when others => -- Black :
         index_mux_select   <= "000";
         unfilt_mux_select  <= '0';
         av_mux_select      <= '0';
      end case;
   end if;
end process;


---------------------------------------------------------------
-- Upper / lower part of active video :
---------------------------------------------------------------
lower_upper_generation : process (clk_i, zreset_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         lower_zupper <= '0';
      elsif en_i = '1' then
         if fvh_i(1) = '1' then
            lower_zupper <= '0';
         elsif av_line_i(10 downto 1) = half_field_count(10 downto 1) then
            lower_zupper <= '1';
         end if;
      end if;
   end if;
end process;

lower_zupper_o <= lower_zupper;


-----------------------------------------------------------------
---- Line grid counter :
-----------------------------------------------------------------
--line_grid_counting : process (clk_i, zreset_i) begin
--   if clk_i'event and clk_i = '1' then
--      if en_i = '1' and last_av_sample_i = '1' then
--         if interlaced_i = '1' then          -- interlaced
--            if fvh_i(1) = '1' or line_grid_count(6 downto 1) = "000000" then
--               line_grid_count <= grid_size_minus_1(6 downto 1) & fvh_i(2);
--            else
--               line_grid_count <= line_grid_count - 2;
--            end if;
--         else                                -- progressive
--            if fvh_i(1) = '1' or line_grid_count = "0000000" then
--               line_grid_count <= grid_size_minus_1;
--            else
--               line_grid_count <= line_grid_count - 1;
--            end if;
--         end if;
--      end if;
--   end if;
--end process;


--------------------------------------------
-- Text mux :
--------------------------------------------
text_mux : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      index_mux_select_o   <= "000";
      unfilt_mux_select_o  <= '0';
      av_mux_select_o      <= '0';
   elsif clk_i'event and clk_i = '1' then
      if en_i = '1' then
         case insert_text_i is
            when '0' =>     -- no text :
               index_mux_select_o   <= index_mux_select;
               unfilt_mux_select_o  <= unfilt_mux_select;
               av_mux_select_o      <= av_mux_select;
            when others =>  -- insert text :
               index_mux_select_o   <= "110";
               unfilt_mux_select_o  <= '0';
               av_mux_select_o      <= '0';
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- lines per frame dependent on system :
-- 487: "000", 576: "001", 720: "010", 1035: "011", 1080: "100"
---------------------------------------------------------------
with system_i select
   lpf_index <= "100" when X"A0",   -- 1080i/60
                "100" when X"B0",   -- 1080i/59.94
                "100" when X"A1",   -- 1080i/50
                "100" when X"82",   -- 1080p/30
                "100" when X"92",   -- 1080p/29.97
                "100" when X"83",   -- 1080p/25
                "100" when X"84",   -- 1080p/24
                "100" when X"94",   -- 1080p/23.98
                "010" when X"86",   -- 720p/60
                "010" when X"96",   -- 720p/59.94
                "010" when X"87",   -- 720p/50
                "010" when X"88",   -- 720p/30
                "010" when X"98",   -- 720p/29.97
                "010" when X"89",   -- 720p/25
                "010" when X"8A",   -- 720p/24
                "010" when X"9A",   -- 720p/23.98
                "000" when X"60",   -- NTSC 525i/59.94
                "001" when X"20",   -- PAL 625i/50
                "100" when others;


---------------------------------------------------------------
-- Parameters dependent on lpf_index :
---------------------------------------------------------------
with lpf_index select
   half_field_count  <= conv_std_logic_vector(244,11)          when "000",   -- 487
                        conv_std_logic_vector(288,11)          when "001",   -- 576
                        conv_std_logic_vector(360,11)          when "010",   -- 720
                        conv_std_logic_vector(540,11)          when others;  -- 1080

with lpf_index select
   red1_start        <= conv_std_logic_vector(326,11)          when "000",   -- 487
                        conv_std_logic_vector(576*4/6,11)      when "001",   -- 576
                        conv_std_logic_vector(720*4/6,11)      when "010",   -- 720
                        conv_std_logic_vector(1080*4/6,11)     when others;  -- 1080

with lpf_index select
   clapper_start     <= conv_std_logic_vector(336,11)          when "000",   -- 487
                        conv_std_logic_vector(396,11)          when "001",   -- 576
                        conv_std_logic_vector(720*31/45,11)    when "010",   -- 720
                        conv_std_logic_vector(1080*31/45,11)   when others;  -- 1080

with lpf_index select
   red2_start        <= conv_std_logic_vector(478,11)          when "000",   -- 487
                        conv_std_logic_vector(564,11)          when "001",   -- 576
                        conv_std_logic_vector(720*44/45,11)    when "010",   -- 720
                        conv_std_logic_vector(1080*44/45,11)   when others;  -- 1080
                      
with lpf_index select
   grid_size_minus_1 <= conv_std_logic_vector(54-1,11)           when "000",   -- 487     -- grid_size must be even!
                        conv_std_logic_vector(64-1,11)           when "001",   -- 576
                        conv_std_logic_vector(80-1,11)           when "010",   -- 720
                        conv_std_logic_vector(120-1,11)          when others;  -- 1080

                        
end behavioral;