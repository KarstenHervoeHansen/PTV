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
	 Port ( 
	 	clkmux_in 	   		: in std_logic_vector(2 downto 0);	-- 000 = 148.5->48, 001 = 148.35->48,-- 010 = 148.5 -> 44.1, 011 = 148.35-> 44.1,-- 100 = 148.5 -> 32,   101 = 148.35 -> 32	                                                            																   
        videoclk_in 		: in std_logic;
--      enable_in			: in std_logic;
        reset_in 			: in std_logic;
        sync_reset_in		: in std_logic;
	   	activedata_in      	: in std_logic_vector(9 downto 0);
--	   	fvh_i              	: in std_logic_vector(2 downto 0);  
--	   	line_o              : in std_logic_vector(10 downto 0);    
        audiodata_out	   	: out std_logic_vector(9 downto 0);
        audioclk_out 		: out std_logic);      
end audio_clk;

--        _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   
--f148.5   |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_  
--            ___     ___     ___     ___     ___     ___     ___     ___     ___     ___     ___     ___     ___     ___ 
--f74.25  ___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |
--                            ___________________________________
--fvh(0)  ___________________|                                   |________________________________
--                                    __________________________________
--fvh_delay__________________________|                                  |___________________
--                            _______
--eav_tick___________________|       |_____________________________________________________

--       -------------------- ------------------------------------------------------------------
--line_o                     X   newline
--       -------------------- ------------------------------------------------------------------               
--       ___________________________   _____________________________________________   ______   _____   _____  ______
-- CS                 idle           X              S_INEAV                          X  ADF1  X  ADF2 X ADF3 X  DID  
--       ---------------------------   ----------------------------------------------  ------   ------ ------  ------

architecture Behavioral of audio_clk is
type m_array is array(0 to 2) of std_logic_vector(12 downto 0);
type state_type is(S_ADF1, S_ADF2,S_ADF3,S_DID, S_DBN, S_DC, S_CLK1, S_CLK2, S_ACTIVEAUDIO, 
    S_ECC, S_CS, S_IDLE, S_INEAV); 
type m_array_4x4 is array(1 to 4, 1 to 4) of std_logic_vector(23 downto 0);
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
signal output_counter_buffer: m_array;
signal compare : integer range 0 to 7504459;
signal interval: integer range 0 to 4459;
signal counter : integer range 0 to 7504459; 
signal temp_clk_counter : std_logic_vector(11 downto 0);
signal index, sample_per_line:  integer range 0 to 3;
signal eav_tick,h_delayed_1   :  std_logic;	
signal current_state, next_state : state_type;
signal sample_pos : integer range 0 to 3;
signal jump_point,enable_in, switch_point: std_logic;
signal eav_count, next_eav_count : integer range 0 to 32;
signal line_o        :std_logic_vector(10 downto 0);
signal video_count : integer range 0 to 2200;
signal fvh_i:  std_logic_vector(2 downto 0);  
signal audio_group: integer range 1 to 4;              
signal audio_chan: integer range 1 to 4;
signal audio_data: m_array_4x4;      --audio_data[group][channel] : audio data channel 5 = audio_data[2][1]
signal control_bit, sync_z, parity_b1: std_logic;

--signal audiodata_ch1, audiodata_ch2, audiodata_ch3, audiodata_ch4:  std_logic_vector(23 downto 0);
--signal audiodata_ch5, audiodata_ch6, audiodata_ch7, audiodata_ch8:  std_logic_vector(23 downto 0);
--signal audiodata_ch9, audiodata_ch10, audiodata_ch11, audiodata_ch12:  std_logic_vector(23 downto 0);
--signal audiodata_ch13, audiodata_ch14, audiodata_ch15, audiodata_ch16:  std_logic_vector(23 downto 0);

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

-- process only for simulation    !!!!!-------------------------------------
make_enable: process(videoclk_in, reset_in)
begin
 if reset_in = '0' then
 	enable_in <= '0';
 elsif videoclk_in'event and videoclk_in = '1' then
 	enable_in <= not enable_in;
 end if;
 end process; 

make_fvh : process(videoclk_in, reset_in)
begin
if reset_in = '0' then
 	fvh_i <= "000";
 	video_count <= 0;
 	line_o <= (others => '0');   
elsif videoclk_in'event and videoclk_in = '1' then
 	 if enable_in = '1' then
 	 	if video_count = 1099 then
 	  	 	fvh_i <= "000";
 	  	 	video_count <= video_count + 1;      
 		elsif video_count = 2199 then	  
 			fvh_i <= "001";	
 			video_count <= 0;
		    line_o <= line_o + '1';
  	 	else 
 	 		video_count <= video_count + 1; 	
 	 	end if;
 	 end if;  	
end if;
end process; 
---------------------------------------------------------------------------
----------------------------------------------------------------------------

 
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
	elsif sync_reset_in = '1' then
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
		counter_buffer(0)<= (others =>'0');
        counter_buffer(1)<= (others =>'0');
		counter_buffer(2)<= (others =>'0');		
		output_counter_buffer(0)<= (others =>'0');
        output_counter_buffer(1)<= (others =>'0');
		output_counter_buffer(2)<= (others =>'0');	
		eav_count <= 0;
   elsif videoclk_in'event and videoclk_in = '1' then
		if sync_reset_in = '1' then
			counter <= 0;	 
			audioclk_out <= '0';
			temp_clk_counter <= (others => '0');
			sample_per_line <= 0;
			index <= 0;
			current_state <= S_IDLE;
			counter_buffer(0)<= (others =>'0');
        	counter_buffer(1)<= (others =>'0');
			counter_buffer(2)<= (others =>'0');		
			output_counter_buffer(0)<= (others =>'0');
        	output_counter_buffer(1)<= (others =>'0');
			output_counter_buffer(2)<= (others =>'0');
			eav_count <= 0;
		elsif enable_in = '1' then  
		    current_state <= next_state; 
		    eav_count <= next_eav_count;    
			if eav_tick = '1' then
				temp_clk_counter <= (others => '0');
				
				if  jump_point = '0' then  							
					output_counter_buffer(0) <= counter_buffer(0);
					output_counter_buffer(1) <= counter_buffer(1);
					if index = 3 then   --3 sets of data ready, special situation for jump point
						counter_buffer(0) <= counter_buffer(2); --maximum 2 sets of data per line, save third to next line
						index <= 1;    --for next line, start storing data into buffer(1) 
						sample_per_line <= 1;   --2 sets of data in current line
					elsif index = 2 then --2 sets of data ready
						index <= 0;
						sample_per_line <= 1;
					else 
					    index <= 0;    
					    sample_per_line <= 0;
					end if;
		   		end if;
		   	else        --count the number of videoclk between audioclk and the previous eav
				temp_clk_counter <= temp_clk_counter + '1';
			end if;
			
			--generate 48kHz audioclk
	        if counter < compare then    
				counter <= counter + interval; 
				audioclk_out <= '0';				
   	   		else    --when time for an audioclk tick, store temp_clk_counter into a buffer
				counter <= counter - compare + interval ;
				audioclk_out <= '1';
				index <= index + 1;			
				if switch_point = '1' then
					counter_buffer(index) <= '1' & temp_clk_counter;
				elsif index = 2 then
					counter_buffer(index) <= '1' & temp_clk_counter;
				else
					counter_buffer(index) <= '0' & temp_clk_counter;

 	    		end if;
			end if;    
			
			if current_state = S_INEAV then
				audio_group <= 1;  
				sample_pos <= 0;	
			elsif current_state = S_CS then
				if audio_group < 4 then
					audio_group <= audio_group + 1;	   
				else
					audio_group <= 1;
					sample_pos <= sample_pos + 1;	
				end if;
			elsif current_state = S_IDLE then
				sample_pos <= 0;
				audio_group <= 1;
			end if;
		end if;
	end if;
end process;

state_description: process(current_state, eav_tick, eav_count,audio_group)
begin
case current_state is
    when S_INEAV => 
    	  if eav_count < 5 then
          	 next_state <= S_INEAV;
          	 next_eav_count <= eav_count + 1;
          	 audiodata_out <= (others => '0'); 
          else
             next_eav_count <= 0;
          	 next_state <= S_ADF1;
 	 	     audiodata_out <= (others => '0'); 
 	 	  end if;
     
	when S_ADF1 =>          
		  next_eav_count <= eav_count;
	 	  audiodata_out <= (others => '0');
		  next_state <= S_ADF2;
	when S_ADF2 => 
	      next_eav_count <= eav_count;
	 	  audiodata_out <= (others => '1');
		  next_state <= S_ADF3;
	when S_ADF3 => 
	      next_eav_count <= eav_count;
	 	  audiodata_out <= (others => '1');
		  next_state <= S_DID;
	when S_DID=> 
		  next_eav_count <= eav_count;
          next_state <= S_DBN;
		  if audio_group = 1 then
		        audiodata_out <= "1011100111";	--2E7 = channel 1-4;   
		  elsif audio_group = 2 then
		  		audiodata_out <= "0111100110";  --1E6 = channel 5-8;
		  elsif audio_group = 3 then
		  		audiodata_out <= "0111100101";  --1E5 = channel 9-12;
		  else 
		  		audiodata_out <= "1011100100";  --2E4 = channel 13-16;
          end if;
	when S_DBN => 
	      next_eav_count <= eav_count;
	 	  audiodata_out <= "1000000000";	--disable continuity indication
		  next_state <= S_DC;
    when S_DC  =>
          next_eav_count <= eav_count;
	 	  audiodata_out <= "1000011000";
		  next_state <= S_CLK1;
    when S_CLK1 =>
          next_eav_count <= eav_count;
	 	  audiodata_out(7 downto 0) <= output_counter_buffer(sample_pos)(7 downto 0);
		  audiodata_out(9 downto 8) <= even_parity_bits(output_counter_buffer(sample_pos)(7 downto 0), 8, 0);
		  next_state <= S_CLK2;
    when S_CLK2 => 
          next_eav_count <= eav_count;
	      audiodata_out(4 downto 0) <= output_counter_buffer(sample_pos)(12 downto 8);
		  audiodata_out(7 downto 5) <= ( others => '0');
		  audiodata_out(9 downto 8) <= even_parity_bits(output_counter_buffer(sample_pos)(12 downto 8), 5, 8);
		  next_state <= S_ACTIVEAUDIO;

    when S_ACTIVEAUDIO =>
		if eav_count = 0 then                --UDW2
        	audiodata_out(7 downto 3) <= audio_data(audio_group, 1)(3 downto 0) & sync_z;
        	audiodata_out(2 downto 0) <= (others => '0');
        	audiodata_out(9 downto 8) <= even_parity_bits( sync_z & audio_data(audio_group, 1)(3 downto 0), 5, 0);
        elsif eav_count = 1 then
        	audiodata_out(7 downto 0) <= audio_data(audio_group, 1)(11 downto 4);
        	audiodata_out(9 downto 8) <= even_parity_bits(audio_data(audio_group, 1)(11 downto 4), 8, 4);		
        elsif eav_count = 2 then                                                                      
        	audiodata_out(7 downto 0) <= audio_data(audio_group, 1)(19 downto 12);                         
        	audiodata_out(9 downto 8) <= even_parity_bits(audio_data(audio_group, 1)(19 downto 12), 8, 12);
        elsif eav_count = 3 then
        	audiodata_out(3 downto 0) <= audio_data(audio_group,1)(23 downto 20);
        	audiodata_out(5 downto 4) <= (others => '0'); --U: user bit(not used), V: validity bit 0=valid
        	audiodata_out(6) <= control_bit;      --control status bit 
        	audiodata_out(7) <= parity_b1;
        	audiodata_out(9 downto 8) <= even_parity_bits(control_bit& parity_b1 & audio_data(audio_group,1)(23 downto 20), 8, 20);
        end if;	
        	
          if eav_count < 15 then
          	next_state <= S_ACTIVEAUDIO;
          	next_eav_count <= eav_count + 1;
          else
            next_eav_count <= 0;	      
		    next_state <= S_ECC;
		  end if;
		  audiodata_out <= activedata_in;
    when S_ECC =>
    	  if eav_count < 5 then
    	  	next_state <= S_ECC;
    	    next_eav_count <= eav_count + 1;
    	  else
 		  	next_eav_count <= 0;    	  
    	  	next_state <= S_CS;  
	      end if;
    	  audiodata_out <= (others => '0');
		  
    when S_CS =>
          next_eav_count <= eav_count;       
	 	  audiodata_out <= (others => '1');

		  if sample_pos = sample_per_line and audio_group = 4 then
		  		next_state <= S_IDLE;
		  else
		  		next_state <= S_ADF1;		  		
		  end if;
	  
    when S_IDLE =>
         next_eav_count <= eav_count;     
	     audiodata_out <= (others => '0');
	 	  if eav_tick = '1' and jump_point = '0' then
		  		next_state <= S_INEAV;
		  else
		  		next_state <= S_IDLE;
 		  end if;
end case;   
end process;

with line_o select
    jump_point 	<= '1' when conv_std_logic_vector(8,11),
		           '1' when conv_std_logic_vector(570,11),
	               '0' when others;
	               
with line_o select
	switch_point <= '1' when conv_std_logic_vector(7,11),
			        '1' when conv_std_logic_vector(569,11),
	           	 	'0' when others;

parity_b1 <= even_parity_bits(audio_data(audio_group, 1), 24, 0)(0);	           	 	
	           	 	
	           	 	
end Behavioral;
