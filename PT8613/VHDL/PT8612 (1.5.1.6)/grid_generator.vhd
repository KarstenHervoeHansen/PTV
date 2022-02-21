library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity grid_generator is
   port (
      clk_i                   : in std_logic;
      en_i                    : in std_logic;
                              
      -- parameters:          
      system_i                : in std_logic_vector(7 downto 0);
      interlaced_transport_i  : in std_logic;

      -- frame control signals:
      sav_tick_advanced_i     : in std_logic;
      last_av_sample_i        : in std_logic;
      fvh_i                   : in std_logic_vector(2 downto 0);
                              
      -- output signals:      
      color_level_o           : out std_logic_vector(4 downto 0);
      v_grid_state_o          : out std_logic_vector(3 downto 0);
      h_grid_state_o          : out std_logic_vector(3 downto 0)
   );
end grid_generator;


architecture behavioral of grid_generator is


signal interlaced                : std_logic;
signal hd_zsd_temp               : std_logic;
signal v_grid_tick               : std_logic;
signal h_grid_tick               : std_logic;
signal h_grid_tick_delayed       : std_logic_vector(1 downto 0);
signal h_grid_line_on            : std_logic;
signal v_grid_line_on            : std_logic;
signal v_grid_state              : std_logic_vector(3 downto 0);
signal h_grid_state              : std_logic_vector(3 downto 0);

signal v_grid_count              : std_logic_vector(6 downto 0);
signal h_grid_count              : std_logic_vector(6 downto 0);
signal v_grid_size_minus_1       : std_logic_vector(6 downto 0);
signal h_grid_size_minus_1       : std_logic_vector(6 downto 0);
signal lpf_index                 : std_logic_vector(2 downto 0);
signal spl_index                 : std_logic_vector(1 downto 0);


constant h_grid_offset           : std_logic_vector(6 downto 0) := "1111110";


begin


---------------------------------------------------------------
-- Line grid counter :
---------------------------------------------------------------
v_grid_counting : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' and last_av_sample_i = '1' then
         if interlaced_transport_i = '1' then          -- interlaced
            if fvh_i(1) = '1' or v_grid_count(6 downto 1) = "000000" then
               v_grid_count <= v_grid_size_minus_1(6 downto 1) & not fvh_i(2);
            else
               v_grid_count(6 downto 1) <= v_grid_count(6 downto 1) - 1;
            end if;
         else                                -- progressive
            if fvh_i(1) = '1' or v_grid_count = "0000000" then
               v_grid_count <= v_grid_size_minus_1;
            else
               v_grid_count <= v_grid_count - 1;
            end if;
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Indicate if grid line :
---------------------------------------------------------------
v_grid_line_on <= '1' when v_grid_count = "0000000" else
                  '1' when v_grid_count = v_grid_size_minus_1 else
                  '0';


---------------------------------------------------------------
-- Generate v_grid_tick :
---------------------------------------------------------------
v_grid_tick_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if last_av_sample_i = '1' then
            if interlaced_transport_i = '1' then          -- interlaced
               if v_grid_count(6 downto 1) = "000000" then
                  v_grid_tick <= '1';
               else
                  v_grid_tick <= '0';
               end if;
            else                                -- progressive
               if v_grid_count = "0000000" then
                  v_grid_tick <= '1';
               else
                  v_grid_tick <= '0';
               end if;
            end if;
         else
            v_grid_tick <= '0';
         end if;
      end if;
   end if;
end process;
                  
                  
---------------------------------------------------------------
-- Generate v_grid_state:
---------------------------------------------------------------
v_grid_state_counting : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if fvh_i(1) = '1' then
            v_grid_state <= (others => '0');
         else
            if v_grid_tick = '1' then
               v_grid_state <= v_grid_state + 1;
            end if;
         end if;
      end if;
   end if;
end process;

v_grid_state_o <= v_grid_state;


---------------------------------------------------------------
-- Sample grid counter :
---------------------------------------------------------------
h_grid_counting : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if sav_tick_advanced_i = '1' then
            h_grid_count <= h_grid_offset;
         elsif h_grid_count = h_grid_size_minus_1 then
            h_grid_count <= conv_std_logic_vector(0,7);
         else
            h_grid_count <= h_grid_count + 1;
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Sample grid tick :
---------------------------------------------------------------
h_grid_tick_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if h_grid_count = "0000000" then
            h_grid_tick <= '1';
         else
            h_grid_tick <= '0';
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate h_grid_state_o:
---------------------------------------------------------------
h_grid_state_counting : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if h_grid_tick_delayed(1) = '1' then
            if fvh_i(0) = '1' then
               h_grid_state <= (others => '0');
            else
               h_grid_state <= h_grid_state + 1;
            end if;
         end if;
      end if;
   end if;
end process;

h_grid_state_o <= h_grid_state;


---------------------------------------------------------------
-- Generate h_grid_line_on:
---------------------------------------------------------------
h_grid_line_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if h_grid_tick_delayed(0) = '1' or h_grid_tick_delayed(1) = '1' then
            h_grid_line_on <= '1';
         else
            h_grid_line_on <= '0';
         end if;
         h_grid_tick_delayed <= h_grid_tick_delayed(0) & h_grid_tick;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate color_level_o:
---------------------------------------------------------------
grid_line_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if h_grid_line_on = '1' or v_grid_line_on = '1' then
            color_level_o <= "10101";     -- 100%
         else   
            color_level_o <= "00001";     -- black
         end if;
      end if;
   end if;
end process;

                          
---------------------------------------------------------------
-- lines per frame dependent on system :
-- 487: "000", 576: "001", 720: "010", 1035: "011", 1080: "100"
---------------------------------------------------------------
with system_i select
   lpf_index <=      "100" when X"A0",   -- 1080i/30
                     "100" when X"B0",   -- 1080i/29.97
                     "100" when X"82",   -- 1080p/30
                     "100" when X"92",   -- 1080p/29.97
                     "100" when X"83",   -- 1080p/25
                     "100" when X"84",   -- 1080p/24
                     "100" when X"94",   -- 1080p/23.98
                     "100" when X"A2",   -- 1080sF/30
                     "100" when X"B2",   -- 1080sF/29.97
                     "100" when X"A3",   -- 1080sF/25
                     "100" when X"A4",   -- 1080sF/24
                     "100" when X"B4",   -- 1080sF/23.98
                     "010" when X"86",   -- 720p/60
                     "010" when X"96",   -- 720p/59.94
                     "010" when X"87",   -- 720p/50
                     "010" when X"88",   -- 720p/30
                     "010" when X"98",   -- 720p/29.97
                     "010" when X"89",   -- 720p/25
                     "010" when X"8A",   -- 720p/24
                     "010" when X"9A",   -- 720p/23.98
                     "000" when X"60",   -- NTSC 525i/29.97
                     "001" when X"20",   -- PAL 625i/25
                     "100" when others;  -- 1080i/25  X"A1"

                                
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
-- Parameters dependent on lpf_index :
---------------------------------------------------------------
with lpf_index select
   v_grid_size_minus_1  <= conv_std_logic_vector(54-1,7)    when "000",    -- 487     -- grid_size must be even!
                           conv_std_logic_vector(64-1,7)    when "001",    -- 576
                           conv_std_logic_vector(80-1,7)    when "010",    -- 720
                           conv_std_logic_vector(120-1,7)   when others;   -- 1080


---------------------------------------------------------------
-- Parameters dependent on spl_index :
---------------------------------------------------------------
with spl_index select
   h_grid_size_minus_1  <= conv_std_logic_vector(45-1,7)   when "00",      -- 720
                           conv_std_logic_vector(80-1,7)   when "01",      -- 1280
                           conv_std_logic_vector(120-1,7)  when others;    -- 1920
                        
                        
end behavioral;