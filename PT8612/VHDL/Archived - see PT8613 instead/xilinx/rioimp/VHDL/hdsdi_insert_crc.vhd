--------------------------------------------------------------------------------
-- hdsdi_insert_crc.vhd
--
-- Inserts CRC values into the HD-SDI video stream.
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
--          none
--
--------------------------------------------------------------------------------
-- 
-- This formats the 18-bit CRC values for each channel into two 10-bit video 
-- words and inserts them into the appropriate places immediately after the line
-- number words in the EAV.
-- 
-- An 18-bit CRC value is formatted into two 10-bit words that are inserted 
-- after the EAV and line number words. The format of the CRC words is shown 
-- below:
--  
--          b9     b8     b7     b6     b5     b4     b3     b2     b1     b0
--       +------+------+------+------+------+------+------+------+------+------+
-- CRC0: |~crc8 | crc8 | crc7 | crc6 | crc5 | crc4 | crc3 | crc2 | crc1 | crc0 |
--       +------+------+------+------+------+------+------+------+------+------+
-- CRC1: |~crc17| crc16| crc15| crc14| crc13| crc12| crc11| crc10| crc9 | crc8 |
--       +------+------+------+------+------+------+------+------+------+------+
-- 
-- This module is purely combinatorial and contains no delay registers.
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

use work.hdsdi_pkg.all;

entity hdsdi_insert_crc is
    port (
        insert_crc: in  std_logic;      -- CRC valaues will be inserted when this input is high
        crc_word0:  in  std_logic;      -- input asserted during time for first CRC word in EAV
        crc_word1:  in  std_logic;      -- input asserted during time for second CRC word in EAV
        c_in:       in  hd_video_type;  -- C channel video input
        y_in:       in  hd_video_type;  -- Y channel video input
        c_crc:      in  hd_crc18_type;  -- C channel CRC value input
        y_crc:      in  hd_crc18_type;  -- Y channel CRC value input
        c_out:      out hd_video_type;  -- C channel video output
        y_out:      out hd_video_type   -- Y channel video output
    );
end hdsdi_insert_crc;

architecture synth of hdsdi_insert_crc is

begin

    process(insert_crc, c_crc, c_in, crc_word0, crc_word1)
    begin
        if insert_crc = '1' and crc_word0 = '1' then
            c_out <= (not c_crc(8) & c_crc(8 downto 0));
        elsif insert_crc = '1' and crc_word1 = '1' then
            c_out <= (not c_crc(17) & c_crc(17 downto 9));
        else
            c_out <= c_in;
        end if;
    end process;

    process(insert_crc, y_crc, y_in, crc_word0, crc_word1)
    begin
        if insert_crc = '1' and crc_word0 = '1' then
            y_out <= (not y_crc(8) & y_crc(8 downto 0));
        elsif insert_crc = '1' and crc_word1 = '1' then
            y_out <= (not y_crc(17) & y_crc(17 downto 9));
        else
            y_out <= y_in;
        end if;
    end process;

end synth;