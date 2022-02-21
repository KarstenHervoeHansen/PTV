library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;


entity channel_controller is
   generic (
      pc_enable_mode          : std_logic
   );
   port (
      sync_reset_i            : in std_logic;
      clk_i                   : in std_logic;
      en_i                    : in std_logic;
      tick_10ms_i             : in std_logic;
      video_clk_i             : in std_logic;
      video_en_i              : in std_logic;
      ui_system_i             : in std_logic_vector(11 downto 0);
      ui_pattern_i            : in std_logic_vector(15 downto 0);
      ui_timing_i             : in std_logic_vector(23 downto 0);
      -- genlock sync reference signals:
      f4m_1484_i              : in std_logic;      -- sync reference - HD systems
      f4m_1485_i              : in std_logic;      -- sync reference - SD NTSC
      f8g_i                   : in std_logic;      -- sync reference
      f2398_i                 : in std_logic;
      f24_i                   : in std_logic;
      f30_i                   : in std_logic;
      -- other input signals:
      frame_end_i             : in std_logic;
      rio_fifo_error_i        : in std_logic;
      -- parameters:
      hd_zsd_o                : out std_logic;
      use_f1485_o             : out std_logic;
      system_o                : out std_logic_vector(11 downto 0);
      pattern_o               : out std_logic_vector(15 downto 0);
      -- reset signals:
      sync_reset_frame_o      : out std_logic;
      sync_reset_mgt_o        : out std_logic;
      mgt_power_down_o        : out std_logic
   );
end channel_controller;


architecture Behavioral of channel_controller is


type statetype is (reset, unreseting, powering, unzeroing, unblacking, running, blacking, zeroing, downpowering);
signal state                  : statetype;
type frame_reset_state_type is (wait_for_reset, reset, unreseting, video_running, resyncing);
signal frame_reset_state      : frame_reset_state_type;
signal mgt_reset_state        : frame_reset_state_type;

signal state_count            : std_logic_vector(7 downto 0);
signal actual_system          : std_logic_vector(11 downto 0);
signal actual_pattern         : std_logic_vector(15 downto 0);
signal MGT_power_down         : std_logic;
signal reset_video            : std_logic;
signal reset_video_clk        : std_logic;
signal frame_start            : std_logic;
signal sync_reset_frame       : std_logic;
signal spl_index              : std_logic_vector(1 downto 0);


signal f4m_1484               : std_logic;
signal f4m_1485               : std_logic;
signal f8g                    : std_logic;
signal f2398                  : std_logic;
signal f24                    : std_logic;
signal f30                    : std_logic;
signal genlock_sync           : std_logic;

signal frame_start_delayed_1  : std_logic;
signal frame_start_delayed_2  : std_logic;
signal sync_reset_advanced_1  : std_logic;

constant black_pattern        : std_logic_vector(15 downto 0) := X"0900";
constant off_system           : std_logic_vector(11 downto 0) := "000011110000";

attribute uselowskewlines     : string;
attribute uselowskewlines of sync_reset_frame_o : signal is "yes";


component frame_sync_delay
   port (
	   mreset_i       : in std_logic;                     -- Master reset
	   clk_i         	: in std_logic;                     -- sample clock
	   sync_i         : in std_logic;
      timing_i       : in std_logic_vector(23 downto 0); -- delay from sync reference
	                                                      -- Input parameters must always be valid
	   frame_sync_o	: out std_logic
	);
end component;


begin


-------------------------------------------------------------
-- Select genlock sync source:
-------------------------------------------------------------
sync_reclocking : process(video_clk_i)
begin
   if video_clk_i'event and video_clk_i = '1' then
      f4m_1484 <= f4m_1484_i;
      f4m_1485 <= f4m_1485_i;
      f8g      <= f8g_i;
      f2398    <= f2398_i;
      f24      <= f24_i;
      f30      <= f30_i;
   end if;
end process;


sync_selecting : process(video_clk_i)
begin
   if video_clk_i'event and video_clk_i = '1' then
      case actual_system(11 downto 4) is
         when X"A0"  => genlock_sync <= f30;          -- 1080i/30        
         when X"B0"  => genlock_sync <= f4m_1484;     -- 1080i/29.97     
         when X"82"  => genlock_sync <= f30;          -- 1080p/30        
         when X"92"  => genlock_sync <= f4m_1484;     -- 1080p/29.97     
         when X"83"  => genlock_sync <= f8g;          -- 1080p/25        
         when X"84"  => genlock_sync <= f24;          -- 1080p/24        
         when X"94"  => genlock_sync <= f2398;        -- 1080p/23.98     
         when X"A2"  => genlock_sync <= f30;          -- 1080sF/30       
         when X"B2"  => genlock_sync <= f4m_1484;     -- 1080sF/29.97    
         when X"A3"  => genlock_sync <= f8g;          -- 1080sF/25       
         when X"A4"  => genlock_sync <= f24;          -- 1080sF/24       
         when X"B4"  => genlock_sync <= f2398;        -- 1080sF/23.98    
         when X"86"  => genlock_sync <= f30;          -- 720p/60         
         when X"96"  => genlock_sync <= f4m_1484;     -- 720p/59.94      
         when X"87"  => genlock_sync <= f8g;          -- 720p/50         
         when X"88"  => genlock_sync <= f30;          -- 720p/30         
         when X"98"  => genlock_sync <= f4m_1484;     -- 720p/29.97      
         when X"89"  => genlock_sync <= f8g;          -- 720p/25         
         when X"8A"  => genlock_sync <= f24;          -- 720p/24         
         when X"9A"  => genlock_sync <= f2398;        -- 720p/23.98      
         when X"60"  => genlock_sync <= f4m_1485;     -- NTSC 525i/29.97 
         when X"20"  => genlock_sync <= f8g;          -- PAL 625i/25     
         when others => genlock_sync <= f8g;          -- 1080i/25   X"A1"
      end case; 
   end if;      
end process;    

-------------------------------------------------------
-- delay timing reference:
-------------------------------------------------------
frame_sync_delaying : frame_sync_delay
   port map (
      mreset_i          => '1',
      clk_i             => video_clk_i,
      sync_i            => genlock_sync,
      timing_i          => ui_timing_i,
      frame_sync_o      => frame_start
   );


---------------------------------------------------------------
-- syncronize the video generator to genlock timing by doing a
-- reset_video at the right time:
---------------------------------------------------------------
frame_start_delayed_generation : process (video_clk_i)
begin
   if video_clk_i'event and video_clk_i = '1' then
      frame_start_delayed_1 <= frame_start;
      frame_start_delayed_2 <= frame_start_delayed_1;
   end if;
end process;


frame_reset_sm : process (video_clk_i)
begin
   if video_clk_i'event and video_clk_i = '1' then
      reset_video_clk <= reset_video;
      if sync_reset_i = '1' then
         frame_reset_state <= reset;
         sync_reset_frame  <= '1';
      else
         case frame_reset_state is
         when wait_for_reset =>
            if reset_video_clk = '0' then                -- interupt reset process and start unreseting
               frame_reset_state <= unreseting;
            elsif (frame_end_i = '1' and video_en_i = '1') or frame_start_delayed_2 = '1' then
               sync_reset_frame  <= '1';                 -- reset at framestart
               frame_reset_state <= reset;
            end if;
         when reset =>
            sync_reset_frame  <= '1';
            if reset_video_clk = '0' then
               frame_reset_state <= resyncing;
            end if;
         when unreseting =>
            if frame_start_delayed_2 = '1' then
               sync_reset_frame  <= '0';                 -- unreset at framestart
               frame_reset_state <= video_running;
            end if;
         when resyncing =>
            if frame_start_delayed_1 = '1' then
               sync_reset_frame  <= '1';                 -- resync
               frame_reset_state <= video_running;
            else
               sync_reset_frame  <= '0';
            end if;
         when others =>    -- video_running
            sync_reset_frame  <= '0';
            if reset_video_clk = '1' then
               frame_reset_state <= wait_for_reset;
            elsif sync_reset_frame = '0' and (frame_start_delayed_2 = '1' and (frame_end_i = '0' or video_en_i = '0')) then     -- out of syncronization:
               if pc_enable_mode = '0' then
                  frame_reset_state <= resyncing;        -- normal mode
               else
                  frame_reset_state <= video_running;    -- debug mode
               end if;
            else 
               frame_reset_state <= video_running;
            end if;
         end case;
      end if;
   end if;
end process;

sync_reset_frame_o <= sync_reset_frame;


mgt_reset_sm : process (video_clk_i)
begin
   if video_clk_i'event and video_clk_i = '1' then
      if sync_reset_i = '1' then
         mgt_reset_state   <= reset;
         sync_reset_mgt_o  <= '1';
      else
         case mgt_reset_state is
         when wait_for_reset =>
            if reset_video_clk = '0' then                -- interupt reset process and start unreseting
               mgt_reset_state <= unreseting;
            elsif (frame_end_i = '1' and video_en_i = '1') or frame_start_delayed_2 = '1' then
               sync_reset_mgt_o  <= '1';                 -- reset at framestart
               mgt_reset_state   <= reset;
            end if;
         when reset =>
            sync_reset_mgt_o   <= '1';
            if reset_video_clk = '0' then
               mgt_reset_state <= resyncing;
            end if;
         when unreseting =>
            if frame_start_delayed_2 = '1' then
               sync_reset_mgt_o  <= '0';                 -- unreset at framestart
               mgt_reset_state   <= video_running;
            end if;
         when resyncing =>
            if frame_start_delayed_1 = '1' then
               sync_reset_mgt_o  <= '1';                 -- resync
               mgt_reset_state   <= video_running;
            else
               sync_reset_mgt_o  <= '0';
            end if;
         when others =>    -- video_running
            sync_reset_mgt_o   <= '0';
            if reset_video_clk = '1' then
               mgt_reset_state <= wait_for_reset;
            elsif rio_fifo_error_i = '1' then            -- out of syncronization:
               mgt_reset_state <= resyncing;
            else 
               mgt_reset_state <= video_running;
            end if;
         end case;
      end if;
   end if;
end process;


-------------------------------------------------------
-- state machine for controlling of sync_reset_mgt, reset, and
-- actual system, pattern and timing parameters
-------------------------------------------------------
controller_sm : process (clk_i) begin
   if clk_i'event and clk_i ='1' then
      if sync_reset_i = '1' then
         state       <= reset;
         state_count <= conv_std_logic_vector(6,8);
      elsif en_i = '1' then
         case state is
            when running =>
               if actual_system /= ui_system_i then
                  state_count <= conv_std_logic_vector(6,8);
                  state       <= blacking;
               end if;
            when blacking =>
               if tick_10ms_i = '1' then
                  if state_count = conv_std_logic_vector(0,8) then
                     state       <= zeroing;
                     state_count <= conv_std_logic_vector(6,8);
                  else
                     state_count <= state_count - 1;
                  end if;
               end if;
            when zeroing =>
               if tick_10ms_i = '1' then
                  if state_count = conv_std_logic_vector(0,8) then
                     state       <= downpowering;
                     state_count <= conv_std_logic_vector(1,8);
                  else
                     state_count <= state_count - 1;
                  end if;
               end if;
            when downpowering =>
               if tick_10ms_i = '1' then
                  if state_count = conv_std_logic_vector(0,8) then
                     state       <= reset;
                  else
                     state_count <= state_count - 1;
                  end if;
               end if;
            when reset =>
               if ui_system_i(11 downto 4) /= off_system(11 downto 4) then
                  state       <= unreseting;
                  state_count <= conv_std_logic_vector(1,8);
               end if;
            when unreseting =>
               if tick_10ms_i = '1' then
                  if state_count = conv_std_logic_vector(0,8) then
                     state <= powering;
                     state_count <= conv_std_logic_vector(12,8);
                  else
                     state_count <= state_count - 1;
                  end if;
               end if;
            when powering =>
               if tick_10ms_i = '1' then
                  if state_count = conv_std_logic_vector(0,8) then
                     state       <= unzeroing;
                     state_count <= conv_std_logic_vector(12,8);
                  else
                     state_count <= state_count - 1;
                  end if;
               end if;
            when unzeroing =>
               if tick_10ms_i = '1' then
                  if state_count = conv_std_logic_vector(0,8) then
                     state <= running;
                  else
                     state_count <= state_count - 1;
                  end if;
               end if;
            when others =>
               null;
         end case;
      end if;
   end if;
end process;


sm_outputs : process (clk_i) begin
   if clk_i'event and clk_i ='1' then
      if sync_reset_i = '1' then
         reset_video       <= '1';
         MGT_power_down    <= '1';
         actual_pattern    <= black_pattern;
         actual_system     <= ui_system_i;
      elsif en_i = '1' then
         case state is
         when blacking =>
            actual_pattern    <= black_pattern;
            MGT_power_down    <= '0';
            reset_video       <= '0';
         when zeroing =>
            actual_pattern    <= black_pattern;
            MGT_power_down    <= '0';
            reset_video       <= '1';
         when downpowering =>
            actual_pattern    <= black_pattern;
            MGT_power_down    <= '1';
            reset_video       <= '1';
         when reset =>
            actual_pattern    <= black_pattern;
            MGT_power_down    <= '1';
            reset_video       <= '1';
         when unreseting =>
            actual_pattern    <= black_pattern;
            actual_system     <= ui_system_i;
            MGT_power_down    <= '1';
            reset_video       <= '1';
         when powering =>
            actual_pattern    <= black_pattern;
            MGT_power_down    <= '0';
            reset_video       <= '1';
         when unzeroing =>
            actual_pattern    <= black_pattern;
            MGT_power_down    <= '0';
            reset_video       <= '0';
         when others =>                         -- running
            actual_pattern    <= ui_pattern_i;
            MGT_power_down    <= '0';
            reset_video       <= '0';
         end case;
      end if;
   end if;
end process;


------------------------------------------------------------
-- Signal reclocking to video_clk_i clock domain:
------------------------------------------------------------
video_clk_reclocking: process (video_clk_i) begin
   if video_clk_i'event and video_clk_i ='1' then
      MGT_power_down_o        <= MGT_power_down;
   end if;
end process;


---------------------------------------------------------------
-- samples per line dependent on system :
-- 720: "00", 1280: "01", 1920: "10"
---------------------------------------------------------------
with actual_system(11 downto 4) select
   spl_index <=      "10" when X"A0",   -- 1080i/30
                     "10" when X"B0",   -- 1080i/29.97
                     "10" when X"82",   -- 1080p/30
                     "10" when X"92",   -- 1080p/29.97
                     "10" when X"83",   -- 1080p/25
                     "10" when X"84",   -- 1080p/24
                     "10" when X"94",   -- 1080p/23.98
                     "10" when X"A2",   -- 1080sF/30
                     "10" when X"B2",   -- 1080sF/29.97
                     "10" when X"A3",   -- 1080sF/25
                     "10" when X"A4",   -- 1080sF/24
                     "10" when X"B4",   -- 1080sF/23.98
                     "01" when X"86",   -- 720p/60
                     "01" when X"96",   -- 720p/59.94
                     "01" when X"87",   -- 720p/50
                     "01" when X"88",   -- 720p/30
                     "01" when X"98",   -- 720p/29.97
                     "01" when X"89",   -- 720p/25
                     "01" when X"8A",   -- 720p/24
                     "01" when X"9A",   -- 720p/23.98
                     "00" when X"60",   -- NTSC 525i/29.97
                     "00" when X"20",   -- PAL 625i/25
                     "10" when others;  -- 1080i/25   X"A1"


with spl_index select
   hd_zsd_o       <= '0'  when "00",    -- 720
                     '1'  when "01",    -- 1280
                     '1'  when others;  -- 1920


with actual_system(11 downto 4) select
   use_f1485_o <=     '1' when X"A0",   -- 1080i/30
                      '0' when X"B0",   -- 1080i/29.97
                      '1' when X"82",   -- 1080p/30
                      '0' when X"92",   -- 1080p/29.97
                      '1' when X"83",   -- 1080p/25
                      '1' when X"84",   -- 1080p/24
                      '0' when X"94",   -- 1080p/23.98
                      '1' when X"A2",   -- 1080sF/30
                      '0' when X"B2",   -- 1080sF/29.97
                      '1' when X"A3",   -- 1080sF/25
                      '1' when X"A4",   -- 1080sF/24
                      '0' when X"B4",   -- 1080sF/23.98
                      '1' when X"86",   -- 720p/60
                      '0' when X"96",   -- 720p/59.94
                      '1' when X"87",   -- 720p/50
                      '1' when X"88",   -- 720p/30
                      '0' when X"98",   -- 720p/29.97
                      '1' when X"89",   -- 720p/25
                      '1' when X"8A",   -- 720p/24
                      '0' when X"9A",   -- 720p/23.98
                      '1' when X"60",   -- NTSC 525i/29.97
                      '1' when X"20",   -- PAL 625i/25
                      '1' when others;  -- 1080i/25   X"A1"
                     
                     
------------------------------------------------------------
-- Mapping of system parameters:
------------------------------------------------------------
system_o    <= actual_system;
pattern_o   <= actual_pattern;


end Behavioral;