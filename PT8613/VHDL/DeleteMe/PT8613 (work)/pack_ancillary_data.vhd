library ieee;
use ieee.STD_LOGIC_1164.all;

package pack_ancillary_data is
	function byte_to_10_bits( val:std_logic_vector) return std_logic_vector;
end pack_ancillary_data;

package body pack_ancillary_data is
	function byte_to_10_bits( val:std_logic_vector) return std_logic_vector is
		variable result : std_logic_vector(9 downto 0) := "0000000000";
	begin
		for i in 0 to 7 loop
			result(8) := result(8) xor val(i);
		end loop;
		result(9) := not result(8);
		result(7 downto 0) := val;
		return (result);
	end byte_to_10_bits;
end pack_ancillary_data;