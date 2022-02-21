use work.pack_reverse_bit_order.all;
library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;
USE work.pack_crc.all;

entity hd_framegenerator1 is
	port ( 
		clk :           in std_logic;   
        mute :          in std_logic;   -- mutes generator when 0
        tsl :           in integer;     -- total samples per line
        asl :           in integer;     -- active samples per line
        lpf :           in integer;     -- lines per frame
        field1_vblnk0 : in integer;     -- vertical blanking borders
        field2_vblnk1 : in integer;
        field2_vblnk0 : in integer;
        field1_vblnk1 : in integer;
        field2_start :  in integer;
        sample :        inout integer;    -- output to videogenerator
        sav :           out std_logic;
        line_clk :      inout std_logic;
        line :          inout integer;    -- output to videogenerator
        fvh_out :       out std_logic_vector(2 downto 0); -- blanking bits
        y :             in std_logic_vector(9 downto 0);     -- input from videogenerator
        cb :            in std_logic_vector(9 downto 0);     -- input from videogenerator
        cr :            in std_logic_vector(9 downto 0);         
		luma :          out std_logic_vector(9 downto 0);   -- output to scrambler
		chroma :        out std_logic_vector(9 downto 0);   -- output to scrambler
        -- debug :
        tp :            out std_logic
	);
end hd_framegenerator1;

architecture behavioral of hd_framegenerator1 is

signal luma_out, chroma_out : std_logic_vector(9 downto 0);
signal chromaselect : std_logic;
signal samplecount : std_logic_vector(11 downto 0);
signal linecount : std_logic_vector(10 downto 0);
signal lpf_vector : std_logic_vector(10 downto 0);
signal xyz : std_logic_vector(9 downto 0);
signal f : std_logic;
signal v : std_logic;
signal h : std_logic;
signal fvh : std_logic_vector(2 downto 0);
signal parity : std_logic_vector(3 downto 0);
signal luma_crc, chroma_crc : std_logic_vector(17 downto 0);
signal luma_crc0, chroma_crc0 : std_logic_vector(17 downto 0);

begin

---- debug : --------
tp <= '0';
---------------------

samplecounter : process ( mute, clk, asl ) begin
	if mute = '0' then
		samplecount <= conv_std_logic_vector(asl,12);
	elsif clk'event and clk = '1' then
		if samplecount /= (tsl-1) then
			samplecount <= samplecount + 1;
			if samplecount = asl then
                line_clk <= '1';
            else
                line_clk <= '0';
			end if;
		else
			samplecount <= conv_std_logic_vector(0,12);
		end if;
	end if;
end process;

sample <= conv_integer(samplecount);
chromaselect <= samplecount(0);         -- for 4:2:2 sampling scheme

lpf_vector <= conv_std_logic_vector(lpf,11);

linecounter : process ( mute, line_clk, linecount ) begin
	if mute = '0' then
		linecount <= conv_std_logic_vector(1,11);
	elsif line_clk'event and line_clk = '1' then
        if linecount /= lpf_vector then
            linecount <= linecount + 1;
        else
            linecount <= conv_std_logic_vector(1,11);
        end if;
    end if;
end process;
line <= conv_integer(linecount);

videogenerator : process ( mute, clk) begin
	if mute = '0' then
		luma_out <= (others => '0');
		chroma_out <= (others => '0');
	elsif clk'event and clk='1' then
		if samplecount = ( tsl-4 ) then
            -- SAV
			luma_out <=     (others => '1');
			chroma_out <=   (others => '1');
            sav <= '1';
		elsif samplecount = asl then
            -- EAV
			luma_out <=     (others => '1');
			chroma_out <=   (others => '1');
		elsif ((samplecount = asl+1) or (samplecount = asl+2) or (samplecount = tsl-3) or (samplecount = tsl-2)) then
            -- SAV & EAV
			luma_out <=     (others => '0');
			chroma_out <=   (others => '0');
            sav <= '0';
		elsif ( samplecount = asl+3 ) or ( samplecount = tsl-1 ) then
            -- insert XYZ word
			luma_out <=     xyz;
			chroma_out <=   xyz;
            fvh_out <=      xyz(8 downto 6);
		elsif samplecount = asl+4 then
			-- insert LN0
            luma_out <=     (not linecount(6)) & linecount(6 downto 0) & "00";
            chroma_out <=   (not linecount(6)) & linecount(6 downto 0) & "00";
		elsif samplecount = asl+5 then
			-- insert LN1
            luma_out <=     "1000" & linecount(10 downto 7) & "00";
            chroma_out <=   "1000" & linecount(10 downto 7) & "00";
		elsif samplecount = asl+6 then
		    -- insert crc0 here
            luma_out <=     (not luma_crc0(8)) & luma_crc0(8 downto 0);
			chroma_out <=   (not chroma_crc0(8)) & chroma_crc0(8 downto 0);
		elsif samplecount = asl+7 then
		    -- insert crc1 here
			luma_out <=     (not luma_crc(17)) & luma_crc(17 downto 9);
			chroma_out <=   (not chroma_crc(17)) & chroma_crc(17 downto 9);
		elsif samplecount > asl+7 then	
            -- horizontal blanking interval
			luma_out <=     "0001000000"; -- 0x040
			chroma_out <=   "1000000000"; -- 0x200
		elsif (linecount < field1_vblnk0) or 
              ((linecount >= field2_vblnk1) and (linecount < field2_vblnk0)) or 
              (linecount >= field1_vblnk1) then 
             -- vertical blanking interval
			luma_out <=     "0001000000"; -- 0x040
			chroma_out <=   "1000000000"; -- 0x200
		else 
            -- active video field :
			luma_out <= y;
            if chromaselect = '0' then
			    chroma_out <= cb;
            else
			    chroma_out <= cr;
            end if;
		end if;
	end if;
end process;

luma_crc0 <= crc_0_4_5_18(luma_out,luma_crc);
chroma_crc0 <= crc_0_4_5_18(chroma_out,chroma_crc);

make_crc : process ( mute, clk) 
begin
	if mute = '0' then
		luma_crc <= conv_std_logic_vector(0,18);
		chroma_crc <= conv_std_logic_vector(0,18);
	elsif clk'event and clk='1' then
		if ((samplecount <= (asl+6)) and (samplecount > 0)) then
			luma_crc <= crc_0_4_5_18(luma_out,luma_crc);
			chroma_crc <= crc_0_4_5_18(chroma_out,chroma_crc);
		elsif (samplecount = (asl+7)) then
			luma_crc <= conv_std_logic_vector(0,18);
			chroma_crc <= conv_std_logic_vector(0,18);
		end if;
    end if;
end process;

make_f_bit : process( mute, line_clk )
-- field bit located in EAV & SAV
begin
    if mute = '0' then
        f <= '0';
    elsif line_clk'event and line_clk = '0' then
        if line = field2_start then
            f <= '1';
        elsif line = 1 then
            f <= '0';
        else
            null;
        end if;
    end if;
end process;

make_v_bit : process( mute, line_clk )
-- vertical blanking bit located in EAV & SAV
begin
    if mute = '0' then
        v <= '1';
    elsif line_clk'event and line_clk = '0' then
        if line = field1_vblnk0 then
			v <= '0';
        elsif line = field2_vblnk1 then
			v <= '1';
        elsif line = field2_vblnk0 then
			v <= '0';
        elsif line = field1_vblnk1 then
            v <= '1';
        else
            null;
        end if;            
    end if;
end process;

make_h_bit : process ( mute, clk ) 
-- horizontal blanking bit located in EAV & SAV
begin
	if mute = '0' then
        h <= '1';
	elsif clk'event and clk = '1' then
		if samplecount > ( asl + 3 ) then 
		    h <= '0';
		else
		    h <= '1';
		end if;
    end if;
end process;

fvh <= f & v & h;
with fvh select
    parity <=   "0000" when "000",
	            "1101" when "001",
	            "1011" when "010",
	            "0110" when "011",
	            "0111" when "100",
	            "1010" when "101",
	            "1100" when "110",
	            "0001" when others;

make_XYZ_word : xyz <= '1' & fvh & parity & "00";

video_out : process
begin
	wait until clk = '1';
	luma <= luma_out;
	chroma <= chroma_out;
end process;

end behavioral;

--make_fvh_bits : process (mute,clk) 
--begin
--	if mute = '0' then
  --      fvh <= "111";
	--elsif clk'event and clk = '1' then
        -- field bit :
--		if linecount = field2_start then
--			fvh(2) <= '1';
--		elsif linecount = 1 then
--			fvh(2) <= '0';
  --      else
    --        null;
	--	end if;
--		if  (linecount < field1_vblnk0) or 
  --          ((linecount >= field2_vblnk1) and (linecount < field2_vblnk0)) or 
    --        (linecount >= field1_vblnk1) then
	--		v <= '1';
	--	else
	--		v <= '0';
	--	end if;
        -- horizontal blanking bit :
--		if samplecount > asl+3 then 
--			fvh(0) <= '0';
--		else
--			fvh(0) <= '1';
--		end if;
--	end if;
--end process;