library ieee;                                   
use ieee.std_logic_1164.all;                    
use ieee.std_logic_arith.all;                   
use ieee.std_logic_unsigned.all;                
                                                
-- pragma translate_off                         
library UNISIM;use UNISIM.VCOMPONENTS.ALL;      
-- pragma translate_on                          
--                                              
                                                
entity stimuli is                       
port(                                           
	reset	            : in std_logic;              
	clk	                : in std_logic;            
	enable				: in std_logic;
	data_valid			: out  std_logic;                                               
	data_out			: out  std_logic_vector(31 downto 0)
);
end stimuli;

architecture a of stimuli is
signal counter: std_logic_vector(15 downto 0);
signal next_enable : std_logic;
begin

count_process: process(reset,clk)
begin
	if reset = '0' then
		counter  <= (others => '0');
		data_out <= (others => '0');
--		next_enable <= '0';
		data_valid <= '0';
	elsif clk'event and clk = '1' then
--		if next_enable = '1' then
--			next_enable <= '0';
--			counter <= counter + '1';
--			data_out <= counter & counter;
--			ram_data_ready <= '1';
		if enable = '1' then	
			counter <= counter + '1';
		--	if counter = 256 then
			    data_valid <= '1';
		--	end if;
			data_out <= counter & counter;
--			next_enable <= '1';			
		end if;
	end if;
end process;

end a;
