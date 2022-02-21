library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;

entity Tri_level_timer is
   Port (
      mreset_i          : in std_logic;      -- Master reset
      frame_start_i     : in std_logic;      -- start generator frame - aktiv høj
      clk_i             : in std_logic;      -- multiplexed 148 MHz clock
      interlaced_i      : in std_logic;      -- ( 1=interlaced, 0=progressiv )
      div10_11_i        : in std_logic;      -- divider clk_i med 10 hvis lav, 11 hvis høj
      sync_mode_lsb_i   : in std_logic;
      spl_div_i         : in std_logic_vector(9 downto 0);  -- total samples pr. line divideret med 10 (prog) eller 11 (interlaced)
      lines_pr_frame_i  : in std_logic_vector(10 downto 0);
      framerate_x2_i    : in std_logic;
      tsg_lvl_i         : in std_logic;
      mute_i            : in std_logic;
      tsg_ok_o          : out std_logic;
      DAC_o             : out std_logic_vector(3 downto 0);
      timing_change_o   : out std_logic
      --debug:
--      debug_o           : out std_logic_vector(15 downto 0) := (others => '0')
   );
end Tri_level_timer;


architecture Behavioral of Tri_level_timer is


component sync_statemachine
   Port (
      mreset_i          : in std_logic;
      frame_reset_i     : in std_logic;
      clk_i             : in std_logic;
      div10_11_i        : in std_logic;   -- 0=720 lines , 1=1080 lines
      framerate_x2_i    : in std_logic;
      interlaced_i      : in std_logic;
      line_begin_i      : in std_logic;
      line_mid_i        : in std_logic;
      make_sync_i       : in std_logic;
      two_pulses_i      : in std_logic;
      tsg_lvl_i         : in std_logic;
      tsg_ok_o          : out std_logic;
      DAC_o             : out std_logic_vector(3 downto 0)
   );
end component;


signal two_pulses                : std_logic;
signal make_sync                 : std_logic;
signal div_count                 : std_logic_vector(3 downto 0);
signal divstart_value            : std_logic_vector(3 downto 0);
signal div_tick                  : std_logic;
signal spl_div_count             : std_logic_vector(9 downto 0);
signal line_count                : integer range 0 to 2047; --std_logic_vector(10 downto 0);
signal line_begin                : std_logic;  -- hjaelpesignal 3
signal line_mid                  : std_logic;  -- hjaelpesignal 4

signal frame_reset               : std_logic;
signal frame_start               : std_logic;
signal interlaced               : std_logic;
signal div10_11                  : std_logic;
signal sync_mode_lsb             : std_logic;
signal spl_div                   : std_logic_vector(9 downto 0);
signal lines_pr_frame            : integer range 0 to 2047; --std_logic_vector(10 downto 0);
signal framerate_x2              : std_logic;
signal frame_end_tick            : std_logic;
signal frame_end_tick_delayed1   : std_logic;
signal mute                      : std_logic;


begin


------------------------------------------------------
-- Parameter Reclocking (and mute latching):
------------------------------------------------------
parameter_reclocking : process (mreset_i, clk_i) begin
	if mreset_i = '0' then
		frame_start    <= '1';
		interlaced     <= interlaced_i;
		div10_11       <= div10_11_i;
		sync_mode_lsb  <= sync_mode_lsb_i;
		spl_div        <= spl_div_i;
		lines_pr_frame <= conv_integer(lines_pr_frame_i);
		framerate_x2   <= framerate_x2_i;
		mute           <= '1';
	elsif clk_i'event and clk_i = '1' then
	   frame_start    <= frame_start_i;
	   if (frame_end_tick = '1') or (frame_start_i = '1') then
	      mute           <= mute_i;
	      interlaced     <= interlaced_i;
	      div10_11       <= div10_11_i;
	      sync_mode_lsb  <= sync_mode_lsb_i;
	      spl_div        <= spl_div_i;
	      lines_pr_frame <= conv_integer(lines_pr_frame_i);
	      framerate_x2   <= framerate_x2_i;
	   end if;
   end if;
end process;


------------------------------------------------------
-- Frame_reset (muting):
------------------------------------------------------
frame_reset <= frame_start or mute;


------------------------------------------------------
-- Generate tri-level pulses:
------------------------------------------------------
statemachine : sync_statemachine port map (
   mreset_i            => mreset_i,
   frame_reset_i       => frame_reset,
   clk_i               => clk_i,
   div10_11_i          => div10_11,
   framerate_x2_i      => framerate_x2,
   interlaced_i        => interlaced,
   line_begin_i        => line_begin,
   line_mid_i          => line_mid,
   make_sync_i         => make_sync,
   two_pulses_i        => two_pulses,
   tsg_lvl_i           => tsg_lvl_i,
   tsg_ok_o            => tsg_ok_o,
   DAC_o               => DAC_o
   );


--------------------------------------------------------------------
-- Make tick every 10 (750 total lines) or 11 (1125 total lines) cycles
--------------------------------------------------------------------
make_divstart_value : with div10_11 select
    divstart_value <= conv_std_logic_vector(11,4) when '1',
                      conv_std_logic_vector(10,4) when others;

make_div_tick : process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      div_tick <= '0';
      div_count <= divstart_value;
   elsif clk_i'event and clk_i = '1' then
      if frame_reset = '1' then
         div_tick <= '1';
         div_count <= divstart_value;
      else
         if div_count = conv_std_logic_vector(1,4) then
            div_count <= divstart_value;
            div_tick <= '1';
         else
            div_count <= div_count - 1;
            div_tick <= '0';
         end if;
      end if;
   end if;
end process;


------------------------------------------------------------------
-- Circular down counting with a line period
------------------------------------------------------------------
spl_div_counting : process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      spl_div_count <= spl_div;
   elsif clk_i'event and clk_i = '1' then
      if frame_reset = '1' then
         spl_div_count <= spl_div;
      elsif div_tick = '1' then
         if spl_div_count = conv_std_logic_vector(0,10) then
            spl_div_count <= spl_div;
         else
            spl_div_count <= spl_div_count - 1;
         end if;
      end if;
   end if;
end process;


------------------------------------------------------------------
-- Make tick at line begin and line mid:
------------------------------------------------------------------
line_begin_and_line_mid : process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      line_begin <= '0';
      line_mid <= '0';
   elsif clk_i'event and clk_i = '1' then
      if frame_reset = '1' then
         line_begin <= '0';
         line_mid <= '0';
      elsif div_tick = '1' then
         -- line_begin :
         if spl_div_count = spl_div then
            line_begin <= '1';
         else
            line_begin <= '0';
         end if;
         -- line_mid :
         if spl_div_count = '0' & spl_div(9 downto 1) then
            line_mid <= '1';
         else
            line_mid <= '0';
         end if;
      else
         line_begin <= '0';
         line_mid <= '0';
      end if;
   end if;
end process;


------------------------------------------------------------------
-- Count number of lines
------------------------------------------------------------------
line_counting : process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      line_count <= lines_pr_frame;                         -- Maybe 0
   elsif clk_i'event and clk_i = '1' then
      if frame_reset = '1' then
         line_count <= lines_pr_frame;                      -- Maybe 0
      elsif line_begin = '1' then
         if line_count = lines_pr_frame then
            line_count <= 1;                                -- Maybe 0
         else
            line_count <= line_count + 1;
         end if;
		end if;
   end if;
end process;


------------------------------------------------------------------
-- For interlaced scan there are two pulses at some lines:
------------------------------------------------------------------
make_two_pulses : process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      two_pulses <= '0';
   elsif clk_i'event and clk_i = '1' then
      if frame_reset = '1' then
         two_pulses <= '0';
      elsif interlaced = '1' then   -- interlaced
         case line_count is
            when 1 =>
               two_pulses <= '1';    -- 2 pulser/linje
            when 7 =>
               two_pulses <= '0';    -- 1 puls/linje
            when 563 =>
               two_pulses <= '1';    -- 2 pulser/linje
            when 569 =>
               two_pulses <= '0';    -- 1 puls/linje
            when others =>
               null;
         end case;
      else                           -- progressive scan :
         two_pulses <= '0';          -- one pulse on all lines
      end if;
   end if;
end process;


------------------------------------------------------------------
-- Indicate if sync pulse shall be made:
------------------------------------------------------------------
make_sync_generation : process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      make_sync <= '0';
   elsif clk_i'event and clk_i = '1' then
      if frame_reset = '1' then
         make_sync <= '0';
      else
         case line_count is
            when 1 =>               -- first field
               make_sync <= '1';    -- make sync
            when 6 =>
               make_sync <= '0';    -- don't make sync
            when 563 =>             -- second field for interlaced
               if interlaced = '1' and line_mid = '1' then
                  make_sync <= '1';
               end if;
            when 568 =>
               if interlaced = '1' and line_mid = '1' then
                  make_sync <= '0';
               end if;
            when others =>
               null;
         end case;
      end if;
   end if;
end process;


------------------------------------------------------------------
-- Generate tick at end of frame:
-- The tick occurs 3 clk's before the line_begin tick
------------------------------------------------------------------
frame_end_ticking: process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      frame_end_tick <= '0';
      frame_end_tick_delayed1 <= '0';
   elsif clk_i'event and clk_i = '1' then
      if div_count = "0011" and spl_div_count = spl_div and line_count = lines_pr_frame then
         frame_end_tick <= '1';
      else
         frame_end_tick <= '0';
      end if;
      frame_end_tick_delayed1 <= frame_end_tick;
   end if;
end process;


------------------------------------------------------------------
-- Indicate if timing is changed:
------------------------------------------------------------------
timing_change_monitoring: process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      timing_change_o <= '0';
   elsif clk_i'event and clk_i = '1' then
      if (frame_end_tick_delayed1 = '1' and frame_start = '1') or (mute = '1') then
         timing_change_o <= '0';
      elsif frame_reset = '1' and frame_end_tick_delayed1 = '0' then
         timing_change_o <= '1';
      end if;
   end if;
end process;

------------------------------------------------------------------
-- Debug:
------------------------------------------------------------------
--debug_o(0) <= frame_reset;
--debug_o(1) <= line_begin;
--debug_o(2) <= line_mid;
--debug_o(3) <= two_pulses;
--debug_o(4) <= make_sync;
--debug_o(5) <= framerate_x2;
--debug_o(8) <= frame_end_tick;
--debug_o(9) <= mute;
--debug_o(10) <= div_tick;
--debug_o(14) <= frame_reset;

end Behavioral;