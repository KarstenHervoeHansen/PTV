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
   		CLK_MUX:		OUT 	std_logic_vector(7 downto 0);
   		ACTIVEDATA:		OUT 	std_logic_vector(23 downto 0));
   	END COMPONENT;
   	

	COMPONENT audio_generator
	 Port ( 
        		videoclk_i      : in std_logic;
        		reset_i 			: in std_logic;
       	 	sync_reset_i		: in std_logic;
     		audiodata_i     : in std_logic_vector(23 downto 0);
        		system_i			: in std_logic_vector(7 downto 0);
		audiodata_tick_o: out std_logic; 
        		audiochroma_o   : out std_logic_vector(9 downto 0);
        		audioluma_o 		: out std_logic_vector(9 downto 0) );  
	end COMPONENT;	
signal clk, reset, sync_reset, enable: std_logic;
signal activedata : 	std_logic_vector(23 downto 0); 
signal line : std_logic_vector(10 downto 0);
signal mux: std_logic_vector(7 downto 0);
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
   	ACTIVEDATA=> activedata );
	
audio_test: audio_generator
	PORT MAP(
        videoclk_i      => clk, 
        reset_i 			=> reset,   
        sync_reset_i		  => sync_reset,	 
     	audiodata_i     => activedata,    
        system_i			=> mux,
	audiodata_tick_o => open, 
        audiochroma_o   => open,
        audioluma_o 		=>open
        );
        

END a;


CONFIGURATION test OF testbench IS   
    FOR a
        FOR sti: stimuli
            USE ENTITY WORK.stimuli(behavioral)
            GENERIC MAP (period => 6.7 ns);
        END FOR;
        
        FOR audio_test : audio_generator
        	   USE ENTITY work.audio_generator(behavioral);
        END FOR;
    END FOR;
END test;

