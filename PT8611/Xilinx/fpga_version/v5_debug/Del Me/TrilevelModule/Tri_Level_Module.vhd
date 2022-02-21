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
      --------- from Mainframe -------------------------------------------------------------------
      mreset_i    : in std_logic;      -- Master Reset fra mainboard
      f1484_i     : in std_logic;      -- 148.35 MHz clock
      f1485_i     : in std_logic;      -- 148.5 MHz clock
      f4m_i       : in std_logic;      -- Vertikal sync reference - 625 lines SD format
      f8g_i       : in std_logic;      -- Vertikal sync reference - 525 lines SD format
 
      --------- from Controller -------------------------------------------------------------------------
      sck_i       : in std_logic;      -- serial interface clock
      mosi_i      : in std_logic;      -- Master Out Serial data In
      cs1_i       : in std_logic;      -- chip select tsg1 (nss)
      cs2_i       : in std_logic;      -- chip select tsg2 (port 2 bit 5)
      cs3_i       : in std_logic;      -- chip select tsg3 (port 1 bit 7)
      cs4_i       : in std_logic;      -- chip select tsg4 (port 1 bit 6)

      --------- to Controller -------------------------------------------------------------------------
      res1_o      : out std_logic;     -- generator 1&2 output levels OK - respons til mainframe
      res2_o      : out std_logic;     -- generator 3&4 output levels OK - respons til mainframe
                  
      --------- TSG outputs --------------------------------------------------------------------------
      tsg1_o      : out std_logic_vector(3 downto 0);   -- 4 bit til DA converter
      tsg2_o      : out std_logic_vector(3 downto 0);
      tsg3_o      : out std_logic_vector(3 downto 0);
      tsg4_o      : out std_logic_vector(3 downto 0);

      --------- til rekonstruktions filtre ----------------------------------------------
      clmp1_o     : out std_logic;      -- til clamping puls - benyttes ikke 
      clmp2_o     : out std_logic;
      clmp3_o     : out std_logic;
      clmp4_o     : out std_logic;
      
      --------- Output level check -------------------------------------------------------------------
      tsg1_lvl_i  : in std_logic;        -- signals from comparators
      tsg2_lvl_i  : in std_logic;
      tsg3_lvl_i  : in std_logic;
      tsg4_lvl_i  : in std_logic;

      --------------------------------------------------------------------------------   
      p0_i        : in std_logic_vector(7 downto 2); -- port 0 [7:2]
      p1_i        : in std_logic_vector(5 downto 0); -- port 1 [5:0]
               
      --------- testpoints -------------------------------------------------------
      led1_o      : out std_logic;     -- forbundet til en noconnect på xc3s50
      led2_o      : out std_logic;     -- forbundet til en noconnect på xc3s50
      led3_o      : out std_logic;

      ext0_o      : out std_logic_vector(3 downto 0);     -- j5 pin 1
      ext1_o      : out std_logic_vector(3 downto 0);     -- j5 pin 2
      ext2_o      : out std_logic_vector(3 downto 0);     -- j5 pin 3
      ext3_o      : out std_logic_vector(3 downto 0)      -- j5 pin 4
   );
   
end Tri_Level_Module;


architecture Behavioral of Tri_Level_Module is


signal tsg1_ok,tsg2_ok,tsg3_ok,tsg4_ok    : std_logic;  -- low pulse when level error
signal sync_2398,sync_24,sync_30          : std_logic;


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
signal f1484                  : std_logic;
signal f1485                  : std_logic;

-- debug:
signal sync_mode              : std_logic_vector(1 downto 0);
signal ch1_genlock_sync       : std_logic;
signal ch1_frame_start        : std_logic;
signal ch1_genlock_sync_tog   : std_logic;
signal ch1_frame_start_tog    : std_logic;
signal f8g_clean_tog          : std_logic;
signal f8g_sync_reset         : std_logic;
signal f4m_clean_tog          : std_logic;
signal f4m_sync_reset         : std_logic;

signal all_genlock_ok         : std_logic;
signal genlock_resync_latch   : std_logic;
signal spl_div                : std_logic_vector(9 downto 0);

constant f8g_count1           : integer := 3375;        -- used to genlock to f8g sync
constant f8g_count2           : integer := 7040;        -- 3375*7040 = 23760000          
constant f8g_bits1            : integer := 12;          -- size of counter 1            
constant f8g_bits2            : integer := 13;          -- size of counter 2            

constant f4m_count1           : integer := 2750;        -- used to genlock to f4m sync  
constant f4m_count2           : integer := 3600;        -- 2750*3600 = 9900000
constant f4m_bits1            : integer := 12;          -- size of counter 1            
constant f4m_bits2            : integer := 12;          -- size of counter 2            

constant sync2398_count1      : integer := 2250;        -- used to generate 23,98Hz sync
constant sync2398_count2      : integer := 2750;        -- 2250*2750 = 6187500
constant sync2398_bits1       : integer := 12;          -- size of counter 1
constant sync2398_bits2       : integer := 12;          -- size of counter 2

constant sync24_count1        : integer := 2250;        -- used to generate 24Hz sync
constant sync24_count2        : integer := 2750;        -- 2250*2750 = 6187500
constant sync24_bits1         : integer := 12;          -- size of counter 1
constant sync24_bits2         : integer := 12;          -- size of counter 2

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


component sync_genlock_regen
	generic (
   	count_val1         : integer;                             -- the period of sync is count_val1*count_val2
      count_val2         : integer;
      bits1              : integer;                             -- number of bits in counters
      bits2              : integer
	);  
  	port (
   	mreset_i           : in std_logic;
   	clk_i              : in std_logic;
   	sync_i             : in std_logic;
   	sync_clean_o       : out std_logic;
		genlock_ok_o       : out std_logic;
		--for debug:
		sync_reset_o       : out std_logic
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


component tri_level_channel     
   Port ( 
      mreset_i       : in std_logic;      -- Master Reset fra mainboard
      f1484_i        : in std_logic;      -- 148.35 MHz clock
      f1485_i        : in std_logic;      -- 148.5 MHz clock
      f4m_i          : in std_logic;      -- Vertikal sync reference - 625 lines SD format
      f8g_i          : in std_logic;      -- Vertikal sync reference - 525 lines SD format
      f2398_i        : in std_logic;      
      f24_i          : in std_logic;      
      f30_i          : in std_logic;
      sck_i          : in std_logic;      -- serial interface clock
      mosi_i         : in std_logic;      -- Master Out Serial data In
      cs_i           : in std_logic;      -- chip select
      tsg_level_i    : in std_logic;      -- Monitoring of the output signal
      DAC_o          : out std_logic_vector(3 downto 0);   -- 4 bit til DA converter
      clmp_o         : out std_logic;     -- til clamping puls - benyttes ikke 
      tsg_ok_o       : out std_logic;     -- tsg status
      -- for debug:
      genlock_sync_o : out std_logic;
      frame_start_o  : out std_logic;
      sync_mode_o    : out std_logic_vector(1 downto 0);
      spl_div_o      : out std_logic_vector(9 downto 0)
   );
end component;


component BUFG    
   Port ( 
      I : in  std_logic;
      O : out std_logic
   );
end component;


begin

--------------------------------------------------------------
-- Use BUFG for clock distribution:
--------------------------------------------------------------

f1484_i_distribution : BUFG 
port map(
   I => f1484_i,
   O => f1484
);

f1485_i_distribution : BUFG
port map(
   I => f1485_i,
   O => f1485
);



--------------------------------------------------------------
-- Generate free running sync pulses with a frequency of 23.98 Hz
--------------------------------------------------------------
sync2398_generation : period_dual_count
generic map (
   count_val1 => sync2398_count1,
   count_val2 => sync2398_count2,
   bits1      => sync2398_bits1,
   bits2      => sync2398_bits2
)
port map (
   mreset_i     => mreset_i,
   clk_i        => f1484,
   sync_reset_i => '0',
   sync_o       => sync_2398
);


--------------------------------------------------------------
-- Generate free running sync pulses with a frequency of 24 Hz
--------------------------------------------------------------
sync24_generation : period_dual_count 
generic map (
   count_val1 => sync24_count1,
   count_val2 => sync24_count2,
   bits1      => sync24_bits1, 
   bits2      => sync24_bits2 
)
port map (
   mreset_i     => mreset_i,
   clk_i        => f1485,
   sync_reset_i => '0',
   sync_o       => sync_24
);


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
   mreset_i     => mreset_i,
   clk_i        => f1485,
   sync_reset_i => '0',
   sync_o       => sync_30
);


--------------------------------------------------------------
-- Genlock to the mainframe f4m signal for NTSC
--------------------------------------------------------------
f4m_genlock_regen : sync_genlock_regen 
   generic map (
      count_val1       => f4m_count1,
      count_val2       => f4m_count2,
      bits1            => f4m_bits1,
      bits2            => f4m_bits2 
   )
   port map (
      mreset_i         => reset_genlock,
      clk_i            => f1484,
      sync_i           => f4m_i,
      sync_clean_o     => f4m_clean,
      genlock_ok_o     => f4m_genlock_ok,
      --for debug:
      sync_reset_o     => f4m_sync_reset
   );

--------------------------------------------------------------
-- Genlock to the mainframe f8g signal for PAL
--------------------------------------------------------------
f8g_genlock_regen : sync_genlock_regen 
   generic map (
      count_val1       => f8g_count1,
      count_val2       => f8g_count2,
      bits1            => f8g_bits1, 
      bits2            => f8g_bits2  
   )
   port map (
      mreset_i         => reset_genlock,
      clk_i            => f1485,
      sync_i           => f8g_i,
      sync_clean_o     => f8g_clean,
      genlock_ok_o     => f8g_genlock_ok,
      --for debug:
      sync_reset_o     => f8g_sync_reset
   );

   
--------------------------------------------
-- For debug
--------------------------------------------   
reset_sequencer_1 : reset_sequencer
   port map ( 
      mreset_i                => mreset_i,
      clk_i                   => f1485_i,
      reset_genlock_o         => reset_genlock,
      reset_genmon_o          => reset_genmon,
      tick_160ms_o            => tick_160ms
   );
   
   
--------------------------------------------
-- Generator channel 1
--------------------------------------------   
Tri_Level_Channel_1 : tri_level_channel
   Port map ( 
      mreset_i                => reset_genlock,
      f1484_i                 => f1484_i,
      f1485_i                 => f1485_i,
      f4m_i                   => f4m_clean,
      f8g_i                   => f8g_clean,
      f2398_i                 => sync_2398,      
      f24_i                   => sync_24,      
      f30_i                   => sync_30,
      sck_i                   => sck_i,
      mosi_i                  => mosi_i,
      cs_i                    => cs1_i,
      tsg_level_i             => tsg1_lvl_i,

      tsg_ok_o                => tsg1_ok,
      DAC_o                   => tsg1_o,
      clmp_o                  => clmp1_o,
      -- for debug:
      genlock_sync_o          => ch1_genlock_sync,
      frame_start_o           => ch1_frame_start,
      sync_mode_o             => sync_mode,
      spl_div_o               => spl_div
   );



------------------------------------------------------------- 
-- Monitors the genlock_resync                                   
-------------------------------------------------------------                                                       
genlock_resync_monitor : process (reset_genmon, f1485) begin          
   if reset_genmon = '0' then                                          
      genlock_resync_latch <= '0';
   elsif f1485'event and f1485 = '1' then                         
      if genlock_resync = '1' then
         genlock_resync_latch <= '1';
      end if;
   end if;                                                      
end process;


genlock_resync <= f4m_genlock_resync or f8g_genlock_resync;                                     

all_genlock_ok <= f4m_genlock_ok and f8g_genlock_ok;                                     



-------------------------------------------------------------
-- Debug:
-------------------------------------------------------------
ch1_genlock_sync_flip : process (mreset_i, ch1_genlock_sync) begin          
   if mreset_i = '0' then                                          
      ch1_genlock_sync_tog <= '1';
   elsif ch1_genlock_sync'event and ch1_genlock_sync = '1' then                         
      ch1_genlock_sync_tog <= not ch1_genlock_sync_tog;
   end if;                                                      
end process;                                                  


ch1_frame_start_flip : process (mreset_i, ch1_frame_start) begin          
   if mreset_i = '0' then                                          
      ch1_frame_start_tog <= '1';
   elsif ch1_frame_start'event and ch1_frame_start = '1' then                         
      ch1_frame_start_tog <= not ch1_frame_start_tog;
   end if;                                                      
end process;                                                  


f8g_clean_flip : process (mreset_i, f8g_clean) begin          
   if mreset_i = '0' then                                          
      f8g_clean_tog <= '1';
   elsif f8g_clean'event and f8g_clean = '1' then                         
      f8g_clean_tog <= not f8g_clean_tog;
   end if;                                                      
end process;         


f4m_clean_flip : process (mreset_i, f4m_clean) begin          
   if mreset_i = '0' then                                          
      f4m_clean_tog <= '1';
   elsif f4m_clean'event and f4m_clean = '1' then                         
      f4m_clean_tog <= not f4m_clean_tog;
   end if;                                                      
end process;         




-------------------------------------------------------------
-- Terminate unused outputs:
-------------------------------------------------------------

      res1_o      <= '1';        -- 1 = level ok
      res2_o      <= '1';        -- 1 = level ok

      tsg2_o      <= (others => '0');
      tsg3_o      <= (others => '0');
      tsg4_o      <= (others => '0');

      clmp2_o     <= '0';
      clmp3_o     <= '0';
      clmp4_o     <= '0';

      ext0_o(3)   <= '0';
      ext0_o(2)   <= '0';
      ext0_o(1)   <= '0';
      ext0_o(0)   <= '0';
                        
      ext1_o(3)   <= '0';
      ext1_o(2)   <= '0';
      ext1_o(1)   <= f4m_i;         
      ext1_o(0)   <= f4m_sync_reset;
                     
      ext2_o(3)   <= f4m_clean_tog; 
      ext2_o(2)   <= f8g_i;
      ext2_o(1)   <= f8g_sync_reset;     
      ext2_o(0)   <= f8g_clean_tog;
            
      ext3_o(3)   <= ch1_frame_start;
      ext3_o(2)   <= ch1_genlock_sync;
      ext3_o(1)   <= ch1_frame_start_tog;
      ext3_o(0)   <= ch1_genlock_sync_tog;
                     
      led1_o      <= all_genlock_ok;
      led2_o      <= f4m_genlock_ok;    --genlock_resync_latch;
      led3_o      <= f8g_genlock_ok;

end Behavioral;