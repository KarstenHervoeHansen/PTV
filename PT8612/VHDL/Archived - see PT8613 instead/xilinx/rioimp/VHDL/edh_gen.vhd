-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: edh_gen.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2005-01-18 10:42:25-07 $
-- /___/   /\    Date Created: Jan 5, 2005
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: edh_gen.vhd,rcs $
-- Revision 1.0  2005-01-18 10:42:25-07  jsnow
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
-- This module generates EDH packets and inserts them into the video stream. The
-- edh_crc module calculates CRC checkwords for each field. These CRC checkwords
-- and the EDH error flags from the edh_flags (not instanced here) are used by
-- edh_tx to generated new EDH packets for insertion into the video stream.
-- 
-- The module implements an output register for the output video stream and all
-- video timing signals.
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

use work.anc_edh_pkg.all;

entity edh_gen is
    port (
        -- inputs
        clk:            in  std_ulogic;     -- clock input
        ce:             in  std_ulogic;     -- clock enable
        rst:            in  std_ulogic;     -- async reset input
        vid_in:         in  video_type;     -- input video port 
        std:            in  vidstd_type;    -- video standard
        locked:         in  std_ulogic;     -- flywheel is locked
        flag_bus:       in  edh_flgset_type;-- input flag bus from edh_flags
        edh_missing:    in  std_ulogic;     -- indicates no EDH packet was found in input video
        in_trs:         in  std_ulogic;     -- asserted during TRS symbol
        in_f:           in  std_ulogic;     -- field bit
        in_v:           in  std_ulogic;     -- vertical blanking bit
        in_h:           in  std_ulogic;     -- horizontal blanking bit
        in_hcnt:        in  hpos_type;      -- current horizontal position
        in_vcnt:        in  vpos_type;      -- current vertical position
        in_sync_switch: in  std_ulogic;     -- sync switching interval indicator
        in_eav_next:    in  std_ulogic;     -- asserted when next word is 1st word of EAV
        in_sav_next:    in  std_ulogic;     -- asserted when next word is 1st word of SAV
        in_xyz_word:    in  std_ulogic;     -- asserted during XYZ word of TRS
        in_anc_next:    in  std_ulogic;     -- asserted when next word is 1st word of ANC packet
        in_edh_next:    in  std_ulogic;     -- asserted when next word is 1st word of EDH packet
        in_edh_loc:     in  std_ulogic;     -- next word is 1st word of an EDH packet location

        -- outputs
        vid_out:        out video_type;     -- output video data
        ap_flag_word:   out std_ulogic;     -- selects the active-picture error flag set for flag_bus
        ff_flag_word:   out std_ulogic;     -- selects the full-field error flag set for flag_bus
        anc_flag_word:  out std_ulogic;     -- selects the anc error flag set for the flag_bus
        edh_packet:     out std_ulogic;     -- asserted during all words of a generated EDH packet
        out_trs:        out std_ulogic;     -- asserted during TRS symbols
        out_f:          out std_ulogic;     -- field indicator
        out_v:          out std_ulogic;     -- vertical blanking indicator
        out_h:          out std_ulogic;     -- horizontal blanking indicator
        out_hcnt:       out hpos_type;      -- horizontal position
        out_vcnt:       out vpos_type;      -- vertical position        
        out_sync_switch:out std_ulogic;     -- synchronous swithcing interval indicator     
        out_eav_next:   out std_ulogic;     -- asserted when next word is first word of an EAV      
        out_sav_next:   out std_ulogic;     -- asserted when next word is first word of an SAV      
        out_xyz_word:   out std_ulogic;     -- asserted during the XYZ word of a TRS symbol
        out_anc_next:   out std_ulogic;     -- asserted when next word is first word of an ANC packet       
        out_edh_next:   out std_ulogic);    -- asserted when next word is first word of an EDH packet
end;

architecture synth of edh_gen is

-------------------------------------------------------------------------------
-- Constant definitions
--      

-------------------------------------------------------------------------------
-- Signal definitions
--
signal ap_crc:          crc16_type;         -- calculated active-picture CRC value
signal ap_crc_valid:    std_ulogic;         -- calculated active-picture CRC valid
signal ff_crc:          crc16_type;         -- calculated full-field CRC value
signal ff_crc_valid:    std_ulogic;         -- calculated full-field CRC valid
signal tx_vid_out:      video_type;         -- video output of edh_tx module
signal tx_edh_packet:   std_ulogic;         -- edh_packet output of edh_tx module
 
--------------------------------------------------------------------------------
-- Component definitions
--
component edh_crc
    port (
        clk:            in  std_ulogic;     -- clock input
        ce:             in  std_ulogic;     -- clock enable
        rst:            in  std_ulogic;     -- async reset input
        f:              in  std_ulogic;     -- field bit
        h:              in  std_ulogic;     -- horizontal blanking bit
        eav_next:       in  std_ulogic;     -- asserted when next sample begins EAV symbol
        xyz_word:       in  std_ulogic;     -- asserted when current word is the XYZ word of a TRS
        vid_in:         in  video_type;     -- video data
        vcnt:           in  vpos_type;      -- vertical line count
        std:            in  vidstd_type;    -- indicates the video standard
        locked:         in  std_ulogic;     -- asserted when flywheel is locked
        ap_crc:         out crc16_type;     -- calculated active picture CRC
        ap_crc_valid:   out std_ulogic;     -- asserted when AP CRC is valid
        ff_crc:         out crc16_type;     -- calculated full-frame CRC
        ff_crc_valid:   out std_ulogic);    -- asserted when FF CRC is valid
end component;

component edh_tx
    port(
        clk:            in  std_ulogic;     -- clock input
        ce:             in  std_ulogic;     -- clock enable
        rst:            in  std_ulogic;     -- async reset input
        vid_in:         in  video_type;     -- video data
        edh_next:       in  std_ulogic;     -- asserted when next sample begins EDH packet
        edh_missing:    in  std_ulogic;     -- received EDH packet is missing
        ap_crc_valid:   in  std_ulogic;     -- asserted when AP CRC is valid
        ap_crc:         in  crc16_type;     -- calculated active picture CRC
        ff_crc_valid:   in  std_ulogic;     -- asserted when FF CRC is valid
        ff_crc:         in  crc16_type;     -- calculated full-frame CRC
        flags_in:       in  edh_flgset_type;-- bus that carries AP, FF, and ANC flags
        ap_flag_word:   out std_ulogic;     -- asserted during AP flag word in EDH packet
        ff_flag_word:   out std_ulogic;     -- asserted during FF flag word in EDH packet
        anc_flag_word:  out std_ulogic;     -- asserted during ANC flag word in EDH packet
        edh_packet:     out std_ulogic;     -- asserted during all words of EDH packet
        edh_vid:        out video_type);    -- generated EDH packet data
end component;

begin
    
    --
    -- edh_crc module
    --
    -- This module computes the CRC values for the output video stream. Also, 
    -- the module generates valid signals for both CRC values based on the 
    -- locked signal. If locked rises during a field, the CRC is considered to 
    -- be invalid.
    --

    CRC : edh_crc
        port map (
        clk             => clk,
        ce              => ce,
        rst             => rst,
        f               => in_f,
        h               => in_h,
        eav_next        => in_eav_next,
        xyz_word        => in_xyz_word,
        vid_in          => vid_in,
        vcnt            => in_vcnt,
        std             => std,
        locked          => locked,
        ap_crc          => ap_crc,
        ap_crc_valid    => ap_crc_valid,
        ff_crc          => ff_crc,
        ff_crc_valid    => ff_crc_valid);

    --
    -- edh_tx module
    --
    -- This module generates a new EDH packet based on the calculated CRC words
    -- and the incoming and local flags.
    --
    TX : edh_tx
        port map (
        clk             => clk,
        ce              => ce,
        rst             => rst,
        vid_in          => vid_in,
        edh_next        => in_edh_loc,
        edh_missing     => edh_missing,
        ap_crc_valid    => ap_crc_valid,
        ap_crc          => ap_crc,
        ff_crc_valid    => ff_crc_valid,
        ff_crc          => ff_crc,
        flags_in        => flag_bus,
        ap_flag_word    => ap_flag_word,
        ff_flag_word    => ff_flag_word,
        anc_flag_word   => anc_flag_word,
        edh_packet      => tx_edh_packet,
        edh_vid         => tx_vid_out);

    --
    -- Output registers for the video stream and video timing signals
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            vid_out <= (others => '0');     
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                vid_out <= tx_vid_out;
            end if;
        end if;
    end process;

    process(clk, rst)
    begin
        if (rst = '1') then
            edh_packet <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                edh_packet <= tx_edh_packet;
            end if;
        end if;
    end process;

    process(clk, rst)
    begin
        if (rst = '1') then
            out_trs <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                out_trs <= in_trs;
            end if;
        end if;
    end process;

    process(clk, rst)
    begin
        if (rst = '1') then
            out_f <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                out_f <= in_f;
            end if;
        end if;
    end process;

    process(clk, rst)
    begin
        if (rst = '1') then
            out_v <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                out_v <= in_v;
            end if;
        end if;
    end process;

    process(clk, rst)
    begin
        if (rst = '1') then
            out_h <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                out_h <= in_h;
            end if;
        end if;
    end process;

    process(clk, rst)
    begin
        if (rst = '1') then
            out_hcnt <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                out_hcnt <= in_hcnt;
            end if;
        end if;
    end process;

    process(clk, rst)
    begin
        if (rst = '1') then
            out_vcnt <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                out_vcnt <= in_vcnt;
            end if;
        end if;
    end process;

    process(clk, rst)
    begin
        if (rst = '1') then
            out_sync_switch <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                out_sync_switch <= in_sync_switch;
            end if;
        end if;
    end process;

    process(clk, rst)
    begin
        if (rst = '1') then
            out_eav_next <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                out_eav_next <= in_eav_next;
            end if;
        end if;
    end process;

    process(clk, rst)
    begin
        if (rst = '1') then
            out_sav_next <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                out_sav_next <= in_sav_next;
            end if;
        end if;
    end process;

    process(clk, rst)
    begin
        if (rst = '1') then
            out_xyz_word <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                out_xyz_word <= in_xyz_word;
            end if;
        end if;
    end process;

    process(clk, rst)
    begin
        if (rst = '1') then
            out_anc_next <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                out_anc_next <= in_anc_next;
            end if;
        end if;
    end process;

    process(clk, rst)
    begin
        if (rst = '1') then
            out_edh_next <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                out_edh_next <= in_edh_next;
            end if;
        end if;
    end process;

end synth;