library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity video_sm is
    Port ( 
        clk :           in std_logic;
		  enable :        in std_logic;
        video_en :      in std_logic;
		  sd_hd :	      in std_logic;
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
end video_sm;

architecture Behavioral of video_sm is

component state_counter is
    Port (
        clk :       in std_logic;
		  enable :        in std_logic;
        video_en:   in std_logic;
        sav :       in std_logic;
        load_val :  in std_logic_vector(10 downto 0);
        count :     inout std_logic_vector(10 downto 0)
    );
end component;

component color_lut is
    Port (
        clk :       in std_logic;
		  enable :        in std_logic;
        v_state :   in integer range 0 to 3;
        h_state :   in integer range 0 to 15;
        pattern :   in std_logic_vector(2 downto 0);
        option :    in std_logic_vector(2 downto 0);
        yramp_en :  in std_logic;
        y :         out std_logic_vector(9 downto 0);
        cb :        out std_logic_vector(9 downto 0);
        cr :        out std_logic_vector(9 downto 0)
    );
end component;

type horz_statetype is ( h0, h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11 );
signal h_state :        horz_statetype;
type vert_statetype is ( v0, v1, v2, v3 );
signal v_state :        vert_statetype;
signal hstate :         integer range 0 to 15;
signal vstate :         integer range 0 to 3;
signal yramp_en :       std_logic;
signal field :          std_logic;
signal vblank :         std_logic;
signal hblank :         std_logic;
signal active_line :    std_logic_vector(10 downto 0) := (others => '0');
signal statetimer :     std_logic_vector(10 downto 0);
signal load_val :       std_logic_vector(10 downto 0);
signal timeout :        std_logic_vector(10 downto 0) := (others => '0');
signal smpte240 :       std_logic;  -- high ved 1035 linier HD format
signal smpte274 :       std_logic;  -- high ved 1080 linier HD format
signal smpte296 :       std_logic;  -- high ved 720 linier HD format
signal rp219 :          std_logic;  -- high når pattern = RP219 picture
signal ff_cb :          std_logic;  -- high ved full field color bars ( og black )
signal ebu_cb :         std_logic;  -- high ved EBU colorbar ( full field og split field w. grey )
signal smpte_cb :       std_logic;  -- high ved smpte colorbar pattern 011

begin

counter : state_counter port map(
    clk         => clk,
	 enable      => enable,
    video_en    => video_en,
    sav         => sav,
    load_val    => load_val,
    count       => statetimer
    );

color : color_lut port map(
    clk         => clk,
	 enable      => enable,
    v_state     => vstate,
    h_state     => hstate,
    pattern     => pattern,
    option      => option,
    yramp_en    => yramp_en,
    y           => y,
    cb          => cb,
    cr          => cr
    );

-- debug : ----------------------------------
tp(23) <= smpte240;
tp(22) <= smpte274;
tp(21) <= smpte296;
tp(20) <= ff_cb;
tp(19) <= smpte_cb;
tp(18) <= ebu_cb;
tp(17) <= rp219;
tp(16 downto 6) <= active_line;
tp(5 downto 4) <= conv_std_logic_vector(vstate,2);
tp(3 downto 0) <= conv_std_logic_vector(hstate,4);
loadval <= conv_integer(load_val);
---------------------------------------------

-- hdformat table :
-- 0000		1080i/30 & 1080i/29.97
-- 0001		1080i/25
-- 0010		1080p/30
-- 0011		1080p/25
-- 0100		1080p/24 & 1080p/23.98
-- 0101		1035i/30 & 1035i/29.97
-- 0110		720p/60 & 720p/59.94
-- 0111		720p/50
-- 1000		720p/30 & 720p/29.97
-- 1001		720p/25
-- 1010		720p/24 & 720p/23.98

with hdformat select
    smpte274 <= '1' when "0000",
                '1' when "0001",
                '1' when "0010",
                '1' when "0011",
                '1' when "0100",
                '0' when others;

with hdformat select
    smpte296 <= '1' when "0110",
                '1' when "0111",
                '1' when "1000",
                '1' when "1001",
                '1' when "1010",
                '0' when others;

smpte240 <= not ( smpte296 or smpte274 );

with pattern select
    rp219 <= '1' when "101",    -- pattern 5 er lig RP219's første vertikale sektion
             '1' when "110",    -- RP219
             '0' when others;

with pattern select
    ff_cb <= '1' when "000",    -- black
             '1' when "001",    -- EBU full field colorbar 75% saturation
             '1' when "010",    -- EBU full field colorbar 100% saturation
             '1' when "101",    -- 75% full field colorbar w. grey bars
             '0' when others;

smpte_cb <= '1' when pattern = "011" else '0';

with pattern select
    ebu_cb <= '1' when "001",   -- full field colorbar 75% saturation
              '1' when "010",   -- full field colorbar 100% saturation
              '1' when "100",   -- split field colorbar with 75% grey
              '0' when others; 

field <= fvh(2);
vblank <= fvh(1);
hblank <= fvh(0);

activeline_counter : process( vblank, hblank )
begin
    if vblank = '1' then
        active_line <= (others => '0');
    elsif hblank'event and hblank = '0' then
        active_line <= active_line + 1;
    end if;
end process;

video_vertical_state : process(ff_cb, line_clk )
begin
    if ff_cb = '1' then
        v_state <= v0;
    elsif line_clk'event and line_clk = '0' then
        if scan_format = '1' then
            -- interlaced format :
            if smpte274 = '1' then
                -- 1080i :
                case active_line is
                    when "00000000000" =>
                        v_state <= v0;
                    when "00000000010" =>       -- debug
                        if debug = '1' then     -- debug
                            v_state <= v1;      -- debug
                        end if;                 -- debug
                    when "00000000100" =>       -- debug
                        if debug = '1' then     -- debug
                            v_state <= v2;      -- debug
                        end if;                 -- debug
                    when "00000000110" =>       -- debug
                        if debug = '1' then     -- debug
                            v_state <= v3;      -- debug
                        end if;                 -- debug
                    when "00100111011" =>   -- linie 315
                        if rp219 = '1' then
                            v_state <= v1;
                        end if;
                    when "00101101000" =>   -- linie 360
                        if rp219 = '1' then
                            v_state <= v2;
                        elsif ( smpte_cb = '1' or ebu_cb = '1' ) then
                            v_state <= v1;
                        end if;
                    when "00110010101" =>   -- linie 405
                        if rp219 = '1' then
                            v_state <= v3;
                        elsif smpte_cb = '1' then
                            v_state <= v2;
                        end if;
                    when others =>
                        null;
                end case;
            else
                -- 1035i :
                case active_line is
                    when "00000000000" =>
                        v_state <= v0;
                    when "00000000010" =>       -- debug
                        if debug = '1' then     -- debug
                            v_state <= v1;      -- debug
                        end if;                 -- debug
                    when "00000000100" =>       -- debug
                        if debug = '1' then     -- debug
                            v_state <= v2;      -- debug
                        end if;                 -- debug
                    when "00000000110" =>       -- debug
                        if debug = '1' then     -- debug
                            v_state <= v3;      -- debug
                        end if;                 -- debug
                    when "00100101110" =>   -- linie 302
                        if rp219 = '1' then
                            v_state <= v1;
                        end if;
                    when "00101011001" =>   -- linie 345
                        if rp219 = '1' then
                            v_state <= v2;
                        elsif ( smpte_cb = '1' or ebu_cb = '1' ) then
                            v_state <= v1;
                        end if;
                    when "00110000100" =>   -- linie 388
                        if rp219 = '1' then
                            v_state <= v3;
                        elsif smpte_cb = '1' then
                            v_state <= v2;
                        end if;
                    when others =>
                        null;
                end case;
            end if;
        else                        
            -- progressive format :
            if smpte274 = '1' then
                -- 1080p :
                case active_line is
                    when "00000000000" =>
                        v_state <= v0;
                    when "00000000010" =>       -- debug
                        if debug = '1' then     -- debug
                            v_state <= v1;      -- debug
                        end if;                 -- debug
                    when "00000000100" =>       -- debug
                        if debug = '1' then     -- debug
                            v_state <= v2;      -- debug
                        end if;                 -- debug
                    when "00000000110" =>       -- debug
                        if debug = '1' then     -- debug
                            v_state <= v3;      -- debug
                        end if;                 -- debug
                    when "01001110110" =>   -- linie 630
                        if rp219 = '1' then
                            v_state <= v1;
                        end if;
                    when "01011010000" =>   -- linie 720
                        if rp219 = '1' then
                            v_state <= v2;
                        elsif ( smpte_cb = '1' or ebu_cb = '1' ) then
                            v_state <= v1;
                        end if;
                    when "01100101010" =>   -- linie 810
                        if rp219 = '1' then
                            v_state <= v3;
                        elsif smpte_cb = '1' then
                            v_state <= v2;
                        end if;
                    when others =>
                        null;
                end case;
            else    
                -- 720p :
                case active_line is
                    when "00000000000" =>
                        v_state <= v0;
                    when "00000000010" =>       -- debug
                        if debug = '1' then     -- debug
                            v_state <= v1;      -- debug
                        end if;                 -- debug
                    when "00000000100" =>       -- debug
                        if debug = '1' then     -- debug
                            v_state <= v2;      -- debug
                        end if;                 -- debug
                    when "00000000110" =>       -- debug
                        if debug = '1' then     -- debug
                            v_state <= v3;      -- debug
                        end if;                 -- debug
                    when "00110100100" =>   -- linie 420
                        if rp219 = '1' then
                            v_state <= v1;
                        end if;
                    when "00111100000" =>   -- linie 480
                        if rp219 = '1' then
                            v_state <= v2;
                        elsif ( smpte_cb = '1' or ebu_cb = '1' ) then
                            v_state <= v1;
                        end if;
                    when "01000011100" =>   -- linie 540
                        if rp219 = '1' then
                            v_state <= v3;
                        elsif smpte_cb = '1' then
                            v_state <= v2;
                        end if;
                    when others =>
                        null;
                end case;
            end if;
        end if;
    end if;
end process;


video_horizontal_state : process( clk, video_en )
begin
    if video_en = '0' then
        h_state <= h0;
    elsif clk'event and clk = '1' then	
	 	if enable = '1' then
        case h_state is
            when h0 =>
                if sav = '1' then
                    if rp219 = '1' then
                        h_state <= h1;   
                        yramp_en <= '0';
                    else
                        h_state <= h2;   
                    end if;
                end if;
            when h1 =>
                if statetimer = timeout then
                    h_state <= h2;
                end if;
            when h2 =>
                if statetimer = timeout then
                    h_state <= h3;
                    if v_state = v2 then
                        yramp_en <= '1';
                    end if;
                end if;
            when h3 =>
                if statetimer = timeout then
                    h_state <= h4;   
                end if;
            when h4 =>
                if statetimer = timeout then
                    h_state <= h5;
                end if;
            when h5 =>
                if statetimer = timeout then
                    h_state <= h6;
                end if;
            when h6 =>
                if statetimer = timeout then
                    h_state <= h7;
                end if;
            when h7 =>
                if statetimer = timeout then
                    if v_state = v3 then
                        h_state <= h10;
                    else
                        h_state <= h8;
                    end if;
                end if;
            when h8 =>
                if statetimer = timeout then
                    h_state <= h9;
                    yramp_en <= '0';
                end if;
            when h9 =>
                if sav = '1' then
                    if rp219 = '1' then
                        h_state <= h1;
                    else
                        h_state <= h2;
                    end if;
                end if;
            when h10 =>
                if statetimer = timeout then
                    h_state <= h11;
                end if;
            when h11 =>
                if statetimer = timeout then
                    h_state <= h8;
                end if;
            when others =>
                h_state <= h0;
        end case;
		end if;
    end if;
end process;


horizontal_state_widths : process( clk, video_en )
begin
    if video_en = '0' then
        load_val <= (others=>'0');
    elsif clk'event and clk = '1' then
	 	if enable = '1' then
        	if smpte296 = '1' then
            -- 1280x720  ( smpte 296m )
            case h_state is
                when h0 =>
                    if smpte_cb = '1' then
                        load_val <= conv_std_logic_vector(184,11);          -- pattern 011
                    else
                        load_val <= conv_std_logic_vector(161,11);          -- pattern 001, 010, 100, 101, 110
                    end if;
                when h1 =>
                    if v_state = v3 then
                        load_val <= conv_std_logic_vector(205,11);          -- pattern 110
                    else
                        load_val <= conv_std_logic_vector(136,11);          -- pattern 101, 110
                    end if;
                when h2 =>
                    if rp219 = '1' then
                        if v_state = v3 then
                            load_val <= conv_std_logic_vector(273,11);      -- pattern 110
                        end if;
                    else
                        if smpte_cb = '1' then
                            if v_state = v2 then
                                load_val <= conv_std_logic_vector(228,11);  -- pattern 011
                            else
                                load_val <= conv_std_logic_vector(182,11);  -- pattern 011
                            end if;
                        else
                            load_val <= conv_std_logic_vector(159,11);      -- pattern 001, 010, 100
                        end if;
                    end if;
                when h3 =>
                    if rp219 = '1' then
                        if v_state = v3 then
                            load_val <= conv_std_logic_vector(113,11);      -- pattern 110
                        end if;
                    elsif smpte_cb = '1' and v_state = v2 then
                        load_val <= conv_std_logic_vector(227,11);          -- pattern 011
                    end if;
                when h4 =>
                    if rp219 = '1' then
                        if v_state = v3 then
                            load_val <= conv_std_logic_vector(45,11);       -- pattern 110
                        else
                            load_val <= conv_std_logic_vector(137,11);      -- pattern 101, 110
                        end if;
                    elsif smpte_cb = '1' and v_state /= v2 then
                        load_val <= conv_std_logic_vector(181,11);          -- pattern 011
                    end if;
                when h5 =>
                    if rp219 = '1' then
                        if v_state /= v3 then
                            load_val <= conv_std_logic_vector(136,11);      -- pattern 110
                        end if;
                    elsif smpte_cb = '1' then
                        if v_state = v2 then
                            load_val <= conv_std_logic_vector(60,11);       -- pattern 011
                        else
                            load_val <= conv_std_logic_vector(182,11);      -- pattern 011
                        end if;
                    end if;
                when h6 =>
                    if rp219 = '1' and v_state = v3 then
                        load_val <= conv_std_logic_vector(44,11);           -- pattern 110
                    end if;
                when h7 =>
                    if rp219 = '1' and v_state = v3 then
                        load_val <= conv_std_logic_vector(45,11);           -- pattern 110
                    end if;
                when h8 =>
                    if smpte_cb = '1' then
                        load_val <= conv_std_logic_vector(182,11);          -- pattern 011
                    else
                        load_val <= conv_std_logic_vector(159,11);          -- pattern 001, 010, 100, 101, 110
                    end if;
                when h9 =>
                    if smpte_cb = '1' then
                        if v_state = v2 then
                            load_val <= conv_std_logic_vector(230,11);      -- pattern 011
                        else
                            load_val <= conv_std_logic_vector(184,11);      -- pattern 011
                        end if;
                    elsif rp219 = '1' then
                        load_val <= conv_std_logic_vector(161,11);          -- pattern 101, 110
                    end if;
                when h11 =>
                    load_val <= conv_std_logic_vector(136,11);
                when others =>
                    null;
            end case;
        else
            -- 1920x1080 og 1920x1035( smpte 274m og 240m )
            case h_state is
                when h0 =>
                    if rp219 = '1' then
                        load_val <= conv_std_logic_vector(241,11);      -- pattern 101, 110
                    else
                        if smpte_cb = '1' then
                            load_val <= conv_std_logic_vector(276,11);  -- pattern 011
                        else
                            load_val <= conv_std_logic_vector(241,11);  -- pattern 001, 010, 100
                        end if;
                    end if;
                when h1 =>
                    if v_state = v3 then
                        load_val <= conv_std_logic_vector(307,11);      -- pattern 110
                    else
                        load_val <= conv_std_logic_vector(205,11);  -- pattern 101, 110
                    end if;
                when h2 =>
                    if rp219 = '1' then
                        if v_state = v3 then
                            load_val <= conv_std_logic_vector(411,11);  -- pattern 110
                        else
                            load_val <= conv_std_logic_vector(205,11);  -- pattern 101, 110
                        end if;
                    else
                        if smpte_cb = '1' then
                            if v_state = v2 then
                                load_val <= conv_std_logic_vector(342,11);  -- pattern 011
                            else
                                load_val <= conv_std_logic_vector(273,11);  -- pattern 011
                            end if;
                        else
                            load_val <= conv_std_logic_vector(239,11);  -- pattern 001, 010, 100
                        end if;
                    end if;
                when h3 =>
                    if rp219 = '1' then
                        if v_state = v3 then
                            load_val <= conv_std_logic_vector(169,11);  -- pattern 110
                        end if;
                    end if;
                when h4 =>
                    if rp219 = '1' then
                        if v_state = v3 then
                            load_val <= conv_std_logic_vector(67,11);   -- pattern 110
                        else
                            load_val <= conv_std_logic_vector(203,11);  -- pattern 101, 110
                        end if;
                    elsif ( smpte_cb = '1' and v_state = v2 ) then
                        load_val <= conv_std_logic_vector(341,11);      -- pattern 011
                    end if;
                when h5 =>
                    if rp219 = '1' then
                        if v_state = v3 then
                            load_val <= conv_std_logic_vector(69,11);   -- pattern 110
                        else
                            load_val <= conv_std_logic_vector(205,11);  -- pattern 101
                        end if;
                    elsif ( smpte_cb = '1' and v_state = v2 ) then
                        load_val <= conv_std_logic_vector(90,11);       -- pattern 011
                    end if;
                when h6 =>
                    if rp219 = '1' then
                        if v_state = v3 then
                            load_val <= conv_std_logic_vector(67,11);   -- pattern 110
                        else
                            load_val <= conv_std_logic_vector(205,11);  -- pattern 101, 110
                        end if;
                    elsif ( smpte_cb = '1' and v_state = v2 ) then
                        load_val <= conv_std_logic_vector(91,11);       -- pattern 011
                    end if;
                when h7 =>
                    if rp219 = '1' then
                        if v_state = v3 then
                            load_val <= conv_std_logic_vector(69,11);   -- pattern 110
                        else
                            load_val <= conv_std_logic_vector(205,11);  -- pattern 101, 110
                        end if;
                    else
                        if smpte_cb = '1' then
                            if v_state = v2 then
                                load_val <= conv_std_logic_vector(90,11);  -- pattern 011
                            else
                                load_val <= conv_std_logic_vector(274,11);  -- pattern 011
                            end if;
                        end if;
                    end if;
                when h8 =>
                    if smpte_cb = '1' then
                        if v_state = v2 then
                            load_val <= conv_std_logic_vector(344,11);  -- pattern 011
                        else
                            load_val <= conv_std_logic_vector(276,11);  -- pattern 011
                        end if;
                    else
                        load_val <= conv_std_logic_vector(241,11);  -- pattern 001, 010, 100, 101, 110
                    end if;
                when h9 =>
                    if ( smpte_cb = '1' and v_state = v2 ) then
                        load_val <= conv_std_logic_vector(344,11);  -- pattern 011
                    end if;
                when h10 =>
                    load_val <= conv_std_logic_vector(67,11);
                when h11 =>
                    load_val <= conv_std_logic_vector(205,11);
                when others =>
                    null;
            end case;
        end if;
		end if;
    end if;
end process;

with h_state select
    hstate <= 0 when h0,
              1 when h1,
              2 when h2,
              3 when h3,
              4 when h4,
              5 when h5,
              6 when h6,
              7 when h7,
              8 when h8,
              9 when h9,
              10 when h10,
              11 when h11,
              12 when others;

with v_state select
    vstate <= 0 when v0,
              1 when v1,
              2 when v2,
              3 when others;

end Behavioral;