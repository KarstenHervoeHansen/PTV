library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;

library UNISIM;
use UNISIM.VComponents.all;


entity chroma_fir_12bit is
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;

      -- parallel data inputs:
      cr_zcb_i             : in std_logic;
      cb_i                 : in std_logic_vector(11 downto 0);
      cr_i                 : in std_logic_vector(11 downto 0);

      -- parallel data outputs:
      chroma_o             : out std_logic_vector(9 downto 0)
   );
end chroma_fir_12bit;


architecture behavioral of chroma_fir_12bit is

type delay_line is array (0 to 9) of std_logic_vector(11 downto 0);

signal c_upper       : delay_line;
signal c_lower       : delay_line;

signal cr_delayed_1  : std_logic_vector(11 downto 0);

signal reg1          : std_logic_vector(12 downto 0);
signal reg2          : std_logic_vector(12 downto 0);
signal reg3          : std_logic_vector(12 downto 0);
signal reg4          : std_logic_vector(12 downto 0);
signal reg5          : std_logic_vector(11 downto 0);
signal factor1       : std_logic_vector(17 downto 0);
signal factor2       : std_logic_vector(17 downto 0);
signal factor3       : std_logic_vector(17 downto 0);
signal factor4       : std_logic_vector(17 downto 0);
signal factor5       : std_logic_vector(17 downto 0);

signal reg6          : std_logic_vector(18 downto 0);
signal reg7          : std_logic_vector(20 downto 0);
signal reg8          : std_logic_vector(22 downto 0);
signal reg9          : std_logic_vector(23 downto 0);

signal prod1         : std_logic_vector(35 downto 0);
signal prod2         : std_logic_vector(35 downto 0);
signal prod3         : std_logic_vector(35 downto 0);
signal prod4         : std_logic_vector(35 downto 0);
signal prod5         : std_logic_vector(35 downto 0);

signal sum6          : std_logic_vector(18 downto 0);
signal sum7          : std_logic_vector(20 downto 0);
signal sum8          : std_logic_vector(22 downto 0);
signal sum9          : std_logic_vector(23 downto 0);
signal sum10         : std_logic_vector(23 downto 0);

signal reg11         : std_logic_vector(18 downto 0);
signal reg12         : std_logic_vector(20 downto 0);
signal reg13         : std_logic_vector(21 downto 0);
signal reg14         : std_logic_vector(22 downto 0);
signal reg15         : std_logic_vector(22 downto 0);

constant fir1        : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(34,17);
constant fir2        : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(185,17);
constant fir3        : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(492,17);
constant fir4        : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(839,17);
constant fir5        : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(996,17);


component MULT18X18
   port(
      A  : in std_logic_vector(17 downto 0);
	 	B  : in std_logic_vector(17 downto 0);
		P  : out std_logic_vector(35 downto 0)
   );
end component;


begin


---------------------------------------------------------------
-- Delaying cr :
---------------------------------------------------------------
cr_delaying : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         cr_delayed_1 <= cr_i;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Muxing of Cb and Cr :
---------------------------------------------------------------
with cr_zcb_i select
    c_upper(0) <= cr_delayed_1 when '1',
                  cb_i         when others;

with cr_zcb_i select
    c_lower(0) <= cb_i         when '1',
                  cr_delayed_1 when others;


---------------------------------------------------------------
-- Delayline of input signal :
---------------------------------------------------------------
muxed_delay_line : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         for j in 1 to 9 loop
            c_upper(j) <= c_upper(j-1);
            c_lower(j) <= c_lower(j-1);
         end loop;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Update registers 1 through 5 :
---------------------------------------------------------------
reg_1_to_5_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         reg1 <= ('0' & c_lower(0)) + ('0' & c_lower(8));
         reg2 <= ('0' & c_upper(2)) + ('0' & c_upper(8));
         reg3 <= ('0' & c_lower(4)) + ('0' & c_lower(8));
         reg4 <= ('0' & c_upper(6)) + ('0' & c_upper(8));
         reg5 <= c_lower(8);
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Asyncronious multipliers :
---------------------------------------------------------------
-- prod1:
--prod1 <= fir1 * reg1;
factor1  <= "00000" & reg1;

multiplier_1 : MULT18X18
port map (
   A  => factor1,
   B  => fir1,
   P  => prod1
);


-- prod2:
--prod2 <= fir2 * reg2;
factor2  <= "00000" & reg2;

multiplier_2 : MULT18X18
port map (
   A  => factor2,
   B  => fir2,
   P  => prod2
);


-- prod3:
--prod3 <= fir3 * reg3;
factor3  <= "00000" & reg3;

multiplier_3 : MULT18X18
port map (
   A  => factor3,
   B  => fir3,
   P  => prod3
);


-- prod4:
--prod4 <= fir4 * reg4;
factor4  <= "00000" & reg4;

multiplier_4 : MULT18X18
port map (
   A  => factor4,
   B  => fir4,
   P  => prod4
);

-- prod5:
--prod5 <= fir5 * reg5;
factor5  <= "000000" & reg5;

multiplier_5 : MULT18X18
port map (
   A  => factor5,
   B  => fir5,
   P  => prod5
);


---------------------------------------------------------------
-- Update registers 11, 12, 13, 14 and 15 :
---------------------------------------------------------------
reg_11_to_15_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         reg11 <= prod1(18 downto 0);
         reg12 <= prod2(20 downto 0);
         reg13 <= prod3(21 downto 0);
         reg14 <= prod4(22 downto 0);
         reg15 <= prod5(22 downto 0);
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Asyncronious sums :
---------------------------------------------------------------
sum6 <= reg11 + conv_std_logic_vector(2**13,19);       --"00010000000000000";
sum7 <= reg12 + ("00" & reg6);
sum8 <= ('0' & reg13) + ("00" & reg7);
sum9 <= ('0' & reg14) + ('0' & reg8);
sum10 <= ('0' & reg15) + reg9;


---------------------------------------------------------------
-- Update registers 6 through 9 and chroma_o :
---------------------------------------------------------------
reg_6_to_9_chroma_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         reg6     <= sum6;
         reg7     <= sum7;
         reg8     <= sum8;
         reg9     <= sum9;
         chroma_o <= sum10(23 downto 14);
      end if;
   end if;
end process;


end behavioral;