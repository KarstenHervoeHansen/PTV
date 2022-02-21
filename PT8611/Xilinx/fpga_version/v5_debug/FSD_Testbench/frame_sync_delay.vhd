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
	   spl_div_i       : in std_logic_vector(9 downto 0);  -- total samples pr line divided by 10 or 11 -1
      div10_11_i      : in std_logic;                     -- divider f148 med 10 hvis lav, 11 hvis høj
	   lpf_i           : in std_logic_vector(10 downto 0); -- total lines per frame
	   samplerate_x2_i : in std_logic;
      phasedelay_i    : in std_logic_vector(23 downto 0); -- delay from sync reference
	                                                       -- Input parameters must always be valid
	   frame_sync_o	 : out std_logic
   );
end frame_sync_delay;


architecture Behavioral of frame_sync_delay is

signal div10_11_count      : std_logic_vector(3 downto 0); -- counter for 10 or 11 samples
signal line_count          : std_logic_vector(9 downto 0);  
signal frame_count         : std_logic_vector(10 downto 0);
signal double_clk_count    : std_logic;
signal double_clk          : std_logic; 

signal delay_count         : std_logic_vector(23 downto 0); 
signal sync                : std_logic;
signal sync_reclk          : std_logic;
signal sync_delayed        : std_logic;
signal sync_mask           : std_logic;
signal count_en            : std_logic;
signal count_load          : std_logic;
signal count_is_2_delayed1 : std_logic;

signal sync_extra_delayed  : std_logic;
signal extra_delay_count   : std_logic_vector(6 downto 0);  
constant extra_delay       : integer := 9;

signal sync_waiting        : std_logic;
signal frame_sync          : std_logic;

begin


--------------------------------------------------------------
-- Reclock sync pulse
--------------------------------------------------------------
Reclocking_sync : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		sync_reclk <= '0';
	elsif clk_i'event and clk_i = '1' then
		sync_reclk <= sync_i;
   end if;
end process;


--------------------------------------------------------------
--Generate extra delayed sync input:
--------------------------------------------------------------
extra_delayed_sync : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		sync_extra_delayed <= '0';
		extra_delay_count <= conv_std_logic_vector(0,7);
	elsif clk_i'event and clk_i = '1' then
		if sync_reclk = '1' then
		   extra_delay_count <= conv_std_logic_vector(extra_delay,7);
		   sync_extra_delayed <= '0';
		elsif extra_delay_count /= conv_std_logic_vector(0,7) then
		   extra_delay_count <= extra_delay_count - 1;
		   if extra_delay_count = conv_std_logic_vector(1,7) then
		      sync_extra_delayed <= '1';
		   else
		      sync_extra_delayed <= '0';
		   end if;
      else
         sync_extra_delayed <= '0';
		end if;
   end if;
end process;


--------------------------------------------------------------
-- Choose if extra delayed sync should be used: (for x2 samplerate)
--------------------------------------------------------------
sync_delay_choosing : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		sync <= '0';
	elsif clk_i'event and clk_i = '1' then
		if samplerate_x2_i = '0' then
		   sync <= sync_reclk;
		else
		   sync <= sync_extra_delayed;
		end if;
   end if;
end process;


--------------------------------------------------------------
-- Delay sync pulse
--------------------------------------------------------------
-- går galt hvis phasedelay_i < 2 !!!


sync_masking : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		sync_mask <= '1';
	elsif clk_i'event and clk_i = '1' then
	   if sync = '1' and sync_mask = '1' then
	      sync_mask <= '0';
	   elsif count_is_2_delayed1 = '1' then
	      sync_mask <= '1';
	   end if;
	end if;
end process;


count_loading : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		count_load <= '0';
	elsif clk_i'event and clk_i = '1' then
	   if sync = '1' and sync_mask = '1' then
	      count_load <= '1';
	   else
	      count_load <= '0';
	   end if;
	end if;
end process;


count_enabling : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		count_en <= '0';
	elsif clk_i'event and clk_i = '1' then
	   if count_load = '1' then
	      count_en <= '1';
	   elsif count_is_2_delayed1 = '1' then
	      count_en <= '0';
	   end if;
	end if;
end process;


count_is_2_checking : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		count_is_2_delayed1 <= '0';
	elsif clk_i'event and clk_i = '1' then
	   if delay_count = conv_std_logic_vector(2,24) then
	      count_is_2_delayed1 <= '1';
	   else
	      count_is_2_delayed1 <= '0';
	   end if;
	end if;
end process;


sync_delay_counting : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		delay_count <= conv_std_logic_vector(0,24); -- phasedelay_i may make design smaller
	elsif clk_i'event and clk_i = '1' then
		if count_load = '1' then
		   delay_count <= phasedelay_i;
		elsif count_en = '1' then
		   delay_count <= delay_count -1;
		end if;
	end if;
end process;


sync_delayed <= count_is_2_delayed1;


-------------------------------------------------------------
-- Circular counter with a frame period. Made of 4 counters.
-------------------------------------------------------------
double_clk <= '0';

frame_counting : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
	   div10_11_count <= "101" & div10_11_i;
	   line_count <= spl_div_i;
	   frame_count <= lpf_i;
	   double_clk_count <= double_clk;
	   frame_sync <= '0';
	   sync_waiting <= '1';
	elsif clk_i'event and clk_i = '1' then
		if sync_delayed = '1' then
		   div10_11_count <= "101" & div10_11_i;
	      line_count <= spl_div_i;
	      frame_count <= lpf_i;
	      double_clk_count <= double_clk;
	      frame_sync <= '1';
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
		               frame_sync <= '1';
		            else
		               frame_count <= frame_count - 1;
		               frame_sync <= '0';
		            end if;
		         else
		            line_count <= line_count - 1;
		            frame_sync <= '0';
		         end if;
		      else
		         div10_11_count <= div10_11_count - 1;
		         frame_sync <= '0';
		      end if;
		   else
		      double_clk_count <= '0';
		      frame_sync <= '0';
		   end if;
		   
		end if;
	end if;
end process;     


-------------------------------------------------------
-- Mapping of sync output:
-------------------------------------------------------

frame_sync_o <= sync_delayed;             --frame_sync

end Behavioral;