library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity rp219_colorbar is
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
end rp219_colorbar;

architecture Behavioral of rp219_colorbar is

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

type horz_statetype is ( mute, h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11, 
                        h12, h13, h14, h15, h16, h17, h18, h19, h20, h21 );
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

begin

-- debug : ----------------------------------
cnt <= conv_integer(statetimer);
loadval <= conv_integer(load_val);
horz <= conv_integer(hstate);
vert <= conv_integer(vstate);
activeline <= conv_integer(active_line);
---------------------------------------------

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

--
--      Illustration of RP219 colorbar and 
--      RP219 horizontal and vertical statemachine :    
--
--      +----------+--------+--------+--------+--------+--------+--------+--------+---------+
--      | 40%gr    | 75%wh  | 75%ye  | 75&cy  | 75%gr  | 75%ma  | 75%re  | 75%blu | 40%gr   |
--  V0  |   H1     |   H2   |   H3   |   H4   |   H5   |   H6   |   H7   |   H8   |   H9    |
--      +----------+--------+--------+--------+--------+--------+--------+--------+---------+
--      | 100% cy  | user 1 |                      75%wh                          | 100%blu |
--  V1  |   H1     |   H2   |                       H10                           |   H9    |
--      +----------+--------+-----------------------------------------------------+---------+
--      | 100% ye  | user 2 |                      Yramp                          | 100%re  |
--  V2  |   H1     |   H2   |                       H11                           |   H9    |
--      +----------+--------+-+---------+------+-----+----+----+----+----+--------+---------+
--      | 15%gr    | 0%bl     | 100% wh | 0%bl | -2% | 0% | 2% | 0% | 4% | 0%bl   | 15%gr   |
--  V3  |   H1     |   H12    |   H13   | H14  | H15 | H16| H17| H18| H19|   H8   |   H9    |
--      +----------+----------+---------+------+-----+----+----+----+----+--------+---------+
--


rp219_vertical_state : process( line_clk )
begin
    if line_clk'event and line_clk = '0' then
        if scan_format = '1' then   -- interlaced format
            if hdformat = "11" then
                -- debug format
                case active_line is
                    when "00000000000" =>
                        v_state <= v0;
                    when "00000010100" => -- 20
                        v_state <= v1;
                    when "00000011001" => -- 25
                        v_state <= v2;
                    when "00000011110" => -- 30
                        v_state <= v3;
                    when others =>
                        null;
                end case;
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


rp219_horizontal_state : process( clk, rp219_en )
begin
    if rp219_en = '0' then
        h_state <= mute;
        load_val <= "00011110001"; -- 241
    elsif clk'event and clk = '1' then
        case h_state is
            when mute =>
                if sav = '1' then
                    h_state <= h1;   
                    yramp_en <= '0';
                end if;
            when h1 =>  -- ( 40% grey, 100% cyan, 100% yellow or 15% grey )
                if statetimer = timeout then
                    if v_state = v3 then
                        h_state <= h12;
                    else
                        h_state <= h2;
                    end if;
                end if;

                if v_state = v3 then
                    load_val <= "00100110011"; -- 307
                else
                    load_val <= "00011001101"; -- 205
                end if;
            when h2 =>  -- ( 75% white, 110% white, +I, -I, 0% black or +Q )
                if statetimer = timeout then
                    if v_state = v0 then
                        h_state <= h3;
                    elsif v_state = v1 then
                        h_state <= h10;
                    else
                        h_state <= h11;
                        yramp_en <= '1';
                    end if;
                end if;
                if v_state = v0 then
                    load_val <= "00011001101"; -- 205
                else
                    load_val <= "10011010001"; -- 1233
                end if;
            when h3 =>  -- ( 75% yellow )
                if statetimer = timeout then
                    h_state <= h4;   
                end if;
            when h4 =>  -- ( 75% cyan )
                if statetimer = timeout then
                    h_state <= h5;
                end if;
                load_val <= "00011001011"; -- 203
            when h5 =>  -- ( 75% green )
                if statetimer = timeout then
                    h_state <= h6;
                end if;
                load_val <= "00011001101"; -- 205
            when h6 =>  -- ( 75% magenta )
                if statetimer = timeout then
                    h_state <= h7;
                end if;
            when h7 =>  -- ( 75% red )
                if statetimer = timeout then
                    h_state <= h8;
                end if;
            when h8 =>  -- ( 75% blue )
                if statetimer = timeout then
                    h_state <= h9;
                end if;
                load_val <= "00011110001"; -- 241
            when h9 =>  -- ( 40% grey, 100% blue, 100% red or 15% grey )
                if sav = '1' then
                    h_state <= h1;
                end if;
            when h10 => -- ( 75% white )
                if statetimer = timeout then
                    h_state <= h9;
                end if;
                load_val <= "00011110001"; -- 241
            when h11 => -- ( Y-ramp )
                if statetimer = timeout then
                    h_state <= h9;
                    yramp_en <= '0';
                end if;
                load_val <= "00011110001"; -- 241
            when h12 => -- ( 0% black )
                if statetimer = timeout then
                    h_state <= h13;
                end if;
                load_val <= "00110011011"; -- 411;
            when h13 => -- ( 100% white )
                if statetimer = timeout then
                    h_state <= h14;
                end if;
                load_val <= "00010101001"; -- 169;
            when h14 => -- ( 0% black )
                if statetimer = timeout then
                    h_state <= h15;
                end if;
                load_val <= "00001000011"; -- 67;
            when h15 => -- ( -2% black )
                if statetimer = timeout then
                    h_state <= h16;
                end if;
                load_val <= "00001000101"; -- 69;
            when h16 => -- ( 0% black )
                if statetimer = timeout then
                    h_state <= h17;
                end if;
                load_val <= "00001000011"; -- 67;
            when h17 => -- ( +2% black )
                if statetimer = timeout then
                    h_state <= h18;
                end if;
                load_val <= "00001000101"; -- 69;
            when h18 => -- ( 0% black )
                if statetimer = timeout then
                    h_state <= h19;
                end if;
                load_val <= "00001000011"; -- 67;
            when h19 => -- ( +4% black )
                if statetimer = timeout then
                    h_state <= h8;
                end if;
                load_val <= "00011001101"; -- 205;
            when others =>
                h_state <= mute;
        end case;
    end if;
end process;

reset_state_counter : process( rp219_en, clk )
-- counteren loades første gang ved SAV
-- derefter igen ved timeout
begin
    if rp219_en = '0' then 
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
    hstate <= "00000" when mute,
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
        "01100" when h12,
        "01101" when h13,
        "01110" when h14,
        "01111" when h15,
        "10000" when h16,
        "10001" when h17,
        "10010" when h18,
        "10011" when h19,
        "10101" when others;

with v_state select
    vstate <= "00" when v0,
        "01" when v1,
        "10" when v2,
        "11" when others;

end Behavioral;