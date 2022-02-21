-------------------------------------------------------------------------------
-- iocm_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library isocm_v10_v2_00_a;
use isocm_v10_v2_00_a.All;

entity iocm_wrapper is
  port (
    ISOCM_Clk : in std_logic;
    SYS_Rst : in std_logic;
    ISOCM_Rst : out std_logic;
    M_ISOCMBRAMRDABUS : in std_logic_vector(8 to 28);
    M_ISOCMBRAMWRABUS : in std_logic_vector(8 to 28);
    M_ISOCMBRAMEN : in std_logic;
    M_ISOCMBRAMWRDBUS : in std_logic_vector(0 to 31);
    M_ISOCMBRAMODDWRITEEN : in std_logic;
    M_ISOCMBRAMEVENWRITEEN : in std_logic;
    M_ISOCMDCRBRAMODDEN : in std_logic;
    M_ISOCMDCRBRAMEVENEN : in std_logic;
    M_ISOCMDCRBRAMRDSELECT : in std_logic;
    S_BRAMISOCMRDDBUS : in std_logic_vector(0 to 63);
    S_BRAMISOCMDCRRDDBUS : in std_logic_vector(0 to 31);
    S_ISOCMSLAVESELECT : in std_logic_vector(0 to 0);
    S_ISOCMDCRSLAVESELECT : in std_logic_vector(0 to 0);
    BRAMISOCMRDDBUS : out std_logic_vector(0 to 63);
    ISARCVALUE : out std_logic_vector(0 to 7);
    ISCNTLVALUE : out std_logic_vector(0 to 7);
    ISOCMBRAMEN : out std_logic;
    ISOCMBRAMWRDBUS : out std_logic_vector(0 to 31);
    ISOCMBRAMODDWRITEEN : out std_logic;
    ISOCMBRAMEVENWRITEEN : out std_logic;
    ISOCMBRAMRDABUS : out std_logic_vector(8 to 28);
    ISOCMBRAMWRABUS : out std_logic_vector(8 to 28);
    ISOCMDCRBRAMODDEN : out std_logic;
    ISOCMDCRBRAMEVENEN : out std_logic;
    ISOCMDCRBRAMRDSELECT : out std_logic;
    BRAMISOCMDCRRDDBUS : out std_logic_vector(0 to 31)
  );
  attribute x_core_info : STRING;
  attribute x_core_info of iocm_wrapper: entity is "isocm_v10_v2_00_a";

end iocm_wrapper;

architecture STRUCTURE of iocm_wrapper is

  component isocm_v10 is
    generic (
      C_NUM_MASTERS : integer;
      C_NUM_SLAVES : integer;
      C_ISCNTLVALUE : std_logic_vector(0 to 7);
      C_ISARCVALUE : std_logic_vector(0 to 7)
    );
    port (
      ISOCM_Clk : in std_logic;
      SYS_Rst : in std_logic;
      ISOCM_Rst : out std_logic;
      M_ISOCMBRAMRDABUS : in std_logic_vector(8 to 28);
      M_ISOCMBRAMWRABUS : in std_logic_vector(8 to 28);
      M_ISOCMBRAMEN : in std_logic;
      M_ISOCMBRAMWRDBUS : in std_logic_vector(0 to 31);
      M_ISOCMBRAMODDWRITEEN : in std_logic;
      M_ISOCMBRAMEVENWRITEEN : in std_logic;
      M_ISOCMDCRBRAMODDEN : in std_logic;
      M_ISOCMDCRBRAMEVENEN : in std_logic;
      M_ISOCMDCRBRAMRDSELECT : in std_logic;
      S_BRAMISOCMRDDBUS : in std_logic_vector(0 to 64*C_NUM_SLAVES-1);
      S_BRAMISOCMDCRRDDBUS : in std_logic_vector(0 to 32*C_NUM_SLAVES-1);
      S_ISOCMSLAVESELECT : in std_logic_vector(0 to C_NUM_SLAVES-1);
      S_ISOCMDCRSLAVESELECT : in std_logic_vector(0 to C_NUM_SLAVES-1);
      BRAMISOCMRDDBUS : out std_logic_vector(0 to 63);
      ISARCVALUE : out std_logic_vector(0 to 7);
      ISCNTLVALUE : out std_logic_vector(0 to 7);
      ISOCMBRAMEN : out std_logic;
      ISOCMBRAMWRDBUS : out std_logic_vector(0 to 31);
      ISOCMBRAMODDWRITEEN : out std_logic;
      ISOCMBRAMEVENWRITEEN : out std_logic;
      ISOCMBRAMRDABUS : out std_logic_vector(8 to 28);
      ISOCMBRAMWRABUS : out std_logic_vector(8 to 28);
      ISOCMDCRBRAMODDEN : out std_logic;
      ISOCMDCRBRAMEVENEN : out std_logic;
      ISOCMDCRBRAMRDSELECT : out std_logic;
      BRAMISOCMDCRRDDBUS : out std_logic_vector(0 to 31)
    );
  end component;

begin

  iocm : isocm_v10
    generic map (
      C_NUM_MASTERS => 1,
      C_NUM_SLAVES => 1,
      C_ISCNTLVALUE => X"83",
      C_ISARCVALUE => X"FF"
    )
    port map (
      ISOCM_Clk => ISOCM_Clk,
      SYS_Rst => SYS_Rst,
      ISOCM_Rst => ISOCM_Rst,
      M_ISOCMBRAMRDABUS => M_ISOCMBRAMRDABUS,
      M_ISOCMBRAMWRABUS => M_ISOCMBRAMWRABUS,
      M_ISOCMBRAMEN => M_ISOCMBRAMEN,
      M_ISOCMBRAMWRDBUS => M_ISOCMBRAMWRDBUS,
      M_ISOCMBRAMODDWRITEEN => M_ISOCMBRAMODDWRITEEN,
      M_ISOCMBRAMEVENWRITEEN => M_ISOCMBRAMEVENWRITEEN,
      M_ISOCMDCRBRAMODDEN => M_ISOCMDCRBRAMODDEN,
      M_ISOCMDCRBRAMEVENEN => M_ISOCMDCRBRAMEVENEN,
      M_ISOCMDCRBRAMRDSELECT => M_ISOCMDCRBRAMRDSELECT,
      S_BRAMISOCMRDDBUS => S_BRAMISOCMRDDBUS,
      S_BRAMISOCMDCRRDDBUS => S_BRAMISOCMDCRRDDBUS,
      S_ISOCMSLAVESELECT => S_ISOCMSLAVESELECT,
      S_ISOCMDCRSLAVESELECT => S_ISOCMDCRSLAVESELECT,
      BRAMISOCMRDDBUS => BRAMISOCMRDDBUS,
      ISARCVALUE => ISARCVALUE,
      ISCNTLVALUE => ISCNTLVALUE,
      ISOCMBRAMEN => ISOCMBRAMEN,
      ISOCMBRAMWRDBUS => ISOCMBRAMWRDBUS,
      ISOCMBRAMODDWRITEEN => ISOCMBRAMODDWRITEEN,
      ISOCMBRAMEVENWRITEEN => ISOCMBRAMEVENWRITEEN,
      ISOCMBRAMRDABUS => ISOCMBRAMRDABUS,
      ISOCMBRAMWRABUS => ISOCMBRAMWRABUS,
      ISOCMDCRBRAMODDEN => ISOCMDCRBRAMODDEN,
      ISOCMDCRBRAMEVENEN => ISOCMDCRBRAMEVENEN,
      ISOCMDCRBRAMRDSELECT => ISOCMDCRBRAMRDSELECT,
      BRAMISOCMDCRRDDBUS => BRAMISOCMDCRRDDBUS
    );

end architecture STRUCTURE;

