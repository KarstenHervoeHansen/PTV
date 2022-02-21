library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity phasedelay is
    Port( 
        start_count :       in std_logic;
        phasedelay_reg :    in std_logic_vector(25 downto 0);
        f148 :              in std_logic;
        f4m_delayed :       out std_logic
        );
end phasedelay;

architecture Behavioral of phasedelay is

signal delay_count :        std_logic_vector(25 downto 0);
signal counter_active :     std_logic;

begin

down_count : process ( start_count, f148 )
begin
    if start_count = '1' then
        delay_count <= "00000000000000000000000000";
        f4m_delayed <= '0';
        counter_active <= '1';
    elsif f148'event and f148 = '1' then
        
        if counter_active = '1' then
            delay_count(0) <= not delay_count(0);
        else
            null;
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

        if delay_count(23 downto 0) = "111111111111111111111111" then
            delay_count(24) <= not delay_count(24);
        end if;

        if delay_count(24 downto 0) = "1111111111111111111111111" then
            delay_count(25) <= not delay_count(25);
        end if;

        if delay_count = phasedelay_reg then
            f4m_delayed <= '1';
            counter_active <= '0';
        else
            f4m_delayed <= '0';
        end if;

    end if;
end process;

end Behavioral;
