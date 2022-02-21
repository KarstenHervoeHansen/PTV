library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;


entity video_generator is
generic (
   bypass_scrambler     : std_logic;
   timing_dummy_data    : std_logic
);
port (
   clk_i                : in std_logic;    
   zreset_i				: in std_logic;
   en_i                 : in std_logic;
   sync_reset_i         : in std_logic;

   -- video parameters:
   system_i             : in std_logic_vector(11 downto 0);
   pattern_i            : in std_logic_vector(15 downto 0);

   -- audio parameters:
   audio_level_index_i  : in std_logic_vector(7 downto 0);
   audio_signal_index_i : in std_logic_vector(7 downto 0);
   click_timing_i       : in std_logic_vector(11 downto 0);
   click_modulation_i   : in std_logic;

   -- text:
   v_pos_i              : in std_logic_vector(3 downto 0);
   h_pos_i              : in std_logic_vector(3 downto 0);
   bmp_heigth_i         : in std_logic_vector(10 downto 0);    -- must be >= 1
   bmp_width_i          : in std_logic_vector(10 downto 0);    -- must be >= 1
   text_on_i            : in std_logic;
   text_color_presence_i: in std_logic_vector(2 downto 0);
   back_color_presence_i: in std_logic_vector(2 downto 0);
   text_size_i          : in std_logic_vector(1 downto 0);
   movement_i           : in std_logic_vector(1 downto 0);     -- "00": no movement, "01": vertical, "10": horizontal
   text_ram_data_i      : in std_logic_vector(7 downto 0);
   text_ram_addr_o      : out std_logic_vector(8 downto 0);
   text_ram_enable_o    : out std_logic;

   -- parallel video output:
   data_a_o             : out std_logic_vector(19 downto 0);
   data_b_o             : out std_logic_vector(19 downto 0);

   -- timing status:
   frame_end_o          : out std_logic;
   TP					: out std_logic_vector(3 downto 0);
   
   ddr_chroma_i         : in std_logic_vector(9 downto 0);
   ddr_luma_i			: in std_logic_vector(9 downto 0);
   fvh_o                : out std_logic_vector(2 downto 0)
                      
);
end video_generator;


architecture Behavioral of video_generator is


signal rio_fifo_err_a         : std_logic;
signal rio_fifo_err_b         : std_logic;
signal interlaced_transport   : std_logic;
signal ycbcr_zrgb             : std_logic;
signal sav_tick_advanced      : std_logic;
signal eav_tick_advanced      : std_logic;
signal frame_end              : std_logic;
signal last_av_sample         : std_logic;
signal lower_zupper           : std_logic;
signal max_open_pulse         : std_logic;
signal max_open_field         : std_logic;
signal audio_click            : std_logic;
signal spl_index              : std_logic_vector(1 downto 0);


signal h_state_cbar           : std_logic_vector(2 downto 0);
signal fvh                    : std_logic_vector(2 downto 0);
signal av_line                : std_logic_vector(10 downto 0);
signal av_sample              : std_logic_vector(10 downto 0);
signal line                   : std_logic_vector(10 downto 0);
signal insert_text            : std_logic;
signal anc_space              : std_logic;

signal v_grid_state           : std_logic_vector(3 downto 0);
signal h_grid_state           : std_logic_vector(3 downto 0);

signal color_presence         : std_logic_vector(2 downto 0);
signal color_level            : std_logic_vector(4 downto 0);
signal color_presence_cbar    : std_logic_vector(2 downto 0);
signal color_level_cbar       : std_logic_vector(4 downto 0);
signal color_presence_cbrd    : std_logic_vector(2 downto 0);
signal color_level_cbrd       : std_logic_vector(4 downto 0);
signal color_presence_window  : std_logic_vector(2 downto 0);
signal color_level_window     : std_logic_vector(4 downto 0);
signal color_presence_pluge   : std_logic_vector(2 downto 0);
signal color_level_pluge      : std_logic_vector(4 downto 0);
signal color_presence_text    : std_logic_vector(2 downto 0);
signal color_level_text       : std_logic_vector(4 downto 0);
signal color_presence_crosshatch : std_logic_vector(2 downto 0);
signal color_level_crosshatch : std_logic_vector(4 downto 0);

signal pattern                : std_logic_vector(15 downto 0);

signal luma_system_a          : std_logic_vector(9 downto 0);
signal chroma_system_a        : std_logic_vector(9 downto 0);
signal luma_system_b          : std_logic_vector(9 downto 0);
signal chroma_system_b        : std_logic_vector(9 downto 0);
signal luma_a                 : std_logic_vector(9 downto 0);
signal chroma_a               : std_logic_vector(9 downto 0);
signal luma_b                 : std_logic_vector(9 downto 0);
signal chroma_b               : std_logic_vector(9 downto 0);

signal y_av_unfilt            : std_logic_vector(11 downto 0);
signal cb_av_unfilt           : std_logic_vector(11 downto 0);
signal cr_av_unfilt           : std_logic_vector(11 downto 0);

signal y_av_ramp              : std_logic_vector(11 downto 0);
signal y_av_clut              : std_logic_vector(11 downto 0);
signal cb_av_clut             : std_logic_vector(11 downto 0);
signal cr_av_clut             : std_logic_vector(11 downto 0);
signal cr_zcb                 : std_logic;
signal filter_force_black     : std_logic;

signal luma_check             : std_logic_vector(9 downto 0);
signal chroma_check           : std_logic_vector(9 downto 0);
signal luma_av_a              : std_logic_vector(9 downto 0);
signal chroma_av_a            : std_logic_vector(9 downto 0);
signal luma_av_fir_a          : std_logic_vector(9 downto 0);
signal chroma_av_fir_a        : std_logic_vector(9 downto 0);
signal luma_av_legal_a        : std_logic_vector(9 downto 0);
signal chroma_av_legal_a      : std_logic_vector(9 downto 0);
signal luma_av_b              : std_logic_vector(9 downto 0);
signal chroma_av_b            : std_logic_vector(9 downto 0);
signal luma_av_fir_b          : std_logic_vector(9 downto 0);
signal chroma_av_fir_b        : std_logic_vector(9 downto 0);
signal luma_av_legal_b        : std_logic_vector(9 downto 0);
signal chroma_av_legal_b      : std_logic_vector(9 downto 0);
signal luma_av_debug          : std_logic_vector(9 downto 0);

signal luma_video_a           : std_logic_vector(9 downto 0);
signal chroma_video_a         : std_logic_vector(9 downto 0);
signal luma_h_blank_a         : std_logic_vector(9 downto 0);
signal chroma_h_blank_a       : std_logic_vector(9 downto 0);
signal luma_v_blank_a         : std_logic_vector(9 downto 0);
signal chroma_v_blank_a       : std_logic_vector(9 downto 0);
signal luma_video_b           : std_logic_vector(9 downto 0);
signal chroma_video_b         : std_logic_vector(9 downto 0);
signal luma_h_blank_b         : std_logic_vector(9 downto 0);
signal chroma_h_blank_b       : std_logic_vector(9 downto 0);
signal luma_v_blank_b         : std_logic_vector(9 downto 0);
signal chroma_v_blank_b       : std_logic_vector(9 downto 0);

signal index_mux_select       : std_logic_vector(2 downto 0);
signal unfilt_mux_select      : std_logic;
signal av_mux_select          : std_logic;

signal scramb_data_a          : std_logic_vector(19 downto 0);
signal os_data_a              : std_logic_vector(19 downto 0);
signal scramb_data_b          : std_logic_vector(19 downto 0);
signal os_data_b              : std_logic_vector(19 downto 0);
signal debug_count            : std_logic_vector(5 downto 0);
signal debug_tick             : std_logic;

signal y_debug                : std_logic_vector(9 downto 0);
signal cb_debug               : std_logic_vector(9 downto 0);
signal cr_debug               : std_logic_vector(9 downto 0);

signal chroma_audio		      : std_logic_vector(9 downto 0);
signal luma_audio		         : std_logic_vector(9 downto 0);

signal audio_sample_tick      : std_logic;
signal audio_data             : std_logic_vector(23 downto 0);

signal noise_1                : std_logic_vector(9 downto 0);
signal noise_2                : std_logic_vector(9 downto 0);



component payload_generator
   port (
      clk_i                : in  std_logic;
      en_i                 : in  std_logic;
      system_i             : in  std_logic_vector(11 downto 0);
      line_i               : in  std_logic_vector(10 downto 0);
      anc_space_i          : in  std_logic;
      data_a_i             : in  std_logic_vector(9 downto 0);
      data_b_i             : in  std_logic_vector(9 downto 0);
      anc_space_o          : out std_logic;
      data_a_o             : out std_logic_vector(9 downto 0);
      data_b_o             : out std_logic_vector(9 downto 0)
   );
end component;


component luma_chroma_fir_12bit
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;
      system_i             : in std_logic_vector(11 downto 0);
      cr_zcb_i             : in std_logic;
      force_black_i        : in std_logic;
      y_i                  : in std_logic_vector(11 downto 0);
      cb_i                 : in std_logic_vector(11 downto 0);
      cr_i                 : in std_logic_vector(11 downto 0);
      luma_a_o             : out std_logic_vector(9 downto 0);
      chroma_a_o           : out std_logic_vector(9 downto 0);
      luma_b_o             : out std_logic_vector(9 downto 0);
      chroma_b_o           : out std_logic_vector(9 downto 0)
   );
end component;


COMPONENT scram20_top
   PORT (
      clk_i        : IN std_logic;
      zreset_i     : IN std_logic;                       -- active low reset
      bypass_i     : IN std_logic;                       -- bypass scrambler
      enable_i     : IN std_logic;
      luma_i       : IN std_logic_vector(9 downto 0);    -- raw luma data in
      chroma_i     : IN std_logic_vector(9 downto 0);    -- raw chroma data in
      data_o       : out std_logic_vector(19 downto 0)   -- parallel scrambled output
   );
END COMPONENT;


component system_generator
   port (
      clk_i                   : in std_logic;
      en_i                    : in std_logic;
      sync_reset_i            : in std_logic;
                              
      -- system parameters:   
      system_i                : in std_logic_vector(7 downto 0);

      -- parallel data inputs (video and ancillary)
      luma_video_a_i          : in std_logic_vector(9 downto 0);
      chroma_video_a_i        : in std_logic_vector(9 downto 0);
      luma_h_blank_a_i        : in std_logic_vector(9 downto 0);
      chroma_h_blank_a_i      : in std_logic_vector(9 downto 0);
      luma_v_blank_a_i        : in std_logic_vector(9 downto 0);
      chroma_v_blank_a_i      : in std_logic_vector(9 downto 0);
      luma_video_b_i          : in std_logic_vector(9 downto 0);
      chroma_video_b_i        : in std_logic_vector(9 downto 0);
      luma_h_blank_b_i        : in std_logic_vector(9 downto 0);
      chroma_h_blank_b_i      : in std_logic_vector(9 downto 0);
      luma_v_blank_b_i        : in std_logic_vector(9 downto 0);
      chroma_v_blank_b_i      : in std_logic_vector(9 downto 0);

      ---system parameter outputs:
      interlaced_transport_o  : out std_logic;

      -- frame control signals:
      last_av_sample_o        : out std_logic;
      av_line_o               : out std_logic_vector(10 downto 0);
      av_sample_o             : out std_logic_vector(10 downto 0);
      line_o                  : out std_logic_vector(10 downto 0);
      fvh_o                   : out std_logic_vector(2 downto 0);    -- blanking bits
      cr_zcb_o                : out std_logic;
      sav_tick_advanced_o     : out std_logic;
      eav_tick_advanced_o     : out std_logic;
      frame_end_o             : out std_logic;
      filter_force_black_o    : out std_logic;
      anc_space_o             : out std_logic;  

      -- parallel data outputs:
      luma_a_o                : out std_logic_vector(9 downto 0);
      chroma_a_o              : out std_logic_vector(9 downto 0);
      luma_b_o                : out std_logic_vector(9 downto 0);
      chroma_b_o              : out std_logic_vector(9 downto 0)
   );
end component;


component colorbar_generator
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;

      -- system parameters:
      pattern_i            : in std_logic_vector(15 downto 0);    -- "00": 100/0/100/0, "01": 100/0/75/0, "10": 75/0/75/0

      -- frame control signals:
      h_grid_state_i       : in std_logic_vector(3 downto 0);

      -- color parameter outputs:
      color_presence_o     : out std_logic_vector(2 downto 0);
      color_level_o        : out std_logic_vector(4 downto 0)
   );
end component;


component clapboard_generator
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      -- system parameters:
      system_i             : in std_logic_vector(7 downto 0);

      -- frame control signals:
      sav_tick_advanced_i  : in std_logic;
      fvh_i                : in std_logic_vector(2 downto 0);

      -- Audio syncronization :
      max_open_field_o     : out std_logic;

      -- Colors:
      color_presence_o     : out std_logic_vector(2 downto 0);
      color_level_o        : out std_logic_vector(4 downto 0)
   );
end component;


component luma_ramp_generator
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;

      -- system parameters:
      system_i             : in std_logic_vector(7 downto 0);

      -- frame control signals:
      sav_tick_advanced_i  : in std_logic;

      -- Video output:
      luma_o               : out std_logic_vector(11 downto 0)
   );
end component;


component color_lut
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;

      system_i             : in std_logic_vector(11 downto 0);
      color_presence_i     : in std_logic_vector(2 downto 0);           -- color_presence_i(2) : r, color_presence_i(1) : g, color_presence_i(0) : b
      color_level_i        : in std_logic_vector(4 downto 0);

      y_o                  : out std_logic_vector(11 downto 0);
      cb_o                 : out std_logic_vector(11 downto 0);
      cr_o                 : out std_logic_vector(11 downto 0)
   );
end component;


component checkfield_generator
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;

      -- frame control signals:
      lower_zupper_i       : in std_logic;
      av_line_i            : in std_logic_vector(10 downto 0);
      eav_tick_advanced_i  : in std_logic;
      frame_end_i          : in std_logic;

      -- color parameter outputs:
      luma_o               : out std_logic_vector(9 downto 0);
      chroma_o             : out std_logic_vector(9 downto 0)
   );
end component;


component line_controller
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;

      -- parameters:
      system_i             : in std_logic_vector(7 downto 0);
      pattern_i            : in std_logic_vector(15 downto 0);

      -- frame control signals:
      av_line_i            : in std_logic_vector(10 downto 0);
      fvh_i                : in std_logic_vector(2 downto 0);
      insert_text_i        : in std_logic;

      -- vertical state signals:
      lower_zupper_o       : out std_logic;

      -- Mux selection signals:
      index_mux_select_o   : out std_logic_vector(2 downto 0);
      unfilt_mux_select_o  : out std_logic;
      av_mux_select_o      : out std_logic
   );
end component;


component audio_click_generator
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      timing_i             : in std_logic_vector(11 downto 0);
      timing_modulation_i  : in std_logic;

      -- syncronization inputs :
      max_open_pulse_i     : in std_logic;

      -- audio pulse:
      audio_click_o        : out std_logic
   );
end component;


component window_generator
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;

      -- system parameters:
      pattern_i            : in std_logic_vector(15 downto 0);

      -- frame control signal:
      v_grid_state_i       : in std_logic_vector(3 downto 0);
      h_grid_state_i       : in std_logic_vector(3 downto 0);

      -- color parameter outputs:
      color_presence_o     : out std_logic_vector(2 downto 0);
      color_level_o        : out std_logic_vector(4 downto 0)
   );
end component;


component grid_generator
   port (
      clk_i                   : in std_logic;
      en_i                    : in std_logic;
                              
      -- parameters:          
      system_i                : in std_logic_vector(7 downto 0);
      interlaced_transport_i  : in std_logic;

      -- frame control signals:
      sav_tick_advanced_i     : in std_logic;
      last_av_sample_i        : in std_logic;
      fvh_i                   : in std_logic_vector(2 downto 0);
                              
      -- output signals:      
      color_level_o           : out std_logic_vector(4 downto 0);
      v_grid_state_o          : out std_logic_vector(3 downto 0);
      h_grid_state_o          : out std_logic_vector(3 downto 0)
   );
end component;


component pluge_generator is
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;

      -- frame control signals:
      h_state_i            : in std_logic_vector(2 downto 0);
      v_state_i            : in std_logic_vector(3 downto 0);

      -- color parameter outputs:
      color_presence_o     : out std_logic_vector(2 downto 0);
      color_level_o        : out std_logic_vector(4 downto 0)
   );
end component;


component audio_generator
   Port (
      videoclk_i           : in std_logic;
      enable_i			      : in std_logic;
      reset_i 			      : in std_logic;
      sync_reset_i	      : in std_logic;
      audiodata_i          : in std_logic_vector(23 downto 0);
      fvh_i                : in std_logic_vector(2 downto 0);
      line_i               : in std_logic_vector(10 downto 0);
      system_i			      : in std_logic_vector(7 downto 0);
		signal_index_i  		: in std_logic_vector(7 downto 0);
      audiodata_tick_o     : out std_logic;
      audiochroma_o        : out std_logic_vector(9 downto 0);
      audioluma_o 		   : out std_logic_vector(9 downto 0);
		debug                : out std_logic_vector(7 downto 0)
     );
end component;


component audio_lut
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;
      audio_sample_tick_i  : in std_logic;
      audio_click_i        : in std_logic;
      signal_index_i       : in std_logic_vector(7 downto 0);
      level_index_i        : in std_logic_vector(7 downto 0);
      audio_data_o         : out std_logic_vector(23 downto 0)
   );
end component;


component text_generator
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      -- input parameters:
      system_i             : in std_logic_vector(7 downto 0);
      v_pos_i              : in std_logic_vector(3 downto 0);
      h_pos_i              : in std_logic_vector(3 downto 0);
      bmp_heigth_i         : in std_logic_vector(10 downto 0);    -- must be >= 1
      bmp_width_i          : in std_logic_vector(10 downto 0);    -- must be >= 1
      text_on_i            : in std_logic;
      text_color_presence_i: in std_logic_vector(2 downto 0);
      back_color_presence_i: in std_logic_vector(2 downto 0);
      text_size_i          : in std_logic_vector(1 downto 0);
      movement_i           : in std_logic_vector(1 downto 0);     -- "00": no movement, "01": vertical, "10": horizontal

      -- frame control signals:
      av_line_i            : in std_logic_vector(10 downto 0);
      av_sample_i          : in std_logic_vector(10 downto 0);
      fvh_i                : in std_logic_vector(2 downto 0);
      v_grid_state_i       : in std_logic_vector(3 downto 0);
      h_grid_state_i       : in std_logic_vector(3 downto 0);
      sav_advanced_i       : in std_logic;

      -- interface to text RAM:
      ram_data_i           : in std_logic_vector(7 downto 0);
      ram_addr_o           : out std_logic_vector(8 downto 0);
      ram_enable_o         : out std_logic;

      -- insert text control:
      insert_text_o        : out std_logic;
      color_presence_o     : out std_logic_vector(2 downto 0)
   );
end component;


component noise_generator is
   generic (
      seed                 : integer        -- 0 to 24
   );
   port (
      clk_148_i            : in std_logic;
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;
      random_word_o        : out std_logic_vector(9 downto 0)
   );
end component;


begin


----------------------------------------------
---- pattern registering at frame start:
----------------------------------------------
--pattern_registering : process (clk_i) begin
--   if clk_i'event and clk_i = '1' then
--      if sync_reset_i = '1' then
--         pattern <= pattern_i;
--      elsif en_i = '1' then
--         if frame_end = '1' then
--            pattern <= pattern_i;
--         end if;
--      end if;
--   end if;
--end process;


----------------------------------------------
---- Audio content generation:
----------------------------------------------
--audio_content_generation : audio_lut
--   port map(
--      clk_i                => clk_i,
--      en_i                 => en_i,
--      sync_reset_i         => sync_reset_i,
--      audio_sample_tick_i  => audio_sample_tick,
--      audio_click_i        => audio_click,
--      signal_index_i       => audio_signal_index_i,
--      level_index_i        => audio_level_index_i,
--      audio_data_o         => audio_data
--   );
--
--
----------------------------------------------
---- Colorbar generation:
----------------------------------------------
--colorbar_index_generation : colorbar_generator
--   port map(
--      clk_i                => clk_i,
--      en_i                 => en_i,
--      pattern_i            => pattern,
--      h_grid_state_i       => h_grid_state,
--      color_presence_o     => color_presence_cbar,
--      color_level_o        => color_level_cbar
--   );
--
--
----------------------------------------------
---- Clapboard generation:
----------------------------------------------
--clapboard_index_generation : clapboard_generator
--   port map(
--      clk_i                => clk_i,
--      en_i                 => en_i,
--      sync_reset_i         => sync_reset_i,
--      system_i             => system_i(11 downto 4),
--      fvh_i                => fvh,
--      max_open_field_o     => max_open_field,
--      sav_tick_advanced_i  => sav_tick_advanced,
--      color_presence_o     => color_presence_cbrd,
--      color_level_o        => color_level_cbrd
--   );
--
--
--max_open_pulse <= lower_zupper and max_open_field;
--
--
--audio_click_generation : audio_click_generator
--  port map(
--      clk_i                => clk_i,
--      en_i                 => en_i,
--      sync_reset_i         => sync_reset_i,
--      timing_i             => click_timing_i,
--      timing_modulation_i  => click_modulation_i,
--      max_open_pulse_i     => max_open_pulse,
--      audio_click_o        => audio_click
--   );
--
--
----------------------------------------------
---- Window generation:
----------------------------------------------
--window_generation : window_generator
--   port map(
--      clk_i                => clk_i,
--      en_i                 => en_i,
--      pattern_i            => pattern,
--      v_grid_state_i       => v_grid_state,
--      h_grid_state_i       => h_grid_state,
--      color_presence_o     => color_presence_window,
--      color_level_o        => color_level_window
--   );
--
--
----------------------------------------------
---- Pluge generation:
----------------------------------------------
--pluge_generation : pluge_generator
--   port map(
--      clk_i                => clk_i,
--      en_i                 => en_i,
--      h_state_i            => h_grid_state(3 downto 1),
--      v_state_i            => v_grid_state,
--      color_presence_o     => color_presence_pluge,
--      color_level_o        => color_level_pluge
--   );
--
--
----------------------------------------------
---- Text generation:
----------------------------------------------
--text_generation : text_generator
--   port map(
--      clk_i                   => clk_i,
--      en_i                    => en_i,
--      sync_reset_i            => sync_reset_i,
--      system_i                => system_i(11 downto 4),
--      v_pos_i                 => v_pos_i(3 downto 0),
--      h_pos_i                 => h_pos_i(3 downto 0),
--      bmp_heigth_i            => bmp_heigth_i,
--      bmp_width_i             => bmp_width_i,
--      text_on_i               => text_on_i,
--      text_color_presence_i   => text_color_presence_i,
--      back_color_presence_i   => back_color_presence_i,
--      text_size_i             => text_size_i,
--      movement_i              => movement_i,
--      av_line_i               => av_line,
--      av_sample_i             => av_sample,
--      fvh_i                   => fvh,
--      v_grid_state_i          => v_grid_state,
--      h_grid_state_i          => h_grid_state,
--      sav_advanced_i          => sav_tick_advanced,
--      ram_data_i              => text_ram_data_i,
--      ram_addr_o              => text_ram_addr_o,
--      ram_enable_o            => text_ram_enable_o,
--      insert_text_o           => insert_text,
--      color_presence_o        => color_presence_text
--   );
--
--color_level_text        <= "10000"; -- 75%
--
----------------------------------------------
---- Crosshatch generation:
----------------------------------------------
--crosshatch_generation : grid_generator
--   port map(
--      clk_i                   => clk_i,
--      en_i                    => en_i,
--      system_i                => system_i(11 downto 4),
--      interlaced_transport_i  => interlaced_transport,
--      sav_tick_advanced_i     => sav_tick_advanced,
--      last_av_sample_i        => last_av_sample,
--      fvh_i                   => fvh,
--      color_level_o           => color_level_crosshatch,
--      v_grid_state_o          => v_grid_state,
--      h_grid_state_o          => h_grid_state
--   );
--
--color_presence_crosshatch  <= "111";
--
--
----------------------------------------------
---- Select source of color index :
----------------------------------------------
--color_index_mux : process (clk_i) begin
--   if clk_i'event and clk_i = '1' then
--      if en_i = '1' then
--         case index_mux_select is
--            when "001" =>     -- Colorbar:
--               color_presence <= color_presence_cbar;
--               color_level    <= color_level_cbar;
--            when "010" =>     -- 75% Red:
--               color_presence <= "100";
--               color_level    <= "10000";
--            when "011" =>     -- Clapper Board:
--               color_presence <= color_presence_cbrd;
--               color_level    <= color_level_cbrd;
--            when "100" =>     -- Window:
--               color_presence <= color_presence_window;
--               color_level    <= color_level_window;
--            when "101" =>     -- Pluge:
--               color_presence <= color_presence_pluge;
--               color_level    <= color_level_pluge;
--            when "110" =>     -- Text:
--               color_presence <= color_presence_text;
--               color_level    <= color_level_text;
--            when "111" =>     -- Crosshatch:
--               color_presence <= color_presence_crosshatch;
--               color_level    <= color_level_crosshatch;
--            when others =>    -- Black:
--               color_presence <= "000";
--               color_level    <= "XXXXX";
--         end case;
--      end if;
--   end if;
--end process;
--
--
--color_generation : color_lut
--   port map(
--      clk_i                => clk_i,
--      en_i                 => en_i,
--      system_i             => system_i,
--      color_presence_i     => color_presence,
--      color_level_i        => color_level,
--      y_o                  => y_av_clut,
--      cb_o                 => cb_av_clut,
--      cr_o                 => cr_av_clut
--   );
--
--
--luma_ramp_generation : luma_ramp_generator
--   port map(
--      clk_i                => clk_i,
--      en_i                 => en_i,
--      system_i             => system_i(11 downto 4),
--      sav_tick_advanced_i  => sav_tick_advanced,
--      luma_o               => y_av_ramp
--   );
--
--
----------------------------------------------
---- Vertical state controller:
----------------------------------------------
--vertical_state_generation : line_controller
--  port map(
--      clk_i                => clk_i,
--      en_i                 => en_i,
--      system_i             => system_i(11 downto 4),
--      pattern_i            => pattern,
--      av_line_i            => av_line,
--      fvh_i                => fvh,
--      insert_text_i		   => insert_text,
--      lower_zupper_o       => lower_zupper,
--      index_mux_select_o   => index_mux_select,
--      unfilt_mux_select_o  => unfilt_mux_select,
--      av_mux_select_o      => av_mux_select
--   );
--
--
----------------------------------------------
---- Unfiltered active video mux:
----------------------------------------------
-----------------------------------------------------------------
---- samples per line dependent on system :
---- 720: "00", 1280: "01", 1920: "10"
-----------------------------------------------------------------
--with system_i(11 downto 4) select
--   spl_index <=      "10" when X"A0",   -- 1080i/30
--                     "10" when X"B0",   -- 1080i/29.97
--                     "10" when X"82",   -- 1080p/30
--                     "10" when X"92",   -- 1080p/29.97
--                     "10" when X"83",   -- 1080p/25
--                     "10" when X"84",   -- 1080p/24
--                     "10" when X"94",   -- 1080p/23.98
--                     "10" when X"A2",   -- 1080sF/30
--                     "10" when X"B2",   -- 1080sF/29.97
--                     "10" when X"A3",   -- 1080sF/25
--                     "10" when X"A4",   -- 1080sF/24
--                     "10" when X"B4",   -- 1080sF/23.98
--                     "01" when X"86",   -- 720p/60
--                     "01" when X"96",   -- 720p/59.94
--                     "01" when X"87",   -- 720p/50
--                     "01" when X"88",   -- 720p/30
--                     "01" when X"98",   -- 720p/29.97
--                     "01" when X"89",   -- 720p/25
--                     "01" when X"8A",   -- 720p/24
--                     "01" when X"9A",   -- 720p/23.98
--                     "00" when X"60",   -- NTSC 525i/29.97
--                     "00" when X"20",   -- PAL 625i/25
--                     "10" when others;  -- 1080i/25   X"A1"
--
--                     
--ycbcr_zrgb_selecting : process (spl_index, system_i) begin
--   if spl_index = "10" then     -- 1080 :                             
--      case system_i(3 downto 0) is
--      when X"1"   =>    ycbcr_zrgb <= '1';   -- YCbCr (4:2:2) 12-bit (dual-link)
--      when X"2"   =>    ycbcr_zrgb <= '1';   -- YCbCr (4:4:4) 10-bit (dual-link)
--      when X"3"   =>    ycbcr_zrgb <= '1';   -- YCbCr (4:4:4) 12-bit (dual-link)
--      when X"4"   =>    ycbcr_zrgb <= '0';   -- RGB   (4:4:4) 10-bit (dual-link)
--      when X"5"   =>    ycbcr_zrgb <= '0';   -- RGB   (4:4:4) 12-bit (dual-link)
--      when others =>    ycbcr_zrgb <= '1';   -- YCbCr (4:2:2) 10-bit (single-link)
--      end case;
--   else
--      ycbcr_zrgb <= '1';                     -- YCbCr (4:2:2) 10-bit (single-link)
--   end if;
--end process;
--
--
--unfiltered_video_mux : process (clk_i) begin
--   if clk_i'event and clk_i = '1' then
--      if en_i = '1' then
--         case unfilt_mux_select is
--            when '0' =>     -- signals from colorlut:
--               y_av_unfilt    <= y_av_clut;
--               cb_av_unfilt   <= cb_av_clut;
--               cr_av_unfilt   <= cr_av_clut;
--            when others =>  -- signals from luma ramp:
--               if ycbcr_zrgb = '1' then    -- YCbCr:
--                  y_av_unfilt    <= y_av_ramp;
--                  cb_av_unfilt   <= "100000000000";
--                  cr_av_unfilt   <= "100000000000";
--               else                       -- RGB:
--                  y_av_unfilt    <= y_av_ramp;
--                  cb_av_unfilt   <= y_av_ramp;
--                  cr_av_unfilt   <= y_av_ramp;
--               end if;
--         end case;
--      end if;
--   end if;
--end process;
--
--
----------------------------------------------
---- Filtering of active video:
----------------------------------------------
--active_video_filtering : luma_chroma_fir_12bit
--   port map(
--      clk_i                => clk_i,
--      en_i                 => en_i,
--      system_i             => system_i,
--      cr_zcb_i             => cr_zcb,
--      force_black_i        => filter_force_black,
--      y_i                  => y_av_unfilt,
--      cb_i                 => cb_av_unfilt,
--      cr_i                 => cr_av_unfilt,
--      luma_a_o             => luma_av_fir_a,
--      chroma_a_o           => chroma_av_fir_a,
--      luma_b_o             => luma_av_fir_b,
--      chroma_b_o           => chroma_av_fir_b
--   );
--
--
----------------------------------------------
---- SDI Checkfield:
----------------------------------------------
--checkfield_generation : checkfield_generator
--   port map (
--      clk_i                => clk_i,
--      en_i                 => en_i,
--      lower_zupper_i       => lower_zupper,
--      av_line_i            => av_line,
--      eav_tick_advanced_i  => eav_tick_advanced,
--      frame_end_i          => frame_end,
--      luma_o               => luma_check,
--      chroma_o             => chroma_check
--   );


--------------------------------------------
-- Active video mux:
--------------------------------------------
--video_mux : process (clk_i) begin
--   if clk_i'event and clk_i = '1' then
--      if en_i = '1' then
 --        case av_mux_select is
 --           when '0' =>    -- signals from filter:
 --              luma_av_a   <= luma_av_fir_a;
 --              chroma_av_a <= chroma_av_fir_a;
 --              luma_av_b   <= luma_av_fir_b;
 --              chroma_av_b <= chroma_av_fir_b;
 --           when others => -- signals from check field:
 video_mux : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
               luma_av_a   <= ddr_luma_i;   --luma_check;
               chroma_av_a <= ddr_chroma_i; --chroma_check;
               luma_av_b   <= (others => '0');--luma_check;
               chroma_av_b <= (others => '0'); --chroma_check;
 --        end case;
      end if;
   end if;
end process;


--------------------------------------------
-- Active video legalizer (none ff or 00 words):
--------------------------------------------
--video_legalizer : process (clk_i) begin
--   if clk_i'event and clk_i = '1' then
--      if en_i = '1' then
video_legalizer : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if luma_av_a(9 downto 2) = "11111111" then
            luma_av_legal_a <= "1111111000";
         elsif luma_av_a(9 downto 2) = "00000000" then
            luma_av_legal_a <= "0000000100";
         else
            luma_av_legal_a <= luma_av_a;
         end if;
         if chroma_av_a(9 downto 2) = "11111111" then
            chroma_av_legal_a <= "1111111000";
         elsif chroma_av_a(9 downto 2) = "00000000" then
            chroma_av_legal_a <= "0000000100";
         else
            chroma_av_legal_a <= chroma_av_a;
         end if;
         if luma_av_b(9 downto 2) = "11111111" then
            luma_av_legal_b <= "1111111000";
         elsif luma_av_b(9 downto 2) = "00000000" then
            luma_av_legal_b <= "0000000100";
         else
            luma_av_legal_b <= luma_av_b;
         end if;
         if chroma_av_b(9 downto 2) = "11111111" then
            chroma_av_legal_b <= "1111111000";
         elsif chroma_av_b(9 downto 2) = "00000000" then
            chroma_av_legal_b <= "0000000100";
         else
            chroma_av_legal_b <= chroma_av_b;
         end if;
      end if;
   end if;
end process;


--------------------------------------------
-- Select between real - or dummy data:
--------------------------------------------
real_dummy_mux : process (luma_av_legal_a, chroma_av_legal_a,  luma_av_legal_b, chroma_av_legal_b)--, luma_audio, chroma_audio)                       
begin
   if timing_dummy_data = '0' then 
      luma_video_a          <= luma_av_a;--luma_av_legal_a;
      chroma_video_a        <= chroma_av_a;--chroma_av_legal_a;
      luma_h_blank_a        <= "0001000000";--luma_audio;
      chroma_h_blank_a      <= "1000000000";--chroma_audio;
      luma_v_blank_a        <= "0001000000";
      chroma_v_blank_a      <= "1000000000";
      luma_video_b          <= luma_av_legal_b;
      chroma_video_b        <= chroma_av_legal_b;
      luma_h_blank_b        <= "0001000000";
      chroma_h_blank_b      <= "1000000000";
      luma_v_blank_b        <= "0001000000";
      chroma_v_blank_b      <= "1000000000";
   else
      luma_video_a          <= "0000000000";
      chroma_video_a        <= "1111111111";
      luma_h_blank_a        <= "0101010101";
      chroma_h_blank_a      <= "0101010101";
      luma_v_blank_a        <= "0000011111";
      chroma_v_blank_a      <= "0000011111";
      luma_video_b          <= "0000000000";
      chroma_video_b        <= "1111111111";
      luma_h_blank_b        <= "0101010101";
      chroma_h_blank_b      <= "0101010101";
      luma_v_blank_b        <= "0000011111";
      chroma_v_blank_b      <= "0000011111";
   end if;
end process;


--------------------------------------------
-- Generate video system:
--------------------------------------------
system_generation : system_generator
   port map(
      clk_i                   => clk_i,
      en_i                    => en_i,
      sync_reset_i            => sync_reset_i,
      system_i                => system_i(11 downto 4),
      luma_video_a_i          => luma_video_a,    
      chroma_video_a_i        => chroma_video_a,  
      luma_h_blank_a_i        => luma_h_blank_a,  
      chroma_h_blank_a_i      => chroma_h_blank_a,
      luma_v_blank_a_i        => luma_v_blank_a,  
      chroma_v_blank_a_i      => chroma_v_blank_a,
      luma_video_b_i          => luma_video_b,    
      chroma_video_b_i        => chroma_video_b,  
      luma_h_blank_b_i        => luma_h_blank_b,  
      chroma_h_blank_b_i      => chroma_h_blank_b,
      luma_v_blank_b_i        => luma_v_blank_b,  
      chroma_v_blank_b_i      => chroma_v_blank_b,
      interlaced_transport_o  => interlaced_transport,
      last_av_sample_o        => last_av_sample,
      av_line_o               => av_line,
      av_sample_o             => av_sample,
      line_o                  => line,
      fvh_o                   => fvh,
      cr_zcb_o                => cr_zcb,
      sav_tick_advanced_o     => sav_tick_advanced,
      eav_tick_advanced_o     => eav_tick_advanced,
      frame_end_o             => frame_end,
      filter_force_black_o    => filter_force_black,
      anc_space_o             => anc_space,  
      luma_a_o                => luma_system_a,
      chroma_a_o              => chroma_system_a,
      luma_b_o                => luma_system_b,
      chroma_b_o              => chroma_system_b
   );

frame_end_o    <= frame_end;


-----------------------------------------------------
-- Insert payload information :
-----------------------------------------------------
payload_insertion : payload_generator
   port map(
      clk_i                => clk_i,       
      en_i                 => en_i,        
      system_i             => system_i,
      line_i               => line,
      anc_space_i          => anc_space,
      data_a_i             => luma_system_a,
      data_b_i             => luma_system_b,
      data_a_o             => luma_a,
      data_b_o             => luma_b
   );    


-----------------------------------------------------
-- Scrambling (done in 20 bit parallel data domain):
-----------------------------------------------------
bit_scrambling_a : scram20_top
   port map(
      clk_i                => clk_i,
      zreset_i             => '1',  --zreset_i,
      bypass_i             => bypass_scrambler,
      enable_i             => en_i,
      luma_i               => luma_a,
      chroma_i             => chroma_system_a,
      data_o               => data_a_o
   );


bit_scrambling_b : scram20_top
   port map(
      clk_i                => clk_i,
      zreset_i             => '1',  --zreset_i,
      bypass_i             => bypass_scrambler,
      enable_i             => en_i,
      luma_i               => luma_b,
      chroma_i             => chroma_system_b,
      data_o               => data_b_o
   );


--------------------------------------------------------
----audio signal generator
--------------------------------------------------------
--audio_generation: audio_generator
--	port map(
--	  videoclk_i			=>  clk_i,
--	  enable_i				=>  en_i,
--	  reset_i				=>  '1', --zreset_i,
--	  sync_reset_i			=>  sync_reset_i,
--	  audiodata_i       	=>  audio_data,
--	  fvh_i					=>  fvh,
--	  line_i				=>  line,
--	  system_i				=>  system_i(11 downto 4),
--	  signal_index_i    	=>  audio_signal_index_i,
--	  audiodata_tick_o  	=>  audio_sample_tick,
--	  audiochroma_o			=>  chroma_audio,
--	  audioluma_o			=>  luma_audio
--	);

fvh_o <= fvh;                                                                              
end Behavioral;