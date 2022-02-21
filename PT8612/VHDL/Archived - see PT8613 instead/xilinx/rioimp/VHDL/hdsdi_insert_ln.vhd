--------------------------------------------------------------------------------
-- hdsdi_insert_ln.vhd
--
-- This module inserts the line numbers into their appropriate places after
-- the EAV.
--
--
--
--                  Author: John F. Snow
--                  Staff Applications Engineer
--
--                  Video Applications
--                  Advanced Products Group
--                  Xilinx, Inc.
--
--                  Copyright (c) 2003 Xilinx, Inc.
--                  All rights reserved
--
--                  Date:   Dec 12, 2003
--
--                  RESTRICTED RIGHTS LEGEND
--
--      This software has not been published by the author, and 
--      has been disclosed to others for the purpose of enhancing 
--      and promoting design productivity in Xilinx products.
--
--      Therefore use, duplication or disclosure, now and in the 
--      future should give consideration to the productivity 
--      enhancements afforded the user of this code by the author's 
--      efforts.  Thank you for using our products !
--
-- Disclaimer:  THESE DESIGNS ARE PROVIDED "AS IS" WITH NO WARRANTY 
--              WHATSOEVER AND XILINX SPECIFICALLY DISCLAIMS ANY 
--              IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
--              A PARTICULAR PURPOSE, OR AGAINST INFRINGEMENT.
--
-- Revision:
--          Dec 12, 2003    XAPP680 release 1.00
-- 
-- Other modules instanced in this design:
--
--          None.
--------------------------------------------------------------------------------
-- 
-- This module formats the 11-bit line number value into two 10-bit words and 
-- inserts them into their proper places immediately after the EAV word. The
-- insert_ln input can disable the insertion of line numbers. The same line
-- number value is inserted into both video channels. 
-- 
-- In the SMPTE 292M standard, the 11-bit line numbers must be formatted into 
-- two 10-bit words with the format of each word as follows:
-- 
--         b9    b8    b7    b6    b5    b4    b3    b2    b1    b0
--      +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
-- LN0: | ~ln6| ln6 | ln5 | ln4 | ln3 | ln2 | ln1 | ln0 |  0  |  0  |
--      +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
-- LN1: |  1  |  0  |  0  |  0  | ln10| ln9 | ln8 | ln7 |  0  |  0  |
--      +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
--       
-- 
-- This module is purely combinatorial and has no delay registers.
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

use work.hdsdi_pkg.all;

entity hdsdi_insert_ln is
    port (
        insert_ln:  in  std_logic;      -- enables insertion of line numbers when 1
        ln_word0:   in  std_logic;      -- input asserted during time for first LN word in EAV
        ln_word1:   in  std_logic;      -- input asserted during time for second LN word in EAV
        c_in:       in  hd_video_type;  -- C channel video input
        y_in:       in  hd_video_type;  -- Y channel video input
        ln:         in  hd_vpos_type;   -- line number inputs
        c_out:      out hd_video_type;  -- C channel video output
        y_out:      out hd_video_type   -- Y channel video output
    );
end hdsdi_insert_ln;

architecture synth of hdsdi_insert_ln is

begin

    process(ln, insert_ln, c_in, ln_word0, ln_word1)
    begin
        if insert_ln = '1' and ln_word0 = '1' then
            c_out <= (not ln(6) & ln(6 downto 0) & "00");
        elsif insert_ln = '1' and ln_word1 = '1' then
            c_out <= ("1000" & ln(10 downto 7) & "00");
        else
            c_out <= c_in;
        end if; 
    end process;

    process(ln, insert_ln, y_in, ln_word0, ln_word1)
    begin
        if insert_ln = '1' and ln_word0 = '1' then
            y_out <= (not ln(6) & ln(6 downto 0) & "00");
        elsif insert_ln = '1' and ln_word1 = '1' then
            y_out <= ("1000" & ln(10 downto 7) & "00");
        else
            y_out <= y_in;
        end if; 
    end process;

end synth;

