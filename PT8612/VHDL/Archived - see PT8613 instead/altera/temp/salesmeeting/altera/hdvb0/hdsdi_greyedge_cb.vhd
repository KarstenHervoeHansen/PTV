library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity hdsdi_greyedge_cb is
    port (
		clk :       in std_logic;   
        mute :      in std_logic;                       -- mutes generator when 0
        hdformat :  in std_logic_vector(1 downto 0);                       
		luma :      out std_logic_vector(9 downto 0);   -- output to scrambler
		chroma :    out std_logic_vector(9 downto 0)    -- output to scrambler
    );
end hdsdi_greyedge_cb;

architecture Behavioral of hdsdi_greyedge_cb is

component hd_framegenerator is
	port ( 
		clk :           in std_logic;
        mute :          in std_logic;   -- mutes generator when 0
        tsl :           in integer;     -- total samples per line
        asl :           in integer;     -- active samples per line
        lpf :           in integer;     -- lines per frame
        field1_vblnk0 : in integer;     -- 
        field2_vblnk1 : in integer;
        field2_vblnk0 : in integer;
        field1_vblnk1 : in integer;
        field2_start :  in integer;
        sample :        inout integer;    -- output to videogenerator
        sav :           out std_logic;
        line_clk :      inout std_logic;
        line :          inout integer;    -- output to videogenerator
        fvh_out :       inout std_logic_vector(2 downto 0); -- blanking bits
        y :             in std_logic_vector(9 downto 0);     -- input from videogenerator
        cb :            in std_logic_vector(9 downto 0);     -- input from videogenerator
        cr :            in std_logic_vector(9 downto 0);         
		luma :          out std_logic_vector(9 downto 0);   -- output to scrambler
		chroma :        out std_logic_vector(9 downto 0);   -- output to scrambler
        -- debug :
        tp :            out std_logic
	);
end component;


component greyedge_colorbar is
    Port ( 
        clk :           in std_logic;
        rp219_en :      in std_logic;
        scan_format :   in std_logic;
        sav :           in std_logic;
        line_clk :      in std_logic;
        fvh :           in std_logic_vector(2 downto 0);
        y :             out std_logic_vector(9 downto 0);
        cb :            out std_logic_vector(9 downto 0);
        cr :            out std_logic_vector(9 downto 0);
        -- debug
        hdformat :      in std_logic_vector(1 downto 0);                       
        horz :          out integer;
        vert :          out integer;
        activeline :    out integer;
        loadval :       out integer;
        cnt :           out integer
        );
end component;


signal scan_format :    std_logic;
signal tsl :            integer;
signal asl :            integer;
signal lpf :            integer;
signal field1_vblnk0 :  integer;
signal field2_vblnk1 :  integer;
signal field2_vblnk0 :  integer;
signal field1_vblnk1 :  integer;
signal field2_start :   integer;
signal y :              std_logic_vector(9 downto 0);
signal cb :             std_logic_vector(9 downto 0);
signal cr :             std_logic_vector(9 downto 0);
signal rp219_en :       std_logic := '1';
signal sample :         integer;
signal sav :            std_logic;
signal line_clk :       std_logic;
signal fvh :            std_logic_vector(2 downto 0);
-- debugging signals :
signal line :           integer;
signal tp :             std_logic;
signal loadval :        integer;
signal statecnt :       integer;
signal horz :          integer;
signal vert :         integer;
signal activeline :     integer;

begin

hdframe : hd_framegenerator port map(
        clk             => clk,
        mute            => mute,
        tsl             => tsl,
        asl             => asl,
        lpf             => lpf,
        field1_vblnk0   => field1_vblnk0,
        field2_vblnk1   => field2_vblnk1,
        field2_vblnk0   => field2_vblnk0,
        field1_vblnk1   => field1_vblnk1,
        field2_start    => field2_start,
        sample          => sample,
        sav             => sav,
        line_clk        => line_clk,
        line            => line,
        fvh_out         => fvh,
        y               => y,
        cb              => cb,
        cr              => cr,
		luma            => luma,
		chroma          => chroma,
        tp              => tp
        );

colorbar : greyedge_colorbar port map(
        clk         => clk,
        rp219_en    => mute,
        scan_format => scan_format,
        sav         => sav,
        line_clk    => line_clk,
        fvh         => fvh,
        y           => y,
        cb          => cb,
        cr          => cr,
        hdformat    => hdformat,
        horz        => horz,
        vert        => vert,
        activeline  => activeline,
        loadval     => loadval,
        cnt         => statecnt
        );


select_format_parameters : process( hdformat )
begin
    case hdformat is
        when "00" =>
            -- SMPTE 274M   1080i
            scan_format <= '1';
            tsl <= 2200;
            asl <= 1920;
            lpf <= 1125;
            field1_vblnk0 <= 21;
            field2_vblnk1 <= 561;
            field2_vblnk0 <= 584;
            field1_vblnk1 <= 1124;
            field2_start <= 564;
        when "01" =>
            -- SMPTE 260M   1035i
            scan_format <= '1';
            tsl <= 2200;
            asl <= 1920;
            lpf <= 1125;
            field1_vblnk0 <= 41;
            field2_vblnk1 <= 558;
            field2_vblnk0 <= 603;
            field1_vblnk1 <= 1121;
            field2_start <= 564;
        when "10" =>
            -- SMPTE 296M   720p
            scan_format <= '0';
            tsl <= 1650;
            asl <= 1280;
            lpf <= 750;
            field1_vblnk0 <= 26;
            field2_vblnk1 <= 750;   -- progressive format -> no field blanking
            field2_vblnk0 <= 750;   -- progressive format -> no field blanking
            field1_vblnk1 <= 746;
            field2_start <= 750;
        when others =>
            -- debug format
            scan_format <= '1';
            tsl <= 2200;
            asl <= 1920;
            lpf <= 125;
            field1_vblnk0 <= 21;
            field2_vblnk1 <= 61;
            field2_vblnk0 <= 84;
            field1_vblnk1 <= 124;
            field2_start <= 64;
    end case;
end process;            

end Behavioral;
