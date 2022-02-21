--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   11:16:20 05/24/2011
-- Design Name:   
-- Module Name:   C:/PT8616/development_src/FPGA/test_DAC.vhd
-- Project Name:  FPGA
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: ad5660_serial_DAC
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
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_SIGNED.ALL;

 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY test_DAC IS
END test_DAC;
 
ARCHITECTURE behavior OF test_DAC IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT ad5660_serial_DAC
    PORT(
         clk_i : IN  std_logic;
         decimation_i : IN  std_logic_vector(3 downto 0);
         word_i : IN  std_logic_vector(15 downto 0);
         update_i : IN  std_logic;
         sclk_o : OUT  std_logic;
         data_o : OUT  std_logic;
         sync_o : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal clk_i : std_logic := '0';
   signal decimation_i : std_logic_vector(3 downto 0) := "0010"; -- decimate by 6 (2*(2+1))
   signal word_i : std_logic_vector(15 downto 0) := "1001000000001001";
   signal update_i : std_logic := '0';

 	--Outputs
   signal sclk_o : std_logic;
   signal data_o : std_logic;
   signal sync_o : std_logic;

   -- Clock period definitions
   constant clk_i_period : time := 6.7 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: ad5660_serial_DAC PORT MAP (
          clk_i => clk_i,
          decimation_i => decimation_i,
          word_i => word_i,
          update_i => update_i,
          sclk_o => sclk_o,
          data_o => data_o,
          sync_o => sync_o
        );

   -- Clock process definitions
   clk_i_process :process
   begin
		clk_i <= '0';
		wait for clk_i_period/2;
		clk_i <= '1';
		wait for clk_i_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
      wait for 100 ns;	

		update_i <= '1';
      wait for clk_i_period;
		update_i <= '0';
		
      -- insert stimulus here 

      wait;
   end process;

END;
