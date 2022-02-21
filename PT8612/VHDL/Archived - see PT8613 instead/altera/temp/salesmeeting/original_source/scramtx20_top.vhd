-- Copyright 2003-2004, Cypress Semiconductor Corporation

-- This SOFTWARE is owned by Cypress Semiconductor Corporation
-- (Cypress) and is protected by United States copyright laws and 
-- international treaty provisions.  Therefore, you must treat this 
-- SOFTWARE like any other copyrighted material (e.g., book, or musical 
-- recording), with the exception that one copy may be made for personal 
-- use or evaluation.  Reproduction, modification, translation, 
-- compilation, or representation of this software in any other form 
-- (e.g., paper, magnetic, optical, silicon, etc.) is prohibited 
-- without the express written permission of Cypress.  

-- This SOFTWARE is protected by and subject to worldwide patent
-- coverage, including U.S. and foreign patents. Use is limited by
-- and subject to the Cypress Software License Agreement.

-- SMPTE-292 scrambler mux/demux wrapper file

-- This design takes an 20 bit parallel data stream and
-- encodes it for serialization using the SMPTE scrambler algorithm.
-- This includes the x^9 + x^4 + 1 scrambler and the x + 1 NRZI
-- encoder.

----------------------------------------------------------------------------------------------------------
--
-- Revision History:
--	3-25-04	Original Draft
----------------------------------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY scram20_top IS
    PORT (
        txclk,                      -- HOTLink TX CKW clock
        reset,                      -- active high reset
        tx_oe,                      -- output enable
        bypass: IN std_logic;       -- raw data mode - bypass scrambler
	ten_twenty : in std_logic;  -- switch for determining either a 10 or 20 bit interface
        trs_filt_lower10: IN std_logic; -- TRS filter lower 10 bits
        trs_filt_upper10: IN std_logic; -- TRS filter upper 10 bits
        txdata_in: IN std_logic_vector(0 to 19); -- raw data in
        trs_det: out std_logic;     -- TRS code detected
        txdata_out: out std_logic_vector(0 to 9)); -- parallel scrambled output
END scram20_top;

----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------

ARCHITECTURE behavior of scram20_top IS

signal reset_sync, bypass_int, ten_twenty_sync, decode_normal : std_logic;
signal data_in_temp, data_temp_rise, data_temp_fall : std_logic_vector(0 to 21);
signal trs_filt_upper10_int1, trs_filt_upper10_int2, trs_filt_upper10_int3, trs_filt_upper10_int4,
       trs_filt_upper10_int5, trs_filt_upper10_int6, 
       trs_filt_lower10_int1, trs_filt_lower10_int2, trs_filt_lower10_int3, trs_filt_lower10_int4,
       trs_filt_lower10_int5, trs_filt_lower10_int6 : std_logic; 
signal txd_in_int0, txd_in_int1, txd_in_int2, txd_in_int3, txd_in_int4, txd_in_int5, txd_in_int6 : std_logic_vector(0 to 19);
signal trs_det_upper10_int, trs_det_lower10_int : std_logic;
signal txd_out : std_logic_vector(0 to 19);
signal data_out_temp_h, data_out_temp_l: std_logic_vector(0 to 10);
signal data_out_temp2 : std_logic_vector(0 to 10);

----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
 -- set to '1' to permanently create a 10-to-10 bit interface, which will optimize out the 20-bit logic
 -- set to zero to allow the ten_twenty input pin to determine whether a 10-20 bit or 10-10 bit interface
constant ten_twenty_param : std_logic := '0';

----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------

component altera_ddr_input22 PORT(
		datain		: IN STD_LOGIC_VECTOR (21 DOWNTO 0);
		inclock		: IN STD_LOGIC ;
		dataout_h		: OUT STD_LOGIC_VECTOR (21 DOWNTO 0);
		dataout_l		: OUT STD_LOGIC_VECTOR (21 DOWNTO 0));
END component;

----------------------------------------------------------------------------------------
component scram20 PORT(
        txclk,                      -- HOTLink TX CKW clock
        reset,                      -- active high reset added by DEF 11-6-03
        trs_filt_lower10: IN std_logic;     -- TRS filter
        trs_filt_upper10: IN std_logic;     -- TRS filter
        bypass: IN std_logic;       -- raw data mode - bypass scrambler
        txd_in: IN std_logic_vector(0 to 19);
        trs_out_lower10: out std_logic;  -- TRS code detected
        trs_out_upper10: out std_logic;  -- TRS code detected
        txd_out: out std_logic_vector(0 to 19)); -- parallel scrambled output
END component;

----------------------------------------------------------------------------------------
component altera_ddr_output11 PORT(
		datain_h		: IN STD_LOGIC_VECTOR (10 DOWNTO 0);
		datain_l		: IN STD_LOGIC_VECTOR (10 DOWNTO 0);
		outclock		: IN STD_LOGIC;
		oe		: IN STD_LOGIC;
		dataout		: OUT STD_LOGIC_VECTOR (10 DOWNTO 0));
END component;

----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------

BEGIN

----------------------------------------------------------------------------------------
--------- pre-assigning the TRS filter and TX data to the DDR input register -----------

data_in_temp <= txdata_in & trs_filt_lower10 & trs_filt_upper10;


----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
altera_ddr_input_inst : altera_ddr_input22 PORT MAP (
		datain	   => data_in_temp,
		inclock	   => txclk,
		dataout_h	 => data_temp_rise,
		dataout_l	 => data_temp_fall);
		
txd_in_int0 <= data_temp_rise(0 to 19);
		
----------------------------------------------------------------------------------------
scram20_inst : scram20 PORT MAP(
        txclk => txclk,                     
        reset => reset_sync,                    
        trs_filt_lower10 => trs_filt_lower10_int6,     
        trs_filt_upper10 => trs_filt_upper10_int6,     
        bypass => bypass_int,      
        txd_in => txd_in_int6,      
        trs_out_lower10 => trs_det_lower10_int,      
        trs_out_upper10 => trs_det_upper10_int,      
        txd_out => txd_out);      

----------------------------------------------------------------------------------------
altera_ddr_output_inst : altera_ddr_output11 PORT MAP (
		datain_h	 => data_out_temp_h,
		datain_l	 => data_out_temp_l,
		outclock	 => txclk,
		oe	  => tx_oe,
		dataout	  => data_out_temp2);
		
----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
synchronous_reset : process(txclk) begin
if rising_edge(txclk) then
    reset_sync <= reset;
    bypass_int <= bypass;
    ten_twenty_sync <= ten_twenty or ten_twenty_param;
end if;
end process;

----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
sequential_logic : process(txclk, reset_sync) begin
if (reset_sync = '1') then
	trs_filt_upper10_int1 <= '0';
	trs_filt_upper10_int2 <= '0';
	trs_filt_upper10_int3 <= '0';
	trs_filt_upper10_int4 <= '0';

	trs_filt_lower10_int1 <= '0';
	trs_filt_lower10_int2 <= '0';
	trs_filt_lower10_int3 <= '0';
	trs_filt_lower10_int4 <= '0';

	txd_in_int1 <= (others => '0');
	txd_in_int2 <= (others => '0');
	txd_in_int3 <= (others => '0');
	txd_in_int4 <= (others => '0');
	
	decode_normal <= '1';
elsif rising_edge(txclk) then
	trs_filt_upper10_int1 <= data_temp_rise(21);
	trs_filt_upper10_int2 <= trs_filt_upper10_int1;
	trs_filt_upper10_int3 <= trs_filt_upper10_int2;
	trs_filt_upper10_int4 <= trs_filt_upper10_int3;

	trs_filt_lower10_int1 <= data_temp_rise(20);
	trs_filt_lower10_int2 <= trs_filt_lower10_int1;
	trs_filt_lower10_int3 <= trs_filt_lower10_int2;
	trs_filt_lower10_int4 <= trs_filt_lower10_int3;

	txd_in_int1 <= txd_in_int0;
	txd_in_int2 <= txd_in_int1;
	txd_in_int3 <= txd_in_int2;
	txd_in_int4 <= txd_in_int3;
	
	if ((txd_in_int2 = "11111111111111111111") and (txd_in_int1 = "00000000000000000000") and 
	    (txd_in_int0 = "00000000000000000000")) then 
	    decode_normal  <= '1';
	elsif ((txd_in_int3(0 to 9) = "1111111111") and (txd_in_int2 = "11111111110000000000") and 
	       (txd_in_int1 = "00000000000000000000") and (txd_in_int0(10 to 19) = "0000000000")) then 
	    decode_normal  <= '0';
	end if;
end if;
end process;


----------------------------------------------------------------------------------------
-------------- 20-bit framing according to TRS -----------------------------------------
trs_filt_upper10_int5 <= trs_filt_upper10_int4 when (decode_normal = '1') else trs_filt_upper10_int3;
trs_filt_lower10_int5 <= trs_filt_lower10_int4;
txd_in_int5 <= txd_in_int4 when (decode_normal = '1') else (txd_in_int4(0 to 9) & txd_in_int3(10 to 19));


----------------------------------------------------------------------------------------
-------------- muxing for either 10 or 20-bit co-processor interface -------------------
trs_filt_upper10_int6 <= data_temp_fall(20) when (ten_twenty_sync = '1') else trs_filt_upper10_int5;
trs_filt_lower10_int6 <= data_temp_rise(20) when (ten_twenty_sync = '1') else trs_filt_lower10_int5;
txd_in_int6 <= (data_temp_rise(0 to 9) & data_temp_fall(0 to 9)) when (ten_twenty_sync = '1') else txd_in_int5;


----------------------------------------------------------------------------------------
-------------- send 20-bit scrambled data to DDR register ------------------------------
data_out_temp_h <= txd_out(0 to 9) & trs_det_lower10_int; -- sent out first
data_out_temp_l <= txd_out(10 to 19) & trs_det_upper10_int; -- sent out second


----------------------------------------------------------------------------------------
-------------- rename outputs after DDR register output --------------------------------
trs_det <= data_out_temp2(10);
txdata_out <= data_out_temp2(0 to 9);


END behavior;


----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------



--Copyright (C) 1991-2004 Altera Corporation
--Any  megafunction  design,  and related netlist (encrypted  or  decrypted),
--support information,  device programming or simulation file,  and any other
--associated  documentation or information  provided by  Altera  or a partner
--under  Altera's   Megafunction   Partnership   Program  may  be  used  only
--to program  PLD  devices (but not masked  PLD  devices) from  Altera.   Any
--other  use  of such  megafunction  design,  netlist,  support  information,
--device programming or simulation file,  or any other  related documentation
--or information  is prohibited  for  any  other purpose,  including, but not
--limited to  modification,  reverse engineering,  de-compiling, or use  with
--any other  silicon devices,  unless such use is  explicitly  licensed under
--a separate agreement with  Altera  or a megafunction partner.  Title to the
--intellectual property,  including patents,  copyrights,  trademarks,  trade
--secrets,  or maskworks,  embodied in any such megafunction design, netlist,
--support  information,  device programming or simulation file,  or any other
--related documentation or information provided by  Altera  or a megafunction
--partner, remains with Altera, the megafunction partner, or their respective
--licensors. No other licenses, including any licenses needed under any third
--party's intellectual property, are provided herein.


LIBRARY ieee;
USE ieee.std_logic_1164.all;

LIBRARY altera_mf;
USE altera_mf.altera_mf_components.all;

ENTITY altera_ddr_input22 IS
	PORT
	(
		datain		: IN STD_LOGIC_VECTOR (21 DOWNTO 0);
		inclock		: IN STD_LOGIC ;
		dataout_h		: OUT STD_LOGIC_VECTOR (21 DOWNTO 0);
		dataout_l		: OUT STD_LOGIC_VECTOR (21 DOWNTO 0)
	);
END altera_ddr_input22;


ARCHITECTURE SYN OF altera_ddr_input22 IS

	SIGNAL sub_wire0	: STD_LOGIC_VECTOR (21 DOWNTO 0);
	SIGNAL sub_wire1	: STD_LOGIC_VECTOR (21 DOWNTO 0);



	COMPONENT altddio_in
	GENERIC (
		width		: NATURAL;
		intended_device_family		: STRING;
		power_up_high		: STRING;
		lpm_type		: STRING
	);
	PORT (
			datain	: IN STD_LOGIC_VECTOR (21 DOWNTO 0);
			inclock	: IN STD_LOGIC ;
			dataout_h	: OUT STD_LOGIC_VECTOR (21 DOWNTO 0);
			dataout_l	: OUT STD_LOGIC_VECTOR (21 DOWNTO 0)
	);
	END COMPONENT;

BEGIN
	dataout_h    <= sub_wire0(21 DOWNTO 0);
	dataout_l    <= sub_wire1(21 DOWNTO 0);

	altddio_in_component : altddio_in
	GENERIC MAP (
		width => 22,
		intended_device_family => "Cyclone",
		power_up_high => "OFF",
		lpm_type => "altddio_in"
	)
	PORT MAP (
		datain => datain,
		inclock => inclock,
		dataout_h => sub_wire0,
		dataout_l => sub_wire1
	);



END SYN;



----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------


LIBRARY ieee;
USE ieee.std_logic_1164.all;

LIBRARY altera_mf;
USE altera_mf.altera_mf_components.all;

ENTITY altera_ddr_output11 IS
	PORT
	(
		datain_h		: IN STD_LOGIC_VECTOR (10 DOWNTO 0);
		datain_l		: IN STD_LOGIC_VECTOR (10 DOWNTO 0);
		outclock		: IN STD_LOGIC ;
		oe		: IN STD_LOGIC  := '1';
		dataout		: OUT STD_LOGIC_VECTOR (10 DOWNTO 0)
	);
END altera_ddr_output11;


ARCHITECTURE SYN OF altera_ddr_output11 IS

	SIGNAL sub_wire0	: STD_LOGIC_VECTOR (10 DOWNTO 0);



	COMPONENT altddio_out
	GENERIC (
		width		: NATURAL;
		power_up_high		: STRING;
		intended_device_family		: STRING;
		oe_reg		: STRING;
		extend_oe_disable		: STRING;
		lpm_type		: STRING
	);
	PORT (
			dataout	: OUT STD_LOGIC_VECTOR (10 DOWNTO 0);
			outclock	: IN STD_LOGIC ;
			oe	: IN STD_LOGIC ;
			datain_h	: IN STD_LOGIC_VECTOR (10 DOWNTO 0);
			datain_l	: IN STD_LOGIC_VECTOR (10 DOWNTO 0)
	);
	END COMPONENT;

BEGIN
	dataout    <= sub_wire0(10 DOWNTO 0);

	altddio_out_component : altddio_out
	GENERIC MAP (
		width => 11,
		power_up_high => "OFF",
		intended_device_family => "Cyclone",
		oe_reg => "REGISTERED",
		extend_oe_disable => "UNUSED",
		lpm_type => "altddio_out"
	)
	PORT MAP (
		outclock => outclock,
		oe => oe,
		datain_h => datain_h,
		datain_l => datain_l,
		dataout => sub_wire0
	);



END SYN;