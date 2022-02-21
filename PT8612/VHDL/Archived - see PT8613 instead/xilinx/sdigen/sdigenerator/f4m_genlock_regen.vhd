library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity f4m_genlock_regen is
    port ( 
	   mreset :       in std_logic;                       -- Master reset
	   clk    :       in std_logic;                       -- sample clock
        f4m    :       in std_logic;
	   f2m    :	   out std_logic
        );
end f4m_genlock_regen;

architecture Behavioral of f4m_genlock_regen is

signal f4m_delayed      : std_logic;
signal f4m_pulse        : std_logic;
signal lookat_f4m_pulse : std_logic;
signal f4m_count        : std_logic_vector(23 downto 0);  -- total samples per line

		
begin


-------------------------------------------------------------
-- Generates pulse at f4m rising edge                      --
-------------------------------------------------------------

f4m_sync_pulse : process (mreset, clk) begin
	if mreset = '0' then
	    f4m_delayed <= '1';
	    f4m_pulse <= '0';
	elsif clk'event and clk = '1' then
		if f4m_delayed = '0' and f4m = '1' then
		     f4m_pulse <= '1';
		else
          	f4m_pulse <= '0';	
		end if;
		f4m_delayed <= f4m;
	end if;
end process;     


--------------------------------------------------------------
-- Circular counter in sync with the f4m_pulse. Tolerant to	--
-- jitter on f4m on +- 1 clock.                             --
--------------------------------------------------------------

f4m_sync_counter : process (mreset, clk) begin
	if mreset = '0' then
		f4m_count <= conv_std_logic_vector(23,24);	    -- N*2+3
		lookat_f4m_pulse <= '1';
	elsif clk'event and clk = '1' then
		if f4m_count = conv_std_logic_vector(0,24) then
			f4m_count <= conv_std_logic_vector(9,24); --conv_std_logic_vector(5939999,23); -- N-1
			lookat_f4m_pulse <= '1';
		elsif lookat_f4m_pulse = '1' and f4m_pulse = '1' then
			f4m_count <= (others => '0');
		else
		   	f4m_count <= f4m_count - 1;
		end if;
		if f4m_count = conv_std_logic_vector(3,24) then
			lookat_f4m_pulse <= '0';
		end if;
	end if;
end process;


--------------------------------------------------------------
-- Makes sync pulses f2m every 2 fields, i.e. two times on 	--
-- a f4m period.									--
--------------------------------------------------------------

f2m_sync_generation : process (mreset, clk) begin
	if mreset = '0' then
		f2m <= '0';
	elsif clk'event and clk = '1' then
		if f4m_count = conv_std_logic_vector(0,23) or f4m_count = conv_std_logic_vector(5,23) then  --0; N/2
			f2m <= '1';
		else
			f2m <= '0';
		end if;
	end if;
end process;

end Behavioral;