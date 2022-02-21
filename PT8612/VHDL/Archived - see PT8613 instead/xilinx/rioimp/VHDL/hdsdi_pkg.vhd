--------------------------------------------------------------------------------
-- hdsdi_pkg.vhd
--
-- VHDL package for the Xilinx HD-SDI (SMPTE 292M) reference designs
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
--                  Copyright (c) 2004 Xilinx, Inc.
--                  All rights reserved
--
--                  Date:   Mar 2, 2004
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
--              Mar 2, 2004    XAPP683 release 1.00
--
-- Other modules instanced in this design:
--          none
--
--------------------------------------------------------------------------------
-- 
-- This package defines global data types and constants used throughout the
-- Xilinx HD-SDI reference designs.
--
--------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

package hdsdi_pkg is

--------------------------------------------------------------------------------
-- These constants defines the widths of various data types and are used
-- the in following data type definitions.
--
constant HD_HCNT_WIDTH :       integer := 12;  -- width of horz position count
constant HD_VCNT_WIDTH :       integer := 11;  -- width of vert position count

--------------------------------------------------------------------------------
-- Data type definitions
--
subtype hd_video_type      is               -- used for all video-width words
            std_logic_vector(9 downto 0);

subtype hd_vid20_type      is               -- used for all 20-bit video words
            std_logic_vector(19 downto 0);  -- containing both Y & C (Y in MS hslf)

subtype hd_vidstd_type     is               -- 4-bit code used to indicate
            std_logic_vector(3 downto 0);   -- the video standard

subtype hd_hpos_type       is               -- horizontal position type
            std_logic_vector (HD_HCNT_WIDTH - 1 downto 0);

subtype hd_vpos_type       is               -- vertical position type
            std_logic_vector (HD_VCNT_WIDTH - 1 downto 0);

subtype hd_crc18_type      is               -- CRC18 data type
            std_logic_vector(17 downto 0);

--------------------------------------------------------------------------------
-- Constant defintions

--
-- This group of constants defines the encoding for the HD video formats used
-- by the video pattern generators and video format detectors.
--
constant HD_FMT_1035i_30    : hd_vidstd_type := "0000"; -- SMPTE 260M 1035i  30 Hz
constant HD_FMT_1080i_25b   : hd_vidstd_type := "0001"; -- SMPTE 295M 1080i  25 Hz
constant HD_FMT_1080i_30    : hd_vidstd_type := "0010"; -- SMPTE 274M 1080i  30 Hz or 1080sF 30 Hz
constant HD_FMT_1080i_25    : hd_vidstd_type := "0011"; -- SMPTE 274M 1080i  25 Hz or 1080sF 25 Hz
constant HD_FMT_1080p_30    : hd_vidstd_type := "0100"; -- SMPTE 274M 1080p  30 Hz 
constant HD_FMT_1080p_25    : hd_vidstd_type := "0101"; -- SMPTE 274M 1080p  25 Hz
constant HD_FMT_1080p_24    : hd_vidstd_type := "0110"; -- SMPTE 274M 1080p  24 Hz
constant HD_FMT_720p_60     : hd_vidstd_type := "0111"; -- SMPTE 296M  720p  60 Hz
constant HD_FMT_1080sF_24   : hd_vidstd_type := "1000"; -- SMPTE 274M 1080sF 24 Hz
constant HD_FMT_RSVD_9      : hd_vidstd_type := "1001"; -- reserved code
constant HD_FMT_RSVD_10     : hd_vidstd_type := "1010"; -- reserved code
constant HD_FMT_RSVD_11     : hd_vidstd_type := "1011"; -- reserved code
constant HD_FMT_RSVD_12     : hd_vidstd_type := "1100"; -- reserved code
constant HD_FMT_RSVD_13     : hd_vidstd_type := "1101"; -- reserved code
constant HD_FMT_RSVD_14     : hd_vidstd_type := "1110"; -- reserved code
constant HD_FMT_RSVD_15     : hd_vidstd_type := "1111"; -- reserved code
     
end;
