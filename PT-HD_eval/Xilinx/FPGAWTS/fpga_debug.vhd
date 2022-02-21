library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity fpga_debug is
    Port (
        -- gs9023a
--        aina : out std_logic;
--        ainb : out std_logic;
        aouta : in std_logic;
        aoutb : in std_logic;
        pclksd : out std_logic;
--        visd : out std_logic_vector(9 downto 0);
--        vosd : in std_logic_vector(9 downto 0);
--        auxen : out std_logic;
        buferr : in std_logic;
        
        -- gs1503
        vihd : out std_logic_vector(19 downto 0);
        --vohd : in std_logic_vector(19 downto 0);
        ao12 : in std_logic;
        ao34 : in std_logic;
        ao56 : in std_logic;
        ao78 : in std_logic;
        --ain12 : out std_logic;
        --ain34 : out std_logic;
        --ain56 : out std_logic;
        --ain78 : out std_logic;
        pclkhd : out std_logic;
        crc_err : in std_logic;
        exth : inout std_logic;
        extf : inout std_logic;
        vid_det : in std_logic;
        error : in std_logic;
        operate : in std_logic;


        -- gs1560
        video :     in std_logic_vector(19 downto 0);
        pclk1560 :  in std_logic;
        h1560 :     in std_logic;
        v1560 :     in std_logic;
        f1560 :     in std_logic;
--        fifold :    in std_logic;
        lock1560 :   in std_logic;
--        dataerr :   in std_logic;


        -- gs1532
        din : out std_logic_vector(19 downto 0);
        pclk1532 : out std_logic;  -- kortsluttet paa print pclk1560
        h1532 : out std_logic;
        v1532 : out std_logic;
        f1532 : out std_logic;
        lock1532 : in std_logic;


        -- adsp-bf533 ez-kit
        --a : out std_logic_vector(18 downto 0);
        --d : inout std_logic_vector(15 downto 0);
        --abe_so0 : in std_logic;
        --abe_so1 : in std_logic;
        --br : out std_logic;
        --bg : in std_logic;
        --bgh : in std_logic;
        --ams : in std_logic_vector(3 downto 0);
        --ardy : out std_logic;
        --aoe : in std_logic;
        --awe : in std_logic;
        --are : in std_logic;
        --tmr1 : inout std_logic;
        --tmr2 : inout std_logic;
        --pp : inout std_logic_vector(3 downto 0);
        --pf : inout std_logic_vector(15 downto 0);
        --rsclk : inout std_logic;
        --rfs1 : inout std_logic;
        --dr1pri : out std_logic;
        --dr1sec : out std_logic;
        --tsclk : inout std_logic;
        --tfs1 : inout std_logic;
        --dt1pri : in std_logic;
        --dt1sec : in std_logic;
        --d_miso : inout std_logic;
        --d_mosi : inout std_logic;
        --d_sck : inout std_logic;
        --dsp_clk : out std_logic;
        --e_dsp_clk : in std_logic;
        --d_reset : out std_logic;
        
         
        -- lcd display
        --d0p : out std_logic;
        --d0m : out std_logic;
        --d1p : out std_logic;
        --d1m : out std_logic;
        --d2p : out std_logic;
        --d2m : out std_logic;
        --d3p_gnd : out std_logic;
        --d3m_gnd : out std_logic;
        --ckp : out std_logic;
        --ckm : out std_logic;

        -- spi interface
        cs_f : in std_logic;
        --sdin : in std_logic;
        --sdout : in std_logic;
        --sclk : in std_logic;

        -- ics660
        --pclko : in std_logic;

        -- pll1700
        --a_pll  : out std_logic;

        -- msd600
        --green3 : in std_logic;
        --green4 : in std_logic;
        --green5 : in std_logic;
        --green6 : in std_logic;
        --blue3 : in std_logic;
        --blue4 : in std_logic;
        --blue5 : in std_logic;
        --blue6 : in std_logic;
        --tp : inout std_logic_vector(4 downto 0);
        
        -- test points
        f_tp1 : out std_logic;
        f_tp2 : out std_logic;
        f_tp3 : in std_logic;

        -- mainframe
        --bb5 : out std_logic;
        --bb6 : out std_logic;
        --fhm : in std_logic;
        --f4m : in std_logic;
        --fhg : in std_logic;
        --f8g : in std_logic;


        -- j7 connector :
        pin5 : inout std_logic;
        pin6 : inout std_logic;
        pin7 : inout std_logic;
        pin8 : inout std_logic;
        pin9 : inout std_logic;
        pin10 : inout std_logic;
        pin11 : inout std_logic;
        pin12 : inout std_logic;
        pin13 : inout std_logic;
        pin14 : inout std_logic;
        pin15 : inout std_logic;
        pin16 : inout std_logic;
        pin17 : inout std_logic;
        pin18 : inout std_logic;
        pin19 : inout std_logic;
        pin20 : inout std_logic;
        pin21 : inout std_logic;
        pin22 : inout std_logic;
        pin23 : inout std_logic;
        pin24 : inout std_logic;
        pin25 : inout std_logic;
        pin26 : inout std_logic;
        pin27 : inout std_logic;
        pin28 : inout std_logic;
        pin29 : inout std_logic;
        pin30 : inout std_logic;
        --pin31 : inout std_logic;
        --pin32 : inout std_logic;
        --pin33 : inout std_logic;
        --pin34 : inout std_logic;
        --pin35 : inout std_logic;
        --pin36 : inout std_logic;
        --pin37 : inout std_logic;
        --pin38 : inout std_logic;
        --pin39 : inout std_logic;
        --pin40 : inout std_logic;
        --pin41 : inout std_logic;
        --pin42 : inout std_logic;
        --pin43 : inout std_logic;
        --pin44 : inout std_logic;
        --pin45 : inout std_logic;
        --pin46 : inout std_logic;
        --pin47 : inout std_logic;
        --pin48 : inout std_logic;
        --pin49 : inout std_logic;
        --pin50 : inout std_logic;
        --pin55 : inout std_logic;
        --pin56 : inout std_logic;
        --pin57 : inout std_logic;
        --pin58 : inout std_logic;
        --pin59 : inout std_logic;
        --pin60 : inout std_logic;
        --pin61 : inout std_logic;
        --pin62 : inout std_logic;
        --pin63 : inout std_logic;
        --pin64 : inout std_logic;
        --pin65 : inout std_logic;
        --pin66 : inout std_logic;
        --pin67 : inout std_logic;
        --pin68 : inout std_logic;
        --pin69 : inout std_logic;
        --pin70 : inout std_logic;
        --pin71 : inout std_logic;
        --pin72 : inout std_logic;
        --pin73 : inout std_logic;
        pin74 : inout std_logic
        --pin75 : inout std_logic;
        --pin76 : inout std_logic;
        --pin77 : inout std_logic;
        --pin78 : inout std_logic;
        --pin79 : inout std_logic;
        --pin80 : inout std_logic;

        -- 
--        tp_cf :     in std_logic
        --ppi_clk :   in std_logic
    );
end fpga_debug;

architecture Behavioral of fpga_debug is


--signal pclk : std_logic;
--attribute clock_signal : string;
--attribute clock_signal of pclk : signal is "yes";


begin


-----------------------------------------------------------------
-- signals to/from SDI reciever GS1560 :
-----------------------------------------------------------------
pin8 <= lock1560;


-----------------------------------------------------------------
-- signals to/from serializer GS1532 :
-----------------------------------------------------------------
video_mux : process( pin74, pclk1560 )
begin
    if pin74 = '1' then
        -- din = parallel video input for serializer
        din(19 downto 1) <= video(19 downto 1); 
        din(0) <= f_tp3;
    elsif pclk1560'event and pclk1560 = '1' then
        din(19 downto 0) <= "00000000000000000000";
    end if;        
end process;

h1532 <= h1560; -- horisontal blanking
v1532 <= v1560; -- vertikal blanking
f1532 <= f1560; -- field blanking

setup_pclk : pclk1532 <= pclk1560;

-----------------------------------------------------------------
-- signals to/from HD dembedder GS1503 :
-----------------------------------------------------------------
setup_video_bus : vihd <= video;
setup_pclk_forGS1503 : pclkhd <= pclk1560;

pin10 <= ao12;      -- audio channels
pin11 <= ao34;
pin12 <= ao56;
pin13 <= ao78;

pin14 <= crc_err;   -- crc error - HIGH if an error is detected
pin15 <= vid_det;   -- valid video detected when HIGH
pin16 <= error;     -- Format error indicator - HIGH if a TRS error is detected
pin17 <= operate;   -- Audio data is being demultiplexed when HIGH
pin18 <= exth;      -- H sync derived from incoming video
pin19 <= extf;      -- F sync derived from incoming video

        
-----------------------------------------------------------------
-- signals to/from SD deembedder GS9023A :
-----------------------------------------------------------------
setup_pclk_for_sddeembedder : pclksd <= pclk1560;
-- debug aes output :
pin5 <= aouta;
pin6 <= aoutb;
--pin7 <= buferr;


-----------------------------------------------------------------
-- setup test_pins :
-----------------------------------------------------------------
--f_tp2 <= cs_f;
--f_tp1 <= pclk1560;

pin27 <= cs_f;
pin28 <= aouta;
pin29 <= aoutb;
pin30 <= buferr;

end Behavioral;
