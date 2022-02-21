library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity serial_interface is
    Port( 
        tsg_reset :             in std_logic;
        cs :                    in std_logic;
        sck :                   in std_logic;
        mosi :                  in std_logic;
        miso :                  in std_logic;       -- (output not used yet - configured as input)

        phasedelay :            out std_logic_vector(25 downto 0);  -- samples x lines x frames
        scan_method :           out std_logic;                      -- ( 0=interlaced, 1=progressiv )
        sync_mode :             out std_logic_vector(1 downto 0);   -- 0X=freerunning, 10=genlocked to f4m, 11= genlocked to f8g
        samples_pr_line :       out integer range 0 to 16383;
        lines_pr_frame :        out integer range 0 to 4095;
        system_clk :            out std_logic
        );
end serial_interface;

architecture Behavioral of serial_interface is

----------------------------------------------------------------------------------------
--signal samples_pr_line_reg :    std_logic_vector(13 downto 0);
--signal lines_pr_frame_reg :     std_logic_vector(10 downto 0);
signal output_id :              std_logic_vector(1 downto 0);
signal id_read :                std_logic_vector(1 downto 0);
signal bit_address :            integer range 0 to 63;
signal cs_int :                 std_logic;

begin

---------------------------------------------------------------
-- REDUCED VERSION WITH HARDCODED FORMATBITS :
---------------------------------------------------------------
-- 749 or 1124 lines_pr_frame => bit 0,3,7,9,10 differs
-- bit 1,2,4,5,6,8 don't differ :
--lines_pr_frame_reg(1) <= '0';
--lines_pr_frame_reg(2) <= '1';
--lines_pr_frame_reg(4) <= '0';
--lines_pr_frame_reg(5) <= '1';
--lines_pr_frame_reg(6) <= '1';
--lines_pr_frame_reg(8) <= '0';
---------------------------------------------------------------
-- set output_id :
output_id <= "01";
--------------------------------------------------------------
-- make intern chip select :
with id_read select
    cs_int <= '0' when "01",
              '1' when others;

id_fetch : process( cs, sck, output_id(0) )
begin
    if cs = '1' then
        id_read(0) <= not output_id(0); -- sikrer at formatlaesningen ikke gaar igang
    elsif sck'event and sck = '0' then
        if bit_address = 1 then
            id_read(1) <= mosi;
        elsif bit_address = 2 then
            id_read(0) <= mosi;
        else
            null;
        end if;
    end if;
end process;


bit_counter : process( cs_int, sck )
begin
    if cs_int = '1' then
        bit_address <= 0;
    elsif sck'event and sck = '0' then
        bit_address <= bit_address + 1;
    end if;
end process;


data_fetch : process( cs_int, sck )
begin
    if cs_int = '1' then
        phasedelay <= "00000000000000000000000000";
    elsif sck'event and sck = '0' then
        case bit_address is
            when 3 =>
                phasedelay(25) <= mosi;
            when 4 =>
                phasedelay(24) <= mosi;
            when 5 =>
                phasedelay(23) <= mosi;
            when 6 =>
                phasedelay(22) <= mosi;
            when 7 =>
                phasedelay(21) <= mosi;
            when 8 =>
                phasedelay(20) <= mosi;
            when 9 =>
                phasedelay(19) <= mosi;
            when 10 =>
                phasedelay(18) <= mosi;
            when 11 =>
                phasedelay(17) <= mosi;
            when 12 =>
                phasedelay(16) <= mosi;
            when 13 =>
                phasedelay(15) <= mosi;
            when 14 =>
                phasedelay(14) <= mosi;
            when 15 =>
                phasedelay(13) <= mosi;
            when 16 =>
                phasedelay(12) <= mosi;
            when 17 =>
                phasedelay(11) <= mosi;
            when 18 =>
                phasedelay(10) <= mosi;
            when 19 =>
                phasedelay(9) <= mosi;
            when 20 =>
                phasedelay(8) <= mosi;
            when 21 =>
                phasedelay(7) <= mosi;
            when 22 =>
                phasedelay(6) <= mosi;
            when 23 =>
                phasedelay(5) <= mosi;
            when 24 =>
                phasedelay(4) <= mosi;
            when 25 =>
                phasedelay(3) <= mosi;
            when 26 =>
                phasedelay(2) <= mosi;
            when 27 =>
                phasedelay(1) <= mosi;
            when 28 =>
                phasedelay(0) <= mosi;
            when others =>
                null;
        end case;
    end if;        
end process;

--phasedelay <= "00000000000000000000000000";
--scan_method <= '0';
--sync_mode <= "00";
--samples_pr_line <= 4399;
--lines_pr_frame_reg(0) <= '0';
--lines_pr_frame_reg(3) <= '0';
--lines_pr_frame_reg(7) <= '0';
--lines_pr_frame_reg(9) <= '0';
--lines_pr_frame_reg(10) <= '1';
--lines_pr_frame <= conv_integer(lines_pr_frame_reg);
--system_clk <= '0';

-- debug :
--phasedelay <= "00000000000000000000000000";
lines_pr_frame <= 1124;
samples_pr_line <= 4399;
scan_method <= '0';
sync_mode <= "11";
--phasedelay <= "00000000000000000000000000";

end Behavioral;









