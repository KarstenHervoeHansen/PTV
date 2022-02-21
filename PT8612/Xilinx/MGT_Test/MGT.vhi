
-- VHDL Instantiation Created from source file MGT.vhd -- 08:28:00 08/31/2005
--
-- Notes: 
-- 1) This instantiation template has been automatically generated using types
-- std_logic and std_logic_vector for the ports of the instantiated module
-- 2) To use this template to instantiate this entity, cut-and-paste and then edit

	COMPONENT MGT
	PORT(
		DADDR_IN : IN std_logic_vector(7 downto 0);
		DADDR_IN_BLK2 : IN std_logic_vector(7 downto 0);
		DCLK_IN : IN std_logic;
		DCLK_IN_BLK2 : IN std_logic;
		DEN_IN : IN std_logic;
		DEN_IN_BLK2 : IN std_logic;
		DI_IN : IN std_logic_vector(15 downto 0);
		DI_IN_BLK2 : IN std_logic_vector(15 downto 0);
		DWE_IN : IN std_logic;
		DWE_IN_BLK2 : IN std_logic;
		MGTCLKN_IN : IN std_logic;
		MGTCLKP_IN : IN std_logic;
		POWERDOWN_IN : IN std_logic;
		POWERDOWN_IN_BLK2 : IN std_logic;
		RXUSRCLK_IN : IN std_logic;
		RXUSRCLK_IN_BLK2 : IN std_logic;
		RXUSRCLK2_IN : IN std_logic;
		RXUSRCLK2_IN_BLK2 : IN std_logic;
		TXCHARDISPMODE_IN : IN std_logic_vector(3 downto 0);
		TXCHARDISPMODE_IN_BLK2 : IN std_logic_vector(3 downto 0);
		TXCHARDISPVAL_IN : IN std_logic_vector(3 downto 0);
		TXCHARDISPVAL_IN_BLK2 : IN std_logic_vector(3 downto 0);
		TXCLKSTABLE_IN : IN std_logic;
		TXCLKSTABLE_IN_BLK2 : IN std_logic;
		TXCRCCLK_IN : IN std_logic;
		TXCRCCLK_IN_BLK2 : IN std_logic;
		TXCRCDATAVALID_IN : IN std_logic;
		TXCRCDATAVALID_IN_BLK2 : IN std_logic;
		TXCRCDATAWIDTH_IN : IN std_logic_vector(2 downto 0);
		TXCRCDATAWIDTH_IN_BLK2 : IN std_logic_vector(2 downto 0);
		TXCRCINIT_IN : IN std_logic;
		TXCRCINIT_IN_BLK2 : IN std_logic;
		TXCRCINTCLK_IN : IN std_logic;
		TXCRCINTCLK_IN_BLK2 : IN std_logic;
		TXCRCIN_IN : IN std_logic_vector(63 downto 0);
		TXCRCIN_IN_BLK2 : IN std_logic_vector(63 downto 0);
		TXCRCPD_IN : IN std_logic;
		TXCRCPD_IN_BLK2 : IN std_logic;
		TXCRCRESET_IN : IN std_logic;
		TXCRCRESET_IN_BLK2 : IN std_logic;
		TXDATAWIDTH_IN : IN std_logic_vector(1 downto 0);
		TXDATAWIDTH_IN_BLK2 : IN std_logic_vector(1 downto 0);
		TXDATA_IN : IN std_logic_vector(31 downto 0);
		TXDATA_IN_BLK2 : IN std_logic_vector(31 downto 0);
		TXENOOB_IN : IN std_logic;
		TXENOOB_IN_BLK2 : IN std_logic;
		TXINHIBIT_IN : IN std_logic;
		TXINHIBIT_IN_BLK2 : IN std_logic;
		TXINTDATAWIDTH_IN : IN std_logic_vector(1 downto 0);
		TXINTDATAWIDTH_IN_BLK2 : IN std_logic_vector(1 downto 0);
		TXPMARESET_IN : IN std_logic;
		TXPMARESET_IN_BLK2 : IN std_logic;
		TXPOLARITY_IN : IN std_logic;
		TXPOLARITY_IN_BLK2 : IN std_logic;
		TXRESET_IN : IN std_logic;
		TXRESET_IN_BLK2 : IN std_logic;
		TXSYNC_IN : IN std_logic;
		TXSYNC_IN_BLK2 : IN std_logic;
		TXUSRCLK_IN : IN std_logic;
		TXUSRCLK_IN_BLK2 : IN std_logic;
		TXUSRCLK2_IN : IN std_logic;
		TXUSRCLK2_IN_BLK2 : IN std_logic;          
		DO_OUT : OUT std_logic_vector(15 downto 0);
		DO_OUT_BLK2 : OUT std_logic_vector(15 downto 0);
		DRDY_OUT : OUT std_logic;
		DRDY_OUT_BLK2 : OUT std_logic;
		TXBUFERR_OUT : OUT std_logic;
		TXBUFERR_OUT_BLK2 : OUT std_logic;
		TXCALFAIL_OUT : OUT std_logic;
		TXCALFAIL_OUT_BLK2 : OUT std_logic;
		TXCRCOUT_OUT : OUT std_logic_vector(31 downto 0);
		TXCRCOUT_OUT_BLK2 : OUT std_logic_vector(31 downto 0);
		TXCYCLELIMIT_OUT : OUT std_logic;
		TXCYCLELIMIT_OUT_BLK2 : OUT std_logic;
		TXLOCK_OUT : OUT std_logic;
		TXLOCK_OUT_BLK2 : OUT std_logic;
		TXOUTCLK1_OUT : OUT std_logic;
		TXOUTCLK1_OUT_BLK2 : OUT std_logic;
		TXOUTCLK2_OUT : OUT std_logic;
		TXOUTCLK2_OUT_BLK2 : OUT std_logic;
		TXPCSHCLKOUT_OUT : OUT std_logic;
		TXPCSHCLKOUT_OUT_BLK2 : OUT std_logic;
		TXRUNDISP_OUT : OUT std_logic_vector(3 downto 0);
		TXRUNDISP_OUT_BLK2 : OUT std_logic_vector(3 downto 0);
		TX1N_OUT : OUT std_logic;
		TX1N_OUT_BLK2 : OUT std_logic;
		TX1P_OUT : OUT std_logic;
		TX1P_OUT_BLK2 : OUT std_logic
		);
	END COMPONENT;

	Inst_MGT: MGT PORT MAP(
		DADDR_IN => ,
		DADDR_IN_BLK2 => ,
		DCLK_IN => ,
		DCLK_IN_BLK2 => ,
		DEN_IN => ,
		DEN_IN_BLK2 => ,
		DI_IN => ,
		DI_IN_BLK2 => ,
		DWE_IN => ,
		DWE_IN_BLK2 => ,
		MGTCLKN_IN => ,
		MGTCLKP_IN => ,
		POWERDOWN_IN => ,
		POWERDOWN_IN_BLK2 => ,
		RXUSRCLK_IN => ,
		RXUSRCLK_IN_BLK2 => ,
		RXUSRCLK2_IN => ,
		RXUSRCLK2_IN_BLK2 => ,
		TXCHARDISPMODE_IN => ,
		TXCHARDISPMODE_IN_BLK2 => ,
		TXCHARDISPVAL_IN => ,
		TXCHARDISPVAL_IN_BLK2 => ,
		TXCLKSTABLE_IN => ,
		TXCLKSTABLE_IN_BLK2 => ,
		TXCRCCLK_IN => ,
		TXCRCCLK_IN_BLK2 => ,
		TXCRCDATAVALID_IN => ,
		TXCRCDATAVALID_IN_BLK2 => ,
		TXCRCDATAWIDTH_IN => ,
		TXCRCDATAWIDTH_IN_BLK2 => ,
		TXCRCINIT_IN => ,
		TXCRCINIT_IN_BLK2 => ,
		TXCRCINTCLK_IN => ,
		TXCRCINTCLK_IN_BLK2 => ,
		TXCRCIN_IN => ,
		TXCRCIN_IN_BLK2 => ,
		TXCRCPD_IN => ,
		TXCRCPD_IN_BLK2 => ,
		TXCRCRESET_IN => ,
		TXCRCRESET_IN_BLK2 => ,
		TXDATAWIDTH_IN => ,
		TXDATAWIDTH_IN_BLK2 => ,
		TXDATA_IN => ,
		TXDATA_IN_BLK2 => ,
		TXENOOB_IN => ,
		TXENOOB_IN_BLK2 => ,
		TXINHIBIT_IN => ,
		TXINHIBIT_IN_BLK2 => ,
		TXINTDATAWIDTH_IN => ,
		TXINTDATAWIDTH_IN_BLK2 => ,
		TXPMARESET_IN => ,
		TXPMARESET_IN_BLK2 => ,
		TXPOLARITY_IN => ,
		TXPOLARITY_IN_BLK2 => ,
		TXRESET_IN => ,
		TXRESET_IN_BLK2 => ,
		TXSYNC_IN => ,
		TXSYNC_IN_BLK2 => ,
		TXUSRCLK_IN => ,
		TXUSRCLK_IN_BLK2 => ,
		TXUSRCLK2_IN => ,
		TXUSRCLK2_IN_BLK2 => ,
		DO_OUT => ,
		DO_OUT_BLK2 => ,
		DRDY_OUT => ,
		DRDY_OUT_BLK2 => ,
		TXBUFERR_OUT => ,
		TXBUFERR_OUT_BLK2 => ,
		TXCALFAIL_OUT => ,
		TXCALFAIL_OUT_BLK2 => ,
		TXCRCOUT_OUT => ,
		TXCRCOUT_OUT_BLK2 => ,
		TXCYCLELIMIT_OUT => ,
		TXCYCLELIMIT_OUT_BLK2 => ,
		TXLOCK_OUT => ,
		TXLOCK_OUT_BLK2 => ,
		TXOUTCLK1_OUT => ,
		TXOUTCLK1_OUT_BLK2 => ,
		TXOUTCLK2_OUT => ,
		TXOUTCLK2_OUT_BLK2 => ,
		TXPCSHCLKOUT_OUT => ,
		TXPCSHCLKOUT_OUT_BLK2 => ,
		TXRUNDISP_OUT => ,
		TXRUNDISP_OUT_BLK2 => ,
		TX1N_OUT => ,
		TX1N_OUT_BLK2 => ,
		TX1P_OUT => ,
		TX1P_OUT_BLK2 => 
	);


