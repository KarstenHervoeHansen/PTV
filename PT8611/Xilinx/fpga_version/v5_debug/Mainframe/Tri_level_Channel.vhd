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
-- v5 :	    23/12 2003 - PS     nyt printudlæg m Spartan3 FPGA
-- v501 :   21/04 2004 - ps     ny sync_statemachine ( pulsetype 1 og 2 )
-- v502 :   26/01 2005 - PS     ny komponent freerunning_reference, der genererer intern 
--                              vertikal referencesignal til 23.98, 24, 30 & 60Hz formater
-- v503 :   10/02 2005 - PS     freerunning_reference komponenten, benytter f148 istedet for
--                              f27 pga. crosstalk fra f148, der forstyrrer tællerne i kom-
--                              ponenten.
-- v503 no27 : 23/05 05 - PS    27 MHz clocken er fjernet fra designet, som forsøg på at 
--                              ordne crosstalkproblem på GCK inputs.
--                              NB: delay=0 svarer nu til, at outputtet ligger ca. 3 148MHz
--                              samples før 
-----------------------------------------------------------------------------------------------
    

entity tri_level_channel is
   Port ( 
--------- til/fra Mainframe -------------------------------------------------------------------
      mreset_i       : in std_logic;      -- Master Reset fra mainboard
      f1484_i        : in std_logic;      -- 148.35 MHz clock
      f1485_i        : in std_logic;      -- 148.5 MHz clock
      tsg_ok_o       : out std_logic;
      
--------- Free-running sync signals -----------------------------------------------------------            
      f4m_i          : in std_logic;      -- Vertikal sync reference - 625 lines SD format
      f8g_i          : in std_logic;      -- Vertikal sync reference - 525 lines SD format
      f2398_i        : in std_logic;      
      f24_i          : in std_logic;      
      f30_i          : in std_logic;

--------- fra CPU -----------------------------------------------------------------------------
      sck_i          : in std_logic;      -- serial interface clock
      mosi_i         : in std_logic;      -- Master Out Serial data In
      cs_i           : in std_logic;      -- chip select

--------- Backend I/O -------------------------------------------------------------------------
      DAC_o          : out std_logic_vector(3 downto 0);   -- 4 bit til DA converter
      clmp_o         : out std_logic;     -- til clamping puls - benyttes ikke 
      tsg_level_i    : in std_logic;      -- Monitoring of the output signal
      
--------- Debug I/O -------------------------------------------------------------------------
      genlock_sync_o : out std_logic;
      frame_start_o  : out std_logic
   );
end tri_level_channel;


architecture Behavioral of tri_level_channel is

signal use_f1485     : std_logic;
signal clk           : std_logic;
signal genlock_sync  : std_logic;
signal spl_div       : std_logic;
signal lpf           : std_logic;
signal div10_11      : std_logic;
signal framerate_x2  : std_logic;
signal sync_mode     : std_logic;
signal fr_ref_sel    : std_logic;
signal scan_method   : std_logic;
signal sysclk_sel    : std_logic;
signal phasedelay    : std_logic;

attribute clock_signal        : string;
attribute clock_signal of clk : signal is "yes";


component bufgmux
   Port (
      I0 : in std_logic;
      I1 : in std_logic;
      S  : in std_logic;
      O  : out std_logic
   );
end component;


component serial_interface
   Port( 
      mreset_i       : in std_logic;                                                                    
      cs_i           : in std_logic;                                                                    
      sck_i          : in std_logic;                                                                    
      clk_i          : in std_logic;                                                                    
      mosi_i         : in std_logic;                                                                    
      spl_div_o      : out std_logic_vector(9 downto 0);                                                                                                                                                                              
      lpf_o          : out std_logic_vector(10 downto 0);
      div10_11_o     : out std_logic;                    
      framerate_x2_o : out std_logic;                    
      sync_mode_o    : out std_logic_vector(1 downto 0); 
      fr_ref_sel_o   : out std_logic_vector(1 downto 0); 
      scan_method_o  : out std_logic;                    
--      sysclk_sel_o   : out std_logic;                    
      phasedelay_o   : out std_logic_vector(23 downto 0);
   );                                                     
end component;


begin


serial_interfacing : serial_interface
   Port map( 
      mreset_i       => mreset_i,    
      cs_i           => cs_i,        
      sck_i          => sck_i,
      mosi_i         => mosi_i,      
      clk_i          => clk,
      spl_div_o      => spl_div,     
      lpf_o          => lpf,         
      div10_11_o     => div10_11,    
      framerate_x2_o => framerate_x2,
      sync_mode_o    => sync_mode,   
      fr_ref_sel_o   => fr_ref_sel,  
      scan_method_o  => scan_method, 
--      sysclk_sel_o   => use_f1485,  
      phasedelay_o   => phasedelay   
   );                                

   
-------------------------------------------------------------
-- Select clock domain:
-------------------------------------------------------------
use_f1485 <= '1';   
   
clock_selecting : bufgmux
   port map(
      I0 => f1484,
      I1 => f1485,
      S  => use_f1485,
      O  => clk
   );
   
   
-------------------------------------------------------------
-- Select genlock sync source:
-------------------------------------------------------------   
Sync_selecting : process(mreset_i, clk)
begin
   if mreset_i = '0' then
      genlock_sync <= '0';
   elsif clk'event and clk = '1' then
      if sync_mode(1) = '1' then
         if sync_mode(0) = '1' then
            genlock_sync <= f8g_i;
         else
            genlock_sync <= f4m_i;
         end if;
      else
         if fr_ref_sel = "00" then
            genlock_sync <= f30_i;
         elsif fr_ref_sel = "10" then
            genlock_sync <= f24_i;
         elsif fr_ref_sel = "11" then
            genlock_sync <= f2398_i;
         end if;
      end if;
   end if;
end process;


------------------------------------------------------------
-- Backend outputs:
------------------------------------------------------------
clmp_o <= '0';
DAC_o <= others => '0';

end Behavioral;