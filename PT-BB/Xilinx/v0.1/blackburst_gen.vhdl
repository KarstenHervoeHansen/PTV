library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity blackburst_gen is
   Port ( 
--------- til/fra Mainframe -------------------------------------------------------------------
        mreset :    in std_logic;       -- Master Reset fra mainboard
        f27 :       in std_logic;       -- 27 MHz masterclock fra mainboard
        f1484 :     in std_logic;       -- alternativ indgang for 148.35 MHz clock
        f1485 :     in std_logic;       -- alternativ indgang for 148.5 MHz clock
--        f4m :       in std_logic;       -- 1/4 x Vertikal sync - 625 lines SD format
--        fhm :       in std_logic;       -- Horisontal sync
--        f8g :       in std_logic;       -- 1/8 x Vertikal sync - 525 lines SD format
--        fhg :       in std_logic;       -- Horisontal sync
        res1 :      out std_logic;      -- tsg outputs OK respons 1
        res2 :      out std_logic;      -- respons 2 ( not used - tied to vcc )
      
--------- til/fra CPU -------------------------------------------------------------------------
--        sck :       in std_logic;       -- serial clock
--        mosi :      in std_logic;       -- Master Out Serial data In
--        cs1 :       in std_logic;       -- chip select tsg1 (nss)
--        cs2 :       in std_logic;       -- chip select tsg2 (port2 bit 5)
--        cs3 :       in std_logic;       -- chip select tsg3 (port 1 bit 7)
--        miso :      in std_logic;       -- not used
--        p0 :        in std_logic_vector(7 downto 2); -- port 0
--        p1 :        in std_logic_vector(6 downto 0); -- port 1

--------- til rekonstruktions filtre -----------------------------------------------------------
--        clmp1 :      out std_logic;      -- til clamping puls
--        clmp2 :      out std_logic;
--        clmp3 :      out std_logic;
--        clmp4 :      out std_logic;

--------- TSG outputs --------------------------------------------------------------------------
--        tsg1_out :      out std_logic_vector(3 downto 0);   -- 4 bit til DA converter
--        tsg2_out :      out std_logic_vector(3 downto 0);
--        tsg3_out :      out std_logic_vector(3 downto 0);
--        tsg_out4 :      out std_logic_vector(3 downto 0);

--------- Output level check -------------------------------------------------------------------
--        tsg1_lvl :       in std_logic;                       -- signaler fra komparatorer
--        tsg2_lvl :       in std_logic;
--        tsg3_lvl :       in std_logic;
--        tsg4_lvl :       in std_logic;

--------- til THS8135 --------------------------------------------------------------------------
        cclk :      inout std_logic;                      -- clock input til converter
        dacm1 :     out std_logic;                      -- mode pin 1
        dacm2 :     out std_logic;                      -- mode pin 2
        rcr :       out std_logic_vector(9 downto 0);    -- (red/red-chroma)     ( 2 MSB = [tsg13; tsg12] )
        gy :        inout std_logic_vector(9 downto 0);    -- (green/luma)         ( gy = [tsg2(1 downto 0); tsg3(3 downto 0); tsg4(3 downto 0)] )
        bcb :       out std_logic_vector(9 downto 0);    -- (blue/blue-chroma)
        sync :      out std_logic;
        sync_t :    out std_logic;
        blank :     out std_logic;

--------- board interconnect -------------------------------------------------------------------
-- sel(x) = clock select f743 OR f742 for tsg number x
-- st(x) = sync_t tsg number x
-- sy(x) = sync tsg number x
        sel0 :      out std_logic_vector(7 downto 0);
        st0 :       out std_logic_vector(7 downto 0);
        sy0 :       out std_logic_vector(7 downto 0);
--        f742 :      out std_logic;
--        f743 :      out std_logic;

--------- testpoints ( D2, D3, D4 LEDs ) -------------------------------------------------------
        led1 :      out std_logic;  -- forbundet til en noconnect på xc3s50
        led2 :      out std_logic;  -- forbundet til en noconnect på xc3s50
        led3 :      inout std_logic

        -- NBNB : alle udkommaterede signaler er ogsaa udkommateret i UCF filen
        );
end blackburst_gen;

architecture Behavioral of blackburst_gen is

signal f74 :        std_logic := '0';                  -- f148/2
signal f148 :       std_logic;
signal sample :     integer range 0 to 2048;
signal line :       integer range 0 to 1024;

component frame_timer
    Port(
        mreset :    in std_logic;
        f74 :       in std_logic;
        sample :    inout integer range 0 to 2048;
        line :      inout integer range 0 to 1024
    );
end component;

component bilevel_timer
    Port(
        mreset :        in std_logic;
        f74 :           in std_logic;
        sample :        in integer range 0 to 2048;
        line :          in integer range 0 to 1024;
        sync :          out std_logic;
        sync_t :        out std_logic;
        blank :         out std_logic
    );
end component;

component sinegen
    Port(
        mreset :        in std_logic;
        f74 :           in std_logic;
        cclk :          out std_logic;
        sine :          out std_logic_vector(9 downto 0)
    );
end component;


begin

frametimer : frame_timer port map(
    mreset  => mreset,
    f74     => f74,
    sample  => sample,
    line    => line
    );

bileveltimer : bilevel_timer port map(
    mreset  => mreset,
    f74     => f74,
    sample  => sample,
    line    => line,
    sync    => sync,
    sync_t  => sync_t,
    blank   => blank
    );

Sinusgenerator : sinegen port map(
    mreset  => mreset,
    f74     => f74,
    cclk    => cclk,
    sine    => gy
    );

f148 <= f1484;
make_f74 : process( mreset, f148 )
begin
    if mreset = '0' then
        f74 <= '0';
    elsif f148'event and f148 = '1' then
        f74 <= not f74;
    end if;
end process;

-- setup converter :
dacm1   <= '0';
dacm2   <= '0';
--sync    <= '1';
--sync_t  <= '0';
--blank   <= '1';


-- set test points :
led1 <= cclk;     
led2 <= mreset;
led3 <= gy(4);

res1 <= '1';
res2 <= '1';

end Behavioral;
