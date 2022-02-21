library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Tilstandsmaskine is
    Port (  reset :         in std_logic;
            f148 :          in std_logic;
            command :       in integer range 0 to 2;   -- saetter tilstandsmaskinen igang og bestemmer pulstype

            scan_method :   in std_logic;               -- ( 0=interlaced, 1=progressiv )
            cnt_setup :     in integer range 0 to 1023;     -- signal kommende fra 'format_config' komponenten

            sync :          out std_logic; -- styresignal til konverteren
            sync_t :        out std_logic; -- styresignal til konverteren
            blank :         out std_logic;  -- styresignal til konverteren

            state :         out integer range 0 to 15 -- for debug only

         );
end Tilstandsmaskine;

architecture Behavioral of Tilstandsmaskine is

component State_timer
   Port ( f148           : in  std_logic;
          counter_load   : in    std_logic; -- saetter 'loadable_down_counter' igang
          cnt_load_value : in    integer range 0 to 1023; -- signal fra 'counter' komponenten - fastsaetter loftet for loadable_down_counter (afhaengigt af positionen i synk signalet)
          cnt_value      : inout integer range 0 to 1023
        );
end component;

signal counter_load     : std_logic := '0'; -- bruges af 'loadable_down_counter' komponenten, saetter taelleren igang
signal State_cond       : integer range 0 to 15; -- signal fra 'counter' komponenten
signal cnt_value        : integer range 0 to 1023 := 11; -- bruges af komponenten 'state_machine', til styring af en waitstate
signal command_started  : integer range 0 to 2;
signal cnt_setup_value   : integer range 0 to 1023;

begin

state <= State_cond;    -- for debug only

down_counter : State_timer port map ( 
    f148              => f148,
    cnt_load_value    => cnt_setup_value,
    counter_load      => counter_load,
    cnt_value         => cnt_value     
    );

-- original :
--make_cnt_load_value : process(State_cond)
--begin
--    case State_cond is
--        when 1 =>  
--            cnt_load_value <= cnt_setup; -- normalt specificeret af brugeren!
--        when 3 =>  
--            cnt_load_value <= 85; -- 4 clocks : puls lengch - 3
--        when 5 =>  
--            cnt_load_value <= 85; -- 4 clocks : puls lengch - 3
--        when 7 =>  
--            cnt_load_value <= 85; -- 8 clocks : puls lengch - 3
--        when others =>   null;
--    end case;
--end process;
make_cnt_load_value : process(State_cond)
begin
    case State_cond is
        when 1 =>  
            cnt_setup_value <= cnt_setup;   -- video format register
        when others => 
            if scan_method = '0' then
                cnt_setup_value <= 85;      -- pulsbredde = ( 44T * 2 ) - 3
            else
                cnt_setup_value <= 77;      -- pulsbredde = ( 40T * 2 ) - 3
            end if;
    end case;
end process;

                        
make_state_machine : process( reset, f148 )
begin
    if reset = '0' then
        State_cond <= 0;
    elsif (f148'event and f148 = '1') then
        case State_cond is
            when 0 =>
                if command /= 0 and State_cond = 0 then
                    State_cond <= State_cond + 1;
                    command_started <= command; -- gemmer information om pulstype i 'command_started'
                    sync <= '1';                -- genererer nul niveau
                    sync_T <= '-';
                end if;
            when 1 =>
                counter_load <= '1';
                State_cond <= State_cond + 1;
            when 2 => -- wait state
                counter_load <= '0';
                if cnt_value = 0 then
                    State_cond <= State_cond + 1;
                    sync <= '0';                -- genererer negativt niveau
                    sync_T <= '0';
                else
                    null;
                end if;
            when 3 =>
                counter_load <= '1';
                State_cond <= State_cond + 1;
            when 4 => -- wait state
                counter_load <= '0';
                if cnt_value = 0 then
                    State_cond <= State_cond + 1;
                    sync <= '0';                -- genererer positivt niveau
                    sync_T <= '1';
                else
                    null;
                end if;  
            when 5 =>
                counter_load <= '1';
                State_cond <= State_cond + 1;
            when 6 => -- wait state
                counter_load <= '0';
                if cnt_value = 0 then
                    State_cond <= State_cond + 1;
                    sync <= '1';                -- generer nul niveau
                    sync_T <= '-';
                else
                    null;
                end if;   
            when 7 =>  
                counter_load <= '1';
                State_cond <= State_cond + 1;
            when 8 => -- wait state
                counter_load <= '0';
                if cnt_value = 0 then
                    if command_started = 1 then     
                        State_cond <= 0;
                        sync <= '0';            -- lav puls type 1
                        sync_T <= '0';
                    else                              
                        State_cond <= 0;  
                        sync <= '1';            -- lav puls type 2
                        sync_T <= '-';
                    end if;
                end if;
            when others =>
                State_cond   <= 0;              -- gaa til foerste state hvis current state er ulovlig
                counter_load <= 'X';            -- 'counter_load' uden betydning her
                sync <= 'X';
                sync_T <= 'X';
            end case;
    end if;

blank <= '0'; -- blank skal altid vaere nul
      
end process;


end Behavioral;
