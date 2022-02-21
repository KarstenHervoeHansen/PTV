library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity hd_framegen is
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
end hd_framegen;

architecture Behavioral of hd_framegen is

component hd_framegenerator is
	port ( 
		clk :           in std_logic;
        mute :          in std_logic;                       -- mutes generator when 0
        tsl :           in std_logic_vector(11 downto 0);   -- total samples per line
        asl :           in std_logic_vector(11 downto 0);   -- active samples per line
        lpf :           in std_logic_vector(10 downto 0);   -- lines per frame
        field1_vblnk0 : in std_logic_vector(10 downto 0);   -- vertical blanking borders
        field2_vblnk1 : in std_logic_vector(10 downto 0);
        field2_vblnk0 : in std_logic_vector(10 downto 0);
        field1_vblnk1 : in std_logic_vector(10 downto 0);
        field2_start :  in std_logic_vector(9 downto 0);
        sav :           out std_logic;
        line_clk :      inout std_logic;
        sample :        inout integer;    -- output to videogenerator
        line :          inout integer;    -- output to videogenerator
        fvh_out :       out std_logic_vector(2 downto 0); -- blanking bits
        y :             in std_logic_vector(9 downto 0);     -- input from videogenerator
        cb :            in std_logic_vector(9 downto 0);     -- input from videogenerator
        cr :            in std_logic_vector(9 downto 0);         
		luma :          out std_logic_vector(9 downto 0);   -- output to scrambler
		chroma :        out std_logic_vector(9 downto 0);   -- output to scrambler
        -- debug :
        tp :            out std_logic
	);
end component;

signal tsl :            std_logic_vector(11 downto 0);
signal asl :            std_logic_vector(11 downto 0);
signal lpf :            std_logic_vector(10 downto 0);
signal field1_vblnk0 :  std_logic_vector(10 downto 0);
signal field2_vblnk1 :  std_logic_vector(10 downto 0);
signal field2_vblnk0 :  std_logic_vector(10 downto 0);
signal field1_vblnk1 :  std_logic_vector(10 downto 0);
signal field2_start :   std_logic_vector(9 downto 0);
signal sample :         integer;
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
        y               => act_y,
        cb              => act_cb,
        cr              => act_cr,
		luma            => luma,
		chroma          => chroma,
        tp              => tp
        );

select_format_parameters : process( hdformat )
begin
    case hdformat is
        when "0000" =>
            -- 1080i/30 and 1080i/29.97
            scan_format <= '1';
            tsl <= conv_std_logic_vector(2200,12);
            asl <= conv_std_logic_vector(1920,12);
            lpf <= conv_std_logic_vector(1125,11);
            field1_vblnk0 <= conv_std_logic_vector(21,11);
            field2_vblnk1 <= conv_std_logic_vector(561,11);
            field2_vblnk0 <= conv_std_logic_vector(584,11);
            field1_vblnk1 <= conv_std_logic_vector(1124,11);
            field2_start <= conv_std_logic_vector(563,10);
        when "0001" =>
            -- 1080i/25
            scan_format <= '1';
            tsl <= conv_std_logic_vector(2640,12);
            asl <= conv_std_logic_vector(1920,12);
            lpf <= conv_std_logic_vector(1125,11);
            field1_vblnk0 <= conv_std_logic_vector(21,11);
            field2_vblnk1 <= conv_std_logic_vector(561,11);
            field2_vblnk0 <= conv_std_logic_vector(584,11);
            field1_vblnk1 <= conv_std_logic_vector(1124,11);
            field2_start <= conv_std_logic_vector(563,10);
        when "0010" =>
            -- 1080p/30
            scan_format <= '0';
            tsl <= conv_std_logic_vector(2200,12);
            asl <= conv_std_logic_vector(1920,12);
            lpf <= conv_std_logic_vector(1125,11);
            field1_vblnk0 <= conv_std_logic_vector(42,11);
            field2_vblnk1 <= (others => '0');
            field2_vblnk0 <= (others => '0');
            field1_vblnk1 <= conv_std_logic_vector(1122,11);
            field2_start <= (others => '0');
        when "0011" =>
            -- 1080p/25
            scan_format <= '0';
            tsl <= conv_std_logic_vector(2640,12);
            asl <= conv_std_logic_vector(1920,12);
            lpf <= conv_std_logic_vector(1125,11);
            field1_vblnk0 <= conv_std_logic_vector(42,11);
            field2_vblnk1 <= (others => '0');
            field2_vblnk0 <= (others => '0');
            field1_vblnk1 <= conv_std_logic_vector(1122,11);
            field2_start <= (others => '0');
        when "0100" =>
            -- 1080p/24 and 1080p/23.98
            scan_format <= '0';
            tsl <= conv_std_logic_vector(2750,12);
            asl <= conv_std_logic_vector(1920,12);
            lpf <= conv_std_logic_vector(1125,11);
            field1_vblnk0 <= conv_std_logic_vector(42,11);
            field2_vblnk1 <= (others => '0');
            field2_vblnk0 <= (others => '0');
            field1_vblnk1 <= conv_std_logic_vector(1122,11);
            field2_start <= (others => '0');
        when "0101" =>
            -- 1035i/60 and 1035i/59.94
            scan_format <= '1';
            tsl <= conv_std_logic_vector(2200,12);
            asl <= conv_std_logic_vector(1920,12);
            lpf <= conv_std_logic_vector(1125,11);
            field1_vblnk0 <= conv_std_logic_vector(41,11);
            field2_vblnk1 <= conv_std_logic_vector(558,11);
            field2_vblnk0 <= conv_std_logic_vector(603,11);
            field1_vblnk1 <= conv_std_logic_vector(1121,11);
            field2_start <= conv_std_logic_vector(563,10);
        when "0110" =>
            -- 720p/60 and 720p/59.94
            scan_format <= '0';
            tsl <= conv_std_logic_vector(1650,12);
            asl <= conv_std_logic_vector(1280,12);
            lpf <= conv_std_logic_vector(750,11);
            field1_vblnk0 <= conv_std_logic_vector(26,11);
            field2_vblnk1 <= (others => '0');
            field2_vblnk0 <= (others => '0');
            field1_vblnk1 <= conv_std_logic_vector(746,11);
            field2_start <= (others => '0');
        when "0111" =>
            -- 720p/50
            scan_format <= '0';
            tsl <= conv_std_logic_vector(1980,12);
            asl <= conv_std_logic_vector(1280,12);
            lpf <= conv_std_logic_vector(750,11);
            field1_vblnk0 <= conv_std_logic_vector(26,11);
            field2_vblnk1 <= (others => '0');
            field2_vblnk0 <= (others => '0');
            field1_vblnk1 <= conv_std_logic_vector(746,11);
            field2_start <= (others => '0');
        when "1000" =>
            -- 720p/30 and 720p/29.97
            scan_format <= '0';
            tsl <= conv_std_logic_vector(3300,12);
            asl <= conv_std_logic_vector(1280,12);
            lpf <= conv_std_logic_vector(750,11);
            field1_vblnk0 <= conv_std_logic_vector(26,11);
            field2_vblnk1 <= (others => '0');
            field2_vblnk0 <= (others => '0');
            field1_vblnk1 <= conv_std_logic_vector(746,11);
            field2_start <= (others => '0');
        when "1001" =>
            -- 720p/25
            scan_format <= '0';
            tsl <= conv_std_logic_vector(3960,12);
            asl <= conv_std_logic_vector(1280,12);
            lpf <= conv_std_logic_vector(750,11);
            field1_vblnk0 <= conv_std_logic_vector(26,11);
            field2_vblnk1 <= (others => '0');
            field2_vblnk0 <= (others => '0');
            field1_vblnk1 <= conv_std_logic_vector(746,11);
            field2_start <= (others => '0');
        when "1010" =>
            -- 720p/24 and 720p/23.98
            scan_format <= '0';
            tsl <= conv_std_logic_vector(4125,12);
            asl <= conv_std_logic_vector(1280,12);
            lpf <= conv_std_logic_vector(750,11);
            field1_vblnk0 <= conv_std_logic_vector(26,11);
            field2_vblnk1 <= (others => '0');
            field2_vblnk0 <= (others => '0');
            field1_vblnk1 <= conv_std_logic_vector(746,11);
            field2_start <= (others => '0');
        when others =>
            -- debug format
            scan_format <= '1';
            tsl <= conv_std_logic_vector(2200,12);
            asl <= conv_std_logic_vector(1920,12);
            lpf <= conv_std_logic_vector(750,11);
            field1_vblnk0 <= conv_std_logic_vector(21,11);
            field2_vblnk1 <= conv_std_logic_vector(61,11);
            field2_vblnk0 <= conv_std_logic_vector(84,11);
            field1_vblnk1 <= conv_std_logic_vector(124,11);
            field2_start <= conv_std_logic_vector(64,10);
    end case;
end process;            

end Behavioral;
