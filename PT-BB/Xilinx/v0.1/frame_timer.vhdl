library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity frame_timer is
    Port(
        mreset :    in std_logic;
        f74 :       in std_logic;
        sample :    inout integer range 0 to 2048;
        line :      inout integer range 0 to 1024
    );
end frame_timer;

architecture Behavioral of frame_timer is

signal cpl :        integer range 0 to 2048 := 1728;    -- f27 cycles pr line in 625 line system
signal lpf :        integer range 0 to 1024 := 625;     -- lines per frame
signal line_clock : std_logic;

begin

sample_counter : process( mreset, f74 )
begin
    if mreset = '0' then
        sample <= 0;
        line_clock <= '1';
    elsif f74'event and f74 = '1' then
        if sample = cpl then
            sample <= 0;
            line_clock <= '1';
        elsif sample /= 255 then
            sample <= sample + 1;
            line_clock <= '0';
        else
            sample <= sample + 1;
        end if;
    end if;
end process;

line_counter : process( mreset, line_clock )
begin
    if mreset = '0' then
        line <= 0;
    elsif line_clock'event and line_clock = '1' then
        if line /= lpf then
            line <= line + 1;
        else
            line <= 0;
        end if;
    end if;
end process;



end Behavioral;
