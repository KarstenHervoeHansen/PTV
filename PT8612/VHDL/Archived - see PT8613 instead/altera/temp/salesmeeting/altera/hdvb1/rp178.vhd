-- rp178.vhd
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
-- rp178 video generation
-- This module gnereates 10 bit flat grey video
-- NTSC and PAL formats are supported for 270, 360 and 540 Mbit/sec SDI output
--
-- output video type is determined by sel as follows:
--    	"000",	--259C NTSC (270 Mbit/s)
--      "001",	--259D NTSC (270 Mbit/s)
--      "010",	--259C PAL (360 Mbit/s)
--      "011",	--259D PAL (360 Mbit/s)
--      "100",	--344M NTSC 4:4:4:4 (540 Mbit/s)
--      "110",  --344M PAL 4:4:4:4 (540 Mbit/s)
--
--
LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;

ENTITY rp178 IS
	PORT ( 
		clk,enable,
		std_altn	 : in std_logic;	-- '1' = standard distribution, '0' = alternate distribution
		sel : in std_logic_vector(2 downto 0);
		data : out std_logic_vector(9 downto 0);
		linecount_out: buffer std_logic_vector(9 downto 0);
		wordcount_out : buffer std_logic_vector(11 downto 0)

	);
END rp178;

ARCHITECTURE a_rp178 OF rp178 IS

signal linecount : std_logic_vector(9 downto 0);
signal wordcount : std_logic_vector(11 downto 0);
signal xyz : std_logic_vector(9 downto 0);
signal F,V,H : std_logic;
signal P : std_logic_vector(3 downto 0);
signal TSL, ASL, LINES, VBLANK, HALFLINES, LINES1 : INTEGER;

BEGIN
WITH sel SELECT -- TOTAL SAMPLES PER LINE
	TSL	<=	1716	WHEN	"000",	--259C NTSC
			2288	WHEN	"001",	--259D NTSC
			1728	WHEN	"010",	--259C PAL
			2304	WHEN	"011",	--259D PAL
			3432	WHEN	"100",	--344M NTSC 4:4:4:4
			3456	WHEN	"110",  --344M PAL 4:4:4:4
			0		WHEN	OTHERS;
WITH sel SELECT -- ACTIVE SAMPLES PER LINE
	ASL	<=	1440	WHEN	"000",
			1920	WHEN	"001",
			1440	WHEN	"010",
			1920	WHEN	"011",
			2880	WHEN	"100",
			2880	WHEN	"110",
			0		WHEN	OTHERS;
WITH sel SELECT -- TOTAL LINES
	LINES	<=	525	WHEN	"000",
				525	WHEN	"001",
				625	WHEN	"010",
				625	WHEN	"011",
				525 WHEN	"100",
				625 WHEN	"110",
				0	WHEN	OTHERS;
WITH sel SELECT -- VERTICAL BALNKING INTERVAL PER FIELD
	VBLANK	<=	19	WHEN	"000",
				19	WHEN	"001",
				22	WHEN	"010",
				22	WHEN	"011",
				19 	WHEN	"100",
				22 	WHEN	"110",
				0 	WHEN 	OTHERS;
WITH sel SELECT -- LINES IN FIRST FIELD
	HALFLINES	<=	263	WHEN	"000",
					263	WHEN	"001",
					313	WHEN	"010",
					313	WHEN	"011",
					263 WHEN	"100",
					313 WHEN	"110",
					0 	WHEN 	OTHERS;
WITH sel SELECT -- END OF FIRST HALF OF SCREEN   ((HALFLINES-VBLANK)*50/100)+VBLANK
	LINES1	<=	141	WHEN	"000",
				141	WHEN	"001",
				168	WHEN	"010",
				168	WHEN	"011",
				141 WHEN	"100",
				168 WHEN	"110",
				0 	WHEN 	OTHERS;

process (enable,clk) begin
	if enable = '0' then
		wordcount <= conv_std_logic_vector(1440,12);
		linecount <= conv_std_logic_vector(1,10);
		wordcount_out  <= conv_std_logic_vector(ASL,12);
		linecount_out <= conv_std_logic_vector(0,10);
	elsif clk'event and clk = '1' then
		wordcount_out <= wordcount;
		linecount_out <= linecount;
		if wordcount < (TSL-1) then
			wordcount <= wordcount+1;
			if wordcount = (ASL-1) then
				if linecount < LINES then
					linecount <= linecount + 1;
				else
					linecount <= conv_std_logic_vector(1,10);
				end if;
			end if;
		else 
			wordcount <= conv_std_logic_vector(0,12);
		end if;
	end if;
end process;

process (enable,clk) begin
	if enable = '0' then
		data <= "0000000000";
	elsif clk'event and clk='1' then
		if wordcount = ASL or wordcount=(TSL-4) then	-- TRS interval
			data <= "1111111111";
		elsif ((wordcount = ASL+1) or (wordcount = ASL+2) or (wordcount = TSL-3) or (wordcount = TSL-2)) then
			data <= "0000000000";
		elsif wordcount = ASL+3 or wordcount = TSL-1 then
			data <= xyz;
		elsif wordcount > ASL+3 then	-- horizontal blanking interval
 	    	if wordcount(0) = '1' then 
				data <= "0001000000"; -- 0x040
			else
				data <= "1000000000"; -- 0x200
			end if;
		elsif linecount <= VBLANK or (linecount > HALFLINES and linecount <= HALFLINES+VBLANK) then -- vertical blanking interval
 	    	if wordcount(0) = '1' then 
				data <= "0001000000"; -- 0x040
			else
				data <= "1000000000"; -- 0x200
			end if;
		ELSIF (linecount > VBLANK and linecount <= LINES1) or(linecount > HALFLINES + VBLANK and linecount <= HALFLINES+LINES1) then -- first half active video field
			if linecount=VBLANK+1 and wordcount = ASL-1 then
				data <= "0010000000";	-- last active sample in line 20 = 0x080
 	    	elsif wordcount(0) = std_altn then 
				data <= "0110011000"; -- 0x198 Y
			else
				data <= "1100000000"; -- 0x300 Cb or Cr
			end if;
		else -- last half active video field
 	    	if wordcount(0) = std_altn then 
				data <= "0100010000"; -- 0x110 Y
			else
				data <= "1000000000"; -- 0x200 Cb or Cr
			end if;
		end if;
	end if;
end process;

process (enable,clk) begin
	if enable = '0' then
		V <= '1';
		F <= '1';
	elsif clk'event and clk = '1' then
		if sel(1) = '0' then
			if linecount = 1 or linecount = 264 then
				V <= '1';
			elsif linecount = 20 or linecount = 283 then
				V <= '0';
			end if;
		else
			if linecount = 624 or linecount = 311 then
				V <= '1';
			elsif linecount = 336 or linecount = 23 then
				V <= '0';
			end if;
		end if;
	
		if(sel(1)='0') then
			if linecount = 4 then
				F <= '0';
			elsif linecount = 266 then
				F <= '1';
			end if;
		else
			if linecount = 1 then
				F <= '0';
			elsif linecount = 313  then
				F <= '1';
			end if;
		end if;

		if wordcount > ASL+3 then 
			H <= '0';
		else
			H <= '1';
		end if;
	end if;
end process;

xyz(9) <= '1';
xyz(8) <= F;
xyz(7) <= V;
xyz(6) <= H;
xyz(5 downto 2) <= P;
xyz(1 downto 0) <= "00";
WITH xyz(8 downto 6) SELECT
    p <=
	"0000" WHEN "000",
	"1101" WHEN "001",
	"1011" WHEN "010",
	"0110" WHEN "011",
	"0111" WHEN "100",
	"1010" WHEN "101",
	"1100" WHEN "110",
	"0001" WHEN OTHERS;

END a_rp178; 

