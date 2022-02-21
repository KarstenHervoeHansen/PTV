library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Tilstandsmaskine is
    Port (  reset      : in std_logic;
            clk            : in std_logic;
            command        : in integer range 0 to 2; -- 
            cnt_setup      : in integer range 0 to 1023; -- signal kommende fra 'format_config' komponenten

            sync       : out std_logic; -- styresignal til konverteren
            sync_t     : out std_logic; -- styresignal til konverteren
            blank      : out std_logic -- styresignal til konverteren
         );
end Tilstandsmaskine;

architecture Behavioral of Tilstandsmaskine is

component State_timer
   Port ( clk            : in  std_logic;

          counter_load   : in    std_logic; -- saetter 'loadable_down_counter' igang
          cnt_load_value : in    integer range 0 to 1023; -- signal fra 'counter' komponenten - fastsaetter loftet for loadable_down_counter (afhaengigt af positionen i synk signalet)
          cnt_value      : inout integer range 0 to 1023
        );
end component;

signal counter_load     : std_logic := '0'; -- bruges af 'loadable_down_counter' komponenten, saetter taelleren igang
signal State_cond       : integer range 0 to 15; -- signal fra 'counter' komponenten
signal cnt_value        : integer range 0 to 1023 := 11; -- bruges af komponenten 'state_machine', til styring af en waitstate
signal command_started  : integer range 0 to 2;
signal cnt_load_value   : integer range 0 to 1023;

begin

down_counter : State_timer port map ( 
    clk               => clk,
    cnt_load_value    => cnt_load_value,
    counter_load      => counter_load,
    cnt_value         => cnt_value     
    );

make_cnt_load_value : process(State_cond)
begin
    case State_cond is
        when 1 =>  cnt_load_value <= cnt_setup; -- normalt specificeret af brugeren!
        when 3 =>  cnt_load_value <= 85; -- 4 clocks : puls lengch - 3
        when 5 =>  cnt_load_value <= 85; -- 4 clocks : puls lengch - 3
        when 7 =>  cnt_load_value <= 85; -- 8 clocks : puls lengch - 3
        when others =>   null;
    end case;
end process;
                        
make_state_machine : process(reset, clk)
   begin
      if reset = '0' then
         State_cond <= 0;
      elsif (clk'event and clk = '1') then
         case State_cond is

            when 0 =>  -- vent paa 'command'
               if command /= 0 then
                   State_cond      <= State_cond + 1;
                   command_started <= command; -- gemmer information om pulstype i 'command_started'
               end if;

            when 1 =>  -- lav nul niveau
               counter_load <= '1';
               State_cond <= State_cond + 1;
               sync <= '1';
               sync_T <= '-';

            when 2 => -- wait state, laengden paa denne waitstate aendrer sig afh. af formatet
               counter_load <= '0';
               if cnt_value = 0 then
                  State_cond <= State_cond + 1;
               else
                  null;
               end if;

            when 3 =>  -- lav negativt niveau
               counter_load <= '1';
               State_cond <= State_cond + 1;
               sync <= '0';
               sync_T <= '0';

            when 4 => -- wait state, har altid laengden 4
               counter_load <= '0';
               if cnt_value = 0 then
                  State_cond <= State_cond + 1;
               else
                  null;
               end if;  

            when 5 =>  -- lav positivt niveau
               counter_load <= '1';
               State_cond <= State_cond + 1;
               sync <= '0';
               sync_T <= '1';

            when 6 => -- wait state, har altid laengden 4
                counter_load <= '0';
                if cnt_value = 0 then
                   State_cond <= State_cond + 1;
                else
                   null;
                end if;   

            when 7 =>  -- lav nul niveau
                counter_load <= '1';
                State_cond <= State_cond + 1;
                sync <= '1';
                sync_T <= '-';

            when 8 => -- wait state, har altid laengden 8
                counter_load <= '0';
                if cnt_value = 0 then
                    if command_started = 1 then     
                        State_cond <= State_cond + 1;  -- lav puls type 1
                    else                              
                        State_cond <= State_cond + 2;  -- lav puls type 2
                    end if;
                end if;

            when 9 =>  -- pulse type 1      -- lav negativt niveau
                counter_load <= 'X';         -- 'counter_load' uden betydning her
                State_cond <= 0; 
                sync <= '0';
                sync_T <= '0';

            when 10 =>  -- pulse type 2     -- behold nul niveau
                counter_load <= 'X';         -- 'counter_load' uden betydning her
                State_cond <= 0; 
                sync <= '1';
                sync_T <= '-';

            when others =>
                State_cond   <= 0;           -- gaa til foerste state hvis current state er ulovlig
--                State_cond <= 'X';
--                sync <= 'X';
--                sync_T <= 'X';
         end case;
      end if;

blank <= '0'; -- blank skal altid vaere nul
      
end process;


end Behavioral;
