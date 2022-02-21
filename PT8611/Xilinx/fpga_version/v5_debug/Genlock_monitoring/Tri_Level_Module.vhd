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

entity Tri_Level_Module is
   Port ( 
      --------- til/fra Mainframe -------------------------------------------------------------------
      mreset_i :    in std_logic;      -- Master Reset fra mainboard
      f1484_i  :    in std_logic;      -- 148.35 MHz clock
      f1485_i  :    in std_logic;      -- 148.5 MHz clock
      f4m_i    :    in std_logic;      -- Vertikal sync reference - 625 lines SD format
      f8g_i    :    in std_logic;      -- Vertikal sync reference - 525 lines SD format

      --------- testpoints -------------------------------------------------------
      led1_o   :    out std_logic;     -- forbundet til en noconnect på xc3s50
      led2_o   :    out std_logic;     -- forbundet til en noconnect på xc3s50
      led3_o   :    out std_logic;

      ext30_o  :    out std_logic;     -- j5 pin 1
      ext31_o  :    out std_logic;     -- j5 pin 2
      ext32_o  :    out std_logic;     -- j5 pin 3
      ext33_o  :    out std_logic      -- j5 pin 4
   );
end Tri_Level_Module;


architecture Behavioral of Tri_Level_Module is


signal tsg1_ok,tsg2_ok,tsg3_ok,tsg4_ok    : std_logic;  -- low pulse when level error
signal ref2398,ref24,ref30                : std_logic;
signal clk1_sel,clk2_sel,not_clk2_sel     : std_logic;
signal clk3_sel,clk4_sel,not_clk4_sel     : std_logic;
signal f148g1,f148g2,f148g3,f148g4        : std_logic;
attribute clock_signal                    : string;
attribute clock_signal of f148g4 : signal is "yes";
attribute clock_signal of f148g1 : signal is "yes";
attribute clock_signal of f148g2 : signal is "yes";
attribute clock_signal of f148g3 : signal is "yes";

-- debug signals :
signal tp1,tp2,tp3,tp4        : std_logic_vector(7 downto 0);   -- test vektorer
signal updown                 : std_logic;
signal which_led              : integer range 0 to 3;
signal flashcount             : integer range 0 to 7;


signal f4m_clean              : std_logic;      
signal f4m_genlock_ok         : std_logic;
signal f4m_genlock_resync     : std_logic;
signal f8g_clean              : std_logic;      
signal f8g_genlock_ok         : std_logic;
signal f8g_genlock_resync     : std_logic;
signal genlock_resync         : std_logic;
signal reset_genlock          : std_logic;
signal reset_genmon           : std_logic;
signal tick_160ms             : std_logic;


constant f4m_sync_period      : integer := 9900000;                      -- 9900000 used for NTSC
constant f8g_sync_period      : integer := 23760000;                      -- 23760000 used for PAL


component sync_genlock_regen
   generic (
      sync_period             : integer;
      sync_delay              : integer
   );                         
   port (                     
      mreset_i                : in std_logic;                       -- Master reset
      clk_i                   : in std_logic;                       -- sample clock
      sync_i                  : in std_logic;
      genlock_ok_o            : out std_logic;
      genlock_resync_o        : out std_logic;
      sync_clean_o            : out std_logic;
      -- for debuging:
      pulse_window_o          : out std_logic;
	   sync_pulse_del_o        : out std_logic
   );                         
end component;                
                              
                              
component reset_sequencer     
   port (                     
      mreset_i                : in std_logic;                        -- Master reset
      clk_i                   : in std_logic;                        -- sample clock
      reset_genlock_o         : out std_logic;
      reset_genmon_o          : out std_logic;
      tick_160ms_o            : out std_logic
   );
end component;

begin


f4m_genlock_monitor : sync_genlock_regen 
generic map (
   sync_period                => f4m_sync_period,
   sync_delay                 => f4m_sync_period-5
)
port map (
   mreset_i                   => reset_genlock,
   clk_i                      => f1484_i,
   sync_i                     => f4m_i,
   genlock_ok_o               => f4m_genlock_ok,
   genlock_resync_o           => f4m_genlock_resync,
   sync_clean_o               => f4m_clean,
   -- for debug:
   pulse_window_o             => ext30_o,
   sync_pulse_del_o           => ext31_o
);


f8g_genlock_monitor : sync_genlock_regen 
   generic map (
      sync_period             => f8g_sync_period,
      sync_delay              => f8g_sync_period-5
   )
   port map (
      mreset_i                => reset_genlock,
      clk_i                   => f1485_i,
      sync_i                  => f8g_i,
      genlock_ok_o            => f8g_genlock_ok,
      genlock_resync_o        => f8g_genlock_resync,
      sync_clean_o            => f8g_clean,
      -- for debug:
      pulse_window_o          => ext32_o,
      sync_pulse_del_o        => ext33_o
   );

reset_sequencer_1 : reset_sequencer
   port map ( 
      mreset_i                => mreset_i,
      clk_i                   => f1485_i,
      reset_genlock_o         => reset_genlock,
      reset_genmon_o          => reset_genmon,
      tick_160ms_o            => tick_160ms
   );

   
------------------------------------------------------------- 
-- Monitors the genlock_ok                                   
-------------------------------------------------------------    
genlock_ok_monitor : process (reset_genmon, f1485_i) begin          
   if mreset_i = '0' then                                          
      led3_o <= '1';
   elsif f1485_i'event and f1485_i = '1' then                         
      if reset_genmon = '1' and (f4m_genlock_ok = '0' or f8g_genlock_ok = '0') then             
         led3_o <= '0';                                       
      end if;                                                     
   end if;                                                      
end process;                                                  


------------------------------------------------------------- 
-- Monitors the genlock_resync                                   
-------------------------------------------------------------                                                       
genlock_resync_monitor : process (reset_genmon, genlock_resync) begin          
   if reset_genmon = '0' then                                          
      led2_o <= '0';
   elsif genlock_resync'event and genlock_resync = '1' then                         
      led2_o <= '1';
   end if;                                                      
end process;                                                  


genlock_resync <= f4m_genlock_resync or f8g_genlock_resync;                                     

led1_o <= f4m_genlock_ok and f8g_genlock_ok;                                     
                                           
end Behavioral;                            