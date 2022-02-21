library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity phasedelay_count1 is
    Port( 
        start_count :   in std_logic;
        phasedelay :    in std_logic_vector(23 downto 0);
        f148 :          in std_logic;
        sync_delayed :  out std_logic;
        -- debug :
        dlycnt_active : out std_logic;
        delaycount :    out integer range 0 to 16777215
        );
end phasedelay_count1;

architecture Behavioral of phasedelay_count1 is

signal delay_count :        std_logic_vector(23 downto 0);  -- phasedelay i f148 cycles
signal counter_active :     std_logic;                      -- benyttes til at starte/stoppe counter

---------------------------------------------------------------
-- default phasedelay ( V=0, H=0, T=0.000 )
-- Forklaring/illustration :
--
-- v = synctidspunkt                    v 
--                      ____             ___________             __________
-- f27m                     |___________|           |___________| 
--                                       _______________________________
-- f4m,f8g              ________________|
--                                                   ____________________
-- f4m_tmp,f8g_tmp      ____________________________|
--                                                     _______________
-- intern_sync         _______________________________|
--                                                           ___
-- start_counter       _____________________________________|   |__
--                         _   _   _   _   _   _   _   _   _   _   _         _   _  
-- f148                  _| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_ ... _| |_| |_
--
-- count indtil delay_count = phasedelay og derefter :                       ___
-- sync_delayed        _____________________________________________________|   |_____
--
-- sync_delayed saetter sync_ok hoej og 'frigiver' tri_level_timer :         ________
-- sync_ok (greset)    _____________________________________________________|
--
-- Ved sampling af f4m og f8g fra mainframebussen og efterflgende synkronisering mm.
-- opstr et delay. phasedelay skal derfor ved V=0, H=0, T=0.000 vaere lig 
-- en frame minus dette delay (20 148MHz cycles)

begin


down_count : process ( start_count, f148 )
begin
    if start_count = '1' then
        delay_count <= "000000000000000000000000";
        sync_delayed <= '0';
        counter_active <= '1';
    elsif f148'event and f148 = '1' then
        
        if counter_active = '1' then
            delay_count(0) <= not delay_count(0);
        else
            delay_count(0) <= '0';  -- counter stopper
        end if;

        if delay_count(0) = '1' then
            delay_count(1) <= not delay_count(1);
        end if;

        if delay_count(1 downto 0) = "11" then
            delay_count(2) <= not delay_count(2);
        end if;

        if delay_count(2 downto 0) = "111" then
            delay_count(3) <= not delay_count(3);
        end if;

        if delay_count(3 downto 0) = "1111" then
            delay_count(4) <= not delay_count(4);
        end if;

        if delay_count(4 downto 0) = "11111" then
            delay_count(5) <= not delay_count(5);
        end if;

        if delay_count(5 downto 0) = "111111" then
            delay_count(6) <= not delay_count(6);
        end if;

        if delay_count(6 downto 0) = "1111111" then
            delay_count(7) <= not delay_count(7);
        end if;

        if delay_count(7 downto 0) = "11111111" then
            delay_count(8) <= not delay_count(8);
        end if;

        if delay_count(8 downto 0) = "111111111" then
            delay_count(9) <= not delay_count(9);
        end if;

        if delay_count(9 downto 0) = "1111111111" then
            delay_count(10) <= not delay_count(10);
        end if;

        if delay_count(10 downto 0) = "11111111111" then
            delay_count(11) <= not delay_count(11);
        end if;

        if delay_count(11 downto 0) = "111111111111" then
            delay_count(12) <= not delay_count(12);
        end if;

        if delay_count(12 downto 0) = "1111111111111" then
            delay_count(13) <= not delay_count(13);
        end if;

        if delay_count(13 downto 0) = "11111111111111" then
            delay_count(14) <= not delay_count(14);
        end if;

        if delay_count(14 downto 0) = "111111111111111" then
            delay_count(15) <= not delay_count(15);
        end if;

        if delay_count(15 downto 0) = "1111111111111111" then
            delay_count(16) <= not delay_count(16);
        end if;

        if delay_count(16 downto 0) = "11111111111111111" then
            delay_count(17) <= not delay_count(17);
        end if;

        if delay_count(17 downto 0) = "111111111111111111" then
            delay_count(18) <= not delay_count(18);
        end if;

        if delay_count(18 downto 0) = "1111111111111111111" then
            delay_count(19) <= not delay_count(19);
        end if;

        if delay_count(19 downto 0) = "11111111111111111111" then
            delay_count(20) <= not delay_count(20);
        end if;

        if delay_count(20 downto 0) = "111111111111111111111" then
            delay_count(21) <= not delay_count(21);
        end if;

        if delay_count(21 downto 0) = "1111111111111111111111" then
            delay_count(22) <= not delay_count(22);
        end if;

        if delay_count(22 downto 0) = "11111111111111111111111" then
            delay_count(23) <= not delay_count(23);
        end if;

        if delay_count = phasedelay then
            sync_delayed <= '1';
            counter_active <= '0';
        else
            sync_delayed <= '0';
        end if;

    end if;
end process;

-- for debugging :
delaycount <= conv_integer(delay_count);
dlycnt_active <= counter_active;

end Behavioral;
