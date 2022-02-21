library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity genlock_counter is
   generic (
      pll_per                 : integer;           -- decimation of clk_i to pll, (91)
      half_frame_per          : integer;           -- count on pll tick           (9900)
      sync_per                : integer;           -- count on frame tick         (2)
      fps_25_z30              : std_logic;         -- 1 : 25, 0 : 30 frames per ~second
      ref_step_debug          : std_logic          -- when high introducese step of pll_ref_o to test pll step response
   );
   port (
      clk_i                   : in std_logic;      -- clock
      sync_i                  : in std_logic;      -- sync input
      trans_tick_i            : in std_logic;      -- tick that needs to be transferred to 148.5MHz domain
      trans_pulse_o           : out std_logic;     -- indicates that a pulse is present that needs to be transferred
      trans_count_o           : out std_logic_vector(6 downto 0);  -- position of pulse that needs to be transferred
      pll_ref_o               : out std_logic;     -- signal to phase comparator
      sec_pulse_o             : out std_logic;     -- second pulse output (duration = pll period)
      sec_tick_advanced_o     : out std_logic;     -- second tick at falling edge of pll_ref_o (advanced pll_count_at_falling+1)
      frame_count_i           : in std_logic_vector(7 downto 0); -- frame number in a second period
      new_frame_count_i       : in std_logic;      -- new frame count value set from serial interface
      set_frame_count_i       : in std_logic;      -- when asserted the frame counter value is set to match the temporary frame counter
      frame_count_o           : out std_logic_vector(7 downto 0); -- frame number in a second period
      warning_o               : out std_logic_vector(1 downto 0); -- a single or more sync period fails & a single or more failing frame count set command
      error_o                 : out std_logic_vector(1 downto 0)  -- repetitive failing sync periods & repetitive failing frame count set commands
   );
end genlock_counter;


-- The example below:
-- pll_per                 = 3
-- half_frame_per          = 2
-- sync_per                = 2
-- fps                     = 3
--
--
--                     -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
--  clk               | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
--                   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
--                             ---                                                                                             ---                                                                                             ---
--  sync_tick                 |   |                                                                                           |   |                                                                                           |   |
--                   ---------     -------------------------------------------------------------------------------------------     -------------------------------------------------------------------------------------------     -------------------------------------------------------------------------------------------
--                   - --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
--  pll_count         X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0 X 2 X 1 X 0
--                   - --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
--                   - ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- -----------
--  half_frame_count  X     0     X     1     X     0     X     1     X     0     X     1     X     0     X     1     X     0     X     1     X     0     X     1     X     0     X     1     X     0     X     1     X     0
--                   - ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- ----------- -----------
--                   ------------- ----------------------- ----------------------- ----------------------- ----------------------- ----------------------- ----------------------- ----------------------- ----------------------- ----------------------- -----------------------
--  frame_count             0     X           1           X           0           X           1           X           0           X           1           X           0           X           1           X           0           X           1           X           0
--                   ------------- ----------------------- ----------------------- ----------------------- ----------------------- ----------------------- ----------------------- ----------------------- ----------------------- ----------------------- -----------------------
--                   ------------- ----------------------------------------------- ----------------------------------------------- ----------------------------------------------- ----------------------------------------------- ----------------------------------------------- -----------------------------------------------
--  sync_count              0     X                       1                       X                       0                       X                       1                       X                       0                       X                       1                       X                       0
--                   ------------- ----------------------------------------------- ----------------------------------------------- ----------------------------------------------- ----------------------------------------------- ----------------------------------------------- -----------------------------------------------
--                             ---                                                                                             ---                                                                                             ---
--  expected_sync             |   |                                                                                           |   |                                                                                           |   |
--                   ---------     -------------------------------------------------------------------------------------------     -------------------------------------------------------------------------------------------     -------------------------------------------------------------------------------------------
--                                 ---                                             ---                                             ---                                             ---                                             ---
--  frame_tick                    |   |                                           |   |                                           |   |                                           |   |                                           |   |
--                   -------------     -------------------------------------------     -------------------------------------------     -------------------------------------------     -------------------------------------------     -------------------------------------------
--                     -------     -------     -------     -------     -------     -------     -------     -------     -------     -------     -------     -------     -------     -------     -------     -------     -------     -------     -------     -------     -------
--  pll_ref_o         |       |   |       |   |       |   |       |   |       |   |       |   |       |   |       |   |       |   |       |   |       |   |       |   |       |   |       |   |       |   |       |   |       |   |       |   |       |   |       |   |       |
--                   -         ---         ---         ---         ---         ---         ---         ---         ---         ---         ---         ---         ---         ---         ---         ---         ---         ---         ---         ---         ---         ---
--                   ----------------- ----------------------------------------------- ----------------------------------------------- ----------------------------------------------- ----------------------------------------------- ----------------------------------------------- -----------------------------------------------
--  sec_count               0         X                       2                       X                       1                       X                       0                       X                       2                       X                       1                       X                       0
--                   ----------------- ----------------------------------------------- ----------------------------------------------- ----------------------------------------------- ----------------------------------------------- ----------------------------------------------- -----------------------------------------------
--                     -----------                                                                                                                                     -----------                                                                                                                                     -----------
--  sec_pulse_o       |           |                                                                                                                                   |           |                                                                                                                                   |           |
--                   -             -----------------------------------------------------------------------------------------------------------------------------------             -----------------------------------------------------------------------------------------------------------------------------------             ---


architecture Behavioral of genlock_counter is

signal sync                         : std_logic;
signal sync_tick                    : std_logic;
signal pll_count                    : std_logic_vector(6 downto 0);
signal half_frame_count             : std_logic_vector(12 downto 0);
signal sync_count                   : std_logic_vector(1 downto 0);
signal last_frame_in_sec            : std_logic;
signal new_frame_count_delayed      : std_logic_vector(1 downto 0);
signal sec_count                    : std_logic_vector(5 downto 0);
signal sec_count_next               : std_logic_vector(5 downto 0);
signal sec_count_tmp                : std_logic_vector(5 downto 0);
signal sec_count_tmp_next           : std_logic_vector(5 downto 0);
signal load_sec_count               : std_logic;
signal load_sec_count_tmp           : std_logic;
signal set_sec_count_delayed        : std_logic_vector(1 downto 0);
signal set_error_count              : std_logic_vector(1 downto 0);

signal frame_count                  : std_logic;
signal expected_sync                : std_logic;
signal genlock_error_count          : std_logic_vector(2 downto 0);
signal pll_ref_rising               : std_logic;
signal pll_ref                      : std_logic;
signal trans_count                  : std_logic_vector(6 downto 0);
signal frame_count_delayed          : std_logic_vector(1 downto 0);
signal sync_reset                   : std_logic;
signal sync_delayed                 : std_logic_vector(2 downto 0);
signal frame_tick                   : std_logic;
signal pll_ref_falling              : std_logic;
signal sec_pulse_delayed            : std_logic_vector(1 downto 0);
signal trans_tick_delayed           : std_logic;
signal illegal_pulse                : std_logic;
signal no_pulse                     : std_logic;

signal pll_ref_delayed              : std_logic_vector(29 downto 0);
signal pll_ref_debug                : std_logic;
signal debug_step_count             : std_logic_vector(3 downto 0);

constant pll_count_at_falling       : integer := 45;
constant pll_count_at_sync_tick     : integer := 45;

component glitch_remover
   port (
      clk_i            : in std_logic;
      signal_i         : in std_logic;
      clean_signal_o   : out std_logic
   );
end component;


begin


-------------------------------------------------------------
-- Remove glitches on sync_i:
-------------------------------------------------------------
glitch_removing_f4m : glitch_remover
   port map(
      clk_i             => clk_i,
      signal_i          => sync_i,
      clean_signal_o    => sync
   );


-------------------------------------------------------------
-- Generate tick at sync rising edge
-------------------------------------------------------------
sync_delayed(0)   <= sync;

sync_tick_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_delayed(1 downto 0) = "01" then   -- rising
         sync_tick <= '1';
      else
         sync_tick <= '0';
      end if;
      sync_delayed(2 downto 1) <= sync_delayed(1 downto 0);
   end if;
end process;


--------------------------------------------------------------
-- Series of counters with a total period equal to the sync period:
--------------------------------------------------------------
pll_counting : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         pll_count <= conv_std_logic_vector(pll_per-1,7);
      else
         if pll_count = 0 then
            pll_count <= conv_std_logic_vector(pll_per-1,7);
         else
            pll_count <= pll_count - 1;
         end if;
      end if;
   end if;
end process;

half_frame_counting : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         half_frame_count <= conv_std_logic_vector(half_frame_per-1,13);
      elsif pll_count = 0 then
         if half_frame_count = 0 then
            half_frame_count <= conv_std_logic_vector(half_frame_per-1,13);
         else
            half_frame_count <= half_frame_count - 1;
         end if;
      end if;
   end if;
end process;

frame_counting : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         frame_count <= '1';
      elsif pll_count = 0 and half_frame_count = 0 then
         if frame_count = '0' then
            frame_count <= '1';
         else
            frame_count <= '0';
         end if;
      end if;
   end if;
end process;

sync_counting : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         sync_count <= conv_std_logic_vector(sync_per-1,2);
      elsif pll_count = 0 and half_frame_count = 0 and frame_count = '0' then
         if sync_count = 0 then
            sync_count <= conv_std_logic_vector(sync_per-1,2);
         else
            sync_count <= sync_count - 1;
         end if;
      end if;
   end if;
end process;


--------------------------------------------------------------
-- Indicates if an illegal sync_pulse occurs
--------------------------------------------------------------
expected_sync_generation : process (pll_count, half_frame_count, frame_count, sync_count) begin
   if pll_count = 0 and half_frame_count = 0 and frame_count = '0' and sync_count = 0 then
      expected_sync <= '1';
   else
      expected_sync <= '0';
   end if;
end process;

illegal_pulse_check : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if expected_sync = '1' then
         illegal_pulse <= '0';
      else                             -- no sync pulse expected:
         if sync_tick = '1' then
            illegal_pulse <= '1';
         end if;
      end if;
   end if;
end process;


--------------------------------------------------------------
-- Indicates if no sync occurs
--------------------------------------------------------------
no_sync_check : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if expected_sync = '1' then
         if sync_tick = '0' then
            no_pulse <= '1';
         else
            no_pulse <= '0';
         end if;
      end if;
   end if;
end process;


--------------------------------------------------------------
-- Genlock error counting.
-- When a legal sync occurs the counter is set to max.
-- Otherwise one decrement if illegal_pulse occurs.
--------------------------------------------------------------
genlock_error_counting : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if expected_sync = '1' then
         if sync_tick = '1' then                -- legal sync:
            genlock_error_count <= "111";
         else
            if illegal_pulse = '1' or no_pulse = '1' then  -- a genlock period with error
               if genlock_error_count /= 0 then -- count towards zero:
                  genlock_error_count <= genlock_error_count - 1;
               end if;
            end if;
         end if;
      end if;
   end if;
end process;


--------------------------------------------------------------
-- Generate sync_reset if genlock is not OK:
--------------------------------------------------------------
sync_reset_generation : process (genlock_error_count, sync_tick) begin
   if genlock_error_count = "000" and sync_tick = '1' then  -- several sync errors and sync_tick:
      sync_reset <= '1';
   else
      sync_reset <= '0';
   end if;
end process;


--------------------------------------------------------------
-- Generate divided clock to PLL:
--------------------------------------------------------------
pll_ref_falling_rising : process (pll_count) begin
   if pll_count = 0 then
      pll_ref_rising <= '1';
   else
      pll_ref_rising <= '0';
   end if;
   if pll_count = conv_std_logic_vector(pll_count_at_falling,7) then
      pll_ref_falling <= '1';
   else
      pll_ref_falling <= '0';
   end if;
end process;


pll_ref_o_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         pll_ref   <= '1';
      else
         if pll_ref_rising = '1' then
            pll_ref   <= '1';
         elsif pll_ref_falling = '1' then
            pll_ref   <= '0';
         end if;
      end if;
   end if;
end process;


--------------------------------------------------------------
-- Generate frame_tick at rising edge of frame_count:
--------------------------------------------------------------
frame_count_delayed(0) <= frame_count;

frame_count_delaying : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      frame_count_delayed(1) <= frame_count_delayed(0);
   end if;
end process;

frame_tick_generation : process (frame_count_delayed) begin
   if frame_count_delayed = "01" then  -- rising edge:
      frame_tick <= '1';
   else
      frame_tick <= '0';
   end if;
end process;


--------------------------------------------------------------
-- Count frames using a johnson counter.
-- The counter is set from the serial interface:
--------------------------------------------------------------
load_sec_count_tmp_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      new_frame_count_delayed <= new_frame_count_delayed(0) & new_frame_count_i;
      if new_frame_count_delayed = "01" then  -- rising edge:
         load_sec_count_tmp <= '1';
      else
         load_sec_count_tmp <= '0';
      end if;
   end if;
end process;

sec_count_tmp_counting : process (sec_count_tmp, frame_tick) begin
   sec_count_tmp_next <= sec_count_tmp;
   if frame_tick = '1' then
      if fps_25_z30 = '1' then       -- f8g genlock:
         case sec_count_tmp is
         when "000000" =>  sec_count_tmp_next <= "000001";   -- 0  : increment
         when "000001" =>  sec_count_tmp_next <= "000011";   -- 1  : increment
         when "000011" =>  sec_count_tmp_next <= "000010";   -- 2  : increment
         when "000010" =>  sec_count_tmp_next <= "000110";   -- 3  : increment
         when "000110" =>  sec_count_tmp_next <= "000111";   -- 4  : increment
         when "000111" =>  sec_count_tmp_next <= "000101";   -- 5  : increment
         when "000101" =>  sec_count_tmp_next <= "000100";   -- 6  : increment
         when "000100" =>  sec_count_tmp_next <= "001100";   -- 7  : increment
         when "001100" =>  sec_count_tmp_next <= "001101";   -- 8  : increment
         when "001101" =>  sec_count_tmp_next <= "001111";   -- 9  : increment
         when "001111" =>  sec_count_tmp_next <= "001110";   -- 10 : increment
         when "001110" =>  sec_count_tmp_next <= "001010";   -- 11 : increment
         when "001010" =>  sec_count_tmp_next <= "001011";   -- 12 : increment
         when "001011" =>  sec_count_tmp_next <= "001001";   -- 13 : increment
         when "001001" =>  sec_count_tmp_next <= "001000";   -- 14 : increment
         when "001000" =>  sec_count_tmp_next <= "011000";   -- 15 : increment
         when "011000" =>  sec_count_tmp_next <= "011001";   -- 16 : increment
         when "011001" =>  sec_count_tmp_next <= "011011";   -- 17 : increment
         when "011011" =>  sec_count_tmp_next <= "011010";   -- 18 : increment
         when "011010" =>  sec_count_tmp_next <= "011110";   -- 19 : increment
         when "011110" =>  sec_count_tmp_next <= "011111";   -- 20 : increment
         when "011111" =>  sec_count_tmp_next <= "011101";   -- 21 : increment
         when "011101" =>  sec_count_tmp_next <= "011100";   -- 22 : increment
         when "011100" =>  sec_count_tmp_next <= "010100";   -- 23 : increment
         when "010100" =>  sec_count_tmp_next <= "110100";   -- 24 : increment
         when "110100" =>  sec_count_tmp_next <= "111100";   -- 25 : increment
         when "111100" =>  sec_count_tmp_next <= "111101";   -- 26 : increment
         when "111101" =>  sec_count_tmp_next <= "111111";   -- 27 : increment
         when "111111" =>  sec_count_tmp_next <= "111110";   -- 28 : increment
         when "111110" =>  sec_count_tmp_next <= "111010";   -- 29 : increment
         when "111010" =>  sec_count_tmp_next <= "111011";   -- 30 : increment
         when "111011" =>  sec_count_tmp_next <= "111001";   -- 31 : increment
         when "111001" =>  sec_count_tmp_next <= "111000";   -- 32 : increment
         when "111000" =>  sec_count_tmp_next <= "101000";   -- 33 : increment
         when "101000" =>  sec_count_tmp_next <= "101001";   -- 34 : increment
         when "101001" =>  sec_count_tmp_next <= "101011";   -- 35 : increment
         when "101011" =>  sec_count_tmp_next <= "101010";   -- 36 : increment
         when "101010" =>  sec_count_tmp_next <= "101110";   -- 37 : increment
         when "101110" =>  sec_count_tmp_next <= "101111";   -- 38 : increment
         when "101111" =>  sec_count_tmp_next <= "101101";   -- 39 : increment
         when "101101" =>  sec_count_tmp_next <= "101100";   -- 40 : increment
         when "101100" =>  sec_count_tmp_next <= "100100";   -- 41 : increment
         when "100100" =>  sec_count_tmp_next <= "100101";   -- 42 : increment
         when "100101" =>  sec_count_tmp_next <= "100111";   -- 43 : increment
         when "100111" =>  sec_count_tmp_next <= "100110";   -- 44 : increment
         when "100110" =>  sec_count_tmp_next <= "100010";   -- 45 : increment
         when "100010" =>  sec_count_tmp_next <= "100011";   -- 46 : increment
         when "100011" =>  sec_count_tmp_next <= "100001";   -- 47 : increment
         when "100001" =>  sec_count_tmp_next <= "100000";   -- 48 : increment
         when others   =>  sec_count_tmp_next <= "000000";   -- 49 : zero      ("100000")
         end case;
      else           -- fps = 30 : f4m genlock:
         case sec_count_tmp is
         when "000000" =>  sec_count_tmp_next <= "000001";   -- 0  : increment
         when "000001" =>  sec_count_tmp_next <= "000011";   -- 1  : increment
         when "000011" =>  sec_count_tmp_next <= "000010";   -- 2  : increment
         when "000010" =>  sec_count_tmp_next <= "000110";   -- 3  : increment
         when "000110" =>  sec_count_tmp_next <= "000111";   -- 4  : increment
         when "000111" =>  sec_count_tmp_next <= "000101";   -- 5  : increment
         when "000101" =>  sec_count_tmp_next <= "000100";   -- 6  : increment
         when "000100" =>  sec_count_tmp_next <= "001100";   -- 7  : increment
         when "001100" =>  sec_count_tmp_next <= "001101";   -- 8  : increment
         when "001101" =>  sec_count_tmp_next <= "001111";   -- 9  : increment
         when "001111" =>  sec_count_tmp_next <= "001110";   -- 10 : increment
         when "001110" =>  sec_count_tmp_next <= "001010";   -- 11 : increment
         when "001010" =>  sec_count_tmp_next <= "001011";   -- 12 : increment
         when "001011" =>  sec_count_tmp_next <= "001001";   -- 13 : increment
         when "001001" =>  sec_count_tmp_next <= "101001";   -- 14 : increment
         when "101001" =>  sec_count_tmp_next <= "101011";   -- 15 : increment
         when "101011" =>  sec_count_tmp_next <= "101010";   -- 16 : increment
         when "101010" =>  sec_count_tmp_next <= "101110";   -- 17 : increment
         when "101110" =>  sec_count_tmp_next <= "101111";   -- 18 : increment
         when "101111" =>  sec_count_tmp_next <= "101101";   -- 19 : increment
         when "101101" =>  sec_count_tmp_next <= "101100";   -- 20 : increment
         when "101100" =>  sec_count_tmp_next <= "100100";   -- 21 : increment
         when "100100" =>  sec_count_tmp_next <= "100101";   -- 22 : increment
         when "100101" =>  sec_count_tmp_next <= "100111";   -- 23 : increment
         when "100111" =>  sec_count_tmp_next <= "100110";   -- 24 : increment
         when "100110" =>  sec_count_tmp_next <= "100010";   -- 25 : increment
         when "100010" =>  sec_count_tmp_next <= "100011";   -- 26 : increment
         when "100011" =>  sec_count_tmp_next <= "100001";   -- 27 : increment
         when "100001" =>  sec_count_tmp_next <= "100000";   -- 28 : increment
         when others   =>  sec_count_tmp_next <= "000000";   -- 29 : zero      ("100000")
         end case;
      end if;
   end if;
end process;

sec_count_tmp_registering : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if load_sec_count_tmp = '1' then
         sec_count_tmp <= frame_count_i(5 downto 0);
      else
         sec_count_tmp <= sec_count_tmp_next;
      end if;
   end if;
end process;

--------------------------------------------------------------
-- Generate second (or second*1.001) pulse by counting frames
-- using a johnson counter:
--------------------------------------------------------------
sec_count_next_generation : process (clk_i) begin
   sec_count_next <= sec_count;
   if frame_tick = '1' then
      if fps_25_z30 = '1' then       -- f8g genlock:
         case sec_count is
         when "000000" =>  sec_count_next <= "000001";   -- 0  : increment
         when "000001" =>  sec_count_next <= "000011";   -- 1  : increment
         when "000011" =>  sec_count_next <= "000010";   -- 2  : increment
         when "000010" =>  sec_count_next <= "000110";   -- 3  : increment
         when "000110" =>  sec_count_next <= "000111";   -- 4  : increment
         when "000111" =>  sec_count_next <= "000101";   -- 5  : increment
         when "000101" =>  sec_count_next <= "000100";   -- 6  : increment
         when "000100" =>  sec_count_next <= "001100";   -- 7  : increment
         when "001100" =>  sec_count_next <= "001101";   -- 8  : increment
         when "001101" =>  sec_count_next <= "001111";   -- 9  : increment
         when "001111" =>  sec_count_next <= "001110";   -- 10 : increment
         when "001110" =>  sec_count_next <= "001010";   -- 11 : increment
         when "001010" =>  sec_count_next <= "001011";   -- 12 : increment
         when "001011" =>  sec_count_next <= "001001";   -- 13 : increment
         when "001001" =>  sec_count_next <= "001000";   -- 14 : increment
         when "001000" =>  sec_count_next <= "011000";   -- 15 : increment
         when "011000" =>  sec_count_next <= "011001";   -- 16 : increment
         when "011001" =>  sec_count_next <= "011011";   -- 17 : increment
         when "011011" =>  sec_count_next <= "011010";   -- 18 : increment
         when "011010" =>  sec_count_next <= "011110";   -- 19 : increment
         when "011110" =>  sec_count_next <= "011111";   -- 20 : increment
         when "011111" =>  sec_count_next <= "011101";   -- 21 : increment
         when "011101" =>  sec_count_next <= "011100";   -- 22 : increment
         when "011100" =>  sec_count_next <= "010100";   -- 23 : increment
         when "010100" =>  sec_count_next <= "110100";   -- 24 : increment
         when "110100" =>  sec_count_next <= "111100";   -- 25 : increment
         when "111100" =>  sec_count_next <= "111101";   -- 26 : increment
         when "111101" =>  sec_count_next <= "111111";   -- 27 : increment
         when "111111" =>  sec_count_next <= "111110";   -- 28 : increment
         when "111110" =>  sec_count_next <= "111010";   -- 29 : increment
         when "111010" =>  sec_count_next <= "111011";   -- 30 : increment
         when "111011" =>  sec_count_next <= "111001";   -- 31 : increment
         when "111001" =>  sec_count_next <= "111000";   -- 32 : increment
         when "111000" =>  sec_count_next <= "101000";   -- 33 : increment
         when "101000" =>  sec_count_next <= "101001";   -- 34 : increment
         when "101001" =>  sec_count_next <= "101011";   -- 35 : increment
         when "101011" =>  sec_count_next <= "101010";   -- 36 : increment
         when "101010" =>  sec_count_next <= "101110";   -- 37 : increment
         when "101110" =>  sec_count_next <= "101111";   -- 38 : increment
         when "101111" =>  sec_count_next <= "101101";   -- 39 : increment
         when "101101" =>  sec_count_next <= "101100";   -- 40 : increment
         when "101100" =>  sec_count_next <= "100100";   -- 41 : increment
         when "100100" =>  sec_count_next <= "100101";   -- 42 : increment
         when "100101" =>  sec_count_next <= "100111";   -- 43 : increment
         when "100111" =>  sec_count_next <= "100110";   -- 44 : increment
         when "100110" =>  sec_count_next <= "100010";   -- 45 : increment
         when "100010" =>  sec_count_next <= "100011";   -- 46 : increment
         when "100011" =>  sec_count_next <= "100001";   -- 47 : increment
         when "100001" =>  sec_count_next <= "100000";   -- 48 : increment
         when others   =>  sec_count_next <= "000000";   -- 49 : zero      ("100000")
         end case;
      else           -- fps = 30 : f4m genlock:
         case sec_count is
         when "000000" =>  sec_count_next <= "000001";   -- 0  : increment
         when "000001" =>  sec_count_next <= "000011";   -- 1  : increment
         when "000011" =>  sec_count_next <= "000010";   -- 2  : increment
         when "000010" =>  sec_count_next <= "000110";   -- 3  : increment
         when "000110" =>  sec_count_next <= "000111";   -- 4  : increment
         when "000111" =>  sec_count_next <= "000101";   -- 5  : increment
         when "000101" =>  sec_count_next <= "000100";   -- 6  : increment
         when "000100" =>  sec_count_next <= "001100";   -- 7  : increment
         when "001100" =>  sec_count_next <= "001101";   -- 8  : increment
         when "001101" =>  sec_count_next <= "001111";   -- 9  : increment
         when "001111" =>  sec_count_next <= "001110";   -- 10 : increment
         when "001110" =>  sec_count_next <= "001010";   -- 11 : increment
         when "001010" =>  sec_count_next <= "001011";   -- 12 : increment
         when "001011" =>  sec_count_next <= "001001";   -- 13 : increment
         when "001001" =>  sec_count_next <= "101001";   -- 14 : increment
         when "101001" =>  sec_count_next <= "101011";   -- 15 : increment
         when "101011" =>  sec_count_next <= "101010";   -- 16 : increment
         when "101010" =>  sec_count_next <= "101110";   -- 17 : increment
         when "101110" =>  sec_count_next <= "101111";   -- 18 : increment
         when "101111" =>  sec_count_next <= "101101";   -- 19 : increment
         when "101101" =>  sec_count_next <= "101100";   -- 20 : increment
         when "101100" =>  sec_count_next <= "100100";   -- 21 : increment
         when "100100" =>  sec_count_next <= "100101";   -- 22 : increment
         when "100101" =>  sec_count_next <= "100111";   -- 23 : increment
         when "100111" =>  sec_count_next <= "100110";   -- 24 : increment
         when "100110" =>  sec_count_next <= "100010";   -- 25 : increment
         when "100010" =>  sec_count_next <= "100011";   -- 26 : increment
         when "100011" =>  sec_count_next <= "100001";   -- 27 : increment
         when "100001" =>  sec_count_next <= "100000";   -- 28 : increment
         when others   =>  sec_count_next <= "000000";   -- 29 : zero      ("100000")
         end case;
      end if;
   end if;
end process;

sec_count_registering : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if load_sec_count = '1' then
         sec_count <= sec_count_tmp_next;
      else
         sec_count <= sec_count_next;
      end if;
   end if;
end process;

load_sec_count_controlling : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      set_sec_count_delayed <= set_sec_count_delayed(0) & set_frame_count_i;
      if set_sec_count_delayed = "01" then               -- rising edge:
         if sec_count = sec_count_tmp then               -- counters in sync:
            load_sec_count  <= '0';
            set_error_count <= (others => '1');
         else                                            -- counters out of sync:
            if set_error_count = 0 then                  -- allow counter change:
               load_sec_count  <= '1';
               set_error_count <= (others => '1');       -- restart counting errors (changes)
            else                                         -- require a number of counter set commands with different setting before executing
               load_sec_count  <= '0';
               set_error_count <= set_error_count - 1;
            end if;
         end if;
      else                                               
         load_sec_count  <= '0';
      end if;
   end if;
end process;

frame_count_o <= "00" & sec_count;

last_frame_in_sec_generation : process (sec_count) begin
   if fps_25_z30 = '1' then    -- f8g:
      if sec_count = "010100" or sec_count = "100000" then  -- 24 or 49
         last_frame_in_sec <= '1';
      else
         last_frame_in_sec <= '0';
      end if;
   else                    -- f4m:
      if sec_count = "100000" then  -- 29
         last_frame_in_sec <= '1';
      else
         last_frame_in_sec <= '0';
      end if;
   end if;
end process;

sec_pulse_generation : process (sec_count, frame_count, half_frame_count) begin
   if last_frame_in_sec = '1' and frame_count = '0' and half_frame_count = 0 then
      sec_pulse_delayed(0) <= '1';
   else
      sec_pulse_delayed(0) <= '0';
   end if;
end process;

sec_pulse_delaying : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      sec_pulse_delayed(1) <= sec_pulse_delayed(0);
   end if;
end process;


sec_pulse_o <= sec_pulse_delayed(1); --sec_pulse_delayed(1);


--------------------------------------------------------------
-- Generate tick (1.001 sec) that needs to be transferred
-- from 27MHz domain to 148.5MHz domain:
--------------------------------------------------------------

--  The example below:
--  pll_per          = 9

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
--  pll_count         X 6 X 5 X 4 X 3 X 2 X 1 X 0 X 8 X 7 X 6 X 5 X 4 X 3 X 2 X 1 X 0 X 8 X 7 X 6 X 5 X 4 X 3 X 2 X 1 X 0 X 8 X 7 X 6 X 5 X 4 X 3 X 2 X 1 X 0
--                   - --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
--                                 ---------------                     ---------------                     ---------------                     ---------------
--  pll_fb_o                      |               |                   |               |                   |               |                   |
--                   -------------                 -------------------                 -------------------                 -------------------
--                                                         ---                                                             ---
--  trans_tick_o                                          |   |                                                           |   |
--                   -------------------------------------     -----------------------------------------------------------     -------------------------------


--------------------------------------------------------------
-- Generate tick (1.001 sec) that needs to be transferred
-- from 27MHz domain to 148.5MHz domain:
--------------------------------------------------------------
--sec_tick_advanced_generation : process (sec_pulse_delayed, clk_i) begin
--   if clk_i'event and clk_i = '1' then
--      sec_pulse_delayed(1) <= sec_pulse_delayed(0);
--   end if;
--   if sec_pulse_delayed = "01" then   -- rising
--      sec_tick_advanced_o <= '1';
--   else
--      sec_tick_advanced_o <= '0';
--   end if;
--end process;

sec_tick_advanced_generation : process (sec_pulse_delayed, pll_ref_falling) begin
   if sec_pulse_delayed(0) = '1' and pll_ref_falling = '1' then
      sec_tick_advanced_o <= '1';
   else
      sec_tick_advanced_o <= '0';
   end if;
end process;


--------------------------------------------------------------
-- Transfer pulse (1.001 sec) from 27MHz domain to 148.5MHz domain:
--------------------------------------------------------------
read_transfer_counter : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if trans_tick_i = '1' then
         trans_count <= pll_count;
      end if;
   end if;
end process;

trans_count_o <= trans_count;


trans_pulse_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      trans_tick_delayed <= trans_tick_i;
      if trans_tick_delayed = '1' then
         trans_pulse_o <= '1';
      elsif trans_count = pll_count then
         trans_pulse_o <= '0';
      end if;
   end if;
end process;


pll_ref_debug_select : process (pll_ref_debug, pll_ref) begin
   if ref_step_debug = '1' then
      pll_ref_o <= pll_ref_debug;
   else
      pll_ref_o <= pll_ref;
   end if;
end process;


--------------------------------------------------------------
-- Genlock status:
--------------------------------------------------------------
warning_generation : process (genlock_error_count, set_error_count) begin
   if genlock_error_count = "111" then
      warning_o(1) <= '0';
   else
      warning_o(1) <= '1';
   end if;
   if set_error_count = "11" then
      warning_o(0) <= '0';
   else
      warning_o(0) <= '1';
   end if;
end process;

error_generation : process (genlock_error_count, set_error_count) begin
   if genlock_error_count = 0 then
      error_o(1) <= '1';
   else
      error_o(1) <= '0';
   end if;
   if set_error_count = 0 then
      error_o(0) <= '1';
   else
      error_o(0) <= '0';
   end if;
end process;


--------------------------------------------------------------
-- Debug: Introduce step in pll_ref_o to test pll response:
--------------------------------------------------------------
pll_ref_delaying : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      pll_ref_delayed <= pll_ref_delayed(28 downto 0) & pll_ref;
   end if;
end process;

debug_step_counting : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if sec_pulse_delayed = "01" then
         debug_step_count <= debug_step_count + 1;
      end if;
   end if;
end process;

debug_step_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if debug_step_count(0) = '1' then
         pll_ref_debug <= pll_ref_delayed(0);
      else
         pll_ref_debug <= pll_ref;
      end if;
   end if;
end process;


end Behavioral;