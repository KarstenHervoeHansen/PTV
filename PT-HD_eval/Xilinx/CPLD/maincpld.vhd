library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity cpld_debug is
    Port ( 
		  -- GF9320	
-- 		  sif_out : in std_logic;
  --        sif_in : out std_logic;
    --      sif_ck : out std_logic;
      --    sif_rst : out std_logic;

          -- GS9023A
		  amsd : out std_logic_vector(2 downto 0);
          ancisd : out std_logic;
          mutesd : out std_logic;
          vmsd : out std_logic_vector(2 downto 0);
          trs : out std_logic;
		  loc9023 : in std_logic;
          edh_ins : out std_logic;
          d_m_sd : out std_logic;

          -- GS1503
		  crc_error : in std_logic;
          error : in std_logic;
          operate : in std_logic;
          vid_det : in std_logic;
          vmhd : out std_logic_vector(3 downto 0);
          amhd : out std_logic_vector(1 downto 0);
          mutehd : out std_logic;
          scrbyp : out std_logic;
          ancihd : out std_logic;
          dscbyp : out std_logic;
          m_d_hd : out std_logic;

          --parallel bus
		  --cpuadr : in std_logic_vector(4 downto 0);
          --cpudat : inout std_logic_vector(7 downto 0);
		  --cpuwe : in std_logic;
      --    cpure : in std_logic;

          -- debug :
          cpudat : inout std_logic_vector(1 downto 0);

          -- GS1560
          iop1560 :     out std_logic;
          fwen_dis :    out std_logic;
          sd_hd_1560 :  inout std_logic;
          rcbyp :       inout std_logic;
          m_s_1560 :    out std_logic;
          d_a1560 :     inout std_logic;
          smptebp1560 : inout std_logic;
          busw_1560 :   out std_logic;
          cs_1560 :     out std_logic;
		  dataerr :     in std_logic;
          loc1560 :     in std_logic;
--          yanc :        in std_logic;
  --        canc :        in std_logic;

          -- GS1532
--          loc1532 : in std_logic;
          busw_1532 : out std_logic;
          sd_hd_1532 : out std_logic;
          blank : out std_logic;
          smptebp1532 : out std_logic;
          iop1532 : out std_logic;
          d_a1532 : out std_logic;
          det_trs : out std_logic;
          cs_1532 : out std_logic;

          -- SPI bus for GS1560, GS1532, FPGA, MSD600
--		  sdin : out std_logic;
  --        sdout : in std_logic;
    --      sck : out std_logic;

          -- FPGA
		  cs_f : out std_logic;
		  tp_cf : inout std_logic;

          -- SPI bus between CPU and CPLD
--		  mosi : in std_logic;
  --        miso : out std_logic;
    --      cpu_sck : in std_logic;

          -- msd600
--		  cs_msd : out std_logic;
          clk_s : out std_logic;

          -- CPU
--		  cs_cpld : out std_logic;

           -- ICS660
--		  s0 : out std_logic;

		  -- PLL1700
		  rstpll : out std_logic

           );
end cpld_debug;


architecture behavioral of cpld_debug is


signal hddmb_error :        std_logic;
signal vid_det_sig :        std_logic;
signal operate_sig :        std_logic;
signal ms1560 :             std_logic;
signal da1560_sig :         std_logic;
signal sd_hd_1560_sig :     std_logic;
signal rcbyp_sig :          std_logic;
signal smptebp1560_sig :    std_logic;


begin


----------------------------------------------------------------
-- setup parameters for GS1560 :
----------------------------------------------------------------
m_s_1560 <= '0';      -- ( master mode = '1' , slave mode = '0' )

-- DVBASI, SD/HD, RCbyp and SMPTEbyp is configured as outputs in master mode
-- da1560_sig <= d_a1560;
-- sd_hd_1560_sig <= sd_hd_1560;
-- rcbyp_sig <= rcbyp;
-- smptebp1560_sig <= smptebp1560;
    
-- DVBASI, SD/HD, RCbyp and SMPTEbyp is configured as inputs in slave mode
d_a1560 <= '0';     --'0' (DVB/ASI pin) no decodong or word alignment of recieved DVB/ASI data if LOW
sd_hd_1560 <= '0';  --'0' (SD/HD pin)( standard def = 1 , high def = 0 )
rcbyp <= '1';       --'0' reclocker bypass - active LOW
smptebp1560 <= '1'; --'z' in peters code
                    --'1' (SMPTEbyp pin) carry out descrambling and word alignment if high ( DVB/ASI must be low )

iop1560 <= '0';     -- '1' in peters code -- IO processing enable - active HIGH
fwen_dis <= '1';    -- Flywheel disable - active LOW
busw_1560 <= '0';   -- '1' in peters code -- ( 10 bit buswidth = 0 , 20 bit buswidth = 1 )
cs_1560 <= 'Z';     -- chip select - active LOW


------------------------------------------------------------------------------------
-- setup GS1503 :
-- ( HDmodul : vmhd=0000, amhd=10, mutehd=0, scrbyp=0, ancihd=0, dscbyp=0, m_d_hd=1 )
-----------------
hddmb_error <= crc_error and error;
operate_sig <= operate;
vid_det_sig <= vid_det;
vmhd <= "0010";     -- 0010=1080P/30 - 0100=1080P/25 - 0000=720P7/60&59.94
amhd <= "10";       -- 10 = AES3 audio mode
mutehd <= '0';      -- mutes audio when HIGH
scrbyp <= '0';      -- scrambler bypass - active high
ancihd <= '0';      -- all ancillary data packets stay in stream when LOW
dscbyp <= '1';      -- descrambler bypass - active HIGH
m_d_hd <= '1';      -- MUX mode = 0 , DEMUX mode = 1


----------------------------------------------------------------------
-- setup GS1532 :
-----------------
--busw_1532 <= '1';  -- 20 bit = 1 / 10 bit = 0
--sd_hd_1532 <= '1';  -- SD=1 / HD=0
--blank <= '1';       -- LUMA/CHROMA data passes unaltered through device when HIGH
--smptebp1532 <= '1';  -- bypass SMPTE scrambling and encoding when LOW
--d_a1532 <= '0';
--iop1532 <= '0';     -- disable IO processing when LOW
--det_trs <= '0';     -- extract timing from supplied H:V:F signals when LOW
--cs_1532 <= '1';     -- no chip select


----------------------------------------------------------------------
-- setup GS9023 :
-----------------
--amsd <= "100";      -- Audio mode = AES/EBU
--ancisd <= '0';      -- Ancilliary data 
--mutesd <= '0';      -- audio is muted when HIGH
--vmsd <= "101";      -- video mode : ( 101=625 lines ( trs=0 )) ( 001 = 525 lines ( trs=0 ))
--trs <= '0';         -- determines video input format together with vmsd(2:0)
--edh_ins <= '0';     -- not used - valid in multiplex mode only
--d_m_sd <= '1';      -- demultiplex mode when HIGH


----------------------------------------------------------------------
-- setup test pins :
--------------------
cs_f <= loc9023;
tp_cf <= loc1560;


end Behavioral;
