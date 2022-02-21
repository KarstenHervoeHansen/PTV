--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:    13:24:34 11/22/05
-- Design Name:    
-- Module Name:    top - Behavioral
-- Project Name:   
-- Target Device:  
-- Tool versions:  
-- Description:
--
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
--------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
library UNISIM;
use UNISIM.VComponents.all;

entity top is
   port(
      zreset_i          : in std_logic;
   	clk_1485_p_i      : in STD_LOGIC;
   	clk_1485_n_i      : in STD_LOGIC;
   	rio_clk_1485_p_i  : in STD_LOGIC;
   	rio_clk_1485_n_i  : in STD_LOGIC;
--   	rio_clk_1484_p_i  : in STD_LOGIC;
--   	rio_clk_1484_n_i  : in STD_LOGIC;
   	
   	cs_1_i            : in std_logic;

      TP1_o             : out STD_LOGIC;
	   TP2_o             : out STD_LOGIC;
	   TP3_o             : out STD_LOGIC;
	   TP4_o             : out STD_LOGIC;
	   pll1_S0_o         : out STD_LOGIC;
	   pll1_S1_o         : out STD_LOGIC;
	   pll1_S2_o         : out STD_LOGIC;
	   pll1_S3_o         : out STD_LOGIC;
	   pll2_S0_o         : out STD_LOGIC;
	   pll2_S1_o         : out STD_LOGIC;
	   pll2_S2_o         : out STD_LOGIC;
	   pll2_S3_o         : out STD_LOGIC;
	   
      sdi_1_p_o         : out std_logic;     -- serial video outputs:
      sdi_1_n_o         : out std_logic
 	   
	 );
end top;


architecture Behavioral of top is

 
component test
   port ( BREFCLK_IN    : in    std_logic; 
          POWERDOWN_IN  : in    std_logic; 
          RXUSRCLK_IN   : in    std_logic; 
          RXUSRCLK2_IN  : in    std_logic; 
          TXDATA_IN     : in    std_logic_vector (19 downto 0); 
          TXINHIBIT_IN  : in    std_logic; 
          TXPOLARITY_IN : in    std_logic; 
          TXRESET_IN    : in    std_logic; 
          TXUSRCLK_IN   : in    std_logic; 
          TXUSRCLK2_IN  : in    std_logic; 
          TXBUFERR_OUT  : out   std_logic; 
          TXN_OUT       : out   std_logic; 
          TXP_OUT       : out   std_logic; 
          TXRUNDISP_OUT : out   std_logic_vector (1 downto 0));
end component;


component IBUFG
   	 port(I : in  std_logic;
         	O : out std_logic);   
end component;


component IBUFGDS
   generic(IOSTANDARD: string);
	port( O: out std_ulogic;
	 		I: in std_ulogic;
			IB:in std_ulogic);
end component;

component BUFG
		port(I: in std_logic;
		     O: out std_logic);
end component;

component os_controller is
	port(
	    clk_148 	  	:  in  std_logic;
	    reset		:  in  std_logic;
	    hdsd_in	    :  in  std_logic;
	    data_in  		:  in  std_logic_vector(19 downto 0);
	    data_out 		:  out std_logic_vector(19 downto 0);
	    enable_out  :  out std_logic
	);
end component;


signal ref_clk      :STD_LOGIC;
signal clk_1485      : STD_LOGIC;
--signal clk_1484      : STD_LOGIC;
signal fifo_error    : STD_LOGIC;
signal sync_reset    : STD_LOGIC;
signal reset_count   : std_logic_vector(28 downto 0);
signal tx_userclk    : std_logic;
signal tp_double     : std_logic_vector(1 downto 0);
signal tx_simu_data  : std_logic_vector(19 downto 0);
signal refclk_1485   : std_logic;
signal enable        : std_logic;
signal tx_data_in    : std_logic_vector(19 downto 0);

begin

diff_clk_1485_bref : IBUFGDS
generic map (IOSTANDARD => "LVDS_25")
port map (
	O  => clk_1485,
	I  => rio_clk_1485_p_i,
	IB => rio_clk_1485_n_i
);

diff_clk_1485_ref : IBUFGDS
generic map (IOSTANDARD => "LVDS_25")
port map (
	O  => refclk_1485,
	I  => clk_1485_p_i,
	IB => clk_1485_n_i
);

make_user_clk : BUFG
port map(
	I => clk_1485,
	O => tx_userclk
);

make_data: process(zreset_i, refclk_1485)
begin
   if(zreset_i = '0') then
	  tx_simu_data <= (others => '0');
   elsif refclk_1485'event and refclk_1485 = '1' then
      if enable  = '1' then
		tx_simu_data <= not tx_simu_data;
	  end if;
	end if;
end process;

os_test : os_controller
port map
(	clk_148     =>  refclk_1485,
	reset	    =>  zreset_i,
	hdsd_in	    =>  '0',
	data_in     =>  "01001100011100001111",
	data_out    =>  tx_data_in,
	enable_out  =>  enable   );


RIO1 : test
port map ( BREFCLK_IN   => clk_1485, 
          POWERDOWN_IN  => '0', 
          RXUSRCLK_IN   => clk_1485, 
          RXUSRCLK2_IN  => clk_1485, 
          TXDATA_IN     => tx_data_in, 
          TXINHIBIT_IN  => '0', 
          TXPOLARITY_IN => '0', 
          TXRESET_IN    => not zreset_i, 
          TXUSRCLK_IN   => clk_1485, 
          TXUSRCLK2_IN  => clk_1485, 
          TXBUFERR_OUT  => fifo_error, 
          TXN_OUT       => sdi_1_n_o, 
          TXP_OUT       => sdi_1_p_o, 
          TXRUNDISP_OUT => tp_double);

-----------------------------------------------------
-- Output mapping :
-----------------------------------------------------
TP1_o <= tx_userclk;
TP2_o <= clk_1485;
TP3_o <= tp_double(0);
TP4_o <= fifo_error;

pll1_S0_o <= '0'; --'0';    --148.5
pll1_S1_o <= '1'; --'1';
pll1_S2_o <= '1'; --'1';
pll1_S3_o <= '0'; --'0';

pll2_S0_o <= '1';    --148.35
pll2_S1_o <= '1';
pll2_S2_o <= '1';
pll2_S3_o <= '0';

end Behavioral;
