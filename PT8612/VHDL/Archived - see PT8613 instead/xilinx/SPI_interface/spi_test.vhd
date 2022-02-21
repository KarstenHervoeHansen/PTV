--///**********************************************************************
--// File:    spi_test.vhd
--// Project: PT8612 HD-SDI Test Pattern Generator
--// Date:    17.02.2006
--// Author:  JK
--// Purpose: To test the SPI interface handling communication between
--//          microcontroller and FPGA.
--// Notes:   This is the top level module incorporating the SPI interface
--//          which is to be tested.  The test itself happens in read_Bram
--//          process. agroup_t
--//***********************************************************************
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;

entity spi_test is
   Port
      (
      --//------- from Mainframe -----------------------------------------------------------------
      mreset_i      : in std_logic;   --// Master Reset fra mainboard

      --//------- clocks -------------------------------------------------------------------------
      brefclk2_p_i  : in std_logic;
      brefclk2_n_i  : in std_logic;

      --//------- from Controller ----------------------------------------------------------------
      sck_i         : in std_logic;   --// serial interface clock
      mosi_i        : in std_logic;   --// Master Out Serial data In
      cs1_i         : in std_logic;   --// chip select channel 1 (nss)

      --//------- to Controller ------------------------------------------------------------------
      ok1_o         : out std_logic;  --// generator 1&2 output levels OK - respons til mainframe
      ok2_o         : out std_logic;  --// generator 3&4 output levels OK - respons til mainframe

      --//------- to PLL's -----------------------------------------------------------------------
      pll1_S0_o     : out std_logic;
      pll1_S1_o     : out std_logic;
      pll1_S2_o     : out std_logic;
      pll1_S3_o     : out std_logic;
      pll2_S0_o     : out std_logic;
      pll2_S1_o     : out std_logic;
      pll2_S2_o     : out std_logic;
      pll2_S3_o     : out std_logic;

      --//------- HD Outputs ---------------------------------------------------------------------
      sd_zhd_1_o    : out std_logic;
      sd_zhd_2_o    : out std_logic;
      sd_zhd_3_o    : out std_logic;
      sd_zhd_4_o    : out std_logic;
      
      --//------- LED's --------------------------------------------------------------------------
      led1_o        : out std_logic;
      led2_o        : out std_logic;
      led3_o        : out std_logic;
      led4_o        : out std_logic
      );
end spi_test;


architecture Behavioral of spi_test is

signal mreset       : std_logic;
signal sck          : std_logic;
signal mosi         : std_logic;
signal cs1          : std_logic;

signal clk2         : std_logic;

signal brefclk2     : std_logic;

--// JK new 
signal ram1_address  : std_logic_vector( 8 downto 0);
signal ram2_address  : std_logic_vector( 8 downto 0);
signal ram3_address  : std_logic_vector( 8 downto 0);
signal ram4_address  : std_logic_vector( 8 downto 0);
signal ram1_data     : std_logic_vector( 7 downto 0);
signal ram2_data     : std_logic_vector( 7 downto 0);
signal ram3_data     : std_logic_vector( 7 downto 0);
signal ram4_data     : std_logic_vector( 7 downto 0);

signal ch1_ready    : std_logic;
signal ch2_ready    : std_logic;
signal ch3_ready    : std_logic;
signal ch4_ready    : std_logic;

signal sys_t        : std_logic_vector( 7 downto 0);
signal pat_t        : std_logic_vector(15 downto 0);
signal del_t        : std_logic_vector(23 downto 0);
signal aclick_t     : std_logic_vector(15 downto 0);
signal asignal_t    : std_logic_vector( 7 downto 0);
signal alevel_t     : std_logic_vector( 7 downto 0);
signal agroup_t     : std_logic_vector( 7 downto 0);
signal asr_t        : std_logic_vector( 7 downto 0);
signal pos_t        : std_logic_vector(15 downto 0);
signal sca_t        : std_logic_vector( 7 downto 0);
signal col_t        : std_logic_vector( 7 downto 0);
signal siz_t        : std_logic_vector(15 downto 0);
signal mov_t        : std_logic_vector( 7 downto 0);
signal xmin_t       : std_logic_vector( 7 downto 0);
signal xmax_t       : std_logic_vector( 7 downto 0);
signal ymin_t       : std_logic_vector( 7 downto 0);
signal ymax_t       : std_logic_vector( 7 downto 0);
signal tms_t        : std_logic_vector( 7 downto 0);

signal tick_10ms    : std_logic;
signal timing_change: std_logic;
signal reset_delayed: std_logic;
signal timing       : std_logic_vector(23 downto 0);



component serial_interface
   Port
      (
      spi_mreset_i    : in  std_logic;   --// master reset
      spi_clk_i       : in  std_logic;   --// clock
      spi_cs_i        : in  std_logic;   --// chip select
      spi_sck_i       : in  std_logic;   --// serial clock
      spi_mosi_i      : in  std_logic;   --// master out serial data in
      
      ch1_Bram_clk_i  : in  std_logic;  --// clock for reading ram
      ch1_Baddr_bus_i : in  std_logic_vector( 8 downto 0);
      ch1_Bdata_bus_o : out std_logic_vector( 7 downto 0);
      ch1_Bram_ok_o   : out std_logic;

      ch2_Bram_clk_i  : in  std_logic;  --// clock for reading ram
      ch2_Baddr_bus_i : in  std_logic_vector( 8 downto 0);
      ch2_Bdata_bus_o : out std_logic_vector( 7 downto 0);
      ch2_Bram_ok_o   : out std_logic;

      ch3_Bram_clk_i  : in  std_logic;  --// clock for reading ram
      ch3_Baddr_bus_i : in  std_logic_vector( 8 downto 0);
      ch3_Bdata_bus_o : out std_logic_vector( 7 downto 0);
      ch3_Bram_ok_o   : out std_logic;

      ch4_Bram_clk_i  : in  std_logic;  --// clock for reading ram
      ch4_Baddr_bus_i : in  std_logic_vector( 8 downto 0);
      ch4_Bdata_bus_o : out std_logic_vector( 7 downto 0);
      ch4_Bram_ok_o   : out std_logic
      );
end component;


component settings_buffer
   Port
   (
   sb_mreset_i   : in  std_logic;   --// master reset
   --// interface to Channel RAM:
   rd_add_o      : out std_logic_vector( 8 downto 0); --// address buss
   rd_dat_i      : in  std_logic_vector( 7 downto 0); --// data to read bus
   ready_i       : in  std_logic;                     --// data ready signal
   --// output section:
   sb_clk_i      : in  std_logic;
   sys_o         : out std_logic_vector( 7 downto 0);
   pat_o         : out std_logic_vector(15 downto 0);
   del_o         : out std_logic_vector(23 downto 0);
   aclick_o      : out std_logic_vector(15 downto 0);
   asignal_o     : out std_logic_vector( 7 downto 0);
   alevel_o      : out std_logic_vector( 7 downto 0);
   agroup_o      : out std_logic_vector( 7 downto 0);
   asr_o         : out std_logic_vector( 7 downto 0);
   pos_o         : out std_logic_vector(15 downto 0);
   sca_o         : out std_logic_vector( 7 downto 0);
   col_o         : out std_logic_vector( 7 downto 0);
   siz_o         : out std_logic_vector(15 downto 0);
   mov_o         : out std_logic_vector( 7 downto 0);
   xmin_o        : out std_logic_vector( 7 downto 0);
   xmax_o        : out std_logic_vector( 7 downto 0);
   ymin_o        : out std_logic_vector( 7 downto 0);
   ymax_o        : out std_logic_vector( 7 downto 0);
   tms_o         : out std_logic_vector( 7 downto 0)
   );
end component;


component glitch_remover
   port
      (
      clk_i            : in  std_logic;
      signal_i         : in  std_logic;
      clean_signal_o   : out std_logic
      );
end component;


component IBUFGDS
   generic(IOSTANDARD: string);
   port
      ( 
      O:  out std_ulogic;
      I:  in  std_ulogic;
      IB: in  std_ulogic
      );
end component;

--// JK addition
component BUFG
   Port
      (
      I : in  std_logic;
      O : out std_logic
      );
end component;

begin

--//------------------------------------------------------------
--// Remove glitches on input signals:
--//------------------------------------------------------------
   
glitch_removing_sck : glitch_remover
   port map
      (
      clk_i             => clk2,                               
      signal_i          => sck_i,
      clean_signal_o    => sck
      );


glitch_removing_mosi : glitch_remover
   port map
      (
      clk_i             => clk2,                               
      signal_i          => mosi_i,
      clean_signal_o    => mosi
      );

glitch_removing_cs1 : glitch_remover
   port map
      (
      clk_i             => clk2,                               
      signal_i          => cs1_i,
      clean_signal_o    => cs1
      );

glitch_removing_mreset : glitch_remover
   port map
      (
      clk_i             => clk2,                               
      signal_i          => mreset_i,
      clean_signal_o    => mreset
      );


diff_buf_brefclk2 : IBUFGDS
generic map (IOSTANDARD => "LVDS_25")
   port map
      (
      O  => brefclk2,
      I  => brefclk2_p_i,
      IB => brefclk2_n_i
      );


clk2_for_logic : BUFG
   port map
      (
      I => brefclk2,
      O => clk2
      );

--//-----------------------------------------------------
--// Serial interface 
--//-----------------------------------------------------
serial_interfacing : serial_interface
   Port map
      (
      spI_mreset_i     => mreset,
      spI_clk_i        => clk2,
      spI_cs_i         => cs1,
      spI_sck_i        => sck,
      spI_mosi_i       => mosi,

      ch1_Bram_clk_i   => clk2,
      ch1_Baddr_bus_i  => ram1_address,
      ch1_Bdata_bus_o  => ram1_data,
      ch1_Bram_ok_o    => ch1_ready,

      ch2_Bram_clk_i   => clk2,
      ch2_Baddr_bus_i  => ram2_address,
      ch2_Bdata_bus_o  => ram2_data,
      ch2_Bram_ok_o    => ch2_ready,

      ch3_Bram_clk_i   => clk2,
      ch3_Baddr_bus_i  => ram3_address,
      ch3_Bdata_bus_o  => ram3_data,
      ch3_Bram_ok_o    => ch3_ready,

      ch4_Bram_clk_i   => clk2,
      ch4_Baddr_bus_i  => ram4_address,
      ch4_Bdata_bus_o  => ram4_data,
      ch4_Bram_ok_o    => ch4_ready
      );

settings_buffering : settings_buffer
   Port map
   (   
   sb_mreset_i   => mreset,      
   rd_add_o      => ram1_address,
   rd_dat_i      => ram1_data,   
   ready_i       => ch1_ready,   
   sb_clk_i      => clk2,

   sys_o         => sys_t,     
   pat_o         => pat_t,     
   del_o         => del_t,     
   aclick_o      => aclick_t,     
   asignal_o     => asignal_t, 
   alevel_o      => alevel_t,  
   agroup_o      => agroup_t,  
   asr_o         => asr_t,     
   pos_o         => pos_t,     
   sca_o         => sca_t,     
   col_o         => col_t,     
   siz_o         => siz_t,     
   mov_o         => mov_t,     
   xmin_o        => xmin_t,    
   xmax_o        => xmax_t,    
   ymin_o        => ymin_t,    
   ymax_o        => ymax_t,    
   tms_o         => tms_t     
   );

--//--------------------------------------------------------------
--//-- Master reset delay and 10ms tick generation:
--//--------------------------------------------------------------
--//master_reset_delaying : master_reset_delay
--//port map(
--//   mreset_i          => mreset,
--//   clk_i             => clk2,       --148.5
--//   tick_10ms_o       => tick_10ms,
--//   reset_delayed_o   => reset_delayed
--//);
--//
--//led_latch1: led_latch
--//   port map (
--//      led_i       => timing_change,
--//      clk_i       => clk2,    -- 148.5
--//      tick_10ms_i => tick_10ms,
--//      led_o       => led1_o
--//   );
--//
--//-----------------------------------------------------------
--// Mapping of output signals:
--//-----------------------------------------------------------

--// PLL configuration:
pll1_S0_o <= '0';    --brefclk2 148.5
pll1_S1_o <= '1';
pll1_S2_o <= '1';
pll1_S3_o <= '0';

pll2_S0_o <= '1';    --brefclk 148.35
pll2_S1_o <= '1';
pll2_S2_o <= '1';
pll2_S3_o <= '0';

sd_zhd_1_o <= '0';
sd_zhd_2_o <= '0';
sd_zhd_3_o <= '0';
sd_zhd_4_o <= '0';

ok1_o <= '1';
ok2_o <= '1';


--//-----------------------------------------------------------------
--// Test the serial interface:
--// Read and display memory contents at CLI_ADDR = 7
--// Note: Data at asignal vector is available at all  times.
--//       Component settings_buffer assures proper updating of the
--//       vetor during a single clock transition.
--//-----------------------------------------------------------------
read_Bram: process(mreset, clk2)
   begin
      if mreset = '0' then
         led1_o <= '0';
         led2_o <= '0';
         led3_o <= '0';
         led4_o <= '0';
         timing_change <= '0';
      elsif clk2'event and clk2 ='1' then
         led1_o <= del_t(0);
         led2_o <= del_t(1);
         led3_o <= del_t(2);
         led4_o <= del_t(3);--//<'1';
      end if;
   end process;

--led1_o <= sck_i;
--led2_o <= mosi_i;
       

end Behavioral;