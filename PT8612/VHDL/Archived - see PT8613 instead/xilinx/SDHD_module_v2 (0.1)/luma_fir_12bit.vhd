library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;

library UNISIM;
use UNISIM.VComponents.all;


entity luma_fir_12bit is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      -- parallel data inputs:
      luma_i               : in std_logic_vector(11 downto 0);

      -- parallel data outputs:
      luma_o               : out std_logic_vector(9 downto 0)
   );
end luma_fir_12bit;


architecture behavioral of luma_fir_12bit is

signal luma_z_1            : std_logic_vector(11 downto 0);
signal luma_z_2            : std_logic_vector(11 downto 0);
signal luma_z_3            : std_logic_vector(11 downto 0);
signal luma_z_4            : std_logic_vector(11 downto 0);
signal luma_z_5            : std_logic_vector(11 downto 0);
signal reg1                : std_logic_vector(17 downto 0);
signal reg2                : std_logic_vector(17 downto 0);
signal reg3                : std_logic_vector(17 downto 0);

signal reg11               : std_logic_vector(15 downto 0);
signal reg12               : std_logic_vector(21 downto 0);
signal reg13               : std_logic_vector(23 downto 0);

signal prod1               : std_logic_vector(35 downto 0);
signal prod2               : std_logic_vector(35 downto 0);
signal prod3               : std_logic_vector(35 downto 0);

signal reg4                : std_logic_vector(16 downto 0);
signal reg5                : std_logic_vector(21 downto 0);

signal sum4                : std_logic_vector(16 downto 0);
signal sum5                : std_logic_vector(21 downto 0);
signal sum6                : std_logic_vector(23 downto 0);

constant fir2              : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(415,17);
constant fir3              : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(1625,17);


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
luma_delaying : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      luma_z_1 <= "000100000000";
      luma_z_2 <= "000100000000";
      luma_z_3 <= "000100000000";
      luma_z_4 <= "000100000000";
      luma_z_5 <= "000100000000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         luma_z_1 <= "000100000000";
         luma_z_2 <= "000100000000";
         luma_z_3 <= "000100000000";
         luma_z_4 <= "000100000000";
         luma_z_5 <= "000100000000";
      elsif en_i = '1' then
         luma_z_1 <= luma_i;
         luma_z_2 <= luma_z_1;
         luma_z_3 <= luma_z_2;
         luma_z_4 <= luma_z_3;
         luma_z_5 <= luma_z_4;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Update registers 1 through 3 :
---------------------------------------------------------------
reg_1_to_3_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      reg1 <= "000000001000000000";
      reg2 <= "000000001000000000";
      reg3 <= "000000001000000000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         reg1 <= "000000001000000000";
         reg2 <= "000000001000000000";
         reg3 <= "000000001000000000";
      elsif en_i = '1' then
         reg1 <= "00000" & (('0' & luma_i) + ('0' & luma_z_5));
         reg2 <= "00000" & (('0' & luma_z_2) + ('0' & luma_z_5));
         reg3 <= "00000" & (('0' & luma_z_4) + ('0' & luma_z_5));
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Asyncronious multipliers :
---------------------------------------------------------------
-- prod1:
prod1 <= "000000000000000" & reg1 & "000";   -- reg1 * 8


-- prod2:
--prod2 <= fir2 * reg2;
multiplier_2 : MULT18X18
port map (
   A  => reg2,
   B  => fir2,
   P  => prod2
);


--prod3:
--prod3 <= fir3 * reg3;
multiplier_3 : MULT18X18
port map (
   A  => reg3,
   B  => fir3,
   P  => prod3
);


---------------------------------------------------------------
-- Update registers 11, 12 and 13 :
---------------------------------------------------------------
reg_11_12_13_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      reg11 <= "0001000000000000";
      reg12 <= "0000110011111000000000";
      reg13 <= "000011001011001000000000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
      reg11 <= "0001000000000000";
      reg12 <= "0000110011111000000000";
      reg13 <= "000011001011001000000000";
      elsif en_i = '1' then
         reg11 <= prod1(15 downto 0);
         reg12 <= prod2(21 downto 0);
         reg13 <= prod3(23 downto 0);
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Asyncronious sums :
---------------------------------------------------------------
sum4 <= '0' & reg11 + "00010000000000000";
sum5 <= reg12 + ("00000" & reg4);
sum6 <= reg13 + ("00" & reg5);


---------------------------------------------------------------
-- Update registers 4, 5 and luma_o :
---------------------------------------------------------------
reg_4_5_luma_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      reg4 <= "00011000000000000";
      reg5 <= "0000110110111000000000";
      luma_o <= "0001000000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
      reg4 <= "00011000000000000";
      reg5 <= "0000110110111000000000";
      luma_o <= "0001000000";
      elsif en_i = '1' then
         reg4 <= sum4;
         reg5 <= sum5;
         luma_o <= sum6(23 downto 14);
      end if;
   end if;
end process;


end behavioral;