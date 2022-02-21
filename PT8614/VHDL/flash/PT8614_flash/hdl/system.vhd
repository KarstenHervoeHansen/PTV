-------------------------------------------------------------------------------
-- system.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity system is
  port (
    fpga_0_RS232_req_to_send_pin : out std_logic;
    fpga_0_RS232_RX_pin : in std_logic;
    fpga_0_RS232_TX_pin : out std_logic;
--    fpga_0_Generic_GPIO_GPIO_d_out_pin : out std_logic_vector(0 to 1);
--    fpga_0_Generic_GPIO_GPIO_in_pin : in std_logic_vector(0 to 1);
--    fpga_0_Generic_GPIO_GPIO_t_out_pin : out std_logic_vector(0 to 1);
--    fpga_0_Generic_GPIO_GPIO_IO_pin : inout std_logic_vector(0 to 1);
--    fpga_0_LEDS_GPIO_d_out_pin : out std_logic_vector(0 to 31);
    fpga_0_Generic_External_Memory_Mem_DQ_pin : inout std_logic_vector(0 to 15);
    fpga_0_Generic_External_Memory_Mem_A_pin : out std_logic_vector(7 to 30);
    fpga_0_Generic_External_Memory_Mem_BEN_pin : out std_logic_vector(0 to 1);
    fpga_0_Generic_External_Memory_Mem_WEN_pin : out std_logic;
    fpga_0_Generic_External_Memory_Mem_OEN_pin : out std_logic_vector(0 to 0);
    fpga_0_Generic_External_Memory_Mem_CEN_pin : out std_logic_vector(0 to 0);
    fpga_0_Generic_External_Memory_flash_csn_dummy_pin : out std_logic;
    fpga_0_Generic_External_Memory_Mem_RPN_pin : out std_logic;
    sys_clk_pin_p : in std_logic;
    sys_clk_pin_n : in std_logic;
    sys_rst_pin : in std_logic;
    FADV : out std_logic;
    FRST : out std_logic;
    FWP : out std_logic
  );
end system;

architecture STRUCTURE of system is

  attribute box_type : STRING;

  component ppc405_0_wrapper is
    port (
      C405CPMCORESLEEPREQ : out std_logic;
      C405CPMMSRCE : out std_logic;
      C405CPMMSREE : out std_logic;
      C405CPMTIMERIRQ : out std_logic;
      C405CPMTIMERRESETREQ : out std_logic;
      C405XXXMACHINECHECK : out std_logic;
      CPMC405CLOCK : in std_logic;
      CPMC405CORECLKINACTIVE : in std_logic;
      CPMC405CPUCLKEN : in std_logic;
      CPMC405JTAGCLKEN : in std_logic;
      CPMC405TIMERCLKEN : in std_logic;
      CPMC405TIMERTICK : in std_logic;
      MCBCPUCLKEN : in std_logic;
      MCBTIMEREN : in std_logic;
      MCPPCRST : in std_logic;
      PLBCLK : in std_logic;
      DCRCLK : in std_logic;
      C405RSTCHIPRESETREQ : out std_logic;
      C405RSTCORERESETREQ : out std_logic;
      C405RSTSYSRESETREQ : out std_logic;
      RSTC405RESETCHIP : in std_logic;
      RSTC405RESETCORE : in std_logic;
      RSTC405RESETSYS : in std_logic;
      C405PLBICUABUS : out std_logic_vector(0 to 31);
      C405PLBICUBE : out std_logic_vector(0 to 7);
      C405PLBICURNW : out std_logic;
      C405PLBICUABORT : out std_logic;
      C405PLBICUBUSLOCK : out std_logic;
      C405PLBICUU0ATTR : out std_logic;
      C405PLBICUGUARDED : out std_logic;
      C405PLBICULOCKERR : out std_logic;
      C405PLBICUMSIZE : out std_logic_vector(0 to 1);
      C405PLBICUORDERED : out std_logic;
      C405PLBICUPRIORITY : out std_logic_vector(0 to 1);
      C405PLBICURDBURST : out std_logic;
      C405PLBICUREQUEST : out std_logic;
      C405PLBICUSIZE : out std_logic_vector(0 to 3);
      C405PLBICUTYPE : out std_logic_vector(0 to 2);
      C405PLBICUWRBURST : out std_logic;
      C405PLBICUWRDBUS : out std_logic_vector(0 to 63);
      C405PLBICUCACHEABLE : out std_logic;
      PLBC405ICUADDRACK : in std_logic;
      PLBC405ICUBUSY : in std_logic;
      PLBC405ICUERR : in std_logic;
      PLBC405ICURDBTERM : in std_logic;
      PLBC405ICURDDACK : in std_logic;
      PLBC405ICURDDBUS : in std_logic_vector(0 to 63);
      PLBC405ICURDWDADDR : in std_logic_vector(0 to 3);
      PLBC405ICUREARBITRATE : in std_logic;
      PLBC405ICUWRBTERM : in std_logic;
      PLBC405ICUWRDACK : in std_logic;
      PLBC405ICUSSIZE : in std_logic_vector(0 to 1);
      PLBC405ICUSERR : in std_logic;
      PLBC405ICUSBUSYS : in std_logic;
      C405PLBDCUABUS : out std_logic_vector(0 to 31);
      C405PLBDCUBE : out std_logic_vector(0 to 7);
      C405PLBDCURNW : out std_logic;
      C405PLBDCUABORT : out std_logic;
      C405PLBDCUBUSLOCK : out std_logic;
      C405PLBDCUU0ATTR : out std_logic;
      C405PLBDCUGUARDED : out std_logic;
      C405PLBDCULOCKERR : out std_logic;
      C405PLBDCUMSIZE : out std_logic_vector(0 to 1);
      C405PLBDCUORDERED : out std_logic;
      C405PLBDCUPRIORITY : out std_logic_vector(0 to 1);
      C405PLBDCURDBURST : out std_logic;
      C405PLBDCUREQUEST : out std_logic;
      C405PLBDCUSIZE : out std_logic_vector(0 to 3);
      C405PLBDCUTYPE : out std_logic_vector(0 to 2);
      C405PLBDCUWRBURST : out std_logic;
      C405PLBDCUWRDBUS : out std_logic_vector(0 to 63);
      C405PLBDCUCACHEABLE : out std_logic;
      C405PLBDCUWRITETHRU : out std_logic;
      PLBC405DCUADDRACK : in std_logic;
      PLBC405DCUBUSY : in std_logic;
      PLBC405DCUERR : in std_logic;
      PLBC405DCURDBTERM : in std_logic;
      PLBC405DCURDDACK : in std_logic;
      PLBC405DCURDDBUS : in std_logic_vector(0 to 63);
      PLBC405DCURDWDADDR : in std_logic_vector(0 to 3);
      PLBC405DCUREARBITRATE : in std_logic;
      PLBC405DCUWRBTERM : in std_logic;
      PLBC405DCUWRDACK : in std_logic;
      PLBC405DCUSSIZE : in std_logic_vector(0 to 1);
      PLBC405DCUSERR : in std_logic;
      PLBC405DCUSBUSYS : in std_logic;
      BRAMDSOCMCLK : in std_logic;
      BRAMDSOCMRDDBUS : in std_logic_vector(0 to 31);
      DSARCVALUE : in std_logic_vector(0 to 7);
      DSCNTLVALUE : in std_logic_vector(0 to 7);
      DSOCMBRAMABUS : out std_logic_vector(8 to 29);
      DSOCMBRAMBYTEWRITE : out std_logic_vector(0 to 3);
      DSOCMBRAMEN : out std_logic;
      DSOCMBRAMWRDBUS : out std_logic_vector(0 to 31);
      DSOCMBUSY : out std_logic;
      BRAMISOCMCLK : in std_logic;
      BRAMISOCMRDDBUS : in std_logic_vector(0 to 63);
      ISARCVALUE : in std_logic_vector(0 to 7);
      ISCNTLVALUE : in std_logic_vector(0 to 7);
      ISOCMBRAMEN : out std_logic;
      ISOCMBRAMEVENWRITEEN : out std_logic;
      ISOCMBRAMODDWRITEEN : out std_logic;
      ISOCMBRAMRDABUS : out std_logic_vector(8 to 28);
      ISOCMBRAMWRABUS : out std_logic_vector(8 to 28);
      ISOCMBRAMWRDBUS : out std_logic_vector(0 to 31);
      C405DCRABUS : out std_logic_vector(0 to 9);
      C405DCRDBUSOUT : out std_logic_vector(0 to 31);
      C405DCRREAD : out std_logic;
      C405DCRWRITE : out std_logic;
      DCRC405ACK : in std_logic;
      DCRC405DBUSIN : in std_logic_vector(0 to 31);
      EICC405CRITINPUTIRQ : in std_logic;
      EICC405EXTINPUTIRQ : in std_logic;
      C405JTGCAPTUREDR : out std_logic;
      C405JTGEXTEST : out std_logic;
      C405JTGPGMOUT : out std_logic;
      C405JTGSHIFTDR : out std_logic;
      C405JTGTDO : out std_logic;
      C405JTGTDOEN : out std_logic;
      C405JTGUPDATEDR : out std_logic;
      MCBJTAGEN : in std_logic;
      JTGC405BNDSCANTDO : in std_logic;
      JTGC405TCK : in std_logic;
      JTGC405TDI : in std_logic;
      JTGC405TMS : in std_logic;
      JTGC405TRSTNEG : in std_logic;
      C405DBGMSRWE : out std_logic;
      C405DBGSTOPACK : out std_logic;
      C405DBGWBCOMPLETE : out std_logic;
      C405DBGWBFULL : out std_logic;
      C405DBGWBIAR : out std_logic_vector(0 to 29);
      DBGC405DEBUGHALT : in std_logic;
      DBGC405EXTBUSHOLDACK : in std_logic;
      DBGC405UNCONDDEBUGEVENT : in std_logic;
      C405TRCCYCLE : out std_logic;
      C405TRCEVENEXECUTIONSTATUS : out std_logic_vector(0 to 1);
      C405TRCODDEXECUTIONSTATUS : out std_logic_vector(0 to 1);
      C405TRCTRACESTATUS : out std_logic_vector(0 to 3);
      C405TRCTRIGGEREVENTOUT : out std_logic;
      C405TRCTRIGGEREVENTTYPE : out std_logic_vector(0 to 10);
      TRCC405TRACEDISABLE : in std_logic;
      TRCC405TRIGGEREVENTIN : in std_logic
    );
  end component;

  attribute box_type of ppc405_0_wrapper: component is "black_box";

  component ppc405_1_wrapper is
    port (
      C405CPMCORESLEEPREQ : out std_logic;
      C405CPMMSRCE : out std_logic;
      C405CPMMSREE : out std_logic;
      C405CPMTIMERIRQ : out std_logic;
      C405CPMTIMERRESETREQ : out std_logic;
      C405XXXMACHINECHECK : out std_logic;
      CPMC405CLOCK : in std_logic;
      CPMC405CORECLKINACTIVE : in std_logic;
      CPMC405CPUCLKEN : in std_logic;
      CPMC405JTAGCLKEN : in std_logic;
      CPMC405TIMERCLKEN : in std_logic;
      CPMC405TIMERTICK : in std_logic;
      MCBCPUCLKEN : in std_logic;
      MCBTIMEREN : in std_logic;
      MCPPCRST : in std_logic;
      PLBCLK : in std_logic;
      DCRCLK : in std_logic;
      C405RSTCHIPRESETREQ : out std_logic;
      C405RSTCORERESETREQ : out std_logic;
      C405RSTSYSRESETREQ : out std_logic;
      RSTC405RESETCHIP : in std_logic;
      RSTC405RESETCORE : in std_logic;
      RSTC405RESETSYS : in std_logic;
      C405PLBICUABUS : out std_logic_vector(0 to 31);
      C405PLBICUBE : out std_logic_vector(0 to 7);
      C405PLBICURNW : out std_logic;
      C405PLBICUABORT : out std_logic;
      C405PLBICUBUSLOCK : out std_logic;
      C405PLBICUU0ATTR : out std_logic;
      C405PLBICUGUARDED : out std_logic;
      C405PLBICULOCKERR : out std_logic;
      C405PLBICUMSIZE : out std_logic_vector(0 to 1);
      C405PLBICUORDERED : out std_logic;
      C405PLBICUPRIORITY : out std_logic_vector(0 to 1);
      C405PLBICURDBURST : out std_logic;
      C405PLBICUREQUEST : out std_logic;
      C405PLBICUSIZE : out std_logic_vector(0 to 3);
      C405PLBICUTYPE : out std_logic_vector(0 to 2);
      C405PLBICUWRBURST : out std_logic;
      C405PLBICUWRDBUS : out std_logic_vector(0 to 63);
      C405PLBICUCACHEABLE : out std_logic;
      PLBC405ICUADDRACK : in std_logic;
      PLBC405ICUBUSY : in std_logic;
      PLBC405ICUERR : in std_logic;
      PLBC405ICURDBTERM : in std_logic;
      PLBC405ICURDDACK : in std_logic;
      PLBC405ICURDDBUS : in std_logic_vector(0 to 63);
      PLBC405ICURDWDADDR : in std_logic_vector(0 to 3);
      PLBC405ICUREARBITRATE : in std_logic;
      PLBC405ICUWRBTERM : in std_logic;
      PLBC405ICUWRDACK : in std_logic;
      PLBC405ICUSSIZE : in std_logic_vector(0 to 1);
      PLBC405ICUSERR : in std_logic;
      PLBC405ICUSBUSYS : in std_logic;
      C405PLBDCUABUS : out std_logic_vector(0 to 31);
      C405PLBDCUBE : out std_logic_vector(0 to 7);
      C405PLBDCURNW : out std_logic;
      C405PLBDCUABORT : out std_logic;
      C405PLBDCUBUSLOCK : out std_logic;
      C405PLBDCUU0ATTR : out std_logic;
      C405PLBDCUGUARDED : out std_logic;
      C405PLBDCULOCKERR : out std_logic;
      C405PLBDCUMSIZE : out std_logic_vector(0 to 1);
      C405PLBDCUORDERED : out std_logic;
      C405PLBDCUPRIORITY : out std_logic_vector(0 to 1);
      C405PLBDCURDBURST : out std_logic;
      C405PLBDCUREQUEST : out std_logic;
      C405PLBDCUSIZE : out std_logic_vector(0 to 3);
      C405PLBDCUTYPE : out std_logic_vector(0 to 2);
      C405PLBDCUWRBURST : out std_logic;
      C405PLBDCUWRDBUS : out std_logic_vector(0 to 63);
      C405PLBDCUCACHEABLE : out std_logic;
      C405PLBDCUWRITETHRU : out std_logic;
      PLBC405DCUADDRACK : in std_logic;
      PLBC405DCUBUSY : in std_logic;
      PLBC405DCUERR : in std_logic;
      PLBC405DCURDBTERM : in std_logic;
      PLBC405DCURDDACK : in std_logic;
      PLBC405DCURDDBUS : in std_logic_vector(0 to 63);
      PLBC405DCURDWDADDR : in std_logic_vector(0 to 3);
      PLBC405DCUREARBITRATE : in std_logic;
      PLBC405DCUWRBTERM : in std_logic;
      PLBC405DCUWRDACK : in std_logic;
      PLBC405DCUSSIZE : in std_logic_vector(0 to 1);
      PLBC405DCUSERR : in std_logic;
      PLBC405DCUSBUSYS : in std_logic;
      BRAMDSOCMCLK : in std_logic;
      BRAMDSOCMRDDBUS : in std_logic_vector(0 to 31);
      DSARCVALUE : in std_logic_vector(0 to 7);
      DSCNTLVALUE : in std_logic_vector(0 to 7);
      DSOCMBRAMABUS : out std_logic_vector(8 to 29);
      DSOCMBRAMBYTEWRITE : out std_logic_vector(0 to 3);
      DSOCMBRAMEN : out std_logic;
      DSOCMBRAMWRDBUS : out std_logic_vector(0 to 31);
      DSOCMBUSY : out std_logic;
      BRAMISOCMCLK : in std_logic;
      BRAMISOCMRDDBUS : in std_logic_vector(0 to 63);
      ISARCVALUE : in std_logic_vector(0 to 7);
      ISCNTLVALUE : in std_logic_vector(0 to 7);
      ISOCMBRAMEN : out std_logic;
      ISOCMBRAMEVENWRITEEN : out std_logic;
      ISOCMBRAMODDWRITEEN : out std_logic;
      ISOCMBRAMRDABUS : out std_logic_vector(8 to 28);
      ISOCMBRAMWRABUS : out std_logic_vector(8 to 28);
      ISOCMBRAMWRDBUS : out std_logic_vector(0 to 31);
      C405DCRABUS : out std_logic_vector(0 to 9);
      C405DCRDBUSOUT : out std_logic_vector(0 to 31);
      C405DCRREAD : out std_logic;
      C405DCRWRITE : out std_logic;
      DCRC405ACK : in std_logic;
      DCRC405DBUSIN : in std_logic_vector(0 to 31);
      EICC405CRITINPUTIRQ : in std_logic;
      EICC405EXTINPUTIRQ : in std_logic;
      C405JTGCAPTUREDR : out std_logic;
      C405JTGEXTEST : out std_logic;
      C405JTGPGMOUT : out std_logic;
      C405JTGSHIFTDR : out std_logic;
      C405JTGTDO : out std_logic;
      C405JTGTDOEN : out std_logic;
      C405JTGUPDATEDR : out std_logic;
      MCBJTAGEN : in std_logic;
      JTGC405BNDSCANTDO : in std_logic;
      JTGC405TCK : in std_logic;
      JTGC405TDI : in std_logic;
      JTGC405TMS : in std_logic;
      JTGC405TRSTNEG : in std_logic;
      C405DBGMSRWE : out std_logic;
      C405DBGSTOPACK : out std_logic;
      C405DBGWBCOMPLETE : out std_logic;
      C405DBGWBFULL : out std_logic;
      C405DBGWBIAR : out std_logic_vector(0 to 29);
      DBGC405DEBUGHALT : in std_logic;
      DBGC405EXTBUSHOLDACK : in std_logic;
      DBGC405UNCONDDEBUGEVENT : in std_logic;
      C405TRCCYCLE : out std_logic;
      C405TRCEVENEXECUTIONSTATUS : out std_logic_vector(0 to 1);
      C405TRCODDEXECUTIONSTATUS : out std_logic_vector(0 to 1);
      C405TRCTRACESTATUS : out std_logic_vector(0 to 3);
      C405TRCTRIGGEREVENTOUT : out std_logic;
      C405TRCTRIGGEREVENTTYPE : out std_logic_vector(0 to 10);
      TRCC405TRACEDISABLE : in std_logic;
      TRCC405TRIGGEREVENTIN : in std_logic
    );
  end component;

  attribute box_type of ppc405_1_wrapper: component is "black_box";

  component jtagppc_0_wrapper is
    port (
      TRSTNEG : in std_logic;
      HALTNEG0 : in std_logic;
      DBGC405DEBUGHALT0 : out std_logic;
      HALTNEG1 : in std_logic;
      DBGC405DEBUGHALT1 : out std_logic;
      C405JTGTDO0 : in std_logic;
      C405JTGTDOEN0 : in std_logic;
      JTGC405TCK0 : out std_logic;
      JTGC405TDI0 : out std_logic;
      JTGC405TMS0 : out std_logic;
      JTGC405TRSTNEG0 : out std_logic;
      C405JTGTDO1 : in std_logic;
      C405JTGTDOEN1 : in std_logic;
      JTGC405TCK1 : out std_logic;
      JTGC405TDI1 : out std_logic;
      JTGC405TMS1 : out std_logic;
      JTGC405TRSTNEG1 : out std_logic
    );
  end component;

  attribute box_type of jtagppc_0_wrapper: component is "black_box";

  component reset_block_wrapper is
    port (
      Slowest_sync_clk : in std_logic;
      Ext_Reset_In : in std_logic;
      Aux_Reset_In : in std_logic;
      Core_Reset_Req : in std_logic;
      Chip_Reset_Req : in std_logic;
      System_Reset_Req : in std_logic;
      Dcm_locked : in std_logic;
      Rstc405resetcore : out std_logic;
      Rstc405resetchip : out std_logic;
      Rstc405resetsys : out std_logic;
      Bus_Struct_Reset : out std_logic_vector(0 to 0);
      Peripheral_Reset : out std_logic_vector(0 to 0)
    );
  end component;

  attribute box_type of reset_block_wrapper: component is "black_box";

  component iocm_wrapper is
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
  end component;

  attribute box_type of iocm_wrapper: component is "black_box";

  component iocm_cntlr_wrapper is
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

  attribute box_type of iocm_cntlr_wrapper: component is "black_box";

  component isocm_bram_wrapper is
    port (
      BRAM_Rst_A : in std_logic;
      BRAM_Clk_A : in std_logic;
      BRAM_EN_A : in std_logic;
      BRAM_WEN_A : in std_logic_vector(0 to 1);
      BRAM_Addr_A : in std_logic_vector(0 to 31);
      BRAM_Din_A : out std_logic_vector(0 to 63);
      BRAM_Dout_A : in std_logic_vector(0 to 63);
      BRAM_Rst_B : in std_logic;
      BRAM_Clk_B : in std_logic;
      BRAM_EN_B : in std_logic;
      BRAM_WEN_B : in std_logic_vector(0 to 1);
      BRAM_Addr_B : in std_logic_vector(0 to 31);
      BRAM_Din_B : out std_logic_vector(0 to 63);
      BRAM_Dout_B : in std_logic_vector(0 to 63)
    );
  end component;

  attribute box_type of isocm_bram_wrapper: component is "black_box";

  component docm_wrapper is
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
  end component;

  attribute box_type of docm_wrapper: component is "black_box";

  component docm_cntlr_wrapper is
    port (
      BRAMDSOCMCLK : in std_logic;
      DSBRAMRST : in std_logic;
      DSOCMBRAMABUS : in std_logic_vector(8 to 29);
      DSOCMBRAMBYTEWRITE : in std_logic_vector(0 to 3);
      DSOCMBRAMEN : in std_logic;
      DSOCMBRAMWRDBUS : in std_logic_vector(0 to 31);
      S_BRAMDSOCMRDDBUS : out std_logic_vector(0 to 31);
      S_DSOCMRWCOMPLETE : out std_logic;
      S_DSOCMSLAVESELECT : out std_logic;
      BRAM_Rst_A : out std_logic;
      BRAM_Clk_A : out std_logic;
      BRAM_EN_A : out std_logic;
      BRAM_WEN_A : out std_logic_vector(0 to 3);
      BRAM_Addr_A : out std_logic_vector(0 to 31);
      BRAM_Din_A : in std_logic_vector(0 to 31);
      BRAM_Dout_A : out std_logic_vector(0 to 31)
    );
  end component;

  attribute box_type of docm_cntlr_wrapper: component is "black_box";

  component dsocm_bram_wrapper is
    port (
      BRAM_Rst_A : in std_logic;
      BRAM_Clk_A : in std_logic;
      BRAM_EN_A : in std_logic;
      BRAM_WEN_A : in std_logic_vector(0 to 3);
      BRAM_Addr_A : in std_logic_vector(0 to 31);
      BRAM_Din_A : out std_logic_vector(0 to 31);
      BRAM_Dout_A : in std_logic_vector(0 to 31);
      BRAM_Rst_B : in std_logic;
      BRAM_Clk_B : in std_logic;
      BRAM_EN_B : in std_logic;
      BRAM_WEN_B : in std_logic_vector(0 to 3);
      BRAM_Addr_B : in std_logic_vector(0 to 31);
      BRAM_Din_B : out std_logic_vector(0 to 31);
      BRAM_Dout_B : in std_logic_vector(0 to 31)
    );
  end component;

  attribute box_type of dsocm_bram_wrapper: component is "black_box";

  component plb_wrapper is
    port (
      PLB_Clk : in std_logic;
      SYS_Rst : in std_logic;
      PLB_Rst : out std_logic;
      PLB_dcrAck : out std_logic;
      PLB_dcrDBus : out std_logic_vector(0 to 31);
      DCR_ABus : in std_logic_vector(0 to 9);
      DCR_DBus : in std_logic_vector(0 to 31);
      DCR_Read : in std_logic;
      DCR_Write : in std_logic;
      M_ABus : in std_logic_vector(0 to 63);
      M_BE : in std_logic_vector(0 to 15);
      M_RNW : in std_logic_vector(0 to 1);
      M_abort : in std_logic_vector(0 to 1);
      M_busLock : in std_logic_vector(0 to 1);
      M_compress : in std_logic_vector(0 to 1);
      M_guarded : in std_logic_vector(0 to 1);
      M_lockErr : in std_logic_vector(0 to 1);
      M_MSize : in std_logic_vector(0 to 3);
      M_ordered : in std_logic_vector(0 to 1);
      M_priority : in std_logic_vector(0 to 3);
      M_rdBurst : in std_logic_vector(0 to 1);
      M_request : in std_logic_vector(0 to 1);
      M_size : in std_logic_vector(0 to 7);
      M_type : in std_logic_vector(0 to 5);
      M_wrBurst : in std_logic_vector(0 to 1);
      M_wrDBus : in std_logic_vector(0 to 127);
      Sl_addrAck : in std_logic_vector(0 to 2);
      Sl_MErr : in std_logic_vector(0 to 5);
      Sl_MBusy : in std_logic_vector(0 to 5);
      Sl_rdBTerm : in std_logic_vector(0 to 2);
      Sl_rdComp : in std_logic_vector(0 to 2);
      Sl_rdDAck : in std_logic_vector(0 to 2);
      Sl_rdDBus : in std_logic_vector(0 to 191);
      Sl_rdWdAddr : in std_logic_vector(0 to 11);
      Sl_rearbitrate : in std_logic_vector(0 to 2);
      Sl_SSize : in std_logic_vector(0 to 5);
      Sl_wait : in std_logic_vector(0 to 2);
      Sl_wrBTerm : in std_logic_vector(0 to 2);
      Sl_wrComp : in std_logic_vector(0 to 2);
      Sl_wrDAck : in std_logic_vector(0 to 2);
      PLB_ABus : out std_logic_vector(0 to 31);
      PLB_BE : out std_logic_vector(0 to 7);
      PLB_MAddrAck : out std_logic_vector(0 to 1);
      PLB_MBusy : out std_logic_vector(0 to 1);
      PLB_MErr : out std_logic_vector(0 to 1);
      PLB_MRdBTerm : out std_logic_vector(0 to 1);
      PLB_MRdDAck : out std_logic_vector(0 to 1);
      PLB_MRdDBus : out std_logic_vector(0 to 127);
      PLB_MRdWdAddr : out std_logic_vector(0 to 7);
      PLB_MRearbitrate : out std_logic_vector(0 to 1);
      PLB_MWrBTerm : out std_logic_vector(0 to 1);
      PLB_MWrDAck : out std_logic_vector(0 to 1);
      PLB_MSSize : out std_logic_vector(0 to 3);
      PLB_PAValid : out std_logic;
      PLB_RNW : out std_logic;
      PLB_SAValid : out std_logic;
      PLB_abort : out std_logic;
      PLB_busLock : out std_logic;
      PLB_compress : out std_logic;
      PLB_guarded : out std_logic;
      PLB_lockErr : out std_logic;
      PLB_masterID : out std_logic_vector(0 to 0);
      PLB_MSize : out std_logic_vector(0 to 1);
      PLB_ordered : out std_logic;
      PLB_pendPri : out std_logic_vector(0 to 1);
      PLB_pendReq : out std_logic;
      PLB_rdBurst : out std_logic;
      PLB_rdPrim : out std_logic;
      PLB_reqPri : out std_logic_vector(0 to 1);
      PLB_size : out std_logic_vector(0 to 3);
      PLB_type : out std_logic_vector(0 to 2);
      PLB_wrBurst : out std_logic;
      PLB_wrDBus : out std_logic_vector(0 to 63);
      PLB_wrPrim : out std_logic;
      PLB_SaddrAck : out std_logic;
      PLB_SMErr : out std_logic_vector(0 to 1);
      PLB_SMBusy : out std_logic_vector(0 to 1);
      PLB_SrdBTerm : out std_logic;
      PLB_SrdComp : out std_logic;
      PLB_SrdDAck : out std_logic;
      PLB_SrdDBus : out std_logic_vector(0 to 63);
      PLB_SrdWdAddr : out std_logic_vector(0 to 3);
      PLB_Srearbitrate : out std_logic;
      PLB_Sssize : out std_logic_vector(0 to 1);
      PLB_Swait : out std_logic;
      PLB_SwrBTerm : out std_logic;
      PLB_SwrComp : out std_logic;
      PLB_SwrDAck : out std_logic;
      PLB2OPB_rearb : in std_logic_vector(0 to 2);
      ArbAddrVldReg : out std_logic;
      Bus_Error_Det : out std_logic
    );
  end component;

  attribute box_type of plb_wrapper: component is "black_box";

  component generic_external_memory_wrapper is
    port (
      PLB_Clk : in std_logic;
      PLB_Rst : in std_logic;
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
      PLB_RNW : in std_logic;
      PLB_size : in std_logic_vector(0 to 3);
      PLB_type : in std_logic_vector(0 to 2);
      Sl_addrAck : out std_logic;
      Sl_MBusy : out std_logic_vector(0 to 1);
      Sl_MErr : out std_logic_vector(0 to 1);
      Sl_rearbitrate : out std_logic;
      Sl_SSize : out std_logic_vector(0 to 1);
      Sl_wait : out std_logic;
      PLB_rdPrim : in std_logic;
      PLB_SAValid : in std_logic;
      PLB_wrPrim : in std_logic;
      PLB_wrBurst : in std_logic;
      PLB_wrDBus : in std_logic_vector(0 to 63);
      Sl_wrBTerm : out std_logic;
      Sl_wrComp : out std_logic;
      Sl_wrDAck : out std_logic;
      PLB_rdBurst : in std_logic;
      Sl_rdBTerm : out std_logic;
      Sl_rdComp : out std_logic;
      Sl_rdDAck : out std_logic;
      Sl_rdDBus : out std_logic_vector(0 to 63);
      Sl_rdWdAddr : out std_logic_vector(0 to 3);
      PLB_pendReq : in std_logic;
      PLB_pendPri : in std_logic_vector(0 to 1);
      PLB_reqPri : in std_logic_vector(0 to 1);
      Mem_A : out std_logic_vector(0 to 31);
      Mem_CEN : out std_logic_vector(0 to 0);
      Mem_OEN : out std_logic_vector(0 to 0);
      Mem_WEN : out std_logic;
      Mem_QWEN : out std_logic_vector(0 to 1);
      Mem_BEN : out std_logic_vector(0 to 1);
      Mem_RPN : out std_logic;
      Mem_CE : out std_logic_vector(0 to 0);
      Mem_ADV_LDN : out std_logic;
      Mem_LBON : out std_logic;
      Mem_CKEN : out std_logic;
      Mem_RNW : out std_logic;
      Mem_DQ_I : in std_logic_vector(0 to 15);
      Mem_DQ_O : out std_logic_vector(0 to 15);
      Mem_DQ_T : out std_logic_vector(0 to 15)
    );
  end component;

  attribute box_type of generic_external_memory_wrapper: component is "black_box";

  component opb_wrapper is
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
  end component;

  attribute box_type of opb_wrapper: component is "black_box";

  component plb2opb_wrapper is
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
  end component;

  attribute box_type of plb2opb_wrapper: component is "black_box";

  component generic_gpio_wrapper is
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
  end component;

  attribute box_type of generic_gpio_wrapper: component is "black_box";

  component leds_wrapper is
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
      GPIO_in : in std_logic_vector(0 to 31);
      GPIO_d_out : out std_logic_vector(0 to 31);
      GPIO_t_out : out std_logic_vector(0 to 31);
      GPIO2_in : in std_logic_vector(0 to 31);
      GPIO2_d_out : out std_logic_vector(0 to 31);
      GPIO2_t_out : out std_logic_vector(0 to 31);
      GPIO_IO_I : in std_logic_vector(0 to 31);
      GPIO_IO_O : out std_logic_vector(0 to 31);
      GPIO_IO_T : out std_logic_vector(0 to 31);
      GPIO2_IO_I : in std_logic_vector(0 to 31);
      GPIO2_IO_O : out std_logic_vector(0 to 31);
      GPIO2_IO_T : out std_logic_vector(0 to 31)
    );
  end component;

  attribute box_type of leds_wrapper: component is "black_box";

  component rs232_wrapper is
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
  end component;

  attribute box_type of rs232_wrapper: component is "black_box";

  component plb_bram_if_cntlr_1_wrapper is
    port (
      plb_clk : in std_logic;
      plb_rst : in std_logic;
      plb_abort : in std_logic;
      plb_abus : in std_logic_vector(0 to 31);
      plb_be : in std_logic_vector(0 to 7);
      plb_buslock : in std_logic;
      plb_compress : in std_logic;
      plb_guarded : in std_logic;
      plb_lockerr : in std_logic;
      plb_masterid : in std_logic_vector(0 to 0);
      plb_msize : in std_logic_vector(0 to 1);
      plb_ordered : in std_logic;
      plb_pavalid : in std_logic;
      plb_rnw : in std_logic;
      plb_size : in std_logic_vector(0 to 3);
      plb_type : in std_logic_vector(0 to 2);
      sl_addrack : out std_logic;
      sl_mbusy : out std_logic_vector(0 to 1);
      sl_merr : out std_logic_vector(0 to 1);
      sl_rearbitrate : out std_logic;
      sl_ssize : out std_logic_vector(0 to 1);
      sl_wait : out std_logic;
      plb_rdprim : in std_logic;
      plb_savalid : in std_logic;
      plb_wrprim : in std_logic;
      plb_wrburst : in std_logic;
      plb_wrdbus : in std_logic_vector(0 to 63);
      sl_wrbterm : out std_logic;
      sl_wrcomp : out std_logic;
      sl_wrdack : out std_logic;
      plb_rdburst : in std_logic;
      sl_rdbterm : out std_logic;
      sl_rdcomp : out std_logic;
      sl_rddack : out std_logic;
      sl_rddbus : out std_logic_vector(0 to 63);
      sl_rdwdaddr : out std_logic_vector(0 to 3);
      plb_pendreq : in std_logic;
      plb_pendpri : in std_logic_vector(0 to 1);
      plb_reqpri : in std_logic_vector(0 to 1);
      bram_rst : out std_logic;
      bram_clk : out std_logic;
      bram_en : out std_logic;
      bram_wen : out std_logic_vector(0 to 7);
      bram_addr : out std_logic_vector(0 to 31);
      bram_din : in std_logic_vector(0 to 63);
      bram_dout : out std_logic_vector(0 to 63)
    );
  end component;

  attribute box_type of plb_bram_if_cntlr_1_wrapper: component is "black_box";

  component plb_bram_if_cntlr_1_bram_wrapper is
    port (
      BRAM_Rst_A : in std_logic;
      BRAM_Clk_A : in std_logic;
      BRAM_EN_A : in std_logic;
      BRAM_WEN_A : in std_logic_vector(0 to 7);
      BRAM_Addr_A : in std_logic_vector(0 to 31);
      BRAM_Din_A : out std_logic_vector(0 to 63);
      BRAM_Dout_A : in std_logic_vector(0 to 63);
      BRAM_Rst_B : in std_logic;
      BRAM_Clk_B : in std_logic;
      BRAM_EN_B : in std_logic;
      BRAM_WEN_B : in std_logic_vector(0 to 7);
      BRAM_Addr_B : in std_logic_vector(0 to 31);
      BRAM_Din_B : out std_logic_vector(0 to 63);
      BRAM_Dout_B : in std_logic_vector(0 to 63)
    );
  end component;

  attribute box_type of plb_bram_if_cntlr_1_bram_wrapper: component is "black_box";

  component dcm_0_wrapper is
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

  attribute box_type of dcm_0_wrapper: component is "black_box";

  component util_bus_split_0_wrapper is
    port (
      Sig : in std_logic_vector(0 to 31);
      Out1 : out std_logic_vector(7 to 30);
      Out2 : out std_logic_vector(31 to 31)
    );
  end component;

  attribute box_type of util_bus_split_0_wrapper: component is "black_box";

  component OBUF is
    port (
      I : in std_logic;
      O : out std_logic
    );
  end component;

  component IBUF is
    port (
      I : in std_logic;
      O : out std_logic
    );
  end component;

  component IOBUF is
    port (
      I : in std_logic;
      IO : inout std_logic;
      O : out std_logic;
      T : in std_logic
    );
  end component;

  component IBUFG is
    port (
      I : in std_logic;
      O : out std_logic
    );
  end component;

  -- Internal signals

  signal C405RSTCHIPRESETREQ : std_logic;
  signal C405RSTCORERESETREQ : std_logic;
  signal C405RSTSYSRESETREQ : std_logic;
  signal RSTC405RESETCHIP : std_logic;
  signal RSTC405RESETCORE : std_logic;
  signal RSTC405RESETSYS : std_logic;
  signal dcm_0_lock : std_logic;
  signal dcm_clk_s : std_logic;
  signal docm_BRAMDSOCMRDDBUS : std_logic_vector(0 to 31);
  signal docm_DSARCVALUE : std_logic_vector(0 to 7);
  signal docm_DSCNTLVALUE : std_logic_vector(0 to 7);
  signal docm_DSOCMBRAMABUS : std_logic_vector(8 to 29);
  signal docm_DSOCMBRAMBYTEWRITE : std_logic_vector(0 to 3);
  signal docm_DSOCMBRAMEN : std_logic;
  signal docm_DSOCMBRAMWRDBUS : std_logic_vector(0 to 31);
  signal docm_DSOCM_Rst : std_logic;
  signal docm_M_DSOCMBRAMABUS : std_logic_vector(8 to 29);
  signal docm_M_DSOCMBRAMBYTEWRITE : std_logic_vector(0 to 3);
  signal docm_M_DSOCMBRAMEN : std_logic;
  signal docm_M_DSOCMBRAMWRDBUS : std_logic_vector(0 to 31);
  signal docm_M_DSOCMBUSY : std_logic;
  signal docm_S_BRAMDSOCMRDDBUS : std_logic_vector(0 to 31);
  signal docm_S_DSOCMRWCOMPLETE : std_logic_vector(0 to 0);
  signal docm_S_DSOCMSLAVESELECT : std_logic_vector(0 to 0);
  signal dsocm_porta_BRAM_Addr : std_logic_vector(0 to 31);
  signal dsocm_porta_BRAM_Clk : std_logic;
  signal dsocm_porta_BRAM_Din : std_logic_vector(0 to 31);
  signal dsocm_porta_BRAM_Dout : std_logic_vector(0 to 31);
  signal dsocm_porta_BRAM_EN : std_logic;
  signal dsocm_porta_BRAM_Rst : std_logic;
  signal dsocm_porta_BRAM_WEN : std_logic_vector(0 to 3);
  signal fpga_0_Generic_External_Memory_Mem_A : std_logic_vector(7 to 30);
  signal fpga_0_Generic_External_Memory_Mem_A_split : std_logic_vector(0 to 31);
  signal fpga_0_Generic_External_Memory_Mem_BEN : std_logic_vector(0 to 1);
  signal fpga_0_Generic_External_Memory_Mem_CEN : std_logic_vector(0 to 0);
  signal fpga_0_Generic_External_Memory_Mem_DQ_I : std_logic_vector(0 to 15);
  signal fpga_0_Generic_External_Memory_Mem_DQ_O : std_logic_vector(0 to 15);
  signal fpga_0_Generic_External_Memory_Mem_DQ_T : std_logic_vector(0 to 15);
  signal fpga_0_Generic_External_Memory_Mem_OEN : std_logic_vector(0 to 0);
  signal fpga_0_Generic_External_Memory_Mem_RPN : std_logic;
  signal fpga_0_Generic_External_Memory_Mem_WEN : std_logic;
  signal fpga_0_Generic_GPIO_GPIO_IO_I : std_logic_vector(0 to 1);
  signal fpga_0_Generic_GPIO_GPIO_IO_O : std_logic_vector(0 to 1);
  signal fpga_0_Generic_GPIO_GPIO_IO_T : std_logic_vector(0 to 1);
  signal fpga_0_Generic_GPIO_GPIO_d_out : std_logic_vector(0 to 1);
  signal fpga_0_Generic_GPIO_GPIO_in : std_logic_vector(0 to 1);
  signal fpga_0_Generic_GPIO_GPIO_t_out : std_logic_vector(0 to 1);
  signal fpga_0_LEDS_GPIO_d_out : std_logic_vector(0 to 31);
  signal fpga_0_RS232_RX : std_logic;
  signal fpga_0_RS232_TX : std_logic;
  signal iocm_BRAMISOCMRDDBUS : std_logic_vector(0 to 63);
  signal iocm_ISARCVALUE : std_logic_vector(0 to 7);
  signal iocm_ISCNTLVALUE : std_logic_vector(0 to 7);
  signal iocm_ISOCMBRAMEN : std_logic;
  signal iocm_ISOCMBRAMEVENWRITEEN : std_logic;
  signal iocm_ISOCMBRAMODDWRITEEN : std_logic;
  signal iocm_ISOCMBRAMRDABUS : std_logic_vector(8 to 28);
  signal iocm_ISOCMBRAMWRABUS : std_logic_vector(8 to 28);
  signal iocm_ISOCMBRAMWRDBUS : std_logic_vector(0 to 31);
  signal iocm_ISOCMDCRBRAMEVENEN : std_logic;
  signal iocm_ISOCMDCRBRAMODDEN : std_logic;
  signal iocm_ISOCMDCRBRAMRDSELECT : std_logic;
  signal iocm_ISOCM_Rst : std_logic;
  signal iocm_M_ISOCMBRAMEN : std_logic;
  signal iocm_M_ISOCMBRAMEVENWRITEEN : std_logic;
  signal iocm_M_ISOCMBRAMODDWRITEEN : std_logic;
  signal iocm_M_ISOCMBRAMRDABUS : std_logic_vector(8 to 28);
  signal iocm_M_ISOCMBRAMWRABUS : std_logic_vector(8 to 28);
  signal iocm_M_ISOCMBRAMWRDBUS : std_logic_vector(0 to 31);
  signal iocm_S_BRAMISOCMDCRRDDBUS : std_logic_vector(0 to 31);
  signal iocm_S_BRAMISOCMRDDBUS : std_logic_vector(0 to 63);
  signal iocm_S_ISOCMDCRSLAVESELECT : std_logic_vector(0 to 0);
  signal iocm_S_ISOCMSLAVESELECT : std_logic_vector(0 to 0);
  signal isocm_porta_BRAM_Addr : std_logic_vector(0 to 31);
  signal isocm_porta_BRAM_Clk : std_logic;
  signal isocm_porta_BRAM_Din : std_logic_vector(0 to 63);
  signal isocm_porta_BRAM_Dout : std_logic_vector(0 to 63);
  signal isocm_porta_BRAM_EN : std_logic;
  signal isocm_porta_BRAM_Rst : std_logic;
  signal isocm_porta_BRAM_WEN : std_logic_vector(0 to 1);
  signal isocm_portb_BRAM_Addr : std_logic_vector(0 to 31);
  signal isocm_portb_BRAM_Clk : std_logic;
  signal isocm_portb_BRAM_Din : std_logic_vector(0 to 63);
  signal isocm_portb_BRAM_Dout : std_logic_vector(0 to 63);
  signal isocm_portb_BRAM_EN : std_logic;
  signal isocm_portb_BRAM_Rst : std_logic;
  signal isocm_portb_BRAM_WEN : std_logic_vector(0 to 1);
  signal jtagppc_0_0_C405JTGTDO : std_logic;
  signal jtagppc_0_0_C405JTGTDOEN : std_logic;
  signal jtagppc_0_0_JTGC405TCK : std_logic;
  signal jtagppc_0_0_JTGC405TDI : std_logic;
  signal jtagppc_0_0_JTGC405TMS : std_logic;
  signal jtagppc_0_0_JTGC405TRSTNEG : std_logic;
  signal jtagppc_0_1_C405JTGTDO : std_logic;
  signal jtagppc_0_1_C405JTGTDOEN : std_logic;
  signal jtagppc_0_1_JTGC405TCK : std_logic;
  signal jtagppc_0_1_JTGC405TDI : std_logic;
  signal jtagppc_0_1_JTGC405TMS : std_logic;
  signal jtagppc_0_1_JTGC405TRSTNEG : std_logic;
  signal net_gnd0 : std_logic;
  signal net_gnd1 : std_logic_vector(0 to 0);
  signal net_gnd10 : std_logic_vector(0 to 9);
  signal net_gnd2 : std_logic_vector(0 to 1);
  signal net_gnd3 : std_logic_vector(0 to 2);
  signal net_gnd32 : std_logic_vector(0 to 31);
  signal net_gnd4 : std_logic_vector(0 to 3);
  signal net_gnd64 : std_logic_vector(0 to 63);
  signal net_gnd8 : std_logic_vector(0 to 7);
  signal net_vcc0 : std_logic;
  signal net_vcc1 : std_logic_vector(0 to 0);
  signal net_vcc3 : std_logic_vector(0 to 2);
  signal opb_M_ABus : std_logic_vector(0 to 31);
  signal opb_M_BE : std_logic_vector(0 to 3);
  signal opb_M_DBus : std_logic_vector(0 to 31);
  signal opb_M_RNW : std_logic_vector(0 to 0);
  signal opb_M_busLock : std_logic_vector(0 to 0);
  signal opb_M_request : std_logic_vector(0 to 0);
  signal opb_M_select : std_logic_vector(0 to 0);
  signal opb_M_seqAddr : std_logic_vector(0 to 0);
  signal opb_OPB_ABus : std_logic_vector(0 to 31);
  signal opb_OPB_BE : std_logic_vector(0 to 3);
  signal opb_OPB_DBus : std_logic_vector(0 to 31);
  signal opb_OPB_MGrant : std_logic_vector(0 to 0);
  signal opb_OPB_RNW : std_logic;
  signal opb_OPB_Rst : std_logic;
  signal opb_OPB_errAck : std_logic;
  signal opb_OPB_retry : std_logic;
  signal opb_OPB_select : std_logic;
  signal opb_OPB_seqAddr : std_logic;
  signal opb_OPB_timeout : std_logic;
  signal opb_OPB_xferAck : std_logic;
  signal opb_Sl_DBus : std_logic_vector(0 to 95);
  signal opb_Sl_errAck : std_logic_vector(0 to 2);
  signal opb_Sl_retry : std_logic_vector(0 to 2);
  signal opb_Sl_toutSup : std_logic_vector(0 to 2);
  signal opb_Sl_xferAck : std_logic_vector(0 to 2);
  signal plb_M_ABus : std_logic_vector(0 to 63);
  signal plb_M_BE : std_logic_vector(0 to 15);
  signal plb_M_MSize : std_logic_vector(0 to 3);
  signal plb_M_RNW : std_logic_vector(0 to 1);
  signal plb_M_abort : std_logic_vector(0 to 1);
  signal plb_M_busLock : std_logic_vector(0 to 1);
  signal plb_M_compress : std_logic_vector(0 to 1);
  signal plb_M_guarded : std_logic_vector(0 to 1);
  signal plb_M_lockErr : std_logic_vector(0 to 1);
  signal plb_M_ordered : std_logic_vector(0 to 1);
  signal plb_M_priority : std_logic_vector(0 to 3);
  signal plb_M_rdBurst : std_logic_vector(0 to 1);
  signal plb_M_request : std_logic_vector(0 to 1);
  signal plb_M_size : std_logic_vector(0 to 7);
  signal plb_M_type : std_logic_vector(0 to 5);
  signal plb_M_wrBurst : std_logic_vector(0 to 1);
  signal plb_M_wrDBus : std_logic_vector(0 to 127);
  signal plb_PLB2OPB_rearb : std_logic_vector(0 to 2);
  signal plb_PLB_ABus : std_logic_vector(0 to 31);
  signal plb_PLB_BE : std_logic_vector(0 to 7);
  signal plb_PLB_MAddrAck : std_logic_vector(0 to 1);
  signal plb_PLB_MBusy : std_logic_vector(0 to 1);
  signal plb_PLB_MErr : std_logic_vector(0 to 1);
  signal plb_PLB_MRdBTerm : std_logic_vector(0 to 1);
  signal plb_PLB_MRdDAck : std_logic_vector(0 to 1);
  signal plb_PLB_MRdDBus : std_logic_vector(0 to 127);
  signal plb_PLB_MRdWdAddr : std_logic_vector(0 to 7);
  signal plb_PLB_MRearbitrate : std_logic_vector(0 to 1);
  signal plb_PLB_MSSize : std_logic_vector(0 to 3);
  signal plb_PLB_MSize : std_logic_vector(0 to 1);
  signal plb_PLB_MWrBTerm : std_logic_vector(0 to 1);
  signal plb_PLB_MWrDAck : std_logic_vector(0 to 1);
  signal plb_PLB_PAValid : std_logic;
  signal plb_PLB_RNW : std_logic;
  signal plb_PLB_Rst : std_logic;
  signal plb_PLB_SAValid : std_logic;
  signal plb_PLB_SMBusy : std_logic_vector(0 to 1);
  signal plb_PLB_SMErr : std_logic_vector(0 to 1);
  signal plb_PLB_abort : std_logic;
  signal plb_PLB_busLock : std_logic;
  signal plb_PLB_compress : std_logic;
  signal plb_PLB_guarded : std_logic;
  signal plb_PLB_lockErr : std_logic;
  signal plb_PLB_masterID : std_logic_vector(0 to 0);
  signal plb_PLB_ordered : std_logic;
  signal plb_PLB_pendPri : std_logic_vector(0 to 1);
  signal plb_PLB_pendReq : std_logic;
  signal plb_PLB_rdBurst : std_logic;
  signal plb_PLB_rdPrim : std_logic;
  signal plb_PLB_reqPri : std_logic_vector(0 to 1);
  signal plb_PLB_size : std_logic_vector(0 to 3);
  signal plb_PLB_type : std_logic_vector(0 to 2);
  signal plb_PLB_wrBurst : std_logic;
  signal plb_PLB_wrDBus : std_logic_vector(0 to 63);
  signal plb_PLB_wrPrim : std_logic;
  signal plb_Sl_MBusy : std_logic_vector(0 to 5);
  signal plb_Sl_MErr : std_logic_vector(0 to 5);
  signal plb_Sl_SSize : std_logic_vector(0 to 5);
  signal plb_Sl_addrAck : std_logic_vector(0 to 2);
  signal plb_Sl_rdBTerm : std_logic_vector(0 to 2);
  signal plb_Sl_rdComp : std_logic_vector(0 to 2);
  signal plb_Sl_rdDAck : std_logic_vector(0 to 2);
  signal plb_Sl_rdDBus : std_logic_vector(0 to 191);
  signal plb_Sl_rdWdAddr : std_logic_vector(0 to 11);
  signal plb_Sl_rearbitrate : std_logic_vector(0 to 2);
  signal plb_Sl_wait : std_logic_vector(0 to 2);
  signal plb_Sl_wrBTerm : std_logic_vector(0 to 2);
  signal plb_Sl_wrComp : std_logic_vector(0 to 2);
  signal plb_Sl_wrDAck : std_logic_vector(0 to 2);
  signal plb_bram_if_cntlr_1_port_BRAM_Addr : std_logic_vector(0 to 31);
  signal plb_bram_if_cntlr_1_port_BRAM_Clk : std_logic;
  signal plb_bram_if_cntlr_1_port_BRAM_Din : std_logic_vector(0 to 63);
  signal plb_bram_if_cntlr_1_port_BRAM_Dout : std_logic_vector(0 to 63);
  signal plb_bram_if_cntlr_1_port_BRAM_EN : std_logic;
  signal plb_bram_if_cntlr_1_port_BRAM_Rst : std_logic;
  signal plb_bram_if_cntlr_1_port_BRAM_WEN : std_logic_vector(0 to 7);
  signal proc_clk_s : std_logic;
  signal sys_bus_reset : std_logic_vector(0 to 0);
  signal sys_clk_s : std_logic;
  signal sys_rst_s : std_logic;
  signal ready_i, ready_o: std_logic_vector(0 to 1);
begin

  -- Internal assignments

  plb_PLB2OPB_rearb(0 to 0) <= B"0";
  plb_PLB2OPB_rearb(2 to 2) <= B"0";
  net_gnd0 <= '0';
  net_gnd1(0 to 0) <= B"0";
  net_gnd10(0 to 9) <= B"0000000000";
  net_gnd2(0 to 1) <= B"00";
  net_gnd3(0 to 2) <= B"000";
  net_gnd32(0 to 31) <= B"00000000000000000000000000000000";
  net_gnd4(0 to 3) <= B"0000";
  net_gnd64(0 to 63) <= B"0000000000000000000000000000000000000000000000000000000000000000";
  net_gnd8(0 to 7) <= B"00000000";
  net_vcc0 <= '1';
  net_vcc1(0 to 0) <= B"1";
  net_vcc3(0 to 2) <= B"111";

  ppc405_0 : ppc405_0_wrapper
    port map (
      C405CPMCORESLEEPREQ => open,
      C405CPMMSRCE => open,
      C405CPMMSREE => open,
      C405CPMTIMERIRQ => open,
      C405CPMTIMERRESETREQ => open,
      C405XXXMACHINECHECK => open,
      CPMC405CLOCK => proc_clk_s,
      CPMC405CORECLKINACTIVE => net_gnd0,
      CPMC405CPUCLKEN => net_vcc0,
      CPMC405JTAGCLKEN => net_vcc0,
      CPMC405TIMERCLKEN => net_vcc0,
      CPMC405TIMERTICK => net_vcc0,
      MCBCPUCLKEN => net_vcc0,
      MCBTIMEREN => net_vcc0,
      MCPPCRST => net_vcc0,
      PLBCLK => sys_clk_s,
      DCRCLK => net_gnd0,
      C405RSTCHIPRESETREQ => C405RSTCHIPRESETREQ,
      C405RSTCORERESETREQ => C405RSTCORERESETREQ,
      C405RSTSYSRESETREQ => C405RSTSYSRESETREQ,
      RSTC405RESETCHIP => RSTC405RESETCHIP,
      RSTC405RESETCORE => RSTC405RESETCORE,
      RSTC405RESETSYS => RSTC405RESETSYS,
      C405PLBICUABUS => plb_M_ABus(32 to 63),
      C405PLBICUBE => plb_M_BE(8 to 15),
      C405PLBICURNW => plb_M_RNW(1),
      C405PLBICUABORT => plb_M_abort(1),
      C405PLBICUBUSLOCK => plb_M_busLock(1),
      C405PLBICUU0ATTR => plb_M_compress(1),
      C405PLBICUGUARDED => plb_M_guarded(1),
      C405PLBICULOCKERR => plb_M_lockErr(1),
      C405PLBICUMSIZE => plb_M_MSize(2 to 3),
      C405PLBICUORDERED => plb_M_ordered(1),
      C405PLBICUPRIORITY => plb_M_priority(2 to 3),
      C405PLBICURDBURST => plb_M_rdBurst(1),
      C405PLBICUREQUEST => plb_M_request(1),
      C405PLBICUSIZE => plb_M_size(4 to 7),
      C405PLBICUTYPE => plb_M_type(3 to 5),
      C405PLBICUWRBURST => plb_M_wrBurst(1),
      C405PLBICUWRDBUS => plb_M_wrDBus(64 to 127),
      C405PLBICUCACHEABLE => open,
      PLBC405ICUADDRACK => plb_PLB_MAddrAck(1),
      PLBC405ICUBUSY => plb_PLB_MBusy(1),
      PLBC405ICUERR => plb_PLB_MErr(1),
      PLBC405ICURDBTERM => plb_PLB_MRdBTerm(1),
      PLBC405ICURDDACK => plb_PLB_MRdDAck(1),
      PLBC405ICURDDBUS => plb_PLB_MRdDBus(64 to 127),
      PLBC405ICURDWDADDR => plb_PLB_MRdWdAddr(4 to 7),
      PLBC405ICUREARBITRATE => plb_PLB_MRearbitrate(1),
      PLBC405ICUWRBTERM => plb_PLB_MWrBTerm(1),
      PLBC405ICUWRDACK => plb_PLB_MWrDAck(1),
      PLBC405ICUSSIZE => plb_PLB_MSSize(2 to 3),
      PLBC405ICUSERR => plb_PLB_SMErr(1),
      PLBC405ICUSBUSYS => plb_PLB_SMBusy(1),
      C405PLBDCUABUS => plb_M_ABus(0 to 31),
      C405PLBDCUBE => plb_M_BE(0 to 7),
      C405PLBDCURNW => plb_M_RNW(0),
      C405PLBDCUABORT => plb_M_abort(0),
      C405PLBDCUBUSLOCK => plb_M_busLock(0),
      C405PLBDCUU0ATTR => plb_M_compress(0),
      C405PLBDCUGUARDED => plb_M_guarded(0),
      C405PLBDCULOCKERR => plb_M_lockErr(0),
      C405PLBDCUMSIZE => plb_M_MSize(0 to 1),
      C405PLBDCUORDERED => plb_M_ordered(0),
      C405PLBDCUPRIORITY => plb_M_priority(0 to 1),
      C405PLBDCURDBURST => plb_M_rdBurst(0),
      C405PLBDCUREQUEST => plb_M_request(0),
      C405PLBDCUSIZE => plb_M_size(0 to 3),
      C405PLBDCUTYPE => plb_M_type(0 to 2),
      C405PLBDCUWRBURST => plb_M_wrBurst(0),
      C405PLBDCUWRDBUS => plb_M_wrDBus(0 to 63),
      C405PLBDCUCACHEABLE => open,
      C405PLBDCUWRITETHRU => open,
      PLBC405DCUADDRACK => plb_PLB_MAddrAck(0),
      PLBC405DCUBUSY => plb_PLB_MBusy(0),
      PLBC405DCUERR => plb_PLB_MErr(0),
      PLBC405DCURDBTERM => plb_PLB_MRdBTerm(0),
      PLBC405DCURDDACK => plb_PLB_MRdDAck(0),
      PLBC405DCURDDBUS => plb_PLB_MRdDBus(0 to 63),
      PLBC405DCURDWDADDR => plb_PLB_MRdWdAddr(0 to 3),
      PLBC405DCUREARBITRATE => plb_PLB_MRearbitrate(0),
      PLBC405DCUWRBTERM => plb_PLB_MWrBTerm(0),
      PLBC405DCUWRDACK => plb_PLB_MWrDAck(0),
      PLBC405DCUSSIZE => plb_PLB_MSSize(0 to 1),
      PLBC405DCUSERR => plb_PLB_SMErr(0),
      PLBC405DCUSBUSYS => plb_PLB_SMBusy(0),
      BRAMDSOCMCLK => sys_clk_s,
      BRAMDSOCMRDDBUS => docm_BRAMDSOCMRDDBUS,
      DSARCVALUE => docm_DSARCVALUE,
      DSCNTLVALUE => docm_DSCNTLVALUE,
      DSOCMBRAMABUS => docm_M_DSOCMBRAMABUS,
      DSOCMBRAMBYTEWRITE => docm_M_DSOCMBRAMBYTEWRITE,
      DSOCMBRAMEN => docm_M_DSOCMBRAMEN,
      DSOCMBRAMWRDBUS => docm_M_DSOCMBRAMWRDBUS,
      DSOCMBUSY => docm_M_DSOCMBUSY,
      BRAMISOCMCLK => sys_clk_s,
      BRAMISOCMRDDBUS => iocm_BRAMISOCMRDDBUS,
      ISARCVALUE => iocm_ISARCVALUE,
      ISCNTLVALUE => iocm_ISCNTLVALUE,
      ISOCMBRAMEN => iocm_M_ISOCMBRAMEN,
      ISOCMBRAMEVENWRITEEN => iocm_M_ISOCMBRAMEVENWRITEEN,
      ISOCMBRAMODDWRITEEN => iocm_M_ISOCMBRAMODDWRITEEN,
      ISOCMBRAMRDABUS => iocm_M_ISOCMBRAMRDABUS,
      ISOCMBRAMWRABUS => iocm_M_ISOCMBRAMWRABUS,
      ISOCMBRAMWRDBUS => iocm_M_ISOCMBRAMWRDBUS,
      C405DCRABUS => open,
      C405DCRDBUSOUT => open,
      C405DCRREAD => open,
      C405DCRWRITE => open,
      DCRC405ACK => net_gnd0,
      DCRC405DBUSIN => net_gnd32,
      EICC405CRITINPUTIRQ => net_gnd0,
      EICC405EXTINPUTIRQ => net_gnd0,
      C405JTGCAPTUREDR => open,
      C405JTGEXTEST => open,
      C405JTGPGMOUT => open,
      C405JTGSHIFTDR => open,
      C405JTGTDO => jtagppc_0_0_C405JTGTDO,
      C405JTGTDOEN => jtagppc_0_0_C405JTGTDOEN,
      C405JTGUPDATEDR => open,
      MCBJTAGEN => net_vcc0,
      JTGC405BNDSCANTDO => net_gnd0,
      JTGC405TCK => jtagppc_0_0_JTGC405TCK,
      JTGC405TDI => jtagppc_0_0_JTGC405TDI,
      JTGC405TMS => jtagppc_0_0_JTGC405TMS,
      JTGC405TRSTNEG => jtagppc_0_0_JTGC405TRSTNEG,
      C405DBGMSRWE => open,
      C405DBGSTOPACK => open,
      C405DBGWBCOMPLETE => open,
      C405DBGWBFULL => open,
      C405DBGWBIAR => open,
      DBGC405DEBUGHALT => net_gnd0,
      DBGC405EXTBUSHOLDACK => net_gnd0,
      DBGC405UNCONDDEBUGEVENT => net_gnd0,
      C405TRCCYCLE => open,
      C405TRCEVENEXECUTIONSTATUS => open,
      C405TRCODDEXECUTIONSTATUS => open,
      C405TRCTRACESTATUS => open,
      C405TRCTRIGGEREVENTOUT => open,
      C405TRCTRIGGEREVENTTYPE => open,
      TRCC405TRACEDISABLE => net_gnd0,
      TRCC405TRIGGEREVENTIN => net_gnd0
    );

  ppc405_1 : ppc405_1_wrapper
    port map (
      C405CPMCORESLEEPREQ => open,
      C405CPMMSRCE => open,
      C405CPMMSREE => open,
      C405CPMTIMERIRQ => open,
      C405CPMTIMERRESETREQ => open,
      C405XXXMACHINECHECK => open,
      CPMC405CLOCK => net_gnd0,
      CPMC405CORECLKINACTIVE => net_gnd0,
      CPMC405CPUCLKEN => net_vcc0,
      CPMC405JTAGCLKEN => net_vcc0,
      CPMC405TIMERCLKEN => net_vcc0,
      CPMC405TIMERTICK => net_vcc0,
      MCBCPUCLKEN => net_vcc0,
      MCBTIMEREN => net_vcc0,
      MCPPCRST => net_vcc0,
      PLBCLK => net_gnd0,
      DCRCLK => net_gnd0,
      C405RSTCHIPRESETREQ => open,
      C405RSTCORERESETREQ => open,
      C405RSTSYSRESETREQ => open,
      RSTC405RESETCHIP => net_gnd0,
      RSTC405RESETCORE => net_gnd0,
      RSTC405RESETSYS => net_gnd0,
      C405PLBICUABUS => open,
      C405PLBICUBE => open,
      C405PLBICURNW => open,
      C405PLBICUABORT => open,
      C405PLBICUBUSLOCK => open,
      C405PLBICUU0ATTR => open,
      C405PLBICUGUARDED => open,
      C405PLBICULOCKERR => open,
      C405PLBICUMSIZE => open,
      C405PLBICUORDERED => open,
      C405PLBICUPRIORITY => open,
      C405PLBICURDBURST => open,
      C405PLBICUREQUEST => open,
      C405PLBICUSIZE => open,
      C405PLBICUTYPE => open,
      C405PLBICUWRBURST => open,
      C405PLBICUWRDBUS => open,
      C405PLBICUCACHEABLE => open,
      PLBC405ICUADDRACK => net_gnd0,
      PLBC405ICUBUSY => net_gnd0,
      PLBC405ICUERR => net_gnd0,
      PLBC405ICURDBTERM => net_gnd0,
      PLBC405ICURDDACK => net_gnd0,
      PLBC405ICURDDBUS => net_gnd64,
      PLBC405ICURDWDADDR => net_gnd4,
      PLBC405ICUREARBITRATE => net_gnd0,
      PLBC405ICUWRBTERM => net_gnd0,
      PLBC405ICUWRDACK => net_gnd0,
      PLBC405ICUSSIZE => net_gnd2,
      PLBC405ICUSERR => net_gnd0,
      PLBC405ICUSBUSYS => net_gnd0,
      C405PLBDCUABUS => open,
      C405PLBDCUBE => open,
      C405PLBDCURNW => open,
      C405PLBDCUABORT => open,
      C405PLBDCUBUSLOCK => open,
      C405PLBDCUU0ATTR => open,
      C405PLBDCUGUARDED => open,
      C405PLBDCULOCKERR => open,
      C405PLBDCUMSIZE => open,
      C405PLBDCUORDERED => open,
      C405PLBDCUPRIORITY => open,
      C405PLBDCURDBURST => open,
      C405PLBDCUREQUEST => open,
      C405PLBDCUSIZE => open,
      C405PLBDCUTYPE => open,
      C405PLBDCUWRBURST => open,
      C405PLBDCUWRDBUS => open,
      C405PLBDCUCACHEABLE => open,
      C405PLBDCUWRITETHRU => open,
      PLBC405DCUADDRACK => net_gnd0,
      PLBC405DCUBUSY => net_gnd0,
      PLBC405DCUERR => net_gnd0,
      PLBC405DCURDBTERM => net_gnd0,
      PLBC405DCURDDACK => net_gnd0,
      PLBC405DCURDDBUS => net_gnd64,
      PLBC405DCURDWDADDR => net_gnd4,
      PLBC405DCUREARBITRATE => net_gnd0,
      PLBC405DCUWRBTERM => net_gnd0,
      PLBC405DCUWRDACK => net_gnd0,
      PLBC405DCUSSIZE => net_gnd2,
      PLBC405DCUSERR => net_gnd0,
      PLBC405DCUSBUSYS => net_gnd0,
      BRAMDSOCMCLK => net_gnd0,
      BRAMDSOCMRDDBUS => net_gnd32,
      DSARCVALUE => net_gnd8,
      DSCNTLVALUE => net_gnd8,
      DSOCMBRAMABUS => open,
      DSOCMBRAMBYTEWRITE => open,
      DSOCMBRAMEN => open,
      DSOCMBRAMWRDBUS => open,
      DSOCMBUSY => open,
      BRAMISOCMCLK => net_gnd0,
      BRAMISOCMRDDBUS => net_gnd64,
      ISARCVALUE => net_gnd8,
      ISCNTLVALUE => net_gnd8,
      ISOCMBRAMEN => open,
      ISOCMBRAMEVENWRITEEN => open,
      ISOCMBRAMODDWRITEEN => open,
      ISOCMBRAMRDABUS => open,
      ISOCMBRAMWRABUS => open,
      ISOCMBRAMWRDBUS => open,
      C405DCRABUS => open,
      C405DCRDBUSOUT => open,
      C405DCRREAD => open,
      C405DCRWRITE => open,
      DCRC405ACK => net_gnd0,
      DCRC405DBUSIN => net_gnd32,
      EICC405CRITINPUTIRQ => net_gnd0,
      EICC405EXTINPUTIRQ => net_gnd0,
      C405JTGCAPTUREDR => open,
      C405JTGEXTEST => open,
      C405JTGPGMOUT => open,
      C405JTGSHIFTDR => open,
      C405JTGTDO => jtagppc_0_1_C405JTGTDO,
      C405JTGTDOEN => jtagppc_0_1_C405JTGTDOEN,
      C405JTGUPDATEDR => open,
      MCBJTAGEN => net_vcc0,
      JTGC405BNDSCANTDO => net_gnd0,
      JTGC405TCK => jtagppc_0_1_JTGC405TCK,
      JTGC405TDI => jtagppc_0_1_JTGC405TDI,
      JTGC405TMS => jtagppc_0_1_JTGC405TMS,
      JTGC405TRSTNEG => jtagppc_0_1_JTGC405TRSTNEG,
      C405DBGMSRWE => open,
      C405DBGSTOPACK => open,
      C405DBGWBCOMPLETE => open,
      C405DBGWBFULL => open,
      C405DBGWBIAR => open,
      DBGC405DEBUGHALT => net_gnd0,
      DBGC405EXTBUSHOLDACK => net_gnd0,
      DBGC405UNCONDDEBUGEVENT => net_gnd0,
      C405TRCCYCLE => open,
      C405TRCEVENEXECUTIONSTATUS => open,
      C405TRCODDEXECUTIONSTATUS => open,
      C405TRCTRACESTATUS => open,
      C405TRCTRIGGEREVENTOUT => open,
      C405TRCTRIGGEREVENTTYPE => open,
      TRCC405TRACEDISABLE => net_gnd0,
      TRCC405TRIGGEREVENTIN => net_gnd0
    );

  jtagppc_0 : jtagppc_0_wrapper
    port map (
      TRSTNEG => net_vcc0,
      HALTNEG0 => net_vcc0,
      DBGC405DEBUGHALT0 => open,
      HALTNEG1 => net_vcc0,
      DBGC405DEBUGHALT1 => open,
      C405JTGTDO0 => jtagppc_0_0_C405JTGTDO,
      C405JTGTDOEN0 => jtagppc_0_0_C405JTGTDOEN,
      JTGC405TCK0 => jtagppc_0_0_JTGC405TCK,
      JTGC405TDI0 => jtagppc_0_0_JTGC405TDI,
      JTGC405TMS0 => jtagppc_0_0_JTGC405TMS,
      JTGC405TRSTNEG0 => jtagppc_0_0_JTGC405TRSTNEG,
      C405JTGTDO1 => jtagppc_0_1_C405JTGTDO,
      C405JTGTDOEN1 => jtagppc_0_1_C405JTGTDOEN,
      JTGC405TCK1 => jtagppc_0_1_JTGC405TCK,
      JTGC405TDI1 => jtagppc_0_1_JTGC405TDI,
      JTGC405TMS1 => jtagppc_0_1_JTGC405TMS,
      JTGC405TRSTNEG1 => jtagppc_0_1_JTGC405TRSTNEG
    );

  reset_block : reset_block_wrapper
    port map (
      Slowest_sync_clk => sys_clk_s,
      Ext_Reset_In => sys_rst_s,
      Aux_Reset_In => net_gnd0,
      Core_Reset_Req => C405RSTCORERESETREQ,
      Chip_Reset_Req => C405RSTCHIPRESETREQ,
      System_Reset_Req => C405RSTSYSRESETREQ,
      Dcm_locked => dcm_0_lock,
      Rstc405resetcore => RSTC405RESETCORE,
      Rstc405resetchip => RSTC405RESETCHIP,
      Rstc405resetsys => RSTC405RESETSYS,
      Bus_Struct_Reset => sys_bus_reset(0 to 0),
      Peripheral_Reset => open
    );

  iocm : iocm_wrapper
    port map (
      ISOCM_Clk => sys_clk_s,
      SYS_Rst => sys_bus_reset(0),
      ISOCM_Rst => iocm_ISOCM_Rst,
      M_ISOCMBRAMRDABUS => iocm_M_ISOCMBRAMRDABUS,
      M_ISOCMBRAMWRABUS => iocm_M_ISOCMBRAMWRABUS,
      M_ISOCMBRAMEN => iocm_M_ISOCMBRAMEN,
      M_ISOCMBRAMWRDBUS => iocm_M_ISOCMBRAMWRDBUS,
      M_ISOCMBRAMODDWRITEEN => iocm_M_ISOCMBRAMODDWRITEEN,
      M_ISOCMBRAMEVENWRITEEN => iocm_M_ISOCMBRAMEVENWRITEEN,
      M_ISOCMDCRBRAMODDEN => net_gnd0,
      M_ISOCMDCRBRAMEVENEN => net_gnd0,
      M_ISOCMDCRBRAMRDSELECT => net_gnd0,
      S_BRAMISOCMRDDBUS => iocm_S_BRAMISOCMRDDBUS,
      S_BRAMISOCMDCRRDDBUS => iocm_S_BRAMISOCMDCRRDDBUS,
      S_ISOCMSLAVESELECT => iocm_S_ISOCMSLAVESELECT(0 to 0),
      S_ISOCMDCRSLAVESELECT => iocm_S_ISOCMDCRSLAVESELECT(0 to 0),
      BRAMISOCMRDDBUS => iocm_BRAMISOCMRDDBUS,
      ISARCVALUE => iocm_ISARCVALUE,
      ISCNTLVALUE => iocm_ISCNTLVALUE,
      ISOCMBRAMEN => iocm_ISOCMBRAMEN,
      ISOCMBRAMWRDBUS => iocm_ISOCMBRAMWRDBUS,
      ISOCMBRAMODDWRITEEN => iocm_ISOCMBRAMODDWRITEEN,
      ISOCMBRAMEVENWRITEEN => iocm_ISOCMBRAMEVENWRITEEN,
      ISOCMBRAMRDABUS => iocm_ISOCMBRAMRDABUS,
      ISOCMBRAMWRABUS => iocm_ISOCMBRAMWRABUS,
      ISOCMDCRBRAMODDEN => iocm_ISOCMDCRBRAMODDEN,
      ISOCMDCRBRAMEVENEN => iocm_ISOCMDCRBRAMEVENEN,
      ISOCMDCRBRAMRDSELECT => iocm_ISOCMDCRBRAMRDSELECT,
      BRAMISOCMDCRRDDBUS => open
    );

  iocm_cntlr : iocm_cntlr_wrapper
    port map (
      BRAMISOCMCLK => sys_clk_s,
      ISBRAMRST => iocm_ISOCM_Rst,
      ISOCMBRAMRDABUS => iocm_ISOCMBRAMRDABUS,
      ISOCMBRAMWRABUS => iocm_ISOCMBRAMWRABUS,
      ISOCMBRAMEN => iocm_ISOCMBRAMEN,
      ISOCMBRAMWRDBUS => iocm_ISOCMBRAMWRDBUS,
      ISOCMBRAMODDWRITEEN => iocm_ISOCMBRAMODDWRITEEN,
      ISOCMBRAMEVENWRITEEN => iocm_ISOCMBRAMEVENWRITEEN,
      ISOCMDCRBRAMODDEN => iocm_ISOCMDCRBRAMODDEN,
      ISOCMDCRBRAMEVENEN => iocm_ISOCMDCRBRAMEVENEN,
      ISOCMDCRBRAMRDSELECT => iocm_ISOCMDCRBRAMRDSELECT,
      S_BRAMISOCMRDDBUS => iocm_S_BRAMISOCMRDDBUS,
      S_BRAMISOCMDCRRDDBUS => iocm_S_BRAMISOCMDCRRDDBUS,
      S_ISOCMSLAVESELECT => iocm_S_ISOCMSLAVESELECT(0),
      S_ISOCMDCRSLAVESELECT => iocm_S_ISOCMDCRSLAVESELECT(0),
      BRAM_Rst_A => isocm_porta_BRAM_Rst,
      BRAM_Clk_A => isocm_porta_BRAM_Clk,
      BRAM_Addr_A => isocm_porta_BRAM_Addr,
      BRAM_EN_A => isocm_porta_BRAM_EN,
      BRAM_WEN_A => isocm_porta_BRAM_WEN,
      BRAM_Dout_A => isocm_porta_BRAM_Dout,
      BRAM_Din_A => isocm_porta_BRAM_Din,
      BRAM_Rst_B => isocm_portb_BRAM_Rst,
      BRAM_Clk_B => isocm_portb_BRAM_Clk,
      BRAM_Addr_B => isocm_portb_BRAM_Addr,
      BRAM_EN_B => isocm_portb_BRAM_EN,
      BRAM_WEN_B => isocm_portb_BRAM_WEN,
      BRAM_Dout_B => isocm_portb_BRAM_Dout,
      BRAM_Din_B => isocm_portb_BRAM_Din
    );

  isocm_bram : isocm_bram_wrapper
    port map (
      BRAM_Rst_A => isocm_porta_BRAM_Rst,
      BRAM_Clk_A => isocm_porta_BRAM_Clk,
      BRAM_EN_A => isocm_porta_BRAM_EN,
      BRAM_WEN_A => isocm_porta_BRAM_WEN,
      BRAM_Addr_A => isocm_porta_BRAM_Addr,
      BRAM_Din_A => isocm_porta_BRAM_Din,
      BRAM_Dout_A => isocm_porta_BRAM_Dout,
      BRAM_Rst_B => isocm_portb_BRAM_Rst,
      BRAM_Clk_B => isocm_portb_BRAM_Clk,
      BRAM_EN_B => isocm_portb_BRAM_EN,
      BRAM_WEN_B => isocm_portb_BRAM_WEN,
      BRAM_Addr_B => isocm_portb_BRAM_Addr,
      BRAM_Din_B => isocm_portb_BRAM_Din,
      BRAM_Dout_B => isocm_portb_BRAM_Dout
    );

  docm : docm_wrapper
    port map (
      DSOCM_Clk => sys_clk_s,
      SYS_Rst => sys_bus_reset(0),
      DSOCM_Rst => docm_DSOCM_Rst,
      M_DSOCMBRAMABUS => docm_M_DSOCMBRAMABUS,
      M_DSOCMBRAMBYTEWRITE => docm_M_DSOCMBRAMBYTEWRITE,
      M_DSOCMBRAMEN => docm_M_DSOCMBRAMEN,
      M_DSOCMBRAMWRDBUS => docm_M_DSOCMBRAMWRDBUS,
      M_DSOCMBUSY => docm_M_DSOCMBUSY,
      M_DSOCMRDADDRVALID => net_gnd0,
      M_DSOCMWRADDRVALID => net_gnd0,
      S_BRAMDSOCMRDDBUS => docm_S_BRAMDSOCMRDDBUS,
      S_DSOCMRWCOMPLETE => docm_S_DSOCMRWCOMPLETE(0 to 0),
      S_DSOCMSLAVESELECT => docm_S_DSOCMSLAVESELECT(0 to 0),
      BRAMDSOCMRDDBUS => docm_BRAMDSOCMRDDBUS,
      DSARCVALUE => docm_DSARCVALUE,
      DSCNTLVALUE => docm_DSCNTLVALUE,
      DSOCMBRAMWRDBUS => docm_DSOCMBRAMWRDBUS,
      DSOCMBRAMEN => docm_DSOCMBRAMEN,
      DSOCMBRAMBYTEWRITE => docm_DSOCMBRAMBYTEWRITE,
      DSOCMBRAMABUS => docm_DSOCMBRAMABUS,
      DSOCMBUSY => open,
      DSOCMRDADDRVALID => open,
      DSOCMWRADDRVALID => open,
      DSOCMRWCOMPLETE => open
    );

  docm_cntlr : docm_cntlr_wrapper
    port map (
      BRAMDSOCMCLK => sys_clk_s,
      DSBRAMRST => docm_DSOCM_Rst,
      DSOCMBRAMABUS => docm_DSOCMBRAMABUS,
      DSOCMBRAMBYTEWRITE => docm_DSOCMBRAMBYTEWRITE,
      DSOCMBRAMEN => docm_DSOCMBRAMEN,
      DSOCMBRAMWRDBUS => docm_DSOCMBRAMWRDBUS,
      S_BRAMDSOCMRDDBUS => docm_S_BRAMDSOCMRDDBUS,
      S_DSOCMRWCOMPLETE => docm_S_DSOCMRWCOMPLETE(0),
      S_DSOCMSLAVESELECT => docm_S_DSOCMSLAVESELECT(0),
      BRAM_Rst_A => dsocm_porta_BRAM_Rst,
      BRAM_Clk_A => dsocm_porta_BRAM_Clk,
      BRAM_EN_A => dsocm_porta_BRAM_EN,
      BRAM_WEN_A => dsocm_porta_BRAM_WEN,
      BRAM_Addr_A => dsocm_porta_BRAM_Addr,
      BRAM_Din_A => dsocm_porta_BRAM_Din,
      BRAM_Dout_A => dsocm_porta_BRAM_Dout
    );

  dsocm_bram : dsocm_bram_wrapper
    port map (
      BRAM_Rst_A => dsocm_porta_BRAM_Rst,
      BRAM_Clk_A => dsocm_porta_BRAM_Clk,
      BRAM_EN_A => dsocm_porta_BRAM_EN,
      BRAM_WEN_A => dsocm_porta_BRAM_WEN,
      BRAM_Addr_A => dsocm_porta_BRAM_Addr,
      BRAM_Din_A => dsocm_porta_BRAM_Din,
      BRAM_Dout_A => dsocm_porta_BRAM_Dout,
      BRAM_Rst_B => net_gnd0,
      BRAM_Clk_B => net_gnd0,
      BRAM_EN_B => net_gnd0,
      BRAM_WEN_B => net_gnd4,
      BRAM_Addr_B => net_gnd32,
      BRAM_Din_B => open,
      BRAM_Dout_B => net_gnd32
    );

  plb : plb_wrapper
    port map (
      PLB_Clk => sys_clk_s,
      SYS_Rst => sys_bus_reset(0),
      PLB_Rst => plb_PLB_Rst,
      PLB_dcrAck => open,
      PLB_dcrDBus => open,
      DCR_ABus => net_gnd10,
      DCR_DBus => net_gnd32,
      DCR_Read => net_gnd0,
      DCR_Write => net_gnd0,
      M_ABus => plb_M_ABus,
      M_BE => plb_M_BE,
      M_RNW => plb_M_RNW,
      M_abort => plb_M_abort,
      M_busLock => plb_M_busLock,
      M_compress => plb_M_compress,
      M_guarded => plb_M_guarded,
      M_lockErr => plb_M_lockErr,
      M_MSize => plb_M_MSize,
      M_ordered => plb_M_ordered,
      M_priority => plb_M_priority,
      M_rdBurst => plb_M_rdBurst,
      M_request => plb_M_request,
      M_size => plb_M_size,
      M_type => plb_M_type,
      M_wrBurst => plb_M_wrBurst,
      M_wrDBus => plb_M_wrDBus,
      Sl_addrAck => plb_Sl_addrAck,
      Sl_MErr => plb_Sl_MErr,
      Sl_MBusy => plb_Sl_MBusy,
      Sl_rdBTerm => plb_Sl_rdBTerm,
      Sl_rdComp => plb_Sl_rdComp,
      Sl_rdDAck => plb_Sl_rdDAck,
      Sl_rdDBus => plb_Sl_rdDBus,
      Sl_rdWdAddr => plb_Sl_rdWdAddr,
      Sl_rearbitrate => plb_Sl_rearbitrate,
      Sl_SSize => plb_Sl_SSize,
      Sl_wait => plb_Sl_wait,
      Sl_wrBTerm => plb_Sl_wrBTerm,
      Sl_wrComp => plb_Sl_wrComp,
      Sl_wrDAck => plb_Sl_wrDAck,
      PLB_ABus => plb_PLB_ABus,
      PLB_BE => plb_PLB_BE,
      PLB_MAddrAck => plb_PLB_MAddrAck,
      PLB_MBusy => plb_PLB_MBusy,
      PLB_MErr => plb_PLB_MErr,
      PLB_MRdBTerm => plb_PLB_MRdBTerm,
      PLB_MRdDAck => plb_PLB_MRdDAck,
      PLB_MRdDBus => plb_PLB_MRdDBus,
      PLB_MRdWdAddr => plb_PLB_MRdWdAddr,
      PLB_MRearbitrate => plb_PLB_MRearbitrate,
      PLB_MWrBTerm => plb_PLB_MWrBTerm,
      PLB_MWrDAck => plb_PLB_MWrDAck,
      PLB_MSSize => plb_PLB_MSSize,
      PLB_PAValid => plb_PLB_PAValid,
      PLB_RNW => plb_PLB_RNW,
      PLB_SAValid => plb_PLB_SAValid,
      PLB_abort => plb_PLB_abort,
      PLB_busLock => plb_PLB_busLock,
      PLB_compress => plb_PLB_compress,
      PLB_guarded => plb_PLB_guarded,
      PLB_lockErr => plb_PLB_lockErr,
      PLB_masterID => plb_PLB_masterID(0 to 0),
      PLB_MSize => plb_PLB_MSize,
      PLB_ordered => plb_PLB_ordered,
      PLB_pendPri => plb_PLB_pendPri,
      PLB_pendReq => plb_PLB_pendReq,
      PLB_rdBurst => plb_PLB_rdBurst,
      PLB_rdPrim => plb_PLB_rdPrim,
      PLB_reqPri => plb_PLB_reqPri,
      PLB_size => plb_PLB_size,
      PLB_type => plb_PLB_type,
      PLB_wrBurst => plb_PLB_wrBurst,
      PLB_wrDBus => plb_PLB_wrDBus,
      PLB_wrPrim => plb_PLB_wrPrim,
      PLB_SaddrAck => open,
      PLB_SMErr => plb_PLB_SMErr,
      PLB_SMBusy => plb_PLB_SMBusy,
      PLB_SrdBTerm => open,
      PLB_SrdComp => open,
      PLB_SrdDAck => open,
      PLB_SrdDBus => open,
      PLB_SrdWdAddr => open,
      PLB_Srearbitrate => open,
      PLB_Sssize => open,
      PLB_Swait => open,
      PLB_SwrBTerm => open,
      PLB_SwrComp => open,
      PLB_SwrDAck => open,
      PLB2OPB_rearb => plb_PLB2OPB_rearb,
      ArbAddrVldReg => open,
      Bus_Error_Det => open
    );

  generic_external_memory : generic_external_memory_wrapper
    port map (
      PLB_Clk => sys_clk_s,
      PLB_Rst => plb_PLB_Rst,
      PLB_abort => plb_PLB_abort,
      PLB_ABus => plb_PLB_ABus,
      PLB_BE => plb_PLB_BE,
      PLB_busLock => plb_PLB_busLock,
      PLB_compress => plb_PLB_compress,
      PLB_guarded => plb_PLB_guarded,
      PLB_lockErr => plb_PLB_lockErr,
      PLB_masterID => plb_PLB_masterID(0 to 0),
      PLB_MSize => plb_PLB_MSize,
      PLB_ordered => plb_PLB_ordered,
      PLB_PAValid => plb_PLB_PAValid,
      PLB_RNW => plb_PLB_RNW,
      PLB_size => plb_PLB_size,
      PLB_type => plb_PLB_type,
      Sl_addrAck => plb_Sl_addrAck(0),
      Sl_MBusy => plb_Sl_MBusy(0 to 1),
      Sl_MErr => plb_Sl_MErr(0 to 1),
      Sl_rearbitrate => plb_Sl_rearbitrate(0),
      Sl_SSize => plb_Sl_SSize(0 to 1),
      Sl_wait => plb_Sl_wait(0),
      PLB_rdPrim => plb_PLB_rdPrim,
      PLB_SAValid => plb_PLB_SAValid,
      PLB_wrPrim => plb_PLB_wrPrim,
      PLB_wrBurst => plb_PLB_wrBurst,
      PLB_wrDBus => plb_PLB_wrDBus,
      Sl_wrBTerm => plb_Sl_wrBTerm(0),
      Sl_wrComp => plb_Sl_wrComp(0),
      Sl_wrDAck => plb_Sl_wrDAck(0),
      PLB_rdBurst => plb_PLB_rdBurst,
      Sl_rdBTerm => plb_Sl_rdBTerm(0),
      Sl_rdComp => plb_Sl_rdComp(0),
      Sl_rdDAck => plb_Sl_rdDAck(0),
      Sl_rdDBus => plb_Sl_rdDBus(0 to 63),
      Sl_rdWdAddr => plb_Sl_rdWdAddr(0 to 3),
      PLB_pendReq => plb_PLB_pendReq,
      PLB_pendPri => plb_PLB_pendPri,
      PLB_reqPri => plb_PLB_reqPri,
      Mem_A => fpga_0_Generic_External_Memory_Mem_A_split,
      Mem_CEN => fpga_0_Generic_External_Memory_Mem_CEN(0 to 0),
      Mem_OEN => fpga_0_Generic_External_Memory_Mem_OEN(0 to 0),
      Mem_WEN => fpga_0_Generic_External_Memory_Mem_WEN,
      Mem_QWEN => open,
      Mem_BEN => fpga_0_Generic_External_Memory_Mem_BEN,
      Mem_RPN => fpga_0_Generic_External_Memory_Mem_RPN,
      Mem_CE => open,
      Mem_ADV_LDN => open,
      Mem_LBON => open,
      Mem_CKEN => open,
      Mem_RNW => open,
      Mem_DQ_I => fpga_0_Generic_External_Memory_Mem_DQ_I,
      Mem_DQ_O => fpga_0_Generic_External_Memory_Mem_DQ_O,
      Mem_DQ_T => fpga_0_Generic_External_Memory_Mem_DQ_T
    );

  opb : opb_wrapper
    port map (
      OPB_Clk => sys_clk_s,
      OPB_Rst => opb_OPB_Rst,
      SYS_Rst => sys_bus_reset(0),
      Debug_SYS_Rst => net_gnd0,
      WDT_Rst => net_gnd0,
      M_ABus => opb_M_ABus,
      M_BE => opb_M_BE,
      M_beXfer => net_gnd1(0 to 0),
      M_busLock => opb_M_busLock(0 to 0),
      M_DBus => opb_M_DBus,
      M_DBusEn => net_gnd1(0 to 0),
      M_DBusEn32_63 => net_vcc1(0 to 0),
      M_dwXfer => net_gnd1(0 to 0),
      M_fwXfer => net_gnd1(0 to 0),
      M_hwXfer => net_gnd1(0 to 0),
      M_request => opb_M_request(0 to 0),
      M_RNW => opb_M_RNW(0 to 0),
      M_select => opb_M_select(0 to 0),
      M_seqAddr => opb_M_seqAddr(0 to 0),
      Sl_beAck => net_gnd3,
      Sl_DBus => opb_Sl_DBus,
      Sl_DBusEn => net_vcc3,
      Sl_DBusEn32_63 => net_vcc3,
      Sl_errAck => opb_Sl_errAck,
      Sl_dwAck => net_gnd3,
      Sl_fwAck => net_gnd3,
      Sl_hwAck => net_gnd3,
      Sl_retry => opb_Sl_retry,
      Sl_toutSup => opb_Sl_toutSup,
      Sl_xferAck => opb_Sl_xferAck,
      OPB_MRequest => open,
      OPB_ABus => opb_OPB_ABus,
      OPB_BE => opb_OPB_BE,
      OPB_beXfer => open,
      OPB_beAck => open,
      OPB_busLock => open,
      OPB_rdDBus => open,
      OPB_wrDBus => open,
      OPB_DBus => opb_OPB_DBus,
      OPB_errAck => opb_OPB_errAck,
      OPB_dwAck => open,
      OPB_dwXfer => open,
      OPB_fwAck => open,
      OPB_fwXfer => open,
      OPB_hwAck => open,
      OPB_hwXfer => open,
      OPB_MGrant => opb_OPB_MGrant(0 to 0),
      OPB_pendReq => open,
      OPB_retry => opb_OPB_retry,
      OPB_RNW => opb_OPB_RNW,
      OPB_select => opb_OPB_select,
      OPB_seqAddr => opb_OPB_seqAddr,
      OPB_timeout => opb_OPB_timeout,
      OPB_toutSup => open,
      OPB_xferAck => opb_OPB_xferAck
    );

  plb2opb : plb2opb_wrapper
    port map (
      PLB_Clk => sys_clk_s,
      OPB_Clk => sys_clk_s,
      PLB_Rst => plb_PLB_Rst,
      OPB_Rst => opb_OPB_Rst,
      Bus_Error_Det => open,
      BGI_Trans_Abort => open,
      BGO_dcrAck => open,
      BGO_dcrDBus => open,
      DCR_ABus => net_gnd10,
      DCR_DBus => net_gnd32,
      DCR_Read => net_gnd0,
      DCR_Write => net_gnd0,
      BGO_addrAck => plb_Sl_addrAck(1),
      BGO_MErr => plb_Sl_MErr(2 to 3),
      BGO_MBusy => plb_Sl_MBusy(2 to 3),
      BGO_rdBTerm => plb_Sl_rdBTerm(1),
      BGO_rdComp => plb_Sl_rdComp(1),
      BGO_rdDAck => plb_Sl_rdDAck(1),
      BGO_rdDBus => plb_Sl_rdDBus(64 to 127),
      BGO_rdWdAddr => plb_Sl_rdWdAddr(4 to 7),
      BGO_rearbitrate => plb_Sl_rearbitrate(1),
      BGO_SSize => plb_Sl_SSize(2 to 3),
      BGO_wait => plb_Sl_wait(1),
      BGO_wrBTerm => plb_Sl_wrBTerm(1),
      BGO_wrComp => plb_Sl_wrComp(1),
      BGO_wrDAck => plb_Sl_wrDAck(1),
      PLB_abort => plb_PLB_abort,
      PLB_ABus => plb_PLB_ABus,
      PLB_BE => plb_PLB_BE,
      PLB_busLock => plb_PLB_busLock,
      PLB_compress => plb_PLB_compress,
      PLB_guarded => plb_PLB_guarded,
      PLB_lockErr => plb_PLB_lockErr,
      PLB_masterID => plb_PLB_masterID(0 to 0),
      PLB_MSize => plb_PLB_MSize,
      PLB_ordered => plb_PLB_ordered,
      PLB_PAValid => plb_PLB_PAValid,
      PLB_rdBurst => plb_PLB_rdBurst,
      PLB_rdPrim => plb_PLB_rdPrim,
      PLB_RNW => plb_PLB_RNW,
      PLB_SAValid => plb_PLB_SAValid,
      PLB_size => plb_PLB_size,
      PLB_type => plb_PLB_type,
      PLB_wrBurst => plb_PLB_wrBurst,
      PLB_wrDBus => plb_PLB_wrDBus,
      PLB_wrPrim => plb_PLB_wrPrim,
      PLB2OPB_rearb => plb_PLB2OPB_rearb(1),
      BGO_ABus => opb_M_ABus,
      BGO_BE => opb_M_BE,
      BGO_busLock => opb_M_busLock(0),
      BGO_DBus => opb_M_DBus,
      BGO_request => opb_M_request(0),
      BGO_RNW => opb_M_RNW(0),
      BGO_select => opb_M_select(0),
      BGO_seqAddr => opb_M_seqAddr(0),
      OPB_DBus => opb_OPB_DBus,
      OPB_errAck => opb_OPB_errAck,
      OPB_MnGrant => opb_OPB_MGrant(0),
      OPB_retry => opb_OPB_retry,
      OPB_timeout => opb_OPB_timeout,
      OPB_xferAck => opb_OPB_xferAck
    );

  generic_gpio : generic_gpio_wrapper
    port map (
      OPB_ABus => opb_OPB_ABus,
      OPB_BE => opb_OPB_BE,
      OPB_Clk => sys_clk_s,
      OPB_DBus => opb_OPB_DBus,
      OPB_RNW => opb_OPB_RNW,
      OPB_Rst => opb_OPB_Rst,
      OPB_select => opb_OPB_select,
      OPB_seqAddr => opb_OPB_seqAddr,
      Sln_DBus => opb_Sl_DBus(0 to 31),
      Sln_errAck => opb_Sl_errAck(0),
      Sln_retry => opb_Sl_retry(0),
      Sln_toutSup => opb_Sl_toutSup(0),
      Sln_xferAck => opb_Sl_xferAck(0),
      IP2INTC_Irpt => open,
      GPIO_in => ready_i,
      GPIO_d_out => ready_o, --fpga_0_Generic_GPIO_GPIO_d_out,
      GPIO_t_out => fpga_0_Generic_GPIO_GPIO_t_out,
      GPIO2_in => net_gnd2,
      GPIO2_d_out => open,
      GPIO2_t_out => open,
      GPIO_IO_I => fpga_0_Generic_GPIO_GPIO_IO_I,
      GPIO_IO_O => fpga_0_Generic_GPIO_GPIO_IO_O,
      GPIO_IO_T => fpga_0_Generic_GPIO_GPIO_IO_T,
      GPIO2_IO_I => net_gnd2,
      GPIO2_IO_O => open,
      GPIO2_IO_T => open
    );

  leds : leds_wrapper
    port map (
      OPB_ABus => opb_OPB_ABus,
      OPB_BE => opb_OPB_BE,
      OPB_Clk => sys_clk_s,
      OPB_DBus => opb_OPB_DBus,
      OPB_RNW => opb_OPB_RNW,
      OPB_Rst => opb_OPB_Rst,
      OPB_select => opb_OPB_select,
      OPB_seqAddr => opb_OPB_seqAddr,
      Sln_DBus => opb_Sl_DBus(32 to 63),
      Sln_errAck => opb_Sl_errAck(1),
      Sln_retry => opb_Sl_retry(1),
      Sln_toutSup => opb_Sl_toutSup(1),
      Sln_xferAck => opb_Sl_xferAck(1),
      IP2INTC_Irpt => open,
      GPIO_in => net_gnd32,
      GPIO_d_out => fpga_0_LEDS_GPIO_d_out,
      GPIO_t_out => open,
      GPIO2_in => net_gnd32,
      GPIO2_d_out => open,
      GPIO2_t_out => open,
      GPIO_IO_I => net_gnd32,
      GPIO_IO_O => open,
      GPIO_IO_T => open,
      GPIO2_IO_I => net_gnd32,
      GPIO2_IO_O => open,
      GPIO2_IO_T => open
    );

  rs232 : rs232_wrapper
    port map (
      OPB_Clk => sys_clk_s,
      OPB_Rst => opb_OPB_Rst,
      Interrupt => open,
      OPB_ABus => opb_OPB_ABus,
      OPB_BE => opb_OPB_BE,
      OPB_RNW => opb_OPB_RNW,
      OPB_select => opb_OPB_select,
      OPB_seqAddr => opb_OPB_seqAddr,
      OPB_DBus => opb_OPB_DBus,
      UART_DBus => opb_Sl_DBus(64 to 95),
      UART_errAck => opb_Sl_errAck(2),
      UART_retry => opb_Sl_retry(2),
      UART_toutSup => opb_Sl_toutSup(2),
      UART_xferAck => opb_Sl_xferAck(2),
      RX => fpga_0_RS232_RX,
      TX => fpga_0_RS232_TX
    );

  plb_bram_if_cntlr_1 : plb_bram_if_cntlr_1_wrapper
    port map (
      plb_clk => sys_clk_s,
      plb_rst => plb_PLB_Rst,
      plb_abort => plb_PLB_abort,
      plb_abus => plb_PLB_ABus,
      plb_be => plb_PLB_BE,
      plb_buslock => plb_PLB_busLock,
      plb_compress => plb_PLB_compress,
      plb_guarded => plb_PLB_guarded,
      plb_lockerr => plb_PLB_lockErr,
      plb_masterid => plb_PLB_masterID(0 to 0),
      plb_msize => plb_PLB_MSize,
      plb_ordered => plb_PLB_ordered,
      plb_pavalid => plb_PLB_PAValid,
      plb_rnw => plb_PLB_RNW,
      plb_size => plb_PLB_size,
      plb_type => plb_PLB_type,
      sl_addrack => plb_Sl_addrAck(2),
      sl_mbusy => plb_Sl_MBusy(4 to 5),
      sl_merr => plb_Sl_MErr(4 to 5),
      sl_rearbitrate => plb_Sl_rearbitrate(2),
      sl_ssize => plb_Sl_SSize(4 to 5),
      sl_wait => plb_Sl_wait(2),
      plb_rdprim => plb_PLB_rdPrim,
      plb_savalid => plb_PLB_SAValid,
      plb_wrprim => plb_PLB_wrPrim,
      plb_wrburst => plb_PLB_wrBurst,
      plb_wrdbus => plb_PLB_wrDBus,
      sl_wrbterm => plb_Sl_wrBTerm(2),
      sl_wrcomp => plb_Sl_wrComp(2),
      sl_wrdack => plb_Sl_wrDAck(2),
      plb_rdburst => plb_PLB_rdBurst,
      sl_rdbterm => plb_Sl_rdBTerm(2),
      sl_rdcomp => plb_Sl_rdComp(2),
      sl_rddack => plb_Sl_rdDAck(2),
      sl_rddbus => plb_Sl_rdDBus(128 to 191),
      sl_rdwdaddr => plb_Sl_rdWdAddr(8 to 11),
      plb_pendreq => plb_PLB_pendReq,
      plb_pendpri => plb_PLB_pendPri,
      plb_reqpri => plb_PLB_reqPri,
      bram_rst => plb_bram_if_cntlr_1_port_BRAM_Rst,
      bram_clk => plb_bram_if_cntlr_1_port_BRAM_Clk,
      bram_en => plb_bram_if_cntlr_1_port_BRAM_EN,
      bram_wen => plb_bram_if_cntlr_1_port_BRAM_WEN,
      bram_addr => plb_bram_if_cntlr_1_port_BRAM_Addr,
      bram_din => plb_bram_if_cntlr_1_port_BRAM_Din,
      bram_dout => plb_bram_if_cntlr_1_port_BRAM_Dout
    );

  plb_bram_if_cntlr_1_bram : plb_bram_if_cntlr_1_bram_wrapper
    port map (
      BRAM_Rst_A => plb_bram_if_cntlr_1_port_BRAM_Rst,
      BRAM_Clk_A => plb_bram_if_cntlr_1_port_BRAM_Clk,
      BRAM_EN_A => plb_bram_if_cntlr_1_port_BRAM_EN,
      BRAM_WEN_A => plb_bram_if_cntlr_1_port_BRAM_WEN,
      BRAM_Addr_A => plb_bram_if_cntlr_1_port_BRAM_Addr,
      BRAM_Din_A => plb_bram_if_cntlr_1_port_BRAM_Din,
      BRAM_Dout_A => plb_bram_if_cntlr_1_port_BRAM_Dout,
      BRAM_Rst_B => net_gnd0,
      BRAM_Clk_B => net_gnd0,
      BRAM_EN_B => net_gnd0,
      BRAM_WEN_B => net_gnd8,
      BRAM_Addr_B => net_gnd32,
      BRAM_Din_B => open,
      BRAM_Dout_B => net_gnd64
    );

  dcm_0 : dcm_0_wrapper
    port map (
      RST => net_gnd0,
      CLKIN => dcm_clk_s,
      CLKFB => proc_clk_s,
      PSEN => net_gnd0,
      PSINCDEC => net_gnd0,
      PSCLK => net_gnd0,
      DSSEN => net_gnd0,
      CLK0 => proc_clk_s,
      CLK90 => open,
      CLK180 => open,
      CLK270 => open,
      CLKDV => sys_clk_s,
      CLK2X => open,
      CLK2X180 => open,
      CLKFX => open,
      CLKFX180 => open,
      STATUS => open,
      LOCKED => dcm_0_lock,
      PSDONE => open
    );

  util_bus_split_0 : util_bus_split_0_wrapper
    port map (
      Sig => fpga_0_Generic_External_Memory_Mem_A_split,
      Out1 => fpga_0_Generic_External_Memory_Mem_A,
      Out2 => open
    );

  obuf_0 : OBUF
    port map (
      I => net_gnd0,
      O => fpga_0_RS232_req_to_send_pin
    );

  ibuf_1 : IBUF
    port map (
      I => fpga_0_RS232_RX_pin,
      O => fpga_0_RS232_RX
    );

  obuf_2 : OBUF
    port map (
      I => fpga_0_RS232_TX,
      O => fpga_0_RS232_TX_pin
    );

--  obuf_3 : OBUF
--    port map (
--      I => fpga_0_Generic_GPIO_GPIO_d_out(0),
--      O => fpga_0_Generic_GPIO_GPIO_d_out_pin(0)
--    );
--
--  obuf_4 : OBUF
--    port map (
--      I => fpga_0_Generic_GPIO_GPIO_d_out(1),
--      O => fpga_0_Generic_GPIO_GPIO_d_out_pin(1)
--    );
--
--  ibuf_5 : IBUF
--    port map (
--      I => fpga_0_Generic_GPIO_GPIO_in_pin(0),
--      O => fpga_0_Generic_GPIO_GPIO_in(0)
--    );
--
--  ibuf_6 : IBUF
--    port map (
--      I => fpga_0_Generic_GPIO_GPIO_in_pin(1),
--      O => fpga_0_Generic_GPIO_GPIO_in(1)
--    );
--
--  obuf_7 : OBUF
--    port map (
--      I => fpga_0_Generic_GPIO_GPIO_t_out(0),
--      O => fpga_0_Generic_GPIO_GPIO_t_out_pin(0)
--    );
--
--  obuf_8 : OBUF
--    port map (
--      I => fpga_0_Generic_GPIO_GPIO_t_out(1),
--      O => fpga_0_Generic_GPIO_GPIO_t_out_pin(1)
--    );
--
--  iobuf_9 : IOBUF
--    port map (
--      I => fpga_0_Generic_GPIO_GPIO_IO_O(0),
--      IO => fpga_0_Generic_GPIO_GPIO_IO_pin(0),
--      O => fpga_0_Generic_GPIO_GPIO_IO_I(0),
--      T => fpga_0_Generic_GPIO_GPIO_IO_T(0)
--    );
--
--  iobuf_10 : IOBUF
--    port map (
--      I => fpga_0_Generic_GPIO_GPIO_IO_O(1),
--      IO => fpga_0_Generic_GPIO_GPIO_IO_pin(1),
--      O => fpga_0_Generic_GPIO_GPIO_IO_I(1),
--      T => fpga_0_Generic_GPIO_GPIO_IO_T(1)
--    );
--
--  obuf_11 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(0),
--      O => fpga_0_LEDS_GPIO_d_out_pin(0)
--    );
--
--  obuf_12 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(1),
--      O => fpga_0_LEDS_GPIO_d_out_pin(1)
--    );
--
--  obuf_13 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(2),
--      O => fpga_0_LEDS_GPIO_d_out_pin(2)
--    );
--
--  obuf_14 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(3),
--      O => fpga_0_LEDS_GPIO_d_out_pin(3)
--    );
--
--  obuf_15 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(4),
--      O => fpga_0_LEDS_GPIO_d_out_pin(4)
--    );
--
--  obuf_16 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(5),
--      O => fpga_0_LEDS_GPIO_d_out_pin(5)
--    );
--
--  obuf_17 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(6),
--      O => fpga_0_LEDS_GPIO_d_out_pin(6)
--    );
--
--  obuf_18 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(7),
--      O => fpga_0_LEDS_GPIO_d_out_pin(7)
--    );
--
--  obuf_19 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(8),
--      O => fpga_0_LEDS_GPIO_d_out_pin(8)
--    );
--
--  obuf_20 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(9),
--      O => fpga_0_LEDS_GPIO_d_out_pin(9)
--    );
--
--  obuf_21 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(10),
--      O => fpga_0_LEDS_GPIO_d_out_pin(10)
--    );
--
--  obuf_22 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(11),
--      O => fpga_0_LEDS_GPIO_d_out_pin(11)
--    );
--
--  obuf_23 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(12),
--      O => fpga_0_LEDS_GPIO_d_out_pin(12)
--    );
--
--  obuf_24 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(13),
--      O => fpga_0_LEDS_GPIO_d_out_pin(13)
--    );
--
--  obuf_25 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(14),
--      O => fpga_0_LEDS_GPIO_d_out_pin(14)
--    );
--
--  obuf_26 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(15),
--      O => fpga_0_LEDS_GPIO_d_out_pin(15)
--    );
--
--  obuf_27 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(16),
--      O => fpga_0_LEDS_GPIO_d_out_pin(16)
--    );
--
--  obuf_28 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(17),
--      O => fpga_0_LEDS_GPIO_d_out_pin(17)
--    );
--
--  obuf_29 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(18),
--      O => fpga_0_LEDS_GPIO_d_out_pin(18)
--    );
--
--  obuf_30 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(19),
--      O => fpga_0_LEDS_GPIO_d_out_pin(19)
--    );
--
--  obuf_31 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(20),
--      O => fpga_0_LEDS_GPIO_d_out_pin(20)
--    );
--
--  obuf_32 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(21),
--      O => fpga_0_LEDS_GPIO_d_out_pin(21)
--    );
--
--  obuf_33 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(22),
--      O => fpga_0_LEDS_GPIO_d_out_pin(22)
--    );
--
--  obuf_34 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(23),
--      O => fpga_0_LEDS_GPIO_d_out_pin(23)
--    );
--
--  obuf_35 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(24),
--      O => fpga_0_LEDS_GPIO_d_out_pin(24)
--    );
--
--  obuf_36 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(25),
--      O => fpga_0_LEDS_GPIO_d_out_pin(25)
--    );
--
--  obuf_37 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(26),
--      O => fpga_0_LEDS_GPIO_d_out_pin(26)
--    );
--
--  obuf_38 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(27),
--      O => fpga_0_LEDS_GPIO_d_out_pin(27)
--    );
--
--  obuf_39 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(28),
--      O => fpga_0_LEDS_GPIO_d_out_pin(28)
--    );
--
--  obuf_40 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(29),
--      O => fpga_0_LEDS_GPIO_d_out_pin(29)
--    );
--
--  obuf_41 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(30),
--      O => fpga_0_LEDS_GPIO_d_out_pin(30)
--    );
--
--  obuf_42 : OBUF
--    port map (
--      I => fpga_0_LEDS_GPIO_d_out(31),
--      O => fpga_0_LEDS_GPIO_d_out_pin(31)
--    );

  iobuf_43 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(0),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(0),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(0),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(0)
    );

  iobuf_44 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(1),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(1),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(1),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(1)
    );

  iobuf_45 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(2),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(2),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(2),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(2)
    );

  iobuf_46 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(3),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(3),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(3),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(3)
    );

  iobuf_47 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(4),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(4),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(4),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(4)
    );

  iobuf_48 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(5),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(5),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(5),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(5)
    );

  iobuf_49 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(6),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(6),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(6),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(6)
    );

  iobuf_50 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(7),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(7),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(7),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(7)
    );

  iobuf_51 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(8),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(8),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(8),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(8)
    );

  iobuf_52 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(9),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(9),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(9),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(9)
    );

  iobuf_53 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(10),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(10),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(10),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(10)
    );

  iobuf_54 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(11),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(11),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(11),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(11)
    );

  iobuf_55 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(12),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(12),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(12),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(12)
    );

  iobuf_56 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(13),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(13),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(13),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(13)
    );

  iobuf_57 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(14),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(14),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(14),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(14)
    );

  iobuf_58 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(15),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(15),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(15),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(15)
    );

  obuf_59 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(7),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(7)
    );

  obuf_60 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(8),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(8)
    );

  obuf_61 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(9),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(9)
    );

  obuf_62 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(10),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(10)
    );

  obuf_63 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(11),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(11)
    );

  obuf_64 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(12),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(12)
    );

  obuf_65 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(13),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(13)
    );

  obuf_66 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(14),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(14)
    );

  obuf_67 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(15),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(15)
    );

  obuf_68 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(16),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(16)
    );

  obuf_69 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(17),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(17)
    );

  obuf_70 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(18),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(18)
    );

  obuf_71 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(19),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(19)
    );

  obuf_72 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(20),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(20)
    );

  obuf_73 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(21),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(21)
    );

  obuf_74 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(22),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(22)
    );

  obuf_75 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(23),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(23)
    );

  obuf_76 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(24),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(24)
    );

  obuf_77 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(25),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(25)
    );

  obuf_78 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(26),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(26)
    );

  obuf_79 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(27),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(27)
    );

  obuf_80 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(28),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(28)
    );

  obuf_81 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(29),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(29)
    );

  obuf_82 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(30),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(30)
    );

  obuf_83 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_BEN(0),
      O => fpga_0_Generic_External_Memory_Mem_BEN_pin(0)
    );

  obuf_84 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_BEN(1),
      O => fpga_0_Generic_External_Memory_Mem_BEN_pin(1)
    );

  obuf_85 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_WEN,
      O => fpga_0_Generic_External_Memory_Mem_WEN_pin
    );

  obuf_86 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_OEN(0),
      O => fpga_0_Generic_External_Memory_Mem_OEN_pin(0)
    );

  obuf_87 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_CEN(0),
      O => fpga_0_Generic_External_Memory_Mem_CEN_pin(0)
    );

  obuf_88 : OBUF
    port map (
      I => net_vcc0,
      O => fpga_0_Generic_External_Memory_flash_csn_dummy_pin
    );

  obuf_89 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_RPN,
      O => fpga_0_Generic_External_Memory_Mem_RPN_pin
    );



  ibuf_92 : IBUF
    port map (
      I => sys_rst_pin,
      O => sys_rst_s
    );

  obuf_93 : OBUF
    port map (
      I => net_gnd0,
      O => FADV
    );

  obuf_94 : OBUF
    port map (
      I => net_vcc0,
      O => FRST
    );

  obuf_95 : OBUF
    port map (
      I => net_vcc0,
      O => FWP
    );
	 
diff_buf_brefclk : IBUFGDS
generic map (IOSTANDARD => "LVDS_25_DT") 
port map (
  		O  => dcm_clk_s,
  		I  => sys_clk_pin_p,
  		IB => sys_clk_pin_n
);

test_readybit: process(sys_clk_s)
begin
	if sys_rst_s = '0' then
		ready_i <= "00";
	elsif sys_clk_s'event and sys_clk_s = '1' then
		if ready_o = "01" then
			ready_i <= "00";
		end if;
	end if;
end process;
end architecture STRUCTURE;

