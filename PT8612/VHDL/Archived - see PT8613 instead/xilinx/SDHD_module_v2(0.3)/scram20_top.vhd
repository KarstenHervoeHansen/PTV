-- Copyright 2003-2004, Cypress Semiconductor Corporation

-- This SOFTWARE is owned by Cypress Semiconductor Corporation
-- (Cypress) and is protected by United States copyright laws and 
-- international treaty provisions.  Therefore, you must treat this 
-- SOFTWARE like any other copyrighted material (e.g., book, or musical 
-- recording), with the exception that one copy may be made for personal 
-- use or evaluation.  Reproduction, modification, translation, 
-- compilation, or representation of this software in any other form 
-- (e.g., paper, magnetic, optical, silicon, etc.) is prohibited 
-- without the express written permission of Cypress.  

-- This SOFTWARE is protected by and subject to worldwide patent
-- coverage, including U.S. and foreign patents. Use is limited by
-- and subject to the Cypress Software License Agreement.

-- SMPTE-292 scrambler mux/demux wrapper file

-- This design takes an 20 bit parallel data stream and
-- encodes it for serialization using the SMPTE scrambler algorithm.
-- This includes the x^9 + x^4 + 1 scrambler and the x + 1 NRZI
-- encoder.

----------------------------------------------------------------------------------------------------------
--
-- Revision History:
--	3-25-04	   Original Draft
-- 2005-10-21  THH   
----------------------------------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;
use work.pack_reverse_bit_order.all;


ENTITY scram20_top IS
   PORT (
      clk_i        : IN std_logic;
      zreset_i     : IN std_logic;                       -- active low reset
      bypass_i     : IN std_logic;                       -- bypass scrambler
      enable_i     : IN std_logic;                       
	   luma_i       : IN std_logic_vector(9 downto 0);    -- raw luma data in
	   chroma_i     : IN std_logic_vector(9 downto 0);    -- raw chroma data in
      data_o       : out std_logic_vector(19 downto 0)); -- parallel scrambled output
END scram20_top;


ARCHITECTURE behavior of scram20_top IS

signal data          : std_logic_vector(0 to 19);
signal scramb_data   : std_logic_vector(0 to 19);

component scram20 PORT(
        txclk,
        zreset_i,
        enable_sync  : IN std_logic;
		  bypass       : IN std_logic;                   -- bypass scrambler
        txd_in       : IN std_logic_vector(0 to 19);
        txd_out      : out std_logic_vector(0 to 19)); -- parallel scrambled output
END component;


BEGIN


make_data: process (clk_i)
begin
	if  clk_i'event and clk_i = '1' then
		if enable_i = '1' then
			data <= reverse_bit_order(chroma_i,10) & reverse_bit_order(luma_i,10);
		end if;
	end if;
end process;

		
scram20_inst : scram20 PORT MAP(
        txclk => clk_i,                     
        zreset_i => zreset_i, 
		  enable_sync => enable_i,                      
        bypass => bypass_i,      
        txd_in => data,            
        txd_out => scramb_data);      


-- mapping of output signals:

		data_o(19) <= scramb_data(19);
		data_o(18) <= scramb_data(18);
		data_o(17) <= scramb_data(17);
	   data_o(16) <= scramb_data(16);
		data_o(15) <= scramb_data(15);
		data_o(14) <= scramb_data(14);
		data_o(13) <= scramb_data(13);
	   data_o(12) <= scramb_data(12);		
		data_o(11) <= scramb_data(11);
		data_o(10) <= scramb_data(10);
		data_o(9)  <= scramb_data(9);
	   data_o(8)  <= scramb_data(8);
		data_o(7)  <= scramb_data(7);
	   data_o(6)  <= scramb_data(6);
		data_o(5)  <= scramb_data(5);
		data_o(4)  <= scramb_data(4);
		data_o(3)  <= scramb_data(3);
	   data_o(2)  <= scramb_data(2);		
		data_o(1)  <= scramb_data(1);
		data_o(0)  <= scramb_data(0);
      
        
END behavior;