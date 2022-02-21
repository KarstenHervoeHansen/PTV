-- eg1.vhd
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
-- EG1 Colour bar generation
-- This module gnereates 10 bit EG1 colour bars
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
						
-- 601 Color values						
--                  R       G       B       Y   Cb   Cr
-- 100% White      1       1       1       940  512  512
--  75% White      0.75    0.75    0.75    721  512  512
--  75% yellow     0.75    0.75    0       646  176  567
--  75% cyan       0       0.75    0.75    525  625  176
--  75% green      0       0.75    0       450  289  231
--  75% magenta    0.75    0       0.75    335  735  793
--  75% red        0.75    0       0       260  399  848
--  75% blue       0       0       0.75    139  848  457
-- black-4%       -0.04   -0.04   -0.04     29  512  512
-- black+4%        0.04    0.04    0.04     99  512  512
-- -I (legal RGB)  0.0000  0.2456  0.4125  231  624  390
-- +Q (legal RGB)  0.2536  0.0000  0.4703  177  684  591
--

LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;

ENTITY eg1 IS
	PORT ( 
		clk,enable	 : in std_logic;
		sel : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
		data : out std_logic_vector(9 downto 0);
		linecount_out: buffer std_logic_vector(9 downto 0);
		wordcount_out : buffer std_logic_vector(11 downto 0)
	);
END eg1;

ARCHITECTURE a_eg1 OF eg1 IS

signal linecount, data_out : std_logic_vector(9 downto 0);
signal wordcount : std_logic_vector(11 downto 0);
signal xyz, xyz0, xyz1 : std_logic_vector(9 downto 0);
signal F,V,H : std_logic;
signal P : std_logic_vector(3 downto 0);
signal p1 : std_logic_vector(4 downto 0);
signal TSL, ASL, LINES, VBLANK, HALFLINES, LINES1, LINES2, b, BD4, BD3 : INTEGER;
SIGNAL BT2, BT3, BT4, BT5, BT6 : INTEGER;
SIGNAL BD4T5, BD4T10, BD4T15, BD3T2 : INTEGER;

constant KEY : std_logic_vector(9 downto 0) := "0001000000";

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
WITH sel SELECT -- END OF FIRST BAND OF BARS
	LINES1	<=	182	WHEN	"000",
				182	WHEN	"001",
				216	WHEN	"010",
				216	WHEN	"011",
				182 WHEN	"100",
				216 WHEN	"110",
				0 	WHEN 	OTHERS;
WITH sel SELECT -- END OF SECOND BAND OF BARS
	LINES2	<=	201	WHEN	"000",
				201	WHEN	"001",
				239	WHEN	"010",
				239	WHEN	"011",
				201 WHEN	"100",
				239 WHEN	"110",
				0 	WHEN 	OTHERS;
WITH sel SELECT  --ASL/7;
	b	<=	205	WHEN	"000",
			274	WHEN	"001",
			205	WHEN	"010",
			274	WHEN	"011",
			411 WHEN 	"100",
			411 WHEN	"110",
			0 	WHEN	OTHERS;
WITH sel SELECT -- b/3
	BD3 <=	68 WHEN "000",
			91 WHEN "001",
			68 WHEN "010",
			91 WHEN "011",
			137 WHEN "100",
			137 WHEN "110",
			0 WHEN OTHERS;
WITH sel SELECT  -- b/3*2
	BD3T2 <=	136 WHEN "000",
				182 WHEN "001",
				136 WHEN "010",
				182 WHEN "011",
				274 WHEN "100",
				274 WHEN "110",
				0 WHEN OTHERS;
WITH sel SELECT  -- b/4
	BD4 <=	51 WHEN "000",
			68 WHEN "001",
			51 WHEN "010",
			68 WHEN "011",
			103 WHEN "100",
			103 WHEN "110",
			0 WHEN OTHERS;
WITH sel SELECT  -- b/4*5
	BD4T5 <=	255 WHEN "000",
				340 WHEN "001",
				255 WHEN "010",
				340 WHEN "011",
				514 WHEN "100",
				514 WHEN "110",
				0 WHEN OTHERS;
WITH sel SELECT  -- b/4*10
	BD4T10 <=	510 WHEN "000",
				680 WHEN "001",
				510 WHEN "010",
				680 WHEN "011",
				1028 WHEN "100",
				1028 WHEN "110",
				0 WHEN OTHERS;
WITH sel SELECT  -- b/4*15
	BD4T15 <=	765 WHEN "000",
				1020 WHEN "001",
				765 WHEN "010",
				1020 WHEN "011",
				1541 WHEN "100",
				1541 WHEN "110",
				0 WHEN OTHERS;
WITH sel SELECT  -- b*2
	BT2 <=	410 WHEN "000",
			548 WHEN "001",
			410 WHEN "010",
			548 WHEN "011",
			822 WHEN "100",
			822 WHEN "110",
			0 WHEN OTHERS;
WITH sel SELECT  -- b*3
	BT3 <=	615 WHEN "000",
			822 WHEN "001",
			615 WHEN "010",
			822 WHEN "011",
			1234 WHEN "100",
			1234 WHEN "110",
			0 WHEN OTHERS;
WITH sel SELECT  --b*4
	BT4 <=	820 WHEN "000",
			1096 WHEN "001",
			820 WHEN "010",
			1096 WHEN "011",
			1646 WHEN "100",
			1646 WHEN "110",
			0 WHEN OTHERS;
WITH sel SELECT  -- b*5
	BT5 <=	1025 WHEN "000",
			1370 WHEN "001",
			1025 WHEN "010",
			1370 WHEN "011",
			2057 WHEN "100",
			2057 WHEN "110",
			0 WHEN OTHERS;
WITH sel SELECT  -- b*6
	BT6 <=	1230 WHEN "000",
			1644 WHEN "001",
			1230 WHEN "010",
			1644 WHEN "011",
			2469 WHEN "100",
			2469 WHEN "110",
			0 WHEN OTHERS;
			
process (enable,clk,asl) begin
	if enable = '0' then
		wordcount <= conv_std_logic_vector(ASL,12);
		linecount <= conv_std_logic_vector(0,10);
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
		data_out <= "0000000000";
	elsif clk'event and clk='1' then
		if wordcount = ASL or wordcount=(TSL-4)then	-- TRS interval
			data_out <= "1111111111";
		elsif ((wordcount = ASL+1) or (wordcount = ASL+2) or (wordcount = TSL-3) or (wordcount = TSL-2)) then
			data_out <= "0000000000";
		elsif wordcount = ASL+3 or wordcount = TSL-1 then
			data_out <= xyz;
		elsif wordcount > ASL+3 then	-- horizontal blanking interval
 	    	if wordcount(0) = '1' then 
				data_out <= "0001000000"; -- 0x040
			else
				data_out <= "1000000000"; -- 0x200
			end if;
		elsif linecount <= VBLANK or (linecount > HALFLINES and linecount < HALFLINES+VBLANK) then -- vertical blanking interval
 	    	if wordcount(0) = '1' then 
				data_out <= "0001000000"; -- 0x040
			else
				data_out <= "1000000000"; -- 0x200
			end if;
		elsif (linecount <= LINES1 or (linecount > HALFLINES+VBLANK and linecount <= HALFLINES+LINES1)) then  -- top band of bars
			if wordcount < b then -- first bar ( grey )
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(512,10);	    --GREY_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(512,10);	    --GREY_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(721,10);	 	--GREY_Y
					end if;
				end if;
			elsif wordcount < BT2 then -- second bar ( yellow )
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(176,10);	    --YELLOW_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(567,10);	    --YELLOW_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(646,10);	 	--YELLOW_Y
					end if;
				end if;
			elsif wordcount < BT3 then -- third bar ( cyan )
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(625,10);	    --CYAN_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(176,10);	    --CYAN_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(525,10);	 	--CYAN_Y
					end if;
				end if;
			elsif wordcount < BT4 then -- fourth bar ( green )
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(289,10);	    --GREEN_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(231,10);	    --GREEN_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(450,10);	 	--GREEN_Y
					end if;
				end if;
			elsif wordcount < BT5 then -- fifth bar ( magenta )
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(735,10);	    --MAGENTA_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(793,10);	    --MAGENTA_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(335,10);	 	--MAGENTA_Y
					end if;
				end if;
			elsif wordcount < BT6 then -- sixth bar ( red )
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(399,10);	    --RED_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(848,10);	    --RED_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(260,10);	 	--RED_Y
					end if;
				end if;
			else 						-- seventh bar ( blue )
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(848,10);	    --BLUE_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(457,10);	    --BLUE_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(139,10);	 	--BLUE_Y
					end if;
				end if;
			end if;		
		elsif linecount <= LINES2 or (linecount > HALFLINES+LINES1 and linecount <= HALFLINES+LINES2) then  -- middle band of bars
			if (wordcount < b) then -- first bar ( blue )
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(848,10);	    --BLUE_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(457,10);	    --BLUE_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(139,10);	 	--BLUE_Y
					end if;
				end if;
			elsif wordcount < BT2 then -- second bar ( black )
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(512,10);	    --BLACK_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(512,10);	    --BLACK_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(64,10);	 		--BLACK_Y
					end if;
				end if;
			elsif wordcount < BT3 then -- third bar ( magenta )
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(735,10);	    --MAGENTA_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(793,10);	    --MAGENTA_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(335,10);	 	--MAGENTA_Y
					end if;
				end if;
			elsif wordcount < BT4 then -- fourth bar ( black )
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(512,10);	    --BLACK_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(512,10);	    --BLACK_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(64,10);	 		--BLACK_Y
					end if;
				end if;
			elsif wordcount < BT5 then -- fifth bar ( cyan )
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(625,10);	    --CYAN_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(176,10);	    --CYAN_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(525,10);	 	--CYAN_Y
					end if;
				end if;
			elsif wordcount < BT6 then -- sixth bar ( black )
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(512,10);	    --BLACK_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(512,10);	    --BLACK_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(64,10);	 		--BLACK_Y
					end if;
				end if;
			else 						-- seventh bar ( grey )
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(512,10);	    --GREY_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(512,10);	    --GREY_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(721,10);	 	--GREY_Y
					end if;
				end if;
			end if;
		else																	-- bottom band of bars
			if wordcount < (BD4T5) then -- -I
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(624,10);	    --I_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(390,10);	    --I_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(231,10);	 	--I_Y
					end if;
				end if;
			elsif wordcount < (BD4T10) then -- 100% White
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(512,10);	    --WHITE_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(512,10);	    --WHITE_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(940,10);	 	--WHITE_Y
					end if;
				end if;
			elsif wordcount < (BD4T15) then -- +Q 
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(684,10);	    --Q_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(591,10);	    --Q_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(177,10);	 	--Q_Y
					end if;
				end if;
			elsif wordcount < (BT5) then -- Black
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(512,10);	    --BLACK_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(512,10);	    --BLACK_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(64,10);	 		--BLACK_Y
					end if;
				end if;
			elsif wordcount < (BT5+BD3) then -- Black - 4%
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(512,10);	    --BLACK-4_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(512,10);	    --BLACK-4_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(29,10);	 		--BLACK-4_Y
					end if;
				end if;
			elsif wordcount < (BT5+BD3T2) then -- Black 
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(512,10);	    --BLACK_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(512,10);	    --BLACK_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(64,10);	 		--BLACK_Y
					end if;
				end if;
			elsif wordcount < (BT6) then -- Black + 4%
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(512,10);	    --BLACK+4_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(512,10);	    --BLACK+4_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(99,10);	 	--BLACK+4_Y
					end if;
				end if;
			else						-- Black
				if wordcount(1 downto 0) = "00" then
					data_out <= conv_std_logic_vector(512,10);	    --BLACK_CB
				elsif wordcount(1 downto 0) = "10" then
					data_out <= conv_std_logic_vector(512,10);	    --BLACK_CR
				else
					if (sel = "110" or sel = "100") and wordcount(1 downto 0) = "11" then
						data_out <= KEY;
					else
						data_out <= conv_std_logic_vector(64,10);	 		--BLACK_Y
					end if;
				end if;
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

data <= data_out;

xyz0(9) <= '1';
xyz0(8) <= F;
xyz0(7) <= V;
xyz0(6) <= H;
xyz0(5 downto 2) <= P;
xyz0(1 downto 0) <= "00";
WITH xyz0(8 downto 6) SELECT
    p <=
	"0000" WHEN "000",
	"1101" WHEN "001",
	"1011" WHEN "010",
	"0110" WHEN "011",
	"0111" WHEN "100",
	"1010" WHEN "101",
	"1100" WHEN "110",
	"0001" WHEN OTHERS;

xyz1(9) <= '1';
xyz1(8) <= F;
xyz1(7) <= V;
xyz1(6) <= H;
xyz1(5) <= '1'; -- S bit set to '1' to indicate Y,Cb,Cr signals
xyz1(4 downto 0) <= p1;
WITH xyz1(8 downto 5) SELECT
    p1 <=
	"00000" WHEN "0000",
	"01110" WHEN "0001",
	"10110" WHEN "0010",
	"11000" WHEN "0011",
	"11100" WHEN "0100",
	"10010" WHEN "0101",
	"01010" WHEN "0110",
	"00100" WHEN "0111",
	"11010" WHEN "1000",
	"10100" WHEN "1001",
	"01100" WHEN "1010",
	"00010" WHEN "1011",
	"00110" WHEN "1100",
	"01000" WHEN "1101",
	"10000" WHEN "1110",
	"11110"	WHEN OTHERS;
	
WITH sel(2) SELECT
	xyz <= xyz0 when '0',
			xyz1 when others;

END a_eg1; 
