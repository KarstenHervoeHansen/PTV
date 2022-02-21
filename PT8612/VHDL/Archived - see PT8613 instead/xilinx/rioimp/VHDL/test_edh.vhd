-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: test_edh.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2005-01-18 10:44:53-07 $
-- /___/   /\    Date Created: Jan 5, 2005
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: test_edh.vhd,rcs $
-- Revision 1.0  2005-01-18 10:44:53-07  jsnow
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
-- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
-- IMPORTANT NOTE:
--
-- This testbench uses shared variables and VHDL '93 file I/O syntax. It must
-- be compiled by a VHDL '93 compliant compiler. In ModelSim, the default is
-- to generated errors on VHDL '93 specific syntax and the -93 flag must be
-- used.
--
-- This testbench also uses the ModelSim function init_signal_spy to monitor
-- some signals down in the hierarchy.
-- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

-- This testbench tests the edh_processor module. The testbench runs for eight
-- frames. A data file called one_frame.txt is loaded into a memory array and 
-- used as the source data into the edh_processor every frame. This data file 
-- contains a video test pattern, along with embedded EDH and ANC packets. The 
-- testbench sometimes substitutes values in place of the values from the data 
-- file in order to force certain error conditions to determine if the 
-- edh_processor detects them.
-- 
-- Field 0: The video decoder is synchronizing during this field. All errors are
-- ignored.
-- 
-- Field 1; No errors are expected during this field. The data from data file is
-- not modified. Any errors occurring in this field would indicate an error with
-- the design.
-- 
-- Field 2: The test bench forces an ANC IDH condition in this field to make 
-- sure it gets encoded into the outgoing EDH packet.
-- 
-- Field 3: The test bench forces an ANC UES condition in this field to make 
-- sure it gets encoded into the outgoing EDH packet.
-- 
-- Field 4: The test bench modifies the input EDH packet in two ways. It changes
-- the active-picture CRC value (the edh_processor should asserted the AP EDH
-- flag as a result). It also asserts the full-field EDH flag in the packet (the
-- edh_processor should clear the FF EDH bit and assert the FF EDA bit in the
-- outgoing EDH packet).
-- 
-- Field 5: The test bench forces a AP IDH error condition during this field. 
-- The AP IDH flag should be set in the outgoing packet.
-- 
-- Field 6: The test bench forces a FF IDH error condition during this field. 
-- The FF IDH flag should be set in the outgoing packet.
-- 
-- Field 7: The testbench forces the checksum of the EDH packet to an incorrect
-- value to make sure the edh_processor detects the error.
-- 
-- Field 8: The testbench removes the EDH packet from the input video stream and
-- checks that the edh_processor reports a missing EDH packet error. All the UES
-- flags in the outgoing packet will be asserted.
-- 
-- Field 9: The testbench forces the ANC IDH and UES error conditions to verify
-- that these flags get set in the outgoing EDH packet.
-- 
-- Field 10: The testbench forces the AP IDH and FF IDH error conditions to 
-- verify that these flags get set in the outgoing EDH packet.
-- 
-- Field 11: The value of the DBN word in the EDH packet is modified by the 
-- testbench. The edh_processor should detected a edh packet format error.  The 
-- UES flags of all three flag sets will be set in the outgoing EDH packet as a 
-- result.
-- 
-- Field 12: The testbench changes the value of one word in the inactive video.
-- This should cause the edh_processor to detect a FF CRC error. The corrupted 
-- word occurs in an ANC packet, so a checksum error for the ANC packet should 
-- also be detected.
-- 
-- Field 13: The testbench modifies the incoming EDH packet so that the AP EDH
-- flag is set. The outgoing EDH packet should have the AP EDH flag clear and 
-- the AP EDA flag set.
-- 
-- Field 14: No errors should be detected in this field.
-- 
-- Field 15: The testbench forces a parity error in the EDH packet. The 
-- edh_processor should detect and report this error.
-- 

library ieee;
use std.textio.all;
use ieee.std_logic_textio.all;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

library modelsim_lib;
use modelsim_lib.util.all;

use work.anc_edh_pkg.all;

entity test_edh is
    constant MAX_MEM : integer := 900900; -- Size of memory needed to hold 1 frame NTSC
    type mem_type is 
        array (MAX_MEM - 1 downto 0) of video_type; 
end;

architecture sim of test_edh is

shared variable memory :    mem_type;       -- Memory array holding test video

 
constant FIRST_EDH :        integer := 13977;   -- Position in memory of start of 1st EDH packet
constant SECOND_EDH :       integer := 465285;  -- Position in memory of start of 2nd EDH packet

-- These constants identify the various words in the EDH packet.
constant DID_WORD      :    integer := 3;
constant DBN_WORD      :    integer := 4;
constant DC_WORD       :    integer := 5;
constant AP_CRC_WORD0  :    integer := 6;
constant AP_CRC_WORD1  :    integer := 7;
constant AP_CRC_WORD2  :    integer := 8;
constant FF_CRC_WORD0  :    integer := 9;
constant FF_CRC_WORD1  :    integer := 10;
constant FF_CRC_WORD2  :    integer := 11;
constant ANC_FLAG_WORD :    integer := 12;
constant AP_FLAG_WORD  :    integer := 13;
constant FF_FLAG_WORD  :    integer := 14;
constant RSVD0         :    integer := 15;
constant RSVD1         :    integer := 16;
constant RSVD2         :    integer := 17;
constant RSVD3         :    integer := 18;
constant RSVD4         :    integer := 19;
constant RSVD5         :    integer := 20;
constant RSVD6         :    integer := 21;
constant CS_WORD       :    integer := 22;

--
-- These signals connect to the modules in the test bench
--
signal clk:                 std_ulogic := '0';  -- clock signal
signal enclk :              std_ulogic := '1';  -- clock enable
signal rst:                 std_ulogic := '1';  -- reset signal
signal d:                   video_type;         -- output of memory array
signal vid_in:              video_type;         -- modified output of memory array
signal std_locked:          std_ulogic;         -- video standard dectector locked
signal std:                 vidstd_type;        -- video standard code
signal trs:                 std_ulogic;         -- video decoder TRS
signal vid_out:             video_type;         -- video output of edh_processor
signal f:                   std_ulogic;         -- field indicator
signal v:                   std_ulogic;         -- vertical blanking indicator
signal h:                   std_ulogic;         -- horizontal blanking indicator
signal hcnt:                hpos_type;          -- horizontal counter
signal vcnt:                vpos_type;          -- vertical counter
signal sync_switch:         std_ulogic;         -- sync switching line
signal locked:              std_ulogic;         -- flywheel locked
signal eav_next:            std_ulogic;         -- EAV is next
signal sav_next:            std_ulogic;         -- SAV is next
signal xyz_word:            std_ulogic;         -- current word is XYZ word
signal anc_next:            std_ulogic;         -- ANC is next
signal edh_next:            std_ulogic;         -- EDH is next
signal rx_ce:               std_ulogic;         -- clock enable
signal edh_packet:          std_ulogic;         -- edh processor EDH packet indicator
signal packet_flags :       edh_pktflg_type;    -- vector containing the packet error flags listed below
signal edh_missing:         std_ulogic;         -- edh packet was missing
signal edh_parity_err:      std_ulogic;         -- edh packet parity error
signal edh_chksum_err:      std_ulogic;         -- edh packet checksum error
signal edh_format_err:      std_ulogic;         -- edh packet format error
signal rx_ap_flags:         edh_flgset_type;    -- received edh packet AP flags
signal rx_ff_flags:         edh_flgset_type;    -- received edh packet FF flags
signal rx_anc_flags:        edh_flgset_type;    -- recevied edh packet ANC flags
signal ap_flags:            edh_flgset_type;    -- transmitted edh packet AP flags
signal ff_flags:            edh_flgset_type;    -- transmitted edh packet FF flags
signal anc_flags:           edh_flgset_type;    -- transmitted edh packet ANC flags
signal errcnt:              edh_errcnt_type;    -- count of fields containing EDH errors
signal anc_idh_local:       std_ulogic := '0';  -- controls the ANC IDH local input to EDH proc
signal anc_ues_local:       std_ulogic := '0';  -- controls the ANC UES local input to EDH proc
signal ap_idh_local:        std_ulogic := '0';  -- controls the AP IDH local input to EDH proc
signal ff_idh_local:        std_ulogic := '0';  -- controls the FF IDH local input to EDH proc
signal errcnt_flg_en:       edh_allflg_type     -- controls the error count flag enables
        := "1111111111111111";  

--
-- These registers delay the output of the decoder by one clock cycle for
-- comparison with the output of the edh_processor module.
--
signal dly_std:             vidstd_type;
signal dly_std_locked:      std_ulogic;
signal dly_vid:             video_type;
signal dly_f:               std_ulogic;
signal dly_v:               std_ulogic;
signal dly_h:               std_ulogic;
signal dly_hcnt:            hpos_type;
signal dly_vcnt:            vpos_type;
signal dly_sync_switch:     std_ulogic;
signal dly_locked:          std_ulogic;
signal dly_eav_next:        std_ulogic;
signal dly_sav_next:        std_ulogic;
signal dly_xyz_word:        std_ulogic;
signal dly_anc_next:        std_ulogic;
signal dly_edh_next:        std_ulogic;

--
-- These signals control the testbench
--
signal i:                    integer;                       -- index into memory array
signal packet:               integer;                       -- current received EDH packet number
signal frames:               integer;                       -- counts number of frames during test
signal check_flags:          boolean := false;              -- asserted when error flags should be checked
signal expected_rx_ap_flags: edh_flgset_type := "00000";    -- holds the expected received AP flags value
signal expected_rx_ff_flags: edh_flgset_type := "00000";    -- holds the expected received FF flags value
signal expected_rx_anc_flags:edh_flgset_type := "00000";    -- holds the expected receivedANC flags value
signal expected_ap_flags:    edh_flgset_type := "00000";    -- holds the expected AP flags value
signal expected_ff_flags:    edh_flgset_type := "00000";    -- holds the expected FF flags value
signal expected_anc_flags:   edh_flgset_type := "00000";    -- holds the expected ANC flags value
signal expected_chksum_err:  std_ulogic := '0';             -- holds the expected checksum error flag value
signal expected_edh_missing: std_ulogic := '0';             -- holds the expected EDH packet missing flag value
signal expected_parity_err:  std_ulogic := '0';             -- holds the expected parity error flag value
signal expected_format_err:  std_ulogic := '0';             -- holds the expected format error flag value
signal error_found:          boolean := false;              -- asserted when a simulation error is detected


--
-- These signals are targets for init_signal_spy functions that get mapped to
-- signals in the edh_processor module. This is required because you cannot
-- directly reference signals inside a VHDL module from elsewhere in the
-- hierarchy.
--
signal ep_dec_std:          vidstd_type;
signal ep_dec_std_locked:   std_ulogic;
signal ep_dec_vid:          video_type;
signal ep_dec_f:            std_ulogic;
signal ep_dec_v:            std_ulogic;
signal ep_dec_h:            std_ulogic;
signal ep_dec_hcnt:         hpos_type;
signal ep_dec_vcnt:         vpos_type;
signal ep_dec_sync_switch:  std_ulogic;
signal ep_dec_locked:       std_ulogic;
signal ep_dec_eav_next:     std_ulogic;
signal ep_dec_sav_next:     std_ulogic;
signal ep_dec_xyz_word:     std_ulogic;
signal ep_dec_anc_next:     std_ulogic;
signal ep_dec_edh_next:     std_ulogic;
signal ep_ap_crc:           crc16_type;
signal ep_ap_crc_valid:     std_ulogic;
signal ep_ff_crc:           crc16_type;
signal ep_ff_crc_valid:     std_ulogic;
signal ep_anc_flags:        edh_flgset_type;
signal ep_ap_flags:         edh_flgset_type;
signal ep_ff_flags:         edh_flgset_type;

signal VCC :                std_ulogic := '1';
signal GND :                std_ulogic := '0';

component edh_processor
    port (
        clk:            in  std_ulogic;     
        ce:             in  std_ulogic;     
        rst:            in  std_ulogic;     

        -- video decoder inputs
        vid_in:         in  video_type;     
        reacquire:      in  std_ulogic;     
        en_sync_switch: in  std_ulogic;     
        en_trs_blank:   in  std_ulogic;     

        -- EDH flag inputs
        anc_idh_local:  in  std_ulogic;     
        anc_ues_local:  in  std_ulogic;     
        ap_idh_local:   in  std_ulogic;     
        ff_idh_local:   in  std_ulogic;     
        errcnt_flg_en:  in  edh_allflg_type;
        clr_errcnt:     in  std_ulogic;     
        receive_mode:   in  std_ulogic;     

        -- video and decoded video timing outputs
        vid_out:        out video_type;     
        std:            out vidstd_type;    
        std_locked:     out std_ulogic;     
        trs:            out std_ulogic;     
        field:          out std_ulogic;     
        v_blank:        out std_ulogic;     
        h_blank:        out std_ulogic;     
        horz_count:     out hpos_type;      
        vert_count:     out vpos_type;      
        sync_switch:    out std_ulogic;     
        locked:         out std_ulogic;     
        eav_next:       out std_ulogic;     
        sav_next:       out std_ulogic;     
        xyz_word:       out std_ulogic;     
        anc_next:       out std_ulogic;     
        edh_next:       out std_ulogic;     

        -- EDH flag outputs
        rx_ap_flags:    out edh_flgset_type;
        rx_ff_flags:    out edh_flgset_type;
        rx_anc_flags:   out edh_flgset_type;
        ap_flags:       out edh_flgset_type;
        ff_flags:       out edh_flgset_type;
        anc_flags:      out edh_flgset_type;
        packet_flags:   out edh_pktflg_type;
        errcnt:         out edh_errcnt_type;
        edh_packet:     out std_ulogic);    
end component;


--
-- Checksum function
--
function calc_parity(d : std_ulogic_vector(7 downto 0)) return std_ulogic is
begin
    return (d(7) xor d(6) xor d(5) xor d(4) xor d(3) xor d(2) xor d(1) xor d(0));   
end calc_parity;            

begin
    
    --
    -- Map some internal signals to the signals at the top level of the
    -- testbench so they can be used as reference signals.
    --
    process
    begin
        init_signal_spy("/U1/dec_std", "/ep_dec_std", 0);
        init_signal_spy("/U1/dec_std_locked", "/ep_dec_std_locked", 0);
        init_signal_spy("/U1/dec_vid", "/ep_dec_vid", 0);
        init_signal_spy("/U1/dec_f", "/ep_dec_f", 0);
        init_signal_spy("/U1/dec_v", "/ep_dec_v", 0);
        init_signal_spy("/U1/dec_h", "/ep_dec_h", 0);
        init_signal_spy("/U1/dec_hcnt", "/ep_dec_hcnt", 0);
        init_signal_spy("/U1/dec_vcnt", "/ep_dec_vcnt", 0);
        init_signal_spy("/U1/dec_sync_switch", "/ep_dec_sync_switch", 0);
        init_signal_spy("/U1/dec_locked", "/ep_dec_locked", 0);
        init_signal_spy("/U1/dec_eav_next", "/ep_dec_eav_next", 0);
        init_signal_spy("/U1/dec_sav_next", "/ep_dec_sav_next", 0);
        init_signal_spy("/U1/dec_xyz_word", "/ep_dec_xyz_word", 0);
        init_signal_spy("/U1/dec_anc_next", "/ep_dec_anc_next", 0);
        init_signal_spy("/U1/dec_edh_next", "/ep_dec_edh_next", 0);
        init_signal_spy("/U1/ap_crc", "/ep_ap_crc", 0);
        init_signal_spy("/U1/ap_crc_valid", "/ep_ap_crc_valid", 0);
        init_signal_spy("/U1/ff_crc", "/ep_ff_crc", 0);
        init_signal_spy("/U1/ff_crc_valid", "/ep_ff_crc_valid", 0);
        init_signal_spy("/U1/anc_flags", "/ep_anc_flags", 0);
        init_signal_spy("/U1/ap_flags", "/ep_ap_flags", 0);
        init_signal_spy("/U1/ff_flags", "/ep_ff_flags", 0);
        wait;
    end process;

    --
    -- Instantiate the video processor module
    --
    U1 : edh_processor
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
            std             => std,
            std_locked      => std_locked,
            trs             => trs,
            vid_out         => vid_out,
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
            edh_packet      => edh_packet);

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
    -- This code keeps track of the number of EDH packets received. Packets received
    -- before the video decoder locks are not counted.
    --
    process
    begin
        packet <= 0;
        
        -- Wait for video decoder to lock to video  
        wait until rst = '0' and locked = '1';

        -- Increment the EDH packet number at each received packet
        loop
            wait until edh_packet'event and edh_packet = '1';
            packet <= packet + 1;
        end loop;
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
    -- When the frame counter changes, this is executed to set up frame specific
    -- values. For the first four frames, all error counter enable flags are
    -- asserted. For the last four frames, only the AP and FF EDH and EDA flags
    -- will cause the error counter to increment. When the frame counter reaches 8,
    -- the test stops.
    -- 
    process(frames)
        variable l : line;
        variable success_string :       string (1 to 28) 
                                        := "Test completed successfully.";
        variable error_string :         string (1 to 40)
                                        := "Expected 7 fields with errors, received ";
    begin
        if (frames >= 4) then
            errcnt_flg_en <= "0000110001100000";    -- 0x0c60
        end if;

        if (frames = 8) then
            
            if (errcnt = 7 and not error_found) then
                write(l, success_string);
                writeline(output, l); 
            else
                write(l, error_string);
                write(l, errcnt);
                writeline(output, l);           
            end if;

            enclk <= '0';
        end if;
    end process;

    --
    -- This code generates the vid_in value that is fed into the modules. Normally,
    -- vid_in is given the value of the d variable, but in somes cases it is
    -- modified to create error conditions.
    --
    process(clk)
    begin
        if (clk'event and clk = '0') then
            vid_in <= d;

            --
            -- During the first EDH packet of frame 2, the AP CRC value is
            -- modified to create a CRC error.
            --
            if (frames = 2) then
                if (i = (FIRST_EDH + AP_CRC_WORD0)) then
                    vid_in <= "1011010100";     -- 0x2d4
                elsif (i = (FIRST_EDH + FF_FLAG_WORD)) then
                    vid_in <= "0100000100";     -- 0x104
                elsif (i = (FIRST_EDH + CS_WORD)) then
                    vid_in <= "1001001000";     -- 0x248
                end if;

            --
            -- During the second EDH packet of frame 3, the EDH packet's checksum
            -- value is modified to create a checksum error.
            --
            elsif (frames = 3) then
                if (i = (SECOND_EDH + CS_WORD)) then
                    vid_in <= "1000000000";     --0x200
                end if;

            --
            -- During the first EDH packet of frame 4, the EDH packet is removed
            -- from the video stream to create an EDH packet missing error.
            --
            elsif (frames = 4) then
                if (i >= FIRST_EDH and i <= (FIRST_EDH + CS_WORD)) then
                    if (i - ((i / 2) * 2) = 0) then
                        vid_in <= "1000000000"; -- 0x200
                    else
                        vid_in <= "0001000000"; -- 0x040
                    end if;
                end if;

            --
            -- During the second field of frame 5, introduce an EDH packet format
            -- error by changing the value of the DBN word.
            --
            elsif (frames = 5) then
                if (i = (SECOND_EDH + DBN_WORD)) then
                    vid_in <= "1000000100";     --0x204;
                elsif (i = (SECOND_EDH + CS_WORD)) then
                    vid_in <= "0101101100";     --0x16c; 
                end if;

            --
            -- During the first field of frame 6, one video data word in the
            -- inactive portion of the video is modified to create a FF CRC error.
            -- The corrupted word is part of an ANC packet. This also causes a
            -- ANC EDH error because the checksum for the ANC packet will be
            -- wrong.
            --
            -- During the second EDH packet of frame 6, the AP EDH bit is set in
            -- the EDH packet.
            --
            elsif (frames = 6) then
                if (i = 5156) then
                    vid_in <= d xor "0000000001";
                elsif (i = (SECOND_EDH + AP_FLAG_WORD)) then
                    vid_in <= "0100000100";     --0x104;
                elsif (i = (SECOND_EDH + CS_WORD)) then
                    vid_in <= "1001101100";     --0x26c;
                end if;

            --
            -- During the first EDH packet of frame 7, the ANC EDH bit is set in
            -- the EDH packet.
            --
            -- During the second EDH packet of frame 7, the parity bit of one of
            -- the words of the packet is forced to an incorrect value to generate
            -- a parity error.
            --
            elsif (frames = 7) then
                if (i = (FIRST_EDH + ANC_FLAG_WORD)) then
                    vid_in <= "0100000100";     --0x104;
                elsif (i = (FIRST_EDH + CS_WORD)) then
                    vid_in <= "0101000100";     --0x144;
                elsif (i = (SECOND_EDH + AP_CRC_WORD2)) then
                    vid_in <= "0110001000";     --0x188;
                elsif (i = (SECOND_EDH + CS_WORD)) then
                    vid_in <= "1001101000";     --0x268;
                end if;
            end if;
        end if;
    end process;

    --
    -- This code generates the local error flag inputs to the EDH processor.
    --      
    process(packet)
    begin
        anc_idh_local <= '0';
        anc_ues_local <= '0';
        ap_idh_local <= '0';
        ff_idh_local <= '0';
        
        case packet is
            when 2 =>   anc_idh_local <= '1';
            when 3 =>   anc_ues_local <= '1';
            when 5 =>   ap_idh_local  <= '1';
            when 6 =>   ff_idh_local  <= '1';
            when 9 => 
                        anc_idh_local <= '1';
                        anc_ues_local <= '1';
            when 10 =>
                        ap_idh_local  <= '1';
                        ff_idh_local  <= '1';

            when others =>
        end case;
    end process;

    --
    -- This code generates the expected values that will be compared with the
    -- actual values generated by the EDH processor.
    --
    process(edh_packet)
    begin
        if (edh_packet'event and edh_packet = '0') then
            if (packet = 0) then
                check_flags <= false;
            else
                check_flags <= true;
            end if;
            
            case packet is
                when 1 =>
                    expected_rx_ap_flags <=  "00000";
                    expected_rx_ff_flags <=  "00000";
                    expected_rx_anc_flags <= "00000";
                    expected_ap_flags <=  "00000";
                    expected_ff_flags <=  "00000";
                    expected_anc_flags <= "00000";
                    expected_chksum_err <= '0';
                    expected_edh_missing <= '0';
                    expected_parity_err <= '0';
                    expected_format_err <= '0';

                when 2 =>
                    expected_rx_ap_flags <=  "00000";
                    expected_rx_ff_flags <=  "00000";
                    expected_rx_anc_flags <= "00000";
                    expected_ap_flags <=  "00000";
                    expected_ff_flags <=  "00000";
                    expected_anc_flags <= "00100";      -- ANC IDH
                    expected_chksum_err <= '0';
                    expected_edh_missing <= '0';
                    expected_parity_err <= '0';
                    expected_format_err <= '0';

                when 3 =>
                    expected_rx_ap_flags <=  "00000";
                    expected_rx_ff_flags <=  "00000";
                    expected_rx_anc_flags <= "00000";
                    expected_ap_flags <=  "00000";
                    expected_ff_flags <=  "00000";
                    expected_anc_flags <= "10000";      -- ANC UES
                    expected_chksum_err <= '0';
                    expected_edh_missing <= '0';
                    expected_parity_err <= '0';
                    expected_format_err <= '0';

                when 4 =>
                    expected_rx_ap_flags <=  "00000";
                    expected_rx_ff_flags <=  "00001";   -- FF EDH
                    expected_rx_anc_flags <= "00000";
                    expected_ap_flags <=  "00001";      -- AP EDH
                    expected_ff_flags <=  "00010";      -- FF EDA
                    expected_anc_flags <= "00000";
                    expected_chksum_err <= '0';
                    expected_edh_missing <= '0';
                    expected_parity_err <= '0';
                    expected_format_err <= '0';

                when 5 =>
                    expected_rx_ap_flags <=  "00000";
                    expected_rx_ff_flags <=  "00000";
                    expected_rx_anc_flags <= "00000";
                    expected_ap_flags <=  "00100";      -- AP IDH
                    expected_ff_flags <=  "00000";
                    expected_anc_flags <= "00000";
                    expected_chksum_err <= '0';
                    expected_edh_missing <= '0';
                    expected_parity_err <= '0';
                    expected_format_err <= '0';

                when 6 =>
                    expected_rx_ap_flags <=  "00000";
                    expected_rx_ff_flags <=  "00000";
                    expected_rx_anc_flags <= "00000";
                    expected_ap_flags <=  "00000";
                    expected_ff_flags <=  "00100";      -- FF IDH
                    expected_anc_flags <= "00000";
                    expected_chksum_err <= '0';
                    expected_edh_missing <= '0';
                    expected_parity_err <= '0';
                    expected_format_err <= '0';

                when 7 =>
                    expected_rx_ap_flags <=  "00000";
                    expected_rx_ff_flags <=  "00000";
                    expected_rx_anc_flags <= "00000";
                    expected_ap_flags <=  "00000";
                    expected_ff_flags <=  "00000";
                    expected_anc_flags <= "00000";
                    expected_chksum_err <= '1';         -- EDH checksum error
                    expected_edh_missing <= '0';
                    expected_parity_err <= '0';
                    expected_format_err <= '0';

                when 8 =>
                    expected_rx_ap_flags <=  "00000";
                    expected_rx_ff_flags <=  "00000";
                    expected_rx_anc_flags <= "00000";
                    expected_ap_flags <=  "10000";      -- UES
                    expected_ff_flags <=  "10000";      -- UES
                    expected_anc_flags <= "10000";      -- UES
                    expected_chksum_err <= '0';
                    expected_edh_missing <= '1';        -- missing EDH packet
                    expected_parity_err <= '0';
                    expected_format_err <= '0';

                when 9 =>
                    expected_rx_ap_flags <=  "00000";
                    expected_rx_ff_flags <=  "00000";
                    expected_rx_anc_flags <= "00000";
                    expected_ap_flags <=  "00000";
                    expected_ff_flags <=  "00000";
                    expected_anc_flags <= "10100";      -- IDH, UES
                    expected_chksum_err <= '0';
                    expected_edh_missing <= '0';
                    expected_parity_err <= '0';
                    expected_format_err <= '0';

                when 10 =>
                    expected_rx_ap_flags <=  "00000";
                    expected_rx_ff_flags <=  "00000";
                    expected_rx_anc_flags <= "00000";
                    expected_ap_flags <=  "00100";      -- AP IDH
                    expected_ff_flags <=  "00100";      -- FF IDH
                    expected_anc_flags <= "00000";
                    expected_chksum_err <= '0';
                    expected_edh_missing <= '0';
                    expected_parity_err <= '0';
                    expected_format_err <= '0';

                when 11 =>
                    expected_rx_ap_flags <=  "00000";
                    expected_rx_ff_flags <=  "00000";
                    expected_rx_anc_flags <= "00000";
                    expected_ap_flags <=  "10000";      -- AP UES
                    expected_ff_flags <=  "10000";      -- FF UES
                    expected_anc_flags <= "10000";      -- ANC UES
                    expected_chksum_err <= '0';
                    expected_edh_missing <= '0';
                    expected_parity_err <= '0';
                    expected_format_err <= '1';         -- EDH packet format error

                when 12 =>
                    expected_rx_ap_flags <=  "00000";
                    expected_rx_ff_flags <=  "00000";
                    expected_rx_anc_flags <= "00000";
                    expected_ap_flags <=  "00000";
                    expected_ff_flags <=  "00001";      -- FF EDH
                    expected_anc_flags <= "00001";      -- ANC EDH
                    expected_chksum_err <= '0';
                    expected_edh_missing <= '0';
                    expected_parity_err <= '0';
                    expected_format_err <= '0';

                when 13 =>
                    expected_rx_ap_flags <=  "00001";   -- AP EDH
                    expected_rx_ff_flags <=  "00000";
                    expected_rx_anc_flags <= "00000";
                    expected_ap_flags <=  "00010";      -- AP EDA
                    expected_ff_flags <=  "00000";
                    expected_anc_flags <= "00000";
                    expected_chksum_err <= '0';
                    expected_edh_missing <= '0';
                    expected_parity_err <= '0';
                    expected_format_err <= '0';

                when 14 =>
                    expected_rx_ap_flags <=  "00000";
                    expected_rx_ff_flags <=  "00000";
                    expected_rx_anc_flags <= "00001";   -- ANC EDH
                    expected_ap_flags <=  "00000";
                    expected_ff_flags <=  "00000";
                    expected_anc_flags <= "00010";      -- ANC EDA
                    expected_chksum_err <= '0';
                    expected_edh_missing <= '0';
                    expected_parity_err <= '0';
                    expected_format_err <= '0';

                when 15 =>
                    expected_rx_ap_flags <=  "00000";
                    expected_rx_ff_flags <=  "00000";
                    expected_rx_anc_flags <= "00000";
                    expected_ap_flags <=  "10000";
                    expected_ff_flags <=  "10000";
                    expected_anc_flags <= "10000";
                    expected_chksum_err <= '0';
                    expected_edh_missing <= '0';
                    expected_parity_err <= '1';   -- EDH packet parity error
                    expected_format_err <= '0';

                when others =>
                    expected_rx_ap_flags <=  "00000";
                    expected_rx_ff_flags <=  "00000";
                    expected_rx_anc_flags <= "00000";
                    expected_ap_flags <=  "00000";
                    expected_ff_flags <=  "00000";
                    expected_anc_flags <= "00000";
                    expected_chksum_err <= '0';
                    expected_edh_missing <= '0';
                    expected_parity_err <= '0';
                    expected_format_err <= '0';
            end case;
        end if; 
    end process;


    --
    -- This code compares the expected and received values of the flags from the
    -- EDH processor. They are compared on falling edge of the H blanking signal
    -- after the EDH packet is received.
    --          
    process                             
        variable l : line;              
        variable pkt_rx_str :               string(1 to 12) 
                                        := "EDH packet #";
        variable pkt_rx_str2 :              string(1 to 10)
                                        := " received.";
        variable actual_str :               string(1 to 9)
                                        := "actual = ";
        variable expected_str :             string(1 to 13)
                                        := "  expected = ";
        variable rxap_flg_mismatch_str:     string(1 to 26)
                                        := "  RX AP flags mismatched, ";
        variable rxff_flg_mismatch_str:     string(1 to 26)
                                        := "  RX FF flags mismatched, ";
        variable rxanc_flg_mismatch_str:    string(1 to 27)
                                        := "  RX ANC flags mismatched, ";
        variable ap_flg_mismatch_str :      string(1 to 23)
                                        := "  AP flags mismatched, ";
        variable ff_flg_mismatch_str :      string(1 to 23)
                                        := "  FF flags mismatched, ";
        variable anc_flg_mismatch_str :     string(1 to 24)
                                        := "  ANC flags mismatched, ";
        variable unexpected_cksum_err_str : string(1 to 48)
                                        := "  Unexpected EDH packet checksum error detected.";
        variable expected_cksum_err_str :   string(1 to 54)
                                        := "  Expected EDH packet checksum error was not detected.";
        variable unexpected_pkt_missing_err_str : string(1 to 36)
                                        := "  EDH packet missing error detected.";
        variable expected_pkt_missing_err_str : string(1 to 58)
                                        := "  Expected an EDH packet missing error, but didn't get it.";
        variable unexpected_parity_err_str : string(1 to 46)
                                        := "  Unexpected EDH packet parity error detected.";
        variable expected_parity_err_str : string(1 to 54)
                                        := "  Expected EDH packet parity error, but didn't get it.";
        variable unexpected_pkt_format_err_str : string(1 to 46)
                                        := "  Unexpected EDH packet format error detected.";
        variable expected_pkt_format_err_str : string(1 to 54)
                                        := "  Expected EDH packet format error, but didn't get it.";
    begin
        wait until (edh_packet'event and edh_packet = '0');
        wait until (h'event and h = '0');
        
        if (packet /= 0) then
            write(l, pkt_rx_str);
            write(l, packet);
            write(l, pkt_rx_str2);
            writeline(output, l); 
        end if;

        if (check_flags) then
            if (rx_ap_flags /= expected_rx_ap_flags) then
                write(l, rxap_flg_mismatch_str);
                write(l, actual_str);
                hwrite(l, rx_ap_flags, left, rx_ap_flags'length);
                write(l, expected_str);
                hwrite(l, expected_rx_ap_flags, left, expected_rx_ap_flags'length);
                writeline(output, l);
                error_found <= true;
            end if;
        
            if (rx_ff_flags /= expected_rx_ff_flags) then
                write(l, rxff_flg_mismatch_str);
                write(l, actual_str);
                hwrite(l, rx_ff_flags, left, rx_ff_flags'length);
                write(l, expected_str);
                hwrite(l, expected_rx_ff_flags, left, expected_rx_ff_flags'length);
                writeline(output, l);
                error_found <= true;
            end if;
        
            if (rx_anc_flags /= expected_rx_anc_flags) then
                write(l, rxanc_flg_mismatch_str);
                write(l, actual_str);
                hwrite(l, rx_anc_flags, left, rx_anc_flags'length);
                write(l, expected_str);
                hwrite(l, expected_rx_anc_flags, left, expected_rx_anc_flags'length);
                writeline(output, l);
                error_found <= true;
            end if;

            if (ap_flags /= expected_ap_flags) then
                write(l, ap_flg_mismatch_str);
                write(l, actual_str);
                hwrite(l, ap_flags, left, ap_flags'length);
                write(l, expected_str);
                hwrite(l, expected_ap_flags, left, expected_ap_flags'length);
                writeline(output, l);
                error_found <= true;
            end if;
        
            if (ff_flags /= expected_ff_flags) then
                write(l, ff_flg_mismatch_str);
                write(l, actual_str);
                hwrite(l, ff_flags, left, ff_flags'length);
                write(l, expected_str);
                hwrite(l, expected_ff_flags, left, expected_ff_flags'length);
                writeline(output, l);
                error_found <= true;
            end if;
        
            if (anc_flags /= expected_anc_flags) then
                write(l, anc_flg_mismatch_str);
                write(l, actual_str);
                hwrite(l, anc_flags, left, anc_flags'length);
                write(l, expected_str);
                hwrite(l, expected_anc_flags, left, expected_anc_flags'length);
                writeline(output, l);
                error_found <= true;
            end if;
        
            if (edh_chksum_err = '1' and expected_chksum_err = '0') then
                write(l, unexpected_cksum_err_str);
                writeline(output, l);
                error_found <= true;
            end if;
        
            if (edh_chksum_err = '0' and expected_chksum_err = '1') then
                write(l, expected_cksum_err_str);
                writeline(output, l);
                error_found <= true;
                end if;
        
            if (edh_missing = '1' and expected_edh_missing = '0') then
                write(l, unexpected_pkt_missing_err_str);
                writeline(output, l);
                error_found <= true;
                end if;
        
            if (edh_missing = '0' and expected_edh_missing = '1') then
                write(l, expected_pkt_missing_err_str);
                writeline(output, l);
                error_found <= true;
            end if;   
        
            if (edh_parity_err = '1' and expected_parity_err = '0') then
                write(l, unexpected_parity_err_str);
                writeline(output, l);
                error_found <= true;
            end if;
        
            if (edh_parity_err = '0' and expected_parity_err = '1') then
                write(l, expected_parity_err_str);
                writeline(output, l);
                error_found <= true;
            end if;

            if (edh_format_err = '1' and expected_format_err = '0') then
                write(l, unexpected_pkt_format_err_str);
                writeline(output, l);
                error_found <= true;
            end if;

            if (edh_format_err = '0' and expected_format_err = '1') then
                write(l, expected_pkt_format_err_str);
                writeline(output, l);
                error_found <= true;
            end if;
        end if; 
    end process;        


    --
    -- Delay registers that delay the video and video timing outputs of the video
    -- decoder so they can be compared with the outputs of the edh_processor.
    --
    process(clk)
    begin
        if (clk'event and clk = '1') then
            dly_std <= ep_dec_std;
            dly_std_locked <= ep_dec_std_locked;
            dly_vid <= ep_dec_vid;
            dly_f <= ep_dec_f;
            dly_v <= ep_dec_v;
            dly_h <= ep_dec_h;
            dly_hcnt <= ep_dec_hcnt;
            dly_vcnt <= ep_dec_vcnt;
            dly_sync_switch <= ep_dec_sync_switch;
            dly_locked <= ep_dec_locked;
            dly_eav_next <= ep_dec_eav_next;
            dly_sav_next <= ep_dec_sav_next;
            dly_xyz_word <= ep_dec_xyz_word;
            dly_anc_next <= ep_dec_anc_next;
            dly_edh_next <= ep_dec_edh_next;    
        end if;
    end process;


    --
    -- This code checks that the output video timing signals match the video
    -- decoders output signals (delayed by the delay registers above.
    --
    process(clk)
       
        variable dly_locked_err_str :   string(1 to 57) 
                                        := "locked output doesn't match video decoder's locked output";
        variable dly_std_err_str :      string(1 to 47) 
                                        := "std output doesn't match video decoder's output";
        variable dly_std_locked_err_str : string(1 to 54) 
                                        := "std_locked output doesn't match video decoder's output";
        variable dly_f_err_str :        string(1 to 54) 
                                        := "std_locked output doesn't match video decoder's output";
    begin
        if (clk'event and clk = '0') then
                
            assert (locked = dly_locked)
                report "locked output doesn't match video decoder's locked output"
                severity error;

            if (locked = '1') then
                assert (std = dly_std)
                    report "std output doesn't match video decoder's output"
                    severity error;
                     
                assert (std_locked = dly_std_locked)
                    report "std_locked output doesn't match video decoder's output"
                    severity error;

                assert (f = dly_f)
                    report "f output doesn't match video decoder's output"
                    severity error;
                    
                assert (v = dly_v)
                    report "v output doesn't match video decoder's output"
                    severity error;
                    
                assert (h = dly_h)
                    report "h output doesn't match video decoder's output"
                    severity error;
                    
                assert (hcnt = dly_hcnt)
                    report "hcnt output doesn't match video decoder's output"
                    severity error;
                    
                assert (vcnt = dly_vcnt)
                    report "vcnt output doesn't match video decoder's output"
                    severity error;
                    
                assert (sync_switch = dly_sync_switch)
                    report "sync_switch output doesn't match video decoder's output"
                    severity error;
                    
                assert (eav_next = dly_eav_next)
                    report "eav_next output doesn't match video decoder's output"
                    severity error;
                    
                assert (sav_next = dly_sav_next)
                    report "sav_next output doesn't match video decoder's output"
                    severity error;
                    
                assert (xyz_word = dly_xyz_word)
                    report "xyz_word output doesn't match video decoder's output"
                    severity error;
                    
                assert (anc_next = dly_anc_next)
                    report "anc_next output doesn't match video decoder's output"
                    severity error;

                assert (edh_next = dly_edh_next)
                    report "edh_next output doesn't match video decoder's output"
                    severity error;
            end if;
        end if;
    end process;    

    --
    -- This clode checks that the vid_out signal matches the video out of the video
    -- decoder during the active portions of the video display. The edh_processor
    -- should never modify the active portion of the video.
    --  
    process(clk)
    begin
        if (clk'event and clk = '0') then
            if (locked = '1' and h = '0' and v = '0') then
                assert (vid_out = dly_vid)
                    report "vid_out output doesn't match video decoder's output"
                    severity error;
            end if; 
        end if;
    end process;

    --
    -- The code below verifies the formatting of the EDH packets generated by the
    -- edh_processor. Other code in this testbench verifies that the CRC values and
    -- flags are as expected. This code verifies that the CRC values and flags were
    -- formatted into an EDH packet properly and that the checksum for the EDH
    -- packet is valid.
    --
    process(clk)
    variable edh_word :     integer;
    variable edh_data :     std_ulogic_vector(7 downto 0);
    variable edh_parity :   std_ulogic;
    variable edh_expected : std_ulogic_vector(9 downto 0);
    variable edh_checksum : std_ulogic_vector(8 downto 0);

    begin
        if (clk'event and clk = '0') then
            
            if (locked = '1' and edh_next = '1') then
                edh_word := 0;
            end if;

            if (locked = '1' and edh_packet = '1') then
                case edh_word is
                    when 0 =>
                        assert (vid_out = "0000000000")
                            report "vid_out isn't equal to first word of ADF (000)"
                            severity error;
                                
                    when 1 =>
                        assert (vid_out = "1111111111")
                            report "vid_out isn't equal to second word of ADF (3FF)"
                            severity error;
                                                        
                    when 2 =>   
                        assert (vid_out = "1111111111")
                            report "vid_out isn't equal to third word of ADF (3FF)"
                            severity error;
                                                        
                    when 3 =>
                        assert (vid_out = "0111110100")
                            report "vid_out isn't equal to DID word of EDH (1F4)"
                            severity error;

                        edh_checksum := vid_out(8 downto 0);

                    when 4 =>
                        assert (vid_out = "1000000000")
                            report "vid_out isn't equal to DBN word of EDH (200)"
                            severity error;

                        edh_checksum := std_ulogic_vector(std_logic_vector(edh_checksum) 
                                        + std_logic_vector(vid_out(8 downto 0)));

                    when 5 =>
                        assert (vid_out = "0100010000")
                            report "vid_out isn't equal to DC word of EDH (110)"
                            severity error;

                        edh_checksum := std_ulogic_vector(std_logic_vector(edh_checksum)
                                         + std_logic_vector(vid_out(8 downto 0)));

                    when 6 =>
                        edh_data := (ep_ap_crc(5 downto 0) & "00");
                        edh_parity := calc_parity(edh_data);
                        edh_expected := (not edh_parity & edh_parity & edh_data);
                        edh_checksum := std_ulogic_vector(std_logic_vector(edh_checksum) 
                                        + std_logic_vector(edh_expected(8 downto 0)));

                        assert (vid_out = edh_expected)
                            report "vid_out doesn't match first AP CRC word of EDH packet"
                            severity error;

                    when 7 =>
                        edh_data := (ep_ap_crc(11 downto 6) & "00");
                        edh_parity := calc_parity(edh_data);
                        edh_expected := (not edh_parity & edh_parity & edh_data);
                        edh_checksum := std_ulogic_vector(std_logic_vector(edh_checksum) 
                                        + std_logic_vector(edh_expected(8 downto 0)));

                        assert (vid_out = edh_expected)
                            report "vid_out doesn't match second AP CRC word of EDH packet"
                            severity error;

                    when 8 =>
                        edh_data := (ep_ap_crc_valid & '0' & ep_ap_crc(15 downto 12) & "00");
                        edh_parity := calc_parity(edh_data);
                        edh_expected := (not edh_parity & edh_parity & edh_data);
                        edh_checksum := std_ulogic_vector(std_logic_vector(edh_checksum) 
                                        + std_logic_vector(edh_expected(8 downto 0)));

                        assert (vid_out = edh_expected)
                            report "vid_out doesn't match third AP CRC word of EDH packet"
                            severity error;

                    when 9 =>
                        edh_data := (ep_ff_crc(5 downto 0) & "00");
                        edh_parity := calc_parity(edh_data);
                        edh_expected := (not edh_parity & edh_parity & edh_data);
                        edh_checksum := std_ulogic_vector(std_logic_vector(edh_checksum) 
                                        + std_logic_vector(edh_expected(8 downto 0)));

                        assert (vid_out = edh_expected)
                            report "vid_out doesn't match first FF CRC word of EDH packet"
                            severity error;

                    when 10 =>
                        edh_data := (ep_ff_crc(11 downto 6) & "00");
                        edh_parity := calc_parity(edh_data);
                        edh_expected := (not edh_parity & edh_parity & edh_data);
                        edh_checksum := std_ulogic_vector(std_logic_vector(edh_checksum) 
                                        + std_logic_vector(edh_expected(8 downto 0)));
                        
                        assert (vid_out = edh_expected)
                            report "vid_out doesn't match second FF CRC word of EDH packet"
                            severity error;

                    when 11 =>
                        edh_data := (ep_ff_crc_valid & '0' & ep_ff_crc(15 downto 12) & "00");
                        edh_parity := calc_parity(edh_data);
                        edh_expected := (not edh_parity & edh_parity & edh_data);
                        edh_checksum := std_ulogic_vector(std_logic_vector(edh_checksum) 
                                        + std_logic_vector(edh_expected(8 downto 0)));
                        
                        assert (vid_out = edh_expected)
                            report "vid_out doesn't match third FF CRC word of EDH packet"
                            severity error;

                    when 12 =>
                        edh_data := ('0' & ep_anc_flags & "00");
                        edh_parity := calc_parity(edh_data);
                        edh_expected := (not edh_parity & edh_parity & edh_data);
                        edh_checksum := std_ulogic_vector(std_logic_vector(edh_checksum) 
                                        + std_logic_vector(edh_expected(8 downto 0)));
                        
                        assert (vid_out = edh_expected)
                            report "vid_out doesn't match ANC flags word of EDH packet"
                            severity error;
                                    
                    when 13 =>
                        edh_data := ('0' & ep_ap_flags & "00");
                        edh_parity := calc_parity(edh_data);
                        edh_expected := (not edh_parity & edh_parity & edh_data);
                        edh_checksum := std_ulogic_vector(std_logic_vector(edh_checksum) 
                                        + std_logic_vector(edh_expected(8 downto 0)));
                        
                        assert (vid_out = edh_expected)
                            report "vid_out doesn't match AP flags word of EDH packet"
                            severity error;
                        
                    when 14 =>
                        edh_data := ('0' & ep_ff_flags & "00");
                        edh_parity := calc_parity(edh_data);
                        edh_expected := (not edh_parity & edh_parity & edh_data);
                        edh_checksum := std_ulogic_vector(std_logic_vector(edh_checksum) 
                                        + std_logic_vector(edh_expected(8 downto 0)));
                        
                        assert (vid_out = edh_expected)
                            report "vid_out doesn't match FF flags word of EDH packet"
                            severity error;
                                     
                    when 15 =>
                        assert (vid_out = "1000000000")
                            report "vid_out doesn't match reserved word value of EDH packet"
                            severity error;

                    when 16 =>
                        assert (vid_out = "1000000000")
                            report "vid_out doesn't match reserved word value of EDH packet"
                            severity error;
                    when 17 =>
                        assert (vid_out = "1000000000")
                            report "vid_out doesn't match reserved word value of EDH packet"
                            severity error;
                    when 18 =>
                        assert (vid_out = "1000000000")
                            report "vid_out doesn't match reserved word value of EDH packet"
                            severity error;
                    when 19 =>
                        assert (vid_out = "1000000000")
                            report "vid_out doesn't match reserved word value of EDH packet"
                            severity error;
                    when 20 =>
                        assert (vid_out = "1000000000")
                            report "vid_out doesn't match reserved word value of EDH packet"
                            severity error;
                    when 21 =>
                        assert (vid_out = "1000000000")
                            report "vid_out doesn't match reserved word value of EDH packet"
                            severity error;

                    when 22 =>
                        edh_expected := (not edh_checksum(8) & edh_checksum);
                        
                        assert (vid_out = edh_expected)
                            report "vid_out doesn't match CS word of EDH packet"
                            severity error;

                    when others =>
                end case;
                edh_word := edh_word + 1;
            end if;
        end if;
    end process;

end sim;