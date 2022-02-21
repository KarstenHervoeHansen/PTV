library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
library UNISIM;
use UNISIM.VComponents.all;


entity freerunning_sync is
   Port(
      mreset_i    : in std_logic;
      f1485_i     : in std_logic;
      f1484_i     : in std_logic;
      sync_2398_o : out std_logic;
      sync_24_o   : out std_logic;
      sync_30_o   : out std_logic;
   );
end freerunning_sync;

architecture Behavioral of freerunning_sync is
   constant sync2398_period : integer := 6187500;
   constant sync24_period   : integer := 6187500;
   constant sync30_period   : integer := 4950000;

component freerun_sync_count
   generic(
      sync_period : integer
   );
   Port(
      mreset_i    : in std_logic;
      clk_i       : in std_logic;
      sync_o      : out std_logic
   );
end component;                

                           
begin

--------------------------------------------------------------
-- Generate sync pulses with a frequency of 23.98 Hz
--------------------------------------------------------------
sync2398_generation : freerun_sync_count 
generic map (
   sync_period => sync2398_period
)
port map (
   mreset_i    => reset_i,
   clk_i       => f1484_i,
   sync_o      => sync_2398_o
);

--------------------------------------------------------------
-- Generate sync pulses with a frequency of 24 Hz
--------------------------------------------------------------
sync24_generation : freerun_sync_count 
generic map (
   sync_period => sync24_period
)
port map (
   mreset_i    => reset_i,
   clk_i       => f1485_i,
   sync_o      => sync_24_o
);

--------------------------------------------------------------
-- Generate sync pulses with a frequency of 30 Hz
--------------------------------------------------------------
sync30_generation : freerun_sync_count 
generic map (
   sync_period => sync30_period
)
port map (
   mreset_i    => reset_i,
   clk_i       => f1485_i,
   sync_o      => sync_30_o
);

end Behavioral;