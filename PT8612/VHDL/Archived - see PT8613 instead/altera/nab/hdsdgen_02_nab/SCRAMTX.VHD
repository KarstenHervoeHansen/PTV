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

-- CY7C9235 SMPTE Encoder Design

-- This design takes an 8 or 10 bit parallel data stream and
-- encodes it for serialization using the SMPTE scrambler algorithm.
-- This includes the x^9 + x^4 + 1 scrambler and the x + 1 NRZI
-- encoder.

-- Top end target clock rate is 40MHz (25ns) for a -83 speed-bin


LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY scramtx IS
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
END scramtx;


ARCHITECTURE archscram of scramtx IS
        
SIGNAL  ina,inb,inc : std_logic;      -- intermediate XOR terms
SIGNAL  ind,ine,inf : std_logic;      -- intermediate XOR terms
SIGNAL  ing         : std_logic;      -- intermediate XOR terms
SIGNAL  trs_l,trs_h : std_logic;      -- TRS detection gates
SIGNAL  ena         : std_logic;      -- HOTLink TX /ENA signal
SIGNAL  ena1        : std_logic;      -- latched /ENA input
SIGNAL  enn1        : std_logic;      -- latched /ENN input
SIGNAL  enn2        : std_logic;      -- pipelined /ENN input
SIGNAL  trs         : std_logic;      -- TRS characters detected
SIGNAL  svsen       : std_logic;      -- latched SVS_en
SIGNAL  scden       : std_logic;      -- latched SC/D_en
SIGNAL  DVB_enl     : std_logic;      -- latched DVB_EN
SIGNAL  trs_filtl   : std_logic;      -- latched TRS_FILT
SIGNAL  bypassl     : std_logic;      -- latched bypass
SIGNAL  inp         : std_logic_vector(0 TO 9);   -- internal XOR terms
SIGNAL  iregA    : std_logic_vector(0 TO 9);   -- input data register
SIGNAL  iregB    : std_logic_vector(0 TO 9);   -- TRS filter register
SIGNAL  scrData     : std_logic_vector(0 TO 9);   -- scrambler register
SIGNAL  scrout      : std_logic_vector(0 TO 9);   -- output data of scrambler
SIGNAL  dout        : std_logic_vector(0 TO 9);   -- output register
SIGNAL  nrzi2       : std_logic_vector(0 TO 9);   -- intermediate NRZI XOR reg
SIGNAL  nrzi1       : std_logic_vector(0 TO 9);   -- intermediate NRZI XOR reg


BEGIN

--------------------------------------------------------------------
--------------------------------------------------------------------
-- Declare the input holding register.  This register accepts raw 
-- 10-bit data on each rising edge of the clock.

-- The input register has two different modes of operation.  With
-- the DVB_EN inactive (HIGH), the input is a 10-bit register
-- configured to accept serial data on every clock cycle and to
-- scramble and encode that data on consecutive clocks.

-- The two sets of signal inputs for this register are:
--      SMPTE       DVB
--      -------     -------
--      data(9)     SVS     (MSB)
--      data(8)     data(7)
--      data(7)     data(6)  
--      data(6)     data(5)
--      data(5)     data(4)
--      data(4)     data(3)
--      data(3)     data(2)
--      data(2)     data(1)
--      data(1)     data(0)
--      data(0)     SC/D    (LSB) 

-- Per the SMPTE-259M spec, the LSB of any word is always 
-- transmited first.  This means bit-0 of the input register is the
-- "first" bit routed through the scrambler and NRZI encoder.
     
RegA: PROCESS BEGIN
    WAIT UNTIL txclk = '1'; -- wait for clock
    -- capture input data word and control signals at rising edge of CKW
        iregA <= data;
        enn2 <= enn1;
        enn1 <= enn_in;
        ena1 <= ena_in;
        svsen <= SVS_en;
        scden <= SCD_en;
        DVB_enl <= DVB_EN;
        trs_filtl <= trs_filt;
        bypassl <= bypass;
END PROCESS RegA;

--------------------------------------------------------------------
--------------------------------------------------------------------
-- declare TRS filter stage.  This logic filters out the low order
-- bits on the input data during TRS fields.  Since comparators
-- are needed to perform this function, and the same comparators
-- are also needed for the TRS detect function, they are both 
-- created in this block.

-- detect all zeros and all ones conditions
trs_l <= '1' WHEN (iregA(2 TO 9) = "00000000") ELSE '0';
trs_h <= '1' WHEN (iregA(2 TO 9) = "11111111") ELSE '0';

-- set TRS output detect flag
trs_det: PROCESS BEGIN
    WAIT UNTIL txclk = '1';
        trs <= NOT ((trs_l OR trs_h) AND dvb_enl);  
        -- set flag if either TRS code is detected and in SMPTE mode
END PROCESS trs_det;


-- bits 0 and 1 do require filtering
filter: PROCESS (iregA,trs_filtl,trs_l,trs_h) BEGIN

        -- always pass upper bits
        iregB(2 TO 9) <= iregA(2 TO 9);  

        -- next check for either TRS character on the upper 8-bits 
        IF (trs_filtl = '0' AND (trs_l = '1' OR trs_h = '1')) THEN
            -- if a trs character is present
            iregB(0 TO 1) <= iregA(2) & iregA(2);
        ELSE
            -- if normal data, pass all bits
            iregB(0 TO 1) <= iregA(0 TO 1);
        END IF;
END PROCESS filter;
        

--------------------------------------------------------------------
--------------------------------------------------------------------
-- The following equations map the functionality of the scrambler.
-- The inp() asignments are the data inputs to the output/scrambler
-- register.  This allows a full 10-bits of input data to be 
-- scrambled in a single clock cycle.

-- These are intermediate XOR terms that are used in 
-- multiple locations.  They are broken out separately to
-- simplify the following equations
    ina    <= iregB(4)   XOR scrData(1)  XOR scrData(5);
    inb    <= iregB(0)   XOR scrData(9)  XOR scrData(5);
    inc    <= scrData(1)    XOR scrData(2)  XOR scrData(6);
    ind    <= scrData(2)    XOR scrData(3)  XOR scrData(7);
    ine    <= scrData(3)    XOR scrData(4)  XOR scrData(8);
    inf    <= scrData(4)    XOR scrData(5)  XOR scrData(9);

-- these assignments define the scrambler as implemented for
-- 10-bit parallel operation
    inp(1) <= iregB(9)   XOR ina         XOR inb;
    inp(2) <= iregB(8)   XOR iregB(3) XOR inc;
    inp(3) <= iregB(7)   XOR iregB(2) XOR ind;
    inp(4) <= iregB(6)   XOR iregB(1) XOR ine;
    inp(5) <= iregB(5)   XOR iregB(0) XOR inf;
    inp(6) <= iregB(4)   XOR scrData(1)  XOR scrData(5);
    inp(7) <= iregB(3)   XOR scrData(2)  XOR scrData(6);
    inp(8) <= iregB(2)   XOR scrData(3)  XOR scrData(7);
    inp(9) <= iregB(1)   XOR scrData(4)  XOR scrData(8);
    inp(0) <= iregB(0)   XOR scrData(5)  XOR scrData(9);

--------------------------------------------------------------------
--------------------------------------------------------------------
-- This process defines the operation of the scrambler register.
-- This register is enabled at every clock.  It takes the data from 
-- the filtered input register
scram_reg: PROCESS BEGIN
    WAIT UNTIL txclk = '1';
        scrData <= inp; -- load scrambled data into the scrambler register
END PROCESS scram_reg;

-- re-map scrabler names to actual output names
        scrout(0) <= scrData(0); -- LSB
        scrout(1) <= scrData(9);
        scrout(2) <= scrData(8);
        scrout(3) <= scrData(7);
        scrout(4) <= scrData(6);
        scrout(5) <= scrData(5);
        scrout(6) <= scrData(4);
        scrout(7) <= scrData(3);
        scrout(8) <= scrData(2);
        scrout(9) <= scrData(1); -- MSB


--------------------------------------------------------------------
--------------------------------------------------------------------
-- declare NRZI encoder

-- following the scrambling operation, the same data must be NRZI 
-- encoded.  This requires a large number of XOR terms to encode
-- all 10 bits in parallel.  This is faster to encode by using
-- multiple register stages, and only encoding a few bits at each
-- stage.  The final encode is performmed in the output register.

-- encoder is built from multiple stages to limit the number of XOR
-- terms between clock stages to allow maximum clock rate


nrzi_enc: PROCESS BEGIN
    WAIT UNTIL txclk = '1';

    -- output minus two state register
    nrzi2(0 TO 5) <= scrout(0 TO 5);
    nrzi2(6) <= scrout(5) XOR scrout(6);
    nrzi2(7) <= scrout(5) XOR scrout(6) XOR scrout(7);
    nrzi2(8) <= scrout(5) XOR scrout(6) XOR scrout(7) 
                XOR scrout(8);
    nrzi2(9) <= scrout(5) XOR scrout(6) XOR scrout(7) 
                XOR scrout(8) XOR scrout(9);

    -- output minus one stage register
    nrzi1(0 TO 2) <= nrzi2(0 TO 2);
    nrzi1(3) <= nrzi2(2) XOR nrzi2(3);
    nrzi1(4) <= nrzi2(2) XOR nrzi2(3) XOR nrzi2(4);
    nrzi1(5) <= nrzi2(2) XOR nrzi2(3) XOR nrzi2(4) XOR nrzi2(5);
    nrzi1(6) <= nrzi2(2) XOR nrzi2(3) XOR nrzi2(4) XOR nrzi2(6);
    nrzi1(7) <= nrzi2(2) XOR nrzi2(3) XOR nrzi2(4) XOR nrzi2(7);
    nrzi1(8) <= nrzi2(2) XOR nrzi2(3) XOR nrzi2(4) XOR nrzi2(8);
    nrzi1(9) <= nrzi2(2) XOR nrzi2(3) XOR nrzi2(4) XOR nrzi2(9);

END PROCESS nrzi_enc;

--------------------------------------------------------------------
--------------------------------------------------------------------
-- assign the output register to the output pins of the PLD
-- the mapping is out of order because the inp and scrData signals
-- were numbered based on the scrambler shifter bit numbers 
-- instead of the LSB/MSB data position

-- declare output register
-- input selection is either NRZI scrambled data or data from iregB
outreg: PROCESS BEGIN
    WAIT UNTIL txclk = '1';
    IF DVB_enl = '0' THEN
        -- deliver latched unencoded DVB data to the output register
        dout(1 TO 8) <= iregA(1 TO 8);
        -- combine the enable signals
        ena <= ENA1 AND ENN2;

        IF (svsen = '0') THEN
            dout(9) <= '0';
        ELSE 
            dout(9) <= iregA(9);
        END IF;

        IF (scden = '0') THEN
            dout(0) <= '0';
        ELSE 
            dout(0) <= iregA(0);
        END IF;

    ELSIF bypassl = '1' THEN
        -- send latched unscrambled data to HOTLink output
        dout <= iregB;
        -- enable HOTLink TX to capture data at every clock
        ena <= '0';
    ELSE
        dout(0) <= dout(9) XOR nrzi1(0);
        dout(1) <= ing;
        dout(2) <= ing XOR nrzi1(2);
        dout(3) <= ing XOR nrzi1(3);
        dout(4) <= ing XOR nrzi1(4);
        dout(5) <= ing XOR nrzi1(5);
        dout(6) <= ing XOR nrzi1(6);
        dout(7) <= ing XOR nrzi1(7);
        dout(8) <= ing XOR nrzi1(8);
        dout(9) <= ing XOR nrzi1(9);
        -- enable HOTLink TX to capture data at every clock
        ena <= '0';
    END IF;

END PROCESS outreg;

ing <= dout(9) XOR nrzi1(0) XOR nrzi1(1);

---------------------------------------------------------------------
-- add in tristate control of all outputs for board testability

ena_out <= ena WHEN (oe = '1') ELSE 'Z';
trs_out <= trs WHEN (oe = '1') ELSE 'Z';
data_out <= dout WHEN (oe = '1') ELSE "ZZZZZZZZZZ";

END archscram;



