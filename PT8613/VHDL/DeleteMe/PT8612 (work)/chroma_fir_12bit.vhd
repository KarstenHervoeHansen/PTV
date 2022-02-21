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
      bits_10_z12_i        : in std_logic;
      samp_422_z444_i      : in std_logic;

      -- parallel data inputs:
      cr_zcb_i             : in std_logic;
      cb_i                 : in std_logic_vector(11 downto 0);
      cr_i                 : in std_logic_vector(11 downto 0);

      -- parallel data outputs:
      chroma_a_o           : out std_logic_vector(9 downto 0);
      chroma_b_o           : out std_logic_vector(9 downto 0);
      cb_lsb_o             : out std_logic_vector(1 downto 0);
      cr_lsb_o             : out std_logic_vector(1 downto 0)
   );
end chroma_fir_12bit;


architecture behavioral of chroma_fir_12bit is

type delay_line is array (0 to 9) of std_logic_vector(11 downto 0);

signal c_upper       : delay_line;
signal c_lower       : delay_line;
signal lsb_b         : std_logic_vector(1 downto 0);
signal cr_delayed_1  : std_logic_vector(11 downto 0);
signal reg1a         : std_logic_vector(12 downto 0);
signal reg2a         : std_logic_vector(12 downto 0);
signal reg3a         : std_logic_vector(12 downto 0);
signal reg4a         : std_logic_vector(12 downto 0);
signal reg5a         : std_logic_vector(11 downto 0);
signal factor1a      : std_logic_vector(17 downto 0);
signal factor2a      : std_logic_vector(17 downto 0);
signal factor3a      : std_logic_vector(17 downto 0);
signal factor4a      : std_logic_vector(17 downto 0);
signal factor5a      : std_logic_vector(17 downto 0);
signal reg6a         : std_logic_vector(18 downto 0);
signal reg7a         : std_logic_vector(20 downto 0);
signal reg8a         : std_logic_vector(22 downto 0);
signal reg9a         : std_logic_vector(23 downto 0);
signal prod1a        : std_logic_vector(35 downto 0);
signal prod2a        : std_logic_vector(35 downto 0);
signal prod3a        : std_logic_vector(35 downto 0);
signal prod4a        : std_logic_vector(35 downto 0);
signal prod5a        : std_logic_vector(35 downto 0);
signal sum6a         : std_logic_vector(18 downto 0);
signal sum7a         : std_logic_vector(20 downto 0);
signal sum8a         : std_logic_vector(22 downto 0);
signal sum9a         : std_logic_vector(23 downto 0);
signal sum10a        : std_logic_vector(23 downto 0);
signal reg11a        : std_logic_vector(18 downto 0);
signal reg12a        : std_logic_vector(20 downto 0);
signal reg13a        : std_logic_vector(21 downto 0);
signal reg14a        : std_logic_vector(22 downto 0);
signal reg15a        : std_logic_vector(22 downto 0);
signal reg3b         : std_logic_vector(12 downto 0);
signal reg4b         : std_logic_vector(12 downto 0);
signal reg5b         : std_logic_vector(11 downto 0);
signal factor3b      : std_logic_vector(17 downto 0);
signal factor4b      : std_logic_vector(17 downto 0);
signal factor5b      : std_logic_vector(17 downto 0);
signal prod3b        : std_logic_vector(35 downto 0);
signal prod4b        : std_logic_vector(35 downto 0);
signal prod5b        : std_logic_vector(35 downto 0);
signal reg13b        : std_logic_vector(21 downto 0);
signal reg14b        : std_logic_vector(22 downto 0);
signal reg15b        : std_logic_vector(22 downto 0);
signal sum8b         : std_logic_vector(22 downto 0);
signal sum9b         : std_logic_vector(23 downto 0);
signal sum10b        : std_logic_vector(23 downto 0);
signal reg8b         : std_logic_vector(22 downto 0);
signal reg9b         : std_logic_vector(23 downto 0);
signal fir1          : std_logic_vector(17 downto 0);
signal fir2          : std_logic_vector(17 downto 0);
signal fir3          : std_logic_vector(17 downto 0);
signal fir4          : std_logic_vector(17 downto 0);
signal fir5          : std_logic_vector(17 downto 0);

constant fir1_slow   : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(34,17);
constant fir2_slow   : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(185,17);
constant fir3_slow   : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(492,17);
constant fir4_slow   : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(839,17);
constant fir5_slow   : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(996,17);

constant fir1_fast   : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(0,17);
constant fir2_fast   : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(0,17);
constant fir3_fast   : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(98,17);
constant fir4_fast   : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(996,17);
constant fir5_fast   : std_logic_vector(17 downto 0) := '0' & conv_std_logic_vector(1908,17);


component MULT18X18
   port(
      A  : in std_logic_vector(17 downto 0);
	 	B  : in std_logic_vector(17 downto 0);
		P  : out std_logic_vector(35 downto 0)
   );
end component;


begin

---------------------------------------------------------------
-- Select fir coefficients :
---------------------------------------------------------------
fir_selecting: process(samp_422_z444_i)
begin
   if samp_422_z444_i = '1' then -- 4:2:2:
      fir1 <= fir1_slow;
      fir2 <= fir2_slow;
      fir3 <= fir3_slow;
      fir4 <= fir4_slow;
      fir5 <= fir5_slow;
   else                          -- 4:4:4:
      fir1 <= fir1_fast;
      fir2 <= fir2_fast;
      fir3 <= fir3_fast;
      fir4 <= fir4_fast;
      fir5 <= fir5_fast;
   end if;

end process;   


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
-- Update registers 1a through 5a :
---------------------------------------------------------------
reg_1a_to_5a_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         reg1a <= ('0' & c_lower(0)) + ('0' & c_lower(8));
         reg2a <= ('0' & c_upper(2)) + ('0' & c_upper(8));
         reg3a <= ('0' & c_lower(4)) + ('0' & c_lower(8));
         reg4a <= ('0' & c_upper(6)) + ('0' & c_upper(8));
         reg5a <= c_lower(8);
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Asyncronious multipliers :
---------------------------------------------------------------
-- prod1a:
--prod1a <= fir1a * reg1a;
factor1a  <= "00000" & reg1a;

multiplier_1a : MULT18X18
port map (
   A  => factor1a,
   B  => fir1,
   P  => prod1a
);


-- prod2a:
--prod2a <= fir2a * reg2a;
factor2a  <= "00000" & reg2a;

multiplier_2a : MULT18X18
port map (
   A  => factor2a,
   B  => fir2,
   P  => prod2a
);


-- prod3a:
--prod3a <= fir3a * reg3a;
factor3a  <= "00000" & reg3a;

multiplier_3a : MULT18X18
port map (
   A  => factor3a,
   B  => fir3,
   P  => prod3a
);


-- prod4a:
--prod4a <= fir4a * reg4a;
factor4a  <= "00000" & reg4a;

multiplier_4a : MULT18X18
port map (
   A  => factor4a,
   B  => fir4,
   P  => prod4a
);

-- prod5a:
--prod5a <= fir5a * reg5a;
factor5a  <= "000000" & reg5a;

multiplier_5a : MULT18X18
port map (
   A  => factor5a,
   B  => fir5,
   P  => prod5a
);


---------------------------------------------------------------
-- Update registers 11a, 12a, 13a, 14a and 15a :
---------------------------------------------------------------
reg_11a_to_15a_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         reg11a <= prod1a(18 downto 0);
         reg12a <= prod2a(20 downto 0);
         reg13a <= prod3a(21 downto 0);
         reg14a <= prod4a(22 downto 0);
         reg15a <= prod5a(22 downto 0);
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Asyncronious sums :
---------------------------------------------------------------
with bits_10_z12_i select
   sum6a    <= reg11a + conv_std_logic_vector(2**13,19) when '1',     -- 10 bits
               reg11a + conv_std_logic_vector(2**11,19) when others;  -- 12 bits

   sum7a    <= reg12a + ("00" & reg6a);
   sum8a    <= ('0' & reg13a) + ("00" & reg7a);
   sum9a    <= ('0' & reg14a) + ('0' & reg8a);
   sum10a   <= ('0' & reg15a) + reg9a;


---------------------------------------------------------------
-- Update registers 6a through 9a and chroma_a_o :
---------------------------------------------------------------
reg_6a_to_9a_chroma_a_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         reg6a       <= sum6a;
         reg7a       <= sum7a;
         reg8a       <= sum8a;
         reg9a       <= sum9a;
         chroma_a_o  <= sum10a(23 downto 14);
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Filter for channel B :
---------------------------------------------------------------

---------------------------------------------------------------
-- Update registers 3b through 5b :
---------------------------------------------------------------
reg_3b_to_5b_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         reg3b <= ('0' & c_upper(3)) + ('0' & c_upper(7));
         reg4b <= ('0' & c_lower(5)) + ('0' & c_lower(7));
         reg5b <= c_upper(7);
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Asyncronious multipliers :
---------------------------------------------------------------
-- prod3b:
--prod3b <= fir3b * reg3b;
factor3b  <= "00000" & reg3b;

multiplier_3b : MULT18X18
port map (
   A  => factor3b,
   B  => fir3_fast,
   P  => prod3b
);


-- prod4b:
--prod4b <= fir4b * reg4b;
factor4b  <= "00000" & reg4b;

multiplier_4b : MULT18X18
port map (
   A  => factor4b,
   B  => fir4_fast,
   P  => prod4b
);

-- prod5b:
--prod5b <= fir5b * reg5b;
factor5b  <= "000000" & reg5b;

multiplier_5b : MULT18X18
port map (
   A  => factor5b,
   B  => fir5_fast,
   P  => prod5b
);


---------------------------------------------------------------
-- Update registers 13b, 14b and 15b :
---------------------------------------------------------------
reg_13b_to_15b_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         reg13b <= prod3b(21 downto 0);
         reg14b <= prod4b(22 downto 0);
         reg15b <= prod5b(22 downto 0);
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Asyncronious sums :
---------------------------------------------------------------
with bits_10_z12_i select
   sum8b    <= ('0' & reg13b) + conv_std_logic_vector(2**13,23) when '1',     -- 10 bits
               ('0' & reg13b) + conv_std_logic_vector(2**11,23) when others;  -- 12 bits

   sum9b    <= ('0' & reg14b) + ('0' & reg8b);

   
with samp_422_z444_i select
   sum10b   <= (others => '0')         when '1',     -- (4:2:2)
               ('0' & reg15b) + reg9b  when others;  -- (4:4:4)

               
---------------------------------------------------------------
-- Update registers 8b through 9b and chroma_b_o :
---------------------------------------------------------------
reg_8b_9b_chroma_b_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         reg8b       <= sum8b;
         reg9b       <= sum9b;
         if samp_422_z444_i = '1' then   
            chroma_b_o  <= "1000000000";     -- alpha channel
         else
            chroma_b_o  <= sum10b(23 downto 14);
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate cb lsbs :
---------------------------------------------------------------
cb_lsb_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         lsb_b <= sum10b(13 downto 12);
         if bits_10_z12_i = '1' then               -- 10 bits:  
            cb_lsb_o <= "00";                      
         else                                      -- 12 bits:
            if cr_zcb_i = '1' then                 -- maybe wrong!
               cb_lsb_o <= sum10a(13 downto 12);
            else                                   
               cb_lsb_o <= lsb_b;
            end if;
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate cr lsbs :
-- generated asyncroniously to advance cr 1 sample.
---------------------------------------------------------------
cr_lsb_generation : process (bits_10_z12_i, cr_zcb_i, sum10a, lsb_b) begin
   if bits_10_z12_i = '1' then                  -- 10 bits:  
      cr_lsb_o <= "00";                         
   else                                         -- 12 bits:
      if cr_zcb_i = '1' then                    -- maybe wrong!
         cr_lsb_o <= lsb_b;
      else                                   
         cr_lsb_o <= sum10a(13 downto 12);
      end if;
   end if;
end process;


end behavioral;