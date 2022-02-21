-- autorate.vhd
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
-- This module implements a state machine for auto rate detection.
--
-- this state machine waits for an external controller to set freq equal to next_freq
-- after 200 ms, the status of lfi is checked and if a line fault is indicated, nex_freq
-- is updated with the next frequecy number to try
--
-- frequency numbers for freq and next_freq represent the following data rates:
--    00 = 270 Mbit/s
--    01 = 360 Mbit/s
--    10 = 540 Mbit/s
--    11 = 1,485 Mbit/s
--

LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;

ENTITY autorate IS
	PORT(
		clkout, 				-- 24 mhz clock from microcontroller
		reset, 					-- reset input
		lfi, cd		: IN	STD_LOGIC;	-- lfi from HOTLink II, cd from cable receiver
		freq					: IN	STD_LOGIC_VECTOR(1 DOWNTO 0);  -- current frequency setting from FPGA register
		next_freq				: BUFFER	STD_LOGIC_VECTOR(1 DOWNTO 0) -- next frequency to try
		);
END autorate;

ARCHITECTURE a OF autorate IS
	SIGNAL timer200ms : INTEGER RANGE 0 to 2399999;
	TYPE STATE_TYPE IS (idle, s270, s360, s540, s1485, s270p, s360p, s540p, s270l,s360l,s540l);
	SIGNAL state   : STATE_TYPE;
	signal cd0,lfi0 : std_logic;
	signal lficount : std_logic_vector(7 downto 0);
BEGIN
	PROCESS
	BEGIN
		WAIT UNTIL clkout = '1';
		
		cd0 <= cd;
		lfi0 <= lfi;
		if reset = '1' or timer200ms = 0  or cd0 = '0' or timer200ms > 2399999 then
			timer200ms <= 2399999;
			lficount <= "00000000";
		elsif freq = next_freq then	-- start timer when PSoC sets freq to match next_freq
			timer200ms <= timer200ms-1;
			if lfi /= lfi0 then lficount <= lficount+1; end if;
		end if;
		
		if cd0 = '0' or reset = '1' then 
			state <= idle;
			next_freq <= "00";
		elsif timer200ms = 0  then
			case state is
				when idle =>
					next_freq <= "00";
					if freq = "00" then
						state <= s270;
					end if;
				when s270 =>
					if lfi0 = '1' or lficount > 20 then
						state <= s360;
						next_freq <= "01";
					else
						state <= s270p;
						next_freq <= "00";
					end if;
				when s360 =>
					if lfi0 = '1' or lficount > 20  then 
						state <= s540;
						next_freq <= "10";
					else
						state <= s270p;
						next_freq <= "00";
					end if;
				when s540 =>
					if lfi0 = '1' or lficount > 20  then 
						state <= s1485;
						next_freq <= "11";
					else
						state <= s270p;
						next_freq <= "00";
					end if;
				when s1485 =>
					if lfi0 = '1' or lficount > 20 then
						state <= s270;
						next_freq <= "00";
					end if;
				when s270p =>
					if lfi0 = '1' or lficount > 20  then
						state <= s360p;
						next_freq <= "01";
					elsif lfi0 = '0' then
						state <= s270l;
					end if;
				when s360p =>
					if lfi0 = '1' or lficount > 20  then
						state <= s540p;
						next_freq <= "10";
					elsif lfi0 = '0' then
						state <= s360l;
					end if;
				when s540p =>
					if lfi0 = '1' or lficount > 20  then
						state <= s1485;
						next_freq <= "11";
					elsif lfi0 = '0' then
						state <= s540l;
					end if;
				when s270l =>
					if lfi0 = '1' or lficount > 20 then
						state <= s270;
						next_freq <= "00";
					end if;
				when s360l =>
					if lfi0 = '1' or lficount > 20 then
						state <= s270;
						next_freq <= "00";
					end if;
				when s540l =>
					if lfi0 = '1' or lficount > 20 then
						state <= s270;
						next_freq <= "00";
					end if;
				when others =>
					state <= idle;
					next_freq <= "00";
			end case;
		end if;

	END PROCESS;

END a;
