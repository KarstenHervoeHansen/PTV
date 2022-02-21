library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity reset_sequencer is
   port ( 
	   mreset_i             		: in std_logic;                        -- Master reset
	   clk_i                		: in std_logic;                        -- sample clock
	   reset_genlock_o				: out std_logic;
	   reset_genmon_o					: out std_logic;
	   tick_160ms_o					: out std_logic
   );
end reset_sequencer;

architecture Behavioral of reset_sequencer is

signal tick_160ms	 	      		: std_logic;
signal tick_160ms_count          : std_logic_vector(24 downto 0); 		-- counter for 160ms tick generation
signal reset_delay_count         : std_logic_vector(5 downto 0);  		-- counter to delay reset to genlock
signal reset_genmon_delay_count	: std_logic_vector(5 downto 0);  		-- counter to delay reset to genlock monitor
signal reset_genlock					: std_logic;
constant genlock_reset_delay		: integer :=63;                        -- 63
constant genmon_reset_delay		: integer :=12;                        -- 12

	
begin

-------------------------------------------------------------
-- Generates 160ms tick pulses
-------------------------------------------------------------

tick_160ms_generation : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
	   tick_160ms_count <= conv_std_logic_vector(23760000-1,25);        -- 23760000-1
		tick_160ms <= '0';
		tick_160ms_o <= '0';
	elsif clk_i'event and clk_i = '1' then
		if tick_160ms_count = conv_std_logic_vector(0,25) then
		   tick_160ms_count <= conv_std_logic_vector(23760000-1,25);     -- 23760000-1
		   tick_160ms <= '1';
		   tick_160ms_o <= '1';
		else
         tick_160ms_count <= tick_160ms_count-1;
         tick_160ms <= '0';
         tick_160ms_o <= '0';
		end if;
	end if;
end process;     


--------------------------------------------------------------
-- Delays the reset signal to genlok_regen
--------------------------------------------------------------
genlock_reset_delaying : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		reset_genlock <= '0';
		reset_genlock_o <= '0';
		reset_delay_count <= conv_std_logic_vector(genlock_reset_delay,6);
	elsif clk_i'event and clk_i = '1' then
		if reset_delay_count = conv_std_logic_vector(0,6) then
		   reset_genlock <= '1';
		else
         if tick_160ms = '1' then
         	reset_delay_count <= reset_delay_count-1;
         end if;
         reset_genlock <= '0';
		end if;
		reset_genlock_o <= reset_genlock;
	end if;
end process;


--------------------------------------------------------------
-- Delays the reset signal to the genlock status monitor
--------------------------------------------------------------
genmon_reset_delaying : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		reset_genmon_o <= '0';
		reset_genmon_delay_count <= conv_std_logic_vector(genmon_reset_delay,6);
	elsif clk_i'event and clk_i = '1' then
		if reset_genmon_delay_count = conv_std_logic_vector(0,6) then
		   reset_genmon_o <= '1';
		else
         if tick_160ms = '1' and reset_genlock = '1' then
         	reset_genmon_delay_count <= reset_genmon_delay_count-1;
         end if;
      end if;
	end if;
end process;

end Behavioral;