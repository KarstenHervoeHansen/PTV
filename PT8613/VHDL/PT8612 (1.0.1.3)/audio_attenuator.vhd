library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_signed.all;
USE ieee.std_logic_arith.all;


entity audio_attenuator is
   port (
      audio_i              : in std_logic_vector(17 downto 0);
      level_index_i        : in std_logic_vector(7 downto 0);
      mute_i               : in std_logic;
      audio_o              : out std_logic_vector(23 downto 0)
   );
end audio_attenuator;


-- Attenuation of an 18 bit signal (2's complement).
-- Done by a multiplication followed by a bit-shift.


architecture behavioral of audio_attenuator is

signal left_shifts      : integer;
signal multiplier_gain  : std_logic_vector(17 downto 0);
signal audio_mult       : std_logic_vector(35 downto 0);
signal audio            : std_logic_vector(23 downto 0);

component MULT18X18
   port(
      A  : in std_logic_vector(17 downto 0);
	 	B  : in std_logic_vector(17 downto 0);
		P  : out std_logic_vector(35 downto 0)
   );
end component;


begin


---------------------------------------------------------------
-- Table of bit shifts vs. dB:
---------------------------------------------------------------
with level_index_i select
   bit_shifts <=  1 when X"1F",   --   -1 dBFS
                  1 when X"1E",   --   -2 dBFS
                  1 when X"1D",   --   -3 dBFS
                  1 when X"1C",   --   -4 dBFS
                  1 when X"1B",   --   -5 dBFS
                  1 when X"1A",   --   -6 dBFS
                  2 when X"19",   --   -7 dBFS
                  2 when X"18",   --   -8 dBFS
                  2 when X"17",   --   -9 dBFS
                  2 when X"16",   --  -10 dBFS
                  2 when X"15",   --  -11 dBFS
                  2 when X"14",   --  -12 dBFS
                  3 when X"13",   --  -13 dBFS
                  3 when X"12",   --  -14 dBFS
                  3 when X"11",   --  -15 dBFS
                  3 when X"10",   --  -16 dBFS
                  3 when X"0F",   --  -17 dBFS
                  3 when X"0E",   --  -18 dBFS
                  4 when X"0D",   --  -19 dBFS
                  4 when X"0C",   --  -20 dBFS
                  4 when X"0B",   --  -21 dBFS
                  4 when X"0A",   --  -22 dBFS
                  4 when X"09",   --  -23 dBFS
                  4 when X"08",   --  -24 dBFS
                  5 when X"07",   --  -25 dBFS
                  5 when X"06",   --  -26 dBFS
                  5 when X"05",   --  -27 dBFS
                  5 when X"04",   --  -28 dBFS
                  5 when X"03",   --  -29 dBFS
                  5 when X"02",   --  -30 dBFS
                  0 when others;  --    0 dBFS               

                  
---------------------------------------------------------------
-- Table of multiplication factors vs. dB:
---------------------------------------------------------------
with level_index_i select                  
   multiplier_gain <=   conv_std_logic_vector(116818,18) when X"1F",   --   -1 dBFS
                        conv_std_logic_vector(104114,18) when X"1E",   --   -2 dBFS
                        conv_std_logic_vector( 92792,18) when X"1D",   --   -3 dBFS
                        conv_std_logic_vector( 82701,18) when X"1C",   --   -4 dBFS
                        conv_std_logic_vector( 73707,18) when X"1B",   --   -5 dBFS
                        conv_std_logic_vector( 65692,18) when X"1A",   --   -6 dBFS
                        conv_std_logic_vector(117095,18) when X"19",   --   -7 dBFS
                        conv_std_logic_vector(104361,18) when X"18",   --   -8 dBFS
                        conv_std_logic_vector( 93012,18) when X"17",   --   -9 dBFS
                        conv_std_logic_vector( 82897,18) when X"16",   --  -10 dBFS
                        conv_std_logic_vector( 73882,18) when X"15",   --  -11 dBFS
                        conv_std_logic_vector( 65848,18) when X"14",   --  -12 dBFS
                        conv_std_logic_vector(117373,18) when X"13",   --  -13 dBFS
                        conv_std_logic_vector(104609,18) when X"12",   --  -14 dBFS
                        conv_std_logic_vector( 93233,18) when X"11",   --  -15 dBFS
                        conv_std_logic_vector( 83094,18) when X"10",   --  -16 dBFS
                        conv_std_logic_vector( 74058,18) when X"0F",   --  -17 dBFS
                        conv_std_logic_vector( 66004,18) when X"0E",   --  -18 dBFS
                        conv_std_logic_vector(117652,18) when X"0D",   --  -19 dBFS
                        conv_std_logic_vector(104858,18) when X"0C",   --  -20 dBFS
                        conv_std_logic_vector( 93454,18) when X"0B",   --  -21 dBFS
                        conv_std_logic_vector( 83291,18) when X"0A",   --  -22 dBFS
                        conv_std_logic_vector( 74233,18) when X"09",   --  -23 dBFS
                        conv_std_logic_vector( 66161,18) when X"08",   --  -24 dBFS
                        conv_std_logic_vector(117932,18) when X"07",   --  -25 dBFS
                        conv_std_logic_vector(105107,18) when X"06",   --  -26 dBFS
                        conv_std_logic_vector( 93676,18) when X"05",   --  -27 dBFS
                        conv_std_logic_vector( 83489,18) when X"04",   --  -28 dBFS
                        conv_std_logic_vector( 74410,18) when X"03",   --  -29 dBFS
                        conv_std_logic_vector( 66318,18) when X"02",   --  -30 dBFS
                        conv_std_logic_vector( 65536,18) when others;  --    0 dBFS               
                                                               
                                                                                 
multiplier_1 : MULT18X18
port map (
   A  => multiplier_gain,
   B  => audio_i,
   P  => audio_mult
);
                  
audio_mult_24_bit  <= audio_mult(34 downto 11);
                  
audio_bit_shift : process (audio_mult, bit_shifts) begin
   case bit_shifts is                                                      
      when 1      => audio <= to_stdlogicvector(to_bitvector(audio_mult_24_bit) sra 1);   -- 1  
      when 2      => audio <= to_stdlogicvector(to_bitvector(audio_mult_24_bit) sra 2);   -- 2  
      when 3      => audio <= to_stdlogicvector(to_bitvector(audio_mult_24_bit) sra 3);   -- 3  
      when 4      => audio <= to_stdlogicvector(to_bitvector(audio_mult_24_bit) sra 4);   -- 4  
      when 5      => audio <= to_stdlogicvector(to_bitvector(audio_mult_24_bit) sra 5);   -- 5  
      when others => audio <= audio_mult_24_bit;
   end case;
end process;

with mute_i select                  
   audio_o  <= audio             when '0',     --   normnal
               (others => '0')   when others;  --   muted

end behavioral;