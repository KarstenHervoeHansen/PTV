-------------------------------------------------------------------------------
-- ppc405_0_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library ppc405_v2_00_c;
use ppc405_v2_00_c.All;

entity ppc405_0_wrapper is
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
  attribute x_core_info : STRING;
  attribute x_core_info of ppc405_0_wrapper: entity is "ppc405_v2_00_c";

end ppc405_0_wrapper;

architecture STRUCTURE of ppc405_0_wrapper is

  component ppc405_top is
    generic (
      C_ISOCM_DCR_BASEADDR : std_logic_vector;
      C_ISOCM_DCR_HIGHADDR : std_logic_vector;
      C_DSOCM_DCR_BASEADDR : std_logic_vector;
      C_DSOCM_DCR_HIGHADDR : std_logic_vector;
      C_DISABLE_OPERAND_FORWARDING : integer;
      C_DETERMINISTIC_MULT : integer;
      C_MMU_ENABLE : integer;
      C_DCR_RESYNC : integer
    );
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

begin

  ppc405_0 : ppc405_top
    generic map (
      C_ISOCM_DCR_BASEADDR => B"0000010000",
      C_ISOCM_DCR_HIGHADDR => B"0000010011",
      C_DSOCM_DCR_BASEADDR => B"0000100000",
      C_DSOCM_DCR_HIGHADDR => B"0000100011",
      C_DISABLE_OPERAND_FORWARDING => 1,
      C_DETERMINISTIC_MULT => 0,
      C_MMU_ENABLE => 1,
      C_DCR_RESYNC => 0
    )
    port map (
      C405CPMCORESLEEPREQ => C405CPMCORESLEEPREQ,
      C405CPMMSRCE => C405CPMMSRCE,
      C405CPMMSREE => C405CPMMSREE,
      C405CPMTIMERIRQ => C405CPMTIMERIRQ,
      C405CPMTIMERRESETREQ => C405CPMTIMERRESETREQ,
      C405XXXMACHINECHECK => C405XXXMACHINECHECK,
      CPMC405CLOCK => CPMC405CLOCK,
      CPMC405CORECLKINACTIVE => CPMC405CORECLKINACTIVE,
      CPMC405CPUCLKEN => CPMC405CPUCLKEN,
      CPMC405JTAGCLKEN => CPMC405JTAGCLKEN,
      CPMC405TIMERCLKEN => CPMC405TIMERCLKEN,
      CPMC405TIMERTICK => CPMC405TIMERTICK,
      MCBCPUCLKEN => MCBCPUCLKEN,
      MCBTIMEREN => MCBTIMEREN,
      MCPPCRST => MCPPCRST,
      PLBCLK => PLBCLK,
      DCRCLK => DCRCLK,
      C405RSTCHIPRESETREQ => C405RSTCHIPRESETREQ,
      C405RSTCORERESETREQ => C405RSTCORERESETREQ,
      C405RSTSYSRESETREQ => C405RSTSYSRESETREQ,
      RSTC405RESETCHIP => RSTC405RESETCHIP,
      RSTC405RESETCORE => RSTC405RESETCORE,
      RSTC405RESETSYS => RSTC405RESETSYS,
      C405PLBICUABUS => C405PLBICUABUS,
      C405PLBICUBE => C405PLBICUBE,
      C405PLBICURNW => C405PLBICURNW,
      C405PLBICUABORT => C405PLBICUABORT,
      C405PLBICUBUSLOCK => C405PLBICUBUSLOCK,
      C405PLBICUU0ATTR => C405PLBICUU0ATTR,
      C405PLBICUGUARDED => C405PLBICUGUARDED,
      C405PLBICULOCKERR => C405PLBICULOCKERR,
      C405PLBICUMSIZE => C405PLBICUMSIZE,
      C405PLBICUORDERED => C405PLBICUORDERED,
      C405PLBICUPRIORITY => C405PLBICUPRIORITY,
      C405PLBICURDBURST => C405PLBICURDBURST,
      C405PLBICUREQUEST => C405PLBICUREQUEST,
      C405PLBICUSIZE => C405PLBICUSIZE,
      C405PLBICUTYPE => C405PLBICUTYPE,
      C405PLBICUWRBURST => C405PLBICUWRBURST,
      C405PLBICUWRDBUS => C405PLBICUWRDBUS,
      C405PLBICUCACHEABLE => C405PLBICUCACHEABLE,
      PLBC405ICUADDRACK => PLBC405ICUADDRACK,
      PLBC405ICUBUSY => PLBC405ICUBUSY,
      PLBC405ICUERR => PLBC405ICUERR,
      PLBC405ICURDBTERM => PLBC405ICURDBTERM,
      PLBC405ICURDDACK => PLBC405ICURDDACK,
      PLBC405ICURDDBUS => PLBC405ICURDDBUS,
      PLBC405ICURDWDADDR => PLBC405ICURDWDADDR,
      PLBC405ICUREARBITRATE => PLBC405ICUREARBITRATE,
      PLBC405ICUWRBTERM => PLBC405ICUWRBTERM,
      PLBC405ICUWRDACK => PLBC405ICUWRDACK,
      PLBC405ICUSSIZE => PLBC405ICUSSIZE,
      PLBC405ICUSERR => PLBC405ICUSERR,
      PLBC405ICUSBUSYS => PLBC405ICUSBUSYS,
      C405PLBDCUABUS => C405PLBDCUABUS,
      C405PLBDCUBE => C405PLBDCUBE,
      C405PLBDCURNW => C405PLBDCURNW,
      C405PLBDCUABORT => C405PLBDCUABORT,
      C405PLBDCUBUSLOCK => C405PLBDCUBUSLOCK,
      C405PLBDCUU0ATTR => C405PLBDCUU0ATTR,
      C405PLBDCUGUARDED => C405PLBDCUGUARDED,
      C405PLBDCULOCKERR => C405PLBDCULOCKERR,
      C405PLBDCUMSIZE => C405PLBDCUMSIZE,
      C405PLBDCUORDERED => C405PLBDCUORDERED,
      C405PLBDCUPRIORITY => C405PLBDCUPRIORITY,
      C405PLBDCURDBURST => C405PLBDCURDBURST,
      C405PLBDCUREQUEST => C405PLBDCUREQUEST,
      C405PLBDCUSIZE => C405PLBDCUSIZE,
      C405PLBDCUTYPE => C405PLBDCUTYPE,
      C405PLBDCUWRBURST => C405PLBDCUWRBURST,
      C405PLBDCUWRDBUS => C405PLBDCUWRDBUS,
      C405PLBDCUCACHEABLE => C405PLBDCUCACHEABLE,
      C405PLBDCUWRITETHRU => C405PLBDCUWRITETHRU,
      PLBC405DCUADDRACK => PLBC405DCUADDRACK,
      PLBC405DCUBUSY => PLBC405DCUBUSY,
      PLBC405DCUERR => PLBC405DCUERR,
      PLBC405DCURDBTERM => PLBC405DCURDBTERM,
      PLBC405DCURDDACK => PLBC405DCURDDACK,
      PLBC405DCURDDBUS => PLBC405DCURDDBUS,
      PLBC405DCURDWDADDR => PLBC405DCURDWDADDR,
      PLBC405DCUREARBITRATE => PLBC405DCUREARBITRATE,
      PLBC405DCUWRBTERM => PLBC405DCUWRBTERM,
      PLBC405DCUWRDACK => PLBC405DCUWRDACK,
      PLBC405DCUSSIZE => PLBC405DCUSSIZE,
      PLBC405DCUSERR => PLBC405DCUSERR,
      PLBC405DCUSBUSYS => PLBC405DCUSBUSYS,
      BRAMDSOCMCLK => BRAMDSOCMCLK,
      BRAMDSOCMRDDBUS => BRAMDSOCMRDDBUS,
      DSARCVALUE => DSARCVALUE,
      DSCNTLVALUE => DSCNTLVALUE,
      DSOCMBRAMABUS => DSOCMBRAMABUS,
      DSOCMBRAMBYTEWRITE => DSOCMBRAMBYTEWRITE,
      DSOCMBRAMEN => DSOCMBRAMEN,
      DSOCMBRAMWRDBUS => DSOCMBRAMWRDBUS,
      DSOCMBUSY => DSOCMBUSY,
      BRAMISOCMCLK => BRAMISOCMCLK,
      BRAMISOCMRDDBUS => BRAMISOCMRDDBUS,
      ISARCVALUE => ISARCVALUE,
      ISCNTLVALUE => ISCNTLVALUE,
      ISOCMBRAMEN => ISOCMBRAMEN,
      ISOCMBRAMEVENWRITEEN => ISOCMBRAMEVENWRITEEN,
      ISOCMBRAMODDWRITEEN => ISOCMBRAMODDWRITEEN,
      ISOCMBRAMRDABUS => ISOCMBRAMRDABUS,
      ISOCMBRAMWRABUS => ISOCMBRAMWRABUS,
      ISOCMBRAMWRDBUS => ISOCMBRAMWRDBUS,
      C405DCRABUS => C405DCRABUS,
      C405DCRDBUSOUT => C405DCRDBUSOUT,
      C405DCRREAD => C405DCRREAD,
      C405DCRWRITE => C405DCRWRITE,
      DCRC405ACK => DCRC405ACK,
      DCRC405DBUSIN => DCRC405DBUSIN,
      EICC405CRITINPUTIRQ => EICC405CRITINPUTIRQ,
      EICC405EXTINPUTIRQ => EICC405EXTINPUTIRQ,
      C405JTGCAPTUREDR => C405JTGCAPTUREDR,
      C405JTGEXTEST => C405JTGEXTEST,
      C405JTGPGMOUT => C405JTGPGMOUT,
      C405JTGSHIFTDR => C405JTGSHIFTDR,
      C405JTGTDO => C405JTGTDO,
      C405JTGTDOEN => C405JTGTDOEN,
      C405JTGUPDATEDR => C405JTGUPDATEDR,
      MCBJTAGEN => MCBJTAGEN,
      JTGC405BNDSCANTDO => JTGC405BNDSCANTDO,
      JTGC405TCK => JTGC405TCK,
      JTGC405TDI => JTGC405TDI,
      JTGC405TMS => JTGC405TMS,
      JTGC405TRSTNEG => JTGC405TRSTNEG,
      C405DBGMSRWE => C405DBGMSRWE,
      C405DBGSTOPACK => C405DBGSTOPACK,
      C405DBGWBCOMPLETE => C405DBGWBCOMPLETE,
      C405DBGWBFULL => C405DBGWBFULL,
      C405DBGWBIAR => C405DBGWBIAR,
      DBGC405DEBUGHALT => DBGC405DEBUGHALT,
      DBGC405EXTBUSHOLDACK => DBGC405EXTBUSHOLDACK,
      DBGC405UNCONDDEBUGEVENT => DBGC405UNCONDDEBUGEVENT,
      C405TRCCYCLE => C405TRCCYCLE,
      C405TRCEVENEXECUTIONSTATUS => C405TRCEVENEXECUTIONSTATUS,
      C405TRCODDEXECUTIONSTATUS => C405TRCODDEXECUTIONSTATUS,
      C405TRCTRACESTATUS => C405TRCTRACESTATUS,
      C405TRCTRIGGEREVENTOUT => C405TRCTRIGGEREVENTOUT,
      C405TRCTRIGGEREVENTTYPE => C405TRCTRIGGEREVENTTYPE,
      TRCC405TRACEDISABLE => TRCC405TRACEDISABLE,
      TRCC405TRIGGEREVENTIN => TRCC405TRIGGEREVENTIN
    );

end architecture STRUCTURE;

