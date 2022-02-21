library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity serial_interface is
    Port(
        mreset :            in std_logic;   -- master reset
        cs :                in std_logic;   -- chip select
        sck :               in std_logic;   -- serial clock
        mosi :              in std_logic;   -- master out serial data in
        spl_div :           inout std_logic_vector(9 downto 0);   -- total samples pr line divided by 10 or 11
        lpf :               out integer range 0 to 2047;        -- total lines per frame
        div10_11 :          out std_logic;                      -- divider f148 med 10 hvis lav, 11 hvis høj
        framerate_x2 :      out std_logic;                      -- signalerer dobbelt framerate
        sync_mode :         out std_logic_vector(1 downto 0);   -- freerunning, sync ref = f4m or sync ref = f8g
        scan_method :       inout std_logic;
        sysclk_sel :        out std_logic;                      -- f148 = 148.35 MHz or 148.5 MHz
        phasedelay :        out std_logic_vector(23 downto 0);  -- delay from sync reference
        freerun_reset :     out std_logic;                      -- indikerer nyt format i video registrene
        new_delay :     out std_logic;
        -- debug :
        lpfbit :        out std_logic
        );
end serial_interface;


architecture Behavioral of serial_interface is


signal bitptr :         integer range 0 to 63;
signal lpf_msb :        std_logic;  -- total lines per frame bliver afgjort udfra en bit


begin

-----------------------------
-- debug out :
lpfbit <= lpf_msb;
-----------------------------

make_bitptr : process( cs, sck )
-- bit pointer dirigerer mosi til relevant register
begin
    if cs = '1' then
        bitptr <= 0;
    elsif sck'event and sck = '1' then
        bitptr <= bitptr + 1;
    end if;
end process;


-------------------------------------------------------------------------------------------------
--  debug phasedelays :
--  "010010111000011111011100" = 1920x1080/59.94I og 1920x1080/29.97P   (4400*1125-20 = 4949980)
--  "010110101010001100001100" = 1920x1080/50I og 1920x1080/25P         (5280*1125-20 = 5939980)
--  "001001011100001111100100" = 1280x720/59.94P                        (3300*750-20  = 2747980)
--  "001011010101000101111100" = 1280x720/50P                           (3960*750-20  = 2969980)
-------------------------------------------------------------------------------------------------


get_format : process(mreset, cs, sck, bitptr )
begin
    if mreset = '0' then
        phasedelay <= "000000000000000000111000";   -- fast debug delay
--        phasedelay <= "010010111000011111011100";   -- default delay = 4949980
        sysclk_sel <= '1';                          -- system clock = 148,35 MHz
        sync_mode <= "11";                          -- reference = f4m
        spl_div <= "0110001111";                    -- 4400 samples per linie
--        spl_div <= "0011011011";                    -- 2200 samples per linie
--        scan_method <= '1';                         -- interlaced format
        scan_method <= '1';                         -- prog = 0, interlaced = 1
        lpf_msb <= '1';                             -- ll25 linier
    elsif sck'event and sck = '0' then
        if cs = '0' then
            case bitptr is
                when 9 =>
                    phasedelay(23) <= mosi;
                when 10 =>
                    phasedelay(22) <= mosi;
                when 11 =>
                    phasedelay(21) <= mosi;
                when 12 =>
                    phasedelay(20) <= mosi;
                when 13 =>
                    phasedelay(19) <= mosi;
                when 14 =>
                    phasedelay(18) <= mosi;
                when 15 =>
                    phasedelay(17) <= mosi;
                when 16 =>
                    phasedelay(16) <= mosi;
                when 17 =>
                    phasedelay(15) <= mosi;
                when 18 =>
                    phasedelay(14) <= mosi;
                when 19 =>
                    phasedelay(13) <= mosi;
                when 20 =>
                    phasedelay(12) <= mosi;
                when 21 =>
                    phasedelay(11) <= mosi;
                when 22 =>
                    phasedelay(10) <= mosi;
                when 23 =>
                    phasedelay(9) <= mosi;
                when 24 =>
                    phasedelay(8) <= mosi;
                when 25 =>
                    phasedelay(7) <= mosi;
                when 26 =>
                    phasedelay(6) <= mosi;
                when 27 =>
                    phasedelay(5) <= mosi;
                when 28 =>
                    phasedelay(4) <= mosi;
                when 29 =>
                    phasedelay(3) <= mosi;
                when 30 =>
                    phasedelay(2) <= mosi;
                when 31 =>
                    phasedelay(1) <= mosi;
                when 32 =>
                    phasedelay(0) <= mosi;
                when 34 =>
                    sysclk_sel <= mosi; 
                when 35 =>
                    sync_mode(1) <= mosi;
                when 36 =>
                    sync_mode(0) <= mosi;
                when 39 =>
                    spl_div(9) <= mosi;
                when 40 =>
                    spl_div(8) <= mosi;
                when 41 =>
                    spl_div(7) <= mosi;
                when 42 =>
                    spl_div(6) <= mosi;
                when 43 =>
                    spl_div(5) <= mosi;
                when 44 =>
                    spl_div(4) <= mosi;
                when 45 =>
                    spl_div(3) <= mosi;
                when 46 =>
                    spl_div(2) <= mosi;
                when 47 =>
                    spl_div(1) <= mosi;
                when 48 =>
                    spl_div(0) <= mosi;
                when 52 =>
                    scan_method <= mosi;
                when 54 =>
                    -- for at spare logik, kigges der kun paa MSB af LinesprFrame parameteren,
                    -- da der i de understoettede formater kun er mulighed for to vaerdier 
                    -- ( 750 lines eller 1125 )
                    lpf_msb <= mosi;
                when others =>
                    null;
            end case;
        end if;
    end if;        
end process;


with lpf_msb select
    lpf <= 749 when '0',
           1124 when others;

div10_11 <= lpf_msb;
-- div10_11 = 0 : divider f148 med 10 ( 720 liniers format )
-- div10_11 = 1 : divider f148 med 11 ( 1035 og 1080 liniers format )


-- framerate_x2 ( active LOW ) signalerer dobbelt framerate formater :
-- 1920x1080/60P , 1920x1080/59.94P og 1920x1080/50P
framerate_x2 <= ( spl_div(9) or spl_div(5) ) or scan_method or not lpf_msb;


make_new_delay : process ( mreset, sck, cs, bitptr )
-- new_delay ( active high ) signalerer, at der er blevet skrevet til phasedelay registeret
-- ( mere specifikt er der blevet skrevet til MSB af phasedelay naar new_delay gaar high )
-- ved nyt format og ogsaa hvis der kun bliver skrevet til phasedelay
begin
    if mreset = '0' then
        new_delay <= '0';
    elsif sck'event and sck = '0' and cs = '0' then
        case bitptr is
            when 16 =>
                -- new_delay skal saettes efter at phasedelay(23) er skrevet
                -- og foer phasedelay(0) bliver skrevet
                -- ( se genlock_timing - make_reset_mask processen )
                new_delay <= '1';
            when others =>
                new_delay <= '0';
        end case;
    end if;
end process;


-- freerun_reset = reset ved freerunning mode ( 30P, 60P og 60i formater )
-- der resettes, naar der skrives til videoregistrene :
make_freerun_reset : freerun_reset <= cs;


end Behavioral;
