LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;
library unisim;
use unisim.vcomponents.all;

Entity testbench IS
END testbench;

ARCHITECTURE a OF testbench IS
	COMPONENT stimuli
		PORT(
   		CLK :	 	OUT 	std_logic;
   		RESET:		OUT     std_logic;
   		SYNC_RESET :    OUT		   std_logic;
   		CLK_MUX:		OUT 	std_logic_vector(2 downto 0);
--   	ENABLE:		OUT     std_logic;
--   	LINE:		OUT     std_logic_vector(10 downto 0);
--   		FVH :		OUT     std_logic_vector(2 downto 0); 
   		ACTIVEDATA:		OUT 	std_logic_vector(9 downto 0));
   	END COMPONENT;
   	

	COMPONENT audio_clk
	 Port ( 
	   clkmux_in 	   	    : in std_logic_vector(2 downto 0);		                                                            																   
       videoclk_in     			: in std_logic;
--     enable_in			      : in std_logic;
       reset_in 			      : in std_logic;
	   sync_reset_in		    : in std_logic;
--	   activedata_in      : in std_logic_vector(9 downto 0);
--	   fvh_i              : in std_logic_vector(2 downto 0);  
--	   line_o             : in std_logic_vector(10 downto 0);    
       audiochroma_out	     : out std_logic_vector(9 downto 0);
       audioluma_out	     : out std_logic_vector(9 downto 0);
       audioclk_out 	    	: out std_logic);      
	end COMPONENT;	
signal clk, reset, sync_reset, enable: std_logic;
signal activedata : 	std_logic_vector(9 downto 0); 
signal line : std_logic_vector(10 downto 0);
signal mux: std_logic_vector(2 downto 0);
signal fvh : std_logic_vector(2 downto 0);
signal clockout: std_logic;
signal dataout: std_logic_vector(9 downto 0);
signal counterout: std_logic_vector(11 downto 0); 
BEGIN


sti:stimuli
     PORT MAP(
   	CLK  => clk,
   	RESET => reset,
   	SYNC_RESET  => sync_reset,
   	CLK_MUX => mux,
--   	ENABLE => enable,
--   	LINE => line,
--   	FVH => fvh,
   	ACTIVEDATA=> activedata );
	
audio_test: audio_clk 
	PORT MAP(
	   clkmux_in 	       => mux,	                                                            																   
       videoclk_in       => clk,   		
--     enable_in			     => enable,    
       reset_in 			     => reset,   
	   sync_reset_in    	=> sync_reset,	   
--	   activedata_in     => activedata,    
--	   fvh_i             => fvh,
---	   line_o            => line,
       audiochroma_out	    => open,
 	   audioluma_out		=> open,
       audioclk_out 	    => open
	);	


END a;


CONFIGURATION test OF testbench IS   
    FOR a
        FOR sti: stimuli
            USE ENTITY WORK.stimuli(behavioral)
            GENERIC MAP (period => 6.7 ns);
        END FOR;
        
        FOR audio_test : audio_clk
        	   USE ENTITY work.audio_clk(structure);
        END FOR;
    END FOR;
END test;

