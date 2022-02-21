library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity parallel_interface is
    Port(
        tsg_reset :             in std_logic;
        cs :                    in std_logic;
        sck :                   in std_logic;
        gpbus :                 in std_logic_vector(7 downto 0);

        phasedelay :            out std_logic_vector(25 downto 0);  -- samples x lines x frames
        scan_method :           out std_logic;                      -- ( 0=interlaced, 1=progressiv )
        sync_mode :             out std_logic_vector(1 downto 0);   -- 0X=freerunning, 10=genlocked to f4m, 11= genlocked to f8g
        samples_pr_line :       out integer range 0 to 16383;
        lines_pr_frame :        out integer range 0 to 4095;
        system_clk :            out std_logic
        );
end parallel_interface;

architecture Behavioral of parallel_interface is

signal which_byte :         integer range 0 to 7;
signal read_id :            std_logic;
signal this_cpld :          std_logic;

begin

make_which_byte : process( cs, sck, gpbus )
begin
    if cs = '1' then
        which_byte <= 0;
    elsif sck'event and sck = '1' then
        which_byte <= which_byte + 1;
    end if;        
end process;

with which_byte select
    read_id <= '1' when 1,
               '0' when others;

read_data : process( tsg_reset, sck, read_id )
begin
    if tsg_reset = '0' then
        this_cpld <= '0';
        phasedelay <= "00000000000000000000001111";
--        scan_method <= '0';
  --      sync_mode <= "00";
    --    system_clk <= '0';
    elsif sck'event and sck = '0' and cs = '0' then
        if ( read_id = '1' ) and (( gpbus(7) = '0' ) and ( gpbus(6) = '1' )) then
            this_cpld <= '1';
            phasedelay(25 downto 20) <= gpbus(5 downto 0);
        end if;
        if this_cpld = '1' then
            case which_byte is
                when 2 =>
                    phasedelay(19 downto 12) <= gpbus(7 downto 0);
                when 3 =>
                    phasedelay(11 downto 4) <= gpbus(7 downto 0);
                when 4 =>
                    phasedelay(3 downto 0) <= gpbus(7 downto 4);
--                    scan_method <= gpbus(3);
  --                  sync_mode <= gpbus(2 downto 1);
    --                system_clk <= gpbus(0);
--                when 5 =>
                    
                when others =>
                    this_cpld <= '0';
            end case;
        end if;                    
    end if;
end process;

scan_method <= '0';
sync_mode <= "10";
samples_pr_line <= 4399;
lines_pr_frame <= 1124;
system_clk <= '0';

end Behavioral;
