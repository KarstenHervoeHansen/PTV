-------------------------------------------------------------------------------
-- dcm_0_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library dcm_module_v1_00_a;
use dcm_module_v1_00_a.All;

entity dcm_0_wrapper is
  port (
    RST : in std_logic;
    CLKIN : in std_logic;
    CLKFB : in std_logic;
    PSEN : in std_logic;
    PSINCDEC : in std_logic;
    PSCLK : in std_logic;
    DSSEN : in std_logic;
    CLK0 : out std_logic;
    CLK90 : out std_logic;
    CLK180 : out std_logic;
    CLK270 : out std_logic;
    CLKDV : out std_logic;
    CLK2X : out std_logic;
    CLK2X180 : out std_logic;
    CLKFX : out std_logic;
    CLKFX180 : out std_logic;
    STATUS : out std_logic_vector(7 downto 0);
    LOCKED : out std_logic;
    PSDONE : out std_logic
  );
  attribute x_core_info : STRING;
  attribute x_core_info of dcm_0_wrapper: entity is "dcm_module_v1_00_a";

end dcm_0_wrapper;

architecture STRUCTURE of dcm_0_wrapper is

  component dcm_module is
    generic (
      C_DFS_FREQUENCY_MODE : STRING;
      C_DLL_FREQUENCY_MODE : STRING;
      C_DUTY_CYCLE_CORRECTION : BOOLEAN;
      C_CLKIN_DIVIDE_BY_2 : BOOLEAN;
      C_CLK_FEEDBACK : STRING;
      C_CLKOUT_PHASE_SHIFT : STRING;
      C_DSS_MODE : STRING;
      C_STARTUP_WAIT : BOOLEAN;
      C_PHASE_SHIFT : INTEGER;
      C_CLKFX_MULTIPLY : INTEGER;
      C_CLKFX_DIVIDE : INTEGER;
      C_CLKDV_DIVIDE : REAL;
      C_CLKIN_PERIOD : REAL;
      C_DESKEW_ADJUST : STRING;
      C_CLKIN_BUF : BOOLEAN;
      C_CLKFB_BUF : BOOLEAN;
      C_CLK0_BUF : BOOLEAN;
      C_CLK90_BUF : BOOLEAN;
      C_CLK180_BUF : BOOLEAN;
      C_CLK270_BUF : BOOLEAN;
      C_CLKDV_BUF : BOOLEAN;
      C_CLK2X_BUF : BOOLEAN;
      C_CLK2X180_BUF : BOOLEAN;
      C_CLKFX_BUF : BOOLEAN;
      C_CLKFX180_BUF : BOOLEAN;
      C_EXT_RESET_HIGH : integer;
      C_FAMILY : string
    );
    port (
      RST : in std_logic;
      CLKIN : in std_logic;
      CLKFB : in std_logic;
      PSEN : in std_logic;
      PSINCDEC : in std_logic;
      PSCLK : in std_logic;
      DSSEN : in std_logic;
      CLK0 : out std_logic;
      CLK90 : out std_logic;
      CLK180 : out std_logic;
      CLK270 : out std_logic;
      CLKDV : out std_logic;
      CLK2X : out std_logic;
      CLK2X180 : out std_logic;
      CLKFX : out std_logic;
      CLKFX180 : out std_logic;
      STATUS : out std_logic_vector(7 downto 0);
      LOCKED : out std_logic;
      PSDONE : out std_logic
    );
  end component;

begin

  dcm_0 : dcm_module
    generic map (
      C_DFS_FREQUENCY_MODE => "LOW",
      C_DLL_FREQUENCY_MODE => "LOW",
      C_DUTY_CYCLE_CORRECTION => TRUE,
      C_CLKIN_DIVIDE_BY_2 => FALSE,
      C_CLK_FEEDBACK => "1X",
      C_CLKOUT_PHASE_SHIFT => "NONE",
      C_DSS_MODE => "NONE",
      C_STARTUP_WAIT => FALSE,
      C_PHASE_SHIFT => 0,
      C_CLKFX_MULTIPLY => 4,
      C_CLKFX_DIVIDE => 1,
      C_CLKDV_DIVIDE => 2.000000,
      C_CLKIN_PERIOD => 6.734007,
      C_DESKEW_ADJUST => "SYSTEM_SYNCHRONOUS",
      C_CLKIN_BUF => FALSE,
      C_CLKFB_BUF => FALSE,
      C_CLK0_BUF => TRUE,
      C_CLK90_BUF => FALSE,
      C_CLK180_BUF => FALSE,
      C_CLK270_BUF => FALSE,
      C_CLKDV_BUF => TRUE,
      C_CLK2X_BUF => FALSE,
      C_CLK2X180_BUF => FALSE,
      C_CLKFX_BUF => FALSE,
      C_CLKFX180_BUF => FALSE,
      C_EXT_RESET_HIGH => 1,
      C_FAMILY => "virtex2p"
    )
    port map (
      RST => RST,
      CLKIN => CLKIN,
      CLKFB => CLKFB,
      PSEN => PSEN,
      PSINCDEC => PSINCDEC,
      PSCLK => PSCLK,
      DSSEN => DSSEN,
      CLK0 => CLK0,
      CLK90 => CLK90,
      CLK180 => CLK180,
      CLK270 => CLK270,
      CLKDV => CLKDV,
      CLK2X => CLK2X,
      CLK2X180 => CLK2X180,
      CLKFX => CLKFX,
      CLKFX180 => CLKFX180,
      STATUS => STATUS,
      LOCKED => LOCKED,
      PSDONE => PSDONE
    );

end architecture STRUCTURE;

