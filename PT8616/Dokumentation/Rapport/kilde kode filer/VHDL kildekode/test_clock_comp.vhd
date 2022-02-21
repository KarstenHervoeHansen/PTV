
--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   09:26:20 03/21/2007
-- Design Name:   clock_compensate
-- Module Name:   test_clock_comp.vhd
-- Project Name:  xc3s100c
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: clock_compensate
--
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends 
-- that these types always be used for the top-level I/O of a design in order 
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.ALL;

ENTITY test_clock_comp_vhd IS
END test_clock_comp_vhd;

ARCHITECTURE behavior OF test_clock_comp_vhd IS 

	-- Component Declaration for the Unit Under Test (UUT)
	COMPONENT clock_compensate
	PORT(
		rst_i : IN std_logic;
		clk_i : IN std_logic;
		pps_i : IN std_logic;          
		synth_pps_o : OUT std_logic;
		phase_diff_o : OUT std_logic_vector(7 downto 0);
		faster_tick : OUT std_logic;
		slower_tick : OUT std_logic
		);
	END COMPONENT;

	--Inputs
	SIGNAL rst_i :  std_logic := '0';
	SIGNAL clk_i :  std_logic := '0';
	SIGNAL pps_i :  std_logic := '0';

	--Outputs
	SIGNAL synth_pps_o :  std_logic;
	SIGNAL phase_diff_o :  std_logic_vector(7 downto 0);
	SIGNAL faster_tick :  std_logic;
	SIGNAL slower_tick :  std_logic;

BEGIN

	-- Instantiate the Unit Under Test (UUT)
	uut: clock_compensate PORT MAP(
		rst_i => rst_i,
		clk_i => clk_i,
		pps_i => pps_i,
		synth_pps_o => synth_pps_o,
		phase_diff_o => phase_diff_o,
		faster_tick => faster_tick,
		slower_tick => slower_tick
	);

	vcxo_gen: process
	begin
		--for i in 1 to 1000 loop
			wait for 250 us; --3367
			clk_i <= NOT clk_i;
			wait for 250 us;
			clk_i <= NOT clk_i;
		--end loop;
	end process;
	
	pps_gen: process
	begin
		--for i in 1 to 1000 loop
			wait for 4900 us; --3367
			pps_i <= NOT pps_i;
			wait for 4900 us;
			pps_i <= NOT pps_i;
		--end loop;
	end process;
	
	tb : PROCESS
	BEGIN
		rst_i <= '1';
		-- Wait 100 ns for global reset to finish
		wait for 100 ns;
		rst_i <= '0';
		-- Place stimulus here

		wait; -- will wait forever
	END PROCESS;

END;
