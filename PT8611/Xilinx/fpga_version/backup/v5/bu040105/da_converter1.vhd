library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity da_converter1 is
    Port (
        tsg_reset : in std_logic;
        f148 :      in std_logic;
        sync :      in std_logic;
        sync_t :    in std_logic;
        tsg1_out :  out std_logic_vector(3 downto 0);
        -- debug :
        f74_db :    out std_logic
        );
end da_converter1;

architecture Behavioral of da_converter1 is

signal f74 :         std_logic;
type state_type is ( negativ, half_negativ, nulniveau, half_positiv, positiv );
signal state :       state_type;

-- trilevelsync_statemaskine benytter sync og sync_t fra trileveltimer til,
-- at generere de korrekte niveauer :
-- sync = sync_t = 00           => negativ niveau
-- sync = 1, sync_t = dont care => nulniveau
-- sync = 0, sync_t = 1         => trilevelsync flanke ( positiv niveau )

-- fra positiv til negativ ( -300mV til 300mV ) er der 5 states ( tsg_out )
-- -300mV, -150mV, 0V, 150mV, 300mV

begin

-- debug signaler :
f74_db <= f74;
------------------

-- f74 = intern neddeling af f148
make_f74 : process( tsg_reset, f148 )
begin
    if tsg_reset = '0' then
        f74 <= '0';
    elsif f148'event and f148 = '0' then    -- debug : flanke aendret til nedagaaende
        f74 <= not f74;
    end if;
end process;


trilevelsync_statemaskine : process( tsg_reset, f74 )
begin
    if tsg_reset = '0' then
        state <= nulniveau;
    elsif f74'event and f74 = '1' then

        case state is
            when positiv =>
                if sync = '1' then
                    state <= half_positiv;
                else
                    null;
                end if;

            when half_positiv =>
                if sync_t = '1' and sync = '0' then
                    state <= positiv;
                else
                    state <= nulniveau;
                end if;
            
            when nulniveau =>
                if sync = '0' and sync_t = '0' then
                    state <= half_negativ;
                elsif sync = '0' and sync_t = '1' then
                    state <= half_positiv;
                else
                    null;
                end if;

            when half_negativ =>
                if sync = '0' and sync_t = '0' then
                    state <= negativ;
                else
                    state <= nulniveau;
                end if;

            when negativ => 
                if sync_t = '1' or sync = '1' then
                    state <= half_negativ;
                else
                    null;
                end if;

        end case;
    end if;
end process;

-- state-vaerdier :
-- positiv          =  600 mV
-- half_positiv     =  300 mV
-- nulniveau        =    0 mV
-- half_neg         = -300 mV
-- neg              = -600 mV

make_tsg1_output : process( tsg_reset, f74, state )
begin
    if tsg_reset = '0' then
        tsg1_out(3 downto 0)  <= "0011";
    elsif f74'event and f74 = '0' then

        case state is
            when positiv =>
                tsg1_out(3 downto 0)  <= "0000";
            when half_positiv =>
                tsg1_out(3 downto 0)  <= "0001";
            when half_negativ =>
                tsg1_out(3 downto 0)  <= "0111";
            when negativ =>
                tsg1_out(3 downto 0)  <= "1111";
            when others =>
                tsg1_out(3 downto 0)  <= "0011";
        end case;

    end if;     
end process;

end Behavioral;
