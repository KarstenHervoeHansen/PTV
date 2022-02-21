-- channelregs.vhd
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
-- This module implemets the channel control and status registers
--
--	0 = cfgreg		configuration register (write/read)
--		freq <= cfgreg(1 downto 0);
--		dvb_nsmpte <= cfgreg(2);
--		insel <= cfgreg(3);
--		pal_nntsc <= cfgreg(4);
--		lpen <= cfgreg(5);
--		chreset <= cfgreg(7);
--
--	1 = txsrcreg	transmit source register (write/read)
--		txsrc <= txsrcreg(2 downto 0);
--
--	2 = statusreg 	status register (read only)
--			statusreg(0) <= lfi;
--			statusreg(1) <= cd;
--			statusreg(2) <= cd2;
--			statusreg(5 DOWNTO 3) <= video_type;
--			statusreg(7 downto 6) <= next_freq;
--
--	3 = century 	(bcd read only)
--
--	4 = year 		(bcd read only)
--
--	5 = month 		(bcd read only)
--
--	6 = day 		(bcd read only)
--
--	8,9,10,11 = Y_crc_errors	HD-SDI Y crc error count (read only)
--
--	12,13,14,15 = C_crc_errors	HD-SDI C crc error count (read only)
--
--  16 = acrc_errors (read only)
--
--  17 = fcrc_errors (read only)

LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;

ENTITY channelregs IS
	PORT(
		reset, write, read	: IN	STD_LOGIC;	-- reset from pin, read & write from spi module
		channel, chsel		: IN 	STD_LOGIC;	-- channel input from SPI, channel select setting fixed in instantiation
		address				: IN	STD_LOGIC_VECTOR(7 DOWNTO 0);	-- address from spi module
		data_in				: INOUT	STD_LOGIC_VECTOR(7 DOWNTO 0);	-- data input from spi module
		data_out			: OUT	STD_LOGIC_VECTOR(7 DOWNTO 0);	-- data output to spi module
		century, year, month, day : IN STD_LOGIC_VECTOR(7 DOWNTO 0);	-- release date from top level module
		video_type			:IN STD_LOGIC_VECTOR(2 DOWNTO 0);
		lfi, cd, cd2		: IN	STD_LOGIC;	-- status indecators from HOTLink II and cable receivers
		dvb_nsmpte, insel	: OUT 	STD_LOGIC;	-- dvb/smpte select output, insel output to HOTLink II
		lpen			: OUT 	STD_LOGIC;	-- lpen output to HOTLink II
		chreset			: OUT 	STD_LOGIC;	--  channel reset
		pal_nntsc		: OUT	STD_LOGIC;	-- pal/ntsc output to test pattern generators
		freq			: OUT	STD_LOGIC_VECTOR(1 DOWNTO 0);	-- frueqency select output
		next_freq		: IN	STD_LOGIC_VECTOR(1 DOWNTO 0);	-- next frequency select input (for auto rate detection)
		txsrc			: out 	STD_LOGIC_VECTOR(2 DOWNTO 0);	-- transmit source select output
		read_acrc_errors			: OUT	STD_LOGIC;		-- read output to edh acrc error counter
		read_fcrc_errors			: OUT	STD_LOGIC;		-- read output to edh fcrc error counter
		acrc_errors, fcrc_errors	: IN STD_LOGIC_VECTOR( 7 DOWNTO 0);  -- crc error counts from edh module
		readYcrc, readCcrc		: BUFFER 	STD_LOGIC;				-- read outputws to HD-SDI CRC error counters
		Y_crc_errors, C_crc_errors : in std_logic_vector(31 downto 0)	-- input values from HD-SDI crc error counters
	);
END channelregs;

ARCHITECTURE a OF channelregs IS
	SIGNAL cfgreg, txsrcreg, statusreg : STD_LOGIC_VECTOR(7 DOWNTO 0);
	signal Ycrcreg, Ccrcreg : std_logic_vector(31 downto 0);
BEGIN
	process (reset, write)
	begin
		if reset = '1' then
			cfgreg <= "00001000";
			txsrcreg <= "00000000";
		elsif write'event and write = '0' then
			if channel = chsel then
				CASE address IS
					WHEN CONV_STD_LOGIC_VECTOR(0,8) =>
						cfgreg <= data_in;
					WHEN CONV_STD_LOGIC_VECTOR(1,8) =>
						txsrcreg(2 downto 0) <= data_in(2 downto 0);
					WHEN OTHERS =>
				END CASE;
			end if;
		END IF;
	end process;

	PROCESS
	BEGIN
		WAIT UNTIL read = '1';
		if channel = chsel then
			statusreg(0) <= lfi;
			statusreg(1) <= cd;
			statusreg(2) <= cd2;
			statusreg(5 DOWNTO 3) <= video_type;
			statusreg(7 downto 6) <= next_freq;
		end if;

	END PROCESS;

	freq <= cfgreg(1 downto 0);
	dvb_nsmpte <= cfgreg(2);
	insel <= cfgreg(3);
	pal_nntsc <= cfgreg(4);
	lpen <= cfgreg(5);
	chreset <= cfgreg(7) or reset;
	txsrc <= txsrcreg(2 downto 0);
	readYcrc <= '1' when read = '1' and address = 8 else '0';
	readCcrc <= '1' when read = '1' and address = 12 else '0';
	read_acrc_errors <= '1' when read = '1' and address = 16 else '0';
	read_fcrc_errors <= '1' when read = '1' and address = 17 else '0';

process
begin
	wait until readYcrc = '1';
	Ycrcreg <= Y_crc_errors;
end process;

PROCESS
begin
	wait until readYcrc = '1';
	Ccrcreg <= C_crc_errors;
end process;

outputmux:
WITH address SELECT
	data_out	<=	cfgreg		WHEN CONV_STD_LOGIC_VECTOR(0,8),
					txsrcreg 	WHEN CONV_STD_LOGIC_VECTOR(1,8),
					statusreg	WHEN CONV_STD_LOGIC_VECTOR(2,8),
					century 	when CONV_STD_LOGIC_VECTOR(3,8),
					year		when CONV_STD_LOGIC_VECTOR(4,8),
					month		when CONV_STD_LOGIC_VECTOR(5,8),
					day			when CONV_STD_LOGIC_VECTOR(6,8),
					Ycrcreg(7 downto 0) when CONV_STD_LOGIC_VECTOR(8,8),
					Ycrcreg(15 downto 8) when CONV_STD_LOGIC_VECTOR(9,8),
					Ycrcreg(23 downto 16) when CONV_STD_LOGIC_VECTOR(10,8),
					Ycrcreg(31 downto 24) when CONV_STD_LOGIC_VECTOR(11,8),
					Ccrcreg(7 downto 0) when CONV_STD_LOGIC_VECTOR(12,8),
					Ccrcreg(15 downto 8) when CONV_STD_LOGIC_VECTOR(13,8),
					Ccrcreg(23 downto 16) when CONV_STD_LOGIC_VECTOR(14,8),
					Ccrcreg(31 downto 24) when CONV_STD_LOGIC_VECTOR(15,8),
					acrc_errors when CONV_STD_LOGIC_VECTOR(16,8),
					fcrc_errors when CONV_STD_LOGIC_VECTOR(17,8),
					CONV_STD_LOGIC_VECTOR(0,8) when	others;
	
END a;
