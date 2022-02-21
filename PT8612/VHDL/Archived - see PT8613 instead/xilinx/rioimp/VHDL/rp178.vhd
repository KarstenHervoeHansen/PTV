-------------------------------------------------------------------------------- 
-- Copyright (c) 2002 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: rp178.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2004-08-27 13:19:18-06 $
-- /___/   /\    Date Created: January 7, 2002 
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: rp178.vhd,rcs $
-- Revision 1.0  2004-08-27 13:19:18-06  jsnow
-- Initial version.
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
-- This video test pattern generator module generates the SMPTE RP 178 specified
-- SDI Checkfield for testing the SMPTE SDI interface. The RP 178 test pattern
-- is made up of two different test patterns: a cable equalization test pattern
-- that stresses the SDI recevier cable equalization circuit with bitstream
-- waveforms with a high amount of DC offset and a PLL test pattern that 
-- stresses the SDI receiver's clock and data recovery PLL's ability to stay 
-- locked with  a bitstream waveform that contains a minimum number of bit 
-- transitions.
-- 
-- The equalization test pattern is generated during the active video portion of
-- the first half of each video field. The PLL test pattern is generated during
-- the active video portion of the second half of each video field.
-- 
-- During the cable equalization test pattern, all chroma samples have a value 
-- of hex 300 and all luma samples have a value of hex 198. When encoded by an 
-- SDI encoder, this will occasionally (when the encoder starting condition is 
-- just right) produce a repeating encoded bitstream waveform with 19 bits of 
-- one polarity followed by a single bit of the opposite polarity. To insure 
-- that both polarities of this pattern occur, the entire video frame must 
-- contain an odd number of 1 bits. To insure this, the last active luma sample 
-- on the first line of the first field contains the value hex 080 rather than 
-- hex 198.
-- 
-- During the PLL test pattern, all chroma samples have a value of hex 200 and 
-- all luma samples have value of hex 110. When encoded by an SDI encoder, this 
-- will occasionally produce a repeating encoded bitstream waveform with 20 high
-- bits and 20 low bits -- a low frequency pattern that tests the PLL's ability 
-- to stay locked.
-- 
-- The above values are the 10-bit values that SDI requires. The video values in
-- this module are actually only generated as 8-bit values and are scaled up to
-- the bit width specified by the VID_WIDTH parameter by filling the LS bits 
-- with zeros. This does produces the correct RP 178 values since all 10-bit 
-- RP 178 pattern values have their LS two bits zeroed.
--
-------------------------------------------------------------------------------- 

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

entity rp178_vhd is
    generic (
        VID_WIDTH : integer := 10
    );
    port (
        clk:        in  std_ulogic;                                 -- clock input
        rst:        in  std_ulogic;                                 -- async reset input
        ce:         in  std_ulogic;                                 -- clock enable input
        q:          out std_ulogic_vector(VID_WIDTH - 1 downto 0);  -- video output
        h_sync:     out std_ulogic;                                 -- horizontal sync out
        v_sync:     out std_ulogic;                                 -- vertical sync out
        field:      out std_ulogic                                  -- field output
    );
end rp178_vhd;

architecture synth of rp178_vhd is

-------------------------------------------------------------------------------
-- Constant Definitions
--

constant HCNT_WIDTH :   integer := 11;                              -- Width of h_counter
constant VCNT_WIDTH :   integer := 10;                              -- Width of v_counter
constant HRGN_WIDTH :   integer := 3;                               -- Width of h_region counter
constant VRGN_WIDTH :   integer := 4;                               -- Width of v_region counter
constant VBAND_WIDTH :  integer := 2;                               -- Width of vband code
constant HBAR_WIDTH :   integer := 2;                               -- Width of hbar code

constant V_EVNT_WIDTH : integer := VCNT_WIDTH;                      -- Width of v_next_evnt
constant H_EVNT_WIDTH : integer := HCNT_WIDTH - 2;                  -- Width of h_next_evnt
constant VROM_WIDTH :   integer := V_EVNT_WIDTH + VBAND_WIDTH + 3;  -- Width of v_rom
constant HROM_WIDTH :   integer := H_EVNT_WIDTH + HBAR_WIDTH + 4;   -- Width of h_rom
        
constant VID_MSB :      integer := VID_WIDTH - 1;                   -- MS bit # of video data path
constant HCNT_MSB :     integer := HCNT_WIDTH - 1;                  -- MS bit # of h_counter
constant VCNT_MSB :     integer := VCNT_WIDTH - 1;                  -- MS bit # of v_counter
constant HRGN_MSB :     integer := HRGN_WIDTH - 1;                  -- MS bit # of h_region counter
constant VRGN_MSB :     integer := VRGN_WIDTH - 1;                  -- MS bit # of v_region counter
constant VBAND_MSB :    integer := VBAND_WIDTH - 1;                 -- MS bit # of vband code
constant HBAR_MSB :     integer := HBAR_WIDTH - 1;                  -- MS bit # of hbar code
constant V_EVNT_MSB :   integer := V_EVNT_WIDTH - 1;                -- MS bit # of v_next_evnt
constant H_EVNT_MSB :   integer := H_EVNT_WIDTH - 1;                -- MS bit # of h_next_evnt
constant VROM_MSB :     integer := VROM_WIDTH - 1;                  -- MS bit # of v_rom
constant HROM_MSB :     integer := HROM_WIDTH - 1;                  -- MS bit # of h_rom


--
-- This group of constants controls the total number of lines (both fields) and
-- specifies where the odd and even lines begin and where the vertical blanking
-- periods begin and end.
--
constant V_TOTAL:       std_logic_vector(VCNT_MSB downto 0) -- number of lines
    := std_logic_vector(TO_UNSIGNED(525, VCNT_WIDTH));      -- in two fields

constant V_O_FLD_START: std_logic_vector(VCNT_MSB downto 0) -- first line of
    := std_logic_vector(TO_UNSIGNED(4, VCNT_WIDTH));        -- odd field

constant V_O_ACT_START: std_logic_vector(VCNT_MSB downto 0) -- first active video
    := std_logic_vector(TO_UNSIGNED(20, VCNT_WIDTH));       -- line of odd field 
                                                        
constant V_O_BNK_START: std_logic_vector(VCNT_MSB downto 0) -- first v blanking line
    := std_logic_vector(TO_UNSIGNED(264, VCNT_WIDTH));      -- of odd field
                                                        
constant V_E_FLD_START: std_logic_vector(VCNT_MSB downto 0) -- first line of even
    := std_logic_vector(TO_UNSIGNED(266, VCNT_WIDTH));      -- field

constant V_E_ACT_START: std_logic_vector(VCNT_MSB downto 0) -- first active video
    := std_logic_vector(TO_UNSIGNED(283, VCNT_WIDTH));      -- line of even field

constant V_E_BNK_START: std_logic_vector(VCNT_MSB downto 0) -- first v blanking line
    := std_logic_vector(TO_UNSIGNED(1, VCNT_WIDTH));        -- in even field

--
-- This group of constants defines the starting line numbers of the various bands
-- in the RP178 test pattern.
--
constant CEQ_START_O : std_logic_vector(VCNT_MSB downto 0)  -- start of CEQ pattern, odd field
    := V_O_ACT_START;
    
constant PLL_START_O : std_logic_vector(VCNT_MSB downto 0)  -- start of PLL pattern, odd field
    := std_logic_vector(TO_UNSIGNED(142, VCNT_WIDTH));

constant CEQ_START_E : std_logic_vector(VCNT_MSB downto 0)  -- start of CEQ pattern, even field
    := V_E_ACT_START;

constant PLL_START_E : std_logic_vector(VCNT_MSB downto 0)  -- start of PLL pattern, even field
    := std_logic_vector(TO_UNSIGNED(403, VCNT_WIDTH));

--
-- This group of constants controls the total number of clocks per line and
-- positions of the two TRS symbols.
--
constant H_TOTAL:       std_logic_vector(HCNT_MSB downto 0) -- total clocks on a 
    := std_logic_vector(TO_UNSIGNED(1716, HCNT_WIDTH));     -- line minus one

constant H_EAV_START:   std_logic_vector(HCNT_MSB downto 0) -- EAV start location
    := std_logic_vector(TO_UNSIGNED(1440, HCNT_WIDTH));         

constant H_SAV_START:   std_logic_vector(HCNT_MSB downto 0) -- SAV start location
    := std_logic_vector(TO_UNSIGNED(1712, HCNT_WIDTH)); 

constant H_ACT_START :  std_logic_vector(HCNT_MSB downto 0) -- first count of first active sample on line
    := std_logic_vector(TO_UNSIGNED(0, HCNT_WIDTH));

constant H_LAST_ACT :   std_logic_vector(HCNT_MSB downto 0) -- last 4 active savmple on line
    := H_EAV_START - 4;

--
-- The last sample on the first active line must be forced to 0x80 when generating
-- the RP 178 pattern.
--
constant RP178_LAST_SAMP_FIRST_ACT_LINE : std_logic_vector(HCNT_MSB downto 0)
    := H_EAV_START - 1;
                                                
--
-- This group of constant defines the component video values for the RP178
-- test patterns. Note that the CR and CB values are the same and are called CX.
-- All values are defined as 8-bit decimal values. If the video width is wider
-- than eight bits, zeros are added as the LSBs at the output.
--
constant CEQ_Y :    std_logic_vector(7 downto 0)    -- cable eq pattern Y
    := std_logic_vector(TO_UNSIGNED(102, 8));

constant CEQ_CX :   std_logic_vector(7 downto 0)    -- cable eq pattern Cx
    := std_logic_vector(TO_UNSIGNED(192, 8));

constant CEQ_YX :   std_logic_vector(7 downto 0)    -- cable eq last value of first line of 1st field
    := std_logic_vector(TO_UNSIGNED(32, 8));

constant PLL_Y :    std_logic_vector(7 downto 0)    -- PLL pattern Y
    := std_logic_vector(TO_UNSIGNED(68, 8));

constant PLL_CX :   std_logic_vector(7 downto 0)    -- PLL pattern Cx
    := std_logic_vector(TO_UNSIGNED(128, 8));

constant BLACK_Y :  std_logic_vector(7 downto 0)    -- black level Y
    := std_logic_vector(TO_UNSIGNED(16, 8));

constant BLACK_CX : std_logic_vector(7 downto 0)    -- black level Cx
    := std_logic_vector(TO_UNSIGNED(128, 8));

constant CEQ :      std_ulogic := '0';
constant PLL :      std_ulogic := '1';


--
-- This set of constants specifies the encoding of the vband signal.
--
constant BAND_V_BLANK : std_logic_vector(VBAND_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(0, VBAND_WIDTH));             -- v blanking band

constant BAND_CEQ :     std_logic_vector(VBAND_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(1, VBAND_WIDTH));             -- cable equalization pattern

constant BAND_PLL :     std_logic_vector(VBAND_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(2, VBAND_WIDTH));             -- PLL test pattern

constant BAND_1ST_LINE: std_logic_vector(VBAND_MSB downto 0) 
    := std_logic_vector(TO_UNSIGNED(3, VBAND_WIDTH));             -- 1st active line of 1st field


--
-- This set of constants specifies the encoding of the hbar signal.
--
constant BARH :     std_logic_vector(HBAR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(0, HBAR_WIDTH));              -- horizontal blanking interval

constant BAR0 :     std_logic_vector(HBAR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(1, HBAR_WIDTH));              -- active portion of line except ...

constant BAR1 :     std_logic_vector(HBAR_MSB downto 0)
    := std_logic_vector(TO_UNSIGNED(2, HBAR_WIDTH));              -- last 4 active words of line


-------------------------------------------------------------------------------
-- Signal Definitions
--

signal h_counter :      std_logic_vector(HCNT_MSB downto 0);      -- horizontal counter
signal v_counter :      std_logic_vector(VCNT_MSB downto 0);      -- vertical counter
signal v_region :       std_logic_vector(VRGN_MSB downto 0);      -- v region counter
signal h_region :       std_logic_vector(HRGN_MSB downto 0);      -- h region counter
signal dout :           std_logic_vector(VID_MSB downto 0);       -- data before output register
signal f :              std_ulogic;                               -- field bit
signal v :              std_ulogic;                               -- v blanking bit
signal h :              std_ulogic;                               -- h blanking bit
signal inc_v :          std_ulogic;                               -- signal to increment the v_counter
signal clr_v :          std_ulogic;                               -- signal to clear the v_counter
signal clr_h :          std_ulogic;                               -- signal to clear the h_counter
signal trs :            std_ulogic;                               -- indicates when TRS is being generated
signal trs_word :       std_logic_vector(1 downto 0);             -- indicates which word of TRS is being output
signal out_sel :        std_logic_vector(2 downto 0);             -- output MUX select bits
signal h_next_evnt :    std_logic_vector(H_EVNT_MSB downto 0);    -- h_counter value of next h event
signal h_rom :          std_logic_vector(HROM_MSB downto 0);      -- output of horz event ROM
signal v_next_evnt :    std_logic_vector(V_EVNT_MSB downto 0);    -- v_counter value of next vert event
signal v_rom :          std_logic_vector(VROM_MSB downto 0);      -- output of vert event ROM
signal v_evnt_match :   std_ulogic;                               -- output of vert event comparator
signal h_evnt_match :   std_ulogic;                               -- output of horz event comparator
signal video :          std_logic_vector(7 downto 0);             -- Y/Cx video values before TRS insertion
signal xyz :            std_logic_vector(7 downto 0);             -- XYZ word of TRS
signal vband :          std_logic_vector(VBAND_MSB downto 0);     -- vertical band
signal hbar :           std_logic_vector(HBAR_MSB downto 0);      -- horizontal bar


begin
    
    --
    -- XYZ word generation
    --
    -- This code generates the TRS symbol's XYZ word. Note that the XYZ word is
    -- generated as an 8-bit value. When assigned to the out vector, if the out
    -- vector is wider than 8-bits, the LS bits are filled in with zeros.
    --
    xyz(7) <= '1';
    xyz(6) <= f;
    xyz(5) <= v;
    xyz(4) <= h;
    xyz(3) <= v xor h;
    xyz(2) <= f xor h;
    xyz(1) <= f xor v;
    xyz(0) <= f xor v xor h;

        
    --
    -- vertical state machine
    --

    --
    --This is the line counter comparator
    --
    v_evnt_match <= '1' when v_counter = v_next_evnt else '0';

    --
    -- v_region counter
    --
    -- This counter holds the  current state value of the vertical state machine.
    -- It increments at the end of line as indicated by the inc_v signal from 
    -- the horizontal state machine if the current contents of the v_counter 
    -- match the v_next_evnt ROM's output.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            v_region <= (others => '1');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (h_counter(1 downto 0) = "11" and inc_v = '1') then
                    if (clr_v = '1') then
                        v_region <= (others => '0');
                    elsif (v_evnt_match = '1') then
                        v_region <= v_region + 1;
                    end if;
                end if;
            end if;
        end if;
    end process;
            
    --
    -- v_envt_rom
    --
    -- This ROM generates the current state values for the vertical state 
    -- machine. It also generates the line number of the next vertical event. 
    --
    process(v_region)
    begin
        case v_region is
    --                                next event            vfclr  band
            when "0000" => v_rom <= ((V_O_FLD_START - 1) & "110" & BAND_V_BLANK);  -- start v blank
            when "0001" => v_rom <= ((V_O_ACT_START - 1) & "100" & BAND_V_BLANK);  -- start fld 1
            when "0010" => v_rom <= (V_O_ACT_START       & "000" & BAND_1ST_LINE); -- 1st active line
            when "0011" => v_rom <= ((PLL_START_O - 1)   & "000" & BAND_CEQ);      -- CEQ pattern band
            when "0100" => v_rom <= ((V_O_BNK_START - 1) & "000" & BAND_PLL);      -- PLL pattern band
            when "0101" => v_rom <= ((V_E_FLD_START - 1) & "100" & BAND_V_BLANK);  -- start v blank
            when "0110" => v_rom <= ((V_E_ACT_START - 1) & "110" & BAND_V_BLANK);  -- start fld 2
            when "0111" => v_rom <= ((PLL_START_E - 1)   & "010" & BAND_CEQ);      -- CEQ pattern band
            when "1000" => v_rom <= ((V_TOTAL - 1)       & "010" & BAND_PLL);      -- PLL pattern band
            when "1001" => v_rom <= (V_TOTAL             & "011" & BAND_PLL);      -- last line fld 2
            when others => v_rom <= (V_TOTAL             & "011" & BAND_PLL);      -- default
        end case;
    end process;

    --
    -- assign the individual fields from the v_envt_rom output
    --
    v_next_evnt <= v_rom(V_EVNT_MSB + VBAND_WIDTH + 3 downto VBAND_WIDTH + 3);
    v           <= v_rom(VBAND_WIDTH + 2);
    f           <= v_rom(VBAND_WIDTH + 1);
    clr_v       <= v_rom(VBAND_WIDTH);
    vband       <= v_rom(VBAND_MSB downto 0);


    --
    -- horizontal state machine
    --

    --
    -- this is the h_counter comparator
    --
    h_evnt_match <= '1' when h_counter(HCNT_MSB downto 2) = h_next_evnt else '0';


    -- h_region counter
    --
    -- The h_region counter contains the current state of the horizontal state 
    -- machine. This counter increments when the contents of the h_counter match 
    -- the value generated by the h_next_evnt ROM.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            h_region <= (others => '1');
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (h_counter(1 downto 0) = "11" and h_evnt_match = '1') then
                    h_region <= h_region + 1;
                end if;
            end if;
        end if;
    end process;        
            
    --
    -- h_rom
    --
    -- Based on the current horizontal state contained in the h_region, this ROM
    -- generates the current state values for the horzintal state machine. It 
    -- also generates the horizontal count value to match against the h_counter 
    -- for the next state transition.
    --
    process(h_region)
    begin
        case h_region is
    --                                next event                            htic    bar
    --                                                                       rnl
    --                                                                       scr
    --                                                                        vh
            when "000" => h_rom <= ( H_EAV_START(HCNT_MSB downto 2)      & "1100" & BARH); -- EAV
            when "001" => h_rom <= ((H_SAV_START(HCNT_MSB downto 2) - 1) & "1000" & BARH); -- h blanking
            when "010" => h_rom <= ((H_TOTAL(HCNT_MSB downto 2)     - 1) & "0101" & BARH); -- SAV
            when "011" => h_rom <= ((H_EAV_START(HCNT_MSB downto 2) - 5) & "0000" & BAR0); -- Active video
            when "100" => h_rom <= ((H_EAV_START(HCNT_MSB downto 2) - 4) & "0000" & BAR0); -- Active video
            when "101" => h_rom <= ((H_EAV_START(HCNT_MSB downto 2) - 3) & "0000" & BAR0); -- Active video
            when "110" => h_rom <= ((H_EAV_START(HCNT_MSB downto 2) - 2) & "0000" & BAR0); -- Active video
            when others=> h_rom <= ((H_EAV_START(HCNT_MSB downto 2) - 1) & "0010" & BAR1); -- last 4 words of line
        end case;
    end process;

    h_next_evnt <= h_rom(H_EVNT_MSB + HBAR_WIDTH + 4 downto HBAR_WIDTH + 4);
    h           <= h_rom(HBAR_WIDTH + 3);
    trs         <= h_rom(HBAR_WIDTH + 2);
    inc_v       <= h_rom(HBAR_WIDTH + 1);
    clr_h       <= h_rom(HBAR_WIDTH);
    hbar        <= h_rom(HBAR_MSB downto 0);

    --
    -- v_counter
    --
    -- The v_counter keeps track of the current line number. It increments 
    -- during the generation of the EAV signal since SMPTE 125M specifies that 
    -- each line begins with the EAV symbol (although the pixel counter is not 0
    -- at this point). To match the SMPTE 125M specification, the count of the 
    -- first line is zero, not one. On reset, the line counter is set to the 
    -- last line. It will increment to the first line one the first clock after 
    -- the reset is negated.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            v_counter <= V_TOTAL;
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (h_counter(1 downto 0) = "11" and inc_v = '1') then
                    if (clr_v = '1') then
                        v_counter <= (0 => '1', others => '0');
                    else
                        v_counter <= v_counter + 1;
                    end if;
                end if;
            end if;
        end if;
    end process;

    --
    -- h_counter
    --
    -- The h_counter keeps track of the current vertical position on the screen.
    -- The h_counter counts from 0 (which is the first active video position) to
    -- the maximum count for the line (which corresponds to last word of the 
    -- SAV). On reset, the h_counter is set to the EAV position so that an EAV 
    -- is the first thing generated after reset.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            h_counter <= H_EAV_START - 1;
        elsif (clk'event and clk = '1') then
            if (ce = '1') then
                if (clr_h = '1' and h_counter(1 downto 0) = "11") then
                    h_counter <= (others => '0');
                else
                    h_counter <= h_counter + 1;
                end if;
            end if;
        end if;
    end process;


    --
    -- This logic generates the video component values.
    -- 

    process(vband, hbar, h_counter)
    begin
        if (vband = BAND_V_BLANK or hbar = BARH) then
            if (h_counter(0) = '1') then
                video <= BLACK_Y;
            else
                video <= BLACK_CX;
            end if;
        elsif (vband = BAND_CEQ) then
            if (h_counter(0) = '1') then
                video <= CEQ_Y;
            else
                video <= CEQ_CX;
            end if;
        elsif (vband = BAND_PLL) then
            if (h_counter(0) = '1') then
                video <= PLL_Y;
            else
                video <= PLL_CX;
            end if;
        elsif (hbar = BAR1 and h_counter(1 downto 0) = "11") then
            video <= CEQ_YX;
        else
            if (h_counter(0) = '1') then
                video <= CEQ_Y;
            else
                video <= CEQ_CX;
            end if;
        end if;
    end process;


    --
    -- This logic implements a mux on the output of the video ROM. Normally, the 
    -- output of the video ROM is sent to the output register. If a TRS is being
    -- generated, this MUX can force the output to be all zeros or all ones as
    -- required.
    --
    trs_word <= h_counter(1 downto 0);
    out_sel <= (trs & trs_word);

    process(out_sel, xyz, video)
        begin
            case out_sel is
                when "100"  => dout <= (others => '1');             -- 0x3FF
                when "101"  => dout <= (others => '0');             -- 0x000
                when "110"  => dout <= (others => '0');             -- 0x000
                when "111"  => dout <= (xyz(7), xyz(6), xyz (5), xyz(4),
                                        xyz(3), xyz(2), xyz (1), xyz(0),
                                        others => '0');             -- xyz
                when others => dout <= (video(7), video(6), video(5), video(4),
                                        video(3), video(2), video(1), video(0),
                                        others => '0');             -- video
            end case;
    end process;

    --
    -- This code implements the output registers.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            q      <= (others => '0');
            h_sync <= '0';
            v_sync <= '0';
            field  <= '0';
        elsif (clk'event and clk ='1') then
            if (ce = '1') then
                q      <= std_ulogic_vector(dout);
                h_sync <= h or trs;
                v_sync <= v;
                field  <= f;
            end if;
        end if;
    end process;

end synth;