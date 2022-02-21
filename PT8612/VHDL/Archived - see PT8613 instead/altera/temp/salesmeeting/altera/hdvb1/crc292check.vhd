-- crc292check.vhd
--
-- Written by:  George Cosens
--              Chief Engineer 
--              Linear Systems Ltd.
--
-- Copyright (c) 2004 Linear Systems Ltd.
-- All rights reserved.
-- Date:  September 29, 2004
--
--
-- This code is intended for use with the Cypress HOTLink II Video Demostration Board
-------------------------------------------------------------------------------------
--
-- This module checks for CRC errors in smpte 292 video data
-- 1080i at 30 frames/sec is supported
--
LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;
use work.pack_crc.all;
use work.pack_reverse_bit_order.all;

ENTITY crc292check IS
	PORT(
		clk, reset	: IN STD_LOGIC;
		data_in		: IN STD_LOGIC_VECTOR(0 to 9);
		readcrc		: IN 	STD_LOGIC;
		crc_errors   : BUFFER STD_LOGIC_VECTOR(31 DOWNTO 0)
	);
END crc292check;

ARCHITECTURE a OF crc292check IS

TYPE STATE_TYPE IS (s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12);
SIGNAL state: STATE_TYPE;
signal crc : std_logic_vector(17 downto 0);
signal crc_error : std_logic;
signal checkdata : std_logic_vector(8 downto 0);
signal crcdata : std_logic_vector(17 downto 0);
signal ldata_in : std_logic_vector(9 downto 0);
signal readcrc_old : std_logic;

BEGIN
	crcdata <= crc;
	PROCESS (clk,reset)
	BEGIN
		IF reset = '1' THEN
			state <= s0;
			crc <= conv_std_logic_vector(0,18);
			crc_error <= '0';
			crc_errors <= conv_std_logic_vector(0,32);
		ELSIF clk'EVENT AND clk = '1' THEN
			readcrc_old <= readcrc;
			if readcrc_old = '1' and readcrc = '0' then
				crc_errors <= conv_std_logic_vector(0,32);
			end if;
			checkdata <= ldata_in(8 downto 0);
			ldata_in(0) <= data_in(0);
			ldata_in(1) <= data_in(1);
			ldata_in(2) <= data_in(2);
			ldata_in(3) <= data_in(3);
			ldata_in(4) <= data_in(4);
			ldata_in(5) <= data_in(5);
			ldata_in(6) <= data_in(6);
			ldata_in(7) <= data_in(7);
			ldata_in(8) <= data_in(8);
			ldata_in(9) <= data_in(9);
			CASE state IS
				WHEN s0 =>
					if crc_error = '1' then
						crc_errors <= crc_errors+1;
					end if;
					crc <= conv_std_logic_vector(0,18);
					crc_error <= '0';
					IF ldata_in = "1111111111" THEN
						state <= s1;
					END IF;

				WHEN s1 =>
						state <= s2;

				WHEN s2 =>
						state <= s3;

				WHEN s3 => -- xyz
					IF ldata_in(6)= '0' THEN
						state <= s4;
					ELSE
						state <= s0;
					END IF;

				WHEN s4 =>
					crc <= crc_0_4_5_18(ldata_in,crc);
					IF ldata_in = "1111111111" THEN
						state <= s5;
					END IF;
				
				WHEN s5 => -- "000"
					crc <= crc_0_4_5_18(ldata_in,crc);
					state <= s6;
				
				WHEN s6 => --"000"
					crc <= crc_0_4_5_18(ldata_in,crc);
					state <= s7;
				
				WHEN s7 =>  --"xyz"
					crc <= crc_0_4_5_18(ldata_in,crc);
					state <= s8;
				
				WHEN s8 => --ln0
					crc <= crc_0_4_5_18(ldata_in,crc);
					state <= s9;
				
				WHEN s9 => --ln1
					crc <= crc_0_4_5_18(ldata_in,crc);
					state <= s10;
				
				WHEN s10 => -- crc0
--					crc <= crc_0_4_5_18(ldata_in,crc);
					state <= s11;
				
				WHEN s11 => -- crc1
					if crcdata(8 downto 0) /= checkdata then
						crc_error <= '1';
					end if;
					state <= s12;
				
				WHEN s12 =>
					if crcdata(17 downto 9) /= checkdata then
						crc_error <= '1';
					end if;
					state <= s0;
				
			END CASE;
		END IF;
	END PROCESS;

END a;