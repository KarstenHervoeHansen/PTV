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
        lpf_tsg1 :      out integer range 0 to 2047;    -- lines_pr_frame TSG1
        sm_tsg1 :       out std_logic;                  -- scan_method    -"-
        lpf_tsg2 :      out integer range 0 to 2047;    -- lines_pr_frame TSG2
        sm_tsg2 :       out std_logic;                  -- scan_method    -"-
        lpf_tsg3 :      out integer range 0 to 2047;    -- lines_pr_frame TSG3
        sm_tsg3 :       out std_logic;                  -- scan_method    -"-
        -- debug :
        outputid :      out std_logic_vector(1 downto 0);
        lpf_ok :        out std_logic;
        sm_ok :         out std_logic
        );
end serial_interface;

architecture Behavioral of serial_interface is

signal buf_ptr :        std_logic_vector(3 downto 0);
--signal format_buffer :  std_logic_vector(11 downto 0);
signal output_id :      std_logic_vector(1 downto 0); -- TSG1=00, TSG2=01, TSG3=10
signal format_read :    std_logic;

-- til simplificering af serial inteface laeses kun scan_method og LSB af lines_pr_frame,
-- da det til de understoettede formater kune er noedvendigt at kende disse.
-- lines pr frame er enten 750 (LSB='1') eller 1125 (LSB='0')
signal scan_method :    std_logic;
signal lpf_lsb :        std_logic;
signal lines_pr_frame : integer range 0 to 2047;
-- debug :
--signal lpf_compare :    std_logic_vector(10 downto 0);
--signal sm_compare :     std_logic;

begin

---------------------------------------------------------------------------
--         __    __    __    __    __    __    __    __    __    __    __    __    __    __
-- sck  __|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|
--      _____
-- cs        |_________________________________________________________________________
--      ________ _____ _____ _____ _____ _____ __  _ _____ _____ _____ _____ __
-- mosi ________|_____|_____|_____|_____|_____|_  __|_____|_____|_____|_____|_
--
--
-- Protokol :
-- (lpf = lines pr frame) 
--
--                  |---------------------------------------------------------------|
-- 1st nibble :     | not used      | Ouput ID MSB  | Output ID LSB | scan_method   |
--                  |---------------|---------------|---------------|---------------|
-- 2nd nibble :     | not used      | lpf MSB       | lfp           | lpf           |
--                  |---------------|---------------|---------------|---------------|
-- 3rd nibble :     | lpf           | lpf           | lfp           | lpf           |
--                  |---------------|---------------|---------------|---------------|
-- 4th nibble :     | lpf           | lpf           | lfp           | lpf LSB       |
--                  |---------------------------------------------------------------|
                

make_bufptr : process( cs, sck )
-- buf_ptr er en pointer til video format bufferen
-- baade lines_pr_frame og scan_method laeses ind i bufferen og
-- clockes derefter over i lines_pr_frame og scan_method for den generator,
-- der svarer til output_id
begin
    if cs = '1' then
        buf_ptr <= "1111";
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

    end if;            
end process;


get_output_id : process( cs, sck )
begin
    if cs = '1' then
        output_id <= "11";  -- soerger for, at ingene generatorer er valgt udenfor chip select
    elsif sck'event and sck = '0' then
        -- de foerste to bit af data-strommen er output ID
        case buf_ptr is
            when "0001" =>
                output_id(1) <= mosi;
            when "0010" =>
                output_id(0) <= mosi;
            when others =>
                null;
        end case;
    end if;
end process;


get_data : process( cs, sck )
begin
    if cs = '1' then
--        format_buffer <= "000000000000";
        scan_method <= '1';
        lpf_lsb <= '0';
    elsif sck'event and sck = '0' then
        case buf_ptr is
            when "0011" =>
--                format_buffer(11) <= mosi;    -- scan_method
                scan_method       <= mosi;
--            when "0100" =>
  --              format_buffer(10) <= mosi;
    --        when "0101" =>
      --          format_buffer(9) <= mosi;
        --    when "0110" =>
          --      format_buffer(8) <= mosi;
            --when "0111" =>
--                format_buffer(7) <= mosi;
  --          when "1000" =>
    --            format_buffer(6) <= mosi;
      --      when "1001" =>
        --        format_buffer(5) <= mosi;
          --  when "1010" =>
            --    format_buffer(4) <= mosi;
--            when "1011" =>
  --              format_buffer(3) <= mosi;
    --        when "1100" =>
      --          format_buffer(2) <= mosi;
        --    when "1101" =>
          --      format_buffer(1) <= mosi;
            when "1110" =>
--                format_buffer(0) <= mosi;
                lpf_lsb <= mosi;
            when others =>
                null;
        end case;
    end if;        
end process;

with buf_ptr select
    -- format_read er en puls, der gaar hoej efter sidst transmitterede bit :
    format_read <= '1' when "1110",
                   '0' when others;

with lpf_lsb select
    lines_pr_frame <= 1124 when '0',
                      749 when others;

write_data : process( format_read )
begin
    if format_read'event and format_read = '1' then
        case output_id is
            when "00" =>
--                lpf_tsg1 <= conv_integer(format_buffer(10 downto 0));
  --              sm_tsg1 <= format_buffer(11);
                lpf_tsg1 <= lines_pr_frame;
                sm_tsg1 <= scan_method;
            when "01" =>
    --            lpf_tsg2 <= conv_integer(format_buffer(10 downto 0));
      --          sm_tsg2 <= format_buffer(11);
                lpf_tsg2 <= lines_pr_frame;
                sm_tsg2 <= scan_method;
            when "10" =>
        --        lpf_tsg3 <= conv_integer(format_buffer(10 downto 0));
          --      sm_tsg3 <= format_buffer(11);
                lpf_tsg3 <= lines_pr_frame;
                sm_tsg3 <= scan_method;
            when others =>
                null;
        end case;
    end if;        
end process;

-- debug :
outputid <= output_id;

end Behavioral;
