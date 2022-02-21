library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_arith.all;
USE ieee.std_logic_unsigned.all; --var unsigned 

--------------------------------------------------------------------
-- Digital PLL :
-- See simulink file: pt8612_dpll_fpga.mld for documentation.
--------------------------------------------------------------------
entity digital_pll_phase_detector is
   generic (
      vcxo_div_per            : integer     
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
end digital_pll_phase_detector;

architecture behavioral of digital_pll_phase_detector is

signal ref_decimated                : std_logic;
signal clk_ref_dec_delayed          : std_logic_vector(1 downto 0);
signal clk_ref_dec_rising           : std_logic;
signal per_end_load_pos_index       : std_logic_vector(4 downto 0);
signal make_shorter                 : std_logic;
signal make_longer                  : std_logic;
signal phase_count                  : std_logic_vector(11 downto 0);
signal period_end_delayed           : std_logic_vector(3 downto 0);
signal phase_diff                   : std_logic_vector(12 downto 0);
signal clk_vcxo_decimated           : std_logic;
signal clk_vcxo_decimated_delayed   : std_logic;


begin

--                           --   --   --   --   --   --   --   --   --   --   --          --   --   --   --   --   --   --   --   --   --   -- 
-- clk_vcxo_i:              |  | |  | |  | |  | |  | |  | |  | |  | |  | |  | |  |        |  | |  | |  | |  | |  | |  | |  | |  | |  | |  | |  |
--                         -    -    -    -    -    -    -    -    -    -    -           -    -    -    -    -    -    -    -    -    -    -    
--                                       -----------------------------------------       --------------------------------------
-- ref_decimated_i:                     |                                                                                      |
--                         -------------                                                                                        --------------------------------
--                                               ----                                    
-- clk_ref_dec_rising:                          |    |                                   
--                         ---------------------      ----------------------------       ---------------------------------------------------------
--                         - ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---       - ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
-- phase_count:             X  7 X  6 X  5 X  4 X  3 X  2 X  1 X  0 X -1 X -2 X -3        X-247X-248X-249X-250X 249X 248X 247X 246X 245X 244X
--                         - ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---       - ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
--                         -------------------------- ----------------------------       ----------------------------------------------------------------
-- phase_diff:                                       X   7  (3*2-1)                           7                                                    
--                         -------------------------- ----------------------------       ----------------------------------------------------------------
--                                                                   -------------       ---------------------
-- clk_vcxo_decimated_o:                                            |                                         |
--                         -----------------------------------------                                           -------------------------------------
--                                                                                                        ----
-- period_end_delayed(1):                                                                                |    |
--                         -------------------------------------------------------       ----------------      --------------------------------------
--                         -------------------------------------------------------       --------------------- --------------------------------------------------
-- lf_int_reg:                                                                                                X                                                  
--                         -------------------------------------------------------       --------------------- --------------------------------------------------
--                         -------------------------- ----------------------------       --------------------- -------------------------------------------
-- lf_output:                                        X                                                        X       
--                         -------------------------- ----------------------------       --------------------- -------------------------------------------
--


--------------------------------------------------------------------
-- Reclock ref_decimated on clean vcxo clock :
--------------------------------------------------------------------
ref_decimated_reclocking : process (clk_vcxo_clean_i)
begin
   if clk_vcxo_clean_i'event and clk_vcxo_clean_i = '1' then
      ref_decimated <= ref_decimated_i;
   end if;
end process;


--------------------------------------------------------------------
-- Phase detector :
--------------------------------------------------------------------
ref_rising_generation : process (clk_vcxo_i)
begin
   if clk_vcxo_i'event and clk_vcxo_i = '1' then
      clk_ref_dec_delayed <= clk_ref_dec_delayed(0) & ref_decimated;
      if clk_ref_dec_delayed(1 downto 0) = "01" then
         clk_ref_dec_rising <= '1';
      else
         clk_ref_dec_rising <= '0';
      end if;
   end if;
end process;

per_end_load_pos_index <= period_end_delayed(2 downto 0) & make_shorter & make_longer;

vcxo_div_counting : process (clk_vcxo_i)
begin
   if clk_vcxo_i'event and clk_vcxo_i = '1' then
      case per_end_load_pos_index is
      when "01000" | "01011" => -- normal count load position, normal period:
         phase_count <= conv_std_logic_vector((vcxo_div_per-1)/2,12);
      when "00110" => -- 1 before normal count load position, shorter period:
         phase_count <= conv_std_logic_vector((vcxo_div_per-1)/2,12);
      when "10001" => -- 1 after normal count load position, longer period:
         phase_count <= conv_std_logic_vector((vcxo_div_per-1)/2,12);
      when others =>
         phase_count <= phase_count - 1;
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
   if phase_count = conv_std_logic_vector((2-vcxo_div_per)/2,12) then
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
      clk_vcxo_decimated <= phase_count(11);
   end if;
end process;


clk_vcxo_decimated_o    <= clk_vcxo_decimated;
vcxo_dec_falling_tick_o <= period_end_delayed(1);
phase_diff_o            <= phase_diff;

--------------------------------------------------------------------
-- Status:
--------------------------------------------------------------------
lock_warning_generation : process (phase_diff)
begin
   if phase_diff = "111111111111" or phase_diff = "000000000001" then
      lock_warning_o <= '0';
   else
      lock_warning_o <= '1';
   end if;
end process;

lock_error_generation : process (clk_vcxo_i)
begin
   if clk_vcxo_i'event and clk_vcxo_i = '1' then
      clk_vcxo_decimated_delayed <= clk_vcxo_decimated;
      if make_shorter = '1' or make_longer = '1' then
         lock_error_o <= '1';
      elsif clk_vcxo_decimated_delayed = '0' and clk_vcxo_decimated = '1' then   -- vcxo decimated rising
         lock_error_o <= '0';
      end if;
   end if;
end process;


end behavioral;