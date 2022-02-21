--//---------------------------------------------------------------------------
--// File:     settings_buffer.vhd
--// Project:  PT8612 HD-SDI Test Pattern Generator
--// Date:     16.03.2006
--// Author:    JK
--// Purpose:  To buffer settings between SPI interface and HD generator.
--//           The purpose is to read sequentially all settings from RAM interface
--//           and place on outputs, so they can be used by channel hardware.  
--//           The reading proceeds as long as the serial interface signals
--//           that reading settings RAM is possible (ready_i input is asserted).
--//           In case of settings consisting of more than one byte, bytes are
--//           first read into some temporary variables and only if there was no
--//           interruption on the ready_i signal will they be transferred into
--//           outputs, after all bytes of given settings are successfuly read. 
--//           If there was an interruption (ready_i goes inactive while 
--//           reading RAM), the member value will not be transferred
--//           to output, which will then contain previous values.
--//           Modified 5.04.2006: Audio Ckick Timing instead of EDH (2 bytes)
--//---------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;


entity settings_buffer is
   Port (
   sb_mreset_i             : in  std_logic;   --// master reset
   --// interface to Channel RAM via Serial Interface:
   rd_add_o                : out std_logic_vector( 4 downto 0); --// address bus
   rd_dat_i                : in  std_logic_vector( 7 downto 0); --// data to read bus
   ready_i                 : in  std_logic;                     --// data ready signal
   --// output section:
   sb_clk_i                : in  std_logic;
   sys_o                   : out std_logic_vector( 7 downto 0);
   pat_o                   : out std_logic_vector(15 downto 0);
   del_o                   : out std_logic_vector(23 downto 0);
   aclick_o                : out std_logic_vector(15 downto 0);
   asignal_o               : out std_logic_vector( 7 downto 0);
   alevel_o                : out std_logic_vector( 7 downto 0);
   agroup_o                : out std_logic_vector( 7 downto 0);
   asr_o                   : out std_logic_vector( 7 downto 0);

   bmp_v_position_o        : out std_logic_vector(15 downto 0);
   bmp_h_position_o        : out std_logic_vector(15 downto 0);
   bmp_h_size_o            : out std_logic_vector(7 downto 0);
   bmp_v_size_o            : out std_logic_vector(7 downto 0);
   bmp_scale_o             : out std_logic_vector(7 downto 0);
   bmp_back_text_color_o   : out std_logic_vector(7 downto 0);
   bmp_movement_o          : out std_logic_vector(7 downto 0);
   bmp_on_zoff_o           : out std_logic
   );
end settings_buffer;

architecture Behavioral of settings_buffer is


signal acounter         : std_logic_vector( 4 downto 0);
signal acounter_delayed : std_logic_vector( 4 downto 0);

--type tmp_settings is array(2 downto 0) of std_logic_vector(7 downto 0);
signal temp_buff     : std_logic_vector(31 downto 0);   --: tmp_settings;

begin

rd_add_o <= acounter;

--//--------------------------------------------------------------
--// Update address counter:
--// If ready_i goes inactive,  reset the counter.
--// If counter points to text, reset the counter.
--//--------------------------------------------------------------
update_RAM_address_counter : process(sb_mreset_i, sb_clk_i)
   begin
      if sb_mreset_i = '0' then
         acounter <= (others => '0');
      elsif sb_clk_i'event and sb_clk_i = '1' then
         if ready_i = '0' then
            acounter <= (others => '0');
         else
            acounter <= acounter + 1;
         end if;
      end if;
   end process;

--//--------------------------------------------------------------
--// Daleayed read address:
--// The purpose is to delay changes on read address by one clk_i,
--// by transferring acounter to another variable and using that
--// another variable when reading data.  This is necessary as 
--// incrementing the acounter and reading happens on the same
--// clock, so when reading, the counter is not yet incremented
--// (as it should be).
--//--------------------------------------------------------------
address_delaying: process(sb_mreset_i, sb_clk_i)
   begin
      if sb_mreset_i = '0' then
         acounter_delayed <= (others => '0');
      elsif sb_clk_i'event and sb_clk_i = '1' then
         acounter_delayed <= acounter;
      end if;
   end process;

--//--------------------------------------------------------------
--// Sequentially read RAM and output data to appropriate vector:
--//--------------------------------------------------------------
temp_buff(7 downto 0) <= rd_dat_i;

read_RAM_and_update: process(sb_mreset_i, sb_clk_i)
   begin
      if sb_mreset_i = '0' then
         asignal_o <= (others => '1');
         
      elsif sb_clk_i'event and sb_clk_i = '1' then
         temp_buff(31 downto 8) <= temp_buff(23 downto 0);
         if ready_i = '1' then
            case acounter_delayed is
            when "00000"     =>  sys_o                   <= temp_buff(7 downto 0);
            when "00010"     =>  pat_o                   <= temp_buff(15 downto 0);
            when "00101"     =>  del_o                   <= temp_buff(23 downto 0);
            when "01000"     =>  aclick_o                <= temp_buff(15 downto 0);
            when "01001"     =>  asignal_o               <= temp_buff(7 downto 0);
            when "01010"     =>  alevel_o                <= temp_buff(7 downto 0);
            when "01100"     =>  bmp_h_position_o        <= temp_buff(15 downto 0);
            when "01110"     =>  bmp_v_position_o        <= temp_buff(15 downto 0);
            when "01111"     =>  bmp_scale_o             <= temp_buff(7 downto 0);
            when "10000"     =>  bmp_back_text_color_o   <= temp_buff(7 downto 0);
            when "10001"     =>  bmp_h_size_o            <= temp_buff(7 downto 0);
            when "10010"     =>  bmp_v_size_o            <= temp_buff(7 downto 0);
            when "10011"     =>  bmp_movement_o          <= temp_buff(7 downto 0);
            when "10100"     =>  bmp_on_zoff_o           <= temp_buff(0);
            when others      =>  null;
            end case;
         end if;
      end if;
   end process;

   
-- 0  00000000 system               1080/25i
-- 1  00000001 pattern              colorbar
-- 2  00000001 attribute            ebu
-- 3  00000000 timing               0                   
-- 4  01011010       
-- 5  10100011                                          
-- 6  00001100                                          
-- 7  00001000 audio_click_timing   0                   
-- 8  00000000                                          
-- 9  00000001 audio_signal         sine                
-- 10 00000011 audio_level          -18dB               
-- 11 00000000 bmp_h_pos            sample 1                                                     
-- 12 00000001 
-- 13 00000000 bmp_v_pos            line 1                                                       
-- 14 00000001 
-- 15 00000000 text_size            2 x 2 pixel / pixel  
-- 16 00000111 text_back_color      white text on black 
-- 17 00110001 bmp_h_size           49                  
-- 18 00100001 bmp_v_size           33                  
-- 19 00000000 movement             off                 
-- 20 00000001 text_on              on                           
  
   
end Behavioral;