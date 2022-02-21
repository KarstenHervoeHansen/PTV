
-- VHDL Instantiation Created from source file state_timer.vhd -- 14:59:09 08/18/2003
--
-- Notes: 
-- 1) This instantiation template has been automatically generated using types
-- std_logic and std_logic_vector for the ports of the instantiated module
-- 2) To use this template to instantiate this entity, cut-and-paste and then edit

	COMPONENT state_timer
	PORT(
		clk : IN std_logic;
		counter_load : IN std_logic;
		cnt_setup : IN std_logic_vector(0 to 1023);       
		cnt_value : INOUT std_logic_vector(0 to 1023)
		);
	END COMPONENT;

	Inst_state_timer: state_timer PORT MAP(
		clk => ,
		counter_load => ,
		cnt_setup => ,
		cnt_value => 
	);


