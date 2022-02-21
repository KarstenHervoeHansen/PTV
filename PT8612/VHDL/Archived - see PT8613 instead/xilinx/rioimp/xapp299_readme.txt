XAPP299 version 1.1 reference design files:

Verilog directory:
  anc_demux.v: 			ANC packet demultiplexor module
  anc_edh_processor.v: 	top-level ANC & EDH processor module
  anc_extract.v: 		used in the anc_demux module to remove packets from video stream
  anc_insert.v: 		used in the anc_mux module to insert packets into video stream
  anc_mux.v: ANC 		packet multiplexor module
  anc_pkt_gen.v: 		used in the anc_mux module to format new ANC packets
  anc_rx.v: 			verifies the checksums of all received ANC packets
  edh_crc.v: 			calculates the AP & FF CRC values for each field
  edh_crc16.v: 			used by edh_crc module to do the actual 16-bit CRC calculations
  edh_crc16_v2.v:		more efficient version of edh_crc16.v
  edh_errcnt.v: 		counts the number of fields received with errors
  edh_flags.v: 			processes received EDH error flags and generates new EDH flags
  edh_gen.v: 			generates new or updates existing outgoing EDH packets   
  edh_loc.v: 			determines where EDH packets should be located in each field
  edh_processor.v: 		EDH-only processor with now ANC mux or demux function
  edh_rx.v: 			processes incoming EDH packets
  edh_tx.v: 			used by edh_gen to generate new EDH packets
  test_ae_edh.v: 		testbench that tests the EDH functions of anc_edh_processor
  test_anc_demux.v: 	testbench that tests the ANC demux functions of anc_edh_processor
  test_anc_mux.v: 		testbench that tests the ANC mux functions of anc_edh_processor
  test_edh.v: 			testbench that tests the edh_processor

VHDL directory:
  This directory contains VHDL versions of all the same files listed above plus:
  anc_edh_pkg.vhd: 		a VHDL package that defines data types and constants used by
                   		the various design files.

Also included is the file one_frame.zip. This file is required by the testbenches.

Note that the anc_edh_processor and edh_processor modules require the reference
design files from XAPP625.

Revision History
----------------

Version 1.1 January 18, 2005

The one_frame.zip file was added.

The edh_crc16_v2.v and edh_crc16_v2.vhd files were added. These are functionally
identical to the older edh_crc16 modules, but are implemented with LUT primitives
to enforce a more efficient implementation.
