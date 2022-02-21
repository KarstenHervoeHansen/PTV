-------------------------------------------------------------------------------- 
-- Copyright (c) 2002 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: cb_eg1_rp178.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2004-08-27 13:14:13-06 $
-- /___/   /\    Date Created: January 7, 2002 
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: cb_eg1_rp178.vhd,rcs $
-- Revision 1.1  2004-08-27 13:14:13-06  jsnow
-- Clock enable fixes.
--
-- Revision 1.0  2004-08-27 13:12:27-06  jsnow
-- Initial release.
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
-- This colorbar generator will generate color bars for the SMPTE 125M video
-- standard which specifies 525 line 4:2:2 component digital video. All video is
-- generated at 10-bit resolution.

-- The color bars comply with SMPTE EG-1 standard color bars. The EG-1 color bar
-- pattern has three horizontal bands with different bars in each band as shown 
-- in the diagram below.

--      b        b        b        b        b        b        b
-- +--------+--------+--------+--------+--------+--------+--------+ - - - - - -
-- |        |        |        |        |        |        |        |   ^  ^
-- |        |        |        |        |        |        |        |   |  |
-- |        |        |        |        |        |        |        |   |  |
-- |        |        |        |        |        |        |        |   |  |
-- |        |        |        |        |        |        |        |  67% |
-- |  GRAY  | YELLOW |  CYAN  |  GREEN | MAGENTA|   RED  |  BLUE  |   |  |
-- |        |        |        |        |        |        |        |   |  |
-- |        |        |        |        |        |        |        |   | 100%
-- |        |        |        |        |        |        |        |   |  |
-- |        |        |        |        |        |        |        |   |  |
-- |        |        |        |        |        |        |        |   v  |
-- +--------+--------+--------+--------+--------+--------+--------+ - - -|
-- |  BLUE  |  BLACK | MAGENTA|  BLACK |  CYAN  |  BLACK |  GRAY  |   8% |
-- +--------+-+------+----+---+------+-+--------+--------+--------+ - - -|
-- |          |           |          |          |  |  |  |        |      |
-- |    -I    |    100%   | +Q       |   BLACK  |BL|BL|BL|  BLACK |      |
-- |          |   WHITE   |          |          |K-|AC|K+|        |      |
-- |          |           |          |          |4%|K |4%|        |      v
-- +----------+-----------+----------+----------+--+--+--+--------+ - - - - - -
--     5/4b        5/4b       5/4b       5/4b    b  b  b      b
--                                               -  -  -
-- b = 1/7 active line time                      3  3  3
-- 
-- This module will also generate SMPTE RP 178-1996 recommended SDI checkfield
-- test pattern that stresses the cable equalizer and the PLL circuits of the
-- SDI receiver.  If the pattern input is low, the EG 1 color bars are
-- generated. If the pattern input is high, then the RP 178 test pattern is
-- generated.
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

entity cb_eg1_rp178_vhd is
    generic (
        VID_WIDTH : integer := 10
    );
    port (
        clk:        in  std_ulogic;                                 -- clock input
        rst:        in  std_ulogic;                                 -- async reset input
        ce:         in  std_ulogic;                                 -- clock enable input
        pattern:    in  std_ulogic;                                 -- 0 = EG1, 1 = RP178
        q:          out std_ulogic_vector(VID_WIDTH - 1 downto 0);  -- video output
        h_sync:     out std_ulogic;                                 -- horizontal sync out
        v_sync:     out std_ulogic;                                 -- vertical sync out
        field:      out std_ulogic                                  -- field output
    );
end cb_eg1_rp178_vhd;


architecture synth of cb_eg1_rp178_vhd is
    
--
-- This group of constants controls the widths of various fields in the design.
-- Note that if the generic VID_WIDTH is modified, the component values of
-- the various colors must be modified accordingly.
--
constant HCNT_WIDTH :   integer := 11;                              -- Width of h_counter
constant VCNT_WIDTH :   integer := 10;                              -- Width of v_counter
constant HRGN_WIDTH :   integer := 4;                               -- Width of h_region counter
constant VRGN_WIDTH :   integer := 4;                               -- Width of v_region counter
constant COLOR_WIDTH :  integer := 4;                               -- Width of color code
constant VBAND_WIDTH :  integer := 2;                               -- Width of vband code

constant V_EVNT_WIDTH : integer := VCNT_WIDTH;                      -- Width of v_next_evnt
constant H_EVNT_WIDTH : integer := HCNT_WIDTH - 2;                  -- Width of h_next_evnt
constant VROM_WIDTH :   integer := V_EVNT_WIDTH + VBAND_WIDTH + 3;  -- Width of v_rom
constant HROM_WIDTH :   integer := H_EVNT_WIDTH + 4;                -- Width of h_rom
constant VIDROM_ADR_WIDTH : integer := COLOR_WIDTH + 2;             -- Width of video ROM address
constant CROM_ADR_WIDTH:integer := VBAND_WIDTH + HRGN_WIDTH;        -- Width of color ROM address  

constant VID_MSB :      integer := VID_WIDTH - 1;                   -- MS bit # of video data path
constant HCNT_MSB :     integer := HCNT_WIDTH - 1;                  -- MS bit # of h_counter
constant VCNT_MSB :     integer := VCNT_WIDTH - 1;                  -- MS bit # of v_counter
constant HRGN_MSB :     integer := HRGN_WIDTH - 1;                  -- MS bit # of h_region counter
constant VRGN_MSB :     integer := VRGN_WIDTH - 1;                  -- MS bit # of v_region counter
constant COLOR_MSB :    integer := COLOR_WIDTH - 1;                 -- MS bit # of color code
constant VBAND_MSB :    integer := VBAND_WIDTH - 1;                 -- MS bit # of vband code
constant V_EVNT_MSB :   integer := V_EVNT_WIDTH - 1;                -- MS bit # of v_next_evnt
constant H_EVNT_MSB :   integer := H_EVNT_WIDTH - 1;                -- MS bit # of h_next_evnt
constant VROM_MSB :     integer := VROM_WIDTH - 1;                  -- MS bit # of v_rom
constant HROM_MSB :     integer := HROM_WIDTH - 1;                  -- MS bit # of h_rom
constant VIDROM_ADR_MSB:integer := VIDROM_ADR_WIDTH - 1;            -- MS bit # of video rom address
constant CROM_ADR_MSB : integer := CROM_ADR_WIDTH - 1;              -- MB bit # of color rom address


--
-- This group of constants controls the total number of lines (both fields) and
-- specifies where the odd and even lines begin and where the vertical blanking
-- periods begin and end.
--
constant V_TOTAL:       std_logic_vector(VCNT_MSB downto 0) -- number of lines
    := std_logic_vector(TO_UNSIGNED(525, VCNT_WIDTH));      -- in two fields

constant V_O_FLD_START: std_logic_vector(VCNT_MSB downto 0) -- first line of
    := std_logic_vector(TO_UNSIGNED(4, VCNT_WIDTH));        -- odd field

constant V_O_ACT_START: std_logic_vector(VCNT_MSB downto 0) -- first active video
    := std_logic_vector(TO_UNSIGNED(20, VCNT_WIDTH));       -- line of odd field 
                                                        
constant V_O_BNK_START: std_logic_vector(VCNT_MSB downto 0) -- first v blanking line
    := std_logic_vector(TO_UNSIGNED(264, VCNT_WIDTH));      -- of odd field
                                                        
constant V_E_FLD_START: std_logic_vector(VCNT_MSB downto 0) -- first line of even
    := std_logic_vector(TO_UNSIGNED(266, VCNT_WIDTH));      -- field

constant V_E_ACT_START: std_logic_vector(VCNT_MSB downto 0) -- first active video
    := std_logic_vector(TO_UNSIGNED(283, VCNT_WIDTH));      -- line of even field

constant V_E_BNK_START: std_logic_vector(VCNT_MSB downto 0) -- first v blanking line
    := std_logic_vector(TO_UNSIGNED(1, VCNT_WIDTH));        -- in even field


--
-- This group of constants controls where the three vertical bands of the
-- color bar pattern begin for each of the two fields.
--
constant BAND1_START_E: std_logic_vector(VCNT_MSB downto 0) -- first line of top
    := std_logic_vector(TO_UNSIGNED(283, VCNT_WIDTH));      -- band, even field

constant BAND1_START_O: std_logic_vector(VCNT_MSB downto 0) -- first line of top
    := std_logic_vector(TO_UNSIGNED(20,  VCNT_WIDTH));      -- band, odd field

constant BAND2_START_E: std_logic_vector(VCNT_MSB downto 0) -- first line of middle
    := std_logic_vector(TO_UNSIGNED(446, VCNT_WIDTH));      -- band, even field

constant BAND2_START_O: std_logic_vector(VCNT_MSB downto 0) -- first line of middle
    := std_logic_vector(TO_UNSIGNED(183, VCNT_WIDTH));      -- band, odd field

constant BAND3_START_E: std_logic_vector(VCNT_MSB downto 0) -- first line of bottom
    := std_logic_vector(TO_UNSIGNED(465, VCNT_WIDTH));      -- band, even field

constant BAND3_START_O: std_logic_vector(VCNT_MSB downto 0) -- first line of bottom
    := std_logic_vector(TO_UNSIGNED(203, VCNT_WIDTH));      -- band, odd field

--
-- This group of constants defines the starting line numbers of the various
-- bands in the RP178 test pattern.
--
constant RP178_CEQ_START_O : std_logic_vector(VCNT_MSB downto 0) -- start of CEQ
    := V_O_ACT_START;                                            -- pattern, odd field

constant RP178_PLL_START_O : std_logic_vector(VCNT_MSB downto 0) -- start of PLL
    := std_logic_vector(TO_UNSIGNED(142,VCNT_WIDTH));            -- pattern, odd field

constant RP178_CEQ_START_E : std_logic_vector(VCNT_MSB downto 0) -- start of CEQ
    := V_E_ACT_START;                                            -- pattern, even field

constant RP178_PLL_START_E : std_logic_vector(VCNT_MSB downto 0) -- start of PLL
    := std_logic_vector(TO_UNSIGNED(403, VCNT_WIDTH));           -- pattern, even field

--
-- This group of constants controls the total number of clocks per line and
-- positions of the two TRS symbols.
--
constant H_TOTAL:       std_logic_vector(HCNT_MSB downto 0) -- total clocks on a 
    := std_logic_vector(TO_UNSIGNED(1716, HCNT_WIDTH));     -- line minus one

constant H_EAV_START:   std_logic_vector(HCNT_MSB downto 0) -- EAV start location
    := std_logic_vector(TO_UNSIGNED(1440, HCNT_WIDTH));         

constant H_SAV_START:   std_logic_vector(HCNT_MSB downto 0) -- SAV start location
    := std_logic_vector(TO_UNSIGNED(1712, HCNT_WIDTH)); 
 
 
--
-- The last sample on the first active line of the first field must be forced to
-- 0x080 when generating the RP 178 pattern. This constant defines the 
-- horizontal sample in which this occurs.
--
constant RP178_LAST_SAMP_FIRST_ACT_LINE : std_logic_vector(HCNT_MSB downto 0)
    := H_EAV_START - 1;

--
-- This group of constants controls the starting horizontal position of each
-- horizontal region. The line is divided up into 12 regions which correspond
-- to possible places for a color bar to begin or end when implementing the
-- SMPTE EG-1 color bars. Some color bars will span several horizontal regions.
--
constant BAR0_START:    std_logic_vector(HCNT_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(0,   HCNT_WIDTH));

constant BAR1_START:    std_logic_vector(HCNT_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(208, HCNT_WIDTH));

constant BAR2_START:    std_logic_vector(HCNT_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(260, HCNT_WIDTH));

constant BAR3_START:    std_logic_vector(HCNT_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(416, HCNT_WIDTH));

constant BAR4_START:    std_logic_vector(HCNT_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(520, HCNT_WIDTH));

constant BAR5_START:    std_logic_vector(HCNT_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(624, HCNT_WIDTH));

constant BAR6_START:    std_logic_vector(HCNT_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(780, HCNT_WIDTH));

constant BAR7_START:    std_logic_vector(HCNT_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(832, HCNT_WIDTH));

constant BAR8_START:    std_logic_vector(HCNT_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(1040,HCNT_WIDTH));

constant BAR9_START:    std_logic_vector(HCNT_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(1108,HCNT_WIDTH));

constant BARA_START:    std_logic_vector(HCNT_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(1180,HCNT_WIDTH));

constant BARB_START:    std_logic_vector(HCNT_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(1248,HCNT_WIDTH));

-- 
-- This group of constants specifies the Y, Cb, and Cr values for each of the
-- colors used in the color bars.
--
constant GRAY_Y:    std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(721, VID_WIDTH));

constant GRAY_CB:   std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(512, VID_WIDTH));

constant GRAY_CR:   std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(512, VID_WIDTH));

constant YELLOW_Y:  std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(674, VID_WIDTH));

constant YELLOW_CB: std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(176, VID_WIDTH));

constant YELLOW_CR: std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(543, VID_WIDTH));

constant CYAN_Y:    std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(581, VID_WIDTH));

constant CYAN_CB:   std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(589, VID_WIDTH));

constant CYAN_CR:   std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(176, VID_WIDTH));

constant GREEN_Y:   std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(534, VID_WIDTH));

constant GREEN_CB:  std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(253, VID_WIDTH));

constant GREEN_CR:  std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(207, VID_WIDTH));

constant MAGENTA_Y: std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(251, VID_WIDTH));

constant MAGENTA_CB:std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(771, VID_WIDTH));

constant MAGENTA_CR:std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(817, VID_WIDTH));

constant RED_Y:     std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(204, VID_WIDTH));

constant RED_CB:    std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(435, VID_WIDTH));

constant RED_CR:    std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(848, VID_WIDTH));

constant BLUE_Y:    std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(111, VID_WIDTH));

constant BLUE_CB:   std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(848, VID_WIDTH));

constant BLUE_CR:   std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(481, VID_WIDTH));

constant BLACK_Y:   std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(64,  VID_WIDTH));

constant BLACK_CB:  std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(512, VID_WIDTH));

constant BLACK_CR:  std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(512, VID_WIDTH));

constant I_Y:   std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(244, VID_WIDTH));

constant I_CB:  std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(612, VID_WIDTH));

constant I_CR:  std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(395, VID_WIDTH));

constant Q_Y:   std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(141, VID_WIDTH));

constant Q_CB:  std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(697, VID_WIDTH));

constant Q_CR:  std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(606, VID_WIDTH));

constant WHITE_Y:   std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(940, VID_WIDTH));

constant WHITE_CB:  std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(512, VID_WIDTH));

constant WHITE_CR:  std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(512, VID_WIDTH));

constant BLKM4_Y:   std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(29,  VID_WIDTH));

constant BLKM4_CB:  std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(512, VID_WIDTH));

constant BLKM4_CR:  std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(512, VID_WIDTH));

constant BLKP4_Y:   std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(99,  VID_WIDTH));

constant BLKP4_CB:  std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(512, VID_WIDTH));

constant BLKP4_CR:  std_logic_vector(VID_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(512, VID_WIDTH));


--
-- This group of constants defines the component video values for the RP178
-- test patterns. Note that the CR and cB values are the same and are called CX.
--
constant CEQ_Y :    std_logic_vector(VID_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(408, VID_WIDTH));

constant CEQ_CX :   std_logic_vector(VID_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(768, VID_WIDTH));

constant CEQ_EX:    std_logic_vector(VID_MSB downto 0)  -- exception for last sample
    := std_logic_vector(TO_UNSIGNED(128, VID_WIDTH));   -- of first active line of
                                                        -- of first field
constant PLL_Y :    std_logic_vector(VID_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(272, VID_WIDTH));

constant PLL_CX :   std_logic_vector(VID_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(512, VID_WIDTH));

constant CEQ : std_ulogic := '0';
constant PLL : std_ulogic := '1';


--
-- This set of constants specifies the encoding of the color values stored in
-- color ROM.
--
constant GRAY :     std_logic_vector(COLOR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(0, COLOR_WIDTH));

constant YELLOW :   std_logic_vector(COLOR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(1, COLOR_WIDTH));

constant CYAN :     std_logic_vector(COLOR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(2, COLOR_WIDTH));

constant GREEN :    std_logic_vector(COLOR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(3, COLOR_WIDTH));

constant MAGENTA :  std_logic_vector(COLOR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(4, COLOR_WIDTH));

constant RED :      std_logic_vector(COLOR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(5, COLOR_WIDTH));

constant BLUE :     std_logic_vector(COLOR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(6, COLOR_WIDTH));

constant BLACK :    std_logic_vector(COLOR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(7, COLOR_WIDTH));

constant ISIG :     std_logic_vector(COLOR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(8, COLOR_WIDTH));

constant QSIG :     std_logic_vector(COLOR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(9, COLOR_WIDTH));

constant WHITE :    std_logic_vector(COLOR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(10, COLOR_WIDTH));

constant BLKM4 :    std_logic_vector(COLOR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(11, COLOR_WIDTH));

constant BLKP4 :    std_logic_vector(COLOR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(12, COLOR_WIDTH));

constant EXTRA1 :   std_logic_vector(COLOR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(13, COLOR_WIDTH));

constant EXTRA2 :   std_logic_vector(COLOR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(14, COLOR_WIDTH));

constant EXTRA3 :   std_logic_vector(COLOR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(15, COLOR_WIDTH));


--
-- The set of constants specifies the encoding of the vband signals.
--
constant BAND_V_BLANK : std_logic_vector(VBAND_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(0, VBAND_WIDTH));       -- v blanking band
constant BAND1 :        std_logic_vector(VBAND_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(1, VBAND_WIDTH));       -- top band
constant BAND2 :        std_logic_vector(VBAND_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(2, VBAND_WIDTH));       -- middle band
constant BAND3 :        std_logic_vector(VBAND_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(3, VBAND_WIDTH));       -- bottom band


-- internal signals
signal h_counter :      std_logic_vector(HCNT_MSB downto 0);      -- horizontal counter
signal v_counter :      std_logic_vector(VCNT_MSB downto 0);      -- vertical counter
signal v_region :       std_logic_vector(VRGN_MSB downto 0);      -- v region counter
signal h_region :       std_logic_vector(HRGN_MSB downto 0);      -- h region counter
signal dout :           std_logic_vector(VID_MSB downto 0);       -- data before out reg
signal color_out :      std_logic_vector(COLOR_MSB downto 0);     -- output of color ROM
signal color_fvh :      std_logic_vector(COLOR_MSB downto 0);     -- ms address bits of video ROM
signal comp :           std_logic_vector(1 downto 0);             -- ls address bits of video ROM
signal vband :          std_logic_vector(VBAND_MSB downto 0);     -- ms address bits of color ROM
signal f :              std_ulogic;                               -- field bit
signal v :              std_ulogic;                               -- v blanking bit
signal h :              std_ulogic;                               -- h blanking bit
signal inc_v :          std_ulogic;                               -- increments v_counter when asserted
signal clr_v :          std_ulogic;                               -- clears v_counter when asserted
signal clr_h :          std_ulogic;                               -- clears h_counter when asserted
signal trs :            std_ulogic;                               -- indicates when TRS is being generated
signal trs_word :       std_logic_vector(1 downto 0);             -- indicates which word of TRS is being output
signal video_out :      std_logic_vector(VID_MSB downto 0);       -- output of video ROM
signal h_next_evnt :    std_logic_vector(H_EVNT_MSB downto 0);    -- h_counter value of next h event
signal h_rom :          std_logic_vector(HROM_MSB downto 0);      -- output of horizontal event ROM
signal v_next_evnt :    std_logic_vector(V_EVNT_MSB downto 0);    -- v_counter value of next v event
signal v_rom :          std_logic_vector(VROM_MSB downto 0);      -- output of vertical event ROM
signal v_evnt_match :   std_ulogic;                               -- output of v event compare
signal h_evnt_match :   std_ulogic;                               -- output of h event compare
signal vrom_addr :      std_logic_vector(VIDROM_ADR_MSB downto 0);-- address to v_rom
signal crom_addr :      std_logic_vector(CROM_ADR_MSB downto 0);  -- address to color rom
signal comp_addr :      std_logic_vector(2 downto 0);             -- comp select bits
signal dout_muxsel :    std_logic_vector(2 downto 0);             -- output mux select bits
signal rp178_out :      std_logic_vector(VID_MSB downto 0);       -- output of RP178 generator
signal mux_out :        std_logic_vector(VID_MSB downto 0);       -- muxed output of both generators
signal ceq_pll :        std_ulogic;                               -- indicates which pattern is active
signal rp178_switch :   std_ulogic;                               -- RP178 pattern switch enable

begin
    
    --
    -- video ROM
    --
    -- The video ROM generates the actual component video values based on the
    -- "color" input value or the TRS symbol's XYZ word based on the FVH bits.
    -- The ROM is organized into sixteen blocks of four words. Each block 
    -- corresponds a color value (or a FVH value). The first word in each block 
    -- is the CB component. The second word is the Y component. The third word 
    -- is the CR component. And, the fourth word is the XYZ word. In the secnod 
    -- half of the ROM, TRS symbols are not present.
    --

    vrom_addr <= color_fvh & comp;

    process(vrom_addr)
    begin
        case vrom_addr is
            when "000000" => video_out <= GRAY_CB;
            when "000001" => video_out <= GRAY_Y;
            when "000010" => video_out <= GRAY_CR;
            when "000011" => video_out <= ('1', others => '0');
            when "000100" => video_out <= YELLOW_CB;
            when "000101" => video_out <= YELLOW_Y;
            when "000110" => video_out <= YELLOW_CR;
            when "000111" => video_out <= ('1','0','0','1','1','1','0','1', others => '0');
            when "001000" => video_out <= CYAN_CB;
            when "001001" => video_out <= CYAN_Y;
            when "001010" => video_out <= CYAN_CR;
            when "001011" => video_out <= ('1','0','1','0','1','0','1','1', others => '0');
            when "001100" => video_out <= GREEN_CB;
            when "001101" => video_out <= GREEN_Y;
            when "001110" => video_out <= GREEN_CR;
            when "001111" => video_out <= ('1','0','1','1','0','1','1','0', others => '0');

            when "010000" => video_out <= MAGENTA_CB;
            when "010001" => video_out <= MAGENTA_Y;
            when "010010" => video_out <= MAGENTA_CR;
            when "010011" => video_out <= ('1','1','0','0','0','1','1','1', others => '0');
            when "010100" => video_out <= RED_CB;
            when "010101" => video_out <= RED_Y;
            when "010110" => video_out <= RED_CR;
            when "010111" => video_out <= ('1','1','0','1','1','0','1','0', others => '0');
            when "011000" => video_out <= BLUE_CB;
            when "011001" => video_out <= BLUE_Y;
            when "011010" => video_out <= BLUE_CR;
            when "011011" => video_out <= ('1','1','1','0','1','1','0','0', others => '0');
            when "011100" => video_out <= BLACK_CB;
            when "011101" => video_out <= BLACK_Y;
            when "011110" => video_out <= BLACK_CR;
            when "011111" => video_out <= ('1','1','1','1','0','0','0','1', others => '0');

            when "100000" => video_out <= I_CB;
            when "100001" => video_out <= I_Y;
            when "100010" => video_out <= I_CR;
            when "100011" => video_out <= (others => '0');
            when "100100" => video_out <= Q_CB;
            when "100101" => video_out <= Q_Y;
            when "100110" => video_out <= Q_CR;
            when "100111" => video_out <= (others => '0');
            when "101000" => video_out <= WHITE_CB;
            when "101001" => video_out <= WHITE_Y;
            when "101010" => video_out <= WHITE_CR;
            when "101011" => video_out <= (others => '0');
            when "101100" => video_out <= BLKM4_CB;
            when "101101" => video_out <= BLKM4_Y;
            when "101110" => video_out <= BLKM4_CR;
            when "101111" => video_out <= (others => '0');

            when "110000" => video_out <= BLKP4_CB;
            when "110001" => video_out <= BLKP4_Y;
            when "110010" => video_out <= BLKP4_CR;
            when "110011" => video_out <= (others => '0');
            when "110100" => video_out <= BLACK_CB;
            when "110101" => video_out <= BLACK_Y;
            when "110110" => video_out <= BLACK_CR;
            when "110111" => video_out <= (others => '0');
            when "111000" => video_out <= BLACK_CB;
            when "111001" => video_out <= BLACK_Y;
            when "111010" => video_out <= BLACK_CR;
            when "111011" => video_out <= (others => '0');
            when "111100" => video_out <= BLACK_CB;
            when "111101" => video_out <= BLACK_Y;
            when "111110" => video_out <= BLACK_CR;
            when "111111" => video_out <= (others => '0');

            when others   => video_out <= (others => '0');
        end case;
    end process;

    --
    -- color ROM
    --
    -- The color ROM converts the vband and h_region values into a color value. 
    -- Thus, it determines the correct color to output based on which region of 
    -- the screen is currently active.
    --

    crom_addr <= vband & h_region;

    process(crom_addr)
    begin
        case crom_addr is
            -- First 16 locations are in the vertical blanking period
            when "000000" => color_out <= BLACK;
            when "000001" => color_out <= BLACK;
            when "000010" => color_out <= BLACK;
            when "000011" => color_out <= BLACK;
            when "000100" => color_out <= BLACK;
            when "000101" => color_out <= BLACK;
            when "000110" => color_out <= BLACK;
            when "000111" => color_out <= BLACK;
            when "001000" => color_out <= BLACK;
            when "001001" => color_out <= BLACK;
            when "001010" => color_out <= BLACK;
            when "001011" => color_out <= BLACK;
            when "001100" => color_out <= BLACK;
            when "001101" => color_out <= BLACK;
            when "001110" => color_out <= BLACK;
            when "001111" => color_out <= BLACK;

            -- Locations 16 thru 31 are in the top color band
            when "010000" => color_out <= BLACK;
            when "010001" => color_out <= BLACK;
            when "010010" => color_out <= BLACK;
            when "010011" => color_out <= GRAY;
            when "010100" => color_out <= YELLOW;
            when "010101" => color_out <= YELLOW;
            when "010110" => color_out <= CYAN;
            when "010111" => color_out <= CYAN;
            when "011000" => color_out <= GREEN;
            when "011001" => color_out <= GREEN;
            when "011010" => color_out <= MAGENTA;
            when "011011" => color_out <= RED;
            when "011100" => color_out <= RED;
            when "011101" => color_out <= RED;
            when "011110" => color_out <= BLUE;
            when "011111" => color_out <= BLUE;

            -- Locations 32 thru 47 are in the middle color band
            when "100000" => color_out <= BLACK;
            when "100001" => color_out <= BLACK;
            when "100010" => color_out <= BLACK;
            when "100011" => color_out <= BLUE;
            when "100100" => color_out <= BLACK;
            when "100101" => color_out <= BLACK;
            when "100110" => color_out <= MAGENTA;
            when "100111" => color_out <= MAGENTA;
            when "101000" => color_out <= BLACK;
            when "101001" => color_out <= BLACK;
            when "101010" => color_out <= CYAN;
            when "101011" => color_out <= BLACK;
            when "101100" => color_out <= BLACK;
            when "101101" => color_out <= BLACK;
            when "101110" => color_out <= GRAY;
            when "101111" => color_out <= GRAY;

            -- Locations 48 through 63 are in the bottom color band
            when "110000" => color_out <= BLACK;
            when "110001" => color_out <= BLACK;
            when "110010" => color_out <= BLACK;
            when "110011" => color_out <= ISIG;
            when "110100" => color_out <= ISIG;
            when "110101" => color_out <= WHITE;
            when "110110" => color_out <= WHITE;
            when "110111" => color_out <= QSIG;
            when "111000" => color_out <= QSIG;
            when "111001" => color_out <= BLACK;
            when "111010" => color_out <= BLACK;
            when "111011" => color_out <= BLKM4;
            when "111100" => color_out <= BLACK;
            when "111101" => color_out <= BLKP4;
            when "111110" => color_out <= BLACK;
            when "111111" => color_out <= BLACK;

            when others   => color_out <= BLACK;
        end case;
    end process;

    --
    -- vertical state machine
    --

    -- generate the line counter comparator
    v_evnt_match <= '1' when v_counter = v_next_evnt else '0';

    --
    -- v_region counter
    --
    -- This counter contains the current state value of the vertical state 
    -- machine. It increments at the end of line as indicated by the inc_v 
    -- signal from the horizontal state machine if the current contents of the 
    -- v_counter match the v_next_evnt ROM's output.
    --
    
    process(clk, rst)
    begin
        if (rst = '1') then
            v_region <= (others => '1');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (h_counter(1 downto 0) = "11" and inc_v = '1') then
                    if (clr_v = '1') then
                        v_region <= (others => '0');
                    elsif (v_evnt_match = '1') then
                        v_region <= v_region + 1;
                    end if;
                end if;
            end if;
        end if;
    end process;
        
    --
    -- v_rom
    --
    -- This ROM generates the current state values for the vertical state 
    -- machine. It also generates the line number of the next vertical event. 
    --
    process(v_region)
    begin
        case v_region is
    --                                next event            vfclr  band
            when "0000" => v_rom <= ((V_O_FLD_START - 1) & "110" & BAND_V_BLANK);  -- start v blank
            when "0001" => v_rom <= ((V_O_ACT_START - 1) & "100" & BAND_V_BLANK);  -- start fld 1
            when "0010" => v_rom <= ((BAND2_START_O - 1) & "000" & BAND1);         -- start band 1
            when "0011" => v_rom <= ((BAND3_START_O - 1) & "000" & BAND2);         -- start band 2
            when "0100" => v_rom <= ((V_O_BNK_START - 1) & "000" & BAND3);         -- start band 3
            when "0101" => v_rom <= ((V_E_FLD_START - 1) & "100" & BAND_V_BLANK);  -- start v blank
            when "0110" => v_rom <= ((V_E_ACT_START - 1) & "110" & BAND_V_BLANK);  -- start fld 2
            when "0111" => v_rom <= ((BAND2_START_E - 1) & "010" & BAND1);         -- start band 1
            when "1000" => v_rom <= ((BAND3_START_E - 1) & "010" & BAND2);         -- start band 2
            when "1001" => v_rom <= ((V_TOTAL       - 1) & "010" & BAND3);         -- start band 3
            when "1010" => v_rom <= ( V_TOTAL            & "011" & BAND3);         -- last line fld 2
            when others => v_rom <= ( V_TOTAL            & "011" & BAND3);         -- default
        end case;
    end process;

    --
    -- assign the individual fields from the v_envt_rom output
    --
    v_next_evnt <= v_rom(V_EVNT_MSB + VBAND_WIDTH + 3 downto VBAND_WIDTH + 3);
    v           <= v_rom(VBAND_WIDTH + 2);
    f           <= v_rom(VBAND_WIDTH + 1);
    clr_v       <= v_rom(VBAND_WIDTH);
    vband       <= v_rom(VBAND_MSB downto 0);


    --
    -- horizontal state machine
    --

    -- generate the h_counter comparator
    h_evnt_match <= '1' when h_counter(HCNT_MSB downto 2) = h_next_evnt else '0';


    -- h_region counter
    --
    -- The h_region counter contains the current state of the horizontal state 
    -- machine. This counter increments when the contents of the h_counter match
    -- the value generated by the h_rom.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            h_region <= (others => '1');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (h_counter(1 downto 0) = "11" and h_evnt_match = '1') then
                    h_region <= h_region + 1;
                end if;
            end if;
        end if;
    end process;        
        
    --
    -- h_rom
    --
    -- Based on the current horizontal state contained in the h_region, this ROM
    -- generates the current state values for the horzintal state machine. It 
    -- also generates the horizontal count value to match against the h_counter 
    -- for the next state transition.
    --
    process(h_region)
    begin
        case h_region is
    --                                       next event                      htic    bar
    --                                                                        rnl
    --                                                                        scr
    --                                                                         vh
            when "0000" => h_rom <= ( H_EAV_START(HCNT_MSB downto 2)      & "1100"); -- EAV
            when "0001" => h_rom <= ((H_SAV_START(HCNT_MSB downto 2) - 1) & "1000"); -- h blanking
            when "0010" => h_rom <= ((H_TOTAL(HCNT_MSB downto 2)     - 1) & "0101"); -- SAV
            when "0011" => h_rom <= ((BAR1_START(HCNT_MSB downto 2)  - 1) & "0000"); -- BAR 0
            when "0100" => h_rom <= ((BAR2_START(HCNT_MSB downto 2)  - 1) & "0000"); -- BAR 1
            when "0101" => h_rom <= ((BAR3_START(HCNT_MSB downto 2)  - 1) & "0000"); -- BAR 2
            when "0110" => h_rom <= ((BAR4_START(HCNT_MSB downto 2)  - 1) & "0000"); -- BAR 3
            when "0111" => h_rom <= ((BAR5_START(HCNT_MSB downto 2)  - 1) & "0000"); -- BAR 4
            when "1000" => h_rom <= ((BAR6_START(HCNT_MSB downto 2)  - 1) & "0000"); -- BAR 5
            when "1001" => h_rom <= ((BAR7_START(HCNT_MSB downto 2)  - 1) & "0000"); -- BAR 6
            when "1010" => h_rom <= ((BAR8_START(HCNT_MSB downto 2)  - 1) & "0000"); -- BAR 7
            when "1011" => h_rom <= ((BAR9_START(HCNT_MSB downto 2)  - 1) & "0000"); -- BAR 8
            when "1100" => h_rom <= ((BARA_START(HCNT_MSB downto 2)  - 1) & "0000"); -- BAR 9
            when "1101" => h_rom <= ((BARB_START(HCNT_MSB downto 2)  - 1) & "0000"); -- BAR A
            when "1110" => h_rom <= ((H_EAV_START(HCNT_MSB downto 2) - 2) & "0000"); -- BAR B
            when others => h_rom <= ((H_EAV_START(HCNT_MSB downto 2) - 1) & "0010"); -- extra state
        end case;
    end process;

    h_next_evnt <= h_rom(H_EVNT_WIDTH + 3 downto 4);
    h           <= h_rom(3);
    trs         <= h_rom(2);
    inc_v       <= h_rom(1);
    clr_h       <= h_rom(0);

    --
    -- v_counter
    --
    -- The v_counter keeps track of the current line number. It increments 
    -- during the generation of the EAV signal since SMPTE 125M specifies that 
    -- each line begins with the EAV symbol (although the pixel counter is not 0
    -- at this point). To match the SMPTE 125M specification, the count of the 
    -- first line is zero, not one. On reset, the line counter is set to the 
    -- last line. It will increment to the first line one the first clock after 
    -- the reset is negated.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            v_counter <= V_TOTAL;
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (h_counter(1 downto 0) = "11" and inc_v = '1') then
                    if (clr_v = '1') then
                        v_counter <= (0 => '1', others => '0');
                    else
                        v_counter <= v_counter + 1;
                    end if;
                end if;
            end if;
        end if;
    end process;

    --
    -- h_counter
    --
    -- The h_counter keeps track of the current vertical position on the screen.
    -- The h_counter counts from 0 (which is the first active video position) to
    -- the maximum count for the line (which corresponds to last word of the 
    -- SAV). On reset, the h_counter is set to the EAV position so that an EAV 
    -- is the first thing generated after reset.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            h_counter <= H_EAV_START - 1;
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (clr_h = '1' and h_counter(1 downto 0) = "11") then
                    h_counter <= (others => '0');
                else
                    h_counter <= h_counter + 1;
                end if;
            end if;
        end if;
    end process;

    --
    -- This logic implements a mux on the output of the color ROM. If a TRS is
    -- to be generated, the FVH bits are output instead of the color_out value.
    with trs select
        color_fvh <= ('0' & f & v & h) when '1',
                     color_out         when others;

    --
    -- This logic generates the two LS address bits into the video ROM. These
    -- bits determine which video component should be generated (Y, Cb, or Cr).
    -- If a TRS is being generated, these bits force the video ROM to generated
    -- the XYZ word.
    --
    comp_addr <= trs & h_counter(1 downto 0);

    process(comp_addr)
    begin
        case comp_addr is
            when "000"  => comp <= "00";        -- Cb
            when "001"  => comp <= "01";        -- Y
            when "010"  => comp <= "10";        -- Cr
            when "011"  => comp <= "01";        -- Y
            when others => comp <= "11";        -- TRS
        end case;
    end process;

    --
    -- This logic implements a mux on the output of the video ROM. Normally, the 
    -- output of the video ROM is sent to the output register. If a TRS is being
    -- generated, this MUX can force the output to be all zeros or all ones as
    -- required.
    --
    trs_word <= h_counter(1 downto 0);
    dout_muxsel <= trs & trs_word;

    process(dout_muxsel, video_out)
    begin
        case dout_muxsel is
            when "100"  => dout <= (others => '1');
            when "101"  => dout <= (others => '0');
            when "110"  => dout <= (others => '0');
            when others => dout <= video_out;
        end case;
    end process;


    -- This code generates the RP178 video value. This is implemented as two
    -- processes. The first defines a registered signal called ceq_pll. This
    -- signal defines whether the cable equalization pattern or the PLL pattern
    -- are to be generated. The value of ceq_pll is only updated during the
    -- generation of the SAV signal and only when the v_counter has reached one
    -- of the transition lines between PLL and CEQ patterns.
    --
    -- The second process generates the Y and CB/CR values based on the value
    -- of the ceq_pll signal. Note that RP 178 requires that the last sample
    -- (1439 for NTSC) on the first active line (20 for NTSC) of the first field
    -- must be set to hex 080. This forces an odd nubmer of 1 bits in the frame.
    --
    rp178_switch <= '1' when 
        (ce = '1' and trs = '1' and h = '0' and h_counter(1 downto 0) = "11") 
        else '0';

    process(clk, rst)
    begin
        if (rst = '1') then
            ceq_pll <= PLL;
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
	            if (rp178_switch = '1') then
	                if (v_counter = RP178_CEQ_START_O) then
	                    ceq_pll <= CEQ;
	                elsif (v_counter = RP178_PLL_START_O) then
	                    ceq_pll <= PLL;
	                elsif (v_counter = RP178_CEQ_START_E) then
	                    ceq_pll <= CEQ;
	                elsif (v_counter = RP178_PLL_START_E) then
	                    ceq_pll <= PLL;
	                end if;
	            end if;
			end if;
        end if;
    end process; 

    process(ceq_pll, h_counter, v_counter)
    begin
        if (ceq_pll = CEQ) then
            if (v_counter = RP178_CEQ_START_O and
                h_counter = RP178_LAST_SAMP_FIRST_ACT_LINE) then
                rp178_out <= CEQ_EX;
            elsif (h_counter(0) = '0') then
                rp178_out <= CEQ_CX;
            else
                rp178_out <= CEQ_Y;
            end if;
        else
            if (h_counter(0) = '0') then
                rp178_out <= PLL_CX;
            else
                rp178_out <= PLL_Y;
            end if;
        end if;
    end process;


    --
    -- The following code multiplexes the EG1 color bar generator's output with
    -- the RP178 test pattern genrator's output. RP178 is output if the pattern
    -- input is high. However, the EG1 generator creates the TRS symbols which
    -- must be output even when RP178 is being generated.
    --  
    mux_out <= rp178_out when (pattern = '1' and trs = '0' and h = '0' and v = '0')
        else dout;


    --
    -- This code implements the output registers.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            q      <= (others => '0');
            h_sync <= '0';
            v_sync <= '0';
            field  <= '0';
        elsif (clk'event and clk ='1') then
            if (ce = '1') then
                q      <= std_ulogic_vector(mux_out);
                h_sync <= h or trs;
                v_sync <= v;
                field  <= f;
            end if;
        end if;
    end process;

end synth;
