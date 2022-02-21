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
signal cs_int :                 std_logic;
signal output_id :              std_logic_vector(1 downto 0);
signal bit_address :            integer range 0 to 7;
signal format_buffer :          std_logic_vector(6 downto 0);
signal format_read :            std_logic;  -- signalerer nyt format laest

begin

---------------------------------------------------------------
-- FORMAT REGISTRE BESTEMT AF OUTPUT ID :
---------------------------------------------------------------

--------------------------------------------------------------
-- set output_id :
output_id <= "01";
--------------------------------------------------------------
-- make intern chip select :
with format_buffer(6 downto 5) select
    cs_int <= '0' when "01",
              '1' when others;

id_fetch : process( cs, sck, output_id(0) )
begin
    if cs = '1' then
        format_buffer(5) <= not output_id(0); -- sikrer at cs_int = '1' naar cs = '1'
    elsif sck'event and sck = '0' then
        if bit_address = 1 then
            -- get output_id(1)
            format_buffer(6) <= mosi;
        elsif bit_address = 2 then
            -- get output_id(0)
            format_buffer(5) <= mosi;
        else
            null;
        end if;
    end if;
end process;


adrress_counter : process( cs, sck, bit_address )
begin
    if cs = '1' then
        bit_address <= 0;
    elsif sck'event and sck = '1' then
        if bit_address /= 7 then
            bit_address <= bit_address + 1;
        else
            bit_address <= 0;
        end if;
    end if;
end process;


data_fetch : process(cs_int, sck, bit_address)
begin
    if cs_int = '1' then
        format_read <= '0';
    elsif sck'event and sck = '0' then
        case bit_address is
            when 3 =>
                format_buffer(4) <= mosi;
            when 4 =>
                format_buffer(3) <= mosi;
            when 5 =>
                format_buffer(2) <= mosi;
            when 6 =>
                format_buffer(1) <= mosi;
            when 7 =>
                format_buffer(0) <= mosi;
            when 0 =>
                format_read <= '1';
            when others =>
                format_read <= '0';
        end case;
    end if;
end process;

-- scan_method '0' = interlaced
-- scan_method '1' = progressiv

-- sync_mode 10 (1 i tabel) = genlocked to f4m ( NTSC 525 line )
-- sync_mode 11 (2 i tabel) = genlocked to f8g ( PAL 625 line )
-- sync_mode 0X (3 i tabel) = freerunning

-- system clock 0 (1 i tabel) = 74.176MHz 
-- system clock 1 (2 i tabel) = 74.25MHz


read_format_buffer : process(tsg_reset, format_read)
begin
    if tsg_reset <= '0' then
--            phasedelay      <= "00100101110000111111001100";
--            phasedelay      <= "00000000000000000001001100";
            scan_method     <= '0';
            sync_mode       <= "00";
            samples_pr_line <= 0;
            lines_pr_frame  <= 0;
            system_clk      <= '0';
    elsif format_read'event and format_read = '1' then
        -- format_buffer(4 downto 0) = format ID ilfg tabel
        case format_buffer(4 downto 0) is
            when "00001" =>
                -- 1920X1080/60/P
                scan_method     <= '1';
                sync_mode(1)    <= '0';
                samples_pr_line <= 2199;
                lines_pr_frame  <= 1124;
                system_clk      <= '1';
            when "00010" =>
                -- 1920x1080/59.59/P
                scan_method     <= '1';
                sync_mode       <= "10";
                samples_pr_line <= 2199;
                lines_pr_frame  <= 1124;
                system_clk      <= '0';
            when "00011" =>
                -- 1920x1080/50/P
                scan_method     <= '1';
                sync_mode       <= "11";
                samples_pr_line <= 2199;
                lines_pr_frame  <= 1124;
                system_clk      <= '1';
            when "00100" =>
                scan_method     <= '0';
                sync_mode(1)    <= '0';
                samples_pr_line <= 4399;
                lines_pr_frame  <= 1124;
                system_clk      <= '1';
            when "00101" =>
                scan_method     <= '0';
                sync_mode       <= "10";
                samples_pr_line <= 4399;
                lines_pr_frame  <= 1124;
                system_clk      <= '0';
            when "00110" =>
                scan_method     <= '0';
                sync_mode       <= "11";
                samples_pr_line <= 4399;
                lines_pr_frame  <= 1124;
                system_clk      <= '1';
            when "00111" =>
                scan_method     <= '1';
                sync_mode(1)    <= '0';
                samples_pr_line <= 4399;
                lines_pr_frame  <= 1124;
                system_clk      <= '1';
            when "01000" =>
                scan_method     <= '1';
                sync_mode       <= "10";
                samples_pr_line <= 4399;
                lines_pr_frame  <= 1124;
                system_clk      <= '0';
            when "01001" =>
                scan_method     <= '1';
                sync_mode       <= "11";
                samples_pr_line <= 5279;
                lines_pr_frame  <= 1124;
                system_clk      <= '1';
            when "01010" =>
                scan_method     <= '1';
                sync_mode(1)    <= '0';
                samples_pr_line <= 5499;
                lines_pr_frame  <= 1124;
                system_clk      <= '1';
            when "01011" =>
                scan_method     <= '1';
                sync_mode(1)    <= '0';
                samples_pr_line <= 5499;
                lines_pr_frame  <= 1124;
                system_clk      <= '0';
            when "01100" =>
                scan_method     <= '0';
                sync_mode(1)    <= '0';
                samples_pr_line <= 4399;
                lines_pr_frame  <= 1124;
                system_clk      <= '1';
            when "01101" =>
                scan_method     <= '0';
                sync_mode       <= "10";
                samples_pr_line <= 4399;
                lines_pr_frame  <= 1124;
                system_clk      <= '0';
            when "01110" =>
                scan_method     <= '1';
                sync_mode       <= "10";
                samples_pr_line <= 3299;
                lines_pr_frame  <= 749;
                system_clk      <= '1';
            when "01111" =>
                scan_method     <= '1';
                sync_mode(1)    <= '0';
                samples_pr_line <= 3299;
                lines_pr_frame  <= 749;
                system_clk      <= '0';
            when "10000" =>
                scan_method     <= '1';
                sync_mode       <= "11";
                samples_pr_line <= 3959;
                lines_pr_frame  <= 749;
                system_clk      <= '1';
            when "10001" =>
                scan_method     <= '1';
                sync_mode(1)    <= '0';
                samples_pr_line <= 6599;
                lines_pr_frame  <= 749;
                system_clk      <= '1';
            when "10010" =>
                scan_method     <= '1';
                sync_mode       <= "10";
                samples_pr_line <= 6599;
                lines_pr_frame  <= 749;
                system_clk      <= '0';
            when "10011" =>
                scan_method     <= '1';
                sync_mode       <= "11";
                samples_pr_line <= 7919;
                lines_pr_frame  <= 749;
                system_clk      <= '1';
            when "10100" =>
                scan_method     <= '1';
                sync_mode(1)    <= '0';
                samples_pr_line <= 8249;
                lines_pr_frame  <= 749;
                system_clk      <= '1';
            when "10101" =>
                scan_method     <= '1';
                sync_mode(1)    <= '0';
                samples_pr_line <= 8249;
                lines_pr_frame  <= 749;
                system_clk      <= '0';
            when others =>
                null;
        end case;
    end if;
end process;

phasedelay <= "00000000000000000000111111";

    
end Behavioral;
