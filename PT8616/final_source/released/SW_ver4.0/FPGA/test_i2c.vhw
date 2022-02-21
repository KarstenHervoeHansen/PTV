--------------------------------------------------------------------------------
-- Copyright (c) 1995-2007 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 9.2.03i
--  \   \         Application : ISE
--  /   /         Filename : test_i2c.vhw
-- /___/   /\     Timestamp : Thu Nov 15 11:06:17 2007
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: test_i2c
--Device: Xilinx
--

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
USE IEEE.STD_LOGIC_TEXTIO.ALL;
USE STD.TEXTIO.ALL;

ENTITY test_i2c IS
END test_i2c;

ARCHITECTURE testbench_arch OF test_i2c IS
    FILE RESULTS: TEXT OPEN WRITE_MODE IS "results.txt";

    COMPONENT I2C_engine
        PORT (
            reset_i : In std_logic;
            clk_i : In std_logic;
            SCL_i : In std_logic;
            SDA_io : InOut std_logic;
            send_byte_i : In std_logic_vector (7 DownTo 0);
            recieved_byte_o : Out std_logic_vector (7 DownTo 0);
            address_i : In std_logic_vector (6 DownTo 0);
            update_flag_o : Out std_logic;
            start_o : Out std_logic;
            stop_o : Out std_logic;
            error_o : Out std_logic
        );
    END COMPONENT;

    SIGNAL reset_i : std_logic := '0';
    SIGNAL clk_i : std_logic := '0';
    SIGNAL SCL_i : std_logic := '0';
    SIGNAL SDA_io : std_logic := 'Z';
    SIGNAL send_byte_i : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL recieved_byte_o : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL address_i : std_logic_vector (6 DownTo 0) := "0000000";
    SIGNAL update_flag_o : std_logic := '0';
    SIGNAL start_o : std_logic := '0';
    SIGNAL stop_o : std_logic := '0';
    SIGNAL error_o : std_logic := '0';

    constant PERIOD : time := 200 ns;
    constant DUTY_CYCLE : real := 0.5;
    constant OFFSET : time := 100 ns;

    BEGIN
        UUT : I2C_engine
        PORT MAP (
            reset_i => reset_i,
            clk_i => clk_i,
            SCL_i => SCL_i,
            SDA_io => SDA_io,
            send_byte_i => send_byte_i,
            recieved_byte_o => recieved_byte_o,
            address_i => address_i,
            update_flag_o => update_flag_o,
            start_o => start_o,
            stop_o => stop_o,
            error_o => error_o
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
                -- -------------  Current Time:  185ns
                WAIT FOR 185 ns;
                SCL_i <= '1';
                SDA_io <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  385ns
                WAIT FOR 200 ns;
                SDA_io <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  585ns
                WAIT FOR 200 ns;
                SCL_i <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  785ns
                WAIT FOR 200 ns;
                SDA_io <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  1385ns
                WAIT FOR 600 ns;
                SCL_i <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  1985ns
                WAIT FOR 600 ns;
                SCL_i <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  2585ns
                WAIT FOR 600 ns;
                SCL_i <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  3185ns
                WAIT FOR 600 ns;
                SCL_i <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  3785ns
                WAIT FOR 600 ns;
                SCL_i <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  4385ns
                WAIT FOR 600 ns;
                SCL_i <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  4985ns
                WAIT FOR 600 ns;
                SCL_i <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  5585ns
                WAIT FOR 600 ns;
                SCL_i <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  5785ns
                WAIT FOR 200 ns;
                SDA_io <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  6185ns
                WAIT FOR 400 ns;
                SCL_i <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  6785ns
                WAIT FOR 600 ns;
                SCL_i <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  7385ns
                WAIT FOR 600 ns;
                SCL_i <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  7985ns
                WAIT FOR 600 ns;
                SCL_i <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  8585ns
                WAIT FOR 600 ns;
                SCL_i <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  9185ns
                WAIT FOR 600 ns;
                SCL_i <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  9785ns
                WAIT FOR 600 ns;
                SCL_i <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  10385ns
                WAIT FOR 600 ns;
                SCL_i <= '0';
                -- -------------------------------------
                WAIT FOR 989815 ns;

            END PROCESS;

    END testbench_arch;

