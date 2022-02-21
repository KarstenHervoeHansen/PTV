library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity altddio_out is
    Port ( dataout : out std_logic_vector(10 downto 0);
           outclock : in std_logic;
           oe : in std_logic;
           datain_h : in std_logic_vector(10 downto 0);
           datain_l : in std_logic_vector(10 downto 0));
end altddio_out;

architecture Behavioral of altddio_out is
signal data_temph :  std_logic_vector(10 downto 0);
signal data_templ :  std_logic_vector(10 downto 0);
begin  

altddioo_h: process(outclock)
begin
	if outclock'event and outclock = '1' then
			data_temph <= datain_h;
--	else
--		if outclock'event and outclock = '0' then
--				dataout <= datain_l;
--	end if;
	end if;
end process;

altddioo_l: process(outclock)
begin		
	if outclock'event and outclock = '0' then
			data_templ <= datain_l;
	end if;
end process;

output_data: process(data_templ, data_temph, outclock, oe)
begin
	if oe = '0' then
		dataout <= (others =>'0');
	else
		if outclock ='1' then

			dataout <= data_temph;
		else
			dataout <= data_templ;
     	end if;
     end if;
end process;

end Behavioral;
