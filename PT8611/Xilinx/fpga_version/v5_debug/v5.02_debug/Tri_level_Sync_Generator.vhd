library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
library UNISIM;
use UNISIM.VComponents.all;

-----------------------------------------------------------------------------------------------
-- v1:      Original kode til Prototype lavet af PEH + studerende Jesper Christoffersen 
--          for DK-Audio, som afgangsprojekt november 2002 til marts 2003
-- v2:      19/6 2003 - PS      tilfoejet clamping- og enablesignaler til rekonstruktionsfiltre
-- v3:      23/5 2003 - PS      nyt hieraki => toplevel = Tri_level_sync_generator.vhd
-- v301 :   21/8 2003 - PS      ny komponent sync_statemachine og state_timer
-- v4 :     8/9  2003 - PS      nyt printudlaeg m 100pin CPLD => ny pin-konfiguration
--                                                               + opsplitning af software
-- v5 :	    23/12 2003 - PS     nyt printudlæg m Spartan3 FPGA
-- v501 :   21/04 2004 - ps     ny sync_statemachine ( pulsetype 1 og 2 )
-- v502 :   26/01 2005 - PS     ny komponent freerunning_reference, der genererer intern 
--                              vertikal referencesignal til 23.98, 24, 30 & 60Hz formater
-----------------------------------------------------------------------------------------------

entity Tri_Level_Sync_Generator is
   Port ( 
--------- til/fra Mainframe -------------------------------------------------------------------
        mreset :    in std_logic;       -- Master Reset fra mainboard
        f27 :       in std_logic;       -- 27 MHz masterclock fra mainboard
        f1484 :     in std_logic;       -- 148.35 MHz clock
        f1485 :     in std_logic;       -- 148.5 MHz clock
        f4m :       in std_logic;       -- Vertikal sync reference - 625 lines SD format
--        fhm :       in std_logic;       -- Horisontal sync
        f8g :       in std_logic;       -- Vertikal sync reference - 525 lines SD format
--        fhg :       in std_logic;       -- Horisontal sync
        res1 :      out std_logic;      -- generator 1&2 output levels OK - respons til mainframe
        res2 :      out std_logic;      -- generator 3&4 output levels OK - respons til mainframe
      
--------- til/fra CPU -------------------------------------------------------------------------
        sck :       in std_logic;       -- serial interface clock
        mosi :      in std_logic;       -- Master Out Serial data In
        cs1 :       in std_logic;       -- chip select tsg1 (nss)
        cs2 :       in std_logic;       -- chip select tsg2 (port 2 bit 5)
        cs3 :       in std_logic;       -- chip select tsg3 (port 1 bit 7)
        cs4 :       in std_logic;       -- chip select tsg4 (port 1 bit 6)
--        miso :      in std_logic;       -- not used
        p0 :        in std_logic_vector(7 downto 2); -- port 0 [7:2]
        p1 :        in std_logic_vector(5 downto 0); -- port 1 [5:0]

--------- til rekonstruktions filtre -----------------------------------------------------------
        clmp1 :     out std_logic;      -- til clamping puls - benyttes ikke 
        clmp2 :     out std_logic;
        clmp3 :     out std_logic;
        clmp4 :     out std_logic;

--------- TSG outputs --------------------------------------------------------------------------
        tsg1_out :  out std_logic_vector(3 downto 0);   -- 4 bit til DA converter
        tsg2_out :  out std_logic_vector(3 downto 0);
        tsg3_out :  out std_logic_vector(3 downto 0);
        tsg4_out :  out std_logic_vector(3 downto 0);

--------- Output level check -------------------------------------------------------------------
        tsg1_lvl :  in std_logic;                       -- signaler fra komparatorer
        tsg2_lvl :  in std_logic;
        tsg3_lvl :  in std_logic;
        tsg4_lvl :  in std_logic;

--------- til THS8135 --------------------------------------------------------------------------
-- ( gy = [tsg2(1 downto 0); tsg3(3 downto 0); tsg4(3 downto 0)] )
--        dacm1 :     out std_logic;                        -- mode pin 1
  --      dacm2 :     out std_logic;                        -- mode pin 2
--        rcr :       in std_logic_vector(7 downto 0);      -- ( 2 MSB = [tsg13; tsg12] )
  --      bcr :       in std_logic_vector(9 downto 0);      

--------- board interconnect ( J4 & J5 ) -------------------------------------------------------
        ext1_out :  out std_logic_vector(3 downto 0);
        ext2_out :  out std_logic_vector(3 downto 0);
        ext3_out :  out std_logic_vector(3 downto 0);
        ext4_out :  out std_logic_vector(3 downto 0);
        spare :     out std_logic_vector(7 downto 0);

--------- testpoints ( D2, D3, D4 LEDs ) -------------------------------------------------------
        led1 :      out std_logic;  -- forbundet til en noconnect på xc3s50
        led2 :      out std_logic;  -- forbundet til en noconnect på xc3s50
        led3 :      inout std_logic

        -- NBNB : alle udkommaterede signaler er ogsaa udkommateret i UCF filen
        );
end Tri_Level_Sync_Generator;


architecture Behavioral of Tri_Level_Sync_Generator is


signal tsg1_ok,tsg2_ok,tsg3_ok,tsg4_ok :    std_logic;  -- low pulse when level error
signal ref2398,ref24,ref30 :                std_logic;
signal clk1_sel,clk2_sel,not_clk2_sel :     std_logic;
signal clk3_sel,clk4_sel,not_clk4_sel :     std_logic;
signal f148g1,f148g2,f148g3,f148g4 :        std_logic;
attribute clock_signal :                    string;
attribute clock_signal of f148g4 : signal is "yes";
attribute clock_signal of f148g1 : signal is "yes";
attribute clock_signal of f148g2 : signal is "yes";
attribute clock_signal of f148g3 : signal is "yes";

-- debug signals :
signal tp1,tp2,tp3,tp4 :    std_logic_vector(7 downto 0);   -- test vektorer
signal updown :             std_logic;
signal which_led :          integer range 0 to 3;
signal flashcount :         integer range 0 to 7;

component syncgenerator
    Port(
        -- fra mainframe : --------------------------------------------------------------
        mreset :        in std_logic;       -- Master Reset fra mainboard
        f27 :           in std_logic;       -- 27 MHz clock fra mainboard
        f148 :          in std_logic;       -- 148.5 eller 148.35 MHz clock
        f4m :           in std_logic;       -- 1/4 x Vertikal sync - 625 lines format
        f8g :           in std_logic;       -- 1/8 x Vertikal sync - 525 lines format
        sysclk_sel :    inout std_logic;    -- til bufgmux select
        -- vertical sync reference til freerunning formater : ---------------------------
        ref2398 :       in std_logic;
        ref24 :         in std_logic;
        ref30 :         in std_logic;
        -- til/fra cpu : ----------------------------------------------------------------
        sck :           in std_logic;
        mosi :          in std_logic;
        cs :            in std_logic;
        -- ouput check til changeover unit : --------------------------------------------
        tsg_lvl :       in std_logic;
        tsg_ok :        out std_logic;
        -- Generator output : -----------------------------------------------------------
        tsg_out :       out std_logic_vector(3 downto 0);
        -- testpoints : -----------------------------------------------------------------
        tp :            out std_logic_vector(7 downto 0)
        );
end component;

component freerunning_reference
    Port(
        mreset :    in std_logic;
        f27 :       in std_logic;
        ref2398 :   out std_logic;
        ref24 :     out std_logic;
        ref30 :     out std_logic
        );
end component;

component bufgmux
    Port (
        I0 :    in std_logic;
        I1 :    in std_logic;
        S :   	in std_logic;
        O :     out std_logic
        );
end component;

begin

trilevel_syncgenerator1 : syncgenerator port map (
        mreset          => mreset,
        f27             => f27,
        f148            => f148g1,
        f4m             => f4m,
        f8g             => f8g,
        sysclk_sel      => clk1_sel,
        ref2398         => ref2398,
        ref24           => ref24,
        ref30           => ref30,
        sck             => sck,
        mosi            => mosi,
        cs              => cs1,
        tsg_lvl         => tsg1_lvl,
        tsg_ok          => tsg1_ok,
        tsg_out         => tsg1_out,
        -- debugging signals and testpoints :
        tp              => tp1 );

trilevel_syncgenerator2 : syncgenerator port map (
        mreset          => mreset,
        f27             => f27,
        f148            => f148g2,
        f4m             => f4m,
        f8g             => f8g,
        sysclk_sel      => clk2_sel,
        ref2398         => ref2398,
        ref24           => ref24,
        ref30           => ref30,
        sck             => sck,
        mosi            => mosi,
        cs              => cs2,
        tsg_lvl         => tsg2_lvl,
        tsg_ok          => tsg2_ok,
        tsg_out         => tsg2_out,
        -- debugging signals and testpoints :
        tp              => tp2 );

trilevel_syncgenerator3 : syncgenerator port map (
        mreset          => mreset,
        f27             => f27,
        f148            => f148g3,
        f4m             => f4m,
        f8g             => f8g,
        sysclk_sel      => clk3_sel,
        ref2398         => ref2398,
        ref24           => ref24,
        ref30           => ref30,
        sck             => sck,
        mosi            => mosi,
        cs              => cs3,
        tsg_lvl         => tsg3_lvl,
        tsg_ok          => tsg3_ok,
        tsg_out         => tsg3_out,
        -- debugging signals and testpoints :
        tp              => tp3 );

trilevel_syncgenerator4 : syncgenerator port map (
        mreset          => mreset,
        f27             => f27,
        f148            => f148g4,
        f4m             => f4m,
        f8g             => f8g,
        sysclk_sel      => clk4_sel,
        ref2398         => ref2398,
        ref24           => ref24,
        ref30           => ref30,
        sck             => sck,
        mosi            => mosi,
        cs              => cs4,
        tsg_lvl         => tsg4_lvl,
        tsg_ok          => tsg4_ok,
        tsg_out         => tsg4_out,
        -- debugging signals and testpoints :
        tp              => tp4 );

freerunningformat_verticalsync : freerunning_reference Port map(
        mreset  => mreset,
        f27     => f27,
        ref2398 => ref2398,
        ref24   => ref24,
        ref30   => ref30 );

globalclock_tsg1 : bufgmux port map(
        I0 => f1485, 
        I1 => f1484, 
        S => clk1_sel,
        O => f148g1 );

globalclock_tsg2 : bufgmux port map(
        I0 => f1484, 
        I1 => f1485, 
        S => not_clk2_sel,
        O => f148g2 );

globalclock_tsg3 : bufgmux port map(
        I0 => f1485, 
        I1 => f1484, 
        S => clk3_sel,
        O => f148g3 );

globalclock_tsg4 : bufgmux port map(
        I0 => f1484, 
        I1 => f1485, 
        S => not_clk4_sel,
        O => f148g4 );

--------------------------------------------------------------
-- select til bufgmux 2 og 4 inverteres, pga delte ressourcer
-- på global clock linien. Designet kan ikke routes, hvis 
-- det ikke gøres på denne måde!!
not_clk2_sel <= not clk2_sel;
not_clk4_sel <= not clk4_sel;

--------------------------------------------------------------
-- level check - svar til mainframe :
res1 <= tsg1_ok and tsg2_ok;
res2 <= tsg3_ok and tsg4_ok;

--------------------------------------------------------------
-- clamp indgang til rekonstruktions filtre :
clmp1 <= '0';
clmp2 <= '0';
clmp3 <= '0';
clmp4 <= '0';

--------------------------------------------------------------
-- initialiseringssekvens ved master reset :
flashleds_on_startup : process(mreset, f8g)
begin
    if mreset = '0' then
        flashcount <= 0;
        which_led <= 3;
    elsif f8g'event and f8g = '1' then
        if flashcount /= 7 then
            flashcount <= flashcount + 1;
            if updown = '1' then
                which_led <= which_led + 1;
            else
                which_led <= which_led - 1;
            end if;
        else
            which_led <= 3;
        end if;
    end if;
end process;

make_updown : process(mreset, f8g)
begin
    if mreset = '0' then
        updown <= '1';
    elsif f8g'event and f8g = '0' then
        if which_led = 2 then
            updown <= '0';
        elsif which_led = 0 then
            updown <= '1';
        else
            null;
        end if;
    end if;
end process;

            
with which_led select
    led1 <= '1' when 0,
            '0' when others;

with which_led select
    led2 <= '1' when 1,
            '0' when others;

with which_led select
    led3 <= '1' when 2,
            '0' when others;

-------------------------------------------------------------
-- set testpoints :
            
ext1_out(3) <= tp1(7);  -- vertical sync TSG1
ext1_out(2) <= tp2(7);  -- vertical sync TSG2
ext1_out(1) <= tp3(7);  -- vertical sync TSG3
ext1_out(0) <= tp4(7);  -- vertical sync TSG4

ext2_out(3) <= tp1(5);  -- genlock_ok sync TSG1
ext2_out(2) <= tp1(2);  -- genlock_ok sync TSG2
ext2_out(1) <= tp1(0);  -- genlock_ok sync TSG3
--ext2_out(0) <= tp4(0);  -- genlock_ok sync TSG4

end Behavioral;