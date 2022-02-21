----------------------------------------------- 
-- CLOCK_MUX.VHD Version 1.1                 -- 
-- This is an example of an instantiation of -- 
-- global buffer (BUFGS) from an internally  -- 
-- driven signal, a multiplexed clock.       -- 
-- March 2001                                -- 
----------------------------------------------- 

library IEEE; 
use IEEE.std_logic_1164.all; 

entity clock_mux is 
    port(
        DATA, SEL; in STD_LOGIC; 
        SLOW_CLOCK, FAST_CLOCK: in  STD_LOGIC; 
        DOUT: out STD_LOGIC
        ); 
end clock_mux; 

architecture XILINX of clock_mux is 

signal CLOCK:      STD_LOGIC; 
signal CLOCK_GBUF: STD_LOGIC; 

component BUFGS  
    port (
        I : in  STD_LOGIC;  
        O : out STD_LOGIC
        ); 
end component; 

begin 

GBUF_FOR_MUX_CLOCK : BUFGS 
    port map(
        I => CLOCK, 
        O => CLOCK_GBUF
        ); 

Clock_MUX: process (SEL, FAST_CLOCK, SLOW_CLOCK) 
begin 
    if (SEL = '1') then  
        CLOCK <= FAST_CLOCK; 
    else 
        CLOCK <= SLOW_CLOCK; 
    end if; 
end process; 

Data_Path: process (CLOCK_GBUF)     
begin 
    if (CLOCK_GBUF'event and CLOCK_GBUF='1')then 
        DOUT <= DATA; 
    end if; 
end process; 

end XILINX;