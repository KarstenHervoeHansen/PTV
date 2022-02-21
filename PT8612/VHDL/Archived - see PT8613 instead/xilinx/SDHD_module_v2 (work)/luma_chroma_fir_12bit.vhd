library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity luma_chroma_fir_12bit is
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;

      -- parallel data inputs:
      cr_zcb_i             : in std_logic;
      force_black_i        : in std_logic;
      y_i                  : in std_logic_vector(11 downto 0);
      cb_i                 : in std_logic_vector(11 downto 0);
      cr_i                 : in std_logic_vector(11 downto 0);

      -- parallel data outputs:
      luma_o               : out std_logic_vector(9 downto 0);
      chroma_o             : out std_logic_vector(9 downto 0)
   );
end luma_chroma_fir_12bit;


architecture behavioral of luma_chroma_fir_12bit is


signal y_mux               : std_logic_vector(11 downto 0);
signal cb_mux              : std_logic_vector(11 downto 0);
signal cr_mux              : std_logic_vector(11 downto 0);

signal luma_fir            : std_logic_vector(9 downto 0);
signal luma_z_1            : std_logic_vector(9 downto 0);
signal luma_z_2            : std_logic_vector(9 downto 0);
signal luma_z_3            : std_logic_vector(9 downto 0);


component luma_fir_12bit is
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;
      luma_i               : in std_logic_vector(11 downto 0);
      luma_o               : out std_logic_vector(9 downto 0)
   );
end component;


component chroma_fir_12bit is
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;
      cr_zcb_i             : in std_logic;
      cb_i                 : in std_logic_vector(11 downto 0);
      cr_i                 : in std_logic_vector(11 downto 0);
      chroma_o             : out std_logic_vector(9 downto 0)
   );
end component;


begin


---------------------------------------------------------------
-- asyncronious y, cb, cr mux:
---------------------------------------------------------------
y_cb_cr_mux: process(force_black_i, y_i, cb_i, cr_i)
begin
   if force_black_i = '1' then
      y_mux    <= "000100000000";
      cb_mux   <= "100000000000";
      cr_mux   <= "100000000000";
   else
      y_mux    <= y_i;
      cb_mux   <= cb_i;
      cr_mux   <= cr_i;
   end if;
end process;   


---------------------------------------------------------------
-- Filtering of luma signal :
---------------------------------------------------------------

luma_filtering : luma_fir_12bit
   port map(
      clk_i             => clk_i,
      en_i              => en_i,
      luma_i            => y_mux,
      luma_o            => luma_fir
   );

---------------------------------------------------------------
-- Delay luma signal to match chroma :
---------------------------------------------------------------
luma_delaying : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
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
      en_i              => en_i,
      cr_zcb_i          => cr_zcb_i,
      cb_i              => cb_mux,
      cr_i              => cr_mux,
      chroma_o          => chroma_o
   );


end behavioral;