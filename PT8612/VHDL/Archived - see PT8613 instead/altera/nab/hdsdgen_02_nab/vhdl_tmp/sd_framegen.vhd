library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity sd_framegen is
    Port ( 
        reset_n : in std_logic;
        clk : in std_logic;
        pal_nntsc : in std_logic;
        txsrc : in std_logic_vector(2 downto 0);
        txrate : in std_logic_vector(1 downto 0);
        video_out : out std_logic_vector(9 downto 0)
        );
end sd_framegen;

architecture Behavioral of sd_framegen is

component sd_framegenerator
    Port (
        clk :           in std_logic;
        mute :          in std_logic;
        sdformat :      in std_logic;
        tsl :           in integer;
        asl :           in integer;
        lpf :           in integer;
        vblank :        in integer;
        field2_start :  in integer;
        luma_sample :   in std_logic_vector(9 downto 0);
        chroma_sample : in std_logic_vector(9 downto 0);
	    video :         out std_logic_vector(9 downto 0);
	    line :          out std_logic_vector(9 downto 0);
	    sample :        out std_logic_vector(11 downto 0)
	);
end component;

component edh
    port (
        clk, enable :               in std_logic;
        data_in :                   in std_logic_vector(9 downto 0);
        linecount :                 in std_logic_vector(9 downto 0);
        wordcount :                 in std_logic_vector(11 downto 0);
        video_type :                in std_logic_vector(2 downto 0);
        read_acrc_errors :          inout std_logic;
        read_fcrc_errors :          inout std_logic;
        acrc_errors, fcrc_errors :  inout std_logic_vector( 7 downto 0);
        data_out :                  out std_logic_vector(9 downto 0)
    );
end component;


signal samplecnt, eg1_wordcount, rp178_wordcount, grey_wordcount : std_logic_vector(11 downto 0);
signal edh_data, data_eg1, data_grey, data_rp178, data, edata : std_logic_vector(9 downto 0);
signal luma_sample, chroma_sample, testcount, testdata: std_logic_vector(9 downto 0);
signal linecnt, eg1_linecount, rp178_linecount, grey_linecount : std_logic_vector(9 downto 0);
signal sel : std_logic_vector(2 downto 0);
signal enable_sdsdigen, enable_grey, enable_rp178, debug : std_logic;
signal tsl, asl, lpf, vblank, field2_start : integer;


begin

luma_sample <= conv_std_logic_vector(64,10);
chroma_sample <= conv_std_logic_vector(512,10);

debug <= '0';
process
	begin
		wait until clk = '1';
            if debug = '1' then
		        video_out <= data_eg1;
            else
		        video_out <= edh_data;
            end if;
	end process;

	enable_sdsdigen <= '1' when ( reset_n = '1' and txsrc = "000"  and txrate <= 2 ) or ( enable_grey = '1' ) or ( enable_rp178 = '1' ) else '0';
	enable_grey <= '1' when reset_n = '1' and txsrc = "001"  and txrate <= 1 else '0';
	enable_rp178 <= '1' when reset_n = '1' and txsrc(2 downto 1) = "01"  and txrate <= 1 else '0';

	
	sel(0) <= txrate(0);
	sel(1) <= pal_nntsc;

	sel(2) <= '1' when txrate = 2 else '0';

sdi_frame : sd_framegenerator
    port map ( 
        clk => clk, 
        mute => enable_sdsdigen,
        sdformat => sel(1),
        tsl => tsl,
        asl => asl,
        lpf => lpf,
        vblank => vblank,
        field2_start => field2_start,
        luma_sample => luma_sample,
        chroma_sample => chroma_sample,
        video => data_eg1,
        line => linecnt,
        sample => samplecnt
    );

error_detection_and_handling : edh
    port map (
        clk => clk, 
        enable => reset_n,
        data_in	=> data_eg1,	
        linecount => linecnt,	
        wordcount => samplecnt, 
        read_acrc_errors => open,
        read_fcrc_errors => open,
        video_type => sel,
        data_out => edh_data
    );


with sel select -- TOTAL SAMPLES PER LINE
	tsl	<=	1716	when	"000",	--259C NTSC
			2288	when	"001",	--259D NTSC
			1728	when	"010",	--259C PAL
			2304	when	"011",	--259D PAL
			3432	when	"100",	--344M NTSC 4:4:4:4
			3456	when    "110",  --344M PAL 4:4:4:4
    		0		when	others;
with sel select -- ACTIVE SAMPLES PER LINE
	asl	<=	1440	when	"000",
			1920	when	"001",
			1440	when	"010",
			1920	when	"011",
			2880	when	"100",
			2880	when	"110",
			0		when	others;
with sel select -- TOTAL LINES
	lpf	<=  525	when	"000",
			525	when	"001",
			625	when	"010",
			625	when	"011",
			525 when	"100",
			625 when	"110",
			0	when    others;
with sel select -- VERTICAL BALNKING INTERVAL PER FIELD
	vblank <=   19	when	"000",
				19	when	"001",
				22	when	"010",
				22	when	"011",
				19 	when	"100",
				22 	when	"110",
				0 	when 	others;
with sel select  -- LINES IN FIRST FIELD  (LINES/2+1)
	field2_start <= 263	when	"000",
			        263	when	"001",
			        313	when	"010",
			        313	when	"011",
			        263 when	"100",
			        313 when	"110",
			        0 	when 	others;
	
end Behavioral;