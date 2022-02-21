library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity video_mux is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;                        -- sync. reset

      -- frame control signals:
      line_o               : in integer;
      fvh_o                : in std_logic_vector(2 downto 0);   -- blanking bits
      calc_crc_o           : in integer;
      
      -- input signals:
      y_av_i               : std_logic_vector(9 downto 0);
      cb_av_i              : std_logic_vector(9 downto 0);
      cr_av_i              : std_logic_vector(9 downto 0);

      luma_ancil_i         : std_logic_vector(9 downto 0);
      croma_ancil_i        : std_logic_vector(9 downto 0);
      
      luma_vblank_i        : std_logic_vector(9 downto 0);
      croma_vblank_i       : std_logic_vector(9 downto 0);
                  
      -- output signals:
      calc_crc_o           : out integer;

      luma_o               : out std_logic_vector(9 downto 0);   -- output to scrambler
      chroma_o             : out std_logic_vector(9 downto 0)    -- output to scrambler
   );
end video_mux;


architecture behavioral of video_mux is


begin


---------------------------------------------------------------
-- Muxing video data:
---------------------------------------------------------------
video_muxing : process(zreset_i, clk_i)
begin
   if zreset_i = '0' then
      luma <= luma_black;
      croma <= croma_black;
   elsif clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         luma <= luma_black;
         croma <= croma_black;
      elsif en_i = '1' then
         case state is
            when sd_sav1 =>                           -- Start active video
               luma <= "0000000000";
               croma <= "1111111111";
            when sd_sav2 =>
               with f_bit & v_bit select
               luma <= "1000000000" when "00",
                       "1010101100" when "01",
                       "1100011100" when "10",
                       "1110110000" when "11",
                       "0000000000" when others;
               croma <= "0000000000";
            when hd_sav1 =>
               luma <= "1111111111";
               croma <= "1111111111";
            when hd_sav2 =>
               luma <= "0000000000";
               croma <= "0000000000";
            when hd_sav3 =>
               luma <= "0000000000";
               croma <= "0000000000";
            when hd_sav4 =>
               luma <= "1000000000" when "00",
                       "1010101100" when "01",
                       "1100011100" when "10",
                       "1110110000" when "11",
                       "0000000000" when others;
               croma <= luma;
            when sd_eav1 =>                           -- End of active video
               luma <= "0000000000";
               croma <= "1111111111";
            when sd_eav2 =>
               with f_bit & v_bit select
               luma <= "1001110100" when "00",
                       "1011011000" when "01",
                       "1101101000" when "10",
                       "1111000100" when "11",
                       "0000000000" when others;
               croma <= "0000000000";
            when hd_eav1 =>
               luma <= "1111111111";
               croma <= "1111111111";
            when hd_eav2 =>
               luma <= "0000000000";
               croma <= "0000000000";
            when hd_eav3 =>
               luma <= "0000000000";
               croma <= "0000000000";
            when hd_eav4 =>
               with f_bit & v_bit select
               luma <= "1001110100" when "00",
                       "1011011000" when "01",
                       "1101101000" when "10",
                       "1111000100" when "11",
                       "0000000000" when others;
               croma <= luma;
            when hd_ln0 =>                            -- Line number
               luma <= not line_count(6)) & line_count(6 downto 0) & "00";
               croma <= luma;
            when hd_ln1 =>
               luma <= "1000" & line_count(10 downto 7) & "00";
               croma <= luma;
            when others =>                            -- CRC, video and ancillary data are inserted later
               luma <= luma_black;
               croma <= croma_black;
         end case;
      end if;
   end if;
end process;


end behavioral;