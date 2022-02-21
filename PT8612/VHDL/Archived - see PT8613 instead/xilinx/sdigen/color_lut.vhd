library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity color_lut is
    Port (
        clk :       in std_logic;
        v_state :   in integer range 0 to 3;
        h_state :   in integer range 0 to 15;
        pattern :   in std_logic_vector(2 downto 0);
        option :    in std_logic_vector(2 downto 0);
        yramp_en :  in std_logic;
        y :         out std_logic_vector(9 downto 0);
        cb :        out std_logic_vector(9 downto 0);
        cr :        out std_logic_vector(9 downto 0)
    );
end color_lut;

architecture Behavioral of color_lut is

signal vert_horz :      std_logic_vector(5 downto 0);
signal v0h2_y,v0h3_y,v0h4_y,v0h5_y,v0h6_y,v0h7_y,v0h8_y,v0h9_y : std_logic_vector(9 downto 0);  -- luminance for vertical state 0
signal v1h2_y,v1h3_y,v1h4_y,v1h5_y,v1h6_y,v1h7_y,v1h8_y,v1h9_y : std_logic_vector(9 downto 0);  -- luminance for vertical state 1
signal v2h2_y,v2h3_y,v2h4_y,v2h5_y,v2h6_y,v2h7_y,v2h8_y,v2h9_y : std_logic_vector(9 downto 0);  -- luminance for vertical state 2
signal v0h3_cb,v0h4_cb,v0h5_cb,v0h6_cb,v0h7_cb,v0h8_cb : std_logic_vector(9 downto 0);          -- chrominance for vertical state 0
signal v1h2_cb,v1h4_cb,v1h6_cb,v1h9_cb : std_logic_vector(9 downto 0);                          -- chrominance for vertical state 1
signal v2h2_cb,v2h4_cb,v2h9_cb : std_logic_vector(9 downto 0);                                  -- chrominance for vertical state 2
signal v0h3_cr,v0h4_cr,v0h5_cr,v0h6_cr,v0h7_cr,v0h8_cr : std_logic_vector(9 downto 0);          -- chrominance for vertical state 0
signal v1h2_cr,v1h4_cr,v1h6_cr,v1h9_cr : std_logic_vector(9 downto 0);                          -- chrominance for vertical state 1
signal v2h2_cr,v2h4_cr,v2h9_cr : std_logic_vector(9 downto 0);                                  -- chrominance for vertical state 2
signal rp219_option2_y,rp219_option3_y : std_logic_vector(9 downto 0);
signal rp219_option2_cb,rp219_option3_cb : std_logic_vector(9 downto 0);
signal rp219_option2_cr,rp219_option3_cr : std_logic_vector(9 downto 0);
signal yramp :          std_logic_vector(9 downto 0) := "0001000000";

begin

make_yramp_for_RP219_TP : process( yramp_en, clk )
begin
    if yramp_en = '0' then
        yramp <= conv_std_logic_vector(64,10);
    elsif clk'event and clk = '1' then
        if yramp /= conv_std_logic_vector(940,10) then
            yramp <= yramp + 1;
        else
            null;
        end if;
    end if;        
end process;

vert_horz <= conv_std_logic_vector(v_state,2) & conv_std_logic_vector(h_state,4);

----------------------------------------------------------------------
-- Pattern table :
--
-- 000 = SDI black
-- 001 = EBU full field colorbar
-- 010 = 100% full field colorbar 
-- 011 = SMPTE colorbar
-- 100 = EBU colorbar + 75% grey split field
-- 101 = 75% colorbar with 40% grey bars at end and start
-- 110 = SMPTE RP219 colorbar
--
-------------------------------------------------------------------------------------------------------------------------------------------
--   vert_horz i parentes
--
--               H1              H2         H3         H4         H5         H6         H7         H8              H9
--          +----------+    +----------+----------+----------+----------+----------+----------+----------+    +----------+
--          |   V0H1   |    |   V0H2   |   V0H3   |   V0H4   |   V0H5   |   V0H6   |   V0H7   |   V0H8   |    |   V0H9   |
--   V0     | (000000) |    | (000001) | (000010) | (000011) | (000100) | (000101) | (000110) | (000111) |    | (001000) |
--          +----------+    +----------+----------+----------+----------+----------+----------+----------+    +----------+
--
--          +----------+    +----------+----------+----------+----------+----------+----------+----------+    +----------+
--          |   V1H1   |    |   V1H2   |   V1H3   |   V1H4   |   V1H5   |   V1H6   |   V1H7   |   V1H8   |    |   V1H9   |
--   V1     | (010000) |    | (010001) | (010010) | (010011) | (010100) | (010101) | (010110) | (010111) |    | (011000) |
--          +----------+    +----------+----------+----------+----------+----------+----------+----------+    +----------+
--
--          +----------+    +----------+----------+----------+----------+----------+----------+----------+    +----------+
--          |   V2H1   |    |   V2H2   |   V2H3   |   V2H4   |   V2H5   |   V2H6   |   V2H7   |   V2H8   |    |   V2H9   |
--   V2     | (100000) |    | (100001) | (100010) | (100011) | (100100) | (100101) | (100110) | (100111) |    | (101000) |
--          +----------+    +----------+----------+----------+----------+----------+----------+----------+    +----------+
--
--                               H2         H3         H4         H5         H6         H7        H10        H11         H8         H9
--          +----------+    +----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+
--          |   V3H1   |    |   V3H2   |   V3H3   |   V3H4   |   V3H5   |   V3H6   |   V3H7   |  V3H10   |  V3H11   |   V3H8   |   V3H9   |
--   V3     | (110000) |    | (110001) | (110010) | (110011) | (110100) | (110101) | (110110) | (111001) | (111010) | (110111) | (111000) |
--          +----------+    +----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+
--          
-------------------------------------------------------------------------------------------------------------------------------------------
-- color matrix for luminance :
with vert_horz select
    y <= conv_std_logic_vector(414,10) when "000001",   -- 40% grey
         v0h2_y                        when "000010",
         v0h3_y                        when "000011",
         v0h4_y                        when "000100",
         v0h5_y                        when "000101",
         v0h6_y                        when "000110",
         v0h7_y                        when "000111",
         v0h8_y                        when "001000",
         v0h9_y                        when "001001",

         conv_std_logic_vector(754,10) when "010001",   -- 100% cyan
         v1h2_y                        when "010010",
         v1h3_y                        when "010011",
         v1h4_y                        when "010100",
         v1h5_y                        when "010101",
         v1h6_y                        when "010110",
         v1h7_y                        when "010111",
         v1h8_y                        when "011000",
         v1h9_y                        when "011001",

         conv_std_logic_vector(877,10) when "100001",   -- 100% yellow
         v2h2_y                        when "100010",
         v2h3_y                        when "100011",
         v2h4_y                        when "100100",
         v2h5_y                        when "100101",
         v2h6_y                        when "100110",
         v2h7_y                        when "100111",
         v2h8_y                        when "101000",
         v2h9_y                        when "101001",

         conv_std_logic_vector(195,10) when "110001",   -- 15% grey
         conv_std_logic_vector(940,10) when "110011",   -- 100% white
         conv_std_logic_vector(46,10)  when "110101",   -- -2% black
         conv_std_logic_vector(82,10)  when "110111",   -- +2% black
         conv_std_logic_vector(195,10) when "111001",   -- 15% grey
         conv_std_logic_vector(99,10)  when "111011",   -- +4% black

         conv_std_logic_vector(64,10)  when others;

-------------------------------------------------------------------------------------------------------------------------------------------
-- decode pattern input for luminance :

with pattern select
    v0h2_y <= conv_std_logic_vector(721,10) when "001", -- 75% white
              conv_std_logic_vector(940,10) when "010", -- 100% white
              conv_std_logic_vector(721,10) when "011",
              conv_std_logic_vector(721,10) when "100",
              conv_std_logic_vector(721,10) when "101",
              conv_std_logic_vector(721,10) when "110",
              conv_std_logic_vector(64,10) when others; -- 0% black
with pattern select
    v0h3_y <= conv_std_logic_vector(674,10) when "001", -- 75% yellow
              conv_std_logic_vector(877,10) when "010", -- 100% yellow
              conv_std_logic_vector(674,10) when "011",
              conv_std_logic_vector(674,10) when "100",
              conv_std_logic_vector(674,10) when "101",
              conv_std_logic_vector(674,10) when "110",
              conv_std_logic_vector(64,10) when others; -- 0% black
with pattern select
    v0h4_y <= conv_std_logic_vector(581,10) when "001", -- 75% cyan
              conv_std_logic_vector(754,10) when "010", -- 100% cyan
              conv_std_logic_vector(581,10) when "011",
              conv_std_logic_vector(581,10) when "100",
              conv_std_logic_vector(581,10) when "101",
              conv_std_logic_vector(581,10) when "110",
              conv_std_logic_vector(64,10) when others; -- 0% black
with pattern select
    v0h5_y <= conv_std_logic_vector(534,10) when "001", -- 75% green
              conv_std_logic_vector(691,10) when "010", -- 100% green (kendes ikke pt)
              conv_std_logic_vector(534,10) when "011",
              conv_std_logic_vector(534,10) when "100",
              conv_std_logic_vector(534,10) when "101",
              conv_std_logic_vector(534,10) when "110",
              conv_std_logic_vector(64,10) when others; -- 0% black
with pattern select
    v0h6_y <= conv_std_logic_vector(251,10) when "001", -- 75% magenta
              conv_std_logic_vector(313,10) when "010", -- 100% magenta (kendes ikke pt)
              conv_std_logic_vector(251,10) when "011",
              conv_std_logic_vector(251,10) when "100",
              conv_std_logic_vector(251,10) when "101",
              conv_std_logic_vector(251,10) when "110",
              conv_std_logic_vector(64,10) when others; -- 0% black
with pattern select
    v0h7_y <= conv_std_logic_vector(204,10) when "001", -- 75% red
              conv_std_logic_vector(250,10) when "010", -- 100% red
              conv_std_logic_vector(204,10) when "011",
              conv_std_logic_vector(204,10) when "100",
              conv_std_logic_vector(204,10) when "101",
              conv_std_logic_vector(204,10) when "110",
              conv_std_logic_vector(64,10) when others; -- 0% black
with pattern select
    v0h8_y <= conv_std_logic_vector(111,10) when "001", -- 75% blue
              conv_std_logic_vector(127,10) when "010", -- 100% blue
              conv_std_logic_vector(111,10) when "011",
              conv_std_logic_vector(111,10) when "100",
              conv_std_logic_vector(111,10) when "101",
              conv_std_logic_vector(111,10) when "110",
              conv_std_logic_vector(64,10) when others; -- 0% black
with pattern select
    v0h9_y <= conv_std_logic_vector(64,10) when "001",  -- 0% black
              conv_std_logic_vector(64,10) when "010",
              conv_std_logic_vector(64,10) when "011",
              conv_std_logic_vector(64,10) when "100",
              conv_std_logic_vector(414,10) when "101",
              conv_std_logic_vector(414,10) when "110", -- 40% GREY
              conv_std_logic_vector(64,10) when others;
with pattern select
    v1h2_y <= conv_std_logic_vector(127,10) when "011",  -- 100% blue
              conv_std_logic_vector(721,10) when "100",  -- 75% white
              rp219_option2_y               when "110",              
              conv_std_logic_vector(64,10)  when others;              
with pattern select
    v1h3_y <= conv_std_logic_vector(721,10) when "100", -- 75% white
              conv_std_logic_vector(721,10) when "110", -- 75% white
              conv_std_logic_vector(64,10)  when others;-- 0% black
with pattern select
    v1h4_y <= conv_std_logic_vector(313,10) when "011", -- 100% magenta (kendes ikke pt)
              conv_std_logic_vector(721,10) when "100", -- 75% white
              conv_std_logic_vector(721,10) when "110", -- 75% white
              conv_std_logic_vector(64,10) when others; -- 0% black
with pattern select
    v1h5_y <= conv_std_logic_vector(721,10) when "100", -- 75% white
              conv_std_logic_vector(721,10) when "110", -- 75% white
              conv_std_logic_vector(64,10) when others; -- 0% black
with pattern select
    v1h6_y <= conv_std_logic_vector(754,10) when "011", -- 100% cyan
              conv_std_logic_vector(721,10) when "100", -- 75% white
              conv_std_logic_vector(721,10) when "110", -- 75% white
              conv_std_logic_vector(64,10) when others; -- 0% black
with pattern select                                     
    v1h7_y <= conv_std_logic_vector(721,10) when "100", -- 75% white
              conv_std_logic_vector(721,10) when "110", -- 75% white
              conv_std_logic_vector(64,10) when others;
with pattern select
    v1h8_y <= conv_std_logic_vector(721,10) when "100", -- 75% white
              conv_std_logic_vector(721,10) when "110", -- 75% white
              conv_std_logic_vector(721,10) when "011", -- 75% white
              conv_std_logic_vector(64,10) when others;
with pattern select
    v1h9_y <= conv_std_logic_vector(111,10) when "110",  -- 100% blue
              conv_std_logic_vector(721,10) when others; -- 75% white
with pattern select
    v2h2_y <= conv_std_logic_vector(244,10) when "011", -- -I
              rp219_option3_y               when "110", -- 75% grey
              conv_std_logic_vector(64,10)  when others;-- 0% black
with pattern select
    v2h3_y <= conv_std_logic_vector(940,10) when "011", -- 100% white
              yramp                         when "110",
              conv_std_logic_vector(64,10)  when others;-- 0% black
with pattern select
    v2h4_y <= conv_std_logic_vector(141,10) when "011",  -- +Q
              yramp                         when "110",
              conv_std_logic_vector(64,10)  when others;  -- 0% black
with pattern select
    v2h5_y <= yramp                        when "110",
              conv_std_logic_vector(64,10) when others;  -- 0% black
with pattern select
    v2h6_y <= conv_std_logic_vector(29,10) when "011",   -- -4% black
              yramp                        when "110",
              conv_std_logic_vector(64,10) when others;  -- 0% black
with pattern select
    v2h7_y <= yramp                        when "110",
              conv_std_logic_vector(64,10) when others;  -- 0% black
with pattern select
    v2h8_y <= conv_std_logic_vector(99,10) when "011",   -- +4% black
              yramp                        when "110",
              conv_std_logic_vector(64,10) when others;  -- 0% black
with pattern select
    v2h9_y <= conv_std_logic_vector(250,10) when "110",  -- 100% red
              conv_std_logic_vector(64,10) when others;  -- 0% black

-------------------------------------------------------------------------------------------------------------------------------------------
-- color matrix for blue chrominance :

with vert_horz select
    cb <= v0h3_cb                       when "000011",
          v0h4_cb                       when "000100",
          v0h5_cb                       when "000101",
          v0h6_cb                       when "000110",
          v0h7_cb                       when "000111",
          v0h8_cb                       when "001000",

          conv_std_logic_vector(615,10) when "010001",   -- 100% cyan
          v1h2_cb                       when "010010",
          v1h4_cb                       when "010100",
          v1h6_cb                       when "010110",
          v1h9_cb                       when "011001",

          conv_std_logic_vector(64,10)  when "100001",   -- 100% yellow
          v2h2_cb                       when "100010",
          v2h4_cb                       when "100100",
          v2h9_cb                       when "101001",

          conv_std_logic_vector(512,10) when others;

-------------------------------------------------------------------------------------------------------------------------------------------
-- decode pattern input for blue chrominance :

with pattern select
    v0h3_cb <= conv_std_logic_vector(176,10) when "001", -- 75% yellow
               conv_std_logic_vector(64,10) when "010", -- 100% yellow
               conv_std_logic_vector(176,10) when "011",
               conv_std_logic_vector(176,10) when "100",
               conv_std_logic_vector(176,10) when "101",
               conv_std_logic_vector(176,10) when "110",
               conv_std_logic_vector(512,10) when others; -- greyscale
with pattern select
    v0h4_cb <= conv_std_logic_vector(589,10) when "001", -- 75% cyan
               conv_std_logic_vector(615,10) when "010", -- 100% cyan
               conv_std_logic_vector(589,10) when "011",
               conv_std_logic_vector(589,10) when "100",
               conv_std_logic_vector(589,10) when "101",
               conv_std_logic_vector(589,10) when "110",
               conv_std_logic_vector(512,10) when others; -- greyscale
with pattern select
    v0h5_cb <= conv_std_logic_vector(253,10) when "001", -- 75% green
               conv_std_logic_vector(167,10) when "010", -- 100% green (kendes ikke pt)
               conv_std_logic_vector(253,10) when "011",
               conv_std_logic_vector(253,10) when "100",
               conv_std_logic_vector(253,10) when "101",
               conv_std_logic_vector(253,10) when "110",
               conv_std_logic_vector(512,10) when others; -- greyscale
with pattern select
    v0h6_cb <= conv_std_logic_vector(771,10) when "001", -- 75% magenta
               conv_std_logic_vector(857,10) when "010", -- 100% magenta (kendes ikke pt)
               conv_std_logic_vector(771,10) when "011",
               conv_std_logic_vector(771,10) when "100",
               conv_std_logic_vector(771,10) when "101",
               conv_std_logic_vector(771,10) when "110",
               conv_std_logic_vector(512,10) when others; -- greyscale
with pattern select
    v0h7_cb <= conv_std_logic_vector(435,10) when "001", -- 75% red
               conv_std_logic_vector(409,10) when "010", -- 100% red
               conv_std_logic_vector(435,10) when "011",
               conv_std_logic_vector(435,10) when "100",
               conv_std_logic_vector(435,10) when "101",
               conv_std_logic_vector(435,10) when "110",
               conv_std_logic_vector(512,10) when others; -- greyscale
with pattern select
    v0h8_cb <= conv_std_logic_vector(848,10) when "001", -- 75% blue
               conv_std_logic_vector(960,10) when "010", -- 100% blue
               conv_std_logic_vector(848,10) when "011",
               conv_std_logic_vector(848,10) when "100",
               conv_std_logic_vector(848,10) when "101",
               conv_std_logic_vector(848,10) when "110",
               conv_std_logic_vector(512,10) when others; -- greyscale
with pattern select
    v1h2_cb <= conv_std_logic_vector(960,10) when "011",  -- 100% blue
               rp219_option2_cb               when "110",              
               conv_std_logic_vector(512,10)  when others;              
with pattern select
    v1h4_cb <= conv_std_logic_vector(857,10) when "011",   -- 100% magenta (kendes ikke pt)
               conv_std_logic_vector(512,10) when others; -- greyscale
with pattern select
    v1h6_cb <= conv_std_logic_vector(615,10) when "011",  -- 100% cyan
               conv_std_logic_vector(512,10) when others; -- greyscale
with pattern select
    v1h9_cb <= conv_std_logic_vector(960,10) when "110",   -- 100% blue
               conv_std_logic_vector(512,10) when others;
with pattern select
    v2h2_cb <= conv_std_logic_vector(612,10) when "011", -- -I
               rp219_option3_cb               when "110",
               conv_std_logic_vector(512,10)  when others;-- greyscale
with pattern select
    v2h4_cb <= conv_std_logic_vector(697,10) when "011",  -- +Q
               conv_std_logic_vector(512,10)  when others;  -- greyscale
with pattern select
    v2h9_cb <= conv_std_logic_vector(409,10) when "110",  -- 100% red
               conv_std_logic_vector(512,10) when others;  -- greyscale

-------------------------------------------------------------------------------------------------------------------------------------------
-- color matrix for red chrominance :

with vert_horz select
    cr <= v0h3_cr                       when "000011",
          v0h4_cr                       when "000100",
          v0h5_cr                       when "000101",
          v0h6_cr                       when "000110",
          v0h7_cr                       when "000111",
          v0h8_cr                       when "001000",

          conv_std_logic_vector(64,10) when "010001",   -- 100% cyan
          v1h2_cr                       when "010010",
          v1h4_cr                       when "010100",
          v1h6_cr                       when "010110",
          v1h9_cr                       when "011001",

          conv_std_logic_vector(553,10)  when "100001",   -- 100% yellow
          v2h2_cr                       when "100010",
          v2h4_cr                       when "100100",
          v2h9_cr                       when "101001",

          conv_std_logic_vector(512,10) when others;

-------------------------------------------------------------------------------------------------------------------------------------------
-- decode pattern input for red chrominance :

with pattern select
    v0h3_cr <= conv_std_logic_vector(543,10) when "001", -- 75% yellow
               conv_std_logic_vector(553,10) when "010", -- 100% yellow
               conv_std_logic_vector(543,10) when "011",
               conv_std_logic_vector(543,10) when "100",
               conv_std_logic_vector(543,10) when "101",
               conv_std_logic_vector(543,10) when "110",
               conv_std_logic_vector(512,10) when others; -- greyscale
with pattern select
    v0h4_cr <= conv_std_logic_vector(176,10) when "001", -- 75% cyan
               conv_std_logic_vector(64,10) when "010", -- 100% cyan
               conv_std_logic_vector(176,10) when "011",
               conv_std_logic_vector(176,10) when "100",
               conv_std_logic_vector(176,10) when "101",
               conv_std_logic_vector(176,10) when "110",
               conv_std_logic_vector(512,10) when others; -- greyscale
with pattern select
    v0h5_cr <= conv_std_logic_vector(207,10) when "001", -- 75% green
               conv_std_logic_vector(105,10) when "010", -- 100% green (kendes ikke pt)
               conv_std_logic_vector(207,10) when "011",
               conv_std_logic_vector(207,10) when "100",
               conv_std_logic_vector(207,10) when "101",
               conv_std_logic_vector(207,10) when "110",
               conv_std_logic_vector(512,10) when others; -- greyscale
with pattern select
    v0h6_cr <= conv_std_logic_vector(817,10) when "001", -- 75% magenta
               conv_std_logic_vector(919,10) when "010", -- 100% magenta (kendes ikke pt)
               conv_std_logic_vector(817,10) when "011",
               conv_std_logic_vector(817,10) when "100",
               conv_std_logic_vector(817,10) when "101",
               conv_std_logic_vector(817,10) when "110",
               conv_std_logic_vector(512,10) when others; -- greyscale
with pattern select
    v0h7_cr <= conv_std_logic_vector(848,10) when "001", -- 75% red
               conv_std_logic_vector(960,10) when "010", -- 100% red
               conv_std_logic_vector(848,10) when "011",
               conv_std_logic_vector(848,10) when "100",
               conv_std_logic_vector(848,10) when "101",
               conv_std_logic_vector(848,10) when "110",
               conv_std_logic_vector(512,10) when others; -- greyscale
with pattern select
    v0h8_cr <= conv_std_logic_vector(481,10) when "001", -- 75% blue
               conv_std_logic_vector(471,10) when "010", -- 100% blue
               conv_std_logic_vector(481,10) when "011",
               conv_std_logic_vector(481,10) when "100",
               conv_std_logic_vector(481,10) when "101",
               conv_std_logic_vector(481,10) when "110",
               conv_std_logic_vector(512,10) when others; -- greyscale
with pattern select
    v1h2_cr <= conv_std_logic_vector(471,10) when "011",  -- 100% blue
               rp219_option2_cr               when "110",              
               conv_std_logic_vector(512,10)  when others;              
with pattern select
    v1h4_cr <= conv_std_logic_vector(919,10) when "011",   -- 100% magenta (kendes ikke pt)
               conv_std_logic_vector(512,10) when others; -- greyscale
with pattern select
    v1h6_cr <= conv_std_logic_vector(64,10) when "011",  -- 100% cyan
               conv_std_logic_vector(512,10) when others; -- greyscale
with pattern select
    v1h9_cr <= conv_std_logic_vector(471,10) when "110",   -- 100% blue
               conv_std_logic_vector(512,10) when others;
with pattern select
    v2h2_cr <= conv_std_logic_vector(395,10) when "011", -- -I
               rp219_option3_cr               when "110",
               conv_std_logic_vector(512,10)  when others;-- greyscale
with pattern select
    v2h4_cr <= conv_std_logic_vector(606,10) when "011",  -- +Q
               conv_std_logic_vector(512,10)  when others;  -- greyscale
with pattern select
    v2h9_cr <= conv_std_logic_vector(960,10) when "110",  -- 100% red
               conv_std_logic_vector(512,10) when others;  -- greyscale


-------------------------------------------------------------------------------------------------------------------------------------------
-- decode option register :

-- decode OPTION *2 :
with option select
    rp219_option2_y <= conv_std_logic_vector(721,10) when "000",   -- 75% white
                       conv_std_logic_vector(940,10) when "001",   -- 100% white
                       conv_std_logic_vector(245,10) when "010",   -- +I
                       conv_std_logic_vector(244,10) when "011",   -- -I
                       conv_std_logic_vector(721,10) when "100",   -- 75% white
                       conv_std_logic_vector(940,10) when "101",   -- 100% white
                       conv_std_logic_vector(245,10) when "110",   -- +I
                       conv_std_logic_vector(244,10) when "111",   -- -I
                       conv_std_logic_vector(64,10) when others;-- 0% black
with option select
    rp219_option2_cb <= conv_std_logic_vector(412,10) when "010",  -- +I
                        conv_std_logic_vector(612,10) when "011",  -- -I
                        conv_std_logic_vector(412,10) when "110",  -- +I
                        conv_std_logic_vector(612,10) when "111",  -- -I
                        conv_std_logic_vector(512,10) when others;  -- greyscale
with option select
    rp219_option2_cr <= conv_std_logic_vector(629,10) when "010",  -- +I
                        conv_std_logic_vector(395,10) when "011",  -- -I
                        conv_std_logic_vector(629,10) when "110",  -- +I
                        conv_std_logic_vector(395,10) when "111",  -- -I
                        conv_std_logic_vector(512,10) when others;  -- greyscale

-- OPTION *3 :
with option select
    rp219_option3_y <= conv_std_logic_vector(141,10) when "100",    -- +Q
                       conv_std_logic_vector(141,10) when "101",
                       conv_std_logic_vector(141,10) when "110",
                       conv_std_logic_vector(141,10) when "111",
                       conv_std_logic_vector(64,10) when others;    -- 0% black
with option select
    rp219_option3_cb <= conv_std_logic_vector(697,10) when "100",   -- +Q
                        conv_std_logic_vector(697,10) when "101",
                        conv_std_logic_vector(697,10) when "110",
                        conv_std_logic_vector(697,10) when "111",
                        conv_std_logic_vector(512,10) when others;  -- greyscale
with option select
    rp219_option3_cr <= conv_std_logic_vector(606,10) when "100",   -- +Q
                        conv_std_logic_vector(606,10) when "101",
                        conv_std_logic_vector(606,10) when "110",
                        conv_std_logic_vector(606,10) when "111",
                        conv_std_logic_vector(512,10) when others;  -- greyscale


end Behavioral;