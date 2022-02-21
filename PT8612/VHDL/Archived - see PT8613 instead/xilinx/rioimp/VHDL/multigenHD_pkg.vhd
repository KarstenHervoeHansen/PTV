-------------------------------------------------------------------------------- 
-- Copyright (c) 2004 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: multigenHD_pkg.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2004-12-09 14:34:53-07 $
-- /___/   /\    Date Created: May 28, 2004 
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: multigenHD_pkg.vhd,rcs $
-- Revision 1.1  2004-12-09 14:34:53-07  jsnow
-- Header update.
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
-- This package defines global data types and constants used throughout the
-- multigenHD design.
--
--------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;
use work.hdsdi_pkg.all;

package multigenHD_pkg is

--
-- This group of constant defines the bit widths of various data types.
--
constant VID_WIDTH :    integer := hd_video_type'length;    -- Width of a video word
constant HRGN_WIDTH :   integer := 5;                       -- Width of h_region counter
constant VRGN_WIDTH :   integer := 5;                       -- Width of v_region counter
constant VBAND_WIDTH :  integer := 3;                       -- Width of vband code
constant YRAMP_FRAC :   integer := 7;                       -- Number of fractional bits in Y Ramp reg
constant YRAMP_WIDTH :  integer := VID_WIDTH + YRAMP_FRAC;  -- Width of Y Ramp reg

constant V_EVNT_WIDTH : integer := HD_VCNT_WIDTH;           -- Width of v_next_evnt
constant H_EVNT_WIDTH : integer := HD_HCNT_WIDTH - 1;       -- Width of h_next_evnt
constant V_EVNT_MSB :   integer := V_EVNT_WIDTH - 1;
constant H_EVNT_MSB :   integer := H_EVNT_WIDTH - 1;

--
-- Types for some data formats used in this module
--
subtype hrgn_type       is                                  -- Horizontal region code type
    std_logic_vector(HRGN_WIDTH - 1 downto 0);

subtype vrgn_type       is                                  -- Vertical region code type
    std_logic_vector(VRGN_WIDTH - 1 downto 0);

subtype vband_type      is                                  -- Vertical band code type
    std_logic_vector(VBAND_WIDTH - 1 downto 0);

subtype yramp_type      is                                  -- Y ramp type
    std_logic_vector(YRAMP_WIDTH - 1 downto 0);

--
-- This group of constants defines the horizontal regions from the horizontal
-- ROM. Note that the three HRGN_USROPTx regions are never generated by the
-- horizontal ROM. An encoder will change HRGN_BAR1 to any of these three values
-- depending on the user option inputs before feeding the modified horizontal
-- region code to the color ROM. The encoder will also determine whether to
-- output the HRGN_CEQ_POL0 or POL1 code depending on whether the current frame
-- is odd or even.
--
constant HRGN_BAR0          : hrgn_type     := "00000"; -- 40% gray
constant HRGN_BAR1          : hrgn_type     := "00001"; -- 75% white (left part 0% black)
constant HRGN_BAR2          : hrgn_type     := "00010"; -- left part 75% yellow (right part 0% black)
constant HRGN_BAR3          : hrgn_type     := "00011"; -- right part 75% yellow (right part 100% white)
constant HRGN_BAR4          : hrgn_type     := "00100"; -- 75% cyan (middle part 100% white)
constant HRGN_BAR5          : hrgn_type     := "00101"; -- left part 75% green (left part 100% white)
constant HRGN_BAR6          : hrgn_type     := "00110"; -- right part 75% green (left part 0% black)
constant HRGN_BAR7          : hrgn_type     := "00111"; -- left part 75% magenta (right part 0% black) 
constant HRGN_BAR8          : hrgn_type     := "01000"; -- middle part 75% magenta (-2% black)
constant HRGN_BAR9          : hrgn_type     := "01001"; -- right part 75% magenta (0% black)
constant HRGN_BAR10         : hrgn_type     := "01010"; -- left part 75% red (+2% black)
constant HRGN_BAR11         : hrgn_type     := "01011"; -- middle part 75% red (0% black)
constant HRGN_BAR12         : hrgn_type     := "01100"; -- right part 75% red (+4% black)
constant HRGN_BAR13         : hrgn_type     := "01101"; -- 75% blue (0% black)
constant HRGN_BAR14         : hrgn_type     := "01110"; -- 40% gray
constant HRGN_INCV          : hrgn_type     := "01111"; -- increment vertical line number
constant HRGN_EAV1          : hrgn_type     := "10000"; -- first two words of EAV
constant HRGN_EAV2_F0       : hrgn_type     := "10001"; -- second two words of EAV for field 0
constant HRGN_HBLANK        : hrgn_type     := "10010"; -- horizontal blanking period
constant HRGN_SAV1          : hrgn_type     := "10011"; -- first two words of SAV
constant HRGN_SAV2_F0       : hrgn_type     := "10100"; -- second two words of SAV for field 0
constant HRGN_LN            : hrgn_type     := "10101"; -- two words of line number
constant HRGN_CRC           : hrgn_type     := "10110"; -- two words of CRC
constant HRGN_SAV2_F1       : hrgn_type     := "10111"; -- second two words of SAV for field 1
constant HRGN_USROPT1       : hrgn_type     := "11000"; -- BAR1 but with 100% white in pat 2, 0% black in pat 3
constant HRGN_USROPT2       : hrgn_type     := "11001"; -- BAR1 but with +I in pat 2, 0% black in pat 3
constant HRGN_USROPT3       : hrgn_type     := "11010"; -- BAR1 but with -I in pat 2, +Q in pat 3
constant HRGN_EAV2_F1       : hrgn_type     := "11011"; -- second two words of EAV for field 1
constant HRGN_CEQ_PLL       : hrgn_type     := "11100"; -- RP 198 cable equalization pattern
constant HRGN_CEQ_POL_0     : hrgn_type     := "11101"; -- RP 198 cable eq pattern, polarity word even frame
constant HRGN_CEQ_POL_1     : hrgn_type     := "11110"; -- RP 198 cable eq pattern, polarity word odd frame
constant HRGN_RST           : hrgn_type     := "11111"; -- initial state after reset


--
-- The group of constants defines the vertical regions from the vertical ROM.
-- Note that the vertical ROM generates a 5-bit vertical region code and a 3-bit
-- vertical band code. The region code is essentially the current state of the
-- vertical state machine and feeds back to the input of the vertical ROM.
-- The vertical band code is sent to the color ROM to indicate the current
-- vertical pattern band.
--
constant VRGN_FM0_F0_VB_0   : vrgn_type     := "00000"; -- frame 0, field 0, first vertical blanking interval
constant VRGN_FM0_F0_1ST_ACT: vrgn_type     := "00001"; -- frame 0, field 0, first active line (for CEQ polarity)
constant VRGN_FM0_F0_PAT1   : vrgn_type     := "00010"; -- frame 0, field 0, pattern 1 or cable eq pattern
constant VRGN_FM0_F0_PAT2   : vrgn_type     := "00011"; -- frame 0, field 0, pattern 2 or PLL pattern
constant VRGN_FM0_F0_PAT3   : vrgn_type     := "00100"; -- frame 0, field 0, pattern 3
constant VRGN_FM0_F0_PAT4   : vrgn_type     := "00101"; -- frame 0, field 0, pattern 4
constant VRGN_FM0_F0_VB_1   : vrgn_type     := "00110"; -- frame 0, field 0, second vertical blanking interval
constant VRGN_FM0_F1_VB_0   : vrgn_type     := "00111"; -- frame 0, field 1, first vertical blanking interval
constant VRGN_FM0_F1_PAT1   : vrgn_type     := "01000"; -- frame 0, field 1, pattern 1 or cable eq pattern
constant VRGN_FM0_F1_PAT2   : vrgn_type     := "01001"; -- frame 0, field 1, pattern 2 or PLL pattern
constant VRGN_FM0_F1_PAT3   : vrgn_type     := "01010"; -- frame 0, field 1, pattern 3
constant VRGN_FM0_F1_PAT4   : vrgn_type     := "01011"; -- frame 0, field 1, pattern 4
constant VRGN_FM0_F1_VB_1   : vrgn_type     := "01100"; -- frame 0, field 1, second vertical blanking interval
constant VRGN_FM0_CLRV      : vrgn_type     := "01101"; -- frame 0 clears the vertical counter back to 1
constant VRGN_X14           : vrgn_type     := "01110"; -- unused
constant VRGN_X15           : vrgn_type     := "01111"; -- unused
constant VRGN_FM1_F0_VB_0   : vrgn_type     := "10000"; -- frame 1, field 0, first vertical blanking interval
constant VRGN_FM1_F0_PAT1   : vrgn_type     := "10001"; -- frame 1, field 0, pattern 1 or cable eq pattern
constant VRGN_FM1_F0_PAT2   : vrgn_type     := "10010"; -- frame 1, field 0, pattern 2 or PLL pattern
constant VRGN_FM1_F0_PAT3   : vrgn_type     := "10011"; -- frame 1, field 0, pattern 3
constant VRGN_FM1_F0_PAT4   : vrgn_type     := "10100"; -- frame 1, field 0, pattern 4
constant VRGN_FM1_F0_VB_1   : vrgn_type     := "10101"; -- frame 1, field 0, second vertical blanking interval
constant VRGN_X22           : vrgn_type     := "10110"; -- unused
constant VRGN_FM1_F1_VB_0   : vrgn_type     := "10111"; -- frame 1, field 1, first vertical blanking interval
constant VRGN_FM1_F1_PAT1   : vrgn_type     := "11000"; -- frame 1, field 1, pattern 1 or cable eq pattern
constant VRGN_FM1_F1_PAT2   : vrgn_type     := "11001"; -- frame 1, field 1, pattern 2 or PLL pattern
constant VRGN_FM1_F1_PAT3   : vrgn_type     := "11010"; -- frame 1, field 1, pattern 3
constant VRGN_FM1_F1_PAT4   : vrgn_type     := "11011"; -- frame 1, field 1, pattern 4
constant VRGN_FM1_F1_VB_1   : vrgn_type     := "11100"; -- frame 1, field 1, second vertical blanking interval
constant VRGN_FM1_CLRV      : vrgn_type     := "11101"; -- frame 1 clears the vertical counter back to 1
constant VRGN_X30           : vrgn_type     := "11110"; -- unused
constant VRGN_RST           : vrgn_type     := "11111"; -- initial state after reset

constant VBAND_VB           : vband_type    := "000";   -- vertical blanking interval
constant VBAND_PAT1         : vband_type    := "001";   -- pattern 1
constant VBAND_PAT2         : vband_type    := "010";   -- pattern 2
constant VBAND_PAT3         : vband_type    := "011";   -- pattern 3
constant VBAND_PAT4         : vband_type    := "100";   -- pattern 4
constant VBAND_CEQ          : vband_type    := "101";   -- cable equalization pattern
constant VBAND_PLL          : vband_type    := "110";   -- PLL pattern
constant VBAND_X7           : vband_type    := "111";   -- unused

--
-- Y-ramp constants
--
-- These constants would need to change if the video width changes from 10-bits.
--
constant YRAMP_INIT         : yramp_type    := "00001111111100000"; -- 63.75 is initial Y ramp starting value
constant Y_INC_1920         : yramp_type    := "00000000001011011"; -- 0.7109375 is ramp increment for 1920 wide standards
constant Y_INC_1280         : yramp_type    := "00000000010001001"; -- 1.0703125 is ramp increment for 1280 wide standards
constant YRAMP_RND          : yramp_type    := "00000000001000000"; -- 0.5 is the round up increment value for rounder

     
end;
