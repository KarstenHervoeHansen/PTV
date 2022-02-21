-- autorate.vhd
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
ENTITY autorate IS
	PORT(
		clkout, 				-- 24 mhz clock from microcontroller
		reset, 					-- reset input
		lfi, cd		: IN	STD_LOGIC;	-- lfi from HOTLink II, cd from cable receiver
		freq					: IN	STD_LOGIC_VECTOR(1 DOWNTO 0);  -- current frequency setting from FPGA register
		next_freq				: BUFFER	STD_LOGIC_VECTOR(1 DOWNTO 0) -- next frequency to try
		);
END autorate;

-- no components

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
-- channelregs.vhd
-- This code is intended for use with the Cypress HOTLink II Video Demostration Board
-------------------------------------------------------------------------------------
--
-- This module implemets the channel control and status registers
--
--	0 = cfgreg		configuration register (write/read)
--		freq <= cfgreg(1 downto 0);
--		dvb_nsmpte <= cfgreg(2);
--		insel <= cfgreg(3);
--		pal_nntsc <= cfgreg(4);
--		lpen <= cfgreg(5);
--		chreset <= cfgreg(7);
--
--	1 = txsrcreg	transmit source register (write/read)
--		txsrc <= txsrcreg(2 downto 0);
--
--	2 = statusreg 	status register (read only)
--			statusreg(0) <= lfi;
--			statusreg(1) <= cd;
--			statusreg(2) <= cd2;
--			statusreg(5 DOWNTO 3) <= video_type;
--			statusreg(7 downto 6) <= next_freq;
--
--	3 = century 	(bcd read only)
--
--	4 = year 		(bcd read only)
--
--	5 = month 		(bcd read only)
--
--	6 = day 		(bcd read only)
--
--	8,9,10,11 = Y_crc_errors	HD-SDI Y crc error count (read only)
--
--	12,13,14,15 = C_crc_errors	HD-SDI C crc error count (read only)
--
--  16 = acrc_errors (read only)
--
--  17 = fcrc_errors (read only)
ENTITY channelregs IS
	PORT(
		reset, write, read	: IN	STD_LOGIC;	-- reset from pin, read & write from spi module
		channel, chsel		: IN 	STD_LOGIC;	-- channel input from SPI, channel select setting fixed in instantiation
		address				: IN	STD_LOGIC_VECTOR(7 DOWNTO 0);	-- address from spi module
		data_in				: INOUT	STD_LOGIC_VECTOR(7 DOWNTO 0);	-- data input from spi module
		data_out			: OUT	STD_LOGIC_VECTOR(7 DOWNTO 0);	-- data output to spi module
		century, year, month, day : IN STD_LOGIC_VECTOR(7 DOWNTO 0);	-- release date from top level module
		video_type			:IN STD_LOGIC_VECTOR(2 DOWNTO 0);
		lfi, cd, cd2		: IN	STD_LOGIC;	-- status indecators from HOTLink II and cable receivers
		dvb_nsmpte, insel	: OUT 	STD_LOGIC;	-- dvb/smpte select output, insel output to HOTLink II
		lpen			: OUT 	STD_LOGIC;	-- lpen output to HOTLink II
		chreset			: OUT 	STD_LOGIC;	--  channel reset
		pal_nntsc		: OUT	STD_LOGIC;	-- pal/ntsc output to test pattern generators
		freq			: OUT	STD_LOGIC_VECTOR(1 DOWNTO 0);	-- frueqency select output
		next_freq		: IN	STD_LOGIC_VECTOR(1 DOWNTO 0);	-- next frequency select input (for auto rate detection)
		txsrc			: out 	STD_LOGIC_VECTOR(2 DOWNTO 0);	-- transmit source select output
		read_acrc_errors			: OUT	STD_LOGIC;		-- read output to edh acrc error counter
		read_fcrc_errors			: OUT	STD_LOGIC;		-- read output to edh fcrc error counter
		acrc_errors, fcrc_errors	: IN STD_LOGIC_VECTOR( 7 DOWNTO 0);  -- crc error counts from edh module
		readYcrc, readCcrc		: BUFFER 	STD_LOGIC;				-- read outputws to HD-SDI CRC error counters
		Y_crc_errors, C_crc_errors : in std_logic_vector(31 downto 0)	-- input values from HD-SDI crc error counters
	);
END channelregs;

-- no components

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
package pack_crc is

  function crc_0_5_12_16
    ( data:  std_logic_vector(9 downto 0);
      crc:   std_logic_vector(15 downto 0) )
    return std_logic_vector;

  function crc_0_4_5_18
    ( Data:  std_logic_vector(9 downto 0);
      CRC:   std_logic_vector(17 downto 0) )
    return std_logic_vector;

end pack_crc;

library IEEE;
use IEEE.std_logic_1164.all;

package body pack_crc is


  function crc_0_5_12_16  
    ( data:  std_logic_vector(9 downto 0);
      crc:   std_logic_vector(15 downto 0) )
    return std_logic_vector is

    variable new_crc: std_logic_vector(15 downto 0);
	variable c: std_logic_vector(15 downto 0);
	variable d: std_logic_vector(9 downto 0);

  begin

	d := data;
	c := crc;
	
	new_crc(15) := c(9) xor c(5) xor c(1) xor d(1) xor d(5) xor d(9);
	new_crc(14) := c(8) xor c(4) xor c(0) xor d(0) xor d(4) xor d(8);
	new_crc(13) := c(7) xor c(3) xor d(3) xor d(7);
	new_crc(12) := c(6) xor c(2) xor d(2) xor d(6);
	new_crc(11) := c(5) xor c(1) xor d(1) xor d(5);
	new_crc(10) := c(4) xor c(0) xor d(0) xor d(4) xor c(9) xor c(5) xor c(1) xor d(1) xor d(5) xor d(9);
	new_crc(9)  := c(3) xor d(3) xor c(8) xor c(4) xor c(0) xor d(0) xor d(4) xor d(8);
	new_crc(8)  := c(2) xor d(2) xor c(7) xor c(3) xor d(3) xor d(7);
	new_crc(7)  := c(1) xor d(1) xor c(6) xor c(2) xor d(2) xor d(6);
	new_crc(6)  := c(0) xor d(0) xor c(5) xor c(1) xor d(1) xor d(5);
	new_crc(5)  := c(15) xor c(4) xor c(0) xor d(0) xor d(4);
	new_crc(4)  := c(14) xor c(3) xor d(3);
	new_crc(3)  := c(13) xor c(2) xor d(2) xor c(9) xor c(5) xor c(1) xor d(1) xor d(5) xor d(9);
	new_crc(2)  := c(12) xor c(1) xor d(1) xor c(8) xor c(4) xor c(0) xor d(0) xor d(4) xor d(8);
	new_crc(1)  := c(11) xor c(0) xor d(0) xor c(7) xor c(3) xor d(3) xor d(7);
	new_crc(0)  := c(10) xor c(6) xor c(2) xor d(2) xor d(6);

    return new_crc;

  end crc_0_5_12_16;

  function crc_0_4_5_18  
    ( data:  std_logic_vector(9 downto 0);
      crc:   std_logic_vector(17 downto 0) )
    return std_logic_vector is

    variable new_crc: std_logic_vector(17 downto 0);
	variable c: std_logic_vector(17 downto 0);
	variable d: std_logic_vector(9 downto 0);

  begin

	d := data;
	c := crc;

	new_crc(17) := c(9)  xor d(9);
	new_crc(16) := c(8)  xor d(8);
	new_crc(15) := c(7)  xor d(7);
	new_crc(14) := c(6)  xor d(6);
	new_crc(13) := c(5)  xor d(5) xor c(9) xor d(9);
	new_crc(12) := c(4)  xor d(4) xor c(8) xor d(8) xor c(9) xor d(9);
	new_crc(11) := c(3)  xor d(3) xor c(7) xor d(7) xor c(8) xor d(8);
	new_crc(10) := c(2)  xor d(2) xor c(6) xor d(6) xor c(7) xor d(7);
	new_crc(9)  := c(1)  xor d(1) xor c(5) xor d(5) xor c(6) xor d(6);
	new_crc(8)  := c(0)  xor d(0) xor c(4) xor d(4) xor c(5) xor d(5);
	new_crc(7)  := c(17) xor c(3) xor d(3) xor c(4) xor d(4);
	new_crc(6)  := c(16) xor c(2) xor d(2) xor c(3) xor d(3);
	new_crc(5)  := c(15) xor c(1) xor d(1) xor c(2) xor d(2);
	new_crc(4)  := c(14) xor c(0) xor d(0) xor c(1) xor d(1);
	new_crc(3)  := c(13) xor c(0) xor d(0);
	new_crc(2)  := c(12);
	new_crc(1)  := c(11);
	new_crc(0)  := c(10);

    return new_crc;

  end crc_0_4_5_18;

end pack_crc;

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
-- crc292check.vhd
-- This code is intended for use with the Cypress HOTLink II Video Demostration Board
-------------------------------------------------------------------------------------
--
-- This module checks for CRC errors in smpte 292 video data
-- 1080i at 30 frames/sec is supported
--
ENTITY crc292check IS
	PORT(
		clk, reset	: IN STD_LOGIC;
		data_in		: IN STD_LOGIC_VECTOR(0 to 9);
		readcrc		: IN 	STD_LOGIC;
		crc_errors   : BUFFER STD_LOGIC_VECTOR(31 DOWNTO 0)
	);
END crc292check;

-- no components

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
-- CY7C9335 SMPTE Decoder/Framer Design

-- This design combines an NRZI decoder, SMPTE descrabler, parallel
-- framer, and DVB-ASI handler (primarily a bypass interface).

-- The NRZI decoder works just the opposite of the encoder.  Instead
-- of using a feedback process, this uses a feedforward process to
-- remove the effect of the encoder.  This should allow the decoder
-- to be implemented with only a minimal number of XOR terms; one
-- per bit implemented in parallel.  Since ten bits are handled in
-- each clock, it should take 10 XOR gates.

-- First determine the shift sequence.  The 10 input bits are b[0:9]
-- as capturered in the input register x[0:9].  Register q[0:9] holds
-- the previously processed ten bits.  The equations for q[0:9] are 
-- determined by calculating the equivalent bits after each shift.  
-- Following ten shifts, the equations will describe the hardware 
-- necessary.

-- Original                 after one shift clock these registers equal
-- x9 = b9   q9 = a9        x9 = c0   q9 = b0+b1
-- x8 = b8   q8 = a8        x8 = b9   q8 = a9
-- x7 = b7   q7 = a7        x7 = b8   q7 = a8
-- x6 = b6   q6 = a6        x6 = b7   q6 = a7
-- x5 = b5   q5 = a5        x5 = b6   q5 = a6
-- x4 = b4   q4 = a4        x4 = b5   q4 = a5
-- x3 = b3   q3 = a3        x3 = b4   q3 = a4
-- x2 = b2   q2 = a2        x2 = b3   q2 = a3
-- x1 = b1   q1 = a1        x1 = b2   q1 = a2
-- x0 = b0   q0 = a0        x0 = b1   q0 = a1


-- after two shift clocks   after nine shift clocks
-- these registers equal    these registers equal
-- x9 = c1   q9 = b1+b2     x9 = c8   q9 = b8+b9
-- x8 = c0   q8 = b0+b1     x8 = c7   q8 = b7+b8
-- x7 = b9   q7 = a9        x7 = c6   q7 = b6+b7
-- x6 = b8   q6 = a8        x6 = c5   q6 = b5+b6
-- x5 = b7   q5 = a7        x5 = c4   q5 = b4+b5
-- x4 = b6   q4 = a6        x4 = c3   q4 = b3+b4
-- x3 = b5   q3 = a5        x3 = c2   q3 = b2+b3
-- x2 = b4   q2 = a4        x2 = c1   q2 = b1+b2
-- x1 = b3   q1 = a3        x1 = c0   q1 = b0+b1
-- x0 = b2   q0 = a2        x0 = b9   q0 = a9


-- after 10 shift clocks these registers equal
-- x9 = c9   q9 = b9+c0   MSB
-- x8 = c8   q8 = b8+b9
-- x7 = c7   q7 = b7+b8
-- x6 = c6   q6 = b6+b7
-- x5 = c5   q5 = b5+b6
-- x4 = c4   q4 = b4+b5
-- x3 = c3   q3 = b3+b4
-- x2 = c2   q2 = b2+b3
-- x1 = c1   q1 = b1+b2
-- x0 = c0   q0 = b0+b1   LSB

-- in reality, all that is needed is 11 bits of data to decode
-- 10 bits in a single clock.  This can be done by keeping one
-- bit from the previous clock cycle to extend the register, 
-- then decoding q0-q9 from the 11 bit register

-- When accepting data from the HOTLink RX, bit Q(a)is the LSB
-- of the received word and is received first.  The descramber
-- requires that the bits be routed through the circuis LSB first. 

-- The descrambler uses a feed-forward form of the x^9 + X^4 + 1 
-- SMPTE scrambler algorithm to unscramble the data stream.

-- The feed-forward descrambler needs to descramble 10 bits every
-- clock cycle.  Since the scrambler code only spans 9 bits, 

-- The initial conditions have the descrambler register D1[0:9] containing
-- the values x[0:9] as a present condition shown here.

-- D1(9) = x9   -- MSB
-- D1(8) = x8 
-- D1(7) = x7
-- D1(6) = x6
-- D1(5) = x5   -- feed-forward tap
-- D1(4) = x4
-- D1(3) = x3
-- D1(2) = x2
-- D1(1) = x1
-- D1(0) = x0   -- LSB 

-- As the first non-descrambled bit d0 is clocked in, the register contents
-- would change to 

-- D1(9) = d0   -- MSB
-- D1(8) = x9 
-- D1(7) = x8
-- D1(6) = x7
-- D1(5) = x6   -- feed-forward tap
-- D1(4) = x5
-- D1(3) = x4
-- D1(2) = x3
-- D1(1) = x2
-- D1(0) = x1 + d0 + x5   -- LSB 

-- If another bit were clocked into the descrambler at this time, the
-- descrambled bit in D1(0) would be shifted out and lost.  To make sure 
-- that no data is lost, the descrambler must be extended by nine bits in a
-- second register

-- D1(9) = d1   -- MSB
-- D1(8) = d0 
-- D1(7) = x9
-- D1(6) = x8
-- D1(5) = x7   -- feed-forward tap
-- D1(4) = x6
-- D1(3) = x5
-- D1(2) = x4
-- D1(1) = x3
-- D1(0) = x2 + d1 + x6 -- LSB 

-- D2(8) = x1 + d0 + x5 -- MSB    
-- D2(7) = x0
-- D2(6) = y8
-- D2(5) = y7
-- D2(4) = y6
-- D2(3) = y5
-- D2(2) = y4
-- D2(1) = y3
-- D2(0) = y2


-- After a third shift the descrambler contents are
-- D1(9) = d2   -- MSB
-- D1(8) = d1 
-- D1(7) = d0
-- D1(6) = x9
-- D1(5) = x8   -- feed-forward tap
-- D1(4) = x7
-- D1(3) = x6
-- D1(2) = x5
-- D1(1) = x4
-- D1(0) = x3 + d2 + x7 -- LSB 

-- D2(8) = x2 + d1 + x6 -- MSB    
-- D2(7) = x1 + d0 + x5
-- D2(6) = x0
-- D2(5) = y8
-- D2(4) = y7
-- D2(3) = y6
-- D2(2) = y5
-- D2(1) = y4
-- D2(0) = y3

-- At the sixth shift, all the previous bits have passed the feed-forward
-- tap and are now just encoding data bits.
-- D1(9) = d5   -- MSB
-- D1(8) = d4 
-- D1(7) = d3
-- D1(6) = d2
-- D1(5) = d1   -- feed-forward tap
-- D1(4) = d0
-- D1(3) = x9
-- D1(2) = x8
-- D1(1) = x7
-- D1(0) = x6 + d5 + d0 -- LSB 

-- D2(8) = x5 + d4 + x9 -- MSB    
-- D2(7) = x4 + d3 + x8
-- D2(6) = x3 + d2 + x7
-- D2(5) = x2 + d1 + x6
-- D2(4) = x1 + d0 + x5
-- D2(3) = x0
-- D2(2) = y8
-- D2(1) = y7
-- D2(0) = y6

-- After the full 10 bits are descrambled, the equations are
-- D1(9) = d9   -- MSB
-- D1(8) = d8 
-- D1(7) = d7
-- D1(6) = d6
-- D1(5) = d5   -- feed-forward tap
-- D1(4) = d4
-- D1(3) = d3
-- D1(2) = d2
-- D1(1) = d1
-- D1(0) = d0 + d9 + d4 -- LSB 

-- D2(8) = x9 + d8 + d3 -- MSB    
-- D2(7) = x8 + d7 + d2
-- D2(6) = x7 + d6 + d1
-- D2(5) = x6 + d5 + d0
-- D2(4) = x5 + d4 + x9
-- D2(3) = x4 + d3 + x8
-- D2(2) = x3 + d2 + x7
-- D2(1) = x2 + d1 + x6
-- D2(0) = x1 + d0 + x5 -- LSB

-- If we now look at the low order 10 bits of this 19 bit register,
-- we can see the equations for the descrambled output character.
-- This character would span from D1(0) as the MSB to D2(0) as the LSB.
 
LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY dscramrx IS 
    PORT (
        rxclk,                      -- CY7B933 CKR recovered clock
		reset_n,
        DVB_select: IN std_logic;   -- select 8B/10B mode (active LOW)
        bypass: IN std_logic;       -- raw data mode, bypass scrambler data
        SYNC_en: IN std_logic;      -- TRS/SYNC correction enabled
        oe: IN std_logic;           -- tristate output enable
        data_in: IN std_logic_vector (0 TO 9);
                                    -- scrambled data from HOTLink RX
        data_out: BUFFER std_logic_vector (0 TO 9);
                                    -- descrambled and framed data
        SYNC_error: out std_logic;   -- SYNC offset error pulse
        RF: out std_logic;       -- HOTLink RX frame control pin
        AB: out std_logic;       -- HOTLink A/B port select pin
        H_SYNC: out std_logic);  -- horizontal sync output toggle
END dscramrx;

    COMPONENT port_sel 

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
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

-- no components

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
-- SMPTE-292 descrambler mux/demux wrapper file
LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY dscram20_top IS 
    PORT (
        rxclk,                      -- CY7B933 CKR recovered clock
        reset,                      -- active high reset
        rx_oe,                      -- output enable
        bypass: IN std_logic;       -- raw data mode, bypass scrambler data
	    ten_twenty : in std_logic;  -- switch for determining either a 10 or 20 bit interface
        sync_en: IN std_logic;      -- TRS/SYNC correction enabled
        rxdata_in: IN std_logic_vector (0 to 9);   -- scrambled data from HOTLink RX
        rxdata_out: out std_logic_vector (0 to 19); -- descrambled and framed data
        sync_err: out std_logic;    -- SYNC offset error pulse
        h_sync: out std_logic);     -- horizontal sync output toggle
END dscram20_top;

    component altera_ddr_input10

    component altera_ddr_output22

    component dscram20

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
-- edh.vhd
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

-- no components

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
ENTITY hdvb0 IS
	PORT(
		-- cyv15g0403 interface
		txclkoa, txclkob		: IN	STD_LOGIC;
		rxclka_p, rxclkb_p		: IN	STD_LOGIC;
		rxclka_n, rxclkb_n		: IN	STD_LOGIC;
		cd_muteb2				: IN	STD_LOGIC;
		rxda, rxdb				: IN	STD_LOGIC_VECTOR(7 DOWNTO 0);
		rxsta, rxstb			: IN	STD_LOGIC_VECTOR(2 DOWNTO 0);
		lfia, lfib				: IN	STD_LOGIC;
		txerra, txerrb			: IN STD_LOGIC;
		cd_mutea, cd_muteb		: INOUT STD_LOGIC;
		txclka, txclkb			: buffer	STD_LOGIC;
		txda, txdb				: buffer	STD_LOGIC_VECTOR(7 DOWNTO 0);
		txcta, txctb			: buffer	STD_LOGIC_VECTOR(1 DOWNTO 0);
		sd_hda, sd_hdb			: buffer 	STD_LOGIC;
		rclkena, rclkenb		: OUT STD_LOGIC;
		lpena, lpenb			: OUT	STD_LOGIC;
		ulca, ulcb				: OUT	STD_LOGIC;
		insela, inselb			: BUFFER	STD_LOGIC;
		spdsela, spdselb		: BUFFER	STD_LOGIC;
		ldtden					: OUT	STD_LOGIC;
		fclka_p, fclka_n, fclkb_p, fclkb_n			: buffer STD_LOGIC;
		

		-- Parallel Interfaces
		ptxda, ptxdb			: IN	STD_LOGIC_VECTOR(9 downto 0);
		prxda, prxdb			: BUFFER	STD_LOGIC_VECTOR(9 DOWNTO 0);
		prxclka, prxclkb		: OUT STD_LOGIC;
		ptxclka, ptxclkb		: IN STD_LOGIC;

		-- led's
		led270a, led270b, led360a, led360b	: OUT STD_LOGIC;	-- LED OUTPUTS
		led540a, led540b, led7425a, led7425b	: OUT STD_LOGIC;
		ledlfia, ledlfib		: BUFFER STD_LOGIC;
		ledcda, ledcdb			: BUFFER STD_LOGIC;
		led2					: OUT STD_LOGIC;
		
		-- fx2/gpif interface
		clkout, reset_n							: IN	STD_LOGIC;
		ifclk									: INOUT	STD_LOGIC;
		pa7_flagd_slcs							: INOUT	STD_LOGIC;
		rdy										: BUFFER	STD_LOGIC_VECTOR(1 DOWNTO 0);
		ctl										: IN	STD_LOGIC_VECTOR(2 DOWNTO 0);
		fd										: INOUT	STD_LOGIC_VECTOR(15 DOWNTO 0);
		
		-- spi interface
		scse, scl, sdo							: IN	STD_LOGIC;
		sdi										: BUFFER	STD_LOGIC);
END hdvb0;

	COMPONENT spi

	COMPONENT channelregs

	COMPONENT smpte_core

	COMPONENT tpgen

	COMPONENT eg1_292

	COMPONENT grey_292

	COMPONENT rp198 IS

	COMPONENT smpte_292

	COMPONENT crc292check

	COMPONENT video_detect

	COMPONENT edh

	COMPONENT autorate

	component pll1

	component pll2

	component upconvert

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
ENTITY hdvb1 IS
	PORT(
		-- cyv15g0403 interface
		txclkoc, txclkod		: IN	STD_LOGIC;
		rxclkc_p, rxclkd_p		: IN	STD_LOGIC;
		rxclkc_n, rxclkd_n		: IN	STD_LOGIC;
		cd_muted2				: IN	STD_LOGIC;
		rxdc, rxdd				: IN	STD_LOGIC_VECTOR(7 DOWNTO 0);
		rxstc, rxstd			: IN	STD_LOGIC_VECTOR(2 DOWNTO 0);
		lfic, lfid				: IN	STD_LOGIC;
		txerrc, txerrd			: IN STD_LOGIC;
		cd_mutec, cd_muted		: INOUT STD_LOGIC;
		txclkc, txclkd			: BUFFER	STD_LOGIC;
		txdc, txdd				: OUT	STD_LOGIC_VECTOR(7 DOWNTO 0);
		txctc, txctd			: OUT	STD_LOGIC_VECTOR(1 DOWNTO 0);
		sd_hdc, sd_hdd			: BUFFER 	STD_LOGIC;
		rclkenc, rclkend		: OUT STD_LOGIC;
		lpenc, lpend			: OUT	STD_LOGIC;
		ulcc, ulcd				: OUT	STD_LOGIC;
		inselc, inseld			: BUFFER	STD_LOGIC;
		spdselc, spdseld		: BUFFER	STD_LOGIC;
		
		fclkc_p, fclkc_n, fclkd_p, fclkd_n			: OUT STD_LOGIC;
		ssi_cdc2				: IN STD_LOGIC;

		-- Parallel Interfaces
		ptxdc, ptxdd			: IN	STD_LOGIC_VECTOR(9 downto 0);
		prxdc, prxdd			: BUFFER	STD_LOGIC_VECTOR(9 DOWNTO 0);
		prxclkc, prxclkd		: OUT STD_LOGIC;
		ptxclkc, ptxclkd		: IN STD_LOGIC;

		-- led's
		led270c, led270d, led360c, led360d	: OUT STD_LOGIC;	-- LED OUTPUTS
		led540c, led540d, led7425c, led7425d	: OUT STD_LOGIC;
		ledlfic, ledlfid		: BUFFER STD_LOGIC;
		ledcdc, ledcdd			: BUFFER STD_LOGIC;
		led2					: OUT STD_LOGIC;
		
		-- fx2/gpif interface
		clkout, reset_n							: IN	STD_LOGIC;
		ifclk									: INOUT	STD_LOGIC;
		pa7_flagd_slcs							: INOUT	STD_LOGIC;
		rdy										: BUFFER	STD_LOGIC_VECTOR(1 DOWNTO 0);
		ctl										: IN	STD_LOGIC_VECTOR(2 DOWNTO 0);
		fd										: INOUT	STD_LOGIC_VECTOR(15 DOWNTO 0);
		
		-- spi interface
		scse1, scl, sdo							: IN	STD_LOGIC;
		sdi										: BUFFER	STD_LOGIC);
END hdvb1;

	COMPONENT spi

	COMPONENT channelregs

	COMPONENT smpte_core

	COMPONENT tpgen

	COMPONENT eg1_292

	COMPONENT grey_292

	COMPONENT rp198 IS

	COMPONENT smpte_292

	COMPONENT crc292check

	COMPONENT video_detect

	COMPONENT edh

	COMPONENT autorate

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
-- megafunction wizard: %ALTSYNCRAM%
-- GENERATION: STANDARD
-- VERSION: WM1.0
-- MODULE: altsyncram 

-- ============================================================
-- File Name: line_buff.vhd
-- Megafunction Name(s):
-- 			altsyncram
-- ============================================================
-- ************************************************************
-- THIS IS A WIZARD-GENERATED FILE. DO NOT EDIT THIS FILE!
--
-- 4.0 Build 214 3/25/2004 SP 1 SJ Web Edition
-- ************************************************************


--Copyright (C) 1991-2004 Altera Corporation
--Any  megafunction  design,  and related netlist (encrypted  or  decrypted),
--support information,  device programming or simulation file,  and any other
--associated  documentation or information  provided by  Altera  or a partner
--under  Altera's   Megafunction   Partnership   Program  may  be  used  only
--to program  PLD  devices (but not masked  PLD  devices) from  Altera.   Any
--other  use  of such  megafunction  design,  netlist,  support  information,
--device programming or simulation file,  or any other  related documentation
--or information  is prohibited  for  any  other purpose,  including, but not
--limited to  modification,  reverse engineering,  de-compiling, or use  with
--any other  silicon devices,  unless such use is  explicitly  licensed under
--a separate agreement with  Altera  or a megafunction partner.  Title to the
--intellectual property,  including patents,  copyrights,  trademarks,  trade
--secrets,  or maskworks,  embodied in any such megafunction design, netlist,
--support  information,  device programming or simulation file,  or any other
--related documentation or information provided by  Altera  or a megafunction
--partner, remains with Altera, the megafunction partner, or their respective
--licensors. No other licenses, including any licenses needed under any third
--party's intellectual property, are provided herein.


LIBRARY ieee;
USE ieee.std_logic_1164.all;

LIBRARY altera_mf;
USE altera_mf.altera_mf_components.all;

ENTITY line_buff IS
	PORT
	(
		data		: IN STD_LOGIC_VECTOR (9 DOWNTO 0);
		wren		: IN STD_LOGIC  := '1';
		wraddress		: IN STD_LOGIC_VECTOR (13 DOWNTO 0);
		rdaddress		: IN STD_LOGIC_VECTOR (12 DOWNTO 0);
		wrclock		: IN STD_LOGIC ;
		rdclock		: IN STD_LOGIC ;
		wr_aclr		: IN STD_LOGIC  := '0';
		rd_aclr		: IN STD_LOGIC  := '0';
		q		: OUT STD_LOGIC_VECTOR (19 DOWNTO 0)
	);
END line_buff;

	COMPONENT altsyncram

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
package pack_reverse_bit_order is
	function reverse_bit_order( val:std_logic_vector; width:integer) return std_logic_vector;
	function rbo(val: std_logic_vector; lsb, msb: integer)return std_logic_vector;
end pack_reverse_bit_order;
package body pack_reverse_bit_order is
	function reverse_bit_order( val:std_logic_vector; width:integer) return std_logic_vector is
		variable result : std_logic_vector(width-1 downto 0) := (others=>'0');
		variable bits : integer := width;
		
	begin
		for i in 0 to bits-1 loop
			result(i) := val(width-i-1);
		end loop;
		return (result);
	end reverse_bit_order;
	
	function rbo(val: std_logic_vector; lsb, msb: integer)return std_logic_vector is
		variable result : std_logic_vector(msb downto lsb) := (others => '0');
		variable bits : integer := (msb-lsb)+1;
	begin
		for i in lsb to msb loop
			result(i) := val(msb-(i-lsb));
		end loop;
		return (result);
	end rbo;
end pack_reverse_bit_order;

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
-- megafunction wizard: %ALTPLL%
-- GENERATION: STANDARD
-- VERSION: WM1.0
-- MODULE: altpll 

-- ============================================================
-- File Name: pll1.vhd
-- Megafunction Name(s):
-- 			altpll
-- ============================================================
-- ************************************************************
-- THIS IS A WIZARD-GENERATED FILE. DO NOT EDIT THIS FILE!
--
-- 4.0 Build 214 3/25/2004 SP 1 SJ Web Edition
-- ************************************************************


--Copyright (C) 1991-2004 Altera Corporation
--Any  megafunction  design,  and related netlist (encrypted  or  decrypted),
--support information,  device programming or simulation file,  and any other
--associated  documentation or information  provided by  Altera  or a partner
--under  Altera's   Megafunction   Partnership   Program  may  be  used  only
--to program  PLD  devices (but not masked  PLD  devices) from  Altera.   Any
--other  use  of such  megafunction  design,  netlist,  support  information,
--device programming or simulation file,  or any other  related documentation
--or information  is prohibited  for  any  other purpose,  including, but not
--limited to  modification,  reverse engineering,  de-compiling, or use  with
--any other  silicon devices,  unless such use is  explicitly  licensed under
--a separate agreement with  Altera  or a megafunction partner.  Title to the
--intellectual property,  including patents,  copyrights,  trademarks,  trade
--secrets,  or maskworks,  embodied in any such megafunction design, netlist,
--support  information,  device programming or simulation file,  or any other
--related documentation or information provided by  Altera  or a megafunction
--partner, remains with Altera, the megafunction partner, or their respective
--licensors. No other licenses, including any licenses needed under any third
--party's intellectual property, are provided herein.

ENTITY pll1 IS
	PORT
	(
		inclk0		: IN STD_LOGIC  := '0';
		pllena		: IN STD_LOGIC  := '1';
		areset		: IN STD_LOGIC  := '0';
		c0		: OUT STD_LOGIC ;
		locked		: OUT STD_LOGIC 
	);
END pll1;

	COMPONENT altpll

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
-- megafunction wizard: %ALTPLL%
-- GENERATION: STANDARD
-- VERSION: WM1.0
-- MODULE: altpll 

-- ============================================================
-- File Name: pll2.vhd
-- Megafunction Name(s):
-- 			altpll
-- ============================================================
-- ************************************************************
-- THIS IS A WIZARD-GENERATED FILE. DO NOT EDIT THIS FILE!
--
-- 4.0 Build 214 3/25/2004 SP 1 SJ Web Edition
-- ************************************************************


--Copyright (C) 1991-2004 Altera Corporation
--Any  megafunction  design,  and related netlist (encrypted  or  decrypted),
--support information,  device programming or simulation file,  and any other
--associated  documentation or information  provided by  Altera  or a partner
--under  Altera's   Megafunction   Partnership   Program  may  be  used  only
--to program  PLD  devices (but not masked  PLD  devices) from  Altera.   Any
--other  use  of such  megafunction  design,  netlist,  support  information,
--device programming or simulation file,  or any other  related documentation
--or information  is prohibited  for  any  other purpose,  including, but not
--limited to  modification,  reverse engineering,  de-compiling, or use  with
--any other  silicon devices,  unless such use is  explicitly  licensed under
--a separate agreement with  Altera  or a megafunction partner.  Title to the
--intellectual property,  including patents,  copyrights,  trademarks,  trade
--secrets,  or maskworks,  embodied in any such megafunction design, netlist,
--support  information,  device programming or simulation file,  or any other
--related documentation or information provided by  Altera  or a megafunction
--partner, remains with Altera, the megafunction partner, or their respective
--licensors. No other licenses, including any licenses needed under any third
--party's intellectual property, are provided herein.

ENTITY pll2 IS
	PORT
	(
		inclk0		: IN STD_LOGIC  := '0';
		pllena		: IN STD_LOGIC  := '1';
		areset		: IN STD_LOGIC  := '0';
		c0		: OUT STD_LOGIC ;
		locked		: OUT STD_LOGIC 
	);
END pll2;

	COMPONENT altpll

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
-- A/B DVB-ASI Port Selector

-- data-stream inversion state machine for DVB-ASI interfaces
--
-- This machine has a five state supervisor machine that tracks
-- the number of errors detected within a specific period of time.
-- It also tracks valid characters and SYNC codes.
-- If too many errors are detected in a small period of time, it
-- is assumed that the data stream is inverted.  The A/B select is
-- then toggled and the machine then waits for a consecutive pair
-- of SYNC codes to be received before starting to check characters
-- again.

PACKAGE port_select IS
        COMPONENT port_sel PORT (
        rxclk,                  -- Receiver clock
        enable: IN std_logic;   -- enable port selector
        reg_data: IN std_logic_vector(0 TO 9);
                                -- latched HOTLink RX data bus
        A_B: BUFFER std_logic   -- A/B port select
        );
        END COMPONENT;
END port_select;

ENTITY port_sel IS PORT (
        rxclk,                  -- Receiver clock
        enable: IN std_logic;   -- enable port selector
        reg_data: IN std_logic_vector(0 TO 9);
                                -- latched HOTLink RX data bus
        A_B: BUFFER std_logic   -- A_B port select
        );
END port_sel;

-- no components

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
-- CY7C9235 SMPTE Encoder Design

-- This design takes an 8 or 10 bit parallel data stream and
-- encodes it for serialization using the SMPTE scrambler algorithm.
-- This includes the x^9 + x^4 + 1 scrambler and the x + 1 NRZI
-- encoder.

-- Top end target clock rate is 40MHz (25ns) for a -83 speed-bin

ENTITY scramtx IS
    PORT (
        txclk,                      -- HOTLink TX CKW clock
        DVB_EN: IN std_logic;       -- select 8B/10B mode (active LOW)
        trs_filt: IN std_logic;     -- TRS filter
        ena_in: IN std_logic;       -- /ENA input
        enn_in: IN std_logic;       -- /ENN input
        SVS_en: IN std_logic;       -- enable transmission of SVS
        SCD_en: IN std_logic;       -- enable selection of commands
        oe: IN std_logic;           -- tristate output enable
        bypass: IN std_logic;       -- raw data mode - bypass scrambler
                                    -- 10-bit raw data interface
        data: IN std_logic_vector(0 to 9);
        trs_out: out std_logic;  -- TRS code detected
        ena_out: out std_logic;  -- HOTLink TX/ENA input
        data_out: BUFFER std_logic_vector(0 to 9)); 
                                    -- parallel scrambled output
END scramtx;

-- no components

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
-- SMPTE 292 20-bit wide Scrambler Design

-- This design takes a 20 bit parallel data stream and
-- encodes it for serialization using the SMPTE scrambler algorithm.
-- This includes the x^9 + x^4 + 1 scrambler and the x + 1 NRZI
-- encoder.

ENTITY scram20 IS
    PORT (
        txclk,                      -- HOTLink TX CKW clock
        reset,                      -- active high reset added by DEF 11-6-03
        trs_filt_lower10: IN std_logic;     -- TRS filter
        trs_filt_upper10: IN std_logic;     -- TRS filter
        bypass: IN std_logic;       -- raw data mode - bypass scrambler
        txd_in: IN std_logic_vector(0 to 19);
        trs_out_lower10: out std_logic;  -- TRS code detected
        trs_out_upper10: out std_logic;  -- TRS code detected
        txd_out: out std_logic_vector(0 to 19)); 
                                    -- parallel scrambled output
END scram20;

-- no components

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
-- SMPTE-292 scrambler mux/demux wrapper file

-- This design takes an 20 bit parallel data stream and
-- encodes it for serialization using the SMPTE scrambler algorithm.
-- This includes the x^9 + x^4 + 1 scrambler and the x + 1 NRZI
-- encoder.

ENTITY scram20_top IS
    PORT (
        txclk,                      -- HOTLink TX CKW clock
        reset,                      -- active high reset
        tx_oe,                      -- output enable
        bypass: IN std_logic;       -- raw data mode - bypass scrambler
	ten_twenty : in std_logic;  -- switch for determining either a 10 or 20 bit interface
        trs_filt_lower10: IN std_logic; -- TRS filter lower 10 bits
        trs_filt_upper10: IN std_logic; -- TRS filter upper 10 bits
        txdata_in: IN std_logic_vector(0 to 19); -- raw data in
        trs_det: out std_logic;     -- TRS code detected
        txdata_out: out std_logic_vector(0 to 9)); -- parallel scrambled output
END scram20_top;

    component altera_ddr_input22

    component scram20

    component altera_ddr_output11

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
-- SMPTE-292 descrambler mux/demux wrapper file

ENTITY smpte_292 IS 
    PORT (
        -- scrambler tx I/O
        txclk,                         -- HOTLink TX CKW clock
        tx_reset,                      -- active high reset
        tx_oe,                         -- output enable
        tx_bypass: IN std_logic;       -- raw data mode - bypass scrambler
	tx_ten_twenty : in std_logic;  -- switch for determining either a 10 or 20 bit interface
        trs_filt_lower10: IN std_logic;-- TRS filter lower 10 bits
        trs_filt_upper10: IN std_logic;-- TRS filter upper 10 bits
        txdata_in: IN std_logic_vector(0 to 19); -- raw data in from Co-Processor
        trs_det: out std_logic;        -- TRS code detected
        txdata_out: out std_logic_vector(0 to 9); -- parallel scrambled output to Hotlink II PHY
        -- de-scrambler rx I/O
	rxclk,                         -- CY7B933 CKR recovered clock
        rx_reset,                      -- active high reset
        rx_oe,                         -- output enable
        rx_bypass: IN std_logic;       -- raw data mode, bypass scrambler data
	rx_ten_twenty : in std_logic;  -- switch for determining either a 10 or 20 bit interface
        sync_en: IN std_logic;         -- TRS/SYNC correction enabled
        rxdata_in: IN std_logic_vector(0 to 9);   -- scrambled data input from HOTLink II PHY
        rxdata_out: out std_logic_vector(0 to 19); -- descrambled and framed data
        sync_err: out std_logic;       -- SYNC offset error pulse
        h_sync: out std_logic);        -- horizontal sync output toggle
END smpte_292;

    component scram20_top

    component dscram20_top

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
ENTITY smpte_core IS
    --GENERIC ( );
    PORT (
        -- Descrambler signals
        rxclk,                      -- CY7B933 CKR recovered clock
        reset_n,
        DVB_select: IN std_logic;   -- select 8B/10B mode (active LOW)
        rxbypass: IN std_logic;       -- raw data mode, bypass scrambler data
        SYNC_en: IN std_logic;      -- TRS/SYNC correction enabled
        rxoe: IN std_logic;           -- tristate output enable
        data_in: IN std_logic_vector (0 TO 9);
                                    -- scrambled data from HOTLink RX
        rxdata_out: buffer std_logic_vector (0 TO 9);
                                    -- descrambled and framed data
        SYNC_error: out std_logic;   -- SYNC offset error pulse
        RF: out std_logic;       -- HOTLink RX frame control pin
        AB: out std_logic;       -- HOTLink A/B port select pin
        H_SYNC: out std_logic;     -- horizontal sync output toggle

        -- Scrambler Signals
        txclk,                      -- HOTLink TX CKW clock
        DVB_EN: IN std_logic;       -- select 8B/10B mode (active LOW)
        trs_filt: IN std_logic;     -- TRS filter
        ena_in: IN std_logic;       -- /ENA input
        enn_in: IN std_logic;       -- /ENN input
        SVS_en: IN std_logic;       -- enable transmission of SVS
        SCD_en: IN std_logic;       -- enable selection of commands
        txoe: IN std_logic;           -- tristate output enable
        txbypass: IN std_logic;       -- raw data mode - bypass scrambler
                                    -- 10-bit raw data interface
        data: IN std_logic_vector(0 to 9);
        trs_out: out std_logic;  -- TRS code detected
        ena_out: out std_logic;  -- HOTLink TX/ENA input
        txdata_out: BUFFER std_logic_vector(0 to 9) -- parallel scrambled output
);
END smpte_core;

    COMPONENT dscramrx

    COMPONENT scramtx

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
-- spi.vhd
-- This code is intended for use with the Cypress HOTLink II Video Demostration Board
-------------------------------------------------------------------------------------
--
-- Serial Peripheral Interface
-- PSoC sends 3 bytes.  If bit 7 of first byte is '1' then it is a read operation
-- If bit 7 of first byte is a '1' the it is a write operation

ENTITY spi IS
	PORT(
		scs, scl, sdo		: IN	STD_LOGIC;	-- SPI inputs from PSoC
		sdi					: OUT	STD_LOGIC;	-- SPI data output to PSoC
		read, write 		: OUT	STD_LOGIC;	-- read and write signals to channel registers
		channel				: OUT	STD_LOGIC;	-- channel select output
		address				: BUFFER	STD_LOGIC_VECTOR(7 DOWNTO 0);	-- address output to channel registers
		data_out			: BUFFER	STD_LOGIC_VECTOR(7 DOWNTO 0);	-- data output to channel registers
		data_in				: IN	STD_LOGIC_VECTOR(7 DOWNTO 0)		-- data input from channel registers
	);
END spi;

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
-- tpgen.vhd
-- This code is intended for use with the Cypress HOTLink II Video Demostration Board
-------------------------------------------------------------------------------------
--
--	Top level file for SDI test pattern generator
--
--	txsrc selects one of four available test patterns
-- 		0 = eg1 colour bars
--		1 = grey pattern
--		2 = r178 pathological test
--		3 = rp178 alternate pathological test
--		others = other transmit sources not supported by this pattern generator
--
--	txrate indicates the transmit clock rate
-- 		0 = 27 mhz
--		1 = 36 mhz
--		2 = 54 mhz
--		3 = 74.25 mhz

LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;
LIBRARY lpm;
USE lpm.lpm_components.all;

ENTITY tpgen IS
	PORT(
		reset_n, clk, pal_nntsc		: IN	STD_LOGIC;			-- active low reset, transmit clock, pal/ntsc select
		txsrc			: IN	STD_LOGIC_VECTOR(2 DOWNTO 0);	-- transmit source select input
		txrate			: IN	STD_LOGIC_VECTOR(1 DOWNTO 0);	-- transmit rate input
		data_out		: OUT STD_LOGIC_VECTOR(9 DOWNTO 0)		-- video data out
	);
END tpgen;

	COMPONENT eg1

	COMPONENT grey

	COMPONENT rp178

	COMPONENT edh

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
-- upconvert.vhd
-- This code is intended for use with the Cypress HOTLink II Video Demostration Board
-------------------------------------------------------------------------------------
--
-- This module converts SDI input to HD-SDI output
-- 1080i at 29.97 frames/sec is supported on output for NTSC 270 Mbit/s input
-- clk27 must be syncronous with data_in, clk74 must be derived from clk27 using PLL's
-- for clk27 = 27MHz, clk74 = 74.25/1.001 MHz
--
-- Incoming SDI data is stored in up to 7 line buffers.
-- HDSDI line data is generated by repeating each pixel sample pair twice as follows: 
--   CRnYnCBnYn+1 -> CRnY2nCBnYn+1 CRn+1Yn+1CBn+1Yn+1
-- Lines are repeated as follows:
--    For each 7 lines of SDI data in, the first 6 lines are repeated twice and the 7th line is repeated 3 times.  
--    This generates 15 lines of HDSDI data for each 7 lines of SDI data in. 
--    This stretches the image vertically by about 7% but matches the average line and frame rates for incoming and outgoing data.
-- Unused active video samples in active outgoing video lines are set to black.  
--    This will result in black bars at the sides of the video image.  
--    Some active video lines at the top and bottom of the HDSDI output are not used, they are also set to black. 
--
-- Clocking:
--
-- In order for the up conversion to work I discovered I needed to use both PLL's in the FPGA(U2) connected 
-- in series to get the correct frequency for transmitting.
 
-- The RXCLKB+ input pin of the FPGA is connected to the input of the first PLL in the FPGA. 
-- The FPGA connects the output of the first PLL to  FCLKB+ internally. 
-- The jumper wire connects FCLKB+ output pin of the FPGA to the  RXCLKA+ input pin on the FPGA(U2). 
-- R205 is removed in order to disconnect the RXCLKA+ output pin of the 15G403 chip from the RXCLKA+ input pin of the FPGA(U2). 
-- The RXCLKA+ pin on the FPGA is connected to the input of the second PLL in the FPGA. 
-- The output of the second PLL is connected to FCLKA+ in the FPGA and the inverted output of the PLL is connected to the FCLKA- in the FPGA. 
-- Moving the jumpers on JP11 connects the FCLKA outputs of the FPGA to the REFCLKA inputs of the 15G0403. 
-- The first PLL multiplies  RXCLKB+ by 25/13, The second multiples this result by 10/7.  the resulting frequency is 74.25/1.001 MHz
-- the resulting clock circuit looks like this:
-- 
-- 
--               |------|          |------|  FCLKA+   |------|  REFCLKA+       
--               |      |          |      |---------->|      |----------> 
--  RXCLKB+      |      |  FCLKB+  |      |           |      |        
--  ------------>| PLL1 |--------->| PPL2 |  FCLKA-   | JP11 |  REFCLKA-
--               |      |          |      |---------->|      |----------> 
--               |      |          |      |           |      |        
--               |------|          |------|           |------|

LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;
USE work.pack_crc.all;

ENTITY upconvert IS
	PORT(
		clk27, clk74, reset		: IN	STD_LOGIC;
		data_in					: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);
		line_count				: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);
		sample_count			: IN	STD_LOGIC_VECTOR(10 DOWNTO 0);
		video_type				: IN	STD_LOGIC_VECTOR(2 DOWNTO 0);
		data_out				: out	STD_LOGIC_VECTOR(19 DOWNTO 0)
	);
END upconvert;

	component line_buff

--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
-- video_detect.vhd
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

-- NO COMPONENTS