-- spi.vhd
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
-- Serial Peripheral Interface
-- PSoC sends 3 bytes.  If bit 7 of first byte is '1' then it is a read operation
-- If bit 7 of first byte is a '1' the it is a write operation

LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;

ENTITY spi IS
	PORT(
		scs, scl, sdo		: IN	STD_LOGIC;	-- SPI inputs from PSoC
		sdi					: OUT	STD_LOGIC;	-- SPI data output to PSoC
		read, write 		: OUT	STD_LOGIC;	-- read and write signals to channel registers
		channel				: OUT	STD_LOGIC;	-- channel select output
		address				: BUFFER	STD_LOGIC_VECTOR(7 DOWNTO 0);	-- address output to channel registers
		data_out			: BUFFER	STD_LOGIC_VECTOR(7 DOWNTO 0);	-- data output to channel registers
		data_in				: IN	STD_LOGIC_VECTOR(7 DOWNTO 0)		-- data input from channel registers
	);
END spi;
ARCHITECTURE a OF spi IS
	SIGNAL bitcount : integer range 0 to 23;
	signal reg: std_logic_vector(23 downto 0);
BEGIN
	PROCESS (scl,scs)
	BEGIN
		IF scs = '0' THEN
			read <='0';
			write <= '0';
			bitcount <= 0;
		ELSIF scl'EVENT AND scl = '1' THEN
			bitcount <= bitcount+1;
			CASE bitcount IS
				WHEN 0 =>
					reg(23) <= sdo;
				WHEN 1 =>
					reg(22) <= sdo;
				WHEN 2 =>
					reg(21) <= sdo;
				WHEN 3 =>
					reg(20) <= sdo;
				WHEN 4 =>
					reg(19) <= sdo;
				WHEN 5 =>
					reg(18) <= sdo;
				WHEN 6 =>
					reg(17) <= sdo;
				WHEN 7 =>
					reg(16) <= sdo;
				WHEN 8 =>
					reg(15) <= sdo;
				WHEN 9 =>
					reg(14) <= sdo;
				WHEN 10 =>
					reg(13) <= sdo;
				WHEN 11 =>
					reg(12) <= sdo;
				WHEN 12 =>
					reg(11) <= sdo;
				WHEN 13 =>
					reg(10) <= sdo;
				WHEN 14 =>
					reg(9) <= sdo;
				WHEN 15 =>
					reg(8) <= sdo;
					address(7 downto 1)<= reg(15 downto 9);
					address(0) <= sdo;
					channel <= reg(16);
					if reg(23)='1' then	-- is read operation?
						read <= '1';
					end if;
				WHEN 16 =>
					read <= '0';
					reg(7) <= sdo;
				WHEN 17 =>
					reg(6) <= sdo;
				WHEN 18 =>
					reg(5) <= sdo;
				WHEN 19 =>
					reg(4) <= sdo;
				WHEN 20 =>
					reg(3) <= sdo;
				WHEN 21 =>
					reg(2) <= sdo;
				WHEN 22 =>
					reg(1) <= sdo;
				WHEN 23 =>
					reg(0) <= sdo;
					data_out(7 downto 1) <= reg(7 downto 1);
					data_out(0) <= sdo;
					if reg(23) = '0' then write <= '1';
					end if;
				WHEN OTHERS =>
					write <= '0';
					read <= '0';
			END CASE;
		END IF;
	END PROCESS;
	PROCESS(scl, scs)
	begin
		if scs = '0' then
			sdi <= 'Z';
		elsif scl'event and scl='0' then
			case bitcount is
				when 16 =>
					sdi <= data_in(7);
				when 17 =>
					sdi <= data_in(6);
				when 18 =>
					sdi <= data_in(5);
				when 19 =>
					sdi <= data_in(4);
				when 20 =>
					sdi <= data_in(3);
				when 21 =>
					sdi <= data_in(2);
				when 22 =>
					sdi <= data_in(1);
				when 23 =>
					sdi <= data_in(0);
				when others =>
			end case;
		end if;
	end process;
END a;
