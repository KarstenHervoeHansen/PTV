library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity serial_interface is
    Port( 
        mreset :        in std_logic;   -- master reset - fra mainframe
        cs :            in std_logic;   -- chip select master CPLD
        sck :           in std_logic;   -- Serial Clock
        mosi :          in std_logic;   -- Master Out Serial In
        lpf_tsg1 :      out integer range 0 to 2047;
        sm_tsg1 :       out std_logic;
        lpf_tsg2 :      out integer range 0 to 2047;
        sm_tsg2 :       out std_logic;
        lpf_tsg3 :      out integer range 0 to 2047;
        sm_tsg3 :       out std_logic;
        -- debug :
        lpf_ok :        out std_logic;
        sm_ok :         out std_logic
        );
end serial_interface;

architecture Behavioral of serial_interface is

signal buf_ptr :        std_logic_vector(3 downto 0);
signal format_buffer :  std_logic_vector(11 downto 0);
signal output_id :      std_logic_vector(1 downto 0);
signal format_read :    std_logic;

-- debug :
--signal lpf_compare :    std_logic_vector(10 downto 0);
--signal sm_compare :     std_logic;

begin

make_bufptr : process( cs, sck )
begin
    if cs = '1' then
        buf_ptr <= "1111";
        format_read <= '0';
    elsif sck'event and sck = '1' then

        buf_ptr(0) <= not buf_ptr(0);

        if buf_ptr(0) = '1' then
            buf_ptr(1) <= not buf_ptr(1);
        end if;

        if buf_ptr(1 downto 0) = "11" then
            buf_ptr(2) <= not buf_ptr(2);
        end if;

        if buf_ptr(2 downto 0) = "111" then
            buf_ptr(3) <= not buf_ptr(3);
        end if;

        if buf_ptr = "1101" then
            format_read <= '1';
        else
            format_read <= '0';
        end if;
            
    end if;            
end process;


getdata : process( cs, sck )
begin
    if cs = '1' then
--        scanmethod <= '0';
--        linesprframe <= "00000000000";
    elsif sck'event and sck = '0' then
        case buf_ptr is
            when "0000" =>
                output_id(1) <= mosi;
            when "0001" =>
                output_id(0) <= mosi;
            when "0010" =>
                -- for at spare makroceller evt :
                case output_id is
                    when "01" =>
                        null;
                    when "01" =>
                        null;
                    when "01" =>
                        null;
                    when others =>
                        null;
                end case;
                format_buffer(11) <= mosi;
            when "0011" =>
                format_buffer(10) <= mosi;
            when "0100" =>
                format_buffer(9) <= mosi;
            when "0101" =>
                format_buffer(8) <= mosi;
            when "0110" =>
                format_buffer(7) <= mosi;
            when "0111" =>
                format_buffer(6) <= mosi;
            when "1000" =>
                format_buffer(5) <= mosi;
            when "1001" =>
                format_buffer(4) <= mosi;
            when "1010" =>
                format_buffer(3) <= mosi;
            when "1011" =>
                format_buffer(2) <= mosi;
            when "1100" =>
                format_buffer(1) <= mosi;
            when "1101" =>
                format_buffer(0) <= mosi;
            when others =>
                null;
        end case;
    end if;        
end process;

write_data : process( mreset, format_read )
begin
    if mreset = '0' then
        null;
    elsif format_read'event and format_read = '1' then
        case output_id is
            when "01" =>
                lpf_tsg1 <= conv_integer(format_buffer(11 downto 1));
                sm_tsg1 <= format_buffer(0);
            when "10" =>
                lpf_tsg2 <= conv_integer(format_buffer(11 downto 1));
                sm_tsg2 <= format_buffer(0);
            when "11" =>
                lpf_tsg3 <= conv_integer(format_buffer(11 downto 1));
                sm_tsg3 <= format_buffer(0);
            when others =>
                null;
        end case;
    end if;
end process;


-- 1920x1080/60/I
-- 1920x1080/59.94/I
-- 1920x1080/50/I
-- 1920x1035/60/I
-- 1920x1035/59.94/I
--lpf_compare <= "10001100100";
--sm_compare <= '0';

--comparedata : process( cs )
--begin
  --  if cs'event and cs = '1' then
    --    if linesprframe = lpf_compare then
      --      lpf_ok <= '1';
        --else
          --  lpf_ok <= '0';
--        end if;

--        if scanmethod = sm_compare then
  --          sm_ok <= '1';
    --    else
      --      sm_ok <= '0';
        --end if;
--    end if;
--end process;



end Behavioral;
