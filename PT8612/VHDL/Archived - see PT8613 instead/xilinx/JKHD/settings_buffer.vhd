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
--//---------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;


entity settings_buffer is
   Port
   (
   sb_mreset_i   : in  std_logic;   --// master reset
   --// interface to Channel RAM via Serial Interface:
   rd_add_o      : out std_logic_vector( 8 downto 0); --// address buss
   rd_dat_i      : in  std_logic_vector( 7 downto 0); --// data to read bus
   ready_i       : in  std_logic;                     --// data ready signal
   --// output section:
   sb_clk_i      : in  std_logic;
   sys_o         : out std_logic_vector( 7 downto 0);
   pat_o         : out std_logic_vector(15 downto 0);
   del_o         : out std_logic_vector(23 downto 0);
   edh_0         : out std_logic_vector( 7 downto 0);
   asignal_o     : out std_logic_vector( 7 downto 0);
   alevel_o      : out std_logic_vector( 7 downto 0);
   agroup_o      : out std_logic_vector( 7 downto 0);
   asr_o         : out std_logic_vector( 7 downto 0);
   pos_o         : out std_logic_vector(15 downto 0);
   sca_o         : out std_logic_vector( 7 downto 0);
   col_o         : out std_logic_vector( 7 downto 0);
   siz_o         : out std_logic_vector( 7 downto 0);
   mov_o         : out std_logic_vector( 7 downto 0);
   xmin_o        : out std_logic_vector( 7 downto 0);
   xmax_o        : out std_logic_vector( 7 downto 0);
   ymin_o        : out std_logic_vector( 7 downto 0);
   ymax_o        : out std_logic_vector( 7 downto 0);
   tms_o         : out std_logic_vector( 7 downto 0)
   );
end settings_buffer;

architecture Behavioral of settings_buffer is

--// The table below contains settings RAM addresses as defined in
--// PT5300 Master Software and in F320 microprocessor software
--// onboard the HD Generator.
--//
--//       Symbol          Value                Setting
--//   ----------------------------------------------------------------
--//      SYS_ADDR           0         /* System                    */
--//      PAT_ADDR           1         /* Pattern                   */
--//      PAT_ADDR + 1       2         /* Attribs                   */
--//      DEL_ADDR           3         /* Delay   MSB               */
--//      DEL_ADDR + 1       4         /* Delay   ...               */
--//      DEL_ADDR + 2       5         /* Delay   ...               */
--//      DEL_ADDR + 3       6         /* Delay   LSB               */
--//      EDH_ADDR           7         /* EDH                       */
--//      SIG_ADDR           8         /* Audio Signal              */
--//      LEV_ADDR           9         /* Audio Level               */
--//      GRO_ADDR          10         /* Audio Group               */
--//      ASR_ADDR          11         /* Audio Sample Rate         */
--//      POSX_ADDR         12         /* Text Position  Xcoord     */
--//      POSY_ADDR         13         /* Text Position  Ycoord     */
--//      SCA_ADDR          14         /* Text Scale                */
--//      COL_ADDR          15         /* Text Color                */
--//      SIZ_ADDR          16         /* Text Bitmap Size  MSB     */
--//      SIZ_ADDR + 1      17         /* Text Bitmap Size  LSB     */
--//      MOV_ADDR          18         /* Text Movement             */
--//      XMIN_ADDR         19         /* Text Movement Bound X MIN */
--//      XMAX_ADDR         20         /* Text Movement Bound X MAX */
--//      YMIN_ADDR         21         /* Text Movement Bound Y MIN */
--//      YMAX_ADDR         22         /* Text Movement Bound Y MAX */
--//      TMS_ADDR          23         /* Text Movement Speed       */
--//      TXT_ADDR          24         /* Text Bitmap               */

constant SYS_ADDR       : std_logic_vector(8 downto 0) := "000000000";  --// System                    */
constant PAT_ADDR       : std_logic_vector(8 downto 0) := "000000001";  --// Pattern                   */
constant ATT_ADDR       : std_logic_vector(8 downto 0) := "000000010";  --// Attribs                   */
constant DEL_ADDR       : std_logic_vector(8 downto 0) := "000000011";  --// Delay                     */
constant EDH_ADDR       : std_logic_vector(8 downto 0) := "000000111";  --// EDH                       */
constant SIG_ADDR       : std_logic_vector(8 downto 0) := "000001000";  --// Audio Signal              */
constant LEV_ADDR       : std_logic_vector(8 downto 0) := "000001001";  --// Audio Level               */
constant GRO_ADDR       : std_logic_vector(8 downto 0) := "000001010";  --// Audio Group               */
constant ASR_ADDR       : std_logic_vector(8 downto 0) := "000001011";  --// Audio Sample Rate         */
constant POS_ADDR       : std_logic_vector(8 downto 0) := "000001100";  --// Text Position             */
constant SCA_ADDR       : std_logic_vector(8 downto 0) := "000001110";  --// Text Scale                */
constant COL_ADDR       : std_logic_vector(8 downto 0) := "000001111";  --// Text Color                */
constant SIZ_ADDR       : std_logic_vector(8 downto 0) := "000010000";  --// Text Bitmap Size          */
constant MOV_ADDR       : std_logic_vector(8 downto 0) := "000010010";  --// Text Movement             */
constant XMIN_ADDR      : std_logic_vector(8 downto 0) := "000010011";  --// Text Movement Bound X MIN */
constant XMAX_ADDR      : std_logic_vector(8 downto 0) := "000010100";  --// Text Movement Bound X MAX */
constant YMIN_ADDR      : std_logic_vector(8 downto 0) := "000010101";  --// Text Movement Bound Y MIN */
constant YMAX_ADDR      : std_logic_vector(8 downto 0) := "000010110";  --// Text Movement Bound Y MAX */
constant TMS_ADDR       : std_logic_vector(8 downto 0) := "000010111";  --// Text Movement Speed       */
constant TXT_ADDR       : std_logic_vector(8 downto 0) := "000011000";  --// Text Bitmap               */


signal acounter         : std_logic_vector( 8 downto 0);
signal acounter_delayed : std_logic_vector( 8 downto 0);

type tmp_settings is array(2 downto 0) of std_logic_vector(7 downto 0);
signal temp_buff        : tmp_settings;

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
         acounter <= SYS_ADDR;
      elsif sb_clk_i'event and sb_clk_i = '1' then
         if ready_i = '0' then
            acounter <=  SYS_ADDR;
         elsif acounter = TXT_ADDR then --// wrap arround if Text address 
            acounter <=  SYS_ADDR;
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
read_RAM_and_update: process(sb_mreset_i, sb_clk_i)
   begin
      if sb_mreset_i = '0' then
         asignal_o <= (others => '1');
      elsif sb_clk_i'event and sb_clk_i = '1' then
         if ready_i = '1' then
            case acounter_delayed is
               when SYS_ADDR             =>
                  sys_o                  <= rd_dat_i;

               when PAT_ADDR             =>
                  temp_buff(2)           <= rd_dat_i;

               when "000000010"          =>
                  pat_o(15 downto  8)    <= temp_buff(2);
                  pat_o( 7 downto  0)    <= rd_dat_i;

               when DEL_ADDR             =>
                  temp_buff(2)           <= rd_dat_i;
               when "000000100"          =>
                  temp_buff(1)           <= rd_dat_i;
               when "000000101"          =>
                  temp_buff(0)           <= rd_dat_i;
               when "000000110"          =>
                  --//del_o(31 downto 24)    <= temp_buff(2);
                  del_o(23 downto 16)    <= temp_buff(1);
                  del_o(15 downto  8)    <= temp_buff(0);
                  del_o( 7 downto  0)    <= rd_dat_i;

               when EDH_ADDR             =>
                  edh_0                  <= rd_dat_i;

               when SIG_ADDR             => 
                  asignal_o              <= rd_dat_i;

               when LEV_ADDR             =>
                  alevel_o               <= rd_dat_i;

               when GRO_ADDR             =>
                  agroup_o               <= rd_dat_i;

               when ASR_ADDR             =>
                  asr_o                  <= rd_dat_i;

               when POS_ADDR             =>
                  temp_buff(2)           <= rd_dat_i;
               when "000001101"          =>
                  pos_o(15 downto  8)    <= temp_buff(2);
                  pos_o( 7 downto  0)    <= rd_dat_i;

               when SCA_ADDR             =>
                  sca_o                  <= rd_dat_i;

               when COL_ADDR             =>
                  col_o                  <= rd_dat_i;

               when SIZ_ADDR             =>
                  siz_o                  <= rd_dat_i;

               when MOV_ADDR             =>
                  mov_o                  <= rd_dat_i;

               when XMIN_ADDR            =>
                  xmin_o                 <= rd_dat_i;

               when XMAX_ADDR            =>
                  xmax_o                 <= rd_dat_i;

               when YMIN_ADDR            =>
                  ymin_o                 <= rd_dat_i;

               when YMAX_ADDR            =>
                  ymax_o                 <= rd_dat_i;

               when TMS_ADDR             =>
                  tms_o                  <= rd_dat_i;
               when others   =>
                  null;
            end case;
         end if;
      end if;
   end process;

end Behavioral;
