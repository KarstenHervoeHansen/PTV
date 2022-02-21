--------------------------------------------------------------------------------
-- hdsdi_rio_refclk.vhd
--
-- RocketIO transceiver instantiation for SMPTE 292M HD-SDI interface
--
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
--          Mar 2, 2004    XAPP683 release 1.00
--
-- Other modules instanced in this design:
--
--          none
--------------------------------------------------------------------------------
-- 
-- Description of module:
-- 
-- This module instantiates a single RocketIO transceiver using the GT_CUSTOM
-- primitive.
-- 
-- The modules swaps the txdata input vector to account for the fact that the
-- RocketIO transmitter sends the MSB first, whereas HD-SDI requires that the
-- LSB to be sent first. It does the same for the rxdata vector from the Rocket
-- IO receiver.
-- 
-- The module also includes the necessary logic to properly reset the RocketIO
-- transceiver. The RocketIO module is kept in reset until several clock cycles
-- after the dcm_locked signal is negated.
-- 
-- Note that this module connects the REFCLK, REFCLK2, BREFCLK, and BREFCLK2 
-- clock pins. However, it set the REF_CLK_V_SEL attribute of the GT_CUSTOM 
-- primitive to 0 so that only the REFCLK and REFCLK2 reference clock inputs 
-- are active. To use the BREFCLK and BREFCLK2 inputs, you must set the 
-- REF_CLK_V_SEL attribute to 1.
-- 
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.hdsdi_pkg.all;

library unisim; 
use unisim.vcomponents.all; 

entity hdsdi_rio_refclk is
    port (
        brefclk:        in  std_logic;      -- BREFCLK
        brefclk2:       in  std_logic;      -- BREFCLK2
        refclk:         in  std_logic;      -- REFCLK
        refclk2:        in  std_logic;      -- REFCLK2
        refclk_sel:     in  std_logic;      -- selects between BREFCLK and BREFCLK2
        rst:            in  std_logic;      -- reset signal
        loopback_en:    in  std_logic;      -- loopback enable
        loopback_mode:  in  std_logic;      -- 0 is serial, 1 is parallel mode
        txinhibit:      in  std_logic;      -- inhibits transmitter when 1
        txdata:         in  hd_vid20_type;  -- data to be transmitted
        txusrclk:       in  std_logic;      -- transmitter usr clock
        txusrclk2:      in  std_logic;      -- transmitter usr clock 2
        rxusrclk:       in  std_logic;      -- receiver usr clock
        rxusrclk2:      in  std_logic;      -- receiver usr clock 2
        dcm_locked:     in  std_logic;      -- DCM for RXUSRCLKs is locked
        rxp:            in  std_logic;      -- serial input - true
        rxn:            in  std_logic;      -- serial input - complement
        rxdata:         out hd_vid20_type;  -- received data from RocketIO receiver
        rxrecclk:       out std_logic;      -- clock recovered by RocketIO receiver
        txp:            out std_logic;      -- serial output - true
        txn:            out std_logic       -- serial output - complement
    );                             
end hdsdi_rio_refclk;

architecture synth of hdsdi_rio_refclk is


-- Internal wires
signal gt_rst :             std_logic;                      -- A delayed reset signal to satisfy the needs of the RocketIO
signal rst_x :              std_logic;                      -- gt_rst or reset
signal startup_counter:     std_logic_vector(2 downto 0);   -- used to generate gt_rst
signal txdata_swap:         std_logic_vector(19 downto 0);  -- data to be transmitted after bit swapping
signal recdata :            std_logic_vector(19 downto 0);  -- received data from RocketIO before bit swapping
signal loop_ctl :           std_logic_vector(1 downto 0);   -- loopback control signals
signal rxcharisk_float:     std_logic_vector(3 downto 2);   -- used to pad rxcharisk output to 4 bits
signal rxdata_float :       std_logic_vector(31 downto 16); -- used to pad rxdata output to 32 bits
signal rxrundisp_float:     std_logic_vector(3 downto 2);   -- used to pad rxrundisp output to 4 bits
signal gt_txchardispmode :  std_logic_vector(3 downto 0);
signal gt_txchardispval :   std_logic_vector(3 downto 0);
signal gt_txdata :          std_logic_vector(31 downto 0);
signal gt_rxcharisk :       std_logic_vector(3 downto 0);
signal gt_rxdata :          std_logic_vector(31 downto 0);
signal gt_rxrundisp :       std_logic_vector(3 downto 0);
signal GND1 :               std_logic;
signal GND :                std_logic_vector(3 downto 0);
signal GND2 :               std_logic_vector(15 downto 0);
signal VCC :                std_logic_vector(3 downto 0);

component GT_CUSTOM
    generic( 
        ALIGN_COMMA_MSB : boolean := FALSE;
        CHAN_BOND_LIMIT : integer := 16;
        CHAN_BOND_MODE : string := "OFF";
        CHAN_BOND_OFFSET : integer := 8;
        CHAN_BOND_ONE_SHOT : boolean := FALSE;
        CHAN_BOND_SEQ_1_1 : bit_vector := "00000000000";
        CHAN_BOND_SEQ_1_2 : bit_vector := "00000000000";
        CHAN_BOND_SEQ_1_3 : bit_vector := "00000000000";
        CHAN_BOND_SEQ_1_4 : bit_vector := "00000000000";
        CHAN_BOND_SEQ_2_1 : bit_vector := "00000000000";
        CHAN_BOND_SEQ_2_2 : bit_vector := "00000000000";
        CHAN_BOND_SEQ_2_3 : bit_vector := "00000000000";
        CHAN_BOND_SEQ_2_4 : bit_vector := "00000000000";
        CHAN_BOND_SEQ_2_USE : boolean := FALSE;
        CHAN_BOND_SEQ_LEN : integer := 1;
        CHAN_BOND_WAIT : integer := 8;
        CLK_COR_INSERT_IDLE_FLAG : boolean := FALSE;
        CLK_COR_KEEP_IDLE : boolean := FALSE;
        CLK_COR_REPEAT_WAIT : integer := 1;
        CLK_COR_SEQ_1_1 : bit_vector := "00000000000";
        CLK_COR_SEQ_1_2 : bit_vector := "00000000000";
        CLK_COR_SEQ_1_3 : bit_vector := "00000000000";
        CLK_COR_SEQ_1_4 : bit_vector := "00000000000";
        CLK_COR_SEQ_2_1 : bit_vector := "00000000000";
        CLK_COR_SEQ_2_2 : bit_vector := "00000000000";
        CLK_COR_SEQ_2_3 : bit_vector := "00000000000";
        CLK_COR_SEQ_2_4 : bit_vector := "00000000000";
        CLK_COR_SEQ_2_USE : boolean := FALSE;
        CLK_COR_SEQ_LEN : integer := 1;
        CLK_CORRECT_USE : boolean := TRUE;
        COMMA_10B_MASK : bit_vector := "1111111000";
        CRC_END_OF_PKT : string := "K29_7";
        CRC_FORMAT : string := "USER_MODE";
        CRC_START_OF_PKT : string := "K27_7";
        DEC_MCOMMA_DETECT : boolean := TRUE;
        DEC_PCOMMA_DETECT : boolean := TRUE;
        DEC_VALID_COMMA_ONLY : boolean := TRUE;
        MCOMMA_10B_VALUE : bit_vector := "1100000000";
        MCOMMA_DETECT : boolean := TRUE;
        PCOMMA_10B_VALUE : bit_vector := "0011111000";
        PCOMMA_DETECT : boolean := TRUE;
        REF_CLK_V_SEL : integer := 0;
        RX_BUFFER_USE : boolean := TRUE;
        RX_CRC_USE : boolean := FALSE;
        RX_DATA_WIDTH : integer := 2;
        RX_DECODE_USE : boolean := TRUE;
        RX_LOS_INVALID_INCR : integer := 1;
        RX_LOS_THRESHOLD : integer := 4;
        RX_LOSS_OF_SYNC_FSM : boolean := TRUE;
        SERDES_10B : boolean := FALSE;
        TERMINATION_IMP : integer := 50;
        TX_BUFFER_USE : boolean := TRUE;
        TX_CRC_FORCE_VALUE : bit_vector := "11010110";
        TX_CRC_USE : boolean := FALSE;
        TX_DATA_WIDTH : integer := 2;
        TX_DIFF_CTRL : integer := 500;
        TX_PREEMPHASIS : integer := 0
    );
    port (
        CHBONDI : in std_logic_vector (3 downto 0);
        CONFIGENABLE : in std_logic;
        CONFIGIN : in std_logic;
        ENMCOMMAALIGN : in std_logic;
        ENPCOMMAALIGN : in std_logic;
        ENCHANSYNC : in std_logic;
        LOOPBACK : in std_logic_vector (1 downto 0);
        POWERDOWN : in std_logic;
        REFCLK : in std_logic;
        REFCLK2 : in std_logic;
        REFCLKSEL : in std_logic;
        BREFCLK : in std_logic;
        BREFCLK2 : in std_logic;
        RXN : in std_logic;
        RXP : in std_logic;
        RXPOLARITY : in std_logic;
        RXRESET : in std_logic;
        RXUSRCLK : in std_logic;
        RXUSRCLK2 : in std_logic;
        TXBYPASS8B10B : in std_logic_vector (3 downto 0);
        TXCHARDISPMODE : in std_logic_vector (3 downto 0);
        TXCHARDISPVAL : in std_logic_vector (3 downto 0);
        TXCHARISK : in std_logic_vector (3 downto 0);
        TXDATA : in std_logic_vector (31 downto 0);
        TXFORCECRCERR : in std_logic;
        TXINHIBIT : in std_logic;
        TXPOLARITY : in std_logic;
        TXRESET : in std_logic;
        TXUSRCLK : in std_logic;
        TXUSRCLK2 : in std_logic;
        CHBONDDONE : out std_logic;
        CHBONDO : out std_logic_vector (3 downto 0);
        CONFIGOUT : out std_logic;
        RXBUFSTATUS : out std_logic_vector (1 downto 0);
        RXCHARISCOMMA : out std_logic_vector (3 downto 0);
        RXCHARISK : out std_logic_vector (3 downto 0);
        RXCHECKINGCRC : out std_logic;
        RXCLKCORCNT : out std_logic_vector (2 downto 0);
        RXCOMMADET : out std_logic;
        RXCRCERR : out std_logic;
        RXDATA : out std_logic_vector (31 downto 0);
        RXDISPERR : out std_logic_vector (3 downto 0);
        RXLOSSOFSYNC : out std_logic_vector (1 downto 0);
        RXNOTINTABLE : out std_logic_vector (3 downto 0);
        RXREALIGN : out std_logic;
        RXRECCLK : out std_logic;
        RXRUNDISP : out std_logic_vector (3 downto 0);
        TXBUFERR : out std_logic;
        TXKERR : out std_logic_vector (3 downto 0);
        TXN : out std_logic;
        TXP : out std_logic;
        TXRUNDISP : out std_logic_vector (3 downto 0)
    );
end component;

begin

    GND1 <= '0';
    GND  <= (others => '0');
    GND2 <= (others => '0');
    VCC  <= (others => '1');

--
-- loopback control signals
--
-- Encodes the loopback_en and loopback_mode signals into the two inputs
-- needed by the RocketIO to control loopback mode.
--
    loop_ctl(1) <= loopback_en and not loopback_mode;
    loop_ctl(0) <= loopback_en and loopback_mode;

--
-- Instantiate the RocketIO transceiver
--
-- Use the GT_CUSTOM primitive because we need to be able to bypass the 8B10B
-- encode/decode functions.
--
   RIO : GT_CUSTOM
        generic map (
            ALIGN_COMMA_MSB             => FALSE,
            CHAN_BOND_LIMIT             => 16,
            CHAN_BOND_MODE              => "OFF",
            CHAN_BOND_OFFSET            => 8,
            CHAN_BOND_ONE_SHOT          => FALSE,
            CHAN_BOND_SEQ_1_1           => "00000000000",
            CHAN_BOND_SEQ_1_2           => "00000000000",
            CHAN_BOND_SEQ_1_3           => "00000000000",
            CHAN_BOND_SEQ_1_4           => "00000000000",
            CHAN_BOND_SEQ_2_1           => "00000000000",
            CHAN_BOND_SEQ_2_2           => "00000000000",
            CHAN_BOND_SEQ_2_3           => "00000000000",
            CHAN_BOND_SEQ_2_4           => "00000000000",
            CHAN_BOND_SEQ_2_USE         => FALSE,
            CHAN_BOND_SEQ_LEN           => 1,
            CHAN_BOND_WAIT              => 8,
            CLK_CORRECT_USE             => FALSE,
            CLK_COR_INSERT_IDLE_FLAG    => FALSE,
            CLK_COR_KEEP_IDLE           => FALSE,
            CLK_COR_REPEAT_WAIT         => 1,
            CLK_COR_SEQ_1_1             => "00000000000",
            CLK_COR_SEQ_1_2             => "00000000000",
            CLK_COR_SEQ_1_3             => "00000000000",
            CLK_COR_SEQ_1_4             => "00000000000",
            CLK_COR_SEQ_2_1             => "00000000000",
            CLK_COR_SEQ_2_2             => "00000000000",
            CLK_COR_SEQ_2_3             => "00000000000",
            CLK_COR_SEQ_2_4             => "00000000000",
            CLK_COR_SEQ_2_USE           => FALSE,
            CLK_COR_SEQ_LEN             => 1,
            COMMA_10B_MASK              => "1111111000",
            CRC_END_OF_PKT              => "K29_7",
            CRC_FORMAT                  => "USER_MODE",
            CRC_START_OF_PKT            => "K27_7",
            DEC_MCOMMA_DETECT           => FALSE,
            DEC_PCOMMA_DETECT           => FALSE,
            DEC_VALID_COMMA_ONLY        => FALSE,
            MCOMMA_10B_VALUE            => "1100000000",
            MCOMMA_DETECT               => FALSE,
            PCOMMA_10B_VALUE            => "0011111000",
            PCOMMA_DETECT               => FALSE,
            RX_BUFFER_USE               => TRUE,
            RX_CRC_USE                  => FALSE,
            RX_DATA_WIDTH               => 2,
            RX_DECODE_USE               => FALSE,
            RX_LOSS_OF_SYNC_FSM         => FALSE,
            RX_LOS_INVALID_INCR         => 1,
            RX_LOS_THRESHOLD            => 4,
            TERMINATION_IMP             => 50,
            SERDES_10B                  => FALSE,
            TX_BUFFER_USE               => TRUE,
            TX_CRC_FORCE_VALUE          => "11010110",
            TX_CRC_USE                  => FALSE,
            TX_DATA_WIDTH               => 2,
            TX_DIFF_CTRL                => 800,
            TX_PREEMPHASIS              => 0,
            REF_CLK_V_SEL               => 0)
     
        port map (
            CHBONDI                     => GND(3 downto 0),
            CONFIGENABLE                => GND1,
            CONFIGIN                    => GND1,
            ENMCOMMAALIGN               => GND1,
            ENPCOMMAALIGN               => GND1,
            ENCHANSYNC                  => GND1,
            LOOPBACK                    => loop_ctl,
            POWERDOWN                   => GND1,
            REFCLK                      => refclk,
            REFCLK2                     => refclk2,
            REFCLKSEL                   => refclk_sel,
            BREFCLK                     => brefclk,
            BREFCLK2                    => brefclk2,
            RXN                         => rxn,
            RXP                         => rxp,
            RXPOLARITY                  => GND1,
            RXRESET                     => rst_x,
            RXUSRCLK                    => rxusrclk,
            RXUSRCLK2                   => rxusrclk2,
            TXBYPASS8B10B               => VCC(3 downto 0),
            TXCHARDISPMODE              => gt_txchardispmode,
            TXCHARDISPVAL               => gt_txchardispval,
            TXCHARISK                   => GND(3 downto 0),
            TXDATA                      => gt_txdata,
            TXFORCECRCERR               => GND1,
            TXINHIBIT                   => txinhibit,
            TXPOLARITY                  => GND1,
            TXRESET                     => rst_x,
            TXUSRCLK                    => txusrclk,
            TXUSRCLK2                   => txusrclk2,
            CHBONDDONE                  => open,
            CHBONDO                     => open,
            CONFIGOUT                   => open,
            RXBUFSTATUS                 => open,
            RXCHARISCOMMA               => open,
            RXCHARISK                   => gt_rxcharisk,
            RXCHECKINGCRC               => open,
            RXCLKCORCNT                 => open,
            RXCOMMADET                  => open,
            RXCRCERR                    => open,
            RXDATA                      => gt_rxdata,
            RXDISPERR                   => open,
            RXLOSSOFSYNC                => open,
            RXNOTINTABLE                => open,
            RXREALIGN                   => open,
            RXRECCLK                    => rxrecclk,
            RXRUNDISP                   => gt_rxrundisp,
            TXBUFERR                    => open,
            TXKERR                      => open,
            TXN                         => txn,
            TXP                         => txp,
            TXRUNDISP                   => open);

gt_txchardispmode <= ("00" & txdata_swap(19) & txdata_swap(9));
gt_txchardispval <= ("00" & txdata_swap(18) & txdata_swap(8));
gt_txdata <= ("0000000000000000" & txdata_swap(17 downto 10) & txdata_swap(7 downto 0));
recdata <= (gt_rxcharisk(1) & gt_rxrundisp(1) & gt_rxdata(15 downto 8) & 
            gt_rxcharisk(0) & gt_rxrundisp(0) & gt_rxdata(7 downto 0));
 
--
-- Swap the received data
--
    process(recdata)
    begin
        for i in 0 to 19 loop
            rxdata(i) <= recdata(19 - i);
        end loop;
    end process;
 
--
-- Swap the transmit data
--
    process(txdata)
    begin
        for i in 0 to 19 loop
            txdata_swap(i) <= txdata(19 - i);
        end loop;
    end process;

--
-- RocketIO transceiver reset logic
--
    process(rxusrclk, dcm_locked)
    begin
        if rxusrclk'event and rxusrclk = '1' then
            if dcm_locked = '0' then
                startup_counter <= (others => '0');
            else
                startup_counter <= (startup_counter(1 downto 0) & '1');
            end if;
        end if;
    end process;

    process(rxusrclk, dcm_locked)
    begin
        if rxusrclk'event and rxusrclk = '1' then
            if dcm_locked = '0' then
                gt_rst <= '1';
            elsif startup_counter(2) = '1' then
                gt_rst <= '0';
            end if;
        end if;
    end process;

    rst_x <= gt_rst or rst;
                             
end synth;