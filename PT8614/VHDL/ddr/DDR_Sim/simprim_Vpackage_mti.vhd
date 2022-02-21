-- $Header: /usr/local/cvs/s010728/02207/report/fig/simprim_Vpackage_mti.vhd,v 1.1 2007/02/26 09:16:09 s030485 Exp $
----------------------------------------------------------------
-- 
-- Created by the Synopsys Library Compiler v3.4a
-- FILENAME     :    simprim_Vtables.vhd
-- FILE CONTENTS:    VITAL Table Package
-- DATE CREATED :    Tue Mar 26 14:02:56 1996
-- 
-- LIBRARY      :    simprim
-- DATE ENTERED :    Tues Mar 12 16:22:19 1996
-- REVISION     :    1.0.1
-- TECHNOLOGY   :    fpga
-- TIME SCALE   :    1 ns
-- LOGIC SYSTEM :    IEEE-1164
-- NOTES        :    
-- HISTORY      :    1. First created by runnning Synopsys LC V3.4a. DP, 03/26/96.
--                   2. Changed package name from VTABLES to VPACKAGE. DP, 03/28/96.
--                   3. Added HEX_TO_SLV16, HEX_TO_SLV32, DECODE_ADDR4, and
--                      DECODE_ADDR5 function declarations. DP, 03/28/96.
--                   4. Added package body with above functions. DP, 03/28/96.
--                   5. Added RCS header. DP, 04/02/96.
--                   6. Added type definitions for std_logic_vector1, 
--                      std_logic_vector2, and std_logic_vector3: 1, 2, and 3 
--                      dimensional arrays of std_logic. DP, 01/28/97.
--                   7. Added X_LATCHE and X_SFF state tables. DP, 02/27/98.
--                   8. Added declarations and bodies for ADDR_IS_VALID and 
--                      SLV_TO_STR functions, and SET_MEM_TO_X, ADDR_OVERLAP
--                      and COLLISION procedures. DP, 02/27/98.
--                   9. Fixed bug in ADDR_OVERLAP procedure. DP, 04/04/98.
--                   10.Added function SLV_TO_INT in VPACKAGE. SG, 09/18/98.
--                   11.Fixed a bug in SLV_TO_STR, SG, 01/05/99.
--                   12.Changed the State Tables to correctly handle Xconditions,
--                      SG, 03/17/99.
--                   13.Fixed a bug in State tables. SG, 4/21/99.
--                   14.Added std_logic_vector4 type definition. SG, 11/22/99.
-- Starting to write HISTORY from F.19 build.                     
-------------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.all;

library VITAL2000;
use VITAL2000.VITAL_Timing.all;
use VITAL2000.VITAL_Primitives.all;

library STD;
use STD.TEXTIO.all;

package VPACKAGE is
  type SimprimErrorType is (ErrVctLng,
                            ErrNoPath,
                            ErrNegPath,
                            ErrNegDel);

  type SimprimErrorSeverityType is array (SimprimErrorType) of severity_level;
  
  type SimprimGlitchKindType is (OnEvent,
                                 OnDetect,
                                 VitalInertial,
                                 VitalTransport);
  type SimprimGlitchDataType is
    record
      SchedTime  : time;
      GlitchTime : time;
      SchedValue : std_ulogic;
      LastValue  : std_ulogic;
    end record;
  
  type LogicCvtTableType is array (std_ulogic) of character;

  type std_logic_vector1 is array (natural range <>) of std_logic;
  
  type std_logic_vector2 is array (natural range <>, natural range <>) of std_logic;
  
  type std_logic_vector3 is array (natural range <>, natural range <>, natural range <>) of std_logic;
  
  type std_logic_vector4 is array (natural range <>, natural range <>, natural range <>, natural range <>) of std_logic;

  type memory_collision_type is (Read_A_Write_B,
                                 Read_B_Write_A,
                                 Write_A_Write_B);    

  constant X_FF_O_tab : VitalStateTableType := (
--   clk  I    CE   set  rst   O        On
    ('-', '-', '-', '1', '0', '-', '-', '1'),
    ('-', '-', '-', '-', '1', '-', '-', '0'),


    ('/', '0', '1', '0', '0', '-', '-', '0'),
    ('/', '1', '1', '0', '0', '-', '-', '1'),
    ('/', 'X', '1', '0', '0', '-', '-', 'X'),
    ('/', '0', 'X', '0', '0', '0', '-', '0'),
    ('/', '1', 'X', '0', '0', '1', '-', '1'),

    ('/', '0', '1', '0', 'X', '-', '-', '0'),
    ('/', '1', '1', '0', 'X', '-', '-', 'X'),    
    ('/', '1', '1', 'X', '0', '-', '-', '1'),        
    ('/', '0', '1', 'X', '0', '-', '-', 'X'),


    ('-', '-', '-', '0', 'r', '0', '-', '0'),
    ('-', '-', '-', '1', 'r', '1', '-', 'X'),
    ('-', '-', '-', 'X', 'r', '-', '-', 'X'),        
    ('-', '-', '-', '0', 'f', '0', '-', '0'),
    ('-', '-', '-', '1', 'f', '0', '-', 'X'),
    ('-', '-', '-', 'X', 'f', '-', '-', 'X'),        
    ('-', '-', '-', 'r', '0', '1', '-', '1'),
    ('-', '-', '-', 'r', 'X', '-', '-', 'X'),    
    ('-', '-', '-', 'f', '0', '1', '-', '1'),

    ('-', '-', '-', 'X', 'N', '-', '-', 'X'),            

    ('*', '-', '0', '-', '-', '-', '-', 'S'),    
    
    ('P', '1', '-', '-', '0', '1', '-', '1'),
    ('P', '0', '-', '0', '-', '0', '-', '0'),
    ('p', '1', '-', '-', '0', '1', '-', '1'),
    ('p', '0', '-', '0', '-', '0', '-', '0'),

    ('n', '-', '-', '-', '-', '-', '-', 'S'),
    ('N', '-', '-', '-', '-', '-', '-', 'S'),    
                                        
    ('-', '*', '-', '-', '-', '-', '-', 'S'),
    ('-', '-', '*', '-', '-', '-', '-', 'S'),
    ('-', '-', '-', 'N', '-', '-', '-', 'S'),
    ('-', '-', '-', '-', 'N', '-', '-', 'S'));
  constant X_SFF_O_tab : VitalStateTableType := (
--   clk  d    ce   set  rst  sset srst O-p  state O;
    ('-', '-', '-', '1', '0', '-', '-', '-', '-', '1'),
    ('-', '-', '-', '-', '1', '-', '-', '-', '-', '0'),

    ('/', '-', '-', '0', '0', '-', '1', '-', '-', '0'),
    ('/', '-', '-', '0', '0', '1', '0', '-', '-', '1'),
    ('^', '-', '-', '0', '0', '-', '1', '0', '-', '0'),
    ('^', '-', '-', '0', '0', '-', '1', '1', '-', 'X'),    
    ('^', '-', '-', '0', '0', '1', '0', '1', '-', '1'),
    ('^', '-', '-', '0', '0', '1', '0', '0', '-', 'X'),    
    ('r', '-', '-', '0', '0', '-', '1', '0', '-', '0'),
    ('r', '-', '-', '0', '0', '-', '1', '1', '-', 'X'),
    ('r', '-', '-', '0', '0', '1', '0', '1', '-', '1'),
    ('r', '-', '-', '0', '0', '1', '0', '0', '-', 'X'),
                                                  
    ('/', '0', '1', '0', '0', '0', '0', '-', '-', '0'),
    ('/', '1', '1', '0', '0', '0', '0', '-', '-', '1'),
    ('/', 'X', '1', '0', '0', '0', '0', '-', '-', 'X'),
    ('/', '0', 'X', '0', '0', '0', '0', '0', '-', '0'),
    ('/', '1', 'X', '0', '0', '0', '0', '1', '-', '1'),

    ('*', '-', '0', '0', '0', '0', '0', '-', '-', 'S'),
    ('n', '-', '-', '0', '0', '-', '-', '-', '-', 'S'),
    ('N', '-', '-', '0', '0', '-', '-', '-', '-', 'S'),
    
    ('/', '-', '0', '0', '0', '0', 'X', '0', '-', '0'),
    ('/', '0', '1', '0', '0', '0', 'X', '-', '-', '0'),
    ('/', '-', '0', '0', '0', 'X', '0', '1', '-', '1'),
    ('/', '1', '1', '0', '0', 'X', '0', '-', '-', '1'),
                                                  
    ('P', '-', '0', '0', '0', '0', 'X', '0', '-', '0'),
    ('P', '-', '0', '0', '0', 'X', '0', '1', '-', '1'),
    ('p', '-', '0', '0', '0', '0', 'X', '0', '-', '0'),
    ('p', '-', '0', '0', '0', 'X', '0', '1', '-', '1'),
                                                  
    ('/', '0', '-', '0', 'X', '0', '0', '0', '-', '0'),
    ('/', '-', '0', '0', 'X', '0', 'X', '0', '-', '0'),
    ('/', '0', '-', '0', 'X', '0', 'X', '0', '-', '0'),
    ('/', '-', '-', '0', 'X', '-', '1', '-', '-', '0'),
    ('/', '0', '1', '0', 'X', '0', '-', '-', '-', '0'),
                                                  
    ('p', '0', '-', '0', 'X', '0', '0', '0', '-', '0'),
    ('p', '-', '0', '0', 'X', '0', 'X', '0', '-', '0'),
    ('p', '0', '-', '0', 'X', '0', 'X', '0', '-', '0'),
    ('p', '-', '-', '0', 'X', '-', '1', '0', '-', '0'),
    ('P', '0', '-', '0', 'X', '0', '0', '0', '-', '0'),
    ('P', '-', '0', '0', 'X', '0', 'X', '0', '-', '0'),
    ('P', '0', '-', '0', 'X', '0', 'X', '0', '-', '0'),
    ('P', '-', '-', '0', 'X', '-', '1', '0', '-', '0'),
                                                  
    ('/', '1', '-', 'X', '0', '0', '0', '1', '-', '1'),
    ('/', '-', '0', 'X', '0', 'X', '0', '1', '-', '1'),
    ('/', '1', '-', 'X', '0', 'X', '0', '1', '-', '1'),
    ('/', '-', '-', 'X', '0', '1', '0', '-', '-', '1'),
    ('/', '1', '1', 'X', '0', '-', '0', '-', '-', '1'),
                                                  
    ('p', '1', '-', 'X', '0', '0', '0', '1', '-', '1'),
    ('p', '-', '0', 'X', '0', 'X', '0', '1', '-', '1'),
    ('p', '1', '-', 'X', '0', 'X', '0', '1', '-', '1'),
    ('p', '-', '-', 'X', '0', '1', '0', '1', '-', '1'),
    ('P', '1', '-', 'X', '0', '0', '0', '1', '-', '1'),
    ('P', '-', '0', 'X', '0', 'X', '0', '1', '-', '1'),
    ('P', '1', '-', 'X', '0', 'X', '0', '1', '-', '1'),
    ('P', '-', '-', 'X', '0', '1', '0', '1', '-', '1'),
                                                  
    ('-', '-', '-', '0', 'r', '-', '-', '0', '-', '0'),
    ('-', '-', '-', '0', 'f', '-', '-', '0', '-', '0'),    
    ('-', '-', '-', 'r', '0', '-', '-', '1', '-', '1'),
    ('-', '-', '-', 'f', '0', '-', '-', '1', '-', '1'),    
                                                  
    ('P', '1', '-', '-', '0', '-', '0', '1', '-', '1'),
    ('P', '0', '-', '0', '-', '0', '-', '0', '-', '0'),
    ('p', '1', '-', '-', '0', '-', '0', '1', '-', '1'),
    ('p', '0', '-', '0', '-', '0', '-', '0', '-', '0'),


                                                  
    ('-', '*', '-', '-', '-', '-', '-', '-', '-', 'S'),
    ('-', '-', '*', '-', '-', '-', '-', '-', '-', 'S'),
    ('-', '-', '-', 'N', '-', '-', '-', '-', '-', 'S'),
    ('-', '-', '-', 'X', 'N', '-', '-', '-', '-', 'X'),
    ('-', '-', '-', '0', 'N', '-', '-', '-', '-', 'S'),
    ('-', '-', '-', '-', '-', '*', '-', '-', '-', 'S'),
    ('-', '-', '-', '-', '-', '-', '*', '-', '-', 'S'));


  constant X_LATCH_O_tab : VitalStateTableType := (
-- clk I set rst On O
    ('1', '0', '0', '0', '-', '0'),
    ('1', '1', '0', '0', '-', '1'),
    ('0', '-', '0', '0', '-', 'S'),
    ('X', '0', '0', 'X', '0', '-'),
    ('X', '1', '0', 'X', '1', '-'),
    ('-', '-', '1', '0', '-', '1'),
    ('-', '-', '-', '1', '-', '0'),
    ('0', '-', '0', 'X', '0', '0'),
    ('1', '0', '0', 'X', '-', '0'),
    ('0', '-', 'X', '0', '1', '1'),
    ('1', '1', 'X', '0', '-', '1'));

  constant X_LATCHE_O_tab : VitalStateTableType := (
-- Clk I set rst ge On O
    ('1', '0', '0', '0', '1', '-', '0'),
    ('1', '1', '0', '0', '1', '-', '1'),

    ('0', '-', '0', '0', '-', '-', 'S'),
    ('-', '-', '0', '0', '0', '-', 'S'),
    ('-', '0', '0', '-', '-', '0', 'S'),
    ('-', '1', '-', '0', '-', '1', 'S'),

    ('-', '-', '1', '0', '-', '-', '1'),
    ('-', '-', '-', '1', '-', '-', '0'),
    ('0', '-', '0', 'X', '-', '0', '0'),
    ('-', '-', '0', 'X', '0', '0', '0'),
    ('1', '0', '0', 'X', '1', '-', '0'),
    ('0', '-', 'X', '0', '-', '1', '1'),
    ('-', '-', 'X', '0', '0', '1', '1'),
    ('1', '1', 'X', '0', '1', '-', '1'));

-------------------------------------------------------------------------------
-- COOLRUNNER STUFF
-------------------------------------------------------------------------------
  constant X_FDD_O_tab : VitalStateTableType := (
    --   ckp set O   I   CE ck rst   O  On
    ( '-', '-', '-', '-', '-', '-', '1', '-', '0' ),  -- rst
    ( '-', '1', '-', '-', '-', '-', '0', '-', '1' ),  -- set
    ( '0', '0', '-', '0', '1', '1', '-', '-', '0' ),  -- O=I
    ( '0', '-', '-', '1', '1', '1', '0', '-', '1' ),  -- O=I
    ( '1', '0', '-', '0', '1', '0', '-', '-', '0' ),  -- O=I
    ( '1', '-', '-', '1', '1', '0', '0', '-', '1' ),  -- O=I
    ( '0', '0', '-', '-', '-', '0', '0', '-', 'S' ),
    ( '1', '0', '-', '-', '-', '1', '0', '-', 'S' ),
    ( 'X', '0', '-', '-', '-', 'X', '0', '-', 'S' ),  -- clk going 'X' to 'X' outputs 'X' 
    ( '0', '-', '1', '-', '-', '0', '0', '-', 'S' ),
    ( '1', '-', '1', '-', '-', '1', '0', '-', 'S' ),
    ( 'X', '-', '1', '-', '-', 'X', '0', '-', 'S' ),  --   
    ( '0', '0', '0', '-', '-', '0', '-', '-', 'S' ),
    ( '1', '0', '0', '-', '-', '1', '-', '-', 'S' ),
    ( 'X', '0', '0', '-', '-', 'X', '-', '-', 'S' ),  -- 
    ( '-', '0', '-', '-', '0', '-', '0', '-', 'S' ),
    ( '-', '0', '0', '0', '-', '-', '-', '-', 'S' ),
    ( '-', '0', '0', '-', '0', '-', '-', '-', 'S' ),  --
    ( '-', '-', '1', '1', '-', '-', '0', '-', 'S' ),
    ( '-', '-', '1', '-', '0', '-', '0', '-', 'S' ),  --
    ( '1', '0', '-', '-', '-', '0', '0', '-', 'S' ),
    ( 'X', '0', '-', '-', '-', '0', '0', '-', 'S' ),
    ( '1', '0', '-', '-', '-', 'X', '0', '-', 'S' ),  --
    ( '-', '-', '1', '-', '0', '-', '0', '-', 'S' ),
    ( '1', '-', '1', '-', '-', '0', '0', '-', 'S' ),
    ( 'X', '-', '1', '-', '-', '0', '0', '-', 'S' ),
    ( '1', '-', '1', '-', '-', 'X', '0', '-', 'S' ),  --
    ( '-', '0', '0', '-', '0', '-', '-', '-', 'S' ),
    ( '1', '0', '0', '-', '-', '0', '-', '-', 'S' ),
    ( 'X', '0', '0', '-', '-', '0', '-', '-', 'S' ),
    ( '1', '0', '0', '-', '-', 'X', '-', '-', 'S' ));

  constant SimprimErrorSeverity : SimprimErrorSeverityType := (
    ErrVctLng  => error,
    ErrNoPath  => warning,
    ErrNegPath => warning,
    ErrNegDel  => warning);

  constant LogicCvtTable : LogicCvtTableType := ( 'U', 'X', '0', '1', 'Z', 'W', 'L', 'H', '-');

  constant SimprimMsgNoPath : string :=
    "No Delay Path Condition TRUE. 0-delay used. Output signal is               : ";

  constant SimprimMsgNegPath : string :=
    "Path Delay less than time since input. 0 delay used. Output signal is                : ";

  constant SimprimMsgNegDel : string :=
    "Negative delay. New output value not scheduled. Output signal is               : ";

  constant SimprimMsgVctLng : string :=
    "Vector (array) lengths not equal. ";

  constant SimprimMsgUnknown : string :=
    "Unknown error message.";



---------------------------------------------------------------------------
-- Function HEX_TO_SLV16 converts hexadecimal string to std_logic_vector of
-- size 15 downto 0.
---------------------------------------------------------------------------
  function HEX_TO_SLV16 (
    INIT : in string(4 downto 1)
    ) return std_logic_vector;

---------------------------------------------------------------------------
-- Function HEX_TO_SLV32 converts hexadecimal string to std_logic_vector of
-- size 31 downto 0.
---------------------------------------------------------------------------
  function HEX_TO_SLV32 (
    INIT : in string(8 downto 1)
    ) return std_logic_vector;

---------------------------------------------------------------------------
-- Function DECODE_ADDR4 decodes a 4 bit address into an integer ranging
-- from 0 to 16.
---------------------------------------------------------------------------
  function DECODE_ADDR4 (
    ADDRESS : in std_logic_vector(3 downto 0)
    ) return integer;

---------------------------------------------------------------------------
-- Function DECODE_ADDR5 decodes a 5 bit address into an integer ranging
-- from 0 to 32.
---------------------------------------------------------------------------
  function DECODE_ADDR5 (
    ADDRESS : in std_logic_vector(4 downto 0)
    ) return integer;

---------------------------------------------------------------------------
-- Function SLV_TO_INT converts standard logic vector into an integer
---------------------------------------------------------------------------
  function SLV_TO_INT (
    SLV : std_logic_vector
    ) return integer;

---------------------------------------------------------------------------
-- Function ADDR_IS_VALID checks for the validity of the argument. A FALSE
-- is returned if any argument bit is other than a '0' or '1'.
---------------------------------------------------------------------------
  function ADDR_IS_VALID (
    SLV : in std_logic_vector
    ) return boolean;

---------------------------------------------------------------------------
-- Function SLV_TO_STR returns a string version of the std_logic_vector
-- argument.
---------------------------------------------------------------------------
  function SLV_TO_STR (
    SLV : in std_logic_vector
    ) return string;

-------------------------------------------------------------------------------
-- Functions defined for PATHPULSE
-------------------------------------------------------------------------------
  function SimprimMinimum ( constant t1, t2 : in time ) return time;

  function SimprimMaximum ( constant t1, t2 : in time ) return time;

  function SimprimMessage (
    constant ErrorId : in SimprimErrorType
    ) return string;

  ---------------------------------------------------------------------------
  -- Function SLV_TO_HEX returns a string version of the std_logic_vector
  -- argument.
  ---------------------------------------------------------------------------
  function SLV_TO_HEX (
    SLV : in std_logic_vector;
    string_length : in integer    
    ) return string;      

---------------------------------------------------------------------------
-- Procedure SET_MEM_TO_X issues an "invalid address" warning and sets the
-- contents of the argument MEM to 'X'.
---------------------------------------------------------------------------
  procedure SET_MEM_TO_X (
    ADDRESS : in    std_logic_vector;
    MEM     : inout std_logic_vector
    );

---------------------------------------------------------------------------
-- Procedure ADDR_OVERLAP determines if there is overlap between the data
-- addressed by ports A and B of a dual port RAM. If there is overlap, the
-- argument OVERLAP is set to TRUE, and the lower and upper indices of the
-- overlap bits in the array used to model the RAM, as well as in the RAM
-- A and B output ports are determined.
---------------------------------------------------------------------------
  procedure ADDR_OVERLAP (
    ADDRESS_A, ADDRESS_B, DAW, DBW     : in  integer;
    OVERLAP                            : out boolean;
    OVRLAP_LSB, OVRLAP_MSB, DOA_OV_LSB,
    DOA_OV_MSB, DOB_OV_LSB, DOB_OV_MSB : out integer
    );

---------------------------------------------------------------------------
-- Procedure COLLISION issues either a "WRITE COLLISION detected" error or
-- a warning that an attempt was made to read some or all of the bits
-- addressed by one port of a dual port RAM while writing to some or all
-- of the bits from the other port. In case of write collision, some or all
-- of the bits addressed by the port at which the collision is detected are
-- set to 'X'.
---------------------------------------------------------------------------
  procedure COLLISION (
    ADDRESS                          : in    std_logic_vector;
    LSB, MSB                         : in    integer;
    MODE, PORT1, PORT2, InstancePath : in    string;
    MEM                              : inout std_logic_vector
    );

-------------------------------------------------------------------------------
-- Procedures for PATHPULSE
-------------------------------------------------------------------------------
  procedure SimprimError (
    constant Routine : in string;
    constant ErrorId : in SimprimErrorType;
    constant Info    : in string
    ) ;

  procedure SimprimReportGlitch (
    constant GlitchRoutine  : in string;
    constant InSignalName   : in string;
    constant PreemptedTime  : in time;
    constant PreemptedValue : in std_ulogic;
    constant NewTime        : in time;
    constant NewValue       : in std_ulogic;
    constant Index          : in integer        := 0;
    constant IsArraySignal  : in boolean        := false;
    constant MsgSeverity    : in severity_level := warning
    ) ;

  procedure SimprimGlitch (
    variable GlitchOccured        : out   boolean;
    signal   OutSignal            : out   std_logic;
    variable GlitchData           : inout SimprimGlitchDataType;
    constant InSignalName         : in    string;
    constant NewValue             : in    std_logic;
    constant PrevValue            : in    std_logic;
    constant PathpulseTime        : in    time           := 0 ns;
    constant MsgOn                : in    boolean        := false;
    constant MsgSeverity          : in    severity_level := warning
    );
  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN STRING := "";
    Constant Unit : IN STRING := "";
    Constant ExpectedValueMsg : IN STRING := "";
    Constant ExpectedGenericValue : IN STRING := "";
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    );

  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN INTEGER;
    Constant Unit : IN STRING := "";
    Constant ExpectedValueMsg : IN STRING := "";
    Constant ExpectedGenericValue : IN INTEGER;
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    );

  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN BOOLEAN;
    Constant Unit : IN STRING := "";
    Constant ExpectedValueMsg : IN STRING := "";
    Constant ExpectedGenericValue : IN STRING := "";
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    );   

  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN INTEGER;
    CONSTANT Unit : IN STRING := "";
    CONSTANT ExpectedValueMsg : IN STRING := "";
    CONSTANT ExpectedGenericValue : IN STRING := "";
    CONSTANT TailMsg      : IN STRING;                    
    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    );

  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN REAL;
    CONSTANT Unit : IN STRING := "";
    CONSTANT ExpectedValueMsg : IN STRING := "";
    CONSTANT ExpectedGenericValue : IN STRING := "";
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    );

  PROCEDURE Memory_Collision_Msg (
    CONSTANT HeaderMsg      : IN STRING := " Memory Collision Error on ";        
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    constant collision_type : in memory_collision_type;
    constant address_a : in std_logic_vector; 
    constant address_b : in std_logic_vector; 
    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := ERROR
    
    
    );       

--  signal X_GSR_GLOBAL_SIGNAL : std_logic;
--  signal X_GTS_GLOBAL_SIGNAL : std_logic;

  procedure detect_resolution (
    constant model_name : in string
    );        

end VPACKAGE;

package body VPACKAGE is
---------------------------------------------------------------------------
-- Function SLV_TO_INT converts a std_logic_vector TO INTEGER
---------------------------------------------------------------------------
  function SLV_TO_INT(SLV : std_logic_vector
                      ) return integer is

    variable int : integer;
    
  begin
    int     := 0;
    for i in SLV'high downto SLV'low loop
      int   := int * 2;
      if SLV(i) = '1' then
        int := int + 1;
      end if;
    end loop;
    return int;
  end;

---------------------------------------------------------------------------
-- Function HEX_TO_SLV16 converts hexadecimal string to std_logic_vector of
-- size 15 downto 0.
---------------------------------------------------------------------------
  function HEX_TO_SLV16 (
    INIT : in string(4 downto 1)
    ) return std_logic_vector is

    variable SLV_16 : std_logic_vector(15 downto 0);

  begin
    for I in 0 to 3 loop
      case INIT(I+1) is
        when '0'       =>
          SLV_16(I*4+3 downto I*4) := "0000";
        when '1'       =>
          SLV_16(I*4+3 downto I*4) := "0001";
        when '2'       =>
          SLV_16(I*4+3 downto I*4) := "0010";
        when '3'       =>
          SLV_16(I*4+3 downto I*4) := "0011";
        when '4'       =>
          SLV_16(I*4+3 downto I*4) := "0100";
        when '5'       =>
          SLV_16(I*4+3 downto I*4) := "0101";
        when '6'       =>
          SLV_16(I*4+3 downto I*4) := "0110";
        when '7'       =>
          SLV_16(I*4+3 downto I*4) := "0111";
        when '8'       =>
          SLV_16(I*4+3 downto I*4) := "1000";
        when '9'       =>
          SLV_16(I*4+3 downto I*4) := "1001";
        when 'a' | 'A' =>
          SLV_16(I*4+3 downto I*4) := "1010";
        when 'b' | 'B' =>
          SLV_16(I*4+3 downto I*4) := "1011";
        when 'c' | 'C' =>
          SLV_16(I*4+3 downto I*4) := "1100";
        when 'd' | 'D' =>
          SLV_16(I*4+3 downto I*4) := "1101";
        when 'e' | 'E' =>
          SLV_16(I*4+3 downto I*4) := "1110";
        when 'f' | 'F' =>
          SLV_16(I*4+3 downto I*4) := "1111";
        when others    =>
          assert false
            report "WARNING : Unknown Hex digit in INIT : " &INIT(I+1)
            severity warning;
          SLV_16(I*4+3 downto I*4) := "XXXX";
      end case;
    end loop;
    return SLV_16;
  end HEX_TO_SLV16;

---------------------------------------------------------------------------
-- Function HEX_TO_SLV32 converts hexadecimal string to std_logic_vector of
-- size 31 downto 0.
---------------------------------------------------------------------------
  function HEX_TO_SLV32 (
    INIT : in string(8 downto 1)
    ) return std_logic_vector is

    variable SLV_32 : std_logic_vector(31 downto 0);

  begin
    for I in 0 to 7 loop
      case INIT(I+1) is
        when '0'       =>
          SLV_32(I*4+3 downto I*4) := "0000";
        when '1'       =>
          SLV_32(I*4+3 downto I*4) := "0001";
        when '2'       =>
          SLV_32(I*4+3 downto I*4) := "0010";
        when '3'       =>
          SLV_32(I*4+3 downto I*4) := "0011";
        when '4'       =>
          SLV_32(I*4+3 downto I*4) := "0100";
        when '5'       =>
          SLV_32(I*4+3 downto I*4) := "0101";
        when '6'       =>
          SLV_32(I*4+3 downto I*4) := "0110";
        when '7'       =>
          SLV_32(I*4+3 downto I*4) := "0111";
        when '8'       =>
          SLV_32(I*4+3 downto I*4) := "1000";
        when '9'       =>
          SLV_32(I*4+3 downto I*4) := "1001";
        when 'a' | 'A' =>
          SLV_32(I*4+3 downto I*4) := "1010";
        when 'b' | 'B' =>
          SLV_32(I*4+3 downto I*4) := "1011";
        when 'c' | 'C' =>
          SLV_32(I*4+3 downto I*4) := "1100";
        when 'd' | 'D' =>
          SLV_32(I*4+3 downto I*4) := "1101";
        when 'e' | 'E' =>
          SLV_32(I*4+3 downto I*4) := "1110";
        when 'f' | 'F' =>
          SLV_32(I*4+3 downto I*4) := "1111";
        when others    =>
          assert false
            report "WARNING : Unknown Hex digit in INIT : "&INIT(I+1)
            severity warning;
          SLV_32(I*4+3 downto I*4) := "XXXX";
      end case;
    end loop;
    return SLV_32;
  end HEX_TO_SLV32;

---------------------------------------------------------------------------
-- Function DECODE_ADDR4 decodes a 4 bit address into an integer ranging
-- from 0 to 16.
---------------------------------------------------------------------------
  function DECODE_ADDR4 (
    ADDRESS : in std_logic_vector(3 downto 0)
    ) return integer is

    variable I : integer;

  begin
    case ADDRESS is
      when "0000" => I := 0;
      when "0001" => I := 1;
      when "0010" => I := 2;
      when "0011" => I := 3;
      when "0100" => I := 4;
      when "0101" => I := 5;
      when "0110" => I := 6;
      when "0111" => I := 7;
      when "1000" => I := 8;
      when "1001" => I := 9;
      when "1010" => I := 10;
      when "1011" => I := 11;
      when "1100" => I := 12;
      when "1101" => I := 13;
      when "1110" => I := 14;
      when "1111" => I := 15;
      when others => I := 16;
    end case;
    return I;
  end DECODE_ADDR4;

---------------------------------------------------------------------------
-- Function DECODE_ADDR5 decodes a 5 bit address into an integer ranging
-- from 0 to 32.
---------------------------------------------------------------------------
  function DECODE_ADDR5 (
    ADDRESS : in std_logic_vector(4 downto 0)
    ) return integer is

    variable I : integer;

  begin
    case ADDRESS is
      when "00000" => I := 0;
      when "00001" => I := 1;
      when "00010" => I := 2;
      when "00011" => I := 3;
      when "00100" => I := 4;
      when "00101" => I := 5;
      when "00110" => I := 6;
      when "00111" => I := 7;
      when "01000" => I := 8;
      when "01001" => I := 9;
      when "01010" => I := 10;
      when "01011" => I := 11;
      when "01100" => I := 12;
      when "01101" => I := 13;
      when "01110" => I := 14;
      when "01111" => I := 15;
      when "10000" => I := 16;
      when "10001" => I := 17;
      when "10010" => I := 18;
      when "10011" => I := 19;
      when "10100" => I := 20;
      when "10101" => I := 21;
      when "10110" => I := 22;
      when "10111" => I := 23;
      when "11000" => I := 24;
      when "11001" => I := 25;
      when "11010" => I := 26;
      when "11011" => I := 27;
      when "11100" => I := 28;
      when "11101" => I := 29;
      when "11110" => I := 30;
      when "11111" => I := 31;
      when others  => I  := 32;
    end case;
    return I;
  end DECODE_ADDR5;

---------------------------------------------------------------------------
-- Function ADDR_IS_VALID checks for the validity of the argument. A FALSE
-- is returned if any argument bit is other than a '0' or '1'.
---------------------------------------------------------------------------
  function ADDR_IS_VALID (
    SLV : in std_logic_vector
    ) return boolean is

    variable IS_VALID : boolean := true;

  begin
    for I in SLV'high downto SLV'low loop
      if (SLV(I) /= '0' and SLV(I) /= '1') then
        IS_VALID := false;
      end if;
    end loop;
    return IS_VALID;
  end ADDR_IS_VALID;

---------------------------------------------------------------------------
-- Function SLV_TO_STR returns a string version of the std_logic_vector
-- argument.
---------------------------------------------------------------------------
  function SLV_TO_STR (
    SLV : in std_logic_vector
    ) return string is

    variable J   : integer := SLV'length;
    variable STR : string (SLV'length downto 1);

  begin
    for I in SLV'high downto SLV'low loop
      case SLV(I) is
        when '0'    => STR(J)    := '0';
        when '1'    => STR(J)    := '1';
        when 'X'    => STR(J)    := 'X';
        when 'U'    => STR(J)    := 'U';
        when others => STR(J) := 'X';
      end case;
      J                       := J - 1;
    end loop;
    return STR;
  end SLV_TO_STR;



-------------------------------------------------------------------------------
--Functions for PATHPULSE
-------------------------------------------------------------------------------
  function SimprimMinimum ( constant t1, t2 : in time ) return time is

  begin
    if ( t1 < t2 ) then return (t1); else return (t2); end if;
  end SimprimMinimum;

  function SimprimMaximum ( constant t1, t2 : in time ) return time is

  begin
    if ( t1 > t2 ) then return (t1); else return (t2); end if;
  end SimprimMaximum;

  function SimprimMessage (
    constant ErrorId : in SimprimErrorType
    ) return string is

  begin
    case ErrorId is
      when ErrVctLng  => return SimprimMsgVctLng;
      when ErrNoPath  => return SimprimMsgNoPath;
      when ErrNegPath => return SimprimMsgNegPath;
      when ErrNegDel  => return SimprimMsgNegDel;
      when others     => return SimprimMsgUnknown;
    end case;
  end;

  ---------------------------------------------------------------------------
  -- Function SLV_TO_HEX returns a hex string version of the std_logic_vector
  -- argument.
  ---------------------------------------------------------------------------
  function SLV_TO_HEX (
    SLV : in std_logic_vector;
    string_length : in integer
    ) return string is

    variable i : integer := 1;
    variable j : integer := 1;
    variable STR : string(string_length downto 1);
    variable nibble : std_logic_vector(3 downto 0) := "0000";
    variable full_nibble_count : integer := 0;
    variable remaining_bits : integer := 0;


  begin
    full_nibble_count := SLV'length/4;
    remaining_bits := SLV'length mod 4;
    for i in 1 to full_nibble_count loop
      nibble := SLV(((4*i) - 1) downto ((4*i) - 4));
      if (nibble = "0000")  then
        STR(j) := '0';
      elsif (nibble = "0001")  then
        STR(j) := '1';
      elsif (nibble = "0010")  then
        STR(j) := '2';
      elsif (nibble = "0011")  then
        STR(j) := '3';
      elsif (nibble = "0100")  then
        STR(j) := '4';
      elsif (nibble = "0101")  then
        STR(j) := '5';
      elsif (nibble = "0110")  then
        STR(j) := '6';
      elsif (nibble = "0111")  then
        STR(j) := '7';
      elsif (nibble = "1000")  then
        STR(j) := '8';
      elsif (nibble = "1001")  then
        STR(j) := '9';
      elsif (nibble = "1010")  then
        STR(j) := 'a';
      elsif (nibble = "1011")  then
        STR(j) := 'b';
      elsif (nibble = "1100")  then
        STR(j) := 'c';
      elsif (nibble = "1101")  then
        STR(j) := 'd';
      elsif (nibble = "1110")  then
        STR(j) := 'e';
      elsif (nibble = "1111")  then
        STR(j) := 'f';          
      end if;
      j := j + 1;
    end loop;
    
    if (remaining_bits /= 0) then
      nibble := "0000";
      nibble((remaining_bits -1) downto 0) := SLV((SLV'length -1) downto (SLV'length - remaining_bits));
      if (nibble = "0000")  then
        STR(j) := '0';
      elsif (nibble = "0001")  then
        STR(j) := '1';
      elsif (nibble = "0010")  then
        STR(j) := '2';
      elsif (nibble = "0011")  then
        STR(j) := '3';
      elsif (nibble = "0100")  then
        STR(j) := '4';
      elsif (nibble = "0101")  then
        STR(j) := '5';
      elsif (nibble = "0110")  then
        STR(j) := '6';
      elsif (nibble = "0111")  then
        STR(j) := '7';
      elsif (nibble = "1000")  then
        STR(j) := '8';
      elsif (nibble = "1001")  then
        STR(j) := '9';
      elsif (nibble = "1010")  then
        STR(j) := 'a';
      elsif (nibble = "1011")  then
        STR(j) := 'b';
      elsif (nibble = "1100")  then
        STR(j) := 'c';
      elsif (nibble = "1101")  then
        STR(j) := 'd';
      elsif (nibble = "1110")  then
        STR(j) := 'e';
      elsif (nibble = "1111")  then
        STR(j) := 'f';          
      end if;
    end if;    
    return STR;
  end SLV_TO_HEX;  
  

---------------------------------------------------------------------------
-- Procedure SET_MEM_TO_X issues an "invalid address" warning and sets the
-- contents of the argument MEM to 'X'.
---------------------------------------------------------------------------
  procedure SET_MEM_TO_X (ADDRESS : in    std_logic_vector;
                          MEM     : inout std_logic_vector
                          ) is

  begin
    assert false report
      "Invalid ADDRESS : "& SLV_TO_STR(ADDRESS) & ". Memory contents will be set to 'X'."
      severity warning;
    for I in MEM'high downto MEM'low loop
      MEM(I) := 'X';
    end loop;
  end SET_MEM_TO_X;

---------------------------------------------------------------------------
-- Procedure ADDR_OVERLAP determines if there is overlap between the data
-- addressed by ports A and B of a dual port RAM. If there is overlap, the
-- argument OVERLAP is set to TRUE, and the lower and upper indices of the
-- overlap bits in the array used to model the RAM, as well as in the RAM
-- A and B output ports are determined.
---------------------------------------------------------------------------
  procedure ADDR_OVERLAP (
    ADDRESS_A, ADDRESS_B, DAW, DBW     : in  integer;
    OVERLAP                            : out boolean;
    OVRLAP_LSB, OVRLAP_MSB, DOA_OV_LSB,
    DOA_OV_MSB, DOB_OV_LSB, DOB_OV_MSB : out integer
    ) is

    variable A_LSB, A_MSB, B_LSB, B_MSB : integer;

  begin
    A_LSB := ADDRESS_A * DAW;
    A_MSB := A_LSB + DAW - 1;
    B_LSB := ADDRESS_B * DBW;
    B_MSB := B_LSB + DBW - 1;

    if (A_MSB < B_LSB or B_MSB < A_LSB) then
      OVERLAP      := false;
    else
      OVERLAP      := true;
      if (A_LSB >= B_LSB) then
        OVRLAP_LSB := A_LSB;
        DOA_OV_LSB := 0;
        DOB_OV_LSB := A_LSB - B_LSB;
      else
        OVRLAP_LSB := B_LSB;
        DOA_OV_LSB := B_LSB - A_LSB;
        DOB_OV_LSB := 0;
      end if;
      if (A_MSB >= B_MSB) then
        OVRLAP_MSB := B_MSB;
        DOA_OV_MSB := DAW - (A_MSB - B_MSB) - 1;
        DOB_OV_MSB := DBW - 1;
      else
        OVRLAP_MSB := A_MSB;
        DOA_OV_MSB := DAW - 1;
        DOB_OV_MSB := DBW - (B_MSB - A_MSB) - 1;
      end if;
    end if;
  end ADDR_OVERLAP;

---------------------------------------------------------------------------
-- Procedure COLLISION issues either a "WRITE COLLISION detected" error or
-- a warning that an attempt was made to read some or all of the bits
-- addressed by one port of a dual port RAM while writing to some or all
-- of the bits from the other port. In case of write collision, some or all
-- of the bits addressed by the port at which the collision is detected are
-- set to 'X'.
---------------------------------------------------------------------------
  procedure COLLISION (
    ADDRESS                          : in    std_logic_vector;
    LSB, MSB                         : in    integer;
    MODE, PORT1, PORT2, InstancePath : in    string;
    MEM                              : inout std_logic_vector
    ) is

  begin
    if (MODE = "write") then
      assert false report
        "WRITE COLLISION detected at " & PORT1 & " in instance " & InstancePath &
        ". Contents of address "& SLV_TO_STR(ADDRESS) &
        " will be wholly or partially set to 'X'."
        severity warning;
      for I in MSB downto LSB loop
        MEM(I) := 'X';
      end loop;
    elsif (MODE = "read") then
      assert false report
        "Attempting to read some or all of contents of address "& SLV_TO_STR(ADDRESS) &
        " from " & PORT2 & " while writing from " & PORT1 &
        " in instance " & InstancePath
        severity warning;
    end if;
  end COLLISION;

-------------------------------------------------------------------------------
-- Procedures for PATHPULSE
-------------------------------------------------------------------------------
  procedure SimprimError (
    constant Routine     : in string;
    constant ErrorId     : in SimprimErrorType;
    constant Info        : in string
    ) is

  begin
    assert false
      report Routine & " :    " & SimprimMessage(ErrorId) & Info
      severity SimprimErrorSeverity(ErrorId);
  end;

  procedure SimprimReportGlitch (
    constant GlitchRoutine  : in string;
    constant InSignalName   : in string;
    constant PreemptedTime  : in time;
    constant PreemptedValue : in std_ulogic;
    constant NewTime        : in time;
    constant NewValue       : in std_ulogic;
    constant Index          : in integer        := 0;
    constant IsArraySignal  : in boolean        := false;
    constant MsgSeverity    : in severity_level := warning
    ) is

    variable StrPtr1, StrPtr2, StrPtr3, StrPtr4, StrPtr5 : line;

  begin
    Write (StrPtr1, PreemptedTime );
    Write (StrPtr2, NewTime);
    Write (StrPtr3, LogicCvtTable(PreemptedValue));
    Write (StrPtr4, LogicCvtTable(NewValue));
    if IsArraySignal then
      Write (StrPtr5, string'( "(" ) );
      Write (StrPtr5, Index);
      Write (StrPtr5, string'( ")" ) );
    else
      Write (StrPtr5, string'( " " ) );
    end if;

    -- Issue Report only if Preempted value has not been
    --  removed from event queue
    assert PreemptedTime > NewTime
      report GlitchRoutine & " : GLITCH Detected on port " &
      InSignalName & StrPtr5.all &
      "; Preempted Future Value := " & StrPtr3.all &
      " @ " & StrPtr1.all &
      "; Newly Scheduled Value  := " & StrPtr4.all &
      " @ " & StrPtr2.all &
      ";"
      severity MsgSeverity;

    DEALLOCATE(StrPtr1);
    DEALLOCATE(StrPtr2);
    DEALLOCATE(StrPtr3);
    DEALLOCATE(StrPtr4);
    DEALLOCATE(StrPtr5);
    return;
  end SimprimReportGlitch;

  procedure SimprimGlitch (
    variable GlitchOccured : out   boolean;
    signal   OutSignal     : out   std_logic;
    variable GlitchData    : inout SimprimGlitchDataType;
    constant InSignalName  : in    string;
    constant NewValue      : in    std_logic;
    constant PrevValue     : in    std_logic;
    constant PathpulseTime : in    time           := 0 ns;
    constant MsgOn         : in    boolean        := false;
    constant MsgSeverity   : in    severity_level := warning
    ) is

    variable NewGlitch : boolean := true;
    variable dly       : time    := PathpulseTime;

  begin
--This condition occurs when the new event happens on the input after the 1st event
--has matured.
    if GlitchData.GlitchTime <= NOW then
      -- Note: NewValue is always /= OldValue when called from VPPD 
      if (NewValue = GlitchData.SchedValue) then
        return;
      end if;
      -- No new glitch, save time for possable future glitch
      NewGlitch             := false;
      GlitchData.GlitchTime := NOW+dly;
      -- Transaction currently scheduled (no glitch if same value)
    elsif (GlitchData.SchedValue = NewValue) then
      NewGlitch   := false;
      -- Transaction currently scheduled represents a glitch
    else
      -- A new glitch has been detected
      NewGlitch   := true;
    end if;
    GlitchOccured := NewGlitch;
    if NewGlitch then
      -- If  messages requested, report the glitch
      if MsgOn then
        SimprimReportGlitch ("SimprimGlitch", InSignalName, GlitchData.GlitchTime, GlitchData.SchedValue, (dly + NOW), NewValue, MsgSeverity => MsgSeverity );
      end if;
      -- Force immediate glitch 
      GlitchData.GlitchTime := NOW;
      OutSignal <= PrevValue after (GlitchData.GlitchTime - NOW);
    end if;
    -- Record the new value and time just scheduled.
    GlitchData.SchedValue := NewValue;
-- GlitchData.SchedTime := NOW+dly;
    return;
  end;

  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN STRING := "";
    Constant Unit : IN STRING := "";
    Constant ExpectedValueMsg : IN STRING := "";
    Constant ExpectedGenericValue : IN STRING := "";
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    ) IS
    VARIABLE Message : LINE;
  BEGIN

    Write ( Message, HeaderMsg );
    Write ( Message, STRING'(" The attribute ") );                
    Write ( Message, GenericName );
    Write ( Message, STRING'(" on ") );
    Write ( Message, EntityName );
    Write ( Message, STRING'(" instance ") );
    Write ( Message, InstanceName );
    Write ( Message, STRING'(" is set to  ") );        
    Write ( Message, GenericValue );
    Write ( Message, Unit );        
    Write ( Message, '.' & LF );
    Write ( Message, ExpectedValueMsg );
    Write ( Message, ExpectedGenericValue );        
    Write ( Message, Unit );
    Write ( Message, TailMsg );

    ASSERT FALSE REPORT Message.ALL SEVERITY MsgSeverity;

    DEALLOCATE (Message);
  END GenericValueCheckMessage;

  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN INTEGER;
    CONSTANT Unit : IN STRING := "";
    CONSTANT ExpectedValueMsg : IN STRING := "";
    CONSTANT ExpectedGenericValue : IN INTEGER;
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    ) IS
    VARIABLE Message : LINE;
  BEGIN

    Write ( Message, HeaderMsg );
    Write ( Message, STRING'(" The attribute ") );                
    Write ( Message, GenericName );
    Write ( Message, STRING'(" on ") );
    Write ( Message, EntityName );
    Write ( Message, STRING'(" instance ") );
    Write ( Message, InstanceName );
    Write ( Message, STRING'(" is set to  ") );        
    Write ( Message, GenericValue );
    Write ( Message, Unit );        
    Write ( Message, '.' & LF );
    Write ( Message, ExpectedValueMsg );
    Write ( Message, ExpectedGenericValue );        
    Write ( Message, Unit );
    Write ( Message, TailMsg );        

    ASSERT FALSE REPORT Message.ALL SEVERITY MsgSeverity;

    DEALLOCATE (Message);
  END GenericValueCheckMessage;

  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN BOOLEAN;
    Constant Unit : IN STRING := "";
    CONSTANT ExpectedValueMsg : IN STRING := "";
    CONSTANT ExpectedGenericValue : IN STRING := "";
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    ) IS
    VARIABLE Message : LINE;
  BEGIN

    Write ( Message, HeaderMsg );
    Write ( Message, STRING'(" The attribute ") );                
    Write ( Message, GenericName );
    Write ( Message, STRING'(" on ") );
    Write ( Message, EntityName );
    Write ( Message, STRING'(" instance ") );
    Write ( Message, InstanceName );
    Write ( Message, STRING'(" is set to  ") );        
    Write ( Message, GenericValue );
    Write ( Message, Unit );        
    Write ( Message, '.' & LF );
    Write ( Message, ExpectedValueMsg );
    Write ( Message, ExpectedGenericValue );        
    Write ( Message, Unit );
    Write ( Message, TailMsg );        

    ASSERT FALSE REPORT Message.ALL SEVERITY MsgSeverity;

    DEALLOCATE (Message);
  END GenericValueCheckMessage;

  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN INTEGER;
    CONSTANT Unit : IN STRING := "";
    CONSTANT ExpectedValueMsg : IN STRING := "";
    CONSTANT ExpectedGenericValue : IN STRING := "";
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    ) IS
    VARIABLE Message : LINE;
  BEGIN

    Write ( Message, HeaderMsg );
    Write ( Message, STRING'(" The attribute ") );                
    Write ( Message, GenericName );
    Write ( Message, STRING'(" on ") );
    Write ( Message, EntityName );
    Write ( Message, STRING'(" instance ") );
    Write ( Message, InstanceName );
    Write ( Message, STRING'(" is set to  ") );        
    Write ( Message, GenericValue );
    Write ( Message, Unit );        
    Write ( Message, '.' & LF );
    Write ( Message, ExpectedValueMsg );
    Write ( Message, ExpectedGenericValue );        
    Write ( Message, Unit );
    Write ( Message, TailMsg );        

    ASSERT FALSE REPORT Message.ALL SEVERITY MsgSeverity;

    DEALLOCATE (Message);
  END GenericValueCheckMessage;
  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN REAL;
    CONSTANT Unit : IN STRING := "";
    CONSTANT ExpectedValueMsg : IN STRING := "";
    CONSTANT ExpectedGenericValue : IN STRING := "";
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    ) IS
    VARIABLE Message : LINE;
  BEGIN

    Write ( Message, HeaderMsg );
    Write ( Message, STRING'(" The attribute ") );                
    Write ( Message, GenericName );
    Write ( Message, STRING'(" on ") );
    Write ( Message, EntityName );
    Write ( Message, STRING'(" instance ") );
    Write ( Message, InstanceName );
    Write ( Message, STRING'(" is set to  ") );        
    Write ( Message, GenericValue );
    Write ( Message, Unit );        
    Write ( Message, '.' & LF );
    Write ( Message, ExpectedValueMsg );
    Write ( Message, ExpectedGenericValue );        
    Write ( Message, Unit );
    Write ( Message, TailMsg );        

    ASSERT FALSE REPORT Message.ALL SEVERITY MsgSeverity;

    DEALLOCATE (Message);
  END GenericValueCheckMessage;

  PROCEDURE Memory_Collision_Msg (
    CONSTANT HeaderMsg      : IN STRING := " Memory Collision Error on ";        
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    constant collision_type : in memory_collision_type;
    constant address_a : in std_logic_vector; 
    constant address_b : in std_logic_vector; 
    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := ERROR
    
    
    ) IS
    variable current_time : time := NOW;
    variable string_length_a : integer;
    variable string_length_b : integer;    

    VARIABLE Message : LINE;
  BEGIN
    if ((address_a'length mod 4) = 0) then
      string_length_a := address_a'length/4;
    elsif ((address_a'length mod 4) > 0) then
      string_length_a := address_a'length/4 + 1;      
    end if;
    if ((address_b'length mod 4) = 0) then
      string_length_b := address_b'length/4;
    elsif ((address_b'length mod 4) > 0) then
      string_length_b := address_b'length/4 + 1;      
    end if;    
    if (collision_type = Read_A_Write_B) then
      Write ( Message, HeaderMsg);
      Write ( Message, EntityName);    
      Write ( Message, STRING'(": "));
      Write ( Message, InstanceName);
      Write ( Message, STRING'(" at simulation time "));
      Write ( Message, current_time);
      Write ( Message, STRING'("."));
      Write ( Message, LF );            
      Write ( Message, STRING'(" A read was performed on address "));
      Write ( Message, SLV_TO_HEX(address_a, string_length_a));
      Write ( Message, STRING'(" (hex) "));            
      Write ( Message, STRING'("of port A while a write was requested to the same address on Port B "));
      Write ( Message, STRING'(" The write will be successful however the read value is unknown until the next CLKA cycle  "));

    elsif (collision_type = Read_B_Write_A) then
      Write ( Message, HeaderMsg);
      Write ( Message, EntityName);    
      Write ( Message, STRING'(": "));
      Write ( Message, InstanceName);
      Write ( Message, STRING'(" at simulation time "));
      Write ( Message, current_time);
      Write ( Message, STRING'("."));
      Write ( Message, LF );            
      Write ( Message, STRING'(" A read was performed on address "));
      Write ( Message, SLV_TO_HEX(address_b, string_length_b));
      Write ( Message, STRING'(" (hex) "));            
      Write ( Message, STRING'("of port B while a write was requested to the same address on Port A "));
      Write ( Message, STRING'(" The write will be successful however the read value is unknown until the next CLKB cycle  "));
      
    elsif (collision_type = Write_A_Write_B) then
      Write ( Message, HeaderMsg);
      Write ( Message, EntityName);    
      Write ( Message, STRING'(": "));
      Write ( Message, InstanceName);
      Write ( Message, STRING'(" at simulation time "));
      Write ( Message, current_time);
      Write ( Message, STRING'("."));
      Write ( Message, LF );            
      Write ( Message, STRING'(" A write was requested to the same address simultaneously at both Port A and Port B of the RAM."));
      Write ( Message, STRING'(" The contents written to the RAM at address location "));      
      Write ( Message, SLV_TO_HEX(address_a, string_length_a));
      Write ( Message, STRING'(" (hex) "));            
      Write ( Message, STRING'("of Port A and address location "));
      Write ( Message, SLV_TO_HEX(address_b, string_length_b));
      Write ( Message, STRING'(" (hex) "));            
      Write ( Message, STRING'("of Port B are unknown. "));
      
    end if;      
    ASSERT FALSE REPORT Message.ALL SEVERITY MsgSeverity;

    DEALLOCATE (Message);
  END Memory_Collision_Msg;      
  
  procedure detect_resolution (
    constant model_name : in string
    ) IS
    
    variable test_value : time;
    variable Message : LINE;
  BEGIN
    test_value := 1 ps;
    if (test_value = 0 ps) then
      Write (Message, STRING'(" Simulator Resolution Error : "));
      Write (Message, STRING'(" Simulator resolution is set to a value greater than 1 ps. "));
      Write (Message, STRING'(" In order to simulate the "));
      Write (Message, model_name);
      Write (Message, STRING'(", the simulator resolution must be set to 1ps or smaller "));
      ASSERT FALSE REPORT Message.ALL SEVERITY ERROR;
      DEALLOCATE (Message);      
    end if;
  END detect_resolution;      
  
  
end VPACKAGE;
