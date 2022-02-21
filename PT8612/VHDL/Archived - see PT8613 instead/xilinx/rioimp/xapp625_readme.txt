XAPP625 version 1.1 reference design files:

Verilog directory:
  autodetect.v: 		automatic video standard detector
  fly_field.v: 			field related logic for flywheel
  fly_fsm.v: 			finite state machine for flywheel
  fly_horz.v: 			horizontal related logic for flywheel
  fly_vert.v: 			vertical related logic for flywheel
  flywheel.v: 			top level module for video flywheel
  multigen.v: 			multi-standard video generator for testbench
  test_vid_decode.v: 	testbench for video decoder
  trs_detect.v: 		TRS & ANC detector module
  trs_detect_v2.v		more efficient implementation of trs_detect.v
  video_decode.v: 		top level module for video decoder

VHDL directory:
  Contains VHDL versions of the same files listed in the Verilog directory.

Revision History
----------------
January 18, 2005 Version 1.1: 

Fixed missing clock enable on output registers in flywheel.vhd file.

Added alternate TRS detector modules, trs_detect_v2.v and trs_detect_v2.vhd.
These are identical to the trs_detect modules in function, but are implemented
with LUT primitives to enforce the most efficient implementation.



