-- Copyright 2003 Linear Systems Ltd.

use work.pack_reverse_bit_order.all;
LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;

-- hdsdgen_02_nab :
-- projekt til NAB2005 exhibition, som skal konfigureres af
-- PT5300 mainframe ( kan ikke styres via USB-interface )

ENTITY hdvb0 IS
	PORT(
		-- cyv15g0403 interface
		txclkoa, txclkob		: IN	STD_LOGIC;
		rxclka_p, rxclkb_p		: IN	STD_LOGIC;
		rxclka_n, rxclkb_n		: IN	STD_LOGIC;
		cd_muteb2				: IN	STD_LOGIC;
		rxda, rxdb				: IN	STD_LOGIC_VECTOR(7 DOWNTO 0);
		rxsta, rxstb			: IN	STD_LOGIC_VECTOR(2 DOWNTO 0);
		lfia, lfib				: IN	STD_LOGIC;
		txerra, txerrb			: IN 	STD_LOGIC;
		cd_mutea, cd_muteb		: INOUT STD_LOGIC;
		txclka, txclkb			: buffer	STD_LOGIC;
		txda, txdb				: buffer	STD_LOGIC_VECTOR(7 DOWNTO 0);
		txcta, txctb			: buffer	STD_LOGIC_VECTOR(1 DOWNTO 0);
		sd_hda, sd_hdb			: buffer 	STD_LOGIC;
		rclkena, rclkenb		: OUT 	STD_LOGIC;
		lpena, lpenb			: OUT	STD_LOGIC;
		ulca, ulcb				: OUT	STD_LOGIC;
		insela, inselb			: BUFFER	STD_LOGIC;
		spdsela, spdselb		: BUFFER	STD_LOGIC;
		ldtden					: OUT	STD_LOGIC;
		fclka_p, fclka_n, fclkb_p, fclkb_n			: buffer STD_LOGIC;

		-- Parallel Interfaces
		ptxda, ptxdb			: IN	STD_LOGIC_VECTOR(9 downto 0);
		prxda, prxdb			: in	STD_LOGIC_VECTOR(9 DOWNTO 0);
		prxclka, prxclkb		: OUT STD_LOGIC;
		ptxclka, ptxclkb		: IN STD_LOGIC;

		-- led's
		led270a, led270b, led360a, led360b	: OUT STD_LOGIC;	-- LED OUTPUTS
		led540a, led540b, led7425a, led7425b	: OUT STD_LOGIC;
		ledlfia, ledlfib		: BUFFER STD_LOGIC;
		ledcda, ledcdb			: BUFFER STD_LOGIC;
		led2					: OUT STD_LOGIC;
		
		-- fx2/gpif interface
		reset_n					: IN	STD_LOGIC;							--
		rdy						: BUFFER	STD_LOGIC_VECTOR(1 DOWNTO 0); 	-- always "00"
		clkout					: IN	STD_LOGIC;							-- ikke i brug
		ifclk					: INOUT	STD_LOGIC;							-- ikke i brug
		pa7_flagd_slcs			: INOUT	STD_LOGIC;  						-- ikke i brug
		ctl						: IN	STD_LOGIC_VECTOR(2 DOWNTO 0);	  	-- ikke i brug
		fd						: INOUT	STD_LOGIC_VECTOR(15 DOWNTO 0);	  	-- ikke i brug
		
		-- spi interface
		scse, scl, sdo			: IN	STD_LOGIC;
		sdi						: BUFFER	STD_LOGIC);
END hdvb0;

ARCHITECTURE a OF hdvb0 IS

	component serial_interface is Port(
        mreset :        in std_logic;   -- master reset
        cs :            in std_logic;   -- chip select
        sck :           in std_logic;   -- serial clock
        mosi :          in std_logic;   -- master out serial data in
        offset :    	out std_logic_vector(23 downto 0);  -- delay from sync reference
        new_delay :     out std_logic;
		sd_hd :			out std_logic;						-- LOW = SD, HIGH = HD
		sd_format :		out std_logic;						-- LOW = 625 line system, HIGH = 525 line system
		scan_format :	out std_logic;						-- LOW = progressive, HIGH = interlaced
		hd_clk :		out std_logic;						-- LOW = 74,25 MHz, HIGH = 74,25/1.001 MHz
		hd_format :		out std_logic_vector(3 downto 0);	-- high definition format
		pattern :		out std_logic_vector(7 downto 0);	-- test pattern select
		tp_option :		out std_logic_vector(7 downto 0);	-- test pattern option
		tp :			out std_logic
        );
	end component;

	COMPONENT spi
		PORT(
		scs, scl, sdo		: IN	STD_LOGIC;	-- SPI inputs from PSoC
		sdi					: OUT	STD_LOGIC;	-- SPI data output to PSoC
		read, write 		: OUT	STD_LOGIC;	-- read and write signals to channel registers
		channel				: OUT	STD_LOGIC;	-- channel select output
		address				: BUFFER	STD_LOGIC_VECTOR(7 DOWNTO 0);	-- address output to channel registers
		data_out			: BUFFER	STD_LOGIC_VECTOR(7 DOWNTO 0);	-- data output to channel registers
		data_in				: IN	STD_LOGIC_VECTOR(7 DOWNTO 0)		-- data input from channel registers
		);
	END COMPONENT;
	
	COMPONENT channelregs
		PORT(
			reset, write, read	: IN	STD_LOGIC;	-- reset from pin, read & write from spi module
			channel, chsel		: IN 	STD_LOGIC;	-- channel input from SPI, channel select setting fixed in instantiation
			address				: IN	STD_LOGIC_VECTOR(7 DOWNTO 0);	-- address from spi module
			data_in				: INOUT	STD_LOGIC_VECTOR(7 DOWNTO 0);	-- data input from spi module
			data_out			: OUT	STD_LOGIC_VECTOR(7 DOWNTO 0);	-- data output to spi module
			century, year, month, day : IN STD_LOGIC_VECTOR(7 DOWNTO 0);	-- release date from top level module
			video_type			:IN STD_LOGIC_VECTOR(2 DOWNTO 0);
			lfi, cd, cd2		: IN	STD_LOGIC;	-- status indecators from HOTLink II and cable receivers
			dvb_nsmpte, insel	: OUT 	STD_LOGIC;	-- dvb/smpte select output, insel output to HOTLink II
			lpen			: OUT 	STD_LOGIC;	-- lpen output to HOTLink II
			chreset			: OUT 	STD_LOGIC;	--  channel reset
			pal_nntsc		: OUT	STD_LOGIC;	-- pal/ntsc output to test pattern generators
			freq			: OUT	STD_LOGIC_VECTOR(1 DOWNTO 0);	-- frequency select output
			next_freq			: in	STD_LOGIC_VECTOR(1 DOWNTO 0);	-- next frequency select input (for auto rate detection)
			txsrc			: out 	STD_LOGIC_VECTOR(2 DOWNTO 0);	-- transmit source select output
			read_acrc_errors			: OUT	STD_LOGIC;		-- read output to edh acrc error counter
			read_fcrc_errors			: OUT	STD_LOGIC;		-- read output to edh fcrc error counter
			acrc_errors, fcrc_errors	: IN STD_LOGIC_VECTOR( 7 DOWNTO 0);  -- crc error counts from edh module
			readYcrc, readCcrc		: BUFFER 	STD_LOGIC;				-- read outputws to HD-SDI CRC error counters
			Y_crc_errors, C_crc_errors : in std_logic_vector(31 downto 0)	-- input values from HD-SDI crc error counters
		);
	END COMPONENT;

	COMPONENT smpte_259
		PORT ( 
			-- Descrambler signals
--	        rxclk,                      -- CY7B933 CKR recovered clock
	--		reset_n,
	  --      DVB_select: IN std_logic;   -- select 8B/10B mode (active LOW)
	    --    rxbypass: IN std_logic;       -- raw data mode, bypass scrambler data
	      --  SYNC_en: IN std_logic;      -- TRS/SYNC correction enabled
	        --rxoe: IN std_logic;           -- tristate output enable
--	        data_in: IN std_logic_vector (0 TO 9);
	                                    -- scrambled data from HOTLink RX
	--        rxdata_out: BUFFER std_logic_vector (0 TO 9);
	                                    -- descrambled and framed data
	  --      SYNC_error: out std_logic;   -- SYNC offset error pulse
	    --    RF: out std_logic;       -- HOTLink RX frame control pin
	      --  AB: out std_logic;       -- HOTLink A/B port select pin
	        --H_SYNC: out std_logic; 	-- horizontal sync output toggle

			-- Scrambler Signals
	        txclk,                      -- HOTLink TX CKW clock
	        DVB_EN: IN std_logic;       -- select 8B/10B mode (active LOW)
	        trs_filt: IN std_logic;     -- TRS filter
	        ena_in: IN std_logic;       -- /ENA input
	        enn_in: IN std_logic;       -- /ENN input
	        SVS_en: IN std_logic;       -- enable transmission of SVS
	        SCD_en: IN std_logic;       -- enable selection of commands
	        txoe: IN std_logic;           -- tristate output enable
	        txbypass: IN std_logic;       -- raw data mode - bypass scrambler
	                                    -- 10-bit raw data interface
	        data: IN std_logic_vector(0 to 9);
	        trs_out: out std_logic;  -- TRS code detected
	        ena_out: out std_logic;  -- HOTLink TX/ENA input
	        txdata_out: BUFFER std_logic_vector(0 to 9) -- parallel scrambled output
	);
	END COMPONENT;

	COMPONENT sd_frame
		PORT(
			reset_n, clk, pal_nntsc		: IN	STD_LOGIC;			-- active low reset, transmit clock, pal/ntsc select
			txsrc			: IN	STD_LOGIC_VECTOR(2 DOWNTO 0);	-- transmit source select input
			txrate			: IN	STD_LOGIC_VECTOR(1 DOWNTO 0);	-- transmit rate input
			video_out		: OUT STD_LOGIC_VECTOR(9 DOWNTO 0)		-- video data out
			);
	END COMPONENT;

	COMPONENT hd_frame
		PORT ( 
			clk :       	in std_logic;   
        	mute :      	in std_logic;                       -- mutes generator when 0
        	hdformat :  	in std_logic_vector(3 downto 0);    -- SMPTE 240M, 274M or 296M
  	    	act_y :     	in std_logic_vector(9 downto 0);     -- input from videogenerator
        	act_cb :    	in std_logic_vector(9 downto 0);     -- input from videogenerator
       		act_cr :    	in std_logic_vector(9 downto 0);         
			scan_format : 	inout std_logic;					-- output for active video modul
			sav :           out std_logic;						-- output for active video modul
			fvh :           out std_logic_vector(2 downto 0);	-- output for active video modul
        	line_clk :      inout std_logic;
			luma :      	out std_logic_vector(9 downto 0);   -- output to scrambler
			chroma :    	out std_logic_vector(9 downto 0)    -- output to scrambler
			);
	END COMPONENT;

	COMPONENT smpte_292
		PORT(
	        -- scrambler tx I/O
	        txclk,                         -- HOTLink TX CKW clock
	        tx_reset,                      -- active high reset
	        tx_oe,                         -- output enable
	        tx_bypass: IN std_logic;       -- raw data mode - bypass scrambler
			tx_ten_twenty : in std_logic;  -- switch for determining either a 10 or 20 bit interface
	        trs_filt_lower10: IN std_logic;-- TRS filter lower 10 bits
	        trs_filt_upper10: IN std_logic;-- TRS filter upper 10 bits
	        txdata_in: IN std_logic_vector(0 to 19); -- raw data in from Co-Processor
	        trs_det: out std_logic;        -- TRS code detected
	        txdata_out: out std_logic_vector(0 to 9)); -- parallel scrambled output to Hotlink II PHY
	        -- de-scrambler rx I/O
--			rxclk,                         -- CY7B933 CKR recovered clock
	--        rx_reset,                      -- active high reset
	  --      rx_oe,                         -- output enable
	    --    rx_bypass: IN std_logic;       -- raw data mode, bypass scrambler data
			--rx_ten_twenty : in std_logic;  -- switch for determining either a 10 or 20 bit interface
--	        sync_en: IN std_logic;         -- TRS/SYNC correction enabled
	--        rxdata_in: IN std_logic_vector(0 to 9);   -- scrambled data input from HOTLink II PHY
	  --      rxdata_out: out std_logic_vector(0 to 19); -- descrambled and framed data
	    --    sync_err: out std_logic;       -- SYNC offset error pulse
	      --  h_sync: out std_logic);        -- horizontal sync output toggle
	END COMPONENT;

--	COMPONENT crc292check
	--	PORT(
		--	clk, reset	: IN STD_LOGIC;
			--data_in		: IN STD_LOGIC_VECTOR(0 to 9);
--			readcrc		: IN 	STD_LOGIC;
	--		crc_errors   : BUFFER STD_LOGIC_VECTOR(31 DOWNTO 0)
		--);
--	END COMPONENT;

--	COMPONENT video_detect
	--	PORT(
		--	clk, reset						: IN	STD_LOGIC;							-- clock and reset lines
			--data_in							: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);		-- 10 bit video data in
--			data_out						: buffer	STD_LOGIC_VECTOR(9 DOWNTO 0);	-- 10 bit video data out
	--		count_out						: buffer 	std_logic_vector(11 downto 0);	-- sample count, valid if linecount != 0, syncronized to data_out
		--	video_type_out					: buffer 	std_logic_vector(2 downto 0);	-- video type indicator
			--linecount_out					: buffer	std_logic_vector(9 downto 0)	-- video line number, syncronized to data_out
--		);
	--END COMPONENT;

	COMPONENT edh
		PORT(
			clk, enable					: IN	STD_LOGIC;
			data_in						: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);
			linecount					: IN 	STD_LOGIC_VECTOR(9 DOWNTO 0);
			wordcount 					: IN 	STD_LOGIC_VECTOR(11 DOWNTO 0);
			video_type					: IN 	STD_LOGIC_VECTOR(2 DOWNTO 0);
			read_acrc_errors			: IN	STD_LOGIC;
			read_fcrc_errors			: IN	STD_LOGIC;
			acrc_errors, fcrc_errors	: BUFFER STD_LOGIC_VECTOR( 7 DOWNTO 0);
			data_out 					: BUFFER STD_LOGIC_VECTOR(9 DOWNTO 0)
		);
	END COMPONENT;

	component video_sm is
    	Port ( 
        	clk :           in std_logic;
	    	video_en :      in std_logic;
			sd_hd :			in std_logic;
         	hdformat :      in std_logic_vector(3 downto 0);                       
	        scan_format :   in std_logic;
    	    pattern :       in std_logic_vector(2 downto 0);
        	option :        in std_logic_vector(2 downto 0);
			sav :           in std_logic;
	        line_clk :      in std_logic;
    	    fvh :           in std_logic_vector(2 downto 0);
        	y :             out std_logic_vector(9 downto 0);
	        cb :            out std_logic_vector(9 downto 0);
    	    cr :            out std_logic_vector(9 downto 0);
        	-- debug
	        debug :         in std_logic;
    	    loadval :       out integer;
  			tp :			out std_logic_vector(23 downto 0)
	        );
	end component;


--	COMPONENT autorate
	--	PORT(
		--	clkout, reset, lfi, cd		: IN	STD_LOGIC;
			--freq						: IN	STD_LOGIC_VECTOR(1 DOWNTO 0);
--			next_freq					: BUFFER	STD_LOGIC_VECTOR(1 DOWNTO 0)
	--		);
--	END COMPONENT;
	
--	component pll1
	--	PORT
		--(
			--inclk0		: IN STD_LOGIC  := '0';
--			pllena		: IN STD_LOGIC  := '1';
	--		areset		: IN STD_LOGIC  := '0';
		--	c0		: OUT STD_LOGIC ;
			--locked		: OUT STD_LOGIC 
--		);
	--end component;

--	component upconvert
	--	PORT(
		--	clk27, clk74, reset		: IN	STD_LOGIC;
			--data_in					: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);
--			line_count				: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);
	--		sample_count			: IN	STD_LOGIC_VECTOR(10 DOWNTO 0);
		--	video_type				: IN	STD_LOGIC_VECTOR(2 DOWNTO 0);
			--data_out				: BUFFER	STD_LOGIC_VECTOR(19 DOWNTO 0)
--		);
	--end component;

	SIGNAL address, data_in, data_out, read_data0, read_data1 : STD_LOGIC_VECTOR(7 DOWNTO 0);
	SIGNAL channel, read, write, dvb_nsmptea, dvb_nsmpteb, pal_nntsca, pal_nntscb : STD_LOGIC;
	SIGNAL freqa, freqb: STD_LOGIC_VECTOR(1 DOWNTO 0);
	SIGNAL txsrca, txsrcb : STD_LOGIC_VECTOR(2 DOWNTO 0);
	signal rxdata_ina,rxdata_outa, txdata_outa, txdata_ina : std_logic_vector(0 to 9);
	signal rxdata_inb,rxdata_outb, txdata_outb, txdata_inb : std_logic_vector(0 to 9);
	signal h_synca,sync_errora, ena_outa,trs_outa,rfa,aba : std_logic;
	signal h_syncb,sync_errorb, ena_outb,trs_outb,rfb,abb : std_logic;
	signal SYNC_ena, trs_filta : std_logic;
	signal SYNC_enb, trs_filtb : std_logic;
	signal tpdataa, tpdatab : std_logic_vector(9 downto 0);
	signal tpdataa0, tpdatab0 : std_logic_vector(0 to 9);
	signal txclka2, txclkb2, rxclka2, rxclkb2, enable_eg1_292a, enable_eg1_292b : std_logic;
	signal enable_rp198a, enable_rp198b : std_logic;
	signal Ydataa, Cdataa, Ydatab, Cdatab : std_logic_vector(9 downto 0);
	signal rp198_Ydataa, rp198_Cdataa, rp198_Ydatab, rp198_Cdatab : std_logic_vector(9 downto 0);
	signal grey_292_Ydataa, grey_292_Cdataa, grey_292_Ydatab, grey_292_Cdatab : std_logic_vector(9 downto 0);
	signal enable_grey_292a, enable_grey_292b : std_logic;
	signal smpte292enablea, smpte292enableb : std_logic;
	signal tx292data_ina, tx292data_inb : std_logic_vector(0 to 19);
	signal tx292data_outa, tx292data_outb : std_logic_vector(0 to 9);
	signal rx292data_outa, rx292data_outb : std_logic_vector(0 to 19);
	signal lrx292data_outa, lrx292data_outb : std_logic_vector(0 to 19);
	SIGNAL century, year, month, day : std_logic_vector(7 downto 0);
	signal Y_crc_errorsa, C_crc_errorsa : std_logic_vector(31 downto 0);
	signal Y_crc_errorsb, C_crc_errorsb : std_logic_vector(31 downto 0);
	signal reset_crca, reset_crcb : std_logic;
	signal readYcrca, readCcrca, readYcrcb, readCcrcb : std_logic;
	signal vd_data_in_a, vd_data_out_a : std_logic_vector(9 downto 0);
	signal vd_linecount_a : std_logic_vector(9 downto 0);
	signal vd_wordcount_a : std_logic_vector(11 downto 0);
	signal video_type_a : std_logic_vector(2 downto 0);
	signal vd_data_in_b, vd_data_out_b : std_logic_vector(9 downto 0);
	signal vd_linecount_b : std_logic_vector(9 downto 0);
	signal vd_wordcount_b : std_logic_vector(11 downto 0);
	signal video_type_b : std_logic_vector(2 downto 0);
	signal acrc_errors_a, fcrc_errors_a : std_logic_vector(7 downto 0);
	signal read_acrc_errors_a, read_fcrc_errors_a : std_logic;
	signal acrc_errors_b, fcrc_errors_b : std_logic_vector(7 downto 0);
	signal read_acrc_errors_b, read_fcrc_errors_b : std_logic;
	signal next_freqa, next_freqb : std_logic_vector(1 downto 0);
	signal c1, pllena,pllenb, ucreset : std_logic;
	signal updata : std_logic_vector( 19 downto 0);
	signal chreseta, chresetb : std_logic;
	signal hdformat :std_logic_vector(3 downto 0) := "0000";
	signal pattern,option : std_logic_vector(7 downto 0);
	signal act_y, act_cb, act_cr : std_logic_vector(9 downto 0);	-- active video samples from video_sm modul
	signal tp : std_logic_vector(23 downto 0);	-- testpoint output from video state machine
    signal line_clk, hd_mute, sav, hd_scanformat, scan_format :std_logic;
    signal fvh : std_logic_vector(2 downto 0);	-- field, vblank, hblank
    signal loadval : integer;
	constant debug : std_logic := '0';	-- high for debug, low for compiling
	
	signal sif_tp,sif_reset_n, sif_rst, sif_cs, sif_sck, sif_mosi, sif_sd_hda : std_logic;
	signal sif_pal_nntsca, sif_scan_format, sif_hd_clk : std_logic;
	signal sif_hd_format : std_logic_vector(3 downto 0);
	signal sif_pattern, sif_option : std_logic_vector(7 downto 0);



BEGIN
	-- Process Statement

	-- Concurrent Procedure Call

	-- Concurrent Signal Assignment
	century	<= x"20";
	year	<= x"04";
	month 	<= x"09";
	day		<= x"28";
	txclka <= txclkoa;
	txclkb <= txclkob;
	ulca <= '1';
	ulcb <= '1';
	ldtden <= '0';
	led2 <= '0';
	rdy(0)<='0';
	rdy(1)<='0';
	
	tpdataa0 <= reverse_bit_order(tpdataa,10);
	
	tpdatab0 <= reverse_bit_order(tpdatab,10);
	
--	prxda <= reverse_bit_order(rxdata_ina,10);

--	prxdb <= reverse_bit_order(rxdata_inb,10);

sif_cs <= prxdb(9);
sif_mosi <= prxdb(8);
sif_sck <= prxdb(7);


--ledlfia 	<= sif_tp;
ledcda		<= sif_cs;
led7425b 	<= sif_mosi;
led540b 	<= sif_sck;

--led360b		<= '0';
--led270b 	<= sif_option(2);
--ledlfib 	<= sif_option(1);
--ledcdb 		<= sif_option(0);

led360b		<= sif_hd_format(3);
led270b 	<= sif_hd_format(2);
ledlfib 	<= sif_hd_format(1);
ledcdb 		<= sif_hd_format(0);

process --(txsrca,Cdataa,Ydataa,lrx292data_outa)
begin
WAIT UNTIL txclka2='1';
	if (txsrca = "000") or (txsrca="001") or (txsrca(2 DOWNTO 1)="01") then
		tx292data_ina <= reverse_bit_order(Cdataa,10)&reverse_bit_order(Ydataa,10);
	elsif txsrca = "101" then
		tx292data_ina <= lrx292data_outa;
--	elsif(txsrca = "111")then
	--	tx292data_ina(0) <= updata(0);--reverse_bit_order(Cdataa,10)&reverse_bit_order(Ydataa,10);
		--tx292data_ina(1) <= updata(1);
--		tx292data_ina(2) <= updata(2);
	--	tx292data_ina(3) <= updata(3);
		--tx292data_ina(4) <= updata(4);
--		tx292data_ina(5) <= updata(5);
	--	tx292data_ina(6) <= updata(6);
		--tx292data_ina(7) <= updata(7);
--		tx292data_ina(8) <= updata(8);
	--	tx292data_ina(9) <= updata(9);
		--tx292data_ina(10) <= updata(10);
--		tx292data_ina(11) <= updata(11);
	--	tx292data_ina(12) <= updata(12);
		--tx292data_ina(13) <= updata(13);
--		tx292data_ina(14) <= updata(14);
	--	tx292data_ina(15) <= updata(15);
		--tx292data_ina(16) <= updata(16);
--		tx292data_ina(17) <= updata(17);
	--	tx292data_ina(18) <= updata(18);
		--tx292data_ina(19) <= updata(19);
	else 
		tx292data_ina <= "00000000000000000000";
	end if;
end process;

process
begin
	wait until txclka2 = '1';
	lrx292data_outa <= rx292data_outa;
end process;

--	ledlfia <= not lfia;
	
	-- Conditional Signal Assignment
	led270a <= '1' when freqa = "00" else '0';
	led360a <= '1' when freqa = "01" else '0';
	led540a <= '1' when freqa = "10" else '0';
	led7425a <= '1' when freqa = "11" else '0';

	smtpe259txmuxa:
	txdata_ina <= 	tpdataa0 when txsrca <= "011" else
					reverse_bit_order(ptxda,10) when txsrca = "100" else
					rxdata_outa;

	rclkena <= '1' when txsrca ="110" ELSE '0';

--	ledcda <= not cd_mutea when insela = '1' else '1';

	enable_eg1_292a <= '1' when freqa="11" and (txsrca="000" or txsrca = "111") else '0';
	enable_grey_292a <= '1' when freqa="11" and txsrca="001" else '0';
	enable_rp198a <= '1' when freqa="11" and txsrca(2 DOWNTO 1) = "01" else '0';
	
	-- Selected Signal Assignment
	channelmux:
	WITH channel SELECT
		data_in	<=	read_data0	WHEN	'0',
					read_data1	WHEN	'1';

	WITH freqa SELECT
		sd_hda <= '0' WHEN "11",
					'1' WHEN others;
	WITH freqa SELECT
		spdsela <= '0' WHEN "00",
					'0' WHEN "01",
					'Z' WHEN "10",
					'1' WHEN "11";

--with txsrca select
--	pllena <= '1';-- when "111", '0' when others;

--pll1_inst : pll1 PORT MAP (
	--	inclk0	 => rxclkb_p,
		--pllena	 => pllena,
--		areset	 => areset_sig,
--		c0	 => fclkb_p,
	--	locked	 => pllenb
--	);
		
	fclka_p <= c1;
	fclka_n <= not c1;

mod0: serial_interface Port map(
        mreset => sif_reset_n,
        cs => sif_cs,
        sck	=> sif_sck,
        mosi => sif_mosi,
        offset => open,
        new_delay => open,
		sd_hd => sif_sd_hda,
		sd_format => sif_pal_nntsca,
		scan_format => sif_scan_format,
		hd_clk => sif_hd_clk,
		hd_format => sif_hd_format,
		pattern => sif_pattern,
		tp_option => sif_option,
		tp => sif_tp);

--u1: spi 
mod1: spi 
	PORT MAP	(
		scs => scse, scl => scl, sdo => sdo,
		sdi => sdi,
		read => read, write => write,
		channel => channel,
		address => address,
		data_out => data_out,
		data_in => data_in
	);
	
--u2: channelregs
mod2: channelregs
	PORT MAP(
		reset => NOT reset_n, 
		write => write, 
		read => read,
		channel => channel, 
		chsel => '0',
		address => address,
		data_in => data_out,
		data_out => read_data0,
		century => century, 
		year => year, 
		month => month, 
		day => day,
		video_type => 
		video_type_a,
		lfi => lfia, 
		cd => cd_mutea, 
		cd2 => '0',
		dvb_nsmpte => dvb_nsmptea, 
		insel => insela,
		lpen => lpena, 
		chreset => chreseta,
		pal_nntsc => pal_nntsca,
		freq => freqa, 
		next_freq => next_freqa,
		txsrc => txsrca,
		read_acrc_errors => read_acrc_errors_a,--			: OUT	STD_LOGIC;		-- read output to edh acrc error counter
		read_fcrc_errors => read_fcrc_errors_a,--			: OUT	STD_LOGIC;		-- read output to edh fcrc error counter
		acrc_errors => acrc_errors_a, 
		fcrc_errors =>fcrc_errors_a,-- 	: IN STD_LOGIC_VECTOR( 7 DOWNTO 0);  -- crc error counts from edh module
		readYcrc => readYcrca, 
		readCcrc =>	readCcrca,
		Y_crc_errors => Y_crc_errorsa, 
		C_crc_errors => C_crc_errorsa
	);
	
--u4a: smpte_259
mod3: smpte_259
	PORT MAP(
--        rxclk => rxclka_p,
--	reset_n => reset_n,
--        DVB_select => '1',
--      rxbypass => '0',
--        SYNC_en => '1',--SYNC_ena,
--      rxoe => '1',
--        data_in => rxdata_ina,
--        rxdata_out => rxdata_outa,
--        SYNC_error => SYNC_errora,
--        RF => RF,
--        AB => AB,
--        H_SYNC => H_SYNCa,
        txclk => txclka,
        DVB_EN => '1',
        trs_filt => '0',
        ena_in => '0',
        enn_in => '0',
        SVS_en => '0',
        SCD_en => '0',
        txoe => '1',
        txbypass => '0',
                                    -- 10-bit raw data interface
        data => txdata_ina,
        trs_out => trs_outa,
        ena_out => ena_outa,
        txdata_out => txdata_outa
	);
	
vd_data_in_a(0) <= rxdata_outa(0);
vd_data_in_a(1) <= rxdata_outa(1);
vd_data_in_a(2) <= rxdata_outa(2);
vd_data_in_a(3) <= rxdata_outa(3);
vd_data_in_a(4) <= rxdata_outa(4);
vd_data_in_a(5) <= rxdata_outa(5);
vd_data_in_a(6) <= rxdata_outa(6);
vd_data_in_a(7) <= rxdata_outa(7);
vd_data_in_a(8) <= rxdata_outa(8);
vd_data_in_a(9) <= rxdata_outa(9);

mod4 : sd_frame
	PORT MAP(
		reset_n => reset_n, 
		clk => txclka, 
		pal_nntsc => pal_nntsca,
		txsrc => txsrca,
		txrate => freqa,
		video_out => tpdataa
		);
		
hd_mute <= enable_eg1_292a and sif_rst;--enable_rp198a and and enable_grey_292a;
hdformat <= sif_hd_format(3 downto 0);	-- 1080i/30
--hdformat <= "0110";	-- 720p/60
pattern <= "00000" & sif_pattern(2 downto 0);
option <= "00000" & sif_option(2 downto 0);

sif_reset : process( txclka, sif_cs )
begin
	if txclka'event and txclka = '1' then
		sif_rst <= sif_cs;
	end if;
end process;


--U9A:	hd_frame
mod5 :	hd_frame
	PORT MAP(
		clk => txclka2, 
		mute => hd_mute,
		hdformat => hdformat,
		act_y => act_y,
	    act_cb => act_cb,
        act_cr => act_cr,
		scan_format => hd_scanformat,
		sav	=> sav,
		fvh => fvh,
        line_clk => line_clk,
		luma => Ydataa,
		chroma => Cdataa
		);
	
mod6 : smpte_292
	PORT MAP(
        -- scrambler tx I/O
        txclk => txclka2,                         -- HOTLink TX CKW clock
        tx_reset => sd_hda,                      -- active high reset
        tx_oe => '1',                         -- output enable
        tx_bypass => '0',       -- raw data mode - bypass scrambler
		tx_ten_twenty => '0',  -- switch for determining either a 10 or 20 bit interface
        trs_filt_lower10 => '0',-- TRS filter lower 10 bits
        trs_filt_upper10 => '0',-- TRS filter upper 10 bits
        txdata_in => tx292data_ina, -- raw data in from Co-Processor
--        trs_det => ,        -- TRS code detected
        txdata_out => tx292data_outa -- parallel scrambled output to Hotlink II PHY
        -- de-scrambler rx I/O
--		rxclk => rxclka2,                         -- CY7B933 CKR recovered clock
  --      rx_reset => sd_hda,                      -- active high reset
    --    rx_oe => '1',                         -- output enable
      --  rx_bypass => '0',       -- raw data mode, bypass scrambler data
		--rx_ten_twenty => '0',  -- switch for determining either a 10 or 20 bit interface
--        sync_en => '1',         -- TRS/SYNC correction enabled
  --      rxdata_in => rxdata_ina,   -- scrambled data input from HOTLink II PHY
    --    rxdata_out => rx292data_outa--, -- descrambled and framed data
--        sync_err => ,       -- SYNC offset error pulse
--        h_sync => ,
	);        -- horizontal sync output toggle

with sd_hda select
	scan_format <= hd_scanformat when '0',
					'1' when others;
	
mod7 : video_sm port map(
        clk         => txclka2,
        video_en    => hd_mute,
		sd_hd		=> sd_hda,
        hdformat    => hdformat,
        scan_format => scan_format,
        pattern     => pattern(2 downto 0),
		option		=> option(2 downto 0),
        sav         => sav,
        line_clk    => line_clk,
        fvh         => fvh,
        y           => act_y,
        cb          => act_cb,
        cr          => act_cr,
		debug		=> debug,
		loadval		=> loadval,
		tp			=> tp
        );

reset_crca <= '1' when (reset_n='0') or freqa /= "11" or lfia = '0' else '0';

ucreset <= '1' when (reset_n = '0' or txsrca /= "111" or lfib = '0') else '0';

process--(freqa,tx292data_outa,txdata_outa)
begin
	wait until txclka = '1';
	if freqa = "11" then
		txda(0) <=  tx292data_outa(0);
		txda(1) <=  tx292data_outa(1);
		txda(2) <=  tx292data_outa(2);
		txda(3) <=  tx292data_outa(3);
		txda(4) <=  tx292data_outa(4);
		txda(5) <=  tx292data_outa(5);
		txda(6) <=  tx292data_outa(6);
		txda(7) <=  tx292data_outa(7);
		txcta(0) <= tx292data_outa(8);
		txcta(1) <= tx292data_outa(9);
	else
		txda(0) <=  txdata_outa(0);
		txda(1) <=  txdata_outa(1);
		txda(2) <=  txdata_outa(2);
		txda(3) <=  txdata_outa(3);
		txda(4) <=  txdata_outa(4);
		txda(5) <=  txdata_outa(5);
		txda(6) <=  txdata_outa(6);
		txda(7) <=  txdata_outa(7);
		txcta(0) <= txdata_outa(8);
		txcta(1) <= txdata_outa(9);
	end if;
end process;

PROCESS
BEGIN
	WAIT UNTIL txclka = '1';
	txclka2 <= not txclka2;
END PROCESS;

END a;