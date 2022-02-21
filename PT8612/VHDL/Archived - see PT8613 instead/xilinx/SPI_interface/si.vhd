--///*******************************************************************
--// File:    serial_interface.vhd
--// Project: PT8612 HD-SDI Test Pattern Generator
--// Date:    07.02.2006
--// Author:  JK
--// Purpose: SPI interface handling communication between
--//          microcontroller and the rest of FPGA
--// Notes:   This interface implements transfer of generator
--//          settings as described in uc_fpga05.doc. 
--//          entity serial_interface incorporates 4 dual port memories
--//          where settings received from microcontroller are stored at
--//          predetermined addresses.  The rest of FPGA hardware can access
--//          these settings via 4 output busses.
--//*********************************************************************
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity serial_interface is
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
      ch1_Bram_clk_i     : in  std_logic;  --// clock for reading ram1
      system1_o          : out std_logic_vector( 7 downto 0);
      pattern1_o         : out std_logic_vector(15 downto 0);
      delay1_o           : out std_logic_vector(23 downto 0);
      ch1_read_settings_i: in  std_logic;

      ch2_Bram_clk_i     : in  std_logic;  --// clock for reading ram2
      system2_o          : out std_logic_vector( 7 downto 0);
      pattern2_o         : out std_logic_vector(15 downto 0);
      delay2_o           : out std_logic_vector(23 downto 0);
      ch2_read_settings_i: in  std_logic;

      ch3_Bram_clk_i     : in  std_logic;  --// clock for reading ram3
      system3_o          : out std_logic_vector( 7 downto 0);
      pattern3_o         : out std_logic_vector(15 downto 0);
      delay3_o           : out std_logic_vector(23 downto 0);
      ch3_read_settings_i: in  std_logic;

      ch4_Bram_clk_i     : in  std_logic;  --// clock for reading ram4
      system4_o          : out std_logic_vector( 7 downto 0);
      pattern4_o         : out std_logic_vector(15 downto 0);
      delay4_o           : out std_logic_vector(23 downto 0);
      ch4_read_settings_i: in  std_logic
      );
end serial_interface;


architecture Behavioral of serial_interface is


signal spi_cs                 : std_logic;
signal spi_sck                : std_logic;
signal spi_mosi               : std_logic;
signal sck_tick               : std_logic;

signal sck_delayed            : std_logic;
signal bitptr                 : integer range 0 to 63;

signal channel_sel            : std_logic_vector( 7 downto 0); --// select channel RAM
signal tmp_selector           : std_logic_vector( 7 downto 0);
signal permit_Bram1_read      : std_logic;
signal permit_Bram2_read      : std_logic;
signal permit_Bram3_read      : std_logic;
signal permit_Bram4_read      : std_logic;
signal permit_ram_write       : std_logic;                     --// enable writing to channel RAMs
signal channel_ram_Aaddr_bus  : std_logic_vector( 8 downto 0);
signal channel_ram_Adata_bus  : std_logic_vector( 7 downto 0);
signal channel_is_known       : std_logic;

--// JK new 
signal ram1_read_address  : std_logic_vector( 8 downto 0);
signal ram2_read_address  : std_logic_vector( 8 downto 0);
signal ram3_read_address  : std_logic_vector( 8 downto 0);
signal ram4_read_address  : std_logic_vector( 8 downto 0);
signal ram1_read_data     : std_logic_vector( 7 downto 0);
signal ram2_read_data     : std_logic_vector( 7 downto 0);
signal ram3_read_data     : std_logic_vector( 7 downto 0);
signal ram4_read_data     : std_logic_vector( 7 downto 0);

type channel_settings is record
   system:  std_logic_vector( 7 downto 0);
   pattern: std_logic_vector(15 downto 0);
   delay:   std_logic_vector(23 downto 0);
end record;

type channel_array is array(4 downto 1) of channel_settings;

signal settings: channel_array;

signal tmp_delay1: std_logic_vector(23 downto 0);
signal tmp_delay2: std_logic_vector(23 downto 0);
signal tmp_delay3: std_logic_vector(23 downto 0);
signal tmp_delay4: std_logic_vector(23 downto 0);

--// Below are constants defining addresses in chnnel RAMs where particular
--// settings are placed.
constant SYS_ADDR   : std_logic_vector( 8 downto 0) := "000000000";  --// System                    */
constant PAT_ADDR   : std_logic_vector( 8 downto 0) := "000000001";  --// Pattern                   */
constant ATT_ADDR   : std_logic_vector( 8 downto 0) := "000000010";  --// Attribs                   */
constant DEL_ADDR   : std_logic_vector( 8 downto 0) := "000000011";  --// Delay                     */
constant EDH_ADDR   : std_logic_vector( 8 downto 0) := "000000111";  --// EDH                       */
constant SIG_ADDR   : std_logic_vector( 8 downto 0) := "000001000";  --// Audio Signal              */
constant LEV_ADDR   : std_logic_vector( 8 downto 0) := "000001001";  --// Audio Level               */
constant GRO_ADDR   : std_logic_vector( 8 downto 0) := "000001010";  --// Audio Group               */
constant ASR_ADDR   : std_logic_vector( 8 downto 0) := "000001011";  --// Audio Sample Rate         */
constant POS_ADDR   : std_logic_vector( 8 downto 0) := "000001100";  --// Text Position             */
constant SCA_ADDR   : std_logic_vector( 8 downto 0) := "000001110";  --// Text Scale                */
constant COL_ADDR   : std_logic_vector( 8 downto 0) := "000001111";  --// Text Color                */
constant SIZ_ADDR   : std_logic_vector( 8 downto 0) := "000010000";  --// Text Bitmap Size          */
constant MOV_ADDR   : std_logic_vector( 8 downto 0) := "000010010";  --// Text Movement             */
constant XMIN_ADDR  : std_logic_vector( 8 downto 0) := "000010011";  --// Text Movement Bound X MIN */
constant XMAX_ADDR  : std_logic_vector( 8 downto 0) := "000010100";  --// Text Movement Bound X MAX */
constant YMIN_ADDR  : std_logic_vector( 8 downto 0) := "000010101";  --// Text Movement Bound Y MIN */
constant YMAX_ADDR  : std_logic_vector( 8 downto 0) := "000010110";  --// Text Movement Bound Y MAX */
constant TMS_ADDR   : std_logic_vector( 8 downto 0) := "000010111";  --// Text Movement Speed       */
constant TXT_ADDR   : std_logic_vector( 8 downto 0) := "000011000";  --// Text Bitmap               */

component ch_ram
   port
      (
      --// input (SPI) part (write only)
      clka:  IN  std_logic;
      addra: IN  std_logic_VECTOR( 8 downto 0);
      dina:  IN  std_logic_VECTOR( 7 downto 0);
      wea:   IN  std_logic;                      --// write enable for part A (active hi)
      ena:   IN  std_logic;
      
      --// output (FPGA) (read only)
      clkb:  IN  std_logic;
      addrb: IN  std_logic_VECTOR( 8 downto 0);
      doutb: OUT std_logic_VECTOR( 7 downto 0);
      enb:   IN  std_logic
      );
end component;

begin

chan1_ram: ch_ram
   port map
      (
      clka   => spi_clk_i,
      addra  => channel_ram_Aaddr_bus,
      dina   => channel_ram_Adata_bus,
      ena    => channel_sel(0),
      wea    => permit_ram_write,
      
      clkb   => ch1_Bram_clk_i,
      addrb  => ram1_read_address,
      doutb  => ram1_read_data,
      enb    => permit_Bram1_read
      );
      
chan2_ram: ch_ram
   port map
      (
      clka   => spi_clk_i,
      addra  => channel_ram_Aaddr_bus,
      dina   => channel_ram_Adata_bus,
      ena    => channel_sel(1),
      wea    => permit_ram_write,
      
      clkb   => ch2_Bram_clk_i,
      addrb  => ram2_read_address,
      doutb  => ram2_read_data,
      enb    => permit_Bram2_read
      );
      
chan3_ram: ch_ram
   port map
      (
      clka   => spi_clk_i,
      addra  => channel_ram_Aaddr_bus,
      dina   => channel_ram_Adata_bus,
      ena    => channel_sel(2),
      wea    => permit_ram_write,
      
      clkb   => ch3_Bram_clk_i,
      addrb  => ram3_read_address,
      doutb  => ram3_read_data,
      enb    => permit_Bram3_read
      );
      
chan4_ram: ch_ram
   port map
      (
      clka   => spi_clk_i,
      addra  => channel_ram_Aaddr_bus,
      dina   => channel_ram_Adata_bus,
      ena    => channel_sel(3),
      wea    => permit_ram_write,
      
      clkb   => ch4_Bram_clk_i,
      addrb  => ram4_read_address,
      doutb  => ram4_read_data,
      enb    => permit_Bram4_read
      );
      

--//--------------------------------------------------------------
--// Reclocking of signals from serial interface to spi_clk_i domain:
--//--------------------------------------------------------------
signal_reclocking: process(spi_mreset_i, spi_clk_i)
   begin
      if spi_mreset_i = '0' then
         spi_cs             <= '1';
         spi_sck            <= '0';
         sck_delayed        <= '0';
         spi_mosi           <= '0';
      elsif spi_clk_i'event and spi_clk_i ='1' then
         spi_cs             <= spi_cs_i;
         spi_sck            <= spi_sck_i;
         sck_delayed        <= spi_sck;
         spi_mosi           <= spi_mosi_i;
      end if;
   end process;

--//--------------------------------------------------------------
--// Control of 'channel_data_ready' signals:
--//--------------------------------------------------------------
data_ready: process(spi_mreset_i, spi_clk_i)
   begin
      if spi_mreset_i = '0' then      
         permit_Bram1_read <= '0';
         permit_Bram2_read <= '0';
         permit_Bram3_read <= '0';
         permit_Bram4_read <= '0';
      elsif spi_clk_i'event and spi_clk_i = '1' then
         --// When SPI transfer is going on and the channel is already known,
         --// we must NOT permit reading the RAM which the SPI writes to...
         --// for example, if SPI transfer writes to RAM 3, permit_Bram3_read will be
         --// disabled...
         if channel_is_known = '1' then
            permit_Bram1_read <= not channel_sel(0);  --// reading ch1 Bram possible
            permit_Bram2_read <= not channel_sel(1);  --// reading ch2 Bram possible
            permit_Bram3_read <= not channel_sel(2);  --// reading ch3 Bram possible
            permit_Bram4_read <= not channel_sel(3);  --// reading ch4 Bram possible
         else
            permit_Bram1_read <= '1';
            permit_Bram2_read <= '1';
            permit_Bram3_read <= '1';
            permit_Bram4_read <= '1';
         end if;
      end if;
   end process;

--//ch1_Bram_ok_o <= permit_Bram1_read;
--//ch2_Bram_ok_o <= permit_Bram2_read;
--//ch3_Bram_ok_o <= permit_Bram3_read;
--//ch4_Bram_ok_o <= permit_Bram4_read;

--//--------------------------------------------------------------
--// Make tick at spi_sck falling edge:
--//--------------------------------------------------------------
sck_ticking: process(spi_mreset_i, spi_clk_i) begin
   if spi_mreset_i = '0' then
      sck_tick <= '0';
   elsif spi_clk_i'event and spi_clk_i ='1' then
      if spi_sck = '0' and sck_delayed = '1' and spi_cs = '0' then
         sck_tick <= '1';
      else
         sck_tick <= '0';
      end if;
   end if;
end process;


--//--------------------------------------------------------------
--// Make pointer to clock in data at spi_sck faling edge:
--// If pointer is 32 (7 bits of data received), it is wrapped to
--// 25 (to receive next 7 bits of data).
--//--------------------------------------------------------------
make_bitptr : process(spi_mreset_i, spi_clk_i, spi_cs)
   begin
      if spi_mreset_i = '0' then
         bitptr <= 1;
      elsif spi_clk_i'event and spi_clk_i = '1' then
         if spi_cs = '1' then
            bitptr <=  1;
         elsif sck_tick = '1' then
            if bitptr = 32 then
               bitptr <= 25;
            else
               bitptr <= bitptr + 1;
            end if;
         end if;
      end if;
   end process;


--//--------------------------------------------------------------
--// Clock in serial data:
--// Receives: Channel number to channel_sel
--//           RAM address to    channel_ram_Aaddr_bus
--//           Data bytes to     channel_ram_Adata_bus
--// Data is received as long as spi_cs is low.
--// channel_ram_Adata_bus is written into RAM when 8 data bits 
--// are received (bitptr = 32).
--// channel_ram_Aaddr_bus is incremented after RAM write.
--//--------------------------------------------------------------
get_serial_data : process(spi_mreset_i, spi_clk_i, spi_cs)
   begin
      if spi_mreset_i = '0' or spi_cs = '1' then
         permit_ram_write <= '0';
         channel_is_known <= '0';
      elsif spi_clk_i'event and spi_clk_i = '1' then
         case bitptr is
            when 32 =>
               if sck_tick = '1' then
                  permit_ram_write <= '1';
               else
                  permit_ram_write <= '0';
               end if;
            when others =>
               permit_ram_write <= '0';
         end case;
         --// increment address bus after write
         if permit_ram_write = '1' then
            channel_ram_Aaddr_bus <= channel_ram_Aaddr_bus + 1;
         end if;
   
         if sck_tick = '1' then
            case bitptr is
               --// get generator number
               when 5  => tmp_selector(3)   <= spi_mosi;
               when 6  => tmp_selector(2)   <= spi_mosi;
               when 7  => tmp_selector(1)   <= spi_mosi;
               when 8  => 
                  tmp_selector(0)  <= spi_mosi;
                  channel_is_known <= '1';
                  channel_sel      <= tmp_selector;
               --// get RAM address MSB
               when 16 => channel_ram_Aaddr_bus(8) <= spi_mosi;
               --// get RAM address LSB
               when 17 => channel_ram_Aaddr_bus(7)  <= spi_mosi;
               when 18 => channel_ram_Aaddr_bus(6)  <= spi_mosi;
               when 19 => channel_ram_Aaddr_bus(5)  <= spi_mosi;
               when 20 => channel_ram_Aaddr_bus(4)  <= spi_mosi;
               when 21 => channel_ram_Aaddr_bus(3)  <= spi_mosi;
               when 22 => channel_ram_Aaddr_bus(2)  <= spi_mosi;
               when 23 => channel_ram_Aaddr_bus(1)  <= spi_mosi;
               when 24 => channel_ram_Aaddr_bus(0)  <= spi_mosi;
   
               --// get data of variable length (length may be 0, but then CS will go inactive)
               when 25 => channel_ram_Adata_bus(7)  <= spi_mosi;
               when 26 => channel_ram_Adata_bus(6)  <= spi_mosi;
               when 27 => channel_ram_Adata_bus(5)  <= spi_mosi;
               when 28 => channel_ram_Adata_bus(4)  <= spi_mosi;
               when 29 => channel_ram_Adata_bus(3)  <= spi_mosi;
               when 30 => channel_ram_Adata_bus(2)  <= spi_mosi;
               when 31 => channel_ram_Adata_bus(1)  <= spi_mosi;
               when 32 => channel_ram_Adata_bus(0)  <= spi_mosi;
   
               when others =>
                          null;
            end case;
         end if; --// if sck_tick = '1'...
      end if;    --// if spi_clk_i'event and spi_clk_i = '1'
   end process;


--//-----------------------------------------------------
--// Control of channel 1 memory read addresses:
--// 
ram1_address_cnt: process(spi_mreset_i, ch1_Bram_clk_i)
   begin
      if spi_mreset_i = '0' then
         ram1_read_address <= "000000000";
      elsif ch1_Bram_clk_i'event and ch1_Bram_clk_i ='1' then
         ram1_read_address <= ram1_read_address + 1;
         if ram1_read_address = EDH_ADDR then
            ram1_read_address <= "000000000";
         end if;
      end if;
   end process;


--//-----------------------------------------------------
--// Control of channel 2 memory read addresses:
--// 
ram2_address_cnt: process(spi_mreset_i, ch2_Bram_clk_i)
   begin
      if spi_mreset_i = '0' then
         ram2_read_address <= "000000000";
      elsif ch2_Bram_clk_i'event and ch2_Bram_clk_i ='1' then
         ram2_read_address <= ram2_read_address + 1;
         if ram2_read_address = EDH_ADDR then
            ram2_read_address <= "000000000";
         end if;
      end if;
   end process;


--//-----------------------------------------------------
--// Control of channel 3 memory read addresses:
--// 
ram3_address_cnt: process(spi_mreset_i, ch3_Bram_clk_i)
   begin
      if spi_mreset_i = '0' then
         ram3_read_address <= "000000000";
      elsif ch3_Bram_clk_i'event and ch3_Bram_clk_i ='1' then
         ram3_read_address <= ram3_read_address + 1;
         if ram3_read_address = EDH_ADDR then
            ram3_read_address <= "000000000";
         end if;
      end if;
   end process;


--//-----------------------------------------------------
--// Control of channel 4 memory read addresses:
--// 
ram4_address_cnt: process(spi_mreset_i, ch4_Bram_clk_i)
   begin
      if spi_mreset_i = '0' then
         ram4_read_address <= "000000000";
      elsif ch4_Bram_clk_i'event and ch4_Bram_clk_i ='1' then
         ram4_read_address <= ram4_read_address + 1;
         if ram4_read_address = EDH_ADDR then
            ram4_read_address <= "000000000";
         end if;
      end if;
   end process;


--// Read channel 1 memory at current address and write data 
--// read to appropriate member of settings record.
read_Bram1: process(ch1_Bram_clk_i)
   begin
      if ch1_Bram_clk_i'event and ch1_Bram_clk_i ='1' then
         if permit_Bram1_read = '1' then
            case ram1_read_address is
               when SYS_ADDR  => 
                  settings(1).system               <= ram1_read_data;
               when PAT_ADDR  => 
                  settings(1).pattern(15 downto 8) <= ram1_read_data;
               when ATT_ADDR  => 
                  settings(1).pattern( 7 downto 0) <= ram1_read_data;
               when DEL_ADDR  =>
                  tmp_delay1(23 downto 16)          <= ram1_read_data;
                  --//settings(1).delay(23 downto 16)  <= ram1_read_data;
               when "000000100"  => 
                  tmp_delay1(15 downto  8)          <= ram1_read_data;
                  --//settings(1).delay(15 downto  8)  <= ram1_read_data;
               when "000000101"  => 
                  settings(1).delay( 7 downto  0)  <= ram1_read_data;
                  settings(1).delay(15 downto  8)  <= tmp_delay1(15 downto  8);
                  settings(1).delay(23 downto 16)  <= tmp_delay1(23 downto 16);
               when others =>
                  null;
            end case;
         end if;
      end if;
   end process;


system1_o  <= settings(1).system;
pattern1_o <= settings(1).pattern;
delay1_o   <= settings(1).delay;


--// Read channel 2 memory at current address and write data
--// read to appropriate member of settings record.
read_Bram2: process(ch2_Bram_clk_i)
   begin
      if ch2_Bram_clk_i'event and ch2_Bram_clk_i ='1' then
         if permit_Bram2_read = '1' then
            case ram2_read_address is
               when SYS_ADDR  => 
                  settings(2).system               <= ram2_read_data;
               when PAT_ADDR  => 
                  settings(2).pattern(15 downto 8) <= ram2_read_data;
               when ATT_ADDR  => 
                  settings(2).pattern( 7 downto 0) <= ram2_read_data;
               when DEL_ADDR  =>
                  tmp_delay2(23 downto 16)          <= ram2_read_data;
                  --//settings(2).delay(23 downto 16)  <= ram2_read_data;
               when "000000100"  => 
                  tmp_delay2(15 downto  8)          <= ram2_read_data;
                  --//settings(2).delay(15 downto  8)  <= ram2_read_data;
               when "000000101"  => 
                  settings(2).delay( 7 downto  0)  <= ram2_read_data;
                  settings(2).delay(15 downto  8)  <= tmp_delay2(15 downto  8);
                  settings(2).delay(23 downto 16)  <= tmp_delay2(23 downto 16);
               when others =>
                  null;
            end case;
         end if;
      end if;
   end process;

system2_o  <= settings(2).system;
pattern2_o <= settings(2).pattern;
delay2_o   <= settings(2).delay;

--// Read channel 3 memory at current address and write data
--// read to appropriate member of settings record.
read_Bram3: process(ch3_Bram_clk_i)
   begin
      if ch3_Bram_clk_i'event and ch3_Bram_clk_i ='1' then
         if permit_Bram3_read = '1' then
            case ram3_read_address is
               when SYS_ADDR  => 
                  settings(3).system               <= ram3_read_data;
               when PAT_ADDR  => 
                  settings(3).pattern(15 downto 8) <= ram3_read_data;
               when ATT_ADDR  => 
                  settings(3).pattern( 7 downto 0) <= ram3_read_data;
               when DEL_ADDR  =>
                  tmp_delay3(23 downto 16)          <= ram3_read_data;
                  --//settings(3).delay(23 downto 16)  <= ram3_read_data;
               when "000000100"  => 
                  tmp_delay3(15 downto  8)          <= ram3_read_data;
                  --//settings(3).delay(15 downto  8)  <= ram3_read_data;
               when "000000101"  => 
                  settings(3).delay( 7 downto  0)  <= ram3_read_data;
                  settings(3).delay(15 downto  8)  <= tmp_delay3(15 downto  8);
                  settings(3).delay(23 downto 16)  <= tmp_delay3(23 downto 16);
               when others =>
                  null;
            end case;
         end if;
      end if;
   end process;

system3_o  <= settings(3).system;
pattern3_o <= settings(3).pattern;
delay3_o   <= settings(3).delay;

--// Read channel 4 memory at current address and write data
--// read to appropriate member of settings record.
read_Bram4: process(ch4_Bram_clk_i)
   begin
      if ch4_Bram_clk_i'event and ch4_Bram_clk_i ='1' then
         if permit_Bram4_read = '1' then
            case ram4_read_address is
               when SYS_ADDR  => 
                  settings(4).system               <= ram4_read_data;
               when PAT_ADDR  => 
                  settings(4).pattern(15 downto 8) <= ram4_read_data;
               when ATT_ADDR  => 
                  settings(4).pattern( 7 downto 0) <= ram4_read_data;
               when DEL_ADDR  =>
                  tmp_delay4(23 downto 16)          <= ram4_read_data;
                  --//settings(4).delay(23 downto 16)  <= ram4_read_data;
               when "000000100"  => 
                  tmp_delay4(15 downto  8)          <= ram4_read_data;
                  --//settings(4).delay(15 downto  8)  <= ram4_read_data;
               when "000000101"  => 
                  settings(4).delay( 7 downto  0)  <= ram4_read_data;
                  settings(4).delay(15 downto  8)  <= tmp_delay4(15 downto  8);
                  settings(4).delay(23 downto 16)  <= tmp_delay4(23 downto 16);
               when others =>
                  null;
            end case;
         end if;
      end if;
   end process;

system4_o  <= settings(4).system;
pattern4_o <= settings(4).pattern;
delay4_o   <= settings(4).delay;


end Behavioral;
