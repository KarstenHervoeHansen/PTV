--///*******************************************************************
--// File:     serial_interface.vhd
--// Project:  PT8612 HD-SDI Test Pattern Generator
--// Date:     07.02.2006
--// Author:    JK
--// Purpose:  SPI interface handling communication between
--//           microcontroller and the rest of FPGA
--// Notes:    This interface implements transfer of generator
--//           settings as described in uc_fpga06.doc. 
--//           Entity serial_interface incorporates 4 dual port memories
--//           where settings received from microcontroller are stored at
--//           predetermined addresses.  The rest of FPGA hardware can access
--//           these settings via 4 output busses.
--//           The output bus consists of clock input, address bus, 
--//           data bus and and active high chx_param_ram_ok_o output signal.
--//           The chx_param_ram_ok_o signal goes inactive (i.e. prohibits
--//           RAM read operations when data is written to RAM by the 
--//           SPI interface.  This happens when spi_cs_i is low from
--//           the moment generator number has been received (bitptr = 9)
--//           to the moment when when spi_cs_i goes hi, indicating end
--//           of SPI data transfer to RAM.  Data for settings of other
--//           channels than that which the SPI is currently transferring
--//           data for, can be read at all times.
--//*********************************************************************
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity serial_interface is
   Port
      (
      spi_mreset_i         : in  std_logic;   --// master reset
      spi_clk_i            : in  std_logic;   --// clock
      spi_cs_i             : in  std_logic;   --// SPI chip select
      spi_sck_i            : in  std_logic;   --// SPI serial clock
      spi_mosi_i           : in  std_logic;   --// SPI master out serial data in
      
      --// Common channel ram address bus for reading by the rest of FPGA:
      --// four data and address buses for reading and four 'data ready' signals
      --// and four clocks

      ch1_param_ram_clk_i  : in  std_logic;  --// clock for reading ram1
      ch1_param_ram_addr_i : in  std_logic_vector( 4 downto 0);
      ch1_param_ram_data_o : out std_logic_vector( 7 downto 0);
      ch1_param_ram_ok_o   : out std_logic;
      ch1_text_ram_clk_i   : in  std_logic;  --// clock for reading text ram
      ch1_text_ram_addr_i  : in  std_logic_vector( 8 downto 0);
      ch1_text_ram_en_i    : in  std_logic;  --// enable signal for reading text ram
      ch1_text_ram_data_o  : out std_logic_vector( 7 downto 0);
      
      ch2_param_ram_clk_i  : in  std_logic;  --// clock for reading ram1
      ch2_param_ram_addr_i : in  std_logic_vector( 4 downto 0);
      ch2_param_ram_data_o : out std_logic_vector( 7 downto 0);
      ch2_param_ram_ok_o   : out std_logic;
      ch2_text_ram_clk_i   : in  std_logic;  --// clock for reading text ram
      ch2_text_ram_addr_i  : in  std_logic_vector( 8 downto 0);
      ch2_text_ram_en_i    : in  std_logic;  --// enable signal for reading text ram
      ch2_text_ram_data_o  : out std_logic_vector( 7 downto 0);      

      ch3_param_ram_clk_i  : in  std_logic;  --// clock for reading ram1
      ch3_param_ram_addr_i : in  std_logic_vector( 4 downto 0);
      ch3_param_ram_data_o : out std_logic_vector( 7 downto 0);
      ch3_param_ram_ok_o   : out std_logic;
      ch3_text_ram_clk_i   : in  std_logic;  --// clock for reading text ram
      ch3_text_ram_addr_i  : in  std_logic_vector( 8 downto 0);
      ch3_text_ram_en_i    : in  std_logic;  --// enable signal for reading text ram
      ch3_text_ram_data_o  : out std_logic_vector( 7 downto 0);      

      ch4_param_ram_clk_i  : in  std_logic;  --// clock for reading ram1
      ch4_param_ram_addr_i : in  std_logic_vector( 4 downto 0);
      ch4_param_ram_data_o : out std_logic_vector( 7 downto 0);
      ch4_param_ram_ok_o   : out std_logic;
      ch4_text_ram_clk_i   : in  std_logic;  --// clock for reading text ram
      ch4_text_ram_addr_i  : in  std_logic_vector( 8 downto 0);
      ch4_text_ram_en_i    : in  std_logic;  --// enable signal for reading text ram
      ch4_text_ram_data_o  : out std_logic_vector( 7 downto 0);      
            
      --// test (debug) signals
      
      ram_we               : out std_logic;
      tick                 : out std_logic
      );
end serial_interface;

architecture Behavioral of serial_interface is

signal spi_cs                 : std_logic;
signal spi_cs_delayed         : std_logic;
signal spi_sck                : std_logic;
signal spi_mosi               : std_logic;
signal sck_tick               : std_logic;

signal sck_delayed            : std_logic;
signal bitptr                 : integer range 0 to 63;

signal channel_sel            : std_logic_vector(3 downto 0);  --// select channel RAM
signal tmp_selector           : std_logic_vector(3 downto 0);
signal permit_Bram1_read      : std_logic;
signal permit_Bram2_read      : std_logic;
signal permit_Bram3_read      : std_logic;
signal permit_Bram4_read      : std_logic;
signal permit_ram_write       : std_logic;                     --// enable writing to channel RAMs
signal command_addr           : std_logic_vector(15 downto 0);
signal param_ram_Aaddr_bus    : std_logic_vector(4 downto 0);
signal text_ram_Aaddr_bus     : std_logic_vector(8 downto 0);

signal channel_ram_Adata_bus  : std_logic_vector(7 downto 0);
signal ram_Adata_bus          : std_logic_vector(7 downto 0);

signal param_ram_write        : std_logic;
signal text_ram_write         : std_logic;

constant last_param_address   : std_logic_vector(4 downto 0) := "11111";

component param_ram
   port
      (
      --// input (SPI) part (write only)
      clka:  IN  std_logic;
      addra: IN  std_logic_VECTOR( 4 downto 0);
      dina:  IN  std_logic_VECTOR( 7 downto 0);
      wea:   IN  std_logic;                      --// write enable for part A (active hi)
      ena:   IN  std_logic;
      
      --// output (FPGA) (read only)
      clkb:  IN  std_logic;
      addrb: IN  std_logic_VECTOR( 4 downto 0);
      doutb: OUT std_logic_VECTOR( 7 downto 0)
      );
end component;


component text_ram
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


chan1_param_ram: param_ram
   port map
      (
      clka   => spi_clk_i,
      addra  => param_ram_Aaddr_bus,
      dina   => ram_Adata_bus,
      ena    => channel_sel(0),
      wea    => param_ram_write,
      
      clkb   => ch1_param_ram_clk_i,
      addrb  => ch1_param_ram_addr_i,
      doutb  => ch1_param_ram_data_o
      );

chan1_text_ram: text_ram
   port map
      (
      clka   => spi_clk_i,
      addra  => text_ram_Aaddr_bus,
      dina   => ram_Adata_bus,
      ena    => channel_sel(0),
      wea    => text_ram_write,
      
      clkb   => ch1_text_ram_clk_i,
      addrb  => ch1_text_ram_addr_i,
      doutb  => ch1_text_ram_data_o,
      enb    => ch1_text_ram_en_i
      );
      
            
chan2_param_ram: param_ram
   port map
      (
      clka   => spi_clk_i,          
      addra  => param_ram_Aaddr_bus,
      dina   => ram_Adata_bus,      
      ena    => channel_sel(1),     
      wea    => param_ram_write,    
      
      clkb   => ch2_param_ram_clk_i,
      addrb  => ch2_param_ram_addr_i,
      doutb  => ch2_param_ram_data_o
      );

chan2_text_ram: text_ram
   port map
      (
      clka   => spi_clk_i,
      addra  => text_ram_Aaddr_bus,
      dina   => ram_Adata_bus,
      ena    => channel_sel(1),
      wea    => text_ram_write,
      
      clkb   => ch2_text_ram_clk_i,
      addrb  => ch2_text_ram_addr_i,
      doutb  => ch2_text_ram_data_o,
      enb    => ch2_text_ram_en_i
      );
      
            
chan3_param_ram: param_ram
   port map
      (
      clka   => spi_clk_i,          
      addra  => param_ram_Aaddr_bus,
      dina   => ram_Adata_bus,      
      ena    => channel_sel(2),     
      wea    => param_ram_write,    
      
      clkb   => ch3_param_ram_clk_i,
      addrb  => ch3_param_ram_addr_i,
      doutb  => ch3_param_ram_data_o
      );
      
chan3_text_ram: text_ram
   port map
      (
      clka   => spi_clk_i,
      addra  => text_ram_Aaddr_bus,
      dina   => ram_Adata_bus,
      ena    => channel_sel(2),
      wea    => text_ram_write,
      
      clkb   => ch3_text_ram_clk_i,
      addrb  => ch3_text_ram_addr_i,
      doutb  => ch3_text_ram_data_o,
      enb    => ch3_text_ram_en_i
      );
      
chan4_param_ram: param_ram
   port map
      (
      clka   => spi_clk_i,          
      addra  => param_ram_Aaddr_bus,
      dina   => ram_Adata_bus,      
      ena    => channel_sel(3),     
      wea    => param_ram_write,    
      
      clkb   => ch4_param_ram_clk_i,
      addrb  => ch4_param_ram_addr_i,
      doutb  => ch4_param_ram_data_o
      );
      
chan4_text_ram: text_ram
   port map
      (
      clka   => spi_clk_i,
      addra  => text_ram_Aaddr_bus,
      dina   => ram_Adata_bus,
      ena    => channel_sel(3),
      wea    => text_ram_write,
      
      clkb   => ch4_text_ram_clk_i,
      addrb  => ch4_text_ram_addr_i,
      doutb  => ch4_text_ram_data_o,
      enb    => ch4_text_ram_en_i
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
         spi_cs_delayed     <= '1';
      elsif spi_clk_i'event and spi_clk_i ='1' then
         spi_cs             <= spi_cs_i;
         spi_cs_delayed     <= spi_cs;
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
--// If pointer is 32 (8 bits of data received), it is wrapped to
--// 25 (to receive next 8 bits of data).
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
         if spi_cs_delayed = '1' then
            permit_Bram1_read <= '1';  --// reading ch1 Bram possible
            permit_Bram2_read <= '1';  --// reading ch2 Bram possible
            permit_Bram3_read <= '1';  --// reading ch3 Bram possible
            permit_Bram4_read <= '1';  --// reading ch4 Bram possible
         elsif bitptr = 10 then
            permit_Bram1_read <= not channel_sel(0);
            permit_Bram2_read <= not channel_sel(1);
            permit_Bram3_read <= not channel_sel(2);
            permit_Bram4_read <= not channel_sel(3);
         end if;
      end if;
   end process;

ch1_param_ram_ok_o <= permit_Bram1_read;
ch2_param_ram_ok_o <= permit_Bram2_read;
ch3_param_ram_ok_o <= permit_Bram3_read;
ch4_param_ram_ok_o <= permit_Bram4_read;

--// Test (debug) signals
ram_we        <= permit_ram_write;
tick          <= sck_tick;

--//--------------------------------------------------------------
--// Clock in serial data:
--// Receives: Channel number to channel_sel
--//           RAM address to    command_addr
--//           Data bytes to     channel_ram_Adata_bus
--// Data is received as long as spi_cs is low.
--// channel_ram_Adata_bus is written into RAM when 8 data bits 
--// are received (bitptr = 32).
--// command_addr is incremented after RAM write.
--//--------------------------------------------------------------
get_serial_data : process(spi_mreset_i, spi_clk_i)
   begin
      if spi_mreset_i = '0' then
         permit_ram_write <= '0';
      elsif spi_clk_i'event and spi_clk_i = '1' then
         --// increment address bus after write
         if permit_ram_write = '1' then
            if command_addr /= X"FFFF" then
               command_addr <= command_addr + 1;
            end if;
         end if;
         
         if spi_cs = '1' then
            permit_ram_write <= '0';
         else 
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
         end if;
   
         if sck_tick = '1' then
            case bitptr is
               --// get generator number
               when 5  => channel_sel(3)   <= spi_mosi;
               when 6  => channel_sel(2)   <= spi_mosi;
               when 7  => channel_sel(1)   <= spi_mosi;
               when 8  => channel_sel(0)   <= spi_mosi;
               --// get command address
               when 9  => command_addr(15) <= spi_mosi;
               when 10 => command_addr(14) <= spi_mosi;
               when 11 => command_addr(13) <= spi_mosi;
               when 12 => command_addr(12) <= spi_mosi;
               when 13 => command_addr(11) <= spi_mosi;
               when 14 => command_addr(10) <= spi_mosi;
               when 15 => command_addr(9)  <= spi_mosi;
               when 16 => command_addr(8)  <= spi_mosi;
               when 17 => command_addr(7)  <= spi_mosi;
               when 18 => command_addr(6)  <= spi_mosi;
               when 19 => command_addr(5)  <= spi_mosi;
               when 20 => command_addr(4)  <= spi_mosi;
               when 21 => command_addr(3)  <= spi_mosi;
               when 22 => command_addr(2)  <= spi_mosi;
               when 23 => command_addr(1)  <= spi_mosi;
               when 24 => command_addr(0)  <= spi_mosi;
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


--------------------------------------------------------
-- Map RAM Address to parameter / text RAM:
--------------------------------------------------------   
ram_address_mapping: process(spi_clk_i)
   begin
      if spi_clk_i'event and spi_clk_i = '1' then
         param_ram_Aaddr_bus     <= command_addr(4 downto 0);
         text_ram_Aaddr_bus      <= command_addr(8 downto 0);
         ram_Adata_bus           <= channel_ram_Adata_bus;
         case command_addr(15 downto 12) is
         when "0000"  =>          -- parameter RAM:
            if command_addr(11 downto 5) = "0000000" then      
               param_ram_write   <= permit_ram_write;
            else
               param_ram_write   <= '0';
            end if;
            text_ram_write    <= '0';
         when "0001"   =>          -- text RAM:
            if command_addr(11 downto 9) = "000" then      
               text_ram_write   <= permit_ram_write;
            else
               text_ram_write   <= '0';
            end if;
            param_ram_write   <= '0';             
         when others =>
            param_ram_write      <= '0';             
            text_ram_write       <= '0';
         end case;
      end if;
   end process;


end Behavioral;