--File:  testbench.vhd 
-------------------------------------------------------------------------------
--**************************************************************************
-- 
--     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
--     SOLELY FOR USE IN DEVELOPING PROGRAMS AND SOLUTIONS FOR
--     XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION
--     AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, APPLICATION
--     OR STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS
--     IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
--     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
--     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
--     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
--     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
--     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
--     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
--     FOR A PARTICULAR PURPOSE.
--
--     (c) Copyright 2003 Xilinx, Inc.
--     All rights reserved.
--
--**************************************************************************
-------------------------------------------------------------------------------
-- Revision 1.0
--
--     Modification History:                                                   
--     Date     Init          Description                                
--   --------  ------  --------------------------------------------------------
--   10/29/2003   ST     Initial release.
-------------------------------------------------------------------------------

LIBRARY ieee;
  USE ieee.std_logic_1164.ALL;
  USE ieee.numeric_std.ALL;
  
library UNISIM;
use UNISIM.VComponents.all;

ENTITY test IS
END test;

ARCHITECTURE behavior OF test IS 

-- Component Declarations:
COMPONENT HD_Gen_Module is
   Port (
      mreset_i       : in std_logic;      -- Master Reset fra mainboard

      brefclk_p_i    : in std_logic;
      brefclk_n_i    : in std_logic;
      fvh_i		 			    : in std_logic_vector(2 downto 0);
      enable_i         : in std_logic;
      sync_reset_i   : in std_logic;
      led_o          : out std_logic_vector(3 downto 0);
		luma_a_o				: out std_logic_vector(9 downto 0);
		chroma_a_o				: out std_logic_vector(9 downto 0);
      ddr2_casb      : out std_logic;
      ddr2_cke       : out std_logic;
      ddr2_clk0      : out std_logic;
      ddr2_clk0b     : out std_logic;
      ddr2_csb       : out std_logic;
      ddr2_rasb      : out std_logic;
      ddr2_web       : out std_logic;
      ddr2_ODT0      : out std_logic;
      ddr2_address   : out std_logic_VECTOR(12 downto 0);
      ddr2_ba        : out std_logic_VECTOR(1 downto 0);
      ddr2_dm        : out std_logic_VECTOR(1 downto 0);
      ddr2_dqs       : inout std_logic_VECTOR(1 downto 0);
      ddr2_dqs_n     : inout std_logic_VECTOR(1 downto 0);
      ddr2_dq        : inout std_logic_VECTOR(15 downto 0)
   );
END COMPONENT;

component ddr2 
port(
    ck      :  in STD_LOGIC;
    ck_n    :  in STD_LOGIC;
    cke     :  in STD_LOGIC;
    cs_n    :  in STD_LOGIC;
    ras_n   :  in STD_LOGIC;
    cas_n   :  in STD_LOGIC;
    we_n    :  in STD_LOGIC;
    dm_rdqs :  inout STD_LOGIC_VECTOR(1 downto 0);
    ba      :  in STD_LOGIC_VECTOR(1 downto 0);
    addr    :  in STD_LOGIC_VECTOR(12 downto 0);
    dq      :  inout STD_LOGIC_VECTOR(15 downto 0);
    dqs     :  inout STD_LOGIC_VECTOR(1 downto 0);
    dqs_n   :  inout STD_LOGIC_VECTOR(1 downto 0);
    rdqs_n  :  out STD_LOGIC;
    odt     :  in STD_LOGIC
);
end component;


-- Signal Declarations:
   signal output_en     :  std_logic := '1';
   signal clkin_P       :  std_logic := '1';
   signal clkin_N       :  std_logic := '0';
   signal RST           :  std_logic := '0'; 
   signal leds          :  std_logic_VECTOR(3 downto 0);        
   signal ddr2_casb     :  std_logic;                     
   signal ddr2_cke      :  std_logic;                     
   signal ddr2_clk0     :  std_logic;                     
   signal ddr2_clk0b    :  std_logic;                               
   signal ddr2_csb      :  std_logic;                     
   signal ddr2_rasb     :  std_logic;                     
   signal ddr2_web      :  std_logic;                     
   signal ddr2_ODT0     :  std_logic;                     
   signal ddr2_address  :  std_logic_VECTOR(12 downto 0); 
   signal ddr2_ba       :  std_logic_VECTOR(1 downto 0);  
   signal ddr2_dm       :  std_logic_VECTOR(1 downto 0);  
   signal ddr2_dqs      :  std_logic_VECTOR(1 downto 0);
   signal ddr2_dqs_n    :  std_logic_VECTOR(1 downto 0);
   signal ddr2_dq       :  std_logic_VECTOR(15 downto 0);
   signal en            :  std_logic;
   signal luma_a        :  std_logic_vector(9 downto 0);
   signal chroma_a      :  std_logic_vector(9 downto 0);
   signal fvh           :  std_logic_vector(2 downto 0);
   signal sync_reset,zreset    :  std_logic;
   
BEGIN

  -- Component Instantiation
 uut: HD_Gen_Module port map
( brefclk_p_i     => clkin_P,
  brefclk_n_i     => clkin_N,
  mreset_i        => RST,
  led_o           => leds,		
  enable_i        => en,
  fvh_i           => fvh,
  sync_reset_i    => sync_reset,
  chroma_a_o      => chroma_a,
  luma_a_o        => luma_a,   
  ddr2_casb       => ddr2_casb   ,
  ddr2_cke        => ddr2_cke    ,
  ddr2_clk0       => ddr2_clk0   ,
  ddr2_clk0b      => ddr2_clk0b  ,
  ddr2_csb        => ddr2_csb    ,
  ddr2_rasb       => ddr2_rasb   ,
  ddr2_web        => ddr2_web    ,
  ddr2_ODT0       => ddr2_ODT0   ,
  ddr2_address    => ddr2_address,
  ddr2_ba         => ddr2_ba     ,
  ddr2_dm         => ddr2_dm     ,
  ddr2_dqs        => ddr2_dqs    ,
  ddr2_dqs_n      => ddr2_dqs_n  ,
  ddr2_dq         => ddr2_dq               
   );

ddr2_inst: ddr2
port map(         
    	ck      => ddr2_clk0,
    	ck_n    => ddr2_clk0b,
    	cke     => ddr2_cke,
    	cs_n    => ddr2_csb,
    	ras_n   => ddr2_rasb,
    	cas_n   => ddr2_casb,
    	we_n    => ddr2_web,
    	dm_rdqs => ddr2_dm,
    	ba      => ddr2_ba,
    	addr    => ddr2_address,
    	dq      => ddr2_dq,
    	dqs     => ddr2_dqs,
    	dqs_n   => ddr2_dqs_n,
    	rdqs_n  => open,
    	odt     => ddr2_odt0
);   

  clkin_P <= not clkin_P after 3.34 ns ;
  clkin_N <= not clkin_N after 3.34 ns ;
  
  en <= '1' after 0ns;--,'1' after 393.7us;--, '0' after 404us,'1' after 404.5us;
  --fvh <= "001"  after 0ns, "011" after 399.7us,"001" after 400.1us,"000" after 404.5us, "001" after 418.1us, "000" after 422.5us;  
  fvh <= "000" after 0ns, "011" after 399us, "010" after 401us, "001" after 403us, "000" after 405us, "001" after 408us, 
  "000" after 409us, "011" after 415us, "010" after 417us, "001" after 417.5us, "000" after 418us,"001" after 423us, "000" after 424us;
  -- RST must be asserted for a minimum of 4 clocks or DCM won't LOCK!
  RST     <= '1' after 300 ns;
  sync_reset  <= '0';-- after 400us, '1' after 400.1us, '0' after 400.15us;

END;
