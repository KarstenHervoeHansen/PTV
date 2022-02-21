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


entity Tri_Level_Channel_clock_flip is

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
      frame_start_o  : out std_logic;
      sync_mode_o    : out std_logic_vector(1 downto 0);
      spl_div_o      : out std_logic_vector(9 downto 0)
   );
end Tri_Level_Channel_clock_flip;


architecture Behavioral of Tri_Level_Channel_clock_flip is

signal use_f1484      : std_logic;
signal use_f1485      : std_logic;
signal clk            : std_logic;
signal genlock_sync   : std_logic;
signal spl_div        : std_logic_vector(9 downto 0);
signal lpf            : std_logic_vector(10 downto 0);
signal div10_11       : std_logic;
signal framerate_x2   : std_logic;
signal sync_mode      : std_logic_vector(1 downto 0);
signal fr_ref_sel     : std_logic_vector(1 downto 0);
signal scan_method    : std_logic;
signal phasedelay     : std_logic_vector(23 downto 0);
signal frame_start    : std_logic;
signal zframe_start   : std_logic;
signal last_count     : std_logic;
signal last_lineframe : std_logic;

attribute clock_signal        : string;
attribute clock_signal of clk : signal is "yes";


component Tri_level_timer
   Port ( 
      mreset            : in std_logic;       -- reset til sync_statemachine
      tsg_reset         : in std_logic;       -- start generator puls - aktiv lav
      f148              : in std_logic;       -- multiplexed 148 MHz clock
      scan_method       : in std_logic;       -- ( 1=interlaced, 0=progressiv )
      div10_11          : in std_logic;
      sync_mode_lsb     : in std_logic;
      spl_div           : in std_logic_vector(9 downto 0);  -- total samples pr. line divideret med 10 (prog) eller 11 (interlaced)
      lines_pr_frame    : in std_logic_vector(10 downto 0);
      framerate_x2      : in std_logic;
      tsg_lvl           : in std_logic;
      tsg_ok            : out std_logic;
      last_count        : out std_logic;  -- indikerer samples ved linieskift
      last_lineframe    : out std_logic;
      tsg_out           : out std_logic_vector(3 downto 0)
   );
end component;


component frame_sync_delay
   port (
	   mreset_i      	 : in std_logic;                     -- Master reset
	   clk_i         	 : in std_logic;                     -- sample clock
	   sync_i          : in std_logic;                     
	   spl_div_i       : in std_logic_vector(9 downto 0);  -- total samples pr line divided by 10 or 11
      div10_11_i      : in std_logic;                     -- divider f148 med 10 hvis lav, 11 hvis høj
	   lpf_i           : in std_logic_vector(10 downto 0); -- total lines per frame
      phasedelay_i    : in std_logic_vector(23 downto 0); -- delay from sync reference
	                                                       -- Input parameters must always be valid
	   frame_sync_o	 : out std_logic
   );
end component;


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
      sysclk_sel_o   : out std_logic;
      phasedelay_o   : out std_logic_vector(23 downto 0)
   );
end component;


begin


tri_level_timing : Tri_level_timer
   port map (
      mreset         => mreset_i,
      tsg_reset      => zframe_start,
      f148           => clk,
      scan_method    => scan_method,
      div10_11       => div10_11,
      sync_mode_lsb  => sync_mode(0),
      spl_div        => spl_div,
      lines_pr_frame => lpf,
      framerate_x2   => framerate_x2,
      tsg_lvl        => tsg_level_i,
      tsg_ok         => tsg_ok_o,
      last_count     => last_count,
      last_lineframe => last_lineframe,
      tsg_out        => DAC_o
   );


frame_sync_delaying : frame_sync_delay
   port map (
	   mreset_i      	=> mreset_i,
	   clk_i         	=> clk,
	   sync_i         => genlock_sync,
	   spl_div_i      => spl_div,  
      div10_11_i     => div10_11,      
	   lpf_i          => lpf, 
      phasedelay_i   => phasedelay,
	   frame_sync_o	=> frame_start
   );


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
      sysclk_sel_o   => use_f1484,
      phasedelay_o   => phasedelay
   );


-------------------------------------------------------------
-- Select clock domain:
-------------------------------------------------------------
use_f1485 <= not use_f1484;

clock_selecting : bufgmux
   port map(
      I0 => f1484_i,
      I1 => f1485_i,
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
         if sync_mode(0) = '0' then
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
-- Combinatoric logic:
------------------------------------------------------------
zframe_start <= not frame_start;


------------------------------------------------------------
-- Backend outputs:
------------------------------------------------------------
clmp_o <= '0';


------------------------------------------------------------
-- Debug:                                         
------------------------------------------------------------
genlock_sync_o <= genlock_sync;
sync_mode_o    <= sync_mode;
frame_start_o  <= frame_start;
spl_div_o <= spl_div;

 
end Behavioral;