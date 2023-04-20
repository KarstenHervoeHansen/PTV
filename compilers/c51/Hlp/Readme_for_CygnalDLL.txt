
CYGNAL C8051F uVISION2 DEBUG DRIVER v1.32 Release Notes
Copyright (c) 2000-2003


KNOWN ISSUES AND LIMITATIONS
----------------------------
	
	1.) Only F00X, F01X, F02X, F04X, F06X, F12X, F2XX, F30X, 
            F31X, F32X & F33X devices are supported by this version. 

	2.) The call stack record facility of the on-chip debug
            logic is not supported.

	3.) Conditional data breakpoints (watchpoints) using the 
	    on-chip debug logic	is not supported. (Note, this does 
	    not affect the ability to "watch" variables using the 
	   "watch window" facility of uVision2.)

	4.) The double separator line in the "Peripherals" menu 
	    is a known issue.  This will be corrected by the next 
	    version of TP51.DLL by Keil Software. 

	5.) When setting breakpoints in uVision2 keep in mind that 
	    although uVision2 will allow the unlimited setting of
            breakpoints Cygnal devices are limited to 4 hardware 
            breakpoints. Breakpoints are also used to perform step
            over, run to main, and  run to cursor operations. When
            more than 4 breakpoints are attempted, an error message
            will be displayed when the debugger tries to run, and 
	    until the excess breakpoints are removed, the error 
            will repeatedly be displayed because uVision does not 
            have a limit and will continue to try to set more than 
            4 breakpoints. 

HARDWARE SETUP
--------------
	
	The hardware setup when using uVision2 for debug is the same 
	as used with the Cygnal IDE.  Connect the EC Serial Adapter 
	to the PC's COM port using the RS232 serial cable and to the 
	target device using the 10-pin JTAG ribbon cable.

USING uVISION2 WITH CYGNAL DEVICES
----------------------------------

	1.) Launch uVision2 (uv2.exe).

	2.) Create a project.   Use "Select Device for Target" from 
	    the "Project" menu to select a Cygnal C8051F device from 
	    the Device Database. (Refer to your Keil "Getting Started 
	    with uVision2" manual for more information on creating and
	    configuring projects.)

	3.) Open the "Options for Target" dialog from the "Project" 
	    menu and select the "Debug" tab.

	4.) Select the "Cygnal C8051F Driver" in the drop-down list.  
	    Click the "Use" radio button next to "Cygnal C8051F 
	    Driver".

	5.) Click the "Load Application at Startup" check box.  This 
	    must be selected to enable the download of your program 
	    to on-chip Flash memory.  Selecting "Go till main()" is 
	    optional. However, if "Go till Main" is not selected, 
            and a single step in the 'C' source is used to start 
            program execution, the first single step will appear 
            to be very slow. This is due to the fact that the 
            startup code is being executed, which actually consists 
            of approximately 300 assembly language instructions being 
            single stepped through.

	6.) Close the "Options for Target" dialog and build the 
	    target program.

	7.) Select "Start/Stop Debug Session" from the "Debug" menu
	    to download your program to on-chip Flash program memory
	    and a begin a debug session.


CYGNAL C8051F uVISION2 DEBUG DRIVER REVISION HISTORY
----------------------------------------------------

Version 1.33
	An error which caused incorrect updates of the Port 1
	configuration register on the Port 1 peripheral dialog
	for F00x and F01x devices has been fixed.

Version 1.32
	Support has been added for F06X devices.

	An error preventing the Port 1 peripheral dialog from 
	displaying correctly on F00x and F01x devices has been fixed.

Version 1.31
	Support has been added for F04X devices.

        The trace record information of the on-chip debug logic 
        for devices in the 04X & 12X families is now supported.


Version 1.30
	A driver installation program has been implemented. 
	The following files are included in the installation:
	CygC8051F.dll - Copied into the C51\Bin directory of an 
		existing Keil installation. The tools.ini file
		will be edited so that the Cygnal C8051F Driver
		can be identified by uVision2.
	uv2.cdb - This is the standard database definition file 
		from Keil and is copied into the UV2 directory 
		only if it is a newer version than what is 
		already present in the existing Keil installation.
	tcyg.dll,dcyg.dll - DLL's provided by Keil which define the
		dialog peripherals used for Cygnal devices. They are
		copied into the C51\Bin directory only is they are
		newer than the existing versions.
	Readme_for_CygnalDLL.txt - The release notes for CygC8051F.dll. 
		This file is copied into the C51\Hlp directory of the
		Keil installation and contains information about using
		uVision with Cygnal devices and the revision history 
		for the DLL. 

	A problem that caused the DLL to hang when a breakpoint was
	set which would never be reached has been fixed.

Version 1.29
	Support has been added for F32X & F33X devices. The C8051F320/21/
        22/23 and C8051F330/31 devices have been added to the database.

        The DLL now supports the new uV2 flash interface. (Version 2.34
	of uV2 is required.)

	A problem which caused peripheral dialog values to display zeros 
	when first displayed has been fixed.

	The reset button is no longer enabled when the target is running.

	A program not responding error that occurred when uV2 was not the 
	active Windows application during flash downloads has been fixed.

	If the target board is disconnected while running the DLL will be
	shut down.

Version 1.28
	Support has been added for F31X devices. The C8051F310 and 
	C8051F311 devices have been added to the database.

        The cache data view for 12x devices has been implemented.


Version 1.27
	Support has been added for F12X devices.

	The following modifications were made to the database:
	The multi-device selections were changed to individual
	descriptions. The following devices were added to 
	the database: C8051F018 & C8051F019 
	C8051F120, C8051F121, C8051F122, C8051F123 
	C8051F124, C8051F125, C8051F126, C8051F127.  

	Scratch pad memory on the F02X & F12X devices is accessed 
	in the memory window using the 's' designator (s:0x0000). 
	uVision2 V2.20 or greater is required to view the scratch-pad
	memory space.

Version 1.26
	A problem with external memory accesses that could cause memory 
	window contents to wrap has been fixed.

Version 1.25
	Database & DLL updated to match new Keil release chip definitions.

	Support has been added for F018 & F019 devices.

	Settings dialog now includes a "Cache Code Memory" option.
	The default is enabled which provides a noticeable speed
 	improvement. However, the cache feature should not be used 
	if code is modifying flash memory contents, because updates 
	may not be reflected.

	The serial adapter firmware embedded in the DLL has been 
	updated to Version 0x0F.

Version 1.24
	The serial adapter firmware embedded in the DLL has been 
	updated to Version 0x0E.

Version 1.23
	The serial adapter firmware embedded in the DLL has been 
	updated to Version 0x0D.

Version 1.22
	Single stepping using a F30X device now correctly updates
	all values on the 'Regs' tab of the Project window. 

Version 1.21
	Support for the scratch pad memory on the F02X devices has
	been implemented. The scratch pad memory is accessed in the
	memory window using the 'c' designator (c:0x10000), with an 
	address in the range of 0x10000 to 0x10080. Memory window
	content modification problem has been fixed. Incorrect PSW 
	value in the Regs tab of the Project Window has been fixed. 

Version 1.2
	Support has been added for F02X & F30X devices.
        Flash programming has been enhanced. The download algorithm
	has been enhanced to improve speed. When flash is being 
        programmed, a status message and progress indication has been
        added. DPTR value is now correctly displayed. Modifications
        to the code space are now updated during step operations.
	A progress dialog was added when a firmware upgrade is 
	performed.

Version 1.1
	External memory modifications typed into a memory window are 
	now persistant. The direct memory accesses in a memory window 
	(d:0x00 - d:0xFF) now are correctly interpreted as follows: 
	0x00 - 0x7f (RAM), 0x80 - 0xFF (SFR).

Version 1.0
	Initial release.
	



