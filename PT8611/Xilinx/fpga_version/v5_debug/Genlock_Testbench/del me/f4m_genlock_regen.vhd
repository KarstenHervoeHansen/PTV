library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity sync_genlock_regen is
   generic (
   	err_holdtime : integer;
   	sync_period	 : integer;
   	sync_delay	 : integer
   ); 
	port ( 
	   mreset_i     : in std_logic;                       -- Master reset
	   clk_i        : in std_logic;                       -- sample clock
      sync_i        : in std_logic;
	   genlock_ok_o : out std_logic;
      sync_clean_o  : out std_logic
   );
end sync_genlock_regen;

architecture Behavioral of sync_genlock_regen is

type genlock_statetype is ( locked, unlocked, locking );

signal one_pulse_in_window   	  : std_logic;
signal no_pulse_outside_window  : std_logic;
signal genlock_error_count      : std_logic_vector(3 downto 0); --genlock_error hold counter
signal sync_1_delayed            : std_logic;
signal sync_2_delayed            : std_logic;
signal sync_pulse                : std_logic;
signal sync_pulse_delayed        : std_logic;
signal lookat_sync_pulse         : std_logic;
signal lookat_sync_pulse_delayed : std_logic;
signal sync_pulse_count	   	  : std_logic;
signal sync_count                : std_logic_vector(23 downto 0); -- total samples per line
signal sync_delay_count          : std_logic_vector(23 downto 0); -- total samples per line
-- constant genlock_error_hold     : integer := 15;			     	 -- hold time for genlock error (0 - 15)
-- constant sync_period             : integer := 5940000;		     	 -- 5940000
-- constant sync_delay              : integer := sync_period/2-5;		 -- 1 to sync_period/2 
--                                                             	 -- sync_period/2-5
	
begin

-------------------------------------------------------------
-- Generates pulse at sync rising edge                      --
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
		sync_delay_count <= conv_std_logic_vector(0,24);
		sync_pulse_delayed <= '0';
	elsif clk_i'event and clk_i = '1' then
		if sync_delay_count = conv_std_logic_vector(1,24) then
			sync_pulse_delayed <= '1';
		else
			sync_pulse_delayed <= '0';
		end if;
		if sync_pulse = '1' then
			sync_delay_count <= conv_std_logic_vector(sync_delay,24);
		elsif sync_delay_count /= conv_std_logic_vector(0,24) then
			sync_delay_count <= sync_delay_count - 1;
		end if;
	end if;
end process;


--------------------------------------------------------------
-- Circular counter in sync with the sync_pulse_delayed.
-- Tolerant to	jitter on sync_pulse on +- 1 clock.
--------------------------------------------------------------

sync_sync_counting : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		sync_count <= conv_std_logic_vector(2*sync_period+3,24);	    -- N*2+3
		lookat_sync_pulse <= '1';
	elsif clk_i'event and clk_i = '1' then
		if sync_count = conv_std_logic_vector(0,24) then
			sync_count <= conv_std_logic_vector(sync_period-1,24);
			lookat_sync_pulse <= '1';
		elsif lookat_sync_pulse = '1' and sync_pulse_delayed = '1' then
			sync_count <= (others => '0');
		else
		   	sync_count <= sync_count - 1;
		end if;
		if sync_count = conv_std_logic_vector(3,24) then
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
		if sync_count = conv_std_logic_vector(0,24) then
			sync_clean_o <= '1';
		else
			sync_clean_o <= '0';
		end if;
	end if;
end process;
	

--------------------------------------------------------------
-- Monitoring of the genlock status. Count number of pulses in
-- time window where lookat_sync_pulse = '0'. If the number of
-- pulses in the window is 1 one_pulse_in_window <= '1' ellers
-- 0. If a pulse occurs outside the window then 
-- no_pulse_outside_window <= '0'.
--------------------------------------------------------------


sync_pulse_monitoring : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		lookat_sync_pulse_delayed <= '0';
		sync_pulse_count <= '0';
		one_pulse_in_window <= '0';
		no_pulse_outside_window <= '0'; 
		lookat_sync_pulse_delayed <= '1';
	elsif clk_i'event and clk_i = '1' then
		if lookat_sync_pulse = '0' and lookat_sync_pulse_delayed = '1' then    --falling edge on lookat_sync_pulse
			if sync_pulse_delayed = '1' then
	     		sync_pulse_count <= '1';
			else
				sync_pulse_count <= '0';
			end if;
			one_pulse_in_window <= '1';
			no_pulse_outside_window <= '1';
		elsif lookat_sync_pulse = '0' and lookat_sync_pulse_delayed = '0' then	--inside window
			if sync_pulse_delayed = '1' then
			   if sync_pulse_count = '1' then
					one_pulse_in_window <= '0';
				else
					sync_pulse_count <= '1';
					one_pulse_in_window <= '1';
				end if;
			else
			   one_pulse_in_window <= '1';
			end if;
			no_pulse_outside_window <= '1';
		elsif lookat_sync_pulse = '1' and lookat_sync_pulse_delayed = '0' then --rising edge on lookat_sync_pulse 
			if sync_pulse_count = '1' then
			   one_pulse_in_window <= '1';
			else
				one_pulse_in_window <= '0';
			end if;
			if sync_pulse_delayed = '1' then
				no_pulse_outside_window <= '0';
			else
				no_pulse_outside_window <= '1';
			end if;
		else
		   one_pulse_in_window <= '1';
		   if sync_pulse_delayed = '1' then
				no_pulse_outside_window <= '0';	   
		   else
		   	no_pulse_outside_window <= '1';
		   end if;
		end if;
		lookat_sync_pulse_delayed <= lookat_sync_pulse;
	end if; 
end process;
	

----------------------------------------------------------------------
-- Holds genlock errors to make them visible on the leds. It counts on
-- the rising edges of then lookat_sync_pulse signal.
----------------------------------------------------------------------

genlock_error_holding : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		genlock_error_count <= conv_std_logic_vector(genlock_error_hold,4);
		genlock_ok_o <= '0';
	elsif clk_i'event and clk_i = '1' then
		if one_pulse_in_window = '0' or no_pulse_outside_window = '0' then --genlock error
			genlock_error_count <= conv_std_logic_vector(genlock_error_hold,4);
			genlock_ok_o <= '0';
		else
			if lookat_sync_pulse = '1' and lookat_sync_pulse_delayed = '0' then --rising edge of lookat_sync_pulse
				if genlock_error_count /= conv_std_logic_vector(0,4) then
			      genlock_error_count <= genlock_error_count - 1;
			   else
			      genlock_ok_o <= '1';
			   end if;
			end if;
		end if;
	end if;
end process;


end Behavioral;