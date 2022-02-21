-- rp198.vhd
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
-- This module generates rp198 test pattern for smpte 292 
-- 1080i at 30 frames/sec is supported
--
use work.pack_reverse_bit_order.all;
library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;
USE work.pack_crc.all;

ENTITY rp198 IS
	PORT ( 
		clk,enable,
		std_altn	 : in std_logic;	-- '1' = standard distribution, '0' = alternate distribution
--		sel : in std_logic_vector(1 downto 0);
		Y_data : out std_logic_vector(9 downto 0);
		C_data : out std_logic_vector(9 downto 0)
	);
END rp198;

ARCHITECTURE a_rp198 OF rp198 IS

signal Y_data_out, C_data_out : std_logic_vector(9 downto 0);
signal linecount : std_logic_vector(10 downto 0);
signal samplecount : std_logic_vector(11 downto 0);
signal xyz : std_logic_vector(9 downto 0);
signal F,V,H : std_logic;
signal P : std_logic_vector(3 downto 0);
signal Y_crc, C_crc : std_logic_vector(17 downto 0);
signal Y_crc0, C_crc0 : std_logic_vector(17 downto 0);
--signal TSL, ASL, LINES, VBLANK, HALFLINES, LINES1 : INTEGER;
-- SMPTE 292 Values for 1080i at 30 fps
CONSTANT TSL :integer :=2200; -- TOTAL SAMPLES PER LINE
CONSTANT ASL :integer :=1920; -- ACTIVE SAMPLES PER LINE
CONSTANT LINES : INTEGER := 1125; -- TOTAL LINES PER FRAME
CONSTANT VBLANK1A : INTEGER := 20; --  END OF FIRST VERTICAL BALNKING INTERVAL IN FIRST FIELD
CONSTANT VBLANK1B : INTEGER := 561; --  START OF SECOND VERTICAL BALNKING INTERVAL IN FIRST FIELD
CONSTANT VBLANK2A : INTEGER := 583; --  END OF FIRST VERTICAL BALNKING INTERVAL IN SECOND FIELD
CONSTANT VBLANK2B : INTEGER := 1124; --  START OF SECOND VERTICAL BALNKING INTERVAL IN SECOND FIELD

CONSTANT HALFLINES : INTEGER := LINES/2+1; -- LINES IN FIRST FIELD
CONSTANT LINES1 : INTEGER := ((VBLANK1B-VBLANK1A)*50/100)+VBLANK1A; -- END OF FIRST HALF OF SCREEN

BEGIN
Y_crc0 <= crc_0_4_5_18(Y_data_out,Y_crc);
C_crc0 <= crc_0_4_5_18(C_data_out,C_crc);
process (enable,clk) begin
	if enable = '0' then
		samplecount <= conv_std_logic_vector(ASL,12);--conv_std_logic_vector(1920,12);
		linecount <= conv_std_logic_vector(1,11);
	elsif clk'event and clk = '1' then
		if samplecount < (TSL-1) then--TSL-1 then
			samplecount <= samplecount+1;
			if samplecount = (ASL-1) then--1439 then
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
		if samplecount = ASL or samplecount=(TSL-4)then --1712 then	-- TRS interval
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
			if linecount=VBLANK1A+1 and samplecount = 0 then
				if std_altn='1' then
					Y_data_out <= conv_std_logic_vector(400,10);	-- firs active Y sample in line 21 = 0x190(400)
				else
					Y_data_out <= conv_std_logic_vector(512,10);	-- firs active Y sample in line 21 = 0x190(400)
				end if;
 	    	else  
				if std_altn='1' then
					Y_data_out <= conv_std_logic_vector(408,10); -- 0x198(498) Y
					C_data_out <= conv_std_logic_vector(768,10); -- 0x300 Cb or Cr
				else
					Y_data_out <= conv_std_logic_vector(768,10); -- 0x198(498) Y
					C_data_out <= conv_std_logic_vector(408,10); -- 0x300 Cb or Cr
				end if;
			end if;
		else -- last half active video field
			if std_altn='1' then
				Y_data_out <= conv_std_logic_vector(272,10); -- 0x110(272) Y
				C_data_out <= conv_std_logic_vector(512,10); -- 0x200(512) Cb or Cr
			else
				Y_data_out <= conv_std_logic_vector(512,10); -- 0x110(272) Y
				C_data_out <= conv_std_logic_vector(272,10); -- 0x200(512) Cb or Cr
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

END a_rp198; 

