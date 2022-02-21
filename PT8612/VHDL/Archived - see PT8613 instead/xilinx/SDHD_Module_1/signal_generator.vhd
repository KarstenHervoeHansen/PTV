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

      -- frame control signals:
      fvh_i                : out std_logic_vector(2 downto 0);    -- blanking bits

      -- parallel data outputs:
      luma_o               : out std_logic_vector(9 downto 0);
      chroma_o             : out std_logic_vector(9 downto 0);

      -- debug:
      debug_o              : out std_logic_vector(9 downto 0)
   );
end colorbar_generator;


architecture behavioral of colorbar_generator is


type h_state_type is (hstart, h1, h2, h3, h4, h5, h6, h7, h8, h9, h10);
signal h_state                : h_state_type;

signal h_state_count          : std_logic_vector(10 downto 0);
signal resolution             : std_logic_vector(2 downto 0);
signal sav_tick               : std_logic;
signal h_bit_delayed_1        : std_logic;

constant h_offset             : integer := 1;  -- horizontal position adjustment

begin


resolution <= "000";


---------------------------------------------------------------
-- horizontal count selection :
---------------------------------------------------------------
h_count_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      h_count <= (others => '0');
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
      h_count <= (others => '0');
      elsif en_i = '1' then
         case resolution is
            when "000"  => -- 720x507 (525 lines)
               h_count   <= conv_std_logic_vector(86,11);          -- Make counts even numbers!
               h1_count  <= conv_std_logic_vector(16-h_offset,11);
               h10_count <= conv_std_logic_vector(16+h_offset,11);
            when "001"  => -- 720x576 (625 lines)
               h_count   <= conv_std_logic_vector(86,11);          -- Make counts even numbers!
               h1_count  <= conv_std_logic_vector(16-h_offset,11);
               h10_count <= conv_std_logic_vector(16+h_offset,11);
            when "010"  => -- 1280x720
               h_count   <= conv_std_logic_vector(152,11);          -- Make counts even numbers!
               h1_count  <= conv_std_logic_vector(32-h_offset,11);
               h10_count <= conv_std_logic_vector(32+h_offset,11);
            when "011"  => -- 1920x1035
               h_count   <= conv_std_logic_vector(230,11);          -- Make counts even numbers!
               h1_count  <= conv_std_logic_vector(40-h_offset,11);
               h10_count <= conv_std_logic_vector(40+h_offset,11);
            when others => -- 1920x1080
               h_count   <= conv_std_logic_vector(230,11);          -- Make counts even numbers!
               h1_count  <= conv_std_logic_vector(40-h_offset,11);
               h10_count <= conv_std_logic_vector(40+h_offset,11);
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Indicate start of active video :
---------------------------------------------------------------
h_bit_delayed_1_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      h_bit_delayed_1 <= '0';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         h_bit_delayed_1 <= '0';
      elsif en_i = '1' then
         h_bit_delayed_1 <= fvh_i(0);
      end if;
   end if;
end process;


sav_tick_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      sav_tick <= '0';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         sav_tick <= '0';
      elsif en_i = '1' then
         if h_bit_delayed_1 = '1' and fvh_i(0) = '0' then
            sav_tick <= '1';
         else
            sav_tick <= '0';
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Horizontal state machine :
---------------------------------------------------------------
horz_sm : process(zreset_i, clk_i)
begin
   if zreset_i = '0' then
      h_state <= hstart;
      h_state_count <= (others => '0');
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         h_state <= hstart;
         h_state_count <= (others => '0');
      elsif en_i = '1' then
         case h_state is
            when hstart =>
               if sav_tick = '1' then
                  h_state_count <= h1_count;
                  h_state <= h1;
               end if;
            when h1 =>
               if h_state_count = "00000000001" then
                  h_state_count <= h_count;
                  h_state <= h2;
               else
                  h_state_count <= h_state_count - 1;
               end if;
            when h2 =>
               if h_state_count = "00000000001" then
                  h_state_count <= h_count;
                  h_state <= h3;
               else
                  h_state_count <= h_state_count - 1;
               end if;
            when h3 =>
               if h_state_count = "00000000001" then
                  h_state_count <= h_count;
                  h_state <= h4;
               else
                  h_state_count <= h_state_count - 1;
               end if;
            when h4 =>
               if h_state_count = "00000000001" then
                  h_state_count <= h_count;
                  h_state <= h5;
               else
                  h_state_count <= h_state_count - 1;
               end if;
            when h5 =>
               if h_state_count = "00000000001" then
                  h_state_count <= h_count;
                  h_state <= h6;
               else
                  h_state_count <= h_state_count - 1;
               end if;
            when h6 =>
               if h_state_count = "00000000001" then
                  h_state_count <= h_count;
                  h_state <= h7;
               else
                  h_state_count <= h_state_count - 1;
               end if;
            when h7 =>
               if h_state_count = "00000000001" then
                  h_state_count <= h_count;
                  h_state <= h8;
               else
                  h_state_count <= h_state_count - 1;
               end if;
            when h8 =>
               if h_state_count = "00000000001" then
                  h_state_count <= h_count;
                  h_state <= h9;
               else
                  h_state_count <= h_state_count - 1;
               end if;
            when h9 =>
               if h_state_count = "00000000001" then
                  h_state_count <= h_count;
                  h_state <= h10;
               else
                  h_state_count <= h_state_count - 1;
               end if;
            when h10 =>
               if h_state_count = "00000000001" then
                  h_state_count <= (others => '0');
                  h_state <= hstart;
               else
                  h_state_count <= h_state_count - 1;
               end if;
            when others =>
               h_state <= hstart;
               h_state_count <= (others => '0');
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Color LUT :
---------------------------------------------------------------
color_lut : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
      
      elsif en_i = '1' then
         case h_state is
            when hstart => 
               y  <= conv_std_logic_vector(64,10);   -- Black
               cb <= conv_std_logic_vector(512,10);  -- Black
               cr <= conv_std_logic_vector(512,10);  -- Black
            when h1 => 
               y  <= conv_std_logic_vector(940,10);  -- 100% White
               cb <= conv_std_logic_vector(512,10);  -- 100% White
               cr <= conv_std_logic_vector(512,10);  -- 100% White
            when others => 

            
         end case;
      end if;
   end if;
end process;




---------------------------------------------------------------
-- Debug:
---------------------------------------------------------------

debug_o(0) <= sync_reset;
debug_o(1) <= frame_start_i;
debug_o(2) <= frame_end;
debug_o(3) <= en_i;

end behavioral;