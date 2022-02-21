library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;


entity video_generator is
port(
   clk_i              : in std_logic;
   reset_i            : in std_logic;
   mute_i             : in std_logic;
   frame_start_i      : in std_logic;
   MGT_power_down_i   : in std_logic;

   -- video parameters:
   system_i           : in std_logic_vector(7 downto 0);
   pattern_i          : in std_logic_vector(15 downto 0);

   -- serial video output:
   sdi_p_o            : out std_logic;
   sdi_n_o            : out std_logic;

   -- timing status:
   timing_change_o    : out std_logic;
   rio_fifo_err_o     : out std_logic;
);
end video_generator;


architecture Behavioral of video_generator is


signal enable              : std_logic;
signal mute                : std_logic;
signal sync_reset          : std_logic;
signal hd_zsd              : std_logic;
signal luma                : std_logic_vector(9 downto 0);
signal chroma              : std_logic_vector(9 downto 0);
signal scramb_data         : std_logic_vector(19 downto 0);
signal os_data             : std_logic_vector(19 downto 0);
signal txdata              : std_logic_vector(19 downto 0);


COMPONENT scram20_top
   PORT (
      clk_i        : IN std_logic;
      zreset_i     : IN std_logic;                       -- active low reset
      bypass_i     : IN std_logic;                       -- bypass scrambler
      enable_i     : IN std_logic;                       
      luma_i       : IN std_logic_vector(9 downto 0);    -- raw luma data in
      chroma_i     : IN std_logic_vector(9 downto 0);    -- raw chroma data in
      data_o       : out std_logic_vector(19 downto 0)); -- parallel scrambled output
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

      luma_ancil_i         : in std_logic_vector(9 downto 0);
      chroma_ancil_i       : in std_logic_vector(9 downto 0);

      ---system parameter outputs:
      hd_zsd_o             : out std_logic;

            -- frame control signals:
      line_o               : out std_logic_vector(10 downto 0);
      fvh_o                : out std_logic_vector(2 downto 0);    -- blanking bits
      cb_zcr_o             : out std_logic;

      -- parallel data outputs:
      luma_o               : out std_logic_vector(9 downto 0);
      chroma_o             : out std_logic_vector(9 downto 0);

      -- other output signals:
      sync_reset_o         : out std_logic;                       -- syncronious reset output to oversampler:
      timing_change_o      : out std_logic
   );
end component;


component rocket_io_top
port(
   rio_clk_1_i       : in std_logic;
   rio_clk_2_i       : in std_logic;
   use_rio_clk_1_i   : in std_logic;
   sync_reset_i      : in std_logic;
   power_down_i      : in std_logic;
   data_clk_i        : in std_logic;
   data_i            : in  std_logic_vector (19 downto 0);
  
   fifo_error_o      : out std_logic;
   sdi_p_o           : out std_logic;     -- serial video outputs:
   sdi_n_o           : out std_logic;
);



end component;

   
begin


--------------------------------------------
-- Generate video system:
--------------------------------------------
system_generation : system_generator
   port map(
      clk_i             => clk_i,
      zreset_i          => zreset_i,
      en_i              => enable,
      frame_start_i     => frame_start_i,
      system_i          => system_i,
      luma_video_i      => "0001000000",
      chroma_video_i    => "1000000000",
      luma_ancil_i      => "1010101010",
      chroma_ancil_i    => "0011001100",

      hd_zsd_o          => hd_zsd,
      luma_o            => luma,
      chroma_o          => chroma,
      sync_reset_o      => sync_reset,
      timing_change_o   => timing_change_o,
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


-----------------------------------------------------
-- MGT serializer (Rocket IO) :
-----------------------------------------------------
serial_data_output :  MY_GT11
	port map(
      rio_clk_1_i       => rio_f1484_i,
      rio_clk_2_i       => rio_f1485_i,
      use_rio_clk_1_i   => use_f1884_i,
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
 
   
   
end Behavioral;