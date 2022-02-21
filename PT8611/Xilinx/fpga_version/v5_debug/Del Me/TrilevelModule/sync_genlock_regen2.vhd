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
   	sync_period	       	: integer;										-- period of sync_i in clk_i
   	sync_delay	       	: integer										-- Delay to allign reconstructed sync 
	);
	port ( 
	   mreset_i             : in std_logic;                        -- Master reset
	   clk_i                : in std_logic;                        -- sample clock
      sync_i               : in std_logic;								-- sync input
	   sync_clean_o         : out std_logic;								-- reconstructed sync output
	   genlock_ok_o         : out std_logic;								-- high if genlock is ok
	   genlock_resync_o     : out std_logic;								-- high if genlock is almost ok
	   
	   -- for debuging:
	   pulse_window_o       : out std_logic;
	   sync_pulse_del_o     : out std_logic
   );
end sync_genlock_regen;

architecture Behavioral of sync_genlock_regen is
                                  
signal sync_1_delayed            : std_logic;
signal sync_2_delayed            : std_logic;
signal sync_pulse                : std_logic;
signal sync_pulse_delayed        : std_logic;
                                  
signal lookat_sync_pulse         : std_logic;
signal lookat_sync_pulse_delayed : std_logic;
                                  
signal sync_count                : std_logic_vector(25 downto 0); -- total samples per line
signal sync_delay_count          : std_logic_vector(25 downto 0); -- total samples per line

constant window_width 				: integer := 3;   -- Width of window where f4m_pulse_delayed is alowed
constant genlock_error_tolerance : integer := 1;   -- Acceptable number of genlock cycles with errors before resyncing


-- Genlock monitoring:
signal illegal_pulse					: std_logic;
signal illegal_pulse_temp			: std_logic;
signal no_pulse						: std_logic;
signal no_pulse_temp					: std_logic;
signal genlock_error_count			: std_logic_vector(2 downto 0); --genlock_error hold counter
signal window_start		   	   : std_logic;		-- Pulse normally just before sync_pulse_delayed
signal window_end						: std_logic;		-- Pulse normally just after sync_pulse_delayed
	
begin

-------------------------------------------------------------
-- Generates pulse at sync rising edge                     --
-------------------------------------------------------------

sync_pulsing : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
	    sync_1_delayed <= '1';
	    sync_2_delayed <= '1';
	    sync_pulse <= '0';
	elsif clk_i'event and clk_i = '1' then
		if sync_2_delayed = '0' and sync_1_delayed = '1' then
			sync_pulse <= '1';
		else
			sync_pulse <= '0';	
		end if;
		sync_1_delayed <= sync_i;
		sync_2_delayed <= sync_1_delayed;
	end if;
end process;     


--------------------------------------------------------------
-- Delays the sync_pulse signal by sync_delay+1
--------------------------------------------------------------

sync_delaying : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		sync_delay_count <= conv_std_logic_vector(0,26);
		sync_pulse_delayed <= '0';
	elsif clk_i'event and clk_i = '1' then
		if sync_delay_count = conv_std_logic_vector(1,26) then
			sync_pulse_delayed <= '1';
		else
			sync_pulse_delayed <= '0';
		end if;
		if sync_pulse = '1' then
			sync_delay_count <= conv_std_logic_vector(sync_delay,26);
		elsif sync_delay_count /=	conv_std_logic_vector(0,26) then
			sync_delay_count <= sync_delay_count - 1;
		end if;
	end if;
end process;


--------------------------------------------------------------
-- Circular counter in sync with the sync_pulse_delayed.
-- Tolerant to	jitter on sync_pulse on +- 1 clock.
--------------------------------------------------------------

sync_counting : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		sync_count <= conv_std_logic_vector(2*sync_period+window_width,26);	   -- N*2+window_width
		lookat_sync_pulse <= '1';
		genlock_resync_o <= '0';
	elsif clk_i'event and clk_i = '1' then
		if sync_count = conv_std_logic_vector(0,26) then
			sync_count <= conv_std_logic_vector(sync_period-1,26);
			lookat_sync_pulse <= '1';
			genlock_resync_o <= '0';
		elsif genlock_error_count = conv_std_logic_vector(0,3) and lookat_sync_pulse = '1' and sync_pulse_delayed = '1' then
			sync_count <= conv_std_logic_vector((window_width-3)/2,26);          -- place window symmetrical around pulse
			genlock_resync_o <= '1';
		else
			sync_count <= sync_count - 1;
			genlock_resync_o <= '0';
		end if;
		if sync_count = conv_std_logic_vector(window_width,26) then
			lookat_sync_pulse <= '0';
		end if;
	end if;
end process;


--------------------------------------------------------------
-- Makes sync pulses sync_clean_o
--------------------------------------------------------------
sync_cleaning : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		sync_clean_o <= '0';
	elsif clk_i'event and clk_i = '1' then
		if sync_count = conv_std_logic_vector(0,25) then
			sync_clean_o <= '1';
		else
			sync_clean_o <= '0';
		end if;
	end if;
end process;
	

--------------------------------------------------------------
-- Makes pulse at window start and end
--------------------------------------------------------------
window_start_end : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		window_start <= '0';
		window_end <= '0';
		lookat_sync_pulse_delayed <= '0';
	elsif clk_i'event and clk_i = '1' then
		lookat_sync_pulse_delayed <= lookat_sync_pulse;
		if lookat_sync_pulse = '0' and lookat_sync_pulse_delayed = '1' then
			window_start <= '1';
		else
			window_start <= '0';
		end if;
		if lookat_sync_pulse = '1' and lookat_sync_pulse_delayed = '0' then
			window_end <= '1';
		else
			window_end <= '0';
		end if;
	end if;
end process;


--------------------------------------------------------------
-- Indicates if an illegal sync_pulse occurs
--------------------------------------------------------------

--------------------------------------------------
-- ændres til winstart til winstop

illegal_pulse_check : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		illegal_pulse_temp <= '1';
		illegal_pulse <= '1';
	elsif clk_i'event and clk_i = '1' then
		if window_start = '1' then
			illegal_pulse <= illegal_pulse_temp;
			illegal_pulse_temp <= '0';
		else
			if lookat_sync_pulse = '1' and sync_pulse_delayed = '1' then
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
			no_pulse <= no_pulse_temp;
			no_pulse_temp <= '1';
		else
			if sync_pulse_delayed = '1' then
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
		genlock_error_count <= conv_std_logic_vector(0,3);
	elsif clk_i'event and clk_i = '1' then
		if sync_pulse_delayed = '1' then
			if no_pulse = '1' or illegal_pulse = '1' then						-- error
				if genlock_error_count /= conv_std_logic_vector(0,3) then	-- decrement towards 0
					genlock_error_count <= genlock_error_count - 1;
				end if;	
			else																				-- no error
				if genlock_error_count /= conv_std_logic_vector(genlock_error_tolerance,3) then	-- increment towards genlock_error_tolerance
					genlock_error_count <= genlock_error_count + 1;
				end if;
			end if;
		end if;
	end if;
end process;


--------------------------------------------------------------
-- Generate genlock_ok_o
--------------------------------------------------------------
genlock_ok_generation : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		genlock_ok_o <= '0';
	elsif clk_i'event and clk_i = '1' then
		if genlock_error_count = conv_std_logic_vector(genlock_error_tolerance,3) then
			genlock_ok_o <= '1';
		else
			genlock_ok_o <= '0';
		end if;
	end if;
end process;


-- For debuging:
pulse_window_o <= lookat_sync_pulse;
sync_pulse_del_o <= sync_pulse_delayed; 

end Behavioral;