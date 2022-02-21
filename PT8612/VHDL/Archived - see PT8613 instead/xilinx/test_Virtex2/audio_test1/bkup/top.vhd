--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:    11:14:34 11/29/05
-- Design Name:    
-- Module Name:    audio_clk - Behavioral
-- Project Name:   
-- Target Device:  
-- Tool versions:  
-- Description:
--
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
--------------------------------------------------------------------------------
use work.pack_even_parity_bit.all;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
library UNISIM;
use UNISIM.VComponents.all;

entity audio_clk is
	 Port ( clkmux_in 	   	: in std_logic_vector(2 downto 0);	-- 000 = 148.5->48, 001 = 148.35->48,-- 010 = 148.5 -> 44.1, 011 = 148.35-> 44.1,-- 100 = 148.5 -> 32,   101 = 148.35 -> 32	                                                            																   
           videoclk_in 			: in std_logic;
           enable_in				: in std_logic;
           reset_in 				: in std_logic;
			  sync_reset_in		: in std_logic;
			  activedata_in      : in std_logic_vector(9 downto 0);
			  audio_counter_out 	: out std_logic_vector(11 downto 0);
			  fvh_i              : in std_logic_vector(2 downto 0);  
			  line_o               : in std_logic_vector(10 downto 0);    
--			  a :   in std_logic_vector(17 downto 0);
--			  b :   in std_logic_vector(17 downto 2);
--			  p    :  out std_logic_vector(35 downto 0);
           audiodata_out	   : out std_logic_vector(9 downto 0);
           audioclk_out 		: out std_logic);      
end audio_clk;

architecture Behavioral of audio_clk is
type m_array is array(0 to 2) of std_logic_vector(11 downto 0);
type state_type is(S_ADF1, S_ADF2,S_ADF3,S_DID, S_DBN, S_DC, S_CLK1, S_CLK2, S_ACTIVEAUDIO, 
    S_ECC, S_CS, S_IDLE); 
constant interval_1485i_48o : integer := 8;
constant compare_1485i_48o  : integer := 12375; 
constant interval_1484i_48o : integer := 91;  
constant compare_1484i_48o  : integer := 140625;

constant interval_1485i_441o: integer := 49;
constant compare_1485i_441o : integer := 82500;
constant interval_1484i_441o: integer := 4459;
constant compare_1484i_441o : integer := 7500000;

constant interval_1485i_32o: integer := 16;
constant compare_1485i_32o : integer := 37125;
constant interval_1484i_32o: integer := 182;
constant compare_1484i_32o : integer := 421875;

signal counter_buffer : m_array;
signal compare : integer range 0 to 7504459;
signal interval: integer range 0 to 4459;
signal counter : integer range 0 to 7504459; 
signal temp_clk_counter : std_logic_vector(11 downto 0);
signal index, sample_per_line:  integer range 0 to 3;
signal eav_tick,h_delayed_1   :  std_logic;	
signal current_state, next_state : state_type;
signal sample_pos : integer range 0 to 3;
signal  jump_point : std_logic;

begin

with clkmux_in select
	  compare <= compare_1485i_48o  when "000",
	             compare_1484i_48o  when "001",
--					 compare_1485i_441o when "010",
--					 compare_1484i_441o when "011",
--					 compare_1485i_32o  when "100",
--					 compare_1484i_32o  when "101",
					 compare_1485i_48o  when others;

with clkmux_in select
	  interval <= interval_1485i_48o  when "000",
	              interval_1484i_48o  when "001",
--					  interval_1485i_441o when "010",
--					  interval_1484i_441o when "011",
--					  interval_1485i_32o  when "100",
--					  interval_1484i_32o  when "101",
					  interval_1485i_48o  when others;


make_fvh0_delay : process(videoclk_in, reset_in)
begin
	if reset_in = '0' then
		h_delayed_1 <= '0';
   elsif videoclk_in'event and videoclk_in = '1' then
		if sync_reset_in = '1' then
			h_delayed_1 <= '0';
		elsif enable_in = '1' then  
		   h_delayed_1 <= fvh_i(0);
      end if;
	end if;
end process;

make_eav_tick : process(reset_in, sync_reset_in, fvh_i(0), h_delayed_1)
begin
	if reset_in = '0' then
		eav_tick <= '0';
	elsif sync_reset_in = '0' then
		eav_tick <= '0';
	elsif fvh_i(0) = '1' and h_delayed_1 = '0' then
		eav_tick <= '1';
	else
		eav_tick <= '0';
	end if;
end process;

make_clk : process(videoclk_in, reset_in)
begin
	if reset_in = '0' then
		counter <= 0;
		audioclk_out <= '0';
		temp_clk_counter <= (others => '0');
		sample_per_line <= 0;
		index <= 0;
		current_state <= S_IDLE;
		sample_pos <= 0;
   elsif videoclk_in'event and videoclk_in = '1' then
		if sync_reset_in = '1' then
			counter <= 0;	 
			audioclk_out <= '0';
			temp_clk_counter <= (others => '0');
			sample_per_line <= 0;
			index <= 0;
			current_state <= S_IDLE;
			sample_pos <= 0;
		elsif enable_in = '1' then  
		   current_state <= next_state;     
		   if eav_tick = '1' then
				temp_clk_counter <= (others => '0');
				if  jump_point = '0' then  
					sample_per_line <= index;			
					index <= 0;
					sample_pos <= 0;
				end if;
         else 
			   temp_clk_counter <= temp_clk_counter + '1';
			end if;
			if current_state = S_CS then
				sample_pos <= sample_pos + 1;
 			end if;
			if counter < compare then
				counter <= counter + interval;
				audioclk_out <= '0';				
			else
				counter <= counter - compare + interval ;
				audioclk_out <= '1';
				counter_buffer(index) <= temp_clk_counter;
				index <= index + 1;
			end if;
      end if;
	end if;
end process;

state_description: process(current_state, sample_pos, eav_tick)
begin
case current_state is
	 when S_ADF1 =>
	 	  audiodata_out <= (others => '0');
		  next_state <= S_ADF2;
	 when S_ADF2 => 
	 	  audiodata_out <= (others => '1');
		  next_state <= S_ADF3;
	 when S_ADF3 => 
	 	  audiodata_out <= (others => '1');
		  next_state <= S_DID;
	 when S_DID=> 
	 	  audiodata_out <= "1011100111";	--2E7 = channel 1-4; 1E6 = channel 5-8; 1E5 = channel 9-12; 2E4 = channel 13-16
		  next_state <= S_DBN;
	 when S_DBN => 
	 	  audiodata_out <= "1000000000";	--disable continuity indication
		  next_state <= S_DC;
    when S_DC  =>
	 	  audiodata_out <= "1000011000";
		  next_state <= S_CLK1;
    when S_CLK1 =>
	 	  audiodata_out(7 downto 0) <= counter_buffer(sample_pos)(7 downto 0);
		  audiodata_out(9 downto 8) <= even_parity_bits(counter_buffer(sample_pos)(7 downto 0), 8);
		  next_state <= S_CLK2;
    when S_CLK2 => 
	     audiodata_out(3 downto 0) <= counter_buffer(sample_pos)(11 downto 8);
		  audiodata_out(4) <= jump_point;
		  audiodata_out(7 downto 5) <= ( others => '0');
		  audiodata_out(9 downto 8) <= even_parity_bits(counter_buffer(sample_pos)(11 downto 8), 4);
		  next_state <= S_ACTIVEAUDIO;

    when S_ACTIVEAUDIO =>
	     audiodata_out <= activedata_in;
		  next_state <= S_ECC;
    when S_ECC =>
	     audiodata_out <= (others => '0');
		  next_state <= S_CS;
    when S_CS =>
	 	  audiodata_out <= (others => '1');
		  
		  if (sample_pos = sample_per_line) then
		  		next_state <= S_IDLE;
		  else
		  		next_state <= S_ADF1;
		  end if;
    when S_IDLE =>
	     
	     audiodata_out <= (others => '0');
	 	  if eav_tick = '1' and jump_point = '0' then
		  		next_state <= S_ADF1;
		  else
		  		next_state <= S_IDLE;
 		  end if;
end case;   
end process;

with line_o select
   jump_point <= '1' when conv_std_logic_vector(8,11),
		           '1' when conv_std_logic_vector(570,11),
	           	  '0' when others;

end Behavioral;
