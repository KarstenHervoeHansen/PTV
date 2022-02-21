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
   frame_start_i      : in std_logic;
   MGT_power_down_i   : in std_logic;

   -- video parameters:
   system_i           : in std_logic_vector(7 downto 0);
   pattern_i          : in std_logic_vector(15 downto 0);

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


--------------------------------------------
-- pattern registering at frame start:
-------------------------------------------- 
pattern_registering : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      pattern <= (others => '0');
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         pattern <= (others => '0');
      elsif enable = '1' then
         if frame_end = '1' then
            pattern <= pattern_i;
         end if;
      end if;
   end if;
end process;


--------------------------------------------
-- Generate active video :
-------------------------------------------- 
colorbar_type_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      colorbar_type <= (others => '0');
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         colorbar_type <= (others => '0');
      elsif enable = '1' then
         case pattern(1 downto 0) is
            when "00" =>
               colorbar_type <= "00";     -- 100/0/100/0
            when "01" =>
               colorbar_type <= "01";     -- 100/0/75/0
            when others =>    
               colorbar_type <= "11";     -- 75/0/75/0
         end case;
      end if;
   end if;
end process;


colorbar_index_generation : colorbar_generator
   port map(
      clk_i                => clk_i,
      zreset_i             => zreset_i,
      en_i                 => enable,
      sync_reset_i         => sync_reset,
      asl_i                => asl,
      colorbar_type_i      => colorbar_type,
      sav_tick_advanced_i  => sav_tick_advanced,
      color_index_o        => color_index_cbar
   );


pattern_selection : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      color_index <= (others => '0');
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         color_index <= (others => '0');
      elsif enable = '1' then
         case pattern(15 downto 8) is
            when X"01" =>     -- Color Bar:
               color_index <= color_index_cbar;
            --when X"02" =>     -- Pluge:
            --   color_index <= "1000";
            when others =>    -- Black:
               color_index <= "0000";
         end case;
      end if;
   end if;
end process;


color_generation : color_generator
   port map(
      clk_i             => clk_i,
      zreset_i          => zreset_i,
      en_i              => enable,
      sync_reset_i      => sync_reset,
      color_system_i    => color_system,
      color_index_i     => color_index,
      y_o               => y_av_cbar,
      cb_o              => cb_av_cbar,
      cr_o              => cr_av_cbar
   );

   
--------------------------------------------
-- Filtering of active video:
--------------------------------------------
active_video_filtering : luma_chroma_fir_12bit
   port map(
      clk_i             => clk_i,
      zreset_i          => zreset_i,
      en_i              => enable,
      sync_reset_i      => sync_reset,
      cr_zcb_i          => cr_zcb,
      y_i               => y_av_cbar,
      cb_i              => cb_av_cbar,
      cr_i              => cr_av_cbar,
      luma_o            => luma_av_fir,
      chroma_o          => chroma_av_fir
   );

   
--------------------------------------------
-- SDI Checkfield:
--------------------------------------------
Checkfield_generation : checkfield_generator
   port map (                  
      clk_i                => clk_i,      
      zreset_i             => zreset_i,   
      en_i                 => enable,     
      sync_reset_i         => sync_reset, 
      system_i             => system_i,
      interlaced_i         => interlaced,
      hd_zsd_i             => hd_zsd,
      fvh_i                => fvh,
      eav_tick_advanced_i  => eav_tick_advanced,
      frame_end_i          => frame_end,
      luma_o               => luma_check, 
      chroma_o             => chroma_check
   );
   

--------------------------------------------
-- Active video selection:
--------------------------------------------
video_selection : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      luma_av   <= "0001000000";
      chroma_av <= "1000000000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         luma_av   <= "0001000000";
         chroma_av <= "1000000000";
      elsif enable = '1' then
         case pattern(15 downto 8) is
            when X"02" =>     -- SDI Checkfield:  X"03"
               luma_av <= luma_check;
               chroma_av <= chroma_check;
            when others =>    -- All other patterns:
               luma_av   <= luma_av_fir;
               chroma_av <= chroma_av_fir; 
         end case;
--luma_av   <= luma_av_fir;  
--chroma_av <= chroma_av_fir;
      end if;
   end if;
end process;
   

--------------------------------------------
-- Active video legalizer (none ff or 00 words):
--------------------------------------------
video_legalizer : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      luma_av_legal   <= "0001000000";
      chroma_av_legal <= "1000000000";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         luma_av_legal   <= "0001000000";
         chroma_av_legal <= "1000000000";
      elsif enable = '1' then
         if luma_av(9 downto 2) = "11111111" then
            luma_av_legal <= "1111111011";
         elsif luma_av(9 downto 2) = "00000000" then
            luma_av_legal <= "0000000100";
         else
            luma_av_legal <= luma_av;
         end if;
         if chroma_av(9 downto 2) = "11111111" then
            chroma_av_legal <= "1111111011";
         elsif chroma_av(9 downto 2) = "00000000" then
            chroma_av_legal <= "0000000100";
         else
            chroma_av_legal <= chroma_av;
         end if;
      end if;
   end if;
end process;

--luma_av_legal     <= "0001000000";
--chroma_av_legal   <= "1000000000";

   
--------------------------------------------
-- Generate video system:
--------------------------------------------
system_generation : system_generator
   port map(
      clk_i                => clk_i,
      zreset_i             => zreset_i,
      en_i                 => enable,
      frame_start_i        => frame_start_i,
      system_i             => system_i,
      luma_video_i         => luma_av_legal,
      chroma_video_i       => chroma_av_legal,
      luma_h_blank_i       => "0001000000",
      chroma_h_blank_i     => "1000000000",
      luma_v_blank_i       => "0001000000",
      chroma_v_blank_i     => "1000000000",
      fvh_o                => fvh,
      sav_tick_advanced_o  => sav_tick_advanced,
      eav_tick_advanced_o  => eav_tick_advanced,
      frame_end_o          => frame_end,
      hd_zsd_o             => hd_zsd,
      interlaced_o         => interlaced,
      asl_o                => asl,
      color_system_o       => color_system,
      cr_zcb_o             => cr_zcb,
      luma_o               => luma,
      chroma_o             => chroma,
      sync_reset_o         => sync_reset,
      timing_change_o      => timing_change_o,
      debug_o              => debug_o
   );


-----------------------------------------------------
-- Scrambling (done in 20 bit parallel data domain):
-----------------------------------------------------
bit_scrambling : scram20_top
   port map(
      clk_i           => clk_i,
      zreset_i        => zreset_i,
      bypass_i        => '0',
      enable_i        => enable,
      luma_i          => luma,
      chroma_i        => chroma,
      data_o          => scramb_data
   );


-----------------------------------------------------
-- Bit oversampling HD: x2, SD: x11 :
-- (done in parallel data domain)
-----------------------------------------------------
serial_video_oversampling : os_controller
   port map(
       clk_148          => clk_i,
       reset            => zreset_i,
       sync_reset_i     => sync_reset,
       hdsd_in          => hd_zsd,
       data_in          => scramb_data,
       data_out         => os_data,
       enable_out       => enable
   );


---------------------------------------------------------------
-- Debug process for measuring delay in rocket IO:
---------------------------------------------------------------
--debug_counter : process (clk_i, zreset_i)
--begin
--   if zreset_i = '0' then
--      debug_count <= "0000";
--   elsif clk_i'event and clk_i = '1' then
--      debug_count <= debug_count + 1;
--   end if;
--end process;
--
--
--debug_data_generation : process (clk_i, zreset_i)
--begin
--   if zreset_i = '0' then
--      debug_data <= (others => '0');
--   elsif clk_i'event and clk_i = '1' then
--      if debug_count = "0000" then
--         debug_data <= "11111111111111111111";
--         debug_tick <= '1';
--      else
--         debug_data <= "11111111110000000000";
--         debug_tick <= '0';
--      end if;
--   end if;
--end process;


-----------------------------------------------------
-- MGT serializer (Rocket IO) :
-----------------------------------------------------
rocket_io_inst :  rocket_io_top
	port map(
      brefclk_i         => brefclk_i,
      brefclk2_i        => brefclk2_i,
      use_clk2_i        => use_clk2_i,
      sync_reset_i      => mute,
      power_down_i      => MGT_power_down_i,
      data_clk_i        => clk_i,
      data_i            => os_data,
      fifo_error_o      => rio_fifo_err_o,
      sdi_p_o           => sdi_p_o,
      sdi_n_o           => sdi_n_o
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
--debug_o(0) <= debug_tick;


end Behavioral;