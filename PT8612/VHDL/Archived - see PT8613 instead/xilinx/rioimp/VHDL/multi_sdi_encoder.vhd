-------------------------------------------------------------------
-- multi_sdi_encoder.vhd
--
-- Multi-rate SDI Encoder module
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
--          Mar 2, 2004     XAPP683 releae 1.00
--
--
-- Other modules instanced in this design:
--
--          smpte_encoder
-------------------------------------------------------------------
-- 
-- Description of module:
-- 
-- This module is the top-level module of the multi-rate HD/SD-SDI encoder. For
-- HD-SDI this module encodes 20 bits of data, 10 bits of chroma (C) and 10 bits
-- of luma (Y), per clock cycle. For SD-SDI, 10 bits are encoded per clock 
-- cycle.
-- 
-- This module instantiates the smpte_encoder module twice, with one module 
-- encoding the C data and the other the Y data. The two modules are cross 
-- connected so that the results from one encoder affects the encoding of the 
-- bits in the other encoder, as required by the HD-SDI encoding scheme. When 
-- encoding SD-SDI, only the Y channel SMPTE encoder is used
-- 
-- The q output is a 20-bit encoded value. Note that this value must be bit-
-- swapped before it can be connected to the 20-bit input of the RocketIO 
-- transmitter. For SD-SDI, only the LS 10-bits of the output are valid.
-- 
-- Note that this module does not make multiple copies of each encoded bit for
-- SD-SDI as required to run the RocketIO MGT in oversampled mode for the slow
-- SD-SDI bit rates. This bit replication must be done externally to this 
-- module.
-- 
--------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

use work.hdsdi_pkg.all;

entity multi_sdi_encoder is
    port (
        clk:        in  std_logic;      -- word rate clock (74.25 MHz)
        rst:        in  std_logic;      -- async reset
        ce:         in  std_logic;      -- clock enable \
        hd_sd:      in  std_logic;      -- 0 = HD, 1 = SD
        nrzi:       in  std_logic;      -- 1 enables NRZ-to-NRZI conversion
        scram:      in  std_logic;      -- 1 enables SDI scrambler
        c:          in  hd_video_type;  -- C channel input data port
        y:          in  hd_video_type;  -- Y channel input data port
        q:          out hd_vid20_type); -- output data port
end multi_sdi_encoder;

architecture synth of multi_sdi_encoder is

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

-- Internal signal definitions
signal c_in_reg :       hd_video_type;  -- C channel input register
signal y_in_reg :       hd_video_type;  -- Y channel input register
signal c_i_scram :                      -- C channel intermediate scrambled data
                        std_logic_vector(8 downto 0);
signal y_i_scram_q:                     -- Y channel intermediate scrambled data
                        std_logic_vector(8 downto 0);
signal c_i_nrzi:        std_logic;      -- C channel intermediate nrzi data
signal c_out :          hd_video_type;  -- output of C scrambler
signal y_out :          hd_video_type;  -- output of Y scrambler
signal y_p_scram_mux :                  -- p_scram input MUX for Y encoder
                        std_logic_vector(8 downto 0);
signal y_p_nrzi_mux :   std_logic;      -- p_nrzi input MUX for Y encoder
signal hd_ce :          std_logic;      -- clock enable for C channel, negated during SD
 
begin

    hd_ce <= not hd_sd and ce;

    --
    -- Instantiate encoder modules for C and Y channels
    --
    C_scram : smpte_encoder
        port map (
            clk         => clk,
            rst         => rst,
            ce          => hd_ce,
            nrzi        => nrzi,
            scram       => scram,
            d           => c_in_reg,
            p_scram     => y_i_scram_q,
            p_nrzi      => y_out(9),
            q           => c_out,
            i_scram     => c_i_scram,
            i_scram_q   => open,
            i_nrzi      => c_i_nrzi);

    Y_scram : smpte_encoder
        port map (
            clk         => clk,
            rst         => rst,
            ce          => ce,
            nrzi        => nrzi,
            scram       => scram,
            d           => y_in_reg,
            p_scram     => y_p_scram_mux,
            p_nrzi      => y_p_nrzi_mux,
            q           => y_out,
            i_scram     => open,
            i_scram_q   => y_i_scram_q,
            i_nrzi      => open);

    --
    -- These MUXes control whether the two smpte_scrambler modules are configured
    -- for HD-SDI or SD-SDI. In HD-SDI, the C and Y channel scramblers are
    -- cross connected to encode a 20-bit word every clock cycle. In SD-SDI mode,
    -- only the Y channel scrambler is used and it's output is feedback to its
    -- inputs to allow the sequential scrambling of the data 10-bits at a time.
    --
    y_p_scram_mux <= y_i_scram_q when hd_sd = '1' else c_i_scram;
    y_p_nrzi_mux <= y_out(9) when hd_sd = '1' else c_i_nrzi;

    --
    -- Input registers
    --
    process(clk, rst)
    begin
        if rst = '1' then
            y_in_reg <= (others => '0');
        elsif clk'event and clk = '1' then
            if ce = '1' then
                y_in_reg <= y;
            end if;
        end if;
    end process;

    process(clk, rst)
    begin
        if rst = '1' then
            c_in_reg <= (others => '0');
        elsif clk'event and clk = '1' then
            if ce = '1' and hd_sd = '0'then
                c_in_reg <= c;
            end if;
        end if;
    end process;
    
    --
    -- Output assignment
    --
    q <= (y_out & c_out);

end synth;