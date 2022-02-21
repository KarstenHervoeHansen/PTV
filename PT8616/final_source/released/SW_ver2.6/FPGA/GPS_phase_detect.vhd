library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_SIGNED.ALL;

entity GPS_phase_detect is
    Port ( clk_i : in  STD_LOGIC;
			  rst_i : in STD_LOGIC;
           GPS_pps_i : in  STD_LOGIC;
			  int_pps_tick_o : out STD_LOGIC;
			  int_pps_o : out STD_LOGIC;
           phase_diff_o : out  STD_LOGIC_VECTOR (27 downto 0);
           update_tick_o : out  STD_LOGIC
			  );
end GPS_phase_detect;

architecture Behavioral of GPS_phase_detect is

--detektere flanker på intern clock og GPS pps
signal gps_edge_tick : std_logic;
signal int_pps_tick : std_logic;
signal update_tick : std_logic;

--signaler til fase komperator
signal phase_counter : std_logic_vector (26 downto 0);
signal phase_diff : std_logic_vector (27 downto 0);

begin
	gps_edge_tick <= GPS_pps_i;
	
	phase_count : process (clk_i)
	begin
		if clk_i'event and clk_i='1' then
			if rst_i = '1' and gps_edge_tick = '1' then
				--reset fasecounter på references opadgående flanke
				phase_counter <= conv_std_logic_vector(0, 27);
			else
				if phase_counter = conv_std_logic_vector(-50000000, 27) then
					phase_counter <= conv_std_logic_vector(49999999, 27);
					update_tick <= '1';
				else
					phase_counter <= phase_counter - 1;
					update_tick <= '0';
				end if;
				
				if phase_counter = conv_std_logic_vector(0, 27) then
					int_pps_tick <= '1';
				else
					int_pps_tick <= '0';
				end if;
			end if;
		end if;
	end process;
	
	phase_comp : process (clk_i)
	begin
		if clk_i'event and clk_i='1' then
			if gps_edge_tick = '1' then
				phase_diff <= (phase_counter & '0'); -- & '1' til sidst
			end if;
		end if;
	end process;

	phase_diff_o <= phase_diff;

	int_pps_tick_o <= int_pps_tick;
	int_pps_o <= phase_counter(26);
	update_tick_o <= update_tick;
end Behavioral;

