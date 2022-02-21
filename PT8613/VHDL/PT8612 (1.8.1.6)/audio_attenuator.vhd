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

signal bit_shifts          : integer;
signal multiplier_gain     : std_logic_vector(17 downto 0);
signal audio_mult          : std_logic_vector(35 downto 0);
signal gained_audio        : std_logic_vector(35 downto 0);

component MULT18X18
   port(
      A  : in std_logic_vector(17 downto 0);
	 	B  : in std_logic_vector(17 downto 0);
		P  : out std_logic_vector(35 downto 0)
   );
end component;


begin


---------------------------------------------------------------
-- Table of multiplication factors and bit shifts vs. dB:
---------------------------------------------------------------
level_parameters : process (level_index_i,mute_i) begin
   if mute_i = '0' then
      case level_index_i is
      when X"00"        => multiplier_gain <= conv_std_logic_vector( 65536,18); bit_shifts <=  0;  --    0 dBFS
      when X"FF"        => multiplier_gain <= conv_std_logic_vector(116818,18); bit_shifts <=  1;  --   -1 dBFS
      when X"FE"        => multiplier_gain <= conv_std_logic_vector(104114,18); bit_shifts <=  1;  --   -2 dBFS
      when X"FD"        => multiplier_gain <= conv_std_logic_vector( 92792,18); bit_shifts <=  1;  --   -3 dBFS
      when X"FC"        => multiplier_gain <= conv_std_logic_vector( 82701,18); bit_shifts <=  1;  --   -4 dBFS
      when X"FB"        => multiplier_gain <= conv_std_logic_vector( 73707,18); bit_shifts <=  1;  --   -5 dBFS
      when X"FA"|X"01"  => multiplier_gain <= conv_std_logic_vector( 65692,18); bit_shifts <=  1;  --   -6 dBFS
      when X"F9"        => multiplier_gain <= conv_std_logic_vector(117095,18); bit_shifts <=  2;  --   -7 dBFS
      when X"F8"        => multiplier_gain <= conv_std_logic_vector(104361,18); bit_shifts <=  2;  --   -8 dBFS
      when X"F7"        => multiplier_gain <= conv_std_logic_vector( 93012,18); bit_shifts <=  2;  --   -9 dBFS
      when X"F6"        => multiplier_gain <= conv_std_logic_vector( 82897,18); bit_shifts <=  2;  --  -10 dBFS
      when X"F5"        => multiplier_gain <= conv_std_logic_vector( 73882,18); bit_shifts <=  2;  --  -11 dBFS
      when X"F4"|X"02"  => multiplier_gain <= conv_std_logic_vector( 65848,18); bit_shifts <=  2;  --  -12 dBFS
      when X"F3"        => multiplier_gain <= conv_std_logic_vector(117373,18); bit_shifts <=  3;  --  -13 dBFS
      when X"F2"        => multiplier_gain <= conv_std_logic_vector(104609,18); bit_shifts <=  3;  --  -14 dBFS
      when X"F1"        => multiplier_gain <= conv_std_logic_vector( 93233,18); bit_shifts <=  3;  --  -15 dBFS
      when X"F0"        => multiplier_gain <= conv_std_logic_vector( 83094,18); bit_shifts <=  3;  --  -16 dBFS
      when X"EF"        => multiplier_gain <= conv_std_logic_vector( 74058,18); bit_shifts <=  3;  --  -17 dBFS
      when X"EE"|X"03"  => multiplier_gain <= conv_std_logic_vector( 66004,18); bit_shifts <=  3;  --  -18 dBFS
      when X"ED"        => multiplier_gain <= conv_std_logic_vector(117652,18); bit_shifts <=  4;  --  -19 dBFS
      when X"EC"        => multiplier_gain <= conv_std_logic_vector(104858,18); bit_shifts <=  4;  --  -20 dBFS
      when X"EB"        => multiplier_gain <= conv_std_logic_vector( 93454,18); bit_shifts <=  4;  --  -21 dBFS
      when X"EA"        => multiplier_gain <= conv_std_logic_vector( 83291,18); bit_shifts <=  4;  --  -22 dBFS
      when X"E9"        => multiplier_gain <= conv_std_logic_vector( 74233,18); bit_shifts <=  4;  --  -23 dBFS
      when X"E8"|X"04"  => multiplier_gain <= conv_std_logic_vector( 66161,18); bit_shifts <=  4;  --  -24 dBFS
      when X"E7"        => multiplier_gain <= conv_std_logic_vector(117932,18); bit_shifts <=  5;  --  -25 dBFS
      when X"E6"        => multiplier_gain <= conv_std_logic_vector(105107,18); bit_shifts <=  5;  --  -26 dBFS
      when X"E5"        => multiplier_gain <= conv_std_logic_vector( 93676,18); bit_shifts <=  5;  --  -27 dBFS
      when X"E4"        => multiplier_gain <= conv_std_logic_vector( 83489,18); bit_shifts <=  5;  --  -28 dBFS
      when X"E3"        => multiplier_gain <= conv_std_logic_vector( 74410,18); bit_shifts <=  5;  --  -29 dBFS
      when X"E2"        => multiplier_gain <= conv_std_logic_vector( 66318,18); bit_shifts <=  5;  --  -30 dBFS
      when X"E1"        => multiplier_gain <= conv_std_logic_vector(118212,18); bit_shifts <=  6;  --  -31 dBFS
      when X"E0"        => multiplier_gain <= conv_std_logic_vector(105356,18); bit_shifts <=  6;  --  -32 dBFS
      when X"DF"        => multiplier_gain <= conv_std_logic_vector( 93899,18); bit_shifts <=  6;  --  -33 dBFS
      when X"DE"        => multiplier_gain <= conv_std_logic_vector( 83687,18); bit_shifts <=  6;  --  -34 dBFS
      when X"DD"        => multiplier_gain <= conv_std_logic_vector( 74586,18); bit_shifts <=  6;  --  -35 dBFS
      when X"DC"        => multiplier_gain <= conv_std_logic_vector( 66475,18); bit_shifts <=  6;  --  -36 dBFS
      when X"DB"        => multiplier_gain <= conv_std_logic_vector(118492,18); bit_shifts <=  7;  --  -37 dBFS
      when X"DA"        => multiplier_gain <= conv_std_logic_vector(105606,18); bit_shifts <=  7;  --  -38 dBFS
      when X"D9"        => multiplier_gain <= conv_std_logic_vector( 94122,18); bit_shifts <=  7;  --  -39 dBFS
      when X"D8"        => multiplier_gain <= conv_std_logic_vector( 83886,18); bit_shifts <=  7;  --  -40 dBFS
      when X"D7"        => multiplier_gain <= conv_std_logic_vector( 74764,18); bit_shifts <=  7;  --  -41 dBFS
      when X"D6"        => multiplier_gain <= conv_std_logic_vector( 66633,18); bit_shifts <=  7;  --  -42 dBFS
      when X"D5"        => multiplier_gain <= conv_std_logic_vector(118774,18); bit_shifts <=  8;  --  -43 dBFS
      when X"D4"        => multiplier_gain <= conv_std_logic_vector(105857,18); bit_shifts <=  8;  --  -44 dBFS
      when X"D3"        => multiplier_gain <= conv_std_logic_vector( 94345,18); bit_shifts <=  8;  --  -45 dBFS
      when X"D2"        => multiplier_gain <= conv_std_logic_vector( 84085,18); bit_shifts <=  8;  --  -46 dBFS
      when X"D1"        => multiplier_gain <= conv_std_logic_vector( 74941,18); bit_shifts <=  8;  --  -47 dBFS
      when X"D0"        => multiplier_gain <= conv_std_logic_vector( 66791,18); bit_shifts <=  8;  --  -48 dBFS
      when X"CF"        => multiplier_gain <= conv_std_logic_vector(119056,18); bit_shifts <=  9;  --  -49 dBFS
      when X"CE"        => multiplier_gain <= conv_std_logic_vector(106108,18); bit_shifts <=  9;  --  -50 dBFS
      when X"CD"        => multiplier_gain <= conv_std_logic_vector( 94569,18); bit_shifts <=  9;  --  -51 dBFS
      when X"CC"        => multiplier_gain <= conv_std_logic_vector( 84285,18); bit_shifts <=  9;  --  -52 dBFS
      when X"CB"        => multiplier_gain <= conv_std_logic_vector( 75119,18); bit_shifts <=  9;  --  -53 dBFS
      when X"CA"        => multiplier_gain <= conv_std_logic_vector( 66950,18); bit_shifts <=  9;  --  -54 dBFS
      when X"C9"        => multiplier_gain <= conv_std_logic_vector(119338,18); bit_shifts <= 10;  --  -55 dBFS
      when X"C8"        => multiplier_gain <= conv_std_logic_vector(106360,18); bit_shifts <= 10;  --  -56 dBFS
      when X"C7"        => multiplier_gain <= conv_std_logic_vector( 94794,18); bit_shifts <= 10;  --  -57 dBFS
      when X"C6"        => multiplier_gain <= conv_std_logic_vector( 84485,18); bit_shifts <= 10;  --  -58 dBFS
      when X"C5"        => multiplier_gain <= conv_std_logic_vector( 75297,18); bit_shifts <= 10;  --  -59 dBFS
      when X"C4"        => multiplier_gain <= conv_std_logic_vector( 67109,18); bit_shifts <= 10;  --  -60 dBFS
      when others       => multiplier_gain <= conv_std_logic_vector( 65536,18); bit_shifts <=  0;  --    0 dBFS               
      end case;
   else
      multiplier_gain <= conv_std_logic_vector(0,18); bit_shifts <= 0;
   end if;
end process;                                                               
                                                                                 
multiplier_1 : MULT18X18
port map (
   A  => multiplier_gain,
   B  => audio_i,
   P  => audio_mult
);
                  
                  
audio_bit_shift : process (audio_mult, bit_shifts) begin
   case bit_shifts is                                                      
      when 1      => gained_audio <= to_stdlogicvector(to_bitvector(audio_mult) sra 1);   -- 1  
      when 2      => gained_audio <= to_stdlogicvector(to_bitvector(audio_mult) sra 2);   -- 2  
      when 3      => gained_audio <= to_stdlogicvector(to_bitvector(audio_mult) sra 3);   -- 3  
      when 4      => gained_audio <= to_stdlogicvector(to_bitvector(audio_mult) sra 4);   -- 4  
      when 5      => gained_audio <= to_stdlogicvector(to_bitvector(audio_mult) sra 5);   -- 5  
      when 6      => gained_audio <= to_stdlogicvector(to_bitvector(audio_mult) sra 6);   -- 6  
      when 7      => gained_audio <= to_stdlogicvector(to_bitvector(audio_mult) sra 7);   -- 7  
      when 8      => gained_audio <= to_stdlogicvector(to_bitvector(audio_mult) sra 8);   -- 8  
      when 9      => gained_audio <= to_stdlogicvector(to_bitvector(audio_mult) sra 9);   -- 9  
      when 10     => gained_audio <= to_stdlogicvector(to_bitvector(audio_mult) sra 10);  -- 10  
      when others => gained_audio <= audio_mult;                                          -- 0
   end case;
end process;

audio_o <= gained_audio(33 downto 10);

end behavioral;