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
            
      -- frame control signals:
      fvh_i                : in std_logic_vector(2 downto 0);
      eav_tick_advanced_i  : in std_logic;
      sav_tick_advanced_i  : in std_logic;
      frame_end_i          : in std_logic;
      
      -- color parameter outputs:
      luma_o               : out std_logic_vector(9 downto 0);
      chroma_o             : out std_logic_vector(9 downto 0)
   );
end checkfield_generator;


architecture behavioral of checkfield_generator is


type line_state_type is (first_field1, upper_field1, lower_field1, end_field1, upper_field2, lower_field2, end_field2);
signal line_state          : line_state_type;

signal frame_count         : std_logic;
signal hd_zsd              : std_logic;
signal hd_zsd_temp         : std_logic;
signal state_count         : std_logic_vector(9 downto 0);
signal half_field_count    : std_logic_vector(9 downto 0);

begin


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
            half_field_count  <= conv_std_logic_vector(270,10);
            hd_zsd <= '1';
         when X"B0" =>
            -- 1080i/59.94
            half_field_count  <= conv_std_logic_vector(270,10);
            hd_zsd <= '1';
         when X"A1" =>           
            -- 1080i/50          
            half_field_count  <= conv_std_logic_vector(270,10);
            hd_zsd <= '1';
         when X"82" =>           
            -- 1080p/30          
            half_field_count  <= conv_std_logic_vector(540,10);
            hd_zsd <= '1';
         when X"92" =>           
            -- 1080p/29.97       
            half_field_count  <= conv_std_logic_vector(540,10);
            hd_zsd <= '1';
         when X"83" =>           
            -- 1080p/25          
            half_field_count  <= conv_std_logic_vector(540,10);
            hd_zsd <= '1';
         when X"84" =>           
            -- 1080p/24          
            half_field_count  <= conv_std_logic_vector(540,10);
            hd_zsd <= '1';
         when X"94" =>           
            -- 1080p/23.98       
            half_field_count  <= conv_std_logic_vector(540,10);
            hd_zsd <= '1';
         when X"A5" =>           
            -- 1035i/60          
            half_field_count  <= conv_std_logic_vector(518,10);
            hd_zsd <= '1';
         when X"B5" =>           
            -- 1035i/59.94       
            half_field_count  <= conv_std_logic_vector(518,10);
            hd_zsd <= '1';
         when X"86" =>           
            -- 720p/60           
            half_field_count  <= conv_std_logic_vector(360,10);
            hd_zsd <= '1';
         when X"96" =>           
            -- 720p/59.94        
            half_field_count  <= conv_std_logic_vector(360,10);
            hd_zsd <= '1';
         when X"87" =>           
            -- 720p/50           
            half_field_count  <= conv_std_logic_vector(360,10);
            hd_zsd <= '1';
         when X"88" =>           
            -- 720p/30           
            half_field_count  <= conv_std_logic_vector(360,10);
            hd_zsd <= '1';
         when X"98" =>           
         -- 720p/29.97           
            half_field_count  <= conv_std_logic_vector(360,10);
            hd_zsd <= '1';
         when X"89" =>           
         -- 720p/25              
            half_field_count  <= conv_std_logic_vector(360,10);
            hd_zsd <= '1';
         when X"8A" =>           
         -- 720p/24              
            half_field_count  <= conv_std_logic_vector(360,10);
            hd_zsd <= '1';
         when X"9A" =>           
         -- 720p/23.98           
            half_field_count  <= conv_std_logic_vector(360,10);
            hd_zsd <= '1';
         when X"60" =>           
         -- NTSC 525i/59.94      
            half_field_count  <= conv_std_logic_vector(124,10);
            hd_zsd <= '0';
         when X"20" =>           
         -- PAL 625i/50          
            half_field_count  <= conv_std_logic_vector(142,10);
            hd_zsd <= '0';
         when others =>          
         -- dont care          
            half_field_count  <= (others => 'X');
            hd_zsd <= 'X';
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Vertical state counter :
---------------------------------------------------------------
vertical_sm : process(zreset_i, clk_i)
begin
   if zreset_i = '0' then
      line_state <= end_field2;
      state_count <= (others => '0');
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         line_state <= end_field2;
         state_count <= (others => '0');
      elsif en_i = '1' then
         if frame_end_i = '1' then 
            line_state <= end_field2;
            state_count <= (others => 'X');
         elsif sav_tick_advanced_i = '1' then
            case line_state is
               when first_field1 =>
                  state_count <= half_field_count;
                  line_state  <= upper_field1;
               when upper_field1 =>
                  if state_count = "00000000001" then
                     state_count <= (others => 'X');
                     line_state  <= lower_field1;
                  else
                     state_count <= state_count - 1;
                  end if;
               when lower_field1 =>
                  if fvh_i(2) = '1' then
                     line_state  <= end_field1;
                  end if;
                  state_count <= (others => 'X');
               when end_field1 =>
                  if fvh_i(2) = '0' then 
                     line_state  <= upper_field2;
                     state_count <= half_field_count;
                  else
                     state_count <= (others => 'X');
                  end if;
               when upper_field2 =>
                  if state_count = "00000000000" then
                     state_count <= (others => 'X');
                     line_state  <= lower_field2;
                  else
                     state_count <= state_count - 1;
                  end if;
               when lower_field2 =>
                  if fvh_i(2) = '1' then 
                     line_state <= end_field2;
                  end if;
                  state_count <= (others => 'X');
               when others => -- end_field2
                  if fvh_i(2) = '0' then 
                     line_state <= first_field1;
                     state_count <= (others => 'X');
                  else
                     state_count <= (others => 'X');
                  end if;
            end case;
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
hd_zsd_temp <= '1';
color_generation : process(zreset_i, clk_i)
begin
   if zreset_i = '0' then
      luma_o   <= "1100000000";
      chroma_o <= "0110011000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         luma_o   <= "1100000000";
         chroma_o <= "0110011000";
      elsif en_i = '1' then
         case line_state is
            when first_field1 =>
               if hd_zsd_temp = '1' then  --hd:
                  if (fvh_i(0) = '1') and (frame_count = '0') then  -- first sample every other frame
                     chroma_o <= "0110010000";
                  else
                     chroma_o <= "0110011000";
                  end if;
                  luma_o <= "1100000000";
               else                    --sd:
                  if eav_tick_advanced_i = '1' then  -- last sample
                     luma_o <= "0010000000";
                  else
                     luma_o <= "1100000000";
                  end if;
                  chroma_o <= "0110011000";
               end if;
            when upper_field1 =>
               luma_o   <= "1100000000";
               chroma_o <= "0110011000";
            when lower_field1 =>
               luma_o   <= "1000000000";
               chroma_o <= "0100010000";
            when end_field1 =>
               luma_o   <= "1100000000";
               chroma_o <= "0110011000";
            when upper_field2 =>
               luma_o   <= "1100000000";
               chroma_o <= "0110011000";
            when lower_field2 =>
               luma_o   <= "1000000000";
               chroma_o <= "0100010000";
            when others =>    -- end_field2
               luma_o   <= "1100000000";
               chroma_o <= "0110011000";
         end case;
      end if;
   end if;
end process;


end behavioral;