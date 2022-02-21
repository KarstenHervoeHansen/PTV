----------------------------------------------------------------------------------
-- Company: DK-technologies
-- Engineer: Jens K Hansen
-- 
-- Create Date:    13:09:21 07/12/2007 
-- Design Name: ref-PLL
-- Module Name:    ref_PLL - Behavioral 
-- Project Name: ref Genlock modul	
-- Target Devices: PT8616
-- Description: PLL til 1 clk og 10 mhz OCXO - benytter kvantiseringsfejl fra ref
--						til at opnå højere præcision på reference 1 clk'en
--
--
-- Revision: 
-- Revision 0.01 - File Created
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_SIGNED.ALL;

entity digital_PLL is
    Port ( ref_clk_i : in  STD_LOGIC;
           clk_i : in  STD_LOGIC;
			  pps_i : in STD_LOGIC;
           cv_word_o : out  STD_LOGIC_VECTOR (15 downto 0);
			  update_dac_o : out STD_LOGIC;
			  int_clk_o : out STD_LOGIC;
			  ref_clk_o : out STD_LOGIC;
			  pps_148_tick_o : out STD_LOGIC
			  );
end digital_PLL;

architecture Behavioral of digital_PLL is

--component digital_pll_phase_detector is
--   generic (
--      vcxo_div_per            : integer         -- decimation of clk_i to pll
--   );
--   port (
--      clk_vcxo_i              : in std_logic;
--      clk_vcxo_clean_i        : in std_logic;
--      ref_decimated_i         : in std_logic;
--      vcxo_dec_falling_tick_o : out std_logic;  -- tick at falling edge of decimated vcxo to phase comparator
--      clk_vcxo_decimated_o    : out std_logic;
--      phase_diff_o            : out std_logic_vector(12 downto 0);
--      lock_warning_o          : out std_logic;
--      lock_error_o            : out std_logic
--   );
--end component;

component digital_phase_detect is
    Port ( clk_i : in  STD_LOGIC;
           ref_clk_tick_i : in  STD_LOGIC;
			  int_clk_o : out STD_LOGIC;
           phase_diff_o : out  STD_LOGIC_VECTOR (12 downto 0);
           update_tick_o : out  STD_LOGIC
			  );
end component;

component digital_phase_loop is
    Port ( clk_i : in STD_LOGIC;
			  phase_diff_i : in  STD_LOGIC_VECTOR (12 downto 0);
           update_tick_i : in  STD_LOGIC;
           dac_word_o : out  STD_LOGIC_VECTOR (15 downto 0)
			  );
end component;

--main signaler
signal int_clk : std_logic;
signal resync_148_period : std_logic;

--signaler til fase loop
signal phase_diff : std_logic_vector (12 downto 0);
signal update_tick : std_logic;

--signal til downclockning af ref clock (100 MHz => 625000 Hz)
signal ref_clk_count : integer range 0 to 1600;--800;
signal ref_clk : std_logic;

begin
--	phase_detector : digital_pll_phase_detector
--   generic map(
--      vcxo_div_per            => 2376
--   )
--   port map(
--      clk_vcxo_i              => clk_i,
--      clk_vcxo_clean_i        => clk_i,
--      ref_decimated_i         => ref_clk,        
--      vcxo_dec_falling_tick_o => update_tick,
--      clk_vcxo_decimated_o    => int_clk,
--      phase_diff_o            => phase_diff
--      --lock_warning_o          => lock_warning_o,
--      --lock_error_o            => lock_error_o  
--   );
	
	phase_detector : digital_phase_detect 
    Port map ( clk_i => clk_i,
					ref_clk_tick_i => ref_clk,
					int_clk_o => int_clk,
					phase_diff_o => phase_diff,
					update_tick_o => update_tick
			  );
		
	--faseloop
	VCXO_phase_loop : digital_phase_loop port map (
		clk_i => clk_i,
		phase_diff_i => phase_diff,
		update_tick_i => update_tick,
		dac_word_o => cv_word_o
		);
	
	--genererer clock til reference (100 MHz -> 62.5 KHz)
	ref_clk_gen : process(ref_clk_i)
	begin
		if ref_clk_i'event and ref_clk_i='1' then
			-- hvis intern pps, reset reference clock
			if pps_i = '1' then
				ref_clk <= '1';
				ref_clk_count <= 0;
				
			-- generer reference clock 
			elsif ref_clk_count = 1599 then --799 then
				ref_clk <= '1'; --not ref_clk;
				ref_clk_count <= 0;
			else
				ref_clk <= '0';
				ref_clk_count <= ref_clk_count + 1;
			end if;
		end if;
	end process;
	
	-- generer periode hvori 148 Mhz 1 pps genereres
	resync_period_gen : process(ref_clk_i)
	begin
		if ref_clk_i'event and ref_clk_i='1' then
			--hvis intern 1 pps, sæt ny periode
			if pps_i = '1' then
				resync_148_period <= '1';
			--reset periode efter én 65200 Hz period
			elsif ref_clk_count = 1599 then --799 and ref_clk = '0' then
				resync_148_period <= '0';
			end if;
		end if;
	end process;
	
	int_clk_o <= int_clk;
	ref_clk_o <= ref_clk;
	update_dac_o <= update_tick;

	--update tick = nedadgående flanke på VCXO clk (62.5 KHz)
	pps_148_tick_o <= resync_148_period and update_tick; 
end Behavioral;

