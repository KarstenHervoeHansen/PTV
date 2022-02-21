
-- VHDL Instantiation Created from source file ad5660_serial_DAC.vhd -- 11:32:36 05/24/2011
--
-- Notes: 
-- 1) This instantiation template has been automatically generated using types
-- std_logic and std_logic_vector for the ports of the instantiated module
-- 2) To use this template to instantiate this entity, cut-and-paste and then edit

	COMPONENT ad5660_serial_DAC
	PORT(
		clk_i : IN std_logic;
		decimation_i : IN std_logic_vector(3 downto 0);
		word_i : IN std_logic_vector(15 downto 0);
		update_i : IN std_logic;          
		sclk_o : OUT std_logic;
		data_o : OUT std_logic;
		sync_o : OUT std_logic
		);
	END COMPONENT;

	Inst_ad5660_serial_DAC: ad5660_serial_DAC PORT MAP(
		clk_i => ,
		decimation_i => ,
		word_i => ,
		update_i => ,
		sclk_o => ,
		data_o => ,
		sync_o => 
	);


