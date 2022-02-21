LabWindows/CVI 4.0.1 Patch 1 for Windows 95/NT
----------------------------------------------
This directory contains the following patch files:
  Patch1 for IDE and Run-time Engine
  ----------------------------------
      disk1.zip          (Disk 1 of 2)
      disk2.zip          (Disk 2 of 2)

  Note: To install on a harddrive, expand zip files as follows:
        disk1.zip in a subdirectory called \disk1\
        disk2.zip in a subdirectory called \disk2\

  Replacement for Run-time Engine Installation Diskette
  -----------------------------------------------------
      RTE-Disk.zip       (Disk 1 of 1)


-------------------------------------------------------------------------------
       LabWindows/CVI for Windows 3.1, 95, and NT, Version 4.0.1
                      Patch 1 Documentation File
       (c) Copyright 1993-1996 National Instruments Corporation.
                         All rights reserved.

           Product and company names listed are trademarks
            or trade names of their respective companies.


This patch contains bug fixes to LabWindows/CVI for Windows 3.1, Version 4.0.1,
or to LabWindows/CVI for Windows 95 and NT, Version 4.0.1.   The patch modifies
both the LabWindows/CVI environment, and the LabWindows/CVI run-time engine.
If only the run-time engine is installed, the patch modifies it.  
Alternatively, there is a disk image of the patched run-time engine that you 
can use in place of the original run-time engine installation diskette.

After applying this patch to LabWindows/CVI for Windows 95 and NT, you can 
still use the orginal installation program to change the compiler compatibility 
mode.  After doing so, you must apply this patch again. 

Note:  The patch can take up to 15 minutes to execute on slower machines.

This patch contains the following bug fixes:


CVI Run-Time Engine
-------------------
          
    - The CVI run-time engine was crashing on Windows NT 4.0 
      when the executable using the run-time engine was statically linked 
      to MSVCRT40.DLL, MFC40.DLL, or WSOCK32.DLL
      
    - CVI run-time engine on Windows 95 and NT was causing a floating 
      point exception (10h) when used with an executable created in 
      Borland's compiler.  Now, calling InitCVIRTE in the Borland 
      executable will prevent this from occurring.


Creating DLLs on Windows 95/NT
------------------------------

    - The Windows 95 DLL loader was crashing on some DLLs created in CVI.
      (This problem did not occur on Windows NT.)

    - Generating DLL import libraries from a header file and a DLL
      would sometimes cause an unrecoverable internal error in CVI if
      the header file contained multiple prototypes for a function in
      the DLL.

    - The CVI linker was reporting an "unresolved reference" error
      when, in the process of creating an executable or DLL, the 
      following occurred:
          * you explicitly forced a library to be fully linked into  
            the executable or DLL, 
          * the library referenced a module in another library that 
            was in the project or the Instrument or Library menu, and 
          * that module otherwise would not have been linked in.


CVI Compiler
------------

    - CVI was giving a missing prototype error if a function had a 
      calling convention qualifier and the function was defined using
      the old-style syntax for the function header.  Example:

         int __stdcall f(int i);
         int __stdcall f(i)
         int i;
         {
         }

    - The preprocessor directive #undef did not affect macros passed
      as arguments to other macros.
      
    - CVI reported a compile error when it encountered two declarations for 
      a function that had argument types defined by a typedef and only 
      one of the declarations contained a full prototype.

    - When CVI offered to add a missing header file, it added it to the top
      of the source file, even when windows.h was also included in the source
      file.  Since CVI include files must appear after windows.h, this could
      cause additional compile errors.  Now, CVI adds the file after the 
      include statement for windows.h.

    - Incorrect code would be generated for Integer division and 
      integer modulus when used in complex expressions, in very rare cases.


CVI Linker
----------

    - If your CVI project contained object files created in Microsoft
      Visual C, and two or more of these object files contained
      identical string literals, CVI was incorrectly reporting a multiple 
      symbol definition when linking the project.


Runtime Errors
--------------

    - CVI was reporting erroneous runtime errors for certain expressions
      when a program used calloc() to allocate a structure containing an
      array of pointers and then casted the result to two different types
      of pointers.


The realloc Function
--------------------

    - When running in debugging mode and repeatedly using the 
      realloc function on an array that contained pointers, 
      the pointer values in the array could be corrupted when the
      realloc function moved the array.

      
User Interface Canvas Controls
------------------------------

    - Certain actions, when taking place within a CanvasStartBatchDraw/
      CanvasEndBatchDraw sequence, resulted in the contents of the
      canvas possibly being lost or corrupted.  These actions
      included resizing the canvas, obtaining the contents of the bitmap
      (via GetCtrlDisplayBitmap or GetCtrlBitmap), and duplicating the
      canvas (via DuplicateCtrl). These actions are now safe inside a
      canvas batch.
      
    - A leak was fixed which was causing memory to be lost after
      discarding a canvas control.

      
User Interface Graph Controls
------------------------------

    - Duplicating a graph control containing a bitmap plot, followed by
      discarding one of the graphs, was causing the remaining graph to
      lose its copy of the bitmap plot.

    - Calling SetCtrlBitmap or SetImageBits on a bitmap plot of a graph
      could have caused a crash if the plot had not yet been drawn.

    - SetGraphCursorIndex was sometimes placing the cursor in unexpected
      locations if the range of the left y axis was different from that
      of the right y axis.
      
    - Shrinking a graph to its smallest possible height (either
      programmatically or in the UI Editor) would cause an internal error
      if the left or right y axis name was non-empty.

    - The algorithm for calculating the size of graph popup panels
      was changed so that they would look better on screens of smaller
      resolution.
      

User Interface Listbox Controls
-------------------------------

    - Operating listbox checkmarks when a listbox was transparent was
      impossible:  checkmarked items would not allow themselves to be
      unchecked.
      
    - InsertListItem and ReplaceListItem are no longer restricted to
      strings of 255 characters or less.


Other User Interface Library Issues
-----------------------------------      

    - 16-bit (65536 colors) .BMP files were not supported.

    - Discarding a panel on an EVENT_GOT_FOCUS was causing a crash.

    - Discarding a panel in a panel callback on any event was
      sometimes causing a crash.
    
    - Breakpointing inside a control callback after an EVENT_VAL_CHANGED
      was causing a second EVENT_VAL_CHANGED to be sent to the callback.
      
    - Editing the label of a command button in the UI editor by using the
      text tool could sometimes cause an crash.


Easyio I/O for DAQ Library
--------------------------
      
    - The Easy I/O for DAQ Library function AIClearAcquisition was failing
      to deallocate some data acquisition resources when it returned an
      error.
  
    - The Easy I/O for DAQ Library was failing to deallocate some Windows 
      resources.  Programs that used Easy I/O functions for an extended 
      period of time would eventually exhaust Windows resources.
      
    - The sample program easyio/ai_async.c was in some cases failing to call
      AIClearAcquisition after calling AIStartAcquisition, resulting in a 
      loss of memory. 


RS-232 Library
--------------
      
    - Fixed a problem in the RS-232 Library that occurred when using 
      InstallComCallback to uninstall a COM callback.  InstallComCallback 
      incorrectly terminated a thread that monitors the RS232 COM port 
      for events, thereby resulting in a loss of memory.  The loss of memory
      could occur if you repeatedly run a program using RS232 callbacks
      in the LabWindows/CVI development environment.

    - Fixed a problem in the RS-232 Library that occurred when opening and 
      writing to a COM port a second time.  If a COM port was closed
      while data still existed in its output queue, any subsequent open and 
      write to the COM port resulted in an erronious error of -1 when 
      executing a ComWrt.

    - Fixed a problem in the RS-232 Library on Windows 3.1 in which
      OpenComConfig fails with -1 if two programs open different
      RS232 ports at the same time.


VxD and Kernel Driver Templates
-------------------------------

   - Moved the asynchronous callback into callback.c because it 
     must be compiled without debugging.

   - Added RunStateChange callbacks to support debugging.

   - Fixed two problems in the win95 VxD source code.

   - Added template.h to define the control codes in a single place.

   - Added .prj files for the templates


Analysis Libraries for Windows 3.1      
----------------------------------

    - The Advanced Analysis Library for Windows 3.1 was missing 11 new
      functions.  

    - The functions Sort and Median would crash for certain input
      values on Windows 3.1.
      

Toolbox
-------      

    - Inserting items into a list created using the toolbox instrument 
      driver has been made faster.


Instrument Drivers
------------------

    - CVI was not always using the correct path for the VXIplug&play
      LIB directory when the compiler compatibility mode was other
      than Visual C/C++.  

    - Updated the VISA constants that are added to .ODL files and
      type libraries generated for instrument drivers using the VISA
      Library.

    - The .ODL file generator was not correctly converting data types
      containing "unsigned char".
       

Function Panels
---------------

    - CVI could crash if more than 40 function panel (.lfp or .fp) files 
      were loaded into the Libraries and Instruments menus.


Watch Window
------------

    - Fixed a bug which caused watch expressions to report incorrect
      values in situations in which they were repeatedly deleted and
      inserted while suspended in a user program.


      
      



