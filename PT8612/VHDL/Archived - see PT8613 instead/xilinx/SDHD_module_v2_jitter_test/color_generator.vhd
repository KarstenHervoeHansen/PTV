library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity color_generator is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      color_system_i       : in std_logic_vector(1 downto 0);     -- "00":ITU-R BT601-5, "01":SMPTE 274 and 296, "10":SMPTE 240
      color_index_i        : in std_logic_vector(3 downto 0);     -- color_index_i(3) : r, color_index_i(2) : g, color_index_i(1) : b, color_index_i(0) : '0':100% '1':75%
      
      y_o                  : out std_logic_vector(11 downto 0);
      cb_o                 : out std_logic_vector(11 downto 0);
      cr_o                 : out std_logic_vector(11 downto 0)
   );
end color_generator;


architecture behavioral of color_generator is


begin


---------------------------------------------------------------
-- Color look-up-table :
---------------------------------------------------------------
color_lut : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      y_o  <= "000100000000";
      cb_o <= "100000000000";
      cr_o <= "000000000000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         y_o  <= "000100000000";
         cb_o <= "100000000000";
         cr_o <= "000000000000";
      elsif en_i = '1' then
         case color_system_i is
            when "00"  =>                                                  -- ITU-R BT601-5:
               case color_index_i(0) is
                  when '0' =>                                              -- 100%:
                     case color_index_i(3 downto 1) is
                        when "001"  => -- Blue:
                           y_o  <= conv_std_logic_vector(655,12);
                           cb_o <= conv_std_logic_vector(3840,12);
                           cr_o <= conv_std_logic_vector(1757,12);
                        when "010"  => -- Green:
                           y_o  <= conv_std_logic_vector(2313,12);
                           cb_o <= conv_std_logic_vector(861,12);
                           cr_o <= conv_std_logic_vector(547,12);
                        when "011"  => -- Cyan:
                           y_o  <= conv_std_logic_vector(2712,12);
                           cb_o <= conv_std_logic_vector(2653,12);
                           cr_o <= conv_std_logic_vector(256,12);
                        when "100"  => -- Red:
                           y_o  <= conv_std_logic_vector(1304,12);
                           cb_o <= conv_std_logic_vector(1443,12);
                           cr_o <= conv_std_logic_vector(3840,12);
                        when "101"  => -- Magenta:
                           y_o  <= conv_std_logic_vector(1703,12);
                           cb_o <= conv_std_logic_vector(3235,12);
                           cr_o <= conv_std_logic_vector(3549,12);
                        when "110"  => -- Yellow:
                           y_o  <= conv_std_logic_vector(3361,12);
                           cb_o <= conv_std_logic_vector(256,12);
                           cr_o <= conv_std_logic_vector(2339,12);
                        when "111"  => -- White:
                           y_o  <= conv_std_logic_vector(3760,12);
                           cb_o <= conv_std_logic_vector(2048,12);
                           cr_o <= conv_std_logic_vector(2048,12);
                        when others => -- Black:
                           y_o  <= conv_std_logic_vector(256,12);
                           cb_o <= conv_std_logic_vector(2048,12);
                           cr_o <= conv_std_logic_vector(2048,12);
                     end case;
                  when others =>                                           -- 75%:
                     case color_index_i(3 downto 1) is
                        when "001"  => -- Blue:
                           y_o  <= conv_std_logic_vector(556,12);
                           cb_o <= conv_std_logic_vector(3392,12);
                           cr_o <= conv_std_logic_vector(1829,12);
                        when "010"  => -- Green:
                           y_o  <= conv_std_logic_vector(1799,12);
                           cb_o <= conv_std_logic_vector(1158,12);
                           cr_o <= conv_std_logic_vector(923,12);
                        when "011"  => -- Cyan:
                           y_o  <= conv_std_logic_vector(2098,12);
                           cb_o <= conv_std_logic_vector(2502,12);
                           cr_o <= conv_std_logic_vector(704,12);
                        when "100"  => -- Red:
                           y_o  <= conv_std_logic_vector(1042,12);
                           cb_o <= conv_std_logic_vector(1594,12);
                           cr_o <= conv_std_logic_vector(3392,12);
                        when "101"  => -- Magenta:
                           y_o  <= conv_std_logic_vector(1341,12);
                           cb_o <= conv_std_logic_vector(2938,12);
                           cr_o <= conv_std_logic_vector(3173,12);
                        when "110"  => -- Yellow:
                           y_o  <= conv_std_logic_vector(2584,12);
                           cb_o <= conv_std_logic_vector(704,12);
                           cr_o <= conv_std_logic_vector(2267,12);
                        when "111"  => -- White:
                           y_o  <= conv_std_logic_vector(2884,12);
                           cb_o <= conv_std_logic_vector(2048,12);
                           cr_o <= conv_std_logic_vector(2048,12);
                        when others => -- Black:
                           y_o  <= conv_std_logic_vector(256,12);
                           cb_o <= conv_std_logic_vector(2048,12);
                           cr_o <= conv_std_logic_vector(2048,12);
                     end case;
               end case;
            when "01"  =>                                                  -- SMPTE 274 and 296:
               case color_index_i(0) is
                  when '0' =>                                              -- 100%:
                     case color_index_i(3 downto 1) is
                        when "001"  => -- Blue:
                           y_o  <= conv_std_logic_vector(509,12);
                           cb_o <= conv_std_logic_vector(3840,12);
                           cr_o <= conv_std_logic_vector(1884,12);
                        when "010"  => -- Green:         
                           y_o  <= conv_std_logic_vector(2762,12);
                           cb_o <= conv_std_logic_vector(667,12);
                           cr_o <= conv_std_logic_vector(420,12);
                        when "011"  => -- Cyan:          
                           y_o  <= conv_std_logic_vector(3015,12);
                           cb_o <= conv_std_logic_vector(2459,12);
                           cr_o <= conv_std_logic_vector(256,12);
                        when "100"  => -- Red:           
                           y_o  <= conv_std_logic_vector(1001,12);
                           cb_o <= conv_std_logic_vector(1637,12);
                           cr_o <= conv_std_logic_vector(3840,12);
                        when "101"  => -- Magenta:       
                           y_o  <= conv_std_logic_vector(1254,12);
                           cb_o <= conv_std_logic_vector(3429,12);
                           cr_o <= conv_std_logic_vector(3676,12);
                        when "110"  => -- Yellow:        
                           y_o  <= conv_std_logic_vector(3507,12);
                           cb_o <= conv_std_logic_vector(256,12);
                           cr_o <= conv_std_logic_vector(2212,12);
                        when "111"  => -- White:         
                           y_o  <= conv_std_logic_vector(3760,12);
                           cb_o <= conv_std_logic_vector(2048,12);
                           cr_o <= conv_std_logic_vector(2048,12);
                        when others => -- Black:         
                           y_o  <= conv_std_logic_vector(256,12);
                           cb_o <= conv_std_logic_vector(2048,12);
                           cr_o <= conv_std_logic_vector(2048,12);
                     end case;
                  when others =>                                           -- 75%:
                     case color_index_i(3 downto 1) is
                        when "001"  => -- Blue:
                           y_o  <= conv_std_logic_vector(446,12);
                           cb_o <= conv_std_logic_vector(3392,12);
                           cr_o <= conv_std_logic_vector(1925,12);
                        when "010"  => -- Green:
                           y_o  <= conv_std_logic_vector(2136,12);
                           cb_o <= conv_std_logic_vector(1012,12);
                           cr_o <= conv_std_logic_vector(827,12);
                        when "011"  => -- Cyan:
                           y_o  <= conv_std_logic_vector(2325,12);
                           cb_o <= conv_std_logic_vector(2356,12);
                           cr_o <= conv_std_logic_vector(704,12);
                        when "100"  => -- Red:
                           y_o  <= conv_std_logic_vector(815,12);
                           cb_o <= conv_std_logic_vector(1740,12);
                           cr_o <= conv_std_logic_vector(3392,12);
                        when "101"  => -- Magenta:
                           y_o  <= conv_std_logic_vector(1004,12);
                           cb_o <= conv_std_logic_vector(3084,12);
                           cr_o <= conv_std_logic_vector(3269,12);
                        when "110"  => -- Yellow:
                           y_o  <= conv_std_logic_vector(2694,12);
                           cb_o <= conv_std_logic_vector(704,12);
                           cr_o <= conv_std_logic_vector(2171,12);
                        when "111"  => -- White:
                           y_o  <= conv_std_logic_vector(2884,12);
                           cb_o <= conv_std_logic_vector(2048,12);
                           cr_o <= conv_std_logic_vector(2048,12);
                        when others => -- Black:
                           y_o  <= conv_std_logic_vector(256,12);
                           cb_o <= conv_std_logic_vector(2048,12);
                           cr_o <= conv_std_logic_vector(2048,12);
                     end case;
               end case;
            when others =>                                                 -- SMPTE 240:
               case color_index_i(0) is
                  when '0' =>                                              -- 100%:
                     case color_index_i(3 downto 1) is
                        when "001"  => -- Blue:
                           y_o  <= conv_std_logic_vector(561,12);
                           cb_o <= conv_std_logic_vector(3840,12);
                           cr_o <= conv_std_logic_vector(1850,12);
                        when "010"  => -- Green:
                           y_o  <= conv_std_logic_vector(2712,12);
                           cb_o <= conv_std_logic_vector(672,12);
                           cr_o <= conv_std_logic_vector(454,12);
                        when "011"  => -- Cyan:
                           y_o  <= conv_std_logic_vector(3017,12);
                           cb_o <= conv_std_logic_vector(2464,12);
                           cr_o <= conv_std_logic_vector(256,12);
                        when "100"  => -- Red:
                           y_o  <= conv_std_logic_vector(999,12);
                           cb_o <= conv_std_logic_vector(1632,12);
                           cr_o <= conv_std_logic_vector(3840,12);
                        when "101"  => -- Magenta:
                           y_o  <= conv_std_logic_vector(1304,12);
                           cb_o <= conv_std_logic_vector(3424,12);
                           cr_o <= conv_std_logic_vector(3642,12);
                        when "110"  => -- Yellow:
                           y_o  <= conv_std_logic_vector(3455,12);
                           cb_o <= conv_std_logic_vector(256,12);
                           cr_o <= conv_std_logic_vector(2246,12);
                        when "111"  => -- White:
                           y_o  <= conv_std_logic_vector(3760,12);
                           cb_o <= conv_std_logic_vector(2048,12);
                           cr_o <= conv_std_logic_vector(2048,12);
                        when others => -- Black:
                           y_o  <= conv_std_logic_vector(256,12);
                           cb_o <= conv_std_logic_vector(2048,12);
                           cr_o <= conv_std_logic_vector(2048,12);
                     end case;
                  when others =>                                           -- 75%:
                     case color_index_i(3 downto 1) is
                        when "001"  => -- Blue:
                           y_o  <= conv_std_logic_vector(485,12);
                           cb_o <= conv_std_logic_vector(3392,12);
                           cr_o <= conv_std_logic_vector(1900,12);
                        when "010"  => -- Green:
                           y_o  <= conv_std_logic_vector(2098,12);
                           cb_o <= conv_std_logic_vector(1016,12);
                           cr_o <= conv_std_logic_vector(852,12);
                        when "011"  => -- Cyan:
                           y_o  <= conv_std_logic_vector(2327,12);
                           cb_o <= conv_std_logic_vector(2360,12);
                           cr_o <= conv_std_logic_vector(704,12);
                        when "100"  => -- Red:
                           y_o  <= conv_std_logic_vector(813,12);
                           cb_o <= conv_std_logic_vector(1736,12);
                           cr_o <= conv_std_logic_vector(3392,12);
                        when "101"  => -- Magenta:
                           y_o  <= conv_std_logic_vector(1042,12);
                           cb_o <= conv_std_logic_vector(3080,12);
                           cr_o <= conv_std_logic_vector(3244,12);
                        when "110"  => -- Yellow:
                           y_o  <= conv_std_logic_vector(2655,12);
                           cb_o <= conv_std_logic_vector(704,12);
                           cr_o <= conv_std_logic_vector(2196,12);
                        when "111"  => -- White:
                           y_o  <= conv_std_logic_vector(2884,12);
                           cb_o <= conv_std_logic_vector(2048,12);
                           cr_o <= conv_std_logic_vector(2048,12);
                        when others => -- Black:
                           y_o  <= conv_std_logic_vector(256,12);
                           cb_o <= conv_std_logic_vector(2048,12);
                           cr_o <= conv_std_logic_vector(2048,12);
                     end case;
               end case;
         end case;
      end if;
   end if;
end process;


end behavioral;