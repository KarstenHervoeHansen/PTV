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

-- SMPTE-292 descrambler mux/demux wrapper file

----------------------------------------------------------------------------------------------------------
--
-- Revision History:
--	3-25-04	Original Draft
----------------------------------------------------------------------------------------------------------
 
LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY smpte_292 IS 
    PORT (
        -- scrambler tx I/O
        txclk,                         -- HOTLink TX CKW clock
        tx_reset,                      -- active high reset
        tx_oe,                         -- output enable
        tx_bypass: IN std_logic;       -- raw data mode - bypass scrambler
		tx_ten_twenty : in std_logic;  -- switch for determining either a 10 or 20 bit interface
        trs_filt_lower10: IN std_logic;-- TRS filter lower 10 bits
        trs_filt_upper10: IN std_logic;-- TRS filter upper 10 bits
        txdata_in: IN std_logic_vector(0 to 19); -- raw data in from Co-Processor
        trs_det: out std_logic;        -- TRS code detected
        txdata_out: out std_logic_vector(0 to 9)); -- parallel scrambled output to Hotlink II PHY
        -- de-scrambler rx I/O
--		rxclk,                         -- CY7B933 CKR recovered clock
  --      rx_reset,                      -- active high reset
    --    rx_oe,                         -- output enable
      --  rx_bypass: IN std_logic;       -- raw data mode, bypass scrambler data
		--rx_ten_twenty : in std_logic;  -- switch for determining either a 10 or 20 bit interface
--        sync_en: IN std_logic;         -- TRS/SYNC correction enabled
  --      rxdata_in: IN std_logic_vector(0 to 9);   -- scrambled data input from HOTLink II PHY
    --    rxdata_out: out std_logic_vector(0 to 19); -- descrambled and framed data
      --  sync_err: out std_logic;       -- SYNC offset error pulse
        --h_sync: out std_logic);        -- horizontal sync output toggle
END smpte_292;


----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------

ARCHITECTURE structure OF smpte_292 IS

component scram20_top PORT (
        txclk,                      -- HOTLink TX CKW clock
        reset,                      -- active high reset added by DEF 11-6-03
        tx_oe,                      -- output enable
        bypass: IN std_logic;       -- raw data mode - bypass scrambler
	ten_twenty : in std_logic;  -- switch for determining either a 10 or 20 bit interface
        trs_filt_lower10: IN std_logic; -- TRS filter lower 10 bits
        trs_filt_upper10: IN std_logic; -- TRS filter upper 10 bits
        txdata_in: IN std_logic_vector(0 to 19); -- raw data in
        trs_det: out std_logic;     -- TRS code detected, either 3FFh or 000h
        txdata_out: out std_logic_vector(0 to 9)); -- parallel scrambled output
END component;

----------------------------------------------------------------------------------------
--component dscram20_top PORT (
  --      rxclk,                      -- CY7B933 CKR recovered clock
    --    reset,                      -- active high reset added by DEF 11-6-03
      --  rx_oe,                      -- output enable
        --bypass: IN std_logic;       -- raw data mode, bypass scrambler data
--	ten_twenty : in std_logic;  -- switch for determining either a 10 or 20 bit interface
  --      sync_en: IN std_logic;      -- TRS/SYNC correction enabled
    --    rxdata_in: IN std_logic_vector(0 to 9);   -- scrambled data from HOTLink RX
      --  rxdata_out: out std_logic_vector(0 to 19); -- descrambled and framed data
        --sync_err: out std_logic;    -- SYNC offset error pulse
--        h_sync: out std_logic);     -- horizontal sync output toggle
--END component;

----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------


BEGIN

----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
scram20_top_inst : scram20_top port map(
        txclk => txclk,                      -- HOTLink TX CKW clock
        reset => tx_reset,                      -- active high reset added by DEF 11-6-03
        tx_oe => tx_oe,                      -- output enable
        bypass => tx_bypass,       -- raw data mode - bypass scrambler
	ten_twenty => tx_ten_twenty,  -- switch for determining either a 10 or 20 bit interface
        trs_filt_lower10 => trs_filt_lower10, -- TRS filter lower 10 bits
        trs_filt_upper10 => trs_filt_upper10, -- TRS filter upper 10 bits
        txdata_in => txdata_in, -- raw data in
        trs_det => trs_det,     -- TRS code detected
        txdata_out => txdata_out); -- parallel scrambled output

----------------------------------------------------------------------------------------
--dscram20_top_inst : dscram20_top PORT map(
  --      rxclk => rxclk,                      -- CY7B933 CKR recovered clock
    --    reset => rx_reset,                      -- active high reset added by DEF 11-6-03
      --  rx_oe => rx_oe,                      -- output enable
        --bypass => rx_bypass,       -- raw data mode, bypass scrambler data
--	ten_twenty => rx_ten_twenty,  -- switch for determining either a 10 or 20 bit interface
  --      sync_en => sync_en,      -- TRS/SYNC correction enabled
    --    rxdata_in => rxdata_in,   -- scrambled data from HOTLink RX
      --  rxdata_out => rxdata_out, -- descrambled and framed data
        --sync_err => sync_err,  -- SYNC offset error pulse
--        h_sync => h_sync);    -- horizontal sync output toggle

----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------

end structure;
