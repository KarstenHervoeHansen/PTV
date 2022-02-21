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

signal vblank :         std_logic;
signal hblank :         std_logic;
signal active_line :    std_logic_vector(10 downto 0);

begin

y <= conv_std_logic_vector(64,10);
cb <= conv_std_logic_vector(512,10);
cr <= conv_std_logic_vector(512,10);

horz <= 0;
vert <= 0;
loadval <= 0;
cnt <= 0;

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

activeline <= conv_integer(active_line);

end Behavioral;
