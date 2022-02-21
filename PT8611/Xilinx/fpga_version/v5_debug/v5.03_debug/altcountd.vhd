library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

-----------------------------------------------------------------
-- altcount.vhd
--
-- Alternativ tæller, implementeret med +operatoren i stedet for den 
-- "binære version" phasedelay_counter.vhd
-- Med denne komponent overholdes alle timing 
-- constraints, hvorimod der ved brug af den binære FF-tæller
-- genereres
-----------------------------------------------------------------



entity phasedelay_count is
    Port( 
        start_count :   in std_logic;
        phasedelay :    in std_logic_vector(23 downto 0);
        f148 :          in std_logic;
        sync_delayed :  out std_logic;
        -- debug :
        dlycnt_active : out std_logic;
        delaycount :    out integer range 0 to 16777215
        );
end phasedelay_count;

architecture Behavioral of phasedelay_count is

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
-- opstår et delay "d1". 
-- phasedelay skal derfor ved V=0, H=0, T=0.000 vaere lig en frame minus d1.
-- NB: Hvis brugeren vælger -6.7ns i timing på PT5300, skrives et delay til generatoren på 
-- 2 frames minus d1, dvs phasedelay er altid i intervallet
-- ( 1 frame - d1 ) < phasedelay <= ( 2 frames - d1 )

begin

phasedelay_counter : process ( start_count, f148 )
-- * start_count er en 1 f148 cycle bred puls, der nulstiller counteren hver gang
--   timing-referencesignalet fra PT5300 ( f4m, f8g eller f4_free ) er registreret.
-- * sync_delayed er det forsinkede timing-referencesignal ( active high ).
-- * counter_active staller counteren, når phase_delay er nået, så der kun
--   bliver genereret 1 sync_delayed per timing-referencesignal
begin
    if start_count = '1' then
        delay_count <= phasedelay;
        counter_active <= '1';  
        sync_delayed <= '0';
    elsif f148'event and f148 = '1' then
        
        if counter_active = '1' then
            delay_count <= delay_count - 1;
        else
            null;
        end if;

        if delay_count = "000000000000000000000000" then
            sync_delayed <= '1';
        else
            sync_delayed <= '0';
        end if;

        if delay_count = "111111111111111111111111" then
            counter_active <= '0';
        end if;

    end if;
end process;

-- for debugging :
delaycount <= conv_integer(delay_count);
dlycnt_active <= counter_active;

end Behavioral;