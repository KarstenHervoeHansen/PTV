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


entity Tri_Level_Channel is
   Port (
--------- to/from Module -------------------------------------------------------------------
      mreset_i          : in std_logic;      -- Master Reset fra mainboard
      reset_delayed_i   : in std_logic;
      f1485_i           : in std_logic;
      tick_10ms_i       : in std_logic;
      clk_i             : in std_logic;      -- 148.35 or 148.5 MHz clock
      tsg_ok_o          : out std_logic;

--------- Sync signals ------------------------------------------------------------------------
      f4m_i             : in std_logic;      -- Vertikal sync reference - 625 lines SD format
      f8g_i             : in std_logic;      -- Vertikal sync reference - 525 lines SD format
      f2398_i           : in std_logic;
      f24_i             : in std_logic;
      f30_i             : in std_logic;

--------- Clock mux ---------------------------------------------------------------------------
      use_f1484_o       : out std_logic;
      use_f1485_o       : out std_logic;

--------- fra CPU -----------------------------------------------------------------------------
      sck_i             : in std_logic;      -- serial interface clock
      mosi_i            : in std_logic;      -- Master Out Serial data In
      cs_i              : in std_logic;      -- chip select

--------- Backend I/O -------------------------------------------------------------------------
      DAC_o             : out std_logic_vector(3 downto 0);   -- 4 bit til DA converter
      clmp_o            : out std_logic;     -- til clamping puls - benyttes ikke
      tsg_level_i       : in std_logic;      -- Monitoring of the output signal

--------- Monitoring I/O -------------------------------------------------------------------------
      timing_change_o   : out std_logic;     
      
--------- Debug I/O ---------------------------------------------------------------------------
      debug_o           : out std_logic_vector(15 downto 0)
   );
end Tri_Level_Channel;


architecture Behavioral of Tri_Level_Channel is

signal use_f1484        : std_logic;
signal genlock_sync     : std_logic;
signal spl_div          : std_logic_vector(9 downto 0);
signal lpf              : std_logic_vector(10 downto 0);
signal div10_11         : std_logic;
signal framerate_x2     : std_logic;
signal sync_mode        : std_logic_vector(1 downto 0);
signal fr_ref_sel       : std_logic_vector(1 downto 0);
signal interlaced       : std_logic;
signal phasedelay       : std_logic_vector(23 downto 0);
signal frame_start      : std_logic;
signal mute             : std_logic;
signal reset_channel    : std_logic;
signal debug            : std_logic_vector(15 downto 0);

signal f4m              : std_logic;
signal f8g              : std_logic;
signal f2398            : std_logic;
signal f24              : std_logic;
signal f30              : std_logic;

signal tsg_level        : std_logic;
signal sampled_tsg_lvl  : std_logic;
signal lvl_count        : std_logic_vector(2 downto 0);
signal spl_is_zero      : std_logic;


component output_level_monitor
   Port (
      mreset_i          : in std_logic;
      clk_i             : in std_logic;
      tick_10ms_i       : in std_logic;
      mute_i            : in std_logic;
      spl_is_zero_i     : in std_logic;
      tsg_level_i       : in std_logic;
      output_ok_o       : out std_logic
   );
end component;


component channel_controller
   port (
      mreset_i                : in std_logic;
      reset_delayed_i         : in std_logic;
      clk_i                   : in std_logic;
      tick_10ms_i             : in std_logic;
      var_clk_i               : in std_logic;
      cs_i                    : in std_logic;
      sck_i                   : in std_logic;
      mosi_i                  : in std_logic;
      mute_o                  : out std_logic;
      use_f1484_o             : out std_logic;
      use_f1485_o             : out std_logic;
      spl_div_o               : out std_logic_vector(9 downto 0);
      lpf_o                   : out std_logic_vector(10 downto 0);
      div10_11_o              : out std_logic;
      framerate_x2_o          : out std_logic;
      sync_mode_o             : out std_logic_vector(1 downto 0);
      fr_ref_sel_o            : out std_logic_vector(1 downto 0);
      scan_method_o           : out std_logic;
      timing_o                : out std_logic_vector(23 downto 0);
      reset_channel_o         : out std_logic;
      spl_is_zero_o           : out std_logic;
      --debug:
      debug_o                 : out std_logic_vector(15 downto 0)
   );
end component;


component Tri_level_timer
   Port (
      mreset_i          : in std_logic;      -- Master reset
      frame_start_i     : in std_logic;      -- start generator frame - aktiv høj
      clk_i             : in std_logic;      -- multiplexed 148 MHz clock
      interlaced_i      : in std_logic;      -- ( 1=interlaced, 0=progressiv )
      div10_11_i        : in std_logic;      -- divider clk_i med 10 hvis lav, 11 hvis høj
      sync_mode_lsb_i   : in std_logic;
      spl_div_i         : in std_logic_vector(9 downto 0);  -- total samples pr. line divideret med 10 (prog) eller 11 (interlaced)
      lines_pr_frame_i  : in std_logic_vector(10 downto 0);
      framerate_x2_i    : in std_logic;
      tsg_lvl_i         : in std_logic;
      mute_i            : in std_logic;
      tsg_ok_o          : out std_logic;
      DAC_o             : out std_logic_vector(3 downto 0);
      timing_change_o   : out std_logic;
      --debug:
      debug_o           : out std_logic_vector(15 downto 0) := (others => '0')
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
      samplerate_x2_i : in std_logic;
	   phasedelay_i    : in std_logic_vector(23 downto 0); -- delay from sync reference
	                                                       -- Input parameters must always be valid
	   frame_sync_o	 : out std_logic
   );
end component;


begin


system_controller : channel_controller
   port map (
      mreset_i          => mreset_i,
      reset_delayed_i   => reset_delayed_i,
      clk_i             => f1485_i,
      tick_10ms_i       => tick_10ms_i,
      var_clk_i         => clk_i,
      cs_i              => cs_i,
      sck_i             => sck_i,
      mosi_i            => mosi_i,
      mute_o            => mute,
      use_f1484_o       => use_f1484_o,
      use_f1485_o       => use_f1485_o,
      spl_div_o         => spl_div,
      lpf_o             => lpf,
      div10_11_o        => div10_11,
      framerate_x2_o    => framerate_x2,
      sync_mode_o       => sync_mode,
      fr_ref_sel_o      => fr_ref_sel,
      scan_method_o     => interlaced,
      timing_o          => phasedelay,
      reset_channel_o   => reset_channel,
      spl_is_zero_o     => spl_is_zero,
      --debug:
      debug_o           => debug
   );


tri_level_timing : Tri_level_timer
   port map (
      mreset_i          => reset_channel,
      frame_start_i     => frame_start,
      clk_i             => clk_i,
      interlaced_i      => interlaced,
      div10_11_i        => div10_11,
      sync_mode_lsb_i   => sync_mode(0),
      spl_div_i         => spl_div,
      lines_pr_frame_i  => lpf,
      framerate_x2_i    => framerate_x2,
      tsg_lvl_i         => tsg_level_i,
      mute_i            => mute,
      tsg_ok_o          => sampled_tsg_lvl,
      DAC_o             => DAC_o,
      timing_change_o   => timing_change_o
      --debug:
      --debug_o           => debug
   );


frame_sync_delaying : frame_sync_delay
   port map (
	   mreset_i      	   => reset_channel,
	   clk_i         	   => clk_i,
	   sync_i            => genlock_sync,
	   spl_div_i         => spl_div,
      div10_11_i        => div10_11,
	   lpf_i             => lpf,
	   samplerate_x2_i   => framerate_x2,
      phasedelay_i      => phasedelay,
	   frame_sync_o	   => frame_start
   );


-------------------------------------------------------------
-- Select genlock sync source:
-------------------------------------------------------------
sync_reclocking : process(reset_channel, clk_i)
begin
   if reset_channel = '0' then
      f4m   <= '0';   
      f8g   <= '0';
      f2398 <= '0';
      f24   <= '0';
      f30   <= '0';
   elsif clk_i'event and clk_i = '1' then
      f4m   <= f4m_i;
      f8g   <= f8g_i;
      f2398 <= f2398_i;
      f24   <= f24_i;
      f30   <= f30_i;
   end if;
end process; 


sync_selecting : process(reset_channel, clk_i)
begin
   if reset_channel = '0' then
      genlock_sync <= '0';
   elsif clk_i'event and clk_i = '1' then
      if sync_mode(1) = '1' then
         if sync_mode(0) = '0' then
            genlock_sync <= f8g;
         else
            genlock_sync <= f4m;
         end if;
      else
         if fr_ref_sel = "00" then
            genlock_sync <= f30;
         elsif fr_ref_sel = "10" then
            genlock_sync <= f24;
         elsif fr_ref_sel = "11" then
            genlock_sync <= f2398;
         end if;
      end if;
   end if;
end process;


------------------------------------------------------------
-- Output level monitoring:
------------------------------------------------------------
output_level_monitoring : output_level_monitor
   port map (
	   mreset_i       => mreset_i,
      clk_i          => f1485_i,
      tick_10ms_i    => tick_10ms_i,
      mute_i         => mute,
      spl_is_zero_i  => spl_is_zero,
      tsg_level_i    => sampled_tsg_lvl,
      output_ok_o    => tsg_ok_o
   );


------------------------------------------------------------
-- Backend outputs:
------------------------------------------------------------
clmp_o <= '0';


------------------------------------------------------------
-- Debug:
------------------------------------------------------------
--debug_o(15 downto 3) <= debug(15 downto 3);
debug_o(0) <= reset_channel;
debug_o(1) <= sampled_tsg_lvl;      
debug_o(2) <= tick_10ms_i;  
debug_o(4) <= mute;         
debug_o(5) <= spl_is_zero;  
debug_o(6) <= tsg_level_i;  
--debug_o(7) <= 
--debug_o(8) <= spl_div(7);
--debug_o(9) <= spl_div(6);
--debug_o(10) <= spl_div(5);
--debug_o(11) <= spl_div(4);
--debug_o(12) <= spl_div(3);
--debug_o(13) <= spl_div(2);
--debug_o(14) <= spl_div(1);
--debug_o(15) <= spl_div(0);


end Behavioral;