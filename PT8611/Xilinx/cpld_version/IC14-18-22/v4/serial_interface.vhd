library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
-- ciao der
entity serial_interface is
    Port(
        mreset :        in std_logic;   -- master reset
        cs :            in std_logic;   -- chip select
        sck :           in std_logic;   -- serial clock
        mosi :          in std_logic;   -- master out serial (data) in
        spl_div :       out std_logic_vector(9 downto 0);   -- total samples pr line divided by 10 or 11
        sync_mode :     out std_logic_vector(1 downto 0);   -- freerunning, sync ref = f4m or sync ref = f8g
        system_clk :    out std_logic;                      -- f148 = 148.35 MHz or 148.5 MHz
        phasedelay :    out std_logic_vector(25 downto 0)   -- delay from sync reference
        );
end serial_interface;

architecture Behavioral of serial_interface is

signal bitptr : integer range 0 to 63;
signal sm_tmp : std_logic_vector(1 downto 0);

begin

-- ( sync_mode vaerdi i tabel er forkert : 10=00, 00=10, 11=11 og 01=01 ) =>
-- ( MSB = not MSB hvis LSB = 0 )
-- set sync_mode :
sync_mode(1) <= not sm_tmp(1) when sm_tmp(0) = '0' else
                    sm_tmp(1);
sync_mode(0) <= sm_tmp(0);


make_bitptr : process( cs, sck )
-- bit pointer dirigerer mosi til relevant register
begin
    if cs = '1' then
        bitptr <= 0;
    elsif sck'event and sck = '1' then
        bitptr <= bitptr + 1;
    end if;
end process;


get_format : process(mreset, cs, sck, bitptr )
begin
    if mreset = '0' then
        -- 1920x1080/60/I efter master reset :
        phasedelay <= "00100101110000111111001100";
        sm_tmp <= "10";
        system_clk <= '0';
        spl_div <= "0110001111";
    elsif sck'event and sck = '0' and cs = '0' then
        case bitptr is
            when 7 =>
                phasedelay(25) <= mosi;
            when 8 =>
                phasedelay(24) <= mosi;
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
                system_clk <= mosi; 
            when 35 =>
                sm_tmp(1) <= mosi;
            when 36 =>
                sm_tmp(0) <= mosi;
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
            when others =>
                null;
        end case;
    end if;        
end process;

end Behavioral;
