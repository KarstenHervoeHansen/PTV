library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-------------------------------------------------------------------------------------
-- v1:      Original kode til Prototype lavet af studerende Jesper Christoffersen 
--          for DK-Audio, som afgangsprojekt november 2002 til marts 2003
-- v2:      19/6 2003 - PS      tilfoejet clamping- og enablesignaler til rekonstruktionsfiltre
-- v3:      23/5 2003 - PS      nyt hieraki => toplevel = Tri_level_sync_generator.vhd
-------------------------------------------------------------------------------------

entity Tri_level_sync_generator is
   Port ( 
        
        reset  : in std_logic;    -- Master Reset fra mainboard
        f27 : in std_logic;       -- 27 MHz masterclock fra mainboard
        clk : in std_logic;       -- Output fra clock multiplier ( 148 MHz )
        clk_out : out std_logic;  -- clocksignal til konverteren ( clk inverteret )
        iclk : in std_logic;        -- input clock til ic6

        ic5_ref : in std_logic;     -- Reference clock output
        ic6_oe1 : in std_logic;     -- Output buffer 1 enable
        ic6_oe2 : in std_logic;     -- Output buffer 2 enable

        tx : out std_logic;       -- RS232 transmit
        rx : in std_logic;        -- RS232 receive            

        f4m : in std_logic;       -- not used
        f8g : in std_logic;       -- not used
        fhm : in std_logic;       -- not used
        fhg : in std_logic;       -- not used
        BBy : in std_logic;       -- not used
        BBx : in std_logic;       -- not used
        
--------- Video clock generator og mulitiplier --------------------------

        IC5_S0  : out std_logic;    -- mode pins
        IC5_S1  : out std_logic;
        IC5_S2  : out std_logic;
        IC5_S3  : out std_logic;

        IC6_S0  : out std_logic;    -- mode pins
        IC6_S1  : out std_logic;
        IC6_S2  : out std_logic;
        IC6_S3  : out std_logic;
          
--------- styresignaler til reconstruction filter --------------------------

        sync_in_1 : out std_logic;      -- aktiverer clamping paa filter output
        sync_in_2 : out std_logic;      -- aktiverer clamping paa filter output
        disable_1 : inout std_logic;    -- filter 1 disable ( aktiv hoej )
        disable_2 : out std_logic;       -- filter 2 disable ( aktiv hoej )

--------- Video DAC ----------------------------------------------

        m1    : in std_logic;           -- mode1 pin
        m2    : in std_logic;           -- mode2 pin

        blank          : out std_logic; -- Sync styresignal til konverteren (altid nul)
        sync           : out std_logic; -- Sync styresignal til konverteren
        sync_t         : out std_logic; -- Sync styresignal til konverteren

        rcr9  : in std_logic;
        rcr8  : in std_logic;
        rcr7  : in std_logic;
        rcr6  : in std_logic;
        rcr5  : in std_logic;
        rcr4  : in std_logic;
        rcr3  : in std_logic;
        rcr2  : in std_logic;
        rcr1  : in std_logic;
        rcr0  : in std_logic;

        gy0  : in std_logic;
        gy1  : in std_logic;
        gy2  : in std_logic;
        gy3  : in std_logic;
        gy4  : in std_logic;
        gy5  : in std_logic;
        gy6  : in std_logic;
        gy7  : in std_logic;
        gy8  : in std_logic;
        gy9  : in std_logic;

        bcb9  : in std_logic;
        bcb8  : in std_logic;
        bcb7  : in std_logic;
        bcb6  : in std_logic;
        bcb5  : in std_logic;
        bcb4  : in std_logic;
        bcb3  : in std_logic;
        bcb2  : in std_logic;
        bcb1  : in std_logic;
        bcb0  : in std_logic;
          
--------- DIP switches ----------------------------------------------
          
        D310  : in std_logic;
        D39  : in std_logic;
        D38  : in std_logic;
        D37  : in std_logic;
        D36  : in std_logic;
        D35  : in std_logic;
        D34  : in std_logic;
        D33  : in std_logic;
        D32  : in std_logic;
        D31  : in std_logic;
          
        D210  : in std_logic;
        D29  : in std_logic;
        D28  : in std_logic;
        D27  : in std_logic;
        D26  : in std_logic;
        D25  : in std_logic;
        D24  : in std_logic;
        D23  : in std_logic;
        D22  : in std_logic;
        D21  : in std_logic;
          
        D110  : in std_logic;
        D19  : in std_logic;
        D18  : in std_logic;
        D17  : in std_logic;
        D16  : in std_logic;
        D15  : in std_logic;
        D14  : in std_logic;
        D13  : in std_logic;
        D12  : in std_logic;
        D11  : in std_logic;
          
--------- test pins ----------------------------------------------

        frame : out std_logic;      -- frame clock (til test formaal)
        line_freq : out std_logic  -- line clock  (til test formaal)

          
          );
end Tri_level_sync_generator;

architecture Behavioral of Tri_level_sync_generator is

component Tri_level_timer
   Port ( 
        reset : in std_logic;       -- Master Reset fra mainboard
        clk   : in std_logic;       -- Output fra clock multiplier ( 148 MHz )
        clk_out        : out std_logic; -- clocksignal til konverteren ( clk inverteret )
            
        IC5_S0  : out std_logic; -- styresignaler til clock synthesizerne
        IC5_S1  : out std_logic;
        IC5_S2  : out std_logic;
        IC5_S3  : out std_logic;

        IC6_S0  : out std_logic;
        IC6_S1  : out std_logic;
        IC6_S2  : out std_logic;
        IC6_S3  : out std_logic;
          
        sync           : out std_logic; -- styresignal til konverteren
        sync_t         : out std_logic; -- styresignal til konverteren
        blank          : out std_logic; -- styresignal til konverteren (altid nul)

--------- rekonstruktionsfiltre -------------------------------------------

        sync_in_1 : out std_logic;      -- aktiverer clamping paa filter output
        sync_in_2 : out std_logic;      -- aktiverer clamping paa filter output
        disable_1 : inout std_logic;    -- filter 1 disable ( aktiv hoej )
        disable_2 : out std_logic;       -- filter 2 disable ( aktiv hoej )

--------- test pins ----------------------------------------------

        frame : out std_logic;          -- frame clock (til test formaal)
        line_freq : out std_logic       -- line clock  (til test formaal)
          
        );
end component;

begin

-------------------------------
-- setup clock synthesizer :
IC5_S0 <= '0'; -- saetter IC5 
IC5_S1 <= '1'; -- til at lave 
IC5_S2 <= '0'; -- 74,25 MHz
IC5_S3 <= '0';
-------------------------------
-- setup clock multiplier :
IC6_S0 <= '1'; -- saetter IC6
IC6_S1 <= '1'; -- til at gange
IC6_S2 <= '1'; -- med faktor 2
IC6_S3 <= '1';
-------------------------------


TriLevelTimer : Tri_level_timer port map (

        reset => reset,
        clk => clk,
        clk_out => clk_out,
        IC5_S0 => IC5_S0,
        IC5_S1 => IC5_S1,
        IC5_S2 => IC5_S2,
        IC5_S3 => IC5_S3,
        IC6_S0 => IC6_S0,
        IC6_S1 => IC6_S1,
        IC6_S2 => IC6_S2,
        IC6_S3 => IC6_S3,
        blank => blank,
        sync => sync,
        sync_t => sync_t,
        sync_in_1 => sync_in_1,
        sync_in_2 => sync_in_2,
        disable_1 => disable_1,
        disable_2 => disable_2,
        frame => frame,
        line_freq => line_freq );

end Behavioral;




