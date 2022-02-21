-------------------------------------------------------------------------------
-- generic_gpio_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library opb_gpio_v3_01_b;
use opb_gpio_v3_01_b.All;

entity generic_gpio_wrapper is
  port (
    OPB_ABus : in std_logic_vector(0 to 31);
    OPB_BE : in std_logic_vector(0 to 3);
    OPB_Clk : in std_logic;
    OPB_DBus : in std_logic_vector(0 to 31);
    OPB_RNW : in std_logic;
    OPB_Rst : in std_logic;
    OPB_select : in std_logic;
    OPB_seqAddr : in std_logic;
    Sln_DBus : out std_logic_vector(0 to 31);
    Sln_errAck : out std_logic;
    Sln_retry : out std_logic;
    Sln_toutSup : out std_logic;
    Sln_xferAck : out std_logic;
    IP2INTC_Irpt : out std_logic;
    GPIO_in : in std_logic_vector(0 to 1);
    GPIO_d_out : out std_logic_vector(0 to 1);
    GPIO_t_out : out std_logic_vector(0 to 1);
    GPIO2_in : in std_logic_vector(0 to 1);
    GPIO2_d_out : out std_logic_vector(0 to 1);
    GPIO2_t_out : out std_logic_vector(0 to 1);
    GPIO_IO_I : in std_logic_vector(0 to 1);
    GPIO_IO_O : out std_logic_vector(0 to 1);
    GPIO_IO_T : out std_logic_vector(0 to 1);
    GPIO2_IO_I : in std_logic_vector(0 to 1);
    GPIO2_IO_O : out std_logic_vector(0 to 1);
    GPIO2_IO_T : out std_logic_vector(0 to 1)
  );
  attribute x_core_info : STRING;
  attribute x_core_info of generic_gpio_wrapper: entity is "opb_gpio_v3_01_b";

end generic_gpio_wrapper;

architecture STRUCTURE of generic_gpio_wrapper is

  component opb_gpio is
    generic (
      C_BASEADDR : std_logic_vector(0 to 31);
      C_HIGHADDR : std_logic_vector(0 to 31);
      C_USER_ID_CODE : INTEGER;
      C_OPB_AWIDTH : INTEGER;
      C_OPB_DWIDTH : INTEGER;
      C_FAMILY : STRING;
      C_GPIO_WIDTH : INTEGER;
      C_ALL_INPUTS : INTEGER;
      C_INTERRUPT_PRESENT : INTEGER;
      C_IS_BIDIR : INTEGER;
      C_DOUT_DEFAULT : std_logic_vector;
      C_TRI_DEFAULT : std_logic_vector;
      C_IS_DUAL : INTEGER;
      C_ALL_INPUTS_2 : INTEGER;
      C_IS_BIDIR_2 : INTEGER;
      C_DOUT_DEFAULT_2 : std_logic_vector;
      C_TRI_DEFAULT_2 : std_logic_vector
    );
    port (
      OPB_ABus : in std_logic_vector(0 to (C_OPB_AWIDTH-1));
      OPB_BE : in std_logic_vector(0 to ((C_OPB_DWIDTH/8)-1));
      OPB_Clk : in std_logic;
      OPB_DBus : in std_logic_vector(0 to (C_OPB_DWIDTH-1));
      OPB_RNW : in std_logic;
      OPB_Rst : in std_logic;
      OPB_select : in std_logic;
      OPB_seqAddr : in std_logic;
      Sln_DBus : out std_logic_vector(0 to (C_OPB_DWIDTH-1));
      Sln_errAck : out std_logic;
      Sln_retry : out std_logic;
      Sln_toutSup : out std_logic;
      Sln_xferAck : out std_logic;
      IP2INTC_Irpt : out std_logic;
      GPIO_in : in std_logic_vector(0 to (C_GPIO_WIDTH-1));
      GPIO_d_out : out std_logic_vector(0 to (C_GPIO_WIDTH-1));
      GPIO_t_out : out std_logic_vector(0 to (C_GPIO_WIDTH-1));
      GPIO2_in : in std_logic_vector(0 to (C_GPIO_WIDTH-1));
      GPIO2_d_out : out std_logic_vector(0 to (C_GPIO_WIDTH-1));
      GPIO2_t_out : out std_logic_vector(0 to (C_GPIO_WIDTH-1));
      GPIO_IO_I : in std_logic_vector(0 to (C_GPIO_WIDTH-1));
      GPIO_IO_O : out std_logic_vector(0 to (C_GPIO_WIDTH-1));
      GPIO_IO_T : out std_logic_vector(0 to (C_GPIO_WIDTH-1));
      GPIO2_IO_I : in std_logic_vector(0 to (C_GPIO_WIDTH-1));
      GPIO2_IO_O : out std_logic_vector(0 to (C_GPIO_WIDTH-1));
      GPIO2_IO_T : out std_logic_vector(0 to (C_GPIO_WIDTH-1))
    );
  end component;

begin

  generic_gpio : opb_gpio
    generic map (
      C_BASEADDR => X"40000000",
      C_HIGHADDR => X"4000ffff",
      C_USER_ID_CODE => 3,
      C_OPB_AWIDTH => 32,
      C_OPB_DWIDTH => 32,
      C_FAMILY => "virtex2p",
      C_GPIO_WIDTH => 2,
      C_ALL_INPUTS => 0,
      C_INTERRUPT_PRESENT => 0,
      C_IS_BIDIR => 1,
      C_DOUT_DEFAULT => X"00000000",
      C_TRI_DEFAULT => X"ffffffff",
      C_IS_DUAL => 0,
      C_ALL_INPUTS_2 => 0,
      C_IS_BIDIR_2 => 1,
      C_DOUT_DEFAULT_2 => X"00000000",
      C_TRI_DEFAULT_2 => X"ffffffff"
    )
    port map (
      OPB_ABus => OPB_ABus,
      OPB_BE => OPB_BE,
      OPB_Clk => OPB_Clk,
      OPB_DBus => OPB_DBus,
      OPB_RNW => OPB_RNW,
      OPB_Rst => OPB_Rst,
      OPB_select => OPB_select,
      OPB_seqAddr => OPB_seqAddr,
      Sln_DBus => Sln_DBus,
      Sln_errAck => Sln_errAck,
      Sln_retry => Sln_retry,
      Sln_toutSup => Sln_toutSup,
      Sln_xferAck => Sln_xferAck,
      IP2INTC_Irpt => IP2INTC_Irpt,
      GPIO_in => GPIO_in,
      GPIO_d_out => GPIO_d_out,
      GPIO_t_out => GPIO_t_out,
      GPIO2_in => GPIO2_in,
      GPIO2_d_out => GPIO2_d_out,
      GPIO2_t_out => GPIO2_t_out,
      GPIO_IO_I => GPIO_IO_I,
      GPIO_IO_O => GPIO_IO_O,
      GPIO_IO_T => GPIO_IO_T,
      GPIO2_IO_I => GPIO2_IO_I,
      GPIO2_IO_O => GPIO2_IO_O,
      GPIO2_IO_T => GPIO2_IO_T
    );

end architecture STRUCTURE;

