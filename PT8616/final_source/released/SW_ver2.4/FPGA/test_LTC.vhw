--------------------------------------------------------------------------------
-- Copyright (c) 1995-2007 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 9.2.03i
--  \   \         Application : ISE
--  /   /         Filename : test_LTC.vhw
-- /___/   /\     Timestamp : Thu Jan 03 12:04:22 2008
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: test_LTC
--Device: Xilinx
--

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
USE IEEE.STD_LOGIC_TEXTIO.ALL;
USE STD.TEXTIO.ALL;

ENTITY test_LTC IS
END test_LTC;

ARCHITECTURE testbench_arch OF test_LTC IS
    FILE RESULTS: TEXT OPEN WRITE_MODE IS "results.txt";

    COMPONENT LTC_generator
        PORT (
            hours_i : In std_logic_vector (7 DownTo 0);
            mins_i : In std_logic_vector (7 DownTo 0);
            secs_i : In std_logic_vector (7 DownTo 0);
            frames_i : In std_logic_vector (7 DownTo 0);
            delay_i : In std_logic_vector (31 DownTo 0);
            dropframe_i : In std_logic;
            format_select_i : In std_logic_vector (1 DownTo 0);
            sync_i : In std_logic;
            clk_i : In std_logic;
            debug_o : Out std_logic;
            ltc_code_o : Out std_logic
        );
    END COMPONENT;

    SIGNAL hours_i : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL mins_i : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL secs_i : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL frames_i : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL delay_i : std_logic_vector (31 DownTo 0) := "00000000000000000000000000000000";
    SIGNAL dropframe_i : std_logic := '0';
    SIGNAL format_select_i : std_logic_vector (1 DownTo 0) := "00";
    SIGNAL sync_i : std_logic := '0';
    SIGNAL clk_i : std_logic := '0';
    SIGNAL debug_o : std_logic := '0';
    SIGNAL ltc_code_o : std_logic := '0';

    constant PERIOD : time := 200 ns;
    constant DUTY_CYCLE : real := 0.5;
    constant OFFSET : time := 100 ns;

    BEGIN
        UUT : LTC_generator
        PORT MAP (
            hours_i => hours_i,
            mins_i => mins_i,
            secs_i => secs_i,
            frames_i => frames_i,
            delay_i => delay_i,
            dropframe_i => dropframe_i,
            format_select_i => format_select_i,
            sync_i => sync_i,
            clk_i => clk_i,
            debug_o => debug_o,
            ltc_code_o => ltc_code_o
        );

        PROCESS    -- clock process for clk_i
        BEGIN
            WAIT for OFFSET;
            CLOCK_LOOP : LOOP
                clk_i <= '0';
                WAIT FOR (PERIOD - (PERIOD * DUTY_CYCLE));
                clk_i <= '1';
                WAIT FOR (PERIOD * DUTY_CYCLE);
            END LOOP CLOCK_LOOP;
        END PROCESS;

        PROCESS
            BEGIN
                -- -------------  Current Time:  100ns
                WAIT FOR 100 ns;
                secs_i <= "01011001";
                frames_i <= "00100010";
                -- -------------------------------------
                -- -------------  Current Time:  185ns
                WAIT FOR 85 ns;
                sync_i <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  385ns
                WAIT FOR 200 ns;
                sync_i <= '0';
                -- -------------------------------------
                WAIT FOR 815 ns;

            END PROCESS;

    END testbench_arch;

