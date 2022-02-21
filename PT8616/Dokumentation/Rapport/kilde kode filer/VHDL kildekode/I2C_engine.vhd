-----------------------------------------------------------
--				 I2C engine
-----------------------------------------------------------

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
			  byte_read_o		: out STD_LOGIC
			  );
end I2C_engine;

architecture Behavioral of I2C_engine is
signal SDA					: STD_LOGIC;
signal SCL					: STD_LOGIC;
signal SDA_out				: STD_LOGIC;
signal SDA_in				: STD_LOGIC;
signal SDA_delay			: STD_LOGIC;
signal SCL_delay			: STD_LOGIC;
signal SDA_rise_flag 	: STD_LOGIC;
signal SDA_fall_flag 	: STD_LOGIC;
signal SCL_rise_flag 	: STD_LOGIC;
signal SCL_fall_flag 	: STD_LOGIC;

signal bit_count			: integer range 0 to 8:=0;
signal command				: STD_LOGIC_VECTOR(7 downto 0);
signal recieved_byte		: STD_LOGIC_VECTOR(7 downto 0);
signal time_out			: STD_LOGIC_VECTOR(21 downto 0);
signal RW_dir				: STD_LOGIC;

-- States til state-machine
type state_type is (idle, read_command, read_byte, write_byte, give_ack_command, give_ack_byte);
signal state				: state_type:=idle;

begin
	--SDA og SCL delay line
	signal_reclocking: process (reset_i, clk_i) 
	begin
		if reset_i = '1' then
			SDA <= '1';
			SDA_delay <= '1';
			SCL <= '1';
			SCL_delay <= '1';
		elsif clk_i'event and clk_i = '1' then
			SDA <= SDA_in;
			SDA_delay <= SDA;	
			SCL <= SCL_i;
			SCL_delay <= SCL;
		end if;
	end process signal_reclocking;
	
	
	--SDA og SCL event-monitor. Sætter flag højt ved stigende eller faldende flanke
	I2C_event: process (reset_i, clk_i) 
	begin
		if reset_i = '1' then
			SDA_rise_flag <= '0';
			SDA_fall_flag <= '0';
			SCL_rise_flag <= '0';
			SCL_fall_flag <= '0';
		elsif clk_i'event and clk_i = '1' then
			--SDA events
			if SDA = '1' and SDA_delay = '0' then
				SDA_fall_flag <= '0';
				SDA_rise_flag <= '1';
			elsif SDA = '0' and SDA_delay = '1' then
				SDA_rise_flag <= '0';
				SDA_fall_flag <= '1';
			else
				SDA_rise_flag <= '0';
				SDA_fall_flag <= '0';
			end if;
			--SCL events
			if SCL = '1' and SCL_delay = '0' then
				SCL_fall_flag <= '0';
				SCL_rise_flag <= '1';
			elsif SCL = '0' and SCL_delay = '1' then
				SCL_rise_flag <= '0';
				SCL_fall_flag <= '1';
			else
				SCL_rise_flag <= '0';
				SCL_fall_flag <= '0';
			end if;
		end if;
	end process I2C_event;	
		
		
	--I2C maskine, fortolker SDA og SCL og sætter pågældende states
	I2C_machine: process (reset_i, clk_i)
	begin
		if reset_i = '1' then
			state <= idle;
			bit_count <= 0;
			time_out <= conv_std_logic_vector(0, 22);
			RW_dir <= '0';
			
		elsif clk_i'event and clk_i='1' then
		--reset timer
		time_out <= time_out + 1;
		
		--I2C-start
			if SCL = '1' and SDA_fall_flag = '1' and state = idle then
				bit_count <= 0;
				time_out <= conv_std_logic_vector(0, 22);
				state <= read_command;
				
		--I2C-stop
			elsif SCL = '1' and SDA_rise_flag = '1' then
				state <= idle;
				
		--læs kommando+adresse/byte ind bitvist
			elsif SCL_rise_flag = '1' then
				time_out <= conv_std_logic_vector(0, 22);
				--command
				if state = read_command then
					if bit_count/=8 then
						command(7-bit_count) <= SDA;
						bit_count <= bit_count + 1;
					end if;
					
				--byte
				elsif state = read_byte then
					if bit_count/=8 then
						recieved_byte(7-bit_count) <= SDA;
						bit_count <= bit_count + 1;
					end if;
				end if; -- State = read_command/read_byte
				
			elsif SCL_fall_flag = '1' then
				if bit_count /= 8 and state = write_byte then
					SDA_out <= send_byte_i(7-bit_count);
					bit_count <= bit_count + 1;
				end if;
				
				if bit_count = 8 and (state = write_byte or state = read_byte) then
					rw_dir<='1';
					SDA_out <= '0';
					recieved_byte_o <= recieved_byte;
					state<=give_ack_byte;
					byte_read_o <= '1';
					
				elsif bit_count = 8 and state = read_command then
					if command(7 downto 1) = address_i then
						rw_dir <= '1';
						SDA_out <= '0';
						state <= give_ack_command;
					--forkert adresse -> idle
					else
						state <= idle;
					end if; --endif command = adresse
					
				elsif state = give_ack_command then
					if command(0) = '0' then
						rw_dir <= '0';
						bit_count <= 0;
						state <= read_byte;
					else
						rw_dir <= '1';
						bit_count <= 1;
						SDA_out <= send_byte_i(7);
						state <= write_byte;
					end if; --endif command(0)
			
				elsif state = give_ack_byte then
					rw_dir <= '0';
					byte_read_o <= '0';
					state<=idle;
				end if;
				
			elsif state = idle then
				rw_dir <= '0';
				byte_read_o <= '0';
				
		--Ellers gør intet, indtil der er time-out (25 ms)
			else
				if time_out = conv_std_logic_vector(3712500, 22) then
					state <= idle;
				end if;
			end if;
		end if; -- clk_i'event
	end process I2C_machine;

	--tristate controller for I2C SDA IO
	with RW_dir select
		SDA_io <= SDA_out when '1',
					'Z' when others;
					
	SDA_in <= SDA_io;

end Behavioral;

