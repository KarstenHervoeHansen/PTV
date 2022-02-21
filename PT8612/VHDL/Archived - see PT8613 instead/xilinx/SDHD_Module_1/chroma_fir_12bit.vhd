library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity chroma_fir_12bit is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      -- parallel data inputs:
      cb_zcr_i             : in std_logic;
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
signal reg5          : std_logic_vector(12 downto 0);



signal reg6          : std_logic_vector(16 downto 0);
signal reg7          : std_logic_vector(19 downto 0);
signal reg8          : std_logic_vector(21 downto 0);
signal reg9          : std_logic_vector(23 downto 0);
signal sum10         : std_logic_vector(23 downto 0);
                           
constant fir2        : std_logic_vector(6 downto 0) := conv_std_logic_vector(92,7);
constant fir3        : std_logic_vector(8 downto 0) := conv_std_logic_vector(323,9);
constant fir4        : std_logic_vector(9 downto 0) := conv_std_logic_vector(674,10);
constant fir5        : std_logic_vector(9 downto 0) := conv_std_logic_vector(951,10);


begin


---------------------------------------------------------------
-- Delaying cr :
---------------------------------------------------------------
cr_delaying : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      cr_delayed_1 <= "100000000000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         cr_delayed_1 <= "100000000000";
      elsif en_i = '1' then
         cr_delayed_1 <= cr_i;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Muxing of Cb and Cr :
---------------------------------------------------------------
with cb_zcr_i select
    c_upper(0) <= cr_delayed_1 when '1',
                  cb_i         when others;

with cb_zcr_i select                    
    c_lower(0) <= cb_i         when '1',                  
                  cr_delayed_1 when others;               


---------------------------------------------------------------
-- Delayline of input signal :
---------------------------------------------------------------
muxed_delay_line : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      c_upper(1 to 9) <= (others => "100000000000");
      c_lower(1 to 9) <= (others => "100000000000");
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         c_upper(1 to 9) <= (others => "100000000000");
         c_lower(1 to 9) <= (others => "100000000000");
      elsif en_i = '1' then
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
reg_1_to_5_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      reg1 <= "1000000000000";
      reg2 <= "1000000000000";
      reg3 <= "1000000000000";
      reg4 <= "1000000000000";
      reg5 <= "1000000000000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         reg1 <= "1000000000000";
         reg2 <= "1000000000000";
         reg3 <= "1000000000000";
         reg4 <= "1000000000000";
         reg5 <= "1000000000000";
      elsif en_i = '1' then
         reg1 <= ('0' & c_lower(0)) + ('0' & c_upper(9));
         reg2 <= ('0' & c_upper(2)) + ('0' & c_lower(9));
         reg3 <= ('0' & c_lower(4)) + ('0' & c_upper(9));
         reg4 <= ('0' & c_upper(6)) + ('0' & c_lower(9));
         reg5 <= ('0' & c_lower(8)) + ('0' & c_upper(9));
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Update registers 6 through 9 and chroma_o :
---------------------------------------------------------------
sum10 <= (reg5 * fir5) + reg9;

reg_6_to_9_chroma_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      reg6     <= conv_std_logic_vector((2**11)*2*8+(2**13),17);
      reg7     <= conv_std_logic_vector((2**11)*2*(8+conv_integer(fir2))+(2**13),20);
      reg8     <= conv_std_logic_vector((2**11)*2*(8+conv_integer(fir2)+conv_integer(fir3))+(2**13),22);
      reg9     <= conv_std_logic_vector((2**11)*2*(8+conv_integer(fir2)+conv_integer(fir3)+conv_integer(fir4))+(2**13),24);
      chroma_o <= "1000000000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
      reg6     <= conv_std_logic_vector((2**11)*2*8+(2**13),17);
      reg7     <= conv_std_logic_vector((2**11)*2*(8+conv_integer(fir2))+(2**13),20);
      reg8     <= conv_std_logic_vector((2**11)*2*(8+conv_integer(fir2)+conv_integer(fir3))+(2**13),22);
      reg9     <= conv_std_logic_vector((2**11)*2*(8+conv_integer(fir2)+conv_integer(fir3)+conv_integer(fir4))+(2**13),24);
      chroma_o <= "1000000000";
      elsif en_i = '1' then
         reg6 <= ('0' & reg1 & "000") + "00010000000000000";
         reg7 <= (reg2 * fir2) + ("000" & reg6);
         reg8 <= (reg3 * fir3) + ("00" & reg7);
         reg9 <= (reg4 * fir4) + ("00" & reg8);
         chroma_o <= sum10(23 downto 14);
      end if;
   end if;
end process;


end behavioral;