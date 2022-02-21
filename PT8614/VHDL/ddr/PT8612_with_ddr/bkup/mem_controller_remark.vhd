library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- pragma translate_off
library UNISIM;use UNISIM.VCOMPONENTS.ALL;
-- pragma translate_on
--

entity mem_controller is
port(         
		reset	            	: in std_logic;
		clk	                	: in std_logic;
		clk90                   : in std_logic;
		clk180					: in std_logic;
		enable_i				: in std_logic;
		sync_reset_i           	: in std_logic;
		burst_done   			: out std_logic;
		
		user_input_address  	: out std_logic_vector(22 downto 0);
		user_bank_address   	: out std_logic_vector(1 downto 0);
		user_input_data  		: out std_logic_vector(31 downto 0);
		user_data_mask   		: out std_logic_vector(3 downto 0);
		
		auto_ref_req			: in std_logic;
		ar_done          		: in std_logic;
		user_cmd_ack     		: in std_logic;
		user_data_valid  		: in std_logic;
		user_output_data 		: in std_logic_vector(31 downto 0);
		init_val         		: in std_logic;
		
		fvh_i		 			: in  std_logic_vector(2 downto 0);
		luma_a_o				: out std_logic_vector(9 downto 0);
		chroma_a_o				: out std_logic_vector(9 downto 0);
		luma_b_o				: out std_logic_vector(9 downto 0);  
		chroma_b_o				: out std_logic_vector(9 downto 0);
		user_config_register1	: out std_logic_vector(14 downto 0);
		user_config_register2	: out std_logic_vector(12 downto 0); 
		user_command_register	: out std_logic_vector(3 downto 0);
		TP1						: out std_logic;
		TP2						: out std_logic;
		TP3						: out std_logic;
		TP_port 				: out std_logic_vector(21 downto 0)
	);
end mem_controller;

architecture   arc_controller of mem_controller is 

component fifo_controller  
port(    
	reset	            : in std_logic;
	clk	                : in std_logic;	
	enable_i			: in std_logic;
	data_in 			: in  std_logic_vector(31 downto 0);
	output_enable		: in  std_logic;
	data_valid  		: in  std_logic;
   ram_data_valid: in std_logic;
	fifo_reset : in std_logic;
	data_out			: out std_logic_vector(35 downto 0);
	fifo_space			: out std_logic
);
end component;

type state_type is(S_INIT, S_MEM_INIT1,S_MEM_INIT2,S_MEM_READ1,S_MEM_READ2,S_MEM_READ_START,S_MEM_READ_END,S_MEM_WRITE_START,S_MEM_WRITE1,S_MEM_WRITE2,S_MEM_WRITE_END,S_MEM_IDLE,S_MEM_PAUSE);
type data_array is array(0 to 11) of std_logic_vector(15 downto 0);
type valid_option_array is array(0 to 15) of std_logic_vector(1 downto 0);
constant red : data_array :=
(X"4416",X"83c0",X"1441",X"683c",X"0144",X"1683",X"c014",X"4168",X"3c01",X"4416",X"83c0",X"1441");                      
signal current_state, next_state: state_type;
signal next_reg3,current_reg3 : std_logic_vector(3 downto 0);
signal next_loop_count, loop_count: integer range 0 to 4096;
signal next_addr,current_input_address : std_logic_vector(23 downto 0);
signal next_input_data, current_input_data : std_logic_vector(31 downto 0);
signal next_burst_done,current_burst_done : std_logic;
signal test_count : integer range 0 to 127;
signal test_data, next_test_data : integer range 0 to 15; --std_logic_vector(31 downto 0);
signal compare_data: std_logic_vector(31 downto 0);
signal user_data_valid_delayed: std_logic;
signal error_count: std_logic_Vector(21 downto 0);
signal current_ba, next_ba : std_logic_vector(1 downto 0);
signal fifo_space: std_logic;
signal o_enable: std_logic;
signal valid_option, next_valid_option : std_logic_vector(1 downto 0);
signal valid_option_delayed: valid_option_array;
signal add_reset : std_logic;
signal fvh_delayed : std_logic_vector(2 downto 0);
signal video_data_out: std_logic_vector(35 downto 0);
signal cb_cr_mux: std_logic;
signal sync_reset_90: std_logic;
signal data_valid: std_logic;
signal space_delayed: std_logic_vector(15 downto 0);
signal correct_space_delayed: std_logic;
signal space_plus_fvh: std_logic;
signal fvh_clk90: std_logic_vector(2 downto 0);
signal user_cmd_ack_clk90: std_logic;	
signal	init_val_clk90   	: std_logic;
begin

fifo_controller_inst: fifo_controller
port map(
	reset	          => reset,
	clk	              => clk90,
	enable_i		  => enable_i,
	data_in 		  => user_output_data,
	output_enable	  => o_enable,
	data_valid  	  => data_valid,
	ram_data_valid => user_data_valid,
	fifo_reset   	  => add_reset,
	data_out		  => video_data_out,
	fifo_space		  => fifo_space );
	
clk0_reg : process(reset, clk)
begin
	if reset = '0' then
	    user_command_register 	<= (others => '0');
	    user_input_address  	<= (others => '0');  
        burst_done				<= '0';         
        user_bank_address <= "00";
   elsif clk'event and clk = '1' then     
   			user_command_register 	<= current_reg3;
   			user_input_address  	<= current_input_address(22 downto 0);       
   			burst_done				<= current_burst_done;
   			user_bank_address <= current_ba; 
   	end if;
end process;

 

--registers for the state machine, in clk90 domain                    
state_machine_reg: process(reset, clk90)             
begin                                              
	if reset = '0' then                               
 		current_state 			<= S_MEM_INIT1;   
 		current_reg3 			<= (others => '0');                   
 		current_input_address  	<= (others => '0');        
 		current_burst_done		<= '0';                           
 		loop_count				<= 0;                                                               
 		current_input_data		<= (others => '0');          
		test_data				<= 4; 
		current_ba   			<= "00";
		valid_option			<= "00";
		user_cmd_ack_clk90 <= '0';
		init_val_clk90 <= '0';
	elsif clk90'event and clk90 = '1' then              
		current_state 			<= next_state;    
		current_reg3 			<= next_reg3;                    
		current_ba      		<= next_ba;      
		current_input_address  	<= next_addr;               
		current_burst_done  	<= next_burst_done;                
		loop_count				<= next_loop_count;                                    
		current_input_data		<= next_input_data;  
		test_data 				<= next_test_data;	 
		valid_option			<= next_valid_option;  	
		user_cmd_ack_clk90 		<= user_cmd_ack;  
		init_val_clk90 			<= init_val;                                 		                                                 
	end if;                                           
end process;                                       


--register that output data to RAM
CLK90_REG: process(reset, clk90)
begin
	if reset = '0' then
 		user_input_data     <= (others => '0');
	elsif clk90'event and clk90 = '1' then
	  		--if user_cmd_ack_clk90 = '1' and 
	  		if next_reg3 = "0100" then			--only output in the write operations
	  			user_input_data <= current_input_data;
      		end if;
	end if;
end process;
	
-- config_register1 = {  PD,WR,TM,EMR(Enable/Disable DLL),
--                       BMR (Normal operation/Normal Operation with Reset DLL),
--                       BMR/EMR,
--                       CAS_latency (3),
--                       Burst type ,
--                       Burst_length (3) }
-- config_register2 = {  Out,RDQS,DQS_n,OCD_Progm,Posted_Cas,RTT,ODS }
user_config_register1 <= "001000000110010";  --two mode registers that configure the RAM
user_config_register2 <= "0001110000100";    --doesn't change during read/write operations   

--combinatorial process of the state machine
state_machine_combi: process(add_reset,valid_option,space_delayed, space_plus_fvh,current_ba, current_input_address, current_state,init_val_clk90,loop_count,user_cmd_ack_clk90,user_data_valid,test_data)
begin
	next_reg3 <= "0000";
	next_loop_count <= loop_count;
	next_burst_done <= '0';
	next_state <= current_state;
	next_input_data(15 downto 0) <= red(test_data+1);
	next_input_data(31 downto 16)<= red(test_data);
	next_test_data <= test_data;
	next_addr <= current_input_address;
	next_ba <= current_ba;
	next_valid_option <= valid_option;
	TP1 <= '0';
	TP2 <= add_reset;
	case current_state is
    when S_MEM_INIT1 =>    
		if loop_count = 0 then			--take 2 clock cycle to write mode registers
			next_state <= S_MEM_INIT1;
			next_loop_count <= 1;
		else
			next_state <= S_MEM_INIT2;
			next_loop_count <= 0;
		end if;
		
	when S_MEM_INIT2 =>
		next_reg3 <= "0010";				    --command register: MEM initialization
		if init_val_clk90 = '0' then			--initializtion successful(set by mem controller)
			next_state <= S_MEM_INIT2;
		else
			next_state <= S_MEM_WRITE_START;
		end if;
	
	when S_MEM_WRITE_START =>
		next_reg3 <= "0100";				    --command register: Write
		if user_cmd_ack_clk90 = '0' then		--Write command acknowleged by mem controller 
			next_state <= S_MEM_WRITE_START;  		
			next_input_data(15 downto 0) <= red(test_data-1);    --only for the first two data in each row
			next_input_data(31 downto 16)<= red(test_data-2); 
			next_addr <= current_input_address;	  
		elsif user_cmd_ack_clk90 = '1' and loop_count = 4 then   --write column 00-03 twice to ensure correct data
		   next_loop_count <= 0;
		   next_state <= S_MEM_WRITE1;
		   if test_data = 10 then		--test_data increases as: 4 6 8 10 3 5 7 9 2 4 6 8...
		      next_test_data <= 3;
		   elsif test_data = 9 then
		      next_test_data <= 2;
		   else
		      next_test_data <= test_data + 2;
		   end if;
		   next_input_data(15 downto 0) <= red(test_data+1);
		   next_input_data(31 downto 16)<= red(test_data);
		elsif user_cmd_ack_clk90 = '1' and loop_count < 1 then
		   next_loop_count <= loop_count + 1;
			next_state <= S_MEM_WRITE_START;
			next_input_data(15 downto 0) <= red(test_data-1);
			next_input_data(31 downto 16)<= red(test_data-2);
			next_addr <= current_input_address;	-- row 0 column 0
		elsif user_cmd_ack_clk90 = '1' and loop_count < 4 then
		   next_loop_count <= loop_count + 1;
			next_state <= S_MEM_WRITE_START;
		   if test_data = 10 then
		      next_test_data <= 3;
		   elsif test_data = 9 then
		      next_test_data <= 2;
		   else
		      next_test_data <= test_data + 2;
		   end if;
			next_input_data(15 downto 0) <= red(test_data+1);
			next_input_data(31 downto 16)<= red(test_data);
			next_addr <= current_input_address;	
		end if;
	when S_MEM_WRITE1 =>                           --this state both increases address and outputs data
		if current_input_address(9 downto 0) = "1111111100" then   --last address in the row
		   next_burst_done <= '1'; 								   --signal that ends the write operation
			next_state <= S_MEM_WRITE_END;
			next_input_data(15 downto 0) <= red(test_data+1);
			next_input_data(31 downto 16)<= red(test_data);
			next_addr <= current_input_address + "100";			   --during write_end state, the input_address is the next row start add
		else         										       
			next_input_data(15 downto 0) <= red(test_data+1);
			next_input_data(31 downto 16)<= red(test_data);				
			next_state <= S_MEM_WRITE2;							   --write1, write2 interleaving mode
			next_addr <= current_input_address + "100";			   --each row has 1k addresses, increase by 4 every other time
		   if current_input_address(9 downto 0) = "1111111000" then  --second last (and the last) address, stop increasing test_data
			   next_test_data	<= test_data;
			else
		      if test_data = 10 then
		         next_test_data <= 3;
		      elsif test_data = 9 then
		         next_test_data <= 2;
		      else
		         next_test_data <= test_data + 2;
		      end if;
			end if;

		end if;
		next_reg3 <= "0100";
		when S_MEM_WRITE2 =>				         --this state only outputs data, without changing address
			next_input_data(15 downto 0) <= red(test_data+1);
			next_input_data(31 downto 16)<= red(test_data);				
			next_state <= S_MEM_WRITE1;
			if current_input_address(9 downto 0) = "1111111000" then --second last(and last) address in a row
			   next_test_data	<= test_data;
			else
		      if test_data = 10 then
		         next_test_data <= 3;
		      elsif test_data = 9 then
		         next_test_data <= 2;
		      else
		         next_test_data <= test_data + 2;
		      end if;
			end if;
        next_reg3 <= "0100";
	when S_MEM_WRITE_END =>
		if loop_count < 2 then
			next_burst_done <= '1';					--burst_done high for 2 cycles
			next_state <= S_MEM_WRITE_END;
			next_loop_count <= loop_count + 1;
			next_reg3 <= "0100";
		elsif loop_count < 4 then
			next_burst_done <= '0';				   --burst_done low for 2 cycles before cmd reg deassert
			next_state <= S_MEM_WRITE_END;
			next_loop_count <= loop_count + 1;
			next_reg3 <= "0100";
		elsif (user_cmd_ack_clk90 = '0') and (current_input_address(23 downto 10) /= "00000000000011") then	   --for entire bank, use "10000000000000" 
		   next_state <= S_MEM_WRITE_START;
			next_loop_count <= 0;	
			next_reg3 <= "0100";
			
------------------------- enable for more than 1 bank (simulate before use) -------------------------------
--		elsif (user_cmd_ack_clk90 = '0') and (current_input_address(22 downto 10) = "10000000000000" ) and (current_ba /= "11") then
--		   next_state <= S_MEM_WRITE_START;
--			next_reg3 <= "0100";
--			next_addr <= (others => '0');         --move to next row
--			next_ba <= current_ba + '1';		 --move to next bank
---------------------------------------------------------------------------------------
		elsif user_cmd_ack_clk90 = '0' then     --done with writing
			next_state <= S_MEM_PAUSE;
			next_loop_count <= 0;	
			next_reg3 <= "0000";
		end if;	    
		TP1 <= '1';
	when S_MEM_PAUSE =>							
		if add_reset = '1' then					--one frame finishes, reset the address in RAM, start outputing
			next_state <= S_MEM_READ_START;
			next_loop_count <= 0;
			next_addr <=  (others => '0');
			next_ba <= "00";
		else
			next_state <= S_MEM_PAUSE;
			next_loop_count <= loop_count;
		end if;		
	when S_MEM_READ_START =>
		if add_reset = '1' then
			next_addr <= (others => '0');		--when frame finishes, reset the address
		end if;
		next_reg3 <= "0110";
		if user_cmd_ack_clk90 = '0' then
			next_state <= S_MEM_READ_START;   
		elsif user_cmd_ack_clk90 = '1' and loop_count = 2 then 
			next_state <= S_MEM_READ1;
			next_loop_count <= 0;
		elsif user_cmd_ack_clk90 = '1' and loop_count < 2 then
		   next_state <= S_MEM_READ_START; 
		   next_loop_count <= loop_count + 1;
		end if;		

	when S_MEM_READ1 =>		 --this state increase address and reads in data
		if add_reset = '0' then      
	    	--if current_input_address(9 downto 0) = "1111111100"  and space_plus_fvh = '1'then  --last column address in a row
			--	next_state <= S_MEM_READ_END;
			--	next_addr <= current_input_address;
			--	next_burst_done <= '1';	    
			if current_input_address(9 downto 0) = "0000100000"  and space_plus_fvh = '1'then   --repeat when "0000100000"
				next_state <= S_MEM_READ2;
				next_addr <= (others => '0');
			elsif space_plus_fvh = '1' and space_delayed(0) = '1' then     -- 				
				next_state <= S_MEM_READ2;
				next_addr <= current_input_address + "100";			
			elsif space_plus_fvh = '0' and space_delayed(0) = '1' then     --space change from 1 to 0    					
				next_state <= S_MEM_READ2;								   --no space in FIFO, pause the address increasing	
				next_addr <= current_input_address;
				next_valid_option <= "00";
			elsif space_plus_fvh = '1' and space_delayed(0) = '0' then	   --space in FIFO again
				next_state <= S_MEM_READ2;
				next_addr <= current_input_address + "100";
				next_valid_option(1) <= '0';
			elsif space_plus_fvh = '0' and space_delayed(0) = '0' then     --no space in FIFO
				next_state <= S_MEM_READ2;
				next_addr <= current_input_address;

			end if;
		else
			next_state <= S_MEM_READ_END;
			next_addr <= (others => '0');
		end if;		
     	next_reg3 <= "0110";
    when S_MEM_READ2 =>	             --this state only reads in data
        if add_reset = '1' then 
    		next_addr <= (others => '0');
    		next_state <= S_MEM_READ_END;
    	else		
			next_state <= S_MEM_READ1;    
			next_addr <= current_input_address;    	 
        	if space_plus_fvh = '0' and space_delayed(0) = '1' then 			--space change from 1 to 0   
        		next_valid_option <= "01";
        	elsif space_plus_fvh = '1' and space_delayed(0) = '0' then
        		next_valid_option(1) <= '1';
        end if; 
      end if;
        next_reg3 <= "0110";
    when S_MEM_READ_END =>
    	if add_reset = '1' then 
    		next_addr <= (others => '0');
    		next_state <= S_MEM_READ_END;
    	else
			if loop_count < 2 then
				next_burst_done <= '1';
				next_state <= S_MEM_READ_END;
				next_loop_count <= loop_count + 1;
				next_reg3 <= "0110";
			elsif loop_count < 4 then
				next_burst_done <= '0';
				next_state <= S_MEM_READ_END;
				next_loop_count <= loop_count + 1;
				next_reg3 <= "0110";                                     
			elsif user_cmd_ack_clk90 = '0' and (current_input_address = "000000000000000000000000") then  --receiving add_reset during read1 or read2 state
			   next_state <= S_MEM_READ_START;
				next_loop_count <= 0;	
				next_reg3 <= "0110"; 
			elsif user_cmd_ack_clk90 = '0' and (current_input_address(22 downto 10) /= "1111111111111") then --more rows in the bank
				next_state <= S_MEM_READ_START;
				next_loop_count <= 0;	
				next_reg3 <= "0110";
			    next_addr <= current_input_address + "100";	
			elsif user_cmd_ack_clk90 = '0' and (current_input_address(22 downto 10) = "1111111111111") then --last row in the bank
				next_state <= S_MEM_READ_START;  
				next_loop_count <= 0;	           
				next_reg3 <= "0110";             
		    	next_addr <= (others => '0');  
		    end if;
		 end if;
------------------------- enable for more than 1 bank  -------------------------------		   	
--		elsif user_cmd_ack_clk90 = '0'	and (current_input_address(22 downto 10) = "1111111111111") and (current_ba /= "11") then
--			next_state <= S_MEM_READ_START;
--			next_loop_count <= 0;	
--			next_reg3 <= "0110";
--	      next_addr <= (others => '0');
--		   next_ba <= current_ba + '1';
-----------------------------------------------------------------------------------------
--		elsif user_cmd_ack_clk90 = '0' then 
--			next_state <= S_MEM_IDLE;
--			next_loop_count <= 0;	
--			next_reg3 <= "0000";
--		end if;	  	  
				    
--	when S_MEM_IDLE =>
--		next_addr <= (others => '0');
--		next_input_data <= X"00000000";
--		next_reg3 <= "0000";	
--		next_state <= S_MEM_IDLE;
--		TP_port <= error_count;
--		TP3 <= '1';
	when others =>
		next_state <= S_MEM_INIT1;	
	end case;
end process;

user_data_mask <= (others => '0');  --always set to 0, not masking the data

-- output enable when enable = 1 and v = 0 and h = 0
make_fifo_enable_reg: process(clk90, reset)
begin
    if reset = '0' then
		o_enable <='0';
    elsif clk90'event and clk90 = '1' then
        	o_enable <= enable_i and (not fvh_i(1)); --and (not fvh_i(0))  
	end if;         
end process;    

--processes that de-multiplex the Cb and Cr (check if the first pixel is correct)
cbcr_mux : process(clk90, reset)
begin
	if reset = '0' then	
		cb_cr_mux <= '0';
	elsif clk90'event and clk90 = '1' then
			if fvh_i(0) = '0' and fvh_delayed(0) = '1' then  --start of a new active line
				cb_cr_mux <= '0';
			elsif o_enable = '1' then
				cb_cr_mux <= not cb_cr_mux;
			end if;		
	end if;
end process;

output_mux : process(cb_cr_mux, video_data_out)
begin
	if cb_cr_mux = '1' then
		chroma_a_o <= video_data_out(21 downto 12);  --take CB
	else
		chroma_a_o <= video_data_out(9 downto 0);    --take CR
	end if;
	luma_a_o <= video_data_out(33 downto 24);
end process;

--The add_reset signal is created according to the v signal. It is asserted one clk after v 0->1
address_reset : process(clk90, reset)
begin
	if reset = '0' then
		add_reset <= '0';
		fvh_delayed <= "111";
		fvh_clk90 <= "111";
	elsif clk90'event and clk90 = '1' then
	  		fvh_delayed <= fvh_clk90;
	  		fvh_clk90 <= fvh_i;
			if fvh_clk90(1) = '1' and fvh_delayed(1) = '0' then
				add_reset <= '1';
			else
				add_reset <= '0';
			end if;
	end if;
end process;

--this process delays the space_plus_fvh and the valid_option signal for 15 times to conform with the delayed data from RAM
delay_space_signal_p: process(clk90,reset)
begin
    if reset = '0' then
        space_delayed <= (others=>'0');
        valid_option_delayed <= (others => "00");
    elsif clk90'event and clk90 = '1' then 
        	space_delayed(0) <= space_plus_fvh;
        	valid_option_delayed(0) <= valid_option;
        	for i in 0 to 14 loop
        	   space_delayed(i+1) <= space_delayed(i);
        	   valid_option_delayed(i+1) <= valid_option_delayed(i);
        	end loop;
    end if;
end process;


--this process is to decide which data is valid when a space_delayed signal disables the address increasing
--it depends on when the valid_option signal is started(in read1 or read2) and when it is ended. Four
--situations of starting and end point exist. 
with valid_option_delayed(12) select
	correct_space_delayed <= space_delayed(13) when "00",
							 space_delayed(14) when "11",
							 space_delayed(13) or space_delayed(14) when "01",
							 space_delayed(13) and  space_delayed(14) when others;


data_valid <= correct_space_delayed and user_data_valid;

---- In horizontal blanking, the fifo must be stopped. Here the H is used together with fifo_space signal
---- to stop the address increasing
--make_space_plus_fvh: process(fifo_space, fvh_clk90)
--begin
--    if fvh_clk90(1) = '1' then
       space_plus_fvh <= fifo_space;
--    else      --consider h during not vertical blanking period
--       space_plus_fvh <= fifo_space and (not fvh_clk90(0));							 		
--    end if;
--end process;

-- valid_option definition:
-- 00: space signal 1->0 at M1, 0->1 at M1
-- 01: space signal 1->0 at M2, 0->1 at M1
-- 10: space signal 1->0 at M1, 0->1 at M2
-- 11: space signal 1->0 at M2, 0->1 at M2

-- valid_option               00  
--                        -------------                             -----------------------------
-- space_plus_fvh                      |                           |
--                                      --------------------------        
--                        ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ -
-- current_state            M1  X  M2  X  M1  X  M2  X  M1  X  M2  X  M1  X  M2  X  M1  X  M2  X                   
--                        ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ -
--                        ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ -
-- data                     D0  X  D1  X  D0  X  D1  X  D0  X  D1  X  D2  X  D3  X  D4  X  D5  X                                 
--                        ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ -
--                        -------------                             -----------------------------
-- correct_space_delayed               |                           |
--                                      ---------------------------


-- valid_option               11  
--                        ------                             -----------------------------
-- space_plus_fvh               |                           |
--                               ---------------------------        
--                        ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ -
-- current_state            M1  X  M2  X  M1  X  M2  X  M1  X  M2  X  M1  X  M2  X  M1  X  M2  X                   
--                        ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ -
--                        ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ -
-- data                     D0  X  D1  X  D0  X  D1  X  D0  X  D1  X  D2  X  D3  X  D4  X  D5  X                                 
--                        ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ -
--                        -------------                             -----------------------------
-- correct_space_delayed               |                           |
--                                      ---------------------------


-- valid_option               10  
--                        -------------                                    ----------------------
-- space_plus_fvh                      |                                  |
--                                      ----------------------------------        
--                        ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ -
-- current_state            M1  X  M2  X  M1  X  M2  X  M1  X  M2  X  M1  X  M2  X  M1  X  M2  X                   
--                        ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ -
--                        ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ -
-- data                     D0  X  D1  X  D0  X  D1  X  D0  X  D1  X  D0  X  D1  X  D2  X  D3  X                                 
--                        ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ -
--                        -------------                                           ---------------
-- correct_space_delayed               |                                         |
--                                      -----------------------------------------


-- valid_option               01  
--                        ------                                    -----------------------------
-- space_plus_fvh               |                                  |
--                               ----------------------------------        
--                        ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ -
-- current_state            M1  X  M2  X  M1  X  M2  X  M1  X  M2  X  M1  X  M2  X  M1  X  M2  X                   
--                        ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ -
--                        ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ -
-- data                     D0  X  D1  X  D0  X  D1  X  D0  X  D1  X  D2  X  D3  X  D4  X  D5  X                                 
--                        ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ -
--                        -------------                             -----------------------------
-- correct_space_delayed               |                           |
--                                      ---------------------------



end  arc_controller;