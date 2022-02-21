library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity color_lut is
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;

      system_i             : in std_logic_vector(11 downto 0);
      color_presence_i     : in std_logic_vector(2 downto 0);           -- color_presence_i(2) : r, color_presence_i(1) : g, color_presence_i(0) : b
      color_level_i        : in std_logic_vector(4 downto 0);

      y_o                  : out std_logic_vector(11 downto 0);
      cb_o                 : out std_logic_vector(11 downto 0);
      cr_o                 : out std_logic_vector(11 downto 0)
   );
end color_lut;


architecture behavioral of color_lut is


signal ycbcr_zrgb       : std_logic;
signal hd_zsd           : std_logic;
signal spl_index        : std_logic_vector(1 downto 0);
signal a_rom            : std_logic_vector(5 downto 0);
signal color_index      : std_logic_vector(8 downto 0);
signal rom_data         : std_logic_vector(35 downto 0);
signal color_pres_del_1 : std_logic_vector(2 downto 0);
signal color_pres_del_2 : std_logic_vector(2 downto 0);


component color_rom
	port(
		A        : in std_logic_vector(5 downto 0);
		CLK      : in std_logic;
		QSPO_CE  : in std_logic;
		QSPO     : out std_logic_vector(35 downto 0)
	);
end component;


begin

---------------------------------------------------------------
-- Parameters :
---------------------------------------------------------------
---------------------------------------------------------------
-- samples per line dependent on system :
-- 720: "00", 1280: "01", 1920: "10"
---------------------------------------------------------------
with system_i(11 downto 4) select
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

                     
with spl_index select
   hd_zsd         <= '0'  when "00",    -- 720
                     '1'  when "01",    -- 1280
                     '1'  when others;  -- 1920

                     
ycbcr_zrgb_selecting : process (spl_index, system_i) begin
   if spl_index = "10" then     -- 1080 :                             
      case system_i(3 downto 0) is
      when X"1"   =>    ycbcr_zrgb <= '1';   -- YCbCr (4:2:2) 12-bit (dual-link)
      when X"2"   =>    ycbcr_zrgb <= '1';   -- YCbCr (4:4:4) 10-bit (dual-link)
      when X"3"   =>    ycbcr_zrgb <= '1';   -- YCbCr (4:4:4) 12-bit (dual-link)
      when X"4"   =>    ycbcr_zrgb <= '0';   -- RGB   (4:4:4) 10-bit (dual-link)
      when X"5"   =>    ycbcr_zrgb <= '0';   -- RGB   (4:4:4) 12-bit (dual-link)
      when others =>    ycbcr_zrgb <= '1';   -- YCbCr (4:2:2) 10-bit (single-link)
      end case;
   else
      ycbcr_zrgb <= '1';                     -- YCbCr (4:2:2) 10-bit (single-link)
   end if;
end process;
             

---------------------------------------------------------------
-- ROM containing 12-bit y & cb & cr vectors (as one 36 bit word):
---------------------------------------------------------------
color_value_rom : color_rom
   port map(
		A        => a_rom,
		CLK      => clk_i,
		QSPO_CE  => en_i,
		QSPO     => rom_data
	);


---------------------------------------------------------------
-- Mapping of output signals:
---------------------------------------------------------------
color_presence_delaying : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         color_pres_del_1 <= color_presence_i;
         color_pres_del_2 <= color_pres_del_1;
      end if;
   end if;
end process;


ycbcr_mapping : process (ycbcr_zrgb, rom_data, color_pres_del_2) begin
   if ycbcr_zrgb = '1' then      -- YCbCr : 
      y_o   <= rom_data(35 downto 24);
      cb_o  <= rom_data(23 downto 12);
      cr_o	<= rom_data(11 downto 0);
   else                          -- RGB :
      if color_pres_del_2(1) = '1' then     -- green presence:
         y_o   <= rom_data(35 downto 24);
      else
         y_o   <= "000100000000";
      end if;
      if color_pres_del_2(0) = '1' then     -- blue presence:
         cb_o   <= rom_data(35 downto 24);
      else
         cb_o   <= "000100000000";
      end if;
      if color_pres_del_2(2) = '1' then     -- red presence:
         cr_o   <= rom_data(35 downto 24);
      else
         cr_o   <= "000100000000";
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Map system, color and level to ROM address:
---------------------------------------------------------------
color_index <= hd_zsd & color_presence_i & color_level_i;


address_mapping : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if ycbcr_zrgb = '0' or color_presence_i = "111" then    -- RGB or White :
            case color_level_i is
            when "00000"      => a_rom <= "011000";   -- white    -5%
            when "00001"      => a_rom <= "011001";   -- white     0%
            when "00010"      => a_rom <= "011010";   -- white     5%
            when "00011"      => a_rom <= "011011";   -- white    10%
            when "00100"      => a_rom <= "011100";   -- white    15%
            when "00101"      => a_rom <= "011101";   -- white    20%
            when "00110"      => a_rom <= "011110";   -- white    25%
            when "00111"      => a_rom <= "011111";   -- white    30%
            when "01000"      => a_rom <= "100000";   -- white    35%
            when "01001"      => a_rom <= "100001";   -- white    40%
            when "01010"      => a_rom <= "100010";   -- white    45%
            when "01011"      => a_rom <= "100011";   -- white    50%
            when "01100"      => a_rom <= "100100";   -- white    55%
            when "01101"      => a_rom <= "100101";   -- white    60%
            when "01110"      => a_rom <= "100110";   -- white    65%
            when "01111"      => a_rom <= "100111";   -- white    70%
            when "10000"      => a_rom <= "101000";   -- white    75%
            when "10001"      => a_rom <= "101001";   -- white    80%
            when "10010"      => a_rom <= "101010";   -- white    85%
            when "10011"      => a_rom <= "101011";   -- white    90%
            when "10100"      => a_rom <= "101100";   -- white    95%
            when "10101"      => a_rom <= "101101";   -- white   100%
            when "10110"      => a_rom <= "101110";   -- white   105%
            when "10111"      => a_rom <= "101111";   -- white    -2%
            when "11000"      => a_rom <= "110000";   -- white     2%
            when "11001"      => a_rom <= "110001";   -- white   pluge grey 1
            when "11010"      => a_rom <= "110010";   -- white   pluge grey 2  
            when "11011"      => a_rom <= "110011";   -- white   pluge grey 2  
            when others       => a_rom <= "011001";   -- black
            end case;
         else
            case color_index is                                                      
            when "100110101"  => a_rom <= "000000";   -- HD blue    100%          
            when "101010101"  => a_rom <= "000001";   -- HD green   100%          
            when "101110101"  => a_rom <= "000010";   -- HD cyan    100%          
            when "110010101"  => a_rom <= "000011";   -- HD red     100%          
            when "110110101"  => a_rom <= "000100";   -- HD magenta 100%          
            when "111010101"  => a_rom <= "000101";   -- HD yellow  100%          
            when "100110000"  => a_rom <= "000110";   -- HD blue     75%          
            when "101010000"  => a_rom <= "000111";   -- HD green    75%              
            when "101110000"  => a_rom <= "001000";   -- HD cyan     75%          
            when "110010000"  => a_rom <= "001001";   -- HD red      75%          
            when "110110000"  => a_rom <= "001010";   -- HD magenta  75%          
            when "111010000"  => a_rom <= "001011";   -- HD yellow   75%          
            when "000110101"  => a_rom <= "001100";   -- SD blue    100%          
            when "001010101"  => a_rom <= "001101";   -- SD green   100%          
            when "001110101"  => a_rom <= "001110";   -- SD cyan    100%          
            when "010010101"  => a_rom <= "001111";   -- SD red     100%
            when "010110101"  => a_rom <= "010000";   -- SD magenta 100%
            when "011010101"  => a_rom <= "010001";   -- SD yellow  100%
            when "000110000"  => a_rom <= "010010";   -- SD blue     75%
            when "001010000"  => a_rom <= "010011";   -- SD green    75%
            when "001110000"  => a_rom <= "010100";   -- SD cyan     75%
            when "010010000"  => a_rom <= "010101";   -- SD red      75%
            when "010110000"  => a_rom <= "010110";   -- SD magenta  75%
            when "011010000"  => a_rom <= "010111";   -- SD yellow   75%
                                                      
            when others       => a_rom <= "011001";   -- black
            end case;
         end if;
      end if;
   end if;
end process;


end behavioral;