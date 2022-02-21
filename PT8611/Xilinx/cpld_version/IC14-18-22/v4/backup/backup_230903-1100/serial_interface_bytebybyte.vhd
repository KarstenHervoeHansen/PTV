library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity serial_interface is
    Port( 
        mreset :                in std_logic;
        cs :                    in std_logic;
        sck :                   in std_logic;
        mosi :                  in std_logic;
        miso :                  in std_logic;       -- (output not used yet - configured as input)

        phasedelay :            out std_logic_vector(25 downto 0);  -- samples x lines x frames
        scan_method :           out std_logic;                      -- ( 0=interlaced, 1=progressiv )
        sync_mode :             out std_logic_vector(1 downto 0);   -- 0X=freerunning, 10=genlocked to f4m, 11= genlocked to f8g
        samples_pr_line :       out integer range 0 to 8191;
        lines_pr_frame :        out integer range 0 to 4095;
        system_clk :            out std_logic
        );
end serial_interface;

architecture Behavioral of serial_interface is

signal samples_pr_line_bin :    std_logic_vector(13 downto 0);
signal lines_pr_frame_bin :     std_logic_vector(10 downto 0);
signal output_id :              std_logic_vector(1 downto 0);
signal id_read :                std_logic_vector(1 downto 0);
signal id_cnt :                 integer range 0 to 3;
signal cs_int :                 std_logic;  -- intern chip select - lav hvis id_read = output_id
signal buffer_ptr :             std_logic_vector(2 downto 0);
signal data_buffer :            std_logic_vector(7 downto 0);
signal byte_ready :             std_logic;  -- signalerer nyt format laest
signal which_byte :             integer range 0 to 7;

begin

---------------------------------------------------------------
-- FORMAT READ BYTE BY BYTE :
---------------------------------------------------------------

--------------------------------------------------------------
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
        id_cnt <= 0;    -- id_cnt 
    elsif sck'event and sck = '0' then
        if id_cnt = 0 then
            -- get output_id(1)
            id_cnt <= 1;
            id_read(1) <= mosi;
        elsif id_cnt = 1 then
            -- get output_id(0)
            id_cnt <= 3;
            id_read(0) <= mosi;
        else
            null;
        end if;
    end if;
end process;


make_buffer_ptr : process( cs_int, sck )
begin
    if cs_int = '1' then
        buffer_ptr <= "111";
        byte_ready <= '1';
    elsif sck'event and sck = '1' then

        buffer_ptr(0) <= not buffer_ptr(0);

        if buffer_ptr(0) = '1' then
            buffer_ptr(1) <= not buffer_ptr(1);
        end if;

        if buffer_ptr(1 downto 0) = "11" then
            buffer_ptr(2) <= not buffer_ptr(2);
        end if;

        if buffer_ptr = "111" then
            byte_ready <= '1';
        else
            byte_ready <= '0';
        end if;

    end if;
end process;


data_fetch : process(cs_int, sck, buffer_ptr)
begin
    if cs_int = '1' then
        null;
    elsif sck'event and sck = '0' then
        case buffer_ptr is
            when "000" =>
                data_buffer(7) <= mosi;
            when "001" =>
                data_buffer(6) <= mosi;
            when "010" =>
                data_buffer(5) <= mosi;
            when "011" =>
                data_buffer(4) <= mosi;
            when "100" =>
                data_buffer(3) <= mosi;
            when "101" =>
                data_buffer(2) <= mosi;
            when "110" =>
                data_buffer(1) <= mosi;
            when others =>
                data_buffer(0) <= mosi;   -- system_clk
        end case;
    end if;
end process;

make_which_byte : process( cs_int, byte_ready )
begin
    if cs_int = '1' then
        which_byte <= 0;
    elsif byte_ready'event and byte_ready = '0' then
        which_byte <= which_byte + 1;
    end if;
end process;

read_data_buffer : process(mreset, byte_ready, which_byte )
begin
    if mreset = '0' then
        phasedelay      <= "00000000000000000001001100";
        scan_method     <= '0';
        sync_mode       <= "10";
        samples_pr_line_bin <= "01000100101111"; -- 4399
        lines_pr_frame_bin  <= "10001100100"; -- 1124
        system_clk      <= '0';
    elsif byte_ready'event and byte_ready = '1' then
        case which_byte is
            when 0 =>
                phasedelay(25 downto 18)            <= data_buffer;
            when 1 =>
                phasedelay(17 downto 10)            <= data_buffer;
            when 2 =>
                phasedelay(9 downto 2)              <= data_buffer;
            when 3 =>
                phasedelay(1 downto 0)              <= data_buffer(7 downto 6);
                scan_method                         <= data_buffer(5);
                sync_mode                           <= data_buffer(4 downto 3);
                samples_pr_line_bin(13 downto 11)   <= data_buffer(2 downto 0);
            when 4 =>
                samples_pr_line_bin(10 downto 3)    <= data_buffer;
            when 5 =>
                samples_pr_line_bin(2 downto 0)     <= data_buffer(7 downto 5);
                lines_pr_frame_bin(10 downto 6)     <= data_buffer(4 downto 0);
            when 6 =>
                lines_pr_frame_bin(5 downto 0)      <= data_buffer(7 downto 2);
                system_clk                          <= data_buffer(1);
            when others =>
                null;
        end case;
    end if;
end process;

samples_pr_line <= conv_integer(samples_pr_line_bin);
lines_pr_frame  <= conv_integer(lines_pr_frame_bin);

--            phasedelay      <= "00100101110000111111001100";
--            phasedelay      <= "00000000000000000001001100";
  --          scan_method     <= '0';
    --        sync_mode       <= "10";
      --      samples_pr_line <= 4399;
        --    lines_pr_frame  <= 1124;
          --  system_clk      <= '0';
--    elsif format_read'event and format_read = '1' then
  --      if data_buffer(56 downto 55) = output_id then
    --        phasedelay      <= data_buffer(54 downto 29);
      --      scan_method     <= data_buffer(28);
        --    sync_mode       <= data_buffer(27 downto 26);
          --  samples_pr_line <= conv_integer(data_buffer(25 downto 12));
            --lines_pr_frame  <= conv_integer(data_buffer(11 downto 1));
--            system_clk      <= data_buffer(0);
  --      else
    --        null;
      --  end if;
--    end if;
--end process;

    
--samples_pr_line <= 4399;--conv_integer(samples_pr_line_bin);
--lines_pr_frame  <= 1125;--conv_integer(lines_pr_frame_bin);

-- 1920x1080/I :
--scan_method <= '0'; -- 0=interlaced scan, 1=progresive scan
--samples_pr_line_bin <= "01000100101111";    -- 4399
--lines_pr_frame_bin  <= "10001100100";       -- 1124
--sync_mode <= "10";                    
--phasedelay <= "00100101110000111111001100";

-- 1280x720/P :
--scan_method <= '1';                 -- 0=interlaced scan, 1=progresive scan
--samples_pr_line <= 3299;        
--lines_pr_frame <= 749;
--sync_mode <= '1';

--system_clk <=

end Behavioral;
