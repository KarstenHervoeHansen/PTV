
--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   12:33:55 02/16/2007
-- Design Name:   digital_pll
-- Module Name:   test_pll.vhd
-- Project Name:  xc3s100c
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: digital_pll
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

ENTITY test_pll_vhd IS
END test_pll_vhd;

ARCHITECTURE behavior OF test_pll_vhd IS 

	-- Component Declaration for the Unit Under Test (UUT)
	COMPONENT digital_pll
	generic (
      vcxo_div_per         : integer         -- decimation of clk_i to pll, (594)
   );
	PORT(
		clk_vcxo_i : IN std_logic;
		clk_ref_decimated_i : IN std_logic;          
		clk_vcxo_dec_tick_o : OUT std_logic;
		pdm_o : OUT std_logic;
		clk_vcxo_decimated_o : OUT std_logic;
		lock_warning_o : OUT std_logic;
		lock_error_o : OUT std_logic
		);
	END COMPONENT;

	--Inputs
	SIGNAL clk_vcxo_i :  std_logic := '0';
	SIGNAL clk_ref_decimated_i :  std_logic := '0';

	--Outputs
	SIGNAL clk_vcxo_dec_tick_o :  std_logic;
	SIGNAL pdm_o :  std_logic;
	SIGNAL clk_vcxo_decimated_o :  std_logic;
	SIGNAL lock_warning_o :  std_logic;
	SIGNAL lock_error_o :  std_logic;

BEGIN

	-- Instantiate the Unit Under Test (UUT)
	uut: digital_pll 
	generic map(
		vcxo_div_per 	=>	594
	)
	PORT MAP(
		clk_vcxo_i => clk_vcxo_i,
		clk_ref_decimated_i => clk_ref_decimated_i,
		clk_vcxo_dec_tick_o => clk_vcxo_dec_tick_o,
		pdm_o => pdm_o,
		clk_vcxo_decimated_o => clk_vcxo_decimated_o,
		lock_warning_o => lock_warning_o,
		lock_error_o => lock_error_o
	);
		
	vcxo_gen: process
	begin
		--for i in 1 to 1000 loop
			wait for 3366 ps; --3367
			clk_vcxo_i <= NOT clk_vcxo_i;
			wait for 3366 ps;
			clk_vcxo_i <= NOT clk_vcxo_i;
		--end loop;
	end process;
	
	gps_ref_gen: process
	begin
		for i in 1 to 10 loop
			wait for 2 us;
			clk_ref_decimated_i <= NOT clk_ref_decimated_i;
			wait for 2 us;
			clk_ref_decimated_i <= NOT clk_ref_decimated_i;
		end loop;
		wait;
	end process;
	
	tb : PROCESS
	BEGIN

		-- Wait 100 ns for global reset to finish
		wait for 100 ns;
			
		wait; -- will wait forever
	END PROCESS;

END;
