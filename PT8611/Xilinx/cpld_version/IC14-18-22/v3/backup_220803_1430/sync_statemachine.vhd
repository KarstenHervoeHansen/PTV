library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity sync_statemachine is
    Port (  
        tsg_start :     in std_logic;
        f148_div :      in std_logic;

        line_begin :    in std_logic;
        line_mid :      in std_logic;
        pulse_type :    in std_logic;
        no_of_pulses :  in std_logic;
    
        sync :          out std_logic;  -- styresignal til konverteren
        sync_t :        out std_logic   -- styresignal til konverteren
        );
end sync_statemachine;

architecture Behavioral of sync_statemachine is

type statetype is ( waitstate, zero1, negativestate, positivestate, zero2, waitone );
signal state :      statetype;

begin

make_sync_signals : process(tsg_start, f148_div)
begin
    if tsg_start = '0' then
        state <= positivestate;
        sync <= '0';
        sync_t <= '1';
    elsif f148_div'event and f148_div = '0' then
        case state is
            when waitstate =>
                if line_begin = '1' or ( line_mid = '1' and no_of_pulses = '1' ) then
                    state <= zero1;
                    sync <= '1';
                else
                    null;
                end if;
            when zero1 =>
                state <= negativestate;
                sync <= '0';
            when negativestate =>
                state <= positivestate;
                sync_t <= '1';
            when positivestate =>
                state <= zero2;
                sync <= '1';
            when zero2 =>
                state <= waitone;
                sync_t <= '0';
            when waitone =>
                state <= waitstate;
                if pulse_type = '0' then
                    sync <= '0';
                else
                    null;
                end if;
        end case;
    end if;
end process;

end Behavioral;
