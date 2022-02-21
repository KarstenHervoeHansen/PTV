-- ****************************************************************************
-- Copyright (c) 2005 DK-Technologies. All Rights Reserved
-- Proprietary Information - Internal Data
-- ----------------------------------------------------------------------------
-- File Name: Testbench.vhd
-- 19 october 2005
--
-- Author(s): Thomas Holm Hansen
--
-- Description: TestBench for black_frame.vhd
-------------------------------------------------------------------------------

LIBRARY ieee;
USE     ieee.std_logic_1164.all;
USE     ieee.std_logic_arith.all;
USE     ieee.std_logic_signed.all;

LIBRARY std;
USE   std.standard.ALL;
USE   std.textio.ALL;


-- Library Synopsys;
-- Use Synopsys.hdl_xmr_pkg.all;
library modelsim_lib;
use modelsim_lib.util.all;


LIBRARY work;
-- USE     work.sim_pkg.all;
-- USE     work.test.all;

entity Testbench is
end Testbench;

architecture behavioral of Testbench is


signal zreset                 : std_logic := '0';
signal Clk                    : std_logic := '1';
signal en                     : std_logic := '1';
signal sync                   : std_logic;
signal sync_reclk             : std_logic;
signal sync_60                : std_logic;


signal hd_zsd                 : std_logic;
signal interlaced             : std_logic;
signal asl                    : std_logic_vector(11 downto 0);
signal lpf                    : std_logic_vector(10 downto 0);
signal hb1s                   : std_logic_vector(11 downto 0);
signal hb2s                   : std_logic_vector(11 downto 0);
signal field1_end             : std_logic_vector(10 downto 0);
signal field2_end             : std_logic_vector(10 downto 0);
signal f_start                : std_logic;
signal field1_vblnk_end       : std_logic_vector(10 downto 0);
signal field1_av_end          : std_logic_vector(10 downto 0);
signal field2_vblnk_end       : std_logic_vector(10 downto 0);
signal field2_av_end          : std_logic_vector(10 downto 0);


constant sync60_count1           : integer := 1375;        -- used to generate 60Hz sync
constant sync60_count2           : integer := 1800;        -- 1375*1800
constant sync60_bits1            : integer := 12;          -- size of counter 1
constant sync60_bits2            : integer := 12;          -- size of counter 2


component system_parameters
   port (
      zreset_i             : in std_logic;
      clk_i                : in std_logic;
      system_i             : in std_logic_vector(7 downto 0);

      hd_zsd_o             : out std_logic;                       -- 1: sd, 0: hd
      interlaced_o         : out std_logic;                       -- 1: interlaced, 0: progressive
		asl_o                : out std_logic_vector(11 downto 0);   -- active samples per line
      lpf_o                : out std_logic_vector(10 downto 0);   -- lines per frame
      hb1s_o               : out std_logic_vector(11 downto 0);   -- horizontal blanking 1 length (samples), asl+10+h_blank1+h_blank2 =tsl
      hb2s_o               : out std_logic_vector(11 downto 0);   -- horizontal blanking 2 length (samples)
      field1_end_o         : out std_logic_vector(10 downto 0);
      field2_end_o         : out std_logic_vector(10 downto 0);
      f_start_o            : out std_logic;
      field1_vblnk_end_o   : out std_logic_vector(10 downto 0);   -- line numbers for vertical blanking borders
      field1_av_end_o      : out std_logic_vector(10 downto 0);
      field2_vblnk_end_o   : out std_logic_vector(10 downto 0);
      field2_av_end_o      : out std_logic_vector(10 downto 0)
   );
end component;


component period_dual_count
   generic(
      count_val1  : integer;
      count_val2  : integer;
      bits1       : integer;
      bits2       : integer
   );
   Port(
      mreset_i    : in std_logic;
      clk_i       : in std_logic;
      sync_reset_i: in std_logic;
      sync_o      : out std_logic
   );
end component;


component black_frame
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      frame_start_i        : in std_logic;

      -- system parameters:
      hd_zsd_i             : in std_logic;                        -- 0: sd, 1: hd
      asl_i                : in std_logic_vector(11 downto 0);    -- active samples per line
      lpf_i                : in std_logic_vector(10 downto 0);    -- lines per frame
      hb1s_i               : in std_logic_vector(11 downto 0);    -- horizontal blanking 1 length (including CRC)
      hb2s_i               : in std_logic_vector(11 downto 0);    -- horizontal blanking 2 length

      field1_end_i         : in std_logic_vector(10 downto 0);
      field2_end_i         : in std_logic_vector(10 downto 0);
      f_start_i            : in std_logic;

      field1_vblnk_end_i   : in std_logic_vector(10 downto 0);    -- line numbers for vertical blanking borders
      field1_av_end_i      : in std_logic_vector(10 downto 0);
      field2_vblnk_end_i   : in std_logic_vector(10 downto 0);
      field2_av_end_i      : in std_logic_vector(10 downto 0);

      -- frame control signals:
      line_o               : out std_logic_vector(10 downto 0);
      fvh_o                : out std_logic_vector(2 downto 0);    -- blanking bits
      calc_crc_o           : out std_logic;
      cb_zcr_o             : out std_logic;

      -- parallel data outputs:
      luma_o               : out std_logic_vector(9 downto 0);
      chroma_o             : out std_logic_vector(9 downto 0);

      -- other output signals:
      sync_reset_o         : out std_logic;                       -- syncronious reset output to oversampler:
      timing_change_o      : out std_logic
   );
end component;


begin


system_parameter_lookup : system_parameters
port map (
   zreset_i                => zreset,
   clk_i                   => Clk,
   system_i                => "10100000",

   hd_zsd_o                => hd_zsd,
   interlaced_o            => interlaced,
   asl_o                   => asl,
   lpf_o                   => lpf,
   hb1s_o                  => hb1s,
   hb2s_o                  => hb2s,

   field1_end_o            => field1_end,
   field2_end_o            => field2_end,
   f_start_o               => f_start,

   field1_vblnk_end_o      => field1_vblnk_end,
   field1_av_end_o         => field1_av_end,
   field2_vblnk_end_o      => field2_vblnk_end,
   field2_av_end_o         => field2_av_end
);


sync60_generation : period_dual_count
generic map (
   count_val1 => sync60_count1,
   count_val2 => sync60_count2,
   bits1      => sync60_bits1,
   bits2      => sync60_bits2
)
port map (
   mreset_i     => zreset,
   clk_i        => Clk,
   sync_reset_i => sync_reclk,
   sync_o       => sync_60
);


black_frame_generator : black_frame
 	port map (
		zreset_i             => zreset,
      frame_start_i        => sync_reclk,
      clk_i                => Clk,
      en_i                 => en,

      hd_zsd_i             => hd_zsd,
      asl_i                => asl,
      lpf_i                => lpf,
      hb1s_i               => hb1s,
      hb2s_i               => hb2s,
      field1_end_i         => field1_end,
      field2_end_i         => field2_end,
      f_start_i            => f_start,
      field1_vblnk_end_i   => field1_vblnk_end,
      field1_av_end_i      => field1_av_end,
      field2_vblnk_end_i   => field2_vblnk_end,
      field2_av_end_i      => field2_av_end
   );


Clk <= not Clk after 5 ps;
zreset <= '1' after 50 ps;
sync <= '0', '1' after 130 ps, '0' after 140 ps;


sync_reclocking : process(zreset, Clk)
begin
   if zreset = '0' then
      sync_reclk <= '0';
   elsif Clk'event and Clk = '1' then
      sync_reclk <= sync;
   end if;
end process;


en_generation : process(zreset, Clk)
begin
   if zreset = '0' then
      en <= '1';
   elsif Clk'event and Clk = '1' then
      if sync_reclk = '1' then
         en <= '0';
      else
         en <= not en;
      end if;
   end if;
end process;


end behavioral;