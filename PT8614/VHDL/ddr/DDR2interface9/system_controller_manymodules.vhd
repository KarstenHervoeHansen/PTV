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
--		dip1                	: out std_logic;
--		dip2                	: out std_logic;
--		dip3                	: out std_logic;
		reset	            	: in std_logic;
		clk	                	: in std_logic;
		clk90                   : in std_logic;
		clk180					: in std_logic;
--		sys_clk_ibuf        	: in std_logic;
		dcm_lock            	: out std_logic;
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

type state_type is(S_INIT, S_MEM_INIT1,S_MEM_INIT2,S_MEM_READ,S_MEM_READ_START,S_MEM_READ_END,S_MEM_WRITE_START,S_MEM_WRITE,S_MEM_WRITE_END,S_MEM_IDLE,S_MEM_PAUSE);
                      
signal current_state, next_state: state_type;
signal next_reg1 : std_logic_vector(14 downto 0);
signal next_reg2 : std_logic_vector(12 downto 0);
signal next_reg3 : std_logic_vector(3 downto 0);
signal next_loop_count, loop_count: integer range 0 to 255;
signal next_addr : std_logic_vector(22 downto 0);
signal next_input_data : std_logic_vector(31 downto 0);
signal next_burst_done : std_logic;
signal intern_reg0, intern_reg1, intern_reg2, intern_reg3: std_logic_vector(31 downto 0);
signal col_addr,next_col_addr: std_logic_vector(7 downto 0);

begin

state_machine_reg: process(reset, clk)
begin
	if reset = '0' then
 		current_state 			<= S_MEM_INIT1;
 		user_config_register1	<= (others => '0');
 		user_config_register2 	<= (others => '0');
 		user_command_register 	<= (others => '0');
 		user_input_address  	<= (others => '0');
 		burst_done				<= '0';        
 		loop_count				<= 0;        
 		col_addr <= (others => '0');
 		
	elsif clk'event and clk = '1' then
		current_state 			<= next_state;
		user_config_register1 	<= next_reg1;
		user_config_register2 	<= next_reg2;
		user_command_register 	<= next_reg3;
		user_input_address  	<= next_addr;
		burst_done				<= next_burst_done;
		loop_count				<= next_loop_count;
		col_addr <= next_col_addr;
		
	end if;
end process;

CLK90_REG: process(reset, clk90)
begin
	if reset = '0' then
 		user_input_data     <= (others => '0');
 		intern_reg0 		<= (others => '0');
 		intern_reg1 		<= (others => '0');
 		intern_reg2 		<= (others => '0');
 		intern_reg3 		<= (others => '0');
	elsif clk90'event and clk90 = '1' then
		if user_cmd_ack = '1' and next_reg3 = "0100" then
			user_input_data <= next_input_data;
		elsif user_data_valid = '1' then		
			intern_reg0 	<= user_output_data;                                         	
			intern_reg1 	<= intern_reg0;                                              	
			intern_reg2 	<= intern_reg1;                                              	
			intern_reg3 	<= intern_reg2;       
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

state_machine_combi: process(current_state,init_val,loop_count,user_cmd_ack,user_data_valid,intern_reg3,intern_reg2,intern_reg1,intern_reg0,col_addr)
begin
	next_reg1 <= "001000000110010";  
	next_reg2 <= "0001110000100";    
	next_reg3 <= (others => '0');
	next_loop_count <= loop_count;
	next_addr <=  (others => '0');
	next_input_data <= X"00000000";
	next_burst_done <= '0';
--	TP1 <= '0';
	--TP2 <= '0';
	TP1 <= user_cmd_ack;
	TP2 <= user_data_valid;
	next_state <= current_state;
	next_col_addr <= col_addr;
	case current_state is
    when S_MEM_INIT1 =>
    	next_reg1 <= "001000000110010";
    	next_reg2 <= "0001110000100";        
		
		if loop_count = 0 then
			next_state <= S_MEM_INIT1;
			next_loop_count <= 1;
		else
			next_state <= S_MEM_INIT2;
			next_loop_count <= 0;
		end if;
		
	when S_MEM_INIT2 =>
		next_reg3 <= "0010";
		if init_val = '0' then
			next_state <= S_MEM_INIT2;
		else
			next_state <= S_MEM_WRITE_START;
		end if;
	
	when S_MEM_WRITE_START =>
		next_reg3 <= "0100";
		next_addr <= "00000000000000000000000";--;--"00000000000000000000001";
		if user_cmd_ack = '0' then
			next_state <= S_MEM_WRITE_START;    
		else
			next_state <= S_MEM_WRITE;
		end if;
		next_col_addr <= (others=>'0');
	when S_MEM_WRITE =>
		if loop_count = 0  then
			next_input_data <= X"5A5A0000";--X"3333CCCC";
			next_addr <= "0000000000000" & col_addr & "00";--"00000110011001100110000";--"00000000000000000000001";
			next_state <= S_MEM_WRITE;
			next_loop_count <= loop_count + 1;
		elsif loop_count = 1 then
			next_input_data <= X"90ABCDEF";--X"5A5A5A5A";
			next_addr <= "0000000000000" & col_addr & "00";--"00000110011001100110000";

			if col_addr = "11111110" then
			   next_state <= S_MEM_WRITE_END;
			   next_loop_count <= 0;--loop_count + 1;
			   next_col_addr <= (others => '0'); 
			else
			   next_state <= S_MEM_WRITE;
			   next_loop_count <= 0;
			   next_col_addr <= unsigned(col_addr) + '1';
			end if;
--		elsif loop_count = 2 then
--			next_input_data <= X"11335577";--X"3C3C3C3C";
--			next_addr <= "00000110011001100110011";
--			next_state <= S_MEM_WRITE;
--			next_loop_count <= loop_count + 1; 
--		elsif loop_count = 3 then
--			next_input_data <= X"FFCCBB99";--X"5555AAAA";
--			next_addr <= "00000000000000000000111";
--			next_state <= S_MEM_WRITE;
--			next_loop_count <= loop_count + 1;
--		elsif loop_count = 4 then
--			next_input_data <= X"11557799";
--			next_addr <= "00000000000000000000111";
--			next_state <= S_MEM_WRITE_END;
--			next_loop_count <= 0;			
		end if;
        next_reg3 <= "0100";
	when S_MEM_WRITE_END =>
		if loop_count < 2 then
			if loop_count = 0 then 
				next_input_data <= X"AACCEE00";
		    end if;
			next_burst_done <= '1';
			next_state <= S_MEM_WRITE_END;
			next_loop_count <= loop_count + 1;
			next_reg3 <= "0100";
			next_addr <= "00000000000000000000000";
		elsif loop_count < 4 then
			next_burst_done <= '0';
			next_state <= S_MEM_WRITE_END;
			next_loop_count <= loop_count + 1;
			next_reg3 <= "0100";
			next_addr <= "00000000000000000000000";
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
		end if;
	when S_MEM_READ_START =>
		next_reg3 <= "0110";
		next_addr <= "0000000000000" & col_addr & "00"; -- 1100110000";--"00000000000000000000001";
		if user_cmd_ack = '0' then
			next_state <= S_MEM_READ_START;    
		else
			next_state <= S_MEM_READ;
		end if;		
	when S_MEM_READ =>
		if loop_count = 0 then
			next_addr <= "0000000000000" & col_addr & "00";--"00000110011001100110000";--"00000000000000000000001";
			next_state <= S_MEM_READ;
			next_loop_count <= loop_count + 1;
			next_col_addr <= unsigned(col_addr) + '1';
		elsif loop_count = 1 then
			next_addr <= "0000000000000" & col_addr & "00";--"00000110011001100110000";
			next_state <= S_MEM_READ;
			next_loop_count <= loop_count + 1;
		elsif loop_count = 2 then
			next_addr <= "0000000000000" & col_addr & "00";--"00000110011001100110000";			
			next_loop_count <= 1;--loop_count + 1; 
			next_col_addr <= unsigned(col_addr) + '1';
			if col_addr = "11111110" then
			   next_state <= S_MEM_READ_END;
			   next_loop_count <= 0;--loop_count + 1; 
			else 
			   next_state <= S_MEM_READ;
			   next_loop_count <= 1;
			end if;
		
			
			
--		elsif loop_count < 8 then
--			next_addr <= "00000110011001100110011";
--			next_state <= S_MEM_READ;
--			next_loop_count <= loop_count + 1;
--		elsif loop_count = 4 then
--			next_addr <= "00000000000000000000111";
--			next_state <= S_MEM_READ;
--			next_loop_count <= loop_count + 1;
--		elsif loop_count = 5 then
--			next_addr <= "00000000000000000000011";
--			next_state <= S_MEM_READ;
--			next_loop_count <= loop_count + 1;
--		elsif loop_count = 6 then
--			next_addr <= "00000000000000000000011";
--			next_state <= S_MEM_READ;
--			next_loop_count <= loop_count + 1;
--		elsif loop_count = 7 then
--			next_addr <= "00000000000000000000111";
--			next_state <= S_MEM_READ;
--			next_loop_count <= loop_count + 1;
--		elsif loop_count = 8 then
--			next_addr <= "00000110011001100110011";
--			next_state <= S_MEM_READ_END;
--			next_loop_count <= 0;			
		end if;
        next_reg3 <= "0110";
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
		elsif user_cmd_ack = '0' and user_data_valid = '0' then
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
	when others =>
		null;	
	end case;
end process;

user_data_mask <= (others => '0');
user_bank_address <= "00";
TP_port <= intern_reg3(31 downto 10);
--TP3 <= read_waiting;
end  arc_controller;