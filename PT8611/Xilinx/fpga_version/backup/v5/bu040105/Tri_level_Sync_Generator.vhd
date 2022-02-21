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
--                                                               + opsplitning af software
-- v5 :	    23/12 2003 - PS     nyt printudlæg m Spartan3 FPGA
-----------------------------------------------------------------------------------------------

entity Tri_Level_Sync_Generator is
   Port ( 
--------- til/fra Mainframe -------------------------------------------------------------------
        mreset :    in std_logic;       -- Master Reset fra mainboard
        f27 :       in std_logic;       -- 27 MHz masterclock fra mainboard
        f1484 :     in std_logic;       -- alternativ indgang for 148.35 MHz clock - ret i UCF file
        f1485 :     in std_logic;       -- alternativ indgang for 148.5 MHz clock - ret i UCF file
        f4m :       in std_logic;       -- 1/4 x Vertikal sync - 625 lines SD format
--        fhm :       in std_logic;       -- Horisontal sync
        f8g :       in std_logic;       -- 1/8 x Vertikal sync - 525 lines SD format
--        fhg :       in std_logic;       -- Horisontal sync
        res1 :      out std_logic;      -- tsg outputs OK respons 1
        res2 :      out std_logic;      -- respons 2 ( not used - tied to vcc )
      
--------- til/fra CPU -------------------------------------------------------------------------
        sck :       in std_logic;       -- serial clock
        mosi :      in std_logic;       -- Master Out Serial data In
        cs1 :       in std_logic;       -- chip select tsg1 (nss)
        cs2 :       in std_logic;       -- chip select tsg2 (port2 bit 5)
        cs3 :       in std_logic;       -- chip select tsg3 (port 1 bit 7)
--        miso :      in std_logic;       -- not used
--        p0 :        in std_logic_vector(7 downto 2); -- port 0
--        p1 :        in std_logic_vector(6 downto 0); -- port 1

--------- til rekonstruktions filtre -----------------------------------------------------------
        clmp1 :      out std_logic;      -- til clamping puls
        clmp2 :      out std_logic;
        clmp3 :      out std_logic;
--        clmp4 :      out std_logic;

--------- TSG outputs --------------------------------------------------------------------------
        tsg1_out :      out std_logic_vector(3 downto 0);   -- 4 bit til DA converter
        tsg2_out :      out std_logic_vector(3 downto 0);
        tsg3_out :      out std_logic_vector(3 downto 0);
--        tsg_out4 :      out std_logic_vector(3 downto 0);

--------- Output level check -------------------------------------------------------------------
        tsg1_lvl :       in std_logic;                       -- signaler fra komparatorer
        tsg2_lvl :       in std_logic;
        tsg3_lvl :       in std_logic;
    --    tsg4_lvl :       in std_logic;

--------- til THS8135 --------------------------------------------------------------------------
-- ( gy = [tsg2(1 downto 0); tsg3(3 downto 0); tsg4(3 downto 0)] )
--        dacm1 :   out std_logic;                        -- mode pin 1
  --      dacm2 :   out std_logic;                        -- mode pin 2
--        rcr :     in std_logic_vector(7 downto 0);      -- ( 2 MSB = [tsg13; tsg12] )
  --      bcr :     in std_logic_vector(9 downto 0);      

--------- board interconnect -------------------------------------------------------------------
-- selx = clock select f743 OR f742 for tsg number x
-- stx = sync_t tsg number x
-- syx = sync tsg number x
        sel0 :      out std_logic;
        st0 :       out std_logic;
        sy0 :       out std_logic;
        sel1 :      out std_logic; 
        st1 :       out std_logic;
        sy1 :       out std_logic;
        sel2 :      out std_logic; 
        st2 :       out std_logic;
        sy2 :       out std_logic;
        sel3 :      out std_logic; 
        st3 :       out std_logic;
        sy3 :       out std_logic;
        sel4 :      out std_logic; 
        st4 :       out std_logic;
        sy4 :       out std_logic;
        sel5 :      out std_logic; 
        st5 :       out std_logic;
        sy5 :       out std_logic;
        sel6 :      out std_logic; 
        st6 :       out std_logic;
        sy6 :       out std_logic;
        sel7 :      out std_logic; 
        st7 :       out std_logic;
        sy7 :       out std_logic;
        f742 :      out std_logic;
        f743 :      out std_logic;

--------- testpoints ( D2, D3, D4 LEDs ) -------------------------------------------------------
        led1 :      out std_logic;  -- forbundet til en noconnect på xc3s50
        led2 :      out std_logic;  -- forbundet til en noconnect på xc3s50
        led3 :      inout std_logic

        -- NBNB : alle udkommaterede signaler er ogsaa udkommateret i UCF filen
        );
end Tri_Level_Sync_Generator;


architecture Behavioral of Tri_Level_Sync_Generator is


signal tsg1_ok :        std_logic;  -- low pulse when level error
signal tsg2_ok :        std_logic;
signal tsg3_ok :        std_logic;
-- debug signals :
signal tp1 :            std_logic_vector(7 downto 0);   -- test vektorer
signal tp2 :            std_logic_vector(7 downto 0);
signal tp3 :            std_logic_vector(7 downto 0);


component syncgenerator1
    Port(
        -- fra mainframe : ------------------------------------
        mreset :        in std_logic;       -- Master Reset fra mainboard
        f27 :           in std_logic;       -- 27 MHz clock fra mainboard
        f1484 :         in std_logic;       -- 148.35 MHz clock
        f1485 :         in std_logic;       -- 148.5 MHz clock
        f4m :           in std_logic;       -- 1/4 x Vertikal sync - 625 lines format
        f8g :           in std_logic;       -- 1/8 x Vertikal sync - 525 lines format
        -- til/fra cpu : --------------------------------------
        sck :           in std_logic;
        mosi :          in std_logic;
        cs1 :           in std_logic;
        -- ouput check til changeover unit : ------------------
        tsg1_lvl :      in std_logic;
        tsg1_ok :       out std_logic;
        -- Generator output : ---------------------------------
        tsg1_out :      out std_logic_vector(3 downto 0);
        -- testpoints : ---------------------------------------
        tp1 :           out std_logic_vector(7 downto 0)
        );
end component;

component syncgenerator2
    Port(
        -- fra mainframe : ------------------------------------
        mreset :        in std_logic;       -- Master Reset fra mainboard
        f27 :           in std_logic;       -- 27 MHz clock fra mainboard
        f1484 :         in std_logic;       -- 148.35 MHz clock
        f1485 :         in std_logic;       -- 148.5 MHz clock
        f4m :           in std_logic;       -- 1/4 x Vertikal sync - 625 lines format
        f8g :           in std_logic;       -- 1/8 x Vertikal sync - 525 lines format
        -- til/fra cpu : --------------------------------------
        sck :           in std_logic;
        mosi :          in std_logic;
        cs2 :           in std_logic;
        -- ouput check til changeover unit : ------------------
        tsg2_lvl :      in std_logic;
        tsg2_ok :       out std_logic;
        -- Generator output : ---------------------------------
        tsg2_out :      out std_logic_vector(3 downto 0);
        -- testpoints : ---------------------------------------
        tp2 :           out std_logic_vector(7 downto 0)
        );
end component;

component syncgenerator3
    Port(
        -- fra mainframe : ------------------------------------
        mreset :        in std_logic;       -- Master Reset fra mainboard
        f27 :           in std_logic;       -- 27 MHz clock fra mainboard
        f1484 :         in std_logic;       -- 148.35 MHz clock
        f1485 :         in std_logic;       -- 148.5 MHz clock
        f4m :           in std_logic;       -- 1/4 x Vertikal sync - 625 lines format
        f8g :           in std_logic;       -- 1/8 x Vertikal sync - 525 lines format
        -- til/fra cpu : --------------------------------------
        sck :           in std_logic;
        mosi :          in std_logic;
        cs3 :           in std_logic;
        -- ouput check til changeover unit : ------------------
        tsg3_lvl :      in std_logic;
        tsg3_ok :       out std_logic;
        -- Generator output : ---------------------------------
        tsg3_out :      out std_logic_vector(3 downto 0);
        -- testpoints : ---------------------------------------
        tp3 :           out std_logic_vector(7 downto 0)
        );
end component;


begin


trilevel_syncgenerator1 : syncgenerator1 port map (
        mreset          => mreset,
        f27             => f27,
        f1484           => f1484,
        f1485           => f1485,
        f4m             => f4m,
        f8g             => f8g,
        sck             => sck,
        mosi            => mosi,
        cs1             => cs1,
        tsg1_lvl        => tsg1_lvl,
        tsg1_ok         => tsg1_ok,
        tsg1_out        => tsg1_out,
        -- debugging signals and testpoints :
        tp1             => tp1
        );

trilevel_syncgenerator2 : syncgenerator2 port map (
        mreset          => mreset,
        f27             => f27,
        f1484           => f1484,
        f1485           => f1485,
        f4m             => f4m,
        f8g             => f8g,
        sck             => sck,
        mosi            => mosi,
        cs2             => cs2,
        tsg2_lvl        => tsg2_lvl,
        tsg2_ok         => tsg2_ok,
        tsg2_out        => tsg2_out,
        -- debugging signals and testpoints :
        tp2             => tp2
        );

trilevel_syncgenerator3 : syncgenerator3 port map (
        mreset          => mreset,
        f27             => f27,
        f1484           => f1484,
        f1485           => f1485,
        f4m             => f4m,
        f8g             => f8g,
        sck             => sck,
        mosi            => mosi,
        cs3             => cs3,
        tsg3_lvl        => tsg3_lvl,
        tsg3_ok         => tsg3_ok,
        tsg3_out        => tsg3_out,
        -- debugging signals and testpoints :
        tp3             => tp3
        );


--------------------------------------------------------------
-- level check - svar til mainframe :
res1 <= '1';--tsg1_ok and tsg2_ok and tsg3_ok;
res2 <= '1';

-- debug :
f742 <= tsg1_ok and tsg2_ok and tsg3_ok;

--------------------------------------------------------------
-- clamp indgang til rekonstruktions filtre :
clmp1 <= '0';
clmp2 <= '0';
clmp3 <= '0';


--------------------------------------------------------------
-- testpins :
led3 <= f4m;
led2 <= mreset;
led1 <= f8g;


sy7 <= tp1(7);
sy6 <= tp1(6);
sy5 <= tp1(5);
sy4 <= tp1(4);
sy3 <= tp1(3);
sy2 <= tp1(2);
sy1 <= tp1(1);
sy0 <= tp1(0);

st7 <= tp2(7);
st6 <= tp2(6);
st5 <= tp2(5);
st4 <= tp2(4);
st3 <= tp2(3);
st2 <= tp2(2);
st1 <= tp2(1);
st0 <= tp2(0);

sel7 <= tp3(7);
sel6 <= tp3(6);
sel5 <= tp3(5);
sel4 <= tp3(4);
sel3 <= tp3(3);
sel2 <= tp3(2);
sel1 <= tp3(1);
sel0 <= tp3(0);


end Behavioral;