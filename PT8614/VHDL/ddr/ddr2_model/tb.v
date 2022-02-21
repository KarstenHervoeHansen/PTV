/****************************************************************************************
*
*    File Name:  tb.v
*      Version:  5.20
*        Model:  BUS Functional
*
* Dependencies:  ddr2.v, ddr2_parameters.vh
*
*  Description:  Micron SDRAM DDR2 (Double Data Rate 2) test bench
*
*         Note:  - Set simulator resolution to "ps" accuracy
*                - Set Debug = 0 to disable $display messages
*
*   Disclaimer   This software code and all associated documentation, comments or other 
*  of Warranty:  information (collectively "Software") is provided "AS IS" without 
*                warranty of any kind. MICRON TECHNOLOGY, INC. ("MTI") EXPRESSLY 
*                DISCLAIMS ALL WARRANTIES EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
*                TO, NONINFRINGEMENT OF THIRD PARTY RIGHTS, AND ANY IMPLIED WARRANTIES 
*                OF MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR PURPOSE. MTI DOES NOT 
*                WARRANT THAT THE SOFTWARE WILL MEET YOUR REQUIREMENTS, OR THAT THE 
*                OPERATION OF THE SOFTWARE WILL BE UNINTERRUPTED OR ERROR-FREE. 
*                FURTHERMORE, MTI DOES NOT MAKE ANY REPRESENTATIONS REGARDING THE USE OR 
*                THE RESULTS OF THE USE OF THE SOFTWARE IN TERMS OF ITS CORRECTNESS, 
*                ACCURACY, RELIABILITY, OR OTHERWISE. THE ENTIRE RISK ARISING OUT OF USE 
*                OR PERFORMANCE OF THE SOFTWARE REMAINS WITH YOU. IN NO EVENT SHALL MTI, 
*                ITS AFFILIATED COMPANIES OR THEIR SUPPLIERS BE LIABLE FOR ANY DIRECT, 
*                INDIRECT, CONSEQUENTIAL, INCIDENTAL, OR SPECIAL DAMAGES (INCLUDING, 
*                WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS INTERRUPTION, 
*                OR LOSS OF INFORMATION) ARISING OUT OF YOUR USE OF OR INABILITY TO USE 
*                THE SOFTWARE, EVEN IF MTI HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH 
*                DAMAGES. Because some jurisdictions prohibit the exclusion or 
*                limitation of liability for consequential or incidental damages, the 
*                above limitation may not apply to you.
*
*                Copyright 2003 Micron Technology, Inc. All rights reserved.
*
* Rev   Author   Date        Changes
* --------------------------------------------------------------------------------
* 1.00  JMK      07/29/03    Initial Release
* 1.10  JMK      08/09/03    Timing Parameter updates to tIS, tIH, tDS, tDH
* 2.20  JMK      08/07/03    General cleanup
* 2.30  JMK      11/26/03    Added cl_min, cl_max, wl_min and wl_max parameters.
*                            Added al_min and al_max parameters.
*                            Removed support for OCD.
* 2.40  JMK      01/15/04    Removed verilog 2001 constructs.
* 2.50  JMK      01/29/04    Removed TRP checks during Precharge command.
* 2.60  JMK      04/20/04    Fixed tWTR check.
* 2.70  JMK      04/30/04    Added tRFC maximum check.
*                            Combined Self Refresh and Power Down always blocks.
*                            Added Reset Function (cke LOW Anytime).
* 2.80  JMK      08/19/04    Precharge is treated as NOP when bank is not active.  
*                            Added checks for tRAS, tWR, tRTP to any bank during Pre-All.
*                            tRFC maximum violation will only display one time.
* 2.90  JMK      11/05/04    Fixed dqs checking during write.
*                            Fixed false tRFC max assertion during power up and self ref.
*                            Added warning for 200us cke low time during initialization.
*                            Added -3, -3E, and -37V speed grades to ddr2_parameters.v
* 3.00  JMK      04/22/05    Removed odt off requirement during power down.
*                            Added tAOND, tAOFD, tANPD, tAXPD, tAONPD, and tAOFPD parameters.
*                            Added odt status messages.
*                            Updated the initialization sequence.
*                            Disable odt and clk pins during self refresh.
*                            Disable cmd and addr pins during power down and self refresh.
* 3.10  JMK      06/07/05    Disable TRPA checking if the part does not have 8 banks.
*                            Changed tAXPD message from error to a warning.
*                            Added tDSS checking.
*                            Removed tDQSL checking during tWPRE and tWPST.
*                            Fixed an burst order error during writes.
*                            Renamed parameters file with .vh extension.
* 3.20  JMK      07/18/05    Removed 14 TCK requirement from LMR to READ.
* 3.30  JMK      08/03/05    Added check for interrupting a burst with auto precharge.
* 4.00  JMK      11/21/05    Parameter names all UPPERCASE, signal names all lowercase.
*                            Clock jitter can be tolerated within specification range.
*                            Clock frequency is sampled from the CK pin.
*                            Scaleable up to 64 dq and 16 dqs bits.
*                            Read data can be randomly skewed using RANDOM_OUT_DELAY.
*                            Parameterized read and write dqs, and read dq.
*                            Initialization can be bypassed using initialize task.
* 4.10  JMK      11/30/05    Fixed compile errors when `MAX_MEM was defined.
* 4.20  JMK      12/09/05    Fixed memory addressing error when `MAX_MEM was defined.
* 4.30  JMK      02/15/06    Added dummy write to initialization sequence.
*                            Removed tWPST maximum checking.
*                            Rising dqs_n edge latches data when enabled in EMR.
*                            Fixed a sign error in the tJIT(cc) calculation.
* 4.40  JMK      02/16/06    Fixed dummy write when`MAX_MEM was defined.
* 4.50  JMK      02/27/06    Fixed extra tDQSS assertions.
*                            Fixed tRCD and tWTR checking.
*                            Errors entering Power Down or Self Refresh will cause reset.
*                            Ignore dqs_n when disabled in EMR.
* 5.00  JMK      04/24/06    Test stimulus now included from external file (subtest.vh)
*                            Fixed tRFC max assertion during self refresh.
*                            Fixed tANPD checking during Power Down.
*                            Removed dummy write from initialization sequence.
* 5.01  JMK      04/28/06    Fixed Auto Precharge to Load Mode, Refresh and Self Refresh.
*                            Removed Auto Precharge error message during Power Down Enter.
* 5.10  JMK      07/26/06    Created internal clock using ck and ck_n.
*                            RDQS can only be enabled in EMR for x8 configurations.
*                            CAS latency is checked vs frequency when DLL locks.
*                            tMOD changed from tCK units to ns units.
*                            Added 50 Ohm setting for Rtt in EMR.
*                            Improved checking of DQS during writes.
* 5.20  JMK      10/02/06    Fixed DQS checking for interrupting write to write and x16.
****************************************************************************************/

// DO NOT CHANGE THE TIMESCALE

`timescale 1ps / 1ps

module tb;

`include "ddr2_parameters.vh"

    reg                         clk         ;
    reg                         clk_n       ;
    reg                         cke         ;
    reg                         cs_n        ;
    reg                         ras_n       ;
    reg                         cas_n       ;
    reg                         we_n        ;
    reg       [BA_BITS - 1 : 0] ba          ;
    reg     [ADDR_BITS - 1 : 0] a           ;
    reg                         odt         ;
    reg                         dq_en       ;
    reg       [DM_BITS - 1 : 0] dm_out      ;
    reg       [DQ_BITS - 1 : 0] dq_out      ;
    reg       [DQ_BITS - 1 : 0] dm_fifo [0 : 2*(AL_MAX+CL_MAX)];
    reg       [DQ_BITS - 1 : 0] dq_fifo [0 : 2*(AL_MAX+CL_MAX)];
    reg                [64 : 0] dq_in_pos   ;
    reg                [64 : 0] dq_in_neg   ;
    reg                         dqs_en      ;
    reg      [DQS_BITS - 1 : 0] dqs_out     ;

    reg                [12 : 0] mode_reg    ;                   //Mode Register
    reg                [12 : 0] ext_mode_reg;                   //Extended Mode Register

    wire                        BO       = mode_reg[3];         //Burst Order
    wire                [7 : 0] BL       = (1<<mode_reg[2:0]);  //Burst Length
    wire                [2 : 0] CL       =     mode_reg[6:4];   //CAS Latency
    wire                        rdqs_en  = ext_mode_reg[11];    //RDQS Enable
    wire                        dqs_n_en = ~ext_mode_reg[10];   //dqs# Enable
    wire                [2 : 0] AL       = ext_mode_reg[5:3];   //Additive Latency

    wire                [3 : 0] RL       = AL + CL          ;   //Read Latency
    wire                [3 : 0] WL       = AL + CL - 1'b1   ;   //Write Latency

    wire      [DM_BITS - 1 : 0] dm_rdqs  = dq_en ? dm_out : {DM_BITS{1'bz}};
    wire      [DQ_BITS - 1 : 0] dq       = dq_en ? dq_out : {DQ_BITS{1'bz}};
    wire     [DQS_BITS - 1 : 0] dqs      = dqs_en ? dqs_out : {DQS_BITS{1'bz}};
    wire     [DQS_BITS - 1 : 0] dqs_n    = (dqs_en & dqs_n_en) ? ~dqs_out : {DQS_BITS{1'bz}};
    wire     [DQS_BITS - 1 : 0] rdqs_n   = {DM_BITS{1'bz}};
    
    wire               [15 : 0] dqs_in   = rdqs_en ? dm_rdqs : dqs;
    wire               [64 : 0] dq_in    = dq;

    ddr2 sdramddr2 (
        clk     , 
        clk_n   , 
        cke     , 
        cs_n    , 
        ras_n   , 
        cas_n   , 
        we_n    , 
        dm_rdqs , 
        ba      , 
        a       , 
        dq      , 
        dqs     ,
        dqs_n   ,
        rdqs_n  ,
        odt
    );

    // timing definition in tCK units
    real    tck   ;
    integer tccd  ;
    integer trc   ;
	integer trrd  ;
    integer trcd  ;
    integer tfaw  ;
    integer tras  ;
	integer trtp  ;
	integer twr   ;
	integer twtr  ;
    integer trp   ;
    integer tmrd  ;
    integer trfc  ;
    integer txsnr ;
    integer txsrd ;
    integer taond ;
    integer taofd ;
    integer tanpd ;
    integer taxpd ;
	integer tmod  ;
    integer txard ;
    integer txards;
    integer txp   ;
	integer tcke  ;
    integer tdllk ;

    initial begin
`ifdef period
        tck = `period ; 
`else
        tck =  TCK_MIN;
`endif
        tccd   = TCCD;
        trc    = TRC;
	    trrd   = ciel(TRRD/tck);
        trcd   = ciel(TRCD/tck);
        tfaw   = ciel(TFAW/tck);
        tras   = ciel(TRAS_MIN/tck);
	    trtp   = ciel(TRTP/tck);
	    twr    = ciel(TWR/tck);
	    twtr   = ciel(TWTR/tck);
        trp    = ciel(TRP/tck);
        tmrd   = TMRD;
        trfc   = ciel(TRFC_MIN/tck);
        txsnr  = ciel(TXSNR/tck);
        txsrd  = TXSRD;
        taond  = TAOND;
        taofd  = ciel(TAOFD);
        tanpd  = TANPD;
        taxpd  = TAXPD;
	    tmod   = ciel(TMOD/tck);
        txard  = TXARD;
        txards = TXARDS - AL;
        txp    = TXP;
	    tcke   = TCKE;
        tdllk  = TDLLK;
    end

    initial clk <= 1'b1;
    initial clk_n <= 1'b0;
    always @(posedge clk) begin
      clk   <= #(tck/2) 1'b0;
      clk_n <= #(tck/2) 1'b1;
      clk   <= #(tck) 1'b1;
      clk_n <= #(tck) 1'b0;
    end

    function integer ciel;
        input number;
        real number;
        if (number > $rtoi(number))
            ciel = $rtoi(number) + 1;
        else
            ciel = number;
    endfunction

    task power_up;
        begin
            cke    <=  1'b0;
            odt    <=  1'b0;
            repeat(10) @(negedge clk);
            cke    <=  1'b1;
            nop (400000/tck+1);
        end
    endtask

    task load_mode;
        input   [BA_BITS - 1 : 0] bank;
        input [ADDR_BITS - 1 : 0] addr;
        begin
            case (bank)
                0:     mode_reg = addr;
                1: ext_mode_reg = addr;
            endcase
            cke     = 1'b1;
            cs_n    = 1'b0;
            ras_n   = 1'b0;
            cas_n   = 1'b0;
            we_n    = 1'b0;
            ba      = bank;
            a       = addr;
            @(negedge clk);
        end
    endtask

    task refresh;
        begin
            cke     =  1'b1;
            cs_n    =  1'b0;
            ras_n   =  1'b0;
            cas_n   =  1'b0;
            we_n    =  1'b1;
            @(negedge clk);
        end
    endtask
     
    task self_refresh;
        input [31:0] count;
        integer count;
        begin
            cke     =  1'b0;
            cs_n    =  1'b0;
            ras_n   =  1'b0;
            cas_n   =  1'b0;
            we_n    =  1'b1;
            repeat(count) @(negedge clk);
        end
    endtask

    task precharge;
        input   [BA_BITS - 1 : 0] bank;
        input                     ap; //precharge all
        begin
            cke     = 1'b1;
            cs_n    = 1'b0;
            ras_n   = 1'b0;
            cas_n   = 1'b1;
            we_n    = 1'b0;
            ba      = bank;
            a       = (ap<<10);
            @(negedge clk);
        end
    endtask
     
    task activate;
        input   [BA_BITS - 1 : 0] bank;
        input  [ROW_BITS - 1 : 0] row;
        begin
            cke     = 1'b1;
            cs_n    = 1'b0;
            ras_n   = 1'b0;
            cas_n   = 1'b1;
            we_n    = 1'b1;
            ba      = bank;
            a       =  row;
            @(negedge clk);
        end
    endtask

    //write task supports burst lengths <= 16
    task write;
        input      [BA_BITS-1 : 0] bank;
        input   [COL_BITS - 1 : 0] col;
        input                      ap; //Auto Precharge
        input [16*DQ_BITS - 1 : 0] dm;
        input [16*DQ_BITS - 1 : 0] dq;
        reg    [ADDR_BITS - 1 : 0] atemp [1:0];
        reg      [DQ_BITS/DM_BITS - 1 : 0] dm_temp;
        integer i,j;
        begin
            cke     = 1'b1;
            cs_n    = 1'b0;
            ras_n   = 1'b1;
            cas_n   = 1'b0;
            we_n    = 1'b0;
            ba      = bank;
            atemp[0] = col & 10'h3ff;   //addr[ 9: 0] = COL[ 9: 0]
            atemp[1] = (col>>10)<<11;   //addr[ N:11] = COL[ N:10]
            a = atemp[0] | atemp[1] | (ap<<10);


            dm_out <= #(WL*tck + 0*tck/2 + tck/4) {DM_BITS{1'b0}};
            for (i=0; i<=BL; i=i+1) begin

                dqs_en <= #(WL*tck + i*tck/2) 1'b1;
                if (i%2 == 0) begin
                    dqs_out <= #(WL*tck + i*tck/2) {DQS_BITS{1'b0}};
                end else begin
                    dqs_out <= #(WL*tck + i*tck/2) {DQS_BITS{1'b1}};
                end

                dq_en  <= #(WL*tck + i*tck/2 + tck/4) 1'b1;
                for (j=0; j<DM_BITS; j=j+1) begin
                    dm_temp = dm>>((i*DM_BITS + j)*DQ_BITS/DM_BITS);
                    dm_out[j] <= #(WL*tck + i*tck/2 + tck/4) &dm_temp;
                end
                dq_out <= #(WL*tck + i*tck/2 + tck/4) dq>>i*DQ_BITS;
            end
            dqs_en <= #(WL*tck + BL*tck/2 + tck/2) 1'b0;
            dq_en  <= #(WL*tck + BL*tck/2 + tck/4) 1'b0;
            @(negedge clk);  
        end
    endtask

    // read without data verification
    task read;
        input    [BA_BITS - 1 : 0] bank;
        input   [COL_BITS - 1 : 0] col;
        input                      ap; //Auto Precharge
        reg    [ADDR_BITS - 1 : 0] atemp [1:0];
        begin
            cke     = 1'b1;
            cs_n    = 1'b0;
            ras_n   = 1'b1;
            cas_n   = 1'b0;
            we_n    = 1'b1;
            ba      = bank;
            atemp[0] = col & 10'h3ff;   //addr[ 9: 0] = COL[ 9: 0]
            atemp[1] = (col>>10)<<11;   //addr[ N:11] = COL[ N:10]
            a = atemp[0] | atemp[1] | (ap<<10);
            @(negedge clk);
        end
    endtask

    // read with data verification
    task read_verify;
        input    [BA_BITS - 1 : 0] bank;
        input   [COL_BITS - 1 : 0] col;
        input                      ap; //Auto Precharge
        input [16*DQ_BITS - 1 : 0] dm; //Expected Data Mask
        input [16*DQ_BITS - 1 : 0] dq; //Expected Data
        integer i;
        begin
            read (bank, col, ap);
            for (i=0; i<BL; i=i+1) begin
                dm_fifo[2*RL + i] = dm >> (i*DQ_BITS);
                dq_fifo[2*RL + i] = dq >> (i*DQ_BITS);
            end
        end
    endtask

    task nop;
        input [31:0] count;
        integer count;
        begin
            cke     =  1'b1;
            cs_n    =  1'b0;
            ras_n   =  1'b1;
            cas_n   =  1'b1;
            we_n    =  1'b1;
            repeat(count) @(negedge clk);
        end
    endtask

    task deselect;
        input [31:0] count;
        integer count;
        begin
            cke     =  1'b1;
            cs_n    =  1'b1;
            ras_n   =  1'b1;
            cas_n   =  1'b1;
            we_n    =  1'b1;
            repeat(count) @(negedge clk);
        end
    endtask

    task power_down;
        input [31:0] count;
        integer count;
        begin
            cke     =  1'b0;
            cs_n    =  1'b1;
            ras_n   =  1'b1;
            cas_n   =  1'b1;
            we_n    =  1'b1;
            repeat(count) @(negedge clk);
        end
    endtask

    function [16*DQ_BITS - 1 : 0] sort_data;
        input [16*DQ_BITS - 1 : 0] dq;
        input [2:0] col;
        integer i;
        reg   [2:0] brst_col;
        reg   [DQ_BITS - 1 :0] burst;
        begin
            sort_data = 0;
            for (i=0; i<BL; i=i+1) begin
                // perform burst ordering
                brst_col = col ^ i;
                if (!BO) begin
                    brst_col[1:0] = col + i;
                end
                burst = dq >> (brst_col*DQ_BITS);
                sort_data = sort_data | burst<<(i*DQ_BITS);
            end
        end
    endfunction

    // receiver(s) for data_verify process
    always @(dqs_in[0]) begin #(TDQSQ); dqs_receiver(0); end
    always @(dqs_in[1]) begin #(TDQSQ); dqs_receiver(1); end
    always @(dqs_in[2]) begin #(TDQSQ); dqs_receiver(2); end
    always @(dqs_in[3]) begin #(TDQSQ); dqs_receiver(3); end
    always @(dqs_in[4]) begin #(TDQSQ); dqs_receiver(4); end
    always @(dqs_in[5]) begin #(TDQSQ); dqs_receiver(5); end
    always @(dqs_in[6]) begin #(TDQSQ); dqs_receiver(6); end
    always @(dqs_in[7]) begin #(TDQSQ); dqs_receiver(7); end

    task dqs_receiver;
    input [31:0] i;
    integer i;
    begin
        if (dqs_in[i]) begin
            case (i)
                0: dq_in_pos[ 7: 0] <= dq_in[ 7: 0];
                1: dq_in_pos[15: 8] <= dq_in[15: 8];
                2: dq_in_pos[23:16] <= dq_in[23:16];
                3: dq_in_pos[31:24] <= dq_in[31:24];
                4: dq_in_pos[39:32] <= dq_in[39:32];
                5: dq_in_pos[47:40] <= dq_in[47:40];
                6: dq_in_pos[55:48] <= dq_in[55:48];
                7: dq_in_pos[63:56] <= dq_in[63:56];
            endcase
        end else if (!dqs_in[i]) begin
            case (i)
                0: dq_in_neg[ 7: 0] <= dq_in[ 7: 0];
                1: dq_in_neg[15: 8] <= dq_in[15: 8];
                2: dq_in_neg[23:16] <= dq_in[23:16];
                3: dq_in_neg[31:24] <= dq_in[31:24];
                4: dq_in_neg[39:32] <= dq_in[39:32];
                5: dq_in_neg[47:40] <= dq_in[47:40];
                6: dq_in_neg[55:48] <= dq_in[55:48];
                7: dq_in_neg[63:56] <= dq_in[63:56];
            endcase
        end
    end
    endtask
    
    // perform data verification as a result of read_verify task call
    always @(clk) begin : data_verify
        integer i;
        reg [DQ_BITS-1 : 0] bit_mask;
        
        for (i=0; i<=2*(AL_MAX+CL_MAX); i=i+1) begin
            dm_fifo[i] = dm_fifo[i+1];
            dq_fifo[i] = dq_fifo[i+1];
        end
        bit_mask = ~dm_fifo[0];
        if (clk) begin
            if ((dq_in_neg & bit_mask) != (dq_fifo[0] & bit_mask))
                $display ("%m at time %t: ERROR: Read data miscompare: Expected = %h, Actual = %h, Mask = %h", $time, dq_fifo[0], dq_in_neg, bit_mask);
        end else begin
            if ((dq_in_pos & bit_mask) != (dq_fifo[0] & bit_mask))
                $display ("%m at time %t: ERROR: Read data miscompare: Expected = %h, Actual = %h, Mask = %h", $time, dq_fifo[0], dq_in_pos, bit_mask);
        end
    end

    reg test_done;
	initial test_done = 0;

    // End-of-test triggered in 'subtest.vh'
    always @(test_done) begin : all_done
		if (test_done == 1) begin
      #5000
      $display ("Simulation is Complete");
			$stop(0);
			$finish;
		end
	end

	// Test included from external file
    `include "subtest.vh"

endmodule
