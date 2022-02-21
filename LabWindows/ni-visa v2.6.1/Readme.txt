National Instruments NI-VISA(TM) Software for Windows. Version 2.6.1.

Copyright(c) 1995, 2002 National Instruments Corporation. All rights reserved.


CONTENTS OF THIS FILE

---------------------

Installing and Uninstalling NI-VISA 2.6.1

LabWindows(TM)/CVI Run-Time Engine

Support for new VISA Features

Support for Multiple GPIB-VXI Controllers

Support for Agilent/HP Controllers

Using viPeekXX/viPokeXX Macros

NI-VISA 2.5 Improvements and Bug Fixes

NI-VISA 2.5.2 Improvements and Bug Fixes

NI-VISA 2.6 Improvements and Bug Fixes

NI-VISA 2.6.1 Improvements and Bug Fixes

Known Issues





INSTALLING AND UNINSTALLING NI-VISA 2.6.1

-----------------------------------------

Before you install NI-VISA 2.6.1, we strongly recommend that you uninstall

any versions of VISA prior to NI-VISA 2.0.1.  NI-VISA 2.6.1 should upgrade

properly over NI-VISA versions 2.0.1 through 2.6.0.



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

The Adobe Acrobat manuals describe the new features of NI-VISA 2.6, but they

are briefly listed here for your convenience.  The manuals are installed in

<VXIPNPPATH>/Winxx/NIvisa/Manuals.



Starting with NI-VISA 2.6, we now support remote NI-VISA access.  Any machine

with the NI-VISA server can export its resources to other NI-VISA clients.

Client and server settings are configured in Measurement & Automation Explorer.

To use a remote VISA resource, the name is a URL including the machine name or

IP address, followed by the traditional VISA resource name.  Examples include:

"visa://machine.domain.com/GPIB0::5::INSTR"

"visa://1.2.3.4/ASRL2::INSTR"



Remote NI-VISA fully supports all functionality of all interface types,

including message based, register based, and all events.



NI-VISA 2.6 also adds new attributes and events for Serial ports.  This

includes most functionality previously available in the LabWindows/CVI native

serial driver.  See the NI-VISA Programmer Reference Manual for details.



The best way to explore the new features is to use the VISA Interactive

Control to open sessions to the new resource types and look at the available

operations, events, and attributes.  VISAIC also has a new feature to help

you find specific resources.  Type a search expression and press <Tab>, or

click the drop-down arrow and select "Create Query" to let VISAIC assist you

in fine-tuning your expression.



The NI-VISA User Manual has been updated to include a new section on formatted

I/O, including many common instrument driver examples.  There is also a new

chapter with information about each supported hardware interface type, including

PXI/PCI. These topics are covered in chapters 5 and 9, respectively. There is a

shortcut at Start>>Programs>>National Instruments>>VISA>>NI-VISA User Manual.



NI-VISA 2.6 officially supports Windows XP.  The complete list of supported

Windows operating systems is now 95, 98, Me, NT 4, 2000, and XP.



We have added support for the PXI::MEMACC resource.  This resource supports

viMemAlloc and viMemFree for allocating blocks of physically contiguous locked

memory on your controller.  You can access that memory with viInX/viOutX,

viMoveInX/viMoveOutX, and viMapAddress/viPeekX/viPokeX/viUnmapAddress.  Use

the address space VI_PXI_ALLOC_SPACE (9).  viFindRsrc will not detect this

resource by default, but you can still call viOpen with that resource string.

To make it appear in the list returned by viFindRsrc, change the PXI setting

in Measurement & Automation Explorer in Tools>>NI-VISA Options>>VISA Options.



IVI driver developers can use the new VISA COM API.  NI-VISA 2.6 does not yet

document this API, so only those developers familiar with the specification

should use it in this release.  The relevant COM classes for getting started

include IResourceManager and IVisaSession, registered in GlobMgr.dll.  The

VISA COM API is functionally equivalent to the VISA C API.  It does not add

features; it is just another language interface.  The VISA COM API can only

be installed if you have Internet Explorer 4.01 or higher on your system.





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



Opening and closing a serial port in a loop on Windows 2000/NT/XP would

cause a memory leak.



LabVIEW RT now supports the attribute VI_ATTR_ASRL_WIRE_MODE.





KNOWN ISSUES

------------

Installing this on a system with NI-VISA 2.01 might remove T&M Explorer even

if NI-VXI 2.x is installed.



Using NI-VISA 2.6.x with Fast User Switching on Windows 2000 or Windows XP

will not currently work with PXI or VXI.  National Instruments recommends

not using this new Windows feature.



The VISA COM API is not yet documented.  This release of the VISA COM API

should only be used by IVI driver developers who are familiar with the

specification.  The VISA COM API can only be installed if you have

Internet Explorer 4.01 or higher on your system.



NI-VISA 2.6.x does not support PXI/PCI devices that rely on the subvendor ID

or the subvendor model code.  This functionality will be added in a future

release of NI-VISA.  Vendors creating such devices must write Windows kernel

drivers in the meantime.



The values for the attribute VI_ATTR_ASRL_WIRE_MODE are listed incorrectly

in the NI-VISA Help and the NI-VISA Programmer Reference Manual.  The correct

macro value names are VI_ASRL_WIRE_485_4, VI_ASRL_WIRE_485_2_DTR_ECHO,

VI_ASRL_WIRE_485_2_DTR_CTRL, and VI_ASRL_WIRE_485_2_AUTO.  The semantics are

otherwise identical to what is documented.

