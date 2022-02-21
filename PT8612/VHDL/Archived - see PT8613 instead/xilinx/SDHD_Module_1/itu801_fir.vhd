library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity itu801_fir is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      -- parallel data inputs:
      luma_i               : in std_logic_vector(9 downto 0);
      chroma_i             : in std_logic_vector(9 downto 0);

      -- parallel data outputs:
      luma_o               : out std_logic_vector(9 downto 0);
      chroma_o             : out std_logic_vector(9 downto 0)
   );
end itu801_fir;


architecture behavioral of itu801_fir is

signal luma_z_1            : std_logic_vector(9 downto 0);
signal luma_z_2            : std_logic_vector(9 downto 0);
signal luma_z_3            : std_logic_vector(9 downto 0);
signal luma_z_4            : std_logic_vector(9 downto 0);
signal luma_z_5            : std_logic_vector(9 downto 0);
signal reg1                : std_logic_vector(10 downto 0);
signal reg2                : std_logic_vector(10 downto 0);
signal reg3                : std_logic_vector(10 downto 0);
signal reg4                : std_logic_vector(12 downto 0);
signal reg5                : std_logic_vector(17 downto 0);
signal sum6                : std_logic_vector(19 downto 0);
                           
constant fir2              : std_logic_vector(6 downto 0) := "1101000";
constant fir3              : std_logic_vector(8 downto 0) := "110010110";


begin


---------------------------------------------------------------
-- Dealyline of input signal :
---------------------------------------------------------------
luma_delaying : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      luma_z_1 <= "0001000000";
      luma_z_2 <= "0001000000";
      luma_z_3 <= "0001000000";
      luma_z_4 <= "0001000000";
      luma_z_5 <= "0001000000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         luma_z_1 <= "0001000000";
         luma_z_2 <= "0001000000";
         luma_z_3 <= "0001000000";
         luma_z_4 <= "0001000000";
         luma_z_5 <= "0001000000";
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
      reg1 <= "00010000000";
      reg2 <= "00010000000";
      reg3 <= "00010000000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         reg1 <= "00010000000";
         reg2 <= "00010000000";
         reg3 <= "00010000000";
      elsif en_i = '1' then
         reg1 <= ('0' & luma_i) + ('0' & luma_z_5);
         reg2 <= ('0' & luma_z_2) + ('0' & luma_z_5);
         reg3 <= ('0' & luma_z_4) + ('0' & luma_z_5);
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Update registers 4, 5 and luma_o :
---------------------------------------------------------------
sum6 <= (reg3 * fir3) + ("00" & reg5);

reg_4_5_luma_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      reg4 <= "0001100000000";
      reg5 <= "000011011100000000";
      luma_o <= "0001000000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         reg4 <= "0001100000000";
         reg5 <= "000011011100000000";
         luma_o <= "0001000000";
      elsif en_i = '1' then
         reg4 <= ('0' & reg1 & '0') + "0001000000000";
         reg5 <= (reg2 * fir2) + ("00000" & reg4);
         luma_o <= sum6(19 downto 10);
      end if;
   end if;
end process;


chroma_o <= chroma_i;


end behavioral;