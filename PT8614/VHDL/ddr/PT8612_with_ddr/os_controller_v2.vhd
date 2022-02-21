--use work.pack_oversample_by_11.all;
library IEEE; 
use IEEE.STD_LOGIC_1164.ALL; 
use IEEE.STD_LOGIC_ARITH.ALL; 
use IEEE.STD_LOGIC_UNSIGNED.ALL;  

entity os_controller is
   port(
       clk_i         :  in  std_logic;
       sync_reset_i  :  in  std_logic;
       hd_zsd_i      :  in  std_logic;
       data_a_i      :  in  std_logic_vector(19 downto 0);
       data_b_i      :  in  std_logic_vector(19 downto 0);
       data_a_o      :  out std_logic_vector(19 downto 0);
       data_b_o      :  out std_logic_vector(19 downto 0);
       enable_o      :  out std_logic
   );
end os_controller;


architecture behavioral of os_controller is

type state is (s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10);
signal current_state    : state := s0;
signal next_state       : state;
signal enable           : std_logic;


attribute uselowskewlines : string;
attribute uselowskewlines of enable_o  : signal is "yes";


begin

STATE_REG : process(clk_i)
begin
if clk_i'event and clk_i = '1' then
   if sync_reset_i = '1' then
      current_state <= s0;
   else
      current_state <= next_state;  
   end if;
end if;
end process;


STATEMACHINE_OUTPUT_A: process(hd_zsd_i , current_state, data_a_i )
begin
   if hd_zsd_i  = '0' then   -- for sd signal
      case current_state is
      when s0 =>               
         next_state <= s1;     
         enable <= '0';    
         data_a_o(19 downto 9)  <= (others => data_a_i (0));
         data_a_o(8 downto 0)   <= (others => data_a_i (1)); 
      when s1 =>               
         next_state <= s2;     
         enable <= '0';    
         data_a_o(19 downto 18) <= (others => data_a_i (1));
         data_a_o(17 downto 7)  <= (others => data_a_i (2)); 
         data_a_o(6 downto 0)   <= (others => data_a_i (3));
      when s2 =>               
         next_state <= s3;     
         enable <= '0';    
         data_a_o(19 downto 16) <= (others => data_a_i (3)); 
         data_a_o(15 downto 5)  <= (others => data_a_i (4));
         data_a_o(4 downto 0)   <= (others => data_a_i (5));
      when s3 =>               
         next_state <= s4;     
         enable <= '0';    
         data_a_o(19 downto 14) <= (others => data_a_i (5)); 
         data_a_o(13 downto 3)  <= (others => data_a_i (6));
         data_a_o(2 downto 0)   <= (others => data_a_i (7));
      when s4 =>               
         next_state <= s5;           
         enable <= '0';    
         data_a_o(19 downto 12) <= (others => data_a_i (7)); 
         data_a_o(11 downto 1)  <= (others => data_a_i (8));
         data_a_o(0)            <= data_a_i (9);
      when s5 =>               
         next_state <= s6;     
         enable <= '0';    
         data_a_o(19 downto 10) <= (others => data_a_i (9)); 
         data_a_o( 9 downto  0) <= (others => data_a_i (10));      
      when s6 =>               
         next_state <= s7;     
         enable <= '0';    
         data_a_o(19)           <= data_a_i (10); 
         data_a_o(18 downto 8)  <= (others => data_a_i (11));
         data_a_o(7 downto 0)   <= (others => data_a_i (12));    
      when s7 =>               
         next_state <= s8;     
         enable <= '0';    
         data_a_o(19 downto 17) <= (others => data_a_i (12)); 
         data_a_o(16 downto 6)  <= (others => data_a_i (13));
         data_a_o(5 downto 0)   <= (others => data_a_i (14));
      when s8 =>               
         next_state <= s9;     
         enable <= '0';    
         data_a_o(19 downto 15) <= (others => data_a_i (14)); 
         data_a_o(14 downto 4)  <= (others => data_a_i (15));
         data_a_o(3 downto 0)   <= (others => data_a_i (16));
      when s9 =>               
         next_state <= s10;    
         enable <= '1';    
         data_a_o(19 downto 13) <= (others => data_a_i (16)); 
         data_a_o(12 downto 2)  <= (others => data_a_i (17));
         data_a_o(1 downto 0)   <= (others => data_a_i (18));
      when s10 =>              
         next_state <= s0;     
         enable <= '0';    
         data_a_o(19 downto 11) <= (others => data_a_i (18));
         data_a_o(10 downto 0)  <= (others => data_a_i (19));
      end case;
   else            --for hd signal
      case current_state is  
      when s0 =>
         data_a_o <= data_a_i (0) & data_a_i (0) & data_a_i (1) & data_a_i (1) & data_a_i (2) & data_a_i (2) & data_a_i (3) & data_a_i (3)
                   & data_a_i (4) & data_a_i (4) & data_a_i (5) & data_a_i (5) & data_a_i (6) & data_a_i (6) & data_a_i (7) & data_a_i (7)
                   & data_a_i (8)& data_a_i (8)& data_a_i (9)& data_a_i (9);
         next_state <= s10;
         enable <= '1';
      when others => 
         data_a_o <= data_a_i (10) & data_a_i (10) & data_a_i (11) & data_a_i (11) & data_a_i (12) & data_a_i (12) & data_a_i (13) & data_a_i (13)
                   & data_a_i (14) & data_a_i (14) & data_a_i (15) & data_a_i (15) & data_a_i (16) & data_a_i (16) & data_a_i (17) & data_a_i (17)
                   & data_a_i (18)& data_a_i (18)& data_a_i (19)& data_a_i (19); 
         next_state <= s0;
         enable <= '0';
      end case;
   end if;
end process;


STATEMACHINE_OUTPUT_B: process(hd_zsd_i , current_state, data_b_i )
begin
   if hd_zsd_i  = '0' then   -- for sd signal
      case current_state is
      when s0 =>               
         data_b_o(19 downto 9)  <= (others => data_b_i (0));
         data_b_o(8 downto 0)   <= (others => data_b_i (1)); 
      when s1 =>               
         data_b_o(19 downto 18) <= (others => data_b_i (1));
         data_b_o(17 downto 7)  <= (others => data_b_i (2)); 
         data_b_o(6 downto 0)   <= (others => data_b_i (3));
      when s2 =>               
         data_b_o(19 downto 16) <= (others => data_b_i (3)); 
         data_b_o(15 downto 5)  <= (others => data_b_i (4));
         data_b_o(4 downto 0)   <= (others => data_b_i (5));
      when s3 =>               
         data_b_o(19 downto 14) <= (others => data_b_i (5)); 
         data_b_o(13 downto 3)  <= (others => data_b_i (6));
         data_b_o(2 downto 0)   <= (others => data_b_i (7));
      when s4 =>               
         data_b_o(19 downto 12) <= (others => data_b_i (7)); 
         data_b_o(11 downto 1)  <= (others => data_b_i (8));
         data_b_o(0)            <= data_b_i (9);
      when s5 =>               
         data_b_o(19 downto 10) <= (others => data_b_i (9)); 
         data_b_o( 9 downto  0) <= (others => data_b_i (10));      
      when s6 =>               
         data_b_o(19)           <= data_b_i (10); 
         data_b_o(18 downto 8)  <= (others => data_b_i (11));
         data_b_o(7 downto 0)   <= (others => data_b_i (12));    
      when s7 =>               
         data_b_o(19 downto 17) <= (others => data_b_i (12)); 
         data_b_o(16 downto 6)  <= (others => data_b_i (13));
         data_b_o(5 downto 0)   <= (others => data_b_i (14));
      when s8 =>               
         data_b_o(19 downto 15) <= (others => data_b_i (14)); 
         data_b_o(14 downto 4)  <= (others => data_b_i (15));
         data_b_o(3 downto 0)   <= (others => data_b_i (16));
      when s9 =>               
         data_b_o(19 downto 13) <= (others => data_b_i (16)); 
         data_b_o(12 downto 2)  <= (others => data_b_i (17));
         data_b_o(1 downto 0)   <= (others => data_b_i (18));
      when s10 =>              
         data_b_o(19 downto 11) <= (others => data_b_i (18));
         data_b_o(10 downto 0)  <= (others => data_b_i (19));
      end case;
   else            --for hd signal
      case current_state is  
      when s0 =>
         data_b_o <= data_b_i (0) & data_b_i (0) & data_b_i (1) & data_b_i (1) & data_b_i (2) & data_b_i (2) & data_b_i (3) & data_b_i (3)
                   & data_b_i (4) & data_b_i (4) & data_b_i (5) & data_b_i (5) & data_b_i (6) & data_b_i (6) & data_b_i (7) & data_b_i (7)
                   & data_b_i (8) & data_b_i (8) & data_b_i (9) & data_b_i (9);
      when others => 
         data_b_o <= data_b_i (10) & data_b_i (10) & data_b_i (11) & data_b_i (11) & data_b_i (12) & data_b_i (12) & data_b_i (13) & data_b_i (13)
                   & data_b_i (14) & data_b_i (14) & data_b_i (15) & data_b_i (15) & data_b_i (16) & data_b_i (16) & data_b_i (17) & data_b_i (17)
                   & data_b_i (18) & data_b_i (18) & data_b_i (19) & data_b_i (19); 
      end case;
   end if;
end process;


enable_registering : process(clk_i)
begin
if clk_i'event and clk_i = '1' then
   if sync_reset_i = '1' then
      enable_o  <= '0';
   else
      enable_o  <= enable;
   end if;
end if;
end process;


end behavioral;    
   