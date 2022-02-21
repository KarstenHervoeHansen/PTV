library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity sync_genlock_regen is
   generic (
   	count_val1           : integer;                             -- the period of sync is count_val1*count_val2
      count_val2           : integer;
      bits1                : integer;                             -- number of bits in counters
      bits2                : integer
	);
	port ( 
	   mreset_i             : in std_logic;                        -- Master reset
	   clk_i                : in std_logic;                        -- sample clock
      sync_i               : in std_logic;								-- sync input
	   sync_clean_o         : out std_logic;								-- reconstructed sync output
	   genlock_ok_o         : out std_logic								-- high if genlock is ok
	   --for debug:
--	   sync_reset_o         : out std_logic
   );
end sync_genlock_regen;


architecture Behavioral of sync_genlock_regen is

signal sync_delayed_1            : std_logic;
signal sync_delayed_2            : std_logic;
signal sync_pulse                : std_logic;
signal sync_pulse_delayed_1      : std_logic;
signal sync_pulse_delayed_2      : std_logic;
signal sync_pulse_delayed_3      : std_logic;

signal window_start		   	   : std_logic;		-- Pulse normally just before sync_pulse_delayed_3
signal window_mid						: std_logic;		-- Pulse normally at sync_pulse_delayed_3
signal window_end						: std_logic;		-- Pulse normally just after sync_pulse_delayed_3
signal sync_reset                : std_logic;

-- Genlock monitoring:
signal illegal_pulse					: std_logic;
signal illegal_pulse_temp			: std_logic;
signal no_pulse						: std_logic;
signal no_pulse_temp					: std_logic;
signal genlock_error_count			: std_logic_vector(2 downto 0); --genlock_error hold counter
signal genlock_resync            : std_logic;


component period_dual_count
   generic(
      count_val1 : integer;
      count_val2 : integer;
      bits1      : integer;
      bits2      : integer
   );
   Port(
      mreset_i    : in std_logic;
      clk_i       : in std_logic;
      sync_reset_i: in std_logic;
      sync_o      : out std_logic
   );
end component;                


begin


-------------------------------------------------------------
-- Generates pulse at sync rising edge
-------------------------------------------------------------
sync_pulsing : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
	   sync_delayed_1 <= '1';
	   sync_delayed_2 <= '1';
	   sync_pulse <= '0';
	elsif clk_i'event and clk_i = '1' then
		if sync_delayed_2 = '0' and sync_delayed_1 = '1' then
			sync_pulse <= '1';
		else
			sync_pulse <= '0';	
		end if;
		sync_delayed_1 <= sync_i;
		sync_delayed_2 <= sync_delayed_1;
	end if;
end process;     


-------------------------------------------------------------
-- Delay sync_pulse
-------------------------------------------------------------
sync_pulse_delaying : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
	   sync_pulse_delayed_1 <= '0';
	   sync_pulse_delayed_2 <= '0';
	   sync_pulse_delayed_3 <= '0';
	elsif clk_i'event and clk_i = '1' then
	   sync_pulse_delayed_1 <= sync_pulse;
	   sync_pulse_delayed_2 <= sync_pulse_delayed_1;
	   sync_pulse_delayed_3 <= sync_pulse_delayed_2;
	end if;
end process;


--------------------------------------------------------------
-- Circular counter with same period as the sync pulse.
--------------------------------------------------------------
sync_period_counting : period_dual_count
generic map (
   count_val1 => count_val1,
   count_val2 => count_val2,
   bits1      => bits1,     
   bits2      => bits2     
)
port map (
   mreset_i     => mreset_i,
   clk_i        => clk_i,
   sync_reset_i => sync_reset,
   sync_o       => window_start
);


-------------------------------------------------------------
-- Delay line of window pulses
-------------------------------------------------------------
winpulse_delaying : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
	   window_mid <= '0';
	   window_end <= '0';
	elsif clk_i'event and clk_i = '1' then
		window_mid <= window_start;
		window_end <= window_mid;
	end if;
end process;     


-------------------------------------------------------------
-- Control the circular sync counter reset
-------------------------------------------------------------
sync_reset_controlling : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
	    sync_reset <= '0';
	elsif clk_i'event and clk_i = '1' then
	   if genlock_resync = '1' and sync_pulse = '1' then
	      sync_reset <= '1';
	   else
	      sync_reset <= '0';
		end if;
	end if;
end process;     


--------------------------------------------------------------
-- Indicates if an illegal sync_pulse occurs
--------------------------------------------------------------
illegal_pulse_check : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		illegal_pulse_temp <= '1';
		illegal_pulse <= '1';
	elsif clk_i'event and clk_i = '1' then
		if window_start = '1' then
			illegal_pulse <= illegal_pulse_temp;
			illegal_pulse_temp <= '0';
		else
			if (window_start = '0' and window_mid = '0' and window_end = '0' ) and sync_pulse_delayed_3 = '1' then
				illegal_pulse_temp <= '1';
			end if;
		end if;
	end if;
end process;


--------------------------------------------------------------
-- Indicates if no sync_pulse occurs
--------------------------------------------------------------
no_pulse_check : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		no_pulse_temp <= '1';
		no_pulse <= '1';
	elsif clk_i'event and clk_i = '1' then
		if window_end = '1' then
		   if sync_pulse_delayed_3 = '1' then
			   no_pulse <= '0';
			else
			   no_pulse <= no_pulse_temp;
			end if;
			no_pulse_temp <= '1';
		else
			if sync_pulse_delayed_3 = '1' then
				no_pulse_temp <= '0';
			end if;
		end if;
	end if;
end process;


--------------------------------------------------------------
-- Genlock error counting. One increment when no error and one
-- decrement when error in a sync_pulse_delayed period
--------------------------------------------------------------
genlock_error_counting : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		genlock_error_count <= "000";
		genlock_resync <= '1';
	elsif clk_i'event and clk_i = '1' then
	   if sync_reset = '1' then
	      genlock_error_count <= "111";
         genlock_resync <= '0';
		elsif sync_pulse_delayed_3 = '1' then
		   if no_pulse = '1' or illegal_pulse = '1' then	                  -- error
		   	if genlock_error_count /= "000" then	                        -- decrement towards 0
		   		genlock_error_count <= genlock_error_count - 1;             
		   		genlock_resync <= '0';                                      
		      else                                                           
		         genlock_resync <= '1';                                      -- the circular counter is resyncronized
		   	end if;	                                                      
		   else																				   -- no error
		   	if genlock_error_count /= "111" then	                        -- increment towards genlock_error_tolerance
		   		genlock_error_count <= genlock_error_count + 1;
		   	end if;
		   	genlock_resync <= '0';
		   end if;
		end if;
	end if;
end process;


--------------------------------------------------------------
-- Regenerated sync, delayed to match backend
--------------------------------------------------------------
sync_clean_delaying : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
	   sync_clean_o <= '0';
	elsif clk_i'event and clk_i = '1' then
		sync_clean_o <= window_start;
	end if;
end process;    


--------------------------------------------------------------
-- Generate genlock_ok_o
--------------------------------------------------------------
genlock_ok_o <= not genlock_resync;


--------------------------------------------------------------
-- For Debug:
--------------------------------------------------------------
--sync_reset_o <= sync_reset;

--f4m_clean_flip : process (mreset_i, f4m_clean) begin          
--   if mreset_i = '0' then                                          
--      f4m_clean_tog <= '1';
--   elsif f4m_clean'event and f4m_clean = '1' then                         
--      f4m_clean_tog <= not f4m_clean_tog;
--   end if;                                                      
--end process;
end Behavioral;