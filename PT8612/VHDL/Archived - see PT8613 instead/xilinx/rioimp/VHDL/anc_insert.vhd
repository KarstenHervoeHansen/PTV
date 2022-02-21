-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: anc_insert.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2005-01-18 10:39:53-07 $
-- /___/   /\    Date Created: Jan 5, 2005
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: anc_insert.vhd,rcs $
-- Revision 1.0  2005-01-18 10:39:53-07  jsnow
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
-- This module is instanced by the anc_mux module. The anc_insert module
-- searches the video stream for an appropriate place to insert a new ANC
-- packet. The ANC space must be of the appropriate type (HANC or VANC) as
-- requested by the hanc_pkt and vanc_pkt inputs. The space must also be large
-- enough to accomodate a packet of the size indicated by the pkt_size value.
-- This module will overwrite end_marker packets but does not overwrite packets
-- that have been marked for deletion.
-- 
-- The new ANC packet data comes in on the anc_in port. When the pkt_rdy
-- input is asserted, the module begins looking for a free ANC space for the
-- packet. Note that when pkt_rdy is asserted, the module assumes that the first
-- word of the ADF is present on the anc_in port. The total number of words in
-- the new packet must also be present on the pkt_size port when pkt_rdy is
-- asserted.
-- 
-- The module will always assure that ANC packets are contiguous in the ANC
-- space. New packets will only be inserted immediately after the TRS symbols
-- at the beginning of the ANC space, after an existing EDH packet, or in place
-- of an end-marker ANC packet. The module respects the "keep-out" area around
-- the synchronous switching interval and also the spaces reserved for EDH
-- packets.
-- 
-- When the modules finds a candidate ANC space for the new packet, it asserts
-- the send_pkt output. At the next rising clock edge, the anc_in port must 
-- change to the second word of the ADF. Each word of the new packet must be 
-- placed on anc_in during each successive clock cycle. When the last word (CS) 
-- of the ANC packet is on the anc_in port, the pkt_end signal must also be 
-- asserted to let the module know that the end of the packet has been reached. 
-- Note that send_pkt is asserted by the module for only a single clock cycle to 
-- begin the process of sending the ANC packet.
-- 
-- In some cases, the module will attempt to insert the packet, only to find out
-- that the packet couldn't really be inserted. This is because either the ANC
-- space was part of the prohibited synchronous switching interval or because
-- another ANC packet already exists in the video stream and cannot be
-- overwritten. In this case, the module will assert the abort_pkt signal. The 
-- module sending the packet to anc_insert must stop sending the packet and 
-- resend the same packet when anc_insert next asserts the send_pkt signal. The
-- anc_insert module is designed in such a way that when a packet insertion is 
-- aborted, the data in the video stream is not corrupted in any way.
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

use work.anc_edh_pkg.all;

entity anc_insert is
    port (
        -- inputs
        clk:            in  std_ulogic;  -- clock input
        ce:             in  std_ulogic;  -- clock enable
        rst:            in  std_ulogic;  -- async reset input
        pkt_rdy :       in  std_ulogic;  -- indicates packet is ready to be sent
        anc_in :        in  video_type;  -- ANC packet data input
        vid_in :        in  video_type;  -- video data input
        pkt_size :      in  pktsize_type;-- size of packet to be sent
        pkt_end :       in  std_ulogic;  -- indicates last word of packet being sent
        anc_next :      in  std_ulogic;  -- vid_in contains ANC packet starting at next word
        eav_next :      in  std_ulogic;  -- vid_in contains EAV starting at next word
        sav_next :      in  std_ulogic;  -- vid_in contains SAV starting at next word
        hanc_pkt :      in  std_ulogic;  -- ANC packet to be sent can be inserted into HANC space
        vanc_pkt :      in  std_ulogic;  -- ANC packet to be sent can be inserted into VANC space
        std :           in  vidstd_type; -- current video standard
        v_blank :       in  std_ulogic;  -- vertical blanking indicator
        field :         in  std_ulogic;  -- field indicator
        horz_count :    in  hpos_type;   -- current horizontal position
        vert_count :    in  vpos_type;   -- current vertical position
        switch_line :   in  std_ulogic;  -- current line is a sync switching line
    
        -- outputs
        vid_out :       out video_type;  -- output video data
        send_pkt :      out std_ulogic;  -- tells anc_pkt_gen to send packet
        abort_pkt :     out std_ulogic); -- tells anc_pkt_gen to abort packet
end;
        
architecture synth of anc_insert is

-- The padding subtype is used to pad the packet size vector to the lenth of
-- the horizontal position vector.
subtype padding   is std_logic_vector(HCNT_WIDTH - 9 downto 0);

-------------------------------------------------------------------------------
-- Constant definitions
--      

--
-- This group of constants defines the states of the EDH processor state
-- machine.
--
constant STATE_WIDTH : integer := 5;
subtype state_type is std_ulogic_vector(STATE_WIDTH - 1 downto 0);

constant S_WAIT :       state_type := "00000";
constant S_EAV1 :       state_type := "00001";
constant S_EAV2 :       state_type := "00010";
constant S_EAV3 :       state_type := "00011";
constant S_WHAT :       state_type := "00100";
constant S_SAV1 :       state_type := "00101";
constant S_SAV2 :       state_type := "00110";
constant S_SAV3 :       state_type := "00111";
constant S_SND1 :       state_type := "01000";
constant S_SND2 :       state_type := "01001";
constant S_ABRT :       state_type := "01010";
constant S_ADF1 :       state_type := "01011";
constant S_ADF2 :       state_type := "01100";
constant S_ADF3 :       state_type := "01101";
constant S_DID  :       state_type := "01110";
constant S_DBN  :       state_type := "01111";
constant S_DC   :       state_type := "10000";
constant S_UDW  :       state_type := "10001";
constant S_NC   :       state_type := "10010";




-------------------------------------------------------------------------------
-- Signal definitions
--
signal current_state:   state_type;     -- FSM current state
signal next_state:      state_type;     -- FSM next state
signal switching:       std_ulogic;     -- asserted if in switching keep-out area
signal done_switching:  std_ulogic;     -- used to generate the switching signal
signal space_avail:     std_ulogic;     -- asserted if space is available for packet
signal set_hanc:        std_ulogic;     -- sets the hanc signal
signal set_vanc:        std_ulogic;     -- clears the hanc signal (vanc = ~hanc)
signal hanc:            std_ulogic;     -- asserted during HANC space
signal ld_udw_cntr:     std_ulogic;     -- loads the UDW counter
signal udw_cntr_mux:    ubyte_type;     -- input MUX for the UDW counter
signal udw_cntr_eq_0:   std_ulogic;     -- asserted when udw_cntr_mux == 0
signal udw_cntr:        ubyte_type;     -- this is the UDW counter register
signal hanc_end:        hpos_type;      -- ending position of HANC space for current video std
signal vanc_end:        hpos_type;      -- ending position of VANC space for current video std
signal f1_edh_line:     vpos_type;      -- line in field 1 where EDH packet occurs
signal f2_edh_line:     vpos_type;      -- line in field 2 where EDH packet occurs
signal fx_edh_line:     vpos_type;      -- line in current field where EDH packet occurs
signal edh_line:        std_ulogic;     -- asserted when current line contains EDH packet
signal free_space:      hpos_type;      -- amount of free space remaining in ANC space
signal space_end:       hpos_type;      -- indicates the ending location of current ANC space
signal pkt_out:         std_ulogic;     -- asserted when ANC packet is being output
signal start_marker:    std_ulogic;     -- asserted when DID == 0x88
signal end_marker:      std_ulogic;     -- asserted when DID == 0x84
signal mod_pkt_size:    pktsize_type;   -- modified packet size value
signal pad_pkt_size:    hpos_type;      -- mod_pkt_size padded to HCNT width
signal mod_space_avail: std_ulogic;     -- when asserted, reduces the pkt_size by 3 for overwrites
signal overwrite:       std_ulogic;     -- asserted when an end marker packet is overwritten
signal abort_pkt_int :  std_ulogic;     -- internal version of abort_pkt
signal send_pkt_int :   std_ulogic;     -- internal version of send_pkt
 
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
    process(current_state, eav_next, sav_next, v_blank, switching, anc_next,
            pkt_rdy, vanc_pkt, hanc_pkt, space_avail, pkt_end,
            udw_cntr_eq_0, start_marker, overwrite)
    begin
        case current_state is
            when S_WAIT =>
                if (eav_next = '1') then
                    next_state <= S_EAV1;
                elsif (sav_next = '1') then
                    next_state <= S_SAV1;
                else
                    next_state <= S_WAIT;
                end if;
                    
            when S_EAV1 =>
                next_state <= S_EAV2;

            when S_EAV2 =>
                next_state <= S_EAV3;

            when S_EAV3 =>
                if (hanc_pkt = '0' or switching = '1') then
                    next_state <= S_WAIT;
                elsif (pkt_rdy = '1') then
                    next_state <= S_WHAT;
                else
                    next_state <= S_WAIT;
                end if;

            when S_WHAT =>
                if (anc_next = '1') then 
                    next_state <= S_ADF1;
                elsif (space_avail = '1') then
                    next_state <= S_SND1;
                else
                    next_state <= S_WAIT;
                end if;

            when S_SAV1 =>
                next_state <= S_SAV2;

            when S_SAV2 =>
                next_state <= S_SAV3;

            when S_SAV3 =>
                if (v_blank = '0' or vanc_pkt = '0') then 
                    next_state <= S_WAIT;
                elsif (pkt_rdy = '1') then
                    next_state <= S_WHAT;
                else
                    next_state <= S_WAIT;
                end if;

            when S_ADF1 =>  
                next_state <= S_ADF2;

            when S_ADF2 =>
                next_state <= S_ADF3;

            when S_ADF3 =>
                next_state <= S_DID;
            
            when S_DID =>
                if (start_marker = '1') then 
                    next_state <= S_NC;
                elsif (overwrite = '1') then
                    next_state <= S_SND2;
                else
                    next_state <= S_DBN;
                end if;

            when S_DBN =>
                next_state <= S_DC;

            when S_DC =>
                if (udw_cntr_eq_0 = '1' and pkt_rdy = '1') then
                    next_state <= S_WHAT;
                elsif (udw_cntr_eq_0 = '1' and pkt_rdy = '0') then
                    next_state <= S_WAIT;
                else
                    next_state <= S_UDW;
                end if;

            when S_UDW =>
                if (udw_cntr_eq_0 = '1' and pkt_rdy = '1') then
                    next_state <= S_WHAT;
                elsif (udw_cntr_eq_0 = '1' and pkt_rdy = '0') then
                    next_state <= S_WAIT;
                else
                    next_state <= S_UDW;
                end if;

            when S_SND1 =>
                if (switching = '1') then
                    next_state <= S_ABRT;
                else
                    next_state <= S_SND2;
                end if;

            when S_SND2 =>
                if (pkt_end = '1') then
                    next_state <= S_WAIT;
                else
                    next_state <= S_SND2;
                end if;

            when S_ABRT =>
                next_state <= S_WAIT;

            when S_NC =>
                if (anc_next = '1') then
                    next_state <= S_ADF1;
                elsif (eav_next = '1') then
                    next_state <= S_EAV1;
                elsif (sav_next = '1') then
                    next_state <= S_SAV1;
                else
                    next_state <= S_NC;
                end if;

            when others =>  
                next_state <= S_WAIT;
        end case;
    end process;
            
    --
    -- FSM: outputs
    --
    -- This block decodes the current state to generate the various outputs of 
    -- the FSM.
    --
    process(current_state, overwrite)
    begin
        -- Unless specifically assigned in the case statement, all FSM outputs
        -- default to the values here.
        send_pkt_int    <= '0';
        abort_pkt_int   <= '0';
        ld_udw_cntr     <= '0';
        set_hanc        <= '0';
        set_vanc        <= '0';
        mod_space_avail <= '0';
                                
        case current_state is
            when S_EAV1 =>
                set_hanc <= '1';

            when S_SAV1 =>
                set_vanc <= '1';

            when S_SND1 =>
                send_pkt_int <= '1';

            when S_ABRT =>
                abort_pkt_int <= '1';

            when S_ADF1 =>
                send_pkt_int <= '1';

            when S_DC =>
                ld_udw_cntr <= '1';

            when S_DID =>
                abort_pkt_int <= not overwrite;
                mod_space_avail <= '1';
            
            when others =>
        end case; 
    end process;

    --
    -- hanc flip-flop
    --
    -- The hanc flip-flop is asserted at the beginning of each HANC space and
    -- negated at the beginning of each VANC space by the FSM. The space_avail
    -- calculations use the hanc signal to determine the amount of available
    -- space left.
    --
    process(clk,rst)
    begin
        if (rst = '1') then
            hanc <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (set_hanc = '1') then
                    hanc <= '1';
                elsif (set_vanc = '1') then
                    hanc <= '0';
                end if;
            end if;
        end if;
    end process;
            
    --
    -- UDW counter
    --
    -- The FSM uses the UDW counter to find the end of each ANC packet in the
    -- input video stream. The FSM loads the UDW counter from the DC word of the
    -- packet. A MUX feeds the decrement circuit which, in turn, feeds the 
    -- counter register. The MUX selects between the DC word from the input 
    -- video stream or the current contents of the UDW counter register. The 
    -- output of the MUX is also connected to a comparator that asserts the 
    -- udw_cntr_eq_0 when either the DC word or the UDW counter, depending on 
    -- which the MUX selects, is equal to zero.
    --
    -- This structure allows the use of one decrementer and one comparator for 
    -- both the DC word and the UDW counter itself.
    --
    udw_cntr_mux <= vid_in(7 downto 0) when ld_udw_cntr = '1' else udw_cntr;

    udw_cntr_eq_0 <= '1' when udw_cntr_mux = UBYTE_ZERO else '0';

    process(clk, rst)
    begin
        if (rst = '1') then
            udw_cntr <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                udw_cntr <= ubyte_type(std_logic_vector(udw_cntr_mux) - 1);
            end if;
        end if;
    end process;

    --
    -- switching signal generator
    --
    -- The switching signal indicates when the current area is part of the keep-
    -- out area where ANC packets should not be inserted in order to avoid 
    -- interference from synchronous switching events. The keep-out area begins 
    -- with the SAV starting the active portion of the switching line and ends 
    -- with the SAV on the next line.
    --
    -- The video decoder provides a signal called switch_line that becomes 
    -- asserted on the first word of the active portion of a switching line. 
    -- However, the switch_line signal from the video_decoder doesn't negate 
    -- soon enough for the purposes of this module. So, a done_switching signal 
    -- is generated that negates the switching signal even when switch_line is 
    -- still asserted. The done_switching signal becomes asserted at the 
    -- beginning of an SAV symbol when switch_line is also asserted.
    --
    -- Note that the en_sync_switch input to the video decoder disables the
    -- switch_line output. Thus, if synchronous switching support is disabled in
    -- the video decoder, ANC packets may be inserted in the sync switching
    -- keep-outarea.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            done_switching <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (sav_next = '1') then
                    done_switching <= switch_line;
                end if;
            end if;
        end if;
    end process;

    switching <= switch_line and not done_switching;

    --
    -- space_avail signal generator
    --
    -- The space_avail signal is asserted if the space remaining in the current
    -- ANC space is sufficient to allow a packet of the size specified by 
    -- pkt_size to be inserted.
    --
    -- The calculation takes into account the current video standard as 
    -- specified by the std code. The sizes of the ANC spaces are dictated by 
    -- the video standard.
    --
    -- The hanc signal indicates whether the current ANC space is HANC or VANC. 
    --
    -- If the hanc signal is asserted, the calculation must also consider 
    -- whether the current line should have an EDH packet at the end of the HANC
    -- space. If so, the space available for insertion of a new packet must be 
    -- reduced by the size of an EDH packet.
    --
    -- Finally, when overwriting an end marker packet, the space available
    -- calculations are altered slightly. Since the decision about whether to 
    -- insert the packet during overwrites doesn't occur until the DID word, the 
    -- first three words of the packet have already been sent, so the packet 
    -- size is reduced by 3.
    --

    --
    -- This code determines the size of HANC and VANC space for the current 
    -- video standard. It also determines the the line number in each field 
    -- where the EDH packet occurs.
    --
    process(std)
    begin
        case std is
            when NTSC_422 =>
                hanc_end <= SAV_NTSC_422;
                vanc_end <= EAV_LOC_NTSC_422;
                f1_edh_line <= NTSC_FLD1_EDH_LINE;
                f2_edh_line <= NTSC_FLD2_EDH_LINE;

            when NTSC_422_WIDE =>
                hanc_end <= SAV_NTSC_422_WIDE;
                vanc_end <= EAV_LOC_NTSC_422_WIDE;
                f1_edh_line <= NTSC_FLD1_EDH_LINE;
                f2_edh_line <= NTSC_FLD2_EDH_LINE;

            when NTSC_4444 =>
                hanc_end <= SAV_NTSC_4444;
                vanc_end <= EAV_LOC_NTSC_4444;
                f1_edh_line <= NTSC_FLD1_EDH_LINE;
                f2_edh_line <= NTSC_FLD2_EDH_LINE;

            when PAL_422 =>
                hanc_end <= SAV_PAL_422;
                vanc_end <= EAV_LOC_PAL_422;
                f1_edh_line <= NTSC_FLD1_EDH_LINE;
                f2_edh_line <= NTSC_FLD2_EDH_LINE;

            when PAL_422_WIDE =>
                hanc_end <= SAV_PAL_422_WIDE;
                vanc_end <= EAV_LOC_PAL_422_WIDE;
                f1_edh_line <= NTSC_FLD1_EDH_LINE;
                f2_edh_line <= NTSC_FLD2_EDH_LINE;

            when PAL_4444 =>
                hanc_end <= SAV_PAL_4444;
                vanc_end <= EAV_LOC_PAL_4444;
                f1_edh_line <= NTSC_FLD1_EDH_LINE;
                f2_edh_line <= NTSC_FLD2_EDH_LINE;

            when others =>
                hanc_end <= SAV_NTSC_422;
                vanc_end <= EAV_LOC_NTSC_422;
                f1_edh_line <= NTSC_FLD1_EDH_LINE;
                f2_edh_line <= NTSC_FLD2_EDH_LINE;

        end case;
    end process;

    --
    -- This code block generates the space_end value. This value gives the 
    -- position of the end of the current ANC space. The space_end value is 
    -- assigned to the hanc_end or vanc_end values generated above. If an EDH 
    -- packet occurs on the current line and the HANC space is the current 
    -- space, the end of the HANC space is moved up by the size of the EDH 
    -- packet to prevent overwritting the EDH packet space.
    --
    process(edh_line, hanc, hanc_end, vanc_end)
    begin
        if (hanc = '1') then
            if (edh_line = '1') then
                space_end <= hanc_end - EDH_PKT_LENGTH;
            else
                space_end <= hanc_end;
            end if;
        else
            space_end <= vanc_end;
        end if;
    end process;

    --
    -- These two statements generate the edh_line signal. This signal is 
    -- asserted when the current line contains the locatino of an EDH packet.
    --
    fx_edh_line <= f1_edh_line when field = '1' else f2_edh_line;
    edh_line <= '1' when vert_count = fx_edh_line else '0';

    --
    -- These statements do the actual space_avail calculations. The free_space
    -- value always reflects the amount of space from the current position to 
    -- the end of the current ANC space. The mod_pkt_size value is normally 
    -- equal to the pkt_size, but will be three less if the FSM indicates that 
    -- an end-marker overwrite is potentially occuring (by sasserteing 
    -- mod_space_avail). The pad_pkt_size is the mod_pkt_size value padded with 
    -- zeros on the MSBs to make it the same width as the horizontal position 
    -- value. Finally, the space_avail signal is generated by comparing the 
    -- pad_pkt_size value to the free_space value.
    --
    process(pkt_size, mod_space_avail)
    begin
        if (mod_space_avail = '1') then
            mod_pkt_size <= pktsize_type(std_logic_vector(pkt_size) - 3);
        else
            mod_pkt_size <= pkt_size;
        end if;
    end process;

    free_space <= space_end - horz_count;
    pad_pkt_size <= hpos_type(("000" & mod_pkt_size));
    space_avail <= '1' when pad_pkt_size <= free_space else '0';


    --
    -- start_marker and end_marker decoders
    --
    -- These statements decode the DID word of ANC packets in the video stream 
    -- to determine if they are start-marker or end-marker packets.
    --
    start_marker <= '1' when vid_in(7 downto 0) = START_MARKER_DID else '0';
    end_marker <= '1' when vid_in(7 downto 0) = END_MARKER_DID else '0';

    --
    -- overwrite signal
    --
    -- This signal is asserted when the DID word of the ANC packet in the video
    -- stream is an end_marker and there is enough space available to insert the
    -- new packet (space_avail asserted). This signal tells the FSM to overwrite
    -- the end-marker packet.
    --
     overwrite <= end_marker and space_avail;
                     
    --
    -- video output MUX
    --
    -- This statement generates a MUX to drive the vid_out port. This MUX 
    -- selects between the input video stream (vid_in) or the new ANC packet 
    -- (anc_in). The MUX, by default, selects the vid_in data to send to 
    -- vid_out. However, when the FSM assertst the send_pkt signal (only 
    -- asserted for one state), the MUX outputs the anc_in data. The send_pkt 
    -- signal is captured in a flip-flop called pkt_out. The pkt_out signal 
    -- remains asserted until the end of the new ANC packet is reached, or the 
    -- packet send is aborted. Because the pkt_out signal is synchronous to the 
    -- clock signal, the MUX is controlled not only by pkt_out but also directly
    -- by the various signals that affect the pkt_out signal so that the MUX 
    -- will switch the cycle before pkt_out changes.
    -- 
    process(switching, abort_pkt_int, pkt_out, send_pkt_int, anc_in, vid_in)
    begin
        if (switching = '0' and abort_pkt_int = '0' and 
            (pkt_out = '1' or send_pkt_int = '1')) then
            vid_out <= anc_in;
        else
            vid_out <= vid_in;
        end if;
    end process;

    process(clk, rst)
    begin
        if (rst = '1') then
            pkt_out <= '0';
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (send_pkt_int = '1') then
                    pkt_out <= '1';
                elsif (pkt_end = '1' or switching = '1' or abort_pkt_int = '1') then
                    pkt_out <= '0';
                end if;
            end if;
        end if;
    end process;


    abort_pkt <= abort_pkt_int;
    send_pkt <= send_pkt_int;

end synth;