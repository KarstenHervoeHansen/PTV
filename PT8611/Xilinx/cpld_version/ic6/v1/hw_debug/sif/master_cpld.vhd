library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity master_cpld is
    Port ( 
        -- til/fra mainframe konnektor :
        mreset :    in std_logic;
--        f27 :       in std_logic;   -- 27 Mhz master clock
  --      f4m :       in std_logic;   -- 4 x field rate (NTSC 525/60)
    --    fhm :       in std_logic;   -- NTSC - horizontal sync 
      --  f8g :       in std_logic;   -- 8 x field rate (PAL 625/50)
        --fhg :       in std_logic;   -- PAL - horizontal sync
        bb5_ok :    out std_logic;  -- level check til mainframe
        bb6_ok :    out std_logic;  -- level check til mainframe

        -- IC1, IC2, IC3, IC4 ( clock generator + mulitplier )
        ic1s :      out std_logic_vector(3 downto 0);    -- IC1 mode
        ic1_ref :   out std_logic;                       -- IC1 reference clock - used for test purpose

        ic2s :      out std_logic_vector(3 downto 0);    -- IC2 mode
        ic2_oe1 :   out std_logic;  -- Output Enable IC2
        ic2_oe2 :   out std_logic;  -- Output Enable IC2

        ic3s :      out std_logic_vector(3 downto 0);    -- IC3 mode
--        ic3_ref :   in std_logic;                       -- IC3 reference clock - not used

        ic4s :      out std_logic_vector(3 downto 0);    -- IC4 mode
        ic4_oe1 :   out std_logic;  -- Output Enable IC4
        ic4_oe2 :   out std_logic;  -- Output Enable IC2

--        f7425 :     in std_logic;   -- 74.25 MHz clock fra IC3
  --      f1485 :     in std_logic;   -- 148.5 MHz clock fra IC4
    --    f74176 :    in std_logic;   -- 74.176 MHz clock fra IC3
        f14835 :    inout std_logic;   -- 148.35 MHz clock fra IC2

        -- serial interface :
        cs :        in std_logic;   -- chip select master CPLD
        sck :       in std_logic;   -- Serial Clock
        miso :      in std_logic;   -- Master In Serial Out
        mosi :      in std_logic;   -- Master Out Serial In

        -- CPU parallel ports
--        p0 :        inout std_logic_vector(7 downto 2);
  --      p1 :        inout std_logic_vector(6 downto 3);

        -- forbindelser til TLG-Cpld'er :
        gpbus17 :   in std_logic;   -- gpbus1 MSB
        gpbus16 :   in std_logic;
        gpbus15 :   out std_logic;
        gpbus14 :   out std_logic;
        gpbus13 :   in std_logic;
        gpbus12 :   out std_logic;
        gpbus11 :   in std_logic;
        gpbus10 :   out std_logic;  -- gpbus2 LSB

        gpbus27 :   in std_logic;   -- gpbus2 MSB
        gpbus26 :   in std_logic;
        gpbus25 :   out std_logic;
        gpbus24 :   out std_logic;
        gpbus23 :   in std_logic;
        gpbus22 :   out std_logic;
        gpbus21 :   in std_logic;
        gpbus20 :   out std_logic;  -- gpbus2 LSB

        gpbus37 :   in std_logic;   -- gpbus3 MSB
        gpbus36 :   in std_logic;
        gpbus35 :   out std_logic;
        gpbus34 :   out std_logic;
        gpbus33 :   in std_logic;
        gpbus32 :   out std_logic;
        gpbus31 :   in std_logic;
        gpbus30 :   out std_logic   -- gpbus2 LSB
        );
end master_cpld;

architecture Behavioral of master_cpld is

signal lpf_tsg1 :       integer range 0 to 2047;    -- lines pr frame TSG1
signal lpf_tsg2 :       integer range 0 to 2047;    -- lines pr frame TSG2
signal lpf_tsg3 :       integer range 0 to 2047;    -- lines pr frame TSG3
signal sm_tsg1 :        std_logic;                  -- scan method TSG1
signal sm_tsg2 :        std_logic;                  -- scan method TSG2
signal sm_tsg3 :        std_logic;                  -- scan method TSG3

-- debug :
signal outputid :       std_logic_vector(1 downto 0);
signal lpf_ok :         std_logic;
signal sm_ok :          std_logic;


component serial_interface
    Port( 
        mreset :        in std_logic;   -- master reset - fra mainframe
        cs :            in std_logic;   -- chip select master CPLD
        sck :           in std_logic;   -- Serial Clock
        mosi :          in std_logic;   -- Master Out Serial In
        lpf_tsg1 :      out integer range 0 to 2047;
        sm_tsg1 :       out std_logic;
        lpf_tsg2 :      out integer range 0 to 2047;
        sm_tsg2 :       out std_logic;
        lpf_tsg3 :      out integer range 0 to 2047;
        sm_tsg3 :       out std_logic;
        -- debug :
        outputid :      out std_logic_vector(1 downto 0);
        lpf_ok :        out std_logic;
        sm_ok :         out std_logic
        );
end component;

begin


sif : serial_interface port map(
    mreset          => mreset,
    cs              => cs,
    sck             => sck,
    mosi            => mosi,
    lpf_tsg1        => lpf_tsg1,
    sm_tsg1         => sm_tsg1,
    lpf_tsg2        => lpf_tsg2,
    sm_tsg2         => sm_tsg2,
    lpf_tsg3        => lpf_tsg3,
    sm_tsg3         => sm_tsg3,
    -- debug :
--    lpf_tsg1        => open,
  --  sm_tsg1         => open,
    --lpf_tsg2        => open,
--    sm_tsg2         => open,
  --  lpf_tsg3        => open,
    --sm_tsg3         => open,
    outputid        => outputid
--    lpf_ok          => open,
  --  sm_ok           => open
    );


-- setup ic1 to generate 74.176 MHz :
ic1s <= "0011";

-- setup multiplier ic2 :
ic2s <= "1111"; -- input x 2 
ic2_oe1 <= '1';
ic2_oe2 <= '1';

-- setup ic3 to generate 74.25 MHz :
ic3s <= "0010";

-- setup multiplier ic4 :
ic4s <= "1111"; -- input x 2 
ic4_oe1 <= '1';
ic4_oe2 <= '1';

bb5_ok <= '1';
bb6_ok <= '1';

-- debug :
--f14835 <= sck;
--ic1_ref <= mosi;

f14835 <= outputid(1);
ic1_ref <= outputid(0); 

end Behavioral;
