library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity sync_statemachine2 is
    Port (  
        mreset :            in std_logic;
        tsg_reset :         in std_logic;
        f148 :              in std_logic;
        f148_div :          in std_logic;
        spldiv_downcount :  in std_logic_vector(9 downto 0);
        line_begin :        in std_logic;
        line_mid :          in std_logic;
        pulse_type :        in std_logic;
        no_of_pulses :      in std_logic;
        tsg2_lvl :          in std_logic;
        tsg2_ok :           out std_logic;
        tsg2_out :          out std_logic_vector(3 downto 0);
        -- debug :
        clksync :           out std_logic;
        smclk :             out std_logic;
        f74_db :            out std_logic;
        sync_sim :          out std_logic;
        sync_t_sim :        out std_logic
        );
end sync_statemachine2;

architecture Behavioral of sync_statemachine2 is


component da_converter2
    Port(
        tsg_reset : in std_logic;
        f148 :      in std_logic;
        sync :      in std_logic;
        sync_t :    in std_logic;
        tsg2_out :  out std_logic_vector(3 downto 0);
        -- debug :
        f74_db :    out std_logic
        );
end component;


type statetype is ( videostate, zero1, negativestate, positivestate, zero2, zero3 );
signal state :      statetype;
signal div :        std_logic_vector(1 downto 0);
signal sm_clk :     std_logic;
signal clk_sync :   std_logic;
signal sync :       std_logic;
signal sync_t :     std_logic;


begin


dac : da_converter2 port map (
        tsg_reset   => tsg_reset,
        f148        => f148,
        sync        => sync,
        sync_t      => sync_t,
        tsg2_out    => tsg2_out,
        -- debug :
        f74_db      => f74_db
        );


-- debug :
smclk <= sm_clk;
clksync <= clk_sync;
sync_sim <= sync;
sync_t_sim <= sync_t;


make_state_machine_clk : process(clk_sync, f148_div)
-- der er 8 f148_div sycles per state i sync_state maskinen.
-- derfor laves en clock sm_clk = f148_div/8
-- som synkroniseres til spl_downcount vha signalet clk_sync
begin
    if clk_sync = '1' then
        div <= "00";
        sm_clk <= '0';
    elsif f148_div'event and f148_div = '0' then
        if div /= "11" then
            div <= div + 1;
        else
            div <= "00";
            sm_clk <= not sm_clk;
        end if;
    end if;            
end process;

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

--------------------------------------------------------------------------------------------------------------
-- ( sync og sync_t fungerer som input til baade "hjemmelavet" DAC og THS8135 VideoDAC )
--
-- (sync, sync_t) = (0, 0) => gaa til negativ               ( output = -300mV )
-- (sync, sync_t) = (0, 1) => gaa til positiv               ( output = 300mV )
-- (sync, sync_t) = (1, don't care) => gaa til nulnivaeu    ( output = 0V )
--
-- sm_clk :    ________         ________         ________         ________         ________         _____
--  __________|        |_______|        |_______|        |_______|        |_______|        |_______|        
--
--            |-- 88T el 80T --|   ( T = 1/148MHz )
--                                               ________________
--   _ _ _ _ _ ________________                 |                |________________________________ _ _ _ _ _ _
--   _ _ _ _ _|                |________________|                                                 |_ _ _ _ _ _
--
--     |              |                 |                |              |              |                |
-- videostate       zero1         negativestate    positivestate      zero2         zero3           videostate  
--
make_sync_signals : process(mreset, sm_clk)
begin
    if mreset = '0' then
        state <= positivestate;
        sync <= '1';    -- gaa til nulnivaeu
        sync_t <= '0';  -- gaa til nulnivaeu
    elsif sm_clk'event and sm_clk = '1' then
        case state is
            when videostate =>
                if line_begin = '1' or ( line_mid = '1' and no_of_pulses = '1' ) then
                    state <= zero1;
                    sync <= '1';    -- gaa til nulnivaeu
                else
                    null;
                end if;
            when zero1 =>
                state <= negativestate;
                sync <= '0';        -- gaa negativ ( sync_t er altid = 0 paa dette sted)
            when negativestate =>
                state <= positivestate;
                sync_t <= '1';      -- lav sync puls - negativ til positiv
            when positivestate =>
                state <= zero2;
                sync <= '1';        -- gaa til nulniveau
            when zero2 =>
                state <= zero3;
                sync_t <= '0';      -- reset sync_t ( ingen skift i niveau )
            when zero3 =>
                state <= videostate;
                if pulse_type = '0' then
                    sync <= '0';    -- gaa negativ
                else
                    null;           -- bliv i nulniveau
                end if;
            when others =>
                null;
        end case;
    end if;
end process;


sample_tsg_lvl : process( mreset, sm_clk )
-- midt i positivestate ( lavtgaaende sm_clk )
-- skal tsg_lvl ( komparatorer og and gate )
-- vaere faldet paa plads og her samples tsg_lvl
begin
    if mreset = '0' then
        tsg2_ok <= '1';
    elsif sm_clk'event and sm_clk = '0' then
        case state is
            when positivestate =>
                tsg2_ok <= tsg2_lvl;
            when others =>
                tsg2_ok <= '1';
        end case;
    end if;
end process;


end Behavioral;
