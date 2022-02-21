-------------------------------------------------------------------------------
-- hispdclks.vhd - Module
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
-- Filename:        hispdclks.vhd
-------------------------------------------------------------------------------
-- Description:
--      This module contains feedthrough modules for functional simulation
-- purposes only. This module can be included in a compile script to be used
-- for simulating Virtex-II Pro double data rate (DDR) designs using local
-- routing clocking between the DCM and BUFGMUX. 
--      This module contains a feedthrough module for each digital clock
-- manager (DCM) in each Virtex-II Pro device. 
--      Note that this module is only used for simulation purposes and is 
-- NOT to be included in the implementation tools design project.
--      An EDIF file is provided, for each DCM/device type, to
-- implement local routing of two DCM clocks between the DCM and BUFGMUX.
-- For design with multiple DCM's, use additional local routing macros. Each
-- macro is named by the device and DCM location. For example, there are 8
-- DCM's in a 2VP70 device, and the local routing macro for the DCM located
-- at coordinate X2Y1 is named dcmx2y1_2vp70. 
-------------------------------------------------------------------------------
--Structure:
--      -- hispeedclks.vhd
--
-------------------------------------------------------------------------------
-- Owners:          Steve Trynosky
-- Revision 1.0
--
--     Modification History:                                                   
--     Date       Init        Description                                
--   --------   ------  -------------------------------------------------------
--   10/28/2003   ST      Initial Release 
-------------------------------------------------------------------------------
-- 
-- This design takes the two clock inputs to the local route module which has
-- fixed placement constraints as well as directed routing for optimal timing
-- performance. An equivalent EDIF file can be found in the archive. Just put 
-- the EDIF file with the top level design in the same directory where the 
-- implementation tools run. For designs with multiple DCM's, multiple EDIF's
-- are provided.
--

-- synthesis translate_off

-- ***************************************************************
--         XC2VP2 functional simulation modules follow:
-- ***************************************************************
--TOP DCM's:XnY1

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y1_2vp2 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y1_2vp2;

ARCHITECTURE translated OF dcmx0y1_2vp2 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y1_2vp2 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx1y1_2vp2;

ARCHITECTURE translated OF dcmx1y1_2vp2 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

--BOTTOM DCM's:XnY0
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y0_2vp2 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y0_2vp2;

ARCHITECTURE translated OF dcmx0y0_2vp2 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y0_2vp2 IS
   PORT (
      clock1_in     : IN STD_LOGIC;   
      clock2_in     : IN STD_LOGIC;   
      clock1_out    : OUT STD_LOGIC;   
      clock2_out    : OUT STD_LOGIC);   
END dcmx1y0_2vp2;

ARCHITECTURE translated OF dcmx1y0_2vp2 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;



-- ***************************************************************
--         XC2VP4 functional simulation modules follow:
-- ***************************************************************
--TOP DCM's:XnY1
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y1_2vp4 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y1_2vp4;

ARCHITECTURE translated OF dcmx0y1_2vp4 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y1_2vp4 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx1y1_2vp4;

ARCHITECTURE translated OF dcmx1y1_2vp4 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

--BOTTOM DCM's:XnY0
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y0_2vp4 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y0_2vp4;

ARCHITECTURE translated OF dcmx0y0_2vp4 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y0_2vp4 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx1y0_2vp4;

ARCHITECTURE translated OF dcmx1y0_2vp4 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;



-- ***************************************************************
--         XC2VP7 functional simulation modules follow:
-- ***************************************************************
--TOP DCM's:XnY1
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y1_2vp7 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y1_2vp7;

ARCHITECTURE translated OF dcmx0y1_2vp7 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y1_2vp7 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx1y1_2vp7;

ARCHITECTURE translated OF dcmx1y1_2vp7 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

--BOTTOM DCM's:XnY0
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y0_2vp7 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y0_2vp7;

ARCHITECTURE translated OF dcmx0y0_2vp7 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y0_2vp7 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx1y0_2vp7;

ARCHITECTURE translated OF dcmx1y0_2vp7 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;



-- ***************************************************************
--         XC2VP20 functional simulation modules follow:
-- ***************************************************************
--TOP DCM's:XnY1
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y1_2vp20 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y1_2vp20;

ARCHITECTURE translated OF dcmx0y1_2vp20 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y1_2vp20 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx1y1_2vp20;

ARCHITECTURE translated OF dcmx1y1_2vp20 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx2y1_2vp20 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx2y1_2vp20;

ARCHITECTURE translated OF dcmx2y1_2vp20 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx3y1_2vp20 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx3y1_2vp20;

ARCHITECTURE translated OF dcmx3y1_2vp20 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

--BOTTOM DCM's:XnY0
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y0_2vp20 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y0_2vp20;

ARCHITECTURE translated OF dcmx0y0_2vp20 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y0_2vp20 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx1y0_2vp20;

ARCHITECTURE translated OF dcmx1y0_2vp20 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx2y0_2vp20 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx2y0_2vp20;

ARCHITECTURE translated OF dcmx2y0_2vp20 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx3y0_2vp20 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx3y0_2vp20;

ARCHITECTURE translated OF dcmx3y0_2vp20 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;



-- ***************************************************************
--         XC2VP30 functional simulation modules follow:
-- ***************************************************************
--TOP DCM's:XnY1
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y1_2vp30 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y1_2vp30;

ARCHITECTURE translated OF dcmx0y1_2vp30 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y1_2vp30 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx1y1_2vp30;

ARCHITECTURE translated OF dcmx1y1_2vp30 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx2y1_2vp30 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx2y1_2vp30;

ARCHITECTURE translated OF dcmx2y1_2vp30 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx3y1_2vp30 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx3y1_2vp30;

ARCHITECTURE translated OF dcmx3y1_2vp30 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

--BOTTOM DCM's:XnY0
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y0_2vp30 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y0_2vp30;

ARCHITECTURE translated OF dcmx0y0_2vp30 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y0_2vp30 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx1y0_2vp30;

ARCHITECTURE translated OF dcmx1y0_2vp30 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx2y0_2vp30 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx2y0_2vp30;

ARCHITECTURE translated OF dcmx2y0_2vp30 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx3y0_2vp30 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx3y0_2vp30;

ARCHITECTURE translated OF dcmx3y0_2vp30 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;



-- ***************************************************************
--         XC2VP40 functional simulation modules follow:
-- ***************************************************************
--TOP DCM's:XnY1
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y1_2vp40 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y1_2vp40;

ARCHITECTURE translated OF dcmx0y1_2vp40 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y1_2vp40 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx1y1_2vp40;

ARCHITECTURE translated OF dcmx1y1_2vp40 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx2y1_2vp40 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx2y1_2vp40;

ARCHITECTURE translated OF dcmx2y1_2vp40 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx3y1_2vp40 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx3y1_2vp40;

ARCHITECTURE translated OF dcmx3y1_2vp40 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

--BOTTOM DCM's:XnY0
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y0_2vp40 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y0_2vp40;

ARCHITECTURE translated OF dcmx0y0_2vp40 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y0_2vp40 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx1y0_2vp40;

ARCHITECTURE translated OF dcmx1y0_2vp40 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx2y0_2vp40 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx2y0_2vp40;

ARCHITECTURE translated OF dcmx2y0_2vp40 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx3y0_2vp40 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx3y0_2vp40;

ARCHITECTURE translated OF dcmx3y0_2vp40 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;



-- ***************************************************************
--         XC2VP50 functional simulation modules follow:
-- ***************************************************************
--TOP DCM's:XnY1
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y1_2vp50 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y1_2vp50;

ARCHITECTURE translated OF dcmx0y1_2vp50 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y1_2vp50 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx1y1_2vp50;

ARCHITECTURE translated OF dcmx1y1_2vp50 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx2y1_2vp50 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx2y1_2vp50;

ARCHITECTURE translated OF dcmx2y1_2vp50 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx3y1_2vp50 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx3y1_2vp50;

ARCHITECTURE translated OF dcmx3y1_2vp50 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

--BOTTOM DCM's:XnY0
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y0_2vp50 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y0_2vp50;

ARCHITECTURE translated OF dcmx0y0_2vp50 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y0_2vp50 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx1y0_2vp50;

ARCHITECTURE translated OF dcmx1y0_2vp50 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx2y0_2vp50 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx2y0_2vp50;

ARCHITECTURE translated OF dcmx2y0_2vp50 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx3y0_2vp50 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx3y0_2vp50;

ARCHITECTURE translated OF dcmx3y0_2vp50 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;



-- ***************************************************************
--         XC2VP70 functional simulation modules follow:
-- ***************************************************************
--TOP DCM's:XnY1
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y1_2vp70 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y1_2vp70;

ARCHITECTURE translated OF dcmx0y1_2vp70 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y1_2vp70 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx1y1_2vp70;

ARCHITECTURE translated OF dcmx1y1_2vp70 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx2y1_2vp70 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx2y1_2vp70;

ARCHITECTURE translated OF dcmx2y1_2vp70 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx3y1_2vp70 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx3y1_2vp70;

ARCHITECTURE translated OF dcmx3y1_2vp70 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

--BOTTOM DCM's:XnY0
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y0_2vp70 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y0_2vp70;

ARCHITECTURE translated OF dcmx0y0_2vp70 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y0_2vp70 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx1y0_2vp70;

ARCHITECTURE translated OF dcmx1y0_2vp70 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx2y0_2vp70 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx2y0_2vp70;

ARCHITECTURE translated OF dcmx2y0_2vp70 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx3y0_2vp70 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx3y0_2vp70;

ARCHITECTURE translated OF dcmx3y0_2vp70 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;





-- ***************************************************************
--         XC2VP100 functional simulation modules follow:
-- ***************************************************************
--TOP DCM's:XnY1
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y1_2vp100 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y1_2vp100;

ARCHITECTURE translated OF dcmx0y1_2vp100 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y1_2vp100 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx1y1_2vp100;

ARCHITECTURE translated OF dcmx1y1_2vp100 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx2y1_2vp100 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx2y1_2vp100;

ARCHITECTURE translated OF dcmx2y1_2vp100 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx3y1_2vp100 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx3y1_2vp100;

ARCHITECTURE translated OF dcmx3y1_2vp100 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx4y1_2vp100 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx4y1_2vp100;

ARCHITECTURE translated OF dcmx4y1_2vp100 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx5y1_2vp100 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx5y1_2vp100;

ARCHITECTURE translated OF dcmx5y1_2vp100 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

--BOTTOM DCM's:XnY0
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx0y0_2vp100 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx0y0_2vp100;

ARCHITECTURE translated OF dcmx0y0_2vp100 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx1y0_2vp100 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx1y0_2vp100;

ARCHITECTURE translated OF dcmx1y0_2vp100 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx2y0_2vp100 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx2y0_2vp100;

ARCHITECTURE translated OF dcmx2y0_2vp100 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx3y0_2vp100 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx3y0_2vp100;

ARCHITECTURE translated OF dcmx3y0_2vp100 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx4y0_2vp100 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx4y0_2vp100;

ARCHITECTURE translated OF dcmx4y0_2vp100 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;

  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE IEEE.numeric_std.ALL;
ENTITY dcmx5y0_2vp100 IS
   PORT (
      clock1_in     : IN std_logic;   
      clock2_in     : IN std_logic;   
      clock1_out    : OUT std_logic;   
      clock2_out    : OUT std_logic);   
END dcmx5y0_2vp100;

ARCHITECTURE translated OF dcmx5y0_2vp100 IS
BEGIN
   clock1_out <= clock1_in;
   clock2_out <= clock2_in;
END translated;
-- synthesis translate_on