-------------------------------------------------------------------------------- 
-- Copyright (c) 2002 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: testbench_cb1.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2004-08-27 13:20:36-06 $
-- /___/   /\    Date Created: January 7, 2002 
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: testbench_cb1.vhd,rcs $
-- Revision 1.0  2004-08-27 13:20:36-06  jsnow
-- Initial version.
--
-------------------------------------------------------------------------------- 
--   
--   XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" 
--   AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND 
--   SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE, 
--   OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, 
--   APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION 
--   THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT, 
--   AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE 
--   FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY 
--   WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE 
--   IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR 
--   REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF 
--   INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
--   FOR A PARTICULAR PURPOSE. 
--
-------------------------------------------------------------------------------- 
--
-- This testbench can be used to compare the output of two different video 
-- pattern generators to verify that they generate the same output.
--
-- In this example, the test bench is comparing the RP 178 pattern produced by
-- the cb_eg1_rp178 module with its pattern input strapped high to the output
-- of the rp178 module. 
--
-------------------------------------------------------------------------------- 

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;

entity testbench_cb1 is
end testbench_cb1;

architecture simulation of testbench_cb1 is

component cb_eg1_rp178
    port(
        clk:        in  std_ulogic;
        rst:        in  std_ulogic;
        ce:         in  std_ulogic;
        pattern:    in  std_ulogic;
        q:          out std_ulogic_vector(9 downto 0);
        h_sync:     out std_ulogic;
        v_sync:     out std_ulogic;
        field:      out std_ulogic
    );
end component;

component rp178
    port(
        clk:        in  std_ulogic;
        rst:        in  std_ulogic;
        ce:         in  std_ulogic;
        q:          out std_ulogic_vector(9 downto 0);
        h_sync:     out std_ulogic;
        v_sync:     out std_ulogic;
        field:      out std_ulogic      
    );
end component;

signal clk :    std_ulogic := '0';
signal rst :    std_ulogic := '1';

-- Output signals of first pattern generator
signal out1 :   std_ulogic_vector(9 downto 0);
signal h1 :     std_ulogic;
signal v1 :     std_ulogic;
signal f1 :     std_ulogic;

-- Output signals of second pattern generator
signal out2 :   std_ulogic_vector(9 downto 0);
signal h2 :     std_ulogic;
signal v2 :     std_ulogic;
signal f2 :     std_ulogic;

signal VCC :    std_ulogic;

begin

    VCC <= '1';     

    --
    -- Instantiate the reference test pattern generator
    --
    U1 : cb_eg1_rp178
        port map (
            clk         => clk,
            rst         => rst,
            ce          => VCC,
            pattern     => VCC,
            q           => out1,
            h_sync      => h1,
            v_sync      => v1,
            field       => f1
    );

    --
    -- Instantiate the test pattern generator to be compared to the reference.
    --
    U2 : rp178
        port map (
            clk         => clk,
            rst         => rst,
            ce          => VCC,
            q           => out2,
            h_sync      => h2,
            v_sync      => v2,
            field       => f2
    );

    --
    -- Generate the clock signal at about 27 MHz.
    --
    clk <= not clk after 18.5 ns;

    --
    -- Assert the reset signal for the first few clock cycles.
    --
    process
    begin
        rst <= '1';
        for count in 1 to 4 loop
            wait until clk'event and clk ='0';
        end loop;
        rst <= '0';
        wait;
    end process;

    --
    -- Compare the outputs of the two test pattern generators.
    --

    process(clk)
    begin
        if (clk'event and clk = '1') then
            assert (out1 = out2)
                report "Video data from U2 does not match U1."
                severity error;

            assert (h1 = h2)
                report "h_sync signal from U2 does not match U1."
                severity error;
                
            assert (v1 = v2)
                report "v_sync signal from U2 does not match U1."
                severity error;
                
            assert (f1 = f2)
                report "field signal from U2 does not match U1."
                severity error;   
        end if; 
    end process;


end simulation;