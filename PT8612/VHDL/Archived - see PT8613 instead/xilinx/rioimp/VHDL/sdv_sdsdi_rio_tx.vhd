--------------------------------------------------------------------------------
-- sdv_sdsdi_tx.vhd
--
-- SD-SDI Tx reference design for the Xilinx SDV demo board
--
--
--                  Author: John F. Snow
--                  Staff Applications Engineer
--
--                  Video Applications
--                  Advanced Products Group
--                  Xilinx, Inc.
--
--                  Copyright (c) 2004 Xilinx, Inc.
--                  All rights reserved
--
--                  Date:   Mar 2, 2004
--
--                  RESTRICTED RIGHTS LEGEND
--
--      This software has not been published by the author, and 
--      has been disclosed to others for the purpose of enhancing 
--      and promoting design productivity in Xilinx products.
--
--      Therefore use, duplication or disclosure, now and in the 
--      future should give consideration to the productivity 
--      enhancements afforded the user of this code by the author's 
--      efforts.  Thank you for using our products !
--
-- Disclaimer:  THESE DESIGNS ARE PROVIDED "AS IS" WITH NO WARRANTY 
--              WHATSOEVER AND XILINX SPECIFICALLY DISCLAIMS ANY 
--              IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
--              A PARTICULAR PURPOSE, OR AGAINST INFRINGEMENT.
--
-- Revision:
--          Mar 2, 2004     XAPP683 release 1.00
--                                                          
-- Other modules instanced in this design:
--      vidgen:             Video pattern generator from XAPP248
--      edh_processor:      SDI EDH processor from XAPP299
--      smpte_encoder:      SDI encoder
--      hdsdi_rio_refclk:   RocketIO module wrapper
--
--------------------------------------------------------------------------------
--
-- Description of module:
-- 
-- This reference design implements a SD-SDI transmitter for the Xilinx 
-- SDV demo board using a RocketIO transceiver overclocked at 4X the SD-SDI bit
-- rate of 270 Mbps. The reference design includes a video pattern generator as 
-- the video source 
-- 
-- The DIP switches on the SDV demo board are used to select options as follows:
-- 
-- DIP Switch 1 selects which video pattern is transmitted
-- 0 = EG-1 color bars, 1 = SD-SDI checkfield
-- 
-- Pushbutton SW2 is a global reset to the FPGA, wired to the GSR global reset net..
-- 
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

use work.anc_edh_pkg.all;
use work.hdsdi_pkg.all;

library unisim; 
use unisim.vcomponents.all; 

entity sdv_sdsdi_rio_tx is
    port (
        clk_54M_p:          in  std_logic;                      -- 54.00 MHz XO
        clk_54M_n:          in  std_logic;
        push_button1:       in  std_logic;                      -- resets all logic
        dip_switches:       in  std_logic_vector(7 downto 0);   -- selects user options
        mr_rx_rxp:          in  std_logic;                      -- RocketIO receiver input -- not used but must be connected
        mr_rx_rxn:          in  std_logic;    
        mr_tx1_txp:         out std_logic;                      -- RocketIO transmitter output
        mr_tx1_txn:         out std_logic;
        mr_tx1_slewrate:    out std_logic;                      -- selects the slew rate of the GS1528 cable driver
        mr_tx2_slewrate:    out std_logic;                      -- Tx2 port isn't used, but drive slewrate anyway

    -- LEDs
        mr_tx2_led:         out std_logic;    
        mr_tx1_led:         out std_logic;    
        mr_rx_led:          out std_logic;    
        asi_tx_led:         out std_logic;    
        sdi_rx_led:         out std_logic;    
        sdi_tx_led:         out std_logic;    
        mr_sync_led:        out std_logic;    
        mr_hd_led:          out std_logic;    
        mr_rate_led:        out std_logic;    
        sdi_sync_led:       out std_logic;    
        sdi_rate_led:       out std_logic;    
        mode_mr_led:        out std_logic;    
        mode_sdi_led:       out std_logic;    
        mode_asi_led:       out std_logic;    

    -- Outputs unused in this demo but required to be driven for SDV Demo board
            
        -- HD-SDI VCXO 
        hdsdi_vcxo_up:      out std_logic;
        hdsdi_vcxo_down:    out std_logic;

        -- SDI 270MHz VCO
        sdi_vco_up:         out std_logic;
        sdi_vco_down:       out std_logic;

        -- SDI 27MHz VCXO
        vcxo_27M_up:        out std_logic;
        vcxo_27M_down:      out std_logic;
        vcxo_27M_sel:       out std_logic;

        -- ICS660 PLL
        ics660_s:           out std_logic_vector(3 downto 0);
        ics660_x1:          out std_logic;

        -- CY22394 PLL
        cypll_oe:           out std_logic;
        cypll_s2:           out std_logic;
        cypll_sclk:         out std_logic;
        cypll_sdat:         out std_logic;

        -- ICS8745 PLL
        ics8745_clkout_p:   out std_logic;
        ics8745_clkout_n:   out std_logic;
        ics8745_sel:        out std_logic_vector(3 downto 0);
        ics8745_pll_sel:    out std_logic;
        ics8745_mr:         out std_logic;

        -- SDI-Tx Port
        sdi_tx_dout_p:      out std_logic;
        sdi_tx_dout_n:      out std_logic;

        -- ASI-Tx Port
        asi_tx_p:           out std_logic;
        asi_tx_n:           out std_logic;

        -- RS-232 Port
        rs232_txd:          out std_logic;
        rs232_rts:          out std_logic
    );
end sdv_sdsdi_rio_tx;

architecture synth of sdv_sdsdi_rio_tx is

-------------------------------------------------------------------------------
-- Signal definitions
--
 
signal clk_54M :        std_logic;                      -- 54.00MHz clock after IOB
signal txusrclk :       std_logic;                      -- TXUSRCLK for transmit RocketIO
signal vidgen_out :     video_type;                     -- output of SD video generator
signal edh_out :        video_type;                     -- SD video out of EDHh processor
signal edh_out_std :    hd_video_type;                  -- edh_out converted to hd_video_type
signal sd_ce :          std_logic;                      -- SD clock enable flip-flop
signal i_scram_q :      std_logic_vector(8 downto 0);   -- intermediat encoder data
signal enc_out :        hd_video_type;                  -- encoder output
signal dipsw :          std_logic_vector(7 downto 0);   -- DIP switches after IOBs
signal txdata :         hd_vid20_type;                  -- encoded data to RocketIO transmitter
signal sd_data :        std_logic_vector(39 downto 0);  -- 40-bit vector from bit replication
signal pb1 :            std_logic;                      -- pushbutton 1
signal n_pb1 :          std_logic;                      -- inverse of pb1
signal VCC :            std_logic;
signal GND :            std_logic;
signal GND2 :           std_logic_vector(1 downto 0);
signal edh_flags_gnd :  edh_allflg_type;

component vidgen
    port(
        -- signals for pattern generator A
        clk_a:      in  std_ulogic;                                 -- clock input
        rst_a:      in  std_ulogic;                                 -- async reset input
        ce_a:       in  std_ulogic;                                 -- clock enable input
        pattern_a:  in  std_ulogic;                                 -- pattern select
        q_a:        out std_ulogic_vector(9 downto 0);              -- video output
        h_sync_a:   out std_ulogic;                                 -- h sync out
        v_sync_a:   out std_ulogic;                                 -- v sync out
        field_a:    out std_ulogic;                                 -- field out

        -- signals for pattern generator B 
        clk_b:      in  std_ulogic;                                 -- clock input
        rst_b:      in  std_ulogic;                                 -- async reset input
        ce_b:       in  std_ulogic;                                 -- clock enable input
        pattern_b:  in  std_ulogic;                                 -- pattern select
        q_b:        out std_ulogic_vector(9 downto 0);              -- video output
        h_sync_b:   out std_ulogic;                                 -- h sync out
        v_sync_b:   out std_ulogic;                                 -- v sync out
        field_b:    out std_ulogic                                  -- field out
        
    );
end component;

component edh_processor
    port(
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
        edh_packet:     out std_ulogic);        -- asserted during all words of a generated EDH packet
end component;

component smpte_encoder is
    port (
        clk:        in  std_logic;      -- word rate clock (74.25 MHz)
        rst:        in  std_logic;      -- async reset
        ce:         in  std_logic;      -- clock enable
        nrzi:       in  std_logic;      -- 1 enables NRZ-to-NRZI conversion
        scram:      in  std_logic;      -- 1 enables SDI scrambler
        d:          in  hd_video_type;  -- input data port
        p_scram:    in                  -- previously scrambled data input
                        std_logic_vector(8 downto 0);
        p_nrzi:     in  std_logic;      -- MSB of previously converted NRZI word
        q:          out hd_video_type;  -- output data port
        i_scram:    out                 -- intermediate scrambled data output
                        std_logic_vector(8 downto 0);
        i_scram_q:  out                 -- registered intermediate scrambled data
                        std_logic_vector(8 downto 0);
        i_nrzi:     out std_logic);     -- intermediate nrzi data output
end component;

component hdsdi_rio_refclk
    port (
        brefclk:        in  std_logic;                  -- BREFCLK
        brefclk2:       in  std_logic;                  -- BREFCLK2
        refclk:         in  std_logic;                  -- REFCLK
        refclk2:        in  std_logic;                  -- REFCLK2
        refclk_sel:     in  std_logic;                  -- selects between BREFCLK and BREFCLK2
        rst:            in  std_logic;                  -- reset signal
        loopback_en:    in  std_logic;                  -- loopback enable
        loopback_mode:  in  std_logic;                  -- 0 is serial, 1 is parallel mode
        txinhibit:      in  std_logic;                  -- inhibits transmitter when 1
        txdata:         in  hd_vid20_type;              -- data to be transmitted
        txusrclk:       in  std_logic;                  -- transmitter usr clock
        txusrclk2:      in  std_logic;                      -- transmitter usr clock 2
        rxusrclk:       in  std_logic;                  -- receiver usr clock
        rxusrclk2:      in  std_logic;                  -- receiver usr clock 2
        dcm_locked:     in  std_logic;                  -- DCM for RXUSRCLKs is locked
        rxp:            in  std_logic;                  -- serial input - true
        rxn:            in  std_logic;                  -- serial input - complement
        rxdata:         out hd_vid20_type;              -- received data from RocketIO receiver
        rxrecclk:       out std_logic;                  -- clock recovered by RocketIO receiver
        txp:            out std_logic;                  -- serial output - true
        txn:            out std_logic                   -- serial output - complement
    );                             
end component;

attribute iostandard : string;
attribute iostandard of XO54   :   label is "LVDSEXT_25_DT";

begin

    VCC <= '1';
    GND <= '0';
    GND2 <= "00";
    edh_flags_gnd <= (others => '0');

    --
    -- Instantiate the startup block
    --
    n_pb1 <= not pb1;

    STARTV2 : STARTUP_VIRTEX2
        port map (
            CLK                 => GND,
            GSR                 => n_pb1,
            GTS                 => GND
        );


    -------------------------------------------------------------------------------
    -- I/O Buffers
    --

    --
    -- Push button switche
    --
    -- PB1 is global reset
    --
    PB1BUF : IBUF_LVCMOS25
        port map (
            I       => push_button1,
            O       => pb1
        );

    --
    -- DIP switches
    --
    SW0BUF : IBUF_LVCMOS25
        port map (
            I       => dip_switches(0),
            O       => dipsw(0)
        );

    SW1BUF : IBUF_LVCMOS25
        port map (
            I       => dip_switches(1),
            O       => dipsw(1)
        );

    SW2BUF : IBUF_LVCMOS25
        port map (
            I       => dip_switches(2),
            O       => dipsw(2)
        );

    SW3BUF : IBUF_LVCMOS25
        port map (
            I       => dip_switches(3),
            O       => dipsw(3)
        );

    SW4BUF : IBUF_LVCMOS25
        port map (
            I       => dip_switches(4),
            O       => dipsw(4)
        );

    SW5BUF : IBUF_LVCMOS25
        port map (
            I       => dip_switches(5),
            O       => dipsw(5)
        );

    SW6BUF : IBUF_LVCMOS25
        port map (
            I       => dip_switches(6),
            O       => dipsw(6)
        );

    SW7BUF : IBUF_LVCMOS25
        port map (
            I       => dip_switches(7),
            O       => dipsw(7)
        );

    --
    -- LED Buffers
    --
    LED0BUF : OBUF_LVCMOS25
        port map (
            I       => VCC,
            O       => mr_tx2_led
        );

    LED1BUF : OBUF_LVCMOS25
        port map (
            I       => GND,
            O       => mr_tx1_led
        );

    LED2BUF : OBUF_LVCMOS25
        port map (
            I       => VCC,
            O       => mr_rx_led
        );

    LED3BUF : OBUF_LVCMOS25
        port map (
            I       => VCC,
            O       => asi_tx_led
        );

    LED4BUF : OBUF_LVCMOS25
        port map (
            I       => VCC,
            O       => sdi_rx_led
        );

    LED5BUF : OBUF_LVCMOS25
        port map (
            I       => VCC,
            O       => sdi_tx_led
        );

    LED6BUF : OBUF_LVCMOS25
        port map (
            I       => VCC,
            O       => mr_sync_led
        );

    LED7BUF : OBUF_LVCMOS25
        port map (
            I       => VCC,
            O       => mr_hd_led
        );

    LED8BUF : OBUF_LVCMOS25
        port map (
            I       => VCC,
            O       => mr_rate_led
        );

    LED9BUF : OBUF_LVCMOS25
        port map (
            I       => VCC,
            O       => sdi_sync_led
        );

    LED10BUF : OBUF_LVCMOS25
        port map (
            I       => VCC,
            O       => sdi_rate_led
        );

    LED11BUF : OBUF_LVCMOS25
        port map (
            I       => VCC,
            O       => mode_mr_led
        );

    LED12BUF : OBUF_LVCMOS25
        port map (
            I       => VCC,
            O       => mode_sdi_led
        );

    LED13BUF : OBUF_LVCMOS25
        port map (
            I       => VCC,
            O       => mode_asi_led
        );

    --
    -- GS1528 Cable Driver slew rate control output
    --
    -- The GS1528 cable driver has a slew rate control input that tells it whether
    -- to compliant with SDI or HD-SDI slew rates. In this demo, we are always
    -- transmitting HD-SDI, so we always force this signal low.
    --
    MRTX2_SLEW : OBUF_LVDCI_33
        port map (
            I       => GND,
            O       => mr_tx2_slewrate
        );

    MRTX1_SLEW : OBUF_LVDCI_33
        port map (
            I       => VCC,
            O       => mr_tx1_slewrate
        );

    --
    -- 54MHz XO input buffer
    --
    XO54 : IBUFGDS
        port map (
            O       => clk_54M,
            I       => clk_54M_p,
            IB      => clk_54M_n
        );

    --
    -- BUFGMUX to select the correct txusrclk and buffer it globally
    --

    BFG1 : BUFG
        port map (
            I       => clk_54M,
            O       => txusrclk);

    --
    -- sd_ce signal generation
    --
    -- This signal is used to enable the SD video pattern generator, EDH processor,
    -- and the SDI encoder every other clock cycle. The txusrclk in SD mode is
    -- 54 MHz, twice the word-rate of SD-SDI. So, this clock enable is used to
    -- enable the SD path only every other clock cycle.
    --
    process(txusrclk)
    begin
        if txusrclk'event and txusrclk = '1' then
            sd_ce <= not sd_ce;
        end if;
    end process;

    --
    -- SD video test pattern generator
    --
    -- This module is from XAPP248. It produces SD video test patterns. Either
    -- color bars or the SD-SDI checkfield can be generated.
    --
    -- This module is clocked at 54 MHz and is enabled every other clock cycle by
    -- the sd_ce signal. When in HD mode, the clock enable is always negated.
    --
    SDVIDGEN : vidgen 
        port map (
            clk_a           => txusrclk,
            rst_a           => n_pb1,
            ce_a            => sd_ce,
            pattern_a       => dipsw(1),
            q_a             => vidgen_out,
            h_sync_a        => open,
            v_sync_a        => open,
            field_a         => open,
            
            clk_b           => GND,
            rst_b           => GND,
            ce_b            => GND,
            pattern_b       => GND,
            q_b             => open,
            h_sync_b        => open,
            v_sync_b        => open,
            field_b         => open);
             
    --
    -- EDH processor
    --
    -- This module computes CRC values for the SD video, formats them into EDH
    -- packets, and inserts them into the appropriate place in the SD video from
    -- the video pattern generator. 
    --
    -- This module is clocked at 54 MHz and is enabled every other clock cycle by
    -- the sd_ce signal. When in HD mode, the clock enable is always negated.
    --
             
    EDH : edh_processor
        port map (
            clk             => txusrclk,
            ce              => sd_ce,
            rst             => n_pb1,
            vid_in          => vidgen_out,
            reacquire       => GND,
            en_sync_switch  => GND,
            en_trs_blank    => GND,
            anc_idh_local   => GND,
            anc_ues_local   => GND,
            ap_idh_local    => GND,
            ff_idh_local    => GND,
            errcnt_flg_en   => edh_flags_gnd,
            clr_errcnt      => GND,
            receive_mode    => GND,
            vid_out         => edh_out,
            std             => open,
            std_locked      => open,
            trs             => open,
            field           => open,
            v_blank         => open,
            h_blank         => open,
            horz_count      => open,
            vert_count      => open,
            sync_switch     => open,
            locked          => open,
            eav_next        => open,
            sav_next        => open,
            xyz_word        => open,
            anc_next        => open,
            edh_next        => open,
            rx_ap_flags     => open,
            rx_ff_flags     => open,
            rx_anc_flags    => open,
            ap_flags        => open,
            ff_flags        => open,
            anc_flags       => open,
            packet_flags    => open,
            errcnt          => open,
            edh_packet      => open);
                        
    edh_out_std <= TO_STDLOGICVECTOR(edh_out);
      
    --
    -- SDI encoder
    --
    -- This module is the SDI encoder. It is clock enabled every other clock
    -- cycle.
    --
    ENC : smpte_encoder
        port map (
            clk         => txusrclk,
            rst         => n_pb1,
            ce          => sd_ce,
            nrzi        => VCC,
            scram       => VCC,
            d           => edh_out_std,
            p_scram     => i_scram_q,
            p_nrzi      => enc_out(9),
            q           => enc_out,
            i_scram     => open,
            i_scram_q   => i_scram_q,
            i_nrzi      => open);
                        

    --
    -- 4X SD-SDI bit replication
    --
    -- Replicates each of the 10 encoded bits 4 times to produce a 40-bit
    -- sd_data vector for the RocketIO transmitter.
    --
    process(enc_out)
    begin
        for i in 0 to 9 loop
            sd_data(i*4)        <= enc_out(i);
            sd_data(i*4 + 1)    <= enc_out(i);
            sd_data(i*4 + 2)    <= enc_out(i);
            sd_data(i*4 + 3)    <= enc_out(i);
        end loop;
    end process;

    --
    -- SD-SDI data MUX
    --
    txdata <= sd_data(39 downto 20) when sd_ce = '1' else sd_data(19 downto 0);

    --
    -- RocketIO Transceiver
    --
    -- This is the RocketIO transceiver. Inside this module are bit swappers 
    -- needed to match the bit order of HD-SDI (LSB first) to the bit order of
    -- the RocketIO module (MSB first). 
    --
    RIO1 : hdsdi_rio_refclk 
        port map (
            brefclk         => GND,
            brefclk2        => GND,
            refclk          => clk_54M,
            refclk2         => GND,
            refclk_sel      => GND,
            rst             => n_pb1,
            loopback_en     => GND,
            loopback_mode   => GND,
            txinhibit       => GND,
            txdata          => txdata,
            txusrclk        => txusrclk,
            txusrclk2       => txusrclk,
            rxusrclk        => txusrclk,        -- even though Rx is not used, rxusrclk should be running
            rxusrclk2       => txusrclk,
            dcm_locked      => VCC,
            rxp             => mr_rx_rxp,
            rxn             => mr_rx_rxn,
            rxdata          => open,
            rxrecclk        => open,
            txp             => mr_tx1_txp,
            txn             => mr_tx1_txn
        );

    -------------------------------------------------------------------------------
    -- Output buffers required to drive SDV board components that are not used by
    -- this demo.
    --

    -- HD-SDI VCXO
    HDVCXO_U : OBUF_LVDCI_33 
        port map (
            I               => GND, 
            O               => hdsdi_vcxo_up
        );

    HDVCXO_D : OBUF_LVDCI_33 
        port map (
            I               => GND, 
            O               => hdsdi_vcxo_down
        );

    -- SDI 270MHz VCO

    SDVCO_U : OBUF_LVDCI_33 
        port map (
            I               => GND, 
            O               => sdi_vco_up
        );

    SDVCO_D : OBUF_LVDCI_33 
        port map (
            I               => GND, 
            O               => sdi_vco_down
        );

    -- SDI 27MHz VCXO

    SDVCXO_U : OBUF_LVDCI_33 
        port map (
            I               => GND, 
            O               => vcxo_27M_up
        );

    SDVCXO_D : OBUF_LVDCI_33 
        port map (
            I               => GND, 
            O               => vcxo_27M_down
        );

    VCXO27M_LPFEN : OBUF_LVDCI_33 
        port map (
            O               => vcxo_27M_sel, 
            I               => VCC
        );

    -- ICS660 PLL

    I660_S0 : OBUF_LVDCI_33 
        port map (
            I               => GND, 
            O               => ics660_s(0)
        );

    I660_S1 : OBUF_LVDCI_33 
        port map (
            I               => GND, 
            O               => ics660_s(1)
        );

    I660_S2 : OBUF_LVDCI_33 
        port map (
            I               => GND, 
            O               => ics660_s(2)
        );

    I660_S3 : OBUF_LVDCI_33 
        port map (
            I               => GND, 
            O               => ics660_s(3)
        );

    I660_X1 : OBUF_LVDCI_25 
        port map (
            I               => GND, 
            O               => ics660_x1
        );

    -- CY22394 PLL

    CYP_OE : OBUF_LVDCI_33 
        port map (
            I               => GND, 
            O               => cypll_oe
        );

    CYP_S2 : OBUF_LVDCI_33 
        port map (
            I               => GND,
            O               => cypll_s2
        );

    CYP_SC : OBUF_LVDCI_33 
        port map (
            I               => GND,
            O               => cypll_sclk
        );

    CYP_SD : OBUF_LVDCI_33 
        port map (
            I               => GND, 
            O               => cypll_sdat
        );

    -- ICS8745 PLL

    I8745_COUT : OBUFDS_LVDS_25 
        port map (
            I               => GND, 
            O               => ics8745_clkout_p,
            OB              => ics8745_clkout_n
        );

    I8745_SEL0 : OBUF_LVDCI_25 
        port map (
            I               => GND, 
            O               => ics8745_sel(0)
        );

    I8745_SEL1 : OBUF_LVDCI_25 
        port map (
            I               => GND,
            O               => ics8745_sel(1)
        );

    I8745_SEL2 : OBUF_LVDCI_25 
        port map (
            I               => GND,
            O               => ics8745_sel(2)
        );

    I8745_SEL3 : OBUF_LVDCI_25 
        port map (
            I               => GND, 
            O               => ics8745_sel(3)
        );

    I8745_PLLSEL : OBUF_LVDCI_25 
        port map (
            I               => GND, 
            O               => ics8745_pll_sel
        );

    I8745_MR : OBUF_LVDCI_25 
        port map (
            I               => GND,
            O               => ics8745_mr
        );

    -- SDI-Tx Port

    SDI_TXD : OBUFDS_LVDS_25 
        port map (
            I               => GND, 
            O               => sdi_tx_dout_p, 
            OB              => sdi_tx_dout_n
        );

    -- ASI-Tx Port
    ASI_TXD : OBUFDS_LVDS_25 
        port map (
            I               => GND,
            O               => asi_tx_p,
            OB              => asi_tx_n
        );

    -- RS-232 Port

    RS232TXD : OBUF_LVDCI_25 
        port map (
            I               => GND,
            O               => rs232_txd
        );

    RS232RTS : OBUF_LVDCI_25
        port map (
            I               => GND,
            O               => rs232_rts
        );

end synth;
