--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   13:56:09 05/24/2011
-- Design Name:   
-- Module Name:   C:/PT8616/development_src/FPGA/test_AD_serial_out.vhd
-- Project Name:  FPGA
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: ad5660_serial_out
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
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY test_AD_serial_out IS
END test_AD_serial_out;
 
ARCHITECTURE behavior OF test_AD_serial_out IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT ad5660_serial_out
    PORT(
         clk_148_i : IN  std_logic;
         data_i : IN  std_logic_vector(15 downto 0);
         new_data_i : IN  std_logic;
         sclk_o : OUT  std_logic;
         dout_o : OUT  std_logic;
         zsync_o : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal clk_148_i : std_logic := '0';
   signal data_i : std_logic_vector(15 downto 0) := "1001000000001001";
   signal new_data_i : std_logic := '0';

 	--Outputs
   signal sclk_o : std_logic;
   signal dout_o : std_logic;
   signal zsync_o : std_logic;

   -- Clock period definitions
   constant clk_148_i_period : time := 6.73 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: ad5660_serial_out PORT MAP (
          clk_148_i => clk_148_i,
          data_i => data_i,
          new_data_i => new_data_i,
          sclk_o => sclk_o,
          dout_o => dout_o,
          zsync_o => zsync_o
        );

   -- Clock process definitions
   clk_148_i_process :process
   begin
		clk_148_i <= '0';
		wait for clk_148_i_period/2;
		clk_148_i <= '1';
		wait for clk_148_i_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
      wait for 100 ns;	

		new_data_i <= '1';
      wait for clk_148_i_period;
		new_data_i <= '0';
      -- insert stimulus here 

      wait;
   end process;

END;
