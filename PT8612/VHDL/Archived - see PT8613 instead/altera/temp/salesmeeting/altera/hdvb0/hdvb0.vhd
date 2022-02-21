-- Copyright 2003 Linear Systems Ltd.

use work.pack_reverse_bit_order.all;
LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;

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
		txerra, txerrb			: IN STD_LOGIC;
		cd_mutea, cd_muteb		: INOUT STD_LOGIC;
		txclka, txclkb			: buffer	STD_LOGIC;
		txda, txdb				: buffer	STD_LOGIC_VECTOR(7 DOWNTO 0);
		txcta, txctb			: buffer	STD_LOGIC_VECTOR(1 DOWNTO 0);
		sd_hda, sd_hdb			: buffer 	STD_LOGIC;
		rclkena, rclkenb		: OUT STD_LOGIC;
		lpena, lpenb			: OUT	STD_LOGIC;
		ulca, ulcb				: OUT	STD_LOGIC;
		insela, inselb			: BUFFER	STD_LOGIC;
		spdsela, spdselb		: BUFFER	STD_LOGIC;
		ldtden					: OUT	STD_LOGIC;
		fclka_p, fclka_n, fclkb_p, fclkb_n			: buffer STD_LOGIC;
		

		-- Parallel Interfaces
		ptxda, ptxdb			: IN	STD_LOGIC_VECTOR(9 downto 0);
		prxda, prxdb			: BUFFER	STD_LOGIC_VECTOR(9 DOWNTO 0);
		prxclka, prxclkb		: OUT STD_LOGIC;
		ptxclka, ptxclkb		: IN STD_LOGIC;

		-- led's
		led270a, led270b, led360a, led360b	: OUT STD_LOGIC;	-- LED OUTPUTS
		led540a, led540b, led7425a, led7425b	: OUT STD_LOGIC;
		ledlfia, ledlfib		: BUFFER STD_LOGIC;
		ledcda, ledcdb			: BUFFER STD_LOGIC;
		led2					: OUT STD_LOGIC;
		
		-- fx2/gpif interface
		clkout, reset_n							: IN	STD_LOGIC;
		ifclk									: INOUT	STD_LOGIC;
		pa7_flagd_slcs							: INOUT	STD_LOGIC;
		rdy										: BUFFER	STD_LOGIC_VECTOR(1 DOWNTO 0);
		ctl										: IN	STD_LOGIC_VECTOR(2 DOWNTO 0);
		fd										: INOUT	STD_LOGIC_VECTOR(15 DOWNTO 0);
		
		-- spi interface
		scse, scl, sdo							: IN	STD_LOGIC;
		sdi										: BUFFER	STD_LOGIC);
END hdvb0;

ARCHITECTURE a OF hdvb0 IS
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

	COMPONENT smpte_core
		PORT ( 
			-- Descrambler signals
	        rxclk,                      -- CY7B933 CKR recovered clock
			reset_n,
	        DVB_select: IN std_logic;   -- select 8B/10B mode (active LOW)
	        rxbypass: IN std_logic;       -- raw data mode, bypass scrambler data
	        SYNC_en: IN std_logic;      -- TRS/SYNC correction enabled
	        rxoe: IN std_logic;           -- tristate output enable
	        data_in: IN std_logic_vector (0 TO 9);
	                                    -- scrambled data from HOTLink RX
	        rxdata_out: BUFFER std_logic_vector (0 TO 9);
	                                    -- descrambled and framed data
	        SYNC_error: out std_logic;   -- SYNC offset error pulse
	        RF: out std_logic;       -- HOTLink RX frame control pin
	        AB: out std_logic;       -- HOTLink A/B port select pin
	        H_SYNC: out std_logic; 	-- horizontal sync output toggle

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

	COMPONENT tpgen_dk
		PORT(
			reset_n, clk, pal_nntsc		: IN	STD_LOGIC;			-- active low reset, transmit clock, pal/ntsc select
			txsrc			: IN	STD_LOGIC_VECTOR(2 DOWNTO 0);	-- transmit source select input
			txrate			: IN	STD_LOGIC_VECTOR(1 DOWNTO 0);	-- transmit rate input
			data_out		: OUT STD_LOGIC_VECTOR(9 DOWNTO 0)		-- video data out
			);
	END COMPONENT;

	COMPONENT eg1_292
		PORT( 
			clk,enable	 : in std_logic;
			Y_data : out std_logic_vector(9 downto 0);
			C_data : out STD_LOGIC_VECTOR(9 DOWNTO 0)
		);
	END COMPONENT;
	
	COMPONENT hdsdi_generator
		PORT ( 
			clk,mute	 : in std_logic;
			hdformat :  in std_logic_vector(1 downto 0);                       
			luma : out std_logic_vector(9 downto 0);
			chroma : out std_logic_vector(9 downto 0)
		);
	END COMPONENT;

	COMPONENT hdsdi_greyedge_cb
		PORT ( 
			clk,mute	 : in std_logic;
			hdformat :  in std_logic_vector(1 downto 0);                       
			luma : out std_logic_vector(9 downto 0);
			chroma : out std_logic_vector(9 downto 0)
		);
	END COMPONENT;

	COMPONENT grey_292
		PORT ( 
			clk,enable	 : in std_logic;
			Y_data : out std_logic_vector(9 downto 0);
			C_data : out std_logic_vector(9 downto 0)
		);
	END COMPONENT;

	COMPONENT rp198 IS
		PORT ( 
			clk,enable,
			std_altn	 : in std_logic;	-- '1' = standard distribution, '0' = alternate distribution
			Y_data : out std_logic_vector(9 downto 0);
			C_data : out std_logic_vector(9 downto 0)
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
	        txdata_out: out std_logic_vector(0 to 9); -- parallel scrambled output to Hotlink II PHY
	        -- de-scrambler rx I/O
			rxclk,                         -- CY7B933 CKR recovered clock
	        rx_reset,                      -- active high reset
	        rx_oe,                         -- output enable
	        rx_bypass: IN std_logic;       -- raw data mode, bypass scrambler data
			rx_ten_twenty : in std_logic;  -- switch for determining either a 10 or 20 bit interface
	        sync_en: IN std_logic;         -- TRS/SYNC correction enabled
	        rxdata_in: IN std_logic_vector(0 to 9);   -- scrambled data input from HOTLink II PHY
	        rxdata_out: out std_logic_vector(0 to 19); -- descrambled and framed data
	        sync_err: out std_logic;       -- SYNC offset error pulse
	        h_sync: out std_logic);        -- horizontal sync output toggle
	END COMPONENT;

	COMPONENT crc292check
		PORT(
			clk, reset	: IN STD_LOGIC;
			data_in		: IN STD_LOGIC_VECTOR(0 to 9);
			readcrc		: IN 	STD_LOGIC;
			crc_errors   : BUFFER STD_LOGIC_VECTOR(31 DOWNTO 0)
		);
	END COMPONENT;

	COMPONENT video_detect
		PORT(
			clk, reset						: IN	STD_LOGIC;							-- clock and reset lines
			data_in							: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);		-- 10 bit video data in
			data_out						: buffer	STD_LOGIC_VECTOR(9 DOWNTO 0);	-- 10 bit video data out
			count_out						: buffer 	std_logic_vector(11 downto 0);	-- sample count, valid if linecount != 0, syncronized to data_out
			video_type_out					: buffer 	std_logic_vector(2 downto 0);	-- video type indicator
			linecount_out					: buffer	std_logic_vector(9 downto 0)	-- video line number, syncronized to data_out
		);
	END COMPONENT;

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

	COMPONENT autorate
		PORT(
			clkout, reset, lfi, cd		: IN	STD_LOGIC;
			freq						: IN	STD_LOGIC_VECTOR(1 DOWNTO 0);
			next_freq					: BUFFER	STD_LOGIC_VECTOR(1 DOWNTO 0)
			);
	END COMPONENT;
	
	component pll1
		PORT
		(
			inclk0		: IN STD_LOGIC  := '0';
			pllena		: IN STD_LOGIC  := '1';
			areset		: IN STD_LOGIC  := '0';
			c0		: OUT STD_LOGIC ;
			locked		: OUT STD_LOGIC 
		);
	end component;

	component pll2
		PORT
		(
			inclk0		: IN STD_LOGIC  := '0';
			pllena		: IN STD_LOGIC  := '1';
			areset		: IN STD_LOGIC  := '0';
			c0		: OUT STD_LOGIC ;
			locked		: OUT STD_LOGIC 
		);
	end component;

	component upconvert
		PORT(
			clk27, clk74, reset		: IN	STD_LOGIC;
			data_in					: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);
			line_count				: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);
			sample_count			: IN	STD_LOGIC_VECTOR(10 DOWNTO 0);
			video_type				: IN	STD_LOGIC_VECTOR(2 DOWNTO 0);
			data_out				: BUFFER	STD_LOGIC_VECTOR(19 DOWNTO 0)
		);
	end component;

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
	signal hdformat :std_logic_vector(1 downto 0) := "00";

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

process --(txsrca,Cdataa,Ydataa,lrx292data_outa)
begin
WAIT UNTIL txclka2='1';
	if(txsrca = "000")then
		tx292data_ina <= reverse_bit_order(Cdataa,10)&reverse_bit_order(Ydataa,10);
	elsif(txsrca="001")then
		tx292data_ina <= reverse_bit_order(grey_292_Cdataa,10)&reverse_bit_order(grey_292_Ydataa,10);
	elsif txsrca(2 DOWNTO 1) = "01" then
		tx292data_ina <= reverse_bit_order(rp198_Cdataa,10)&reverse_bit_order(rp198_Ydataa,10);
	elsif txsrca = "101" then
		tx292data_ina <= lrx292data_outa;
	elsif(txsrca = "111")then
		tx292data_ina(0) <= updata(0);--reverse_bit_order(Cdataa,10)&reverse_bit_order(Ydataa,10);
		tx292data_ina(1) <= updata(1);
		tx292data_ina(2) <= updata(2);
		tx292data_ina(3) <= updata(3);
		tx292data_ina(4) <= updata(4);
		tx292data_ina(5) <= updata(5);
		tx292data_ina(6) <= updata(6);
		tx292data_ina(7) <= updata(7);
		tx292data_ina(8) <= updata(8);
		tx292data_ina(9) <= updata(9);
		tx292data_ina(10) <= updata(10);
		tx292data_ina(11) <= updata(11);
		tx292data_ina(12) <= updata(12);
		tx292data_ina(13) <= updata(13);
		tx292data_ina(14) <= updata(14);
		tx292data_ina(15) <= updata(15);
		tx292data_ina(16) <= updata(16);
		tx292data_ina(17) <= updata(17);
		tx292data_ina(18) <= updata(18);
		tx292data_ina(19) <= updata(19);
	else 
		tx292data_ina <= "00000000000000000000";
	end if;
end process;

process
begin
	wait until txclka2 = '1';
	lrx292data_outa <= rx292data_outa;
end process;

process --(txsrcb,Cdatab,Ydatab,lrx292data_outb)
begin
WAIT UNTIL txclkb2 = '1';
	if txsrcb = "000" then
		tx292data_inb <= reverse_bit_order(Cdatab,10)&reverse_bit_order(Ydatab,10);
	elsif(txsrcb="001")then
		tx292data_inb <= reverse_bit_order(grey_292_Cdatab,10)&reverse_bit_order(grey_292_Ydatab,10);
	elsif txsrcb(2 DOWNTO 1) = "01" then
		tx292data_inb <= reverse_bit_order(rp198_Cdatab,10)&reverse_bit_order(rp198_Ydatab,10);
	elsif txsrcb = "101" then
		tx292data_inb <= lrx292data_outb;
--	elsif txsrcb = "111" then
--		tx292data_inb <= reverse_bit_order(Cdatab,10)&reverse_bit_order(Ydatab,10);
	else 
		tx292data_inb <= "00000000000000000000";
	end if;
end process;

process
begin
	wait until txclkb2 = '1';
	lrx292data_outb <= rx292data_outb;
end process;

	ledlfia <= not lfia;
	ledlfib <= not lfib;
	
	-- Conditional Signal Assignment
	led270a <= '1' when freqa = "00" else '0';
	led360a <= '1' when freqa = "01" else '0';
	led540a <= '1' when freqa = "10" else '0';
	led7425a <= '1' when freqa = "11" else '0';
	led270b <= '1' when freqb = "00" else '0';
	led360b <= '1' when freqb = "01" else '0';
	led540b <= '1' when freqb = "10" else '0';
	led7425b <= '1' when freqb = "11" else '0';
	
	smtpe259txmuxa:
	txdata_ina <= 	tpdataa0 when txsrca <= "011" else
					reverse_bit_order(ptxda,10) when txsrca = "100" else
					rxdata_outa;
	smpte259txmuxb:
	txdata_inb <=	tpdatab0 WHEN txsrcb <= "011" ELSE
					reverse_bit_order(ptxdb,10) when txsrcb = "100" else
					rxdata_outb;

	rclkena <= '1' when txsrca ="110" ELSE '0';
	rclkenb <= '1' when txsrcb ="110" ELSE '0';

	ledcda <= not cd_mutea when insela = '1' else '1';
	ledcdb <= not cd_muteb when inselb = '1' else cd_muteb2;

	enable_eg1_292a <= '1' when freqa="11" and (txsrca="000" or txsrca = "111") else '0';
	enable_eg1_292b <= '1' when freqb="11" and txsrcb="000" else '0';
	enable_grey_292a <= '1' when freqa="11" and txsrca="001" else '0';
	enable_grey_292b <= '1' when freqb="11" and txsrcb="001" else '0';
	enable_rp198a <= '1' when freqa="11" and txsrca(2 DOWNTO 1) = "01" else '0';
	enable_rp198b <= '1' when freqb="11" and txsrcb(2 DOWNTO 1) = "01" else '0';
	
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
	WITH freqb SELECT
		sd_hdb <= '0' WHEN "11", -- for smpte292m
					'1' WHEN others; -- for smpte259m, smpte344m, dvb-asi
	WITH freqb SELECT
		spdselb <= '0' WHEN "00",
					'0' WHEN "01",
					'Z' WHEN "10",
					'1' WHEN "11";
					
--with txsrca select
	pllena <= '1';-- when "111", '0' when others;

pll1_inst : pll1 PORT MAP (
		inclk0	 => rxclkb_p,
		pllena	 => pllena,
--		areset	 => areset_sig,
		c0	 => fclkb_p,
		locked	 => pllenb
	);
	
pll2_inst : pll2 PORT MAP (
		inclk0	 => rxclka_p,
		pllena	 => pllenb,
--		areset	 => areset_sig,
		c0	 => c1--,
--		locked	 => locked_sig
	);
	
	fclka_p <= c1;
	fclka_n <= not c1;
u1: spi 
	PORT MAP	(
		scs => scse, scl => scl, sdo => sdo,
		sdi => sdi,
		read => read, write => write,
		channel => channel,
		address => address,
		data_out => data_out,
		data_in => data_in
	);
	
u2: channelregs
	PORT MAP(
		reset => NOT reset_n, write => write, read => read,
		channel => channel, chsel => '0',
		address => address,
		data_in => data_out,
		data_out => read_data0,
		century => century, year => year, month => month, day => day,
		video_type => video_type_a,
		lfi => lfia, cd => cd_mutea, cd2 => '0',
		dvb_nsmpte => dvb_nsmptea, insel => insela,
		lpen => lpena, chreset => chreseta,
		pal_nntsc => pal_nntsca,
		freq => freqa, next_freq => next_freqa,
		txsrc => txsrca,
		read_acrc_errors => read_acrc_errors_a,--			: OUT	STD_LOGIC;		-- read output to edh acrc error counter
		read_fcrc_errors => read_fcrc_errors_a,--			: OUT	STD_LOGIC;		-- read output to edh fcrc error counter
		acrc_errors => acrc_errors_a, fcrc_errors =>fcrc_errors_a,-- 	: IN STD_LOGIC_VECTOR( 7 DOWNTO 0);  -- crc error counts from edh module
		readYcrc => readYcrca, readCcrc =>	readCcrca,
		Y_crc_errors => Y_crc_errorsa, C_crc_errors => C_crc_errorsa
	);
	
u3: channelregs
	PORT MAP(
		reset => NOT reset_n, write => write, read => read,
		channel => channel, chsel => '1',
		address => address,
		data_in => data_out,
		data_out => read_data1,
		century => century, year => year, month => month, day => day,
		video_type => video_type_b,
		lfi => lfib, cd => cd_muteb, cd2 => cd_muteb2,
		dvb_nsmpte => dvb_nsmpteb, insel => inselb,
		lpen => lpenb, chreset => chresetb,
		pal_nntsc => pal_nntscb,
		freq => freqb, next_freq => next_freqb,
		txsrc => txsrcb,
		read_acrc_errors => read_acrc_errors_b,--			: OUT	STD_LOGIC;		-- read output to edh acrc error counter
		read_fcrc_errors => read_fcrc_errors_b,--			: OUT	STD_LOGIC;		-- read output to edh fcrc error counter
		acrc_errors => acrc_errors_b, fcrc_errors =>fcrc_errors_b,-- 	: IN STD_LOGIC_VECTOR( 7 DOWNTO 0);  -- crc error counts from edh module
		readYcrc => readYcrcb, readCcrc =>	readCcrcb,
		Y_crc_errors => Y_crc_errorsb, C_crc_errors => C_crc_errorsb
	);

u13a: autorate
		PORT MAP(
			clkout => clkout, reset => chreseta, lfi => ledlfia, cd => ledcda,
			freq => freqa,
			next_freq => next_freqa
			);

u13b: autorate
		PORT MAP(
			clkout => clkout, reset => chresetb, lfi => ledlfib, cd => ledcdb,
			freq => freqb,
			next_freq => next_freqb
			);

u4a: smpte_core
	PORT MAP(
        rxclk => rxclka_p,
		reset_n => reset_n,
        DVB_select => '1',
        rxbypass => '0',
        SYNC_en => '1',--SYNC_ena,
        rxoe => '1',
        data_in => rxdata_ina,
        rxdata_out => rxdata_outa,
        SYNC_error => SYNC_errora,
--        RF => RF,
--        AB => AB,
        H_SYNC => H_SYNCa,
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

U11A: video_detect
	PORT MAP(
		clk => rxclka_p, reset => not reset_n,						--: IN	STD_LOGIC;							-- clock and reset lines
		data_in => vd_data_in_a,							--: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);		-- 10 bit video data in
		data_out => vd_data_out_a,						--: buffer	STD_LOGIC_VECTOR(9 DOWNTO 0);	-- 10 bit video data out
		count_out => vd_wordcount_a,						--: buffer 	std_logic_vector(11 downto 0);	-- sample count, valid if linecount != 0, syncronized to data_out
		video_type_out => video_type_a,					--: buffer 	std_logic_vector(2 downto 0);	-- video type indicator
		linecount_out => vd_linecount_a					--: buffer	std_logic_vector(9 downto 0)	-- video line number, syncronized to data_out
	);
U12A: edh
	PORT MAP(
		clk => rxclka_p, enable	=> reset_n,--				: IN	STD_LOGIC;
		data_in=> vd_data_out_a,--						: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);
		linecount =>  vd_linecount_a,--					: IN 	STD_LOGIC_VECTOR(9 DOWNTO 0);
		wordcount => vd_wordcount_a,-- 					: IN 	STD_LOGIC_VECTOR(11 DOWNTO 0);
		video_type => video_type_a,--					: IN 	STD_LOGIC_VECTOR(2 DOWNTO 0);
		read_acrc_errors => read_acrc_errors_a,--			: IN	STD_LOGIC;
		read_fcrc_errors => read_fcrc_errors_a,--			: IN	STD_LOGIC;
		acrc_errors => acrc_errors_a, fcrc_errors => fcrc_errors_a,--	: BUFFER STD_LOGIC_VECTOR( 7 DOWNTO 0);
		data_out => prxda--					: BUFFER STD_LOGIC_VECTOR(9 DOWNTO 0)
	);

u4b: smpte_core
	PORT MAP(
        rxclk => rxclkb_p,
		reset_n => reset_n,
        DVB_select => '1',
        rxbypass => '0',
        SYNC_en => '1',--SYNC_enb,
        rxoe => '1',
        data_in => rxdata_inb,
        rxdata_out => rxdata_outb,
        SYNC_error => SYNC_errorb,
 --       RF => RF,
 --       AB => AB,
        H_SYNC => H_SYNCb,
        txclk => txclkb,
        DVB_EN => '1',
        trs_filt => '0',
        ena_in => '0',
        enn_in => '0',
        SVS_en => '0',
        SCD_en => '0',
        txoe => '1',
        txbypass => '0',
                                    -- 10-bit raw data interface
        data => txdata_inb,
        trs_out => trs_outb,
        ena_out => ena_outb,
        txdata_out => txdata_outb
	);
	
vd_data_in_b(0) <= rxdata_outb(0);
vd_data_in_b(1) <= rxdata_outb(1);
vd_data_in_b(2) <= rxdata_outb(2);
vd_data_in_b(3) <= rxdata_outb(3);
vd_data_in_b(4) <= rxdata_outb(4);
vd_data_in_b(5) <= rxdata_outb(5);
vd_data_in_b(6) <= rxdata_outb(6);
vd_data_in_b(7) <= rxdata_outb(7);
vd_data_in_b(8) <= rxdata_outb(8);
vd_data_in_b(9) <= rxdata_outb(9);

U11B: video_detect
	PORT MAP(
		clk => rxclkb_p, reset => not reset_n,						--: IN	STD_LOGIC;							-- clock and reset lines
		data_in => vd_data_in_b,							--: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);		-- 10 bit video data in
		data_out => vd_data_out_b,						--: buffer	STD_LOGIC_VECTOR(9 DOWNTO 0);	-- 10 bit video data out
		count_out => vd_wordcount_b,						--: buffer 	std_logic_vector(11 downto 0);	-- sample count, valid if linecount != 0, syncronized to data_out
		video_type_out => video_type_b,					--: buffer 	std_logic_vector(2 downto 0);	-- video type indicator
		linecount_out => vd_linecount_b					--: buffer	std_logic_vector(9 downto 0)	-- video line number, syncronized to data_out
	);
U12B: edh
	PORT MAP(
		clk => rxclkb_p, enable	=> reset_n,--				: IN	STD_LOGIC;
		data_in=> vd_data_out_b,--						: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);
		linecount =>  vd_linecount_b,--					: IN 	STD_LOGIC_VECTOR(9 DOWNTO 0);
		wordcount => vd_wordcount_b,-- 					: IN 	STD_LOGIC_VECTOR(11 DOWNTO 0);
		video_type => video_type_b,--					: IN 	STD_LOGIC_VECTOR(2 DOWNTO 0);
		read_acrc_errors => read_acrc_errors_b,--			: IN	STD_LOGIC;
		read_fcrc_errors => read_fcrc_errors_b,--			: IN	STD_LOGIC;
		acrc_errors => acrc_errors_b, fcrc_errors => fcrc_errors_b,--	: BUFFER STD_LOGIC_VECTOR( 7 DOWNTO 0);
		data_out => prxdb--					: BUFFER STD_LOGIC_VECTOR(9 DOWNTO 0)
	);

U5A: tpgen_dk	
	PORT MAP(
		reset_n => reset_n, clk => txclka, pal_nntsc => pal_nntsca,
		txsrc => txsrca,
		txrate => freqa,
		data_out => tpdataa
		);
		
U5B: tpgen_dk
	PORT MAP(
		reset_n => reset_n, clk => txclkb, pal_nntsc => pal_nntscb,
		txsrc => txsrcb,
		txrate => freqb,
		data_out => tpdatab
		);
		
U6A: 	eg1_292
	PORT MAP(
		clk => txclka2, enable => enable_eg1_292a,
--		sel : IN STD_LOGIC_VECTOR(1 DOWNTO 0);
		Y_data => Ydataa,
		C_data => Cdataa
	);
	
U6B: 	eg1_292
	PORT MAP(
		clk => txclkb2, enable => enable_eg1_292b,
		Y_data => Ydatab,
		C_data => Cdatab
	);
	
U8A: 	rp198
	PORT MAP(
		clk => txclka2, enable => enable_rp198a,
		std_altn => not txsrca(0),	-- '1' = standard distribution, '0' = alternate distribution
		Y_data => rp198_Ydataa,
		C_data => rp198_Cdataa
	);
	
U8B: 	rp198
	PORT MAP(
		clk => txclkb2, enable => enable_rp198b,
		std_altn => not txsrcb(0),	-- '1' = standard distribution, '0' = alternate distribution
		Y_data => rp198_Ydatab,
		C_data => rp198_Cdatab
	);
	
hdformat <= "00";
U9A:	hdsdi_generator
	PORT MAP(
		clk => txclka2, mute => enable_grey_292a,
		hdformat => hdformat,
		luma => grey_292_Ydataa,
		chroma => grey_292_Cdataa
	);
	
U9B:	hdsdi_greyedge_cb
	PORT MAP(
		clk => txclkb2, mute => enable_grey_292b,
		hdformat => hdformat,
		luma => grey_292_Ydatab,
		chroma => grey_292_Cdatab
	);
	
U7A : smpte_292
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
        txdata_out => tx292data_outa, -- parallel scrambled output to Hotlink II PHY
        -- de-scrambler rx I/O
		rxclk => rxclka2,                         -- CY7B933 CKR recovered clock
        rx_reset => sd_hda,                      -- active high reset
        rx_oe => '1',                         -- output enable
        rx_bypass => '0',       -- raw data mode, bypass scrambler data
		rx_ten_twenty => '0',  -- switch for determining either a 10 or 20 bit interface
        sync_en => '1',         -- TRS/SYNC correction enabled
        rxdata_in => rxdata_ina,   -- scrambled data input from HOTLink II PHY
        rxdata_out => rx292data_outa--, -- descrambled and framed data
--        sync_err => ,       -- SYNC offset error pulse
--        h_sync => ,
	);        -- horizontal sync output toggle

U7B : smpte_292
	PORT MAP(
        -- scrambler tx I/O
        txclk => txclkb2,                         -- HOTLink TX CKW clock
        tx_reset => sd_hdb,                      -- active high reset
        tx_oe => '1',                         -- output enable
        tx_bypass => '0',       -- raw data mode - bypass scrambler
		tx_ten_twenty => '0',  -- switch for determining either a 10 or 20 bit interface
        trs_filt_lower10 => '0',-- TRS filter lower 10 bits
        trs_filt_upper10 => '0',-- TRS filter upper 10 bits
        txdata_in => tx292data_inb, -- raw data in from Co-Processor
--        trs_det => ,        -- TRS code detected
        txdata_out => tx292data_outb, -- parallel scrambled output to Hotlink II PHY
        -- de-scrambler rx I/O
		rxclk => rxclkb2,                         -- CY7B933 CKR recovered clock
        rx_reset => sd_hdb,                      -- active high reset
        rx_oe => '1',                         -- output enable
        rx_bypass => '0',       -- raw data mode, bypass scrambler data
		rx_ten_twenty => '0',  -- switch for determining either a 10 or 20 bit interface
        sync_en => '1',         -- TRS/SYNC correction enabled
        rxdata_in => rxdata_inb,   -- scrambled data input from HOTLink II PHY
        rxdata_out => rx292data_outb--, -- descrambled and framed data
--        sync_err => ,       -- SYNC offset error pulse
--        h_sync => ,
	);        -- horizontal sync output toggle
	
	U10AY : crc292check
		PORT MAP(
			clk => rxclka2, reset => reset_crca,--	: IN STD_LOGIC;
			data_in	=> rx292data_outa(10 to 19),--	: IN STD_LOGIC_VECTOR(0 to 9);
			readcrc => readYcrca,
			crc_errors => Y_crc_errorsa  -- : BUFFER STD_LOGIC_VECTOR(7 DOWNTO 0)
		);

	U10AC : crc292check
		PORT MAP(
			clk => rxclka2, reset => reset_crca,--	: IN STD_LOGIC;
			data_in	=> rx292data_outa(0 to 9),--	: IN STD_LOGIC_VECTOR(0 to 9);
			readcrc => readCcrca,
			crc_errors => C_crc_errorsa  -- : BUFFER STD_LOGIC_VECTOR(7 DOWNTO 0)
		);

	U10BY : crc292check
		PORT MAP(
			clk => rxclkb2, reset => reset_crcb,--	: IN STD_LOGIC;
			data_in	=> rx292data_outb(10 to 19),--	: IN STD_LOGIC_VECTOR(0 to 9);
			readcrc => readYcrcb,
			crc_errors => Y_crc_errorsb  -- : BUFFER STD_LOGIC_VECTOR(7 DOWNTO 0)
		);

	U10BC : crc292check
		PORT MAP(
			clk => rxclkb2, reset => reset_crcb,--	: IN STD_LOGIC;
			data_in	=> rx292data_outb(0 to 9),--	: IN STD_LOGIC_VECTOR(0 to 9);
			readcrc => readCcrcb,
			crc_errors => C_crc_errorsb  -- : BUFFER STD_LOGIC_VECTOR(7 DOWNTO 0)
		);

reset_crca <= '1' when (reset_n='0') or freqa /= "11" or lfia = '0' else '0';
reset_crcb <= '1' when (reset_n='0') or freqb /= "11" or lfib = '0' else '0';

	ucreset <= '1' when (reset_n = '0' or txsrca /= "111" or lfib = '0') else '0';
	upconverta : upconvert
		PORT MAP(
			clk27 => rxclkb_p, clk74 => txclka2, reset => not reset_n,
			data_in	=> vd_data_out_b,
			line_count => vd_linecount_b,
			sample_count => vd_wordcount_b(10 downto 0),
			video_type => video_type_b,
			data_out => updata
		);

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

process--(freqb,tx292data_outb,txdata_outb)
begin
	wait until txclkb = '1';
	if freqb = "11" then
		txdb(0) <=  tx292data_outb(0);
		txdb(1) <=  tx292data_outb(1);
		txdb(2) <=  tx292data_outb(2);
		txdb(3) <=  tx292data_outb(3);
		txdb(4) <=  tx292data_outb(4);
		txdb(5) <=  tx292data_outb(5);
		txdb(6) <=  tx292data_outb(6);
		txdb(7) <=  tx292data_outb(7);
		txctb(0) <= tx292data_outb(8);
		txctb(1) <= tx292data_outb(9);
	else
		txdb(0) <=  txdata_outb(0);
		txdb(1) <=  txdata_outb(1);
		txdb(2) <=  txdata_outb(2);
		txdb(3) <=  txdata_outb(3);
		txdb(4) <=  txdata_outb(4);
		txdb(5) <=  txdata_outb(5);
		txdb(6) <=  txdata_outb(6);
		txdb(7) <=  txdata_outb(7);
		txctb(0) <= txdata_outb(8);
		txctb(1) <= txdata_outb(9);
	end if;
end process;

PROCESS
BEGIN
	WAIT UNTIL txclka = '1';
	txclka2 <= not txclka2;
END PROCESS;

PROCESS
BEGIN
	WAIT UNTIL txclkb = '1';
	txclkb2 <= not txclkb2;
END PROCESS;

process
begin
	wait until rxclka_p = '1';
	rxclka2 <= not rxclka2;
end process;

process
begin
	wait until rxclkb_p = '1';
	rxclkb2 <= not rxclkb2;
end process;

process
begin
	wait until rxclka_n = '1';
	rxdata_ina <= rxsta(1 downto 0) & reverse_bit_order(rxda,8);
end process;

process
begin
	wait until rxclkb_n = '1';
	rxdata_inb <= rxstb(1 downto 0) & reverse_bit_order(rxdb,8);
end process;

END a;
