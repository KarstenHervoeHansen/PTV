library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
library UNISIM;
use UNISIM.VComponents.all;

-----------------------------------------------------------------------------------------------
-- v1:      Original kode til Prototype lavet af PEH + studerende Jesper Christoffersen 
--          for DK-Audio, som afgangsprojekt november 2002 til marts 2003
-- v2:      19/6 2003 - PS      tilfoejet clamping- og enablesignaler til rekonstruktionsfiltre
-- v3:      23/5 2003 - PS      nyt hieraki => toplevel = Tri_level_sync_generator.vhd
-- v301 :   21/8 2003 - PS      ny komponent sync_statemachine og state_timer
-- v4 :     8/9  2003 - PS      nyt printudlaeg m 100pin CPLD => ny pin-konfiguration
--                                                               + opsplitning af software
-- v5:      23/12 2003 - PS     nyt printudlæg m Spartan3 FPGA
-- v501:    21/04 2004 - ps     ny sync_statemachine ( pulsetype 1 og 2 )
-- v502:    26/01 2005 - PS     ny komponent freerunning_reference, der genererer intern 
--                              vertikal referencesignal til 23.98, 24, 30 & 60Hz formater
-- v503:    10/02 2005 - PS     freerunning_reference komponenten, benytter f148 istedet for
--                              f27 pga. crosstalk fra f148, der forstyrrer tællerne i kom-
--                              ponenten.
-- v503 no27: 23/05 05 - PS     27 MHz clocken er fjernet fra designet, som forsøg på at 
--                              ordne crosstalkproblem på GCK inputs.
--                              NB: delay=0 svarer nu til, at outputtet ligger ca. 3 148MHz
--                              samples før
-- v503 genlock monitor
--          01-06-2005 - thh    Only genlock cirquit implemented
-----------------------------------------------------------------------------------------------

entity dual_count_toplevel is
   
end dual_count_toplevel;


architecture Behavioral of dual_count_toplevel is

constant sync30_count1        : integer := 1375;        -- used to generate 30Hz sync
constant sync30_count2        : integer := 3600;        -- 1375*3600 = 4950000
constant sync30_bits1         : integer := 12;          -- size of counter 1
constant sync30_bits2         : integer := 12;          -- size of counter 2


component period_dual_count
   generic(
      count_val1  : integer;
      count_val2  : integer;
      bits1       : integer;
      bits2       : integer
   );
   Port(
      mreset_i    : in std_logic;
      clk_i       : in std_logic;
      sync_reset_i: in std_logic;
      sync_o      : out std_logic
   );
end component;                

begin



--------------------------------------------------------------
-- Generate free running sync pulses with a frequency of 30 Hz
--------------------------------------------------------------
sync30_generation : period_dual_count 
generic map (
   count_val1 => sync30_count1,
   count_val2 => sync30_count2,
   bits1      => sync30_bits1, 
   bits2      => sync30_bits2 
)
port map (
   mreset_i     => '1',
   sync_reset_i => '0'
);
end Behavioral;