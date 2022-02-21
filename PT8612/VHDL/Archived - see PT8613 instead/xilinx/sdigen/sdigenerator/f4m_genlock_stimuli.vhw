-- F:\PT8612\VHDL\XILINX\SDIGEN\SDIGENERATOR
-- VHDL Test Bench created by
-- HDL Bencher 6.1i
-- Thu May 26 07:32:35 2005
-- 
-- Notes:
-- 1) This testbench has been automatically generated from
--   your Test Bench Waveform
-- 2) To use this as a user modifiable testbench do the following:
--   - Save it as a file with a .vhd extension (i.e. File->Save As...)
--   - Add it to your project as a testbench source (i.e. Project->Add Source...)
-- 

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;
USE IEEE.STD_LOGIC_TEXTIO.ALL;
USE STD.TEXTIO.ALL;

ENTITY f4m_genlock_stimuli IS
END f4m_genlock_stimuli;

ARCHITECTURE testbench_arch OF f4m_genlock_stimuli IS
-- If you get a compiler error on the following line,
-- from the menu do Options->Configuration select VHDL 87
FILE RESULTS: TEXT OPEN WRITE_MODE IS "results.txt";
	COMPONENT f4m_genlock_regen
		PORT (
			mreset : In  std_logic;
			clk : In  std_logic;
			f4m : In  std_logic;
			f2m : Out  std_logic
		);
	END COMPONENT;

	SIGNAL mreset : std_logic;
	SIGNAL clk : std_logic;
	SIGNAL f4m : std_logic;
	SIGNAL f2m : std_logic;

BEGIN
	UUT : f4m_genlock_regen
	PORT MAP (
		mreset => mreset,
		clk => clk,
		f4m => f4m,
		f2m => f2m
	);

	PROCESS -- clock process for clk,
	BEGIN
		CLOCK_LOOP : LOOP
		clk <= transport '0';
		WAIT FOR 10 ps;
		clk <= transport '1';
		WAIT FOR 10 ps;
		WAIT FOR 490 ps;
		clk <= transport '0';
		WAIT FOR 490 ps;
		END LOOP CLOCK_LOOP;
	END PROCESS;

	PROCESS   -- Process for clk
		VARIABLE TX_OUT : LINE;
		VARIABLE TX_ERROR : INTEGER := 0;

		PROCEDURE CHECK_f2m(
			next_f2m : std_logic;
			TX_TIME : INTEGER
		) IS
			VARIABLE TX_STR : String(1 to 4096);
			VARIABLE TX_LOC : LINE;
		BEGIN
			-- If compiler error ("/=" is ambiguous) occurs in the next line of code
			-- change compiler settings to use explicit declarations only
			IF (f2m /= next_f2m) THEN 
				STD.TEXTIO.write(TX_LOC,string'("Error at time="));
				STD.TEXTIO.write(TX_LOC, TX_TIME);
				STD.TEXTIO.write(TX_LOC,string'("ps f2m="));
				IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, f2m);
				STD.TEXTIO.write(TX_LOC, string'(", Expected = "));
				IEEE.STD_LOGIC_TEXTIO.write(TX_LOC, next_f2m);
				STD.TEXTIO.write(TX_LOC, string'(" "));
				TX_STR(TX_LOC.all'range) := TX_LOC.all;
				STD.TEXTIO.writeline(results, TX_LOC);
				STD.TEXTIO.Deallocate(TX_LOC);
				ASSERT (FALSE) REPORT TX_STR SEVERITY ERROR;
				TX_ERROR := TX_ERROR + 1;
			END IF;
		END;

		BEGIN
		-- --------------------
		mreset <= transport '0';
		f4m <= transport '0';
		-- --------------------
		WAIT FOR 10000 ps; -- Time=10000 ps
		mreset <= transport '1';
		-- --------------------
		WAIT FOR 10000 ps; -- Time=20000 ps
		f4m <= transport '1';
		-- --------------------
		WAIT FOR 10000 ps; -- Time=30000 ps
		f4m <= transport '0';
		-- --------------------
		WAIT FOR 89000 ps; -- Time=119000 ps
		f4m <= transport '1';
		-- --------------------
		WAIT FOR 10000 ps; -- Time=129000 ps
		f4m <= transport '0';
		-- --------------------
		WAIT FOR 20000 ps; -- Time=149000 ps
		f4m <= transport '1';
		-- --------------------
		WAIT FOR 10000 ps; -- Time=159000 ps
		f4m <= transport '0';
		-- --------------------
		WAIT FOR 1010 ps; -- Time=160010 ps
		-- --------------------

		IF (TX_ERROR = 0) THEN 
			STD.TEXTIO.write(TX_OUT,string'("No errors or warnings"));
			STD.TEXTIO.writeline(results, TX_OUT);
			ASSERT (FALSE) REPORT
				"Simulation successful (not a failure).  No problems detected. "
				SEVERITY FAILURE;
		ELSE
			STD.TEXTIO.write(TX_OUT, TX_ERROR);
			STD.TEXTIO.write(TX_OUT, string'(
				" errors found in simulation"));
			STD.TEXTIO.writeline(results, TX_OUT);
			ASSERT (FALSE) REPORT
				"Errors found during simulation"
				SEVERITY FAILURE;
		END IF;
	END PROCESS;
END testbench_arch;

CONFIGURATION f4m_genlock_regen_cfg OF f4m_genlock_stimuli IS
	FOR testbench_arch
	END FOR;
END f4m_genlock_regen_cfg;
