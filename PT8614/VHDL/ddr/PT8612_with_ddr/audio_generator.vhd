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
    S_ECC, S_CS, S_IDLE, S_INEAV,S_AF,S_RATE,S_ACT,S_DEL,S_RSRV); 


constant interval_1485i_48o : std_logic_vector(3 downto 0)  := "1000";      --8
constant compare_1485i_48o  : std_logic_vector(13 downto 0) := "11000001010111";   --12375
constant interval_1484i_48o : std_logic_vector(6 downto 0)  := "1011011";     --91  
constant compare_1484i_48o  : std_logic_vector(17 downto 0) := "100010010101010001";  --140625
constant interval_comp_1485 : std_logic_vector(13 downto 0) := conv_std_logic_vector(12375-8,14);    -- inteval1-compare1
constant interval_comp_1484 : std_logic_vector(17 downto 0) := conv_std_logic_vector(140625-91,18);  --inteval2-compare2

constant interval_135i_48o : std_logic_vector(2 downto 0)  := "100";      --4 ,sd        
constant compare_135i_48o  : std_logic_vector(10 downto 0) := "10001100101";   --1125    sd
constant interval_comp_135 : std_logic_vector(10 downto 0) := conv_std_logic_vector(1125-4,11); 
constant fifo_size      : integer := 8;

type fifo_array is array(0 to fifo_size-1) of std_logic_vector(13 downto 0);
type audiodata_array_4 is array(0 to 3) of std_logic_vector(25 downto 0);
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
signal index:  integer range 0 to 3;
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
signal control_b1, sync_z: std_logic;
signal audio_chroma, audio_luma: std_logic_vector(9 downto 0);
signal ecc0, ecc1, ecc2, ecc3, ecc4, ecc5, cksm: std_logic_vector(7 downto 0);
signal cs_chroma, next_cs_chroma, cs_luma, next_cs_luma: std_logic_vector(8 downto 0);
signal ecc_stall, ecc_clear : std_logic;
signal comp_delayed, comp_delayed_1 : std_logic_vector(1 downto 0);
signal audio_tick_1484, audio_tick_1485, audiotick: std_logic;
signal current_audiodata_tick,next_audiodata_tick : std_logic;
signal chroma_reg,luma_reg: std_logic_vector(9 downto 0);
signal next_fifo_read: std_logic;
signal max_pr_line: std_logic_vector(fifo_size-1 downto 0);
signal sample_count : std_logic_vector(7 downto 0);
signal next_control_group, control_group: integer range 0 to 7;
signal next_control_state_count,control_state_count: integer range 0 to 7;
signal current_control_state,next_control_state:state_type;

signal counter_sd: std_logic_vector(10 downto 0);     
signal comp_delayed_sd: std_logic_vector(1 downto 0);
signal audio_tick_sd: std_logic;  
signal sample_per_line: integer range 0 to 7;
signal sample_amount: integer range 0 to 7;  
signal current_sd_state,next_sd_state : state_type;
signal sd_chan,next_sd_chan: integer range 0 to 3;
signal sd_sample, next_sd_sample: integer range 0 to 192;
signal sd_statecount, next_sd_statecount: integer range 0 to 63;
signal sd_chroma, sd_chroma_reg, sd_luma, sd_luma_reg: std_logic_vector(9 downto 0);
signal sd_cs, next_sd_cs: std_logic_vector(8 downto 0);
signal sd_audio_data: std_logic_vector(19 downto 0);
signal sd_control_b1,sd_sync_z: std_logic;
signal sd_control_b1_direct,sd_sync_z_direct: std_logic;
signal sd_wordnr, next_sd_wordnr: integer range 0 to 3;
signal sd_hd: std_logic;  --0:sd, 1:hd
signal sd_tick, next_sd_tick : std_logic;
signal sd_group, next_sd_group : integer range 0 to 7;
signal eav_tick_previous : std_logic;
signal line_count: integer range 0 to 15;
signal frame_count: integer range 0 to 7;

signal audio_buffer: audiodata_array_4;
signal buffer_index, next_buffer_index: integer range 0 to 3;
signal write_buffer: std_logic;
signal sd_sel, next_sd_sel: std_logic;
signal sd_data_en: std_logic;
signal next_hd_dbn,hd_dbn: std_logic_vector(7 downto 0);
signal next_sd_dbn,sd_dbn: std_logic_vector(7 downto 0);

signal use_f1485 : std_logic;
signal system_max_per_line : std_logic_vector(7 downto 0);

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

with system_i select
   sd_hd <= '0' when X"20",
            '0' when X"60",
            '1' when others;
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
         when X"A0"|X"B0"|X"A1"|X"A2"|X"B2"|X"A3"|X"A4"|X"B4" =>         -- 1080i/sF 
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
         when X"20" =>                        --PAL
            if line_i = "00000000110" or line_i = "00000000111" or line_i = "00000001000" 
            or line_i = "00100111111" or line_i = "00101000000" or line_i = "00101000001" then  --6,319
               switch_point <= '1';
            else
               switch_point <= '0';
            end if;
         when X"60" =>             --NTSC
            if line_i = "00000001010" or line_i = "00000001011" or line_i = "00000001100" or
            line_i = "00100010001" or line_i = "00100010010" or line_i = "00100010011"  then  --10, 273
               switch_point <= '1';
            else
               switch_point <= '0';
            end if;
         when others =>            -- progressive hd
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

make_previous_tick: process(fvh_i,h_delayed_1)
begin
   if fvh_i(0) = '1' and h_delayed_1 = '0' then
      eav_tick_previous <= '1';     --make in-advanced eav_tick to place SD signal correctly   
   else             		                         
      eav_tick_previous <= '0';		                       
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

-------------------------------------------------
-- audio tick for SD clock( from 148.5 Mhz )
-------------------------------------------------
--make_audio_sample_tick_sd: process(videoclk_i, reset_i)
--begin
--   if reset_i = '0' then
--		counter_sd                <= (others => '0');
--  		comp_delayed_sd          <= (others => '0');
--  		audio_tick_sd         <= '0';
--   elsif videoclk_i'event and videoclk_i = '1' then
--		if sync_reset_i = '1' then
--   		counter_sd             <= (others => '0');
--     		comp_delayed_sd       <= (others => '0');
--     		audio_tick_sd      <= '0';
--		elsif enable_i = '1' then  
--		   if sd_hd = '0' then -- and switch_point = '0'  then
--		         if audio_tick_sd = '1' then		         
--		            counter_sd <= counter_sd - interval_comp_135;
--		         else
--		            counter_sd <= counter_sd + interval_135i_48o;
--		            
--		         end if;
--    		      
--		         if counter_sd < compare_135i_48o then
--    		         comp_delayed_sd(0)   <= '0';
--    		      else
--    		         comp_delayed_sd(0)   <= '1';
--    		      end if;
--    		      comp_delayed_sd(1)      <= comp_delayed_sd(0);
--    		      if comp_delayed_sd = "01" then   -- rising
--    		         audio_tick_sd   <= '1';
--    		      else
--    		         audio_tick_sd   <= '0';    		         
--    		      end if;
--         end if;
--      end if;
--	end if;
--end process;

-------------------------------------------------
--count audio tick for each line in SD system
-------------------------------------------------
--count_sample_per_line: process(reset_i,videoclk_i)
--begin
--   if reset_i = '0' then
--      sample_per_line <= 0;
--      sample_amount <= 0;  
--   elsif videoclk_i'event and videoclk_i = '1' then
--      if sync_reset_i = '1' then
--         sample_per_line <= 0;
--         sample_amount <= 0;
--      elsif enable_i = '1' then
--         if eav_tick = '1' and audio_tick_sd = '1' then  --both audio tick and eav tick
--            sample_per_line <= sample_amount;
--            sample_amount <= 1;     
--         elsif eav_tick = '1' then --and switch_point = '0' then  --don't reset in switch area
--            sample_per_line <= sample_amount;
--            sample_amount <= 0;
--         elsif audio_tick_sd = '1' then
--            sample_amount <= sample_amount + 1;
--         end if;
--      end if;
--   end if;
--end process;      
------------------------------------------------------------------
--define the audio sample distribution for each line in sd system
------------------------------------------------------------------
define_sample_per_line: process(reset_i,videoclk_i) 
begin                                              
   if reset_i = '0' then                           
      sample_per_line <= 0;    
      line_count <= 0;    
      frame_count <= 0;                                         
   elsif videoclk_i'event and videoclk_i = '1' then
      if sync_reset_i = '1' then                   
         sample_per_line <= 0;   
         line_count <= 0;      
         frame_count <= 0;                               
      elsif enable_i = '1' then                    
         if eav_tick = '1' and switch_point = '0' then
            if system_i = X"20" then   --PAL 625/50
               if line_i = "00000000001" then
                  line_count <= 0;
                  sample_per_line <= 4;
               elsif line_i = "00100101111"  then  --an extra 4 sample line
                  line_count <= line_count + 1; 
                  sample_per_line <= 4;
               elsif line_count = 9 then
                  line_count <= 0;
                  sample_per_line <= 4;
               else                
                  line_count <= line_count + 1; 
                  sample_per_line <= 3;
               end if;
            
            elsif system_i = X"60" then  --NTSC 525/59.94
               if line_i = "00000000100" then
                  sample_per_line <= 4;
                  line_count <= 0;
                  if frame_count = 4 then
                     frame_count <= 0;
                  else
                     frame_count <= frame_count + 1;
                  end if;
               elsif line_i = "00100001011" then
                  sample_per_line <= 4;   
                  line_count <= 0;        
               elsif line_i = "00100000111" and (frame_count = 0 or frame_count = 2 or frame_count = 4) then
                  sample_per_line <= 4;                --special line , 4 audio samples in frame 1,3,5
                  line_count <= 0;                     -- 3 audio samples in frame 2,4
               elsif line_i = "00100000111" and (frame_count = 1 or frame_count = 3) then   
                  sample_per_line <= 3;   
                  line_count <= 0;                              
               elsif line_count = 11 then   
                  line_count <= 0;         
                  sample_per_line <= 4;                            
               else 
                  sample_per_line <= 3;
                  line_count <= line_count + 1; 
               end if;
            end if;
         end if;
     end if;
  end if;
end process;
                                                 
--------------------------------------------------------
--choose the audio tick according to the current system           
--------------------------------------------------------
make_audio_sample_tick_register:process(videoclk_i, reset_i)
begin
   if reset_i = '0' then
     elsif videoclk_i'event and videoclk_i = '1' then
		if sync_reset_i = '1' then
		elsif enable_i = '1' then
         if use_f1485 = '1' then
            audiotick <= audio_tick_1485;
         else
            audiotick <= audio_tick_1484;
         end if;                            
      end if;
	end if;
end process;
           
with system_i select
   use_f1485 <=       '1' when X"A0",   -- 1080i/30
                      '0' when X"B0",   -- 1080i/29.97
                      '1' when X"82",   -- 1080p/30
                      '0' when X"92",   -- 1080p/29.97
                      '1' when X"83",   -- 1080p/25
                      '1' when X"84",   -- 1080p/24
                      '0' when X"94",   -- 1080p/23.98
                      '1' when X"A2",   -- 1080sF/30
                      '0' when X"B2",   -- 1080sF/29.97
                      '1' when X"A3",   -- 1080sF/25
                      '1' when X"A4",   -- 1080sF/24
                      '0' when X"B4",   -- 1080sF/23.98
                      '1' when X"86",   -- 720p/60
                      '0' when X"96",   -- 720p/59.94
                      '1' when X"87",   -- 720p/50
                      '1' when X"88",   -- 720p/30
                      '0' when X"98",   -- 720p/29.97
                      '1' when X"89",   -- 720p/25
                      '1' when X"8A",   -- 720p/24
                      '0' when X"9A",   -- 720p/23.98
                      '1' when X"60",   -- NTSC 525i/29.97
                      '1' when X"20",   -- PAL 625i/25
                      '1' when others;  -- 1080i/25   X"A1"                
 
---------------------------------------------------------------------------------------				 	
--define how many audio samples can be output for each line. 
--can be 2 or 3 according to the system lookup table, and 0 for a jump line in all systems. 
---------------------------------------------------------------------------------------			                    
define_max_pr_line: process(videoclk_i, reset_i)
begin
   if reset_i = '0' then 
      max_pr_line <= system_max_per_line;
	elsif videoclk_i'event and videoclk_i = '1' then	
		if sync_reset_i = '1' then		 
         max_pr_line <= system_max_per_line;
		elsif enable_i = '1' then	
		   if eav_tick = '1' then
		      if switch_point = '1' then
		         max_pr_line <= (others => '0');  --NB: max_pr_line can only be read at EAV_tick
		      else 
		         max_pr_line <= system_max_per_line;
		      end if;
		   end if;
	   end if;
	end if;
end process;   	  

with system_i select
   system_max_per_line <=  "00000011" when X"A0",   -- 1080i/30
                           "00000011" when X"B0",   -- 1080i/29.97
                           "00000011" when X"82",   -- 1080p/30
                           "00000011" when X"92",   -- 1080p/29.97
                           "00000111" when X"83",   -- 1080p/25
                           "00000111" when X"84",   -- 1080p/24
                           "00000111" when X"94",   -- 1080p/23.98
                           "00000011" when X"A2",   -- 1080sF/30
                           "00000011" when X"B2",   -- 1080sF/29.97
                           "00000111" when X"A3",   -- 1080sF/25
                           "00000111" when X"A4",   -- 1080sF/24
                           "00000111" when X"B4",   -- 1080sF/23.98
                           "00000011" when X"86",   -- 720p/60
                           "00000011" when X"96",   -- 720p/59.94
                           "00000011" when X"87",   -- 720p/50
                           "00000111" when X"88",   -- 720p/30
                           "00000111" when X"98",   -- 720p/29.97
                           "00001111" when X"89",   -- 720p/25
                           "00001111" when X"8A",   -- 720p/24
                           "00001111" when X"9A",   -- 720p/23.98
                           "00000011" when X"60",   -- NTSC 525i/29.97
                           "00000011" when X"20",   -- PAL 625i/25
                           "00000111" when others;  -- 1080i/25   X"A1"

                           
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
      luma_reg <= (others => '0');	
		audioluma_o <= (others => '0');
	   audiochroma_o <= (others =>'0');
   elsif videoclk_i'event and videoclk_i = '1' then
		if sync_reset_i = '1' then			
		   chroma_reg <= (others => '0');
		   luma_reg <= (others => '0');			
		   audioluma_o <= (others => '0');
	      audiochroma_o <= (others =>'0');
		elsif enable_i = '1' then  	
		   if signal_index_i  = X"03" then        --00=silence, 01=sine, 02=click, 03=off
			   audioluma_o   <= "0001000000";
			   audiochroma_o <= "1000000000";
	      elsif sd_hd = '0' and switch_point = '0' then   --sd signal
			   audioluma_o   <= sd_luma;   
			   audiochroma_o <= sd_chroma; 
			elsif sd_hd = '0' and switch_point = '1' then   --no signal in switch lines
			   audioluma_o   <= "0001000000";                              
			   audiochroma_o <= "1000000000";                              
			elsif control_point = '1' then  --output control packet and data packet
				audioluma_o <= audio_luma;
				audiochroma_o <= audio_chroma;
			else                                               --output data packet
				audioluma_o <= "0001000000";  --same as luma blank output
			   audiochroma_o <= audio_chroma;
			end if;
			chroma_reg <= audio_chroma;   --registered output data for CRC use
			luma_reg   <= audio_luma; 
		end if;
	end if;
end process;

-------------------------------------------------------------------
-- sample_counter: count from 0 to 191, to generate channel status
-------------------------------------------------------------------
sample_counting : process (videoclk_i, reset_i) begin
   if reset_i = '0' then
     sample_count <= (others => '0');
     sync_z       <= '0';
     control_b1   <= '0';   
   elsif videoclk_i'event and videoclk_i = '1' then
      if sync_reset_i = '1' then
         sample_count <= (others => '0');
         sync_z       <=  '0';
         control_b1   <=  '0';
      elsif enable_i = '1' and current_audiodata_tick = '1' then
         if sample_count = conv_std_logic_vector(191,8) then 
            sample_count <= (others => '0');
            sync_z     <= '0';
            control_b1 <= '0';   --checksum
         elsif sample_count = conv_std_logic_vector(189,8) then
            sample_count <= sample_count + 1;
            sync_z     <= '0';
            control_b1 <= '1';   --checksum
         elsif sample_count = conv_std_logic_vector(188,8) then
            sample_count <= sample_count + 1;
            sync_z     <= '0';
            control_b1 <= '1';   --checksum
         elsif sample_count = conv_std_logic_vector(185,8) then  --186 for stereo
            sample_count <= sample_count + 1;
            sync_z     <= '0';
            control_b1 <= '1';   --checksum
         elsif sample_count = conv_std_logic_vector(0,8) then  --first bit
            sample_count <= sample_count + 1;
            sync_z     <= '1';               
            control_b1 <= '1';  --1 = professional 0 = consumer
         else
            sample_count <= sample_count + 1;
            sync_z     <= '0';
            control_b1 <= '0';            
         end if;
      end if;
   end if;
end process;                

state_machine_register : process(videoclk_i, reset_i)
begin
   if reset_i = '0' then
		audio_data <= (others => '0');
		current_state <= S_IDLE;
		state_count <= 0;
		cs_chroma <= (others => '0');
		cs_luma <= (others => '0');
		audio_group <= 1;
		audio_chan <= 1;
		aes_count <= 0;
		current_audiodata_tick <= '0';		
		control_group <= 1;
		control_state_count <= 0;
		current_control_state<= S_IDLE;
		hd_dbn <= X"01";		
	elsif videoclk_i'event and videoclk_i = '1' then
		if sync_reset_i = '1' then
			audio_data <= (others => '0');
			current_state <= S_IDLE;
			state_count <= 0;
			cs_chroma <= (others => '0');
			cs_luma <= (others => '0');
			audio_group <= 1;
			audio_chan <= 1;
			aes_count <= 0;
		   current_audiodata_tick <= '0';
     		control_group <= 1;            
     		control_state_count <= 0;      
     		current_control_state<= S_IDLE;
		   hd_dbn <= X"01";
      elsif enable_i = '1' then  
		   current_state <= next_state; 
		   state_count <= next_state_count;    
		   audio_chan <= next_chan;
		   audio_group <= next_group;	
		   cs_chroma <= next_cs_chroma;	
		   cs_luma <= next_cs_luma;	
		   audio_data <= audiodata_i;
         current_audiodata_tick <= next_audiodata_tick;
         aes_count <= next_aes_count;
         control_group <= next_control_group;
         control_state_count <= next_control_state_count;
         current_control_state <= next_control_state;
         hd_dbn <= next_hd_dbn;
      end if;
   end if;
end process;     

output_audio_tick: process(sd_tick,current_audiodata_tick, sd_hd)
begin
if sd_hd = '1' then 
   audiodata_tick_o  <= current_audiodata_tick;
else
   audiodata_tick_o  <= sd_tick;
end if;
end process;


state_for_control: process(current_control_state,eav_tick, control_state_count,control_group, 
   cs_luma, audio_luma,luma_reg)
begin
next_cs_luma <= cs_luma + luma_reg(8 downto 0);
next_control_group <= control_group;
next_control_state_count <= control_state_count;

case current_control_state is
   when S_INEAV =>
      if control_state_count < 4 then
         next_control_state <= S_INEAV;
         next_control_state_count <= control_state_count + 1;
      else
         next_control_state_count <= 0;
         next_control_state <= S_ADF1;
      end if;
      audio_luma   <= "0001000000"; 
      next_cs_luma <= (others => '0');
      next_control_group <= 1;
   when S_ADF1 =>          
	 	  audio_luma <= (others => '0');
		  next_control_state <= S_ADF2;
		  next_cs_luma <= (others => '0');
	when S_ADF2 => 
	 	  audio_luma <= (others => '1');
		  next_control_state <= S_ADF3;
		  next_cs_luma <= (others => '0');
	when S_ADF3 => 
	 	  audio_luma <= (others => '1');
		  next_control_state <= S_DID;
		  next_cs_luma <= (others => '0');
	when S_DID=> 
        next_control_state <= S_DBN;
        next_cs_luma <= (others => '0');
		  if control_group = 1 then
		      audio_luma   <= "0111100011";   --1e3
		  elsif control_group = 2 then
		  		audio_luma   <= "1011100010";  --2e2 			  		
		  elsif control_group = 3 then
		  		audio_luma   <= "1011100001";  --2e1	  			
		  else 
		  		audio_luma   <= "0111100000";  --1e0
	     end if;	  
	when S_DBN => 
	 	  audio_luma   <= "1000000000";   --always 200h
		  next_control_state <= S_DC;
   when S_DC  =>
	 	  audio_luma   <= "0100001011";   --always 0x10B
		  next_control_state <= S_AF;
   when S_AF =>
   	  audio_luma <= "1000000000"; -- AF field: frame numbering not available;
		  next_control_state <= S_RATE;
   when S_RATE =>
   	  audio_luma <= "1000000000";   --Rate field: 48kHz synchronous audio
		  next_control_state <= S_ACT;
	when S_ACT =>	  
		  audio_luma <= "1000001111";  --ACT field, all channels active
		  next_control_state <= S_DEL;
	when S_DEL =>
	     if control_state_count = 0 or control_state_count = 3 then
	         audio_luma <= "1000000001";
	         next_control_state_count <= control_state_count + 1;	
	         next_control_state <= S_DEL;  
	     elsif control_state_count = 5 then
	      	audio_luma <= "1000000000"; 
	      	next_control_state_count <= 0;
	      	next_control_state <= S_RSRV;     
        else
            audio_luma <= "1000000000";
            next_control_state_count <= control_state_count + 1; 
            next_control_state <= S_DEL;    
        end if;
   when S_RSRV  =>
        if control_state_count = 0 then
            audio_luma <= "1000000000";
            next_control_state_count <= control_state_count + 1; 
            next_control_state <= S_RSRV;  
        else
            audio_luma <= "1000000000";
            next_control_state_count <= 0; 
            next_control_state <= S_CS;
        end if;
   when S_CS =>
        next_cs_luma(8 downto 0) <= (others => '0');     
        audio_luma(8 downto 0) <= cs_luma + luma_reg(8 downto 0); 
        audio_luma(9) <= not audio_luma(8);  
        if control_group = 4 then
            next_control_state <= S_IDLE;
            next_control_group <= 1;
        else
            next_control_state <= S_ADF1;
            next_control_group <= control_group + 1;
        end if;
   when others =>
   	   audio_luma <= "0001000000";    --same as luma horizontal blank value
         next_control_group <= 1;
         next_cs_luma <= (others => '0');
	 	   if eav_tick = '1' and sd_hd = '1' then
		  	   next_control_state <= S_INEAV;
		   else
		  	   next_control_state <= S_IDLE;
 		   end if; 
   end case;
end process;   
   
state_description: process(current_state, eav_tick, state_count, audio_group, audio_chan,audio_data,
   aes_count,cs_chroma, audio_chroma,chroma_reg,insert_now,previous_line,fifo_data_out,hd_dbn)
begin
next_chan <= audio_chan;
next_group <= audio_group;
next_aes_count <= aes_count;
next_state_count <= state_count;
ecc_stall <= '0';
ecc_clear <= '0';
next_audiodata_tick <= '0';
next_cs_chroma <= cs_chroma + chroma_reg(8 downto 0);
next_fifo_read <= '0';
next_hd_dbn <= hd_dbn;
audio_chroma <= "1000000000"; 
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
        next_group <= 1;
        next_cs_chroma <= (others => '0');
        ecc_clear <= '1';
	when S_ADF1 =>          
	 	  audio_chroma <= (others => '0');
		  next_state <= S_ADF2;
		  next_cs_chroma <= (others => '0');
	when S_ADF2 => 
	 	  audio_chroma <= (others => '1');
		  next_state <= S_ADF3;
		  next_cs_chroma <= (others => '0');
	when S_ADF3 => 
	 	  audio_chroma <= (others => '1');
		  next_state <= S_DID;
		  next_cs_chroma <= (others => '0');
	when S_DID=> 
        next_state <= S_DBN;
        next_cs_chroma <= (others => '0');
		  if audio_group = 1 then
		        audio_chroma <= "1011100111";	--2E7 = channel 1-4;  
		  elsif audio_group = 2 then
		  		audio_chroma <= "0111100110";  --1E6 = channel 5-8;			  		
		  elsif audio_group = 3 then
		  		audio_chroma <= "0111100101";  --1E5 = channel 9-12;		  			
		  else 
		  		audio_chroma <= "1011100100";  --2E4 = channel 13-16;
	      end if;
	when S_DBN => --disable continuity indication
	 	  audio_chroma(7 downto 0) <= hd_dbn; 	--DBN optional, either from 1 to 255 as active or "1000000000" as inactive
		  audio_chroma(9 downto 8) <= even_parity_bits(hd_dbn, 8, 0);
	 	  next_state <= S_DC;
    when S_DC  =>
	 	  audio_chroma <= "1000011000"; --always 0x218
		  next_state <= S_CLK1;
    when S_CLK1 =>
	 	  audio_chroma(7 downto 0) <= fifo_data_out(7 downto 0);
		  audio_chroma(9 downto 8) <= even_parity_bits(fifo_data_out(7 downto 0), 8, 0);
		  next_state <= S_CLK2;
    when S_CLK2 => 
	     audio_chroma(3 downto 0) <= fifo_data_out(11 downto 8);
	     audio_chroma(4) <= previous_line;
	     audio_chroma(5) <= fifo_data_out(12);
		  audio_chroma(7 downto 6) <= (others => '0');
		  audio_chroma(9 downto 8) <= even_parity_bits(previous_line&fifo_data_out(12 downto 8), 6, 0);		--8 5 8
		  next_state <= S_ACTIVEAUDIO;
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
			 elsif audio_chan = 3 then
				audio_chroma(3) <= sync_z;
        		audio_chroma(9 downto 8) <= even_parity_bits( sync_z & audio_data(3 downto 0), 5, 0);
          elsif audio_chan = 2 then
        		audio_chroma(3) <= '0';
        		audio_chroma(9 downto 8) <= even_parity_bits( audio_data(3 downto 0), 4, 0);
        	 else
        		audio_chroma(3) <= '0';
        		audio_chroma(9 downto 8) <= even_parity_bits( audio_data(3 downto 0), 4, 0);
        	 end if;

      elsif state_count = 1 then
        	audio_chroma(7 downto 0) <= audio_data(11 downto 4);
        	audio_chroma(9 downto 8) <= even_parity_bits(audio_data(11 downto 4), 8, 4);
      elsif state_count = 2 then                                                                      
        	audio_chroma(7 downto 0) <= audio_data(19 downto 12);                         
        	audio_chroma(9 downto 8) <= even_parity_bits(audio_data(19 downto 12), 8, 12);
      else
        	audio_chroma(3 downto 0) <= audio_data(23 downto 20);
        	audio_chroma(5 downto 4) <= (others => '0'); --U: user bit(not used), V: validity bit 0=valid
        	audio_chroma(6) <= control_b1;      --control status bit 
        	audio_chroma(7) <= even_parity_bits(control_b1 & audio_data, 25, 0)(0);	       
        	audio_chroma(9 downto 8) <= even_parity_bits(audio_chroma(7 downto 0), 8, 0);
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
		  if insert_now(0) = '0' and audio_group = 4 then  --finish with output for this line
		  		next_state <= S_IDLE;
		  		next_group <= 1;
            next_audiodata_tick <= '1';
            if aes_count = 191 then
               next_aes_count <= 0;
            else
               next_aes_count <= aes_count + 1;
            end if;
            if hd_dbn = X"FF" then
               next_hd_dbn <= X"01";
            else
               next_hd_dbn <= hd_dbn + '1';
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
            if hd_dbn = X"FF" then
               next_hd_dbn <= X"01";
            else
               next_hd_dbn <= hd_dbn + '1';
            end if;
		  else								-- more channel group in the current dataset
		  		next_state <= S_ADF1;
		  		next_group <= audio_group + 1;    
		  end if;
		  ecc_clear <= '1';
    when others =>							--S_IDLE
	      audio_chroma <= "1000000000";  --same as chroma horizontal blank value
         next_group <= 1;
		   next_chan <= 1;
		   next_cs_chroma <= (others=> '0');
		   ecc_clear <= '1';
	 	   if eav_tick = '1' and sd_hd = '1' then
		  	   next_state <= S_INEAV;
		   else
		  	   next_state <= S_IDLE;
 		   end if; 
    end case;
end process;



SD_state: process(current_sd_state,sd_chan,sd_sample,sd_statecount,sd_chroma_reg,sd_luma_reg,sd_cs,sd_audio_data,
  sd_control_b1,sd_sync_z,eav_tick_previous, sd_wordnr, buffer_index, sd_sel,sd_dbn)
begin

next_sd_chan  <= sd_chan;
next_sd_sample <= sd_sample;
next_sd_tick <= '0';
next_sd_statecount <= sd_statecount;
next_sd_wordnr <= sd_wordnr;
next_sd_group <= sd_group;
next_buffer_index <= buffer_index;
write_buffer <= '0';
next_sd_sel <= sd_sel;
next_sd_dbn <= sd_dbn;
case current_sd_state is
   when S_ADF1 =>
      if switch_point = '0' then
         next_sd_state  <= S_ADF2;
      else
         next_sd_state <= S_IDLE;
      end if;
      sd_chroma      <= (others => '0');
      sd_luma        <= (others => '1');  
      next_sd_cs <= (others => '0');
      sd_data_en <= '1';
      
   when S_ADF2 =>
      next_sd_state  <= S_DBN;
      sd_chroma      <= (others => '1');
      if sd_group = 1 then      
         sd_luma   <= "1011111111";        
      elsif sd_group = 2 then         
         sd_luma   <= "0111111101";        
      elsif sd_group = 3 then         
         sd_luma   <= "0111111011";        
      elsif sd_group = 4 then   
         sd_luma   <= "1011111001";   
      end if;          
      next_sd_cs <= (others => '0');
      sd_data_en <= '1';
   when S_DBN =>
      next_sd_state  <= S_ACTIVEAUDIO; 

      sd_chroma(9 downto 8) <= even_parity_bits(sd_dbn,8,0);
      sd_chroma(7 downto 0) <=  sd_dbn;    --DBN optional, either from 1 to 255 as active or "1000000000" as inactive
      
      if sample_per_line =  3 then
         sd_luma        <= "1000100100";       --DC: 3 samples per line
      else
         sd_luma        <= "1000110000";       --DC: 4 samples per line
      end if;
      
      if sd_group = 4 then
         if sd_dbn = X"ff" then
            next_sd_dbn <= X"01";
         else
            next_sd_dbn <= sd_dbn + '1';
         end if;
      else  
         next_sd_dbn <= sd_dbn;
      end if;
      
      next_sd_cs <= sd_cs + sd_luma_reg(8 downto 0);
      next_sd_wordnr <= 0;
      next_sd_statecount <= 0;
      next_sd_chan <= 0;
      sd_data_en <= '1';
   when S_ACTIVEAUDIO =>
      if sd_wordnr = 0 then      --sample 1
         sd_chroma(9) <= not sd_audio_data(5);         --X                      
         sd_chroma(8 downto 3) <= sd_audio_data(5 downto 0);        
         sd_chroma(2 downto 1) <= CONV_STD_LOGIC_VECTOR(sd_chan,2);
         sd_chroma(0) <= sd_sync_z;                                 

         sd_luma(9) <= not sd_audio_data(14);          --X+1   
         sd_luma(8 downto 0) <= sd_audio_data(14 downto 6);

         next_sd_wordnr <= 1;
         next_sd_chan <= sd_chan + 1;
         sd_data_en <= '0';        
         
         if sd_chan = 0 and sd_group = 1 then       
            next_sd_tick <= '1';               --give a tick in advance for the next sample
            write_buffer <= '1';               --save current data into buffer
            next_buffer_index <= buffer_index + 1;   
         elsif sd_chan = 0 then
            next_buffer_index <= buffer_index + 1;        
         end if;  
      elsif sd_wordnr = 1 then                    
         sd_chroma(9 downto 8) <= even_parity_bits( sd_sync_z & CONV_STD_LOGIC_VECTOR(sd_chan-1,2) & 
           sd_control_b1 & sd_audio_data, 24, 0); --X+2                              
         sd_chroma(7) <= sd_control_b1;                               
         sd_chroma(6 downto 5) <= "00"; --valid data, no user data 
         sd_chroma(4 downto 0) <= sd_audio_data(19 downto 15);        
         
         sd_luma(9) <= not sd_audio_data(5);                --X      
         sd_luma(8 downto 3) <= sd_audio_data(5 downto 0);        
         sd_luma(2 downto 1) <= CONV_STD_LOGIC_VECTOR(sd_chan,2);
         sd_luma(0) <= sd_sync_z;                                                
         
         next_sd_wordnr <= 2;
         sd_data_en <= '0';
         if sd_chan = 3 and sd_group = 1 then
            if sd_sample = 191 then            
               next_sd_sample <= 0;            
            else                               
               next_sd_sample <= sd_sample + 1;
            end if;                              
         end if; 
              
      else
         sd_chroma(9) <= not sd_audio_data(14);            --X+1  
         sd_chroma(8 downto 0) <= sd_audio_data(14 downto 6);
                            
         sd_luma(9 downto 8) <= even_parity_bits( sd_sync_z & CONV_STD_LOGIC_VECTOR(sd_chan,2) & 
           sd_control_b1 & sd_audio_data, 24, 0);       --X+2                              
         sd_luma(7) <= sd_control_b1;                               
         sd_luma(6 downto 5) <= "00"; --valid data, no user data 
         sd_luma(4 downto 0) <= sd_audio_data(19 downto 15);        
                                      
         next_sd_wordnr <= 0;
         
         if sd_chan = 3 then
            next_sd_chan <= 0;                 -- current sample done
            sd_data_en <= '1';
         else
            next_sd_chan <= sd_chan + 1;  --if overflow, reset to zero
            sd_data_en <= '0';
         end if;
      end if;    
  
      if sd_statecount < sample_per_line * 6 - 1  then
         next_sd_statecount <= sd_statecount + 1;
         next_sd_state  <= S_ACTIVEAUDIO;
      else      
         next_sd_statecount <= 0; 
         next_sd_state  <= S_CS;      
      end if;
      next_sd_cs <= sd_cs + sd_chroma_reg(8 downto 0) + sd_luma_reg(8 downto 0);
   when S_CS =>
      if sd_group = 4 then
			next_sd_state <= S_IDLE;
			next_sd_group <= 1;
			next_sd_sel <= '0';
		else
			next_sd_state <= S_ADF1;
			next_sd_group <= sd_group + 1; 
			next_sd_sel <= '1';   
	   end if;
	   next_buffer_index <= 0;
	   sd_data_en <= '1';
	   sd_chroma(8 downto 0) <= sd_cs + sd_chroma_reg(8 downto 0)+ sd_luma_reg(8 downto 0); 
	   sd_chroma(9) <= not sd_chroma(8);
	   sd_luma   <= "0001000000";
	   next_sd_cs <= (others=>'0');
   when others =>
      sd_chroma      <= "1000000000";
      sd_luma        <= "0001000000";
      next_sd_group  <= 1;
      next_buffer_index <= 0;
      next_sd_sel <= '0';
      sd_data_en <= '1';
	   if eav_tick_previous = '1' and sd_hd = '0' then
     	   next_sd_state <= S_ADF1;                   
      else                                        
     	   next_sd_state <= S_IDLE;                 
      end if;    
      next_sd_cs <= (others=>'0');
   end case;                                   
end process;

SD_register: process(videoclk_i, reset_i)
begin
if reset_i = '0' then
   sd_chan        <= 0;
   sd_sample      <= 0;
   sd_statecount  <= 0;
   sd_cs          <= (others=>'0');
   current_sd_state <= S_IDLE;
   sd_chroma_reg  <= (others=>'0'); 
   sd_luma_reg    <= (others=>'0');
   sd_tick        <= '0'; 
   sd_wordnr      <= 0;
   sd_group       <= 1;
   sd_dbn         <= X"01";  
elsif videoclk_i'event and videoclk_i = '1' then
   if sync_reset_i = '1' then
      sd_chan        <= 0;
      sd_sample      <= 0;
      sd_statecount  <= 0;
      sd_cs          <= (others=>'0');
      current_sd_state <= S_IDLE;  
      sd_chroma_reg  <= (others=>'0');
      sd_luma_reg    <= (others=>'0'); 
      sd_tick        <= '0';
      sd_wordnr      <= 0;
      sd_group       <= 1;   
      sd_dbn         <= X"01";   
   elsif enable_i = '1' then  
      sd_group       <=  next_sd_group;
      sd_chan        <=  next_sd_chan;
      sd_sample      <=  next_sd_sample;
      sd_statecount  <=  next_sd_statecount;
      sd_cs          <=  next_sd_cs;
      current_sd_state <= next_sd_state;
      sd_wordnr      <= next_sd_wordnr;
      sd_chroma_reg  <= sd_chroma;
      sd_luma_reg    <= sd_luma;
      sd_tick        <= next_sd_tick;
      sd_dbn         <= next_sd_dbn;
   end if;
end if;
end process;


memory_buffer_management:process(videoclk_i, reset_i)
begin
   if reset_i = '0' then
      audio_buffer   <= (others=>"00000000000000000000000000");
      buffer_index   <= 0;
      sd_audio_data  <= (others => '0');
      sd_control_b1  <= '0';
      sd_sync_z      <= '0';     
   elsif videoclk_i'event and videoclk_i = '1' then
      if sync_reset_i = '1' then
         audio_buffer   <= (others=>"00000000000000000000000000");
         buffer_index   <= 0;
         sd_audio_data  <= (others => '0');
         sd_control_b1  <= '0';
         sd_sync_z      <= '0';
      elsif enable_i = '1' then 
         if sd_data_en = '1' then    -- a tick for each sample
            if sd_sel = '0' then     -- read data from memory(sd_sel=0) or buffer(sd_sel=1)
               sd_audio_data <= audiodata_i(23 downto 4);
               sd_control_b1 <=  sd_control_b1_direct; 
               sd_sync_z     <=  sd_sync_z_direct;     
            else
               sd_audio_data <= audio_buffer(buffer_index)(23 downto 4);
               sd_control_b1 <= audio_buffer(buffer_index)(24);
               sd_sync_z     <= audio_buffer(buffer_index)(25);
            end if;
         end if;
         
         if write_buffer = '1' then  --write data into buffer
            audio_buffer(buffer_index)(23 downto 0) <= audiodata_i;
            audio_buffer(buffer_index)(24) <= sd_control_b1_direct; 
            audio_buffer(buffer_index)(25) <= sd_sync_z_direct;     
         end if;
         
         buffer_index   <= next_buffer_index;
         sd_sel <= next_sd_sel;
      end if;
   end if;
end process;




with sd_sample select     
	sd_control_b1_direct <= '1' when 0,--professional
				  '1' when 185,   --checksum
				  '1' when 188,   --checksum
				  '1' when 189,   --checksum
	           '0' when others;   
    	 	                   
with sd_sample select     
	sd_sync_z_direct <= '1' when  0,   
			  '0' when others;
			  
debug(1) <= enable_i;			  
debug(0) <= audio_tick_sd;		  	  	           	 	

end Behavioral;