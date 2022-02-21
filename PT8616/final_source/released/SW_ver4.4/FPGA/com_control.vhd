----------------------------------------------------------------------------------
-- Company: DK-technologies
-- Engineer: Jens K Hansen
-- 
-- Create Date:    12:23:46 08/13/2007 
-- Design Name: GPS-genlock module
-- Module Name:    com_control - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: Styring af indlæsning fra MCU. Indlæser 1x adressebyte
--	og derefter vilkårligt antal bytes. Beregner derefter checksum.
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity com_control is
	port (
		clk_i : in std_logic;
		rst_i : in std_logic;
		I2C_start_i	: in std_logic;
		I2C_update_i	: in std_logic;
		I2C_stop_i	: in std_logic;
		I2C_byte_i : in std_logic_vector (7 downto 0);
		address_o : out std_logic_vector (7 downto 0);
		update_register_o : out std_logic
	);
end com_control;

architecture Behavioral of com_control is

--state signaler
type state_type is (idle, read_adr, RW_byte, update);
signal state	: state_type;

--delaylines
signal start_line : std_logic_vector(1 downto 0);
signal update_line : std_logic_vector(1 downto 0);
signal stop_line : std_logic_vector(1 downto 0);

--styrings signaler
signal address : std_logic_vector(7 downto 0);

begin
	edge_detector : process (clk_i)
	begin
		if clk_i'event and clk_i = '1' then
			if rst_i = '1' then
				update_line <= "00";
				stop_line <= "00";
			else
				start_line <= start_line(0) & I2C_start_i;
				update_line <= update_line(0) & I2C_update_i;
				stop_line <= stop_line(0) & I2C_stop_i;
			end if;
		end if;
	end process;

	state_machine : process (clk_i)
	begin
		if clk_i'event and clk_i = '1' then
			-- hvis reset, så sæt i idle
			if rst_i = '1' then
				state <= idle;
			elsif state = idle and update_line = "01" then
				state <= read_adr;
			--hvis indlæst adresse, gå til read/write bye
			elsif state = read_adr then
				state <= RW_byte;
			--hvis "update" når i byte læs/skriv mode, så opdater
			elsif state = RW_byte and update_line = "01" then
				state <= update;
			--hvis opdateret, gå til næste byte
			elsif state = update then
				state <= RW_byte;
			--hvis "stop", gå i idle mode
			elsif stop_line = "01" then
				state <= idle;
			end if;
		end if;
	end process;

	control : process (clk_i, rst_i, state)
	begin
		if clk_i'event and clk_i = '1' then
			if state = idle then
				update_register_o <= '0';
				address <= address;
				address_o <= address;
				
			elsif state = read_adr then
				update_register_o <= '0';
				address <= I2C_byte_i;
				address_o <= I2C_byte_i;
				
			elsif state = RW_byte then
				update_register_o <= '0';
				address <= address;
				address_o <= address;
				
			elsif state = update then
				update_register_o <= '1';
				address <= address+1;
				address_o <= address;
			end if;
		end if;
	end process;
end Behavioral;

