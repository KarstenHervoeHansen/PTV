library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;
USE work.pack_crc.all;

entity crc_inserter is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;                        -- sync. reset

      -- control signals:
      calc_crc_i           : in std_logic;                        -- high when crc is calculated, crc words are inserted immedeately after

      -- video data:
      data_i               : in std_logic_vector(9 downto 0);     -- Horizontal blanking data
      data_o               : out std_logic_vector(9 downto 0)     -- output to scrambler
   );
end crc_inserter;

--                                 |< crc is calculated here  >|  (w3 to w6, both included)
--                           ---------------------------
-- calc_crc_i:              |                           |
--                 ---------                             -----------------------------------------
--                 -- ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------
-- data_i:           X  w1  X  w2  X  w3  X  w4  X  w5  X  w6  X  w7  X  w8  X  w9  X  w10 X  w11
--                 -- ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------
--                 -- ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------
-- data_o:           X  w0  X  w1  X  w2  X  w3  X  w4  X  w5  X  w6  X crc0 X crc1 X  w9  X  w10
--                 -- ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------
--                                                              ------
-- insert_crc(0):                                              |      |
--                 --------------------------------------------        ---------------------------
--                                                                     ------
-- insert_crc(1):                                                     |      |
--                 ---------------------------------------------------        --------------------


architecture behavioral of crc_inserter is

signal crc_reg_out         : std_logic_vector(17 downto 0);
signal crc_reg_in          : std_logic_vector(17 downto 0);
signal insert_crc          : std_logic_vector(1 downto 0);
signal calc_crc_delayed_1  : std_logic;


begin


--------------------------------------------------------------------
-- generate insert_crc signals:
--------------------------------------------------------------------
insert_crc_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      insert_crc           <= (others => '0');
      calc_crc_delayed_1   <= '0';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         insert_crc           <= (others => '0');
         calc_crc_delayed_1   <= '0';
      elsif en_i = '1' then
         if calc_crc_i = '0' and calc_crc_delayed_1 = '1' then    -- falling edge
            insert_crc(0)  <= '1';
         else
            insert_crc(0)  <= '0';
         end if;
         insert_crc(1)        <= insert_crc(0);
         calc_crc_delayed_1   <= calc_crc_i;
      end if;
   end if;
end process;


--------------------------------------------------------------------
-- calculate crc register inputs:
--------------------------------------------------------------------
crc_reg_in_calcuation : process (data_i, crc_reg_out)
begin
   crc_reg_in(0)   <= crc_reg_out(10);
   crc_reg_in(1)   <= crc_reg_out(11);
   crc_reg_in(2)   <= crc_reg_out(12);
   crc_reg_in(3)   <= crc_reg_out(13)                                                                xor crc_reg_out(0) xor data_i(0);
   crc_reg_in(4)   <= crc_reg_out(14)                               xor crc_reg_out(0) xor data_i(0) xor crc_reg_out(1) xor data_i(1);
   crc_reg_in(5)   <= crc_reg_out(15)                               xor crc_reg_out(1) xor data_i(1) xor crc_reg_out(2) xor data_i(2);      
   crc_reg_in(6)   <= crc_reg_out(16)                               xor crc_reg_out(2) xor data_i(2) xor crc_reg_out(3) xor data_i(3);      
   crc_reg_in(7)   <= crc_reg_out(17)                               xor crc_reg_out(3) xor data_i(3) xor crc_reg_out(4) xor data_i(4);
   crc_reg_in(8)   <=                 crc_reg_out(0)  xor data_i(0) xor crc_reg_out(4) xor data_i(4) xor crc_reg_out(5) xor data_i(5);
   crc_reg_in(9)   <=                 crc_reg_out(1)  xor data_i(1) xor crc_reg_out(5) xor data_i(5) xor crc_reg_out(6) xor data_i(6);
   crc_reg_in(10)  <=                 crc_reg_out(2)  xor data_i(2) xor crc_reg_out(6) xor data_i(6) xor crc_reg_out(7) xor data_i(7);
   crc_reg_in(11)  <=                 crc_reg_out(3)  xor data_i(3) xor crc_reg_out(7) xor data_i(7) xor crc_reg_out(8) xor data_i(8);
   crc_reg_in(12)  <=                 crc_reg_out(4)  xor data_i(4) xor crc_reg_out(8) xor data_i(8) xor crc_reg_out(9) xor data_i(9);
   crc_reg_in(13)  <=                 crc_reg_out(5)  xor data_i(5) xor crc_reg_out(9) xor data_i(9);
   crc_reg_in(14)  <=                 crc_reg_out(6)  xor data_i(6);
   crc_reg_in(15)  <=                 crc_reg_out(7)  xor data_i(7);
   crc_reg_in(16)  <=                 crc_reg_out(8)  xor data_i(8);
   crc_reg_in(17)  <=                 crc_reg_out(9)  xor data_i(9);
end process;


--------------------------------------------------------------------
-- crc registers:
--------------------------------------------------------------------
crc_registering : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      crc_reg_out <= (others => '0');
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         crc_reg_out <= (others => '0');
      elsif en_i = '1' then
         if calc_crc_i = '1' and calc_crc_delayed_1 = '0' then    -- calc_crc rising
            crc_reg_out <= (others => '0');                       -- reset crc calculation
         elsif calc_crc_delayed_1 = '1' then
            crc_reg_out <= crc_reg_in;
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
      data_o <= (others => '0');
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         data_o <= data_i;
      elsif en_i = '1' then
         case insert_crc is
         when "01" =>
            data_o <= (not crc_reg_out(8)) & crc_reg_out(8 downto 0);
         when "10" =>
            data_o <= (not crc_reg_out(17)) & crc_reg_out(17 downto 9);
         when others =>
            data_o <= data_i;
         end case;   
      end if;
   end if;
end process;


end behavioral;