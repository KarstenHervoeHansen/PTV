library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- pragma translate_off
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
-- pragma translate_on
--

entity fifo_controller is
port(    
	reset	            : in std_logic;
	clk	                : in std_logic;	
	data_in 			: in  std_logic_vector(31 downto 0);
	output_enable		: in  std_logic;
	data_valid  		: in  std_logic;
	
	data_out			: out std_logic_vector(35 downto 0);
	ram_req				: out std_logic
);
end fifo_controller;

architecture arc_fifo of fifo_controller is 	

constant fifo1size: integer := 30;

type state_type is (S0,S1,S2,S3,S4,S5,S6,S7,S8,S9,S10,S11,S12,S13,S14,S15);
type fifo2 is array(0 to 2) of std_logic_vector(63 downto 0); 
type fifo1 is array(0 to fifo1size-1) of std_logic_vector(31 downto 0);
signal current_state, next_state: state_type;
signal next_data_out : std_logic_vector(35 downto 0);
signal current_read, next_read: std_logic;
signal fifo1_data: fifo1;
signal fifo2_data: fifo2; 
signal fifo2_index, next_index: integer range 0 to 7;
signal fifo1_output: std_logic_vector(63 downto 0);
signal fifo1_req : std_logic;
signal fifo1_read_index, fifo1_write_index: integer range 0 to 31;
signal wait_next_data: std_logic;
signal init_ctr,init_ctr_delayed: integer range 0 to 31;
signal fifo1_init: std_logic;
signal fifo1_space: std_logic;
signal fifo1carry: std_logic;
signal delay_space_signal: std_logic_vector(15 downto 0);
signal final_valid: std_logic;
signal data_in_reg: std_logic_vector(31 downto 0);

begin

fifo1_read_process: process (clk, reset)
begin
	if reset = '0' then	
		fifo1_read_index <= 0;
		fifo1_output <= (others => '0');
	elsif clk'event and clk = '1' then
	  if output_enable = '1' then
		if fifo1_init = '0' then	
		 	fifo1_read_index  <= 0;
		elsif fifo1_req = '1' then
			fifo1_output(31 downto 0) <= fifo1_data(fifo1_read_index); 
			fifo1_output(63 downto 32) <= fifo1_data(fifo1_read_index+1); 
			if fifo1_read_index = fifo1size-2 then
				fifo1_read_index <= 0;
			else
				fifo1_read_index <= fifo1_read_index + 2;
			end if;
		end if;
     end if;
	end if;

end process;

fifo1_write_process: process(clk,reset)
begin
	if reset = '0' then	
		wait_next_data <= '0';
		fifo1_write_index <= 0;
		fifo1_data <= (others => X"00000000");
		init_ctr <= 0;
		init_ctr_delayed <= 0;
		fifo1_init <= '0';
	elsif clk'event and clk = '1' then
	    data_in_reg <= data_in;
		if fifo1_init = '0' and final_valid = '1' then				--initialize process
		 	fifo1_data(init_ctr) <= data_in_reg;
		 --	init_ctr <= init_ctr + 1;
		 --	init_ctr_delayed <= init_ctr;
		 --	if init_ctr = 19 then
		 		fifo1_init <= '1' ; 
		 		fifo1_write_index <= fifo1_write_index + 1;
		 --	end if;			
		elsif final_valid = '1' and fifo1_read_index /= fifo1_write_index  then																	  	
			fifo1_data(fifo1_write_index) <= data_in_reg;			
			if fifo1_write_index >= fifo1size-1 then
				fifo1_write_index <= 0;
			else
				fifo1_write_index <= fifo1_write_index + 1;
			end if;
		end if;
	end if;
end process;	

fifo1_index_control: process (fifo1carry, init_ctr, fifo1_init,fifo1_read_index, fifo1_write_index)
begin			
--	if fifo1_init = '0' and init_ctr <= 6 then
--		fifo1_space <= '1';
	if fifo1_read_index-fifo1_write_index >= 14 and fifo1carry = '1' then
	   fifo1_space <= '1';
	elsif fifo1_write_index - fifo1_read_index < 17 and fifo1carry = '0' then
		fifo1_space <= '1';
	else
		fifo1_space <= '0';
	end if;
end process;			

fifo1_carry: process( clk, reset)
begin
    if reset = '0' then
        fifo1carry <= '0';
    elsif clk'event and clk = '1' then
        if fifo1_read_index = fifo1size-2 then
           fifo1carry <= '0' ;
        elsif fifo1_write_index = fifo1size-1 then
            fifo1carry <= '1';
        end if;
     end if;
end process;

ram_req <= fifo1_space;

fifo2_reg: process(clk, reset)
begin
    if reset = '0' then	
        current_state <= S0;
        fifo2_data <= (others => X"0000000000000000");
        data_out <=(others => '0');
        current_read <= '0';
    elsif clk'event and clk = '1' then
    	if output_enable = '1' and fifo1_init = '1' then
    		current_state <= next_state;
    		if next_read = '1' then
    			fifo2_data(next_index) <= fifo1_output;
    		end if;
    		data_out <= next_data_out;	    		
    		fifo2_index <= next_index;
    	end if;
    	current_read <= next_read;
    end if;
end process;

fifo1_req <= next_read and output_enable;

fifo2_combi: process(current_state,fifo2_data)
begin
	next_index <= 0;
		case current_state is
		when S0 =>
			next_data_out <= fifo2_data(0)(35 downto 0);
			next_state <= S1;
			next_read <= '0';
		when S1 =>
			next_data_out <= fifo2_data(1)(7 downto 0) & fifo2_data(0)(63 downto 36); 
			next_state <= S2;
			next_read <= '1';
			next_index <= 0;
		when S2 =>
			next_data_out <= fifo2_data(1)(43 downto 8);
			next_state <= S3;
			next_read <= '0';
		when S3 =>
			next_data_out <= fifo2_data(2)(15 downto 0) & fifo2_data(1)(63 downto 44);
			next_state <= S4;
			next_read  <= '1';
			next_index <= 1;
		when S4 =>
			next_data_out <= fifo2_data(2)(51 downto 16);
			next_state <= S5;
			next_read  <= '0';
		when S5 =>
			next_data_out <= fifo2_data(0)(23 downto 0) & fifo2_data(2)(63 downto 52);
			next_state <= S6;
			next_read  <= '1';
			next_index <= 2;	
		when S6 =>
			next_data_out <= fifo2_data(0)(59 downto 24); 
			next_state <= S7;
			next_read  <= '0';				
		when S7 =>
			next_data_out <= fifo2_data(1)(31 downto 0)& fifo2_data(0)(63 downto 60);
			next_state <= S8;
			next_read  <= '1';
			next_index <= 0;
		when S8 =>
			next_data_out <= fifo2_data(2)(3 downto 0) & fifo2_data(1)(63 downto 32);
			next_state <= S9;
			next_read <= '1';
			next_index <= 1;
		when S9 =>
			next_data_out <= fifo2_data(2)(39 downto 4); 
			next_state <= S10;
			next_read <= '0';			
		when S10 =>
			next_data_out <= fifo2_data(0)(11 downto 0) & fifo2_data(2)(63 downto 40);
			next_state <= S11;
			next_read <= '1';
			next_index <= 2;
		when S11 =>
			next_data_out <=  fifo2_data(0)(47 downto 12);
			next_state <= S12;
			next_read  <= '0';			
		when S12 =>
			next_data_out <= fifo2_data(1)(19 downto 0) & fifo2_data(0)(63 downto 48);
			next_state <= S13;
			next_read  <= '1';
			next_index <= 0;
		when S13 =>
			next_data_out <= fifo2_data(1)(55 downto 20) ;
			next_state <= S14;
			next_read  <= '0';
		when S14 =>
			next_data_out <= fifo2_data(2)(27 downto 0)& fifo2_data(1)(63 downto 56); 
			next_state <= S15;
			next_read  <= '1';	
			next_index <= 1;			
		when S15 =>
			next_data_out <= fifo2_data(2)(63 downto 28);
			next_state <= S0;
			next_read  <= '1';
			next_index <= 2;
		end case;
end process;

delay_space_signal_p: process(clk,reset)
begin
    if reset = '0' then
        delay_space_signal <= (others=>'0');
    elsif clk'event and clk = '1' then
        delay_space_signal(0) <= fifo1_space;
        for i in 0 to 14 loop
           delay_space_signal(i+1) <= delay_space_signal(i);
        end loop;
    end if;
end process;

final_valid <= data_valid and delay_space_signal(13);
end arc_fifo;
