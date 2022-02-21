library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;


entity parallel_to_serial_backend is
generic (
   pc_enable_mode       : std_logic
);
port(
   brefclk_i            : in std_logic;
   brefclk2_i           : in std_logic;
                        
   MGT_power_down_i     : in std_logic;
   sync_reset_i         : in std_logic;
   sync_reset_mgt_i     : in std_logic;
   
   -- video parameters:
   use_clk2_i           : in std_logic;
   hd_zsd_i             : in std_logic;
   
   -- video input:
   data_a_i             : in std_logic_vector(19 downto 0);
   data_b_i             : in std_logic_vector(19 downto 0);
   
   -- clock for video generator:
   clk_o                : out std_logic;
   enable_o             : out std_logic;
   
   -- serial video output:
   sdi_p_a_o            : out std_logic;
   sdi_n_a_o            : out std_logic;
   sdi_p_b_o            : out std_logic;
   sdi_n_b_o            : out std_logic;
                        
   rio_fifo_err_o       : out std_logic
);
end parallel_to_serial_backend;


architecture Behavioral of parallel_to_serial_backend is


signal rio_fifo_err_a         : std_logic;
signal rio_fifo_err_b         : std_logic;
signal clk                    : std_logic;
signal os_data_a              : std_logic_vector(19 downto 0);
signal os_data_b              : std_logic_vector(19 downto 0);
      
      

component os_controller
   port(
       clk_i         :  in  std_logic;
       sync_reset_i  :  in  std_logic;
       hd_zsd_i      :  in  std_logic;
       data_a_i      :  in  std_logic_vector(19 downto 0);
       data_b_i      :  in  std_logic_vector(19 downto 0);
       data_a_o      :  out std_logic_vector(19 downto 0);
       data_b_o      :  out std_logic_vector(19 downto 0);
       enable_o      :  out std_logic
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


begin


--------------------------------------------
-- Select video clock:
--------------------------------------------
clock_selecting : bufgmux
   port map(
      I0 => brefclk_i, 
      I1 => brefclk2_i, 
      S  => use_clk2_i,
      O  => clk
   );

clk_o <= clk;   


-----------------------------------------------------
-- Bit oversampling HD: x2, SD: x11 :
-- (done in parallel data domain)
-----------------------------------------------------
serial_video_oversampling : os_controller
   port map(
       clk_i               => clk,
       sync_reset_i        => sync_reset_i,
       hd_zsd_i            => hd_zsd_i,
       data_a_i            => data_a_i,
       data_b_i            => data_b_i,
       data_a_o            => os_data_a,
       data_b_o            => os_data_b,
       enable_o            => enable_o
   );

   
-----------------------------------------------------
-- MGT serializer (Rocket IO) :
-----------------------------------------------------
rocket_io_inst_a :  rocket_io_top
	port map(
      brefclk_i            => brefclk_i,
      brefclk2_i           => brefclk2_i,
      use_clk2_i           => use_clk2_i,
      sync_reset_i         => sync_reset_mgt_i,
      power_down_i         => MGT_power_down_i,
      data_clk_i           => clk,
      data_i               => os_data_a,
      fifo_error_o         => rio_fifo_err_a,
      sdi_p_o              => sdi_p_a_o,
      sdi_n_o              => sdi_n_a_o
   );

   
rocket_io_inst_b :  rocket_io_top
	port map(
      brefclk_i            => brefclk_i,
      brefclk2_i           => brefclk2_i,
      use_clk2_i           => use_clk2_i,
      sync_reset_i         => sync_reset_mgt_i,
      power_down_i         => MGT_power_down_i,
      data_clk_i           => clk,
      data_i               => os_data_b,
      fifo_error_o         => rio_fifo_err_b,
      sdi_p_o              => sdi_p_b_o,
      sdi_n_o              => sdi_n_b_o
   );

     
rio_fifo_err_o <= rio_fifo_err_a or rio_fifo_err_b;


end Behavioral;