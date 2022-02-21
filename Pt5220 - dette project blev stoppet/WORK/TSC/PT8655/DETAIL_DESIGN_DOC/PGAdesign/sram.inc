-- Copyright (C) 1988-1999 Altera Corporation
-- Any megafunction design, and related net list (encrypted or decrypted),
-- support information, device programming or simulation file, and any other
-- associated documentation or information provided by Altera or a partner
-- under Altera's Megafunction Partnership Program may be used only to
-- program PLD devices (but not masked PLD devices) from Altera.  Any other
-- use of such megafunction design, net list, support information, device
-- programming or simulation file, or any other related documentation or
-- information is prohibited for any other purpose, including, but not
-- limited to modification, reverse engineering, de-compiling, or use with
-- any other silicon devices, unless such use is explicitly licensed under
-- a separate agreement with Altera or a megafunction partner.  Title to
-- the intellectual property, including patents, copyrights, trademarks,
-- trade secrets, or maskworks, embodied in any such megafunction design,
-- net list, support information, device programming or simulation file, or
-- any other related documentation or information provided by Altera or a
-- megafunction partner, remains with Altera, the megafunction partner, or
-- their respective licensors.  No other licenses, including any licenses
-- needed under any third party's intellectual property, are provided herein.

-- MAX+plus II Include File
-- Version 9.21 2/10/99
-- Created: Thu Jun 24 15:54:01 1999

FUNCTION sram (addr[16..0], count[16..0], pload, clk6144, rd, we, cs_audioram, play_audio, wclk_sel, wclk_xlr_disable, wclk)
    RETURNS (data[7..0], d[7..0], a[16..0], we_sram, oe_sram, cs_sram, ser_data_bnc, ser_data_xlr, wclk_bnc);
