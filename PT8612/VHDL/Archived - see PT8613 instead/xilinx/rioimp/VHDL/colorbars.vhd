-------------------------------------------------------------------------------- 
-- Copyright (c) 2002 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: colorbars.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2004-08-27 13:16:12-06 $
-- /___/   /\    Date Created: January 7, 2002 
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: colorbars.vhd,rcs $
-- Revision 1.0  2004-08-27 13:16:12-06  jsnow
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
-- This colorbar generator will generate color bars for the SMPTE 125M video
-- standard which specifies 525 line 4:2:2 component digital video. All video is
-- generated at 10-bit resolution.
-- 
-- The color bars comply with SMPTE EG-1 standard color bars with the exception 
-- of the bottom band which simply contains one white bar and one black bar. The
-- upper two bands are EG-1 compliant containing 7 color bars each. The top band
-- contains the following bars, in order from left to right across the screen:
-- white, yellow, cyan, green, magenta, red, and blue. The middle band contains
-- the following bars from left to right: blue, black, magenta, black, cyan,
-- black, white.
-- 
-- This module generates all the correct TRS characters to mark the start of
-- active video (SAV) and the end of active video (EAV). Each TRS character is
-- four words long. The format of the TRS is shown below:
--  
--  word 1:     0x3ff
--  word 2:     0x000
--  word 3:     0x000
--  word 4:     XYZ (see below)
-- 
-- The XYZ word specifies the field, horizontal blanking, and vertical blanking
-- and contains some error detection bits. The legal values for the XYZ word are
-- listed below:
-- 
--  F V H   XYZ (hex)
--  0 0 0   0x200
--  0 0 1   0x274
--  0 1 0   0x29c
--  0 1 1   0x2d8
--  1 0 0   0x31c
--  1 0 1   0x368
--  1 1 0   0x3b0
--  1 1 1   0x3c4
-- 
-- F is 1 for even fields and 0 for odd fields. V is 1 during the vertical
-- blanking period, 0 otherwise. H is 1 during the horizontal blanking period, 0
-- otherwise.
-- 
-- The module is driven by two counters, the line_counter that counts vertical
-- lines from 1 to 525 and the pixel_counter that counts horizontal pixels from 
-- 0 to 1715.
-- 
-- For convenience in debugging video applications, the module also outputs the
-- H, V, and F bits.
-- 
-- This generator basically consits of two ROM based state machines, one for the
-- vertical direction and one for the horizontal direction. Changing the ROM
-- contents and the associated video timing parameters will affect the test
-- pattern that is generated.
-- 
-- The vertical state machine is responsible for controlling the generation of 
-- the V and F bits and for determining when the transitions occur between the 
-- various color bar bands on the screen.
-- 
-- The vertical state machine is driven by the v_evnt_cntr counter and a ROM
-- called v_evnt_rom. The v_evnt_cntr generates the address into the ROM. There 
-- is also a line counter called v_counter which contains the current line 
-- number. The output of the v_next_evnt field of the ROM is continuously 
-- compared to the current value of the v_counter. When a match occurs, the 
-- v_evnt_cntr is incremented and the vertical state machine moves to the next 
-- state. The v_evnt_rom generates the V and F bits, the clr_v signal that 
-- causes both the v_counter and the v_evnt_cntr to reset at the end of the 
-- current line, and the vband values which specifies which vertical region 
-- (color bar band) is currently active.
-- 
-- It should be noted that the v_next_evnt field of the vertical state machine's
-- ROM contains the line number immediately before the event transition. For
-- example, if it is desired for the state machine to generate a new set of
-- outputs on line number 4, then the v_next_evnt field must be set to 3. When 
-- the v_counter reaches line 3 a match occurs which causes the v_evnt_cntr to 
-- be incremented at the beginning of line 4, generating a new set of outputs 
-- from the v_evnt_rom. The same is true for the horizontal state machine 
-- described below.
-- 
-- The horizontal state machine is responsible for controlling the generation of
-- the H bit, for incrementing the v_counter at the beginning of the horizontal
-- blanking period, for determining when to generate the TRS symbols, and for
-- determining which color bar should be generated.
-- 
-- The horizontal state machine has a structure virtually identical to the
-- vertical state machine with a h_region counter, h_rom, and h_counter. 
-- The output of the h_rom are the H bit, a trs signal which indicates when
-- a TRS symbol is to be generated, an inc_v signal which causes the v_counter 
-- to increment, a clr_h signal which resets the h_counter (but not, in this 
-- case, the h_region counter), and the h_region value which specifies which 
-- horizontal region of the screen (color bar) is currently active. Note that 
-- the clr_h signal does not reset the h_region counter. This means that 
-- h_region counter must always cycle through  exactly 16 states across one 
-- line. If there are not 16 events required per line, dummy states must be 
-- inserted for padding.
-- 
-- The contents of the horizontal and vertical state machine ROMs are basically
-- all controlled by the horizontal and vertical parameters. Changing these
-- parameters will change the number of lines, horizontal pixels, and color bar
-- positions.
-- 
-- The h_region and vband values generated by the two state machines are used as
-- the address into the color ROM which determines which of eight colors should 
-- be generated for the current pixel. The color generated by the color ROM is 
-- used as an address into the video ROM which generates the actual YCbCr 
-- component video values.
-- 
-- When a TRS symbol is being generated, the address into the video ROM consists
-- of the F, V, and H bits. The video ROM generates the correct XYZ word for the
-- TRS symbol.
-- 
-- It is important to note that this colorbar generator produces does not have a
-- low pass filter on the video output. Therefore, the edge rates of the
-- transitions between color bars are too high to meet normal video standards. 
-- This colorbar generator is intended to be used in conjunction with a low pass
-- video filter that will remove the high frequency content. 
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

entity colorbars_vhd is
    generic (
        VID_WIDTH : integer := 10
    );
    port (
        clk:        in  std_ulogic;                                 -- clock input
        rst:        in  std_ulogic;                                 -- async reset input
        ce:         in  std_ulogic;                                 -- clock enable input
        q:          out std_ulogic_vector(VID_WIDTH - 1 downto 0);  -- video output
        h_sync:     out std_ulogic;                                 -- horizontal sync out
        v_sync:     out std_ulogic;                                 -- vertical sync ou
        field:      out std_ulogic                                  -- field output
    );
end colorbars_vhd;

architecture synth of colorbars_vhd is

--
-- This group of constants controls the widths of various fields in the design.
-- Note that if the generic VID_WIDTH is modified, the component values of
-- the various colors must be modified accordingly.
--
constant HCNT_WIDTH :   integer := 11;                              -- Width of h_counter
constant VCNT_WIDTH :   integer := 10;                              -- Width of v_counter
constant HRGN_WIDTH :   integer := 4;                               -- Width of h_region counter
constant VRGN_WIDTH :   integer := 4;                               -- Width of v_region counter
constant COLOR_WIDTH :  integer := 3;                               -- Width of color code
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


--
-- The set of constants specifies the encoding of the vband signals.
--
constant BAND_V_BLANK : std_logic_vector(VBAND_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(0, VBAND_WIDTH));             -- v blanking band

constant BAND1 :        std_logic_vector(VBAND_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(1, VBAND_WIDTH));             -- top band

constant BAND2 :        std_logic_vector(VBAND_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(2, VBAND_WIDTH));             -- middle band

constant BAND3 :        std_logic_vector(VBAND_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(3, VBAND_WIDTH));             -- bottom band


-------------------------------------------------------------------------------
-- Signal Definitions
--
signal h_counter :      std_logic_vector(HCNT_MSB downto 0);      -- horizontal counter
signal v_counter :      std_logic_vector(VCNT_MSB downto 0);      -- vertical counter
signal v_region :       std_logic_vector(VRGN_MSB downto 0);      -- v region counter
signal h_region :       std_logic_vector(HRGN_MSB downto 0);      -- h region counter
signal dout :           std_logic_vector(VID_MSB downto 0);       -- data before output register
signal color_out :      std_logic_vector(COLOR_MSB downto 0);     -- output of color ROM
signal color_fvh :      std_logic_vector(COLOR_MSB downto 0);     -- ms address bits of video ROM
signal comp :           std_logic_vector(1 downto 0);             -- ls address bits of video ROM
signal vband :          std_logic_vector(VBAND_MSB downto 0);     -- ms address bits of color ROM
signal f :              std_ulogic;                               -- field bit
signal v :              std_ulogic;                               -- v blanking bit
signal h :              std_ulogic;                               -- h blanking bit
signal inc_v :          std_ulogic;                               -- signal to increment the v_counter
signal clr_v :          std_ulogic;                               -- signal to clear the v_counter
signal clr_h :          std_ulogic;                               -- signal to clear the h_counter
signal trs :            std_ulogic;                               -- indicates when TRS is being generated
signal trs_word :       std_logic_vector(1 downto 0);             -- indicates which word of TRS is being output
signal video_out :      std_logic_vector(VID_MSB downto 0);       -- output of video ROM
signal h_next_evnt :    std_logic_vector(H_EVNT_MSB downto 0);    -- h_counter value of next h event
signal h_rom :          std_logic_vector(HROM_MSB downto 0);      -- output of horz event ROM
signal v_next_evnt :    std_logic_vector(V_EVNT_MSB downto 0);    -- v_counter value of next vert event
signal v_rom :          std_logic_vector(VROM_MSB downto 0);      -- output of vert event ROM
signal v_evnt_match :   std_ulogic;                               -- output of vert event comparator
signal h_evnt_match :   std_ulogic;                               -- output of horz event comparator
signal vrom_addr :      std_logic_vector(VIDROM_ADR_MSB downto 0);-- address to vert rom
signal crom_addr :      std_logic_vector(CROM_ADR_MSB downto 0);  -- address to color rom
signal comp_addr :      std_logic_vector(2 downto 0);             -- comp select bits
signal dout_muxsel :    std_logic_vector(2 downto 0);             -- output mux select bits


begin
    
    --
    -- video ROM
    --
    -- The video ROM generates the actual component video values based on the
    -- "color" input value or the TRS symbol's XYZ word based on the FVH bits.
    -- The ROM is organized into eight blocks of four words. Each block 
    -- corresponds to a color value (or a FVH value). The first word in each 
    -- block is the CB component. The second word is the Y component. The third 
    -- word is the CR component. And, the fourth word is the XYZ word.
    --
    vrom_addr <= color_fvh & comp;

    process(vrom_addr)
    begin
        case vrom_addr is
            when "00000" => video_out <= GRAY_CB;
            when "00001" => video_out <= GRAY_Y;
            when "00010" => video_out <= GRAY_CR;
            when "00011" => video_out <= ('1', others => '0');
            when "00100" => video_out <= YELLOW_CB;
            when "00101" => video_out <= YELLOW_Y;
            when "00110" => video_out <= YELLOW_CR;
            when "00111" => video_out <= ('1','0','0','1','1','1','0','1', others => '0');
            when "01000" => video_out <= CYAN_CB;
            when "01001" => video_out <= CYAN_Y;
            when "01010" => video_out <= CYAN_CR;
            when "01011" => video_out <= ('1','0','1','0','1','0','1','1', others => '0');
            when "01100" => video_out <= GREEN_CB;
            when "01101" => video_out <= GREEN_Y;
            when "01110" => video_out <= GREEN_CR;
            when "01111" => video_out <= ('1','0','1','1','0','1','1','0', others => '0');
            when "10000" => video_out <= MAGENTA_CB;
            when "10001" => video_out <= MAGENTA_Y;
            when "10010" => video_out <= MAGENTA_CR;
            when "10011" => video_out <= ('1','1','0','0','0','1','1','1', others => '0');
            when "10100" => video_out <= RED_CB;
            when "10101" => video_out <= RED_Y;
            when "10110" => video_out <= RED_CR;
            when "10111" => video_out <= ('1','1','0','1','1','0','1','0', others => '0');
            when "11000" => video_out <= BLUE_CB;
            when "11001" => video_out <= BLUE_Y;
            when "11010" => video_out <= BLUE_CR;
            when "11011" => video_out <= ('1','1','1','0','1','1','0','0', others => '0');
            when "11100" => video_out <= BLACK_CB;
            when "11101" => video_out <= BLACK_Y;
            when "11110" => video_out <= BLACK_CR;
            when others  => video_out <= ('1','1','1','1','0','0','0','1', others => '0');
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
            when "110011" => color_out <= GRAY;
            when "110100" => color_out <= GRAY;
            when "110101" => color_out <= GRAY;
            when "110110" => color_out <= GRAY;
            when "110111" => color_out <= GRAY;
            when "111000" => color_out <= GRAY;
            when "111001" => color_out <= BLACK;
            when "111010" => color_out <= BLACK;
            when "111011" => color_out <= BLACK;
            when "111100" => color_out <= BLACK;
            when "111101" => color_out <= BLACK;
            when "111110" => color_out <= BLACK;
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
    -- This is essentially the current state value of the vertical state machine.
    -- It increments at the end of line as indicated by the inc_v signal from 
    -- the horizontal state machine if the current contents of the v_counter 
    -- match the v_next_evnt ROM's output.
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
    -- v_envt_rom
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
    -- the value generated by the h_next_evnt ROM.
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
    -- Based on the current horizontal state contained in the h_region counter, 
    -- this ROM generates the current state values for the horzintal state 
    -- machine. It also generates the horizontal count value to match against 
    -- the h_counter for the next state transition.
    --
    process(h_region)
    begin
        case h_region is
    --                                 next event                            htic
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
        color_fvh <= (f & v & h) when '1',
                     color_out   when others;

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
                q      <= std_ulogic_vector(dout);
                h_sync <= h or trs;
                v_sync <= v;
                field  <= f;
            end if;
        end if;
    end process;

end synth;