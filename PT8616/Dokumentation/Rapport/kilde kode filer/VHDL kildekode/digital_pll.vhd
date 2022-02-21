-----------------------------------------------------------
--				 Digital PLL
-----------------------------------------------------------

library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_arith.all;
USE ieee.std_logic_signed.all;

entity digital_pll is
   generic (
      vcxo_div_per         : integer         -- decimation of clk_i to pll, (495/500)
   );
   port (
      clk_vcxo_i           : in std_logic;
      clk_ref_decimated_i  : in std_logic;
      clk_vcxo_dec_tick_o  : out std_logic;  -- tick at falling edge of decimated vcxo to phase comparator 
      pdm_o                : out std_logic;  -- pulse density modulated signal
      clk_vcxo_decimated_o : out std_logic;  -- for debug
      lock_warning_o       : out std_logic;
      lock_error_o         : out std_logic;
		ext_make_shorter		: in std_logic;
		ext_make_longer		: in std_logic
   );
end digital_pll;

architecture behavioral of digital_pll is

-- phase detector:
signal clk_ref_dec_delayed          : std_logic_vector(2 downto 0);
signal clk_ref_dec_dac_en_delayed   : std_logic_vector(2 downto 0);

signal clk_ref_dec_rising           : std_logic;
signal per_end_load_pos_index       : std_logic_vector(6 downto 0);
signal make_shorter                 : std_logic;
signal make_longer                  : std_logic;
signal phase_count                  : std_logic_vector(9 downto 0);
signal period_end_delayed           : std_logic_vector(3 downto 0);
signal phase_diff                   : std_logic_vector(10 downto 0);
signal phase_diff_limit             : std_logic_vector(10 downto 0);
signal ext_make_shorter_flag			: std_logic;
signal ext_make_longer_flag			: std_logic;
signal ext_make_longer_delayline		: std_logic_vector(1 downto 0);
signal ext_make_shorter_delayline	: std_logic_vector(1 downto 0);
signal clear_flag							: std_logic;
                                    
-- dac:                             
signal dac_input                    : std_logic_vector(15 downto 0);
signal dac_enable                   : std_logic;
signal pdm_low                      : std_logic;
signal dac_enable_count             : std_logic_vector(4 downto 0);
signal dac_int_reg                  : std_logic_vector(16 downto 0);
                                    
--loop filter:                      
signal phase_diff_sign_vector       : std_logic_vector(20 downto 0);
signal phase_diff_extended          : std_logic_vector(31 downto 0);
signal lf_input_sum                 : std_logic_vector(31 downto 0);
signal lf_int_limit                 : std_logic_vector(30 downto 0);
signal lf_int_reg                   : std_logic_vector(30 downto 0);
signal lf_output_sum                : std_logic_vector(16 downto 0);
signal lf_output                    : std_logic_vector(15 downto 0);
                                    
constant zeros                      : std_logic_vector(21 downto 0) := "0000000000000000000000";
constant alpha                      : integer := -9;  --[-15;0]
constant beta                       : integer := 5;   --[1;6]


begin


--------------------------------------------------------------------
-- Phase detector :
--------------------------------------------------------------------
ref_rising_generation : process (clk_vcxo_i)
begin
   if clk_vcxo_i'event and clk_vcxo_i = '1' then
      clk_ref_dec_delayed <= clk_ref_dec_delayed(1 downto 0) & clk_ref_decimated_i;
      if clk_ref_dec_delayed(2 downto 1) = "01" then
         clk_ref_dec_rising <= '1';
      else
         clk_ref_dec_rising <= '0';
      end if;
   end if;
end process;

per_end_load_pos_index <= period_end_delayed(2 downto 0) & make_shorter & make_longer & ext_make_shorter_flag & ext_make_longer_flag;

vcxo_div_counting : process (clk_vcxo_i)
begin
   if clk_vcxo_i'event and clk_vcxo_i = '1' then
		ext_make_shorter_delayline(1) <= ext_make_shorter_delayline(0);
		ext_make_shorter_delayline(0) <= ext_make_shorter;
		
		ext_make_longer_delayline(1) <= ext_make_longer_delayline(0);
		ext_make_longer_delayline(0) <= ext_make_longer;
		
		if ext_make_longer_delayline = "01" then
			ext_make_longer_flag <= '1';
		elsif ext_make_shorter_delayline = "01" then
			ext_make_shorter_flag <= '1';
		elsif clear_flag = '1' then
			ext_make_shorter_flag <= '0';
			ext_make_longer_flag <= '0';
		end if;
		
      case per_end_load_pos_index is
			when "0100000" | "0101100" => -- normal count load position, normal period:
				phase_count <= conv_std_logic_vector((vcxo_div_per-1)/2,10);
			when "0011000" | "0010010" => -- 1 before normal count load position, shorter period:
				phase_count <= conv_std_logic_vector((vcxo_div_per-1)/2,10);
				clear_flag <= '1';
			when "1000100" | "1000001" => -- 1 after normal count load position, longer period:
				phase_count <= conv_std_logic_vector((vcxo_div_per-1)/2,10);
				clear_flag <= '1';
			when others =>
				phase_count <= phase_count - 1;
				clear_flag <= '0';
      end case;
   end if;
end process;

make_longer_generation : process (clk_vcxo_i)
begin
   if clk_vcxo_i'event and clk_vcxo_i = '1' then
      if period_end_delayed(0) = '1' and clk_ref_dec_rising = '1' then
         make_longer <= '1';
      elsif period_end_delayed(2) = '1' then
         make_longer <= '0';
      end if;
   end if;
end process;

make_shorter_generation : process (clk_vcxo_i)
begin
   if clk_vcxo_i'event and clk_vcxo_i = '1' then
      if period_end_delayed(3) = '1' and clk_ref_dec_rising = '1' then
         make_shorter <= '1';
      elsif period_end_delayed(2) = '1' then
         make_shorter <= '0';
      end if;
   end if;
end process;

period_end_delayed_generation : process (phase_count, clk_vcxo_i)
begin
   if phase_count = conv_std_logic_vector((2-vcxo_div_per)/2,10) then
      period_end_delayed(0) <= '1';
   else
      period_end_delayed(0) <= '0';
   end if;
   if clk_vcxo_i'event and clk_vcxo_i = '1' then
      period_end_delayed(3 downto 1) <= period_end_delayed(2 downto 0);
   end if;
end process;

phase_diff_generation : process (clk_vcxo_i)
begin
   if clk_vcxo_i'event and clk_vcxo_i = '1' then
      if clk_ref_dec_rising = '1' then
         phase_diff <= phase_count & '1';
      end if;
   end if;
end process;

clk_vcxo_decimated_generation : process (clk_vcxo_i)
begin
   if clk_vcxo_i'event and clk_vcxo_i = '1' then
      clk_vcxo_decimated_o <= phase_count(9);
   end if;
end process;

clk_vcxo_dec_tick_o <= period_end_delayed(1);

--------------------------------------------------------------------
-- Loop filter :
--------------------------------------------------------------------
phase_diff_sign_vector_generation : process (phase_diff)
begin
   for i in 0 to 20 loop
      phase_diff_sign_vector(i) <= phase_diff(10);
   end loop;
end process;
--      32                       21                 10
phase_diff_extended <= phase_diff_sign_vector & phase_diff;       -- phase_diff represented by 32 length vector in 2's compliment

lf_input_summing : process (lf_int_reg, phase_diff_extended)
begin
   lf_input_sum(16-alpha downto 0) <= ('0' & lf_int_reg(15-alpha downto 0)) + phase_diff_extended(16-alpha downto 0);     -- multiply the input to the integrator by 2^alpha using oner more msb to prevent overflow
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

lf_integrator_reg : process (clk_vcxo_i)
begin
   if clk_vcxo_i'event and clk_vcxo_i = '1' then
      if period_end_delayed(1) = '1' then
         lf_int_reg(15-alpha downto 0) <= lf_int_limit(15-alpha downto 0);
      end if;
   end if;
end process;

lf_output_summing : process (lf_int_reg, phase_diff_extended)  -- LF output sum using one more msb than the DAC to prevent from overflow
begin
   --                                                                                                beta zeros   
   lf_output_sum <= ('0' & lf_int_reg(15-alpha downto -alpha)) + (phase_diff_extended(16-beta downto 0) & zeros(beta-1 downto 0));
end process;

lf_output_limiter : process (lf_output_sum)           -- limit the lf_output_sum and take out the lsb's to DAC (16)
begin
   if lf_output_sum(16 downto 15) = "11" then         -- lower limit:
      lf_output <= (others => '0');
   elsif lf_output_sum(16 downto 15) = "10" then      -- upper limit:
      lf_output <= (others => '1');
   else
      lf_output <= lf_output_sum(15 downto 0);        -- normal range:
   end if;
end process;


--------------------------------------------------------------------
-- Single bit DAC : 
-- 16-bit resolution 1st order single bit modulator with return to zero pulses
--------------------------------------------------------------------
dac_input_register : process (clk_vcxo_i)
begin
   if clk_vcxo_i'event and clk_vcxo_i = '1' then
      if dac_enable = '1' then
         clk_ref_dec_dac_en_delayed <= clk_ref_dec_dac_en_delayed(1 downto 0) & clk_ref_dec_delayed(2);
         if clk_ref_dec_dac_en_delayed(2 downto 1) = "01" then
            dac_input <= lf_output;
         end if;
      end if;
   end if;
end process;

dac_enable_generation : process (clk_vcxo_i)
begin
   if clk_vcxo_i'event and clk_vcxo_i = '1' then
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

dac_int_reg_generation : process (clk_vcxo_i)
begin
   if clk_vcxo_i'event and clk_vcxo_i = '1' then
      if dac_enable = '1' then
         dac_int_reg <= dac_int_reg + (not dac_int_reg(16) & dac_input);
      end if;
   end if;
end process;

pdm_generation : process (clk_vcxo_i)
begin
   if clk_vcxo_i'event and clk_vcxo_i = '1' then
      if pdm_low = '0' and dac_int_reg(16) = '0' then
         pdm_o <= '1';
      else
         pdm_o <= '0';
      end if;
   end if;
end process;

--------------------------------------------------------------------
-- PLL status:
--------------------------------------------------------------------
lock_warning_generation : process (phase_diff)
begin
   if phase_diff > "1111111000" and phase_diff < "0000000111" then
      lock_warning_o <= '0';
   else
      lock_warning_o <= '1';
   end if;
end process;

lock_error_generation : process (make_longer, make_shorter)
begin
   if make_shorter = '1' or make_longer = '1' then
      lock_error_o <= '1';
   else
      lock_error_o <= '0';
   end if;
end process;


end behavioral;