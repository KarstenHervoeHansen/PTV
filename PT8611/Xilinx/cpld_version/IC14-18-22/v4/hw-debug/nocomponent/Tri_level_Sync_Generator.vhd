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
        f74m :                  in std_logic;       -- output clock fra clock synthesizer - input til multiplier
        f148m :                 in std_logic;       -- 148 MHz master clock fra clock multiplier

        f4m :                   in std_logic;       -- 1/8 x Vertikal sync  ( NTSC format )
        fhm :                   in std_logic;       -- Horisontal sync      ( NTSC - 525 linier pr frame )
        f8g :                   in std_logic;       -- 1/4 x Vertikal sync  ( PAL format )
        fhg :                   in std_logic;       -- Horisontal sync      ( PAL - 625 linier pr frame )

--------- til/fra MasterCPLD ------------------------------------------------------------------
        gpbus :                 in std_logic_vector(7 downto 0);
        
--------- til/fra CPU -------------------------------------------------------------------------
        cs :                    in std_logic;
        sck :                   in std_logic;
        mosi :                  in std_logic;
        miso :                  in std_logic;       -- (output not used yet)

--------- til rekonstruktions filter -----------------------------------------------------------
        sync_in :               in std_logic;      -- styrer clamping paa filter output

--------- til THS8135 --------------------------------------------------------------------------
--        dac_clk :               out std_logic;      -- f74i - internt genereret af f148m
--        dac_mode :              in std_logic_vector(1 downto 0); -- pin m2 og m1 pa DAC
--        blank :                 out std_logic;        -- altid lav
        sync :                  inout std_logic;      -- Sync styresignal til konverteren
        sync_t :                inout std_logic;      -- Sync styresignal til konverteren

--        rcr :                   out std_logic_vector(9 downto 0);   -- output for debug (not used yet)
--        gy :                    out std_logic_vector(9 downto 0);    -- not used yet
--        bcb_h :                 out std_logic_vector(5 downto 0); -- inout for debug

        bcb_l :                 in std_logic_vector(3 downto 0)
        );
end Tri_Level_Sync_Generator;


architecture Behavioral of Tri_Level_Sync_Generator is


begin

--sync <= f27m;
--sync_T <= not f27m;

end Behavioral;
