-------------------------------------------------------------------------------
-- opb_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library opb_v20_v1_10_c;
use opb_v20_v1_10_c.All;

entity opb_wrapper is
  port (
    OPB_Clk : in std_logic;
    OPB_Rst : out std_logic;
    SYS_Rst : in std_logic;
    Debug_SYS_Rst : in std_logic;
    WDT_Rst : in std_logic;
    M_ABus : in std_logic_vector(0 to 31);
    M_BE : in std_logic_vector(0 to 3);
    M_beXfer : in std_logic_vector(0 to 0);
    M_busLock : in std_logic_vector(0 to 0);
    M_DBus : in std_logic_vector(0 to 31);
    M_DBusEn : in std_logic_vector(0 to 0);
    M_DBusEn32_63 : in std_logic_vector(0 to 0);
    M_dwXfer : in std_logic_vector(0 to 0);
    M_fwXfer : in std_logic_vector(0 to 0);
    M_hwXfer : in std_logic_vector(0 to 0);
    M_request : in std_logic_vector(0 to 0);
    M_RNW : in std_logic_vector(0 to 0);
    M_select : in std_logic_vector(0 to 0);
    M_seqAddr : in std_logic_vector(0 to 0);
    Sl_beAck : in std_logic_vector(0 to 2);
    Sl_DBus : in std_logic_vector(0 to 95);
    Sl_DBusEn : in std_logic_vector(0 to 2);
    Sl_DBusEn32_63 : in std_logic_vector(0 to 2);
    Sl_errAck : in std_logic_vector(0 to 2);
    Sl_dwAck : in std_logic_vector(0 to 2);
    Sl_fwAck : in std_logic_vector(0 to 2);
    Sl_hwAck : in std_logic_vector(0 to 2);
    Sl_retry : in std_logic_vector(0 to 2);
    Sl_toutSup : in std_logic_vector(0 to 2);
    Sl_xferAck : in std_logic_vector(0 to 2);
    OPB_MRequest : out std_logic_vector(0 to 0);
    OPB_ABus : out std_logic_vector(0 to 31);
    OPB_BE : out std_logic_vector(0 to 3);
    OPB_beXfer : out std_logic;
    OPB_beAck : out std_logic;
    OPB_busLock : out std_logic;
    OPB_rdDBus : out std_logic_vector(0 to 31);
    OPB_wrDBus : out std_logic_vector(0 to 31);
    OPB_DBus : out std_logic_vector(0 to 31);
    OPB_errAck : out std_logic;
    OPB_dwAck : out std_logic;
    OPB_dwXfer : out std_logic;
    OPB_fwAck : out std_logic;
    OPB_fwXfer : out std_logic;
    OPB_hwAck : out std_logic;
    OPB_hwXfer : out std_logic;
    OPB_MGrant : out std_logic_vector(0 to 0);
    OPB_pendReq : out std_logic_vector(0 to 0);
    OPB_retry : out std_logic;
    OPB_RNW : out std_logic;
    OPB_select : out std_logic;
    OPB_seqAddr : out std_logic;
    OPB_timeout : out std_logic;
    OPB_toutSup : out std_logic;
    OPB_xferAck : out std_logic
  );
  attribute x_core_info : STRING;
  attribute x_core_info of opb_wrapper: entity is "opb_v20_v1_10_c";

end opb_wrapper;

architecture STRUCTURE of opb_wrapper is

  component opb_v20 is
    generic (
      C_BASEADDR : std_logic_vector;
      C_HIGHADDR : std_logic_vector;
      C_OPB_AWIDTH : integer;
      C_OPB_DWIDTH : integer;
      C_NUM_MASTERS : integer;
      C_NUM_SLAVES : integer;
      C_USE_LUT_OR : integer;
      C_EXT_RESET_HIGH : integer;
      C_DYNAM_PRIORITY : integer;
      C_PARK : integer;
      C_PROC_INTRFCE : integer;
      C_REG_GRANTS : integer;
      C_DEV_BLK_ID : integer;
      C_DEV_MIR_ENABLE : integer
    );
    port (
      OPB_Clk : in std_logic;
      OPB_Rst : out std_logic;
      SYS_Rst : in std_logic;
      Debug_SYS_Rst : in std_logic;
      WDT_Rst : in std_logic;
      M_ABus : in std_logic_vector(0 to (C_OPB_AWIDTH*C_NUM_MASTERS)-1);
      M_BE : in std_logic_vector(0 to (C_OPB_DWIDTH+7)/8*C_NUM_MASTERS-1);
      M_beXfer : in std_logic_vector(0 to C_NUM_MASTERS-1);
      M_busLock : in std_logic_vector(0 to C_NUM_MASTERS-1);
      M_DBus : in std_logic_vector(0 to (C_OPB_DWIDTH*C_NUM_MASTERS)-1);
      M_DBusEn : in std_logic_vector(0 to C_NUM_MASTERS-1);
      M_DBusEn32_63 : in std_logic_vector(0 to C_NUM_MASTERS-1);
      M_dwXfer : in std_logic_vector(0 to C_NUM_MASTERS-1);
      M_fwXfer : in std_logic_vector(0 to C_NUM_MASTERS-1);
      M_hwXfer : in std_logic_vector(0 to C_NUM_MASTERS-1);
      M_request : in std_logic_vector(0 to C_NUM_MASTERS-1);
      M_RNW : in std_logic_vector(0 to C_NUM_MASTERS-1);
      M_select : in std_logic_vector(0 to C_NUM_MASTERS-1);
      M_seqAddr : in std_logic_vector(0 to C_NUM_MASTERS-1);
      Sl_beAck : in std_logic_vector(0 to C_NUM_SLAVES-1);
      Sl_DBus : in std_logic_vector(0 to (C_OPB_DWIDTH*C_NUM_SLAVES)-1);
      Sl_DBusEn : in std_logic_vector(0 to C_NUM_SLAVES-1);
      Sl_DBusEn32_63 : in std_logic_vector(0 to C_NUM_SLAVES-1);
      Sl_errAck : in std_logic_vector(0 to C_NUM_SLAVES-1);
      Sl_dwAck : in std_logic_vector(0 to C_NUM_SLAVES-1);
      Sl_fwAck : in std_logic_vector(0 to C_NUM_SLAVES-1);
      Sl_hwAck : in std_logic_vector(0 to C_NUM_SLAVES-1);
      Sl_retry : in std_logic_vector(0 to C_NUM_SLAVES-1);
      Sl_toutSup : in std_logic_vector(0 to C_NUM_SLAVES-1);
      Sl_xferAck : in std_logic_vector(0 to C_NUM_SLAVES-1);
      OPB_MRequest : out std_logic_vector(0 to C_NUM_MASTERS-1);
      OPB_ABus : out std_logic_vector(0 to C_OPB_AWIDTH-1);
      OPB_BE : out std_logic_vector(0 to (C_OPB_DWIDTH+7)/8-1);
      OPB_beXfer : out std_logic;
      OPB_beAck : out std_logic;
      OPB_busLock : out std_logic;
      OPB_rdDBus : out std_logic_vector(0 to C_OPB_DWIDTH-1);
      OPB_wrDBus : out std_logic_vector(0 to C_OPB_DWIDTH-1);
      OPB_DBus : out std_logic_vector(0 to C_OPB_DWIDTH-1);
      OPB_errAck : out std_logic;
      OPB_dwAck : out std_logic;
      OPB_dwXfer : out std_logic;
      OPB_fwAck : out std_logic;
      OPB_fwXfer : out std_logic;
      OPB_hwAck : out std_logic;
      OPB_hwXfer : out std_logic;
      OPB_MGrant : out std_logic_vector(0 to C_NUM_MASTERS-1);
      OPB_pendReq : out std_logic_vector(0 to C_NUM_MASTERS-1);
      OPB_retry : out std_logic;
      OPB_RNW : out std_logic;
      OPB_select : out std_logic;
      OPB_seqAddr : out std_logic;
      OPB_timeout : out std_logic;
      OPB_toutSup : out std_logic;
      OPB_xferAck : out std_logic
    );
  end component;

begin

  opb : opb_v20
    generic map (
      C_BASEADDR => X"FFFFFFFF",
      C_HIGHADDR => X"00000000",
      C_OPB_AWIDTH => 32,
      C_OPB_DWIDTH => 32,
      C_NUM_MASTERS => 1,
      C_NUM_SLAVES => 3,
      C_USE_LUT_OR => 1,
      C_EXT_RESET_HIGH => 1,
      C_DYNAM_PRIORITY => 0,
      C_PARK => 0,
      C_PROC_INTRFCE => 0,
      C_REG_GRANTS => 1,
      C_DEV_BLK_ID => 0,
      C_DEV_MIR_ENABLE => 0
    )
    port map (
      OPB_Clk => OPB_Clk,
      OPB_Rst => OPB_Rst,
      SYS_Rst => SYS_Rst,
      Debug_SYS_Rst => Debug_SYS_Rst,
      WDT_Rst => WDT_Rst,
      M_ABus => M_ABus,
      M_BE => M_BE,
      M_beXfer => M_beXfer,
      M_busLock => M_busLock,
      M_DBus => M_DBus,
      M_DBusEn => M_DBusEn,
      M_DBusEn32_63 => M_DBusEn32_63,
      M_dwXfer => M_dwXfer,
      M_fwXfer => M_fwXfer,
      M_hwXfer => M_hwXfer,
      M_request => M_request,
      M_RNW => M_RNW,
      M_select => M_select,
      M_seqAddr => M_seqAddr,
      Sl_beAck => Sl_beAck,
      Sl_DBus => Sl_DBus,
      Sl_DBusEn => Sl_DBusEn,
      Sl_DBusEn32_63 => Sl_DBusEn32_63,
      Sl_errAck => Sl_errAck,
      Sl_dwAck => Sl_dwAck,
      Sl_fwAck => Sl_fwAck,
      Sl_hwAck => Sl_hwAck,
      Sl_retry => Sl_retry,
      Sl_toutSup => Sl_toutSup,
      Sl_xferAck => Sl_xferAck,
      OPB_MRequest => OPB_MRequest,
      OPB_ABus => OPB_ABus,
      OPB_BE => OPB_BE,
      OPB_beXfer => OPB_beXfer,
      OPB_beAck => OPB_beAck,
      OPB_busLock => OPB_busLock,
      OPB_rdDBus => OPB_rdDBus,
      OPB_wrDBus => OPB_wrDBus,
      OPB_DBus => OPB_DBus,
      OPB_errAck => OPB_errAck,
      OPB_dwAck => OPB_dwAck,
      OPB_dwXfer => OPB_dwXfer,
      OPB_fwAck => OPB_fwAck,
      OPB_fwXfer => OPB_fwXfer,
      OPB_hwAck => OPB_hwAck,
      OPB_hwXfer => OPB_hwXfer,
      OPB_MGrant => OPB_MGrant,
      OPB_pendReq => OPB_pendReq,
      OPB_retry => OPB_retry,
      OPB_RNW => OPB_RNW,
      OPB_select => OPB_select,
      OPB_seqAddr => OPB_seqAddr,
      OPB_timeout => OPB_timeout,
      OPB_toutSup => OPB_toutSup,
      OPB_xferAck => OPB_xferAck
    );

end architecture STRUCTURE;

