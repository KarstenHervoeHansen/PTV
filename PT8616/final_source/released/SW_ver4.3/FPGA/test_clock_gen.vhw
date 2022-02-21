--------------------------------------------------------------------------------
-- Copyright (c) 1995-2007 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 9.2.03i
--  \   \         Application : ISE
--  /   /         Filename : test_clock_gen.vhw
-- /___/   /\     Timestamp : Tue Nov 13 14:11:55 2007
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: test_clock_gen
--Device: Xilinx
--

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
USE IEEE.STD_LOGIC_TEXTIO.ALL;
USE STD.TEXTIO.ALL;

ENTITY test_clock_gen IS
END test_clock_gen;

ARCHITECTURE testbench_arch OF test_clock_gen IS
    FILE RESULTS: TEXT OPEN WRITE_MODE IS "results.txt";

    COMPONENT LTC_clockgen
        PORT (
            clk_i : In std_logic;
            sync_i : In std_logic;
            format_i : In std_logic_vector (1 DownTo 0);
            ltc_clock_tick_o : Out std_logic;
            ltc_frame_tick_o : Out std_logic;
            ltc_bitcount_o : Out std_logic_vector (7 DownTo 0)
        );
    END COMPONENT;

    SIGNAL clk_i : std_logic := '0';
    SIGNAL sync_i : std_logic := '0';
    SIGNAL format_i : std_logic_vector (1 DownTo 0) := "00";
    SIGNAL ltc_clock_tick_o : std_logic := '0';
    SIGNAL ltc_frame_tick_o : std_logic := '0';
    SIGNAL ltc_bitcount_o : std_logic_vector (7 DownTo 0) := "00000000";

    constant PERIOD : time := 200 ns;
    constant DUTY_CYCLE : real := 0.5;
    constant OFFSET : time := 100 ns;

    BEGIN
        UUT : LTC_clockgen
        PORT MAP (
            clk_i => clk_i,
            sync_i => sync_i,
            format_i => format_i,
            ltc_clock_tick_o => ltc_clock_tick_o,
            ltc_frame_tick_o => ltc_frame_tick_o,
            ltc_bitcount_o => ltc_bitcount_o
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
                -- -------------  Current Time:  585ns
                WAIT FOR 585 ns;
                sync_i <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  785ns
                WAIT FOR 200 ns;
                sync_i <= '0';
                -- -------------------------------------
                WAIT FOR 415 ns;

            END PROCESS;

    END testbench_arch;

