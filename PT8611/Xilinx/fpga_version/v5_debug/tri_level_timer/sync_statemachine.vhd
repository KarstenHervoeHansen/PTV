library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity sync_statemachine is
   Port (  
      mreset_i          : in std_logic;
      frame_reset_i     : in std_logic;
      clk_i             : in std_logic;
      div10_11_i        : in std_logic;   -- 0: 720 lines , 1: 1080 lines
      framerate_x2_i    : in std_logic;
      interlaced_i      : in std_logic;   -- 0: P, 1: I
      line_begin_i      : in std_logic;
      line_mid_i        : in std_logic;
      make_sync_i       : in std_logic;
      two_pulses_i      : in std_logic;
      tsg_lvl_i         : in std_logic;
      tsg_ok_o          : out std_logic;
      DAC_o             : out std_logic_vector(3 downto 0)
   );
end sync_statemachine;


architecture Behavioral of sync_statemachine is


component analog_levels
   Port(              
      mreset_i          : in std_logic;
      frame_reset_i     : in std_logic;
      clk_i             : in std_logic;
      sync_zero_i       : in std_logic;
      sync_polarity_i   : in std_logic;
      DAC_o             : out std_logic_vector(3 downto 0)
   );
end component;


type state_type is ( zero1, neg_pulse, pos_pulse, zero2, sync_pulse, zero3);
signal state            : state_type;
signal state_count      : std_logic_vector(11 downto 0);
signal sync_zero        : std_logic;
signal sync_polarity    : std_logic;
signal tsg_lvl_reclock  : std_logic;
signal zero1_dur        : std_logic_vector(11 downto 0);
signal neg_dur          : std_logic_vector(11 downto 0);
signal pos_dur          : std_logic_vector(11 downto 0);
signal zero2_dur        : std_logic_vector(11 downto 0);
signal sync_dur         : std_logic_vector(11 downto 0);


begin


dac : analog_levels
   port map (
      mreset_i          => mreset_i,
      frame_reset_i     => frame_reset_i,
      clk_i             => clk_i,     
      sync_zero_i       => sync_zero,     
      sync_polarity_i   => sync_polarity,   
      DAC_o             => DAC_o   
   );


--                 pos_pulse
--                  ------                                                                 
--                 /      \  zero2                      zero3                           
-- -------        /        ---------                   -------|-------        /
--  zero1 \      /                  \                 /         zero1 \      /
--         ------                    -----------------                 ------
--        neg_pulse                      sync_pulse


------------------------------------------------------------------------------
-- Generation of durations:
-- 1xsamplerate: zero1_dur + neg_dur = 173
-- 2xsamplerate: zero1_dur + neg_dur = 86
-- The different delays in x1 and x2 modes are compensated in frame_sync_delay
------------------------------------------------------------------------------
duration_generation : process(mreset_i, clk_i )
begin
   if clk_i'event and clk_i = '1' then
      if div10_11_i = '0' then                        -- system D
         zero1_dur <= conv_std_logic_vector(2*48-1,12);
         neg_dur   <= conv_std_logic_vector(2*39,12);--(2*39,12);
         pos_dur   <= conv_std_logic_vector(2*41,12);
         zero2_dur <= conv_std_logic_vector(2*223,12);
         sync_dur  <= conv_std_logic_vector(2*1274,12);
      elsif interlaced_i = '1' then                  -- system B or C
         zero1_dur <= conv_std_logic_vector(2*44-1,12);
         neg_dur   <= conv_std_logic_vector(2*43,12);
         pos_dur   <= conv_std_logic_vector(2*45,12);
         zero2_dur <= conv_std_logic_vector(2*88,12);
         sync_dur  <= conv_std_logic_vector(2*880,12);
      elsif framerate_x2_i = '1' then                -- system A and 2xsamplerate
         zero1_dur <= conv_std_logic_vector(44-1,12);
         neg_dur   <= conv_std_logic_vector(43,12);
         pos_dur   <= conv_std_logic_vector(45,12);
         zero2_dur <= conv_std_logic_vector(88,12);
         sync_dur  <= conv_std_logic_vector(1980,12);      
      else                                           -- system A and 1xsamplerate
         zero1_dur <= conv_std_logic_vector(2*44-1,12);
         neg_dur   <= conv_std_logic_vector(2*43,12);
         pos_dur   <= conv_std_logic_vector(2*45,12);
         zero2_dur <= conv_std_logic_vector(2*88,12);
         sync_dur  <= conv_std_logic_vector(2*1980,12);      
      end if;
   end if;
end process;
--         zero1_dur <= conv_std_logic_vector(44-1,12);      
--         neg_dur   <= conv_std_logic_vector(43,12);        
--         pos_dur   <= conv_std_logic_vector(45,12);        
--         zero2_dur <= conv_std_logic_vector(88,12);        
--         sync_dur  <= conv_std_logic_vector(1980,12);      
--

make_sync_sm : process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      state <= zero1;
      state_count <= zero1_dur;
   elsif clk_i'event and clk_i = '1' then
      if frame_reset_i = '1' then
         state <= zero1;
         state_count <= zero1_dur;
      else
         case state is
            when zero1 =>
               if state_count = conv_std_logic_vector(1,12) then
                  state <= neg_pulse;
                  state_count <= neg_dur;
               else
                  state_count <= state_count - 1;
               end if;
            when neg_pulse =>
               if state_count = conv_std_logic_vector(1,12) then
                  state <= pos_pulse;
                  state_count <= pos_dur;
               else
                  state_count <= state_count - 1;
               end if;
            when pos_pulse =>
               if state_count = conv_std_logic_vector(1,12) then
                  state <= zero2;
                  state_count <= zero2_dur;
               else
                  state_count <= state_count - 1;
               end if;
            when zero2 =>
               if state_count = conv_std_logic_vector(1,12) then
                  state <= sync_pulse;
                  state_count <= sync_dur;
               else
                  state_count <= state_count - 1;
               end if;
            when sync_pulse =>
               if state_count = conv_std_logic_vector(1,12) then
                  state <= zero3;
               else
                  state_count <= state_count - 1;
               end if;
            when zero3 =>
               if line_begin_i = '1' or ( line_mid_i = '1' and two_pulses_i = '1' ) then
                  state <= zero1;
                  state_count <= zero1_dur;
               else
                  null;
               end if;
            when others =>
               null;
         end case;
      end if;
   end if;
end process;


-------------------------------------------------------------------
-- Generate sync_zero and sync_polarity from state
-------------------------------------------------------------------
sync_zero_generation : process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      sync_zero <= '1';
   elsif clk_i'event and clk_i = '1' then
      if frame_reset_i = '1' then
         sync_zero <= '1';
      else
         case state is
            when neg_pulse =>
               sync_zero <= '0';
            when pos_pulse =>
               sync_zero <= '0';
            when zero2 =>
               sync_zero <= '1';
            when sync_pulse =>
               if make_sync_i = '1' then
                  sync_zero <= '0';
               end if;
            when zero3 =>
               sync_zero <= '1';
            when others =>
               null;
         end case;
      end if;
   end if;                
end process;


sync_polarity_generation : process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      sync_polarity <= '0';
   elsif clk_i'event and clk_i = '1' then
      if frame_reset_i = '1' then
         sync_polarity <= '0';
      else
         case state is
            when pos_pulse =>
               sync_polarity <= '1';
            when zero2 =>
               sync_polarity <= '0';
            when others =>
               null;
         end case;
      end if;
   end if;                
end process;


-------------------------------------------------------------------
-- Sampling of output level at the end of pos_pulse state 
-------------------------------------------------------------------
tsg_lvl_reclocking : process(mreset_i, clk_i )
begin
   if mreset_i = '0' then
      tsg_lvl_reclock <= '0';
   elsif clk_i'event and clk_i = '1' then
      tsg_lvl_reclock <= tsg_lvl_i;
   end if;
end process;


tsg_lvl_sampling : process(mreset_i, clk_i )
begin
   if mreset_i = '0' then
      tsg_ok_o <= '0';
   elsif clk_i'event and clk_i = '1' then
      if state = pos_pulse and state_count = conv_std_logic_vector(1,12) then
         tsg_ok_o <= tsg_lvl_reclock;  
      end if;
   end if;
end process;


end Behavioral;