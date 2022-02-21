-- tpgen.vhd
--
-- Written by:  George Cosens
--              Chief Engineer 
--              Linear Systems Ltd.
--
-- Copyright (c) 2004 Linear Systems Ltd.
-- All rights reserved.
-- Date:  September 29, 2004
--
-- This code is intended for use with the Cypress HOTLink II Video Demostration Board
-------------------------------------------------------------------------------------
--
--	Top level file for SDI test pattern generator
--
--	txsrc selects one of four available test patterns
-- 		0 = eg1 colour bars
--		1 = grey pattern
--		2 = r178 pathological test
--		3 = rp178 alternate pathological test
--		others = other transmit sources not supported by this pattern generator
--
--	txrate indicates the transmit clock rate
-- 		0 = 27 mhz
--		1 = 36 mhz
--		2 = 54 mhz
--		3 = 74.25 mhz

LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;
LIBRARY lpm;
USE lpm.lpm_components.all;

ENTITY tpgen IS
	PORT(
		reset_n, clk, pal_nntsc		: IN	STD_LOGIC;			-- active low reset, transmit clock, pal/ntsc select
		txsrc			: IN	STD_LOGIC_VECTOR(2 DOWNTO 0);	-- transmit source select input
		txrate			: IN	STD_LOGIC_VECTOR(1 DOWNTO 0);	-- transmit rate input
		data_out		: OUT STD_LOGIC_VECTOR(9 DOWNTO 0)		-- video data out
	);
END tpgen;

ARCHITECTURE a OF tpgen IS

	COMPONENT eg1
		PORT ( 
			clk,enable	 : IN std_logic;
			sel : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
			data : OUT std_logic_vector(9 DOWNTO 0);
			linecount_out: BUFFER std_logic_vector(9 DOWNTO 0);
			wordcount_out : BUFFER std_logic_vector(11 DOWNTO 0)
		);
	END COMPONENT;
	
	COMPONENT grey
		PORT ( 
			clk,enable	 : IN std_logic;
			sel : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
			data : OUT STD_LOGIC_VECTOR(9 DOWNTO 0);
			linecount_out: BUFFER STD_LOGIC_VECTOR(9 DOWNTO 0);
			wordcount_out : BUFFER STD_LOGIC_VECTOR(11 DOWNTO 0)
		);
	END COMPONENT;

	COMPONENT rp178
		PORT ( 
			clk,enable,
			std_altn	 : IN std_logic;	-- '1' = standard distribution, '0' = alternate distribution
			sel : IN std_logic_vector(2 DOWNTO 0);
			data : OUT std_logic_vector(9 DOWNTO 0);
			linecount_out: BUFFER std_logic_vector(9 DOWNTO 0);
			wordcount_out : BUFFER std_logic_vector(11 DOWNTO 0)
		);
	END COMPONENT;
	
	COMPONENT edh
		PORT(
			clk, enable					: IN	STD_LOGIC;
			data_in						: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);
			linecount					: IN 	STD_LOGIC_VECTOR(9 DOWNTO 0);
			wordcount 					: IN 	STD_LOGIC_VECTOR(11 DOWNTO 0);
			video_type					: IN 	STD_LOGIC_VECTOR(2 DOWNTO 0);
			read_acrc_errors			: IN	STD_LOGIC;
			read_fcrc_errors			: IN	STD_LOGIC;
			acrc_errors, fcrc_errors	: BUFFER STD_LOGIC_VECTOR( 7 DOWNTO 0);
			data_out 					: BUFFER STD_LOGIC_VECTOR(9 DOWNTO 0)
		);
	END COMPONENT;

	SIGNAL enable_eg1, enable_grey, enable_rp178 : STD_LOGIC;
	SIGNAL data_eg1, data_grey, data_rp178, data, edata : STD_LOGIC_VECTOR(9 DOWNTO 0);
	SIGNAL sel : STD_LOGIC_VECTOR(2 DOWNTO 0);
	SIGNAL testcount, testdata: std_logic_vector(9 DOWNTO 0);
	SIGNAL linecount, eg1_linecount, rp178_linecount, grey_linecount : STD_LOGIC_VECTOR(9 DOWNTO 0);
	SIGNAL wordcount, eg1_wordcount, rp178_wordcount, grey_wordcount : STD_LOGIC_VECTOR(11 DOWNTO 0);
	
BEGIN
	PROCESS
	BEGIN
		WAIT UNTIL clk = '1';
		data_out <= data;

	END PROCESS;

	enable_eg1<=	'1' WHEN reset_n = '1' and txsrc = "000"  and txrate <= 2 ELSE '0';
	enable_grey <=	'1' WHEN reset_n = '1' and txsrc = "001"  and txrate <= 1 ELSE '0';
	enable_rp178 <=	'1' WHEN reset_n = '1' and txsrc(2 DOWNTO 1) = "01"  and txrate <= 1 ELSE '0';
	
	sel(0) <= txrate(0);
	sel(1) <= pal_nntsc;
	sel(2) <= '1' when txrate = 2 else '0';

	WITH txsrc(1 DOWNTO 0) SELECT
		edata	<=	data_eg1	WHEN	"00",
					data_grey	WHEN	"01",
					data_rp178	WHEN	"10",
					data_rp178	WHEN	"11",
					"0000000000" WHEN	OTHERS;
	
	WITH txsrc(1 DOWNTO 0) SELECT
		linecount <= 	eg1_linecount	WHEN "00",
						grey_linecount	WHEN "01",
						rp178_linecount	WHEN "10",
						rp178_linecount	WHEN "11",
						"0000000000"	WHEN OTHERS;
					
	WITH txsrc(1 DOWNTO 0) SELECT
		wordcount <= 	eg1_wordcount	WHEN "00",
						grey_wordcount	WHEN "01",
						rp178_wordcount	WHEN "10",
						rp178_wordcount	WHEN "11",
						"000000000000"	WHEN OTHERS;
					
	u1: eg1
		PORT MAP( 
			clk => clk, enable => enable_eg1,
			sel => sel,
			data => data_eg1,
			linecount_out => eg1_linecount,
			wordcount_out => eg1_wordcount
		);
	
	u2: grey
		PORT MAP( 
			clk => clk, enable =>enable_grey,
			sel => sel,
			data => data_grey,
			linecount_out => grey_linecount,
			wordcount_out => grey_wordcount
		);

	u3: rp178
		PORT MAP( 
			clk => clk, enable =>enable_rp178,
			std_altn => not txsrc(0),
			sel => sel,
			data => data_rp178,
			linecount_out => rp178_linecount,
			wordcount_out => rp178_wordcount
		);

	u4: edh
		PORT MAP(
			clk => clk, enable => reset_n,
			data_in	=> edata,	
			linecount => linecount,	
			wordcount => wordcount, 
			read_acrc_errors => '0',
			read_fcrc_errors =>'0',
			video_type => sel,
			data_out => data
		);
END a;
