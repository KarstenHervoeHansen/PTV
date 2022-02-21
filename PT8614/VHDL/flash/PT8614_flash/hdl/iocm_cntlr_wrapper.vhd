-------------------------------------------------------------------------------
-- iocm_cntlr_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library isbram_if_cntlr_v3_00_a;
use isbram_if_cntlr_v3_00_a.All;

entity iocm_cntlr_wrapper is
  port (
    BRAMISOCMCLK : in std_logic;
    ISBRAMRST : in std_logic;
    ISOCMBRAMRDABUS : in std_logic_vector(8 to 28);
    ISOCMBRAMWRABUS : in std_logic_vector(8 to 28);
    ISOCMBRAMEN : in std_logic;
    ISOCMBRAMWRDBUS : in std_logic_vector(0 to 31);
    ISOCMBRAMODDWRITEEN : in std_logic;
    ISOCMBRAMEVENWRITEEN : in std_logic;
    ISOCMDCRBRAMODDEN : in std_logic;
    ISOCMDCRBRAMEVENEN : in std_logic;
    ISOCMDCRBRAMRDSELECT : in std_logic;
    S_BRAMISOCMRDDBUS : out std_logic_vector(0 to 63);
    S_BRAMISOCMDCRRDDBUS : out std_logic_vector(0 to 31);
    S_ISOCMSLAVESELECT : out std_logic;
    S_ISOCMDCRSLAVESELECT : out std_logic;
    BRAM_Rst_A : out std_logic;
    BRAM_Clk_A : out std_logic;
    BRAM_Addr_A : out std_logic_vector(0 to 31);
    BRAM_EN_A : out std_logic;
    BRAM_WEN_A : out std_logic_vector(0 to 1);
    BRAM_Dout_A : out std_logic_vector(0 to 63);
    BRAM_Din_A : in std_logic_vector(0 to 63);
    BRAM_Rst_B : out std_logic;
    BRAM_Clk_B : out std_logic;
    BRAM_Addr_B : out std_logic_vector(0 to 31);
    BRAM_EN_B : out std_logic;
    BRAM_WEN_B : out std_logic_vector(0 to 1);
    BRAM_Dout_B : out std_logic_vector(0 to 63);
    BRAM_Din_B : in std_logic_vector(0 to 63)
  );
  attribute x_core_info : STRING;
  attribute x_core_info of iocm_cntlr_wrapper: entity is "isbram_if_cntlr_v3_00_a";

end iocm_cntlr_wrapper;

architecture STRUCTURE of iocm_cntlr_wrapper is

  component isbram_if_cntlr is
    generic (
      C_BASEADDR : std_logic_vector(0 to 31);
      C_HIGHADDR : std_logic_vector(0 to 31);
      C_BRAM_EN : integer;
      C_RANGECHECK : integer
    );
    port (
      BRAMISOCMCLK : in std_logic;
      ISBRAMRST : in std_logic;
      ISOCMBRAMRDABUS : in std_logic_vector(8 to 28);
      ISOCMBRAMWRABUS : in std_logic_vector(8 to 28);
      ISOCMBRAMEN : in std_logic;
      ISOCMBRAMWRDBUS : in std_logic_vector(0 to 31);
      ISOCMBRAMODDWRITEEN : in std_logic;
      ISOCMBRAMEVENWRITEEN : in std_logic;
      ISOCMDCRBRAMODDEN : in std_logic;
      ISOCMDCRBRAMEVENEN : in std_logic;
      ISOCMDCRBRAMRDSELECT : in std_logic;
      S_BRAMISOCMRDDBUS : out std_logic_vector(0 to 63);
      S_BRAMISOCMDCRRDDBUS : out std_logic_vector(0 to 31);
      S_ISOCMSLAVESELECT : out std_logic;
      S_ISOCMDCRSLAVESELECT : out std_logic;
      BRAM_Rst_A : out std_logic;
      BRAM_Clk_A : out std_logic;
      BRAM_Addr_A : out std_logic_vector(0 to 31);
      BRAM_EN_A : out std_logic;
      BRAM_WEN_A : out std_logic_vector(0 to 1);
      BRAM_Dout_A : out std_logic_vector(0 to 63);
      BRAM_Din_A : in std_logic_vector(0 to 63);
      BRAM_Rst_B : out std_logic;
      BRAM_Clk_B : out std_logic;
      BRAM_Addr_B : out std_logic_vector(0 to 31);
      BRAM_EN_B : out std_logic;
      BRAM_WEN_B : out std_logic_vector(0 to 1);
      BRAM_Dout_B : out std_logic_vector(0 to 63);
      BRAM_Din_B : in std_logic_vector(0 to 63)
    );
  end component;

begin

  iocm_cntlr : isbram_if_cntlr
    generic map (
      C_BASEADDR => X"ffff8000",
      C_HIGHADDR => X"ffffffff",
      C_BRAM_EN => 0,
      C_RANGECHECK => 0
    )
    port map (
      BRAMISOCMCLK => BRAMISOCMCLK,
      ISBRAMRST => ISBRAMRST,
      ISOCMBRAMRDABUS => ISOCMBRAMRDABUS,
      ISOCMBRAMWRABUS => ISOCMBRAMWRABUS,
      ISOCMBRAMEN => ISOCMBRAMEN,
      ISOCMBRAMWRDBUS => ISOCMBRAMWRDBUS,
      ISOCMBRAMODDWRITEEN => ISOCMBRAMODDWRITEEN,
      ISOCMBRAMEVENWRITEEN => ISOCMBRAMEVENWRITEEN,
      ISOCMDCRBRAMODDEN => ISOCMDCRBRAMODDEN,
      ISOCMDCRBRAMEVENEN => ISOCMDCRBRAMEVENEN,
      ISOCMDCRBRAMRDSELECT => ISOCMDCRBRAMRDSELECT,
      S_BRAMISOCMRDDBUS => S_BRAMISOCMRDDBUS,
      S_BRAMISOCMDCRRDDBUS => S_BRAMISOCMDCRRDDBUS,
      S_ISOCMSLAVESELECT => S_ISOCMSLAVESELECT,
      S_ISOCMDCRSLAVESELECT => S_ISOCMDCRSLAVESELECT,
      BRAM_Rst_A => BRAM_Rst_A,
      BRAM_Clk_A => BRAM_Clk_A,
      BRAM_Addr_A => BRAM_Addr_A,
      BRAM_EN_A => BRAM_EN_A,
      BRAM_WEN_A => BRAM_WEN_A,
      BRAM_Dout_A => BRAM_Dout_A,
      BRAM_Din_A => BRAM_Din_A,
      BRAM_Rst_B => BRAM_Rst_B,
      BRAM_Clk_B => BRAM_Clk_B,
      BRAM_Addr_B => BRAM_Addr_B,
      BRAM_EN_B => BRAM_EN_B,
      BRAM_WEN_B => BRAM_WEN_B,
      BRAM_Dout_B => BRAM_Dout_B,
      BRAM_Din_B => BRAM_Din_B
    );

end architecture STRUCTURE;

