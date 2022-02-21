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

-- CY7C9335 SMPTE Decoder/Framer Design

-- This design combines an NRZI decoder, SMPTE descrabler, parallel
-- framer, and DVB-ASI handler (primarily a bypass interface).

-- The NRZI decoder works just the opposite of the encoder.  Instead
-- of using a feedback process, this uses a feedforward process to
-- remove the effect of the encoder.  This should allow the decoder
-- to be implemented with only a minimal number of XOR terms; one
-- per bit implemented in parallel.  Since ten bits are handled in
-- each clock, it should take 10 XOR gates.

-- First determine the shift sequence.  The 10 input bits are b[0:9]
-- as capturered in the input register x[0:9].  Register q[0:9] holds
-- the previously processed ten bits.  The equations for q[0:9] are 
-- determined by calculating the equivalent bits after each shift.  
-- Following ten shifts, the equations will describe the hardware 
-- necessary.

-- Original                 after one shift clock these registers equal
-- x9 = b9   q9 = a9        x9 = c0   q9 = b0+b1
-- x8 = b8   q8 = a8        x8 = b9   q8 = a9
-- x7 = b7   q7 = a7        x7 = b8   q7 = a8
-- x6 = b6   q6 = a6        x6 = b7   q6 = a7
-- x5 = b5   q5 = a5        x5 = b6   q5 = a6
-- x4 = b4   q4 = a4        x4 = b5   q4 = a5
-- x3 = b3   q3 = a3        x3 = b4   q3 = a4
-- x2 = b2   q2 = a2        x2 = b3   q2 = a3
-- x1 = b1   q1 = a1        x1 = b2   q1 = a2
-- x0 = b0   q0 = a0        x0 = b1   q0 = a1


-- after two shift clocks   after nine shift clocks
-- these registers equal    these registers equal
-- x9 = c1   q9 = b1+b2     x9 = c8   q9 = b8+b9
-- x8 = c0   q8 = b0+b1     x8 = c7   q8 = b7+b8
-- x7 = b9   q7 = a9        x7 = c6   q7 = b6+b7
-- x6 = b8   q6 = a8        x6 = c5   q6 = b5+b6
-- x5 = b7   q5 = a7        x5 = c4   q5 = b4+b5
-- x4 = b6   q4 = a6        x4 = c3   q4 = b3+b4
-- x3 = b5   q3 = a5        x3 = c2   q3 = b2+b3
-- x2 = b4   q2 = a4        x2 = c1   q2 = b1+b2
-- x1 = b3   q1 = a3        x1 = c0   q1 = b0+b1
-- x0 = b2   q0 = a2        x0 = b9   q0 = a9


-- after 10 shift clocks these registers equal
-- x9 = c9   q9 = b9+c0   MSB
-- x8 = c8   q8 = b8+b9
-- x7 = c7   q7 = b7+b8
-- x6 = c6   q6 = b6+b7
-- x5 = c5   q5 = b5+b6
-- x4 = c4   q4 = b4+b5
-- x3 = c3   q3 = b3+b4
-- x2 = c2   q2 = b2+b3
-- x1 = c1   q1 = b1+b2
-- x0 = c0   q0 = b0+b1   LSB

-- in reality, all that is needed is 11 bits of data to decode
-- 10 bits in a single clock.  This can be done by keeping one
-- bit from the previous clock cycle to extend the register, 
-- then decoding q0-q9 from the 11 bit register

-- When accepting data from the HOTLink RX, bit Q(a)is the LSB
-- of the received word and is received first.  The descramber
-- requires that the bits be routed through the circuis LSB first. 

-- The descrambler uses a feed-forward form of the x^9 + X^4 + 1 
-- SMPTE scrambler algorithm to unscramble the data stream.

-- The feed-forward descrambler needs to descramble 10 bits every
-- clock cycle.  Since the scrambler code only spans 9 bits, 

-- The initial conditions have the descrambler register D1[0:9] containing
-- the values x[0:9] as a present condition shown here.

-- D1(9) = x9   -- MSB
-- D1(8) = x8 
-- D1(7) = x7
-- D1(6) = x6
-- D1(5) = x5   -- feed-forward tap
-- D1(4) = x4
-- D1(3) = x3
-- D1(2) = x2
-- D1(1) = x1
-- D1(0) = x0   -- LSB 

-- As the first non-descrambled bit d0 is clocked in, the register contents
-- would change to 

-- D1(9) = d0   -- MSB
-- D1(8) = x9 
-- D1(7) = x8
-- D1(6) = x7
-- D1(5) = x6   -- feed-forward tap
-- D1(4) = x5
-- D1(3) = x4
-- D1(2) = x3
-- D1(1) = x2
-- D1(0) = x1 + d0 + x5   -- LSB 

-- If another bit were clocked into the descrambler at this time, the
-- descrambled bit in D1(0) would be shifted out and lost.  To make sure 
-- that no data is lost, the descrambler must be extended by nine bits in a
-- second register

-- D1(9) = d1   -- MSB
-- D1(8) = d0 
-- D1(7) = x9
-- D1(6) = x8
-- D1(5) = x7   -- feed-forward tap
-- D1(4) = x6
-- D1(3) = x5
-- D1(2) = x4
-- D1(1) = x3
-- D1(0) = x2 + d1 + x6 -- LSB 

-- D2(8) = x1 + d0 + x5 -- MSB    
-- D2(7) = x0
-- D2(6) = y8
-- D2(5) = y7
-- D2(4) = y6
-- D2(3) = y5
-- D2(2) = y4
-- D2(1) = y3
-- D2(0) = y2


-- After a third shift the descrambler contents are
-- D1(9) = d2   -- MSB
-- D1(8) = d1 
-- D1(7) = d0
-- D1(6) = x9
-- D1(5) = x8   -- feed-forward tap
-- D1(4) = x7
-- D1(3) = x6
-- D1(2) = x5
-- D1(1) = x4
-- D1(0) = x3 + d2 + x7 -- LSB 

-- D2(8) = x2 + d1 + x6 -- MSB    
-- D2(7) = x1 + d0 + x5
-- D2(6) = x0
-- D2(5) = y8
-- D2(4) = y7
-- D2(3) = y6
-- D2(2) = y5
-- D2(1) = y4
-- D2(0) = y3

-- At the sixth shift, all the previous bits have passed the feed-forward
-- tap and are now just encoding data bits.
-- D1(9) = d5   -- MSB
-- D1(8) = d4 
-- D1(7) = d3
-- D1(6) = d2
-- D1(5) = d1   -- feed-forward tap
-- D1(4) = d0
-- D1(3) = x9
-- D1(2) = x8
-- D1(1) = x7
-- D1(0) = x6 + d5 + d0 -- LSB 

-- D2(8) = x5 + d4 + x9 -- MSB    
-- D2(7) = x4 + d3 + x8
-- D2(6) = x3 + d2 + x7
-- D2(5) = x2 + d1 + x6
-- D2(4) = x1 + d0 + x5
-- D2(3) = x0
-- D2(2) = y8
-- D2(1) = y7
-- D2(0) = y6

-- After the full 10 bits are descrambled, the equations are
-- D1(9) = d9   -- MSB
-- D1(8) = d8 
-- D1(7) = d7
-- D1(6) = d6
-- D1(5) = d5   -- feed-forward tap
-- D1(4) = d4
-- D1(3) = d3
-- D1(2) = d2
-- D1(1) = d1
-- D1(0) = d0 + d9 + d4 -- LSB 

-- D2(8) = x9 + d8 + d3 -- MSB    
-- D2(7) = x8 + d7 + d2
-- D2(6) = x7 + d6 + d1
-- D2(5) = x6 + d5 + d0
-- D2(4) = x5 + d4 + x9
-- D2(3) = x4 + d3 + x8
-- D2(2) = x3 + d2 + x7
-- D2(1) = x2 + d1 + x6
-- D2(0) = x1 + d0 + x5 -- LSB

-- If we now look at the low order 10 bits of this 19 bit register,
-- we can see the equations for the descrambled output character.
-- This character would span from D1(0) as the MSB to D2(0) as the LSB.


 
LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY dscramrx IS 
    PORT (
        rxclk,                      -- CY7B933 CKR recovered clock
		reset_n,
        DVB_select: IN std_logic;   -- select 8B/10B mode (active LOW)
        bypass: IN std_logic;       -- raw data mode, bypass scrambler data
        SYNC_en: IN std_logic;      -- TRS/SYNC correction enabled
        oe: IN std_logic;           -- tristate output enable
        data_in: IN std_logic_vector (0 TO 9);
                                    -- scrambled data from HOTLink RX
        data_out: BUFFER std_logic_vector (0 TO 9);
                                    -- descrambled and framed data
        SYNC_error: out std_logic;   -- SYNC offset error pulse
        RF: out std_logic;       -- HOTLink RX frame control pin
        AB: out std_logic;       -- HOTLink A/B port select pin
        H_SYNC: out std_logic);  -- horizontal sync output toggle


END dscramrx;

ARCHITECTURE structural OF dscramrx IS
COMPONENT port_sel 
	PORT (
        rxclk,                  -- Receiver clock
        enable: IN std_logic;   -- enable port selector
        reg_data: IN std_logic_vector(0 TO 9);
                                -- latched HOTLink RX data bus
        A_B: BUFFER std_logic   -- A_B port select
        );
END COMPONENT;

SIGNAL in_reg: std_logic_vector (0 TO 10);      -- NRZI/input register
SIGNAL nrz_reg: std_logic_vector (0 TO 9);      -- NRZ pipeline register
SIGNAL din: std_logic_vector (0 TO 9);          -- descrambler equations
SIGNAL d0: std_logic_vector (0 TO 9);           -- low order descrambler register
SIGNAL d1: std_logic_vector (1 TO 9);           -- high order descrambler register
SIGNAL p1: std_logic_vector (0 TO 9);           -- first pipeline stage
SIGNAL p2: std_logic_vector (0 TO 9);           -- second pipeline stage
SIGNAL p3: std_logic_vector (0 TO 9);           -- third pipeline stage
SIGNAL p4: std_logic_vector (0 TO 9);           -- fourth pipeline stage
SIGNAL p5: std_logic_vector (0 TO 9);           -- fifth pipeline stage
SIGNAL dmux: std_logic_vector (0 TO 9);         -- descrambled and framed
SIGNAL dout: std_logic_vector (0 TO 9);         -- output data register
SIGNAL SYNC: std_logic;                         -- horizontal sync toggle
-- This signal changes state one clock prior to the first word of
--  the TRS being presented at the outputs.

SIGNAL offset1: std_logic_vector (3 DOWNTO 0);  -- four-bit offset pointer
SIGNAL offset2: std_logic_vector (3 DOWNTO 0);  -- first pipelined offset
SIGNAL offset3: std_logic_vector (3 DOWNTO 0);  -- second pipelined offset
SIGNAL offset4: std_logic_vector (3 DOWNTO 0);  -- fourth pipelined offset
SIGNAL cmp_v: std_logic_vector (0 TO 18);       -- compare vector
SIGNAL cmp_i: std_logic_vector (0 TO 18);       -- XORed compare
SIGNAL cmp_x1: std_logic_vector (0 TO 18);      -- compare don't cares for 1's
SIGNAL cmp_x0: std_logic_vector (0 TO 9);       -- compare don't cares for 0's
SIGNAL cmp_m1: std_logic_vector (0 TO 18);      -- match vector
SIGNAL cmp_m0: std_logic_vector (0 TO 9);       -- match vector
SIGNAL match1: std_logic;                       -- match signal for 1's compare
SIGNAL match0: std_logic;                       -- match signal for 0's compare
SIGNAL no_match: std_logic;                     -- create factor point
SIGNAL SYNC_flag: std_logic;                    -- flag for TRS offset update
SIGNAL SYNC_err: std_logic;                     -- SYNC detected in wrong position
SIGNAL A_B: std_logic;                          -- A/B port selector signal
SIGNAL portsel_en: std_logic;                   -- enable A/B port selector

BEGIN

-- First declare the input register.  This is an 11 bit register
proc1: PROCESS BEGIN
    WAIT UNTIL rxclk = '1';
        -- bit 9 is the first bit received by the HOTLink receiver
        in_reg(0) <= in_reg(10);    -- extend to an 11 bit register
        in_reg(1 TO 10) <= data_in;    -- capture input bus
END PROCESS proc1;

----------------------------------------------------------------------
----------------------------------------------------------------------
-- Next item is the NRZI to NRZ decoder.  Since 11 bits of data are
-- present in the input register, it is possible to decode a full
-- 10 bits in one clock
proc2: PROCESS BEGIN
    WAIT UNTIL rxclk = '1';
        nrz_reg(9) <= in_reg(10) XOR in_reg(9); -- MSB
        nrz_reg(8) <= in_reg(9) XOR in_reg(8);
        nrz_reg(7) <= in_reg(8) XOR in_reg(7);
        nrz_reg(6) <= in_reg(7) XOR in_reg(6);
        nrz_reg(5) <= in_reg(6) XOR in_reg(5);
        nrz_reg(4) <= in_reg(5) XOR in_reg(4);
        nrz_reg(3) <= in_reg(4) XOR in_reg(3);
        nrz_reg(2) <= in_reg(3) XOR in_reg(2);
        nrz_reg(1) <= in_reg(2) XOR in_reg(1);
        nrz_reg(0) <= in_reg(1) XOR in_reg(0);  -- LSB
END PROCESS proc2;

----------------------------------------------------------------------
----------------------------------------------------------------------
-- Next comes the descrambler.  This descrambler uses the 
-- same X^9 + x^4 + 1 polynomial as the scrambler.  These equations 
-- are assigned to the respective bits of register desc.

-- descrambler equations
    din(9) <= nrz_reg(0) XOR nrz_reg(9) XOR nrz_reg(4); -- MSB
    din(8) <= d0(9) XOR nrz_reg(8) XOR nrz_reg(3);
    din(7) <= d0(8) XOR nrz_reg(7) XOR nrz_reg(2);
    din(6) <= d0(7) XOR nrz_reg(6) XOR nrz_reg(1);
    din(5) <= d0(6) XOR nrz_reg(5) XOR nrz_reg(0);
    din(4) <= d0(5) XOR nrz_reg(4) XOR d0(9);
    din(3) <= d0(4) XOR nrz_reg(3) XOR d0(8);
    din(2) <= d0(3) XOR nrz_reg(2) XOR d0(7);
    din(1) <= d0(2) XOR nrz_reg(1) XOR d0(6);   
    din(0) <= d0(1) XOR nrz_reg(0) XOR d0(5);           -- LSB

regIntFF: PROCESS BEGIN
    -- on rising CKR clock capture data from NRZI decoder into
    -- the first register of the descrambler
    WAIT UNTIL rxclk = '1';
        -- descramble data
            d0(9) <= nrz_reg(9);    
            d0(8) <= nrz_reg(8);
            d0(7) <= nrz_reg(7);      
            d0(6) <= nrz_reg(6);    
            d0(5) <= nrz_reg(5);
            d0(4) <= nrz_reg(4);      
            d0(3) <= nrz_reg(3);    
            d0(2) <= nrz_reg(2);
            d0(1) <= nrz_reg(1);
END PROCESS regIntFF;

dataIntFF: PROCESS BEGIN
    -- setup descrambler bypass register
    WAIT UNTIL rxclk = '1';
        IF bypass = '1' THEN    -- if bypass is active
            -- then route data around the NRZI decoder and descrambler
            d0(0) <= in_reg(10);
            d1(1 TO 9) <= in_reg(1 TO 9); 

        ELSE
            d0(0) <= din(9);    -- MSB      

            d1(9) <= din(8);        
            d1(8) <= din(7);        
            d1(7) <= din(6);
            d1(6) <= din(5);        
            d1(5) <= din(4);        
            d1(4) <= din(3);
            d1(3) <= din(2);        
            d1(2) <= din(1);        
            d1(1) <= din(0);    -- LSB
        END IF;
END PROCESS dataIntFF;

-- rename outputs from scrambler registers.  This is the first stage of
-- fully descrambled data.  It has not yet been framed.
        p1(9) <= d0(0);
        p1(0 TO 8) <= d1(1 TO 9);


----------------------------------------------------------------------
----------------------------------------------------------------------
-- add in pipeline registers to allow data to be examined and shifted

pipes: PROCESS BEGIN
    WAIT UNTIL rxclk = '1';
        IF (DVB_select = '0') THEN
--            -- if DVB-ASI mode is selected, then route data direct to output
--            dout(0 TO 7) <= in_reg(2 TO 9); -- data bits are shifted to 
--                                            -- lower part of the bus
--            dout(9) <= in_reg(10);          -- route the RVS signal
--            dout(8) <= in_reg(1);           -- route the SC/D signal  
             dout <= in_reg(1 TO 10);         -- route bits straight through
        ELSE
            dout <= dmux;    -- route descrambled and framed data to output
        END IF;
        p5 <= p4;    
        p4 <= p3;
        p3 <= p2;
        p2 <= p1;
END PROCESS pipes;


----------------------------------------------------------------------
----------------------------------------------------------------------
-- next step is the framer.  It must allow a frame operation on 
-- every clock cycle.  The first thing to look for is the first '01'
-- combination in register p1/p2. 

-- The parallel framer will be looking for the 30 bit
-- sequence of bits 3ff, 000, 000.  This pattern can arrive
-- at the framer in any of 10 possible framing offsets,
-- from 0 to 9 bits.  This allows a 4-bit field to be used
-- to contain the framer offset field.  This field can then
-- be used to control a barrel shifter connected to a
-- later pipeline stage in the design, just
-- prior to the output register.

-- These registers could contain the data in the following 
-- positions:

--     T4         T3         T2         T1
-- 9876543210 9876543210 9876543210 9876543210 
-- xxxxxxxxxx 0000000000 0000000000 1111111111 = 0 offset
-- xxxxxxxxx0 0000000000 0000000001 111111111x = 9 bit offset
-- xxxxxxxx00 0000000000 0000000011 11111111xx = 8 bits offset
-- xxxxxxx000 0000000000 0000000111 1111111xxx = 7 bits offset 
-- xxxxxx0000 0000000000 0000001111 111111xxxx = 6 bits offset
-- xxxxx00000 0000000000 0000011111 11111xxxxx = 5 bits offset
-- xxxx000000 0000000000 0000111111 1111xxxxxx = 4 bits offset
-- xxx0000000 0000000000 0001111111 111xxxxxxx = 3 bits offset
-- xx00000000 0000000000 0011111111 11xxxxxxxx = 2 bits offset
-- x000000000 0000000000 0111111111 1xxxxxxxxx = 1 bits offset
-- 0000000000 0000000000 1111111111 xxxxxxxxxx = no match

-- Note that all possible combinations contain a check for all 0's
-- in the T3 time slot.  The first check requires a detection of the
-- first 1 in the p1 pipeline register. 

-- This point would be captured as an offset, and used for
-- a 10-bit compare in the next clock cycle.

-- The offset point is determined by a sequential IF/ELSIF test
-- to create a priority encoder.
pri_enc: PROCESS (p1) BEGIN
    IF p1(9) = '1' THEN
      offset1 <= "1111";        -- no match
      ELSE IF p1(8) = '1' THEN
        offset1 <= "0001";        -- set offset1 to 1
        ELSE IF p1(7) = '1' THEN
          offset1 <= "0010";        -- set offset1 to 2
          ELSE IF p1(6) = '1' THEN
            offset1 <= "0011";        -- set offset1 to 3
            ELSE IF p1(5) = '1' THEN
              offset1 <= "0100";        -- set offset1 to 4
              ELSE IF p1(4) = '1' THEN
                offset1 <= "0101";        -- set offset1 to 5
                ELSE IF p1(3) = '1' THEN
                  offset1 <= "0110";        -- set offset1 to 6
                  ELSE IF p1(2) = '1' THEN
                    offset1 <= "0111";        -- set offset1 to 7
                    ELSE IF p1(1) = '1' THEN
                      offset1 <= "1000";        -- set offset1 to 8
                      ELSE IF p1(0) = '1' THEN
                        offset1 <= "1001";        -- set offset1 to 9
                      ELSE
                        offset1 <= "0000";        -- no offset = all zeros
                      END IF;
                    END IF;
                  END IF;
                END IF;
              END IF;
            END IF;
          END IF;
        END IF;
      END IF;
    END IF;
END PROCESS pri_enc;

-- create logic factoring point
no_match <= '1' WHEN (offset3 /= offset4) ELSE '0';

-- capture offset state
offs1: PROCESS (rxclk,reset_n) BEGIN
	if reset_n = '0' then
	SYNC <= '0';
	elsif rxclk'event and rxclk = '1' then
--    WAIT UNTIL rxclk = '1';     -- synchronous process

        offset2 <= offset1;     -- capture the offset1 decode

        -- Next check to see if a first field match exists
        IF (match1 = '1' AND offset1 = "0000") THEN    
        -- if the first field contains the 1's match, and the 
        -- second field is all zeros, then keep offset pointer
            offset3 <= offset2;
        ELSE
        -- otherwise clear the offset pointer
            offset3 <= "1111";
        END IF;

        -- Lastly, see if a third field match exists
        IF (offset3 /= "1111" AND match0 = '1') THEN
            -- TRS detected, check if update necessary
            IF ((SYNC_en = '1') AND (no_match = '1') AND (SYNC_flag = '0')) THEN
                -- if correction is enabled, then check flag status
                -- only set SYNC_flag
                offset4 <= offset4; -- maintain present offset
                SYNC_flag <= '1';   -- TRS detected in error
                SYNC_err <= '1';    -- set SYNC error flag
            ELSE
                offset4 <= offset3;
                SYNC_flag <= '0';       -- clear flag on update
                SYNC_err <= '0';        -- clear SYNC error pulse
            END IF; 
            SYNC <= NOT SYNC;       -- toggle SYNC output on all TRS fields
        ELSE    
            -- no TRS found, maintain state
            offset4 <= offset4;
            SYNC <= SYNC;
            SYNC_flag <= SYNC_flag;
            SYNC_err <= '0';        -- clear SYNC error pulse
        END IF;
	end if;
END PROCESS offs1;

-- the offset2 values are used to set the don't care mask for the 
-- 1's compare operation
WITH offset2 SELECT
    cmp_x1 <=   "0000000000111111111" WHEN "0000",
                "1000000000011111111" WHEN "1001",
                "1100000000001111111" WHEN "1000",
                "1110000000000111111" WHEN "0111",
                "1111000000000011111" WHEN "0110",
                "1111100000000001111" WHEN "0101",
                "1111110000000000111" WHEN "0100",
                "1111111000000000011" WHEN "0011",
                "1111111100000000001" WHEN "0010",
                "1111111110000000000" WHEN "0001",
                "-------------------" WHEN OTHERS;

-- the offset3 values are used to set the don't care mask for the
-- 0's compare operation
WITH offset3 SELECT
    cmp_x0 <=   "0000000000" WHEN "0000",
                "1111111110" WHEN "0001",
                "1111111100" WHEN "0010",
                "1111111000" WHEN "0011",
                "1111110000" WHEN "0100",
                "1111100000" WHEN "0101",
                "1111000000" WHEN "0110",
                "1110000000" WHEN "0111",
                "1100000000" WHEN "1000",
                "1000000000" WHEN "1001",
                "----------" WHEN OTHERS;
            

----------------------------------------------------------------------
----------------------------------------------------------------------
-- Next is the compare block.  This needs to look at the contents of 
-- the data stream for three consecutive clocks, and look for the
-- 3FF, 000, 000 data pattern.  To simplify this, I'll actually do
-- a 19-bit comparator using XORs and Don't Cares to allow it to
-- look for only the bits in question.  Each bit of the comparator
-- needs to be able to match a 1, a 0, or an X.  This reduces to 
-- an XOR, feeding an OR for each bit, all combined into a 19-wide
-- AND.

-- The framer needs to do compares for 1s across a 19 bit path.  This 
-- path is created by concatenation of parts of the p2 and p3 registers 
-- for the first (1's) compare
cmp_v <= p3 & p2(0 TO 8);

-- next mask out the don't care bits
cmp_m1 <= cmp_x1 OR cmp_v;

-- now check for all bits match
match1 <= '1' WHEN (cmp_m1 = "1111111111111111111") ELSE '0';

-- Looking for next field being zero's is accomplished by the offset1
-- logic on p1.

-- The final zeros field check is another masked check that only looks
-- at the p1 regsiter.
cmp_m0 <= p1 AND cmp_x0;

-- Now check for all bits match
match0 <= '1' WHEN (cmp_m0 = "0000000000") ELSE '0';


----------------------------------------------------------------------
----------------------------------------------------------------------
-- last major item is the output barrel shifter

WITH offset4 SELECT
        dmux <= p5                      WHEN "0000",
            (p5(1 TO 9) & p4(0))      WHEN "1001",
            (p5(2 TO 9) & p4(0 TO 1)) WHEN "1000",
            (p5(3 TO 9) & p4(0 TO 2)) WHEN "0111",
            (p5(4 TO 9) & p4(0 TO 3)) WHEN "0110",
            (p5(5 TO 9) & p4(0 TO 4)) WHEN "0101",
            (p5(6 TO 9) & p4(0 TO 5)) WHEN "0100",
            (p5(7 TO 9) & p4(0 TO 6)) WHEN "0011",
            (p5(8 TO 9) & p4(0 TO 7)) WHEN "0010",
            (p5(9)      & p4(0 TO 8)) WHEN "0001",
            ("----------")            WHEN OTHERS;

----------------------------------------------------------------------
----------------------------------------------------------------------
-- declare A/B port selector state machine

portsel_en <= NOT DVB_select;           -- 

ABsel: port_sel PORT MAP (      -- 
        rxclk,                  -- (I) CKR clock
        portsel_en,             -- (I) enable
        in_reg(1 TO 10),        -- (I) registered data bus
        A_B);                   -- (O) port select signal


----------------------------------------------------------------------
----------------------------------------------------------------------
-- add in tristate control of all outputs for board testability

data_out <= dout WHEN (oe = '1') ELSE "ZZZZZZZZZZ";
H_SYNC <= SYNC;-- WHEN (oe = '1') ELSE 'Z';
RF <= (NOT DVB_select) WHEN (oe = '1') ELSE 'Z';
SYNC_error <= SYNC_err WHEN (oe = '1') ELSE 'Z';
AB <= A_B WHEN (oe = '1') ELSE 'Z';  -- 

END structural;

