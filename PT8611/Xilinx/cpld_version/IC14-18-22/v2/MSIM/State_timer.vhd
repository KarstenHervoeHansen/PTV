library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity State_timer is
   Port ( clk            : in    std_logic;
          counter_load   : in    std_logic; -- saetter 'loadable_down_counter' igang
          cnt_setup      : in    integer range 0 to 1023; -- signal fra 'counter' komponenten - fastsaetter loftet for loadable_down_counter (afhaengigt af positionen i synk signalet)
          cnt_value      : inout integer range 0 to 1023 -- signal til 'state_machine' komponenten

        );
end State_timer;

architecture Behavioral of state_timer is

begin

make_count_down : process(clk)
   begin
      if (clk'event and clk = '1') then
         if counter_load = '1' then -- gaa igang ved signal fra tilstandsmaskinen
            cnt_value <= cnt_setup; -- 'load' taelleren
         else
            if cnt_value /= 0 then
                cnt_value <= (cnt_value -1);
            else
                cnt_value <= 1023;
            end if;
         end if;
      end if;
end process;

end Behavioral;
