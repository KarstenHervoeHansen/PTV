/************************************************************************/
/*                                                                      */
/*           National Instruments NI-VISA(TM) Software Interface        */
/*            Copyright 1996, National Instruments Corporation.         */
/*                          All rights reserved.                        */
/*                                                                      */
/*             README.TXT file for NI-VISA for Windows 95/NT            */
/*                                                                      */
/************************************************************************/

IMPORTANT NOTES:
----------------

1) This is the VISA 1.0.3 release build 0x00100003.

2) This version of NI-VISA supports the following development environments:
   - LabVIEW 4.x
   - LabWindows/CVI 4.x
   - Microsoft Visual C/C++ 2.x, 4.x
   - Borland C/C++ 4.5.x
   - Microsoft VisualBasic 4.0

3) Users who want to use the viPeekXX/viPokeXX macros for platforms on which
   the macros are available should do the following:
		
   - Rename the current "visa.h" to "visaspec.h".
   - Rename "nivisa._h" in the include directory to "visa.h".
   - Define a macro called NIVISA_PEEKPOKE before including "visa.h".

   Please note that the macros are currently available only on the following
   platforms:

   - Win32 (All C compilers)
   - Win16 (Microsoft and Borland C compilers only)
   - Solaris (LabWindows/CVI only)

4) When compiling C source code, you may need to tell your compiler where to
   find the VISA header files and libraries.  For LabWindows/CVI, VISA
   support is built in, so the location of the VISA files is already known.

   For Microsoft Visual C/C++, you can add the following entries to your
   AUTOEXEC.BAT file.  This assumes a Windows 95 installation into C:\VXIPNP:

   SET INCLUDE=%INCLUDE%;C:\VXIPNP\WIN95\INCLUDE
   SET LIB=%LIB%;C:\VXIPNP\WIN95\LIB\MSC

   Alternatively, in the Microsoft Visual C/C++ environment, you can add the
   above directories to your project by selecting the Project|Settings menu
   option and changing the directory entries in the 'C/C++' and 'Link' tabs.

   For Borland C/C++, you can add the directories to your project by choosing
   the Options|Project menu option and modifying the 'Directories' topic.

   For other compilers, refer to the documentation that came with your
   compiler package for detailed information about how to inform the compiler
   and linker about the VISA header files and libraries in the VXIplug&play
   directory tree.


KNOWN PROBLEMS:
---------------

1) If you are using NI-VISA with the NI-VXI Upgrade for Windows 95, and
   your 32-bit application calls viRead or viScanf when the device does
   not have data ready, the system may hang rather than time out.  This
   problem should not occur when running 16-bit applications under
   Windows 95.  If this occurs, contact National Instruments for a newer
   version or patch of the NI-VXI driver.

2) If you are running 16-bit VISA applications on a system with a 32-bit
   native NI-488.2 driver, when using GPIB or GPIB-VXI, the current
   release of NI-VISA does not report events (VI_EVENT_SERVICE_REQ and
   VI_EVENT_IO_COMPLETION).

3) If you are using the GPIB-VXI controller and a viRead or viScanf
   times out even though the device has data ready, the next call to
   viRead or viScanf may return the data that should have been returned
   the previous time in addition to any new data that the device has ready.

4) This release of NI-VISA does not support the GPIB-232CT-A controller.

5) If you are using the VISA Interactive Control, you may see the message
   "A Run-State Change Callback caused a protection fault" upon exiting.
   You can safely ignore this message, as Win32 recovers correctly.

6) If you call viOpen or viFindRsrc for any GPIB or GPIB-VXI device, the
   NI-VISA driver will send a "*IDN?" to any device at primary address 1
   to determine if it is a GPIB-VXI controller.  If you have a device at
   primary address 1 that does not respond to "*IDN?", you must change
   its address.
   

PROBLEMS FIXED SINCE RELEASE 1.0:
---------------------------------

1) In release 1.0, if you were enabled for a service request event when
   using GPIB or GPIB-VXI, were using asynchronous I/O, and the SRQ line
   was asserted while a transfer was in progress, the system would crash.

2) When installing from floppy disk, release 1.0 had a problem when reading
   from the second disk.

3) Invalid NULL user buffer pointers are now trapped by the VISA library
   and return a new error code, 0xBFFF0071.

4) Zero-byte reads and writes for GPIB or GPIB-VXI transfers in release 1.0
   caused a system crash in Windows NT.  This is a valid request and now
   returns successfully.

5) In release 1.0, switching event callbacks into suspended mode would not
   always take effect immediately if there were still some events in the
   callback queue.

6) In release 1.0, viOpen for a nonexistent GPIB or GPIB-VXI device would
   incorrectly give VI_SUCCESS.

7) Fixed GPIB-VXI problems with A24 accesses and block moves.

8) In release 1.0, the attribute value for Serial Stop Bits was not properly
   checked and could be set to some incorrect values.  The only valid values
   are 10 (for 1 stop bit) and 20 (for 2 stop bits).

9) The formatted I/O routines would occasionally drop data when the output
   buffer became almost full.

10) In release 1.0, the GPIB events (VI_EVENT_SERVICE_REQ and
    VI_EVENT_IO_COMPLETION) would not work for GPIB boards other than 0.

11) Sometimes viMapAddress would return an error but the window would still
    appear to be mapped.

12) Once an application's event queue became full, additional events could
    cause a protection fault in the VISA library.

13) In release 1.0, any GPIB-VXI had to appear at secondary address 0.  That
    address was being ignored but is now being properly used.

14) Fixed formatted I/O problems with unsigned modifiers such as %u and %x.

15) In release 1.0, the VISA Interactive Control used a beta library with an
    expiration date of September 1996.  This release will not expire.
