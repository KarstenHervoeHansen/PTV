architecture Behavioral of color_lut is

signal h1_y :           std_logic_vector(9 downto 0) := (others => '0');
signal h1_cb :          std_logic_vector(9 downto 0) := (others => '0');
signal h1_cr :          std_logic_vector(9 downto 0) := (others => '0');
signal h2_option :      std_logic_vector(4 downto 0) := (others => '0');
signal h2_pattern2 :    std_logic_vector(1 downto 0) := (others => '0');
signal h2_pattern3 :    std_logic := '0';
signal h2_y :           std_logic_vector(9 downto 0) := (others => '0');
signal h2_cb :          std_logic_vector(9 downto 0) := (others => '0'); 
signal h2_cr :          std_logic_vector(9 downto 0) := (others => '0'); 
signal h8_y :           std_logic_vector(9 downto 0) := (others => '0');
signal h8_cb :          std_logic_vector(9 downto 0) := (others => '0');
signal h8_cr :          std_logic_vector(9 downto 0) := (others => '0');
signal h9_y :           std_logic_vector(9 downto 0) := (others => '0');
signal h9_cb :          std_logic_vector(9 downto 0) := (others => '0');
signal h9_cr :          std_logic_vector(9 downto 0) := (others => '0');
signal yramp :          std_logic_vector(9 downto 0) := "0001000000";


begin


with v_state select
    h1_y <= conv_std_logic_vector(414,10) when 0,    -- 40% grey
            conv_std_logic_vector(754,10) when 1,    -- 100% cyan
            conv_std_logic_vector(877,10) when 2,    -- 100% yellow
            conv_std_logic_vector(195,10) when others;  -- 15% grey
with v_state select
    h1_cb <= conv_std_logic_vector(512,10) when 0,
             conv_std_logic_vector(615,10) when 1,
             conv_std_logic_vector(64,10) when 2,
             conv_std_logic_vector(512,10) when others;
with v_state select
    h1_cr <= conv_std_logic_vector(512,10) when 0,
             conv_std_logic_vector(64,10) when 1,
             conv_std_logic_vector(553,10) when 2,
             conv_std_logic_vector(512,10) when others;

h2_option <= conv_std_logic_vector(v_state,2) & h2_pattern2 & h2_pattern3;
-- h2_pattern2 : "00"=75%white, "01"=100%white, "10"=+I, "11"=-I
-- h2_pattern3 : '0'=0%black, '1'=+Q
with h2_option select
    h2_y <= conv_std_logic_vector(940,10) when "01010", -- 100% white
            conv_std_logic_vector(940,10) when "01011", -- 100% white
            conv_std_logic_vector(245,10) when "01100", -- +I
            conv_std_logic_vector(245,10) when "01101", -- +I
            conv_std_logic_vector(244,10) when "01110", -- -I
            conv_std_logic_vector(244,10) when "01111", -- -I
            conv_std_logic_vector(64,10) when "10000",  -- 0% black
            conv_std_logic_vector(141,10) when "10001", -- +Q
            conv_std_logic_vector(64,10) when "10010",  -- 0% black
            conv_std_logic_vector(141,10) when "10011", -- +Q
            conv_std_logic_vector(64,10) when "10100",  -- 0% black
            conv_std_logic_vector(141,10) when "10101", -- +Q
            conv_std_logic_vector(64,10) when "10110",  -- 0% black
            conv_std_logic_vector(141,10) when "10111", -- +Q
            conv_std_logic_vector(721,10) when others;  -- 75%white
-- h2_option <= ( v_state & h2_pattern2 & h2_pattern3 );
-- h2_pattern2 : "00"=75%white, "01"=100%white, "10"=+I, "11"=-I
-- h2_pattern3 : '0'=0%black, '1'=+Q
with h2_option select
    h2_cb <= conv_std_logic_vector(412,10) when "01100", -- +I
             conv_std_logic_vector(412,10) when "01101", -- +I
             conv_std_logic_vector(612,10) when "01110", -- -I
             conv_std_logic_vector(612,10) when "01111", -- -I
             conv_std_logic_vector(697,10) when "10001", -- +Q
             conv_std_logic_vector(697,10) when "10011", -- +Q
             conv_std_logic_vector(697,10) when "10101", -- +Q
             conv_std_logic_vector(697,10) when "10111", -- +Q
             conv_std_logic_vector(512,10) when others;  -- grey, black or white
-- h2_option <= ( v_state & h2_pattern2 & h2_pattern3 );
-- h2_pattern2 : "00"=75%white, "01"=100%white, "10"=+I, "11"=-I
-- h2_pattern3 : '0'=0%black, '1'=+Q
with h2_option select
    h2_cr <= conv_std_logic_vector(629,10) when "01100", -- +I
             conv_std_logic_vector(629,10) when "01101", -- +I
             conv_std_logic_vector(395,10) when "01110", -- -I
             conv_std_logic_vector(395,10) when "01111", -- -I
             conv_std_logic_vector(606,10) when "10001", -- +Q
             conv_std_logic_vector(606,10) when "10011", -- +Q
             conv_std_logic_vector(606,10) when "10101", -- +Q
             conv_std_logic_vector(606,10) when "10111", -- +Q
             conv_std_logic_vector(512,10) when others;  -- grey, black or white

with v_state select
    h8_y <= conv_std_logic_vector(111,10) when 0,       -- 75% blue
            conv_std_logic_vector(64,10) when others;   -- 0% black
with v_state select
    h8_cb <= conv_std_logic_vector(848,10) when 0,      -- 75% blue
             conv_std_logic_vector(512,10) when others; -- 0% black
with v_state select
    h8_cr <= conv_std_logic_vector(481,10) when 0,      -- 75% blue
             conv_std_logic_vector(512,10) when others; -- 0% black

with v_state select
    h9_y <= conv_std_logic_vector(414,10) when 0,       -- 40% grey
            conv_std_logic_vector(127,10) when 1,       -- 100% blue
            conv_std_logic_vector(250,10) when 2,       -- 100% red
            conv_std_logic_vector(195,10) when others;  -- 15% grey
with v_state select
    h9_cb <= conv_std_logic_vector(960,10) when 1,      -- 100% blue
             conv_std_logic_vector(409,10) when 2,      -- 100% red
             conv_std_logic_vector(512,10) when others; -- grey or black
with v_state select
    h9_cr <= conv_std_logic_vector(471,10) when 1,      -- 100% blue
             conv_std_logic_vector(960,10) when 2,      -- 100% red
             conv_std_logic_vector(512,10) when others; -- grey or black

with h_state select
    y <= conv_std_logic_vector(64,10) when 0,               -- 0% black
         h1_y when 1,                                 -- 40% grey, 100% Cyan, 100% Yellow or 15% grey
         h2_y when 2,                                 -- 75% white, 100% white, +I, -I, 0% black or +Q
         conv_std_logic_vector(674,10) when 3,        -- 75% yellow
         conv_std_logic_vector(581,10) when 4,        -- 75% cyan
         conv_std_logic_vector(534,10) when 5,        -- 75% green
         conv_std_logic_vector(251,10) when 6,        -- 75% magenta
         conv_std_logic_vector(204,10) when 7,        -- 75% red
         h8_y when 8,                                 -- 75% blue or 0% black
         h9_y when 9,                                 -- 40% grey, 100% blue, 100% red or 15% grey
         conv_std_logic_vector(721,10) when 10,       -- 75% white
         yramp when 11,                               -- linear luma-ramp
         conv_std_logic_vector(64,10) when 12,        -- 0% black
         conv_std_logic_vector(940,10) when 13,       -- 100% white
         conv_std_logic_vector(64,10) when 14,        --  0% black
         conv_std_logic_vector(46,10) when 15,        -- -2% black
         conv_std_logic_vector(64,10) when 16,        -- 0% black
         conv_std_logic_vector(82,10) when 17,        -- +2% black
         conv_std_logic_vector(64,10) when 18,        -- 0% black
         conv_std_logic_vector(99,10) when 19,          -- +4% black
         conv_std_logic_vector(414,10) when others;   -- 15% grey

with h_state select
    cb <= h1_cb when 1,                               -- 40% grey, 100% Cyan, 100% Yellow or 15% grey
         h2_cb when 2,                                -- 75% white, 100% white, +I, -I, 0% black or +Q
         conv_std_logic_vector(176,10) when 3,        -- 75% yellow
         conv_std_logic_vector(589,10) when 4,        -- 75% cyan
         conv_std_logic_vector(253,10) when 5,        -- 75% green
         conv_std_logic_vector(771,10) when 6,        -- 75% magenta
         conv_std_logic_vector(435,10) when 7,        -- 75% red
         h8_cb when 8,                                -- 75% blue
         h9_cb when 9,                                -- 40% grey, 100% blue, 100% red or 15% grey
         conv_std_logic_vector(512,10) when others;   -- 75% white, luma-ramp, -2% black, 0% black, +4% black, 100% white or 15% grey

with h_state select
    cr <= h1_cr when 1,                               -- 40% grey, 100% Cyan, 100% Yellow or 15% grey
         h2_cr when 2,                                -- 75% white, 100% white, +I, -I, 0% black or +Q
         conv_std_logic_vector(543,10) when 3,        -- 75% yellow
         conv_std_logic_vector(176,10) when 4,        -- 75% cyan
         conv_std_logic_vector(207,10) when 5,        -- 75% green
         conv_std_logic_vector(817,10) when 6,        -- 75% magenta
         conv_std_logic_vector(848,10) when 7,        -- 75% red
         h8_cr when 8,                                -- 75% blue
         h9_cr when 9,                                -- 40% grey, 100% blue, 100% red or 15% grey
         conv_std_logic_vector(512,10) when others;   -- 75% white, luma-ramp, -2% black, 0% black, +4% black, 100% white or 15% grey

make_yramp_for_RP219_TP : process( yramp_en, clk )
begin
    if yramp_en = '0' then
        yramp <= conv_std_logic_vector(64,10);
    elsif clk'event and clk = '1' then
        if yramp /= conv_std_logic_vector(940,10) then
            yramp <= yramp + 1;
        else
            null;
        end if;
    end if;        
end process;

end Behavioral;