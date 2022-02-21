National Instruments NI-VISA(TM) Software for Windows. Version 3.0.

Copyright(c) 1995, 2003 National Instruments Corporation. All rights reserved.



NATIONAL INSTRUMENTS HOLDS MULTIPLE COPYRIGHTS, TRADEMARKS, AND PATENTS

FOR THIS PRODUCT.  BY DOWNLOADING AND/OR INSTALLING THIS PRODUCT, YOU AGREE

TO THE TERMS OF THE license.txt FILE THAT RESIDES IN THE SAME DIRECTORY AS

THIS readme.txt FILE ON THE CD AND/OR WEB SITE.  FOR PATENTS COVERING

NATIONAL INSTRUMENTS PRODUCTS, REFER TO THE patents.txt FILE ON THE ROOT

DIRECTORY OF YOUR CD (IF APPLICABLE) AND/OR www.ni.com/patents.





CONTENTS OF THIS FILE

---------------------

Installing and Uninstalling NI-VISA 3.0

LabWindows(TM)/CVI Run-Time Engine

Support for new VISA Features

PXI Geographic Addressing Support

PXI Resource Naming

PXI System Configuration

Support for Multiple GPIB-VXI Controllers

Support for Agilent/HP Controllers

Using viPeekXX/viPokeXX Macros

NI-VISA 2.5 Improvements and Bug Fixes

NI-VISA 2.5.2 Improvements and Bug Fixes

NI-VISA 2.6 Improvements and Bug Fixes

NI-VISA 2.6.1 Improvements and Bug Fixes

NI-VISA 3.0 Improvements and Bug Fixes

Known Issues





INSTALLING AND UNINSTALLING NI-VISA 3.0

---------------------------------------

NI-VISA 3.0 does not support Windows 95.  If you need to run your VISA

application on Windows 95, you must install NI-VISA 2.6.1 or lower.



Before you install NI-VISA 3.0, we strongly recommend that you uninstall

any versions of VISA prior to NI-VISA 2.0.1.  NI-VISA 3.0 should upgrade

properly over NI-VISA versions 2.0.1 through 2.6.1.



After uninstalling NI-VISA, some DLLs and executables may remain in the

NI-VISA directories. To complete the uninstall process, you must restart your

machine. Note: the uninstaller application will not notify you when rebooting

is necessary. If you plan to re-install NI-VISA, you should always reboot

the machine after running the uninstaller.





LABWINDOWS/CVI RUN-TIME ENGINE

------------------------------

The Setup program now automatically installs the LabWindows/CVI Run-Time

Engine when necessary. The NI-VISA interactive utility, VISAIC, requires the

LabWindows/CVI Run-Time Engine 6.0 or later to be installed. When you install

this utility, Setup also installs the LabWindows/CVI Run-Time Engine if it is

not already present.





SUPPORT FOR NEW VISA FEATURES

-----------------------------

Starting with NI-VISA 2.6, we now support remote NI-VISA access.  Any machine

with the NI-VISA server can export its resources to other NI-VISA clients.

Configure client and server settings in Measurement & Automation Explorer.

To use a remote VISA resource, the name is a URL including the machine name or

IP address, followed by the traditional VISA resource name.  Examples include:

"visa://machine.domain.com/GPIB0::5::INSTR"

"visa://1.2.3.4/ASRL2::INSTR"



We have added support for the PXI::MEMACC resource.  This resource supports

viMemAlloc and viMemFree for allocating blocks of physically contiguous locked

memory on your controller.  You can access that memory with viInX/viOutX,

viMoveInX/viMoveOutX, and viMapAddress/viPeekX/viPokeX/viUnmapAddress.  Use

the address space VI_PXI_ALLOC_SPACE (9).  viFindRsrc will not detect this

resource by default, but you can still call viOpen with that resource string.

To make it appear in the list returned by viFindRsrc, change the PXI setting

in Measurement & Automation Explorer in Tools>>NI-VISA Options>>VISA Options.



IVI driver developers can use the new VISA COM API.  NI-VISA 3.0 does not yet

document this API, so only those developers familiar with the specification

should use it in this release.  The relevant COM classes for getting started

include IResourceManager and IVisaSession, registered in GlobMgr.dll.  The

VISA COM API is functionally equivalent to the VISA C API.  It does not add

features; it is just another language interface.  The VISA COM API can only

be installed if you have Internet Explorer 4.01 or higher on your system.



NI-VISA 3.0 adds support for USB devices without their own Windows drivers.

To use NI-VISA for such a device, you must first run the VISA Driver

Development Wizard and create an INF file for binding your device to the

NI-VISA driver. The resource format you use to open such a device is this:

"USB::<manfID>::<modelID>::<serial#>[::<USBinterface>]::RAW".  The attributes

are not yet documented but are listed in the visa.h file.  To use this

functionality in your C program you must #define NIVISA_USB before including

the visa.h header file.



NI-VISA 3.0 adds several new features for PXI resources.  First, NI-VISA now

includes the ability to determine a PXI device’s geographical address, such as

its chassis and slot number, using two new PXI attributes.  Second, NI-VISA

includes two new resource name formats that improve upon the existing naming

convention.  Finally, Measurement and Automation Explorer (MAX) now includes

a PXI System tree view for enabling geographic addressing and other features.





PXI GEOGRAPHIC ADDRESSING SUPPORT

---------------------------------

NI-VISA 3.0 now supports geographic addressing for PXI resources.  Geographic

addressing includes the ability to identify the chassis number and slot number

for a PXI module.  To support geographic addressing, the following attributes

have been implemented on the PXI INSTR resource type:



VI_ATTR_SLOT - specifies the PXI slot number for instrument

VI_ATTR_PXI_CHASSIS_NUM - specifies the PXI chassis number for the instrument



Using these attributes, PXI resources can be located based on their geographic

address in a system.  For example, the resource description string

"PXI?*INSTR{VI_ATTR_PXI_CHASSIS_NUM==1 && VI_ATTR_SLOT==3}" can be used to

locate a PXI device in chassis 1, slot 3.





PXI RESOURCE NAMING

-------------------

NI-VISA 3.0 includes a new PXI resource format based on the traditional

bus number/device number/function number addressing scheme:



"PXI0::<busNum>-<devNum>[.<funcNum>]::INSTR"



For example, given a PXI device at bus 2, device number 17, function

number 1, the corresponding resource name for this device would be

"PXI0::2-17.1::INSTR".



In this new resource name, the "0" in "PXI0" represents the PCI interface

number for the PXI system.  Currently, this interface number is always "0".

Note that the interface number is not necessarily the same as a device's

PCI bus number.



Of course, NI-VISA continues to support the legacy PXI naming convention:



"PXI<busNum>::<devNum>[::funcNum]::INSTR"



Note that, for a single-function PXI device on PCI bus 0, both of these

resource formats yield an identical string.





PXI SYSTEM CONFIGURATION

------------------------

In order to enable geographic addressing, a PXI system description file

(pxisys.ini) must be present on the system.  Beginning with NI-VISA 3.0, the

Measurement and Automation Explorer (MAX) includes the ability to identify

components in a PXI system.  Through the hardware identification process, MAX

collects the information necessary to generate a PXI system description file.



Follow these steps to configure your PXI system for geographic addressing:



1) Launch MAX.

2) Expand the "Devices and Interfaces" folder.  If you are configuring a

   LabVIEW RT system, locate that system's "Devices and Interfaces" folder

   under the "Remote Systems" view.

3) Identify your PXI controller.  Locate the "PXI System 0" item.  "PXI

   System 0" represents the PXI system and its controller.  To identify this

   controller, right click on "PXI System 0" and use the "Identify As"

   feature, selecting your controller from the resulting list.

   NOTE: If you are using an external PC to control the PXI system via a PXI

         bridge like MXI-3, select this option from the menu.

4) Identify your PXI chassis.  Expand the "PXI System 0" view to locate your

   PXI chassis in the system.  For embedded controllers, a chassis is added

   when the controller has been identified.  For a chassis controlled via

   MXI-3, these chassis are added automatically by the software.  To identify

   a chassis, right-click on the chassis and use the "Identify As" feature,

   selecting your chassis from the resulting list.  Chassis will be assigned

   a number based on the PCI bus number that the chassis resides on.



Once the controller and chassis have been identified, a pxisys.ini file is

created in the <windows> directory.  This file conforms to the PXI

specification, and it is used to allow drivers (including NI-VISA) and other

configuration software to enable geographic addressing and other features

of PXI.



Special Note to LabVIEW RT Users: For LabVIEW RT systems, the pxisys.ini file

is generated locally (on the host system), and it must be manually transferred

to the remote system via ftp.  When identifying components in a remote system,

MAX will prompt you for a location to save a local copy of the pxisys.ini file.

After identifying all PXI components, ftp this local copy of pxisys.ini to the

remote system, placing the file in the "c:\ni-rt\system" directory.





SUPPORT FOR MULTIPLE GPIB-VXI CONTROLLERS

-----------------------------------------

NI-VISA supports multiple GPIB-VXI controllers in a single system. Because the

National Instruments GPIB-VXI controller ships with a primary address of 1,

NI-VISA automatically searches for any GPIB-VXI controllers at that address.



If you have changed the primary address (for example, if you added a second

GPIB-VXI controller to your system and set the primary address of the second

controller to 9), or if you have a GPIB-VXI controller from another vendor,

you need to add it using Measurement & Automation Explorer so NI-VISA can find

it. Right-click on "Devices and Interfaces" and select "Create New..." to add

a new GPIB-VXI interface. Specify the controller's GPIB primary and secondary

addresses, as well as the GPIB board to which it is connected. Then specify a

new GPIB-VXI interface number and its logical address (usually 0).



If you are using a GPIB-VXI command module from another vendor, you also need

to obtain and install that vendor's VISA GPIB-VXI component.



If you have a GPIB device at primary address 1 and that device does not

recognize "*IDN?", you should disable the GPIB-VXI auto-detect feature in

Measurement & Automation Explorer from the Tools>>NI-VISA>>VISA Options menu.





SUPPORT FOR AGILENT/HP CONTROLLERS

----------------------------------

NI-VISA now works with Agilent/HP GPIB and VXI controller cards. To

use this feature, go into the Measurement & Automation Explorer, select

"Tools>>NI-VISA>>VISA Options", choose the Passports tab, and enable the

NiVisaTulip.dll Passport. NI-VISA will automaticaly find and access any

GPIB or VXI devices. However, NI-VISA will not configure the Agilent/HP

hardware; you must still do that with the Agilent/HP I/O Config utility.



If you have both National Instruments and Agilent/HP controllers in the same

system, you must configure the cards at different system addresses, such as

GPIB0 and GPIB1. If both vendors try to use the same interface address,

NI-VISA will report an error from viFindRsrc or VISA Find Resources.





USING viPeekXX/viPokeXX MACROS

------------------------------

Users who want to use the viPeekXX/viPokeXX macros for platforms on which the

macros are available must define a macro called NIVISA_PEEKPOKE before

including "visa.h". Please note that the macros are currently available

on only the following platforms:



- Win32 (All C compilers)

- Solaris (LabWindows/CVI only)



If you have used these macros with a version of NI-VISA prior to 2.0, your

application will continue to run, but the peek/poke performance will decrease.

You should relink with the libraries supplied with this installation to regain

the prior peek/poke performance.





NI-VISA 2.5 IMPROVEMENTS AND BUG FIXES

--------------------------------------

GPIB performance has improved. Its performance is now identical to NI-488.



If you are using the GPIB-ENET controller, the previous version of NI-VISA

would take a minute or more to time out if a GPIB-ENET controller were not

connected or powered on. The timeout is now much shorter.



Serial performance has improved. It no longer consumes excessive CPU time

during read operations when the data is not immediately available.



The VISA serial driver now works with baud rates higher than 256000. NI-VISA

will allow any baud rate that the Windows driver will accept.



If you set the Flow Control serial setting in T&M Explorer, it was not stored

properly. This has been fixed in Measurement & Automation Explorer.



This version of NI-VISA incorporates general bug fixes and improvements for

GPIB-VXI. The NI-VISA code instrument will now load properly in configurations

where it previously failed.



If a GPIB-VXI chassis is reset while an application has VISA loaded, the

GPIB-VXI would previously become accessible only as GPIB but not GPIB-VXI.

This release of NI-VISA will now properly re-detect the GPIB-VXI.



Fixed bug with the GPIB-VXI and hardware triggers.



Fixed bug for PXI in Windows NT where it would occasionally crash on exit.



Fixed bug for PXI where repeated calls to viOpen or VISA Open would

occasionally return an undocumented status code.



The documentation now includes resource strings and valid parameter values

for PXI operations.



The PXI Driver Development Wizard will now also generate a .inf file for

Windows 2000.



This version of NI-VISA incorporates general bug fixes and improvements for

formatted I/O, especially with very large buffers and with the following

modifiers: "%b", ",", "#", "!ob", "!ol", and "%[".



The documentation for viPrintf and viFlush better describes when formatted

I/O buffers are sent. The documentation for viScanf better describes the ","

and "#" modifiers.



Added capability to see low-level VISA formatted I/O buffers in NI Spy.



Improved NI Spy performance when capturing calls, logging to disk, and opening

files.



Fixed bug with starting NI Spy while DLLs are in use.



Fixed bugs in NI Spy where invalid buffer addresses or invalid buffer sizes

would previously cause NI Spy to crash.



Fixed installer bugs where the PATH and VXIPNPPATH environment variables were

occasionally incorrect, especially with long directory names on Windows 95/98.





NI-VISA 2.5.2 IMPROVEMENTS AND BUG FIXES

----------------------------------------

Fixed bug with enabling the exception event on GPIB-VXI.



Fixed bug with GPIB-USB-A and NI-VISA on Windows 2000.



The performance of viFindRsrc has improved.



On Serial reads where the data is not immediately available, the CPU usage is

now lower.



Fixed bug with using NI-VISA on Windows 2000 as Guest users.



viScanf %f modifier will now properly scan numbers bigger than 2^31-1.



By default, viFindRsrc will return only the PXI/PCI instruments for which you

created INF files using the PXI driver development wizard. But you can open

sessions to all resources that you were able to find using earlier versions of

NI-VISA. To enable viFindRsrc to return all available resources, change the PXI

setting in Measurement & Automation Explorer in Tools>>NI-VISA>>VISA Options.



Fixed bug with Measurement & Automation Explorer with GPIB on Windows 2000.



The PXI Driver Development Wizard will now also generate a .inf file for

LabVIEW RT.





NI-VISA 2.6 IMPROVEMENTS AND BUG FIXES

--------------------------------------

viGpibPassControl now returns an error if the primary and/or secondary address

is invalid.



In VXI, mapping a trigger line already mapped produced VI_ERROR_LINE_IN_USE 

(0xBFFF0042), instead of VI_SUCCESS_TRIG_MAPPED (0x3FFF007E).  This now

returns the expected status code.



In PXI, VXI, and GPIB-VXI, using viMoveInXX and viMoveOutXX with a negative 

value for the length parameter would occasionally cause the system to reboot.

This now returns the error code VI_ERROR_INV_LENGTH (0xBFFF0083) instead.



In PXI, viMoveInXX and viMoveOutXX would access the same location rather than

incrementing the address.  This has been fixed.



In PXI, if the device memory window was less than 4KB and the Windows OS put

it at an offset that was not aligned on a 4KB boundary, NI-VISA would not be

able to access it properly.  This has been fixed.



In PXI, the ability to query VI_ATTR_MANF_NAME and VI_ATTR_MODEL_NAME was not

implemented.  This has been added.  You should generate new INF files using

the new version of the PXI Driver Development Wizard.  Many supported cards

have already been added to the NI database and should be listed in MAX.  Any

devices using the new INF files will also have their names listed properly.



In PXI, if the device model code began with a zero (0), using viEnableEvent

would return an error.  This has been fixed.



If you opened and closed a parallel port quickly, viOpen would occasionally

return VI_ERROR_SYSTEM_ERROR (0xBFFF0000).  This problem may still occur, but

it is the result of an OS error indicating the resource is busy.  In this

case, NI-VISA now returns VI_ERROR_RSRC_BUSY (0xBFFF0072) instead.



If you wrote data to a parallel port on Windows NT/2000, the timeout was

being ignored.  This has been fixed.



In Serial, if you tried to set 8 data bits and 1.5 stop bits, NI-VISA would

return the error VI_ERROR_SYSTEM_ERROR (0xBFFF0000).  On Windows this is an

invalid combination, because 1.5 stop bits is supported only with 5 data bits.

With 8 data bits, you may use either 1 or 2 stop bits only.  In this case,

NI-VISA now returns VI_ERROR_INV_SETUP (0xBFFF003A) instead.



viReadToFile now writes the correct data to the file.



If you created a TCP/IP VXI-11 device entry in Measurement & Automation

Explorer, and then exited and restarted MAX, that device would no longer

show up.  This has been fixed.



For TCP/IP SOCKET and INSTR resources, the openTimeout specified in viOpen

is now used as the network timeout during the connection sequence.  The

default openTimeout, if not specified, is 2 seconds (2000 milliseconds).



For TCP/IP SOCKET resources, trying to set the attribute VI_ATTR_TCPIP_NODELAY

to VI_TRUE would return an error on Windows 98.  This has been fixed.



For TCP/IP INSTR resources, trying to do a viRead or viReadAsync on a buffer

with a size larger than a few kilobytes, would either return an error or

occasionally crash.  This has been fixed.



In Measurement & Automation Explorer, the GPIB resources could not be dragged

into LabVIEW 6i.  This has been fixed.



After uninstalling NI-VISA 2.5.x, Measurement & Automation Explorer would

display a VISA Error.  This has been fixed.



Fixed installer bugs where the PATH and VXIPNPPATH environment variables were

incorrect, when using directory names with spaces on Windows 95/98.



NI Spy would crash if the application being spied upon had a pathname greater

than 72 characters.  This has been fixed.



Calling viTerminate on a Serial transfer would return the incorrect number

of bytes transferred up to that time.  This has been fixed.



Calling viTerminate on a Serial transfer would cause subsequent transfer

attempts (read or write) on the same session to appear to complete

prematurely.  This has been fixed.



Fixed LabWindows/CVI function panel help.  Added descriptions of the new

resource string formats and event types that NI-VISA 2.5 and higher supports.



viFindRsrc would previously reject certain valid expressions and accept

certain invalid expressions, usually involving spaces.  This has been fixed.



Calling viUninstallHandler from the callback is not recommended, but is

supported.  Doing this in NI-VISA 2.5.2 would take 15 seconds.



Calling viEnableEvent on VI_EVENT_SERVICE_REQ for GPIB would not generate

an event if the SRQ line was already asserted.  This has been fixed.



viFindRsrc now automatically detects parallel ports other than just LPT1.



In Serial, RS-485 does not support all the lines that RS-232 does.  Querying

the attributes corresponding to those lines will return VI_UNKNOWN_STATE.



In Serial, the DTR line was always asserted on viOpen.  For RS-232, that

continues to happen.  For RS-485 we no longer assert the line automatically.



In Serial, if data arrives at the same time as viTerminate is called or a

timeout occurs, the data now gets read.  If this satisfies the read request,

then the read operation returns a successful status.



Improved NI Spy performance.  Each call now takes less space in memory and

on disk.  Large files now load much faster.  When loading files larger than

the call history depth, there is now an option to load only the first or

last calls, instead of the entire file.



Calling viOpen with access mode 1 (exclusive lock) did not work as expected

on GPIB interface sessions.  This has been fixed.





NI-VISA 2.6.1 IMPROVEMENTS AND BUG FIXES

----------------------------------------

NI-VISA is now partially localized for Japanese.  The localized portions

include the installer, NI Spy, and the VISA provider in MAX.



In PXI, handling interrupts using the VISA queue is now faster on LabVIEW RT

and Windows 2000/NT/XP.



When you open a VISA session to GPIB0::INTFC and immediately query the

attribute VI_ATTR_GPIB_SRQ_STATE, it previously returned the value

VI_STATE_UNASSERTED.  It now returns VI_STATE_UNKNOWN because the SRQ line

cannot be monitored if the interface is not yet CIC.  The correct state is

returned after calling viGpibSendIFC.



For a TCPIP INSTR resource, calling viTerminate did not work correctly.



For a TCPIP INSTR resource, calling viRead or viWrite with a large buffer

occasionally would return an error without transmitting the entire buffer.



For a TCPIP INSTR resource, opening and closing sessions to multiple host

addresses would cause NI-VISA to crash.



For a TCPIP INSTR resource, calling viLock with a timeout longer than the

I/O timeout for that session could cause NI-VISA to return VI_ERROR_TMO.



viScanf %# modifier will now output the number of characters put into the

buffer excluding the terminating null character.  This applies to the %#s, 

%#t, %#T, and %#[] modifiers. NI-VISA treats the maximum array size 

(specified on input) as the total buffer size including the terminating null

character.



Once viScanf returned VI_ERROR_TMO, it would no longer work properly on

that session.



Opening and closing a serial port in a loop on Windows 2000/NT/XP would

cause a memory leak.



LabVIEW RT now supports the attribute VI_ATTR_ASRL_WIRE_MODE.





NI-VISA 3.0 IMPROVEMENTS AND BUG FIXES

--------------------------------------

The driver now works properly with Fast User Switching on Windows XP/2000.



For a PXI INSTR resource, we now support PXI/PCI devices that rely on the

subvendor ID and/or the subvendor model code.  To use this functionality

you must create a new .INF file with the VISA Driver Development Wizard.

The attributes VI_ATTR_PXI_SUB_MANF_ID and VI_ATTR_PXI_SUB_MODEL_CODE

are no longer supported by the VISA driver.  The correct attributes to

query are now VI_ATTR_MANF_ID and VI_ATTR_MODEL_CODE.  These will return

the primary vendor information if no subvendor information is present,

and they will return the subvendor information if it is present.



For a PXI MEMACC resource, viMemFree now frees all memory correctly.



For a Serial INSTR resource, viOpen on both Windows and LabVIEW RT will now

accept "COMx" as a valid resource name even if MAX has not yet been run.



For a Serial INSTR resource, viOpen will now open a port if the process

that had been using that port exited ungracefully (such as with Ctrl-C).

The previous behavior had been that viOpen would return VI_ERROR_RSRC_BUSY.



For a Serial INSTR resource, viWriteAsync was not implemented.  Now it is.



For a Serial INSTR resource, multiple sessions to the same port caused

weird behavior if you tried to both write and read data simultaneously.

This has been fixed.



For a Serial INSTR resource, if you enabled RTS/CTS flow control and then

manually tried to set the state of the RTS line, some operating systems

would generate VI_ERROR_NSUP_ATTR while others would return VI_SUCCESS but

not actually modify the state of the line.  Neither of these behaviours was

correct.  Now NI-VISA will return VI_ERROR_INV_SETUP in this case on all

operating systems.



For a Serial INSTR resource, the RS-485 wire mode attribute now works

properly on Windows XP/2000.



In GPIB, the attribute VI_ATTR_INTF_INST_NAME would report an unknown

board type for some GPIB-USB and GPIB-ENET controllers.  The symptom was

also visible in the VISA Interactive Control utility.  This attribute now

reports the correct information.



In GPIB, the implementation of viFindRsrc could take up to 10 seconds if

you had NI-488.2 version 2.0 installed.  The symptom was also visible in

LabVIEW when you dropped the first VISA resource control on the front

panel.  This now takes less than 1 second on most machines.



NI Spy would occasionally incorrectly report an output jobID of 0 for

viReadAsync or viWriteAsync even though the operation completed

successfully with a valid non-zero jobID.



NI Spy would not restore the window location on a monitor other than the

primary monitor.  This now works as expected.



If you ran MAX as a guest user and edited the Passport settings, the new

settings could not be saved to the registry, but MAX did not generate a

warning.  This has been fixed, and you will now get a warning.



For a TCPIP SOCKET resource, viReadAsync was not implemented.  Now it is.



The remote NI-VISA server could not be started on the same machine as

the remote DAQ server because they both used the same port.  Now the

remote NI-VISA server uses port 3537 by default.



viFindRsrc would crash on a complex expression with multiple "|" symbols.

This has been fixed.



If you had a GPIB device at primary address 1 that generated a response to

*IDN? that was longer than 100 characters, viFindRsrc would occasionally

crash.  This has been fixed.



Some VISA COM components were not being correctly registered.



The VISA COM API would return an incorrect string from ReadString if the

number of bytes read was exactly what was requested.



The VISA COM API would not return output string parameters correctly if

the variable passed into the function was uninitialized or had a garbage

value.  This problem was normally only seen from C++ when using a raw BSTR

instead of CComBSTR.  This has been fixed.



In GPIB, the implementation of viFindRsrc on LabVIEW RT would find GPIB

resources only on GPIB0.  It now finds GPIB resources on any configured

GPIB interface number.



Calling viUnmapAddress when using an Agilent HP-E1406A GPIB-VXI controller

would cause the application to crash.  This has been fixed.



The attributes VI_ATTR_MANF_NAME and VI_ATTR_MODEL_NAME are now correctly

implemented for GPIB-VXI regardless of whether NI-VXI is installed.



The maximum simultaneous number of GPIB-VXI controllers supported by

NI-VISA has changed from 10 to 32.



The VISA Driver Development Wizard (formerly known as the PXI Driver

Development Wizard) now allows multiple logical-OR PXI interrupt conditions.



You can now edit the binding properties of a VISA TCPIP resource in MAX.

For example, you can change the hostname or IP address.



MAX can now auto-detect VXI-11 TCPIP resources on your subnet.  When you

right-click on Devices and Interfaces and choose Create New..., you then

need to specify a VXI-11 instrument and click the option to have MAX browse

your local subnet.



If you enabled the VISA COM API for VI_EVENT_SERVICE_REQ for a callback,

it would crash when the event was being raised.  This has been fixed.



If you tried to exit the VISA Interactive Control utility by right-clicking

on the taskbar icon and choosing Close, the utility would hang.  This has

been fixed.



Using remote NI-VISA with asynchronous I/O did not properly transmit data

asynchronously.  This has been fixed.



In Serial on LabVIEW RT, you can now configure advanced settings (such as

FIFO triggers and the default RS-485 wire mode) in MAX.



In Serial, if the Windows configuration settings for a given port were

different from the NI-VISA settings, MAX would overwrite the Windows settings

when you hit Refresh.  Now, MAX points out the different settings and lets

you choose.



In MAX, selecting Rename on an alias and changing merely the case of the

alias would cause an error.  This has been fixed.



When calling viWaitOnEvent repeatedly with a timeout of 0 (immediate), some

interfaces and event types would still consume excessive CPU time.  Now doing

this will yield the CPU regardless of the interface or event type.



For a Serial INSTR resource, if a parity or framing error occurred, NI-VISA

would not return the correct error condition.  This has been fixed.



viFindRsrc would return VI_ERROR_INV_EXPR if the expression exceeded 255

characters.  Now it will accept resource expressions of any length.



If a PXI multi-function device did not have function 0 registered as an

NI-VISA resource, then viFindRsrc would not return any of the other functions

of that device either, even if they were registered with NI-VISA.  This now

works properly.



Calling viFindRsrc would always assert REN on each GPIB board after finding

devices.  Now viFindRsrc will re-assert REN on each board only if there is

at least 1 open VISA INSTR session on that board.



For a TCPIP SOCKET resource, using viRead with termchar enabled was much

slower than reading the exact number of bytes.  The performance of using

termchar with viRead is now much better than it was.





KNOWN ISSUES

------------

Installing this on a system with NI-VISA 2.01 might remove T&M Explorer even

if NI-VXI 2.x is installed.



The VISA COM API is not yet documented.  This release of the VISA COM API

should only be used by IVI driver developers who are familiar with the

specification.  The VISA COM API can only be installed if you have

Internet Explorer 4.01 or higher on your system.

