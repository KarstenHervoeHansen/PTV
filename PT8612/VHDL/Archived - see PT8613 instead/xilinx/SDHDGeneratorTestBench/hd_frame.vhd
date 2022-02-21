library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity hd_frame is 
   port ( 
	 clk_i         : in std_logic;                       -- sample clock
      mute_i        : in std_logic;                       -- active low
      -- video registers :
      hdformat_i    : in std_logic_vector(3 downto 0);    -- SMPTE 240M, 274M or 296M
      -- active video from video_sm :
  	 act_y_i       : in std_logic_vector(9 downto 0);
      act_cb_i      : in std_logic_vector(9 downto 0);
      act_cr_i      : in std_logic_vector(9 downto 0);
      -- outputs for reading new format data :
      endof_frame_o : out std_logic;
      -- outputs for video_sm :
	 sav_o         : out std_logic;                      -- start of active video
	 fvh_o         : out std_logic_vector(2 downto 0);   -- Field, Vert-blank and Horz-blank bits
      line_clk_o    : inout std_logic;                    -- total line clock
      -- parallel video out :
	 luma_o        : out std_logic_vector(9 downto 0);
	 chroma_o      : out std_logic_vector(9 downto 0)
   );
end hd_frame;

architecture Behavioral of hd_frame is

component hd_framegenerator is
   port ( 
	 clk            : in std_logic;
      mute           : in std_logic;                       -- mutes generator when 0
      tsl            : in std_logic_vector(11 downto 0);   -- total samples per line
      asl            : in std_logic_vector(11 downto 0);   -- active samples per line
      lpf            : in std_logic_vector(10 downto 0);   -- lines per frame
      field1_vblnk0  : in std_logic_vector(10 downto 0);   -- vertical blanking borders
      field2_vblnk1  : in std_logic_vector(10 downto 0);
      field2_vblnk0  : in std_logic_vector(10 downto 0);
      field1_vblnk1  : in std_logic_vector(10 downto 0);
      field2_start   : in std_logic_vector(9 downto 0);
      sav            : out std_logic;
      line_clk       : inout std_logic;
      sample         : inout integer;    -- output to videogenerator
      line           : inout integer;    -- output to videogenerator
      fvh_out        : out std_logic_vector(2 downto 0); -- blanking bits
      endof_frame    : out std_logic;
      y              : in std_logic_vector(9 downto 0);     -- input from videogenerator
      cb             : in std_logic_vector(9 downto 0);     -- input from videogenerator
      cr             : in std_logic_vector(9 downto 0);         
	 luma           : out std_logic_vector(9 downto 0);   -- output to scrambler
	 chroma         : out std_logic_vector(9 downto 0);   -- output to scrambler
      -- debug :
      tp             : out std_logic
   );
end component;

-- format defining signals :
signal tsl           : std_logic_vector(11 downto 0);  -- total samples per line
signal asl           : std_logic_vector(11 downto 0);  -- active samples per line
signal lpf           : std_logic_vector(10 downto 0);  -- lines per frame
signal field1_vblnk0 : std_logic_vector(10 downto 0);  -- 
signal field2_vblnk1 : std_logic_vector(10 downto 0);
signal field2_vblnk0 : std_logic_vector(10 downto 0);
signal field1_vblnk1 : std_logic_vector(10 downto 0);
signal field2_start  : std_logic_vector(9 downto 0);
-------------------------------------------------------
signal sample        : integer;
-- debugging signals :
signal line          : integer;
signal tp            : std_logic;
signal loadval       : integer;
signal statecnt      : integer;
signal horz          : integer;
signal vert          : integer;
signal activeline    : integer;

begin


hdframe : hd_framegenerator
   port map(
      clk            => clk,
      mute           => mute,
      tsl            => tsl,
      asl            => asl,
      lpf            => lpf,
      field1_vblnk0  => field1_vblnk0,
      field2_vblnk1  => field2_vblnk1,
      field2_vblnk0  => field2_vblnk0,
      field1_vblnk1  => field1_vblnk1,
      field2_start   => field2_start,
      sample         => sample,
      sav            => sav,
      line_clk       => line_clk,
      line           => line,
      fvh_out        => fvh,
      endof_frame    => endof_frame,
      y              => act_y,
      cb             => act_cb,
      cr             => act_cr,
	 luma           => luma,
	 chroma         => chroma,
      tp             => tp
   );

-- hdformat table :
-- 0000		1080i/30 & 1080i/29.97
-- 0001		1080i/25
-- 0010		1080p/30
-- 0011		1080p/25
-- 0100		1080p/24 & 1080p/23.98
-- 0101		1035i/30 & 1035i/29.97
-- 0110		720p/60 & 720p/59.94
-- 0111		720p/50
-- 1000		720p/30 & 720p/29.97
-- 1001		720p/25
-- 1010		720p/24 & 720p/23.98

select_format_parameters : process( hdformat )
begin
   case hdformat is
      when "0000" =>
         -- 1080i/30 and 1080i/29.97
         tsl <= conv_std_logic_vector(2200,12);
         asl <= conv_std_logic_vector(1920,12);
         lpf <= conv_std_logic_vector(1125,11);
         field1_vblnk0 <= conv_std_logic_vector(21,11);
         field2_vblnk1 <= conv_std_logic_vector(561,11);
         field2_vblnk0 <= conv_std_logic_vector(584,11);
         field1_vblnk1 <= conv_std_logic_vector(1124,11);
         field2_start <= conv_std_logic_vector(563,10);
      when "0001" =>
         -- 1080i/25
         tsl <= conv_std_logic_vector(2640,12);
         asl <= conv_std_logic_vector(1920,12);
         lpf <= conv_std_logic_vector(1125,11);
         field1_vblnk0 <= conv_std_logic_vector(21,11);
         field2_vblnk1 <= conv_std_logic_vector(561,11);
         field2_vblnk0 <= conv_std_logic_vector(584,11);
         field1_vblnk1 <= conv_std_logic_vector(1124,11);
         field2_start <= conv_std_logic_vector(563,10);
      when "0010" =>
         -- 1080p/30
         tsl <= conv_std_logic_vector(2200,12);
         asl <= conv_std_logic_vector(1920,12);
         lpf <= conv_std_logic_vector(1125,11);
         field1_vblnk0 <= conv_std_logic_vector(42,11);
         field2_vblnk1 <= (others => '0');
         field2_vblnk0 <= (others => '0');
         field1_vblnk1 <= conv_std_logic_vector(1122,11);
         field2_start <= (others => '0');
      when "0011" =>
         -- 1080p/25
         tsl <= conv_std_logic_vector(2640,12);
         asl <= conv_std_logic_vector(1920,12);
         lpf <= conv_std_logic_vector(1125,11);
         field1_vblnk0 <= conv_std_logic_vector(42,11);
         field2_vblnk1 <= (others => '0');
         field2_vblnk0 <= (others => '0');
         field1_vblnk1 <= conv_std_logic_vector(1122,11);
         field2_start <= (others => '0');
      when "0100" =>
         -- 1080p/24 and 1080p/23.98
         tsl <= conv_std_logic_vector(2750,12);
         asl <= conv_std_logic_vector(1920,12);
         lpf <= conv_std_logic_vector(1125,11);
         field1_vblnk0 <= conv_std_logic_vector(42,11);
         field2_vblnk1 <= (others => '0');
         field2_vblnk0 <= (others => '0');
         field1_vblnk1 <= conv_std_logic_vector(1122,11);
         field2_start <= (others => '0');
      when "0101" =>
         -- 1035i/60 and 1035i/59.94
         tsl <= conv_std_logic_vector(2200,12);
         asl <= conv_std_logic_vector(1920,12);
         lpf <= conv_std_logic_vector(1125,11);
         field1_vblnk0 <= conv_std_logic_vector(41,11);
         field2_vblnk1 <= conv_std_logic_vector(558,11);
         field2_vblnk0 <= conv_std_logic_vector(603,11);
         field1_vblnk1 <= conv_std_logic_vector(1121,11);
         field2_start <= conv_std_logic_vector(563,10);
      when "0110" =>
         -- 720p/60 and 720p/59.94
         tsl <= conv_std_logic_vector(1650,12);
         asl <= conv_std_logic_vector(1280,12);
         lpf <= conv_std_logic_vector(750,11);
         field1_vblnk0 <= conv_std_logic_vector(26,11);
         field2_vblnk1 <= (others => '0');
         field2_vblnk0 <= (others => '0');
         field1_vblnk1 <= conv_std_logic_vector(746,11);
         field2_start <= (others => '0');
      when "0111" =>
         -- 720p/50
         tsl <= conv_std_logic_vector(1980,12);
         asl <= conv_std_logic_vector(1280,12);
         lpf <= conv_std_logic_vector(750,11);
         field1_vblnk0 <= conv_std_logic_vector(26,11);
         field2_vblnk1 <= (others => '0');
         field2_vblnk0 <= (others => '0');
         field1_vblnk1 <= conv_std_logic_vector(746,11);
         field2_start <= (others => '0');
      when "1000" =>
         -- 720p/30 and 720p/29.97
         tsl <= conv_std_logic_vector(3300,12);
         asl <= conv_std_logic_vector(1280,12);
         lpf <= conv_std_logic_vector(750,11);
         field1_vblnk0 <= conv_std_logic_vector(26,11);
         field2_vblnk1 <= (others => '0');
         field2_vblnk0 <= (others => '0');
         field1_vblnk1 <= conv_std_logic_vector(746,11);
         field2_start <= (others => '0');
      when "1001" =>
         -- 720p/25
         tsl <= conv_std_logic_vector(3960,12);
         asl <= conv_std_logic_vector(1280,12);
         lpf <= conv_std_logic_vector(750,11);
         field1_vblnk0 <= conv_std_logic_vector(26,11);
         field2_vblnk1 <= (others => '0');
         field2_vblnk0 <= (others => '0');
         field1_vblnk1 <= conv_std_logic_vector(746,11);
         field2_start <= (others => '0');
      when "1010" =>
         -- 720p/24 and 720p/23.98
         tsl <= conv_std_logic_vector(4125,12);
         asl <= conv_std_logic_vector(1280,12);
         lpf <= conv_std_logic_vector(750,11);
         field1_vblnk0 <= conv_std_logic_vector(26,11);
         field2_vblnk1 <= (others => '0');
         field2_vblnk0 <= (others => '0');
         field1_vblnk1 <= conv_std_logic_vector(746,11);
         field2_start <= (others => '0');
      when others =>
         -- debug format
         tsl <= conv_std_logic_vector(2200,12);
         asl <= conv_std_logic_vector(1920,12);
         lpf <= conv_std_logic_vector(750,11);
         field1_vblnk0 <= conv_std_logic_vector(21,11);
         field2_vblnk1 <= conv_std_logic_vector(61,11);
         field2_vblnk0 <= conv_std_logic_vector(84,11);
         field1_vblnk1 <= conv_std_logic_vector(124,11);
         field2_start <= conv_std_logic_vector(64,10);
   end case;
end process;            

end Behavioral;