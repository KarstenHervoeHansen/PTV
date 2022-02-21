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
--	3-25-04	Original Draft
----------------------------------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY scram20_top IS
    PORT (
        txclk,                      -- HOTLink TX CKW clock
        reset,                      -- active high reset
        tx_oe,                      -- output enable
        bypass:      IN std_logic;       -- raw data mode - bypass scrambler
        enable_sync: IN std_logic;
		  txdata_in:   IN std_logic_vector(0 to 19); -- raw data in
        txdata_out:  out std_logic_vector(0 to 19)); -- parallel scrambled output
END scram20_top;

ARCHITECTURE behavior of scram20_top IS

signal reset_sync, bypass_int: std_logic;
component scram20 PORT(
        txclk,                      -- HOTLink TX CKW clock
        reset,                      -- active high reset added by DEF 11-6-03
        enable_sync : IN std_logic;
		  bypass: IN std_logic;       -- raw data mode - bypass scrambler
        txd_in: IN std_logic_vector(0 to 19);
        txd_out: out std_logic_vector(0 to 19)); -- parallel scrambled output
END component;


BEGIN
		
scram20_inst : scram20 PORT MAP(
        txclk => txclk,                     
        reset => reset_sync, 
		  enable_sync => enable_sync,                      
        bypass => bypass_int,      
        txd_in => txdata_in,            
        txd_out => txdata_out);      

		
synchronous_reset : process(txclk) 
begin
if txclk'event and txclk = '1' then
	if enable_sync = '1' then
   	reset_sync <= reset;
    	bypass_int <= bypass;
	end if;
end if;
end process;

END behavior;