library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity color_lut is
    Port (
        clk :       in std_logic;
        v_state :   in std_logic_vector(1 downto 0);
        h_state :   in std_logic_vector(4 downto 0);
        yramp_en :  in std_logic;
        y :         out std_logic_vector(9 downto 0);
        cb :        out std_logic_vector(9 downto 0);
        cr :        out std_logic_vector(9 downto 0)
    );
end color_lut;

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

signal reset :          std_logic;
signal cnt :            integer;


begin

with h_state select
    reset <= '1' when "00000",
             '1' when "00001",
             '0' when others;

process( clk, reset )
begin
    if reset = '1' then
        cnt <= 0;
    elsif clk'event and clk = '0' then
        if cnt /= 6 then
            cnt <= cnt + 1;
        end if;
    end if;
end process;

process( clk, h_state )
begin
    if reset = '1' then
        y <= conv_std_logic_vector(534,10);
        cb <= conv_std_logic_vector(253,10);
        cr <= conv_std_logic_vector(207,10);
    elsif clk'event and clk = '0' then
        case cnt is
            when 0 =>
                y <= conv_std_logic_vector(64,10);
                cb <= conv_std_logic_vector(512,10);
                cr <= conv_std_logic_vector(512,10);
            when 1 =>
                y <= conv_std_logic_vector(507,10);
                cb <= conv_std_logic_vector(302,10);
                cr <= conv_std_logic_vector(265,10);
            when 2 =>
                y <= conv_std_logic_vector(457,10);
                cb <= conv_std_logic_vector(394,10);
                cr <= conv_std_logic_vector(374,10);
            when 3 =>
                y <= conv_std_logic_vector(393,10);
                cb <= conv_std_logic_vector(512,10);
                cr <= conv_std_logic_vector(512,10);
            when 4 =>
                y <= conv_std_logic_vector(328,10);
                cb <= conv_std_logic_vector(630,10);
                cr <= conv_std_logic_vector(650,10);
            when 5 =>
                y <= conv_std_logic_vector(278,10);
                cb <= conv_std_logic_vector(722,10);
                cr <= conv_std_logic_vector(759,10);
            when 6 =>
                y <= conv_std_logic_vector(251,10);
                cb <= conv_std_logic_vector(771,10);
                cr <= conv_std_logic_vector(817,10);
            when others =>
                y <= conv_std_logic_vector(534,10);
                cb <= conv_std_logic_vector(253,10);
                cr <= conv_std_logic_vector(207,10);
        end case;                
    end if;
end process;

end Behavioral;