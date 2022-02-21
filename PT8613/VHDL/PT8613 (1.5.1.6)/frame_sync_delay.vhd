library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity frame_sync_delay is
   port (
	   mreset_i       : in std_logic;                     -- Master reset
	   clk_i         	: in std_logic;                     -- sample clock
	   sync_i         : in std_logic;                     
      timing_i       : in std_logic_vector(23 downto 0); -- delay from sync reference
	                                                      -- Input parameters must always be valid
	   frame_sync_o	: out std_logic
   );
end frame_sync_delay;


architecture Behavioral of frame_sync_delay is

signal delay_count         : std_logic_vector(23 downto 0); 
signal sync                : std_logic;
signal sync_mask           : std_logic;
signal count_en            : std_logic;
signal count_load          : std_logic;
signal count_is_2_delayed1 : std_logic;

constant extra_delay       : integer := -21;   -- -10; -- 10  range: +/- 1/2 frame


begin


--------------------------------------------------------------
-- Reclock sync pulse
--------------------------------------------------------------
Reclocking_sync : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		sync <= '0';
	elsif clk_i'event and clk_i = '1' then
		sync <= sync_i;
   end if;
end process;


--------------------------------------------------------------
-- Delay sync pulse
--------------------------------------------------------------
-- timing_i must be >= 2 !


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
	   if delay_count = conv_std_logic_vector(2**24-extra_delay+2,24) then
	      count_is_2_delayed1 <= '1';
	   else
	      count_is_2_delayed1 <= '0';
	   end if;
	end if;
end process;


sync_delay_counting : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		delay_count <= conv_std_logic_vector(0,24);
	elsif clk_i'event and clk_i = '1' then
		if count_load = '1' then
		   delay_count <= timing_i;
		elsif count_en = '1' then
		   delay_count <= delay_count -1;
		end if;
	end if;
end process;


-------------------------------------------------------
-- Mapping of sync output:
-------------------------------------------------------
frame_sync_o <= count_is_2_delayed1;


end Behavioral;