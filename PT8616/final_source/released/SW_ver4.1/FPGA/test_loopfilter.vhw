--------------------------------------------------------------------------------
-- Copyright (c) 1995-2003 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 9.1.03i
--  \   \         Application : ISE
--  /   /         Filename : test_loopfilter.vhw
-- /___/   /\     Timestamp : Thu Jan 10 13:18:53 2008
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: test_loopfilter
--Device: Xilinx
--

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_SIGNED.ALL;
USE IEEE.STD_LOGIC_TEXTIO.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;
USE STD.TEXTIO.ALL;

ENTITY test_loopfilter IS
END test_loopfilter;

ARCHITECTURE testbench_arch OF test_loopfilter IS
    FILE RESULTS: TEXT OPEN WRITE_MODE IS "results.txt";

    COMPONENT GPS_phase_loop
        PORT (
            clk_i : In std_logic;
            rst_i : In std_logic;
            fix_i : In std_logic;
            fastslow_i : In std_logic;
            phase_diff_i : In std_logic_vector (27 DownTo 0);
            update_tick_i : In std_logic;
            dac_word_o : Out std_logic_vector (15 DownTo 0)
        );
    END COMPONENT;

    SIGNAL clk_i : std_logic := '0';
    SIGNAL rst_i : std_logic := '0';
    SIGNAL fix_i : std_logic := '0';
    SIGNAL fastslow_i : std_logic := '0';
    SIGNAL phase_diff_i : std_logic_vector (27 DownTo 0) := "0000000000000000000000000000";
    SIGNAL update_tick_i : std_logic := '0';
    SIGNAL dac_word_o : std_logic_vector (15 DownTo 0) := "0000000000000000";

    constant PERIOD_clk_i : time := 200 ns;
    constant DUTY_CYCLE_clk_i : real := 0.5;
    constant OFFSET_clk_i : time := 100 ns;
    constant PERIOD_update_tick_i : time := 1000 ns;
    constant DUTY_CYCLE_update_tick_i : real := 0.2;
    constant OFFSET_update_tick_i : time := 100 ns;

    BEGIN
        UUT : GPS_phase_loop
        PORT MAP (
            clk_i => clk_i,
            rst_i => rst_i,
            fix_i => fix_i,
            fastslow_i => fastslow_i,
            phase_diff_i => phase_diff_i,
            update_tick_i => update_tick_i,
            dac_word_o => dac_word_o
        );

        PROCESS    -- clock process for clk_i
        BEGIN
            WAIT for OFFSET_clk_i;
            CLOCK_LOOP : LOOP
                clk_i <= '0';
                WAIT FOR (PERIOD_clk_i - (PERIOD_clk_i * DUTY_CYCLE_clk_i));
                clk_i <= '1';
                WAIT FOR (PERIOD_clk_i * DUTY_CYCLE_clk_i);
            END LOOP CLOCK_LOOP;
        END PROCESS;

        PROCESS    -- clock process for update_tick_i
        BEGIN
            WAIT for OFFSET_update_tick_i;
            CLOCK_LOOP : LOOP
                update_tick_i <= '0';
                WAIT FOR (PERIOD_update_tick_i - (PERIOD_update_tick_i * DUTY_CYCLE_update_tick_i));
                update_tick_i <= '1';
                WAIT FOR (PERIOD_update_tick_i * DUTY_CYCLE_update_tick_i);
            END LOOP CLOCK_LOOP;
        END PROCESS;

        PROCESS    -- Process for clk_i
            BEGIN
                -- -------------  Current Time:  185ns
                WAIT FOR 185 ns;
                rst_i <= '1';
                fastslow_i <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  1185ns
                WAIT FOR 1000 ns;
                rst_i <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  3385ns
                WAIT FOR 2200 ns;
                phase_diff_i <= "1111111111111111111111000000";
                -- -------------------------------------
                -- -------------  Current Time:  9385ns
                WAIT FOR 6000 ns;
                phase_diff_i <= "0000000000000000000000000000";
                -- -------------------------------------
                WAIT FOR 991615 ns;

            END PROCESS;

    END testbench_arch;

