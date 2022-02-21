library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity toplvl is
    Port(
		clk :       	in std_logic;   
        mute :      	in std_logic;                       -- mutes generator when 0
        hdformat :  	in std_logic_vector(3 downto 0);    -- SMPTE 240M, 274M or 296M
        pattern :       in std_logic_vector(2 downto 0);
        option :        in std_logic_vector(2 downto 0);
        luma :          out std_logic_vector(9 downto 0);
        chroma :        out std_logic_vector(9 downto 0)
        );

end toplvl;

architecture Behavioral of toplvl is

component hd_frame is
    port (
		clk :       	in std_logic;   
        mute :      	in std_logic;                       -- mutes generator when 0
        hdformat :  	in std_logic_vector(3 downto 0);    -- SMPTE 240M, 274M or 296M
  	    act_y :     	in std_logic_vector(9 downto 0);     -- input from videogenerator
        act_cb :    	in std_logic_vector(9 downto 0);     -- input from videogenerator
       	act_cr :    	in std_logic_vector(9 downto 0);         
		scan_format : 	inout std_logic;					-- output for active video modul
		sav :           out std_logic;						-- output for active video modul
		fvh :           out std_logic_vector(2 downto 0);	-- output for active video modul
        line_clk :      inout std_logic;
		luma :      	out std_logic_vector(9 downto 0);   -- output to scrambler
		chroma :    	out std_logic_vector(9 downto 0)    -- output to scrambler
    );
end component;

component video_sm is
    Port ( 
        clk :           in std_logic;
        video_en :      in std_logic;
		sd_hd :	        in std_logic;
        hdformat :      in std_logic_vector(3 downto 0);                       
        scan_format :   in std_logic;
        pattern :       in std_logic_vector(2 downto 0);
        option :        in std_logic_vector(2 downto 0);
        sav :           in std_logic;
        line_clk :      in std_logic;
        fvh :           in std_logic_vector(2 downto 0);
        y :             out std_logic_vector(9 downto 0);
        cb :            out std_logic_vector(9 downto 0);
        cr :            out std_logic_vector(9 downto 0);
        -- debug
        debug :         in std_logic;
        loadval :       out integer;
  		tp :			out std_logic_vector(23 downto 0)
        );
end component;

signal act_y : std_logic_vector(9 downto 0);
signal act_cb : std_logic_vector(9 downto 0);
signal act_cr : std_logic_vector(9 downto 0);
signal scan_format : std_logic;
signal sav : std_logic;
signal fvh : std_logic_vector(2 downto 0);
signal line_clk : std_logic;
signal hstate : std_logic_vector(3 downto 0);
signal vstate : std_logic_vector(1 downto 0);
signal activeline : std_logic_vector(10 downto 0);
signal loadval :    integer;
signal smpte240 :       std_logic;
signal smpte274 :       std_logic;
signal smpte296 :       std_logic;
signal rp219 :          std_logic;
signal ff_cb :          std_logic;
signal ebu_cb :         std_logic;
signal smpte_cb :       std_logic;

constant debug :        std_logic := '1';
constant vcc :          std_logic := '1';
constant gnd :          std_logic := '0';

begin

hdgen : hd_frame port map(
    clk => clk,
    mute => mute,
    hdformat => hdformat,
  	act_y => act_y,
    act_cb => act_cb,
    act_cr => act_cr,
	scan_format => scan_format,
	sav => sav,
	fvh => fvh,
    line_clk => line_clk,
	luma => luma,
	chroma => chroma
    );

active_video : video_sm port map(
    clk => clk,
    video_en => mute,
    sd_hd => vcc,
    hdformat => hdformat,
	scan_format => scan_format,
    pattern => pattern,
    option => option,
    sav => sav,
    line_clk => line_clk,
    fvh => fvh,
    y => act_y,
    cb => act_cb,
    cr => act_cr,
    debug => debug,
    loadval => loadval,
    tp(23) => smpte240,
    tp(22) => smpte274,
    tp(21) => smpte296,
    tp(20) => ff_cb,
    tp(19) => smpte_cb,
    tp(18) => ebu_cb,
    tp(17) => rp219,
    tp(16 downto 6) => activeline,
    tp(5 downto 4) => vstate,
    tp(3 downto 0) => hstate
    );

end Behavioral;
