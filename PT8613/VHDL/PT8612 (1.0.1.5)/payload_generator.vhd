use work.pack_ancillary_data.all;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity payload_generator is
   port (
      clk_i                : in  std_logic;
      en_i                 : in  std_logic;
      system_i             : in  std_logic_vector(11 downto 0);
      line_i               : in  std_logic_vector(10 downto 0);
      anc_space_i          : in  std_logic;
      data_a_i             : in  std_logic_vector(9 downto 0);
      data_b_i             : in  std_logic_vector(9 downto 0);
      anc_space_o          : out std_logic;
      data_a_o             : out std_logic_vector(9 downto 0);
      data_b_o             : out std_logic_vector(9 downto 0)
   );
end payload_generator;


---------------------------------------------------------------
-- For payload definitions see SMPTE 352M-2002
---------------------------------------------------------------

architecture Behavioral of payload_generator is


type statetype is (S_ADF1, S_ADF2, S_ADF3, S_DID, S_SDID, S_DC, S_BYTE1, S_BYTE2, S_BYTE3, S_BYTE4, S_CS, S_IDLE);
signal state                     : statetype;

signal lpf_index                 : std_logic_vector(2 downto 0);
signal hd_zsd                    : std_logic;
signal single_zdual              : std_logic;
signal interlaced                : std_logic;
signal insert_now                : std_logic;
signal progressive_transport     : std_logic;
signal progressive_picture       : std_logic;
signal lpf_single                : std_logic_vector(3 downto 0);
signal lpf_and_interlaced_index  : std_logic_vector(3 downto 0);
signal lpf_progressive_transport : std_logic_vector(3 downto 0);
signal picture_rate              : std_logic_vector(3 downto 0);
signal sampling_structure        : std_logic_vector(3 downto 0);
signal aspect_ratio              : std_logic;
signal channel_assignment_a      : std_logic_vector(1 downto 0);
signal channel_assignment_b      : std_logic_vector(1 downto 0);
signal bit_depth                 : std_logic_vector(1 downto 0);

signal byte1                     : std_logic_vector(7 downto 0);
signal byte2                     : std_logic_vector(7 downto 0);
signal byte3                     : std_logic_vector(7 downto 0);
signal byte4a                    : std_logic_vector(7 downto 0);
signal byte4b                    : std_logic_vector(7 downto 0);

signal data                      : std_logic_vector(9 downto 0);
signal data_a                    : std_logic_vector(9 downto 0);
signal data_b                    : std_logic_vector(9 downto 0);

signal cs_reg_a                  : std_logic_vector(8 downto 0);
signal cs_reg_b                  : std_logic_vector(8 downto 0);

signal insert_line1              : std_logic_vector(10 downto 0);
signal insert_line2              : std_logic_vector(10 downto 0);


begin


word_sm : process(clk_i)
begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         case state is
         when S_ADF1    =>                          
            if anc_space_i = '1' then
               state <= S_ADF2;
            end if;
         when S_ADF2    =>                          
            state <= S_ADF3;
         when S_ADF3    =>                          
            state <= S_DID;
         when S_DID     =>                          
            state <= S_SDID;
         when S_SDID    =>                          
            state <= S_DC;
         when S_DC      =>                          
            state <= S_BYTE1;
         when S_BYTE1   =>                          
            state <= S_BYTE2;
         when S_BYTE2   =>                          
            state <= S_BYTE3;
         when S_BYTE3   =>                          
            state <= S_BYTE4;
         when S_BYTE4   =>                          
            state <= S_CS;
         when S_CS      =>                          
            state <= S_IDLE;
         when others    => -- S_IDLE
            if anc_space_i = '0' then
               state <= S_ADF1;                            
            end if;
         end case;
      end if;
   end if;
end process;


state_to_output : process(state, byte1, byte2, byte3, byte4a, byte4b, data, insert_now, anc_space_i, cs_reg_a, cs_reg_b, data_a_i, data_b_i)
begin
   if anc_space_i = '1' and insert_now = '1' then
      case state is
         when S_ADF1   =>                          
            data        <= (others => '0');
            data_a      <= data;
            data_b      <= data;
            anc_space_o <= '0';
         when S_ADF2   =>
            data        <= (others => '1');
            data_a      <= data;
            data_b      <= data;
            anc_space_o <= '0';
         when S_ADF3   =>                          
            data        <= (others => '1');
            data_a      <= data;
            data_b      <= data;
            anc_space_o <= '0';
         when S_DID    =>                          
            data        <= byte_to_10_bits(X"41");
            data_a      <= data;
            data_b      <= data;
            anc_space_o <= '0';
         when S_SDID   =>                          
            data        <= byte_to_10_bits(X"01");
            data_a      <= data;
            data_b      <= data;
            anc_space_o <= '0';
         when S_DC     =>                          
            data        <= byte_to_10_bits(X"04");
            data_a      <= data;
            data_b      <= data;
            anc_space_o <= '0';
         when S_BYTE1  =>                          
            data        <= byte_to_10_bits(byte1);
            data_a      <= data;
            data_b      <= data;
            anc_space_o <= '0';
         when S_BYTE2  =>                     
            data        <= byte_to_10_bits(byte2);
            data_a      <= data;
            data_b      <= data;
            anc_space_o <= '0';
         when S_BYTE3  =>                     
            data        <= byte_to_10_bits(byte3);
            data_a      <= data;
            data_b      <= data;
            anc_space_o <= '0';
         when S_BYTE4  =>                     
            data        <= (others => 'X');
            data_a      <= byte_to_10_bits(byte4a);
            data_b      <= byte_to_10_bits(byte4b);
            anc_space_o <= '0';
         when S_CS  =>                     
            data                 <= (others => 'X');
            data_a(8 downto 0)   <= cs_reg_a;
            data_b(8 downto 0)   <= cs_reg_b;
            data_a(9)            <= not cs_reg_a(8);
            data_b(9)            <= not cs_reg_b(8);
            anc_space_o <= '0';
         when others => -- S_IDLE
            data        <= (others => 'X');
            data_a      <= data_a_i;
            data_b      <= data_b_i;
            anc_space_o <= '1';
      end case;
   else
      data        <= (others => 'X');
      data_a      <= data_a_i;
      data_b      <= data_b_i;
      anc_space_o <= anc_space_i;
   end if;
end process;


data_a_o <= data_a;
data_b_o <= data_b;


check_sum_generation : process(clk_i)
begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         case state is
         when S_ADF3 =>
            cs_reg_a   <= (others => '0');
            cs_reg_b   <= (others => '0');
         when others =>
            cs_reg_a   <= cs_reg_a + data_a(8 downto 0);
            cs_reg_b   <= cs_reg_b + data_b(8 downto 0);
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Lines where packet is inserted :
---------------------------------------------------------------
lpf_progressive_transport <= lpf_index & progressive_transport;

with lpf_index select
   insert_line1            <= (others => '1')                     when "000",    -- 525  
                              (others => '1')                     when "001",    -- 625  
                              conv_std_logic_vector(10,11)        when "010",    -- 720  
                              conv_std_logic_vector(10,11)        when others;   -- 1080 

with lpf_progressive_transport select
   insert_line2            <= conv_std_logic_vector(572,11)       when "1000",   -- 1080 interlaced
                              (others => '1')                     when others;
                              

insert_now_generation : process(insert_line1, insert_line2, line_i)
begin
   if line_i = insert_line1 or line_i = insert_line2 then
      insert_now <= '1';
   else
      insert_now <= '0';
   end if;
end process;
                              
                              
                              
---------------------------------------------------------------
-- Byte1 :
---------------------------------------------------------------


lpf_single <= lpf_index & single_zdual;                           

with lpf_single select
   byte1                   <= X"81" when "0000",   -- 525  single
                              X"81" when "0010",   -- 625  single
                              X"84" when "0100",   -- 720  single
                              X"81" when "0001",   -- 525  dual   (nonexistent)
                              X"81" when "0011",   -- 625  dual   (nonexistent)
                              X"84" when "0101",   -- 720  dual   (nonexistent)
                              X"87" when "1001",   -- 1080 dual
                              X"85" when others;   -- 1080 single


---------------------------------------------------------------
-- single / dual link :
---------------------------------------------------------------
with system_i(3 downto 0) select
   single_zdual            <= '0' when X"1",   -- YCbCrA 4:2:2:4  12-bit (dual-link)                             
                              '0' when X"2",   -- YCbCr  4:4:4    10-bit (dual-link)                             
                              '0' when X"3",   -- YCbCrA 4:4:4:4  12-bit (dual-link)                             
                              '0' when X"4",   -- RGB    4:4:4    10-bit (dual-link)                             
                              '0' when X"5",   -- RGBA   4:4:4:4  12-bit (dual-link)                             
                              '1' when others; -- YCbCr  4:2:2    10-bit (single-link)                           


---------------------------------------------------------------
-- lines per frame dependent on system :
-- 487: "000", 576: "001", 720: "010", 1080: "100"
---------------------------------------------------------------
with system_i(11 downto 4) select
   lpf_index <=      "100" when X"A0",   -- 1080i/30
                     "100" when X"B0",   -- 1080i/29.97
                     "100" when X"82",   -- 1080p/30
                     "100" when X"92",   -- 1080p/29.97
                     "100" when X"83",   -- 1080p/25
                     "100" when X"84",   -- 1080p/24
                     "100" when X"94",   -- 1080p/23.98
                     "100" when X"A2",   -- 1080sF/30
                     "100" when X"B2",   -- 1080sF/29.97
                     "100" when X"A3",   -- 1080sF/25
                     "100" when X"A4",   -- 1080sF/24
                     "100" when X"B4",   -- 1080sF/23.98
                     "010" when X"86",   -- 720p/60
                     "010" when X"96",   -- 720p/59.94
                     "010" when X"87",   -- 720p/50
                     "010" when X"88",   -- 720p/30
                     "010" when X"98",   -- 720p/29.97
                     "010" when X"89",   -- 720p/25
                     "010" when X"8A",   -- 720p/24
                     "010" when X"9A",   -- 720p/23.98
                     "000" when X"60",   -- NTSC 525i/29.97
                     "001" when X"20",   -- PAL 625i/25
                     "100" when others;  -- 1080i/25  X"A1"

                           
---------------------------------------------------------------
-- Byte2 :
---------------------------------------------------------------


byte2 <= progressive_transport & progressive_picture & "00" & picture_rate; 


with system_i(11 downto 4) select
   progressive_transport   <= '0' when X"A0",   -- 1080i/30
                              '0' when X"B0",   -- 1080i/29.97
                              '1' when X"82",   -- 1080p/30
                              '1' when X"92",   -- 1080p/29.97
                              '1' when X"83",   -- 1080p/25
                              '1' when X"84",   -- 1080p/24
                              '1' when X"94",   -- 1080p/23.98
                              '0' when X"A2",   -- 1080sF/30
                              '0' when X"B2",   -- 1080sF/29.97
                              '0' when X"A3",   -- 1080sF/25
                              '0' when X"A4",   -- 1080sF/24
                              '0' when X"B4",   -- 1080sF/23.98
                              '1' when X"86",   -- 720p/60
                              '1' when X"96",   -- 720p/59.94
                              '1' when X"87",   -- 720p/50
                              '1' when X"88",   -- 720p/30
                              '1' when X"98",   -- 720p/29.97
                              '1' when X"89",   -- 720p/25
                              '1' when X"8A",   -- 720p/24
                              '1' when X"9A",   -- 720p/23.98
                              '0' when X"60",   -- NTSC 525i/29.97
                              '0' when X"20",   -- PAL 625i/25
                              '0' when others;  -- 1080i/25

with system_i(11 downto 4) select
   progressive_picture     <= '0' when X"A0",   -- 1080i/30       
                              '0' when X"B0",   -- 1080i/29.97    
                              '1' when X"82",   -- 1080p/30       
                              '1' when X"92",   -- 1080p/29.97    
                              '1' when X"83",   -- 1080p/25       
                              '1' when X"84",   -- 1080p/24       
                              '1' when X"94",   -- 1080p/23.98    
                              '1' when X"A2",   -- 1080sF/30      
                              '1' when X"B2",   -- 1080sF/29.97   
                              '1' when X"A3",   -- 1080sF/25      
                              '1' when X"A4",   -- 1080sF/24      
                              '1' when X"B4",   -- 1080sF/23.98   
                              '1' when X"86",   -- 720p/60        
                              '1' when X"96",   -- 720p/59.94     
                              '1' when X"87",   -- 720p/50        
                              '1' when X"88",   -- 720p/30        
                              '1' when X"98",   -- 720p/29.97     
                              '1' when X"89",   -- 720p/25        
                              '1' when X"8A",   -- 720p/24        
                              '1' when X"9A",   -- 720p/23.98     
                              '0' when X"60",   -- NTSC 525i/29.97
                              '0' when X"20",   -- PAL 625i/25    
                              '0' when others;  -- 1080i/25       
                              
                                                  
with system_i(11 downto 4) select
   picture_rate            <= X"7" when X"A0",   -- 1080i/30
                              X"6" when X"B0",   -- 1080i/29.97
                              X"7" when X"82",   -- 1080p/30
                              X"6" when X"92",   -- 1080p/29.97
                              X"5" when X"83",   -- 1080p/25
                              X"3" when X"84",   -- 1080p/24
                              X"2" when X"94",   -- 1080p/23.98
                              X"7" when X"A2",   -- 1080sF/30      
                              X"6" when X"B2",   -- 1080sF/29.97   
                              X"5" when X"A3",   -- 1080sF/25      
                              X"3" when X"A4",   -- 1080sF/24      
                              X"2" when X"B4",   -- 1080sF/23.98   
                              X"B" when X"86",   -- 720p/60
                              X"A" when X"96",   -- 720p/59.94
                              X"9" when X"87",   -- 720p/50
                              X"7" when X"88",   -- 720p/30
                              X"6" when X"98",   -- 720p/29.97
                              X"5" when X"89",   -- 720p/25
                              X"3" when X"8A",   -- 720p/24
                              X"2" when X"9A",   -- 720p/23.98
                              X"6" when X"60",   -- NTSC 525i/29.97
                              X"5" when X"20",   -- PAL 625i/25
                              X"5" when others;  -- 1080i/25


---------------------------------------------------------------
-- Byte3 :
---------------------------------------------------------------

byte3 <= aspect_ratio & "000" & sampling_structure;

                     
-- aspect_ratio = '0' : 4:3, aspect_ratio = '1' : 16:9 (in HD systems : reserved, set to '0')
with system_i(11 downto 4) select
   aspect_ratio            <= '1' when X"60",   -- NTSC 525i/59.94
                              '1' when X"20",   -- PAL 625i/50
                              '0' when others;  -- All HD systems

                     
---------------------------------------------------------------
-- sampling_structure :
---------------------------------------------------------------
with system_i(3 downto 0) select
   sampling_structure   <= X"4" when X"1",   -- YCbCrA 4:2:2:4  12-bit (dual-link)                             
                           X"1" when X"2",   -- YCbCr  4:4:4    10-bit (dual-link)                             
                           X"5" when X"3",   -- YCbCrA 4:4:4:4  12-bit (dual-link)                             
                           X"2" when X"4",   -- RGB    4:4:4    10-bit (dual-link)                             
                           X"6" when X"5",   -- RGBA   4:4:4:4  12-bit (dual-link)                             
                           X"0" when others; -- YCbCr  4:2:2    10-bit (single-link)                           
                                                                                                            

---------------------------------------------------------------
-- Byte4 :
---------------------------------------------------------------
byte4a <= channel_assignment_a & "0000" & bit_depth; 
byte4b <= channel_assignment_b & "0000" & bit_depth;                            
                           
with system_i(3 downto 0) select
   bit_depth            <= "10" when X"1",   -- YCbCrA 4:2:2:4  12-bit (dual-link)                             
                           "01" when X"2",   -- YCbCr  4:4:4    10-bit (dual-link)                             
                           "10" when X"3",   -- YCbCrA 4:4:4:4  12-bit (dual-link)                             
                           "01" when X"4",   -- RGB    4:4:4    10-bit (dual-link)                             
                           "10" when X"5",   -- RGBA   4:4:4:4  12-bit (dual-link)                             
                           "01" when others; -- YCbCr  4:2:2    10-bit (single-link)                           

                           
---------------------------------------------------------------
-- channel assignment :
---------------------------------------------------------------
channel_assignment_a <= "00";   -- single link or ch1 of multi link

with system_i(3 downto 0) select
   channel_assignment_b <= "01" when X"1",   -- YCbCrA 4:2:2:4  12-bit (dual-link)                             
                           "01" when X"2",   -- YCbCr  4:4:4    10-bit (dual-link)                             
                           "01" when X"3",   -- YCbCrA 4:4:4:4  12-bit (dual-link)                             
                           "01" when X"4",   -- RGB    4:4:4    10-bit (dual-link)                             
                           "01" when X"5",   -- RGBA   4:4:4:4  12-bit (dual-link)                             
                           "00" when others; -- YCbCr  4:2:2    10-bit (single-link)                           

end Behavioral;