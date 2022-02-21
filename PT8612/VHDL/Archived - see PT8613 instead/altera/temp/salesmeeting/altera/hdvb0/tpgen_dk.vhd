library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity tpgen_dk is
    Port ( 
        reset_n : in std_logic;
        clk : in std_logic;
        pal_nntsc : in std_logic;
        txsrc : in std_logic_vector(2 downto 0);
        txrate : in std_logic_vector(1 downto 0);
        data_out : out std_logic_vector(9 downto 0)
        );
end tpgen_dk;

architecture Behavioral of tpgen_dk is

component sdsdi_generator
	Port (
		reset_n :   in std_logic;       -- active low reset
        clk :       in std_logic;       -- transmit clock
        pal_nntsc : in std_logic;		-- pal/ntsc select
		txsrc :     in std_logic_vector(2 downto 0);    -- transmit source select input
		txrate :    in std_logic_vector(1 downto 0);    -- transmit rate input
		data_out :  out std_logic_vector(9 downto 0)    -- video data out
	);
end component;

begin

sdi : sdsdi_generator port map(
    reset_n => reset_n,
    clk => clk,
    pal_nntsc => pal_nntsc,
    txsrc => txsrc,
    txrate => txrate,
    data_out => data_out
    );

end Behavioral;