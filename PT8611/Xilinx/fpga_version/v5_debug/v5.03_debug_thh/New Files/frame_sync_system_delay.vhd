library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity frame_sync_clk is
	Port(
		mreset_i			: in std_logic;
		sync_i			: in std_logic;
		phasedelay_i	: in std_logic_vector(23 downto 0);	-- 0 to frame length - 1
		f1484_i			: in std_logic;
		f1485_i			: in std_logic;
		frame_sync_o	: out std_logic;
		clk_o				: out std_logic
	);
end frame_sync_clk;

architecture Behavioral of frame_sync_clk is

signal delay_count	: std_logic_vector(23 downto 0);  	-- phasedelay in clk cycles
signal frame_length  : std_logic_vector(23 downto 0);  	-- frame length in clk cycles

---------------------------------------------------------------
-- default phasedelay ( V=0, H=0, T=0.000 )
-- Forklaring/illustration :
--
--                        _   _   _         _   _   _   _   _   _         _   _  
-- clk               	_| |_| |_| |_ ... _| |_| |_| |_| |_| |_| |_ ... _| |_| |_
--								      ___
-- sync_i               _____|   |___ ... _______________________________________
--
-- count indtil delay_count = phasedelay og derefter :
--                                                  ___                   ___
-- frame_sync_o         _____________ ... _________|   |___________ ... _|   |___
--
--                           |<-- phasedelay+2 --->|<----- 1 Frame ----->| 
--

begin

sync_delaying : process (mreset, delay_reset = '0', clk)
begin
	if mreset = '0' or delay_reset = '0' then
		delay_count <= phasedelay_i;
		sync_delayed <= '0';
	elsif clk'event and clk = '1' then
		if sync_i = '1' then
			delay_count <= phasedelay_i;
		end if;
		if delay_count = conv_std_logic_vector(0,24) then
			sync_delayed <= '1';
		end if;
	-- check: lav nedtælling
---------------------------------------------------------------------------	
	end if;
end process;






down_count : process ( start_count, f148 )
begin
    if start_count = '1' then
        delay_count <= "000000000000000000000000";
        sync_delayed <= '0';
        counter_active <= '1';
    elsif f148'event and f148 = '1' then
        
        if counter_active = '1' then
            delay_count(0) <= not delay_count(0);
        else
            delay_count(0) <= '0';  -- counter stopper
        end if;

        if delay_count(0) = '1' then
            delay_count(1) <= not delay_count(1);
        end if;

        if delay_count(1 downto 0) = "11" then
            delay_count(2) <= not delay_count(2);
        end if;

        if delay_count(2 downto 0) = "111" then
            delay_count(3) <= not delay_count(3);
        end if;

        if delay_count(3 downto 0) = "1111" then
            delay_count(4) <= not delay_count(4);
        end if;

        if delay_count(4 downto 0) = "11111" then
            delay_count(5) <= not delay_count(5);
        end if;

        if delay_count(5 downto 0) = "111111" then
            delay_count(6) <= not delay_count(6);
        end if;

        if delay_count(6 downto 0) = "1111111" then
            delay_count(7) <= not delay_count(7);
        end if;

        if delay_count(7 downto 0) = "11111111" then
            delay_count(8) <= not delay_count(8);
        end if;

        if delay_count(8 downto 0) = "111111111" then
            delay_count(9) <= not delay_count(9);
        end if;

        if delay_count(9 downto 0) = "1111111111" then
            delay_count(10) <= not delay_count(10);
        end if;

        if delay_count(10 downto 0) = "11111111111" then
            delay_count(11) <= not delay_count(11);
        end if;

        if delay_count(11 downto 0) = "111111111111" then
            delay_count(12) <= not delay_count(12);
        end if;

        if delay_count(12 downto 0) = "1111111111111" then
            delay_count(13) <= not delay_count(13);
        end if;

        if delay_count(13 downto 0) = "11111111111111" then
            delay_count(14) <= not delay_count(14);
        end if;

        if delay_count(14 downto 0) = "111111111111111" then
            delay_count(15) <= not delay_count(15);
        end if;

        if delay_count(15 downto 0) = "1111111111111111" then
            delay_count(16) <= not delay_count(16);
        end if;

        if delay_count(16 downto 0) = "11111111111111111" then
            delay_count(17) <= not delay_count(17);
        end if;

        if delay_count(17 downto 0) = "111111111111111111" then
            delay_count(18) <= not delay_count(18);
        end if;

        if delay_count(18 downto 0) = "1111111111111111111" then
            delay_count(19) <= not delay_count(19);
        end if;

        if delay_count(19 downto 0) = "11111111111111111111" then
            delay_count(20) <= not delay_count(20);
        end if;

        if delay_count(20 downto 0) = "111111111111111111111" then
            delay_count(21) <= not delay_count(21);
        end if;

        if delay_count(21 downto 0) = "1111111111111111111111" then
            delay_count(22) <= not delay_count(22);
        end if;

        if delay_count(22 downto 0) = "11111111111111111111111" then
            delay_count(23) <= not delay_count(23);
        end if;

        if delay_count = phasedelay then
            sync_delayed <= '1';
            counter_active <= '0';
        else
            sync_delayed <= '0';
        end if;
    end if;
end process;

-- for debugging :
delaycount <= conv_integer(delay_count);
dlycnt_active <= counter_active;

end Behavioral;
