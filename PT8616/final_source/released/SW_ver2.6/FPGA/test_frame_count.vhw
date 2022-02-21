--------------------------------------------------------------------------------
-- Copyright (c) 1995-2007 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 9.2.03i
--  \   \         Application : ISE
--  /   /         Filename : test_frame_count.vhw
-- /___/   /\     Timestamp : Fri Nov 09 15:40:25 2007
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: test_frame_count
--Device: Xilinx
--

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
USE IEEE.STD_LOGIC_TEXTIO.ALL;
USE STD.TEXTIO.ALL;

ENTITY test_frame_count IS
END test_frame_count;

ARCHITECTURE testbench_arch OF test_frame_count IS
    FILE RESULTS: TEXT OPEN WRITE_MODE IS "results.txt";

    COMPONENT LTC_frame_counter
        PORT (
            clk_i : In std_logic;
            ltc_frame_tick_i : In std_logic;
            sync_i : In std_logic;
            dropframe_i : In std_logic;
            format_i : In std_logic_vector (1 DownTo 0);
            hours_i : In std_logic_vector (7 DownTo 0);
            mins_i : In std_logic_vector (7 DownTo 0);
            secs_i : In std_logic_vector (7 DownTo 0);
            frames_i : In std_logic_vector (7 DownTo 0);
            hour_tens_o : Out std_logic_vector (1 DownTo 0);
            hour_units_o : Out std_logic_vector (3 DownTo 0);
            min_tens_o : Out std_logic_vector (2 DownTo 0);
            min_units_o : Out std_logic_vector (3 DownTo 0);
            sec_tens_o : Out std_logic_vector (2 DownTo 0);
            sec_units_o : Out std_logic_vector (3 DownTo 0);
            frame_tens_o : Out std_logic_vector (1 DownTo 0);
            frame_units_o : Out std_logic_vector (3 DownTo 0);
            update_frame_o : Out std_logic
        );
    END COMPONENT;

    SIGNAL clk_i : std_logic := '0';
    SIGNAL ltc_frame_tick_i : std_logic := '0';
    SIGNAL sync_i : std_logic := '0';
    SIGNAL dropframe_i : std_logic := '0';
    SIGNAL format_i : std_logic_vector (1 DownTo 0) := "00";
    SIGNAL hours_i : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL mins_i : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL secs_i : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL frames_i : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL hour_tens_o : std_logic_vector (1 DownTo 0) := "00";
    SIGNAL hour_units_o : std_logic_vector (3 DownTo 0) := "0000";
    SIGNAL min_tens_o : std_logic_vector (2 DownTo 0) := "000";
    SIGNAL min_units_o : std_logic_vector (3 DownTo 0) := "0000";
    SIGNAL sec_tens_o : std_logic_vector (2 DownTo 0) := "000";
    SIGNAL sec_units_o : std_logic_vector (3 DownTo 0) := "0000";
    SIGNAL frame_tens_o : std_logic_vector (1 DownTo 0) := "00";
    SIGNAL frame_units_o : std_logic_vector (3 DownTo 0) := "0000";
    SIGNAL update_frame_o : std_logic := '0';

    constant PERIOD_clk_i : time := 20 ns;
    constant DUTY_CYCLE_clk_i : real := 0.5;
    constant OFFSET_clk_i : time := 100 ns;
    constant PERIOD_ltc_frame_tick_i : time := 120 ns;
    constant DUTY_CYCLE_ltc_frame_tick_i : real := 0.1666666667;
    constant OFFSET_ltc_frame_tick_i : time := 100 ns;

    BEGIN
        UUT : LTC_frame_counter
        PORT MAP (
            clk_i => clk_i,
            ltc_frame_tick_i => ltc_frame_tick_i,
            sync_i => sync_i,
            dropframe_i => dropframe_i,
            format_i => format_i,
            hours_i => hours_i,
            mins_i => mins_i,
            secs_i => secs_i,
            frames_i => frames_i,
            hour_tens_o => hour_tens_o,
            hour_units_o => hour_units_o,
            min_tens_o => min_tens_o,
            min_units_o => min_units_o,
            sec_tens_o => sec_tens_o,
            sec_units_o => sec_units_o,
            frame_tens_o => frame_tens_o,
            frame_units_o => frame_units_o,
            update_frame_o => update_frame_o
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

        PROCESS    -- clock process for ltc_frame_tick_i
        BEGIN
            WAIT for OFFSET_ltc_frame_tick_i;
            CLOCK_LOOP : LOOP
                ltc_frame_tick_i <= '0';
                WAIT FOR (PERIOD_ltc_frame_tick_i - (PERIOD_ltc_frame_tick_i * DUTY_CYCLE_ltc_frame_tick_i));
                ltc_frame_tick_i <= '1';
                WAIT FOR (PERIOD_ltc_frame_tick_i * DUTY_CYCLE_ltc_frame_tick_i);
            END LOOP CLOCK_LOOP;
        END PROCESS;

        PROCESS    -- Process for clk_i
            BEGIN
                -- -------------  Current Time:  100ns
                WAIT FOR 100 ns;
                frames_i <= "00100111";
                secs_i <= "01011001";
                format_i <= "10";
                -- -------------------------------------
                -- -------------  Current Time:  105ns
                WAIT FOR 5 ns;
                dropframe_i <= '1';
                sync_i <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  125ns
                WAIT FOR 20 ns;
                sync_i <= '0';
                -- -------------------------------------
                WAIT FOR 995 ns;

            END PROCESS;

    END testbench_arch;

