library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_arith.all;
USE ieee.std_logic_unsigned.all;

--------------------------------------------------------------------
-- Digital PLL :
-- See simulink file: pt8612_dpll_fpga.mld for documentation.
--------------------------------------------------------------------
entity digital_pll_sd_dac is
   port (
      clk_i                      : in std_logic;
      data_i                     : in std_logic_vector(15 downto 0);
      enable_o                   : out std_logic;
      pdm_o                      : out std_logic
   );
end digital_pll_sd_dac;


architecture behavioral of digital_pll_sd_dac is

-- dac:
signal dac_enable                   : std_logic;
signal pdm_low                      : std_logic;
signal dac_enable_count             : std_logic_vector(4 downto 0);
signal dac_int_reg                  : std_logic_vector(16 downto 0);


begin


--------------------------------------------------------------------
-- Single bit DAC :
-- 16-bit resolution 1st order single bit modulator with return to zero pulses
--------------------------------------------------------------------

dac_enable_generation : process (clk_i)
begin
   if clk_i'event and clk_i = '1' then
      if dac_enable_count = 0 then
         dac_enable_count <= "11111";
      else
         dac_enable_count <= dac_enable_count - 1;
      end if;
      if dac_enable_count = "00001" then
         dac_enable <= '1';
      else
         dac_enable <= '0';
      end if;
      if dac_enable_count = 0 then
         pdm_low <= '1';
      elsif dac_enable_count = "11000" then
         pdm_low <= '0';
      end if;
   end if;
end process;

enable_o <= dac_enable;


dac_int_reg_generation : process (clk_i)
begin
   if clk_i'event and clk_i = '1' then
      if dac_enable = '1' then
         dac_int_reg <= dac_int_reg + (not dac_int_reg(16) & data_i);
      end if;
   end if;
end process;

pdm_generation : process (clk_i)
begin
   if clk_i'event and clk_i = '1' then
      if pdm_low = '0' and dac_int_reg(16) = '0' then
         pdm_o <= '1';
      else
         pdm_o <= '0';
      end if;
   end if;
end process;


end behavioral;