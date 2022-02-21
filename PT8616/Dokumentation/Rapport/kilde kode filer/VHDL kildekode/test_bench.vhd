
--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   14:58:34 10/17/2006
-- Design Name:   i2c_main
-- Module Name:   test_bench.vhd
-- Project Name:  i2c_test
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: i2c_main
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

ENTITY test_bench_vhd IS
END test_bench_vhd;

ARCHITECTURE behavior OF test_bench_vhd IS 

	-- Component Declaration for the Unit Under Test (UUT)
	COMPONENT i2c_main
	PORT(
		scl : IN std_logic;
		rst : IN std_logic;    
		sda : INOUT std_logic      
		);
	END COMPONENT;

	--Inputs
	SIGNAL scl :  std_logic := '1';
	SIGNAL rst :  std_logic := '1';

	--BiDirs
	SIGNAL sda :  std_logic:='H';
BEGIN

	-- Instantiate the Unit Under Test (UUT)
	uut: i2c_main PORT MAP(
		sda => sda,
		scl => scl,
		rst => rst
	);

	tb : PROCESS
	BEGIN

		-- Wait 100 ns for global reset to finish
		wait for 10 us;
		rst<='0';
		
		wait for 10 us;
		--i2c start
		sda<='0';
		
-----------------------------------------Command
		--i2c write byte
		wait for 20 us;
		scl<='0';
		
		--bit0
		wait for 20 us;
		sda<='H';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit1
		wait for 20 us;
		sda<='H';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit2
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit3
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit4
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit5
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit6
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit7					++READ/WRITE
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
	
		--acknowledge
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';

-----------------------------------------Byte 1
		
		--bit0
		wait for 20 us;
		sda<='H';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit1
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit2
		wait for 20 us;
		sda<='H';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit3
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit4
		wait for 20 us;
		sda<='H';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit5
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit6
		wait for 20 us;
		sda<='H';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit7
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--acknowledge
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--i2c start
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		
		-----------------------------------------Byte 2
		wait for 80 us;
		--i2c start
		sda<='0';
		
		--i2c write byte
		wait for 20 us;
		scl<='0';
		
		--bit0
		wait for 20 us;
		sda<='H';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit1
		wait for 20 us;
		sda<='H';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit2
		wait for 20 us;
		sda<='H';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit3
		wait for 20 us;
		sda<='H';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit4
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit5
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit6
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--bit7					++READ/WRITE
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--acknowledge
		wait for 20 us;
		sda<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		--i2c stop
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		sda<='1';
		wait for 20 us;
		scl<='0';
		
		--------------------------------------Ekstra clocks
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		wait for 20 us;
		scl<='1';
		wait for 20 us;
		scl<='0';
		
		-- Place stimulus here
		wait; -- will wait forever
	END PROCESS;

END;
