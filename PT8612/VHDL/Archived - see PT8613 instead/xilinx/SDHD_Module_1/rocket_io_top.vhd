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
      rio_clk_1_i       : in std_logic;
      rio_clk_2_i       : in std_logic;
      use_rio_clk_1_i   : in std_logic;
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


signal rio_clk          : std_logic;
signal toggle_bit       : std_logic;
signal data             : std_logic_vector (19 downto 0);


attribute clock_signal              : string;
attribute clock_signal of rio_clk   : signal is "yes";


component bufgmux
   Port (
      I0 : in std_logic;
      I1 : in std_logic;
      S  : in std_logic;
      O  : out std_logic
   );
end component;


component MY_GT11
   port(
      TXUSRCLK_IN                  : in    std_logic;
      TXUSRCLK2_IN                 : in    std_logic;
 		RXUSRCLK2_IN                 : in    std_logic;
      GREFCLK_IN                   : in    std_logic;
 		TXRESET_IN                   : in    std_logic;
 		POWERDOWN_IN                 : in    std_logic;
      TXDATA_IN                    : in    std_logic_vector (19 downto 0);

      TXBUFERR_OUT                 : out   std_logic;
      TX1N_OUT                     : out   std_logic;
      TX1P_OUT                     : out   std_logic
   );
end component;


begin


-----------------------------------------------------
-- Reclock input data:
-----------------------------------------------------
data_reclocking : process (power_down_i, data_clk_i) begin
   if data_clk_i'event and data_clk_i ='1' then
      data <= data_i;
   end if;
end process;


-----------------------------------------------------
-- Select ref clock to MGT serializer (Rocket IO) :
-----------------------------------------------------
rio_clock_select : bufgmux
   port map(
      I0 => rio_clk_2_i,
      I1 => rio_clk_1_i,
      S  => use_rio_clk_1_i,
      O  => rio_clk
   );


-----------------------------------------------------
-- MGT serializer (Rocket IO) :
-----------------------------------------------------
serial_data_output :  MY_GT11
	port map(
		TXUSRCLK_IN    => data_clk_i,
		TXUSRCLK2_IN   => data_clk_i,
		RXUSRCLK2_IN   => data_clk_i,
		GREFCLK_IN 	   => rio_clk,
		TXRESET_IN     => sync_reset_i,
		POWERDOWN_IN   => power_down_i,
	   TXDATA_IN      => data,

	   TXBUFERR_OUT   => fifo_error_o,
		TX1N_OUT 	   => sdi_n_o,
		TX1P_OUT 	   => sdi_p_o
   );


end Behavioral;