-------------------------------------------------------------------------------- 
-- Copyright (c) 2004 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: multigenHD_vert.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2005-01-11 14:05:33-07 $
-- /___/   /\    Date Created: May 28, 2004 
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: multigenHD_vert.vhd,rcs $
-- Revision 1.2  2005-01-11 14:05:33-07  jsnow
-- VHDL generics for INIT and SRVAL were only 32 bits instead of
-- the required 36 bits. This caused ModelSim errors.
--
-- Revision 1.1  2004-12-09 15:01:58-07  jsnow
-- Cosmetic changes only.
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
-- This file contains the vertical sequencer for the HD video pattern generator.
-- A block RAM is used as a finite state machine, sequencing through the various
-- vertical sections of each video pattern. The module outputs a v_band code 
-- indicating which vertical portion of the video pattern should be displayed.      
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

use work.hdsdi_pkg.all;
use work.multigenHD_pkg.all;

library unisim; 
use unisim.vcomponents.all; 

entity multigenHD_vert is
    port (
        clk:            in  std_logic;                      -- video clock
        rst:            in  std_logic;                      -- async reset
        ce:             in  std_logic;                      -- clock enable
        std:            in  std_logic_vector(2 downto 0);   -- video standard select
        pattern:        in  std_logic_vector(1 downto 0);   -- selects pattern type (colorbars or checkfield)
        h_counter_lsb:  in  std_logic;                      -- LSB of horizontal counter
        v_inc:          in  std_logic;                      -- causes the vertical counter to increment
        v_band:         out vband_type;                     -- vertical band code output
        v:              out std_logic;                      -- vertical blanking indicator
        f:              out std_logic;                      -- field indicator
        first_line:     out std_logic;                      -- asserted during first active line
        y_ramp_inc_sel: out std_logic;                      -- controls Y-Ramp increment selection
        line_num:       out hd_vpos_type                    -- current vertical line number
    );
end multigenHD_vert;

architecture synth of multigenHD_vert is


-------------------------------------------------------------------------------
-- Signal definitions
--

signal vrom_addr :          std_logic_vector(8 downto 0);           -- VROM address
signal vrom_out :           std_logic_vector(31 downto 0);          -- VROM output
signal v_counter :          hd_vpos_type;                           -- vertical counter
signal v_next_evnt :        std_logic_vector(V_EVNT_MSB downto 0);  -- next vertical event
signal v_evnt_match :       std_logic;                              -- output of vertical event comparator
signal vrom_en :            std_logic;                              -- EN input to vertical ROM
signal v_region :           vrgn_type;                              -- current vertical region
signal v_band_rom :         vband_type;                             -- v_band for most patterns
signal v_band_75_rom :      vband_type;                             -- v_band for 75% color bars pattern
signal v_clr :              std_logic;                              -- clears the vertical counter
signal GND :                std_logic := '0';
signal VCC :                std_logic := '1';
signal GND4 :               std_logic_vector(3 downto 0) := "0000";
signal GND32 :              std_logic_vector(31 downto 0) := (others => '0');

attribute INIT : string;
attribute SRVAL : string;
attribute WRITE_MODE : string;
attribute INITP_00 : string;
attribute INITP_01 : string;
attribute INITP_02 : string;
attribute INITP_03 : string;
attribute INITP_04 : string;
attribute INITP_05 : string;
attribute INITP_06 : string;
attribute INITP_07 : string;
attribute INIT_00 : string;
attribute INIT_01 : string;
attribute INIT_02 : string;
attribute INIT_03 : string;
attribute INIT_04 : string;
attribute INIT_05 : string;
attribute INIT_06 : string;
attribute INIT_07 : string;
attribute INIT_08 : string;
attribute INIT_09 : string;
attribute INIT_0A : string;
attribute INIT_0B : string;
attribute INIT_0C : string;
attribute INIT_0D : string;
attribute INIT_0E : string;
attribute INIT_0F : string;
attribute INIT_10 : string;
attribute INIT_11 : string;
attribute INIT_12 : string;
attribute INIT_13 : string;
attribute INIT_14 : string;
attribute INIT_15 : string;
attribute INIT_16 : string;
attribute INIT_17 : string;
attribute INIT_18 : string;
attribute INIT_19 : string;
attribute INIT_1A : string;
attribute INIT_1B : string;
attribute INIT_1C : string;
attribute INIT_1D : string;
attribute INIT_1E : string;
attribute INIT_1F : string;
attribute INIT_20 : string;
attribute INIT_21 : string;
attribute INIT_22 : string;
attribute INIT_23 : string;
attribute INIT_24 : string;
attribute INIT_25 : string;
attribute INIT_26 : string;
attribute INIT_27 : string;
attribute INIT_28 : string;
attribute INIT_29 : string;
attribute INIT_2A : string;
attribute INIT_2B : string;
attribute INIT_2C : string;
attribute INIT_2D : string;
attribute INIT_2E : string;
attribute INIT_2F : string;
attribute INIT_30 : string;
attribute INIT_31 : string;
attribute INIT_32 : string;
attribute INIT_33 : string;
attribute INIT_34 : string;
attribute INIT_35 : string;
attribute INIT_36 : string;
attribute INIT_37 : string;
attribute INIT_38 : string;
attribute INIT_39 : string;
attribute INIT_3A : string;
attribute INIT_3B : string;
attribute INIT_3C : string;
attribute INIT_3D : string;
attribute INIT_3E : string;
attribute INIT_3F : string;

-- XST synthesis initialization code VROM
-- Created by multigenHD_romgen.v
-- Video format mapping:
--   000 =  SMPTE 295M - 1080i  25Hz (1250 lines/frame)
--   001 =  SMPTE 274M - 1080sF 24Hz & 23.98Hz         
--   002 =  SMPTE 274M - 1080i  30Hz & 29.97 Hz        
--   003 =  SMPTE 274M - 1080i  25Hz                   
--   004 =  SMPTE 274M - 1080p  30Hz & 29.97Hz         
--   005 =  SMPTE 274M - 1080p  25Hz                   
--   006 =  SMPTE 274M - 1080p  24Hz & 23.98Hz         
--   007 =  SMPTE 296M - 720p   60Hz & 59.94Hz         
attribute INIT of VROM : label is "00048FFFF";
attribute SRVAL of VROM : label is "00048FFFF";
attribute WRITE_MODE of VROM : label is "READ_FIRST";
attribute INITP_00 of VROM : label is "0000000000000000000000000000000000000000000000000000000000000000";
attribute INITP_01 of VROM : label is "0000000000000000000000000000000000000000000000000000000000000000";
attribute INITP_02 of VROM : label is "0000000000000000000000000000000000000000000000000000000000000000";
attribute INITP_03 of VROM : label is "0000000000000000000000000000000000000000000000000000000000000000";
attribute INITP_04 of VROM : label is "0000000000000000000000000000000000000000000000000000000000000000";
attribute INITP_05 of VROM : label is "0000000000000000000000000000000000000000000000000000000000000000";
attribute INITP_06 of VROM : label is "0000000000000000000000000000000000000000000000000000000000000000";
attribute INITP_07 of VROM : label is "0000000000000000000000000000000000000000000000000000000000000000";
attribute INIT_00 of VROM : label is "0018582800084E2702044D8602033CA5020237040201316302810A2200080A01";
attribute INIT_01 of VROM : label is "00089C4000089C4000589C5000189C2D02149BAC02138ACB0212852A02117F89";
attribute INIT_02 of VROM : label is "0038583800089C4000284E3702244D9502233CB4022237130221317200280A11";
attribute INIT_03 of VROM : label is "0008002000089C4000789C4000389C3D02349BBC02338ADB0232853A02317F99";
attribute INIT_04 of VROM : label is "0018582800084E2702040006020300050C064D860A052BC30A850A2200080A01";
attribute INIT_05 of VROM : label is "00089C4000089C4000589C5000189C2D0214000C0213000B0C169BAC0A1579E9";
attribute INIT_06 of VROM : label is "0038583800089C4000284E3702240015022300140C264D950A252BD200280A11";
attribute INIT_07 of VROM : label is "0008002000089C4000789C4000389C3D0234001C0233001B0C369BBC0A3579F9";
attribute INIT_08 of VROM : label is "001848E800084667020446060203352502022F84020129E3028102A200080281";
attribute INIT_09 of VROM : label is "00088CA000088CA000588CB000188C8D02148C6C02137B8B021275EA02117049";
attribute INIT_0A of VROM : label is "003848F800088CA000284677022446150223353402222F93022129F200280291";
attribute INIT_0B of VROM : label is "0008002000088CA000788CA000388C9D02348C7C02337B9B023275FA02317059";
attribute INIT_0C of VROM : label is "001848E80008466702040006020300050C0646060A0524430A8502A200080281";
attribute INIT_0D of VROM : label is "00088CA000088CA000588CB000188C8D0214000C0213000B0C168C6C0A156AA9";
attribute INIT_0E of VROM : label is "003848F800088CA00028467702240015022300140C2646150A25245200280291";
attribute INIT_0F of VROM : label is "0008002000088CA000788CA000388C9D0234001C0233001B0C368C7C0A356AB9";
attribute INIT_10 of VROM : label is "001848E800084667020446060203352502022F84020129E3028102A200080281";
attribute INIT_11 of VROM : label is "00088CA000088CA000588CB000188C8D02148C6C02137B8B021275EA02117049";
attribute INIT_12 of VROM : label is "003848F800088CA000284677022446150223353402222F93022129F200280291";
attribute INIT_13 of VROM : label is "0008002000088CA000788CA000388C9D02348C7C02337B9B023275FA02317059";
attribute INIT_14 of VROM : label is "001848E80008466702040006020300050C0646060A0524430A8502A200080281";
attribute INIT_15 of VROM : label is "00088CA000088CA000588CB000188C8D0214000C0213000B0C168C6C0A156AA9";
attribute INIT_16 of VROM : label is "003848F800088CA00028467702240015022300140C2646150A25245200280291";
attribute INIT_17 of VROM : label is "0008002000088CA000788CA000388C9D0234001C0233001B0C368C7C0A356AB9";
attribute INIT_18 of VROM : label is "001848E800084667020446060203352502022F84020129E3028102A200080281";
attribute INIT_19 of VROM : label is "00088CA000088CA000588CB000188C8D02148C6C02137B8B021275EA02117049";
attribute INIT_1A of VROM : label is "003848F800088CA000284677022446150223353402222F93022129F200280291";
attribute INIT_1B of VROM : label is "0008002000088CA000788CA000388C9D02348C7C02337B9B023275FA02317059";
attribute INIT_1C of VROM : label is "001848E80008466702040006020300050C0646060A0524430A8502A200080281";
attribute INIT_1D of VROM : label is "00088CA000088CA000588CB000188C8D0214000C0213000B0C168C6C0A156AA9";
attribute INIT_1E of VROM : label is "003848F800088CA00028467702240015022300140C2646150A25245200280291";
attribute INIT_1F of VROM : label is "0008002000088CA000788CA000388C9D0234001C0233001B0C368C7C0A356AB9";
attribute INIT_20 of VROM : label is "0018000800088C8D02048C2602036A6502025F24020153E30281054200080521";
attribute INIT_21 of VROM : label is "00088CA000088CA000488CB000188C8D0214000C0213000B0212000A02110009";
attribute INIT_22 of VROM : label is "0038001800088CA000288C9D02248C3502236A7402225F33022153F200280531";
attribute INIT_23 of VROM : label is "0008002000088CA000688CA000388C9D0234001C0233001B0232001A02310019";
attribute INIT_24 of VROM : label is "0018000800088C8D02040006020300050C068C260A0548A30A85054200080521";
attribute INIT_25 of VROM : label is "00088CA000088CA000488CB000188C8D0214000C0213000B0C16000C0A150009";
attribute INIT_26 of VROM : label is "0038001800088CA000288C9D02240015022300140C268C350A2548B200280531";
attribute INIT_27 of VROM : label is "0008002000088CA000688CA000388C9D0234001C0233001B0C36001C0A350019";
attribute INIT_28 of VROM : label is "0018000800088C8D02048C2602036A6502025F24020153E30281054200080521";
attribute INIT_29 of VROM : label is "00088CA000088CA000488CB000188C8D0214000C0213000B0212000A02110009";
attribute INIT_2A of VROM : label is "0038001800088CA000288C9D02248C3502236A7402225F33022153F200280531";
attribute INIT_2B of VROM : label is "0008002000088CA000688CA000388C9D0234001C0233001B0232001A02310019";
attribute INIT_2C of VROM : label is "0018000800088C8D02040006020300050C068C260A0548A30A85054200080521";
attribute INIT_2D of VROM : label is "00088CA000088CA000488CB000188C8D0214000C0213000B0C16000C0A150009";
attribute INIT_2E of VROM : label is "0038001800088CA000288C9D02240015022300140C268C350A2548B200280531";
attribute INIT_2F of VROM : label is "0008002000088CA000688CA000388C9D0234001C0233001B0C36001C0A350019";
attribute INIT_30 of VROM : label is "0018000800088C8D02048C2602036A6502025F24020153E30281054200080521";
attribute INIT_31 of VROM : label is "00088CA000088CA000488CB000188C8D0214000C0213000B0212000A02110009";
attribute INIT_32 of VROM : label is "0038001800088CA000288C9D02248C3502236A7402225F33022153F200280531";
attribute INIT_33 of VROM : label is "0008002000088CA000688CA000388C9D0234001C0233001B0232001A02310019";
attribute INIT_34 of VROM : label is "0018000800088C8D02040006020300050C068C260A0548A30A85054200080521";
attribute INIT_35 of VROM : label is "00088CA000088CA000488CB000188C8D0214000C0213000B0C16000C0A150009";
attribute INIT_36 of VROM : label is "0038001800088CA000288C9D02240015022300140C268C350A2548B200280531";
attribute INIT_37 of VROM : label is "0008002000088CA000688CA000388C9D0234001C0233001B0C36001C0A350019";
attribute INIT_38 of VROM : label is "0118000801085DAD03045D26030346A503023F24030137A30381034201080321";
attribute INIT_39 of VROM : label is "01085DC001085DC001485DD001185DAD0314000C0313000B0312000A03110009";
attribute INIT_3A of VROM : label is "0138001801085DC001285DBD03245D35032346B403223F33032137B201280331";
attribute INIT_3B of VROM : label is "0108002001085DC001685DC001385DBD0334001C0333001B0332001A03310019";
attribute INIT_3C of VROM : label is "0118000801085DAD03040006030300050D065D260B0530230B85034201080321";
attribute INIT_3D of VROM : label is "01085DC001085DC001485DD001185DAD0314000C0313000B0D16000C0B150009";
attribute INIT_3E of VROM : label is "0138001801085DC001285DBD03240015032300140D265D350B25303201280331";
attribute INIT_3F of VROM : label is "0108002001085DC001685DC001385DBD0334001C0333001B0D36001C0B350019";

begin


    ------------------------------------------------------------------------------
    -- Vertical section
    --
    vrom_addr <= (std & pattern(0) & v_region);
    vrom_en <= (ce and v_inc and h_counter_lsb and v_evnt_match) or rst;

    VROM : RAMB16_S36 
        -- Simulation initialization code VROM
        -- Created by multigenHD_romgen.v
        -- Video format mapping:
        --   000 =  SMPTE 295M - 1080i  25Hz (1250 lines/frame)
        --   001 =  SMPTE 274M - 1080sF 24Hz & 23.98Hz         
        --   002 =  SMPTE 274M - 1080i  30Hz & 29.97 Hz        
        --   003 =  SMPTE 274M - 1080i  25Hz                   
        --   004 =  SMPTE 274M - 1080p  30Hz & 29.97Hz         
        --   005 =  SMPTE 274M - 1080p  25Hz                   
        --   006 =  SMPTE 274M - 1080p  24Hz & 23.98Hz         
        --   007 =  SMPTE 296M - 720p   60Hz & 59.94Hz         
        -- pragma translate_off
        generic map (
        INIT       => X"00048FFFF",
        SRVAL      => X"00048FFFF",
        WRITE_MODE => "READ_FIRST",
        INITP_00 => X"0000000000000000000000000000000000000000000000000000000000000000",
        INITP_01 => X"0000000000000000000000000000000000000000000000000000000000000000",
        INITP_02 => X"0000000000000000000000000000000000000000000000000000000000000000",
        INITP_03 => X"0000000000000000000000000000000000000000000000000000000000000000",
        INITP_04 => X"0000000000000000000000000000000000000000000000000000000000000000",
        INITP_05 => X"0000000000000000000000000000000000000000000000000000000000000000",
        INITP_06 => X"0000000000000000000000000000000000000000000000000000000000000000",
        INITP_07 => X"0000000000000000000000000000000000000000000000000000000000000000",
        INIT_00 => X"0018582800084E2702044D8602033CA5020237040201316302810A2200080A01",
        INIT_01 => X"00089C4000089C4000589C5000189C2D02149BAC02138ACB0212852A02117F89",
        INIT_02 => X"0038583800089C4000284E3702244D9502233CB4022237130221317200280A11",
        INIT_03 => X"0008002000089C4000789C4000389C3D02349BBC02338ADB0232853A02317F99",
        INIT_04 => X"0018582800084E2702040006020300050C064D860A052BC30A850A2200080A01",
        INIT_05 => X"00089C4000089C4000589C5000189C2D0214000C0213000B0C169BAC0A1579E9",
        INIT_06 => X"0038583800089C4000284E3702240015022300140C264D950A252BD200280A11",
        INIT_07 => X"0008002000089C4000789C4000389C3D0234001C0233001B0C369BBC0A3579F9",
        INIT_08 => X"001848E800084667020446060203352502022F84020129E3028102A200080281",
        INIT_09 => X"00088CA000088CA000588CB000188C8D02148C6C02137B8B021275EA02117049",
        INIT_0A => X"003848F800088CA000284677022446150223353402222F93022129F200280291",
        INIT_0B => X"0008002000088CA000788CA000388C9D02348C7C02337B9B023275FA02317059",
        INIT_0C => X"001848E80008466702040006020300050C0646060A0524430A8502A200080281",
        INIT_0D => X"00088CA000088CA000588CB000188C8D0214000C0213000B0C168C6C0A156AA9",
        INIT_0E => X"003848F800088CA00028467702240015022300140C2646150A25245200280291",
        INIT_0F => X"0008002000088CA000788CA000388C9D0234001C0233001B0C368C7C0A356AB9",
        INIT_10 => X"001848E800084667020446060203352502022F84020129E3028102A200080281",
        INIT_11 => X"00088CA000088CA000588CB000188C8D02148C6C02137B8B021275EA02117049",
        INIT_12 => X"003848F800088CA000284677022446150223353402222F93022129F200280291",
        INIT_13 => X"0008002000088CA000788CA000388C9D02348C7C02337B9B023275FA02317059",
        INIT_14 => X"001848E80008466702040006020300050C0646060A0524430A8502A200080281",
        INIT_15 => X"00088CA000088CA000588CB000188C8D0214000C0213000B0C168C6C0A156AA9",
        INIT_16 => X"003848F800088CA00028467702240015022300140C2646150A25245200280291",
        INIT_17 => X"0008002000088CA000788CA000388C9D0234001C0233001B0C368C7C0A356AB9",
        INIT_18 => X"001848E800084667020446060203352502022F84020129E3028102A200080281",
        INIT_19 => X"00088CA000088CA000588CB000188C8D02148C6C02137B8B021275EA02117049",
        INIT_1A => X"003848F800088CA000284677022446150223353402222F93022129F200280291",
        INIT_1B => X"0008002000088CA000788CA000388C9D02348C7C02337B9B023275FA02317059",
        INIT_1C => X"001848E80008466702040006020300050C0646060A0524430A8502A200080281",
        INIT_1D => X"00088CA000088CA000588CB000188C8D0214000C0213000B0C168C6C0A156AA9",
        INIT_1E => X"003848F800088CA00028467702240015022300140C2646150A25245200280291",
        INIT_1F => X"0008002000088CA000788CA000388C9D0234001C0233001B0C368C7C0A356AB9",
        INIT_20 => X"0018000800088C8D02048C2602036A6502025F24020153E30281054200080521",
        INIT_21 => X"00088CA000088CA000488CB000188C8D0214000C0213000B0212000A02110009",
        INIT_22 => X"0038001800088CA000288C9D02248C3502236A7402225F33022153F200280531",
        INIT_23 => X"0008002000088CA000688CA000388C9D0234001C0233001B0232001A02310019",
        INIT_24 => X"0018000800088C8D02040006020300050C068C260A0548A30A85054200080521",
        INIT_25 => X"00088CA000088CA000488CB000188C8D0214000C0213000B0C16000C0A150009",
        INIT_26 => X"0038001800088CA000288C9D02240015022300140C268C350A2548B200280531",
        INIT_27 => X"0008002000088CA000688CA000388C9D0234001C0233001B0C36001C0A350019",
        INIT_28 => X"0018000800088C8D02048C2602036A6502025F24020153E30281054200080521",
        INIT_29 => X"00088CA000088CA000488CB000188C8D0214000C0213000B0212000A02110009",
        INIT_2A => X"0038001800088CA000288C9D02248C3502236A7402225F33022153F200280531",
        INIT_2B => X"0008002000088CA000688CA000388C9D0234001C0233001B0232001A02310019",
        INIT_2C => X"0018000800088C8D02040006020300050C068C260A0548A30A85054200080521",
        INIT_2D => X"00088CA000088CA000488CB000188C8D0214000C0213000B0C16000C0A150009",
        INIT_2E => X"0038001800088CA000288C9D02240015022300140C268C350A2548B200280531",
        INIT_2F => X"0008002000088CA000688CA000388C9D0234001C0233001B0C36001C0A350019",
        INIT_30 => X"0018000800088C8D02048C2602036A6502025F24020153E30281054200080521",
        INIT_31 => X"00088CA000088CA000488CB000188C8D0214000C0213000B0212000A02110009",
        INIT_32 => X"0038001800088CA000288C9D02248C3502236A7402225F33022153F200280531",
        INIT_33 => X"0008002000088CA000688CA000388C9D0234001C0233001B0232001A02310019",
        INIT_34 => X"0018000800088C8D02040006020300050C068C260A0548A30A85054200080521",
        INIT_35 => X"00088CA000088CA000488CB000188C8D0214000C0213000B0C16000C0A150009",
        INIT_36 => X"0038001800088CA000288C9D02240015022300140C268C350A2548B200280531",
        INIT_37 => X"0008002000088CA000688CA000388C9D0234001C0233001B0C36001C0A350019",
        INIT_38 => X"0118000801085DAD03045D26030346A503023F24030137A30381034201080321",
        INIT_39 => X"01085DC001085DC001485DD001185DAD0314000C0313000B0312000A03110009",
        INIT_3A => X"0138001801085DC001285DBD03245D35032346B403223F33032137B201280331",
        INIT_3B => X"0108002001085DC001685DC001385DBD0334001C0333001B0332001A03310019",
        INIT_3C => X"0118000801085DAD03040006030300050D065D260B0530230B85034201080321",
        INIT_3D => X"01085DC001085DC001485DD001185DAD0314000C0313000B0D16000C0B150009",
        INIT_3E => X"0138001801085DC001285DBD03240015032300140D265D350B25303201280331",
        INIT_3F => X"0108002001085DC001685DC001385DBD0334001C0333001B0D36001C0B350019"
        )
        -- pragma translate_on
        port map (
            DO      => vrom_out,
            DOP     => open,
            ADDR    => vrom_addr,
            CLK     => clk,
            DI      => GND32,
            DIP     => GND4,
            EN      => vrom_en,
            SSR     => rst,
            WE      => GND
        );

    v_region        <= vrom_out(4 downto 0);
    v_next_evnt     <= vrom_out(15 downto 5);
    v_band_rom      <= vrom_out(18 downto 16);
    v               <= vrom_out(19);
    f               <= vrom_out(20);
    v_clr           <= vrom_out(22);
    first_line      <= vrom_out(23);
    y_ramp_inc_sel  <= vrom_out(24);
    v_band_75_rom   <= vrom_out(27 downto 25);

    -- 
    -- Vertical counter
    --
    -- the vertical counter increments once per line. When the v_clr signal
    -- is asserted the counter resets to a value of 1.
    --
    process(clk, rst)
    begin
        if rst = '1' then
            v_counter <= (others => '1');
        elsif clk'event and clk = '1' then
            if ce = '1' and h_counter_lsb ='1' then
                if v_inc = '1' then
                    if v_clr = '1' then
                        v_counter <= (0 => '1', others => '0');
                    else
                        v_counter <= v_counter + 1;
                    end if;
                end if;
            end if;
        end if;
    end process;

    line_num <= v_counter;

    --
    -- Vertical event comparator
    --
    -- This logic compares the current vertical counter value with the 
    -- v_next_evnt field from the VROM. When they match, v_evnt_match is
    -- asserted to enable clock of the VROM.
    --
    v_evnt_match <= '1' when v_next_evnt = v_counter or v_clr = '1' else '0';

    --
    -- v_band MUX
    --
    -- When 75% color bars are being generated use the v_band_75_rom bits
    -- otherwise use v_band_rom.
    --
    v_band <= v_band_75_rom when pattern(1) = '1' else v_band_rom;
                    
end synth;