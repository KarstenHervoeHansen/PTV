library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
--
-- pragma translate_off
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
-- pragma translate_on
--
entity   infrastructure_iobs_16bit  is
 port(
      clk0              : in STD_LOGIC;
      clk90             : in STD_LOGIC;
   clk180 : in std_logic;
   clk270 : in std_logic;
      ddr2_clk0         : out STD_LOGIC;
      ddr2_clk0b        : out STD_LOGIC
      );
end   infrastructure_iobs_16bit;  

architecture   arc_infrastructure_iobs_16bit of   infrastructure_iobs_16bit    is


attribute syn_keep : boolean;
attribute xc_props : string;
  
     
---- Component declarations -----

 component IBUFGDS_LVDS_25     
  port ( I  : in std_logic;     
         IB : in std_logic;    
         O  : out std_logic);   
 end component;

 component FDDRRSE 
 port( Q  : out std_logic;
       C0 : in std_logic;
       C1 : in std_logic;
       CE : in std_logic;
       D0 : in std_logic;
       D1 : in std_logic;
       R  : in std_logic;
       S  : in std_logic);
 end component;

 component OBUF
 port (
   O : out std_logic;
   I : in std_logic);
 end component;

---- ******************* ----
---- Signal declarations ----
---- ******************* ----

signal ddr2_clk0_q          :std_logic;
signal ddr2_clk0b_q         :std_logic;
signal vcc                  :std_logic;
signal gnd                  :std_logic;



---- **************************************************
---- iob attributes for instantiated FDDRRSE components
---- **************************************************

attribute xc_props of U1: label is "IOB=TRUE";
attribute xc_props of U2: label is "IOB=TRUE";


begin

 gnd <= '0';
 vcc <= '1';



----  Component instantiations  ----

--- ***********************************
----     This includes instantiation of the output DDR flip flop
----     for ddr clk's and dimm clk's
---- ***********************************************************

U1 : FDDRRSE port map (
                        Q  => ddr2_clk0_q ,
                        C0 => clk0,
                        C1 => clk180,
                        CE => vcc,
                        D0 => vcc,
                        D1 => gnd,
                         R => gnd,
                         S => gnd);

U2 : FDDRRSE port map (
                        Q => ddr2_clk0b_q ,
                        C0 => clk0,
                        C1 => clk180,
                        CE => vcc,
                        D0 => gnd,
                        D1 => vcc,
                         R => gnd,
                         S => gnd);



---- ******************************************
---- Ouput BUffers for ddr clk's and dimm clk's
---- ******************************************


r1 : OBUF port map (
                     I => ddr2_clk0_q,
                     O => ddr2_clk0);

r2 : OBUF port map (
                     I => ddr2_clk0b_q,
                     O => ddr2_clk0b);


end   arc_infrastructure_iobs_16bit;  
