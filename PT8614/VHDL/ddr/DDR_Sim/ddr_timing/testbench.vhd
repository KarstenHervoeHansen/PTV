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
--COMPONENT HD_Gen_Module is
--   Port (
--      mreset_i       : in std_logic;      -- Master Reset fra mainboard

--      brefclk_p_i    : in std_logic;
--      brefclk_n_i    : in std_logic;

--      led_o          : out std_logic_vector(3 downto 0);

--      ddr2_casb      : out std_logic;
--      ddr2_cke       : out std_logic;
--      ddr2_clk0      : out std_logic;
--      ddr2_clk0b     : out std_logic;
--      ddr2_csb       : out std_logic;
--      ddr2_rasb      : out std_logic;
--      ddr2_web       : out std_logic;
--      ddr2_ODT0      : out std_logic;
--      ddr2_address   : out std_logic_VECTOR(12 downto 0);
--      ddr2_ba        : out std_logic_VECTOR(1 downto 0);
--      ddr2_dm        : out std_logic_VECTOR(1 downto 0);
--      ddr2_dqs       : inout std_logic_VECTOR(1 downto 0);
--      ddr2_dqs_n     : inout std_logic_VECTOR(1 downto 0);
--      ddr2_dq        : inout std_logic_VECTOR(15 downto 0)
--   );
--END COMPONENT;


component HD_Gen_Module is
  port (
    mreset_i : in STD_LOGIC := 'X'; 
    brefclk2_n_i : in STD_LOGIC := 'X';     
    ddr_rst_dqs_div_i : in STD_LOGIC := 'X';     
    ddr_rst_dqs_div_o : out STD_LOGIC; 
    clk_27_i : in STD_LOGIC := 'X'; 
    brefclk2_p_i : in STD_LOGIC := 'X'; 
    brefclk_n_i : in STD_LOGIC := 'X'; 
    brefclk_p_i : in STD_LOGIC := 'X'; 
    pc_ibf_i : in STD_LOGIC := 'X'; 
    pc_zstb_o : out STD_LOGIC; 
    ddr2_cke : out STD_LOGIC; 
    ddr2_rasb : out STD_LOGIC; 
    ddr2_csb : out STD_LOGIC; 
    vcxo_control_1_o : out STD_LOGIC; 
    ddr2_casb : out STD_LOGIC; 
    vcxo_control_2_o : out STD_LOGIC; 
    ddr2_web : out STD_LOGIC; 
    ddr2_ODT0 : out STD_LOGIC; 
    ddr2_clk0 : out STD_LOGIC; 
    ddr2_clk0b : out STD_LOGIC; 
    ddr2_dqs_n : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
    ddr2_dqs : inout STD_LOGIC_VECTOR ( 1 downto 0 ); 
    ddr2_dq : inout STD_LOGIC_VECTOR ( 15 downto 0 ); 
    led_o : out STD_LOGIC_VECTOR ( 3 downto 0 ); 
    pc_data_o : out STD_LOGIC_VECTOR ( 21 downto 0 ); 
    ddr2_ba : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
    ddr2_dm : out STD_LOGIC_VECTOR ( 1 downto 0 ); 
    ddr2_address : out STD_LOGIC_VECTOR ( 12 downto 0 ) 
  );
end component;

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
   signal test_port     :  std_logic_vector(21 downto 0);
   signal rst_dqs       :  std_logic;
   
BEGIN

  -- Component Instantiation
 uut: HD_Gen_Module port map
( brefclk_p_i     => clkin_P,
  brefclk_n_i     => clkin_N,
  brefclk2_p_i    => clkin_P,
  brefclk2_n_i    => clkin_N,
  clk_27_i        => clkin_P,
  ddr_rst_dqs_div_i => rst_dqs,
  ddr_rst_dqs_div_o => rst_dqs, 
  pc_ibf_i        => '1',
  pc_data_o       => test_port,
  mreset_i        => RST,
  led_o           => leds,
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
  
  -- RST must be asserted for a minimum of 4 clocks or DCM won't LOCK!
  RST     <= '1' after 50 ns;
END;
