-------------------------------------------------------------------
-- multi_sdi_bitrep_4x.vhd
--
-- Multirate HD/SD-SDI Transmitter bit replicator
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
--          Mar 2, 2004     XAPP683 release 1.00
--
--
-- Other modules instanced in this design:
--
--          none
-------------------------------------------------------------------
-- 
-- Description of module:
-- 
-- This module is used to create the 20-bit input value for the RocketIO's
-- TXDATA port. For HD-SDI, it simply passes the 20 encoded bits from the
-- input to the output. For SD-SDI, however, it replicates each of the 10 MSBs
-- of the input vector four times, producing a 40-bit value from each 10-bit
-- input value. This allows the RocketIO transmitter to be run at 4X the
-- actual bit rate, sending each bit four times in a row.
-- 
-- When running in SD-SDI mode, this module outputs the 40-bit vector as two
-- 20-bit vectors. The clock input that controls this should be a 2X word-rate
-- clock. For example, if the reference clock to the RocketIO is 54MHz then the
-- bit rate of the RocketIO transmitter will be 20 times this clock or 1.08 Gbps
-- which is exactly 4X the 270 Mbps SD-SDI bit rate. The same 54MHz clock is
-- used as the TXUSRCLK and TXUSRCLK2 inputs to the RocketIO and also as the
-- input clock to this module to control the SD-SDI data output MUX.
-- 
--------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

use work.hdsdi_pkg.all;

entity multi_sdi_bitrep_4x is
    port (
        ms_bits:    in  std_logic;      -- controls SD MS/LS MUX: 0 = LS 20 bits
        hd_sd:      in  std_logic;      -- 0 = HD, 1 = SD
        d:          in  hd_vid20_type;  -- input data vector
        q:          out hd_vid20_type   -- output data vector
    );
end multi_sdi_bitrep_4x;

architecture synth of multi_sdi_bitrep_4x is
    
signal sd_data :    std_logic_vector(39 downto 0);
signal sd_data_mux: hd_vid20_type;

begin
    
    --
    -- 4X SD-SDI bit replication
    --
    -- Replicates each of the 10 MS bits of the d input port four times to 
    -- produce a 40-bit sd_data vector.
    --
    process(d(19 downto 10))
    begin
        for i in 0 to 9 loop
            sd_data(i * 4)     <= d(10 + i);
            sd_data(i * 4 + 1) <= d(10 + i);
            sd_data(i * 4 + 2) <= d(10 + i);
            sd_data(i * 4 + 3) <= d(10 + i);
        end loop;
    end process;

    --
    -- SD-SDI data MUX
    --
    -- This MUX controls whether the MS 20 or LS 20 bits of the sd_data vector
    -- are sent to the HD/SD MUX.
    --
    sd_data_mux <= sd_data(39 downto 20) when ms_bits = '1' else 
                   sd_data(19 downto 0);

    --
    -- HD/SD data MUX
    --
    -- This MUX passes the input d port out in HD mode. In SD mode, the output
    -- of the SD-SDI data MUX is sent out.
    --
    q <= sd_data_mux when hd_sd = '1' else d;

end synth;