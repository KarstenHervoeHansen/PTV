library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;

entity bilevel_timer is
    Port(
        mreset :        in std_logic;
        f74 :           in std_logic;
        sample :        in integer range 0 to 2048;
        line :          in integer range 0 to 1024;
        sync :          out std_logic;
        sync_t :        out std_logic;
        blank :         out std_logic
    );
end bilevel_timer;

architecture Behavioral of bilevel_timer is

signal line_begin : std_logic;  -- indikerer horisontal sync
signal line_mid :   std_logic;    -- saetter gang i pre-, vertical- eller post equalizing puls
signal nofp :       std_logic;        -- number of pulses per line 0=1, 1=2

begin

blank <= '1';
sync <= '1';
sync_t <= '0';

--make_line_begin : process( mreset, f74 )
--begin
--    if  mreset = '0' then
--    elsif f74'event and f74 = '0' then
--        case sample is
--            when 0 =>
--            when 63 =>
--            when others =>
--                null;
--        end case;
--    end if;
--end process;


end Behavioral;
