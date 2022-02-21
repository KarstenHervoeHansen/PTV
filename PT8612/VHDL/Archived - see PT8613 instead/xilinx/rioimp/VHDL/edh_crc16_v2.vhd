-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: edh_crc16_v2.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2005-01-13 15:15:42-07 $
-- /___/   /\    Date Created: Jan 5, 2005
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: edh_crc16_v2.vhd,rcs $
-- Revision 1.1  2005-01-13 15:15:42-07  jsnow
-- XAPP580 version 1.0 release.
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
-- This module does a 16-bit CRC calculation.
-- 
-- The calculation is a standard CRC16 calculation with a polynomial of x^16 + 
-- x^12 + x^5 + 1. The function considers the LSB of the video data as the first
-- bit shifted into the CRC generator, although the implementation given here is
-- a fully parallel CRC, calculating all 16 CRC bits from the 10-bit video data 
-- in one clock cycle.  
-- 
-- The assignment statements have all be optimized to use 4-input XOR gates
-- wherever possible to fit efficiently in the Xilinx FPGA architecture.
-- 
-- There are two input ports: c and d. The 16-bit c port must be connected to 
-- the CRC "accumulation" register hold the last calculated CRC value. The 10-
-- bit d port is connected to the video stream.
-- 
-- The output port, crc, must be connected to the input of CRC "accumulation"
-- register.
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

library unisim; 
use unisim.vcomponents.all; 

entity edh_crc16 is
port (
    c :             in    std_logic_vector(15 downto 0);    -- current CRC value
    d :             in    std_logic_vector(9  downto 0);    -- input data word
    crc :           inout std_logic_vector(15 downto 0));   -- new calculated CRC value
end edh_crc16;

architecture synth of edh_crc16 is

attribute init : string;
attribute init of CRC_T1 : label is "6996";
attribute init of CRC0   : label is "6";
attribute init of CRC1   : label is "6996";
attribute init of CRC2   : label is "6996";
attribute init of CRC3   : label is "6996";
attribute init of CRC4   : label is "96";
attribute init of CRC5   : label is "6";
attribute init of CRC6   : label is "96";
attribute init of CRC7   : label is "96";
attribute init of CRC8   : label is "96";
attribute init of CRC9   : label is "96";
attribute init of CRC10  : label is "6";
attribute init of CRC11  : label is "6996";
attribute init of CRC12  : label is "6996";
attribute init of CRC13  : label is "6996";
attribute init of CRC14  : label is "96";
attribute init of CRC15  : label is "96";

--
-- Signal definitions
--
signal t1 :     std_logic;  -- intermediate product term used several times

begin

CRC_T1 : LUT4
-- synthesis translate_off
generic map (INIT => X"6996")
-- synthesis translate_on
port map (
    O  => t1,
    I0 => d(4),
    I1 => c(4),
    I2 => d(0),
    I3 => c(0));

CRC0 : LUT2
-- synthesis translate_off
generic map (INIT => X"6")
-- synthesis translate_on
port map (
    O  => crc(0),
    I0 => c(10),
    I1 => crc(12));

CRC1 : LUT4
-- synthesis translate_off
generic map (INIT => X"6996")
-- synthesis translate_on
port map (
    O   => crc(1),
    I0  => c(11),
    I1  => d(0),
    I2  => c(0),
    I3  => crc(13));

CRC2 : LUT4
-- synthesis translate_off
generic map (INIT => X"6996")
-- synthesis translate_on
port map (
    O   => crc(2),
    I0  => c(12),
    I1  => d(1),
    I2  => c(1),
    I3  => crc(14));

CRC3 : LUT4
-- synthesis translate_off
generic map (INIT => X"6996")
-- synthesis translate_on
port map (
    O   => crc(3),
    I0  => c(13),
    I1  => d(2),
    I2  => c(2),
    I3  => crc(15));

CRC4 : LUT3
-- synthesis translate_off
generic map (INIT => X"96")
-- synthesis translate_on
port map (
    O   => crc(4),
    I0  => c(14),
    I1  => d(3),
    I2  => c(3));

CRC5 : LUT2
-- synthesis translate_off
generic map (INIT => X"6")
-- synthesis translate_on
port map (
    O   => crc(5),
    I0  => c(15),
    I1  => t1);

CRC6 : LUT3
-- synthesis translate_off
generic map (INIT => X"96")
-- synthesis translate_on
port map (
    O   => crc(6),
    I0  => d(0),
    I1  => c(0),
    I2  => crc(11));

CRC7 : LUT3
-- synthesis translate_off
generic map (INIT => X"96")
-- synthesis translate_on
port map (
    O   => crc(7),
    I0  => d(1),
    I1  => c(1),
    I2  => crc(12));

CRC8 : LUT3
-- synthesis translate_off
generic map (INIT => X"96")
-- synthesis translate_on
port map (
    O   => crc(8),
    I0  => d(2),
    I1  => c(2),
    I2  => crc(13));

CRC9 : LUT3
-- synthesis translate_off
generic map (INIT => X"96")
-- synthesis translate_on
port map (
    O   => crc(9),
    I0  => d(3),
    I1  => c(3),
    I2  => crc(14));

CRC10 : LUT2
-- synthesis translate_off
generic map (INIT => X"6")
-- synthesis translate_on
port map (
    O   => crc(10),
    I0  => t1,
    I1  => crc(15));

CRC11 : LUT4
-- synthesis translate_off
generic map (INIT => X"6996")
-- synthesis translate_on
port map (
    O   => crc(11),
    I0  => d(5),
    I1  => c(5),
    I2  => d(1),
    I3  => c(1));

CRC12 : LUT4
-- synthesis translate_off
generic map (INIT => X"6996")
-- synthesis translate_on
port map (
    O   => crc(12),
    I0  => d(6),
    I1  => c(6),
    I2  => d(2),
    I3  => c(2));

CRC13 : LUT4
-- synthesis translate_off
generic map (INIT => X"6996")
-- synthesis translate_on
port map (
    O   => crc(13),
    I0  => d(7),
    I1  => c(7),
    I2  => d(3),
    I3  => c(3));

CRC14 : LUT3
-- synthesis translate_off
generic map (INIT => X"96")
-- synthesis translate_on
port map (
    O   => crc(14),
    I0  => d(8),
    I1  => c(8),
    I2  => t1);

CRC15 : LUT3
-- synthesis translate_off
generic map (INIT => X"96")
-- synthesis translate_on
port map (
    O   => crc(15),
    I0  => d(9),
    I1  => c(9),
    I2  => crc(11));

end synth;
