-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: trs_detect_v2.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2005-01-13 15:14:47-07 $
-- /___/   /\    Date Created: Jan 5, 2005
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: trs_detect_v2.vhd,rcs $
-- Revision 1.2  2005-01-13 15:14:47-07  jsnow
-- XAPP580 version 1.0 release.
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
-- This module examines the input video stream for TRS symbols and ancillary data
-- packets. It does some decoding of the TRS symbols and ANC packets to generate
-- a variety of outputs.
-- 
-- This input video stream is passed through a four register pipeline, delaying 
-- the video by four clock cycles. This allows the pipeline to contain the 
-- entire TRS symbol or the ancillary data flag plus DID word to allow them to 
-- be decoded before the video emerges from the module.
-- 
-- This module has the following inputs:
-- 
-- clk: clock input
-- 
-- ce: clock enable
-- 
-- rst: asynchronous reset input
-- 
-- vid_in: input video stream port
-- 
-- This module generates the following outputs:
-- 
-- vid_out: This is the output video stream. It is identical to the input video
-- stream, but delayed by four clock cycles.
-- 
-- rx_trs: This output is asserted only when the first word of a TRS symbol is
-- present on vid_out.
-- 
-- rx_eav: This output is asserted only when the first word of an EAV symbol is
-- present on vid_out.
-- 
-- rx_sav: This output is asserted only when the first word of an SAV symbol is
-- present on vid_out.
-- 
-- rx_f: This is the field indicator bit F latched from the XYZ word of the last
-- received TRS symbol.
-- 
-- rx_v: This is the vertical blanking interval bit V latched from the XYZ word 
-- of the last received TRS symbol.
-- 
-- rx_h: This is the horizontal blanking interval bit H latched from the XYZ 
-- word of the last received TRS symbol.
-- 
-- rx_xyz: This output is asserted when the XYZ word of a TRS symbol is present
-- on vid_out.
-- 
-- rx_xyz_err: This output is asserted when the received XYZ word contains an
-- error. It is only asserted when the XYZ word appears on vid_out. This signal 
-- is only valid for the 4:2:2 video standards.
-- 
-- rx_xyz_err_4444: This output is asserted when the received XYZ word contains 
-- an error. It is only asserted when the XYZ word appears on vid_out. This 
-- signals is only valid for the 4:4:4:4 video standards.
-- 
-- rx_anc: This output is asserted when the first word of an ANC packet (the 
-- first word of the ancillary data flag) is present on vid_out.
-- 
-- rx_edh: This output is asserted when the first word of an EDH packet (the 
-- first word of the ancillary data flag) is present on vid_out.
-- 
-- Note: This version contains revisions to make the design smaller. The design
-- was made smaller by removing the output register and adding, in its place to 
-- keep the latency the same, another input register for the pipe. Since the
-- output register required many FFs for various decoded signals, moving the
-- pipeline stage to the input, where only 10-bits of video needs to be 
-- registered, saves about 5 flip-flops (some flip-flops get added by internal 
-- registers).Also, the all_zeros and all_ones decoding has been implemented as 
-- LUTs with MUXCY elements. Overall, the net effect is 5 less FFs and 5 less 
-- LUTs, but the design can have a better packing factor, resulting in even 
-- better savings.
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

library unisim; 
use unisim.vcomponents.all; 

entity trs_detect is
port (
    clk :           in  std_logic;              -- clock input
    ce :            in  std_logic;              -- clock enable
    rst :           in  std_logic;              -- async reset input
    vid_in :        in                          -- video input
                    std_logic_vector(9 downto 0);
    vid_out :       out                         -- delayed & clipped video out
                    std_logic_vector(9 downto 0);
    rx_trs :        out std_logic;              -- asserted during first word of TRS
    rx_eav :        out std_logic;              -- asserted during first word of EAV
    rx_sav :        out std_logic;              -- asserted during first word of SAV
    rx_f :          out std_logic;              -- field bit from last received TRS
    rx_v :          out std_logic;              -- v blank bit from last received TRS
    rx_h :          out std_logic;              -- h blank bit from last received TRS
    rx_xyz :        out std_logic;              -- asserted during TRS XYZ word
    rx_xyz_err :    out std_logic;              -- XYZ error flag for non-4444 standards
    rx_xyz_err_4444:out std_logic;              -- XYZ error flag for 4444 standards
    rx_anc :        out std_logic;              -- asserted during first word of ADF
    rx_edh :        out std_logic);             -- asserted during first word of ADF
                                                --   if this is an EDH packet
end trs_detect;

architecture synth of trs_detect is

attribute init : string;
attribute init of A1LUTA : label is "8000";
attribute init of A1LUTB : label is "8000";
attribute init of A0LUTA : label is "0001";
attribute init of A0LUTB : label is "0001";

         
-------------------------------------------------------------------------------
-- Signal definitions
--

signal in_reg :         std_logic_vector(9 downto 0)    -- input register
                            := (others => '0');
signal pipe0_vid :      std_logic_vector(9 downto 0)    -- pipeline register 0 
                            := (others => '0');
signal pipe1_vid :      std_logic_vector(9 downto 0)    -- pipeline register 1
                            := (others => '0');
signal pipe1_ones :     std_logic := '0';               -- 1 if pipe1_vid(9:2) is all 1s
signal pipe1_zeros :    std_logic := '0';               -- 1 if pipe1_vid(9:2) is all 0s
signal pipe2_vid :      std_logic_vector(9 downto 0)    -- pipeline register 2
                            := (others => '0');
signal pipe2_ones :     std_logic := '0';               -- 1 if pipe2_vid(9:2) is all 1s
signal pipe2_zeros :    std_logic := '0';               -- 1 if pipe2_vid(9:2) is all 0s
signal xyz :            std_logic;                      -- XYZ detect signal
signal xyz_err :        std_logic;                      -- XYZ error detect signal
signal xyz_err_4444 :   std_logic;                      -- XYZ error signal for 4444 formats
signal anc :            std_logic;                      -- internal ANC signal
signal trs :            std_logic;                      -- internal TRS signal
signal eav :            std_logic;                      -- internal EAV signal
signal sav :            std_logic;                      -- internal SAV signal
signal edh_in :         std_logic;                      -- 1 when in_reg = 0x1f4
signal all_ones_in :    std_logic;                      -- 1 if pipe0_vid(9:2) is all 1s
signal all_zeros_in :   std_logic;                      -- 1 if pipe0_vid(9:2) is all 0s
signal trs_delay :      std_logic_vector(2 downto 0)    -- delay reg used to generate xyz
                            := (others => '0');
signal f :              std_logic;                      -- internal F signal
signal v :              std_logic;                      -- internal V signal
signal h :              std_logic;                      -- internal H signal
signal f_reg :          std_logic := '0';               -- F bit storage
signal v_reg :          std_logic := '0';               -- V bit storage
signal h_reg :          std_logic := '0';               -- H bit storage
signal all_ones_luta :  std_logic;                      
signal all_ones_lutb :  std_logic;
signal all_ones_muxcya :std_logic;
signal all_zeros_luta : std_logic;
signal all_zeros_lutb : std_logic;
signal all_zeros_muxcya:std_logic;

begin

--
-- in_reg
--
-- The input register loads the value on the vid_in port.
--
process(clk)
begin
    if clk'event and clk='1' then
        if ce='1' then
            in_reg <= vid_in;
        end if;
    end if;
end process;

-- 
-- pipe0_vid
--
process(clk)
begin
    if clk'event and clk='1' then
        if ce='1' then
            pipe0_vid <= in_reg;
        end if;
    end if;
end process;

--
-- all ones and all zeros detectors
--
-- This logic determines if the input video word is all ones or all zeros. To
-- provide compatibility with 8-bit video equipment, the LS two bits are
-- ignored.  
-- 
A1LUTA : LUT4
-- synthesis translate_off
generic map (INIT => X"8000")
-- synthesis translate_on
port map (
    O   => all_ones_luta,
    I0  => pipe0_vid(9),
    I1  => pipe0_vid(8),
    I2  => pipe0_vid(7),
    I3  => pipe0_vid(6));
-- Output of this LUT is asserted only if all inputs are 1

A1LUTB : LUT4
-- synthesis translate_off
generic map (INIT => X"8000")
-- synthesis translate_on
port map (
    O   => all_ones_lutb,
    I0  => pipe0_vid(5),
    I1  => pipe0_vid(4),
    I2  => pipe0_vid(3),
    I3  => pipe0_vid(2));
-- Output of this LUT is asserted only if all inputs are 1

A1MUXA : MUXCY_L
port map (
    LO  => all_ones_muxcya,
    CI  => '1',
    DI  => '0',
    S   => all_ones_lutb);

A1MUXB : MUXCY_L
port map (
    LO  => all_ones_in,
    CI  => all_ones_muxcya,
    DI  => '0',
    S   => all_ones_luta);


A0LUTA : LUT4
-- synthesis translate_off
generic map (INIT => X"0001")
-- synthesis translate_on
port map (
    O   => all_zeros_luta,
    I0  => pipe0_vid(9),
    I1  => pipe0_vid(8),
    I2  => pipe0_vid(7),
    I3  => pipe0_vid(6));
-- Output of this lUT is asserted only if all inputs are 0

A0LUTB : LUT4
-- synthesis translate_off
generic map (INIT => X"0001")
-- synthesis translate_on
port map (
    O   => all_zeros_lutb,
    I0  => pipe0_vid(5),
    I1  => pipe0_vid(4),
    I2  => pipe0_vid(3),
    I3  => pipe0_vid(2));
-- Output of this LUT is asserted only if all inputs are 0

A0MUXA : MUXCY_L
port map (
    LO  => all_zeros_muxcya,
    CI  => '1',
    DI  => '0',
    S   => all_zeros_lutb);

A0MUXB : MUXCY_L
port map (
    LO  => all_zeros_in,
    CI  => all_zeros_muxcya,
    DI  => '0',
    S   => all_zeros_luta);


--
-- DID detector decoder
--
-- The edh_in signal is asserted if the in_reg contains a value of 0x1f4.
-- This is the value of the DID word for an EDH packet. 
--
edh_in <= '1' when in_reg = "0111110100" else '0';

--
-- pipe1
--
-- The pipe1 register holds the input video and the outputs of the all zeros
-- and all ones detectors.
--
process(clk)
begin
    if clk'event and clk='1' then
        if ce='1' then
            pipe1_vid   <= pipe0_vid;
            pipe1_ones  <= all_ones_in;
            pipe1_zeros <= all_zeros_in;
        end if;
    end if;
end process;

--
-- pipe2_reg
--
-- The pipe2 register delays the contents of the pipe1 register for one more
-- clock cycle.
--
process(clk)
begin
    if clk'event and clk='1' then
        if ce='1' then
            pipe2_vid   <= pipe1_vid;
            pipe2_ones  <= pipe1_ones;
            pipe2_zeros <= pipe1_zeros;
        end if;
    end if;
end process;


--
-- TRS & ANC detector
--
-- The trs signal when the sequence 3ff, 000, 000 is stored in the pipe2, pipe1,
-- and in_reg registers, respectively. The anc signal is asserted when these
-- same registers hold the sequence 000, 3ff, 3ff.
--
trs <= all_zeros_in and pipe1_zeros and pipe2_ones;
anc <= all_ones_in and pipe1_ones and pipe2_zeros;
eav <= trs and in_reg(6);
sav <= trs and not in_reg(6);

--
-- f, v, and h flag generation
--
f <= in_reg(8) when trs='1' else f_reg;
v <= in_reg(7) when trs='1' else v_reg;
h <= in_reg(6) when trs='1' else h_reg;

process(clk, rst)
begin
    if rst='1' then
        f_reg <= '0';
        v_reg <= '0';
        h_reg <= '0';
    elsif clk'event and clk='1' then
        if ce='1' then
            f_reg <= f;
            v_reg <= v;
            h_reg <= h;
        end if;
    end if;
end process;

--
-- XYZ and XYZ error logic
--
-- The xyz signal is asserted when the pipe2 register holds the XYZ word of a
-- TRS symbol. The xyz_err signal is asserted if an error is detected in the
-- format of the XYZ word stored in pipe2. This signal is not valid for the
-- 4444 component digital video formats. The xyz_err_4444 signal is asserted
-- for XYZ word format errors.
--
xyz <= trs_delay(2);

xyz_err <= 
    trs_delay(2) and 
    ((pipe2_vid(5) xor pipe2_vid(7) xor pipe2_vid(6)) or                 -- P3 = V xor H
     (pipe2_vid(4) xor pipe2_vid(8) xor pipe2_vid(6)) or                 -- P2 = F xor H
     (pipe2_vid(3) xor pipe2_vid(8) xor pipe2_vid(7)) or                 -- P1 = F xor V
     (pipe2_vid(2) xor pipe2_vid(8) xor pipe2_vid(7) xor pipe2_vid(6)) or  -- P0 = F xor V xor H
     not pipe2_vid(9));

xyz_err_4444 <= 
    trs_delay(2) and
    ((pipe2_vid(4) xor pipe2_vid(8) xor pipe2_vid(7) xor pipe2_vid(6)) or  -- P4 = F xor V xor H
     (pipe2_vid(3) xor pipe2_vid(8) xor pipe2_vid(7) xor pipe2_vid(5)) or  -- P3 = F xor V xor S
     (pipe2_vid(2) xor pipe2_vid(7) xor pipe2_vid(6) xor pipe2_vid(5)) or  -- P2 = V xor H xor S
     (pipe2_vid(1) xor pipe2_vid(8) xor pipe2_vid(6) xor pipe2_vid(5)) or  -- P1 = F xor H xor S
     not pipe2_vid(9));

process(clk,rst)
begin
    if rst='1' then
        rx_xyz_err <= '0';
    elsif clk'event and clk='1' then
        if ce='1' then
            rx_xyz_err <= xyz_err;
        end if;
    end if;
end process;

process(clk,rst)
begin
    if rst='1' then
        rx_xyz_err_4444 <= '0';
    elsif clk'event and clk='1' then
        if ce='1' then
            rx_xyz_err_4444 <= xyz_err_4444;
        end if;
    end if;
end process;

--
-- trs_delay register
--
-- Used to assert the xyz signal when pipe2 contains the XYZ word of a TRS
-- symbol.
process(clk,rst)
begin
    if rst='1' then
        trs_delay <= (others => '0');
    elsif clk'event and clk='1' then
        if ce='1' then
            trs_delay <= (trs_delay(1 downto 0) & trs);
        end if;
    end if;
end process;

--
-- assign the outputs
--
vid_out <= pipe2_vid;
rx_trs <= trs;
rx_eav <= eav;
rx_sav <= sav;
rx_anc <= anc;
rx_xyz <= xyz;
rx_edh <= anc and edh_in;
rx_f <= f;
rx_v <= v;
rx_h <= h;
      
end synth;