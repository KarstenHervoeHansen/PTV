--******************************************************************************
--
--  Xilinx, Inc. 2002                 www.xilinx.com
--
--  XAPP 253 - Synthesizable DDR SDRAM Controller
--
--*******************************************************************************
--
--  File name :       data_path_iobs.vhd
--
--  Description :     All the inputs and outputs related to data path module 
--                    are declared here. The outputs and inputs are 
--                    registered within the IOB's.                   
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
-- pragma translate_off
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
-- pragma translate_on
--
entity   data_path_iobs_16bit  is
port(
    clk               : in std_logic;
  clk180 : in std_logic;
  clk270 : in std_logic;
    dqs_reset         : in std_logic;
    dqs_enable        : in std_logic;
    ddr_dqs           : inout std_logic_vector(1 downto 0);
    ddr_dqs_n         : inout std_logic_vector(1 downto 0);
    ddr_dq            : inout std_logic_vector(15 downto 0);
    write_data_falling: in std_logic_vector(15 downto 0);
    write_data_rising : in std_logic_vector(15 downto 0);
    write_en_val      : in std_logic;
    clk90             : in std_logic;
    reset270_r        : in std_logic;
    data_mask_f       : in std_logic_vector(1 downto 0);
    data_mask_r       : in std_logic_vector(1 downto 0);
    dqs_int_delay_in0 : out std_logic;
    dqs_int_delay_in1 : out std_logic;
    dq                : out std_logic_vector(15 downto 0);
    ddr_dm            : out std_logic_vector(1 downto 0)                
);
end   data_path_iobs_16bit;  

architecture   arc_data_path_iobs_16bit of   data_path_iobs_16bit    is


component ddr_dqs_iob
port(
     clk            : in std_logic;
  clk180 : in std_logic;
     ddr_dqs_reset  : in std_logic;
     ddr_dqs_enable : in std_logic;
     ddr_dqs        : inout std_logic;
     ddr_dqs_n      : inout std_logic;
     dqs            : out std_logic
     );
end component;

component ddr_dq_iob
port (
      ddr_dq_inout       : inout std_logic; --Bi-directional SDRAM data bus
      write_data_falling : in std_logic;    --Transmit data, output on falling edge
      write_data_rising  : in std_logic;    --Transmit data, output on rising edge
      read_data_in       : out std_logic;   -- Received data
  clk270 : in std_logic;
      clk90              : in std_logic;    --Clock 90
      write_en_val       : in std_logic;    --Transmit enable
      reset              : in std_logic); 
end component;


component	ddr2_dm_16bit 
port (                                                
      ddr_dm       : out std_logic_vector(1 downto 0);
      mask_falling : in std_logic_vector(1 downto 0); 
      mask_rising  : in std_logic_vector(1 downto 0); 
  clk270 : in std_logic;
      clk90        : in std_logic    --Clock 90                          
      );                                              
end component; 


component IOBUF_SSTL2_II
port (
       I  : in std_logic;
       T  : in std_logic;
       IO : inout std_logic;
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



begin

  
--***********************************************************************
-- DQS IOB instantiations
--***********************************************************************

 ddr_dqs_iob0 : ddr_dqs_iob port map (
                              clk            => clk,
		      clk180   =>    clk180,
                              ddr_dqs_reset  => dqs_reset,
                              ddr_dqs_enable => dqs_enable,
                              ddr_dqs        => ddr_dqs(0),
                              ddr_dqs_n      => ddr_dqs_n(0),
                              dqs            => dqs_int_delay_in0
                             );
                             
 ddr_dqs_iob1 : ddr_dqs_iob port map (
                              clk            => clk,
		      clk180   =>    clk180,
                              ddr_dqs_reset  => dqs_reset,
                              ddr_dqs_enable => dqs_enable,
                              ddr_dqs        => ddr_dqs(1),
                              ddr_dqs_n      => ddr_dqs_n(1),
                              dqs            => dqs_int_delay_in1
                             );
                             
--***********************************************************************
-- Dq IOB instantiations
--***********************************************************************                            );

ddr_dq_iob0 : ddr_dq_iob port map( 
                                     ddr_dq_inout       => ddr_dq(0),
                                     write_data_rising  => write_data_rising(0),
                                     write_data_falling => write_data_falling(0),
                                     read_data_in       => dq(0),
                                     clk90              => clk90,
		      clk270   =>    clk270,
                                     write_en_val       => write_en_val,
                                     reset              => reset270_r
                                     );

ddr_dq_iob1 : ddr_dq_iob port map( 
                                     ddr_dq_inout       => ddr_dq(1),
                                     write_data_rising  => write_data_rising(1),
                                     write_data_falling => write_data_falling(1),
                                     read_data_in       => dq(1),
                                     clk90              => clk90,
		      clk270   =>    clk270,
                                     write_en_val       => write_en_val,
                                     reset              => reset270_r
                                     );

ddr_dq_iob2 : ddr_dq_iob port map( 
                                     ddr_dq_inout       => ddr_dq(2),
                                     write_data_rising  => write_data_rising(2),
                                     write_data_falling => write_data_falling(2),
                                     read_data_in       => dq(2),
                                     clk90              => clk90,
		      clk270   =>    clk270,
                                     write_en_val       => write_en_val,
                                     reset              => reset270_r
                                     );

ddr_dq_iob3 : ddr_dq_iob port map( 
                                     ddr_dq_inout       => ddr_dq(3),
                                     write_data_rising  => write_data_rising(3),
                                     write_data_falling => write_data_falling(3),
                                     read_data_in       => dq(3),
                                     clk90              => clk90,
		      clk270   =>    clk270,
                                     write_en_val       => write_en_val,
                                     reset              => reset270_r
                                     );

ddr_dq_iob4 : ddr_dq_iob port map( 
                                     ddr_dq_inout       => ddr_dq(4),
                                     write_data_rising  => write_data_rising(4),
                                     write_data_falling => write_data_falling(4),
                                     read_data_in       => dq(4),
                                     clk90              => clk90,
		      clk270   =>    clk270,
                                     write_en_val       => write_en_val,
                                     reset              => reset270_r
                                     );

ddr_dq_iob5 : ddr_dq_iob port map( 
                                     ddr_dq_inout       => ddr_dq(5),
                                     write_data_rising  => write_data_rising(5),
                                     write_data_falling => write_data_falling(5),
                                     read_data_in       => dq(5),
                                     clk90              => clk90,
		      clk270   =>    clk270,
                                     write_en_val       => write_en_val,
                                     reset              => reset270_r
                                     );

ddr_dq_iob6 : ddr_dq_iob port map( 
                                     ddr_dq_inout       => ddr_dq(6),
                                     write_data_rising  => write_data_rising(6),
                                     write_data_falling => write_data_falling(6),
                                     read_data_in       => dq(6),
                                     clk90              => clk90,
		      clk270   =>    clk270,
                                     write_en_val       => write_en_val,
                                     reset              => reset270_r
                                     );

ddr_dq_iob7 : ddr_dq_iob port map( 
                                     ddr_dq_inout       => ddr_dq(7),
                                     write_data_rising  => write_data_rising(7),
                                     write_data_falling => write_data_falling(7),
                                     read_data_in       => dq(7),
                                     clk90              => clk90,
		      clk270   =>    clk270,
                                     write_en_val       => write_en_val,
                                     reset              => reset270_r
                                     );

ddr_dq_iob8 : ddr_dq_iob port map( 
                                     ddr_dq_inout       => ddr_dq(8),
                                     write_data_rising  => write_data_rising(8),
                                     write_data_falling => write_data_falling(8),
                                     read_data_in       => dq(8),
                                     clk90              => clk90,
		      clk270   =>    clk270,
                                     write_en_val       => write_en_val,
                                     reset              => reset270_r
                                     );

ddr_dq_iob9 : ddr_dq_iob port map( 
                                     ddr_dq_inout       => ddr_dq(9),
                                     write_data_rising  => write_data_rising(9),
                                     write_data_falling => write_data_falling(9),
                                     read_data_in       => dq(9),
                                     clk90              => clk90,
		      clk270   =>    clk270,
                                     write_en_val       => write_en_val,
                                     reset              => reset270_r
                                     );

ddr_dq_iob10 : ddr_dq_iob port map( 
                                     ddr_dq_inout       => ddr_dq(10),
                                     write_data_rising  => write_data_rising(10),
                                     write_data_falling => write_data_falling(10),
                                     read_data_in       => dq(10),
                                     clk90              => clk90,
		      clk270   =>    clk270,
                                     write_en_val       => write_en_val,
                                     reset              => reset270_r
                                     );

ddr_dq_iob11 : ddr_dq_iob port map( 
                                     ddr_dq_inout       => ddr_dq(11),
                                     write_data_rising  => write_data_rising(11),
                                     write_data_falling => write_data_falling(11),
                                     read_data_in       => dq(11),
                                     clk90              => clk90,
		      clk270   =>    clk270,
                                     write_en_val       => write_en_val,
                                     reset              => reset270_r
                                     );

ddr_dq_iob12 : ddr_dq_iob port map( 
                                     ddr_dq_inout       => ddr_dq(12),
                                     write_data_rising  => write_data_rising(12),
                                     write_data_falling => write_data_falling(12),
                                     read_data_in       => dq(12),
                                     clk90              => clk90,
		      clk270   =>    clk270,
                                     write_en_val       => write_en_val,
                                     reset              => reset270_r
                                     );

ddr_dq_iob13 : ddr_dq_iob port map( 
                                     ddr_dq_inout       => ddr_dq(13),
                                     write_data_rising  => write_data_rising(13),
                                     write_data_falling => write_data_falling(13),
                                     read_data_in       => dq(13),
                                     clk90              => clk90,
		      clk270   =>    clk270,
                                     write_en_val       => write_en_val,
                                     reset              => reset270_r
                                     );

ddr_dq_iob14 : ddr_dq_iob port map( 
                                     ddr_dq_inout       => ddr_dq(14),
                                     write_data_rising  => write_data_rising(14),
                                     write_data_falling => write_data_falling(14),
                                     read_data_in       => dq(14),
                                     clk90              => clk90,
		      clk270   =>    clk270,
                                     write_en_val       => write_en_val,
                                     reset              => reset270_r
                                     );

ddr_dq_iob15 : ddr_dq_iob port map( 
                                     ddr_dq_inout       => ddr_dq(15),
                                     write_data_rising  => write_data_rising(15),
                                     write_data_falling => write_data_falling(15),
                                     read_data_in       => dq(15),
                                     clk90              => clk90,
		      clk270   =>    clk270,
                                     write_en_val       => write_en_val,
                                     reset              => reset270_r
                                     );

--***********************************************************************
--  DM IOB instantiations
--***********************************************************************  


ddr2_dm0	:	ddr2_dm_16bit	port	map	( 
                             ddr_dm       => ddr_dm(1 downto 0),
		      clk270   =>    clk270,
                             mask_falling => data_mask_f(1 downto 0),
                             mask_rising  => data_mask_r(1 downto 0),
                             clk90        => clk90
                            );    


end   arc_data_path_iobs_16bit;  
         
 




   