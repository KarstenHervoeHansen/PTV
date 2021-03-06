--///**********************************************************************
--// File:    spi_test.vhd
--// Project: PT8612 HD-SDI Test Pattern Generator
--// Date:    17.02.2006                             ram_address
--// Author:  JK
--// Purpose: To test the SPI interface handling communication between
--//          microcontroller and FPGA
--// Notes:   This is the top level module incorporating the SPI interface
--//          which is to be tested.  The test itself happens in read_Bram
--//          process.
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

signal asignal_t     :   std_logic_vector( 7 downto 0);

--// Below are constants defining addresses in chnnel RAMs where particular
--// settings are placed.
constant SYS_ADDR   : std_logic_vector(8 downto 0) := "000000000";  --// System                    
constant PAT_ADDR   : std_logic_vector(8 downto 0) := "000000001";  --// Pattern                   
constant ATT_ADDR   : std_logic_vector(8 downto 0) := "000000010";  --// Attribs                   
constant DEL_ADDR   : std_logic_vector(8 downto 0) := "000000011";  --// Delay                     
constant EDH_ADDR   : std_logic_vector(8 downto 0) := "000000111";  --// EDH                       
constant SIG_ADDR   : std_logic_vector(8 downto 0) := "000001000";  --// Audio Signal              
constant LEV_ADDR   : std_logic_vector(8 downto 0) := "000001001";  --// Audio Level               
constant GRO_ADDR   : std_logic_vector(8 downto 0) := "000001010";  --// Audio Group               
constant ASR_ADDR   : std_logic_vector(8 downto 0) := "000001011";  --// Audio Sample Rate         
constant POS_ADDR   : std_logic_vector(8 downto 0) := "000001100";  --// Text Position             
constant SCA_ADDR   : std_logic_vector(8 downto 0) := "000001110";  --// Text Scale                
constant COL_ADDR   : std_logic_vector(8 downto 0) := "000001111";  --// Text Color                
constant SIZ_ADDR   : std_logic_vector(8 downto 0) := "000010000";  --// Text Bitmap Size          
constant MOV_ADDR   : std_logic_vector(8 downto 0) := "000010010";  --// Text Movement             
constant XMIN_ADDR  : std_logic_vector(8 downto 0) := "000010011";  --// Text Movement Bound X MIN 
constant XMAX_ADDR  : std_logic_vector(8 downto 0) := "000010100";  --// Text Movement Bound X MAX 
constant YMIN_ADDR  : std_logic_vector(8 downto 0) := "000010101";  --// Text Movement Bound Y MIN 
constant YMAX_ADDR  : std_logic_vector(8 downto 0) := "000010110";  --// Text Movement Bound Y MAX 
constant TMS_ADDR   : std_logic_vector(8 downto 0) := "000010111";  --// Text Movement Speed       
constant TXT_ADDR   : std_logic_vector(8 downto 0) := "000011000";  --// Text Bitmap               


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
   --// interface to Channel RAM (in serial_interface):
   rd_add_o      : out std_logic_vector( 8 downto 0);
   rd_dat_i      : in  std_logic_vector( 7 downto 0);
   ready_i       : in  std_logic;
   --// output section:
   sb_clk_i      : in  std_logic;
   asignal_o     : out std_logic_vector( 7 downto 0)
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
   asignal_o     => asignal_t
   );


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
--// Read and display memory contents at SIG_ADDR
--// Note: channel RAM is read at rising edge of clk2 and
--//       when chx_ready = '1'.
--//-----------------------------------------------------------------
read_Bram: process(mreset, clk2)
   begin
      if mreset = '0' then
         led1_o <= '0';
         led2_o <= '0';
         led3_o <= '0';
         led4_o <= '0';
      elsif clk2'event and clk2 ='1' then
         led1_o <= asignal_t(0);
         led2_o <= asignal_t(1);
         led3_o <= asignal_t(2);
         led4_o <= asignal_t(3);
      end if;
   end process;

end Behavioral;