--******************************************************************************
--
--  Xilinx, Inc. 2002                 www.xilinx.com
--
--  XAPP 253 - Synthesizable DDR SDRAM Controller
--
--*******************************************************************************
--
--  File name :       controller_iobs.vhd
--
--  Description :     
--                  All the IO's related to controller module are declared as
--                  IOBUF's in this module
--
--  Date - revision : 05/01/2002
--
--  Author :          Lakshmi Gopalakrishnan ( Modified by Sailaja)
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
entity controller_iobs_16bit_00 is
port(
    clk0              : in std_logic;
  clk180 : in std_logic;
    ddr_rasb_cntrl   : in std_logic;
    ddr_casb_cntrl   : in std_logic;
    ddr_web_cntrl    : in std_logic;
    ddr_cke_cntrl    : in std_logic;
    ddr_csb_cntrl    : in std_logic;
    ddr_ODT_cntrl    : in std_logic;
    ddr_address_cntrl: in std_logic_vector((row_address_p -1) downto 0);
    ddr_ba_cntrl     : in std_logic_vector((bank_address_p -1) downto 0);
    rst_dqs_div_int  : in std_logic;
    rst_dqs_div_in   : in std_logic;
    ddr_rasb         : out std_logic;
    ddr_casb         : out std_logic;
    ddr_web          : out std_logic;
    ddr_ba           : out std_logic_vector((bank_address_p -1) downto 0);
    ddr_address      : out std_logic_vector((row_address_p -1) downto 0);
    ddr_cke          : out std_logic;
    ddr_csb          : out std_logic;
    ddr_ODT0         : out std_logic;
    rst_dqs_div      : out std_logic;
    rst_dqs_div_out  : out std_logic
   
);
end controller_iobs_16bit_00;


architecture arc_controller_iobs_16bit_00 of controller_iobs_16bit_00 is

attribute xc_props : string;
attribute syn_keep : boolean;

component IOBUF_SSTL18_II
port (
       I  : in std_logic;
       T  : in std_logic;
       IO : inout std_logic;
       O  : out std_logic);
end component;

component IBUF
port (
       I  : in std_logic;
       O  : out std_logic);
end component;

component OBUF_HSTL_II_18

port (
       I  : in std_logic;
       O  : out std_logic);
end component;

component FD
   port(
      Q                              :  out   STD_LOGIC;
      D                              :  in    STD_LOGIC;
      C                              :  in    STD_LOGIC);
end component;

component OBUF
 port (
   O : out std_logic;
   I : in std_logic);
 end component;


---- **************************************************
---- iob attributes for instantiated FD components
---- **************************************************

signal GND        : std_logic;
signal ddr_web_q  : std_logic;
signal ddr_rasb_q : std_logic;
signal ddr_casb_q : std_logic;
signal ddr_cke_q  : std_logic;

signal ddr_address_reg : std_logic_vector((row_address_p -1) downto 0);
signal ddr_ba_reg      : std_logic_vector((bank_address_p -1) downto 0);

attribute xc_props of iob_web:  label is "IOB=TRUE";
attribute xc_props of iob_rasb: label is "IOB=TRUE";
attribute xc_props of iob_casb: label is "IOB=TRUE";
attribute xc_props of iob_cke: label is "IOB=TRUE";

 
begin

GND <= '0';
  

---- ******************************************* ----
----  Includes the instantiation of FD for cntrl ----
----            signals                          ----
---- ******************************************* ----
           
iob_web : FD port map (
                         Q    => ddr_web_q,
                         D    => ddr_web_cntrl,
                         C    => clk180);
                         
iob_rasb : FD port map (
                         Q    => ddr_rasb_q,
                         D    => ddr_rasb_cntrl,
                         C    => clk180);
                         
iob_casb : FD port map (
                         Q    => ddr_casb_q,
                         D    => ddr_casb_cntrl,
                         C    => clk180);                                                  
                                                        
---- ************************************* ----
----  Output buffers for control signals   ----
---- ************************************* ----

r16 : OBUF port map (
                     I => ddr_web_q,
                     O => ddr_web);

r17 : OBUF port map (
                     I => ddr_rasb_q,
                     O => ddr_rasb);

r18 : OBUF port map (
                     I => ddr_casb_q,
                     O => ddr_casb);

r19 : OBUF port map (
                     I => ddr_csb_cntrl,
                     O => ddr_csb);


iob_cke : FD port map(
	               Q => ddr_cke_q,
      	         D => ddr_cke_cntrl,
      	         C => clk180); 

                
r20 : OBUF port map (
        		   I => ddr_cke_q,
			   O => ddr_cke);


--r20 : OBUF port map (
--                     I => ddr_csb_cntrl,
--                     O => ddr_csb);


r21 : OBUF port map (
                     I => ddr_ODT_cntrl,
                     O => ddr_ODT0);


---- ************************************* ----
----  Output buffers for address signals   ----
---- ************************************* ----

iob_addr_0 : FD port map (
                         Q    => ddr_address_reg(0),
                         D    => ddr_address_cntrl(0),
                         C    => clk180); 
iob_addr_1 : FD port map (
                         Q    => ddr_address_reg(1),
                         D    => ddr_address_cntrl(1),
                         C    => clk180); 
iob_addr_2 : FD port map (
                         Q    => ddr_address_reg(2),
                         D    => ddr_address_cntrl(2),
                         C    => clk180); 
iob_addr_3 : FD port map (
                         Q    => ddr_address_reg(3),
                         D    => ddr_address_cntrl(3),
                         C    => clk180); 
iob_addr_4 : FD port map (
                         Q    => ddr_address_reg(4),
                         D    => ddr_address_cntrl(4),
                         C    => clk180); 
iob_addr_5 : FD port map (
                         Q    => ddr_address_reg(5),
                         D    => ddr_address_cntrl(5),
                         C    => clk180); 
iob_addr_6 : FD port map (
                         Q    => ddr_address_reg(6),
                         D    => ddr_address_cntrl(6),
                         C    => clk180); 
iob_addr_7 : FD port map (
                         Q    => ddr_address_reg(7),
                         D    => ddr_address_cntrl(7),
                         C    => clk180); 
iob_addr_8 : FD port map (
                         Q    => ddr_address_reg(8),
                         D    => ddr_address_cntrl(8),
                         C    => clk180); 
iob_addr_9 : FD port map (
                         Q    => ddr_address_reg(9),
                         D    => ddr_address_cntrl(9),
                         C    => clk180); 
iob_addr_10 : FD port map (
                         Q    => ddr_address_reg(10),
                         D    => ddr_address_cntrl(10),
                         C    => clk180); 
iob_addr_11 : FD port map (
                         Q    => ddr_address_reg(11),
                         D    => ddr_address_cntrl(11),
                         C    => clk180); 
iob_addr_12 : FD port map (
                         Q    => ddr_address_reg(12),
                         D    => ddr_address_cntrl(12),
                         C    => clk180); 
-- generater_prob

iob_ba_0 : FD port map (
                         Q    => ddr_ba_reg(0),
                         D    => ddr_ba_cntrl(0),
                         C    => clk180); 
iob_ba_1 : FD port map (
                         Q    => ddr_ba_reg(1),
                         D    => ddr_ba_cntrl(1),
                         C    => clk180); 
-- prob_ba

G1: for i in (row_address_p -1) downto 0  generate

r:OBUF port map (
                    I => ddr_address_reg(i),
                     O => ddr_address(i));
end generate G1;

G2: for i in (bank_address_p -1) downto 0  generate


r:OBUF port map (
                     I => ddr_ba_reg(i),
                     O => ddr_ba(i));

end generate G2;                     


  ---************************************** Copied from board test logic ****************************************---

                        
rst_iob_inbuf  :  IBUF  port map
                            ( I  => rst_dqs_div_in,
                              O  => rst_dqs_div);
  

                            
rst_iob_outbuf  :  OBUF  port map
                            ( I  => rst_dqs_div_int,
                              O  => rst_dqs_div_out);

----************************************** Copied from board test logic ****************************************---
                       
end arc_controller_iobs_16bit_00;                
         
 




