library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-------------------------------------------------------------------------------------
-- v1:      Original kode til Prototype lavet af PEH + studerende Jesper Christoffersen 
--          for DK-Audio, som afgangsprojekt november 2002 til marts 2003
-- v2:      19/6 2003 - PS      tilfoejet clamping- og enablesignaler til rekonstruktionsfiltre
-- v3:      23/5 2003 - PS      nyt hieraki => toplevel = Tri_level_sync_generator.vhd
-------------------------------------------------------------------------------------

entity Tri_level_sync_generator is
   Port ( 
        
        reset :                 in std_logic;      -- Master Reset fra mainboard
        f27 :                   in std_logic;         -- 27 MHz masterclock fra mainboard
        f148 :                  in std_logic;        -- Output fra clock multiplier ( 148 MHz )
        f148_inv :              out std_logic;   -- clocksignal til konverteren ( f148 inverteret )
        f74 :                   in std_logic;         -- input clock til ic6

        tx :                    out std_logic;       -- RS232 transmit
        rx :                    in std_logic;        -- RS232 receive            

        f4m :                   in std_logic;       -- not used
        f8g :                   in std_logic;       -- not used
        fhm :                   in std_logic;       -- not used
        fhg :                   in std_logic;       -- not used
        BBy :                   in std_logic;       -- not used
        BBx :                   in std_logic;       -- not used
        
--------- Video clock generator og mulitiplier --------------------------

        clk_generator_mode  :   out std_logic_vector(3 downto 0);       -- mode pins
        clk_generator_ref :     in std_logic;                           -- Reference clock output
        multiplier_mode  :      out std_logic_vector(3 downto 0);       -- mode pins
        multiplier_oe1 :        in std_logic;                           -- Output buffer 1 enable
        multiplier_oe2 :        in std_logic;                           -- Output buffer 2 enable
          
--------- styresignaler til reconstruction filter --------------------------

        sync_in_1 :             out std_logic;      -- aktiverer clamping paa filter output
        sync_in_2 :             out std_logic;      -- aktiverer clamping paa filter output
        disable_1 :             inout std_logic;    -- filter 1 disable ( aktiv hoej )
        disable_2 :             out std_logic;      -- filter 2 disable ( aktiv hoej )

--------- Video DAC ----------------------------------------------

        m1 :                    in std_logic;       -- mode1 pin
        m2 :                    in std_logic;       -- mode2 pin

        blank :                 out std_logic;      -- Sync styresignal til konverteren (altid nul)
        sync :                  out std_logic;      -- Sync styresignal til konverteren
        sync_t :                out std_logic;      -- Sync styresignal til konverteren
          
--------- DIP switches ----------------------------------------------
          
        D310 :                  in std_logic;
        D39 :                   in std_logic;
        D38 :                   in std_logic;
        D37 :                   in std_logic;
        D36 :                   in std_logic;
        D35 :                   in std_logic;
        D34 :                   in std_logic;
        D33 :                   in std_logic;
        D32 :                   in std_logic;
        D31 :                   in std_logic;
          
        D210 :                  in std_logic;
        D29 :                   in std_logic;
        D28 :                   in std_logic;
        D27 :                   in std_logic;
        D26 :                   in std_logic;
        D25 :                   in std_logic;
        D24 :                   in std_logic;
        D23 :                   in std_logic;
        D22 :                   in std_logic;
        D21 :                   in std_logic;
          
        D110 :                  in std_logic;
        D19 :                   in std_logic;
        D18 :                   in std_logic;
        D17 :                   in std_logic;
        D16 :                   in std_logic;
        D15 :                   in std_logic;
        D14 :                   in std_logic;
        D13 :                   in std_logic;
        D12 :                   in std_logic;
        D11 :                   in std_logic;
          
--------- test pins ----------------------------------------------

        frame :                 out std_logic;      -- frame clock (til test formaal)
        line_freq :             out std_logic       -- line clock  (til test formaal)
          
        );
end Tri_level_sync_generator;

architecture Behavioral of Tri_level_sync_generator is

component Tri_level_timer
   Port ( 
        reset :             in std_logic;       -- start generator pulse - aktiv lav
        f148 :              in std_logic;       -- Output fra clock multiplier ( 148 MHz )
        f148_inv :          out std_logic; -- clocksignal til konverteren ( f148 inverteret )

        scan_method :       in std_logic;                 -- ( 0=interlaced, 1=progressiv )
        cnt_setup :         in integer range 0 to 1023;   -- vaerdi til valg af Video standard        
        samples_pr_line :   in integer range 0 to 8191;   -- * 2 pga oversampling
        lines_pr_frame :    in integer range 0 to 2047;                  
            
        sync :              out std_logic;      -- styresignal til konverteren
        sync_t :            out std_logic;      -- styresignal til konverteren
        blank :             out std_logic;      -- styresignal til konverteren (altid nul)

--------- rekonstruktionsfiltre -------------------------------------------

        sync_in_1 :         out std_logic;      -- aktiverer clamping paa filter output
        sync_in_2 :         out std_logic;      -- aktiverer clamping paa filter output
        disable_1 :         inout std_logic;    -- filter 1 disable ( aktiv hoej )
        disable_2 :         out std_logic;      -- filter 2 disable ( aktiv hoej )

--------- test pins ----------------------------------------------

        frame :             out std_logic;                  -- frame clock (til test formaal)
        line_freq :         out std_logic;                  -- line clock  (til test formaal)
        real_pixel :        inout integer range 0 to 8191;  -- pixel synkron med sync og sync_t
        real_line :         out integer range 0 to 2047     -- linie synkron med sync og sync_t
          
        );
end component;



--------------------------------------------
-- video format registre, der skal tilskrives af CPU :

signal real_pixel :          integer range 0 to 8191;   -- for debug only
signal real_line :           integer range 0 to 2047;   -- for debug only

signal cnt_setup :          integer range 0 to 1023;    -- 10 bit
signal samples_pr_line :    integer range 0 to 8191;    -- 13 bit
signal lines_pr_frame :     integer range 0 to 2047;    -- 11 bit
signal scan_method :        std_logic;                  -- 1 bit ( 0=interlaced, 1=progressiv )  

begin

-------------------------------
-- setup clock synthesizer :
clk_generator_mode <= "0010";
--IC5_S0 <= '0'; -- saetter IC5 
--IC5_S1 <= '1'; -- til at lave 
--IC5_S2 <= '0'; -- 74,25 MHz
--IC5_S3 <= '0';
-------------------------------
-- setup clock multiplier :
multiplier_mode <= "1111";
--IC6_S0 <= '1'; -- saetter IC6
--IC6_S1 <= '1'; -- til at gange
--IC6_S2 <= '1'; -- med faktor 2
--IC6_S3 <= '1';
-------------------------------


-- 1920x1080/I :
--cnt_setup <= 173;                 -- = 2*b - 88 - 1
--scan_method <= '0';               -- 0=interlaced scan, 1=progresive scan
--samples_pr_line <= 4399;        
--lines_pr_frame <= 1124;

-- 1280x720/P :
cnt_setup <= 157;
scan_method <= '1';               -- 0=interlaced scan, 1=progresive scan
samples_pr_line <= 3299;        
lines_pr_frame <= 749;

TriLevelTimer : Tri_level_timer port map (
        reset => reset,
        f148 => f148,
        f148_inv => f148_inv,
        scan_method => scan_method,
        cnt_setup => cnt_setup,
        samples_pr_line => samples_pr_line,
        lines_pr_frame => lines_pr_frame,
        blank => blank,
        sync => sync,
        sync_t => sync_t,
        sync_in_1 => sync_in_1,
        sync_in_2 => sync_in_2,
        disable_1 => disable_1,
        disable_2 => disable_2,
        frame => frame,
        line_freq => line_freq,
        real_pixel => real_pixel,
        real_line => real_line
        );


end Behavioral;
