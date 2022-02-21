--------------------------------------------------------------------------------
-- Copyright (c) 1995-2007 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 9.2.03i
--  \   \         Application : ISE
--  /   /         Filename : test_digital_loop.vhw
-- /___/   /\     Timestamp : Thu Jan 17 13:29:42 2008
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: test_digital_loop
--Device: Xilinx
--

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_SIGNED.ALL;
USE IEEE.STD_LOGIC_TEXTIO.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;
USE STD.TEXTIO.ALL;

ENTITY test_digital_loop IS
END test_digital_loop;

ARCHITECTURE testbench_arch OF test_digital_loop IS
    FILE RESULTS: TEXT OPEN WRITE_MODE IS "results.txt";

    COMPONENT digital_phase_loop
        PORT (
            clk_i : In std_logic;
            phase_diff_i : In std_logic_vector (10 DownTo 0);
            update_tick_i : In std_logic;
            dac_word_o : Out std_logic_vector (15 DownTo 0)
        );
    END COMPONENT;

    SIGNAL clk_i : std_logic := '0';
    SIGNAL phase_diff_i : std_logic_vector (10 DownTo 0) := "00000000000";
    SIGNAL update_tick_i : std_logic := '0';
    SIGNAL dac_word_o : std_logic_vector (15 DownTo 0) := "0000000000000000";

    constant PERIOD_clk_i : time := 200 ns;
    constant DUTY_CYCLE_clk_i : real := 0.5;
    constant OFFSET_clk_i : time := 100 ns;
    constant PERIOD_update_tick_i : time := 1000 ns;
    constant DUTY_CYCLE_update_tick_i : real := 0.2;
    constant OFFSET_update_tick_i : time := 100 ns;

    BEGIN
        UUT : digital_phase_loop
        PORT MAP (
            clk_i => clk_i,
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
                phase_diff_i <= "11011010111";
                -- -------------------------------------
                WAIT FOR 1815 ns;

            END PROCESS;

    END testbench_arch;

