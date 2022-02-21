----------------------------------------------------------------------------------
-- Company: DK-technologies
-- Engineer: Jens K Hansen
-- 
-- Create Date:    13:09:21 07/12/2007 
-- Design Name: ref-PLL
-- Module Name:    ref_PLL - Behavioral 
-- Project Name: ref Genlock modul	
-- Target Devices: PT8616
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
    Port ( --rst_i : in STD_LOGIC;
			  ref_clk_i : in  STD_LOGIC;
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

component digital_pll_phase_detector is
   generic (
      vcxo_div_per            : integer         -- decimation of clk_i to pll
   );
   port (
      clk_vcxo_i              : in std_logic;
      clk_vcxo_clean_i        : in std_logic;
      ref_decimated_i         : in std_logic;
      vcxo_dec_falling_tick_o : out std_logic;  -- tick at falling edge of decimated vcxo to phase comparator
      clk_vcxo_decimated_o    : out std_logic;
      phase_diff_o            : out std_logic_vector(12 downto 0);
      lock_warning_o          : out std_logic;
      lock_error_o            : out std_logic
   );
end component;

component digital_phase_loop is
--component digital_phase_loop_wmults is
    Port ( --rst_i : in STD_LOGIC;
			  clk_i : in STD_LOGIC;
			  phase_diff_i : in  STD_LOGIC_VECTOR (12 downto 0);
           update_tick_i : in  STD_LOGIC;
           dac_word_o : out  STD_LOGIC_VECTOR (15 downto 0)
			  );
end component;

component digital_phase_detect is
    Port ( clk_i : in  STD_LOGIC;
           ref_clk_tick_i : in  STD_LOGIC;
			  int_clk_o : out STD_LOGIC;
           phase_diff_o : out  STD_LOGIC_VECTOR (11 downto 0);
           update_tick_o : out  STD_LOGIC
			  );
end component;

--main signaler
signal int_clk : std_logic;

--signaler til fase loop
signal phase_diff : std_logic_vector (12 downto 0);
signal update_tick : std_logic;

--signal til downclockning af ref clock (100 MHz => 62500 Hz)
signal ref_clk_count : integer range 0 to 1600;
signal ref_clk : std_logic;
signal ref_clk_edge : std_logic_vector(1 downto 0);
signal trigger : std_logic;
signal tick_delay : integer range 0 to 149000000;

signal cv_word : std_logic_vector(15 downto 0);

begin

	phase_detector : digital_pll_phase_detector
   generic map(
      vcxo_div_per            => 2376
   )
   port map(
      clk_vcxo_i              => clk_i,
      clk_vcxo_clean_i        => clk_i,
      ref_decimated_i         => ref_clk,
      vcxo_dec_falling_tick_o => update_tick,
      clk_vcxo_decimated_o    => int_clk,
      phase_diff_o            => phase_diff,
      lock_warning_o          => open,
      lock_error_o            => open
   );


	--faseloop
	VCXO_phase_loop : digital_phase_loop port map (
		--rst_i => rst_i,
		clk_i => clk_i,
		phase_diff_i => phase_diff,
		update_tick_i => update_tick,
		dac_word_o => cv_word
		);

process (clk_i)
	begin
		if clk_i'event and clk_i='1' then
			cv_word_o <= cv_word;
			update_dac_o <= update_tick;
					
			if ref_clk_edge = "01" then
				trigger <= '1';
				tick_delay <= tick_delay - 1;
				
			elsif trigger = '1' and update_tick = '1' then
				trigger <= '0';
				tick_delay <= 148499999;
				
			else
				tick_delay <= tick_delay - 1;
			end if;

			if tick_delay = 596 then
				pps_148_tick_o <= '1';
			else
				pps_148_tick_o <= '0';
			end if;
		end if;
	end process;
	
	--genererer clock til reference (100 MHz -> 62.5 KHz)
	ref_clk_tick_gen : process(ref_clk_i)
	begin
		if ref_clk_i'event and ref_clk_i='1' then	
			ref_clk_edge <= ref_clk_edge(0) & pps_i;
			
			-- generer reference clock 
			if ref_clk_edge = "01" then
				ref_clk <= '0';
				ref_clk_count <= 400;
			elsif ref_clk_count = 799 then
				ref_clk <= '1';
				ref_clk_count <= ref_clk_count + 1;
			elsif ref_clk_count = 1599 then
				ref_clk <= '0';
				ref_clk_count <= 0;
			else
				ref_clk_count <= ref_clk_count + 1;
			end if;
		end if;
	end process;
	
	int_clk_o <= int_clk;
	ref_clk_o <= ref_clk;
	 
end Behavioral;

