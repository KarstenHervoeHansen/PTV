-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: test_anc_demux.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2005-01-18 10:44:01-07 $
-- /___/   /\    Date Created: Jan 5, 2005
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: test_anc_demux.vhd,rcs $
-- Revision 1.0  2005-01-18 10:44:01-07  jsnow
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
-- This testbench tests the ANC packet demuxing capabilities of the
-- anc_edh_processor module.
-- 
-- The testbench first waits for the video decoder module to lock to the video.
-- Then, it inserts ANC packets into the horizontal blanking interval on five
-- consecutive lines. On the first line, a Type 1 packet is inserted which 
-- should match the DID code on the did_a input. On the remaining lines, Type 2 
-- packets are inserted that should match the DID/SDID pairs on the a through d 
-- inputs sequentially (line 2 matches the "a" pair, line 3 matches the "b" 
-- pair, etc.)
-- 
-- On the last line, the one that matches the "d" input pair, the del_pkt input
-- is asserted to cause the packet to be marked for deletion in the video 
-- stream.
-- 

library ieee;
use std.textio.all;
use ieee.std_logic_textio.all;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;
use work.anc_edh_pkg.all;

entity test_anc_demux is
    constant MAX_MEM : integer := 900900; -- Size of memory needed to hold 1 frame NTSC
    type mem_type is array (MAX_MEM - 1 downto 0) of video_type;    
end;

architecture sim of test_anc_demux is

shared variable memory :    mem_type;       -- Memory array holding test video

--------------------------------------------------------------------------------
-- Constant definitions
--  
constant FIRST_EDH          : integer := 13977;     -- Position in memory of start of 1st EDH packet
constant SECOND_EDH         : integer := 465285;    -- Position in memory of start of 2nd EDH packet

-- These constants specify the positions of some words in the ANC packet.
constant DID_WORD           : integer := 3;
constant DBN_WORD           : integer := 4;
constant DC_WORD            : integer := 5;

-- These constants define the values of certain words in inserted ANC packets.
constant ADF0               : video_type := "0000000000";
constant ADF1               : video_type := "1111111111";
constant INSERT_DID_TYPE1   :ubyte_type := ubyte_type(TO_UNSIGNED(16#c5#, ubyte_type'length));
constant INSERT_DID_TYPE2   :ubyte_type := ubyte_type(TO_UNSIGNED(16#7f#, ubyte_type'length));
constant INSERT_DBN         : ubyte_type := ubyte_type(TO_UNSIGNED(16#00#, ubyte_type'length));
constant INSERT_SDID1       : ubyte_type := ubyte_type(TO_UNSIGNED(16#10#, ubyte_type'length));
constant INSERT_SDID2       : ubyte_type := ubyte_type(TO_UNSIGNED(16#20#, ubyte_type'length));
constant INSERT_SDID3       : ubyte_type := ubyte_type(TO_UNSIGNED(16#30#, ubyte_type'length));
constant INSERT_SDID4       : ubyte_type := ubyte_type(TO_UNSIGNED(16#40#, ubyte_type'length));
constant INSERT_DC          : ubyte_type := ubyte_type(TO_UNSIGNED(16#08#, ubyte_type'length));

constant START_LINE         : vpos_type := "0100101100";    -- 300
constant START_LINE_PLUS_1  : vpos_type := "0100101101";    -- 301
constant START_LINE_PLUS_2  : vpos_type := "0100101110";    -- 302
constant START_LINE_PLUS_3  : vpos_type := "0100101111";    -- 303
constant START_LINE_PLUS_4  : vpos_type := "0100110000";    -- 304
constant FIRST_FREE         : hpos_type := hpos_type(TO_UNSIGNED(1477,hpos_type'length));

--
-- This constant specifies the number of cycles of clock latency through the
-- anc_edh_processor, including video decoder latency.
--
constant LATENCY        : integer := 10;

--------------------------------------------------------------------------------
-- Signal definitions
--

--
-- These signals connect to the modules in the test bench
--
signal clk :            std_ulogic := '0';  -- clock signal
signal enclk :          std_ulogic := '1';  -- clock enable
signal rst :            std_ulogic := '1';  -- reset signal
signal d:               video_type;         -- output of memory array
signal vid_in :         video_type;         -- video input to module
signal std_locked :     std_ulogic;         -- video standard dectector locked
signal std :            vidstd_type;        -- video standard code
signal trs :            std_ulogic;         -- video decoder TRS
signal vid_out :        video_type;         -- video output of edh_processor
signal f :              std_ulogic;         -- field indicator
signal v :              std_ulogic;         -- vertical blanking indicator
signal h :              std_ulogic;         -- horizontal blanking indicator
signal hcnt :           hpos_type;          -- horizontal counter
signal vcnt :           vpos_type;          -- vertical counter
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
signal did_a :          ubyte_type;         -- "A" set DID match input
signal sdid_a :         ubyte_type;         -- "A" set SDID match input
signal en_a :           std_ulogic;         -- "A" set SDID match input
signal del_pkt_a :      std_ulogic;         -- "A" set delete packet input
signal did_b :          ubyte_type;         -- "B" set DID match input
signal sdid_b :         ubyte_type;         -- "B" set SDID match input
signal en_b :           std_ulogic;         -- "B" set SDID match input
signal del_pkt_b :      std_ulogic;         -- "B" set delete packet input
signal did_c :          ubyte_type;         -- "C" set DID match input
signal sdid_c :         ubyte_type;         -- "C" set SDID match input
signal en_c :           std_ulogic;         -- "C" set SDID match input
signal del_pkt_c :      std_ulogic;         -- "C" set delete packet input
signal did_d :          ubyte_type;         -- "D" set DID match input
signal sdid_d :         ubyte_type;         -- "D" set SDID match input
signal en_d :           std_ulogic;         -- "D" set SDID match input
signal del_pkt_d :      std_ulogic;         -- "D" set delete packet input
signal anc_out :        video_type;         -- demuxed ANC packet data output
signal anc_out_valid :  std_ulogic;         -- ANC packet data out is valid
signal anc_match:                           -- indicates which DID/SDID set matches packet
        std_ulogic_vector(1 downto 0);
signal did :            std_ulogic;         -- DID word on anc_out
signal dbn :            std_ulogic;         -- DBN word on anc_out
signal sdid :           std_ulogic;         -- SDID word on anc_out
signal dc :             std_ulogic;         -- DC word on anc_out
signal udw :            std_ulogic;         -- UDW word on anc_out
signal cs :             std_ulogic;         -- CS word on anc_out
signal packet_flags :   edh_pktflg_type;    -- vector containing the packet error flags listed below

--
-- These signals control the testbench
--
type dly_video_type     is array (LATENCY-1 downto 0) of video_type;

signal i:               integer;            -- index into memory array
signal frames:          integer;            -- counts number of frames during test
signal error_found:     boolean := false;   -- asserted when a simulation error is detected
signal expected_vid:    video_type;         -- expected video value
signal match_code :                         -- used to generated expected data                  
        std_ulogic_vector(1 downto 0);
signal type_code :      integer;            -- packet type code
signal expected_anc :   video_type;         -- expected anc data out
signal expected_match :                     -- expected DID/SDID match set code
        std_ulogic_vector(1 downto 0);
signal expected_valid : std_ulogic;         -- expected anc_out_valid value
signal expected_did :   std_ulogic;         -- expected DID signal
signal expected_dbn :   std_ulogic;         -- expected DBN signal
signal expected_sdid :  std_ulogic;         -- expected SDID signal
signal expected_dc :    std_ulogic;         -- expected DC signal
signal expected_udw :   std_ulogic;         -- expected UDW signal
signal expected_cs :    std_ulogic;         -- expected CS signal
signal dly_vid :        dly_video_type;     -- delays vid_in for output comparison
signal replace :        std_ulogic := '0';  -- replaced memory output with ANC packet data when asserted
signal ins_start :                          -- insert starting position for ANC packet
        hpos_type := (others => '0');
signal ins_did :                            -- inserted DID value
        ubyte_type := (others => '0');
signal ins_sdid :                           -- inserted SDID/DBN value
        ubyte_type := (others => '0');
signal ins_dc :                             -- inserted DC value
        ubyte_type := (others => '0');
signal checksum :                           -- inserted checksum value
        cksum_type := (others => '0');

signal VCC :            std_ulogic  := '1';
signal GND :            std_ulogic  := '0';
signal GND8 :           udwadr_type := "00000000";
signal GND10 :          video_type  := "0000000000";

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

--
-- pad2_9 function
--
-- This function takes an 8-bit value, computes a parity bit, then creates a
-- 9-bit word with the parity bit in bit 8, followed by the original 8-bit 
-- value.
--          
function pad2_9(d: ubyte_type) return cksum_type is
    variable parity : std_ulogic;
begin
    parity := d(7) xor d(6) xor d(5) xor d(4) xor d(3) xor d(2) xor d(1) xor d(0);
    return(parity & d); 
end pad2_9;

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
        hanc_pkt        => GND,
        vanc_pkt        => GND,
        pkt_rdy_in      => GND,
        calc_udw_parity => GND,
        anc_in          => GND10,
        ld_did          => GND,
        ld_dbn          => GND,
        ld_dc           => GND,
        ld_udw          => GND,
        udw_wr_adr      => GND8,
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
        anc_out         => anc_out,
        anc_out_valid   => anc_out_valid,
        anc_match       => anc_match,
        did             => did,
        dbn             => dbn,
        sdid            => sdid,
        dc              => dc,
        udw             => udw,
        cs              => cs,
        pkt_in_empty    => open);

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
    -- memory array and assign that value to d.
    --
    process(i)
    begin
        if (i < 0 or i >= MAX_MEM or frames > 7) then
            d <= (others => '0');
        else
            d <= memory(i);
        end if;
    end process;

    --
    -- This code initializes the DID/SDID input pairs to the processor.
    --
    process
    begin
        en_a <= '1';
        did_a <= INSERT_DID_TYPE1;
        sdid_a <= INSERT_SDID1;
        del_pkt_a <= '0';
        en_b <= '1';
        did_b <= INSERT_DID_TYPE2;
        sdid_b <= INSERT_SDID2;
        del_pkt_b <= '0';
        en_c <= '1';
        did_c <= INSERT_DID_TYPE2;
        sdid_c <= INSERT_SDID3;
        del_pkt_c <= '0';
        en_d <= '1';
        did_d <= INSERT_DID_TYPE2;
        sdid_d <= INSERT_SDID4;
        del_pkt_d <= '1';

        wait until (locked'event and locked = '1');
        wait until (vcnt = START_LINE + 1);
        wait until (clk'event and clk = '0');
        did_a <= INSERT_DID_TYPE2;
        wait;
    end process;


    --
    -- This code generates various values for the inserted ANC packet.
    --
    process(vcnt)
    begin
        case vcnt is
            when START_LINE => 
                replace <= locked;
                ins_start <= FIRST_FREE;
                ins_did <= INSERT_DID_TYPE1;
                ins_sdid <= ubyte_type(TO_UNSIGNED(16#40#, ubyte_type'length));
                ins_dc <= INSERT_DC;

            when START_LINE_PLUS_1 =>
                replace <= locked;
                ins_start <= FIRST_FREE;
                ins_did <= INSERT_DID_TYPE2;
                ins_sdid <= INSERT_SDID1;
                ins_dc <= INSERT_DC;
       
            when START_LINE_PLUS_2 =>
                replace <= locked;
                ins_start <= FIRST_FREE;
                ins_did <= INSERT_DID_TYPE2;
                ins_sdid <= INSERT_SDID2;
                ins_dc <= INSERT_DC;
               
            when START_LINE_PLUS_3 =>
                replace <= locked;
                ins_start <= FIRST_FREE;
                ins_did <= INSERT_DID_TYPE2;
                ins_sdid <= INSERT_SDID3;
                ins_dc <= INSERT_DC;
                
            when START_LINE_PLUS_4 =>
                replace <= locked;
                ins_start <= FIRST_FREE;
                ins_did <= INSERT_DID_TYPE2;
                ins_sdid <= INSERT_SDID4;
                ins_dc <= INSERT_DC;
                
            when others =>
                replace <= '0';
                ins_start <= (others => '0');
                ins_did <= (others => '0');
                ins_sdid <= (others => '0');
                ins_dc <= (others => '0');
        end case;   
    end process;

    --
    -- This code replaces the data from the memory with inserted ANC packet data.
    --
    process(clk)
        variable temp_hpos :  std_logic_vector(11 downto 0);
    begin
        if (clk'event and clk = '0') then
            vid_in <= d;

            if (replace = '1') then
                if (hcnt = ins_start) then
                    vid_in <= ADF0;
                elsif (hcnt = ins_start + 1 or hcnt = ins_start + 2) then
                    vid_in <= ADF1;
                elsif (hcnt = ins_start + DID_WORD) then
                    vid_in <= pad2_10(ins_did);
                    checksum <= pad2_9(ins_did);
                elsif (hcnt = ins_start + DBN_WORD) then
                    vid_in <= pad2_10(ins_sdid);
                    checksum <= cksum_type(std_logic_vector(checksum) + std_logic_vector(pad2_9(ins_sdid)));
                elsif (hcnt = ins_start + DC_WORD) then
                    vid_in <= pad2_10(ins_dc);
                    checksum <= cksum_type(std_logic_vector(checksum) + std_logic_vector(pad2_9(ins_dc)));
                elsif (hcnt > ins_start + DC_WORD and 
                       hcnt <= ins_start + DC_WORD + std_logic_vector(ins_dc)) then
                    temp_hpos := std_logic_vector(hcnt - (ins_start + DC_WORD));
                    vid_in <= video_type(temp_hpos(9 downto 0) or "1000000000");
                    checksum <= cksum_type(std_logic_vector(checksum) + temp_hpos(8 downto 0));
                elsif (hcnt = ins_start + DC_WORD + std_logic_vector(ins_dc) + 1) then
                    vid_in <= (not checksum(8) & checksum);
                else
                    vid_in <= d;
                end if;
            end if;
        end if;
    end process;


    --
    -- This statement stops the simulation when the vcnt value reaches the end of
    -- the simulation.
    --
    process
        variable l : line;
        variable success_string :       string (1 to 36) 
                                        := "Test completed with no errors found.";
        variable error_string :         string (1 to 27)
                                        := "Test completed with errors.";
    begin
        wait until (locked'event and locked = '1');
        wait until (vcnt = START_LINE + 5);

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


    -- Generate the expected data
    process(vcnt)
    begin
        if (vcnt = START_LINE) then
            match_code <= "00";
            type_code <= 1;

        elsif (vcnt = START_LINE_PLUS_1) then
            match_code <= "00";
            type_code <= 2;

        elsif (vcnt = START_LINE_PLUS_2) then
            match_code <= "01";
            type_code <= 2;

        elsif (vcnt = START_LINE_PLUS_3) then
            match_code <= "10";
            type_code <= 2;

        elsif (vcnt = START_LINE_PLUS_4) then
            match_code <= "11";
            type_code <= 2;

        else
            match_code <= "00";
            type_code <= 0;
        end if;
    end process;

    process(clk)
    begin
        if (clk'event and clk = '0') then
            
            expected_vid <= dly_vid(LATENCY-1);
            expected_anc <= dly_vid(LATENCY-2);
                
            expected_did <= '0';
            expected_dbn <= '0';
            expected_sdid <= '0';
            expected_dc <= '0';
            expected_udw <= '0';
            expected_cs <= '0';
            expected_valid <= '0';
            expected_match <= "00";
                
            if (locked = '1') then
                if (vcnt >= START_LINE and vcnt < START_LINE + 5) then
                    if (hcnt = FIRST_FREE + DID_WORD + LATENCY - 1) then
                        expected_match <= match_code;
                        expected_did <= '1';
                        expected_valid <= '1';
                    end if;
                
                    if (hcnt = FIRST_FREE + DBN_WORD + LATENCY - 1) then
                        expected_match <= match_code;
                    
                        if (type_code = 1) then
                            expected_dbn <= '1';
                        else
                            expected_sdid <= '1';
                        end if;

                        expected_valid <= '1';
                    
                        if (vcnt = START_LINE_PLUS_4) then
                            expected_vid <= video_type(TO_UNSIGNED(16#180#, video_type'length));
                        end if;
                    end if;
                
                    if (hcnt = FIRST_FREE + DC_WORD + LATENCY - 1) then
                        expected_match <= match_code;
                        expected_dc <= '1';
                        expected_valid <= '1';
                    end if;
                
                    if (hcnt >= FIRST_FREE + DC_WORD + LATENCY and
                        hcnt <= FIRST_FREE + DC_WORD + std_logic_vector(INSERT_DC) + LATENCY - 1) then
                        expected_match <= match_code;
                        expected_udw <= '1';
                        expected_valid <= '1';
                    end if;
                
                    if (hcnt = FIRST_FREE + DC_WORD + std_logic_vector(INSERT_DC) + LATENCY) then 
                        expected_match <= match_code;
                        expected_cs <= '1';
                        expected_valid <= '1';
                    end if;
                
                    if (hcnt = FIRST_FREE + DC_WORD + std_logic_vector(INSERT_DC) + LATENCY + 1) then 
                        if (vcnt = START_LINE_PLUS_4) then
                            expected_vid <= video_type(TO_UNSIGNED(16#1ec#, video_type'length));
                        end if;
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
        variable l : line;
        variable exp_str        : string (1 to 12) := " expected = ";
        variable vid_out_str    : string (1 to 17) := "Error: vid_out = ";
        variable anc_out_str    : string (1 to 17) := "Error: anc_out = ";
        variable match_str      : string (1 to 19) := "Error: anc_match = ";
        variable did_str        : string (1 to 13) := "Error: did = ";
        variable dbn_str        : string (1 to 13) := "Error: dbn = ";
        variable sdid_str       : string (1 to 14) := "Error: sdid = ";
        variable dc_str         : string (1 to 12) := "Error: dc = ";
        variable udw_str        : string (1 to 13) := "Error: udw = ";
        variable cs_str         : string (1 to 12) := "Error: cs = ";
        variable valid_str      : string (1 to 52) := "Error: anc_out_valid was expected to be 1, but was 0";
        variable temp_byte      : std_logic_vector(3 downto 0);
    begin
        if (clk'event and clk = '1') then
            if (locked = '1' and edh_packet = '0') then
                if (expected_vid /= vid_out) then
                    write(l, vid_out_str);
                    hwrite(l, ("00" & vid_out), left, vid_out'length + 2);
                    write(l, exp_str);
                    hwrite(l, ("00" & expected_vid), left, expected_vid'left + 2);
                    writeline(output, l);
                    error_found <= true;
                end if;

                if (expected_anc /= anc_out) then
                    write(l, anc_out_str);
                    hwrite(l, ("00" & anc_out), left, anc_out'length + 2);
                    write(l, exp_str);
                    hwrite(l, ("00" & expected_anc), left, expected_anc'left + 2);
                    writeline(output, l);
                    error_found <= true;
                end if;

                if (expected_valid = '1') then

                    if (anc_out_valid = '0') then
                        write(l, valid_str);
                        writeline(output, l);
                        error_found <= true;    
                    end if;

                    if (anc_match /= expected_match) then
                        write(l, match_str);
                        hwrite(l, ("00" & anc_match), left, 4);
                        write(l, exp_str);
                        hwrite(l, ("00" & expected_match), left, 4);
                        writeline(output, l);
                        error_found <= true;
                    end if;

                    if (did /= expected_did) then
                        write(l, did_str);
                        temp_byte := "000" & did;
                        hwrite(l, temp_byte, left, 4);
                        write(l, exp_str);
                        temp_byte := "000" & expected_did;
                        hwrite(l, temp_byte, left, 4);
                        writeline(output, l);
                        error_found <= true;
                    end if;
                    
                    if (dbn /= expected_dbn) then
                        write(l, dbn_str);
                        temp_byte := "000" & dbn;
                        hwrite(l, temp_byte, left, 4);
                        write(l, exp_str);
                        temp_byte := "000" & expected_dbn;
                        hwrite(l, temp_byte, left, 4);
                        writeline(output, l);
                        error_found <= true;
                    end if;

                    if (sdid /= expected_sdid) then
                        write(l, sdid_str);
                        temp_byte := "000" & sdid;
                        hwrite(l, temp_byte, left, 4);
                        write(l, exp_str);
                        temp_byte := "000" & expected_sdid;
                        hwrite(l, temp_byte, left, 4);
                        writeline(output, l);
                        error_found <= true;
                    end if;

                    if (dc /= expected_dc) then
                        write(l, dc_str);
                        temp_byte := "000" & dc;
                        hwrite(l, temp_byte, left, 4);
                        write(l, exp_str);
                        temp_byte := "000" & expected_dc;
                        hwrite(l, temp_byte, left, 4);
                        writeline(output, l);
                        error_found <= true;
                    end if;

                    if (udw /= expected_udw) then
                        write(l, udw_str);
                        temp_byte := "000" & udw;
                        hwrite(l, temp_byte, left, 4);
                        write(l, exp_str);
                        temp_byte := "000" & expected_udw;
                        hwrite(l, temp_byte, left, 4);
                        writeline(output, l);
                        error_found <= true;
                    end if;

                    if (cs /= expected_cs) then
                        write(l, cs_str);
                        temp_byte := "000" & cs;
                        hwrite(l, temp_byte, left, 4);
                        write(l, exp_str);
                        temp_byte := "000" & expected_cs;
                        hwrite(l, temp_byte, left, 4);
                        writeline(output, l);
                        error_found <= true;
                    end if;
                end if;
            end if;
        end if; 
    end process;

end sim;