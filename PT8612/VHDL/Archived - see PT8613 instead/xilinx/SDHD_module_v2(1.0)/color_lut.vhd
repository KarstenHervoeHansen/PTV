library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity color_lut is
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;

      hd_zsd_i             : in std_logic;
      color_presence_i     : in std_logic_vector(2 downto 0);           -- color_presence_i(2) : r, color_presence_i(1) : g, color_presence_i(0) : b
      color_level_i        : in std_logic_vector(4 downto 0);

      y_o                  : out std_logic_vector(11 downto 0);
      cb_o                 : out std_logic_vector(11 downto 0);
      cr_o                 : out std_logic_vector(11 downto 0)
   );
end color_lut;


architecture behavioral of color_lut is


signal a_rom         : std_logic_vector(5 downto 0);
signal color_index   : std_logic_vector(8 downto 0);
signal rom_data      : std_logic_vector(35 downto 0);

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
y_o   <= rom_data(35 downto 24);
cb_o  <= rom_data(23 downto 12);
cr_o	<= rom_data(11 downto 0);


---------------------------------------------------------------
-- Map system, color and level to ROM address:
---------------------------------------------------------------
color_index <= hd_zsd_i & color_presence_i & color_level_i;


address_mapping : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         case color_index is                                                      
            when "100110101" => a_rom <= "000000";    -- HD blue    100%          
            when "101010101" => a_rom <= "000001";    -- HD green   100%          
            when "101110101" => a_rom <= "000010";    -- HD cyan    100%          
            when "110010101" => a_rom <= "000011";    -- HD red     100%          
            when "110110101" => a_rom <= "000100";    -- HD magenta 100%          
            when "111010101" => a_rom <= "000101";    -- HD yellow  100%          
            when "100110000" => a_rom <= "000110";    -- HD blue     75%          
            when "101010000" => a_rom <= "000111";    -- HD green    75%              
            when "101110000" => a_rom <= "001000";    -- HD cyan     75%          
            when "110010000" => a_rom <= "001001";    -- HD red      75%          
            when "110110000" => a_rom <= "001010";    -- HD magenta  75%          
            when "111010000" => a_rom <= "001011";    -- HD yellow   75%          
            when "000110101" => a_rom <= "001100";    -- SD blue    100%          
            when "001010101" => a_rom <= "001101";    -- SD green   100%          
            when "001110101" => a_rom <= "001110";    -- SD cyan    100%          
            when "010010101" => a_rom <= "001111";    -- SD red     100%
            when "010110101" => a_rom <= "010000";    -- SD magenta 100%
            when "011010101" => a_rom <= "010001";    -- SD yellow  100%
            when "000110000" => a_rom <= "010010";    -- SD blue     75%
            when "001010000" => a_rom <= "010011";    -- SD green    75%
            when "001110000" => a_rom <= "010100";    -- SD cyan     75%
            when "010010000" => a_rom <= "010101";    -- SD red      75%
            when "010110000" => a_rom <= "010110";    -- SD magenta  75%
            when "011010000" => a_rom <= "010111";    -- SD yellow   75%

            when "111100000" => a_rom <= "011000";    -- HD white    -5%
            when "111100001" => a_rom <= "011001";    -- HD white     0%
            when "111100010" => a_rom <= "011010";    -- HD white     5%
            when "111100011" => a_rom <= "011011";    -- HD white    10%
            when "111100100" => a_rom <= "011100";    -- HD white    15%
            when "111100101" => a_rom <= "011101";    -- HD white    20%
            when "111100110" => a_rom <= "011110";    -- HD white    25%
            when "111100111" => a_rom <= "011111";    -- HD white    30%
            when "111101000" => a_rom <= "100000";    -- HD white    35%
            when "111101001" => a_rom <= "100001";    -- HD white    40%
            when "111101010" => a_rom <= "100010";    -- HD white    45%
            when "111101011" => a_rom <= "100011";    -- HD white    50%
            when "111101100" => a_rom <= "100100";    -- HD white    55%
            when "111101101" => a_rom <= "100101";    -- HD white    60%
            when "111101110" => a_rom <= "100110";    -- HD white    65%
            when "111101111" => a_rom <= "100111";    -- HD white    70%
            when "111110000" => a_rom <= "101000";    -- HD white    75%
            when "111110001" => a_rom <= "101001";    -- HD white    80%
            when "111110010" => a_rom <= "101010";    -- HD white    85%
            when "111110011" => a_rom <= "101011";    -- HD white    90%
            when "111110100" => a_rom <= "101100";    -- HD white    95%
            when "111110101" => a_rom <= "101101";    -- HD white   100%
            when "111110110" => a_rom <= "101110";    -- HD white   105%
            when "111110111" => a_rom <= "101111";    -- HD white    -2%
            when "111111000" => a_rom <= "110000";    -- HD white     2%
            when "111111001" => a_rom <= "110001";    -- HD white   pluge grey 1
            when "111111010" => a_rom <= "110010";    -- HD white   pluge grey 2  
            when "111111011" => a_rom <= "110011";    -- HD white   pluge grey 2  

            when "011100000" => a_rom <= "011000";    -- SD white    -5%
            when "011100001" => a_rom <= "011001";    -- SD white     0%
            when "011100010" => a_rom <= "011010";    -- SD white     5%
            when "011100011" => a_rom <= "011011";    -- SD white    10%
            when "011100100" => a_rom <= "011100";    -- SD white    15%
            when "011100101" => a_rom <= "011101";    -- SD white    20%
            when "011100110" => a_rom <= "011110";    -- SD white    25%
            when "011100111" => a_rom <= "011111";    -- SD white    30%
            when "011101000" => a_rom <= "100000";    -- SD white    35%
            when "011101001" => a_rom <= "100001";    -- SD white    40%
            when "011101010" => a_rom <= "100010";    -- SD white    45%
            when "011101011" => a_rom <= "100011";    -- SD white    50%
            when "011101100" => a_rom <= "100100";    -- SD white    55%
            when "011101101" => a_rom <= "100101";    -- SD white    60%
            when "011101110" => a_rom <= "100110";    -- SD white    65%
            when "011101111" => a_rom <= "100111";    -- SD white    70%
            when "011110000" => a_rom <= "101000";    -- SD white    75%
            when "011110001" => a_rom <= "101001";    -- SD white    80%
            when "011110010" => a_rom <= "101010";    -- SD white    85%
            when "011110011" => a_rom <= "101011";    -- SD white    90%
            when "011110100" => a_rom <= "101100";    -- SD white    95%
            when "011110101" => a_rom <= "101101";    -- SD white   100%
            when "011110110" => a_rom <= "101110";    -- SD white   105%
            when "011110111" => a_rom <= "101111";    -- SD white    -2%
            when "011111000" => a_rom <= "110000";    -- SD white     2%
            when "011111001" => a_rom <= "110001";    -- SD white   pluge grey 1
            when "011111010" => a_rom <= "110010";    -- SD white   pluge grey 2  
            when "011111011" => a_rom <= "110011";    -- SD white   pluge grey 2  

            when others      => a_rom <= "011001";    -- black
         end case;
      end if;
   end if;
end process;


end behavioral;