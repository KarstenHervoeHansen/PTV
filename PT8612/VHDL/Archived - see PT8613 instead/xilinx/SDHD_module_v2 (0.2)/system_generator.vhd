library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity system_generator is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      frame_start_i        : in std_logic;

      -- system parameters:
      system_i             : in std_logic_vector(7 downto 0);
      
      -- parallel data inputs (video and ancillary)
      luma_video_i         : in std_logic_vector(9 downto 0);
      chroma_video_i       : in std_logic_vector(9 downto 0);

      luma_h_blank_i       : in std_logic_vector(9 downto 0);
      chroma_h_blank_i     : in std_logic_vector(9 downto 0);
      
      luma_v_blank_i       : in std_logic_vector(9 downto 0);
      chroma_v_blank_i     : in std_logic_vector(9 downto 0);
      
      ---system parameter outputs:
      hd_zsd_o             : out std_logic;
      interlaced_o         : out std_logic;
      asl_o                : out std_logic_vector(10 downto 0);
      color_system_o       : out std_logic_vector(1 downto 0);
      
      -- frame control signals:
      last_av_sample_o     : out std_logic;
      av_line_o            : out std_logic_vector(10 downto 0);
      av_sample_o          : out std_logic_vector(10 downto 0);
      line_o               : out std_logic_vector(10 downto 0);
      fvh_o                : out std_logic_vector(2 downto 0);    -- blanking bits
      cr_zcb_o             : out std_logic;
      sav_tick_advanced_o  : out std_logic;
      eav_tick_advanced_o  : out std_logic;
      frame_end_o          : out std_logic;

      -- parallel data outputs:
      luma_o               : out std_logic_vector(9 downto 0);
      chroma_o             : out std_logic_vector(9 downto 0);

      -- other output signals:
      sync_reset_o         : out std_logic;                       -- syncronious reset output to oversampler:
      timing_change_o      : out std_logic;
      
      -- debug:
      debug_o              : out std_logic_vector(9 downto 0)
   );
end system_generator;


architecture behavioral of system_generator is


type state_type is (sd_eav1, sd_eav2, hd_eav1, hd_eav2, hd_eav3, hd_eav4, hd_ln0, hd_ln1,
                    h_blank1, h_blank2, sd_sav1, sd_sav2, hd_sav1, hd_sav2, hd_sav3, hd_sav4, active_video);
signal state                  : state_type;
signal state_count            : std_logic_vector(11 downto 0);
signal active_line_count      : std_logic_vector(10 downto 0);
signal luma                   : std_logic_vector(9 downto 0);
signal chroma                 : std_logic_vector(9 downto 0);
signal XYZ                    : std_logic_vector(9 downto 0);
signal f_bit                  : std_logic;
signal v_bit                  : std_logic;
signal h_bit                  : std_logic_vector(3 downto 0);
signal fvh_int                : std_logic_vector(2 downto 0);
signal line_count             : std_logic_vector(10 downto 0);
signal av_sample              : std_logic_vector(10 downto 0);

signal look_at_frame_start    : std_logic;
signal sync_reset             : std_logic;
signal cr_zcb                 : std_logic;
signal last_av_sample         : std_logic;
signal frame_end              : std_logic;
signal frame_start_delayed_1  : std_logic;
signal frame_start_delayed_2  : std_logic;
signal sync_reset_advanced_1  : std_logic;
signal calc_crc               : std_logic;

-- system parameters:
signal hd_zsd                 : std_logic;                        -- 0: sd, 1: hd
signal interlaced             : std_logic;                        -- 0: progressive, 1: Interlaced
signal asl                    : std_logic_vector(10 downto 0);    -- active samples per line
signal lpf                    : std_logic_vector(10 downto 0);    -- lines per frame
signal hb1s                   : std_logic_vector(11 downto 0);    -- horizontal blanking 1 length (including CRC)
signal hb2s                   : std_logic_vector(11 downto 0);    -- horizontal blanking 2 length
signal field1_end             : std_logic_vector(10 downto 0);
signal field2_end             : std_logic_vector(10 downto 0);
signal f_start                : std_logic;
signal sav_tick_advanced      : std_logic;
signal line_start             : std_logic_vector(10 downto 0);

signal field1_vblnk_end       : std_logic_vector(10 downto 0);    -- line numbers for vertical blanking borders
signal field1_av_end          : std_logic_vector(10 downto 0);
signal field2_vblnk_end       : std_logic_vector(10 downto 0);
signal field2_av_end          : std_logic_vector(10 downto 0);
                                                            
constant luma_black           : std_logic_vector(9 downto 0)    := "0001000000";
constant chroma_black         : std_logic_vector(9 downto 0)    := "1000000000";
constant av_sample_advancing  : integer := -24;                    -- maximum 0
constant sav_tick_advancing   : integer := 26;                     -- minimum 5
constant eav_tick_advancing   : integer := 4;--6;                  -- minimum 0

attribute uselowskewlines : string;
attribute uselowskewlines of sync_reset_o : signal is "yes";



component crc_inserter
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;                        -- sync. reset
      calc_crc_i           : in std_logic;                        -- high when crc is calculated, crc words are inserted immedeately after
      luma_i               : in std_logic_vector(9 downto 0);     -- Horizontal blanking data
      chroma_i             : in std_logic_vector(9 downto 0);
      luma_o               : out std_logic_vector(9 downto 0);    -- output to scrambler
      chroma_o             : out std_logic_vector(9 downto 0)     -- output to scrambler
   );
end component;


component system_parameters
   port (
      zreset_i             : in std_logic;
      clk_i                : in std_logic;
      system_i             : in std_logic_vector(7 downto 0);
      hd_zsd_o             : out std_logic;                       -- 0: sd, 1: hd
      interlaced_o         : out std_logic;                       -- 1: interlaced, 0: progressive
		asl_o                : out std_logic_vector(10 downto 0);   -- active samples per line
      lpf_o                : out std_logic_vector(10 downto 0);   -- lines per frame
      hb1s_o               : out std_logic_vector(11 downto 0);   -- horizontal blanking 1 length (samples), asl+10+h_blank1+h_blank2 =tsl
      hb2s_o               : out std_logic_vector(11 downto 0);   -- horizontal blanking 2 length (samples)
      field1_end_o         : out std_logic_vector(10 downto 0);
      field2_end_o         : out std_logic_vector(10 downto 0);
      f_start_o            : out std_logic;
      line_start_o         : out std_logic_vector(10 downto 0);
      field1_vblnk_end_o   : out std_logic_vector(10 downto 0);   -- line numbers for vertical blanking borders
      field1_av_end_o      : out std_logic_vector(10 downto 0);
      field2_vblnk_end_o   : out std_logic_vector(10 downto 0);
      field2_av_end_o      : out std_logic_vector(10 downto 0);
      color_system_o       : out std_logic_vector(1 downto 0)
   );
end component;


begin

--                                                                                                        ---
-- sync_reset:                                                                                           |   |
--                       --------------------------------------------------------------------------------     ----------------------------------------------------------------------------
--                       -------------- --------- --------- ------------------------------------------------- ------------------------------- --------- --------- ------------------------
-- line_state (sd):       active_video X sd_eav1 X sd_eav2 X h_blank1                                        X           h_blank2            X sd_sav1 X sd_sav2 X active_video
--                       -------------- --------- --------- ------------------------------------------------- ------------------------------- --------- --------- ------------------------
--                       -------------- --------- --------- --------- --------- --------- -------- ---------- ---------- --------- ---------- --------- --------- ------------------------
-- line_state (hd):       active_video X hd_eav1 X hd_eav2 X hd_eav3 X hd_eav4 X hd_ln0  X hd_ln1 X h_blank1 X h_blank2 X hd_sav1 X hd_sav2  X hd_sav3 X hd_sav4 X active_video
--                       -------------- --------- --------- --------- --------- --------- -------- ---------- ---------- --------- ---------- --------- --------- ------------------------
--                       -------------- --------------------------------------------------------------------------------------------------------------------------------------------------
-- line_o:                   lpf       X   line_start
--                       -------------- --------------------------------------------------------------------------------------------------------------------------------------------------
--                       -------------- --------------------------------------------------------------------------------------------------------------------------------------------------
-- fvh_o(2):                           X   (transition not necessarily at line 1)
--                       -------------- --------------------------------------------------------------------------------------------------------------------------------------------------
--                       -------------- --------------------------------------------------------------------------------------------------------------------------------------------------
-- fvh_o(1):                           X   (transition not necessarily at line 1)
--                       -------------- --------------------------------------------------------------------------------------------------------------------------------------------------
--                                      -------------------------------------------------------------------------------------------------------------------------
-- fvh_o(0):                           |                                                                                                                         |
--                       --------------                                                                                                                           --------------
--                       ------------------------ --------- --------- --------------------------------------------------------------------------------- --------- --------- --------------
-- luma_o/chroma_o (sd):  active_video           X  eav1   X  eav2   X                    black                                                        X  sav1   X  sav2   X  active_video
--                       ------------------------ --------- --------- --------------------------------------------------------------------------------- --------- --------- --------------
--                       ------------------------ --------- --------- --------- --------- -------- ---------- -------------------- ---------- --------- --------- --------- --------------
-- luma_o/chroma_o (hd):  active_video           X  eav1   X  eav2   X  eav3   X  eav4   X  ln0   X  ln1     X    black           X  sav1    X  sav2   X  sav3   X  sav4   X  active_video
--                       ------------------------ --------- --------- --------- --------- -------- ---------- -------------------- ---------- --------- --------- --------- --------------
--                       -------------------------------------------------------------------------                                                                ------------------------
-- calc_crc:                                                                                      |                                                              |
--                                                                                                 --------------------------------------------------------------


---------------------------------------------------------------
-- System parameters lookup:
---------------------------------------------------------------
system_parameter_lookup : system_parameters
port map (
   zreset_i                => zreset_i,
   clk_i                   => clk_i,
   system_i                => system_i,
   hd_zsd_o                => hd_zsd,
   interlaced_o            => interlaced,
   asl_o                   => asl,
   lpf_o                   => lpf,
   hb1s_o                  => hb1s,
   hb2s_o                  => hb2s,
   field1_end_o            => field1_end,
   field2_end_o            => field2_end,
   f_start_o               => f_start,
   line_start_o            => line_start,
   field1_vblnk_end_o      => field1_vblnk_end,
   field1_av_end_o         => field1_av_end,
   field2_vblnk_end_o      => field2_vblnk_end,
   field2_av_end_o         => field2_av_end,
   color_system_o          => color_system_o
);


---------------------------------------------------------------
-- Line state machine. Updates only when en_i = '1'
---------------------------------------------------------------
line_sm : process(zreset_i, clk_i)
begin
   if zreset_i = '0' then
      state <= h_blank2;
      state_count <= hb2s;
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         state <= h_blank2;
         state_count <= hb2s;
      elsif en_i = '1' then
         case state is
            when h_blank2 =>                          -- H. Blanking
               if state_count = conv_std_logic_vector(1,12) then
                  if hd_zsd = '0' then
                     state <= sd_sav1;
                     state_count <= "XXXXXXXXXXXX";
                  else
                     state <= hd_sav1;
                     state_count <= "XXXXXXXXXXXX";
                  end if;
               else
                  state_count <= state_count - 1;
               end if;
            when sd_sav1 =>                           -- Start active video words
               state <= sd_sav2;
            when sd_sav2 =>
               state <= active_video;
               state_count <= '0' & asl;
            when hd_sav1 =>
               state <= hd_sav2;
               state_count <= "XXXXXXXXXXXX";
            when hd_sav2 =>
               state <= hd_sav3;
               state_count <= "XXXXXXXXXXXX";
            when hd_sav3 =>
               state <= hd_sav4;
               state_count <= "XXXXXXXXXXXX";
            when hd_sav4 =>
               state <= active_video;
               state_count <= '0' & asl;
            when active_video =>                      -- Active video
               if state_count = conv_std_logic_vector(1,12) then
                  if hd_zsd = '0' then
                     state <= sd_eav1;
                     state_count <= "XXXXXXXXXXXX";
                  else
                     state <= hd_eav1;
                     state_count <= "XXXXXXXXXXXX";
                  end if;
               else
                  state_count <= state_count - 1;
               end if;
            when sd_eav1 =>                           -- End active video words
               state <= sd_eav2;
            when sd_eav2 =>
               state <= h_blank1;
               state_count <= hb1s;
            when hd_eav1 =>
               state <= hd_eav2;
               state_count <= "XXXXXXXXXXXX";
            when hd_eav2 =>
               state <= hd_eav3;
               state_count <= "XXXXXXXXXXXX";
            when hd_eav3 =>
               state <= hd_eav4;
               state_count <= "XXXXXXXXXXXX";
            when hd_eav4 =>
               state <= hd_ln0;
               state_count <= "XXXXXXXXXXXX";
            when hd_ln0 =>                            -- Line number
               state <= hd_ln1;
               state_count <= "XXXXXXXXXXXX";
            when hd_ln1 =>
               state <= h_blank1;
               state_count <= hb1s;
            when h_blank1 =>                          -- H. blanking
               if state_count = conv_std_logic_vector(1,12) then
                  state <= h_blank2;
                  state_count <= hb2s;
               else
                  state_count <= state_count - 1;
               end if;
            when others =>                            -- Never happens
               state <= h_blank2;
               state_count <= hb2s;
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Indicate last sample of active video (at every line)
---------------------------------------------------------------
last_av_sample_generation : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      last_av_sample <= '0';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         last_av_sample <= '0';
      elsif en_i = '1' then
         if state = active_video and state_count = conv_std_logic_vector(2,12) then
            last_av_sample <= '1';
         else
            last_av_sample <= '0';
         end if;
      end if;
   end if;
end process;

last_av_sample_o <= last_av_sample;


---------------------------------------------------------------
-- circular counter with a period on total lines per frame.
-- Counts when en_i = '1'
---------------------------------------------------------------
line_counter : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      line_count <= conv_std_logic_vector(1,11);
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         line_count <= line_start;
      elsif en_i = '1' and last_av_sample = '1' then
         if line_count = lpf then
            line_count <= conv_std_logic_vector(1,11);
         else
            line_count <= line_count + 1;
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Count active video lines. Range 0 to active lpf - 1:
---------------------------------------------------------------
av_line_counter : process (clk_i, zreset_i) begin
   if zreset_i = '0' then
      active_line_count <= conv_std_logic_vector(0,11);
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         active_line_count <= conv_std_logic_vector(0,11);
      elsif en_i = '1' and last_av_sample = '1' then
         if line_count = field1_vblnk_end then     -- first active video line first field
            active_line_count <= conv_std_logic_vector(0,11);
         elsif line_count = field2_vblnk_end then  -- 2nd av line (first line 2nd field)
            active_line_count <= conv_std_logic_vector(1,11);
         else                                      -- active lines and following blanking lines
            if interlaced = '0' then
               active_line_count <= active_line_count + 1;    
            else
               active_line_count <= active_line_count + 2;    
            end if;
         end if;
      end if;
   end if;
end process;

av_line_o <= active_line_count;


---------------------------------------------------------------
-- Generate blanking signals:
---------------------------------------------------------------
f_bit_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      f_bit <= f_start;
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         f_bit <= f_start;
      elsif en_i = '1' and last_av_sample = '1' then
         if line_count = field2_end then
            f_bit <= '0';
         elsif line_count = field1_end then
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
      elsif en_i = '1' and last_av_sample = '1' then
         if line_count = field2_av_end then
            v_bit <= '1';
         elsif line_count = field1_vblnk_end then
            v_bit <= '0';
         elsif line_count = field1_av_end then
            v_bit <= '1';
         elsif line_count = field2_vblnk_end then
            v_bit <= '0';
         end if;
      end if;
   end if;
end process;


h_bit_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      h_bit <= "1111";
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         h_bit <= "1111";
      elsif en_i = '1' then
         if state = sd_sav2 or state = hd_sav4 then
            h_bit(0) <= '0';
         elsif last_av_sample = '1' then
            h_bit(0) <= '1';
         end if;
         h_bit(3 downto 1) <= h_bit(2 downto 0);
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate XYZ word :
---------------------------------------------------------------
fvh_int <= f_bit & v_bit & not h_bit(3);

XYZ_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      XYZ <= (others => '0');
   elsif clk_i'event and clk_i = '1' then
      if en_i = '1' then
         XYZ(9 downto 6) <= '1' & fvh_int;
         XYZ(1 downto 0) <= "00";
         case fvh_int is
            when "000" => XYZ(5 downto 2) <= "0000";
            when "001" => XYZ(5 downto 2) <= "1101";
            when "010" => XYZ(5 downto 2) <= "1011";
            when "011" => XYZ(5 downto 2) <= "0110";
            when "100" => XYZ(5 downto 2) <= "0111";
            when "101" => XYZ(5 downto 2) <= "1010";
            when "110" => XYZ(5 downto 2) <= "1100";
            when others => XYZ(5 downto 2) <= "0001";
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate av_sample_o :
---------------------------------------------------------------
av_sample_generation : process (clk_i, zreset_i)
begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if sav_tick_advanced = '1' then
            av_sample <= conv_std_logic_vector(2**11-1+av_sample_advancing,11);
         else
            av_sample <= av_sample + 1;
         end if; 
      end if;
   end if;
end process;

av_sample_o <= av_sample;


---------------------------------------------------------------
-- Generate sav_tick_advanced :
---------------------------------------------------------------
sav_tick_advanced_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      sav_tick_advanced <= '0';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         sav_tick_advanced <= '0';
      elsif en_i = '1' then
         if state = h_blank2 then
            if hd_zsd = '0' then    -- sd
               if state_count = conv_std_logic_vector(sav_tick_advancing-2,12) then
                  sav_tick_advanced <= '1';
               else
                  sav_tick_advanced <= '0';
               end if;
            else                    -- hd
               if state_count = conv_std_logic_vector(sav_tick_advancing-4,12) then
                  sav_tick_advanced <= '1';
               else
                  sav_tick_advanced <= '0';
               end if;
            end if;   
         else
            sav_tick_advanced <= '0';
         end if; 
      end if;
   end if;
end process;

sav_tick_advanced_o <= sav_tick_advanced;


---------------------------------------------------------------
-- Generate eav_tick_advanced_o :
---------------------------------------------------------------
eav_tick_advanced_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      eav_tick_advanced_o <= '0';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         eav_tick_advanced_o <= '0';
      elsif en_i = '1' then
         if state = active_video then
            if state_count = conv_std_logic_vector(eav_tick_advancing+1,12) then
               eav_tick_advanced_o <= '1';
            else
               eav_tick_advanced_o <= '0';
            end if;
         else
            eav_tick_advanced_o <= '0';
         end if; 
      end if;
   end if;
end process;


---------------------------------------------------------------
-- chroma selecting signal for video mux:
---------------------------------------------------------------
cr_zcb_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      cr_zcb <= '1';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         cr_zcb <= '1';
      elsif en_i = '1' then
         cr_zcb <= not cr_zcb;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Muxing video:
---------------------------------------------------------------
video_muxing : process(zreset_i, clk_i)
begin
   if zreset_i = '0' then
      luma <= luma_black;
      chroma <= chroma_black;
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         luma <= luma_h_blank_i;
         chroma <= chroma_h_blank_i;
      elsif en_i = '1' then
         case state is
            when sd_sav1 =>                           -- Start active video
               luma   <= "0000000000";
               chroma <= "1111111111";
            when sd_sav2 =>
               luma   <= XYZ;
               chroma <= "0000000000";
            when hd_sav1 =>
               luma   <= "1111111111";
               chroma <= "1111111111";
            when hd_sav2 =>
               luma   <= "0000000000";
               chroma <= "0000000000";
            when hd_sav3 =>
               luma   <= "0000000000";
               chroma <= "0000000000";
            when hd_sav4 =>
               luma   <= XYZ;
               chroma <= XYZ;
            when active_video =>                      -- Active video
               if v_bit = '0' then                    -- active range 
                  luma   <= luma_video_i;
                  chroma <= chroma_video_i;
               else
                  luma   <= luma_v_blank_i;
                  chroma <= chroma_v_blank_i;
               end if;   
            when sd_eav1 =>                           -- End of active video
               luma   <= "0000000000";
               chroma <= "1111111111";
            when sd_eav2 =>
               luma   <= XYZ;
               chroma <= "0000000000";
            when hd_eav1 =>
               luma   <= "1111111111";
               chroma <= "1111111111";
            when hd_eav2 =>
               luma   <= "0000000000";
               chroma <= "0000000000";
            when hd_eav3 =>
               luma   <= "0000000000";
               chroma <= "0000000000";
            when hd_eav4 =>
               luma   <= XYZ;
               chroma <= XYZ;
            when hd_ln0 =>                            -- Line number
               luma   <= not line_count(6) & line_count(6 downto 0) & "00";
               chroma <= not line_count(6) & line_count(6 downto 0) & "00";
            when hd_ln1 =>
               luma   <= "1000" & line_count(10 downto 7) & "00";
               chroma <= "1000" & line_count(10 downto 7) & "00";
            when h_blank1 =>
               luma   <= luma_h_blank_i;
               chroma <= chroma_h_blank_i;
            when h_blank2 =>
               luma   <= luma_h_blank_i;
               chroma <= chroma_h_blank_i;
            when others =>                            -- CRC is inserted later
               luma   <= "XXXXXXXXXX";
               chroma <= "XXXXXXXXXX";
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate calc_crc:
---------------------------------------------------------------
calc_crc_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      calc_crc <= '0';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         calc_crc <= '0';
      elsif en_i = '1' then
         if state = hd_ln1 then
            calc_crc <= '0';
         elsif state = hd_sav4 then
            calc_crc <= '1';
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Calculate and insert CRC:
---------------------------------------------------------------
CRC_insertion : crc_inserter
port map (
   clk_i                => clk_i,
   zreset_i             => zreset_i,
   en_i                 => en_i,
   sync_reset_i         => sync_reset,
   calc_crc_i           => calc_crc,
   luma_i               => luma,
   chroma_i             => chroma,
   luma_o               => luma_o,
   chroma_o             => chroma_o
);


---------------------------------------------------------------
-- Generate frame_end :
---------------------------------------------------------------
last_fr_sample_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      frame_end <= '1';
   elsif clk_i'event and clk_i = '1' then
      if sync_reset = '1' then
         frame_end <= '0';
      elsif en_i = '1' then
         if state = h_blank1 and state_count = conv_std_logic_vector(2,12) and line_count = line_start then
            frame_end <= '1';
         else
            frame_end <= '0';
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate look_at_frame_start (timing_change_o) and sync_reset :
---------------------------------------------------------------
frame_start_delayed_1_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      frame_start_delayed_1 <= '0';
      frame_start_delayed_2 <= '0';
   elsif clk_i'event and clk_i = '1' then
      frame_start_delayed_1 <= frame_start_i;
      frame_start_delayed_2 <= frame_start_delayed_1;
   end if;
end process;


look_at_frame_start_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      look_at_frame_start <= '1';
   elsif clk_i'event and clk_i = '1' then
      if frame_start_delayed_2 = '1' then
         if frame_end = '1' and en_i = '1' then
            look_at_frame_start <= '0';
         else
            look_at_frame_start <= '1';
         end if;
      end if;
   end if;
end process;
--look_at_frame_start <= '1';


sync_reset_advanced_1_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      sync_reset_advanced_1 <= '1';
   elsif clk_i'event and clk_i = '1' then
      if look_at_frame_start = '1' then
         sync_reset_advanced_1 <= frame_start_i;
      else
         sync_reset_advanced_1 <= '0';
      end if;
   end if;
end process;
                                   

sync_reset_generation : process (clk_i, zreset_i)
begin
   if zreset_i = '0' then
      sync_reset <= '1';
   elsif clk_i'event and clk_i = '1' then
      sync_reset <= sync_reset_advanced_1;
   end if;
end process;


timing_change_o   <= look_at_frame_start;
fvh_o             <= f_bit & v_bit & h_bit(0);
line_o            <= line_count;
sync_reset_o      <= sync_reset;
cr_zcb_o          <= cr_zcb;
hd_zsd_o          <= hd_zsd;
interlaced_o      <= interlaced;
asl_o             <= asl;
frame_end_o       <= frame_end;

---------------------------------------------------------------
-- Debug:
---------------------------------------------------------------

debug_o(0) <= sync_reset;
debug_o(1) <= frame_start_i;
debug_o(2) <= frame_end;
debug_o(3) <= en_i;

end behavioral;