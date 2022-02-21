LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY testbench IS
END testbench;

ARCHITECTURE behavior OF testbench IS 

	COMPONENT Tilstandsmaskine
	PORT(
		reset           : IN std_logic;
		clk             : IN std_logic;
		command         : IN integer range 0 to 2;
		cnt_value_spec  : IN integer range 0 to 63;          
		sync            : OUT std_logic;
		sync_t          : OUT std_logic;
		blank           : OUT std_logic
		);
	END COMPONENT;

	SIGNAL reset           :  std_logic;
	SIGNAL clk             :  std_logic;
	SIGNAL command         :  integer range 0 to 2;
	SIGNAL cnt_value_spec  :  integer range 0 to 63;   
	SIGNAL sync            :  std_logic;
	SIGNAL sync_t          :  std_logic;
	SIGNAL blank           :  std_logic;

   SIGNAL counter_load    :  std_logic;
	SIGNAL cnt_value       :  integer range 0 to 63;


BEGIN

	uut: Tilstandsmaskine PORT MAP(
		reset           => reset,
		clk             => clk,
		command         => command,
		cnt_value_spec  => cnt_value_spec,
		sync            => sync,
		sync_t          => sync_t,
		blank           => blank
	);


--  Test Bench Statements
   tb : PROCESS
   BEGIN
   
      reset <= '1';
      counter_load <= '1';
      cnt_value_spec <= 53;
         clk <= '0';
         wait for 3 ns;
         clk <= '1';
         wait for 3 ns;
         counter_load <= '0';

   reset <= '0';
   command <= 0;
      for i in 0 to 5 loop
          clk <= '0';
          wait for 3 ns;
          clk <= '1';
          wait for 3 ns;
      end loop;

      command <= 1;
      clk <= '0';
      wait for 3 ns;
      clk <= '1';
      wait for 3 ns;
      command <= 0;
      for i in 0 to 1125 loop
          clk <= '0';
          wait for 3 ns;
          clk <= '1';
          wait for 3 ns;
      end loop;

  
      command <= 2;
      clk <= '0';
      wait for 3 ns;
      clk <= '1';
      wait for 3 ns;
      command <= 0;
      for i in 0 to 1125 loop
          clk <= '0';
          wait for 3 ns;
          clk <= '1';
          wait for 3 ns;
      end loop;

      wait; -- will wait forever
   END PROCESS;
--  End Test Bench 

END;
