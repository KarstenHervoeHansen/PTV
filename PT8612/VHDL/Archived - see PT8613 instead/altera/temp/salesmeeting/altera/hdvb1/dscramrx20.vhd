-- Copyright 2003-2004, Cypress Semiconducdowntor Corporation

-- This SOFTWARE is owned by Cypress Semiconductor Corporation
-- (Cypress) and is protected by United States copyright laws and 
-- international treaty provisions.  Therefore, you must treat this 
-- SOFTWARE like any other copyrighted material (e.g., book, or musical 
-- recording), with the exception that one copy may be made for personal 
-- use or evaluation.  Reproduction, modification, translation, 
-- compilation, or representation of this software in any other form 
-- (e.g., paper, magnetic, optical, silicon, etc.) is prohibited 
-- without the express written permission of Cypress.  

-- This SOFTWARE is protected by and subject to worldwide patent
-- coverage, including U.S. and foreign patents. Use is limited by
-- and subject to the Cypress Software License Agreement.

-- SMPTE 292 20-bit wide Descrambler/Framer Design

-- This design combines an NRZI decoder, SMPTE descrabler, and parallel
-- framer.

-- The NRZI decoder works just the opposite of the encoder.  Instead
-- of using a feedback process, this uses a feedforward process to
-- remove the effect of the encoder.  This should allow the decoder
-- to be implemented with only a minimal number of XOR terms; one
-- per bit implemented in parallel.

-- in reality, all that is needed is 21 bits of data to decode
-- 20 bits in a single clock.  This can be done by keeping one
-- bit from the previous clock cycle to extend the register, 
-- then decoding q0-q19 from the 21 bit register

-- ***********************************************************!!!!
-- When accepting data from the HOTLink RX, bit Q(a)is the LSB
-- of the received word and is received first!!!!  The descramber
-- requires that the bits be routed through the circuits LSB first!!!!
-- ***********************************************************!!!!

-- The descrambler uses a feed-forward form of the x^9 + X^4 + 1 
-- SMPTE scrambler algorithm to unscramble the data stream.

----------------------------------------------------------------------------------------------------------
--
-- Revision History:
--	3-25-04	Original Draft
----------------------------------------------------------------------------------------------------------

 
library ieee;
use ieee.std_logic_1164.all;

entity dscram20 is 
    port (
        rxclk,                      -- CY7B933 CKR recovered clock
        reset,                      -- active high reset added by DEF 11-6-03
        bypass: in std_logic;       -- raw data mode, bypass scrambler data
        sync_en: in std_logic;      -- TRS/sync correction enabled
        rxd_in: in std_logic_vector (0 to 19);
                                    -- scrambled data from HOTLink RX
        rxd_out: out std_logic_vector (0 to 19);
                                    -- descrambled and framed data
        sync_err: out std_logic;    -- sync offset error pulse
        h_sync: out std_logic);     -- horizontal sync output toggle
end dscram20;

architecture structural of dscram20 is
signal in_reg: std_logic_vector (0 to 20);       -- NRZI/input register
signal nrz_reg: std_logic_vector (0 to 19);      -- NRZ pipeline register
signal din: std_logic_vector (0 to 19);          -- descrambler equations
signal d0: std_logic_vector (0 to 19);           -- low order descrambler register
signal d1: std_logic_vector (1 to 19);           -- high order descrambler register
signal p1: std_logic_vector (0 to 19);       -- first  pipeline stage
signal p2: std_logic_vector (0 to 19);       -- second pipeline stage
signal p3: std_logic_vector (0 to 19);       -- third  pipeline stage
signal p4: std_logic_vector (0 to 19);       -- fourth pipeline stage
signal p5: std_logic_vector (0 to 19);       -- fifth  pipeline stage
signal p6: std_logic_vector (0 to 19);       -- fifth  pipeline stage
signal dout: std_logic_vector (0 to 19);     -- output data register
signal sync: std_logic;                          -- horizontal sync toggle
-- This signal changes state one clock prior to the first word of
--  the TRS being presented at the outputs.

signal p2_all0 : std_logic;  -- indicator for when pipe 2 is all zeros
signal offset_detect1  : std_logic_vector (19 downto 0);  -- 19-bit offset pointer, 1st stage
signal offset_previous : std_logic_vector (19 downto 0);  -- 19-bit offset pointer, 1st stage
signal offset_detect2  : std_logic_vector (4 downto 0);  -- 5-bit offset pointer, 2nd stage


begin

-- First declare the input register.  This is a 21 bit register
proc1: process(reset,rxclk) 
begin
	if (reset = '1') then
		in_reg <= (others => '0');
	elsif rising_edge(rxclk) then
	-- bit 9 is the first bit received by the HOTLink receiver
		in_reg(0) <= in_reg(20);    -- extend to an 11 bit register
		in_reg(1 TO 20) <= rxd_in;    -- capture input bus
	end if;
END process proc1;

----------------------------------------------------------------------
----------------------------------------------------------------------
-- Next item is the NRZI to NRZ decoder.  Since 11 bits of data are
-- present in the input register, it is possible to decode a full
-- 10 bits in one clock
proc2: process(reset,rxclk) 
begin
    if (reset = '1') then
        nrz_reg <= (others => '0');
    elsif rising_edge(rxclk) then
        nrz_reg(19) <= in_reg(20) XOR in_reg(19); -- MSB
        nrz_reg(18) <= in_reg(19) XOR in_reg(18);
        nrz_reg(17) <= in_reg(18) XOR in_reg(17);
        nrz_reg(16) <= in_reg(17) XOR in_reg(16);
        nrz_reg(15) <= in_reg(16) XOR in_reg(15);
        nrz_reg(14) <= in_reg(15) XOR in_reg(14);
        nrz_reg(13) <= in_reg(14) XOR in_reg(13);
        nrz_reg(12) <= in_reg(13) XOR in_reg(12);
        nrz_reg(11) <= in_reg(12) XOR in_reg(11);
        nrz_reg(10) <= in_reg(11) XOR in_reg(10);
        nrz_reg(9) <= in_reg(10) XOR in_reg(9);
        nrz_reg(8) <= in_reg(9) XOR in_reg(8);
        nrz_reg(7) <= in_reg(8) XOR in_reg(7);
        nrz_reg(6) <= in_reg(7) XOR in_reg(6);
        nrz_reg(5) <= in_reg(6) XOR in_reg(5);
        nrz_reg(4) <= in_reg(5) XOR in_reg(4);
        nrz_reg(3) <= in_reg(4) XOR in_reg(3);
        nrz_reg(2) <= in_reg(3) XOR in_reg(2);
        nrz_reg(1) <= in_reg(2) XOR in_reg(1);
        nrz_reg(0) <= in_reg(1) XOR in_reg(0);  -- LSB
	end if;
END process proc2;

----------------------------------------------------------------------
----------------------------------------------------------------------
-- Next comes the descrambler.  This descrambler uses the 
-- same X^9 + x^4 + 1 polynomial as the scrambler.  These equations 
-- are assigned to the respective bits of register desc.

-- descrambler equations
	din(19) <= nrz_reg(19) xor nrz_reg(14) xor nrz_reg(10); -- MSB
	din(18) <= nrz_reg(18) xor nrz_reg(13) xor nrz_reg(9);
	din(17) <= nrz_reg(17) xor nrz_reg(12) xor nrz_reg(8);
	din(16) <= nrz_reg(16) xor nrz_reg(11) xor nrz_reg(7);
	din(15) <= nrz_reg(15) xor nrz_reg(10) xor nrz_reg(6);
	din(14) <= nrz_reg(14) xor nrz_reg(9) xor nrz_reg(5);
	din(13) <= nrz_reg(13) xor nrz_reg(8) xor nrz_reg(4);
	din(12) <= nrz_reg(12) xor nrz_reg(7) xor nrz_reg(3);
	din(11) <= nrz_reg(11) xor nrz_reg(6) xor nrz_reg(2);
	din(10) <= nrz_reg(10) xor nrz_reg(5) xor nrz_reg(1);
	din(9) <= nrz_reg(9) xor nrz_reg(4) xor nrz_reg(0);
	din(8) <= nrz_reg(8) xor nrz_reg(3) xor d0(19);
	din(7) <= nrz_reg(7) xor nrz_reg(2) xor d0(18);
	din(6) <= nrz_reg(6) xor nrz_reg(1) xor d0(17);
	din(5) <= nrz_reg(5) xor nrz_reg(0) xor d0(16);
	din(4) <= nrz_reg(4) xor d0(19) xor d0(15);
	din(3) <= nrz_reg(3) xor d0(18) xor d0(14);
	din(2) <= nrz_reg(2) xor d0(17) xor d0(13);
	din(1) <= nrz_reg(1) xor d0(16) xor d0(12);
	din(0) <= nrz_reg(0) xor d0(15) xor d0(11); -- LSB


dataIntFF: process(reset,rxclk) 
begin
    -- setup descrambler bypass register
	if (reset = '1') then
		d0 <= (others => '0');
		d1 <= (others => '0');
	elsif rising_edge(rxclk) then
	IF bypass = '1' THEN    -- if bypass is active
            -- then route data around the NRZI decoder and descrambler
            d0(0)       <= in_reg(20);
            d1(1 TO 19) <= in_reg(1 TO 19); 

	ELSE
	    -- on rising CKR clock capture data from NRZI decoder into
	    -- the first register of the descrambler
	        -- descramble data
		d0(19) <= nrz_reg(19);    
		d0(18) <= nrz_reg(18);
		d0(17) <= nrz_reg(17);      
		d0(16) <= nrz_reg(16);    
		d0(15) <= nrz_reg(15);
		d0(14) <= nrz_reg(14);      
		d0(13) <= nrz_reg(13);    
		d0(12) <= nrz_reg(12);
		d0(11) <= nrz_reg(11);
		d0(10) <= nrz_reg(10);    
		d0(9)  <= nrz_reg(9);    
		d0(8)  <= nrz_reg(8);
		d0(7)  <= nrz_reg(7);      
		d0(6)  <= nrz_reg(6);    
		d0(5)  <= nrz_reg(5);
		d0(4)  <= nrz_reg(4);      
		d0(3)  <= nrz_reg(3);    
		d0(2)  <= nrz_reg(2);
		d0(1)  <= nrz_reg(1);
		d0(0)  <= din(19);    -- MSB      
		d1(19) <= din(18);        
		d1(18) <= din(17);        
		d1(17) <= din(16);
		d1(16) <= din(15);        
		d1(15) <= din(14);        
		d1(14) <= din(13);
		d1(13) <= din(12);        
		d1(12) <= din(11);        
		d1(11) <= din(10);
		d1(10) <= din(9);        
		d1(9)  <= din(8);        
		d1(8)  <= din(7);        
		d1(7)  <= din(6);
		d1(6)  <= din(5);        
		d1(5)  <= din(4);        
 		d1(4)  <= din(3);
		d1(3)  <= din(2);        
		d1(2)  <= din(1);        
		d1(1)  <= din(0);    -- LSB
	END IF;
end if;
END process dataIntFF;


-- rename outputs from scrambler registers.  This is the first stage of
-- fully descrambled data.  It has not yet been framed.
p1(19) <= d0(0);
p1(0 TO 18) <= d1(1 TO 19);

----------------------------------------------------------------------
----------------------------------------------------------------------
-- add in pipeline registers to allow data to be examined and shifted

pipes: process(reset,rxclk) 
begin
	if (reset = '1') then
		p2   <= (others => '0');
		p3   <= (others => '0');
		p4   <= (others => '0');
		p5   <= (others => '0');
		p6   <= (others => '0');
	elsif rising_edge(rxclk) then
		p2 <= p1;
		p3 <= p2;
		p4 <= p3;
		p5 <= p4;    
		p6 <= p5;    
	end if;
END process pipes;


----------------------------------------------------------------------
----------------------------------------------------------------------
-- next step is the framer.  It must allow a frame operation on 
-- every clock cycle.  The first thing to look for is the first '01'
-- combination in register p1/p2. 

-- The parallel framer will be looking for the 60 bit
-- sequence of bits FFFFFh, 00000h, 00000h.  This pattern can arrive
-- at the framer in any of 20 possible framing offsets,
-- from 0 to 19 bits.  This allows a 5-bit field to be used
-- to contain the framer offset field.  This field can then
-- be used to control a barrel shifter connected to a
-- later pipeline stage in the design, just
-- prior to the output register.

-- These registers could contain the data in the following 
-- positions:

--register:      p1                     p2                     p3                     p4
--
--BITS 19         9876543210	 19         9876543210	 19         9876543210	 19         9876543210			offset	
--      xxxxxxxxxxxxxxxxxxxx	  00000000000000000000   00000000000000000000	  11111111111111111111	=	0	bits	offset
--      xxxxxxxxxxxxxxxxxxx0   00000000000000000000   00000000000000000001   1111111111111111111x	=	19	bits	offset
--      xxxxxxxxxxxxxxxxxx00   00000000000000000000   00000000000000000011   111111111111111111xx	=	18	bits	offset
--      xxxxxxxxxxxxxxxxx000   00000000000000000000   00000000000000000111   11111111111111111xxx	=	17	bits	offset
--      xxxxxxxxxxxxxxxx0000   00000000000000000000   00000000000000001111   1111111111111111xxxx	=	16	bits	offset
--      xxxxxxxxxxxxxxx00000   00000000000000000000   00000000000000011111   111111111111111xxxxx	=	15	bits	offset
--      xxxxxxxxxxxxxx000000   00000000000000000000   00000000000000111111   11111111111111xxxxxx	=	14	bits	offset
--      xxxxxxxxxxxxx0000000   00000000000000000000   00000000000001111111   1111111111111xxxxxxx	=	13	bits	offset
--      xxxxxxxxxxxx00000000   00000000000000000000   00000000000011111111   111111111111xxxxxxxx	=	12	bits	offset
--      xxxxxxxxxxx000000000   00000000000000000000   00000000000111111111   11111111111xxxxxxxxx	=	11	bits	offset
--      xxxxxxxxxx0000000000   00000000000000000000   00000000001111111111   1111111111xxxxxxxxxx	=	10	bits	offset
--      xxxxxxxxx00000000000   00000000000000000000   00000000011111111111   111111111xxxxxxxxxxx	=	9	bits	offset
--      xxxxxxxx000000000000   00000000000000000000   00000000111111111111   11111111xxxxxxxxxxxx	=	8	bits	offset
--      xxxxxxx0000000000000   00000000000000000000   00000001111111111111   1111111xxxxxxxxxxxxx	=	7	bits	offset
--      xxxxxx00000000000000   00000000000000000000   00000011111111111111   111111xxxxxxxxxxxxxx	=	6	bits	offset
--      xxxxx000000000000000   00000000000000000000   00000111111111111111   11111xxxxxxxxxxxxxxx	=	5	bits	offset
--      xxxx0000000000000000   00000000000000000000   00001111111111111111   1111xxxxxxxxxxxxxxxx	=	4	bits	offset
--      xxx00000000000000000   00000000000000000000   00011111111111111111   111xxxxxxxxxxxxxxxxx	=	3	bits	offset
--      xx000000000000000000   00000000000000000000   00111111111111111111   11xxxxxxxxxxxxxxxxxx	=	2	bits	offset
--      x0000000000000000000   00000000000000000000   01111111111111111111   1xxxxxxxxxxxxxxxxxxx	=	1	bit	offset


-- Note that all possible combinations contain a check for all 0's
-- in the T3 time slot.

p2_all0 <= '1' when (p2 = "00000000000000000000") else '0';   

barrel_shift: process(reset,rxclk) 
begin
	if (reset = '1') then
		offset_detect1  <= (others => '0');
		offset_detect2  <= (others => '0');
		offset_previous <= (others => '0');
		dout            <= (others => '0');
		sync_err        <= '0';
		sync            <= '0';
	elsif rising_edge(rxclk) then
		-- Note that the pipe registers are a (X to Y) format instead of (X downto Y)
		-- This means that if p1 = 11111111111111111110, then p1(19) = 0, and p1(0) = 1
		-- The table above will seem to be reversed in the logic below, but the bit positions are preserved
				
		-- *** NO offset ***
		if ((p2_all0 = '1') and (p3 = "00000000000000000000") and (p4 = "11111111111111111111")) then
			offset_detect1(0) <= '1';   else   offset_detect1(0) <= '0'; end if;

		-- offset = 19
		if ((p1(0) = '0') and (p2_all0 = '1') and (p3 = "10000000000000000000") and (p4(1 to 19) = "1111111111111111111")) then
			offset_detect1(19) <= '1';   else   offset_detect1(19) <= '0'; end if;

		-- offset = 18
		if ((p1(0 to 1) = "00") and (p2_all0 = '1') and (p3 = "11000000000000000000") and (p4(2 to 19) = "111111111111111111")) then
			offset_detect1(18) <= '1';   else   offset_detect1(18) <= '0'; end if;

		-- offset = 17
		if ((p1(0 to 2) = "000") and (p2_all0 = '1') and (p3 = "11100000000000000000") and (p4(3 to 19) = "11111111111111111")) then
			offset_detect1(17) <= '1';   else   offset_detect1(17) <= '0'; end if;

		-- offset = 16
		if ((p1(0 to 3) = "0000") and (p2_all0 = '1') and (p3 = "11110000000000000000") and (p4(4 to 19) = "1111111111111111")) then
			offset_detect1(16) <= '1';   else   offset_detect1(16) <= '0'; end if;

		-- offset = 15
		if ((p1(0 to 4) = "00000") and (p2_all0 = '1') and (p3 = "11111000000000000000") and (p4(5 to 19) = "111111111111111")) then
			offset_detect1(15) <= '1';   else   offset_detect1(15) <= '0'; end if;

		-- offset = 14
		if ((p1(0 to 5) = "000000") and (p2_all0 = '1') and (p3 = "11111100000000000000") and (p4(6 to 19) = "11111111111111")) then
			offset_detect1(14) <= '1';   else   offset_detect1(14) <= '0'; end if;

		-- offset = 13
		if ((p1(0 to 6) = "0000000") and (p2_all0 = '1') and (p3 = "11111110000000000000") and (p4(7 to 19) = "1111111111111")) then
			offset_detect1(13) <= '1';   else   offset_detect1(13) <= '0'; end if;

		-- offset = 12
		if ((p1(0 to 7) = "00000000") and (p2_all0 = '1') and (p3 = "11111111000000000000") and (p4(8 to 19) = "111111111111")) then
			offset_detect1(12) <= '1';   else   offset_detect1(12) <= '0'; end if;

		-- offset = 11
		if ((p1(0 to 8) = "000000000") and (p2_all0 = '1') and (p3 = "11111111100000000000") and (p4(9 to 19) = "11111111111")) then
			offset_detect1(11) <= '1';   else   offset_detect1(11) <= '0'; end if;

		-- offset = 10
		if ((p1(0 to 9) = "0000000000") and (p2_all0 = '1') and (p3 = "11111111110000000000") and (p4(10 to 19) = "1111111111")) then
			offset_detect1(10) <= '1';   else   offset_detect1(10) <= '0'; end if;

		-- offset = 9
		if ((p1(0 to 10) = "00000000000") and (p2_all0 = '1') and (p3 = "11111111111000000000") and (p4(11 to 19) = "111111111")) then
			offset_detect1(9) <= '1';   else   offset_detect1(9) <= '0'; end if;

		-- offset = 8
		if ((p1(0 to 11) = "000000000000") and (p2_all0 = '1') and (p3 = "11111111111100000000") and (p4(12 to 19) = "11111111")) then
			offset_detect1(8) <= '1';   else   offset_detect1(8) <= '0'; end if;

		-- offset = 7
		if ((p1(0 to 12) = "0000000000000") and (p2_all0 = '1') and (p3 = "11111111111110000000") and (p4(13 to 19) = "1111111")) then
			offset_detect1(7) <= '1';   else   offset_detect1(7) <= '0'; end if;

		-- offset = 6
		if ((p1(0 to 13) = "00000000000000") and (p2_all0 = '1') and (p3 = "11111111111111000000") and (p4(14 to 19) = "111111")) then
			offset_detect1(6) <= '1';   else   offset_detect1(6) <= '0'; end if;

		-- offset = 5
		if ((p1(0 to 14) = "000000000000000") and (p2_all0 = '1') and (p3 = "11111111111111100000") and (p4(15 to 19) = "11111")) then
			offset_detect1(5) <= '1';   else   offset_detect1(5) <= '0'; end if;

		-- offset = 4
		if ((p1(0 to 15) = "0000000000000000") and (p2_all0 = '1') and (p3 = "11111111111111110000") and (p4(16 to 19) = "1111")) then
			offset_detect1(4) <= '1';   else   offset_detect1(4) <= '0'; end if;

		-- offset = 3
		if ((p1(0 to 16) = "00000000000000000") and (p2_all0 = '1') and (p3 = "11111111111111111000") and (p4(17 to 19) = "111")) then
			offset_detect1(3) <= '1';   else   offset_detect1(3) <= '0'; end if;

		-- offset = 2
		if ((p1(0 to 17) = "000000000000000000") and (p2_all0 = '1') and (p3 = "11111111111111111100") and (p4(18 to 19) = "11")) then
			offset_detect1(2) <= '1';   else   offset_detect1(2) <= '0'; end if;

		-- offset = 1
		if ((p1(0 to 18) = "0000000000000000000") and (p2_all0 = '1') and (p3 = "11111111111111111110") and (p4(19) = '1')) then
			offset_detect1(1) <= '1';   else   offset_detect1(1) <= '0'; end if;
		
		
		if (offset_detect1 /= "00000000000000000000") then
			offset_previous <= offset_detect1;
		end if;


		if (offset_detect1 /= "00000000000000000000") then
		case offset_detect1 is
			when "00000000000000000001" => offset_detect2 <= "10100"; -- offset 0, but TRS still detected
			when "10000000000000000000" => offset_detect2 <= "10011"; -- offset 19
			when "01000000000000000000" => offset_detect2 <= "10010"; -- offset 18
			when "00100000000000000000" => offset_detect2 <= "10001"; -- offset 17
			when "00010000000000000000" => offset_detect2 <= "10000"; -- offset 16
			when "00001000000000000000" => offset_detect2 <= "01111"; -- offset 15
			when "00000100000000000000" => offset_detect2 <= "01110"; -- offset 14
			when "00000010000000000000" => offset_detect2 <= "01101"; -- offset 13
			when "00000001000000000000" => offset_detect2 <= "01100"; -- offset 12
			when "00000000100000000000" => offset_detect2 <= "01011"; -- offset 11
			when "00000000010000000000" => offset_detect2 <= "01010"; -- offset 10
			when "00000000001000000000" => offset_detect2 <= "01001"; -- offset 9
			when "00000000000100000000" => offset_detect2 <= "01000"; -- offset 8
			when "00000000000010000000" => offset_detect2 <= "00111"; -- offset 7
			when "00000000000001000000" => offset_detect2 <= "00110"; -- offset 6
			when "00000000000000100000" => offset_detect2 <= "00101"; -- offset 5
			when "00000000000000010000" => offset_detect2 <= "00100"; -- offset 4
			when "00000000000000001000" => offset_detect2 <= "00011"; -- offset 3
			when "00000000000000000100" => offset_detect2 <= "00010"; -- offset 2
			when "00000000000000000010" => offset_detect2 <= "00001"; -- offset 1
			when others => offset_detect2 <= "00000"; -- no offset, no TRS detected
		end case;
		end if;
		
		
		if (offset_detect1 /= "00000000000000000000") then -- offset TRS detected
			if ((sync_en = '1') and (offset_detect1 /= offset_previous)) then 
				-- is offset different than previous offset ??
		                sync_err <= '1';    -- set sync error flag
			else
		                sync_err <= '0';
			end if;
			sync <= not sync;
		else
	            sync_err  <= '0';        -- clear sync error pulse
		end if;


		case offset_detect2 is
			when "10100" => dout <= p6; -- offset 0, but TRS still detected
			when "10011" => dout <= p6(1 to 19) & p5(0) ; -- offset 19
			when "10010" => dout <= p6(2 to 19) & p5(0 to 1); -- offset 18
			when "10001" => dout <= p6(3 to 19) & p5(0 to 2); -- offset 17
			when "10000" => dout <= p6(4 to 19) & p5(0 to 3); -- offset 16
			when "01111" => dout <= p6(5 to 19) & p5(0 to 4); -- offset 15
			when "01110" => dout <= p6(6 to 19) & p5(0 to 5); -- offset 14
			when "01101" => dout <= p6(7 to 19) & p5(0 to 6); -- offset 13
			when "01100" => dout <= p6(8 to 19) & p5(0 to 7); -- offset 12
			when "01011" => dout <= p6(9 to 19) & p5(0 to 8); -- offset 11
			when "01010" => dout <= p6(10 to 19) & p5(0 to 9); -- offset 10
			when "01001" => dout <= p6(11 to 19) & p5(0 to 10); -- offset 9
			when "01000" => dout <= p6(12 to 19) & p5(0 to 11); -- offset 8
			when "00111" => dout <= p6(13 to 19) & p5(0 to 12); -- offset 7
			when "00110" => dout <= p6(14 to 19) & p5(0 to 13); -- offset 6
			when "00101" => dout <= p6(15 to 19) & p5(0 to 14); -- offset 5
			when "00100" => dout <= p6(16 to 19) & p5(0 to 15); -- offset 4
			when "00011" => dout <= p6(17 to 19) & p5(0 to 16); -- offset 3
			when "00010" => dout <= p6(18 to 19) & p5(0 to 17); -- offset 2
			when "00001" => dout <= p6(19) & p5(0 to 18); -- offset 1
			when others  => dout <= p6; -- no offset no TRS detected
		end case;
	end if;
end process barrel_shift;

rxd_out <= dout;
h_sync <= sync;

end structural;