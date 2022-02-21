library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity dac is
    Port (
        reset :         in std_logic;
        f74 :           in std_logic;
        f148 :          in std_logic;
        sync :          in std_logic;
        sync_t :        in std_logic;
        d3 :            inout std_logic_vector(7 downto 0)
    );
end dac;

architecture Behavioral of dac is

type state_type is ( neg, trekvart_neg, half_neg, kvart_neg, nulniveau, kvart_pos, half_pos, trekvart_pos, pos );
signal state :          state_type;
--signal last_state :     state_type;

begin

make_sync : process( reset, f74 )
begin
    if reset = '0' then
        state <= nulniveau;
--        last_state <= nulniveau;
    elsif f74'event and f74 = '1' then
        case state is
            when pos =>
                if sync = '1' then
                    state <= half_pos;
                else
                    null;
                end if;
--                last_state <= pos;

            when trekvart_pos =>
                state <= pos;
--                last_state <= trekvart_pos;

            when half_pos =>
                if sync_t = '1' and sync = '0' then
                    state <= pos;
                else
                    state <= nulniveau;
                end if;
--                last_state <= half_pos;
            
            when kvart_pos =>
                state <= nulniveau;
--                last_state <= kvart_pos;

            when nulniveau =>
                if sync = '0' and sync_t = '0' then
                    state <= half_neg;
                elsif sync = '0' and sync_t = '1' then
                    state <= half_pos;
                else
                    null;
                end if;
--                last_state <= nulniveau;

            when kvart_neg =>
                state <= nulniveau;
--                last_state <= kvart_neg;

            when half_neg =>
                if sync = '0' and sync_t = '0' then
                    state <= neg;
                elsif sync = '0' and sync_t = '1' then
                    state <= nulniveau;
                else
                    state <= nulniveau;
                end if;
--                last_state <= half_neg;

            when trekvart_neg =>
                state <= neg;
--                last_state <= trekvart_neg;

            when neg => 
                if ( sync = '0' and sync_t = '1' ) or ( sync = '1' )then
                    state <= half_neg;
                else
                    null;
                end if;
--                last_state <= neg;

        end case;
    end if;
end process;

-- state-vaerdier :
-- pos              =  300 mV
-- trekvart_pos     =  225 mV
-- half_pos         =  150 mV
-- nulniveau        =    0 mV
-- half_neg         = -150 mV
-- neg              = -300 mV

make_d3_output : process( reset, f74, state )
begin
    if f74'event and f74 = '0' then
        case state is
            when pos =>
                d3(7 downto 0)  <= "00000000";
            when half_pos =>
                d3(7 downto 0)  <= "00000001";
            when half_neg =>
                d3(7 downto 0)  <= "01000011";
            when neg =>
                d3(7 downto 0)  <= "11000011";
            when others =>
                d3(7 downto 0)  <= "00000011";
        end case;
    end if;     
end process;

--check_state : process( f148 ) -- debug process
--begin
--    if reset = '0' then
--        d3(0) <= '0';
--    elsif f148'event and f148 = '0' then
--        if state /= last_state then
--            d3(0) <= not d3(0);
--        else
--            null;
--        end if;
--    end if;
--end process;

end Behavioral;

       -- case state is
          --  when pos =>
        --        if sync = '1' then
      --              state <= half_pos;
    --            else
  --                  null;
--                end if;
--                last_state <= pos;

  --          when trekvart_pos =>
--                state <= pos;
--                last_state <= trekvart_pos;

--            when half_pos =>
           --     if sync_t = '1' and sync = '0' then
         --           state <= trekvart_pos;
       --         else
     --               state <= kvart_pos;
   --             end if;
--                last_state <= half_pos;
    --        
  --          when kvart_pos =>
--                state <= nulniveau;
--                last_state <= kvart_pos;

            --when nulniveau =>
            --    if sync = '0' and sync_t = '0' then
          --          state <= half_neg;
        --        elsif sync = '0' and sync_t = '1' then
      --              state <= half_pos;
    --            else
  --                  null;
--                end if;
--                last_state <= nulniveau;

  --          when kvart_neg =>
--                state <= nulniveau;
--                last_state <= kvart_neg;

            --when half_neg =>
          --      if sync = '0' and sync_t = '0' then
              --      state <= trekvart_neg;
        --        elsif sync = '0' and sync_t = '1' then
      --              state <= nulniveau;
    --            else
  --                  state <= kvart_neg;
--                end if;
--                last_state <= half_neg;

  --          when trekvart_neg =>
--                state <= neg;
--                last_state <= trekvart_neg;

          --  when neg => 
        --        if ( sync = '0' and sync_t = '1' ) or ( sync = '1' )then
      --              state <= half_neg;
    --            else
  --                  null;
--                end if;
--                last_state <= neg;
--
--        end case;





--        case state is
  --          when pos =>
    --            d3(9 downto 2)  <= "00000000";
      --      when trekvart_pos =>
        --        d3(9 downto 2)  <= "00000001";
          --  when half_pos =>
            --    d3(9 downto 2)  <= "00000011";
--            when kvart_pos =>
  --              d3(9 downto 2)  <= "00000111";
    --        when kvart_neg =>
      --          d3(9 downto 2)  <= "00011111";
        --    when half_neg =>
          --      d3(9 downto 2)  <= "00111111";
            --when trekvart_neg =>
--                d3(9 downto 2)  <= "01111111";
  --          when neg =>
    --            d3(9 downto 2)  <= "11111111";
      --      when others =>
        --        d3(9 downto 2)  <= "00001111";
--        end case;

