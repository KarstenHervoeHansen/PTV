library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity luma_chroma_fir_12bit is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      -- parallel data inputs:
      cr_zcb_i             : in std_logic;
      y_i                  : in std_logic_vector(11 downto 0);
      cb_i                 : in std_logic_vector(11 downto 0);
      cr_i                 : in std_logic_vector(11 downto 0);

      -- parallel data outputs:
      luma_o               : out std_logic_vector(9 downto 0);
      chroma_o             : out std_logic_vector(9 downto 0)
   );
end luma_chroma_fir_12bit;


architecture behavioral of luma_chroma_fir_12bit is


signal luma_fir            : std_logic_vector(9 downto 0);
signal luma_z_1            : std_logic_vector(9 downto 0);
signal luma_z_2            : std_logic_vector(9 downto 0);
signal luma_z_3            : std_logic_vector(9 downto 0);


component luma_fir_12bit is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;
      luma_i               : in std_logic_vector(11 downto 0);
      luma_o               : out std_logic_vector(9 downto 0)
   );
end component;


component chroma_fir_12bit is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;
      cr_zcb_i             : in std_logic;
      cb_i                 : in std_logic_vector(11 downto 0);
      cr_i                 : in std_logic_vector(11 downto 0);
      chroma_o             : out std_logic_vector(9 downto 0)
   );
end component;


begin


---------------------------------------------------------------
-- Filtering of luma signal :
---------------------------------------------------------------

luma_filtering : luma_fir_12bit
   port map(
      clk_i             => clk_i,
      zreset_i          => zreset_i,
      en_i              => en_i,
      sync_reset_i      => sync_reset_i,
      luma_i            => y_i,
      luma_o            => luma_fir
   );

---------------------------------------------------------------
-- Delay luma signal to match chroma :
---------------------------------------------------------------
luma_delaying : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      luma_z_1 <= "0001000000";
      luma_z_2 <= "0001000000";
      luma_z_3 <= "0001000000";
      luma_o   <= "0001000000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         luma_z_1 <= "0001000000";
         luma_z_2 <= "0001000000";
         luma_z_3 <= "0001000000";
         luma_o   <= "0001000000";
      elsif en_i = '1' then
         luma_z_1 <= luma_fir;
         luma_z_2 <= luma_z_1;
         luma_z_3 <= luma_z_2;
         luma_o   <= luma_z_3;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Filtering of chroma signal :
---------------------------------------------------------------
chroma_filtering : chroma_fir_12bit
   port map(
      clk_i             => clk_i,
      zreset_i          => zreset_i,
      en_i              => en_i,
      sync_reset_i      => sync_reset_i,
      cr_zcb_i          => cr_zcb_i,
      cb_i              => cb_i,
      cr_i              => cr_i,
      chroma_o          => chroma_o
   );


end behavioral;