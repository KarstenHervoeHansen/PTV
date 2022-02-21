--******************************************************************************
--
--  Xilinx, Inc. 2002                 www.xilinx.com
--
--  XAPP 253 - Synthesizable DDR SDRAM Controller
--
--*******************************************************************************
--
--  File name :       controller.vhd
--
--  Description :     
--                    Main DDR SDRAM controller block. This includes the following
--                    features:
--                    - The controller state machine that controls the 
--                    initialization process upon power up, as well as the 
--                    read, write, and refresh commands. 
--                    - Accepts and decodes the user commands.
--                    - Generates the address and Bank address signals
--                    - Generates control signals for other modules, including
--                    the control signals for the dqs_en block.
--
--  Date - revision : 05/01/2002
--
--  Author :          Lakshmi Gopalakrishnan(Modified by Sailaja)
--
--  Contact : e-mail  hotline@xilinx.com
--            phone   + 1 800 255 7778 
--
--  Disclaimer: LIMITED WARRANTY AND DISCLAMER. These designs are 
--              provided to you "as is". Xilinx and its licensors make and you 
--              receive no warranties or conditions, express, implied, 
--              statutory or otherwise, and Xilinx specifically disclaims any 
--              implied warranties of merchantability, non-infringement, or 
--              fitness for a particular purpose. Xilinx does not warrant that 
--              the functions contained in these designs will meet your 
--              requirements, or that the operation of these designs will be 
--              uninterrupted or error free, or that defects in the Designs 
--              will be corrected. Furthermore, Xilinx does not warrant or 
--              make any representations regarding use or the results of the 
--              use of the designs in terms of correctness, accuracy, 
--              reliability, or otherwise. 
--
--              LIMITATION OF LIABILITY. In no event will Xilinx or its 
--              licensors be liable for any loss of data, lost profits, cost 
--              or procurement of substitute goods or services, or for any 
--              special, incidental, consequential, or indirect damages 
--              arising from the use or operation of the designs or 
--              accompanying documentation, however caused and on any theory 
--              of liability. This limitation will apply even if Xilinx 
--              has been advised of the possibility of such damage. This 
--              limitation shall apply not-withstanding the failure of the 
--              essential purpose of any limited remedies herein. 
--
--  Copyright © 2002 Xilinx, Inc.
--  All rights reserved 
--  
--*****************************************************************************
-- Code modified to fix refresh bug and combinational loop in accept_cmd_in with next_state, 9.8.2005
-- June 9-2005.
-- Write recovery implementation is corrected
-- Latency is reduced by 1 clock
-- cas_latency1 removed
-- Sailaja
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use work.parameter_16bit_00.all;
--
-- pragma translate_off
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
-- pragma translate_on
--
entity controller_16bit_00 is
port(
    dip1             : in std_logic;
    dip3             : in std_logic;
    clk              : in std_logic;
  clk180           : in std_logic;
    rst0             : in std_logic;
    rst180           : in std_logic;
    address          : in std_logic_vector(((row_address_p + column_address_p)-1) downto 0);
    bank_address     : in std_logic_vector((bank_address_p-1) downto 0);
    config_register1 : in std_logic_vector(14 downto 0);
    config_register2 : in std_logic_vector(12 downto 0);
    command_register : in std_logic_vector(3 downto 0);
    burst_done       : in std_logic;
    ddr_rasb_cntrl   : out std_logic;
    ddr_casb_cntrl   : out std_logic;
    ddr_web_cntrl    : out std_logic;
    ddr_ba_cntrl     : out std_logic_vector((bank_address_p-1) downto 0);
    ddr_address_cntrl: out std_logic_vector((row_address_p-1) downto 0);
    ddr_cke_cntrl    : out std_logic;
    ddr_csb_cntrl    : out std_logic;
    ddr_ODT_cntrl    : out std_logic;
    dqs_enable       : out std_logic;
    dqs_reset        : out std_logic;
    write_enable     : out std_logic;
    rst_calib        : out std_logic;
    rst_dqs_div_int  : out std_logic;
    cmd_ack          : out std_logic;
    init             : out std_logic;
    ar_done          : out std_logic;
    wait_200us       : in std_logic;
    auto_ref_req     : out std_logic
);
end controller_16bit_00;
   

architecture arc_controller_16bit_00 of controller_16bit_00 is

attribute syn_keep : boolean;

component FD
   port(
      Q                              :  out   STD_LOGIC;
      D                              :  in    STD_LOGIC;
      C                              :  in    STD_LOGIC);
end component;

--attribute IOB : string ; 
--attribute IOB of rst_iob_out : label is "FALSE"; 

type s_m is (IDLE, PRECHARGE, LOAD_MODE_REG, AUTO_REFRESH, ACTIVE, 
         FIRST_WRITE, WRITE_WAIT, BURST_WRITE, READ_AFTER_WRITE, PRECHARGE_AFTER_WRITE,
         PRECHARGE_AFTER_WRITE_2,READ_WAIT, BURST_READ, ODT_TURN_ON,  
         ACTIVE_WAIT);
signal next_state,current_state: s_m;

signal ack_reg                : std_logic;
signal ack_o                  : std_logic;
signal address_reg            : std_logic_vector(((row_address_p + column_address_p)-1) downto 0);
signal address_config         : std_logic_vector((row_address_p-1) downto 0);
signal auto_ref               : std_logic;
signal auto_ref1              : std_logic;
signal AUTOREF_value          : std_logic;
signal AUTO_REF_detect        : std_logic;
signal AUTO_REF_detect1        : std_logic;
signal AUTO_REF_pulse_end     : std_logic;
signal AUTOREF_COUNT          : std_logic_vector(11 downto 0);
signal AUTOREF_CNT_val        : std_logic_vector(11 downto 0);
signal AR_done_p              : std_logic;
signal Auto_Ref_issued        : std_logic;
signal Auto_Ref_issued_p      : std_logic;
signal BA_address_active      : std_logic_vector((bank_address_p-1) downto 0);
signal BA_address_conflict    : std_logic;
signal BA_address_reg         : std_logic_vector((bank_address_p-1) downto 0);
signal burst_length           : std_logic_vector(2 downto 0);
signal burst_cnt_max          : std_logic_vector(2 downto 0);
signal CAS_COUNT              : std_logic_vector(1 downto 0);
signal cas_count_value        : std_logic_vector(1 downto 0);
signal cas_latency            : std_logic_vector(2 downto 0);
--signal cas_latency1           : std_logic_vector(2 downto 0);

signal COLUMN_ADDRESS_reg      : std_logic_vector((column_address_p-1) downto 0);
signal COLUMN_ADDRESS_reg1     : std_logic_vector((column_address_p-1) downto 0);
signal COLUMN_ADDRESS_reg2     : std_logic_vector((column_address_p-1) downto 0);
signal COLUMN_ADDRESS_reg3     : std_logic_vector((column_address_p-1) downto 0);
signal COLUMN_ADDRESS_reg4     : std_logic_vector((column_address_p-1) downto 0);
signal COLUMN_ADDRESS_reg5     : std_logic_vector((column_address_p-1) downto 0);
signal COLUMN_ADDRESS_reg6     : std_logic_vector((column_address_p-1) downto 0);
signal COLUMN_ADDRESS          : std_logic_vector((column_address_p-1) downto 0);

signal command_reg            : std_logic_vector(3 downto 0);
signal config_reg1            : std_logic_vector(14 downto 0);
signal config_reg2            : std_logic_vector(12 downto 0);
signal CONFLICT               : std_logic;
signal CONFLICT_value         : std_logic;
signal ddr_rasb1              : std_logic;
signal ddr_casb1              : std_logic;
signal ddr_web1               : std_logic;
signal ddr_rasb2              : std_logic;
signal ddr_casb2              : std_logic;
signal ddr_web2               : std_logic;
signal ddr_rasb3              : std_logic;
signal ddr_casb3              : std_logic;
signal ddr_web3               : std_logic;
signal ddr_rasb4              : std_logic;
signal ddr_casb4              : std_logic;
signal ddr_web4               : std_logic;
signal ddr_rasb5              : std_logic;
signal ddr_casb5              : std_logic;
signal ddr_web5               : std_logic;
signal ddr_rst_dqs_rasb4      : std_logic;
signal ddr_rst_dqs_casb4      : std_logic;
signal ddr_rst_dqs_web4       : std_logic;

signal ddr_ba1                : std_logic_vector((bank_address_p-1) downto 0);
signal ddr_ba2                : std_logic_vector((bank_address_p-1) downto 0);
signal ddr_ba3                : std_logic_vector((bank_address_p-1) downto 0);
signal ddr_ba4                : std_logic_vector((bank_address_p-1) downto 0);

signal ddr_address1           : std_logic_vector((row_address_p-1) downto 0);
signal ddr_address2           : std_logic_vector((row_address_p-1) downto 0);
signal ddr_address3           : std_logic_vector((row_address_p-1) downto 0);
signal ddr_address4           : std_logic_vector((row_address_p-1) downto 0);


signal DQS_enable_out         : std_logic;
signal DQS_reset_out          : std_logic;
signal DLL_RST_COUNT_value    : std_logic_vector(7 downto 0);
signal DLL_RST_COUNT          : std_logic_vector(7 downto 0);
signal INIT_COUNT_value       : std_logic_vector(3 downto 0);
signal INIT_COUNT             : std_logic_vector(3 downto 0);
signal INIT_DONE              : std_logic;
signal INIT_DONE_value        : std_logic;
signal INIT_MEMORY            : std_logic;
signal init_mem               : std_logic;
signal initialize_memory      : std_logic;
signal INIT_PRE_COUNT_en      : std_logic;
signal INIT_PRE_COUNT         : std_logic_vector(6 downto 0);
signal ld_mode                : std_logic;
signal MRD_COUNT_value        : std_logic_vector(1 downto 0);
signal max_ref_cnt            : std_logic_vector(11 downto 0);
signal MRD_COUNT              : std_logic_vector(1 downto 0);
signal PRECHARGE_cmd          : std_logic;
signal ras_count_value        : std_logic_vector(3 downto 0);
signal ras_count              : std_logic_vector(3 downto 0);
signal rdburst_chk            : std_logic;
signal read_cmd               : std_logic;
signal read_cmd1              : std_logic;
signal read_cmd2              : std_logic;
signal read_cmd3              : std_logic;
signal read_cmd4              : std_logic;
signal read_cmd5              : std_logic;
signal read_cmd6              : std_logic;
signal read_cmd7              : std_logic;
signal read_cmd8              : std_logic;
signal read_rcd_end           : std_logic;
signal read_cmd_reg           : std_logic;
signal RRD_COUNT              : std_logic_vector(1 downto 0);
signal RCDR_COUNT             : std_logic_vector(2 downto 0);
signal RCDW_COUNT             : std_logic_vector(2 downto 0);
signal RP_cnt_value           : std_logic_vector(2 downto 0); 
signal RFC_COUNT_value        : std_logic_vector(5 downto 0);
signal RFC_COUNT_reg          : std_logic;
signal AR_Done_reg            : std_logic;
signal RRD_COUNT_value        : std_logic_vector(1 downto 0);
signal RCDR_COUNT_value       : std_logic_vector(2 downto 0);
signal RCDW_COUNT_value       : std_logic_vector(2 downto 0);
signal RC_COUNT_value         : std_logic_vector(3 downto 0);
signal rdburst_end_cnt_value  : std_logic_vector(2 downto 0);
signal rdburst_end_cnt        : std_logic_vector(2 downto 0);
signal rdburst_end_1            : std_logic;
signal rdburst_end_2            : std_logic;
signal rdburst_end_3            : std_logic;
signal rdburst_end_4            : std_logic;
signal rdburst_end_5            : std_logic;
signal rdburst_end_6            : std_logic;
signal rdburst_end_7            : std_logic;
signal rdburst_end_8            : std_logic;
signal rdburst_end_r            : std_logic;
signal read_enable_out_r      : std_logic;
signal rdburst_end            : std_logic;
signal RP_COUNT               : std_logic_vector(2 downto 0);
signal RC_COUNT               : std_logic_vector(3 downto 0);
signal RFC_COUNT              : std_logic_vector(5 downto 0);
signal Read_enable_out        : std_logic;
signal ROW_ADDRESS            : std_logic_vector((row_address_p-1) downto 0);
signal ROW_ADDRESS_reg        : std_logic_vector((row_address_p-1) downto 0);
signal row_address_active_reg : std_logic_vector((row_address_p-1) downto 0);
signal row_address_conflict   : std_logic;
signal rst_dqs_div_r          : std_logic;
signal wrburst_end_cnt_value  : std_logic_vector(2 downto 0);
signal wrburst_end_cnt        : std_logic_vector(2 downto 0);
signal wrburst_end            : std_logic;
signal wrburst_end_1          : std_logic;
signal wrburst_end_2          : std_logic;
signal wrburst_end_3          : std_logic;
signal wrburst_end_4          : std_logic;
signal wrburst_end_5          : std_logic;
signal wrburst_end_6          : std_logic;
signal wrburst_end_7          : std_logic;
signal wrburst_end_8          : std_logic;
signal wrburst_end_9          : std_logic;
signal wrburst_chk            : std_logic;
signal wrburst_chk_value      : std_logic;
signal WR_COUNT               : std_logic_vector(2 downto 0);
signal Write_enable_out       : std_logic;
signal Write_enable_out1      : std_logic;
signal Write_enable_out2      : std_logic;
signal Write_enable_out3      : std_logic;
signal Write_cmd              : std_logic;
signal write_cmd_in           : std_logic;
signal write_cmd2             : std_logic;
signal write_cmd3             : std_logic;
signal write_cmd4             : std_logic;
signal write_cmd5             : std_logic;
signal write_cmd1             : std_logic;
signal write_cmd6             : std_logic;
signal write_cmd7             : std_logic;
signal write_cmd8             : std_logic;
signal write_cmd_issued       : std_logic;
signal GND                    : std_logic;
signal GO_TO_ACTIVE_value     : std_logic;
signal GO_TO_ACTIVE           : std_logic;
signal dqs_div_cascount       : std_logic_vector(2 downto 0);
signal dqs_div_rdburstcount   : std_logic_vector(2 downto 0);
signal DQS_enable1            : std_logic;
signal DQS_enable2            : std_logic;
signal DQS_enable3            : std_logic;
signal DQS_enable4            : std_logic;
signal DQS_enable5            : std_logic;
signal DQS_enable6            : std_logic;
signal DQS_enable7            : std_logic;
signal DQS_enable8            : std_logic;
signal DQS_reset1_clk0        : std_logic;
signal DQS_reset2_clk0        : std_logic;
signal DQS_reset3_clk0        : std_logic;
signal DQS_reset4_clk0        : std_logic;
signal DQS_reset5_clk0        : std_logic;
signal DQS_reset6_clk0        : std_logic;
signal DQS_reset7_clk0        : std_logic;
signal DQS_reset8_clk0        : std_logic;
signal DQS_enable_int         : std_logic;
signal DQS_reset_int          : std_logic;
signal rst180_r               : std_logic;
signal rst0_r                 : std_logic;
signal ddr_rasb4_cntrl        : std_logic;
signal ddr_casb4_cntrl        : std_logic;
signal ddr_web4_cntrl         : std_logic;
signal EMR                    : std_logic_vector(12 downto 0);
signal LMR_DLL_rst            : std_logic_vector(12 downto 0);
signal LMR_DLL_set            : std_logic_vector(12 downto 0);
signal ODT_COUNT              : std_logic_vector(1 downto 0);
signal ODT_COUNT_value        : std_logic_vector(1 downto 0);
signal ddr_ODT1               : std_logic;
signal ddr_ODT2               : std_logic;
signal ddr_ODT3               : std_logic;
signal ddr_ODT4               : std_logic;
signal ddr_ODT5               : std_logic;
--signal ODT_ack_o              : std_logic;
--signal ODT_ack_r            : std_logic;
signal init_done_1          : std_logic;
signal rst_dqs_div_int_in   : std_logic;
signal rst_dqs_div_int1     : std_logic;
signal rst_dqs_div_int2     : std_logic;
signal rst_dqs_div_int3     : std_logic;
signal vcc                  : std_logic;
signal cke_en               : std_logic;
signal NOP_COUNT            : std_logic_vector(1 downto 0);
signal NOP_COUNT_value      : std_logic_vector(1 downto 0);

signal low            	     : std_logic_vector(13 downto 0);
signal high            	     : std_logic_vector(1 downto 0);

signal EMR13                : std_logic_vector(13 downto 0);
signal LMR_DLL_rst13        : std_logic_vector(13 downto 0);
signal LMR_DLL_set13        : std_logic_vector(13 downto 0);

signal EMR12_13             : std_logic_vector((row_address_p - 13) downto 0);
signal LMR_DLL_rst12_13     : std_logic_vector((row_address_p - 13) downto 0);
signal LMR_DLL_set12_13     : std_logic_vector((row_address_p - 13) downto 0);
signal GO_TO_ODT_ON_value   : std_logic;
signal GO_TO_ODT_ON         : std_logic;
signal accept_cmd_in        : std_logic;
signal dqs_enable_i         : std_logic;


signal auto_ref_wait  : std_logic;
 signal auto_ref_wait1 : std_logic;
 signal auto_ref_wait2 : std_logic;




attribute syn_preserve : boolean;
attribute syn_preserve of cas_latency : signal is true;
--attribute syn_preserve of cas_latency1 : signal is true;

attribute syn_preserve of LMR_DLL_rst : signal is true;
attribute syn_preserve of LMR_DLL_set : signal is true;
attribute syn_preserve of config_reg1 : signal is true;
attribute syn_preserve of config_reg2 : signal is true;
attribute syn_preserve of address_reg : signal is true;
attribute syn_preserve of BA_address_reg : signal is true;  
attribute syn_preserve of COLUMN_ADDRESS_reg : signal is true;  
attribute syn_preserve of ROW_ADDRESS_reg : signal is true;  
attribute syn_preserve of current_state : signal is true;  
attribute syn_preserve of CAS_COUNT: signal is true;  
attribute syn_preserve of BA_address_active: signal is true;  
attribute syn_preserve of BA_address_conflict: signal is true;  
attribute syn_preserve of EMR: signal is true;  
attribute syn_keep of low : signal is true;
attribute syn_keep of high : signal is true;           
attribute syn_preserve of EMR13: signal is true;  
attribute syn_preserve of EMR12_13: signal is true;  
attribute syn_preserve of LMR_DLL_rst13: signal is true;  
attribute syn_preserve of LMR_DLL_rst12_13: signal is true;  
attribute syn_preserve of LMR_DLL_set13: signal is true;  
attribute syn_preserve of LMR_DLL_set12_13: signal is true;  

begin


low <= "00000000000000";
high <= "01";

EMR13 <= ('0' & EMR);
LMR_DLL_rst13 <= ('0' & LMR_DLL_rst);
LMR_DLL_set13 <= ('0' & LMR_DLL_set);

EMR12_13 <= EMR13((row_address_p - 1) downto 12);
LMR_DLL_rst12_13 <= LMR_DLL_rst13((row_address_p - 1) downto 12);
LMR_DLL_set12_13 <= LMR_DLL_set13((row_address_p - 1) downto 12);  


--  Input : CONFIG REGISTER FORMAT 
-- config_register = {   EMR(Enable/Disable DLL),
--                       BMR (Normal operation/Normal Operation with Reset DLL),
--                       BMR/EMR,
--                       CAS_latency (3),
--                       Burst type ,
--                       Burst_length (3) }
--
-- Input : COMMAND REGISTER FORMAT
--          000  - NOP
--          001  - Precharge 
--          010  - Auto Refresh
--          011  - SElf REfresh
--          100  - Write Request
--          101  - Load Mode Register
--          110  - Read request
--          111  - Burst terminate
--
-- Input : Address format
--   row address = input address(19 downto 8)
--   column addrs = input address( 7 downto 0)
--

ddr_csb_cntrl <= '0';
ddr_cke_cntrl <= NOT wait_200us;
ROW_ADDRESS <= address_reg(((row_address_p + column_address_p)-1) downto column_address_p); 
COLUMN_ADDRESS <= address_reg((column_address_p-1) downto 0);
init <= init_done;
GND <= '0';
vcc <= '1';

ddr_rasb_cntrl <= ddr_rasb1 ;
ddr_casb_cntrl <= ddr_casb1 ;
ddr_web_cntrl  <= ddr_web1 ;

rst_dqs_div_int <= rst_dqs_div_int1;
   

ddr_address_cntrl   <= ddr_address1;
ddr_ba_cntrl        <= ddr_ba1;

               
--  Input : CONFIG REGISTER FORMAT 
-- config_register1 = {  PD,WR,TM,EMR(Enable/Disable DLL),
--                       BMR (Normal operation/Normal Operation with Reset DLL),
--                       BMR/EMR,
--                       CAS_latency (3),
--                       Burst type ,
--                       Burst_length (3) }
--New BITS For Test Mode(TM), Write Recovery(WR), Poer Down (PD) in LMR and ODS,RTT, Posted CAS, DQS#,
-- RDQS,Out & OCD Program in EMR are added
-- config_register2 = {  Out,RDQS,DQS_n,OCD_Progm,Posted_Cas,RTT,ODS }
--
-- Input : COMMAND REGISTER FORMAT
--          000  - NOP
--          001  - Precharge 
--          010  - Auto Refresh
--          011  - SElf REfresh
--          100  - Write Request
--          101  - Load Mode Register
--          110  - Read request
--          111  - Burst terminate
--

process(clk180)
 begin
   if clk180'event and clk180 = '1' then 
     rst180_r <= rst180;
   end if;
end process;

process(clk)
 begin
   if clk'event and clk = '1' then 
     rst0_r <= rst0;
   end if;
end process;


--********************************************************************************************
-- register input commands from the user
-- 
--********************************************************************************************
  process(clk180)
  begin
   if clk180'event and clk180 = '1' then
    if rst180_r = '1' then
        config_reg1 <= "000000000000000";
        config_reg2 <= "0000000000000";
        command_reg <= "0000";
        ROW_ADDRESS_reg <= (others => '0');
        COLUMN_ADDRESS_reg <= (others => '0');
        BA_address_reg <= (others => '0');
        address_reg <= (others => '0');
       else
        config_reg1 <= config_register1;
        config_reg2 <= config_register2;
        command_reg <= command_register;
        ROW_ADDRESS_reg    <= address_reg(((row_address_p + column_address_p)-1) downto column_address_p);
        COLUMN_ADDRESS_reg <= address_reg((column_address_p-1) downto 0);
        BA_address_reg <= Bank_address;
        address_reg <= address;
    end if;
   end if;
  end process;

process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1' then
     burst_length <= "000";
     cas_latency  <= "000";
--     cas_latency1 <= "000";
     EMR          <= "0000000000000";
     LMR_DLL_rst  <= "0000000000000";
     LMR_DLL_set  <= "0000000000000";
  else
     burst_length <= config_reg1(2 downto 0);    
     cas_latency  <= config_reg1(6 downto 4);
--     cas_latency1 <= config_reg1(6 downto 4);
     EMR          <= (config_reg2(12 downto 7) & config_reg2(3) & config_reg2(6 downto 4) & config_reg2(2 downto 0));   
     LMR_DLL_rst  <= (config_reg1(14 downto 11) & vcc & config_reg1(10) & config_reg1(6 downto 0));--DLL is reset;
     LMR_DLL_set  <= (config_reg1(14 downto 11) & GND & config_reg1(10) & config_reg1(6 downto 0));--DLL is set;
  end if;
 end if;
end process;

accept_cmd_in <= '1' when ((current_state = IDLE) and (RP_COUNT = "000") and RFC_COUNT_reg = '1' 
                  and auto_ref_wait = '0') else '0';

PRECHARGE_cmd      <= '1' when (command_register = "0001" and accept_cmd_in = '1') else '0';
initialize_memory  <= '1' when (command_register = "0010") else '0';
write_cmd_in       <= '1' when (command_register = "0100" and accept_cmd_in = '1') else '0';
ld_mode            <= '1' when (command_register = "0101" and accept_cmd_in = '1') else '0';
read_cmd           <= '1' when (command_register = "0110" and accept_cmd_in = '1') else '0';


--**************************************************************************
-- write_cmd is used to determine when there is a new write request
--**************************************************************************

-- register write_cmd until WRITE command needs to be issued

 process(clk180)
  begin
  if clk180'event and clk180 = '1' then
     if rst180_r = '1' then
       write_cmd1 <= '0';
       write_cmd2 <= '0';
       write_cmd3 <= '0';
       write_cmd4 <= '0';
       write_cmd5 <= '0';
       write_cmd6  <= '0';
       write_cmd7  <= '0';
       write_cmd8  <= '0';
       write_cmd   <= '0';
     else
	  if (accept_cmd_in = '1') then
           write_cmd1 <= write_cmd_in;
        end if; 
       write_cmd2 <= write_cmd1;
       write_cmd3 <= write_cmd2;
       write_cmd4 <= write_cmd3;
       write_cmd5 <= write_cmd4; 
       write_cmd6 <= write_cmd5; 
       write_cmd7 <= write_cmd6; 
       write_cmd8  <= write_cmd7;   
       write_cmd  <= write_cmd6;               
     end if;
    end if;
  end process; 


--************************************************************************
-- register read cmd until READ command needs to be issued 
--************************************************************************

  process(clk180)
  begin
   if clk180'event and clk180 = '1' then
     if rst180_r = '1' then
       read_cmd1 <= '0';
       read_cmd2 <= '0';
       read_cmd3 <= '0';
       read_cmd4 <= '0';
       read_cmd5 <= '0';
       read_cmd6  <= '0';
       read_cmd7  <= '0';
       read_cmd8  <= '0';
       read_cmd_reg   <= '0';
       read_rcd_end   <= '0';
     else
	  if (accept_cmd_in = '1') then
           read_cmd1 <= read_cmd;
        end if; 
       read_cmd2 <= read_cmd1;
       read_cmd3 <= read_cmd2;
       read_cmd4 <= read_cmd3;
       read_cmd5 <= read_cmd4; 
       read_cmd6 <= read_cmd5;  
       read_cmd7 <= read_cmd6;
       read_cmd_reg    <= read_cmd7; 
       read_cmd8       <= read_cmd7; 
       read_rcd_end    <= read_cmd8;
     end if;
    end if;
  end process; 

--********************************************************************************************                
-- MRD Counter
-- an executable command can be issued only after Tmrd(2 cycles) after a LMR command is issued
--********************************************************************************************
MRD_COUNT_value <= "11" when (next_state = LOAD_MODE_REG) else
                   (MRD_COUNT - "01") when (MRD_COUNT /= "00") else
                    "00";
--********************************************************************************************                
-- RFC Counter
-- an executable command can be issued only after Trfc(60 ns => 60/5 = 12 cycles for 200MHz, 
-- 60 ns => 60/3.75 = 16 cycles for 266MHz  ) 
--                                after a AUTOREFRESH command is issued
--********************************************************************************************

RFC_COUNT_value <= RFC_COUNTER when (next_state = AUTO_REFRESH) else
                  (RFC_COUNT - "000001") when (RFC_COUNT /= "000000") else
                   "000000";
--********************************************************************************************                
-- RP Counter
-- an executable command can be issued only after Trp(20 ns for a -5 device => 4 cycles) 
--                        after a PRECHARGE command is issued
--********************************************************************************************
RP_cnt_value <= "100" when (next_state = PRECHARGE ) else
                (RP_COUNT - "001") when (RP_COUNT /= "000") else
                 "000";
--********************************************************************************************                
-- RRD Counter
-- minimum interval between successive ACTIVE commands to different banks - Trrd
-- 2 clock cycles
--********************************************************************************************
RRD_COUNT_value <= "10" when (next_state = ACTIVE) else
                   (RRD_COUNT - 1) when RRD_COUNT /= "00" else
                    "00"; 
--*********************************************************************************************
-- ACTIVE to READ/WRITE counter
-- RCDr counter
-- ACTIVE to READ delay - (-5) device requires 20 ns of delay => 4 clock cycles
--
-- RCDW counter
-- ACTIVE to WRITE delay - (-5) device requires 10 ns of delay => 2 clock cycles
--
--*********************************************************************************************


RCDR_COUNT_value <=  "101" when ( next_state = ACTIVE) else
                   (RCDR_COUNT - "001" ) when (RCDR_COUNT /= "000") else
                    "000";

RCDW_COUNT_value <= "100" when  next_state = ACTIVE else
                   (RCDW_COUNT - "001") when (RCDW_COUNT /= "000") else
                    "000";
--*********************************************************************************************
-- ACTIVE to PRECHARGE counter
-- RAS counter
-- ACTIVE to PRECHARGE delay -
-- the memory device require 40 ns (8 clock cycles)after issuing an ACTIVE command before issuing a
-- PRECHARGE command
--
--*********************************************************************************************
RAS_count_value <= "1000" when next_state = ACTIVE else
                    RAS_COUNT - "0001" when RAS_COUNT /= "0000" else
                    "0000";
--**********************************************************************************************
-- RC counter
-- an ACTIVE command to a different row can be issued only after the previous row has been 
-- precharged & after Trc is met 
-- Trc = 60 ns = 12 clock cycles
--**********************************************************************************************
RC_COUNT_value <= "1100" when next_state = ACTIVE else
                  (RC_COUNT - "0001") when (RC_COUNT /= "0000") else
                   "0000";
--********************************************************************************************                
-- WR Counter
-- a PRECHARGE command can be applied only after 2 cycles after a WRITE command has finished 
-- executing
--********************************************************************************************
 
  process (clk180)
  begin
  if clk180'event and clk180 = '1' then
   if rst180_r = '1' then
	WR_COUNT <= "000";
    else
	if (dqs_enable_i = '1') then
	  WR_COUNT <=  WRITE_RECOVERY_VAL ;
	elsif (WR_COUNT /= "000") then
          WR_COUNT <= WR_COUNT - "001";
        end if;  
   end if;
  end if;
end process;



ODT_COUNT_value <= "11" when (GO_TO_ODT_ON = '1') else
                  (ODT_COUNT - "01") when (ODT_COUNT /= "00") else
                  "00";

                  
--********************************************************************************************************
-- Auto refresh counter - the design uses AUTO REFRESH
-- the DDR SDRAM requires AUTO REFRESH cycles at an average interval of 7.8 us
-- Hence, a counter value to obtain a 7.7 us clock for Auto Refresh
-- (Refresh Request is raised for every 7.7 us to allow for termination of any ongoing bus transfer)
-- Hence for 200MHz frequency,The Refresh_count_value = freq * refresh_time_period = 267*7.7 = 2055 
--**********************************************************************************************************
--max_ref_cnt <= "100000000111";    -- AUTOREF_COUNT  (7.7 us) -- 110000110000 (15.6 us) --2048


max_ref_cnt <= REF_FREQ_CNT;    -- AUTOREF_COUNT  (7.7 us)

AUTOREF_value  <= '1' when (AUTOREF_COUNT = max_ref_cnt) else
                  '0';
                  
AUTOREF_CNT_val <= "000000000000" when ( (AUTOREF_COUNT = max_ref_cnt)  or (INIT_DONE = '0') ) else
                   AUTOREF_COUNT + "000000000001"; 

                          
process(clk180)   
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1' then
     AUTOREF_COUNT <= "000000000000";
  else
     AUTOREF_COUNT <= AUTOREF_CNT_val;
  end if;
 end if;
end process;

process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1'  then
      AUTO_REF_pulse_end <= '0';
      AUTO_REF_detect1   <= '0';
      AUTO_REF_detect    <= '0';
  else
      AUTO_REF_detect1   <= AUTOREF_value;
      AUTO_REF_detect    <= AUTO_REF_detect1;
      AUTO_REF_pulse_end <= AUTO_REF_detect;
  end if;
 end if;
end process;

AUTO_REF1 <= '1' when ((AUTO_REF_detect = '1') and (AUTO_REF_pulse_end /= '1'))  else
           '0';
           
AR_done_p   <= '1' when AR_Done_reg = '1' else
               '0';     
           
process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1' then
     auto_ref_wait   <= '0';
     AR_done         <= '0';
     Auto_Ref_issued <= '0';
   else
      if (auto_ref1 = '1' and auto_ref_wait ='0') then
        auto_ref_wait <= '1';
     elsif (Auto_Ref_issued = '1') then
        auto_ref_wait <= '0';
      else  
        auto_ref_wait <= auto_ref_wait;
     end if;

     AR_done <= AR_done_p;
     Auto_Ref_issued <= Auto_Ref_issued_p;

  end if;
 end if;
end process;


process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1' then
     auto_ref_wait1 <= '0';
     auto_ref_wait2 <= '0';
     auto_ref       <= '0';
     
   else
      if (Auto_Ref_issued = '1') then
       auto_ref_wait1 <= '0';
       auto_ref_wait2 <= '0';
       auto_ref       <= '0';
      else  
        auto_ref_wait1 <= auto_ref_wait;
        auto_ref_wait2 <= auto_ref_wait1;
        auto_ref       <= auto_ref_wait2;
     end if;
   end if;
 end if;
end process;
        
--auto_ref_req <= auto_ref;

auto_ref_req <= auto_ref_wait;


Auto_Ref_issued_p <= '1'  when (NEXT_STATE = AUTO_REFRESH) else '0';


--**********************************************************************************************
-- Burst count value counter when there are cosecutive READs or WRITEs
-- While doing consecutive READs or WRITEs, the Burst_count value determines when the next 
-- READ or WRITE command should be issued. The burst length is determined while loading the 
-- Load Mode Register
-- burst_cnt_max shows the number of clock cycles for each burst
-- burst_cnt_max = 1 for a burst length of 2, since it is ddr
--**********************************************************************************************
burst_cnt_max <= "010" when burst_length = "001" else
                 "010" when burst_length = "010" else
                 "100" when burst_length = "011" else
                 "100" when burst_length = "111" else
                 "000";

--********************************************************************************************
-- CAS latency counter
-- CAS latencies of 2,3,4 can be set using Mode register bits M(6:4)
--
--      M6  M5  M4   CAS latency
--      0    1   0 -    2
--      0    1   1 -    3
--      1    0   0 -    4
--      others     -   reserved
-- This design uses a CAS latency of 3 for a clock rate of 200 MHz
--
--********************************************************************************************
cas_count_value <= "10" when (NEXT_STATE = BURST_READ) else
                    CAS_COUNT - "01" when CAS_COUNT /= "00" else
                    "00";



RDburst_end_cnt_value <= burst_cnt_max when (CAS_COUNT = "01") else
                      RDBURST_END_CNT - "001" when RDBURST_END_CNT /= "000" else
                     "000" ;


WRburst_end_cnt_value <= burst_cnt_max  when ( (next_state = FIRST_WRITE) or (next_state = BURST_WRITE) ) else
                                                     
                         WRburst_end_cnt - "001" when WRburst_end_cnt /= "000" else
                         "000";


wrburst_chk <= '1' when ( (next_state = BURST_WRITE) or (next_state = WRITE_WAIT)  ) else
                      '0';
                      
rdburst_chk  <= '1' when ( (next_state = BURST_READ) or (next_state = READ_WAIT) ) else
                 '0';
process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1' then
    rdburst_end_1 <= '0';
    rdburst_end_2 <= '0';
    rdburst_end_3 <= '0';
    rdburst_end_4 <= '0';
    rdburst_end_5 <= '0';
  else
    if ( ( CAS_COUNT(1) = '1' or CAS_COUNT(0) = '1' or burst_cnt_max = "010")  and burst_done = '1') then 
       rdburst_end_1 <= '1';
    else
        rdburst_end_1 <= '0';
    end if;
    rdburst_end_2 <= rdburst_end_1;
    rdburst_end_3 <= rdburst_end_2;
    rdburst_end_4 <= rdburst_end_3;
    rdburst_end_5 <= rdburst_end_4;
    rdburst_end_6 <= rdburst_end_5;
    rdburst_end_7 <= rdburst_end_6;
    rdburst_end_8 <= rdburst_end_7;
  end if;
 end if;
end process;

rdburst_end_r <= rdburst_end_3 or rdburst_end_4 or rdburst_end_5 or rdburst_end_6 or rdburst_end_7 or rdburst_end_8;
rdburst_end <= rdburst_end_1 or rdburst_end_2 ;

process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1' then
     wrburst_end_1 <= '0';
     wrburst_end_2 <= '0';
     wrburst_end_3 <= '0';
     wrburst_end_4 <= '0';
     wrburst_end_5 <= '0';
     wrburst_end_6 <= '0';
     wrburst_end_7 <= '0';
     wrburst_end_8 <= '0';
     wrburst_end_9 <= '0';
  else
     if (  ( (WRburst_end_cnt(1) or WRburst_end_cnt(0)) = '1' or burst_cnt_max = "010") and burst_done = '1' ) then
         wrburst_end_1 <= '1';
     else 
         wrburst_end_1 <= '0';
     end if;
     wrburst_end_2 <= wrburst_end_1;
     wrburst_end_3   <= wrburst_end_2; 
     wrburst_end_4  <= wrburst_end_3;
     wrburst_end_5  <= wrburst_end_4;
     wrburst_end_6  <= wrburst_end_5;
     wrburst_end_7  <= wrburst_end_6;
     wrburst_end_8  <= wrburst_end_7;
  end if;
 end if;  
end process;

wrburst_end <= wrburst_end_1 or wrburst_end_2 or wrburst_end_3;
--**********************************************************************************************
-- to generate the Data Strobe enable and reset signal
-- The DQS signal needs to be generated center aligned with the data. 
-- The controller generates the DQS enable signal when the state machine is in the FIRST_WRITE
-- state,to take care of the write preamble
--**********************************************************************************************
DQS_enable_out <= '1' when ( (next_state = FIRST_WRITE) or (next_state = BURST_WRITE)  or (WRburst_end_cnt /= "000")  ) else
                  '0';

DQS_reset_out  <= '1' when next_state = FIRST_WRITE else
                 '0';

dqs_enable <= dqs_enable_i;

dqs_enable_i <= DQS_enable2 when (cas_latency = "011") else
              DQS_enable3 when (cas_latency = "100") else  
              DQS_enable1; 

dqs_reset  <= DQS_reset2_clk0 when (cas_latency = "011") else
              DQS_reset3_clk0 when (cas_latency = "100") else
              DQS_reset1_clk0;
              

process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1' then
    DQS_enable_int <= '0';
    DQS_reset_int  <= '0';
  else
    DQS_enable_int <= DQS_enable_out;
    DQS_reset_int  <= DQS_reset_out;
  end if;        
 end if;
end process;

process(clk)
begin
if clk'event and clk = '1' then
 if rst0_r = '1' then
   DQS_enable1    <= '0';
   DQS_enable2    <= '0';
   DQS_enable3    <= '0';
   DQS_enable4    <= '0'; 
   DQS_enable5    <= '0';
   DQS_enable6    <= '0'; 
   DQS_reset1_clk0 <= '0';
   DQS_reset2_clk0 <= '0';
   DQS_reset3_clk0 <= '0';
   DQS_reset4_clk0 <= '0';
   DQS_reset5_clk0 <= '0';
   DQS_reset6_clk0 <= '0';
 else                 
   DQS_enable1     <= DQS_enable_int;
   DQS_enable2     <= DQS_enable1;
   DQS_enable3     <= DQS_enable2;
   DQS_enable4     <= DQS_enable3;
   DQS_enable5     <= DQS_enable4;
   DQS_enable6     <= DQS_enable5;
   DQS_reset1_clk0 <= DQS_reset_int;
   DQS_reset2_clk0 <= DQS_reset1_clk0;
   DQS_reset3_clk0 <= DQS_reset2_clk0;
   DQS_reset4_clk0 <= DQS_reset3_clk0;
   DQS_reset5_clk0 <= DQS_reset4_clk0;
   DQS_reset6_clk0 <= DQS_reset5_clk0;
 end if;               
end if;                
end process;           
--****************************************************************************************************
--Generating WRITE and READ enable signals
--*****************************************************************************************************

write_enable_out <= '1' when ( (wrburst_chk = '1') or (WRburst_end_cnt /= "000"))  else
                     '0'; 

read_enable_out <= '1' when ( (cas_count /= "00" ) or (rdburst_chk = '1')  )else
                    '0'; 
                    
read_enable_out_r <= read_enable_out or rdburst_end_r;

process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1' then
   write_enable <= '0';
  else
   write_enable_out1 <= write_enable_out;
   write_enable_out2 <= write_enable_out1;
   write_enable_out3 <= write_enable_out2;
   if cas_latency = "011" then
      write_enable  <= write_enable_out;
   elsif cas_latency = "100" then
           write_enable  <= write_enable_out1;
   else
       write_enable  <= write_enable_out;
   end if;    
   
  end if;
 end if;
end process;

cmd_ack <= ack_reg;

ACK_REG_INST1 : FD port map ( 
                               Q => ack_reg,
                               D => ack_o,                             
                               C => clk180);     
                   


ack_o <= '1' when (  (  (cas_latency = "011") and ( write_cmd4 = '1' or read_cmd1 = '1') )   or 
                     (  (cas_latency = "100") and ( write_cmd5 = '1' or read_cmd2 = '1') ) )else
                '0';


--*********************************************************************************************
--  to initialize memory
--*********************************************************************************************

process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1' then
     init_memory <= '0';
     init_done <= '0';
  else
     init_memory <= init_mem;
     init_done <= init_done_value;
  end if;
 end if;
end process;

process (clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1' then
      INIT_PRE_COUNT <= "1101011"; --107
  else
       if(INIT_PRE_COUNT_en = '1') then
          INIT_PRE_COUNT <= INIT_PRE_COUNT - "0000001";
       else
          INIT_PRE_COUNT <= INIT_PRE_COUNT;
       end if;   
  end if;
 end if;
end process;

init_mem <=  '1' when initialize_memory = '1' else
--             '0' when (  (init_count = "1001") and (MRD_COUNT = "00") ) else
             '0' when (  (init_count = "1011") and (MRD_COUNT = "00") ) else
             init_memory;

-- counter for Memory Initialization sequence

INIT_COUNT_value <= (INIT_COUNT + "0001") when ( ( (next_state = PRECHARGE) or (next_state = LOAD_MODE_REG)
                                  or (next_state = AUTO_REFRESH) )) else
                    INIT_COUNT ;

--INIT_DONE_value <= '1' when (  (INIT_COUNT = "1001") and (DLL_RST_COUNT = "00000001") ) else
--                   '0';                    

INIT_DONE_value <= '1' when (  (INIT_COUNT = "1011") and (DLL_RST_COUNT = "00000001") ) else
                   '0';                    


-- counter to issue a PRECHARGE command, 400ns after giving NOP command during initialization
INIT_PRE_COUNT_en <= '1' when ( init_memory = '1' and INIT_PRE_COUNT > "0000001" ) else '0';

--Counter for DLL Reset complete
DLL_RST_COUNT_value <= "11001000" when (init_memory = '1' and INIT_COUNT = "010") else     --200
                        (DLL_RST_COUNT - "00000001") when (DLL_RST_COUNT /= "00000001") else
                        "00000001";   


-- State machine goes to ACTIVE state directly with every read command                             
 GO_TO_ACTIVE_value <= '1' when  ( read_cmd = '1' and read_cmd1 /= '1')  else
                             '0';
                             
-- ODT is enabled with every write command                             
 GO_TO_ODT_ON_value <= '1' when  ( write_cmd_in = '1' and write_cmd1 /= '1')  else
                             '0';

-- To check if there is a bank conflict after an ACTIVE command has been issued for a particular bank

CONFLICT_value  <= '1' when (RRD_COUNT = "01" and BA_address_conflict = '1') else
                   '0';
process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1' then
   CONFLICT     <= '0';
   GO_TO_ACTIVE <= '0';
   GO_TO_ODT_ON <= '0';
  else
   CONFLICT     <= CONFLICT_value;
   GO_TO_ACTIVE <= GO_TO_ACTIVE_value;
   GO_TO_ODT_ON <= GO_TO_ODT_ON_value;
  end if;
 end if;
end process;

--**********************************************************************************************
-- Register counter values
--**********************************************************************************************
process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1'  then
     INIT_COUNT     <= "0000";
     DLL_RST_COUNT  <= "00000000";
     RP_COUNT       <= "000";
     MRD_COUNT      <= "00";
     RFC_COUNT      <= "000000";
     RAS_COUNT      <= "0000";
     CAS_COUNT      <= "00";
     RRD_COUNT      <= "00";
     RCDR_COUNT     <= "000";
     RCDW_COUNT     <= "000";
     RC_COUNT       <= "0000";
     RDBURST_END_CNT <= "000";
     WRburst_end_cnt <= "000";  
     ODT_COUNT       <= "00";
     AR_Done_reg     <= '0';
     RFC_COUNT_reg   <= '0';
  else
     --INIT_COUNT     <= INIT_COUNT_value;

    if init_memory = '1' then
       INIT_COUNT    <= INIT_COUNT_value;
        else
       INIT_COUNT    <= INIT_COUNT;
      end if;

     DLL_RST_COUNT  <= DLL_RST_COUNT_value;
     RP_COUNT       <= RP_CNT_value;
     MRD_COUNT      <= MRD_COUNT_value after 1 ns;
     RFC_COUNT      <= RFC_COUNT_value;
     RAS_COUNT      <= RAS_COUNT_value;
     CAS_COUNT      <= CAS_COUNT_value;
     RRD_COUNT      <= RRD_COUNT_value;
     RCDR_COUNT     <= RCDR_COUNT_value;
     RCDW_COUNT     <= RCDW_COUNT_value;
     RC_COUNT       <= RC_COUNT_value;
     WRBURST_END_CNT <= WRBURST_END_CNT_value;
     RDBURST_END_CNT <= RDBURST_END_CNT_value;
     ODT_COUNT       <= ODT_COUNT_value;
     if(RFC_COUNT = "000010") then --2
        AR_Done_reg <= '1';
     else
       AR_Done_reg <= '0';
     end if;  
     
     if(AR_Done_reg = '1') then 
        RFC_COUNT_reg <= '1';
     elsif (Auto_Ref_issued_p = '1') then
        RFC_COUNT_reg <= '0';
     else        
       RFC_COUNT_reg <= RFC_COUNT_reg;
     end if;   
  end if;
  
 end if;
end process;
--*********************************************************************************************
-- to check current state for the address bus
--*********************************************************************************************
process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1'  then
   current_state <= IDLE;
  else
   current_state <= next_state;
  end if;
 end if;
end process;
--*********************************************************************************************
-- main state machine
--*********************************************************************************************

process (rst180_r , RP_COUNT , INIT_COUNT , MRD_COUNT , RFC_COUNT , CAS_COUNT , WR_COUNT , GO_TO_ACTIVE , ld_mode ,
          write_cmd3 , read_cmd5 , CONFLICT , current_state , ODT_COUNT , wrburst_end , wrburst_end_cnt ,
           burst_length , rdburst_end , INIT_MEMORY , RCDW_COUNT , RCDR_COUNT , PRECHARGE_CMD , INIT_PRE_COUNT
           , GO_TO_ODT_ON , auto_ref , RFC_COUNT_reg )
begin
--if clk180'event and clk180 = '1' then
if rst180_r = '1' then
    next_state <= IDLE;
else
    case current_state is 
      
         when IDLE =>
               if INIT_MEMORY = '1' then
                  case INIT_COUNT is 

                   -- this state is for NOP/Deselect
                   when "0000" =>
                          if(INIT_PRE_COUNT = "0000001") then
                              next_state <= PRECHARGE;
                          else
                              next_state <= IDLE;
                          end if; 
                        
                   when "0001" =>
                         if RP_COUNT = "000" then   
                              next_state <= LOAD_MODE_REG;
                         else
                               next_state <= IDLE;
                         end if;
                   when "0010" =>
                         if MRD_COUNT = "01" then
                            next_state <= LOAD_MODE_REG;  -- for reseting DLL in Base Mode register
                         else
                            next_state <= IDLE;
                         end if;
                   when "0011" =>
                         if MRD_COUNT = "01" then
                            next_state <= LOAD_MODE_REG; -- For EMR
                         else
                            next_state <= IDLE;
                         end if;
                   when "0100" =>
                         if MRD_COUNT = "01" then
                            next_state <= LOAD_MODE_REG; -- For EMR
                         else
                            next_state <= IDLE;
                         end if;
                   when "0101" =>
                         if MRD_COUNT = "01" then
                            next_state <= PRECHARGE; 
                         else
                            next_state <= IDLE;
                         end if;

                   when "0110" => 
                         if RP_COUNT = "000" then -- wait for 4 clock cycles (Trp)
                               next_state <= AUTO_REFRESH;
                         else
                               next_state <= IDLE;
                         end if;
                   when "0111" =>
                         if RFC_COUNT_reg = '1' then
                           next_state <= AUTO_REFRESH;
                         else 
                           next_state <= IDLE;
                         end if;
                   when "1000" =>
                         if RFC_COUNT_reg = '1' then
                            next_state <= LOAD_MODE_REG;  -- to deactivate the rst DLL bit in the LMR
                         else
                            next_state <= IDLE;
                         end if;
                   when "1001" =>
                         if MRD_COUNT = "01" then
                            next_state <= LOAD_MODE_REG;  -- to set OCD to default in EMR
                         else
                            next_state <= IDLE;
                         end if;
                   when "1010" =>
                         if MRD_COUNT = "01" then
                            next_state <= LOAD_MODE_REG;  --  OCD exit in EMR
                         else
                            next_state <= IDLE;
                         end if;
                   when "1011" =>
                         if MRD_COUNT = "01" then
                           next_state <= IDLE;
			 else 
			       next_state <= current_state;	
                         end if;
                   when others =>
                        next_state <= IDLE;
                   end case; 

            elsif( auto_ref = '1' and RFC_COUNT_reg = '1' and RP_COUNT = "000" ) then
                  next_state <= AUTO_REFRESH; -- normal Refresh in the IDLE state

            elsif PRECHARGE_CMD = '1' then
                  next_state <= PRECHARGE;

            elsif ld_mode = '1' then 
                 next_state <= LOAD_MODE_REG;
                             
            elsif GO_TO_ODT_ON = '1' or CONFLICT = '1' then   
                  next_state <= ODT_TURN_ON;
              
           elsif GO_TO_ACTIVE = '1'  or CONFLICT = '1' then
                  next_state <= ACTIVE; 
           

            else
                  next_state <= IDLE;
            end if;              

        when ODT_TURN_ON =>
                 if ODT_COUNT = "00" then 
                  next_state <= ACTIVE;
                 else                                  
                  next_state <= ODT_TURN_ON;
                 end if;  

         when PRECHARGE =>
              next_state <= IDLE;

         when LOAD_MODE_REG =>
              next_state <= IDLE;
      
         when AUTO_REFRESH =>
              next_state <= IDLE; 
         
         when ACTIVE =>
              next_state <= ACTIVE_WAIT;

        when ACTIVE_WAIT =>
                if (RCDW_COUNT = "01" and write_cmd3 = '1') then
                    next_state <= FIRST_WRITE;
                elsif (RCDR_COUNT = "001" and read_cmd5 = '1') then
                    next_state <= BURST_READ;
                else    
                    next_state <= ACTIVE_WAIT;
                end if;

         when FIRST_WRITE =>
              -- to meet the write preamble   
              if (burst_length = "001" )  then
                  next_state <= BURST_WRITE;
              else
                  next_state <= WRITE_WAIT;
              end if;

         when WRITE_WAIT =>
                     case wrburst_end is
                       when '1' =>
                         next_state <= PRECHARGE_AFTER_WRITE;  
                     
                     when '0' =>
                            --for RL
                            if WRburst_end_cnt = "001"  then-- changed from 010
                                 next_state <= BURST_WRITE;
                            else
                                 next_state <= WRITE_WAIT;
                            end if;
                     when others =>
                            next_state <= WRITE_WAIT;
                         
                     end case;

          when BURST_WRITE =>
                     if (burst_length = "001" ) then
                          next_state <= BURST_WRITE;
                     else
                         next_state <= WRITE_WAIT;
                     end if; 
         
          when READ_after_WRITE =>
                     next_state <= BURST_READ;

          when PRECHARGE_AFTER_WRITE =>
                        next_state <= PRECHARGE_AFTER_WRITE_2;
                     
          when PRECHARGE_AFTER_WRITE_2 =>   
                     if(WR_COUNT = "00") then  
                        next_state <= PRECHARGE;
                     else
                        next_state <= PRECHARGE_AFTER_WRITE_2;
                     end if;  

          when READ_WAIT =>
                     case rdburst_end is
                    when '1' =>
                             next_state <= PRECHARGE_AFTER_WRITE;
                    when '0' =>
                         if CAS_COUNT = "01" then
                             next_state <= BURST_READ;
                         else
                             next_state <= READ_WAIT;
                         end if;
                    when others =>
                         next_state <= READ_WAIT;
                    end case;

          when BURST_READ =>
                     if (burst_length = "001" )then
                          next_state <= BURST_READ;
                     else 
                         next_state <= READ_WAIT;
                     end if; 
                    
          when others =>
                next_state <= IDLE;
        end case;
  end if; 
end process;    

--************************************************************************************************
-- address generation logic
--************************************************************************************************
address_config((row_address_p-1) downto 7) <= (EMR12_13 & EMR(11 downto 7)) when INIT_COUNT = "0100" else -- EMR
							    (EMR12_13 & EMR(11 downto 10) & "111") when INIT_COUNT = "1010" else -- EMR(OCD set to default)
							    (EMR12_13 & EMR(11 downto 10) & "000") when INIT_COUNT = "1011" else -- EMR(OCD exit)
                               (others => '0') when INIT_COUNT = "0010" else         -- EMR (2)
                               (others => '0') when INIT_COUNT = "0011" else         -- EMR (3)
                               (LMR_DLL_rst12_13 & LMR_DLL_rst(11 downto 7)) when (INIT_COUNT = "0101" or (current_state = LOAD_MODE_REG and INIT_COUNT /= "1001") ) else
                               (LMR_DLL_set12_13 & LMR_DLL_set(11 downto 7)) when (INIT_COUNT = "1001" and current_state /= PRECHARGE) else
                               (low(row_address_p-12 downto 0) & "1000") when current_state = PRECHARGE else 
                               (others => '0');
                               
address_config(6 downto 4) <=  EMR(6 downto 4) when (INIT_COUNT = "0100" or INIT_COUNT = "1010" or INIT_COUNT = "1011")else  --EMR
                               "000" when (INIT_COUNT = "0010" or INIT_COUNT = "0011") else --EMR(2) & EMR(3)
                               cas_latency when current_state = LOAD_MODE_REG else
                                "000";
                                
address_config(3)          <= EMR(3) when (INIT_COUNT = "0100" or INIT_COUNT = "1010" or INIT_COUNT = "1011") else '0'; -- design uses sequential burst


address_config(2 downto 0) <= EMR(2 downto 0) when (INIT_COUNT = "0100" or INIT_COUNT = "1010" or INIT_COUNT = "1011") else                 -- EMR
                              "000" when (INIT_COUNT = "0010" or INIT_COUNT = "0011")  else -- EMR(2) & EMR(3)
                               burst_length when current_state = LOAD_MODE_REG else
                              "000";


ddr_address1 <=    address_config when (current_state = LOAD_MODE_REG or current_state = PRECHARGE) else
                   ROW_ADDRESS_reg when (current_state = ACTIVE) else
                   (low((row_address_p-column_address_p-1) downto 0) & COLUMN_ADDRESS_reg) when current_state = BURST_WRITE or current_state = FIRST_WRITE or
                   current_state = BURST_READ else (others => '0');

ddr_ba1 <=     (low((bank_address_p-2) downto 0) & '1') when  (current_state = LOAD_MODE_REG and (INIT_COUNT = "0100" or INIT_COUNT = "1010" or INIT_COUNT = "1011" ) ) else       --EMR
               (high((bank_address_p-2) downto 0) & '0') when  (current_state = LOAD_MODE_REG and INIT_COUNT = "0010")  else       --EMR(2)
               (high((bank_address_p-2) downto 0) & '1') when  (current_state = LOAD_MODE_REG and INIT_COUNT = "0011")  else       --EMR(3)
               BA_ADDRESS_reg when ( current_state = ACTIVE or current_state = FIRST_WRITE or current_state = BURST_WRITE or current_state = BURST_READ)else
               (others => '0');

ddr_ODT1 <=  '1' when (write_cmd8 = '1') else '0';
    
--********************************************************************************************************
--  register row address
--********************************************************************************************************
process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1' then
   row_address_active_reg <= (others => '0');
   row_address_conflict <= '0';
  else
   if next_state = ACTIVE then
       row_address_active_reg <= row_address_reg;
   else
       row_address_active_reg <= row_address_active_reg;
   end if;
   if row_address_reg /= row_address_active_reg then
       row_address_conflict <= '1';
   else
       row_address_conflict <= '0';
   end if;
  end if;   
 end if;
end process;

--********************************************************************************************************
--  register bank address
--********************************************************************************************************

process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1' then
   BA_address_active <= (others => '0');
   BA_address_conflict <= '0';
  else
   if next_state = ACTIVE then
       BA_address_active <= BA_address_reg;
   else
       BA_address_active <= BA_address_active;
   end if;
   if BA_address_reg /= BA_address_active then
       BA_address_conflict <= '1';
   else
       BA_address_conflict <= '0';
   end if;
  end if;   
 end if;
end process;

--********************************************************************************************************
--  register column address
--********************************************************************************************************
process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1' then
     column_address_reg1 <= (others => '0');
     column_address_reg2 <= (others => '0');
     column_address_reg3 <= (others => '0');
     column_address_reg4 <= (others => '0');
     column_address_reg5 <= (others => '0');
     column_address_reg6 <= (others => '0');
  else
     column_address_reg1 <= column_address_reg;
     column_address_reg2 <= column_address_reg1;
     column_address_reg3 <= column_address_reg2;
     column_address_reg4 <= column_address_reg3;
     column_address_reg5 <= column_address_reg4;
     column_address_reg6 <= column_address_reg5;
  end if;
 end if;
end process;



--**************************************************************************************************
--Pipeline stages for ddr_address and ddr_ba
--**************************************************************************************************

process(clk180)
begin
if clk180'event and clk180 = '1' then 
if rst180_r = '1' then
   ddr_address2  <= (others => '0');
   ddr_address3  <= (others => '0');
   ddr_ba2       <= (others => '0');
   ddr_ba3       <= (others => '0');
   ddr_ODT2      <= '0';
   ddr_ODT3      <= '0';
else
    ddr_address2 <= ddr_address1;
    ddr_address3 <= ddr_address2;
    ddr_ba2      <= ddr_ba1;
    ddr_ba3      <= ddr_ba2;
    ddr_ODT2     <= ddr_ODT1;
    ddr_ODT3     <= ddr_ODT2;
end if;
end if;
end process;

process(clk180)
begin
if clk180'event and clk180 = '1' then 
 if rst180_r = '1' then
    ddr_ODT4        <= '0';
    ddr_ODT5        <= '0';
    ddr_ODT_cntrl   <= '0';
 else
    ddr_ODT4        <= ddr_ODT3;
    ddr_ODT5        <= ddr_ODT4;
    ddr_ODT_cntrl   <= ddr_ODT2;
 end if;
end if;
end process;

process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1' then
   ddr_address4  <= (others => '0');
 --  ddr_address_cntrl   <= (others => '0');
  else
   ddr_address4  <= ddr_address3;
 --   ddr_address_cntrl   <= ddr_address1;
  end if;
 end if;
end process;

process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1' then
     ddr_ba4 <= (others => '0');
 --    ddr_ba_cntrl <= (others => 'Z');
  else
     ddr_ba4 <= ddr_ba3;--ba3
  --     ddr_ba_cntrl  <= ddr_ba1;

  end if;
 end if;
end process;

--************************************************************************************************
-- control signals to the Memory
--************************************************************************************************

ddr_rasb1 <= '0' when ( current_state = ACTIVE or current_state = PRECHARGE or current_state = AUTO_REFRESH 
                    or current_state = LOAD_MODE_REG) else
             '1';

ddr_casb1 <= '0' when ( current_state = BURST_READ or current_state = BURST_WRITE or current_state = FIRST_WRITE
                      or current_state = AUTO_REFRESH or current_state = LOAD_MODE_REG) else
             '1';

ddr_web1  <= '0' when (current_state = BURST_WRITE or current_state = FIRST_WRITE 
                        or current_state = PRECHARGE or current_state = LOAD_MODE_REG) else
             '1';

--*************************************************************************************************
-- register CONTROL SIGNALS outputs
--**************************************************************************************************
process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1'  then
   ddr_rasb3 <= '1';
   ddr_casb3 <= '1';
   ddr_web3  <= '1';
   ddr_rasb2 <= '1';
   ddr_casb2 <= '1';
   ddr_web2  <= '1';
  else
   ddr_rasb2    <= ddr_rasb1;
   ddr_casb2    <= ddr_casb1;
   ddr_web2     <= ddr_web1;
   ddr_rasb3     <= ddr_rasb2;
   ddr_casb3    <= ddr_casb2;
   ddr_web3     <= ddr_web2;
  end if;
 end if;
end process;

process(clk180)
begin
 if clk180'event and clk180 = '1' then
  if rst180_r = '1' then
   ddr_rasb4_cntrl  <= '1';
   ddr_casb4_cntrl  <= '1';
   ddr_web4_cntrl   <= '1'; 
   ddr_rst_dqs_rasb4  <= '1';
   ddr_rst_dqs_casb4  <= '1';
   ddr_rst_dqs_web4   <= '1'; 
   ddr_rasb4          <= '1';
   ddr_casb4          <= '1';
   ddr_web4           <= '1';
   ddr_rasb5          <= '1';
   ddr_casb5          <= '1';
   ddr_web5           <= '1';
  else
      ddr_rasb4          <= ddr_rasb3;
      ddr_casb4          <= ddr_casb3;
      ddr_web4           <= ddr_web3;
      ddr_rasb5          <= ddr_rasb4;
      ddr_casb5          <= ddr_casb4;
      ddr_web5           <= ddr_web4;
--for rst_dqs_div
--      if (cas_latency = "011") then -- CL3
--         ddr_rst_dqs_rasb4  <= ddr_rasb1; --for RL
-- 	 ddr_rst_dqs_casb4  <= ddr_casb1;
--   	 ddr_rst_dqs_web4   <= ddr_web1; 
      if(cas_latency = "100") then -- CL4
         ddr_rst_dqs_rasb4  <= ddr_rasb1;
 	 ddr_rst_dqs_casb4  <= ddr_casb1;
   	 ddr_rst_dqs_web4   <= ddr_web1; 
      else
        ddr_rst_dqs_rasb4  <= ddr_rst_dqs_rasb4;
   	ddr_rst_dqs_casb4  <= ddr_rst_dqs_casb4;
   	ddr_rst_dqs_web4   <= ddr_rst_dqs_web4; 
      end if;
  end if;  
 end if; 
end process;

process(clk180)
begin
  if clk180'event and clk180 = '1' then
    if rst180_r = '1' then
        dqs_div_cascount <= "000";
        dqs_div_rdburstcount <= "000";
        rst_dqs_div_r <= '0';
    else
        if (ddr_rst_dqs_rasb4 = '1' and ddr_rst_dqs_casb4 = '0' and ddr_rst_dqs_web4 = '1' and cas_latency = "100") then
             dqs_div_cascount <= "010";
        elsif ( ddr_rasb1 = '1' and ddr_casb1 = '0' and ddr_web1 = '1' and cas_latency = "011" ) then
             dqs_div_cascount <= "010";
        else
           if dqs_div_cascount /= "000" then
             dqs_div_cascount <= dqs_div_cascount - "001";
           else
              dqs_div_cascount <= dqs_div_cascount;
           end if;
       end if;
        if (dqs_div_cascount = "01") then
            dqs_div_rdburstcount <= "010"; --?? changed from "010" to "011"
        else 
            if dqs_div_rdburstcount /= "00" then
               dqs_div_rdburstcount <= dqs_div_rdburstcount - "01";
            else
               dqs_div_rdburstcount <= dqs_div_rdburstcount;
            end if;
        end if;
        if (dqs_div_cascount = "001" and burst_length = "010") then
            rst_dqs_div_r <= '1';
        elsif (dqs_div_cascount = "011" and burst_length = "011") then
            rst_dqs_div_r <= '1';
        elsif (dqs_div_rdburstcount = "001" and dqs_div_cascount = "000") then
            rst_dqs_div_r <= '0';
        else
            rst_dqs_div_r <= rst_dqs_div_r;
        end if;
    end if;      
  end if;
end process;    

rst_calib0 : FD port map (
                         Q    => rst_calib,     
                         D    => rst_dqs_div_r,                         
                         C    => clk180);         
        

rst_iob_out : FD port map (
                          Q    => rst_dqs_div_int1,     
                          D    => rst_dqs_div_r,                         
                          C    => clk180); --CLK180
                      
                                  
                            
end arc_controller_16bit_00;                
