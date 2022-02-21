library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity sd_framegenerator is
    Port (
        clk :           in std_logic;
        mute :          in std_logic;
        sdformat :      in std_logic;
        tsl :           in integer;
        asl :           in integer;
        lpf :           in integer;
        vblank :        in integer;
        field2_start :  in integer;
        luma_sample :   in std_logic_vector(9 downto 0);
        chroma_sample : in std_logic_vector(9 downto 0);
	    video :         out std_logic_vector(9 downto 0);
	    line :          out std_logic_vector(9 downto 0);
	    sample :        out std_logic_vector(11 downto 0)
    );
end sd_framegenerator;

architecture Behavioral of sd_framegenerator is

signal linecount : std_logic_vector(9 downto 0);
signal samplecount : std_logic_vector(11 downto 0);
signal xyz : std_logic_vector(9 downto 0);
signal field,vbit,hbit : std_logic;
signal parity : std_logic_vector(3 downto 0);

begin


process ( mute, clk ) begin
	if mute = '0' then
		samplecount <= conv_std_logic_vector(asl,12);
		linecount <= conv_std_logic_vector(1,10);
		sample  <= conv_std_logic_vector(asl,12);
		line <= conv_std_logic_vector(0,10);
	elsif clk'event and clk = '1' then
		sample <= samplecount;
		line <= linecount;
		if samplecount < tsl-1 then
			samplecount <= samplecount+1;
			if samplecount = asl-1 then
				if linecount < lpf then
					linecount <= linecount + 1;
				else
					linecount <= conv_std_logic_vector(1,10);
				end if;
			end if;
		else 
			samplecount <= conv_std_logic_vector(0,12);
		end if;
	end if;
end process;

process (mute,clk) begin
	if mute = '0' then
		video <= "0000000000";
	elsif clk'event and clk='1' then
		if samplecount = asl or samplecount = tsl-4 then	-- TRS interval
			video <= "1111111111";
		elsif ( (samplecount = asl+1) or 
                (samplecount = asl+2) or 
                (samplecount = tsl-3) or 
                (samplecount = tsl-2) ) then
			video <= "0000000000";
		elsif   samplecount = asl+3 or 
                samplecount = tsl-1 then
			video <= xyz;
		elsif samplecount > asl+3 then	-- horizontal blanking interval
 	    	if samplecount(0) = '1' then 
				video <= "0001000000"; -- 0x040
			else
				video <= "1000000000"; -- 0x200
			end if;
		elsif linecount <= vblank or ( linecount > field2_start and linecount <= field2_start + vblank ) then -- vertical blanking interval
 	    	if samplecount(0) = '1' then 
				video <= "0001000000"; -- 0x040
			else
				video <= "1000000000"; -- 0x200
			end if;
		else -- active video field
 	    	if samplecount(0) = '1' then 
				video <= luma_sample; --  Y
			else
				video <= chroma_sample; --  Cb or Cr
			end if;
		end if;
	end if;
end process;

process (mute,clk) begin
	if mute = '0' then
		vbit <= '1';
		field <= '1';
	elsif clk'event and clk = '1' then
		if sdformat = '0' then
			if linecount = 1 or linecount = 264 then
				vbit <= '1';
			elsif linecount = 20 or linecount = 283 then
				vbit <= '0';
			end if;
		else
			if linecount = 624 or linecount = 311 then
				vbit <= '1';
			elsif linecount = 336 or linecount = 23 then
				vbit <= '0';
			end if;
		end if;
		if sdformat = '0' then
			if linecount = 4 then
				field <= '0';
			elsif linecount = 266 then
				field <= '1';
			end if;
		else
			if linecount = 1 then
				field <= '0';
			elsif linecount = 313  then
				field <= '1';
			end if;
		end if;
		if samplecount > asl+3 then 
			hbit <= '0';
		else
			hbit <= '1';
		end if;
	end if;
end process;

with xyz(8 downto 6) select
    parity <= "0000" when "000",
            "1101" when "001",
            "1011" when "010",
            "0110" when "011",
            "0111" when "100",
            "1010" when "101",
            "1100" when "110",
            "0001" when others;

xyz <= '1' & field & vbit & hbit & parity & "00";

end Behavioral;