library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_SIGNED.ALL;

entity digital_phase_detect is
    Port ( clk_i : in  STD_LOGIC;
           ref_clk_tick_i : in  STD_LOGIC;
			  int_clk_o : out STD_LOGIC;
           phase_diff_o : out  STD_LOGIC_VECTOR (12 downto 0);
           update_tick_o : out  STD_LOGIC
			  );
end digital_phase_detect;

architecture Behavioral of digital_phase_detect is

--detektere flanker på intern clock og GPS pps
signal int_clk_tick : std_logic;
signal update_tick : std_logic;

--signaler til fase komperator
signal phase_counter : std_logic_vector (11 downto 0);
signal phase_diff : std_logic_vector (12 downto 0);

begin
	-- fase tæller (148.5 MHz => 62,5 KHz)
	phase_count : process (clk_i)
	begin
		if clk_i'event and clk_i='1' then
			if phase_counter = conv_std_logic_vector(-1188, 12) then
				phase_counter <= conv_std_logic_vector(1187, 12);
				update_tick <= '1';
			else
				phase_counter <= phase_counter - 1;
				update_tick <= '0';
			end if;
			
			--lav tick ved hver periode
			if phase_counter = conv_std_logic_vector(0, 12) then
				int_clk_tick <= '1';
			else
				int_clk_tick <= '0';
			end if;
		end if;
	end process;
	
	-- latch faseforskel, ved tick
	phase_comp : process (clk_i)
	begin
		if clk_i'event and clk_i='1' then
			if ref_clk_tick_i = '1' then
				phase_diff <= (phase_counter & '1');
			end if;
		end if;
	end process;

	-- outputs
	phase_diff_o <= phase_diff;
	int_clk_o <= phase_counter(11);
	update_tick_o <= update_tick;
	
end Behavioral;
