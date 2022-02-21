library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity luma_ramp_generator is
   port (
      clk_i                : in std_logic;
      en_i                 : in std_logic;

      -- system parameters:
      system_i             : in std_logic_vector(7 downto 0);

      -- frame control signals:
      sav_tick_advanced_i  : in std_logic;

      -- Video output:
      luma_o               : out std_logic_vector(11 downto 0)
   );
end luma_ramp_generator;


architecture behavioral of luma_ramp_generator is

type ramp_state_type is (c_start, black, ramp, white, c_end);
signal ramp_state             : ramp_state_type;

signal step_size              : std_logic_vector(13 downto 0);
signal luma                   : std_logic_vector(13 downto 0);
signal width_black            : std_logic_vector(13 downto 0);

signal start_color_presence   : std_logic_vector(2 downto 0);
signal spl_index              : std_logic_vector(1 downto 0);

constant sav_offset           : integer := 8;  -- horizontal position adjustment


begin


---------------------------------------------------------------
-- Horizontal state counter :
---------------------------------------------------------------
horz_sm : process(clk_i)
begin
   if clk_i'event and clk_i = '1' then
      if en_i = '1' then
         if sav_tick_advanced_i = '1' then
            ramp_state  <= black;
            luma        <= conv_std_logic_vector(1,14);
         else
            case ramp_state is
               when black =>
                  if luma = width_black then
                     ramp_state  <= ramp;
                     luma        <= "00010000000000";               -- Black
                  else
                     luma <= luma + 1;
                  end if;
               when ramp =>
                  if luma = conv_std_logic_vector(16*940,14) then  --100% white
                     ramp_state  <= white;
                     luma <= conv_std_logic_vector(0,14);
                  else
                     luma <= luma + step_size;
                  end if;
               when others => -- white
                  ramp_state  <= white;
                  luma        <= (others => 'X');
            end case;
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- Generate luma_o:
---------------------------------------------------------------
luma_o_generation : process(clk_i)
begin
   if clk_i'event and clk_i = '1' then       
      if en_i = '1' then                     
         case ramp_state is                     
            when c_start =>                     
               luma_o <= "000100000000";     -- black
            when black =>                       
               luma_o <= "000100000000";     -- black
            when ramp =>
               luma_o <= luma(13 downto 2);
            when others =>                   -- white
               luma_o <= conv_std_logic_vector(4*940,12); --100% white
         end case;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- samples per line dependent on system :
-- 720: "00", 1280: "01", 1920: "10"
---------------------------------------------------------------
with system_i select
   spl_index <=      "10" when X"A0",   -- 1080i/30
                     "10" when X"B0",   -- 1080i/29.97
                     "10" when X"82",   -- 1080p/30
                     "10" when X"92",   -- 1080p/29.97
                     "10" when X"83",   -- 1080p/25
                     "10" when X"84",   -- 1080p/24
                     "10" when X"94",   -- 1080p/23.98
                     "10" when X"A2",   -- 1080sF/30
                     "10" when X"B2",   -- 1080sF/29.97
                     "10" when X"A3",   -- 1080sF/25
                     "10" when X"A4",   -- 1080sF/24
                     "10" when X"B4",   -- 1080sF/23.98
                     "01" when X"86",   -- 720p/60
                     "01" when X"96",   -- 720p/59.94
                     "01" when X"87",   -- 720p/50
                     "01" when X"88",   -- 720p/30
                     "01" when X"98",   -- 720p/29.97
                     "01" when X"89",   -- 720p/25
                     "01" when X"8A",   -- 720p/24
                     "01" when X"9A",   -- 720p/23.98
                     "00" when X"60",   -- NTSC 525i/29.97
                     "00" when X"20",   -- PAL 625i/25
                     "10" when others;  -- 1080i/25   X"A1"


---------------------------------------------------------------
-- Parameters dependent on spl_index :
---------------------------------------------------------------
with spl_index select
   step_size            <= conv_std_logic_vector(24,14)                 when "00",     -- 720
                           conv_std_logic_vector(12,14)                 when "01",     -- 1280
                           conv_std_logic_vector(8,14)                  when others;   -- 1920
                                                                        
with spl_index select                                                   
   width_black          <= conv_std_logic_vector(51+16+sav_offset,14)   when "00",     -- 720
                           conv_std_logic_vector(55+2+sav_offset,14)    when "01",     -- 1280
                           conv_std_logic_vector(83+2+sav_offset,14)    when others;   -- 1920
                           

end behavioral;