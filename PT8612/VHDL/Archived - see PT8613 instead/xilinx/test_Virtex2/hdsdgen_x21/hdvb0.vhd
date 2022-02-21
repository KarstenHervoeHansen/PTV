use work.pack_reverse_bit_order.all;
LIBRARY ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;

-- This allows simulation of Xilinx components
-- synopsys translate_off
library unisim;
use unisim.vcomponents.all;
-- synopsys translate_on

-- hdsdgen_02_nab :
-- projekt til NAB2005 exhibition, som skal konfigureres af
-- PT5300 mainframe ( kan ikke styres via USB-interface )

ENTITY hdvb0 IS
	PORT(
		-- from pt5300 mainframe
 		f7425					: IN STD_LOGIC;
		reset_n				: IN	STD_LOGIC;
		Debug9				: out std_logic_vector(9 downto 0);
	
		-- test point
		TP1					: OUT STD_LOGIC;
		TP2					: OUT STD_LOGIC;
		TP3					: OUT STD_LOGIC;
		TP4					: OUT STD_LOGIC;
		

		-- serial interface
		CS						: IN STD_LOGIC;
		SCK					: IN STD_LOGIC;
		MOSI					: IN STD_LOGIC;

      TXp_OUT_113a				: OUT STD_LOGIC;
	   TXn_OUT_113a				: OUT STD_LOGIC;

      TXp_OUT_113b				: OUT STD_LOGIC;
	   TXn_OUT_113b				: OUT STD_LOGIC;

 		f1484					: IN STD_LOGIC;
		f1485					: IN STD_LOGIC;
		f7417					: IN STD_LOGIC
		);
END hdvb0;

ARCHITECTURE a OF hdvb0 IS
 
-- This is a clock buffer with an associated low-skew clock distribution tree,
-- which is neccessary to avoid clock skew on the divided clock.
   component BUFG
    	 port(I : in  std_logic;
         	O : out std_logic);   
	end component;

	component MY_GT11 
		 port(  
       	RX1N_IN                      : in    std_logic; 
       	RX1P_IN                      : in    std_logic; 
       	TX1N_OUT                     : out   std_logic; 
       	TX1P_OUT                     : out   std_logic;
       	RXRECCLK1_OUT                : out   std_logic; 
       	TXOUTCLK1_OUT                : out   std_logic; 
       	TXOUTCLK2_OUT                : out   std_logic; 
       	RXUSRCLK2_IN                 : in    std_logic; 
       	TXUSRCLK_IN                  : in    std_logic; 
       	TXUSRCLK2_IN                 : in    std_logic; 
		   GREFCLK_IN                   : in    std_logic; 
      	RXDATA_OUT                   : out   std_logic_vector (7 downto 0);  
       	TXDATA_IN                    : in    std_logic_vector (19 downto 0));
    end component;

	component serial_interface is Port(
        mreset :        in std_logic;   -- master reset
        cs :            in std_logic;   -- chip select
        sck :           in std_logic;   -- serial clock
        mosi :          in std_logic;   -- master out serial data in
        offset :    		out std_logic_vector(23 downto 0);  -- delay from sync reference
        new_delay :     out std_logic;
		  sd_hd :			out std_logic;						-- LOW = SD, HIGH = HD
		  sd_format :		out std_logic;						-- LOW = 625 line system, HIGH = 525 line system
		  scan_format :	out std_logic;						-- LOW = progressive, HIGH = interlaced
		  hd_clk :			out std_logic;						-- LOW = 74,25 MHz, HIGH = 74,25/1.001 MHz
		  hd_format :		out std_logic_vector(3 downto 0);	-- high definition format
		  pattern :			out std_logic_vector(7 downto 0);	-- test pattern select
		  tp_option :		out std_logic_vector(7 downto 0);	-- test pattern option
		  tp :				out std_logic
        );
	end component;

	COMPONENT hd_frame
		PORT ( 
			clk :       	in std_logic;   
        	enable     :   in std_logic;
			mute :      	in std_logic;                       -- mutes generator when 0
        	hdformat :  	in std_logic_vector(3 downto 0);    -- SMPTE 240M, 274M or 296M
  	    	act_y :     	in std_logic_vector(9 downto 0);     -- input from videogenerator
        	act_cb :    	in std_logic_vector(9 downto 0);     -- input from videogenerator
       	act_cr :    	in std_logic_vector(9 downto 0);         
			scan_format : 	inout std_logic;					-- output for active video modul
			sav :          out std_logic;						-- output for active video modul
			fvh :          out std_logic_vector(2 downto 0);	-- output for active video modul
        	line_clk :     inout std_logic;
			luma :      	out std_logic_vector(9 downto 0);   -- output to scrambler
			chroma :    	out std_logic_vector(9 downto 0);    -- output to scrambler
			tp:				out std_logic
			);
	END COMPONENT;

	COMPONENT scram20_top  
    PORT (
        txclk:			   in std_logic;		-- HOTLink TX CKW clock
        enable_sync :	in std_logic;
        reset:			   in std_logic;            		-- active high reset
        tx_oe:			   in std_logic;           		-- output enable
        bypass:         in std_logic;       	-- raw data mode - bypass scrambler
        txdata_in:      in std_logic_vector(0 to 19); -- raw data in
        txdata_out:     out std_logic_vector(0 to 19)); -- parallel scrambled output
   END COMPONENT;

	component video_sm is
    	Port ( 
        	clk :           in std_logic;
			enable   :	    in std_logic;
	    	video_en :      in std_logic;
			sd_hd :			 in std_logic;
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
  			tp :		   	 out std_logic_vector(23 downto 0)
	        );
	end component;


	component os_controller is
	port(
	    clk_148 	  	:  in  std_logic;
	    reset		   :  in  std_logic;
	    hdsd_in	      :  in  std_logic;
	    data_in  		:  in  std_logic_vector(19 downto 0);
	    data_out 		:  out std_logic_vector(19 downto 0);
	    enable_out    :  out std_logic
	);
	end component;
	
	--	signal txclka2: std_logic;
	signal Ydataa, Cdataa : std_logic_vector(9 downto 0);
	signal tx292data_ina : std_logic_vector(0 to 19);
	signal tx292data_outa: std_logic_vector(0 to 19);
	signal hdformat :std_logic_vector(3 downto 0) := "0000";
	signal pattern,option : std_logic_vector(7 downto 0);
	signal act_y, act_cb, act_cr : std_logic_vector(9 downto 0);	-- active video samples from video_sm modul
	signal tp : std_logic_vector(23 downto 0);	-- testpoint output from video state machine
   signal line_clk, hd_mute, sav, hd_scanformat, scan_format :std_logic;
   signal fvh : std_logic_vector(2 downto 0);	-- field, vblank, hblank
	signal sif_scan_format,sif_tp, sif_hd_clk, sif_rst, sif_sck,  sif_sd_hda : std_logic;
	signal sif_pal_nntsca : std_logic;
	signal sif_hd_format : std_logic_vector(3 downto 0);
	signal sif_pattern, sif_option : std_logic_vector(7 downto 0);
	signal loadval : integer;
	signal txda		: 	STD_LOGIC_VECTOR(19 DOWNTO 0);
	signal txclka, txclka_RIO, TXRIOA_REFCLK, TXRIOA_USRCLK  : std_logic;
   signal tx_clk1_out_113a, tx_clk2_out_113a,tx_clk1_out_113b, tx_clk2_out_113b: std_logic;
	signal txdaOs_in, txdaOs_out: std_logic_vector(19 downto 0);
   signal enable_hd :std_logic;


	constant f8g_count1           : integer := 3375;        -- used to genlock to f8g sync
	constant f8g_count2           : integer := 3520;        -- 3375*3520 = 11880000          
	constant f8g_bits1            : integer := 12;          -- size of counter 1            
	constant f8g_bits2            : integer := 12;          -- size of counter 2            

	constant f4m_count1           : integer := 2750;        -- used to genlock to f4m sync  
	constant f4m_count2           : integer := 1800;        -- 2750*1800 = 4950000
	constant f4m_bits1            : integer := 12;          -- size of counter 1            
	constant f4m_bits2            : integer := 11;          -- size of counter 2 
	constant debug : std_logic := '0';	-- high for debug, low for compiling
BEGIN

-- Use a clock buffer to avoid clock-skew (IMPORTANT!)

--clockbuf1: BUFG
--      port map(I => f7425, O => txclka);
--clockbuf2: BUFG
--      port map(I => f7417, O => TXRIOA_REFCLK);
--clockbuf3: BUFG
--      port map(I => TX_CLK1_OUT_113a, O => TXRIOA_USRCLK);
clockbuf4: BUFG
      port map(I => f1485, O => txclka);
clockbuf5: BUFG
		port map(I => f1484, O => TXRIOA_REFCLK);
clockbuf6: BUFG
		port map(I => TX_CLK1_OUT_113a, O => TXRIOA_USRCLK);

mgt113a : MY_GT11
	port map(
 		RX1N_IN 		   => '0',
   	RX1P_IN 		   => '0',
		TX1N_OUT 	   => TXn_OUT_113a,
		TX1P_OUT 	   => TXp_OUT_113a,
		RXRECCLK1_OUT  => open,
		TXOUTCLK1_OUT  => TX_CLK1_OUT_113a,
		TXOUTCLK2_OUT  => TX_CLK2_OUT_113a,
		RXUSRCLK2_IN   => TXRIOA_USRCLK, 
		TXUSRCLK_IN    => TXRIOA_USRCLK,
		TXUSRCLK2_IN   => TXRIOA_USRCLK,
		GREFCLK_IN 	   => TXRIOA_REFCLK,
	   RXDATA_OUT	   => open,
		TXDATA_IN      => txda);

mgt113b : MY_GT11
	port map(
 		RX1N_IN 		   => '0',
   	RX1P_IN 		   => '0',
		TX1N_OUT 	   => TXn_OUT_113b,
		TX1P_OUT 	   => TXp_OUT_113b,
		RXRECCLK1_OUT  => open,
		TXOUTCLK1_OUT  => TX_CLK1_OUT_113b,
		TXOUTCLK2_OUT  => TX_CLK2_OUT_113b,
		RXUSRCLK2_IN   => TXRIOA_USRCLK, 
		TXUSRCLK_IN    => TXRIOA_USRCLK,
		TXUSRCLK2_IN   => TXRIOA_USRCLK,
		GREFCLK_IN 	   => TXRIOA_REFCLK,
	   RXDATA_OUT	   => open,
		TXDATA_IN      => txda);

mod0: serial_interface Port map(
      mreset => reset_n,
      cs => cs,
      sck	=> SCK,
      mosi => MOSI,
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

--U9A:	hd_frame
mod5 :	hd_frame
	PORT MAP(
		clk => txclka, 
		enable   => enable_hd,
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
		chroma => Cdataa,
    	tp => tp4
		);
	
mod6 : scram20_top
	PORT MAP(
        -- scrambler tx I/O
        txclk => txclka,             -- TX clock
		  enable_sync => enable_hd,
        reset => not sif_sd_hda,      -- active high reset
        tx_oe => '1',                 -- output enable
        bypass => '0',                -- '1'= bypass. raw data mode, bypass scrambler
        txdata_in =>  tx292data_ina,  -- raw data in from Co-Processor
        txdata_out => tx292data_outa  -- parallel scrambled output to Hotlink II PHY
	);     

mod7 : video_sm port map(
        clk         => txclka,
		  enable      => enable_hd,
        video_en    => hd_mute,
	     sd_hd		  => '0',				--sd_hda,
        hdformat    => hdformat,
        scan_format => hd_scanformat,
        pattern     => pattern(2 downto 0),
		  option		  => option(2 downto 0),
        sav         => sav,
        line_clk    => line_clk,
        fvh         => fvh,
        y           => act_y,
        cb          => act_cb,
        cr          => act_cr,
		  debug	     => debug,
		  loadval	  => loadval,
		  tp			  => tp
        );

mod8:  os_controller port map(
	    clk_148  	  => txclka,
	    reset		  => reset_n,
	    hdsd_in	     => '1',	         -- 1 = hd; 0 = sd;
	    data_in  	  => txdaOs_in,	
	    data_out 	  => txdaOs_out,
	    enable_out   => enable_hd
	);


assign_txda: process(TXRIOA_USRCLK)
begin
 if TXRIOA_USRCLK'event and TXRIOA_USRCLK = '1' then
      txda <= txdaOs_out;
end if;
end process;

make_OsData: process(tx292data_outa)
begin
		txdaOs_in(19)<=  tx292data_outa(19);
		txdaOs_in(18)<=  tx292data_outa(18);
		txdaOs_in(17)<=  tx292data_outa(17);
	   txdaOs_in(16)<=  tx292data_outa(16);
		txdaOs_in(15)<=  tx292data_outa(15);
		txdaOs_in(14)<=  tx292data_outa(14);
		txdaOs_in(13)<=  tx292data_outa(13);
	   txdaOs_in(12)<=  tx292data_outa(12);		
		txdaOs_in(11)<=  tx292data_outa(11);
		txdaOs_in(10)<=  tx292data_outa(10);
		txdaOs_in(9) <=  tx292data_outa(9);
	   txdaOs_in(8) <=  tx292data_outa(8);
		txdaOs_in(7) <=  tx292data_outa(7);
	   txdaOs_in(6) <=  tx292data_outa(6);
		txdaOs_in(5) <=  tx292data_outa(5);
		txdaOs_in(4) <=  tx292data_outa(4);
		txdaOs_in(3) <=  tx292data_outa(3);
	   txdaOs_in(2) <=  tx292data_outa(2);		
		txdaOs_in(1) <=  tx292data_outa(1);
		txdaOs_in(0) <=  tx292data_outa(0);
end process;


make_tx292data: process (txclka)
begin
	if  txclka'event and txclka = '1' then
		if enable_hd = '1' then
			tx292data_ina <= reverse_bit_order(Cdataa,10) & reverse_bit_order(Ydataa,10);	 --originally Cdataa first and Ydataa second
		end if;
	end if;
end process;

hd_mute <= sif_rst; 
hdformat <= sif_hd_format(3 downto 0);	-- 1080i/30
--hdformat <= "0110";	-- 720p/60
pattern <= "00000" & sif_pattern(2 downto 0);
option <= "00000" & sif_option(2 downto 0);

sif_reset : process(txclka)
begin
	if txclka'event and txclka = '1' then
      if enable_hd = '1' then
			sif_rst <= cs;
  		end if;
	end if;
end process;

--make_txclka2: PROCESS(txclka)
--BEGIN
--	if txclka'event and txclka = '1' then
--		txclka2 <= not txclka2;
--	end if;
--END PROCESS;
--
--
  TP1  <= txdaOs_out(18);
  TP2  <= TX_CLK1_OUT_113a;
  TP3  <= TX_CLK2_OUT_113a;			 
--  TP4  <=  ;				 --tp4 bruges i portmap, test point in hd_framegenerator
  Debug9(0) <= sav;
  Debug9(1) <= txclka;
  Debug9(2) <= txdaOs_in(9);
  Debug9(3) <= txdaOs_in(19);
  Debug9(4) <= txdaOs_out(19);
END a;