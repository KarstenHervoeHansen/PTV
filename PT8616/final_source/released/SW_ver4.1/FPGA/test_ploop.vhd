--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   09:27:37 06/06/2011
-- Design Name:   
-- Module Name:   C:/PT8616/development_src/FPGA/test_ploop.vhd
-- Project Name:  FPGA
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: digital_phase_loop
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
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_SIGNED.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY test_ploop IS
END test_ploop;
 
ARCHITECTURE behavior OF test_ploop IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT digital_phase_loop
    PORT(
         clk_i : IN  std_logic;
         phase_diff_i : IN  std_logic_vector(9 downto 0);
         update_tick_i : IN  std_logic;
         P_gain : IN  std_logic_vector(17 downto 0);
         I_gain : IN  std_logic_vector(17 downto 0);
         dac_word_o : OUT  std_logic_vector(15 downto 0)
        );
    END COMPONENT;
	 
	 COMPONENT digital_phase_loop_wmults
    PORT(
         clk_i : IN  std_logic;
         phase_diff_i : IN  std_logic_vector(9 downto 0);
         update_tick_i : IN  std_logic;
         P_gain : IN  std_logic_vector(17 downto 0);
         I_gain : IN  std_logic_vector(17 downto 0);
         dac_word_o : OUT  std_logic_vector(15 downto 0)
        );
    END COMPONENT;
	 
	 
    

   --Inputs
   signal clk_i : std_logic := '0';
   signal phase_diff_i : std_logic_vector(9 downto 0) := conv_std_logic_vector(-90, 10);
   signal update_tick_i : std_logic := '0';
   signal P_gain : std_logic_vector(17 downto 0) := conv_std_logic_vector(32, 18);
   signal I_gain : std_logic_vector(17 downto 0) := conv_std_logic_vector(128, 18);

 	--Outputs
   signal dac_word_o : std_logic_vector(15 downto 0);

   -- Clock period definitions
   constant clk_i_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: digital_phase_loop_wmults PORT MAP (
--   uut: digital_phase_loop PORT MAP (
          clk_i => clk_i,
          phase_diff_i => phase_diff_i,
          update_tick_i => update_tick_i,
          P_gain => P_gain,
          I_gain => I_gain,
          dac_word_o => dac_word_o
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
		update_tick_i <= '1';
		
      wait for clk_i_period;
		update_tick_i <= '0';
		
      --wait;
   end process;

END;
