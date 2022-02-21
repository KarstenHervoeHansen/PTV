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
--	   	audiodata_in      	: in std_logic_vector(9 downto 0);
--	   	fvh_i              	: in std_logic_vector(2 downto 0);  
--	   	line_o              : in std_logic_vector(10 downto 0);    
        audiodata_out	   	: out std_logic_vector(9 downto 0);
        audioclk_out 		: out std_logic);      
end audio_clk;

--        _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   
--f148.5   |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_  
--            ___     ___     ___      ___     ___     ___     ___     ___     ___     ___     ___     ___     ___     ___ 
--f74.25  ___|   |___|   |___|   |___ |   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |
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
signal sample_pos, next_pos : integer range 0 to 3;
signal jump_point,enable_in, switch_point: std_logic;
signal eav_count, next_eav_count : integer range 0 to 32;
signal line_o        :std_logic_vector(10 downto 0);
signal video_count : integer range 0 to 2200;
signal fvh_i:  std_logic_vector(2 downto 0);  
signal audio_group,next_group: integer range 1 to 4;              
signal audio_chan, next_chan: integer range 1 to 4;
--signal audio_data: m_array_4x4;      --audio_data[group][channel] : audio data channel 5 = audio_data[2][1]
signal audio_data: std_logic_vector(23 downto 0);
signal aes_count, next_aes_count : integer range 0 to 192;
signal control_b1, sync_z, parity_b1: std_logic;
signal audio_output: std_logic_vector(9 downto 0);
signal ecc0, ecc1, ecc2, ecc3, ecc4, ecc5, cksm: std_logic_vector(7 downto 0);
signal cs, next_cs : std_logic_vector(8 downto 0);
signal ecc_stall, ecc_clear : std_logic;
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
		ecc0 <= (others => '0');
		ecc1 <= (others => '0');
		ecc2 <= (others => '0');
		ecc3 <= (others => '0');
		ecc4 <= (others => '0');
		ecc5 <= (others => '0');
		cs <= (others => '0');
		audio_group <= 1;
		audio_chan <= 1;
		sample_pos <= 0;
		aes_count <= 0;
		audiodata_out <= (others => '0');
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
			ecc0 <= (others => '0');
			ecc1 <= (others => '0');
			ecc2 <= (others => '0');
			ecc3 <= (others => '0');
			ecc4 <= (others => '0');
			ecc5 <= (others => '0');
			cs <= (others => '0');
			audio_group <= 1;
			audio_chan <= 1;
			sample_pos <= 0;
			aes_count <= 0;
			audiodata_out <= (others => '0');
		elsif enable_in = '1' then  
		    current_state <= next_state; 
		    eav_count <= next_eav_count;    
		    audio_chan <= next_chan;
		    sample_pos <= next_pos;
		    audio_group <= next_group;	
		    cs <= next_cs;		
			if next_aes_count = 192 then
				aes_count <= 0;
			else
				aes_count <= next_aes_count;
			end if;	  
			
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
   			
			--make eccword
			if ecc_clear = '1' then
				ecc5 <= (others => '0');
				ecc4 <= (others => '0');
				ecc3 <= (others => '0');
				ecc2 <= (others => '0');
				ecc1 <= (others => '0');
				ecc0 <= (others => '0');
			elsif ecc_stall = '0' then
				ecc5 <= cksm;
				ecc4 <= ecc5 xor cksm;
				ecc3 <= ecc4 xor cksm;
				ecc2 <= ecc3 xor cksm;
				ecc1 <= ecc2;
				ecc0 <= ecc1 xor cksm;
			end if;	
			audiodata_out <= audio_output;  --clock output data out
		end if;
	end if;
end process;

cksm <= audio_output(7 downto 0) xor ecc0;


state_description: process(current_state, eav_tick,jump_point, eav_count, audio_group, audio_chan,sample_pos,sample_per_line, aes_count,cs)
begin
next_chan <= audio_chan;
next_group <= audio_group;
next_pos <= sample_pos;
next_aes_count <= aes_count;
next_eav_count <= eav_count;
ecc_stall <= '0';
ecc_clear <= '0';
case current_state is
    when S_INEAV => 
    	  if eav_count < 8 then
          	 next_state <= S_INEAV;
          	 next_eav_count <= eav_count + 1;
          	 audio_output <= (others => '0'); 
          else
             next_eav_count <= 0;
          	 next_state <= S_ADF1;
 	 	     audio_output <= (others => '0'); 
 	 	  end if;
          next_group <= 1;
          next_pos <= 0;
          next_cs <= cs + audio_output(8 downto 0);
	when S_ADF1 =>          
	 	  audio_output <= (others => '0');
		  next_state <= S_ADF2;
		  next_cs<= cs + audio_output(8 downto 0);
	when S_ADF2 => 
	 	  audio_output <= (others => '1');
		  next_state <= S_ADF3;
		  next_cs <= cs + audio_output(8 downto 0);
	when S_ADF3 => 
	 	  audio_output <= (others => '1');
		  next_state <= S_DID;
		  next_cs <= cs + audio_output(8 downto 0);
	when S_DID=> 
          next_state <= S_DBN;
          next_cs <= cs + audio_output(8 downto 0);
		  if audio_group = 1 then
		        audio_output <= "1011100111";	--2E7 = channel 1-4;   
		  elsif audio_group = 2 then
		  		audio_output <= "0111100110";  --1E6 = channel 5-8;
		  elsif audio_group = 3 then
		  		audio_output <= "0111100101";  --1E5 = channel 9-12;
		  else 
		  		audio_output <= "1011100100";  --2E4 = channel 13-16;
          end if;
	when S_DBN => 
	 	  audio_output <= "1000000000";	--disable continuity indication
		  next_state <= S_DC;
		  next_cs <= cs+ audio_output(8 downto 0);
    when S_DC  =>
	 	  audio_output <= "1000011000";
		  next_state <= S_CLK1;
		  next_cs <= cs + audio_output(8 downto 0);
    when S_CLK1 =>
	 	  audio_output(7 downto 0) <= output_counter_buffer(sample_pos)(7 downto 0);
		  audio_output(9 downto 8) <= even_parity_bits(output_counter_buffer(sample_pos)(7 downto 0), 8, 0);
		  next_state <= S_CLK2;
		  next_cs<= cs + audio_output(8 downto 0);
    when S_CLK2 => 
	      audio_output(4 downto 0) <= output_counter_buffer(sample_pos)(12 downto 8);
		  audio_output(7 downto 5) <= ( others => '0');
		  audio_output(9 downto 8) <= even_parity_bits(output_counter_buffer(sample_pos)(12 downto 8), 5, 8);
		  next_state <= S_ACTIVEAUDIO;
		  next_cs <= cs + audio_output(8 downto 0);
    when S_ACTIVEAUDIO =>
    	next_cs <= cs + audio_output(8 downto 0);
		if eav_count = 0 then                
        	audio_output(7 downto 4) <= audio_data(3 downto 0);
        	audio_output(2 downto 0) <= (others => '0');
        	
        	if audio_chan = 1 or audio_chan = 3 then
        		audio_output(3) <= sync_z;
        		audio_output(9 downto 8) <= even_parity_bits( sync_z & audio_data(3 downto 0), 5, 0);
        	else
        		audio_output(3) <= '0';
        		audio_output(9 downto 8) <= even_parity_bits( audio_data(3 downto 0), 4, 0);
        	end if;
        	
        elsif eav_count = 1 then
        	audio_output(7 downto 0) <= audio_data(11 downto 4);
        	audio_output(9 downto 8) <= even_parity_bits(audio_data(11 downto 4), 8, 4);		
        elsif eav_count = 2 then                                                                      
        	audio_output(7 downto 0) <= audio_data(19 downto 12);                         
        	audio_output(9 downto 8) <= even_parity_bits(audio_data(19 downto 12), 8, 12);
        else
        	audio_output(3 downto 0) <= audio_data(23 downto 20);
        	audio_output(5 downto 4) <= (others => '0'); --U: user bit(not used), V: validity bit 0=valid
        	audio_output(6) <= control_b1;      --control status bit 
        	audio_output(7) <= parity_b1;
        	audio_output(9 downto 8) <= even_parity_bits(audio_output(7 downto 0), 8, 0);
        	--audio_output(9 downto 8) <= even_parity_bits(control_b1& parity_b1 & audio_data(23 downto 20), 6, 20);
       	end if;	
        
       	if audio_chan = 4 and eav_count = 3 then   --all four channels finished
       		next_state <= S_ECC;
       		next_eav_count <= 0;	      
		    next_chan <= 1;	
       	elsif eav_count = 3 then				   -- a channel finished
          	next_state <= S_ACTIVEAUDIO;
          	next_eav_count <= 0;
          	next_chan <= audio_chan + 1;       	
 		else       	
          	next_state <= S_ACTIVEAUDIO;
          	next_eav_count <= eav_count + 1;
          	next_chan <= audio_chan;          	
		end if;
		
		
    when S_ECC =>
    	next_cs <= cs + audio_output(8 downto 0);
    	ecc_stall <= '1';
    	
    	if eav_count = 0 then
    		audio_output(7 downto 0) <= ecc0;
    		audio_output(9 downto 8) <= even_parity_bits(ecc0, 8, 0);
    		next_state <= S_ECC;
    	    next_eav_count <= eav_count + 1;
    	elsif eav_count = 1 then
    		audio_output(7 downto 0) <= ecc1;
    		audio_output(9 downto 8) <= even_parity_bits(ecc1, 8, 0);
    		next_state <= S_ECC;
    	    next_eav_count <= eav_count + 1;	
    	elsif eav_count = 2 then
    		audio_output(7 downto 0) <= ecc2;
    		audio_output(9 downto 8) <= even_parity_bits(ecc2, 8, 0);
    		next_state <= S_ECC;
    	    next_eav_count <= eav_count + 1;
    	elsif eav_count = 3 then
    		audio_output(7 downto 0) <= ecc3;
    		audio_output(9 downto 8) <= even_parity_bits(ecc3, 8, 0);
    		next_state <= S_ECC;
    	    next_eav_count <= eav_count + 1;
    	elsif eav_count = 4 then
    		audio_output(7 downto 0) <= ecc4;
    		audio_output(9 downto 8) <= even_parity_bits(ecc4, 8, 0);
    		next_state <= S_ECC;
    	    next_eav_count <= eav_count + 1;
    	elsif eav_count = 5 then
    		audio_output(7 downto 0) <= ecc5;
    		audio_output(9 downto 8) <= even_parity_bits(ecc5, 8, 0);
 		  	next_eav_count <= 0;    	  
    	  	next_state <= S_CS;  
	    end if;
		  
    when S_CS =>
    	  next_cs(8 downto 0) <= (others => '0');     
	 	  audio_output(8 downto 0) <= cs; 
		  audio_output(9) <= not audio_output(8);
		  
		  if sample_pos = sample_per_line and audio_group = 4 then  --finish with output for this line
		  		next_state <= S_IDLE;
		  		next_group <= 1;
		  		next_pos <= 0;
		  		next_aes_count <= aes_count + 1;
		  elsif audio_group = 4 then  --1 more data set in this line
		  		next_state <= S_ADF1;
		  		next_group <= 1;
		  	    next_pos <= sample_pos + 1;	
		  	    next_aes_count <= aes_count + 1;
		  else								-- more channel group in the current dataset
		  		next_state <= S_ADF1;
		  		next_group <= audio_group + 1;    
	  		
		  end if;
		  ecc_clear <= '1';
    when S_IDLE =>
	     audio_output <= (others => '0');
	     next_pos <= 0;
		 next_group <= 1;
		 next_chan <= 1;
		 next_cs <= (others=> '0');
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

parity_b1 <= even_parity_bits(audio_data, 24, 0)(0);	       


with aes_count select
	control_b1 <= '1' when 0,
				  '0' when others;
    	 	
with aes_count select
	sync_z <= '1' when  0,
			  '0' when others;	           	 	

audio_data <= (others => '0');
		  	           	 	
end Behavioral;
