library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity sync_statemachine is
    Port (  
        tsg_reset :         in std_logic;
        f148_div :          in std_logic;
        spldiv_downcount :  in std_logic_vector(9 downto 0);
        line_begin :        in std_logic;
        line_mid :          in std_logic;
        pulse_type :        in std_logic;
        no_of_pulses :      in std_logic;
        sync :              out std_logic;  -- styresignal til konverteren
        sync_t :            out std_logic  -- styresignal til konverteren
        -- debug :
--        clksync :           out std_logic;
  --      smclk :             out std_logic
        );
end sync_statemachine;

architecture Behavioral of sync_statemachine is

type statetype is ( videostate, zero1, negativestate, positivestate, zero2, zero3 );
signal state :      statetype;
signal nextstate :  statetype;
signal div :        std_logic_vector(1 downto 0);
signal sm_clk :     std_logic;
signal clk_sync :   std_logic;

begin

-- debug :
--smclk <= sm_clk;
--clksync <= clk_sync;

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

make_clk_sync : process(tsg_reset, f148_div)
-- clk_sync (active high) synkroniserer statemaskine clock'en til
-- spldiv_downcount fra line_mid ned til 0 = linieskift
begin
    if tsg_reset = '0' then
        clk_sync <= '0';
    elsif f148_div'event and f148_div = '1' then
        if spldiv_downcount = "0000111100" then
            clk_sync <= '1';
        else
            clk_sync <= '0';
        end if;
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
make_sync_signals : process(tsg_reset, sm_clk)
begin
    if tsg_reset = '0' then
        state <= positivestate;
        sync <= '1';
        sync_t <= '0';
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


end Behavioral;
