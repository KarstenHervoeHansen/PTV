-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: test_anc_mux.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2005-01-18 10:44:24-07 $
-- /___/   /\    Date Created: Jan 5, 2005
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: test_anc_mux.vhd,rcs $
-- Revision 1.0  2005-01-18 10:44:24-07  jsnow
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
-- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
-- IMPORTANT NOTE:
--
-- This testbench uses shared variables and VHDL '93 file I/O syntax. It must
-- be compiled by a VHDL '93 compliant compiler. In ModelSim, the default is
-- to generated errors on VHDL '93 specific syntax and the -93 flag must be
-- used.
--!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
--
-- This testbench tests the ANC packet insert function of the anc_edh_processor
-- module.
-- 
-- The testbench reads a full frame of video data from the one_frame.txt file 
-- into a memory array. This frame of video is fed into the anc_edh_processor 
-- repeatedly until the testbench terminates.
-- 
-- The testbench begins by loading information about the ANC packet to be 
-- inserted in the video stream. A DID value of h'c5 and a DBN value of 0 are 
-- used. The data count value (DC) varies from packet to packet, depending on 
-- the specific test situation. The UDW words loaded into ANC packet generator 
-- begin at zero for the first word and increment by one for each word up to 255.
--  
-- To test the ANC packet insert function, the testbench first waits for the
-- anc_edh_processor module to lock to the video stream, as indicated by the
-- assertion of the locked output. Once lock is achieved, the testbench waits 
-- for vertical line number 1. The first few video lines in one_frame.txt have 
-- extra ANC packets inserted in the HANC space to facilitate testing of the ANC 
-- packet insertion function. Once line 1 begins, the testbench loads a DC value
-- and asserts the pkt_rdy_in signal repeatedly to cause an ANC packet to be 
-- inserted several times over a period of 13 video lines.
-- 
-- Line 1: In one_frame.txt, the HANC space of line 1 contains a 55-word long 
-- ANC packet followed by an end_marker packet. Including the space occupied by
-- the end_marker packet, there is 213 words of space available in the HANC 
-- space. The testbench attempts to insert a new ANC packet that is exactly 213 
-- words long.
-- 
-- Line 2: In one_frame.txt, the HANC space of line 2 contains several ANC 
-- packets followed by an end_marker packet. Including the space occupied by the 
-- end_marker packet, there are 10 words of free space. The testbench attempts 
-- to insert an ANC packet that is 9 words long.
-- 
-- Line 3/4: In one_frame.txt, there are 42 free words available in the HANC 
-- space of line 3 and 43 free words in the HANC space of line 4. The testbench 
-- attempts to insert an ANC packet that is 43 words long. This module should 
-- not insert this packet in line 3 due to lack of space, but it should insert 
-- it in line 4.
-- 
-- Line 5: In one_frame.txt, the HANC space begins with a 43-word long ANC 
-- packet. There is no end_marker packet. There are 225 free words in the HANC 
-- space. The testbench attempts to insert an ANC packet that is exactly 225 
-- words long.
-- 
-- Line 6: In one_frame.txt, the HANC space begins with a 43-word long ANC 
-- packet and there is no end_marker packet. The testbench attempts to insert an
-- ANC packet that is 23 words long.
-- 
-- Line 7: In one_frame.txt, the HANC space of line 7 begins with a conforming 
-- 43-word long ANC packet followed by a start_marker packet, 175 words of
-- non-conforming ANC data, then an end_marker packet. Including the end_marker
-- packet, there are 43 words of available HANC space. The testbench attempts to
-- insert an ANC packet that is 43 words long.
-- 
-- Line 8: In one_frame.txt, the HANC space of line 8 contains one ANC packet 
-- that is 43 words long with no end_marker packet. The testbench attempts to 
-- insert a 43-word long ANC packet.
-- 
-- Line 9/10: In one_frame.txt, the HANC space of line 9 contains a 55-word long
-- ANC packet, a start_marker packet, 141 words of non-conforming ANC data, an
-- end_marker packet, 35 free words, and the EDH packet. Including the 
-- end_marker packet, there are 42 words of free space. The testbench attempts 
-- to insert an ANC packet that is 43 words long. This shouldn't fit if the 
-- space-available calculation correctly takes into consideration the EDH 
-- packet. Instead, the ANC packet should be inserted on line 10 which an 
-- entirely empty ANC space in the one_frame.txt file.
-- 
-- Line 10/11: The VANC space of line 10 and the HANC space of line 11 are keep-
-- out areas due to synchronous switching. After the ANC packet is inserted in 
-- the HANC space of line 10, the testbench immediately sets up an ANC packet 
-- that can be inserted in either the HANC or VANC space. The module should not 
-- insert this packet until the VANC space of line 11 due to the keep out areas.
-- 
-- Line 12: The testbench simply tries to insert a HANC packet that is 43 words
-- long. This should easily fit in the HANC space of line 12. In this case, the
-- calc_udw_parity input signal is asserted to cause the module to generate 
-- parity bits for the UDW data.
-- 
-- Line 13: The testbench attempts to insert a 262-word long ANC packet and
-- indicates that it can go in either the HANC or VANC space. This packet is too
-- long to fit in the available HANC space of line 13 and should be inserted in 
-- the VANC space.
--

library ieee;
use std.textio.all;
use ieee.std_logic_textio.all;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;
use work.anc_edh_pkg.all;

entity test_anc_mux is
    constant MAX_MEM : integer := 900900; -- Size of memory needed to hold 1 frame NTSC
    type mem_type is array (MAX_MEM - 1 downto 0) of video_type;    
end;

architecture sim of test_anc_mux is

shared variable memory :    mem_type;       -- Memory array holding test video


--------------------------------------------------------------------------------
-- Constant definitions
--  
constant FIRST_EDH  : integer := 13977;     -- Position in memory of start of 1st EDH packet
constant SECOND_EDH : integer := 465285;    -- Position in memory of start of 2nd EDH packet

-- These constants specify the positions of some words in the ANC packet.
constant DID_WORD   : integer := 3;
constant DBN_WORD   : integer := 4;
constant DC_WORD    : integer := 5;

-- These constants define the values of certain words in inserted ANC packets.
constant ADF0       : video_type := "0000000000";
constant ADF1       : video_type := "1111111111";
constant INSERT_DID : ubyte_type := ubyte_type(TO_UNSIGNED(16#c5#, ubyte_type'length));
constant INSERT_DBN : ubyte_type := "00000000";

-- These constants give the starting positions of inserted ANC packets along
-- with the DC value indicating the length of the ANC packet and the resulting
-- checksum word.
constant LINE1_INS_START : hpos_type    := hpos_type(TO_UNSIGNED(1499, hpos_type'length));
constant LINE1_DC        : ubyte_type   := ubyte_type(TO_UNSIGNED(206, ubyte_type'length));
constant LINE1_CS        : video_type   := video_type(TO_UNSIGNED(16#10e#, video_type'length));

constant LINE2_INS_START : hpos_type    := hpos_type(TO_UNSIGNED(1702, hpos_type'length));
constant LINE2_DC        : ubyte_type   := ubyte_type(TO_UNSIGNED(2, ubyte_type'length));
constant LINE2_CS        : video_type   := video_type(TO_UNSIGNED(16#1c8#, video_type'length));

constant LINE4_INS_START : hpos_type    := hpos_type(TO_UNSIGNED(1669, hpos_type'length));
constant LINE4_DC        : ubyte_type   := ubyte_type(TO_UNSIGNED(36, ubyte_type'length));
constant LINE4_CS        : video_type   := video_type(TO_UNSIGNED(16#15f#, video_type'length));

constant LINE5_INS_START : hpos_type    := hpos_type(TO_UNSIGNED(1487, hpos_type'length));
constant LINE5_DC        : ubyte_type   := ubyte_type(TO_UNSIGNED(218, ubyte_type'length));
constant LINE5_CS        : video_type   := video_type(TO_UNSIGNED(16#104#, video_type'length));

constant LINE6_INS_START : hpos_type    := hpos_type(TO_UNSIGNED(1487, hpos_type'length));
constant LINE6_DC        : ubyte_type   := ubyte_type(TO_UNSIGNED(16, ubyte_type'length));
constant LINE6_CS        : video_type   := video_type(TO_UNSIGNED(16#24d#, video_type'length));

constant LINE7_INS_START : hpos_type    := hpos_type(TO_UNSIGNED(1669, hpos_type'length));
constant LINE7_DC        : ubyte_type   := ubyte_type(TO_UNSIGNED(36, ubyte_type'length));
constant LINE7_CS        : video_type   := video_type(TO_UNSIGNED(16#15f#, video_type'length));

constant LINE8_INS_START : hpos_type    := hpos_type(TO_UNSIGNED(1487, hpos_type'length));
constant LINE8_DC        : ubyte_type   := ubyte_type(TO_UNSIGNED(36, ubyte_type'length));
constant LINE8_CS        : video_type   := video_type(TO_UNSIGNED(16#15f#, video_type'length));

constant LINE10_INS_START: hpos_type    := hpos_type(TO_UNSIGNED(1444, hpos_type'length));
constant LINE10_DC       : ubyte_type   := ubyte_type(TO_UNSIGNED(36, ubyte_type'length));
constant LINE10_CS       : video_type   := video_type(TO_UNSIGNED(16#15f#, video_type'length));

constant LINE11_INS_START: hpos_type    := hpos_type(TO_UNSIGNED(0, hpos_type'length)); 
constant LINE11_DC       : ubyte_type   := ubyte_type(TO_UNSIGNED(36, ubyte_type'length));
constant LINE11_CS       : video_type   := video_type(TO_UNSIGNED(16#15f#, video_type'length));

constant LINE12_INS_START: hpos_type    := hpos_type(TO_UNSIGNED(1487, hpos_type'length));
constant LINE12_DC       : ubyte_type   := ubyte_type(TO_UNSIGNED(36, ubyte_type'length));
constant LINE12_CS       : video_type   := video_type(TO_UNSIGNED(16#15f#, video_type'length));

constant LINE13_INS_START: hpos_type    := hpos_type(TO_UNSIGNED(0, hpos_type'length));  
constant LINE13_DC       : ubyte_type   := ubyte_type(TO_UNSIGNED(255, ubyte_type'length));
constant LINE13_CS       : video_type   := video_type(TO_UNSIGNED(16#245#, video_type'length));

constant END_LINE        : vpos_type    := vpos_type(TO_UNSIGNED(14, vpos_type'length));

--
-- This constant specifies the number of cycles of clock latency through the
-- anc_edh_processor, including video decoder latency.
--
constant LATENCY         : integer      := 10;

--------------------------------------------------------------------------------
-- Signal definitions
--

--
-- These signals connect to the modules in the test bench
--
signal clk :            std_ulogic := '0';  -- clock signal
signal enclk :          std_ulogic := '1';  -- clock enable signal
signal rst :            std_ulogic := '1';  -- reset signal
signal vid_in :         video_type;         -- output of memory array
signal std_locked :     std_ulogic;         -- video standard dectector locked
signal std:             vidstd_type;        -- video standard code
signal trs:             std_ulogic;         -- video decoder TRS
signal vid_out:         video_type;         -- video output of edh_processor
signal f:               std_ulogic;         -- field indicator
signal v:               std_ulogic;         -- vertical blanking indicator
signal h:               std_ulogic;         -- horizontal blanking indicator
signal hcnt:            hpos_type;          -- horizontal counter
signal vcnt:            vpos_type;          -- vertical counter
signal sync_switch:     std_ulogic;         -- sync switching line
signal locked:          std_ulogic;         -- flywheel locked
signal eav_next:        std_ulogic;         -- EAV is next
signal sav_next:        std_ulogic;         -- SAV is next
signal xyz_word:        std_ulogic;         -- current word is XYZ word
signal anc_next:        std_ulogic;         -- ANC is next
signal edh_next:        std_ulogic;         -- EDH is next
signal rx_ce:           std_ulogic;         -- clock enable
signal edh_packet:      std_ulogic;         -- edh processor EDH packet indicator
signal edh_missing:     std_ulogic;         -- edh packet was missing
signal edh_parity_err:  std_ulogic;         -- edh packet parity error
signal edh_chksum_err:  std_ulogic;         -- edh packet checksum error
signal edh_format_err:  std_ulogic;         -- edh packet format error
signal rx_ap_flags:     edh_flgset_type;    -- received edh packet AP flags
signal rx_ff_flags:     edh_flgset_type;    -- received edh packet FF flags
signal rx_anc_flags:    edh_flgset_type;    -- recevied edh packet ANC flags
signal ap_flags:        edh_flgset_type;    -- transmitted edh packet AP flags
signal ff_flags:        edh_flgset_type;    -- transmitted edh packet FF flags
signal anc_flags:       edh_flgset_type;    -- transmitted edh packet ANC flags
signal errcnt:          edh_errcnt_type;    -- count of fields containing EDH errors
signal anc_idh_local:   std_ulogic := '0';  -- controls the ANC IDH local input to EDH proc
signal anc_ues_local:   std_ulogic := '0';  -- controls the ANC UES local input to EDH proc
signal ap_idh_local:    std_ulogic := '0';  -- controls the AP IDH local input to EDH proc
signal ff_idh_local:    std_ulogic := '0';  -- controls the FF IDH local input to EDH proc
signal errcnt_flg_en:   edh_allflg_type     -- controls the error count flag enables
        := "1111111111111111"; 
signal hanc_pkt:        std_ulogic;         -- asserted if ANC packet is to be inserted in HANC space
signal vanc_pkt:        std_ulogic;         -- asserted if ANC packet is to be inserted in VACN space
signal pkt_rdy_in:      std_ulogic;         -- asserted when ANC packet is ready to send
signal calc_udw_parity: std_ulogic;         -- causes module to compute parity for UDW words
signal anc_in:          video_type;         -- ANC packet input data bus
signal ld_did:          std_ulogic;         -- Loads DID word of ANC packet
signal ld_dbn:          std_ulogic;         -- Loads DBN/SDID word of ANC packet
signal ld_dc:           std_ulogic;         -- Loads the DC word of ANC packet
signal ld_udw:          std_ulogic;         -- Loads the UDW words of ANC packet
signal udw_wr_adr:      udwadr_type;        -- UDW RAM write address
signal pkt_in_empty:    std_ulogic;         -- status output of ANC MUX
signal packet_flags :   edh_pktflg_type;    -- vector containing the packet error flags listed below

--
-- These signals control the testbench
--
type dly_video_type     is array (LATENCY-1 downto 0) of video_type;

signal i:               integer;            -- index into memory array
signal frames:          integer;            -- counts number of frames during test
signal error_found:     boolean := false;   -- asserted when a simulation error is detected
signal expected_vid:    video_type;         -- expected video value
signal dly_vid:         dly_video_type;     -- delays vid_in for output comparison
signal ins_start:       hpos_type;          -- ANC packet insertion starting position
signal dc_val:          ubyte_type;         -- inserted ANC packet DC value
signal cs_val:          video_type;         -- inserted ANC packet CS value

signal VCC :            std_ulogic := '1';
signal GND :            std_ulogic := '0';
signal GND8 :           ubyte_type := "00000000";

component anc_edh_processor
    port (
        clk:            in  std_ulogic;     -- clock input
        ce:             in  std_ulogic;     -- clock enable
        rst:            in  std_ulogic;     -- async reset input

    -- video decoder inputs
        vid_in:         in  video_type;     -- input video
        reacquire:      in  std_ulogic;     -- forces autodetect to reacquire the video standard
        en_sync_switch: in  std_ulogic;     -- enables synchronous switching
        en_trs_blank:   in  std_ulogic;     -- enables TRS blanking when asserted

        -- EDH flag inputs
        anc_idh_local:  in  std_ulogic;     -- ANC IDH flag input
        anc_ues_local:  in  std_ulogic;     -- ANC UES flag input
        ap_idh_local:   in  std_ulogic;     -- AP IDH flag input
        ff_idh_local:   in  std_ulogic;     -- FF IDH flag input
        errcnt_flg_en:  in  edh_allflg_type;-- selects which error flags increment the error counter
        clr_errcnt:     in  std_ulogic;     -- clears the error counter
        receive_mode:   in  std_ulogic;     -- 1 enables receiver, 0 for generate only

        -- ANC demux inputs
        en_a:           in  std_ulogic;     -- enable for DID A decoder
        did_a:          in  ubyte_type;     -- first DID code to match
        sdid_a:         in  ubyte_type;     -- first SDID code to match
        del_pkt_a:      in  std_ulogic;     -- packet will be deleted if asserted
        en_b:           in  std_ulogic;     -- enable for DID B decoder
        did_b:          in  ubyte_type;     -- second DID code to match
        sdid_b:         in  ubyte_type;     -- second SDID code to match
        del_pkt_b:      in  std_ulogic;     -- packet will be deleted if asserted
        en_c:           in  std_ulogic;     -- enable for DID C decoder
        did_c:          in  ubyte_type;     -- third DID code to match
        sdid_c:         in  ubyte_type;     -- third SDID code to match
        del_pkt_c:      in  std_ulogic;     -- packet will be deleted if asserted
        en_d:           in  std_ulogic;     -- enable for DID D decoder
        did_d:          in  ubyte_type;     -- fourth DID code to match
        sdid_d:         in  ubyte_type;     -- fourth SDID code to match
        del_pkt_d:      in  std_ulogic;     -- packet will be deleted if asserted

        -- ANC mux inputs
        hanc_pkt:       in  std_ulogic;     -- ANC packet to be sent can be inserted into HANC space
        vanc_pkt:       in  std_ulogic;     -- ANC packet to be sent can be inserted into VANC space
        pkt_rdy_in:     in  std_ulogic;     -- indicates packet has been loaded into input regs
        calc_udw_parity:in  std_ulogic;     -- indicates that module should calculate parity on UDW
        anc_in:         in  video_type;     -- input data bus
        ld_did:         in  std_ulogic;     -- load DID register
        ld_dbn:         in  std_ulogic;     -- load SDID/DBN register
        ld_dc:          in  std_ulogic;     -- load DC register
        ld_udw:         in  std_ulogic;     -- loads the UDW RAM
        udw_wr_adr:     in  udwadr_type;        -- UDW RAM write address

        -- video and decoded video timing outputs
        vid_out:        out video_type;     -- output video stream with EDH packets inserted
        std:            out vidstd_type;    -- video standard code
        std_locked:     out std_ulogic;     -- video standard detector is locked
        trs:            out std_ulogic;     -- asserted during flywheel generated TRS symbol
        field:          out std_ulogic;     -- field indicator
        v_blank:        out std_ulogic;     -- vertical blanking indicator
        h_blank:        out std_ulogic;     -- horizontal blanking indicator
        horz_count:     out hpos_type;      -- horizontal position
        vert_count:     out vpos_type;      -- vertical position
        sync_switch:    out std_ulogic;     -- asserted on lines where synchronous switching is allowed
        locked:         out std_ulogic;     -- asserted when flywheel is synchronized to video
        eav_next:       out std_ulogic;     -- next word is first word of EAV
        sav_next:       out std_ulogic;     -- next word is first word of SAV
        xyz_word:       out std_ulogic;     -- current word is the XYZ word of a TRS
        anc_next:       out std_ulogic;     -- next word is first word of a received ANC packet
        edh_next:       out std_ulogic;     -- next word is first word of a received EDH packet

        -- EDH flag outputs
        rx_ap_flags:    out edh_flgset_type;-- AP error flags received from last EDH packet
        rx_ff_flags:    out edh_flgset_type;-- FF error flags received from last EDH packet
        rx_anc_flags:   out edh_flgset_type;-- ANC error flags freceived from last EDH packet
        ap_flags:       out edh_flgset_type;-- transmitted AP error flags from last field
        ff_flags:       out edh_flgset_type;-- transmitted FF error flags from last field
        anc_flags:      out edh_flgset_type;-- transmitted ANC error flags from last field
        packet_flags:   out edh_pktflg_type;-- error flags related to the received packet processing
        errcnt:         out edh_errcnt_type;-- errored fields counter
        edh_packet:     out std_ulogic;     -- asserted during all words of a generated EDH packet

        -- ANC demux outputs
        anc_out:        out video_type;     -- output packet data
        anc_out_valid:  out std_ulogic;     -- asserted while all words of a matching packet are on anc_out
        anc_match:      out                 -- indicates DID/SDID combination matched the current packet
            std_ulogic_vector(1 downto 0);
        did:            out std_ulogic;     -- asserted when a DID word from a matching packet is on anc_out
        dbn:            out std_ulogic;     -- asserted when a DBN word from a matching packet is on anc_out
        sdid:           out std_ulogic;     -- asserted when an SDID word from a matching packet is on anc_out
        dc:             out std_ulogic;     -- asserted when a DC word from a matching packet is on anc_out
        udw:            out std_ulogic;     -- asserted when a UDW word from a matching packet is on anc_out
        cs:             out std_ulogic;     -- asserted when a CS word from a matching packet is on anc_out

        -- ANC mux outputs
        pkt_in_empty:   out std_ulogic);    -- module is ready for input packet to be loaded

end component;

--
-- pad2_10 function
--
-- This function takes an 8-bit value, computes a parity bit, then creates a
-- 10-bit word with the complement of the parity bit in bit 9 and the parity
-- bit in bit 8, followed by the original 8-bit value.
--          
function pad2_10(d: ubyte_type) return video_type is
variable parity : std_ulogic;
begin
    parity := d(7) xor d(6) xor d(5) xor d(4) xor d(3) xor d(2) xor d(1) xor d(0);
    return(not parity & parity & d);    
end pad2_10;

begin
    
    --
    -- Instantiate the ANC/EDH processor module
    --
    U1 : anc_edh_processor
        port map (
        clk             => clk,
        ce              => rx_ce,
        rst             => rst,
        vid_in          => vid_in,
        reacquire       => GND,
        en_sync_switch  => VCC,
        en_trs_blank    => VCC,
        anc_idh_local   => anc_idh_local,
        anc_ues_local   => anc_ues_local,
        ap_idh_local    => ap_idh_local,
        ff_idh_local    => ff_idh_local,
        errcnt_flg_en   => errcnt_flg_en,
        clr_errcnt      => GND,
        receive_mode    => VCC,
        en_a            => GND,
        did_a           => GND8,
        sdid_a          => GND8,
        del_pkt_a       => GND,
        en_b            => GND,
        did_b           => GND8,
        sdid_b          => GND8,
        del_pkt_b       => GND,
        en_c            => GND,
        did_c           => GND8,
        sdid_c          => GND8,
        del_pkt_c       => GND,
        en_d            => GND,
        did_d           => GND8,
        sdid_d          => GND8,
        del_pkt_d       => GND,
        hanc_pkt        => hanc_pkt,
        vanc_pkt        => vanc_pkt,
        pkt_rdy_in      => pkt_rdy_in,
        calc_udw_parity => calc_udw_parity,
        anc_in          => anc_in,
        ld_did          => ld_did,
        ld_dbn          => ld_dbn,
        ld_dc           => ld_dc,
        ld_udw          => ld_udw,
        udw_wr_adr      => udw_wr_adr,
        vid_out         => vid_out,
        std             => std,
        std_locked      => std_locked,
        trs             => trs,
        field           => f,
        v_blank         => v,
        h_blank         => h,
        horz_count      => hcnt,
        vert_count      => vcnt,
        sync_switch     => sync_switch,
        locked          => locked,
        eav_next        => eav_next,
        sav_next        => sav_next,
        xyz_word        => xyz_word,
        anc_next        => anc_next,
        edh_next        => edh_next,
        rx_ap_flags     => rx_ap_flags,
        rx_ff_flags     => rx_ff_flags,
        rx_anc_flags    => rx_anc_flags,
        ap_flags        => ap_flags,
        ff_flags        => ff_flags,
        anc_flags       => anc_flags,
        packet_flags    => packet_flags,
        errcnt          => errcnt,
        edh_packet      => edh_packet,
        anc_out         => open,
        anc_out_valid   => open,
        anc_match       => open,
        did             => open,
        dbn             => open,
        sdid            => open,
        dc              => open,
        udw             => open,
        cs              => open,
        pkt_in_empty    => pkt_in_empty);

    edh_format_err <= packet_flags(3);
    edh_chksum_err <= packet_flags(2);
    edh_parity_err <= packet_flags(1);
    edh_missing    <= packet_flags(0);


    --
    -- Read in one frame of NTSC video into memory array
    --
    process
    file     infile:    TEXT open read_mode is "C:/work/XAPP299/sim/one_frame.txt";
    variable buf:       line;
    variable data:      std_logic_vector(11 downto 0);
    variable words:     integer := 0;
    variable good:      boolean;
     
    begin
        while not (endfile(infile)) loop
            readline(infile, buf);              -- read a line from file into buffer
            for i in 0 to 15 loop               -- there are 16 words per line in the file
                exit when words = MAX_MEM;      -- last line doesn't contain 16 words so put an early escape mechanism here
                hread(buf, data, good);         -- read one word from the buffer
                assert good                     -- make sure the read was OK
                    report "Text I/O read error"
                    severity failure;
                memory(words) := video_type(data(9 downto 0));  -- write word to the memory array
                words := words + 1;
            end loop;
        end loop;
        wait;   
    end process;

            
    --
    -- Generate a clock signal at 27 MHz.
    --
    clk <= enclk and not clk after 18.5 ns;
    rx_ce <= '1';

    --
    -- Assert the reset signal for the first few clock cycles
    --
    process
    begin
        for count in 1 to 5 loop
            wait until clk'event and clk = '0';
        end loop;
        rst <= '0';
        wait;
    end process;

    --
    -- This code generates the index into the memory array and the frame counter.
    -- The index begins at a negative number. When the index is negative, no video
    -- is generated. A few cycles with no video are used to make sure the modules
    -- come out of reset before the test set is sent. When the index reaches the
    -- maximum size of the memory array, it is reset to zero and the frame counter
    -- is incremented.
    --

    process(clk, rst)
    begin
        if (rst = '1') then
            i <= -5;
            frames <= 0;
        elsif (clk'event and clk = '1') then
            if (i = MAX_MEM - 1 and frames < 8) then
                i <= 0;
                frames <= frames + 1;
            else
                i <= i + 1;
            end if;
        end if; 
    end process;

    --
    -- Whenever the memory index changes, look up a new video data value from the
    -- memory array and assign that value to vid_in. 
    --
    process(i)
    begin
        if (i < 0 or i >= MAX_MEM or frames > 7) then
            vid_in <= (others => '0');
        else
            vid_in <= memory(i);
        end if;
    end process;

    --
    -- The following code scripts the insertion of ANC packets into the 
    -- anc_edh_processor module.
    --
    process
    begin
        pkt_rdy_in <= '0';
        hanc_pkt <= '1';
        vanc_pkt <= '0';
        ld_did <= '0';
        ld_dbn <= '0';
        ld_udw <= '0';
        ld_dc <= '0';
        calc_udw_parity <= '0';

        wait until (rst'event and rst = '0');
        for j in 1 to 5 loop
            wait until (clk'event and clk = '0');
        end loop;

        -- Load the UDW RAM with data.
        for j in 0 to 255 loop
            udw_wr_adr <= udwadr_type(TO_UNSIGNED(j, udw_wr_adr'length));
            anc_in <= video_type("1000000000" + std_logic_vector(TO_UNSIGNED(j, video_type'length)));
            ld_udw <= '1';
            wait until (clk'event and clk = '0');
        end loop;

        ld_udw <= '0';

        -- Write the DID value
        anc_in <= ("00" & INSERT_DID);
        ld_did <= '1';
        wait until (clk'event and clk = '0');
        
        -- Write the DBN value
        ld_did <= '0';
        anc_in <= ("00" & INSERT_DBN);
        ld_dbn <= '1';
        wait until (clk'event and clk = '0');

        -- Line1: Set the DC value for the first packet to 206, exactly the
        -- amount of available HANC space in Line 1.
        ld_dbn <= '0';
        anc_in <= ("00" & LINE1_DC);
        ld_dc <= '1';

        wait until (clk'event and clk = '0');
        ld_dc <= '0';

        -- Now wait for the video decoder to lock, then wait for vcnt == 1.
        wait until (locked'event and locked = '1');
        wait until (vcnt = 1);
        wait until (clk'event and clk = '0');
        pkt_rdy_in <= '1';

        wait until (pkt_in_empty'event and pkt_in_empty = '0');
        wait until (clk'event and clk = '0');
        pkt_rdy_in <= '0';

        wait until (pkt_in_empty'event and pkt_in_empty = '1');
        wait until (clk'event and clk = '0');

        -- Line 2: Insert a HANC packet of length 9.
        anc_in <= ("00" & LINE2_DC);
        ld_dc <= '1';
        wait until (clk'event and clk = '0');
        ld_dc <= '0';
        pkt_rdy_in <= '1';

        wait until (pkt_in_empty'event and pkt_in_empty = '0');
        wait until (clk'event and clk = '0');
        pkt_rdy_in <= '0';

        wait until (pkt_in_empty'event and pkt_in_empty = '1');
        wait until (clk'event and clk = '0');

        -- Line 3/4: Attempt to insert a HANC packet of length 43. This is
        -- one word too large for the available HANC space of line 3 and will
        -- be inserted in line 4.
        anc_in <= ("00" & LINE4_DC);
        ld_dc <= '1';
        wait until (clk'event and clk = '0');
        ld_dc <= '0';
        pkt_rdy_in <= '1';

        wait until (pkt_in_empty'event and pkt_in_empty = '0');
        wait until (clk'event and clk = '0');
        pkt_rdy_in <= '0';

        wait until (pkt_in_empty'event and pkt_in_empty = '1');
        wait until (clk'event and clk = '0');
        
        -- Line 5: Insert a 225-word HANC packet on line 5.
        anc_in <= ("00" & LINE5_DC);
        ld_dc <= '1';
        wait until (clk'event and clk = '0');
        ld_dc <= '0';
        pkt_rdy_in <= '1';

        wait until (pkt_in_empty'event and pkt_in_empty = '0');
        wait until (clk'event and clk = '0');
        pkt_rdy_in <= '0';

        wait until (pkt_in_empty'event and pkt_in_empty = '1');
        wait until (clk'event and clk = '0');

        -- Line 6: Insert a 23-word HANC packet on line 6.
        anc_in <= ("00" & LINE6_DC);
        ld_dc <= '1';
        wait until (clk'event and clk = '0');
        ld_dc <= '0';
        pkt_rdy_in <= '1';

        wait until (pkt_in_empty'event and pkt_in_empty = '0');
        wait until (clk'event and clk = '0');
        pkt_rdy_in <= '0';

        wait until (pkt_in_empty'event and pkt_in_empty = '1');
        wait until (clk'event and clk = '0');

        -- Line 7: Load a 43-word HANC packet on line 7.
        anc_in <= ("00" & LINE7_DC);
        ld_dc <= '1';
        wait until (clk'event and clk = '0');
        ld_dc <= '0';
        pkt_rdy_in <= '1';

        wait until (pkt_in_empty'event and pkt_in_empty = '0');
        wait until (clk'event and clk = '0');
        pkt_rdy_in <= '0';

        wait until (pkt_in_empty'event and pkt_in_empty = '1');
        wait until (clk'event and clk = '0');

        -- Line 8: Load a 43-word HANC packet on line 8.
        anc_in <= ("00" & LINE8_DC);
        ld_dc <= '1';
        wait until (clk'event and clk = '0');
        ld_dc <= '0';
        pkt_rdy_in <= '1';

        wait until (pkt_in_empty'event and pkt_in_empty = '0');
        wait until (clk'event and clk = '0');
        pkt_rdy_in <= '0';

        wait until (pkt_in_empty'event and pkt_in_empty = '1');
        wait until (clk'event and clk = '0');

        -- Line 9/10: Attempt to load a 43-word HANC packet on line 9. Because
        -- of the EDH packet, this is one-word too big for line 9 and will be
        -- inserted in the HANC space of line 10.
        anc_in <= ("00" & LINE10_DC);
        ld_dc <= '1';
        wait until (clk'event and clk = '0');
        ld_dc <= '0';
        pkt_rdy_in <= '1';

        wait until (pkt_in_empty'event and pkt_in_empty = '0');
        wait until (clk'event and clk = '0');
        pkt_rdy_in <= '0';

        wait until (pkt_in_empty'event and pkt_in_empty = '1');
        wait until (clk'event and clk = '0');

        -- Line 10/11: Attempt to insert a 43-word ANC packet in either HANC
        -- or VANC space beginning with the VANC space of line 10. Due to
        -- the synchronous-switching keep out areas, this packet should not
        -- load until the VANC space of line 11.
        vanc_pkt <= '1';
        anc_in <= ("00" & LINE11_DC);
        ld_dc <= '1';
        wait until (clk'event and clk = '0');
        ld_dc <= '0';
        pkt_rdy_in <= '1';

        wait until (pkt_in_empty'event and pkt_in_empty = '0');
        wait until (clk'event and clk = '0');
        pkt_rdy_in <= '0';

        wait until (pkt_in_empty'event and pkt_in_empty = '1');
        wait until (clk'event and clk = '0');
        vanc_pkt <= '0';

        -- Line 12: Load a 43-word ANC packet in the HANC space of line 12, but
        -- assert the calc_udw_parity signal.
        calc_udw_parity <= '1';
        anc_in <= ("00" & LINE12_DC);
        ld_dc <= '1';
        wait until (clk'event and clk = '0');
        ld_dc <= '0';
        pkt_rdy_in <= '1';

        wait until (pkt_in_empty'event and pkt_in_empty = '0');
        wait until (clk'event and clk = '0');
        pkt_rdy_in <= '0';

        wait until (pkt_in_empty'event and pkt_in_empty = '1');
        wait until (clk'event and clk = '0');
        calc_udw_parity <= '0';

        -- Reload the DC word with a value of 255. Set the vanc_pkt signal high.
        -- This packet should be too big to fit in the available space in the 
        -- HANC space and should get inserted into the VANC space of line 13.
        vanc_pkt <= '1';
        anc_in <= ("00" & LINE13_DC);
        wait until (trs'event and trs = '0');
        wait until (clk'event and clk = '0');
        ld_dc <= '1';
        wait until (clk'event and clk = '0');
        ld_dc <= '0';
        pkt_rdy_in <= '1';

        wait until (pkt_in_empty'event and pkt_in_empty = '0');
        wait until (clk'event and clk = '0');
        pkt_rdy_in <= '0';

        wait until (pkt_in_empty'event and pkt_in_empty = '1');
        wait until (clk'event and clk = '0');

        wait;
    end process;

    --
    -- This statement stops the simulation when the vcnt value reaches the END_LINE
    -- value the first time after the video_decoder locks. The simulation is stopped
    -- by disabling the clock.
    --
    process
        variable l : line;
        variable success_string :       string (1 to 36) 
                                        := "Test completed with no errors found.";
        variable error_string :         string (1 to 27)
                                        := "Test completed with errors.";
    begin
        wait until (locked'event and locked = '1');
        wait until (vcnt = END_LINE);

        if (error_found) then
            write(l, error_string);
            writeline(output, l);
        else
            write(l, success_string);
            writeline(output, l);
        end if;

        enclk <= '0';
        wait;
    end process;


    --
    -- This code generates expected video output values. Normally, the expected
    -- value is equal to the input video value, delayed appropriately to compensate
    -- for the latency through the processor. However, at certain places, ANC
    -- packets are inserted, so the code below creates expected values to match
    -- those inserted ANC packets.
    --

    -- This first process sets up the values for where the ANC packet should be
    -- inserted, what the length of the packet is, and what the checksum
    -- value should be.
    process(vcnt)
    begin
        case vcnt is
            when "0000000001" =>
                ins_start <= LINE1_INS_START;
                dc_val    <= LINE1_DC;
                cs_val    <= LINE1_CS;
            
            when "0000000010" =>
                ins_start <= LINE2_INS_START;
                dc_val    <= LINE2_DC;
                cs_val    <= LINE2_CS;

            when "0000000100" =>
                ins_start <= LINE4_INS_START;
                dc_val    <= LINE4_DC;
                cs_val    <= LINE4_CS;
            
            when "0000000101" =>
                ins_start <= LINE5_INS_START;
                dc_val    <= LINE5_DC;
                cs_val    <= LINE5_CS;
            
            when "0000000110" =>
                ins_start <= LINE6_INS_START;
                dc_val    <= LINE6_DC;
                cs_val    <= LINE6_CS;
            
            when "0000000111" =>
                ins_start <= LINE7_INS_START;
                dc_val    <= LINE7_DC;
                cs_val    <= LINE7_CS;
            
            when "0000001000" =>
                ins_start <= LINE8_INS_START;
                dc_val    <= LINE8_DC;
                cs_val    <= LINE8_CS;
            
            when "0000001010"  =>
                ins_start <= LINE10_INS_START;
                dc_val    <= LINE10_DC;
                cs_val    <= LINE10_CS;
            
            when "0000001011"  =>
                ins_start <= LINE11_INS_START;
                dc_val    <= LINE11_DC;
                cs_val    <= LINE11_CS;
            
            when "0000001100"  =>
                ins_start <= LINE12_INS_START;
                dc_val    <= LINE12_DC;
                cs_val    <= LINE12_CS;
            
            when "0000001101"  =>
                ins_start <= LINE13_INS_START;
                dc_val    <= LINE13_DC;
                cs_val    <= LINE13_CS;

            when others =>
                ins_start <= (others => '0');
                dc_val    <= (others => '0');
                cs_val    <= (others => '0');

        end case;   
    end process;

    -- This always statement generates the expected video values based on the
    -- ANC packet information from the always statement above.
    process(clk)
    variable temp_ubyte : ubyte_type;
    variable temp_hpos :  std_logic_vector(11 downto 0);
    begin
        if (clk'event and clk = '0') then
            expected_vid <= dly_vid(LATENCY - 1);

            if (locked = '1') then
                
                if (vcnt = vpos_type(TO_UNSIGNED( 1, vpos_type'length)) or
                    vcnt = vpos_type(TO_UNSIGNED( 2, vpos_type'length)) or  
                    vcnt = vpos_type(TO_UNSIGNED( 4, vpos_type'length)) or
                    vcnt = vpos_type(TO_UNSIGNED( 5, vpos_type'length)) or  
                    vcnt = vpos_type(TO_UNSIGNED( 6, vpos_type'length)) or  
                    vcnt = vpos_type(TO_UNSIGNED( 7, vpos_type'length)) or  
                    vcnt = vpos_type(TO_UNSIGNED( 8, vpos_type'length)) or  
                    vcnt = vpos_type(TO_UNSIGNED(10, vpos_type'length)) or  
                    vcnt = vpos_type(TO_UNSIGNED(11, vpos_type'length)) or  
                    vcnt = vpos_type(TO_UNSIGNED(12, vpos_type'length)) or  
                    vcnt = vpos_type(TO_UNSIGNED(13, vpos_type'length)))    
                 then
                    if (hcnt = ins_start) then
                        expected_vid <= ADF0;
                    elsif (hcnt = ins_start + 1 or
                           hcnt = ins_start + 2) then
                        expected_vid <= ADF1;
                    elsif (hcnt = ins_start + DID_WORD) then
                        expected_vid <= pad2_10(INSERT_DID);
                    elsif (hcnt = ins_start + DBN_WORD) then
                        expected_vid <= pad2_10(INSERT_DBN);
                    elsif (hcnt = ins_start + DC_WORD) then
                        expected_vid <= pad2_10(dc_val);
                    elsif (hcnt > ins_start + DC_WORD and
                           hcnt < ins_start + std_logic_vector(dc_val) + 6) then
                        if (vcnt = vpos_type(TO_UNSIGNED(12, vpos_type'length))) then
                            temp_hpos := std_logic_vector(hcnt - ins_start - 6);
                            temp_ubyte := ubyte_type(temp_hpos(7 downto 0));
                            expected_vid <= pad2_10(temp_ubyte);
                        else
                            temp_hpos := std_logic_vector(hcnt - ins_start - 6);
                            expected_vid <= video_type(temp_hpos(9 downto 0) or "1000000000"); 
                        end if;
                    elsif (hcnt = ins_start + std_logic_vector(dc_val) + 6) then
                        expected_vid <= cs_val;
                    end if;
                end if;
            end if;
        end if;
    end process;


    --
    -- Delay the vid_in value by the amount of latency in the anc_edh_processor
    -- module. The delayed video is used as a reference to verify that the video
    -- out of the module is correct.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            for k in 0 to LATENCY - 1 loop
                dly_vid(k) <= (others => '0');
            end loop;
        elsif (clk'event and clk = '1') then
            for k in 0 to LATENCY - 2 loop
                dly_vid(k+1) <= dly_vid(k);
            end loop;
            dly_vid(0) <= vid_in;
        end if;
    end process;


    --
    -- Compare expected with video out of processor except during the EDH packet.
    -- The EDH packet will not match the input EDH packet value due to EDH
    -- packet processing.
    --
    process(clk)
        variable l  : line;
        variable s1 : string (1 to 17) := "Error: vid_out = ";
        variable s2 : string (1 to 12) := " expected = ";
    begin
        if (clk'event and clk = '1') then
            if (locked = '1' and edh_packet = '0') then
                if (expected_vid /= vid_out) then
                    write(l, s1);
                    hwrite(l, ("00" & vid_out), left, vid_out'length + 2);
                    write(l, s2);
                    hwrite(l, ("00" & expected_vid), left, expected_vid'left + 2);
                    writeline(output, l);
                    error_found <= true;
                end if;
            end if;
        end if;
    end process;


end sim;