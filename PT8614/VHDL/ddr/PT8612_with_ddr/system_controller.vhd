library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- pragma translate_off
library UNISIM;use UNISIM.VCOMPONENTS.ALL;
-- pragma translate_on
--

entity system_controller is
port(         
		reset	            	: in std_logic;
		clk	                	: in std_logic;
		clk90                   : in std_logic;
		clk180					: in std_logic;
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
		
		output_enable 			: in  std_logic;
		video_data_out			: out std_logic_vector(35 downto 0);
		
		user_config_register1	: out std_logic_vector(14 downto 0);
		user_config_register2	: out std_logic_vector(12 downto 0); 
		user_command_register	: out std_logic_vector(3 downto 0);
		TP1						: out std_logic;
		TP2						: out std_logic;
		TP3						: out std_logic;
		TP_port 				: out std_logic_vector(21 downto 0)
	);
end system_controller;

architecture   arc_controller of system_controller is 

component fifo_controller  
port(    
	reset	            : in std_logic;
	clk	                : in std_logic;	
	data_in 			: in  std_logic_vector(31 downto 0);
	output_enable		: in  std_logic;
	data_valid  		: in  std_logic;
	
	data_out			: out std_logic_vector(35 downto 0);
	ram_req				: out std_logic
);
end component;

type state_type is(S_INIT, S_MEM_INIT1,S_MEM_INIT2,S_MEM_READ1,S_MEM_READ2,S_MEM_READ_START,S_MEM_READ_END,S_MEM_WRITE_START,S_MEM_WRITE,S_MEM_WRITE_END,S_MEM_IDLE,S_MEM_PAUSE);
type data_array is array(0 to 11) of std_logic_vector(15 downto 0);
constant red : data_array :=
(X"4416",X"83c0",X"1441",X"683c",X"0144",X"1683",X"c014",X"4168",X"3c01",X"4416",X"83c0",X"1441");                      
signal current_state, next_state: state_type;
signal next_reg1,current_reg1 : std_logic_vector(14 downto 0);
signal next_reg2,current_reg2 : std_logic_vector(12 downto 0);
signal next_reg3,current_reg3 : std_logic_vector(3 downto 0);
signal next_loop_count, loop_count: integer range 0 to 4096;
signal next_addr,current_input_address : std_logic_vector(22 downto 0);
signal next_input_data, current_input_data : std_logic_vector(31 downto 0);
signal next_burst_done,current_burst_done : std_logic;
signal intern_reg0, intern_reg1, intern_reg2, intern_reg3: std_logic_vector(31 downto 0);
signal col_addr,next_col_addr: std_logic_vector(7 downto 0);
signal test_count : integer range 0 to 127;
signal test_data, next_test_data : integer range 0 to 15; --std_logic_vector(31 downto 0);
signal compare_data: std_logic_vector(31 downto 0);
signal user_data_valid_delayed: std_logic;
signal error_count: std_logic_Vector(21 downto 0);
signal current_ba, next_ba : std_logic_vector(1 downto 0);
signal ram_req: std_logic;
signal o_enable: std_logic;
signal addr_reverse, next_addr_reverse : std_logic;
begin

--state_machine_reg: process(reset, clk)
--begin
--	if reset = '0' then
-- 		current_state 			<= S_MEM_INIT1;
-- 		user_config_register1	<= (others => '0');
-- 		user_config_register2 	<= (others => '0');
-- 		user_command_register 	<= (others => '0');
-- 		user_input_address  	<= (others => '0');
-- 		burst_done				<= '0';        
-- 		loop_count				<= 0;        
-- 		col_addr <= (others => '0');
-- 		intern_reg0 		<= (others => '0');
-- 		intern_reg1 		<= (others => '0');
-- 		intern_reg2 		<= (others => '0');
-- 		intern_reg3 		<= (others => '0');   
-- 		current_input_data	<= (others => '0');  
--	elsif clk'event and clk = '1' then
--		current_state 			<= next_state;
--		user_config_register1 	<= next_reg1;
--		user_config_register2 	<= next_reg2;
--		user_command_register 	<= next_reg3;
--		user_input_address  	<= next_addr;
--		burst_done				<= next_burst_done;
--		loop_count				<= next_loop_count;
--		col_addr <= next_col_addr;
--		current_input_data		<= next_input_data;
--		if user_data_valid = '1' then		
--			intern_reg0 	<= user_output_data;                                         	
--			intern_reg1 	<= intern_reg0;                                              	
--			intern_reg2 	<= intern_reg1;                                              	
--			intern_reg3 	<= intern_reg2;       
--		end if;		
--		
--	end if;
--end process;
fifo_controller_inst: fifo_controller
port map(
	reset	          => reset,
	clk	              => clk90,
	data_in 		  => user_output_data,
	output_enable	  => o_enable, -- output_enable,
	data_valid  	  => user_data_valid,
	data_out		  => video_data_out,
	ram_req			  => ram_req );
	
clk0_reg : process(reset, clk)
begin
	if reset = '0' then
	    user_config_register1	<= (others => '0'); 
	    user_config_register2 	<= (others => '0');
	    user_command_register 	<= (others => '0');
	    user_input_address  	<= (others => '0');  
        burst_done				<= '0';         
       user_bank_address <= "00";
   elsif clk'event and clk = '1' then     
   		user_config_register1 	<= current_reg1;     
   		user_config_register2 	<= current_reg2;
   		user_command_register 	<= current_reg3;
   		user_input_address  	<= current_input_address;       
   		burst_done				<= current_burst_done;
   		user_bank_address <= current_ba; 
   end if;
end process;
       
                    
state_machine_reg: process(reset, clk90)             
begin                                              
	if reset = '0' then                               
 		current_state 			<= S_MEM_INIT1;                
 		current_reg1			<= (others => '0');       
 		current_reg2 			<= (others => '0');      
 		current_reg3 			<= (others => '0');      
 		current_input_address  	<= (others => '0');        
 		current_burst_done		<= '0';                           
 		loop_count				<= 0;                             
 		col_addr <= (others => '0');                    
 		intern_reg0 			<= (others => '0');               
 		intern_reg1 			<= (others => '0');               
 		intern_reg2 			<= (others => '0');               
 		intern_reg3 			<= (others => '0');               
 		current_input_data		<= (others => '0');          
		test_data				<= 4; 
		current_ba   <= "00";
		addr_reverse  <= '0';
	elsif clk90'event and clk90 = '1' then                
		current_state 			<= next_state;                  
		current_reg1 			<= next_reg1;             
		current_reg2 			<= next_reg2;             
		current_reg3 			<= next_reg3;       
		current_ba      <= next_ba;      
		current_input_address  	<= next_addr;               
		current_burst_done  	<= next_burst_done;                
		loop_count				<= next_loop_count;                
		col_addr 				<= next_col_addr;                       
		current_input_data		<= next_input_data;  
		test_data 				<= next_test_data;
		addr_reverse  <= next_addr_reverse;
		if user_data_valid = '1' then		                  
			intern_reg0 		<= user_output_data;               
			intern_reg1 		<= intern_reg0;                    
			intern_reg2 		<= intern_reg1;                    
			intern_reg3 		<= intern_reg2;                    
		end if;		                                        
		                                                 
	end if;                                           
end process;                                       

--TP_port <= intern_reg0(31 downto 10);

CLK90_REG: process(reset, clk90)
begin
	if reset = '0' then
 		user_input_data     <= (others => '0');
	elsif clk90'event and clk90 = '1' then
		if user_cmd_ack = '1' and next_reg3 = "0100" then
			user_input_data <= current_input_data;
			--user_input_data <= next_input_data;
      end if;
	end if;
end process;

--CLK180_REG: process(reset, clk180)
--begin
--	if reset = '0' then
--	 	intern_reg0 		<= (others => '0');
-- 		intern_reg1 		<= (others => '0');
-- 		intern_reg2 		<= (others => '0');
-- 		intern_reg3 		<= (others => '0');
--	elsif clk180'event and clk180 = '1' then
--		if user_cmd_ack = '1' and user_data_valid = '1' and next_reg3 = "0110" then
--			intern_reg0 	<= user_output_data;
--			intern_reg1 	<= intern_reg0;
--			intern_reg2 	<= intern_reg1;
--			intern_reg3 	<= intern_reg2;
--		end if;
--	end if;
--end process;
--	
-- config_register1 = {  PD,WR,TM,EMR(Enable/Disable DLL),
--                       BMR (Normal operation/Normal Operation with Reset DLL),
--                       BMR/EMR,
--                       CAS_latency (3),
--                       Burst type ,
--                       Burst_length (3) }
-- config_register2 = {  Out,RDQS,DQS_n,OCD_Progm,Posted_Cas,RTT,ODS }

state_machine_combi: process(addr_reverse, ram_req,current_ba,error_count, current_input_address, current_state,init_val,loop_count,user_cmd_ack,user_data_valid,intern_reg3,intern_reg2,intern_reg1,intern_reg0,col_addr,test_data)
begin
	next_reg1 <= "001000000110010";  
	next_reg2 <= "0001110000100";    
	next_reg3 <= (others => '0');
	next_loop_count <= loop_count;
	next_burst_done <= '0';
--	TP1 <= '0';
	--TP2 <= '0';
	--TP1 <= user_cmd_ack;
	TP2 <= user_data_valid;
	next_state <= current_state;
	next_col_addr <= col_addr;
	next_input_data(15 downto 0) <= red(test_data+1);
	next_input_data(31 downto 16)<= red(test_data);
	next_test_data <= test_data;
	next_addr <= current_input_address;
	TP_port <= (others => '0');
	next_ba <= current_ba;
	next_addr_reverse <= '0';
	TP3 <= '0';
	case current_state is
    when S_MEM_INIT1 =>
    	next_reg1 <= "001000000110010";
    	next_reg2 <= "0001110000100";        
		
		if loop_count = 0 then			--take 2 clock cycle to write mode registers
			next_state <= S_MEM_INIT1;
			next_loop_count <= 1;
		else
			next_state <= S_MEM_INIT2;
			next_loop_count <= 0;
		end if;
		
	when S_MEM_INIT2 =>
		next_reg3 <= "0010";				--command register: MEM initialization
		if init_val = '0' then			--initializtion successful(set by mem controller)
			next_state <= S_MEM_INIT2;
		else
			next_state <= S_MEM_WRITE_START;
		end if;
	
	when S_MEM_WRITE_START =>
		next_reg3 <= "0100";				--command register: Write
		next_addr <= current_input_address;	-- row 0 column 0
		if user_cmd_ack = '0' then						-- Write command acknowleged by mem controller 
			next_state <= S_MEM_WRITE_START;  		
			next_input_data(15 downto 0) <= red(test_data-1);
			next_input_data(31 downto 16)<= red(test_data-2);    
		elsif user_cmd_ack = '1' and loop_count = 2 then   --takes 2 more cycles to input new column address
		   next_loop_count <= 0;
		   next_state <= S_MEM_WRITE;
		   if test_data = 10 then
		      next_test_data <= 3;
		   elsif test_data = 9 then
		      next_test_data <= 2;
		   else
		      next_test_data <= test_data + 2;
		   end if;
			next_input_data(15 downto 0) <= red(test_data+1);
			next_input_data(31 downto 16)<= red(test_data);
		elsif user_cmd_ack = '1' and loop_count < 2 then
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
		end if;
		next_col_addr <= (others=>'0');
	when S_MEM_WRITE =>
		if loop_count < 510  then         --each row has 1k addresses, increase by 2 each time (should be 4 every other time)
			next_input_data(15 downto 0) <= red(test_data+1);
			next_input_data(31 downto 16)<= red(test_data);				
			next_state <= S_MEM_WRITE;
			next_loop_count <= loop_count + 1;
			next_addr <= current_input_address + "10";
			--if loop_count = 508 or loop_count = 509 then
			if loop_count = 509 then
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
		elsif loop_count =  510 then   
			next_state <= S_MEM_WRITE_END;
			next_loop_count <= 0;	
			next_input_data(15 downto 0) <= red(test_data+1);
			next_input_data(31 downto 16)<= red(test_data);
			next_addr <= current_input_address + "10";
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
		elsif (user_cmd_ack = '0') and (current_input_address(22 downto 10) /= "00000000000001") then				--ack low, write finishes
		   next_state <= S_MEM_WRITE_START;
			next_loop_count <= 0;	
			next_reg3 <= "0100";
			next_addr <= current_input_address + "10";         --move to next row		
			
------------------------- enable for more than 1 bank  -------------------------------
--		elsif (user_cmd_ack = '0') and (current_input_address(22 downto 10) = "1111111111111") and (current_ba /= "11") then				--ack low, write finishes
--		   next_state <= S_MEM_WRITE_START;
--			next_loop_count <= 0;	
--			next_reg3 <= "0100";
--			next_addr <= (others => '0');         --move to next row
--			next_ba <= current_ba + '1';		 --move to next bank
---------------------------------------------------------------------------------------
		elsif user_cmd_ack = '0' then
			next_state <= S_MEM_PAUSE;
			next_loop_count <= 0;	
			next_reg3 <= "0000";
		end if;	    
	when S_MEM_PAUSE =>
		if loop_count < 100 then
			next_state <= S_MEM_PAUSE;
			next_loop_count <= loop_count + 1;
		else
			next_state <= S_MEM_READ_START;
			next_loop_count <= 0;
			next_addr <= (others => '0');		
		   next_ba <= "00";
		end if;
		
	when S_MEM_READ_START =>
		next_reg3 <= "0110";
		if user_cmd_ack = '0' then
			next_state <= S_MEM_READ_START;   
		elsif user_cmd_ack = '1' and loop_count = 2 then 
			next_state <= S_MEM_READ1;
			next_loop_count <= 0;
		elsif user_cmd_ack = '1' and loop_count < 2 then
		   next_state <= S_MEM_READ_START; 
		   next_loop_count <= loop_count + 1;
		end if;		

	when S_MEM_READ1 =>		       
	   if loop_count < 510 and ram_req = '1' then         --each row has 1k addresses, increase by 2 each time (should be 4 every other time)				
			next_state <= S_MEM_READ2;
			next_loop_count <= loop_count + 1;
			next_addr <= current_input_address + "100";
		elsif loop_count = 510 then
			next_state <= S_MEM_READ_END;
			next_loop_count <= 0;	
			next_addr <= current_input_address;
			next_burst_done <= '1';
		elsif ram_req = '0' then               --no space in FIFO, pause the address increasing
						
			next_state <= S_MEM_READ1;
			if addr_reverse = '0' then
			   next_addr <= current_input_address;
			   next_loop_count <= loop_count;
			else
			   next_addr <= current_input_address - "100";
			   next_loop_count <= loop_count - 2;
			end if;
		end if;
      next_reg3 <= "0110";
    when S_MEM_READ2 =>			
			next_state <= S_MEM_READ1;
			next_loop_count <= loop_count + 1;
         next_reg3 <= "0110"; 
         if ram_req = '1' then
            next_addr_reverse <= '0'; 
         else
            next_addr_reverse <= '1';
         end if;        
    when S_MEM_READ_END =>
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
			
		elsif user_cmd_ack = '0'	and (current_input_address(22 downto 10) /= "0000000000001") then
			next_state <= S_MEM_READ_START;
			next_loop_count <= 0;	
			next_reg3 <= "0110";
		   next_addr <= current_input_address + "100";	
------------------------- enable for more than 1 bank  -------------------------------		   	
--		elsif user_cmd_ack = '0'	and (current_input_address(22 downto 10) = "1111111111111") and (current_ba /= "11") then
--			next_state <= S_MEM_READ_START;
--			next_loop_count <= 0;	
--			next_reg3 <= "0110";
--	      next_addr <= (others => '0');
--		   next_ba <= current_ba + '1';
---------------------------------------------------------------------------------------
		elsif user_cmd_ack = '0' then --and user_data_valid = '0' then
			next_state <= S_MEM_IDLE;
			next_loop_count <= 0;	
			next_reg3 <= "0000";
		end if;	  	  

	when S_MEM_IDLE =>
		next_addr <= (others => '0');
		next_input_data <= X"00000000";
		next_reg3 <= "0000";	
		next_state <= S_MEM_IDLE;
		--if intern_reg3 = X"5A5A3C3C" then
		--	TP1 <= '1';
		--end if;
		--if intern_reg1 = X"11335577" and intern_reg0 = X"FFCCBB99" then
		--	TP2 <= '1';
		--end if;
		TP_port <= error_count;
		TP3 <= '1';
	when others =>
		next_state <= S_MEM_INIT1;	
	end case;
end process;

user_data_mask <= (others => '0');


--comp_read_write: process(clk, reset)
--begin
--    if reset = '0' then
--       compare_data <= (others => '0');
--    elsif clk'event and clk = '1' then
--       if user_data_valid = '1' then
--          compare_data <= compare_data + '1';
--       end if;
--    end if;
--end process;
--           
--comp_read_write_clk0: process(clk, reset)
--begin
--    if reset = '0' then
--       TP1 <= '0';
--		 error_count <= (others => '0');
--    elsif clk'event and clk = '1' then
--       if user_data_valid = '1' then
--          if (compare_data(15 downto 0) & compare_data(15 downto 0)) /= user_output_data then
--              TP1 <= '1';
--				  error_count <= error_count + '1'; 
--          end if;
--       end if;
--    end if;
--end process;
sim_oe: process(clk90, reset)
begin
    if reset = '0' then
        o_enable <= '0';
    elsif clk90'event and clk90 = '1' then
        if output_enable = '1' then
           o_enable <=not o_enable;
        else
           o_enable <= '0';
    end if;
   end if;
end process;
end  arc_controller;