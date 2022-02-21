library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity frame_sync_delay is
   port ( 
	   mreset_i      	 : in std_logic;                     -- Master reset
	   clk_i         	 : in std_logic;                     -- sample clock
	   sync_i          : in std_logic;                     
	   spl_div_i       : in std_logic_vector(9 downto 0);  -- total samples pr line divided by 10 or 11
      div10_11_i      : in std_logic;                     -- divider f148 med 10 hvis lav, 11 hvis høj
	   lpf_i           : in std_logic_vector(10 downto 0); -- total lines per frame
      phasedelay_i    : in std_logic_vector(23 downto 0); -- delay from sync reference
	                                                       -- Input parameters must always be valid
	   frame_sync_o	 : out std_logic
   );
end frame_sync_delay;

architecture Behavioral of frame_sync_delay is

signal div10_11_count : std_logic_vector(3 downto 0);     -- counter for 10 or 11 samples
signal line_count     : std_logic_vector(9 downto 0);  
signal frame_count    : std_logic_vector(10 downto 0);  
signal delay_count    : std_logic_vector(23 downto 0); 

signal sync           : std_logic;
signal sync_delayed   : std_logic;
signal counting       : std_logic;
signal sync_waiting   : std_logic;
	
begin

--------------------------------------------------------------
-- Reclocking sync pulse
--------------------------------------------------------------
Reclocking_sync : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		sync <= '0';
	elsif clk_i'event and clk_i = '1' then
		sync <= sync_i;
   end if;
end process;


--------------------------------------------------------------
-- Delaying sync pulse
--------------------------------------------------------------
delaying_sync : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		sync_delayed <= '0';
		delay_count <= conv_std_logic_vector(0,24);
		counting <= '0';
	elsif clk_i'event and clk_i = '1' then
		if delay_count = conv_std_logic_vector(0,24) then
		   if sync = '1' then
		      delay_count <= phasedelay_i;
		      counting <= '1';
		   else
		      counting <= '0';
		   end if;
		   if counting = '1' then
		      sync_delayed <= '1';
		   else
		      sync_delayed <= '0';   
		   end if;
		else
		   if counting = '1' then
		      delay_count <= delay_count-1;
		   end if;
		   sync_delayed <= '0';
		end if;
	end if;
end process;


-------------------------------------------------------------
-- Circular counter with a frame period. Made of 3 counters.
-------------------------------------------------------------
frame_counting : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
	   div10_11_count <= "101" & div10_11_i;
	   line_count <= spl_div_i;
	   frame_count <= lpf_i;
	   frame_sync_o <= '0';
	   sync_waiting <= '1';
	elsif clk_i'event and clk_i = '1' then
		if sync_delayed = '1' then
		   div10_11_count <= "101" & div10_11_i;
	      line_count <= spl_div_i;
	      frame_count <= lpf_i;
	      frame_sync_o <= '1';
	      sync_waiting <= '0';
		elsif sync_waiting = '0' then
		   if div10_11_count = conv_std_logic_vector(1,4) then
		      div10_11_count <= "101" & div10_11_i;
		      if line_count = conv_std_logic_vector(1,10) then
		         line_count <= spl_div_i;
		         if frame_count = conv_std_logic_vector(1,10) then
		            frame_count <= lpf_i;
		            frame_sync_o <= '1';
		         else
		            frame_count <= frame_count - 1;
		            frame_sync_o <= '0';
		         end if;
		      else
		         line_count <= line_count - 1;
		         frame_sync_o <= '0';
		      end if;
		   else
		      div10_11_count <= div10_11_count - 1;
		      frame_sync_o <= '0';
		   end if;
		end if;
	end if;
end process;     


-----------------------------------------------------------
-- Make parameter clocking

end Behavioral;