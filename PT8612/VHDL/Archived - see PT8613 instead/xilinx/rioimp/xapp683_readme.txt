XAPP683 version 1.00 reference design files:

Verilog Directory:
    hdsdi_crc.v:           HD-SDI CRC generator
    hdsdi_insert_crc.v:    HD-SDI CRC insertion
    hdsdi_insert_ln.v:     HD-SDI line number insertion
    hdsdi_rio_refclk.v:    Wrapper around GT_CUSTOM RocketIO module 
                           with REFCLKs selected
    hdsdi_tx_crc_ln.v:     Contains the HD-SDI CRC generation & insertion
                           and line number insertion modules
    multi_sdi_bitrep_4x.v: Bit replicator for SD-SDI transmitter
    multi_sdi_encoder.v:   Multi-rate SDI encoder
    sdv_multi_sdi_tx.v:    Top-level multi-rate SDI transmitter reference
                           design for Xilinx SDV demo board
    sdv_multi_sdi_tx.ucf:  Constraints file for sdv_multi_sdi_tx design
    sdv_sdsdi_rio_tx.v:    Top-level SD-SDI only transmitter reference
                           design for Xilinx SDV demo board
    sdv_sdsdi_rio_tx.ucf:  Constraints file for sdv_sdsdi_rio_tx design
    smpte_encoder.v:       10-bit SDI encoder module used by multi_sdi_encoder

VHDL Directory:
    This directory contains VHDL versions of the sames files listed above.
    The only addition is the VHDL package file hdsdi_pkg.vhd that includes
    definitions used by all the VHDL modules.

The two top-level reference designs also require the design files from
the following Xilinx application notes:

XAPP682: HDTV Video Pattern Generator
XAPP248: SDTV Video Pattern Generator
XAPP299: SD-SDI EDH Processor
XAPP625: SD-SDI video decoder and flywheel
    





