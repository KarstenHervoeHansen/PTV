library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity checkfield_generator is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      -- system parameters:
      system_i             : in std_logic_vector(7 downto 0);
      interlaced_i         : in std_logic;
      hd_zsd_i             : in std_logic;
            
      -- frame control signals:
      fvh_i                : in std_logic_vector(2 downto 0);
      eav_tick_advanced_i  : in std_logic;
      frame_end_i          : in std_logic;
      
      -- color parameter outputs:
      luma_o               : out std_logic_vector(9 downto 0);
      chroma_o             : out std_logic_vector(9 downto 0)
   );
end checkfield_generator;


architecture behavioral of checkfield_generator is


signal frame_count         : std_logic;
signal lower_zupper        : std_logic;
signal interlaced          : std_logic;
signal hd_zsd_temp         : std_logic;
signal state_count         : std_logic_vector(9 downto 0);
signal half_field_count    : std_logic_vector(10 downto 0);
signal active_line_count   : std_logic_vector(10 downto 0);    -- first active line: active_line_count = 0
signal fvh                 : std_logic_vector(2 downto 0);


begin


---------------------------------------------------------------
-- Active line counter :
-- Counts on fvh bits
---------------------------------------------------------------
fvh_registering : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      fvh <= "011";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         fvh <= "011";
      elsif en_i = '1' then
         fvh <= fvh_i;
      end if;
   end if;
end process;


active_line_counting : process(zreset_i, clk_i)
begin
   if zreset_i = '0' then
      active_line_count <= (others => '0');
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         active_line_count <= (others => '0');
      elsif en_i = '1' then
         if fvh(0) = '0' and fvh_i(0) = '1' then    -- eav
            case fvh(2 downto 1) is
            when "01" =>   -- vertical blanking field 0
               active_line_count <= conv_std_logic_vector(0,11);
            when "11" =>   -- vertical blanking field 1
               active_line_count <= conv_std_logic_vector(1,11);
            when others => --active lines
               if interlaced_i = '0' then
                  active_line_count <= active_line_count + 1;    
               else
                  active_line_count <= active_line_count + 2;    
               end if;
            end case;
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- half field count selection :
---------------------------------------------------------------
state_count_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      half_field_count  <= (others => '0');
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         half_field_count  <= (others => '0');
      elsif en_i = '1' then
         case system_i is
         when X"A0" =>
            -- 1080i/60
            half_field_count  <= conv_std_logic_vector(540,11);
         when X"B0" =>
            -- 1080i/59.94
            half_field_count  <= conv_std_logic_vector(540,11);
         when X"A1" =>           
            -- 1080i/50          
            half_field_count  <= conv_std_logic_vector(540,11);
         when X"82" =>           
            -- 1080p/30          
            half_field_count  <= conv_std_logic_vector(540,11);
         when X"92" =>           
            -- 1080p/29.97       
            half_field_count  <= conv_std_logic_vector(540,11);
         when X"83" =>           
            -- 1080p/25          
            half_field_count  <= conv_std_logic_vector(540,11);
         when X"84" =>           
            -- 1080p/24          
            half_field_count  <= conv_std_logic_vector(540,11);
         when X"94" =>           
            -- 1080p/23.98       
            half_field_count  <= conv_std_logic_vector(540,11);
         when X"A5" =>           
            -- 1035i/60          
            half_field_count  <= conv_std_logic_vector(518,11);
         when X"B5" =>           
            -- 1035i/59.94       
            half_field_count  <= conv_std_logic_vector(518,11);
         when X"86" =>           
            -- 720p/60           
            half_field_count  <= conv_std_logic_vector(360,11);
         when X"96" =>           
            -- 720p/59.94        
            half_field_count  <= conv_std_logic_vector(360,11);
         when X"87" =>           
            -- 720p/50           
            half_field_count  <= conv_std_logic_vector(360,11);
         when X"88" =>           
            -- 720p/30           
            half_field_count  <= conv_std_logic_vector(360,11);
         when X"98" =>           
         -- 720p/29.97           
            half_field_count  <= conv_std_logic_vector(360,11);
         when X"89" =>           
         -- 720p/25              
            half_field_count  <= conv_std_logic_vector(360,11);
         when X"8A" =>           
         -- 720p/24              
            half_field_count  <= conv_std_logic_vector(360,11);
         when X"9A" =>           
         -- 720p/23.98           
            half_field_count  <= conv_std_logic_vector(360,11);
         when X"60" =>           
         -- NTSC 525i/59.94      
            half_field_count  <= conv_std_logic_vector(244,11);
         when X"20" =>           
         -- PAL 625i/50          
            half_field_count  <= conv_std_logic_vector(288,11);
         when others =>          
         -- dont care          
            half_field_count  <= (others => 'X');
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Upper / lower part of active video : 
---------------------------------------------------------------
lower_upper_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      lower_zupper <= '0';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         lower_zupper <= '0';
      elsif en_i = '1' then
         if fvh(1) = '1' then
            lower_zupper <= '0';
         elsif active_line_count(10 downto 1) = half_field_count(10 downto 1) then
            lower_zupper <= '1';
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Frame counter:
---------------------------------------------------------------
frame_counter : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      frame_count <= '0';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         frame_count <= '0';
      elsif en_i = '1' and frame_end_i = '1' then
         frame_count <= not frame_count;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate colors :
---------------------------------------------------------------
hd_zsd_temp <= hd_zsd_i;
color_generation : process(zreset_i, clk_i)
begin
   if zreset_i = '0' then
      luma_o   <= "0110011000";
      chroma_o <= "1100000000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         luma_o   <= "0110011000";
         chroma_o <= "1100000000";
      elsif en_i = '1' then
         if lower_zupper = '0' then -- upper half
            if active_line_count = conv_std_logic_vector(0,11) then  -- 1st line
               if eav_tick_advanced_i = '1' then                     -- last sample
                  if frame_count = '0' or hd_zsd_temp = '0' then     -- odd frames or sd
                     luma_o <= "0010000000";
                  else                                               -- even frames and hd 
                     luma_o <= "0110011000";
                  end if;
               else
                  luma_o <= "0110011000";
               end if;
            else                                                     -- rest of upper half
               luma_o   <= "0110011000";
            end if;
            chroma_o <= "1100000000";            
         else                                                        -- lower half
            luma_o   <= "0100010000";
            chroma_o <= "1000000000";
         end if;
      end if;
   end if;
end process;


end behavioral;