----------------------------------------------------------------------------------
-- Company: DK-technologies
-- Engineer: Jens K Hansen
-- 
-- Create Date:    13:09:21 07/12/2007 
-- Design Name: GPS-PLL
-- Module Name:    GPS_PLL - Behavioral 
-- Project Name: GPS Genlock modul	
-- Target Devices: PT8616
-- Description: PLL til 1 PPS og 10 mhz OCXO - benytter kvantiseringsfejl fra GPS
--						til at opnå højere præcision på reference 1 PPS'en
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

entity GPS_PLL is
    Port ( GPS_PPS_i : in  STD_LOGIC;
           clk_i : in  STD_LOGIC;
			  rst_i : in STD_LOGIC;
			  fix_i : in STD_LOGIC;
			  OCXO_cv_value_i : in STD_LOGIC_VECTOR (15 downto 0);
			  OCXO_loop_control_i : in STD_LOGIC;
           cv_word_o : out  STD_LOGIC_VECTOR (15 downto 0);
			  phase_diff_o : out STD_LOGIC_VECTOR (15 downto 0);
			  update_dac_o : out STD_LOGIC;
			  int_PPS_tick_o : out STD_LOGIC;
			  int_PPS_o : out STD_LOGIC);
end GPS_PLL;

architecture Behavioral of GPS_PLL is

component GPS_phase_detect is
    Port ( clk_i : in  STD_LOGIC;
			  rst_i : in STD_LOGIC;
           GPS_pps_i : in  STD_LOGIC;
			  int_pps_tick_o : out STD_LOGIC;
			  int_pps_o : out STD_LOGIC;
           phase_diff_o : out  STD_LOGIC_VECTOR (27 downto 0);
           update_tick_o : out  STD_LOGIC
			  );
end component;

component GPS_phase_loop is
    Port ( clk_i : in STD_LOGIC;
			  rst_i : in STD_LOGIC;
			  fix_i : in STD_LOGIC;
			  fastslow_i : in STD_LOGIC;
			  OCXO_cv_value_i : in STD_LOGIC_VECTOR (15 downto 0);
			  phase_diff_i : in  STD_LOGIC_VECTOR (27 downto 0);
           update_tick_i : in  STD_LOGIC;
           dac_word_o : out  STD_LOGIC_VECTOR (15 downto 0)
			  );
end component;

--signaler til fase loop
signal phase_diff : std_logic_vector (27 downto 0);
signal update_tick : std_logic;

--signaler til generering af OCXO 1 PPS
signal int_pps : std_logic;
signal int_pps_tick : std_logic;

begin
	-- Fase detektor
	phase_detect : GPS_phase_detect port map (
		clk_i => clk_i,
		rst_i => rst_i,
		GPS_pps_i => GPS_pps_i,
		int_pps_tick_o => int_pps_tick,
		int_pps_o => int_pps,
		phase_diff_o => phase_diff,
		update_tick_o => update_tick );
		
	-- Updater DAC udgang
	update_dac_o <= update_tick;
	
	-- Faseloop
	phase_loop : GPS_phase_loop port map (
		clk_i => clk_i,
		rst_i => rst_i,
		fix_i => fix_i,
		fastslow_i => OCXO_loop_control_i,
		OCXO_cv_value_i => OCXO_cv_value_i,
		phase_diff_i => phase_diff,
		update_tick_i => update_tick,
		dac_word_o => cv_word_o
		);
	
	int_pps_o <= int_pps;
	int_pps_tick_o <= int_pps_tick;
	phase_diff_o <= phase_diff(15 downto 0);
end Behavioral;

