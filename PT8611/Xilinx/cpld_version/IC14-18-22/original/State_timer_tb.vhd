LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY testbench IS
END testbench;

ARCHITECTURE testbench_arch OF testbench IS

-- Component Declaration
	Component state_timer is
Port (     clk            : in std_logic;
           counter_load   : in std_logic; 
           cnt_value_spec : in integer range 0 to 63;
           cnt_value      : inout integer range 0 to 63
         );

	END COMPONENT;

	SIGNAL clk             :  std_logic;
   SIGNAL counter_load    :  std_logic;
	SIGNAL cnt_value       :  integer range 0 to 63;
	SIGNAL cnt_value_spec  :  integer range 0 to 63;

BEGIN

-- Component Instantiation
	uut: state_timer PORT MAP(
		clk 		      => clk,
		cnt_value      => cnt_value,
      cnt_value_spec => cnt_value_spec,
      counter_load   => counter_load);

--  Test Bench Statements
   tb : PROCESS
   BEGIN
      cnt_value_spec <= 53;
      counter_load <= '1';
         clk <= '0';
         wait for 3 ns;
         clk <= '1';
         wait for 3 ns;
         counter_load <= '0';

      for i in 0 to 52 loop
          clk <= '0';
          wait for 3 ns;
          clk <= '1';
          wait for 3 ns;
      end loop;
          
      cnt_value_spec <= 53;
      counter_load <= '1';
         clk <= '0';
         wait for 3 ns;
         clk <= '1';
         wait for 3 ns;
         counter_load <= '0';
  
      for i in 0 to 52 loop
          clk <= '0';
          wait for 3 ns;
          clk <= '1';
          wait for 3 ns;
      end loop;


      wait; -- will wait forever
   END PROCESS;
--  End Test Bench 

END;
