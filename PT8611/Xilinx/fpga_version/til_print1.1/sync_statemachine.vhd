library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity sync_statemachine is
    Port (  
        mreset :            in std_logic;
        tsg_reset :         in std_logic;
        f148 :              in std_logic;
        f148_div :          in std_logic;
        div10_11 :          in std_logic;   -- 0=720 lines , 1=1080 lines
        spldiv_downcount :  in std_logic_vector(9 downto 0);
        line_begin :        in std_logic;
        line_mid :          in std_logic;
        pulse_type :        in std_logic;
        no_of_pulses :      in std_logic;
        tsg_lvl :           in std_logic;
        tsg_ok :            out std_logic;
        tsg_out :           out std_logic_vector(3 downto 0);
        -- debug :
        statecounter :      out integer range 0 to 63;
        state_db :          out integer range 0 to 7;
        clksync :           out std_logic;
        smclk :             out std_logic;
        lvlsample_db :      inout std_logic;
        f74_db :            out std_logic;
        sync_sim :          out std_logic;
        sync_t_sim :        out std_logic
        );
end sync_statemachine;

architecture Behavioral of sync_statemachine is


component analog_levels
    Port(              
        tsg_reset : in std_logic;
        f148 :      in std_logic;
        sync :      in std_logic;
        sync_t :    in std_logic;
        tsg_out :  out std_logic_vector(3 downto 0);
        -- debug :
        f74_db :    out std_logic
        );
end component;


type statetype is ( negative1, negative2, positive1, positive2, counterstate1, counterstate2, flexstate);
signal state :          statetype;
signal state_counter :  std_logic_vector(5 downto 0);
signal div :            std_logic_vector(1 downto 0);
signal sm_clk :         std_logic;
signal clk_sync :       std_logic;
signal tsg_level :      std_logic;
signal sync :           std_logic;
signal sync_t :         std_logic;


begin


dac : analog_levels port map (
        tsg_reset   => tsg_reset,
        f148        => f148,
        sync        => sync,
        sync_t      => sync_t,
        tsg_out     => tsg_out,
        -- debug :
        f74_db      => f74_db
        );


-- debug :
smclk <= sm_clk;
clksync <= clk_sync;
sync_sim <= sync;
sync_t_sim <= sync_t;
statecounter <= conv_integer(state_counter);


make_state_machine_clk : process(clk_sync, f148_div)
-- der er 8 f148_div sycles per state i sync_state maskinen.
-- derfor laves en clock sm_clk = f148_div/8
-- som synkroniseres til spl_downcount vha signalet clk_sync
begin
    if clk_sync = '1' then
        div <= "00";
    elsif f148_div'event and f148_div = '0' then
        div(0) <= not div(0);
        if div(0) = '1' then
            div(1) <= not div(1);
        end if;
    end if;            
end process;
sm_clk <= div(1);

make_clk_sync : process(mreset, f148_div, spldiv_downcount )
-- clk_sync (active high) synkroniserer statemaskine clock'en til
-- spldiv_downcount fra line_mid ned til 0 = linieskift
begin
    if mreset = '0' then
        clk_sync <= '0';
    elsif f148_div'event and f148_div = '1' then
        case spldiv_downcount is
            when "0011011100" =>
                clk_sync <= '1';
            when "0000011100" =>
                clk_sync <= '1';
            when others =>
                clk_sync <= '0';
        end case;
    end if;        
end process;


make_sync_statemachine : process(mreset, sm_clk)
begin
    if mreset = '0' then
        state <= positive1;
    elsif sm_clk'event and sm_clk = '1' then
        case state is
            when flexstate =>
                if line_begin = '1' or ( line_mid = '1' and no_of_pulses = '1' ) then
                    state <= negative1;
                    state_db <= 0;
                else
                    null;
                end if;
            when negative1 =>
                state <= negative2;
            when negative2 =>
                state_db <= 1;
                state <= positive1;
            when positive1 =>
                state <= positive2;
            when positive2 =>
                state_db <= 2;
                state <= counterstate1;
                if div10_11 = '1' then
                    state_counter <= "000011";  -- 1080 lines format
                else
                    state_counter <= "001010";  -- 720 lines format
                end if;
            when counterstate1 =>
                if state_counter /= "000000" then
                    state_counter <= state_counter - 1;
                else
                    state <= counterstate2;
                    state_db <= 3;
                    if div10_11 = '1' then
                        state_counter <= "100111";  -- 1080 lines format
                    else
                        state_counter <= "111111";  -- 720 lines format
                    end if;
                end if;
            when counterstate2 =>
                if state_counter /= "000000" then
                    state_counter <= state_counter - 1;
                else
                    state <= flexstate;
                    state_db <= 4;
                end if;
            when others =>
                null;
        end case;
    end if;
end process;


make_sync : process( mreset, sm_clk )
begin
    if mreset = '0' then
        sync <= '0';
    elsif sm_clk'event and sm_clk = '0' then
        case state is
            when negative1 =>
                sync <= '0';
            when counterstate1 =>
                sync <= '1';
            when counterstate2 =>
                if pulse_type = '0' then
                    sync <= '0';
                end if;
            when flexstate =>
                sync <= '1';
            when others =>
                null;
        end case;
    end if;                
end process;


make_sync_t : process( mreset, sm_clk )
begin
    if mreset = '0' then
        sync_t <= '0';
    elsif sm_clk'event and sm_clk = '0' then
        case state is
            when positive1 =>
                sync_t <= '1';
            when counterstate1 =>
                sync_t <= '0';
            when others =>
                null;
        end case;
    end if;                
end process;


sample_tsg_lvl : process( tsg_reset, sm_clk )
-- midt i positivestate ( lavtgaaende sm_clk )
-- skal tsg_lvl ( komparatorer og and gate )
-- vaere faldet paa plads og her samples tsg_lvl
begin
    if tsg_reset = '0' then
        tsg_ok <= '1';
        tsg_level <= '1';
        lvlsample_db <= '1';
    elsif sm_clk'event and sm_clk = '1' then
        case state is
            when positive2 =>
                tsg_level <= tsg_lvl;
                lvlsample_db <= not lvlsample_db;
            when others =>
                tsg_ok <= tsg_level;
        end case;
    end if;
end process;


end Behavioral;
