-------------------------------------------------------------------------------- 
-- Copyright (c) 2005 Xilinx, Inc. 
-- All Rights Reserved 
-------------------------------------------------------------------------------- 
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /   Vendor: Xilinx 
-- \   \   \/    Author: John F. Snow, Advanced Product Division, Xilinx, Inc.
--  \   \        Filename: $RCSfile: test_vid_decode.vhd,rcs $
--  /   /        Date Last Modified:  $Date: 2005-01-18 09:59:46-07 $
-- /___/   /\    Date Created: Jan 5, 2005
-- \   \  /  \ 
--  \___\/\___\ 
-- 
--
-- Revision History: 
-- $Log: test_vid_decode.vhd,rcs $
-- Revision 1.0  2005-01-18 09:59:46-07  jsnow
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
-- This testbench tests the video_decode module. The testbench instances the
-- video_decode module and a video generator module called multigen. The 
-- multigen module can generate video for all six digital component video 
-- formats supported by video_decode.
-- 
-- The testbench beins by testing that the video_decode module can detect and 
-- lock to the PAL 4:2:2 and then the NTSC 4:2:2 standard. During the NTSC 
-- 4:2:2, the testbench also tests the video decoder is correctly doing TRS 
-- blanking, synchronous switching, and generating correct TRS symbols in the 
-- absence of input video. After testing these features of the video decoder, 
-- the testbench tests that the video decoder detects and locks to the remaining
--  four component video standards.
-- 
-- The piece of code used to test TRS blanking, synchronous switching, and 
-- interrupted video TRS generation can be moved and excuted while multigen is
-- generating any of the six video standards.

use std.textio.all;

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

entity test_vid_decode is
end test_vid_decode;


architecture simulation of test_vid_decode is
    
--
-- Constant definitions
--
constant MAX_VID_DELAY : integer := 10;                     -- Max length of video delay pipeline
constant HCNT_WIDTH :    integer := 12;                     -- horizontal counter width
constant HCNT_MSB :      integer := HCNT_WIDTH - 1;         -- horizontal counter MSB
constant VCNT_WIDTH :    integer := 10;                     -- vertical counter width
constant VCNT_MSB :      integer := VCNT_WIDTH - 1;         -- vertical counter MSB

--
-- The video subtype is used for all the 10-bit wide video paths in the design.
-- The vid_array type is used as a video FIFO.
-- 
subtype video is        std_ulogic_vector(9 downto 0);
type vid_array is array (MAX_VID_DELAY - 1 downto 0) of video;

--
-- Signal definitions
--
signal clk :            std_ulogic := '0';                  -- clock
signal enclk :          std_ulogic := '1';                  -- used to kill clock at end of test
signal rst :            std_ulogic := '1';                  -- reset
signal mg_vid :         video;                              -- multigen video output
signal std_locked :     std_ulogic;                         -- autodetect locked to standard
signal std_in :         std_ulogic_vector(2 downto 0);      -- video standard for generated video, set by testbench
signal std :            std_ulogic_vector(2 downto 0);      -- video standard code from video decoder
signal mg_f :           std_ulogic_vector(2 downto 0);      -- multigen field output
signal f :              std_ulogic;                         -- LSB of mg_f, indicates field status for component vid
signal v :              std_ulogic;                         -- multigen vertical blank output
signal h :              std_ulogic;                         -- multigen horizontal blank output
signal mg_trs :         std_ulogic;                         -- TRS output of multigen
signal kill :           std_ulogic;                         -- kills the video into the video decoder when asserted
signal vid_in :         video;                              -- multigen video after kill circuit
signal trs :            std_ulogic;                         -- video decoder trs output
signal vid_out :        video;                              -- video output of video decoder
signal field :          std_ulogic;                         -- video decoder f output
signal v_blank :        std_ulogic;                         -- video decoder v output
signal h_blank :        std_ulogic;                         -- video decoder h output
signal hcnt :           std_logic_vector(HCNT_MSB downto 0);-- video decoder hcnt output
signal vcnt :           std_logic_vector(VCNT_MSB downto 0);-- video decoder vcnt output
signal sync_switch :    std_ulogic;                         -- video decoder sync_switch output
signal locked :         std_ulogic;                         -- video decoder locked output
signal eav_next :       std_ulogic;                         -- video decoder eav_next output
signal sav_next :       std_ulogic;                         -- video decoder sav_next output
signal xyz_word :       std_ulogic;                         -- video decoder xyz_word output
signal anc_next :       std_ulogic;                         -- video decoder anc_next output
signal edh_next :       std_ulogic;                         -- video decoder edh_next output
signal gen_ce :         std_ulogic;                         -- clock enable for multigen
signal rx_ce :          std_ulogic;                         -- clock enable for video decoder
signal early_v :        std_ulogic;                         -- enables early fall of V bit
signal ref_vid :        video;                              -- reference video to compare to vid_out
signal en_trs_blank :   std_ulogic;                         -- input to video deocder
signal vid_delay :      vid_array;                          -- video delay pipeline;
signal dont_check :     boolean;                            -- don't compare video out when TRUE
signal pipe_delay :     integer;                            -- current amount of delay on video
signal VCC :            std_ulogic;
signal GND :            std_ulogic;
signal unlock_ok :      boolean;
signal set_unlock_ok :  boolean;
signal clr_unlock_ok :  boolean;
signal clr_unlock_ok2 : boolean;

--
-- Component declarations.
--
component video_decode
    generic (
        HCNT_WIDTH : integer := 12;
        VCNT_WIDTH : integer := 10);
    port (
        clk:            in    std_ulogic;                   -- clock input
        ce:             in    std_ulogic;                   -- clock enable
        rst:            in    std_ulogic;                   -- async reset input
        vid_in:         in    std_ulogic_vector(9 downto 0);-- input video
        reacquire:      in    std_ulogic;                   -- forces autodetect to reacquire the standard
        en_sync_switch: in    std_ulogic;                   -- enables sync switching
        en_trs_blank:   in    std_ulogic;                   -- enables TRS blanking
        std:            inout std_ulogic_vector(2 downto 0);-- video standard code
        std_locked:     inout std_ulogic;                   -- autodetect circuit is locked to standard
        trs:            out   std_ulogic;                   -- asserted during TRS symbol
        vid_out:        out   std_ulogic_vector(9 downto 0);-- output video stream
        field:          out   std_ulogic;                   -- field indicator
        v_blank:        out   std_ulogic;                   -- vertical blanking indicator
        h_blank:        out   std_ulogic;                   -- horizontal blanking indicator
        horz_count:     out   
            std_logic_vector(HCNT_WIDTH-1 downto 0);        -- horizontal counter
        vert_count:     out   
            std_logic_vector(VCNT_WIDTH-1 downto 0);        -- vertical counter
        sync_switch:    out   std_ulogic;                   -- asserted on sync switching lines
        locked:         out   std_ulogic;                   -- asserted when flywheel is synced to input video
        eav_next:       out   std_ulogic;                   -- next word is first word of EAV
        sav_next:       out   std_ulogic;                   -- next word is first word of SAV
        xyz_word:       out   std_ulogic;                   -- current word is the XYZ word
        anc_next:       out   std_ulogic;                   -- next word is first word of ANC packet
        edh_next:       out   std_ulogic                    -- next word is first word of EDH packet
    );                      
end component;

component multigen
    port (
        clk:        in    std_ulogic;                       -- clock input
        ce:         in    std_ulogic;                       -- clock enable
        rst:        in    std_ulogic;                       -- async reset input
        std:        in    std_ulogic_vector(2 downto 0);    -- video standard code
        s:          in    std_ulogic;                       -- color space select for 4444
        early_v:    in    std_ulogic;                       -- enables early V bit fall
        q:          out   std_ulogic_vector(9 downto 0);    -- video output
        field:      out   std_ulogic_vector(2 downto 0);    -- field bits
        v_blank:    out   std_ulogic;                       -- vertical blanking indicator
        h_blank:    out   std_ulogic;                       -- horizontal blanking indicator
        trs:        inout std_ulogic                        -- asserted during TRS symbol
    );
end component;

begin

    VCC <= '1';
    GND <= '0';

    --
    -- Instantiate the video_decode module. This is the module being tested.
    --
    U1: video_decode
        generic map (
            HCNT_WIDTH => 12,
            VCNT_WIDTH => 10)
        port map (
            clk             => clk,
            ce              => rx_ce,
            rst             => rst,
            vid_in          => vid_in,
            reacquire       => GND,
            en_sync_switch  => VCC,
            en_trs_blank    => en_trs_blank,
            std             => std,
            std_locked      => std_locked,
            trs             => trs,
            vid_out         => vid_out,
            field           => field,
            v_blank         => v_blank,
            h_blank         => h_blank,
            horz_count      => hcnt,
            vert_count      => vcnt,
            sync_switch     => sync_switch,
            locked          => locked,
            eav_next        => eav_next,
            sav_next        => sav_next,
            xyz_word        => xyz_word,
            anc_next        => anc_next,
            edh_next        => edh_next);

    --
    -- Instantiate the multigen module. This module generates video to feed
    -- into the video_decode module.
    --
    U2: multigen
        port map (
            clk             => clk,
            ce              => gen_ce,
            rst             => rst,
            std             => std_in,
            s               => VCC,
            early_v         => early_v,
            q               => mg_vid,
            field           => mg_f,
            v_blank         => v,
            h_blank         => h,
            trs             => mg_trs);

    f <= mg_f(0);
    --
    -- Generate the clock signal.
    --
    -- This produces an approximately 27MHz clock. This is the correct clock 
    -- rate for the PAL and NTSC 4:2:2 4x3 aspect ration standards, but is not 
    -- correct for the other supported video standards.
    --
    clk <= enclk and not clk after 18.5 ns;

    --
    -- Video kill circuit
    --
    -- Kills the video coming from multigen when kill is asserted. 
    --
    process(mg_vid, kill)
    begin
        if (kill = '1') then
            vid_in <= (others => '0');
        else
            vid_in <= mg_vid;
        end if;
    end process;


    --
    -- Assert the reset signal for the first few clock cycles.
    --
    process
    begin
        for count in 1 to 5 loop
            wait until clk'event and clk = '0';
        end loop;
        rst <= '0';
        wait;
    end process;

    --
    -- This process is used for interprocess communications. It looks for rising
    -- edges on the set_unlock_ok, clr_unlock_ok, or clr_unlock_ok2 signals and
    -- either sets or clears the unlock_ok signal as appropriate. The unlock_ok
    -- signal is used by one of the checking routines to determine if it is
    -- OK if the flywheel becomes unlocked. The flywheel is permitted to
    -- unlock when the input video standard changes, but not after the flywheel
    -- has locked to the new video standard, until it changes again.
    -- 
    process(set_unlock_ok, clr_unlock_ok, clr_unlock_ok2)
    begin
        if (set_unlock_ok'event and set_unlock_ok) then
            unlock_ok <= TRUE;
        elsif (clr_unlock_ok'event and clr_unlock_ok) then
            unlock_ok <= FALSE;
        elsif (clr_unlock_ok2'event and clr_unlock_ok2) then
            unlock_ok <= FALSE;
        end if;
    end process;
    --
    -- On rising edge of ad_locked, make sure that the standard detected by
    -- the autodetect circuit matches the standard being generated by multigen
    -- and print either an error or a success message.
    --
    process(std_locked)
        variable l : line;
        variable pre :      string(1 to 21) := "autodetect locked to ";
        variable n422 :     string(1 to 21) := "NTSC 4:2:2           ";
        variable n422wide : string(1 to 21) := "NTSC 4:2:2 widescreen";
        variable n4444 :    string(1 to 21) := "NTSC 4:4:4:4         ";
        variable p422 :     string(1 to 21) := "PAL 4:2:2            ";
        variable p422wide : string(1 to 21) := "PAL 4:2:2 widescreen ";
        variable p4444 :    string(1 to 21) := "PAL 4:4:4:4          ";
        variable unknown :  string(1 to 21) := "unknown              ";
        variable result :   string(1 to 21);
    begin
        if (std_locked'event and std_locked = '1') then
            if (rst = '0' and std /= std_in) then
                assert (FALSE)
                    report "autodetect did not generated correct std code."
                    severity failure;
            else
                case std is
                    when "000"  => result := n422;
                    when "010"  => result := n422wide;
                    when "011"  => result := n4444;
                    when "100"  => result := p422;
                    when "110"  => result := p422wide;
                    when "111"  => result := p4444;
                    when others => result := unknown;
                end case;
                write(l, pre);
                write(l, result);
                writeline(output, l);
            end if;
        end if;
    end process;
        
    --
    -- On rising edge of locked, negate the unlock_ok signal since the flywheel
    -- should stay locked until we change the input standard.
    --
    process(locked)
        variable l : line;
        variable fly_locked : string(1 to 16) := "Flywheel locked.";
    begin
        if (rst = '0') then
            if (locked'event and locked = '1') then
                clr_unlock_ok2 <= TRUE;
                write(l, fly_locked);
                writeline(output, l);
                clr_unlock_ok2 <= FALSE;
            end if;
        end if;
    end process;

    --
    -- On falling edge of locked, make sure the the unlock_ok signal is asserted.
    -- If it is not, then generate an error.
    --
    process(locked)
    begin
        if (rst = '0') then 
            if (locked'event and locked = '0') then
                assert (unlock_ok)
                    report "flywheel became unlocked unexpectedly."
                    severity failure;
            end if;
        end if;
    end process;

    --
    -- Implement a delay of the video data so that it can be compared to the 
    -- data coming from the flywheel.
    --
    process(clk, rst)
    begin
        if (rst = '1') then
            for i in 0 to MAX_VID_DELAY - 1 loop
                vid_delay(i) <= (others => '0');
            end loop;
        elsif (clk'event and clk = '1') then
            for i in MAX_VID_DELAY - 1 downto 1 loop
                vid_delay(i) <= vid_delay(i - 1);
            end loop;
            vid_delay(0) <= mg_vid;
        end if;
    end process;
            

    --
    -- Video checker
    --
    -- Compare the output video stream with the input video stream. This 
    -- comparison is not conducted when the flywheel is unlocked or when 
    -- dont_check is asserted. For XYZ words, if the early_v signal is asserted,
    -- the V bit and the XYZ word protection bits affected by V are not checked 
    -- if the current line number is in the region where the V bit is allowed to
    -- fall early.
    --
    process(clk)
    begin
        if (clk'event and clk = '1') then
            ref_vid <= vid_delay(pipe_delay - 1);
        end if;
    end process;

    process(clk)
        variable l : line;
        variable vid_interrupt_err_str : string(1 to 47) := 
            "Video difference during interrupt input video: ";
        variable vid_err_str : string(1 to 18) := "Video difference: ";
        variable ref_vid_str : string(1 to 10) := "ref_vid = "; 
        variable vid_out_str : string(1 to 12) := "  vid_out = ";
    begin
        if (clk'event and clk = '0') then

            if (kill = '1' and not dont_check) then
                if (trs = '1' and ref_vid /= vid_out) then
                    write(l, vid_interrupt_err_str);
                    write(l, ref_vid_str);
                    hwrite(l, "00" & ref_vid);
                    write(l, vid_out_str);
                    hwrite(l, "00" & vid_out);
                    writeline(output, l); 
                    assert FALSE severity failure;
                end if;
            elsif (locked = '1' and not unlock_ok) then
                if (early_v = '1' and xyz_word = '1' and
                    ((vcnt >= std_logic_vector(TO_UNSIGNED(10,  VCNT_WIDTH)) and
                      vcnt <= std_logic_vector(TO_UNSIGNED(19,  VCNT_WIDTH))) or
                     (vcnt >= std_logic_vector(TO_UNSIGNED(273, VCNT_WIDTH)) and
                      vcnt <= std_logic_vector(TO_UNSIGNED(282, VCNT_WIDTH))))) then
                    if ((ref_vid and "1101010011") /= (vid_out and "1101010011")) then
                        write(l, vid_err_str);
                        write(l, ref_vid_str);
                        hwrite(l, "00" & ref_vid);
                        write(l, vid_out_str);
                        hwrite(l, "00" & vid_out);
                        writeline(output, l); 
                        assert FALSE severity failure;
                    end if;
                elsif (not dont_check) then
                    if (ref_vid /= vid_out) then
                        write(l, vid_err_str);
                        write(l, ref_vid_str);
                        hwrite(l, "00" & ref_vid);
                        write(l, vid_out_str);
                        hwrite(l, "00" & vid_out);
                        writeline(output, l); 
                        assert FALSE severity failure;
                    end if;
                end if;
            end if;
        end if;
    end process;

    -------------------------------------------------------------------------------
    --
    -- The following initial block is the "script" that drives the test bench.
    -- 
    process
        variable l : line;
        variable testing_pal422_str :       string (1 to 23) 
                                    := "Testing PAL 4:2:2      ";
        variable testing_ntsc422_str :      string (1 to 23) 
                                    := "Testing NTSC 4:2:2     ";
        variable testing_ntsc422_wide_str : string (1 to 23) 
                                    := "Testing NTSC 4:2:2 wide";
        variable testing_ntsc4444_str :     string (1 to 23) 
                                    := "Testing NTSC 4:4:4:4   ";
        variable testing_pal422_wide_str :  string (1 to 23) 
                                    := "Testing PAL 4:2:2 wide ";
        variable testing_pal4444_str :      string (1 to 23) 
                                    := "Testing PAL 4:4:4:4    ";
        variable testing_TRSblank_str :     string (1 to 23) 
                                    := "Testing TRS blanking   ";
        variable testing_syncswitch_str :   string (1 to 23) 
                                    := "Testing sync switching ";
        variable testing_int_vid_str :      string (1 to 47) 
                                    := "Testing TRS generation during interrupted video";
        variable end_of_test_str :          string (1 to 23) 
                                    := "End of test.           ";
    begin
        kill <= '0';
        gen_ce <= '1';
        rx_ce <= '1';
        early_v <= '0';
        en_trs_blank <= '1';                -- enable TRS blanking
        dont_check <= FALSE;
        pipe_delay <= 5;

    --
    -- First test PAL 4:2:2 format
    --
        clr_unlock_ok <= TRUE;
        std_in <= "100";
        wait until clk'event;
        clr_unlock_ok <= FALSE;
        write(l, testing_pal422_str);
        writeline(output, l);
        for count in 1 to 3 loop            -- do 2 frames of PAL 422
            wait until f'event and f = '0';
        end loop;
        for count in 1 to 6 loop            -- wait a few lines
            wait until h'event and h = '0';
        end loop;

    --
    -- Next, change the multigen format to NTSC 4:2:2
    --
        set_unlock_ok <= TRUE;
        std_in <= "000";                    -- switch to NTSC 422
        early_v <= '1';                     -- test NTSC 422 with early fall of V bit
        wait until clk'event;
        set_unlock_ok <= FALSE;
        write(l, testing_ntsc422_str);
        writeline(output, l);
        for count in 1 to 2 loop            -- do 2 frames of PAL 422
            wait until f'event and f = '0';
        end loop;
        for count in 1 to 30 loop           -- wait a few lines
            wait until h'event and h = '0';
        end loop;

        early_v <= '0';

    --
    -- Test TRS blanking
    --
    -- The following tests will cause the the video generator (multigen) and the
    -- video decoder to be be skewed from each other first by four clocks in one
    -- direction then four clocks in the other direction. This is done by
    -- controlling the clock enables to these modules. The purpose of this test
    -- is to test that the flywheel correctly implements TRS blanking when the
    -- received video stream's TRS symbol does not line up with the TRS 
    -- generated by the flywheel.
    --
    -- The testbench enables TRS blanking. Because of the way the test bench
    -- is constructed, the flywheel generated and inserted TRS sybmols should 
    -- always match the reference video stream's TRS symbols, even when the 
    -- video streams are skewed by a small amount. You can see that multiple 
    -- copies of the TRS symbol appear in the flywheel's output video stream 
    -- when TRS blanking is disabled by negating the en_trs_blank signal to turn
    -- off TRS blanking. The test bench will report errors during the TRS 
    -- blanking test if TRS blanking is turned off.
    --
        write(l, testing_TRSblank_str);
        writeline(output, l);

        for count in 1 to 100 loop
            wait until clk'event and clk = '0';
        end loop;

        rx_ce <= '0';
        dont_check <= TRUE;

        for count in 1 to 4 loop
            wait until clk'event and clk = '0';
        end loop;

        pipe_delay <= 9;
        rx_ce <= '1';

        for count in 1 to 4 loop
            wait until clk'event and clk = '0';
        end loop;
        dont_check <= FALSE;
        
        wait until h'event and h = '0';

        for count in 1 to 100 loop
            wait until clk'event and clk = '0';
        end loop;

        gen_ce <= '0';
        dont_check <= TRUE;

        for count in 1 to 8 loop
            wait until clk'event and clk = '0';
        end loop;

        pipe_delay <= 1;
        gen_ce <= '1';

        for count in 1 to 8 loop
            wait until clk'event and clk = '0';
        end loop;
        dont_check <= FALSE;

        wait until h'event and h = '0';

        for count in 1 to 100 loop
            wait until clk'event and clk = '0';
        end loop;

        rx_ce <= '0';
        dont_check <= TRUE;
        for count in 1 to 4 loop
            wait until clk'event and clk = '0';
        end loop;

        pipe_delay <= 5;
        rx_ce <= '1';

        for count in 1 to 4 loop
            wait until clk'event and clk = '0';
        end loop;
        dont_check <= FALSE;

        for count in 1 to 10 loop           -- wait a few lines
            wait until h'event and h = '0';
        end loop;

    --
    -- Test synchronous switching
    --
    -- The code below waits for a synchronous switching interval then inserts a
    -- one clock cycle skew by disabling the receiver. The dont_check variable 
    -- is asserted to prevent the detection of the errors that occur before the
    -- flywheel resyncs at the end of the line. The dont_check variable is 
    -- negated just before the SAV symbol is generated.
    --
    -- This process is then repeated for a skew in the opposite direction.
    --
        write(l, testing_syncswitch_str);
        writeline(output, l);

        -- Wait for next synchronous switching line
        wait until f'event and f = '1';
        for count in 1 to 7 loop            
            wait until h'event and h = '0';
        end loop;

        -- 1 clock skew during sync switch
        wait until sync_switch'event and sync_switch = '1';
        for count in 1 to 700 loop          
            wait until clk'event and clk = '0';
        end loop;
        dont_check <= TRUE;
        rx_ce <= '0';                           -- disable the receiver for 1 clock
        wait until clk'event and clk = '0';     -- repeat this line for larger amounts of skew
        rx_ce <= '1';
        for count in 1 to 2 loop
            wait until trs'event and trs = '1'; -- wait for SAV
        end loop;
        dont_check <= FALSE;

        for count in 1 to 2 loop
            wait until sync_switch'event and sync_switch = '1';
        end loop;
        for count in 1 to 700 loop          
            wait until clk'event and clk = '0';
        end loop;
        dont_check <= TRUE;
        gen_ce <= '0';                          -- disable the transmitter for 1 clock
        wait until clk'event and clk = '0';     -- repeat this line for larger amounts of skew
        gen_ce <= '1';
        for count in 1 to 2 loop
            wait until trs'event and trs = '1'; -- wait for SAV
        end loop;
        dont_check <= FALSE;

    --
    -- Now test that the video decoder continues to generate TRS symbols during
    -- blanked video. The kill signal is asserted to kill the video going into 
    -- the video decoder. The multigen module continues to generate video so 
    -- that the TRS symbols gnerated by the video decoder can be compared with 
    -- those from multigen.
    --
        write(l, testing_int_vid_str);
        writeline(output, l);
        wait until (clk'event and clk = '0');
        set_unlock_ok <= TRUE;
        kill <= '1';
        early_v <= '0';
        wait until clk'event;
        set_unlock_ok <= FALSE;

        for count in 1 to 2  loop
            wait until f'event and f = '0';
        end loop;

        kill <= '0';
        dont_check <= TRUE;

        for count in 1 to 2 loop
            wait until h'event and h = '0';
        end loop;

        dont_check <= FALSE;

    --
    -- Test the remaining video standards
    --
        set_unlock_ok <= TRUE;
        std_in <= "010";                    -- switch to NTSC 422 WIDE
        early_v <= '0';
        wait until clk'event;
        set_unlock_ok <= FALSE;
        write(l, testing_ntsc422_wide_str);
        writeline(output, l);
        for count in 1 to 2 loop            -- do 2 frames of NTSC 422 WIDE
            wait until f'event and f = '0';
        end loop;
        for count in 1 to 6 loop
            wait until h'event and h = '0';
        end loop;

        set_unlock_ok <= TRUE;
        std_in <= "011";                    -- switch to NTSC 4444
        wait until clk'event;
        set_unlock_ok <= FALSE;
        write(l, testing_ntsc4444_str);
        writeline(output, l);
        for count in 1 to 2 loop            -- do 2 frames of NTSC 4444
            wait until f'event and f = '0';
        end loop;
        for count in 1 to 6 loop
            wait until h'event and h = '0';
        end loop;

        set_unlock_ok <= TRUE;
        std_in <= "110";                    -- switch to PAL 422 WIDE
        wait until clk'event;
        set_unlock_ok <= FALSE;
        write(l, testing_pal422_wide_str);
        writeline(output, l);
        for count in 1 to 2 loop            -- do 2 frames of PAL 422 WIDE
            wait until f'event and f = '0';
        end loop;
        for count in 1 to 6 loop
            wait until h'event and h = '0';
        end loop;

        set_unlock_ok <= TRUE;
        std_in <= "111";                    -- switch to PAL 4444
        wait until clk'event;
        set_unlock_ok <= FALSE;
        write(l, testing_pal4444_str);
        writeline(output, l);
        for count in 1 to 2 loop            -- do 2 frames of PAL 4444
            wait until f'event and f = '0';
        end loop;
        for count in 1 to 6 loop
            wait until h'event and h = '0';
        end loop;


        write(l, end_of_test_str);
        writeline(output, l);

        enclk <= '0';

    end process;

end simulation;