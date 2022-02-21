library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
library UNISIM;
use UNISIM.VComponents.all;


entity freerunning_reference is
    Port(
        mreset :    in std_logic;
        f1485 :     in std_logic;
        f1484 :     in std_logic;
        ref2398 :   out std_logic;
        ref24 :     out std_logic;
        ref30 :     out std_logic;
        tp :        out std_logic_vector(1 downto 0)
        );
end freerunning_reference;

architecture Behavioral of freerunning_reference is

signal div16 :                              std_logic_vector(3 downto 0);
signal div2 :                               std_logic;
signal f9 :                                 std_logic;
signal f74 :                                std_logic;
signal ref2398_cnt :                        std_logic_vector(23 downto 0);
signal ref24_cnt :                          std_logic_vector(20 downto 0);
signal ref30_cnt :                          std_logic_vector(20 downto 0);

begin

tp(1) <= f9;
tp(0) <= f74;

--------------------------------------------------------------
-- global reference sync signal til 30Hz, 60Hz, 24Hz
-- og 23.98 Hz freerunning formater :

divide_f1485by16 : process( mreset, f1485 )
begin
    if f1485'event and f1485 = '0' then
        if mreset = '0' then
            div16 <= (others=>'0');
        else
            div16 <= div16 + 1;
        end if;
    end if;
end process;
f9 <= div16(3); -- f9 = 148.5/16 = 9.281 MHz clock

divide_f1484by2 : process( mreset, f1484 )
begin
    if f1484'event and f1484 = '0' then
        if mreset = '0' then
            f74 <= '0';
        else
            f74 <= not f74; -- f74 = 148.5/(2*1.001) = 74.176 MHz clock
        end if;
    end if;
end process;


make_24Hz_global_reference : process(mreset, f9)
-- størrelsen (1546874+1) * f9 cycles
-- svarer til 2 frames i 24 Hz framerate formater
begin
    if f9'event and f9 = '1' then
        if mreset = '0' or ref24_cnt = conv_std_logic_vector(1546874,21) then
            ref24_cnt <= (others=>'0');
        else
            ref24_cnt <= ref24_cnt + '1';
        end if;

        if ref24_cnt = conv_std_logic_vector(1546874,21) then
            ref24 <= '1';
--        elsif ref24_cnt = 1048576 then
        elsif ref24_cnt = 512 then
            ref24 <= '0';
        end if;

    end if;        
end process;

make_2398Hz_global_reference : process(mreset, f74)
-- størrelsen (12374999+1) * f74 cycles
-- svarer til 2 frames i 23.98 Hz framerate formater
begin
    if f74'event and f74 = '1' then
        if mreset = '0' or ref2398_cnt = conv_std_logic_vector(12374999,24) then
            ref2398_cnt <= (others=>'0');
        else
            ref2398_cnt <= ref2398_cnt + '1';
        end if;
        
        if ref2398_cnt = conv_std_logic_vector(12374999,24) then
            ref2398 <= '1';
--        elsif ref2398_cnt = 8388608 then
        elsif ref2398_cnt = 4096 then
            ref2398 <= '0';
        end if;

    end if;        
end process;

make_30Hz60Hz_global_reference : process(mreset, f9)
-- størrelsen (1237499+1) * f9 cycles
-- svarer til 2 frames i 30&60 Hz framerate formater
begin
    if f9'event and f9 = '1' then
        if mreset = '0' or ref30_cnt = conv_std_logic_vector(1237499,21) then
            ref30_cnt <= (others=>'0');
        else
            ref30_cnt <= ref30_cnt + '1';
        end if;

        if ref30_cnt = conv_std_logic_vector(1237499,21) then
            ref30 <= '1';
--        elsif ref30_cnt = 524288 then
        elsif ref30_cnt = 512 then
            ref30 <= '0';
        end if;

    end if;        
end process;

end Behavioral;