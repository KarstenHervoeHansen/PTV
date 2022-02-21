library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;


entity HD_Gen_Module is
   Port (
      --------- from Mainframe -----------------------------------------------------------------
      mreset_i          : in std_logic;      -- Master Reset fra mainboard
      f4m_i             : in std_logic;      -- Genlock syncronization for NTSC (4 field period)
      f8g_i             : in std_logic;      -- Genlock syncronization for PAL (8 field period)

      --------- clocks -------------------------------------------------------------------------
      clk_27_i          : in std_logic;
      brefclk_p_i       : in std_logic;
      brefclk_n_i       : in std_logic;
      brefclk2_p_i      : in std_logic;
      brefclk2_n_i      : in std_logic;

      --------- from Controller ----------------------------------------------------------------
      ok1_o             : out std_logic;     -- generator 1&2 output levels OK - respons til mainframe
      ok2_o             : out std_logic;     -- generator 3&4 output levels OK - respons til mainframe

      --------- to PLL's -----------------------------------------------------------------------
      pll1_s_o          : out std_logic_vector(3 downto 0);
      pll2_s_o          : out std_logic_vector(3 downto 0);
      
      --------- vcxo : --------------------------------------------------------------------------
      dac_sclk_1_o      : out std_logic;
      dac_dout_1_o      : out std_logic;     -- serial data to external vcxo DAC
      dac_zsync_1_o     : out std_logic;
      dac_sclk_2_o      : out std_logic;
      dac_dout_2_o      : out std_logic;     -- serial data to external vcxo DAC
      dac_zsync_2_o     : out std_logic;
      
      --------- HD Outputs ---------------------------------------------------------------------
      sdi_p_1_i         : in std_logic;
      sdi_n_1_i         : in std_logic;
      sdi_p_1_o         : out std_logic;
      sdi_n_1_o         : out std_logic;

      sd_zhd_1_o        : out std_logic;
      sd_zhd_2_o        : out std_logic;
      sd_zhd_3_o        : out std_logic;
      sd_zhd_4_o        : out std_logic;

      --------- LED's --------------------------------------------------------------------------
      led_o             : out std_logic_vector(3 downto 0);

      --------- Debug I/O ---------------------------------------------------------------------
      red_o             : out std_logic_vector(5 downto 0);
      green_o           : out std_logic_vector(5 downto 0);
      blue_o            : out std_logic_vector(5 downto 0)
   );
end HD_Gen_Module;


architecture Behavioral of HD_Gen_Module is

signal reset                        : std_logic_vector(3 downto 0);
signal led_status                   : std_logic_vector(3 downto 0);
signal led                          : std_logic_vector(3 downto 0);

signal clk_27                       : std_logic;
signal clk1                         : std_logic;
signal clk2                         : std_logic;
signal rx_clk                       : std_logic;
signal brefclk                      : std_logic;
signal brefclk2                     : std_logic;
signal tick_10ms                    : std_logic;
signal tick_10ms_delayed            : std_logic_vector(1 downto 0);
signal vcxo_dac_enable              : std_logic;                                


signal tx_chroma                    : std_logic_vector( 9 downto 0);
signal tx_luma                      : std_logic_vector( 9 downto 0);
signal tx_sdi_data                  : std_logic_vector(19 downto 0);
signal tx_data                      : std_logic_vector(19 downto 0);
signal rx_data                      : std_logic_vector(19 downto 0);
signal rx_sdi_data                  : std_logic_vector(19 downto 0);
signal rx_enable                    : std_logic;
signal tx_enable                    : std_logic;
signal debug_data                   : std_logic_vector(19 downto 0);

signal descramb_data                : std_logic_vector(19 downto 0);


signal mgt_sync_reset               : std_logic;
signal rx_fifo_status               : std_logic_vector(1 downto 0);
signal tx_fifo_error                : std_logic;



signal test_sample_count            : integer range 0 to 8;
signal debug                        : std_logic_vector(3 downto 0);
signal clock_debug                  : std_logic_vector(9 downto 0);
          
component IBUFGDS
   generic(IOSTANDARD: string);
   port( O: out std_ulogic;
         I: in std_ulogic;
         IB:in std_ulogic);
end component;


component BUFG
   Port (
      I : in  std_logic;
      O : out std_logic
   );
end component;


component master_reset_delay
   generic (
      sticky_led              : std_logic_vector(3 downto 0)         -- When high the led remains on until led_off is asserted
   );
   port (
      zreset_i                : in std_logic;                        -- Master reset
      clk_27_i                : in std_logic;
      led_i                   : in std_logic_vector(3 downto 0);
      led_off_i               : in std_logic;
      led_o                   : out std_logic_vector(3 downto 0);
      tick_10ms_o             : out std_logic;
      reset_o                 : out std_logic_vector(3 downto 0)
   );
end component;


component ad5660_serial_out
   port (
      clk_148_i                  : in std_logic;
      data_i                     : in std_logic_vector(15 downto 0);
      new_data_i                 : in std_logic;
      sclk_o                     : out std_logic;
      dout_o                     : out std_logic;
      zsync_o                    : out std_logic
   );
end component;

component rocket_io_top_loopback
   port(
      brefclk_i            : in std_logic;
      brefclk2_i           : in std_logic;
      use_clk2_i           : in std_logic;
      sync_reset_i         : in std_logic;
      power_down_i         : in std_logic;
      tx_data_clk_i        : in std_logic;
      tx_data_i            : in std_logic_vector (19 downto 0);
      rx_recovered_clk_o   : out std_logic;
      rx_data_o            : out std_logic_vector (19 downto 0);
      tx_fifo_error_o      : out std_logic;
      rx_fifo_status_o     : out std_logic_vector (1 downto 0);
      sdi_p_i              : in std_logic;
      sdi_n_i              : in std_logic;
      sdi_p_o              : out std_logic;
      sdi_n_o              : out std_logic
   );
end component;

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

component sdi_bit_restore
   port(
      clk_i             : in  std_logic;
      rio_data_i        : in  std_logic_vector(19 downto 0);
      sdi_data_o        : out std_logic_vector(19 downto 0);
      enable_o          : out std_logic;
      locked_warning_o  : out std_logic;
      locked_error_o    : out std_logic;      
      debug_o           : out std_logic_vector(3 downto 0)
   );
end component;

component multi_sdi_decoder
   port (
      clk            : in  std_logic;      -- word rate clock (74.25 MHz)
      rst            : in  std_logic;      -- async reset
      ce             : in  std_logic;      -- clock enable
      hd_sd          : in  std_logic;      -- 0 = HD, 1 = SD
      d              : in  std_logic_vector(19 downto 0);  -- input data port
      q              : out std_logic_vector(19 downto 0)); -- output data port
end component;

component scram20_top
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

component sdi_bit_framer
   port(
      clk_i             : in std_logic;
      enable_i          : in std_logic;
      hd_zsd_i          : in std_logic;
      data_i            : in std_logic_vector(19 downto 0);
      chroma_o          : out std_logic_vector(9 downto 0);
      luma_o            : out std_logic_vector(9 downto 0);
      trs_present_o     : out std_logic;
      debug_o           : out std_logic_vector(3 downto 0)
   );
end component;

begin

--------------------------------------------------------------
-- Differential clock inputs:
--------------------------------------------------------------
diff_buf_brefclk : IBUFGDS
generic map (IOSTANDARD => "LVDS_25_DT") -- generic map (IOSTANDARD => "LVDS_25")
port map (
   O  => brefclk,
   I  => brefclk_p_i,
   IB => brefclk_n_i
);

diff_buf_brefclk2 : IBUFGDS
generic map (IOSTANDARD => "LVDS_25_DT")  -- generic map (IOSTANDARD => "LVDS_25")
port map (
   O  => brefclk2,
   I  => brefclk2_p_i,
   IB => brefclk2_n_i
);

clk1_for_logic : BUFG
port map(
   I => brefclk,
   O => clk1
);

clk2_for_logic : BUFG
port map(
   I => brefclk2,
   O => clk2
);

clk27_for_logic : BUFG
port map (
   I => clk_27_i,
   O => clk_27
);


--------------------------------------------------------------
-- VCXO control:
--------------------------------------------------------------
dac_enable_generation : process (clk1)
begin
   if clk1'event and clk1 = '1' then
      tick_10ms_delayed <= tick_10ms_delayed(0) & tick_10ms;
      if tick_10ms_delayed = "01" then
         vcxo_dac_enable <= '1';
      else
         vcxo_dac_enable <= '0';
      end if;
   end if;
end process;

serial_data_to_dac_1 : ad5660_serial_out
   port map (
      clk_148_i   => clk1,
      data_i      => "0100000000000000",
      new_data_i  => vcxo_dac_enable,
      sclk_o      => dac_sclk_1_o,
      dout_o      => dac_dout_1_o,
      zsync_o     => dac_zsync_1_o
   );

serial_data_to_dac_2 : ad5660_serial_out
   port map (
      clk_148_i   => clk1,
      data_i      => "0100000000000000",
      new_data_i  => vcxo_dac_enable,
      sclk_o      => dac_sclk_2_o,
      dout_o      => dac_dout_2_o,
      zsync_o     => dac_zsync_2_o
   );

--------------------------------------------------------------
-- Master reset delay, 10ms tick generation and led control :
--------------------------------------------------------------
master_reset_delaying : master_reset_delay
generic map (
   sticky_led        => "0000"
)
port map(
   zreset_i          => mreset_i,
   clk_27_i          => clk_27,
   led_i             => led_status,
   led_off_i         => '0',--not cs1_i,
   led_o             => led,
   tick_10ms_o       => tick_10ms,
   reset_o           => reset
);

led_status <= "1000";
--led_o <= led;

--------------------------------------------------------------
-- Generate signals for TX:
--------------------------------------------------------------
valid_generation : process (clk1)
begin
   if clk1'event and clk1 = '1' then
   	  half_clk_enable <=	not half_clk_enable;
   	  if space_i = '1' then
   	     tmp_space <= '1';
   	  elsif half_clk_enable = '1' then
   	     tmp_space <= '0';
   	  end if;
   	  if half_clk_enable = '1' then
   	     space <= tmp_space;
   	  end if;
      space_delayed <= space_delayed(13 downto 0) & space;
      if space_delayed(12) = '0' then
		valid <= '0';
      else
      	valid <= '1'; 
      end if;
   end if;
end process;

valid_o <= '0';

dummy_data_counting : process (clk1)
begin
   if clk1'event and clk1 = '1' then
   	if valid = '1' then
      if test_sample_count = 8 then
      	test_sample_count <= 0;
      else
      	test_sample_count <= test_sample_count + 1;
      end if;
     end if;
   end if;
end process;

test_data_generation : process (test_sample_count)
begin
   case test_sample_count is
         when 0 => 
            tx_chroma   <= "0000000001";
            tx_luma     <= "1111111110";
            debug_data	 <= "11001100110010101010";
         when 1 => 
            tx_chroma   <= "0000000000";
            tx_luma     <= "1111111111";
            debug_data	 <= "11110000111100001100";
         when 2 => 
            tx_chroma   <= "1111111111";  -- SD TRS
            tx_luma     <= "0000000000";  -- SD TRS
            debug_data	 <= "00001111000011110000";
         when 3 => 
            tx_chroma   <= "0000000000";  -- SD TRS
            tx_luma     <= "1111111111";  -- SD TRS
            debug_data	 <= "00000000111111110000";
         when 4 => 
            tx_chroma   <= "0000000000";
            tx_luma     <= "1111111111";
            debug_data	 <= "11110000000011111111";
         when 5 => 
            tx_chroma   <= "0000000000";
            tx_luma     <= "1111111111";
            debug_data	 <= "11111111000000001111";
         when 6 => 
            tx_chroma   <= "0000000000";
            tx_luma     <= "1111111111";
            debug_data	 <= "00000000000000000000";
         when 7 => 
            tx_chroma   <= "0000000000";
            tx_luma     <= "1111111111";
            debug_data	 <= "00000000000000000000";
         when others => 
            tx_chroma   <= "0000000000";
            tx_luma     <= "1111111111";
            debug_data	 <= "00000000000000000000";
         end case;
end process;


-----------------------------------------------------
-- Scrambling (done in 20 bit parallel data domain):
-----------------------------------------------------
bit_scrambling_a : scram20_top
   port map(
      clk_i                => clk1,
      zreset_i             => '1',  --zreset_i,
      bypass_i             => '0',
      enable_i             => tx_enable,
      luma_i               => "0000000000", --tx_luma,
      chroma_i             => "1111111111", --tx_chroma,
      data_o               => tx_sdi_data
   );

-----------------------------------------------------
-- Oversampling :
-----------------------------------------------------
serial_data_oversampling : os_controller
   port map(
       clk_i               => clk1,
       sync_reset_i        => mgt_sync_reset,
       hd_zsd_i            => '0',
       data_a_i            => "11110000111000110010",--debug_data,--tx_sdi_data,
       data_b_i            => "00000000000000000000",
       data_a_o            => tx_data,
       enable_o            => tx_enable
   );

   
-----------------------------------------------------
-- MGT serializers (Rocket IO) :
-----------------------------------------------------
rocket_io_inst :  rocket_io_top_loopback
   port map(
      brefclk_i            => brefclk,
      brefclk2_i           => brefclk2,
      use_clk2_i           => '0',
      sync_reset_i         => mgt_sync_reset,
      power_down_i         => '0',
      tx_data_clk_i        => clk1,
      tx_data_i            => tx_data,
      tx_fifo_error_o      => open,
      rx_fifo_status_o     => open,
      sdi_p_i              => sdi_p_1_i,
      sdi_n_i              => sdi_n_1_i,
      sdi_p_o              => sdi_p_1_o,
      sdi_n_o              => sdi_n_1_o,
      rx_recovered_clk_o   => rx_clk,
      rx_data_o            => rx_data
   );

-------------------------------------------------------------
-- SDI bit restoring:
-- In HD data is passed through. In SD the SDI signal is
-- sampled at 20*148,5e6 Hz ~= 3GHz. I.E. one SD bit is sampled
-- approximately 11 times. The data rate is downconverted and
-- 20 bit parallel data is output with a clock enable signal on
-- the recovered clock from the MGT.
-------------------------------------------------------------
sdi_bit_restoring : sdi_bit_restore
   port map(
      clk_i               => clk1,		--rx_clk,
      rio_data_i          => tx_data,	--rx_data,
      sdi_data_o          => rx_sdi_data,
      enable_o            => rx_enable,
      locked_warning_o    => open,
      locked_error_o      => open,          
      debug_o             => led_o
   );

-------------------------------------------------------------
-- Descrambling (see Xilinx xapp514):
-- Note that it always runs in HD because 20 bit data width is
-- used in SD as well in this design.
-------------------------------------------------------------
sdi_decrambling : multi_sdi_decoder
   port map(
      clk         => clk1,--rx_clk,        -- word rate clock (74.25 MHz)
      rst         => '0',           -- async reset
      ce          => rx_enable,     -- clock enable
      hd_sd       => '0',           -- 0 = HD, 1 = SD
      d           => rx_sdi_data,   -- input data port
      q           => descramb_data  -- output data port
   );
   
-------------------------------------------------------------
-- SDI bit framing:
-------------------------------------------------------------
sdi_bit_framing : sdi_bit_framer
   port map(
      clk_i             => rx_clk,
      enable_i          => rx_enable,
      hd_zsd_i          => '0',
      data_i            => descramb_data,
      chroma_o          => open,
      luma_o            => open,
      trs_present_o     => open,
      debug_o           => open
   );
   
-------------------------------------------------------------
-- Mapping of output signals:
-------------------------------------------------------------
ok1_o <= '1';
ok2_o <= '1';

sd_zhd_1_o           <= '1';
sd_zhd_2_o           <= '1';
sd_zhd_3_o           <= '1';
sd_zhd_4_o           <= '1';


-------------------------------------------------------------
-- Signals for debug:
-------------------------------------------------------------
blue_o         <= rx_sdi_data(17 downto 12);
green_o        <= rx_sdi_data(11 downto 6);
red_o          <= rx_sdi_data(5 downto 0);
               
pll1_s_o       <= "0110";
pll2_s_o       <= "0111";

end Behavioral;