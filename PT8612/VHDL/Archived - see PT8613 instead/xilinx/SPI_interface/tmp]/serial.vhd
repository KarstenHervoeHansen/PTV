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
      addrb  => ch1_Baddr_bus_i,
      doutb  => ch1_Bdata_bus_o,
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
      addrb  => ch2_Baddr_bus_i,
      doutb  => ch2_Bdata_bus_o,
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
      addrb  => ch3_Baddr_bus_i,
      doutb  => ch3_Bdata_bus_o,
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
      addrb  => ch4_Baddr_bus_i,
      doutb  => ch4_Bdata_bus_o,
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
         if spi_cs = '1' then
            permit_Bram1_read <= '1';  --// reading ch1 Bram possible
            permit_Bram2_read <= '1';  --// reading ch2 Bram possible
            permit_Bram3_read <= '1';  --// reading ch3 Bram possible
            permit_Bram4_read <= '1';  --// reading ch4 Bram possible
         else
            permit_Bram1_read <= not channel_sel(0);
            permit_Bram2_read <= not channel_sel(1);
            permit_Bram3_read <= not channel_sel(2);
            permit_Bram4_read <= not channel_sel(3);
         end if;
      end if;
   end process;

ch1_Bram_ok_o <= permit_Bram1_read;
ch2_Bram_ok_o <= permit_Bram2_read;
ch3_Bram_ok_o <= permit_Bram3_read;
ch4_Bram_ok_o <= permit_Bram4_read;

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
               when 9 =>  --// this means that address MSB goes in, but we don't use it...
                  channel_is_known <= '1';
                  channel_sel      <= tmp_selector;
               --// get RAM address MSB
               when 16 => channel_ram_Aaddr_bus( 8) <= spi_mosi;
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
         end if; --// if sck_tick = '1'....
      end if;    --// if spi_clk_i'event and spi_clk_i = '1'
   end process;


end Behavioral;
