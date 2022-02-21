library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity black_frame is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      frame_start_i        : in std_logic;

      -- system parameters:
      sd_zhd_i             : in std_logic;                        -- 1: sd, 0: hd
      asl_i                : in std_logic_vector(11 downto 0);    -- active samples per line
      lpf_i                : in std_logic_vector(10 downto 0);    -- lines per frame
      hb1s_i               : in std_logic_vector(11 downto 0);    -- horizontal blanking 1 length (including CRC)
      hb2s_i               : in std_logic_vector(11 downto 0);    -- horizontal blanking 2 length

      field1_end_i         : in std_logic_vector(10 downto 0);
      field2_end_i         : in std_logic_vector(10 downto 0);
      f_start_i            : in std_logic;

      field1_vblnk_end_i   : in std_logic_vector(10 downto 0);    -- line numbers for vertical blanking borders
      field1_av_end_i      : in std_logic_vector(10 downto 0);
      field2_vblnk_end_i   : in std_logic_vector(10 downto 0);
      field2_av_end_i      : in std_logic_vector(10 downto 0);

      -- frame control signals:
      line_o               : out integer;
      fvh_o                : out std_logic_vector(2 downto 0);    -- blanking bits
      calc_crc_o           : out integer;
      cb_zcr_o             : out integer;
      line_end_o           : out integer;                         
      
      -- parallel data outputs:
      luma_o               : out std_logic_vector(9 downto 0);
      chroma_o             : out std_logic_vector(9 downto 0);
      
      -- other output signals:
      sync_reset_o         : out integer;                         -- syncronious reset output to oversampler:
      timing_change_o      : out integer
   );
end black_frame;


architecture behavioral of black_frame is


type state_type is (sd_eav1, sd_eav2, hd_eav1, hd_eav2, hd_eav3, hd_eav4, hd_ln0, hd_ln1,
                    h_blank1, h_blank2, sd_sav1, sd_sav2, hd_sav1, hd_sav2, hd_sav3, hd_sav4, active_video);
signal state               : state_type;     
signal state_count         : std_logic_vector(11 downto 0);
signal luma                : std_logic_vector(9 downto 0);
signal croma               : std_logic_vector(9 downto 0);
signal f_bit               : std_logic;
signal v_bit               : std_logic;
signal h_bit               : std_logic;
signal line_count          : std_logic_vector(10 downto 0);
signal look_at_frame_start : std_logic;
signal sync_reset          : std_logic;

constant luma_black        : std_logic_vector(9 downto 0)    := "0001000000";
constant croma_black       : std_logic_vector(9 downto 0)    := "1000000000";


begin

--                                                                                                       ---
-- sync_reset:                                                                                       |   |
--                      --------------------------------------------------------------------------------     ----------------------------------------------------------------------------
--                      -------------- --------- --------- ------------------------------------------------- ------------------------------- --------- --------- ------------------------
-- line_state (sd):      active_video X sd_eav1 X sd_eav2 X h_blank1                                        X           h_blank2            X sd_sav1 X sd_sav2 X active_video
--                      -------------- --------- --------- ------------------------------------------------- ------------------------------- --------- --------- ------------------------
--                      -------------- --------- --------- --------- --------- --------- -------- ---------- ---------- --------- ---------- --------- --------- ------------------------
-- line_state (hd):      active_video X hd_eav1 X hd_eav2 X hd_eav3 X hd_eav4 X hd_ln0  X hd_ln1 X h_blank1 X h_blank2 X hd_sav1 X hd_sav2  X hd_sav3 X hd_sav4 X active_video
--                      -------------- --------- --------- --------- --------- --------- -------- ---------- ---------- --------- ---------- --------- --------- ------------------------
--                      ------------------------ ----------------------------------------------------------------------------------------------------------------------------------------
-- line_o:                     lpf              X   1
--                      ------------------------ ----------------------------------------------------------------------------------------------------------------------------------------
--                      ------------------------ ----------------------------------------------------------------------------------------------------------------------------------------
-- f_bit:                                       X   (transition not necessarily at line 1)
--                      ------------------------ ----------------------------------------------------------------------------------------------------------------------------------------
--                      ------------------------ ----------------------------------------------------------------------------------------------------------------------------------------
-- v_bit:                                       X   (transition not necessarily at line 1)
--                      ------------------------ ----------------------------------------------------------------------------------------------------------------------------------------
--                                               -------------------------------------------------------------------------------------------------------------------------
-- h_bit :                                      |                                                                                                                         |
--                      ------------------------                                                                                                                           --------------
--                      ------------------------ --------- --------- --------------------------------------------------------------------------------- --------- --------- --------------
-- luma_o/croma_o (sd):  black                  X  eav1   X  eav2   X                    black                                                        X  sav1   X  sav2   X  black
--                      ------------------------ --------- --------- --------------------------------------------------------------------------------- --------- --------- --------------
--                      ------------------------ --------- --------- --------- --------- -------- ---------- -------------------- ---------- --------- --------- --------- --------------
-- luma_o/croma_o (hd):  black                  X  eav1   X  eav2   X  eav3   X  eav4   X  ln0   X  ln1     X    black           X  sav1    X  sav2   X  sav3   X  sav4   X  black
--                      ------------------------ --------- --------- --------- --------- -------- ---------- -------------------- ---------- --------- --------- --------- --------------
--                      -------------------------------------------------------------------------                                                                ------------------------
-- calc_crc_o:                                                                                   |                                                              |
--                                                                                                --------------------------------------------------------------


---------------------------------------------------------------
-- Line state machine. Updates only when en_i = '1'
---------------------------------------------------------------
line_sm : process(zreset_i, clk_i)
begin
   if zreset_i = '0' then
      state <= h_blank2;
      state_count <= hb2s_i;
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         state <= h_blank2;
         state_count <= hb2s_i;
      elsif en_i = '1' then
         case state is
            when h_blank2 =>                          -- H. Blanking
               if state_count = conv_std_logic_vector(1,12) then
                  if sd_zhd_i = '1' then
                     state <= sd_sav1;
                     state_count <= "XXXXXXXXXXX";
                  else
                     state <= hd_sav1;
                     state_count <= "XXXXXXXXXXX";
                  end if;
               else
                  state_count <= state_count - 1;
               end if;
            when sd_sav1 =>                           -- Start active video words
               state <= sd_sav2;
            when sd_sav2 =>
               state <= active_video;
               state_count <= asl_i;
            when hd_sav1 =>
               state <= hd_sav2;
               state_count <= "XXXXXXXXXXX";
            when hd_sav2 =>
               state <= hd_sav3;
               state_count <= "XXXXXXXXXXX";
            when hd_sav3 =>
               state <= hd_sav4;
               state_count <= "XXXXXXXXXXX";
            when hd_sav4 =>
               state <= active_video;
               state_count <= asl_i;
            when active_video =>                      -- Active video
               if state_count = conv_std_logic_vector(1,12) then
                  if sd_zhd_i = '1' then
                     state <= sd_eav1;
                     state_count <= "XXXXXXXXXXX";
                  else
                     state <= hd_eav1;
                     state_count <= "XXXXXXXXXXX";
                  end if;
               else
                  state_count <= state_count - 1;
               end if;
            when sd_eav1 =>                           -- End active video words
               state <= sd_eav2;
            when sd_eav2 =>
               state <= h_blank1;
               state_count <= hb1s_i;
            when hd_eav1 =>
               state <= hd_eav2;
               state_count <= "XXXXXXXXXXX";
            when hd_eav2 =>
               state <= hd_eav3;
               state_count <= "XXXXXXXXXXX";
            when hd_eav3 =>
               state <= hd_eav4;
               state_count <= "XXXXXXXXXXX";
            when hd_eav4 =>
               state <= hd_ln0;
               state_count <= "XXXXXXXXXXX";
            when hd_ln0 =>                            -- Line number
               state <= hd_ln1;
               state_count <= "XXXXXXXXXXX";
            when hd_ln1 =>
               state <= h_blank1;
               state_count <= hb1s_i;
            when h_blank1 =>                          -- H. blanking
               if state_count = conv_std_logic_vector(1,12) then
                  state <= h_blank2;
                  state_count <= hb2s_i;
               else
                  state_count <= state_count - 1;
               end if;
            when others =>                            -- Never happens
               state <= h_blank2;
               state_count <= hb2s_i;
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- circular counter with a period on total lines per frame.
-- Counts when en_i = '1'
---------------------------------------------------------------
line_counter : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      line_count <= conv_std_logic_vector(1,11);
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         line_count <= conv_std_logic_vector(1,11);
      elsif en_i = '1' and (state = sd_eav1 or state = hd_eav1) then
         if line_count = lpf_i then
            line_count <= conv_std_logic_vector(1,11);
         else
            line_count <= line_count + 1;
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate blanking signals:
---------------------------------------------------------------
f_bit_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      f_bit <= f_start_i;
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         f_bit <= f_start_i;
      elsif en_i = '1' and (state = sd_eav1 or state = hd_eav1) then
         if line_count = field2_end_i then
            f_bit <= '0';
         elsif line_count = field1_end_i then
            f_bit <= '1';
         end if;
      end if;
   end if;
end process;


v_bit_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      v_bit <= '1';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         v_bit <= '1';
      elsif en_i = '1' and (state = sd_eav1 or state = hd_eav1) then
         if line_count = field2_av_end_i then
            v_bit <= '1';
         elsif line_count = field1_vblnk_end_i then
            v_bit <= '0';
         elsif line_count = field1_av_end_i then
            v_bit <= '1';
         elsif line_count = field2_vblnk_end_i then
            v_bit <= '0';
         end if;
      end if;
   end if;
end process;


h_bit_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      h_bit <= '0';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         h_bit <= '0';
      elsif en_i = '1' then
         if state = active_video then
            h_bit <= '0';
         else
            h_bit <= '1';
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Muxing video data:
---------------------------------------------------------------
cb_zcr_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      cb_zcr_o <= '0';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         cb_zcr_o <= '0';
      elsif en_i = '1' then
         cb_zcr_o <= not cb_zcr_o;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Muxing video data:
---------------------------------------------------------------
video_muxing : process(zreset_i, clk_i)
begin
   if zreset_i = '0' then
      luma <= luma_black;
      croma <= croma_black;
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
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


---------------------------------------------------------------
-- Generate calc_crc_o:
---------------------------------------------------------------
calc_crc_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      calc_crc_o <= '0';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         calc_crc_o <= '0';
      elsif en_i = '1' then
         if state = hd_ln1 then
            calc_crc_o <= '0';
         elsif state = hd_sav4 then
            calc_crc_o <= '1';
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate line_end_o:
---------------------------------------------------------------
last_samp_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      line_end_o <= '0';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         line_end_o <= '0';
      elsif en_i = '1' then
         if state = h_blank1 and state_count = conv_std_logic_vector(1,12) then
            line_end_o <= '1';
         else
            line_end_o <= 'o';
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate look_at_frame_start and sync_reset: 
---------------------------------------------------------------
frame_end_delayed_1 <= line_end and en_i and line_count = conv_std_logic_vector(1,11);


look_at_frame_start_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      look_at_frame_start <= '1';
   elsif clk_i'event and clk_i = '1' then
      if frame_start_i = '0' and sync_reset = '1' then
         look_at_frame_start <= '0';   
      elsif frame_start_i = '1' and frame_end = '0' then
         look_at_frame_start <= '1';
      end if;
   end if;
end process;


sync_reset_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      sync_reset <= '1';
   elsif clk_i'event and clk_i = '1' then
      if look_at_frame_start = '1' then
         sync_reset <= frame_start_i;   
      else
         sync_reset <= '0';
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Mapping of outputs:
---------------------------------------------------------------
line_o <= line_count;
luma_o <= luma;
croma_o <= croma;
fvh <= f_bit & v_bit & h_bit;
sync_reset_o <= sync_reset;
timing_change_o <= look_at_frame_start;

end behavioral;