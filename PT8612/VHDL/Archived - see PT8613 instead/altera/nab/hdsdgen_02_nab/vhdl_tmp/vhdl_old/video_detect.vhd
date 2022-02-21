-- video_detect.vhd
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
-- determine video type
-- this module finds the video type by counting sampes between EAV and SAV
-- Sample number is determined by first sample after SAV being 0
-- line number is determined by F bit transition in accordance with ITU-R BT.656-4
-- Video types are: 
--    0 = NTSC 525 line 13.5 MHz, 
--    1 = NTSC 525 line 18 MHz, 
--    2 = PAL 625 line 13.5 MHz,
--    3 = PAL 625 Line 18MHz, 
--    4 = NTSC 525 line 4:4:4:4, 
--    6 = PAL 525 line 4:4:4:4,
--    7 = unknown or not yet determined
--
-- data_out, count_out, video_type_out, and linecount_out are generated for use by edh.vhd so that
-- it can locate edh packets in SDI video streams
--
LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;

ENTITY video_detect IS
	PORT(
		clk, reset						: IN	STD_LOGIC;							-- clock and reset lines
		data_in							: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);		-- 10 bit video data in
		data_out						: buffer	STD_LOGIC_VECTOR(9 DOWNTO 0);	-- 10 bit video data out
		count_out						: buffer 	std_logic_vector(11 downto 0);	-- sample count, valid if linecount != 0, syncronized to data_out
		video_type_out					: buffer 	std_logic_vector(2 downto 0);	-- video type indicator
		linecount_out					: buffer	std_logic_vector(9 downto 0)	-- video line number, syncronized to data_out
	);
END video_detect;

ARCHITECTURE a OF video_detect IS
	signal fdata_in : std_logic_vector(9 downto 0);
	signal video_type : integer range 0 to 7;
	signal count : integer range 0 to 3456;
	signal linecount : integer range 0 to 625;
	signal trs_errors : integer range 0 to 3;
	signal old_f, f : std_logic;
	TYPE STATE_TYPE IS (wait_TRS, trs0, trs1, test_EAV, wait_SAV, foundsav);
	SIGNAL state   : STATE_TYPE;

BEGIN
	data_filter:
	PROCESS
	BEGIN
		WAIT UNTIL clk = '1';
		if data_in >= 1020 then
			fdata_in <= conv_std_logic_vector(1023,10);
		else 
			fdata_in <= data_in;
		end if;
		data_out <= data_in;
	END PROCESS data_filter;

	video_detect:
	PROCESS(clk, reset)
	BEGIN
		if reset = '1' then
			state <= wait_TRS;
			count <= 0;
			video_type <= 7;
		elsif clk'event and clk = '1' then
			old_f <= f;
			CASE state IS
				WHEN wait_TRS =>
					count <= 0;
					linecount <= 0;
					trs_errors <= 0;
					if fdata_in = "1111111111" then
						state <= trs0;
					end if;
				WHEN trs0 =>
					if data_out = "0000000000" then
						state <= trs1;
					else
						state <= wait_TRS;
					end if;
				WHEN trs1 =>
					if data_out = "0000000000" then
						state <= test_EAV;
					else
						state <= wait_TRS;
					end if;
				WHEN test_EAV =>
					count <= count+4;
					if data_out(6) = '1' then	-- test h bit for EAV
						state <= wait_SAV;
						f <= data_out(8);
					else						-- if not EAV go back to waiting for TRS
						state <= wait_TRS;
					end if;
				when wait_SAV =>
					if fdata_in = "1111111111" then
						CASE count IS
							WHEN 272 =>
								state <= foundsav;
								video_type <= 0;
								count <= 1713;
							WHEN 364 =>
								state <= foundsav;
								video_type <= 1;
								count <= 2285;
							WHEN 284 =>
								state <= foundsav;
								video_type <= 2;
								count <= 1725;
							WHEN 380 =>
								state <= foundsav;
								video_type <= 3;
								count <= 2301;
							WHEN 548 =>
								state <= foundsav;
								video_type <= 4;
								count <= 3429;
							WHEN 572 =>
								state <= foundsav;
								video_type <= 6;
								count <= 3453;
							WHEN OTHERS =>
								state <= wait_TRS;
								video_type <= 7;
								count <= 0;
								linecount <= 0;
						END CASE;
					else
						count <= count +1;
					end if;
				when foundsav =>
					if trs_errors > 1 then
						state <= wait_TRS;
						video_type <= 7;
						count <= 0;
						linecount <= 0;
					else
						case video_type is
							when 0 =>
								if count < 1715 then
									count <= count+1;
								else count <= 0;
								end if;
								if count=1443 then
									f <= data_out(8);
								end if;
								IF count=1440 or count = 1712 THEN
									if fdata_in /= "1111111111" then
										trs_errors <= trs_errors+1;
									else
										trs_errors <= 0;
									end if;
								END IF;
								if old_f = '1' and f = '0' then
									linecount <= 4;
								elsif old_f = '0' and f = '1' then
									linecount <= 266;
								elsif linecount /= 0 and count = 1439 then
									if linecount < 525 then
										linecount <= linecount+1;
									else
										linecount <= 1;
									end if;
								end if;
							when 1 =>
								if count < 2287 then
									count <= count+1;
								else count <= 0;
								end if;
								if count=1923 then
									f <= data_out(8);
								end if;
								IF count=1920 or count = 2284 THEN
									if fdata_in /= "1111111111" then
										trs_errors <= trs_errors+1;
									else
										trs_errors <= 0;
									end if;
								END IF;
								if old_f = '1' and f = '0' then
									linecount <= 4;
								elsif old_f = '0' and f = '1' then
									linecount <= 266;
								elsif linecount /= 0 and count = 1919 then
									if linecount < 525 then
										linecount <= linecount+1;
									else
										linecount <= 1;
									end if;
								end if;
							when 2 => 
								if count < 1727 then
									count <= count+1;
								else count <= 0;
								end if;
								if count=1443 then
									f <= data_out(8);
								end if;
								IF count=1440 or count = 1724 THEN
									if fdata_in /= "1111111111" then
										trs_errors <= trs_errors+1;
									else
										trs_errors <= 0;
									end if;
								END IF;
								if old_f = '1' and f = '0' then
									linecount <= 1;
								elsif old_f = '0' and f = '1' then
									linecount <= 313;
								elsif linecount /= 0 and count = 1439 then
									if linecount < 625 then
										linecount <= linecount+1;
									else
										linecount <= 1;
									end if;
								end if;
							when 3 =>
								if count < 2303 then
									count <= count+1;
								else count <= 0;
								end if;
								if count=1923 then
									f <= data_out(8);
								end if;
								IF count=1920 or count = 2300 THEN
									if fdata_in /= "1111111111" then
										trs_errors <= trs_errors+1;
									else
										trs_errors <= 0;
									end if;
								END IF;
								if old_f = '1' and f = '0' then
									linecount <= 1;
								elsif old_f = '0' and f = '1' then
									linecount <= 313;
								elsif linecount /= 0 and count = 1919 then
									if linecount < 625 then
										linecount <= linecount+1;
									else
										linecount <= 1;
									end if;
								end if;
							when 4 =>
								if count < 3431 then
									count <= count+1;
								else count <= 0;
								end if;
								if count=2883 then
									f <= data_out(8);
								end if;
								IF count=2880 or count = 3428 THEN
									if fdata_in /= "1111111111" then
										trs_errors <= trs_errors+1;
									else
										trs_errors <= 0;
									end if;
								END IF;
								if old_f = '1' and f = '0' then
									linecount <= 4;
								elsif old_f = '0' and f = '1' then
									linecount <= 266;
								elsif linecount /= 0 and count = 2879 then
									if linecount < 525 then
										linecount <= linecount+1;
									else
										linecount <= 1;
									end if;
								end if;
							when 6 =>
								if count < 3455 then
									count <= count+1;
								else count <= 0;
								end if;
								if count=2883 then
									f <= data_out(8);
								end if;
								IF count=2880 or count = 3452 THEN
									if fdata_in /= "1111111111" then
										trs_errors <= trs_errors+1;
									else
										trs_errors <= 0;
									end if;
								END IF;
								if old_f = '1' and f = '0' then
									linecount <= 1;
								elsif old_f = '0' and f = '1' then
									linecount <= 313;
								elsif linecount /= 0 and count = 2879 then
									if linecount < 625 then
										linecount <= linecount+1;
									else
										linecount <= 1;
									end if;
								end if;
							WHEN OTHERS =>
								state <= wait_TRS;
								video_type <= 7;
								count <= 0;
								linecount <= 0;
						end case;
					end if;
			END CASE;
		end if;
	END PROCESS video_detect;

	video_type_out <= conv_std_logic_vector(video_type,3);
	count_out <= conv_std_logic_vector(count,12);
	linecount_out <= conv_std_logic_vector(linecount, 10);

END a;
