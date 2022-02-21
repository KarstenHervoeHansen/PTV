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
use work.sinu_data.all;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
library UNISIM;
use UNISIM.VComponents.all;

entity audio_generator is
	 Port (                                              																   
        videoclk_i      : in std_logic;
        enable_i			: in std_logic;
        reset_i 			: in std_logic;
        sync_reset_i		: in std_logic;
     	  audiodata_i     : in std_logic_vector(23 downto 0);
	     fvh_i           : in std_logic_vector(2 downto 0);  
	     line_i          : in std_logic_vector(10 downto 0);  
		  system_i			: in std_logic_vector(7 downto 0);
		  signal_index_i  : in std_logic_vector(7 downto 0);
		  audiodata_tick_o: out std_logic; 
        audiochroma_o   : out std_logic_vector(9 downto 0);
        audioluma_o 		: out std_logic_vector(9 downto 0);
        debug           : out std_logic_vector(7 downto 0)

        );      
end audio_generator;

--        _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   
--f148.5   |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_  
--            ___     ___     ___      ___     ___     ___     ___     ___     ___     ___     ___     ___     ___     ___ 
--enable  ___|   |___|   |___|   |___ |   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |
--                            ___________________________________
--fvh(0)  ___________________|                                   |________________________________
--                                     __________________________________
--fvh_delay___________________________|                                  |___________________
--                                     _______
--eav_tick____________________________|       |_____________________________________________________

--       -------------------- ------------------------------------------------------------------
--line_o                     X   newline
--       -------------------- ------------------------------------------------------------------      
--                                     ___________________________________________________________
--switchpoint                         |       
--       -----------------------------        
--       ------------------------------------- -------------------------------------------------
--max_pr_line                   000011        X         000000
--       ------------------------------------- -------------------------------------------------   
--       ------------------------------------- -------------------------------------------------
--insert_now                                  X         
--       ------------------------------------- -------------------------------------------------   
--               _____________________________  ______________________________________  ______   _____   _____  ______
-- Current_state      idle                    X              S_INEAV                  X  ADF1  X  ADF2 X ADF3 X  DID  
--               -----------------------------  --------------------------------------  ------   ------ ------  ------
 
--system look-up table
  --------------------------------------------------------- 
  --system_i |  system     |max_pr_line|  
  --------------------------------------------------------
  --  X"A0"  |  1080i/60   |    2      |  
  --------------------------------------------------------
  --  X"B0"  |  1080i/59.94|    2      |  
  --------------------------------------------------------   
  --  X"A1"  |  1080i/50   |    3      |  
  --------------------------------------------------------   
  --  X"82"  |  1080p/30   |    2      |   
  --------------------------------------------------------  
  --  X"92"  |  1080p/29.97|    2      |   
  --------------------------------------------------------     
  --  X"83"  |  1080p/25   |    3      |   
  --------------------------------------------------------
  --  X"84"  |  1080p/24   |    3      |   
  --------------------------------------------------------
  --  X"94"  |  1080p/23.98|    3      |    
  -------------------------------------------------------- 
  --  X"A5"  |  1035i/60   |    x      |   
  -------------------------------------------------------- 
  --  X"B5"  |  1035i/59.94|    x      |   
  -------------------------------------------------------- 
  --  X"86"  |  720p/60    |    2      |   
  --------------------------------------------------------
  --  X"96"  |  720p/59.94 |    2      |   
  -------------------------------------------------------- 
  --  X"87"  |  720p/50    |    2      |   
  -------------------------------------------------------- 
  --  X"88"  |  720p/30    |    3      |   
  -------------------------------------------------------- 
  --  X"98"  |  720p/29.97 |    3      |   
  -------------------------------------------------------- 
  --  X"89"  |  720p/25    |    4      |   
  -------------------------------------------------------- 
  --  X"8A"  |  720p/24    |    4      |   
  -------------------------------------------------------- 
  --  X"9A"  |  720p/23.98 |    4      |   
  --------------------------------------------------------- 
  --  X"60"  |  NTSC 525i/59.94|  X    |  
  --------------------------------------------------------- 
  --  X"20"  |  PAL 625i/50 |     X    |  
  ----------------------------------------------------------                    

--framerate = [60,59.94,50,30,29.97,25,24,23.98,30,29.97,25,30,29.97,25,24,23.98];
--              2   2   2   3   3    4  4  4     2    2   3  2   2    3  3   3                                                                                                
architecture Behavioral of audio_generator is
type state_type is(S_ADF1, S_ADF2,S_ADF3,S_DID, S_DBN, S_DC, S_CLK1, S_CLK2, S_ACTIVEAUDIO, 
    S_ECC, S_CS, S_IDLE, S_INEAV); 


constant interval_1485i_48o : std_logic_vector(3 downto 0)  := "1000";      --8
constant compare_1485i_48o  : std_logic_vector(13 downto 0) := "11000001010111";   --12375
constant interval_1484i_48o : std_logic_vector(6 downto 0)  := "1011011";     --91  
constant compare_1484i_48o  : std_logic_vector(17 downto 0) := "100010010101010001";  --140625
constant interval_comp_1485 : std_logic_vector(13 downto 0) := conv_std_logic_vector(12375-8,14);    -- inteval1-compare1
constant interval_comp_1484 : std_logic_vector(17 downto 0) := conv_std_logic_vector(140625-91,18);  --inteval2-compare2
constant fifo_size      : integer := 8;

type fifo_array is array(0 to fifo_size-1) of std_logic_vector(13 downto 0);
--constant interval_1485i_441o: integer := 49;
--constant compare_1485i_441o : integer := 82500;
--constant interval_1484i_441o: integer := 4459;
--constant compare_1484i_441o : integer := 7500000;
--
--constant interval_1485i_32o: integer := 16;
--constant compare_1485i_32o : integer := 37125;
--constant interval_1484i_32o: integer := 182;
--constant compare_1484i_32o : integer := 421875;

signal fifo_data_in     : std_logic_vector(12 downto 0);
signal fifo_data_out    : std_logic_vector(12 downto 0);
signal fifo_data        : fifo_array;
signal fifo_write       : std_logic;
signal fifo_read        : std_logic;
signal read_write_eav   : std_logic_vector(2 downto 0);
signal previous_line    : std_logic;
signal fifo_write_point : std_logic_vector(fifo_size downto 0);
signal insert_now       : std_logic_vector(fifo_size-1 downto 0);
signal counter1: std_logic_vector(13 downto 0);
signal counter2: std_logic_vector(17 downto 0);
signal temp_clk_counter : std_logic_vector(12 downto 0);
signal index, sample_per_line:  integer range 0 to 3;
signal eav_tick,h_delayed_1   :  std_logic;	
signal current_state, next_state : state_type;
signal switch_point, control_point: std_logic;
signal state_count, next_state_count : integer range 0 to 31;
signal line_o        :std_logic_vector(10 downto 0);
signal video_count : integer range 0 to 2200;
signal audio_group,next_group: integer range 1 to 4;              
signal audio_chan, next_chan: integer range 1 to 4;
signal audio_data: std_logic_vector(23 downto 0);
signal aes_count, next_aes_count : integer range 0 to 192;
signal control_b1, sync_z, parity_b1: std_logic;
signal audio_chroma, audio_luma: std_logic_vector(9 downto 0);
signal ecc0, ecc1, ecc2, ecc3, ecc4, ecc5, cksm: std_logic_vector(7 downto 0);
signal cs_chroma, next_cs_chroma, cs_luma, next_cs_luma: std_logic_vector(8 downto 0);
signal ecc_stall, ecc_clear : std_logic;
signal comp_delayed, comp_delayed_1 : std_logic_vector(1 downto 0);
signal audio_tick_1484, audio_tick_1485, audiotick: std_logic;
signal next_audiodata_tick : std_logic;
signal chroma_reg: std_logic_vector(9 downto 0);
signal next_fifo_read: std_logic;
signal max_pr_line: std_logic_vector(fifo_size-1 downto 0);
begin
-- process only for simulation    !!!!!-------------------------------------
--make_enable: process(videoclk_in, reset_i)
--begin
-- if reset_in = '0' then
-- 	enable_in <= '0';
-- elsif videoclk_in'event and videoclk_in = '1' then
-- 	enable_in <= not enable_in;
-- end if;
-- end process; 
--
--make_fvh : process(videoclk_in, reset_in)
--begin
--if reset_in = '0' then
-- 	fvh_i <= "000";
-- 	video_count <= 0;
-- 	line_o <= (others => '0');   
--elsif videoclk_in'event and videoclk_in = '1' then
-- 	 if enable_in = '1' then
-- 	 	if video_count = 1099 then
-- 	  	 	fvh_i <= "000";
-- 	  	 	video_count <= video_count + 1;      
-- 		elsif video_count = 2199 then	  
-- 			fvh_i <= "001";	
-- 			video_count <= 0;
--		    line_o <= line_o + '1';
--  	 	else 
-- 	 		video_count <= video_count + 1; 	
-- 	 	end if;
-- 	 end if;  	
--end if;
--end process; 
---------------------------------------------------------------------------
----------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------
-- Make FIFO for clk data:
--------------------------------------------------------------------------------------------------

read_write_eav <= fifo_read & fifo_write & eav_tick;

clk_fifo : process(videoclk_i,reset_i)
begin
   if reset_i = '0' then
         fifo_write_point <= conv_std_logic_vector(1,fifo_size+1);
         insert_now       <= (others => '0');
         fifo_data        <= (others=>"00000000000000");
   elsif videoclk_i'event and videoclk_i = '1' then
      if sync_reset_i = '1' or fifo_write_point = 0 then
         fifo_write_point <= conv_std_logic_vector(1,fifo_size+1);
         insert_now       <= (others => '0');
         fifo_data        <= (others=>"00000000000000");
      elsif enable_i = '1' then
         case read_write_eav is
         when "000"   =>         -- no read, no write, no eav:
            null;
         when "001"|"101"   =>   -- no read, no write, eav: | do read, no write, eav: Read on eav should never happen - read ignored!
            for i in 0 to fifo_size-1 loop
               if max_pr_line(i) = '0' then
                  fifo_data(i)(13) <= '1';   -- tag elements that are not inserted next line (including unvalid elements)
               end if;
               if max_pr_line(i) = '1' and fifo_write_point(fifo_size downto i+1) /= 0 then  -- tag valid elements that are inserted now
                  insert_now(i)   <= '1';
               else
                  insert_now(i)   <= '0';
               end if;
            end loop;
         when "010"   =>         -- no read, do write, no eav:
            if fifo_write_point(fifo_size) = '0' then             -- only write if space in buffer:
               for i in 0 to fifo_size-1 loop
                  if fifo_write_point(i) = '1' then
                     fifo_data(i) <= '0' & fifo_data_in;
                  end if;  
               end loop;
               fifo_write_point <= fifo_write_point(fifo_size-1 downto 0) & '0';
            end if;
         when "011" | "111"   => -- no read, do write, eav | do read, do write, eav: Read on eav should never happen - read ignored!
            if fifo_write_point(fifo_size) = '0' then             -- only write if space in buffer:
               for i in 0 to fifo_size-1 loop
                  if fifo_write_point(i) = '1' then
                     fifo_data(i)(12 downto 0) <= fifo_data_in;
                  end if;
                 -- fifo_data(i)(13) <= not max_pr_line(i);     -- tag elements that are not inserted next line (mux_bit)
                  if max_pr_line(i) = '0' then
                     fifo_data(i)(13) <= '1';   -- tag elements that are not inserted next line (including unvalid elements)
                  end if;
                 
                  if max_pr_line(i) = '1' and fifo_write_point(fifo_size downto i) /= 0 then  -- tag valid elements that are inserted now
                     insert_now(i)   <= '1';
                  else
                     insert_now(i)   <= '0';
                  end if;
               end loop;
               fifo_write_point <= fifo_write_point(fifo_size-1 downto 0) & '0';  -- increase write pointer
            else                                                  -- if no space in buffer ignore write as when "001"
               for i in 0 to fifo_size-1 loop
                  if max_pr_line(i) = '0' then
                     fifo_data(i)(13) <= '1';   -- tag elements that are not inserted next line (including unvalid elements)
                  end if;
                  if max_pr_line(i) = '1' and fifo_write_point(fifo_size downto i+1) /= 0 then  -- tag valid elements that are inserted now
                     insert_now(i)   <= '1';
                  else
                     insert_now(i)   <= '0';
                  end if;
               end loop;
            end if;
         when "100"   =>         -- do read, no write, no eav:
            if fifo_write_point(0) = '0' then                     -- only if fifo contains data:
               for i in 0 to fifo_size-2 loop
                  fifo_data(i) <= fifo_data(i+1);
               end loop;
               insert_now   <= '0' & insert_now(fifo_size-1 downto 1);
               fifo_write_point <= '0' & fifo_write_point(fifo_size downto 1);
            end if;
         when "110"   =>         -- do read, do write, no eav:
            for i in 0 to fifo_size-2 loop
               if fifo_write_point(i+1) = '1' then
                  fifo_data(i) <= '0' & fifo_data_in;
               else
                  fifo_data(i) <= fifo_data(i+1);
               end if;
            end loop;
            insert_now   <= '0' & insert_now(fifo_size-1 downto 1);
         when others => -- never happens
            null;
         end case;
	   end if;
   end if;
end process;      


fifo_data_out  <= fifo_data(0)(12 downto 0);
previous_line  <= fifo_data(0)(13);
--fifo_empty     <= fifo_write_point(0);

--------------------------------------------------------------------------------------------------
-- generate tick for switch point, jump point(switch point -1), control point(jump point + 1)
--------------------------------------------------------------------------------------------------
define_switchpoint: process(videoclk_i,reset_i)
begin
   if  reset_i = '0' then
     switch_point <= '0'; 
     control_point <= '0';
   elsif videoclk_i'event and videoclk_i = '1' then
      if sync_reset_i = '1' then 
         switch_point <= '0'; 
         control_point <= '0';
       
      elsif enable_i = '1' then
	      case system_i is
         when X"A0"|X"B0"|X"A1" =>            -- 1080i/60 -- 1080i/59.94 -- 1080i/50
            	if line_i = "01000111001" or line_i = "00000000111" then  --7, 569
            		switch_point <= '1';
            	else
            	   switch_point <= '0';
            	end if;
            	if line_i = "01000111011" or line_i =  "00000001001" then  --9, 571
            		control_point <= '1';
               else
                  control_point <= '0';
            	end if;
         when others =>
            	if line_i = "00000000111" then  --7
            		switch_point <= '1';
            	else
            	   switch_point <= '0';
            	end if;
            		
            	if line_i =  "00000001001" then  --9
            		control_point <= '1';
            	else
                  control_point <= '0';           	
            	end if;
	      end case;
	   end if;
   end if;
end process;      

----------------------------------------------------
-- generate the pulse eav_tick, indicating new line
----------------------------------------------------                          
make_fvh0_delay : process(videoclk_i, reset_i)
begin
	if reset_i = '0' then
		h_delayed_1 <= '0';
		eav_tick <= '0';	
   elsif videoclk_i'event and videoclk_i = '1' then
		if sync_reset_i = '1' then
			h_delayed_1 <= '0';
			eav_tick <= '0';	
		elsif enable_i = '1' then  
		   h_delayed_1 <= fvh_i(0);
		   if fvh_i(0) = '1' and h_delayed_1 = '0' then
		      eav_tick <= '1';
	      else             		   
		      eav_tick <= '0';		   
		   end if;		      
      end if;
	end if;
end process;

-------------------------------------------------
-- audio tick for 148.4 Mhz clock
-------------------------------------------------
make_audio_sample_tick_1484 : process(videoclk_i, reset_i)
begin
   if reset_i = '0' then
		counter2                <= (others => '0');
  		comp_delayed            <= (others => '0');
  		audio_tick_1484         <= '0';
   elsif videoclk_i'event and videoclk_i = '1' then
		if sync_reset_i = '1' then
   		counter2             <= (others => '0');
     		comp_delayed         <= (others => '0');
     		audio_tick_1484      <= '0';
		elsif enable_i = '1' then  
		   if audio_tick_1484 = '1' then
		      counter2 <= counter2 - interval_comp_1484;
		   else
		      counter2 <= counter2 + interval_1484i_48o;
		   end if;
    		if counter2 < compare_1484i_48o then
    		   comp_delayed(0)   <= '0';
    		else
    		   comp_delayed(0)   <= '1';
    		end if;
    		comp_delayed(1)      <= comp_delayed(0);
    		if comp_delayed = "01" then   -- rising
    		   audio_tick_1484   <= '1';
    		else
    		   audio_tick_1484   <= '0';
    		end if;
      end if;
	end if;
end process;

-------------------------------------------------
-- audio tick for 148.5 Mhz clock:
-------------------------------------------------
make_audio_sample_tick_1485 : process(videoclk_i, reset_i)
begin
   if reset_i = '0' then
		counter1                <= (others => '0');
  		comp_delayed_1          <= (others => '0');
  		audio_tick_1485         <= '0';
   elsif videoclk_i'event and videoclk_i = '1' then
		if sync_reset_i = '1' then
   		counter1             <= (others => '0');
     		comp_delayed_1       <= (others => '0');
     		audio_tick_1485      <= '0';
		elsif enable_i = '1' then  
		   if audio_tick_1485 = '1' then
		      counter1 <= counter1 - interval_comp_1485;
		   else
		      counter1 <= counter1 + interval_1485i_48o;
		   end if;
    		if counter1 < compare_1485i_48o then
    		   comp_delayed_1(0)   <= '0';
    		else
    		   comp_delayed_1(0)   <= '1';
    		end if;
    		comp_delayed_1(1)      <= comp_delayed_1(0);
    		if comp_delayed_1 = "01" then   -- rising
    		   audio_tick_1485   <= '1';
    		else
    		   audio_tick_1485   <= '0';
    		end if;
      end if;
	end if;
end process;


make_audio_sample_tick_register:process(videoclk_i, reset_i)
begin
   if reset_i = '0' then
     elsif videoclk_i'event and videoclk_i = '1' then
		if sync_reset_i = '1' then
		elsif enable_i = '1' then
         case system_i is
            when X"B0"|X"92"|X"94"|X"B5"|X"96"|X"9A" =>
               audiotick <= audio_tick_1484;
            when others =>
               audiotick <= audio_tick_1485;
         end case;                            
      end if;
	end if;
end process;
           
                
 
---------------------------------------------------------------------------------------				 	
--define how many audio samples can be output for each line. 
--can be 2 or 3 according to the system lookup table, and 0 for a jump line in all systems. 
---------------------------------------------------------------------------------------			                    
define_max_pr_line: process(videoclk_i, reset_i)
begin
   if reset_i = '0' then 
		case system_i is 
		   when X"89"|X"8A"|X"9A" =>
		      max_pr_line <="00001111";
		   when X"83"|X"84"|X"94"|X"A1"|X"88"|X"98"  =>
            max_pr_line <="00000111";
		   when others =>
		      max_pr_line <="00000011";
		end case;  
	elsif videoclk_i'event and videoclk_i = '1' then	
		if sync_reset_i = '1' then		 
		   case system_i is 
		      when X"89"|X"8A"|X"9A" =>
		         max_pr_line <="00001111";
		      when X"83"|X"84"|X"94"|X"A1"|X"88"|X"98"  =>
               max_pr_line <="00000111";
		      when others =>
		         max_pr_line <="00000011";
		   end case;  
		elsif enable_i = '1' then	
		   if eav_tick = '1' then
		      if switch_point = '1' then
		         max_pr_line <= (others => '0');  --NB: max_pr_line can only be read at EAV_tick
		      else 
		         case system_i is 
		            when X"89"|X"8A"|X"9A" =>
		               max_pr_line <="00001111";
		            when X"83"|X"84"|X"94"|X"A1"|X"88"|X"98"  =>
                     max_pr_line <="00000111";
		            when others =>
		               max_pr_line <="00000011";
		         end case;
		         
		      end if;
		   end if;
	   end if;
	end if;
end process;
		  
	      	  
-----------------------------------------------------------------------		 	
--count the number of videoclk between audioclk and the previous eav	
-----------------------------------------------------------------------	 	
Clk_of_audiosample_to_nexteav: process(videoclk_i, reset_i)
begin
   if reset_i = '0' then 
		temp_clk_counter <= (others => '0');	
	elsif videoclk_i'event and videoclk_i = '1' then	
		if sync_reset_i = '1' then		
			temp_clk_counter <= (others => '0');
		elsif enable_i = '1' then	
			if eav_tick = '1' then
			   temp_clk_counter <= (others => '0');
			else
			   temp_clk_counter <= temp_clk_counter + '1';
			end if;
		end if;
	end if;
end process;

-----------------------------------------------	 	
--read data from the FIFO buffer when cs state
-----------------------------------------------	
FIFO_read_generation : process(videoclk_i, reset_i)
begin
   if reset_i = '0' then 	
       fifo_read <= '0';         
   elsif videoclk_i'event and videoclk_i = '1' then
      if sync_reset_i = '1' then		
          fifo_read <= '0';            
      elsif enable_i = '1' then
            fifo_read <= next_fifo_read;
      end if;
   end if;
end process;

------------------------------------------------------------	 	
--write data into the FIFO buffer when an audio sample comes
------------------------------------------------------------ 	
FIFO_write_generation : process(videoclk_i, reset_i)
begin
   if reset_i = '0' then  
   	fifo_write   <= '0';
		fifo_data_in <= (others => '0');
   elsif videoclk_i'event and videoclk_i = '1' then
		if sync_reset_i = '1' then		
   	   fifo_write   <= '0';
		   fifo_data_in <= (others => '0');		
		elsif enable_i = '1' then
 		   if audiotick  = '1' then               
				fifo_write   <= '1';
				fifo_data_in <= temp_clk_counter;
			else 
			   fifo_write   <= '0';
         end if;
      end if;
   end if;
end process;

------------------------------------------------------------	 	
--calculate the checksum. ECC(X)=X6+X5+X3+X2+X+1
------------------------------------------------------------ 	
make_ecc:process(videoclk_i, reset_i)
begin
   if reset_i = '0' then 
		ecc0 <= (others => '0');
		ecc1 <= (others => '0');
		ecc2 <= (others => '0');
		ecc3 <= (others => '0');
		ecc4 <= (others => '0');
		ecc5 <= (others => '0');
   elsif videoclk_i'event and videoclk_i = '1' then
		if sync_reset_i = '1' then							
			ecc0 <= (others => '0');
			ecc1 <= (others => '0');
			ecc2 <= (others => '0');
			ecc3 <= (others => '0');
			ecc4 <= (others => '0');
			ecc5 <= (others => '0');
		elsif enable_i = '1' then 	
		   if ecc_clear = '1' then       --make eccword			
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
	   end if;
   end if;
end process;
cksm <= audio_chroma(7 downto 0) xor ecc0;
	
audio_output : process(videoclk_i, reset_i)
begin
   if reset_i = '0' then
      chroma_reg <= (others => '0');		
		audioluma_o <= (others => '0');
	   audiochroma_o <= (others =>'0');
   elsif videoclk_i'event and videoclk_i = '1' then
		if sync_reset_i = '1' then			
		   chroma_reg <= (others => '0');		
		   audioluma_o <= (others => '0');
	      audiochroma_o <= (others =>'0');
		elsif enable_i = '1' then  	
		   if signal_index_i  = X"03" then        --00=silence, 01=sine, 02=click, 03=off
			   audioluma_o   <= "0001000000";
			   audiochroma_o <= "1000000000";
	      elsif system_i = X"60" or system_i = X"20" then   --sd signal, not implemented yet
			   audioluma_o   <= "0001000000";
			   audiochroma_o <= "1000000000";
			elsif control_point = '1' and audio_group = 1 then  --output control packet and data packet
				audioluma_o <= audio_luma;
				audiochroma_o <= audio_chroma;
			else                                               --output data packet
				audioluma_o <= "0001000000";  --same as luma blank output
			   audiochroma_o <= audio_chroma;
			end if;
			chroma_reg <= audio_chroma;   --registered output data for CRC use
		end if;
	end if;
end process;
                
state_machine_register : process(videoclk_i, reset_i)
begin
   if reset_i = '0' then
		audio_data <= (others => '0');
--		sample_per_line <= 0;
		current_state <= S_IDLE;
		state_count <= 0;
		cs_chroma <= (others => '0');
		cs_luma <= (others => '0');
		audio_group <= 1;
		audio_chan <= 1;
		aes_count <= 0;
		audiodata_tick_o <= '0';		
	 elsif videoclk_i'event and videoclk_i = '1' then
		if sync_reset_i = '1' then
			audio_data <= (others => '0');
--			sample_per_line <= 0;
			current_state <= S_IDLE;
			state_count <= 0;
			cs_chroma <= (others => '0');
			cs_luma <= (others => '0');
			audio_group <= 1;
			audio_chan <= 1;
			aes_count <= 0;
		   audiodata_tick_o <= '0';
      elsif enable_i = '1' then  
		   current_state <= next_state; 
		   state_count <= next_state_count;    
		   audio_chan <= next_chan;
		   audio_group <= next_group;	
		   cs_chroma <= next_cs_chroma;	
		   cs_luma <= next_cs_luma;	
		   audio_data <= audiodata_i;
         audiodata_tick_o <= next_audiodata_tick;
         aes_count <= next_aes_count;
      end if;
   end if;
end process;      

state_description: process(current_state, eav_tick, state_count, audio_group, audio_chan,audio_data,
   sample_per_line, aes_count,cs_chroma, cs_luma, audio_luma, audio_chroma,chroma_reg,insert_now,previous_line,
   fifo_data_out)
begin
next_chan <= audio_chan;
next_group <= audio_group;
next_aes_count <= aes_count;
next_state_count <= state_count;
ecc_stall <= '0';
ecc_clear <= '0';
next_cs_luma <= cs_luma;
next_audiodata_tick <= '0';
next_cs_chroma <= cs_chroma + chroma_reg(8 downto 0);
next_fifo_read <= '0';

if eav_tick = '1' then
   next_state <= S_INEAV;
else
   case current_state is
    when S_INEAV =>
        if insert_now(0) = '0' then  
            next_state <= S_IDLE;
    	  elsif state_count < 4 then
          	 next_state <= S_INEAV;
          	 next_state_count <= state_count + 1;
        else
             next_state_count <= 0;
          	 next_state <= S_ADF1;
 	 	  end if;
 	 	  audio_chroma <= "1000000000"; 
        audio_luma   <= "0001000000"; 
        next_group <= 1;
        next_cs_chroma <= (others => '0');
        next_cs_luma <= (others => '0');
        ecc_clear <= '1';
	when S_ADF1 =>          
	 	  audio_chroma <= (others => '0');
	 	  audio_luma <= (others => '0');
		  next_state <= S_ADF2;
		  next_cs_chroma <= (others => '0');
		  next_cs_luma <= cs_luma;
	when S_ADF2 => 
	 	  audio_chroma <= (others => '1');
	 	  audio_luma <= (others => '1');
		  next_state <= S_ADF3;
		  next_cs_chroma <= (others => '0');
		  next_cs_luma <= cs_luma + "111111111";
	when S_ADF3 => 
	 	  audio_chroma <= (others => '1');
	 	  audio_luma <= (others => '1');
		  next_state <= S_DID;
		  next_cs_chroma <= (others => '0');
		  next_cs_luma <= cs_luma + "111111111";
	when S_DID=> 
        next_state <= S_DBN;
        next_cs_chroma <= (others => '0');
		  if audio_group = 1 then
		        audio_chroma <= "1011100111";	--2E7 = channel 1-4;  
		        audio_luma   <= "0111100011";   --1e3
		        next_cs_luma   <= cs_luma   +  "111100011";
		  elsif audio_group = 2 then
		  		audio_chroma <= "0111100110";  --1E6 = channel 5-8;
		  		audio_luma   <= "1011100010";  --2e2  		
		        next_cs_luma   <= cs_luma   + "011100010"; 		  		
		  elsif audio_group = 3 then
		  		audio_chroma <= "0111100101";  --1E5 = channel 9-12;
		  		audio_luma   <= "1011100001";  --2e1		  		
		        next_cs_luma   <= cs_luma   + "011100001";		  		
		  else 
		  		audio_chroma <= "1011100100";  --2E4 = channel 13-16;
		  		audio_luma   <= "0111100000";  --1e0
		        next_cs_luma   <= cs_luma   + "111100000";        
	      end if;
	when S_DBN => 
	 	  audio_chroma <= "1000000000";	--disable continuity indication
	 	  audio_luma   <= "1000000000";   --always 200h
		  next_state <= S_DC;
		  next_cs_luma   <= cs_luma;
    when S_DC  =>
	 	  audio_chroma <= "1000011000"; --always 0x218
	 	  audio_luma   <= "0100001011";   --always 0x10B
		  next_state <= S_CLK1;
		  next_cs_luma   <= cs_luma   + "100001011";
    when S_CLK1 =>
	 	  audio_chroma(7 downto 0) <= fifo_data_out(7 downto 0);
		  audio_chroma(9 downto 8) <= even_parity_bits(fifo_data_out(7 downto 0), 8, 0);
		  audio_luma <= "1000000000"; -- AF field: frame numbering not available;
		  next_state <= S_CLK2;
		  next_cs_luma  <= cs_luma;
    when S_CLK2 => 
	     audio_chroma(3 downto 0) <= fifo_data_out(11 downto 8);
	     audio_chroma(4) <= previous_line;
	     audio_chroma(5) <= fifo_data_out(12);
		  audio_chroma(7 downto 6) <= (others => '0');
		  audio_chroma(9 downto 8) <= even_parity_bits(previous_line&fifo_data_out(12 downto 8), 6, 0);		--8 5 8
		  audio_luma <= "1000000000"; --Rate field: 48kHz synchronous audio
		  next_state <= S_ACTIVEAUDIO;
		  next_cs_luma <= cs_luma;
		  if audio_group = 4 then
		      next_fifo_read <= '1';
		  else
		      next_fifo_read <= '0';
		  end if;
    when S_ACTIVEAUDIO =>
    	if state_count = 0 then                
        	audio_chroma(7 downto 4) <= audio_data(3 downto 0);
        	audio_chroma(2 downto 0) <= (others => '0');
        	if audio_chan = 1 then
        		audio_chroma(3) <= sync_z;
        		audio_chroma(9 downto 8) <= even_parity_bits( sync_z & audio_data(3 downto 0), 5, 0);
        		audio_luma <= "1000001111";  --ACT field, all channels active
        		next_cs_luma <= cs_luma + "000001111";
			elsif audio_chan = 3 then
				audio_chroma(3) <= sync_z;
        		audio_chroma(9 downto 8) <= even_parity_bits( sync_z & audio_data(3 downto 0), 5, 0);
        		audio_luma <= "1000000000";
        		next_cs_luma <= cs_luma;
         elsif audio_chan = 2 then
        		audio_chroma(3) <= '0';
        		audio_chroma(9 downto 8) <= even_parity_bits( audio_data(3 downto 0), 4, 0);
        		audio_luma <= "1000000001";
        		next_cs_luma <= cs_luma +"000000001";
        	else
        		audio_chroma(3) <= '0';
        		audio_chroma(9 downto 8) <= even_parity_bits( audio_data(3 downto 0), 4, 0);
        		audio_luma <= "0001000000";
        	    next_cs_luma <= cs_luma;        --no more valid luma data
        	end if;

        elsif state_count = 1 then
        	audio_chroma(7 downto 0) <= audio_data(11 downto 4);
        	audio_chroma(9 downto 8) <= even_parity_bits(audio_data(11 downto 4), 8, 0);
        	if audio_chan = 1 then  
        		audio_luma <= "1000000001";
        		next_cs_luma <= cs_luma+"000000001";
         elsif audio_chan = 2 then
        		audio_luma <= "1000000000";
        		next_cs_luma <= cs_luma;       
        	elsif audio_chan = 3 then
        		audio_luma(8 downto 0) <= cs_luma;
        		audio_luma(9) <= not cs_luma(8);
        		next_cs_luma <= (others => '0');
        	else
        		audio_luma <= "0001000000";
        		next_cs_luma <= cs_luma;        --no more valid luma data
        	end if;		
        elsif state_count = 2 then                                                                      
        	audio_chroma(7 downto 0) <= audio_data(19 downto 12);                         
        	audio_chroma(9 downto 8) <= even_parity_bits(audio_data(19 downto 12), 8, 0);
        	if audio_chan = 1 or audio_chan = 2 then
        		audio_luma <= "1000000000";
        		next_cs_luma <= cs_luma;
        	else
        		audio_luma <= "0001000000";
        		next_cs_luma <= cs_luma;        --no more valid luma data
        	end if;
        else
        	audio_chroma(3 downto 0) <= audio_data(23 downto 20);
        	audio_chroma(5 downto 4) <= (others => '0'); --U: user bit(not used), V: validity bit 0=valid
        	audio_chroma(6) <= control_b1;      --control status bit 
        	audio_chroma(7) <= parity_b1;
        	audio_chroma(9 downto 8) <= even_parity_bits(audio_chroma(7 downto 0), 8, 0);
         if audio_chan = 1 or audio_chan = 2 then
        		audio_luma <= "1000000000";
        		next_cs_luma <= cs_luma;
        	else
        		audio_luma <= "0001000000";
        		next_cs_luma <= cs_luma;         --no more valid luma data
        	end if;
       	end if;	
        
       	if audio_chan = 4 and state_count = 3 then   --all four channels finished
       		next_state <= S_ECC;
       		next_state_count <= 0;	      
		    next_chan <= 1;	
       	elsif state_count = 3 then				   -- a channel finished
          	next_state <= S_ACTIVEAUDIO;
          	next_state_count <= 0;
          	next_chan <= audio_chan + 1;       	
 		elsif state_count < 3 then       	
          	next_state <= S_ACTIVEAUDIO;
          	next_state_count <= state_count + 1;
          	next_chan <= audio_chan; 
        else                                 --error situation, exit immediately
        	next_state <= S_ECC;
        	next_state_count <= 0;
        	next_chan <= 1;       	
		end if;
				
    when S_ECC =>
    	ecc_stall <= '1';
    	audio_luma <= "0001000000";
    	if state_count = 0 then
    		audio_chroma(7 downto 0) <= ecc0;
    		audio_chroma(9 downto 8) <= even_parity_bits(ecc0, 8, 0);
    		next_state <= S_ECC;
    	    next_state_count <= state_count + 1;
    	elsif state_count = 1 then
    		audio_chroma(7 downto 0) <= ecc1;
    		audio_chroma(9 downto 8) <= even_parity_bits(ecc1, 8, 0);
    		next_state <= S_ECC;
    	    next_state_count <= state_count + 1;	
    	elsif state_count = 2 then
    		audio_chroma(7 downto 0) <= ecc2;
    		audio_chroma(9 downto 8) <= even_parity_bits(ecc2, 8, 0);
    		next_state <= S_ECC;
    	    next_state_count <= state_count + 1;
    	elsif state_count = 3 then
    		audio_chroma(7 downto 0) <= ecc3;
    		audio_chroma(9 downto 8) <= even_parity_bits(ecc3, 8, 0);
    		next_state <= S_ECC;
    	    next_state_count <= state_count + 1;
    	elsif state_count = 4 then
    		audio_chroma(7 downto 0) <= ecc4;
    		audio_chroma(9 downto 8) <= even_parity_bits(ecc4, 8, 0);
    		next_state <= S_ECC;
    	    next_state_count <= state_count + 1;
    	else
    		audio_chroma(7 downto 0) <= ecc5;
    		audio_chroma(9 downto 8) <= even_parity_bits(ecc5, 8, 0);
 		  	next_state_count <= 0;    	  
    	  	next_state <= S_CS;  
	    end if;
		  
    when S_CS =>
    	  next_cs_chroma(8 downto 0) <= (others => '0');     
	 	  audio_chroma(8 downto 0) <= cs_chroma + chroma_reg(8 downto 0); 
		  audio_chroma(9) <= not audio_chroma(8);
		  audio_luma <= "0001000000";
		  if insert_now(0) = '0' and audio_group = 4 then  --finish with output for this line
		  		next_state <= S_IDLE;
		  		next_group <= 1;
            next_audiodata_tick <= '1';
            if aes_count = 191 then
               next_aes_count <= 0;
            else
               next_aes_count <= aes_count + 1;
            end if;
		  elsif audio_group = 4 then  --1 more data set in this line
		  		next_state <= S_ADF1;
		  		next_group <= 1;
            next_audiodata_tick <= '1';
            if aes_count = 191 then
               next_aes_count <= 0;
            else
               next_aes_count <= aes_count + 1;
            end if;
		  else								-- more channel group in the current dataset
		  		next_state <= S_ADF1;
		  		next_group <= audio_group + 1;    
		  end if;
		  ecc_clear <= '1';
    when others =>							--S_IDLE
	      audio_chroma <= "1000000000";  --same as chroma horizontal blank value
	      audio_luma <= "0001000000";    --same as luma horizontal blank value
         next_group <= 1;
		   next_chan <= 1;
         next_cs_luma <= (others => '0');
		   next_cs_chroma <= (others=> '0');
		   ecc_clear <= '1';
	 	   if eav_tick = '1' then
		  	   next_state <= S_INEAV;
		   else
		  	   next_state <= S_IDLE;
 		   end if; 
    end case;
end if;
end process;

parity_b1 <= even_parity_bits(audio_data, 24, 0)(0);	       

with aes_count select
	control_b1 <= '1' when 0,
	 					'1' when 7,
				  '0' when others;
    	 	
with aes_count select
	sync_z <= '1' when  0,
			  '0' when others;	           	 	

debug(7 downto 2) <= (others=>'0');
debug(1) <= audio_tick_1485;
debug(0) <= audio_tick_1484;			  
			  	  	           	 	
end Behavioral;
