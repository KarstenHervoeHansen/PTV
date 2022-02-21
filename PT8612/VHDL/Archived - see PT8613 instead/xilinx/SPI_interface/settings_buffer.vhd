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
end settings_buffer;

architecture Behavioral of settings_buffer is

--// The table below contains settings RAM addresses as defined in
--// PT5300 Master Software and in F320 microprocessor software
--// onboard the HD Generator.
--//
--//       Symbol          Value                Setting
--//   ----------------------------------------------------------------
--#define   SYS_ADDR           0         /* System                    */
--#define   PAT_ADDR           1         /* Pattern, Attribs          */
--#define   DEL_ADDR           3         /* Delay   MSB...LSB         */
--#define   CLI_ADDR           7         /* Audio Click               */
--#define   SIG_ADDR           9         /* Audio Signal              */
--#define   LEV_ADDR          10         /* Audio Level               */
--#define   GRO_ADDR          11         /* Audio Group               */
--#define   ASR_ADDR          12         /* Audio Sample Rate         */
--#define   POSX_ADDR         13         /* Text Position  Xcoord     */
--#define   POSY_ADDR         14         /* Text Position  Ycoord     */
--#define   SCA_ADDR          15         /* Text Scale                */
--#define   COL_ADDR          16         /* Text Color                */
--#define   SIZ_ADDR          17         /* Text Bitmap Size MSB..LSB */
--#define   MOV_ADDR          19         /* Text Movement             */
--#define   XMIN_ADDR         20         /* Text Movement Bound X MIN */
--#define   XMAX_ADDR         21         /* Text Movement Bound X MAX */
--#define   YMIN_ADDR         21         /* Text Movement Bound Y MIN */
--#define   YMAX_ADDR         23         /* Text Movement Bound Y MAX */
--#define   TMS_ADDR          24         /* Text Movement Speed       */
--#define   TXT_ADDR          30         /* Text Bitmap               */

constant SYS  : integer :=   0;
constant PAT  : integer :=   1;
constant ATT  : integer :=   2;
constant DEL  : integer :=   3;
constant DEL1 : integer :=   4;
constant DEL2 : integer :=   5;

constant CLI  : integer :=   7;
constant CLI1 : integer :=   8;
constant SIG  : integer :=   9;
constant LEV  : integer :=  10;
constant GRO  : integer :=  11;
constant ASR  : integer :=  12;
constant POSX : integer :=  13;
constant POSN : integer :=  14;
constant SCA  : integer :=  15;
constant COL  : integer :=  16;
constant SIZ  : integer :=  17;
constant SIZ1 : integer :=  18;
constant MOV  : integer :=  19;
constant XMIN : integer :=  20;
constant XMAX : integer :=  21;
constant YMIN : integer :=  22;
constant YMAX : integer :=  23;
constant TMS  : integer :=  24;
                                       --//
constant TXT  : integer :=  30;

constant SYS_ADDR    : std_logic_vector(8 downto 0) := conv_std_logic_vector(SYS,  9);  --// System                    */
constant PAT_ADDR    : std_logic_vector(8 downto 0) := conv_std_logic_vector(PAT,  9);
constant ATT_ADDR    : std_logic_vector(8 downto 0) := conv_std_logic_vector(ATT,  9);
constant DEL_ADDR    : std_logic_vector(8 downto 0) := conv_std_logic_vector(DEL,  9);
constant DEL_ADDR_1  : std_logic_vector(8 downto 0) := conv_std_logic_vector(DEL1, 9);
constant DEL_ADDR_2  : std_logic_vector(8 downto 0) := conv_std_logic_vector(DEL2, 9);
--constant DEL_ADDR_3  : std_logic_vector(8 downto 0) := conv_std_logic_vector(DEL3, 9);
constant CLI_ADDR    : std_logic_vector(8 downto 0) := conv_std_logic_vector(CLI,  9);
constant CLI_ADDR_1  : std_logic_vector(8 downto 0) := conv_std_logic_vector(CLI1, 9);
constant SIG_ADDR    : std_logic_vector(8 downto 0) := conv_std_logic_vector(SIG,  9); 
constant LEV_ADDR    : std_logic_vector(8 downto 0) := conv_std_logic_vector(LEV,  9);
constant GRO_ADDR    : std_logic_vector(8 downto 0) := conv_std_logic_vector(GRO,  9);
constant ASR_ADDR    : std_logic_vector(8 downto 0) := conv_std_logic_vector(ASR,  9);
constant POS_ADDR    : std_logic_vector(8 downto 0) := conv_std_logic_vector(POSX, 9);
constant POS_ADDR_1  : std_logic_vector(8 downto 0) := conv_std_logic_vector(POSN, 9);
constant SCA_ADDR    : std_logic_vector(8 downto 0) := conv_std_logic_vector(SCA,  9);
constant COL_ADDR    : std_logic_vector(8 downto 0) := conv_std_logic_vector(COL,  9);
constant SIZ_ADDR    : std_logic_vector(8 downto 0) := conv_std_logic_vector(SIZ,  9);
constant SIZ_ADDR_1  : std_logic_vector(8 downto 0) := conv_std_logic_vector(SIZ1, 9);
constant MOV_ADDR    : std_logic_vector(8 downto 0) := conv_std_logic_vector(MOV,  9);
constant XMIN_ADDR   : std_logic_vector(8 downto 0) := conv_std_logic_vector(XMIN, 9);
constant XMAX_ADDR   : std_logic_vector(8 downto 0) := conv_std_logic_vector(XMAX, 9);
constant YMIN_ADDR   : std_logic_vector(8 downto 0) := conv_std_logic_vector(YMIN, 9);
constant YMAX_ADDR   : std_logic_vector(8 downto 0) := conv_std_logic_vector(YMAX, 9);
constant TMS_ADDR    : std_logic_vector(8 downto 0) := conv_std_logic_vector(TMS,  9); 

constant TXT_ADDR    : std_logic_vector(8 downto 0) := conv_std_logic_vector(TXT,  9); 



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

               when ATT_ADDR             =>
                  pat_o(15 downto  8)    <= temp_buff(2);
                  pat_o( 7 downto  0)    <= rd_dat_i;

               when DEL_ADDR             =>
                  temp_buff(1)           <= rd_dat_i;
               when DEL_ADDR_1           =>
                  temp_buff(0)           <= rd_dat_i;
               when DEL_ADDR_2           =>
                  del_o(23 downto 16)    <= temp_buff(1);
                  del_o(15 downto  8)    <= temp_buff(0);
                  del_o( 7 downto  0)    <= rd_dat_i;

               when CLI_ADDR             =>
                  temp_buff(0)           <= rd_dat_i;
                  
               when CLI_ADDR_1           =>
                  aclick_o(15 downto  8) <= temp_buff(0);
                  aclick_o( 7 downto  0) <= rd_dat_i;
               
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
               when POS_ADDR_1           =>
                  pos_o(15 downto  8)    <= temp_buff(2);
                  pos_o( 7 downto  0)    <= rd_dat_i;

               when SCA_ADDR             =>
                  sca_o                  <= rd_dat_i;

               when COL_ADDR             =>
                  col_o                  <= rd_dat_i;

               when SIZ_ADDR             =>
                  temp_buff(0)           <= rd_dat_i;

               when SIZ_ADDR_1           =>
                  siz_o(15 downto  8)    <= temp_buff(0);
                  siz_o( 7 downto  0)    <= rd_dat_i;

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
