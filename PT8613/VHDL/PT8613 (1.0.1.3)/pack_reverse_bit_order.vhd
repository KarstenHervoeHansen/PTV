library ieee;
use ieee.STD_LOGIC_1164.all;

package pack_reverse_bit_order is
	function reverse_bit_order( val:std_logic_vector; width:integer) return std_logic_vector;
	function rbo(val: std_logic_vector; lsb, msb: integer)return std_logic_vector;
end pack_reverse_bit_order;
package body pack_reverse_bit_order is
	function reverse_bit_order( val:std_logic_vector; width:integer) return std_logic_vector is
		variable result : std_logic_vector(width-1 downto 0) := (others=>'0');
		variable bits : integer := width;
		
	begin
		for i in 0 to bits-1 loop
			result(i) := val(width-i-1);
		end loop;
		return (result);
	end reverse_bit_order;
	
	function rbo(val: std_logic_vector; lsb, msb: integer)return std_logic_vector is
		variable result : std_logic_vector(msb downto lsb) := (others => '0');
		variable bits : integer := (msb-lsb)+1;
	begin
		for i in lsb to msb loop
			result(i) := val(msb-(i-lsb));
		end loop;
		return (result);
	end rbo;
end pack_reverse_bit_order;