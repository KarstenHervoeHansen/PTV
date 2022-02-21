
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity I2C_engine is
    Port ( reset_i 			: in  STD_LOGIC;
           clk_i 				: in  STD_LOGIC;
           SCL_i 				: in  STD_LOGIC;
           SDA_io 			: inout  STD_LOGIC;
           send_byte_i 		: in  STD_LOGIC_VECTOR (7 downto 0);
           recieved_byte_o : out	STD_LOGIC_VECTOR (7 downto 0);
			  address_i			: in 	STD_LOGIC_VECTOR (6 downto 0);
			  update_flag_o	: out STD_LOGIC;
			  start_o			: out STD_LOGIC;
			  stop_o				: out STD_LOGIC;
			  debug_o			: out STD_LOGIC
			  );
end I2C_engine;

architecture Behavioral of I2C_engine is
signal SDA_out				: STD_LOGIC;
signal SDA_in				: STD_LOGIC;
signal SDA_delay			: STD_LOGIC_VECTOR(1 downto 0);
signal SCL_delay			: STD_LOGIC_VECTOR(1 downto 0);

signal bit_count			: integer range 0 to 9:=0;
signal send_byte			: STD_LOGIC_VECTOR(7 downto 0);
signal recieve_byte		: STD_LOGIC_VECTOR(7 downto 0);
signal time_out			: STD_LOGIC_VECTOR(21 downto 0);
signal RW_dir				: STD_LOGIC;

--timeout tid (25 ms)
constant time_out_reload : integer:=3712500;

-- States til state-machine
type state_type is (idle, new_readbyte, new_readcom, new_writebyte, wait_for_downclk, wait_for_upclk, read_SDA, write_SDA, give_ack, update_buffer, confirm_command);
signal state				: state_type;

-- Commands til I2C
type i2c_command is (read_command, read_byte, write_byte);
signal command				: i2c_command;

begin
	--SDA og SCL delay line
	signal_reclocking: process (clk_i) 
	begin
		if clk_i'event and clk_i = '1' then
			if reset_i = '1' then
				SDA_delay <= "11";
				SCL_delay <= "11";
			else
				SDA_delay <= SDA_delay(0) & SDA_in;
				SCL_delay <= SCL_delay(0) & SCL_i;
			end if;
		end if;
	end process signal_reclocking;
		
	--tristate controller for I2C SDA IO
	with RW_dir select
		SDA_io <= SDA_out when '1',
					'Z' when others;
					
	SDA_in <= SDA_io;
	
	--State machine for I2C engine
	I2C_machine : process (clk_i)
	begin
		if clk_i'event and clk_i='1' then
			if reset_i = '1' then
				state <= idle;
			else
				--idle + start betingelse
				if SDA_delay="10" and SCL_delay="11" then
					state <= new_readcom;
				
				--stop betingelse
				elsif SDA_delay="01" and SCL_delay="11" then
					state <= idle;
					
				--ved ny indlæsning (læs kommando eller byte, skriv byte)
				elsif state=new_readcom and command=read_command then
					state <= wait_for_upclk;
				elsif state=new_readbyte and command=read_byte then
					state <= wait_for_upclk;
				elsif state=new_writebyte and command=write_byte then
					state <= write_SDA;
					
				--ved læsning af kommando eller byte
				elsif state=wait_for_upclk and SCL_delay="01" then
					state <= read_SDA;
				elsif state=read_SDA and bit_count/=7 then
					state <= wait_for_upclk;
				elsif state=read_SDA and bit_count=7 and command=read_command then				
					state <= confirm_command;
				elsif state=read_SDA and bit_count=7 and command=read_byte then				
					state <= update_buffer;
									
				--ved skrivning af byte
				elsif state=write_SDA then
					state <= wait_for_downclk;
				elsif state=wait_for_downclk and SCL_delay="10" and bit_count/=8 then
					state <= write_SDA;
				elsif state=wait_for_downclk and SCL_delay="10" and bit_count=8 then
					state <= update_buffer;
				
				--ved opdatering af buffer
				elsif state=confirm_command and command=read_command and recieve_byte(7 downto 1)=address_i and SCL_delay="10" then
					state <= give_ack;
				elsif state=confirm_command and command=read_command and recieve_byte(7 downto 1)/=address_i then 
					state <= idle;
				elsif state=update_buffer and command=read_byte and SCL_delay="10" then
					state <= give_ack;
				elsif state=update_buffer and command=write_byte then --and SCL_delay="10" then
					state <= give_ack;
				
				--give acknowledge
				elsif state=give_ack and command=read_command and recieve_byte(0)='0' and SCL_delay="10" then
					state <= new_readbyte;
				elsif state=give_ack and command=read_command and recieve_byte(0)='1' and SCL_delay="10" then
					state <= new_writebyte;
				elsif state=give_ack and command=read_byte and SCL_delay="10" then
					state <= new_readbyte;
				elsif state=give_ack and command=write_byte and SCL_delay="10" then
					state <= idle; --her kan laves læsning af flere simultane bytes				

				elsif time_out = 0 then
					state <= idle;
				end if;
			end if;
		end if;
	end process;
				
	-- Fortolk state-machine
	interpret_machine : process(clk_i)
	begin
		if clk_i'event and clk_i='1' then		
			if state = idle then
				command <= read_command; 
				bit_count <= 0;
				update_flag_o <= '0';
				start_o <= '0';
				stop_o <= '1';
				RW_dir <= '0';
				SDA_out <= '0';
				recieve_byte <= "00000000";
				--recieved_byte_o <= "00000000";
				send_byte <= send_byte_i;						
				time_out <= conv_std_logic_vector(time_out_reload, 22);
									
			elsif state = new_readcom then
				command <= read_command;
				bit_count <= 0;
				update_flag_o <= '0';
				start_o <= '1';
				stop_o <= '0';
				RW_dir <= '0';
				SDA_out <= '0';
				recieve_byte <= "00000000";
				--recieved_byte_o <= "00000000";
				send_byte <= send_byte_i;						
				time_out <= conv_std_logic_vector(time_out_reload, 22);
				
			elsif state = new_readbyte then
				command <= read_byte;
				bit_count <= 0;
				update_flag_o <= '0';
				start_o <= '0';
				stop_o <= '0';
				RW_dir <= '0';
				SDA_out <= '0';
				recieve_byte <= "00000000";
				--recieved_byte_o <= "00000000";
				send_byte <= send_byte_i;						
				time_out <= conv_std_logic_vector(time_out_reload, 22);
				
			elsif state = new_writebyte then
				command <= write_byte;
				bit_count <= 0;
				update_flag_o <= '0';
				start_o <= '0';
				stop_o <= '0';
				RW_dir <= '1';
				SDA_out <= '0';
				recieve_byte <= "00000000";
				--recieved_byte_o <= "00000000";
				send_byte <= send_byte_i;						
				time_out <= conv_std_logic_vector(time_out_reload, 22);
							
			elsif state = wait_for_downclk then
				--command <= write_command;
				--bit_count <= bit_count+1;
				update_flag_o <= '0';
				start_o <= '0';
				stop_o <= '0';
				RW_dir <= '1';
				--SDA_out <= '0';
				recieve_byte <= "00000000";
				--recieved_byte_o <= "00000000";
				--send_byte <= send_byte_i;						
				time_out <= time_out-1;		
				
			elsif state = wait_for_upclk then
				--command <= read_command;
				--bit_count <= 0;
				update_flag_o <= '0';
				start_o <= '0';
				stop_o <= '0';
				RW_dir <= '0';
				SDA_out <= '0';
				--recieve_byte <= "00000000";
				--recieved_byte_o <= "00000000";
				send_byte <= send_byte_i;						
				time_out <= time_out-1;
								
			elsif state = read_SDA then
				--command <= read_command;
				bit_count <= bit_count+1;
				update_flag_o <= '0';
				start_o <= '0';
				stop_o <= '0';
				RW_dir <= '0';
				SDA_out <= '0';
				recieve_byte(7-bit_count) <= SDA_in;
				--recieved_byte_o <= "00000000";
				send_byte <= send_byte_i;						
				time_out <= conv_std_logic_vector(time_out_reload, 22);			
							
			elsif state = write_SDA then
				--command <= read_command;
				bit_count <= bit_count+1;
				update_flag_o <= '0';
				start_o <= '0';
				stop_o <= '0';
				RW_dir <= '1';
				SDA_out <= send_byte(7-bit_count);
				recieve_byte <= "00000000";
				--recieved_byte_o <= "00000000";
				send_byte <= send_byte_i;						
				time_out <= conv_std_logic_vector(time_out_reload, 22);			
							
			elsif state = give_ack then			
				--command <= read_command;
				bit_count <= 0;
				update_flag_o <= '0';
				start_o <= '0';
				stop_o <= '0';
				RW_dir <= '1';
				SDA_out <= '0';
				--recieve_byte <= "00000000";
				--recieved_byte_o <= "00000000";
				send_byte <= send_byte_i;						
				time_out <= time_out-1;
				
			elsif state = update_buffer then
				--command <= read_command;
				bit_count <= 0;
				update_flag_o <= '1';
				start_o <= '0';
				stop_o <= '0';
				RW_dir <= '0';
				SDA_out <= '0';
				--recieve_byte <= "00000000";
				recieved_byte_o <= recieve_byte;
				send_byte <= send_byte_i;						
				time_out <= conv_std_logic_vector(time_out_reload, 22);							
				
			elsif state = confirm_command then
				--command <= read_command;
				bit_count <= 0;
				update_flag_o <= '0';
				start_o <= '0';
				stop_o <= '0';
				RW_dir <= '0';
				SDA_out <= '0';
				--recieve_byte <= "00000000";
				--recieved_byte_o <= recieve_byte;
				send_byte <= send_byte_i;						
				time_out <= conv_std_logic_vector(time_out_reload, 22);							

			else
				command <= read_command;
				bit_count <= 0;
				update_flag_o <= '0';
				start_o <= '0';
				stop_o <= '0';
				RW_dir <= '0';
				SDA_out <= '0';
				recieve_byte <= "00000000";
				--recieved_byte_o <= "00000000";
				send_byte <= send_byte_i;
				time_out <= conv_std_logic_vector(time_out_reload, 22);
			end if;
		end if;
	end process;
			
	debug_o <= RW_dir;
end Behavioral;

