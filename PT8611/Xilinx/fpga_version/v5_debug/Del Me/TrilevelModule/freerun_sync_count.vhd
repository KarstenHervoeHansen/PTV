library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
library UNISIM;
use UNISIM.VComponents.all;

entity freerun_sync_count is
   generic(
      sync_period : integer
   );
   Port(
      mreset_i : in std_logic;
      clk_i    : in std_logic;
      sync_o   : out std_logic
   );
end freerun_sync_count;

architecture Behavioral of freerun_sync_count is
signal sync_cnt : std_logic_vector(22 downto 0);

begin

--------------------------------------------------------------
-- Generate sync pulses with a period of sync_period
--------------------------------------------------------------
sync_counting : process(mreset_i, clk_i)
begin
if mreset_i = '0' then                                          
   sync_cnt <= (others=>'0');
   sync_o <= '0';
elsif clk_i'event and clk_i = '1' then
   if sync_cnt = conv_std_logic_vector(0,23) then
      sync_cnt <= conv_std_logic_vector(sync_period-1,23);
      sync_o <= '1';
   else
      sync_cnt <= sync_cnt - 1;
      sync_o <= '0';
   end if;
end if;    
end process;



---------------------------
-- Counting a frame

frame_counting : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
	   div10_11_count <= "101" & div10_11_i;
	   line_count <= spl_div_i;
	   frame_count <= lpf_i;
	   double_clk_count <= double_clk;
	   frame_sync_o <= '0';
	   sync_waiting <= '1';
	elsif clk_i'event and clk_i = '1' then
		if sync_delayed = '1' then
		   div10_11_count <= "101" & div10_11_i;
	      line_count <= spl_div_i;
	      frame_count <= lpf_i;
	      double_clk_count <= double_clk;
	      frame_sync_o <= '1';
	      sync_waiting <= '0';
		elsif sync_waiting = '0' then
		   if double_clk_count = '0' then
		      double_clk_count <= double_clk;
		      if div10_11_count = conv_std_logic_vector(1,4) then
		         div10_11_count <= "101" & div10_11_i;
		         if line_count = conv_std_logic_vector(0,10) then
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
		   else
		      double_clk_count <= '0';
		      frame_sync_o <= '0';
		   end if;
		   
		end if;
	end if;
end process;     

----------------


end Behavioral;