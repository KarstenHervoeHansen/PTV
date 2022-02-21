library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY stimuli IS
GENERIC (period:    TIME := 6.7 ns);
PORT
(
	CLK :	 	OUT 	std_logic;
  	RESET:		OUT     std_logic;
   	SYNC_RESET :    OUT		   std_logic;
   	CLK_MUX:		OUT 	std_logic_vector(2 downto 0);
--  	ENABLE:		OUT     std_logic;
--   	LINE:		OUT     std_logic_vector(10 downto 0);
--   	FVH :		OUT     std_logic_vector(2 downto 0);
   	ACTIVEDATA:		OUT 	std_logic_vector(9 downto 0)
 );
 END stimuli;
 
 ARCHITECTURE behavioral of stimuli IS
 signal enable_s, clk_s, reset_s: std_logic;
 BEGIN
 
 PROCESS       
    BEGIN
        clk_s <= '1', '0' AFTER period/2;
        WAIT FOR period;
 END PROCESS;
  
-- PROCESS 
--   BEGIN 	
--   	FVH <= "001", "000" AFTER 14300 ns;
--   	WAIT FOR 28600 ns;
-- END PROCESS;
 
 CLK <= clk_s;
 CLK_MUX		<= "000";
 --FVH             <= "000" AFTER 0 ns, "001" AFTER 7370 ns, "000" AFTER 14740 ns; 
-- LINE		<= "00000000000" AFTER 0 ns, "00000001000" AFTER 57.2 us, "00000000000" AFTER 85us;      
 RESET		 <= '0' AFTER 0 ns, '1' AFTER 1 ns;
 SYNC_RESET      <=	'1' AFTER 0 ns, '0' AFTER 1 ns;
 ACTIVEDATA      <=  "1010101010" AFTER 0 ns;
  
 END behavioral;