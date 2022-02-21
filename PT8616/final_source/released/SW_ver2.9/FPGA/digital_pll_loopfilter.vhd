library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_arith.all;
USE ieee.std_logic_unsigned.all;

--------------------------------------------------------------------
-- Digital PLL :
-- See simulink file: pt8612_dpll_fpga.mld for documentation.
--------------------------------------------------------------------
entity digital_pll_loopfilter is
   port (
      clk_i                   : in std_logic;
      sample_tick_i           : in std_logic;
      phase_i                 : in std_logic_vector(12 downto 0);
      lf_out_o                : out std_logic_vector(15 downto 0)
   );
end digital_pll_loopfilter;


architecture behavioral of digital_pll_loopfilter is


signal phase_sign_vector      : std_logic_vector(18 downto 0);
signal phase_extended         : std_logic_vector(31 downto 0);
signal lf_input_sum           : std_logic_vector(31 downto 0);
signal lf_int_limit           : std_logic_vector(30 downto 0);
signal lf_int_reg             : std_logic_vector(30 downto 0);
signal lf_output_sum          : std_logic_vector(18 downto 0);

constant zeros                : std_logic_vector(21 downto 0) := "0000000000000000000000";
constant alpha                : integer := -5; -- -7; --[-15;0]
constant beta                 : integer :=  6; --  5;  --[1;6]


begin


phase_sign_vector_generation : process (phase_i)
begin
   for i in 0 to 18 loop
      phase_sign_vector(i) <= phase_i(12);
   end loop;
end process;
--      32               19             13
phase_extended <= phase_sign_vector & phase_i;       -- phase_i represented by 32 length vector in 2's compliment

lf_input_summing : process (lf_int_reg, phase_extended)
begin
   lf_input_sum(16-alpha downto 0) <= ('0' & lf_int_reg(15-alpha downto 0)) + phase_extended(16-alpha downto 0);     -- multiply the input to the integrator by 2^alpha using oner more msb to prevent overflow
end process;

lf_int_limiter : process (lf_input_sum)                           -- limit integrator register input so no overflow occurs in integrator:
begin
   if lf_input_sum(16-alpha downto 15-alpha) = "11" then          -- lower limit:
      lf_int_limit(15-alpha downto 0) <= (others => '0');
   elsif lf_input_sum(16-alpha downto 15-alpha) = "10" then       -- upper limit:
      lf_int_limit(15-alpha downto 0) <= (others => '1');
   else                                                           -- normal range:
      lf_int_limit(15-alpha downto 0) <= lf_input_sum(15-alpha downto 0);
   end if;
end process;

lf_integrator_reg : process (clk_i)
begin
   if clk_i'event and clk_i = '1' then
      if sample_tick_i = '1' then
         lf_int_reg(15-alpha downto 0) <= lf_int_limit(15-alpha downto 0);
      end if;
   end if;
end process;

lf_output_summing : process (lf_int_reg, phase_extended)  -- LF output sum using one more msb than the DAC to prevent from overflow
begin
   --                                                                                                          beta zeros
   lf_output_sum <= ("000" & lf_int_reg(15-alpha downto -alpha)) + (phase_extended(18-beta downto 0) & zeros(beta-1 downto 0));
end process;

lf_output_limiter : process (lf_output_sum)           -- limit the lf_output_sum and take out the lsb's to DAC (16)
begin
	if lf_output_sum(18 downto 16) = "000" then         -- normal range:
		lf_out_o <= lf_output_sum(15 downto 0);         
	elsif lf_output_sum(18) = '1'	then						 -- lower limit
      lf_out_o <= (others => '0');
   else															    -- upper limit
		lf_out_o <= (others => '1');
   end if;
end process;


end behavioral;