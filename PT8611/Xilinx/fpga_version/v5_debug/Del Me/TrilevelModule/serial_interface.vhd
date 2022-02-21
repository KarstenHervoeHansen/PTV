library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity serial_interface is
   Port(
      mreset_i             : in std_logic;                          -- master reset
      cs_i                 : in std_logic;                          -- chip select
      sck_i                : in std_logic;                          -- serial clock
      mosi_i               : in std_logic;                          -- master out serial data in
      clk_i                : in std_logic;                          -- video clock in for reclocking of parameters

      spl_div_o            : out std_logic_vector(9 downto 0);      -- total samples pr line divided by 10 or 11
      lpf_o                : out std_logic_vector(10 downto 0);     -- total lines per frame
      div10_11_o           : out std_logic;                         -- divider f148 med 10 hvis lav ( 720 liniers format ), 11 hvis høj ( 1035 og 1080 liniers format )
      framerate_x2_o       : out std_logic;                         -- (active LOW) signalerer dobbelt framerate formater : 1920x1080/60P, 1920x1080/59.94P og 1920x1080/50P
      sync_mode_o          : out std_logic_vector(1 downto 0);      -- 0X=freerunning, 10=genlocked to f8g, 11= genlocked to f4m
      fr_ref_sel_o         : out std_logic_vector(1 downto 0);      -- angiver hvilken vertikal reference, der skal synces til i tilfælde af freerunnning formater: 00 = ref30, 01 = not valid, 10 = ref24, 11 = ref2398
      scan_method_o        : out std_logic;                       
      sysclk_sel_o         : out std_logic;                         -- f148 = 148.35 MHz or 148.5 MHz
      phasedelay_o         : out std_logic_vector(23 downto 0)      -- delay from sync reference
   );
end serial_interface;


architecture Behavioral of serial_interface is


signal bitptr                : integer range 0 to 63;
signal lpf_msb               : std_logic;                           -- total lines per frame bliver afgjort udfra en bit
signal spl_div               : std_logic_vector(9 downto 0);
signal scan_method           : std_logic;
signal sysclk_sel            : std_logic;
signal sync_mode             : std_logic_vector(1 downto 0);
signal phasedelay            : std_logic_vector(23 downto 0);
signal lpf                   : std_logic_vector(10 downto 0);
signal framerate_x2          : std_logic;
signal fr_ref_sel            : std_logic_vector(1 downto 0);

signal param_valid           : std_logic;

constant default_phasedelay  : std_logic_vector(23 downto 0) := "010010111000011111011100";   -- fast debug delay
constant default_sysclk_sel  : std_logic := '1';                                              -- system clock = 148,35 MHz
constant default_sync_mode   : std_logic_vector(1 downto 0) := "01";
constant default_spl_div     : std_logic_vector(9 downto 0) := "0110001111";                  -- 4400 samples per linie
constant default_scan_method : std_logic := '1';                                              -- prog = 0, interlaced = 1
constant default_lpf_msb     : std_logic := '1';                                              -- ll25 linier


-------------------------------------------------------------------------------------------------
-- debug phasedelays :
-- "010010111000011111011100" = 1920x1080/59.94I og 1920x1080/29.97P   (4400*1125-20 = 4949980)
-- "010110101010001100001100" = 1920x1080/50I og 1920x1080/25P         (5280*1125-20 = 5939980)
-- "001001011100001111100100" = 1280x720/59.94P                        (3300*750-20  = 2747980)
-- "001011010101000101111100" = 1280x720/50P                           (3960*750-20  = 2969980)

-- debug spl_div :
-- "0110001111";    (4400 samples per linie)
-- "0011011011";    (2200 samples per linie)
-------------------------------------------------------------------------------------------------


begin


make_bitptr : process(cs_i, sck_i)
-- bit pointer dirigerer mosi til relevant register
begin
   if cs_i = '1' then
      bitptr <= 0;
   elsif sck_i'event and sck_i = '1' then
      bitptr <= bitptr + 1;
   end if;
end process;


get_format : process(mreset_i, sck_i)
begin
   if mreset_i = '0' then
      phasedelay <= default_phasedelay;   
      sysclk_sel <= default_sysclk_sel;                          
      sync_mode <= default_sync_mode;
      spl_div <= default_spl_div;                    
      scan_method <= default_scan_method;                         
      lpf_msb <= default_lpf_msb;                     -- ll25 linier
   elsif sck_i'event and sck_i = '0' then
      if cs_i = '0' then
         case bitptr is
            when 9 =>  phasedelay(23)   <= mosi_i;
            when 10 => phasedelay(22)   <= mosi_i;
            when 11 => phasedelay(21)   <= mosi_i;
            when 12 => phasedelay(20)   <= mosi_i;
            when 13 => phasedelay(19)   <= mosi_i;
            when 14 => phasedelay(18)   <= mosi_i;
            when 15 => phasedelay(17)   <= mosi_i;
            when 16 => phasedelay(16)   <= mosi_i;
            when 17 => phasedelay(15)   <= mosi_i;
            when 18 => phasedelay(14)   <= mosi_i;
            when 19 => phasedelay(13)   <= mosi_i;
            when 20 => phasedelay(12)   <= mosi_i;
            when 21 => phasedelay(11)   <= mosi_i;
            when 22 => phasedelay(10)   <= mosi_i;
            when 23 => phasedelay(9)    <= mosi_i;
            when 24 => phasedelay(8)    <= mosi_i;
            when 25 => phasedelay(7)    <= mosi_i;
            when 26 => phasedelay(6)    <= mosi_i;
            when 27 => phasedelay(5)    <= mosi_i;
            when 28 => phasedelay(4)    <= mosi_i;
            when 29 => phasedelay(3)    <= mosi_i;
            when 30 => phasedelay(2)    <= mosi_i;
            when 31 => phasedelay(1)    <= mosi_i;
            when 32 => phasedelay(0)    <= mosi_i;
            when 34 => sysclk_sel       <= mosi_i; 
            when 35 => sync_mode(1)     <= mosi_i;
            when 36 => sync_mode(0)     <= mosi_i;
            when 39 => spl_div(9)       <= mosi_i;
            when 40 => spl_div(8)       <= mosi_i;
            when 41 => spl_div(7)       <= mosi_i;
            when 42 => spl_div(6)       <= mosi_i;
            when 43 => spl_div(5)       <= mosi_i;
            when 44 => spl_div(4)       <= mosi_i;
            when 45 => spl_div(3)       <= mosi_i;
            when 46 => spl_div(2)       <= mosi_i;
            when 47 => spl_div(1)       <= mosi_i;
            when 48 => spl_div(0)       <= mosi_i;
            when 52 => scan_method      <= mosi_i;
            when 54 => lpf_msb          <= mosi_i; -- for at spare logik, kigges der kun paa MSB af LinesprFrame parameteren,
            when others => null;                 -- da der i de understoettede formater kun er mulighed for to vaerdier    
         end case;                               -- ( 750 lines eller 1125 )                                               
      end if;
   end if;        
end process;


------------------------------------------------------------------
-- Asyncronious parameter mapping:
------------------------------------------------------------------

-- lines pr frame afgøres pt på et enkelt bit :    (Changed from 749 and 1124 to 750 and 1125!)
with lpf_msb select
    lpf <= conv_std_logic_vector(750,11) when '0',
           conv_std_logic_vector(1125,11) when others;
framerate_x2 <= ( spl_div(9) or spl_div(5) ) or scan_method or not lpf_msb;
fr_ref_sel <= spl_div(5) & sysclk_sel;


-------------------------------------------------------------------
-- Reclocking of parameters:
-------------------------------------------------------------------
Param_reclocking : process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      phasedelay_o   <= phasedelay;
      sysclk_sel_o   <= sysclk_sel;
      sync_mode_o    <= sync_mode; 
      spl_div_o      <= spl_div;   
      scan_method_o  <= scan_method;
      div10_11_o     <= lpf_msb;  
      lpf_o          <= lpf;       
      framerate_x2_o <= framerate_x2;
      fr_ref_sel_o   <= fr_ref_sel;
      param_valid    <= '0';
   elsif clk_i'event and clk_i = '1' then
      param_valid <= cs_i;
      if param_valid = '1' then
         phasedelay_o   <= phasedelay;
         sysclk_sel_o   <= sysclk_sel;
         sync_mode_o    <= sync_mode; 
         spl_div_o      <= spl_div;   
         scan_method_o  <= scan_method;
         div10_11_o     <= lpf_msb;  
         lpf_o          <= lpf;       
         framerate_x2_o <= framerate_x2;
         fr_ref_sel_o   <= fr_ref_sel;
      end if;
   end if;
end process;

end Behavioral;