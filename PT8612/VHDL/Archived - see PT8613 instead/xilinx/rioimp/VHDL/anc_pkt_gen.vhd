-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: anc_pkt_gen.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2005-01-18 10:40:55-07 $
-- /___/   /\    Date Created: Jan 5, 2005
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: anc_pkt_gen.vhd,rcs $
-- Revision 1.0  2005-01-18 10:40:55-07  jsnow
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
-- This module formats an ANC packet from raw data written into the module. Once
-- all the raw data has been written, as indicated by the pkt_rdy_in input being
-- asserted, the module will assert the pkt_rdy_out signal to the anc_insert
-- module and place the first word of the ADF on the anc_out port.
-- 
-- When the anc_insert module is ready to insert the packet, it will assert the
-- send_pkt signal. The anc_pkt_gen module responds by sequentially placing each
-- word of the ANC packet on the anc_out port for one clock cycle. It also 
-- asserts the pkt_end signal when the last word of the packet (CS) is on the 
-- anc_out port.
-- 
-- The anc_insert module sometimes needs to abort the insertion of a packet 
-- after it has asserted the send_pkt signal. This is indicated by the abort_pkt
-- input. This signal may be asserted up through the DBN/SDID word of the 
-- packet, but not later.
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
-- It should be noted that it is not possible to simply insert another copy of a
-- packet by reasserting pkt_rdy_in. This is because the DC word of the packet
-- is destroyed in the process of transmitting the packet to the anc_insert
-- module. At the very least, the DC word must be re-written to the anc_pkt_gen
-- module before pkt_rdy_in is reasserted.
-- 
-- This module calculates the size of the current packet by adding the number of
-- overhead words to the size of the payload. The total size of the packet is
-- sent out through the pkt_size port. This information is used by the 
-- anc_insert module.
-- 
-- The UDW words of the new packet are written into a 2560-bit (256 words x 
-- 10-bits) RAM and stored until they are sent to the anc_insert module. This 
-- RAM may be implemented in the FPGA as either distributed RAM or as block RAM.
-- The module contains code that will allows XST to infer either a single-port
-- distributed RAM or a dual-port block RAM. A `define statement defines a
-- variable called UDW_BLOCK_RAM. If this variable is defined, then block RAM
-- will be inferred (if the FPGA has block RAM). If this variable is not
-- defined, distributed RAM is inferred. If the variable is defined, but the
-- FPGA does not have block RAM, a dual-port distributed RAM will be inferred.
-- This dual-port RAM will consume twice as many LUTs as the single-port RAM and
-- is therefore rather wasteful.
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

use work.anc_edh_pkg.all;

entity anc_pkt_gen is
    port (
        -- inputs
        clk:            in  std_ulogic;  -- clock input
        ce:             in  std_ulogic;  -- clock enable
        rst:            in  std_ulogic;  -- async reset input
        pkt_rdy_in:     in  std_ulogic;  -- indicates packet has been loaded into registers
        calc_udw_parity:in  std_ulogic;  -- indicates that module should calculate parity on UDW
        send_pkt:       in  std_ulogic;  -- causes module to send packet
        abort_pkt:      in  std_ulogic;  -- will abort the sending of a packet

        -- input packet RAM/register write controls and data
        data_in:        in  video_type;  -- input data bus
        ld_did:         in  std_ulogic;  -- loads DID register
        ld_dbn:         in  std_ulogic;  -- loads SDID/DBN register
        ld_dc:          in  std_ulogic;  -- loads DC register
        ld_udw:         in  std_ulogic;  -- loads the UDW RAM
        udw_wr_adr:     in  udwadr_type; -- UDW RAM write address
    
        -- outputs
        anc_out:        out video_type;  -- ANC packet data output
        pkt_size:       out pktsize_type;-- total number of words in packet
        pkt_in_empty:   out std_ulogic;  -- module is ready for input packet to be loaded
        pkt_end:        out std_ulogic;  -- output indicating last word of packet is being sent
        pkt_rdy_out:    out std_ulogic); -- output indicating module is ready to send packet
end;

architecture synth of anc_pkt_gen is

--
-- Define a datatype for the UDW memory array
--
type udw_ram_type is array (UDW_MEM_SIZE - 1 downto 0) of video_type; 

--
-- This group of constants defines the states of the EDH processor state
-- machine.
--
constant STATE_WIDTH :  integer := 4;
subtype state_type is std_ulogic_vector(STATE_WIDTH - 1 downto 0);

constant S_WAIT :       state_type := "0000";
constant S_ADF1 :       state_type := "0001";
constant S_ADF2 :       state_type := "0010";
constant S_DID  :       state_type := "0011";
constant S_DBN  :       state_type := "0100";
constant S_DC   :       state_type := "0101";
constant S_UDW  :       state_type := "0110";
constant S_CHK  :       state_type := "0111";
constant S_END  :       state_type := "1000";
constant S_DONE :       state_type := "1001";
constant S_ABRT :       state_type := "1010";

--
-- This group of constants specifies the select values for the various MUXes
-- in the module.
--
constant PMUXSEL_WIDTH  : integer := 2; 
subtype  pmuxsel_type is std_ulogic_vector(PMUXSEL_WIDTH - 1 downto 0);

constant PMUXSEL_DID    : pmuxsel_type := "00";
constant PMUXSEL_DBN    : pmuxsel_type := "01";
constant PMUXSEL_DC     : pmuxsel_type := "10";
constant PMUXSEL_UDW    : pmuxsel_type := "11";

constant OMUXSEL_WIDTH  : integer := 2;
subtype  omuxsel_type is std_ulogic_vector(OMUXSEL_WIDTH - 1 downto 0);

constant OMUXSEL_ADF0   : omuxsel_type := "00";
constant OMUXSEL_ADF1   : omuxsel_type := "01";
constant OMUXSEL_DATA   : omuxsel_type := "10";
constant OMUXSEL_CHK    : omuxsel_type := "11";     

constant DMUXSEL_PGEN   : std_ulogic := '0';
constant DMUXSEL_UDW    : std_ulogic := '1';  

--
-- These constants define the Ancillary Data Flag word values
--
constant ANC_ADF0       : video_type := (others => '0');
constant ANC_ADF1       : video_type := (others => '1'); 

-------------------------------------------------------------------------------
-- Signal definitions
--
signal current_state :  state_type;     -- FSM current state
signal next_state :     state_type;     -- FSM next state
signal parity :         std_ulogic;     -- used to generate parity_err signal
signal pword :          video_type;     -- data word with parity calculated
signal checksum :       cksum_type;     -- checksum generator for ANC packet
signal clr_checksum :   std_ulogic;     -- asserted to clear the checksum
signal udw_rd_cntr :    udwadr_type;    -- UDW read counter
signal clr_udw_rd_cntr: std_ulogic;     -- clears the UDW read address
signal inc_udw_rd_cntr: std_ulogic;     -- increments the UDW read address
signal dc_eq_0 :        std_ulogic;     -- asserted when DC register equals 0
signal dec_dc :         std_ulogic;     -- causes DC register to decrement
signal pmux :           ubyte_type;     -- output of the parity gen input MUX
signal dmux :           video_type;     -- selects between parity gen out or UDW RAM out
signal omux :           video_type;     -- output data MUX
signal pmux_sel :       pmuxsel_type;   -- parity MUX select
signal dmux_sel :       std_ulogic;     -- data MUX select
signal omux_sel :       omuxsel_type;   -- output MUX select
signal did :            ubyte_type;     -- DID register
signal dbn :            ubyte_type;     -- DBN/SDID register
signal dc :             ubyte_type;     -- DC register/counter
signal udw_ram :        udw_ram_type;   -- UDW RAM
signal udw_wr_en :      std_ulogic;     -- write enable signal for UDW RAM
signal ld_out_reg :     std_ulogic;     -- loads the anc_out reg when asserted
signal pkt_in_empty_x : std_ulogic;     -- internal version of pkt_in_empty output
signal udw :            video_type;     -- UDW RAM output
 
--------------------------------------------------------------------------------
-- If using block RAM, use this signal definition.
--
-- signal udw_rd_adr :      udwadr_type;    -- RAM read address reg for block RAM

--------------------------------------------------------------------------------
-- If using distributed RAM, use this signal definition.
--
signal udw_adr :        udwadr_type;    -- MUXed address into single port RAM


--
-- XST 4.1 was trying to use one-hot encoding for pmux_sel and omux_sel. These
-- statements force XST to use the constant values as given.
--
attribute fsm_encoding : string;
attribute fsm_encoding of pmux_sel : signal is "user";
attribute fsm_encoding of omux_sel : signal is "user";

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
    process(current_state, pkt_rdy_in, send_pkt, dc_eq_0, abort_pkt)
    begin
        case current_state is
            when S_WAIT =>
                if (pkt_rdy_in = '1') then
                    next_state <= S_ADF1;
                else
                    next_state <= S_WAIT;
                end if;

            when S_ADF1 =>
                if (send_pkt = '1') then
                    next_state <= S_ADF2;
                else
                    next_state <= S_ADF1;
                end if;

            when S_ADF2 =>
                if (abort_pkt = '1') then
                    next_state <= S_ABRT;
                else
                    next_state <= S_DID;
                end if;
            
            when S_DID =>
                if (abort_pkt = '1') then
                    next_state <= S_ABRT;
                else
                    next_state <= S_DBN;
                end if;

            when S_DBN =>
                if (abort_pkt = '1') then
                    next_state <= S_ABRT;
                else
                    next_state <= S_DC;
                end if;

            when S_DC =>
                if (dc_eq_0 = '1') then
                    next_state <= S_CHK;
                else
                    next_state <= S_UDW;
                end if;

            when S_UDW =>
                if (dc_eq_0 = '1') then
                    next_state <= S_CHK;
                else
                    next_state <= S_UDW;
                end if;

            when S_CHK =>
                next_state <= S_END;

            when S_END =>
                next_state <= S_DONE;

            when S_DONE =>
                if (pkt_rdy_in = '1') then
                    next_state <= S_DONE;
                else
                    next_state <= S_WAIT;
                end if;

            when S_ABRT =>
                next_state <= S_ADF1;

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
    process(current_state, calc_udw_parity, send_pkt)
    begin
        -- Unless specifically assigned in the case statement, all FSM outputs
        -- default to the values here..
        pkt_rdy_out     <= '0';
        pmux_sel        <= PMUXSEL_DID;
        dmux_sel        <= DMUXSEL_PGEN;
        omux_sel        <= OMUXSEL_DATA;
        clr_checksum    <= '0';
        clr_udw_rd_cntr <= '0';
        inc_udw_rd_cntr <= '0';
        dec_dc          <= '0';
        pkt_end         <= '0';
        pkt_in_empty_x  <= '0';
        ld_out_reg      <= '0';
        
        case current_state is
            when S_WAIT => 
                pkt_in_empty_x <= '1';
                omux_sel <= OMUXSEL_ADF0;
                ld_out_reg <= '1';

            when S_ADF1 =>
                omux_sel <= OMUXSEL_ADF1;
                pkt_rdy_out <= '1';
                if (send_pkt = '1') then
                    ld_out_reg <= '1';
                else
                    ld_out_reg <= '0';
                end if; 

            when S_ADF2 =>
                omux_sel <= OMUXSEL_ADF1;
                ld_out_reg <= '1';
                clr_checksum <= '1';

            when S_DID =>
                pmux_sel <= PMUXSEL_DID;
                dmux_sel <= DMUXSEL_PGEN;
                omux_sel <= OMUXSEL_DATA;
                ld_out_reg <= '1';

            when S_DBN =>
                pmux_sel <= PMUXSEL_DBN;
                dmux_sel <= DMUXSEL_PGEN;
                omux_sel <= OMUXSEL_DATA;
                ld_out_reg <= '1';
                clr_udw_rd_cntr <= '1';  

            when S_DC =>
                pmux_sel <= PMUXSEL_DC;
                dmux_sel <= DMUXSEL_PGEN;
                omux_sel <= OMUXSEL_DATA;
                inc_udw_rd_cntr <= '1';
                dec_dc <= '1';
                ld_out_reg <= '1';
            
            when S_UDW =>
                pmux_sel <= PMUXSEL_UDW;
                if (calc_udw_parity = '1') then
                    dmux_sel <= DMUXSEL_PGEN;
                else
                    dmux_sel <= DMUXSEL_UDW;
                end if;
                omux_sel <= OMUXSEL_DATA;
                inc_udw_rd_cntr <= '1';
                dec_dc <= '1';
                ld_out_reg <= '1';
                
            when S_CHK =>
                omux_sel <= OMUXSEL_CHK;
                ld_out_reg <= '1';

            when S_END =>
                pkt_end <= '1';

            when S_ABRT =>
                omux_sel <= OMUXSEL_ADF0;
                ld_out_reg <= '1';

            when others =>
        end case;
    end process;
                                
    --
    -- pmux
    --
    -- The pmux selects the input to the parity generator. There are four 
    -- inputs: DID register, DBN register, DC register, and UDW RAM.
    --
    process(pmux_sel, udw, did, dbn, dc)
    begin
        case pmux_sel is
            when PMUXSEL_DID => pmux <= did;
            when PMUXSEL_DBN => pmux <= dbn;
            when PMUXSEL_DC  => pmux <= dc;
            when others      => pmux <= udw(7 downto 0);
        end case;
    end process;

    --
    -- dmux
    --
    -- The dmux selects the data word to be sent to the omux and the checksum 
    -- generator. There are two inputs: the UDW RAM or the output of the pmux 
    -- via the parity geneator.
    --
    process(dmux_sel, pword, udw)
    begin
        if (dmux_sel = DMUXSEL_UDW) then
            dmux <= udw;
        else
            dmux <= pword;
        end if;
    end process;

    --
    -- omux
    --
    -- The omux selects the value to send to the output of the module. There are
    -- four inputs: all zeros (first ADF word), all ones (second & third ADF
    -- words), the output of the dmux, or the checksum generator.
    --
    process(omux_sel, dmux, checksum)
    begin
        case omux_sel is
            when OMUXSEL_ADF0 => omux <= ANC_ADF0;
            when OMUXSEL_ADF1 => omux <= ANC_ADF1;
            when OMUXSEL_DATA => omux <= dmux;
            when others       => omux <= (not checksum(8) & checksum);
        end case;
    end process;

    --
    -- parity bit generation
    --
    -- This code calculates the parity of bits 7:0 of the data word. The parity
    -- bit is inserted at bit 8 of the data word and the inverse of the parity
    -- bit is inserted at bit 9.
    --
    parity <= pmux(7) xor pmux(6) xor pmux(5) xor pmux(4) xor pmux(3) xor
              pmux(2) xor pmux(1) xor pmux(0);

    pword <= (not parity & parity & pmux(7 downto 0));

    --
    -- checksum calculator
    --
    -- This code generates a checksum for the ANC packet. The checksum is 
    -- cleared to zero prior to beginning the checksum calculation by the FSM 
    -- asserting the clr_checksum signal. The data word is added to the current  
    -- checksum continuously. The checksum is a 9-bit value and is computed by  
    -- summing all but the MSB of the data word with the current checksum value  
    -- and ignoring any carry bits.
    --
    process(clk,rst)
    begin
        if (rst = '1') then
            checksum <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (clr_checksum = '1') then
                    checksum <= (others => '0');
                else
                    checksum <= std_ulogic_vector(std_logic_vector(checksum) + 
                                                  std_logic_vector(dmux(8 downto 0)));
                end if;
            end if;
        end if;
    end process;

    --
    -- DID register
    --
    -- This register holds the DID word for the ANC packet. The DID register 
    -- loads from the data_in bus when ld_did is asserted. Only eight bits are 
    -- stored in the DID register. The module generates the correct parity bits 
    -- for the two MSBs of the DID word when inserted in the ANC packet.
    --
    process(clk,rst)
    begin
        if (rst = '1') then
            did <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (ld_did = '1' and pkt_in_empty_x = '1') then
                    did <= data_in(7 downto 0);
                end if;
            end if;
        end if;
    end process;

    --
    -- DBN/SDID register
    --
    -- This register holds the DBN or SDID word for the ANC packet. The DBN 
    -- register loads from the data_in bus when the ld_dbn signal is asserted. 
    -- Only eight bits are stored in the register. The module generates the 
    -- correct parity bits for the two MSBs of the DBN/SDID word when inserted
    -- into the ANC packet.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            dbn <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (ld_dbn = '1' and pkt_in_empty_x = '1') then
                    dbn <= data_in(7 downto 0);
                end if;
            end if;
        end if;
    end process;
                
    --
    -- DC register/counter
    --
    -- This register holds the DC (data count) word for the ANC packet. It loads
    -- from the data_in bus when the ld_dc signal is asserted. Only eight bits 
    -- are stored in the register. The DC register also acts as a down counter 
    -- as UDW words are output.
    --
    process(clk,rst)
    begin
        if (rst = '1') then
            dc <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ld_dc = '1' and pkt_in_empty_x = '1') then
                dc <= data_in(7 downto 0);
            elsif (dec_dc = '1') then
                dc <= std_ulogic_vector(std_logic_vector(dc) - 1);
            end if;
        end if;
    end process;
            
    dc_eq_0 <= '1' when dc = UBYTE_ZERO else '0';

    --
    -- UDW RAM
    --  
    -- The UDW RAM holds the user data words. Data is written into the RAM by
    -- placing the data on the data_in bus, the address on the udw_wr_adr bus, 
    -- and asserting the ld_udw signal. All 10-bits of the data_in bus are 
    -- written to the UDW RAM. However, if the calc_udw_parity signal is 
    -- asserted as the packet is being generated, the two MSBs of the UDW RAM  
    -- are ignored and instead, the parity generator is used to generate parity 
    -- bits for the two MSBs of the UDW.
    --
    -- Code is given to infer either a block RAM or a single-port distributed
    -- RAM. Separate code is given for the distributed RAM so that a single-port 
    -- RAM will be inferred rather than a larger dual-port distributed RAM. In
    -- order to keep the control timing of both cases identical, a register for  
    -- the read address is inserted for the distributed RAM case. The block RAM 
    -- case requires a read address register in order to infer the block RAM.
    --

    ----------------------------------------------------------------------------
    -- Infer dual-port block RAM
    --
    -- Uncomment the following code to infer block RAM for the UDW RAM

--  	udw_wr_en <= ce and ld_udw and pkt_in_empty_x;
--   
--  	process(clk)
--  	begin
--      	if (clk'event and clk = '1') then
--          	if (udw_wr_en = '1') then
--              udw_ram(conv_integer(udw_wr_adr)) <= data_in;
--         	 end if;
--          	udw_rd_adr <= udw_rd_cntr;
--      	end if;
--  	end process;
--   
--  	udw <= udw_ram(conv_integer(udw_rd_adr));

    ----------------------------------------------------------------------------

    ----------------------------------------------------------------------------
    -- Infer single-port distributed RAM

    udw_wr_en <= ce and ld_udw and pkt_in_empty_x;
    udw_adr <= udw_wr_adr when ((ld_udw and pkt_in_empty_x) = '1') 
               else udw_rd_cntr;
      
    process(clk)
    begin
        if (clk'event and clk = '1') then
            if (udw_wr_en = '1') then
                udw_ram(conv_integer(udw_adr)) <= data_in;
            end if;
            udw <= udw_ram(conv_integer(udw_adr));
        end if;
    end process;

    --------------------------------------------------------------------------------

    --
    -- UDW read counter
    --
    -- This eight-bit up counter generates read addresses into the UDW RAM to 
    -- read out the UDW words in correct order.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            udw_rd_cntr <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (clr_udw_rd_cntr = '1') then
                    udw_rd_cntr <= (others => '0');
                elsif (inc_udw_rd_cntr = '1') then
                    udw_rd_cntr <= udw_rd_cntr + 1;
                end if;
            end if;
        end if;
    end process;

    --
    -- anc_out
    --
    -- This is the output register of the module. It loads from the omux 
    -- whenever the ld_out_reg signal is asserted.
    --
    process(clk,rst)
    begin
        if (rst = '1') then
            anc_out <= (others => '0');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (ld_out_reg = '1') then
                    anc_out <= omux;
                end if;
            end if;
        end if;
    end process;

    --
    -- pkt_size
    --
    -- The pkt_size output is set to seven more than the size of they ANC 
    -- packet's payload (DC word).
    --
    pkt_size <= std_ulogic_vector(std_logic_vector(('0' & dc)) + 7);
        
    pkt_in_empty <= pkt_in_empty_x;
                    
end synth;