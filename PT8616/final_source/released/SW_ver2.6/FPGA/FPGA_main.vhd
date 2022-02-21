	----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:08:49 08/14/2007 
-- Design Name: 
-- Module Name:    FPGA_main - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_SIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;

entity FPGA_main is
  Port ( 
			  -- clocks
			  clk_10_i : in  STD_LOGIC;
			  clk_148_p : in STD_LOGIC;
			  clk_148_n : in STD_LOGIC;
			  
			  -- debug io
           tp1_o : out  STD_LOGIC;
			  tp2_o : out  STD_LOGIC;
			  tp3_o : out	STD_LOGIC;
           led1_o : out  STD_LOGIC;
			  led2_o : out STD_LOGIC;
			  --jump_i : in STD_LOGIC;
			  
			  -- Sync out
			  h_d_o : out STD_LOGIC;
			  f_d_o : out STD_LOGIC;

			  -- LTC out
			  LTC_a_out : out STD_LOGIC;
			  LTC_b_out : out STD_LOGIC;
			  
			  -- OCXO (10 MHz) DAC
			  sync_10mhz_o : out STD_LOGIC;
			  sclk_10mhz_o : out STD_LOGIC;
			  din_10mhz_o : out STD_LOGIC;
			  
			  -- VCXO (148.5 MHz) DAC
			  sync_148mhz : out STD_LOGIC;
			  sclk_148mhz : out STD_LOGIC;
			  din_148mhz : out STD_LOGIC;
			  
			  -- GPS io 
			  PPS_i : in STD_LOGIC;
			  		  
			  -- I2C io
			  SDA_io : inout STD_LOGIC;
			  SCL_i : in STD_LOGIC;
			  
			  -- MCU-FPGA intercons
			  ext_int_o : out STD_LOGIC;
			  rst_i : in STD_LOGIC;
			  PPS_status_o : out STD_LOGIC;
			  
			  UBX_TX_i : in STD_LOGIC;
			  NTP_RX: out STD_LOGIC;
			  MCU_RX: out STD_LOGIC;
			  NTP_PPS: out STD_LOGIC
			  );

end FPGA_main;

architecture Behavioral of FPGA_main is

-- software version -- 11
-- Version 1.0 - First release
-- Version 1.1 - Added support for NTP option
-- Version 1.2 - Changed fast PLL logic (failed periodically). Also some code cleanup and conversion to ISE 12.3
-- Version 1.3 - Changed fast PLL again. Was unstable at boot in version 1.2.
constant software_version : std_logic_vector(7 downto 0) := "00010011"; 

-- system control
-- bit 0: 0=PAL/1=NTSC
-- bit 1: PAL sync sec. Høj hver 4 sekund, for sync (6.25 Hz gentages hver 4. sek)
-- bit 2: OCXO reset
signal system_control: std_logic_vector(7 downto 0);

-- format select, 0 = 625/50, 1 = 525/60
signal format_select : std_logic;

-- PAL sync sec
signal PAL_sync_sec : std_logic;

-- OCXO reset
signal OCXO_reset : std_logic;

-- OCXO hurtigt/langsomt loop control
signal OCXO_loop_control : std_logic;

-- OCXO loop fix control
signal OCXO_loop_fix : std_logic;


-- clock signaler
signal clk_148: std_logic;
signal clk_100: std_logic;

signal pps_148_tick : std_logic;				-- tick på én 148.5 MHz clockcycle

signal ms_tick : 	std_logic;	--tick på 148.5 mhz clock, der angiver 1 ms start, tilpasset GPS-tid
signal ms_count : integer range 0 to 1000; --tæller millisekunder, starter fra 0 på 1 PPS
signal ms_dec_count : integer range 0 to 148500; -- decimere 148.5 MHz til 1000 Hz

--video signaler
signal NTSC_offset : std_logic_vector(15 downto 0);	-- offset i ms (0 til 1000)
signal SPG_sync_signal : std_logic;	--sync-reset til sync-pulse-generator
signal PAL_sync : std_logic;			--sync signal til PAL (hvert hele sekund)
signal NTSC_sync : std_logic;			--sync signal til NTSC (1/1,001 sekund)

-- PAL horisontal konstanter
constant PAL_h_hi_time : integer := 1722;
constant PAL_h_lo_time : integer := 7782;
constant PAL_h_offset  : integer := 6714;

-- PAL field konstanter
constant PAL_f_hi_time : integer := 5932202*4+7798*3;
constant PAL_f_lo_time : integer := 7798;
constant PAL_f_offset  : integer := 7798;

-- NTSC horisontal konstanter
constant NTSC_h_hi_time : integer := 1656;
constant NTSC_h_lo_time : integer := 7782;
constant NTSC_h_offset  : integer := 6725;

-- NTSC field konstanter
constant NTSC_f_hi_time : integer := 4946920*2+8030;
constant NTSC_f_lo_time : integer := 8030;
constant NTSC_f_offset  : integer := 8030;

-- signaler til valgte video-format
signal hor_hi_time : integer;
signal hor_lo_time : integer;
signal hor_offset  : integer;

signal field_hi_time : integer;
signal field_lo_time : integer;
signal field_offset  : integer;

signal f_d  : std_logic;

-- LTC signaler
signal LTC_setup : std_logic_vector(7 downto 0);

-- LTC A
signal LTC_a_code : std_logic;
signal LTC_a_hours : std_logic_vector(7 downto 0);
signal LTC_a_mins : std_logic_vector(7 downto 0);
signal LTC_a_secs : std_logic_vector(7 downto 0);
signal LTC_a_frames : std_logic_vector(7 downto 0);
signal LTC_a_delay : std_logic_vector(31 downto 0);
signal LTC_a_dropframe : std_logic;
signal LTC_a_format : std_logic_vector(1 downto 0);
signal LTC_a_sync : std_logic;
signal LTC_a_debug : std_logic;

---- LTC B
signal LTC_b_code : std_logic;
signal LTC_b_hours : std_logic_vector(7 downto 0);
signal LTC_b_mins : std_logic_vector(7 downto 0);
signal LTC_b_secs : std_logic_vector(7 downto 0);
signal LTC_b_frames : std_logic_vector(7 downto 0);
signal LTC_b_delay : std_logic_vector(31 downto 0);
signal LTC_b_dropframe : std_logic;
signal LTC_b_format : std_logic_vector(1 downto 0);
signal LTC_b_sync : std_logic;
signal LTC_b_debug : std_logic;

signal kvant_pps : std_logic;

-- signaler til styring af OCXO (10 MHz) PLL
signal OCXO_cv_word : std_logic_vector(15 downto 0);
signal OCXO_update_dac : std_logic;
signal int_pps	: std_logic;
signal quant_error : std_logic_vector(7 downto 0);
signal OCXO_extern_value : std_logic_vector(15 downto 0);
signal OCXO_phase_diff : STD_LOGIC_VECTOR (15 downto 0);

-- signaler til styring af VCXO (148.5 MHz) PLL
signal VCXO_cv_word : std_logic_vector(15 downto 0);
signal VCXO_update_dac : std_logic;
signal int_pps_tick_100mhz : std_logic;
signal int_clk250k	: std_logic;
signal ref_clk250k	: std_logic;

-- I2C signaler
signal I2C_start : std_logic;
signal I2C_update : std_logic;
signal I2C_stop : std_logic;
signal I2C_byte_in : std_logic_vector (7 downto 0);
signal I2C_byte_out : std_logic_vector (7 downto 0);
signal I2C_debug : std_logic;

--status signaler
signal pps_status : std_logic;
signal pps_status_count : integer range 0 to 11000001;
signal pps_edge : std_logic_vector(1 downto 0);

--signal forlænger (til NTP)
signal delayline: std_logic_vector (1 downto 0);
signal count : integer range 0 to 1485000; -- decimere 148.5 MHz til 1000 Hz									 
signal longer_tick : std_logic;

signal clk_int : std_logic;
signal data_int : std_logic;
signal sync_int : std_logic;

-- Styring af AD5660 DAC
component ad5660_serial_out is
   port (
      clk_148_i                  : in std_logic;                        
      data_i                     : in std_logic_vector(15 downto 0);
      new_data_i                 : in std_logic;                        
      sclk_o                     : out std_logic;
      dout_o                     : out std_logic;
      zsync_o                    : out std_logic
   );
end component;

	COMPONENT ad5660_serial_DAC
	PORT(
		clk_i : IN std_logic;
		decimation_i : IN std_logic_vector(3 downto 0);
		word_i : IN std_logic_vector(15 downto 0);
		update_i : IN std_logic;          
		sclk_o : OUT std_logic;
		data_o : OUT std_logic;
		sync_o : OUT std_logic
		);
	END COMPONENT;


-- PLL (låser OCXO 10 MHz med GPS 1 PPS)
component GPS_PLL is
    Port ( GPS_PPS_i : in  STD_LOGIC;
           clk_i : in  STD_LOGIC;
			  rst_i : in STD_LOGIC;
			  fix_i : in STD_LOGIC;
			  OCXO_cv_value_i : in STD_LOGIC_VECTOR(15 downto 0);
			  OCXO_loop_control_i : in STD_LOGIC;
           cv_word_o : out  STD_LOGIC_VECTOR (15 downto 0);
			  phase_diff_o : out STD_LOGIC_VECTOR (15 downto 0);
			  update_dac_o : out STD_LOGIC;
			  int_PPS_tick_o : out STD_LOGIC;
			  int_PPS_o : out STD_LOGIC);
end component;


-- PLL (Låser VCXO 148.5 MHz til OCXO 10 MHz)
component digital_PLL is
    Port ( --rst_i : in STD_LOGIC;
			  ref_clk_i : in  STD_LOGIC;
           clk_i : in  STD_LOGIC;
			  pps_i : in STD_LOGIC;
           cv_word_o : out  STD_LOGIC_VECTOR (15 downto 0);
			  update_dac_o : out STD_LOGIC;
			  int_clk_o : out STD_LOGIC;
			  ref_clk_o : out STD_LOGIC;
			  pps_148_tick_o : out STD_LOGIC);
end component;


-- DCM (OCXO 10 MHz => 100 MHz)
component OCXO_clk is
   port ( CLKIN_IN        : in    std_logic; 
          RST_IN          : in    std_logic; 
          CLKFX_OUT       : out   std_logic; 
          LOCKED_OUT      : out   std_logic
			 );
end component;


-- Puls generator. Pulsgenerering til NTSC/PAL osv
component pulse_gen is
    Port ( clk_i : in  STD_LOGIC;
           rst_i : in  STD_LOGIC;
           per_hi_i : in integer;
           per_lo_i : in integer;
           per_offset_i : in integer;
           pulse_o : out  STD_LOGIC);
end component;


-- LTC generator
component LTC_generator is
    Port ( hours_i	:	in STD_LOGIC_VECTOR (7 downto 0);
			  mins_i	: 	in STD_LOGIC_VECTOR (7 downto 0);
			  secs_i	:	in STD_LOGIC_VECTOR (7 downto 0);
			  frames_i : in STD_LOGIC_VECTOR (7 downto 0);
			  delay_i : in STD_LOGIC_VECTOR (31 downto 0);
			  dropframe_i : in STD_LOGIC;
			  format_select_i	:	in STD_LOGIC_VECTOR (1 downto 0);
           sync_i : in  STD_LOGIC;
           clk_i : in  STD_LOGIC;	-- 148.5 MHz master clock
			  debug_o : out STD_LOGIC;
           ltc_code_o : out  STD_LOGIC
			  );
end component;


-- Seriel I2C interface
component I2C_engine is
    Port ( reset_i 			: in  STD_LOGIC;
           clk_i 				: in  STD_LOGIC;
           SCL_i 				: in  STD_LOGIC;
           SDA_io 			: inout  STD_LOGIC;
           send_byte_i 		: in  STD_LOGIC_VECTOR (7 downto 0);
           recieved_byte_o : out	STD_LOGIC_VECTOR (7 downto 0);
			  address_i			: in 	STD_LOGIC_VECTOR (6 downto 0);
			  update_flag_o	: out STD_LOGIC;
			  start_o			: out STD_LOGIC;
			  stop_o				: out STD_LOGIC;
			  debug_o			: out STD_LOGIC
			  );
end component;


-- Styring af indkommende data
component com_control is
	port (
		clk_i : in std_logic;
		rst_i : in std_logic;
		I2C_start_i	: in std_logic;
		I2C_update_i	: in std_logic;
		I2C_stop_i	: in std_logic;
		I2C_byte_i : in std_logic_vector (7 downto 0);
		address_o : out std_logic_vector (7 downto 0);
		update_register_o : out std_logic
	);
end component;


--register signaler
signal register_address : std_logic_vector(7 downto 0);
signal update_register : std_logic;

-- RAM
type register_ram is array (28 downto 0) of std_logic_vector(7 downto 0);
signal genlock_register : register_ram;

----------------------------------------------------------------------------
-- Hoved struktur
----------------------------------------------------------------------------

begin
	-- buffer for differential clock
	diff_buf_clk : IBUFGDS
		generic map (IOSTANDARD => "LVDS_25")
		port map (
			O  => clk_148,
			I  => clk_148_p,
			IB => clk_148_n
			);

	-- DAC controller for OCXO
	dac_OCXO : ad5660_serial_out port map (
      clk_148_i => clk_100,
      data_i => OCXO_cv_word,
      new_data_i => OCXO_update_dac,                    
      sclk_o => sclk_10mhz_o,
      dout_o => din_10mhz_o,
      zsync_o => sync_10mhz_o
   );
	
	-- PLL for OCXO
	OCXO_pll : GPS_PLL port map (
		GPS_PPS_i => kvant_pps, 
      clk_i => clk_100,
		rst_i => OCXO_reset,
		fix_i => OCXO_loop_fix,  -- lav hvis GPS forbindelse (fix hvis ingen)
		OCXO_cv_value_i => OCXO_extern_value,
		OCXO_loop_control_i => OCXO_loop_control,
		phase_diff_o => OCXO_phase_diff,
      cv_word_o => OCXO_cv_word,
		update_dac_o => OCXO_update_dac,
		int_PPS_tick_o => int_pps_tick_100mhz,
	   int_PPS_o => int_pps);

	-- AD5660 DAC
	Inst_ad5660_serial_DAC: ad5660_serial_DAC PORT MAP(
		clk_i => clk_148,
		decimation_i => "0010",
		word_i => VCXO_cv_word,
		update_i => VCXO_update_dac,
		sclk_o => clk_int,
		data_o => data_int,
		sync_o => sync_int
	);

	sclk_148mhz <= clk_int;
	din_148mhz <= data_int;
	sync_148mhz <= sync_int;
	
	-- PLL for VCXO
	VCXO_pll : digital_PLL port map (
		--rst_i => rst_i,
		ref_clk_i => clk_100,
		clk_i => clk_148,
		pps_i => int_pps_tick_100mhz,
		cv_word_o => VCXO_cv_word,
		update_dac_o => VCXO_update_dac,
		int_clk_o => int_clk250k,
		ref_clk_o => ref_clk250k,
		pps_148_tick_o => pps_148_tick
		);

	-- DCM til generering af 100 MHz clock til OCXO PLL
	PLL_clock_gen : OCXO_clk port map (
		CLKIN_IN => clk_10_i,
		RST_IN => '0',
		CLKFX_OUT => clk_100
		--LOCKED_OUT => 
		);
		
	-- laver pulser med PAL=15625 Hz (625 * 25) eller NTSC = 15750/1.001 (525*30)/1.001, udfra 148.5 MHz
	h_gen: pulse_gen  port map ( 
	  clk_i => clk_148,
	  rst_i => SPG_sync_signal,
	  per_hi_i => hor_hi_time,
	  per_lo_i => hor_lo_time,
	  per_offset_i => hor_offset,
	  pulse_o => h_d_o
	);
	
	-- laver pulser med PAL=25 Hz eller NTSC = 30/1.001 Hz ved 148.5 MHz 
	f_gen: pulse_gen  port map ( 
	  clk_i => clk_148,
	  rst_i => SPG_sync_signal,
	  per_hi_i => field_hi_time,
	  per_lo_i => field_lo_time,
	  per_offset_i => field_offset,
	  pulse_o => f_d
	);
		
	-- I2C serielt interface
	serial_interface : I2C_engine port map (
		reset_i => '0',
		clk_i => clk_148,
		SCL_i => SCL_i,
		SDA_io => SDA_io,
		send_byte_i => I2C_byte_out,
		recieved_byte_o => I2C_byte_in,
		address_i => "1111000",
		update_flag_o => I2C_update,
		start_o => I2C_start,
		stop_o => I2C_stop,
		debug_o => I2C_debug
		);
		
	-- LTC generator A
	LTC_A: LTC_generator port map(
      hours_i => LTC_a_hours,
		mins_i => LTC_a_mins,
		secs_i => LTC_a_secs,
		frames_i => LTC_a_frames,
		delay_i => LTC_a_delay,
		dropframe_i => LTC_a_dropframe,
		format_select_i => LTC_a_format,
      sync_i => LTC_a_sync,
      clk_i => clk_148,
		debug_o => LTC_a_debug,
      ltc_code_o => LTC_a_code
		);
		
	-- LTC generator B
	LTC_B: LTC_generator port map(
      hours_i => LTC_b_hours,
		mins_i => LTC_b_mins,
		secs_i => LTC_b_secs,
		frames_i => LTC_b_frames,
		delay_i => LTC_b_delay,
		dropframe_i => LTC_b_dropframe,
		format_select_i => LTC_b_format,
      sync_i => LTC_b_sync,
      clk_i => clk_148,
		debug_o => LTC_b_debug,
      ltc_code_o => LTC_b_code
		);
			
	-- Kommunikations kontrol
	communication : com_control port map (
		clk_i => clk_148,
		rst_i => '0',
		I2C_start_i	=> I2C_start,
		I2C_update_i => I2C_update,
		I2C_stop_i => I2C_stop,
		I2C_byte_i => I2C_byte_in,
		address_o => register_address,
		update_register_o => update_register
	);
	
	-- PPS detekt. Højt signal hvis puls indenfor 1 sekund
	pps_detect : process (clk_10_i)
	begin
		if clk_10_i'event and clk_10_i = '1' then
			if rst_i = '1' then
				pps_status_count <= 0;
				PPS_status <= '0';
			elsif PPS_i='1' then
				pps_status_count <= 0;
				PPS_status <= '1';
			else
				pps_status_count <= pps_status_count + 1;
				
				-- hvis timeout, lavt signal
				if pps_status_count > 10500000 then
					PPS_status <= '0';
				end if;
			end if;
		end if;
	end process;
	
	pps_100_generate : process (clk_100)
	begin
		if clk_100'event and clk_100 = '1' then
			-- kant detektion
			pps_edge <= pps_edge(0) & pps_i;
			
			-- GPS 1 pps
			if pps_edge = "01" then
				kvant_pps <= '1';
			else
				kvant_pps <= '0';
			end if;
		end if;
	end process;
	
	-- millisekund clock, laver ticks på 1 KHz ved 148.5 MHz og tæller fra 0 til 999 ved 1 KHz
	ms_clock_generator : process (clk_148)
	begin
		if clk_148'event and clk_148 = '1' then
			if pps_148_tick = '1' then
				ms_count <= 0;
				ms_dec_count <= 0;
				ms_tick <= '1';
			elsif ms_dec_count = 148499 then
				ms_count <= ms_count + 1;
				ms_dec_count <= 0;
				ms_tick <= '1';
			else
				ms_dec_count <= ms_dec_count + 1;
				ms_tick <= '0';
			end if;
		end if;
	end process;
	
	-- NTSC/PAL select MUX
	with format_select select
		hor_hi_time <= PAL_h_hi_time when '0',
						NTSC_h_hi_time when others;
	
	with format_select select
		hor_lo_time <= PAL_h_lo_time when '0',
						NTSC_h_lo_time when others;
	
	with format_select select
		hor_offset <= PAL_h_offset when '0',
						NTSC_h_offset when others;
						
	with format_select select
		field_hi_time <= PAL_f_hi_time when '0',
						NTSC_f_hi_time when others;
	
	with format_select select
		field_lo_time <= PAL_f_lo_time when '0',
						NTSC_f_lo_time when others;
	
	with format_select select
		field_offset <= PAL_f_offset when '0',
						NTSC_f_offset when others;
												
	process(clk_148)
	begin
		if clk_148'event and clk_148='1' then
			if format_select = '0' then
				SPG_sync_signal <= (PAL_sync and PAL_sync_sec );
			else
				SPG_sync_signal <= NTSC_sync;
			end if;
		end if;
	end process;

	LTC_a_out <= LTC_a_code;
	LTC_b_out <= LTC_b_code;
	
	LTC_a_sync <= pps_148_tick;
	LTC_b_sync <= pps_148_tick;
	
	-- generer sync, der opdatere offset til F og H SPG (PAL/NTSC)
	SPG_sync_handler : process(clk_148)
	begin
		if clk_148'event and clk_148='1' then
			-- PAL = 1 PPS, tilpasset GPS
			PAL_sync <= pps_148_tick;

			-- NTSC = tilpasset 1/1,001 sekunder
			if ms_tick = '1' and (ms_count = conv_integer(NTSC_offset)) then
				NTSC_sync <= '1';
			else
				NTSC_sync <= '0';
			end if;
		end if;
	end process;
	
	-- Indlæser I2C data i hoved registeret (benytter com_control til håndtering af I2C-data)
	register_handler: process(clk_148)
	begin
		if clk_148'event and clk_148='1' then
			if update_register = '1' then
				genlock_register(conv_integer(register_address)) <= i2c_byte_in;
			end if;
		end if;
	end process;
					
	-- output multiplexer
	with register_address select
				i2c_byte_out <= 	genlock_register(0) 			when "00000000",
										genlock_register(1) 			when "00000001",
										genlock_register(2) 			when "00000010",
										genlock_register(3) 			when "00000011",
										genlock_register(4) 			when "00000100",
										genlock_register(5) 			when "00000101",
										genlock_register(6) 			when "00000110",
										genlock_register(7) 			when "00000111",
										genlock_register(8) 			when "00001000",
										genlock_register(9) 			when "00001001",
										genlock_register(10) 		when "00001010",
										genlock_register(11) 		when "00001011",
										genlock_register(12) 		when "00001100",
										genlock_register(13) 		when "00001101",
										genlock_register(14) 		when "00001110",
										genlock_register(15) 		when "00001111",
										genlock_register(16) 		when "00010000",
										genlock_register(17) 		when "00010001",
										genlock_register(18) 		when "00010010",
										genlock_register(19) 		when "00010011",
										genlock_register(20) 		when "00010100",
										genlock_register(21)			when "00010101",
										genlock_register(22)			when "00010110",
										software_version 				when "00100000",
										OCXO_cv_word (7 downto 0) 	when "00100001",
										OCXO_cv_word (15 downto 8) when "00100010",
										OCXO_phase_diff(7 downto 0)when "00100011",
										OCXO_phase_diff(15 downto 8)	when "00100100",
										"10101010" 						when others;
	
	-- quant error må ikke clockes, da data skal være tilgængelig tidligere
	quant_error <= genlock_register(0);
	
	-- OCXO startværdier
	OCXO_extern_value(7 downto 0) <= genlock_register(21);
	OCXO_extern_value(15 downto 8) <= genlock_register(22);
	
	-- Map register ud til interne signaler (register består af 8 bits blokke)	
	-- Opdateres hvert sekund	
	register_mapout : process(clk_148)
	begin
		if clk_148'event and clk_148='1' then
			if pps_148_tick = '1' then
				-- NTSC offset register
				NTSC_offset(7 downto 0) <= genlock_register(1);
				NTSC_offset(15 downto 8) <= genlock_register(2);
				-- LTC a tider
				LTC_a_hours <= genlock_register(3);
				LTC_a_mins <= genlock_register(4);
				LTC_a_secs <= genlock_register(5);
				LTC_a_frames <= genlock_register(6);
				-- LTC a delay
				LTC_a_delay(7 downto 0) <= genlock_register(7);
				LTC_a_delay(15 downto 8) <= genlock_register(8);
				LTC_a_delay(23 downto 16) <= genlock_register(9);
				LTC_a_delay(31 downto 24) <= genlock_register(10);
				-- LTC b tider
				LTC_b_hours <= genlock_register(11);
				LTC_b_mins <= genlock_register(12);
				LTC_b_secs <= genlock_register(13);
				LTC_b_frames <= genlock_register(14);
				-- LTC b delay
				LTC_b_delay(7 downto 0) <= genlock_register(15);
				LTC_b_delay(15 downto 8) <= genlock_register(16);
				LTC_b_delay(23 downto 16) <= genlock_register(17);
				LTC_b_delay(31 downto 24) <= genlock_register(18);
				-- LTC a+b format
				LTC_setup <= genlock_register(19);
				-- System parametre
				system_control <= genlock_register(20);
				
			end if;
		end if;
	end process;
	
	-- udmapning af pakkede bytes
	-- valg af genlock format NTSC/PAL til mainframe
	format_select <= system_control(0);
	-- flag om der skal synces for PAL i pågældne sekund
	PAL_sync_sec <= system_control(1);
	-- OCXO reset
	OCXO_reset <= system_control(2);
	-- OCXO loop control
	OCXO_loop_control <= system_control(3);
	-- OCXO PLL fix control
	OCXO_loop_fix <= system_control(4);
	
	-- LTC setup
	LTC_a_format <= LTC_setup(1 downto 0);
	LTC_a_dropframe <= LTC_setup(2);
	
	LTC_b_format <= LTC_setup(5 downto 4);
	LTC_b_dropframe <= LTC_setup(6);
	
	f_d_o <= f_d;
		
	-- LED output
	led1_o <= not OCXO_loop_control;
	led2_o <= PPS_status;
	
	-- 1 PPS interrupt til MCU
	ext_int_o <= int_pps;
	
	-- status flag til MCU
	PPS_status_o <= PPS_status;
	
	NTP_RX <= UBX_TX_i;
	MCU_RX <= UBX_TX_i;
	
	-- for NTP, forlænger 1 pps til 1 ms høj
	longer_tick_gen : process (clk_148)
	begin
		if clk_148'event and clk_148='1' then	
			if pps_148_tick = '1' then
				count <= 1484999;
				longer_tick <= '1';
			elsif count = 0 then
				longer_tick <= '0';
			else
				count <= count-1;
			end if;
		end if;
	end process;
	
	NTP_PPS <= longer_tick;
	
--	TP1_o <= int_pps_tick_100mhz;
--	TP2_o <= pps_148_tick;
--	TP3_o <= VCXO_update_dac;

--	TP1_o <= int_clk250k;
--	TP2_o <= '0';
--	TP3_o <= ref_clk250k;


	TP1_o <= int_pps_tick_100mhz;
	TP2_o <= '0';
	TP3_o <= PPS_i;
	
end Behavioral;

