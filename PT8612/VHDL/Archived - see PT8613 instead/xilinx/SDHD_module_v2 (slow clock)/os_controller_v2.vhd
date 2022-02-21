--use work.pack_oversample_by_11.all;
library IEEE; 
use IEEE.STD_LOGIC_1164.ALL; 
use IEEE.STD_LOGIC_ARITH.ALL; 
use IEEE.STD_LOGIC_UNSIGNED.ALL;  

entity os_controller is
   port(
       clk_148       :  in  std_logic;
       reset         :  in  std_logic;
       sync_reset_i  :  in  std_logic;
       hdsd_in       :  in  std_logic;
       data_in       :  in  std_logic_vector(19 downto 0);
       data_out      :  out std_logic_vector(19 downto 0);
       enable_out    :  out std_logic;
       debug_clk_o   :  out std_logic;
       debug_data_o  :  out std_logic_vector(9 downto 0)
   );
end os_controller;


architecture behavioral of os_controller is

type state is (s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10);
signal current_state    : state := s0;
signal next_state       : state;
signal enable           : std_logic;


attribute uselowskewlines : string;
attribute uselowskewlines of enable_out: signal is "yes";


begin

-----------------------------------------------------------
-- Parallel data out for debug :
-----------------------------------------------------------

debug_data_output : process(hdsd_in, current_state, data_in, clk_148)
begin
   if hdsd_in = '0' then   -- for sd signal
      case current_state is
      when s0 =>               
         debug_data_o  <= data_in(9 downto 0); 
         debug_clk_o   <= '0'; 
      when s1 =>               
         debug_data_o  <= data_in(9 downto 0); 
         debug_clk_o   <= '0'; 
      when s2 =>               
         debug_data_o  <= data_in(9 downto 0); 
         debug_clk_o   <= '1'; 
      when s3 =>               
         debug_data_o  <= data_in(9 downto 0); 
         debug_clk_o   <= '1'; 
      when s4 =>               
         debug_data_o  <= data_in(9 downto 0); 
         debug_clk_o   <= '1'; 
      when s5 =>               
         debug_data_o  <= data_in(19 downto 10); 
         debug_clk_o   <= '0'; 
      when s6 =>               
         debug_data_o  <= data_in(19 downto 10); 
         debug_clk_o   <= '0'; 
      when s7 =>               
         debug_data_o  <= data_in(19 downto 10); 
         debug_clk_o   <= '1'; 
      when s8 =>               
         debug_data_o  <= data_in(19 downto 10); 
         debug_clk_o   <= '1'; 
      when s9 =>               
         debug_data_o  <= data_in(19 downto 10); 
         debug_clk_o   <= '1'; 
      when s10 =>              
         debug_data_o  <= data_in(19 downto 10); 
         debug_clk_o   <= '1'; 
      end case;
   else            --for hd signal
      case current_state is  
      when s0 =>
         debug_data_o <= data_in(9 downto 0);
      when others => 
         debug_data_o <= data_in(19 downto 10); 
      end case;
      debug_clk_o   <= not clk_148;
   end if;
  
end process;

-------------------------------------------------------------


STATE_REG : process(clk_148, reset)
begin
if reset = '0' then
   current_state  <= s10;
elsif clk_148'event and clk_148 = '1' then
   if sync_reset_i = '1' then
      current_state <= s0;
   else
      current_state <= next_state;  
   end if;
end if;
end process;


STATEMACHINE_OUTPUT: process(hdsd_in, current_state, data_in)
begin
  if hdsd_in = '0' then   -- for sd signal
    case current_state is
    when s0 =>               
       next_state <= s1;     
       enable <= '0';    
       data_out(19 downto 9)  <= (others => data_in(0));
       data_out(8 downto 0)   <= (others => data_in(1)); 
    when s1 =>               
       next_state <= s2;     
       enable <= '0';    
       data_out(19 downto 18) <= (others => data_in(1));
       data_out(17 downto 7)  <= (others => data_in(2)); 
       data_out(6 downto 0)   <= (others => data_in(3));
    when s2 =>               
       next_state <= s3;     
       enable <= '0';    
       data_out(19 downto 16) <= (others => data_in(3)); 
       data_out(15 downto 5)  <= (others => data_in(4));
       data_out(4 downto 0)   <= (others => data_in(5));
    when s3 =>               
       next_state <= s4;     
       enable <= '0';    
       data_out(19 downto 14) <= (others => data_in(5)); 
       data_out(13 downto 3)  <= (others => data_in(6));
       data_out(2 downto 0)   <= (others => data_in(7));
    when s4 =>               
       next_state <= s5;           
       enable <= '0';    
       data_out(19 downto 12) <= (others => data_in(7)); 
       data_out(11 downto 1)  <= (others => data_in(8));
       data_out(0)            <= data_in(9);
    when s5 =>               
       next_state <= s6;     
       enable <= '0';    
       data_out(19 downto 10) <= (others => data_in(9)); 
       data_out( 9 downto  0) <= (others => data_in(10));      
    when s6 =>               
       next_state <= s7;     
       enable <= '0';    
       data_out(19)           <= data_in(10); 
       data_out(18 downto 8)  <= (others => data_in(11));
       data_out(7 downto 0)   <= (others => data_in(12));    
    when s7 =>               
       next_state <= s8;     
       enable <= '0';    
       data_out(19 downto 17) <= (others => data_in(12)); 
       data_out(16 downto 6)  <= (others => data_in(13));
       data_out(5 downto 0)   <= (others => data_in(14));
    when s8 =>               
       next_state <= s9;     
       enable <= '0';    
       data_out(19 downto 15) <= (others => data_in(14)); 
       data_out(14 downto 4)  <= (others => data_in(15));
       data_out(3 downto 0)   <= (others => data_in(16));
    when s9 =>               
       next_state <= s10;    
       enable <= '1';    
       data_out(19 downto 13) <= (others => data_in(16)); 
       data_out(12 downto 2)  <= (others => data_in(17));
       data_out(1 downto 0)   <= (others => data_in(18));
    when s10 =>              
       next_state <= s0;     
       enable <= '0';    
       data_out(19 downto 11) <= (others => data_in(18));
       data_out(10 downto 0)  <= (others => data_in(19));
   end case;
  else            --for hd signal
    case current_state is  
    when s0 =>
       data_out <= data_in(0) & data_in(0) & data_in(1) & data_in(1) & data_in(2) & data_in(2) & data_in(3) & data_in(3)
                   & data_in(4) & data_in(4) & data_in(5) & data_in(5) & data_in(6) & data_in(6) & data_in(7) & data_in(7)
                   & data_in(8)& data_in(8)& data_in(9)& data_in(9);
       next_state <= s10;
       enable <= '1';
    when others => 
         data_out <= data_in(10) & data_in(10) & data_in(11) & data_in(11) & data_in(12) & data_in(12) & data_in(13) & data_in(13)
                     & data_in(14) & data_in(14) & data_in(15) & data_in(15) & data_in(16) & data_in(16) & data_in(17) & data_in(17)
                     & data_in(18)& data_in(18)& data_in(19)& data_in(19); 
       next_state <= s0;
       enable <= '0';
    end case;
  end if;
  
end process;


enable_registering : process(clk_148, reset)
begin
if reset = '0' then
   enable_out  <= '1';
elsif clk_148'event and clk_148 = '1' then
   if sync_reset_i = '1' then
      enable_out  <= '0';
   else
      enable_out  <= enable;
   end if;
end if;
end process;


end behavioral;    
   