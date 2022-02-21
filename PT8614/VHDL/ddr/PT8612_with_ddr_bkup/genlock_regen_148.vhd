library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity genlock_regen_148 is
   generic (
      tick_per                : integer;           -- Period on 10 (148.5/1.001) or 11 (148.5) used to make 27MHz tick on the 148.5MHz clock
      pll_per_div_tick_per    : integer;           -- decimation of clk_148_i to pll, 148.5 : 495/11 (45) or 148.5/1.001 : 500/10 (50)
      tick_count_at_tick      : integer;           -- Value of tick_count where the sec_tick_o is placed (larger value : sec_tick_o advances)
      pll_count_at_tick       : integer;           -- Value of pll_count where the sec_tick_o is placed (larger value : sec_tick_o advances)
      tick_count_dec_falling  : integer;           -- Value of tick_count when tick_count_dec_falling occurs (influences on position of sec_tick_o and trans_tick_o, larger value : pulses are delayed)
      pll_count_dec_falling   : integer            -- Value of pll_count when tick_count_dec_falling occurs (influences on position of sec_tick_o and trans_tick_o)
   );
   port (
      clk_148_i               : in std_logic;
      sec_pulse_i             : in std_logic;      -- sync pulse input from genlock_counter
      trans_pulse_i           : in std_logic;      -- indicates that a pulse from 27MHz domain is present
      trans_count_i           : in std_logic_vector(6 downto 0);  -- position of pulse that needs to be transferred
      vcxo_dec_falling_tick_i : in std_logic;      -- falling edge of vcxo divided
      sec_tick_o              : out std_logic;     -- second (or 1.001s) tick output
      trans_tick_o            : out std_logic      -- transferred tick
   );
end genlock_regen_148;


architecture Behavioral of genlock_regen_148 is


signal pll_count                 : std_logic_vector(5 downto 0);
signal tick_27_count             : std_logic_vector(3 downto 0);
signal sec_pulse                 : std_logic;
signal sec_pulse_tmp             : std_logic;

signal pll_fb_rising             : std_logic;
signal pll_fb_rising_reg         : std_logic;
signal pll_fb_reg                : std_logic;
signal pll_fb_rising_delayed     : std_logic;
signal pll_fb_falling            : std_logic;
signal del_count                 : std_logic_vector(6 downto 0);
signal tick_27                   : std_logic;
signal output_tick               : std_logic;


begin


--------------------------------------------------------------
-- decimator counting on clock from VCXO :
--------------------------------------------------------------
tick_27_counting : process (clk_148_i) begin
   if clk_148_i'event and clk_148_i = '1' then
      if vcxo_dec_falling_tick_i = '1' then
         tick_27_count <= conv_std_logic_vector(tick_count_dec_falling,4);
      else
         if tick_27_count = 0 then
            tick_27_count <= conv_std_logic_vector(tick_per-1,4);
         else
            tick_27_count <= tick_27_count - 1;
         end if;
      end if;
   end if;
end process;


pll_per_counting : process (clk_148_i) begin
   if clk_148_i'event and clk_148_i = '1' then
      if vcxo_dec_falling_tick_i = '1' then
         pll_count <= conv_std_logic_vector(pll_count_dec_falling,6);
      else
         if tick_27_count = 0 then
            if pll_count = 0 then
               pll_count <= conv_std_logic_vector(pll_per_div_tick_per-1,6);
            else
               pll_count <= pll_count - 1;
            end if;
         end if;
      end if;
   end if;
end process;


--------------------------------------------------------------
-- Count on approximately 27MHz to transfer tick from 27MHz
-- domain to 148.5MHz domain :
--------------------------------------------------------------
tick_27_generation : process (tick_27_count) begin
   if tick_27_count = 0 or tick_27_count = conv_std_logic_vector(5,4) then
      tick_27 <= '1';
   else
      tick_27 <= '0';
   end if;
end process;


count_27_delay_lsb : process (clk_148_i) begin
   if clk_148_i'event and clk_148_i = '1' then
      if tick_27_count = 0 then
         del_count(0) <= '1';
      elsif tick_27_count = conv_std_logic_vector(5,4) then
         del_count(0) <= '0';
      end if;
   end if;
end process;

del_count(6 downto 1) <= pll_count;


--------------------------------------------------------------
-- position of sec pulse :
--------------------------------------------------------------
output_tick_generation : process (pll_count, tick_27_count) begin
   if pll_count = conv_std_logic_vector(pll_count_at_tick,9) and tick_27_count = conv_std_logic_vector(tick_count_at_tick,4) then
      output_tick <= '1';
   else
      output_tick <= '0';
   end if;
end process;


--------------------------------------------------------------
-- regenerate sec tick in vcxo clock domain :
--------------------------------------------------------------
sec_pulse_tmp_generation : process (clk_148_i) begin
   if clk_148_i'event and clk_148_i = '1' then
      if vcxo_dec_falling_tick_i = '1' then
         if sec_pulse_i = '1' then
            sec_pulse_tmp <= '1';
         else
            sec_pulse_tmp <= '0';
         end if;
      end if;
   end if;
end process;


sec_tick_generation : process (clk_148_i) begin
   if clk_148_i'event and clk_148_i = '1' then
      if output_tick = '1' and sec_pulse_tmp = '1' then  --sec_pulse_i = '1' then
         sec_tick_o <= '1';
      else
         sec_tick_o <= '0';
      end if;
   end if;
end process;


--------------------------------------------------------------
-- regenerate pulse, transferred from 27MHz domain :
--------------------------------------------------------------
--  The example below:
--  pll_per_minus_1         = 8   (period 9)

--  In 27 MHz domain (this component):
--                     -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
--  clk               | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
--                   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
--                                 -------------------                 -------------------                 -------------------                 ---------------
--  pll_ref_o                     |                   |               |                   |               |                   |               |
--                   -------------                     ---------------                     ---------------                     ---------------
--                                     ---                                                             ---
--  trans_tick_i                      |   |                                                           |   |
--                   -----------------     -----------------------------------------------------------     ---------------------------------------------------
--                   - --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
--  pll_count         X 2 X 1 X 0 X 8 X 7 X 6 X 5 X 4 X 3 X 2 X 1 X 0 X 8 X 7 X 6 X 5 X 4 X 3 X 2 X 1 X 0 X 8 X 7 X 6 X 5 X 4 X 3 X 2 X 1 X 0 X 8 X 7 X 6 X 5
--                   - --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
--                   --------------------- --------------------------------------------------------------- ---------------------------------------------------
--  trans_count_o           ?             X                               7                               X                   0
--                   --------------------- --------------------------------------------------------------- ---------------------------------------------------
--                                             -------------------------------                                 -------------------------------
--  trans_pulse_o                             |                               |                               |                               |
--                   -------------------------                                 -------------------------------                                 ---------------
--
--  In 148.5 MHz domain (148_genlock_regen):
--                   - --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
--  pll_count         X 5 X 4 X 3 X 2 X 1 X 0 X 8 X 7 X 6 X 5 X 4 X 3 X 2 X 1 X 0 X 8 X 7 X 6 X 5 X 4 X 3 X 2 X 1 X 0 X 8 X 7 X 6 X 5 X 4 X 3 X 2 X 1 X 0 X 8
--                   - --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
--                                 ---------------                     ---------------                     ---------------                     ---------------
--  pll_fb_o                      |               |                   |               |                   |               |                   |
--                   -------------                 -------------------                 -------------------                 -------------------
--                                                         ---                                                             ---
--  trans_tick_o                                          |   |                                                           |   |
--                   -------------------------------------     -----------------------------------------------------------     -------------------------------


trans_tick_generation : process (clk_148_i) begin
   if clk_148_i'event and clk_148_i = '1' then
      if trans_pulse_i = '1' and trans_count_i = del_count and tick_27 = '1' then
         trans_tick_o <= '1';
      else
         trans_tick_o <= '0';
      end if;
   end if;
end process;


end Behavioral;