library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;

library UNISIM;
use UNISIM.VComponents.all;


entity luma_fir_12bit is
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;
      bits_10_z12_i        : in std_logic;
      
      -- parallel data inputs:
      luma_i               : in std_logic_vector(11 downto 0);

      -- parallel data outputs:
      luma_o               : out std_logic_vector(11 downto 0)
   );
end luma_fir_12bit;


architecture behavioral of luma_fir_12bit is

signal luma_z_1            : std_logic_vector(11 downto 0);
signal luma_z_2            : std_logic_vector(11 downto 0);
signal luma_z_3            : std_logic_vector(11 downto 0);
signal luma_z_4            : std_logic_vector(11 downto 0);
signal reg1                : std_logic_vector(12 downto 0);
signal reg2                : std_logic_vector(12 downto 0);
signal reg3                : std_logic_vector(11 downto 0);
signal factor1             : std_logic_vector(17 downto 0);
signal factor2             : std_logic_vector(17 downto 0);
signal factor3             : std_logic_vector(17 downto 0);

signal reg11               : std_logic_vector(19 downto 0);
signal reg12               : std_logic_vector(22 downto 0);
signal reg13               : std_logic_vector(22 downto 0);

signal prod1               : std_logic_vector(35 downto 0);
signal prod2               : std_logic_vector(35 downto 0);
signal prod3               : std_logic_vector(35 downto 0);

signal reg4                : std_logic_vector(20 downto 0);
signal reg5                : std_logic_vector(23 downto 0);

signal sum4                : std_logic_vector(20 downto 0);
signal sum5                : std_logic_vector(23 downto 0);
signal sum6                : std_logic_vector(23 downto 0);

constant fir1              : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(98,17);
constant fir2              : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(996,17);
constant fir3              : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(1908,17);


component MULT18X18
   port(
      A  : in std_logic_vector(17 downto 0);
	 	B  : in std_logic_vector(17 downto 0);
		P  : out std_logic_vector(35 downto 0)
   );
end component;


begin


---------------------------------------------------------------
-- Delayline of input signal :
---------------------------------------------------------------
luma_delaying : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         luma_z_1 <= luma_i;
         luma_z_2 <= luma_z_1;
         luma_z_3 <= luma_z_2;
         luma_z_4 <= luma_z_3;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Update registers 1 through 3 :
---------------------------------------------------------------
reg_1_to_3_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         reg1 <= ('0' & luma_i) + ('0' & luma_z_4);
         reg2 <= ('0' & luma_z_2) + ('0' & luma_z_4);
         reg3 <= luma_z_4;
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
factor3  <= "000000" & reg3;

multiplier_3 : MULT18X18
port map (
   A  => factor3,
   B  => fir3,
   P  => prod3
);


---------------------------------------------------------------
-- Update registers 11, 12 and 13 :
---------------------------------------------------------------
reg_11_12_13_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         reg11 <= prod1(19 downto 0);     --13bit * 7bit
         reg12 <= prod2(22 downto 0);     --13bit * 10bit
         reg13 <= prod3(22 downto 0);     --12bit * 11bit
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Asyncronious sums :
---------------------------------------------------------------
with bits_10_z12_i select
   sum4 <=  ('0' & reg11) + "000000010000000000000" when '1',     -- 10 bits
            ('0' & reg11) + "000000000100000000000" when others;  -- 12 bits

sum5 <= ('0' & reg12) + ("000" & reg4);
sum6 <= ('0' & reg13) + reg5;


---------------------------------------------------------------
-- Update registers 4, 5 and luma_o :
---------------------------------------------------------------
reg_4_5_luma_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         reg4 <= sum4;
         reg5 <= sum5;
         if bits_10_z12_i = '1' then  -- 10 bits: 
            luma_o <= sum6(23 downto 14) & "00";
         else                       -- 12 bits:
            luma_o <= sum6(23 downto 12);
         end if;
      end if;
   end if;
end process;


end behavioral;