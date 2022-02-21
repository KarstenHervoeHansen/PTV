--------------------------------------------------------------
-- global reference sync signals for 30Hz, 60Hz, 24Hz
-- og 23.98 Hz freerunning formats :
--------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
library UNISIM;
use UNISIM.VComponents.all;


entity freerunning_reference is
	Port(
		mreset_i		: in std_logic;
		f1485_i		: in std_logic;
		f1484_i		: in std_logic;
		ref2398_o	: out std_logic;
		ref24_o		: out std_logic;
		ref30_o		: out std_logic;
	);
end freerunning_reference;

architecture Behavioral of freerunning_reference is

signal ref2398_cnt		: std_logic_vector(22 downto 0);
signal ref24_cnt			: std_logic_vector(22 downto 0);
signal ref30_cnt 			: std_logic_vector(22 downto 0);

begin


-------------------------------------------------------------
-- Generate freerunning 24 Hz sync
-------------------------------------------------------------
make_24Hz_global_reference : process(mreset_i, f1485_i)
begin
	if mreset_i = '0' then
	   ref24_count <= conv_std_logic_vector(6187500-1,23);
		ref24_o <= '0';
	elsif f1485_i'event and f1585 = '1' then
		if ref24_count = conv_std_logic_vector(0,23) then
		   ref24_count <= conv_std_logic_vector(6187500-1,23);
		   ref24_o <= '1';
		else
         ref24_o <= '0';
		end if;
	end if;
end process;


-------------------------------------------------------------
-- Generate freerunning 23.98 Hz sync
-------------------------------------------------------------
make_2398Hz_global_reference : process(mreset_i, f1484_i)
begin
	if mreset_i = '0' then
	   ref2398_count <= conv_std_logic_vector(6187500-1,23);
		ref2398_o <= '0';
	elsif f1484_i'event and f1584 = '1' then
		if ref2398_count = conv_std_logic_vector(0,23) then
		   ref2398_count <= conv_std_logic_vector(6187500-1,23);
		   ref2398_o <= '1';
		else
         ref2398_o <= '0';
		end if;
	end if;
end process;


-------------------------------------------------------------
-- Generate freerunning 30 Hz sync
-------------------------------------------------------------
make_30Hz_global_reference : process(mreset_i, f1485_i)
begin
	if mreset_i = '0' then
	   ref30_count <= conv_std_logic_vector(4950000-1,23);
		ref30_o <= '0';
	elsif f1485_i'event and f1585 = '1' then
		if ref30_count = conv_std_logic_vector(0,23) then
		   ref30_count <= conv_std_logic_vector(6187500-1,23);
		   ref30_o <= '1';
		else
         ref30_o <= '0';
		end if;
	end if;
end process;

end Behavioral;