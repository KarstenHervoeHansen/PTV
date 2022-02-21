library IEEE;
use IEEE.std_logic_1164.all;

package pack_crc is

  function crc_0_5_12_16
    ( data:  std_logic_vector(9 downto 0);
      crc:   std_logic_vector(15 downto 0) )
    return std_logic_vector;

  function crc_0_4_5_18
    ( Data:  std_logic_vector(9 downto 0);
      CRC:   std_logic_vector(17 downto 0) )
    return std_logic_vector;

end pack_crc;

library IEEE;
use IEEE.std_logic_1164.all;

package body pack_crc is


  function crc_0_5_12_16  
    ( data:  std_logic_vector(9 downto 0);
      crc:   std_logic_vector(15 downto 0) )
    return std_logic_vector is

    variable new_crc: std_logic_vector(15 downto 0);
	variable c: std_logic_vector(15 downto 0);
	variable d: std_logic_vector(9 downto 0);

  begin

	d := data;
	c := crc;
	
	new_crc(15) := c(9) xor c(5) xor c(1) xor d(1) xor d(5) xor d(9);
	new_crc(14) := c(8) xor c(4) xor c(0) xor d(0) xor d(4) xor d(8);
	new_crc(13) := c(7) xor c(3) xor d(3) xor d(7);
	new_crc(12) := c(6) xor c(2) xor d(2) xor d(6);
	new_crc(11) := c(5) xor c(1) xor d(1) xor d(5);
	new_crc(10) := c(4) xor c(0) xor d(0) xor d(4) xor c(9) xor c(5) xor c(1) xor d(1) xor d(5) xor d(9);
	new_crc(9)  := c(3) xor d(3) xor c(8) xor c(4) xor c(0) xor d(0) xor d(4) xor d(8);
	new_crc(8)  := c(2) xor d(2) xor c(7) xor c(3) xor d(3) xor d(7);
	new_crc(7)  := c(1) xor d(1) xor c(6) xor c(2) xor d(2) xor d(6);
	new_crc(6)  := c(0) xor d(0) xor c(5) xor c(1) xor d(1) xor d(5);
	new_crc(5)  := c(15) xor c(4) xor c(0) xor d(0) xor d(4);
	new_crc(4)  := c(14) xor c(3) xor d(3);
	new_crc(3)  := c(13) xor c(2) xor d(2) xor c(9) xor c(5) xor c(1) xor d(1) xor d(5) xor d(9);
	new_crc(2)  := c(12) xor c(1) xor d(1) xor c(8) xor c(4) xor c(0) xor d(0) xor d(4) xor d(8);
	new_crc(1)  := c(11) xor c(0) xor d(0) xor c(7) xor c(3) xor d(3) xor d(7);
	new_crc(0)  := c(10) xor c(6) xor c(2) xor d(2) xor d(6);

    return new_crc;

  end crc_0_5_12_16;

  function crc_0_4_5_18  
    ( data:  std_logic_vector(9 downto 0);
      crc:   std_logic_vector(17 downto 0) )
    return std_logic_vector is

    variable new_crc: std_logic_vector(17 downto 0);
	variable c: std_logic_vector(17 downto 0);
	variable d: std_logic_vector(9 downto 0);

  begin

	d := data;
	c := crc;

	new_crc(17) := c(9)  xor d(9);
	new_crc(16) := c(8)  xor d(8);
	new_crc(15) := c(7)  xor d(7);
	new_crc(14) := c(6)  xor d(6);
	new_crc(13) := c(5)  xor d(5) xor c(9) xor d(9);
	new_crc(12) := c(4)  xor d(4) xor c(8) xor d(8) xor c(9) xor d(9);
	new_crc(11) := c(3)  xor d(3) xor c(7) xor d(7) xor c(8) xor d(8);
	new_crc(10) := c(2)  xor d(2) xor c(6) xor d(6) xor c(7) xor d(7);
	new_crc(9)  := c(1)  xor d(1) xor c(5) xor d(5) xor c(6) xor d(6);
	new_crc(8)  := c(0)  xor d(0) xor c(4) xor d(4) xor c(5) xor d(5);
	new_crc(7)  := c(17) xor c(3) xor d(3) xor c(4) xor d(4);
	new_crc(6)  := c(16) xor c(2) xor d(2) xor c(3) xor d(3);
	new_crc(5)  := c(15) xor c(1) xor d(1) xor c(2) xor d(2);
	new_crc(4)  := c(14) xor c(0) xor d(0) xor c(1) xor d(1);
	new_crc(3)  := c(13) xor c(0) xor d(0);
	new_crc(2)  := c(12);
	new_crc(1)  := c(11);
	new_crc(0)  := c(10);

    return new_crc;

  end crc_0_4_5_18;

end pack_crc;

