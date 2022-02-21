LabWindows/CVI 4.0.1 Patch 2 for Windows 95/NT
----------------------------------------------
This directory contains the following patch files:
  Patch2 for IDE and Run-time Engine
  ----------------------------------
      disk1.zip          (Disk 1 of 1)

  Replacement for Run-time Engine Installation Diskette
  -------------------------------------------------------
      RTE-Disk.zip       (Disk 1 of 1)


-------------------------------------------------------------------------------
 
       LabWindows/CVI for Windows 3.1, 95, and NT, Version 4.0.1
                      Patch 2 Documentation File
       (c) Copyright 1993-1997 National Instruments Corporation.
                         All rights reserved.

           Product and company names listed are trademarks
            or trade names of their respective companies.


This patch contains bug fixes to LabWindows/CVI for Windows 3.1, Version 4.0.1,
or to LabWindows/CVI for Windows 95 and NT, Version 4.0.1.   

WARNING:  To use this patch (Patch 2), you must first have applied Patch 1.

The patch modifies both the LabWindows/CVI environment and the 
LabWindows/CVI run-time engine.  If only the run-time engine is installed, 
the patch modifies it.  Alternatively, there is a disk image of the 
patched run-time engine that you can use in place of the original 
run-time engine installation diskette.

After applying this patch to LabWindows/CVI for Windows 95 and NT, you can 
still use the orginal installation program to change the compiler 
compatibility mode.  After doing so you must apply Patch 1 again,
but you do not need to apply this patch (Patch 2).   (Note:  When running 
Patch 1 again, you will see two error messages.  Ignore them.)

Note:  The patch can take up to 15 minutes to execute on slower machines.

This patch contains the following bug fixes:


CVI Run-Time Engine
-------------------
          
    - In rare cases on Windows NT, standalone executables could not run
      because they were unable to load the CVI run-time engine.

      
CVI Linker
-----------

    - The CVI linker was taking an extremely long time when linking
      object files that contained very deeply nested structures.

    - On Windows 95 and NT, the CVI linker was sometimes reporting a
      "Bad COFF library member header" error when trying to link a 
      DLL import library created in Visual C.  Generally, this occurred 
      only on import libraries created in Visual C version 2.0.


Creating Object Files Compatible with Borland/Watcom/Symantec
-------------------------------------------------------------

    - CVI was hanging when attempting to create an object file
      compatible with the Borland, Watcom, or Symantec compiler, 
      if the last global variable declared in the source file was
      initialized to point to another global variable.  Example:

          int  x;
          int  *px = x;


Variable Display
----------------

    - Modifying elements in an array of unsigned shorts or unsigned chars 
      in the Variable Display was causing the immediately subsequent array
      elements to be modified also.


Create Distribution Kit
------------------------

    - The Create Distribution Kit utility was failing when the destination
      drive was larger than 2 megabytes.

    - Distribution kits created with CVI 4.0.1, Patch 1, were not 
      overwriting the CVI 4.0.1 run-time engine with the patched version.


ANSI C Library
--------------

    - The scanf, sscanf, and fscanf functions, when processing the "%s"
      type specifier, were not recognizing characters in the range
      0x80 to 0xFF as belonging in the target string. 

    - The run-time checking for the malloc, calloc, realloc, and free
      functions was sometimes failing to report errors when the 
      dynamic memory heap was corrupted.


Utility Library
---------------

    - The GetExternalModuleAddr and RunExternalModule functions were
      taking an unnecessarily long time when called from a standalone
      executable.


RS232 Library
-------------

    - The RS232 Library had a maximum of 32 ports.  This has been 
      increased to 132.

    - On Windows 3.1, the OpenComConfig function was erroneously 
      reporting an "invalid baud rate" error when passed a baud 
      rate of 115200. 

    - The ComSetEscape function was erroneously reporting an error 
      when the GETMAXCOM option was used (Windows 3.1 only).
    

User Interface Library
----------------------

    - The PlotScaledIntensity function was sometimes drawing the wrong
      colors when the number of X points was greater than the number 
      of Y points.

    - The GetImageBits and GetBitmapData functions were writing 
      beyond the end of the mask buffer parameter.  This could
      cause corrupted dynamic memory and/or crashes.

    - The SavePanelState function was sometimes erroneously reporting
      an error or crashing when the target file did not already
      exist on disk.

    - Calls to DuplicateCtrl on a graph control (or DuplicatePanel
      on a panel containing a graph control) were sometimes failing
      and returning an error in cases where the graph contained a 
      bitmap or intensity plot.

    


