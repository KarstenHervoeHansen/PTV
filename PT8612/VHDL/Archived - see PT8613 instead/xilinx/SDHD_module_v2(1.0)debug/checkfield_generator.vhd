library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity checkfield_generator is
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;

      -- frame control signals:
      lower_zupper_i       : in std_logic;
      av_line_i            : in std_logic_vector(10 downto 0);   
      eav_tick_advanced_i  : in std_logic;
      frame_end_i          : in std_logic;
      
      -- color parameter outputs:
      luma_o               : out std_logic_vector(9 downto 0);
      chroma_o             : out std_logic_vector(9 downto 0)
   );
end checkfield_generator;


architecture behavioral of checkfield_generator is


signal frame_count         : std_logic;
signal interlaced          : std_logic;
signal hd_zsd_temp         : std_logic;
signal state_count         : std_logic_vector(9 downto 0);
signal half_field_count    : std_logic_vector(10 downto 0);


begin


---------------------------------------------------------------
-- Frame counter:
---------------------------------------------------------------
frame_counter : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' and frame_end_i = '1' then
         frame_count <= not frame_count;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate colors :
---------------------------------------------------------------
hd_zsd_temp <= '1';--hd_zsd_i;

color_generation : process(clk_i)
begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if lower_zupper_i = '0' then -- upper half
            if av_line_i = conv_std_logic_vector(0,11) then          -- 1st line
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