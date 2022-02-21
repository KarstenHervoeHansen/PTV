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
        f27 :       in std_logic;
        ref2398 :   out std_logic;
        ref24 :     out std_logic;
        ref30 :     out std_logic
        );
end freerunning_reference;

architecture Behavioral of freerunning_reference is

signal div32 :                              std_logic_vector(4 downto 0);
signal f675 :                               std_logic;
signal f844 :                               std_logic;
signal ref2398_cnt :                        std_logic_vector(20 downto 0);
signal ref24_cnt :                          std_logic_vector(17 downto 0);
signal ref30_cnt :                          std_logic_vector(16 downto 0);

begin

--------------------------------------------------------------
-- global reference sync signal til 30Hz, 60Hz, 24Hz
-- og 23.98 Hz freerunning formater :

divide_f27by32 : process( mreset, f27 )
begin
    if mreset = '0' then
        div32 <= (others=>'0');
    elsif f27'event and f27 = '1' then
        div32 <= div32 + 1;
    end if;
end process;

f844 <= div32(4);   -- 27MHz divideret med 32 = 0.844 MHz
f675 <= div32(1);   -- 27MHz divideret med 4 = 6.750 MHz

make_2398Hz_global_reference : process(mreset, f675)
-- størrelsen (1126124+1) * f675 cycles
-- svarer til 2 frames i 23.98 Hz framerate formater
begin
    if mreset = '0' then
        ref2398_cnt <= (others=>'0');
        ref2398 <= '0';
    elsif f675'event and f675 = '1' then
        if ref2398_cnt /= conv_std_logic_vector(1126124,21) then
            ref2398_cnt <= ref2398_cnt + 1;
            ref2398 <= '0';
        else
            ref2398_cnt <= (others=>'0');
            ref2398 <= '1';
        end if;
    end if;        
end process;

make_24Hz_global_reference : process(mreset, f844)
-- størrelsen (140624+1) * f844 cycles
-- svarer til 2 frames i 24 Hz framerate formater
begin
    if mreset = '0' then
        ref24_cnt <= (others=>'0');
        ref24 <= '0';
    elsif f844'event and f844 = '1' then
        if ref24_cnt /= conv_std_logic_vector(140624,18) then
            ref24_cnt <= ref24_cnt + 1;
            ref24 <= '0';
        else
            ref24_cnt <= (others=>'0');
            ref24 <= '1';
        end if;
    end if;        
end process;

make_30Hz60Hz_global_reference : process(mreset, f844)
-- størrelsen (112499+1) * f844 cycles
-- svarer til 2 frames i 30&60 Hz framerate formater
begin
    if mreset = '0' then
        ref30_cnt <= (others=>'0');
        ref30 <= '0';
    elsif f844'event and f844 = '1' then
        if ref30_cnt /= conv_std_logic_vector(112499,17) then
            ref30_cnt <= ref30_cnt + 1;
            ref30 <= '0';
        else
            ref30_cnt <= (others=>'0');
            ref30 <= '1';
        end if;
    end if;        
end process;

end Behavioral;