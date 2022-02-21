library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--library UNISIM;
--use UNISIM.VComponents.all;

entity sdi_generator is 
    port(
        -- signals to/from PT5300 :
        mreset :        in std_logic;   -- master reset
        f27 :           in std_logic;   -- 27 MHz clock
        f4m :           in std_logic;   -- genlocking puls from PT5300 - 625 lines compliant formats
        f8g :           in std_logic;   -- genlocking puls from PT5300 - 525 lines compliant formats
        ok1 :           out std_logic;  -- level check respons to PT5300
        ok2 :           out std_logic;  -- level check respons to PT5300
        -- clock signals from ICS clock synthesizers :
        f7425 :         in std_logic;   -- 74.25 MHz clock from ICS clock synth
        f1485 :         in std_logic;   -- 148.5 MHz clock from ICS clock synth
        f7417 :         in std_logic;   -- 74.25/1.001 MHz clock from ICS clock synth
        f1484 :         in std_logic;   -- 148.5/1.001 MHz clock from ICS clock synth
        -- signals to/from Cygnal uProcessor :
        cs3 :           in std_logic;   -- chip select from uProcessor
        cs2 :           in std_logic;   -- chip select from uProcessor
        cs1 :           in std_logic;   -- chip select from uProcessor
        cs0 :           in std_logic;   -- chip select from uProcessor
        sck :           in std_logic;   -- serial clock from uProcessor
        mosi :          in std_logic    -- master out serial data in from uProcessor
        );
end sdi_generator;

architecture Behavioral of sdi_generator is

    component f4m_genlock_regen
        port(
            mreset    : in std_logic;   -- master reset
		  clk       : in std_logic;   
		  f4m       : in std_logic;   -- 4 field sync signal from mainframe
		  f2m       : out std_logic;  -- Genlock sync every 2nd field
	       );
    end component;


    component serial_interface
        port(
            mreset :        in std_logic;                       -- master reset
            -- serial communication from Cygnal processor :
            cs :            in std_logic;                       -- chip select
            sck :           in std_logic;                       -- serial clock
            mosi :          in std_logic;                       -- master out serial data in
            -- video registers :
            offset :    	out std_logic_vector(23 downto 0);  -- delay from sync reference
		    sd_hd :			out std_logic;						-- LOW = SD, HIGH = HD
		    sd_format :		out std_logic;						-- LOW = 625 line system, HIGH = 525 line system
		    scan_format :	out std_logic;						-- LOW = progressive, HIGH = interlaced
		    hd_clk :		out std_logic;						-- LOW = 74,25 MHz, HIGH = 74,25/1.001 MHz
		    hd_format :		out std_logic_vector(3 downto 0);	-- high definition format
		    pattern :		out std_logic_vector(7 downto 0);	-- test pattern select
		    tp_option :		out std_logic_vector(7 downto 0);	-- test pattern option
            -- misc :
            new_delay :     out std_logic;                      -- new offset indicating pulse
            endof_frame :   in std_logic;                       -- read new data pulse
            -- debug :
	        debug :         in std_logic;
		    tp :			out std_logic
            );
    end component;

	component sd_frame
		port(
			reset_n :   in std_logic;                           -- active low reset
            clk :       in std_logic;                           -- transmit clock
            -- video registers :
            pal_nntsc : in std_logic;			                -- pal/ntsc select
            -- parallel video out :
			video_out : out std_logic_vector(9 downto 0)        -- interleaved parallel video output
			);
	end component;

	component hd_frame
		port ( 
			clk :       	in std_logic;                       -- sample clock
        	mute :      	in std_logic;                       -- active low
            -- video registers :
        	hdformat :  	in std_logic_vector(3 downto 0);    -- SMPTE 240M, 274M or 296M
            -- active video from video_sm :
  	    	act_y :     	in std_logic_vector(9 downto 0);
        	act_cb :    	in std_logic_vector(9 downto 0);
       		act_cr :    	in std_logic_vector(9 downto 0);
            -- outputs for reading new format data :
            endof_frame :   out std_logic;
            -- outputs for video_sm :
			sav :           out std_logic;                      -- start of active video
			fvh :           out std_logic_vector(2 downto 0);   -- Field, Vert-blank and Horz-blank bits
        	line_clk :      inout std_logic;                    -- total line clock
            -- parallel video out :
			luma :      	out std_logic_vector(9 downto 0);
			chroma :    	out std_logic_vector(9 downto 0)
			);
	end component;

	component video_sm is
    	port ( 
        	clk :           in std_logic;                       -- sample clock
	    	video_en :      in std_logic;                       -- active high enable
            -- video registers :
			sd_hd :			in std_logic;
         	hdformat :      in std_logic_vector(3 downto 0);                       
	        scan_format :   in std_logic;
    	    pattern :       in std_logic_vector(2 downto 0);
        	option :        in std_logic_vector(2 downto 0);
            -- timing signals from hd_framegenerator
			sav :           in std_logic;                       -- start of active video
	        line_clk :      in std_logic;                       -- total line clock
    	    fvh :           in std_logic_vector(2 downto 0);    -- Field, Vert-blank and Horz-blank bits
            -- active video output
        	y :             out std_logic_vector(9 downto 0);
	        cb :            out std_logic_vector(9 downto 0);
    	    cr :            out std_logic_vector(9 downto 0);
        	-- debug
	        debug :         in std_logic;                       
    	    loadval :       out integer;
  			tp :			out std_logic_vector(23 downto 0)
	        );
	end component;

signal sdclk :          std_logic;                      -- 27 MHz clock signal
signal hdclk :          std_logic;                      -- 74.25 MHz or 74.25/1.001 MHz clock signal
signal hdluma :         std_logic_vector(9 downto 0);   -- video signal
signal hdchroma :       std_logic_vector(9 downto 0);   -- video signal
signal offset :         std_logic_vector(23 downto 0);  -- delay from sync reference
signal new_delay :      std_logic;                      -- new offset signal
signal endof_frame :    std_logic;                      -- read format data
signal sd_hd :          std_logic;                      -- 0=SD , 1=HD
signal sd_format :      std_logic;                      -- 525 or 625 line system
signal scan_format :    std_logic;                      -- interlaced or progressive
signal hd_clk :         std_logic;                      -- 74.25 or 74.17 MHz
signal hd_format :      std_logic_vector(3 downto 0);   -- format select register
signal pattern :        std_logic_vector(7 downto 0);   -- test pattern select register
signal tp_option :      std_logic_vector(7 downto 0);   -- test pattern option select register
signal sif_tp :         std_logic;                      -- test point from serial interface module
signal vsm_tp :         std_logic_vector(23 downto 0);  -- test points from video state machine module
signal debug :          std_logic;                      -- high for simulation
signal loadval :        integer;                        -- test point from video state machine module
signal vsm_clk :        std_logic;                      -- clock signal for video state machine
signal line_clk :       std_logic;                      -- line clock
signal reset_n :        std_logic;                      -- reset for sdsdi generator
signal hd_reset :       std_logic;                      -- reset for hdsdi generator
signal video_en :       std_logic;                      -- enable for video state machine modul
signal act_y :          std_logic_vector(9 downto 0);   -- active luma data
signal act_cb :         std_logic_vector(9 downto 0);   -- active chroma data
signal act_cr :         std_logic_vector(9 downto 0);   -- active chroma data
signal sav :            std_logic;                      -- start of active video
signal fvh :            std_logic_vector( 2 downto 0);  -- field, vertical blank & horizontal blank bits

begin
    
    f2m_genlock : f4m_genlock_regen port map(
        mreset => mreset,
	   f7425	=> clk,
        f4m    => f4m,
	   f2m    => f2m	   
	   );

    sif : serial_interface port map(
        mreset => mreset,
        cs => cs0,
        sck => sck,
        mosi => mosi,
        offset => offset,
        new_delay => new_delay,
		sd_hd => sd_hd,
		sd_format => sd_format,
		scan_format => scan_format,
		hd_clk => hd_clk,
		hd_format => hd_format,
		pattern => pattern,
		tp_option => tp_option,
        endof_frame => endof_frame,
        debug => debug,
		tp => sif_tp
        );

    sd : sd_frame port map(
        reset_n => reset_n, 
        clk => sdclk, 
        pal_nntsc => sd_format,
        video_out => open
        );
		
    hd : hd_frame	port map(
		clk => hdclk, 
		mute => hd_reset,
		hdformat => hd_format,
		act_y => act_y,
	    act_cb => act_cb,
        act_cr => act_cr,
        endof_frame => endof_frame,
		sav	=> sav,
		fvh => fvh,
        line_clk => line_clk,
		luma => hdluma,
		chroma => hdchroma
		);
	
    video : video_sm port map(
        clk         => vsm_clk,
        video_en    => video_en,
		sd_hd		=> sd_hd,
        hdformat    => hd_format,
        scan_format => scan_format,
        pattern     => pattern(2 downto 0),
		option		=> tp_option(2 downto 0),
        sav         => sav,
        line_clk    => line_clk,
        fvh         => fvh,
        y           => act_y,
        cb          => act_cb,
        cr          => act_cr,
		debug		=> debug,
		loadval		=> loadval,
		tp			=> vsm_tp
        );

	genlock_regen port map(
        clk         => vsm_clk,					---------------------------
        video_en    => video_en,
		sd_hd		=> sd_hd,
        hdformat    => hd_format,
        scan_format => scan_format,
        pattern     => pattern(2 downto 0),
		option		=> tp_option(2 downto 0),
        sav         => sav,
        line_clk    => line_clk,
        fvh         => fvh,
		loadval		=> loadval,
		tp			=> vsm_tp
        );

assign_videostatemachine_clock : vsm_clk <= hdclk when sd_hd = '1' else sdclk;

debug <= '0';   -- high for simulation

end Behavioral;
