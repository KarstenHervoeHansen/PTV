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

-- SMPTE-292 descrambler mux/demux wrapper file
----------------------------------------------------------------------------------------------------------
--
-- Revision History:
--	3-25-04	Original Draft
----------------------------------------------------------------------------------------------------------
 
LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY dscram20_top IS 
    PORT (
        rxclk,                      -- CY7B933 CKR recovered clock
        reset,                      -- active high reset
        rx_oe,                      -- output enable
        bypass: IN std_logic;       -- raw data mode, bypass scrambler data
	ten_twenty : in std_logic;  -- switch for determining either a 10 or 20 bit interface
        sync_en: IN std_logic;      -- TRS/SYNC correction enabled
        rxdata_in: IN std_logic_vector (0 to 9);   -- scrambled data from HOTLink RX
        rxdata_out: out std_logic_vector (0 to 19); -- descrambled and framed data
        sync_err: out std_logic;    -- SYNC offset error pulse
        h_sync: out std_logic);     -- horizontal sync output toggle
END dscram20_top;


ARCHITECTURE behavior OF dscram20_top IS

signal reset_sync, bypass_int, ten_twenty_sync, sync_en_int : std_logic;
signal rxd_in_int : std_logic_vector(0 to 19);
signal sync_err_int, h_sync_int : std_logic;
signal rxd_out_int : std_logic_vector(0 to 19);
signal data_temp_clk_rise, data_temp_clk_fall, data_temp_output : std_logic_vector(0 to 21);

----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
 -- set to '1' to permanently create a 10-to-10 bit interface, which will optimize out the 20-bit logic
 -- set to zero to allow the ten_twenty input pin to determine whether a 10-20 bit or 10-10 bit interface
constant ten_twenty_param : std_logic := '0';

----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------

component altera_ddr_input10 port(
		datain		: IN std_logic_vector (9 DOWNTO 0);
		inclock		: IN std_logic ;
		dataout_h		: OUT std_logic_vector (9 DOWNTO 0);
		dataout_l		: OUT std_logic_vector (9 DOWNTO 0));
end component;

----------------------------------------------------------------------------------------
component altera_ddr_output22 port(
		datain_h		: IN std_logic_vector (21 DOWNTO 0);
		datain_l		: IN std_logic_vector (21 DOWNTO 0);
		outclock		: IN std_logic;
		oe		: IN std_logic;
		dataout		: OUT std_logic_vector (21 DOWNTO 0));
end component;

----------------------------------------------------------------------------------------
component dscram20 port(
        rxclk,
        reset,                      -- active high reset added by DEF 11-6-03
        bypass: IN std_logic;       -- raw data mode, bypass scrambler data
        sync_en: IN std_logic;      -- TRS/SYNC correction enabled
        rxd_in: IN std_logic_vector (0 TO 19);   -- scrambled data from HOTLink RX
        rxd_out: out std_logic_vector (0 TO 19); -- descrambled and framed data
        sync_err: out std_logic;    -- SYNC offset error pulse
        h_sync: out std_logic);     -- horizontal sync output toggle
end component;

----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------


begin

----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
altera_ddr_input_inst10 : altera_ddr_input10 port map (
		datain	  => rxdata_in,
		inclock	  => rxclk,
		dataout_h => rxd_in_int(10 to 19),
		dataout_l => rxd_in_int(0 to 9));
	
----------------------------------------------------------------------------------------
dscram20_inst : dscram20 port map (
        rxclk => rxclk,                     
        reset => reset_sync,                    
        bypass => bypass_int,      
        sync_en => sync_en_int,
        rxd_in => rxd_in_int,
        rxd_out => rxd_out_int,
        sync_err => sync_err_int,
        h_sync => h_sync_int);

----------------------------------------------------------------------------------------
altera_ddr_output_inst22 : altera_ddr_output22 port map (
		datain_h	 => data_temp_clk_rise,
		datain_l	 => data_temp_clk_fall,
		outclock	 => rxclk,
		oe	  => rx_oe,
		dataout	  => data_temp_output);

----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------

synchronous_reset : process(rxclk) begin
if rising_edge(rxclk) then
    reset_sync <= reset;
    bypass_int <= bypass;
    ten_twenty_sync <= ten_twenty or ten_twenty_param;
    sync_en_int <= sync_en;
end if;
end process;

----------------------------------------------------------------------------------------
-------------- muxing for either 10 or 20-bit co-processor interface -------------------
data_temp_clk_rise <= (rxd_out_int(0 to 9) & "0000000000" & h_sync_int & sync_err_int) 
                       when (ten_twenty_sync = '1') else 
                       (rxd_out_int & h_sync_int & sync_err_int);
data_temp_clk_fall <= (rxd_out_int(10 to 19) & "0000000000" & h_sync_int & sync_err_int)
                       when (ten_twenty_sync = '1') else
                       (rxd_out_int & h_sync_int & sync_err_int);
	           
----------------------------------------------------------------------------------------
-------------- rename outputs after DDR register output --------------------------------
sync_err   <= data_temp_output(21);
h_sync     <= data_temp_output(20);
rxdata_out <= data_temp_output(0 to 19);
	
end behavior;


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

ENTITY altera_ddr_input10 IS
	PORT
	(
		datain		: IN std_logic_vector (9 DOWNTO 0);
		inclock		: IN std_logic ;
		dataout_h		: OUT std_logic_vector (9 DOWNTO 0);
		dataout_l		: OUT std_logic_vector (9 DOWNTO 0)
	);
END altera_ddr_input10;


ARCHITECTURE SYN OF altera_ddr_input10 IS

	SIGNAL sub_wire0	: std_logic_vector (9 DOWNTO 0);
	SIGNAL sub_wire1	: std_logic_vector (9 DOWNTO 0);



	COMPONENT altddio_in
	GENERIC (
		width		: NATURAL;
		intended_device_family		: STRING;
		power_up_high		: STRING;
		lpm_type		: STRING
	);
	PORT (
			datain	: IN std_logic_vector (9 DOWNTO 0);
			inclock	: IN std_logic ;
			dataout_h	: OUT std_logic_vector (9 DOWNTO 0);
			dataout_l	: OUT std_logic_vector (9 DOWNTO 0)
	);
	END COMPONENT;

BEGIN
	dataout_h    <= sub_wire0(9 DOWNTO 0);
	dataout_l    <= sub_wire1(9 DOWNTO 0);

	altddio_in_component : altddio_in
	GENERIC MAP (
		width => 10,
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

ENTITY altera_ddr_output22 IS
	PORT
	(
		datain_h		: IN std_logic_vector (21 DOWNTO 0);
		datain_l		: IN std_logic_vector (21 DOWNTO 0);
		outclock		: IN std_logic ;
		oe		: IN std_logic  := '1';
		dataout		: OUT std_logic_vector (21 DOWNTO 0)
	);
END altera_ddr_output22;


ARCHITECTURE SYN OF altera_ddr_output22 IS

	SIGNAL sub_wire0	: std_logic_vector (21 DOWNTO 0);



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
			dataout	: OUT std_logic_vector (21 DOWNTO 0);
			outclock	: IN std_logic ;
			oe	: IN std_logic ;
			datain_h	: IN std_logic_vector (21 DOWNTO 0);
			datain_l	: IN std_logic_vector (21 DOWNTO 0)
	);
	END COMPONENT;

BEGIN
	dataout    <= sub_wire0(21 DOWNTO 0);

	altddio_out_component : altddio_out
	GENERIC MAP (
		width => 22,
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
