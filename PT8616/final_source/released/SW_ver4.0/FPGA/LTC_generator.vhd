-----------------------------------------------------------
--				 LTC generator
-----------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity LTC_generator is
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
end LTC_generator;

architecture Behavioral of LTC_generator is

--main signals
signal ltc_sync : STD_LOGIC;

--delay signals
signal delay_sync : STD_LOGIC;
signal delay_count_high : std_logic_vector (3 downto 0);
signal delay_count_low : std_logic_vector (23 downto 0);

--modulation clock signals
signal ltc_clock_tick : STD_LOGIC;
signal ltc_frame_tick : STD_LOGIC;
signal ltc_bitcount : STD_LOGIC_VECTOR(7 downto 0);

--biphase modulated frame 
signal ltc_frame 		: STD_LOGIC_VECTOR (79 downto 0);
signal biphase_code	: STD_LOGIC;

-- udregning af fasekorrektionsbit
--signal phase_corr_bit	: STD_LOGIC;
--signal counter 			: integer range 0 to 80;
--signal calculate			: STD_LOGIC;

constant sync_word		: STD_LOGIC_VECTOR (15 downto 0):="0011111111111101";
constant BG					: STD_LOGIC_VECTOR (3 downto 0):="0000";

signal hour_tens : STD_LOGIC_VECTOR(1 downto 0);
signal hour_units : STD_LOGIC_VECTOR(3 downto 0);
signal min_tens : STD_LOGIC_VECTOR(2 downto 0);
signal min_units : STD_LOGIC_VECTOR(3 downto 0);
signal sec_tens : STD_LOGIC_VECTOR(2 downto 0);
signal sec_units : STD_LOGIC_VECTOR(3 downto 0);
signal frame_tens : STD_LOGIC_VECTOR(1 downto 0);
signal frame_units : STD_LOGIC_VECTOR(3 downto 0);

-- komponent til clock-generering
component LTC_clockgen is
   Port ( 
		  clk_i : in  STD_LOGIC;
		  sync_i : in  STD_LOGIC;
		  format_i : in  STD_LOGIC_VECTOR (1 downto 0);
		  ltc_clock_tick_o : out  STD_LOGIC;
		  ltc_frame_tick_o : out STD_LOGIC;
		  ltc_sync_o : out STD_LOGIC;
		  ltc_bitcount_o : out STD_LOGIC_VECTOR(7 downto 0));
	end component;

-- komponent til at tælle frames
component LTC_frame_counter is
   Port ( 	
			-- inputs
			clk_i : in  STD_LOGIC;
			ltc_frame_tick_i : in STD_LOGIC;
			ltc_sync_i : in STD_LOGIC;
			dropframe_i : in STD_LOGIC;
			format_i : in STD_LOGIC_VECTOR (1 downto 0);
			hours_i : in STD_LOGIC_VECTOR(7 downto 0);
			mins_i : in STD_LOGIC_VECTOR(7 downto 0);
			secs_i : in STD_LOGIC_VECTOR(7 downto 0);
			frames_i : in STD_LOGIC_VECTOR(7 downto 0);
			--outputs
			hour_tens_o : out STD_LOGIC_VECTOR(1 downto 0);
			hour_units_o : out STD_LOGIC_VECTOR(3 downto 0);
			min_tens_o : out STD_LOGIC_VECTOR(2 downto 0);
			min_units_o : out STD_LOGIC_VECTOR(3 downto 0);
			sec_tens_o : out STD_LOGIC_VECTOR(2 downto 0);
			sec_units_o : out STD_LOGIC_VECTOR(3 downto 0);
			frame_tens_o : out STD_LOGIC_VECTOR(1 downto 0);
			frame_units_o : out STD_LOGIC_VECTOR(3 downto 0)
	 );	
	end component;
	
component ltc_biphase_gen is
   Port ( 	
			clk_i : in  STD_LOGIC;
			ltc_clock_tick_i : in  STD_LOGIC;
			ltc_bitcount_i : in STD_LOGIC_VECTOR(7 downto 0);
			ltc_frame_i : in  STD_LOGIC_VECTOR(79 downto 0);
			biphase_o : out  STD_LOGIC);
	end component;

begin
	LTC_clockgenerator : LTC_clockgen port map (
		clk_i => clk_i,
      sync_i => delay_sync,
      format_i => format_select_i,
      ltc_clock_tick_o => ltc_clock_tick,
		ltc_frame_tick_o => ltc_frame_tick,
		ltc_sync_o => ltc_sync,
		ltc_bitcount_o => ltc_bitcount
	);
		
	LTC_frame_count : LTC_frame_counter port map (
		clk_i => clk_i,
		ltc_frame_tick_i => ltc_frame_tick,
		ltc_sync_i => ltc_sync,
		dropframe_i => dropframe_i,
		format_i => format_select_i,
		hours_i => hours_i,
		mins_i => mins_i,
		secs_i => secs_i, 
		frames_i => frames_i, 
		hour_tens_o => hour_tens,
		hour_units_o => hour_units,
		min_tens_o => min_tens,
		min_units_o => min_units, 
		sec_tens_o => sec_tens,
		sec_units_o => sec_units,
		frame_tens_o => frame_tens,
		frame_units_o => frame_units
	);	
	 
	LTC_biphase_generator : ltc_biphase_gen port map (
		clk_i => clk_i,
		ltc_clock_tick_i => ltc_clock_tick,
		ltc_bitcount_i => ltc_bitcount,
		ltc_frame_i => ltc_frame,
		biphase_o => biphase_code
	);
	

	-- generer sync, efter (delay_i) clock cycles
	delay_sync_gen : process (clk_i)
	begin
		if clk_i'event and clk_i = '1' then
			if sync_i = '1' then
				delay_count_low <= (others=>'0');
				delay_count_high <= (others=>'0');
			elsif delay_count_low = "111111111111111111111111" then
				delay_count_low <= (others => '0');
				delay_count_high <= delay_count_high + 1;
			else
				delay_count_low <= delay_count_low + 1;
			end if;
			
			if (delay_count_high & delay_count_low) = delay_i then
				delay_sync <= '1';
			else
				delay_sync <= '0';
			end if;
		end if;
	end process;
				
	LTC_frame <= frame_units & BG & frame_tens & dropframe_i & '0' & BG & sec_units & BG & sec_tens & "0" & BG & min_units & BG & min_tens & "0" & BG & hour_units & BG & hour_tens & "00" & BG & sync_word;

	debug_o <= delay_sync;	
	ltc_code_o <= biphase_code;
end Behavioral;

