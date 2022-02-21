XAPP248 version 1.1 reference design files:

Verilog directory:
  cb_eg1.v: SMPTE EG 1 color bar generator using distributed RAM
  cb_eg1_rp178.v SMPTE EG 1 and RP 178 generator using distributed RAM
  colorbars.v: non-standard, simplified version of EG 1 color bars using distributed RAM
  crom_NTSC_EG1_RP178_sim.V: Block RAM simulation init file for CROM in vidgen.v
  crom_NTSC_EG1_RP178_syn.v: Block RAM synthesis init file for CROM in vidgen.v
  hrom_NTSC_EG1_RP178_sim.v: Block RAM simulation init file for HROM in vidgen.v
  hrom_NTSC_EG1_RP178_syn.v: Block RAM synthesis init file for HROM in vidgen.v
  rp178.v: SMPTE RP 178 generator using distributed RAM
  testbench_cb1.v: An example test bench for test pattern generators
  vidgen.v: Block RAM-based video test pattern generator
  vrom_NTSC_EG1_RP178_sim.v: Block RAM simulation init file for VROM in vidgen.v
  vrom_NTSC_EG1_RP178_syn.v: Block RAM synthesis init file for VROM in vidgen.v

VHDL directory:
  Contains VHDL versions of the same files listed in the Verilog directory.

cbgen directory:
  bin sub-directory:
    cbgen.exe: Windows executable version of the cbgen utility to create vidgen block RAM init files
    NTSC_EG1_RP178.txt: NTSC EG 1 & RP 178 test pattern definition file for cbgen utility. This is
                        the source file used to generate the crom, hrom, and vrom init files in the
                        Verilog and VHDL directories.
    PAL_EG1_RP178.txt: PAL EG 1 & RP 178 test pattern definition file for cbgen utility
  
  src sub-directory:
    Contains all the C source code files for the cbgen utility


Revision History
----------------
Version 1.1 January 18, 2005

Clock enables were not properly implemented in the vidgen.v, vidgen.vhd, and 
cb_eg1_rp178.vhd files. This release fixes these clock enable issues. 

PAL versions of the vidgen block RAM init files were added.
