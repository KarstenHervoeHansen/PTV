////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2010 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: M.81d
//  \   \         Application: netgen
//  /   /         Filename: PLL_gain.v
// /___/   /\     Timestamp: Mon Jun 06 10:57:34 2011
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -intstyle ise -w -sim -ofmt verilog ./tmp/_cg\PLL_gain.ngc ./tmp/_cg\PLL_gain.v 
// Device	: 3s250evq100-5
// Input file	: ./tmp/_cg/PLL_gain.ngc
// Output file	: ./tmp/_cg/PLL_gain.v
// # of Modules	: 1
// Design Name	: PLL_gain
// Xilinx        : C:\Xilinx\12.4\ISE_DS\ISE\
//             
// Purpose:    
//     This verilog netlist is a verification model and uses simulation 
//     primitives which may not represent the true implementation of the 
//     device, however the netlist is functionally correct and should not 
//     be modified. This file cannot be synthesized and should only be used 
//     with supported simulation tools.
//             
// Reference:  
//     Command Line Tools User Guide, Chapter 23 and Synthesis and Simulation Design Guide, Chapter 6
//             
////////////////////////////////////////////////////////////////////////////////

`timescale 1 ns/1 ps

module PLL_gain (
a, b, p
)/* synthesis syn_black_box syn_noprune=1 */;
  input [9 : 0] a;
  input [17 : 0] b;
  output [27 : 0] p;
  
  // synthesis translate_off
  
  wire \blk00000003/sig0000003b ;
  wire \blk00000003/sig00000002 ;
  wire \blk00000003/sig00000001 ;
  wire NLW_blk00000001_P_UNCONNECTED;
  wire NLW_blk00000002_G_UNCONNECTED;
  wire \NLW_blk00000003/blk00000006_P<34>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_P<33>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_P<32>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_P<31>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_P<30>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_P<29>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_P<28>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_P<27>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<17>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<16>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<15>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<14>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<13>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<12>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<11>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<10>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<9>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<8>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<7>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<6>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<5>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<4>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<3>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<2>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<1>_UNCONNECTED ;
  wire \NLW_blk00000003/blk00000006_BCOUT<0>_UNCONNECTED ;
  wire [9 : 0] a_0;
  wire [17 : 0] b_1;
  wire [27 : 0] p_2;
  assign
    a_0[9] = a[9],
    a_0[8] = a[8],
    a_0[7] = a[7],
    a_0[6] = a[6],
    a_0[5] = a[5],
    a_0[4] = a[4],
    a_0[3] = a[3],
    a_0[2] = a[2],
    a_0[1] = a[1],
    a_0[0] = a[0],
    b_1[17] = b[17],
    b_1[16] = b[16],
    b_1[15] = b[15],
    b_1[14] = b[14],
    b_1[13] = b[13],
    b_1[12] = b[12],
    b_1[11] = b[11],
    b_1[10] = b[10],
    b_1[9] = b[9],
    b_1[8] = b[8],
    b_1[7] = b[7],
    b_1[6] = b[6],
    b_1[5] = b[5],
    b_1[4] = b[4],
    b_1[3] = b[3],
    b_1[2] = b[2],
    b_1[1] = b[1],
    b_1[0] = b[0],
    p[27] = p_2[27],
    p[26] = p_2[26],
    p[25] = p_2[25],
    p[24] = p_2[24],
    p[23] = p_2[23],
    p[22] = p_2[22],
    p[21] = p_2[21],
    p[20] = p_2[20],
    p[19] = p_2[19],
    p[18] = p_2[18],
    p[17] = p_2[17],
    p[16] = p_2[16],
    p[15] = p_2[15],
    p[14] = p_2[14],
    p[13] = p_2[13],
    p[12] = p_2[12],
    p[11] = p_2[11],
    p[10] = p_2[10],
    p[9] = p_2[9],
    p[8] = p_2[8],
    p[7] = p_2[7],
    p[6] = p_2[6],
    p[5] = p_2[5],
    p[4] = p_2[4],
    p[3] = p_2[3],
    p[2] = p_2[2],
    p[1] = p_2[1],
    p[0] = p_2[0];
  VCC   blk00000001 (
    .P(NLW_blk00000001_P_UNCONNECTED)
  );
  GND   blk00000002 (
    .G(NLW_blk00000002_G_UNCONNECTED)
  );
  MULT18X18SIO #(
    .AREG ( 0 ),
    .BREG ( 0 ),
    .B_INPUT ( "DIRECT" ),
    .PREG ( 0 ))
  \blk00000003/blk00000006  (
    .CEA(\blk00000003/sig0000003b ),
    .CEB(\blk00000003/sig0000003b ),
    .CEP(\blk00000003/sig0000003b ),
    .CLK(\blk00000003/sig00000001 ),
    .RSTA(\blk00000003/sig00000002 ),
    .RSTB(\blk00000003/sig00000002 ),
    .RSTP(\blk00000003/sig00000002 ),
    .A({b_1[17], b_1[16], b_1[15], b_1[14], b_1[13], b_1[12], b_1[11], b_1[10], b_1[9], b_1[8], b_1[7], b_1[6], b_1[5], b_1[4], b_1[3], b_1[2], b_1[1]
, b_1[0]}),
    .B({a_0[9], a_0[9], a_0[9], a_0[9], a_0[9], a_0[9], a_0[9], a_0[9], a_0[9], a_0[8], a_0[7], a_0[6], a_0[5], a_0[4], a_0[3], a_0[2], a_0[1], a_0[0]
}),
    .BCIN({\blk00000003/sig00000002 , \blk00000003/sig00000002 , \blk00000003/sig00000002 , \blk00000003/sig00000002 , \blk00000003/sig00000002 , 
\blk00000003/sig00000002 , \blk00000003/sig00000002 , \blk00000003/sig00000002 , \blk00000003/sig00000002 , \blk00000003/sig00000002 , 
\blk00000003/sig00000002 , \blk00000003/sig00000002 , \blk00000003/sig00000002 , \blk00000003/sig00000002 , \blk00000003/sig00000002 , 
\blk00000003/sig00000002 , \blk00000003/sig00000002 , \blk00000003/sig00000002 }),
    .P({p_2[27], \NLW_blk00000003/blk00000006_P<34>_UNCONNECTED , \NLW_blk00000003/blk00000006_P<33>_UNCONNECTED , 
\NLW_blk00000003/blk00000006_P<32>_UNCONNECTED , \NLW_blk00000003/blk00000006_P<31>_UNCONNECTED , \NLW_blk00000003/blk00000006_P<30>_UNCONNECTED , 
\NLW_blk00000003/blk00000006_P<29>_UNCONNECTED , \NLW_blk00000003/blk00000006_P<28>_UNCONNECTED , \NLW_blk00000003/blk00000006_P<27>_UNCONNECTED , 
p_2[26], p_2[25], p_2[24], p_2[23], p_2[22], p_2[21], p_2[20], p_2[19], p_2[18], p_2[17], p_2[16], p_2[15], p_2[14], p_2[13], p_2[12], p_2[11], 
p_2[10], p_2[9], p_2[8], p_2[7], p_2[6], p_2[5], p_2[4], p_2[3], p_2[2], p_2[1], p_2[0]}),
    .BCOUT({\NLW_blk00000003/blk00000006_BCOUT<17>_UNCONNECTED , \NLW_blk00000003/blk00000006_BCOUT<16>_UNCONNECTED , 
\NLW_blk00000003/blk00000006_BCOUT<15>_UNCONNECTED , \NLW_blk00000003/blk00000006_BCOUT<14>_UNCONNECTED , 
\NLW_blk00000003/blk00000006_BCOUT<13>_UNCONNECTED , \NLW_blk00000003/blk00000006_BCOUT<12>_UNCONNECTED , 
\NLW_blk00000003/blk00000006_BCOUT<11>_UNCONNECTED , \NLW_blk00000003/blk00000006_BCOUT<10>_UNCONNECTED , 
\NLW_blk00000003/blk00000006_BCOUT<9>_UNCONNECTED , \NLW_blk00000003/blk00000006_BCOUT<8>_UNCONNECTED , 
\NLW_blk00000003/blk00000006_BCOUT<7>_UNCONNECTED , \NLW_blk00000003/blk00000006_BCOUT<6>_UNCONNECTED , 
\NLW_blk00000003/blk00000006_BCOUT<5>_UNCONNECTED , \NLW_blk00000003/blk00000006_BCOUT<4>_UNCONNECTED , 
\NLW_blk00000003/blk00000006_BCOUT<3>_UNCONNECTED , \NLW_blk00000003/blk00000006_BCOUT<2>_UNCONNECTED , 
\NLW_blk00000003/blk00000006_BCOUT<1>_UNCONNECTED , \NLW_blk00000003/blk00000006_BCOUT<0>_UNCONNECTED })
  );
  VCC   \blk00000003/blk00000005  (
    .P(\blk00000003/sig0000003b )
  );
  GND   \blk00000003/blk00000004  (
    .G(\blk00000003/sig00000002 )
  );

// synthesis translate_on

endmodule

// synthesis translate_off

`ifndef GLBL
`define GLBL

`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;

    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (weak1, weak0) GSR = GSR_int;
    assign (weak1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

endmodule

`endif

// synthesis translate_on
