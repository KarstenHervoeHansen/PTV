library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;


------------------------------------------------------------------
-- The delay from data_i to video output on the BNC connctor
-- is approximately 3 clock cycles at 148.5 MHz.
------------------------------------------------------------------


entity rocket_io_top is
   port(
      brefclk_i         : in std_logic;
      brefclk2_i        : in std_logic;
      use_clk2_i        : in std_logic;
      sync_reset_i      : in std_logic;
      power_down_i      : in std_logic;
      data_clk_i        : in std_logic;
      data_i            : in std_logic_vector (19 downto 0);

      fifo_error_o      : out std_logic;
      sdi_p_o           : out std_logic;     -- serial video outputs:
      sdi_n_o           : out std_logic
   );
end rocket_io_top;


architecture Behavioral of rocket_io_top is


signal data             : std_logic_vector (19 downto 0);


component rocketio
   port (
      BREFCLK_IN        : in std_logic; 
      BREFCLK2_IN       : in std_logic; 
      ENMCOMMAALIGN_IN  : in std_logic; 
      ENPCOMMAALIGN_IN  : in std_logic; 
      LOOPBACK_IN       : in std_logic_vector (1 downto 0); 
      POWERDOWN_IN      : in std_logic; 
      REFCLKSEL_IN      : in std_logic; 
      RXN_IN            : in std_logic; 
      RXPOLARITY_IN     : in std_logic; 
      RXP_IN            : in std_logic; 
      RXRESET_IN        : in std_logic; 
      RXUSRCLK_IN       : in std_logic; 
      RXUSRCLK2_IN      : in std_logic; 
      TXDATA_IN         : in std_logic_vector (19 downto 0); 
      TXINHIBIT_IN      : in std_logic; 
      TXPOLARITY_IN     : in std_logic; 
      TXRESET_IN        : in std_logic; 
      TXUSRCLK_IN       : in std_logic; 
      TXUSRCLK2_IN      : in std_logic; 
      RXBUFSTATUS_OUT   : out std_logic_vector (1 downto 0); 
      RXDATA_OUT        : out std_logic_vector (19 downto 0); 
      RXRECCLK_OUT      : out std_logic; 
      TXBUFERR_OUT      : out std_logic; 
      TXN_OUT           : out std_logic; 
      TXP_OUT           : out std_logic
   );
end component;


component IBUFGDS
   generic(IOSTANDARD: string);
	port(
	   O  : out std_ulogic;
	 	I  : in std_ulogic;
		IB : in std_ulogic
	);
end component;


begin


---------------------------------------------------
-- Reclock input data:
---------------------------------------------------
data_reclocking : process (power_down_i, data_clk_i) begin
   if data_clk_i'event and data_clk_i ='1' then
      data <= data_i;
   end if;
end process;


-----------------------------------------------------
-- MGT serializer (Rocket IO) :
-----------------------------------------------------
serial_data_output : rocketio
   port map (
      BREFCLK_IN       => brefclk_i,
      BREFCLK2_IN      => brefclk2_i,
      ENMCOMMAALIGN_IN => '0',
      ENPCOMMAALIGN_IN => '0',
      LOOPBACK_IN      => "00",
      POWERDOWN_IN     => power_down_i,
      REFCLKSEL_IN     => use_clk2_i,
      RXN_IN           => '0',
      RXPOLARITY_IN    => '0',
      RXP_IN           => '0',
      RXRESET_IN       => sync_reset_i,
      RXUSRCLK_IN      => data_clk_i,
      RXUSRCLK2_IN     => data_clk_i,
      TXDATA_IN        => data,
      TXINHIBIT_IN     => '0',
      TXPOLARITY_IN    => '0',
      TXRESET_IN       => sync_reset_i,
      TXUSRCLK_IN      => data_clk_i,
      TXUSRCLK2_IN     => data_clk_i,
      RXBUFSTATUS_OUT  => open,
      RXDATA_OUT       => open,
      RXRECCLK_OUT     => open,
      TXBUFERR_OUT     => fifo_error_o,
      TXN_OUT          => sdi_n_o,
      TXP_OUT          => sdi_p_o
   );

   
end Behavioral;