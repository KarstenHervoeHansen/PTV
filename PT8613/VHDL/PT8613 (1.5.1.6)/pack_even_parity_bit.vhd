--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:    12:37:14 12/05/05
-- Design Name:    
-- Module Name:    even_parity_bit - Behavioral
-- Project Name:   
-- Target Device:  
-- Tool versions:  
-- Description:
--
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
--------------------------------------------------------------------------------
library ieee;
use ieee.STD_LOGIC_1164.all;

package pack_even_parity_bit is
	function even_parity_bits( vector:std_logic_vector; width:integer;offset: integer) return std_logic_vector;
end pack_even_parity_bit;
package body pack_even_parity_bit is
	function even_parity_bits( vector:std_logic_vector; width:integer;offset:integer) return std_logic_vector is
		variable result : std_logic_vector(1 downto 0) := "00";
		variable bits : integer := width;
		variable offsets: integer := offset;
	begin
		for i in 0 to bits-1 loop
			result(0) := result(0) xor vector(i);
		end loop;
		result(1) := not result(0);
		return (result);
	end even_parity_bits;
end pack_even_parity_bit;