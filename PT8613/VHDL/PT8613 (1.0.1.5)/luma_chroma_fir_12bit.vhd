library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity luma_chroma_fir_12bit is
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;
      system_i             : in std_logic_vector(11 downto 0);
      
      -- parallel data inputs:
      cr_zcb_i             : in std_logic;
      force_black_i        : in std_logic;
      y_i                  : in std_logic_vector(11 downto 0);
      cb_i                 : in std_logic_vector(11 downto 0);
      cr_i                 : in std_logic_vector(11 downto 0);

      -- parallel data outputs:
      luma_a_o             : out std_logic_vector(9 downto 0);
      chroma_a_o           : out std_logic_vector(9 downto 0);
      luma_b_o             : out std_logic_vector(9 downto 0);
      chroma_b_o           : out std_logic_vector(9 downto 0)
   );
end luma_chroma_fir_12bit;


architecture behavioral of luma_chroma_fir_12bit is


signal spl_index           : std_logic_vector(1 downto 0);
signal even_parity         : std_logic;
signal samp_422_z444       : std_logic;
signal bits_10_z12         : std_logic;

signal luma_lsb            : std_logic_vector(1 downto 0);
signal cb_lsb              : std_logic_vector(1 downto 0);
signal cr_lsb              : std_logic_vector(1 downto 0);

signal chroma_black        : std_logic_vector(11 downto 0);

signal y_mux               : std_logic_vector(11 downto 0);
signal cb_mux              : std_logic_vector(11 downto 0);
signal cr_mux              : std_logic_vector(11 downto 0);

signal luma_fir            : std_logic_vector(11 downto 0);
signal luma_z_1            : std_logic_vector(11 downto 0);
signal luma_z_2            : std_logic_vector(11 downto 0);
signal luma_z_3            : std_logic_vector(11 downto 0);


component luma_fir_12bit is
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;
      bits_10_z12_i        : in std_logic;
      luma_i               : in std_logic_vector(11 downto 0);
      luma_o               : out std_logic_vector(11 downto 0)
   );
end component;


component chroma_fir_12bit is
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
end component;


begin


---------------------------------------------------------------
-- Parameters :
---------------------------------------------------------------
---------------------------------------------------------------
-- samples per line dependent on system :
-- 720: "00", 1280: "01", 1920: "10"
---------------------------------------------------------------
with system_i(11 downto 4) select
   spl_index <=      "10" when X"A0",   -- 1080i/30
                     "10" when X"B0",   -- 1080i/29.97
                     "10" when X"82",   -- 1080p/30
                     "10" when X"92",   -- 1080p/29.97
                     "10" when X"83",   -- 1080p/25
                     "10" when X"84",   -- 1080p/24
                     "10" when X"94",   -- 1080p/23.98
                     "10" when X"A2",   -- 1080sF/30
                     "10" when X"B2",   -- 1080sF/29.97
                     "10" when X"A3",   -- 1080sF/25
                     "10" when X"A4",   -- 1080sF/24
                     "10" when X"B4",   -- 1080sF/23.98
                     "01" when X"86",   -- 720p/60
                     "01" when X"96",   -- 720p/59.94
                     "01" when X"87",   -- 720p/50
                     "01" when X"88",   -- 720p/30
                     "01" when X"98",   -- 720p/29.97
                     "01" when X"89",   -- 720p/25
                     "01" when X"8A",   -- 720p/24
                     "01" when X"9A",   -- 720p/23.98
                     "00" when X"60",   -- NTSC 525i/29.97
                     "00" when X"20",   -- PAL 625i/25
                     "10" when others;  -- 1080i/25   X"A1"


parameter_generation: process(system_i, spl_index)
begin
   if spl_index = "10" then  -- 1080:
      case system_i(3 downto 0) is
      when X"1" =>      -- YCbCr (4:2:2) 12-bit (dual-link)
         samp_422_z444  <= '1';
         bits_10_z12    <= '0';
         chroma_black   <= "100000000000";
      when X"2" =>      -- YCbCr (4:4:4) 10-bit (dual-link)
         samp_422_z444  <= '0';
         bits_10_z12    <= '1';
         chroma_black   <= "100000000000";
      when X"3" =>      -- YCbCr (4:4:4) 12-bit (dual-link)
         samp_422_z444  <= '0';
         bits_10_z12    <= '0';
         chroma_black   <= "100000000000";
      when X"4" =>      -- RGB   (4:4:4) 10-bit (dual-link)
         samp_422_z444  <= '0';
         bits_10_z12    <= '1';
         chroma_black   <= "000100000000";
      when X"5" =>      -- RGB   (4:4:4) 12-bit (dual-link)
         samp_422_z444  <= '0';
         bits_10_z12    <= '0';
         chroma_black   <= "000100000000";
      when others =>    -- YCbCr (4:2:2) 10-bit (single-link)
         samp_422_z444  <= '1';
         bits_10_z12    <= '1';
         chroma_black   <= "100000000000";
      end case;
   else
      samp_422_z444  <= '1';
      bits_10_z12    <= '1';
      chroma_black   <= "100000000000";
   end if;
end process;
                    
                     
---------------------------------------------------------------
-- asyncronious y, cb, cr mux:
---------------------------------------------------------------
y_cb_cr_mux: process(force_black_i, chroma_black, y_i, cb_i, cr_i)
begin
   if force_black_i = '1' then
      y_mux    <= "000100000000";
      cb_mux   <= chroma_black;
      cr_mux   <= chroma_black;
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
      bits_10_z12_i     => bits_10_z12,
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
         luma_a_o <= luma_z_3(11 downto 2);
         if bits_10_z12 = '1' then   -- 10 bits:       -- see SMPTE 372M for mapping of data
            luma_lsb <= "00";
         else                          -- 12 bits:
            luma_lsb <= luma_z_3(1 downto 0);
         end if;
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
      bits_10_z12_i     => bits_10_z12,
      samp_422_z444_i   => samp_422_z444,                
      cr_zcb_i          => cr_zcb_i,
      cb_i              => cb_mux,
      cr_i              => cr_mux,
      chroma_a_o        => chroma_a_o,
      chroma_b_o        => chroma_b_o,
      cb_lsb_o          => cb_lsb,
      cr_lsb_o          => cr_lsb
   );
   

---------------------------------------------------------------
-- Mapping of lsbs into luma_b_o (alpha channel) :
-- generated asyncroniously to advance 1 sample.
---------------------------------------------------------------
luma_b_generation : process (bits_10_z12, even_parity, luma_lsb, cb_lsb, cr_lsb) begin
   if bits_10_z12 = '1' then                    -- 10 bits:  
      luma_b_o <= "0001000000";                         
   else                                         -- 12 bits:
      luma_b_o(9)          <= not even_parity;
      luma_b_o(8)          <= even_parity;
      luma_b_o(7 downto 6) <= luma_lsb;
      luma_b_o(5 downto 4) <= cb_lsb;
      luma_b_o(3 downto 2) <= cr_lsb;
      luma_b_o(1 downto 0) <= "00";
   end if;
end process;
   
even_parity <= luma_lsb(1) xor luma_lsb(0) xor cb_lsb(1) xor cb_lsb(0) xor cr_lsb(1) xor cr_lsb(0);
   
end behavioral;