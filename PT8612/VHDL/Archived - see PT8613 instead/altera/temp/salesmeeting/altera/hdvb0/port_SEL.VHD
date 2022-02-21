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

-- A/B DVB-ASI Port Selector

--
-- data-stream inversion state machine for DVB-ASI interfaces
--
-- This machine has a five state supervisor machine that tracks
-- the number of errors detected within a specific period of time.
-- It also tracks valid characters and SYNC codes.
-- If too many errors are detected in a small period of time, it
-- is assumed that the data stream is inverted.  The A/B select is
-- then toggled and the machine then waits for a consecutive pair
-- of SYNC codes to be received before starting to check characters
-- again.

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


PACKAGE port_select IS
        COMPONENT port_sel PORT (
        rxclk,                  -- Receiver clock
        enable: IN std_logic;   -- enable port selector
        reg_data: IN std_logic_vector(0 TO 9);
                                -- latched HOTLink RX data bus
        A_B: BUFFER std_logic   -- A/B port select
        );
        END COMPONENT;
END port_select;

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;

ENTITY port_sel IS PORT (
        rxclk,                  -- Receiver clock
        enable: IN std_logic;   -- enable port selector
        reg_data: IN std_logic_vector(0 TO 9);
                                -- latched HOTLink RX data bus
        A_B: BUFFER std_logic   -- A_B port select
        );
END port_sel;

--USE work.std_arith.all;         -- used with counter

ARCHITECTURE arch1 OF port_sel IS

-- declare internal signals
SIGNAL ctr_en: std_logic;             -- counter enable
SIGNAL ctr_reset: std_logic;          -- counter reset
SIGNAL sync: std_logic;               -- K28.5 detected
SIGNAL cnt: std_logic_vector(3 DOWNTO 0);     -- 4-bit counter vector

-- declare state machine
TYPE sync_state IS (
        state0,                 -- wait for SYNC codes
        state1,                 -- look for second sync
        state2,                 -- no errors, in sync
        state3,                 -- 1 error, in sync
        state4,                 -- 2 errors, in sync
        state5,                 -- 3 errors, in sync
        state6                  -- toggle A/B
        );
-- declare state machine encoding, state variable, and initial state
--ATTRIBUTE state_encoding OF sync_state:TYPE IS one_hot_zero;
SIGNAL s_state : sync_state := state0;

BEGIN

sync <= '1' WHEN ((reg_data(0) = '1') AND  -- command character
                  ((reg_data(1 TO 4) = "1010") OR --K28.5   
                   ((reg_data(6 TO 8) = "111") AND  -- error character
                    (reg_data(1) = '1' OR reg_data(2) = '1'))))
            ELSE '0';



-- re-code the controlling machine to add the "enable" term into the
-- middle of the case statement            
proc1:  PROCESS BEGIN
        WAIT UNTIL (rxclk='1');
        CASE s_state IS
            WHEN state0 =>
                IF (enable = '0') THEN  -- if no enable, reset the
                                        -- state machine
                    s_state <= state0;
                ELSE
                    -- look for SYNC codes
                    -- the counter is reset in this state 
                    IF (sync = '1') THEN
                        -- if SYNC detected, then look for 2nd SYNC
                        s_state <= state1;
                    ELSE    -- else maintain currect state
                        s_state <= state0;
                    END IF;
                END IF;
            WHEN state1 =>
                IF (enable = '0') THEN  -- if no enable, reset the
                                        -- state machine
                    s_state <= state0;
                ELSE
                    -- SYNC has been detected, now look for 2nd SYNC
                    -- meeting the rules for the multi-byte framer.
                    -- counter is enabled
                    IF (sync = '1') THEN
                        s_state <= state2;
                    ELSIF (cnt = "0011") THEN
                        -- if too many bytes without SYNC then 
                        -- start looking again
                        s_state <= state0;    
                    ELSE
                        s_state <= state1;
                    END IF;
                END IF;
            WHEN state2 =>
                IF (enable = '0') THEN  -- if no enable, reset the
                                        -- state machine
                    s_state <= state0;
                ELSE
                    -- syncronization found, now look for errors
                    IF (reg_data(9)='1') THEN
                        -- if error detected, then look for more
                        s_state <= state3;
                    ELSE    -- wait and monitor data stream
                        s_state <= state2;
                    END IF;
                END IF;
            WHEN state3 =>
                IF (enable = '0') THEN  -- if no enable, reset the
                                        -- state machine
                    s_state <= state0;
                ELSE
                    -- one error detected, look for more
                    IF (reg_data(9)='1') THEN
                        -- if second error then look for third
                        s_state <= state4;
                    ELSIF  (cnt="1111") THEN
                        -- if 16 bytes without error then return
                        -- to wait state
                        s_state <= state2;
                    ELSE    -- increment count
                        s_state <= state3;
                    END IF;
                END IF;
            WHEN state4 =>
                IF (enable = '0') THEN  -- if no enable, reset the
                                        -- state machine
                    s_state <= state0;
                ELSE
                    -- second error detected, look for third
                    IF (reg_data(9)='1') THEN
                        -- if third error then look for fourth
                        s_state <= state5;
                    ELSIF  (cnt="1111") THEN
                        -- if 16 bytes without error then return
                        -- to previous state
                        s_state <= state3;
                    ELSE    -- increment count
                        s_state <= state4;
                    END IF;
                END IF;
            WHEN state5 =>
                IF (enable = '0') THEN  -- if no enable, reset the
                                        -- state machine
                    s_state <= state0;
                ELSE
                    -- third error detected, look for fourth
                    IF (reg_data(9)='1') THEN
                        -- if fourth error, then toggle port
                        s_state <= state6;
                    ELSIF  (cnt="1111") THEN
                        -- if 16 bytes without error then return
                        -- to previous state
                        s_state <= state4;
                    ELSE    -- increment counter
                        s_state <= state5;
                    END IF;
                END IF;
            WHEN state6 =>
                -- too many errors or machine disabled, invert data stream
                s_state <= state0;  -- return to neutral state
            WHEN others =>
                s_state <= state0;
        END CASE;
END PROCESS proc1;

-- build 4-bit counter with enable and reset

ctr_en <=  '1' WHEN (enable = '1' AND 
                     ((s_state=state1) OR
                      (s_state=state3 AND reg_data(0)='0') OR
                      (s_state=state4 AND reg_data(0)='0') OR
                      (s_state=state5 AND reg_data(0)='0')))
                ELSE '0';


ctr_reset <= '1' WHEN (          (s_state = state0)  OR
                ((reg_data(9) = '1') AND (s_state = state2)) OR
                ((reg_data(9) = '1') AND (s_state = state3)) OR
                ((reg_data(9) = '1') AND (s_state = state4))) 
                ELSE '0';


-- declare counter
upcount: PROCESS BEGIN
        WAIT UNTIL rxclk = '1';
        IF (ctr_reset = '1') THEN
                cnt <= "0000";
        ELSIF (ctr_en = '1') THEN
                cnt <= cnt + 1;
        ELSE
                cnt <= cnt;
        END IF;
END PROCESS upcount;


-- assign output
sel_out: PROCESS BEGIN
        WAIT UNTIL rxclk = '1';
        IF ((s_state = state6) AND (enable = '1')) THEN
                A_B <= NOT A_B;
        ELSE
                A_B <= A_B;
        END IF;
END PROCESS sel_out;

END arch1;

