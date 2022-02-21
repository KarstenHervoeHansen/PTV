library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity crc_inserter is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;                        -- sync. reset

      -- control signals:
      calc_crc_i           : in std_logic;                        -- high when crc is calculated, crc words are inserted immedeately after

      -- video data:
      luma_i               : in std_logic_vector(9 downto 0);     -- Horizontal blanking data
      croma_i              : in std_logic_vector(9 downto 0);

      luma_o               : out std_logic_vector(9 downto 0);    -- output to scrambler
      chroma_o             : out std_logic_vector(9 downto 0)     -- output to scrambler
   );
end crc_inserter;

--                                 |< crc is calculated here  >|  (w1 to w6, both included)
--                           ---------------------------
-- calc_crc_i:              |                           |
--                 ---------                             -----------------------------------------
--                 -- ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------
-- luma_i/croma_i:   X  w1  X  w2  X  w3  X  w4  X  w5  X  w6  X  w7  X  w8  X  w9  X  w10 X  w11
--                 -- ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------
--                 -- ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------
-- luma_o/croma_o:   X  w0  X  w1  X  w2  X  w3  X  w4  X  w5  X  w6  X crc0 X crc1 X  w9  X  w10
--                 -- ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------
--                                                              ------
-- insert_crc0:                                                |      |
--                 --------------------------------------------        ---------------------------
--                                                                     ------
-- insert_crc1:                                                       |      |
--                 ---------------------------------------------------        --------------------


architecture behavioral of crc_inserter is

signal luma_crc            : std_logic_vector(17 downto 0);
signal chroma_crc          : std_logic_vector(17 downto 0);
signal insert_crc0         : std_logic;
signal insert_crc1         : std_logic;
signal calc_crc_delayed_1  : std_logic;


begin


--------------------------------------------------------------------
-- generate insert_crc signals:
--------------------------------------------------------------------
calc_crc_delaying : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      calc_crc_delayed_1 <= '0';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         calc_crc_delayed_1 <= '0';
      elsif en_i = '1' then
         calc_crc_delayed_1 <= calc_crc_i;
      end if;
   end if;
end process;

insert_crc_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      insert_crc0 <= '0';
      insert_crc1 <= '0';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         insert_crc0 <= '0';
         insert_crc1 <= '0';
      elsif en_i = '1' then
         if calc_crc_i = '0' and calc_crc_delayed_1 = '1' then
            insert_crc0 <= '1';
         else
            insert_crc0 <= '0';
         end if;
         insert_crc1 <= insert_crc0;
      end if;
   end if;
end process;


--------------------------------------------------------------------
-- calculate crc:
--------------------------------------------------------------------
luma_crc0  <= crc_0_4_5_18(luma_i,luma_crc);
croma_crc0 <= crc_0_4_5_18(croma_i,croma_crc);

-- save crc iteration for next step:
crc_registering : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      luma_crc <= (others => '0');
      chroma_crc <= (others => '0');
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         luma_crc <= (others => '0');
         chroma_crc <= (others => '0');
      elsif en_i = '1' then
         if calc_crc_i = '1' and calc_crc_delayed_1 = '0' then
            luma_crc <= (others => '0');
            chroma_crc <= (others => '0');
         else
            luma_crc <= luma_crc0;
            chroma_crc <= croma_crc0;
         end if;
      end if;
   end if;
end process;


--------------------------------------------------------------------
-- insert crc:
--------------------------------------------------------------------
crc_insertion : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      luma_o <= luma_i;
      chroma_o <= croma_i;
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         luma_o <= luma_i;
         chroma_o <= croma_i;
      elsif en_i = '1' then
         if insert_crc0 = '1' then
            luma_o <= (not luma_crc0(8)) & luma_crc0(8 downto 0);
            chroma_o <= (not chroma_crc0(8)) & chroma_crc0(8 downto 0);
         elsif insert_crc1 = '1' then
            luma_o <= (not luma_crc(17)) & luma_crc(17 downto 9);
            chroma_o <= (not chroma_crc(17)) & chroma_crc(17 downto 9);
         else
            luma_o <= luma_i;
            chroma_o <= croma_i;
         end if;
      end if;
   end if;
end process;


function crc_0_4_5_18  
   ( data:  std_logic_vector(9 downto 0);
      crc:  std_logic_vector(17 downto 0) )
   return std_logic_vector is

   variable new_crc: std_logic_vector(17 downto 0);
	variable c: std_logic_vector(17 downto 0);
	variable d: std_logic_vector(9 downto 0);

   begin

	d := data;
	c := crc;

	new_crc(17) := c(9)  xor d(9);
	new_crc(16) := c(8)  xor d(8);
	new_crc(15) := c(7)  xor d(7);
	new_crc(14) := c(6)  xor d(6);
	new_crc(13) := c(5)  xor d(5) xor c(9) xor d(9);
	new_crc(12) := c(4)  xor d(4) xor c(8) xor d(8) xor c(9) xor d(9);
	new_crc(11) := c(3)  xor d(3) xor c(7) xor d(7) xor c(8) xor d(8);
	new_crc(10) := c(2)  xor d(2) xor c(6) xor d(6) xor c(7) xor d(7);
	new_crc(9)  := c(1)  xor d(1) xor c(5) xor d(5) xor c(6) xor d(6);
	new_crc(8)  := c(0)  xor d(0) xor c(4) xor d(4) xor c(5) xor d(5);
	new_crc(7)  := c(17) xor c(3) xor d(3) xor c(4) xor d(4);
	new_crc(6)  := c(16) xor c(2) xor d(2) xor c(3) xor d(3);
	new_crc(5)  := c(15) xor c(1) xor d(1) xor c(2) xor d(2);
	new_crc(4)  := c(14) xor c(0) xor d(0) xor c(1) xor d(1);
	new_crc(3)  := c(13) xor c(0) xor d(0);
	new_crc(2)  := c(12);
	new_crc(1)  := c(11);
	new_crc(0)  := c(10);

   return new_crc;

end crc_0_4_5_18;


end behavioral;