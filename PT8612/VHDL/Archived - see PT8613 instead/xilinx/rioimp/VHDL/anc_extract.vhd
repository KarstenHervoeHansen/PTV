-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: anc_extract.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2005-01-18 10:39:25-07 $
-- /___/   /\    Date Created: Jan 5, 2005
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: anc_extract.vhd,rcs $
-- Revision 1.0  2005-01-18 10:39:25-07  jsnow
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
-- This module is instanced by the anc_demux module to do all the work of packet
-- demultiplexing.
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

entity anc_extract is
    port (
        -- inputs
        clk:            in    std_ulogic;   -- clock input
        ce:             in    std_ulogic;   -- clock enable
        rst:            in    std_ulogic;   -- async reset input
        anc_next:       in    std_ulogic;   -- ANC packet begins next word
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

        -- outputs
        data_out:       out   video_type;   -- output packet data
        data_out_valid: out   std_ulogic;   -- asserted while all words of a matching packet are on data_out
        match_code:     out                 -- indicates DID/SDID combination matched the current packet
            std_ulogic_vector(1 downto 0);  
        did:            out   std_ulogic;   -- asserted when a DID word from a matching packet is on data_out
        dbn:            out   std_ulogic;   -- asserted when a DBN word from a matching packet is on data_out
        sdid:           out   std_ulogic;   -- asserted when an SDID word from a matching packet is on data_out
        dc:             out   std_ulogic;   -- asserted when a DC word from a matching packet is on data_out
        udw:            out   std_ulogic;   -- asserted when a UDW word from a matching packet is on data_out
        cs:             out   std_ulogic;   -- asserted when a CS word from a matching packet is on data_out
        vid_out:        out   video_type);  -- output video stream
end;

architecture synth of anc_extract is

constant LATENCY :      integer := 3;

-------------------------------------------------------------------------------
-- Subtype definitions
--      
subtype delay     is    -- used to delay 1-bit video timing signals to match latency
            std_ulogic_vector(LATENCY-1 downto 0);

subtype matchcode is    -- code indicating which DID/SDID input pair matched ANC packet
            std_ulogic_vector(1 downto 0);

subtype matchvec  is    -- unary bit version of matchcode
            std_ulogic_vector(3 downto 0);

-------------------------------------------------------------------------------
-- Constant definitions
--      

--
-- This group of constants defines the states of the EDH processor state
-- machine.
--
constant STATE_WIDTH :  integer := 4;

subtype state           is std_ulogic_vector(STATE_WIDTH - 1 downto 0);

constant S_WAIT :       state := "0000";
constant S_ADF1 :       state := "0001";
constant S_ADF2 :       state := "0010";
constant S_ADF3 :       state := "0011";
constant S_DID :        state := "0100";
constant S_SDID :       state := "0101";
constant S_DC :         state := "0110";
constant S_UDW :        state := "0111";
constant S_CS :         state := "1000";


-------------------------------------------------------------------------------
-- Signal definitions
--
signal current_state:   state;          -- FSM current state
signal next_state:      state;          -- FSM next state
signal vid_in_reg:      video_type;     -- video input pipeline reg
signal vid_dly_1:       video_type;     -- video pipeline delay reg
signal vid_dly_2:       video_type;     -- video pipeline delay reg
signal vid_mux:         video_type;     -- output video MUX
signal anc_next_dly:    delay;          -- delay reg for anc_next signal
signal pkt_type:        std_ulogic;     -- 1=type 1 ANC pkt, 0 = type 2
signal pkt_delete:      std_ulogic;     -- asserted if pkt should be deleted
signal ld_udw_cntr:     std_ulogic;     -- loads the UDW counter
signal udw_cntr:        ubyte_type;     -- counts the UDW words in packet
signal udw_cntr_eq_0:   std_ulogic;     -- asserted when UDW counter - 1 = 0
signal udw_cntr_mux:    ubyte_type;     -- input MUX for UDW counter
signal do_delete:       std_ulogic;     -- causes DID to be replaced by deletion code
signal ld_match_code:   std_ulogic;     -- loads the match_code output register
signal check_did:       std_ulogic;     -- controls the comparator input MUXes
signal mux_a:           ubyte_type;     -- cmp_a input mux
signal mux_b:           ubyte_type;     -- cmp_b input mux
signal mux_c:           ubyte_type;     -- cmp_c input mux
signal mux_d:           ubyte_type;     -- cmp_d input mux
signal a_x:             ubyte_type;     -- mux_a output after 8-bit conversion
signal b_x:             ubyte_type;     -- mux_b output after 8-bit conversion
signal c_x:             ubyte_type;     -- mux_c output after 8-bit conversion
signal d_x:             ubyte_type;     -- mux_d output after 8-bit conversion
signal in_x:            ubyte_type;     -- DID or SDID from input video after 8-bit conversion
signal cmp_a:           std_ulogic;     -- compares a_x with in_x
signal cmp_b:           std_ulogic;     -- compares b_x with in_x
signal cmp_c:           std_ulogic;     -- compares c_x with in_x
signal cmp_d:           std_ulogic;     -- compares d_x with in_x
signal did_match:       matchvec;       -- register that holds cmp_[a:d] outputs
signal sdid_match:      matchvec;       -- vector containing SDID comparison results
signal matches:         matchvec;       -- vector indicating did/sdid pair matches
signal encoded_match:   matchcode;      -- priority encoded packet match value
signal match:           std_ulogic;     -- asserted on a packet match
signal set_do_valid:    std_ulogic;     -- sets the data_out_valid signal
signal clr_do_valid:    std_ulogic;     -- clears the data_out_valid signal
signal checksum:        cksum_type;     -- checksum generator
signal clr_checksum:    std_ulogic;     -- clears the checksum generator
signal output_checksum: std_ulogic;     -- checksum is output when asserted
signal did_8_bit_d:     std_ulogic;     -- asserted when DID indicates 8-bit value
signal did_8_bit:       std_ulogic;     -- registered versio of did_8_bit_d
signal eight_bit:       std_ulogic;     -- asserted when DID/SDID pair are 8-bit values
signal ld_pkt_type :    std_ulogic;     -- loads the pkt_type register

begin
    
    --
    -- FSM: current_state register
    --
    -- This code implements the current state register. 
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            current_state <= S_WAIT;
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                current_state <= next_state;
            end if;
        end if;
    end process;

    --
    -- FSM: next_state logic
    --
    -- This case statement generates the next_state value for the FSM based on
    -- the current_state and the various FSM inputs.
    --
    process(current_state, anc_next_dly(2), match, udw_cntr_eq_0)
    begin
        case current_state is
            when S_WAIT =>  
                if (anc_next_dly(2) = '1') then
                    next_state <= S_ADF1;
                else
                    next_state <= S_WAIT;
                end if;

            when S_ADF1 =>
                next_state <= S_ADF2;

            when S_ADF2 =>
                next_state <= S_ADF3;

            when S_ADF3 =>
                if (match = '1') then
                    next_state <= S_DID;
                else
                    next_state <= S_WAIT;
                end if;

            when S_DID =>
                next_state <= S_SDID;

            when S_SDID =>
                next_state <= S_DC;

            when S_DC =>
                if (udw_cntr_eq_0 = '1') then
                    next_state <= S_CS;
                else
                    next_state <= S_UDW;
                end if;

            when S_UDW =>
                if (udw_cntr_eq_0 = '1') then
                    next_state <= S_CS;
                else
                    next_state <= S_UDW;
                end if;

            when S_CS =>
                if (anc_next_dly(2) = '1') then
                    next_state <= S_ADF1;
                else
                    next_state <= S_WAIT;
                end if;

            when others =>
                next_state <= S_WAIT;
        end case;   
    end process;
                            
    --
    -- FSM: outputs
    --
    -- This block decodes the current state to generate the various outputs of the
    -- FSM.
    --
    process(current_state, pkt_type, pkt_delete, match)
    begin
        -- Unless specifically assigned in the case statement, all FSM outputs
        -- are as defined below.
        do_delete           <= '0';
        set_do_valid        <= '0';
        clr_do_valid        <= '0';
        did                 <= '0';
        sdid                <= '0';
        dbn                 <= '0';
        dc                  <= '0';
        udw                 <= '0';
        cs                  <= '0';
        ld_udw_cntr         <= '0';
        ld_match_code       <= '0';
        clr_checksum        <= '0';
        output_checksum     <= '0';
        check_did           <= '0';
        ld_pkt_type         <= '0';

        case current_state is
            when S_ADF2 =>
                check_did <= '1';
                ld_pkt_type <= '1';

            when S_ADF3 =>
                if (match = '1') then
                    set_do_valid <= '1';
                else
                    set_do_valid <= '0';
                end if;
                clr_checksum <= '1';
                ld_match_code <= '1';

            when S_DID =>
                did <= '1';
                if (pkt_delete = '1') then
                    do_delete <= '1';
                end if;

            when S_SDID =>
                if (pkt_type = '1') then
                    dbn <= '1';
                else
                    sdid <= '1';
                end if;

            when S_DC =>
                dc <= '1';
                ld_udw_cntr <= '1';

            when S_UDW =>
                udw <= '1';

            when S_CS =>
                cs <= '1';
                clr_do_valid <= '1';
                output_checksum <= '1';

            when others =>
        end case;
    end process;

    --
    -- pkt_delete register
    --
    -- This register has a MUX at the input. This MUX selects the appropriate
    -- del_pkt_[a:d] input depending on which DID/SDID pair matched the packet.
    -- The register implements a one clock delay so that pkt_delete is valid when
    -- the state machine needs it.
    --
    process(clk,rst)
    begin
        if (rst = '1') then
            pkt_delete <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                case encoded_match is
                    when "00" =>   pkt_delete <= del_pkt_a;
                    when "01" =>   pkt_delete <= del_pkt_b;
                    when "10" =>   pkt_delete <= del_pkt_c;
                    when "11" =>   pkt_delete <= del_pkt_d;
                    when others => pkt_delete <= '0'; 
                end case;
            end if;
        end if;
    end process;

    --
    -- video delay registers
    --
    -- The input video stream is delayed through a pipeline of three registers so
    -- that the module has time to examine the DID and SDID words for a match 
    -- before acting on the packet.
    --
    process(clk,rst)
    begin
        if (rst = '1') then
            vid_in_reg <= (others => '0');
            vid_dly_1 <= (others => '0');
            vid_dly_2 <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                vid_in_reg <= vid_in;
                vid_dly_1 <= vid_in_reg;
                vid_dly_2 <= vid_dly_1;
            end if;
        end if;
    end process;
            
    --
    -- video output MUX
    --
    -- The output video stream is normally equal to the contents of the vid_dly_2
    -- register. However, if do_delete is asserted, the deletion code is output
    -- in place of the DID word. If the output_checksum signal is asserted, then
    -- the contents of the checksum generator is output in place of the checksum
    -- in the packet. Note that the MUX is actually implemented as two cascaded
    -- MUXes because the checksum generator needs the intermediate value called
    -- vid_mux.
    --

    vid_mux <= DEL_DID when do_delete = '1' else vid_dly_2;

    vid_out <= ((not checksum(8)) & checksum) when output_checksum = '1' 
               else vid_mux;
     
    --
    -- UDW counter
    --
    -- The UDW counter counts the number of user data words in the packet so that
    -- the FSM knows when all of the user data words in the packet have been
    -- processed. A MUX selects between the DC value from the input video stream
    -- or the contents of the UDW counter. The output of the MUX is decrement by
    -- one before being loaded into the UDW counter. The output of the MUX is also
    -- tested to see if it is equal to zero and udw_cntr_eq_0 is asserted if so.
    -- Because the FSM needs to test both the DC value and the UDW counter for zero
    -- this design uses only one comparator to do both tests.
    --
    udw_cntr_mux <= vid_dly_2(7 downto 0) when ld_udw_cntr = '1' else udw_cntr;

    udw_cntr_eq_0 <= '1' when udw_cntr_mux = UBYTE_ZERO else '0';

    process(clk, rst)
    begin
        if (rst = '1') then
            udw_cntr <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                udw_cntr <= std_ulogic_vector(std_logic_vector(udw_cntr_mux) - 1);
            end if;
        end if;
    end process;

    -- 
    -- anc_next_dly register
    --
    -- This register delays the anc_next input signal by three clock cycles -- 
    -- matching the pipeline delay that the module implements on the video path.
    --
    process(clk,rst)
    begin
        if (rst = '1') then
            anc_next_dly <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                anc_next_dly <= (anc_next_dly(1 downto 0) & anc_next);
            end if;
        end if;
    end process;

    --
    -- data_out_valid logic
    --
    -- The data_out_valid signal is controlled by the set_do_valid and clr_do_valid
    -- signals from the FSM. This signal becomes asserted when the DID word is
    -- present on the data_out port and stays asserted through the clock cycle when
    -- the checksum word is present on the data_out port.
    --
    process(clk,rst)
    begin
        if (rst = '1') then
            data_out_valid <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (set_do_valid = '1') then
                    data_out_valid <= '1';
                elsif (clr_do_valid = '1') then
                    data_out_valid <= '0';
                end if;
            end if;
        end if;
    end process;

    --
    -- The data_out port carries the demuxed packet information. It is simply
    -- set to the value of the vid_dly_2 register. The data_out_valid signal
    -- can be used to determine when valid information is present on this port.
    --
    data_out <= vid_dly_2;

    --
    -- checksum generator
    --
    -- This logic generates a 9-bit checksum for the ANC packet. A new checksum
    -- is generated in case the packet was marked for deletion by the module by
    -- changing the DID value. This causes the checksum to change.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            checksum <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (clr_checksum = '1') then
                    checksum <= (others => '0');
                else
                    checksum <= std_ulogic_vector(std_logic_vector(checksum) +
                                                  std_logic_vector(vid_mux(8 downto 0)));
                end if;
            end if;
        end if;
    end process;

    -----------------------------------------------------------------------------
    -- DID/SDID match logic
    --
    -- This logic first compares the DID word with the four input DID values. Next
    -- The SDID word is compared with the four input SDID values but this comparison
    -- is only used for type 2 packets (pkt_type = 0). If the DID/SDID pair matches
    -- one of the four input pairs, then the match signal is asserted, and a 2-bit 
    -- code indicating which pair matched is loaded into the match_code register.
    --

    --
    -- This code generates the did_8_bit_d and did_8_bit signals. The did_8_bit_d
    -- signal is asserted when the vid_in_reg value indicates an 8-bit DID value.
    -- This signal is delayed by one clock cycle to generated the did_8_bit signal.
    -- The eight_bit signal is a combination of these two signals. It is equal to
    -- did_8_bit_d when the DID word is in the vid_in_reg, otherwise it is equal
    -- to the did_8_bit signal.
    --
    did_8_bit_d <= not vid_in_reg(7) and not vid_in_reg(6) and not vid_in_reg(5) and
                   not vid_in_reg(4) and (vid_in_reg(3) or vid_in_reg(2));

    process(clk,rst)
    begin
        if (rst = '1') then
            did_8_bit <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                did_8_bit <= did_8_bit_d;
            end if;
        end if;
    end process;

    eight_bit <= did_8_bit_d when check_did = '1' else did_8_bit;

    --
    -- The pkt_type signal is derived from the DID word in the input video stream.
    -- It is asserted for type 1 ANC packets and negated for type 2 ANC packets.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            pkt_type <= '0';
        elsif (clk'event and clk = '1') then
            if (ld_pkt_type = '1') then
                pkt_type <= vid_in_reg(7);
            end if;
        end if;
    end process;

    --
    -- The following MUXes select between the the DID and SDID values for each pair
    -- so that the proper value is fed to the comparators to be tested against the
    -- DID and SDID words in the packet.
    --
    mux_a <= did_a when check_did = '1' else sdid_a;
    mux_b <= did_b when check_did = '1' else sdid_b;
    mux_c <= did_c when check_did = '1' else sdid_c;
    mux_d <= did_d when check_did = '1' else sdid_d;

    --
    -- These equations are for 8-bit conditioning. The outputs of the DID/SDID\
    -- MUXes are converted to 8-bit values if the eight_bit signal is asserted.
    -- Likewise, the DID and SDID words from the input video stream are converted
    -- to 8-bit values if eight_bit is asserted. 
    a_x <=  (mux_a(7 downto 2) & (mux_a(1) and not eight_bit) & 
             (mux_a(0) and not eight_bit));
    b_x <=  (mux_b(7 downto 2) & (mux_b(1) and not eight_bit) & 
             (mux_b(0) and not eight_bit));
    c_x <=  (mux_c(7 downto 2) & (mux_c(1) and not eight_bit) & 
             (mux_c(0) and not eight_bit));
    d_x <=  (mux_d(7 downto 2) & (mux_d(1) and not eight_bit) & 
             (mux_d(0) and not eight_bit));
    in_x <= (vid_in_reg(7 downto 2) & (vid_in_reg(1) and not eight_bit) & 
             (vid_in_reg(0) and not eight_bit));
    --
    -- These MUXes first compare the DID values with the contents of the vid_in_reg
    -- and then, on the next clock cycle, they compare the SDID values with the
    -- contents of the vid_in_reg.
    --
    cmp_a <= '1' when en_a = '1' and (a_x = in_x) else '0';
    cmp_b <= '1' when en_b = '1' and (b_x = in_x) else '0';
    cmp_c <= '1' when en_c = '1' and (c_x = in_x) else '0';
    cmp_d <= '1' when en_d = '1' and (d_x = in_x) else '0';

    --
    -- The did_match register holds the outputs of the comparators after the DID
    -- comparison is done.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            did_match <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                did_match <= (cmp_d & cmp_c & cmp_b & cmp_a);
            end if;
        end if;
    end process;

    --
    -- This statement essentially ORs the pkt_type bit with the output of the
    -- comparators when the SDID comparison is done. This insures that for type 1
    -- packets the results of the SDID word comparison are forced to be true. For
    -- type 1 packets, there is no SDID word and the comparison is invalid.
    --
    sdid_match <= (others => '1') when pkt_type = '1' 
                  else (cmp_d & cmp_c & cmp_b & cmp_a);

    --
    -- If both the DID word and the SDID word match, then the corresponding matches
    -- bit is asserted.
    --
    matches <= sdid_match and did_match;

    --
    -- The match signal is asserted if any of the DID/SDID pairs were successfully
    -- matched with the packet. It is generated by ORing the matches vector.
    --
    match <= matches(3) or matches(2) or matches(1) or matches(0);

    --
    -- This code generates the encoded_match signal. This is a priority encoded
    -- value derived from the matches vector.
    --
    process(matches)
    begin
        if (matches(0) = '1') then
            encoded_match <= "00";
        elsif (matches(1) = '1') then
            encoded_match <= "01";
        elsif (matches(2) = '1') then
            encoded_match <= "10";
        elsif (matches(3) = '1') then
            encoded_match <= "11";
        else
            encoded_match <= "00";
        end if;
    end process;

    --
    -- The match_code register holds the encoded_match value for the entire duration
    -- that the packet is being output by the module.
    --
    process(clk,rst)
    begin
        if (rst = '1') then
            match_code <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1' and ld_match_code = '1') then
                match_code <= encoded_match;
            end if;
        end if;
    end process;

end synth;