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

// You must compile the wrapper file color_rom.v when simulating
// the core, color_rom. When compiling the wrapper file, be sure to
// reference the XilinxCoreLib Verilog simulation library. For detailed
// instructions, please refer to the "CORE Generator Help".

`timescale 1ns/1ps

module color_rom(
	A,
	CLK,
	QSPO_CE,
	QSPO);


input [5 : 0] A;
input CLK;
input QSPO_CE;
output [35 : 0] QSPO;

// synopsys translate_off

      C_DIST_MEM_V7_1 #(
		6,	// c_addr_width
		"0",	// c_default_data
		2,	// c_default_data_radix
		64,	// c_depth
		1,	// c_enable_rlocs
		1,	// c_generate_mif
		1,	// c_has_clk
		0,	// c_has_d
		0,	// c_has_dpo
		0,	// c_has_dpra
		0,	// c_has_i_ce
		0,	// c_has_qdpo
		0,	// c_has_qdpo_ce
		0,	// c_has_qdpo_clk
		0,	// c_has_qdpo_rst
		0,	// c_has_qdpo_srst
		1,	// c_has_qspo
		1,	// c_has_qspo_ce
		0,	// c_has_qspo_rst
		0,	// c_has_qspo_srst
		0,	// c_has_rd_en
		0,	// c_has_spo
		0,	// c_has_spra
		0,	// c_has_we
		1,	// c_latency
		"color_rom.mif",	// c_mem_init_file
		0,	// c_mem_type
		0,	// c_mux_type
		0,	// c_qce_joined
		0,	// c_qualify_we
		1,	// c_read_mif
		0,	// c_reg_a_d_inputs
		0,	// c_reg_dpra_input
		0,	// c_sync_enable
		36)	// c_width
	inst (
		.A(A),
		.CLK(CLK),
		.QSPO_CE(QSPO_CE),
		.QSPO(QSPO),
		.D(),
		.DPRA(),
		.SPRA(),
		.I_CE(),
		.WE(),
		.QDPO_CE(),
		.QDPO_CLK(),
		.RD_EN(),
		.QSPO_RST(),
		.QDPO_RST(),
		.QSPO_SRST(),
		.QDPO_SRST(),
		.SPO(),
		.DPO(),
		.QDPO());


// synopsys translate_on

// FPGA Express black box declaration
// synopsys attribute fpga_dont_touch "true"
// synthesis attribute fpga_dont_touch of color_rom is "true"

// XST black box declaration
// box_type "black_box"
// synthesis attribute box_type of color_rom is "black_box"

endmodule

