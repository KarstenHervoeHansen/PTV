-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: anc_edh_processor.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2005-01-18 10:38:50-07 $
-- /___/   /\    Date Created: Jan 5, 2005
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: anc_edh_processor.vhd,rcs $
-- Revision 1.0  2005-01-18 10:38:50-07  jsnow
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
-- This module instances and interconnects the various modules that make up the
-- ANC and EDH processors. The video_decode module from XAPP625 is also
-- instanced.
-- 
-- This module calculates CRC checkwords for each field of the input video
-- stream. It finds the EDH packets in the input video stream, if they are
-- present, and compares the calculated checkwords with the ones in the EDH
-- packets. If differences are detected, these will be noted in the error flags
-- of the EDH packet in the outgoing video stream.
-- 
-- For each ANC packet in the video stream, the module calculates a checksum and
-- compares it to the packet's CS word. Any differences will be noted in the
-- error flags of the EDH packet in the outgoing video stream.
-- 
-- The module includes one instance of an anc_demux. This module will search the
-- input video stream for up to four different types of ANC packets and
-- demultiplex them to the anc_out port. Optionally, demultiplexed ANC packets
-- can be marked for deletion.
-- 
-- The module includes on instance of an anc_mux. This module will multiplex new
-- ANC packets into the video stream.
-- 
-- After the ANC packet processing, the module recalculates the CRC words for
-- each field and inserts them, along with the updated error flags, into the
-- EDH packet in the output video stream. If the input video stream did not
-- have an EDH packet, and new EDH packet is created and inserted at the
-- appropriate spot in the video stream. 
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

use work.anc_edh_pkg.all;

entity edh_processor is
    port (
        clk:            in  std_ulogic;         -- clock input
        ce:             in  std_ulogic;         -- clock enable
        rst:            in  std_ulogic;         -- async reset input

    -- video decoder inputs
        vid_in:         in  video_type;         -- input video
        reacquire:      in  std_ulogic;         -- forces autodetect to reacquire the video standard
        en_sync_switch: in  std_ulogic;         -- enables synchronous switching
        en_trs_blank:   in  std_ulogic;         -- enables TRS blanking when asserted

        -- EDH flag inputs
        anc_idh_local:  in  std_ulogic;         -- ANC IDH flag input
        anc_ues_local:  in  std_ulogic;         -- ANC UES flag input
        ap_idh_local:   in  std_ulogic;         -- AP IDH flag input
        ff_idh_local:   in  std_ulogic;         -- FF IDH flag input
        errcnt_flg_en:  in  edh_allflg_type;    -- selects which error flags increment the error counter
        clr_errcnt:     in  std_ulogic;         -- clears the error counter
        receive_mode:   in  std_ulogic;         -- 1 enables receiver, 0 for generate only

        -- ANC demux inputs
        en_a:           in  std_ulogic;         -- enable for DID A decoder
        did_a:          in  ubyte_type;         -- first DID code to match
        sdid_a:         in  ubyte_type;         -- first SDID code to match
        del_pkt_a:      in  std_ulogic;         -- packet will be deleted if asserted
        en_b:           in  std_ulogic;         -- enable for DID B decoder
        did_b:          in  ubyte_type;         -- second DID code to match
        sdid_b:         in  ubyte_type;         -- second SDID code to match
        del_pkt_b:      in  std_ulogic;         -- packet will be deleted if asserted
        en_c:           in  std_ulogic;         -- enable for DID C decoder
        did_c:          in  ubyte_type;         -- third DID code to match
        sdid_c:         in  ubyte_type;         -- third SDID code to match
        del_pkt_c:      in  std_ulogic;         -- packet will be deleted if asserted
        en_d:           in  std_ulogic;         -- enable for DID D decoder
        did_d:          in  ubyte_type;         -- fourth DID code to match
        sdid_d:         in  ubyte_type;         -- fourth SDID code to match
        del_pkt_d:      in  std_ulogic;         -- packet will be deleted if asserted

        -- ANC mux inputs
        hanc_pkt:       in  std_ulogic;         -- ANC packet to be sent can be inserted into HANC space
        vanc_pkt:       in  std_ulogic;         -- ANC packet to be sent can be inserted into VANC space
        pkt_rdy_in:     in  std_ulogic;         -- indicates packet has been loaded into input regs
        calc_udw_parity:in  std_ulogic;         -- indicates that module should calculate parity on UDW
        anc_in:         in  video_type;         -- input data bus
        ld_did:         in  std_ulogic;         -- load DID register
        ld_dbn:         in  std_ulogic;         -- load SDID/DBN register
        ld_dc:          in  std_ulogic;         -- load DC register
        ld_udw:         in  std_ulogic;         -- loads the UDW RAM
        udw_wr_adr:     in  udwadr_type;        -- UDW RAM write address

        -- video and decoded video timing outputs
        vid_out:        out video_type;         -- output video stream with EDH packets inserted
        std:            out vidstd_type;        -- video standard code
        std_locked:     out std_ulogic;         -- video standard detector is locked
        trs:            out std_ulogic;         -- asserted during flywheel generated TRS symbol
        field:          out std_ulogic;         -- field indicator
        v_blank:        out std_ulogic;         -- vertical blanking indicator
        h_blank:        out std_ulogic;         -- horizontal blanking indicator
        horz_count:     out hpos_type;          -- horizontal position
        vert_count:     out vpos_type;          -- vertical position
        sync_switch:    out std_ulogic;         -- asserted on lines where synchronous switching is allowed
        locked:         out std_ulogic;         -- asserted when flywheel is synchronized to video
        eav_next:       out std_ulogic;         -- next word is first word of EAV
        sav_next:       out std_ulogic;         -- next word is first word of SAV
        xyz_word:       out std_ulogic;         -- current word is the XYZ word of a TRS
        anc_next:       out std_ulogic;         -- next word is first word of a received ANC packet
        edh_next:       out std_ulogic;         -- next word is first word of a received EDH packet

        -- EDH flag outputs
        rx_ap_flags:    out edh_flgset_type;    -- AP error flags received from last EDH packet
        rx_ff_flags:    out edh_flgset_type;    -- FF error flags received from last EDH packet
        rx_anc_flags:   out edh_flgset_type;    -- ANC error flags freceived from last EDH packet
        ap_flags:       out edh_flgset_type;    -- transmitted AP error flags from last field
        ff_flags:       out edh_flgset_type;    -- transmitted FF error flags from last field
        anc_flags:      out edh_flgset_type;    -- transmitted ANC error flags from last field
        packet_flags:   out edh_pktflg_type;    -- error flags related to the received packet processing
        errcnt:         out edh_errcnt_type;    -- errored fields counter
        edh_packet:     out std_ulogic;         -- asserted during all words of a generated EDH packet

        -- ANC demux outputs
        anc_out:        out video_type;         -- output packet data
        anc_out_valid:  out std_ulogic;         -- asserted while all words of a matching packet are on anc_out
        anc_match:      out                     -- indicates DID/SDID combination matched the current packet
            std_ulogic_vector(1 downto 0);
        did:            out std_ulogic;         -- asserted when a DID word from a matching packet is on anc_out
        dbn:            out std_ulogic;         -- asserted when a DBN word from a matching packet is on anc_out
        sdid:           out std_ulogic;         -- asserted when an SDID word from a matching packet is on anc_out
        dc:             out std_ulogic;         -- asserted when a DC word from a matching packet is on anc_out
        udw:            out std_ulogic;         -- asserted when a UDW word from a matching packet is on anc_out
        cs:             out std_ulogic;         -- asserted when a CS word from a matching packet is on anc_out

        -- ANC mux outputs
        pkt_in_empty:   out std_ulogic);        -- module is ready for input packet to be loaded

end;

architecture synth of edh_processor is

-------------------------------------------------------------------------------
-- Signal definitions
--
signal dec_std:         vidstd_type;            -- video_decode std output
signal dec_std_locked:  std_ulogic;             -- video_decode std locked output
signal dec_vid:         video_type;             -- video_decode video output
signal dec_trs:         std_ulogic;             -- video_decode trs output
signal dec_f:           std_ulogic;             -- video_decode field output
signal dec_v:           std_ulogic;             -- video_decode v_blank output
signal dec_h:           std_ulogic;             -- video_decode h_blank output
signal dec_hcnt:        hpos_type;              -- video_decode horz_count output
signal dec_vcnt:        vpos_type;              -- video_decode vert_count output
signal dec_sync_switch: std_ulogic;             -- video_decode sync_switch output
signal dec_locked:      std_ulogic;             -- video_decode locked output
signal dec_eav_next:    std_ulogic;             -- video_decode eav_next output
signal dec_sav_next:    std_ulogic;             -- video_decode sav_next output
signal dec_xyz_word:    std_ulogic;             -- video_decode xyz_word output
signal dec_anc_next:    std_ulogic;             -- video_decode anc_next output
signal dec_edh_next:    std_ulogic;             -- video_decode edh_next output
signal ap_crc:          crc16_type;             -- calculated active pic CRC
signal ap_crc_valid:    std_ulogic;             -- calculated active pic CRC valid signal
signal ff_crc:          crc16_type;             -- calculated full field CRC
signal ff_crc_valid:    std_ulogic;             -- calculated full field CRC valid signal
signal edh_missing:     std_ulogic;             -- EDH packet missing error flag
signal edh_parity_err:  std_ulogic;             -- EDH packet parity error flag
signal edh_chksum_err:  std_ulogic;             -- EDH packet checksum error flag
signal edh_format_err:  std_ulogic;             -- EDH packet format error flag
signal tx_edh_next:     std_ulogic;             -- generated EDH packet begins on next word
signal flag_bus:        edh_flgset_type;        -- flag bus between EDH_FLAGS and EDH_TX
signal ap_flag_word:    std_ulogic;             -- selects AP flags for flag bus
signal ff_flag_word:    std_ulogic;             -- selects FF flags for flag bus
signal anc_flag_word:   std_ulogic;             -- selects ANC flags for flag bus
signal rx_ap_crc_valid: std_ulogic;             -- received active pic CRC valid signal
signal rx_ap_crc:       crc16_type;             -- received active pic CRC
signal rx_ff_crc_valid: std_ulogic;             -- received full field CRC valid signal
signal rx_ff_crc:       crc16_type;             -- received full field CRC
signal in_ap_flags:     edh_flgset_type;        -- received AP flags to edh_flags
signal in_ff_flags:     edh_flgset_type;        -- received FF flags to edh_flags
signal in_anc_flags:    edh_flgset_type;        -- received ACN flags to edh_flags
signal errcnt_en:       std_ulogic;             -- enables error counter
signal anc_edh_local:   std_ulogic;             -- ANC EDH signal
signal tx_edh_packet:   std_ulogic;             -- asserted when edh packet is to be generated
signal dmux_vid:        video_type;             -- video out of anc_demux
signal dmux_trs:        std_ulogic;             -- trs output of anc_demux
signal dmux_f:          std_ulogic;             -- f output of anc_demux
signal dmux_v:          std_ulogic;             -- v output of anc_demux
signal dmux_h:          std_ulogic;             -- h output of anc_demux
signal dmux_hcnt:       hpos_type;              -- hcnt output of anc_demux
signal dmux_vcnt:       vpos_type;              -- vcnt output of anc_demux
signal dmux_sync_switch:std_ulogic;             -- sync_switch output of anc_demux
signal dmux_eav_next:   std_ulogic;             -- eav_next output of anc_demux
signal dmux_sav_next:   std_ulogic;             -- sav_next output of anc_demux
signal dmux_xyz_word:   std_ulogic;             -- xyz_word output of anc_demux
signal dmux_anc_next:   std_ulogic;             -- anc_next output of anc_demux
signal dmux_edh_next:   std_ulogic;             -- edh_next output of anc_demux
signal dmux_edh_loc:    std_ulogic;             -- edh_loc output of anc_demux
signal mux_vid:         video_type;             -- video out of anc_mux
signal mux_trs:         std_ulogic;             -- trs output of anc_mux
signal mux_f:           std_ulogic;             -- f output of anc_mux
signal mux_v:           std_ulogic;             -- v output of anc_mux
signal mux_h:           std_ulogic;             -- h output of anc_mux
signal mux_hcnt:        hpos_type;              -- hcnt output of anc_mux
signal mux_vcnt:        vpos_type;              -- vcnt output of anc_mux
signal mux_sync_switch: std_ulogic;             -- sync_switch output of anc_mux
signal mux_eav_next:    std_ulogic;             -- eav_next output of anc_mux
signal mux_sav_next:    std_ulogic;             -- sav_next output of anc_mux
signal mux_xyz_word:    std_ulogic;             -- xyz_word output of anc_mux
signal mux_anc_next:    std_ulogic;             -- anc_next output of anc_mux
signal mux_edh_next:    std_ulogic;             -- edh_next output of anc_mux
signal mux_edh_loc:     std_ulogic;             -- edh_loc output of anc_mux
signal edh_all_flags :  edh_allflg_type;        -- flag vector for edh_errcnt module
signal anc_flags_int :  edh_flgset_type;        -- internal version of anc_flags output
signal ap_flags_int :   edh_flgset_type;        -- internal version of ap_flags output
signal ff_flags_int :   edh_flgset_type;        -- internal version of ff_flags output
signal VCC :            std_ulogic;		        -- pull-up

component video_decode
    port(
        clk:            in    std_ulogic;       -- clock input
        ce:             in    std_ulogic;       -- clock enable
        rst:            in    std_ulogic;       -- async reset input
        vid_in:         in    video_type;       -- input video
        reacquire:      in    std_ulogic;       -- forces autodetect to reacquire the standard
        en_sync_switch: in    std_ulogic;       -- enables sync switching
        en_trs_blank:   in    std_ulogic;       -- enables TRS blanking
        std:            inout vidstd_type;      -- video standard code
        std_locked:     inout std_ulogic;       -- autodetect circuit is locked to standard
        trs:            out   std_ulogic;       -- asserted during TRS symbol
        vid_out:        out   video_type;       -- output video stream
        field:          out   std_ulogic;       -- field indicator
        v_blank:        out   std_ulogic;       -- vertical blanking indicator
        h_blank:        out   std_ulogic;       -- horizontal blanking indicator
        horz_count:     out   hpos_type;        -- horizontal counter
        vert_count:     out   vpos_type;        -- vertical counter
        sync_switch:    out   std_ulogic;       -- asserted on sync switching lines
        locked:         out   std_ulogic;       -- asserted when flywheel is synced to input video
        eav_next:       out   std_ulogic;       -- next word is first word of EAV
        sav_next:       out   std_ulogic;       -- next word is first word of SAV
        xyz_word:       out   std_ulogic;       -- current word is the XYZ word
        anc_next:       out   std_ulogic;       -- next word is first word of ANC packet
        edh_next:       out   std_ulogic);      -- next word is first word of EDH packet
end component;

component edh_crc
    port (
        clk:            in  std_ulogic;         -- clock input
        ce:             in  std_ulogic;         -- clock enable
        rst:            in  std_ulogic;         -- async reset input
        f:              in  std_ulogic;         -- field bit
        h:              in  std_ulogic;         -- horizontal blanking bit
        eav_next:       in  std_ulogic;         -- asserted when next samplebegins EAV symbol
        xyz_word:       in  std_ulogic;         -- asserted when current word is the XYZ word of a TRS
        vid_in:         in  video_type;         -- video data
        vcnt:           in  vpos_type;          -- vertical line count
        std:            in  vidstd_type;        -- indicates the video standard
        locked:         in  std_ulogic;         -- asserted when flywheel is locked
        ap_crc:         out crc16_type;         -- calculated active picture CRC
        ap_crc_valid:   out std_ulogic;         -- asserted when AP CRC is valid
        ff_crc:         out crc16_type;         -- calculated full-frame CRC
        ff_crc_valid:   out std_ulogic);        -- asserted when FF CRC is valid
end component;

component edh_rx
    port (
        clk:            in  std_ulogic;         -- clock input
        ce:             in  std_ulogic;         -- clock enable
        rst:            in  std_ulogic;         -- async reset input
        rx_edh_next:    in  std_ulogic;         -- indicates the next word is the first word of a received EDH packet
        vid_in:         in  video_type;         -- video data
        edh_next:       in  std_ulogic;         -- EDH packet begins on next sample
        reg_flags:      in  std_ulogic;         -- 1 = register flag words, 0 = feed vid_in through
        ap_crc_valid:   out std_ulogic;         -- valid bit for active picture CRC
        ap_crc:         out crc16_type;         -- active picture CRC
        ff_crc_valid:   out std_ulogic;         -- valid bit for full field CRC
        ff_crc:         out crc16_type;         -- full field CRC
        edh_missing:    out std_ulogic;         -- asserted when last expected EDH packet was missing
        edh_parity_err: out std_ulogic;         -- asserted when a parity error occurs in EDH packet
        edh_chksum_err: out std_ulogic;         -- asserted when a checksum error occurs in EDH packet
        edh_format_err: out std_ulogic;         -- asserted when a format error is found in EDH packet
        in_ap_flags:    out edh_flgset_type;    -- received AP flag word to edh_flags module
        in_ff_flags:    out edh_flgset_type;    -- received FF flag word to edh_flags module
        in_anc_flags:   out edh_flgset_type;    -- received ANC flag word to edh_flags module
        rx_ap_flags:    out edh_flgset_type;    -- received & registered AP flags for external inspection
        rx_ff_flags:    out edh_flgset_type;    -- received & registered FF flags for external inspection
        rx_anc_flags:   out edh_flgset_type);   -- received & registered ANC flags for external inspection
end component;

component edh_loc
    port (
        clk:            in  std_ulogic;         -- clock input
        ce:             in  std_ulogic;         -- clock enable
        rst:            in  std_ulogic;         -- async reset input
        f:              in  std_ulogic;         -- field bit
        vcnt:           in  vpos_type;          -- vertical line count
        hcnt:           in  hpos_type;          -- horizontal position
        std:            in  vidstd_type;        -- video standards
        edh_next:       out std_ulogic);        -- EDH packet should begin on next sample
end component;

component anc_rx
    port (
        clk:            in  std_ulogic;         -- clock input
        ce:             in  std_ulogic;         -- clock enable
        rst:            in  std_ulogic;         -- async reset input
        locked:         in  std_ulogic;         -- video decoder locked signal
        rx_anc_next:    in  std_ulogic;         -- asserted when next word begins ANC packet
        rx_edh_next:    in  std_ulogic;         -- asserted when next word begins EDH packet
        edh_packet:     in  std_ulogic;         -- indicates an EDH packet is being generated
        vid_in:         in  video_type;         -- video input data
        anc_edh_local:  out std_ulogic);        -- ANC error detected here flag
end component;

component edh_flags
    port (
        clk:                in  std_ulogic;     -- clock input
        ce:                 in  std_ulogic;     -- clock enable
        rst:                in  std_ulogic;     -- async reset input
        receive_mode:       in  std_ulogic;     -- asserted if receiver is active
        ap_flag_word:       in  std_ulogic;     -- selects the AP flag word for flag_bus
        ff_flag_word:       in  std_ulogic;     -- selects the FF flag word for flag_bus
        anc_flag_word:      in  std_ulogic;     -- selects the ANC flag word for the flag_bus
        edh_missing:        in  std_ulogic;     -- EDH packet missing from data stream
        edh_parity_err:     in  std_ulogic;     -- EDH packet parity error
        edh_format_err:     in  std_ulogic;     -- EDH packet format error
        rx_ap_crc_valid:    in  std_ulogic;     -- received AP CRC valid bit
        rx_ap_crc:          in  crc16_type;     -- received AP CRC value 
        rx_ff_crc_valid:    in  std_ulogic;     -- received FF CRC valid bit
        rx_ff_crc:          in  crc16_type;     -- received FF CRC value
        rx_ap_flags:        in  edh_flgset_type;-- received AP flag word
        rx_ff_flags:        in  edh_flgset_type;-- received FF flag word
        rx_anc_flags:       in  edh_flgset_type;-- recevied ANC flag word
        anc_edh_local:      in  std_ulogic;     -- local ANC EDH flag input
        anc_idh_local:      in  std_ulogic;     -- local ANC IDH flag input
        anc_ues_local:      in  std_ulogic;     -- local ANC UES flag input
        ap_idh_local:       in  std_ulogic;     -- local AP IDH flag input
        ff_idh_local:       in  std_ulogic;     -- local FF IDH flag input
        calc_ap_crc_valid:  in  std_ulogic;     -- calculated AP CRC valid bit
        calc_ap_crc:        in  crc16_type;     -- calculated AP CRC value
        calc_ff_crc_valid:  in  std_ulogic;     -- calculated FF CRC value
        calc_ff_crc:        in  crc16_type;     -- calculated FF CRC
        flags:              out edh_flgset_type;-- flag bus output
        ap_flags:           out edh_flgset_type;-- AP flags from last EDH packet sent
        ff_flags:           out edh_flgset_type;-- FF flags from last EDH packet sent
        anc_flags:          out edh_flgset_type);-- ANC flags from last EDH packet sent
end component;

component edh_errcnt
    port (
        clk:            in    std_ulogic;       -- clock input
        ce:             in    std_ulogic;       -- clock enable
        rst:            in    std_ulogic;       -- async reset input
        clr_errcnt:     in    std_ulogic;       -- clears the error counter
        count_en:       in    std_ulogic;       -- enables error counter when high
        flag_enables:   in    edh_allflg_type;  -- specifies which error flags cause the counter to increment
        flags:          in    edh_allflg_type;  -- error flag inputs
        edh_next:       in    std_ulogic;       -- counter increment on edh_next asserted
        errcnt:         out   edh_errcnt_type); -- errored field counter
end component;

component anc_demux
    port (
        clk:            in    std_ulogic;       -- clock input
        ce:             in    std_ulogic;       -- clock enable
        rst:            in    std_ulogic;       -- async reset input
        vid_in:         in    video_type;       -- video data input
        en_a:           in    std_ulogic;       -- enable for DID A decoder
        did_a:          in    ubyte_type;       -- first DID code to match
        sdid_a:         in    ubyte_type;       -- first SDID code to match
        del_pkt_a:      in    std_ulogic;       -- packet will be deleted if asserted
        en_b:           in    std_ulogic;       -- enable for DID B decoder
        did_b:          in    ubyte_type;       -- second DID code to match
        sdid_b:         in    ubyte_type;       -- second SDID code to match
        del_pkt_b:      in    std_ulogic;       -- packet will be deleted if asserted
        en_c:           in    std_ulogic;       -- enable for DID C decoder
        did_c:          in    ubyte_type;       -- third DID code to match
        sdid_c:         in    ubyte_type;       -- third SDID code to match
        del_pkt_c:      in    std_ulogic;       -- packet will be deleted if asserted
        en_d:           in    std_ulogic;       -- enable for DID D decoder
        did_d:          in    ubyte_type;       -- fourth DID code to match
        sdid_d:         in    ubyte_type;       -- fourth SDID code to match
        del_pkt_d:      in    std_ulogic;       -- packet will be deleted if asserted
        in_trs:         in    std_ulogic;       -- asserted during the four words of all TRS symbols
        in_f:           in    std_ulogic;       -- field indicator
        in_v:           in    std_ulogic;       -- vertical blanking indicator
        in_h:           in    std_ulogic;       -- horizontal blanking indicator
        in_hcnt:        in    hpos_type;        -- horizontal position
        in_vcnt:        in    vpos_type;        -- vertical position
        in_sync_switch: in    std_ulogic;       -- synchronous switching line indicator
        in_eav_next:    in    std_ulogic;       -- next word begins an EAV
        in_sav_next:    in    std_ulogic;       -- next word begins an SAV
        in_xyz_word:    in    std_ulogic;       -- current word is a TRS XYZ word
        in_anc_next:    in    std_ulogic;       -- asserted when next word begins received ANC packet
        in_edh_next:    in    std_ulogic;       -- asserted when next word begins received EDH packet
        in_edh_loc:     in    std_ulogic;       -- asserted when next word is starting location where EDH
        data_out:       out   video_type;       -- output packet data
        data_out_valid: out   std_ulogic;       -- asserted while all words of a matching packet are on data_out
        match_code:     out                     -- indicates DID/SDID combination matched the current packet
            std_ulogic_vector(1 downto 0);  
        did:            out   std_ulogic;       -- asserted when a DID word from a matching packet is on data_out
        dbn:            out   std_ulogic;       -- asserted when a DBN word from a matching packet is on data_out
        sdid:           out   std_ulogic;       -- asserted when an SDID word from a matching packet is on data_out
        dc:             out   std_ulogic;       -- asserted when a DC word from a matching packet is on data_out
        udw:            out   std_ulogic;       -- asserted when a UDW word from a matching packet is on data_out
        cs:             out   std_ulogic;       -- asserted when a CS word from a matching packet is on data_out
        vid_out:        out   video_type;       -- output video stream
        out_trs:        out   std_ulogic;       -- delayed version of in_trs
        out_f:          out   std_ulogic;       -- delayed version of in_f
        out_v:          out   std_ulogic;       -- delayed version of in_v
        out_h:          out   std_ulogic;       -- delayed version of in_h
        out_hcnt:       out   hpos_type;        -- delayed version of in_hcnt
        out_vcnt:       out   vpos_type;        -- delayed version of in_vcnt
        out_sync_switch:out   std_ulogic;       -- delayed version of in_sync_switch
        out_eav_next:   out   std_ulogic;       -- delayed version of in_eav_next
        out_sav_next:   out   std_ulogic;       -- delayed version of in_sav_next
        out_xyz_word:   out   std_ulogic;       -- delayed version of in_xyz_word
        out_anc_next:   out   std_ulogic;       -- delayed version of in_anc_next
        out_edh_next:   out   std_ulogic;       -- delayed version of in_edh_next
        out_edh_loc:    out   std_ulogic);      -- delayed version of in_edh_loc
end component;

component anc_mux
    port (
        clk:            in    std_ulogic;       -- clock input
        ce:             in    std_ulogic;       -- clock enable
        rst:            in    std_ulogic;       -- async reset input
        vid_in:         in    video_type;       -- video data input
        std:            in    vidstd_type;      -- current video standard
        hanc_pkt:       in    std_ulogic;       -- ANC packet to be sent can be inserted into HANC space
        vanc_pkt:       in    std_ulogic;       -- ANC packet to be sent can be inserted into VANC space
        pkt_rdy_in:     in    std_ulogic;       -- indicates packet has been loaded into input regs
        calc_udw_parity:in    std_ulogic;       -- indicates that module should calculate parity on UDW
        data_in:        in    video_type;       -- input data bus
        ld_did:         in    std_ulogic;       -- load DID register
        ld_dbn:         in    std_ulogic;       -- load SDID/DBN register
        ld_dc:          in    std_ulogic;       -- load DC register
        ld_udw:         in    std_ulogic;       -- loads the UDW RAM
        udw_wr_adr:     in    udwadr_type;      -- UDW RAM write address
        in_trs:         in    std_ulogic;       -- asserted during TRS symbols in video stream
        in_f:           in    std_ulogic;       -- field indicator
        in_v:           in    std_ulogic;       -- vertical blanking indicator
        in_h:           in    std_ulogic;       -- horizontal blanking indicator
        in_hcnt:        in    hpos_type;        -- horizontal position
        in_vcnt:        in    vpos_type;        -- vertical position
        in_sync_switch: in    std_ulogic;       -- synchronous switching line indicator
        in_eav_next:    in    std_ulogic;       -- indicates that next word is first word of an EAV
        in_sav_next:    in    std_ulogic;       -- indicates that next word is first word of an SAV
        in_xyz_word:    in    std_ulogic;       -- indicates that current word is XYZ word of a TRS
        in_anc_next:    in    std_ulogic;       -- indicates that next word is first word of ANC packet
        in_edh_next:    in    std_ulogic;       -- indicates that next word is first word of EDH packet
        in_edh_loc:     in    std_ulogic;       -- asserted when next word is beginning of EDH packet location
        vid_out:        out   video_type;       -- output video data
        pkt_in_empty:   out   std_ulogic;       -- module is ready for input packet to be loaded
        out_trs:        out   std_ulogic;       -- "delayed" in_trs signal
        out_f:          out   std_ulogic;       -- "delayed" in_f signal
        out_v:          out   std_ulogic;       -- "delayed" in_v signal
        out_h:          out   std_ulogic;       -- "delayed" in_h signal
        out_hcnt:       out   hpos_type;        -- "delayed" in_hcnt signal
        out_vcnt:       out   vpos_type;        -- "delayed" in_vcnt signal
        out_sync_switch:out   std_ulogic;       -- "delayed" in_sync_switch signal
        out_eav_next:   out   std_ulogic;       -- "delayed" in_eav_next signal
        out_sav_next:   out   std_ulogic;       -- "delayed" in_sav_next signal
        out_xyz_word:   out   std_ulogic;       -- "delayed" in_xyz_word signal
        out_anc_next:   out   std_ulogic;       -- "delayed" in_anc_next signal
        out_edh_next:   out   std_ulogic;       -- "delayed" in_edh_next signal
        out_edh_loc:    out   std_ulogic);      -- "delayed" in_edh_loc signal
end component;

component edh_gen
    port (
        clk:            in  std_ulogic;         -- clock input
        ce:             in  std_ulogic;         -- clock enable
        rst:            in  std_ulogic;         -- async reset input
        vid_in:         in  video_type;         -- input video port 
        std:            in  vidstd_type;        -- video standard
        locked:         in  std_ulogic;         -- flywheel is locked
        flag_bus:       in  edh_flgset_type;    -- input flag bus from edh_flags
        edh_missing:    in  std_ulogic;         -- indicates no EDH packet was found in input video
        in_trs:         in  std_ulogic;         -- asserted during TRS symbol
        in_f:           in  std_ulogic;         -- field bit
        in_v:           in  std_ulogic;         -- vertical blanking bit
        in_h:           in  std_ulogic;         -- horizontal blanking bit
        in_hcnt:        in  hpos_type;          -- current horizontal position
        in_vcnt:        in  vpos_type;          -- current vertical position
        in_sync_switch: in  std_ulogic;         -- sync switching interval indicator
        in_eav_next:    in  std_ulogic;         -- asserted when next word is 1st word of EAV
        in_sav_next:    in  std_ulogic;         -- asserted when next word is 1st word of SAV
        in_xyz_word:    in  std_ulogic;         -- asserted during XYZ word of TRS
        in_anc_next:    in  std_ulogic;         -- asserted when next word is 1st word of ANC packet
        in_edh_next:    in  std_ulogic;         -- asserted when next word is 1st word of EDH packet
        in_edh_loc:     in  std_ulogic;         -- next word is 1st word of an EDH packet location
        vid_out:        out video_type;         -- output video data
        ap_flag_word:   out std_ulogic;         -- selects the active-picture error flag set for flag_bus
        ff_flag_word:   out std_ulogic;         -- selects the full-field error flag set for flag_bus
        anc_flag_word:  out std_ulogic;         -- selects the anc error flag set for the flag_bus
        edh_packet:     out std_ulogic;         -- asserted during all words of a generated EDH packet
        out_trs:        out std_ulogic;         -- asserted during TRS symbols
        out_f:          out std_ulogic;         -- field indicator
        out_v:          out std_ulogic;         -- vertical blanking indicator
        out_h:          out std_ulogic;         -- horizontal blanking indicator
        out_hcnt:       out hpos_type;          -- horizontal position
        out_vcnt:       out vpos_type;          -- vertical position        
        out_sync_switch:out std_ulogic;         -- synchronous swithcing interval indicator     
        out_eav_next:   out std_ulogic;         -- asserted when next word is first word of an EAV      
        out_sav_next:   out std_ulogic;         -- asserted when next word is first word of an SAV      
        out_xyz_word:   out std_ulogic;         -- asserted during the XYZ word of a TRS symbol
        out_anc_next:   out std_ulogic;         -- asserted when next word is first word of an ANC packet       
        out_edh_next:   out std_ulogic);        -- asserted when next word is first word of an EDH packet
end component;

begin
	
	VCC <= '1';

    --
    -- video_decoder module
    --
    -- This module is from the XAPP625 refernce design. The module determines 
    -- the video standard of the input video stream and decodes the video timing
    -- information.
    --

    DEC : video_decode
        port map (
            clk                 => clk,
            ce                  => ce,
            rst                 => rst,
            vid_in              => vid_in,
            reacquire           => reacquire,
            en_sync_switch      => en_sync_switch,
            en_trs_blank        => en_trs_blank,
            std                 => dec_std,
            std_locked          => dec_std_locked,
            trs                 => dec_trs,
            vid_out             => dec_vid,
            field               => dec_f,
            v_blank             => dec_v,
            h_blank             => dec_h,
            horz_count          => dec_hcnt,
            vert_count          => dec_vcnt,
            sync_switch         => dec_sync_switch,
            locked              => dec_locked,
            eav_next            => dec_eav_next,
            sav_next            => dec_sav_next,
            xyz_word            => dec_xyz_word,
            anc_next            => dec_anc_next,
            edh_next            => dec_edh_next);

    --
    -- edh_crc module
    --
    -- This module computes the CRC values for the incoming video stream, vid_in.
    -- Also, the module generates valid signals for both CRC values based on the
    -- locked signal. If locked rises during a field, the CRC is considered to 
    -- be invalid.

    CRC : edh_crc
        port map (
            clk                 => clk,
            ce                  => ce,
            rst                 => rst,
            f                   => dec_f,
            h                   => dec_h,
            eav_next            => dec_eav_next,
            xyz_word            => dec_xyz_word,
            vid_in              => dec_vid,
            vcnt                => dec_vcnt,
            std                 => dec_std,
            locked              => dec_locked,
            ap_crc              => ap_crc,
            ap_crc_valid        => ap_crc_valid,
            ff_crc              => ff_crc,
            ff_crc_valid        => ff_crc_valid);

    --
    -- edh_rx module
    --
    -- This module processes EDH packets found in the incoming video stream. The
    -- CRC words and valid flags are captured from the packet. Various error 
    -- flags related to errors found in the packet are generated.
    --
    EDHRX : edh_rx
        port map (
            clk                 => clk,
            ce                  => ce,
            rst                 => rst,
            rx_edh_next         => dec_edh_next,
            vid_in              => dec_vid,
            edh_next            => tx_edh_next,
            reg_flags           => VCC,
            ap_crc_valid        => rx_ap_crc_valid,
            ap_crc              => rx_ap_crc,
            ff_crc_valid        => rx_ff_crc_valid,
            ff_crc              => rx_ff_crc,
            edh_missing         => edh_missing,
            edh_parity_err      => edh_parity_err,
            edh_chksum_err      => edh_chksum_err,
            edh_format_err      => edh_format_err,
            in_ap_flags         => in_ap_flags,
            in_ff_flags         => in_ff_flags,
            in_anc_flags        => in_anc_flags,
            rx_ap_flags         => rx_ap_flags,
            rx_ff_flags         => rx_ff_flags,
            rx_anc_flags        => rx_anc_flags);

    --
    -- edh_loc module
    --
    -- This module locates the beginning of an EDH packet in the incoming video
    -- stream. It asserts the tx_edh_next siganl the sample before the EDH 
    -- packet begins on vid_in.
    --
    LOC : edh_loc
        port map (
            clk                 => clk,
            ce                  => ce,
            rst                 => rst,
            f                   => dec_f,
            vcnt                => dec_vcnt,
            hcnt                => dec_hcnt,
            std                 => dec_std,
            edh_next            => tx_edh_next);

    --
    -- anc_rx
    --
    -- This module calculates a checksum value for each ANC packet in the video
    -- stream and compares this checksum with the CS word in the ANC packet. If
    -- a difference is found, the anc_edh_local signal is asserted and stays
    -- asserted until the edh_packet signal indicates that the next EDH packet 
    -- has been sent.
    --
    ANCRX : anc_rx
        port map (
            clk                 => clk,
            ce                  => ce,
            rst                 => rst,
            locked              => dec_locked,
            rx_anc_next         => dec_anc_next,
            rx_edh_next         => dec_edh_next,
            edh_packet          => tx_edh_packet,
            vid_in              => dec_vid,
            anc_edh_local       => anc_edh_local);


    --
    -- edh_flags module
    --
    -- This module creates the error flags that are included in the new
    -- EDH packet created by the GEN module. It also captures those flags until 
    -- the next EDH packet and provides them as outputs.
    --
    FLAGS : edh_flags 
        port map (
            clk                 => clk,
            ce                  => ce,
            rst                 => rst,
            receive_mode        => receive_mode,
            ap_flag_word        => ap_flag_word,
            ff_flag_word        => ff_flag_word,
            anc_flag_word       => anc_flag_word,
            edh_missing         => edh_missing,
            edh_parity_err      => edh_parity_err,
            edh_format_err      => edh_format_err,
            rx_ap_crc_valid     => rx_ap_crc_valid,
            rx_ap_crc           => rx_ap_crc,
            rx_ff_crc_valid     => rx_ff_crc_valid,
            rx_ff_crc           => rx_ff_crc,
            rx_ap_flags         => in_ap_flags,
            rx_ff_flags         => in_ff_flags,
            rx_anc_flags        => in_anc_flags,
            anc_edh_local       => anc_edh_local,
            anc_idh_local       => anc_idh_local,
            anc_ues_local       => anc_ues_local,
            ap_idh_local        => ap_idh_local,
            ff_idh_local        => ff_idh_local,
            calc_ap_crc_valid   => ap_crc_valid,
            calc_ap_crc         => ap_crc,
            calc_ff_crc_valid   => ff_crc_valid,
            calc_ff_crc         => ff_crc,
            flags               => flag_bus,
            ap_flags            => ap_flags_int,
            ff_flags            => ff_flags_int,
            anc_flags           => anc_flags_int);

    ap_flags <= ap_flags_int;
    ff_flags <= ff_flags_int;
    anc_flags <= anc_flags_int;

    --
    -- edh_errcnt module
    --
    -- This counter increments once for every field that contains an enabled 
    -- error.
    --

    edh_all_flags <= (edh_chksum_err & ap_flags_int & ff_flags_int & anc_flags_int);

    ERRCNTR : edh_errcnt
        port map (
            clk                 => clk,
            ce                  => ce,
            rst                 => rst,
            clr_errcnt          => clr_errcnt,
            count_en            => errcnt_en,
            flag_enables        => errcnt_flg_en,
            flags               => edh_all_flags,
            edh_next            => tx_edh_next,
            errcnt              => errcnt);

    --
    -- This logic generates an enable for the error counter. The error counter
    -- will be disabled after reset. It remains disabled until after the video
    -- decoder has locked to the input video stream and the first EDH packet has
    -- been received.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            errcnt_en <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (dec_locked = '1' and dec_edh_next = '1') then
                    errcnt_en <= '1';
                end if;
            end if;
        end if;
    end process;

    -- 
    -- anc_demux module
    --
    -- This module searches for matching ANC packets in the video stream and
    -- demultiplexes them, making them available separately from the video 
    -- stream. Demultiplexed packets can be optionally deleted from the video 
    -- stream.
    --
    DMUX : anc_demux
        port map (
            clk                 => clk,
            ce                  => ce,
            rst                 => rst,
            vid_in              => dec_vid,
            en_a                => en_a,
            did_a               => did_a,
            sdid_a              => sdid_a,
            del_pkt_a           => del_pkt_a,
            en_b                => en_b,
            did_b               => did_b,
            sdid_b              => sdid_b,
            del_pkt_b           => del_pkt_b,
            en_c                => en_c,
            did_c               => did_c,
            sdid_c              => sdid_c,
            del_pkt_c           => del_pkt_c,
            en_d                => en_d,
            did_d               => did_d,
            sdid_d              => sdid_d,
            del_pkt_d           => del_pkt_d,
            in_trs              => dec_trs,
            in_f                => dec_f,
            in_v                => dec_v,
            in_h                => dec_h,
            in_hcnt             => dec_hcnt,
            in_vcnt             => dec_vcnt,
            in_sync_switch      => dec_sync_switch,
            in_eav_next         => dec_eav_next,
            in_sav_next         => dec_sav_next,
            in_xyz_word         => dec_xyz_word,
            in_anc_next         => dec_anc_next,
            in_edh_next         => dec_edh_next,
            in_edh_loc          => tx_edh_next,
            data_out            => anc_out,
            data_out_valid      => anc_out_valid,
            match_code          => anc_match,
            did                 => did,
            dbn                 => dbn,
            sdid                => sdid,
            dc                  => dc,
            udw                 => udw,
            cs                  => cs,
            vid_out             => dmux_vid,
            out_trs             => dmux_trs,
            out_f               => dmux_f,
            out_v               => dmux_v,
            out_h               => dmux_h,
            out_hcnt            => dmux_hcnt,
            out_vcnt            => dmux_vcnt,
            out_sync_switch     => dmux_sync_switch,
            out_eav_next        => dmux_eav_next,
            out_sav_next        => dmux_sav_next,
            out_xyz_word        => dmux_xyz_word,
            out_anc_next        => dmux_anc_next,
            out_edh_next        => dmux_edh_next,
            out_edh_loc         => dmux_edh_loc);

    --
    -- anc_mux module
    --
    -- This module will search for free ANC space and insert an ANC packet into 
    -- that space.
    --
    MUX : anc_mux 
        port map (
            clk                 => clk,
            ce                  => ce,
            rst                 => rst,
            vid_in              => dmux_vid,
            std                 => dec_std,
            hanc_pkt            => hanc_pkt,
            vanc_pkt            => vanc_pkt,
            pkt_rdy_in          => pkt_rdy_in,
            calc_udw_parity     => calc_udw_parity,
            data_in             => anc_in,
            ld_did              => ld_did,
            ld_dbn              => ld_dbn,
            ld_dc               => ld_dc,
            ld_udw              => ld_udw,
            udw_wr_adr          => udw_wr_adr,
            in_trs              => dmux_trs,
            in_f                => dmux_f,
            in_v                => dmux_v,
            in_h                => dmux_h,
            in_hcnt             => dmux_hcnt,
            in_vcnt             => dmux_vcnt,
            in_sync_switch      => dmux_sync_switch,
            in_eav_next         => dmux_eav_next,
            in_sav_next         => dmux_sav_next,
            in_xyz_word         => dmux_xyz_word,
            in_anc_next         => dmux_anc_next,
            in_edh_next         => dmux_edh_next,
            in_edh_loc          => dmux_edh_loc,
            vid_out             => mux_vid,
            pkt_in_empty        => pkt_in_empty,
            out_trs             => mux_trs,
            out_f               => mux_f,
            out_v               => mux_v,
            out_h               => mux_h,
            out_hcnt            => mux_hcnt,
            out_vcnt            => mux_vcnt,
            out_sync_switch     => mux_sync_switch,
            out_eav_next        => mux_eav_next,
            out_sav_next        => mux_sav_next,
            out_xyz_word        => mux_xyz_word,
            out_anc_next        => mux_anc_next,
            out_edh_next        => mux_edh_next,
            out_edh_loc         => mux_edh_loc);


    --
    -- edh_gen module
    --
    -- This module generates a new EDH packet based on the calculated CRC words
    -- and the incoming and local flags.
    --

    GEN : edh_gen
        port map (
            clk                 => clk,
            ce                  => ce,
            rst                 => rst,
            vid_in              => mux_vid,
            std                 => dec_std,
            locked              => dec_locked,
            flag_bus            => flag_bus,
            edh_missing         => edh_missing,
            in_trs              => mux_trs,
            in_f                => mux_f,
            in_v                => mux_v,
            in_h                => mux_h,
            in_hcnt             => mux_hcnt,
            in_vcnt             => mux_vcnt,
            in_sync_switch      => mux_sync_switch,
            in_eav_next         => mux_eav_next,
            in_sav_next         => mux_sav_next,
            in_xyz_word         => mux_xyz_word,
            in_anc_next         => mux_anc_next,
            in_edh_next         => mux_edh_next,
            in_edh_loc          => mux_edh_loc,
            vid_out             => vid_out,
            ap_flag_word        => ap_flag_word,
            ff_flag_word        => ff_flag_word,
            anc_flag_word       => anc_flag_word,
            edh_packet          => tx_edh_packet,
            out_trs             => trs,
            out_f               => field,
            out_v               => v_blank,
            out_h               => h_blank,
            out_hcnt            => horz_count,
            out_vcnt            => vert_count,
            out_sync_switch     => sync_switch,
            out_eav_next        => eav_next,
            out_sav_next        => sav_next,
            out_xyz_word        => xyz_word,
            out_anc_next        => anc_next,
            out_edh_next        => edh_next);

    --
    -- packet_flags
    --
    -- The various error flags related to the format of received EDH packets are
    -- assigned to an error flag vector here.
    --
    packet_flags <= (edh_format_err & edh_chksum_err & edh_parity_err & edh_missing);

    --
    -- misc outputs
    --
    -- These statements assign output ports to several misc. signals.
    --
    edh_packet <= tx_edh_packet;
    std <= dec_std;
    std_locked <= dec_std_locked;
    locked <= dec_locked;

end synth;