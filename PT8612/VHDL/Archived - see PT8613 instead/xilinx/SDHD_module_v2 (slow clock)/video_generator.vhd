library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;


entity video_generator is
port(
   clk_i                : in std_logic;
   brefclk_i            : in std_logic;
   brefclk2_i           : in std_logic;
   use_clk2_i           : in std_logic;
                        
   zreset_i             : in std_logic;
   mute_i               : in std_logic;
   frame_start_i        : in std_logic;
   MGT_power_down_i     : in std_logic;

   -- video parameters:
   system_i             : in std_logic_vector(7 downto 0);
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

   -- serial video output:
   sd_zhd_o             : out std_logic;
   sdi_p_o              : out std_logic;
   sdi_n_o              : out std_logic;
                        
   -- timing status:    
   timing_change_o      : out std_logic;
   rio_fifo_err_o       : out std_logic;
                        
   -- debug:            
   debug_o              : out std_logic_vector(11 downto 0)
);
end video_generator;


architecture Behavioral of video_generator is


signal enable                 : std_logic;
signal mute                   : std_logic;
signal sync_reset             : std_logic;
signal hd_zsd                 : std_logic;
signal interlaced             : std_logic;
signal sav_tick_advanced      : std_logic;
signal eav_tick_advanced      : std_logic;
signal frame_end              : std_logic;
signal last_av_sample         : std_logic;
signal lower_zupper           : std_logic;
signal max_open_pulse         : std_logic;
signal max_open_field         : std_logic;
signal audio_click            : std_logic;

signal h_state_cbar           : std_logic_vector(2 downto 0);
signal fvh                    : std_logic_vector(2 downto 0);
signal av_line                : std_logic_vector(10 downto 0);
signal av_sample              : std_logic_vector(10 downto 0);
signal asl                    : std_logic_vector(10 downto 0);
signal line                   : std_logic_vector(10 downto 0);
signal insert_text            : std_logic;

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

signal luma                   : std_logic_vector(9 downto 0);
signal chroma                 : std_logic_vector(9 downto 0);

signal y_av_unfilt            : std_logic_vector(11 downto 0);
signal cb_av_unfilt           : std_logic_vector(11 downto 0);
signal cr_av_unfilt           : std_logic_vector(11 downto 0);

signal y_av_ramp              : std_logic_vector(11 downto 0);
signal y_av_clut              : std_logic_vector(11 downto 0);
signal cb_av_clut             : std_logic_vector(11 downto 0);
signal cr_av_clut             : std_logic_vector(11 downto 0);
signal cr_zcb                 : std_logic;
signal filter_force_black     : std_logic;


signal luma_av_legal          : std_logic_vector(9 downto 0);
signal chroma_av_legal        : std_logic_vector(9 downto 0);
signal luma_check             : std_logic_vector(9 downto 0);
signal chroma_check           : std_logic_vector(9 downto 0);
signal luma_av                : std_logic_vector(9 downto 0);
signal chroma_av              : std_logic_vector(9 downto 0);
signal luma_av_fir            : std_logic_vector(9 downto 0);
signal chroma_av_fir          : std_logic_vector(9 downto 0);
signal luma_av_debug          : std_logic_vector(9 downto 0);

signal index_mux_select       : std_logic_vector(2 downto 0);
signal unfilt_mux_select      : std_logic;
signal av_mux_select          : std_logic;

signal scramb_data            : std_logic_vector(19 downto 0);
signal os_data                : std_logic_vector(19 downto 0);
signal txdata                 : std_logic_vector(19 downto 0);
signal scramb_data_res        : std_logic_vector(19 downto 0);
signal debug_count            : std_logic_vector(5 downto 0);
signal debug_tick             : std_logic;

signal y_debug                : std_logic_vector(9 downto 0);
signal cb_debug               : std_logic_vector(9 downto 0);
signal cr_debug               : std_logic_vector(9 downto 0);

signal chroma_audio		      : std_logic_vector(9 downto 0);
signal luma_audio		         : std_logic_vector(9 downto 0);

signal audio_sample_tick      : std_logic;
signal audio_data             : std_logic_vector(23 downto 0);

component luma_chroma_fir_12bit
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;
      cr_zcb_i             : in std_logic;
      force_black_i        : in std_logic;
      y_i                  : in std_logic_vector(11 downto 0);
      cb_i                 : in std_logic_vector(11 downto 0);
      cr_i                 : in std_logic_vector(11 downto 0);
      luma_o               : out std_logic_vector(9 downto 0);
      chroma_o             : out std_logic_vector(9 downto 0)
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


component os_controller
   port(
       clk_148       :  in  std_logic;
       reset         :  in  std_logic;
       sync_reset_i  :  in  std_logic;
       hdsd_in       :  in  std_logic;
       data_in       :  in  std_logic_vector(19 downto 0);
       data_out      :  out std_logic_vector(19 downto 0);
       enable_out    :  out std_logic;
       debug_clk_o   :  out std_logic;
       debug_data_o  :  out std_logic_vector(9 downto 0)
   );
end component;


component system_generator
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      frame_start_i        : in std_logic;

      -- system parameters:
      system_i             : in std_logic_vector(7 downto 0);

      -- parallel data inputs (video and ancillary)
      luma_video_i         : in std_logic_vector(9 downto 0);
      chroma_video_i       : in std_logic_vector(9 downto 0);

      luma_h_blank_i       : in std_logic_vector(9 downto 0);
      chroma_h_blank_i     : in std_logic_vector(9 downto 0);

      luma_v_blank_i       : in std_logic_vector(9 downto 0);
      chroma_v_blank_i     : in std_logic_vector(9 downto 0);

      ---system parameter outputs:
      hd_zsd_o             : out std_logic;
      interlaced_o         : out std_logic;
      asl_o                : out std_logic_vector(10 downto 0);
      color_system_o       : out std_logic_vector(1 downto 0);

      -- frame control signals:
      last_av_sample_o     : out std_logic;
      av_line_o            : out std_logic_vector(10 downto 0);
      av_sample_o          : out std_logic_vector(10 downto 0);
      line_o               : out std_logic_vector(10 downto 0);
      fvh_o                : out std_logic_vector(2 downto 0);    -- blanking bits
      cr_zcb_o             : out std_logic;
      sav_tick_advanced_o  : out std_logic;
      eav_tick_advanced_o  : out std_logic;
      frame_end_o          : out std_logic;
      filter_force_black_o : out std_logic;

      -- parallel data outputs:
      luma_o               : out std_logic_vector(9 downto 0);
      chroma_o             : out std_logic_vector(9 downto 0);

      -- other output signals:
      sync_reset_o         : out std_logic;                       -- syncronious reset output to oversampler:
      timing_change_o      : out std_logic;

      -- debug:
      debug_o              : out std_logic_vector(9 downto 0)
   );
end component;


component rocket_io_top
   port(
      brefclk_i         : in std_logic;
      brefclk2_i        : in std_logic;
      use_clk2_i        : in std_logic;
      sync_reset_i      : in std_logic;
      power_down_i      : in std_logic;
      data_clk_i        : in std_logic;
      data_i            : in std_logic_vector (19 downto 0);
      fifo_error_o      : out std_logic;
      sdi_p_o           : out std_logic;     -- serial video outputs:
      sdi_n_o           : out std_logic
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

      hd_zsd_i             : in std_logic;
      color_presence_i     : in std_logic_vector(2 downto 0);     -- color_presence_i(2) : r, color_presence_i(1) : g, color_presence_i(0) : b
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
      clk_i                : in std_logic;
      en_i                 : in std_logic;

      -- parameters:
      system_i             : in std_logic_vector(7 downto 0);
      interlaced_i         : in std_logic;

      -- frame control signals:
      sav_tick_advanced_i  : in std_logic;
      last_av_sample_i     : in std_logic;
      fvh_i                : in std_logic_vector(2 downto 0);

      -- output signals:
      color_level_o        : out std_logic_vector(4 downto 0);
      v_grid_state_o       : out std_logic_vector(3 downto 0);
      h_grid_state_o       : out std_logic_vector(3 downto 0)
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


begin


--------------------------------------------
-- pattern registering at frame start:
--------------------------------------------
pattern_registering : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         pattern <= pattern_i; 
      elsif enable = '1' then
         if frame_end = '1' then
            pattern <= pattern_i;
         end if;
      end if;
   end if;
end process;


--------------------------------------------
-- Audio content generation:
--------------------------------------------
audio_content_generation : audio_lut
   port map(
      clk_i                => clk_i,     
      en_i                 => enable,    
      sync_reset_i         => sync_reset,
      audio_sample_tick_i  => audio_sample_tick,
      audio_click_i        => audio_click,
      signal_index_i       => audio_signal_index_i,
      level_index_i        => audio_level_index_i,
      audio_data_o         => audio_data
   );


--------------------------------------------
-- Colorbar generation:
--------------------------------------------
colorbar_index_generation : colorbar_generator
   port map(
      clk_i                => clk_i,
      en_i                 => enable,
      pattern_i            => pattern,
      h_grid_state_i       => h_grid_state,            
      color_presence_o     => color_presence_cbar,
      color_level_o        => color_level_cbar
   );


--------------------------------------------
-- Clapboard generation:
--------------------------------------------
clapboard_index_generation : clapboard_generator
   port map(
      clk_i                => clk_i,
      en_i                 => enable,
      sync_reset_i         => sync_reset,
      system_i             => system_i,
      fvh_i                => fvh,
      max_open_field_o     => max_open_field,
      sav_tick_advanced_i  => sav_tick_advanced,
      color_presence_o     => color_presence_cbrd,
      color_level_o        => color_level_cbrd
   );


max_open_pulse <= lower_zupper and max_open_field;


audio_click_generation : audio_click_generator
  port map(
      clk_i                => clk_i,
      en_i                 => enable,
      sync_reset_i         => sync_reset,
      timing_i             => click_timing_i,
      timing_modulation_i  => click_modulation_i,
      max_open_pulse_i     => max_open_pulse,
      audio_click_o        => audio_click
   );


--------------------------------------------
-- Window generation:
--------------------------------------------
window_generation : window_generator
   port map(
      clk_i                => clk_i,                      
      en_i                 => enable,            
      pattern_i            => pattern,    
      v_grid_state_i       => v_grid_state,
      h_grid_state_i       => h_grid_state,            
      color_presence_o     => color_presence_window,
      color_level_o        => color_level_window   
   );        


--------------------------------------------
-- Pluge generation:
--------------------------------------------
pluge_generation : pluge_generator
   port map(
      clk_i                => clk_i,                
      en_i                 => enable,               
      h_state_i            => h_grid_state(3 downto 1),
      v_state_i            => v_grid_state,
      color_presence_o     => color_presence_pluge,
      color_level_o        => color_level_pluge   
   );


--------------------------------------------
-- Text generation:
--------------------------------------------
text_generation : text_generator
   port map(
      clk_i                   => clk_i,     
      en_i                    => enable,    
      sync_reset_i            => sync_reset,
      system_i                => system_i,
      v_pos_i                 => v_pos_i(3 downto 0),         
      h_pos_i                 => h_pos_i(3 downto 0),       
      bmp_heigth_i            => bmp_heigth_i,         
      bmp_width_i             => bmp_width_i,          
      text_on_i               => text_on_i,            
      text_color_presence_i   => text_color_presence_i,
      back_color_presence_i   => back_color_presence_i,
      text_size_i             => text_size_i,          
      movement_i              => movement_i,           
      av_line_i               => av_line,
      av_sample_i             => av_sample,
      fvh_i                   => fvh,
      v_grid_state_i          => v_grid_state,
      h_grid_state_i          => h_grid_state,
      sav_advanced_i          => sav_tick_advanced,
      ram_data_i              => text_ram_data_i,
      ram_addr_o              => text_ram_addr_o,
      ram_enable_o            => text_ram_enable_o,
      insert_text_o           => insert_text,
      color_presence_o        => color_presence_text
   );

color_level_text        <= "10000"; -- 75%

--------------------------------------------
-- Crosshatch generation:
--------------------------------------------
crosshatch_generation : grid_generator
   port map(
      clk_i                => clk_i,      
      en_i                 => enable,    
      system_i             => system_i,
      interlaced_i         => interlaced,
      sav_tick_advanced_i  => sav_tick_advanced,
      last_av_sample_i     => last_av_sample,
      fvh_i                => fvh,
      color_level_o        => color_level_crosshatch,
      v_grid_state_o       => v_grid_state,
      h_grid_state_o       => h_grid_state
   );

color_presence_crosshatch  <= "111";


--------------------------------------------
-- Select source of color index :
--------------------------------------------
color_index_mux : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if enable = '1' then
         case index_mux_select is
            when "001" =>     -- Colorbar:
               color_presence <= color_presence_cbar;
               color_level    <= color_level_cbar;
            when "010" =>     -- 75% Red:
               color_presence <= "100";
               color_level    <= "10000";
            when "011" =>     -- Clapper Board:
               color_presence <= color_presence_cbrd;
               color_level    <= color_level_cbrd;
            when "100" =>     -- Window:
               color_presence <= color_presence_window;
               color_level    <= color_level_window;
            when "101" =>     -- Pluge:
               color_presence <= color_presence_pluge;
               color_level    <= color_level_pluge;
            when "110" =>     -- Text:
               color_presence <= color_presence_text;
               color_level    <= color_level_text;
            when "111" =>     -- Crosshatch:
               color_presence <= color_presence_crosshatch;
               color_level    <= color_level_crosshatch;
            when others =>    -- Black:
               color_presence <= "000";
               color_level    <= "XXXXX";
         end case;
      end if;
   end if;
end process;


color_generation : color_lut
   port map(
      clk_i                => clk_i,
      en_i                 => enable,
      hd_zsd_i             => hd_zsd,
      color_presence_i     => color_presence,
      color_level_i        => color_level,
      y_o                  => y_av_clut,
      cb_o                 => cb_av_clut,
      cr_o                 => cr_av_clut
   );


luma_ramp_generation : luma_ramp_generator
   port map(
      clk_i                => clk_i,
      en_i                 => enable,
      system_i             => system_i,
      sav_tick_advanced_i  => sav_tick_advanced,
      luma_o               => y_av_ramp
   );


--------------------------------------------
-- Vertical state controller:
--------------------------------------------
vertical_state_generation : line_controller
  port map(
      clk_i                => clk_i,
      en_i                 => enable,
      system_i             => system_i,
      pattern_i            => pattern,
      av_line_i            => av_line,
      fvh_i                => fvh,
      insert_text_i		   => insert_text,
      lower_zupper_o       => lower_zupper,
      index_mux_select_o   => index_mux_select,
      unfilt_mux_select_o  => unfilt_mux_select,
      av_mux_select_o      => av_mux_select
   );


--------------------------------------------
-- Unfiltered active video mux:
--------------------------------------------
unfiltered_video_mux : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if enable = '1' then
         case unfilt_mux_select is
            when '0' =>     -- signals from colorlut:
               y_av_unfilt    <= y_av_clut;
               cb_av_unfilt   <= cb_av_clut;
               cr_av_unfilt   <= cr_av_clut;
            when others =>  -- signals from luma ramp:
               y_av_unfilt    <= y_av_ramp;        --"XXXXXXXXXXXX",
               cb_av_unfilt   <= "100000000000";   --"XXXXXXXXXXXX",
               cr_av_unfilt   <= "100000000000";   --"XXXXXXXXXXXX",
         end case;
      end if;
   end if;
end process;


--------------------------------------------
-- Filtering of active video:
--------------------------------------------
active_video_filtering : luma_chroma_fir_12bit
   port map(
      clk_i                => clk_i,
      en_i                 => enable,
      cr_zcb_i             => cr_zcb,
      force_black_i        => filter_force_black,
      y_i                  => y_av_unfilt,
      cb_i                 => cb_av_unfilt,
      cr_i                 => cr_av_unfilt,
      luma_o               => luma_av_fir,
      chroma_o             => chroma_av_fir
   );


--------------------------------------------
-- SDI Checkfield:
--------------------------------------------
checkfield_generation : checkfield_generator
   port map (
      clk_i                => clk_i,
      en_i                 => enable,
      lower_zupper_i       => lower_zupper,
      av_line_i            => av_line,
      eav_tick_advanced_i  => eav_tick_advanced,
      frame_end_i          => frame_end,
      luma_o               => luma_check,
      chroma_o             => chroma_check
   );


--------------------------------------------
-- Active video mux:
--------------------------------------------
video_mux : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if enable = '1' then
         case av_mux_select is
            when '0' =>    -- signals from filter:
               luma_av     <= luma_av_fir;
               chroma_av   <= chroma_av_fir;
            when others => -- signals from check field:
               luma_av     <= luma_check;
               chroma_av   <= chroma_check;
         end case;
      end if;
   end if;
end process;


--------------------------------------------
-- Active video legalizer (none ff or 00 words):
--------------------------------------------
video_legalizer : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if enable = '1' then
         if luma_av(9 downto 2) = "11111111" then
            luma_av_legal <= "1111111000";
         elsif luma_av(9 downto 2) = "00000000" then
            luma_av_legal <= "0000000100";
         else
            luma_av_legal <= luma_av;
         end if;
         if chroma_av(9 downto 2) = "11111111" then
            chroma_av_legal <= "1111111000";
         elsif chroma_av(9 downto 2) = "00000000" then
            chroma_av_legal <= "0000000100";
         else
            chroma_av_legal <= chroma_av;
         end if;
      end if;
   end if;
end process;


--------------------------------------------
-- Generate video system:
--------------------------------------------
system_generation : system_generator
   port map(
      clk_i                => clk_i,
      zreset_i             => '1',  --zreset_i,
      en_i                 => enable,
      frame_start_i        => frame_start_i,
      system_i             => system_i,
      luma_video_i         => luma_av_legal,
      chroma_video_i       => chroma_av_legal,
      luma_h_blank_i       => luma_audio,
      chroma_h_blank_i     => chroma_audio,
      luma_v_blank_i       => "0001000000",
      chroma_v_blank_i     => "1000000000",
      last_av_sample_o     => last_av_sample,
      av_line_o            => av_line,
      line_o			      => line,
      av_sample_o          => av_sample,
      fvh_o                => fvh,
      sav_tick_advanced_o  => sav_tick_advanced,
      eav_tick_advanced_o  => eav_tick_advanced,
      frame_end_o          => frame_end,
      hd_zsd_o             => hd_zsd,
      interlaced_o         => interlaced,
      asl_o                => asl,
      cr_zcb_o             => cr_zcb,
      filter_force_black_o => filter_force_black,
      luma_o               => luma,
      chroma_o             => chroma,
      sync_reset_o         => sync_reset,
      timing_change_o      => timing_change_o,
      debug_o              => open-- debug_o
   );


-----------------------------------------------------
-- Scrambling (done in 20 bit parallel data domain):
-----------------------------------------------------
bit_scrambling : scram20_top
   port map(
      clk_i                => clk_i,
      zreset_i             => '1',  --zreset_i,
      bypass_i             => '1',  --'0',
      enable_i             => enable,
      luma_i               => luma,
      chroma_i             => chroma,
      data_o               => scramb_data
   );


-----------------------------------------------------
-- Bit oversampling HD: x2, SD: x11 :
-- (done in parallel data domain)
-----------------------------------------------------
serial_video_oversampling : os_controller
   port map(
       clk_148             => clk_i,
       reset               => zreset_i,
       sync_reset_i        => sync_reset,
       hdsd_in             => hd_zsd,
       data_in             => scramb_data,
       data_out            => os_data,
       enable_out          => enable,
       debug_clk_o         => debug_o(10),
       debug_data_o        => debug_o(9 downto 0)
   );
   
   

-----------------------------------------------------
-- MGT serializer (Rocket IO) :
-----------------------------------------------------
--rocket_io_inst :  rocket_io_top
--	port map(
--      brefclk_i            => brefclk_i,
--      brefclk2_i           => brefclk2_i,
--      use_clk2_i           => use_clk2_i,
--      sync_reset_i         => mute,
--      power_down_i         => MGT_power_down_i,
--      data_clk_i           => clk_i,
--      data_i               => os_data,
--      fifo_error_o         => rio_fifo_err_o,
--      sdi_p_o              => sdi_p_o,
--      sdi_n_o              => sdi_n_o
--   );
rio_fifo_err_o    <= '0';
sdi_p_o           <= '0';
sdi_n_o           <= '0';


------------------------------------------------------
--audio signal generator
--
------------------------------------------------------
audio_generation: audio_generator
	port map(
    	videoclk_i			=>  clk_i,
		enable_i				=>  enable,
		reset_i				=>  '1', --zreset_i,
		sync_reset_i		=>  sync_reset,
      audiodata_i       =>  audio_data, 
		fvh_i					=>  fvh,
		line_i				=>  line,
		system_i				=>  system_i,
		signal_index_i    =>  audio_signal_index_i,
      audiodata_tick_o  =>  audio_sample_tick,
		audiochroma_o		=>  chroma_audio,
		audioluma_o			=>  luma_audio
--		debug					=>  debug_o(7 downto 0)
	);	
	
---------------------------------------------------------------
-- Mute latching :
---------------------------------------------------------------
mute_latching : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      mute <= '1';
   elsif clk_i'event and clk_i = '1' then
      if frame_start_i = '1' then
         mute <= mute_i;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Mapping of outputs :
---------------------------------------------------------------
sd_zhd_o <= not hd_zsd;


---------------------------------------------------------------
-- Debug :
---------------------------------------------------------------
debug_o(11) <= frame_end;


end Behavioral;