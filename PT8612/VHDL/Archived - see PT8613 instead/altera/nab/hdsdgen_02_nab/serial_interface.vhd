library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity serial_interface is Port(
        mreset :        in std_logic;   -- master reset
        cs :            in std_logic;   -- chip select
        sck :           in std_logic;   -- serial clock
        mosi :          in std_logic;   -- master out serial data in
        offset :    	out std_logic_vector(23 downto 0);  -- delay from sync reference
        new_delay :     out std_logic;
		sd_hd :			out std_logic;						-- LOW = SD, HIGH = HD
		sd_format :		out std_logic;						-- LOW = 625 line system, HIGH = 525 line system
		scan_format :	out std_logic;						-- LOW = progressive, HIGH = interlaced
		hd_clk :		out std_logic;						-- LOW = 74,25 MHz, HIGH = 74,25/1.001 MHz
		hd_format :		out std_logic_vector(3 downto 0);	-- high definition format
		pattern :		out std_logic_vector(7 downto 0);	-- test pattern select
		tp_option :		out std_logic_vector(7 downto 0);	-- test pattern option
		tp :			out std_logic
		);
end serial_interface;


architecture Behavioral of serial_interface is


signal bitptr :         integer range 0 to 63;
signal hd_format_tmp :	std_logic_vector(3 downto 0);

begin

with hd_format_tmp select	-- workaround til præsentation på ITU
	hd_format <= "0000" when "0000",
	             "0001" when "0001",
	             "0010" when "0010",
	             "0011" when "0011",
	             "0100" when "0100",
	             "0101" when "0101",
				 "0110" when others;

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
    if mreset = '1' then
        offset <= "000000000000000000111000";   -- fast debug delay
		sd_hd <= '0';
		sd_format <= '0';
		scan_format <= '0';
		hd_clk <= '0';
		hd_format_tmp <= "0000";	-- 1080i/30-29.97
		pattern <= "00000000";
		tp <= '0';
    elsif sck'event and sck = '0' then
        if cs = '0' then
            case bitptr is
                when 9 =>
                    offset(23) <= mosi;
                when 10 =>
                    offset(22) <= mosi;
                when 11 =>
                    offset(21) <= mosi;
                when 12 =>
                    offset(20) <= mosi;
                when 13 =>
                    offset(19) <= mosi;
                when 14 =>
                    offset(18) <= mosi;
                when 15 =>
                    offset(17) <= mosi;
                when 16 =>
                    offset(16) <= mosi;
                when 17 =>
                    offset(15) <= mosi;
                when 18 =>
                    offset(14) <= mosi;
                when 19 =>
                    offset(13) <= mosi;
                when 20 =>
                    offset(12) <= mosi;
                when 21 =>
                    offset(11) <= mosi;
                when 22 =>
                    offset(10) <= mosi;
                when 23 =>
                    offset(9) <= mosi;
                when 24 =>
                    offset(8) <= mosi;
                when 25 =>
                    offset(7) <= mosi;
                when 26 =>
                    offset(6) <= mosi;
                when 27 =>
                    offset(5) <= mosi;
                when 28 =>
                    offset(4) <= mosi;
                when 29 =>
                    offset(3) <= mosi;
                when 30 =>
                    offset(2) <= mosi;
                when 31 =>
                    offset(1) <= mosi;
                when 32 =>
                    offset(0) <= mosi;
                when 33 =>
					tp <= '1';
					sd_hd <= mosi; 
                when 34 =>
					tp <= '0';
					sd_format <= mosi;
                when 35 =>
                    scan_format <= mosi;
                when 36 =>
	                hd_clk <= mosi;
                when 37 =>
                    hd_format_tmp(3) <= mosi;
                when 38 =>
                    hd_format_tmp(2) <= mosi;
                when 39 =>
                    hd_format_tmp(1) <= mosi;
                when 40 =>
                    hd_format_tmp(0) <= mosi;
                when 41 =>
                    pattern(7) <= mosi;
                when 42 =>
                    pattern(6) <= mosi;
                when 43 =>
                    pattern(5) <= mosi;
                when 44 =>
                    pattern(4) <= mosi;
                when 45 =>
                    pattern(3) <= mosi;
                when 46 =>
                    pattern(2) <= mosi;
                when 47 =>
                    pattern(1) <= mosi;
                when 48 =>
                    pattern(0) <= mosi;
                when 49 =>
                    tp_option(7) <= mosi;
                when 50 =>
                    tp_option(6) <= mosi;
                when 51 =>
                    tp_option(5) <= mosi;
                when 52 =>
                    tp_option(4) <= mosi;
                when 53 =>
                    tp_option(3) <= mosi;
                when 54 =>
                    tp_option(2) <= mosi;
                when 55 =>
                    tp_option(1) <= mosi;
                when 56 =>
                    tp_option(0) <= mosi;
                when others =>
                    null;
            end case;
        end if;
    end if;        
end process;

make_new_delay : process ( mreset, sck, cs, bitptr )
-- new_delay ( active high ) signalerer, at der er blevet skrevet til offset registeret
begin
    if mreset = '0' then
        new_delay <= '0';
    elsif sck'event and sck = '0' and cs = '0' then
        case bitptr is
            when 16 =>
                -- new_delay skal saettes efter at offset(23) er skrevet
                -- og foer offset(0) bliver skrevet
                -- ( se genlock_timing - make_reset_mask processen )
                new_delay <= '1';
            when others =>
                new_delay <= '0';
        end case;
    end if;
end process;

end Behavioral;