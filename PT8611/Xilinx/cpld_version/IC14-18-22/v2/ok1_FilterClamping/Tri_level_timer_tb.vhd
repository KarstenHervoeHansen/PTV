LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY testbench IS
END testbench;

ARCHITECTURE behavior OF testbench IS 

	COMPONENT Tri_level_timer
   Port ( reset : in std_logic;
          clk   : in std_logic;
            
--        cnt_value_spec : in integer range 0 to 127;   -- bruges af komponenten 'loadable_down_counter', bestemmer laengden af wait-states'ene i statemaskinen - afhaengigt af position i tri-level synk signalet

          sync           : out std_logic; -- styresignal til konverteren
          sync_t         : out std_logic; -- styresignal til konverteren
          blank          : out std_logic -- styresignal til konverteren
          );
	END COMPONENT;

	signal reset : std_logic;
	SIGNAL clk 		:  std_logic;
	
--	SIGNAL cnt_value_spec : integer range 0 to 127;
	
	SIGNAL sync   : std_logic; 
	SIGNAL sync_t : std_logic; 
	SIGNAL blank  : std_logic;

	SIGNAL clk_no : integer;

--	SIGNAL state_cond :  integer range 0 to 15;
--	SIGNAL command : integer range 0 to 2;

	SIGNAL tri_level : std_logic;

BEGIN

   make_tri_level : process(sync, sync_t)
   begin
       if sync = '1' then
           tri_level <= 'Z';
       elsif sync = '0' and sync_t = '0' then
           tri_level <= '0';
       elsif sync = '0' and sync_t = '1' then
           tri_level <= '1';
       else
           tri_level <= 'X';
       end if;
   end process;

	uut: Tri_level_timer PORT MAP(
		reset => reset,
      clk => clk,
--      cnt_value_spec => cnt_value_spec,
      sync => sync,
      sync_t =>  sync_t,
      blank => blank
    );

--  Test Bench Statements
   tb : PROCESS
   BEGIN

--   cnt_value_spec <= 85;

   reset <= '1';
       clk <= '0';
       wait for 3 ns;
       clk <= '1';
       wait for 3 ns;
       clk <= '0';
       wait for 3 ns;
       clk <= '1';
       wait for 3 ns;
   reset <= '0';
       clk <= '0';
       wait for 3 ns;
       clk <= '1';
       wait for 3 ns;

      for i in 0 to 3450000 loop
          clk_no <= i;

          clk <= '0';
            wait for 3 ns;
          clk <= '1';
            wait for 3 ns;
      end loop;
      wait; -- will wait forever
   END PROCESS;
--  End Test Bench 

END;
