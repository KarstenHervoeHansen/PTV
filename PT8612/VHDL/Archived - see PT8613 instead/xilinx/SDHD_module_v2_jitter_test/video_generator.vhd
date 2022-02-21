library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;


entity video_generator is
port(
   clk_i              : in std_logic;
   brefclk_i          : in std_logic;
   brefclk2_i         : in std_logic;
   use_clk2_i         : in std_logic;

   zreset_i           : in std_logic;
   mute_i             : in std_logic;
   MGT_power_down_i   : in std_logic;

   -- video parameters:
   system_i           : in std_logic_vector(7 downto 0);
   pattern_i          : in std_logic_vector(15 downto 0);

   
   use_f1484_o        : out std_logic;

   -- serial video output:
   sd_zhd_o           : out std_logic;
   sdi_p_o            : out std_logic;
   sdi_n_o            : out std_logic;

   -- timing status:
   timing_change_o    : out std_logic;
   rio_fifo_err_o     : out std_logic;

   -- debug:
   debug_o            : out std_logic_vector(9 downto 0)
);
end video_generator;


architecture Behavioral of video_generator is


signal enable              : std_logic;
signal mute                : std_logic;
signal sync_reset          : std_logic;
signal hd_zsd              : std_logic;
signal interlaced          : std_logic;
signal sav_tick_advanced   : std_logic;
signal eav_tick_advanced   : std_logic;
signal frame_end           : std_logic;
signal fvh                 : std_logic_vector(2 downto 0);
signal asl                 : std_logic_vector(10 downto 0);
signal color_system        : std_logic_vector(1 downto 0);
signal color_index         : std_logic_vector(3 downto 0);
signal color_index_cbar    : std_logic_vector(3 downto 0);
signal colorbar_type       : std_logic_vector(1 downto 0);
signal pattern             : std_logic_vector(15 downto 0);

signal luma                : std_logic_vector(9 downto 0);
signal chroma              : std_logic_vector(9 downto 0);

signal y_av_cbar           : std_logic_vector(11 downto 0);
signal cb_av_cbar          : std_logic_vector(11 downto 0);
signal cr_av_cbar          : std_logic_vector(11 downto 0);
signal cr_zcb              : std_logic;

signal luma_av_legal       : std_logic_vector(9 downto 0);
signal chroma_av_legal     : std_logic_vector(9 downto 0);
signal luma_check          : std_logic_vector(9 downto 0);
signal chroma_check        : std_logic_vector(9 downto 0);
signal luma_av             : std_logic_vector(9 downto 0);
signal chroma_av           : std_logic_vector(9 downto 0);
signal luma_av_fir         : std_logic_vector(9 downto 0);
signal chroma_av_fir       : std_logic_vector(9 downto 0);
signal luma_av_debug       : std_logic_vector(9 downto 0);

signal scramb_data         : std_logic_vector(19 downto 0);
signal os_data             : std_logic_vector(19 downto 0);
signal txdata              : std_logic_vector(19 downto 0);
signal scramb_data_res     : std_logic_vector(19 downto 0);
signal debug_count         : std_logic_vector(5 downto 0);
signal debug_tick          : std_logic;

signal y_debug             : std_logic_vector(9 downto 0);
signal cb_debug            : std_logic_vector(9 downto 0);
signal cr_debug            : std_logic_vector(9 downto 0);


component luma_chroma_fir_12bit
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;
      cr_zcb_i             : in std_logic;
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
       enable_out    :  out std_logic
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
      line_o               : out std_logic_vector(10 downto 0);
      fvh_o                : out std_logic_vector(2 downto 0);    -- blanking bits
      cr_zcb_o             : out std_logic;
      sav_tick_advanced_o  : out std_logic;
      eav_tick_advanced_o  : out std_logic;
      frame_end_o          : out std_logic;

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


component system_parameters
   port (
      zreset_i             : in std_logic;
      clk_i                : in std_logic;
      system_i             : in std_logic_vector(7 downto 0);
      hd_zsd_o             : out std_logic;                       -- 0: sd, 1: hd
      interlaced_o         : out std_logic;                       -- 1: interlaced, 0: progressive
		asl_o                : out std_logic_vector(10 downto 0);   -- active samples per line
      lpf_o                : out std_logic_vector(10 downto 0);   -- lines per frame
      hb1s_o               : out std_logic_vector(11 downto 0);   -- horizontal blanking 1 length (samples), asl+10+h_blank1+h_blank2 =tsl
      hb2s_o               : out std_logic_vector(11 downto 0);   -- horizontal blanking 2 length (samples)
      field1_end_o         : out std_logic_vector(10 downto 0);
      field2_end_o         : out std_logic_vector(10 downto 0);
      f_start_o            : out std_logic;
      line_start_o         : out std_logic_vector(10 downto 0);
      field1_vblnk_end_o   : out std_logic_vector(10 downto 0);   -- line numbers for vertical blanking borders
      field1_av_end_o      : out std_logic_vector(10 downto 0);
      field2_vblnk_end_o   : out std_logic_vector(10 downto 0);
      field2_av_end_o      : out std_logic_vector(10 downto 0);
      color_system_o       : out std_logic_vector(1 downto 0)
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
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      -- system parameters:
      asl_i                : in std_logic_vector(10 downto 0);
      colorbar_type_i      : in std_logic_vector(1 downto 0);    -- "00": 100/0/100/0, "01": 100/0/75/0, "10": 75/0/100/0, "11": 75/0/75/0

      -- frame control signals:
      sav_tick_advanced_i  : in std_logic;

      -- color parameter outputs:
      color_system_o       : out std_logic_vector(1 downto 0);
      color_index_o        : out std_logic_vector(3 downto 0)
   );
end component;


component color_generator
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      color_system_i       : in std_logic_vector(1 downto 0);
      color_index_i        : in std_logic_vector(3 downto 0);     -- color_index_i(3) : r, color_index_i(2) : g, color_index_i(1) : b, color_index_i(0) : '0':100% '1':75%

      y_o                  : out std_logic_vector(11 downto 0);
      cb_o                 : out std_logic_vector(11 downto 0);
      cr_o                 : out std_logic_vector(11 downto 0)
   );
end component;


component checkfield_generator
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      -- system parameters:
      system_i             : in std_logic_vector(7 downto 0);
      interlaced_i         : in std_logic;
      hd_zsd_i             : in std_logic;
      
       -- frame control signals:
      fvh_i                : in std_logic_vector(2 downto 0);
      eav_tick_advanced_i  : in std_logic;
      frame_end_i          : in std_logic;
      
      -- color parameter outputs:
      luma_o               : out std_logic_vector(9 downto 0);
      chroma_o             : out std_logic_vector(9 downto 0)
   );
end component;


begin


-----------------------------------------------------
-- system parameters:
-----------------------------------------------------
system_parameter_lookup : system_parameters
port map (
   zreset_i                => zreset_i,
   clk_i                   => clk_i,
   system_i                => system_i,
   interlaced_o            => use_f1484_o,
   hd_zsd_o                => hd_zsd
);


-----------------------------------------------------
-- Bit oversampling HD: x2, SD: x11 :
-- (done in parallel data domain)
-----------------------------------------------------
serial_video_oversampling : os_controller
   port map(
       clk_148          => clk_i,
       reset            => zreset_i,
       sync_reset_i     => mute_i,
       hdsd_in          => hd_zsd,
       data_in          => "01010101010101010101",
       data_out         => os_data,
       enable_out       => enable
   );


-----------------------------------------------------
-- MGT serializer (Rocket IO) :
-----------------------------------------------------
rocket_io_inst :  rocket_io_top
	port map(
      brefclk_i         => brefclk_i,
      brefclk2_i        => brefclk2_i,
      use_clk2_i        => use_clk2_i,
      sync_reset_i      => mute_i,
      power_down_i      => MGT_power_down_i,
      data_clk_i        => clk_i,
      data_i            => os_data,
      fifo_error_o      => rio_fifo_err_o,
      sdi_p_o           => sdi_p_o,
      sdi_n_o           => sdi_n_o
   );


---------------------------------------------------------------
-- Mapping of outputs :
---------------------------------------------------------------
sd_zhd_o <= not hd_zsd;


---------------------------------------------------------------
-- Debug :
---------------------------------------------------------------
--debug_o(0) <= debug_tick;


end Behavioral;