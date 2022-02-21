-- Copyright 1997, Cypress Semiconductor Corporation

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

LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY smpte_259 IS
    --GENERIC ( );
    PORT (
        -- Descrambler signals
--        rxclk,                      -- CY7B933 CKR recovered clock
  --      reset_n,
    --    DVB_select: IN std_logic;   -- select 8B/10B mode (active LOW)
      --  rxbypass: IN std_logic;       -- raw data mode, bypass scrambler data
        --SYNC_en: IN std_logic;      -- TRS/SYNC correction enabled
--        rxoe: IN std_logic;           -- tristate output enable
  --      data_in: IN std_logic_vector (0 TO 9);
                                    -- scrambled data from HOTLink RX
    --    rxdata_out: buffer std_logic_vector (0 TO 9);
                                    -- descrambled and framed data
      --  SYNC_error: out std_logic;   -- SYNC offset error pulse
        --RF: out std_logic;       -- HOTLink RX frame control pin
--        AB: out std_logic;       -- HOTLink A/B port select pin
  --      H_SYNC: out std_logic;     -- horizontal sync output toggle

        -- Scrambler Signals
        txclk,                      -- HOTLink TX CKW clock
        DVB_EN: IN std_logic;       -- select 8B/10B mode (active LOW)
        trs_filt: IN std_logic;     -- TRS filter
        ena_in: IN std_logic;       -- /ENA input
        enn_in: IN std_logic;       -- /ENN input
        SVS_en: IN std_logic;       -- enable transmission of SVS
        SCD_en: IN std_logic;       -- enable selection of commands
        txoe: IN std_logic;           -- tristate output enable
        txbypass: IN std_logic;       -- raw data mode - bypass scrambler
                                    -- 10-bit raw data interface
        data: IN std_logic_vector(0 to 9);
        trs_out: out std_logic;  -- TRS code detected
        ena_out: out std_logic;  -- HOTLink TX/ENA input
        txdata_out: BUFFER std_logic_vector(0 to 9) -- parallel scrambled output
);
END smpte_259;

ARCHITECTURE a OF smpte_259 IS
--COMPONENT dscramrx
  --  PORT(
    --    rxclk,                      -- CY7B933 CKR recovered clock
      --  reset_n,
        --DVB_select: IN std_logic;   -- select 8B/10B mode (active LOW)
--        bypass: IN std_logic;       -- raw data mode, bypass scrambler data
  --      SYNC_en: IN std_logic;      -- TRS/SYNC correction enabled
    --    oe: IN std_logic;           -- tristate output enable
      --  data_in: IN std_logic_vector (0 TO 9);
                                    -- scrambled data from HOTLink RX
--        data_out: BUFFER std_logic_vector (0 TO 9);
                                    -- descrambled and framed data
--        SYNC_error: out std_logic;   -- SYNC offset error pulse
  --      RF: out std_logic;       -- HOTLink RX frame control pin
    --    AB: out std_logic;       -- HOTLink A/B port select pin
      --  H_SYNC: out std_logic);  -- horizontal sync output toggle
--END COMPONENT;

COMPONENT scramtx
    PORT (
        txclk,                      -- HOTLink TX CKW clock
        DVB_EN: IN std_logic;       -- select 8B/10B mode (active LOW)
        trs_filt: IN std_logic;     -- TRS filter
        ena_in: IN std_logic;       -- /ENA input
        enn_in: IN std_logic;       -- /ENN input
        SVS_en: IN std_logic;       -- enable transmission of SVS
        SCD_en: IN std_logic;       -- enable selection of commands
        oe: IN std_logic;           -- tristate output enable
        bypass: IN std_logic;       -- raw data mode - bypass scrambler
                                    -- 10-bit raw data interface
        data: IN std_logic_vector(0 to 9);
        trs_out: out std_logic;  -- TRS code detected
        ena_out: out std_logic;  -- HOTLink TX/ENA input
        data_out: BUFFER std_logic_vector(0 to 9));
                                    -- parallel scrambled output
END COMPONENT;
BEGIN

--u1: dscramrx
  --  PORT MAP(
    --    rxclk => rxclk,
      --  reset_n => reset_n,
        --DVB_select => DVB_select,
--        bypass => rxbypass,
  --      SYNC_en => SYNC_en,
    --    oe => rxoe,
      --  data_in => data_in,
        --data_out => rxdata_out,
--        SYNC_error => SYNC_error,
  --      RF => RF,
    --    AB => AB,
      --  H_SYNC => H_SYNC
--);

u2: scramtx
    PORT MAP(
        txclk => txclk,
        DVB_EN => DVB_en,
        trs_filt => trs_filt,
        ena_in => ena_in,
        enn_in => enn_in,
        SVS_en => SVS_en,
        SCD_en => SCD_en,
        oe => txoe,
        bypass => txbypass,
        data => data,
        trs_out => trs_out,
        ena_out => ena_out,
        data_out => txdata_out);


END a;
