-- edh.vhd
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
-- edh packet processing
-- supported video types are: 
--    0 = NTSC 525 line 13.5 MHz, 
--    1 = NTSC 525 line 18 MHz, 
--    2 = PAL 625 line 13.5 MHz,
--    3 = PAL 625 Line 18MHz, 
--    4 = NTSC 525 line 4:4:4:4, 
--    6 = PAL 525 line 4:4:4:4,
--    7 = unknown or not yet determined
--
-- wordcount, video_type, and linecount are used to locate EDH packets in SDI video streams on data_in.
-- This module generates EDH packets for an incoming SDI data stream and inserts the EDH packet into the 
-- outgoing data sream.  If EDH packets are present in the incoming data stream, the active fieled and 
-- full field CRC values are checked against the newly calculated CRC's and the errors are counted.
-- read_arcr_errors and read_fcrcerrors reset the asociated error counters.
--
--
LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;
use work.pack_reverse_bit_order.all;
use work.pack_crc.all;

ENTITY edh IS
	PORT(
		clk, enable					: IN	STD_LOGIC;
		data_in						: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);
		linecount					: IN 	STD_LOGIC_VECTOR(9 DOWNTO 0);
		wordcount 					: IN 	STD_LOGIC_VECTOR(11 DOWNTO 0);
		video_type					: IN 	STD_LOGIC_VECTOR(2 DOWNTO 0);
		read_acrc_errors			: IN	STD_LOGIC;
		read_fcrc_errors			: IN	STD_LOGIC;
		acrc_errors, fcrc_errors	: BUFFER STD_LOGIC_VECTOR( 7 DOWNTO 0);
		data_out 					: BUFFER STD_LOGIC_VECTOR(9 DOWNTO 0)
	);
END edh;

ARCHITECTURE a OF edh IS
	SIGNAL fdata_in, rfdata_in, edhword, expression, checksum : STD_LOGIC_VECTOR(9 DOWNTO 0);
	SIGNAL adef, apef, ffef, acrc_5_0, acrc_11_6, acrc_15_12, fcrc_5_0, fcrc_11_6, fcrc_15_12 : STD_LOGIC_VECTOR(9 DOWNTO 0);
	SIGNAL f_crc, a_crc : STD_LOGIC_VECTOR(15 DOWNTO 0);
	SIGNAL rx_fcrc, rx_acrc : STD_LOGIC_VECTOR(15 DOWNTO 0);
	signal reset_crc : std_logic;
	signal edhcount : std_logic_vector(11 downto 0);
	signal active_crc_range, full_crc_range, edh_detected, inc_acrc_errors, inc_fcrc_errors : std_logic;
	signal read_acrc_errors0, read_fcrc_errors0 : std_logic;
	
BEGIN
	crc_calculate:
	PROCESS (clk, enable)
	BEGIN
		IF enable = '0' THEN
			f_crc <= conv_std_logic_vector(0,16);
			a_crc <= conv_std_logic_vector(0,16);
		ELSIF clk'event and clk = '1' THEN
			if linecount = 0 or reset_crc = '1' then
				f_crc <= conv_std_logic_vector(0,16);
			elsif full_crc_range = '1' then
				f_crc <= crc_0_5_12_16(fdata_in,f_crc);
			else
			end if;
			if linecount = 0 or reset_crc = '1' then
				a_crc <= conv_std_logic_vector(0,16);
			elsif active_crc_range = '1' then
				a_crc <= crc_0_5_12_16(fdata_in,a_crc);
			else
			end if;
		END IF;

	END PROCESS crc_calculate;

	fdata_in <= conv_std_logic_vector(1023,10) when data_in >= 1020 else
				data_in;
	
	edh_insert:
	PROCESS
	BEGIN
		WAIT UNTIL clk = '1';
		CASE video_type IS
			WHEN "000" => -- ntsc 13.5 mhx
				if (linecount = 9 or linecount = 272) and (wordcount >= 1689 and wordcount <= 1711) then
					data_out <= edhword;
				else
					data_out <= data_in;	-- not edh packet, pass data through
				end if;
			WHEN "001" => -- ntsc 18 mhz
				if (linecount = 9 or linecount = 272) and (wordcount >= 2261 and wordcount <= 2283) then
					data_out <= edhword;
				else
					data_out <= data_in;	-- not edh packet, pass data through
				end if;
			WHEN "010" => -- pal 13.5 mhz
				if (linecount = 5 or linecount = 318) and (wordcount >= 1701 and wordcount <= 1723) then
					data_out <= edhword;
				else
					data_out <= data_in;	-- not edh packet, pass data through
				end if;
			WHEN "011" => -- pal 18 mhz
				if (linecount = 5 or linecount = 318) and (wordcount >= 2277 and wordcount <= 2299) then
					data_out <= edhword;
				else
					data_out <= data_in;	-- not edh packet, pass data through
				end if;
			WHEN "100" => -- ntsc 4:4:4:4 
				if (linecount = 9 or linecount = 272) and (wordcount >= 3405 and wordcount <= 3427) then
					data_out <= edhword;
				else
					data_out <= data_in;	-- not edh packet, pass data through
				end if;
			WHEN "110" => -- pal 4:4:4:4
				if (linecount = 5 or linecount = 318) and (wordcount >= 3429 and wordcount <= 3451) then
					data_out <= edhword;
				else
					data_out <= data_in;	-- not edh packet, pass data through
				end if;
			WHEN OTHERS =>
				data_out <= data_in;
		END CASE;
	END PROCESS edh_insert;

crc_ranges:
process(linecount, wordcount, video_type)
begin
CASE video_type IS
	WHEN "000" =>
		if ((linecount >= 21 and linecount <= 262) or (linecount >= 284 and linecount <= 525))
				and wordcount <= 1439 then
			active_crc_range <= '1';
		else
			active_crc_range <= '0';
		end if;
		if ((linecount = 12 or linecount = 275) and (wordcount <= 1439 or wordcount >= 1444))
				or (linecount >= 13 and linecount <= 271) or (linecount >= 276 and linecount <= 525) or (linecount <= 8) then
			full_crc_range <= '1';
		else
			full_crc_range <= '0';
		end if;
		if (linecount = 9 or linecount = 272) and wordcount = 0 then
			reset_crc <= '1';
		else
			reset_crc <= '0';
		end if;
	WHEN "001" =>
		if ((linecount >= 21 and linecount <= 262) or (linecount >= 284 and linecount <= 525))
				and wordcount <= 1919 then
			active_crc_range <= '1';
		else
			active_crc_range <= '0';
		end if;
		if ((linecount = 12 or linecount = 275) and (wordcount <= 1919 or wordcount >= 1924))
				or (linecount >= 13 and linecount <= 271) or (linecount >= 276 and linecount <= 525) or (linecount <= 8) then
			full_crc_range <= '1';
		else
			full_crc_range <= '0';
		end if;
		if (linecount = 9 or linecount = 272) and wordcount = 0 then
			reset_crc <= '1';
		else
			reset_crc <= '0';
		end if;
	WHEN "010" =>
		if ((linecount >= 24 and linecount <= 310) or (linecount >= 336 and linecount <= 622))
				and wordcount <= 1439 then
			active_crc_range <= '1';
		else
			active_crc_range <= '0';
		end if;
		if ((linecount = 8 or linecount = 321) and (wordcount <= 1439 or wordcount >= 1444))
				or (linecount >= 9 and linecount <= 317) or (linecount >= 322 and linecount <= 625) or (linecount <= 4) then
			full_crc_range <= '1';
		else
			full_crc_range <= '0';
		end if;
		if (linecount = 5 or linecount = 318) and wordcount = 0 then
			reset_crc <= '1';
		else
			reset_crc <= '0';
		end if;
	WHEN "011" =>
		if ((linecount >= 24 and linecount <= 310) or (linecount >= 336 and linecount <= 622))
				and wordcount <= 1919 then
			active_crc_range <= '1';
		else
			active_crc_range <= '0';
		end if;
		if ((linecount = 8 or linecount = 321) and (wordcount <= 1919 or wordcount >= 1924))
				or (linecount >= 9 and linecount <= 317) or (linecount >= 322 and linecount <= 625) or (linecount <= 4) then
			full_crc_range <= '1';
		else
			full_crc_range <= '0';
		end if;
		if (linecount = 5 or linecount = 318) and wordcount = 0 then
			reset_crc <= '1';
		else
			reset_crc <= '0';
		end if;
	WHEN "100" =>
		if ((linecount >= 21 and linecount <= 262) or (linecount >= 284 and linecount <= 525))
				and wordcount <= 2879 then
			active_crc_range <= '1';
		else
			active_crc_range <= '0';
		end if;
		if ((linecount = 12 or linecount = 275) and (wordcount <= 2879 or wordcount >= 2884))
				or (linecount >= 13 and linecount <= 271) or (linecount >= 276 and linecount <= 525) or (linecount <= 8) then
			full_crc_range <= '1';
		else
			full_crc_range <= '0';
		end if;
		if (linecount = 9 or linecount = 272) and wordcount = 0 then
			reset_crc <= '1';
		else
			reset_crc <= '0';
		end if;
	WHEN "110" =>
		if ((linecount >= 24 and linecount <= 310) or (linecount >= 336 and linecount <= 622))
				and wordcount <= 2879 then
			active_crc_range <= '1';
		else
			active_crc_range <= '0';
		end if;
		if ((linecount = 8 or linecount = 321) and (wordcount <= 2879 or wordcount >= 2884))
				or (linecount >= 9 and linecount <= 317) or (linecount >= 321 and linecount <= 625) or (linecount <= 4) then
			full_crc_range <= '1';
		else
			full_crc_range <= '0';
		end if;
		if (linecount = 5 or linecount = 318) and wordcount = 0 then
			reset_crc <= '1';
		else
			reset_crc <= '0';
		end if;
	WHEN OTHERS =>
		active_crc_range <= '0';
		full_crc_range <= '0';
END CASE;
end process crc_ranges;

edh_locate:
PROCESS (wordcount, linecount, video_type)
BEGIN
	CASE video_type IS
		WHEN "000" =>
			if (linecount = 9 or linecount = 272) and wordcount >= 1689 and wordcount <= 1711 then
				edhcount <= wordcount - 1689;
			else
				edhcount <= conv_std_logic_vector(0,12);
			end if;
		WHEN "001" =>
			if (linecount = 9 or linecount = 272) and wordcount >= 2261 and wordcount <= 2283 then
				edhcount <= wordcount - 2261;
			else
				edhcount <= conv_std_logic_vector(0,12);
			end if;
		WHEN "010" =>
			if (linecount = 5 or linecount = 318) and wordcount >= 1701 and wordcount <= 1723 then
				edhcount <= wordcount - 1701;
			else
				edhcount <= conv_std_logic_vector(0,12);
			end if;
		WHEN "011" =>
			if (linecount = 5 or linecount = 318) and wordcount >= 2277 and wordcount <= 2299 then
				edhcount <= wordcount - 2277;
			else
				edhcount <= conv_std_logic_vector(0,12);
			end if;
		WHEN "100" =>
			if (linecount = 9 or linecount = 272) and wordcount >= 3405 and wordcount <= 3427 then
				edhcount <= wordcount - 3405;
			else
				edhcount <= conv_std_logic_vector(0,12);
			end if;
		WHEN "110" =>
			if (linecount = 5 or linecount = 318) and wordcount >= 3429 and wordcount <= 3451 then
				edhcount <= wordcount - 3429;
			else
				edhcount <= conv_std_logic_vector(0,12);
			end if;
		WHEN OTHERS =>
			edhcount <= conv_std_logic_vector(0,12);
	END CASE;

END PROCESS edh_locate;

crc_compare:
process
begin
	wait until clk = '1';
	case edhcount is
		when conv_std_logic_vector(1,12) =>
			if fdata_in = "1111111111" then
				edh_detected <= '1';
			else
				edh_detected <= '0';
			end if;
		when conv_std_logic_vector(6,12) =>
			rx_acrc(5 downto 0) <= data_in(7 downto 2);
		when conv_std_logic_vector(7,12) =>
			rx_acrc(11 downto 6) <= data_in(7 downto 2);
		when conv_std_logic_vector(8,12) =>
			rx_acrc(15 downto 12) <= data_in(5 downto 2);
		when conv_std_logic_vector(9,12) =>
			rx_fcrc(5 downto 0) <= data_in(7 downto 2);
		when conv_std_logic_vector(10,12) =>
			rx_fcrc(11 downto 6) <= data_in(7 downto 2);
		when conv_std_logic_vector(11,12) =>
			rx_fcrc(15 downto 12) <= data_in(5 downto 2);
		when conv_std_logic_vector(12,12) =>
			if edh_detected = '1' then
				if rx_acrc /= a_crc then
					inc_acrc_errors <= '1';
				else
					inc_acrc_errors <= '0';
				end if;
				
				if rx_fcrc /= f_crc then
					inc_fcrc_errors <= '1';
				else
					inc_fcrc_errors <= '0';
				end if;
			end if;
--			edh_detected <= '0';
		when others =>
			inc_acrc_errors <= '0';
			inc_fcrc_errors <= '0';
	end case;
end process crc_compare;

crc_error_counters:
process(enable,clk)
begin
	if enable = '0' then
		acrc_errors <= CONV_STD_LOGIC_VECTOR(0,8);
		fcrc_errors <= CONV_STD_LOGIC_VECTOR(0,8);
		read_acrc_errors0 <= '0';
		read_fcrc_errors0 <= '0';
	elsif clk'event and clk='1' then
		read_acrc_errors0 <= read_acrc_errors;
		read_fcrc_errors0 <= read_fcrc_errors;
		if read_acrc_errors = '0' and read_acrc_errors0 = '1' then
			acrc_errors <= CONV_STD_LOGIC_VECTOR(0,8);
		elsif inc_acrc_errors = '1' then
			acrc_errors <= acrc_errors + 1;
		end if;
		if read_fcrc_errors = '0' and read_fcrc_errors0 = '1' then
			fcrc_errors <= CONV_STD_LOGIC_VECTOR(0,8);
		elsif inc_fcrc_errors = '1' then
			fcrc_errors <= acrc_errors + 1;
		end if;
	end if;
end process;
--calculate edh word values

expression <= "0000000000";
adef <= "1000000000";
apef <= "1000000000";
ffef <= "1000000000";

acrc_5_0(1 downto 0) <= "00";
acrc_5_0(7 downto 2) <= a_crc(5 downto 0);
acrc_5_0(8) <= acrc_5_0(7) xor acrc_5_0(6) xor acrc_5_0(5) xor acrc_5_0(4) xor acrc_5_0(3) xor acrc_5_0(2);
acrc_5_0(9) <= not acrc_5_0(8);

acrc_11_6(1 downto 0) <= "00";
acrc_11_6(7 downto 2) <= a_crc(11 downto 6);
acrc_11_6(8) <= acrc_11_6(7) xor acrc_11_6(6) xor acrc_11_6(5) xor acrc_11_6(4) xor acrc_11_6(3) xor acrc_11_6(2);
acrc_11_6(9) <= not acrc_11_6(8);

acrc_15_12(1 downto 0) <= "00";
acrc_15_12(5 downto 2) <= a_crc(15 downto 12);
acrc_15_12(7 downto 6) <= "10";
acrc_15_12(8) <= acrc_15_12(7) xor acrc_15_12(6) xor acrc_15_12(5) xor acrc_15_12(4) xor acrc_15_12(3) xor acrc_15_12(2);
acrc_15_12(9) <= not acrc_15_12(8);

fcrc_5_0(1 downto 0) <= "00";
fcrc_5_0(7 downto 2) <= f_crc(5 downto 0);
fcrc_5_0(8) <= fcrc_5_0(7) xor fcrc_5_0(6) xor fcrc_5_0(5) xor fcrc_5_0(4) xor fcrc_5_0(3) xor fcrc_5_0(2);
fcrc_5_0(9) <= not fcrc_5_0(8);

fcrc_11_6(1 downto 0) <= "00";
fcrc_11_6(7 downto 2) <= f_crc(11 downto 6);
fcrc_11_6(8) <= fcrc_11_6(7) xor fcrc_11_6(6) xor fcrc_11_6(5) xor fcrc_11_6(4) xor fcrc_11_6(3) xor fcrc_11_6(2);
fcrc_11_6(9) <= not fcrc_11_6(8);

fcrc_15_12(1 downto 0) <= "00";
fcrc_15_12(5 downto 2) <= f_crc(15 downto 12);
fcrc_15_12(7 downto 6) <= "10";
fcrc_15_12(8) <= fcrc_15_12(7) xor fcrc_15_12(6) xor fcrc_15_12(5) xor fcrc_15_12(4) xor fcrc_15_12(3) xor fcrc_15_12(2);
fcrc_15_12(9) <= not fcrc_15_12(8);

checksum(8 downto 0) <= conv_std_logic_vector(500,9) + conv_std_logic_vector(272,9) +
						acrc_5_0(8 downto 0) + acrc_11_6(8 downto 0) + acrc_15_12(8 downto 0)+
						fcrc_5_0(8 downto 0) + fcrc_11_6(8 downto 0) + fcrc_15_12(8 downto 0)+
						adef(8 downto 0) + apef(8 downto 0) + ffef(8 downto 0);
checksum(9) <= not checksum(8);

WITH edhcount SELECT
	edhword	<=		"0000000000"	WHEN	"000000000000",   -- conv_std_logic_vector(0,12),
					"1111111111"	WHEN	"000000000001",   --conv_std_logic_vector(1,12),
					"1111111111"	WHEN	"000000000010",   --conv_std_logic_vector(2,12),
					"0111110100"	WHEN	"000000000011",   --conv_std_logic_vector(3,12),
					"1000000000"	WHEN	"000000000100",   --conv_std_logic_vector(4,12),
					"0100010000"	WHEN	"000000000101",   --conv_std_logic_vector(5,12),
					acrc_5_0		WHEN	"000000000110",   --conv_std_logic_vector(6,12),
					acrc_11_6		WHEN	"000000000111",   --conv_std_logic_vector(7,12),
					acrc_15_12	WHEN	"000000001000",   --conv_std_logic_vector(8,12),
					fcrc_5_0		WHEN	"000000001001",   --conv_std_logic_vector(9,12),
					fcrc_11_6		WHEN	"000000001010",   --conv_std_logic_vector(10,12),
					fcrc_15_12	WHEN	"000000001011",   --conv_std_logic_vector(11,12),
					adef			WHEN	"000000001100",   --conv_std_logic_vector(12,12),
					apef			WHEN	"000000001101",   --conv_std_logic_vector(13,12),
					ffef			WHEN	"000000001110",   --conv_std_logic_vector(14,12),
					"1000000000"	WHEN	"000000001111",   --conv_std_logic_vector(15,12),
					"1000000000"	WHEN	"000000010000",   --conv_std_logic_vector(16,12),
					"1000000000"	WHEN	"000000010001",   --conv_std_logic_vector(17,12),
					"1000000000"	WHEN	"000000010010",   --conv_std_logic_vector(18,12),
					"1000000000"	WHEN	"000000010011",   --conv_std_logic_vector(19,12),
					"1000000000"	WHEN	"000000010100",   --conv_std_logic_vector(20,12),
					"1000000000"	WHEN	"000000010101",   --conv_std_logic_vector(21,12),
					checksum		WHEN	"000000010110",   --conv_std_logic_vector(22,12),
					"0000000000"	WHEN OTHERS;

END a;
