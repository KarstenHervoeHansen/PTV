library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;

----------------------------------------------------------------------------------------------
-- 2005-09-26: Created                    Thomas Holm Hansen
--
----------------------------------------------------------------------------------------------

entity HD_Gen_Channel is   
   generic (
      channel_id        : integer;
      pc_enable_mode    : std_logic
   );
   Port (
--------- to/from Module ---------------------------------------------------------------------
      reset_i           : in std_logic;      
      reset_delayed_i   : in std_logic;      
      f1485_i           : in std_logic;
      brefclk_i         : in std_logic;
      brefclk2_i        : in std_logic;
      tick_10ms_i       : in std_logic;

--------- Sync signals -----------------------------------------------------------------------
      f4m_1484_i        : in std_logic;      -- sync reference - HD systems
      f4m_1485_i        : in std_logic;      -- sync reference - SD NTSC
      f8g_i             : in std_logic;      -- sync reference
      f2398_i           : in std_logic;
      f24_i             : in std_logic;
      f30_i             : in std_logic;

--------- fra CPU ----------------------------------------------------------------------------
      cs_i              : in std_logic;      -- chip select
      sck_i             : in std_logic;      -- serial clock
      mosi_i            : in std_logic;      -- master out serial data in

--------- HD Outputs -------------------------------------------------------------------------
      sd_zhd_o          : out std_logic;
      sdi_p_a_o         : out std_logic;
      sdi_n_a_o         : out std_logic;
      sdi_p_b_o         : out std_logic;
      sdi_n_b_o         : out std_logic;
      
--------- Monitoring I/O ---------------------------------------------------------------------
      video_reset_o     : out std_logic;
      rio_fifo_err_o    : out std_logic;

--------- Debug I/O ---------------------------------------------------------------------
      pc_ibf_i          : in std_logic := '0';
      pc_data_o         : out std_logic_vector(9 downto 0);
      pc_zstb_o         : out std_logic;
      debug_o           : out std_logic_vector(3 downto 0)
   );
end HD_Gen_Channel;


architecture Behavioral of HD_Gen_Channel is


signal video_clk           : std_logic;
signal enable              : std_logic;
signal pc_enable           : std_logic;
signal hw_enable           : std_logic;
signal use_f1485           : std_logic;
signal genlock_sync        : std_logic;
signal hd_zsd              : std_logic;

signal ui_system           : std_logic_vector(11 downto 0);
signal ui_pattern          : std_logic_vector(15 downto 0);
signal system              : std_logic_vector(11 downto 0);
signal pattern             : std_logic_vector(15 downto 0);
signal ui_timing           : std_logic_vector(23 downto 0);
signal rio_fifo_err        : std_logic;
signal sync_reset_frame    : std_logic;
signal sync_reset_mgt      : std_logic;

signal data_a              : std_logic_vector(19 downto 0);
signal data_b              : std_logic_vector(19 downto 0);
                           
signal frame_end           : std_logic;
signal mgt_power_down      : std_logic;
                           
signal f4m_1484            : std_logic;
signal f4m_1485            : std_logic;
signal f8g                 : std_logic;
signal f2398               : std_logic;
signal f24                 : std_logic;
signal f30                 : std_logic;
                           
signal tsg_level           : std_logic;
signal sampled_tsg_lvl     : std_logic;
signal lvl_count           : std_logic_vector(2 downto 0);
signal spl_is_zero         : std_logic;
signal debug               : std_logic_vector(9 downto 0);
                           
signal click_timing        : std_logic_vector(11 downto 0);
signal click_modulation    : std_logic;
signal debug_pattern       : std_logic_vector(15 downto 0);

signal audio_level_index   : std_logic_vector(7 downto 0);
signal audio_signal_index  : std_logic_vector(7 downto 0);
signal insert_text         : std_logic;

signal bmp_v_position      : std_logic_vector(15 downto 0);
signal bmp_h_position      : std_logic_vector(15 downto 0);
signal bmp_h_size          : std_logic_vector(10 downto 0);
signal bmp_v_size          : std_logic_vector(10 downto 0);
signal bmp_scale           : std_logic_vector( 7 downto 0);
signal bmp_back_text_color : std_logic_vector( 7 downto 0);
signal bmp_movement        : std_logic_vector( 7 downto 0);
signal bmp_on_zoff         : std_logic;

signal text_ram_addr       : std_logic_vector( 8 downto 0);
signal text_ram_en         : std_logic;
signal text_ram_data       : std_logic_vector( 7 downto 0);

signal pc_data             : std_logic_vector(9 downto 0);
signal pc_zstb             : std_logic;


component channel_controller
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
      f4m_1484_i              : in std_logic;      -- sync reference - HD systems
      f4m_1485_i              : in std_logic;      -- sync reference - SD NTSC
      f8g_i                   : in std_logic;      -- sync reference
      f2398_i                 : in std_logic;
      f24_i                   : in std_logic;
      f30_i                   : in std_logic;
      frame_end_i             : in std_logic;
      rio_fifo_error_i        : in std_logic;
      hd_zsd_o                : out std_logic;
      use_f1485_o             : out std_logic;
      system_o                : out std_logic_vector(11 downto 0);
      pattern_o               : out std_logic_vector(15 downto 0);
      sync_reset_frame_o      : out std_logic;
      sync_reset_mgt_o        : out std_logic;
      mgt_power_down_o        : out std_logic
   );

end component;


component video_generator
   generic (
      pc_enable_mode          : std_logic
   );                          
   port(                      
      clk_i                   : in std_logic;
      en_i                    : in std_logic;    
      sync_reset_i            : in std_logic;
      system_i                : in std_logic_vector(11 downto 0);
      pattern_i               : in std_logic_vector(15 downto 0);
      audio_level_index_i     : in std_logic_vector(7 downto 0);
      audio_signal_index_i    : in std_logic_vector(7 downto 0);
      click_timing_i          : in std_logic_vector(11 downto 0);
      click_modulation_i      : in std_logic;
      v_pos_i                 : in std_logic_vector(3 downto 0);
      h_pos_i                 : in std_logic_vector(3 downto 0);
      bmp_heigth_i            : in std_logic_vector(10 downto 0);    -- must be >= 1
      bmp_width_i             : in std_logic_vector(10 downto 0);    -- must be >= 1
      text_on_i               : in std_logic;
      text_color_presence_i   : in std_logic_vector(2 downto 0);
      back_color_presence_i   : in std_logic_vector(2 downto 0);
      text_size_i             : in std_logic_vector(1 downto 0);
      movement_i              : in std_logic_vector(1 downto 0);     -- "00": no movement, "01": vertical, "10": horizontal
      text_ram_data_i         : in std_logic_vector(7 downto 0);
      text_ram_addr_o         : out std_logic_vector(8 downto 0);
      text_ram_enable_o       : out std_logic;
      data_a_o                : out std_logic_vector(19 downto 0);
      data_b_o                : out std_logic_vector(19 downto 0);
      frame_end_o             : out std_logic
   );
end component;


component parallel_to_serial_backend
generic (
   pc_enable_mode       : std_logic
);
port(
   brefclk_i            : in std_logic;
   brefclk2_i           : in std_logic;
   mgt_power_down_i     : in std_logic;
   sync_reset_i         : in std_logic;
   sync_reset_mgt_i     : in std_logic;
   use_clk2_i           : in std_logic;
   hd_zsd_i             : in std_logic;
   data_a_i             : in std_logic_vector(19 downto 0);
   data_b_i             : in std_logic_vector(19 downto 0);
   clk_o                : out std_logic;
   enable_o             : out std_logic;
   sdi_p_a_o            : out std_logic;
   sdi_n_a_o            : out std_logic;
   sdi_p_b_o            : out std_logic;
   sdi_n_b_o            : out std_logic;
   rio_fifo_err_o       : out std_logic
);
end component;


component parallel_to_pc_backend
   port(
      sync_reset_i         : in std_logic;
      clk_i                : in std_logic;
      enable_o             : out std_logic;
      data_a_i             : in std_logic_vector(19 downto 0);
      data_b_i             : in std_logic_vector(19 downto 0);
      pc_ibf_i             : in std_logic;
      pc_data_o            : out std_logic_vector(9 downto 0);
      pc_zstb_o            : out std_logic
   );
end component;


component serial_interface
   generic (
      channel_id              : integer
   );
   Port (
      clk_i                   : in  std_logic;     -- clock
      reset_i                 : in  std_logic;     -- sync_reset
      cs_i                    : in  std_logic;     -- chip select
      sck_i                   : in  std_logic;     -- serial clock
      mosi_i                  : in  std_logic;     -- master out serial data in
      system_o                : out std_logic_vector(11 downto 0);
      pattern_o               : out std_logic_vector(15 downto 0);
      timing_o                : out std_logic_vector(23 downto 0);
      click_timing_o          : out std_logic_vector(11 downto 0);
      audio_level_index_o     : out std_logic_vector(7 downto 0);
      audio_signal_index_o    : out std_logic_vector(7 downto 0);
      bmp_v_position_o        : out std_logic_vector(15 downto 0);
      bmp_h_position_o        : out std_logic_vector(15 downto 0);
      bmp_h_size_o            : out std_logic_vector(7 downto 0);
      bmp_v_size_o            : out std_logic_vector(7 downto 0);
      bmp_scale_o             : out std_logic_vector(7 downto 0);
      bmp_back_text_color_o   : out std_logic_vector(7 downto 0);
      bmp_movement_o          : out std_logic_vector(7 downto 0);
      bmp_on_zoff_o           : out std_logic;
      text_ram_clk_i          : in  std_logic;     -- clock for reading text ram
      text_ram_addr_i         : in  std_logic_vector( 8 downto 0);
      text_ram_en_i           : in  std_logic;     -- enable signal for reading text ram
      text_ram_data_o         : out std_logic_vector( 7 downto 0)
   );
end component;


begin


-------------------------------------------------------
-- Serial interface 
-------------------------------------------------------
serial_interfacing : serial_interface
generic map (
   channel_id               => channel_id
)
port map (
   clk_i                    => f1485_i,
   reset_i                  => reset_i,
   cs_i                     => cs_i,   
   sck_i                    => sck_i , 
   mosi_i                   => mosi_i, 
   system_o                 => ui_system,                
   pattern_o                => ui_pattern,               
   timing_o                 => ui_timing,                
   click_timing_o           => click_timing,          
   audio_level_index_o      => audio_level_index,     
   audio_signal_index_o     => audio_signal_index,    
   bmp_v_position_o         => bmp_v_position,        
   bmp_h_position_o         => bmp_h_position,        
   bmp_h_size_o             => bmp_h_size(7 downto 0),
   bmp_v_size_o             => bmp_v_size(7 downto 0),
   bmp_scale_o              => bmp_scale,             
   bmp_back_text_color_o    => bmp_back_text_color,   
   bmp_movement_o           => bmp_movement,          
   bmp_on_zoff_o            => bmp_on_zoff,           
   text_ram_clk_i           => video_clk,                      
   text_ram_addr_i          => text_ram_addr,      
   text_ram_en_i            => text_ram_en,        
   text_ram_data_o          => text_ram_data      
);

bmp_h_size(10 downto 8) <= (others => '0');
bmp_v_size(10 downto 8) <= (others => '0');


--------------------------------------------
-- Controller:
--------------------------------------------
system_controller : channel_controller
   generic map (
      pc_enable_mode          => pc_enable_mode
   )                          
   port map (
      sync_reset_i            => reset_delayed_i,
      clk_i                   => f1485_i,
      en_i                    => '1',
      tick_10ms_i             => tick_10ms_i,
      video_clk_i             => video_clk,
      video_en_i              => enable,
      ui_system_i             => ui_system, 
      ui_pattern_i            => ui_pattern,
      ui_timing_i             => ui_timing,
      f4m_1484_i              => f4m_1484_i,
      f4m_1485_i              => f4m_1485_i,
      f8g_i                   => f8g_i,     
      f2398_i                 => f2398_i,   
      f24_i                   => f24_i,     
      f30_i                   => f30_i,     
      frame_end_i             => frame_end,
      rio_fifo_error_i        => rio_fifo_err,
      hd_zsd_o                => hd_zsd,
      use_f1485_o             => use_f1485,       
      system_o                => system,          
      pattern_o               => pattern,         
      sync_reset_frame_o      => sync_reset_frame,
      sync_reset_mgt_o        => sync_reset_mgt,  
      mgt_power_down_o        => mgt_power_down   
   );

   
video_reset_o  <= sync_reset_frame;
sd_zhd_o       <= not hd_zsd;   


video_generator_instance : video_generator
   generic map (
      pc_enable_mode          => pc_enable_mode
   )                          
   port map (
      clk_i                   => video_clk,
      en_i                    => enable,
      sync_reset_i            => sync_reset_frame,                    
      system_i                => system,            
      pattern_i               => pattern,                        
      audio_level_index_i     => audio_level_index,              
      audio_signal_index_i    => audio_signal_index,             
      click_timing_i          => click_timing,                   
      click_modulation_i      => '0',                            
      v_pos_i                 => bmp_v_position(3 downto 0),     
      h_pos_i                 => bmp_h_position(3 downto 0),     
      bmp_heigth_i            => bmp_v_size,                     
      bmp_width_i             => bmp_h_size,                     
      text_on_i               => bmp_on_zoff,                    
      text_color_presence_i   => bmp_back_text_color(2 downto 0),
      back_color_presence_i   => bmp_back_text_color(6 downto 4),
      text_size_i             => bmp_scale(1 downto 0),          
      movement_i              => bmp_movement(1 downto 0),       
      text_ram_data_i         => text_ram_data,                  
      text_ram_addr_o         => text_ram_addr,                  
      text_ram_enable_o       => text_ram_en,                    
      data_a_o                => data_a,
      data_b_o                => data_b,
      frame_end_o             => frame_end                       
   );
   
   
----------------------------------------------------
-- Backend with oversampling, and serializer:
----------------------------------------------------   
serializer_backend : parallel_to_serial_backend
   generic map (
      pc_enable_mode    => pc_enable_mode
   )
   port map (
      brefclk_i         => brefclk_i,
      brefclk2_i        => brefclk2_i,
      mgt_power_down_i  => mgt_power_down,
      sync_reset_i      => sync_reset_frame,
      sync_reset_mgt_i  => sync_reset_mgt,
      use_clk2_i        => use_f1485,
      hd_zsd_i          => hd_zsd,
      data_a_i          => data_a,
      data_b_i          => data_b,
      clk_o             => video_clk,
      enable_o          => hw_enable,
      sdi_p_a_o         => sdi_p_a_o,
      sdi_n_a_o         => sdi_n_a_o,
      sdi_p_b_o         => sdi_p_b_o,
      sdi_n_b_o         => sdi_n_b_o,
      rio_fifo_err_o    => rio_fifo_err
   );

rio_fifo_err_o <= rio_fifo_err;


-------------------------------------------------------------
-- Debug:
-------------------------------------------------------------
pc_debug_backend : parallel_to_pc_backend
   port map (
      sync_reset_i      => sync_reset_frame,
      clk_i             => video_clk,
      enable_o          => pc_enable,
      data_a_i          => data_a,
      data_b_i          => data_b,
      pc_ibf_i          => pc_ibf_i, 
      pc_data_o         => pc_data,
      pc_zstb_o         => pc_zstb
   );
   
   
select_enable : process (pc_enable, hw_enable)
begin
   if pc_enable_mode = '1' then
      enable      <= pc_enable;
      pc_data_o   <= pc_data;
      pc_zstb_o   <= pc_zstb;
   else
      enable      <= hw_enable;
      pc_data_o   <= (others => '0');
      pc_zstb_o   <= '0';
   end if;
end process;


debug_o  <= use_f1485 & "000";

 
end Behavioral;