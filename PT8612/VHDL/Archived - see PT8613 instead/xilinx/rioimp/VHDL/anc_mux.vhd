-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: anc_mux.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2005-01-18 10:40:24-07 $
-- /___/   /\    Date Created: Jan 5, 2005
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: anc_mux.vhd,rcs $
-- Revision 1.0  2005-01-18 10:40:24-07  jsnow
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
-- This module generates and mulitplexes ANC packets into the video stream. The
-- raw ANC packet data is written to the module through the data_in port. The
-- anc_pkt_gen module formats the ANC packet and provides it to the anc_insert
-- module for insertion into an appropriate ANC space.
-- 
-- The module will overwrite end-marker ANC packets, but it does not overwrite
-- packets that have been marked for deletion.
-- 
-- The following signals are used to write the ANC data to the module. The CS 
-- and ADF words are generated automatically and do not need to be written to 
-- the module.
-- 
-- pkt_in_empty: This output must be asserted high before any ANC data may be
-- written to the module. It remains high until pkt_rdy_in is asserted, at which
-- point it goes low and remains low until the ANC packet has been inserted into
-- the video stream.
-- 
-- data_in: This 10-bit port is the input data port for the raw ANC data. When
-- 8-bit values (DID, SDID, DBN, and DC) are written, they must be placed on the
-- least significant 8-bits of the data_in port.
-- 
-- ld_did: When this signal is asserted, the module accepts the 8-bit DID value
-- from the data_in port.
-- 
-- ld_dbn: When this signal is asserted, the module accepts the 8-bit DBN or 
-- SDID value from the data_in port.
-- 
-- ld_dc: When this signal is asserted, the module accepts the 8-bit DC value
-- from the data_in port.
-- 
-- ld_udw: When this signal is asserted, a 10-bit user data word value is
-- accepted from the data_in port. The udw_wr_adr value must indicate which word
-- of the UDW payload is currently being written whenever ld_udw is asserted.
-- 
-- udw_wr_adr: This 8-bit input port must be set to indicate which word of the 
-- UDW words is currently being written whenever ld_udw is asserted. When the 
-- first word of the payload is written, udw_wr_adr must be 0. It must be 1 for 
-- the second word, and so on.
-- 
-- calc_udw_parity: If the UDW words are formatted such that they have data in
-- the LS eight bits, and even parity bit in bit 8, and the complement of bit 8
-- in bit 9, then the module can automatically calculate the parity bits (8 and 
-- 9). When the calc_udw_parity signal is asserted at the same time as ld_udw,
-- the two MS bits of data_in are ignored and the module calculates bits 8 and 9
-- of the UDW word as parity bits.
-- 
-- pkt_rdy_in: Once all the words of the ANC packet have been written to the 
-- module, the pkt_rdy_in signal must be asserted. This signal only needs to be
-- asserted for one clock cycle. The pkt_rdy_in signal must be negated before
-- the pkt_empty_in signal will be reasserted by the module.
-- 
-- hanc_pkt and vanc_pkt: At the same time that pkt_rdy_in is asserted, these
-- inputs must be set appropriately to specify whether the new packet is to be
-- inserted into HANC space or VANC space. If both inputs are asserted, the new
-- packet will be inserted into the first available space, either HANC or VANC,
-- that has enough free space for the packet.
-- 
-- This module also has inputs and outputs for each of the various video timing
-- signals from the video decoder. This is to allow these signals to be delayed
-- by the same amount as the video path is delayed so that they can remain
-- synchronized. However, the current anc_mux design has a purely combinatorial
-- path for the video stream, so no cycles of latency are introduced. Thus the
-- video timing signals are passed directly from the inputs to the outputs 
-- without delay. If future versions of the anc_mux module do introduce some 
-- latency on the video path, equivalent latency can be added here to the video 
-- timing signals. This can be done without affecting the wiring of modules 
-- higher in the hierarchy.
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

use work.anc_edh_pkg.all;

entity anc_mux is
    port (
        -- inputs
        clk:            in    std_ulogic;   -- clock input
        ce:             in    std_ulogic;   -- clock enable
        rst:            in    std_ulogic;   -- async reset input
        vid_in:         in    video_type;   -- video data input
        std:            in    vidstd_type;  -- current video standard
        hanc_pkt:       in    std_ulogic;   -- ANC packet to be sent can be inserted into HANC space
        vanc_pkt:       in    std_ulogic;   -- ANC packet to be sent can be inserted into VANC space
        pkt_rdy_in:     in    std_ulogic;   -- indicates packet has been loaded into input regs
        calc_udw_parity:in    std_ulogic;   -- indicates that module should calculate parity on UDW
        data_in:        in    video_type;   -- input data bus
        ld_did:         in    std_ulogic;   -- load DID register
        ld_dbn:         in    std_ulogic;   -- load SDID/DBN register
        ld_dc:          in    std_ulogic;   -- load DC register
        ld_udw:         in    std_ulogic;   -- loads the UDW RAM
        udw_wr_adr:     in    udwadr_type;  -- UDW RAM write address
        in_trs:         in    std_ulogic;   -- asserted during TRS symbols in video stream
        in_f:           in    std_ulogic;   -- field indicator
        in_v:           in    std_ulogic;   -- vertical blanking indicator
        in_h:           in    std_ulogic;   -- horizontal blanking indicator
        in_hcnt:        in    hpos_type;    -- horizontal position
        in_vcnt:        in    vpos_type;    -- vertical position
        in_sync_switch: in    std_ulogic;   -- synchronous switching line indicator
        in_eav_next:    in    std_ulogic;   -- indicates that next word is first word of an EAV
        in_sav_next:    in    std_ulogic;   -- indicates that next word is first word of an SAV
        in_xyz_word:    in    std_ulogic;   -- indicates that current word is XYZ word of a TRS
        in_anc_next:    in    std_ulogic;   -- indicates that next word is first word of ANC packet
        in_edh_next:    in    std_ulogic;   -- indicates that next word is first word of EDH packet
        in_edh_loc:     in    std_ulogic;   -- asserted when next word is beginning of EDH packet location

        -- outputs
        vid_out:        out   video_type;   -- output video data
        pkt_in_empty:   out   std_ulogic;   -- module is ready for input packet to be loaded
        out_trs:        out   std_ulogic;   -- "delayed" in_trs signal
        out_f:          out   std_ulogic;   -- "delayed" in_f signal
        out_v:          out   std_ulogic;   -- "delayed" in_v signal
        out_h:          out   std_ulogic;   -- "delayed" in_h signal
        out_hcnt:       out   hpos_type;    -- "delayed" in_hcnt signal
        out_vcnt:       out   vpos_type;    -- "delayed" in_vcnt signal
        out_sync_switch:out   std_ulogic;   -- "delayed" in_sync_switch signal
        out_eav_next:   out   std_ulogic;   -- "delayed" in_eav_next signal
        out_sav_next:   out   std_ulogic;   -- "delayed" in_sav_next signal
        out_xyz_word:   out   std_ulogic;   -- "delayed" in_xyz_word signal
        out_anc_next:   out   std_ulogic;   -- "delayed" in_anc_next signal
        out_edh_next:   out   std_ulogic;   -- "delayed" in_edh_next signal
        out_edh_loc:    out   std_ulogic);  -- "delayed" in_edh_loc signal
end;


architecture synth of anc_mux is


--
-- These constants define the encoding of the video standard input port (std).
--
constant NTSC_422:      std_ulogic_vector(2 downto 0) := "000";
constant NTSC_INVALID:  std_ulogic_vector(2 downto 0) := "001";
constant NTSC_422_WIDE: std_ulogic_vector(2 downto 0) := "010";
constant NTSC_4444:     std_ulogic_vector(2 downto 0) := "011";
constant PAL_422:       std_ulogic_vector(2 downto 0) := "100";
constant PAL_INVALID:   std_ulogic_vector(2 downto 0) := "101";
constant PAL_422_WIDE:  std_ulogic_vector(2 downto 0) := "110";
constant PAL_4444:      std_ulogic_vector(2 downto 0) := "111";


-------------------------------------------------------------------------------
-- Signal definitions
--
signal send_pkt :       std_ulogic;         -- send_pkt signal between modules
signal abort_pkt :      std_ulogic;         -- abort_pkt signal between modules
signal pkt_rdy :        std_ulogic;         -- indicates packet is ready to be sent
signal anc_in :         video_type;         -- ANC packet data input
signal pkt_size :       pktsize_type;       -- size of packet to be sent
signal pkt_end :        std_ulogic;         -- indicates last word of packet being sent
signal anc_data :       video_type;         -- data bus from anc_pkt_gen to anc_insert
signal ins_out :        video_type;         -- video output of anc_insert module
signal hanc_pkt_q :     std_ulogic;         -- registered version of hanc_pkt input
signal vanc_pkt_q :     std_ulogic;         -- registered version of vanc_pkt input

--
-- Component definitions
--
component anc_pkt_gen
    port(
        clk :           in    std_ulogic;
        ce :            in    std_ulogic;
        rst :           in    std_ulogic;
        pkt_rdy_in :    in    std_ulogic;
        calc_udw_parity:in    std_ulogic;
        send_pkt :      in    std_ulogic;
        abort_pkt :     in    std_ulogic;
        data_in :       in    video_type;
        ld_did :        in    std_ulogic;
        ld_dbn :        in    std_ulogic;
        ld_dc :         in    std_ulogic;
        ld_udw :        in    std_ulogic;
        udw_wr_adr :    in    udwadr_type;
        anc_out :       out   video_type;
        pkt_size :      out   pktsize_type;
        pkt_in_empty :  out   std_ulogic;
        pkt_end :       out   std_ulogic;
        pkt_rdy_out :   out   std_ulogic);
end component;

component anc_insert
    port(
        clk :           in    std_ulogic;
        ce :            in    std_ulogic;
        rst :           in    std_ulogic;
        pkt_rdy :       in    std_ulogic;
        anc_in :        in    video_type;
        vid_in :        in    video_type;
        pkt_size :      in    pktsize_type;
        pkt_end :       in    std_ulogic;
        anc_next :      in    std_ulogic;
        eav_next :      in    std_ulogic;
        sav_next :      in    std_ulogic;
        hanc_pkt :      in    std_ulogic;
        vanc_pkt :      in    std_ulogic;
        std :           in    vidstd_type;
        v_blank :       in    std_ulogic;
        field :         in    std_ulogic;
        horz_count :    in    hpos_type;
        vert_count :    in    vpos_type;
        switch_line :   in    std_ulogic;
        vid_out :       out   video_type;
        send_pkt :      out   std_logic;
        abort_pkt :     out   std_logic);
end component;

begin
        
    --
    -- Instantiate the ANC packet generator module
    --
    ANC_GEN : anc_pkt_gen
        port map (
            clk             => clk,
            ce              => ce,
            rst             => rst,
            pkt_rdy_in      => pkt_rdy_in,
            calc_udw_parity => calc_udw_parity,
            send_pkt        => send_pkt,
            abort_pkt       => abort_pkt,
            data_in         => data_in,
            ld_did          => ld_did,
            ld_dbn          => ld_dbn,
            ld_dc           => ld_dc,
            ld_udw          => ld_udw,
            udw_wr_adr      => udw_wr_adr,
            anc_out         => anc_data,
            pkt_size        => pkt_size,
            pkt_in_empty    => pkt_in_empty,
            pkt_end         => pkt_end,
            pkt_rdy_out     => pkt_rdy);

    --
    -- When pkt_rdy_in signal is asserted, capture the hanc_pkt and vanc_pkt 
    -- inputs.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            hanc_pkt_q <= '1';
            vanc_pkt_q <= '1';
        elsif (clk'event and clk = '1') then
            if (ce = '1' and pkt_rdy_in = '1') then
                hanc_pkt_q <= hanc_pkt;
                vanc_pkt_q <= vanc_pkt;
            end if;
        end if;
    end process;

    --
    --
    -- Instantiate the ANC packet insertion module
    --
    INSERT : anc_insert
        port map (
            clk             => clk,
            ce              => ce,
            rst             => rst,
            pkt_rdy         => pkt_rdy,
            anc_in          => anc_data,
            vid_in          => vid_in,
            pkt_size        => pkt_size,
            pkt_end         => pkt_end,
            anc_next        => in_anc_next,
            eav_next        => in_eav_next,
            sav_next        => in_sav_next,
            hanc_pkt        => hanc_pkt_q,
            vanc_pkt        => vanc_pkt_q,
            std             => std,
            v_blank         => in_v,
            field           => in_f,
            horz_count      => in_hcnt,
            vert_count      => in_vcnt,
            switch_line     => in_sync_switch,
            vid_out         => ins_out,
            send_pkt        => send_pkt,
            abort_pkt       => abort_pkt);

    --
    -- Output assignments
    --
    vid_out <= ins_out;
    out_f <= in_f;
    out_v <= in_v;
    out_h <= in_h;
    out_hcnt <= in_hcnt;
    out_vcnt <= in_vcnt;
    out_sync_switch <= in_sync_switch;
    out_trs <= in_trs;
    out_eav_next <= in_eav_next;
    out_sav_next <= in_sav_next;
    out_xyz_word <= in_xyz_word;
    out_anc_next <= in_anc_next;
    out_edh_next <= in_edh_next;
    out_edh_loc <= in_edh_loc;

end synth;