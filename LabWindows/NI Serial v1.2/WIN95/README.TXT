National Instruments Serial Software for Windows 95 for the
AT-232, AT-485, PCMCIA-232 and PCMCIA-485
Version 1.2
Copyright 1996 National Instruments Corporation.
All Rights Reserved.


Thank you for purchasing this Serial interface board and the NI Serial
software for Microsoft Windows 95.


WARNING: For users of PCMCIA-232 or PCMCIA-485 cards in Toshiba's
Satellite Pro 420CDS or 2150CDS notebook
----------------------------------------------------------------------------
We have noticed that there is a device on the notebook at I/O address
1F8H that only decodes the lower 10 address lines, which Windows 95 does 
not recognize. If any of our serial ports are configured at base I/O address
1F8h, 5F8h, 9F8h, or DF8h, one of two things happens: the computer
locks up when the card is inserted or the diagnostic fails.

To avoid this problem, complete the following steps to reserve these address
spaces BEFORE you insert your card.
1. Select Start>>Settings>>Control Panel.
2. Double click on the System icon.
3. In the System Properties dialog box that appears select the 
   Device Manager tab.
4. Double-click on the Computer icon at the top of the Device Manager
   list of devices.
5. Click on the Reserve Resources tab.
6. Select the Input/Output (I/O) button.
7. Enter each of the following address ranges by selecting the Add button
   for each.
	Start Value	End Value
	01F8		01FF	
	05F8		05FF
	09F8		09FF
	0DF8		0DFF
8. After you have entered these values and selected OK, restart Windows 95
   so that it can reserve these resources.		
9. You can now insert your card.

We appreciate communicating with the people who use our products.  We are
also very interested in hearing about the applications you develop using
our products.  To make it easy for you to communicate with us, Appendix E
in the "Getting Started with Your Serial Hardware and Software for 
Windows 95" manual contains forms for you to complete before contacting us.



