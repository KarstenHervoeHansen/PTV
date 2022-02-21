library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_SIGNED.ALL;

entity digital_phase_loop_wmults is
    Port ( --rst_i : in STD_LOGIC;
			  clk_i : in STD_LOGIC;
			  phase_diff_i : in  STD_LOGIC_VECTOR (9 downto 0);
			  update_tick_i : in  STD_LOGIC;
			  P_gain : in std_logic_vector(17 downto 0);
			  I_gain : in std_logic_vector(17 downto 0);
			  dac_word_o : out  STD_LOGIC_VECTOR (15 downto 0)
			  );
end digital_phase_loop_wmults;

architecture Behavioral of digital_phase_loop_wmults is

signal integrator : std_logic_vector (27 downto 0):= (others => '0');

component PLL_gain
	port (
	--clk: in std_logic;
	a: in std_logic_vector(9 downto 0);
	b: in std_logic_vector(17 downto 0);
	p: out std_logic_vector(27 downto 0));
end component;

-- Synplicity black box declaration
attribute syn_black_box : boolean;
attribute syn_black_box of PLL_gain: component is true;

signal P_gain_out : std_logic_vector(27 downto 0);
signal I_gain_out : std_logic_vector(27 downto 0);

begin
			
	P_gainer : PLL_gain
	port map (
		--clk => clk_i,
		a => phase_diff_i,
		b => P_gain,
		p => P_gain_out
	);
				
	I_gainer : PLL_gain
	port map (
		--clk => clk_i,
		a => phase_diff_i,
		b => I_gain,
		p => I_gain_out
	);
	
	-- integrator
	integrator_update : process(clk_i)
	begin
		if clk_i'event and clk_i = '1' then
			if update_tick_i = '1' then								
				--integrator
				integrator <= integrator + I_gain_out;	
			end if;
			
			--proportional del
			dac_word_o <= integrator(27 downto 12) + P_gain_out(15 downto 0);
		end if;
	end process;
	
end Behavioral;

