-- Copyright 2003 Linear Systems Ltd.

use work.pack_reverse_bit_order.all;
LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;

ENTITY hdvb1 IS
	PORT(
		-- cyv15g0403 interface
		txclkoc, txclkod		: IN	STD_LOGIC;
		rxclkc_p, rxclkd_p		: IN	STD_LOGIC;
		rxclkc_n, rxclkd_n		: IN	STD_LOGIC;
		cd_muted2				: IN	STD_LOGIC;
		rxdc, rxdd				: IN	STD_LOGIC_VECTOR(7 DOWNTO 0);
		rxstc, rxstd			: IN	STD_LOGIC_VECTOR(2 DOWNTO 0);
		lfic, lfid				: IN	STD_LOGIC;
		txerrc, txerrd			: IN STD_LOGIC;
		cd_mutec, cd_muted		: INOUT STD_LOGIC;
		txclkc, txclkd			: BUFFER	STD_LOGIC;
		txdc, txdd				: OUT	STD_LOGIC_VECTOR(7 DOWNTO 0);
		txctc, txctd			: OUT	STD_LOGIC_VECTOR(1 DOWNTO 0);
		sd_hdc, sd_hdd			: BUFFER 	STD_LOGIC;
		rclkenc, rclkend		: OUT STD_LOGIC;
		lpenc, lpend			: OUT	STD_LOGIC;
		ulcc, ulcd				: OUT	STD_LOGIC;
		inselc, inseld			: BUFFER	STD_LOGIC;
		spdselc, spdseld		: BUFFER	STD_LOGIC;
		
		fclkc_p, fclkc_n, fclkd_p, fclkd_n			: OUT STD_LOGIC;
		ssi_cdc2				: IN STD_LOGIC;

		-- Parallel Interfaces
		ptxdc, ptxdd			: IN	STD_LOGIC_VECTOR(9 downto 0);
		prxdc, prxdd			: BUFFER	STD_LOGIC_VECTOR(9 DOWNTO 0);
		prxclkc, prxclkd		: OUT STD_LOGIC;
		ptxclkc, ptxclkd		: IN STD_LOGIC;

		-- led's
		led270c, led270d, led360c, led360d	: OUT STD_LOGIC;	-- LED OUTPUTS
		led540c, led540d, led7425c, led7425d	: OUT STD_LOGIC;
		ledlfic, ledlfid		: BUFFER STD_LOGIC;
		ledcdc, ledcdd			: BUFFER STD_LOGIC;
		led2					: OUT STD_LOGIC;
		
		-- fx2/gpif interface
		clkout, reset_n							: IN	STD_LOGIC;
		ifclk									: INOUT	STD_LOGIC;
		pa7_flagd_slcs							: INOUT	STD_LOGIC;
		rdy										: BUFFER	STD_LOGIC_VECTOR(1 DOWNTO 0);
		ctl										: IN	STD_LOGIC_VECTOR(2 DOWNTO 0);
		fd										: INOUT	STD_LOGIC_VECTOR(15 DOWNTO 0);
		
		-- spi interface
		scse1, scl, sdo							: IN	STD_LOGIC;
		sdi										: BUFFER	STD_LOGIC);
END hdvb1;

ARCHITECTURE a OF hdvb1 IS
	COMPONENT spi
		PORT(
			scs, scl, sdo		: IN	STD_LOGIC;
			sdi					: OUT	STD_LOGIC;
			read, write 		: OUT	STD_LOGIC;
			channel				: OUT	STD_LOGIC;
			address				: BUFFER	STD_LOGIC_VECTOR(7 DOWNTO 0);
			data_out			: BUFFER	STD_LOGIC_VECTOR(7 DOWNTO 0);
			data_in				: IN	STD_LOGIC_VECTOR(7 DOWNTO 0)
		);
	END COMPONENT;
	
	COMPONENT channelregs
		PORT(
			reset, write, read	: IN	STD_LOGIC;
			channel, chsel		: IN 	STD_LOGIC;
			address				: IN	STD_LOGIC_VECTOR(7 DOWNTO 0);
			data_in				: INOUT	STD_LOGIC_VECTOR(7 DOWNTO 0);
			data_out			: OUT	STD_LOGIC_VECTOR(7 DOWNTO 0);
			century, year, month, day : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
			video_type			:IN STD_LOGIC_VECTOR(2 DOWNTO 0);
			lfi, cd, cd2		: IN	STD_LOGIC;
			dvb_nsmpte, insel	: OUT 	STD_LOGIC;
			lpen			: OUT 	STD_LOGIC;
			chreset			: OUT 	STD_LOGIC;	--  channel reset
			pal_nntsc		: OUT	STD_LOGIC;
			freq			: OUT	STD_LOGIC_VECTOR(1 DOWNTO 0);
			next_freq			: in	STD_LOGIC_VECTOR(1 DOWNTO 0);	-- next frequency select input (for auto rate detection)
			txsrc			: out 	STD_LOGIC_VECTOR(2 DOWNTO 0);
			read_acrc_errors			: OUT	STD_LOGIC;		-- read output to edh acrc error counter
			read_fcrc_errors			: OUT	STD_LOGIC;		-- read output to edh fcrc error counter
			acrc_errors, fcrc_errors	: IN STD_LOGIC_VECTOR( 7 DOWNTO 0);  -- crc error counts from edh module
			readYcrc, readCcrc		: OUT 	STD_LOGIC;
			Y_crc_errors, C_crc_errors : in std_logic_vector(31 downto 0)
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

	COMPONENT tpgen
		PORT(
			reset_n, clk, pal_nntsc		: IN	STD_LOGIC;
			txsrc			: IN	STD_LOGIC_VECTOR(2 DOWNTO 0);
			txrate			: IN	STD_LOGIC_VECTOR(1 DOWNTO 0);
			data_out		: OUT STD_LOGIC_VECTOR(9 DOWNTO 0)
			);
	END COMPONENT;

	COMPONENT eg1_292
		PORT( 
			clk,enable	 : in std_logic;
			Y_data : out std_logic_vector(9 downto 0);
			C_data : out STD_LOGIC_VECTOR(9 DOWNTO 0)
		);
	END COMPONENT;
	
	component hdsdi_ffebu_cb is
    	port (
			clk :       in std_logic;   
        	mute :      in std_logic;                       -- mutes generator when 0
        	hdformat :  in std_logic_vector(1 downto 0);                       
			luma :      out std_logic_vector(9 downto 0);   -- output to scrambler
			chroma :    out std_logic_vector(9 downto 0)    -- output to scrambler
		    );
	end component;
	
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
	
	SIGNAL address, data_in, data_out, read_data0, read_data1 : STD_LOGIC_VECTOR(7 DOWNTO 0);
	SIGNAL channel, read, write, dvb_nsmptec, dvb_nsmpted, pal_nntscc, pal_nntscd : STD_LOGIC;
	SIGNAL freqc, freqd: STD_LOGIC_VECTOR(1 DOWNTO 0);
	SIGNAL txsrcc, txsrcd : STD_LOGIC_VECTOR(2 DOWNTO 0);
	signal rxdata_inc,rxdata_outc, txdata_outc, txdata_inc : std_logic_vector(0 to 9);
	signal rxdata_ind,rxdata_outd, txdata_outd, txdata_ind : std_logic_vector(0 to 9);
	signal h_syncc,sync_errorc, ena_outc,trs_outc,rfa,abc : std_logic;
	signal h_syncd,sync_errord, ena_outd,trs_outd,rfb,abd : std_logic;
	signal SYNC_enc, trs_filtc : std_logic;
	signal SYNC_end, trs_filtd : std_logic;
	signal tpdatac, tpdatad : std_logic_vector(9 downto 0);
	signal tpdatac0, tpdatad0 : std_logic_vector(0 to 9);
	signal txclkc2, txclkd2, rxclkc2, rxclkd2, enable_eg1_292c, enable_eg1_292d : std_logic;
	signal enable_rp198c, enable_rp198d : std_logic;
	signal Ydatac, Cdatac, Ydatad, Cdatad : std_logic_vector(9 downto 0);
	signal rp198_Ydatac, rp198_Cdatac, rp198_Ydatad, rp198_Cdatad : std_logic_vector(9 downto 0);
	signal grey_292_Ydatac, grey_292_Cdatac, grey_292_Ydatad, grey_292_Cdatad : std_logic_vector(9 downto 0);
	signal enable_grey_292c, enable_grey_292d : std_logic;
	signal smpte292enablec, smpte292enabled : std_logic;
	signal tx292data_inc, tx292data_ind : std_logic_vector(0 to 19);
	signal tx292data_outc, tx292data_outd : std_logic_vector(0 to 9);
	signal rx292data_outc, rx292data_outd : std_logic_vector(0 to 19);
	signal lrx292data_outc, lrx292data_outd : std_logic_vector(0 to 19);
	SIGNAL century, year, month, day : std_logic_vector(7 downto 0);
	signal Y_crc_errorsc, C_crc_errorsc : std_logic_vector(31 downto 0);
	signal Y_crc_errorsd, C_crc_errorsd : std_logic_vector(31 downto 0);
	signal reset_crcc, reset_crcd : std_logic;
	signal readYcrcc, readCcrcc, readYcrcd, readCcrcd : std_logic;
	signal vd_data_in_c, vd_data_out_c : std_logic_vector(9 downto 0);
	signal vd_linecount_c : std_logic_vector(9 downto 0);
	signal vd_wordcount_c : std_logic_vector(11 downto 0);
	signal video_type_c : std_logic_vector(2 downto 0);
	signal vd_data_in_d, vd_data_out_d : std_logic_vector(9 downto 0);
	signal vd_linecount_d : std_logic_vector(9 downto 0);
	signal vd_wordcount_d : std_logic_vector(11 downto 0);
	signal video_type_d : std_logic_vector(2 downto 0);
	signal acrc_errors_c, fcrc_errors_c : std_logic_vector(7 downto 0);
	signal read_acrc_errors_c, read_fcrc_errors_c : std_logic;
	signal acrc_errors_d, fcrc_errors_d : std_logic_vector(7 downto 0);
	signal read_acrc_errors_d, read_fcrc_errors_d : std_logic;
	signal next_freqc, next_freqd, hdformat : std_logic_vector(1 downto 0);
	signal chresetc, chresetd : std_logic;
	
BEGIN

	hdformat <= "00";
	
	-- Process Statement

	-- Concurrent Procedure Call

	-- Concurrent Signal Assignment
	century	<= x"20";
	year	<= x"04";
	month 	<= x"09";
	day		<= x"28";
	txclkc <= txclkoc;
	txclkd <= txclkod;
	ulcc <= '1';
	ulcd <= '1';
--	ldtden <= '1';
	led2 <= '0';
	rdy(0)<='0';
	rdy(1)<='0';
	
	tpdatac0 <= reverse_bit_order(tpdatac,10);
	
	tpdatad0 <= reverse_bit_order(tpdatad,10);
	
--	prxdc <= reverse_bit_order(rxdata_inc,10);

--	prxdd <= reverse_bit_order(rxdata_ind,10);

process --(txsrca,Cdataa,Ydataa,lrx292data_outa)
begin
WAIT UNTIL txclkc2='1';
	if(txsrcc="000")then
		tx292data_inc <= reverse_bit_order(Cdatac,10)&reverse_bit_order(Ydatac,10);
	elsif(txsrcc="001")then
		tx292data_inc <= reverse_bit_order(grey_292_Cdatac,10)&reverse_bit_order(grey_292_Ydatac,10);
	elsif txsrcc(2 DOWNTO 1) = "01" then
		tx292data_inc <= reverse_bit_order(rp198_Cdatac,10)&reverse_bit_order(rp198_Ydatac,10);
	elsif txsrcc = "101" then
		tx292data_inc <= lrx292data_outc;
	else 
		tx292data_inc <= "00000000000000000000";
	end if;
end process;

process
begin
	wait until txclkc2 = '1';
	lrx292data_outc <= rx292data_outc;
end process;

process --(txsrcb,Cdatab,Ydatab,lrx292data_outb)
begin
WAIT UNTIL txclkd2 = '1';
	if txsrcd = "000" then
		tx292data_ind <= reverse_bit_order(Cdatad,10)&reverse_bit_order(Ydatad,10);
	elsif(txsrcd="001")then
		tx292data_ind <= reverse_bit_order(grey_292_Cdatad,10)&reverse_bit_order(grey_292_Ydatad,10);
	elsif txsrcd(2 DOWNTO 1) = "01" then
		tx292data_ind <= reverse_bit_order(rp198_Cdatad,10)&reverse_bit_order(rp198_Ydatad,10);
	elsif txsrcd = "101" then
		tx292data_ind <= lrx292data_outd;
	else 
		tx292data_ind <= "00000000000000000000";
	end if;
end process;

process
begin
	wait until txclkd2 = '1';
	lrx292data_outd <= rx292data_outd;
end process;

	ledlfic <= not lfic;
	ledlfid <= not lfid;
	
	-- Conditional Signal Assignment
	led270c <= '1' when freqc = "00" else '0';
	led360c <= '1' when freqc = "01" else '0';
	led540c <= '1' when freqc = "10" else '0';
	led7425c <= '1' when freqc = "11" else '0';
	led270d <= '1' when freqd = "00" else '0';
	led360d <= '1' when freqd = "01" else '0';
	led540d <= '1' when freqd = "10" else '0';
	led7425d <= '1' when freqd = "11" else '0';
	
	smtpe259txmuxc:
	txdata_inc <= 	tpdatac0 when txsrcc <= "011" else
					ptxdc when txsrcc = "100" else
					rxdata_outc;
	smpte259txmuxd:
	txdata_ind <=	tpdatad0 WHEN txsrcd <= "011" ELSE
					ptxdd when txsrcd = "100" else
					rxdata_outd;

	rclkenc <= '1' when txsrcc ="110" ELSE '0';
	rclkend <= '1' when txsrcd ="110" ELSE '0';

	ledcdc <= not cd_mutec when inselc = '1' else ssi_cdc2;
	ledcdd <= not cd_muted when inseld = '1' else cd_muted2;

	enable_eg1_292c <= '1' when freqc="11" and txsrcc="000" else '0';
	enable_eg1_292d <= '1' when freqd="11" and txsrcd="000" else '0';
	enable_grey_292c <= '1' when freqc="11" and txsrcc="001" else '0';
	enable_grey_292d <= '1' when freqd="11" and txsrcd="001" else '0';
	enable_rp198c <= '1' when freqc="11" and txsrcc(2 DOWNTO 1) = "01" else '0';
	enable_rp198d <= '1' when freqd="11" and txsrcd(2 DOWNTO 1) = "01" else '0';

	-- Selected Signal Assignment
	channelmux:
	WITH channel SELECT
		data_in	<=	read_data0	WHEN	'0',
					read_data1	WHEN	'1';

	WITH freqc SELECT
		sd_hdc <= '0' WHEN "11",
					'1' WHEN others;
	WITH freqc SELECT
		spdselc <= '0' WHEN "00",
					'0' WHEN "01",
					'Z' WHEN "10",
					'1' WHEN "11";
	WITH freqd SELECT
		sd_hdd <= '0' WHEN "11", -- for smpte292m
					'1' WHEN others; -- for smpte259m, smpte344m, dvb-asi
	WITH freqd SELECT
		spdseld <= '0' WHEN "00",
					'0' WHEN "01",
					'Z' WHEN "10",
					'1' WHEN "11";
					
u1: spi 
	PORT MAP	(
		scs => scse1, scl => scl, sdo => sdo,
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
		video_type => video_type_c,
		lfi => lfic, cd => cd_mutec, cd2 => ssi_cdc2,
		dvb_nsmpte => dvb_nsmptec, insel => inselc,
		lpen => lpenc, chreset => chresetc,
		pal_nntsc => pal_nntscc,
		freq => freqc, next_freq => next_freqc,
		txsrc => txsrcc,
		read_acrc_errors => read_acrc_errors_c,--			: OUT	STD_LOGIC;		-- read output to edh acrc error counter
		read_fcrc_errors => read_fcrc_errors_c,--			: OUT	STD_LOGIC;		-- read output to edh fcrc error counter
		acrc_errors => acrc_errors_c, fcrc_errors =>fcrc_errors_c,-- 	: IN STD_LOGIC_VECTOR( 7 DOWNTO 0);  -- crc error counts from edh module
		readYcrc => readYcrcc, readCcrc =>	readCcrcc,
		Y_crc_errors => Y_crc_errorsc, C_crc_errors => C_crc_errorsc
	);
	
u3: channelregs
	PORT MAP(
		reset => NOT reset_n, write => write, read => read,
		channel => channel, chsel => '1',
		address => address,
		data_in => data_out,
		data_out => read_data1,
		century => century, year => year, month => month, day => day,
		video_type => video_type_d,
		lfi => lfid, cd => cd_muted, cd2 => cd_muted2,
		dvb_nsmpte => dvb_nsmpted, insel => inseld,
		lpen => lpend, chreset => chresetd,
		pal_nntsc => pal_nntscd,
		freq => freqd, next_freq => next_freqd,
		txsrc => txsrcd,
		read_acrc_errors => read_acrc_errors_d,--			: OUT	STD_LOGIC;		-- read output to edh acrc error counter
		read_fcrc_errors => read_fcrc_errors_d,--			: OUT	STD_LOGIC;		-- read output to edh fcrc error counter
		acrc_errors => acrc_errors_d, fcrc_errors =>fcrc_errors_d,-- 	: IN STD_LOGIC_VECTOR( 7 DOWNTO 0);  -- crc error counts from edh module
		readYcrc => readYcrcd, readCcrc =>	readCcrcd,
		Y_crc_errors => Y_crc_errorsd, C_crc_errors => C_crc_errorsd
	);

u13a: autorate
		PORT MAP(
			clkout => clkout, reset => chresetc, lfi => ledlfic, cd => ledcdc,
			freq => freqc,
			next_freq => next_freqc
			);

u13b: autorate
		PORT MAP(
			clkout => clkout, reset => chresetd, lfi => ledlfid, cd => ledcdd,
			freq => freqd,
			next_freq => next_freqd
			);

u4a: smpte_core
	PORT MAP(
        rxclk => rxclkc_p,
		reset_n => reset_n,
        DVB_select => '1',
        rxbypass => '0',
        SYNC_en => '1',--SYNC_ena,
        rxoe => '1',
        data_in => rxdata_inc,
        rxdata_out => rxdata_outc,
        SYNC_error => SYNC_errorc,
--        RF => RF,
--        AB => AB,
        H_SYNC => H_SYNCc,
        txclk => txclkc,
        DVB_EN => '1',
        trs_filt => '0',
        ena_in => '0',
        enn_in => '0',
        SVS_en => '0',
        SCD_en => '0',
        txoe => '1',
        txbypass => '0',
                                    -- 10-bit raw data interface
        data => txdata_inc,
        trs_out => trs_outc,
        ena_out => ena_outc,
        txdata_out => txdata_outc
	);
	
vd_data_in_c(0) <= rxdata_outc(0);
vd_data_in_c(1) <= rxdata_outc(1);
vd_data_in_c(2) <= rxdata_outc(2);
vd_data_in_c(3) <= rxdata_outc(3);
vd_data_in_c(4) <= rxdata_outc(4);
vd_data_in_c(5) <= rxdata_outc(5);
vd_data_in_c(6) <= rxdata_outc(6);
vd_data_in_c(7) <= rxdata_outc(7);
vd_data_in_c(8) <= rxdata_outc(8);
vd_data_in_c(9) <= rxdata_outc(9);

U11A: video_detect
	PORT MAP(
		clk => rxclkc_p, reset => not reset_n,						--: IN	STD_LOGIC;							-- clock and reset lines
		data_in => vd_data_in_c,							--: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);		-- 10 bit video data in
		data_out => vd_data_out_c,						--: buffer	STD_LOGIC_VECTOR(9 DOWNTO 0);	-- 10 bit video data out
		count_out => vd_wordcount_c,						--: buffer 	std_logic_vector(11 downto 0);	-- sample count, valid if linecount != 0, syncronized to data_out
		video_type_out => video_type_c,					--: buffer 	std_logic_vector(2 downto 0);	-- video type indicator
		linecount_out => vd_linecount_c					--: buffer	std_logic_vector(9 downto 0)	-- video line number, syncronized to data_out
	);
U12A: edh
	PORT MAP(
		clk => rxclkc_p, enable	=> reset_n,--				: IN	STD_LOGIC;
		data_in=> vd_data_out_c,--						: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);
		linecount =>  vd_linecount_c,--					: IN 	STD_LOGIC_VECTOR(9 DOWNTO 0);
		wordcount => vd_wordcount_c,-- 					: IN 	STD_LOGIC_VECTOR(11 DOWNTO 0);
		video_type => video_type_c,--					: IN 	STD_LOGIC_VECTOR(2 DOWNTO 0);
		read_acrc_errors => read_acrc_errors_c,--			: IN	STD_LOGIC;
		read_fcrc_errors => read_fcrc_errors_c,--			: IN	STD_LOGIC;
		acrc_errors => acrc_errors_c, fcrc_errors => fcrc_errors_c,--	: BUFFER STD_LOGIC_VECTOR( 7 DOWNTO 0);
		data_out => prxdc--					: BUFFER STD_LOGIC_VECTOR(9 DOWNTO 0)
	);

u4b: smpte_core
	PORT MAP(
        rxclk => rxclkd_p,
		reset_n => reset_n,
        DVB_select => '1',
        rxbypass => '0',
        SYNC_en => SYNC_end,
        rxoe => '1',
        data_in => rxdata_ind,
        rxdata_out => rxdata_outd,
        SYNC_error => SYNC_errord,
 --       RF => RF,
 --       AB => AB,
        H_SYNC => H_SYNCd,
        txclk => txclkd,
        DVB_EN => '1',
        trs_filt => '0',
        ena_in => '0',
        enn_in => '0',
        SVS_en => '0',
        SCD_en => '0',
        txoe => '1',
        txbypass => '0',
                                    -- 10-bit raw data interface
        data => txdata_ind,
        trs_out => trs_outd,
        ena_out => ena_outd,
        txdata_out => txdata_outd
	);
	
vd_data_in_d(0) <= rxdata_outd(0);
vd_data_in_d(1) <= rxdata_outd(1);
vd_data_in_d(2) <= rxdata_outd(2);
vd_data_in_d(3) <= rxdata_outd(3);
vd_data_in_d(4) <= rxdata_outd(4);
vd_data_in_d(5) <= rxdata_outd(5);
vd_data_in_d(6) <= rxdata_outd(6);
vd_data_in_d(7) <= rxdata_outd(7);
vd_data_in_d(8) <= rxdata_outd(8);
vd_data_in_d(9) <= rxdata_outd(9);

U11B: video_detect
	PORT MAP(
		clk => rxclkd_p, reset => not reset_n,						--: IN	STD_LOGIC;							-- clock and reset lines
		data_in => vd_data_in_d,							--: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);		-- 10 bit video data in
		data_out => vd_data_out_d,						--: buffer	STD_LOGIC_VECTOR(9 DOWNTO 0);	-- 10 bit video data out
		count_out => vd_wordcount_d,						--: buffer 	std_logic_vector(11 downto 0);	-- sample count, valid if linecount != 0, syncronized to data_out
		video_type_out => video_type_d,					--: buffer 	std_logic_vector(2 downto 0);	-- video type indicator
		linecount_out => vd_linecount_d					--: buffer	std_logic_vector(9 downto 0)	-- video line number, syncronized to data_out
	);
U12B: edh
	PORT MAP(
		clk => rxclkd_p, enable	=> reset_n,--				: IN	STD_LOGIC;
		data_in=> vd_data_out_d,--						: IN	STD_LOGIC_VECTOR(9 DOWNTO 0);
		linecount =>  vd_linecount_d,--					: IN 	STD_LOGIC_VECTOR(9 DOWNTO 0);
		wordcount => vd_wordcount_d,-- 					: IN 	STD_LOGIC_VECTOR(11 DOWNTO 0);
		video_type => video_type_d,--					: IN 	STD_LOGIC_VECTOR(2 DOWNTO 0);
		read_acrc_errors => read_acrc_errors_d,--			: IN	STD_LOGIC;
		read_fcrc_errors => read_fcrc_errors_d,--			: IN	STD_LOGIC;
		acrc_errors => acrc_errors_d, fcrc_errors => fcrc_errors_d,--	: BUFFER STD_LOGIC_VECTOR( 7 DOWNTO 0);
		data_out => prxdd--					: BUFFER STD_LOGIC_VECTOR(9 DOWNTO 0)
	);

U5A: tpgen
	PORT MAP(
		reset_n => reset_n, clk => txclkc, pal_nntsc => pal_nntscc,
		txsrc => txsrcc,
		txrate => freqc,
		data_out => tpdatac
		);
		
U5B: tpgen
	PORT MAP(
		reset_n => reset_n, clk => txclkd, pal_nntsc => pal_nntscd,
		txsrc => txsrcd,
		txrate => freqd,
		data_out => tpdatad
		);
		
U6A: 	eg1_292
	PORT MAP(
		clk => txclkc2, enable => enable_eg1_292c,
--		sel : IN STD_LOGIC_VECTOR(1 DOWNTO 0);
		Y_data => Ydatac,
		C_data => Cdatac
	);
	
U6B: 	eg1_292
	PORT MAP(
		clk => txclkd2, enable => enable_eg1_292d,
		Y_data => Ydatad,
		C_data => Cdatad
	);
	
U8A: 	rp198
	PORT MAP(
		clk => txclkc2, enable => enable_rp198c,
		std_altn => not txsrcc(0),	-- '1' = standard distribution, '0' = alternate distribution
		Y_data => rp198_Ydatac,
		C_data => rp198_Cdatac
	);
	
U8B: 	rp198
	PORT MAP(
		clk => txclkd2, enable => enable_rp198d,
		std_altn => not txsrcd(0),	-- '1' = standard distribution, '0' = alternate distribution
		Y_data => rp198_Ydatad,
		C_data => rp198_Cdatad
	);
	
U9A:	hdsdi_ffebu_cb	-- i stedet for channel C - grey_292
	PORT MAP(
		clk => txclkc2, mute => enable_grey_292c,
		hdformat => hdformat,
		luma => grey_292_Ydatac,
		chroma => grey_292_Cdatac
	);
	
U9B:	grey_292
	PORT MAP(
		clk => txclkd2, enable => enable_grey_292d,
		Y_data => grey_292_Ydatad,
		C_data => grey_292_Cdatad
	);
	
U7A : smpte_292
	PORT MAP(
        -- scrambler tx I/O
        txclk => txclkc2,                         -- HOTLink TX CKW clock
        tx_reset => sd_hdc,                      -- active high reset
        tx_oe => '1',                         -- output enable
        tx_bypass => '0',       -- raw data mode - bypass scrambler
		tx_ten_twenty => '0',  -- switch for determining either a 10 or 20 bit interface
        trs_filt_lower10 => '0',-- TRS filter lower 10 bits
        trs_filt_upper10 => '0',-- TRS filter upper 10 bits
        txdata_in => tx292data_inc, -- raw data in from Co-Processor
--        trs_det => ,        -- TRS code detected
        txdata_out => tx292data_outc, -- parallel scrambled output to Hotlink II PHY
        -- de-scrambler rx I/O
		rxclk => rxclkc2,                         -- CY7B933 CKR recovered clock
        rx_reset => sd_hdc,                      -- active high reset
        rx_oe => '1',                         -- output enable
        rx_bypass => '0',       -- raw data mode, bypass scrambler data
		rx_ten_twenty => '0',  -- switch for determining either a 10 or 20 bit interface
        sync_en => '1',         -- TRS/SYNC correction enabled
        rxdata_in => rxdata_inc,   -- scrambled data input from HOTLink II PHY
        rxdata_out => rx292data_outc--, -- descrambled and framed data
--        sync_err => ,       -- SYNC offset error pulse
--        h_sync => ,
	);        -- horizontal sync output toggle

U7B : smpte_292
	PORT MAP(
        -- scrambler tx I/O
        txclk => txclkd2,                         -- HOTLink TX CKW clock
        tx_reset => sd_hdd,                      -- active high reset
        tx_oe => '1',                         -- output enable
        tx_bypass => '0',       -- raw data mode - bypass scrambler
		tx_ten_twenty => '0',  -- switch for determining either a 10 or 20 bit interface
        trs_filt_lower10 => '0',-- TRS filter lower 10 bits
        trs_filt_upper10 => '0',-- TRS filter upper 10 bits
        txdata_in => tx292data_ind, -- raw data in from Co-Processor
--        trs_det => ,        -- TRS code detected
        txdata_out => tx292data_outd, -- parallel scrambled output to Hotlink II PHY
        -- de-scrambler rx I/O
		rxclk => rxclkd2,                         -- CY7B933 CKR recovered clock
        rx_reset => sd_hdd,                      -- active high reset
        rx_oe => '1',                         -- output enable
        rx_bypass => '0',       -- raw data mode, bypass scrambler data
		rx_ten_twenty => '0',  -- switch for determining either a 10 or 20 bit interface
        sync_en => '1',         -- TRS/SYNC correction enabled
        rxdata_in => rxdata_ind,   -- scrambled data input from HOTLink II PHY
        rxdata_out => rx292data_outd--, -- descrambled and framed data
--        sync_err => ,       -- SYNC offset error pulse
--        h_sync => ,
	);        -- horizontal sync output toggle
	
	U10CY : crc292check
		PORT MAP(
			clk => rxclkc2, reset => reset_crcc,--	: IN STD_LOGIC;
			data_in	=> rx292data_outc(10 to 19),--	: IN STD_LOGIC_VECTOR(0 to 9);
			readcrc => readYcrcc,
			crc_errors => Y_crc_errorsc  -- : BUFFER STD_LOGIC_VECTOR(7 DOWNTO 0)
		);

	U10CC : crc292check
		PORT MAP(
			clk => rxclkc2, reset => reset_crcc,--	: IN STD_LOGIC;
			data_in	=> rx292data_outc(0 to 9),--	: IN STD_LOGIC_VECTOR(0 to 9);
			readcrc => readCcrcc,
			crc_errors => C_crc_errorsc  -- : BUFFER STD_LOGIC_VECTOR(7 DOWNTO 0)
		);

	U10DY : crc292check
		PORT MAP(
			clk => rxclkd2, reset => reset_crcd,--	: IN STD_LOGIC;
			data_in	=> rx292data_outd(10 to 19),--	: IN STD_LOGIC_VECTOR(0 to 9);
			readcrc => readYcrcd,
			crc_errors => Y_crc_errorsd  -- : BUFFER STD_LOGIC_VECTOR(7 DOWNTO 0)
		);

	U10DC : crc292check
		PORT MAP(
			clk => rxclkd2, reset => reset_crcd,--	: IN STD_LOGIC;
			data_in	=> rx292data_outd(0 to 9),--	: IN STD_LOGIC_VECTOR(0 to 9);
			readcrc => readCcrcd,
			crc_errors => C_crc_errorsd  -- : BUFFER STD_LOGIC_VECTOR(7 DOWNTO 0)
		);

reset_crcc <= '1' when (reset_n='0') or freqc /= "11" or lfic = '0' else '0';
reset_crcd <= '1' when (reset_n='0') or freqd /= "11" or lfid = '0' else '0';

process
begin
	wait until txclkc = '1';
	if freqc = "11" then
		txdc(0) <=  tx292data_outc(0);
		txdc(1) <=  tx292data_outc(1);
		txdc(2) <=  tx292data_outc(2);
		txdc(3) <=  tx292data_outc(3);
		txdc(4) <=  tx292data_outc(4);
		txdc(5) <=  tx292data_outc(5);
		txdc(6) <=  tx292data_outc(6);
		txdc(7) <=  tx292data_outc(7);
		txctc(0) <= tx292data_outc(8);
		txctc(1) <= tx292data_outc(9);
	else
		txdc(0) <=  txdata_outc(0);
		txdc(1) <=  txdata_outc(1);
		txdc(2) <=  txdata_outc(2);
		txdc(3) <=  txdata_outc(3);
		txdc(4) <=  txdata_outc(4);
		txdc(5) <=  txdata_outc(5);
		txdc(6) <=  txdata_outc(6);
		txdc(7) <=  txdata_outc(7);
		txctc(0) <= txdata_outc(8);
		txctc(1) <= txdata_outc(9);
	end if;
end process;

process
begin
	wait until txclkd = '1';
	if freqd = "11" then
		txdd(0) <=  tx292data_outd(0);
		txdd(1) <=  tx292data_outd(1);
		txdd(2) <=  tx292data_outd(2);
		txdd(3) <=  tx292data_outd(3);
		txdd(4) <=  tx292data_outd(4);
		txdd(5) <=  tx292data_outd(5);
		txdd(6) <=  tx292data_outd(6);
		txdd(7) <=  tx292data_outd(7);
		txctd(0) <= tx292data_outd(8);
		txctd(1) <= tx292data_outd(9);
	else
		txdd(0) <=  txdata_outd(0);
		txdd(1) <=  txdata_outd(1);
		txdd(2) <=  txdata_outd(2);
		txdd(3) <=  txdata_outd(3);
		txdd(4) <=  txdata_outd(4);
		txdd(5) <=  txdata_outd(5);
		txdd(6) <=  txdata_outd(6);
		txdd(7) <=  txdata_outd(7);
		txctd(0) <= txdata_outd(8);
		txctd(1) <= txdata_outd(9);
	end if;
end process;

PROCESS
BEGIN
	WAIT UNTIL txclkc = '1';
	txclkc2 <= not txclkc2;
END PROCESS;

PROCESS
BEGIN
	WAIT UNTIL txclkd = '1';
	txclkd2 <= not txclkd2;
END PROCESS;

process
begin
	wait until rxclkc_p = '1';
	rxclkc2 <= not rxclkc2;
end process;

process
begin
	wait until rxclkd_p = '1';
	rxclkd2 <= not rxclkd2;
end process;

process
begin
	wait until rxclkc_n = '1';
	rxdata_inc <= rxstc(1 downto 0) & reverse_bit_order(rxdc,8);
end process;

process
begin
	wait until rxclkd_n = '1';
	rxdata_ind <= rxstd(1 downto 0) & reverse_bit_order(rxdd,8);
end process;

END a;
