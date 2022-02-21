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
        f1485m :                in std_logic;       -- 148.5 MHz master clock fra ic4
        f14835m :               in std_logic;       -- 148.5/1.001 MHz master clock fra ic2
        f4m :                   in std_logic;       -- 1/8 x Vertikal sync  ( NTSC format )
        fhm :                   in std_logic;       -- Horisontal sync      ( NTSC - 525 linier pr frame )
        f8g :                   in std_logic;       -- 1/4 x Vertikal sync  ( PAL format )
        fhg :                   in std_logic;       -- Horisontal sync      ( PAL - 625 linier pr frame )

--------- til/fra MasterCPLD ------------------------------------------------------------------
        gpbus7 :                 out std_logic;
        gpbus6 :                 out std_logic;
        gpbus5 :                 in std_logic;
        gpbus4 :                 in std_logic;
        gpbus3 :                 out std_logic;
        gpbus2 :                 in std_logic;
        gpbus1 :                 out std_logic;
        gpbus0 :                 in std_logic;
        
--------- til/fra CPU -------------------------------------------------------------------------
        cs :                    in std_logic;
        sck :                   in std_logic;
        mosi :                  in std_logic;
        miso :                  in std_logic;       -- (output not used yet)

--------- til rekonstruktions filter -----------------------------------------------------------
        sync_in :               out std_logic;      -- styrer clamping paa filter output

--------- til THS8135 --------------------------------------------------------------------------
--        dac_clk :               out std_logic;      -- f74i - internt genereret af f1485m eller f14835m
  --      dac_mode :              in std_logic_vector(1 downto 0); -- pin m2 og m1 pa DAC
    --    blank :                 out std_logic;        -- altid lav
--        rcr :                   in std_logic_vector(9 downto 0);   -- output for debug (not used yet)
  --      gy :                    in std_logic_vector(9 downto 0);    -- not used yet
    --    bcb_h :                 in std_logic_vector(5 downto 0); -- inout for debug
        bcb_l :                 out std_logic_vector(3 downto 0);   -- output til "hjemmelavet" DA-konverter
        sync_tp :               inout std_logic;      -- Sync styresignal til konverteren / test pin
        sync_t_tp :             inout std_logic       -- Sync styresignal til konverteren / test pin
        );
end Tri_Level_Sync_Generator;


architecture Behavioral of Tri_Level_Sync_Generator is


component serial_interface
    Port( 
        mreset :            in std_logic;
        cs :                in std_logic;
        sck :               in std_logic;
        mosi :              in std_logic;
        spl_div :           out std_logic_vector(9 downto 0);
        sync_mode :         out std_logic_vector(1 downto 0);
        system_clk :        out std_logic;
        phasedelay :        out std_logic_vector(25 downto 0)
        );
end component;


--------------------------------------------------------------
-- force global clocks :
attribute BUFG: string;
attribute BUFG of f1485m: signal is "CLK";
attribute BUFG of f14835m: signal is "CLK";
-------------------------------------------------------------------------
signal spl_div :    std_logic_vector(9 downto 0);
signal sync_mode :  std_logic_vector(1 downto 0);
signal system_clk : std_logic;
signal phasedelay : std_logic_vector(25 downto 0);


begin


sif : serial_interface port map (
        mreset          => mreset,
        cs              => cs,
        sck             => sck,
        mosi            => mosi,
        spl_div         => spl_div,
        sync_mode       => sync_mode,
        system_clk      => system_clk,
        phasedelay      => phasedelay
        );

-- spl_div
test_spl_div : process(cs)
begin
if cs = '1' then
--    case spl_div is
--        when "0100000111" => -- 0X 107 - Format ID = 3
  --          sync_tp <= '1';
    --    when "0011011011" => -- 0X 0DB - Format ID = 1, 2
      --      sync_t_tp <= '1';

--        when "0110001111" => -- 0X 18F - Format ID = 4, 5, 12, 13
  --          sync_tp <= '1';
    --    when "0111011111" => -- 0X 1DF - Format ID = 6
      --      sync_t_tp <= '1';

--        when "0110110111" => -- 0X 1B7  - Format ID = 7, 8
  --          sync_tp <= '1';
    --    when "1000001111" => -- 0X 20F  - Format ID = 9
      --      sync_t_tp <= '1';

--        when "1000100101" => -- 0X 225  - Format ID = 10, 11
  --          sync_tp <= '1';
    --    when "0101001001" => -- 0X 149  - Format ID = 14, 15
      --      sync_t_tp <= '1';

--        when "0110001011" => -- 0X 18B  - Format ID = 16
  --          sync_tp <= '1';
    --    when "1010010011" => -- 0X 293 - Format ID = 17, 18
      --      sync_t_tp <= '1';

--        when "1100010111" => -- 0X 317 - Format ID = 19
  --          sync_tp <= '1';
    --    when "1100111000" => -- 0X 338 - Format ID = 20, 21
      --      sync_t_tp <= '1';

--        when others =>
  --          sync_tp <= '0';
    --        sync_t_tp <= '0';
--    end case;
end if;
end process;

--sync_tp <= sync_mode(1);
--sync_t_tp <= sync_mode(0);

sync_tp <= system_clk;
sync_t_tp <= sck;


end Behavioral;
