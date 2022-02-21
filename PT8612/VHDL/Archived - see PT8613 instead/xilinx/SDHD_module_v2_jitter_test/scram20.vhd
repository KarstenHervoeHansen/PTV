-- Copyright 2003-2004, Cypress Semiconductor Corporation

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

-- SMPTE 292 20-bit wide Scrambler Design

-- This design takes a 20 bit parallel data stream and
-- encodes it for serialization using the SMPTE scrambler algorithm.
-- This includes the x^9 + x^4 + 1 scrambler and the x + 1 NRZI
-- encoder.

----------------------------------------------------------------------------------------------------------
--
-- Revision History:
--	3-25-04	Original Draft
----------------------------------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY scram20 IS
    PORT (
        txclk			: in std_logic;                     -- TX clock
		  enable_sync  : in std_logic;
        zreset_i 		: in std_logic;
        bypass			: in std_logic;       					-- raw data mode - bypass scrambler
        txd_in			: in std_logic_vector(0 to 19);
        txd_out		: out std_logic_vector(0 to 19)); 	-- parallel scrambled output
                                    
END scram20;


ARCHITECTURE archscram of scram20 IS

SIGNAL  bypassl  	: std_logic;      				 -- latched bypass
SIGNAL  A       	: std_logic_vector(0 TO 19);   -- input data register
SIGNAL  B       	: std_logic_vector(0 TO 19);   -- TRS filter register
SIGNAL  S     		: std_logic_vector(0 TO 19);   -- scrambler register
SIGNAL  So      	: std_logic_vector(0 TO 19);   -- output data of scrambler
SIGNAL  dout      : std_logic_vector(0 TO 19);   -- output register
SIGNAL  N2       	: std_logic_vector(0 TO 19);   -- intermediate NRZI XOR reg
SIGNAL  N       	: std_logic_vector(0 TO 19);   -- intermediate NRZI XOR reg

signal B_delayed_1   : std_logic_vector(0 TO 19);
signal B_delayed_2   : std_logic_vector(0 TO 19);
signal B_delayed_3   : std_logic_vector(0 TO 19);


BEGIN

--------------------------------------------------------------------
--------------------------------------------------------------------
-- Declare the input holding register.  This register accepts raw 
-- 20-bit data on each rising edge of the clock.

-- The two sets of signal inputs for this register are:
--      SMPTE       
--      -------     
--      data(19)     (MSB) of upper 10 and overall
--      data(18)    
--      data(17)    
--      data(16)    
--      data(15)    
--      data(14)    
--      data(13)    
--      data(12)    
--      data(11)   
--      data(10)    (LSB) of upper 10
--      data(9)     (MSB) of lower 10
--      data(8)     
--      data(7)     
--      data(6)     
--      data(5)    
--      data(4)     
--      data(3)     
--      data(2)     
--      data(1)     
--      data(0)     (LSB) of lower 10 and overall

-- Per the SMPTE-259M spec, the LSB of any word is always 
-- transmited first.  This means bit-0 of the input register is the
-- "first" bit routed through the scrambler and NRZI encoder.
     
RegA: PROCESS(zreset_i, txclk) 
BEGIN
	if (zreset_i = '0') then
        A     <= (others => '0');
        bypassl   <= '0';
   elsif rising_edge(txclk) then -- wait for clock
    -- capture input data word and control signals at rising edge of CKW
        if enable_sync = '1' then
		  		A     <= txd_in;
        		bypassl   <= bypass;
			end if;
    end if;
END PROCESS RegA;


-- bits 0 and 1 do require filtering
filter: PROCESS(A) 
BEGIN
		  B(0 TO 9)   <= A(0 TO 9);  
        B(10 TO 19) <= A(10 TO 19); 
END PROCESS filter;
        

--------------------------------------------------------------------
--------------------------------------------------------------------
-- This process defines the operation of the scrambler register.
-- This register is enabled at every clock.  It takes the data from 
-- the filtered input register
scram_reg: PROCESS(zreset_i,txclk) 
BEGIN
	if (zreset_i = '0') then
		S <= (others => '0');
	elsif rising_edge(txclk) then
        --------------------------------------------------------------------
	-- The following equations map the functionality of the scrambler.
	-- The inp() asignments are the data inputs to the output/scrambler
	-- register.  This allows a full 10-bits of input data to be 
	-- scrambled in a single clock cycle.  Scram Equation  x^9 + x^4 + 1		

	-- these assignments define the scrambler as implemented for
	-- 20-bit parallel operation
		if enable_sync = '1' then	
			S(1) <= B(19) xor B(14) xor B(9) xor B(4) xor S(1) xor S(5) xor B(0) xor S(5) xor S(9) xor B(5) 
				xor B(0) xor S(5) xor S(9) xor S(4) xor B(10) xor B(5) xor B(0) xor S(5) xor S(9) xor S(4) 
				xor B(1) xor S(4) xor S(8);
			S(2) <= B(18) xor B(13) xor B(8) xor B(3) xor S(2) xor S(6) xor S(1) xor B(4) xor S(1) xor S(5) 
				xor B(9) xor B(4) xor S(1) xor S(5) xor B(0) xor S(5) xor S(9);
			S(3) <= B(17) xor B(12) xor B(7) xor B(2) xor S(3) xor S(7) xor S(2) xor B(3) xor S(2) xor S(6) 
				xor B(8) xor B(3) xor S(2) xor S(6) xor S(1);
			S(4) <= B(16) xor B(11) xor B(6) xor B(1) xor S(4) xor S(8) xor S(3) xor B(2) xor S(3) xor S(7) 
				xor B(7) xor B(2) xor S(3) xor S(7) xor S(2);
			S(5) <= B(15) xor B(10) xor B(5) xor B(0) xor S(5) xor S(9) xor S(4) xor B(1) xor S(4) xor S(8) 
				xor B(6) xor B(1) xor S(4) xor S(8) xor S(3);
			S(6) <= B(14) xor B(9) xor B(4) xor S(1) xor S(5) xor B(0) xor S(5) xor S(9) xor B(5) xor B(0) 
				xor S(5) xor S(9) xor S(4);
			S(7) <= B(13) xor B(8) xor B(3) xor S(2) xor S(6) xor S(1) xor B(4) xor S(1) xor S(5);
			S(8) <= B(12) xor B(7) xor B(2) xor S(3) xor S(7) xor S(2) xor B(3) xor S(2) xor S(6);
			S(9) <= B(11) xor B(6) xor B(1) xor S(4) xor S(8) xor S(3) xor B(2) xor S(3) xor S(7);
			S(10) <= B(10) xor B(5) xor B(0) xor S(5) xor S(9) xor S(4) xor B(1) xor S(4) xor S(8);
			S(11) <= B(9) xor B(4) xor S(1) xor S(5) xor B(0) xor S(5) xor S(9);
			S(12) <= B(8) xor B(3) xor S(2) xor S(6) xor S(1);
			S(13) <= B(7) xor B(2) xor S(3) xor S(7) xor S(2);
			S(14) <= B(6) xor B(1) xor S(4) xor S(8) xor S(3);
			S(15) <= B(5) xor B(0) xor S(5) xor S(9) xor S(4);
			S(16) <= B(4) xor S(1) xor S(5);
			S(17) <= B(3) xor S(2) xor S(6);
			S(18) <= B(2) xor S(3) xor S(7);
			S(19) <= B(1) xor S(4) xor S(8);
			S(0) <=  B(0) xor S(5) xor S(9); -- actually the 20th and leading bit (LSB)
		end if;
	end if;
END PROCESS scram_reg;

-- re-map scrambler names to actual output names
        So(0) <= S(0); -- LSB lower 10
        So(1) <= S(19);
        So(2) <= S(18);
        So(3) <= S(17);
        So(4) <= S(16);
        So(5) <= S(15);
        So(6) <= S(14);
        So(7) <= S(13);
        So(8) <= S(12);
        So(9) <= S(11);
        So(10) <= S(10);
        So(11) <= S(9);
        So(12) <= S(8);
        So(13) <= S(7);
        So(14) <= S(6);
        So(15) <= S(5);
        So(16) <= S(4);
        So(17) <= S(3);
        So(18) <= S(2);
        So(19) <= S(1); -- MSB upper 10

--------------------------------------------------------------------
--------------------------------------------------------------------
-- declare NRZI encoder

-- following the scrambling operation, the same data must be NRZI 
-- encoded.  This requires a large number of XOR terms to encode
-- all 20 bits in parallel.  This is faster to encode by using
-- multiple register stages, and only encoding a few bits at each
-- stage.  The final encode is performmed in the output register.

-- encoder is built from multiple stages to limit the number of XOR
-- terms between clock stages to allow maximum clock rate

nrzi_enc: PROCESS(zreset_i,txclk) 
BEGIN
	if (zreset_i = '0') then
		N2 <= (others => '0');
		N  <= (others => '0');
	elsif rising_edge(txclk) then
		if enable_sync = '1' then
			N(1) <= So(19) xor So(18) xor So(17) xor So(16) xor So(15) xor So(14) xor So(13) xor 
				So(12) xor So(11) xor So(10) xor So(9) xor So(8) xor So(7) xor So(6) xor So(5) 
				xor So(4) xor So(3) xor So(2) xor So(1) xor So(0) xor N(1);
			N(2) <= So(18) xor So(17) xor So(16) xor So(15) xor So(14) xor So(13) xor So(12) xor 
				So(11) xor So(10) xor So(9) xor So(8) xor So(7) xor So(6) xor So(5) xor So(4) 
				xor So(3) xor So(2) xor So(1) xor So(0) xor N(1);
			N(3) <= So(17) xor So(16) xor So(15) xor So(14) xor So(13) xor So(12) xor So(11) xor 
				So(10) xor So(9) xor So(8) xor So(7) xor So(6) xor So(5) xor So(4) xor So(3) 
				xor So(2) xor So(1) xor So(0) xor N(1);
			N(4) <= So(16) xor So(15) xor So(14) xor So(13) xor So(12) xor So(11) xor So(10) xor 
				So(9) xor So(8) xor So(7) xor So(6) xor So(5) xor So(4) xor So(3) xor So(2) 
				xor So(1) xor So(0) xor N(1);
			N(5) <= So(15) xor So(14) xor So(13) xor So(12) xor So(11) xor So(10) xor So(9) xor 
				So(8) xor So(7) xor So(6) xor So(5) xor So(4) xor So(3) xor So(2) xor So(1) 
				xor So(0) xor N(1);
			N(6) <= So(14) xor So(13) xor So(12) xor So(11) xor So(10) xor So(9) xor So(8) xor So(7) 
				xor So(6) xor So(5) xor So(4) xor So(3) xor So(2) xor So(1) xor So(0) xor N(1);
			N(7) <= So(13) xor So(12) xor So(11) xor So(10) xor So(9) xor So(8) xor So(7) xor So(6) 
				xor So(5) xor So(4) xor So(3) xor So(2) xor So(1) xor So(0) xor N(1);
			N(8) <= So(12) xor So(11) xor So(10) xor So(9) xor So(8) xor So(7) xor So(6) xor So(5) 
				xor So(4) xor So(3) xor So(2) xor So(1) xor So(0) xor N(1);
			N(9) <= So(11) xor So(10) xor So(9) xor So(8) xor So(7) xor So(6) xor So(5) xor So(4) 
				xor So(3) xor So(2) xor So(1) xor So(0) xor N(1);
			N(10) <= So(10) xor So(9) xor So(8) xor So(7) xor So(6) xor So(5) xor So(4) xor So(3) 
				xor So(2) xor So(1) xor So(0) xor N(1);
			N(11) <= So(9) xor So(8) xor So(7) xor So(6) xor So(5) xor So(4) xor So(3) xor So(2) 
				xor So(1) xor So(0) xor N(1);
			N(12) <= So(8) xor So(7) xor So(6) xor So(5) xor So(4) xor So(3) xor So(2) xor So(1) 
				xor So(0) xor N(1);
			N(13) <= So(7) xor So(6) xor So(5) xor So(4) xor So(3) xor So(2) xor So(1) xor So(0) 
				xor N(1);
			N(14) <= So(6) xor So(5) xor So(4) xor So(3) xor So(2) xor So(1) xor So(0) xor N(1);
			N(15) <= So(5) xor So(4) xor So(3) xor So(2) xor So(1) xor So(0) xor N(1);
			N(16) <= So(4) xor So(3) xor So(2) xor So(1) xor So(0) xor N(1);
			N(17) <= So(3) xor So(2) xor So(1) xor So(0) xor N(1);
			N(18) <= So(2) xor So(1) xor So(0) xor N(1);
			N(19) <= So(1) xor So(0) xor N(1);
			N(0)  <= So(0) xor N(1);
		
			N2(0) <= N(0);
			N2(1) <= N(19);
			N2(2) <= N(18);
			N2(3) <= N(17);
			N2(4) <= N(16);
			N2(5) <= N(15);
			N2(6) <= N(14);
			N2(7) <= N(13);
			N2(8) <= N(12);
			N2(9) <= N(11);
			N2(10) <= N(10);
			N2(11) <= N(9);
			N2(12) <= N(8);
			N2(13) <= N(7);
			N2(14) <= N(6);
			N2(15) <= N(5);
			N2(16) <= N(4);
			N2(17) <= N(3);
			N2(18) <= N(2);
			N2(19) <= N(1);
	 	end if;
    end if;
END PROCESS nrzi_enc;




-------------------------------------------------------------
-- Registers for delaying in bypass mode:
-------------------------------------------------------------
bypass_delaying: PROCESS(zreset_i,txclk) 
BEGIN
   if (zreset_i = '0') then
	   B_delayed_1 <= (others => '0');
	   B_delayed_2 <= (others => '0');
	   B_delayed_3 <= (others => '0');
	elsif rising_edge(txclk) then
		if enable_sync = '1' then
	      B_delayed_1 <= B;
	      B_delayed_2 <= B_delayed_1;
	      B_delayed_3 <= B_delayed_2;
      end if;
   end if;
END PROCESS;


--------------------------------------------------------------------
--------------------------------------------------------------------
-- assign the output register to the output pins of the PLD
-- the mapping is out of order because the inp and S signals
-- were numbered based on the scrambler shifter bit numbers 
-- instead of the LSB/MSB data position

-- declare output register
-- input selection is either NRZI scrambled data or data from B
outreg: PROCESS(zreset_i,txclk) 
BEGIN
    if (zreset_i = '0') then
	dout <= (others => '0');
	--ena  <= '0'; -- commented 11-11-03 by DEF
	elsif rising_edge(txclk) then
		if enable_sync = '1' then
			IF bypassl = '1' THEN
        -- send latched unscrambled data to HOTLink output
        		dout <= B_delayed_3;
        -- enable HOTLink TX to capture data at every clock
			ELSE
        		dout <= N2;
        -- enable HOTLink TX to capture data at every clock
			END IF;
		end if;
    end if;
END PROCESS outreg;


---------------------------------------------------------------------
-- add in tristate control of all outputs for board testability

txd_out <= dout;

END archscram;


