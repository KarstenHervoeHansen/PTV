/*******************************************************************************
*     This file is owned and controlled by Xilinx and must be used             *
*     solely for design, simulation, implementation and creation of            *
*     design files limited to Xilinx devices or technologies. Use              *
*     with non-Xilinx devices or technologies is expressly prohibited          *
*     and immediately terminates your license.                                 *
*                                                                              *
*     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"            *
*     SOLELY FOR USE IN DEVELOPING PROGRAMS AND SOLUTIONS FOR                  *
*     XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION          *
*     AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, APPLICATION              *
*     OR STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS                *
*     IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,                  *
*     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE         *
*     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY                 *
*     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE                  *
*     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR           *
*     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF          *
*     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          *
*     FOR A PARTICULAR PURPOSE.                                                *
*                                                                              *
*     Xilinx products are not intended for use in life support                 *
*     appliances, devices, or systems. Use in such applications are            *
*     expressly prohibited.                                                    *
*                                                                              *
*     (c) Copyright 1995-2004 Xilinx, Inc.                                     *
*     All rights reserved.                                                     *
*******************************************************************************/
// The synopsys directives "translate_off/translate_on" specified below are
// supported by XST, FPGA Compiler II, Mentor Graphics and Synplicity synthesis
// tools. Ensure they are correct for your synthesis tool(s).

// You must compile the wrapper file mathtest.v when simulating
// the core, mathtest. When compiling the wrapper file, be sure to
// reference the XilinxCoreLib Verilog simulation library. For detailed
// instructions, please refer to the "CORE Generator Help".

`timescale 1ns/1ps

module mathtest(
	dividend,
	divisor,
	quot,
	remd,
	clk,
	rfd,
	aclr,
	sclr,
	ce);


input [31 : 0] dividend;
input [31 : 0] divisor;
output [31 : 0] quot;
output [31 : 0] remd;
input clk;
output rfd;
input aclr;
input sclr;
input ce;

// synopsys translate_off

      SDIVIDER_V3_0 #(
		0,	// c_has_aclr
		0,	// c_has_ce
		0,	// c_has_sclr
		1,	// c_sync_enable
		8,	// divclk_sel
		32,	// dividend_width
		32,	// divisor_width
		0,	// fractional_b
		32,	// fractional_width
		0)	// signed_b
	inst (
		.DIVIDEND(dividend),
		.DIVISOR(divisor),
		.QUOT(quot),
		.REMD(remd),
		.CLK(clk),
		.RFD(rfd),
		.ACLR(aclr),
		.SCLR(sclr),
		.CE(ce));


// synopsys translate_on

// FPGA Express black box declaration
// synopsys attribute fpga_dont_touch "true"
// synthesis attribute fpga_dont_touch of mathtest is "true"

// XST black box declaration
// box_type "black_box"
// synthesis attribute box_type of mathtest is "black_box"

endmodule

