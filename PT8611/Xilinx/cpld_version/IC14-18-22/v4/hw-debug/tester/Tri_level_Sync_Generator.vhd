library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-----------------------------------------------------------------------------------------------
-- v1:      Original kode til Prototype lavet af PEH + studerende Jesper Christoffersen 
--          for DK-Audio, som afgangsprojekt november 2002 til marts 2003
-- v2:      19/6 2003 - PS      tilfoejet clamping- og enablesignaler til rekonstruktionsfiltre
-- v3:      23/5 2003 - PS      nyt hieraki => toplevel = Tri_level_sync_generator.vhd
-- v301 :   21/8 2003 - PS      ny komponent sync_statemachine og state_timer
-- v4 :     8/9  2003 - PS      nyt printudlaeg m 100pin CPLD => ny pin-konfiguration
-----------------------------------------------------------------------------------------------

entity Tri_Level_Sync_Generator is
   Port ( 
--------- fra Mainframe -----------------------------------------------------------------------
        mreset :                in std_logic;       -- Master Reset fra mainboard
        f27m :                  in std_logic;       -- 27 MHz masterclock fra mainboard
        f14835m :               in std_logic;       -- 148.35 MHz clock fra IC2
        f1485m :                in std_logic;       -- 148.5 MHz clock fra IC4

        f4m :                   in std_logic;       -- 1/8 x Vertikal sync  ( NTSC format )
        fhm :                   in std_logic;       -- Horisontal sync      ( NTSC - 525 linier pr frame )
        f8g :                   in std_logic;       -- 1/4 x Vertikal sync  ( PAL format )
        fhg :                   in std_logic;       -- Horisontal sync      ( PAL - 625 linier pr frame )

--------- til/fra MasterCPLD ------------------------------------------------------------------
        gpbus :                 out std_logic_vector(7 downto 0);
        
--------- til/fra CPU -------------------------------------------------------------------------
        cs :                    in std_logic;
        sck :                   in std_logic;
        mosi :                  in std_logic;
        miso :                  in std_logic;       -- (output not used yet)

--------- til rekonstruktions filter -----------------------------------------------------------
        sync_in :               out std_logic;      -- styrer clamping paa filter output

--------- til THS8135 --------------------------------------------------------------------------
--        dac_clk :               out std_logic;      -- f74i - internt genereret af f148
--        dac_mode :              in std_logic_vector(1 downto 0); -- pin m2 og m1 pa DAC
--        blank :                 out std_logic;        -- altid lav
        sync :                  inout std_logic;      -- Sync styresignal til konverteren
        sync_t :                inout std_logic;      -- Sync styresignal til konverteren

--        rcr :                   out std_logic_vector(9 downto 0);   -- output for debug (not used yet)
--        gy :                    out std_logic_vector(9 downto 0);    -- not used yet
--        bcb_h :                 out std_logic_vector(5 downto 0); -- inout for debug

        bcb_l :                 out std_logic_vector(3 downto 0)
        );
end Tri_Level_Sync_Generator;


architecture Behavioral of Tri_Level_Sync_Generator is

signal phase :          std_logic_vector(9 downto 0);
signal f148_div :       std_logic;
signal f14835_div2 :    std_logic;
signal f1485_div2 :     std_logic;

signal div :        integer range 0 to 15;

begin

bcb_l(3) <= phase(9);
bcb_l(2) <= phase(8);
bcb_l(1) <= phase(7);
bcb_l(0) <= phase(6);

test_f14835m : process( mreset, f14835m )
begin
    if mreset = '0' then
        f14835_div2 <= '0';
    elsif f14835m'event and f14835m = '1' then
        f14835_div2 <= not f14835_div2;
    end if;
end process;

test_f1485m : process( mreset, f1485m )
begin
    if mreset = '0' then
        f1485_div2 <= '0';
    elsif f1485m'event and f1485m = '1' then
        f1485_div2 <= not f1485_div2;
    end if;
end process;


divf27 : process( mreset, f27m )
begin
    if mreset = '0' then
        phase <= "0000000000";
    elsif f27m'event and f27m = '1' then
        phase(0) <= not phase(0);

        if phase(0) = '1' then
            phase(1) <= not phase(1);
        end if;

        if phase(1 downto 0) = "11" then
            phase(2) <= not phase(2);
        end if;

        if phase(2 downto 0) = "111" then
            phase(3) <= not phase(3);
        end if;

        if phase(3 downto 0) = "1111" then
            phase(4) <= not phase(4);
        end if;

        if phase(4 downto 0) = "11111" then
            phase(5) <= not phase(5);
        end if;

        if phase(5 downto 0) = "111111" then
            phase(6) <= not phase(6);
        end if;

        if phase(6 downto 0) = "1111111" then
            phase(7) <= not phase(7);
        end if;

        if phase(7 downto 0) = "11111111" then
            phase(8) <= not phase(8);
        end if;

        if phase(8 downto 0) = "111111111" then
            phase(9) <= not phase(9);
        end if;
    end if;
end process;

make_f1438_div : process( mreset, f14835_div2  ) 
begin
    if mreset = '0' then
        div <= 0;
        f148_div <= '0';
    elsif f14835_div2'event and f14835_div2 = '0' then 
        if (div /= 10) then
            div <= div + 1;
            if div = 4 then
                f148_div <= '1';
            end if;
        else
            div <= 0;
            f148_div <= '0';
        end if;
    end if;
end process;

sync <= f148_div;
sync_t <= f148_div and f1485_div2;


end Behavioral;
