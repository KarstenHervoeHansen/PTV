-------------------------------------------------------------------------------
-- docm_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library dsocm_v10_v2_00_a;
use dsocm_v10_v2_00_a.All;

entity docm_wrapper is
  port (
    DSOCM_Clk : in std_logic;
    SYS_Rst : in std_logic;
    DSOCM_Rst : out std_logic;
    M_DSOCMBRAMABUS : in std_logic_vector(8 to 29);
    M_DSOCMBRAMBYTEWRITE : in std_logic_vector(0 to 3);
    M_DSOCMBRAMEN : in std_logic;
    M_DSOCMBRAMWRDBUS : in std_logic_vector(0 to 31);
    M_DSOCMBUSY : in std_logic;
    M_DSOCMRDADDRVALID : in std_logic;
    M_DSOCMWRADDRVALID : in std_logic;
    S_BRAMDSOCMRDDBUS : in std_logic_vector(0 to 31);
    S_DSOCMRWCOMPLETE : in std_logic_vector(0 to 0);
    S_DSOCMSLAVESELECT : in std_logic_vector(0 to 0);
    BRAMDSOCMRDDBUS : out std_logic_vector(0 to 31);
    DSARCVALUE : out std_logic_vector(0 to 7);
    DSCNTLVALUE : out std_logic_vector(0 to 7);
    DSOCMBRAMWRDBUS : out std_logic_vector(0 to 31);
    DSOCMBRAMEN : out std_logic;
    DSOCMBRAMBYTEWRITE : out std_logic_vector(0 to 3);
    DSOCMBRAMABUS : out std_logic_vector(8 to 29);
    DSOCMBUSY : out std_logic;
    DSOCMRDADDRVALID : out std_logic;
    DSOCMWRADDRVALID : out std_logic;
    DSOCMRWCOMPLETE : out std_logic
  );
  attribute x_core_info : STRING;
  attribute x_core_info of docm_wrapper: entity is "dsocm_v10_v2_00_a";

end docm_wrapper;

architecture STRUCTURE of docm_wrapper is

  component dsocm_v10 is
    generic (
      C_NUM_MASTERS : integer;
      C_NUM_SLAVES : integer;
      C_DSCNTLVALUE : std_logic_vector(0 to 7);
      C_DSARCVALUE : std_logic_vector(0 to 7);
      C_FIXED_LATENCY : std_logic
    );
    port (
      DSOCM_Clk : in std_logic;
      SYS_Rst : in std_logic;
      DSOCM_Rst : out std_logic;
      M_DSOCMBRAMABUS : in std_logic_vector(8 to 29);
      M_DSOCMBRAMBYTEWRITE : in std_logic_vector(0 to 3);
      M_DSOCMBRAMEN : in std_logic;
      M_DSOCMBRAMWRDBUS : in std_logic_vector(0 to 31);
      M_DSOCMBUSY : in std_logic;
      M_DSOCMRDADDRVALID : in std_logic;
      M_DSOCMWRADDRVALID : in std_logic;
      S_BRAMDSOCMRDDBUS : in std_logic_vector(0 to 32*C_NUM_SLAVES-1);
      S_DSOCMRWCOMPLETE : in std_logic_vector(0 to C_NUM_SLAVES-1);
      S_DSOCMSLAVESELECT : in std_logic_vector(0 to C_NUM_SLAVES-1);
      BRAMDSOCMRDDBUS : out std_logic_vector(0 to 31);
      DSARCVALUE : out std_logic_vector(0 to 7);
      DSCNTLVALUE : out std_logic_vector(0 to 7);
      DSOCMBRAMWRDBUS : out std_logic_vector(0 to 31);
      DSOCMBRAMEN : out std_logic;
      DSOCMBRAMBYTEWRITE : out std_logic_vector(0 to 3);
      DSOCMBRAMABUS : out std_logic_vector(8 to 29);
      DSOCMBUSY : out std_logic;
      DSOCMRDADDRVALID : out std_logic;
      DSOCMWRADDRVALID : out std_logic;
      DSOCMRWCOMPLETE : out std_logic
    );
  end component;

begin

  docm : dsocm_v10
    generic map (
      C_NUM_MASTERS => 1,
      C_NUM_SLAVES => 1,
      C_DSCNTLVALUE => X"83",
      C_DSARCVALUE => X"21",
      C_FIXED_LATENCY => '1'
    )
    port map (
      DSOCM_Clk => DSOCM_Clk,
      SYS_Rst => SYS_Rst,
      DSOCM_Rst => DSOCM_Rst,
      M_DSOCMBRAMABUS => M_DSOCMBRAMABUS,
      M_DSOCMBRAMBYTEWRITE => M_DSOCMBRAMBYTEWRITE,
      M_DSOCMBRAMEN => M_DSOCMBRAMEN,
      M_DSOCMBRAMWRDBUS => M_DSOCMBRAMWRDBUS,
      M_DSOCMBUSY => M_DSOCMBUSY,
      M_DSOCMRDADDRVALID => M_DSOCMRDADDRVALID,
      M_DSOCMWRADDRVALID => M_DSOCMWRADDRVALID,
      S_BRAMDSOCMRDDBUS => S_BRAMDSOCMRDDBUS,
      S_DSOCMRWCOMPLETE => S_DSOCMRWCOMPLETE,
      S_DSOCMSLAVESELECT => S_DSOCMSLAVESELECT,
      BRAMDSOCMRDDBUS => BRAMDSOCMRDDBUS,
      DSARCVALUE => DSARCVALUE,
      DSCNTLVALUE => DSCNTLVALUE,
      DSOCMBRAMWRDBUS => DSOCMBRAMWRDBUS,
      DSOCMBRAMEN => DSOCMBRAMEN,
      DSOCMBRAMBYTEWRITE => DSOCMBRAMBYTEWRITE,
      DSOCMBRAMABUS => DSOCMBRAMABUS,
      DSOCMBUSY => DSOCMBUSY,
      DSOCMRDADDRVALID => DSOCMRDADDRVALID,
      DSOCMWRADDRVALID => DSOCMWRADDRVALID,
      DSOCMRWCOMPLETE => DSOCMRWCOMPLETE
    );

end architecture STRUCTURE;

