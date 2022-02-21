----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    10:50:15 05/24/2011 
-- Design Name: 
-- Module Name:    ad5660_serial_DAC - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_SIGNED.ALL;


-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity ad5660_serial_DAC is
    Port ( clk_i : in  STD_LOGIC;
           decimation_i : in  STD_LOGIC_VECTOR (3 downto 0);
           word_i : in  STD_LOGIC_VECTOR (15 downto 0);
           update_i : in  STD_LOGIC;
           sclk_o : out  STD_LOGIC;
           data_o : out  STD_LOGIC;
           sync_o : out  STD_LOGIC);
end ad5660_serial_DAC;

architecture Behavioral of ad5660_serial_DAC is

-- State typer
type state_type is (	st_idle, st_latchdata, st_shiftdata, st_waitclock, st_setclocklow);

signal state, next_state : state_type; 

signal bit_count : std_logic_vector(4 downto 0);
signal dec_clk_count : std_logic_vector(3 downto 0);
signal shift_register : std_logic_vector(23 downto 0);
signal sclk : std_logic;

begin

	-- output serial clock
	sclk_o <= sclk;

	SYNC_PROC: process (clk_i)
   begin
      if clk_i'event and clk_i = '1' then
			state <= next_state;      
      end if;
   end process;
	
	OUTPUT_DECODE: process (clk_i)
   begin
		if clk_i'event and clk_i = '1' then
			 case state is
				when st_idle =>
					sclk <= '1';
					data_o <= '1';
					sync_o <= '1';
					shift_register <= (others => '0');
					dec_clk_count <= (others => '0');
					bit_count <= (others => '0');
					
				when st_latchdata =>
					sclk <= '1';
					data_o <= '1';
					sync_o <= '0';
					shift_register <= "00000000" & word_i;
					dec_clk_count <= (others => '0');
					bit_count <= (others => '0');
					
				when st_shiftdata =>
					sclk <= '1';
					data_o <= shift_register(23);
					sync_o <= '0';
					shift_register <= shift_register(22 downto 0) & '0';
					dec_clk_count <= (others => '0');
					bit_count <= bit_count + 1;
					
				when st_waitclock =>
					sclk <= sclk;
					data_o <= shift_register(23);
					sync_o <= '0';
					shift_register <= shift_register;
					dec_clk_count <= dec_clk_count + 1;
					bit_count <= bit_count;
					
				when st_setclocklow =>
					sclk <= '0';
					data_o <= shift_register(23);
					sync_o <= '0';
					shift_register <= shift_register;
					dec_clk_count <= (others => '0');
					bit_count <= bit_count;
					
				when others =>
					sclk <= '1';
					data_o <= '1';
					sync_o <= '1';
					shift_register <= (others => '0');
					dec_clk_count <= (others => '0');
					bit_count <= bit_count;
			end case;
		end if;
	end process;
	
	NEXT_STATE_DECODE: process (state, update_i, dec_clk_count)
   begin
      --declare default state for next_state to avoid latches
      next_state <= state;  --default is to stay in current state
     		
      case state is
         when st_idle =>
				if update_i = '1' then
					next_state <= st_latchdata;
				end if;

			when st_latchdata =>
				next_state <= st_waitclock;
				
			when st_shiftdata =>
				next_state <= st_waitclock;
				
			when st_waitclock =>
				if dec_clk_count = decimation_i then
					if sclk = '1' then
						next_state <= st_setclocklow;
					else
						if bit_count = "10111" then
							next_state <= st_idle;
						else
							next_state <= st_shiftdata;
						end if;
					end if;
				end if;
				
			when st_setclocklow =>
				next_state <= st_waitclock;
			
			when others =>
			 next_state <= st_idle;
		end case;
	end process;
end Behavioral;

