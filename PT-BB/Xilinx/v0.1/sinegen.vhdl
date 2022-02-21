library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity sinegen is
    Port ( 
        mreset :    in std_logic;
        f74 :       in std_logic;
        cclk :      out std_logic;
        sine :      out std_logic_vector(9 downto 0)
        );
end sinegen;

architecture Behavioral of sinegen is

type sineword_arr is array(15 downto 0) of std_logic_vector(9 downto 0);
signal index :      integer range 0 to 15;
--signal sine_arr :   sineword_arr := ("1000000000","1001100010","1010110101","1011101101",
  --                                   "1100000000","1011101101","1010110101","1001100010",
    --                                 "1000000000","0110011110","0101001011","0100010011",
      --                               "0100000000","0100010011","0101001011","0110011110");

signal sine_arr :   sineword_arr := ("1000000000","1001101110","1011001100","1100001010",
                                     "1100100000","1100001010","1011001100","1001101110",
                                     "1000000000","0110010010","0100110100","0011110110",
                                     "0011100000","0011110110","0100110100","0110010010");

begin

index_count : process( mreset, f74 )
begin
    if mreset = '0' then
        index <= 0;
    elsif f74'event and f74 = '1' then
        if index /= 15 then
            index <= index + 1;
        else
            index <= 0;
        end if;
    end if;            
end process;


make_output : process( mreset, f74 )
begin
    if f74'event and f74 = '0' then
        sine <= sine_arr(index);
    end if;        
end process;

cclk <= f74;

end Behavioral;