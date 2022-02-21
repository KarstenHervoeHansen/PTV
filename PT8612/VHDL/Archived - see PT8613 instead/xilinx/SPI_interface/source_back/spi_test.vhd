--///**********************************************************************
--// File:    spi_test.vhd
--// Project: PT8612 HD-SDI Test Pattern Generator
--// Date:    17.02.2006                             ch_Bram_clk_i
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
signal ram1_address  : std_logic_vector(11 downto 0);
signal ram2_address  : std_logic_vector(11 downto 0);
signal ram3_address  : std_logic_vector(11 downto 0);
signal ram4_address  : std_logic_vector(11 downto 0);
signal ram1_data     : std_logic_vector( 7 downto 0);
signal ram2_data     : std_logic_vector( 7 downto 0);
signal ram3_data     : std_logic_vector( 7 downto 0);
signal ram4_data     : std_logic_vector( 7 downto 0);

signal ch1_ready    : std_logic;
signal ch2_ready    : std_logic;
signal ch3_ready    : std_logic;
signal ch4_ready    : std_logic;

type channel_settings is record
   system:  std_logic_vector( 7 downto 0);
   pattern: std_logic_vector(15 downto 0);
   delay:   std_logic_vector(23 downto 0);
end record;

type channel_array is array(4 downto 1) of channel_settings;

signal settings: channel_array;

--// Below are constants defining addresses in chnnel RAMs where particular
--// settings are placed.
constant SYS_ADDR   : std_logic_vector(11 downto 0) := "000000000000";  --// System                    */
constant PAT_ADDR   : std_logic_vector(11 downto 0) := "000000000001";  --// Pattern                   */
constant ATT_ADDR   : std_logic_vector(11 downto 0) := "000000000010";  --// Attribs                   */
constant DEL_ADDR   : std_logic_vector(11 downto 0) := "000000000011";  --// Delay                     */
constant EDH_ADDR   : std_logic_vector(11 downto 0) := "000000000111";  --// EDH                       */
constant SIG_ADDR   : std_logic_vector(11 downto 0) := "000000001000";  --// Audio Signal              */
constant LEV_ADDR   : std_logic_vector(11 downto 0) := "000000001001";  --// Audio Level               */
constant GRO_ADDR   : std_logic_vector(11 downto 0) := "000000001010";  --// Audio Group               */
constant ASR_ADDR   : std_logic_vector(11 downto 0) := "000000001011";  --// Audio Sample Rate         */
constant POS_ADDR   : std_logic_vector(11 downto 0) := "000000001100";  --// Text Position             */
constant SCA_ADDR   : std_logic_vector(11 downto 0) := "000000001110";  --// Text Scale                */
constant COL_ADDR   : std_logic_vector(11 downto 0) := "000000001111";  --// Text Color                */
constant SIZ_ADDR   : std_logic_vector(11 downto 0) := "000000010000";  --// Text Bitmap Size          */
constant MOV_ADDR   : std_logic_vector(11 downto 0) := "000000010010";  --// Text Movement             */
constant XMIN_ADDR  : std_logic_vector(11 downto 0) := "000000010011";  --// Text Movement Bound X MIN */
constant XMAX_ADDR  : std_logic_vector(11 downto 0) := "000000010100";  --// Text Movement Bound X MAX */
constant YMIN_ADDR  : std_logic_vector(11 downto 0) := "000000010101";  --// Text Movement Bound Y MIN */
constant YMAX_ADDR  : std_logic_vector(11 downto 0) := "000000010110";  --// Text Movement Bound Y MAX */
constant TMS_ADDR   : std_logic_vector(11 downto 0) := "000000010111";  --// Text Movement Speed       */
constant TXT_ADDR   : std_logic_vector(11 downto 0) := "000000011000";  --// Text Bitmap               */



component serial_interface
   Port
      (
      spi_mreset_i    : in  std_logic;   --// master reset
      spi_clk_i       : in  std_logic;   --// clock
      spi_cs_i        : in  std_logic;   --// chip select
      spi_sck_i       : in  std_logic;   --// serial clock
      spi_mosi_i      : in  std_logic;   --// master out serial data in
      
      --// Common channel ram address bus for reading by the rest of FPGA:
      --// four data and address buses for reading and four 'data ready' signals
      --// and four clocks
      ch1_Bram_clk_i  : in  std_logic;  --// clock for reading ram1
      ch1_Baddr_bus_i : in  std_logic_vector(11 downto 0);
      ch1_Bdata_bus_o : out std_logic_vector( 7 downto 0);
      ch1_Bram_ok_o   : out std_logic;

      ch2_Bram_clk_i  : in  std_logic;  --// clock for reading ram2
      ch2_Baddr_bus_i : in  std_logic_vector(11 downto 0);
      ch2_Bdata_bus_o : out std_logic_vector( 7 downto 0);
      ch2_Bram_ok_o   : out std_logic;

      ch3_Bram_clk_i  : in  std_logic;  --// clock for reading ram3
      ch3_Baddr_bus_i : in  std_logic_vector(11 downto 0);
      ch3_Bdata_bus_o : out std_logic_vector( 7 downto 0);
      ch3_Bram_ok_o   : out std_logic;

      ch4_Bram_clk_i  : in  std_logic;  --// clock for reading ram4
      ch4_Baddr_bus_i : in  std_logic_vector(11 downto 0);
      ch4_Bdata_bus_o : out std_logic_vector( 7 downto 0);
      ch4_Bram_ok_o   : out std_logic
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

--//-----------------------------------------------------
--// Control of channel memory read addresses:
--// 
ram_address_cnt: process(mreset, clk2, ch1_ready, ch2_ready, ch3_ready, ch4_ready)
   begin
      if mreset = '0' then
         ram1_address <= "000000000000";
         ram2_address <= "000000000000";
         ram3_address <= "000000000000";
         ram4_address <= "000000000000";
      elsif clk2'event and clk2 ='1' then
         ram1_address <= ram1_address + 1;
         if ram1_address = EDH_ADDR then
            ram1_address <= "000000000000";
         end if;

         ram2_address <= ram2_address + 1;
         if ram2_address = EDH_ADDR then
            ram2_address <= "000000000000";
         end if;

         ram3_address <= ram3_address + 1;
         if ram3_address = EDH_ADDR then
            ram3_address <= "000000000000";
         end if;

         ram4_address <= ram4_address + 1;
         if ram4_address = EDH_ADDR then
            ram4_address <= "000000000000";
         end if;
      end if;
   end process;

--//-----------------------------------------------------
--// Test the serial interface:
--// Signal which channel is currently updated with LEDs.
--// Read channel memory at current address and write data
--// read to appropriate member of settings record.
--//-----------------------------------------------------
read_Bram: process(mreset, clk2, ch1_ready, ch2_ready, ch3_ready, ch4_ready)
   begin
      if clk2'event and clk2 ='1' then
         if ch1_ready = '1' then
            led1_o <= '1';
            led2_o <= '0';
            led3_o <= '0';
            led4_o <= '0';
            case ram1_address is
               when SYS_ADDR  => 
                  settings(1).system <= ram1_data;
               when PAT_ADDR  => 
                  settings(1).pattern(15 downto 8) <= ram1_data;
               when ATT_ADDR  => 
                  settings(1).pattern( 7 downto 0) <= ram1_data;
               when DEL_ADDR  => 
                  settings(1).delay(23 downto 16) <= ram1_data;
               when "000000000100"  => 
                  settings(1).delay(15 downto  8) <= ram1_data;
               when "000000000101"  => 
                  settings(1).delay( 7 downto  0) <= ram1_data;
               when others =>
                  null;
            end case;

         elsif  ch2_ready = '1' then
            led1_o <= '0';
            led2_o <= '1';
            led3_o <= '0';
            led4_o <= '0';
            case ram2_address is
               when SYS_ADDR  => 
                  settings(2).system <= ram2_data;
               when PAT_ADDR  => 
                  settings(2).pattern(15 downto 8) <= ram2_data;
               when ATT_ADDR  => 
                  settings(2).pattern( 7 downto 0) <= ram2_data;
               when DEL_ADDR  => 
                  settings(2).delay(23 downto 16) <= ram2_data;
               when "000000000100"  => 
                  settings(2).delay(15 downto  8) <= ram2_data;
               when "000000000101"  => 
                  settings(2).delay( 7 downto  0) <= ram2_data;
               when others =>
                  null;
            end case;
         elsif  ch3_ready = '1' then
            led1_o <= '0';
            led2_o <= '0';
            led3_o <= '1';
            led4_o <= '0';
            case ram3_address is
               when SYS_ADDR  => 
                  settings(3).system <= ram3_data;
               when PAT_ADDR  => 
                  settings(3).pattern(15 downto 8) <= ram3_data;
               when ATT_ADDR  => 
                  settings(3).pattern( 7 downto 0) <= ram3_data;
               when DEL_ADDR  => 
                  settings(3).delay(23 downto 16) <= ram3_data;
               when "000000000100"  => 
                  settings(3).delay(15 downto  8) <= ram3_data;
               when "000000000101"  => 
                  settings(3).delay( 7 downto  0) <= ram3_data;
               when others =>
                  null;
            end case;
         elsif  ch4_ready = '1' then
            led1_o <= '0';
            led2_o <= '0';
            led3_o <= '0';
            led4_o <= '1';
            case ram4_address is
               when SYS_ADDR  => 
                  settings(4).system <= ram4_data;
               when PAT_ADDR  => 
                  settings(4).pattern(15 downto 8) <= ram4_data;
               when ATT_ADDR  => 
                  settings(4).pattern( 7 downto 0) <= ram4_data;
               when DEL_ADDR  => 
                  settings(4).delay(23 downto 16) <= ram4_data;
               when "000000000100"  => 
                  settings(4).delay(15 downto  8) <= ram4_data;
               when "000000000101"  => 
                  settings(4).delay( 7 downto  0) <= ram4_data;
               when others =>
                  null;
            end case;
         end if;
      end if;
   end process;





end Behavioral;


