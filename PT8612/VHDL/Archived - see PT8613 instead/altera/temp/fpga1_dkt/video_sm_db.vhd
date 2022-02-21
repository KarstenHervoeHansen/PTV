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
        video_en :      in std_logic;
        scan_format :   in std_logic;
        pattern :       in std_logic_vector(1 downto 0);
        sav :           in std_logic;
        line_clk :      in std_logic;
        fvh :           in std_logic_vector(2 downto 0);
        y :             out std_logic_vector(9 downto 0);
        cb :            out std_logic_vector(9 downto 0);
        cr :            out std_logic_vector(9 downto 0);
        -- debug
        hdformat :      in std_logic_vector(3 downto 0);                       
        horz :          out integer;
        vert :          out integer;
        activeline :    out integer;
        loadval :       out integer;
        cnt :           out integer
        );
end video_sm;

architecture Behavioral of video_sm is

component state_counter is
    Port (
        clk :       in std_logic;
        load :      in std_logic;
        load_val :  in std_logic_vector(10 downto 0);
        count :     inout std_logic_vector(10 downto 0)
    );
end component;

component color_lut is
    Port (
        clk :       in std_logic;
        v_state :   in std_logic_vector(1 downto 0);
        h_state :   in std_logic_vector(4 downto 0);
        yramp_en :  in std_logic;
        y :         out std_logic_vector(9 downto 0);
        cb :        out std_logic_vector(9 downto 0);
        cr :        out std_logic_vector(9 downto 0)
    );
end component;

type horz_statetype is ( h0, h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11 );
signal h_state :     horz_statetype;
type vert_statetype is ( v0, v1, v2, v3 );
signal v_state :     vert_statetype;
signal hstate :         std_logic_vector(4 downto 0);
signal vstate :         std_logic_vector(1 downto 0);
signal yramp_en :       std_logic;
signal field :          std_logic;
signal vblank :         std_logic;
signal hblank :         std_logic;
signal active_line :    std_logic_vector(10 downto 0);
signal statetimer :     std_logic_vector(10 downto 0);
signal load_val :       std_logic_vector(10 downto 0);
signal load :           std_logic;
signal timeout :        std_logic_vector(10 downto 0) := (others => '0');
signal black :          std_logic;
signal rp219 :          std_logic;
signal ebucb :          std_logic;
signal smptecb :        std_logic;
signal dbformat :       std_logic;

begin

-- debug : ----------------------------------
cnt <= conv_integer(statetimer);
loadval <= conv_integer(load_val);
horz <= conv_integer(hstate);
vert <= conv_integer(vstate);
activeline <= conv_integer(active_line);
---------------------------------------------

with pattern select
    rp219 <= '1' when "01",
             '0' when others;

with pattern select
    dbformat <= '1' when "00",
                '0' when others;

counter : state_counter port map(
    clk         => clk,
    load        => load,
    load_val    => load_val,
    count       => statetimer
    );

color : color_lut port map(
    clk         => clk,
    v_state     => vstate,
    h_state     => hstate,
    yramp_en    => yramp_en,
    y           => y,
    cb          => cb,
    cr          => cr
    );

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

video_vertical_state : process( line_clk )
begin
    if line_clk'event and line_clk = '0' then
        if scan_format = '1' then   -- interlaced format
            if hdformat = "1111" then
                -- debug format
                case active_line is
                    when "00000000000" =>
                        v_state <= v0;
                    when "00000010100" => -- 20
                        v_state <= v1;
                    when "00000011001" => -- 25
                        if rp219 = '1' then
                            v_state <= v2;
                        end if;
                    when "00000011110" => -- 30
                        v_state <= v3;
                    when others =>
                        null;
                end case;
            elsif hdformat = "0000" then
                v_state <= v0;
            else
                case active_line is
                    -- 1080i
                    when "00000000000" =>
                        v_state <= v0;
                    when "00100111011" => -- 315  
                        v_state <= v1;
                    when "00101101000" => -- 360
                        v_state <= v2;
                    when "00110010101" => -- 405
                        v_state <= v3;
                    when others =>
                        null;
                end case;
            end if;
        else                        -- progressive format
            case active_line is
                when "00000000000" =>
                    v_state <= v0;
                when "01001110110" => -- 630
                    v_state <= v1;
                when "01011010000" => -- 720
                    v_state <= v2;
                when "01100101010" => -- 810
                    v_state <= v3;
                when others =>
                    null;
            end case;
        end if;
    end if;
end process;


video_horizontal_state : process( clk, video_en )
begin
    if video_en = '0' then
        h_state <= h0;
    elsif clk'event and clk = '1' then
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
                    if dbformat = '1' then
                        h_state <= h9;
                    else
                        h_state <= h2;
                    end if;
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
                    if rp219 = '1' or dbformat = '1' then
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
end process;


h_state_widths : process( clk, video_en )
begin
    if video_en = '0' then
        load_val <= conv_std_logic_vector(241,11);
    elsif clk'event and clk = '1' then
        case h_state is
            when h0 =>
                if rp219 = '1' then
                    load_val <= conv_std_logic_vector(241,11);
                else
                    if dbformat = '1' then
                        load_val <= conv_std_logic_vector(960,11);
                    else
                        load_val <= conv_std_logic_vector(276,11);
                    end if;
                end if;
            when h1 =>
                if v_state = v3 then
                    load_val <= conv_std_logic_vector(307,11);
                else
                    if dbformat = '1' then
                        load_val <= conv_std_logic_vector(960,11);
                    else
                        load_val <= conv_std_logic_vector(205,11);
                    end if;
                end if;
            when h2 =>
                if rp219 = '1' then
                    if v_state = v3 then
                        load_val <= conv_std_logic_vector(411,11);
                    else
                        load_val <= conv_std_logic_vector(205,11);
                    end if;
                else
                    load_val <= conv_std_logic_vector(273,11);
                end if;
            when h3 =>
                if rp219 = '1' then
                    if v_state = v3 then
                        load_val <= conv_std_logic_vector(169,11);
                    end if;
                end if;
            when h4 =>
                if rp219 = '1' then
                    if v_state = v3 then
                        load_val <= conv_std_logic_vector(67,11);
                    else
                        load_val <= conv_std_logic_vector(203,11);
                    end if;
                end if;
            when h5 =>
                if rp219 = '1' then
                    if v_state = v3 then
                        load_val <= conv_std_logic_vector(69,11);
                    else
                        load_val <= conv_std_logic_vector(205,11);
                    end if;
                end if;
            when h6 =>
                if rp219 = '1' then
                    if v_state = v3 then
                        load_val <= conv_std_logic_vector(67,11);
                    else
                        load_val <= conv_std_logic_vector(205,11);
                    end if;
                end if;
            when h7 =>
                if rp219 = '1' then
                    if v_state = v3 then
                        load_val <= conv_std_logic_vector(69,11);
                    else
                        load_val <= conv_std_logic_vector(205,11);
                    end if;
                else
                    load_val <= conv_std_logic_vector(274,11);
                end if;
            when h8 =>
                if rp219 = '1' then 
                    load_val <= conv_std_logic_vector(241,11);
                else
                    load_val <= conv_std_logic_vector(276,11);
                end if;
            when h10 =>
                load_val <= conv_std_logic_vector(67,11);
            when h11 =>
                load_val <= conv_std_logic_vector(205,11);
            when others =>
                null;
        end case;
    end if;
end process;


reset_state_counter : process( video_en, clk )
-- counteren loades første gang ved SAV
-- derefter igen ved timeout
begin
    if video_en = '0' then 
        load <= '0';
    elsif clk'event and clk = '0' then
        if sav = '1' then
            load <= '1';
        else
            load <= '0';
        end if;
    end if;
end process;

with h_state select
    hstate <= "00000" when h0,
        "00001" when h1,
        "00010" when h2,
        "00011" when h3,
        "00100" when h4,
        "00101" when h5,
        "00110" when h6,
        "00111" when h7,
        "01000" when h8,
        "01001" when h9,
        "01010" when h10,
        "01011" when h11,
        "11111" when others;

with v_state select
    vstate <= "00" when v0,
        "01" when v1,
        "10" when v2,
        "11" when others;

end Behavioral;