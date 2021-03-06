[COREGEN.VERILOG Component Instantiation.cgcount]
text000=" "
text001=" "
text002="// The following must be inserted into your Verilog file for this"
text003="// core to be instantiated. Change the instance name and port connections"
text004="// (in parentheses) to your own signal names."
text005=" "
text006="cgcount YourInstanceName ("
text007="    .Q(Q),"
text008="    .CLK(CLK),"
text009="    .Q_THRESH1(Q_THRESH1));"
text010=""
text011=" "
type=template
[COREGEN.VHDL Component Instantiation.cgcount]
text000=" "
text001=" "
text002="-- The following code must appear in the VHDL architecture header:"
text003=" "
text004="component cgcount"
text005="    port ("
text006="    Q: OUT std_logic_VECTOR(22 downto 0);"
text007="    CLK: IN std_logic;"
text008="    Q_THRESH1: OUT std_logic);"
text009="end component;"
text010=""
text011=""
text012=""
text013=" "
text014="-------------------------------------------------------------"
text015=" "
text016="-- The following code must appear in the VHDL architecture body."
text017="-- Substitute your own instance name and net names."
text018=" "
text019="your_instance_name : cgcount"
text020="        port map ("
text021="            Q => Q,"
text022="            CLK => CLK,"
text023="            Q_THRESH1 => Q_THRESH1);"
text024=" "
type=template
