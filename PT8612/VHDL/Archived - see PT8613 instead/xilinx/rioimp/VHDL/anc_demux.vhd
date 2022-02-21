-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: anc_demux.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2005-01-18 10:38:15-07 $
-- /___/   /\    Date Created: Jan 5, 2005
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: anc_demux.vhd,rcs $
-- Revision 1.0  2005-01-18 10:38:15-07  jsnow
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
-- This module demultiplexes ANC packets from the video stream. Up to four
-- different types of packets may be searched for and demultiplexed. Packets
-- that have been demultiplexed may optionally be marked for deletion in the
-- video stream.
-- 
-- This module is a wrapper around the anc_demux module where all of the
-- demultiplexing functions are implemented. The anc_extract module also delays
-- the various video timing signals by three clock cycles to match the three 
-- clock cycle delay that anc_extract adds to the video path. This keeps the 
-- video and the video timing signals synchronized.
-- 
-- The module allows up to four different ANC packet types to be specified. 
-- There are four sets (a through d) of inputs signals used to find ANC packets 
-- to be demultiplexed. The signals in each set are described below:
-- 
-- did_[a:d]: These eight-bit values are compared against the DID words of all 
-- ANC packets to search for ANC packets to be demultiplexed. If the DID word 
-- indicates a Type 2 packet, the SDID word must also match. For Type 1 packets 
-- the SDID word is ignored.
-- 
-- sdid_[a:d]: These eight-bit values are compared against the SDID words of all
-- Type 2 ANC packets to search for ANC packets to be demultiplexed.
-- 
-- en_[a:d]: Each DID/SDID pair has an associated enable signal. If the enable
-- signal is low, the pair is not used when searching for ANC packets to
-- demultiplex.
-- 
-- del_pkt_[a:d]: Each DID/SDID pair has an associated del_pkt signal. If the
-- del_pkt signal is high, ANC packets matching the DID/SDID pair are marked for
-- deletion in the video stream after they have been demultiplexed.
-- 
-- The demultiplexed packet data comes out the data_out port along with a number
-- of decoded signals indicating what type of data is on the port.
-- 
-- data_out: This 10-bit port carries the demultiplexed ANC packet data. 
-- Actually, this port is quite simply the vid_in port delayed by three clock 
-- cycles.
-- 
-- data_out_valid: This signal indicates when a demultiplexed ANC packet is on
-- the data_out port. This signal becomes asserted when the DID word is on the
-- data_out port and stays asserted through the last word of the ANC packet (CS).
-- This signal is not asserted during the three-word ADF that precedes the ANC
-- packet.
-- 
-- match_code: This 2-bit output port indicates which of the DID/SDID input sets
-- matched the ANC packet that is being demultiplexed to the data_out port: "00"
-- for set A, "01" for set B, "10" for set C, and "11" for set D. This port
-- is valid when data_out_valid signal is asserted high.
-- 
-- did: This signal is asserted when the DID word of a demultiplexed packet is
-- on the data_out port.
-- 
-- sdid: This signal is asserted when the SDID word of a demultiplexed Type 2
-- packet is on the data_out port.
-- 
-- dbn: This signal is asserted when the DBN word of a demultiplexed Type 1
-- packet is on the data_out port.
-- 
-- dc: This signal is asserted when the DC word of a demultiplexed packet is on
-- the data_out port.
-- 
-- udw: This signal is asserted when any UDW word of a demultiplexed packet is
-- on the data_out port.
-- 
-- cs: This signal is asserted when the CS word of a demultiplexed packet is on
-- the data_out port.
-- 
-- The module also has a vid_out port. The data on this port is usually the same
-- as the vid_in port, but delayed by three clock cycles. However, ANC packets
-- can be marked for deletion by the module. Those that get marked for deletion
-- have their DID and CS value modified when they come out the vid_out port.
-- Packets that get marked for deletion by the module appear in their original
-- form on the data_out port.
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

use work.anc_edh_pkg.all;

entity anc_demux is
    port (
        -- inputs
        clk:            in    std_ulogic;   -- clock input
        ce:             in    std_ulogic;   -- clock enable
        rst:            in    std_ulogic;   -- async reset input
        vid_in:         in    video_type;   -- video data input
        en_a:           in    std_ulogic;   -- enable for DID A decoder
        did_a:          in    ubyte_type;   -- first DID code to match
        sdid_a:         in    ubyte_type;   -- first SDID code to match
        del_pkt_a:      in    std_ulogic;   -- packet will be deleted if asserted
        en_b:           in    std_ulogic;   -- enable for DID B decoder
        did_b:          in    ubyte_type;   -- second DID code to match
        sdid_b:         in    ubyte_type;   -- second SDID code to match
        del_pkt_b:      in    std_ulogic;   -- packet will be deleted if asserted
        en_c:           in    std_ulogic;   -- enable for DID C decoder
        did_c:          in    ubyte_type;   -- third DID code to match
        sdid_c:         in    ubyte_type;   -- third SDID code to match
        del_pkt_c:      in    std_ulogic;   -- packet will be deleted if asserted
        en_d:           in    std_ulogic;   -- enable for DID D decoder
        did_d:          in    ubyte_type;   -- fourth DID code to match
        sdid_d:         in    ubyte_type;   -- fourth SDID code to match
        del_pkt_d:      in    std_ulogic;   -- packet will be deleted if asserted
        in_trs:         in    std_ulogic;   -- asserted during the four words of all TRS symbols
        in_f:           in    std_ulogic;   -- field indicator
        in_v:           in    std_ulogic;   -- vertical blanking indicator
        in_h:           in    std_ulogic;   -- horizontal blanking indicator
        in_hcnt:        in    hpos_type;    -- horizontal position
        in_vcnt:        in    vpos_type;    -- vertical position
        in_sync_switch: in    std_ulogic;   -- synchronous switching line indicator
        in_eav_next:    in    std_ulogic;   -- next word begins an EAV
        in_sav_next:    in    std_ulogic;   -- next word begins an SAV
        in_xyz_word:    in    std_ulogic;   -- current word is a TRS XYZ word
        in_anc_next:    in    std_ulogic;   -- asserted when next word begins received ANC packet
        in_edh_next:    in    std_ulogic;   -- asserted when next word begins received EDH packet
        in_edh_loc:     in    std_ulogic;   -- asserted when next word is starting location where EDH
                                            -- packet should be located
        
        -- outputs
        data_out:       out   video_type;   -- output packet data
        data_out_valid: out   std_ulogic;   -- high  while all words of a matching packet are on data_out
        match_code:     out                 -- indicates DID/SDID combination matched the current packet
            std_ulogic_vector(1 downto 0);  
        did:            out   std_ulogic;   -- high when a DID word from a matching packet is on data_out
        dbn:            out   std_ulogic;   -- high when a DBN word from a matching packet is on data_out
        sdid:           out   std_ulogic;   -- high when an SDID word from a matching packet is on data_out
        dc:             out   std_ulogic;   -- high when a DC word from a matching packet is on data_out
        udw:            out   std_ulogic;   -- high when a UDW word from a matching packet is on data_out
        cs:             out   std_ulogic;   -- high when a CS word from a matching packet is on data_out
        vid_out:        out   video_type;   -- output video stream
        out_trs:        out   std_ulogic;   -- delayed version of in_trs
        out_f:          out   std_ulogic;   -- delayed version of in_f
        out_v:          out   std_ulogic;   -- delayed version of in_v
        out_h:          out   std_ulogic;   -- delayed version of in_h
        out_hcnt:       out   hpos_type;    -- delayed version of in_hcnt
        out_vcnt:       out   vpos_type;    -- delayed version of in_vcnt
        out_sync_switch:out   std_ulogic;   -- delayed version of in_sync_switch
        out_eav_next:   out   std_ulogic;   -- delayed version of in_eav_next
        out_sav_next:   out   std_ulogic;   -- delayed version of in_sav_next
        out_xyz_word:   out   std_ulogic;   -- delayed version of in_xyz_word
        out_anc_next:   out   std_ulogic;   -- delayed version of in_anc_next
        out_edh_next:   out   std_ulogic;   -- delayed version of in_edh_next
        out_edh_loc:    out   std_ulogic);  -- delayed version of in_edh_loc
end;

architecture synth of anc_demux is

subtype delay     is std_ulogic_vector(2 downto 0);

-------------------------------------------------------------------------------
-- Signal definitions
--
signal trs_dly :        delay;              -- delay registers for in_trs
signal f_dly :          delay;              -- delay registers for in_f 
signal v_dly :          delay;              -- delay registers for in_v
signal h_dly :          delay;              -- delay registers for in_h
signal hcnt_dly0 :      hpos_type;          -- delay register for in_hcnt
signal hcnt_dly1 :      hpos_type;          -- delay register for in_hcnt
signal hcnt_dly2 :      hpos_type;          -- delay register for in_hcnt
signal vcnt_dly0 :      vpos_type;          -- delay register for in_vcnt
signal vcnt_dly1 :      vpos_type;          -- delay register for in_vcnt
signal vcnt_dly2 :      vpos_type;          -- delay register for in_vcnt
signal sync_switch_dly: delay;              -- delay register for in_sync_switch
signal eav_next_dly :   delay;              -- delay registers for in_eav_next
signal sav_next_dly :   delay;              -- delay registers for in_sav_next
signal xyz_word_dly :   delay;              -- delay registers for in_xyz_word
signal anc_next_dly :   delay;              -- delay registers for in_anc_next
signal edh_next_dly :   delay;              -- delay registers for in_edh_next
signal edh_loc_dly :    delay;              -- delay registers for in_edh_loc

--------------------------------------------------------------------------------
-- Component definitions
--
component anc_extract
    port(
        clk:          in    std_ulogic;     -- clock input
        ce:           in    std_ulogic;     -- clock enable
        rst:          in    std_ulogic;     -- async reset
        anc_next:     in    std_ulogic;     -- ANC packet begins next word
        vid_in:       in    video_type;     -- input video stream
        en_a:         in    std_ulogic;     -- enable for DID/SDID pair A
        did_a:        in    ubyte_type;     -- DID A value
        sdid_a:       in    ubyte_type;     -- SDID A value
        del_pkt_a:    in    std_ulogic;     -- delete packet when asserted
        en_b:         in    std_ulogic;     -- enable for DID/SDID pair B
        did_b:        in    ubyte_type;     -- DID B value
        sdid_b:       in    ubyte_type;     -- SDID B value
        del_pkt_b:    in    std_ulogic;     -- delete packet when asserted
        en_c:         in    std_ulogic;     -- enable for DID/SDID pair C
        did_c:        in    ubyte_type;     -- DID C value
        sdid_c:       in    ubyte_type;     -- SDID C value
        del_pkt_c:    in    std_ulogic;     -- delete packet when asserted
        en_d:         in    std_ulogic;     -- enable for DID/SDID pair D
        did_d:        in    ubyte_type;     -- DID D value
        sdid_d:       in    ubyte_type;     -- SDID D value
        del_pkt_d:    in    std_ulogic;     -- delete packet when asserted
        data_out:     out   video_type;     -- video output port
        data_out_valid:
                      out   std_ulogic;     -- asserted when data_out contains ANC packet
        match_code:   out                   -- indicates which DID/SDID pair matched packet
            std_ulogic_vector(1 downto 0);
        did:          out   std_ulogic;     -- asserted when DID on data_out
        dbn:          out   std_ulogic;     -- asserted when DBN on data_out
        sdid:         out   std_ulogic;     -- asserted when SDID on data_out
        dc:           out   std_ulogic;     -- asserted when DC on data_out
        udw:          out   std_ulogic;     -- asserted when UDW on data_out
        cs:           out   std_ulogic;     -- asserted when CS on data_out
        vid_out:      out   video_type);    -- output video port
end component;

begin

    --
    -- anc_extract module
    --
    -- This module does all the work of demuxing ANC packets.
    --
    extract: anc_extract
        port map ( 
        clk             => clk,
        ce              => ce,
        rst             => rst,
        anc_next        => in_anc_next,
        vid_in          => vid_in,
        en_a            => en_a,
        did_a           => did_a,
        sdid_a          => sdid_a,
        del_pkt_a       => del_pkt_a,
        en_b            => en_b,
        did_b           => did_b,
        sdid_b          => sdid_b,
        del_pkt_b       => del_pkt_b,
        en_c            => en_c,
        did_c           => did_c,
        sdid_c          => sdid_c,
        del_pkt_c       => del_pkt_c,
        en_d            => en_d,
        did_d           => did_d,
        sdid_d          => sdid_d,
        del_pkt_d       => del_pkt_d,
        data_out        => data_out,
        data_out_valid  => data_out_valid,
        match_code      => match_code,
        did             => did,
        dbn             => dbn,
        sdid            => sdid,
        dc              => dc,
        udw             => udw,
        cs              => cs,
        vid_out         => vid_out);
        
    --
    -- Delay registers for video timing signals
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            trs_dly <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                trs_dly <= (trs_dly(1 downto 0) & in_trs);
            end if;
        end if;
    end process;

    out_trs <= trs_dly(2);

    process(clk, rst)
    begin
        if (rst = '1') then
            f_dly <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                f_dly <= (f_dly(1 downto 0) & in_f);
            end if;
        end if;
    end process;

    out_f <= f_dly(2);

    process(clk, rst)
    begin
        if (rst = '1') then
            v_dly <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                v_dly <= (v_dly(1 downto 0) & in_v);
            end if;
        end if;
    end process;

    out_v <= v_dly(2);

    process(clk, rst)
    begin
        if (rst = '1') then
            h_dly <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                h_dly <= (h_dly(1 downto 0) & in_h);
            end if;
        end if;
    end process;

    out_h <= h_dly(2);

    process(clk, rst)
    begin
        if (rst = '1') then
            hcnt_dly0 <= (others => '0');
            hcnt_dly1 <= (others => '0');
            hcnt_dly2 <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                hcnt_dly0 <= in_hcnt;
                hcnt_dly1 <= hcnt_dly0;
                hcnt_dly2 <= hcnt_dly1;
            end if;
        end if;
    end process;

    out_hcnt <= hcnt_dly2;

    process(clk, rst)
    begin
        if (rst = '1') then
            vcnt_dly0 <= (others => '0');
            vcnt_dly1 <= (others => '0');
            vcnt_dly2 <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                vcnt_dly0 <= in_vcnt;
                vcnt_dly1 <= vcnt_dly0;
                vcnt_dly2 <= vcnt_dly1;
            end if;
        end if;
    end process;

    out_vcnt <= vcnt_dly2;

    process(clk, rst)
    begin
        if (rst = '1') then
            sync_switch_dly <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                sync_switch_dly <= (sync_switch_dly(1 downto 0) & in_sync_switch);
            end if;
        end if;
    end process;

    out_sync_switch <= sync_switch_dly(2);

    process(clk, rst)
    begin
        if (rst = '1') then
            eav_next_dly <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                eav_next_dly <= (eav_next_dly(1 downto 0) & in_eav_next);
            end if;
        end if;
    end process;

    out_eav_next <= eav_next_dly(2);

    process(clk, rst)
    begin
        if (rst = '1') then
            sav_next_dly <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                sav_next_dly <= (sav_next_dly(1 downto 0) & in_sav_next);
            end if;
        end if;
    end process;

    out_sav_next <= sav_next_dly(2);

    process(clk, rst)
    begin
        if (rst = '1') then
            xyz_word_dly <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                xyz_word_dly <= (xyz_word_dly(1 downto 0) & in_xyz_word);
            end if;
        end if;
    end process;

    out_xyz_word <= xyz_word_dly(2);

    process(clk, rst)
    begin
        if (rst = '1') then
            anc_next_dly <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                anc_next_dly <= (anc_next_dly(1 downto 0) & in_anc_next);
            end if;
        end if;
    end process;

    out_anc_next <= anc_next_dly(2);

    process(clk, rst)
    begin
        if (rst = '1') then
            edh_next_dly <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                edh_next_dly <= (edh_next_dly(1 downto 0) & in_edh_next);
            end if;
        end if;
    end process;

    out_edh_next <= edh_next_dly(2);

    process(clk, rst)
    begin
        if (rst = '1') then
            edh_loc_dly <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                edh_loc_dly <= (edh_loc_dly(1 downto 0) & in_edh_loc);
            end if;
        end if;
    end process;

    out_edh_loc <= edh_loc_dly(2);

end synth;