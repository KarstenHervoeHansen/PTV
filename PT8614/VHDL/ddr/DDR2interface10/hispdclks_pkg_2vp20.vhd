-------------------------------------------------------------------------------
-- hispdclks_pkg_2vp20.vhd - Module
-------------------------------------------------------------------------------
--**************************************************************************
-- 
--     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
--     SOLELY FOR USE IN DEVELOPING PROGRAMS AND SOLUTIONS FOR
--     XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION
--     AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, APPLICATION
--     OR STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS
--     IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
--     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
--     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
--     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
--     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
--     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
--     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
--     FOR A PARTICULAR PURPOSE.
--
--     (c) Copyright 2003 Xilinx, Inc.
--     All rights reserved.
--
--**************************************************************************
-------------------------------------------------------------------------------
-- Filename:        hispdclks_pkg_2vp20.vhd
-------------------------------------------------------------------------------
-- Description:
-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------
-- Owners:          Kraig Lund
-- Revision 1.0
--
--     Modification History:                                                   
--     Date       Init        Description                                
--   --------   ------  -------------------------------------------------------
--   10/30/2003   KL      Initial Release 
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

package HISPDCLKS_PKG_2vp20 is

attribute BOX_TYPE : string;

COMPONENT dcmx0y0_2vp20
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END COMPONENT;
attribute BOX_TYPE of dcmx0y0_2vp20 : component is "BLACK_BOX";

COMPONENT dcmx1y0_2vp20
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END COMPONENT;
attribute BOX_TYPE of dcmx1y0_2vp20 : component is "BLACK_BOX";

COMPONENT dcmx2y0_2vp20
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END COMPONENT;
attribute BOX_TYPE of dcmx2y0_2vp20 : component is "BLACK_BOX";

COMPONENT dcmx3y0_2vp20
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END COMPONENT;
attribute BOX_TYPE of dcmx3y0_2vp20 : component is "BLACK_BOX";

COMPONENT dcmx0y1_2vp20
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END COMPONENT;
attribute BOX_TYPE of dcmx0y1_2vp20 : component is "BLACK_BOX";

COMPONENT dcmx1y1_2vp20
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END COMPONENT;
attribute BOX_TYPE of dcmx1y1_2vp20 : component is "BLACK_BOX";

COMPONENT dcmx2y1_2vp20
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END COMPONENT;
attribute BOX_TYPE of dcmx2y1_2vp20 : component is "BLACK_BOX";

COMPONENT dcmx3y1_2vp20
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END COMPONENT;
attribute BOX_TYPE of dcmx3y1_2vp20 : component is "BLACK_BOX";

end HISPDCLKS_PKG_2vp20;
