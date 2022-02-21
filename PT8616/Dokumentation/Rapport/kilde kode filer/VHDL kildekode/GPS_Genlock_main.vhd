-----------------------------------------------------------
--				 Main module
-----------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- bibliotek til brug af IBUFGDS (clock niveauer)
library UNISIM;
use UNISIM.VComponents.all;

entity GPS_Genlock_Module is
port (
	--inputs
	clk_p_i    	: 	in std_logic;			--clock positiv
   clk_n_i    	: 	in std_logic;			--clock negativ
	scl_i			:	in std_logic;			--SCL til I2C
	--reset_i		:	in std_logic;			--master reset
	--dip0_i		: 	in std_logic;			--dip 0 input
	--dip1_i		:	in std_logic;			--dip 1 input
	--dip2_i		:	in std_logic;			--dip 2 input
	--dip3_i		:	in std_logic;			--dip 3 input
	--freq_i		:	in std_logic;			--10 MHz input fra GPS
	--led_red_i	:	in std_logic;			--rød LED fra GPS
	--led_green_i	:	in std_logic;		--grøn LED fra GPS
	pps_i			:	in std_logic;			--1 Puls pr sekund fra GPS
	
	--outputs	
	--ns_d_o		:	out std_logic;			--genlock signal output
	--h_d_o			:	out std_logic;			--genlock signal output
	--f_d_o			:	out std_logic;			--genlock signal output
	VCXO_ctrl_o	:	out std_logic;			--VCXO-control output
	--event_o		:	out std_logic;			--eventpuls til GPS
	--LTC_code_o	:	out std_logic;			--LTC_code output
	led0_o		:	out std_logic;			--debug led
	led1_o		:	out std_logic;			--debug led
	
	--in/outputs
	MCU_gpio0_io	:	out std_logic;		--general purpose io til MCU
	MCU_gpio1_io	:	out std_logic;		--general purpose io til MCU
	MCU_gpio2_io	:	in std_logic;		--general purpose io til MCU
	MCU_gpio3_io	:	in std_logic;		--general purpose io til MCU
	gpio0_io			:	in std_logic;		--general purpose io til GPS			
	--gpio1_io			:	inout std_logic;		--general purpose io til GPS
	--gpio2_io			:	inout std_logic;		--general purpose io til GPS
	--gpio3_io			:	inout std_logic;		--general purpose io til GPS
	sda_io			:	inout std_logic		--SDA til I2C
);

end GPS_Genlock_Module;

architecture Behavioral of GPS_Genlock_module is

-- States til state-machine
type state_type is (start_up, wait_for_GPS, wait_for_PLL, wait_for_time, wait_for_1pps, stable, unstable);
signal kernel_state				: state_type:=start_up;

-- System signals -----------------------------------------------
-- master clock
signal clk						:	std_logic;		--148.5 MHz clock

-- system resets
signal kernel_reset			:	std_logic;
signal master_reset			:	std_logic;

-- system status variables
signal master_status_flag	:	std_logic;
signal PLL_status_flag		:	std_logic;
signal PLL_locked_flag		:	std_logic;
signal GPS_locked_flag		:	std_logic;
signal PLL_lock_delayline	:	std_logic_vector (9 downto 0);
signal sec_pulse_delayline	:	std_logic_vector (1 downto 0);
signal sample_period			:	std_logic_vector (27 downto 0);
signal time_recieved			:	std_logic;
signal offset					:	std_logic_vector (31 downto 0);

-- I2C signaler
signal I2C_read_flag			: 	std_logic;
signal I2C_read_delayline	:	std_logic_vector (1 downto 0);
signal send_byte				:	std_logic_vector (7 downto 0);
signal recieved_byte			:	std_logic_vector (7 downto 0);
signal byte_count				:	std_logic_vector (2 downto 0);
signal I2C_reset				:	std_logic;

-- System timing ------------------------------------------------
-- GPS PLL reference clock
-- 1PPS free run
signal sec_pulse				: 	std_logic;
signal faster_tick			:	std_logic;
signal slower_tick			:	std_logic;

signal GPS_10_mhz				:	std_logic;
signal GPS_ref_clk			:	std_logic;
signal GPS_ref_count			:	std_logic_vector(4 downto 0);

-- PLL decimeret output
signal clk_vcxo_dec			:	std_logic;

-- I2C interface komponent
component I2C_engine Port (
		reset_i 				: in  STD_LOGIC;
      clk_i 				: in  STD_LOGIC;
      SCL_i 				: in  STD_LOGIC;
      SDA_io 				: inout  STD_LOGIC;
      send_byte_i 		: in  STD_LOGIC_VECTOR (7 downto 0);
		recieved_byte_o 	: out STD_LOGIC_VECTOR (7 downto 0);
		address_i			: in 	STD_LOGIC_VECTOR (6 downto 0);
		byte_read_o			: out STD_LOGIC
	);
end component;
	
-- Digital PLL komponent
component digital_pll is
   generic (
      vcxo_div_per         : integer         -- decimation of clk_i to pll, (594)
   );
   port (
      clk_vcxo_i           : in std_logic;
      clk_ref_decimated_i  : in std_logic;
      clk_vcxo_dec_tick_o  : out std_logic;  -- tick at falling edge of decimated vcxo to phase comparator 
      pdm_o                : out std_logic;  -- pulse density modulated signal
      clk_vcxo_decimated_o : out std_logic;  -- for debug
      lock_warning_o       : out std_logic;
      lock_error_o         : out std_logic;
		ext_make_shorter		: in std_logic;
		ext_make_longer		: in std_logic
   );
end component;

component clock_compensate is
   port (
      rst_i				:	in	std_logic;
		clk_i				:	in std_logic;
		pps_i				:	in	std_logic;
		synth_pps_o		:	out std_logic;
		phase_diff_o	:	out std_logic_vector(7 downto 0);
		faster_tick		:	out std_logic;
		slower_tick		:	out std_logic
   );
end component;

begin


----------------------------------------------------------------------
--------------    Instansering af komponenter    ---------------------
----------------------------------------------------------------------
	diff_buf_clk : IBUFGDS
	generic map (IOSTANDARD => "LVDS_25_DT")
	port map (
		O  => clk,
		I  => clk_p_i,
		IB => clk_n_i );
		
	-- I2C interface til LTC encoder	
	I2C_interface: I2C_engine port map (
		reset_i => I2C_reset,
		clk_i => clk,
		SCL_i => SCL_i,
		SDA_io => SDA_io,
		send_byte_i => send_byte,
		recieved_byte_o => recieved_byte,
		address_i => "1010000",
		byte_read_o => I2C_read_flag
		);
	
	-- PLL til VCXO med GPS 0.25 MHz som reference
	pll : digital_pll
	generic map(
		vcxo_div_per 	=>	594
	)
	port map (
		clk_vcxo_i => clk,
      clk_ref_decimated_i => GPS_ref_clk,
		pdm_o => VCXO_ctrl_o,
		clk_vcxo_decimated_o => clk_vcxo_dec,
		--lock_error_o => led0_o,
		lock_warning_o => PLL_locked_flag,
		ext_make_longer => slower_tick,
		ext_make_shorter => faster_tick
	);
	
	pps_synth : clock_compensate port map (
		rst_i => kernel_reset,
		clk_i => clk,
		pps_i => pps_i,
		synth_pps_o => sec_pulse,
		--phase_diff_o => send_byte,
		faster_tick	=> faster_tick,
		slower_tick => slower_tick
	);		
	
	
----------------------------------------------------------------------
--------------   hovedkerne statemachine styring   -------------------
----------------------------------------------------------------------
	kernel_machine : process(master_reset, clk)
	begin	
		if clk'event and clk='1' then
			if master_reset='1' then
				kernel_state <= start_up;
			else
				if kernel_state=start_up then
					kernel_state <= wait_for_GPS;
				elsif kernel_state=wait_for_GPS and GPS_locked_flag='1' then
					kernel_state <= wait_for_PLL;
				elsif kernel_state=wait_for_PLL and PLL_status_flag='1' then
					kernel_state <= wait_for_time;
				elsif kernel_state=wait_for_time and time_recieved='1' then
					kernel_state <= wait_for_1pps;
				elsif kernel_state=wait_for_1pps and sec_pulse_delayline = "01" then
					kernel_state <= stable;
				elsif kernel_state=stable and GPS_locked_flag='0' then
					kernel_state <= unstable;
				elsif kernel_state=unstable and GPS_locked_flag='1' then
					kernel_state <= stable;
				end if;
			end if;
		end if;
	end process;
				
	kernel_machine_interpret : process(kernel_state)
	begin
		if kernel_state = start_up then
			kernel_reset <= '1';
			I2C_reset <= '1';
			send_byte <= "00000001";
		elsif kernel_state = wait_for_GPS then
			kernel_reset <= '1';
			I2C_reset <= '1';
			send_byte <= "00000010";
		elsif kernel_state = wait_for_PLL then
			kernel_reset <= '1';
			I2C_reset <= '1';
			send_byte <= "00000011";
		elsif kernel_state = wait_for_time then
			mcu_gpio1_io <= PLL_status_flag;
			kernel_reset <= '1';
			I2C_reset <= '0';
			send_byte <= "00000100";
		elsif kernel_state = wait_for_1pps then
			kernel_reset <= '1';
			I2C_reset <= '0';
			send_byte <= "00000101";
		elsif kernel_state = stable then
			kernel_reset <= '0';
			I2C_reset <= '0';
			send_byte <= "00000110";
		elsif kernel_state = unstable then
			kernel_reset <= '0';
			I2C_reset <= '0';
			send_byte <= "00000111";
		end if;
	end process;

----------------------------------------------------------------------
-----------------------    I2C-håndtering    -------------------------
----------------------------------------------------------------------

	I2C_handling : process(clk, I2C_reset)
	begin
		if clk'event and clk= '1' then
			if I2C_reset = '1' then
				byte_count <= "000";
				time_recieved <= '0';
			else
				I2C_read_delayline <= I2C_read_delayline(0) & I2C_read_flag;
				
				if I2C_read_delayline = "01" then
					if recieved_byte = "10111011" then
						byte_count <= "000";
					else
						byte_count <= byte_count + 1;
						case byte_count is
							when "001" =>
								offset(7 downto 0) <= recieved_byte;
							when "010" =>
								offset(15 downto 8) <= recieved_byte;
							when "011" =>
								offset(23 downto 16) <= recieved_byte;
							when "100" =>
								offset(31 downto 24) <= recieved_byte;
							when others =>
								offset <= "00000000000000000000000000000000";
						end case;
					end if;
				end if;
		
				if byte_count = "011" then
					time_recieved <= '1';
				else
					time_recieved <= '0';
				end if;
				
			end if;
		end if;
	end process;
					
----------------------------------------------------------------------
------------------   master-signaler og clocks   ---------------------
----------------------------------------------------------------------
	--generer 250 KHz til PLL fra 10 MHz GPS clock
	GPS_ref_clk_gen : process(GPS_10_mhz, master_reset)
	begin
		if master_reset = '1' then
			GPS_ref_count <= conv_std_logic_vector(19, 5);
			GPS_ref_clk <= '0';
		elsif GPS_10_mhz'event and GPS_10_mhz='1' then
			GPS_ref_count <= GPS_ref_count-1;
			if GPS_ref_count = 0 then
				GPS_ref_count <= conv_std_logic_vector(19, 5);
				GPS_ref_clk <= NOT GPS_ref_clk;
			end if;
		end if;
	end process;
	
	--delay_line generator for div signaler
	delayline_gen : process(clk, master_reset)
	begin
		if clk'event and clk='1' then
			if master_reset='1' then
				PLL_lock_delayline <= "0000000000";
				sample_period <= conv_std_logic_vector(148499999, 28);
				sec_pulse_delayline <= "00";
			else
				--intern 1 PPS delayline
				sec_pulse_delayline <= sec_pulse_delayline(0) & pps_i;
				
				--PLL lock line
				if sample_period = 0 then
					sample_period <= conv_std_logic_vector(148499999, 28);
					PLL_lock_delayline <= PLL_lock_delayline(8 downto 0) & not PLL_locked_flag;
					
					if PLL_lock_delayline="1111111111" then
						PLL_status_flag <= '1';
					else
						PLL_status_flag <= '0';
					end if;
				else
					sample_period <= sample_period-1;
				end if;
			end if;
		end if;
	end process;


----------------------------------------------------------------------
-----------------   hovedkerne signal-routing   ----------------------
----------------------------------------------------------------------
	--output for LEDs (debuggig)
	led0_o <= sec_pulse;
	led1_o <= master_reset;--pps_i;

	--master signaler
	master_reset <= mcu_gpio3_io;--reset_i;

	--in/outs
	mcu_gpio0_io <= sec_pulse;
	GPS_10_mhz <= gpio0_io;
	GPS_locked_flag <= mcu_gpio2_io;
end Behavioral;

