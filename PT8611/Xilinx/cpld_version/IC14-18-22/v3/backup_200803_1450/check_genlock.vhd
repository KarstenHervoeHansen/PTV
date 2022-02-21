library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity check_genlock is
    Port ( 
        reset :         in std_logic;
        f148 :          in std_logic;
        f4m_int :       in std_logic;
        sync_pulse :    in std_logic;
        sync_ok :       out std_logic);
end check_genlock;

architecture Behavioral of check_genlock is

type state_type is ( wait_for_sync, first_check, second_check, sync_ok_state, out_of_sync);
signal state : state_type;

begin

check_genlock_statemachine : process( reset, f148 )
begin
    if reset = '0' then
        state <= wait_for_sync;
        sync_ok <= '0'; -- evt '1' ??
    elsif f148'event and f148 = '1' then
        case state is

            when wait_for_sync =>
                if f4m_int = '1' then
                    if sync_pulse = '0' then
                        state <= sync_ok_state;
                        sync_ok <= '1';
                    else
                        state <= first_check;
                    end if;
                else
                    null;
                end if;

            when first_check =>
                if sync_pulse = '0' then
                    state <= sync_ok_state;
                    sync_ok <= '1';
                else
                    state <= second_check;
                end if;

            when second_check =>
                if sync_pulse = '0' then
                    state <= sync_ok_state;
                    sync_ok <= '1';
                else
                    state <= out_of_sync;
                    sync_ok <= '0';
                end if;

            when sync_ok_state =>
                if f4m_int = '0' then
                    state <= wait_for_sync;
                else
                    null;
                end if;

            when out_of_sync =>
                if f4m_int = '0' then
                    state <= wait_for_sync;
                else
                    null;
                end if;

        end case;
    end if;
end process;

end Behavioral;
