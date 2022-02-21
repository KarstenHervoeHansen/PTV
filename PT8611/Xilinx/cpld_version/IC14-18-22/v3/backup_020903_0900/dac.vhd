library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity dac is
    Port (
        reset :         in std_logic;
        f74 :           in std_logic;
        sync :          in std_logic;
        sync_t :        in std_logic;
        d3 :            inout std_logic_vector(7 downto 0)
        );
end dac;

architecture Behavioral of dac is

type state_type is ( neg, half_neg, nulniveau, half_pos, pos );
signal state :          state_type;

-- trilevelsync_statemaskine benytter sync og sync_t fra trileveltimer til,
-- at generere de korrekte niveauer :
-- sync = sync_t = 00       => negativ niveau
-- sync = 1, sync_t = X     => nulniveau
-- sync = 0, sync_t = 1     => trilevelsync flanke ( positiv niveau )

-- fra positiv til negativ ( -300mV til 300mV ) er der 5 states ( spaendingsniveauer )
-- -300mV, -150mV, 0V, 150mV, 300mV

begin

trilevelsync_statemaskine : process( reset, f74 )
begin
    if reset = '0' then
        state <= nulniveau;
    elsif f74'event and f74 = '1' then

        case state is
            when pos =>
                if sync = '1' then
                    state <= half_pos;
                else
                    null;
                end if;

            when half_pos =>
                if sync_t = '1' and sync = '0' then
                    state <= pos;
                else
                    state <= nulniveau;
                end if;
            
            when nulniveau =>
                if sync = '0' and sync_t = '0' then
                    state <= half_neg;
                elsif sync = '0' and sync_t = '1' then
                    state <= half_pos;
                else
                    null;
                end if;

            when half_neg =>
                if sync = '0' and sync_t = '0' then
                    state <= neg;
                elsif sync = '0' and sync_t = '1' then
                    state <= nulniveau;
                else
                    state <= nulniveau;
                end if;

            when neg => 
                if ( sync = '0' and sync_t = '1' ) or ( sync = '1' )then
                    state <= half_neg;
                else
                    null;
                end if;

        end case;
    end if;
end process;

-- state-vaerdier :
-- pos              =  300 mV
-- half_pos         =  150 mV
-- nulniveau        =    0 mV
-- half_neg         = -150 mV
-- neg              = -300 mV

make_d3_output : process( reset, f74, state )
begin
    if reset = '0' then
        d3(7 downto 0)  <= "00000011";
    elsif f74'event and f74 = '0' then

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

end Behavioral;
