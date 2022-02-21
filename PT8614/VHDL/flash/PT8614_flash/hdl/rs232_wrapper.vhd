-------------------------------------------------------------------------------
-- rs232_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library opb_uartlite_v1_00_b;
use opb_uartlite_v1_00_b.All;

entity rs232_wrapper is
  port (
    OPB_Clk : in std_logic;
    OPB_Rst : in std_logic;
    Interrupt : out std_logic;
    OPB_ABus : in std_logic_vector(0 to 31);
    OPB_BE : in std_logic_vector(0 to 3);
    OPB_RNW : in std_logic;
    OPB_select : in std_logic;
    OPB_seqAddr : in std_logic;
    OPB_DBus : in std_logic_vector(0 to 31);
    UART_DBus : out std_logic_vector(0 to 31);
    UART_errAck : out std_logic;
    UART_retry : out std_logic;
    UART_toutSup : out std_logic;
    UART_xferAck : out std_logic;
    RX : in std_logic;
    TX : out std_logic
  );
  attribute x_core_info : STRING;
  attribute x_core_info of rs232_wrapper: entity is "opb_uartlite_v1_00_b";

end rs232_wrapper;

architecture STRUCTURE of rs232_wrapper is

  component opb_uartlite is
    generic (
      C_BASEADDR : std_logic_vector;
      C_HIGHADDR : std_logic_vector;
      C_OPB_DWIDTH : integer;
      C_OPB_AWIDTH : integer;
      C_DATA_BITS : integer;
      C_CLK_FREQ : integer;
      C_BAUDRATE : integer;
      C_USE_PARITY : integer;
      C_ODD_PARITY : integer
    );
    port (
      OPB_Clk : in std_logic;
      OPB_Rst : in std_logic;
      Interrupt : out std_logic;
      OPB_ABus : in std_logic_vector(0 to C_OPB_AWIDTH-1);
      OPB_BE : in std_logic_vector(0 to C_OPB_DWIDTH/8-1);
      OPB_RNW : in std_logic;
      OPB_select : in std_logic;
      OPB_seqAddr : in std_logic;
      OPB_DBus : in std_logic_vector(0 to C_OPB_DWIDTH-1);
      UART_DBus : out std_logic_vector(0 to C_OPB_DWIDTH-1);
      UART_errAck : out std_logic;
      UART_retry : out std_logic;
      UART_toutSup : out std_logic;
      UART_xferAck : out std_logic;
      RX : in std_logic;
      TX : out std_logic
    );
  end component;

begin

  rs232 : opb_uartlite
    generic map (
      C_BASEADDR => X"40600000",
      C_HIGHADDR => X"4060ffff",
      C_OPB_DWIDTH => 32,
      C_OPB_AWIDTH => 32,
      C_DATA_BITS => 8,
      C_CLK_FREQ => 74000000,
      C_BAUDRATE => 9600,
      C_USE_PARITY => 0,
      C_ODD_PARITY => 1
    )
    port map (
      OPB_Clk => OPB_Clk,
      OPB_Rst => OPB_Rst,
      Interrupt => Interrupt,
      OPB_ABus => OPB_ABus,
      OPB_BE => OPB_BE,
      OPB_RNW => OPB_RNW,
      OPB_select => OPB_select,
      OPB_seqAddr => OPB_seqAddr,
      OPB_DBus => OPB_DBus,
      UART_DBus => UART_DBus,
      UART_errAck => UART_errAck,
      UART_retry => UART_retry,
      UART_toutSup => UART_toutSup,
      UART_xferAck => UART_xferAck,
      RX => RX,
      TX => TX
    );

end architecture STRUCTURE;

