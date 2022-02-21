--------------------------------------------------------------------------------
-- hdsdi_tx_crc_ln.vhd
--
-- SMPTE 292M-1998 HD-SDI transmitter CRC generator/insertion and line number
-- insertion
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
--          Mar 2, 2004     XAPP682 release 1.00
--
-- Other modules instanced in this design:
--
--          hdsdi_crc
--          hdsdi_insert_crc
--          hdsdi_insert_ln
--
--------------------------------------------------------------------------------
--
-- Description of module:
-- 
-- This module inserts line numbers and generates and inserts CRCs into HD
-- video streams. Note that this module is essentially identical to the
-- hdsdi_tx_path module from XAPP680 except that the HD-SDI encoder has been
-- removed to leave just the LN insertion and CRC generation and insertion
-- capability.
--
-- There are several control inputs to this module:
-- 
-- insert_ln: a high enables the module to insert the line numbers from the ln
-- port into the video channels.
-- 
-- insert_crc: a high enables the module to insert the CRC values it calculates
-- into the video channels.
-- 
-- force_crc_err: when high, this input forces the module to generate a bad CRC
-- value which will be inserted into only the C channel if insert_crc is also 
-- high. The Y channel CRC value will not be affected.
--  
-- The module also has the following timing and data inputs:
-- 
-- c_in, y_in: C and Y channel video inputs
-- 
-- ln: line number input port
-- 
-- eav: must be asserted when the XYZ word of an EAV is present on c_in and y_in
-- 
-- sav: must be asserted when the XYZ word of an SAV is present on c_in and y_in
-- 
-- The module does not contain any input or output registers. If timing can not 
-- be met on the input side of the module, place pipeline delay registers on the
-- c_in, y_in, eav, and sav signals. The ln port may not need a pipeline delay
-- register if it remains asserted for several clock cycles after the EAV.
-- 
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

use work.hdsdi_pkg.all;

entity hdsdi_tx_crc_ln is
    port (
        clk:            in  std_logic;      -- clock input
        rst:            in  std_logic;      -- async reset input
        ce:             in  std_logic;      -- clock enable input
        c_in:           in  hd_video_type;  -- chroma channel data input
        y_in:           in  hd_video_type;  -- luma channel data input
        insert_crc:     in  std_logic;      -- high enables generation & insertion of CRCs
        force_crc_err:  in  std_logic;      -- high forces CRC error to be generated
        insert_ln:      in  std_logic;      -- high enables line number insertion
        ln:             in  hd_vpos_type;   -- line numbers to be inserted
        eav:            in  std_logic;      -- input asserted during XYZ word of EAV symbol
        sav:            in  std_logic;      -- input asserted during XYZ word of SAV symbol
        c_out:          out hd_video_type;  -- chroma channel output
        y_out:          out hd_video_type   -- luma channel output
    );
end hdsdi_tx_crc_ln;

architecture synth of hdsdi_tx_crc_ln is

--
-- Internal signals
--
signal c_crc_calc :     hd_crc18_type;               -- output of C channel CRC generator
signal c_crc_out :      hd_crc18_type;               -- CRC with forced CRC errors if force_crc_err asserted
signal y_crc_out :      hd_crc18_type;               -- output of Y channel CRC generator
signal c_ln_out :       hd_video_type;               -- output of C channel line number inserter
signal y_ln_out :       hd_video_type;               -- output of Y channel line number inserter
signal c_crc_ins_out :  hd_video_type;               -- C channel after CRC insertion
signal y_crc_ins_out :  hd_video_type;               -- Y channel after CRC insertion
signal crc_en :         std_logic;                   -- controls which words are included in CRC calculations
signal clr_crc :        std_logic;                   -- clears the CRC calculators to start new calculation
signal eav_dly :        std_logic_vector(3 downto 0);-- shift reg used to create video timing
signal crc_en_ce :      std_logic;                   -- crc_en AND crc

component hdsdi_insert_ln
    port (
        insert_ln:  in  std_logic;      -- enables insertion of line numbers when 1
        ln_word0:   in  std_logic;      -- input asserted during time for first LN word in EAV
        ln_word1:   in  std_logic;      -- input asserted during time for second LN word in EAV
        c_in:       in  hd_video_type;  -- C channel video input
        y_in:       in  hd_video_type;  -- Y channel video input
        ln:         in  hd_vpos_type;   -- line number inputs
        c_out:      out hd_video_type;  -- C channel video output
        y_out:      out hd_video_type   -- Y channel video output
    );
end component;

component hdsdi_insert_crc
    port (
        insert_crc: in  std_logic;      -- CRC valaues will be inserted when this input is high
        crc_word0:  in  std_logic;      -- input asserted during time for first CRC word in EAV
        crc_word1:  in  std_logic;      -- input asserted during time for second CRC word in EAV
        c_in:       in  hd_video_type;  -- C channel video input
        y_in:       in  hd_video_type;  -- Y channel video input
        c_crc:      in  hd_crc18_type;  -- C channel CRC value input
        y_crc:      in  hd_crc18_type;  -- Y channel CRC value input
        c_out:      out hd_video_type;  -- C channel video output
        y_out:      out hd_video_type   -- Y channel video output
    );
end component;

component hdsdi_crc
    port (
        clk:        in  std_logic;      -- word rate clock (74.25 MHz)
        ce:         in  std_logic;      -- clock enable
        rst:        in  std_logic;      -- async reset
        clr:        in  std_logic;      -- assert during first cycle of CRC calc
        d:          in  hd_video_type;  -- video word input
        crc_out:    out hd_crc18_type); -- CRC output value
end component;

begin

    --
    -- EAV delay register
    --
    -- Generates timing control signals for line number insertion and CRC generation
    -- and insertion.
    --
    process(clk, rst)
    begin
        if rst = '1' then
            eav_dly <= (others => '0');
        elsif clk'event and clk = '1' then
            if ce = '1' then
                eav_dly <= (eav_dly(2 downto 0) & eav);
            end if;
        end if;
    end process;

    --
    -- Instantiate the line number formatting and insertion module
    --
    INS_LN : hdsdi_insert_ln
        port map (
            insert_ln       => insert_ln,
            ln_word0        => eav_dly(0),
            ln_word1        => eav_dly(1),
            c_in            => c_in,
            y_in            => y_in,
            ln              => ln,
            c_out           => c_ln_out,
            y_out           => y_ln_out
        );
            

    --
    -- Generate timing control signals for the CRC calculators.
    --
    -- The crc_en signal determines which words are included into the CRC 
    -- calculation. All words that enter the hdsdi_crc module when crc_en is high
    -- are included in the calculation. To meet the HD-SDI spec, the CRC calculation
    -- must being with the first word after the SAV and end after the second line
    -- number word after the EAV.
    --
    -- The clr_crc signal clears the internal registers of the hdsdi_crc modules to
    -- cause a new CRC calculation to begin. The crc_en signal is asserted during
    -- the XYZ word of the SAV since the next word after the SAV XYZ word is the
    -- first word to be included into the new CRC calculation.
    --
    process(clk, rst)
    begin
        if rst = '1' then
            crc_en <= '0';
        elsif clk'event and clk = '1' then
            if ce = '1' then
                if sav = '1' then
                    crc_en <= '1';
                elsif eav_dly(1) = '1' then
                    crc_en <= '0';
                end if;
            end if;
        end if;
    end process;

    process(clk, rst)
    begin
        if rst = '1' then
            clr_crc <= '0';
        elsif clk'event and clk = '1' then
            if ce = '1' then
                clr_crc <= sav;
            end if;
        end if;
    end process;

    --
    -- Instantiate the CRC generators
    --
    crc_en_ce <= ce and crc_en;

    C_CRC : hdsdi_crc
        port map (
            clk             => clk,
            ce              => crc_en_ce,
            rst             => rst,
            clr             => clr_crc,
            d               => c_ln_out,
            crc_out         => c_crc_calc
        );

    Y_CRC : hdsdi_crc
        port map (
            clk             => clk,
            ce              => crc_en_ce,
            rst             => rst,
            clr             => clr_crc,
            d               => y_ln_out,
            crc_out         => y_crc_out
        );

    --
    -- Force a C channel CRC error if force_crc_err is asserted by inverting the 
    -- LSB of the calculated C channel CRC value.
    --
    c_crc_out <= (c_crc_calc(17 downto 1) & (c_crc_calc(0) xor force_crc_err));

    --
    -- Insert the CRC values into the Y and C channels. The CRC values are inserted
    -- after the line number words after the EAV.
    --
    CRC : hdsdi_insert_crc
        port map (
            insert_crc      => insert_crc,
            y_in            => y_ln_out,
            c_in            => c_ln_out,
            crc_word0       => eav_dly(2),
            crc_word1       => eav_dly(3),
            y_crc           => y_crc_out,
            c_crc           => c_crc_out,
            y_out           => y_crc_ins_out,
            c_out           => c_crc_ins_out
        );

    y_out <= y_crc_ins_out;
    c_out <= c_crc_ins_out;

end synth;