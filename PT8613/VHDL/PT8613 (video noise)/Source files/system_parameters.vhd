library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity system_parameters is
   port (
      system_i                : in std_logic_vector(7 downto 0);
                              
      hd_zsd_o                : out std_logic;                       -- 0: sd, 1: hd
      interlaced_transport_o  : out std_logic;                       -- 1: interlaced, 0: progressive
		asl_o                   : out std_logic_vector(10 downto 0);   -- active samples per line
      lpf_o                   : out std_logic_vector(10 downto 0);   -- lines per frame
      hb1s_o                  : out std_logic_vector(11 downto 0);   -- horizontal blanking 1 length (samples), asl+10+h_blank1+h_blank2 =tsl
      hb2s_o                  : out std_logic_vector(11 downto 0);   -- horizontal blanking 2 length (samples)
                              
      field1_end_o            : out std_logic_vector(10 downto 0);
      field2_end_o            : out std_logic_vector(10 downto 0);
      f_start_o               : out std_logic;
      line_start_o            : out std_logic_vector(10 downto 0);
                              
      field1_vblnk_end_o      : out std_logic_vector(10 downto 0);   -- line numbers for vertical blanking borders
      field1_av_end_o         : out std_logic_vector(10 downto 0);
      field2_vblnk_end_o      : out std_logic_vector(10 downto 0);
      field2_av_end_o         : out std_logic_vector(10 downto 0)
   );
end system_parameters;


architecture Behavioral of system_parameters is


signal lpf_index                 : std_logic_vector(2 downto 0);
signal spl_index                 : std_logic_vector(1 downto 0);
signal hd_zsd                    : std_logic;
signal interlaced_transport      : std_logic;
signal lpf_interlaced_transport  : std_logic_vector(3 downto 0);


constant delay_adjust      : integer := -7;  -- Adjusts timing in video sample domain ("enable" domain)
                                             -- by offsetting hb1s and hb2s

begin


-- hdformat table :
-- 0000     1080i/30 & 1080i/29.97
-- 0001     1080i/25
-- 0010     1080p/30
-- 0011     1080p/25
-- 0100     1080p/24 & 1080p/23.98
-- 0101     1035i/30 & 1035i/29.97
-- 0110     720p/60 & 720p/59.94
-- 0111     720p/50
-- 1000     720p/30 & 720p/29.97
-- 1001     720p/25
-- 1010     720p/24 & 720p/23.98


---------------------------------------------------------------
-- lines per frame dependent on system :
-- 487: "000", 576: "001", 720: "010", 1035: "011", 1080: "100"
---------------------------------------------------------------
with system_i select
   lpf_index <=      "100" when X"A0",   -- 1080i/30
                     "100" when X"B0",   -- 1080i/29.97
                     "100" when X"82",   -- 1080p/30
                     "100" when X"92",   -- 1080p/29.97
                     "100" when X"83",   -- 1080p/25
                     "100" when X"84",   -- 1080p/24
                     "100" when X"94",   -- 1080p/23.98
                     "100" when X"A2",   -- 1080sF/30
                     "100" when X"B2",   -- 1080sF/29.97
                     "100" when X"A3",   -- 1080sF/25
                     "100" when X"A4",   -- 1080sF/24
                     "100" when X"B4",   -- 1080sF/23.98
                     "010" when X"86",   -- 720p/60
                     "010" when X"96",   -- 720p/59.94
                     "010" when X"87",   -- 720p/50
                     "010" when X"88",   -- 720p/30
                     "010" when X"98",   -- 720p/29.97
                     "010" when X"89",   -- 720p/25
                     "010" when X"8A",   -- 720p/24
                     "010" when X"9A",   -- 720p/23.98
                     "000" when X"60",   -- NTSC 525i/29.97
                     "001" when X"20",   -- PAL 625i/25
                     "100" when others;  -- 1080i/25  X"A1"


---------------------------------------------------------------
-- samples per line dependent on system :
-- 720: "00", 1280: "01", 1920: "10"
---------------------------------------------------------------
with system_i select
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

                     
---------------------------------------------------------------
-- hb1s_o dependent on system :
---------------------------------------------------------------
with system_i select
   hb1s_o       <=   conv_std_logic_vector(82-delay_adjust,12)    when X"A0",   -- 1080i/30
                     conv_std_logic_vector(82-delay_adjust,12)    when X"B0",   -- 1080i/29.97
                     conv_std_logic_vector(82-delay_adjust,12)    when X"82",   -- 1080p/30
                     conv_std_logic_vector(82-delay_adjust,12)    when X"92",   -- 1080p/29.97
                     conv_std_logic_vector(522-delay_adjust,12)   when X"83",   -- 1080p/25
                     conv_std_logic_vector(632-delay_adjust,12)   when X"84",   -- 1080p/24
                     conv_std_logic_vector(632-delay_adjust,12)   when X"94",   -- 1080p/23.98
                     conv_std_logic_vector(82-delay_adjust,12)    when X"A2",   -- 1080sF/30
                     conv_std_logic_vector(82-delay_adjust,12)    when X"B2",   -- 1080sF/29.97
                     conv_std_logic_vector(522-delay_adjust,12)   when X"A3",   -- 1080sF/25
                     conv_std_logic_vector(632-delay_adjust,12)   when X"A4",   -- 1080sF/24
                     conv_std_logic_vector(632-delay_adjust,12)   when X"B4",   -- 1080sF/23.98
                     conv_std_logic_vector(104-delay_adjust,12)   when X"86",   -- 720p/60
                     conv_std_logic_vector(104-delay_adjust,12)   when X"96",   -- 720p/59.94
                     conv_std_logic_vector(434-delay_adjust,12)   when X"87",   -- 720p/50
                     conv_std_logic_vector(1754-delay_adjust,12)  when X"88",   -- 720p/30
                     conv_std_logic_vector(1754-delay_adjust,12)  when X"98",   -- 720p/29.97
                     conv_std_logic_vector(2414-delay_adjust,12)  when X"89",   -- 720p/25
                     conv_std_logic_vector(2579-delay_adjust,12)  when X"8A",   -- 720p/24
                     conv_std_logic_vector(2579-delay_adjust,12)  when X"9A",   -- 720p/23.98
                     conv_std_logic_vector(14-delay_adjust,12)    when X"60",   -- NTSC 525i/29.97
                     conv_std_logic_vector(10-delay_adjust,12)    when X"20",   -- PAL 625i/25
                     conv_std_logic_vector(522-delay_adjust,12)   when others;  -- 1080i/25  X"A1"


---------------------------------------------------------------
-- interlaced_transport dependent on system :
---------------------------------------------------------------
with system_i select
   interlaced_transport <= '1' when X"A0",   -- 1080i/30
                           '1' when X"B0",   -- 1080i/29.97
                           '0' when X"82",   -- 1080p/30
                           '0' when X"92",   -- 1080p/29.97
                           '0' when X"83",   -- 1080p/25
                           '0' when X"84",   -- 1080p/24
                           '0' when X"94",   -- 1080p/23.98
                           '1' when X"A2",   -- 1080sF/30
                           '1' when X"B2",   -- 1080sF/29.97
                           '1' when X"A3",   -- 1080sF/25
                           '1' when X"A4",   -- 1080sF/24
                           '1' when X"B4",   -- 1080sF/23.98
                           '0' when X"86",   -- 720p/60
                           '0' when X"96",   -- 720p/59.94
                           '0' when X"87",   -- 720p/50
                           '0' when X"88",   -- 720p/30
                           '0' when X"98",   -- 720p/29.97
                           '0' when X"89",   -- 720p/25
                           '0' when X"8A",   -- 720p/24
                           '0' when X"9A",   -- 720p/23.98
                           '1' when X"60",   -- NTSC 525i/29.97
                           '1' when X"20",   -- PAL 625i/25
                           '1' when others;  -- 1080i/25

interlaced_transport_o <= interlaced_transport;

---------------------------------------------------------------
-- Parameters dependent on lpf_index :
---------------------------------------------------------------
with lpf_index select
   lpf_o                   <= conv_std_logic_vector(525,11)    when "000",    -- 487
                              conv_std_logic_vector(625,11)    when "001",    -- 576
                              conv_std_logic_vector(750,11)    when "010",    -- 720
                              conv_std_logic_vector(1125,11)   when others;   -- 1080

with lpf_index select
   line_start_o            <= conv_std_logic_vector(4,11)      when "000",    -- 487
                              conv_std_logic_vector(1,11)      when "001",    -- 576
                              conv_std_logic_vector(1,11)      when "010",    -- 720
                              conv_std_logic_vector(1,11)      when others;   -- 1080

with lpf_index select
   f_start_o               <= '1'                              when "000",    -- 487
                              '0'                              when "001",    -- 576
                              '0'                              when "010",    -- 720
                              '0'                              when others;   -- 1080

with lpf_index select
   hb2s_o                  <= conv_std_logic_vector(120+delay_adjust,12) when "000",    -- 487
                              conv_std_logic_vector(130+delay_adjust,12) when "001",    -- 576
                              conv_std_logic_vector(256+delay_adjust,12) when "010",    -- 720
                              conv_std_logic_vector(188+delay_adjust,12) when others;   -- 1080

lpf_interlaced_transport <= lpf_index & interlaced_transport;

with lpf_interlaced_transport select
   field1_end_o            <= conv_std_logic_vector(265,11)    when "0001",   -- 487 interlaced_transport
                              conv_std_logic_vector(312,11)    when "0011",   -- 576 interlaced_transport
                              conv_std_logic_vector(563,11)    when "1001",   -- 1080 interlaced_transport
                              conv_std_logic_vector(0,11)      when others;   -- progressive_transport

with lpf_interlaced_transport select
   field2_end_o            <= conv_std_logic_vector(3,11)      when "0001",   -- 487 interlaced_transport
                              conv_std_logic_vector(625,11)    when "0011",   -- 576 interlaced_transport
                              conv_std_logic_vector(1125,11)   when "1001",   -- 1080 interlaced_transport
                              conv_std_logic_vector(0,11)      when others;   -- progressive_transport

with lpf_interlaced_transport select
   field1_vblnk_end_o      <= conv_std_logic_vector(19,11)     when "0001",   -- 487 interlaced_transport
                              conv_std_logic_vector(22,11)     when "0011",   -- 576 interlaced_transport
                              conv_std_logic_vector(20,11)     when "1001",   -- 1080 interlaced_transport
                              conv_std_logic_vector(41,11)     when "1000",   -- 1080 progressive_transport
                              conv_std_logic_vector(25,11)     when others;   -- 720 progressive_transport

with lpf_interlaced_transport select
   field1_av_end_o         <= conv_std_logic_vector(263,11)    when "0001",   -- 487 interlaced_transport
                              conv_std_logic_vector(310,11)    when "0011",   -- 576 interlaced_transport
                              conv_std_logic_vector(560,11)    when "1001",   -- 1080 interlaced_transport
                              conv_std_logic_vector(1121,11)   when "1000",   -- 1080 progressive_transport
                              conv_std_logic_vector(745,11)    when others;   -- 720 progressive_transport

with lpf_interlaced_transport select
   field2_vblnk_end_o      <= conv_std_logic_vector(282,11)    when "0001",   -- 487 interlaced_transport
                              conv_std_logic_vector(335,11)    when "0011",   -- 576 interlaced_transport
                              conv_std_logic_vector(583,11)    when "1001",   -- 1080 interlaced_transport
                              conv_std_logic_vector(0,11)      when others;   -- progressive_transport

with lpf_interlaced_transport select
   field2_av_end_o         <= conv_std_logic_vector(525,11)    when "0001",   -- 487 interlaced_transport
                              conv_std_logic_vector(623,11)    when "0011",   -- 576 interlaced_transport
                              conv_std_logic_vector(1123,11)   when "1001",   -- 1080 interlaced_transport
                              conv_std_logic_vector(0,11)      when others;   -- progressive_transport


---------------------------------------------------------------
-- Parameters dependent on spl_index :
---------------------------------------------------------------
with spl_index select
   asl_o                   <= conv_std_logic_vector(720,11)    when "00",     -- 720
                              conv_std_logic_vector(1280,11)   when "01",     -- 1280
                              conv_std_logic_vector(1920,11)   when others;   -- 1920

with spl_index select
   hd_zsd                  <= '0'                              when "00",     -- 720
                              '1'                              when "01",     -- 1280
                              '1'                              when others;   -- 1920
hd_zsd_o <= hd_zsd;


end Behavioral;