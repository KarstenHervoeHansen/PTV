--File:  testbench.vhd 
-------------------------------------------------------------------------------
--**************************************************************************
-- 
--     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
--     SOLELY FOR USE IN DEVELOPING PROGRAMS AND SOLUTIONS FOR
--     XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION
--     AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, APPLICATION
--     OR STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS
--     IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
--     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
--     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
--     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
--     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
--     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
--     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
--     FOR A PARTICULAR PURPOSE.
--
--     (c) Copyright 2003 Xilinx, Inc.
--     All rights reserved.
--
--**************************************************************************
-------------------------------------------------------------------------------
-- Revision 1.0
--
--     Modification History:                                                   
--     Date     Init          Description                                
--   --------  ------  --------------------------------------------------------
--   10/29/2003   ST     Initial release.
-------------------------------------------------------------------------------

LIBRARY ieee;
  USE ieee.std_logic_1164.ALL;
  USE ieee.numeric_std.ALL;
  
library UNISIM;
use UNISIM.VComponents.all;

ENTITY test IS
END test;

ARCHITECTURE behavior OF test IS 

-- Component Declarations:
COMPONENT stimuli                      
port(                                           
	reset	            : in std_logic;              
	clk	                : in std_logic;            
	enable				: in std_logic;
	data_valid  		: out  std_logic;                                               
	data_out			: out  std_logic_vector(31 downto 0)
);
END COMPONENT;

component fifo_controller 
port(    
	reset	            : in std_logic;
	clk	                : in std_logic;
	
	data_in 			: in  std_logic_vector(31 downto 0);
	output_enable		: in  std_logic;
	data_valid			: in  std_logic;
	data_out			: out std_logic_vector(19 downto 0);
	ram_req				: out std_logic
);
end component;


-- Signal Declarations:
   signal clkin_P       :  std_logic := '1';
   signal clkin_N       :  std_logic := '0';
   signal RST           :  std_logic := '0'; 
   signal fifo_in       :  std_logic_VECTOR(31 downto 0);        
   signal req		    :  std_logic;                                                   
   signal fifo_out      :  std_logic_VECTOR(19 downto 0); 
   signal en,en_reg			:  std_logic;
   signal ram_data_ready:  std_logic;
BEGIN

  -- Component Instantiation
 uut: fifo_controller port map
	( reset     		=> RST,             
	  clk       		=> clkin_P,             
	  data_in       	=> fifo_in,		     
	  output_enable		=> en_reg,  
	  data_valid		=> ram_data_ready,  
	  data_out 			=> fifo_out,		     
	  ram_req  			=> req		 
	  );

s_inst: stimuli port map(         
	reset	   		=>  RST,
	clk	       		=>  clkin_P,
	enable	   		=>  req,	      
	data_valid		=> ram_data_ready,      
	data_out  	 	=>  fifo_in
);   

  clkin_P <= not clkin_P after 3.34 ns ;
  clkin_N <= not clkin_N after 3.34 ns ;
  
  -- RST must be asserted for a minimum of 4 clocks or DCM won't LOCK!
  RST     <= '1' after 300 ns;
  
  en 	  <= '0' after 0 ns,
  			 '1' after 500 ns,
  			 '0' after 2131 ns,
  			 '1' after 2291 ns;
  			-- '0' after 1298 ns,
  			-- '1' after 1344 ns;
  
  reg_en: process(clkin_P)
  begin
  	if clkin_P'event and clkin_P = '1' then
  		en_reg <= en;
  	end if;
  end process;			 
  END behavior;
