XAPP682 version 1.1 reference design files:

Verilog Directory:
    multigenHD.v:         Top level of the video pattern generator
    multigenHD_horz.v:	  Horizontal section of video pattern generator
    multigenHD_vert.v:	  Vertical section of video pattern generator
    multigenHD_output.v:  Output section of video pattern generator
    multigenHD_romgen.v:  Verilog program that creates the block RAM init files

VHDL Directory:
    multigenHD.vhd:       Top level of the video pattern generator
    multigenHD_horz.vhd:  Horizontal section of video pattern generator
    multigenHD_vert.vhd:  Vertical section of video pattern generator
    multigenHD_output.vhd:Output section of video pattern generator
    multigenHD_pkg.vhd:   Package file required by the other multigen modules.

Revision History
----------------

Release 1.1: 2005/01/14:

The multigenHD_romgen.v file was generating 32-bit values for the VHDL generics
SRVAL and INIT when they should have been 36-bit. ModelSim 5.8x would flag this
as a fatal error. The romgen program has been updated to correctly produce 36-bit
values for these generics and the VHDL files that include the block RAM inits
created by romgen have been updated with 36-bit SRVAL and INIT values.

    





