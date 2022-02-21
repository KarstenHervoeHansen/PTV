-------------------------------------------------------------------------------
-- plb2opb_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library plb2opb_bridge_v1_01_a;
use plb2opb_bridge_v1_01_a.All;

entity plb2opb_wrapper is
  port (
    PLB_Clk : in std_logic;
    OPB_Clk : in std_logic;
    PLB_Rst : in std_logic;
    OPB_Rst : in std_logic;
    Bus_Error_Det : out std_logic;
    BGI_Trans_Abort : out std_logic;
    BGO_dcrAck : out std_logic;
    BGO_dcrDBus : out std_logic_vector(0 to 31);
    DCR_ABus : in std_logic_vector(0 to 9);
    DCR_DBus : in std_logic_vector(0 to 31);
    DCR_Read : in std_logic;
    DCR_Write : in std_logic;
    BGO_addrAck : out std_logic;
    BGO_MErr : out std_logic_vector(0 to 1);
    BGO_MBusy : out std_logic_vector(0 to 1);
    BGO_rdBTerm : out std_logic;
    BGO_rdComp : out std_logic;
    BGO_rdDAck : out std_logic;
    BGO_rdDBus : out std_logic_vector(0 to 63);
    BGO_rdWdAddr : out std_logic_vector(0 to 3);
    BGO_rearbitrate : out std_logic;
    BGO_SSize : out std_logic_vector(0 to 1);
    BGO_wait : out std_logic;
    BGO_wrBTerm : out std_logic;
    BGO_wrComp : out std_logic;
    BGO_wrDAck : out std_logic;
    PLB_abort : in std_logic;
    PLB_ABus : in std_logic_vector(0 to 31);
    PLB_BE : in std_logic_vector(0 to 7);
    PLB_busLock : in std_logic;
    PLB_compress : in std_logic;
    PLB_guarded : in std_logic;
    PLB_lockErr : in std_logic;
    PLB_masterID : in std_logic_vector(0 to 0);
    PLB_MSize : in std_logic_vector(0 to 1);
    PLB_ordered : in std_logic;
    PLB_PAValid : in std_logic;
    PLB_rdBurst : in std_logic;
    PLB_rdPrim : in std_logic;
    PLB_RNW : in std_logic;
    PLB_SAValid : in std_logic;
    PLB_size : in std_logic_vector(0 to 3);
    PLB_type : in std_logic_vector(0 to 2);
    PLB_wrBurst : in std_logic;
    PLB_wrDBus : in std_logic_vector(0 to 63);
    PLB_wrPrim : in std_logic;
    PLB2OPB_rearb : out std_logic;
    BGO_ABus : out std_logic_vector(0 to 31);
    BGO_BE : out std_logic_vector(0 to 3);
    BGO_busLock : out std_logic;
    BGO_DBus : out std_logic_vector(0 to 31);
    BGO_request : out std_logic;
    BGO_RNW : out std_logic;
    BGO_select : out std_logic;
    BGO_seqAddr : out std_logic;
    OPB_DBus : in std_logic_vector(0 to 31);
    OPB_errAck : in std_logic;
    OPB_MnGrant : in std_logic;
    OPB_retry : in std_logic;
    OPB_timeout : in std_logic;
    OPB_xferAck : in std_logic
  );
  attribute x_core_info : STRING;
  attribute x_core_info of plb2opb_wrapper: entity is "plb2opb_bridge_v1_01_a";

end plb2opb_wrapper;

architecture STRUCTURE of plb2opb_wrapper is

  component plb2opb_bridge is
    generic (
      C_NO_PLB_BURST : integer;
      C_DCR_INTFCE : integer;
      C_FAMILY : string;
      C_NUM_ADDR_RNG : integer;
      C_RNG0_BASEADDR : std_logic_vector;
      C_RNG0_HIGHADDR : std_logic_vector;
      C_RNG1_BASEADDR : std_logic_vector;
      C_RNG1_HIGHADDR : std_logic_vector;
      C_RNG2_BASEADDR : std_logic_vector;
      C_RNG2_HIGHADDR : std_logic_vector;
      C_RNG3_BASEADDR : std_logic_vector;
      C_RNG3_HIGHADDR : std_logic_vector;
      C_PLB_AWIDTH : integer;
      C_PLB_DWIDTH : integer;
      C_PLB_NUM_MASTERS : integer;
      C_PLB_MID_WIDTH : integer;
      C_OPB_AWIDTH : integer;
      C_OPB_DWIDTH : integer;
      C_DCR_BASEADDR : std_logic_vector;
      C_DCR_HIGHADDR : std_logic_vector;
      C_DCR_AWIDTH : integer;
      C_DCR_DWIDTH : integer;
      C_IRQ_ACTIVE : std_logic;
      C_BGI_TRANSABORT_CNT : integer;
      C_CLK_ASYNC : integer;
      C_HIGH_SPEED : integer;
      C_INCLUDE_BGI_TRANSABORT : integer
    );
    port (
      PLB_Clk : in std_logic;
      OPB_Clk : in std_logic;
      PLB_Rst : in std_logic;
      OPB_Rst : in std_logic;
      Bus_Error_Det : out std_logic;
      BGI_Trans_Abort : out std_logic;
      BGO_dcrAck : out std_logic;
      BGO_dcrDBus : out std_logic_vector(0 to C_DCR_DWIDTH-1);
      DCR_ABus : in std_logic_vector(0 to C_DCR_AWIDTH-1);
      DCR_DBus : in std_logic_vector(0 to C_DCR_DWIDTH-1);
      DCR_Read : in std_logic;
      DCR_Write : in std_logic;
      BGO_addrAck : out std_logic;
      BGO_MErr : out std_logic_vector(0 to C_PLB_NUM_MASTERS-1);
      BGO_MBusy : out std_logic_vector(0 to C_PLB_NUM_MASTERS-1);
      BGO_rdBTerm : out std_logic;
      BGO_rdComp : out std_logic;
      BGO_rdDAck : out std_logic;
      BGO_rdDBus : out std_logic_vector(0 to C_PLB_DWIDTH-1);
      BGO_rdWdAddr : out std_logic_vector(0 to 3);
      BGO_rearbitrate : out std_logic;
      BGO_SSize : out std_logic_vector(0 to 1);
      BGO_wait : out std_logic;
      BGO_wrBTerm : out std_logic;
      BGO_wrComp : out std_logic;
      BGO_wrDAck : out std_logic;
      PLB_abort : in std_logic;
      PLB_ABus : in std_logic_vector(0 to C_PLB_AWIDTH-1);
      PLB_BE : in std_logic_vector(0 to C_PLB_DWIDTH/8-1);
      PLB_busLock : in std_logic;
      PLB_compress : in std_logic;
      PLB_guarded : in std_logic;
      PLB_lockErr : in std_logic;
      PLB_masterID : in std_logic_vector(0 to C_PLB_MID_WIDTH-1);
      PLB_MSize : in std_logic_vector(0 to 1);
      PLB_ordered : in std_logic;
      PLB_PAValid : in std_logic;
      PLB_rdBurst : in std_logic;
      PLB_rdPrim : in std_logic;
      PLB_RNW : in std_logic;
      PLB_SAValid : in std_logic;
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      PLB_wrBurst : in std_logic;
      PLB_wrDBus : in std_logic_vector(0 to C_PLB_DWIDTH-1);
      PLB_wrPrim : in std_logic;
      PLB2OPB_rearb : out std_logic;
      BGO_ABus : out std_logic_vector(0 to C_OPB_AWIDTH-1);
      BGO_BE : out std_logic_vector(0 to C_OPB_DWIDTH/8-1);
      BGO_busLock : out std_logic;
      BGO_DBus : out std_logic_vector(0 to C_OPB_DWIDTH-1);
      BGO_request : out std_logic;
      BGO_RNW : out std_logic;
      BGO_select : out std_logic;
      BGO_seqAddr : out std_logic;
      OPB_DBus : in std_logic_vector(0 to C_OPB_DWIDTH-1);
      OPB_errAck : in std_logic;
      OPB_MnGrant : in std_logic;
      OPB_retry : in std_logic;
      OPB_timeout : in std_logic;
      OPB_xferAck : in std_logic
    );
  end component;

begin

  plb2opb : plb2opb_bridge
    generic map (
      C_NO_PLB_BURST => 0,
      C_DCR_INTFCE => 0,
      C_FAMILY => "virtex2p",
      C_NUM_ADDR_RNG => 1,
      C_RNG0_BASEADDR => X"40000000",
      C_RNG0_HIGHADDR => X"7fffffff",
      C_RNG1_BASEADDR => X"FFFFFFFF",
      C_RNG1_HIGHADDR => X"00000000",
      C_RNG2_BASEADDR => X"FFFFFFFF",
      C_RNG2_HIGHADDR => X"00000000",
      C_RNG3_BASEADDR => X"FFFFFFFF",
      C_RNG3_HIGHADDR => X"00000000",
      C_PLB_AWIDTH => 32,
      C_PLB_DWIDTH => 64,
      C_PLB_NUM_MASTERS => 2,
      C_PLB_MID_WIDTH => 1,
      C_OPB_AWIDTH => 32,
      C_OPB_DWIDTH => 32,
      C_DCR_BASEADDR => B"1111111111",
      C_DCR_HIGHADDR => B"0000000000",
      C_DCR_AWIDTH => 10,
      C_DCR_DWIDTH => 32,
      C_IRQ_ACTIVE => '1',
      C_BGI_TRANSABORT_CNT => 31,
      C_CLK_ASYNC => 1,
      C_HIGH_SPEED => 1,
      C_INCLUDE_BGI_TRANSABORT => 1
    )
    port map (
      PLB_Clk => PLB_Clk,
      OPB_Clk => OPB_Clk,
      PLB_Rst => PLB_Rst,
      OPB_Rst => OPB_Rst,
      Bus_Error_Det => Bus_Error_Det,
      BGI_Trans_Abort => BGI_Trans_Abort,
      BGO_dcrAck => BGO_dcrAck,
      BGO_dcrDBus => BGO_dcrDBus,
      DCR_ABus => DCR_ABus,
      DCR_DBus => DCR_DBus,
      DCR_Read => DCR_Read,
      DCR_Write => DCR_Write,
      BGO_addrAck => BGO_addrAck,
      BGO_MErr => BGO_MErr,
      BGO_MBusy => BGO_MBusy,
      BGO_rdBTerm => BGO_rdBTerm,
      BGO_rdComp => BGO_rdComp,
      BGO_rdDAck => BGO_rdDAck,
      BGO_rdDBus => BGO_rdDBus,
      BGO_rdWdAddr => BGO_rdWdAddr,
      BGO_rearbitrate => BGO_rearbitrate,
      BGO_SSize => BGO_SSize,
      BGO_wait => BGO_wait,
      BGO_wrBTerm => BGO_wrBTerm,
      BGO_wrComp => BGO_wrComp,
      BGO_wrDAck => BGO_wrDAck,
      PLB_abort => PLB_abort,
      PLB_ABus => PLB_ABus,
      PLB_BE => PLB_BE,
      PLB_busLock => PLB_busLock,
      PLB_compress => PLB_compress,
      PLB_guarded => PLB_guarded,
      PLB_lockErr => PLB_lockErr,
      PLB_masterID => PLB_masterID,
      PLB_MSize => PLB_MSize,
      PLB_ordered => PLB_ordered,
      PLB_PAValid => PLB_PAValid,
      PLB_rdBurst => PLB_rdBurst,
      PLB_rdPrim => PLB_rdPrim,
      PLB_RNW => PLB_RNW,
      PLB_SAValid => PLB_SAValid,
      PLB_size => PLB_size,
      PLB_type => PLB_type,
      PLB_wrBurst => PLB_wrBurst,
      PLB_wrDBus => PLB_wrDBus,
      PLB_wrPrim => PLB_wrPrim,
      PLB2OPB_rearb => PLB2OPB_rearb,
      BGO_ABus => BGO_ABus,
      BGO_BE => BGO_BE,
      BGO_busLock => BGO_busLock,
      BGO_DBus => BGO_DBus,
      BGO_request => BGO_request,
      BGO_RNW => BGO_RNW,
      BGO_select => BGO_select,
      BGO_seqAddr => BGO_seqAddr,
      OPB_DBus => OPB_DBus,
      OPB_errAck => OPB_errAck,
      OPB_MnGrant => OPB_MnGrant,
      OPB_retry => OPB_retry,
      OPB_timeout => OPB_timeout,
      OPB_xferAck => OPB_xferAck
    );

end architecture STRUCTURE;

