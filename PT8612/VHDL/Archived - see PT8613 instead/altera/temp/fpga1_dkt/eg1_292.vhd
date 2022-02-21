-- eg1_292.vhd
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
-- This module generates eg1 colour bars for smpte 292 
-- 1080i at 30 frames/sec is supported
--
--
-- 709 Color values						
--                    R       G       B      Y   Cb   Cr
-- 100% White       1.0000  1.0000  1.0000  940  512  512
-- 75% White        0.7500  0.7500  0.7500  721  512  512
-- 75% yellow       0.7500  0.7500  0.0000  674  176  543
-- 75% cyan         0.0000  0.7500  0.7500  581  589  176
-- 75% green        0.0000  0.7500  0.0000  534  253  207
-- 75% magenta      0.7500  0.0000  0.7500  251  771  817
-- 75% red          0.7500  0.0000  0.0000  204  435  848
-- 75% blue         0.0000  0.0000  0.7500  111  848  481
-- black-4%        -0.0400 -0.0400 -0.0400   29  512  512
-- black+4%         0.0400  0.0400  0.0400   99  512  512
-- -I (legal RGB)   0.0000  0.2456  0.4125  244  612  395
-- +Q (legal RGB)   0.2536  0.0000  0.4703  141  697  606
--
use work.pack_reverse_bit_order.all;
library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;
USE work.pack_crc.all;

ENTITY eg1_292 IS
	PORT ( 
		clk,enable	 : in std_logic;
		Y_data : out std_logic_vector(9 downto 0);
		C_data : out STD_LOGIC_VECTOR(9 DOWNTO 0)
	);
END eg1_292;

ARCHITECTURE a_eg1_292 OF eg1_292 IS

signal Y_data_out, C_data_out : std_logic_vector(9 downto 0);
signal linecount : std_logic_vector(10 downto 0);
signal samplecount : std_logic_vector(11 downto 0);
signal xyz : std_logic_vector(9 downto 0);
signal F,V,H : std_logic;
signal P : std_logic_vector(3 downto 0);
signal Y_crc, C_crc : std_logic_vector(17 downto 0);
signal Y_crc0, C_crc0 : std_logic_vector(17 downto 0);

-- SMPTE 292 Values for 1080i at 30 frames/sec (2 fields per frame)
CONSTANT TSL :integer :=2200; -- TOTAL SAMPLES PER LINE
CONSTANT ASL :integer :=1920; -- ACTIVE SAMPLES PER LINE
CONSTANT LINES : INTEGER := 1125; -- TOTAL LINES PER FRAME
CONSTANT VBLANK1A : INTEGER := 20; --  END OF FIRST VERTICAL BALNKING INTERVAL IN FIRST FIELD
CONSTANT VBLANK1B : INTEGER := 561; --  START OF SECOND VERTICAL BALNKING INTERVAL IN FIRST FIELD
CONSTANT VBLANK2A : INTEGER := 583; --  END OF FIRST VERTICAL BALNKING INTERVAL IN SECOND FIELD
CONSTANT VBLANK2B : INTEGER := 1124; --  START OF SECOND VERTICAL BALNKING INTERVAL IN SECOND FIELD

CONSTANT HALFLINES : INTEGER := LINES/2+1; -- LINES IN FIRST FIELD

CONSTANT LINES1 : INTEGER := ((VBLANK1B-VBLANK1A)*67/100)+VBLANK1A; -- END OF FIRST BAND OF BARS
CONSTANT LINES2 : INTEGER := LINES1+((VBLANK1B-VBLANK1A)*8/100); -- END OF SECOND BAND OF BARS
CONSTANT b :integer :=ASL/4/7*4;
CONSTANT BD3 : INTEGER := b/3;
CONSTANT BD3T2 : INTEGER := b*2/3;
CONSTANT BD4 : INTEGER := b/4;
CONSTANT BD4T5 : INTEGER := b*5/4;
CONSTANT BD4T10 : INTEGER := b*10/4;
CONSTANT BD4T15 : INTEGER := b*15/4;
CONSTANT BT2 : INTEGER := b*2;
CONSTANT BT3 : INTEGER := b*3;
CONSTANT BT4 : INTEGER := b*4;
CONSTANT BT5 : INTEGER := b*5;
CONSTANT BT6 : INTEGER := b*6;

BEGIN
Y_crc0 <= crc_0_4_5_18(Y_data_out,Y_crc);
C_crc0 <= crc_0_4_5_18(C_data_out,C_crc);
process (enable,clk) begin
	if enable = '0' then
		samplecount <= conv_std_logic_vector(ASL,12);
		linecount <= conv_std_logic_vector(1,11);
	elsif clk'event and clk = '1' then
		if samplecount < (TSL-1) then
			samplecount <= samplecount+1;
			if samplecount = (ASL-1) then
				if linecount < LINES then
					linecount <= linecount + 1;
				else
					linecount <= conv_std_logic_vector(1,11);
				end if;
			end if;
		else 
			samplecount <= conv_std_logic_vector(0,12);
		end if;
	end if;
end process;

process (enable,clk) begin
	if enable = '0' then
		Y_data_out <= "0000000000";
		C_data_out <= "0000000000";
		Y_crc <= conv_std_logic_vector(0,18);
		C_crc <= conv_std_logic_vector(0,18);
	elsif clk'event and clk='1' then
		if ((samplecount <= (ASL+6)) and (samplecount > 0)) then
			Y_crc <= crc_0_4_5_18(Y_data_out,Y_crc);
			C_crc <= crc_0_4_5_18(C_data_out,C_crc);
		elsif (samplecount = (ASL+9)) then
			Y_crc <= conv_std_logic_vector(0,18);
			C_crc <= conv_std_logic_vector(0,18);
		end if;
		if samplecount = ASL or samplecount=(TSL-4)then -- TRS interval
			Y_data_out <= "1111111111";
			C_data_out <= "1111111111";
		elsif ((samplecount = ASL+1) or (samplecount = ASL+2) or (samplecount = TSL-3) or (samplecount = TSL-2)) then
			Y_data_out <= "0000000000";
			C_data_out <= "0000000000";
		elsif samplecount = ASL+3 or samplecount = TSL-1 then
			Y_data_out <= xyz;
			C_data_out <= xyz;
		ELSIF samplecount = ASL+4 then
			-- LN0 here
			Y_data_out(1 downto 0) <= "00";
			Y_data_out(8 downto 2) <= linecount(6 downto 0);
			Y_data_out(9) <= not linecount(6);
			C_data_out(1 downto 0) <= "00";
			C_data_out(8 downto 2) <= linecount(6 downto 0);
			C_data_out(9) <= not linecount(6);
		ELSIF samplecount = ASL+5 then
			-- LN1 here
			Y_data_out(1 downto 0) <= "00";
			Y_data_out(5 downto 2) <= linecount(10 downto 7);
			Y_data_out(8 downto 6) <= "000";
			Y_data_out(9) <= '1';
			C_data_out(1 downto 0) <= "00";
			C_data_out(5 downto 2) <= linecount(10 downto 7);
			C_data_out(8 downto 6) <= "000";
			C_data_out(9) <= '1';
		elsif samplecount = ASL+6 then
		-- add crc0 here
			Y_Data_out(8 downto 0) <= Y_crc0(8 downto 0);
			Y_data_out(9) <= not Y_crc0(8);
			C_Data_out(8 downto 0) <= C_crc0(8 downto 0);
			C_data_out(9) <= not C_crc0(8);
		elsif samplecount = ASL+7 then
		-- add crc1 here
			Y_Data_out(8 downto 0) <= Y_crc(17 downto 9);
			Y_data_out(9) <= not Y_crc(17);
			C_Data_out(8 downto 0) <= C_crc(17 downto 9);
			C_data_out(9) <= not C_crc(17);
		elsif samplecount > ASL+7 then	-- horizontal blanking interval
			Y_data_out <= "0001000000"; -- 0x040
			C_data_out <= "1000000000"; -- 0x200
		elsif (linecount <= VBLANK1A) or ((linecount >= VBLANK1B) and (linecount <= VBLANK2A)) or (linecount >= VBLANK2B) then -- vertical blanking interval
			Y_data_out <= "0001000000"; -- 0x040
			C_data_out <= "1000000000"; -- 0x200
		elsif (linecount <= LINES1 or (linecount > VBLANK2A and linecount <= HALFLINES+LINES1)) then  -- top band of bars
			if samplecount < b then -- first bar ( grey )
				Y_data_out <= conv_std_logic_vector(721,10);	 	--GREY_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(512,10);	    --GREY_CB
				else
					C_data_out <= conv_std_logic_vector(512,10);	    --GREY_CR
				end if;
			elsif samplecount < BT2 then -- second bar ( yellow )
				Y_data_out <= conv_std_logic_vector(674,10);	 	--YELLOW_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(176,10);	    --YELLOW_CB
				else
					C_data_out <= conv_std_logic_vector(543,10);	    --YELLOW_CR
				end if;
			elsif samplecount < BT3 then -- third bar ( cyan )
				Y_data_out <= conv_std_logic_vector(581,10);	 	--CYAN_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(589,10);	    --CYAN_CB
				else
					C_data_out <= conv_std_logic_vector(176,10);	    --CYAN_CR
				end if;
			elsif samplecount < BT4 then -- fourth bar ( green )
				Y_data_out <= conv_std_logic_vector(534,10);	 	--GREEN_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(253,10);	    --GREEN_CB
				else
					C_data_out <= conv_std_logic_vector(207,10);	    --GREEN_CR
				end if;
			elsif samplecount < BT5 then -- fifth bar ( magenta )
				Y_data_out <= conv_std_logic_vector(251,10);	 	--MAGENTA_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(771,10);	    --MAGENTA_CB
				else
					C_data_out <= conv_std_logic_vector(817,10);	    --MAGENTA_CR
				end if;
			elsif samplecount < BT6 then -- sixth bar ( red )
				Y_data_out <= conv_std_logic_vector(204,10);	 	--RED_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(435,10);	    --RED_CB
				else
					C_data_out <= conv_std_logic_vector(848,10);	    --RED_CR
				end if;
			else 						-- seventh bar ( blue )
				Y_data_out <= conv_std_logic_vector(111,10);	 	--BLUE_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(848,10);	    --BLUE_CB
				else
					C_data_out <= conv_std_logic_vector(481,10);	    --BLUE_CR
				end if;
			end if;		
		elsif linecount <= LINES2 or (linecount > HALFLINES+LINES1 and linecount <= HALFLINES+LINES2) then  -- middle band of bars
			if (samplecount < b) then -- first bar ( blue )
				Y_data_out <= conv_std_logic_vector(111,10);	 	--BLUE_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(848,10);	    --BLUE_CB
				else
					C_data_out <= conv_std_logic_vector(481,10);	    --BLUE_CR
				end if;
			elsif samplecount < BT2 then -- second bar ( black )
				Y_data_out <= conv_std_logic_vector(64,10);	 		--BLACK_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(512,10);	    --BLACK_CB
				else
					C_data_out <= conv_std_logic_vector(512,10);	    --BLACK_CR
				end if;
			elsif samplecount < BT3 then -- third bar ( magenta )
				Y_data_out <= conv_std_logic_vector(251,10);	 	--MAGENTA_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(771,10);	    --MAGENTA_CB
				else
					C_data_out <= conv_std_logic_vector(817,10);	    --MAGENTA_CR
				end if;
			elsif samplecount < BT4 then -- fourth bar ( black )
				Y_data_out <= conv_std_logic_vector(64,10);	 		--BLACK_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(512,10);	    --BLACK_CB
				else
					C_data_out <= conv_std_logic_vector(512,10);	    --BLACK_CR
				end if;
			elsif samplecount < BT5 then -- fifth bar ( cyan )
				Y_data_out <= conv_std_logic_vector(581,10);	 	--CYAN_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(589,10);	    --CYAN_CB
				else
					C_data_out <= conv_std_logic_vector(176,10);	    --CYAN_CR
				end if;
			elsif samplecount < BT6 then -- sixth bar ( black )
				Y_data_out <= conv_std_logic_vector(64,10);	 		--BLACK_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(512,10);	    --BLACK_CB
				else
					C_data_out <= conv_std_logic_vector(512,10);	    --BLACK_CR
				end if;
			else 						-- seventh bar ( grey )
				Y_data_out <= conv_std_logic_vector(721,10);	 	--GREY_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(512,10);	    --GREY_CB
				else
					C_data_out <= conv_std_logic_vector(512,10);	    --GREY_CR
				end if;
			end if;
		else	-- bottom band of bars
			if samplecount < (BD4T5) then -- -I
				Y_data_out <= conv_std_logic_vector(244,10);	 	--I_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(612,10);	    --I_CB
				else
					C_data_out <= conv_std_logic_vector(395,10);	    --I_CR
				end if;
			elsif samplecount < (BD4T10) then -- 100% White
				Y_data_out <= conv_std_logic_vector(940,10);	 	--WHITE_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(512,10);	    --WHITE_CB
				else
					C_data_out <= conv_std_logic_vector(512,10);	    --WHITE_CR
				end if;
			elsif samplecount < (BD4T15) then -- +Q 
				Y_data_out <= conv_std_logic_vector(141,10);	 	--Q_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(697,10);	    --Q_CB
				else
					C_data_out <= conv_std_logic_vector(606,10);	    --Q_CR
				end if;
			elsif samplecount < (BT5) then -- Black
				Y_data_out <= conv_std_logic_vector(64,10);	 		--BLACK_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(512,10);	    --BLACK_CB
				else
					C_data_out <= conv_std_logic_vector(512,10);	    --BLACK_CR
				end if;
			elsif samplecount < (BT5+BD3) then -- Black - 4%
				Y_data_out <= conv_std_logic_vector(29,10);	 		--BLACK-4_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(512,10);	    --BLACK-4_CB
				else
					C_data_out <= conv_std_logic_vector(512,10);	    --BLACK-4_CR
				end if;
			elsif samplecount < (BT5+BD3T2) then -- Black 
				Y_data_out <= conv_std_logic_vector(64,10);	 		--BLACK_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(512,10);	    --BLACK_CB
				else
					C_data_out <= conv_std_logic_vector(512,10);	    --BLACK_CR
				end if;
			elsif samplecount < (BT6) then -- Black + 4%
				Y_data_out <= conv_std_logic_vector(99,10);	 	--BLACK+4_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(512,10);	    --BLACK+4_CB
				else
					C_data_out <= conv_std_logic_vector(512,10);	    --BLACK+4_CR
				end if;
			else						-- Black
				Y_data_out <= conv_std_logic_vector(64,10);	 		--BLACK_Y
				if samplecount(0) = '0' then
					C_data_out <= conv_std_logic_vector(512,10);	    --BLACK_CB
				elsif samplecount(1 downto 0) = "10" then
					C_data_out <= conv_std_logic_vector(512,10);	    --BLACK_CR
				else
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
		if (linecount <= VBLANK1A) or ((linecount >= VBLANK1B) and (linecount <= VBLANK2A)) or (linecount >= VBLANK2B) then -- vertical blanking interval
			V <= '1';
		else
			V <= '0';
		end if;

		if linecount <= HALFLINES then
			F <= '0';
		else
			F <= '1';
		end if;

		if samplecount > ASL+3 then 
			H <= '0';
		else
			H <= '1';
		end if;
	end if;
end process;

process
begin
	wait until clk = '1';
	Y_data <= Y_data_out;
	C_data <= C_data_out;
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

END a_eg1_292; 
