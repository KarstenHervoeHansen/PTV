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
                           y_o  <= conv_std_logic_vector(41,8) & "0000";
                           cb_o <= conv_std_logic_vector(240,8) & "0000";
                           cr_o <= conv_std_logic_vector(110,8) & "0000";
                        when "010"  => -- Green:
                           y_o  <= conv_std_logic_vector(145,8) & "0000";
                           cb_o <= conv_std_logic_vector(54,8) & "0000";
                           cr_o <= conv_std_logic_vector(34,8) & "0000";
                        when "011"  => -- Cyan:
                           y_o  <= conv_std_logic_vector(170,8) & "0000";
                           cb_o <= conv_std_logic_vector(166,8) & "0000";
                           cr_o <= conv_std_logic_vector(16,8) & "0000";
                        when "100"  => -- Red:
                           y_o  <= conv_std_logic_vector(81,8) & "0000";
                           cb_o <= conv_std_logic_vector(90,8) & "0000";
                           cr_o <= conv_std_logic_vector(240,8) & "0000";
                        when "101"  => -- Magenta:
                           y_o  <= conv_std_logic_vector(106,8) & "0000";
                           cb_o <= conv_std_logic_vector(202,8) & "0000";
                           cr_o <= conv_std_logic_vector(222,8) & "0000";
                        when "110"  => -- Yellow:
                           y_o  <= conv_std_logic_vector(210,8) & "0000";
                           cb_o <= conv_std_logic_vector(16,8) & "0000";
                           cr_o <= conv_std_logic_vector(146,8) & "0000";
                        when "111"  => -- White:
                           y_o  <= conv_std_logic_vector(235,8) & "0000";
                           cb_o <= conv_std_logic_vector(128,8) & "0000";
                           cr_o <= conv_std_logic_vector(128,8) & "0000";
                        when others => -- Black:
                           y_o  <= conv_std_logic_vector(16,8) & "0000";
                           cb_o <= conv_std_logic_vector(128,8) & "0000";
                           cr_o <= conv_std_logic_vector(128,8) & "0000";
                     end case;
                  when others =>                                           -- 75%:
                     case color_index_i(3 downto 1) is
                        when "001"  => -- Blue:
                           y_o  <= conv_std_logic_vector(35,8) & "0000";
                           cb_o <= conv_std_logic_vector(212,8) & "0000";
                           cr_o <= conv_std_logic_vector(114,8) & "0000";
                        when "010"  => -- Green:
                           y_o  <= conv_std_logic_vector(112,8) & "0000";
                           cb_o <= conv_std_logic_vector(72,8) & "0000";
                           cr_o <= conv_std_logic_vector(58,8) & "0000";
                        when "011"  => -- Cyan:
                           y_o  <= conv_std_logic_vector(131,8) & "0000";
                           cb_o <= conv_std_logic_vector(156,8) & "0000";
                           cr_o <= conv_std_logic_vector(44,8) & "0000";
                        when "100"  => -- Red:
                           y_o  <= conv_std_logic_vector(65,8) & "0000";
                           cb_o <= conv_std_logic_vector(100,8) & "0000";
                           cr_o <= conv_std_logic_vector(212,8) & "0000";
                        when "101"  => -- Magenta:
                           y_o  <= conv_std_logic_vector(84,8) & "0000";
                           cb_o <= conv_std_logic_vector(184,8) & "0000";
                           cr_o <= conv_std_logic_vector(198,8) & "0000";
                        when "110"  => -- Yellow:
                           y_o  <= conv_std_logic_vector(162,8) & "0000";
                           cb_o <= conv_std_logic_vector(44,8) & "0000";
                           cr_o <= conv_std_logic_vector(142,8) & "0000";
                        when "111"  => -- White:
                           y_o  <= conv_std_logic_vector(180,8) & "0000";
                           cb_o <= conv_std_logic_vector(128,8) & "0000";
                           cr_o <= conv_std_logic_vector(128,8) & "0000";
                        when others => -- Black:
                           y_o  <= conv_std_logic_vector(16,8) & "0000";
                           cb_o <= conv_std_logic_vector(128,8) & "0000";
                           cr_o <= conv_std_logic_vector(128,8) & "0000";
                     end case;
               end case;
            when "01"  =>                                                  -- SMPTE 274 and 296:
               case color_index_i(0) is
                  when '0' =>                                              -- 100%:
                     case color_index_i(3 downto 1) is
                        when "001"  => -- Blue:
                           y_o  <= conv_std_logic_vector(41,8) & "0000";
                           cb_o <= conv_std_logic_vector(240,8) & "0000";
                           cr_o <= conv_std_logic_vector(110,8) & "0000";
                        when "010"  => -- Green:
                           y_o  <= conv_std_logic_vector(145,8) & "0000";
                           cb_o <= conv_std_logic_vector(54,8) & "0000";
                           cr_o <= conv_std_logic_vector(34,8) & "0000";
                        when "011"  => -- Cyan:
                           y_o  <= conv_std_logic_vector(170,8) & "0000";
                           cb_o <= conv_std_logic_vector(166,8) & "0000";
                           cr_o <= conv_std_logic_vector(16,8) & "0000";
                        when "100"  => -- Red:
                           y_o  <= conv_std_logic_vector(81,8) & "0000";
                           cb_o <= conv_std_logic_vector(90,8) & "0000";
                           cr_o <= conv_std_logic_vector(240,8) & "0000";
                        when "101"  => -- Magenta:
                           y_o  <= conv_std_logic_vector(106,8) & "0000";
                           cb_o <= conv_std_logic_vector(202,8) & "0000";
                           cr_o <= conv_std_logic_vector(222,8) & "0000";
                        when "110"  => -- Yellow:
                           y_o  <= conv_std_logic_vector(210,8) & "0000";
                           cb_o <= conv_std_logic_vector(16,8) & "0000";
                           cr_o <= conv_std_logic_vector(146,8) & "0000";
                        when "111"  => -- White:
                           y_o  <= conv_std_logic_vector(235,8) & "0000";
                           cb_o <= conv_std_logic_vector(128,8) & "0000";
                           cr_o <= conv_std_logic_vector(128,8) & "0000";
                        when others => -- Black:
                           y_o  <= conv_std_logic_vector(16,8) & "0000";
                           cb_o <= conv_std_logic_vector(128,8) & "0000";
                           cr_o <= conv_std_logic_vector(128,8) & "0000";
                     end case;
                  when others =>                                           -- 75%:
                     case color_index_i(3 downto 1) is
                        when "001"  => -- Blue:
                           y_o  <= conv_std_logic_vector(35,8) & "0000";
                           cb_o <= conv_std_logic_vector(212,8) & "0000";
                           cr_o <= conv_std_logic_vector(114,8) & "0000";
                        when "010"  => -- Green:
                           y_o  <= conv_std_logic_vector(112,8) & "0000";
                           cb_o <= conv_std_logic_vector(72,8) & "0000";
                           cr_o <= conv_std_logic_vector(58,8) & "0000";
                        when "011"  => -- Cyan:
                           y_o  <= conv_std_logic_vector(131,8) & "0000";
                           cb_o <= conv_std_logic_vector(156,8) & "0000";
                           cr_o <= conv_std_logic_vector(44,8) & "0000";
                        when "100"  => -- Red:
                           y_o  <= conv_std_logic_vector(65,8) & "0000";
                           cb_o <= conv_std_logic_vector(100,8) & "0000";
                           cr_o <= conv_std_logic_vector(212,8) & "0000";
                        when "101"  => -- Magenta:
                           y_o  <= conv_std_logic_vector(84,8) & "0000";
                           cb_o <= conv_std_logic_vector(184,8) & "0000";
                           cr_o <= conv_std_logic_vector(198,8) & "0000";
                        when "110"  => -- Yellow:
                           y_o  <= conv_std_logic_vector(162,8) & "0000";
                           cb_o <= conv_std_logic_vector(44,8) & "0000";
                           cr_o <= conv_std_logic_vector(142,8) & "0000";
                        when "111"  => -- White:
                           y_o  <= conv_std_logic_vector(180,8) & "0000";
                           cb_o <= conv_std_logic_vector(128,8) & "0000";
                           cr_o <= conv_std_logic_vector(128,8) & "0000";
                        when others => -- Black:
                           y_o  <= conv_std_logic_vector(16,8) & "0000";
                           cb_o <= conv_std_logic_vector(128,8) & "0000";
                           cr_o <= conv_std_logic_vector(128,8) & "0000";
                     end case;
               end case;
            when others =>                                                 -- SMPTE 240:
               case color_index_i(0) is
                  when '0' =>                                              -- 100%:
                     case color_index_i(3 downto 1) is
                        when "001"  => -- Blue:
                           y_o  <= conv_std_logic_vector(41,8) & "0000";
                           cb_o <= conv_std_logic_vector(240,8) & "0000";
                           cr_o <= conv_std_logic_vector(110,8) & "0000";
                        when "010"  => -- Green:
                           y_o  <= conv_std_logic_vector(145,8) & "0000";
                           cb_o <= conv_std_logic_vector(54,8) & "0000";
                           cr_o <= conv_std_logic_vector(34,8) & "0000";
                        when "011"  => -- Cyan:
                           y_o  <= conv_std_logic_vector(170,8) & "0000";
                           cb_o <= conv_std_logic_vector(166,8) & "0000";
                           cr_o <= conv_std_logic_vector(16,8) & "0000";
                        when "100"  => -- Red:
                           y_o  <= conv_std_logic_vector(81,8) & "0000";
                           cb_o <= conv_std_logic_vector(90,8) & "0000";
                           cr_o <= conv_std_logic_vector(240,8) & "0000";
                        when "101"  => -- Magenta:
                           y_o  <= conv_std_logic_vector(106,8) & "0000";
                           cb_o <= conv_std_logic_vector(202,8) & "0000";
                           cr_o <= conv_std_logic_vector(222,8) & "0000";
                        when "110"  => -- Yellow:
                           y_o  <= conv_std_logic_vector(210,8) & "0000";
                           cb_o <= conv_std_logic_vector(16,8) & "0000";
                           cr_o <= conv_std_logic_vector(146,8) & "0000";
                        when "111"  => -- White:
                           y_o  <= conv_std_logic_vector(235,8) & "0000";
                           cb_o <= conv_std_logic_vector(128,8) & "0000";
                           cr_o <= conv_std_logic_vector(128,8) & "0000";
                        when others => -- Black:
                           y_o  <= conv_std_logic_vector(16,8) & "0000";
                           cb_o <= conv_std_logic_vector(128,8) & "0000";
                           cr_o <= conv_std_logic_vector(128,8) & "0000";
                     end case;
                  when others =>                                           -- 75%:
                     case color_index_i(3 downto 1) is
                        when "001"  => -- Blue:
                           y_o  <= conv_std_logic_vector(35,8) & "0000";
                           cb_o <= conv_std_logic_vector(212,8) & "0000";
                           cr_o <= conv_std_logic_vector(114,8) & "0000";
                        when "010"  => -- Green:
                           y_o  <= conv_std_logic_vector(112,8) & "0000";
                           cb_o <= conv_std_logic_vector(72,8) & "0000";
                           cr_o <= conv_std_logic_vector(58,8) & "0000";
                        when "011"  => -- Cyan:
                           y_o  <= conv_std_logic_vector(131,8) & "0000";
                           cb_o <= conv_std_logic_vector(156,8) & "0000";
                           cr_o <= conv_std_logic_vector(44,8) & "0000";
                        when "100"  => -- Red:
                           y_o  <= conv_std_logic_vector(65,8) & "0000";
                           cb_o <= conv_std_logic_vector(100,8) & "0000";
                           cr_o <= conv_std_logic_vector(212,8) & "0000";
                        when "101"  => -- Magenta:
                           y_o  <= conv_std_logic_vector(84,8) & "0000";
                           cb_o <= conv_std_logic_vector(184,8) & "0000";
                           cr_o <= conv_std_logic_vector(198,8) & "0000";
                        when "110"  => -- Yellow:
                           y_o  <= conv_std_logic_vector(162,8) & "0000";
                           cb_o <= conv_std_logic_vector(44,8) & "0000";
                           cr_o <= conv_std_logic_vector(142,8) & "0000";
                        when "111"  => -- White:
                           y_o  <= conv_std_logic_vector(180,8) & "0000";
                           cb_o <= conv_std_logic_vector(128,8) & "0000";
                           cr_o <= conv_std_logic_vector(128,8) & "0000";
                        when others => -- Black:
                           y_o  <= conv_std_logic_vector(16,8) & "0000";
                           cb_o <= conv_std_logic_vector(128,8) & "0000";
                           cr_o <= conv_std_logic_vector(128,8) & "0000";
                     end case;
               end case;
         end case;
      end if;
   end if;
end process;


end behavioral;